// EditWndMgr.h: interface for the CEditWndMgr class.
// Make By Hwoarang Sangwoo 2002 10 01
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
#define AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum//�Ʒ��� enum �� ��ȭ ���� �ʵ��� �ؾ� �մϴ�. ���� �ϼ���. 0���� �ݵ�� HWND_NOW_FOCUS ���� �մϴ�.
{
	HWND_NOW_FOCUS = 0,
	HWND_CHAT_INPUT = 1,
	HWND_CHAT_TARGET_INPUT = 2,
	HWND_1,//hwnd1
	HWND_2,//hwnd2
	HWND_3,//hwndEditBox ;  // �̸��� ����
	HWND_BBS, //hwnd_bbs
	HWND_MAIL1,
	HWND_MAIL2,
	HWND_MAIL3,
	HWND_GAME_MAIN,//���� ������//�׻� �������� ��ġ
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
	CEditWndMgr(const CEditWndMgr &old);//�⺻���� �����Ǵ� ���� �����ڸ� ��� �� �� �����ϴ�.
	CEditWndMgr operator= (const CEditWndMgr &old);//���� �����ڸ� ��� �� �� �����ϴ�.
private:	
	vector<HWNDINFO> vtWnd;//������ ���� ����

public:
	int InitEditWnd();
	void ClearAllTxt();//�Էµ� ������ ��� ����ϴ�.//""�� �ִ°̴ϴ�.
	void ClearTxt(const int iIndex);//������ ����Ʈ �ڽ��� ������ ����ϴ�.
	
	int	SetTxt(const int iIndex, const char* txt)//�����쿡 �ؽ�Ʈ�� ���� �մϴ�.
	{
		HWND tempHWnd = GetHWnd(iIndex);
		SetWindowText(tempHWnd,(txt)?txt:NULL);
		SendMessage( tempHWnd, WM_KEYDOWN, VK_END, 0 );
		return 1;
	}
	int	GetTxt(const int iIndex, char* txt, const int iSize)//�������� �ؽ�Ʈ�� �����ɴϴ�.
	{
		return GetWindowText( GetHWnd(iIndex), txt, iSize );
	}

	HWND SetWndFocus(const int iIndex);//�������� ��Ŀ���� ���� �մϴ�.
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
	int CheckSysKey( UINT uiMessage, WPARAM wParam );//alt + F4 ���� �ý��� �޼��� üũ

	int GetFocusWndIndex()//HWND_NOW_FOCUS �� �ε����� ��ȯ ���� �ʴ´�
	{
		HWND HWndFocus = GetHWnd(HWND_NOW_FOCUS);
		for(wndItor itor = vtWnd.begin() ;itor != vtWnd.end();itor++)
		{
			if(HWndFocus == (*itor).hWnd &&  HWND_NOW_FOCUS !=(*itor).iIndex)//������� ��ġ�ϵ� �ε����� ��Ŀ���̸� �ȵȴ�
			{
				return (*itor).iIndex;
			}
		}
		return 0;
	}
private:
	void Clear();
	void DestroyEditWnd();
	friend LRESULT CALLBACK CommonEditWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//������ ������ ���ν���
	friend LRESULT CALLBACK ChatWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//ä�� ��ȭ ��� ���� ������ ���ν���
	friend LRESULT CALLBACK ChatTargetWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//ä�� ���� �Է�â�� ������ ���ν���
	friend LRESULT CALLBACK MailWndProc1( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//����1 ���� ���� ������ ���ν���
	friend LRESULT CALLBACK MailWndProc2( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//����2 ���� ���� ������ ���ν���
	friend LRESULT CALLBACK MailWndProc3( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );//����3 ���� ���� ������ ���ν���
};
extern CEditWndMgr EWndMgr;
#endif // !defined(AFX_EDITWNDMGR_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
