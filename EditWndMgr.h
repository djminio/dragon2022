// EditWndMgr.h: interface for the CEditWndMgr class.
// Make By Hwoarang Sangwoo 2002 10 01
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
#define AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum//아래의 enum 을 변화 하지 않도록 해야 합니다. 주의 하세요. 0번은 반드시 HWND_NOW_FOCUS 여야 합니다.
{
	HWND_NOW_FOCUS = 0,
	HWND_CHAT_INPUT = 1,
	HWND_CHAT_TARGET_INPUT = 2,
	HWND_1,//hwnd1
	HWND_2,//hwnd2
	HWND_3,//hwndEditBox ;  // 이름을 위한
	HWND_BBS, //hwnd_bbs
	HWND_MAIL1,
	HWND_MAIL2,
	HWND_MAIL3,
	HWND_GAME_MAIN,//게임 윈도우//항상 마지막에 위치
};

typedef struct HWNDINFO
{
	int		iIndex;
	HWND	hWnd;
#ifdef STRICT
	WNDPROC	ProcAddress;
#else
	FARPROC	ProcAddress;
#endif
}t_HWndInfo,*lpHWNDINFO;

typedef vector<HWNDINFO>::iterator wndItor;

class CEditWndMgr  
{
public:
	CEditWndMgr();
	virtual ~CEditWndMgr();
private:
	CEditWndMgr(const CEditWndMgr &old);//기본으로 생성되는 복사 생성자를 사용 할 수 없습니다.
	CEditWndMgr operator= (const CEditWndMgr &old);//대입 연산자를 사용 할 수 없습니다.
private:	
	vector<HWNDINFO> vtWnd;//윈도우 정보 벡터

public:
	int InitEditWnd();
	void ClearAllTxt();//입력된 내용을 모두 지웁니다.//""를 넣는겁니다.
	void ClearTxt(const int iIndex);//선택한 에디트 박스의 내용을 지웁니다.
	
	int	SetTxt(const int iIndex, const char* txt)//윈도우에 텍스트를 셋팅 합니다.
	{
		HWND tempHWnd = GetHWnd(iIndex);
		SetWindowText(tempHWnd,(txt)?txt:NULL);
		SendMessage( tempHWnd, WM_KEYDOWN, VK_END, 0 );
		return 1;
	}
	int	GetTxt(const int iIndex, char* txt, const int iSize)//윈도우의 텍스트를 가져옵니다.
	{
		return GetWindowText( GetHWnd(iIndex), txt, iSize );
	}

	HWND SetWndFocus(const int iIndex);//윈도우의 포커스를 셋팅 합니다.
	HWND GetHWnd(const int iHWndIndex)
	{
		for(wndItor itor = vtWnd.begin() ;itor != vtWnd.end();itor++)
		{
			if(  iHWndIndex == (*itor).iIndex)
			{
				return (*itor).hWnd;
			}
		}
		return 0;
	}

#ifdef STRICT
	WNDPROC	GetProcAddress(const int iHWndIndex)
#else
	FARPROC	GetProcAddress(const int iHWndIndex)
#endif
	{
		for(wndItor itor = vtWnd.begin() ;itor != vtWnd.end();itor++)
		{
			if(  iHWndIndex == (*itor).iIndex)
			{
				return (*itor).ProcAddress;
			}
		}
		return 0;
	}

	int IsFocus(const int iIndex){return (iIndex == GetFocusWndIndex())?1:0;}
	int CheckSysKey( UINT uiMessage, WPARAM wParam );//alt + F4 류의 시스템 메세지 체크

	int GetFocusWndIndex()//HWND_NOW_FOCUS 의 인덱스를 반환 하지 않는다
	{
		HWND HWndFocus = GetHWnd(HWND_NOW_FOCUS);
		for(wndItor itor = vtWnd.begin() ;itor != vtWnd.end();itor++)
		{
			if(HWndFocus == (*itor).hWnd &&  HWND_NOW_FOCUS !=(*itor).iIndex)//윈도우는 일치하되 인덱스가 포커스이면 안된다
			{
				return (*itor).iIndex;
			}
		}
		return 0;
	}
private:
	void Clear();
	void DestroyEditWnd();
	friend LRESULT CALLBACK CommonEditWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//보통의 윈도우 프로시저
	friend LRESULT CALLBACK ChatWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//채팅 대화 상대 지정 윈도우 프로시저
	friend LRESULT CALLBACK ChatTargetWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//채팅 내용 입력창의 윈도우 프로시저
	friend LRESULT CALLBACK MailWndProc1( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//메일1 에서 쓰는 윈도우 프로시저
	friend LRESULT CALLBACK MailWndProc2( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//메일2 에서 쓰는 윈도우 프로시저
	friend LRESULT CALLBACK MailWndProc3( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//메일3 에서 쓰는 윈도우 프로시저
};
extern CEditWndMgr EWndMgr;
#endif // !defined(AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
