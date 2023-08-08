#include "StdAfx.h"
#include <windowsx.H>
#include "DirectDraw.h"

#include "Dragon.h"
#include "Resource.h"
#include "Hong_Sprite.h"	
#include "Hong_Sub.h"
#include "GameProc.h"
#include "Char.h"
#include "Item.h"
#include "Map.h"
#include "Object.h"
#include "Hong_Light.h"
#include "SkillTool.h"
#include "MouseCursor.h"
#include "CharDataTable.h"
#include "Effect.h"
#include "Counseler.h"
#include "Skill.h"
#include "DirectSound.h"
#include "LineCommand.h"
#include "Menu.h"
#include "Music.h"
#include "SmallMenu.h"
#include "Menuset.h"
#include "Tool.h"
#include "MenuNetWork.h"// 030307 soto추가.
#include "ArenaManager.h"

//Define added by Eleval for Light GM Client:
//#define LIGHT_GM_CLIENT
//End of it
//< LTH-040810-KO 1.04p.
#include "Prison.h"
#include <atlstr.h>
extern HWND	g_hMultiDlg;//soto-030603
extern CPrison g_cPrison;
extern bool	bCharListUp;				// 능력치 변경을 보내닌다.
extern bool	bTackListUp ;				// 택틱스 변경
extern bool	ScreenCaptureStartFlag;
extern int g_StartMenuOn;				//  현재 스타트 메뉴인지, 아님 메인 메뉴인지 판단...
extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
extern void LoadHeroClothAccessoryDataOne_tool(int sprno, int no); // CSD-030306
extern void FreeHeroClothAccessoryDataOne_tool(int sprno, int no); // CSD-030306
extern void	ChangeItem(int start);	// 임시 아이템 바꾸기 		
extern DWORD GetExpByLv(int lv);		// 레벨을 경험치로 변환해 준다.
extern bool LoadItemJoinTable();
extern void WarLoopTime(char* Arg);		// 030308 soto
CDownMsgView g_MapMoveTU_Log;
CDownMsgView::CDownMsgView()
{
	// TODO: add construction code here
	m_vDebugViewMsg.reserve(20);	//vector갯수를 20개로 처음에 잡음
	m_hLogHWND = NULL;
}

CDownMsgView::~CDownMsgView()
{
	ReleaseData();
	m_hLogHWND = NULL;
}

void CDownMsgView::Init_SetHwnd(HWND hWND)
{
	m_hLogHWND = hWND;


	//콤보박스의 dropdown list의 크기를 좀더 크게 설정
	short int nWidthPixel = SendMessage(m_hLogHWND, CB_GETDROPPEDWIDTH, 0, 0);
	SendMessage(m_hLogHWND, CB_SETDROPPEDWIDTH, (WPARAM)nWidthPixel + 200, 0);

}
void CDownMsgView::ReleaseData()
{
	m_vDebugViewMsg.clear();
}

/*
void CDownMsgView::GetDrawRect(CRect& _Rect)
{
	_Rect.left	= 10;
	_Rect.top	= 20;
	if (0 < m_vDebugViewMsg.size())	//메세지의 크기에 맞춰서 Clip하자.
	{
		_Rect.right		= m_vDebugViewMsg[m_vDebugViewMsg.size()-1].pos.x + 20;
		_Rect.bottom	= m_vDebugViewMsg[m_vDebugViewMsg.size()-1].pos.y;
	}
	else
	{
		_Rect.right		= 20;
		_Rect.bottom	= 20;
	}
}
*/

void CDownMsgView::PutDebugMsg(CString& Msg, CPoint& pos/*=CPoint(10,10)*/)
{
	int nCount = (int)m_vDebugViewMsg.size();

	/*
	if (0 < nCount)
	{
		if (10 == pos.x && 10 == pos.y)
		{
			pos.x = m_vDebugViewMsg[nCount-1].pos.x;
			pos.y = m_vDebugViewMsg[nCount-1].pos.y - 20;
		}

	}
	else if (0 == nCount)//처음세팅만 상대적인 Scroll View에서 시작하게 세팅
	{
		pos.y = 20;
	}
	*/

	CString strTmp;

	/*
	#ifdef _DEBUG
	strTmp.Format("%2d]  %s  [%s Ln:%d]",nCount,Msg,THIS_FILE,__LINE__);
	#else
	*/	strTmp.Format("%2d]  %s", nCount, Msg);
	/*
	#endif
	*/
	m_vDebugViewMsg.push_back(_DebugMsg(strTmp, pos));


	SendMessage(m_hLogHWND, CB_ADDSTRING, 0, (LPARAM)strTmp.GetBuffer(0));
	int nTUCount = SendMessage(m_hLogHWND, CB_GETCOUNT, 0, 0);
	if (0 < nTUCount)
	{
		SendMessage(m_hLogHWND, CB_SETCURSEL, 0, 0);
	}

	//	CRect rect;
	//	GetDrawRect(rect);
	//	InvalidateRect(NULL,&rect,true);
}
///////////////// SoundUp lkh 테스트를 위해 ////////////////////
//extern int	RainAmount;
extern BOOL	Statue_Weather;

extern HWND g_hSetScenarioDlgWnd = NULL; // 030307 soto

// << 031013 kyo
extern bool g_IsActiveProFiler;	
extern ProFileShowType g_ProFileType;
void ShowProFileDlg();
void ToggleSanta(); //Eleval 22/06/09 - To let GMs removing their Santa Power
void ToolPlaySound(); //Eleval
void ToolLMSMenu(); //Eleval 18/07/09 - To pop LMS Menu
void ToolExpEventMenu(); //Eleval 22/07/09 - To pop Exp Event Menu
void ToolPKMenu(); //Eleval 24/07/09 - To show PK Managment Menu
void ToolCheckQuestMenu(); //Eleval 13/08/09 - To test Quest NPC
void BanCharacter();
void ForceStartWar();
extern BOOL CALLBACK ForceNationWarProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK GenerateItemNewProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);
extern void generateitem();
BOOL CALLBACK ToolPlaySoundProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam); //Eleval 19/06/09 - Tool Play Sound
BOOL CALLBACK ToolLMSMenuProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam); //Eleval 18/07/09 - Tool LMS Menu
BOOL CALLBACK ToolExpEventMenuProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam); //Eleval 22/07/09 - Tool Exp Event
BOOL CALLBACK ToolPKMenuProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam); //Eleval 24/07/09 - Exp Event Menu
BOOL CALLBACK ToolCheckQuestMenuProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam); //Eleval 13/08/09 - To test Quest Npc
BOOL CALLBACK ProFilerProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BanAccountProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);

// >> 031013 kyo


///////////////////////`///////////////////////////////////////////////////////////////
/////   extern 

extern CHARACTERLIST g_CharacterList;

extern int		EndOfAnimationFlag;		

////////////////////////////////////////////////////////////////////////////////////////
/////	

int				SpriteOrderFrame, SpriteOrderDir;
int				AnimationEditWorkStartFlag;
int				CheckEquipmentFlag;
int				TempAnimationCount;
int				IDCheckedMapObject;

HWND			CheckAnimationHwnd;

DEVELOPE_ENVIR	SysInfo;



int				tool_ViewAttrEventFlag;
int				tool_ViewAttrDontFlag;
int				tool_ViewAttrRoofOnOff;


int tool_AnimationEditCurFrame;
int tool_AnimationEditTotalSpriteNo;
int tool_ViewAnimationFrame;
bool tool_DrawMap, tool_DrawObject, tool_DrawFrontMap, tool_ViewMessage, tool_ViewChat, tool_ViewNumber, tool_ViewMenu;					
int tool_DrawRoofMap;
int tool_MultiChar;

int tool_FrameCheck;

int tool_mapobject_modify;
int tool_ViewAttrFrontFlag;
int tool_ID_INPUT_MAP_ATTR;
int tool_ViewAttrRiver;
int tool_ViewAttrGEffect;
int tool_ViewAttrInside;
int tool_ViewAttrNoBattle;
int tool_ViewAttrCanMatch;
int tool_ViewAttrTop;				// 010904 LTS
int tool_ViewAttrOccupied;
int tool_Pathfind	= 1;
bool tool_ViewMyExp;
bool tool_ViewMonsterNo = false;	// YonMy;8;


int		tool_MouseCursorBlock;
char	tool_ID_EFFECT_TEST;
char	tool_IDD_INPUT_MAPOBJECT;
int		tool_CurMapObjectId=1;
char	tool_ViewAllCharacter;
bool	tool_SkillTool_FARMING;
bool	tool_SkillTool_MINING;	
bool	tool_SkillTool_FISHING;	
bool	tool_SkillTool_CHOPPING;
bool	tool_SkillTool_HUB;
bool	tool_SkillTool_NPC;
bool	tool_SkillTool_ITEM_SEAL;



int tool_ID_SKILL_INPUT;
HWND	InputSkillAttrHdlg;
HWND	tool_IDD_INPUT_MAPOBJECTHdlg;
HWND	tool_MouseCursorHdlg;

int tool_ID_INPUT_MYHOUSE;
int tool_ID_OUTPUT_MYHOUSE;
HWND	InputMyhouseHdlg;

//------------------------------------------------
HWND	InputMapAttrHdlg;
int tMapAttrBrush;
int tMapAttrX, tMapAttrY;
int tMapAttr = IDC_ATTR_DONT;
int tMapAttrAllAffect = IDC_SOURCE_AFFECT_NO;
int tMapAttr_Offset_Modify = IDC_ATTR_OFFSET_NO2;
int tMapAttr_Brush_Paint = IDC_PAINT_NO;
int tMapAttr_Brush_DePaint = IDC_DEPAINT_NO;			// 010904 LTS

int SoundRangeX=200;	// LTS SOUND
int SoundRangeY=170;

//------------------------------------------------


#ifdef _DEBUG
char			AnimationName[MAX_ANIMATION][ 110];					
#endif



////////////////////////////////////////////////////////////////////////////////////////////
////   user function



void XLine( int x, int y, DWORD xl, WORD color )
{
	LPVOID	Dtmp = (LPVOID)( (char*)g_DestBackBuf + y * dDxSize + (x * 2 ));
	
	_asm{
		push	es;
		push	ds;
		pop		es;
		
		mov		edi, Dtmp;
		
		xor		eax, eax;
		mov		ax, color;
		mov		ecx, 16;
		shl		eax, cl;
		
		mov		ax, color;
		
		mov		ecx, xl;
		shr		ecx, 1;
		
		
loop_1:
		mov		[edi], eax;
		add		edi, 4;
		
		loop	loop_1;
		
		pop		es;
	};
}	


void Line(int sx, int sy, int ex, int ey, int color)
{	
	int t,distance;
	int xerr = 0, yerr = 0, delta_x, delta_y;
	int incx, incy;
	int temp;
	
	sx -= Mapx;
	sy -= Mapy;
	ex -= Mapx;
	ey -= Mapy;
	
	delta_x = ex - sx;
	delta_y = ey - sy;
	
	incx = (delta_x > 0) ?  1 : ((delta_x == 0) ? 0 : -1) ;
	incy = (delta_y > 0) ?  1 : ((delta_y == 0) ? 0 : -1) ;
	
	delta_x = abs(delta_x);
	delta_y = abs(delta_y);
	
	distance = (delta_x > delta_y) ? delta_x : delta_y ;
	int maxoff = dDxSize * SCREEN_HEIGHT;
	for(t=0; t<=distance; t++)
	{
		if( sx >= 0 && sx < SCREEN_WIDTH && sy >= 0 && sy < SCREEN_HEIGHT ) 
		{
			temp = 2*sx + sy * dDxSize;
			*(WORD *)( g_DestBackBuf + temp ) = color;
		}
		
		xerr += delta_x;
		yerr += delta_y;
		if(xerr > distance)    {        xerr -= distance;       sx += incx;      }
		if(yerr > distance)    {        yerr -= distance;       sy += incy;      }
	}
}	






void PutDot( int x, int y, WORD color )
{
	if( x < 0 ) return;
	if( y < 0 ) return;
	if( x > SCREEN_WIDTH -1 ) return;
	if( y > SCREEN_HEIGHT -1 ) return;
	
	
	*(WORD *)(g_DestBackBuf + x*2 + y * dDxSize ) = color;
}

void Box_( int sx, int sy,  int xl, int yl, int color )
{	
	Line( sx, sy, sx, sy+yl-1, color );
	Line( sx, sy, sx+xl-1, sy, color );
	Line( sx+xl-1, sy+yl-1, sx, sy+yl-1, color );
	Line( sx+xl-1, sy+yl-1, sx+xl-1, sy, color );
}

void Box( int sx, int sy,  int ex, int ey, int color )
{	
	Line( sx, sy, sx, ey+1, color );
	Line( sx, sy, ex+1, sy, color );
	Line( ex, ey, sx, ey, color );
	Line( ex, ey, ex, sy, color );
}

void BoxFill( int sx, int sy, int xl, int yl, int color)
{
	int i;
	yl += sy;
	for( i = sy ; i < yl ; i ++)
	{
		XLine( sx, sy, xl, color);
	}
}





void Box( int x1, int y1,  int x2, int y2,  int x3, int y3,  int x4, int y4, int color )
{	
	Line( x1, y1, x2, y2, color );
	Line( x2, y2, x3, y3, color );
	Line( x3, y3, x4, y4, color );
	Line( x4, y4, x1, y1, color );
}	



void Circle(int xc,int yc,int a0,int col )
{				
	int x1 = 0 ;
	int y1 = a0;
				
	long a = a0;
	long b = a0;
				
	long Asquared = a*a;
	long TwoAsquared = 2 * Asquared;
	long Bsquared = b * b;
	long TwoBsquared = 2 * Bsquared;
				
	long d;		
	long dx,dy;	
	d = Bsquared - Asquared * b + Asquared/4l;
	dx = 0;		
	dy = TwoAsquared * b;
				
	while(dx < dy)
	{			
        PutDot(xc+x1,yc+y1,col);
        PutDot(xc-x1,yc+y1,col);
        PutDot(xc-x1,yc-y1,col);
        PutDot(xc+x1,yc-y1,col);
		if(d>0l)
		{		
			--y1;
			dy -= TwoAsquared;
			d -= dy;
		}		
		++x1;	
		dx += TwoBsquared;
		d += Bsquared + dx;
	}			
	d += (3l*(Asquared - Bsquared)/2l - (dx + dy)) /2l;
				
	while(y1>=0)
	{			
		PutDot(xc+x1,yc+y1,col);
        PutDot(xc-x1,yc+y1,col);
        PutDot(xc-x1,yc-y1,col);
        PutDot(xc+x1,yc-y1,col);
		
		if(d<0l)
		{		
			++x1;
			dx += TwoBsquared;
			d += dx;
		}		
		-- y1;	
		dy -= TwoAsquared;
		d += Asquared - dy;
	}			
}				




int GetTile( int x, int y )
{
	return TileMap[ x][y].attr_dont;
}

void PutTile( int x, int y, int v )
{
	TileMap[ x][y].attr_dont = v;
}


void PaintTileAttrDont(int x, int y )
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_dont == 0 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_dont == 0 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_dont = 1;
	}
}		

//___________________________________________________________________________ // 010904 LTS DEPAINT GROUP
void DePaintTileAttrDont(int x, int y )   
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_dont == 1 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_dont == 1 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_dont = 0;
	}
}		
//___________________________________________________________________________ // 010904 LTS

void PaintTileAttrRiver(int x, int y )
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_river == 0 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_river == 0 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_river = 1;
	}
}		

//___________________________________________________________________________ // 010904 LTS DEPAINT GROUP
void DePaintTileAttrRiver(int x, int y )             
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_river == 1 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_river == 1 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_river = 0;
	}
}		
//___________________________________________________________________________ // 010904 LTS


void PaintTileAttrGeffect(int x, int y )
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_light == 0 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_light == 0 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_light = 1;
	}
}		
//___________________________________________________________________________ // 010904 LTS DEPAINT GROUP
void DePaintTileAttrGeffect(int x, int y )		
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_light == 1 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_light == 1 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_light = 0;
	}
}		
//___________________________________________________________________________ // 010904 LTS

void PaintTileAttrInside(int x, int y )
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_inside == 0 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_inside == 0 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_inside = 1;
	}
}		
//___________________________________________________________________________ // 010904 LTS DEPAINT GROUP
void DePaintTileAttrInside(int x, int y )		
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_inside == 1 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_inside == 1 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_inside = 0;
	}
}		
//___________________________________________________________________________ // 010904 LTS

void PaintTileAttrNoBattle(int x, int y )
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_no_battle == 0 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_no_battle == 0 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_no_battle = 1;
	}
}		
//___________________________________________________________________________ // 010904 LTS DEPAINT GROUP
void DePaintTileAttrNoBattle(int x, int y )		
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].attr_no_battle == 1 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].attr_no_battle == 1 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].attr_no_battle = 0;
	}
}		
//___________________________________________________________________________ // 010904 LTS

void PaintTileAttrCanMatch(int x, int y )
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].canmatch == 0 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].canmatch == 0 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].canmatch = 1;
	}
}		

//___________________________________________________________________________ // 010904 LTS DEPAINT GROUP
void DePaintTileAttrCanMatch(int x, int y )		
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].canmatch == 1 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].canmatch == 1 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].canmatch = 0;
	}
}	
//___________________________________________________________________________ // 010904 LTS

//___________________________________________________________________________ // 010904 LTS DEPAINT GROUP

void PaintTileAttrTop(int x, int y )  
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].empty__4 == 0 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].empty__4 == 0 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].empty__4 = 1;
	}
}		

void DePaintTileAttrTop(int x, int y )  // 010715 LTS
{	
	int sx, ex;
	
	sx = x;
	while( TileMap[sx][y].empty__4 == 1 && sx >= 0 )	sx--;
	if( sx < 0 ) sx = 0;
	
	ex = x;
	while( TileMap[ex][y].empty__4 == 1 && ex < g_Map.file.wWidth )	ex++;
	if( ex >= g_Map.file.wWidth ) ex = g_Map.file.wWidth -1;
	
	for( ; sx <= ex ; sx ++)
	{
		TileMap[ sx][y].empty__4 = 0;
	}
}		
//___________________________________________________________________________ // 010904 LTS

void DoSomeThingWithKey( WPARAM wParam, LPARAM lParam )
{			
	switch(wParam)
	{
	case 'H':
		{
			if(EWndMgr.IsFocus(HWND_GAME_MAIN))
			{
				ViewTipToggle = !ViewTipToggle;
			}
		}break;
	case VK_F12:
		{
			CaptureScreenForJpg();
		}break;
	default:
		{
		}break;
	}
#ifdef _DEBUG
	if( !GetSysInfo( SI_GAME_MAKE_MODE ) ) return;
	
	switch( wParam )									
	{						
	case VK_PRIOR :	if( tMapAttr_Offset_Modify == IDC_ATTR_OFFSET_YES2 )
					{
						TileMap[tMapAttrX][tMapAttrY].frontoffset -=1;
					}
		else
		{
			AddSkillCursor( Hero, 1, rand()%30, 150+ Mapx,200 +Mapy, 1);
		}
		break;		
		
	case VK_NEXT :	if( tMapAttr_Offset_Modify == IDC_ATTR_OFFSET_YES2 )
					{
						TileMap[tMapAttrX][tMapAttrY].frontoffset +=1;
					}
		break;		
		
	case VK_F2 :
		{
		}break;
	case VK_F3 : 
		Hero->nCharacterData[ MOVP ] += 10;
		break;
		
	case VK_F4 : 
		
		LoadCursorAni();
		LoadMouseCursor( "Cursor.Spr" );
		JustMsg(" CursorAni.txt를 읽었습니다." );
		break;
		
	case VK_F5 :	
		{
			
			struct tm *now;
			time_t nowTime;
			short int hour, min, sec;
			time( &nowTime );
			now = localtime(&nowTime);
			hour = now->tm_hour;
			min  = now->tm_min;
			sec  = now->tm_sec;
			
			// PJH 0422
			FILE *fp;
			fp = fopen( "CharacterInfo.txt", "at+" );
			if( fp )
			{
				if( Hero == NULL )
				{
					fprintf( fp, " --- CharacterInfo --- \n Hero == NULL !!!!\n" );
					fclose(fp);
				}
				else
				{
					fprintf( fp, " --- CharacterInfo(%2d시%2d분%2d초) --- \n name = %s\n position(x,y) = (%d, %d),(%d, %d),(%d, %d) \n condition = %d\n ",
						hour,min,sec,
						Hero->name,
						Hero->x,Hero->y,Hero->gox,Hero->goy,		
						Hero->destx,Hero->desty,
						Hero->condition
						);			
					
					int sprc	= Hero->AnimationTable[ Hero->nCurrentAction].cFrame[  Hero->nCurrentFrame.GetDecrypted() ];
					int sprno	= Hero->sprno;
					int lev = Hero->lv;
					int ac0		= Hero->accessory[0];
					int ac1		= Hero->accessory[1];
					int ac2		= Hero->accessory[2];
					int ac3		= Hero->accessory[3];
					
					Hero->sp		= &CharSpr[ sprno ].sp[ Hero->direction][ sprc];
					
					fprintf( fp, "nCurrentAction=%d nCurrentFrame=%d sprc=%d sprno=%d dir=%d sp=%p \n\n", 
						Hero->nCurrentAction, 
						Hero->nCurrentFrame.GetDecrypted(),
						sprc,	
						sprno,	
						Hero->direction,
						Hero->sp );
					
					fclose(fp);	
				}				
			}
		}
		break;
		
		
	case VK_F6 :
		
		
		if (! pMusic->OpenFile("A.MP3") )
			MessageBox( g_hwndMain, "Visual Play Error","error",MB_ICONEXCLAMATION|MB_OK);
		else pMusic->Play();
		
		/*
		t_just_ani tp;
		
		  tp.id = 10008;
		  tp.ani = 1;
		  
			extern void Recv_JustAni( t_just_ani  *tp );
			Recv_JustAni( &tp );
		*/			
		
		break;
		
	case VK_F7 :	pMusic->Stop();
		
	case VK_F8 : 
		break;    
		
	case VK_F9 : 
		// acer4
		if( !Hero ) break;
		DestroyCharacterListExceptHero();
		OutputSmallMap();
		break;	  	
	}
#endif
}																	

void CheckCharacterFrameOrder( WPARAM wParam, LPARAM lParam )
{					
	int i,j;		
	
	if( SysInfo.viewcharacterframe == 0 ) return;
	if( AnimationEditWorkStartFlag == 1 ) return;
	
	
	switch( wParam )
	{		
	case VK_DOWN :	SpriteOrderDir += 7;
		SpriteOrderDir = (SpriteOrderDir) % 8;	break;
	case VK_UP   :	SpriteOrderDir = (++SpriteOrderDir) % 8;	break;
		
	case VK_LEFT :	SpriteOrderFrame--; if( SpriteOrderFrame < 0 )		SpriteOrderFrame = 74;
		
		break;	
		
	case VK_RIGHT :  SpriteOrderFrame++; if( SpriteOrderFrame >= 75 )	SpriteOrderFrame = 0;
	/*					if( SpriteOrderFrame < 27 ) // 한손.. 방패.
	{
	Hero->accessory[2] = 61;
	Hero->accessory[3] = 87;
	}
	else if( 27 <= SpriteOrderFrame && SpriteOrderFrame < 35 )  // 양손.
	{
	Hero->accessory[2] = 74;
	Hero->accessory[3] = 0;
	}
	else if( 36 <= SpriteOrderFrame && SpriteOrderFrame < 71 ) // 활동작..
	{
	Hero->accessory[2] = 61;
	Hero->accessory[3] = 87;
	}
	else if( 71 <= SpriteOrderFrame && SpriteOrderFrame < 75 ) 
	{
	Hero->accessory[2] = 74;
	Hero->accessory[3] = 0;
	}*/
		break;	
		
	case ' ' :
	case 'Z' :
	case 'z' :  SpriteOrder[Hero->sprno][ SpriteOrderDir][ SpriteOrderFrame] ++;
		if( SpriteOrder[Hero->sprno][ SpriteOrderDir][ SpriteOrderFrame] > 5 )
			SpriteOrder[Hero->sprno][ SpriteOrderDir][ SpriteOrderFrame] = 0;
		break;
		
	case 's' :		
	case 'S' :  
		{		
			FILE *fp;
			char tempfilename[ FILENAME_MAX];
			sprintf( tempfilename, "./char/%02dClothOrder.bin",  Hero->sprno );
			fp = Fopen( tempfilename, "wb" );
			if( fp )
			{	
				for( i = 0 ; i < 8 ; i ++)
				{
					for( j = 0 ; j < 75 ; j ++)
					{
						fwrite( &SpriteOrder[Hero->sprno][i][j], 1,1, fp );
					}
				}
				fclose(fp);
				JustMsg( tempfilename );
			}	
		}		
		break;	
	}	
}		


//----------------------------------------------------------------------------------


//-----------------------   Mode Setting   ----------------
//-----------------------   Mode Setting   ----------------
//-----------------------   Mode Setting   ----------------
//-----------------------   Mode Setting   ----------------
//-----------------------   Mode Setting   ----------------



#define SM_ReadInt(x)	fscanf (fp, "%d", &##x); 
#define SM_WriteInt(x)	fprintf (fp, "%d\n", ##x);

#define DEVELOPEMENT_ENVIR_FILENAME	"dragonrajaonline_hongdoit.txt"

void LoadInitialData (void)
{		
	SysInfo.music    = 1;		// 1이면 음악을 켬 
	SysInfo.effect   = 1;		// 1이면 효과을 켬 
	
	//	return;//
	
#ifdef _DEBUG
	
	
	FILE *fp = Fopen ( DEVELOPEMENT_ENVIR_FILENAME, "rt");
	if( fp )
	{	
		SM_ReadInt (SysInfo.dx);
		SM_ReadInt (SysInfo.notdead );
		SM_ReadInt (SysInfo.music);
		SM_ReadInt (SysInfo.effect);
		SM_ReadInt (SysInfo.cddata);
		SM_ReadInt (SysInfo.onekill );
		SM_ReadInt (SysInfo.gamemakemode );
		SM_ReadInt (SysInfo.notconectserver );
		SM_ReadInt (SysInfo.enable_menu );
		SM_ReadInt (SysInfo.viewcharacterframe );
		SM_ReadInt (SysInfo.gamemakemode );
		SM_ReadInt (SysInfo.loaditemfromtoi2 );
		SM_ReadInt (SysInfo.makeanitable_bin );
		SM_ReadInt (SysInfo.port );
		SM_ReadInt (SysInfo.entergameserver );
		SM_ReadInt (SysInfo.enteragent );
		SM_ReadInt (SysInfo.enterproxy );
		fscanf( fp, "%s\n", MapName );
		fscanf( fp, "%s\n", SysInfo.ip );
		fscanf( fp, "%s\n", SysInfo. agentip );
		fscanf( fp, "%s\n", SysInfo. proxyip );
		SM_ReadInt (SysInfo.keyr );
		SM_ReadInt (SysInfo.keyp );
		SM_ReadInt (SysInfo.mapx );
		SM_ReadInt (SysInfo.mapy );
		SM_ReadInt (SysInfo.makesou2size );
		
		fclose (fp);
	}	
	else
	{	
		
#endif
		
		//SysInfo.dx		 = 0;		// 1이면 전체모드,		0 : 창모드		// switch screen
		SysInfo.notdead	 = 0;		// 0 : normal           1 : 주인공은 무적
		SysInfo.music    = 1;		// 1이면 음악을 켬 
		SysInfo.effect   = 1;		// 1이면 효과을 켬 
		SysInfo.cddata   = 0;		// 0 : read from CD		1 : From hard
		SysInfo.onekill  = 0;		// 0 : normal			1 : onekill.
		SysInfo.gamemakemode= 0;		// 0 : no debug info.	1 : show debug info.
		SysInfo.notconectserver  = 0;	// 0 : conect server	1 : not conect
		SysInfo.enable_menu = 0;	// 0 : conect server	1 : not conect
		SysInfo.gamemakemode = 0;	// 0 : conect server	1 : not conect
		SysInfo.loaditemfromtoi2 = 0;
		
#ifdef _DEBUG
	}	
#endif
}		



void WriteInitialData (void)
{		
	FILE *fp = Fopen ( DEVELOPEMENT_ENVIR_FILENAME, "wt");
	if( fp )
	{	
		SM_WriteInt (SysInfo.dx);
		SM_WriteInt (SysInfo.notdead );
		SM_WriteInt (SysInfo.music);
		SM_WriteInt (SysInfo.effect);
		SM_WriteInt (SysInfo.cddata);
		SM_WriteInt (SysInfo.onekill );
		SM_WriteInt (SysInfo.gamemakemode );
		SM_WriteInt (SysInfo.notconectserver );
		SM_WriteInt (SysInfo.enable_menu );
		SM_WriteInt (SysInfo.viewcharacterframe );
		SM_WriteInt (SysInfo.gamemakemode );
		SM_WriteInt (SysInfo.loaditemfromtoi2 );
		SM_WriteInt (SysInfo.makeanitable_bin );
		SM_WriteInt (SysInfo.port );
		SM_WriteInt (SysInfo.entergameserver );
		SM_WriteInt (SysInfo.enteragent );
		SM_WriteInt (SysInfo.enterproxy );
		fprintf( fp, "%s\n", MapName );
		fprintf( fp, "%s\n", SysInfo.ip );
		fprintf( fp, "%s\n", SysInfo.agentip );
		fprintf( fp, "%s\n", SysInfo.proxyip );
		SM_WriteInt (SysInfo.keyr );
		SM_WriteInt (SysInfo.keyp );
		SM_WriteInt (SysInfo.mapx );
		SM_WriteInt (SysInfo.mapy );
		SM_WriteInt (SysInfo.makesou2size );
		
		fclose (fp);
	}	
}		



BOOL CALLBACK ModeSettingProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{						
	char temp[ FILENAME_MAX];
	
	switch(Message)		
	{					
	case WM_INITDIALOG:
		LoadInitialData ();
		CheckRadioButton( hDlg, IDC_RADIO1, IDC_RADIO2, SysInfo.dx		? IDC_RADIO1 : IDC_RADIO2 );
		CheckDlgButton( hDlg , IDC_CHECK_SUPER_HERO,	SysInfo.notdead	? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_CHECK_SOUND,			SysInfo.music   ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_CHECK_EFFECT_WAVE,	SysInfo.effect	? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_CHECK_READ_CDDATA,	SysInfo.cddata	? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_CHECK_ONE_KILL,		SysInfo.onekill	? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_CHECK_CONECT_SERVER,	SysInfo.notconectserver ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_CHECK_ENABLE_MENU,	SysInfo.enable_menu ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_CHECK_VIEWCHARACTERFRAME,	SysInfo.viewcharacterframe? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_CHECK_GAMEMAKEMODE ,	SysInfo.gamemakemode ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_LOAD_ITEM_FROM_TOI2,	SysInfo.loaditemfromtoi2 ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_SAVE_ANIMATION_TABLE,SysInfo.makeanitable_bin ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_ENTER_AGENTIP,		SysInfo.enteragent ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_ENTER_PROXYIP,		SysInfo.enterproxy ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_ENTER_GAMESERVER,	SysInfo.entergameserver ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_KEY_RECORD ,	SysInfo.keyr ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_KEY_PLAY ,	SysInfo.keyp ? MF_CHECKED : MF_UNCHECKED );
		CheckDlgButton( hDlg , IDC_MAP_SCROLL_TYPE ,	SysInfo.mapscrolltype ? MF_CHECKED : MF_UNCHECKED );
		
		Edit_SetText (GetDlgItem(hDlg, IDC_MAPNAME ), MapName );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_IP ), SysInfo.ip );
		Edit_SetText (GetDlgItem(hDlg, IDC_AGENTIP ), SysInfo.agentip );
		Edit_SetText (GetDlgItem(hDlg, IDC_PROXYIP ), SysInfo.proxyip );
		sprintf (temp, "%d", SysInfo.port );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_PORT ), temp );
		
		sprintf (temp, "%d", SysInfo.mapx );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_MAPX ), temp );
		sprintf (temp, "%d", SysInfo.mapy );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_MAPY ), temp );
		
		CheckDlgButton( hDlg , IDC_MAKE_SOU2_SIZE,	SysInfo.makesou2size? MF_CHECKED : MF_UNCHECKED );
		
		
		
		
		//						Edit_SetText (GetDlgItem(hDlg, IDC_EDIT7), SysInfo.notepad );
		return TRUE;			
								
	case WM_PAINT:						
		break;						
		
	case WM_COMMAND:			
		switch( LOWORD( wParam )) 
		{					
		case  IDOK:	SysInfo.dx		 = IsDlgButtonChecked( hDlg, IDC_RADIO1            );
			if( SysInfo.dx == 1 )	g_DirectDrawInfo.bFullscreen = TRUE;
			
			SysInfo.notdead			= IsDlgButtonChecked( hDlg, IDC_CHECK_SUPER_HERO  );
			SysInfo.music			= IsDlgButtonChecked( hDlg, IDC_CHECK_SOUND       );
			SysInfo.effect			= IsDlgButtonChecked( hDlg, IDC_CHECK_EFFECT_WAVE );
			SysInfo.cddata			= IsDlgButtonChecked( hDlg, IDC_CHECK_READ_CDDATA );
			SysInfo.onekill			= IsDlgButtonChecked( hDlg, IDC_CHECK_ONE_KILL    );
			SysInfo.gamemakemode	= IsDlgButtonChecked( hDlg, IDC_CHECK_DEBUGINFO   );
			SysInfo.notconectserver	= IsDlgButtonChecked( hDlg, IDC_CHECK_CONECT_SERVER );
			SysInfo.enable_menu		= IsDlgButtonChecked( hDlg, IDC_CHECK_ENABLE_MENU );
			SysInfo.viewcharacterframe = IsDlgButtonChecked( hDlg, IDC_CHECK_VIEWCHARACTERFRAME );
			SysInfo.gamemakemode	= IsDlgButtonChecked( hDlg, IDC_CHECK_GAMEMAKEMODE );
			SysInfo.loaditemfromtoi2= IsDlgButtonChecked( hDlg, IDC_LOAD_ITEM_FROM_TOI2 );
			SysInfo.makeanitable_bin= IsDlgButtonChecked( hDlg, IDC_SAVE_ANIMATION_TABLE );
			SysInfo.keyr=				IsDlgButtonChecked( hDlg, IDC_KEY_RECORD );
			SysInfo.keyp=				IsDlgButtonChecked( hDlg, IDC_KEY_PLAY );
			SysInfo.mapscrolltype   =	IsDlgButtonChecked( hDlg, IDC_MAP_SCROLL_TYPE );
			
			Edit_GetText( GetDlgItem(hDlg, IDC_MAPNAME), MapName, FILENAME_MAX);
			
			SysInfo.entergameserver  = IsDlgButtonChecked( hDlg, IDC_ENTER_GAMESERVER );
			SysInfo.enteragent		 = IsDlgButtonChecked( hDlg, IDC_ENTER_AGENTIP );
			SysInfo.enterproxy		 = IsDlgButtonChecked( hDlg, IDC_ENTER_PROXYIP );
			
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_IP ),  SysInfo.ip, IP_LENGTH );
			Edit_GetText( GetDlgItem(hDlg, IDC_AGENTIP ),  SysInfo.agentip, IP_LENGTH );
			Edit_GetText( GetDlgItem(hDlg, IDC_PROXYIP ),  SysInfo.proxyip, IP_LENGTH );
			
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_PORT ), temp, FILENAME_MAX);
			SysInfo.port = atoi( temp );
			
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_MAPX ), temp, FILENAME_MAX);
			SysInfo.mapx = atoi( temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_MAPY ), temp, FILENAME_MAX);
			SysInfo.mapy = atoi( temp );
			
			SysInfo.makesou2size = IsDlgButtonChecked( hDlg, IDC_MAKE_SOU2_SIZE );
#ifdef _DEBUG // finito 060507		
			if( GetSysInfo( SI_GAME_MAKE_MODE ) ) WriteInitialData ();
#endif
			SysInfo.result = 1;
			
			EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
			
		case IDCANCEL :									
			SysInfo.result = 999;
			EndDialog(hDlg, LOWORD(wParam));	
			
			return (TRUE);									
		}											
		break;										
	}													
	return FALSE;									
}													

//#define _SWITCH_SCREEN

void ModeSetting ( HWND hwnd, HINSTANCE hInstance)
{
#ifdef _DEBUG
	FILE *fp = Fopen( DEVELOPEMENT_ENVIR_FILENAME, "rb" );
	if(fp) {
		fclose(fp);
		DialogBox( hInstance, MAKEINTRESOURCE( IDD_DEVELOPE_ENVIR ), NULL, (DLGPROC)ModeSettingProc );
	}
#else
	AGAIN_:

	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DEVELOPE_ENVIR1 ), NULL, (DLGPROC)ModeSettingProc );
	
	if( SysInfo.result == 1 &&  SysInfo.dx == 0 )  // 실행하기 위해 OK를 눌렀다. 
	{
		// Finito Added a check to see if use|r is using a colour mode which is over 16 bits
		if( GetBitsPerPixel() <= 8 || GetBitsPerPixel() > 16) 
		{	
			JustMsg ( "You need to be using 16bit( 65536 colors) mode if you want to run the game in a window!\n" );
			goto AGAIN_;
		}
	}
#endif
/*#else 
	
	
	SysInfo.music = 1;
	SysInfo.effect = 1;
	
#ifdef _SWITCH_SCREEN
	SysInfo.dx = 0;		// switch screen
	g_DirectDrawInfo.bFullscreen = false;// switch screen
#else
	SysInfo.dx = 1;		// switch screen
	g_DirectDrawInfo.bFullscreen = true;// switch screen
#endif
	
#endif*/
}		


//------------------------------------------------------------------------------------
// Animation을 check하기 위한 루틴...

BOOL CALLBACK CheckEquipmentProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{	
	LPCHARACTER lpChar;
	char		temp[FILENAME_MAX];
	int	*ac;
	RECT rect, grect;
	static int id = 1;
	
	lpChar = ReturnCharacterID( id );
#ifdef _DEBUG // finito 060507	
	if( SysInfo.notconectserver == 1 )
	{
		if( lpChar == NULL ) 
		{
			id =0;
			lpChar = Hero;
			JustMsg( " 그런  ID는 없어요.   ID를 0번으로 바꿉니다. " );
		}
	}
	else
#endif
	{
		if( lpChar == NULL ) 
		{
			id =1;
			lpChar = Hero;
			JustMsg( " 그런  ID는 없어요.   ID를 1번으로 바꿉니다. " );
		}
	}
	
	
	ac= lpChar->accessory;
	
	switch(Message)		
	{					
	case WM_INITDIALOG:	CheckRadioButton( hDlg, IDC_RADIO_1_FEMALE, IDC_RADIO_1_MALE , lpChar->sprno ? IDC_RADIO_1_MALE : IDC_RADIO_1_FEMALE );
		
		sprintf (temp, "%d", ac[0] );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT1), temp);
		sprintf (temp, "%d", ac[2]);
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT4), temp);
		sprintf (temp, "%d", ac[3]);
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT5), temp);
		sprintf( temp, "%d", id );
		Edit_SetText (GetDlgItem(hDlg, IDC_CHAR_NO), temp);
		
		ShowWindow( hDlg , SW_HIDE);
		GetWindowRect( hDlg, &rect );
		GetWindowRect( g_hwndMain, &grect );
		MoveWindow( hDlg, grect.right+1, 0, rect.right, rect.bottom, true);
		
		return TRUE;	
		
	case WM_PAINT:		
		break;			
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{			
		case  IDOK :
			
			sprintf( temp, "%d", id );
			Edit_GetText (GetDlgItem(hDlg, IDC_CHAR_NO), temp, 9);
			id = atoi( temp );
			lpChar = ReturnCharacterID( id );
			if( lpChar == NULL ) 
			{
				id =1;
				lpChar = Hero;
				JustMsg( " 그런  ID는 없어요.   ID를 1번으로 바꿉니다. " );
			}
			ac= lpChar->accessory;
			
			
			lpChar->sprno		 = IsDlgButtonChecked( hDlg, IDC_RADIO_1_MALE  );
			
			if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_NO ) )
			{
				lpChar->basicAction = ACTION_NONE; 
				lpChar->basicWalk	= ACTION_BASIC_WALK; 
			}
			if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_ONESWORD ) )
			{
				lpChar->basicAction = ACTION_BATTLE_NONE; 
				lpChar->basicWalk	= ACTION_BATTLE_WALK_OWNHAND; 	
			}
			if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_TWOSWORD ) )
			{	
				lpChar->basicAction = ACTION_BATTLE_NONE_TWOHAND;
				lpChar->basicWalk = ACTION_BATTLE_WALK_TWOHAND;
			}
			if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_PENSING ) )
			{	
				lpChar->basicAction = ACTION_ATTACK_PENSWORD2;
				lpChar->basicWalk = ACTION_BATTLE_WALK_OWNHAND;
			}
			
			if( IsDlgButtonChecked( hDlg, IDC_CHECK_1_VIEW_PART ) )
				CheckEquipmentFlag = TRUE;
			else
				CheckEquipmentFlag = FALSE;
			
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT1), temp, 9);
			ac[0] = atoi (temp);
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT4), temp, 9);
			ac[2] = atoi (temp);
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT5), temp, 9);
			ac[3] = atoi (temp);
			
			////
			
			LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[0]);
			
			{
				
				LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[0]+1);
				//						LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[0]+200);		// NEW_CHAR YGI
				//						LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[0]+201);
			}
			
			{
				//						FreeHeroClothAccessoryDataOne( lpChar->sprno, old[2] );
				LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[2]);
			}
			
			{
				//						FreeHeroClothAccessoryDataOne( lpChar->sprno, old[3] );
				LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[3]);
			}
			
			////					
			
			//EndDialog(hDlg, LOWORD(wParam));
			GameTestProc( &g_DirectDrawInfo );
			return (TRUE);
			
		case IDCANCEL :	
			EndDialog(hDlg, LOWORD(wParam));	
			
			return (TRUE);
		}		
		break;		
	}				
	return FALSE;	
}					


void CheckEquipmentData( HWND hwnd, HINSTANCE hInstance)
{		
	HWND  hCreateCheckEquipment =
		CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_CHECK_EQUIP ), g_hwndMain, (DLGPROC)CheckEquipmentProc );
	ShowWindow( hCreateCheckEquipment, SW_HIDE );
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hCreateCheckEquipment, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow(hCreateCheckEquipment, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow(hCreateCheckEquipment, SW_SHOW);
	
}		


//------------------------------------------------------------------------------------

void GetAnimationTxt( LPCHARACTER lpChar, HWND hwnd )
{   //< CSD-031014    
	int  i;
	char temp[FILENAME_MAX];
	int	 AnotherLineCount, LineCount = Edit_GetLineCount( hwnd ) -1;
	int  CharCount;
	int  frame, dx, dy, attackf, wave, height, effect;
	
	lpChar->AnimationTable[ TempAnimationCount].nCount = LineCount;
	
	AnotherLineCount = 0;
	for( i = 0; i < LineCount ; i++)
	{	
		CharCount = Edit_GetLine( hwnd, i, temp, FILENAME_MAX );	// 030812 YGI
		
		if (temp[CharCount-2] == '\r')
		{
			temp[CharCount-2] = '\n';
			temp[CharCount-1] = '\0';
		}
		else 
		{
			temp[CharCount] = '\0';
		}
		
		sscanf( temp, "%d %d %d %d %d %d %d ", &frame, &dx, &dy, &attackf, &wave, &height, &effect );
		
		lpChar->AnimationTable[ TempAnimationCount].cFrame[ i]	= frame -1;
		lpChar->AnimationTable[ TempAnimationCount].dx[ i]		= dx;
		lpChar->AnimationTable[ TempAnimationCount].dy[ i]		= dy;
		lpChar->AnimationTable[ TempAnimationCount].attackf[ i]	= attackf;
		lpChar->AnimationTable[ TempAnimationCount].wave[ i]	= wave;
		lpChar->AnimationTable[ TempAnimationCount].height[ i]	= height;
		lpChar->AnimationTable[ TempAnimationCount].effect[ i]	= effect;
		
		
		AnotherLineCount ++;
	}	
	
	if( AnotherLineCount != LineCount ) 	JustMsg( "라인수가 달라.." );
	lpChar->AnimationTable[ TempAnimationCount].nCount = LineCount;
}	


void SetAnimationTxt( LPCHARACTER lpChar, HWND hDlg )
{	
	char temp[ FILENAME_MAX];
	int  i;
#ifdef _DEBUG
	sprintf (temp, "%d-%s", TempAnimationCount+1, AnimationName[ TempAnimationCount] );
#endif
	Edit_SetText ( GetDlgItem(hDlg, IDC_ANI_TITLE ), temp);
	
	SendMessage(GetDlgItem(hDlg, IDC_ANI_EDIT ), WM_SETTEXT, 0, (LPARAM)"");
	for( i = 0 ; i < lpChar->AnimationTable[ TempAnimationCount].nCount ; i ++)
	{
		sprintf( temp,"%5d     %5d  %5d     %5d     %5d     %5d      %5d\n", 
			lpChar->AnimationTable[ TempAnimationCount].cFrame[ i]+1,
			lpChar->AnimationTable[ TempAnimationCount].dx[ i],
			lpChar->AnimationTable[ TempAnimationCount].dy[ i],
			lpChar->AnimationTable[ TempAnimationCount].attackf[ i],
			lpChar->AnimationTable[ TempAnimationCount].wave[ i],
			lpChar->AnimationTable[ TempAnimationCount].height[ i],
			lpChar->AnimationTable[ TempAnimationCount].effect[ i] );
		
		Edit_ReplaceSel(GetDlgItem(hDlg, IDC_ANI_EDIT ), (LPARAM)temp);
	}	
}	//> CSD-031014	



void PutClipBoard( char *s )
{	
	int len = strlen( s );
	HGLOBAL  hGlobalMemory;
	char *pGlobalMemory;
	
	hGlobalMemory = GlobalAlloc( GHND, len + 1 );
	if( hGlobalMemory )
	{
		pGlobalMemory = (char *)GlobalLock( hGlobalMemory );
		strcpy( pGlobalMemory, s );
		GlobalUnlock( hGlobalMemory );
		OpenClipboard( g_hwndMain );
		EmptyClipboard();
		SetClipboardData( CF_TEXT, hGlobalMemory );
		CloseClipboard();
		GlobalFree( hGlobalMemory );
	}
}	


extern int g_bToolViewPlusEquip;	// NEW_CHAR YGI		

BOOL CALLBACK CheckAnimationProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{											
	LPCHARACTER lpChar = Hero->lpNext;
	char		temp[FILENAME_MAX];
	static int oldsprno;
	int i,j;			
	FILE *fp;			
	int  ScreenX;		
	RECT rect, grect;		
	int	*ac= lpChar->accessory;
	int old[4]; 
	switch(Message)		
	{						
	case WM_INITDIALOG:	sprintf (temp, "%d", lpChar->sprno );
		Edit_SetText (GetDlgItem(hDlg, IDC_CHAR_NO ), temp);
		oldsprno = lpChar->sprno;
		sprintf(temp, "%d", lpChar->mutant);
		Edit_SetText(GetDlgItem(hDlg, EDT_CHAR_MUTANT), temp);
		sprintf (temp, "%d", lpChar->basicAction+1);
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_BASIC ), temp);
		tool_AnimationEditTotalSpriteNo = 0;
		tool_AnimationEditCurFrame = 0;
		sprintf( temp, "%d", tool_AnimationEditCurFrame +1 );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_CURFRAME ), temp);
		
		if( oldsprno <= 1 )
		{
			sprintf (temp, "%d", ac[0] );
			Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ACC0), temp);
			sprintf (temp, "%d", ac[1]);
			Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ACC1), temp);
			sprintf (temp, "%d", ac[2]);
			Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ACC2), temp);
			sprintf (temp, "%d", ac[3]);
			Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ACC3), temp);
		}
		
		
		CheckAnimationHwnd = GetDlgItem(hDlg, IDC_ANI_EDIT );
		
		ScreenX = GetSystemMetrics(SM_CXSCREEN);
		//ShowWindow( CheckAnimationHwnd, SW_HIDE);
		GetWindowRect( hDlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
		GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
		MoveWindow( hDlg, grect.right+1, 0, rect.right, rect.bottom, true);
		TempAnimationCount = 0;
		SetAnimationTxt( lpChar, hDlg );
		
		Button_SetCheck( GetDlgItem( hDlg, IDC_PLUS_EQUIP ), 1 );
		Button_SetCheck(GetDlgItem(hDlg, CHK_NPC_HEADER), 0);
		Button_SetCheck(GetDlgItem(hDlg, CHK_NPC_BODY), 0);
		Button_SetCheck(GetDlgItem(hDlg, CHK_NPC_WEAPON), 0);
		Button_SetCheck(GetDlgItem(hDlg, CHK_EFFECT_ONE), 0);
		Button_SetCheck(GetDlgItem(hDlg, CHK_EFFECT_TWO), 0);
		return TRUE;	
		
	case WM_PAINT:		
		break;			
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{		
			
			/////////////////////////////////////////////////////////
		case IDC_PLUS_EQUIP :
			{
				g_bToolViewPlusEquip = Button_GetCheck( GetDlgItem( hDlg, IDC_PLUS_EQUIP ) );
				EndOfAnimationFlag = 0;
				while( EndOfAnimationFlag != 3 )
				{
					GameTestProc( &g_DirectDrawInfo );
				}
				
				break;
			}
			
		case IDC_CHANGE_EQUIP :
			
			if( lpChar->sprno <= 1 )
			{
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_NO ) )
				{
					lpChar->basicAction = ACTION_NONE; 
					lpChar->basicWalk	= ACTION_BASIC_WALK; 
				}
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_ONESWORD ) )
				{
					lpChar->basicAction = ACTION_BATTLE_NONE; 
					lpChar->basicWalk	= ACTION_BATTLE_WALK_OWNHAND; 	
				}
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_TWOSWORD ) )
				{	
					lpChar->basicAction = ACTION_BATTLE_NONE_TWOHAND;
					lpChar->basicWalk = ACTION_BATTLE_WALK_TWOHAND;
				}
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_PENSING ) )
				{	
					lpChar->basicAction = ACTION_ATTACK_PENSWORD2;
					lpChar->basicWalk = ACTION_BATTLE_WALK_OWNHAND;
				}
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_1_BONG ) )
				{	
					lpChar->basicAction = ACTION_BATTLE_NONE;
					lpChar->basicWalk = ACTION_BATTLE_WALK_OWNHAND;
				}
				
				
				Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_ACC0 ), temp, 9);
				old[0] = ac[0]; ac[0] = atoi (temp);
				Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_ACC1 ), temp, 9);
				old[1] = ac[1]; ac[1] = atoi (temp);
				Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_ACC2 ), temp, 9);
				old[2] = ac[2]; ac[2] = atoi (temp);
				Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_ACC3 ), temp, 9);
				old[3] = ac[3]; ac[3] = atoi (temp);
				
				
				if( old[0] != ac[0] )
				{
					FreeHeroClothAccessoryDataOne( lpChar->sprno, old[0] );
					LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[0]);
					FreeHeroClothAccessoryDataOne( lpChar->sprno, old[0] +1);
					LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[0]+1);
					
					//FreeHeroClothAccessoryDataOne( lpChar->sprno, old[0]+200 );			// NEW_CHAR YGI
					//LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[0]+200);
					//FreeHeroClothAccessoryDataOne( lpChar->sprno, old[0] +201);
					//LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[0]+201);
				}
				if( old[1] != ac[1] )
				{
					FreeHeroClothAccessoryDataOne( lpChar->sprno, old[1] );
					LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[1]);
				}
				if( old[2] != ac[2] )
				{
					FreeHeroClothAccessoryDataOne( lpChar->sprno, old[2] );
					LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[2]);
				}
				if( old[3] != ac[3] )
				{
					FreeHeroClothAccessoryDataOne( lpChar->sprno, old[3] );
					LoadHeroClothAccessoryDataOne( lpChar->sprno, ac[3]);
				}
			}
			else
			{
				JustMsg( "몬스터나 NPC는 장비를 장착할수 없습니다. " );
			}
			
			break;
			
			///////////////////////////////////////////////////////////////
			
			
		case ID_ANIMATION_LOAD  : 
			{
				LoadChacterAnimationData_Tool(lpChar);
				
				break;
			}
		case ID_ANIMATION_SAVE	:	
			{
				sprintf( temp, "./char_test/%03dAniTable.txt", lpChar->sprno );
				//				sprintf( temp, "t.txt", lpChar->sprno );
				
				fp = Fopen( temp, "wt" );
				
				for( i = 0 ; i < lpChar->TotalAni; i ++)
				{
#ifdef _DEBUG
					fprintf( fp, "#%s\n", AnimationName[i] );
#endif
					
					for( j = 0 ; j < lpChar->AnimationTable[i].nCount ; j ++)
					{
						fprintf( fp, "%d %3d %3d %3d %3d %3d %3d  0  0  0  0\n", 
							lpChar->AnimationTable[i].cFrame[ j]+1,
							lpChar->AnimationTable[i].dx[ j],
							lpChar->AnimationTable[i].dy[ j],
							lpChar->AnimationTable[i].attackf[ j],
							lpChar->AnimationTable[i].wave[ j],
							lpChar->AnimationTable[i].height[ j],
							lpChar->AnimationTable[i].effect[ j] );
					}
					fprintf( fp, "\n\n\n" );
				}
				
				fclose(fp);
				
				JustMsg( "%d 번 캐릭터의 Animation Data를 저장합니다. \n \n[%s]", lpChar->sprno, temp );
				
				break;
			}		
			
		case IDC_CHARACTER_LOAD :
			{
				lpChar->animationType	= ANIMATION_TYPE_TEST;
				lpChar->peacests = 1;
				
				Edit_GetText(GetDlgItem(hDlg, IDC_CHAR_NO), temp, 9);
				lpChar->sprno = atoi(temp);
				
				Edit_GetText(GetDlgItem(hDlg, EDT_CHAR_MUTANT), temp, 9);
				lpChar->mutant = atoi(temp);
				
				sprintf( temp, "./CHAR/%03d000.spr", lpChar->sprno );
				if( CharBuf[lpChar->sprno] != NULL ) {	FreeCharacterSpriteData( CharBuf[lpChar->sprno] ); CharBuf[lpChar->sprno] = 0; }
				tool_AnimationEditTotalSpriteNo = LoadCharSpriteData( temp,  &CharBuf[lpChar->sprno], &CharBufLength[lpChar->sprno], &CharSpr[ lpChar->sprno], LD_CHAR_SPRITE_ALL_LOAD );
				LoadChacterAnimationData_Tool( lpChar );
				
				const int nMutant = lpChar->mutant;
				const int nSprNo = lpChar->sprno;
				
				for (int i = 0; i < MAX_CHARACTOR_PART_; i++)
				{
					if (CharLayerBuf[nSprNo][i][nMutant] == NULL)
					{
						char szBuffer[FILENAME_MAX];
						sprintf(szBuffer, "./CHAR/%03d%01d%02d.spr", nSprNo, i, nMutant);
						LoadCharSpriteData(szBuffer, 
							&CharLayerBuf[nSprNo][i][nMutant],
							&CharLayerBufLength[nSprNo][i][nMutant],
							&CharLayerSpr[nSprNo][i][nMutant], 
							LD_CHAR_SPRITE_ALL_LOAD);
						CharLayerBufCount[nSprNo][i][nMutant] = 1;
					}
				}
				
				TempAnimationCount = 0;
				SetAnimationTxt( lpChar, hDlg );
				
				if( lpChar->sprno <= 1 )
				{
					sprintf (temp, "%d", ac[0]);	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ACC0 ), temp);
					sprintf (temp, "%d", ac[1]);	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ACC1 ), temp);
					sprintf (temp, "%d", ac[2]);	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ACC2 ), temp);
					sprintf (temp, "%d", ac[3]);	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ACC3 ), temp);
				}
				tool_AnimationEditCurFrame = 0;
				sprintf( temp, "%d", tool_AnimationEditCurFrame +1);
				Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_CURFRAME ), temp);
				
				
				break;
			}
		case IDC_NEXT_ANIMATION :	if( TempAnimationCount < lpChar->TotalAni - 1 ) 
									{
										GetAnimationTxt( lpChar, CheckAnimationHwnd );
										TempAnimationCount ++;
										SetAnimationTxt( lpChar, hDlg );
									}
			break;	
			
		case IDC_PRI_ANIMATION	:	if( TempAnimationCount > 0  ) 
									{
										GetAnimationTxt( lpChar, CheckAnimationHwnd );
										TempAnimationCount --;
										SetAnimationTxt( lpChar, hDlg );
									}								
			break;
			
		case IDC_PLAY_ANIMATION : 	Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_BASIC ), temp, 9);
			lpChar->basicAction = atoi (temp)-1;
			
			Edit_GetText( GetDlgItem(hDlg, IDC_CHAR_NO ), temp, 9);
			lpChar->sprno = atoi(temp);
			GetAnimationTxt( lpChar, CheckAnimationHwnd );
			lpChar->nCurrentAction = TempAnimationCount;
			lpChar->nCurrentFrame.PutEncrypted(0);
			lpChar->animationType	= ANIMATION_TYPE_TEST;
			AnimationEditWorkStartFlag = TRUE;
			
			EndOfAnimationFlag = 0;
			while( EndOfAnimationFlag != 3 )
			{
				GameTestProc( &g_DirectDrawInfo );
			}
			
			break;
			
			
			
		case IDC_BUTTON1 : lpChar->direction = DIRECTION_DOWN;		; GameTestProc( &g_DirectDrawInfo ); return TRUE;
		case IDC_BUTTON2 : lpChar->direction = DIRECTION_LEFTDOWN;  ; GameTestProc( &g_DirectDrawInfo ); return TRUE;
		case IDC_BUTTON3 : lpChar->direction = DIRECTION_LEFT;		; GameTestProc( &g_DirectDrawInfo ); return TRUE;
		case IDC_BUTTON4 : lpChar->direction = DIRECTION_LEFTUP;	; GameTestProc( &g_DirectDrawInfo ); return TRUE;
		case IDC_BUTTON5 : lpChar->direction = DIRECTION_UP;		; GameTestProc( &g_DirectDrawInfo ); return TRUE;
		case IDC_BUTTON6 : lpChar->direction = DIRECTION_RIGHTUP;	; GameTestProc( &g_DirectDrawInfo ); return TRUE;
		case IDC_BUTTON7 : lpChar->direction = DIRECTION_RIGHT;		; GameTestProc( &g_DirectDrawInfo ); return TRUE;
		case IDC_BUTTON8 : lpChar->direction = DIRECTION_RIGHTDOWN;	; GameTestProc( &g_DirectDrawInfo ); return TRUE;
			
			
		case IDC_FRAME_DOWN :	tool_AnimationEditCurFrame --; 
			if( tool_AnimationEditCurFrame < 0 ) 
			{
				tool_AnimationEditCurFrame = tool_AnimationEditTotalSpriteNo/8 -1;
			}
			sprintf( temp, "%d", tool_AnimationEditCurFrame+1 );
			Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_CURFRAME ), temp);
			;
			GameTestProc( &g_DirectDrawInfo );
			sprintf( temp, " %3d       0       0       0       0       0\n", tool_AnimationEditCurFrame );
			PutClipBoard( temp );
			return TRUE;
			
		case IDC_FRAME_UP   :	tool_AnimationEditCurFrame ++; 
			if( tool_AnimationEditCurFrame >=  tool_AnimationEditTotalSpriteNo/8 ) 
			{
				tool_AnimationEditCurFrame = 0;
			}
			sprintf( temp, "%d", tool_AnimationEditCurFrame +1 );
			Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_CURFRAME ), temp);
			sprintf( temp, " %3d       0       0       0       0       0\n", tool_AnimationEditCurFrame );
			PutClipBoard( temp );
			;
			GameTestProc( &g_DirectDrawInfo );
			return TRUE;
		case IDC_FRAME_DOWN2 :	tool_AnimationEditCurFrame -=10; 
			if( tool_AnimationEditCurFrame < 0 ) 
			{
				tool_AnimationEditCurFrame = tool_AnimationEditTotalSpriteNo/8 -1;
			}
			sprintf( temp, "%d", tool_AnimationEditCurFrame +1);
			Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_CURFRAME ), temp);
			;
			GameTestProc( &g_DirectDrawInfo );
			sprintf( temp, " %3d       0       0       0       0       0\n", tool_AnimationEditCurFrame );
			PutClipBoard( temp );
			return TRUE;
			
		case IDC_FRAME_UP2  :	tool_AnimationEditCurFrame +=10; 
			if( tool_AnimationEditCurFrame >=  tool_AnimationEditTotalSpriteNo/8 ) 
			{
				tool_AnimationEditCurFrame = 0;
			}
			sprintf( temp, "%d", tool_AnimationEditCurFrame +1);
			Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_CURFRAME ), temp);
			;
			GameTestProc( &g_DirectDrawInfo );
			sprintf( temp, " %3d       0       0       0       0       0\n", tool_AnimationEditCurFrame );
			PutClipBoard( temp );
			return TRUE;
		case IDCANCEL :		
			{
				EndDialog(hDlg, LOWORD(wParam));	
				
				lpChar->position.x = lpChar->x / TILE_SIZE;
				lpChar->position.y = lpChar->y / TILE_SIZE;
				int tx = lpChar->position.x - 10;
				if( tx < 0 ) tx = 0;		
				int ty = lpChar->position.y - 7;
				if( ty < 0 ) ty = 0;		
				RECT	rect;				
				rect.left	= tx;			
				rect.top	= ty;			
				rect.right	= tx + 20;		
				rect.bottom = ty + 11;		
				
				;
				SetHeroPosition(0);
				
				return (TRUE);
			}
			}		
			break;		
	}				
	
	return FALSE;	
}					

void CheckAnimationEdit( HWND hwnd, HINSTANCE hInstance)
{	
	HINSTANCE  hinst;
	hinst = LoadLibrary("riched32.dll");
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DIALOG_ANIMATION_EDIT ), NULL, (DLGPROC)CheckAnimationProc );
	FreeLibrary( hinst );
}					


/////////////////////////////////////////////////////////////////////////////////////
///  이벤트 번호 입력..


BOOL CALLBACK InputEventNoProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{											
	char		temp[FILENAME_MAX];
	static int mx, my;
	
	switch(Message)		
	{						
	case WM_INITDIALOG:	mx = Mox / TILE_SIZE; 
		my = Moy / TILE_SIZE; 
		sprintf (temp, "%d", ReturnEventNo( mx, my ) );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT1 ), temp);
		return TRUE;	
		
	case WM_PAINT:		
		break;			
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{		
		case IDOK     : Edit_GetText( GetDlgItem(hDlg, IDC_EDIT1), temp, 9);
			ChangeEventNo( mx, my, atoi (temp) );
			;
			return (TRUE);
			
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));	
			;
			return (TRUE);
		}		
		break;		
	}				
	
	return FALSE;	
}					

void InputEventNo( HWND hwnd, HINSTANCE hInstance)
{	
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_INPUT_EVENT_NO ), NULL, (DLGPROC)InputEventNoProc );
}					



////////////////////////////////////////////////////////////////////////////////


#define MAPOBJECT_PUTOFFXY	 { 	sprintf( temp, "%d", mo->offx ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OFFSETX ), temp); sprintf( temp, "%d", mo->offy );	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OFFSETY ), temp);	}
#define MAPOBJECT_PUTXY	 { sprintf( temp, "%d", mo->x );	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_X ), temp); sprintf( temp, "%d", mo->y );	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_Y ), temp); }


int tool_MapObject_IDC_ALL_AFFECT = IDC_ALL_AFFECT;


void ModifyMapObjectSort( LPMAPOBJECT mo, int cmd, int gap)
{
	int i;
	
	for( i = 0 ; i < 	TotalMapObject  ; i ++)
	{
		if( mo->id == Mo[i].id )
		{
			switch( cmd )
			{
			case IDC_BUTTON_SORTL_U	: Mo[i].dy0[ mo->curframe ]-=gap;break;
			case IDC_BUTTON_SORTL_D	: Mo[i].dy0[ mo->curframe ]+=gap;break;
			case IDC_BUTTON_SORTL_L	: Mo[i].dx0[ mo->curframe ]-=gap;break;
			case IDC_BUTTON_SORTL_R	: Mo[i].dx0[ mo->curframe ]+=gap;break;
			case IDC_BUTTON_SORTR_U	: Mo[i].dy1[ mo->curframe ]-=gap;break;
			case IDC_BUTTON_SORTR_D	: Mo[i].dy1[ mo->curframe ]+=gap;break;
			case IDC_BUTTON_SORTR_L	: Mo[i].dx1[ mo->curframe ]-=gap;break;
			case IDC_BUTTON_SORTR_R	: Mo[i].dx1[ mo->curframe ]+=gap;break;
			}
			Mo[i].attr2 = mo->attr2;
		}
	}
	
}


void ModifyMapObjectDontBox(  LPMAPOBJECT mo, int cmd, int gap)
{
	int i;
	
	for( i = 0 ; i < 	TotalMapObject  ; i ++)
	{
		if( mo->id == Mo[i].id )
		{
			
			//	0109 ItemDontBox
			//			switch( cmd )
			//			{
			//				case IDC_DONTBOX_L1	:	Mo[i].dtx0 -=gap;break;
			//				case IDC_DONTBOX_U1	:	Mo[i].dty0 -=gap;break;	
			//				case IDC_DONTBOX_R1	:	Mo[i].dtx0 -=gap;break;
			//				case IDC_DONTBOX_D1	:	Mo[i].dty0 -=gap;break;
			//				case IDC_DONTBOX_L2	:	Mo[i].dtx1 -=gap;break;
			//				case IDC_DONTBOX_U2	:	Mo[i].dty1 -=gap;break;
			//				case IDC_DONTBOX_R2	:	Mo[i].dtx1 -=gap;break;
			//				case IDC_DONTBOX_D2	:	Mo[i].dty1 -=gap;break;
			//				case IDC_DONTBOX_L3	:	Mo[i].dtx2 -=gap;break;
			//				case IDC_DONTBOX_U3	:	Mo[i].dty2 -=gap;break;
			//				case IDC_DONTBOX_R3	:	Mo[i].dtx2 -=gap;break;
			//				case IDC_DONTBOX_D3	:	Mo[i].dty2 -=gap;break;
			//				case IDC_DONTBOX_L4	:	Mo[i].dtx3 -=gap;break;
			//				case IDC_DONTBOX_U4	:	Mo[i].dty3 -=gap;break;
			//				case IDC_DONTBOX_R4	:	Mo[i].dtx3 -=gap;break;
			//				case IDC_DONTBOX_D4	:	Mo[i].dty3 -=gap;break;
			//			}
		}
	}
}



void ModifyMapObjectSound( LPMAPOBJECT mo )
{
	int i;
	
	if( tool_MapObject_IDC_ALL_AFFECT == IDC_ALL_AFFECT )
	{
		for( i = 0 ; i < 	TotalMapObject  ; i ++)
		{
			if( mo->id == Mo[i].id )
			{
				Mo[i].soundno	 = mo->soundno;
				Mo[i].soundframe = mo->soundframe;
				Mo[i].sounddelay = mo->sounddelay;
			}
		}
	}
}



void ModifyMapObjectOrItem( LPMAPOBJECT mo )
{
	int i;
	
	if( tool_MapObject_IDC_ALL_AFFECT == IDC_ALL_AFFECT )
	{
		for( i = 0 ; i < 	TotalMapObject  ; i ++)
		{
			if( mo->id == Mo[i].id )
			{
				Mo[i].objectoritem = mo->objectoritem;
			}
		}
	}
}

void MapObjectAniTypeChange( LPMAPOBJECT mo, int anitype )
{
	int i;
	
	if( tool_MapObject_IDC_ALL_AFFECT == IDC_ALL_AFFECT )
	{
		for( i = 0 ; i < TotalMapObject  ; i ++)
		{
			if( mo->id == Mo[i].id )
			{
				Mo[i].animationType = mo->animationType;
			}
		}
	}
}
void MapObjectAttr1Change( LPMAPOBJECT mo, int attr1 )
{
	int i;
	
	if( tool_MapObject_IDC_ALL_AFFECT == IDC_ALL_AFFECT )
	{
		for( i = 0 ; i < TotalMapObject  ; i ++)
		{
			if( mo->id == Mo[i].id )
			{
				Mo[i].attr1 = mo->attr1;
			}
		}
	}
}

void MapObjectAttrChange( LPMAPOBJECT mo, int attr )
{
	int i;
	
	if( tool_MapObject_IDC_ALL_AFFECT == IDC_ALL_AFFECT )
	{
		for( i = 0 ; i < TotalMapObject  ; i ++)
		{
			if( mo->id == Mo[i].id )
			{
				Mo[i].attr = mo->attr;
			}
		}
	}
}

void MapObjectLightChange( LPMAPOBJECT mo )
{
	int i;
	if( tool_MapObject_IDC_ALL_AFFECT == IDC_ALL_AFFECT )
	{
		for( i = 0 ; i < TotalMapObject  ; i ++)
		{
			if( mo->id == Mo[i].id )
			{
				Mo[i].light		= mo->light	;
				Mo[i].lightx	= mo->lightx;
				Mo[i].lighty	= mo->lighty;
			}
		}
	}
}





void XYModify( LPMAPOBJECT mo, int x, int y, int offx, int offy )
{
	int i;
	if( tool_MapObject_IDC_ALL_AFFECT == IDC_ALL_AFFECT )
	{
		for( i = 0 ; i < TotalMapObject  ; i ++)
		{
			if( mo->id == Mo[i].id )
			{
				Mo[i].x		+= x;
				Mo[i].y		+= y;
				Mo[i].offx 	+= offx;
				Mo[i].offy 	+= offy;
			}
		}
	}
	else
	{
		mo->x		+= x;
		mo->y		+= y;
		mo->offx 	+= offx;
		mo->offy 	+= offy;
	}
}

///////////////////////// SoundUp lkh 수정 /////////////////////////////
BOOL CALLBACK ViewMapObjectInfoProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LPMAPOBJECT mo;	
	int t;
	RECT rect, grect;
	static  int  curMapObject;
	int al;	
	int iteminbox;
	int	rate_Play=0;
	
	
	char temp[FILENAME_MAX];
	switch(Message)	
	{
	case WM_INITDIALOG:	curMapObject = IDCheckedMapObject;
		mo = &Mo[ IDCheckedMapObject];
		//						IDCheckedMapObject = -1;
		sprintf( temp, "%d", mo->id );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ID ), temp);
		sprintf( temp, "%d", mo->x );	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_X ), temp);
		sprintf( temp, "%d", mo->y );	Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_Y ), temp);
		sprintf( temp, "%d", mo->offx );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OFFSETX ), temp);
		sprintf( temp, "%d", mo->offy );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OFFSETY ), temp);
		
		sprintf( temp, "%d", mo->animationType );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ANITYPE_), temp);
		
		sprintf( temp, "%d / %d", mo->curframe+1, MoImage[ mo->id-1].nFrameCount );
		Edit_SetText (GetDlgItem(hDlg, IDC_CURFRAME ), temp);
		
		al = mo->attr1 >> 3 ;
		sprintf( temp, "%d", al );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ALPHA ), temp);
		
		sprintf( temp, "%d", mo->dum );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_ITEM_IN_BOX ), temp);
		
		//						ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_COMBO_ALPHA ), al);
		
		//						sprintf( temp, "%d", mo->animationOrder );
		//						Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OUT_ ), temp);
		//						sprintf( temp, "%d", mo->spriteType );
		//						Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SPRITE_ ), temp);
		//						sprintf( temp, "%d", mo->light );
		//						Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_LIGHT_ ), temp);
		
		switch( mo->animationType )
		{
		case 0						 :  mo->animationType = OBJECTANI_NO;
		case OBJECTANI_NO			 :	t = IDC__ANI6; break;
		case OBJECTANI_RETURNLOOP	 : 	t = IDC__ANI7; break;
		case OBJECTANI_LOOP			 :	t = IDC__ANI8; break;
		case OBJECTANI_1_RETURNLOOP	 :	t = IDC__ANI9; break;
		case OBJECTANI_1_LOOP		 :	t = IDC__ANI10; break;
		case OBJECTANI_TOGGLE_ANI	 : 	t = IDC__ANI11; break;
		}
		CheckRadioButton( hDlg, IDC__ANI6, IDC__ANI11, t );
		
		switch( mo->attr )
		{
		case OBJECTATTR_BACK_ABOVE	:	t = IDC_RADIO_OUT0;		break;
		case OBJECTATTR_BACK_BELOW	:	t = IDC_RADIO_OUT1;		break;
		case OBJECTATTR_FRONT_ABOVE	:	t = IDC_RADIO_OUT2;		break;
		case OBJECTATTR_FRONT_BELOW	:	t = IDC_RADIO_OUT3;		break;
		}
		CheckRadioButton( hDlg, IDC_RADIO_OUT0, IDC_RADIO_OUT3, t );
		
		/**/	
		switch( mo->attr1 & 0x0003 )
		{
		case OBJECTSPRITE_NORMAL :  t = IDC_RADIO_SPRITE0; break;
		case OBJECTSPRITE_TRANS	 :  t = IDC_RADIO_SPRITE1; break;
		case OBJECTSPRITE_ALPHA  :  t = IDC_RADIO_SPRITE2; break;
		}
		CheckRadioButton( hDlg, IDC_RADIO_SPRITE0, IDC_RADIO_SPRITE2, t );
		
		// mo->light...
		switch( mo->light )
		{
		case 0 :  t = IDC_RADIO_LIGHT_0; break;
		case 1 :  t = IDC_RADIO_LIGHT_1; break;
		case 2 :  t = IDC_RADIO_LIGHT_2; break;
		case 3 :  t = IDC_RADIO_LIGHT_3; break;
		case 4 :  t = IDC_RADIO_LIGHT_4; break;
		case 5 :  t = IDC_RADIO_LIGHT_5; break;
		case 6 :  t = IDC_RADIO_LIGHT_6; break;
		case 7 :  t = IDC_RADIO_LIGHT_7; break;
		case 8 :  t = IDC_RADIO_LIGHT_8; break;
		case 9 :  t = IDC_RADIO_LIGHT_9; break;
		case 10:  t = IDC_RADIO_LIGHT_10; break;
		case 11:  t = IDC_RADIO_LIGHT_11; break;
		case 12:  t = IDC_RADIO_LIGHT_12; break;
		case 13:  t = IDC_RADIO_LIGHT_13; break;
		case 14:  t = IDC_RADIO_LIGHT_14; break;
		case 15:  t = IDC_RADIO_LIGHT_15; break;
		case 16:  t = IDC_RADIO_LIGHT_16; break;
		case 17:  t = IDC_RADIO_LIGHT_17; break;
		}
		CheckRadioButton( hDlg, IDC_RADIO_LIGHT_0, IDC_RADIO_LIGHT_17, t );
		
		switch( mo->attr2 )
		{
		case 0 :  t = IDC_SORT_M; break;
		case 1 :  t = IDC_SORT_L; break;
		case 2 :  t = IDC_SORT_R; break;
		}
		
		CheckRadioButton( hDlg, IDC_SORT_L, IDC_SORT_R, t );
		if( t == IDC_SORT_L || t == IDC_SORT_R )
		{
			sprintf( temp, "%d", mo->dx0[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_X), temp);
			sprintf( temp, "%d", mo->dy0[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_Y), temp);
			sprintf( temp, "%d", mo->dx1[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_X), temp);
			sprintf( temp, "%d", mo->dy1[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_Y), temp);
		}
		
		switch( mo->objectoritem )
		{
		case ITEMTYPE_NORMAL		:	t = IDC_ITEM_NORMAL4	;					break;
		case ITEMTYPE_CHAIR			:	t = IDC_ITEM_CHAIR4		;					break;
		case ITEMTYPE_TABLE			:	t = IDC_ITEM_TABLE4		;					break;
		case ITEMTYPE_DOOR			:	t = IDC_ITEM_DOOR4		;					break;
		case ITEMTYPE_BOX			:	t = IDC_ITEM_BOX4		;					break;
		case ITEMTYPE_SIGNBOARD		:	t = IDC_ITEM_SIGNBOARD4	;					break;
		case ITEMTYPE_DIRECTBOARD	:	t = IDC_ITEM_DIRECTBOARD4;					break;
			
		case ITEMTYPE_WORKTABLE_MINING		:	t = IDC_ITEM_WORKTABLE_MINING4;		break;		// 작업대 용광로(71)
		case ITEMTYPE_WORKTABLE_BATTLE      :	t = IDC_ITEM_WORKTABLE_BATTLE4;		break;      // 작업대 배틀
		case ITEMTYPE_WORKTABLE_SUNBAN		:   t = IDC_ITEM_WORKTABLE_SUNBAN4;		break;		// 작업대 선반
		case ITEMTYPE_WORKTABLE_MORU		:	t = IDC_ITEM_WORKTABLE_MORU4;		break;		// 작업대 모루
		case ITEMTYPE_WORKTABLE_HWADUK		: 	t = IDC_ITEM_WORKTABLE_HWADUK4;		break;		// 작업대 화덕
		case ITEMTYPE_WORKTABLE_SIHUMKWAN	:   t = IDC_ITEM_WORKTABLE_SIHUMKWAN4;	break;		// 작업대 시험관류
		case ITEMTYPE_WORKTABLE_MULAE		: 	t = IDC_ITEM_WORKTABLE_MULAE4;		break;		// 작업대 물레
		case ITEMTYPE_WORKTABLE_BULDAE		: 	t = IDC_ITEM_WORKTABLE_BULDAE4;		break;		// 작업대 불대 
			
		case ITEMTYPE_SEAL_NOR				: 	t = IDC_ITEM_SEAL_NOR				;	break;		// 작업대 불대 
		case ITEMTYPE_SEAL_SPC				: 	t = IDC_ITEM_SEAL_SPC				;	break;		// 작업대 불대 
		case ITEMTYPE_HP_UP					: 	t = IDC_ITEM_HP_UP					;	break;		// 작업대 불대 
		case ITEMTYPE_MP_UP					: 	t = IDC_ITEM_MP_UP					;	break;		// 작업대 불대 
		case ITEMTYPE_SCALE					: 	t = IDC_ITEM_SP_UP					;	break;		// 작업대 불대	// 010614 khs
		case ITEMTYPE_NATION_STONE			: 	t = IDC_ITEM_NATION_STONE			;	break;		// 작업대 불대 
		case ITEMTYPE_GUILD_STONE			: 	t = IDC_ITEM_GUILD_STONE			;	break;		// 작업대 불대 
		case ITEMTYPE_SENARIO_STONE			: 	t = IDC_ITEM_SENARIO_STONE			;	break;		// 작업대 불대 
		case ITEMTYPE_COLOSSUS_STONE		: 	t = IDC_ITEM_COLOSSUS_STONE			;	break;		// 작업대 불대 
			
			
		}
		CheckRadioButton( hDlg, IDC_ITEM_NORMAL4, IDC_ITEM_COLOSSUS_STONE, t );
		
		CheckRadioButton( hDlg, IDC_ALL_AFFECT, IDC_ALL_AFFECT_NO, tool_MapObject_IDC_ALL_AFFECT );
		
		sprintf( temp, "%d", MoImage[ mo->id-1].nFrameCount );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_FRAME ), temp);
		
		// Sound.
		sprintf( temp, "%d", mo->soundno );			//10000단위 이상은 딜레이를 위한 카운트 변수용
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_NO ), temp);
		sprintf( temp, "%d", mo->soundframe );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_FRAME ), temp);
		sprintf( temp, "%d", mo->sounddelay );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_DEALY ), temp);
		
		
		GetWindowRect( hDlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
		GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
		MoveWindow( hDlg, grect.right+1, 0, rect.right, rect.bottom, true);
		
		//						CheckRadioButton( hDlg, IDC_ALL_AFFECT, IDC_ALL_AFFECT_NO, tool_MapObject_IDC_ALL_AFFECT );
		
		return TRUE;	
		
		case WM_PAINT:		
			break;			
			
			
		case WM_COMMAND:	
			mo = &Mo[ curMapObject];
			
			
			switch( LOWORD( wParam )) 
			{		
			case ID_SOUND_PLAY  :   
				{
					int tempno;
					Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_NO ), temp, 9);
					tempno	= atoi( temp );
					tempno	= tempno%1000;
					PlayListAutoSounds( tempno, 0,0,0 ); break;
				}
				
				
			case IDC_ALL_AFFECT :		tool_MapObject_IDC_ALL_AFFECT = IDC_ALL_AFFECT;		break;
			case IDC_ALL_AFFECT_NO :	tool_MapObject_IDC_ALL_AFFECT = IDC_ALL_AFFECT_NO;	break;
				
			case IDC_BUTTON_OFF_LEFT	: XYModify( mo,  0,  0, -1,  0 ); MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); break;
			case IDC_BUTTON_OFF_RIGHT	: XYModify( mo,  0,  0,  1,  0 ); MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); break;
			case IDC_BUTTON_OFF_DOWN	: XYModify( mo,  0,  0,  0,  1 ); MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); break;
			case IDC_BUTTON_OFF_UP		: XYModify( mo,  0,  0,  0, -1 ); MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); break;
			case IDC_Y_U : { XYModify( mo,  0, -1,  0,  0 ); ;GameTestProc( &g_DirectDrawInfo ); MAPOBJECT_PUTXY } break;
			case IDC_Y_D : { XYModify( mo,  0,  1,  0,  0 ); ;GameTestProc( &g_DirectDrawInfo ); MAPOBJECT_PUTXY } break;
			case IDC_X_L : { XYModify( mo, -1,  0,  0,  0 ); ;GameTestProc( &g_DirectDrawInfo ); MAPOBJECT_PUTXY } break;
			case IDC_X_R : { XYModify( mo,  1,  0,  0,  0 ); ;GameTestProc( &g_DirectDrawInfo ); MAPOBJECT_PUTXY } break;
			case IDC_Y_U2: { XYModify( mo,  0,-10,  0,  0 ); ;GameTestProc( &g_DirectDrawInfo ); MAPOBJECT_PUTXY } break;
			case IDC_Y_D2: { XYModify( mo,  0, 10,  0,  0 ); ;GameTestProc( &g_DirectDrawInfo ); MAPOBJECT_PUTXY } break;
			case IDC_X_L2: { XYModify( mo,-10,  0,  0,  0 ); ;GameTestProc( &g_DirectDrawInfo ); MAPOBJECT_PUTXY } break;
			case IDC_X_R2: { XYModify( mo, 10,  0,  0,  0 ); ;GameTestProc( &g_DirectDrawInfo ); MAPOBJECT_PUTXY } break;
				
			case IDC_ORI_L :  { XYModify( mo, -1,  0,  1,  0 ); MAPOBJECT_PUTXY; MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); } break;
			case IDC_ORI_R :  { XYModify( mo,  1,  0, -1,  0 ); MAPOBJECT_PUTXY; MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); } break;
			case IDC_ORI_U :  { XYModify( mo,  0, -1,  0,  1 ); MAPOBJECT_PUTXY; MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); } break;
			case IDC_ORI_D :  { XYModify( mo,  0,  1,  0, -1 ); MAPOBJECT_PUTXY; MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); } break;
			case IDC_ORI_LL : { XYModify( mo,-10,  0, 10,  0 ); MAPOBJECT_PUTXY; MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); } break;
			case IDC_ORI_RR : { XYModify( mo, 10,  0,-10,  0 ); MAPOBJECT_PUTXY; MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); } break;
			case IDC_ORI_UU : { XYModify( mo,  0,-10,  0, 10 ); MAPOBJECT_PUTXY; MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); } break;
			case IDC_ORI_DD : { XYModify( mo,  0, 10,  0,-10 ); MAPOBJECT_PUTXY; MAPOBJECT_PUTOFFXY; ;GameTestProc( &g_DirectDrawInfo ); } break;
				
				//////////////////// 0203 이규훈 수정 //////////////////
			case IDC_BUTTON_SAVE		: 
#ifdef _DEBUG // finito 060507
				if(SysInfo.notconectserver)
					SaveTOI2( MapName );
				else
#endif
					MessageBox(NULL,"서버와 연결된 상태에서는 SAVE되지 않습니다.",0,MB_OK);
				break;
				
			case IDC_BUTTON_SORTL_U	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTL_U, 3 );  sprintf( temp, "%d", mo->dy0[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_Y), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTL_D	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTL_D, 3 );  sprintf( temp, "%d", mo->dy0[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_Y), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTL_L	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTL_L, 3 );  sprintf( temp, "%d", mo->dx0[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_X), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTL_R	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTL_R, 3 );  sprintf( temp, "%d", mo->dx0[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_X), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTR_U	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTR_U, 3 );  sprintf( temp, "%d", mo->dy1[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_Y), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTR_D	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTR_D, 3 );  sprintf( temp, "%d", mo->dy1[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_Y), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTR_L	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTR_L, 3 );  sprintf( temp, "%d", mo->dx1[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_X), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTR_R	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTR_R, 3 );  sprintf( temp, "%d", mo->dx1[ mo->curframe ] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_X), temp);	;GameTestProc( &g_DirectDrawInfo );break;
				
			case IDC_BUTTON_SORTL_U2	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTL_U, 20 );  sprintf( temp, "%d", mo->dy0[mo->curframe] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_Y), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTL_D2	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTL_D, 20 );  sprintf( temp, "%d", mo->dy0[mo->curframe] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_Y), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTL_L2	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTL_L, 20 );  sprintf( temp, "%d", mo->dx0[mo->curframe] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_X), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTL_R2	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTL_R, 20 );  sprintf( temp, "%d", mo->dx0[mo->curframe] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTL_X), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTR_U2	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTR_U, 20 );  sprintf( temp, "%d", mo->dy1[mo->curframe] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_Y), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTR_D2    : ModifyMapObjectSort( mo, IDC_BUTTON_SORTR_D, 20 );  sprintf( temp, "%d", mo->dy1[mo->curframe] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_Y), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTR_L2	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTR_L, 20 );  sprintf( temp, "%d", mo->dx1[mo->curframe] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_X), temp);	;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_SORTR_R2	: ModifyMapObjectSort( mo, IDC_BUTTON_SORTR_R, 20 );  sprintf( temp, "%d", mo->dx1[mo->curframe] ); Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SORTR_X), temp);	;GameTestProc( &g_DirectDrawInfo );break;
				
			case IDC_DONTBOX_L1	:	
			case IDC_DONTBOX_U1	:	
			case IDC_DONTBOX_R1	:	
			case IDC_DONTBOX_D1	:	
			case IDC_DONTBOX_L2	:	
			case IDC_DONTBOX_U2	:	
			case IDC_DONTBOX_R2	:	
			case IDC_DONTBOX_D2	:	
			case IDC_DONTBOX_L3	:	
			case IDC_DONTBOX_U3	:	
			case IDC_DONTBOX_R3	:	
			case IDC_DONTBOX_D3	:	
			case IDC_DONTBOX_L4	:	
			case IDC_DONTBOX_U4	:	
			case IDC_DONTBOX_R4	:	
			case IDC_DONTBOX_D4	:	// 0109 ItemDontBox
				//										ModifyMapObjectDontBox( mo, LOWORD( wParam ), 20 );
				//										; 
				//										GameTestProc( &g_DirectDrawInfo );
				break;
				
			case IDC_BUTTON_LIGHT_U : mo->lighty -= 3; MapObjectLightChange( mo ); ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_LIGHT_D : mo->lighty += 3; MapObjectLightChange( mo ); ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_LIGHT_L : mo->lightx -= 3; MapObjectLightChange( mo ); ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_BUTTON_LIGHT_R : mo->lightx += 3; MapObjectLightChange( mo ); ;GameTestProc( &g_DirectDrawInfo );break;
				
			case IDC_CURFRAME_DOWN  :	mo->curframe --; if( mo->curframe < 0 ) mo->curframe = 0;
				sprintf( temp, "%d / %d", mo->curframe+1, MoImage[ mo->id-1].nFrameCount );
				Edit_SetText (GetDlgItem(hDlg, IDC_CURFRAME ), temp);
				;
				GameTestProc( &g_DirectDrawInfo );
				break;
			case IDC_CURFRAME_UP	:	mo->curframe ++; if(  mo->curframe >= MoImage[ mo->id-1].nFrameCount  ) mo->curframe = MoImage[ mo->id-1].nFrameCount -1;
				sprintf( temp, "%d / %d ", mo->curframe+1, MoImage[ mo->id-1].nFrameCount );
				Edit_SetText (GetDlgItem(hDlg, IDC_CURFRAME ), temp);
				;
				GameTestProc( &g_DirectDrawInfo );
				
			case IDC_RADIO_LIGHT_0 : mo->light = 0;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_1 : mo->light = 1;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_2 : mo->light = 2;	MapObjectLightChange( mo );	 ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_3 : mo->light = 3;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_4 : mo->light = 4;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_5 : mo->light = 5;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_6 : mo->light = 6;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_7 : mo->light = 7;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_8 : mo->light = 8;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_9 : mo->light = 9;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_10: mo->light =10;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_11: mo->light =11;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_12: mo->light =12;	MapObjectLightChange( mo );	 ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_13: mo->light =13;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_14: mo->light =14;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_15: mo->light =15;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_16: mo->light =16;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
			case IDC_RADIO_LIGHT_17: mo->light =17;	MapObjectLightChange( mo );  ;GameTestProc( &g_DirectDrawInfo );break;
				
			case IDC_OBS_OVERLAP:
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_FRAME ), temp, 9);
				mo->soundframe = atoi(temp);
				
				if(mo->soundframe/1000)
					mo->soundframe	-= 1000;
				else
					mo->soundframe	+= 1000;
				
				sprintf( temp, "%d", mo->soundframe );
				Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_FRAME ), temp);
				break;
				/*
				case IDC_OBS_3TYPE:
				if(mo->sounddelay/1000 == 1)
				mo->sounddelay	-= 1000;
				else
				mo->sounddelay	+= 1000;
				break;
				*/
			case IDC_OBS_ETC:
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_DEALY ), temp, 9);
				mo->sounddelay = atoi(temp);
				
				if(mo->sounddelay/1000)
					mo->sounddelay	-= 1000;
				else
					mo->sounddelay	+= 1000;
				
				sprintf( temp, "%d", mo->sounddelay );
				Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_DEALY ), temp);
				break;
				
			case IDOK     :
				Edit_GetText ( GetDlgItem(hDlg, IDC_EDIT_ID ), temp, 9);
				//						mo->id = atoi( temp ) - moid;
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_X ), temp, 9);
				mo->x = atoi( temp );
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_Y ), temp, 9);
				mo->y = atoi( temp );
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OFFSETX ), temp, 9);
				mo->offx = atoi( temp );
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OFFSETY ), temp, 9);
				mo->offy = atoi( temp );
				//Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_ANITYPE_), temp, 9);
				//mo->animationType = atoi( temp );
				//Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OUT_ ), temp, 9);
				//mo->animationOrder = atoi( temp );
				
				
				if( IsDlgButtonChecked( hDlg, IDC__ANI6 ) == BST_CHECKED ) { mo->animationType = OBJECTANI_NO				; MapObjectAniTypeChange(mo, OBJECTANI_NO			);}
				if( IsDlgButtonChecked( hDlg, IDC__ANI7 ) == BST_CHECKED ) { mo->animationType = OBJECTANI_RETURNLOOP		; MapObjectAniTypeChange(mo, OBJECTANI_RETURNLOOP	);}
				if( IsDlgButtonChecked( hDlg, IDC__ANI8 ) == BST_CHECKED ) { mo->animationType = OBJECTANI_LOOP				; MapObjectAniTypeChange(mo, OBJECTANI_LOOP			);}
				if( IsDlgButtonChecked( hDlg, IDC__ANI9 ) == BST_CHECKED ) { mo->animationType = OBJECTANI_1_RETURNLOOP		; MapObjectAniTypeChange(mo, OBJECTANI_1_RETURNLOOP	);}
				if( IsDlgButtonChecked( hDlg, IDC__ANI10) == BST_CHECKED ) { mo->animationType = OBJECTANI_1_LOOP			; MapObjectAniTypeChange(mo, OBJECTANI_1_LOOP		);}
				if( IsDlgButtonChecked( hDlg, IDC__ANI11) == BST_CHECKED ) { mo->animationType = OBJECTANI_TOGGLE_ANI		; MapObjectAniTypeChange(mo, OBJECTANI_TOGGLE_ANI	);}
				
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_OUT0 ) == BST_CHECKED ){ mo->attr = OBJECTATTR_BACK_ABOVE ; MapObjectAttrChange( mo, OBJECTATTR_BACK_ABOVE  ); }
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_OUT1 ) == BST_CHECKED ){ mo->attr = OBJECTATTR_BACK_BELOW ; MapObjectAttrChange( mo, OBJECTATTR_BACK_BELOW  ); }
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_OUT2 ) == BST_CHECKED ){ mo->attr = OBJECTATTR_FRONT_ABOVE; MapObjectAttrChange( mo, OBJECTATTR_FRONT_ABOVE ); }
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_OUT3 ) == BST_CHECKED ){ mo->attr = OBJECTATTR_FRONT_BELOW; MapObjectAttrChange( mo, OBJECTATTR_FRONT_BELOW ); }
				
				//						al = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_COMBO_ALPHA));
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_ALPHA ), temp, 9);
				al = atoi( temp );
				al <<= 3;
				mo->attr1 = ((mo->attr1 & 0x7) | al);
				
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_SPRITE0 ) == BST_CHECKED ) {mo->attr1 = ((mo->attr1 & 0xf8) + OBJECTSPRITE_NORMAL); MapObjectAttr1Change(mo, OBJECTSPRITE_NORMAL);}
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_SPRITE1 ) == BST_CHECKED ) {mo->attr1 = ((mo->attr1 & 0xf8) + OBJECTSPRITE_TRANS ); MapObjectAttr1Change(mo, OBJECTSPRITE_TRANS );}
				if( IsDlgButtonChecked( hDlg, IDC_RADIO_SPRITE2 ) == BST_CHECKED ) {mo->attr1 = ((mo->attr1 & 0xf8) + OBJECTSPRITE_ALPHA ); MapObjectAttr1Change(mo, OBJECTSPRITE_ALPHA );}
				
				
				if( IsDlgButtonChecked( hDlg, IDC_SORT_M ) == BST_CHECKED ) mo->attr2 = 0;
				if( IsDlgButtonChecked( hDlg, IDC_SORT_L ) == BST_CHECKED ) mo->attr2 = 1;
				if( IsDlgButtonChecked( hDlg, IDC_SORT_R ) == BST_CHECKED ) mo->attr2 = 2;
				
				
				if( IsDlgButtonChecked( hDlg, IDC_ITEM_NORMAL4	) == BST_CHECKED ) {	mo->objectoritem = ITEMTYPE_NORMAL;	ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_CHAIR4	) == BST_CHECKED ) {	mo->objectoritem = ITEMTYPE_CHAIR ;	ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_TABLE4	) == BST_CHECKED ) {	mo->objectoritem = ITEMTYPE_TABLE ;	ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_DOOR4	) == BST_CHECKED ) {	mo->objectoritem = ITEMTYPE_DOOR ;	ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_BOX4 	) == BST_CHECKED ) {	mo->objectoritem = ITEMTYPE_BOX ;	ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_SIGNBOARD4) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_SIGNBOARD ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_DIRECTBOARD4) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_DIRECTBOARD ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_WORKTABLE_MINING4		) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_WORKTABLE_MINING		 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_WORKTABLE_BATTLE4		) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_WORKTABLE_BATTLE      ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_WORKTABLE_SUNBAN4		) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_WORKTABLE_SUNBAN		 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_WORKTABLE_MORU4			) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_WORKTABLE_MORU		 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_WORKTABLE_HWADUK4		) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_WORKTABLE_HWADUK		 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_WORKTABLE_SIHUMKWAN4	) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_WORKTABLE_SIHUMKWAN	 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_WORKTABLE_MULAE4		) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_WORKTABLE_MULAE		 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_WORKTABLE_BULDAE4		) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_WORKTABLE_BULDAE		 ;		ModifyMapObjectOrItem( mo );	}
				
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_SEAL_NOR				) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_SEAL_NOR				 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_SEAL_SPC				) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_SEAL_SPC				 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_HP_UP					) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_HP_UP				 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_MP_UP					) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_MP_UP				 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_SP_UP					) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_SCALE				;		ModifyMapObjectOrItem( mo );	}	// 010614 KHS
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_NATION_STONE			) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_NATION_STONE			 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_GUILD_STONE				) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_GUILD_STONE			 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_SENARIO_STONE			) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_SENARIO_STONE	 ;		ModifyMapObjectOrItem( mo );	}
				else if( IsDlgButtonChecked( hDlg, IDC_ITEM_COLOSSUS_STONE			) == BST_CHECKED ) {   mo->objectoritem = ITEMTYPE_COLOSSUS_STONE	 ;		ModifyMapObjectOrItem( mo );	}
				
				
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_ITEM_IN_BOX ), temp, 9);
				iteminbox = atoi( temp );
				mo->dum= iteminbox;
				
				// Sound...
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_NO ), temp, 9);
				mo->soundno	= atoi(temp);
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_FRAME ), temp, 9);
				mo->soundframe = atoi(temp);
				Edit_GetText (GetDlgItem(hDlg, IDC_EDIT_OBJECT_SOUND_DEALY ), temp, 9);
				mo->sounddelay = atoi(temp);
				if((mo->sounddelay%1000)<66)
					mo->soundno = (mo->soundno%1000) + (mo->sounddelay%1000)*1000;
				
				ModifyMapObjectSound( mo );
				
				;
				GameTestProc( &g_DirectDrawInfo );
				
				EndDialog(hDlg, LOWORD(wParam));	
				SetAcquire( g_bIsActive );
				
				return (FALSE);
				
			case IDCANCEL :	;
				EndDialog(hDlg, LOWORD(wParam));	
				SetAcquire( g_bIsActive );
				
				return (TRUE);
			}		
			break;		
	}				
	
	return FALSE;	
}					


void ViewMapObjectInfo( void )
{
	DialogBox( g_hInstance, MAKEINTRESOURCE( IDD_VIEW_MAPOBJECT_INFO ), g_hwndMain, (DLGPROC)ViewMapObjectInfoProc);
}





BOOL CALLBACK ViewItemInfoProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{											
	char temp[FILENAME_MAX];
	switch(Message)		
	{						
	case WM_INITDIALOG:	sprintf( temp, "%d", CursorCheckedItem->server_id );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_SERVER_ID ), temp);
		sprintf( temp, "%d", CursorCheckedItem->x );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_X ), temp);
		sprintf( temp, "%d", CursorCheckedItem->y );
		
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_Y ), temp);
		sprintf( temp, "%d", Mox - CursorCheckedItem->x  );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_MOX ), temp);
		sprintf( temp, "%d", Moy - CursorCheckedItem->y  );
		Edit_SetText (GetDlgItem(hDlg, IDC_EDIT_MOY), temp);
		
		return TRUE;	
		
	case WM_PAINT:		
		break;			
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{		
		case IDOK     : EndDialog(hDlg, LOWORD(wParam));	
			;
			return (TRUE);
			
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));	
			;
			return (TRUE);
		}		
		break;		
	}				
	
	return FALSE;	
}					


void ViewItemInfo( void )
{
	DialogBox( g_hInstance, MAKEINTRESOURCE( IDD_VIEW_ITEMINFO ), g_hwndMain, (DLGPROC)ViewItemInfoProc);
}




BOOL CALLBACK ChangeBodyRGBProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{											
	int r,g,b;
	
	char temp[FILENAME_MAX];
	switch(Message)		
	{						
	case WM_INITDIALOG:	
		return TRUE;	
		
	case WM_PAINT:		
		break;			
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{		
		case IDOK     : 	Edit_GetText( GetDlgItem(hDlg, IDC_EDIT1), temp, 9);
			r = atoi (temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT4), temp, 9);
			g = atoi (temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT5), temp, 9);
			b = atoi (temp );
			Hero->bodycolor = ReturnBlendRGB( r,g,b );
			;
			EndDialog(hDlg, LOWORD(wParam));	
			return (TRUE);
			
		case IDOK2     : 	Edit_GetText( GetDlgItem(hDlg, IDC_EDIT1), temp, 9);
			r = atoi (temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT4), temp, 9);
			g = atoi (temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT5), temp, 9);
			b = atoi (temp );
			Hero->clothcolor = ReturnBlendRGB( r,g,b );
			;
			EndDialog(hDlg, LOWORD(wParam));	
			return (TRUE);
		case IDOK3     : 	Edit_GetText( GetDlgItem(hDlg, IDC_EDIT1), temp, 9);
			r = atoi (temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT4), temp, 9);
			g = atoi (temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT5), temp, 9);
			b = atoi (temp );
			EquipItemAttr[WT_NECK].attr[IATTR_RESERVED1] = 65536 * r + 256 * g + b
			;
			EndDialog(hDlg, LOWORD(wParam));	
			return (TRUE);
		
		case IDCANCEL :		EndDialog(hDlg, LOWORD(wParam));	
			;
			return (TRUE);
		}		
		break;		
	}				
	
	return FALSE;	
}					

void ChangeBodyRGB( void )
{	
	DialogBox( g_hInstance, MAKEINTRESOURCE( IDD_CHANGE_BODYRGB ), g_hwndMain, (DLGPROC)ChangeBodyRGBProc );
}	
extern void SendLogOutCharacter(char* szName);
BOOL CALLBACK BanAccountProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char characterName[20];
	char reason[255];
	char duration[500];

	switch (Message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_PAINT:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BAN: 	
			Edit_GetText(GetDlgItem(hDlg, IDC_EDIT1), characterName, 20);
			Edit_GetText(GetDlgItem(hDlg, IDC_EDIT4), reason, 255);
			Edit_GetText(GetDlgItem(hDlg, IDC_EDIT5), duration, 500);
			
			if (!(*characterName))
			{
				return false;
			}

			if (!(*reason))
			{
				return false;
			}
			if (!(*duration))
			{
				return false;
			}
			//if (duration <= " ") duration ='';
			
			// Send packet to mapserver
			t_packet p;
			p.h.header.type = CMD_BAN_ACCOUNT;
			p.h.header.size = sizeof(t_ban_account);

			memcpy(&p.u.BanAccount.characterName, characterName, 20);
			memcpy(&p.u.BanAccount.duration, duration, 255);
			memcpy(&p.u.BanAccount.reason, reason, 255);

			QueuePacket(&p, 1);
			//SendLogOutCharacter(characterName);
			EndDialog(hDlg, LOWORD(wParam));

			return (TRUE);

		case ID_UNBAN:
			Edit_GetText(GetDlgItem(hDlg, IDC_EDIT1), characterName, 20);

			if (!(*characterName))
			{
				return false;
			}
			p.h.header.type = CMD_UNBAN_ACCOUNT;
			p.h.header.size = sizeof(t_ban_account);

			memcpy(&p.u.BanAccount.characterName, characterName, 20);
			QueuePacket(&p, 1);
			EndDialog(hDlg, LOWORD(wParam));

			return (TRUE);

		case ID_ACCOUNT_LOCK:
			Edit_GetText(GetDlgItem(hDlg, IDC_EDIT1), characterName, 20);
			Edit_GetText(GetDlgItem(hDlg, IDC_EDIT4), reason, 255);
			if (!(*characterName))
			{
				return false;
			}
			if (!(*reason))
			{
				return false;
			}
			p.h.header.type = CMD_ACCOUNT_LOCK;
			p.h.header.size = sizeof(t_ban_account);
			::EatRearWhiteChar(characterName);
			memcpy(&p.u.BanAccount.characterName, characterName, 20);
			memcpy(&p.u.BanAccount.reason, reason, 255);
			QueuePacket(&p, 1);
			EndDialog(hDlg, LOWORD(wParam));

			return (TRUE);
		case IDCANCEL:		EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
		case ID_CLOSE_BAN_TOOL: EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
		}
		break;
	}

	return FALSE;
}
//t_ItemGrade ItemGrade[MAX_ITEM_GRADE];
//t_ItemRare ItemRare[MAX_ITEM_RARE];
void BanCharacter()

{
#ifdef _DEBUG

	HWND hDlgWnd = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_BAN_TOOL), g_hwndMain, (DLGPROC)BanAccountProc);
	ShowWindow(hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect(hDlgWnd, &rect);
	GetWindowRect(g_hwndMain, &grect);
	MoveWindow(hDlgWnd, grect.right + 1, 0, rect.right, rect.bottom, true);
	ShowWindow(hDlgWnd, SW_SHOWNORMAL);

#endif
	//DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_BAN_TOOL), g_hwndMain, (DLGPROC)BanAccountProc);
}
void ForceStartWar()

{
#ifdef _DEBUG

	HWND hDlgWnd = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_FORCE_START_NATION_WAR), g_hwndMain, (DLGPROC)ForceNationWarProc);
	ShowWindow(hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect(hDlgWnd, &rect);
	GetWindowRect(g_hwndMain, &grect);
	MoveWindow(hDlgWnd, grect.right + 1, 0, rect.right, rect.bottom, true);
	ShowWindow(hDlgWnd, SW_SHOWNORMAL);

#endif
	//DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_BAN_TOOL), g_hwndMain, (DLGPROC)BanAccountProc);
}
BOOL CALLBACK ForceNationWarProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	bool Start=false;


	switch (Message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_PAINT:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			t_packet p;

		

			p.u.ForceDungeon.port = 5700;
			
			// Send packet to mapserver
			p.h.header.type = CMD_LOCALWAR_BEGIN_FORCE;
			p.h.header.size = sizeof(t_ForceNationWar);

			
			QueuePacket(&p, 1);
	
			EndDialog(hDlg, LOWORD(wParam));

			return (TRUE);



			
		case IDCANCEL:		EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
		}
		break;
	}

	return FALSE;
}
CItem_Plant Han_Name[ITEM_NAME_MAX];
t_ItemLegendSub legend[MAX_ITEM_LEGEND_SUB_BONUS_MAX];
ITEM_LEGEND_BONUS legendbonus[MAX_ITEM_LEGEND_BONUS_KIND];
BOOL CALLBACK GenerateItemNewProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	//Item_Ref.nItem[i]
	char temp[600] = { 0, };

	int grade;
	switch (Message)
	{
	case WM_INITDIALOG:
	{
		HWND hRare1Wnd = GetDlgItem(hDlg, CBO_RARE1);
		HWND hRare2Wnd = GetDlgItem(hDlg, CBO_RARE2);
		HWND hRare3Wnd = GetDlgItem(hDlg, CBO_RARE3);
		HWND hRare4Wnd = GetDlgItem(hDlg, CBO_RARE4);
		HWND hRare5Wnd = GetDlgItem(hDlg, CBO_RARE5);
		HWND hDynamicWnd = GetDlgItem(hDlg, CBO_DYNAMIC);
		HWND hItemName = GetDlgItem(hDlg, CBO_ITEMGEN_NAME);
		HWND hItemName2 = GetDlgItem(hDlg, CBO_ITEMGEN_NAME2);
		// honestly there's thousands of items lol 
		// might be best to write the i guess ahah and thats opnly weaspons
		// 

		for (int i = 0; i != Num_Of_CItem_Weapon; ++i)
		{
			// cringe C not C++
			if (strcmp(Item_Ref.Item_type[WEAPON][i]->GetItemHanName(), "null") == 0)
				continue;

			SendMessage(hItemName, CB_ADDSTRING, 0, (LPARAM)Item_Ref.Item_type[WEAPON][i]->GetItemHanName());
		}
		for (int i = 0; i != Num_Of_CItem_Accessory; ++i)
		{
			// cringe C not C++
			if (strcmp(Item_Ref.Item_type[ACCESSORY][i]->GetItemHanName(), "null") == 0)
				continue;

			SendMessage(hItemName2, CB_ADDSTRING, 0, (LPARAM)Item_Ref.Item_type[ACCESSORY][i]->GetItemHanName());
		}

		for (int i = 0; i < MAX_ITEM_GRADE; ++i)
		{
			if ((ItemGrade[i].iGrade) > 0) SendMessage(hRare5Wnd, CB_ADDSTRING, 0, (LPARAM)ItemGrade[i].iGrade);//Han_Name[i].GetItemHanName());
		}
		for (int i = 0; i < MAX_ITEM_RARE; ++i)
		{
			if (strlen(ItemRare[i].LocalName) > 0) SendMessage(hRare1Wnd, CB_ADDSTRING, 0, (LPARAM)ItemRare[i].LocalName);
		}
		for (int i = 0; i < MAX_ITEM_RARE; ++i)
		{
			if (strlen(ItemRare[i].LocalName) > 0) SendMessage(hRare2Wnd, CB_ADDSTRING, 0, (LPARAM)ItemRare[i].LocalName);
		}

		for (int i = 0; i < MAX_ITEM_RARE; ++i)
		{
			if (strlen(ItemRare[i].LocalName) > 0) SendMessage(hRare3Wnd, CB_ADDSTRING, 0, (LPARAM)ItemRare[i].LocalName);
		}
		for (int i = 0; i < MAX_ITEM_RARE; ++i)
		{
			if (strlen(ItemRare[i].LocalName) > 0) SendMessage(hRare4Wnd, CB_ADDSTRING, 0, (LPARAM)ItemRare[i].LocalName);
		}
		for (int i = 0; i < MAX_ITEM_FUNCTION; ++i)
		{
			if (strlen(ItemFunction[i].Name) > 0) SendMessage(hDynamicWnd, CB_ADDSTRING, 0, (LPARAM)ItemFunction[i].Name);
		}



		SendMessage(hRare1Wnd, CB_SETCURSEL, 0, 0);
		SendMessage(hRare2Wnd, CB_SETCURSEL, 0, 0);
		SendMessage(hRare3Wnd, CB_SETCURSEL, 0, 0);
		SendMessage(hRare4Wnd, CB_SETCURSEL, 0, 0);
		SendMessage(hRare5Wnd, CB_SETCURSEL, 0, 0);
		SendMessage(hDynamicWnd, CB_SETCURSEL, 0, 0);
		SendMessage(hItemName, CB_SETCURSEL, 0, 0);
		SendMessage(hItemName2, CB_SETCURSEL, 0, 0);
		SetDlgItemInt(hDlg, IDC_AMOUNT, 1, false);



		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case GENERATEOK:
		{
			int itemno = GetDlgItemInt(hDlg, IDC_ITEM, NULL, false);
			int amount = GetDlgItemInt(hDlg, IDC_AMOUNT, NULL, false);
			int legendno = GetDlgItemInt(hDlg, IDC_LEGENDNO, NULL, false);
			int classtype = GetDlgItemInt(hDlg, IDC_CLASSTYPE, NULL, false);
			if (amount == 0) amount = 1;
			grade = GetDlgItemInt(hDlg, IDC_GRADE, NULL, false);
			if (grade > 15) grade = 15;
			t_packet packet;

			if (IsDlgButtonChecked(hDlg, IDC_ISRARE))
			{
				if (!grade)
				{
					JustMsg("between 1-15");
					break;



				}
				HWND hRareList = GetDlgItem(hDlg, CBO_RARE1);
				SendMessage(hRareList, WM_GETTEXT, 600, (LPARAM)temp);

				for (int i = 0; i < MAX_ITEM_RARE; i++)
				{

					if (!strcmp(temp, ItemRare[i].LocalName))
					{

						packet.u.GenerateItem.soksung1 = i;

						break;
					}
					packet.u.GenerateItem.grade = grade;
				}
				HWND hRareList2 = GetDlgItem(hDlg, CBO_RARE2);
				SendMessage(hRareList2, WM_GETTEXT, 600, (LPARAM)temp);

				for (int i = 0; i < MAX_ITEM_RARE; i++)
				{

					if (!strcmp(temp, ItemRare[i].LocalName))
					{

						packet.u.GenerateItem.soksung2 = i;


						break;
					}
					packet.u.GenerateItem.grade = grade;
				}
				HWND hRareList3 = GetDlgItem(hDlg, CBO_RARE3);
				SendMessage(hRareList3, WM_GETTEXT, 600, (LPARAM)temp);

				for (int i = 0; i < MAX_ITEM_RARE; i++)
				{

					if (!strcmp(temp, ItemRare[i].LocalName))
					{

						packet.u.GenerateItem.soksung3 = i;

						break;
					}
					packet.u.GenerateItem.grade = grade;
				}
			}
			if (IsDlgButtonChecked(hDlg, IDC_ISLEGEND))
			{
				if (!grade)
				{
					JustMsg("between 1-15");
					break;



				}
				if (classtype > 4) // this is 1-3 lol
				{
					JustMsg("Has to be 1-4");
					break;


				}


				if (legendno > 15) // 1-14
				{
					JustMsg("Has to be 1-15");
					break;


				}
				packet.u.GenerateItem.isHigh = 2;

				HWND hRareList4 = GetDlgItem(hDlg, CBO_RARE4);
				SendMessage(hRareList4, WM_GETTEXT, 600, (LPARAM)temp);

				for (int i = 0; i < MAX_ITEM_RARE; i++)
				{

					if (!strcmp(temp, ItemRare[i].LocalName))
					{

						packet.u.GenerateItem.soksung1 = i;


						break;
					}

					packet.u.GenerateItem.soksung2 = legendno;
					packet.u.GenerateItem.soksung3 = classtype;
					packet.u.GenerateItem.grade = grade;

				}


			}
			else
			{
				packet.u.GenerateItem.isHigh = 0;
			}

			if (IsDlgButtonChecked(hDlg, IDC_ISDYNAMIC))
			{

				if (!grade)
				{
					JustMsg("between 1-15");

					break;
				}
				packet.u.GenerateItem.isDynamicRare = true;

				HWND hRareList5 = GetDlgItem(hDlg, CBO_DYNAMIC);
				SendMessage(hRareList5, WM_GETTEXT, 600, (LPARAM)temp);

				for (int i = 0; i < MAX_ITEM_FUNCTION; i++)
				{

					if (!strcmp(temp, ItemFunction[i].Name))
					{

						packet.u.GenerateItem.soksung1 = i;


						break;
					}

					packet.u.GenerateItem.soksung2 = 0;
					packet.u.GenerateItem.soksung3 = 0;
					packet.u.GenerateItem.grade = grade;
				}


			}


			else
			{
				packet.u.GenerateItem.isDynamicRare = false;
			}

			// weapon name
			{

				HWND hRareList = GetDlgItem(hDlg, CBO_ITEMGEN_NAME);
				SendMessage(hRareList, WM_GETTEXT, 600, (LPARAM)temp);

				for (int i = 0; i != Num_Of_CItem_Weapon; ++i)
				{
					// cringe C not C++
					if (strcmp(Item_Ref.Item_type[WEAPON][i]->GetItemHanName(), temp) == 0)
					{
						//packet.u.GenerateItem.itemno = Item_Ref.Item_type[WEAPON][i]->GetItemId();

						// will fill the itemid box
						// could work not sure we'll see

						// give me an itemid that you know 10057
						// what's that R.E ? yes

						SetDlgItemInt(hDlg, IDC_ITEM, Item_Ref.Item_type[WEAPON][i]->GetItemId(), false);
						itemno = Item_Ref.Item_type[WEAPON][i]->GetItemId();
						packet.u.GenerateItem.type = WEAPON; // temp change it dynamically
					}
				}




				if (!itemno)
				{
					JustMsg("Please Enter item no");
					break;
				}

				if (IsDlgButtonChecked(hDlg, IDC_ISHIGH))
				{
					if (!grade)
					{
						JustMsg("between 1-15");
						break;


					}
					packet.u.GenerateItem.isHigh = 1;

					HWND hRareList = GetDlgItem(hDlg, CBO_RARE1);
					SendMessage(hRareList, WM_GETTEXT, 600, (LPARAM)temp);

					for (int i = 0; i < MAX_ITEM_RARE; i++)
					{

						if (!strcmp(temp, ItemRare[i].LocalName))
						{

							packet.u.GenerateItem.soksung1 = i;

							break;
						}
						packet.u.GenerateItem.grade = grade;
					}



				}
				HWND hRareList5 = GetDlgItem(hDlg, CBO_RARE5);
				SendMessage(hRareList5, WM_GETTEXT, 600, (LPARAM)temp);
				if (IsDlgButtonChecked(hDlg, IDC_ISNORMAL))
				{



					packet.u.GenerateItem.soksung1 = 0;
					packet.u.GenerateItem.soksung2 = 0;
					packet.u.GenerateItem.soksung3 = 0;
					packet.u.GenerateItem.grade = 0;



				}

				packet.u.GenerateItem.itemno = itemno;
				packet.u.GenerateItem.amount = amount;




				//packet.u.GenerateItem.grade = grade;
				packet.h.header.type = CMD_GENERATE_ITEM;
				packet.h.header.size = sizeof(t_generate_item);

				QueuePacket(&packet, 1);
				return (TRUE);
			}
		}
		}
					case IDCANCEL:
						EndDialog(hDlg, 0);
						return true;
					}
					return false;
					break;
				}
				
			
		}
		
	

void generateitem()
{
#ifdef _DEBUG

	HWND hDlgWnd = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_GENERATEITEM), g_hwndMain, (DLGPROC)GenerateItemNewProc);
	ShowWindow(hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect(hDlgWnd, &rect);
	GetWindowRect(g_hwndMain, &grect);
	MoveWindow(hDlgWnd, grect.right + 1, 0, rect.right, rect.bottom, true);
	ShowWindow(hDlgWnd, SW_SHOWNORMAL);

#endif
}



/////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void ChangeMapAttrDont2Geffect ( void )
{
	LPMAPFILE lpMapFile = &g_Map.file;
	
	for ( int y = 0; y < lpMapFile->wHeight; y++ )
	{
		for ( int x = 0; x < lpMapFile->wWidth; x++ )
		{
			if( TileMap[x][y].attr_dont )	TileMap[x][y].attr_light = 1;
		}
	}
}



void tool_InputMapAttr( int mx, int my, int flag )
{
	int back, front, roof;
	
	LPMAPFILE lpMapFile = &g_Map.file;
	
	LPTILE t = &TileMap[mx][my];
	
	back	= t->back;
	front	= t->front;
	roof	= t->roof;
	
	if( tMapAttr_Brush_Paint == IDC_PAINT_YES )
	{	
		if( flag )
			switch( tMapAttr )
		{
		case IDC_ATTR_DONT		:	PaintTileAttrDont( mx, my );		break;
		case IDC_ATTR_RIVER		:	PaintTileAttrRiver( mx, my );		break;
		case IDC_ATTR_GEFFECT	:	PaintTileAttrGeffect( mx, my );		break;
		case IDC_ATTR_INSIDE	:	PaintTileAttrInside( mx, my );		break;
		case IDC_ATTR_NO_BATTLE :	PaintTileAttrNoBattle( mx, my );	break;
		case IDC_ATTR_CANMATCH  :	PaintTileAttrCanMatch( mx, my );	break;
		case IDC_ATTR_TOP       :   PaintTileAttrTop(mx,my);			break;		// 010904 LTS    //높낯이의 구현 
		}
		return;
	}
	
	//________________________________________________________________________	// 010904 LTS
	if (tMapAttr_Brush_DePaint == IDC_DEPAINT_YES)								//제거 붓의 구현 
	{
		if( flag )
			switch( tMapAttr )
		{
		case IDC_ATTR_DONT		:	DePaintTileAttrDont( mx, my );		break;
		case IDC_ATTR_RIVER		:	DePaintTileAttrRiver( mx, my );		break;
		case IDC_ATTR_GEFFECT	:	DePaintTileAttrGeffect( mx, my );	break;
		case IDC_ATTR_INSIDE	:	DePaintTileAttrInside( mx, my );	break;
		case IDC_ATTR_NO_BATTLE :	DePaintTileAttrNoBattle( mx, my );	break;
		case IDC_ATTR_CANMATCH  :	DePaintTileAttrCanMatch( mx, my );	break;
		case IDC_ATTR_TOP       :   DePaintTileAttrTop(mx,my);			break; 
		}
		return;
	}
	//________________________________________________________________________	// 010904 LTS
	
	
	if( tMapAttrAllAffect == IDC_SOURCE_AFFECT_YES )
	{
		for ( int y = 0; y < lpMapFile->wHeight; y++ )
		{
			for ( int x = 0; x < lpMapFile->wWidth; x++ )
			{
				if( (int)TileMap[x][y].back == back )
				{
					switch( tMapAttr )
					{
					case IDC_ATTR_DONT		:	TileMap[x][y].attr_dont		= flag;		break;
					case IDC_ATTR_RIVER		:	TileMap[x][y].attr_river	= flag;		break;
					case IDC_ATTR_GEFFECT	:	TileMap[x][y].attr_light	= flag;		break;
					case IDC_ATTR_INSIDE	:	TileMap[x][y].attr_inside	= flag;		break;
					case IDC_ATTR_NO_BATTLE :	TileMap[x][y].attr_no_battle= flag;		break;
					case IDC_ATTR_CANMATCH  :	TileMap[x][y].canmatch      = flag;		break;
					case IDC_ATTR_TOP		:	TileMap[x][y].empty__4      = flag;		break;	// 010904 LTS
					}
				}
			}
		}
	}
	else
	{
		switch( tMapAttr )
		{
		case IDC_ATTR_DONT		:	t->attr_dont		= flag;		break;
		case IDC_ATTR_RIVER		:	t->attr_river		= flag;		break;
		case IDC_ATTR_GEFFECT	:	t->attr_light		= flag;		break;
		case IDC_ATTR_INSIDE	:	t->attr_inside		= flag;		break;
		case IDC_ATTR_NO_BATTLE	:	t->attr_no_battle	= flag;		break;
		case IDC_ATTR_CANMATCH  :	t->canmatch			= flag;		break;
		case IDC_ATTR_TOP		:	t->empty__4			= flag;		break;	// 010904 LTS
		}
	}
}










BOOL CALLBACK InputMapAttrProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{																				
	
	switch(Message)												
	{												
	case WM_INITDIALOG:		CheckRadioButton( hDlg, IDC_ATTR_DONT, IDC_ATTR_GEFFECT, tMapAttr );
		CheckRadioButton( hDlg, IDC_SOURCE_AFFECT_YES, IDC_SOURCE_AFFECT_NO, tMapAttrAllAffect );
		CheckRadioButton( hDlg, IDC_ATTR_OFFSET_YES2, IDC_ATTR_OFFSET_NO2, tMapAttr_Offset_Modify );
		CheckRadioButton( hDlg, IDC_PAINT_YES, IDC_PAINT_YES, tMapAttr_Brush_Paint );
		CheckRadioButton( hDlg, IDC_DEPAINT_YES, IDC_DEPAINT_NO, tMapAttr_Brush_DePaint);	// 010904 LTS
		return TRUE;											
		
	case WM_PAINT:										
		break;												
		
	case WM_COMMAND:								
		
		switch( LOWORD( wParam ))				
		{										
		case IDC_PAINT_YES		:	tMapAttr_Brush_Paint = IDC_PAINT_YES;		break;
		case IDC_PAINT_NO		:	tMapAttr_Brush_Paint = IDC_PAINT_NO;		break;
			//______________________________________________________________________________
		case IDC_DEPAINT_YES	:	tMapAttr_Brush_DePaint = IDC_DEPAINT_YES;	break;   // 010904 LTS
		case IDC_DEPAINT_NO		:	tMapAttr_Brush_DePaint = IDC_DEPAINT_NO;	break;
			//______________________________________________________________________________
			
		case IDC_ATTR_OFFSET_YES2:  tMapAttr_Offset_Modify = IDC_ATTR_OFFSET_YES2;	tool_ViewAttrFrontFlag = 1; break;
		case IDC_ATTR_OFFSET_NO2 :	tMapAttr_Offset_Modify = IDC_ATTR_OFFSET_NO2;	tool_ViewAttrFrontFlag = 0; break;
			
		case IDC_ATTR_DONT		:   tool_ViewAttrDontFlag	= 1;	tMapAttr = IDC_ATTR_DONT;		break; 
		case IDC_ATTR_RIVER		:   tool_ViewAttrRiver		= 1;	tMapAttr = IDC_ATTR_RIVER ;		break; 
		case IDC_ATTR_GEFFECT	:	tool_ViewAttrGEffect	= 1;	tMapAttr = IDC_ATTR_GEFFECT ;	break; 
		case IDC_ATTR_INSIDE	:	tool_ViewAttrInside		= 1;	tMapAttr = IDC_ATTR_INSIDE ;	break; 
		case IDC_ATTR_NO_BATTLE :	tool_ViewAttrNoBattle	= 1;	tMapAttr = IDC_ATTR_NO_BATTLE;	break; 
		case IDC_ATTR_CANMATCH  :	tool_ViewAttrCanMatch   = 1;	tMapAttr = IDC_ATTR_CANMATCH;	break; 
		case IDC_ATTR_OCCUPIED	:	tool_ViewAttrOccupied	= 1;	tMapAttr = IDC_ATTR_OCCUPIED;	break; 
		case IDC_ATTR_TOP		  :	tool_ViewAttrTop		= 1;	tMapAttr = IDC_ATTR_TOP;		break;
			
			
		case IDC_SOURCE_AFFECT_YES :   tMapAttrAllAffect = IDC_SOURCE_AFFECT_YES ;	break; //SetFocus( g_hwndMain );g_bIsActive = TRUE;	SetAcquire( g_bIsActive ); ;break;
		case IDC_SOURCE_AFFECT_NO  :   tMapAttrAllAffect = IDC_SOURCE_AFFECT_NO ;	break; //	SetFocus( g_hwndMain );g_bIsActive = TRUE;	SetAcquire( g_bIsActive ); ;break;
			
		case IDC_DONT_TO_GE :	if( YesOrNo( "Dont 속성을 효과벽 속성으로 Copy합니다. ", "?" ) == IDYES )
								{
									if( YesOrNo( "여기서 잠깐! \n 음.. 갱신된 Data는 UnDo되지 않습니다. '.MAP'을 먼저 저장하세요 \n\n 'Yes'를 누르면 게속진행합니다.", "확인" )  == IDYES )
									{
										ChangeMapAttrDont2Geffect();
										JustMsg( "모든 Dont 속성이 효과벽속성으로 바뀌었습니다. " );
									}
								}
			break;									
			
		case IDOK     : 							
			{									
				char temp[FILENAME_MAX];
				sprintf( temp, "./map/%s.map", MapName );
				
				if( YesOrNo( "맵을 갱신합니다. \n\n '%s'", "확인" ) == IDYES )
				{
					MapWirteFile( &g_Map.file, temp );
				}
				return (TRUE);
			}
			
		case IDCANCEL :		EndDialog(hDlg, LOWORD(wParam));	
			tool_ID_INPUT_MAP_ATTR	= 0;
			tool_ViewAttrDontFlag	= 0;
			tool_ViewAttrRiver		= 0;
			tool_ViewAttrFrontFlag	= 0;
			tool_ViewAttrGEffect	= 0;					
			tool_ViewAttrInside     = 0;
			tool_ViewAttrNoBattle	= 0;
			tool_ViewAttrCanMatch   = 0;
			tool_ViewAttrOccupied	= 0;
			tool_ViewAttrTop		= 0;    // 010904 LTS
			
			;
			return (TRUE);
		}		
		break;		
	}				
	
	return FALSE;	
}					

BOOL CALLBACK tool_MouseCursorProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{																				
	static int cursorno = CurMouseCursorNo;								
	char temp[ FILENAME_MAX];
	switch(Message)												
	{							
	case WM_INITDIALOG:
		return TRUE;											
		
	case WM_PAINT:										
		break;												
		
	case WM_COMMAND:								
		
		switch( LOWORD( wParam ))				
		{										
		case IDC_CURSOR_DOWN	:	Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_START ), temp, FILENAME_MAX );
			cursorani[CurMouseCursorNo-1].start = atoi( temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_END ), temp, FILENAME_MAX );
			cursorani[CurMouseCursorNo-1].end = atoi( temp );
			
			cursorno --;
			if( cursorno < 1 ) cursorno = MAX_MOUSE_CURSOR;
			CursorNo( cursorno );
			;
			GameTestProc( &g_DirectDrawInfo );
			break;
			
		case IDC_CURSOR_UP		:	Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_START ), temp, FILENAME_MAX );
			cursorani[CurMouseCursorNo-1].start = atoi( temp );
			Edit_GetText( GetDlgItem(hDlg, IDC_EDIT_END ), temp, FILENAME_MAX );
			cursorani[CurMouseCursorNo-1].end = atoi( temp );
			
			cursorno ++;
			if( cursorno >= MAX_MOUSE_CURSOR ) cursorno = 1;
			CursorNo( cursorno );
			;
			GameTestProc( &g_DirectDrawInfo );
			break;
			
		case IDLOAD     :	LoadCursorAni();
			LoadMouseCursor( "Cursor.Spr" );
			JustMsg(" CursorAni.txt와 Cursor.Spr를 읽었습니다." );
			break;
			
		case IDOK     : 							
			{									
				return (TRUE);
			}
			
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));	
			tool_MouseCursorBlock = 0;
			;
			return (TRUE);
		}		
		break;		
	}				
	
	return FALSE;	
}					








static int tool_MapObjectEditBoxSetting( HWND hDlg )
{
	char temp[ FILENAME_MAX];
	
	sprintf( temp, "%d", 	tool_CurMapObjectId );
	Edit_SetText( GetDlgItem(hDlg, IDC_EDIT_OBJECT_NO ), temp );
	
	return 1;
}

static int tool_MapObjectSelect( HWND hDlg, int id )
{
	
	
	//	if( MapNumber == 0 ) eno = 1999;
	//	else eno = 999;
	
	tool_CurMapObjectId += id  ;
	
	//	if( tool_CurMapObjectId < 1 ) tool_CurMapObjectId	= eno;
	//	if( tool_CurMapObjectId >= eno ) tool_CurMapObjectId =   1;
	
	LoadTempSprite( tool_CurMapObjectId ); 
	
	tool_MapObjectEditBoxSetting( hDlg );											
	
	;									
	
	GameTestProc( &g_DirectDrawInfo );								
	
	return 1;											
}														


BOOL CALLBACK tool_IDD_INPUT_MAPOBJECTProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{											
	switch(Message)							
	{										
	case WM_INITDIALOG:		LoadTempSprite( tool_CurMapObjectId ); 	
		tool_MapObjectEditBoxSetting( hDlg );
		return TRUE;											
		
	case WM_PAINT:	
		break;									
		
	case WM_COMMAND:
		
		switch( LOWORD( wParam ))
		{		
		case IDC_IO_U1000:	tool_MapObjectSelect( hDlg, -1000); break;
		case IDC_IO_U100 :	tool_MapObjectSelect( hDlg, -100 ); break;
		case IDC_IO_U10  :	tool_MapObjectSelect( hDlg, -10  ); break;
		case IDC_IO_U1   :	tool_MapObjectSelect( hDlg, -1   ); break;
		case IDC_IO_D1   :	tool_MapObjectSelect( hDlg,  1   ); break;
		case IDC_IO_D10  :	tool_MapObjectSelect( hDlg,  10  ); break;
		case IDC_IO_D100 :	tool_MapObjectSelect( hDlg,  100 ); break;
		case IDC_IO_D1000:	tool_MapObjectSelect( hDlg,  1000); break;
			
		case IDOK     :
		case IDCANCEL :		EndDialog(hDlg, LOWORD(wParam));	
			tool_IDD_INPUT_MAPOBJECT = 0;
			;
			return (TRUE);
			
		case IDCANCEL2 :	DeleteMapObjectAll( tool_CurMapObjectId );
			
			break;
		}		
		break;		
	}				
	
	return FALSE;	
}					







/////////////////////////////////////////////////////////////////////////////////


void tool_CheckViewAttr( WPARAM wParam )
{	
	switch( wParam )
	{
	case ID_VIEW_ATTR_RIVER		:	tool_ViewAttrRiver		= !tool_ViewAttrRiver;			break;
	case ID_VIEW_ATTR_GEFFECT	:	tool_ViewAttrGEffect	= !tool_ViewAttrGEffect;		break;
	case ID_VIEW_ATTR_SHOW_ROOF :	tool_ViewAttrRoofOnOff	= !tool_ViewAttrRoofOnOff;		break;
	case ID_VIEW_ATTR_EVENT		:	tool_ViewAttrEventFlag	= !tool_ViewAttrEventFlag;		break;
	case ID_VIEW_ATTR_DONT		:	tool_ViewAttrDontFlag	= !tool_ViewAttrDontFlag;		break;
	case ID_VIEW_ATTR_FRONT		: 	tool_ViewAttrFrontFlag	= !tool_ViewAttrFrontFlag;		break;
	case ID_VIEW_ATTR_INSIDE	:	tool_ViewAttrInside		= !tool_ViewAttrInside;			break;
	case ID_VIEW_ATTR_NO_BATTLE :	tool_ViewAttrNoBattle	= !tool_ViewAttrNoBattle;		break;
	case ID_VIEW_ATTR_CANMATCH  :	tool_ViewAttrCanMatch   = !tool_ViewAttrCanMatch;		break;
		//	case ID_VIEW_ATTR_TOP  :	tool_ViewAttrTop   = !tool_ViewAttrTop;		break;   // 010904 LTS
	case ID_VIEW_ATTR_OCCUPIED		:	tool_ViewAttrOccupied	= !tool_ViewAttrOccupied;		break;
	}
}	






/////////////////////////////////////////////////////////////////////////////////////
//				아이템 생성에 관련된 프로시져..			//1216
/////////////////////////////////////////////////////////////////////////////////////
extern k_client_create_item gener_item;
extern bool bGenerateItemOn;

BOOL CALLBACK GenerateItemProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{						
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			SetDlgItemInt( hDlg, IDC_TYPE, gener_item.type, true );
			SetDlgItemInt( hDlg, IDC_NUMBER, gener_item.num, true );
			SetDlgItemInt( hDlg, IDC_DURATION, gener_item.dur, true );
			return TRUE;	
		}
		
	case WM_PAINT:		SetTimer( g_hwndMain, 13, 1000, NULL);
  						break;			
						
	case WM_TIMER  :	if( wParam == 13 ) 	
						{
							;
							GameTestProc( &g_DirectDrawInfo );
						}
		break;
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{			
		case IDOK :	{
			char szBuffer[127];
			GetDlgItemText( hDlg, IDC_TYPE, szBuffer, sizeof( szBuffer ) -1 );
			gener_item.type = atoi(szBuffer);
			GetDlgItemText( hDlg, IDC_NUMBER, szBuffer, sizeof( szBuffer ) -1 );
			gener_item.num = atoi(szBuffer);
			GetDlgItemText( hDlg, IDC_DURATION, szBuffer, sizeof( szBuffer ) -1 );
			gener_item.dur = (DWORD )atof(szBuffer);
			GetDlgItemText( hDlg, IDC_GENER_ITEM_NUM, szBuffer, sizeof( szBuffer ) -1 );
			gener_item.gennum = atoi(szBuffer);
			
			bGenerateItemOn = true;
			
			
			
					}
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));
			;
			return (TRUE);
#ifdef _DEBUG
		case IDC_SPAWN_SARD:
			{
				t_packet p;
				p.h.header.type = CMD_CONFORM_SADONIX;
				p.h.header.size = sizeof(t_ConformSadonix);
				memcpy(&p.u.ConformSadonix.name, "",20);
				p.u.ConformSadonix.item.item_no = 1046;
				p.u.ConformSadonix.x = Hero->x;
				p.u.ConformSadonix.y = Hero->y;
				p.u.ConformSadonix.cn = Hero->id;
				QueuePacket(&p, 1);
				return true;
			}
#endif
		default : return true;
		}
		break;
	}
	return false;
}

void CreateItemByTool( )
{
#ifdef _DEBUG
	HWND hCreateitem = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_GENERATE_ITEM ), g_hwndMain, (DLGPROC)GenerateItemProc );
	ShowWindow(hCreateitem, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hCreateitem, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hCreateitem, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow(hCreateitem, SW_SHOW);
#endif
}




BOOL CALLBACK DeleteNPCProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{						
	
	char szBuffer[127];
	int from, to;
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			return TRUE;	
		}
		
	case WM_PAINT:		break;			
		
	case WM_TIMER  :	break;
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{			
			// 배치된 NPC중에서 from->to로 바꾼다. 
		case ID_NPC_CHANGE :
			GetDlgItemText( hDlg, IDC_EDIT_FROM, szBuffer, sizeof( szBuffer ) -1 );
			from = atoi(szBuffer);
			GetDlgItemText( hDlg, IDC_EDIT_TO, szBuffer, sizeof( szBuffer ) -1 );
			to	 = atoi(szBuffer);
			
			sprintf( szBuffer, "현 맵에 있는 NPC배치중에서 \n %d(%s)-> %d(%s)로 바꿉니다.", from, g_infNpc[from].szName, to, g_infNpc[to].szName); // CSD-030419
			
			if( YesOrNo( szBuffer, "주의" ) == IDYES ) 
			{
				ChangeNPCGenerate( from, to );
				break;
			}
			break;
			
			// 전체 NPC를 지운다. 
		case ID_NPC_ALL_DELETE :
			sprintf( szBuffer, "현 맵에 있는 NPC배치를 모두 없앱니다. " );
			
			if( YesOrNo( szBuffer, "주의" ) == IDYES ) 
			{
				DeleteNPCGenerate();
				
				// 지우는 함수... 
				break;
			}
			break;
			
			//  하나를 지운다. 
		case IDOK :	{	
			int  no;
			
			GetDlgItemText( hDlg, IDC_EDIT1, szBuffer, sizeof( szBuffer ) -1 );
			no = atoi(szBuffer);
			sprintf( szBuffer, "다음의 NPC배치를 없앱니다. \n %d번 %s ", no, g_infNpc[no].szName); // CSD-030419
			
			if( YesOrNo( szBuffer, "주의" ) == IDYES ) 
			{
				DeleteNPCGenerate( no );
				// 지우는 함수... 
				break;
			}
			break;
			
					}
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
		default : return true;
		}
		break;
	}
	return false;
}


void DeleteNPCTool( )
{
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	HWND hDeleteNPC= CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_NPC_DELETE ), g_hwndMain, (DLGPROC)DeleteNPCProc );
	ShowWindow( hDeleteNPC, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDeleteNPC, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hDeleteNPC, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow( hDeleteNPC, SW_SHOW);
#endif
#endif
}






/////////////////////////////////////////////////////////////////////////////////////
//				캐릭터를 내맘대로
/////////////////////////////////////////////////////////////////////////////////////













BOOL CALLBACK 
ChangeCharacterAbilityProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)		//### 1216 YGI
{		
	int ability_to_idc[] ={ IDC_ABILIT_STR ,IDC_ABILIT_DEX ,IDC_ABILIT_CON ,IDC_ABILIT_WIS ,
		IDC_ABILIT_INT ,IDC_ABILIT_CHA ,IDC_ABILIT_MOVP,IDC_ABILIT_ENDU,
		IDC_ABILIT_MOR ,IDC_ABILIT_LUCK,IDC_ABILIT_WSPS };
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			for( int i=STR; i<=LUCK;i++ )
			{
				SetDlgItemInt( hDlg, ability_to_idc[i] , SCharacterData.nCharacterAbility[i], true );
			}
			
			SetDlgItemInt( hDlg, IDC_ABILIT_MONEY, SCharacterData.nMoney, true );
			return TRUE;	
		}
		
	case WM_PAINT:		SetTimer( g_hwndMain, 13, 1000, NULL);
  						break;			
						
	case WM_TIMER  :	if( wParam == 13 ) 	
						{
							;
							GameTestProc( &g_DirectDrawInfo );
						}
		break;
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{			
		case IDC_ABILIT_STR :			break;
		case IDOK :
		case IDC_UPDATE :	{
			;
			char szBuffer[127];
			double data;
			UINT flag;
			for( int i=STR; i<=LUCK; i++ )
			{
				flag = GetDlgItemText( hDlg, ability_to_idc[i], szBuffer, sizeof( szBuffer ) -1 ); 
				if( flag )
				{
					data = atof( szBuffer );
					SCharacterData.nCharacterAbility[i] = (int) data;
				}
			}
			
			flag = GetDlgItemText( hDlg, IDC_ABILIT_MONEY, szBuffer, sizeof( szBuffer ) -1 );
			if( flag )
			{
				data = atof( szBuffer );
				SCharacterData.nMoney = (DWORD) data;
			}
			
			flag = GetDlgItemText( hDlg, IDC_ABILIT_LVPOINT, szBuffer, sizeof( szBuffer ) -1 );
			if( flag )
			{
				data = atof( szBuffer );
				SCharacterData.LvUpPoint = (int) data;
			}
			
			bCharListUp = true;
							}
		case IDCANCEL :	
			{
				EndDialog(hDlg, LOWORD(wParam));
				SetFocus2(HWND_GAME_MAIN);//021001 lsw
				;
				return (TRUE);
			}break;
		default : return true;
		}					
		break;							
	}										
	return false;						
}									


void ChangeCharacterAbility( )
{										
	
#ifdef _DEBUG	

#ifndef LIGHT_GM_CLIENT
	
	HWND hAbilityDlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_CHARACTER_ABILITY_SET ), g_hwndMain, (DLGPROC)ChangeCharacterAbilityProc );
	//	DialogBox( g_hInstance, MAKEINTRESOURCE( IDD_CHARACTER_ABILITY_SET), NULL, (DLGPROC)ChangeCharacterAbilityProc );
	//	HWND hAbilityDlg = GetDlgItem(NULL, IDD_CHARACTER_ABILITY_SET );
	ShowWindow(hAbilityDlg, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;							
	RECT grect;								
	GetWindowRect( hAbilityDlg, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hAbilityDlg, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow(hAbilityDlg, SW_SHOW);
	
	SetFocus( hAbilityDlg );//021001 lsw

#endif
	
#endif								
}								

////////////////////////////////////////////////////////////////////////////////////////
//	패스워드 입력	// 0405
////////////////////////////////////////////////////////////////////////////////////////
// acer4
BOOL CALLBACK ToolPasswordProc2(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)		
{		
	switch(Message)		
	{					
	case WM_INITDIALOG:	break;
		
	case WM_PAINT:		SetTimer( g_hwndMain, 20, 1000, NULL);
  						break;			
						
	case WM_TIMER  :	if( wParam == 20 ) 	
						{
							;
							GameTestProc( &g_DirectDrawInfo );
						}
		break;
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{			
		case IDOK :{
			;
			char lpszPW[21] = {0,};
			GetDlgItemText( hDlg, IDC_EDIT_TOOL_PW, lpszPW, 20 );
			if( !(*lpszPW) )
			{
				return false;
			}
			CallServer( CMD_SEACH_OTHER_CHAR, lpszPW, strlen(lpszPW)  );
				   }
		case IDCANCEL :	
			{
				EndDialog(hDlg, LOWORD(wParam));
				SetFocus2(HWND_GAME_MAIN);//021001 lsw
				;
				return (TRUE);
			}break;
		default : return true;
		}
		break;
	}
	return false;
}

extern void SendToolPassword( char *password );
BOOL CALLBACK ToolPasswordProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)		
{		
	switch(Message)		
	{					
	case WM_INITDIALOG:	break;
		
	case WM_PAINT:		SetTimer( g_hwndMain, 13, 1000, NULL);
  						break;			
						
	case WM_TIMER  :	if( wParam == 13 ) 	
						{
							;
							GameTestProc( &g_DirectDrawInfo );
						}
		break;
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{			
		case IDOK :{
			;
			char lpszPW[11] = {0,};
			GetDlgItemText( hDlg, IDC_EDIT_TOOL_PW, lpszPW, 11 );
			if( !(*lpszPW) )
			{
				EndDialog(hDlg, LOWORD(wParam));
				return false;
			}
			SendToolPassword( lpszPW );
			EndDialog(hDlg, LOWORD(wParam));
				   }
		case IDCANCEL :	
			{
				EndDialog(hDlg, LOWORD(wParam));
				SetFocus2(HWND_GAME_MAIN);//021001 lsw
				;
				return (TRUE);
			}break;
		default : return true;
		}
		break;
	}
	return false;
}

void ToolPassWord( int type = 0 )
{
	if( g_StartMenuOn ) return;
	
#ifdef _DEBUG
	
	HWND hAbilityDlg;
	switch( type )
	{
	case 1: hAbilityDlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_INPUT_PASSWORD ), g_hwndMain, (DLGPROC)ToolPasswordProc2 );
		break;
	default : hAbilityDlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_INPUT_PASSWORD ), g_hwndMain, (DLGPROC)ToolPasswordProc );
	}
	
	ShowWindow(hAbilityDlg, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hAbilityDlg, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hAbilityDlg, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow(hAbilityDlg, SW_SHOW);
	
	SetFocus( hAbilityDlg );//021001 lsw
	
#endif
}



















BOOL CALLBACK ChangeCharacterTacticsProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)		
{					
	int tac_to_idc[] ={ IDC_TAC_CRAPPLE	,IDC_TAC_SWORD	,IDC_TAC_ARCHERY,IDC_TAC_FENCING,
		IDC_TAC_MACE	,IDC_TAC_PIERCE	,IDC_TAC_WHIRL	,IDC_TAC_HURL 	,
		IDC_TAC_PARRYING,IDC_TAC_DSWORD,IDC_TAC_DMACE	,IDC_TAC_MAGERY	, 
		IDC_TAC_ORISON,	};
	int tac2_to_idc[] ={IDC_TAC_CRAPPLE2	,IDC_TAC_SWORD2	,IDC_TAC_ARCHERY2,IDC_TAC_FENCING2,
		IDC_TAC_MACE2		,IDC_TAC_PIERCE2,IDC_TAC_WHIRL2	,IDC_TAC_HURL2	,
		IDC_TAC_PARRYING2	,IDC_TAC_DSWORD2,IDC_TAC_DMACE2	,IDC_TAC_MAGERY2	, 
		IDC_TAC_ORISON2,	};
	
	
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{			
			for( int i=CRAPPLE; i<=ORISON;i++ )
			{		
				SetDlgItemInt( hDlg, tac_to_idc[i] , SCharacterData.TacticsExp[i], true );
				SetDlgItemInt( hDlg, tac2_to_idc[i] , SCharacterData.SkillId[i+88], true );
			}		
			return TRUE;	
		}			
		
	case WM_PAINT:		SetTimer( g_hwndMain, 13, 1000, NULL);
  						break;								
						
	case WM_TIMER  :	if( wParam == 13 ) 	
						{								
							;
							GameTestProc( &g_DirectDrawInfo );
						}									
		break;						
		
	case WM_COMMAND:						
		switch( LOWORD( wParam ))			
		{								
		case IDC_CONVERT : {		
			;	
			char szBuffer[127];
			UINT flag;
			int data2;
			for( int i=CRAPPLE; i<=ORISON; i++ )
			{	
				flag = GetDlgItemText( hDlg, tac2_to_idc[i], szBuffer, sizeof( szBuffer ) -1 ); 
				if( flag )
				{
					data2 = atoi( szBuffer );
					DWORD exp = GetExpByLv( data2 );
					SetDlgItemInt( hDlg, tac_to_idc[i] , exp, true );
				}
			}	
			break;
						   }		
		case IDC_UPDATE :	{		
			;		
			char szBuffer[127];
			double data;
			UINT flag;
			int data2;
			int i;
			for( i=CRAPPLE; i<=ORISON; i++ )
			{		
				flag = GetDlgItemText( hDlg, tac2_to_idc[i], szBuffer, sizeof( szBuffer ) -1 ); 
				if( flag )
				{	
					data2 = atoi( szBuffer );
					DWORD exp = GetExpByLv( data2 );
					SetDlgItemInt( hDlg, tac_to_idc[i] , exp, true );
				}	
			}		
												
			for( i=CRAPPLE; i<=ORISON; i++ )
			{		
				flag = GetDlgItemText( hDlg, tac_to_idc[i], szBuffer, sizeof( szBuffer ) -1 ); 
				if( flag )
				{	
					data = atof( szBuffer );
					SCharacterData.TacticsExp[i] = (DWORD )data;
				}	
			}		
			bTackListUp  = true;
							}		
		case IDCANCEL :	
			{
				EndDialog(hDlg, LOWORD(wParam));
				SetFocus2(HWND_GAME_MAIN);//021001 lsw
				;
				return (TRUE);
			}break;
		default : return true;		
		}								
		break;								
	}										
	return false;							
}											


void ChangeCharacterTactics( )
{
	
#ifdef _DEBUG

#ifndef LIGHT_GM_CLIENT
	
	HWND hTacticsDlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_CHANGE_TACTICS ), g_hwndMain, (DLGPROC)ChangeCharacterTacticsProc );
	ShowWindow(hTacticsDlg, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hTacticsDlg, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hTacticsDlg, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow(hTacticsDlg, SW_SHOW);
	
	SetFocus( hTacticsDlg );//021001 lsw

#endif
	
#endif
}


// Skill 경험치 변경...
///////////////////////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG

BOOL CALLBACK SkillExpProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{					
	int handle[ 45] = { IDC_SKILL_EDIT_0, IDC_SKILL_EDIT_1, IDC_SKILL_EDIT_2, IDC_SKILL_EDIT_3, IDC_SKILL_EDIT_4, 
		IDC_SKILL_EDIT_5,   IDC_SKILL_EDIT_6,  IDC_SKILL_EDIT_7,  IDC_SKILL_EDIT_8,  IDC_SKILL_EDIT_9, 
		IDC_SKILL_EDIT_10,  IDC_SKILL_EDIT_11, IDC_SKILL_EDIT_12, IDC_SKILL_EDIT_13, IDC_SKILL_EDIT_14, 
		IDC_SKILL_EDIT_15,  IDC_SKILL_EDIT_16, IDC_SKILL_EDIT_17, IDC_SKILL_EDIT_18, IDC_SKILL_EDIT_19, 
		IDC_SKILL_EDIT_20,  IDC_SKILL_EDIT_21, IDC_SKILL_EDIT_22, IDC_SKILL_EDIT_23, IDC_SKILL_EDIT_24, 
		IDC_SKILL_EDIT_25,  IDC_SKILL_EDIT_26, IDC_SKILL_EDIT_27, IDC_SKILL_EDIT_28, IDC_SKILL_EDIT_29, 
		IDC_SKILL_EDIT_30,  IDC_SKILL_EDIT_31, IDC_SKILL_EDIT_32, IDC_SKILL_EDIT_33, IDC_SKILL_EDIT_34, 
		IDC_SKILL_EDIT_35,  IDC_SKILL_EDIT_36, IDC_SKILL_EDIT_37, IDC_SKILL_EDIT_38, IDC_SKILL_EDIT_39, 
		IDC_SKILL_EDIT_39,  IDC_SKILL_EDIT_39, IDC_SKILL_EDIT_39, IDC_SKILL_EDIT_39, IDC_SKILL_EDIT_39 };
	
	
	
	char temp[FILENAME_MAX];
	
	int c;
	int t;
	int i;
	
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			c = 0;
			for( i = 0 ; i < 45; i ++)
			{
				sprintf( temp, "%s", SCharacterData.nJobSkill[c++] );
				Edit_SetText (GetDlgItem(hDlg, handle[i] ), temp);
			}					
			
			return TRUE;		
		}					
		
	case WM_PAINT:		break;			
		
	case WM_TIMER  :	break;
		
	case WM_COMMAND:		
		switch( LOWORD( wParam )) 
		{				
		case IDOK :	{	
			t_packet p;
			p.h.header.type  = CMD_DEBUG_CHANGE_SKILL_EXP;
			p.h.header.size  = sizeof( t_debug_change_skill_exp );
			
			p.u.debug_change_skill_exp.id = Hero->id;
			
			c = 0;
			for( i = 0 ; i < 45 ; i ++)
			{
				Edit_GetText( GetDlgItem(hDlg, handle[i] ), temp, FILENAME_MAX);
				t = atoi( temp );
				if( t < 0 ) t = 0;
				if( t > 1000000 ) t = 1000000;
				//011012 lsw >
				SCharacterData.nJobSkill[ c].skillexp  = t;
				//011012 lsw <
				p.u.debug_change_skill_exp.skillexp[c] = t;
				c++;
			}
			QueuePacket( &p, 1 );
			break;
			
					}
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
		default : return true;
		}
		break;
	}
	return false;
}

#endif



void ChangeSkillExp( )
{
#ifdef _DEBUG

#ifndef LIGHT_GM_CLIENT
	
	HWND hSkillExp= CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_SKILL_EXP ), g_hwndMain, (DLGPROC)SkillExpProc );
	ShowWindow( hSkillExp, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hSkillExp, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hSkillExp, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow( hSkillExp, SW_SHOW);

#endif

#endif
}




HWND PbcHwnd;
HWND PbcAllHwnd;

BOOL CALLBACK PublicBroadCastingProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	static char temp[ MAX_PATH];
	
	switch(Message)
	{
	case WM_INITDIALOG:		Edit_SetText ( GetDlgItem(hDlg, IDC_EDIT1 ), temp );
		return TRUE;
		
	case WM_PAINT:
		break;
		
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDOK     :		Edit_GetText( GetDlgItem(hDlg, IDC_EDIT1 ), temp, MAX_PATH );
			SendBBS( temp );
			break;
			
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));	
			return (TRUE);
		}
		break;
	}
	
	return FALSE;	
}

void PublicBroadCasting( void )
{
	
#ifdef _DEBUG
	
	PbcHwnd = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_PUBLIC_MESSAGE ), g_hwndMain, (DLGPROC)PublicBroadCastingProc );
	ShowWindow( PbcHwnd, SW_SHOW );
	
#endif
	
}


BOOL CALLBACK PublicBroadCastingAllProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	static char temp[ MAX_PATH];
	
	switch(Message)
	{
	case WM_INITDIALOG:		Edit_SetText ( GetDlgItem(hDlg, IDC_EDIT1 ), temp );
		return TRUE;
		
	case WM_PAINT:
		break;
		
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDOK     :		Edit_GetText( GetDlgItem(hDlg, IDC_EDIT1 ), temp, MAX_PATH );
			SendBBSAll( temp );
			break;
			
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));	
			return (TRUE);
		}
		break;
	}
	
	return FALSE;	
}

void PublicBroadCastingtoAll( void )
{
	
#ifdef _DEBUG
	
	PbcAllHwnd = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_PUBLIC_MESSAGE_ALL ), g_hwndMain, (DLGPROC)PublicBroadCastingAllProc );
	ShowWindow( PbcAllHwnd, SW_SHOW );
	
#endif
	
}



#ifdef _DEBUG

void SystemMenuProcess( WPARAM wParam )
{				
	static int snow;
	static int rain;
	int  ScreenX;		
	RECT rect, grect;		
	int i,j;
	
	
	switch (wParam)	
	{
#ifndef LIGHT_GM_CLIENT
	case ID_CHECK_EQUIP :			CheckEquipmentData( g_hwndMain, g_hInstance );
		break;	
		
	case ID_CHECK_ANIMATION :		CheckAnimationEdit(  g_hwndMain, g_hInstance );
		break;	
		
	case ID_CHECK_CLOTH_ORDER :		SysInfo.viewcharacterframe =  !SysInfo.viewcharacterframe;
		break;	
		
	case ID_VIEW_ANIMATION_FRAME :	tool_ViewAnimationFrame = !tool_ViewAnimationFrame;
		break;	
	case ID_VIEW_PROFILER: // 031013 kyo
		{
			::ShowProFileDlg();
			break;
		}			
	case ID_DRAWMAP :				tool_DrawMap = !tool_DrawMap;
		break;
	case ID_DRAWFRONTMAP :			tool_DrawFrontMap = !tool_DrawFrontMap;
		break;
		
	case ID_DRAWOBJECT :			tool_DrawObject = !tool_DrawObject;
		break;
		
	case ID_DONT_VIEW_MESSAGE :     tool_ViewMessage = !tool_ViewMessage;
		break;
		
	case ID_DONT_VIEW_CHAT	:		tool_ViewChat	= !tool_ViewChat;
		break;
		
	case ID_DONT_VIEW_DAMAGE_NUMBER : tool_ViewNumber= !tool_ViewNumber;
		break;
		
	case ID_DONT_VIEW_MAIN_INTERFACE: tool_ViewMenu = !tool_ViewMenu;
		break;
		
		
	case ID_DRAWROOFMAP :			tool_DrawRoofMap = !tool_DrawRoofMap;
		
		for( i = 0 ; i < g_Map.file.wHeight ; i ++)
		{
			for( j = 0 ;  j < g_Map.file.wWidth ; j ++)
			{
				// 1이면 그리지 않는다
				TileMap[ j ][ i].show_roof = tool_DrawRoofMap;
			}
		}
		break;

	case ID_CHECK_QUEST :	ToolCheckQuestMenu(); break;
#endif
		
	case ID_EVENT_SCRIPT_SAVE : 
		{
			//		char temp[FILENAME_MAX];
			//		FILE *fp;
			//		sprintf( tmep, "%s", mapname );
			//		fp =Fopen( 
		}
		break;
#ifndef LIGHT_GM_CLIENT
	case ID_VIEW_ATTR_GEFFECT	:
	case ID_VIEW_ATTR_RIVER		:
	case ID_VIEW_ATTR_SHOW_ROOF	:
	case ID_VIEW_ATTR_DONT		:	 
	case ID_VIEW_ATTR_EVENT		:
	case ID_VIEW_ATTR_FRONT		:		
	case ID_VIEW_ATTR_OCCUPIED	:
	case ID_VIEW_ATTR_INSIDE	:	tool_CheckViewAttr( wParam ); 
		break;
		
	case ID_CHANGE_BODYRGB :		ChangeBodyRGB( );
		break;
#endif
	case ID_MULTICHAR :
		{
			//acer6
			char *type = "MOVE RIGHT BUTTON";
			CallServer( CMD_USE_GM_TOOL, type, strlen( type ) );
			
			tool_MultiChar = !tool_MultiChar;
			break;
		}
		break;
#ifndef LIGHT_GM_CLIENT
		
	case ID_FRAMECHECK : tool_FrameCheck = !tool_FrameCheck;
		break;
#endif	
	case ID_SYSINFO_GAMEMAKEMODE : SysInfo.gamemakemode = !SysInfo.gamemakemode;
		break;
		
	case ID_VIEW_NIGHT0 :	DayLightControl =31;	break;
	case ID_VIEW_NIGHT3 :	DayLightControl = 3 ;	break;
	case ID_VIEW_NIGHT5 :	DayLightControl = 5 ;	break;
	case ID_VIEW_NIGHT7 :	DayLightControl = 7 ;	break;
	case ID_VIEW_NIGHT9 :	DayLightControl = 9 ;	break;
	case ID_VIEW_NIGHT11:	DayLightControl = 11;	break;
	case ID_VIEW_NIGHT13 :	DayLightControl = 13;	break;
	case ID_VIEW_NIGHT15 :	DayLightControl = 15;	break;
	case ID_VIEW_NIGHT17 :	DayLightControl = 17;	break;
	case ID_VIEW_NIGHT19 :	DayLightControl = 19;	break;
	case ID_VIEW_NIGHT21 :	DayLightControl = 21;	break;
	case ID_VIEW_NIGHT23 :	DayLightControl = 23;	break;
	case ID_VIEW_NIGHT25 :	DayLightControl = 25;	break;
	case ID_VIEW_NIGHT30 :	DayLightControl = 30;	break;

		///////////////// SoundUp lkh 테스트를 위해 ////////////////////	
	case ID_VIEW_RAIN  :	rain = !rain;
		if( rain )
		{
			RainStart( 900, 0 );
			RainAmount = 900;
			Statue_Weather = 1;
		}
		else
		{
			RainStop();
			RainAmount = 0;
			Statue_Weather = 0;
		}
		break;
		
	case ID_VIEW_SNOW  :	snow = !snow;
		if( snow ) SnowStart( 600, 0 );
		else SnowStop();
		break;
		// 031110 YGI
#ifndef LIGHT_GM_CLIENT
	case ID_CHANGE_FONT : 
		{
			extern int LoadHangulEnglishFont( char *caHangulFileName, char *caEngFileName );
			LoadHangulEnglishFont( NULL, NULL );
			break;
		}
		
	case ID_SIGHT_VIEW_NORMAL :		if( Hero )	Hero->sight = 10;
		break;
		
	case ID_SIGHT_VIEW_WIDER   :	if(Hero ) Hero->sight = 540;
		break;
		
		
		
	case ID_MAPOBJECT_MODIFY : tool_mapobject_modify = !tool_mapobject_modify;
		SysInfo.gamemakemode = 1;
								if( tool_mapobject_modify ) 
									AddCurrentStatusMessage( 255,255,255, "MapObject수정가능" );
								else AddCurrentStatusMessage( 255,0,0, "MapObject수정 불가" );
								break;
								
								
								
								
	case ID_INPUT_MAP_ATTR :
		{
			
			tool_ID_INPUT_MAP_ATTR = !tool_ID_INPUT_MAP_ATTR;
			
			if( tool_ID_INPUT_MAP_ATTR  )
			{
				InputMapAttrHdlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_INPUT_MAP_ATTR ), g_hwndMain, (DLGPROC)InputMapAttrProc );
				ShowWindow(InputMapAttrHdlg, SW_HIDE);
				
				ScreenX = GetSystemMetrics(SM_CXSCREEN);
				//ShowWindow( CheckAnimationHwnd, SW_HIDE);
				GetWindowRect( InputMapAttrHdlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
				GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
				MoveWindow( InputMapAttrHdlg, grect.right+1, 0, rect.right, rect.bottom, true);
				ShowWindow(InputMapAttrHdlg, SW_SHOW);
			}
			else
			{	
				DestroyWindow( InputMapAttrHdlg );
				InputMapAttrHdlg  = NULL;
			}	
			break;
		}	
		
	case ID_INPUT_SKILL :	
		{
			//int  ScreenX;		
			//RECT rect, grect;		
			
			tool_ID_SKILL_INPUT = TRUE;
			
			if( tool_ID_SKILL_INPUT )
			{
				InputSkillAttrHdlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_INPUT_SKILL ), g_hwndMain, (DLGPROC)SkillToolproc );
				ShowWindow(InputSkillAttrHdlg, SW_HIDE);
				
				ScreenX = GetSystemMetrics(SM_CXSCREEN);
				//ShowWindow( CheckAnimationHwnd, SW_HIDE);
				GetWindowRect( InputSkillAttrHdlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
				GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
				//MoveWindow( InputSkillAttrHdlg, grect.right+1, 0, rect.right, rect.bottom, true);
				ShowWindow( InputSkillAttrHdlg, SW_SHOW);
			}
			//else
			//{	
			//	DestroyWindow( InputSkillAttrHdlg );
			//	InputSkillAttrHdlg  = NULL;
			//}	
			break;
		}
		
		
	case ID_SERVER_ITEMTABLE_RELOAD :
		{	
			t_packet p;
			
			p.h.header.type = CMD_SERVER_ITEMTABLE_RELOAD;
			p.h.header.size = 0;
			QueuePacket( &p, 1 );
			
			LoadItemJoinTable();
			
			break;
		}
		
	case ID_SERVER_NPC_ACCESS_TABLE_RELOAD :
		{	
			t_packet p;
			p.h.header.type = CMD_SERVER_NPC_ACCESS_TABLE_RELOAD;
			p.h.header.size = 0;
			QueuePacket( &p,1 );
			break;
		}	
		
		
	case ID_INPUT_SKILL_NPC_DELETE :		
		{
			DeleteNPCTool();
			break;
		}
	case ID_MOUSECURSOR_CHECK : 
		tool_MouseCursorBlock = !tool_MouseCursorBlock;
		
		if( tool_MouseCursorBlock  )
		{
			tool_MouseCursorHdlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_MOUSECURSOR_HOTSPOT ), g_hwndMain, (DLGPROC)tool_MouseCursorProc );
			ShowWindow(tool_MouseCursorHdlg, SW_HIDE);
			
			ScreenX = GetSystemMetrics(SM_CXSCREEN);
			//ShowWindow( CheckAnimationHwnd, SW_HIDE);
			GetWindowRect( tool_MouseCursorHdlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
			GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
			MoveWindow( tool_MouseCursorHdlg, grect.right+1, 0, rect.right, rect.bottom, true);
			ShowWindow(tool_MouseCursorHdlg, SW_SHOW);
		}
		else
		{	
			DestroyWindow( tool_MouseCursorHdlg );
			tool_MouseCursorHdlg  = NULL;
		}	
		
		break;
		
	case ID_EFFECT_TEST : tool_ID_EFFECT_TEST = !tool_ID_EFFECT_TEST;
		if( tool_ID_EFFECT_TEST )
		{
			AddCurrentStatusMessage( 0,0,255, "마법이 정상화 되었습니다." );
		}
		else
		{
			AddCurrentStatusMessage( 0,0,255, "오른쪽마우스에의해 fireBall이 나갑니다." );
		}
		break;
		
		
	case ID_INPUT_MAPOBJECT_MENU :
		tool_IDD_INPUT_MAPOBJECT = !tool_IDD_INPUT_MAPOBJECT;
		
		if( tool_IDD_INPUT_MAPOBJECT  )
		{
			tool_IDD_INPUT_MAPOBJECTHdlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_INPUT_MAPOBJECT ), g_hwndMain, (DLGPROC)tool_IDD_INPUT_MAPOBJECTProc );
			ShowWindow(tool_MouseCursorHdlg, SW_HIDE);
			
			ScreenX = GetSystemMetrics(SM_CXSCREEN);
			//ShowWindow( CheckAnimationHwnd, SW_HIDE);
			GetWindowRect( tool_IDD_INPUT_MAPOBJECTHdlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
			GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
			MoveWindow( tool_IDD_INPUT_MAPOBJECTHdlg, grect.right+1, 0, rect.right, rect.bottom, true);
			ShowWindow(tool_IDD_INPUT_MAPOBJECTHdlg, SW_SHOW);
		}
		else
		{	
			DestroyWindow( tool_IDD_INPUT_MAPOBJECTHdlg );
			tool_IDD_INPUT_MAPOBJECTHdlg  = NULL;
		}	
		break;
		
	case ID_INPUT_MYHOUSE :
		{
			if(tool_ID_INPUT_MYHOUSE == FALSE)	tool_ID_INPUT_MYHOUSE = TRUE;
			else								tool_ID_INPUT_MYHOUSE = FALSE;
			break;
		}
	case ID_OUTPUT_MYHOUSE :
		{
			if(tool_ID_OUTPUT_MYHOUSE == FALSE)	tool_ID_OUTPUT_MYHOUSE = TRUE;
			else								tool_ID_OUTPUT_MYHOUSE = FALSE;
			break;
		}
	case ID_CHARACTER_LIST	:	if( !g_StartMenuOn ) ChangeCharacterAbility( ); break;
	case ID_GENERATE_ITEM	:	if( !g_StartMenuOn ) CreateItemByTool( );		break;
	case ID_CHANGE_TACTICS	:	if( !g_StartMenuOn ) ChangeCharacterTactics( ); break;
	case IDD_SMALL_RADAR	:   tool_ViewAllCharacter = !tool_ViewAllCharacter;	
								tool_SkillTool_FARMING  = tool_SkillTool_MINING = tool_SkillTool_FISHING = tool_SkillTool_CHOPPING = tool_SkillTool_HUB = tool_SkillTool_NPC = tool_SkillTool_ITEM_SEAL = false;
								break;
								
	case IDD_SMALL_RADAR_FARM		:  tool_SkillTool_FARMING  = tool_SkillTool_MINING = tool_SkillTool_FISHING = tool_SkillTool_CHOPPING = tool_SkillTool_HUB = tool_SkillTool_NPC = tool_SkillTool_ITEM_SEAL = false; tool_SkillTool_FARMING	= true;	break;
	case IDD_SMALL_RADAR_MINING		:  tool_SkillTool_FARMING  = tool_SkillTool_MINING = tool_SkillTool_FISHING = tool_SkillTool_CHOPPING = tool_SkillTool_HUB = tool_SkillTool_NPC = tool_SkillTool_ITEM_SEAL = false; tool_SkillTool_MINING	= true;	break;
	case IDD_SMALL_RADAR_FISHING	:  tool_SkillTool_FARMING  = tool_SkillTool_MINING = tool_SkillTool_FISHING = tool_SkillTool_CHOPPING = tool_SkillTool_HUB = tool_SkillTool_NPC = tool_SkillTool_ITEM_SEAL = false; tool_SkillTool_FISHING	= true;	break;
	case IDD_SMALL_RADAR_CUTTING	:  tool_SkillTool_FARMING  = tool_SkillTool_MINING = tool_SkillTool_FISHING = tool_SkillTool_CHOPPING = tool_SkillTool_HUB = tool_SkillTool_NPC = tool_SkillTool_ITEM_SEAL = false; tool_SkillTool_CHOPPING= true;	break;
	case IDD_SMALL_RADAR_HUB		:  tool_SkillTool_FARMING  = tool_SkillTool_MINING = tool_SkillTool_FISHING = tool_SkillTool_CHOPPING = tool_SkillTool_HUB = tool_SkillTool_NPC = tool_SkillTool_ITEM_SEAL = false; tool_SkillTool_HUB		= true;	break;
	case IDD_SMALL_RADAR_NPC		:  tool_SkillTool_FARMING  = tool_SkillTool_MINING = tool_SkillTool_FISHING = tool_SkillTool_CHOPPING = tool_SkillTool_HUB = tool_SkillTool_NPC = tool_SkillTool_ITEM_SEAL = false; tool_SkillTool_NPC		= true;	break;
	case IDD_SMALL_RADAR_ITEM_SEAL	:  tool_SkillTool_FARMING  = tool_SkillTool_MINING = tool_SkillTool_FISHING = tool_SkillTool_CHOPPING = tool_SkillTool_HUB = tool_SkillTool_NPC = tool_SkillTool_ITEM_SEAL = false; tool_SkillTool_ITEM_SEAL= true;	break;
		
	case ID_PATHFIND_0 :	tool_Pathfind = 0; break;
	case ID_PATHFIND_1 :	tool_Pathfind = 1; break;
	case ID_PATHFIND_2 :	tool_Pathfind = 2; break;
	case ID_PATHFIND_3 :	tool_Pathfind = 3; break;
	case ID_PATHFIND_4 :	tool_Pathfind = 4; break;
	case ID_PATHFIND_5 :	tool_Pathfind = 5; break;
#endif
		
	case ID_COUNSELER_MOVE	: SendBlockMove();		break;
	case ID_COUNSELER_SKILL	: SendBlockSkill();		break;
	case ID_COUNSELER_ATTACK: SendBlockAttack();	break;
	case ID_COUNSELER_MAGIC : SendBlockMagic();		break;
	case ID_COUNSELER_ANTI_TEAM: SendBlockAntiTeam();		break;
	case ID_COUNSELER_AB_MOVE_DUNGEON_KEEPER:	g_absolute_move     = 0; g_absolute_logout = 0;   g_make_ghost	   = 0; g_give_life		= 0; g_absolute_move_dk = !g_absolute_move_dk;	break;
	case ID_COUNSELER_AB_MOVE: g_absolute_move_dk  = 0; g_absolute_logout = 0;   g_make_ghost	   = 0; g_give_life		= 0; g_absolute_move    = !g_absolute_move;		break;
	case ID_ABSOLUTE_LOGOUT	:  g_absolute_move_dk  = 0; g_make_ghost      = 0;   g_give_life	   = 0; g_absolute_move = 0; g_absolute_logout  = !g_absolute_logout;	break;
	case ID_MAKE_GHOST		:  g_absolute_move_dk  = 0; g_absolute_logout = 0;   g_give_life	   = 0; g_absolute_move = 0; g_make_ghost	    = !g_make_ghost;		break;
	case ID_GIVE_LIFE		:  g_absolute_move_dk  = 0; g_make_ghost      = 0;   g_absolute_logout = 0; g_absolute_move = 0; g_give_life	    = !g_give_life;			break;
		
		////////////////////// 0311 lkh 추가 ///////////////////////////
	case ID_PLAY_SOUND :		//Edited by Eleval 19/06/09 - Was ID_SOUND_FESTIVAL
		//SendCMD_CHARACTER_DEMAND(CCD_PLAYSOUND_EVENT1, Hero->id);
		//g_Operator_Function = OPERATOR_PLAYSOUND_EVENT1;
		ToolPlaySound();
		break;						
	case ID_SOUND_HANDCLAP :		
		//SendCMD_CHARACTER_DEMAND(CCD_PLAYSOUND_EVENT2, Hero->id);
		g_Operator_Function = OPERATOR_PLAYSOUND_EVENT2;
		break;						
	case ID_OUTPUT_CELEBRATE :		
		g_Operator_Function = OPERATOR_OUTPUT_CELEBRATE;
		break;	
	case ID_GM_ENABLE :
		ToggleSanta();
		break;
		
		////////////////////////////////////////////////////////////////
		
		////////////////////// 0405 YGI ///////////////////////////
		
	case ID_TOOL_PW :			ToolPassWord(); 
		break;
		
		// 020428 YGI acer
	case IDM_LOGOUT_NAME :  ToolLogoutCharacter(); 
		break;
	case ID_GM_MOVETOCHARACTER: MoveToCharacter(); //Thralas
		break;
	case ID_MENU_MOVE_MAP : ToolMapMove(); 
		break;
	case ID_MENU_MOVE_MAP_MULTIPLE: ToolMapMoveMultiple();
		break;
	
//soto 030307
	case ID_MOSTER_GENERATER: ToolGenerateMonster();
		break;
	case ID_LOCALWAR : ::SetLocalWarTime();
		break;
	case ID_NATIONSET: ::SetNationWarTime();
		break;
//soto Offline Work End.
		
		// acer2
	case ID_OTHER_CHAR_INFO : ToolPassWord(1); 
		break;
		
	case ID_TRIP_AROUND_ON :	SettingTripAround();
		break;
		
	case ID_TRIP_AROUND_OFF:	DeSettingTripAround();
		break;
		
	case ID_CHANGE_SKILL_EXP :	ChangeSkillExp();
		break;

	case ID_GM_BANTOOL:
		if (g_StartMenuOn) return;
		BanCharacter();
		break;
	case IDD_FORCE_START_NATION_WAR:
		//if (g_StartMenuOn) return;
		ForceStartWar();
		break;
	case IDD_GENERATEITEM:
		if (g_StartMenuOn) return;
		generateitem();
		break;
	case ID_PUBLIC_MESSAGE_IN_MAP :		PublicBroadCasting(); 
		break;
		
	case ID_PUBLIC_MESSAGE_IN_MAP_ALL : PublicBroadCastingtoAll();
		break;
		
		
	case ID_START_EVENT : StartOXEvent();
		break;
		
	case ID_TEAM_BATTLE_END   :		if( MapNumber == 16 )  // MapName : FIGHT..
									{	
										CallServer( CMD_FIGHT_MAP_END );
									}	
		else 
		{	
			AddCurrentStatusMessage( 255, 100, 100, "팀배틀용이 아니라 '팀배틀 시작/끝' 명령을 보낼수 없습니다." );
		}	
		break;
	case ID_TEAM_BATTLE_START :		if( MapNumber == 16 )  // MapName : FIGHT..
									{	
										// 계인은 여기에 추가하라...
										CallServer( CMD_FIGHT_MAP_START );
									}	
		else 
		{	
			AddCurrentStatusMessage( 255, 100, 100, "팀배틀용이 아니라 '팀배틀 시작/끝' 명령을 보낼수 없습니다." );
		}	
		break;							
		
	case IDD_MY_SKILL_EXP   :		tool_ViewMyExp = !tool_ViewMyExp;
		
		break;
	case ID_EVENT_LMSMENU:
		ToolLMSMenu();
		break;
	case ID_EVENT_EXP:
		ToolExpEventMenu();
		break;
	case ID_EVENT_PK:
		ToolPKMenu();
		
	default : Check_Kein_Menu( wParam ); break;			// 1004 YGI
		
	}			

	;
}				

//soto 030307
//여기서는 누군가 메인 메뉴의 어떤 항목을 선택했을때의 메세지를 처리한다.
void	MainMenuSelected(WPARAM wParam,LPARAM lParam)
{
	HMENU hGMMenu	= NULL; // "GM" 메뉴의 핸들이다.
	HMENU hToolMenu  = NULL;
	HMENU hOption	= NULL;

	if(SysInfo.notdead) // 일때는.. IDC_DRAGON 메뉴를 사용 한다.
	{
		hOption  = GetSubMenu((HMENU)wParam,2); // 세번째.
		hGMMenu = GetSubMenu((HMENU)wParam,6); // 7번째에 위치하고 있다.
		hToolMenu  = GetSubMenu((HMENU)wParam,5); //여섯.
		
	}
	else // IDC_GM 메뉴를 사용한다.
	{
		hGMMenu = GetSubMenu((HMENU)wParam,0); // 첫번째에 위치하고 있다.
		hOption = GetSubMenu((HMENU)wParam,1); // 두번째 위치하고 있다.
	}
	
	if(g_absolute_move)CheckMenuItem(hGMMenu,ID_COUNSELER_AB_MOVE,MF_CHECKED);
	else CheckMenuItem(hGMMenu,ID_COUNSELER_AB_MOVE,MF_UNCHECKED);

	if(g_absolute_move_dk)CheckMenuItem(hGMMenu,ID_COUNSELER_AB_MOVE_DUNGEON_KEEPER,MF_CHECKED);
	else CheckMenuItem(hGMMenu,ID_COUNSELER_AB_MOVE_DUNGEON_KEEPER,MF_UNCHECKED);

	if(g_absolute_logout)CheckMenuItem(hGMMenu,ID_ABSOLUTE_LOGOUT,MF_CHECKED);
	else CheckMenuItem(hGMMenu,ID_ABSOLUTE_LOGOUT,MF_UNCHECKED);
	
	if(g_make_ghost) CheckMenuItem(hGMMenu,ID_MAKE_GHOST,MF_CHECKED);
	else CheckMenuItem(hGMMenu,ID_MAKE_GHOST,MF_UNCHECKED);

	if(g_give_life)CheckMenuItem(hGMMenu,ID_GIVE_LIFE,MF_CHECKED);
	else CheckMenuItem(hGMMenu,ID_GIVE_LIFE,MF_UNCHECKED);

	if(tool_MultiChar)
		CheckMenuItem(hOption,ID_MULTICHAR,MF_CHECKED);
	else CheckMenuItem(hOption,ID_MULTICHAR,MF_UNCHECKED);

	if(SysInfo.gamemakemode)
		CheckMenuItem(hOption,ID_SYSINFO_GAMEMAKEMODE,MF_CHECKED);
	else
		CheckMenuItem(hOption,ID_SYSINFO_GAMEMAKEMODE,MF_UNCHECKED);

	//< soto-030326
	if(hToolMenu) // for Developer
	{
		if(SMenu[MN_EFFECT_TOOL].bActive)
		{
			CheckMenuItem(hToolMenu,ID_CHECK_EFFECT,MF_CHECKED);
		}
		else
		{
			CheckMenuItem(hToolMenu,ID_CHECK_EFFECT,MF_UNCHECKED);
		}
		
	}
	
	//> soto-030326
}
//soto Offline Work End.


#else

void SystemMenuProcess( WPARAM wParam )
{
	
}

#endif


BOOL CALLBACK AgreeDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	static char temp[ MAX_PATH];
	
	switch(Message)
	{
	case WM_INITDIALOG:		
		{
			FILE *fp;
			fp = Fopen( "agree.txt", "rt" );
			if( !fp ) 
			{
				JustMsg( " 'agree.txt' file not found!!!" );
				EndDialog(hDlg, LOWORD(IDCANCEL));
				return false;
			}
			
			char temp[1024];
			while( fgets( temp, 1024, fp ) )
			{
				if( temp[0] != ';' ) break;
			}
			EatRearWhiteChar( temp );
			Edit_SetText( GetDlgItem(hDlg, IDC_TITLE ), temp );
			
			SendMessage(GetDlgItem(hDlg, IDC_RICHEDIT_TITLE ), WM_SETTEXT, 0, (LPARAM)"");
			char temp2[8192+2048] = {0,};
			int count=0;
			while( fgets( temp, 1024, fp ) )
			{					
				if( temp[0] == ';' ) continue;
				EatRearWhiteChar( temp );
				if( count ) sprintf( temp2, "%s\n%s", temp2, temp );
				else strcpy( temp2, temp );
				Edit_SetText(GetDlgItem(hDlg, IDC_RICHEDIT_TITLE ), temp2);
				//Edit_ReplaceSel(GetDlgItem(hDlg, IDC_RICHEDIT_TITLE ), (LPARAM)temp);
				//ListBox_AddString( GetDlgItem(hDlg, IDC_LIST_TITLE ), temp );
				count++;
			}
			fclose( fp );
			return true;
		}
		
	case WM_PAINT:
		break;
		
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDOK     :	
			EndDialog(hDlg, LOWORD(wParam));
			return true;
		case IDCANCEL :	
			EndDialog(hDlg, LOWORD(wParam));
			return ( false );
		}
		break;
	}
	
	return FALSE;	
}

int AgreeStartGame( HWND hwnd, HINSTANCE hInstance )
{
	HINSTANCE  hinst;
	hinst = LoadLibrary("riched32.dll");
	int ret = DialogBox( hInstance, MAKEINTRESOURCE( IDD_TITLE_ ), NULL, (DLGPROC)AgreeDlgProc );
	FreeLibrary( hinst );
	return ret;
}


inline int GetSysInfo( int type )
{
#ifdef _DEBUG
	switch( type )
	{
	case SI_GAME_MAKE_MODE : return SysInfo.gamemakemode;
	case SI_NOT_CONNECT_MODE : return SysInfo.notconectserver;
	}
#endif
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// 강제 로그 아웃
extern void SendLogOutCharacter( char *szName );
BOOL CALLBACK 
ToolLogoutCharacterProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)		
{		
	switch(Message)		
	{					
	case WM_INITDIALOG:	break;
		
	case WM_PAINT:		SetTimer( g_hwndMain, 13, 1000, NULL);
  						break;			
						
	case WM_TIMER  :	if( wParam == 13 ) 	
						{
							;
							GameTestProc( &g_DirectDrawInfo );
						}
		break;
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{			
		case IDOK :
			{
				char lpszName[20];
				GetDlgItemText( hDlg, IDC_EDIT_TOOL_PW, lpszName, 20 );
				if( !(*lpszName) )
				{
					return false;
				}
				SendLogOutCharacter( lpszName );
			}
		case IDCANCEL :	
			{
				EndDialog(hDlg, LOWORD(wParam));
				SetFocus2(HWND_GAME_MAIN);//021001 lsw
				;
				return (TRUE);
			}break;
		default : return true;
		}
		break;
	}
	return false;
}

void ToolLogoutCharacter( )
{
	if( g_StartMenuOn ) return;
	
#ifdef _DEBUG
	HWND hDlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_INPUT_PASSWORD ), g_hwndMain, (DLGPROC)ToolLogoutCharacterProc );
	ShowWindow(hDlg, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlg, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hDlg, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow(hDlg, SW_SHOW);
	SetWindowText( hDlg, "Input Name" );
	
	SetFocus( hDlg );//021001 lsw
#endif
}

BOOL CALLBACK MoveToCharacterProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam) //Eleval 19/08/09
{
#ifdef _DEBUG
	switch(Message)		
	{					
	case WM_INITDIALOG:	break;
		
	case WM_PAINT:		SetTimer( g_hwndMain, 20, 1000, NULL);
  						break;			
						
	case WM_TIMER  :	if( wParam == 20 ) 	
						{
							;
							GameTestProc( &g_DirectDrawInfo );
						}
		break;
		
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
		{			
		case IDOK :{
			;
			char lpszPW[21] = {0,};
			GetDlgItemText( hDlg, IDC_EDIT_TOOL_PW, lpszPW, 20 );
			if( !(*lpszPW) )
			{
				return false;
			}
			CallServer( CMD_TELEPORT_OTHER_CHAR, lpszPW, strlen(lpszPW)  );
				   }
		case IDCANCEL :	
			{
				EndDialog(hDlg, LOWORD(wParam));
				SetFocus2(HWND_GAME_MAIN);//021001 lsw
				;
				return (TRUE);
			}break;
		default : return true;
		}
		break;
	}
	
#endif
	return false;
}

void MoveToCharacter()
{
#ifdef _DEBUG
	if(g_StartMenuOn) return;
	HWND hDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_INPUT_PASSWORD), g_hwndMain, MoveToCharacterProc);
	ShowWindow(hDlg, SW_SHOW);
	SetWindowText(hDlg, "Enter character name");
	SetFocus(hDlg);
#endif
}

#ifdef _DEBUG
#define ERT_ANSER_O	100
#define ERT_ANSER_X	200

BOOL CALLBACK OXEventProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	static int check_ox = ERT_ANSER_O;
	char temp[MAX_PATH];
	
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			CheckRadioButton( hDlg, IDC_RADIO_X, IDC_RADIO_O, IDC_RADIO_O );
			SetTimer( g_hwndMain, 14, 1000, NULL);
			return TRUE;
		}					
		
	case WM_PAINT:		
		
		//GetWindowRect( GetDlgItem(hDlg, IDC_OPEN_MAP), &rect[0] );
		//GetWindowRect( GetDlgItem(hDlg, IDC_END_EVENT ), &rect[1] );
		//GetWindowRect( GetDlgItem(hDlg, IDC_START_EVENT ), &rect[2] );
		//GetWindowRect( GetDlgItem(hDlg, IDC_SEND_QUESTION ), &rect[3] );
		//GetWindowRect( GetDlgItem(hDlg, IDC_EXPLAIN ), rect[0] );
		//GetWindowRect( GetDlgItem(hDlg, IDC_EXPLAIN ), rect[0] );
		
		break;			
		
	case WM_TIMER:
		if( wParam == 14 )
		{
			;
			GameTestProc( &g_DirectDrawInfo );
		}
		break;
		/*
		case WM_NCHITTEST :
		{
		//GetRect(
		POINT point;
		point.x= GET_X_LPARAM(lParam);
		point.y= GET_Y_LPARAM(lParam);
		//ScreenToClient( hDlg, &point );
		
		  for( int i=0; i<4; i++ )
		  {
		  if( RectAndDotCrash( &rect[i], point.x, point.y) )
		  {
		  crash = i;
		  break;
		  }
		  }
		  
			if( crash >= 0 )
			{
			sprintf( temp, "%d", i );
			Edit_SetText(GetDlgItem(hDlg, IDC_EXPLAIN ), temp );
			}
			break;
	}*/
		
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{				
		case IDOK :	
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
			
		case IDC_RADIO_O :
			{
				check_ox = ERT_ANSER_O;
				break;
			}
		case IDC_RADIO_X :
			{
				check_ox = ERT_ANSER_X;
				break;
			}
		case IDC_OPEN_MAP : 
			{
				SendOXEventAboutMap( 1 );	// 참여 유도
				break;
			}
		case IDC_END_EVENT :
			{
				SendOXEventAboutMap( 3 );	// 종료
				break;
			}
		case IDC_START_EVENT :
			{
				SendOXEventAboutMap( 2 );	// 종료
				break;
			}
		case IDC_CLOSE_MAP	:
			{
				SendOXEventAboutMap( 4 );	// 종료
				break;
			}
		case IDC_SEND_QUESTION:
			{
				Edit_GetText( GetDlgItem(hDlg, IDC_QUESTION), temp, MAX_PATH );
				SendOXQuestion( temp );
				break;
			}
		case IDC_ANSER :
			{
				switch( check_ox )
				{
				case ERT_ANSER_O :	sprintf( temp, kein_GetMenuString( 120 ),"X"  );
					break;
				case ERT_ANSER_X :	sprintf( temp, kein_GetMenuString( 120 ),"O" );
					break;
				}
				if( MessageBox(NULL,temp,0,MB_OKCANCEL) == IDOK )
				{
					char type = check_ox;
					CallServer( CMD_OX_ANSER, &type, 1 );
					k_set_time set_time;
					set_time.hour = 0;
					set_time.minute = 0;
					CallServer( CMD_OX_SET_TIME, &set_time, sizeof( k_set_time ) );
				}
				break;
			}
		case IDC_SET_TIME :
			{
				char hour, minute;
				Edit_GetText( GetDlgItem(hDlg, IDC_TIME_HOUR), temp, MAX_PATH );
				hour = atoi( temp );
				Edit_GetText( GetDlgItem(hDlg, IDC_TIME_MINUTE), temp, MAX_PATH );
				minute = atoi( temp );
				if( hour || minute )
				{
					k_set_time set_time;
					set_time.hour = hour;
					set_time.minute = minute;
					CallServer( CMD_OX_SET_TIME, &set_time, sizeof( k_set_time ) );
				}
			}
			
		default : return true;
		}
		break;
	}
	return false;
}
#endif

void StartOXEvent( )
{
#ifdef _DEBUG
	
	HWND hOXEvent= CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_EVENT_TOOL ), g_hwndMain, (DLGPROC)OXEventProc );
	ShowWindow( hOXEvent, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hOXEvent, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hOXEvent, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow( hOXEvent, SW_SHOW);
#endif
}




// 020428 YGI acer
#ifdef _DEBUG
BOOL CALLBACK ToolMapMoveProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char temp[MAX_PATH];
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			SetTimer( g_hwndMain, 15, 1000, NULL);
			return TRUE;
		}					
		
	case WM_PAINT:
		break;			
		
	case WM_TIMER:
		if( wParam == 15 )
		{
			;
			GameTestProc( &g_DirectDrawInfo );
		}
		break;
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{
		case IDC_BARANTAN:
			{
				t_packet packet;
				strcpy( temp, "barantan");
				packet.u.kein.direct_map_move.map_number = ConvMapnameToMapnumber(temp);
				packet.u.kein.direct_map_move.x = 327;
				packet.u.kein.direct_map_move.y = 114;
				strcpy(packet.u.kein.direct_map_move.name, "");
				packet.h.header.type = CMD_DIRECT_MAP_MOVE;
				packet.h.header.size = sizeof(k_direct_map_move) - 20;// + strlen(temp);
				QueuePacket(&packet, 1);

				EndDialog(hDlg, LOWORD(wParam));
				return true;
			}
			break;
		case IDC_RENUS:
			{
				t_packet packet;
				strcpy( temp, "renes_c");
				packet.u.kein.direct_map_move.map_number = ConvMapnameToMapnumber(temp);
				packet.u.kein.direct_map_move.x = 207;
				packet.u.kein.direct_map_move.y = 175;
				strcpy(packet.u.kein.direct_map_move.name, "");
				packet.h.header.type = CMD_DIRECT_MAP_MOVE;
				packet.h.header.size = sizeof(k_direct_map_move) - 20;// + strlen(temp);
				QueuePacket(&packet, 1);

				EndDialog(hDlg, LOWORD(wParam));
				return true;
			}
			break;
		case IDC_HELTERNT:
			{
				t_packet packet;
				strcpy( temp, "ma-in");
				packet.u.kein.direct_map_move.map_number = ConvMapnameToMapnumber(temp);
				packet.u.kein.direct_map_move.x = 238;
				packet.u.kein.direct_map_move.y = 120;
				strcpy(packet.u.kein.direct_map_move.name, "");
				packet.h.header.type = CMD_DIRECT_MAP_MOVE;
				packet.h.header.size = sizeof(k_direct_map_move) - 20;// + strlen(temp);
				QueuePacket(&packet, 1);

				EndDialog(hDlg, LOWORD(wParam));
				return true;
			}
			break;
		case IDOK :	// map move
			{
				t_packet packet;
				
				Edit_GetText( GetDlgItem(hDlg, IDC_MAP_NAME), temp, MAX_PATH );
				EatRearWhiteChar( temp );
				if( !temp[0] ) strcpy( temp, MapInfo[ MapNumber].mapfile );	// 020705 YGI
				int map_number = ConvMapnameToMapnumber( temp );
				if( map_number == -1 )
				{
					JustMsg( "Unknown mapname!" );
					return true;
				}
				packet.u.kein.direct_map_move.map_number = map_number;
				
				Edit_GetText( GetDlgItem(hDlg, IDC_MAP_X), temp, MAX_PATH );
				packet.u.kein.direct_map_move.x = atoi( temp );
				
				Edit_GetText( GetDlgItem(hDlg, IDC_MAP_Y), temp, MAX_PATH );
				packet.u.kein.direct_map_move.y = atoi( temp );
				
				Edit_GetText( GetDlgItem(hDlg, IDC_TARGET_NAME), temp, MAX_PATH );
				EatRearWhiteChar( temp );
				strcpy( packet.u.kein.direct_map_move.name, temp );
				
				packet.h.header.type = CMD_DIRECT_MAP_MOVE;
				packet.h.header.size = sizeof( k_direct_map_move ) - 20 + strlen( temp );
				QueuePacket( &packet, 1 );
			}
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
			
		default : return true;
		}
		break;
	}
	return false;
}

//extern CPrison g_cPrison;
//030224 soto
bool IsNPC(LPCHARACTER pCharac)
{
	return (pCharac->sprno > 1); // 0,1이 아닌 스프라이트 넘버를 가졌으면 NPC이다.
}

//030307 soto For Mutiple Users Move Dialog.
BOOL CALLBACK ToolMapMoveMultipleProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char temp[MAX_PATH];
	
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			HWND	g_hMultiDlg = hDlg;

			HWND hListWnd = GetDlgItem(hDlg,LST_UNSELECTED);
						
			LPCHARACTER		pChar = g_CharacterList.lpFirst;
			while( pChar != NULL )
			{
			
				
				if(!IsNPC(pChar) && g_CharacterList.lpFirst != pChar)		//자기자신은 마지막에 들어간다.
				SendMessage(hListWnd,LB_ADDSTRING,0,(LPARAM)pChar->name);	//왜 냐하면..이동될때 자신은 마지막에 이동되어야.
																			//유저들이 바르게 큰트롤 될수 있기 때문이다.
				pChar = pChar->lpNext;
			}
			
			SendMessage(hListWnd,LB_ADDSTRING,0,(LPARAM)g_CharacterList.lpFirst->name);
			


			int nCount = SendMessage(hListWnd,LB_GETCOUNT,0,0);
			if(0 < nCount)
			{
				SendMessage(hListWnd,LB_SETCURSEL,0,0);
			}


			HWND hMapNameWnd = GetDlgItem(hDlg,CBO_MAPNAME);

			int index = 0;
			for(int i = 0;i < MAX_MAP_;i++)
			{
				if( 0 < strlen(MapInfo[i].mapname) )
				{	
					//< soto-030320
					char szTemp[MAX_PATH] = {0,};
					sprintf(szTemp,"%s [%s]",MapInfo[i].mapname,MapInfo[i].mapfile);
					index = SendMessage(hMapNameWnd,CB_ADDSTRING,0,(LPARAM)szTemp);
					SendMessage(hMapNameWnd,CB_SETITEMDATA,index,(LPARAM)MapInfo[i].mapfile);
				}	//> soto-030320
			}
			SendMessage(hMapNameWnd,CB_SETCURSEL,0,0);



			return TRUE;
		}
	case WM_PAINT:
		{

		}
		break;
//	case WM_ACTIVATE:
//		{
//			return TRUE;
//		}		
	case WM_TIMER:
		{
		}
		break;
	case WM_COMMAND:
		{
			switch( LOWORD( wParam )) 
			{				
				case BTN_MOVE :	// map move
				{


					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);
					int nCount = SendMessage(hLeftListWnd,LB_GETCOUNT,0,0);
					
					if(0 < nCount)
					{
						HWND hChkYllseWnd = GetDlgItem(hDlg,CHK_YLLSE);
						HWND hChkZypernWnd = GetDlgItem(hDlg,CHK_ZYPERN);
						HWND hChkViseusWnd = GetDlgItem(hDlg,CHK_VISEUS);
						bool bYllse		= (SendMessage(hChkYllseWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bZypern	= (SendMessage(hChkZypernWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bViseus	= (SendMessage(hChkViseusWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						// 속한나라	0 : 없음, 1, 2, 3: 바이서스 4: 자이펀 5:헤게모니아 6: 일스


						t_packet packet;
						
						HWND hMapNameWnd = GetDlgItem(hDlg,CBO_MAPNAME); // 콤보 박스의  핸들이다.
						SendMessage(hMapNameWnd,WM_GETTEXT,MAX_PATH,(LPARAM)temp); // 현재 쓰여져 있는 텍스트를 읽어 온다.
						EatRearWhiteChar( temp );
						
						int nCBIndex = SendMessage(hMapNameWnd,CB_FINDSTRING,0,(LPARAM)temp);
						
						if(nCBIndex == CB_ERR)
						{
							if( 0 >= strlen(temp) )
								strcpy( temp, MapInfo[ MapNumber].mapfile );	// 020705 YGI
							else
							{
								JustMsg( "Error! Wrong map name1" );
								return true;
							}

						}
						else
						{
							strcpy(temp,(LPCSTR)SendMessage(hMapNameWnd,CB_GETITEMDATA,nCBIndex,0));
							SendMessage(hMapNameWnd,CB_SETCURSEL,nCBIndex,0);
						}

						int map_number = ConvMapnameToMapnumber( temp );
						if( map_number == -1 )
						{
							JustMsg( "Error! Wrong map name" );
							return true;
						}
						packet.u.kein.direct_map_move.map_number = map_number;

						Edit_GetText( GetDlgItem(hDlg, EDT_XPOS), temp, MAX_PATH );
						packet.u.kein.direct_map_move.x = atoi( temp );

						Edit_GetText( GetDlgItem(hDlg, EDT_YPOS), temp, MAX_PATH );
						packet.u.kein.direct_map_move.y = atoi( temp );


						for(int nIndex = 0;nIndex < nCount;nIndex++)
						{
							
							SendMessage(hLeftListWnd,LB_GETTEXT,(WPARAM)nIndex,(LPARAM)temp);
							EatRearWhiteChar( temp );
							strcpy( packet.u.kein.direct_map_move.name, temp );

							LPCHARACTER	pChar = ExistHe(temp);
							if(pChar)
							{
								//체크 되어 있는 사람 들만 각 국가에 해당 되는 사람들만.
								if( (pChar->name_status.nation == 6 && bYllse) || 
									(pChar->name_status.nation == 4 && bZypern) ||
									(pChar->name_status.nation == 3 && bViseus) )
								{
									packet.h.header.type = CMD_DIRECT_MAP_MOVE;
									packet.h.header.size = sizeof( k_direct_map_move ) - 20 + strlen( temp );
									QueuePacket( &packet, 1 );

//									Sleep(200);
								}

							
							}

							
						}
					}				
				}
				break;
				case BTN_DEL_USER:// 가능 리스트에 있는 이름들을 불가능 리스트로 옮긴다.
				{
					char szID[600] = {0,};
					
					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);
					HWND hRightListWnd = GetDlgItem(hDlg,LST_UNSELECTED);

					int nIndex = SendMessage(hLeftListWnd,LB_GETCURSEL,0,0);
					
					if(nIndex != LB_ERR)
					SendMessage(hLeftListWnd,LB_GETTEXT,(WPARAM)nIndex,(LPARAM)szID);

					if(strlen(szID))
					{
						nIndex = SendMessage(hLeftListWnd,LB_DELETESTRING,(WPARAM)nIndex,0);
						
						if(nIndex != LB_ERR)
						{
							SendMessage(hLeftListWnd,LB_SETCURSEL,(WPARAM)nIndex,0);
						}

						
						LPCHARACTER	pChar = ExistHe(szID);
						if(pChar)
						{
							if(pChar == Hero)//자기자신을 가리키면.
								nIndex = SendMessage(hRightListWnd,LB_INSERTSTRING,-1,(LPARAM)szID); // 맨 마지막에 넣는다.
							else 
								nIndex = SendMessage(hRightListWnd,LB_INSERTSTRING,0,(LPARAM)szID); // 일반 유저이면 처음에 삽입.
							
							SendMessage(hRightListWnd,LB_SETCURSEL,(WPARAM)nIndex,0);
							
						}
						else
						{
							SendMessage(hRightListWnd,LB_SETCURSEL,(WPARAM)0,0);							
						}
					

						
					}
				}
				break;
				case BTN_ENABLE_USER:
				{
					char szID[600] = {0,};
					
					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);
					HWND hRightListWnd = GetDlgItem(hDlg,LST_UNSELECTED);

					int nIndex = SendMessage(hRightListWnd,LB_GETCURSEL,0,0);
					
					if(nIndex != LB_ERR)
					SendMessage(hRightListWnd,LB_GETTEXT,(WPARAM)nIndex,(LPARAM)szID);

					if(strlen(szID))
					{
						nIndex = SendMessage(hRightListWnd,LB_DELETESTRING,(WPARAM)nIndex,0);
						
						if(nIndex != LB_ERR)
						{
							SendMessage(hRightListWnd,LB_SETCURSEL,(WPARAM)nIndex,0);
						}

						
						LPCHARACTER	pChar = ExistHe(szID);
						if(pChar)
						{
							if(pChar == Hero)//자기자신을 가리키면.
								nIndex = SendMessage(hLeftListWnd,LB_INSERTSTRING,-1,(LPARAM)szID); // 맨 마지막에 넣는다.
							else 
								nIndex = SendMessage(hLeftListWnd,LB_INSERTSTRING,0,(LPARAM)szID); // 일반 유저이면 처음에 삽입.
							
							SendMessage(hLeftListWnd,LB_SETCURSEL,(WPARAM)nIndex,0);
						}
						else
						{
							SendMessage(hLeftListWnd,LB_SETCURSEL,(WPARAM)0,0);
						}
						
						
					}					
				}
				break;
				case BTN_DEL_USER_ALL:
				{
					char szID[600] = {0,};

					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);
					HWND hRightListWnd = GetDlgItem(hDlg,LST_UNSELECTED);

					int nCount = SendMessage(hLeftListWnd,LB_GETCOUNT,0,0);

					if(0 < nCount)
					{
						for(int nIndex = 0;nIndex < nCount;nIndex++)
						{
							SendMessage(hLeftListWnd,LB_GETTEXT,nIndex,(LPARAM)szID);
							SendMessage(hRightListWnd,LB_ADDSTRING,0,(LPARAM)szID);
						}

						SendMessage(hLeftListWnd,LB_RESETCONTENT,0,0);
					}
				}
				break;
				case BTN_ENABLE_USER_ALL:
				{
					char szID[600] = {0,};

					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);
					HWND hRightListWnd = GetDlgItem(hDlg,LST_UNSELECTED);

					int nCount = SendMessage(hRightListWnd,LB_GETCOUNT,0,0);

					if(0 < nCount)
					{
						for(int nIndex = 0;nIndex < nCount;nIndex++)
						{
							SendMessage(hRightListWnd,LB_GETTEXT,nIndex,(LPARAM)szID);
							SendMessage(hLeftListWnd,LB_ADDSTRING,0,(LPARAM)szID);
						}

						SendMessage(hRightListWnd,LB_RESETCONTENT,0,0);
					}
				}
				break;

				case BTN_IMPRISON:
				{
					HWND hLeftListWnd = GetDlgItem(hDlg, LST_SELECTED_USERS);
					int nCount = SendMessage(hLeftListWnd, LB_GETCOUNT, 0, 0);
					if (0 < nCount)
					{
						char szTemp[MAX_PATH] = { 0, };
						char szName[MAX_PATH] = { 0, };
						int nIndex;
						for (nIndex = 0; nIndex < nCount; ++nIndex)
						{

							SendMessage(hLeftListWnd, LB_GETTEXT, (WPARAM)nIndex, (LPARAM)szName);
							::EatRearWhiteChar(szName);

							//< LTH-040723-KO 감옥에 유저 넣기 패킷전송
							sprintf(szTemp, lan->OutputMessage(0, 650), szName);		// "'%s'를 감옥으로 이동합니까?"
							if (::YesOrNo(szTemp, lan->OutputMessage(0, 639)) == IDYES)	// "운영자 모드" 메시지 박스.
							{
								g_cPrison.BadUserImprison(szName);
							}
							//> LTH-040723-KO
						}
					}
					
				}
				break;
				case BTN_RELEASE:
				{

					HWND hLeftListWnd = GetDlgItem(hDlg, LST_SELECTED_USERS);
					int nCount = SendMessage(hLeftListWnd, LB_GETCOUNT, 0, 0);

					if (0 < nCount)
					{
						char szTemp[MAX_PATH] = { 0, };
						char szName[MAX_PATH] = { 0, };
						int nIndex;
						for (nIndex = 0; nIndex < nCount; ++nIndex)
						{

							SendMessage(hLeftListWnd, LB_GETTEXT, (WPARAM)nIndex, (LPARAM)szName);
							::EatRearWhiteChar(szName);

							//< LTH-040723-KO 감옥에 유저 빼내기 패킷전송
							sprintf(szTemp, lan->OutputMessage(0, 651), szName);		// "'%s'를 출소 시킵니까?"
							if (::YesOrNo(szTemp, lan->OutputMessage(0, 640)) == IDYES)	// "운영자 모드" 메시지 박스.
							{
								g_cPrison.BadUserRelease(szName);
							}
							//> LTH-040723-KO
						}
					}

				}
				break;
				case IDC_TU_OFF:
				{
					char szTemp_user[MAX_PATH] = { 0, };
					Edit_GetText(GetDlgItem(hDlg, EDT_TU_NAME), szTemp_user, MAX_PATH);
					if (0 == strlen(szTemp_user))
					{
						return true;	//입력된 글이 없다면 skip
					}
					CString str;
					if (!Hero->IsCounselor())
					{
						
						str.Format(_T("can not use because only allow GameOperator"));
						g_DebugWnd_TU.PutDebugMsg(str);
						return true;
						
					}

					//051227_kch trace user log를 보여준다.
					str.Format(_T("Trace User (%s) OFF"), szTemp_user);
					g_DebugWnd_TU.PutDebugMsg(str);

					t_packet _Packet;
					_Packet.h.header.type = CMD_USER_TRACE_C_TO_MAP_REG_OFF_REQ;
					_Packet.h.header.size = sizeof(t_char_in_game);

					strncpy(_Packet.u.char_in_game.name, szTemp_user, sizeof(_Packet.u.char_in_game.name));
					QueuePacket(&_Packet, 1);

					//이동처리후, 메인화면을 활성화
					
					break;
				}

				case IDC_TU_ON:
				{
				

					char szTemp_user[MAX_PATH] = { 0, };
					Edit_GetText(GetDlgItem(hDlg, EDT_TU_NAME), szTemp_user, MAX_PATH);

					if (0 == strlen(szTemp_user))
					{
						return true;		//입력된 글이 없다면 skip
					}

					CString str;
					if (!Hero->IsCounselor())
					{	//운영자가 아닐경우는 사용할수 없다.
						str.Format(_T("can not use because only allow GameOperator"));
						g_DebugWnd_TU.PutDebugMsg(str);
						return true;
					}

					str.Format(_T("Trace User (%s) ON"), szTemp_user);
					g_DebugWnd_TU.PutDebugMsg(str);

					t_packet _Packet;
					_Packet.h.header.type = CMD_USER_TRACE_C_TO_MAP_REG_ON_REQ;
					_Packet.h.header.size = sizeof(t_char_in_game);

					strncpy(_Packet.u.char_in_game.name, szTemp_user, sizeof(_Packet.u.char_in_game.name));
					QueuePacket(&_Packet, 1);

					//이동처리후, 메인화면을 활성화
					
				}
				break;
				case IDC_TU_LOG_CLEAR:
				{
					HWND hTU_LogWnd = GetDlgItem(hDlg, IDC_TU_LOG);
					SendMessage(hTU_LogWnd, CB_RESETCONTENT, 0, 0);

					g_DebugWnd_TU.ReleaseData();
				}
				break;
				case BTN_LOGOUT:
				{
					t_packet p;
					LPCHARACTER	pChar = NULL;
					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);

					int nCount = SendMessage(hLeftListWnd,LB_GETCOUNT,0,0);				
										

					if(0 < nCount)
					{
						HWND hChkYllseWnd = GetDlgItem(hDlg,CHK_YLLSE);
						HWND hChkZypernWnd = GetDlgItem(hDlg,CHK_ZYPERN);
						HWND hChkViseusWnd = GetDlgItem(hDlg,CHK_VISEUS);
						
						bool bYllse		= (SendMessage(hChkYllseWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bZypern	= (SendMessage(hChkZypernWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bViseus	= (SendMessage(hChkViseusWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						// 속한나라	0 : 없음, 1, 2, 3: 바이서스 4: 자이펀 5:헤게모니아 6: 일스

						char szID[600] = {0,};

						for(int nIndex = 0;nIndex < nCount;nIndex++)
						{
							SendMessage(hLeftListWnd,LB_GETTEXT,nIndex,(LPARAM)szID);
							pChar = ExistHe(szID);

							if(pChar)
							{
								//체크 되어 있는 사람 들만 각 국가에 해당 되는 사람들만.
								if( (pChar->name_status.nation == 6 && bYllse) || 
									(pChar->name_status.nation == 4 && bZypern) ||
									(pChar->name_status.nation == 3 && bViseus) )
								{
														
									p.h.header.type = CMD_ABSOLUTE_LOGOUT;
									p.u.absolute_logout.id = pChar->id;
									p.h.header.size = sizeof( t_absolute_logout );
									QueuePacket( &p, 1 );
								}
							}
							
							SendMessage(hLeftListWnd,LB_DELETESTRING,nIndex,0);
							nIndex--;nCount--;
						}
					}
				}
				break;
				case BTN_RESCURE:
				{
					t_packet p;
					LPCHARACTER	pChar = NULL;
					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);

					int nCount = SendMessage(hLeftListWnd,LB_GETCOUNT,0,0);
					if(0 < nCount)
					{
						HWND hChkYllseWnd = GetDlgItem(hDlg,CHK_YLLSE);
						HWND hChkZypernWnd = GetDlgItem(hDlg,CHK_ZYPERN);
						HWND hChkViseusWnd = GetDlgItem(hDlg,CHK_VISEUS);
						
						bool bYllse		= (SendMessage(hChkYllseWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bZypern	= (SendMessage(hChkZypernWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bViseus	= (SendMessage(hChkViseusWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						// 속한나라	0 : 없음, 1, 2, 3: 바이서스 4: 자이펀 5:헤게모니아 6: 일스

						char szID[600] = {0,};

						for(int nIndex = 0;nIndex < nCount;nIndex++)
						{
							SendMessage(hLeftListWnd,LB_GETTEXT,nIndex,(LPARAM)szID);
							pChar = ExistHe(szID);

							if(pChar)
							{
								//체크 되어 있는 사람 들만 각 국가에 해당 되는 사람들만.
								if( (pChar->name_status.nation == 6 && bYllse) || 
									(pChar->name_status.nation == 4 && bZypern) ||
									(pChar->name_status.nation == 3 && bViseus) )
								{
									p.h.header.type = CMD_GIVE_LIFE;
									p.u.make_ghost.id = pChar->id;
									p.h.header.size = sizeof( t_give_life );
									QueuePacket( &p, 1 );
								}
							}							
						}
					}					
				}
				break;
				case BTN_MAKE_GHOST:
				{
					t_packet p;
					LPCHARACTER	pChar = NULL;
					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);

					int nCount = SendMessage(hLeftListWnd,LB_GETCOUNT,0,0);
					if(0 < nCount)
					{
						HWND hChkYllseWnd = GetDlgItem(hDlg,CHK_YLLSE);
						HWND hChkZypernWnd = GetDlgItem(hDlg,CHK_ZYPERN);
						HWND hChkViseusWnd = GetDlgItem(hDlg,CHK_VISEUS);
						
						bool bYllse		= (SendMessage(hChkYllseWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bZypern	= (SendMessage(hChkZypernWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bViseus	= (SendMessage(hChkViseusWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						// 속한나라	0 : 없음, 1, 2, 3: 바이서스 4: 자이펀 5:헤게모니아 6: 일스

						char szID[600] = {0,};

						for(int nIndex = 0;nIndex < nCount;nIndex++)
						{
							SendMessage(hLeftListWnd,LB_GETTEXT,nIndex,(LPARAM)szID);
							pChar = ExistHe(szID);

							if(pChar)
							{
								//체크 되어 있는 사람 들만 각 국가에 해당 되는 사람들만.
								if( (pChar->name_status.nation == 6 && bYllse) || 
									(pChar->name_status.nation == 4 && bZypern) ||
									(pChar->name_status.nation == 3 && bViseus) )
								{
									p.h.header.type = CMD_MAKE_GHOST;
									p.u.make_ghost.id = pChar->id;
									p.h.header.size = sizeof( t_make_ghost );
									QueuePacket( &p, 1 );
									//acer7
									char *type = "GIVE_LIFE";
									CallServer( CMD_USE_GM_TOOL, type, strlen( type ) );
								}
							}
						}
					}					
				}
				break;
				case BTN_REFRESH:
				{
					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);
					HWND hRightListWnd = GetDlgItem(hDlg,LST_UNSELECTED);

					SendMessage(hLeftListWnd,LB_RESETCONTENT,0,0);
					SendMessage(hRightListWnd,LB_RESETCONTENT,0,0);
					
					LPCHARACTER		pChar = g_CharacterList.lpFirst;


					while( pChar != NULL )
					{
						if(!IsNPC(pChar) && g_CharacterList.lpFirst != pChar)		//자기자신은 마지막에 들어간다.
						SendMessage(hRightListWnd,LB_ADDSTRING,0,(LPARAM)pChar->name);
						
						pChar = pChar->lpNext;
					}

					SendMessage(hRightListWnd,LB_ADDSTRING,0,(LPARAM)g_CharacterList.lpFirst->name);


					int nCount = SendMessage(hLeftListWnd,LB_GETCOUNT,0,0);
			
					if(0 < nCount)
					{
						SendMessage(hLeftListWnd,LB_SETCURSEL,0,0);
					}
					return TRUE;					
				}
				break;

				case BTN_LISTSAVE: // 선택된 리스트를 "MutiControl.txt" 에 저장한다.
				{
					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);
					int count = SendMessage(hLeftListWnd,LB_GETCOUNT,0,0);

					FILE* fp = NULL;
					fp = fopen("MutiControl.txt","w+t");
					
					if(fp)
					{
						for(int i = 0;i < count;i++)
						{
							SendMessage(hLeftListWnd,LB_GETTEXT,i,(LPARAM)temp);
							fprintf(fp,"%s\n",temp);
						}

						fclose(fp);
					}
				}
				break;
				case IDC_REMOVENK:
				{
					t_packet packet;

					LPCHARACTER	pChar = NULL;
					HWND hLeftListWnd = GetDlgItem(hDlg,LST_SELECTED_USERS);

					int nCount = SendMessage(hLeftListWnd,LB_GETCOUNT,0,0);
					if(0 < nCount)
					{
						HWND hChkYllseWnd = GetDlgItem(hDlg,CHK_YLLSE);
						HWND hChkZypernWnd = GetDlgItem(hDlg,CHK_ZYPERN);
						HWND hChkViseusWnd = GetDlgItem(hDlg,CHK_VISEUS);
						
						bool bYllse		= (SendMessage(hChkYllseWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bZypern	= (SendMessage(hChkZypernWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						bool bVyseus	= (SendMessage(hChkViseusWnd,BM_GETCHECK,0,0) == BST_CHECKED);
						// 속한나라	0 : 없음, 1, 2, 3: 바이서스 4: 자이펀 5:헤게모니아 6: 일스
						
						if(!bYllse && !bZypern && !bVyseus)
						{
							bYllse = true;
							bZypern = true;
							bVyseus = true;
						}

						char szID[600] = {0,};

						for(int nIndex = 0;nIndex < nCount;nIndex++)
						{
							SendMessage(hLeftListWnd,LB_GETTEXT,nIndex,(LPARAM)szID);
							pChar = ExistHe(szID);

							if(pChar)
							{
								//체크 되어 있는 사람 들만 각 국가에 해당 되는 사람들만.
								if( (pChar->name_status.nation == 6 && bYllse) || 
									(pChar->name_status.nation == 4 && bZypern) ||
									(pChar->name_status.nation == 3 && bVyseus) )
								{
									packet.u.remove_nk.id = pChar->id;
									packet.h.header.type = CMD_REMOVE_NK;
									packet.h.header.size = sizeof(t_remove_nk);
									QueuePacket(&packet, 1);
								}
							}
						}
					}					
				}
				break;
				case IDCANCEL :
				{
					EndDialog(hDlg, LOWORD(wParam));
					return (TRUE);
				}

				default : 
					{
						//return true;
					}
			}
		}
		break;
	}

//	return DefWindowProc(hDlg,Message,wParam,lParam);
	return false;
//	return true;
}
//030307 soto End.

// soto 030307

BOOL CALLBACK ToolGenerateMonsterProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char temp[600] = {0,};
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			HWND hNameWnd = GetDlgItem(hDlg,CBO_NAME);
			//HWND hSprNoWnd = GetDlgItem(hDlg,EDT_SPRNO);
			//HWND hAmountWnd = GetDlgItem(hDlg, IDC_AMOUNT);
			//HWND hPosXWnd = GetDlgItem(hDlg, IDC_POSX);
			//HWND hPosYWnd = GetDlgItem(hDlg, IDC_POSY);

			for (int i = 0; i < MAX_CHARACTER_SPRITE_; ++i)
			{
				if(strlen(g_infNpc[i].szName) > 0) SendMessage(hNameWnd,CB_ADDSTRING,0,(LPARAM)g_infNpc[i].szName);
			}

			
			SendMessage(hNameWnd,CB_SETCURSEL,0,0);
			//SendMessage(hSprNoWnd,WM_SETTEXT,0,(LPARAM)"1");
			SetDlgItemInt(hDlg, IDC_AMOUNT, 1, false);
			SetDlgItemInt(hDlg, IDC_SPRITENO, 0, false);
			SetDlgItemInt(hDlg, IDC_POSX, (Hero->x - 16) / TILE_SIZE, false);
			SetDlgItemInt(hDlg, IDC_POSY, (Hero->y - 16) / TILE_SIZE, false);
			//CheckDlgButton(hDlg, IDC_RADIO1, BST_CHECKED);//Eleval 1/06/09
			//CheckDlgButton(hDlg, IDC_SPREAD, BST_CHECKED);
			//CheckDlgButton(hDlg, IDC_SUMMON, BST_CHECKED);

			return TRUE;
		}
	case WM_COMMAND:
		{
			switch( LOWORD( wParam )) 
			{
			/*
			case EN_CHANGE:
				{
					if(lParam == EDT_SPRNO)
					{
						HWND hNameWnd = GetDlgItem(hDlg,CBO_NAME);
						HWND hSprNoWnd = GetDlgItem(hDlg,EDT_SPRNO);
						SendMessage(hSprNoWnd,WM_GETTEXT,0,(LPARAM)temp);
						int sprno = atoi(temp);
						if(sprno < 1 || sprno > 101)
						{
							return false;
						}
						sprno--;
						SendMessage(hNameWnd,CB_SETCURSEL,sprno,0);
						
					}
				}
				break;
			*/
			case IDC_CURPOS:
				{
					SetDlgItemInt(hDlg, IDC_POSX, (Hero->x - 16) / TILE_SIZE, false);
					SetDlgItemInt(hDlg, IDC_POSY, (Hero->y - 16) / TILE_SIZE, false);
				}
				break;
			case IDC_KILLALL:
				{
					CallServer(CMD_SUMMON_KILLALL);
				}
				break;
			case IDOK:
				{
					int amount = GetDlgItemInt(hDlg, IDC_AMOUNT, NULL, false);
					if(amount == 0) amount = 1;
					int loot = GetDlgItemInt(hDlg, IDC_AMOUNT2, NULL, false);
					int lootAmount = GetDlgItemInt(hDlg, IDC_AMOUNT3, NULL, false);
					BOOL bX, bY;
					int x = GetDlgItemInt(hDlg, IDC_POSX, &bX, false);
					int y = GetDlgItemInt(hDlg, IDC_POSY, &bY, false);
					if(bX == false || bY == false)
					{
						x = (Hero->x - 16) / TILE_SIZE;
						y = (Hero->y - 16) / TILE_SIZE;
					}
					
					
					HWND hMonsterlist = GetDlgItem(hDlg, CBO_NAME);
					SendMessage(hMonsterlist, WM_GETTEXT, 600, (LPARAM)temp);
					
					t_packet packet;
					
					int spriteno = GetDlgItemInt(hDlg, IDC_SPRITENO, NULL, false);
					if(spriteno != 0)
					{
						//if(IsDlgButtonChecked( hDlg, IDC_RADIO1 ) || spriteno >= 100)
							packet.u.summon_monster.id = spriteno;
						/*else if(IsDlgButtonChecked( hDlg, IDC_RADIO2 ) && spriteno < 100)
							packet.u.summon_monster.id = spriteno + 100;
						else if(IsDlgButtonChecked( hDlg, IDC_RADIO3 ) && spriteno < 100)*/ //Need to be debuged
						/*{
							AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,298));
							packet.u.summon_monster.id = spriteno + 200;
						}*/
					}
					else
					{
						for (int i = 0; i < MAX_CHARACTER_SPRITE_; i++)
						{
							if(!strcmp(temp, g_infNpc[i].szName))
							{
								packet.u.summon_monster.id = i;
								break;
							}
						}
					}

					/*if(IsDlgButtonChecked( hDlg, IDC_RADIO1 ) || spriteno >= 100)
						packet.u.summon_monster.id += 0;
					else if(IsDlgButtonChecked( hDlg, IDC_RADIO2 ) && spriteno < 100)
						packet.u.summon_monster.id += 100;
					else if(IsDlgButtonChecked( hDlg, IDC_RADIO3 ) && spriteno < 100)
					{
						AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,298));
						packet.u.summon_monster.id += 200;
					}*/ //Need to be debuged

					if(IsDlgButtonChecked(hDlg, IDC_SPREAD) == BST_CHECKED)
					{
						packet.u.summon_monster.spread = true;
					}
					else
					{
						packet.u.summon_monster.spread = false;
					}

					if(IsDlgButtonChecked(hDlg, IDC_SUMMON) == BST_CHECKED)
					{
						packet.u.summon_monster.summon = true;
					}
					else
					{
						packet.u.summon_monster.summon = false;
					}

					packet.u.summon_monster.amount = amount;
					packet.u.summon_monster.x = x;
					packet.u.summon_monster.y = y;
					packet.u.summon_monster.loot = loot;
					packet.u.summon_monster.lootAmount = lootAmount;
					packet.h.header.type = CMD_SUMMON_MONSTER;
					packet.h.header.size = sizeof(t_summon_monster);

					QueuePacket(&packet, 1);
				}
				break;

			case IDCANCEL :
				EndDialog(hDlg,0);
				return false;
			}
		}
	}
	
	return false;
}
// soto 030307 End.

#endif


void ToolMapMove()
{
	// acer4
	if( g_StartMenuOn ) return;
	
	
#ifdef _DEBUG
	
	HWND hMapMove= CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_MAP_MOVE ), g_hwndMain, (DLGPROC)ToolMapMoveProc);
	ShowWindow( hMapMove, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hMapMove, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hMapMove, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow( hMapMove, SW_SHOW);
#endif
}


//////// soto 030307
void ToolMapMoveMultiple()
{
	if( g_StartMenuOn) return;
	
#ifdef _DEBUG
	
	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_MAP_MOVE_MULTIPLE ), g_hwndMain, (DLGPROC)ToolMapMoveMultipleProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);
	
#endif
}


void ToolGenerateMonster()
{
	if( g_StartMenuOn) return;

#ifdef _DEBUG

	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_MONSTERMAKE ), g_hwndMain, (DLGPROC)ToolGenerateMonsterProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);

#endif
}

extern void SetLocalWarTime()
{
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT

	JustMsg(" 아직 지원되지 않는 기능입니다. - soto -"); return; //아직 지원되지 않는다. 030308 soto


	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_LOCAL_WAR_SET ), g_hwndMain, (DLGPROC)SetLocalWarTimeProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);

#endif
#endif
}



extern void SetNationWarTime()
{
	#ifdef _DEBUG
	#ifndef LIGHT_GM_CLIENT

	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_NATION_WAR ), g_hwndMain, (DLGPROC)SetNationWarTimeProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);

#endif

#endif
}




extern BOOL CALLBACK SetLocalWarTimeProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
	#ifdef _DEBUG
//	char temp[MAX_PATH];
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			return TRUE;
		}
	case WM_PAINT:
		break;			

	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{				
		case IDOK :	
			{
				
			}
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
			
		default : return true;
		}
		break;
	}
#endif
	return false;

}




extern BOOL CALLBACK SetNationWarTimeProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{	
			HWND hCbStatusWnd = GetDlgItem(hDlg,CBO_WAR_STATUS);
			HWND hCbFieldNameWnd = GetDlgItem(hDlg,CBO_WAR_FIELD_NAME);
			HWND hLoopTimeWnd = GetDlgItem(hDlg,EDT_LOOP_TIME);

			SendMessage(hCbStatusWnd,CB_ADDSTRING,0,(LPARAM)"0: Default");
			SendMessage(hCbStatusWnd,CB_ADDSTRING,0,(LPARAM)"1: Peace");
			SendMessage(hCbStatusWnd,CB_ADDSTRING,0,(LPARAM)"2: Vote");
			SendMessage(hCbStatusWnd,CB_ADDSTRING,0,(LPARAM)"3: Prepare");
			SendMessage(hCbStatusWnd,CB_ADDSTRING,0,(LPARAM)"4: War");

			SendMessage(hCbStatusWnd,CB_SETCURSEL,0,0);

			SendMessage(hCbFieldNameWnd,CB_ADDSTRING,0,(LPARAM)"0: Mountain Recess");
			SendMessage(hCbFieldNameWnd,CB_ADDSTRING,0,(LPARAM)"1: Plain");
			SendMessage(hCbFieldNameWnd,CB_ADDSTRING,0,(LPARAM)"2: Island");
			SendMessage(hCbFieldNameWnd,CB_ADDSTRING,0,(LPARAM)"3: Snowy Field");
			SendMessage(hCbFieldNameWnd,CB_ADDSTRING,0,(LPARAM)"4: Hades");

			SendMessage(hCbFieldNameWnd,CB_SETCURSEL,0,0);

			SendMessage(hLoopTimeWnd,WM_SETTEXT,0,(LPARAM)"0"); // 초기 시간은 0분.

			return TRUE;
		}
		
	case WM_PAINT:
		break;			
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{				
		case IDOK :	
			{
				char szTemp[128] = {0,};
				char szTemp2[128] = {0,};
				HWND hCbStatusWnd = GetDlgItem(hDlg,CBO_WAR_STATUS);
				HWND hCbFieldNameWnd = GetDlgItem(hDlg,CBO_WAR_FIELD_NAME);
				HWND hLoopTimeWnd = GetDlgItem(hDlg,EDT_LOOP_TIME);

				int nIndex = SendMessage(hCbStatusWnd,CB_GETCURSEL,0,0);
				if(nIndex == LB_ERR)return false;
				itoa(nIndex,szTemp,10);strcat(szTemp," ");

				nIndex = SendMessage(hCbFieldNameWnd,CB_GETCURSEL,0,0);
				if(nIndex == LB_ERR)return false;
				itoa(nIndex,szTemp2,10);strcat(szTemp,szTemp2);strcat(szTemp," ");

				SendMessage(hLoopTimeWnd,WM_GETTEXT,128,(LPARAM)szTemp2);
				strcat(szTemp,szTemp2);

				SysInfo.gamemakemode = true;
				
				WarLoopTime(szTemp); // 여기서 정보를 보낸다.

				SysInfo.gamemakemode = false;
			}
		break;
			
		case IDCANCEL :	EndDialog(hDlg, LOWORD(wParam));
			return (TRUE);
			
		default : return true;
		}
		break;
	}
#endif
	return false;

}
///////// soto 030307 End.

// 020830 YGI
void StartSetScenarioTime()
{
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	
	HWND hDlg= CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_SET_SCENARIO_TIME ), g_hwndMain, (DLGPROC)ProcSetScenario);
	ShowWindow( hDlg, SW_HIDE);
	
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlg, &rect );
	GetWindowRect( g_hwndMain, &grect );
	MoveWindow( hDlg, grect.right+1, 0, rect.right, rect.bottom, true);
	ShowWindow( hDlg, SW_SHOW);
#endif
#endif
}

//< soto 030307





////// ProcSetScenario에서만 쓰이는 전용 함수이다. 딴데서 쓰지 맙시다.
//
//
void ReleaseListData(HWND hListWnd)
{
	int count = SendMessage(hListWnd,LB_GETCOUNT,0,0);
	for(int i = 0;i < count;i++)
	{
		void* data = (void*)SendMessage(hListWnd,LB_GETITEMDATA,i,0);
		delete data;
	}
}

void SelectedTimeList(HWND hListWnd,int index)
{
	if(index == LB_ERR || !g_hSetScenarioDlgWnd)return;

	char temp[128] = {0,};

	HWND hStartWnd = GetDlgItem(g_hSetScenarioDlgWnd,EDT_STARTTIME);
	HWND hStartMinWnd = GetDlgItem(g_hSetScenarioDlgWnd,EDT_STARTMIN);
	HWND hEndWnd = GetDlgItem(g_hSetScenarioDlgWnd,EDT_ENDTIME);
	HWND hEndMinWnd = GetDlgItem(g_hSetScenarioDlgWnd,EDT_ENDMIN);

	k_server_scenario_time_info* data = (k_server_scenario_time_info*)SendMessage(hListWnd,LB_GETITEMDATA,index,0);
	
	// 각가의 에디트 창에 해당정보를 디스플레이.
	itoa(data->nStartHour,temp,10);
	SendMessage(hStartWnd,WM_SETTEXT,0,(LPARAM)temp);//시
	itoa(data->nStartMinute,temp,10);
	SendMessage(hStartMinWnd,WM_SETTEXT,0,(LPARAM)temp);//분

	itoa(data->nEndHour,temp,10);
	SendMessage(hEndWnd,WM_SETTEXT,0,(LPARAM)temp);//시
	itoa(data->nEndMinute,temp,10);
	SendMessage(hEndMinWnd,WM_SETTEXT,0,(LPARAM)temp);//분
}

void SendSelectedSenarioName(int index)
{
	if(0 > index || 2 < index || !g_hSetScenarioDlgWnd)
	{
		return; // 지금은 세개의 던젼 밖에 없다.
	}

	HWND hNameWnd = GetDlgItem(g_hSetScenarioDlgWnd,CBO_SCENARIO_NAME);
	HWND hListWnd = GetDlgItem(g_hSetScenarioDlgWnd,LST_SCHEDULE);
	
	ReleaseListData(hListWnd);// 리스트에 테이터가 있을수도 있다.
	SendMessage(hListWnd,LB_RESETCONTENT,0,0); //깨끗이 리스트를 지운다.
	SendMessage(hNameWnd,CB_SETCURSEL,index,0);//해당 던젼이름으로 바꾼다.

	// 다음에 서버에 해당던전의 시간표를 요청한다.
	t_packet packet;
	packet.h.header.type = CMD_SCENARIO_TIME_INFO;
	packet.h.header.size = sizeof( k_client_scenario_time_info );
	packet.u.kein.client_scenario_time.idDungeon =  index+1;;
	
	
	QueuePacket( &packet, 1 );
}

// 여기서는 리스트에다 해당 텍스트를 추가 시켜주고 데이터를 연결만 시켜주면 된다
bool AddTimeList(void* pTemp)
{
	k_server_scenario_time_info* pData = (k_server_scenario_time_info*)pTemp;
	
	if(!g_hSetScenarioDlgWnd)return false;
	
	HWND hNameWnd = GetDlgItem(g_hSetScenarioDlgWnd,CBO_SCENARIO_NAME);
	HWND hListWnd = GetDlgItem(g_hSetScenarioDlgWnd,LST_SCHEDULE);
	int nCbIndex = SendMessage(hNameWnd,CB_GETCURSEL,0,0);
	
	if(pData->idDungeon != (nCbIndex  + 1) )
	{
		ReleaseListData(hListWnd);
		SendMessage(hListWnd,LB_RESETCONTENT,0,0);
		return false;
	}

	char szAdd[600] = {0,};
	char szTemp[600] = {0,};

	char szHourText[64] = {0,};
	char szDayText[64] = {0,};

#ifdef KOREA_LOCALIZING_
	strcpy(szHourText,"시");
	strcpy(szDayText,"요일");		
#endif

	::strcpy(szAdd,::GetDayOfWeekByNo(pData->nApplyDay));
	strcat(szAdd,szDayText); // "요일" 의텍스트이다.


	itoa(pData->nStartHour,szTemp,10);
	strcat(szAdd,szTemp);
	
	strcat(szAdd,szHourText);
	
	strcat(szAdd," ~ ");
	
	itoa(pData->nEndHour,szTemp,10);
	strcat(szAdd,szTemp);
	
	strcat(szAdd,szHourText);

	int nListIndex = SendMessage(hListWnd,LB_ADDSTRING,0,(LPARAM)szAdd);
	SendMessage(hListWnd,LB_SETITEMDATA,nListIndex,(LPARAM)pData);

	SendMessage(hListWnd,LB_SETCURSEL,nListIndex,0);

	return true;
}



//
//
///////


BOOL CALLBACK ProcSetScenario(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{
			SetTimer( g_hwndMain, 18, 1000, NULL);

			g_hSetScenarioDlgWnd = hDlg;

			// 초기화할때.. 콤보박스와 리스트박스에 내용을 위해. 던젼에 대한 자료를 요청한다.
			HWND hNameWnd = GetDlgItem(g_hSetScenarioDlgWnd,CBO_SCENARIO_NAME);
			SendMessage(hNameWnd,CB_ADDSTRING,0,(LPARAM)"ABYSS");
			SendMessage(hNameWnd,CB_ADDSTRING,0,(LPARAM)"UNDEAD");
			SendMessage(hNameWnd,CB_ADDSTRING,0,(LPARAM)"BIGMAZE");
			
			::SendSelectedSenarioName(0);
			return TRUE;
		}
		
	case WM_PAINT:
		break;			
		
	case WM_TIMER:
		if( wParam == 18 )
		{
			;
			GameTestProc( &g_DirectDrawInfo );
		}
		break;
	case WM_COMMAND:
		{
			switch( LOWORD( wParam )) 
			{
			case IDOK :
				{
					t_packet packet;
					k_scenario_time_change* pData = NULL;

					HWND hStartWnd = GetDlgItem(g_hSetScenarioDlgWnd,EDT_STARTTIME);
					HWND hStartMinWnd = GetDlgItem(g_hSetScenarioDlgWnd,EDT_STARTMIN);
					HWND hEndWnd = GetDlgItem(g_hSetScenarioDlgWnd,EDT_ENDTIME);
					HWND hEndMinWnd = GetDlgItem(g_hSetScenarioDlgWnd,EDT_ENDMIN);

					HWND hListWnd = GetDlgItem(g_hSetScenarioDlgWnd,LST_SCHEDULE);
					HWND hNameWnd = GetDlgItem(g_hSetScenarioDlgWnd,CBO_SCENARIO_NAME);

					int nCbIndex = SendMessage(hNameWnd,CB_GETCURSEL,0,0);
					int nListIndex = SendMessage(hListWnd,LB_GETCURSEL,0,0);
					if(nListIndex == LB_ERR || nCbIndex == LB_ERR) return false;
					

					char szStr1[128] = {0,};
					char szStr2[128] = {0,};
					char szStr3[128] = {0,};
					char szStr4[128] = {0,};

					SendMessage(hStartWnd,WM_GETTEXT,128,(WPARAM)szStr1);
					SendMessage(hStartMinWnd,WM_GETTEXT,128,(WPARAM)szStr2);
					SendMessage(hEndWnd,WM_GETTEXT,128,(WPARAM)szStr3);
					SendMessage(hEndMinWnd,WM_GETTEXT,128,(WPARAM)szStr4);

					if(0 > strlen(szStr1) || 0 > strlen(szStr2) || 0 > strlen(szStr3)|| 0 > strlen(szStr4))
					{
						JustMsg("모든 값이 셋팅되어야 합니다.");
						return false;
					}

					
					pData = (k_scenario_time_change*)SendMessage(hListWnd,LB_GETITEMDATA,nListIndex,0);

					packet.u.kein.scenario_time_change.idDungeon	= pData->idDungeon;
					packet.u.kein.scenario_time_change.idStartUp	= pData->idStartUp;
					packet.u.kein.scenario_time_change.nApplyDay	= pData->nApplyDay;
					
					packet.u.kein.scenario_time_change.nStartHour	= atoi(szStr1);
					packet.u.kein.scenario_time_change.nStartMinute = atoi(szStr2);
					packet.u.kein.scenario_time_change.nEndHour		= atoi(szStr3);
					packet.u.kein.scenario_time_change.nEndMinute	= atoi(szStr4);
					

					packet.h.header.type = CMD_CHANGE_SCENARIO_TIME;
					packet.h.header.size = sizeof( k_scenario_time_change );

					QueuePacket( &packet, 1 );			

					::SendSelectedSenarioName(nCbIndex);														
					//> CSD-030306
				}
				break;
			case IDCANCEL :
				EndDialog(hDlg, LOWORD(wParam));
				g_hSetScenarioDlgWnd = NULL;// 이 대화창은 닫히게 되니까. 핸들을 없애 준다.
				return (TRUE);
				
			}
			switch(HIWORD(wParam))
			{
				case LBN_SELCHANGE:
				{
					HWND hListWnd = GetDlgItem(hDlg,LST_SCHEDULE);				
					HWND hNameWnd = GetDlgItem(g_hSetScenarioDlgWnd,CBO_SCENARIO_NAME);
					if((HWND)lParam == hListWnd)
					{
						int nIndex = SendMessage(hListWnd,LB_GETCURSEL,0,0);
						::SelectedTimeList(hListWnd,nIndex);
					}
					else					
					if((HWND)lParam == hNameWnd)
					{
						int nIndex = SendMessage(hNameWnd,CB_GETCURSEL,0,0);
						::SendSelectedSenarioName(nIndex);
					}
				}
				break;
			}

		}
		break;
	}
#endif
#endif
	return false;
}
//> soto 030307

void ShowProFileDlg() // 031013 kyo
{
	if( g_StartMenuOn) return;	

#ifdef _DEBUG	
#ifndef LIGHT_GM_CLIENT
	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_PROFILE_TYPED_MSG ), g_hwndMain, (DLGPROC)ProFilerProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);	
#endif
#endif
}

BOOL CALLBACK ProFilerProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{ // 031013 kyo
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{	
			return TRUE;
		}
		
	case WM_PAINT:
		break;			
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{				
		case IDOK :	
			{
				g_ProFileType.bParent = IsDlgButtonChecked(hDlg,CHE_PARENT) == BST_CHECKED;
				g_ProFileType.bCurrent = IsDlgButtonChecked(hDlg,CHE_CURRENT)== BST_CHECKED;
				g_ProFileType.bMax = IsDlgButtonChecked(hDlg,CHE_MAX) == BST_CHECKED;
				g_ProFileType.bAvr = IsDlgButtonChecked(hDlg,CHE_AVR) == BST_CHECKED;
				g_ProFileType.bMin = IsDlgButtonChecked(hDlg,CHE_MIN) == BST_CHECKED;
				g_ProFileType.bRate = IsDlgButtonChecked(hDlg,CHE_RATE) == BST_CHECKED;
				g_IsActiveProFiler = true;
				//CHE_CURRENT CHE_MAX CHE_AVR CHE_MIN
				break;
			}			
		case IDCANCEL :
			{
				g_IsActiveProFiler = false;
				EndDialog(hDlg, LOWORD(wParam));	
				return true;
			}
			
		default :
			{
				return true;
			}
		}
		break;
	}
#endif
#endif
	return false;
}

void ToolPlaySound() //Eleval 19/06/09 - Tool Play Sound
{
	if( g_StartMenuOn) return;
	
#ifdef _DEBUG
	
	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_PLAY_SOUND ), g_hwndMain, (DLGPROC)ToolPlaySoundProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);
	
#endif
}

void ToolLMSMenu() //Eleval 18/07/09 - Tool LMS Menu
{
	if( g_StartMenuOn) return;
	
#ifdef _DEBUG
	
	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_LMS_MENU ), g_hwndMain, (DLGPROC)ToolLMSMenuProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);
	
#endif
}

BOOL CALLBACK ToolPlaySoundProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam) //Eleval 19/06/09 - Tool Play Sound
{ // 031013 kyo
#ifdef _DEBUG
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{	
			SetDlgItemInt(hDlg, IDC_NUMBER, 1, false);
			return TRUE;
		}		
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{				
		case IDOK :	
			{
				int number = GetDlgItemInt(hDlg, IDC_NUMBER, NULL, false);
				if(number < 1 || number > 2472)
					number = 1;

				t_packet packet;

				packet.h.header.type = CMD_TOOL_PLAY_SOUND;
				packet.h.header.size = sizeof(t_tool_play_sound);
				packet.u.tool_play_sound.sound_number = number;

				QueuePacket(&packet, 1);
				EndDialog(hDlg, LOWORD(wParam));
				break;
			}			
		case IDCANCEL :
			{
				EndDialog(hDlg, LOWORD(wParam));	
				return true;
			}
			
		default :
			{
				return true;
			}
		}
		break;
	}
#endif
	return false;
}

void ToggleSanta() //Eleval 22/06/09 - To let GMs Removing their Santa Power
{
#ifdef _DEBUG
	if(Hero->name_status.santa_power)
		Hero->name_status.santa_power = 0;
	else
		Hero->name_status.santa_power = 2;

	t_packet packet;

	packet.h.header.type = CMD_TOOL_TOGGLE_SANTA;
	packet.h.header.size = sizeof(t_gm_toggle_santa);
	packet.u.gm_toggle_santa.santa_power = Hero->name_status.santa_power;


	QueuePacket(&packet, 1);
#endif
}

BOOL CALLBACK ToolLMSMenuProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam) //Eleval 18/07/09 - Tool LMS Menu
{
#ifdef _DEBUG
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{	
			//SetDlgItemInt(hDlg, IDC_NUMBER, 1, false);
			return TRUE;
		}		
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{			
		case IDC_START:
			{
				g_pArenaManager->SendReadyGame(Hero->id);
				EndDialog(hDlg, LOWORD(wParam));
				return true;
				break;
			}
		case IDC_JOIN:
			{
				g_pArenaManager->SendEnterLobby(8);
				g_pArenaManager->SendEnterTeam(0);
				break;
			}
		case IDC_SETLVL:
			{
				int lvl_min = GetDlgItemInt(hDlg, IDC_LVLMIN, NULL, false);
				int lvl_max = GetDlgItemInt(hDlg, IDC_LVLMAX, NULL, false);
				if(lvl_min <= 0) lvl_min = 1;
				if(lvl_min > 150) lvl_min = 150;
				if(lvl_max <= 0) lvl_max = 1;
				if(lvl_max > 150) lvl_max = 150;

				t_packet packet;
				packet.h.header.type = CMD_LMS_SET_LVL;
				packet.h.header.size= sizeof(t_lms_set_lvl);
				if(lvl_min <= lvl_max)
				{
					packet.u.lms_set_lvl.min = lvl_min;
					packet.u.lms_set_lvl.max = lvl_max;
				}
				else
				{
					packet.u.lms_set_lvl.min = lvl_max;
					packet.u.lms_set_lvl.max = lvl_min;
				}

				QueuePacket(&packet, 1);

				AddCurrentStatusMessage( 255,255,0,"Set Lvl Range in LMS to %d - %d", lvl_min, lvl_max);				
				break;
			}
		case IDC_CLEAR:
			{
				t_packet packet;
				packet.h.header.type = CMD_LMS_SET_LVL;
				packet.h.header.size= sizeof(t_lms_set_lvl);
				packet.u.lms_set_lvl.min = 1;
				packet.u.lms_set_lvl.max = 150;

				QueuePacket(&packet, 1);

				AddCurrentStatusMessage( 255,255,0,"Reseted Lvl Range in LMS Arena");
				break;
			}
		case IDOK :				
		case IDCANCEL :
			{
				EndDialog(hDlg, LOWORD(wParam));	
				return true;
				break;
			}
			
		default :
			{
				return true;
				break;
			}
		}
		break;
	}
#endif
	return false;
}

void ToolExpEventMenu()
{
if( g_StartMenuOn) return;
	
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	
	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_EXPEVENT ), g_hwndMain, (DLGPROC)ToolExpEventMenuProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);
#endif
	
#endif
}

BOOL CALLBACK ToolExpEventMenuProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam) //Eleval 22/07/09 - Exp Event Menu
{
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{	
			CheckDlgButton(hDlg, IDC_RADIO1, BST_CHECKED);
			return TRUE;
		}		
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{			
		case IDC_APPLY:
			{
				int multiplier = GetDlgItemInt(hDlg, IDC_MULTIPLIER, NULL, false);
				int range = 0;
				int days[7];
				if(IsDlgButtonChecked( hDlg, IDC_RADIO1 ))
					range = 0;
				else
					range = 1;
				if(IsDlgButtonChecked( hDlg, IDC_DAY1 )) //Sunday
					days[0] = 1;
				if(IsDlgButtonChecked( hDlg, IDC_DAY2 )) //Monday
					days[1] = 1;
				if(IsDlgButtonChecked( hDlg, IDC_DAY3 )) //Tuesday
					days[2] = 1;
				if(IsDlgButtonChecked( hDlg, IDC_DAY4 )) //Wednesday
					days[3] = 1;
				if(IsDlgButtonChecked( hDlg, IDC_DAY5 )) //Thursday
					days[4] = 1;
				if(IsDlgButtonChecked( hDlg, IDC_DAY6 )) //Friday
					days[5] = 1;
				if(IsDlgButtonChecked( hDlg, IDC_DAY7 )) //Saturday
					days[6] = 1;

				if(multiplier < 1) multiplier = 1;
				if(multiplier > 10) multiplier = 1000;

				t_packet packet;
				packet.h.header.type = CMD_SET_EXP_EVENT;
				packet.h.header.size = sizeof(t_set_exp_event);
				packet.u.set_exp_event.multiplier = multiplier;
				packet.u.set_exp_event.range = range;
				for(int i = 0 ; i < 7 ; i++)
				{
					packet.u.set_exp_event.days[i] = days[i];
				}
				packet.u.set_exp_event.active = 1;

				QueuePacket(&packet, 1);

				AddCurrentStatusMessage( 255,255,0,"Exp Event set with multiplier %d", multiplier);
				break;
			}
		case IDC_RESETEXP:
			{
				int range = 0;
				if(IsDlgButtonChecked( hDlg, IDC_RADIO1 ))
					range = 0;
				else
					range = 1;
				t_packet packet;
				packet.h.header.type = CMD_SET_EXP_EVENT;
				packet.h.header.size = sizeof(t_set_exp_event);
				packet.u.set_exp_event.multiplier = 1;
				packet.u.set_exp_event.range = range;
				for(int i = 0 ; i < 7 ; i++)
				{
					packet.u.set_exp_event.days[i] = 0;
				}
				packet.u.set_exp_event.active = 0;
				QueuePacket(&packet, 1);
				AddCurrentStatusMessage( 255,255,0,"Ended Exp Event");
				break;
			}
		case IDOK :				
		case IDCANCEL :
			{
				EndDialog(hDlg, LOWORD(wParam));	
				return true;
				break;
			}
			
		default :
			{
				return true;
				break;
			}
		}
		break;
	}
#endif
#endif
	return false;
}

void ToolPKMenu()
{
#ifdef _DEBUG
	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_MNPK ), g_hwndMain, (DLGPROC)ToolPKMenuProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);

#endif

}

BOOL CALLBACK ToolPKMenuProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam) //Eleval 22/07/09 - Exp Event Menu
{
#ifdef _DEBUG
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{	
			CheckDlgButton(hDlg, IDC_RADIO1, BST_CHECKED);
			return TRUE;
		}		
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{			
		case IDC_ENPK:
			{
				int lvl_min = 1;
				int lvl_max = 150;
				int range = 0;
				int lvl_restr = 0;

				lvl_min = GetDlgItemInt(hDlg, IDC_LVLMIN, NULL, false);
				lvl_max = GetDlgItemInt(hDlg, IDC_LVLMAX, NULL, false);

				if(lvl_min < 1 || lvl_min > 150)
					lvl_min = 1;
				if(lvl_max < 1 || lvl_max > 150)
					lvl_max = 150;

				if(IsDlgButtonChecked( hDlg, IDC_RADIO1 ))
					range = 0;
				else
					range = 1;

				if(IsDlgButtonChecked( hDlg, IDC_CHECK1 ))
					lvl_restr = 1;
				else
					lvl_restr = 0;

				t_packet packet;

				packet.h.header.type = CMD_TOGGLE_PKMODE_GM;
				packet.h.header.size = sizeof(t_tool_toggle_pk);
				packet.u.tool_toggle_pk.type = 0;
				if(lvl_min <= lvl_max)
				{
					packet.u.tool_toggle_pk.lvl_min = lvl_min;
					packet.u.tool_toggle_pk.lvl_max = lvl_max;
				}
				else
				{
					packet.u.tool_toggle_pk.lvl_min = lvl_max;
					packet.u.tool_toggle_pk.lvl_max = lvl_min;
				}
				packet.u.tool_toggle_pk.range = 0;
				packet.u.tool_toggle_pk.restr = lvl_restr;

				QueuePacket(&packet, 1);

				AddCurrentStatusMessage( 255,255,0,"Enabled PK");
				break;
			}
		case IDC_DISPK:
			{
				int lvl_min = 1;
				int lvl_max = 150;
				int range = 0;
				int lvl_restr = 0;

				lvl_min = GetDlgItemInt(hDlg, IDC_LVLMIN, NULL, false);
				lvl_max = GetDlgItemInt(hDlg, IDC_LVLMAX, NULL, false);

				if(lvl_min < 1 || lvl_min > 150)
					lvl_min = 1;
				if(lvl_max < 1 || lvl_max > 150)
					lvl_max = 150;

				if(IsDlgButtonChecked( hDlg, IDC_RADIO1 ))
					range = 0;
				else
					range = 1;

				if(IsDlgButtonChecked( hDlg, IDC_CHECK1 ))
					lvl_restr = 1;
				else
					lvl_restr = 0;

				t_packet packet;

				packet.h.header.type = CMD_TOGGLE_PKMODE_GM;
				packet.h.header.size = sizeof(t_tool_toggle_pk);
				packet.u.tool_toggle_pk.type = 1;
				if(lvl_min <= lvl_max)
				{
					packet.u.tool_toggle_pk.lvl_min = lvl_min;
					packet.u.tool_toggle_pk.lvl_max = lvl_max;
				}
				else
				{
					packet.u.tool_toggle_pk.lvl_min = lvl_max;
					packet.u.tool_toggle_pk.lvl_max = lvl_min;
				}
				packet.u.tool_toggle_pk.range = 0;
				packet.u.tool_toggle_pk.restr = lvl_restr;

				QueuePacket(&packet, 1);
				AddCurrentStatusMessage( 255,255,0,"Disabled PK");
				break;
			}
		case IDC_ENBATTLE:
			{
				int lvl_min = 1;
				int lvl_max = 150;
				int range = 0;
				int lvl_restr = 0;

				lvl_min = GetDlgItemInt(hDlg, IDC_LVLMIN, NULL, false);
				lvl_max = GetDlgItemInt(hDlg, IDC_LVLMAX, NULL, false);

				if(lvl_min < 1 || lvl_min > 150)
					lvl_min = 1;
				if(lvl_max < 1 || lvl_max > 150)
					lvl_max = 150;

				if(IsDlgButtonChecked( hDlg, IDC_RADIO1 ))
					range = 0;
				else
					range = 1;

				if(IsDlgButtonChecked( hDlg, IDC_CHECK1 ))
					lvl_restr = 1;
				else
					lvl_restr = 0;

				t_packet packet;

				packet.h.header.type = CMD_TOGGLE_PKMODE_GM;
				packet.h.header.size = sizeof(t_tool_toggle_pk);
				packet.u.tool_toggle_pk.type = 2;
				if(lvl_min <= lvl_max)
				{
					packet.u.tool_toggle_pk.lvl_min = lvl_min;
					packet.u.tool_toggle_pk.lvl_max = lvl_max;
				}
				else
				{
					packet.u.tool_toggle_pk.lvl_min = lvl_max;
					packet.u.tool_toggle_pk.lvl_max = lvl_min;
				}
				packet.u.tool_toggle_pk.range = 0;
				packet.u.tool_toggle_pk.restr = lvl_restr;

				QueuePacket(&packet, 1);

				AddCurrentStatusMessage( 255,255,0,"Enabled Battle");
				break;
			}
		case IDC_DISBATTLE:
			{
				int lvl_min = 1;
				int lvl_max = 150;
				int range = 0;
				int lvl_restr = 0;

				lvl_min = GetDlgItemInt(hDlg, IDC_LVLMIN, NULL, false);
				lvl_max = GetDlgItemInt(hDlg, IDC_LVLMAX, NULL, false);

				if(lvl_min < 1 || lvl_min > 150)
					lvl_min = 1;
				if(lvl_max < 1 || lvl_max > 150)
					lvl_max = 150;

				if(IsDlgButtonChecked( hDlg, IDC_RADIO1 ))
					range = 0;
				else
					range = 1;

				if(IsDlgButtonChecked( hDlg, IDC_CHECK1 ))
					lvl_restr = 1;
				else
					lvl_restr = 0;

				t_packet packet;

				packet.h.header.type = CMD_TOGGLE_PKMODE_GM;
				packet.h.header.size = sizeof(t_tool_toggle_pk);
				packet.u.tool_toggle_pk.type = 3;
				if(lvl_min <= lvl_max)
				{
					packet.u.tool_toggle_pk.lvl_min = lvl_min;
					packet.u.tool_toggle_pk.lvl_max = lvl_max;
				}
				else
				{
					packet.u.tool_toggle_pk.lvl_min = lvl_max;
					packet.u.tool_toggle_pk.lvl_max = lvl_min;
				}
				packet.u.tool_toggle_pk.range = 0;
				packet.u.tool_toggle_pk.restr = lvl_restr;

				QueuePacket(&packet, 1);

				AddCurrentStatusMessage( 255,255,0,"Disabled Battle");
				break;
			}
		case IDOK :				
			{
				EndDialog(hDlg, LOWORD(wParam));	
				return true;
				break;
			}
			
		default :
			{
				return true;
				break;
			}
		}
		break;
	}
#endif
	return false;
}

void ToolCheckQuestMenu() //Eleval 13/08/09 - To test Quest NPC
{
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	HWND hDlgWnd = CreateDialog(g_hInstance,MAKEINTRESOURCE( IDD_CHECK_QUEST ), g_hwndMain, (DLGPROC)ToolCheckQuestMenuProc);
	ShowWindow( hDlgWnd, SW_HIDE);
	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	RECT rect;
	RECT grect;
	GetWindowRect( hDlgWnd, &rect);
	GetWindowRect( g_hwndMain, &grect);
	MoveWindow( hDlgWnd, grect.right+1,0,rect.right,rect.bottom, true);
	ShowWindow( hDlgWnd, SW_SHOWNORMAL);
#endif

#endif

}

BOOL CALLBACK ToolCheckQuestMenuProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam) //Eleval 13/08/09 - To test Quest Npc
{
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	switch(Message)		
	{					
	case WM_INITDIALOG:	
		{	
			SetDlgItemInt(hDlg, IDC_NUMBER, 1, false);
			return TRUE;
		}		
	case WM_COMMAND:
		switch( LOWORD( wParam )) 
		{				
		case IDC_ADD :	
			{
				int nX = GetDlgItemInt(hDlg, IDC_X, NULL, false);
				int nY = GetDlgItemInt(hDlg, IDC_Y, NULL, false);

				t_packet packet;

				packet.h.header.type = CMD_TOOL_QUEST_CHECK;
				packet.h.header.size = sizeof(t_tool_quest_check);
				packet.u.tool_quest_check.force = false;
				packet.u.tool_quest_check.nX = nX;
				packet.u.tool_quest_check.nY = nY;

				QueuePacket(&packet, 1);
				EndDialog(hDlg, LOWORD(wParam));
				break;
			}			
		case IDC_FORCE :
			{
				t_packet packet;

				packet.h.header.type = CMD_TOOL_QUEST_CHECK;
				packet.h.header.size = sizeof(t_tool_quest_check);
				packet.u.tool_quest_check.force = true;

				QueuePacket(&packet, 1);

				EndDialog(hDlg, LOWORD(wParam));	
				return true;
			}
			
		default :
			{
				return true;
			}
		}
		break;
	}
#endif
#endif
	return false;
}
