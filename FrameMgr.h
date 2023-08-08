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
		FRAME_DELAY = 60,//60이 게임에 쓰이는 값입니다.
	};
public:
	CFrameMgr();
	virtual ~CFrameMgr();
	
public:
	
	__int32* MakeINT32Memory();
	void FreeMemories();
	bool CanNextFrame();//다음 프레임으로 갈 수 있는지 체크 합니다. true이면 다음 프레임으로 갈 수 있습니다.
	void DoFrameDelay(char** pTemp);//한 프레임에서 할 수 있는 모든 일이 종료 되었을때 다음 프레임을 위해서 블록을 건다.
	

	__int32 GetFrameCount();
	void	IncFrameCount();//한프레임
	void	InitTime();

private://프레임 딜레이용 변수
	static DWORD* m_pdwSkipDelay;
	static int* m_pnIsSkiped;
	static int* m_piSkipCount;
	static __int32* m_pnFrameDelay;
	static DWORD* m_pdwFrameCheckTime;//프레임 체킹 시간
	static DWORD* m_pdwPrevFrameCheckTime;//이전 프레임 체킹 시간
	static DWORD* m_pdwTargetTime;
private://프레임 카운트용 변수
	DWORD* MakeDwordMemory();
	int* GetFrameDelayBuffer(); // CSD-030804
	int m_iNowFrame; 
	int m_iFrameCounter; 
	
};

extern CFrameMgr g_FrameMgr;
#define g_pFrameMgr		CFrameMgr::Pointer()	//frame manager
#endif // !defined(AFX_FRAMEMGR_H__5521928D_1621_4CA5_B2CE_501D72B875F5__INCLUDED_)
