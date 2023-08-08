// CrackMgr.cpp: implementation of the CCrackMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CrackMgr.h"
#include "network.h"
#include "dragon.h"
#include "CurrentMsgMgr.h"
#include <Process.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int CHECK_ACCEL_RECOMMAND_TIME	=	30000; // 30초만에 응답 한다

CCrackMgr CrackMgr;
CCrackMgr::CCrackMgr()
{
	m_i64Clock = 0;
	m_hCrackThread = INVALID_HANDLE_VALUE;
	SetThreadApplyGab(0);
	SetCTRun(false);
}

CCrackMgr::~CCrackMgr()
{

}

void CCrackMgr::InitCrackMgr()
{
	::memset(&RecvPacketSave,0,sizeof(t_server_check_accel));//받은 패킷 고대로 보관
	dwRecommandTime = 0;//응답 해야 할 시간
	dwRealTime		= 0;
	dwRecvedTime	= 0;
}

void CCrackMgr::RecvCheckAccel(t_server_check_accel *packet)//받기
{
	dwRealTime = timeGetTime();
	RecvPacketSave = *packet;
	dwRecvedTime	= dwRealTime;//받은 시간 셋팅
	dwRecommandTime = dwRealTime + CHECK_ACCEL_RECOMMAND_TIME;
}

void CCrackMgr::AutoCheckAccel()//보내기 //자동으로 시간 계산 해서
{
//클라이언트 엑셀
	//첫 체크후 1초 뒤에 체크한다
	//체크 했을때 5% 이상의 오차가 나면 멈춰 버리자
/*	CLOCK cl;
	GetNowClock(&cl);
	static __int64 s_i64Clock = *(__int64*)&cl;
	static	DWORD s_dwPreTime = ::timeGetTime();
	const DWORD dwTimeGab = ::timeGetTime() - s_dwPreTime;
	if( dwTimeGab > 1000 )//1초가 지났다면
	{
		s_dwPreTime = ::timeGetTime();//시간 갱신

		CLOCK cl2;
		GetNowClock(&cl2);//현재 클럭
		
		__int64 i64Curr = *(__int64*)&cl2;
		__int64 i64Result = i64Curr - s_i64Clock;

		if(i64Result)//평균 재서 m_i64Clock와 얼마나 차이나는지 확인
		{
			const __int64 i64NowClock = i64Result*1000/dwTimeGab;
//			::AddCurrentStatusMessage(FONT_COLOR_RED,"%d",i64NowClock);
			if( i64NowClock )//5%정도로 끊으면 좋을까? .. -_-;..
			{
				if( m_i64Clock + (m_i64Clock*3/100) < i64NowClock //3% 허용 오차를 벗어나면
				||	m_i64Clock - (m_i64Clock*3/100) > i64NowClock )
				{
//					JustMsg("Your System have problem");
//					g_DBGLog.Log(LOG_LV1, "Your System have problem (%d)", m_i64Clock);//모드 셋팅 기록 될 파일의 고유 이름입니다.
//					g_DBGLog.Log(LOG_LV1, "Your System have problem2 (%d)", i64NowClock);//모드 셋팅 기록 될 파일의 고유 이름입니다.
//					::ExitApplication(EA_CPU_CLOCK_ERROR);
//					::exit(0);
				}
			}
		}
		GetNowClock(&cl);
		s_i64Clock = *(__int64*)&cl;//새로운 기준점 클럭을 셋팅
	}
*/

	if(!IsCrackThreadRun())
	{
		::ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
	}
//서버엑셀레이터
	if	((	!dwRecommandTime && dwRecvedTime)
		||(	dwRecommandTime && !dwRecvedTime))
	{//어느 한쪽만 셋팅이 될때
		ExitApplication(EA_CHECK_ACCEL);
	}
	const int iResult = (int)((int)dwRealTime-(int)dwRecommandTime);
	dwRealTime = timeGetTime();
	if(dwRecommandTime && dwRecvedTime)//응답 시간이 셋팅 되어 있다. 받은 시간이 셋팅 되어 있다
	{	//	if( dwRealTime > dwRecommandTime)//이게 더 크면
		if(	0 < iResult )
		{
			t_packet	packet;
			packet.h.header.type		= CMD_CHECK_ACCEL;
			packet.h.header.size	= sizeof( t_server_check_accel );
			memcpy(&packet.u.server_check_accel ,&RecvPacketSave,sizeof( t_server_check_accel ));//아까 받은거
			QueuePacket( &packet, 1 );
			InitCrackMgr();//초기화
		}
	}
}

void CCrackMgr::SetInitClock()
{
	m_i64Clock = GetAverageClock();//최초의 클럭값을 가져 옵니다.
}

__int64 CCrackMgr::GetAverageClock()
{
	CLOCK	cl ={0,};
	GetNowClock(&cl);
	__int64 prv = *(__int64*)&cl;

	Sleep(100);//1000 -> 100// 1초간의 클럭을 체크하자...
	
	GetNowClock(&cl);// 나중 클럭을 얻어서..
	__int64 cur = *(__int64*)&cl;

	return (cur - prv);// 빼면...		// 600MHz cpu라면...ex) 600698387, 
}

void __stdcall CCrackMgr::GetNowClock(CLOCK* pClock)
{
	__asm
	{
		mov			esi,dword ptr[pClock]
		rdtsc
		mov			dword ptr[esi],eax
		mov			dword ptr[esi+4],edx
	}
}

bool CCrackMgr::StartCrackThread()//크랙 체크 쓰레드 시작하게 하기
{//윈도우 버전을 체크하여 인정 될 수 있는 갭을 셋팅 합니다.
	const DWORD dwVersion = ::GetVersion();

	// Get the Windows version.
	const DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	const DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

	/*
	// Get the build number.
	if (dwVersion < 0x80000000)              // Windows NT/2000/XP
	{
//		dwBuild = (DWORD)(HIWORD(dwVersion));
		SetThreadApplyGab(500);
	}
	else if (dwWindowsMajorVersion < 4)      // Win32s
	{
//		dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
		SetThreadApplyGab(1500);
	}
	else  
	*/
	// Windows 95/98/Me
	{
//		dwBuild =  0;
		SetThreadApplyGab(2000);
	}


	m_hCrackThread = (HANDLE)::_beginthreadex(NULL,0, &CrackCheckThread, NULL,0,(unsigned int*)&m_dwThreadNo);
	
	int temp = GetThreadPriority(m_hCrackThread);
	
	const BOOL bIsOk = ::SetThreadPriority(m_hCrackThread,THREAD_PRIORITY_TIME_CRITICAL);
	const int ret = ::GetThreadPriority(m_hCrackThread);

	if(INVALID_HANDLE_VALUE == m_hCrackThread)
	{
		SetCTRun(false);
	}
	else
	{
		SetCTRun(true);
	}
	return (GetCTRun() && bIsOk);
}

bool CCrackMgr::StopCrackThread()//크랙 체크 쓰레드 멈추게하기
{
	if(IsCrackThreadRun())
	{
		SetCTRun(false);
		::CloseHandle(m_hCrackThread);
		m_hCrackThread = INVALID_HANDLE_VALUE;
		return true;
	}
	return false;
}

bool CCrackMgr::IsCrackThreadRun()//크랙 체크 쓰레드 멈추게하기
{
	if(m_hCrackThread != INVALID_HANDLE_VALUE //핸들이 박살 났거나
	|| GetCTRun())//플래그가 꺼진다면
	{
		return true;
	}
	return false;
}

unsigned __stdcall CrackCheckThread(LPVOID pValue)
{	//< CSD-030808
	// 031110 YGI
#ifdef _DEBUG	
	return 0;
#endif
	////////////////
	const int i1Sec = 1000;

	BOOL bTGT_SLP	= false;
	BOOL bGTC_SLP	= false;
	BOOL bTGT_GTC	= false;
	DWORD dwPrevTGT = 0, dwNowTGT = 0;
	DWORD dwPrevGTC = 0, dwNowGTC = 0;
	while(true)
	{
		if(!CrackMgr.IsCrackThreadRun())
		{
			::_endthread();
			::exit(0);
		}

		dwPrevTGT = ::timeGetTime();		
		::Sleep(100); //1000 -> 100
		dwNowTGT = ::timeGetTime();
		
		DWORD dwRetTGT = (dwNowTGT - dwPrevTGT);
		if(	i1Sec+CrackMgr.GetThreadApplyGab() < dwRetTGT)
		{
			g_DBGLog.Log(LOG_LV1, "Your System have problem Type(%d) ", dwRetTGT);//	,dwRetGTC);//모드 셋팅 기록 될 파일의 고유 이름입니다.
			::ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
			bTGT_SLP = true;
		}


		t_packet	p;
		p.h.header.type = CMD_SEND_ACCEL_TYPE;
		p.h.header.size = sizeof(t_ACCEL_TYPE);
		p.u.Accel_Type.nType = 0;//현재는 쓰이지 않는다.
		
		dwPrevGTC = ::GetTickCount();
		::Sleep(100); //1000 -> 100
		dwNowGTC = ::GetTickCount();

		DWORD dwRetGTC = (dwNowGTC - dwPrevGTC);

		if(	i1Sec+CrackMgr.GetThreadApplyGab() < dwRetGTC)
		{
			bGTC_SLP = true;
		}

		
		dwPrevGTC	 = GetTickCount();
		dwPrevTGT = ::timeGetTime();		
		while (GetTickCount() - dwPrevGTC < 1000)
		{	// 1초간 딜레이
			Sleep(1);
		}

		dwNowTGT = ::timeGetTime();
		dwRetTGT = dwNowTGT-dwPrevTGT;
		if(	i1Sec+CrackMgr.GetThreadApplyGab() < dwRetTGT)
		{
//			g_DBGLog.Log(LOG_LV1, "Your System have problem Type(%d) ", dwRetTGT);//	,dwRetGTC);//모드 셋팅 기록 될 파일의 고유 이름입니다.
//			::ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
			bTGT_GTC = true;
		}
		
		if(bTGT_SLP)//timeGetTime과 sleep에서 문제가 생겼다.
		{
			if(!bGTC_SLP)//GetTickCount 와 sleep문제는 없다.
			{
				if(bTGT_GTC)//timeGetTime과 GetTickCount에 문제가 생겼다.
				{
					//timeGetTime이 훅킹 당했다.
					strcpy(p.u.Accel_Type.strMsg,"timeGetTime");
					::QueuePacket(&p,1);//서버로 보낸다.
				}
			}
			else
			{
				if(bTGT_GTC)//timeGetTime과 GetTickCount에 문제가 생겼다.
				{
					//셋다 문제가 있다.
					strcpy(p.u.Accel_Type.strMsg,"timeGetTime and GetTickCount and Sleep");
					::QueuePacket(&p,1);//서버로 보낸다.
				}
				else
				{
					//Sleep이 문제다.
					strcpy(p.u.Accel_Type.strMsg,"Sleep");
					::QueuePacket(&p,1);//서버로 보낸다.
				}
			}
		}
		else if(bTGT_GTC)//timeGetTime과 GetTickCount가 문제가 생겼다.
		{
			if(bGTC_SLP)
			{
				//GetTickCount가 문제다.
				strcpy(p.u.Accel_Type.strMsg,"GetTickCount");
				::QueuePacket(&p,1);//서버로 보낸다.
			}
			else
			{
				//어느 문제인지 알수가 없다.
				strcpy(p.u.Accel_Type.strMsg,"Unknown");
				::QueuePacket(&p,1);//서버로 보낸다.

			}
			
		}
		else if(bGTC_SLP)
		{
			//어느 문제인지 알수가 없다.
			strcpy(p.u.Accel_Type.strMsg,"Unknown");
//			::QueuePacket(&p,1);//서버로 보낸다.
		}
		
	}
	CrackMgr.SetCTRun(false);
//#endif
	return 0;
}	//> CSD-030808

BOOL DumpException(LPEXCEPTION_POINTERS lpExcep,char* szOutMsg)
{
	if (!lpExcep)
		return FALSE;

	BOOL		result = TRUE;
	SYSTEMTIME time;

	const DWORD		dwExceptionAddress = (DWORD)lpExcep->ExceptionRecord->ExceptionAddress;
	const DWORD		dwExceptionCode = lpExcep->ExceptionRecord->ExceptionCode;
	const DWORD		dwExceptionFlags = lpExcep->ExceptionRecord->ExceptionFlags;
	const DWORD		dwNumberParameters = lpExcep->ExceptionRecord->NumberParameters;
	const DWORD		dwDr0 = lpExcep->ContextRecord->Dr0;
	const DWORD		dwDr1 = lpExcep->ContextRecord->Dr1;
	const DWORD		dwDr2 = lpExcep->ContextRecord->Dr2;
	const DWORD		dwDr3 = lpExcep->ContextRecord->Dr3;
	const DWORD		dwDr6 = lpExcep->ContextRecord->Dr6;
	const DWORD		dwDr7 = lpExcep->ContextRecord->Dr7;

	const DWORD		dwSegCs = lpExcep->ContextRecord->SegCs;

	const DWORD		dwSegGs = lpExcep->ContextRecord->SegGs;
	const DWORD		dwSegFs = lpExcep->ContextRecord->SegFs;
	const DWORD		dwSegEs = lpExcep->ContextRecord->SegEs;
	const DWORD		dwEFlags = lpExcep->ContextRecord->EFlags;

	const DWORD		dwEsi = lpExcep->ContextRecord->Esi;
	const DWORD		dwEdi = lpExcep->ContextRecord->Edi;
	const DWORD		dwEbp = lpExcep->ContextRecord->Ebp;
	const DWORD		dwEsp = lpExcep->ContextRecord->Esp;
	const DWORD		dwEip = lpExcep->ContextRecord->Eip;

	const DWORD		dwEax = lpExcep->ContextRecord->Eax;
	const DWORD		dwEbx = lpExcep->ContextRecord->Ebx;
	const DWORD		dwEcx = lpExcep->ContextRecord->Ecx;
	const DWORD		dwEdx = lpExcep->ContextRecord->Edx;
	__try
	{
		GetLocalTime(&time);
	
		g_CrashLog.Log(LOG_LV1,"%04d-%02d-%02d-%02d-%02d-%02d",
			time.wYear,time.wMonth,time.wDay,time.wMinute,time.wSecond);
		
		g_CrashLog.Log(LOG_LV1,szOutMsg);

		g_CrashLog.Log(LOG_LV1,"Crashed address %xh",dwExceptionAddress);

		g_CrashLog.Log(LOG_LV1,"Crashed Map Address %04X:%08X",
			dwSegCs,
			dwExceptionAddress);

		g_CrashLog.Log(LOG_LV1,"Exception Code %u",dwExceptionCode);
		g_CrashLog.Log(LOG_LV1,"Exception Flag %xh",dwExceptionFlags);
		g_CrashLog.Log(LOG_LV1,"NumberParameters %d",dwNumberParameters);
		g_CrashLog.Log(LOG_LV1,"Dr0 %xh",dwDr0);
		g_CrashLog.Log(LOG_LV1,"Dr1 %xh",dwDr1);
		g_CrashLog.Log(LOG_LV1,"Dr2 %xh",dwDr2);
		g_CrashLog.Log(LOG_LV1,"Dr3 %xh",dwDr3);
		g_CrashLog.Log(LOG_LV1,"Dr6 %xh",dwDr6);
		g_CrashLog.Log(LOG_LV1,"Dr7 %xh",dwDr7);
		
		g_CrashLog.Log(LOG_LV1,"SegGs %xh",dwSegGs);
		g_CrashLog.Log(LOG_LV1,"SegFs %xh",dwSegFs);
		g_CrashLog.Log(LOG_LV1,"SegEs %xh",dwSegEs);
		g_CrashLog.Log(LOG_LV1,"EFlags %xh",dwEFlags);
		
		g_CrashLog.Log(LOG_LV1,"Esi %xh",dwEsi);
		g_CrashLog.Log(LOG_LV1,"Edi %xh",dwEdi);
		g_CrashLog.Log(LOG_LV1,"Ebp %xh",dwEbp);
		g_CrashLog.Log(LOG_LV1,"Esp %xh",dwEsp);
		g_CrashLog.Log(LOG_LV1,"Eip %xh",dwEip);
		

		g_CrashLog.Log(LOG_LV1,"Eax %xh",dwEax);
		g_CrashLog.Log(LOG_LV1,"Ebx %xh",dwEbx);
		g_CrashLog.Log(LOG_LV1,"Ecx %xh",dwEcx);
		g_CrashLog.Log(LOG_LV1,"Edx %xh",dwEdx);

		g_CrashLog.Log(LOG_LV1,"Log Complete!");

	}
	__except (GetExceptionCode())
	{
		result = FALSE;
	}

	return result;
}
