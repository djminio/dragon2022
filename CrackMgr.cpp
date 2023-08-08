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

const int CHECK_ACCEL_RECOMMAND_TIME	=	30000; // 30�ʸ��� ���� �Ѵ�

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
	::memset(&RecvPacketSave,0,sizeof(t_server_check_accel));//���� ��Ŷ ���� ����
	dwRecommandTime = 0;//���� �ؾ� �� �ð�
	dwRealTime		= 0;
	dwRecvedTime	= 0;
}

void CCrackMgr::RecvCheckAccel(t_server_check_accel *packet)//�ޱ�
{
	dwRealTime = timeGetTime();
	RecvPacketSave = *packet;
	dwRecvedTime	= dwRealTime;//���� �ð� ����
	dwRecommandTime = dwRealTime + CHECK_ACCEL_RECOMMAND_TIME;
}

void CCrackMgr::AutoCheckAccel()//������ //�ڵ����� �ð� ��� �ؼ�
{
//Ŭ���̾�Ʈ ����
	//ù üũ�� 1�� �ڿ� üũ�Ѵ�
	//üũ ������ 5% �̻��� ������ ���� ���� ������
/*	CLOCK cl;
	GetNowClock(&cl);
	static __int64 s_i64Clock = *(__int64*)&cl;
	static	DWORD s_dwPreTime = ::timeGetTime();
	const DWORD dwTimeGab = ::timeGetTime() - s_dwPreTime;
	if( dwTimeGab > 1000 )//1�ʰ� �����ٸ�
	{
		s_dwPreTime = ::timeGetTime();//�ð� ����

		CLOCK cl2;
		GetNowClock(&cl2);//���� Ŭ��
		
		__int64 i64Curr = *(__int64*)&cl2;
		__int64 i64Result = i64Curr - s_i64Clock;

		if(i64Result)//��� �缭 m_i64Clock�� �󸶳� ���̳����� Ȯ��
		{
			const __int64 i64NowClock = i64Result*1000/dwTimeGab;
//			::AddCurrentStatusMessage(FONT_COLOR_RED,"%d",i64NowClock);
			if( i64NowClock )//5%������ ������ ������? .. -_-;..
			{
				if( m_i64Clock + (m_i64Clock*3/100) < i64NowClock //3% ��� ������ �����
				||	m_i64Clock - (m_i64Clock*3/100) > i64NowClock )
				{
//					JustMsg("Your System have problem");
//					g_DBGLog.Log(LOG_LV1, "Your System have problem (%d)", m_i64Clock);//��� ���� ��� �� ������ ���� �̸��Դϴ�.
//					g_DBGLog.Log(LOG_LV1, "Your System have problem2 (%d)", i64NowClock);//��� ���� ��� �� ������ ���� �̸��Դϴ�.
//					::ExitApplication(EA_CPU_CLOCK_ERROR);
//					::exit(0);
				}
			}
		}
		GetNowClock(&cl);
		s_i64Clock = *(__int64*)&cl;//���ο� ������ Ŭ���� ����
	}
*/

	if(!IsCrackThreadRun())
	{
		::ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
	}
//��������������
	if	((	!dwRecommandTime && dwRecvedTime)
		||(	dwRecommandTime && !dwRecvedTime))
	{//��� ���ʸ� ������ �ɶ�
		ExitApplication(EA_CHECK_ACCEL);
	}
	const int iResult = (int)((int)dwRealTime-(int)dwRecommandTime);
	dwRealTime = timeGetTime();
	if(dwRecommandTime && dwRecvedTime)//���� �ð��� ���� �Ǿ� �ִ�. ���� �ð��� ���� �Ǿ� �ִ�
	{	//	if( dwRealTime > dwRecommandTime)//�̰� �� ũ��
		if(	0 < iResult )
		{
			t_packet	packet;
			packet.h.header.type		= CMD_CHECK_ACCEL;
			packet.h.header.size	= sizeof( t_server_check_accel );
			memcpy(&packet.u.server_check_accel ,&RecvPacketSave,sizeof( t_server_check_accel ));//�Ʊ� ������
			QueuePacket( &packet, 1 );
			InitCrackMgr();//�ʱ�ȭ
		}
	}
}

void CCrackMgr::SetInitClock()
{
	m_i64Clock = GetAverageClock();//������ Ŭ������ ���� �ɴϴ�.
}

__int64 CCrackMgr::GetAverageClock()
{
	CLOCK	cl ={0,};
	GetNowClock(&cl);
	__int64 prv = *(__int64*)&cl;

	Sleep(100);//1000 -> 100// 1�ʰ��� Ŭ���� üũ����...
	
	GetNowClock(&cl);// ���� Ŭ���� ��..
	__int64 cur = *(__int64*)&cl;

	return (cur - prv);// ����...		// 600MHz cpu���...ex) 600698387, 
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

bool CCrackMgr::StartCrackThread()//ũ�� üũ ������ �����ϰ� �ϱ�
{//������ ������ üũ�Ͽ� ���� �� �� �ִ� ���� ���� �մϴ�.
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

bool CCrackMgr::StopCrackThread()//ũ�� üũ ������ ���߰��ϱ�
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

bool CCrackMgr::IsCrackThreadRun()//ũ�� üũ ������ ���߰��ϱ�
{
	if(m_hCrackThread != INVALID_HANDLE_VALUE //�ڵ��� �ڻ� ���ų�
	|| GetCTRun())//�÷��װ� �����ٸ�
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
			g_DBGLog.Log(LOG_LV1, "Your System have problem Type(%d) ", dwRetTGT);//	,dwRetGTC);//��� ���� ��� �� ������ ���� �̸��Դϴ�.
			::ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
			bTGT_SLP = true;
		}


		t_packet	p;
		p.h.header.type = CMD_SEND_ACCEL_TYPE;
		p.h.header.size = sizeof(t_ACCEL_TYPE);
		p.u.Accel_Type.nType = 0;//����� ������ �ʴ´�.
		
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
		{	// 1�ʰ� ������
			Sleep(1);
		}

		dwNowTGT = ::timeGetTime();
		dwRetTGT = dwNowTGT-dwPrevTGT;
		if(	i1Sec+CrackMgr.GetThreadApplyGab() < dwRetTGT)
		{
//			g_DBGLog.Log(LOG_LV1, "Your System have problem Type(%d) ", dwRetTGT);//	,dwRetGTC);//��� ���� ��� �� ������ ���� �̸��Դϴ�.
//			::ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
			bTGT_GTC = true;
		}
		
		if(bTGT_SLP)//timeGetTime�� sleep���� ������ �����.
		{
			if(!bGTC_SLP)//GetTickCount �� sleep������ ����.
			{
				if(bTGT_GTC)//timeGetTime�� GetTickCount�� ������ �����.
				{
					//timeGetTime�� ��ŷ ���ߴ�.
					strcpy(p.u.Accel_Type.strMsg,"timeGetTime");
					::QueuePacket(&p,1);//������ ������.
				}
			}
			else
			{
				if(bTGT_GTC)//timeGetTime�� GetTickCount�� ������ �����.
				{
					//�´� ������ �ִ�.
					strcpy(p.u.Accel_Type.strMsg,"timeGetTime and GetTickCount and Sleep");
					::QueuePacket(&p,1);//������ ������.
				}
				else
				{
					//Sleep�� ������.
					strcpy(p.u.Accel_Type.strMsg,"Sleep");
					::QueuePacket(&p,1);//������ ������.
				}
			}
		}
		else if(bTGT_GTC)//timeGetTime�� GetTickCount�� ������ �����.
		{
			if(bGTC_SLP)
			{
				//GetTickCount�� ������.
				strcpy(p.u.Accel_Type.strMsg,"GetTickCount");
				::QueuePacket(&p,1);//������ ������.
			}
			else
			{
				//��� �������� �˼��� ����.
				strcpy(p.u.Accel_Type.strMsg,"Unknown");
				::QueuePacket(&p,1);//������ ������.

			}
			
		}
		else if(bGTC_SLP)
		{
			//��� �������� �˼��� ����.
			strcpy(p.u.Accel_Type.strMsg,"Unknown");
//			::QueuePacket(&p,1);//������ ������.
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
