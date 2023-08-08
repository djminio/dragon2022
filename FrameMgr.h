// FrameMgr.h: interface for the CFrameMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAMEMGR_H__5521928D_1621_4CA5_B2CE_501D72B875F5__INCLUDED_)
#define AFX_FRAMEMGR_H__5521928D_1621_4CA5_B2CE_501D72B875F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFrameMgr  : public TSingleton<CFrameMgr>
{
	enum eFrameDelay
	{
		FRAME_DELAY = 60,//60�� ���ӿ� ���̴� ���Դϴ�.
	};
public:
	CFrameMgr();
	virtual ~CFrameMgr();
	
public:
	
	__int32* MakeINT32Memory();
	void FreeMemories();
	bool CanNextFrame();//���� ���������� �� �� �ִ��� üũ �մϴ�. true�̸� ���� ���������� �� �� �ֽ��ϴ�.
	void DoFrameDelay(char** pTemp);//�� �����ӿ��� �� �� �ִ� ��� ���� ���� �Ǿ����� ���� �������� ���ؼ� ����� �Ǵ�.
	

	__int32 GetFrameCount();
	void	IncFrameCount();//��������
	void	InitTime();

private://������ �����̿� ����
	static DWORD* m_pdwSkipDelay;
	static int* m_pnIsSkiped;
	static int* m_piSkipCount;
	static __int32* m_pnFrameDelay;
	static DWORD* m_pdwFrameCheckTime;//������ üŷ �ð�
	static DWORD* m_pdwPrevFrameCheckTime;//���� ������ üŷ �ð�
	static DWORD* m_pdwTargetTime;
private://������ ī��Ʈ�� ����
	DWORD* MakeDwordMemory();
	int* GetFrameDelayBuffer(); // CSD-030804
	int m_iNowFrame; 
	int m_iFrameCounter; 
	
};

extern CFrameMgr g_FrameMgr;
#define g_pFrameMgr		CFrameMgr::Pointer()	//frame manager
#endif // !defined(AFX_FRAMEMGR_H__5521928D_1621_4CA5_B2CE_501D72B875F5__INCLUDED_)
