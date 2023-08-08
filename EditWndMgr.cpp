// EditWndMgr.cpp: implementation of the CEditWndMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chat.h"
#include "Hangul.h"
#include "menu.h"
#include "dragon.h"
#include "Hong_Sub.h"
#include "Map.h"
#include "CharDataTable.h"
#include "TeamBattle.h"
#include "GameProc.h"
#include "LineCommand.h"
#include "Tool.h"
#include "Hong_Sprite.h"
#include "directsound.h"
#include "SmallMenu.h"
#include "resource.h"
#include "MenuSet.h"
#include "SmallMenuSet.h"
#include "smallmenu.h"
#include "EditWndMgr.h"

bool g_ControlKeyOn;

extern void DoButtonCheckOfMenu( int i, int j );
extern void DoQuickmemoryByKeyInput(const int iInputKey,bool bIsDikCheck);

#ifdef STRICT
	WNDPROC	g_ParentProcAddress = NULL;
#else
	FARPROC	g_ParentProcAddress = NULL;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEditWndMgr EWndMgr;
CEditWndMgr::CEditWndMgr()
{
	Clear();
}

CEditWndMgr::~CEditWndMgr()
{
	Clear();
}

HWND SetFocus2( const int iIndex)//021001 lsw
{
	return EWndMgr.SetWndFocus(iIndex);
}

HWND CEditWndMgr::SetWndFocus(int iIndex)
{
	HWND hWnd = 0;
#ifdef STRICT
	WNDPROC	ProcAddress;
#else
	FARPROC	ProcAddress;
#endif
	wndItor itor = vtWnd.begin();
	for(itor = vtWnd.begin() ;itor != vtWnd.end();itor++)//인덱스와 일치하는 윈도우를 받는다
	{
		if(iIndex == (*itor).iIndex)
		{
			hWnd		= (*itor).hWnd;
			ProcAddress = (*itor).ProcAddress; 
			break;
		}
	}
	
	for(wndItor itor2 = vtWnd.begin() ;itor2 != vtWnd.end();itor2++)//NowFocus에 물려 준다
	{
		if(HWND_NOW_FOCUS == (*itor2).iIndex)
		{
			(*itor2).hWnd = hWnd;
			(*itor).ProcAddress = ProcAddress;
		}
	}
	return SetFocus( hWnd );
}

int CEditWndMgr::CheckSysKey( UINT uiMessage, WPARAM wParam )
{
	if(WM_SYSKEYDOWN == uiMessage)
	{
		switch( wParam )
		{
		case VK_F4 :
			{
				CallMenu( MN_SYSTEM_LOGOUT, 193, 75, true ); 
				return 0;
			}break;
		}
	}
	return 1;
}

int CEditWndMgr::InitEditWnd()
{
	t_HWndInfo HwndInfo;

	HwndInfo.iIndex = HWND_NOW_FOCUS;
	HwndInfo.hWnd = NULL;
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_CHAT_INPUT;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD , 1,1, 500, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )ChatWndProc );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )ChatWndProc );
#endif
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_CHAT_TARGET_INPUT;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD, 1,1, 150, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )ChatTargetWndProc );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )ChatTargetWndProc );
#endif
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_1;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD| ES_AUTOHSCROLL , 1,1, 270, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )CommonEditWndProc );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )CommonEditWndProc );
#endif
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_2;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD| ES_AUTOHSCROLL , 1,1, 270, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )CommonEditWndProc );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )CommonEditWndProc );
#endif
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_3;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD| ES_AUTOHSCROLL |ES_WANTRETURN|ES_AUTOVSCROLL|ES_MULTILINE, 1,1, 265, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )CommonEditWndProc );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )CommonEditWndProc );
#endif
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_BBS;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD| ES_AUTOHSCROLL , 1,1, 265, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )CommonEditWndProc );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )CommonEditWndProc );
#endif
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_MAIL1;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD| ES_AUTOHSCROLL , 1,1, 265, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )MailWndProc1 );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )MailWndProc1 );
#endif
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_MAIL2;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD| ES_AUTOHSCROLL , 1,1, 265, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )MailWndProc2 );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )MailWndProc2 );
#endif
	vtWnd.push_back(HwndInfo);

	HwndInfo.iIndex = HWND_MAIL3;
	HwndInfo.hWnd = ::CreateWindow( "edit", NULL, WS_CHILD| ES_AUTOHSCROLL |ES_AUTOVSCROLL| ES_WANTRETURN | ES_MULTILINE, 1,1, 265, 20, g_hwndMain, NULL, g_hInstance, NULL );
	if(!HwndInfo.hWnd){JustMsg("Edit Window Create Failed(1)"); return 0;}
#ifdef STRICT
	HwndInfo.ProcAddress = ( WNDPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )MailWndProc3 );
#else
	HwndInfo.ProcAddress = ( FARPROC )::SetWindowLong( HwndInfo.hWnd, GWL_WNDPROC, ( LONG )MailWndProc3 );
#endif
	vtWnd.push_back(HwndInfo);

	g_ParentProcAddress = HwndInfo.ProcAddress;
	
	HwndInfo.iIndex = HWND_GAME_MAIN;//게임윈도우 등록
	HwndInfo.hWnd  = g_hwndMain;
	vtWnd.push_back(HwndInfo);
	return 1;
}

void CEditWndMgr::DestroyEditWnd()
{
	for(wndItor itor = vtWnd.begin() ;itor != vtWnd.end();itor++)
	{
		if(HWND_NOW_FOCUS != (*itor).iIndex)
		{
			if((*itor).hWnd)
			{
				DestroyWindow((*itor).hWnd);
				(*itor).hWnd = NULL;
			}
		}
	}
}

void CEditWndMgr::ClearAllTxt()//입력된 내용을 모두 지웁니다.//""를 넣는겁니다.
{
	for(int i = HWND_NOW_FOCUS;  HWND_GAME_MAIN >i;i++)
	{
		ClearTxt(i);
	}
}

void CEditWndMgr::ClearTxt(const int iIndex)//선택한 에디트 박스의 내용을 지웁니다.
{	
	for(wndItor itor = vtWnd.begin() ;itor != vtWnd.end();itor++)
	{
		if(iIndex == (*itor).iIndex )
		{
			SetTxt(iIndex,"");
		}
	}
}

void CEditWndMgr::Clear()
{
	DestroyEditWnd();
	vtWnd.clear();
}

//에디트 박스에 연결될 프로시져
LRESULT CALLBACK CommonEditWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam )
{
	if( EWndMgr.CheckSysKey( uiMessage, wParam ) == 0 ) return 0;

	switch ( uiMessage )
	{
	case WM_INPUTLANGCHANGE :	
		{
			::CheckChineseInput( hWnd, lParam );
		}break;

	case	WM_KEYDOWN:
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data
			switch ( nVirtKey )
			{
				case	VK_LEFT:
				case	VK_RIGHT:
				case	VK_UP:
				case	VK_DOWN:
				case	VK_HOME:
				case	VK_INSERT:
				case	VK_DELETE:
				{
					return	0;
				}break;
			}
		}break;
	case WM_KEYUP:
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data
			switch ( nVirtKey )
			{
			case	VK_RETURN:
			case	VK_TAB:		
				{
					if(SMenu[MN_GUILD_REGIST_DEGREE].bActive) 
					{
						SMenu[MN_GUILD_REGIST_DEGREE].work++;
						SMenu[MN_GUILD_REGIST_DEGREE].work %= 5;
						SetWindowText( hWnd, "" );
					}
					if(SMenu[MN_LOGIN].bActive)//메인 인터페이스 입력 전이라면
					{
						switch(EWndMgr.GetFocusWndIndex())
						{
						case HWND_1:
							{
								id_password = false;
								EWndMgr.SetWndFocus(HWND_2);
							}break;
						case HWND_2:
							{
								switch (nVirtKey)
								{
								case	VK_RETURN:
									{
										SMenu[MN_LOGIN].nField[0].fCheakFlag=TRUE;
										EWndMgr.SetWndFocus(HWND_GAME_MAIN);
									}break;
								case	VK_TAB:	
									{
										id_password = true;
										EWndMgr.SetWndFocus(HWND_1);
									}break;
								}
							}break;
						default:
							{
							}break;
						}
					}
					g_aCurrentKeys[DIK_RETURN] = 0;
				}break;	
			}
		}break;
	case WM_CHAR: 
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )//엔터나 탭 치면 삑삑 소리나는거 방지
			{
				case	VK_RETURN: return 0;
				case	VK_TAB: return 0;
			}
		}break;
	}
	return	CallWindowProc( g_ParentProcAddress, hWnd, uiMessage, wParam, lParam );
}

LRESULT CALLBACK ChatWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam )
{
	if( EWndMgr.CheckSysKey( uiMessage, wParam ) == 0 ) return 0;

	switch ( uiMessage )
	{
	case WM_INPUTLANGCHANGE :	
		{
			::CheckChineseInput( hWnd, lParam );
		}break;
	case WM_KEYDOWN:
		{
			const int	nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data
			switch ( nVirtKey )
			{
			case	VK_NEXT:
				{
					DWORD selStart, selEnd;
					//if(GetAsyncKeyState(VK_SHIFT) >= 0)
					//		return	0;
					SendMessage(hWnd,EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);
					if(selStart > 0)
						selStart--;
					SendMessage(hWnd,EM_SETSEL,selStart,selStart);
				}
				return 0;
			case	VK_PRIOR:
				{
					DWORD selStart, selEnd;
					//if(GetAsyncKeyState(VK_SHIFT) >= 0)
					//		return	0;
					SendMessage(hWnd,EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);
					//if(selStart > 0)
						selStart++;
					SendMessage(hWnd,EM_SETSEL,selStart,selStart);
				}
				return 0;
			case	VK_UP:
			case	VK_DOWN:
			case	VK_INSERT:
			/*case	VK_HOME:
			case	VK_END:
			//case	VK_DELETE:*/
				{
					return 0;
				}break;
			case	VK_TAB:	
				{
					const int iTarget = GetChatTarget();
					switch(iTarget)
					{
						case CHAT_TARGET_WISPER1	:
						case CHAT_TARGET_WISPER2	:
						case CHAT_TARGET_WISPER3	:
						case CHAT_TARGET_WISPER4	:
						case CHAT_TARGET_WISPER5	:
							{
								char temp[MAX_CHAT_STRING_]={""};
								EWndMgr.ClearTxt(HWND_CHAT_INPUT);
								SetChatTarget(iTarget+1);
							}break;
						case CHAT_TARGET_PARTY:
							{
								char temp[MAX_CHAT_STRING_]={""};
								EWndMgr.ClearTxt(HWND_CHAT_INPUT);
								SetChatTarget(SELECT_WISPER_MODE);
							}break;
						case CHAT_TARGET_GUILD:
							{
								char temp[MAX_CHAT_STRING_]={""};
								EWndMgr.ClearTxt(HWND_CHAT_INPUT);
								SetChatTarget(CHAT_TARGET_PARTY);
							}break;
						case CHAT_TARGET_NORMAL:
							{
								char temp[MAX_CHAT_STRING_]={""};
								EWndMgr.ClearTxt(HWND_CHAT_INPUT);
								SetChatTarget(CHAT_TARGET_GUILD);
							}break;
					}
					return 0;
				}break;
			case	VK_RETURN: 
				{
					bool IsChatModeChange = true;
					switch(GetChatTarget())
					{
					case SELECT_WISPER_MODE	:
					case CHAT_TARGET_WISPER1	:
					case CHAT_TARGET_WISPER2	:
					case CHAT_TARGET_WISPER3	:
					case CHAT_TARGET_WISPER4	:
					case CHAT_TARGET_WISPER5	:
						{
							char temp[MAX_CHAT_STRING_]={0,};
							char name[NM_LENGTH]={0,};
							EWndMgr.GetTxt( HWND_CHAT_INPUT, temp, MAX_CHAT_STRING_-1 );
							EWndMgr.GetTxt( HWND_CHAT_TARGET_INPUT, name, NM_LENGTH-1 );
							if( !name[0] )//이름이 없다면
							{
								SetFocus2(HWND_CHAT_TARGET_INPUT);//021001 lsw
							}
							else
							{
								IsChatModeChange = SendChatWisper(name,temp);
								const int iNextFocus = InsertWisperList(name);
								if(!IsChatBoxLock())
								{
									SetChatTarget( CHAT_TARGET_NORMAL );//020530 lsw
								}
								else
								{
									SetChatTarget(iNextFocus);
								}
								EWndMgr.ClearTxt(HWND_CHAT_INPUT);
							}
						}break;
					case CHAT_TARGET_PARTY:
						{
							char temp[MAX_CHAT_STRING_]={""};
							EWndMgr.GetTxt( HWND_CHAT_INPUT, temp, MAX_CHAT_STRING_-1 );
							IsChatModeChange = SendPartyChatByLineCommand( temp );
							EWndMgr.ClearTxt(HWND_CHAT_INPUT);
						}break;
					case CHAT_TARGET_GUILD:
						{
							char temp[MAX_CHAT_STRING_]={""};
							EWndMgr.GetTxt( HWND_CHAT_INPUT, temp, MAX_CHAT_STRING_-1 );
							IsChatModeChange = SendChatGuild( temp );
							EWndMgr.ClearTxt(HWND_CHAT_INPUT);
						}break;
					case CHAT_TARGET_NORMAL:
					default:
						{
							char temp[MAX_CHAT_STRING_]={""};
							EWndMgr.GetTxt( HWND_CHAT_INPUT, temp, MAX_CHAT_STRING_-1 );
							IsChatModeChange = SendChatNormal(temp);
							EWndMgr.ClearTxt(HWND_CHAT_INPUT);
						}break;
					}
					if(IsChatModeChange&&(!IsChatBoxLock()))//채팅모드 닫기 인가?
					{
						SetChatMode(CM_COMMAND,true);
					}
					else
					{
						SetChatMode(CM_MESSAGE);
					}
					return 0;
				}break;//case	VK_RETURN: 
			default:{}break;
			}
		}break;
	case WM_KEYUP:
		{
			const int	nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data
			switch ( nVirtKey )
			{
			case	VK_LEFT:
			case	VK_RIGHT:
			case	VK_UP:
			case	VK_DOWN:
			case	VK_INSERT:
			case	VK_HOME:
			/*case	VK_END:
			case	VK_DELETE:*/
				{
					return 0;
				}
				break;
			case	VK_ESCAPE:  
				{
					SetChatTarget(CHATTYPE_NORMAL);//020705 lsw
					SetChatMode(CM_COMMAND,true);
					return 0;
				}break;
			case VK_F1:
			case VK_F2:
			case VK_F3:
			case VK_F4:
			case VK_F5:
			case VK_F6:
			case VK_F7:
			case VK_F8:
				{
					DoQuickmemoryByKeyInput(nVirtKey,false);
					return 0;
				}break;
			}//switch ( nVirtKey )
		}break;
	case WM_CHAR: 
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
				case	VK_RETURN: return 0;
				case	VK_TAB: return 0;
			}
		}break;
	}
	return	CallWindowProc( g_ParentProcAddress, hWnd, uiMessage, wParam, lParam );
}

LRESULT CALLBACK ChatTargetWndProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam )
{
	if( EWndMgr.CheckSysKey( uiMessage, wParam ) == 0 ) return 0;

	switch ( uiMessage )
	{
	case WM_INPUTLANGCHANGE :
		{
			{::CheckChineseInput( hWnd, lParam );}break;
		}
	case	WM_KEYDOWN:
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
			case VK_NEXT:
				{
					DWORD selStart, selEnd;
					//if(GetAsyncKeyState(VK_SHIFT) >= 0)
					//		return	0;
					SendMessage(hWnd,EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);
					if(selStart > 0)
						selStart--;
					SendMessage(hWnd,EM_SETSEL,selStart,selStart);
				}
				return 0;
			case	VK_PRIOR:
				{
					DWORD selStart, selEnd;
					//if(GetAsyncKeyState(VK_SHIFT) >= 0)
					//		return	0;
					SendMessage(hWnd,EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);
					//if(selStart > 0)
						selStart++;
					SendMessage(hWnd,EM_SETSEL,selStart,selStart);
				}
				return 0;
			case	VK_UP:
			case	VK_DOWN:
/*			case	VK_HOME:
			case	VK_END:
			case	VK_INSERT:
			case	VK_DELETE:*/
				{
					return	0;
				}break;
			case	VK_RETURN:
			case	VK_TAB:			
				{
					int iTarget = GetChatTarget();
					switch(iTarget)
					{
					case SELECT_WISPER_MODE	://귓말 대상 입력 일때 
						{
							char name[MAX_CHAT_STRING_]={""};
							EWndMgr.ClearTxt(HWND_CHAT_INPUT);
							EWndMgr.GetTxt( HWND_CHAT_TARGET_INPUT, name, MAX_CHAT_STRING_-1 );
							if(!name[0])//if No Name
							{
								SetChatTarget(CHAT_TARGET_WISPER1);//goto Next 
							}
							else
							{
								EWndMgr.SetWndFocus(HWND_CHAT_INPUT);
							}
						}break;
					default:
						{
							SetChatMode(CM_MESSAGE);
						}break;
					}
					return 0;
				}break;
			}
		}break;
	case WM_KEYUP: 
		{	
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data
			switch ( nVirtKey )
			{
			case	VK_UP:
			case	VK_DOWN:
			/*case	VK_LEFT:
			case	VK_RIGHT:
			case	VK_HOME:
			case	VK_END:
			case	VK_INSERT:
			case	VK_DELETE:*/
				{
					return	0;
				}
			case VK_F1:
			case VK_F2:
			case VK_F3:
			case VK_F4:
			case VK_F5:
			case VK_F6:
			case VK_F7:
			case VK_F8:
				{
					DoQuickmemoryByKeyInput(nVirtKey,false);
				}break;
			case	VK_ESCAPE: 
				{
					SetChatTarget(CHATTYPE_NORMAL);//020705 lsw
					SetChatMode(CM_COMMAND,true);
					return 0;
				}break;
			}
		}break;
	case WM_CHAR: 
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
				case	VK_RETURN: return 0;
				case	VK_TAB: return 0;
			}
		}break;
	}
	return	CallWindowProc( g_ParentProcAddress, hWnd, uiMessage, wParam, lParam );
}

LRESULT CALLBACK MailWndProc1( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam )
{
	if( EWndMgr.CheckSysKey( uiMessage, wParam ) == 0 ) return 0;

	switch ( uiMessage )
	{
	case WM_INPUTLANGCHANGE :	
		{
			::CheckChineseInput( hWnd, lParam );
		}break;
	case	WM_KEYDOWN:
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
		/*	case	VK_LEFT:
			case	VK_RIGHT:
			case	VK_UP:
			case	VK_DOWN:
			case	VK_HOME:
			case	VK_INSERT:
			case	VK_DELETE:
				{
					return	0;
				}break;*/
			case	VK_TAB: 
			case	VK_RETURN: 	
				{
					SMenu[MN_MAIL_WRITE].work=2; 
					SMenu[MN_HUNTER_REGIST].work=2;
					return 0;
				}break;
			}//switch ( nVirtKey )
		}break;
	case WM_CHAR: 
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
			case	VK_RETURN: {return 0;}break;
			case	VK_TAB: {return 0;}break;
			}//switch ( nVirtKey )
		}break;
	}
	return	CallWindowProc( g_ParentProcAddress, hWnd, uiMessage, wParam, lParam );
}

LRESULT CALLBACK MailWndProc2( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam )
{
	if( EWndMgr.CheckSysKey( uiMessage, wParam ) == 0 ) return 0;

	switch ( uiMessage )
	{
	case WM_INPUTLANGCHANGE :	
		{
			::CheckChineseInput( hWnd, lParam );
		}break;
	case	WM_KEYDOWN:
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
		//	case	VK_SHIFT : 
			/*case	VK_LEFT:
			case	VK_RIGHT:
			case	VK_UP:
			case	VK_DOWN:
			case	VK_HOME:
			case	VK_INSERT:
			case	VK_DELETE:
			{
				return	0;
			}*/
			case	VK_TAB: 
				{
					if( LeftShiftOn ) 
					{ 
						SMenu[MN_MAIL_WRITE].work=1; 
						SMenu[MN_HUNTER_REGIST].work=1;
						break;
					}
				}//break; 없는겁니다..
			case	VK_RETURN: 	
				{
					SMenu[MN_MAIL_WRITE].work=3; 
					SMenu[MN_HUNTER_REGIST].work=3;
					return 0;
				}break;		
			}//switch ( nVirtKey )
		}break;
	case WM_CHAR: 
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
				case	VK_RETURN: return 0;
				case	VK_TAB: return 0;
			}
		}break;
	}
	return	CallWindowProc( g_ParentProcAddress, hWnd, uiMessage, wParam, lParam );
}

LRESULT CALLBACK MailWndProc3( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam )
{
	if( EWndMgr.CheckSysKey( uiMessage, wParam ) == 0 ) return 0;

	switch ( uiMessage )
	{
	case WM_INPUTLANGCHANGE :	
		{
			::CheckChineseInput( hWnd, lParam );
		}break;
	case	WM_KEYDOWN:
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
			case	VK_UP:
				{
					DWORD selStart, selEnd;
					int selection;
					char text[2048] = {0,};

					SendMessage(hWnd,EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);
					/* 1000 is a hard coded value for max number of characters in the mail window (see kh_menuset aroundline 6000)*/
					GetWindowText(hWnd, text, 1000 );
					selection = selStart;
					MakeCursorUpDown(text, selection, true);

					SendMessage(hWnd,EM_SETSEL,(WPARAM)selection,(LPARAM)selection);
				}
				return 0;
			case	VK_DOWN:
				{
					DWORD selStart, selEnd;
					int selection;
					char text[2048] = {0,};

					SendMessage(hWnd,EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);
					/* 1000 is a hard coded value for max number of characters in the mail window (see kh_menuset aroundline 6000)*/
					GetWindowText(hWnd, text, 1000 );
					selection = selStart;
					MakeCursorUpDown(text, selection,false);

					SendMessage(hWnd,EM_SETSEL,(WPARAM)selection,(LPARAM)selection);
				}
				return 0;
		//	case	VK_SHIFT :
			/*case	VK_LEFT:
			case	VK_DOWN:
			case	VK_HOME:
			case	VK_INSERT:
			case	VK_DELETE:
			{
				return	0;
			}*/
			case	VK_TAB: 
				{
					if( LeftShiftOn ) 
					{
						SMenu[MN_MAIL_WRITE].work=2;
						SMenu[MN_HUNTER_REGIST].work=2;
					}
				}//break; 없는것
			case	VK_RETURN: 
				{
					return 0;
				}break;
			}
		}break;
	case WM_CHAR: 
		{
			int		nVirtKey = ( int )wParam;	// virtual-key code 
			LPARAM	lKeyData = lParam;			// key data

			switch ( nVirtKey )
			{
				case	VK_TAB: return 0;
			}
		}break;
	}
	return	CallWindowProc( g_ParentProcAddress, hWnd, uiMessage, wParam, lParam );
}