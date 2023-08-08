#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <Io.h>
	
	
#include "Dragon.h"
#include "Char.h"
#include "Tool.h"
#include "GameProc.h"
#include "Map.h"
#include "Hangul.h"
#include "DirectSound.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"
#include "Convert565to555.h"
#include "Skill.h"
#include "Item.h"
	
#define __MOUSE_CURSOR_H__
#include "MouseCursor.h"
	
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////
/////	extern variables...
/////


extern SMENU SMenu[ MAX_MENU]; // menustart.cpp


extern Spr *SkillIcon;






/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////	
/////	user variables...
/////

//////////// 0527 lkh 추가 ////////////
t_queue		g_queue;

int CurMouseCursorNo = 1;
static int CurMouseCursorSpotX = 18, CurMouseCursorSpotY = 10;
static int 	CurMouseCursorStart, CurMouseCursorEnd, CurMouseCursorAni;


static int OldMouseCursorNo;
static int PushPopMoueCursor;


unsigned char *McBuf;
Spr mc[ MAX_MOUSE_CURSOR][20];

/* = { 
	{1,11},
	{1,5},
 {1,2},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {1,1},
 {1,1},
 {1,1}, // 10
 {1,1}, 
 {1,1},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0}, // 20
 {0,0}, 
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {1,5},
 {1,4},
 {1,5}, // 30
 {1,5}, 
 {0,0},
 {1,4},
 {1,4},
 {1,3},
 {1,4},
 {1,3},
 {1,4},
 {1,4},
 {1,3}, // 40
 {1,2}, 
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},
 {0,0},  
{0,0}};  // 49*/



void CursorNo( int no );

	
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////
/////		user functions..
/////



void LoadCursorAni( void )
{
	FILE *fp;
	int i;
	char temp[ FILENAME_MAX];

	fp = Fopen( "./data/CursorAni.txt", "rt" );
	if( fp )
	{
		for( i = 0 ; i < MAX_MOUSE_CURSOR ; i ++)
		{
			fgets( temp, FILENAME_MAX, fp );
			sscanf( temp, "%d %d %d %d ",	&cursorani[i].start, 
											&cursorani[i].end,
											&cursorani[i].offx,
											&cursorani[i].offy
											);
		}
		fclose(fp);

		fp = Fopen( "./data/CursorAni.b", "wb" );
		if( fp )
		{
			fwrite( cursorani, sizeof(t_cursorani), MAX_MOUSE_CURSOR, fp );
			fclose(fp);
		}
	}
	else
	{
		fp = Fopen("./data/CursorAni.b", "rb" );
		if( fp )
		{
			fread( cursorani, sizeof(t_cursorani), MAX_MOUSE_CURSOR, fp );
			fclose(fp);
		}
	}
}


// 0810 KHS
void CurMouseCursorFrameCounter( void )
{
	static int del;
	del ++;

	if( tool_MouseCursorBlock ) 
	{
		if( ++del >= 2 ) {	CurMouseCursorAni ++; del = 0; }
				if( CurMouseCursorAni > CurMouseCursorEnd ) 
					CurMouseCursorAni = CurMouseCursorStart; 
		
		return;
	}

	switch( CurMouseCursorNo )
	{
		// 일반커서.
	case 1 : 	if( ++del >= 2 ) {	CurMouseCursorAni ++; del = 0; }
				if( CurMouseCursorAni > CurMouseCursorEnd ) 
					CurMouseCursorAni = CurMouseCursorStart; 
		break;

		// 일반잡기켜서.
	case 2 :	if( ++del >= 2 ) {	CurMouseCursorAni ++; del = 0; }
				if( CurMouseCursorAni > CurMouseCursorEnd ) CurMouseCursorAni = CurMouseCursorEnd;
		break;

		// 마우스의 이동포인트 
	case 3 :	if( ++del >= 2 ) {	CurMouseCursorAni ++; del = 0; }
				if( CurMouseCursorAni > CurMouseCursorEnd ) 
				{
					CursorNo( 1 );
				}	
		break;

	case 53 :	CurMouseCursorAni ++; 
				if( CurMouseCursorAni > CurMouseCursorEnd ) 
				{
					CursorNo( 1 );
				}	
		break;
		
	default :	CurMouseCursorAni = CurMouseCursorStart;
		break;
	}
}



void GetImageBox( char *backbuf, char *ground, int sx, int sy, int ex, int ey )
{	
	int xl = (ex - sx + 1) * 2;
	int c=0;

	for( ; sy <= ey ; sy ++) 
	{
		memcpy( backbuf + c * xl, ground + sx * 2 + sy * dDxSize, xl );
		c++;
	}
}

void PutImageBox( char *backbuf, char *ground, int sx, int sy, int ex, int ey )
{						
	int xl = (ex - sx + 1) * 2;
	int c=0;

	for( ; sy <= ey ; sy ++) 
	{
		memcpy( ground + sx * 2 + sy * dDxSize, backbuf + c * xl, xl );
		c++;
	}
}

/*
// 1이 의미하는것은 평상시를 말한다. 
// 1이 들어면 함수내에서	전투상태, 
//							기본상태에 따라 마우스를 바뀌준다. 

  1  11  25  36   // 1 : 일반커서 
  1  10  22  37   // 2 : 일반잡기커서 
  1   3  22  37   // 3 : 이동 Point 효과 
  0   0  -7   0   // 4 : 일반 터치커서 
  0   0   0   0   // 5 : 전투커서 
  0   0   0   0   // 6 : 전투터치커서1 
  1   1 -19  23   // 7 : 마법 커서1 
  1   5 -24   2   // 8 : 화면 스크롤 커서1 
  1   5   2   4   // 9 : 화면 스크롤 커서2 
  1   5  20  15   // 10 : 화면 스크롤 커서3 
  1   5  17  26   // 11 : 화면 스크롤 커서4 
  1   8 -12  33   // 12 : 화면 스크롤 커서5 
  1   5 -44  48   // 13 : 화면 스크롤 커서6 
  1   5 -62   3   // 14 : 화면 스크롤 커서7 
  1   5 -53  13   // 15 : 화면 스크롤 커서8 
  1  10  22  20   // 16 : 스킬일반커서1 
  1   6  89  17   // 17 : 스킬터치커서1 
  1   8 -17  21   // 18 : 스킬 논터치 커서 
  1   8  10  25   // 19 : 스킬detect커서1 
  1   4  25  31   // 20 : 스킬OBPoint커서 
  1   6  25  31   // 21 : 스킬OBPoint터치커서 
  1  10  23  20   // 22 : 스킬Identified커서 
  1  10  23  20   // 23 : 스킬Identified터치커서 
  1  10  23  20   // 24 : 스킬음악커서 
  1  10  23  20   // 25 : 스킬음악터치 
  0   0   0   0   // 26 : 스킬Hide커서 
  1   8 -17  20   // 27 : 스킬Hide 터치커서 
  1  10  18  12   // 28 : 스킬 삽커서 
  1   8  25  20   // 29 : 스킬 낫커서 
  1   8   7  21   // 30 : 스킬 곡괭이커서 
  1  18  89  17   // 31 : 스킬 낚시대커서 
  1   6 -12  14   // 32 : 스킬 작은칼커서 
  1  11 -37  -9   // 33 : 스킬 그믈커서 
  1   8  61 -27   // 34 : 스킬 도끼커서 
  1   5  44  14   // 35 : 스킬 톱커서 
  1   7  53  -6   // 36 : 스킬 도살칼커서 
  1   7   0   0   // 37 : 스킬 호미커서 
  1   5  78  37   // 38 : 스킬 막대기커서 
  1   9 -18   8   // 39 : 스킬 우유통커서 
  1   5   0  11   // 40 : 스킬 우유통클릭커서 
  1   4  -6  23   // 41 : 스킬 가위터치커서 
  1  10  23  20   // 42 : 스킬 힐링  
  1  10  23  20   // 43 : 스킬 힐링 터치커서 
  1  10  22  37   // 44 : 일반커서테두리빨간색 
  1  10  22  37   // 45 : 일반잡기커서테두리빨간색 
  1   3  22  37   // 46 : 일반 터치커서테두리빨간색 
  1  10  22  37   // 47 : 일반커서초록색 
  1  10  22  37   // 48 : 일반잡기커서초록색 
  1   3  22  37   // 49 : 일반 터치커서초록색 
  1   9  -1  18   // 50 : 부싯돌 
  1   8 -17  21   // 51 : 이동포인트 
  0   0   0   0   // 52 : 마법 캐스팅 효과 
  0   0   0   0   // 53 :  
  0   0   0   0   // 54 :  
  0   0   0   0   // 55 :  
  0   0   0   0   // 56 :  
  0   0   0   0   // 57 :  
  0   0   0   0   // 58 :  
  0   0   0   0   // 59 :  
  0   0   0   0   // 60 :  
	
*/	
	
	
	
void CursorStatus( void )
{		

	return;



	static int cursor= -1, oldcursor;
		
	oldcursor = cursor;
	if( PushPopMoueCursor == 0 ) 
	if( SkillStatus == SKILL_OFF || CurMouseCursorNo == 1 ) // 상황이 맞추어 출력하라..
	{	
		if( Hero->peacests == 0	)
		{		
			if( g_GameInfo.lpcharacter )
			{	
				switch( g_GameInfo.nSelectedSpriteType )
				{
				case SPRITETYPE_ON_THE_CEILING_CHAR	 :
				case SPRITETYPE_MONSTER		:
				case SPRITETYPE_NPC			:
				case SPRITETYPE_CHARACTER	:	cursor = 4; // 일반 터치커서 
				break;
				}
			}	
			else
			{	
				cursor = 1;
			}	
		}
		else	
		{
			if( g_GameInfo.lpcharacter )
			{	
				switch( g_GameInfo.nSelectedSpriteType )
				{
				case SPRITETYPE_ON_THE_CEILING_CHAR	 :
				case SPRITETYPE_MONSTER		:
				case SPRITETYPE_NPC			:
				case SPRITETYPE_CHARACTER	:	cursor = 6; // 전투터치커서
				break;
				}
			}	
			else
			{	
				cursor = 5;
			}	
		}
		
		if( CursorCheckedItem )		cursor = 2; // 일반잡기커서 
		
		// 마법이 켜져있으면
		if( Hero )
		if( Hero->ready_Magic == 2 || Hero->ready_Magic == 3 ) cursor = 7; //마법 커서1 
		
		
		if( TileMap[Mox / TILE_SIZE][ Moy/TILE_SIZE].attr_dont )
		{
			cursor = 44; // 44 : 일반커서테두리빨간색 
		
		}

		//	BBD 031114	이렇게 되면 g_MouseInMenu에 메뉴번호를 넣은 의미가 없어짐
		//	계인 PM 님이 의도한 코드를 막는듯하여 임시로 주석처리
/*
		if( g_MouseInMenu == true )	
		{
		}
		*/
	}	
		
	if( oldcursor != cursor ) // 마우스커셔가 바뀌었으면....
	{	
		CursorNo( cursor );
	}	
}






// 0810 KHS
void CursorNo( int no )
{	
	int x = 0, y = 0;	
	
	if( no == 0 ) return;
	switch( no )
	{
		case 53 : 
			break;

		default :	if( no >=16 ) no /= 10;
			break;
	}
	
	CurMouseCursorNo = no;
	
	CurMouseCursorStart = cursorani[no-1].start;
	CurMouseCursorEnd   = cursorani[no-1].end;
	
	CurMouseCursorSpotX = cursorani[no-1].offx;
	CurMouseCursorSpotY = cursorani[no-1].offy;
	
	CurMouseCursorAni   = CurMouseCursorStart;
}	
	
	
	
	
void PushMouseCursor( void )
{	
	if( 8 <= CurMouseCursorNo && CurMouseCursorNo <= 15 )
	{
		return;
	}
	
	if( PushPopMoueCursor ) return;
	PushPopMoueCursor =1;
		 
	OldMouseCursorNo = CurMouseCursorNo;
}	
	
void PopMouseCursor( void )
{	
	if( SMenu[MN_MAININTERFACE].bActive == FALSE ) return;
	
	
	if( PushPopMoueCursor )
	{
		CursorNo( OldMouseCursorNo );
		PushPopMoueCursor = 0;
	}
}	
	
	
	
	
	
extern void		PutItemIcon(int x, int y, int item_number, int delay=0, int effect = 2, int is_not_equip = 0 );
extern	POINT	MouseItemInRect;
	
extern ItemAttr	InvItemAttr[3][3][8];
extern ItemAttr	EquipItemAttr[8];
extern ItemAttr	QuickItemAttr[7];
	
	
	
void withSkillItem( void )
{	
	int x, y;
	DWORD no;
	
	if( SkillStatus != SKILL_READY ) return;
		
	switch( SkillNo )
	{
		// Sow
	case 22  :	break;
	
	default : return;
	}
	
	x = g_pointMouseX + CurMouseCursorSpotX;
	y = g_pointMouseY + CurMouseCursorSpotY;
	
	PutItemIcon( x, y + 20, SkillItemNo );
	
	
	switch( SkillItemPOS.type )
	{
	case QUICK :	no = QuickItemAttr[ SkillItemPOS.p3].attr[ IATTR_MUCH];	break;
	case INV  :		no = InvItemAttr[ SkillItemPOS.p1][ SkillItemPOS.p2][ SkillItemPOS.p3].attr[ IATTR_MUCH];	break;
	case EQUIP :	no = EquipItemAttr[ SkillItemPOS.p3].attr[ IATTR_MUCH];	break;
	default : return;
	}
	
	HprintBold( x, y+30, 0xffff, 0, "%2d", no );
}	
	
void CursorDisplayBack( void )
{		
	if( MouseCursorOnOff == false ) return;// 기술시전중.
	if( SMenu[MN_LOGO_START].bActive==true ) return;

	PmcSx = SCREEN_WIDTH;
	PmcSy = SCREEN_HEIGHT;
	PmcEx = PmcEy = 0;
	Spr *s = &mc[ CurMouseCursorNo-1][ CurMouseCursorAni];
/*	switch(CurMouseCursorNo)
	{
	case 1://sa
		{
	//		s = FieldTypeNomalPut(int mx, int my, int x,int y,int nImageNumber, int type);
		}break;
	default :
		{
			s = &mc[ CurMouseCursorNo-1][ CurMouseCursorAni];
		}break;
	}*/
	int sx = g_pointMouseX + CurMouseCursorSpotX - s->ox;
	int sy = g_pointMouseY + CurMouseCursorSpotY - s->oy;
	int ex = sx + s->xl;
	int ey = sy + s->yl;
		
	if( sx < PmcSx ) PmcSx = sx;
	if( sy < PmcSy ) PmcSy = sy;
	if( ex > PmcEx ) PmcEx = ex;
	if( ey > PmcEy ) PmcEy = ey;
		
	if( PmcSx < 0 ) PmcSx = 0;
	if( PmcSy < 0 ) PmcSy = 0;
	if( PmcEx > SCREEN_WIDTH -1) PmcEx = SCREEN_WIDTH-1;
	if( PmcEy > SCREEN_HEIGHT-1) PmcEy = SCREEN_HEIGHT-1;

#ifdef _DEBUG	
	if( (PmcEx-PmcSx+1)*(PmcEy-PmcSy+1) >= MAX_MOUSE_AREA_BUF_SIZE )
		Error( "Mouse MgBuf is Too Small..\n %d %d %d %d",  PmcSx, PmcSy,  PmcEx, PmcEy );
#endif	

	GetImageBox( MgBuf, g_DestBackBuf, PmcSx, PmcSy,  PmcEx, PmcEy );

	if( SMenu[MN_LODING].bActive != TRUE )  // 화면 Loading할때는 찍지 않는다.
	{	
		PutCompressedImage( g_pointMouseX + CurMouseCursorSpotX, g_pointMouseY + CurMouseCursorSpotY, s );
		int ttx = g_pointMouseX + Mapx, tty = g_pointMouseY + Mapy;

		static int to, tcolor=0, ccolor=0;
		
		if( to ) 
		{
			tcolor += 20;  if( tcolor >= 250  ) { tcolor = 250; to = !to;  }
		}
		else 
		{
			tcolor -= 20;  if( tcolor <= 150  ) { to = !to; }
		}
		ccolor = RGB16( tcolor, tcolor, tcolor );

		Line( ttx-2, tty, ttx+3, tty, ccolor );
		Line( ttx, tty-2, ttx, tty+3, ccolor );

		withSkillItem();
	}

//-------
	if( tool_MouseCursorBlock )
	{
		PutCompressedImage( 320 + CurMouseCursorSpotX, 240 + CurMouseCursorSpotY, s );
		int ttx = 320 + Mapx, 
			tty = 240 + Mapy;
		Line( ttx-5, tty, ttx+5, tty, 0xffff );
		Line( ttx, tty-5, ttx, tty+5, 0xffff );
	}
//-------
	CurMouseCursorFrameCounter();
}
/*
{		
	if( MouseCursorOnOff == false ) return;// 기술시전중.
//	if( SMenu[MN_LOGO_START].bActive==true ) return;
	if( SMenu[MN_LODING].bActive ==true )return;  // 화면 Loading할때는 찍지 않는다.
	PmcSx = SCREEN_WIDTH;
	PmcSy = SCREEN_HEIGHT;
	PmcEx = PmcEy = 0;
	Spr *s;

	switch(CurMouseCursorNo)
	{
	case 1://sa
		{
			s = GetSprOfMenu(CURSOR_DEFAULT_IMG	,0);
			FieldTypeNomalPut(0,0,g_pointMouseX,g_pointMouseY,0,CURSOR_DEFAULT_IMG);
	//CURSOR_CLICK_IMG				10024
	//		s = FieldTypeNomalPut(int mx, int my, int x,int y,int nImageNumber, int type);
		}break;
	default :
		{
			s = &mc[ CurMouseCursorNo-1][ CurMouseCursorAni];
		}break;
	}

	const int sx = g_pointMouseX + CurMouseCursorSpotX - s->ox;
	const int sy = g_pointMouseY + CurMouseCursorSpotY - s->oy;
	const int ex = sx + s->xl;
	const int ey = sy + s->yl;
		
	if( sx < PmcSx ) PmcSx = sx;
	if( sy < PmcSy ) PmcSy = sy;
	if( ex > PmcEx ) PmcEx = ex;
	if( ey > PmcEy ) PmcEy = ey;
		
	if( PmcSx < 0 ) PmcSx = 0;
	if( PmcSy < 0 ) PmcSy = 0;
	if( PmcEx > SCREEN_WIDTH -1) PmcEx = SCREEN_WIDTH-1;
	if( PmcEy > SCREEN_HEIGHT-1) PmcEy = SCREEN_HEIGHT-1;

#ifdef _DEBUG	
	if( (PmcEx-PmcSx+1)*(PmcEy-PmcSy+1) >= MAX_MOUSE_AREA_BUF_SIZE )
	{
		Error( "Mouse MgBuf is Too Small..\n %d %d %d %d",  PmcSx, PmcSy,  PmcEx, PmcEy );
	}
	Hprint2(100,120, g_DestBackBuf,"마우스 %d,%d",CurMouseCursorNo,CurMouseCursorAni);
#endif	

//	GetImageBox( MgBuf, g_DestBackBuf, PmcSx, PmcSy,  PmcEx, PmcEy );
	switch(CurMouseCursorNo)
	{
	case 1:
		break;
	default:
		{
			PutCompressedImage( g_pointMouseX + CurMouseCursorSpotX, g_pointMouseY + CurMouseCursorSpotY, s );//마우스 커서 그림 찍는곳
		}
	}
	
	//클릭포인트 라인 그리는 곳
	{
	const int ClickPointX = g_pointMouseX + Mapx;
	const int ClickPointY = g_pointMouseY + Mapy;
	static char cRelayColor = 0;//static 이다
	cRelayColor+=16;
	const unsigned char ucTempColor= abs(cRelayColor);
	const WORD wColor = ConvertColor(ucTempColor,ucTempColor,ucTempColor);
	Line( ClickPointX-2,ClickPointY,	ClickPointX+3,	ClickPointY,	wColor );
	Line( ClickPointX,	ClickPointY-2,	ClickPointX,	ClickPointY+3,	wColor );
	}

	withSkillItem();
	
	if( tool_MouseCursorBlock )
	{
		PutCompressedImage( 320 + CurMouseCursorSpotX, 240 + CurMouseCursorSpotY, s );
		int ttx = 320 + Mapx, 
			tty = 240 + Mapy;
		Line( ttx-5, tty, ttx+5, tty, 0xffff );
		Line( ttx, tty-5, ttx, tty+5, 0xffff );
	}
	CurMouseCursorFrameCounter();
}
*/
/*
void CursorDisplay_Sub( int *newflag )
{			
	int sx, sy, ex, ey;
	char *t;
	static int oldx, oldy;


	if( MouseCursorOnOff == false ) return;// 기술시전중.

//	if( SMenu[MN_LOGO_START_SAMSUNG].bActive==TRUE ) return;
	if( SMenu[MN_LOGO_START].bActive==true ) return;

	
	if( oldx == g_pointMouseX && oldy == g_pointMouseY ) return;
	oldx = g_pointMouseX, oldy = g_pointMouseY;

	t = g_DestBackBuf;
	WORD twdxsize = wDxSize;
	DWORD tddxsize = dDxSize;
	wDxSize = wSurDxSize;
	dDxSize = dSurDxSize;

	g_DestBackBuf = GetSurfacePointer( g_DirectDrawInfo.lpDirectDrawSurfacePrimary );
	if( *newflag == 1 )
	{	
		PutImageBox( MgBuf, g_DestBackBuf, PmcSx, PmcSy,  PmcEx, PmcEy );
	}	
	PmcSx = SCREEN_WIDTH;
	PmcSy = SCREEN_HEIGHT;
	PmcEx = PmcEy = 0;
	
	Spr *s = &mc[ CurMouseCursorNo-1][ CurMouseCursorAni]; 
	sx = g_pointMouseX + CurMouseCursorSpotX - s->ox;
	sy = g_pointMouseY + CurMouseCursorSpotY - s->oy;
	ex = sx + s->xl;
	ey = sy + s->yl;
	
	if( sx < PmcSx ) PmcSx = sx;
	if( sy < PmcSy ) PmcSy = sy;
	if( ex > PmcEx ) PmcEx = ex;
	if( ey > PmcEy ) PmcEy = ey;

	if( PmcSx < 0 ) PmcSx = 0;
	if( PmcSy < 0 ) PmcSy = 0;
	if( PmcEx > SCREEN_WIDTH -1) PmcEx = SCREEN_WIDTH-1;
	if( PmcEy > SCREEN_HEIGHT-1) PmcEy = SCREEN_HEIGHT-1;

	if( (PmcEx-PmcSx+1)*(PmcEy-PmcSy+1) >= MAX_MOUSE_AREA_BUF_SIZE )
		Error( "Mouse MgBuf is Too Small..\n %d %d %d %d",  PmcSx, PmcSy,  PmcEx, PmcEy );

	GetImageBox( MgBuf, g_DestBackBuf, PmcSx, PmcSy,  PmcEx, PmcEy );

	*newflag = 1;	

	if( SMenu[MN_LODING].bActive != TRUE )  // 화면 Loading할때는 찍지 않는다.
	{	
		PutCompressedImage( g_pointMouseX + CurMouseCursorSpotX, g_pointMouseY + CurMouseCursorSpotY, s );
		withSkillItem();
	}	

	wDxSize = twdxsize;
	dDxSize = tddxsize;

	g_DestBackBuf = t;
}
*/
/*
int CursorDisplay( int *newflag )
{
//	MSG msg;
	int c=0;

	if( SysInfo.dx == 0 ) return 1;

//  while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) !=0 )
//  {	
//		if( msg.message == WM_QUIT ) 
//		{	
//			QuitFlag = SWD_QUIT;
//			return 0;
//		}
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}	




	if( c == 0 ) CursorDisplay_Sub( newflag );

	return 1;
}
*/
void FreeMouseCursor ( void )
{	
	if( McBuf ) GlobalFree( McBuf );
	McBuf = NULL;
}
void LoadMouseCursor( char *filename )
{		
	FILE *fp;
	char  temp[ FILENAME_MAX];
	short no;
	DWORD sysfilelength, size;
	DWORD nOff = 0;
	int   i,j;

	FreeMouseCursor ();
		
	sprintf( temp, "./data/%s", filename );
		
	fp = Fopen( temp, "rb" );
	if( fp == NULL ) return;
		
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR );  // 오프셋 스킵
	sysfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;
			
	MemAlloc(McBuf, sysfilelength );
		
	for( i = 0 ;i < no / 20  ; i ++) // MAX_MOUSE_CURSOR
	{	
		for( j = 0 ; j < 20 ; j ++)
		{
			if( nOff >= sysfilelength )
				Error("mouse cursor Sprite file size too big" );

			fread( &size, 4,1, fp);
			
			fread( &mc[i][j].xl, sizeof( short ), 1, fp);
			fread( &mc[i][j].yl, sizeof( short ), 1, fp);
			fread( &mc[i][j].ox, sizeof( short ), 1, fp);
			fread( &mc[i][j].oy, sizeof( short ), 1, fp);
			fread( &mc[i][j].size, sizeof( unsigned int ), 1, fp);
			
			fread( McBuf + nOff, mc[i][j].size,1 ,fp);
			mc[i][j].img = (char *)McBuf + nOff;
			
			convert565to555( &mc[i][j]);
			nOff += size;

		}	
	}	
	fclose(fp);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void DeleteSkillCursor( LPSKILLCURSOR tt )
{		
	LPSKILLCURSOR  t = g_SkillCursor;

	while( t != NULL )
	{		
		if( t == tt )
		{	
			if( t->ch == Hero ) 
			{
				MouseCursorOnOff = true;
			}

			if( t == g_SkillCursor )
			{	
				t = g_SkillCursor->next;
				MemFree( g_SkillCursor );
		
				g_SkillCursor = t;
				if( t != NULL )	g_SkillCursor->prev = NULL;
				
				return;
			}
			else 
			{
				if ( t->prev != NULL )
					t->prev->next = t->next;
	
				if( t->next != NULL )
					t->next->prev = t->prev;
	
				
   				MemFree( t );
				return;
			}
		}
		t = t->next;
	}		
}	
LPSKILLCURSOR AddSkillCursor( LPCHARACTER ch, int skillno, int cursorno, int x, int y, int repeat )
{
	LPSKILLCURSOR temp = NULL, t;
	cursorno /= 10;
	if( cursorno == 0 ) return NULL;
	
	//-----------------------------------
	
	if( ch == Hero ) MouseCursorOnOff = false;
	
	MemAlloc( temp, sizeof( SKILLCURSOR ));
	
	temp->ch		= ch;
	temp->skillno	= skillno;
	temp->cursorno	= cursorno-1;
	temp->x			= x;
	temp->y			= y;
	temp->curframe	= cursorani[cursorno-1].start;
	temp->repeat	= repeat;
	temp->endtime   = g_curr_time + repeat;
	
	
	temp->prev = NULL;
	temp->next = NULL;
	
	
	
	if( g_SkillCursor == NULL )
	{	
		g_SkillCursor		= temp;
	}	
	else
	{	
		t					= g_SkillCursor;
		g_SkillCursor		= temp;
		g_SkillCursor->next	= t;
		t->prev				= g_SkillCursor;
	}		
	
	return g_SkillCursor;
}	
	
bool StopDoingSkillCursor( LPCHARACTER ch )
{	
	LPSKILLCURSOR t, temp;
	
	t = g_SkillCursor;
	while( t != NULL )
	{		
		if( t->ch == ch )
		{
			temp = t->next; 
			DeleteSkillCursor( t ); 
			t = temp; 
			continue; 
		}	
		t = t->next;
	}			
	return false;
}	
	
	
	
bool IsDoingSkillCursor( LPCHARACTER ch )
{	
	LPSKILLCURSOR t;
	
	t = g_SkillCursor;
	while( t != NULL )
	{		
		if( t->ch == ch )
		{
			return true;
		}	
			
		t = t->next;
	}			
	return false;
}	
	
	
	
	
void OutputSkillCursor( void )
{	
	LPSKILLCURSOR t, temp;
	LPORDERTABLE o;
	
	t = g_SkillCursor;
	while( t != NULL )
	{		
		if( t->curframe >= cursorani[ t->cursorno].end ) 
		{	
			t->repeat--;
			if( t->repeat <= 0 ) 
//			if( g_curr_time > t->endtime )
			{
				temp = t->next; 
				DeleteSkillCursor( t ); 
				t = temp; 
				continue; 
			}
			else
			{
				t->curframe = cursorani[t->cursorno].start;
			}
		}	
			
		t->sp = &mc[ t->cursorno ][ t->curframe ];
			
		if( t->curframe == cursorani[t->cursorno].start )
		{
			if( SkillTable[ t->skillno].Effect_wave1 )
			{
				PlayListAutoSounds( SkillTable[ t->skillno].Effect_wave1, 0, 0, 0 );
			}
		}
		
		
		if( BoxAndDotCrash( Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, t->x, t->y ) )
		{		
			o = &g_OrderInfo.order[ g_OrderInfo.count ];
					
			o->wType		= SPRITETYPE_SKILLCURSOR;
			o->x			= t->x + cursorani[t->cursorno].offx;
			o->y			= t->y + cursorani[t->cursorno].offy;
			o->height		= 0;
			o->offset		= t->y + cursorani[t->cursorno].offy;
			o->lpvData		= (LPVOID)t;
			o->show			= 1;
			o->sort			= 1;
			g_OrderInfo.count++;
		}	
		
		t->curframe ++;
		t = t->next;
	}		
}		
	
/////////////////////////////////////////////////////////////////////////////////////////
	
static int		HEAD, TAIL;
	
void ClearQueue( void )
{	
	HEAD = TAIL = 0;
}
	
int queuein( t_queue *n )
{	
	if( (TAIL+1) %  MAX_MQUEUE != HEAD )
	{	
		memcpy( &aQueue[TAIL], n, sizeof( t_queue ) );
		TAIL ++;
		TAIL = TAIL % MAX_MQUEUE ;
		return 1;
	}	
	return 0;
}	

int queueout( t_queue *n)
{		
	if( TAIL != HEAD )
	{	
		memcpy( n, &aQueue[HEAD], sizeof( t_queue ) );
		HEAD ++;
		HEAD = HEAD % MAX_MQUEUE;
		return 1;
	}	
		
	n = NULL;
	return 0;
}	
	
	
	
	
	
	
	
	
	
		