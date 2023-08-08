// FrameMgr.cpp: implementation of the CFrameMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrameMgr.h"
#include "Hong_Sub.h"
#include "dragon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char szCheckFileName[MAX_PATH] = "./CrackCheckTrap/Trap.txt";

CFrameMgr g_FrameMgr;

DWORD* CFrameMgr::m_pdwFrameCheckTime = NULL;
DWORD* CFrameMgr::m_pdwPrevFrameCheckTime = NULL;
DWORD* CFrameMgr::m_pdwTargetTime = NULL;
__int32* CFrameMgr::m_pnFrameDelay = NULL;
int*	CFrameMgr::m_piSkipCount = NULL;
int*	CFrameMgr::m_pnIsSkiped = NULL;
DWORD*	CFrameMgr::m_pdwSkipDelay = NULL;


CFrameMgr::CFrameMgr()
{
	InitTime();
}

CFrameMgr::~CFrameMgr()
{
	InitTime();
	FreeMemories();
}

void CFrameMgr::InitTime()//시간 초기화
{
	SAFE_DELETE(m_pnFrameDelay);
	SAFE_DELETE(m_pdwFrameCheckTime);
	SAFE_DELETE(m_pdwPrevFrameCheckTime);
	SAFE_DELETE(m_pdwTargetTime);
	SAFE_DELETE(m_piSkipCount);
	SAFE_DELETE(m_pnIsSkiped);
	SAFE_DELETE(m_pdwSkipDelay);

	m_pnFrameDelay = this->MakeINT32Memory();*m_pnFrameDelay = 0;
	m_pdwFrameCheckTime	= this->MakeDwordMemory();
	m_pdwPrevFrameCheckTime = this->MakeDwordMemory();
	m_pdwTargetTime = this->MakeDwordMemory();
	m_piSkipCount = new int; *m_piSkipCount = 0;
	m_pnIsSkiped = new int; *m_pnIsSkiped = 0;
	m_pdwSkipDelay = this->MakeDwordMemory();*m_pdwSkipDelay = 0;


	*m_pdwFrameCheckTime = *m_pdwPrevFrameCheckTime = *m_pdwTargetTime = ::timeGetTime();

	m_iNowFrame = 0;
	m_iFrameCounter = 0;
}

bool CFrameMgr::CanNextFrame()
{	//< CSD-030804
	SAFE_DELETE(m_pdwFrameCheckTime);
	m_pdwFrameCheckTime	= this->MakeDwordMemory();
	*m_pdwFrameCheckTime	= ::timeGetTime();

	const DWORD dwTimeGab = *m_pdwFrameCheckTime - *m_pdwPrevFrameCheckTime;//이 자리가 호출 되는데 까지 걸린 시간.
	
	SAFE_DELETE(m_pdwPrevFrameCheckTime);
	m_pdwPrevFrameCheckTime = this->MakeDwordMemory();
	
	*m_pdwPrevFrameCheckTime = *m_pdwFrameCheckTime;	//지금 시간을 셋팅
	// 경우의 수. 프레임을 찍을때 기준보다 1. 길었다, 2.짧았다.
	// 스킵을 포함해서 프로시저는 1회 호출 타임이 평균 기준에 맞춰 줘야 한다.	
	
	// 1. 길다면
	//		한 프레임을 그린후 걸린 딜레이를 저장해 두고.
	//		스킵을 시키면서 1 프레임이 간것과 같이 딜레이를 줄여줍니다.
	//		그렇게 딜레이를 제거 하면서 - 값으로 돌아 섰을때
	//		다시 한프레임을 그려줍니다.
	
	if(*m_pnIsSkiped)
	{
		SAFE_DELETE(m_pdwSkipDelay);m_pdwSkipDelay = this->MakeDwordMemory();
		*m_pdwSkipDelay = dwTimeGab;
	}
	// 2. 짧다면.
	//		프레임이 끝나기 전에 DoFrameDelay를 걸어 프레임 타임을 지연 시킨다.
//COMMENT	char szMsg[ FILENAME_MAX]={0,}; 
	FILE *fp = NULL;

//COMMENT	sprintf( szMsg, "dwTimeGab = %d \n", dwTimeGab);
//COMMENT	OutputDebugString( szMsg );

//	static int nbabo;
	int* pnFRAME_DELAY	= this->GetFrameDelayBuffer();
	int* pnHACK_CHECK	= this->GetFrameDelayBuffer();
//	int pnFRAME_DELAY = FRAME_DELAY;
//	int s_iHackDelay = FRAME_DELAY;

		
	IncFrameCount();
	
	if(0 < *m_piSkipCount)
	{
//COMMENT	sprintf( szMsg, "Skip s_iSkipCount = %d \n", s_iSkipCount);
//COMMENT	OutputDebugString( szMsg );
		--(*m_piSkipCount);
		goto __SKIP_FRAME;
	}

	if(	*pnFRAME_DELAY != *pnHACK_CHECK)
	{
		ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
	}
	if(*m_pnFrameDelay >= *pnFRAME_DELAY)
	{
//COMMENT	sprintf( szMsg, "Skip m_nFrameDelay = %d \n", m_nFrameDelay);
//COMMENT	OutputDebugString( szMsg );
		*m_pnFrameDelay -= *pnFRAME_DELAY;
		goto __SKIP_FRAME;
	}
	
	if(	*pnFRAME_DELAY != FRAME_DELAY)
	{
		ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
	}
	if(*pnFRAME_DELAY < dwTimeGab)//프레임이 기준보다 길었습니다.
	{
		SAFE_DELETE(m_piSkipCount);m_piSkipCount  = new int;
		*m_piSkipCount = (dwTimeGab / *pnFRAME_DELAY)-1;//110->1 130->2
	 	*m_pnFrameDelay += dwTimeGab % *pnFRAME_DELAY;
	}

//COMMENT	sprintf( szMsg, "Frame Go %d\n", dwTimeGab);
//COMMENT	OutputDebugString( szMsg );
	goto __DO_FRAME;

__DO_FRAME://로그를 위해서 goto 를 사용합니다.
	{
		/*
		fp = Fopen(szCheckFileName,"at+");
		if(fp)
		{
			fprintf(fp,"Do Frame %d %d %d\n", dwTimeGab,*m_piSkipCount,*m_pnFrameDelay);
			::fclose(fp);	
		}
		*/
		*m_pnIsSkiped = 0;
		if(*m_pdwSkipDelay)
		{
			*m_pnFrameDelay += *m_pdwSkipDelay;
			SAFE_DELETE(m_pdwSkipDelay);m_pdwSkipDelay = this->MakeDwordMemory();
			*m_pdwSkipDelay = 0;
		}

		SAFE_DELETE(pnFRAME_DELAY);//soto
		SAFE_DELETE(pnHACK_CHECK);//soto

		return true;
	}

__SKIP_FRAME://로그를 위해서 goto 를 사용합니다.
	{
		/*
		fp = Fopen(szCheckFileName,"at+");
		if(fp)
		{
			fprintf(fp,"SKIP Frame %d %d %d\n", dwTimeGab,*m_piSkipCount,*m_pnFrameDelay);
			::fclose(fp);	
		}
		*/
		*m_pnIsSkiped = 1;
		if(*m_pdwSkipDelay)
		{
			*m_pnFrameDelay += *m_pdwSkipDelay;
			SAFE_DELETE(m_pdwSkipDelay);m_pdwSkipDelay = this->MakeDwordMemory();
			*m_pdwSkipDelay = 0;
		}

		SAFE_DELETE(pnFRAME_DELAY);//soto
		SAFE_DELETE(pnHACK_CHECK);//soto
		return false;
	}
}	//> CSD-030804

void CFrameMgr::DoFrameDelay(char** pTemp)
{//진행이 너무 빠르게 끝났다면 딜레이를 줌

	char	*pCheck = NULL;	
	const DWORD dwGab = ::timeGetTime() - *m_pdwFrameCheckTime;

		
	int*	pFrame_Delay = this->GetFrameDelayBuffer();
		
	if( *pFrame_Delay > dwGab )
	{
		int iForceDelay = (*pFrame_Delay - dwGab);
		if(iForceDelay < 0) iForceDelay = 0;
		
		*pTemp	= new char[10];
		pCheck	= new char[10];
		DWORD	dwSleepTimeCheck = timeGetTime();
		::Sleep(iForceDelay);

		if(iForceDelay > 0)
		{			
			if(timeGetTime() - dwSleepTimeCheck < iForceDelay)
			{
				//Sleep을 수행하지 않았다.뛰어 넘었다.
				ExitApplication(EA_CHECK_ACCEL);
			}
		}
	}
	else
	{
		int iForceDelay = (*pFrame_Delay - dwGab);
		if(iForceDelay < 0) iForceDelay = 0;
		
		*pTemp	= new char[10];
		pCheck	= new char[10];
		DWORD	dwSleepTimeCheck = timeGetTime();
		::Sleep(iForceDelay);

		if(iForceDelay > 0)
		{			
			if(timeGetTime() - dwSleepTimeCheck < iForceDelay)
			{
				//Sleep을 수행하지 않았다.뛰어 넘었다.
				ExitApplication(EA_CHECK_ACCEL);
			}
		}
	}
	
	delete pFrame_Delay;

	strcpy(pCheck,"c8");
	delete pCheck;
	
}

void CFrameMgr::IncFrameCount()//루핑이 호출 되는 머리에 붙여줍니다
{
	const DWORD dwNowTime = ::timeGetTime();

	if(dwNowTime > *m_pdwTargetTime)
	{
		m_iNowFrame = m_iFrameCounter;
		m_iFrameCounter = 1;//프레임 카운트 종료
		*m_pdwTargetTime = dwNowTime+1000;//1초 뒤로 셋팅
//COMMENT OutputDebugString( "1 Sec Over\n");
		/*
		FILE* fp = Fopen(szCheckFileName,"at+");
		if(fp)
		{
			fprintf(fp,"1 Sec Over\n");
			::fclose(fp);	
		}
		*/
	}
	else
	{
//COMMENT char szMsg[ FILENAME_MAX]={0,};
//COMMENT sprintf( szMsg, "Counter %d\n", m_iFrameCounter);
//COMMENT OutputDebugString( szMsg );
		++m_iFrameCounter;
	}

	return;
}

 __int32 CFrameMgr::GetFrameCount()
{
	return m_iNowFrame;
}
//soto-hack ??
int* CFrameMgr::GetFrameDelayBuffer()
{	//< CSD-030804
	int* pNumber = new int;
	*pNumber = 30*2;
	return pNumber;
}	//> CSD-030804

DWORD* CFrameMgr::MakeDwordMemory()
{
	return new DWORD;
}

void CFrameMgr::FreeMemories()
{
	SAFE_DELETE(m_pnFrameDelay);
	SAFE_DELETE(m_pdwFrameCheckTime);
	SAFE_DELETE(m_pdwPrevFrameCheckTime);
	SAFE_DELETE(m_pdwTargetTime);
	SAFE_DELETE(m_piSkipCount);
	SAFE_DELETE(m_pnIsSkiped);
	SAFE_DELETE(m_pdwSkipDelay);
}

__int32* CFrameMgr::MakeINT32Memory()
{
	return new __int32;
}
