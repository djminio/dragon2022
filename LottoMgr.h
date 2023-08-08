// LottoMgr.h: interface for the CLottoMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOTTOMGR_H__451262F1_F06A_469A_987E_4D3B1DA114E1__INCLUDED_)
#define AFX_LOTTOMGR_H__451262F1_F06A_469A_987E_4D3B1DA114E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Network.h"

#define MAXIMUM_LOTTO_NUM	24

class CLottoMgr  
{
	int		m_anNumbers[10];
	int		m_anWinNumbers[10];
	int		m_nViewLottoCount;//���� ȭ�鿡 ������ �ζ��� ���.0���� �ɼ�����. 0���̸� ������ ���ƾ� �ϴϱ�.
	int		m_anBuyLotto[5][10];
	int		m_nInsertCount;
	static	CLottoMgr*	m_pClass;
public:
	void RecvLottoSeek(t_LOTTOPAPER_SEEK* pSeek);
	int CalPageCount(int nTotalLottoCount);
	int	m_nTotalPage;//����µ� ������.
	int GetCurrentPage();
	int GetTotalLottoCount();
	void SendLottoViewSeek(bool bNext = true);
	void RecvOpenLottoMenu(t_OPEN_LOTTO_MENU* pOpenLotto);
	void SendOpenLottoMenu();
	int		m_nMaxLottoCount;
	void ClearCurLottoNumbers();
	int* GetCurLottoNumbers();
	void RecvCheckWinner(t_CHECK_WINNER* pCheckWinner);
	void SendCheckWinner();
	void RecvBuyLotto(t_BUY_LOTTO* pBuyLotto);
	bool SendBuyLotto();
	void RecvOpenWinnerMenu(t_WINNER_MENU* pOpenMsg);
	void SendOpenWinnerMenu();
	bool WinnerMenuTextDisplay(int nMenuNum);
	void SetWinNumbers(int* anNumbers,int NumCount);
	void LottoMenuTextDisplay(int nMenuNum);
	static	CLottoMgr*	GetClass(){return m_pClass;}
	static	bool	Create(){ if(!m_pClass){new CLottoMgr; return true;} return false;}
	static	bool	Destroy(){ if(m_pClass){delete m_pClass;m_pClass = NULL; return true;}return false;}
	int GetCurCount();
	void Clear();
	bool IsFullNumber();
	int InsertNumber(int nLottoNumber);
	
	void Sort(int aNumbers[], int size);
	bool IsMember(int nNumber);
	int RandomNumber();
	int CreateOneNumber();
	
	int GetMinNumber(int aNumbers[], int size);
	void Generate6Number();
	CLottoMgr();
	virtual ~CLottoMgr();

private:
	int m_nTotalLottoCount;
	int m_nCrruntPage;
};

extern	inline CLottoMgr*	LottoMgr(){return CLottoMgr::GetClass();}

#endif // !defined(AFX_LOTTOMGR_H__451262F1_F06A_469A_987E_4D3B1DA114E1__INCLUDED_)
