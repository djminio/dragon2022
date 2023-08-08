// CrackMgr.h: interface for the CCrackMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRACKMGR_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_)
#define AFX_CRACKMGR_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int CMD_CHECK_ACCEL				=	6060;

typedef struct server_check_accel
{
	DWORD	dwCheckAccelNowtime;
} t_server_check_accel;

class CCrackMgr  
{
	typedef	struct CLOCK
	{
		DWORD	dwLow;
		DWORD	dwHi;
	};

public:
	CCrackMgr();
	virtual ~CCrackMgr();
	
	void InitCrackMgr();
	void RecvCheckAccel(t_server_check_accel *packet);//받기
	void AutoCheckAccel();//보내기 //자동으로 시간 계산 해서

	//CPU Clock 관리
	__int64 GetAverageClock();//CPU의 평균 클럭값 구하기
	void SetInitClock();//CPU의 시작 초기값 구하기
	void __stdcall GetNowClock(CLOCK* pClock);//현재 클럭값

	//스레드 관리.
	bool StartCrackThread();//크랙 체크 쓰레드 시작하게 하기
	bool StopCrackThread();//크랙 체크 쓰레드 멈추게하기
	bool IsCrackThreadRun();//크랙 체크 쓰레드 멈추게하기
	DWORD GetThreadApplyGab(){return m_dwThreadApplyGab;}
	void SetThreadApplyGab(const DWORD dwValue){m_dwThreadApplyGab = dwValue;}
	bool GetCTRun()const { return m_bIsCTRun;}
	void SetCTRun(const bool bIsRun){ m_bIsCTRun = bIsRun;}
private:
	t_server_check_accel RecvPacketSave;//받은 패킷 고대로 보관
	DWORD	dwRecommandTime;//응답 해야 할 시간
	DWORD	dwRecvedTime;//응답 해야 할 시간
	DWORD	dwRealTime;
	__int64	m_i64Clock;
	
	DWORD	m_dwThreadNo;
	bool	m_bIsCTRun;
	HANDLE	m_hCrackThread;
	DWORD	m_dwThreadApplyGab;
};
extern CCrackMgr CrackMgr;
extern unsigned __stdcall CrackCheckThread(LPVOID pValue);
extern BOOL DumpException(LPEXCEPTION_POINTERS lpExcep,char* szOutMsg);
#endif // !defined(AFX_CRACKMGR_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_)
