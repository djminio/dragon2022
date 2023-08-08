// CurrentMsgMgr.cpp: implementation of the CCurrentMsgMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CurrentMsgMgr.h"
#include "gameproc.h"
#include "map.h"
#include "char.h"
#include "path.h"
#include "network.h"
#include "object.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"
#include "menu.h"
#include "Hangul.h"
#include "Dragon.h"
#include "Effect.h"
#include "CharDataTable.h"
#include "Tool.h"
#include "MouseCursor.h"
#include "Skill.h"
#include "Item.h"
#include "SkillTool.h"
#include "SmallMenu.h"
#include "LineCommand.h"
#include "Counseler.h"
#include "NPC_Pattern.h"
#include "Resource.h"

CCurrentMsgMgr  CurrentMsgMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurrentMsgMgr::CCurrentMsgMgr()
{

}

CCurrentMsgMgr::~CCurrentMsgMgr()
{

}

void AddCurrentStatusMessage( const WORD wColor, char *msg, ... )
{	
	char temp[MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf( temp, msg, arg );
	}
    va_end( arg );
	CurrentMsgMgr.AddString(wColor,"%s",temp);
}	

void AddCurrentStatusMessage( const int R, const int G, const int B, char *msg, ... )
{	//< CSD-030723
	char temp[MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf( temp, msg, arg );
	}
    va_end( arg );
	CurrentMsgMgr.AddString(R,G,B,"%s",temp);
}	//> CSD-030723

int CurStsMsgCount;
	
static int s_nCount = 0; // CSD-030520
	
void CCurrentMsgMgr::CheckCurrentStatusMessage()
{
	static int tt = 0;
	tt --;
	if( tt > 0 )  return;
	tt = 3;
		
	for( int i = 0; i < NUM_STATUSMESSAGE ; i ++ )
	{	
		if( CurStsMsg[ i].msg[0] != 0 )
		{
			CURRENTSTATUSMESSAGE *lpCSM = &CurStsMsg[ i];
			if( lpCSM->y < 0 )
			{
				lpCSM->msg[0] = 0; 
				CurStsMsgCount --; 
			}
			lpCSM->y --;
		}
	}	
}

void CCurrentMsgMgr::ViewCurrentStatusMessage( void )
{	//< CSD-030723
#ifdef _DEBUG
	if( tool_ViewMessage ) 	{return;}
#endif
	CURRENTSTATUSMESSAGE *lpCSM;
	int     gapy = (ViewTipsFlag)?130:0;
	
	if(!system_info.action)	
	{		
		int iViewPos = -1;
		for( int i = 0; i < NUM_STATUSMESSAGE ; i ++ )
		{
			if( CurStsMsg[i].msg[0] != 0 )
			{
				iViewPos = i;
			}
		}
		if(-1 != iViewPos)
		{	
			lpCSM = &CurStsMsg[iViewPos];
			HprintBold( 4, 340, lpCSM->dwColor,  FONT_COLOR_BLACK, "%s",lpCSM->msg );
			lpCSM->delay--;
			if( lpCSM->delay <= 0 ) 
			{
				lpCSM->msg[0] = 0;
			}
		}	
		return;
	}		
			
	if( CurStsMsgCount <= 0 )	
	{
		return;
	}

	for( int i = 0; i < NUM_STATUSMESSAGE ; i ++)
	{
		if(CurStsMsg[i].msg[0]!=0)
		{
			lpCSM = &CurStsMsg[i];
			const int len = strlen( lpCSM->msg );
			HprintBold( lpCSM->x, lpCSM->y+gapy, lpCSM->dwColor,  FONT_COLOR_BLACK, "%s",lpCSM->msg );
		}
	}
			
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;
	HangulOutputArea( 0, SCREEN_WIDTH-1, 0, SCREEN_HEIGHT-1  );
}	//> CSD-030723

void CCurrentMsgMgr::AddString( const int R, const int G, const int B, char *msg, ... )
{	
	char temp[ MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf( temp, msg, arg );
	}
    va_end( arg );
	

	int gap, maxy = -100;
	for( int i = 0; i < NUM_STATUSMESSAGE ; i ++ )
	{
		if( CurStsMsg[ i].msg[0] != 0 )
		if( maxy < CurStsMsg[ i].y ) 
		{
			maxy = CurStsMsg[ i].y;
		}
	} 

	if( maxy > STATUSMESSAGE_Y - 14 )
	{
		gap = maxy - STATUSMESSAGE_Y + 14;
		for( int i = 0; i < NUM_STATUSMESSAGE ; i ++ )
		{
			if( CurStsMsg[ i].msg[0] != 0 ) CurStsMsg[ i].y -= gap;
		}
	}
	
	CURRENTSTATUSMESSAGE *lpCSM;
	lpCSM = &CurStsMsg[s_nCount]; // CSD-030520
	strncpy( lpCSM->msg, temp, MAX_PATH );
	lpCSM->msg[MAX_PATH] = 0;
	lpCSM->x = STATUSMESSAGE_X;
	lpCSM->y = STATUSMESSAGE_Y;
	lpCSM->delay = 30;
	lpCSM->dwColor = RGB(R,G,B);
	
	CurStsMsgCount++;
	
	s_nCount += 4; // CSD-030520
	s_nCount %= NUM_STATUSMESSAGE; // CSD-030520
	return;
}
void CCurrentMsgMgr::AddString( const WORD wColor, char *msg, ... )
{
	char temp[ MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf( temp, msg, arg );
	}
    va_end( arg );
	const int	R = (wColor>>11 & 0x1f) << 3;
	const int	G = (wColor>>6  & 0x1f) << 3;
	const int	B = (wColor     & 0x1f) << 3;
	AddString(R,G,B,"%s",temp);
}
void CCurrentMsgMgr::Clear()
{
	for(int i= 0; i<NUM_STATUSMESSAGE;i++)
	{
		GetCurStsMsg(i)->msg[0] =0;
	}
}
