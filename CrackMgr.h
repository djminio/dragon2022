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
	void RecvCheckAccel(t_server_check_accel *packet);//�ޱ�
	void AutoCheckAccel();//������ //�ڵ����� �ð� ��� �ؼ�

	//CPU Clock ����
	__int64 GetAverageClock();//CPU�� ��� Ŭ���� ���ϱ�
	void SetInitClock();//CPU�� ���� �ʱⰪ ���ϱ�
	void __stdcall GetNowClock(CLOCK* pClock);//���� Ŭ����

	//������ ����.
	bool StartCrackThread();//ũ�� üũ ������ �����ϰ� �ϱ�
	bool StopCrackThread();//ũ�� üũ ������ ���߰��ϱ�
	bool IsCrackThreadRun();//ũ�� üũ ������ ���߰��ϱ�
	DWORD GetThreadApplyGab(){return m_dwThreadApplyGab;}
	void SetThreadApplyGab(const DWORD dwValue){m_dwThreadApplyGab = dwValue;}
	bool GetCTRun()const { return m_bIsCTRun;}
	void SetCTRun(const bool bIsRun){ m_bIsCTRun = bIsRun;}
private:
	t_server_check_accel RecvPacketSave;//���� ��Ŷ ���� ����
	DWORD	dwRecommandTime;//���� �ؾ� �� �ð�
	DWORD	dwRecvedTime;//���� �ؾ� �� �ð�
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
