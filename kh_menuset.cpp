#include "stdafx.h"
#include <ddraw.h>
#include "Hangul.h"
#include "dragon.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "SmallMenuSet.h"
#include "stdio.h"
#include "directsound.h"
#include "directinput.h"
#include "Char.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "skill.h"
#include "MenuNetwork.h"
#include "MouseCursor.h"
#include "CharDataTable.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "NationSys.h"
#include "NationSupport.h"
#include "menudef2.h"
#include "skill_lsw.h"
#include "LineCommand.h"
#include "MagicSys.h"
#include "CombatSys.h"
#include "chat.h"
#include "TextOutMgr.h"
#include "auction.h"
#include "HunterSystem.h"
#include "ExchangeMgr.h"
#include "CReadArgument.h"
#include "MailMgr.h"
#include "ArenaManager.h"

extern void SWD_LOGON_Clicked(const int nMenuIndex);
const int HPMPBarHeight = 91;//HP MP 막대의 길이
const int SPBarWidth	= 118;//HP MP 막대의 길이
const int EXPBarWidth	= 223;//EXP 막대의 길이

extern int refresh_magery_time;
extern int g_LocalWarBegin;			// 011215 LTS
extern bool g_bWarBegin;			// 011215 LTS
extern int g_iScenarioOn;
void DisplayTacGauge(SMENU *SubMenu,int j, const int x, const int y, const int iMax, const int iNow, const int iLv,int refresh,int first_exp);
void CallWisperList(const bool bOpen);
void ExplainAuctionItem( int x, int y, ItemAttr *olg_item ,const bool bIsDurDraw);//020821-2 lsw
extern MakeItem make_item_result;
extern void SendNationPacket(int type);//010730 lsw
extern bool isNationWarfieldServer();
extern int CheckNationStatus(int StatusNo);
extern int WarResultAniCount;
extern RareMain Soksungattr_gm;
extern MakeItem gm_make_item_result;
extern int gm_make_item_result_needitem[2];
extern MakeItem gm_make_item_list[MAX_UPGRADE_RARE];
extern bool DoButtonCheckOfMenu_lsw(int i , int j);
static int iClickPointX=g_pointMouseX, iClickPointY = g_pointMouseY;
extern SMENU SMenu[MAX_MENU];
extern SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];
extern Spr		god_spr[GOD_MENU_IMG_MAX];		// 신전 메뉴 
extern void ::SetRect(RECT &rect,int left, int top, int right, int bottom);    
extern void JustMsg( const char *s, ... );
extern void DisplayMailIconExplain(const int iX, const int iY,const int iMailType);
int WarResultAniCount = 0;


DWORD Convert16to32(const WORD wColor)
{
	return RGB( (wColor>>11 & 0x1f) << 3, 
				(wColor>>6  & 0x1f) << 3,
				(wColor     & 0x1f) << 3);
}

void SMenuReg(int iMainType,int x, int y , int iImageType,int iImageNumber, int iFieldCount,int iCheakType,int iTemp,int iWork,int iKey )
{
	if( iMainType <= 0 || iMainType >= MAX_MENU)
	{
		JustMsg("InitMenu Failed");
		return;
	} 
	SMENU *pMenu = &SMenu[iMainType];
	pMenu->x			=	x;
	pMenu->y			=	y;
	pMenu->nImageNumber	=	iImageNumber;
	pMenu->nImageType	=	iImageType;
	pMenu->nFieldCount	=	iFieldCount;
	pMenu->CheakType	=	iCheakType;
	pMenu->nTemp		=	iTemp;				//전 단계 메뉴 번호를 가지고 있다.
	pMenu->work			=	iWork;				//여러 일을 할 임시 변수
	pMenu->key			=	iKey;				//단축키 역할을 할...
}

void SMenuFTReg(const int iMainType,const int iFieldNo,const int iButtonType,const int iSpecailWillDo,
						const int iX, const int iY,const int iWidth, const int iHeight,
						const int iImageType, const int iRectImage, const int iImageNumber,const int iWillDo)
{
	if( iMainType <= 0 || iMainType >= MAX_MENU)
	{
		JustMsg("InitMenu Failed");
		return;
	} 
	SMENU *pMenu	= &SMenu[iMainType];
	if( iFieldNo >= pMenu->nFieldCount)
	{
		JustMsg("Field Reg Failed %d %d",iMainType,iFieldNo);
		return;
	}
	SFIELD *pField	= &pMenu->nField[iFieldNo];

	pField->nType = iButtonType;
	pField->nSpecialWillDo=iSpecailWillDo;
	pField->x = iX;
	pField->y = iY;
	pField->nRectImage = iRectImage;
	pField->nImageNumber = iImageNumber;
	pField->nImageType = iImageType;
	pField->nWillDo = iWillDo;
	::SetRect(pField->rCheakBox, iX, iY, iX+iWidth, iY+iHeight);
	
	return;
}

void lsw_menu_set()
{
///////////////////////////////////////////////////////////////////////////////////////////	
//010708 lsw 010730 
//전쟁 관리소
	{
	::SMenuReg(MN_NATION_BATTLE_BBS,CENTER_X, 100,NATION_BATTLE_BBS_IMG,0,10);

	//전쟁설명 NATION_NOW			OK		설명창 띄우자.
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nSpecialWillDo=SWD_NATION_DECSRIPT;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].y = 54;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nRectImage = 12;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nImageNumber = 11;
	SMenu[MN_NATION_BATTLE_BBS].nField[0].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[0].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[0].x,SMenu[MN_NATION_BATTLE_BBS].nField[0].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[0].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[0].y+30);

	//전쟁상황 NATION_NOW			OK
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nSpecialWillDo=SWD_NATION_NOW;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nWillDo=MN_NATION_NOW;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].y = 84;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nRectImage = 10;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nImageNumber = 9;
	SMenu[MN_NATION_BATTLE_BBS].nField[2].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[2].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[2].x,SMenu[MN_NATION_BATTLE_BBS].nField[2].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[2].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[2].y+30);

	//전쟁참여 NATION JOIN				OK
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nSpecialWillDo=SWD_NATION_JOIN;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nWillDo=MN_NATION_JOIN;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].y = 114;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nRectImage = 14;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nImageNumber = 13;
	SMenu[MN_NATION_BATTLE_BBS].nField[3].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[3].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[3].x,SMenu[MN_NATION_BATTLE_BBS].nField[3].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[3].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[3].y+30);

	//부대 NATION BUDAE					OK
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nSpecialWillDo=SWD_NATION_BUDAE;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nWillDo=MN_NATION_BUDAE;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].y = 144;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nRectImage = 4;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nImageNumber = 3;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[4].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x,SMenu[MN_NATION_BATTLE_BBS].nField[4].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[4].y+30);

	//용병			OK
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nSpecialWillDo=SWD_NATION_GUEST;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nWillDo=MN_NATION_GUEST;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].y = 174;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nRectImage = 6;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nImageNumber = 5;
	SMenu[MN_NATION_BATTLE_BBS].nField[5].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[5].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[5].x,SMenu[MN_NATION_BATTLE_BBS].nField[5].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[5].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[5].y+30);

	//전쟁터 이동			OK
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nSpecialWillDo=SWD_NATION_GOTO_WAR_MAP;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nWillDo=MN_NATION_GOTO_WAR_MAP;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].y = 234;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nRectImage = 16;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nImageNumber = 15;
	SMenu[MN_NATION_BATTLE_BBS].nField[6].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[6].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[6].x,SMenu[MN_NATION_BATTLE_BBS].nField[6].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[6].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[6].y+30);

	//전쟁 기념관			
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nSpecialWillDo=SWD_NATION_LADDER;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nWillDo=MN_NATION_LADDER;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].y = 264;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nRectImage = 8;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nImageNumber = 7;
	SMenu[MN_NATION_BATTLE_BBS].nField[7].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[7].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[7].x,SMenu[MN_NATION_BATTLE_BBS].nField[7].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[7].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[7].y+30);

	//exit 버튼
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].x = 69;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].y =307;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nRectImage = 18;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nImageNumber = 17;
	SMenu[MN_NATION_BATTLE_BBS].nField[8].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[8].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[8].x,SMenu[MN_NATION_BATTLE_BBS].nField[8].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[8].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[8].y+30);

	//전쟁 무기 구입
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nSpecialWillDo=SWD_NATION_WEAPON_BUY;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].x = 42;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].y =204;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nRectImage = 2;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nImageNumber = 1;
	SMenu[MN_NATION_BATTLE_BBS].nField[9].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[9].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[9].x,SMenu[MN_NATION_BATTLE_BBS].nField[9].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[9].x+110,SMenu[MN_NATION_BATTLE_BBS].nField[9].y+30);


	/*	//전쟁터 정보 NATION_INFO
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nSpecialWillDo=SWD_NATION_INFO;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x = 20;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].y = 70;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nRectImage = 9;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nImageNumber = 10;
	SMenu[MN_NATION_BATTLE_BBS].nField[4].nImageType = NATION_BATTLE_BBS_IMG;
	::SetRect(SMenu[MN_NATION_BATTLE_BBS].nField[4].rCheakBox,
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x,SMenu[MN_NATION_BATTLE_BBS].nField[4].y,
	SMenu[MN_NATION_BATTLE_BBS].nField[4].x+130,SMenu[MN_NATION_BATTLE_BBS].nField[4].y+30);
	*/
}

/////////////////////////////////////////////////
//공격로로 지정 하겠다
//전쟁터 공격로 지정	
	{
	::SMenuReg(MN_NATION_INFO,CENTER_X, 100,NATION_INFO_IMG,0,8);
	
	SMenu[MN_NATION_INFO].nField[0].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_INFO].nField[0].x = 21;
	SMenu[MN_NATION_INFO].nField[0].y = 46;
	SMenu[MN_NATION_INFO].nField[0].nImageNumber = 5;
	SMenu[MN_NATION_INFO].nField[0].nImageType = NATION_INFO_IMG;
	
	//1 번 지정
	SMenu[MN_NATION_INFO].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[1].nSpecialWillDo=SWD_NATION_INFO_SELECT;
	SMenu[MN_NATION_INFO].nField[1].nWillDo=1;
	SMenu[MN_NATION_INFO].nField[1].x = 18+175;
	SMenu[MN_NATION_INFO].nField[1].y = 27+221;
	SMenu[MN_NATION_INFO].nField[1].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[1].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[1].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[1].rCheakBox,SMenu[MN_NATION_INFO].nField[1].x - 10 ,SMenu[MN_NATION_INFO].nField[1].y-10,
		SMenu[MN_NATION_INFO].nField[1].x+24,SMenu[MN_NATION_INFO].nField[1].y+40);

	//2 번 지정
	SMenu[MN_NATION_INFO].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[2].nSpecialWillDo=SWD_NATION_INFO_SELECT;
	SMenu[MN_NATION_INFO].nField[2].nWillDo=2; //nWillDo 에 있는 값으로 공격로를 만든다 
	SMenu[MN_NATION_INFO].nField[2].x = 18+116;
	SMenu[MN_NATION_INFO].nField[2].y = 27+34;
	SMenu[MN_NATION_INFO].nField[2].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[2].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[2].rCheakBox,SMenu[MN_NATION_INFO].nField[2].x - 10 ,SMenu[MN_NATION_INFO].nField[2].y-10,
		SMenu[MN_NATION_INFO].nField[2].x+24,SMenu[MN_NATION_INFO].nField[2].y+40);

	//3 번 지정
	SMenu[MN_NATION_INFO].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[3].nSpecialWillDo=SWD_NATION_INFO_SELECT;
	SMenu[MN_NATION_INFO].nField[3].nWillDo=3;
	SMenu[MN_NATION_INFO].nField[3].x = 18+49;
	SMenu[MN_NATION_INFO].nField[3].y = 27+81;
	SMenu[MN_NATION_INFO].nField[3].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[3].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[3].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[3].rCheakBox,SMenu[MN_NATION_INFO].nField[3].x - 10 ,SMenu[MN_NATION_INFO].nField[3].y-10,
		SMenu[MN_NATION_INFO].nField[3].x+24,SMenu[MN_NATION_INFO].nField[3].y+40);

	//4 번 지정
	SMenu[MN_NATION_INFO].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[4].nSpecialWillDo=SWD_NATION_INFO_SELECT;
	SMenu[MN_NATION_INFO].nField[4].nWillDo=4;
	SMenu[MN_NATION_INFO].nField[4].x = 18+49;
	SMenu[MN_NATION_INFO].nField[4].y = 27+81;
	SMenu[MN_NATION_INFO].nField[4].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[4].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[4].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[4].rCheakBox,SMenu[MN_NATION_INFO].nField[4].x - 10 ,SMenu[MN_NATION_INFO].nField[4].y-10,
		SMenu[MN_NATION_INFO].nField[4].x+24,SMenu[MN_NATION_INFO].nField[4].y+40);


//닫기
	SMenu[MN_NATION_INFO].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_INFO].nField[5].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_INFO].nField[5].x = 384;
	SMenu[MN_NATION_INFO].nField[5].y = 302;
	SMenu[MN_NATION_INFO].nField[5].nRectImage = 3;
	SMenu[MN_NATION_INFO].nField[5].nImageNumber = 2;
	SMenu[MN_NATION_INFO].nField[5].nImageType = NATION_INFO_IMG;
	::SetRect(SMenu[MN_NATION_INFO].nField[5].rCheakBox,SMenu[MN_NATION_INFO].nField[5].x,SMenu[MN_NATION_INFO].nField[5].y,
		SMenu[MN_NATION_INFO].nField[5].x+100,SMenu[MN_NATION_INFO].nField[5].y+30);

	SMenu[MN_NATION_INFO].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_INFO].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_INFO].nField[6].x = 340;
	SMenu[MN_NATION_INFO].nField[6].y = 64;
	SMenu[MN_NATION_INFO].nField[6].nRectImage = 150;
	strcpy(SMenu[MN_NATION_INFO].nField[6].temp, lan->OutputMessage(0,145) );

	// 가만히 서 있는 아이
	SMenu[MN_NATION_INFO].nField[7].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_INFO].nField[7].nWillDo=0;
	SMenu[MN_NATION_INFO].nField[7].x = 18+136;
	SMenu[MN_NATION_INFO].nField[7].y = 27+208;
	SMenu[MN_NATION_INFO].nField[7].nRectImage = 1;
	SMenu[MN_NATION_INFO].nField[7].nImageNumber = 1;
	SMenu[MN_NATION_INFO].nField[7].nImageType = NATION_INFO_IMG;


	}

	//전쟁 시작
	{
	::SMenuReg(MN_NATION_WAR_START,CENTER_X, 100,NATION_WAR_START_IMG,0,10);

	//지도
	SMenu[MN_NATION_WAR_START].nField[0].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_WAR_START].nField[0].x = 21;
	SMenu[MN_NATION_WAR_START].nField[0].y = 46;
	SMenu[MN_NATION_WAR_START].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[0].nImageType = NATION_WAR_START_IMG;

	//공격로
	SMenu[MN_NATION_WAR_START].nField[1].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[1].x = 21;
	SMenu[MN_NATION_WAR_START].nField[1].y = 46;
	SMenu[MN_NATION_WAR_START].nField[1].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[1].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[1].nImageType = NATION_WAR_START_IMG;

	//공격로
	SMenu[MN_NATION_WAR_START].nField[2].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[2].x = 21;
	SMenu[MN_NATION_WAR_START].nField[2].y = 46;
	SMenu[MN_NATION_WAR_START].nField[2].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[2].nImageType = NATION_WAR_START_IMG;

	//공격로
	SMenu[MN_NATION_WAR_START].nField[3].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[3].x = 21;
	SMenu[MN_NATION_WAR_START].nField[3].y = 46;
	SMenu[MN_NATION_WAR_START].nField[3].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[3].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[3].nImageType = NATION_WAR_START_IMG;

	//공격로
	SMenu[MN_NATION_WAR_START].nField[4].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[4].x = 21;
	SMenu[MN_NATION_WAR_START].nField[4].y = 46;
	SMenu[MN_NATION_WAR_START].nField[4].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[4].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[4].nImageType = NATION_WAR_START_IMG;


	//설명서
	SMenu[MN_NATION_WAR_START].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_START].nField[5].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_WAR_START].nField[5].x = 340;
	SMenu[MN_NATION_WAR_START].nField[5].y = 64;
	SMenu[MN_NATION_WAR_START].nField[5].nRectImage = 150;
//010927 lsw
//	strcpy(SMenu[MN_NATION_WAR_START].nField[4].temp, lan->OutputMessage(0,145) );
	
	//닫기
	SMenu[MN_NATION_WAR_START].nField[6].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_WAR_START].nField[6].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_WAR_START].nField[6].x = 384;
	SMenu[MN_NATION_WAR_START].nField[6].y = 302;
	SMenu[MN_NATION_WAR_START].nField[6].nRectImage = 3;
	SMenu[MN_NATION_WAR_START].nField[6].nImageNumber = 2;
	SMenu[MN_NATION_WAR_START].nField[6].nImageType = NATION_WAR_START_IMG;
	::SetRect(SMenu[MN_NATION_WAR_START].nField[6].rCheakBox,SMenu[MN_NATION_WAR_START].nField[6].x,SMenu[MN_NATION_WAR_START].nField[6].y,
		SMenu[MN_NATION_WAR_START].nField[6].x+100,SMenu[MN_NATION_WAR_START].nField[6].y+30);

	//방어자의 공격로
	SMenu[MN_NATION_WAR_START].nField[7].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_WAR_START].nField[7].x = 21;
	SMenu[MN_NATION_WAR_START].nField[7].y = 46;
	SMenu[MN_NATION_WAR_START].nField[7].nWillDo = 0;
	SMenu[MN_NATION_WAR_START].nField[7].nImageNumber = 1;
	SMenu[MN_NATION_WAR_START].nField[7].nImageType = NATION_WAR_START_IMG;

	}
//투표 완료
////////////////////////////////////////////////////////////////////////
//사령관 투표
	{
	::SMenuReg(MN_NATION_VOTE,CENTER_X, 100,NATION_VOTE_IMG,0,20);
		
	//메뉴 종료
	SMenu[MN_NATION_VOTE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_VOTE].nField[0].x = 121;
	SMenu[MN_NATION_VOTE].nField[0].y = 305;
	SMenu[MN_NATION_VOTE].nField[0].nRectImage = 6;
	SMenu[MN_NATION_VOTE].nField[0].nImageNumber = 5;
	SMenu[MN_NATION_VOTE].nField[0].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[0].rCheakBox,SMenu[MN_NATION_VOTE].nField[0].x,SMenu[MN_NATION_VOTE].nField[0].y,
		SMenu[MN_NATION_VOTE].nField[0].x+50,SMenu[MN_NATION_VOTE].nField[0].y+30);
	
	//지지 하기 gg아님 ㅡ.ㅡ
	SMenu[MN_NATION_VOTE].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[1].nSpecialWillDo=SWD_NATION_VOTE_SELECT_YOU;
	SMenu[MN_NATION_VOTE].nField[1].x = 118;
	SMenu[MN_NATION_VOTE].nField[1].y = 260;
	SMenu[MN_NATION_VOTE].nField[1].nRectImage = 4;
	SMenu[MN_NATION_VOTE].nField[1].nImageNumber = 3;
	SMenu[MN_NATION_VOTE].nField[1].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[1].rCheakBox,SMenu[MN_NATION_VOTE].nField[1].x,SMenu[MN_NATION_VOTE].nField[1].y,
		SMenu[MN_NATION_VOTE].nField[1].x+35,SMenu[MN_NATION_VOTE].nField[1].y+30);

	//등록 하기 ( 내가 사령관 할래)
	SMenu[MN_NATION_VOTE].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[2].nSpecialWillDo=SWD_NATION_VOTE_I_AM_HERO;
	SMenu[MN_NATION_VOTE].nField[2].x = 49;
	SMenu[MN_NATION_VOTE].nField[2].y = 260;
	SMenu[MN_NATION_VOTE].nField[2].nRectImage = 2;
	SMenu[MN_NATION_VOTE].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_VOTE].nField[2].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[2].rCheakBox,SMenu[MN_NATION_VOTE].nField[2].x,SMenu[MN_NATION_VOTE].nField[2].y,
		SMenu[MN_NATION_VOTE].nField[2].x+50,SMenu[MN_NATION_VOTE].nField[2].y+50);

	//이전
	SMenu[MN_NATION_VOTE].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[3].nSpecialWillDo=SWD_NATION_VOTE_PREV_REFRESH;
	SMenu[MN_NATION_VOTE].nField[3].nWillDo	=	-1;
	SMenu[MN_NATION_VOTE].nField[3].x = 39;
	SMenu[MN_NATION_VOTE].nField[3].y = 113;
	SMenu[MN_NATION_VOTE].nField[3].nRectImage = 8;
	SMenu[MN_NATION_VOTE].nField[3].nImageNumber = 7;
	SMenu[MN_NATION_VOTE].nField[3].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[3].rCheakBox,SMenu[MN_NATION_VOTE].nField[3].x,SMenu[MN_NATION_VOTE].nField[3].y,
		SMenu[MN_NATION_VOTE].nField[3].x+50,SMenu[MN_NATION_VOTE].nField[3].y+50);
	//다음
	SMenu[MN_NATION_VOTE].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_VOTE].nField[4].nSpecialWillDo=SWD_NATION_VOTE_NEXT_REFRESH;
	SMenu[MN_NATION_VOTE].nField[4].nWillDo=	1;
	SMenu[MN_NATION_VOTE].nField[4].x = 151;
	SMenu[MN_NATION_VOTE].nField[4].y = 113;
	SMenu[MN_NATION_VOTE].nField[4].nRectImage = 10;
	SMenu[MN_NATION_VOTE].nField[4].nImageNumber = 9;
	SMenu[MN_NATION_VOTE].nField[4].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[4].rCheakBox,SMenu[MN_NATION_VOTE].nField[4].x,SMenu[MN_NATION_VOTE].nField[4].y,
		SMenu[MN_NATION_VOTE].nField[4].x+50,SMenu[MN_NATION_VOTE].nField[4].y+50);

	//글자 뜰 곳들
	SMenu[MN_NATION_VOTE].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[5].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[5].x = 70;
	SMenu[MN_NATION_VOTE].nField[5].y = 165;
	SMenu[MN_NATION_VOTE].nField[5].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[5].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[5].rCheakBox,SMenu[MN_NATION_VOTE].nField[5].x,SMenu[MN_NATION_VOTE].nField[5].y,
		SMenu[MN_NATION_VOTE].nField[5].x+100,SMenu[MN_NATION_VOTE].nField[5].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[5].temp,  "1번째 칸" );

	SMenu[MN_NATION_VOTE].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[6].x = 70;
	SMenu[MN_NATION_VOTE].nField[6].y = 182;
	SMenu[MN_NATION_VOTE].nField[6].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[6].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[6].rCheakBox,SMenu[MN_NATION_VOTE].nField[6].x,SMenu[MN_NATION_VOTE].nField[6].y,
		SMenu[MN_NATION_VOTE].nField[6].x+100,SMenu[MN_NATION_VOTE].nField[6].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[6].temp,  "2번째 칸" );

		SMenu[MN_NATION_VOTE].nField[7].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[7].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[7].x = 70;
	SMenu[MN_NATION_VOTE].nField[7].y = 200;
	SMenu[MN_NATION_VOTE].nField[7].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[7].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[7].rCheakBox,SMenu[MN_NATION_VOTE].nField[7].x,SMenu[MN_NATION_VOTE].nField[7].y,
		SMenu[MN_NATION_VOTE].nField[7].x+100,SMenu[MN_NATION_VOTE].nField[7].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[7].temp,  "3번째 칸" );

	SMenu[MN_NATION_VOTE].nField[8].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[8].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[8].x = 103;
	SMenu[MN_NATION_VOTE].nField[8].y = 221;
	SMenu[MN_NATION_VOTE].nField[8].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[8].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[8].rCheakBox,SMenu[MN_NATION_VOTE].nField[8].x,SMenu[MN_NATION_VOTE].nField[8].y,
		SMenu[MN_NATION_VOTE].nField[8].x+100,SMenu[MN_NATION_VOTE].nField[8].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[8].temp,  "FAME" );

	SMenu[MN_NATION_VOTE].nField[9].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[9].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[9].x = 103;
	SMenu[MN_NATION_VOTE].nField[9].y = 237;
	SMenu[MN_NATION_VOTE].nField[9].nRectImage = 150;
	SMenu[MN_NATION_VOTE].nField[9].nImageType = NATION_VOTE_IMG;
	::SetRect(SMenu[MN_NATION_VOTE].nField[9].rCheakBox,SMenu[MN_NATION_VOTE].nField[9].x,SMenu[MN_NATION_VOTE].nField[9].y,
		SMenu[MN_NATION_VOTE].nField[9].x+100,SMenu[MN_NATION_VOTE].nField[9].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[9].temp,  "LADDER" );

	//투표 대상의 얼굴.
	SMenu[MN_NATION_VOTE].nField[10].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[10].nImageNumber = DP_FACE_PUT;
	SMenu[MN_NATION_VOTE].nField[10].nImageType = 0;
	SMenu[MN_NATION_VOTE].nField[10].nRectImage = 0;
	SMenu[MN_NATION_VOTE].nField[10].x = 79;
	SMenu[MN_NATION_VOTE].nField[10].y = 88;

	SMenu[MN_NATION_VOTE].nField[11].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[11].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[11].x = 210;
	SMenu[MN_NATION_VOTE].nField[11].y = 100;
	SMenu[MN_NATION_VOTE].nField[11].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[11].rCheakBox,SMenu[MN_NATION_VOTE].nField[11].x,SMenu[MN_NATION_VOTE].nField[11].y,
		SMenu[MN_NATION_VOTE].nField[11].x+100,SMenu[MN_NATION_VOTE].nField[11].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[11].temp,  "애들 리스트1" );

	SMenu[MN_NATION_VOTE].nField[12].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[12].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[12].x = 210;
	SMenu[MN_NATION_VOTE].nField[12].y = 130;
	SMenu[MN_NATION_VOTE].nField[12].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[12].rCheakBox,SMenu[MN_NATION_VOTE].nField[12].x,SMenu[MN_NATION_VOTE].nField[12].y,
		SMenu[MN_NATION_VOTE].nField[12].x+100,SMenu[MN_NATION_VOTE].nField[12].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[12].temp,  "애들 리스트1" );


	SMenu[MN_NATION_VOTE].nField[13].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[13].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[13].x = 210;
	SMenu[MN_NATION_VOTE].nField[13].y = 160;
	SMenu[MN_NATION_VOTE].nField[13].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[13].rCheakBox,SMenu[MN_NATION_VOTE].nField[13].x,SMenu[MN_NATION_VOTE].nField[13].y,
		SMenu[MN_NATION_VOTE].nField[13].x+100,SMenu[MN_NATION_VOTE].nField[13].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[13].temp,  "애들 리스트1" );


	SMenu[MN_NATION_VOTE].nField[14].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[14].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[14].x = 210;
	SMenu[MN_NATION_VOTE].nField[14].y = 190;
	SMenu[MN_NATION_VOTE].nField[14].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[14].rCheakBox,SMenu[MN_NATION_VOTE].nField[14].x,SMenu[MN_NATION_VOTE].nField[14].y,
		SMenu[MN_NATION_VOTE].nField[14].x+100,SMenu[MN_NATION_VOTE].nField[14].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[14].temp,  "애들 리스트1" );

	SMenu[MN_NATION_VOTE].nField[15].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[15].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[15].x = 210;
	SMenu[MN_NATION_VOTE].nField[15].y = 220;
	SMenu[MN_NATION_VOTE].nField[15].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[15].rCheakBox,SMenu[MN_NATION_VOTE].nField[15].x,SMenu[MN_NATION_VOTE].nField[15].y,
		SMenu[MN_NATION_VOTE].nField[15].x+100,SMenu[MN_NATION_VOTE].nField[15].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[15].temp,  "애들 리스트1" );

	SMenu[MN_NATION_VOTE].nField[16].nType = FT_DATA_PUT;
	SMenu[MN_NATION_VOTE].nField[16].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_VOTE].nField[16].x = 210;
	SMenu[MN_NATION_VOTE].nField[16].y = 250;
	SMenu[MN_NATION_VOTE].nField[16].nRectImage = 150;
	::SetRect(SMenu[MN_NATION_VOTE].nField[16].rCheakBox,SMenu[MN_NATION_VOTE].nField[16].x,SMenu[MN_NATION_VOTE].nField[16].y,
		SMenu[MN_NATION_VOTE].nField[16].x+100,SMenu[MN_NATION_VOTE].nField[16].y+30);
	strcpy(SMenu[MN_NATION_VOTE].nField[16].temp,  "애들 리스트1" );

	}
//부대
////////////////////////////////////////////////////////////////////////
//부대 편성
	{
	::SMenuReg(MN_NATION_BUDAE,CENTER_X, 100,NATION_BUDAE_IMG,0,20);

	//닫기 버튼
	SMenu[MN_NATION_BUDAE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_BUDAE].nField[0].x = 121;
	SMenu[MN_NATION_BUDAE].nField[0].y = 305;
	SMenu[MN_NATION_BUDAE].nField[0].nRectImage = 10;
	SMenu[MN_NATION_BUDAE].nField[0].nImageNumber = 9;
	SMenu[MN_NATION_BUDAE].nField[0].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[0].rCheakBox,SMenu[MN_NATION_BUDAE].nField[0].x,SMenu[MN_NATION_BUDAE].nField[0].y,
		SMenu[MN_NATION_BUDAE].nField[0].x+70,SMenu[MN_NATION_BUDAE].nField[0].y+25);

	//사령관 투표
	SMenu[MN_NATION_BUDAE].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[1].nSpecialWillDo=SWD_NATION_VOTE;
	SMenu[MN_NATION_BUDAE].nField[1].nWillDo=MN_NATION_VOTE;
	SMenu[MN_NATION_BUDAE].nField[1].x = 169;
	SMenu[MN_NATION_BUDAE].nField[1].y = 45;
	SMenu[MN_NATION_BUDAE].nField[1].nRectImage = 12;
	SMenu[MN_NATION_BUDAE].nField[1].nImageNumber = 11;
	SMenu[MN_NATION_BUDAE].nField[1].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[1].rCheakBox,SMenu[MN_NATION_BUDAE].nField[1].x,SMenu[MN_NATION_BUDAE].nField[1].y,
		SMenu[MN_NATION_BUDAE].nField[1].x+70,SMenu[MN_NATION_BUDAE].nField[1].y+25);
	
	//공격로 정보
	SMenu[MN_NATION_BUDAE].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[2].nSpecialWillDo=SWD_NATION_ATTACK_WAY_POINT;
	SMenu[MN_NATION_BUDAE].nField[2].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BUDAE].nField[2].x = 42;
	SMenu[MN_NATION_BUDAE].nField[2].y = 211;
	SMenu[MN_NATION_BUDAE].nField[2].nRectImage = 13;
	SMenu[MN_NATION_BUDAE].nField[2].nImageNumber = 14;
	SMenu[MN_NATION_BUDAE].nField[2].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[2].rCheakBox,SMenu[MN_NATION_BUDAE].nField[2].x,SMenu[MN_NATION_BUDAE].nField[2].y,
		SMenu[MN_NATION_BUDAE].nField[2].x+70,SMenu[MN_NATION_BUDAE].nField[2].y+25);

	//이전
	SMenu[MN_NATION_BUDAE].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[3].nSpecialWillDo=SWD_NATION_BUDAE_PREV_REFRESH;
	SMenu[MN_NATION_BUDAE].nField[3].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BUDAE].nField[3].x = 38;
	SMenu[MN_NATION_BUDAE].nField[3].y = 237;
	SMenu[MN_NATION_BUDAE].nField[3].nRectImage = 8;
	SMenu[MN_NATION_BUDAE].nField[3].nImageNumber = 7;
	SMenu[MN_NATION_BUDAE].nField[3].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[3].rCheakBox,SMenu[MN_NATION_BUDAE].nField[3].x,SMenu[MN_NATION_BUDAE].nField[3].y,
		SMenu[MN_NATION_BUDAE].nField[3].x+40,SMenu[MN_NATION_BUDAE].nField[3].y+25);
	//다음
	SMenu[MN_NATION_BUDAE].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[4].nSpecialWillDo=SWD_NATION_BUDAE_NEXT_REFRESH;
	SMenu[MN_NATION_BUDAE].nField[4].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BUDAE].nField[4].x = 95;
	SMenu[MN_NATION_BUDAE].nField[4].y = 237;
	SMenu[MN_NATION_BUDAE].nField[4].nRectImage = 6;
	SMenu[MN_NATION_BUDAE].nField[4].nImageNumber = 5;
	SMenu[MN_NATION_BUDAE].nField[4].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[4].rCheakBox,SMenu[MN_NATION_BUDAE].nField[4].x,SMenu[MN_NATION_BUDAE].nField[4].y,
		SMenu[MN_NATION_BUDAE].nField[4].x+40,SMenu[MN_NATION_BUDAE].nField[4].y+25);
	//등록
	SMenu[MN_NATION_BUDAE].nField[5].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_BUDAE].nField[5].nSpecialWillDo=SWD_NATION_BUDAE_JOIN;
	SMenu[MN_NATION_BUDAE].nField[5].nWillDo=MN_NATION_INFO;
	SMenu[MN_NATION_BUDAE].nField[5].x = 55;
	SMenu[MN_NATION_BUDAE].nField[5].y = 265;
	SMenu[MN_NATION_BUDAE].nField[5].nRectImage = 4;
	SMenu[MN_NATION_BUDAE].nField[5].nImageNumber = 3;
	SMenu[MN_NATION_BUDAE].nField[5].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[5].rCheakBox,SMenu[MN_NATION_BUDAE].nField[5].x,SMenu[MN_NATION_BUDAE].nField[5].y,
		SMenu[MN_NATION_BUDAE].nField[5].x+70,SMenu[MN_NATION_BUDAE].nField[5].y+25);

	//글자 뜰 곳
	SMenu[MN_NATION_BUDAE].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[6].x = 69;
	SMenu[MN_NATION_BUDAE].nField[6].y = 112;
	SMenu[MN_NATION_BUDAE].nField[6].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[6].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[6].rCheakBox,SMenu[MN_NATION_BUDAE].nField[6].x,SMenu[MN_NATION_BUDAE].nField[6].y,
		SMenu[MN_NATION_BUDAE].nField[6].x+100,SMenu[MN_NATION_BUDAE].nField[6].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[6].temp,  "부대번호" );
	
	SMenu[MN_NATION_BUDAE].nField[7].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_BUDAE].nField[7].nImageNumber = 28;
	SMenu[MN_NATION_BUDAE].nField[7].x = 35;
	SMenu[MN_NATION_BUDAE].nField[7].y = 91;
	SMenu[MN_NATION_BUDAE].nField[7].nImageType = NATION_NOW_IMG;
		
	SMenu[MN_NATION_BUDAE].nField[8].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[8].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[8].x = 35;
	SMenu[MN_NATION_BUDAE].nField[8].y = 130;
	SMenu[MN_NATION_BUDAE].nField[8].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[8].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[8].rCheakBox,SMenu[MN_NATION_BUDAE].nField[8].x,SMenu[MN_NATION_BUDAE].nField[8].y,
		SMenu[MN_NATION_BUDAE].nField[8].x+100,SMenu[MN_NATION_BUDAE].nField[8].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[8].temp,  "부대장이름" );

	SMenu[MN_NATION_BUDAE].nField[9].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[9].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[9].x = 88;
	SMenu[MN_NATION_BUDAE].nField[9].y = 94;
	SMenu[MN_NATION_BUDAE].nField[9].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[9].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[9].rCheakBox,SMenu[MN_NATION_BUDAE].nField[9].x,SMenu[MN_NATION_BUDAE].nField[9].y,
		SMenu[MN_NATION_BUDAE].nField[9].x+100,SMenu[MN_NATION_BUDAE].nField[9].y+30);

	SMenu[MN_NATION_BUDAE].nField[11].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[11].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[11].x = 71;
	SMenu[MN_NATION_BUDAE].nField[11].y = 111;
	SMenu[MN_NATION_BUDAE].nField[11].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[11].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[11].rCheakBox,SMenu[MN_NATION_BUDAE].nField[11].x,SMenu[MN_NATION_BUDAE].nField[11].y,
		SMenu[MN_NATION_BUDAE].nField[11].x+100,SMenu[MN_NATION_BUDAE].nField[11].y+30);
//	strcpy(SMenu[MN_NATION_BUDAE].nField[11].temp,  "제 1 부대" );//부대명
/////////////////////////////
//다른 부대 정보
	SMenu[MN_NATION_BUDAE].nField[12].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[12].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[12].x = 153;
	SMenu[MN_NATION_BUDAE].nField[12].y = 87;
	SMenu[MN_NATION_BUDAE].nField[12].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[12].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[12].rCheakBox,SMenu[MN_NATION_BUDAE].nField[12].x,SMenu[MN_NATION_BUDAE].nField[12].y,
		SMenu[MN_NATION_BUDAE].nField[12].x+100,SMenu[MN_NATION_BUDAE].nField[12].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[12].temp,  "제 1 부대      %d/50" );


	SMenu[MN_NATION_BUDAE].nField[13].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[13].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[13].x = 153;
	SMenu[MN_NATION_BUDAE].nField[13].y = 105;
	SMenu[MN_NATION_BUDAE].nField[13].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[13].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[13].rCheakBox,SMenu[MN_NATION_BUDAE].nField[13].x,SMenu[MN_NATION_BUDAE].nField[13].y,
		SMenu[MN_NATION_BUDAE].nField[13].x+100,SMenu[MN_NATION_BUDAE].nField[13].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[13].temp,  "제 2 부대      %d/40" );


	SMenu[MN_NATION_BUDAE].nField[14].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[14].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[14].x = 153;
	SMenu[MN_NATION_BUDAE].nField[14].y = 125;
	SMenu[MN_NATION_BUDAE].nField[14].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[14].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[14].rCheakBox,SMenu[MN_NATION_BUDAE].nField[14].x,SMenu[MN_NATION_BUDAE].nField[14].y,
		SMenu[MN_NATION_BUDAE].nField[14].x+100,SMenu[MN_NATION_BUDAE].nField[14].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[14].temp,  "제 3 부대      %d/30" );


	SMenu[MN_NATION_BUDAE].nField[15].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[15].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[15].x = 153;
	SMenu[MN_NATION_BUDAE].nField[15].y = 144;
	SMenu[MN_NATION_BUDAE].nField[15].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[15].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[15].rCheakBox,SMenu[MN_NATION_BUDAE].nField[15].x,SMenu[MN_NATION_BUDAE].nField[15].y,
		SMenu[MN_NATION_BUDAE].nField[15].x+100,SMenu[MN_NATION_BUDAE].nField[15].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[15].temp,  "제 4 부대      %d/20" );

	SMenu[MN_NATION_BUDAE].nField[16].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[16].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[16].x = 61;
	SMenu[MN_NATION_BUDAE].nField[16].y = 51;
	SMenu[MN_NATION_BUDAE].nField[16].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[16].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[16].rCheakBox,SMenu[MN_NATION_BUDAE].nField[16].x,SMenu[MN_NATION_BUDAE].nField[16].y,
		SMenu[MN_NATION_BUDAE].nField[16].x+100,SMenu[MN_NATION_BUDAE].nField[16].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[16].temp,  "사령관." );

	SMenu[MN_NATION_BUDAE].nField[17].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[17].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[17].x = 33;
	SMenu[MN_NATION_BUDAE].nField[17].y = 148+3;
	SMenu[MN_NATION_BUDAE].nField[17].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[17].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[17].rCheakBox,SMenu[MN_NATION_BUDAE].nField[17].x,SMenu[MN_NATION_BUDAE].nField[17].y,
		SMenu[MN_NATION_BUDAE].nField[17].x+100,SMenu[MN_NATION_BUDAE].nField[17].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[17].temp,  "레어 속성." );


	SMenu[MN_NATION_BUDAE].nField[18].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[18].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[18].x = 33;
	SMenu[MN_NATION_BUDAE].nField[18].y = 166+3;
	SMenu[MN_NATION_BUDAE].nField[18].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[18].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[18].rCheakBox,SMenu[MN_NATION_BUDAE].nField[18].x,SMenu[MN_NATION_BUDAE].nField[18].y,
		SMenu[MN_NATION_BUDAE].nField[18].x+100,SMenu[MN_NATION_BUDAE].nField[18].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[18].temp,  "레어 속성" );

	SMenu[MN_NATION_BUDAE].nField[19].nType = FT_DATA_PUT;
	SMenu[MN_NATION_BUDAE].nField[19].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_BUDAE].nField[19].x = 33;
	SMenu[MN_NATION_BUDAE].nField[19].y = 184+3;
	SMenu[MN_NATION_BUDAE].nField[19].nRectImage = 150;
	SMenu[MN_NATION_BUDAE].nField[19].nImageType = NATION_BUDAE_IMG;
	::SetRect(SMenu[MN_NATION_BUDAE].nField[19].rCheakBox,SMenu[MN_NATION_BUDAE].nField[19].x,SMenu[MN_NATION_BUDAE].nField[19].y,
		SMenu[MN_NATION_BUDAE].nField[19].x+100,SMenu[MN_NATION_BUDAE].nField[19].y+30);
	strcpy(SMenu[MN_NATION_BUDAE].nField[19].temp,  "레어 속성" );

	//부대 마크 현재 얼굴 이미지로 되어있음
//	SMenu[MN_NATION_BUDAE].nField[11].nType = FT_DATA_PUT;
//	SMenu[MN_NATION_BUDAE].nField[11].nImageNumber = DP_FACEIMAGE;
//	SMenu[MN_NATION_BUDAE].nField[11].x = 34;
//	SMenu[MN_NATION_BUDAE].nField[11].y = 92;

	}
////////////////////////////////////////////////////////////////////////
//	전쟁터 현황 
	{
	::SMenuReg(MN_NATION_NOW,CENTER_X, 100,NATION_NOW_IMG,0,49);

	//닫기
	SMenu[MN_NATION_NOW].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_NOW].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_NOW].nField[0].x = 379;
	SMenu[MN_NATION_NOW].nField[0].y = 302;
	SMenu[MN_NATION_NOW].nField[0].nRectImage = 6;
	SMenu[MN_NATION_NOW].nField[0].nImageNumber = 5;
	SMenu[MN_NATION_NOW].nField[0].nImageType = NATION_NOW_IMG;
	::SetRect(SMenu[MN_NATION_NOW].nField[0].rCheakBox,SMenu[MN_NATION_NOW].nField[0].x,SMenu[MN_NATION_NOW].nField[0].y,
	SMenu[MN_NATION_NOW].nField[0].x+100,SMenu[MN_NATION_NOW].nField[0].y+25);

	//리프레쉬
	SMenu[MN_NATION_NOW].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_NOW].nField[1].nSpecialWillDo=SWD_NATION_NOW_REFRESH;
	SMenu[MN_NATION_NOW].nField[1].x = 432;
	SMenu[MN_NATION_NOW].nField[1].y = 270;
	SMenu[MN_NATION_NOW].nField[1].nRectImage = 8;
	SMenu[MN_NATION_NOW].nField[1].nImageNumber = 7;
	SMenu[MN_NATION_NOW].nField[1].nImageType = NATION_NOW_IMG;
	::SetRect(SMenu[MN_NATION_NOW].nField[1].rCheakBox,SMenu[MN_NATION_NOW].nField[1].x ,SMenu[MN_NATION_NOW].nField[1].y,
	SMenu[MN_NATION_NOW].nField[1].x+80 ,SMenu[MN_NATION_NOW].nField[1].y+30);

	//부대편성
	SMenu[MN_NATION_NOW].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_NOW].nField[2].nSpecialWillDo=SWD_NATION_NOW_BUDAE;
	SMenu[MN_NATION_NOW].nField[2].x = 320;
	SMenu[MN_NATION_NOW].nField[2].y = 270;
	SMenu[MN_NATION_NOW].nField[2].nRectImage = 2;
	SMenu[MN_NATION_NOW].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_NOW].nField[2].nImageType = NATION_NOW_IMG;
	::SetRect(SMenu[MN_NATION_NOW].nField[2].rCheakBox,SMenu[MN_NATION_NOW].nField[2].x ,SMenu[MN_NATION_NOW].nField[2].y,
	SMenu[MN_NATION_NOW].nField[2].x+80 ,SMenu[MN_NATION_NOW].nField[2].y+30);

	//배경 이미지 3개 번호를 바꿔주자.
	SMenu[MN_NATION_NOW].nField[3].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[3].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_NOW].nField[3].x = 20;
	SMenu[MN_NATION_NOW].nField[3].y = 46;
	SMenu[MN_NATION_NOW].nField[3].nImageNumber = 9;
	
	//제목 1개 ( 1개는 메인에 박힌거 씀)(그림 종류는 2개
	SMenu[MN_NATION_NOW].nField[4].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_NOW].nField[4].x = 187;
	SMenu[MN_NATION_NOW].nField[4].y = 0;
	SMenu[MN_NATION_NOW].nField[4].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[4].nImageNumber = 3;//또는 4

	
	// 남은 시간 1개
	SMenu[MN_NATION_NOW].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[5].nImageNumber = DP_REMAIN_TIME_PUT;//시간 풋으로 바꾸자
	SMenu[MN_NATION_NOW].nField[5].x = 415;
	SMenu[MN_NATION_NOW].nField[5].y = 48;
	SMenu[MN_NATION_NOW].nField[5].nRectImage = 150;
	SMenu[MN_NATION_NOW].nField[5].nImageType = NATION_NOW_IMG;

	// 아군 숫자 1개
	SMenu[MN_NATION_NOW].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[6].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[6].x = 406;
	SMenu[MN_NATION_NOW].nField[6].y = 66;
	strcpy(SMenu[MN_NATION_NOW].nField[6].temp,  "아군 숫자" );

	// 적군 숫자 1개
	SMenu[MN_NATION_NOW].nField[7].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[7].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[7].x = 406;
	SMenu[MN_NATION_NOW].nField[7].y = 86;
	strcpy(SMenu[MN_NATION_NOW].nField[7].temp,  "적군 숫자" );

	// 글자 12 개
	SMenu[MN_NATION_NOW].nField[8].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[8].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[8].x = 369;
	SMenu[MN_NATION_NOW].nField[8].y = 119;
	strcpy(SMenu[MN_NATION_NOW].nField[8].temp,  "내용 2 " );

	SMenu[MN_NATION_NOW].nField[9].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[9].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[9].x = 369;
	SMenu[MN_NATION_NOW].nField[9].y = 138;
	strcpy(SMenu[MN_NATION_NOW].nField[9].temp,  "내용 3 " );

	SMenu[MN_NATION_NOW].nField[10].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[10].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[10].x = 332;
	SMenu[MN_NATION_NOW].nField[10].y = 156;
	strcpy(SMenu[MN_NATION_NOW].nField[10].temp,  "내용 4 " );

	SMenu[MN_NATION_NOW].nField[11].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[11].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[11].x = 460;
	SMenu[MN_NATION_NOW].nField[11].y = 119;
	strcpy(SMenu[MN_NATION_NOW].nField[11].temp,  "내용 5 " );

	SMenu[MN_NATION_NOW].nField[12].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[12].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[12].x = 460;
	SMenu[MN_NATION_NOW].nField[12].y = 138;
	strcpy(SMenu[MN_NATION_NOW].nField[12].temp,  "내용 6 " );

	SMenu[MN_NATION_NOW].nField[13].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[13].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[13].x = 423;
	SMenu[MN_NATION_NOW].nField[13].y = 156;
	strcpy(SMenu[MN_NATION_NOW].nField[13].temp,  "내용 7 " );

	SMenu[MN_NATION_NOW].nField[14].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[14].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[14].x = 369;
	SMenu[MN_NATION_NOW].nField[14].y = 189;
	strcpy(SMenu[MN_NATION_NOW].nField[14].temp,  "내용 8 " );

	SMenu[MN_NATION_NOW].nField[15].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[15].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[15].x = 369;
	SMenu[MN_NATION_NOW].nField[15].y = 208;
	strcpy(SMenu[MN_NATION_NOW].nField[15].temp,  "내용 9 " );

	SMenu[MN_NATION_NOW].nField[16].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[16].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[16].x = 332;
	SMenu[MN_NATION_NOW].nField[16].y = 226;
	strcpy(SMenu[MN_NATION_NOW].nField[16].temp,  "내용 10 " );

	SMenu[MN_NATION_NOW].nField[17].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[17].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[17].x = 460;
	SMenu[MN_NATION_NOW].nField[17].y = 189;
	strcpy(SMenu[MN_NATION_NOW].nField[17].temp,  "내용 11 " );

	SMenu[MN_NATION_NOW].nField[18].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[18].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[18].x = 460;
	SMenu[MN_NATION_NOW].nField[18].y = 208;
	strcpy(SMenu[MN_NATION_NOW].nField[18].temp,  "내용 12 " );

	SMenu[MN_NATION_NOW].nField[19].nType = FT_DATA_PUT;
	SMenu[MN_NATION_NOW].nField[19].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_NOW].nField[19].x = 423;
	SMenu[MN_NATION_NOW].nField[19].y = 226;
	strcpy(SMenu[MN_NATION_NOW].nField[19].temp,  "내용 1 " );
	
	// 부대 마크 4개 
	SMenu[MN_NATION_NOW].nField[20].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[20].nType = FT_NOMAL_PUT_BY_NWILLDO_CUSTOM;
	SMenu[MN_NATION_NOW].nField[20].x = 331;
	SMenu[MN_NATION_NOW].nField[20].y = 119;
	SMenu[MN_NATION_NOW].nField[20].nImageNumber = 21;
 
	SMenu[MN_NATION_NOW].nField[21].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[21].nType = FT_NOMAL_PUT_BY_NWILLDO_CUSTOM;
	SMenu[MN_NATION_NOW].nField[21].x = 423;
	SMenu[MN_NATION_NOW].nField[21].y = 119;
	SMenu[MN_NATION_NOW].nField[21].nImageNumber = 22;

	SMenu[MN_NATION_NOW].nField[22].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[22].nType = FT_NOMAL_PUT_BY_NWILLDO_CUSTOM;
	SMenu[MN_NATION_NOW].nField[22].x = 331;
	SMenu[MN_NATION_NOW].nField[22].y = 189;
	SMenu[MN_NATION_NOW].nField[22].nImageNumber = 23;

	SMenu[MN_NATION_NOW].nField[23].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[23].nType = FT_NOMAL_PUT_BY_NWILLDO_CUSTOM;
	SMenu[MN_NATION_NOW].nField[23].x = 423;
	SMenu[MN_NATION_NOW].nField[23].y = 189;
	SMenu[MN_NATION_NOW].nField[23].nImageNumber = 24;
	
	// 성문 4개
	SMenu[MN_NATION_NOW].nField[24].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[24].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[24].x = 19;
	SMenu[MN_NATION_NOW].nField[24].y = 44;
	SMenu[MN_NATION_NOW].nField[24].nImageNumber =13;

	SMenu[MN_NATION_NOW].nField[25].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[25].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[25].x = 19;
	SMenu[MN_NATION_NOW].nField[25].y = 44;
	SMenu[MN_NATION_NOW].nField[25].nImageNumber =13;
	
	SMenu[MN_NATION_NOW].nField[26].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[26].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[26].x = 19;
	SMenu[MN_NATION_NOW].nField[26].y = 44;
	SMenu[MN_NATION_NOW].nField[26].nImageNumber =13;

	SMenu[MN_NATION_NOW].nField[27].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[27].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[27].x = 19;
	SMenu[MN_NATION_NOW].nField[27].y = 44;
	SMenu[MN_NATION_NOW].nField[27].nImageNumber =13;

	//결계석 5개 
	SMenu[MN_NATION_NOW].nField[28].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[28].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[28].x = 19;
	SMenu[MN_NATION_NOW].nField[28].y = 44;
	SMenu[MN_NATION_NOW].nField[28].nImageNumber =12;

	SMenu[MN_NATION_NOW].nField[29].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[29].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[29].x = 19;
	SMenu[MN_NATION_NOW].nField[29].y = 44;
	SMenu[MN_NATION_NOW].nField[29].nImageNumber =12;

	SMenu[MN_NATION_NOW].nField[30].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[30].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[30].x = 19;
	SMenu[MN_NATION_NOW].nField[30].y = 44;
	SMenu[MN_NATION_NOW].nField[30].nImageNumber =12;

	SMenu[MN_NATION_NOW].nField[31].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[31].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[31].x = 19;
	SMenu[MN_NATION_NOW].nField[31].y = 44;
	SMenu[MN_NATION_NOW].nField[31].nImageNumber =12;
	
	SMenu[MN_NATION_NOW].nField[32].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[32].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[32].x = 19;
	SMenu[MN_NATION_NOW].nField[32].y = 44;
	SMenu[MN_NATION_NOW].nField[32].nImageNumber =12;

	//자기 위치 나타냄 //010928 lsw
	SMenu[MN_NATION_NOW].nField[33].nImageType = NATION_NOW_IMG;
	SMenu[MN_NATION_NOW].nField[33].nType = FT_NOMAL_PUT_BY_NWILLDO;
	SMenu[MN_NATION_NOW].nField[33].x = 0;
	SMenu[MN_NATION_NOW].nField[33].y = 0;
	SMenu[MN_NATION_NOW].nField[33].nWillDo = 1;
	SMenu[MN_NATION_NOW].nField[33].nImageNumber =33;

	}
////////////////////////////////////////////////////////////////////////
//전쟁 기념관
	{
	::SMenuReg(MN_NATION_LADDER,CENTER_X, 0,NATION_LADDER_IMG,0,2);
	
	SMenu[MN_NATION_LADDER].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_LADDER].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_LADDER].nField[0].x = 30;
	SMenu[MN_NATION_LADDER].nField[0].y = 200;
	SMenu[MN_NATION_LADDER].nField[0].nRectImage = 1;
	SMenu[MN_NATION_LADDER].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_LADDER].nField[0].nImageType = NATION_LADDER_IMG;
	::SetRect(SMenu[MN_NATION_LADDER].nField[0].rCheakBox,30,200,110,230);

	}
	
////////////////////////////////////////////////////////////////////////	
// 전쟁 참여 투표 
	{

	::SMenuReg(MN_NATION_JOIN,CENTER_X, 100,NATION_JOIN_IMG,0,9);
	
	//닫기
	SMenu[MN_NATION_JOIN].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_JOIN].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_JOIN].nField[0].x = 176;
	SMenu[MN_NATION_JOIN].nField[0].y = 305;
	SMenu[MN_NATION_JOIN].nField[0].nRectImage = 5;
	SMenu[MN_NATION_JOIN].nField[0].nImageNumber = 4;
	SMenu[MN_NATION_JOIN].nField[0].nImageType = NATION_JOIN_IMG;
	::SetRect(SMenu[MN_NATION_JOIN].nField[0].rCheakBox,SMenu[MN_NATION_JOIN].nField[0].x,SMenu[MN_NATION_JOIN].nField[0].y,
		SMenu[MN_NATION_JOIN].nField[0].x+100,SMenu[MN_NATION_JOIN].nField[0].y+30);

	//전쟁 참여 하기 (join)
	SMenu[MN_NATION_JOIN].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_JOIN].nField[1].nSpecialWillDo=SWD_NATION_JOIN_JOIN_WAR;
	SMenu[MN_NATION_JOIN].nField[1].x = 66;
	SMenu[MN_NATION_JOIN].nField[1].y = 305;
	SMenu[MN_NATION_JOIN].nField[1].nRectImage = 7;
	SMenu[MN_NATION_JOIN].nField[1].nImageNumber = 6;
	SMenu[MN_NATION_JOIN].nField[1].nImageType = NATION_JOIN_IMG;
	::SetRect(SMenu[MN_NATION_JOIN].nField[1].rCheakBox,SMenu[MN_NATION_JOIN].nField[1].x,SMenu[MN_NATION_JOIN].nField[1].y,
		SMenu[MN_NATION_JOIN].nField[1].x+100,SMenu[MN_NATION_JOIN].nField[1].y+30);
	
	//그림
	SMenu[MN_NATION_JOIN].nField[2].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_JOIN].nField[2].x = 22;
	SMenu[MN_NATION_JOIN].nField[2].y = 51;
	SMenu[MN_NATION_JOIN].nField[2].nImageNumber = 1;
	SMenu[MN_NATION_JOIN].nField[2].nImageType = NATION_JOIN_IMG;

	SMenu[MN_NATION_JOIN].nField[3].nType = FT_DATA_PUT;
	SMenu[MN_NATION_JOIN].nField[3].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_JOIN].nField[3].x = 37;
	SMenu[MN_NATION_JOIN].nField[3].y = 95;
	SMenu[MN_NATION_JOIN].nField[3].nRectImage = 150;
	//010927 lsw
//	strcpy(SMenu[MN_NATION_JOIN].nField[3].temp,  lan->OutputMessage(0,142) );
	
	//그래프
	SMenu[MN_NATION_JOIN].nField[4].nType = FT_DATA_PUT;
	SMenu[MN_NATION_JOIN].nField[4].nImageNumber = DP_NATION_JOIN_STATUS_BAR;
	SMenu[MN_NATION_JOIN].nField[4].x = 238;
	SMenu[MN_NATION_JOIN].nField[4].y = 275;
	SMenu[MN_NATION_JOIN].nField[4].nRectImage = 8;
	SMenu[MN_NATION_JOIN].nField[4].nWillDo = 0;
	SMenu[MN_NATION_JOIN].nField[4].nImageType = NATION_JOIN_IMG;

	//참여 인원 숫자
	SMenu[MN_NATION_JOIN].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_JOIN].nField[5].nImageNumber = DP_STRING_PUT;
	SMenu[MN_NATION_JOIN].nField[5].x = 274;
	SMenu[MN_NATION_JOIN].nField[5].y = 280;
	SMenu[MN_NATION_JOIN].nField[5].nImageType = NATION_JOIN_IMG;


}
	
	

//////////////////////////////////////////////////////////////////
//010730 lsw 용병 참여
	{
	::SMenuReg(MN_NATION_GUEST,CENTER_X, 100,NATION_GUEST_IMG,0,4);

	//닫기
	SMenu[MN_NATION_GUEST].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GUEST].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_GUEST].nField[0].x = 70;
	SMenu[MN_NATION_GUEST].nField[0].y = 164;
	SMenu[MN_NATION_GUEST].nField[0].nRectImage = 2;
	SMenu[MN_NATION_GUEST].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_GUEST].nField[0].nImageType = NATION_GUEST_IMG;
	::SetRect(SMenu[MN_NATION_GUEST].nField[0].rCheakBox,SMenu[MN_NATION_GUEST].nField[0].x,SMenu[MN_NATION_GUEST].nField[0].y,
		SMenu[MN_NATION_GUEST].nField[0].x+100,SMenu[MN_NATION_GUEST].nField[0].y+30);
	
	//용병 부대 편성
	SMenu[MN_NATION_GUEST].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GUEST].nField[1].nSpecialWillDo=SWD_NATION_GUEST_BUDAE;
	SMenu[MN_NATION_GUEST].nField[1].x = 43;
	SMenu[MN_NATION_GUEST].nField[1].y = 54;
	SMenu[MN_NATION_GUEST].nField[1].nRectImage = 6;
	SMenu[MN_NATION_GUEST].nField[1].nImageNumber = 5;
	SMenu[MN_NATION_GUEST].nField[1].nImageType = NATION_GUEST_IMG;
	::SetRect(SMenu[MN_NATION_GUEST].nField[1].rCheakBox,SMenu[MN_NATION_GUEST].nField[1].x,SMenu[MN_NATION_GUEST].nField[1].y,
		SMenu[MN_NATION_GUEST].nField[1].x+100,SMenu[MN_NATION_GUEST].nField[1].y+30);

	//바이참여
	SMenu[MN_NATION_GUEST].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GUEST].nField[2].nSpecialWillDo=SWD_NATION_GUEST_JOIN;
	SMenu[MN_NATION_GUEST].nField[2].nWillDo=1;
	SMenu[MN_NATION_GUEST].nField[2].x = 43;
	SMenu[MN_NATION_GUEST].nField[2].y = 84;
	SMenu[MN_NATION_GUEST].nField[2].nRectImage = 4;
	SMenu[MN_NATION_GUEST].nField[2].nImageNumber = 3;
	SMenu[MN_NATION_GUEST].nField[2].nImageType = NATION_GUEST_IMG;
	::SetRect(SMenu[MN_NATION_GUEST].nField[2].rCheakBox,SMenu[MN_NATION_GUEST].nField[2].x,SMenu[MN_NATION_GUEST].nField[2].y,
		SMenu[MN_NATION_GUEST].nField[2].x+100,SMenu[MN_NATION_GUEST].nField[2].y+30);

	//자이펀 참여
	SMenu[MN_NATION_GUEST].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GUEST].nField[3].nSpecialWillDo=SWD_NATION_GUEST_JOIN;
	SMenu[MN_NATION_GUEST].nField[3].nWillDo=2;
	SMenu[MN_NATION_GUEST].nField[3].x = 43;
	SMenu[MN_NATION_GUEST].nField[3].y = 114;
	SMenu[MN_NATION_GUEST].nField[3].nRectImage = 8;
	SMenu[MN_NATION_GUEST].nField[3].nImageNumber = 7;
	SMenu[MN_NATION_GUEST].nField[3].nImageType = NATION_GUEST_IMG;
	::SetRect(SMenu[MN_NATION_GUEST].nField[3].rCheakBox,SMenu[MN_NATION_GUEST].nField[3].x,SMenu[MN_NATION_GUEST].nField[3].y,
		SMenu[MN_NATION_GUEST].nField[3].x+100,SMenu[MN_NATION_GUEST].nField[3].y+30);

	}
	

//////////////////////////////////////////////////////////////////////////////////////////
//전쟁터로 이동
	{
	::SMenuReg(MN_NATION_GOTO_WAR_MAP,CENTER_X, 150,NATION_GOTO_WAR_MAP_IMG,0,6);
	
	//닫기
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].x = 70;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].y = 164;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nRectImage = 8;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nImageNumber = 7;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].nImageType = NATION_GOTO_WAR_MAP_IMG;
	::SetRect(SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].rCheakBox,SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].x,SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].y,
		SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].x+100,SMenu[MN_NATION_GOTO_WAR_MAP].nField[0].y+30);
	//전쟁터 1
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nSpecialWillDo=SWD_NATION_GOTO_WAR_MAP_TARGET;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nWillDo=1;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].x = 43;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].y = 54;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nRectImage = 2;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nImageNumber = 1;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].nImageType = NATION_GOTO_WAR_MAP_IMG;
	::SetRect(SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].rCheakBox,SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].x,SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].y,
		SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].x+100,SMenu[MN_NATION_GOTO_WAR_MAP].nField[1].y+30);
	//전쟁터 2
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nSpecialWillDo=SWD_NATION_GOTO_WAR_MAP_TARGET;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nWillDo=2;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].x = 43;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].y = 84;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nRectImage = 4;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nImageNumber = 3;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].nImageType = NATION_GOTO_WAR_MAP_IMG;
	::SetRect(SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].rCheakBox,SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].x,SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].y,
		SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].x+100,SMenu[MN_NATION_GOTO_WAR_MAP].nField[2].y+30);

	//전쟁터 3
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nSpecialWillDo=SWD_NATION_GOTO_WAR_MAP_TARGET;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nWillDo=3;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].x = 43;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].y = 114;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nRectImage = 6;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nImageNumber = 5;
	SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].nImageType = NATION_GOTO_WAR_MAP_IMG;
	::SetRect(SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].rCheakBox,SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].x,SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].y,
		SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].x+100,SMenu[MN_NATION_GOTO_WAR_MAP].nField[3].y+30);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//전쟁터로 이동
	//국가전 메세지 박스(?) yes no
	{
	::SMenuReg(MN_NATION_MSGBOX, CENTER_X, 50, NATION_MSGBOX_IMG,0,3);

	//취소
	SMenu[MN_NATION_MSGBOX].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MSGBOX].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_MSGBOX].nField[0].x = 177;
	SMenu[MN_NATION_MSGBOX].nField[0].y = 305;
	SMenu[MN_NATION_MSGBOX].nField[0].nRectImage = 2;
	SMenu[MN_NATION_MSGBOX].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_MSGBOX].nField[0].nImageType = NATION_MSGBOX_IMG;
	::SetRect(SMenu[MN_NATION_MSGBOX].nField[0].rCheakBox,SMenu[MN_NATION_MSGBOX].nField[0].x,SMenu[MN_NATION_MSGBOX].nField[0].y,
		SMenu[MN_NATION_MSGBOX].nField[0].x+100,SMenu[MN_NATION_MSGBOX].nField[0].y+30);
	
	//오케
	SMenu[MN_NATION_MSGBOX].nField[1].nType =			FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_MSGBOX].nField[1].nSpecialWillDo=	SWD_NATION_MSGBOX_OK;
	SMenu[MN_NATION_MSGBOX].nField[1].nWillDo=0;
	SMenu[MN_NATION_MSGBOX].nField[1].x = 65;
	SMenu[MN_NATION_MSGBOX].nField[1].y = 305;
	SMenu[MN_NATION_MSGBOX].nField[1].nRectImage = 4;
	SMenu[MN_NATION_MSGBOX].nField[1].nImageNumber = 3;
	SMenu[MN_NATION_MSGBOX].nField[1].nImageType = NATION_MSGBOX_IMG;
	::SetRect(SMenu[MN_NATION_MSGBOX].nField[1].rCheakBox,SMenu[MN_NATION_MSGBOX].nField[1].x,SMenu[MN_NATION_MSGBOX].nField[1].y,
		SMenu[MN_NATION_MSGBOX].nField[1].x+100,SMenu[MN_NATION_MSGBOX].nField[1].y+30);

	//설명서
	SMenu[MN_NATION_MSGBOX].nField[2].nType = FT_DATA_PUT;
	SMenu[MN_NATION_MSGBOX].nField[2].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_MSGBOX].nField[2].x = 40;
	SMenu[MN_NATION_MSGBOX].nField[2].y = 60;
	SMenu[MN_NATION_MSGBOX].nField[2].nRectImage = 220;
	SMenu[MN_NATION_MSGBOX].nField[2].nImageType = NATION_MSGBOX_IMG;
	::SetRect(SMenu[MN_NATION_MSGBOX].nField[2].rCheakBox,SMenu[MN_NATION_MSGBOX].nField[0].x,SMenu[MN_NATION_MSGBOX].nField[0].y,
		SMenu[MN_NATION_MSGBOX].nField[2].x+100,SMenu[MN_NATION_MSGBOX].nField[0].y+30);
	strcpy(SMenu[MN_NATION_MSGBOX].nField[2].temp,  "이 크크크.. ㅡ.ㅡ;; 으하하하하하핳~" );
	}
////////////////////////////////////////////////////////////////////
// 던 메세지(DONE)
	{
	::SMenuReg(MN_NATION_DONEBOX, CENTER_X, 100, NATION_DONEBOX_IMG,0,11);
	
	//취소
	SMenu[MN_NATION_DONEBOX].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_DONEBOX].nField[0].nSpecialWillDo=SWD_SUBMENU_BACK;
	SMenu[MN_NATION_DONEBOX].nField[0].x = 123;
	SMenu[MN_NATION_DONEBOX].nField[0].y = 305;
	SMenu[MN_NATION_DONEBOX].nField[0].nRectImage = 2;
	SMenu[MN_NATION_DONEBOX].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_DONEBOX].nField[0].nImageType = NATION_DONEBOX_IMG;
	::SetRect(SMenu[MN_NATION_DONEBOX].nField[0].rCheakBox,SMenu[MN_NATION_DONEBOX].nField[0].x,SMenu[MN_NATION_DONEBOX].nField[0].y,
		SMenu[MN_NATION_DONEBOX].nField[0].x+100,SMenu[MN_NATION_DONEBOX].nField[0].y+30);
	
	//설명서
	SMenu[MN_NATION_DONEBOX].nField[1].nType = FT_DATA_PUT;
	SMenu[MN_NATION_DONEBOX].nField[1].nImageNumber = DP_RECT_STRING_PUT;
	SMenu[MN_NATION_DONEBOX].nField[1].x = 40;
	SMenu[MN_NATION_DONEBOX].nField[1].y = 60;
	SMenu[MN_NATION_DONEBOX].nField[1].nRectImage = 250;
	SMenu[MN_NATION_DONEBOX].nField[1].nImageType = NATION_DONEBOX_IMG;
	strcpy(SMenu[MN_NATION_DONEBOX].nField[1].temp,  "내용 로딩중~입니다 길이에 맞게.." );
	}

	//투데이 노티스..
	{
	::SMenuReg(MN_NATION_TODAY_NOTICE,CENTER_X,100, NATION_TODAY_NOTICE_IMG, 0,15);
	
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].x = 121;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].y = 305;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nRectImage = 2;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nImageNumber = 1;
	SMenu[MN_NATION_TODAY_NOTICE].nField[0].nImageType = NATION_TODAY_NOTICE_IMG;
	::SetRect(SMenu[MN_NATION_TODAY_NOTICE].nField[0].rCheakBox,SMenu[MN_NATION_TODAY_NOTICE].nField[0].x,SMenu[MN_NATION_TODAY_NOTICE].nField[0].y,
		SMenu[MN_NATION_TODAY_NOTICE].nField[0].x+100,SMenu[MN_NATION_TODAY_NOTICE].nField[0].y+30);
	
	//1번 그림 바닥에 깔리는것
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT	;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nSpecialWillDo=SWD_NATION_TODAY_NOTICE_IMAGE;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nImageNumber	= 9;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nRectImage		= 10;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].x = 23;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].y = 55;
	SMenu[MN_NATION_TODAY_NOTICE].nField[1].nImageType = NATION_TODAY_NOTICE_IMG;
	::SetRect(SMenu[MN_NATION_TODAY_NOTICE].nField[1].rCheakBox,SMenu[MN_NATION_TODAY_NOTICE].nField[1].x,SMenu[MN_NATION_TODAY_NOTICE].nField[1].y,
		SMenu[MN_NATION_TODAY_NOTICE].nField[1].x+80,SMenu[MN_NATION_TODAY_NOTICE].nField[1].y+55);
	
	//2번 그림 바닥에 깔리는것
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT	;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nSpecialWillDo=SWD_NATION_TODAY_NOTICE_IMAGE;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nImageNumber	= 9;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nRectImage		= 10;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].x = 120;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].y = 55;
	SMenu[MN_NATION_TODAY_NOTICE].nField[2].nImageType = NATION_TODAY_NOTICE_IMG;
	::SetRect(SMenu[MN_NATION_TODAY_NOTICE].nField[2].rCheakBox,SMenu[MN_NATION_TODAY_NOTICE].nField[2].x,SMenu[MN_NATION_TODAY_NOTICE].nField[2].y,
		SMenu[MN_NATION_TODAY_NOTICE].nField[2].x+80,SMenu[MN_NATION_TODAY_NOTICE].nField[2].y+55);
	
	//3번 그림 바닥에 깔리는것
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT	;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nSpecialWillDo=SWD_NATION_TODAY_NOTICE_IMAGE;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nImageNumber	= 9;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nRectImage		= 10;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].x = 217;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].y = 55;
	SMenu[MN_NATION_TODAY_NOTICE].nField[3].nImageType = NATION_TODAY_NOTICE_IMG;
	::SetRect(SMenu[MN_NATION_TODAY_NOTICE].nField[3].rCheakBox,SMenu[MN_NATION_TODAY_NOTICE].nField[3].x,SMenu[MN_NATION_TODAY_NOTICE].nField[3].y,
		SMenu[MN_NATION_TODAY_NOTICE].nField[3].x+80,SMenu[MN_NATION_TODAY_NOTICE].nField[3].y+55);
	
	//내용 1 2 3 
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].nType = FT_DATA_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].nImageNumber = DP_STRING_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].x = 63;
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].y = 117;
	SMenu[MN_NATION_TODAY_NOTICE].nField[4].nRectImage = 200;
	
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].nImageNumber = DP_STRING_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].x = 160;
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].y = 117;
	SMenu[MN_NATION_TODAY_NOTICE].nField[5].nRectImage = 200;
	
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].nImageNumber = DP_STRING_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].x = 257;
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].y = 117;
	SMenu[MN_NATION_TODAY_NOTICE].nField[6].nRectImage = 200;
	
	//설명서
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].nType = FT_DATA_PUT;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].nImageNumber = DP_RECT_STRING_PUT_CUSTOM;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].x = 33;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].y = 148;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].nRectImage = 260;
	SMenu[MN_NATION_TODAY_NOTICE].nField[7].nImageType = 15;
	strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[7].temp,  " " );
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//국가전의결과를 보여줌
	{
	::SMenuReg(MN_NATION_WAR_RESULT, CENTER_X, 30, NATION_WAR_RESULT_IMG,0,11);//승리
//	::SMenuReg(MN_NATION_WAR_RESULT, 150, 100, NATION_WAR_RESULT_IMG,4,3);//패배

	SMenu[MN_NATION_WAR_RESULT].nField[0].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[0].nImageNumber = DP_NATION_WAR_RESULT_ANIMATION;
	SMenu[MN_NATION_WAR_RESULT].nField[0].x = 180;
	SMenu[MN_NATION_WAR_RESULT].nField[0].y = 250;
	SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 0;	//이겼나 졌나
	SMenu[MN_NATION_WAR_RESULT].nField[0].nSpecialWillDo= 0;//루프 돌까 말까( FF >> 는 2 REW << 는 -2
	SMenu[MN_NATION_WAR_RESULT].nField[0].nWillDo		= 0;//루프 시작점
	SMenu[MN_NATION_WAR_RESULT].nField[0].nRectImage	= 0;//루프의 끝점

	//바이서스 사망자
	SMenu[MN_NATION_WAR_RESULT].nField[1].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[1].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[1].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[1].y = 98;
	SMenu[MN_NATION_WAR_RESULT].nField[1].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[1].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[1].nRectImage = 9;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[1].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[1].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);
	
	//자이펀 사망자
	SMenu[MN_NATION_WAR_RESULT].nField[2].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[2].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[2].y = 139;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nRectImage = 8;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[2].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[2].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);
	
	//바이서스 참여자
	SMenu[MN_NATION_WAR_RESULT].nField[3].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[3].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[3].y = 183;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nRectImage = 9;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[3].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[3].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);
	
	//바이서스 - 일스 참여자
	SMenu[MN_NATION_WAR_RESULT].nField[4].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[4].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[4].y = 228;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nRectImage = 13;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[4].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[4].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);

	//자이펀 참여자
	SMenu[MN_NATION_WAR_RESULT].nField[5].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[5].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[5].y = 270;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nRectImage = 8;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[5].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[5].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);

	//자이펀 - 일스 참여자
	SMenu[MN_NATION_WAR_RESULT].nField[6].nType = FT_DATA_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nImageNumber = DP_NATION_WAR_RESULT_BAR;
	SMenu[MN_NATION_WAR_RESULT].nField[6].x = 516;		// BBD 040313
	SMenu[MN_NATION_WAR_RESULT].nField[6].y = 312;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nWillDo = 100;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nSpecialWillDo = 0;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nRectImage = 12;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[6].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[0].x,SMenu[MN_NATION_WAR_RESULT].nField[0].y,
		SMenu[MN_NATION_WAR_RESULT].nField[6].x+100,SMenu[MN_NATION_WAR_RESULT].nField[0].y+30);

	//승리국가 이름 ( 배경에 붙은거 말고 )
	SMenu[MN_NATION_WAR_RESULT].nField[7].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 1;
	SMenu[MN_NATION_WAR_RESULT].nField[7].nImageType = NATION_WAR_RESULT_IMG;

	//Done
	SMenu[MN_NATION_WAR_RESULT].nField[8].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[8].nSpecialWillDo=SWD_NATION_WAR_RESULT;
	SMenu[MN_NATION_WAR_RESULT].nField[8].x = 294;
	SMenu[MN_NATION_WAR_RESULT].nField[8].y = 349;
	SMenu[MN_NATION_WAR_RESULT].nField[8].nRectImage = 6;
	SMenu[MN_NATION_WAR_RESULT].nField[8].nImageNumber = 5;
	SMenu[MN_NATION_WAR_RESULT].nField[8].nImageType = NATION_WAR_RESULT_IMG;
	::SetRect(SMenu[MN_NATION_WAR_RESULT].nField[8].rCheakBox,SMenu[MN_NATION_WAR_RESULT].nField[8].x,SMenu[MN_NATION_WAR_RESULT].nField[8].y,
		SMenu[MN_NATION_WAR_RESULT].nField[8].x+200,SMenu[MN_NATION_WAR_RESULT].nField[8].y+50);

	//<! BBD 040311 참여자 글자 이미지
	SMenu[MN_NATION_WAR_RESULT].nField[9].nType = FT_NOMAL_PUT;
	SMenu[MN_NATION_WAR_RESULT].nField[9].x = 380;
	SMenu[MN_NATION_WAR_RESULT].nField[9].y = 84;
	SMenu[MN_NATION_WAR_RESULT].nField[9].nImageNumber = 19;
	SMenu[MN_NATION_WAR_RESULT].nField[9].nImageType = NATION_WAR_RESULT_IMG;
	//> BBD 040311 참여자 글자 이미지

	}
	//011010 lsw >
	{
	const int iMnIndex	= MN_TUTORIAL_DISPLAY;
	const int iImg		= TUTORIAL_DISPLAY_IMG;
	::SMenuReg(iMnIndex, CENTER_X, 30, TUTORIAL_DISPLAY_IMG,0,2);
	//버튼
	::SMenuFTReg(iMnIndex, 0, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
		0, 0, 200, 50,TUTORIAL_DISPLAY_TMP_IMG, 2, 0);
	}
	//011010 lsw <
	{//011012 lsw 
	::SMenuReg(MN_ITEM_MAKE_SELECT, CENTER_X, 20+GABY_SCREEN, ITEM_MAKE_RARE_SELECT_IMG,0,20);

	// ok, cancel  Button 
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].nSpecialWillDo=SWD_ITEM_MAKE_SELECT_OK;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].x=26;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].y=270;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].nRectImage=294;
	SMenu[MN_ITEM_MAKE_SELECT].nField[0].nImageNumber=295;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[0].rCheakBox,26,270,66 ,310);

	SMenu[MN_ITEM_MAKE_SELECT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].nSpecialWillDo=SWD_SKILL_CANCEL;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].x=180;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].y=270;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].nRectImage=292;
	SMenu[MN_ITEM_MAKE_SELECT].nField[1].nImageNumber=293;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[1].rCheakBox,180,270,220,310);

	// 아이템 화면 출력
	//011030 lsw >
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].nType=FT_ITEM_MAKE_SELECT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].x=32;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].y=51;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].nSHideNomalCount=100;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].nSHideNomalStart=0;
	SMenu[MN_ITEM_MAKE_SELECT].nField[2].nShideNomalPlus=1;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[2].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[2].x,SMenu[MN_ITEM_MAKE_SELECT].nField[2].y
		,SMenu[MN_ITEM_MAKE_SELECT].nField[2].x+210,SMenu[MN_ITEM_MAKE_SELECT].nField[2].y+150);
	//011030 lsw <
	// scroll button
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].nWillDo=2;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].x=171;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].y=117;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].nRectImage=641;
	SMenu[MN_ITEM_MAKE_SELECT].nField[3].nImageNumber=642;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[3].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[3].x,
		SMenu[MN_ITEM_MAKE_SELECT].nField[3].y,
		SMenu[MN_ITEM_MAKE_SELECT].nField[3].x+30,
		SMenu[MN_ITEM_MAKE_SELECT].nField[3].y+20);
	
	// down
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].nWillDo=2;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].x=200;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].y=117;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].nRectImage=643;
	SMenu[MN_ITEM_MAKE_SELECT].nField[4].nImageNumber=644;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[4].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[4].x,
		SMenu[MN_ITEM_MAKE_SELECT].nField[4].y,
		SMenu[MN_ITEM_MAKE_SELECT].nField[4].x+30,
		SMenu[MN_ITEM_MAKE_SELECT].nField[4].y+20);

	// 제목 띄우기	
	SMenu[MN_ITEM_MAKE_SELECT].nField[5].nType=FT_NOMAL_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[5].x=80;
	SMenu[MN_ITEM_MAKE_SELECT].nField[5].y=6;
	SMenu[MN_ITEM_MAKE_SELECT].nField[5].nImageNumber=640;		//상황에 따라 바뀜

	// 선택 아이템 출력
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].x=30;
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].y=135;
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].nRectImage=155;
	SMenu[MN_ITEM_MAKE_SELECT].nField[6].nImageNumber=DP_SELECT_SELECT;
	
	//레어 버튼
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nType=			FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nSpecialWillDo=SWD_ITEM_MAKE_SELECT_RARE;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].x=160;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].y=233;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nRectImage=5;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nImageNumber=5;
	SMenu[MN_ITEM_MAKE_SELECT].nField[7].nImageType = ITEM_MAKE_RARE_SELECT_IMG;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[7].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[7].x,SMenu[MN_ITEM_MAKE_SELECT].nField[7].y
		,SMenu[MN_ITEM_MAKE_SELECT].nField[7].x+80,SMenu[MN_ITEM_MAKE_SELECT].nField[7].y+30);

	// scroll button 갯수 출력 버튼
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].nType=FT_MAKE_RARE_MORE;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].nWillDo=1;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].x=114;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].y=268;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].nRectImage=642;
	SMenu[MN_ITEM_MAKE_SELECT].nField[8].nImageNumber=641;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[8].rCheakBox,
		SMenu[MN_ITEM_MAKE_SELECT].nField[8].x,	SMenu[MN_ITEM_MAKE_SELECT].nField[8].y,
		SMenu[MN_ITEM_MAKE_SELECT].nField[8].x+30,	SMenu[MN_ITEM_MAKE_SELECT].nField[8].y+16);
	// down
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].nType=FT_MAKE_RARE_MORE;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].nWillDo=-1;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].x=142;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].y=268;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].nRectImage=644;
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].nImageNumber=643;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[9].rCheakBox,
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].x,	SMenu[MN_ITEM_MAKE_SELECT].nField[9].y,
	SMenu[MN_ITEM_MAKE_SELECT].nField[9].x+30,	SMenu[MN_ITEM_MAKE_SELECT].nField[9].y+16);

	
	//만들 개수 디스 플레이
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].nType = FT_DATA_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].nImageNumber = DP_RECT_STRING_PUT;//011218 lsw
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].x = 90;
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].y = 270;
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].nRectImage = 150;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[10].rCheakBox,SMenu[MN_ITEM_MAKE_SELECT].nField[10].x,SMenu[MN_ITEM_MAKE_SELECT].nField[10].y,
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].x+100,SMenu[MN_ITEM_MAKE_SELECT].nField[10].y+30);
	strcpy(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,  "0" );

	// Max
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nType=FT_MAKE_RARE_MORE;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nWillDo=0;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].x=100;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].y=289;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nRectImage=2;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nImageNumber=1;
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].nImageType = ITEM_MAKE_RARE_SELECT_IMG;
	::SetRect(SMenu[MN_ITEM_MAKE_SELECT].nField[11].rCheakBox,
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].x,	SMenu[MN_ITEM_MAKE_SELECT].nField[11].y,
	SMenu[MN_ITEM_MAKE_SELECT].nField[11].x+40,	SMenu[MN_ITEM_MAKE_SELECT].nField[11].y+25);


	SMenu[MN_ITEM_MAKE_SELECT].nField[12].nType = FT_DATA_PUT;
	SMenu[MN_ITEM_MAKE_SELECT].nField[12].nImageNumber = DP_RARE_SUCCESS_RATE_DISPLAY_NO_TOTAL;
	SMenu[MN_ITEM_MAKE_SELECT].nField[12].x = 25;
	SMenu[MN_ITEM_MAKE_SELECT].nField[12].y = 240;
	SMenu[MN_ITEM_MAKE_SELECT].nField[12].nRectImage = 150;
	strcpy(SMenu[MN_ITEM_MAKE_SELECT].nField[12].temp,  "3번째 칸" );
	}
	

	{//020214 lsw
	::SMenuReg(MN_ITEM_MAKE_RARE_SELECT, CENTER_X, 30, ITEM_MAKE_RARE_SELECT_IMG,6,30);
	
	//0번은 안바뀌게 조심 하라// ok, 
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_ITEM_MAKE_SELECT_RARE_OK, 26+45, 270, 40, 40, 0, 294, 295,0);
	//cancel
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_ITEM_MAKE_SELECT_RARE_CANCEL, 180+45, 270, 40, 40, 0, 292, 293,0);

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 2, FT_RARE_GROUP, 0, 25, 60, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,1);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[2].temp,  "" );
	
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 3, FT_RARE_GROUP, 0, 25, 90, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,2);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[3].temp,  "" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 4, FT_RARE_GROUP, 0, 25, 120, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,3);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[4].temp,  "" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 5, FT_RARE_GROUP, 0, 25, 150, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,4);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[5].temp,  "" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 6, FT_RARE_GROUP, 0, 25, 180, 100, 22, ITEM_MAKE_RARE_SELECT_IMG, 9, 4,5);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[6].temp,  "" );

	//스크롤 위로
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 7, FT_RARE_GROUP_SCROLL, 0, 116, 200, 25, 30, ITEM_MAKE_RARE_SELECT_IMG, 7, 7,-1);
	//스크롤 아래로
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 8, FT_RARE_GROUP_SCROLL, 0, 135, 200, 25, 30, ITEM_MAKE_RARE_SELECT_IMG, 8, 8,1);

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 10, FT_DATA_PUT, 0, 25, 243, 25, 30, 0, 150, DP_RARE_SUCCESS_RATE_DISPLAY);
	// 0
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 16, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[16].temp,  "0" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 17, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 80, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[17].temp,  "1" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 18, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+40, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[18].temp,  "2" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 19, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+60, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[19].temp,  "3" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 20, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+80, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[20].temp,  "4" );

	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 21, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+100, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[21].temp,  "5" );
	
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 22, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+120, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[22].temp,  "6" );
	
	::SMenuFTReg(MN_ITEM_MAKE_RARE_SELECT, 23, FT_RARE_GROUP_MEMBER_DISPLAY, 0, 190, 60+140, 80, 11, 0, 200, DP_RARE_SUCCESS_RATE_DISPLAY);
	strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[23].temp,  "7" );

	}
	{
	::SMenuReg(MN_COMBAT_SKILL_POINT, CENTER_X, 40,COMBAT_SKILL_POINT_IMG,0,30);//iWork 는 선택된 그룹을 나타낸다
	//OK		01// 6개 짜리 Done
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_SMALLMENU_END, 118, 360, 83, 35,COMBAT_SKILL_POINT_IMG, 43, 42);
//	//CANCEL	02
//	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 2, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, 
//						SWD_SMALLMENU_END, 184, 358, 73, 35,COMBAT_SKILL_POINT_IMG, 4, 6);
	//DONE		03//두개 짜리 OK 버튼
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 3, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_SELECT_DONE, 130, 350, 73, 30,COMBAT_SKILL_POINT_IMG,	45, 44);
	//전투스킬 그룹1 00
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 4, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, //버튼 4 , 5, 6 이 변하지 않게 할것 CalcCombatSkillPointGroup(j-3)
						SWD_CSP_GROUP_ICON, 37, 70, 73, 35,COMBAT_SKILL_POINT_IMG, 15, 15,1);
	//전투스킬 그룹2 00	124.70
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 5, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_ICON, 124, 70, 73, 35,COMBAT_SKILL_POINT_IMG, 17, 18,2);
	//전투스킬 그룹3 00	211.70
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 6, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_ICON, 211, 70, 73, 35,COMBAT_SKILL_POINT_IMG, 19, 20,3);
	//전투스킬아이콘1 01	46.154
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 7, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 46, 154, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//전투스킬아이콘2 01	141.135
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 8, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 242, 154, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//전투스킬아이콘3 01	242.154
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 9, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 68, 234, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//전투스킬아이콘4 01	215.234
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 10, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 215, 234, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//전투스킬아이콘5 01	142.305
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 11, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 142, 305, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//전투스킬아이콘6 01	68.234
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 12, FT_CS_ICON, 
						SWD_SC_ICON_POINT, 141, 135, 42, 42, COMBAT_SKILL_ICON_IMG, 15, 16);
	//그룹 선택 스킬1	02	//nWillDo 값이 그룹 타입 1타입 2타입
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 13, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_SELECT, 29, 120, 127, 229,COMBAT_SKILL_POINT_IMG, 34,33,1);
	//그룹 선택 스킬2	02
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 14, FT_SHOW_SPECIAL_WILLDO_AUTO_PUT, 
						SWD_CSP_GROUP_SELECT, 160, 120, 127, 229,COMBAT_SKILL_POINT_IMG, 35,32,2);
	//포인트 업			01//한글 프린팅 까지 하는 버튼을 만들고 Rect 를 이용 하여 1 필드로 처리
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 15, FT_CSP_POINT_UP, 
						10, 250, 310, 38, 34,COMBAT_SKILL_POINT_IMG, 31, 30);
//////////////////////그룹 디스플레이
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 16, FT_CS_GROUP_EXPLAIN, 
						0, 84-10, 134+3, 42, 42,COMBAT_SKILL_ICON_IMG, 31, 30,1);

	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 17, FT_CS_GROUP_EXPLAIN, //그룹 설명 
						0, 193+10, 134+3, 42, 42,COMBAT_SKILL_ICON_IMG, 31, 30,2);
///////////////////// 왼쪽 스킬 설명창
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 18, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 40, 200, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 19, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 40, 220+2, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 20, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 40, 240+4, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 21, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 40, 260+6, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 22, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 40, 280+8, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 23, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 40, 300+10, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
///////////////////// 오른쪽 스킬 설명창
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 24, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 169, 200, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 25, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 169, 220+2, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 26, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 169, 240+4, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 27, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 169, 260+6, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 28, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 169, 280+8, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	::SMenuFTReg(MN_COMBAT_SKILL_POINT, 29, FT_CS_EXPLAIN, //스킬 자체 설명
						0, 169, 300+10, 90, 20,COMBAT_SKILL_POINT_IMG, 31, 30);
	}
	{
#ifndef _SDL
		::SMenuReg(MN_COMBAT_SKILL_DOING, 483, 361, 0, 0, 30);
#else // !_SDL
		//::SMenuReg(MN_COMBAT_SKILL_DOING, 683 + 40, 561 - 35, 0, 0, 30);
		::SMenuReg(MN_COMBAT_SKILL_DOING, 483, 361, 0, 0, 30);
#endif
	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
	//현재 저장된 스킬
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 1, FT_CSD_MAIN_ICON,
						0, 275, 89, 42, 42,COMBAT_SKILL_ICON_IMG, 72, 72);
//게이지 바 배경 그림
//	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 2, FT_NOMAL_PUT, 
//						0, 275, 0, 0, 0,COMBAT_SKILL_ICON_IMG, 85, 85);
	//게이지 바 CP COUNT
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 3, FT_CSD_POINT_COUNT_BAR,
						0, 239, 131, 0, 0,COMBAT_SKILL_ICON_IMG, 87, 87);
	//게이지 바 CP MAX
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 4, FT_CSD_POINT_LINE_BAR, 
						0, 269, 131, 0, 0,COMBAT_SKILL_ICON_IMG, 84, 84);
	//영역1-1 //nWillDo는 전투스킬 인덱스를 담는다
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 5, FT_CSD_ICON,
						SWD_SC_ICON_DOING, 225, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역1-2
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 6, FT_CSD_ICON,
						SWD_SC_ICON_DOING, 180, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역1-3
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 7, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 135, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역1-4
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 8, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 90, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역1-5
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 9, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 45, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역1-6
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 10, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 0, 0, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역2-1
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 11, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 225, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역2-2
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 12, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 180, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역2-3
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 13, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 135, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역2-4
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 14, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 90, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);						
	//영역2-5
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 15, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 45, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역2-6
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 16, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 0, 45, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역3-1
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 17, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 225, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역3-2
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 18, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 180, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역3-3
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 19, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 135, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역3-4
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 20, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 90, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역3-5
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 21, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 45, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	//영역3-6
	::SMenuFTReg(MN_COMBAT_SKILL_DOING, 22, FT_CSD_ICON, 
						SWD_SC_ICON_DOING, 0, 90, 0, 0,COMBAT_SKILL_ICON_IMG,0,0);
	//					SWD_SC_ICON_DOING, 0, 0, 42, 42,COMBAT_SKILL_ICON_IMG,0,0);
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	시간 표시 메뉴
//010910 lsw
	{
//	::SMenuReg(MN_TIME_DISPLAY_MENU, 600, 452, MAININTERFACE_IMG,79,8);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
	::SMenuReg(MN_TIME_DISPLAY_MENU, (SCREEN_WIDTH-219)/2-10, 0, 0,0,8);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
//국가전		//이름
//전투스킬		//레벨  좌표
	// 빽판
	::SMenuFTReg(MN_TIME_DISPLAY_MENU,0,FT_ALPHA_PUT,0,0, 0,0, 0, MAININTERFACE_IMG, 79, 79);
	// 마을 및 좌표 출력
	::SMenuFTReg(MN_TIME_DISPLAY_MENU,1,FT_XY_DISPLAY		, 0, 137, 5, 0, 0, 0, 0,0,0);
	::SMenuFTReg(MN_TIME_DISPLAY_MENU,2,FT_NATION_DISPLAY	, 0, 30, 5, 0, 0, 0, 0,0);
	}
	{//상인조합
		// 경매, 전체공지 메뉴
		::SMenuReg(MN_MERCHANT_MAIN, CENTER_X, 75, MERCHANT_IMG,1,6);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
		// Exit Button 
		::SMenuFTReg(MN_MERCHANT_MAIN,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			49, 235, 158-49, 264-235, MERCHANT_IMG, 15, 16);
		// 공지 메뉴 부르기
		::SMenuFTReg(MN_MERCHANT_MAIN,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU,
			31, 54, 178-31, 87-54, MERCHANT_IMG, 3, 4,MN_MERCHANT_BBS);// 공지 메뉴
//<! BBD 040401	acer측의 요청에 따라 대만민국 쌍것덜은 옥션질 몬하게 한다.
#ifdef TAIWAN_LOCALIZING_
		//거래물품 정보
		::SMenuFTReg(MN_MERCHANT_MAIN,2,FT_NONE, SWD_CALLMENU,
			31, 98, 178-31, 131-98, MERCHANT_IMG, 6, 7,MN_MERCHANT_FIND);//검색메뉴를 띄운다
		//거래물품 등록
		::SMenuFTReg(MN_MERCHANT_MAIN,3,FT_NONE, SWD_SEND_GET_MERCHANT_SELL_LIST,
			31, 142, 178-31, 175-142, MERCHANT_IMG, 9, 10,MN_MERCHANT_SELL_INFO);//자신이 파는 물품의 정보를 받아오도록 한다
		//거래물품 찾기
		::SMenuFTReg(MN_MERCHANT_MAIN,4,FT_NONE, SWD_SEND_GET_MERCHANT_RESULT_LIST,
			31, 186, 178-31, 219-186, MERCHANT_IMG, 12, 13,MN_MERCHANT_RESULT_TAKE);//결과물을 보도록 한다
#else
		//거래물품 정보
		::SMenuFTReg(MN_MERCHANT_MAIN,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU,
			31, 98, 178-31, 131-98, MERCHANT_IMG, 6, 7,MN_MERCHANT_FIND);//검색메뉴를 띄운다
		//거래물품 등록
		::SMenuFTReg(MN_MERCHANT_MAIN,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_GET_MERCHANT_SELL_LIST,
			31, 142, 178-31, 175-142, MERCHANT_IMG, 9, 10,MN_MERCHANT_SELL_INFO);//자신이 파는 물품의 정보를 받아오도록 한다
		//거래물품 찾기
		::SMenuFTReg(MN_MERCHANT_MAIN,4,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_GET_MERCHANT_RESULT_LIST,
			31, 186, 178-31, 219-186, MERCHANT_IMG, 12, 13,MN_MERCHANT_RESULT_TAKE);//결과물을 보도록 한다
#endif		
//> BBD 040401	acer측의 요청에 따라 대만민국 쌍것덜은 옥션질 몬하게 한다.
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	{// 공지 메뉴
	int gab_x = 7;
	const int iMnIndex	= MN_MERCHANT_BBS;
	const int iImg		= MERCHANT_IMG;

	::SMenuReg(iMnIndex, CENTER_X, 75, iImg,0,10);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다

	::SMenuFTReg(iMnIndex,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MN_MERCHANT_BBS_OK, // 공지 날리기
		20, 317, 110, 30, iImg, 18, 19,0);
	
	::SMenuFTReg(iMnIndex,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, // cancel
		148, 317, 110, 30, iImg, 21, 22,0);
	
//	SMenu[iMnIndex].nField[2].nValue=&SMenu[iMnIndex].key;
//	::SMenuFTReg(iMnIndex,2,FT_CHECK_COMBO, SWD_SUBMENU_BACK, // Check item
//		26+gab_x, 53+gab_x, 116, 22, 0, 0, 0,0);
	
	SMenu[iMnIndex].key =1;
//	SMenu[iMnIndex].nField[3].nValue=&SMenu[iMnIndex].key;
//	::SMenuFTReg(iMnIndex,3,FT_CHECK_COMBO, SWD_SUBMENU_BACK, // Check bbs
//		26+gab_x, 142+gab_x, 116, 22, 0, 0, 0,1);
	
	SMenu[iMnIndex].nField[4].nValue=&SMenu[iMnIndex].work;
	::SMenuFTReg(iMnIndex,4,FT_CHECK_COMBO, SWD_SUBMENU_BACK, // Check 1 map
		67+40+gab_x, 237-4+gab_x, 116, 22, 0, 0, 0,0);
	
	SMenu[iMnIndex].nField[5].nValue=&SMenu[iMnIndex].work;
	::SMenuFTReg(iMnIndex,5,FT_CHECK_COMBO, SWD_SUBMENU_BACK, // Check all map
		135+41+gab_x, 237-4+gab_x, 116, 22, 0, 0, 0,1);

	::SMenuFTReg(iMnIndex,6,FT_VIEW_BBS_MONEY, 0, // view money	// 가격의 차이	// key// 공지 때릴때의 대가
		77, 273, 0, 0, 0, 77, 0,0);

	::SMenuFTReg(iMnIndex,7,FT_WRITE_BBS, 0, // 공지 사항 출력
		45, 90, 130, 50, 0, 182, 3,0);//nImageNumber= 3;// 최대 줄수

//	::SMenuFTReg(iMnIndex,8,FT_WRITE_BBS_ITEM_NAME, 0, // 아이템 관련 출력	// 이름
//		40, 83, 170, 20, 0, 175, 3,0);//nImageNumber= 3;// 최대 줄수

//	::SMenuFTReg(iMnIndex,9,FT_WRITE_BBS_ITEM_MONEY, 0, // 아이템 관련 출력	// 가격
//		40, 113, 150, 20, 0, 140, 3,0);//nImageNumber= 3;// 최대 줄수
	}

//<soto-통합
	{
	//거래물품 정보(FIND) 상인조합의 1번째 버튼
		const int iMnIndex	= MN_MERCHANT_FIND;
		const int iImg		= MERCHANT_FINDER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 75, iImg,0,26);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
		
		
		// Ok Button
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_GET_MERCHANT_BUY_LIST,//이게 각종 값 조합 후에 Send 를 날린다
			56, 252, 110, 30, iImg, 1, 3);
		// Cancel Button
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU,
			313, 252, 110, 30, iImg, 4, 6,MN_MERCHANT_MAIN);
		
		//설명문
		strcpy(SMenu[iMnIndex].nField[iFtNum].temp,lan->OutputMessage(2,750));//임시
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT,0,
			270,70, 200, 220, 16, 180, DP_RECT_STRING_PUT_CUSTOM);// 설명

		//키워드 입력
		::SMenuFTReg(iMnIndex,iFtNum++,FT_WRITE_MERCHANT, SWD_NONE,
			86,61,130,40, iImg, 180, 0,1);

		//상인이름 입력
		::SMenuFTReg(iMnIndex,iFtNum++,FT_WRITE_MERCHANT, SWD_NONE,
			86,61 + 64,130,40, iImg, 180, 0,2);

		//기간 텍스트 출력
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			86,61 + 128,130,40, iImg, 0, DP_AUCTION_DATE);

		//기간 위로 버튼.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_UPDOWN_BTN,
			226,184,  30, 30, iImg, 8, 9,0);

		//기간 아래로 버튼
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_UPDOWN_BTN,
			226,211, 30, 30, iImg, 10, 11,1);

		//키워드 입력체크.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_CHECK_MERCHANT_FIND,0,
			222,64,34,34,iImg,7,7,0);
		//상인이름 입력 체크.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_CHECK_MERCHANT_FIND,1,
			222,64 + 65,34,34,iImg,7,7,0);		
/*		
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			90,65+10,  98, 32, iImg, 0, DP_AUCTION_FK_RARE);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			90,129+10, 98, 32, iImg, 0, DP_AUCTION_FK_LV);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			90,194+10, 98, 32, iImg, 0, DP_AUCTION_FK_TACTIC);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0,
			90,258+10, 98, 32, iImg, 0, DP_AUCTION_FK_WEAR);
		
		//검색기 리스트//nWillDo 는 검색어 종류 1.클래스 2.속성(레어) 3.레벨 4.택틱 5.착용점
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_RARE_SET,
			188,66,  30, 30, iImg, 8, 9,0);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_RARE_SET,
			221,66, 30, 30, iImg, 11, 12,1);

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_LV_SET,
			188,130, 30, 30, iImg, 8, 9,0);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_LV_SET,
			221,130, 30, 30, iImg, 11, 12,1);

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_TACTIC_SET,
			188,195, 30, 30, iImg, 8, 9,0);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_TACTIC_SET,
			221,195, 30, 30, iImg, 11, 12,1);

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_WEAR_SET,
			188,259, 30, 30, iImg, 8, 9,0);
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_AUCTION_FIND_KEY_WEAR_SET,
			221,259, 30, 30, iImg, 11, 12,1);
*/
	}
//>soto-통합

	{//구매자가 보는 현재 판매중인 물품 리스트
		const int iMnIndex	= MN_MERCHANT_BUY;
		const int iImg		= MERCHANT_BUY_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 50, iImg,0,15);//필드는 10개가 필요합니다.
		//취소,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, //030127 lsw
			416, 364, 150, 30, iImg, 5, 6,MN_MERCHANT_MAIN);
		//구입,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BUY_THIS_ITEM,//이게 각종 값 조합 후에 Send 를 날린다
			94, 364, 150, 30, iImg, 2, 3);
		//스크롤 업
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BUY_LIST_SCROLL_UP, 
			624, 95, 25, 40, iImg, 8, 9,MN_MERCHANT_MAIN);
		//스크롤 다운
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BUY_LIST_SCROLL_DOWN, 
			624, 270, 25, 40, iImg, 11, 12,MN_MERCHANT_MAIN);
		//체크박스1 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 14, 14,0);
		//체크박스2 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 14, 14,0);
		//체크박스3 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 14, 14,0);
		//체크박스4 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 14, 14,0);
		//체크박스5 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 14, 14,0);
		//체크박스6 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BUY_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 14, 14,0);
	}
	{//거래물품등록(상인만 쓰는 메뉴)//판매물품 전체 보기
		const int iMnIndex	= MN_MERCHANT_SELL_INFO;
		const int iImg		= MERCHANT_SELL_INFO_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 75, iImg,0,15);//필드는 10개가 필요합니다.
		//취소,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU, 
			367, 364, 110, 30, iImg, 8, 9,MN_MERCHANT_MAIN);
		//등록취소,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_SELL_ITEM_DELETE,//이게 각종 값 조합 후에 Send 를 날린다
			55, 364, 110, 30, iImg, 2, 3);
		//아이템등록,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MERCHANT_REGISTER,//이게 각종 값 조합 후에 Send 를 날린다
			211, 364, 110, 30, iImg, 5, 6);
		//스크롤 업
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_SELL_LIST_SCROLL_UP,
			494, 95, 25, 40, iImg, 11, 12,MN_MERCHANT_MAIN);
		//스크롤 다운
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_SELL_LIST_SCROLL_DOWN,
			494, 270, 25, 40, iImg, 14, 15,MN_MERCHANT_MAIN);
		////key는 인덱스 work 는 페이지 번호
		//체크박스1 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 17, 16,0);
		//체크박스2 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 17, 16,0);
		//체크박스3 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 17, 16,0);
		//체크박스4 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 17, 16,0);
		//체크박스5 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 17, 16,0);
		//체크박스6 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 17, 16,0);
	}
	{//판매자의 아이템 등록
		const int iMnIndex	= MN_MERCHANT_REGISTER;
		const int iImg		= MERCHANT_SELL_REGISTER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 50, 100, iImg,0,15);//필드는 10개가 필요합니다.
		//닫기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLSMALLMENU, 
			263,239,110,30, iImg, 5, 6,MN_MERCHANT_SELL_INFO);
		//신물품등록(OK)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_SELL_ITEM_REGISTER, 
			45,239,110,30, iImg, 2, 3,MN_MERCHANT_MAIN);
		//아이템 드롭존
		::SMenuFTReg(iMnIndex,iFtNum,FT_MERCHANT_SELL_ITEM_DROP_ZONE, 0, 
			93+12,99+15,0,0, iImg, 0, 0,MN_MERCHANT_MAIN);
		::SetRect(SMenu[iMnIndex].nField[iFtNum++].rCheakBox,35,78,35+153,78+80);//영역은 뒤에 셋팅
		
		//콤보박스(아래로 가는 화살표)(돈입력 받을 수 있게 할것
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MERCHANT_SELL_TYPE_SET, 
			153,183, 30,30, iImg, 8, 9,1);			
		//콤보박스(아래로 가는 화살표)(돈입력 받을 수 있게 할것
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MERCHANT_SELL_TYPE_SET, 
			184,183, 30,30, iImg, 12, 13,1);			
		//아이템 설명
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_EXPLAIN, 0, 
			217,65,182,101, iImg, 14, 15,MN_MERCHANT_MAIN);
		//가격 입력
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_SELL_ITEM_VALUE_INPUT, 0, 
			217,185,182,101, iImg, 14, 15,MN_MERCHANT_MAIN);
	}
	{//거래결과물 찾기(TAKE)
		const int iMnIndex	= MN_MERCHANT_RESULT_TAKE;
		const int iImg		= MERCHANT_RESULT_TAKE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 50, iImg,0,15);//필드는 10개가 필요합니다.
		//닫기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLSMALLMENU, 
			227,364,110,30, iImg, 2, 3,MN_MERCHANT_MAIN);

		//결과물 찾기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_RESULT_ITEM_TAKE, 
			42,364,110,30, iImg, 13, 14,MN_MERCHANT_MAIN);

		//스크롤 업
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_UP, 
			341,95,26,43, iImg, 5, 6);

		//스크롤 다운
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_DOWN, 
			341,270,26,43, iImg, 8, 9);	

		//체크박스1 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 11, 11,0);
		//체크박스2 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 11, 11,0);
		//체크박스3 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 11, 11,0);
		//체크박스4 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 11, 11,0);
		//체크박스5 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 11, 11,0);
		//체크박스6 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 11, 11,0);
	}
	{//직접거래 하기
		const int iMnIndex	= MN_MERCHANT_DIRECT_EXCHAGE_LIST;
		const int iImg		= MERCHANT_DIRECT_EXCHANGE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 50, iImg,0,15);//필드는 10개가 필요합니다.
		//닫기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			313,364,152,30, iImg, 5, 6);
		//아이템구입
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BUY_THIS_ITEM, 
			67,364,152,30, iImg, 2, 3,MN_MERCHANT_MAIN);
		//스크롤 업
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_UP, 
			494,95, 26,43, iImg, 8, 9);
		//스크롤 다운
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_DOWN, 
			494,270, 26,43, iImg, 12, 13);	
		//체크박스1 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 14, 14,0);
		//체크박스2 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 14, 14,0);
		//체크박스3 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 14, 14,0);
		//체크박스4 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 14, 14,0);
		//체크박스5 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 14, 14,0);
		//체크박스6 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_DIRECT_BUY_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 14, 14,0);
	}
//<! BBD 040303		// Auction 등록된 물건찾기 메뉴
	{
		const int iMnIndex	= MN_MERCHANT_BACKUP_MAIN;
		const int iImg		= MERCHANT_RESULT_TAKE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 50, iImg,0,15);//필드는 10개가 필요합니다.

		// 패킷 보내기 --> 패킷 받을 때 까지 무시한다.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_BACKUP_GET_LIST, SWD_NONE, 
			100,364,110,30, iImg, 2, 3, iMnIndex);

		//닫기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			227,364,110,30, iImg, 2, 3);

		//결과물 찾기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BACKUP_ITEM_TAKE, 
			42,364,110,30, iImg, 13, 14,MN_MERCHANT_MAIN);

		//스크롤 업
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_UP, 
			341,95,26,43, iImg, 5, 6);

		//스크롤 다운
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_DOWN, 
			341,270,26,43, iImg, 8, 9);	

		//체크박스1 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 0, 
			15, 82, 20, 20, iImg, 11, 11,0);
		//체크박스2 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 1, 
			15, 128, 20, 20, iImg, 11, 11,0);
		//체크박스3 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 2, 
			15, 174, 20, 20, iImg, 11, 11,0);
		//체크박스4 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 3, 
			15, 220, 20, 20, iImg, 11, 11,0);
		//체크박스5 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 4, 
			15, 266, 20, 20, iImg, 11, 11,0);
		//체크박스6 (아이템도 찍습니다)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MERCHANT_RESULT_ITEM_CHECK, 5, 
			15, 312, 20, 20, iImg, 11, 11,0);
	}
//> BBD 040303		// Auction 등록된 물건찾기 메뉴
	{//	오리지날 아이템 교환창
		//필드 넘버 안바뀌도록 조심 하시오!
		//LEFT RIGHT 가 반드시 일치 하지는 않습니다. 
		//내가 올릴 자리는 반드시 오른쪽 사이드 입니다.
		//오른쪽임에도 불구하고 LEFT가 등록 된 것도 있습니다.
		const int iMnIndex	= MN_EXCHANGE_ITEM;
		const int iImg		= EXCHANGE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 30, 70, iImg,0,6);//필드는 10개가 필요합니다.
		// text 출력
		::SMenuFTReg(iMnIndex,0,FT_EXCHANGE_ITEM_ETC, SWD_CALLMENU, 
			0,54, 164,34, iImg, 0, 0);//0
		// 아이템과 check 표시
		::SMenuFTReg(iMnIndex,1,FT_EXCHANGE_ITEM, 0,
			236,99,174,139, iImg, 6, 6,RIGHT);//1	//오른쪽에 있습니다//내가 주는쪽
		
		::SMenuFTReg(iMnIndex,2,FT_EXCHANGE_ITEM, 0, 
			26,99,174,139, iImg, 5, 5,LEFT);//2	//왼쪽에 있습니다//내가 받는쪽
			
		::SMenuFTReg(iMnIndex,3,FT_EXCHANGE_ITEM_CHECK,0, //3 필드 넘버 안꼬이게 조심
			17,258,25,25, iImg, 4, 4,LEFT);// 체크박스//상대방이 클릭하는 사이드
			
		::SMenuFTReg(iMnIndex,4,FT_EXCHANGE_ITEM_CHECK,0, //4 필드 넘버 안꼬이게 조심
			401,258,25,25, iImg, 4,4, RIGHT);// 체크박스//내가 클릭 하는 사이드
		//Cancel
		::SMenuFTReg(iMnIndex,5,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_ITEM_EXCHANGE_CANCEL, 
			166,296,110,30, iImg, 2, 3);
	}
	{// 1:1 결투 메뉴
		const int iMnIndex	= MN_MAN_TO_MAN;
		const int iImg		= MAN_TO_MAN_IMG;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, 50, 50, iImg,0,7);//필드는 10개가 필요합니다.
		//이름 출력
		::SMenuFTReg(iMnIndex,VS_LEFT_DISPLAY_FIELD_NO,FT_MAN_TO_MAN_INFO_DISPLAY, 0, 
			29,102, 174,139, iImg, 0, 0,LEFT);//0
		//이름 출력
		::SMenuFTReg(iMnIndex,VS_RIGHT_DISPLAY_FIELD_NO,FT_MAN_TO_MAN_INFO_DISPLAY, 0, 
			239,102, 174,139, iImg, 0, 0,RIGHT);//0
		//체크 버튼//상대방
		::SMenuFTReg(iMnIndex,3,FT_MAN_TO_MAN_CHECK, 1, 
			49,260, 0,0, iImg, 4, 4,LEFT);//0
		//체크 버튼
		::SMenuFTReg(iMnIndex,4,FT_MAN_TO_MAN_CHECK, 2, 
			375,260, 20,20, iImg, 4, 4,RIGHT);//0`
		// cancel
		::SMenuFTReg(iMnIndex,5,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAN_TO_MAN_CANCEL, 
			166,296, 110,30, iImg, 2, 3);//0
	}
	{// 길드관리소 메인
		const int iMnIndex	= MN_GUILD_ADMIN;
		const int iImg		= GUILD_ADMIN;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 75, iImg,0,4);//필드는 10개가 필요합니다.
		// exit	
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			49,235, 110,30, iImg, 11, 12);
		//길드관리
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU, 
			23,63,164,34, iImg, 2, 3,MN_GUILD_MAIN);
		// grand master // gm 등록
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALLMENU, 
			23,120,164,34, iImg, 5, 6,MN_GM_LIST);
		//4개 짜리 인터페이스 또는 상인메뉴를 부른다
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MERCHANT_OR_SKILL_MASTER, 
			23,175, 164,34, iImg, 8, 9,GM_QUEST_MERCHANT);
	}
	{//gm 퀘스트의 EXPLAIN 눌렀을때 아이템 설명
		const int iMnIndex	= MN_GM_ITEM_EXPLAIN;
		const int iImg		= GUILD_LIST;
		int iFtNum = 0;
		// 그랜드 마스터 등록 조건 아이템 설명
		::SMenuReg(iMnIndex, CENTER_X, 34, iImg,7,5);//필드는 10개가 필요합니다.
		// 아이템 설명
		::SMenuFTReg(iMnIndex,iFtNum++,FT_GM_ITEM_EXPLAIN, 0, 
			60,109, 100, 15, iImg, 0, 0,155);//0
		//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
			27,325, 101-27, 348-325, iImg, 8, 9);//0
		// CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
			173,325, 247-173, 348-325, iImg, 5, 6);//0
		// up  scroll 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, SWD_GUILD_EXPLAIN_SCROLL, 
			218,263, 239-218, 275-263, 0, 235, 235,LEFT);//0
		// down  scroll 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, SWD_GUILD_EXPLAIN_SCROLL, 
			218,278, 239-218, 275-278, 0, 235, 235,LEFT);//0
	}
	{//gm 퀘스트가 요구하는 인터페이스
		const int iMnIndex	= MN_GM_LIST;
		const int iImg		= GUILD_LIST;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 34, iImg,7,8);//필드는 10개가 필요합니다.

		// 그랜드 마스터 가능 직업 리스트
		::SMenuFTReg(iMnIndex,iFtNum++,FT_GM_LIST, 0, 
			112,312,0, 0, iImg, 0, 4);//0// 스크롤 버튼도 찍어 준다.
		// OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_REGIST, 
			27,325,101-27, 348-325, iImg, 8, 9);//1번 필드
		// CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
			173,325,247-173, 348-325, iImg, 5, 6);//2번 필드
		// explain
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, 1, 
			185,178,51, 17, GUILD_ADMIN_ETC, 1, 2,MN_GM_ITEM_EXPLAIN);
		// explain
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, 2, 
			185,198,51, 17, GUILD_ADMIN_ETC, 1, 2,MN_GM_ITEM_EXPLAIN);
		// explain
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, 3, 
			185,218,51, 17, GUILD_ADMIN_ETC, 1, 2,MN_GM_ITEM_EXPLAIN);
		// up  scroll 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, SWD_GM_LIST_SCROLL, 
			115,315,25, 20, iImg, 2, 3,LEFT);
		// down  scroll 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_NO_CHECK, SWD_GM_LIST_SCROLL, 
			115,336,25, 20, iImg, 0, 1,RIGHT);
	}
	{//기성 고르기	버튼 4개 짜리
		const int iMnIndex	= MN_SKILL_MASTER_LIST;
		const int iImg		= SKILL_MASTER_KIND_SELECT_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 34, iImg,0,8);//필드는 10개가 필요합니다.

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_SKILL_MASTER_QUEST, 
			30,53, 200,34, iImg, 2, 3,GM_QUEST_BLACKSMITH_MASTER);//대장장이
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_SKILL_MASTER_QUEST, 
			30,106, 200,34, iImg, 5, 6,GM_QUEST_CARPENTER_MASTER);//목수
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_SKILL_MASTER_QUEST, 
			30,159, 200,34, iImg, 8, 9,GM_QUEST_BOWCRAFT_MASTER);//활
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_SKILL_MASTER_QUEST, 
			30,212, 200,34, iImg, 11, 12,GM_QUEST_TAILOR_MASTER);//재단
		// Done
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
			74,260,110,30, iImg, 14, 15);
	}
	{	
		const int iMnIndex	= MN_GM_2ND_SKILL;
		const int iImg		= GM_2ND_SKILL_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 150, 70, iImg,0,25,0);

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_2ND_OK, 
			19,323,92,30, iImg, 2, 3);//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_2ND_CANCEL, 
			198,323,92,30, iImg, 5, 6);//CANCEL
		//재료분석:  24,196    262/108
		//만들 아이템(타겟)(왼쪽 상단아이콘 출력)// 이거 해서 하자ㅡ.ㅡ 분기에 넣어 주자
		//여기에 글자 출력도 들어 있다.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			24,79,0,0, iImg,0,DP_GM_2ND_SKILL_TARGET_ITEM_PUT);//CANCEL
		//재료 올릴 곳.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_GM_2ND_SKILL_MAKE_ITEM, 0, 
			175,59,104,87, iImg, 0, 21);//CANCEL
		//아이템 찍어짐
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			72+2,55+4,0,0, iImg, DP_RARE_UPGRADE_RESULT, 21);//CANCEL
	}
	{//기성 스킬
		const int iMnIndex	= MN_SKILL_MASTER_MAKE;
		const int iImg		= SKILL_MASTER_MAKE_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 80, 30, iImg,0,10,0);
		
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SKILL_MASTER_MULTIRARE_OK, 
			52,404,92,30, iImg, 2, 3);//OK

		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_2ND_CANCEL, 
			244,404,92,30, iImg, 5, 6);//CANCEL
		
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			24,82,182,87, iImg,0,DP_SKILL_MASTER_RESULT_ITEM);
		//재료 올릴 곳.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SKILL_MASTER_MAKE_ITEM, 0, 
			228,59,129,103, iImg, 0, 21);
	}
	::SetGMQUESTMenu(GM_QUEST_MERCHANT);
	{//돈 교환 하기(BOND_MONEY)
		const int iMnIndex	= MN_EXCHANGE_BOND_MONEY;
		const int iImg		= EXCHANGE_BOND_MONEY_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 150, 30, iImg,0,10,0);
		//CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			148,317,110,30, iImg, 5, 6);
		//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_EXCHANGE_BOND_MONEY_OK, 
			20,317,110,30, iImg, 2, 3);
		//글자 창
		strcpy(SMenu[iMnIndex].nField[iFtNum].temp, lan->OutputMessage(4,561) );
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, 0, 
			34,64,0,0, 15,200,DP_RECT_STRING_PUT_CUSTOM);
		//아이템 실제 드롭존 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_EXCHANGE_BOND_MONEY_ITEM, 0, 
			41,218,194,68, iImg, 0, 0);//CANCEL
	}
	
	{//메일 메뉴
		const int iMnIndex	= MN_MAIL_MENU;
		const int iImg		= MAIL_MENU;

		::SMenuReg(iMnIndex, 400, 0, iImg,0,12,0);
		//받은 편지함 (서버로 리스트를 요청 하면 받을때 바탕 셋팅 해서 콜을 한다)
		::SMenuFTReg(iMnIndex,0, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, 
			20,61,73,19, iImg, 49, 49, MVT_RECV_LIST);//위치 안바뀌게 조심 (0번)

		//지운 편지함
		::SMenuFTReg(iMnIndex,1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, 
			98,61,72,19, iImg, 50, 50, MVT_DELETE_LIST);//위치 안바뀌게 조심 (1번)

		//메일 복원
		::SMenuFTReg(iMnIndex,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_REPAIR, 
			186,442 ,78,30, iImg, 51, 52,MVT_DELETE_LIST);

		int iFtNum = 3;
		//ALL(모두 선택)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SELECT_ALL, 
			23,86,24,24, iImg, 24, 25);
		
		//Zone(체크버튼, 이름(메일쓰기), 메일제목(읽기), 시간)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_LIST, SWD_NONE, 
			26,112,380,400,iImg);
		
		//스크롤 버튼 앞
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SCROLL, 
			155,402,26,25, iImg, 30, 31,SCROLL_DOWN);
		//스크롤 버튼 뒤
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SCROLL, 
			219,402,26,25, iImg, 32, 33,SCROLL_UP);
		//현재 페이지
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			198, 410,78,30, iImg, 0, DP_NOW_VIEW_MAIL_PAGE);
		
		//메일 쓰기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAIL_WRITE, 
			18,442 ,78,30, iImg, 4, 5,0);
		//삭제
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_DELETE, 
			102,442 ,78,30, iImg, 6, 7,0);
		//Cancel
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			304,442 ,78,30, iImg, 10, 11);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	메일 보내기 메뉴
	{
		const int iMnIndex	= MN_MAIL_WRITE;
		const int iImg		= MAIL_MENU;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, 400, 0, iImg,3,20,0);

		//CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, 
			304,442,78,30, iImg, 10, 11,MVT_RECV_LIST);
		
		//좌 스크롤 (메일타입)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_TEMP_MAIL_TYPE_SCROLL, 
			94,86,26,25, iImg, 30, 31,SCROLL_UP);
		//우 스크롤 (메일타입)
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_TEMP_MAIL_TYPE_SCROLL, 
			152,86,26,25, iImg, 32, 33,SCROLL_DOWN);

		//친구 리스트
//		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_FRIEND_MENU, 
//			297,114,78,30, iImg, 22, 23);

		//받는 사람 입력
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_WRITE, SWD_NONE, 
			98,122,190,20, iImg, 180, 0,1);
		//제목
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_WRITE, SWD_NONE, 
			98,152,270,20, iImg, 180, 0,2);
		//내용
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_WRITE, SWD_NONE, 
			30,178,320,250, iImg, 315, 0,3);
		//위 스크롤

		//아래 스크롤

		//보내기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_WRITE_SEND, 
			18,442,78,30, iImg, 16, 17);

		//다시쓰기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_WRITE_DELETE, 
			102,442,78,30, iImg, 18, 19);

		//메일타입 아이콘 //순서 맨 마지막입니다.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT,SWD_NONE, 
			126,90,26,25, iImg, 0, DP_TEMP_MAIL_ICON,0);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		const int iMnIndex	= MN_MAIL_READ;
		const int iImg		= MAIL_MENU;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, 400, 0, iImg,2,15,0);
		
		//CANCEL
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, 
			304,442,78,30, iImg, 10, 11,MVT_RECV_LIST);
		
		//친구로등록
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SENDER_REGIST_FRIEND, 
			209,115,78,30, iImg, 20, 21,1);

		//보낸이
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_READ, SWD_NONE, 
			98,124,100,20, iImg, 180, 0,1);
		//제목
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_READ, SWD_NONE, 
			98,152,100,20, iImg, 180, 0,2);
		//내용
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_READ, SWD_NONE, 
			30,177,100,20, iImg, 315, 0,3);
		//날짜,
		::SMenuFTReg(iMnIndex,iFtNum++,FT_MAIL_READ, SWD_NONE, 
			250,92,100,20, iImg, 180, 0,4);
		
		//지우기
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_DELETE, 
			18,442 ,78,30, iImg, 12, 13,1);

		//답장
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAIL_WRITE, 
			102,442 ,78,30, iImg, 14, 15,1);

		//PC에 저장
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_SAVE, 
			186,442 ,78,30, iImg, 8, 9);
		
		//메일타입 아이콘 //순서 맨 마지막입니다.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT,SWD_NONE, 
			97,89,26,25, iImg, 0, DP_TEMP_MAIL_ICON,0);
	}
	{
////////////////////////////////////////////////////////////////////////////////////////////
// 아레나
		const int iMnIndex	= MN_ARENA_LOBBY;
		const int iImg		= ARENA_IMG;
		int iFtNum = 0;

		::SMenuReg(iMnIndex, 287, 0, iImg,0,25,0);

		// RED 등록
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_BATTLE_REGIST, 
			18,82,36,26, iImg, 8, 9,ATI_RED_TEAM);

		// blue 등록
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_BATTLE_REGIST, 
			300,82,36,26, iImg, 8, 9,ATI_BLUE_TEAM);

		// 대기실
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_FIGHTMAP_WAIT_MEMBER, 
			152,82 ,50,26, iImg, 22, 23,0);
		
		//시작 대기시간.
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			470,195 ,50,26, iImg, 0, DP_ARENA_WAIT_TIME);

		//R 승률
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			110,90,50,26, iImg, 0, DP_ARENA_TEAM_WIN_RATE,ATI_RED_TEAM);
		//B 승률
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			260,90,50,26, iImg, 0, DP_ARENA_TEAM_WIN_RATE,ATI_BLUE_TEAM);

		//R 총배팅액
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			360,86 ,50,26, iImg, 0, DP_BETTING_MONEY,ATI_RED_TEAM);
		//B 총배팅액
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			360,110 ,50,26, iImg, 0, DP_BETTING_MONEY,ATI_BLUE_TEAM);

		//내 배팅액
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			360, 143 , 50, 26, iImg, 0, DP_MY_BETTING_MONEY, 0);
		
		//내 배당금
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			360, 167 , 50, 26, iImg, 0, DP_MY_BETTING_MONEY, 1);

		// 개개별팀 정보 보여주기		// red//R 팀원 정보
		SMenu[iMnIndex].nField[iFtNum].nSHideNomalCount=1;
		SMenu[iMnIndex].nField[iFtNum].nSHideNomalStart=0;
		SMenu[iMnIndex].nField[iFtNum].nShideNomalPlus=1;
		::SMenuFTReg(iMnIndex,iFtNum++,FT_VIEW_BATTLE_MEMBER, SWD_NONE, 
			22,120,100,100, iImg, 0, 0,ATI_RED_TEAM);
		
		// 개개별팀 정보 보여주기		// blue//B 팀원 정보 
		SMenu[iMnIndex].nField[iFtNum].nSHideNomalCount=1;
		SMenu[iMnIndex].nField[iFtNum].nSHideNomalStart=0;
		SMenu[iMnIndex].nField[iFtNum].nShideNomalPlus=1;
		::SMenuFTReg(iMnIndex,iFtNum++,FT_VIEW_BATTLE_MEMBER, SWD_NONE, 
			194,120,100,100, iImg, 0, 0,ATI_BLUE_TEAM);

		// BATTLE을 시작한다.//스타트 k
		::SMenuFTReg(iMnIndex,iFtNum++,FT_FIGHTMAP_START, SWD_FIGHTMAP_WAIT_MEMBER, 
			67,342 ,110,30, iImg, 2, 3,0);

		//닫고 나간다//Cancel k
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_FIGHT_WAIT_EXIT, //
			335,342 ,110,30, iImg, 5, 6,0);

		//대기자 명단 k
		::SMenuFTReg(iMnIndex,iFtNum++,FT_VIEW_BATTLE_WAIT_MEMBER, SWD_NONE, 
			22,258 ,78,30, iImg, 0, 0,0);

		//대기자 스크롤 ->
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_NONE, 
			467,253 ,30,30, iImg, 17, 18,SCROLL_UP);

		//대기자 스크롤 <-
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_NONE, 
			467,302 ,30,30, iImg, 20, 21,SCROLL_DOWN);
		//관전
		::SMenuFTReg(iMnIndex,iFtNum++, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_ARENA_OBSERVER_MENU, 
			364,214 ,50,26, iImg, 11, 12,MN_OBSERVER_MODE);

		//도박
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_ARENA_BETTING_MENU, 
			364+74,214 ,50,26, iImg, 14, 15,MN_BETTING);

/*		// red  ->
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_DSCROLL_AUTO_PUT, SWD_NONE, 
			186,442 ,78,30, iImg, 8, 9,18);

		// red  <-
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_ISCROLL_AUTO_PUT, SWD_NONE, 
			186,442 ,78,30, iImg, 8, 9,18);

		// blue ->
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_DSCROLL_AUTO_PUT, SWD_NONE, 
			186,442 ,78,30, iImg, 8, 9,19);
		
		// blue <-
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_ISCROLL_AUTO_PUT, SWD_NONE, 
			186,442 ,78,30, iImg, 8, 9,19);
*/	}
	{//관전 모드
		const int iMnIndex	= MN_OBSERVER_MODE;
		const int iImg		= OBSERVER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 100, iImg,0,10,0);
		//안내문
		::SMenuFTReg(iMnIndex,iFtNum,FT_DATA_PUT,0,
			24,65,0,0, iImg,200, DP_RECT_STRING_PUT);
		::strcpy(SMenu[iMnIndex].nField[iFtNum++].temp, lan->OutputMessage(3,281) );

		//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_OBSERVER_TEAM_SELECT, 
			25,197,78,30, iImg, 3, 4);
		//Cancel
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			142,197,78,30, iImg, 6, 7);
		//체크 박스
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SELECT_OBSERVER_TARGET_TEAM, SWD_NONE, 
			82,160,18,18, iImg, 8, 8,ATI_RED_TEAM);
		//체크 박스
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SELECT_OBSERVER_TARGET_TEAM, SWD_NONE, 
			210,160,18,18, iImg, 8, 8,ATI_BLUE_TEAM);
	}
	{//배팅 메뉴 
		const int iMnIndex	= MN_BETTING;
		const int iImg		= OBSERVER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, CENTER_X, 100, iImg,1,10,0);
		//안내문
		::SMenuFTReg(iMnIndex,iFtNum,FT_DATA_PUT,0,
			25,130,0,0, 16,230, DP_RECT_STRING_PUT_CUSTOM,FONT_COLOR_SOLID_RED);
		::strcpy(SMenu[iMnIndex].nField[iFtNum++].temp, lan->OutputMessage(3,280) );
		
		//OK
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SEND_MY_ARENA_BETTING_MONEY, 
			224,242 ,42,30, iImg, 11, 12);
		//Done
		::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SMALLMENU_END, 
			103,289 ,75,30, iImg, 14, 15);
		//은행돈
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT,	0,
			58+40,72,0,0,iImg,0,DP_BANK_TOTAL);

		//내 현재 걸려있는 금액
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 	
			30+90, 103, 50, 26, iImg, 0, DP_MY_BETTING_MONEY,0);

		//체크 박스
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SELECT_BETTING_TARGET_TEAM, SWD_NONE, 
			85,195,25,25, iImg, 8, 8,ATI_RED_TEAM);
		//체크 박스
		::SMenuFTReg(iMnIndex,iFtNum++,FT_SELECT_BETTING_TARGET_TEAM, SWD_NONE, 
			85,222,25,25, iImg, 8, 8,ATI_BLUE_TEAM);

		//R 총배팅액
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			30+50,195 ,50,26, iImg, 0, DP_BETTING_MONEY,ATI_RED_TEAM);
		//B 총배팅액
		::SMenuFTReg(iMnIndex,iFtNum++,FT_DATA_PUT, SWD_NONE, 
			30+50,222 ,50,26, iImg, 0, DP_BETTING_MONEY,ATI_BLUE_TEAM);

		//내 배팅액 입력
		::SMenuFTReg(iMnIndex,iFtNum++,FT_ARENA_BETTING, SWD_NONE, 	
			30, 252, 50, 26, iImg, 180, 0, 0);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	revival menu 메뉴
	{
		const int iMnIndex	= MN_REVIVAL;
		const int iImg		= OBSERVER_IMG;
		int iFtNum = 0;
		::SMenuReg(iMnIndex, 400-80, 48, iImg,16,2,0);
		// revival 버튼
		::SMenuFTReg(iMnIndex,iFtNum++,FT_REVIVAL_BUTTON, SWD_NONE, 
			0,0,80,30, iImg, 17, 18);
	}

}

void SetGMQUESTMenu(const int iGmQuestIndex)
{//상인 퀘스트 OK 하기(아이템 뭐 필요하고 하는거 //021108 lsw
	int iBgImgNo = 0;
	switch(iGmQuestIndex)
	{
	case GM_QUEST_MERCHANT://상인         
		{
			iBgImgNo = 1;
		}break;
	case GM_QUEST_BLACKSMITH_MASTER:	//대장장이 기성
	case GM_QUEST_CARPENTER_MASTER:	//목수 기성    
	case GM_QUEST_BOWCRAFT_MASTER:	//활제작사 기성
	case GM_QUEST_TAILOR_MASTER	:	//재단사 기성
		{
			iBgImgNo = 0;
		}break;
	}//switch(iResult)
	
	const int iMnIndex	= NM_MERCHANT_QUEST;
	const int iImg		= SKILL_MASTER_QUEST_IMG;
	int iFtNum = 0;

	::SMenuReg(iMnIndex, CENTER_X, 34, iImg,iBgImgNo,8,0,0,iGmQuestIndex);//work 에 셋팅 되어야 뜬다.
	//OK
	::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_GM_REGIST_2, 
		27-3,325-3,101-27, 348-325, iImg, 3, 4);
	// CANCEL
	::SMenuFTReg(iMnIndex,iFtNum++,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SUBMENU_BACK, 
		173-3,325-3,247-173, 348-325, iImg, 6, 7);
	// 그랜드 마스터 가능 직업 리스트//디스플레이
	::SMenuFTReg(iMnIndex,iFtNum++,FT_DISPLAY_GM_REGIST, 0, 
		112,312,0, 0, iImg, 0, 4,GM_QUEST_MERCHANT);//0// 스크롤 버튼도 찍어 준다.
}

void StartMapSelectMenuSet(const int iNation)//내 국가를 넣으면 메뉴를 셋팅 한다
{	
	::SMenuReg(MN_START_MAP, 0, 0, NEW_START_MAP_IMG,0,17);
	// connect game
	::SMenuFTReg(MN_START_MAP,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_CONNECT,
						300, 523,200, 62, NEW_START_MAP_IMG, 3, 4,MN_LODING);
	// recent logout	
	::SMenuFTReg(MN_START_MAP,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_CONNECT_RECENT,
						501, 523,200, 62, NEW_START_MAP_IMG, 5, 6,MN_LODING);
	// back button( 4명 선택 화면으로 돌아감 )
	::SMenuFTReg(MN_START_MAP,9,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_RE_LOGON,
						99, 523,200, 62, NEW_START_MAP_IMG, 1, 2,MN_SELECT_CHARACTER);
	// 선택되어진 마을 계속 보여주기
	SMenu[MN_START_MAP].nField[15].nType=FT_SELECT_CITY;
	SMenu[MN_START_MAP].nField[15].x=558;
	SMenu[MN_START_MAP].nField[15].y=543;
	SMenu[MN_START_MAP].nField[15].nImageNumber=0;
	SMenu[MN_START_MAP].nField[15].nImageType=START_MAP;

	// 마을 선택하는 효과 보여주기 ( 헬턴트 )
	SMenu[MN_START_MAP].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[3].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[3].nWillDo=SC_HELTERNT_VILLAGE;
	//SMenu[MN_START_MAP].nField[3].nWillDo=SC_HELTERNT_CASTLE;
	SMenu[MN_START_MAP].nField[3].x=64-20;
	SMenu[MN_START_MAP].nField[3].y=132-20;
	SMenu[MN_START_MAP].nField[3].nRectImage=7;
	SMenu[MN_START_MAP].nField[3].nImageNumber=7;
	SMenu[MN_START_MAP].nField[3].nSHideNomalCount=17;
	SMenu[MN_START_MAP].nField[3].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[3].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[3].rCheakBox,64,132,121,185);

	// 마을 선택하는 효과 보여주기 ( 바이서스 임펠 )
#ifndef CHINA_LOCALIZING_
	SMenu[MN_START_MAP].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
#endif
	SMenu[MN_START_MAP].nField[4].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[4].nWillDo=SC_BAISUS_IMPEL;
	SMenu[MN_START_MAP].nField[4].x=310-20;
	SMenu[MN_START_MAP].nField[4].y=241-20;
	SMenu[MN_START_MAP].nField[4].nRectImage=8;
	SMenu[MN_START_MAP].nField[4].nImageNumber=8;
	SMenu[MN_START_MAP].nField[4].nSHideNomalCount=18;
	SMenu[MN_START_MAP].nField[4].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[4].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[4].rCheakBox,310,241,367,294);

	// 마을 선택하는 효과 보여주기 ( 레너스 )
	SMenu[MN_START_MAP].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[5].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[5].nWillDo=SC_RENUS;
	SMenu[MN_START_MAP].nField[5].x=176-20;
	SMenu[MN_START_MAP].nField[5].y=178-20;
	SMenu[MN_START_MAP].nField[5].nRectImage=9;
	SMenu[MN_START_MAP].nField[5].nImageNumber=9;
	SMenu[MN_START_MAP].nField[5].nSHideNomalCount=19;
	SMenu[MN_START_MAP].nField[5].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[5].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[5].rCheakBox,176,178,233,231);

	// 마을 선택하는 효과 보여주기 ( 바란탄 )
	SMenu[MN_START_MAP].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[6].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[6].nWillDo=SC_BARANTAN;
	SMenu[MN_START_MAP].nField[6].x=520;
	SMenu[MN_START_MAP].nField[6].y=225;
	SMenu[MN_START_MAP].nField[6].nRectImage=5;
	SMenu[MN_START_MAP].nField[6].nImageNumber=5;
	SMenu[MN_START_MAP].nField[6].nSHideNomalCount=6;
	SMenu[MN_START_MAP].nField[6].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[6].rCheakBox,520,243,610,313);

	// 마을 선택하는 효과 보여주기 ( 이라무스시 )
#ifndef CHINA_LOCALIZING_
	SMenu[MN_START_MAP].nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
#endif
	SMenu[MN_START_MAP].nField[7].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[7].nWillDo=SC_IRAMUS;
	SMenu[MN_START_MAP].nField[7].x=239-20;
	SMenu[MN_START_MAP].nField[7].y=207-20;
	SMenu[MN_START_MAP].nField[7].nRectImage=11;
	SMenu[MN_START_MAP].nField[7].nImageNumber=11;
	SMenu[MN_START_MAP].nField[7].nSHideNomalCount=21;
	SMenu[MN_START_MAP].nField[7].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[7].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[7].rCheakBox,239,207,296,260);

	// 마을 선택하는 효과 보여주기 ( 다프유벤 )
#ifndef CHINA_LOCALIZING_
	SMenu[MN_START_MAP].nField[8].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
#endif
	SMenu[MN_START_MAP].nField[8].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[8].nWillDo=SC_DIPH_UBEN;
	SMenu[MN_START_MAP].nField[8].x=171-20;
	SMenu[MN_START_MAP].nField[8].y=402-20;
	SMenu[MN_START_MAP].nField[8].nRectImage=12;
	SMenu[MN_START_MAP].nField[8].nImageNumber=12;
	SMenu[MN_START_MAP].nField[8].nSHideNomalCount=22;
	SMenu[MN_START_MAP].nField[8].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[8].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[8].rCheakBox,171,402,228,455);
/*
	// 마을 선택하는 효과 보여주기 ( 헬턴트 마을 )
	SMenu[MN_START_MAP].nField[10].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[10].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[10].nWillDo=SC_HELTERNT_VILLAGE;
	SMenu[MN_START_MAP].nField[10].x=75;
	SMenu[MN_START_MAP].nField[10].y=155;
	SMenu[MN_START_MAP].nField[10].nRectImage=25;
	SMenu[MN_START_MAP].nField[10].nImageNumber=25;
	SMenu[MN_START_MAP].nField[10].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[10].rCheakBox,120,154,154,176);

	// 마을 선택하는 효과 보여주기 ( 휴다인 강 )
	SMenu[MN_START_MAP].nField[11].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[11].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[11].nWillDo=SC_HUDAIN_RIVER;
	SMenu[MN_START_MAP].nField[11].x=46;
	SMenu[MN_START_MAP].nField[11].y=204;
	SMenu[MN_START_MAP].nField[11].nRectImage=26;
	SMenu[MN_START_MAP].nField[11].nImageNumber=26;
	SMenu[MN_START_MAP].nField[11].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[11].rCheakBox,70,185,110,207);

	// 마을 선택하는 효과 보여주기 ( 회색산맥 )
	SMenu[MN_START_MAP].nField[12].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
	SMenu[MN_START_MAP].nField[12].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[12].nWillDo=SC_GRAY_MOUNTAIN;
	SMenu[MN_START_MAP].nField[12].x=239;
	SMenu[MN_START_MAP].nField[12].y=61;
	SMenu[MN_START_MAP].nField[12].nRectImage=27;
	SMenu[MN_START_MAP].nField[12].nImageNumber=27;
	SMenu[MN_START_MAP].nField[12].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[12].rCheakBox,230,50,337,90);
*/
	// 마을 선택하는 효과 보여주기 ( 타이넨 )
#ifndef CHINA_LOCALIZING_
	SMenu[MN_START_MAP].nField[13].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2;
#endif
	SMenu[MN_START_MAP].nField[13].nSpecialWillDo=SWD_WILLDO_WORK;
	SMenu[MN_START_MAP].nField[13].nWillDo=SC_TYREN;
	SMenu[MN_START_MAP].nField[13].x=102-20;
	SMenu[MN_START_MAP].nField[13].y=354-20;
	SMenu[MN_START_MAP].nField[13].nRectImage=10;
	SMenu[MN_START_MAP].nField[13].nImageNumber=10;
	SMenu[MN_START_MAP].nField[13].nSHideNomalCount=28;
	SMenu[MN_START_MAP].nField[13].nShideNomalPlus=3;
	SMenu[MN_START_MAP].nField[13].nImageType=START_MAP;
	::SetRect(SMenu[MN_START_MAP].nField[13].rCheakBox,102,354,159,407);
	
}

void StartMenuSet2()
{
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////로그인 메뉴/////////////////////////////////////////////////
	{
	::CallLoginMenu();
	::SMenuReg(MN_LOGIN, 0, 0, MAINSTART_LOGIN_IMG,0,10);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
	
	// 에디트 박스 띄우고 그 사이를 왔다갔다.....
	// 에디트 박스의 글씨를 출력만..
	::SMenuFTReg(MN_LOGIN, 3, FT_SPECIAL_WILLDO, SWD_IDEDITBOX, 260,358, 91, 19, -1, 0, 0 ,0);
	::SMenuFTReg(MN_LOGIN, 5, FT_DATA_PUT, 0, 365,362 ,193, 15,MAINSTART_LOGIN_IMG, 0,DP_IDEDITBOX,0);
	
	::SMenuFTReg(MN_LOGIN, 4, FT_SPECIAL_WILLDO, SWD_PASSEDITBOX, 260,396, 91, 19, -1, 0, 0 ,0);
	::SMenuFTReg(MN_LOGIN, 6, FT_DATA_PUT, 0, 365,401, 193, 15, 0, 0, DP_PASSEDITBOX,0);

	//필드 번호 바꾸지 말것 엔터키 안먹음 로그인 하기
	::SMenuFTReg(MN_LOGIN,0,FT_SPECIAL_WILLDO,SWD_LOGON,363,429,91,19,-1,0,0,MN_SELECT_CHARACTER);//로그인
	//SHOW CREDITS버튼 선택시 띄우기
//	::SMenuFTReg(MN_LOGIN,7,FT_HIDE_WILLDO_PUT,0,326,464,157,27,MAINSTART_LOGIN_IMG,2,2,MN_CREDIT);
	//INTRO버튼 선택시 뛰우기
//	::SMenuFTReg(MN_LOGIN,8,FT_HIDE_WILLDO_PUT,0,364,495,75,27,MAINSTART_LOGIN_IMG,3,3,MN_INFO);
	//EXIT버튼 선택시 
	::SMenuFTReg(MN_LOGIN,1,FT_SPECIAL_WILLDO,SWD_QUIT,467,429,91,19,-1,0,0,0);
	//NewId버튼 선택시
//	::SMenuFTReg(MN_LOGIN,9,FT_SPECIAL_WILLDO,SWD_NEW_ID,352,432,100,30,MAINSTART_LOGIN_IMG,5,5,0);
	}
//////////////////////////////////////////////////////////////////////////////////////
	//서버 선택 메뉴
	//메인메뉴뛰우기
	{
	SMenu[MN_MAINSTART_START].x=293;
	SMenu[MN_MAINSTART_START].y=224;
	SMenu[MN_MAINSTART_START].nImageNumber=9;
	SMenu[MN_MAINSTART_START].nImageType=START_ETC;
	SMenu[MN_MAINSTART_START].nFieldCount=5;
	SMenu[MN_MAINSTART_START].CheakType=2;

	//LOCATION버튼 선택시 뛰우기	오른쪽에 셀렉트 메뉴 띄우기
	SMenu[MN_MAINSTART_START].nField[0].nType=FT_NO_CHECK;  // 필드 번호 직접 엑세스 하는 부분 있음
	SMenu[MN_MAINSTART_START].nField[0].nSpecialWillDo=SWD_CALL_OTHER;
	SMenu[MN_MAINSTART_START].nField[0].nWillDo=MN_LOCATION;
	SMenu[MN_MAINSTART_START].nField[0].x=29;
	SMenu[MN_MAINSTART_START].nField[0].y=29;
	SMenu[MN_MAINSTART_START].nField[0].nRectImage=831;
	SMenu[MN_MAINSTART_START].nField[0].nImageNumber=31;

	//SELECT버튼 선택시 뛰우기,		왼쪽에 로케이션 메뉴 띄우기
	SMenu[MN_MAINSTART_START].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAINSTART_START].nField[1].nSpecialWillDo=SWD_CALL_OTHER;   // 필드 번호 직접 엑세스 하는 부분 있음
	SMenu[MN_MAINSTART_START].nField[1].nWillDo=MN_SELECT;
	SMenu[MN_MAINSTART_START].nField[1].x=114;
	SMenu[MN_MAINSTART_START].nField[1].y=28;
	SMenu[MN_MAINSTART_START].nField[1].nRectImage=10;
	SMenu[MN_MAINSTART_START].nField[1].nImageNumber=11;
	SMenu[MN_MAINSTART_START].nField[1].nImageType=START_ETC;

	//BACK버튼 선택시 뛰우기,  초기 메뉴로 돌아가기
	SMenu[MN_MAINSTART_START].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAINSTART_START].nField[2].nSpecialWillDo=SWD_LOGON;
	SMenu[MN_MAINSTART_START].nField[2].nWillDo=MN_SELECT_CHARACTER;
	SMenu[MN_MAINSTART_START].nField[2].x=113;
	SMenu[MN_MAINSTART_START].nField[2].y=115;
	SMenu[MN_MAINSTART_START].nField[2].nRectImage=14;
	SMenu[MN_MAINSTART_START].nField[2].nImageNumber=15;
	SMenu[MN_MAINSTART_START].nField[2].nImageType=START_ETC;

	//컨넥트 버튼 선택시 뛰우기
	SMenu[MN_MAINSTART_START].nField[3].nType=FT_NO_CHECK;
//	SMenu[MN_MAINSTART_START].nField[3].nSpecialWillDo=FT_HIDE_WILLDO_AUTO_PUT;
	SMenu[MN_MAINSTART_START].nField[3].nWillDo=MN_LODING;
	SMenu[MN_MAINSTART_START].nField[3].x=74;
	SMenu[MN_MAINSTART_START].nField[3].y=74;
	SMenu[MN_MAINSTART_START].nField[3].nRectImage=16;
	SMenu[MN_MAINSTART_START].nField[3].nImageNumber=17;
	SMenu[MN_MAINSTART_START].nField[3].nImageType=START_ETC;

	// Recent 버튼 선택할 경우
	SMenu[MN_MAINSTART_START].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MAINSTART_START].nField[4].nSpecialWillDo=SWD_CONNECT_RECENT;
	SMenu[MN_MAINSTART_START].nField[4].nWillDo=MN_LODING;
	SMenu[MN_MAINSTART_START].nField[4].x=29;
	SMenu[MN_MAINSTART_START].nField[4].y=115;
	SMenu[MN_MAINSTART_START].nField[4].nRectImage=12;
	SMenu[MN_MAINSTART_START].nField[4].nImageNumber=13;
	SMenu[MN_MAINSTART_START].nField[4].nImageType=START_ETC;



	}
	{
	//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//	인물 선택
	::SMenuReg(MN_SELECT_CHARACTER, 0, 0, SELECT_CHARACTER_IMG,0,12);
	
	::SMenuFTReg(MN_SELECT_CHARACTER,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_RESET_JOB,
						94, 506,200, 62, SELECT_CHARACTER_IMG, 6, 1);
	
	::SMenuFTReg(MN_SELECT_CHARACTER,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_CHARACTER_DEL_ADD,
						295, 506,200, 62, SELECT_CHARACTER_IMG, 5, 2);

	::SMenuFTReg(MN_SELECT_CHARACTER,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_IDPASS,//ID Pass로 전환
						496, 506,200, 62, SELECT_CHARACTER_IMG, 4, 3);
	
	::SMenuFTReg(MN_SELECT_CHARACTER,2,FT_NO_CHECK,0,
						295, 506,200, 62, SELECT_CHARACTER_IMG, 11, 11);//가변 필드 Make캐릭터 로 변하는 부분

	//select 1 box
	SMenu[MN_SELECT_CHARACTER].nField[4].nType=FT_NOMAL_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[4].nSpecialWillDo= SWD_CHARACTER_SELECT;
	SMenu[MN_SELECT_CHARACTER].nField[4].nWillDo = 0;
	SMenu[MN_SELECT_CHARACTER].nField[4].x=38;
	SMenu[MN_SELECT_CHARACTER].nField[4].y=20;
	SMenu[MN_SELECT_CHARACTER].nField[4].nRectImage= 30;
	SMenu[MN_SELECT_CHARACTER].nField[4].nImageNumber= 30;
	SMenu[MN_SELECT_CHARACTER].nField[4].nImageType= START_ETC;
	::SetRect(SMenu[MN_SELECT_CHARACTER].nField[4].rCheakBox,38,19,200,244);

	//select 2 box
	SMenu[MN_SELECT_CHARACTER].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[5].nSpecialWillDo=SWD_CHARACTER_SELECT;
	SMenu[MN_SELECT_CHARACTER].nField[5].nWillDo= 1;
	SMenu[MN_SELECT_CHARACTER].nField[5].x=38;
	SMenu[MN_SELECT_CHARACTER].nField[5].y=254;
	SMenu[MN_SELECT_CHARACTER].nField[5].nRectImage= 30;
	SMenu[MN_SELECT_CHARACTER].nField[5].nImageNumber= 30;
	SMenu[MN_SELECT_CHARACTER].nField[5].nImageType= START_ETC;
	::SetRect(SMenu[MN_SELECT_CHARACTER].nField[5].rCheakBox,38,254,200,479);

	//select 3 box
	SMenu[MN_SELECT_CHARACTER].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[6].nSpecialWillDo= SWD_CHARACTER_SELECT;
	SMenu[MN_SELECT_CHARACTER].nField[6].nWillDo= 2;
	SMenu[MN_SELECT_CHARACTER].nField[6].x=596;
	SMenu[MN_SELECT_CHARACTER].nField[6].y=20;
	SMenu[MN_SELECT_CHARACTER].nField[6].nRectImage= 30;
	SMenu[MN_SELECT_CHARACTER].nField[6].nImageNumber= 30;
	SMenu[MN_SELECT_CHARACTER].nField[6].nImageType= START_ETC;
	::SetRect(SMenu[MN_SELECT_CHARACTER].nField[6].rCheakBox,595,19,757,244);

	//select 4 box
	SMenu[MN_SELECT_CHARACTER].nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[7].nSpecialWillDo= SWD_CHARACTER_SELECT;
	SMenu[MN_SELECT_CHARACTER].nField[7].nWillDo= 3;
	SMenu[MN_SELECT_CHARACTER].nField[7].x=595;
	SMenu[MN_SELECT_CHARACTER].nField[7].y=254;
	SMenu[MN_SELECT_CHARACTER].nField[7].nRectImage= 30;
	SMenu[MN_SELECT_CHARACTER].nField[7].nImageNumber= 30;
	SMenu[MN_SELECT_CHARACTER].nField[7].nImageType= START_ETC;
	::SetRect(SMenu[MN_SELECT_CHARACTER].nField[7].rCheakBox,595,254,757,479);

	//1 box - name field
	SMenu[MN_SELECT_CHARACTER].nField[8].nType=FT_DATA_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[8].nWillDo= 0;
	SMenu[MN_SELECT_CHARACTER].nField[8].x=60;
	SMenu[MN_SELECT_CHARACTER].nField[8].y=53;
	SMenu[MN_SELECT_CHARACTER].nField[8].nImageNumber= DP_NAME_SELECT;

	//2 box - name field
	SMenu[MN_SELECT_CHARACTER].nField[9].nType=FT_DATA_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[9].nWillDo= 1;
	SMenu[MN_SELECT_CHARACTER].nField[9].x=60;
	SMenu[MN_SELECT_CHARACTER].nField[9].y=286;
	SMenu[MN_SELECT_CHARACTER].nField[9].nImageNumber= DP_NAME_SELECT;
	
	//3 box - name field
	SMenu[MN_SELECT_CHARACTER].nField[10].nType=FT_DATA_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[10].nWillDo= 2;
	SMenu[MN_SELECT_CHARACTER].nField[10].x=617;
	SMenu[MN_SELECT_CHARACTER].nField[10].y=53;
	SMenu[MN_SELECT_CHARACTER].nField[10].nImageNumber= DP_NAME_SELECT;

	//4 box - name field
	SMenu[MN_SELECT_CHARACTER].nField[11].nType=FT_DATA_PUT;
	SMenu[MN_SELECT_CHARACTER].nField[11].nWillDo= 3;
	SMenu[MN_SELECT_CHARACTER].nField[11].x=617;
	SMenu[MN_SELECT_CHARACTER].nField[11].y=286;
	SMenu[MN_SELECT_CHARACTER].nField[11].nImageNumber= DP_NAME_SELECT;

	}
	{// ###
	///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////로딩화면///////////////////////////////////////////////////////
	// 로딩부분에서 게이지바가 올라가고 다 차면 메인 인터페이스 장면으로 넘어간다.
		::SMenuReg(MN_LODING, 0, 0, CONNECTING_IMG,0,2);
/*
		SMenu[MN_LODING].x=0;
		SMenu[MN_LODING].y=0;
	//	SMenu[MN_LODING].nImageNumber=210;
		SMenu[MN_LODING].nImageNumber=20;
		SMenu[MN_LODING].nImageType=START_ETC;
		SMenu[MN_LODING].nFieldCount=2;
		SMenu[MN_LODING].CheakType=0;
*/
		::SMenuFTReg(MN_LODING,0,FT_GAUGE,0,
						183, 82,0, 0, CONNECTING_IMG, 1, 16);
/*
		SMenu[MN_LODING].nField[0].nType=FT_GAUGE;
		SMenu[MN_LODING].nField[0].nSpecialWillDo = 391+GABX_SCREEN;
*/
		SMenu[MN_LODING].nField[1].nType=FT_LODING_LIGHT;
		SMenu[MN_LODING].nField[1].x=255+GABX_SCREEN;
		SMenu[MN_LODING].nField[1].y=290+GABY_SCREEN-7;
		SMenu[MN_LODING].nField[1].nRectImage=930;		// 마지막 화면
		SMenu[MN_LODING].nField[1].nImageNumber=906;
		SMenu[MN_LODING].nField[1].nSHideNomalCount=906;
	}
	{
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// 게임 만든 사람 설명창
	::SMenuReg(MN_CREDIT, 0, 0, SHOW_CREDIT_IMG,0,2);
/*
	SMenu[MN_CREDIT].x=0;
	SMenu[MN_CREDIT].y=0;
	SMenu[MN_CREDIT].nImageNumber=29;
	SMenu[MN_CREDIT].nImageType=START_ETC;
	SMenu[MN_CREDIT].nFieldCount=2;
*/
/*	SMenu[MN_CREDIT].nField[0].nType = FT_NOMAL_PUT;
	SMenu[MN_CREDIT].nField[0].x=258;
	SMenu[MN_CREDIT].nField[0].y=106;
	SMenu[MN_CREDIT].nField[0].nImageNumber = 28;
	SMenu[MN_CREDIT].nField[0].nImageType = START_ETC;
*/
	SMenu[MN_CREDIT].nField[1].nType = FT_CREDIT_PUT;
	SMenu[MN_CREDIT].nField[1].nWillDo = 288;
	SMenu[MN_CREDIT].nField[1].x=234;
	SMenu[MN_CREDIT].nField[1].y=180;
	SMenu[MN_CREDIT].nField[1].nRectImage= 288;
	}
	StartMapSelectMenuSet(NW_BY);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
// 서버 선택 메뉴
	::SMenuReg(MN_SELECT_SERVER, 293, 224, SELECT_SERVER,0,8);
	SMenu[MN_SELECT_SERVER].work = CheckSelectServer();//020828 lsw 서버 세트 갯수
	//293 224
	::SMenuFTReg(MN_SELECT_SERVER,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALL_MENU,
						74, 194,100,30, SELECT_SERVER, 6,7, MN_SYSTEM_LOGOUT);
	
//	::SMenuFTReg(MN_SELECT_SERVER,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALL_MENU,
//						74+293, 194+224,100,30, SELECT_SERVER, 6,7, MN_SYSTEM_LOGOUT);

	//스크롤 버튼 위		// work 값과 key값을 이용한다.
	SMenu[MN_SELECT_SERVER].nField[1].nType=FT_SELECT_SERVER_SCROLL;
	SMenu[MN_SELECT_SERVER].nField[1].nWillDo=SCROLL_UP;
	SMenu[MN_SELECT_SERVER].nField[1].x=197;
	SMenu[MN_SELECT_SERVER].nField[1].y=29;
	SMenu[MN_SELECT_SERVER].nField[1].nSpecialWillDo = 1;		// 보통 평상시 버튼
	SMenu[MN_SELECT_SERVER].nField[1].nRectImage=4;
	SMenu[MN_SELECT_SERVER].nField[1].nImageNumber=5;
	SMenu[MN_SELECT_SERVER].nField[1].nImageType=SELECT_SERVER;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[1].rCheakBox,197,29, 216,58);

	//스크롤 버튼 아래
	SMenu[MN_SELECT_SERVER].nField[2].nType=FT_SELECT_SERVER_SCROLL;
	SMenu[MN_SELECT_SERVER].nField[2].nWillDo=SCROLL_DOWN;
	SMenu[MN_SELECT_SERVER].nField[2].x=197;
	SMenu[MN_SELECT_SERVER].nField[2].y=166;
	SMenu[MN_SELECT_SERVER].nField[2].nSpecialWillDo = 8;		// 보통 평상시 버튼
	SMenu[MN_SELECT_SERVER].nField[2].nRectImage=2;
	SMenu[MN_SELECT_SERVER].nField[2].nImageNumber=3;
	SMenu[MN_SELECT_SERVER].nField[2].nImageType=SELECT_SERVER;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[2].rCheakBox,197,166,216,195);

	int y = 10;
    // 서버 리스트 //1
	SMenu[MN_SELECT_SERVER].nField[3].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[3].nWillDo=0;
	SMenu[MN_SELECT_SERVER].nField[3].x=111;
	SMenu[MN_SELECT_SERVER].nField[3].y=44+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[3].rCheakBox,29,44+y,179,64+y);//020828 lsw
	
	// 2
	SMenu[MN_SELECT_SERVER].nField[4].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[4].nWillDo=1;
	SMenu[MN_SELECT_SERVER].nField[4].x=111;
	SMenu[MN_SELECT_SERVER].nField[4].y=69+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[4].rCheakBox,29,69+y,179,89+y);//020828 lsw

	// 3
	SMenu[MN_SELECT_SERVER].nField[5].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[5].nWillDo=2;
	SMenu[MN_SELECT_SERVER].nField[5].x=111;
	SMenu[MN_SELECT_SERVER].nField[5].y=94+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[5].rCheakBox,29,94+y,179,114+y);//020828 lsw

	// 4
	SMenu[MN_SELECT_SERVER].nField[6].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[6].nWillDo=3;
	SMenu[MN_SELECT_SERVER].nField[6].x=111;
	SMenu[MN_SELECT_SERVER].nField[6].y=119+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[6].rCheakBox,29,119+y,179,139+y);//020828 lsw

	// 5
	SMenu[MN_SELECT_SERVER].nField[7].nType=FT_SELECT_SERVER_BUTTON;
	SMenu[MN_SELECT_SERVER].nField[7].nWillDo=4;
	SMenu[MN_SELECT_SERVER].nField[7].x=111;
	SMenu[MN_SELECT_SERVER].nField[7].y=144+y;
	::SetRect(SMenu[MN_SELECT_SERVER].nField[7].rCheakBox,29,144+y,179,164+y);//020828 lsw
	}	

	{//020802 lsw 
//	::SMenuReg(MN_TIME_DISPLAY_MENU, 600, 452, MAININTERFACE_IMG,79,8);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
	::SMenuReg(MN_DEL_CHAR_COMFORM, -1, 40, DEL_CHAR_COMFORM_IMG,0,8);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
	::SMenuFTReg(MN_DEL_CHAR_COMFORM,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_DELETE_CHARATER,52, 298, 80, 30, DEL_CHAR_COMFORM_IMG, 1,2);
	::SMenuFTReg(MN_DEL_CHAR_COMFORM,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_DELETE_CHARATER_CLOSE,188, 298, 80, 30, DEL_CHAR_COMFORM_IMG, 3,4);
	::SMenuFTReg(MN_DEL_CHAR_COMFORM,3,FT_INPUT_DEL_CHAR_COMFORM_KEY_VIEW	, 0, 81, 241, 0, 0, 0, 0,0);
	}
	{//020815-2 lsw 
	::SMenuReg(MN_SELECT_TUTORIAL_LOGIN, -1, 100, SELECT_TUTORIAL_LOGIN_IMG,0,8);	//300 짜리 인터페이스 이니까//투명이라 0 0 이다
		

#ifdef TAIWAN_LOCALIZING_
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SELECT_TUTORIAL_OK,18	, 138, 80, 30,	SELECT_TUTORIAL_LOGIN_IMG, 4,3);////OK	해당 버튼 세개.//윌두 값으로 이 버튼은 일을 합니다//1번을 함부로 바꾸지 말것
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_RE_LOGON,101, 138, 80, 30,	SELECT_TUTORIAL_LOGIN_IMG, 7,6,MN_SELECT_CHARACTER);//Cancel 캐릭 선택화면으로 이동

	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 60 , 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,SC_SCHOLIUM2);//초보자사냥터(초보자 사냥터로 이동
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,4,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 91, 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,-1);//마을로 접속(메뉴 닫는거 ymenuFocus 를 맞춰 줘야 한다//레벨7 이상
#else
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SELECT_TUTORIAL_OK,18	, 162, 80, 30,	SELECT_TUTORIAL_LOGIN_IMG, 4,3);////OK	해당 버튼 세개.//윌두 값으로 이 버튼은 일을 합니다//1번을 함부로 바꾸지 말것
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_RE_LOGON,101, 162, 80, 30,	SELECT_TUTORIAL_LOGIN_IMG, 7,6,MN_SELECT_CHARACTER);//Cancel 캐릭 선택화면으로 이동

	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 57 , 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,SC_SCHOLIUM);//입문 가이드(튜토리얼 맵으로 이동
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,4,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 86 , 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,SC_SCHOLIUM2);//초보자사냥터(초보자 사냥터로 이동
	::SMenuFTReg(MN_SELECT_TUTORIAL_LOGIN,5,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_SET_LOGIN_TARGET, 31, 117, 130, 25,	SELECT_TUTORIAL_LOGIN_IMG, 1,1,-1);//마을로 접속(메뉴 닫는거 ymenuFocus 를 맞춰 줘야 한다//레벨7 이상
#endif
	}
}

extern const int MAX_START;
extern int menu[];
void StartMenuSubProcessType2(SMENU *SubMenu,int i, int j,int k)
{
	switch(SubMenu->nField[j].nType)  //구성요소가 어떤 타입이지?
	{
	case FT_GAUGE:
		{
			const int iResult = LoadingGameData();

			switch( iResult )
			{
			case -1 :
			case -2 :
				{
					::CallLoginMenu();
					// SMenu[MN_SELECT_CHARACTER].bActive=true;
					SMenu[MN_LODING].bActive=false;
					CallOkCancelMessageBox(  MN_LODING, 0,0, lan->OutputMessage(6,58), 0 );//lsw
					return;
				}break;
			case -3 :	
				{
					::CallLoginMenu();
					// SMenu[MN_SELECT_CHARACTER].bActive=true;
					SMenu[MN_LODING].bActive=false;
					CallOkCancelMessageBox(  MN_LODING, 0,0, lan->OutputMessage(6,59), 0 );//  lsw
					return;
				}break;
			case LGDS_END:
				{
					for(int x=0;x<MAX_START;x++) SMenu[menu[x]].bActive=FALSE;		//모든 창을 닫는다.
					SMenu[MN_MAININTERFACE].bActive=TRUE;
				}break;
			}

			const int iMax = SubMenu->nField[j].nImageNumber;
			const int iImageNo = (iResult*iMax) /LGDS_END ;

			FieldTypeNomalPut(	SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y,
								SubMenu->nField[j].nRectImage+iImageNo,SubMenu->nField[j].nImageType);
		}break;	
	case FT_ANIMATION2 : //020515 lsw 
		{
			break;//우선 작동 안함
			const int iAniMaxNo	= SubMenu->nField[j].nSpecialWillDo;
			const int iImageNo	= ++SubMenu->nField[j].nImageNumber;
			if(iImageNo >= iAniMaxNo)
			{
				SubMenu->nField[j].nImageNumber =0;
			}
			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x,SubMenu->nField[j].y,SubMenu->nField[j].nImageNumber,SubMenu->nField[j].nImageType);//에니메이션 찍기
		}break;
	case FT_CREDIT_PUT : 
		{
			bool go = false;
			if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 ) go = true;
			static bool count = false;
			if( LButtonDown ) 
			{
				count = true;
			}
			else if( count ) go = true;

			if( go )
			{
				count = false;
				g_aCurrentKeys[DIK_ESCAPE] = 0;
				SubMenu->bActive = false;
				::CallLoginMenu();
			}
		 }break;
	case FT_LODING_LIGHT :	
		{
/*			static bool turn = false;
			static bool img_count = true;
			int &img = SubMenu->nField[j].nSHideNomalCount;
			int &img2 = SubMenu->nField[j].nSHideNomalStart;

			if( img >= SubMenu->nField[j].nRectImage ) 
			{
				img = SubMenu->nField[j].nRectImage;
				turn = true;
			}
			else if( img <= SubMenu->nField[j].nImageNumber ) 
			{
				img = SubMenu->nField[j].nImageNumber;
				turn = false;
			}

			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, img);
			if( img_count ) 
			{
				if( turn )	img--;
				else img++;
				
				img2++;
			}
//			img_count= !img_count;

			int kia_start = 10022;		// 시작 기아 번호
//			int kia_start2 = 10027;
			FieldTypeNomalPut(0, 0, 387+GABX_SCREEN, 0+GABY_SCREEN, kia_start + img2%5);
//			FieldTypeNomalPut(0, 0, 162+GABX_SCREEN, 0+GABY_SCREEN, kia_start2 + img2%5);
			
///*
			if( img >= 906 && img < 911) SubMenu->nField[j-1].nImageNumber = 211;
			else if( img >= 911 && img < 915) SubMenu->nField[j-1].nImageNumber = 468;
			else if( img >= 915 && img < 920) SubMenu->nField[j-1].nImageNumber = 469;
			else if( img >= 920 && img < 924) SubMenu->nField[j-1].nImageNumber = 470;
			else if( img >= 924 && img < 929) SubMenu->nField[j-1].nImageNumber = 471;
			else if( img >= 929 && img < 934) SubMenu->nField[j-1].nImageNumber = 472;
* /			else SubMenu->nField[j-1].nImageNumber = 473;
			
		*/}	break;	

		case FT_INPUT_DEL_CHAR_COMFORM_KEY_VIEW://020820 lsw
		{
			SetFocus2(HWND_1);//021001 lsw
			char szSecretKeyCode[51] = {0,};//020820 lsw
			char szSecretKeyCodeTemp[51] = {0,};//020901 lsw
			EWndMgr.GetTxt( HWND_1, szSecretKeyCode, 51 );//020820 lsw

			static int cursor = 0 ;
			Hcolor( FONT_COLOR_WHITE );
			
			if(EWndMgr.IsFocus(HWND_1))//커서가 여기서 깜빡
			{
				const int iPassLen = strlen(szSecretKeyCode);//별표 처리
			//	for(int temp=0; temp < iPassLen; temp++) 
				const int iDisplayLen = (iPassLen>24)?24:iPassLen;
				for(int temp=0; iDisplayLen > temp; temp++) //24자만 찍어라//021014 lsw
				{
					szSecretKeyCodeTemp[temp]='*'; 
					szSecretKeyCodeTemp[temp+1] = NULL;
				}
				if((cursor%=12) < 6){Hprint2( SubMenu->x+SubMenu->nField[j].x+2 , SubMenu->y+SubMenu->nField[j].y+4+2, g_DestBackBuf, "%s_",szSecretKeyCodeTemp );}
				else {	Hprint2( SubMenu->x+SubMenu->nField[j].x+2 , SubMenu->y+SubMenu->nField[j].y+4+2, g_DestBackBuf, "%s",szSecretKeyCodeTemp );}
			}
			cursor++;
			//윈도 죽으면 죽이는 루틴,
			//포커스 잃으면 다시 가져오는 루틴
		}break;
	}
	return;
}

void MenuSet2()// 필드를 추가할때 죽었을 경우 체크 되지 않아야 할 필드라면  IsNoCheckFieldByMain()  함수에 추가한다.
{
	{
		int AddVert = 4; // 세로 보정값. //soto-1.02 4
		int	AddHorz = 6; // 가로 보정값. //soto-1.02 6
		
		//::SMenuReg(MN_ITEM, 480, 0, INVENTORY_IMG,0,25,0);
		::SMenuReg(MN_ITEM, 480, 0, INVENTORY_IMG,0,26,0); // soto-040411
		//자신을 닫기
		::SMenuFTReg(MN_ITEM, 1, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_RMENUCLOSE, 238 + AddHorz, 319 + AddVert, 70,30,INVENTORY_IMG,6,7);
		//인벤토리창 스크롤을 위한 가상 필드
		//PAGE_NUMBER__ 는 2 입니다 2번 필드 쓰지 마세요
		::SMenuFTReg(MN_ITEM, PAGE_NUMBER__, FT_HIDE_NOMAL_GROUP_PUT, 0, 0 + AddHorz, 0 + AddVert,  0,0,0,0,0);
	//	SMenu[MN_ITEM].nField[2].nType=FT_HIDE_NOMAL_GROUP_PUT;
		SMenu[MN_ITEM].nField[PAGE_NUMBER__].nSHideNomalNumber=HN_INVENTORY;
		SMenu[MN_ITEM].nField[PAGE_NUMBER__].nSHideNomalCount=3;
		SMenu[MN_ITEM].nField[PAGE_NUMBER__].nSHideNomalStart=0;
		SMenu[MN_ITEM].nField[PAGE_NUMBER__].nShideNomalPlus=1;
		
		//실제 마우스 이벤트를 체크하는 인벤토리 필드
		::SMenuFTReg(MN_ITEM, 3, FT_INVENTORY, 0, 22 +AddHorz,200+AddVert,35*8,35*3,0,0);
		//매직 메뉴 뛰우기
		::SMenuFTReg(MN_ITEM, 4, FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAININTERFACE_MAGIC, 7+AddHorz, 319+AddVert,  70,30,INVENTORY_IMG,3,4);
		//인벤토리창 스크롤용 필드					
		::SMenuFTReg(MN_ITEM, 5, FT_HIDE_ISCROLL_AUTO_PUT, 0, 162+AddHorz, 320+AddVert, 40,30,INVENTORY_IMG,9,10,2);
		//인벤토리창 스크롤용 필드
		::SMenuFTReg(MN_ITEM, 6, FT_HIDE_DSCROLL_AUTO_PUT, 0, 126+AddHorz, 320+AddVert, 40,30,INVENTORY_IMG,12,13,2);

		//무기 장착 화면 띄우기//		SMenu[MN_ITEM].nField[7].nImageNumber=120;  //menu.cpp에서 이미지 바꾸는 루틴 존재함.
		::SMenuFTReg(MN_ITEM, 7, FT_NOMAL_PUT, 0, 126+AddHorz, 42+AddVert, 0,0,INVENTORY_IMG,1,1,0);

		// 무기 장착 화면에 아이템 선택하기 //nWillDo는 무기의 종류를 판단하기 위해 사용. 8~15번은 직접 엑세스 하는 부분이 있습니다.
		::SMenuFTReg(MN_ITEM, 8, FT_ARMOR_INVENTORY, WEAR_RIGHT_HAND, 152+AddHorz, 64+AddVert, 40,40,INVENTORY_IMG,0,0,WT_WEAPON);
		SMenu[MN_ITEM].nField[8].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 9, FT_ARMOR_INVENTORY, WEAR_HEAD, 255+AddHorz, 66+AddVert, 40,40,INVENTORY_IMG,0,0,WT_HELMET);
		SMenu[MN_ITEM].nField[9].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 10, FT_ARMOR_INVENTORY, WEAR_BODY, 208+AddHorz, 109+AddVert, 40,40,INVENTORY_IMG,0,0,WT_ARMOR);
		SMenu[MN_ITEM].nField[10].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 11, FT_ARMOR_INVENTORY, WEAR_LEFT_HAND, 257+AddHorz, 152+AddVert, 40,40,INVENTORY_IMG,0,0,WT_SHIELD);
		SMenu[MN_ITEM].nField[11].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 12, FT_ARMOR_INVENTORY, WEAR_UNIQUE, 27+AddHorz, 67+AddVert, 40,40,INVENTORY_IMG,0,0,WT_UNIQUE1);
		SMenu[MN_ITEM].nField[12].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 13, FT_ARMOR_INVENTORY, WEAR_UNIQUE, 63+AddHorz, 67+AddVert, 40,40,INVENTORY_IMG,0,0,WT_UNIQUE2);
		SMenu[MN_ITEM].nField[13].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 14, FT_ARMOR_INVENTORY, WEAR_NECK, 20+AddHorz, 127+AddVert, 40,40,INVENTORY_IMG,0,0,WT_NECK);
		SMenu[MN_ITEM].nField[14].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 15, FT_ARMOR_INVENTORY, WEAR_SHOES, 67+AddHorz, 127+AddVert, 40,40,INVENTORY_IMG,0,0,WT_SHOES);
		SMenu[MN_ITEM].nField[15].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 16, FT_DATA_PUT, 0, 152+AddHorz,	64+AddVert,		40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_WEAPON);
		::SMenuFTReg(MN_ITEM, 17, FT_DATA_PUT, 0, 255+AddHorz,	66+AddVert,		40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_HELMET);
		::SMenuFTReg(MN_ITEM, 18, FT_DATA_PUT, 0, 208+AddHorz,	109+AddVert,	40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_ARMOR);
		::SMenuFTReg(MN_ITEM, 19, FT_DATA_PUT, 0, 257+AddHorz,	152+AddVert,	40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_SHIELD);
		::SMenuFTReg(MN_ITEM, 20, FT_DATA_PUT, 0, 27+AddHorz,		67+AddVert,		40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_UNIQUE1);
		::SMenuFTReg(MN_ITEM, 21, FT_DATA_PUT, 0, 63+AddHorz,		67+AddVert,		40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_UNIQUE2);
		::SMenuFTReg(MN_ITEM, 22, FT_DATA_PUT, 0, 20+AddHorz,		127+AddVert,	40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_NECK);
		::SMenuFTReg(MN_ITEM, 23, FT_DATA_PUT, 0, 67+AddHorz,		127+AddVert,	40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_SHOES);

		//654,170 , 480 = 
		::SMenuFTReg(MN_ITEM, 24, FT_NONE, 0, 134 + 10,		140 + 2,	40,40,0,0,0,0);//soto-030513
		//이거 좌표 조정
		/*	양초 우선은 막아둔다
		::SMenuFTReg(MN_ITEM, 24, FT_CANDLE_INVENTORY, WEAR_SHOES, 138, 134, 40,40,INVENTORY_IMG,0,0,WT_SHOES);
		SMenu[MN_ITEM].nField[24].nImageNumber=865;
		::SMenuFTReg(MN_ITEM, 25, FT_DATA_PUT, 0, 138,		134,	40,40,INVENTORY_IMG,0,DP_ARMOR_INVENTORY,WT_SHOES);
		*/
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//스테이터스 메뉴
	{
	::SMenuReg(MN_STATUS, 0, 0, STATUS_IMG,0,27,0);
	// thai2 YGI
	::SMenuFTReg(MN_STATUS,0,FT_NOMAL_PUT,0,4,323,75,23,STATUS_IMG,17,17); // CSD-030324 : 게이지 그림 세모로 만드는거
	::SMenuFTReg(MN_STATUS,4,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_MAININTERFACE_ABILITY,4,323,75,23,STATUS_IMG,18,19); // CSD-030324 
	// 택틱스 메뉴 부르기 
	::SMenuFTReg(MN_STATUS,23,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_STATUS_TACTICS,82,323,75,23,STATUS_IMG,8,9);
	//자신을 닫기
	::SMenuFTReg(MN_STATUS,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LMENUCLOSE,238,323,75,23,STATUS_IMG,14,15);
	//161	32 옵션 버튼//시스템
	::SMenuFTReg(MN_STATUS,26,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_MENU_OPEN,160,323,75,23,STATUS_IMG,11,12,MN_SYSTEM);
	//캐릭터 얼굴찍기
	::SMenuFTReg(MN_STATUS,12,FT_DATA_PUT,0,16,44,0,0,STATUS_IMG,0,DP_FACEIMAGE);
	//캐릭터 이름 찍기
	::SMenuFTReg(MN_STATUS,5,FT_DATA_PUT,0,140,54,0,0,STATUS_IMG,0,DP_NAME_CENTER);
	//클래스 데이타 찍기
	::SMenuFTReg(MN_STATUS,2,FT_DATA_PUT,0,260,54,0,0,STATUS_IMG,0,DP_CLASS);
	//직업 데이타 찍기
	::SMenuFTReg(MN_STATUS,3,FT_DATA_PUT,0,90,135,0,0,STATUS_IMG,0,DP_JOB);
	//길드 표시하기	// ->돈으로 바뀜
	::SMenuFTReg(MN_STATUS,13,FT_DATA_PUT,0,224,135,0,0,STATUS_IMG,0,DP_MONEY);
	//가치관 데이타 찍기 안찍음
//	::SMenuFTReg(MN_STATUS,4,FT_DATA_PUT,0,191,95,0,0,STATUS_IMG,0,DP_ARIGEMENT);
	//캐릭터 레벨 찍기
	::SMenuFTReg(MN_STATUS,8,FT_DATA_PUT,0,96,85,0,0,STATUS_IMG,0,DP_LEVEL);
	//캐릭터 경험치 찍기
	::SMenuFTReg(MN_STATUS,9,FT_DATA_PUT,0,118,86,0,0,STATUS_IMG,0,DP_EXP);
	//다음 경험치 찍기
	::SMenuFTReg(MN_STATUS,22,FT_DATA_PUT,0,209,86,0,0,STATUS_IMG,0,DP_EXP_NEXT);
	//캐릭터 HP찍기  life
	::SMenuFTReg(MN_STATUS,10,FT_DATA_PUT,0,75,172,0,0,STATUS_IMG,0,DP_HP);
	// 명성 찍기
	::SMenuFTReg(MN_STATUS,21,FT_DATA_PUT,0,237,172,0,0,STATUS_IMG,0,DP_FAME);
	//캐릭터 MP찍기  mana
	::SMenuFTReg(MN_STATUS,11,FT_DATA_PUT,0,75,198,0,0,STATUS_IMG,0,DP_MP);
	//BEAT 찍기
	::SMenuFTReg(MN_STATUS,19,FT_DATA_PUT,0,237,198,0,0,STATUS_IMG,0,DP_HEAT);
	//HEALTH 찍기
	::SMenuFTReg(MN_STATUS,17,FT_DATA_PUT,0,75,224,0,0,STATUS_IMG,0,DP_HEALTH);
	//AC 찍기
	::SMenuFTReg(MN_STATUS,20,FT_DATA_PUT,0,237,224,0,0,STATUS_IMG,0,DP_AC);
	//TACTICE 찍기
	::SMenuFTReg(MN_STATUS,15,FT_DATA_PUT,0,75,250,0,0,STATUS_IMG,0,DP_TACTICE);
	//DAMAGE 찍기    //wsps 는 바뀜 ( dp_wsps )
	::SMenuFTReg(MN_STATUS,14,FT_DATA_PUT,0,237,250,0,0,STATUS_IMG,0,DP_DAMAGE);
	// 믿는 신과 신앙심 찍기
//	::SMenuFTReg(MN_STATUS,24,FT_BELIEVE_GOD,0,90,285,0,0,MAIN_ETC,0,1);//020515 lsw
	//MOV 찍기 //없네 ..
//	::SMenuFTReg(MN_STATUS,18,FT_DATA_PUT,0,223,207,0,0,STATUS_IMG,0,DP_MOV);
//	종	90 285
//	신	226 285
	}
/////////////////////////////////////////////////////////////////////////////////////////
//어빌리티 메뉴
	{
	::SMenuReg(MN_ABILITY, 0, 0, STATUS_IMG,1,39,0);
	//스테이터스 메뉴 뛰우기
	//4 82 238 
	::SMenuFTReg(MN_ABILITY,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_MAININTERFACE_ABILITYTOSTATUS,4,323,75,23,STATUS_IMG,5,6);
	// 택틱스 메뉴 부르기 
	::SMenuFTReg(MN_ABILITY,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_STATUS_TACTICS,82,323,75,23,STATUS_IMG,8,9);
	//DONE
	::SMenuFTReg(MN_ABILITY,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LMENUCLOSE,238,323,75,23,STATUS_IMG,14,15);
	//시스템 메뉴 열기
	::SMenuFTReg(MN_ABILITY,38,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_MENU_OPEN,160,323,75,23,STATUS_IMG,11,12,MN_SYSTEM);

	::SMenuFTReg(MN_ABILITY,4,	FT_DATA_PUT,0,97,98,0,0,		STATUS_IMG,0,DP_ABILITY,STR	);
	::SMenuFTReg(MN_ABILITY,5,	FT_DATA_PUT,0,97,98+25,0,0,		STATUS_IMG,0,DP_ABILITY,DEX	);
	::SMenuFTReg(MN_ABILITY,6,	FT_DATA_PUT,0,97,98+25*2,0,0,	STATUS_IMG,0,DP_ABILITY,CON	);
	::SMenuFTReg(MN_ABILITY,7,	FT_DATA_PUT,0,97,98+25*3,0,0,	STATUS_IMG,0,DP_ABILITY,WIS	);
	::SMenuFTReg(MN_ABILITY,8,	FT_DATA_PUT,0,97,98+25*4,0,0,	STATUS_IMG,0,DP_ABILITY,INT_);
	::SMenuFTReg(MN_ABILITY,9,	FT_DATA_PUT,0,97,98+25*5,0,0,	STATUS_IMG,0,DP_ABILITY,MOVP);
	::SMenuFTReg(MN_ABILITY,10,	FT_DATA_PUT,0,97,98+25*6,0,0,	STATUS_IMG,0,DP_ABILITY,ENDU);
	::SMenuFTReg(MN_ABILITY,11,	FT_DATA_PUT,0,97,98+25*7,0,0,	STATUS_IMG,0,DP_ABILITY,CHA	);
	::SMenuFTReg(MN_ABILITY,12,	FT_DATA_PUT,0,97,98+25*8,0,0,	STATUS_IMG,0,DP_ABILITY,LUCK);
	::SMenuFTReg(MN_ABILITY,22,	FT_DATA_PUT,0,253,97,75,23,		STATUS_IMG,0,DP_ABILITY,WSPS);

	::SMenuFTReg(MN_ABILITY,26+0,FT_NO_CHECK,20,135,94+(0)*25,30,15,STATUS_IMG,21,22,STR	);
	::SMenuFTReg(MN_ABILITY,26+1,FT_NO_CHECK,20,135,94+(1)*25,30,15,STATUS_IMG,21,22,DEX	);
	::SMenuFTReg(MN_ABILITY,26+2,FT_NO_CHECK,20,135,94+(2)*25,30,15,STATUS_IMG,21,22,CON	);
	::SMenuFTReg(MN_ABILITY,26+3,FT_NO_CHECK,20,135,94+(3)*25,30,15,STATUS_IMG,21,22,WIS	);
	::SMenuFTReg(MN_ABILITY,26+4,FT_NO_CHECK,20,135,94+(4)*25,30,15,STATUS_IMG,21,22,INT_);
	::SMenuFTReg(MN_ABILITY,26+5,FT_NO_CHECK,20,135,94+(5)*25,30,15,STATUS_IMG,21,22,MOVP);
	::SMenuFTReg(MN_ABILITY,26+6,FT_NO_CHECK,20,135,94+(6)*25,30,15,STATUS_IMG,21,22,ENDU);
	::SMenuFTReg(MN_ABILITY,26+7,FT_NO_CHECK,20,135,94+(7)*25,30,15,STATUS_IMG,21,22,CHA	);
	::SMenuFTReg(MN_ABILITY,26+8,FT_NO_CHECK,20,135,94+(8)*25,30,15,STATUS_IMG,21,22,LUCK);
	::SMenuFTReg(MN_ABILITY,26+9,FT_NO_CHECK,20,289,94+(0)*25,30,15,STATUS_IMG,21,22,WSPS);

//	::SMenuFTReg(MN_ABILITY,12,	FT_DATA_PUT,0,134,312,75,23,STATUS_IMG,0,DP_ABILITY,MOR);
	
	//이름 찍기
	::SMenuFTReg(MN_ABILITY,14,FT_DATA_PUT,0,105,47,0,0,STATUS_IMG,0,DP_NAME_CENTER);
	//클래스 데이타 찍기
	::SMenuFTReg(MN_ABILITY,15,FT_DATA_PUT,0,245,47,0,0,STATUS_IMG,0,DP_CLASS);
	//레벨 찍기
	::SMenuFTReg(MN_ABILITY,16,FT_DATA_PUT,0,97,72,0,0,STATUS_IMG,0,DP_LEVEL);
	//캐릭터 경험치 찍기
	::SMenuFTReg(MN_ABILITY,17,FT_DATA_PUT,0,118,72,0,0,STATUS_IMG,0,DP_EXP);
	::SMenuFTReg(MN_ABILITY,18,FT_DATA_PUT,0,209,72,0,0,STATUS_IMG,0,DP_EXP_NEXT);

	// 위자드냐 프리스트냐에 따른 ...//성직자 주문, 마법사 주문 WS PS 그림 찍는거 직접 엑세스 있습니다. SMenu[MN_ABILITY].nField[21] 로 찾으면 됩니다
	::SMenuFTReg(MN_ABILITY,21,FT_NO_CHECK,0,166,93,0,0,STATUS_IMG,24,24);
	//저항력 찍기
	::SMenuFTReg(MN_ABILITY,24,FT_DATA_PUT,0,250,136,0,0,STATUS_IMG,24,DP_RESIST);
//	SMenu[MN_ABILITY].nField[24].nSHideNomalCount=4;
//	SMenu[MN_ABILITY].nField[24].nSHideNomalStart=0;
//	SMenu[MN_ABILITY].nField[24].nShideNomalPlus=1;

	::SMenuFTReg(MN_ABILITY,25,FT_DATA_PUT,0,256,298,0,0,STATUS_IMG,23,DP_DATAPUT);
	SMenu[MN_ABILITY].nField[25].nValue=&SCharacterData.LvUpPoint;
	}
	{
////////////////////////////////////////////////////////////////////////////////////////////
//	택틱스남 메뉴 뛰우기
	::SMenuReg(MN_TACTICS, 0, 0, STATUS_IMG,2,10,0);
	//스테이터스 메뉴 뛰우기
	//4 82 238 
	::SMenuFTReg(MN_TACTICS,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_TACTICS_STATUS,4,323,75,23,STATUS_IMG,5,6);
	//어빌리티 메뉴 뛰우기
	::SMenuFTReg(MN_TACTICS,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_TACTICS_ABILITY,82,323,75,23,STATUS_IMG,18,19);
	//DONE
	::SMenuFTReg(MN_TACTICS,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LMENUCLOSE,238,323,75,23,STATUS_IMG,14,15);
	
	::SMenuFTReg(MN_TACTICS,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_MENU_OPEN,160,323,75,23,STATUS_IMG,11,12,MN_SYSTEM);
//15 259
	::SMenuFTReg(MN_TACTICS,4,FT_DATA_PUT,0,137,57,0,0,STATUS_IMG,0,DP_TACTICS_DATA,39);

	::SMenuFTReg(MN_TACTICS,6,FT_EXPLAIN_MENU,0,114,21,75,23,STATUS_IMG,0,0,EXPLAIN_TACTICS_MAN);// 여자는 EXPLAIN_TACTICS_WOMAN;//init 메뉴에서 
	//SMenu[MN_TACTICS].nField[6//공격마법 운용 이라는 그림 찍기////15 260  //15 236//::SMenuFTReg(MN_TACTICS,6로 찾으면 됨
	::SMenuFTReg(MN_TACTICS,5,FT_NONE,0,137,57,0,0,STATUS_IMG,25,25,0);
	}
	{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// 파티 메뉴
	::SMenuReg(MN_PARTY, 480, 0, PARTY_IMG,0,15,0);
	// RELEASE button
	::SMenuFTReg(MN_PARTY,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_PARTY_RELEASE,236,195,80,30,PARTY_IMG,11,12,12);
	//자신을 닫기
	::SMenuFTReg(MN_PARTY,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_RMENUCLOSE,236,298,80,30,PARTY_IMG,14,15);
	::SMenuFTReg(MN_PARTY,13,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_PARTY_ALL_DEL,236,246,80,30,PARTY_IMG,17,18);
	// RELATION 버튼 출력
	::SMenuFTReg(MN_PARTY,2,FT_PUT_BUTTON,SWD_PARTY_BUTTON,18,42,60,25,PARTY_IMG,2,3,PS_EXP);
	SMenu[MN_PARTY].nField[2].nSHideNomalNumber=12;
	SMenu[MN_PARTY].nField[2].nSHideNomalCount=2;//평소버튼
		// PARTY 버튼 출력
	::SMenuFTReg(MN_PARTY,3,FT_PUT_BUTTON,SWD_PARTY_BUTTON,117,42,60,25,PARTY_IMG,5,6,PS_TACTIC);
	SMenu[MN_PARTY].nField[3].nSHideNomalNumber=12;
	SMenu[MN_PARTY].nField[3].nSHideNomalCount=5;//평소버튼
	
	// EMPROYMENT 버튼 출력
	::SMenuFTReg(MN_PARTY,4,FT_PUT_BUTTON,SWD_PARTY_BUTTON,216,42,60,25,PARTY_IMG,8,9,PS_MAGIC_TAC);
	SMenu[MN_PARTY].nField[4].nSHideNomalNumber=12;
	SMenu[MN_PARTY].nField[4].nSHideNomalCount=8;//평소버튼
	//다른 파티원들 출력 :
	// 먼저 가까이만 가도 숫자 필드의 숫자에 불이 들어온다. 1번
	// 다른 캐릭터와 관계가 있다면 nSpecialWillDo를 체크(true)해 둔다			// origin 좌표로 이미지를 찍는다.
	//1번
	::SMenuFTReg(MN_PARTY,6,FT_PARTY,12, 55,217,0,0,0,468,468,0);
	::SetRect(SMenu[MN_PARTY].nField[6].rCheakBox,25,182,85,252);//범위 체크 스타일이 다르다
	// 2번
	::SMenuFTReg(MN_PARTY,7,FT_PARTY,12, 124,217,0,0,0,469,469,1);
	::SetRect(SMenu[MN_PARTY].nField[7].rCheakBox,94,182,154,252);
	// 3번
	::SMenuFTReg(MN_PARTY,8,FT_PARTY,12, 193,217,0,0,0,470,470,2);
	::SetRect(SMenu[MN_PARTY].nField[8].rCheakBox,163,182,233,252);
	//4번
	::SMenuFTReg(MN_PARTY,9,FT_PARTY,12, 55,297,0,0,0,471,471,3);
	::SetRect(SMenu[MN_PARTY].nField[9].rCheakBox,25,262,85,332);
	//5번
	::SMenuFTReg(MN_PARTY,10,FT_PARTY,12, 124,297,0,0,0,472,472,4);
	::SetRect(SMenu[MN_PARTY].nField[10].rCheakBox,94,262,154,332);
	//6번
	::SMenuFTReg(MN_PARTY,11,FT_PARTY,12, 193,297,0,0,0,473,473,5);
	::SetRect(SMenu[MN_PARTY].nField[11].rCheakBox,163,262,233,332);
	// 관계된 사람을 클릭했을때, 얼굴과 스킬 보여주기
	::SMenuFTReg(MN_PARTY,12,FT_PARTY_FACE,0, 80,124,0,0,0,0,0,0);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	//메인 인터페이스 메뉴
	{
#ifndef _SDL
		::SMenuReg(MN_MAININTERFACE, 0, 371 + GABY_SCREEN * 2, MAININTERFACE_IMG, 0, 50, 0); //y += 168 for 1024 - Thralas
#else
		::SMenuReg(MN_MAININTERFACE, (SCREEN_WIDTH / 2) - 400, SCREEN_HEIGHT - 110, MAININTERFACE_IMG, 0, 50, 0);
#endif

	
	//메인 메뉴 뛰우기	OK
	SMenu[MN_MAININTERFACE].bActive=TRUE;
	//스테이터스 메뉴 뛰우기 OK
	::SMenuFTReg(MN_MAININTERFACE,STATUS_FIELD,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_MAININTERFACE_STATUS,194,67,40,40,MAININTERFACE_IMG,30,31);
	//SP 밥 게이지 띄우기 OK
	::SMenuFTReg(MN_MAININTERFACE,1,FT_DATA_PUT,0,68,41,0,0,MAININTERFACE_IMG,5,DP_HEALTH_GAUGE);
	//이모티콘	
	::SMenuFTReg(MN_MAININTERFACE,2,FT_DO,0,66,12,30,20,MAININTERFACE_IMG,3,4,DO_FACE_MENU);
	//얼굴표정 그림 뛰우기
	::SMenuFTReg(MN_MAININTERFACE,3,FT_DATA_PUT,0,66,12,30,20,MAININTERFACE_IMG,4,DP_FACE,0);
	//HP게이지 뛰우기	OK
	::SMenuFTReg(MN_MAININTERFACE,4,FT_DATA_PUT,0,0,2,0,0,HP_IMG,2,DP_HP_BAR);
	//MP게이지 뛰우기	OK
	::SMenuFTReg(MN_MAININTERFACE,5,FT_DATA_PUT,0,723,2,0,0,MANA_IMG,2,DP_MP_BAR);//MANA_IMG DIVINE_IMG로 자동
	//이름 뛰우기
	::SMenuFTReg(MN_MAININTERFACE,6,FT_DATA_PUT,0,267,46,0,0,0,0,DP_NAME_CENTER);
	//경험치 띄우기
	::SMenuFTReg(MN_MAININTERFACE,7,FT_DATA_PUT,0,397,46,0,0,MAININTERFACE_IMG,28,DP_MAIN_LEVEL);
	//레벨 뛰우기
	::SMenuFTReg(MN_MAININTERFACE,8,FT_DATA_PUT,0,350,46,0,0,MAININTERFACE_IMG,28,DP_LEVEL_IN_TIME_MENU);
	//마우스를 체크하는 단축 아이템 메뉴 뛰우기
	::SMenuFTReg(MN_MAININTERFACE,11,FT_ACCEL_INVENTORY,0,461,69,33*5,35,0,0,0);
	// MAP
	::SMenuFTReg(MN_MAININTERFACE,MAP_FIELD,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_INFO_FLAG_SET,673,83,66,23,MAININTERFACE_IMG,53,54,IF_MAP);
	//TodayNotice 버튼
	::SMenuFTReg(MN_MAININTERFACE,TODAY_NOTICE_FIELD,FT_TODAY_NOTICE,0,674,28,33,33,MAININTERFACE_IMG,55,56,0);
	// battle 바뀌는 버튼 출력 //acer7
	::SMenuFTReg(MN_MAININTERFACE,PEACE_BATTLE_FIELD,FT_PEACE_BATTLE_PUT,0,673,59,66,23,MAININTERFACE_IMG,53,54);//50 51 피스 59 60 배틀 62 63 피케이
	//전투스킬 메뉴 띄우기
	::SMenuFTReg(MN_MAININTERFACE,COMBAT_SKILL_POINT_FIELD,FT_MAIN_MENU,SWD_CALL_COMBAT_SKILL_POINT,369,67,40,40,MAININTERFACE_IMG,42,43);//020515 lsw
	//매직 메뉴 띄우기
	::SMenuFTReg(MN_MAININTERFACE,MAGIC_FIELD,FT_MAIN_MENU,SWD_MAININTERFACE_MAGIC,326,67,40,40,MAININTERFACE_IMG,38,40);
	//아이템 메뉴 띄우기
	::SMenuFTReg(MN_MAININTERFACE,INVENTORY_FIELD,FT_MAIN_MENU,SWD_MAININTERFACE_ITEM,237,67,40,40,MAININTERFACE_IMG,33,34);
	//파티 메뉴 띄우기
	::SMenuFTReg(MN_MAININTERFACE,PARTY_FIELD,FT_MAIN_MENU,SWD_MAININTERFACE_PARTY,282,67,40,40,MAININTERFACE_IMG,36,37);
	// 시스템 메뉴 띄우기
	::SMenuFTReg(MN_MAININTERFACE,OPTION_FIELD,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_MENU_OPEN,638,75,31,31,MAININTERFACE_IMG,47,47,MN_SYSTEM);
	//스킬 메뉴 띄우기
	::SMenuFTReg(MN_MAININTERFACE,SKILL_FIELD,FT_MAIN_MENU,SWD_MAININTERFACE_SKILL,414,67,40,40,MAININTERFACE_IMG,45,46,MN_QUICK_SKILL);
	//국가전 메뉴
	::SMenuFTReg(MN_MAININTERFACE,NATION_BBS_FIELD,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALL_MENU,701,21,40,36,MAININTERFACE_IMG,65,66,MN_NATION_BATTLE_BBS);
	::SMenuFTReg(MN_MAININTERFACE,HOT_KEY_FIELD,FT_HOTKEY_ICON,0,HOTKEY_ICON_POS_X,HOTKEY_ICON_POS_Y,40,40,COMBAT_SKILL_ICON_IMG,90,90,0);//전투스킬에 있는 아이콘을 쓴다
	///////////////////// 채팅 /////////////////////
	// 입력란 
	::SMenuFTReg(MN_MAININTERFACE,31,FT_DO,0,103,4,0,0,MAININTERFACE_IMG,67,68,DO_CHATTING_EDITBOX);
	// 메일 메뉴 띄우기
	::SMenuFTReg(MN_MAININTERFACE,MAIL_FIELD,FT_MAININTERFACE_MAIL_CALL,SWD_NONE,
		638,42,31,31,MAININTERFACE_IMG,48,48,MVT_RECV_LIST);
	//포션박스
	::SMenuFTReg(MN_MAININTERFACE,38,FT_POTION_BOX,0,790,-20,0,0,0,0,0,0);
	//택틱 디스플레이
	::SMenuFTReg(MN_MAININTERFACE,39,FT_DISPLAY_MAGIC_EXP,0,66,53,30,30,MAININTERFACE_IMG,16,27);
	::SMenuFTReg(MN_MAININTERFACE,40,FT_DISPLAY_TAC_EXP,0,66,72,30,30,MAININTERFACE_IMG,16,27);
	
	::SMenuFTReg(MN_MAININTERFACE,41,FT_DISPLAY_SKILL_EXP,0,66,91,30,30,MAININTERFACE_IMG,16,27);
	
	//	귓말 대상 선택창//42를 MunuEdit 에서 쓴다//쬐끄만 버튼
	::SMenuFTReg(MN_MAININTERFACE,42,FT_SELECT_WISPER_TARGET,0,145,6,15,15,MAININTERFACE_IMG,76,75,0);

	//길드채팅, 보통 채팅 선택//SMenu[MN_MAININTERFACE].nField[44].nWillDo 44번 바꿀때 조심 직접 엑세스..
	//nWillDo 0 이면 보통 , 1 이면 길드 //여기서 포커스 강제 셋팅및 버튼 셋팅 까지/SMenu/끄트머리에 있는 버튼
	::SMenuFTReg(MN_MAININTERFACE,44,FT_SELECT_CHAT_TYPE,0,609,7,30,25,MAININTERFACE_IMG,72,71,73);

	::SMenuFTReg(MN_MAININTERFACE,45,FT_NOMAL_PUT,0,66,52,0,0,MAININTERFACE_IMG,96,96);//게이지 그림 세모로 만드는거
	//친구 메뉴
	::SMenuFTReg(MN_MAININTERFACE,FRIEND_FIELD,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_CALL_FRIEND_MENU,100,17,30,20,MAININTERFACE_IMG,97,97);//친구 버튼

	//Lock Button	nWillDo 0 --> UnLock 1 -> lock
	::SMenuFTReg(MN_MAININTERFACE,47,FT_CHAT_LOCK,0,639,7,30,35,MAININTERFACE_IMG,69,70,1);//Lock Button//nWillDo가 1 이면 Lock 있음

	::SMenuFTReg(MN_MAININTERFACE,9,FT_HUNTER_VERSUS_DISPLAY,0,639,-200,30,35,MAININTERFACE_IMG,69,70,1);//021001 lsw

	}
	{
	/////////////////////////////////////////////////////////////////////////////////
// 메인 메뉴 설명 메뉴 
	SMenu[MN_MAIN_EXPLAIN].x=0;
	SMenu[MN_MAIN_EXPLAIN].y=371+GABY_SCREEN*2;
	SMenu[MN_MAIN_EXPLAIN].CheakType=0;
	SMenu[MN_MAIN_EXPLAIN].nFieldCount=1;  

	SMenu[MN_MAIN_EXPLAIN].nField[0].nType=FT_EXPLAIN_MENU;
	SMenu[MN_MAIN_EXPLAIN].nField[0].nWillDo = EXPLAIN_MAININTERFACE;
	}
	{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//얼굴 표정 선택 메뉴 이모티콘 메뉴
	//메인메뉴 뛰우기
	/* 031021 이전 앤날 이모티콘 띄우는 방
	::SMenuReg(MN_FACE, 98, 360+GABY_SCREEN*2, NEW_EMOTE_IMG,0,9,0);
	//표정 선택
	::SMenuFTReg(MN_FACE,0,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+0,	8+0,30,30,	NEW_EMOTE_IMG,2,2);
	::SMenuFTReg(MN_FACE,1,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40,	8+0,30,30,	NEW_EMOTE_IMG,3,3);
	::SMenuFTReg(MN_FACE,2,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*2,8+0,30,30,	NEW_EMOTE_IMG,4,4);
	::SMenuFTReg(MN_FACE,3,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*3,8+0,30,30,	NEW_EMOTE_IMG,5,5);
	::SMenuFTReg(MN_FACE,4,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,4+40*4,8+0,30,30,	NEW_EMOTE_IMG,6,6);//이미지 때문에 보정
	::SMenuFTReg(MN_FACE,5,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*5,8+0,30,30,	NEW_EMOTE_IMG,7,7);
	::SMenuFTReg(MN_FACE,6,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*6,8+0,30,30,	NEW_EMOTE_IMG,8,8);
	::SMenuFTReg(MN_FACE,7,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,8+40*7,8+0,30,30,	NEW_EMOTE_IMG,9,9);
	
	::SMenuFTReg(MN_FACE,8,FT_SPECIAL_WILLDO,SWD_UNMOUSEFOCUS,0,0,0,0,0,0,0);
	*/
	//<< 031021 kyo
	::SMenuReg(MN_FACE, 98, 260+GABY_SCREEN*2, NEW_EMOTE_IMG,0,34,0);
	
	::SMenuFTReg(MN_FACE,0,FT_NOMAL_PUT,SWD_NONE,-10    ,-1,30,30,	NEW_EMOTE_IMG,1,1);		// BBD 040413
//<! BBD 040112 에이서 이모티콘 변경
	int acer = 0;
#ifdef TAIWAN_LOCALIZING_
	acer = 1;
#endif
#ifdef HONGKONG_LOCALIZING_
	acer = 1;
#endif
#ifdef CHINA_LOCALIZING_
	acer = 1;
#endif

	if(acer) //에이서면 이모티콘의 메뉴를 뿌리는 좌표가 다르다
	{
		::SMenuFTReg(MN_FACE,1,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,6+0,		8+0,30,30,NEW_EMOTE_IMG,2,2);
		::SMenuFTReg(MN_FACE,2,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,6+41,	8+0,30,30,NEW_EMOTE_IMG,3,3);
		::SMenuFTReg(MN_FACE,3,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,6+41*2,	8+0,30,30,NEW_EMOTE_IMG,4,4);
		::SMenuFTReg(MN_FACE,4,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,6+41*3,	8+0,30,30,NEW_EMOTE_IMG,5,5);
		::SMenuFTReg(MN_FACE,5,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,41*4,	8+0,30,30,NEW_EMOTE_IMG,6,6);
		::SMenuFTReg(MN_FACE,6,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,6+41*5,	8+0,30,30,NEW_EMOTE_IMG,7,7);
		::SMenuFTReg(MN_FACE,7,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,6+41*6,	8+0,30,30,NEW_EMOTE_IMG,8,8);
		::SMenuFTReg(MN_FACE,8,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,6+41*7,	8+0,30,30,NEW_EMOTE_IMG,9,9);
	}
	else
	{
		::SMenuFTReg(MN_FACE,1,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,2,	2,30,30,NEW_EMOTE_IMG,2,2);
		::SMenuFTReg(MN_FACE,2,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,45,	3,30,30,NEW_EMOTE_IMG,3,3);
		::SMenuFTReg(MN_FACE,3,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,85,	4,30,30,NEW_EMOTE_IMG,4,4);
		::SMenuFTReg(MN_FACE,4,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,125,	4,30,30,NEW_EMOTE_IMG,5,5);
		::SMenuFTReg(MN_FACE,5,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,165,	5,30,30,NEW_EMOTE_IMG,6,6);
		::SMenuFTReg(MN_FACE,6,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,205,	3,30,30,NEW_EMOTE_IMG,7,7);
		::SMenuFTReg(MN_FACE,7,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,250,	4,30,30,NEW_EMOTE_IMG,8,8);
		::SMenuFTReg(MN_FACE,8,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,288,	5,30,30,NEW_EMOTE_IMG,9,9);
	}
//> BBD 040112 에이서 이모티콘 변경
	::SMenuFTReg(MN_FACE,9,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,331,	3,30,30,NEW_EMOTE_IMG,10,10);
	::SMenuFTReg(MN_FACE,10,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,370,4,30,30,NEW_EMOTE_IMG,11,11);

	::SMenuFTReg(MN_FACE,11,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,0,	45,30,30,NEW_EMOTE_IMG,12,12);
	::SMenuFTReg(MN_FACE,12,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,45,	48,30,30,NEW_EMOTE_IMG,13,13);
	::SMenuFTReg(MN_FACE,13,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,81,	43,30,30,NEW_EMOTE_IMG,14,14);
	::SMenuFTReg(MN_FACE,14,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,124,	41,30,30,NEW_EMOTE_IMG,15,15);
	::SMenuFTReg(MN_FACE,15,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,165,	45,30,30,NEW_EMOTE_IMG,16,16);
	::SMenuFTReg(MN_FACE,16,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,205,	45,30,30,NEW_EMOTE_IMG,17,17);
	::SMenuFTReg(MN_FACE,17,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,246,	48,30,30,NEW_EMOTE_IMG,18,18);
	::SMenuFTReg(MN_FACE,18,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,286,	42,30,30,NEW_EMOTE_IMG,19,19);
	::SMenuFTReg(MN_FACE,19,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,331,	44,30,30,NEW_EMOTE_IMG,20,20);
	::SMenuFTReg(MN_FACE,20,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,367,	42,30,30,NEW_EMOTE_IMG,21,21);
	::SMenuFTReg(MN_FACE,21,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,6,	90,30,30,NEW_EMOTE_IMG,22,22); // finito added the old emotions 030707
	::SMenuFTReg(MN_FACE,22,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,47,	90,30,30,NEW_EMOTE_IMG,23,23);
	::SMenuFTReg(MN_FACE,23,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,88,	90,30,30,NEW_EMOTE_IMG,24,24);
	::SMenuFTReg(MN_FACE,24,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,129, 90,30,30,NEW_EMOTE_IMG,25,25);
	::SMenuFTReg(MN_FACE,25,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,167, 90,30,30,NEW_EMOTE_IMG,26,26);
	::SMenuFTReg(MN_FACE,26,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,211, 90,30,30,NEW_EMOTE_IMG,27,27);
	::SMenuFTReg(MN_FACE,27,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,251, 90,30,30,NEW_EMOTE_IMG,28,28);
	::SMenuFTReg(MN_FACE,28,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,293, 90,30,30,NEW_EMOTE_IMG,29,29);
	::SMenuFTReg(MN_FACE,29,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,331,	87,30,30,NEW_EMOTE_IMG,30,30); //Eleval 22/06/09 - New Emotes
	::SMenuFTReg(MN_FACE,30,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,370,	87,30,30,NEW_EMOTE_IMG,31,31);
	::SMenuFTReg(MN_FACE,31,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,412,	5,30,30,NEW_EMOTE_IMG,32,32);
	::SMenuFTReg(MN_FACE,32,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,412,	45,30,30,NEW_EMOTE_IMG,33,33);
	::SMenuFTReg(MN_FACE,33,FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,SWD_FACE_CHOOSE,412,	88,30,30,NEW_EMOTE_IMG,34,34);
	//>> 031012 kyo
	}
	{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//채팅타입 선택
	//메인메뉴 뛰우기
	::SMenuReg(MN_WISPER_LIST, 140, 350+GABY_SCREEN*2, 0,0,15,0);
	//표정 선택
	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_NORMAL,FT_SELECT_WISPER,CHATTYPE_NORMAL	,8,	8-0   ,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_NORMAL);
	strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_NORMAL].temp,lan->OutputMessage(2,625));//보통채팅
	
	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_GUILD,FT_SELECT_WISPER,CHATTYPE_GUILD		,8,	8-18  ,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_GUILD);
	strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_GUILD].temp,lan->OutputMessage(2,626));//길드채팅

	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_PARTY,FT_SELECT_WISPER,CHATTYPE_PARTY		,8,	8-18*2  ,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_PARTY);
	strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_PARTY].temp,lan->OutputMessage(2,628));//길드채팅

	::SMenuFTReg(MN_WISPER_LIST,SELECT_WISPER_MODE,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*3,130,18,	MAININTERFACE_IMG,78,77,SELECT_WISPER_MODE);
	strcpy(SMenu[MN_WISPER_LIST].nField[SELECT_WISPER_MODE].temp,lan->OutputMessage(2,627));//귓속말 상대입력
	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER1		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*4,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER1		);
	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER2		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*5,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER2		);
	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER3		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*6,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER3		);
	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER4		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*7,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER4		);
	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER5		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*8,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER5		);
//	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER6		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*8,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER6		);
//	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER7		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*9,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER7		);
//	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER8		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*10,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER8		);
//	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER9		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*11,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER9		);
//	::SMenuFTReg(MN_WISPER_LIST,CHAT_TARGET_WISPER10		,FT_SELECT_WISPER,CHATTYPE_WHISPER	,8,	8-18*12,130,18,	MAININTERFACE_IMG,78,77,CHAT_TARGET_WISPER10	);
		
/*	SMenu[MN_FACE].nField[8].nType=FT_SPECIAL_WILLDO;
	SMenu[MN_FACE].nField[8].nSpecialWillDo=SWD_UNMOUSEFOCUS;
	SMenu[MN_FACE].nField[8].fCheakFlag=FALSE;
	SMenu[MN_FACE].nField[8].fLButtonDown=FALSE;
//	::SetRect(SMenu[MN_FACE].nField[8].rCheakBox,-118,-369,486,111);
*/	}

	
//	MN_DISPLAY_SKILL_EXP//사라짐 3가지 종류 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//메인인터페이스에서 시스템 메뉴
	{
	::SMenuReg(MN_SYSTEM, 215+GABX_SCREEN, 57+GABY_SCREEN, NEW_SYSTEM_IMG,0,7,0);
	//메뉴 글자
	/*
	::SMenuFTReg(MN_SYSTEM,1,FT_HIDE_NOMAL_GROUP_PUT,SWD_SMALLMENU_END,66,188,60,30,NEW_SYSTEM_IMG,1,1);
	SMenu[MN_SYSTEM].nField[1].nSHideNomalNumber=HN_SYSTEM_TEXT;
	SMenu[MN_SYSTEM].nField[1].nSHideNomalCount=3;
	SMenu[MN_SYSTEM].nField[1].nSHideNomalStart=0;
	SMenu[MN_SYSTEM].nField[1].nShideNomalPlus=3;
	*/
	//마을로 가기
	::SMenuFTReg(MN_SYSTEM,5,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_MOVE_TO_HOMETOWN	,27,46,100,25,NEW_SYSTEM_IMG,1,1,MN_SYSTEM_LOGOUT);
	//option용 사각형
	::SMenuFTReg(MN_SYSTEM,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_FOCUS_WILLDO	,27,70,100,25,NEW_SYSTEM_IMG,1,1,MN_SYSTEM_OPTION);
	//help용 사각형
	::SMenuFTReg(MN_SYSTEM,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_HELP			,27,94,100,25,NEW_SYSTEM_IMG,1,1);
	//logout용 사각형
	::SMenuFTReg(MN_SYSTEM,4,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_FOCUS_WILLDO	,27,118,100,25,NEW_SYSTEM_IMG,1,1,MN_SYSTEM_LOGOUT);
	//exit
	::SMenuFTReg(MN_SYSTEM,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALLMENU_END,49,152,60,30,NEW_SYSTEM_IMG,2,3);
	}
	{
		::SMenuReg(MN_GHOST_TO_ALIVE, 215+GABX_SCREEN, 57+GABY_SCREEN, NEW_GHOST_TO_ALIVE_IMG,0,4,0);
		::SMenuFTReg(MN_GHOST_TO_ALIVE,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_MOVE_TO_HOMETOWN_WITH_ALIVE,22,23,153,25,NEW_GHOST_TO_ALIVE_IMG,1,1);
		::SMenuFTReg(MN_GHOST_TO_ALIVE,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,0,22,48,153,25,NEW_GHOST_TO_ALIVE_IMG,1,1);
	}
	{//021001 lsw
//		::SMenuReg(MN_CHATTING, 10, 355, 0,0,10,0);
		::SMenuReg(MN_CHATTING, 0, 370, NEW_CHAT_BACK_IMG,0,10,0);
		//드래그 존	//드래그존은 항상 마지막 필드여야 한다
		::SMenuFTReg(MN_CHATTING,9,FT_DRAG_ZONE,0,					3,5,19,20,NEW_CHAT_BACK_IMG,1,2);
		//버튼들
		::SMenuFTReg(MN_CHATTING,1,FT_CHAT_VIEW_SET,CHATTYPE_NORMAL,	3,24,28,20,NEW_CHAT_BACK_IMG,3,4,1);
		::SMenuFTReg(MN_CHATTING,2,FT_CHAT_VIEW_SET,CHATTYPE_WHISPER,	3,45+4,28,20,NEW_CHAT_BACK_IMG,5,6,1);
		::SMenuFTReg(MN_CHATTING,3,FT_CHAT_VIEW_SET,CHATTYPE_GUILD,	3,65+9,28,20,NEW_CHAT_BACK_IMG,7,8,1);
		::SMenuFTReg(MN_CHATTING,4,FT_CHAT_VIEW_SET,CHATTYPE_PARTY,	3,85+14,28,20,NEW_CHAT_BACK_IMG,13,12,1); // Finito for party chat icon an stuff
		//대화 찍힘
		::SMenuFTReg(MN_CHATTING,6,FT_DO,0,22,-30,0,0,NEW_CHAT_BACK_IMG,0,0,DO_CHATTING_VIEW); // Finito for party chat icon an stuff
	//	#define FT_CHAT_SCROLL					121		// 채팅시 스크롤 버튼 
	}
///////////////////////////////////////////////////////////////////////////////////////
// 사용한 매직 저장하고 보여주는 메뉴 
	{
	SMenu[MN_QUICK_MAGIC_PLUS].bActive=true;
#ifndef _SDL
	SMenu[MN_QUICK_MAGIC_PLUS].x = 0;
	SMenu[MN_QUICK_MAGIC_PLUS].y = 494;
#else
	SMenu[MN_QUICK_MAGIC_PLUS].x = 0;
	SMenu[MN_QUICK_MAGIC_PLUS].y = 694 - 50;
#endif
	SMenu[MN_QUICK_MAGIC_PLUS].nFieldCount=2;

	// 단축 매직 추가 ( 사용한 매직 누적 시키기 )
	SMenu[MN_QUICK_MAGIC_PLUS].nField[0].nType=FT_QUICK_MAGIC_PLUS;
	SMenu[MN_QUICK_MAGIC_PLUS].nField[0].x=0;			// 몇개냐에 따라 다르다...
	SMenu[MN_QUICK_MAGIC_PLUS].nField[0].y=-54;
	SMenu[MN_QUICK_MAGIC_PLUS].nField[0].nImageType=MAIN_ETC;

	// 클릭한 마법을 시전에 성공하면 그 마법 이름 보여주기
	SMenu[MN_QUICK_MAGIC_PLUS].nField[1].nType=FT_USED_MAGIC_EXPLAIN;
	SMenu[MN_QUICK_MAGIC_PLUS].nField[1].x=0;			// 몇개냐에 따라 다르다...
	SMenu[MN_QUICK_MAGIC_PLUS].nField[1].y=-54;
	SMenu[MN_QUICK_MAGIC_PLUS].nField[1].nImageType=MAIN_ETC;

	///////////////////////////////////////////////////////////////////////////////////////
// 단축 마법 스크롤에 붙어 있는 시간 메뉴 단축키
	SMenu[MN_TIME_DISPLAY_ON_OFF].bActive=true;
	SMenu[MN_TIME_DISPLAY_ON_OFF].nImageNumber=22;
	SMenu[MN_TIME_DISPLAY_ON_OFF].nImageType=MAIN_ETC;
	SMenu[MN_TIME_DISPLAY_ON_OFF].nFieldCount=1;

	// 단축 매직 추가 ( 사용한 매직 누적 시키기 )
	SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].nType=FT_TIME_DISPLAY_ON_OFF;
	SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].x=5;
	SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].y=3;
	SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].nImageNumber=14;
	SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].nImageType=MAIN_ETC;
	::SetRect(SMenu[MN_TIME_DISPLAY_ON_OFF].nField[0].rCheakBox,4,0,32,30);

////////////////////////////////////////////////////////////////////////////////
//	단축 마법 배열 스크롤 메뉴
#ifndef _SDL
	SMenu[MN_QUICK_MAGIC_SCROLL].x = 261;
	SMenu[MN_QUICK_MAGIC_SCROLL].y = 441;
#else // !_SDL

	SMenu[MN_QUICK_MAGIC_SCROLL].x=461;
	SMenu[MN_QUICK_MAGIC_SCROLL].y=641;
#endif
	SMenu[MN_QUICK_MAGIC_SCROLL].nImageNumber=21;
	SMenu[MN_QUICK_MAGIC_SCROLL].nImageType=MAIN_ETC; 
	SMenu[MN_QUICK_MAGIC_SCROLL].nFieldCount=2;

	SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nType=FT_QUICK_MAGIC_ARRAY_SCROLL;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].x=5;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].y=5;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nWillDo=0;			// 0: up 버튼
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nRectImage=18;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nImageNumber=19;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].nImageType=MAIN_ETC;
	::SetRect(SMenu[MN_QUICK_MAGIC_SCROLL].nField[0].rCheakBox,3,3,23,20);

	SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nType=FT_QUICK_MAGIC_ARRAY_SCROLL;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].x=5;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].y=34;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nWillDo=1;			// 1: down 버튼
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nRectImage=17;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nImageNumber=20;
	SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].nImageType=MAIN_ETC;
	::SetRect(SMenu[MN_QUICK_MAGIC_SCROLL].nField[1].rCheakBox,3,32,23,49);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
//	퀵스킬 메뉴 
	{//y좌표는 채팅창 따라 바뀝니다.
		const int iMnIndex	= MN_QUICK_SKILL;
		const int iImg		= SKILL_MENU;
		int iFtNum = 0;

#ifndef _SDL
		::SMenuReg(iMnIndex, CENTER_X, 426, iImg, 0, 5);
#else // !_SDL

		::SMenuReg(iMnIndex, CENTER_X, 426, iImg,0,5);
#endif
		// 스킬 아이콘 화면 출력 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_QUICK_SKILL_PUT, 3, 
			46,22, 35,35, iImg, 0, 3);
		// scroll up -- 제목 	
		::SMenuFTReg(iMnIndex,iFtNum++,FT_QUICK_SKILL_SCROLL, 0,
			7,10, 10,15, iImg, 0, 1,0);
		// scroll down -- 제목 
		::SMenuFTReg(iMnIndex,iFtNum++,FT_QUICK_SKILL_SCROLL, 0,
			26,10, 10,15, iImg, 0, 2,1);
		// scroll left -- 아이콘
		::SMenuFTReg(iMnIndex,iFtNum++,FT_QUICK_SKILL_SCROLL_LR, 0, 
			0,34, 24,32, iImg, 0, 6,0);
		// scroll right -- 아이콘  
		::SMenuFTReg(iMnIndex,iFtNum++,FT_QUICK_SKILL_SCROLL_LR, 0, 
			22,34, 24,32, iImg, 0, 9,1);
	}
}

void lsw_DoRButtonDownOfMenu(const int i,const int j)//우버튼 클릭
{
	switch(SMenu[i].nField[j].nType)
	{
  	case FT_CSD_MAIN_ICON://우클릭시 클리어
		{ 
			::CSDMainIconClear();
		}break;
	  case FT_CSD_ICON:
		{
			switch(SMenu[i].nField[j].nSpecialWillDo)
			{
			case SWD_SC_ICON_DOING:
				{
					SetSCDMainAndOtherClear(SMenu[i].nField[1].nWillDo);
				}break;
			}
		}break;
	default :
		break;
	}
}

void lsw_DoLButtonDownOfMenu(const int i,const int j)
{	//버튼을 누르고 있을때 호출 됨
	SMENU *SubMenu	= &SMenu[i];

	int &fRcMouse	= SubMenu->nField[j].fRectMouse;
	int &fLBDown	= SubMenu->nField[j].fLButtonDown;

	int &iImgType	= SubMenu->nField[j].nImageType;
	int &iRcImg		= SubMenu->nField[j].nRectImage;
	int &iImgNo		= SubMenu->nField[j].nImageNumber;

	int &iFieldX	= SubMenu->nField[j].x;
	int &iFieldY	= SubMenu->nField[j].y;
	
	int &iMainX		= SubMenu->x;
	int &iMainY		= SubMenu->y;
	int &iWork		= SubMenu->work;

	int &iSwd		= SubMenu->nField[j].nSpecialWillDo;

	int &nWillDo	=SubMenu->nField[j].nWillDo;
	const int iType = SubMenu->nField[j].nType;

	switch( iType )
	{
	case FT_ACCEL_INVENTORY :
		{
			const int ItemX = ( g_pointMouseX - (SMenu[i].x + SMenu[i].nField[j].x) ) / 33;
			if( 0 > ItemX || 5<= ItemX)
			{
				break;
			}
			if( g_MouseItemType )	
			{
				break;
			}
			//단축 인멘토리 창에 아이템이 있으면 얻는다. 
			if( QuickItemAttr[ItemX].item_no )
			{
				if( IsExchangeNow() )
				{
					int a;
					for(a=0; a<MAX_EXCHANGE_ITEM; a++)
					{
						if( item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type == QUICK && item_give[a].item.item_pos.p3 == ItemX )
							break;
					}
					if( a != MAX_EXCHANGE_ITEM ) break;
				}

				ItemSoundOfGetItem( QuickItemAttr[ItemX].item_no );
				g_MouseItemType = 1;
				g_MouseItemNumber = QuickItemAttr[ItemX].item_no;
				HandItemAttr = QuickItemAttr[ItemX];
				DeleteItem( &QuickItemAttr[ItemX]);

				SetItemPos(QUICK, ItemX, &IP_base);
				POS pos_t;
				SetItemPos(HAND, &pos_t);
				SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
			}
		 }break;
	case FT_CANDLE_INVENTORY://양초
		{
			const int ItemX = 5;
			if( g_MouseItemType )	
			{
				break;
			}
			//단축 인멘토리 창에 아이템이 있으면 얻는다. 
			if( QuickItemAttr[ItemX].item_no )
			{
				if( IsExchangeNow() )
				{
					int a;
					for(a=0; a<MAX_EXCHANGE_ITEM; a++)
					{
						if( item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type == QUICK && item_give[a].item.item_pos.p3 == ItemX )
							break;
					}
					if( a != MAX_EXCHANGE_ITEM ) break;
				}

				ItemSoundOfGetItem( QuickItemAttr[ItemX].item_no );
				g_MouseItemType = 1;
				g_MouseItemNumber = QuickItemAttr[ItemX].item_no;
				HandItemAttr = QuickItemAttr[ItemX];
				DeleteItem( &QuickItemAttr[ItemX]);

				SetItemPos(QUICK, ItemX, &IP_base);
				POS pos_t;
				SetItemPos(HAND, &pos_t);
				SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
			}
		 }break;
	case FT_EXCHANGE_ITEM : 
		{
			if(!IsExchangeNow())
			{
				break;
			}
			int iSideInfo = SMenu[i].nField[j].nWillDo;//LEFT? OR RIGHT
			::DeleteItemAtExchangeGiveItemList(iSideInfo);
		}break;
	case FT_GM_2ND_SKILL_MAKE_ITEM:	
		{//011008 lsw  >작업대의 물건을 손으로 가져감. 아무것도 없을때는 목표물을 제거
			if( g_MouseItemType ) 
			{//손에 뭘 들고 있다.
				break;
			}
			int a;
			for( a=0; a<MAX_UPGRADE_RARE; a++ )
			{//아템 번호가 있고 마우스 역역 체크가 되었다
				if( gm_make_item_list[a].item_no 
				&&	MouseInRectCheak( SMenu[i].x, SMenu[i].y, gm_make_item_list[a].rect ) )
				{
					g_MouseItemType = 1;//마우스에 아이템을 붙여 준다
					g_MouseItemNumber = gm_make_item_list[a].item_no;
					HandItemAttr = gm_make_item_list[a].attr;//
					IP_base = gm_make_item_list[a].pos;

					POS pos_t;
					SetItemPos(HAND, &pos_t);
					SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
					memset( &gm_make_item_list[a], 0, sizeof( MakeItem ) );
					DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
					break;
				}
			}
			int temp =0;

			for( a=0; a<MAX_UPGRADE_RARE; a++ )
			{
				if(gm_make_item_list[a].item_no)
				{
					temp++;
				}
			}
			
			if(!temp)
			{
				ZeroMemory(&gm_make_item_result,sizeof(MakeItem));
				ZeroMemory(&Soksungattr_gm,sizeof(int)); 
			}
		}break;
	case FT_SKILL_MASTER_MAKE_ITEM://021111 lsw
		{
			if( g_MouseItemType ) 
			{//손에 뭘 들고 있다.
				break;
			}
			for( int a=0; a<2; a++ )
			{
				const ItemAttr Item = g_MultiRareSrc.MultiRareSrc[a].attr ;
				if( Item.item_no
				&& MouseInRectCheak( SMenu[i].x, SMenu[i].y, g_MultiRareSrc.MultiRareSrc[a].rect ) )
				{
					g_MouseItemType = 1;
					g_MouseItemNumber = Item.item_no;
					HandItemAttr = Item;
					IP_base = g_MultiRareSrc.MultiRareSrc[a].pos;

					POS pos_t;
					SetItemPos(HAND, &pos_t);
					SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
					memset( &g_MultiRareSrc.MultiRareSrc[a], 0, sizeof( MakeItem ) );
					DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
					break;
				}
			}
		}break;
	case FT_MERCHANT_SELL_ITEM_DROP_ZONE://올려진걸 다시 줍는 부분
		{//021126 lsw
			if( g_MouseItemType ) 
			{//손에 뭘 들고 있다.
				break;
			}
			if(fRcMouse)//마우스가 위 일때
			{
				g_MouseItemType = 1;//마우스에 아이템을 붙여 준다
				const ItemAttr *item = Auction.GetSellItemAttr();//아이템 Pos 받기
				if(!item){break;}//아이템이 없으면 안되지.

				g_MouseItemNumber = item->item_no;
				HandItemAttr = *item;//
				IP_base = Auction.GetSellItemPos();

				POS pos_t;
				::SetItemPos(HAND, &pos_t);
				::SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
				Auction.ClearSellItem();
				::DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
			}//if(fRcMouse)//마우스가 위 일때
		}break;
	case FT_EXCHANGE_BOND_MONEY_ITEM:
		{
			if( g_MouseItemType ) 
			{//손에 뭘 들고 있다.
				break;
			}
			if(fRcMouse)//마우스가 위 일때
			{
				g_MouseItemType = 1;
				const ItemAttr *pItem = Auction.GetBondItemAttr();
				if(!pItem){break;}//아이템이 없으면 안되지.

				g_MouseItemNumber = pItem->item_no;
				HandItemAttr = *pItem;//
				IP_base = Auction.GetBondItemPos();

				POS pos_t;
				::SetItemPos(HAND, &pos_t);
				::SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
				Auction.ClearBondItem();
				::DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
			}
		}break;
	default:break;
	}
}

void MenuSubProcessType_FT_DO(SMENU *SubMenu, int j)//020515 lsw
{
	int &iFieldX = SubMenu->nField[j].x;
	int &iFieldY = SubMenu->nField[j].y;
	int &iMainX = SubMenu->x;
	int &iMainY = SubMenu->y;
	int &iImgNo =SubMenu->nField[j].nImageNumber;

	switch(SubMenu->nField[j].nWillDo)
	{
	case DO_CHATTING_VIEW :	
		{
			ChatMgr.DisplayData( iMainX+iFieldX , iMainY+iFieldY );//020702 lsw
		}break;
	case DO_CHATTING_EDITBOX :	
		{
			if( !IsChatBoxActive() ) //채팅 모드가 아니면 애들을 닫게 만든다.
			{
				SetChatMode(CM_COMMAND);//포커스는 게임 메인으로
				CallWisperList(false);				
				break;
			}
			else//채팅 가능 모드
			{
				SubMenu->nField[44].nType = FT_SELECT_CHAT_TYPE;
				SubMenu->nField[47].nType = FT_CHAT_LOCK;
				
				char temp[MAX_CHAT_STRING_]={""};
				char wisper_target[23]={""};
				EWndMgr.GetTxt( HWND_CHAT_TARGET_INPUT, wisper_target, 20 );
				EWndMgr.GetTxt( HWND_CHAT_INPUT, temp, 52 );//020530 lsw
				
				if( !EWndMgr.IsFocus(HWND_CHAT_INPUT) && 
					!EWndMgr.IsFocus(HWND_CHAT_TARGET_INPUT) )
				{//이상한데로 포커스가 갔으면 뻇어온다 기본은 채팅 입력창에다
					SetFocus2(HWND_CHAT_INPUT);	//021001 lsw					
				}
				//이름칸에 글이 없으면 포커스를 이름 입력으로 몰아 놓는다
				if(!wisper_target[0])
				{
					SetFocus2(HWND_CHAT_TARGET_INPUT);//021001 lsw
				}

				static int cursor = 0 ;
				//입력창 바닥 찍기
				FieldTypeNomalPutFx3( iMainX, iMainY, iFieldX, iFieldY, iImgNo , SubMenu->nField[j].nImageType,18,1);
				FieldTypeNomalPutFx3( iMainX+58, iMainY+4, iFieldX, iFieldY, SubMenu->nField[j].nRectImage , SubMenu->nField[j].nImageType,15,1);
				
				Hcolor( FONT_COLOR_WHITE );
				/*if(EWndMgr.IsFocus(HWND_CHAT_TARGET_INPUT))//커서가 여기서 깜빡
				{
					if((cursor%=12) < 6)
					{
						Hprint2( iMainX+iFieldX+58+2 , iMainY+iFieldY+4+2, g_DestBackBuf, "%s",wisper_target );
					}
					else
					{
						Hprint2( iMainX+iFieldX+58+2 , iMainY+iFieldY+4+2, g_DestBackBuf, "%s",wisper_target );
					}
					Hprint2( iMainX+iFieldX+58+2 +125, iMainY+iFieldY+4+2, g_DestBackBuf, "%s",temp );
				}
				if(EWndMgr.IsFocus(HWND_CHAT_INPUT))
				{
					if((cursor%=12) < 6)
					{
						Hprint2( iMainX+iFieldX+58+2 +125, iMainY+iFieldY+4+2, g_DestBackBuf, "%s",temp );
					}
					else
					{
						Hprint2( iMainX+iFieldX+58+2 +125, iMainY+iFieldY+4+2, g_DestBackBuf, "%s",temp );
					}
					Hprint2( iMainX+iFieldX+58+2 , iMainY+iFieldY+4+2, g_DestBackBuf, "%s",wisper_target );
				}*/
				/****
					Added by Tya -- cursor that moves according to the cursor.. :p
				****/
				Hprint2( iMainX+iFieldX+58+2 , iMainY+iFieldY+4+2, g_DestBackBuf, "%s",wisper_target );
				Hprint2( iMainX+iFieldX+58+2 +125, iMainY+iFieldY+4+2, g_DestBackBuf, "%s",temp );
				
				if((cursor%=12) < 6)
				{
					DWORD selStart, selEnd;
					int x;
					if(EWndMgr.IsFocus(HWND_CHAT_TARGET_INPUT))
					{
						SendMessage(EWndMgr.GetHWnd(HWND_CHAT_TARGET_INPUT),EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);
						x=GetTextWidth("%s",selStart,wisper_target);
						if(x<120)
							Hprint2( iMainX+iFieldX+58+2-3 + x, iMainY+iFieldY+4+2, g_DestBackBuf, "|");
					}
					if(EWndMgr.IsFocus(HWND_CHAT_INPUT))
					{
						SendMessage(EWndMgr.GetHWnd(HWND_CHAT_INPUT),EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);
						x = GetTextWidth("%s",selStart,temp);
						if(x < 310)
							Hprint2( iMainX+iFieldX+58+2-3+125 + x, iMainY+iFieldY+4+2, g_DestBackBuf, "|" );
					}
				}

				/*
					End of adds
				*/

				cursor++;

				if( g_aCurrentKeys[DIK_ESCAPE] & 0x80 ) 
				{
					SetChatMode(CM_COMMAND);
				}
			}
		}break;
	default:
		break;
	}
}

void lsw_MenuSubProcessType(SMENU *SubMenu,const int j)
{
	int &fRcMouse	= SubMenu->nField[j].fRectMouse;
	int &fLBDown	= SubMenu->nField[j].fLButtonDown;

	int &iImgType	= SubMenu->nField[j].nImageType;
	int &iRcImg		= SubMenu->nField[j].nRectImage;
	int &iImgNo		= SubMenu->nField[j].nImageNumber;

	int &iFieldX	= SubMenu->nField[j].x;
	int &iFieldY	= SubMenu->nField[j].y;
	
	int &iMainX		= SubMenu->x;
	int &iMainY		= SubMenu->y;
	int &iWork		= SubMenu->work;

	int &iSwd		= SubMenu->nField[j].nSpecialWillDo;

	int &nWillDo	=SubMenu->nField[j].nWillDo;
	const int iType = SubMenu->nField[j].nType;
	switch(iType)
	{
	case	FT_HUNTER_VERSUS_DISPLAY:
	{	///////////////////////////////////////////////
		my_hunter_list->print2( 700, 50 );		// 현상범 목록 보여주기
		///////////////////////// 현상범에 의한 위험 감지
		if( ::IsRightWindowOpen() ) break;
		if( danger_by_hunter )
		{
			int color = danger_by_hunter % 2;
			if( color )
				Hcolor( FONT_COLOR_PLUS );
			else Hcolor( FONT_COLOR_NAME );
			
			Hprint2( 700, 50, g_DestBackBuf, "DANGER" );
			danger_by_hunter -- ;
			if( danger_by_hunter < 0 ) danger_by_hunter = 0;
			
		}
		static int bCheck = -1;
		bCheck++;
		///////////////////////////////////////////
		//  1:1 결투 시작을 알리기
		if( man_to_man_start )
		{
			if( man_to_man_start < 80 )
			{
				static int img_max = 0;
				if( !img_max )
				{
					Spr *temp_spr;
					int j;
					for( j=0; j<START_FIGHT_IMG_MAX; j++ )
					{
						temp_spr = GetSprOfMenu( START_FIGHT_MAP, j );
						if( !temp_spr->img ) break;
					}
					img_max = j;
				}
				static int ct = 0;

				if( ct==0 ) MP3( SN_FIGHT_START_READY );
				if( ct==13 ) MP3( SN_FIGHT_START_FIGHT );

				Spr *s1 = GetSprOfMenu( START_FIGHT_MAP, ct );
				if( !s1 ) break;

				PutCompressedImageFX( SCREEN_WIDTH/2, 200, s1, 19, 2 );

				ct++;
				if( ct >= img_max ) 
				{
					man_to_man_start=0;
					ct=0;
					if( g_FightMapStart ) OpenFightMapSignMenuOpen( fight_map_live_man );		// 전광판 띄워주기..
				}
			}
			else
				man_to_man_start --;
		}

		if( fight_map_stop )
		{
			if( fight_map_stop < 80 )
			{
				static int ct = 0;
				if( ct==0 ) MP3( SN_FIGHT_STOP );
				Spr *s = GetSprOfMenu( STOP_FIGHT_MAP, ct );
				if( s && s->img )
				{
					PutCompressedImageFX( SCREEN_WIDTH/2, 200, s, 17, 2 );
					//PutCompressedImage( SCREEN_WIDTH/2, 200, s);//, 19, 2 );
					if( bCheck%2 )ct++;
				}
				else
				{
					fight_map_stop=0;
					ct=0;
					//OpenFightMapSignMenuOpen( fight_map_live_man );		// 전광판 띄워주기..
					MenuSoundClose( MN_FIGHT_MAP_SIGN );
				}
			}
			else fight_map_stop--;
		}
		else
		{
			static int animation=0;
			if( g_fight_win > 0 )
			{													
				static int ct = 0;
				if( ct==0 ) MP3( 2211 );
				if( ct == 44 ) MP3( 2204 );
				if( ct == 60  ) MP3( SN_FIGHT_WIN );

				Spr *s1 = GetSprOfMenu( FIGHT_MAP_WIN, ct );
				Spr *s2 = GetSprOfMenu( FIGHT_MAP_WIN, ct+1 );

				if( s1 && s2 && s1->img && s2->img )
				{
					PutCompressedImage( SCREEN_WIDTH/2, 200, s1 );
					PutCompressedImageFX( SCREEN_WIDTH/2, 200, s2, 17, 2 );

					static int flag = 1;
					if( ct > 10 )
					{
						if( animation < 4 )
						{
							if( ct==40 || ct==10 )
							{
								flag = !flag;
								animation++;
							}
							if( flag ) ct += 2;
							else ct -= 2;
						}
						else ct+=2;
					}
					else
					{
						flag = true;
						ct += 2;
						animation++;
					}
				}
				else
				{														
					g_fight_win=0;
					ct=0;
					animation = 0;
				}
			}
			if( g_fight_lose > 0 )
			{
				static int ct = 0;
				//if( ct==0 ) MP3( SN_FIGHT_LOSE );
				if( ct==22 || ct==32 ) MP3( 302 );
				if( ct == 70 ) MP3( 2203 );
				if( ct== 84 ) MP3( SN_FIGHT_LOSE );

				Spr *s1 = GetSprOfMenu( FIGHT_MAP_LOSE, ct );
				Spr *s2 = GetSprOfMenu( FIGHT_MAP_LOSE, ct+1 );

				if( !s1 || !s2 ) 
				{
					g_fight_lose=0;
					ct=0;
					break;
				}
				else
				{
					if( s1->img && s2->img )
					{
						PutCompressedImage( SCREEN_WIDTH/2, 300, s1 );
						PutCompressedImageFX( SCREEN_WIDTH/2, 300, s2, 17, 2 );
						if( bCheck%2 )ct+=2;
					}
					else
					{
						g_fight_lose=0;
						ct=0;
					}
				}
			}
		}
		if( g_RemainTimeDelay )
		{
			static int cn = 0;
			int div = (5-g_RemainTimeType-1)*10;		// 시간 이펙트가 한꺼번애 바인딩 되어 있기 때문에
			
			if( !(cn%10) ) MP3( SN_FIGHT_TIME );
			
			Spr *s1 = GetSprOfMenu( FIGHT_MAP_TIME, div+cn%10 );
			Spr *s2 = GetSprOfMenu( FIGHT_MAP_TIME, div+cn%10+1 );
			if( !s1 || !s2 ) 
			{
				break;
			}
			else
			{
				if( s1->img && s2->img )
				{
					PutCompressedImage( SCREEN_WIDTH/2+(SCREEN_WIDTH-640)/2, SCREEN_HEIGHT/2-(SCREEN_HEIGHT-480)/2, s1 );
					PutCompressedImageFX( SCREEN_WIDTH/2+(SCREEN_WIDTH-640)/2, SCREEN_HEIGHT/2-(SCREEN_HEIGHT-480)/2, s2, 17, 2 );
					if( bCheck%2 )cn+=2;
				}
			}
			g_RemainTimeDelay--;
			if( !g_RemainTimeDelay ) cn = 0;
		}
	}break;
case FT_RARE_GROUP_MEMBER_DISPLAY://020214 lsw
	{
		if(fRcMouse)//마우스 업이 되었을때
		{
			if(iImgNo)//플래그 온 이고 
			{
				Hcolor( FONT_COLOR_RARE_MAIN );
				RectTextPut(iMainX+iFieldX,iMainY+iFieldY,iRcImg,SubMenu->nField[j].temp); 
				const int iX	= iMainX+iFieldX+100;
				const int iY	= iMainY+iFieldY-20;
				const int iRareIndex = iImgNo;
				DrawRareExplain(iX,iY,iRareIndex,SubMenu->key);
			}
		}
		else
		{
			Hcolor( FONT_COLOR_WHITE );
			RectTextPut(iMainX+iFieldX,iMainY+iFieldY,iRcImg,SubMenu->nField[j].temp); 
		}
	}break;
case 	FT_CS_ICON://반투명은 3,4가 좋다
	{
		if(fRcMouse)//마우스 업이 되었을때
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iRcImg, iImgType);
		//	DrawCombatSkillExplain(Hero->x-Mapx+20,Hero->y-Mapy-80, nWillDo);
		//	DrawCombatSkillExplain(iMainX-8,325, nWillDo);
			DrawCombatSkillExplain(563,130, nWillDo);
		}
		else
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iImgNo, iImgType);
		}
		const int iCSIndex = nWillDo;
		if(iCSIndex)
		{
			const int iSkillLv = CombatSkill[iCSIndex].iSkillLv;
			if(iSkillLv >0 )//배운것이라야 찍는다
			{
				const int iLvImageNo = 79 + iSkillLv - 1;//79번 이미지 부터 시작
				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,5);
				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,2);
			}
		}
		
	}break;
case 	FT_CSD_ICON://반투명은 3,4가 좋다
	{
		const int iCSIndex = nWillDo;
		if(iCSIndex)
		{
			if(fRcMouse)//마우스 업이 되었을때
			{
				FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,false), iImgType);
		//		FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,false), iImgType,5,2);
				DrawCombatSkillExplain(Hero->x-Mapx+20,Hero->y-Mapy-80, nWillDo);
			}
			else
			{
			//	FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iImgNo, iImgType,5,5);
				FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,true), iImgType);
			}
			const int iLvImageNo = 79 + CombatSkill[iCSIndex].iSkillLv - 1 ;//79번 이미지 부터 시작
			FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,5);
			FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,2);
			DisplayQuickMemory(QM_COMBATSKILL,iCSIndex,iMainX+iFieldX+15,iMainY+iFieldY+15);
			if( fRcMouse )
			{
				SetQuickMemoryByKeyInput(QM_COMBATSKILL,iCSIndex);
			}
		}
	}break;
case 	FT_CSD_MAIN_ICON:
	{
	// FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX+3,iFieldY+3,72, iImgType,5,5);//디폴트가 들어있는 버튼
	// FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,86, iImgType,5,5);//디폴트가 들어있는 버튼
	 FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,86, iImgType);//디폴트가 들어있는 버튼			
		const int iCSIndex = nWillDo;
		if(iCSIndex)
		{
			const int iImageNo = SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,false);
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iImageNo, iImgType);
			{//레벨 찍는 곳
			const int iLvImageNo = 79 + CombatSkill[iCSIndex].iSkillLv - 1 ;//79번 이미지 부터 시작
			FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,5);
			FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,iLvImageNo, iImgType,5,2);
			}
			DisplayQuickMemory(QM_COMBATSKILL,iCSIndex,iMainX+iFieldX+18,iMainY+iFieldY+18);
			if(fRcMouse)//마우스 업이 되었을때
			{
				DrawCombatSkillExplain(Hero->x-Mapx+20,Hero->y-Mapy-80, nWillDo);
				SetQuickMemoryByKeyInput(QM_COMBATSKILL,iCSIndex);
			}
		}
	}break;

case 	FT_CSP_POINT_UP:
	{
		if(fRcMouse)
		{
			if(fLBDown)//버튼 클릭 이벤트
			{
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iRcImg, iImgType );
			}
			else
			{
				FieldTypeNomalPut   (iMainX, iMainY, iFieldX, iFieldY , iImgNo, iImgType );
				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX, iFieldY , iImgNo, iImgType, 3, 2 );
			}
		}
		else
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType );
		}			
		SetHangulAlign(TA_CENTER);
		Hprint2( iMainX+iFieldX-27 ,iMainY+iFieldY+19, g_DestBackBuf,"%d",iCSReservPoint);  //줄간격은 15이다
	}break;
case 	FT_CSD_POINT_COUNT_BAR:
	{
		if(SubMenu->nField[1].nWillDo)//저장된 스킬이 있으면 출력
		{
	//		FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG,5 ,6);//빽판 찍기
	//		FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG,2 ,4);//빽판 찍기
			//계산 부분 넣기
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG);//빽판 찍기
			const int iMax = nWillDo			= iCSPCMax;
			const int iNow = iSwd	= iCSPCNow;
			if(!iMax || !iNow ) {break;}

			const int iBarHeight = 88;
			const int iTarget = ((float)iNow) / ((float)iMax) * ((float)iBarHeight) ;
			
			for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
			{
				FieldTypeNomalPutFx3(iMainX, iMainY, 
					iFieldX+23,
					iFieldY+iBarHeight-iCount+14,
					iImgNo,
					iImgType,17,1);
			}
			SetHangulAlign(TA_CENTER);
			Hcolor( FONT_COLOR_WHITE );
			Hprint2( iMainX+iFieldX+33,iMainY+iFieldY+iBarHeight+4, g_DestBackBuf,"%d",iNow);
		}
	}break;
case 	FT_CSD_POINT_LINE_BAR:
	{
		if(SubMenu->nField[1].nWillDo)//저장된 스킬이 있으면 출력
		{
		//	FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG,5,6);//빽판 찍기
		//	FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,85, COMBAT_SKILL_ICON_IMG);//빽판 찍기
			//계산 부분 넣기
			const int iMax = nWillDo			= iCSPMax;
			const int iNow = iSwd	= iCSPNow;
			if(!iMax || !iNow ) {break;}

			const int iBarHeight = 88;
			const int iTarget = ((float)iNow) / ((float)iMax) * ((float)iBarHeight) ;
			
			for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
			{
				FieldTypeNomalPutFx3(iMainX, iMainY, 
					iFieldX+23,
					iFieldY+iBarHeight-iCount+14,//-3,
					iImgNo,
					iImgType,17,1);
			}
			SetHangulAlign(TA_CENTER);
			Hcolor( FONT_COLOR_WHITE );
			Hprint2( iMainX+iFieldX+33,iMainY+iFieldY+iBarHeight+4, g_DestBackBuf,"%d",iNow);
		}
	}break;
case	FT_CS_GROUP_EXPLAIN:
	{	//	iWork;//그룹 넘버		//	nWillDo;//그룹 타입
		const int iImageNo = 73 + (iWork-1)*2 +(nWillDo-1);//찍을 그림은 73 번 부터 시작
//			const int x = iMainX+iFieldX;
//			const int y = iMainY+iFieldY;
		FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iImageNo, COMBAT_SKILL_ICON_IMG);//빽판 찍기
		
//			Hcolor( FONT_COLOR_YELLOW );	
//			SetHangulAlign(TA_CENTER);
//			Hprint2(x+21,y+50,g_DestBackBuf,"<%s>",CombatSkillGroupExplain[(iWork-1)*2 +(nWillDo)].LocalName);
		
		if(fRcMouse)//마우스 업 일때
		{
			DrawCombatSkillGroupExplain(563,130, iWork,nWillDo);
		}
	}break;
case	FT_CS_EXPLAIN:
	{
		int iCSkillIndex = CombatSkill[nWillDo].iSkillNo;
		if(fRcMouse)//마우스 업 일때
		{
			Hcolor( FONT_COLOR_PLUS );	
			Hprint2(iMainX +iFieldX,iMainY+ iFieldY,g_DestBackBuf,CombatSkillExplain[iCSkillIndex].EngName);
			DrawCombatSkillExplain(563,130, nWillDo);
		}
		else
		{
			Hcolor( FONT_COLOR_WHITE );	
			Hprint2(iMainX +iFieldX,iMainY+ iFieldY,g_DestBackBuf,CombatSkillExplain[iCSkillIndex].EngName);
		}
	}break;
case FT_PEACE_BATTLE_PUT :
	{//49 50 51 피스 58 59 60 배틀 61 62 63 피케이
		// battle mode
		const int param = main_interface.data[IF_BATTLE_PEACE];
		int nomal[] = { 49, 58, 61 };
		int light[] = { 50, 59, 62 };
		int down[] =  { 51, 60, 63 };

		int *image_num = nomal;//기본 보통 그림
		
		if( fRcMouse ) //올라오면
		{
			if( fLBDown )
			{
				image_num = down;
			}
			else
			{
				image_num = light;
			}
		}
		
		FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, 
			image_num[param], iImgType );
	}break;
	case FT_DISPLAY_TAC_EXP:
		{
			
			int first_exp=0, exp, lv, next;
			int refresh = 0;
			static int old_tactics = SCharacterData.tatics;
			if( old_tactics != SCharacterData.tatics ) 
			{
				refresh = 1;
				old_tactics = SCharacterData.tatics;
			}
			exp = SCharacterData.TacticsExp[SCharacterData.tatics-88];

			int olg_lv = SCharacterData.SkillId[SCharacterData.tatics];
			if( olg_lv )
				first_exp = NPC_Lev_Ref[olg_lv].nMinExp;
			else olg_lv = 0;

			next = SCharacterData.TacticsExpNext[SCharacterData.tatics-88]; 
			if( !next ) next = 1;
			lv = olg_lv;		// tactic은 +1로 표현해 준다.
			DisplayTacGauge(SubMenu,j,iMainX+iFieldX,iMainY+iFieldY,next,exp,lv,refresh,first_exp);
			
			SetHangulAlign( TA_LEFT );
			Hprint2( iMainX+iFieldX+25, iMainY+iFieldY+2, g_DestBackBuf, "%s", explain_tactics[SCharacterData.tatics-88] );
			// Magic EXP Percentage JHDR
			const int iNow = exp - first_exp;
			const int iMax = next - first_exp;
			const float fExpRate = (float(iNow)) / (float(iMax));
			int Per = (int)(fExpRate * 100);
			HprintBold(iMainX + iFieldX + 66, iMainY + iFieldY + 1,
				Convert16to32(FONT_COLOR_WHITE), Convert16to32(FONT_COLOR_BLACK),
				"%d%%", Per);
		}break;
	case FT_DISPLAY_MAGIC_EXP:
		{
			int refresh = 0;
			
			int first_exp, exp, lv, next;
			int tac = (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) ? ORISON : MAGERY ;
			exp = SCharacterData.TacticsExp[tac];

			int olg_lv = SCharacterData.SkillId[tac+88];
			if( olg_lv )
				first_exp = NPC_Lev_Ref[olg_lv].nMinExp;
			else first_exp = 0;

			next = SCharacterData.TacticsExpNext[tac]; 
			if( !next ) next = 1;
			lv = olg_lv;
			if( refresh_magery_time <= 0 )
			{
				CallServer( CMD_REQ_MAGERY );  //서버와 프로토콜을 맞춘 후...
				refresh_magery_time = 100;
			}
			else refresh_magery_time--;
			DisplayTacGauge(SubMenu,j,iMainX+iFieldX,iMainY+iFieldY,next,exp,lv,refresh,first_exp);
			SetHangulAlign( TA_LEFT );
			Hprint2( iMainX+iFieldX+25, iMainY+iFieldY+2, g_DestBackBuf, "%s", explain_tactics[tac] );

			// Magic EXP Percentage JHDR
			const int iNow = exp - first_exp;
			const int iMax = next - first_exp;
			const float fExpRate = (float(iNow)) / (float(iMax));
			int Per = (int)(fExpRate * 100);
			HprintBold(iMainX + iFieldX + 66, iMainY + iFieldY + 1,
				Convert16to32(FONT_COLOR_WHITE), Convert16to32(FONT_COLOR_BLACK),
				"%d%%", Per);

		}break;
	case FT_DISPLAY_SKILL_EXP:
		{
			int refresh = 0;
			
			int first_exp, exp, lv, next;
			int job;
			if( !before_skill_no )
			{
				GetSkillMother( g_GmMain[Hero->job].skill, &job, 1 );
			}
			else job = before_skill_no;
			
			static int old = job;
			if( old != job ) SubMenu->CheakType = 0;
			//011012 lsw >
			exp = SCharacterData.nJobSkill[skill[job].inclusive].skillexp % 10000;
			lv =  SCharacterData.nJobSkill[skill[job].inclusive].skillexp / 10000;
			//011012 lsw <

			next = 10000;
			first_exp = 0;
			DisplayTacGauge(SubMenu,j,iMainX+iFieldX,iMainY+iFieldY,next,exp,lv,refresh,first_exp);
			SetHangulAlign( TA_LEFT );
			Hprint2( iMainX+iFieldX+25, iMainY+iFieldY+2, g_DestBackBuf, "%s", skill[job].han_name );
			// Magic EXP Percentage JHDR
			const int iNow = exp - first_exp;
			const int iMax = next - first_exp;
			const float fExpRate = (float(iNow)) / (float(iMax));
			int Per = (int)(fExpRate * 100);
			HprintBold(iMainX + iFieldX + 66, iMainY + iFieldY + 1,
				Convert16to32(FONT_COLOR_WHITE), Convert16to32(FONT_COLOR_BLACK),
				"%d%%", Per);
		}break;
	case FT_EXPLAIN_MENU :
		{
#ifdef _DEBUG // finito 060507
			if(GetSysInfo( SI_GAME_MAKE_MODE ) && g_aCurrentKeys[DIK_F12] & 0x80 )	// 설명메뉴 refresh
			{
				SubMenu->CheakType = 0;
			}
#endif
			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;
				if( SubMenu->nField[j+1].nType != FT_EXPLAIN_MENU )		// 다음 필드 타입이 같은경우가 아니면 데이타를 읽어 온다.
					SetMenuExplain( nWillDo, SubMenu->Id, j );
			}

			if( fRcMouse )
			{	//if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
				ExplainMenuSet( SubMenu->nField[j].temp );
			}
		}break;
	case FT_NATION_DISPLAY :
		{
			SetHangulAlign(TA_CENTER);
			Hcolor( NationColor16[MapInfo[MapNumber].nation] );
			Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%s", NationName[MapInfo[MapNumber].nation]);
		}break;
	//011004 lsw
	case FT_XY_DISPLAY :
		{	//< CSD-030509
			const int mapx = g_Map.x + g_pointMouseX / TILE_SIZE;
			const int mapy = g_Map.y + g_pointMouseY / TILE_SIZE;
			SetHangulAlign(TA_CENTER);
			Hcolor( RGB16(200,200,0) );
			Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%s(%3d,%3d)", MapNumberName, mapx, mapy);
			break;
		}	//> CSD-030509
	case FT_WILLDO_WORK :
		{
 			if (iWork==1)
			{
				FieldTypeNomalPut(iMainX,iMainY,38,3,4 , TIME_MENU );
			}
			else 
			{
				FieldTypeNomalPut(iMainX,iMainY,38,3,3 , TIME_MENU );
			}
		 }break;
	case FT_MAININTERFACE_MAIL_CALL:
		{
			if(g_MailMgr.GetExistNotRead())//안읽은 놈이 있음
			{
				static bool bBlend	= true;
				static int iAlpha =10;

				if(	bBlend ) 
				{	iAlpha += 2;	
					if( iAlpha > 10 ) 
					{
						bBlend = !bBlend;
					}
				}
				else
				{
					iAlpha -=1;	
					if( iAlpha < 5 ) 
					{
						bBlend = !bBlend;
					}
				}
				FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo, iImgType,iAlpha,2);
			}
			else
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType);
			}

		}break;
//<! BBD 040303
	case FT_MERCHANT_BACKUP_GET_LIST : 
		{

			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;
				// 패킷 전송
				Auction.SendCMD_MERCHANT_BACKUP_LIST_REQUEST();//판매자의 백업된 아이템을 받아옴
			}
			break;
		}
//> BBD 040303
	case FT_TODAY_NOTICE:
		{	
			if (g_LocalWarBegin || g_bWarBegin || g_iScenarioOn || nWillDo > 0)//이벤트 있음
			{
				static bool bBlend	= true;
				static int iAlpha =10;

				if(	bBlend ) 
				{	iAlpha += 2;	
					if( iAlpha > 10 ) 
					{
						bBlend = !bBlend;
					}
				}
				else
				{
					iAlpha -=1;	
					if( iAlpha < 5 ) 
					{
						bBlend = !bBlend;
					}
				}
				FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo, iImgType,iAlpha,2);
			}
			else
			{
				FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType,15,1);
			}
		}break;
	case FT_ALPHA_PUT:
		{
			FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo, iImgType,15,1);
		}break;
	case FT_DRAG_ZONE:
		{
			int &iOn = nWillDo;
			if(iOn)
			{
				const int iGab = 10;
				const int iTargetX = g_pointMouseX;
				const int iTargetY = g_pointMouseY;
				if(0+iGab < iTargetX && GAME_SCREEN_XSIZE-iGab > iTargetX )
				{
					iMainX = iTargetX-iGab;
				}
				if(0+iGab < iTargetY &&	GAME_SCREEN_YSIZE-iGab > iTargetY )
				{
					iMainY = iTargetY-iGab;
				}
			}
		}break; //브레이크 없습니다 그리고 아래 애들 위치 바꾸지 마세요
	case FT_CHAT_VIEW_SET:
		{
			if(ChatMgr.GetViewChat(iSwd))
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType);
			}
			else
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iRcImg , iImgType);
			}
		}break;
	case FT_CHAT_LOCK:
		{
			if(nWillDo)
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType);
			}
			else
			{
				FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iRcImg , iImgType);
			}
		}break;				
	case FT_SELECT_CHAT_TYPE:
		{
			const int SendType = GetChatTarget();
			switch(SendType)
			{
			case 0:
				{	
					EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SMenu[MN_WISPER_LIST].nField[SendType].temp );//021001 lsw
					SetFocus2(HWND_CHAT_INPUT);//021001 lsw
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType);
				}break;
			case 1:
				{	
					EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SMenu[MN_WISPER_LIST].nField[SendType].temp );//021001 lsw
					SetFocus2(HWND_CHAT_INPUT);//021001 lsw
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, iRcImg , iImgType);
				}break;
			case 2://귓속말 상대 선택
				{
					EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SMenu[MN_WISPER_LIST].nField[SendType].temp );//021001 lsw
					SetFocus2(HWND_CHAT_INPUT);//021001 lsw
					// Finito added image number manually
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, 100 , iImgType);				}break;
			case 3:// Finito
				{
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, nWillDo, iImgType);
				}break;
			default:
				{
					EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SMenu[MN_WISPER_LIST].nField[SendType].temp );//021001 lsw
					SetFocus2(HWND_CHAT_INPUT);//021001 lsw
					FieldTypeNomalPut(iMainX,iMainY,iFieldX,iFieldY, nWillDo, iImgType);
				}break;
			}
		}break;
	case FT_SELECT_WISPER:
		{
			if(!IsAbleChatTargetName(nWillDo))
			{
				break;
			}
			FieldTypeNomalPutFx3(iMainX,iMainY,iFieldX,iFieldY, iImgNo , iImgType,15,1);//기본 바탕을 찍고
			Hprint2(iMainX+iFieldX+2, iMainY+iFieldY+4, g_DestBackBuf,"%s",SubMenu->nField[j].temp);//020505 lsw
			const int TargetCh = nWillDo;
			if(TargetCh == iWork || fRcMouse)
			{
				FieldTypeNomalPut(iMainX+2,iMainY+2,iFieldX,iFieldY, iRcImg , iImgType);
			}
		}break;
	case FT_SELECT_WISPER_TARGET:
		{
		//	FieldTypeNomalPut(iMainX+2,iMainY+2,iFieldX,iFieldY, iRcImg , iImgType);
		}break;
	case FT_HOTKEY_ICON ://020701 lsw
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX,iFieldY,iImgNo, iImgType);//디폴트가 들어있는 버튼
			const int iNo = HotKeyMemory.iNo;
			if (!iNo|| strcmp(Hero->name,HotKeyMemory.OwnerName))//주인 이름이 아니라면
			{
				break;
			}
			const int iX = iMainX+iFieldX+21;
			const int iY = iMainY+iFieldY+21;//SetMagicOn 에 숫자 맞춰 줘라 +21

			switch(HotKeyMemory.iType)
			{
			case QM_MAGIC:
				{
					if( iNo )//마법 넘버가 있고
					{
						if(!RectImageOn) 
						{
							PutMagicIcon(iX,iY, iNo);	// 출력
						}
					}
					else 
					{
						break;
					}
					if( RectImageOn && !MagicOnFlag.type )	
					{
						PutMagicIcon(iX , iY, iNo);
						static int ct=1;
						static bool ct_flag = 0;

						PutMagicIcon(iX , iY, iNo, ct+2);

						if(ct_flag) {ct++;}
						else {ct--;}

						ct %= 9 ;
						if( ct == 0 ){ ct_flag = 1;}
						if( ct == 8 ){ ct_flag = 0;}
					}
					if( fRcMouse || fLBDown || SubMenu->nField[j].fRButtonDown )	
					{
						CheckMagicExplain( HotKeyMemory.iNo );
						PutMagicIcon(iX, iY, iNo, 8);		// 밝은 그림
					}
				}break;
			case QM_SKILL:
				{
					if( iNo )//번호가 있고
					{
						PutSkillIcon(iX, iY, iNo);	// 출력
					}
					else 
					{
						break;
					}

					if( fRcMouse && !g_MouseItemType )//마우스가 올라 가면
					{
						CheckSkillExplain( iNo );
					}
					if( fLBDown || SkillOn == 1)//스킬이 시전중 이라면
					{
						PutSkillIcon(iX, iY, iNo, 8);		// 밝은 그림
					}
				}break;
			case QM_COMBATSKILL:
				{
					
				}break;
			}
			SetHangulAlign(TA_CENTER);
		//	Hcolor(FONT_COLOR_YELLOW);
			HprintBold( iX-10, iY-4, FONT_COLOR_SOLID_YELLOW, FONT_COLOR_BLACK, "'/'" );
		}break;
	case FT_QUICK_MAGIC_PLUS : 
		{
#ifndef _SDL
		int magic_x =0;
#else // !_SDL

			int magic_x = 0;
#endif// 매직 아이콘을 찍을 시작위치
			RECT &rect = SubMenu->nField[j].rCheakBox;
			switch( magic_plus_count )		// 그동안 사용한 마법의 백 판 찍기
			{
				case 0:
				case 1:	
					{
							FieldTypeNomalPut( iMainX, iMainY,  376, -52, 15, MAIN_ETC );
							FieldTypeNomalPut( iMainX, iMainY,  379, -52, 16, MAIN_ETC );
							magic_x = 379;					// 아이콘 처음 찍을 위치
							::SetRect( rect, magic_x, -52, magic_x+44, 0 );
							SMenu[MN_QUICK_MAGIC_SCROLL].bActive = false;
							SMenu[MN_TIME_DISPLAY_ON_OFF].x = iMainX+423;
							SMenu[MN_TIME_DISPLAY_ON_OFF].y = iMainY-52;
					}break;
				case 2:
				case 3:
				case 4:
				case 5:	
					{	
						int gabx = 22*(magic_plus_count-1);
						FieldTypeNomalPut( iMainX, iMainY,  376-gabx, -52, 15, MAIN_ETC );
						int i;
						for( i=0; i<magic_plus_count; i++ )
							FieldTypeNomalPut( iMainX, iMainY, 379-gabx+44*i , -52, 16, MAIN_ETC );
						magic_x = 379-gabx;
						::SetRect( rect, magic_x, -52, magic_x+i*44, 0 );
						SMenu[MN_QUICK_MAGIC_SCROLL].bActive = false;
						SMenu[MN_TIME_DISPLAY_ON_OFF].x = iMainX+423+gabx;
						SMenu[MN_TIME_DISPLAY_ON_OFF].y = iMainY-52;
					}break;
				default :
					{
						FieldTypeNomalPut( iMainX, iMainY,  376-22*4, -52, 15, MAIN_ETC );
						for( int i=0; i<5; i++ )
						{
							FieldTypeNomalPut( iMainX, iMainY, 379-22*4+44*i , -52, 16, MAIN_ETC );
						}
						
						magic_x = 379-22*4;		
						::SetRect( rect, magic_x, -52, magic_x+5*44, 0 );
						SMenu[MN_QUICK_MAGIC_SCROLL].bActive = true;

						//FieldTypeNomalPut( iMainX, iMainY,  423+22*4, -52, 22, MAIN_ETC );
						SMenu[MN_TIME_DISPLAY_ON_OFF].x = iMainX+423+22*4;
						SMenu[MN_TIME_DISPLAY_ON_OFF].y = iMainY-52;
						
					 }break;
			}

			int &scroll_max = SubMenu->nField[j].nSHideNomalCount;		// 총 스크롤 할 갯수
			scroll_max = magic_plus_count;

			const int scroll_start = magic_plus_start;
			for( int i=scroll_start, count = 0; i<scroll_max, count<5; i++, count++ )
			{
				if( magic_plus[i] )
				{
				//	PutMagicIcon( iMainX+magic_x+44*count+22, iMainY-52+27, g_mgrBattle.Convert(magic_plus[i]) );
					PutMagicIcon( iMainX+magic_x+44*count+22, iMainY-52+27, magic_plus[i] );
					DisplayQuickMemory(QM_MAGIC,magic_plus[i],iMainX+magic_x+44*count+16,iMainY-52+22);
				}
				else 
				{
					break;
				}
			}

			if( fRcMouse || fLBDown )
			{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
				const int x_count = (g_pointMouseX - SubMenu->nField[j].rCheakBox.left) / 44;
				const int x_check = (g_pointMouseX - SubMenu->nField[j].rCheakBox.left) % 44;
				if( x_check < 2 || x_check > 41 ) break;

				if( magic_plus[scroll_start+x_count] )
				{
					PutMagicIcon( iMainX+magic_x+44*x_count+22, iMainY-52+27, magic_plus[scroll_start+x_count], 8, 2 );
					CheckMagicExplain( magic_plus[scroll_start+x_count] );												
				}
				SetQuickMemoryByKeyInput(QM_MAGIC,magic_plus[scroll_start+x_count]);
			}
	   }break;
	case	FT_RARE_GROUP:
		{	//
			const int iRareGroup = iWork + nWillDo;
			const int iIsDynamicRare = SMenu[MN_ITEM_MAKE_RARE_SELECT].key;
			if( 0 > iRareGroup || MAX_RARE_GROUP <= iRareGroup)
			{
				break;
			}
			if(RareGroup[iIsDynamicRare][iRareGroup].name[0])
			{
				Hprint2( iMainX+iFieldX+30, iMainY+iFieldY, g_DestBackBuf , "%s", RareGroup[iIsDynamicRare][iRareGroup].name);
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);

				if( SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[0].nWillDo == iRareGroup)
				{
					FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iRcImg, iImgType);
				}
			}
		}break;
	case FT_MERCHANT_SELL_ITEM_DROP_ZONE:
		{
			ItemAttr *item = Auction.GetSellItemAttr();//아이템 Pos 받기
			if(!item){break;}//아이템 없으면 무시
			PutItemIcon(iMainX	+	iFieldX,//+	iFieldX, 
						iMainY	+	iFieldY,//+	iFieldY,
						item->item_no,30,1 );
		}break;
	case FT_MERCHANT_SELL_ITEM_EXPLAIN:
		{
			ItemAttr *item = Auction.GetSellItemAttr();//아이템 Pos 받기
			if(!item){return;}
			ExplainAuctionItem(iMainX+iFieldX,iMainY+iFieldY,	item,true);
			//아이템이름, 아이템 내구도, 레어, 레벨, 요구능력, 택틱만
		}break;
	case FT_MERCHANT_SELL_ITEM_VALUE_INPUT:
		{//숫자만 입력 되게끔
			const int iX = iMainX+iFieldX;
			const int iY = iMainY+iFieldY;
			if(!EWndMgr.IsFocus(HWND_1))
			{
				::SetFocus2(HWND_1);
			}
			char szTemp[11] = {0,};
			EWndMgr.GetTxt( HWND_1, szTemp, 10);
			const int iSellValue = ::atol( szTemp );
			sprintf( szTemp, "%u", iSellValue);
			EWndMgr.SetTxt( HWND_1,szTemp);//021001 lsw

			if(iSellValue)
			{
				TxtOut.RcTXTOutNoBG(iX - 150, iY+7, 100,0,lan->OutputMessage(4,540));//021126 lsw
			}
			else
			{
				TxtOut.RcTXTOutNoBG(iX - 150, iY+7, 100,0,lan->OutputMessage(4,541));//021126 lsw
			}


			TxtOut.RcTXTOutNoBG(iX +30, iY+7, 100,0,"%u",iSellValue);
			Auction.SetSellValue(iSellValue);
		}break;
	case FT_MERCHANT_RESULT_ITEM_CHECK:
	case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
	case FT_MERCHANT_BUY_ITEM_CHECK:
	case FT_MERCHANT_SELL_ITEM_CHECK:
		{
			int iIndex = 0;
			SEARCHRESULT *SR = NULL;
			switch(iType)
			{
			case FT_MERCHANT_RESULT_ITEM_CHECK:
				{
					iIndex = Auction.GetTakeItemIndex();
					SR = &Auction.GetSearchList().ResultList[iSwd + Auction.GetResultViewPage()*MAX_VIEW_ABLE_PER_PAGE ];
				}break;
			case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
			case FT_MERCHANT_BUY_ITEM_CHECK:
				{
					iIndex = Auction.GetBuyItemIndex();
					SR = &Auction.GetSearchList().ResultList[iSwd + Auction.GetBuyerViewPage()*MAX_VIEW_ABLE_PER_PAGE ];
				}break;
			case FT_MERCHANT_SELL_ITEM_CHECK:
				{
					iIndex = Auction.GetDeleteItemIndex();
					SR = &Auction.GetSearchList().ResultList[iSwd + Auction.GetSellerViewPage()*MAX_VIEW_ABLE_PER_PAGE ];
				}break;
			}		
			
			const int iX = iMainX+iFieldX;
			const int iY = iMainY+iFieldY;
			
			nWillDo = SR->iIndex;
			
			ItemAttr *pItem = &SR->m_ResultItem;	if(!pItem){break;}
			const int iItemNo = pItem->item_no;
			CItem *t = ItemUnit(iItemNo);			if(!t){break;}

			if(iItemNo)
			{
				char szTemp[MAX_PATH] ={0,};
				if(DIVIDE_ITEM == t->GetRButton())
				{
					sprintf( szTemp, "%d", pItem->attr[IATTR_MUCH] );
				}
				else
				{
					WORD d_max=0, d_curr=0;
					GetItemDuration( *pItem, d_curr, d_max );
					sprintf( szTemp, "%d/%d", d_curr/10, d_max/10 );
				}

				switch(iType)
				{
				case FT_MERCHANT_RESULT_ITEM_CHECK:
					{
						ExplainAuctionItem(iX+78,iY+5,pItem,false);//아이템 설명
						Hcolor(FONT_COLOR_WHITE);
						if(!strcmp(Hero->name,SR->szBuyerName))
						{
							PutItemIcon(iX+50 ,iY+20,iItemNo);	
							TxtOut.RcTXTOutNoBG(iX+257,iY+8,50,0,szTemp);//내구도
						}
						else
						{
							if(!strcmp(Hero->name,SR->szSellerName))
							{
								TxtOut.RcTXTOutNoBG(iX +38, iY+7, 30,0,lan->OutputMessage(4,551));//030103 lsw
								TxtOut.RcTXTOutNoBG(iX +257, iY+7, 50,0,"%d",SR->iSellValue);
							}
							else
							{
								TxtOut.RcTXTOutNoBG(iX +38, iY+7, 30,0,"Error");
							}
						}
						if(	iX+35+35 > g_pointMouseX&& iX+35 < g_pointMouseX
						&&	iY+35 > g_pointMouseY	&& iY < g_pointMouseY)
						{
							ExplainItemAttr = *pItem;
						}
					}break;
				case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
					{
						ExplainAuctionItem(iX+70,iY+5,pItem,false);//아이템 설명

						Hcolor(FONT_COLOR_WHITE);//021121 lsw
						TxtOut.RcTXTOutNoBG(iX+393-130,iY+8,50,0,szTemp);//내구도
						Hcolor(FONT_COLOR_WHITE);	
						const int iSellValue = SR->iSellValue;
						if(iSellValue)//판매금액이 있으면 
						{
							TxtOut.RcTXTOutNoBG(iX+460-130,iY+15,60,0,lan->OutputMessage(4,540));//내구도 이름
							TxtOut.RcTXTOutNoBG(iX+532-130,iY+7,60,0,"%d",iSellValue);//내구도 이름
						}
						else//직접거래라면
						{
							TxtOut.RcTXTOutNoBG(iX+460-130,iY+15,60,0,lan->OutputMessage(4,541));//내구도 이름
						}
						PutItemIcon(iX+42 ,iY+20,iItemNo);	//아이템 아이콘
						if(	iX+25+35 > g_pointMouseX&& iX+25 < g_pointMouseX
						&&	iY+35 > g_pointMouseY	&& iY < g_pointMouseY)
						{
							ExplainItemAttr = *pItem;
						}
					}break;
				case FT_MERCHANT_BUY_ITEM_CHECK:
					{
						ExplainAuctionItem(iX+70,iY+5,pItem,false);//아이템 설명
					
						Hcolor(FONT_COLOR_WHITE);//021121 lsw
						TxtOut.RcTXTOutNoBG(iX+260,iY+15,200,0,SR->szSellerName);//판매자 이름
						TxtOut.RcTXTOutNoBG(iX+393,iY+8,50,0,szTemp);//내구도	
						Hcolor(FONT_COLOR_WHITE);
						const int iSellValue = SR->iSellValue;
						if(iSellValue)//판매금액이 있으면 
						{
							TxtOut.RcTXTOutNoBG(iX+460,iY+15,60,0,lan->OutputMessage(4,540));//내구도 이름
							TxtOut.RcTXTOutNoBG(iX+532,iY+7,60,0,"%d",iSellValue);//내구도 이름
						}
						else//직접거래라면
						{
							TxtOut.RcTXTOutNoBG(iX+460,iY+15,60,0,lan->OutputMessage(4,541));//내구도 이름
							TxtOut.RcTXTOutNoBG(iX+532,iY+7,60,0,"%d",iSellValue);//내구도 이름
						}
						PutItemIcon(iX+42 ,iY+20,iItemNo);	//아이템 아이콘
						if(	iX+25+35 > g_pointMouseX&& iX+25 < g_pointMouseX
						&&	iY+35 > g_pointMouseY	&& iY < g_pointMouseY)
						{
							ExplainItemAttr = *pItem;
						}
					}break;
				case FT_MERCHANT_SELL_ITEM_CHECK:
					{
						if(iIndex )//체크버튼
						{	if(iIndex == nWillDo )
							{
								FieldTypeNomalPut(iMainX+3, iMainY+3, iFieldX, iFieldY, iRcImg, iImgType);
							}
						}
						
						ExplainAuctionItem(iX+70,iY+5,pItem,false);//아이템 설명
						Hcolor(FONT_COLOR_WHITE);
						TxtOut.RcTXTOutNoBG(iX+260,iY+8,50,0,szTemp);//내구도	
						const int iSellValue = SR->iSellValue;
						if(iSellValue)//판매금액이 있으면 
						{
							TxtOut.RcTXTOutNoBG(iX+322,iY+15,60,0,lan->OutputMessage(4,540));//내구도 이름
							TxtOut.RcTXTOutNoBG(iX+402,iY+7,60,0,"%d",iSellValue);//내구도 이름
						}
						else//직접거래라면
						{
							TxtOut.RcTXTOutNoBG(iX+322,iY+15,60,0,lan->OutputMessage(4,541));//내구도 이름
						}
						PutItemIcon(iX+42 ,iY+20,iItemNo);	//아이템 아이콘
						if(	iX+25+35 > g_pointMouseX&& iX+25 < g_pointMouseX
						&&	iY+35 > g_pointMouseY	&& iY < g_pointMouseY)
						{
							ExplainItemAttr = *pItem;
						}
					}break;
				}

				if(iIndex && iIndex == nWillDo )//위에 공통으로 체크 버튼 찍히는거
				{//체크표시
					switch(iType)
					{
					case FT_MERCHANT_RESULT_ITEM_CHECK:
						{
							FieldTypeNomalPut(iX+4, iY, 3, 4, iRcImg, iImgType);
						}break;
					case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
					case FT_MERCHANT_BUY_ITEM_CHECK:
					case FT_MERCHANT_SELL_ITEM_CHECK:
						{
							FieldTypeNomalPut(iX, iY, 3, 4, iRcImg, iImgType);
						}break;
					}
				}
				//마우스 업 되면 아이템 정보 디스플레이
				
			}//if(iItemNo)
			else
			{
				Hcolor(FONT_COLOR_WHITE);
				switch(iType)
				{
				case FT_MERCHANT_RESULT_ITEM_CHECK:
					{
						TxtOut.RcTXTOutNoBG(iX +38, iY+7, 30,0,lan->OutputMessage(4,550));
					}break;
				case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
				case FT_MERCHANT_BUY_ITEM_CHECK:
				case FT_MERCHANT_SELL_ITEM_CHECK:
					{
						TxtOut.RcTXTOutNoBG(iX +30, iY+7, 30,0,lan->OutputMessage(4,550));
					}break;
				}				
			}
		}break;
	case FT_EXCHANGE_ITEM : 
		{
		//	COMMENT ::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
			switch(ExchangeMgr.GetExchangeType())
			{
			case EXCHANGE_TYPE_MERCHANT_DIRECT:
				{//021121
					if(LEFT == nWillDo)
					{
						if(ExchangeMgr.m_bIsIAmMerchant)//내가 상인이면 왼쪽에다 페이지를 찍음(LEFT --> 말이 왼쪽이고 Right 다 ㅡㅡ;..
						{
							::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo,iImgType );//직접거래의 경우 바닥에 깐다
						}
					}
					else//오른쪽인데 내가 상인이면 뿌린다
					{
						if(!ExchangeMgr.m_bIsIAmMerchant)
						{
							::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo,iImgType );//직접거래의 경우 바닥에 깐다
						}
					}
					//한쪽은 경매 물품이니까 알아서처리 합시다 ㅡㅡ;. 이후에요
					::DisplayExchangeItemList(nWillDo,  fRcMouse);					
				}break;
			default:
				{
					::DisplayExchangeItemList(nWillDo,  fRcMouse);	break;
				}break;
			}//switch(ExchangeMgr.GetExchangeType())
		}break;
	case FT_EXCHANGE_ITEM_CHECK : 
		{
			const bool bIsOkMySide = ExchangeMgr.IsMySideConformStateOk();
			const bool bIsOkOtherSide = ExchangeMgr.IsOtherSideConformStateOk();//021030 lsw

			switch(nWillDo)
			{
			case LEFT:
				{
					if(bIsOkOtherSide)
					{
						::FieldTypeNomalPut(iMainX, iMainY, iFieldX+5, iFieldY+5, iImgNo ,iImgType);
					}
				}break;
			case RIGHT:
				{
					if(bIsOkMySide)
					{
						::FieldTypeNomalPut(iMainX, iMainY, iFieldX+5, iFieldY+5, iImgNo ,iImgType);
					}
				}break;
			}
		}break;
	case FT_EXCHANGE_ITEM_ETC:	
		{	// 인벤 창 이외의 메뉴는 다 없애버린다.
			if( ::IsLeftWindowOpen() ) 	{::LeftClose();}
			Hcolor( FONT_COLOR_NAME );
			SetHangulAlign( TA_CENTER );			
			::Hprint2( iMainX+306+112/2,iMainY+65, g_DestBackBuf, "%s", SCharacterData.sCharacterName );//자신 이름
			::Hprint2( iMainX+23+112/2,iMainY+65, g_DestBackBuf, "%s", ExchangeMgr.GetExchangeTargetName() );//교환상대의 이름
		}break;
	case FT_MAN_TO_MAN_INFO_DISPLAY:
		{
			if( ::IsLeftWindowOpen() ) 	{::LeftClose();}
			char *target_name = SubMenu->nField[j].temp;//타겟은 무조건 왼쪽 이기 때문
			LPCHARACTER ch = ::ExistHe( target_name );

			int iLineCt =0;
			const int iLineGab =20;
			const int iGabX =10;
			const int iGabY =5;
			const int iX = iMainX+iFieldX+iGabX;
			const int iY = iMainY+iFieldY+iGabY;

			Hcolor( FONT_COLOR_WHITE );
			SetHangulAlign( TA_CENTER );

			if(LEFT== nWillDo)
			{
				if( !ch ) // 존재하지 않는다면..
				{
					::SendManToManRefusal( SubMenu->key );
					::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,50));
					::MenuSoundClose( SubMenu->Id );
					break;
				}
				::Hprint2( iMainX+iFieldX+50,iMainY+64, g_DestBackBuf, "%s", ch->name );//자리 보정
			}
			else
			{//if(!ch)break; 를 달아 줘야 하지만 오른쪽일 경우는 Hero다 없으면 그냥 튕겨줘
				::Hprint2( iMainX+iFieldX+125,iMainY+64, g_DestBackBuf, "%s", ch->name );//자리 보정
			}
			::SetHangulAlign( TA_LEFT );
			
			::PutLvNameByColor(iX,iY+iLineCt*iLineGab,ch);
			iLineCt++;

			::Hprint2( iX,iY+iLineCt*iLineGab, g_DestBackBuf, "Level : %d",ch->lv );
			iLineCt++;

			::Hprint2( iX,iY+iLineCt*iLineGab, g_DestBackBuf, "HP : %d",ch->_HpMax );
			iLineCt++;//마나 찍는건 안됨
		}break;
	case FT_MAN_TO_MAN_CHECK :
		{
			const int iGabX=3,iGabY=3;
			if( nWillDo == LEFT ) 
			{
				FieldTypeNomalPut(iMainX+iGabX, iMainY+iGabY, iFieldX, iFieldY, iImgNo, iImgType );
			}
			else
			{
				if( iWork == 1 )		// 결투 신청을 받았을때만.
				{
					if( fRcMouse || fLBDown )
					{
						FieldTypeNomalPut(iMainX+iGabX, iMainY+iGabY, iFieldX, iFieldY, iImgNo, iImgType);
					}
				}
			}
		}break;
	case FT_GM_LIST : 
		{
			int do_search = 0;		//  explain버튼이 눌러 져야 하는지
			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;
				SetGMskill();
				do_search = 1;
				for( int a=0; a<5; a++ ) //3~7 필드 우선 필드타입 없에고
				{
					SubMenu->nField[3+a].nType=FT_NO_CHECK;//필드 끄기
				}
			}
			if( !GM_temp_list_count ) 
			{
				Hcolor( FONT_COLOR_NUMBER );
				::RectTextPut(iMainX+59, iMainY+165, 157, lan->OutputMessage(7,89) );//등록 할 수 없소
				break;
			}

			static int ct = iWork;

			if( ct != iWork )		// 스크롤 됐을 경우
			{
				do_search = 1;
				ct = iWork;
			}
			// 1.내가 gm으로 등록해야할 스킬 참고					// GM_temp_list[ct]
			// 2. 그 스킬에 필요한 아이템 및 퀘스트 참고			// g_GmMain
			int gm_list = GM_temp_list[ct];

			GM_QUEST *pGmMain = &g_GmMain[gm_list];

			if( do_search )
			{
				for( int a=0; a<MAX_GM_NEED_ITEM_COUNT; a++ )
				{
					const int iItemNo = pGmMain->need_item[a];
					if( iItemNo )
					{
						int para = g_GM_Item.SearchById( iItemNo );
						if( para == -1 )
						{
							SubMenu->nField[3+a].nType=FT_NO_CHECK;
						}
						else 
						{
							SubMenu->nField[3+a].nSHideNomalCount=0;
							SubMenu->nField[3+a].nType=FT_GM_ITEM_EXPLAIN_BUTTON;
						}
					}
					else
					{
						SubMenu->nField[3+a].nType=FT_NO_CHECK;
					}
				}
			}

			::DisplayGmRegistRequest(gm_list,iMainX,iMainY);

			if( GM_temp_list_count > 1 )		// 스크롤 버튼 찍어 준다.
			{
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
				if( ct-1 < 0 )
				{
					SubMenu->nField[6].nType=FT_NOMAL_PUT;
				}
				else 
				{
					SubMenu->nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				}

				if( ct+1 >= GM_temp_list_count ) 
				{
					SubMenu->nField[7].nType=FT_NOMAL_PUT;
				}
				else 
				{
					SubMenu->nField[7].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				}
			}
			else 
			{
				SubMenu->nField[6].nType=FT_NO_CHECK;
				SubMenu->nField[7].nType=FT_NO_CHECK;
			}
		}break;
	case FT_DISPLAY_GM_REGIST:
		{
			::DisplayGmRegistRequest(iWork,iMainX,iMainY);
		}break;
	case FT_GM_ITEM_EXPLAIN_BUTTON:
		{
			int &ct= SubMenu->nField[j].nSHideNomalCount;
			ct++;
			if(ct < 15)
			{
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iRcImg, iImgType);
			}
			if(ct > 30)	ct = 0;
			if(fRcMouse || fLBDown)
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
		}break;
	case FT_GM_ITEM_EXPLAIN : 
		{
			int width = SubMenu->nField[j].nWillDo;		// 글 찍기 넓이 
			int para = SubMenu->key;		// 전단계 메뉴에서 가져온 g_GM_Item 에서 사용할 파라메터
			int &scroll = iWork;		// 스크롤일경우 그 스크롤 번호
			int &page_max = SubMenu->nField[0].nSHideNomalCount;		// 총 페이지수를 저장할 변수
			if( SubMenu->CheakType == 0 )
			{
				SubMenu->CheakType = 1;
				int line = GetLineRectTextPut( width, g_GM_Item.GetExplain(para) );
				page_max = (line-1)/8 +1 ;
				scroll = 0;
			}
			if( page_max > 1 )
			{
				static int count = 0;
				if( scroll-1 >= 0 )		// 화살표 위로 깜박이기
				{
					SubMenu->nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
					if( (count%14) < 7  )
						FieldTypeNomalPut(iMainX, iMainY, SubMenu->nField[3].x, SubMenu->nField[3].y, SubMenu->nField[3].nImageNumber, SubMenu->nField[3].nImageType);
				}
				else SubMenu->nField[3].nType=FT_NO_CHECK;

				if( scroll+1 < page_max )		// 화살표 아래로 깜박이기
				{
					SubMenu->nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
					if( (count%14) < 7  )
						FieldTypeNomalPut(iMainX, iMainY, SubMenu->nField[4].x, SubMenu->nField[4].y, SubMenu->nField[4].nImageNumber, SubMenu->nField[4].nImageType);
				}
				else SubMenu->nField[4].nType=FT_NO_CHECK;

				count++;
			}

			Hcolor( FONT_COLOR_NAME );
			SetHangulAlign( TA_CENTER );
			int gaby = 0;
			Hprint2( iMainX+137, iMainY+127+gaby*18, g_DestBackBuf, "%s", g_GM_Item.GetName(para) );gaby++;
			SetHangulAlign( TA_LEFT);
			Hcolor( FONT_COLOR_NUMBER );
			
			SkipRectTextPut( iMainX+iFieldX, iMainY+127+gaby*18, width, scroll*8, g_GM_Item.GetExplain(para), 8 );
		}break;
	case FT_QUICK_SKILL_PUT : 
		{
			int ret = 0;
			if( !SubMenu->CheakType ) ret = 1;		// 맨처음 시작한다면...
			
			int x = iMainX+iFieldX;
			int y = iMainY+iFieldY;

			int old_start = SubMenu->nField[j].nSHideNomalStart;
			int old_max = SubMenu->nField[j].nSHideNomalCount;
			static int old_start2 = old_start; 

			static int work = iWork;
			static int scroll_max[4] = {0,};
			static int scroll_count[4] = {0,};
			if( !SubMenu->CheakType || work != iWork || old_start2 != old_start )
			{
				scroll_count[work] = old_start;
				old_start2 = old_start;
				work = iWork;
				SubMenu->CheakType = 1;
				scroll_max[work] = SetQuickSkill( skill_index, scroll_count[work], work );
			}

			int &start = SubMenu->nField[j].nSHideNomalStart;
			int &max = SubMenu->nField[j].nSHideNomalCount;
			start = scroll_count[work];
			max = scroll_max[work];

			FieldTypeNomalPut(iMainX, iMainY,51,2, iWork+12, SKILL_MENU );		// 먼저 제목을 찍고
			if( !max ) return;

			FieldTypeNomalPut(0,0,x,y, 3, SKILL_MENU );		// 왼쪽
			int count = 0;
			int gabx = 0;
			int size = 0;

			while( count < 5 && skill_index[ count ] )
			{
				Spr *s = GetSprOfMenu( SKILL_MENU, 4);
				if( !s ) continue;
				size = s->xl;
				gabx = size*count;
				FieldTypeNomalPut(0,0,x+4+gabx,y, 4, SKILL_MENU );		// 가운데...
				PutSkillIcon( x+4+gabx+20, y+22, skill_index[ count ] );
				DisplayQuickMemory(QM_SKILL,skill_index[ count ], x+4+gabx+20-8,y+22-4);//020610 lsw 퀵에 저장
				count++;
				gabx = s->xl*count;
			}
			FieldTypeNomalPut(0,0,x+4+gabx,y, 5, SKILL_MENU );		// 오른쪽
			static int old_gabx = gabx;
			if( ret || old_gabx != gabx )
			{
				::SetRect(SubMenu->nField[j].rCheakBox,iFieldX,iFieldY,iFieldX+3+gabx,iFieldY+44);//021111 lsw
				old_gabx = gabx;
			}
			///////////////////////////////////////////////////////////////////////////
			if( SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fLButtonDown )
			{
				if( size )
				{
					int ItemX = ( g_pointMouseX - (x+4) )/size;
					PutSkillIcon( x+4+size*ItemX+20, y+22, skill_index[ ItemX ], 8, 2 );
					CheckSkillExplain( skill_index[ ItemX ] );
				}
			}
			////////////////////////////////////////////////////////////////////////////
			if( max >= 5 )
			{
				SubMenu->nField[iSwd].nType = FT_QUICK_SKILL_SCROLL_LR;
				SubMenu->nField[iSwd+1].nType = FT_QUICK_SKILL_SCROLL_LR;
			}
			else 
			{
				SubMenu->nField[iSwd].nType = FT_NO_CHECK;
				SubMenu->nField[iSwd+1].nType = FT_NO_CHECK;
			}
			if( SubMenu->nField[j].fRectMouse )
			{
				int x = iMainX+iFieldX;
				int ItemX = ( g_pointMouseX - (x+4) )/38;
				int s = skill_index[ItemX];
				
				if( !s || !skill[s].num ) break;
				SetQuickMemoryByKeyInput(QM_SKILL,s);
			}
		}break;	
	case FT_GM_2ND_SKILL_MAKE_ITEM : 
		{
			for( int a=0; a<MAX_UPGRADE_RARE; a++ )
			{
				if( gm_make_item_list[a].item_no ) 
					PutItemIcon(	SubMenu->x	+	gm_make_item_list[a].x	,//+	SubMenu->nField[j].x, 
									SubMenu->y	+	gm_make_item_list[a].y	,//+	SubMenu->nField[j].y,
									gm_make_item_list[a].item_no,30,1 );
			}
			break;
		}
	case FT_SKILL_MASTER_MAKE_ITEM://021111 lsw
		{
			for( int a=0; a<2; a++ )
			{
				const ItemAttr Item = g_MultiRareSrc.MultiRareSrc[a].attr ;
				const int iItemNo = Item.item_no;
				if( iItemNo ) 
				{
					PutItemIcon(	iMainX	+	g_MultiRareSrc.MultiRareSrc[a].x	,//+	SubMenu->nField[j].x, 
									iMainY	+	g_MultiRareSrc.MultiRareSrc[a].y	,//+	SubMenu->nField[j].y,
									iItemNo,30,1 );
				}
			}
		}break;
	case FT_EXCHANGE_BOND_MONEY_ITEM:
		{
			const ItemAttr *pItem = Auction.GetBondItemAttr() ;
			if(pItem)
			{
				const int iItemNo = pItem->item_no;
				if( iItemNo ) 
				{
					::PutItemIcon(	iMainX	+	iFieldX+100,
									iMainY	+	iFieldY+30,
									iItemNo,30,1 );
				}

			}
		}break;
	case FT_MAIL_READ : 
		{
			char *text = NULL;
			char szTime[MAX_PATH]={0,};
			switch( nWillDo )
			{
			case 1: 
				{
					text = g_MailMgr.GetTempMailSender();
				}break;
			case 2: 
				{
					text = g_MailMgr.GetTempMailTitle(); 
				}break;
			case 3: 
				{
					text =g_MailMgr.GetTempMailBody(); 
				}break; 
			case 4: 
				{
					g_MailMgr.GetTempMailDate(true,szTime); 
					text = szTime;
				}break; 
			default:{}break;
			}

			// 031110 YGI
			RectTextPut( iMainX+iFieldX, iMainY+iFieldY, SubMenu->nField[j].nRectImage, text , 15, 15 ); // CSD-030723
		}break;
	case FT_MAIL_WRITE:
		{	
			int willdo = SubMenu->nField[j].nWillDo;
			int &count = SubMenu->nField[j].nSHideNomalCount;

			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;

				SetChatMode(CM_COMMAND);
				
				SetFocus2(HWND_MAIL1);//021001 lsw
				SubMenu->work = 1;
			}

			char text[2048] = {0,};
			int max = 0;
			int iHWndIndex = 0;//021001 lsw
			HWND hWnd = EWndMgr.GetHWnd(iHWndIndex);
			switch( willdo )
			{
				case 1: iHWndIndex = HWND_MAIL1; max = 20;   break;
				case 2: iHWndIndex = HWND_MAIL2; max = 29;  break;
				case 3: iHWndIndex = HWND_MAIL3; max = 1000; break;
				default: break;
			}

			EWndMgr.GetTxt(iHWndIndex, text, max );
//			EWndMgr.SetTxt(iHWndIndex,text);//021001 lsw

			if( SubMenu->work == willdo )
				SetFocus2( iHWndIndex );

			ChangeString( text, '\r', ' ');
			//RectTextPut(iFieldX+iMainX, iFieldY+iMainY,iRcImg, text, 15, 15);
			TextOutInRect(text, iFieldX+iMainX, iFieldY+iMainY,iRcImg, 15);

			if( SubMenu->work == willdo )
			{
				if(count%16 <= 8)
				{
					DWORD selStart, selEnd;

					SendMessage(hWnd,EM_GETSEL,(WPARAM)&selStart,(LPARAM)&selEnd);

					if(iHWndIndex != HWND_MAIL3 && selStart <= max)
						Hprint2( iFieldX+iMainX-3 + GetTextWidth("%s",selStart,text), iFieldY+iMainY, g_DestBackBuf, "|");
					else
					{
						int x,y;
						GetCursorLocation(text,selStart,x,y,iRcImg,15);
						Hprint2( iFieldX+iMainX-3 + x, iFieldY+iMainY + y, g_DestBackBuf, "|");
					}
				}

				/*	Removed : added a real cursor.
				if( count %16 > 8 ) 
				{
					sprintf( text, "%s_", text );
				}*/
				
				count++;
			}

		}break;
	case FT_MAIL_LIST:
		{	//타입, 보낸사람, 제목, 날짜
			for(int i = 0; MAX_MAIL_PAGE > i; i++)
			{
				if(!g_MailMgr.GetMailIndex(i))//인덱스 값 없는 놈이면 무시
				{
					continue;
				}
				
				const WORD wDefaultColor = (g_MailMgr.GetMailIsRead(i)?FONT_COLOR_DEFAULT:FONT_COLOR_WHITE);
				
				Hcolor(wDefaultColor);
				const int iX = iMainX+iFieldX;
				const int iY = iMainY+iFieldY+ CMailMgr::LIST_LINE_GAB*i +6;//라인 간격

				//선택됨 마크
				const bool bIsSelect = g_MailMgr.IsSelected(i);
				if(bIsSelect)
				{
					::Hprint2(iX+6,iY-1,g_DestBackBuf,"X");
				}

				//아이콘
				const int iMailType = g_MailMgr.GetMailType(i);
				const int iMailIconNo = g_MailMgr.GetMailIconNo(iMailType);
				::FieldTypeNomalPut(iX+25,iY-4,0,0,iMailIconNo,iImgType);

				if( ::BoxAndDotCrash( iX+25, iY-4, 20, 20, g_pointMouseX, g_pointMouseY ) )
				{
					::DisplayMailIconExplain(iMainX-150,iMainY+10,iMailType);
				}

				//보낸사람
				const char *szSender = g_MailMgr.GetMailSender(i);

				if( BoxAndDotCrash( iX+53,iY-3, 100, 18, g_pointMouseX, g_pointMouseY ) )
				{
					Hcolor(FONT_COLOR_BLIGHT_BLUE);
				}
				else
				{
					Hcolor(wDefaultColor);
				}

				::Hprint2(iX+53,iY,g_DestBackBuf, "%s", (char*)szSender);

				//제목
				if( BoxAndDotCrash( iX+162,iY-3, 130, 18, g_pointMouseX, g_pointMouseY ) )
				{
					::Hcolor(FONT_COLOR_RED);
				}
				else
				{
					::Hcolor(wDefaultColor);
				}

				const char *szTitle = g_MailMgr.GetMailTitle(i);
				char szPress[MAX_PATH] = {0,};
				strcpy(szPress,szTitle);
				szPress[20] = '\0';
				::Hprint2(iX+162,iY,g_DestBackBuf, "%s", szPress);

				Hcolor(wDefaultColor);
				char szTime[MAX_PATH]= {0,};
				g_MailMgr.GetMailDate(i,false,szTime);
				::Hprint2(iX+299,iY,g_DestBackBuf, "%s", szTime);
			}
		}break;
	case FT_SELECT_BETTING_TARGET_TEAM:
		{
			const int iTarget = GetArenaBettingTarget();
			if(nWillDo == iTarget)
			{
				::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
			}
		}break;
	case FT_SELECT_OBSERVER_TARGET_TEAM:
		{	//< CSD-030521
			if (g_pArenaManager->IsEnableGambling(Hero->id))
			{
				const int iTarget = GetArenaObserverTarget();
			
				if (nWillDo == iTarget)
				{
					::FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY, iImgNo, iImgType);
				}
			}

			break;
		}	//> CSD-030521
	case FT_BATTLE_WAIT : 
		{
		}break;
	case FT_FIGHTMAP_START: 
		{	//< CSD-030521
			if (!g_pArenaManager->IsLeader(Hero->id))
			{
				break;
			}

			if(SubMenu->nField[j].fLButtonDown)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
			}
			else if(SubMenu->nField[j].fRectMouse)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
			}									
			else
			{
				int &count = SubMenu->nField[j].nSHideNomalCount;
				count++;
				if( count > 3 )
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
					if( count > 6 ) count = 0;
				}										
			}

			break;
		}	//> CSD-030521
	case FT_VIEW_BATTLE_MEMBER: 
		{	//< CSD-030521
			int gab = SubMenu->nField[j].nSHideNomalStart;		// 스크롤
			int &max = SubMenu->nField[j].nSHideNomalCount;

			CArenaTeam* pTeam = g_pArenaManager->GetArenaTeam(nWillDo);
			
			if (pTeam == NULL)
			{
				break;
			}

			max = pTeam->GetMemberCount()/4 + 1;
			
			CArenaTeam::LIST_MEMBER vtTeam = pTeam->GetMemberSet();
			int nCount = 0;

			for (CArenaTeam::ITOR_MEMBER i = vtTeam.begin(); i != vtTeam.end(); ++i)
			{
				LPCHARACTER pMember = ::ReturnCharacterPoint(*i);
	
				if (pMember == NULL)
				{
					continue;
				}

				char* name = pMember->name;
				const int iPutY = iMainY + iFieldY + nCount*18;

				if (g_pArenaManager->IsLeader(*i))
				{
					Hcolor(FONT_COLOR_NAME);
				}
				else
				{
					Hcolor(FONT_COLOR_NUMBER);
				}

				RectTextPut( iMainX+iFieldX, iPutY, 75, name, 18, 1 );

				if (SubMenu->nField[j].fRectMouse && BoxAndDotCrash(iMainX+iFieldX, iPutY, 100, 20, g_pointMouseX, g_pointMouseY))
				{	//< CSD-030520
					/*
					if( g_FightMapMember.m_dwTotalId[nWillDo][i] )
					{
						//g_ViewFightMapCharInfo = true;
						ViewFightMapCharInfo( 
							g_FightMapMember.m_Name[nWillDo][i], 
							g_FightMapMember.m_dwTotalId[nWillDo][i],
							iMainX-150,
							iMainY+iFieldY );
					}
					*/
				}	//> CSD-030520

				if (++nCount >= 7)
				{
					break;
				}
			}

			break;
		}	//> CSD-030521
	case FT_VIEW_BATTLE_WAIT_MEMBER: 
		{	//< CSD-030521
			int ct = 0;
			CArenaLobby::LIST_MEMBER ltLobby =  g_pArenaManager->GetLobbyInfo()->GetLobbyMemberSet();

			for (CArenaLobby::ITOR_MEMBER i = ltLobby.begin(); i != ltLobby.end(); ++i)
			{
				LPCHARACTER pMember = ::ReturnCharacterPoint(*i);
	
				if (pMember == NULL)
				{
					continue;
				}

				char* pName = pMember->name;

				CArenaTeam* pTeam = g_pArenaManager->GetObserveTeam(*i);

				if (pTeam == NULL)
				{
					::Hcolor(FONT_COLOR_WHITE);
				}
				else
				{
					switch (pTeam->GetTeamNumber())
					{
					case 0:
						{
							::Hcolor(FONT_COLOR_SOLID_RED);
							break;
						}
					case 1:
						{
							::Hcolor(FONT_COLOR_SOLID_BLUE);
							break;
						}
					}
				}

				::Hprint2(iMainX+iFieldX+(ct%4)*100, iMainY+iFieldY+(ct/4)*18, g_DestBackBuf, "%s", pName);
				++ct;
			}

			break;
		 }	//> CSD-030521
	case FT_ARENA_BETTING:
		{
			char temp[20] = {0,};
			::SetFocus2(HWND_3);
			EWndMgr.GetTxt( HWND_3, temp,20 );
			::EatRearWhiteChar( temp );
			DWORD dwMoney = ::atol( temp );

			bool bIsForce = false;
			if(SCharacterData.BankMoney < dwMoney)
			{
				dwMoney = SCharacterData.BankMoney;
				bIsForce = true;
			}
			
			if( bIsForce || CheckNumber( temp, strlen( temp ) ) )
			{	
				sprintf( temp, "%d", dwMoney );
				EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
			}

			::PrintMoney( SubMenu, j, dwMoney, SubMenu->nField[j].nRectImage );
	   }break;
	default: break;
	}
}

void lsw_FT_DATA_PUT(SMENU *SubMenu ,int j)
{
	int i = 0;
	int k = 0;
	int nWeapon_Type;													//무기 종류 판단을 위한 변수 (무기, 방패, 헬멧, 갑옷)
	BOOL flag=TRUE; int & ability=nWeapon_Type;							// 주인공 능력치 출력을 위한 임시 변수

	int &iMainX = SubMenu->x;
	int &iMainY = SubMenu->y;
	int &iFieldX = SubMenu->nField[j].x;
	int &iFieldY = SubMenu->nField[j].y;
	
	int &nWillDo = SubMenu->nField[j].nWillDo;
	int &iImgType = SubMenu->nField[j].nImageType;

	int &nRcImg = SubMenu->nField[j].nRectImage;

	switch(SubMenu->nField[j].nImageNumber)
	{
	case DP_RECT_STRING_PUT_CUSTOM ://색 변형 없는거 //011218 lsw
		{
			const int iHeight= iImgType;//이미지 타입은 글 높이
			const int iColor = nWillDo;
			if( iColor )
			{
				Hcolor( iColor );
			}
			(iHeight)?
				RectTextPut(iMainX+iFieldX,iMainY+iFieldY,nRcImg,SubMenu->nField[j].temp,iHeight): 
				RectTextPut(iMainX+iFieldX,iMainY+iFieldY,nRcImg,SubMenu->nField[j].temp); 
			break;
		}
	//011008 lsw >
	case DP_GM_2ND_SKILL_TARGET_ITEM_PUT:
		{
			int iLineGab = 15;
			if(!gm_make_item_result.item_no){break;}
	//			::PutItemIcon(iMainX+iFieldX ,iMainY+iFieldY+ 10, gm_make_item_result.item_no);	
			memcpy(&Soksungattr_gm,&gm_make_item_result.attr.attr[IATTR_RARE_MAIN],sizeof(int));
			int count=0;
			CItem *item = ItemUnit( gm_make_item_result.attr );

			Hcolor( FONT_COLOR_RARE_MAIN );
			
			Hprint2(iMainX+34, iMainY+90 + count * iLineGab, g_DestBackBuf,"%s",(Soksungattr_gm.IsDynamicRare)? ItemFunction[Soksungattr_gm.soksung1].NameAdd:ItemRare[Soksungattr_gm.soksung1].LocalName);//020505 lsw
			count++;

			Hprint2(iMainX+34, iMainY+90 + count * iLineGab, g_DestBackBuf,"%d Grade",Soksungattr_gm.grade+1 );
			count++;

			HprintBold( iMainX+34,iMainY+90+count* iLineGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemHanName());
			count = 0;
			// x는 현재 수량
			//요구 아이템  x/1;
			//대상 아이템  x/ 100%되는 수량;
			int ineedItemCount1 = 0;
			int ineedItemCount2 = 0;
			int iTargetItemCount= 0;
			const int iResultItemKind = GetItemAttr( gm_make_item_result.item_no, ITEM_KIND);
			if( MAX_ITEM_MUTANT_KIND <= iResultItemKind || 0 > iResultItemKind )
			{
				break;
			}

			const int iNeedItemNo1 = ItemMutantKind[iResultItemKind].AddItem[0]/100;
			const int iNeedItemNo2 = ItemMutantKind[iResultItemKind].AddItem[1]/100;//020505 lsw
			
			for ( int xx = 0 ; xx < MAX_UPGRADE_RARE ; xx++)
			{
				if(iNeedItemNo1 && (iNeedItemNo1  == gm_make_item_list[xx].item_no  ))
				{
					ineedItemCount1++;
				}
				else if(iNeedItemNo2 && ( iNeedItemNo2  == gm_make_item_list[xx].item_no) )
				{
					ineedItemCount2++;
				}
				else if(gm_make_item_list[xx].item_no == gm_make_item_result.item_no)
				{
					iTargetItemCount++;
				}
			}

			const int iUpgradeType = ItemMutantKind[item->GetItemKind()].upgrade_type - 1;//020505 lsw -1 헤서 참조 하라
			if(0 > iUpgradeType ) {break;}//업글 안되는건데 ㅡㅡ;;
			
			const int iGrade		= Soksungattr_gm.grade+1;

			int iPerpectMuch = ItemGrade[iGrade].ResourceAmount[1]; // ResourceAmount[iUpgradeType];
			int iPerpectMuch2 = ItemGrade[iGrade].ResourceAmount[0];
			if (iTargetItemCount > ItemGrade[iGrade].ResourceAmount[1])
			{
				break;
			}

			float fSuccessRate =0.000;
			Hcolor( FONT_COLOR_WHITE );
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s",  GetItemHanName(iNeedItemNo1));
			if(ineedItemCount1< 0 ||  ineedItemCount1 > 1){  Hcolor( FONT_COLOR_PLUS );	}
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"                        %d/1",ineedItemCount1);

			count++;
			if( (iNeedItemNo1 != iNeedItemNo2) && iNeedItemNo2)
			{
				Hcolor( FONT_COLOR_WHITE );
				Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s",  GetItemHanName(iNeedItemNo2));
				if(ineedItemCount2< 0 ||  ineedItemCount2 > 1){   Hcolor( FONT_COLOR_PLUS );	}
				Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"                        %d/1",ineedItemCount2);
				count++;
			}
			Hcolor( FONT_COLOR_RARE_MAIN );
			
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s ", (Soksungattr_gm.IsDynamicRare)?ItemFunction[Soksungattr_gm.soksung1].NameAdd:ItemRare[Soksungattr_gm.soksung1].LocalName);
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"                    %d Grade", Soksungattr_gm.grade);
			count++;

			Hcolor( FONT_COLOR_WHITE );
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"%s",item->GetItemHanName() );
			if( iTargetItemCount > iPerpectMuch2 || iTargetItemCount == 0)
			{
				Hcolor( FONT_COLOR_PLUS );
			}
			Hprint2(iMainX+34, iMainY+205 + count * iLineGab, g_DestBackBuf,"                        %d/%d",iTargetItemCount,iPerpectMuch);
			count++;

			Hcolor( FONT_COLOR_WHITE );
			Hprint2(iMainX+34, iMainY+138 , g_DestBackBuf,"SuccessRate [   %%]");

			if ( ineedItemCount1  && iNeedItemNo1)//요구 아이템이 존재 하고 아이템 갯수가 있다
			{
				fSuccessRate = float(iTargetItemCount) / float(iPerpectMuch2);

				if((int)fSuccessRate && iNeedItemNo2)
				{
					if( ineedItemCount2 )//두번째 요구치도 존재 한다
					{
						fSuccessRate = float(iTargetItemCount) / float(iPerpectMuch2);
					}
					else
					{
						fSuccessRate = 0;
					}
				}
			}
			if( (fSuccessRate*100) < 50)
			{
				Hcolor( FONT_COLOR_PLUS );
				SMenu[MN_GM_2ND_SKILL].nField[0].nType = 0;
			}
			else
			{
				Hcolor( FONT_COLOR_YELLOW );
				SMenu[MN_GM_2ND_SKILL].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			}
			Hprint2(iMainX+34, iMainY+138, g_DestBackBuf,"             %3d",int(fSuccessRate*100));
		}break;//case DP_GM_2ND_SKILL_TARGET_ITEM_PUT:
	case	DP_SKILL_MASTER_RESULT_ITEM://재료분석도 찍어야 함
		{	//결과물 아이템 뽑는 루틴//결과물 아이템의 아이템 이름과 레어만 찍음
			const int iLGab = 18;
			
			ItemAttr Item1= g_MultiRareSrc.MultiRareSrc[0].attr;
			ItemAttr Item2= g_MultiRareSrc.MultiRareSrc[1].attr;
			
			if(!Item1.item_no ||!Item2.item_no){break;}//아이템 번호는 반드시있어야 하고
			if(Item1.item_no != Item2.item_no){break;}//아이템 번호가 같아야지

			const RareMain *pTempRare1 =  (RareMain *)&Item1.attr[IATTR_RARE_MAIN];
			const RareMain *pTempRare2 =  (RareMain *)&Item2.attr[IATTR_RARE_MAIN];
			if(	pTempRare1->IsDynamicRare
			||	pTempRare2->IsDynamicRare
			||	pTempRare1->iHighLevel
			||	pTempRare2->iHighLevel){break;}//하이나 기능 아템은 출력 안시켜.. 틀린거 거든
			
			RareMain RareResult;
			{//결과물 셋팅
				RareResult.grade = (pTempRare1->grade+pTempRare2->grade)/2;
				RareResult.soksung1=pTempRare1->soksung1;
				RareResult.soksung2=pTempRare2->soksung1;
				RareResult.soksung3=0;
				RareResult.iHighLevel = 0;
				RareResult.IsDynamicRare = false;
			}

			const RareMain *pRare = &RareResult;
			const int iGrade = pRare->grade;
			const int iWidth = TXT_BG_NORMAL_WIDTH;//021111 lsw

			CItem *item = ItemUnit( gm_make_item_result.attr );
			Hcolor( FONT_COLOR_RARE_MAIN );
			if(!aIMR[iGrade].iAble)
			{	//안됨
				break;
			}
			int iCt= 0;
			::Hprint2(iMainX+34, iMainY+104 + iCt * iLGab, g_DestBackBuf,"%s",(pRare->IsDynamicRare)? ItemFunction[pRare->soksung1].NameAdd:ItemRare[pRare->soksung1].LocalName);//020505 lsw
			::Hprint2(iMainX+34, iMainY+104 + iCt * iLGab, g_DestBackBuf,"                     %d Grade",iGrade );
			iCt++;
			
			::Hprint2(iMainX+34, iMainY+104 + iCt * iLGab, g_DestBackBuf,"%s",(pRare->IsDynamicRare)? ItemFunction[pRare->soksung2].NameAdd:ItemRare[pRare->soksung2].LocalName);//020505 lsw
			::Hprint2(iMainX+34, iMainY+104 + iCt * iLGab, g_DestBackBuf,"                     %d Grade",iGrade );
			iCt++;

			HprintBold( iMainX+34,iMainY+104+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemHanName());
			iCt=0;

			const int iX = iMainX+iFieldX+5;
			const int iY = iMainY+iFieldY+150;

			bool bIsFail = false;

			if(Item1.item_no)
			{
				::Hprint2(iX, iY+ iCt * iLGab, g_DestBackBuf,"%s",(pTempRare1->IsDynamicRare)? ItemFunction[pTempRare1->soksung1].NameAdd:ItemRare[pTempRare1->soksung1].LocalName);//020505 lsw
				::Hprint2(iX, iY+ iCt * iLGab, g_DestBackBuf,"                     %d Grade",pTempRare1->grade );
				iCt++;
				::HprintBold( iX,iY+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemHanName());
				iCt++;
			}
			else
			{
				bIsFail = true;
			}

			if(Item2.item_no)
			{
				::Hprint2(iX, iY+ iCt * iLGab, g_DestBackBuf,"%s",(pTempRare2->IsDynamicRare)? ItemFunction[pTempRare2->soksung1].NameAdd:ItemRare[pTempRare2->soksung1].LocalName);//020505 lsw
				::Hprint2(iX, iY+ iCt * iLGab, g_DestBackBuf,"                     %d Grade",pTempRare2->grade );
				iCt++;
				::HprintBold( iX,iY+iCt* iLGab, FONT_COLOR_NAME, RGB16( 0,0,0 ),"%s",item->GetItemHanName());
				iCt++;
			}
			else
			{
				bIsFail = true;
			}
			
			for(int xx= 0;MAX_IMR_FIELD_CT>xx;xx++)
			{
				int iNationAddCt = 0;
				switch(Hero->name_status.nation)
				{
				case NW_BY:{if(xx == 0){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				case NW_ZY:{if(xx == 1){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				case NW_YL:{if(xx == 2){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				}
				const int iNeedItemNo = aIMR[iGrade].aItemNo[xx];
				const int iNeedItemCt = aIMR[iGrade].aItemCt[xx]+iNationAddCt;
				if(iNeedItemNo && iNeedItemCt)
				{
					CItem *t = ItemUnit( iNeedItemNo );
					if(!t){continue;}
					const int iNowCt = ::CheckInventory(iNeedItemNo,iNeedItemCt);//내가 원한 값에서 현재 결과를 주니까 -값이 나올 수 있다
					const int iRCT = TxtOut.RcTXTOutNoBG(iX,iY+iLGab*iCt,iWidth,0,"%s %d/%d",t->GetItemHanName(),iNowCt+iNeedItemCt,iNeedItemCt);
					iCt+=iRCT;
					if(0 > iNowCt)//재료가 모자라네 실패
					{
						bIsFail = true;
					}
				}
			}			
			
			Hcolor( FONT_COLOR_WHITE );
			Hprint2(iMainX+34, iMainY+155 , g_DestBackBuf,"SuccessRate [   %%]");
			if(bIsFail)
			{
				Hcolor( FONT_COLOR_PLUS );
				Hprint2(iMainX+34, iMainY+155, g_DestBackBuf,"             %3d",0);
			}
			else
			{
				Hcolor( FONT_COLOR_YELLOW );
				Hprint2(iMainX+34, iMainY+155, g_DestBackBuf,"             %3d",50);
			}

		}break;//case	DP_SKILL_MASTER_RESULT_ITEM://기성의 결과물 아이템 디스플레이
	case	DP_NATION_WAR_RESULT_BAR:
		{
		//	if( nWillDo==0 )
		//	{	break;		}

			if( nWillDo > SubMenu->nField[j].nSpecialWillDo)
			{
	//				SubMenu->nField[j].nSpecialWillDo++;
				SubMenu->nField[j].nSpecialWillDo+=(rand()%10+1);//011031 lsw 
			}
			else
			{
				SubMenu->nField[j].nSpecialWillDo = nWillDo;
			}

			// 500 명기준에
			float max = ( 250.000 * float(SubMenu->nField[j].nSpecialWillDo) / 3000.000 );
			for(int iCount = 0; iCount < __min( max, 250 ) ; iCount++)//011031 lsw
			{
				FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX+iCount,iFieldY,nRcImg, NATION_WAR_RESULT_IMG, 6, 1);
			}
			Hprint2(iMainX+iFieldX, iMainY+iFieldY+3, g_DestBackBuf,"%d",SubMenu->nField[j].nSpecialWillDo);
		}break;
	case DP_NATION_WAR_RESULT_ANIMATION:
		{
			Spr *s1;
			Spr *s2;

			int iType = 	iImgType;

			int *iAniCount;
			int *iLoofStartFrame ;
			int *iLoofEndFrame	;
			int *iLoofOKFlag	;
			
			static int delayTime = 1;//delayTime은 0이 되면 안된다
			static int delayTimeValue = 0;

			iAniCount		=	&WarResultAniCount;
			iLoofStartFrame =	&nWillDo;			//루프의 시작점
			iLoofEndFrame	= 	&nRcImg;		//루프의 끝점
			iLoofOKFlag		=	&SubMenu->nField[j].nSpecialWillDo;	//루프 돌까 말까

			//루프 돌아라에 체크가 들어오지 않으면 그냥 증가한다
			//맥스치 까지 가면 루프 체크를 해 주고 시작 프레임과 마지막 프레임을 정한다
			//플래그는 앞으로 갈때 2 를 주고 뒤로 갈때 -2 를 준다

			if( (*iLoofOKFlag) )//플래그가 값이 있을때
			{
				if ( !(  delayTimeValue % delayTime) )
				{
					(*iAniCount) += (*iLoofOKFlag);
					delayTimeValue = 1;
				}

				delayTimeValue++;

				if( (*iAniCount) < (*iLoofStartFrame))
				{
					(*iLoofOKFlag) = 2;
				}
				else if( (*iAniCount) > (*iLoofEndFrame) )
				{
					(*iLoofOKFlag) = -2;
				}
			}
			else//플래그 값없을때 (초기)
			{
				if(iType == 0)//승리 시에는
				{
					if( (*iAniCount) > 92 )//카운트는꼭! 짝수로 맞출것
					{
						(*iLoofOKFlag)  = 2;
						(*iAniCount)	=	(*iLoofStartFrame)		= 94;
						(*iLoofEndFrame)		= 104;
						delayTime = 2;
					}
					iFieldX = 180;
					iFieldY = 250;
				}
				else
				{
					if( (*iAniCount) > 84 )//카운트는꼭! 짝수로 맞출것
					{
						(*iLoofOKFlag)  = 2;
						(*iAniCount)	=	(*iLoofStartFrame)		= 48;
						(*iLoofEndFrame)		= 54;
						delayTime = 3;
					}
					iFieldX = 180;
					iFieldY = 300;
				}
				(*iAniCount)+=2;
			}
			//실제 애니메이션 데이타 선택
			if( iType == 0 )
			{
				if( (*iAniCount)==0 || (*iAniCount)==104) MP3( 2211 );
				if( (*iAniCount) == 44 ) MP3( 2204 );
				if( (*iAniCount) == 60  ) MP3( SN_FIGHT_WIN );
				s1 = GetSprOfMenu( FIGHT_MAP_WIN, (*iAniCount));
				s2 = GetSprOfMenu( FIGHT_MAP_WIN, (*iAniCount)+1);
			}
			else
			{
				if( (*iAniCount)==22 || (*iAniCount)==32 ) MP3( 302 );
				if( (*iAniCount) == 70 ) MP3( 2203 );
				if( (*iAniCount)== 84 ) MP3( SN_FIGHT_LOSE );
				s1 = GetSprOfMenu( FIGHT_MAP_LOSE, (*iAniCount));
				s2 = GetSprOfMenu( FIGHT_MAP_LOSE, (*iAniCount)+1);
			}

			//출력
			if( s1 && s1->img ){PutCompressedImage	( iMainX+iFieldX, iMainX+iFieldY, s1 );}
			if( s2 && s2->img ){PutCompressedImageFX( iMainX+iFieldX, iMainX+iFieldY, s2, 17, 2 );}	
		}break;
	case DP_RARE_UPGRADE_RESULT:
		{
			if(nWillDo==100)
			{
				AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,510));
			//	AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,257));//011031 lsw
				nWillDo = 0;
				CloseAllMenu();	
			}
			
			else if( nWillDo < 100 && nWillDo > 0) 
			{
				//011030 lsw >
				AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,223));
		//		AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,253));//011031 lsw
				nWillDo = 0;
				//011030 lsw <
				CloseAllMenu();	
			}
		}break;
	//011008 lsw <
	case	DP_NATION_JOIN_STATUS_BAR:
		{
			float max = ( 150.000 * float(nWillDo) / 200.000 );
			
			for(i = 0; i < max; i++)//전쟁 가능 되는것
			{
				 FieldTypeNomalPutFx3(iMainX, iMainY, iFieldX,iFieldY-i,nRcImg, NATION_JOIN_IMG, 15, 1);
				 if(i > 150 ) break;
			}
		}
		break;
	case DP_REMAIN_TIME_PUT:
		{
			static int NationTime = timeGetTime();

			if(	timeGetTime() - NationTime> 1000)
			{
				NationTime = timeGetTime();
				if(nRcImg > 0)
				{
				nRcImg --;
				}
			}

			int RemainTime  = nRcImg; 

			DWORD temp =DWORD(RemainTime % 3600);
			Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%2d:%2d:%2d",char(RemainTime / 3600),char(temp / 60),char(temp % 60));
		}break;
	case	DP_RARE_SUCCESS_RATE_DISPLAY_NO_TOTAL:
		{
			int JoinNo = item_select_result.join_no;
			
			int sno = SkillTable[SkillNo].MotherSkillType;
			//011031 lsw>
			if( JoinNo && SkillNo && sno && item_select_result.item_no)
			{
				int myskillLv = SCharacterData.nJobSkill[sno].skillexp/10000;
				int itemLv = item_join[JoinNo].skill1_minimum;
				
				CItem *t = ItemUnit(item_select_result.item_no);
				if(!t) break;
				int iFuncItem = 0;
				switch(item_select_result.item_no/1000)
				{
				case PLANT			:
					{
						iFuncItem = ((CItem_Plant*)t)->cure_disease1;
					}break;
				case HERB			:
					{
						iFuncItem = ((CItem_Herb*)t)->cure_disease1;
					//	cure_disease1
					}break;
				case COOK			:
					{
						iFuncItem = ((CItem_Cook*)t)->cure_disease1;
					//	cure_disease1
					}break;
				case POTION			:
					{
						iFuncItem = ((CItem_Potion*)t)->cure_disease1;
					//	cure_disease1
					}break;
				case MINERAL		:
				case TOOL			:
				case WEAPON			:
				case DISPOSABLE		:
				case ARMOR			:
				case ACCESSORY		:
				case ETC			:
				default:break;
				}
				if(iFuncItem)
				{
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					"%s", ItemFunction[iFuncItem].NameAdd);
					break;
				}

				int itemKind = 	ItemMutantKind[t->GetItemKind()].upgrade_type-1;// -1 이 나오면 레어업그레이드 불가
				
				if(itemKind > 3 || itemKind < 0)
				{
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					"%s", lan->OutputMessage(2,521));
				}
				else
				{
					//011030 lsw >
					int r = SCharacterData.nJobSkill[sno].rare/10+RareProbability[myskillLv - itemLv].bonus_suc/10;
					if( r > 100 )
					{	r = 100;	}
					if( r < 0	)
					{	r = 0;		}
					Hcolor( FONT_COLOR_WHITE );
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					lan->OutputMessage(2,520),
					//rare_probability[myskillLv - itemLv].exp_rare_suc[itemKind], 
					SCharacterData.nJobSkill[sno].rare/10,
					SCharacterData.nJobSkill[sno].rare%10,
					RareProbability[myskillLv - itemLv].bonus_suc/10
					);
					//011030 lsw <
					Hcolor( FONT_COLOR_YELLOW );
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					"                 %3d ",RareProbability[myskillLv - itemLv].bonus_suc/10);
					Hcolor( FONT_COLOR_WHITE );
				}
			}
		}break;

	case	DP_RARE_SUCCESS_RATE_DISPLAY:
		{
			int JoinNo = item_select_result.join_no;
			
			int sno = SkillTable[SkillNo].MotherSkillType;

			if( JoinNo && SkillNo && sno)
			{
				int myskillLv = SCharacterData.nJobSkill[sno].skillexp/10000;
				int itemLv = item_join[JoinNo].skill1_minimum;
				
				CItem *t = ItemUnit(item_select_result.item_no/1000,item_select_result.item_no%1000);
				if(!t) break;

				int itemKind = 	ItemMutantKind[t->GetItemKind()].upgrade_type-1;// -1 이 나오면 레어업그레이드 불가
				
				if(itemKind > 3 || itemKind < 0)
				{
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					"%s", lan->OutputMessage(2,254));
				}
				else
				{
					t_SkillExp2 *dayexp ;
					dayexp = (t_SkillExp2*)&SCharacterData.nJobSkill[sno-14];

					if( dayexp->makecount > RareProbability[myskillLv].max_rare_day)
					{
						dayexp->makecount  = RareProbability[myskillLv].max_rare_day;
					}
					//011030 lsw >
					int r = SCharacterData.nJobSkill[sno].rare/10+RareProbability[myskillLv - itemLv].bonus_suc/10;
					if( r > 100 )
					{	r = 100;	}
					if( r < 0	)
					{	r = 0;		}
					//011110 lsw
					if(dayexp->makecount < RareProbability[myskillLv].max_rare_day )
					{
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					lan->OutputMessage(2,255),
					//rare_probability[myskillLv - itemLv].exp_rare_suc[itemKind], 
					SCharacterData.nJobSkill[sno].rare/10,
					SCharacterData.nJobSkill[sno].rare%10,
					RareProbability[myskillLv - itemLv].bonus_suc/10, 
					RareProbability[myskillLv - itemLv].bonus_suc%10, 
					r,
					dayexp->makecount,
					RareProbability[myskillLv].max_rare_day
					);
					}
					else
					{
					Hprint2(iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,
					lan->OutputMessage(2,265),
					//rare_probability[myskillLv - itemLv].exp_rare_suc[itemKind], 
					SCharacterData.nJobSkill[sno].rare/10,
					SCharacterData.nJobSkill[sno].rare%10,
					RareProbability[myskillLv - itemLv].bonus_suc/10, 
					RareProbability[myskillLv - itemLv].bonus_suc%10, 
					r
					);
					}

					//011030 lsw <
				}
			}
		}
		break;
	case DP_HEALTH_GAUGE :	
		{
			if( SCharacterData.nCharacterMAXSP > 0 )
			{	
				const int iMax = SCharacterData.nCharacterMAXSP;
				const int iNow = SCharacterData.nCharacterSP;

				int iTarget = ((float)iNow) / ((float)iMax) * ((float)SPBarWidth) ; 
				if( SPBarWidth < iTarget){iTarget = SPBarWidth;}
				for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
				{
					FieldTypeNomalPut(iMainX, iMainY, iFieldX+iCount, iFieldY, 							
						nRcImg, iImgType);
				}
			}
		}break;
	case DP_HP_BAR :	
		{
			//int realiNow = 0;
			if( SCharacterData.nCharacterMAXHP > 0 )
			{
				const int iMax = SCharacterData.nCharacterMAXHP;
				const int iNow = /*(Hero->hp > iMax) ? iMax : Hero->hp;*/SCharacterData.nCharacterHP/*Hero->hp*/; //Eleval 18/08/09 - HP BUG
	
				int iTarget = ((float)iNow) / ((float)iMax) * ((float)HPMPBarHeight) ; 
				if( HPMPBarHeight < iTarget){iTarget = HPMPBarHeight;}
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY,
						0, iImgType);
				for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
				{
					FieldTypeNomalPutFx3(iMainX+4, iMainY+HPMPBarHeight+17, iFieldX, iFieldY-iCount,
						nRcImg, iImgType,13,1);
				}					
				FieldTypeNomalPut(iMainX, iMainY,iFieldX, iFieldY,
						1, iImgType);

				//realiNow = iNow;
			}
			SetHangulAlign( TA_CENTER );
			Hcolor( FONT_COLOR_WHITE );
			Hprint2( iMainX+iFieldX+30, iMainY+iFieldY+HPMPBarHeight, g_DestBackBuf, "%d",  SCharacterData.nCharacterHP/*Hero->hprealiNow*/); //test from eleval for HP BUG
		}break;
	case DP_HP :
		{	
			int hp = SCharacterData.nCharacterHP/*Hero->hp*/; //Eleval 18/08/09 - HP Bug
			if( hp > SCharacterData.nCharacterMAXHP ) hp = SCharacterData.nCharacterMAXHP;
			SetHangulAlign( TA_RIGHT ); 
//				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d/%d",hp, SCharacterData.nCharacterMAXHP);
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.nCharacterMAXHP);
			break;
		}
	case DP_MP_BAR :	
		{
			if( SCharacterData.nCharacterMAXMP > 0 )
			{
				const int iMax = SCharacterData.nCharacterMAXMP;
				const int iNow = SCharacterData.nCharacterMP;
				
				int iTarget = ((float)iNow) / ((float)iMax) * ((float)HPMPBarHeight) ;
				if( HPMPBarHeight < iTarget){iTarget = HPMPBarHeight;}
				FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY,0, DIVINE_IMG);
				if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL )//디바인 일때
				{
					for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
					{
						FieldTypeNomalPutFx3(iMainX+21, iMainY+HPMPBarHeight+17, iFieldX, iFieldY-iCount,nRcImg, DIVINE_IMG,13,1);
					}					
				}
				else 
				{
					for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
					{
						FieldTypeNomalPutFx3(iMainX+21, iMainY+HPMPBarHeight+17, iFieldX, iFieldY-iCount,nRcImg, MANA_IMG,13,1);
					}					
				}
				FieldTypeNomalPut(iMainX, iMainY,iFieldX, iFieldY,1, DIVINE_IMG);
			}
			SetHangulAlign( TA_CENTER );
			Hcolor( FONT_COLOR_WHITE );
			Hprint2( iMainX+iFieldX+47, iMainY+iFieldY+HPMPBarHeight, g_DestBackBuf, "%d",  SCharacterData.nCharacterMP);
		}break;
	case DP_MP :	
		{
		//	if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL )
		//	{
		//		FieldTypeNomalPut( iMainX, iMainY, 10, 196, 23, MAIN_ETC );
			//	}

			SetHangulAlign( TA_RIGHT ); 
//				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d/%d",SCharacterData.nCharacterMP, SCharacterData.nCharacterMAXMP);
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d",SCharacterData.nCharacterMAXMP);
		}break;


	case DP_MAIN_LEVEL : 
		{
			const int before_exp = NPC_Lev_Ref[SCharacterData.nLevel].nMinExp;
			if( SCharacterData.nExpNext-before_exp> 0 )
			{
				const int iMax = NPC_Lev_Ref[SCharacterData.nLevel].nMaxExp - before_exp;//다음 경험치 총량 - 이전 경험치 총량
				const int iNow = __max(SCharacterData.nExp-before_exp,0);

				const float fExpRate= ((float)iNow) / ((float)iMax);
				const int iTarget = __min((int)(fExpRate * EXPBarWidth), EXPBarWidth); 
				for(int iCount = 0; iCount < iTarget; iCount++)//011031 lsw
				{
					::FieldTypeNomalPut(iMainX, iMainY, iFieldX+iCount, iFieldY, nRcImg, iImgType );
				}
				
				
				::Hcolor(ConvertColor(200,200,9));
				if(SCharacterData.nLevel <= 100)//100 Level 이하 경험치 나옴
				{
					::SetHangulAlign( TA_RIGHT );
					::HprintBold( iMainX+iFieldX+EXPBarWidth, iMainY+iFieldY, 
						Convert16to32(FONT_COLOR_EXP_PERCENT),Convert16to32(FONT_COLOR_BLACK),
						"%d", SCharacterData.nExp );
				}
				::SetHangulAlign( TA_LEFT );
				int Per = (int)(fExpRate*100);
			//	if(0> Per){Per += 100; Per *= -1;}
				::HprintBold( iMainX+iFieldX, iMainY+iFieldY, 
						Convert16to32(FONT_COLOR_EXP_PERCENT),Convert16to32(FONT_COLOR_BLACK),
						"%d%%", Per);
//							"%d%%", __max(0,(int)(fExpRate*100)) );
			}
		 }break;
	
	case DP_FACE :	//020515 lsw
		{
			FieldTypeNomalPut(iMainX, iMainY, iFieldX, iFieldY,nRcImg,iImgType ); //표정 그림은 255번 부터시작 
		}break;
	case DP_FACEIMAGE :	// 얼굴 이미지 찍기
		{
			PutCharImage(iMainX+iFieldX, iMainY+iFieldY, SCharacterData.nCharacterData[FACE]); 
		}break;
	case DP_FACE_PUT : //011001 lsw
		{
			if(nWillDo)
			{
				 PutCharImage( iMainX+iFieldX, iMainY+iFieldY, nRcImg, 6, FS_SMALL,2, iImgType );
			}
		}
		break;
	case DP_LEVEL_IN_TIME_MENU:
		{
			SetHangulAlign( TA_LEFT );
			//::Hcolor( Lv_Color[SCharacterData.nLevel-1].GetRGB16() );
			::HprintBold( iMainX+iFieldX, iMainY+iFieldY, 
						Convert16to32(FONT_COLOR_WHITE),Convert16to32(FONT_COLOR_BLACK),
						"Lv.%d", SCharacterData.nLevel );
		}break;
	case DP_LEVEL :	
		{
		//	SetHangulAlign( TA_RIGHT );
			::Hcolor( Lv_Color[SCharacterData.nLevel-1].GetRGB16() );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d",SCharacterData.nLevel);
		}break;
	case DP_EXP :	
		{
			if( SCharacterData.nLevel <= 100 || GetSysInfo( SI_GAME_MAKE_MODE ) )
			{
				SetHangulAlign( TA_RIGHT );
				static DWORD old_exp = SCharacterData.nExp;
				static DWORD old_exp2 = SCharacterData.nExp;
				static int plus = 0;

				if( !SubMenu->CheakType )	old_exp = SCharacterData.nExp;

				if( old_exp < SCharacterData.nExp )
				{
					bool refresh_plus = false;
					if( old_exp2 < SCharacterData.nExp ) refresh_plus = true;
					old_exp2 = SCharacterData.nExp;

					if( !plus || refresh_plus )
					{
						plus = (int )((SCharacterData.nExp - old_exp)/30.0f);
						if( !plus ) plus = 1;
					}
					Hprint2( iMainX+iFieldX+88, iMainY+iFieldY, g_DestBackBuf,"%u",old_exp);
					old_exp += plus;
					if( old_exp > SCharacterData.nExp ) old_exp = SCharacterData.nExp;
				}
				else
				{
					Hprint2( iMainX+iFieldX+88, iMainY+iFieldY, g_DestBackBuf,"%u",SCharacterData.nExp);
					plus = 0;
				}
			}
			else//100 렙 이상
			{
				const int nWidth = 88;
				DWORD exp = SCharacterData.nExp;
				DWORD expNext = SCharacterData.nExpNext;
				if( expNext )
				{
					int unit = expNext/10;
					int type = exp/unit;		// 몇번째 인가..
					if( type >= 1 )
					{
						for( int i=0; i<nWidth; i++ )
						{
							FieldTypeNomalPut( iMainX+i, iMainY-2, iFieldX, iFieldY, 0, IIT_EXP_GAUGE );
						}
					}
					int count = int((float)(exp-unit*type)*nWidth/(unit));
					if(count > nWidth)
					{
						count = nWidth;
					}
				}
			}
		}break;
	case DP_EXP_NEXT :	
		{
			if( SCharacterData.nLevel <= 100 )
			{
				SetHangulAlign( TA_RIGHT );
				Hprint2( iMainX+iFieldX+88, iMainY+iFieldY, g_DestBackBuf,"%u",SCharacterData.nExpNext);
				break;
			}
			else
			{
				const int nWidth = 88;
				DWORD exp = SCharacterData.nExp;
				DWORD expNext = SCharacterData.nExpNext;
				if( expNext )
				{
					int count = (int)((float)exp*nWidth/expNext);
					if(count > nWidth)
					{
						count = nWidth;
					}
#ifdef _DEBUG // finito 060507
					if( GetSysInfo( SI_NOT_CONNECT_MODE ) )
					{
						SCharacterData.nExp += 500000;
					}
#endif
					for( int i=0; i<count; i++ )
					{
						FieldTypeNomalPut( iMainX+i, iMainY-2, iFieldX, iFieldY, 0, IIT_EXP_GAUGE );
					}
				}
			}
		}break;
	case DP_DAMAGE :	
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.nCharacterAbility[DAMAGE] );
		}break;
	case DP_HEALTH :	
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d/%d",SCharacterData.nCharacterSP, SCharacterData.nCharacterMAXSP);
		}break;
		
	case DP_TACTICS_DATA :	
		{
			if( !SubMenu->CheakType )	// 택틱스 창 활성화 바로 직후
			{
				//SubMenu->CheakType = 1;		//FT_EXPLAIN_MENU에서 초기화 해준다.
				ReqTacSkillExp( );		// 수치 갱신 
				SubMenu->work = refresh_magery_time;
			}

			if( refresh_magery_time <= 0 ) 
			{
				CallServer( CMD_REQ_MAGERY );  //서버와 프로토콜을 맞춘 후...
				refresh_magery_time = 100;
			}
			else refresh_magery_time--;

			int x = iMainX+iFieldX;
			int y = iMainY+iFieldY;
			int tactic_para[][13] = 
			{ 
				{ CRAPPLE, SWORD, ARCHERY, PARRYING, MACE, PIERCE, WHIRL, HURL, MAGERY, D_SWORD, D_MACE, }, 
				{ CRAPPLE, SWORD, ARCHERY, FENCING, MACE, PIERCE, WHIRL, HURL,PARRYING, MAGERY, }, 

			};
			int max;
			int first_param = 0;
			if( SCharacterData.nCharacterData[GENDER] == MALE )
			{
				if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )
				{
			//	#if defined (TAIWAN_LOCALIZING_) || defined (HONGKONG_LOCALIZING_)//010730 lms 대만 magery 덮어쓰기
			//		FieldTypeNomalPut( iMainX, iMainY, 10, 216, nWillDo, iImgType );	// magery 로 덮어 씌우기
			//	#else
			//		FieldTypeNomalPut( iMainX, iMainY, 11, 220, nWillDo, iImgType );	// magery 로 덮어 씌우기
			//	#endif
					tactic_para[0][8] = MAGERY;
				}
				else tactic_para[0][8] =  ORISON;

				first_param = 0;
				max = 11;
			}
			else
			{
				if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )
				{
					tactic_para[1][9] = MAGERY;//공격마법운용
				//	FieldTypeNomalPut( iMainX, iMainY, 10, 244, nWillDo, iImgType );	// magery 로 덮어 씌우기
				}
				else tactic_para[1][9] =  ORISON;

				first_param = 1;
				max = 10;
			}
			int lv, exp, exp_next;
			const int nWidth1 = 69;
			const int nWidth2 = 69;

			for( int a = 0; a<max; a++ )
			{
#ifdef _DEBUG // finito 060507
				if( GetSysInfo( SI_NOT_CONNECT_MODE ) )
				{
					Hero->aStepInfo[a + TAC_INDEX] = 1;
					SCharacterData.TacticsExp[tactic_para[first_param][a]] += 100000;
					RecvTacSkillExp( (t_char_info_tac_skillexp *)SCharacterData.TacticsExp );
				}
#endif
				lv = SCharacterData.SkillId[tactic_para[first_param][a]+88]+1;
				exp = SCharacterData.TacticsExp[tactic_para[first_param][a]];
				exp_next = SCharacterData.TacticsExpNext[tactic_para[first_param][a]];
			//	Hprint2( x, y+gabx[first_param][a], g_DestBackBuf,"%d", lv );
				Hprint2( x, y+a*23, g_DestBackBuf,"%d", lv );
				if( lv > 101 )
				{
					if( exp_next )
					{
						int unit = exp_next/10;
						int type = exp/unit;		// 몇번째 인가..
						if( type >= 1 )
						{
							for( int i=0; i<nWidth1; i++ )
							{
								FieldTypeNomalPut( iMainX+23+i, iMainY+a*23, iFieldX, iFieldY, type, IIT_EXP_GAUGE );
							}
						}
						int count = int((float)(exp-unit*type)*nWidth1/(unit));
						for( int i=0; i<count; i++ )
						{
							FieldTypeNomalPut( iMainX+23+i, iMainY+a*23, iFieldX, iFieldY, (type+1), IIT_EXP_GAUGE );
						}
					}

					if( exp_next )
					{
						int count = (int)((float)exp*nWidth2/exp_next);
						for( int i=0; i<count; i++ )
						{
						//	FieldTypeNomalPut( iMainX+97+i, iMainY+gabx[first_param][a]-1, iFieldX, iFieldY, 0, IIT_EXP_GAUGE );//
							FieldTypeNomalPut( iMainX+96+i, iMainY+a*23, iFieldX, iFieldY, 0, IIT_EXP_GAUGE );//
						}
					}
				}

				if( lv <= 101 || GetSysInfo( SI_GAME_MAKE_MODE ) )//100 이하는
				{
					Hprint2( x+25, y+a*23, g_DestBackBuf,"%u", exp );		//디스플레이 해준다
					Hprint2( x+98, y+a*23, g_DestBackBuf,"%u", exp_next );	//디스플레이 해준다
				}
			}
			break;
		}
	 //가치관 출력
	case DP_ARIGEMENT :	
		{
			Hcolor( FONT_COLOR_DEFAULT );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_ARIGEMENT_TEXT][SCharacterData.nCharacterData[ARIGEMENT]].temp);
		}break;
	case DP_NAME_CENTER :		//이름 출력
		{
			Hcolor( FONT_COLOR_NAME );
			SetHangulAlign(TA_CENTER);
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s", SCharacterData.sCharacterName );
		}break;
	case DP_NAME :		//이름 출력
		{
			Hcolor( FONT_COLOR_NAME );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s", SCharacterData.sCharacterName );
		}break;

	case DP_CLASS :	//클래스 출력
		{
			Hcolor( FONT_COLOR_DEFAULT );
			SetHangulAlign(TA_CENTER);
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][SCharacterData.nCharacterData[CLASS]].temp );
		}break;

	case DP_SPELL :		//스펠 출력
		{
			Hcolor( FONT_COLOR_DEFAULT );
			if(SCharacterData.nCharacterData[SPELL] == 0)
			{
				Hprint( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"WIZARD");
			}
			else if(SCharacterData.nCharacterData[SPELL] == 1)
			{
				Hprint( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"PRIEST");
			}
		}break;
		 
	case DP_TACTICS :	//택틱스 출력
		{
			Hcolor( FONT_COLOR_DEFAULT );
			if(SCharacterData.nCharacterData[GENDER]==0) 
			{
				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_TACTICS_MAN_TEXT][SCharacterData.nCharacterData[TACTICS_MAN]].temp  );
			}
			else	
			{
				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_TACTICS_WOMAN_TEXT][SCharacterData.nCharacterData[TACTICS_WOMAN]].temp  );
			}
		}break;
		 
	case DP_JOB :		//직업 출력
		{
			Hcolor( FONT_COLOR_DEFAULT );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_JOB_TEXT][SCharacterData.nCharacterData[JOB]-20].temp  );
		}break;
	case DP_ABILITY : 	
		{
			AbilityOpenWhenLevelUp();		// 레벨업에 관련하여... 초기화
			const int ability = nWillDo; 
			const int basic = SCharacterData.nCharacterAbility[ability]; //+SCharacterData.EachUp[ability];	// eachup은 레벨업 포인트를 뺄때만 참조
			const int plus = Hero_Ability[ability];
		//	SetHangulAlign( TA_RIGHT );
			if (basic == g_mgrDual.GetAbility(ability)) 
			{
				Hcolor(FONT_COLOR_MAX);
			}//020707 lsw
		//	Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d", basic );

			if( plus > basic ) 
			{
				Hcolor( FONT_COLOR_SPECIAL );
				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d", plus );
			}
			else if( plus < basic )
			{
				Hcolor( FONT_COLOR_PLUS );
				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d", plus );
			}
			else
			{
				Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%d", plus );
			}

		}break;
						
	case DP_RESIST : 
		{
			for( int a=0; a<MAX_RESIST_TYPE; a++)
			{
				SetHangulAlign( TA_RIGHT );
				::Hprint2( iMainX+iFieldX+31, iMainY+iFieldY+a*25, g_DestBackBuf,"%d", 
							SCharacterData.naBasicResist[a]+ 
							SCharacterData.naExtentionResist[a]);
			}
		 }break;

	case DP_WEIGHT	:
		{
			Hcolor( FONT_COLOR_SPECIAL );
			if(SubMenu->Id == MN_ITEM)
			{
				if( !Hero_Ability[STR] )		// 있을 수 없다!
				{
					CallOkCancelMessageBox( SubMenu->Id, 0,0,lan->OutputMessage(6,63), 0 );//010215 lsw
					//CallServer(CMD_RESEND_CHARLIST);
					break;
				}
				float rate;	// 내가 들수 있는 무게와 아이템 총 무게와의 비
				int max = Hero_Ability[STR] * 100;		// 들수 있는 무게 맥스
				/*2000 + STR* 35 + 레벨* 20*/
				rate =   SCharacterData.nWeight / (float)max * 100.0f;
				if( rate > 150.0f ) rate = 150.0f;
				if( rate < 0 ) rate = 150.0f;
				float width_rate = 80 / 150.0f;	
				if( rate != 150 )
				{
					for( int start=(int)(rate*width_rate); start<80; start++ )		// rate*width_rate =>실제 거리
					{
						if( rate < 100 ) 
								FieldTypeNomalPut(iMainX+start, iMainY, iFieldX, iFieldY, nRcImg );
						else	FieldTypeNomalPut(iMainX+start, iMainY, iFieldX, iFieldY, SubMenu->nField[j].nSHideNomalCount );
					}
				}
				SetHangulAlign( TA_RIGHT );//020720 lsw
#ifdef _DEBUG // finito 060507
				if( GetSysInfo( SI_GAME_MAKE_MODE ) )
				{
					Hprint2( 50, 100, g_DestBackBuf,"<%d>", SCharacterData.nWeight );
					Hprint2( 50, 120, g_DestBackBuf,"<%d>", (int)rate );
				}
#endif
				FieldTypeNomalPut(iMainX, iMainY, 124+20, 35+3, 31, MAIN_ETC );		// 갈과 활 그림
				FieldTypeNomalPut(iMainX, iMainY, 124+144, 35+100, 32, MAIN_ETC );		// 방패와 화살 이미지
			}
			else if(SubMenu->Id == MN_BANK_CUSTODY)	
			{
				int weight;
				weight = GetBankItemWeight();
				SetHangulAlign( TA_RIGHT );//020720 lsw
				Hprint2( iMainX+iFieldX+142, iMainY+iFieldY, g_DestBackBuf,"%d", weight);
			}
		}break;

	case DP_ARMOR_INVENTORY :	
		{	
			nWeapon_Type = nWillDo; 
			if(EquipItemAttr[nWeapon_Type].item_no)
			{
				PutItemIcon( iMainX+iFieldX+15 , iMainY+iFieldY+15, EquipItemAttr[nWeapon_Type].item_no );
			}
			else if( EquipItemAttr[WT_WEAPON].item_no													// 무기를 들고 있는데
				&& (nWeapon_Type == WT_SHIELD)															// 방패자리에 아무것도 없고
				&& ( GetItemAttr( EquipItemAttr[WT_WEAPON].item_no, WEAR_ABLE ) == WEAR_TWO_HAND )	)	// 그 무기기가 양손검인데
			{
				int kind = GetItemAttr( EquipItemAttr[WT_WEAPON].item_no, ITEM_KIND );
				if( kind == IK_SHORT_BOW || kind == IK_LONG_BOW )
				{
					static int ct;
					if( ct%10 ) 
					{
						PutItemIcon( iMainX+iFieldX+15 , iMainY+iFieldY+15, 7005, 0, 2, 1 );
					}
					ct++;
				}
				else PutItemIcon( iMainX+iFieldX+15 , iMainY+iFieldY+15, EquipItemAttr[WT_WEAPON].item_no, 8, 2 );
			}
		}break;

	case DP_SELECT_ITEM	:	
		{
			if( nWillDo == ITEM_BUY )
			{
				if( SubMenu->work )
				{
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, SubMenu->work );
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, SubMenu->work,8,2 );
					Hcolor( FONT_COLOR_NAME );
					SetHangulAlign( TA_CENTER );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY+10, g_DestBackBuf, "%s", GetItemName( SubMenu->work ) );
					ItemExplain( SubMenu->work );
					
					int t = GetItemAttr(SubMenu->work, VALUE);		// 가격이 처음 0인것을 막기 위해...
					if( t )
					{
						SetHangulAlign( TA_RIGHT ); 
						Hcolor( FONT_COLOR_NUMBER );
						int price = GetItemAttr(SubMenu->work, VALUE);
//															if( GetItemAttr( SubMenu->work, DO_RBUTTON ) == DIVIDE_ITEM )
//																price *= GetItemAttr( SubMenu->work, DURATION );
						Hprint2( iMainX+91+57, iMainY+231, g_DestBackBuf,"%d" , price );
					}
				}
			}
			else if( nWillDo == ITEM_SELL )
			{
				if( !SubMenu->CheakType )		// 처음 그 메뉴가 불러 졌을 경우
				{
					SubMenu->work = -1;
					SubMenu->CheakType = 1;
					break;
				}
				if(SubMenu->work == -1 ) break;
				int selelct = SubMenu->work;
				int item = aSellBuyItemAttr[selelct].item_no;
				int item_pos = aSellBuyItemPos[selelct];		// 인벤토리에서의 아이템 위치 
//													int item_pos = SubMenu->work;
				int i_a = (item_pos & 0x00000060) >> 5 ;
				int i_b = (item_pos & 0x00000018) >> 3 ;
				int i_c = (item_pos & 0x00000007) ;
				ItemAttr t = InvItemAttr[i_a][i_b][i_c];
				if( item )
				{
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, item );
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, item,8,2 );
					Hcolor( FONT_COLOR_NAME );
					SetHangulAlign( TA_CENTER );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY+10, g_DestBackBuf, "%s", GetItemName( item ) );
					ItemExplain(  t );
					SetHangulAlign( TA_RIGHT ); 
					DWORD price = GetItemValueSell( t );
//														if( price == 0 ) break;		// 돈 출력을 막는다.
					Hcolor( FONT_COLOR_NUMBER );
					Hprint2( iMainX+91+57, iMainY+231, g_DestBackBuf,"%d", price );
				}
			}
			else if( nWillDo == ITEM_REPAIR )
			{
				if( !SubMenu->CheakType )		// 처음 그 메뉴가 불러 졌을 경우
				{
					SubMenu->work = -1;
					SubMenu->CheakType = 1;
					break;
				}
				if(SubMenu->work == -1 ) break;
				int select = SubMenu->work;
				int item = aSellBuyItemAttr[select].item_no;		
				int item_pos = aSellBuyItemPos[select];		// 인벤토리에서의 아이템 위치 
				int i_a = (item_pos & 0x00000060) >> 5 ;
				int i_b = (item_pos & 0x00000018) >> 3 ;
				int i_c = (item_pos & 0x00000007) ;
				ItemAttr t = InvItemAttr[i_a][i_b][i_c];

				if( item )
				{
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, item );
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, item,8,2 );
					Hcolor( FONT_COLOR_NAME );
					SetHangulAlign( TA_CENTER );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY, g_DestBackBuf, "%s", GetItemName( item ) );

					Hcolor( FONT_COLOR_NUMBER );
					WORD d_max, d_curr; 
					GetItemDuration( t, d_curr, d_max );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY+15, g_DestBackBuf, "%d / %d", d_curr/10, d_max/10 );

					int price=1;
					if( d_max )
					{
						//price = (int)( GetItemAttr(t.item_no, VALUE)/d_max*(d_max - d_curr)*DUR_RATIO +.5 );
						price = GetItemAttr(t.item_no, VALUE);
						price = (int)( (float)price/d_max*(d_max - d_curr)*DUR_RATIO +.5 );//;+((RareMain*)(&t.attr[IATTR_RARE_MAIN]))->grade*1000000;//020505 lsw

						if(t.attr[IATTR_RARE_MAIN])
						{	
							const int iRepairPoint = (int)( (float)((d_max - d_curr))*RARE_DUR_RATIO +.5); // 가격 계산 및 판단 
							const int iRareGrade  = ((LPRareMain)(&t.attr[IATTR_RARE_MAIN]))->grade;
							price += (iRepairPoint/2) * ((iRareGrade*iRareGrade)+10) * 100	; // 가격 계산 및 판단 //020815 lsw
						}
						if( !price ) price = 1;
					}
					SetHangulAlign( TA_RIGHT ); //020705 lsw
					Hprint2( iMainX+91+57, iMainY+231, g_DestBackBuf,"%d", price );
					ItemExplain( t );
				}
			}
			else if( nWillDo == ITEM_BUY_MALL )
			{
				if( SubMenu->work )
				{
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, SubMenu->work );
					PutItemIcon( iMainX+iFieldX+15, iMainY+iFieldY+15, SubMenu->work,8,2 );
					Hcolor( FONT_COLOR_NAME );
					SetHangulAlign( TA_CENTER );
					Hprint2( iMainX+iFieldX+95, iMainY+iFieldY+10, g_DestBackBuf, "%s", GetItemName( SubMenu->work ) );
					ItemExplain( SubMenu->work );
					
					int t = GetItemAttr(SubMenu->work, RAJA_EMBLEM);		// 가격이 처음 0인것을 막기 위해...
					if( t )
					{
						SetHangulAlign( TA_RIGHT ); 
						Hcolor( FONT_COLOR_NUMBER );
						int price = GetItemAttr(SubMenu->work, RAJA_EMBLEM);
//															if( GetItemAttr( SubMenu->work, DO_RBUTTON ) == DIVIDE_ITEM )
//																price *= GetItemAttr( SubMenu->work, DURATION );
						Hprint2( iMainX+148, iMainY+326, g_DestBackBuf,"%d" , price );
					}
				}
			}
	}break;

	case DP_ARIGEMENT_BAR : 
		{
			MP3( SN_MAGIC_BALL );
			switch(SCharacterData.nCharacterData[ARIGEMENT])
			{
				case 0 :
				case 1 :	FieldTypeNomalPut(iMainX, iMainY, 157, 178, 310); //막대기의 이미지 번호는 310
				case 2 :	FieldTypeNomalPut(iMainX, iMainY, 157, 200, 310); //막대기의 이미지 번호는 310
				case 3 :	FieldTypeNomalPut(iMainX, iMainY, 157, 221, 310); //막대기의 이미지 번호는 310
				case 4 :
				case 5 :	FieldTypeNomalPut(iMainX, iMainY, 157, 250, 310); //막대기의 이미지 번호는 310
				case 6 :	FieldTypeNomalPut(iMainX, iMainY, 157, 272, 310); //막대기의 이미지 번호는 310
				case 7 :	FieldTypeNomalPut(iMainX, iMainY, 157, 295, 310); //막대기의 이미지 번호는 310
				case 8 :break;
			}
		}break;

	case DP_GUILD :		
		{
			Hcolor( FONT_COLOR_DEFAULT );
			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf,"%s",SCharacterData.nGuild);
		}break;
	case DP_MONEY :		
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.nMoney);
		}break;
	case DP_FAME :		
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.nCharacterAbility[FAME]);
			break;
			//						case DP_WSPS :		if(flag) { ability=WSPS; flag=FALSE; }
			if(flag) { ability=AC; flag=FALSE; }
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", Hero_Ability[ability]);
		}break;
	case DP_TACTICE :	
		{
			SetHangulAlign( TA_RIGHT ); 
			if( SCharacterData.tatics != -1 )
			{
				Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.SkillId[SCharacterData.tatics]+1 );
			}
			else 
			{
				Hprint2( iMainX+iFieldX+58, iMainY+iFieldY, g_DestBackBuf,"%d", 0 );
			}
		}break;
	case DP_HEAT :	
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+57, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.beat); 
		}break;
	case DP_AC	:
		{
			SetHangulAlign( TA_RIGHT ); 
			Hprint2( iMainX+iFieldX+57, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.ac	); 
			break;
		}
	case DP_MOV	:		
		{
			SetHangulAlign( TA_RIGHT );
			Hprint2( iMainX+iFieldX+57, iMainY+iFieldY, g_DestBackBuf,"%d", SCharacterData.mov); 
		}break;

	case DP_MONEY2 :	
		{
			PrintMoney( SubMenu, j, (DWORD)(*SubMenu->nField[j].nValue)+nBankTemp, nWillDo );
		}break;
	case DP_SKILL_ICON :
		{
			for(int a=0;a<2;a++) 
			{
				for(int b=0;b<4;b++)  
				{                          
					if(SkillInventory[nWillDo][a][b])
					{
						PutSkillIcon(iMainX+iFieldX+17+41*b , iMainY+iFieldY+17+a*36, SkillInventory[nWillDo][a][b]);
					}
/*														else if( SkillInventory2[nWillDo][a][b] )
					{
						PutSkillIcon(iMainX+iFieldX+17+41*b , iMainY+iFieldY+17+a*36, SkillInventory2[nWillDo][a][b], 15, 1);
					}*/
				}
			}
		}break;

	case DP_BANK_CURRENT_PAY :	
		{
			PrintMoney(	SubMenu, j, GetCurrentBankItemPay(), 124 ); 
			break;
		}
	case DP_BANK_PAY :		
		{
			PrintMoney(	SubMenu, j, GetCurrentBankItemPay(), 124 ); 
		}break;
	case DP_LOAN_TOTAL :	
		{
			PrintMoney(	SubMenu, j, SCharacterData.LoanTotal, 105 ); 
		}break;
	case DP_LOAN_ABLE : 	
		{
			if( SCharacterData.LoanTotal > SCharacterData.LastLoan ) 
			{
				PrintMoney( SubMenu, j, SCharacterData.LoanTotal - SCharacterData.LastLoan, 105 );
			}
			else 
			{
				PrintMoney( SubMenu, j, 0, 105 ); 
			}
		}break;
	case DP_BANK_SAVE :		
		{
			PrintMoney( SubMenu, j, SCharacterData.BankMoney, 153 ); 
		}break;

	case DP_LAST_LOAN:		
		{
			PrintMoney(	SubMenu, j, SCharacterData.LastLoan, 105 ); break;
		}
		
	case DP_TEMP_MONEY :
		{
			if( !SubMenu->CheakType )
			{
				SubMenu->CheakType = 1;
				SetChatMode(CM_COMMAND);
				EWndMgr.ClearAllTxt();//021001 lsw
				SetFocus2( HWND_3 );//021001 lsw
			}
			DWORD max = 1000000000;		
			switch( SubMenu->Id )
			{
				case MN_BANK_DEPOSIT	: 
					{
						if( SCharacterData.BankMoney > 4000000000 )
						{
							max = 0;
						}
						else
						{
							max = __min( 4000000000 - SCharacterData.BankMoney, SCharacterData.nMoney );
						}
						break;
					}
				case MN_BANK_DEFRAYAL	: max = SCharacterData.BankMoney; break;
				//case MN_BANK_LOAN		: max = MyBank.GetLoanAble(); break;
				case MN_BANK_REPAYMENT	: 
										if( SCharacterData.LastLoan > SCharacterData.nMoney)
											max = SCharacterData.LastLoan;
										else
											max = SCharacterData.nMoney;
										break;
			}
			//021001 lsw	
			char temp[20] = {0,};
			SetFocus2(HWND_3);
			EWndMgr.GetTxt(HWND_3,temp,20);
			EatRearWhiteChar( temp );
			nBankTemp = atol( temp );
			int refresh = 0;
			if( nBankTemp<0)
			{
				nBankTemp = 0;
				refresh = 1;
			}
			if( (DWORD)nBankTemp>max )		//// 020925 YGI
			{
				nBankTemp = max;
				refresh = 1;
			}

			if( refresh || CheckNumber( temp, strlen( temp ) ) )
			{	
				sprintf( temp, "%u", nBankTemp );
				EWndMgr.SetTxt(HWND_3, temp);//021001 lsw //윈도우에 텍스트를 셋팅 합니다.
			}
			PrintMoney( SubMenu, j, nBankTemp, 153 ); 
		}break;
	case DP_BANK_TOTAL :
		{
			if(SubMenu->Id == MN_BANK_DEPOSIT)
			{
				PrintMoney( SubMenu, j, SCharacterData.BankMoney+nBankTemp, 153 );
			}
			else 
			{
				PrintMoney( SubMenu, j, SCharacterData.BankMoney-nBankTemp, 153 );
			}
			break;
		}
	case DP_DAY_TEXT :	
		{
			PrintDate( SubMenu, j, g_GameInfo.timeGame, 100 ); 
		}break;
	case DP_CREDIT_TEXT :
		{
			SetHangulFont( 4 );
			Hcolor( RGB16(255, 255, 255) );
			SetHangulAlign( TA_CENTER ); 
			int lenght = 50;
			Spr *s = GetSprOfMenu( SubMenu->nImageType, SubMenu->nImageNumber );
			if( s ) lenght= s->xl / 2;
			Hprint2( iMainX+lenght, iMainY+iFieldY, g_DestBackBuf, "Believable" );
			SetHangulFont( 2 );
		 }break;

	case DP_REPAYMENT_MONEY :	
		{
			PrintMoney(	SubMenu, j, SCharacterData.LastLoan, 105 ); 
		}break;
	case DP_REPAYMENT_DATE :	
		{
			PrintDate(SubMenu, j, SCharacterData.LastLoan_Time, 100); 
		}break;
	case DP_CHARACTER_MONEY :	
		{
			PrintMoney(	SubMenu, j, SCharacterData.nMoney, nWillDo); 
		}break;  //캐릭터가 가지고 있는 돈을 찍는다. 공통으로 쓰임
	case DP_CHARACTER_RAJA_EMBLEM :	
		{
			PrintMoney(	SubMenu, j, GetItemMuchByMuchItem(10057), nWillDo); 
		}break;
	case DP_AUCTION_FK_RARE://상인메뉴에서 검색기에 따른 한글 출력
		{//021014 lsw
//			const int iValue = Auction.GetFKRareType();
//			int iType = 2, iNum = 0;
//			switch(iValue)
//			{
//			case NORMAL_ITEM:	{iNum = 711; }break;
//			case RARE_ITEM	:	{iNum = 712; }break;
//			case HIGH_ITEM	:	{iNum = 713; }break;
//			case DYNAMIC_RARE_ITEM:	{iNum = 714; }break;
//			case SET_ITEM	:	{iNum = 715; }break;
//			default:{return;}break;
//			}
//			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, lan->OutputMessage(iType, iNum ));
		}break;
	case DP_AUCTION_FK_LV:
		{
//			const int iValue = Auction.GetFKLevel();
//			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%d~%d", iValue*10, (iValue+1)*10);
		}break;
	case DP_AUCTION_FK_TACTIC:
		{
//			const int iValue = Auction.GetFKTacticType();
//			Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, ::GetTacticName(iValue) );
		}break;
	case DP_AUCTION_FK_WEAR:
		{
//			const int iValue = Auction.GetFKWearType();
//			int iType = 2, iNum = 0;
//			switch(iValue)
//			{
//			case WEAR_ANYTHING	:{iNum = 720;}break;
//			case WEAR_ONLY_INVEN:{iNum = 721;}break;
//			case WEAR_RIGHT_HAND:{iNum = 722;}break;
//			case WEAR_LEFT_HAND	:{iNum = 723;}break;
//			case WEAR_HEAD		:{iNum = 724;}break;
//			case WEAR_BODY		:{iNum = 725;}break;
//			case WEAR_NECK		:{iNum = 726;}break;
//			case WEAR_SHOES		:{iNum = 727;}break;
//			case WEAR_UNIQUE	:{iNum = 728;}break;
//			case WEAR_BELT		:{iNum = 729;}break;
//			case WEAR_TWO_HAND	:{iNum = 730;}break;
//			default:{return;}break;
//			}
//			::Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, lan->OutputMessage(iType, iNum));
		}break;
	case DP_BETTING_MONEY:
		{
			DWORD dwMoney = ::GetTeamBettingMoney(nWillDo);
			::PrintMoney(SubMenu,j,dwMoney,130);
		}break;
	case DP_ARENA_TEAM_WIN_RATE:
		{	//< CSD-030520
			const float fWinRate = ::GetTeamWinRate(nWillDo);
			::Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%3.1f%%",fWinRate);
			break;
		}	//> CSD-030520
	case DP_MY_BETTING_MONEY:
		{
			switch(nWillDo)
			{
			case 0://배팅금
				{
					DWORD dwMoney = ::GetMyBettingMoney();
					::PrintMoney(SubMenu,j,dwMoney,130);
				}break;
			case 1://배당금
				{
					DWORD dwMoney = ::GetMyBettingResult();
					::PrintMoney(SubMenu,j,dwMoney,130);
				}break;
			}
		}break;
	case DP_ARENA_WAIT_TIME:
		{
			::SetHangulAlign(TA_CENTER);
			const DWORD dwTime = ::GetAreanWaitTime();
			::Hprint2( iMainX+iFieldX, iMainY+iFieldY, g_DestBackBuf, "%d",dwTime );
		}break;
	case DP_TEMP_MAIL_ICON:
		{
			const int iMailType = g_MailMgr.GetTempMailType();
			const int iMailIconNo = g_MailMgr.GetMailIconNo(iMailType);
			::FieldTypeNomalPut(iMainX+iFieldX,iMainY+iFieldY,0,0,iMailIconNo,iImgType);
			
			if( ::BoxAndDotCrash( iMainX+iFieldX,iMainY+iFieldY, 20, 20, g_pointMouseX, g_pointMouseY ) )
			{
				const int iX = iMainX+iFieldX+30, iY = iMainY+iFieldY;
				::DisplayMailIconExplain(iX,iY,iMailType);
			}
		}break;
	case DP_NOW_VIEW_MAIL_PAGE:
		{
			::Hcolor(FONT_COLOR_WHITE);
			::SetHangulAlign(TA_CENTER);
			TxtOut.RcTXTOutNoBG(iMainX+iFieldX,iMainY+iFieldY,150,1,"%d/5",g_MailMgr.GetNowViewPageNo()+1 );
		}break;
	}
	return;
}

bool DoButtonCheckOfMenu_lsw( int i, int j )
{
	SMENU *SubMenu = &SMenu[i];

	int &nWillDo= SubMenu->nField[j].nWillDo;
	int &iSwd = SubMenu->nField[j].nSpecialWillDo;

	int &work	= SubMenu->work;

	int &iMainX = SubMenu->x;
	int &iMainY = SubMenu->y;
	
	int &iFieldX = SubMenu->nField[j].x;
	int &iFieldY = SubMenu->nField[j].y;
	
	switch(SubMenu->nField[j].nType)
	{
	case	FT_RARE_GROUP:
		{	//
			const int iRareGroup = work + nWillDo;

			if( 0 > iRareGroup || MAX_RARE_GROUP <= iRareGroup)
			{
				break;
			}
			
			SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[0].nWillDo=iRareGroup;//레어 그룹 값

			const int iIsDynamicRare = SMenu[MN_ITEM_MAKE_RARE_SELECT].key;
			for ( int count = 0; count < 8 ; count++)
			{
				if(!iIsDynamicRare)//스태틱 레어
				{
					int tempRareNum = RareGroup[iIsDynamicRare][iRareGroup].rareset[count].rare_num;
					if(  tempRareNum )
					{
						strcpy(SubMenu->nField[count+16].temp,ItemRare[tempRareNum].LocalName);//020505 lsw
						SubMenu->nField[count+16].nImageNumber = tempRareNum;
					}
					else
					{
						strcpy(SubMenu->nField[count+16].temp," ");
						SubMenu->nField[count+16].nImageNumber = 0;
					}
				}
				else
				{
					int tempRareNum = RareGroup[iIsDynamicRare][iRareGroup].rareset[count].rare_num;
					if(  tempRareNum )
					{
						strcpy(SubMenu->nField[count+16].temp,ItemFunction[tempRareNum].NameAdd);//020505 lsw
						SubMenu->nField[count+16].nImageNumber = tempRareNum;
					}
					else
					{
						strcpy(SubMenu->nField[count+16].temp," ");
						SubMenu->nField[count+16].nImageNumber = 0;
					}
				}
				
			}
		}break;
  case	FT_RARE_GROUP_SCROLL://startpoint 값을 주어서 스크롤
		{
			const int UpDown = nWillDo;
			if( UpDown == 1)//DOWN 수정중
			{//011025 lsw >// 1 부터 시작 하고 	//0 번 라인 안나오니까 -1 한번 더 해주고
				const int iRareGroup = work + SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[6].nWillDo;
				const int iIsDynamicRare = SMenu[MN_ITEM_MAKE_RARE_SELECT].key;

				if(0 > iRareGroup || MAX_RARE_GROUP-1 <= iRareGroup){break;}//MAX_RARE_GROUP-1 배열의 가능한 마지막 부분

				if(!RareGroup[iIsDynamicRare][iRareGroup+4].name[0])
				{	
					break;
				}
			}
			else if(UpDown == -1 )//UP
			{
				if( SMenu[MN_ITEM_MAKE_RARE_SELECT].work <=0 )
				{
					SMenu[MN_ITEM_MAKE_RARE_SELECT].work =0;
					break;
				}
			}
			SMenu[MN_ITEM_MAKE_RARE_SELECT].work += UpDown;//이게 스크롤때문에 보여지는 시작점
		}break;
	//011013 lsw >
	case FT_MAKE_RARE_MORE:
		{	
			int join_no = item_select_result.join_no;		// 만들고자 하는 아이템이 몇번째 아이템인가...
			
			CItem  *rit1;
			CItem  *rit2;
			CItem  *rit3;
			CItem  *rit4;

			rit1 = ItemUnit(item_join[join_no].resource1);
			rit2 = ItemUnit(item_join[join_no].resource2);
			rit3 = ItemUnit(item_join[join_no].resource3);
			rit4 = ItemUnit(item_join[join_no].resource4);
			
			int r1amount= item_join[join_no].resource1_amount;	
			int r2amount= item_join[join_no].resource2_amount;	
			int r3amount= item_join[join_no].resource3_amount;	
			int r4amount= item_join[join_no].resource4_amount;	

			int r1count=0;
			int r2count=0;
			int r3count=0;
			int r4count=0;
			
			int r1= -1;
			int r2= -1;
			int r3= -1;
			int r4= -1;
			int *MakeMuch = 0;

			MakeMuch = &skillmakeiteminfo.MakeMuch;
			//011030 lsw >
			GetSkillResorceAmount(r1count,r2count,r3count,r4count);
			//011030 lsw <
			if( r1amount ) 				{r1 =r1count /r1amount;}
			if( r2amount ) 				{r2 =r2count /r2amount;}
			if( r3amount ) 				{r3 =r3count /r3amount;}
			if( r4amount ) 				{r4 =r4count /r4amount;}
			
			if(r1 != -1)				{	*MakeMuch = r1;	}
			if(r2 != -1)				{	*MakeMuch = __min(*MakeMuch,r2);}
			if(r3 != -1)				{	*MakeMuch = __min(*MakeMuch,r3);}
			if(r4 != -1)				{	*MakeMuch = __min(*MakeMuch,r4);}

			int AddValue = nWillDo;

			int *now = &SMenu[MN_ITEM_MAKE_SELECT].nField[10].nWillDo;
			
			if( AddValue )
			{
				*now += AddValue;
				if( *now < 0 )
				{	*now = 0;	}
				if(*MakeMuch < *now)
				{	*now= *MakeMuch;	}
			}
			else
			{
				*now= *MakeMuch;
			}
			*MakeMuch = *now;
			//011022 lsw > 임시로 10개이상 불가능으로 만듬
			if(*MakeMuch > 10 )
			{
				//011030 lsw >
				AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,501));
				//011030 lsw <
				*MakeMuch = 10;
				*now = 10;
			}
			//011022 lsw >
			sprintf(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,"%d ",*MakeMuch);
		}break;
	case 	FT_CS_ICON:
		{
			switch(iSwd)
			{
			case SWD_SC_ICON_POINT:
				{
					SetCSPIconImageANDnWillDo(work,SubMenu->nField[3].nWillDo);//Done버튼에 의해 호출 되었다면 강제로 셋팅을 해야 하기 때문이다
					SubMenu->nField[j].nImageNumber = SubMenu->nField[j].nRectImage;//선택된 이미지 뿌린다 
					SubMenu->nField[15].nWillDo = nWillDo;//어빌리티 업 버튼에 스킬 번호 준다
#ifdef _DEBUG
					if( GetSysInfo(SI_GAME_MAKE_MODE ) )	// 020925 YGI
					AddCurrentStatusMessage(0,255,255,"전투스킬 포인트 아이콘에 클릭 하셨습니다");
#endif
				}break;
			}
		}break;

	case FT_CSD_ICON:
		{
			switch(iSwd)
			{
			case SWD_SC_ICON_DOING:
				{
					if (nWillDo == 0)
					{
						CSDMainIconClear();
					}
					const int nSkill = nWillDo + 214;
					
					if (GetCombat()->IsEnable(nSkill))
					{	//< CSD-TW-030606
						g_mgrBattle.SendCombatSelect(nSkill);
					}	//> CSD-TW-030606
#ifdef _DEBUG
					if( GetSysInfo(SI_GAME_MAKE_MODE ) )	// 020925 YGI
					AddCurrentStatusMessage(0,255,255,"전투스킬 두잉 아이콘에 클릭 하셨습니다");
#endif
				}break;
			}
		}break;

	case 	FT_CSD_MAIN_ICON:
		{
#ifdef _DEBUG
			if( GetSysInfo(SI_GAME_MAKE_MODE ) )	// 020925 YGI
			AddCurrentStatusMessage(0,255,255,"전투스킬 두잉 메인 버튼에 좌클릭 하셨습니다");
#endif
			InitCombatSkill();
			SetCSDIconRectAndnWillDo();
		}break;
	case FT_CSP_POINT_UP:
		{
			SendCSLvUp(nWillDo);
		}break;
	case FT_PEACE_BATTLE_PUT :
		{
			if (Hero->nCurrentAction!=17)	// LTS SITDOWN				// 앉은 상태 기본 평화상태 
			{
				switch(GetBattlePeace())
				{
				case PEACE_MODE://평화모드라면
					{
						SetBattlePeace( BATTLE_MODE );//전투 모드로 바꿔주고
						AddCurrentStatusMessage(FONT_COLOR_RED, lan->OutputMessage(0,2) );//전투모드로 전환합니다.
					}break;
				case BATTLE_MODE://전투모드라면
					{
						MP3(SN_FIGHT );
						SetBattlePeace( PK_MODE );//PK모드로 바꿔 주고
					//	AddCurrentStatusMessage( 255,255,255, kein_GetMenuString(96) );	의자에 앉으면 안되는 기능입니다.
					}break;
				case PK_MODE://피케이 모드라면 
					{
						MP3(SN_FIGHT );
						SetBattlePeace( PEACE_MODE );//평화모드로 바꿔주자
						AddCurrentStatusMessage(FONT_COLOR_WHITE, lan->OutputMessage(0,1) );//평화모드로 전환합니다.
					}break;
				default:
					{
					}break;
				}
			}
		}break;
	case FT_SELECT_WISPER_TARGET:
		{
			bool bOn = SMenu[MN_WISPER_LIST].bActive;//열려 있었다면 
			CallWisperList(!bOn);//닫고 
			if(bOn)//닫히는 순간 . 즉 아까 열려 있었으니까 
			{
				SetChatTarget(CHAT_TARGET_NORMAL);
			}
		}break;
	case FT_SELECT_WISPER:
		{
			switch(iSwd)
			{
			case CHATTYPE_NORMAL	:
				{
					SetChatTarget(nWillDo);
					SetFocus2(HWND_CHAT_INPUT);//021001 lsw
				}break;
			case CHATTYPE_GUILD		:
				{
					SetChatTarget(nWillDo);
					SetFocus2(HWND_CHAT_INPUT);//021001 lsw
				}break;
			case CHATTYPE_PARTY		:
				{
					SetChatTarget(nWillDo);
					SetFocus2(HWND_CHAT_INPUT);//021001 lsw
				}break;
			case CHATTYPE_WHISPER	:
				{
					if(IsAbleChatTargetName(nWillDo))//이름이 있다면//없으면 선택 되어서는 안된다(아무일도 하지 않는 버튼이다)
					{	//이름이 있다면 이름을 가져다 주고 
						SetChatTarget(nWillDo);
						if(GetChatTarget() == SELECT_WISPER_MODE)//새이름 선택
						{
							SetChatTarget(SELECT_WISPER_MODE);
						}
						else//새이름 선택이 아니라면
						{
							EWndMgr.SetTxt( HWND_CHAT_TARGET_INPUT, SubMenu->nField[j].temp );//021001 lsw
							SetFocus2(HWND_CHAT_INPUT);//021001 lsw
						}
					}
					else
					{
						work = 0;	//노말 채팅
						SetFocus2(HWND_CHAT_INPUT);//021001 lsw
					}
				}break;
			}
			CallWisperList(false);//닫기및 셋팅
		}break;
	case FT_CHAT_LOCK:
		{
			if(!nWillDo)
			{
				nWillDo = 1;
				AddCurrentStatusMessage(FONT_COLOR_BLIGHT_GREEN2,lan->OutputMessage(0,609));//020705 lsw
			}
			else
			{
				nWillDo = 0;
				AddCurrentStatusMessage(FONT_COLOR_BLIGHT_GREEN,lan->OutputMessage(0,610));//020705 lsw
			}			
		}break;
	case FT_SELECT_CHAT_TYPE:
		{//nWillDo를 선택
			switch(GetChatTarget())
			{
			case 2:
				{
					SetChatTarget(CHAT_TARGET_NORMAL);
				}break;
			case 1: //MN_WISPER_LIST에 따라서 찎히는게 다르다
				{
					SetChatTarget(CHAT_TARGET_PARTY);//보통 채팅 으로 전환
				}break;
			case 0:
				{
					SetChatTarget(CHAT_TARGET_GUILD);//길드 채팅으로 전환
				}break;

			}
		}break;
	case	FT_CHAT_VIEW_SET:
		{
			const int iChatType = iSwd;
			const int iOn = ChatMgr.GetViewChat(iChatType);
			ChatMgr.SetViewChat(iChatType,!iOn);//021001 lsw
		}break;
	case	FT_DRAG_ZONE:
		{	
			if(nWillDo)
			{
				nWillDo = 0;
			}
			else
			{
				nWillDo = 1;
			}
		}break;

	case FT_HOTKEY_ICON://020701 lsw
		{
			const int iNo = HotKeyMemory.iNo;
			if(!iNo || strcmp(Hero->name,HotKeyMemory.OwnerName)) //자기 이름 아니면 실행 안된다
			{
				break;
			}

			switch(HotKeyMemory.iType)
			{
			case QM_MAGIC:
				{
					const int nMagic = g_mgrBattle.Convert(iNo);
					if (GetMagic()->IsEnable(nMagic))
					{	//< CSD-TW-030606
						RectImageOn = true; 
						MagicOnFlag.SetMagicOn(iNo, iMainX+iFieldX+15, iMainY+iFieldY+15, 0);
						g_mgrBattle.SendMagicSelect(iNo);
					}	//> CSD-TW-030606
				}break;
			case QM_SKILL:
				{
					if( !iNo || !skill[iNo].num) break;
					MP3( SN_SKILL );

					if( SelectSkill(iNo)==1 ) //스킬이 사용 중이라면
					{	// 스킬 취소		KHS
						SkillSettingClear( );
						break;
					}
					else 									
					{
						SkillOn = 1;
						SelectSkill( iNo );
						y_MenuFocus = 0;
						CanMakeItemListSet( iNo );
					}
				}break;
			case QM_COMBATSKILL:
				{
				}break;
			}
		}break;					  
	case FT_MERCHANT_RESULT_ITEM_CHECK: 
		{
			if(nWillDo)//받아 와야지
			{
				if(nWillDo == Auction.GetTakeItemIndex())
				{
					Auction.SetTakeItemIndex(0);
				}
				else
				{
					Auction.SetTakeItemIndex(nWillDo);
				}
			}
		}break;
	case FT_MERCHANT_DIRECT_BUY_ITEM_CHECK:
	case FT_MERCHANT_BUY_ITEM_CHECK:
		{
			if(nWillDo)
			{
				if(nWillDo == Auction.GetBuyItemIndex() )//같은거 또 눌렸냐 ㅡㅡ;.
				{
					Auction.SetBuyItemIndex(0);
				}
				else
				{
					Auction.SetBuyItemIndex(nWillDo);
				}
			}
		}break;
	case FT_MERCHANT_SELL_ITEM_CHECK:
		{
			if(nWillDo)
			{
				if(nWillDo == Auction.GetDeleteItemIndex())//같은거 또 눌렸냐 ㅡㅡ;.
				{
					Auction.SetDeleteItemIndex(0);
				}
				else
				{
					Auction.SetDeleteItemIndex(nWillDo);
				}
			}
		}break;
	case FT_EXCHANGE_ITEM_CHECK : 
		{
			if( nWillDo == RIGHT ) // Only Right Side
			{
				const bool bMySideOk	= ExchangeMgr.IsMySideConformStateOk();
				const bool bOtherSideOk	= ExchangeMgr.IsOtherSideConformStateOk();
				if(!bMySideOk)
				{
					ExchangeMgr.SetMySideConformState(true);// Change My side
					ExchangeMgr.SendExchangeStateSign(true);// Send Now State

					if( bMySideOk && bOtherSideOk )	// 거래 성립!
					{
						ExchangeMgr.SendExchangeResult();
					}
				}
				else//030108 lsw
				{
					ExchangeMgr.SetOtherSideConformState(false);
					ExchangeMgr.SetMySideConformState(false);// Change My side
					ExchangeMgr.SendExchangeStateSign(false);// Send Now State
				}
			}
		}break;
	case FT_MAN_TO_MAN_CHECK :
		{
			if( RIGHT == nWillDo && work == 1 )	// 누군가 결투를 신청했다.
			{
				// 결투 성립... 서버로 보낸다.
				::SendManToManStart( SubMenu->key );
				SubMenu->bActive = false;
			}
		}break;
	case FT_EXCHANGE_ITEM : 
		{
			if(!IsExchangeNow())
			{
				break;
			}
			if( nWillDo == RIGHT )	// 내가 준 아이템
			{
				if( g_MouseItemType == 1 )//아이템을 들고 있다면
				{
					CItem* i = ItemUnit(HandItemAttr.item_no); // finito 040707 check if item is noechange an therefore can't drop
					if (!i) break; 

					if (i->GetItemKind() == IK_NOEXCHANGE) 
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString(245) );
						break;
					}

					int a;
					for( a=0; a<MAX_EXCHANGE_ITEM; a++ )
					{
						if( !item_give[a].item.item_attr.item_no )
						{
							if( IP_base.type == EQUIP )
							{
								int a=0, b=0, c=0;
								if( ::SearchEmpty(a, b, c) )
								{
									::SetItemPos( INV, a, b, c, &IP_base );
								}
								else break;
							}
							else if( IP_base.type != QUICK && IP_base.type != INV )		// 그외의 위치에서의 아이템은 받아주질 않는다.
							{
								break;
							}

							::PutMouseItemCheck();		// 실제로 아이템을 되돌려 놓는다.
							::SetExchageItemData(a,*GetItemByPos(IP_base),IP_base,true);//021121 lsw
							break;
						}
					}
					if(a ==MAX_EXCHANGE_ITEM)
					{
						MP3( SN_WARNING );
					}
				}
				else
				{
					MP3( SN_WARNING );
				}
			}
		}break;
			//011004 lsw >
	case FT_GM_2ND_SKILL_MAKE_ITEM : 
		{//올려놓는 순서 //핸드에 아이템이 있는가?. -> 없는데마우스 업하면 리스트의 아템(포지션) 지움 
			int ct=0;
			if( g_MouseItemType != 1) 
			{
				for( int a=0; a<MAX_UPGRADE_RARE; a++ )
				{
					if( gm_make_item_list[a].item_no && MouseInRectCheak( SMenu[i].x, SMenu[i].y, gm_make_item_list[a].rect ) )
					{
						memset( &gm_make_item_list[a], 0, sizeof( MakeItem ) );
						break;
					}
				}
				break;
			}

			int blankpos  = -1;
			for( int a=0; a < MAX_UPGRADE_RARE ; a++ )
			{
				if( gm_make_item_list[a].item_no ) 
				{
					ct++;//들어있는 아이템 갯수
				}
				else
				{
					blankpos = a;
				}
			}//010708 lsw 카운트 세기
			
			if( ct>=MAX_UPGRADE_RARE) break; // 꽉 찼다. 

			//처음 들어 오면 처음에 들어 올수 있는 아이템인지 확인(아이템 넘버, 속성 있는가)후 추가
			if( IP_base.type != INV ) 
			{
				AddCurrentStatusMessage(FONT_COLOR_WHITE , lan->OutputMessage(2,530));//인벤토리 아템을 넣으 십쇼..
				break;
			}
			if( ct == 0 )//아무것도 없으면 대상을 지정 하고 0번에 넣는다
			{	 
				if( !CheckBy2ndGM_This_Item_Upgradeable(HandItemAttr,0))
				{
					break;//추가 불가능
				}
			}
			else
			{
				if(!CheckBy2ndGM_Insert_to_GMItemList(HandItemAttr,0))//처음 아이템이 아니면
					break;//추가 불가능
			}
			gm_make_item_list[blankpos].item_no = HandItemAttr.item_no;
			gm_make_item_list[blankpos].x = g_pointMouseX-SMenu[i].x;
			gm_make_item_list[blankpos].y = g_pointMouseY-SMenu[i].y;
			::SetRect( &gm_make_item_list[blankpos].rect, gm_make_item_list[blankpos].x-15,gm_make_item_list[blankpos].y-15,gm_make_item_list[blankpos].x+15,gm_make_item_list[blankpos].y+15 );
			gm_make_item_list[blankpos].attr = HandItemAttr;
			gm_make_item_list[blankpos].pos = IP_base;		// 인벤토리 내의 장소를 기억한다.
		}break;
	case FT_SKILL_MASTER_MAKE_ITEM://021111 lsw
		{//올려놓는 순서 //핸드에 아이템이 있는가?. -> 없는데마우스 업하면 리스트의 아템(포지션) 지움 
			int ct=0;
			if( g_MouseItemType != 1) 
			{
				for( int a=0; a<2; a++ )
				{
					const ItemAttr Item = g_MultiRareSrc.MultiRareSrc[a].attr;
					if( Item.item_no
					&& 	MouseInRectCheak( SMenu[i].x, SMenu[i].y, g_MultiRareSrc.MultiRareSrc[a].rect ) )
					{
						memset( &g_MultiRareSrc.MultiRareSrc[a], 0, sizeof( MakeItem ) );
						break;
					}
				}
				break;
			}

			int blankpos  = -1;
			for( int a=0; a < 2 ; a++ )
			{
				const ItemAttr Item = g_MultiRareSrc.MultiRareSrc[a].attr;
				if( Item.item_no ) 
				{
					ct++;//들어있는 아이템 갯수
				}
				else
				{
					blankpos = a;
				}
			}//들어갈 수 있는 자리 체크
			
			if( ct>=2)
			{
				break; // 꽉 찼다. 
			}

			//처음 들어 오면 처음에 들어 올수 있는 아이템인지 확인(아이템 넘버, 속성 있는가)후 추가
			if( IP_base.type != INV ) 
			{
				AddCurrentStatusMessage(FONT_COLOR_WHITE , lan->OutputMessage(2,530));//인벤토리 아템을 넣으 십쇼..
				break;
			}
			if( ct == 0 )
			{	 
				if( !CheckBy2ndGM_This_Item_Upgradeable(HandItemAttr,1))
				{//레어 업글 가능 할때 멀티로 만들 수가 있다
					break;//추가 불가능
				}
			}
			if(	ct == 1 )//대상 체크를 하는법
			{	 
				if( !CheckBy2ndGM_Insert_to_GMItemList(HandItemAttr,1))
				{//레어 업글 가능 할때 멀티로 만들 수가 있다
					break;//추가 불가능
				}
			}

			g_MultiRareSrc.MultiRareSrc[blankpos].item_no	= HandItemAttr.item_no;
			g_MultiRareSrc.MultiRareSrc[blankpos].x			= g_pointMouseX-SMenu[i].x;
			g_MultiRareSrc.MultiRareSrc[blankpos].y			= g_pointMouseY-SMenu[i].y;
			::SetRect( &g_MultiRareSrc.MultiRareSrc[blankpos].rect,
				g_MultiRareSrc.MultiRareSrc[blankpos].x-15,
				g_MultiRareSrc.MultiRareSrc[blankpos].y-15,
				g_MultiRareSrc.MultiRareSrc[blankpos].x+15,
				g_MultiRareSrc.MultiRareSrc[blankpos].y+15 );
			g_MultiRareSrc.MultiRareSrc[blankpos].attr		= HandItemAttr;
			g_MultiRareSrc.MultiRareSrc[blankpos].pos		= IP_base;		// 인벤토리 내의 장소를 기억한다.

		}break;
	case	FT_MERCHANT_SELL_ITEM_DROP_ZONE: 
		{//올려놓는 순서 
			if( g_MouseItemType != 1) //핸드에 아이템이 있는가?. -> 없는데마우스 업하면 리스트의 아템(포지션) 지움 
			{
				break;
			}
			Auction.SetSellItem(IP_base,HandItemAttr);
		}break;
	case FT_EXCHANGE_BOND_MONEY_ITEM:
		{//아이템 올림
			if( g_MouseItemType != 1) //핸드에 아이템이 있는가?. -> 없는데마우스 업하면 리스트의 아템(포지션) 지움 
			{
				break;
			}
			switch(HandItemAttr.item_no)
			{
			case BOND_MONEY_1000:
			case BOND_MONEY_500:
			case BOND_MONEY_100:
			case BOND_MONEY_50:
			case BOND_MONEY_10:
			case BOND_MONEY_5:
				{
					Auction.SetBondItem(IP_base,HandItemAttr);
				}break;
			default:
				{
					::AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(4,560));
				}break;
			}
		}break;
	case FT_HUNTER_REGIST :
	case FT_MAIL_WRITE :
		{
			SMenu[i].work = SMenu[i].nField[j].nWillDo;
		}break;
	case FT_FRIEND_NAME :
		{
			int &page_status = SubMenu->work;
			WORD page;
			WORD page_max;
			GetWORDOfDWORD( (DWORD)page_status, page, page_max);

			static int max_line = 10;
			int real_count =0;
			int count=0;
			int login;
			char *name;

			int a;
			for( a=0; a<30; a++)
			{
				if( count >= max_line ) break;
				name = g_Menu_Variable.m_FriendGrup->GetFriend( a, login );
				if( !name[0]) continue;
				if( login )
				{
					if( page*max_line > real_count )
					{
						real_count++;
						continue;
					}
					name = g_Menu_Variable.m_FriendGrup->CheckRect( a, 2, 0, 0 );
					if( name )
					{
						// 메일
						SmallMenuClose();
						::Call_MAIL_WRITE_MENU(name);
						return true;
					}
					name = g_Menu_Variable.m_FriendGrup->CheckRect( a, 1, 0, 0 );
					if( name )
					{
						// 귓속말
						SendWisperByLineCommand( name );
						return true;
					}
					count++;
					real_count++;
				}
			}
			for( a=0; a<30; a++)
			{
				if( count >= max_line ) break;
				name = g_Menu_Variable.m_FriendGrup->GetFriend( a, login );
				if( !name[0]) continue;
				if( !login )
				{
					if( page*max_line > real_count )
					{
						real_count++;
						continue;
					}
					name = g_Menu_Variable.m_FriendGrup->CheckRect( a, 2, 0, 0 );
					if( name )
					{
						// 메일
						SmallMenuClose();
						::Call_MAIL_WRITE_MENU(name);
						return true;
					}
					count++;
					real_count++;
				}
			}
		}break;
	case FT_MAIL_LIST://클릭시에
		{	//타입, 보낸사람, 제목, 날짜
			for(int i = 0; MAX_MAIL_PAGE > i; i++)
			{
				const int iIndex = g_MailMgr.GetMailIndex(i);

				if(!iIndex)//인덱스 값 없는 놈이면 무시
				{
					continue;
				}
				
				const int iX = iMainX+iFieldX;
				const int iY = iMainY+iFieldY+ CMailMgr::LIST_LINE_GAB*i+6;//라인 간격

				//선택됨 마크
				const bool bIsSelect = g_MailMgr.IsSelected(i);
				if( BoxAndDotCrash( iX+6,iY-1, 17, 17, g_pointMouseX, g_pointMouseY ) )
				{
					g_MailMgr.SetSelected(i,!bIsSelect);
				}

				//보낸사람
				if( BoxAndDotCrash( iX+53,iY-3, 100, 18, g_pointMouseX, g_pointMouseY ) )
				{
					const char *szSender = g_MailMgr.GetMailSender(i);
					::Call_MAIL_WRITE_MENU(szSender);
				}

				//제목
				if( BoxAndDotCrash( iX+162,iY-3, 130, 18, g_pointMouseX, g_pointMouseY ) )
				{
					g_MailMgr.RequestBody(iIndex);
					::CloseAllMenu();
				}
			}
		}break;
	case FT_SELECT_OBSERVER_TARGET_TEAM:
		{
			SetArenaObserverTarget(nWillDo);
		}break;
	case FT_SELECT_BETTING_TARGET_TEAM:
		{
			::SetArenaBettingTarget(nWillDo);
		}break;
	case FT_TODAY_NOTICE:
		{	
			if (SMenu[i].nField[j].nWillDo > 0 )
			{
				SMenu[i].nField[j].nWillDo--;
			}
			if(g_iScenarioOn){g_iScenarioOn =0;}
			CloseAllMenu();
			WarMenuNoticeOpen();//011013 lsw
			CallServer( CMD_GET_SCENARIO_INFO );
		}break;
	case FT_MAININTERFACE_MAIL_CALL://MailMenu Call
		{
			g_MailMgr.SendReqMailList(nWillDo,0);
		}break;
	default:
		{
			return false;
		}break;
	}
	return true;
}



void kh_menu_set( )
{
/////////////////////////////////////////////////////////////////////////////////////
// 퀘스트 메뉴
	SMenu[MN_QUEST].nImageType= QUEST_MENU;
	SMenu[MN_QUEST].x= 132+GABX_SCREEN;
	SMenu[MN_QUEST].y= 100;			
	SMenu[MN_QUEST].nFieldCount	= 4;
	SMenu[MN_QUEST].nImageNumber = 1;


	// 왼쪽 책장 넘기기
	SMenu[MN_QUEST].nField[0].nImageType = QUEST_MENU;
	SMenu[MN_QUEST].nField[0].nType = FT_QUEST_SCROLL;			// 처리하는 곳 MenuSubProcessType() 그림을 찍어 주도록 하는곳, DoButtonCheckOfMenu() 마우스 클릭 관계..
	SMenu[MN_QUEST].nField[0].nWillDo = 0;		// left
	//SMenu[MN_QUEST].nField[0].nType = FT_DSCROLL_MEM_BOOK;
	SMenu[MN_QUEST].nField[0].x=39;
	SMenu[MN_QUEST].nField[0].y=64;
	SMenu[MN_QUEST].nField[0].nRectImage=5;
	SMenu[MN_QUEST].nField[0].nImageNumber=5;
	::SetRect(SMenu[MN_QUEST].nField[0].rCheakBox,39,64,39+167,64+107);

	// 오른쪽 책장 넘기기
	SMenu[MN_QUEST].nField[1].nImageType = QUEST_MENU;
	SMenu[MN_QUEST].nField[1].nType = FT_QUEST_SCROLL;			// 처리하는 곳 MenuSubProcessType() 그림을 찍어 주도록 하는곳, DoButtonCheckOfMenu() 마우스 클릭 관계.. 
	SMenu[MN_QUEST].nField[1].nWillDo = 1;		// right
	//SMenu[MN_QUEST].nField[1].nType = FT_ISCROLL_MEM_BOOK;
	SMenu[MN_QUEST].nField[1].x=330;
	SMenu[MN_QUEST].nField[1].y=193;
	SMenu[MN_QUEST].nField[1].nRectImage=4;
	SMenu[MN_QUEST].nField[1].nImageNumber=4;
	::SetRect(SMenu[MN_QUEST].nField[1].rCheakBox,330,193,330+35,193+15);

	// go index
	SMenu[MN_QUEST].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_QUEST].nField[2].nSpecialWillDo = SWD_SUBMENU_BACK;	
	SMenu[MN_QUEST].nField[2].x=310;
	SMenu[MN_QUEST].nField[2].y=17;
	SMenu[MN_QUEST].nField[2].nRectImage=3;
	SMenu[MN_QUEST].nField[2].nImageNumber=3;
	SMenu[MN_QUEST].nField[2].nImageType=QUEST_MENU;
	::SetRect(SMenu[MN_QUEST].nField[2].rCheakBox,310,17,368,35);

	// EXIT	
	SMenu[MN_QUEST].nField[3].nImageType=QUEST_MENU;
	SMenu[MN_QUEST].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_QUEST].nField[3].nSpecialWillDo = SWD_SMALLMENU_END;	
	SMenu[MN_QUEST].nField[3].x=0;
	SMenu[MN_QUEST].nField[3].y=19;
	SMenu[MN_QUEST].nField[3].nRectImage=2;
	SMenu[MN_QUEST].nField[3].nImageNumber=2;
	::SetRect(SMenu[MN_QUEST].nField[3].rCheakBox,0,19,42,54);
	// 숫자 찍기 (계열)// 왼쪽
	/*
	SMenu[MN_QUEST].nField[3].nType = FT_QUEST_SCROLL;
	SMenu[MN_QUEST].nField[3].x=96+35;
	SMenu[MN_QUEST].nField[3].y=33+26+80;
	SMenu[MN_QUEST].nField[3].nImageNumber=DP_MAGIC_CLASS;

	// 숫자 찍기 (계열)// 오른쪽
	SMenu[MN_QUEST].nField[4].nType = FT_QUEST_SCROLL;
	SMenu[MN_QUEST].nField[4].x=265+35-5;
	SMenu[MN_QUEST].nField[4].y=33+26+80;
	SMenu[MN_QUEST].nField[4].nImageNumber=DP_MAGIC_CLASS;
*/

//
// 퀘스트 목록 출력하기
//
/////////////////////////////////////////////////////////////////	
	SMenu[MN_QUEST_INDEX].nImageType= QUEST_MENU;
	SMenu[MN_QUEST_INDEX].x= 132+GABX_SCREEN;
	SMenu[MN_QUEST_INDEX].y= 100;			
	SMenu[MN_QUEST_INDEX].nFieldCount = 5;
	SMenu[MN_QUEST_INDEX].nImageNumber = 0;
	SMenu[MN_QUEST_INDEX].key = 14;		// 한페이지당 출력가능한 인덱스 최대값

	
	SMenu[MN_QUEST_INDEX].nField[0].nType = FT_QUEST_INDEX;
	SMenu[MN_QUEST_INDEX].nField[0].nWillDo = 0;	
	SMenu[MN_QUEST_INDEX].nField[0].x=33;
	SMenu[MN_QUEST_INDEX].nField[0].y=64;
	SMenu[MN_QUEST_INDEX].nField[0].nRectImage=268;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[0].rCheakBox,0,40,200,193);

	SMenu[MN_QUEST_INDEX].nField[1].nType = FT_QUEST_INDEX;
	SMenu[MN_QUEST_INDEX].nField[1].nWillDo = 1;	
	SMenu[MN_QUEST_INDEX].nField[1].x=33;
	SMenu[MN_QUEST_INDEX].nField[1].y=64;
	SMenu[MN_QUEST_INDEX].nField[1].nRectImage=268;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[1].rCheakBox,200,40,400,193);

	
	//왼쪽 책장 넘기기 
	SMenu[MN_QUEST_INDEX].nField[2].nImageType = QUEST_MENU;
	SMenu[MN_QUEST_INDEX].nField[2].nType = FT_QUEST_INDEX_SCROLL;			// 처리하는 곳 MenuSubProcessType() 그림을 찍어 주도록 하는곳, DoButtonCheckOfMenu() 마우스 클릭 관계..
	SMenu[MN_QUEST_INDEX].nField[2].nWillDo = 0;		// left
	//SMenu[MN_QUEST_INDEX].nField[2].nType = FT_DSCROLL_MEM_BOOK;
	SMenu[MN_QUEST_INDEX].nField[2].x=51;
	SMenu[MN_QUEST_INDEX].nField[2].y=193;
	SMenu[MN_QUEST_INDEX].nField[2].nRectImage=5;
	SMenu[MN_QUEST_INDEX].nField[2].nImageNumber=5;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[2].rCheakBox,51,193,50+35,193+15);

	// 오른쪽 책장 넘기기
	SMenu[MN_QUEST_INDEX].nField[3].nImageType = QUEST_MENU;
	SMenu[MN_QUEST_INDEX].nField[3].nType = FT_QUEST_INDEX_SCROLL;			// 처리하는 곳 MenuSubProcessType() 그림을 찍어 주도록 하는곳, DoButtonCheckOfMenu() 마우스 클릭 관계.. 
	SMenu[MN_QUEST_INDEX].nField[3].nWillDo = 1;		// right
	//SMenu[MN_QUEST_INDEX].nField[3].nType = FT_ISCROLL_MEM_BOOK;
	SMenu[MN_QUEST_INDEX].nField[3].x=330;
	SMenu[MN_QUEST_INDEX].nField[3].y=193;
	SMenu[MN_QUEST_INDEX].nField[3].nRectImage=4;
	SMenu[MN_QUEST_INDEX].nField[3].nImageNumber=4;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[3].rCheakBox,330,193,330+35,193+15);

	// EXIT	
	SMenu[MN_QUEST_INDEX].nField[4].nImageType=QUEST_MENU;
	SMenu[MN_QUEST_INDEX].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_QUEST_INDEX].nField[4].nSpecialWillDo = SWD_SMALLMENU_END;	
	SMenu[MN_QUEST_INDEX].nField[4].x=0;
	SMenu[MN_QUEST_INDEX].nField[4].y=19;
	SMenu[MN_QUEST_INDEX].nField[4].nRectImage=2;
	SMenu[MN_QUEST_INDEX].nField[4].nImageNumber=2;
	::SetRect(SMenu[MN_QUEST_INDEX].nField[4].rCheakBox,0,19,42,54);
	
	
//------------------------------------------------------------------------------------------------
// 명성 메뉴 
//------------------------------------------------------------------------------------------------
// 명성메뉴 
	SMenu[MN_FAME].x= 132+GABX_SCREEN;
	SMenu[MN_FAME].y= 50;			
	SMenu[MN_FAME].nFieldCount = 6;
	SMenu[MN_FAME].nImageType= FAME_MENU;
	SMenu[MN_FAME].nImageNumber = 0;

////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------
// 캐릭터 색상변경 인터 페이스 
//------------------------------------------------------------------------------------------------

	//메인 메뉴 뛰우기
	SMenu[MN_CHARACTER_COLOR_CHANGE].x=CENTER_X;
	SMenu[MN_CHARACTER_COLOR_CHANGE].y=100;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nImageNumber=15;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nImageType= SKIN_MENU;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nFieldCount=8; 
// 	SMenu[MN_CHARACTER_COLOR_CHANGE].key=2;			// 1:body, 0:skin

	// 돈찍기
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].nType=FT_SKIN_MONEY;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].x=45;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].y=190;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].nImageNumber=14;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].nImageType=SKIN_MENU;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[1].rCheakBox,97,199, 167, 212 );

	// ok button
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nSpecialWillDo=SWD_CHARACTER_COLOR_CHANGE;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].x=43;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].y=232;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nRectImage=10;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nImageNumber=9;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].nImageType=SKIN_MENU;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[2].rCheakBox,43,232,43+76,232+27);

	// cancel button
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nSpecialWillDo=SWD_SUBMENU_BACK;//021007 lsw
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].x=140;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].y=232;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nRectImage=5;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nImageNumber=4;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].nImageType=SKIN_MENU;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[3].rCheakBox,140,232,140+76,232+27);

	// RGB 값 선택하기 
	//R
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].nType=FT_COLOR_CHANGE_GAUGE;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].x=204;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].y=78;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].nImageNumber=401;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[4].rCheakBox,55,76,193,90);

	//G
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].nType=FT_COLOR_CHANGE_GAUGE;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].x=204;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].y=115;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].nImageNumber=486;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[5].rCheakBox,55,113,193,127);

	//B
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].nType=FT_COLOR_CHANGE_GAUGE;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].x=204;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].y=150;
	SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].nImageNumber=503;
	::SetRect(SMenu[MN_CHARACTER_COLOR_CHANGE].nField[6].rCheakBox,55,148,193,162);

	// Mantle dye
	SMenu[MN_MANTLE_COLOR_CHANGE].x=CENTER_X;
	SMenu[MN_MANTLE_COLOR_CHANGE].y=100;
	SMenu[MN_MANTLE_COLOR_CHANGE].nImageNumber=0;
	SMenu[MN_MANTLE_COLOR_CHANGE].nImageType= MANTLEDYE_IMG;
	SMenu[MN_MANTLE_COLOR_CHANGE].nFieldCount=7; 
// 	SMenu[MN_CHARACTER_COLOR_CHANGE].key=2;			// 1:body, 0:skin

	// ok button
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nSpecialWillDo=SWD_MANTLE_COLOR_CHANGE;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].x=43;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].y=232;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nRectImage=8;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nImageNumber=7;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].nImageType=MANTLEDYE_IMG;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[1].rCheakBox,43,232,43+76,232+27);

	// cancel button
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nSpecialWillDo=SWD_MANTLEDYE_CANCEL;//021007 lsw
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].x=140;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].y=232;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nRectImage=6;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nImageNumber=5;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].nImageType=MANTLEDYE_IMG;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[2].rCheakBox,140,232,140+76,232+27);

	// RGB 값 선택하기 
	//R
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].nType=FT_MANTLE_COLOR_CHANGE_GAUGE;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].x=204;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].y=78;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].nImageNumber=401;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[3].rCheakBox,55,76,193,90);

	//G
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].nType=FT_MANTLE_COLOR_CHANGE_GAUGE;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].x=204;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].y=115;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].nImageNumber=486;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[4].rCheakBox,55,113,193,127);

	//B
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].nType=FT_MANTLE_COLOR_CHANGE_GAUGE;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].x=204;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].y=150;
	SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].nImageNumber=503;
	::SetRect(SMenu[MN_MANTLE_COLOR_CHANGE].nField[5].rCheakBox,55,148,193,162);

	// Item mall reece
	SMenu[MN_ITEM_MALL].x=345+GABX_SCREEN;
	SMenu[MN_ITEM_MALL].y=20;
	SMenu[MN_ITEM_MALL].nImageNumber= 0;
	SMenu[MN_ITEM_MALL].nImageType = ITEMMALL_IMG;
	SMenu[MN_ITEM_MALL].nFieldCount = 7;
	SMenu[MN_ITEM_MALL].key = 15;
	SMenu[MN_ITEM_MALL].CheakType = 0;

	// Buy button
	SMenu[MN_ITEM_MALL].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MALL].nField[0].nSpecialWillDo=SWD_MALL_ITEM_BUY;
	SMenu[MN_ITEM_MALL].nField[0].x=16;
	SMenu[MN_ITEM_MALL].nField[0].y=352;
	SMenu[MN_ITEM_MALL].nField[0].nRectImage=1;
	SMenu[MN_ITEM_MALL].nField[0].nImageNumber=2;
	SMenu[MN_ITEM_MALL].nField[0].nImageType = ITEMMALL_IMG;
	SetRect(SMenu[MN_ITEM_MALL].nField[0].rCheakBox,16, 352, 16+76, 352+27);

	// Close button
	SMenu[MN_ITEM_MALL].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	SMenu[MN_ITEM_MALL].nField[1].nSpecialWillDo=SWD_SMALLMENU_END;
	SMenu[MN_ITEM_MALL].nField[1].x=136;
	SMenu[MN_ITEM_MALL].nField[1].y=352;
	SMenu[MN_ITEM_MALL].nField[1].nRectImage=3;
	SMenu[MN_ITEM_MALL].nField[1].nImageNumber=4;
	SMenu[MN_ITEM_MALL].nField[1].nImageType = ITEMMALL_IMG;
	SetRect(SMenu[MN_ITEM_MALL].nField[1].rCheakBox,136, 352, 136+76, 352+27);

	// Select item
	SMenu[MN_ITEM_MALL].nField[2].nType=FT_MALL_ITEM;
	SMenu[MN_ITEM_MALL].nField[2].nSHideNomalCount=5;
	SMenu[MN_ITEM_MALL].nField[2].nSHideNomalStart=0;
	SMenu[MN_ITEM_MALL].nField[2].nShideNomalPlus=1;	
	SMenu[MN_ITEM_MALL].nField[2].x=14;
	SMenu[MN_ITEM_MALL].nField[2].y=122;
	SetRect(SMenu[MN_ITEM_MALL].nField[2].rCheakBox,13,119,189,225);

	// Shows the currently selected item
	SMenu[MN_ITEM_MALL].nField[3].nType=FT_DATA_PUT;
	SMenu[MN_ITEM_MALL].nField[3].nWillDo=ITEM_BUY_MALL;
	SMenu[MN_ITEM_MALL].nField[3].x=43;
	SMenu[MN_ITEM_MALL].nField[3].y=242;
	SMenu[MN_ITEM_MALL].nField[3].nImageNumber=DP_SELECT_ITEM;

	// Scroll up
	SMenu[MN_ITEM_MALL].nField[4].nType=FT_HIDE_DSCROLL_AUTO_PUT;
	SMenu[MN_ITEM_MALL].nField[4].nWillDo = 2;
	SMenu[MN_ITEM_MALL].nField[4].x=193;
	SMenu[MN_ITEM_MALL].nField[4].y=129;
	SMenu[MN_ITEM_MALL].nField[4].nRectImage=57;		
	SMenu[MN_ITEM_MALL].nField[4].nImageNumber=57;		
	SMenu[MN_ITEM_MALL].nField[4].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_ITEM_MALL].nField[4].rCheakBox,193, 129, 193+17, 129+17);

	// Scroll down
	SMenu[MN_ITEM_MALL].nField[5].nType=FT_HIDE_ISCROLL_AUTO_PUT;
	SMenu[MN_ITEM_MALL].nField[5].nWillDo = 2;
	SMenu[MN_ITEM_MALL].nField[5].x=193;
	SMenu[MN_ITEM_MALL].nField[5].y=199;
	SMenu[MN_ITEM_MALL].nField[5].nRectImage=59;			
	SMenu[MN_ITEM_MALL].nField[5].nImageNumber=59;			
	SMenu[MN_ITEM_MALL].nField[5].nImageType=RUNE_IMG;
	SetRect(SMenu[MN_ITEM_MALL].nField[5].rCheakBox,193, 199, 193+17, 199+17);

	SMenu[MN_ITEM_MALL].nField[6].nType=FT_DATA_PUT;
	SMenu[MN_ITEM_MALL].nField[6].nWillDo=74;
	SMenu[MN_ITEM_MALL].nField[6].x=74;
	SMenu[MN_ITEM_MALL].nField[6].y=298;
	SMenu[MN_ITEM_MALL].nField[6].nImageNumber=DP_CHARACTER_RAJA_EMBLEM;
}

bool IsSameItemNoANDPos(const int &iItemNoSrc ,const int &iItemNoDest, const POS &pos,const int iType, const int iFirst, const int iSecond, const int iThird)
{//이거랑 일치하면 디스플레이 못해
	if(iItemNoSrc == iItemNoDest
	&&	pos.type == iType
	&&	pos.p1 ==	iFirst
	&&	pos.p2 ==	iSecond
	&&	pos.p3 ==	iThird)
	{
		return true;
	}
	return false;
}

void ButtonDownAtInventory(const int i, const int j)
{
	const int ItemX = ( g_pointMouseX - (SMenu[i].x + 19) ) / 35;
	const int ItemY = ( g_pointMouseY - (SMenu[i].y + 210) ) / 35;
	if( ItemY >= 3 || ItemY < 0 || ItemX >= 8 || ItemX < 0 ) 
	{
		return;
	}
	
	const int ItemX2 = ( StateMousePoint.x - (SMenu[i].x + 19) ) / 35;
	const int ItemY2 = ( StateMousePoint.y - (SMenu[i].y + 210) ) / 35;
	if( ItemX2 != ItemX || ItemY2 != ItemY2 ) 
	{
		return;
	}
																	
	const int xCheck = ( g_pointMouseX - (SMenu[i].x + 19) ) % 35;			//경계선의 클릭은 무시
	const int yCheck = ( g_pointMouseY - (SMenu[i].y + 210) ) % 35;
	if( (xCheck < 2) || (xCheck > 30) || (yCheck < 2) || (yCheck > 30 ) )  
	{
		return;	
	}

	const int first = SMenu[i].nField[PAGE_NUMBER__].nSHideNomalStart;
	const int second = ItemY;
	const int third = ItemX;
	ItemAttr &item = InvItemAttr[first][second][third];

	if( g_MouseItemType )	
	{
		return;
	}

	if( SMenu[MN_GAMBLE].bActive )	// 작업대가 활성화 됐을 경우 작업대에 놓여 있는 아이템은 선택되어지지 않는다.
	{
		int a;
		for(a=0; a<20; a++)
		{
			const POS &tempPos = make_item_list[a].pos;
			const ITEMATTR *pItem = &make_item_list[a].attr;
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
		if( a != 20 ) 
		{
			return;
		}
	}
	//010708 lsw
	if( SMenu[MN_GM_2ND_SKILL].bActive )	// GM 의작업대가 활성화 됐을 경우 작업대에 놓여 있는 아이템은 선택되어지지 않는다.
	{
		int a;
		for(a=0; a<MAX_UPGRADE_RARE; a++)
		{
			const POS &tempPos = gm_make_item_list[a].pos;
			const ITEMATTR *pItem = &gm_make_item_list[a].attr;
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
		if( a != 20 ) 
		{
			return;
		}
	}
	if( SMenu[MN_SKILL_MASTER_MAKE].bActive )
	{
		int a;
		for( a=0; a<2; a++ )
		{
			const POS &tempPos = g_MultiRareSrc.MultiRareSrc[a].pos;
			const ITEMATTR *pItem = &g_MultiRareSrc.MultiRareSrc[a].attr;
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
		if( a!=2 ) 
		{
			return;
		}
	}
	if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
	{
		const POS &tempPos = Auction.GetSellItemPos();//팔겠다고 정한건 어둡게 찍음
		const ITEMATTR *pItem = Auction.GetSellItemAttr();
		if(pItem)
		{
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
	}
	if( IsExchangeNow() )
	{
		int a;
		for(a=0; a<MAX_EXCHANGE_ITEM; a++)
		{
			const POS &tempPos = item_give[a].item.item_pos;
			const ITEMATTR *pItem = &item_give[a].item.item_attr;
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
		if( a != MAX_EXCHANGE_ITEM ) 
		{
			return;
		}
	}

	if(SMenu[MN_EXCHANGE_BOND_MONEY].bActive)
	{
		const POS tempPos = Auction.GetBondItemPos();
		const ITEMATTR *pItem = Auction.GetBondItemAttr();
		if(pItem)
		{
			if(::IsSameItemNoANDPos(item.item_no ,pItem->item_no, tempPos,INV,first, second, third))
			{
				return;
			}
		}
	}

	if( item.item_no )
	{
		const int iItemNo = item.item_no;
		::ItemSoundOfGetItem(iItemNo);
		g_MouseItemType=1;
		g_MouseItemNumber = iItemNo;

		HandItemAttr = item;
		::SetItemPos(INV, first,ItemY,ItemX, &IP_base);
		POS pos_t;
		::SetItemPos(HAND, &pos_t);
		::DeleteItem( &item );
		::SendMoveItem( iItemNo, IP_base, pos_t );
	}
	return;
}

void	DisplayInventoryMenu(SMENU *SubMenu ,int j)
{
	int AddVert = 4; // 세로 보정값. //soto-1.02 4
	int	AddHorz = 6; // 가로 보정값. //soto-1.02 6
	
	int i=0, k=0;
	int first_para = SubMenu->nField[ PAGE_NUMBER__].nSHideNomalStart;

	
	for(k=0;k<3;k++) 
	{
		for(i=0;i<8;i++)  //인벤토리창의 세로 최대 크기는 3
		{	//nField[2]는 스크롤을 위한 가상 필드이다//PAGE_NUMBER__
			const ItemAttr &put_item = InvItemAttr[first_para][k][i];
			if( put_item.item_no )
			{
				int put=1;
				if( SMenu[MN_GAMBLE].bActive )
				{
					for( int a=0; a<20; a++ )
					{
						const POS &pos		= make_item_list[a].pos;
						const int iItemNo	= make_item_list[a].item_no;
						if(::IsSameItemNoANDPos(put_item.item_no ,iItemNo, pos,INV,first_para, k, i))//같으면 어두워라
						{
							::PutItemIcon(SubMenu->x+13+22+35*i + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
							put=0;
							break;
						}
					}
					if( put ) 
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}
				}
				else if( IsExchangeNow() )
				{
					for( int a=0; a<MAX_EXCHANGE_ITEM; a++ )
					{
						const POS &pos		= item_give[a].item.item_pos;
						const int iItemNo	= item_give[a].item.item_attr.item_no;
						if(::IsSameItemNoANDPos(put_item.item_no ,iItemNo, pos,INV,first_para, k, i))//같으면 어두워라
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
							put=0;
							break;
						}
					}
					if( put ) 
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}
				}
				else if( SMenu[MN_GM_2ND_SKILL].bActive )//011001 lsw 2차 지엠 때문
				{
					for( int a=0; a<MAX_UPGRADE_RARE; a++ )
					{
						const POS &pos = gm_make_item_list[a].pos;
						const int iItemNo = gm_make_item_list[a].item_no;
						if(::IsSameItemNoANDPos(put_item.item_no ,iItemNo, pos,INV,first_para, k, i))//같으면 어두워라
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
							put=0;
							break;
						}
					}
					if( put )
					{
						
						if( !gm_make_item_result.item_no)//결과물에 아무것도 없으면 만들수 있는 아이템 리스트를 준다
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
						}
						else//결과 물이 나왔으면 결과물이 될수 있는 아이들만 밝게 찍는다
						{
							if(	put_item.item_no == gm_make_item_result.item_no
							||	put_item.item_no == gm_make_item_result_needitem[0]
							||	put_item.item_no == gm_make_item_result_needitem[1])
							{
								::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
							}
							else
							{
								::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no, 15, 1);
							}
						}
					}
				}
				else if( SMenu[MN_SKILL_MASTER_MAKE].bActive )
				{
					for( int a=0; a<2; a++ )
					{
						const POS &pos = g_MultiRareSrc.MultiRareSrc[a].pos;\
						const int iItemNo = g_MultiRareSrc.MultiRareSrc[a].item_no;
						if(::IsSameItemNoANDPos(put_item.item_no ,iItemNo, pos,INV,first_para, k, i))//같으면 어두워라
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
							put=0;
							break;
						}
					}
					if(put)
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k, put_item.item_no);
					}
				}
				else if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
				{
					const POS &pos = Auction.GetSellItemPos();//팔겠다고 정한건 어둡게 찍음
					ItemAttr *pItem = Auction.GetSellItemAttr();
					if(pItem)
					{
						if(::IsSameItemNoANDPos(put_item.item_no ,pItem->item_no, pos,INV,first_para, k, i))//같으면 어두워라
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
						}
						else
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
						}
					}
					else
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}
				}
				else if(SMenu[MN_EXCHANGE_BOND_MONEY].bActive)//021126 lsw
				{
					const POS &pos = Auction.GetBondItemPos();//팔겠다고 정한건 어둡게 찍음
					ItemAttr *pItem = Auction.GetBondItemAttr();
					if(pItem)
					{
						if(::IsSameItemNoANDPos(put_item.item_no ,pItem->item_no, pos,INV,first_para, k, i))//같으면 어두워라
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no,15,1);
						}
						else
						{
							::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
						}
					}
					else
					{
						::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
					}

				}
				else //이것도 저것도 아닌 상태라면
				{
					::PutItemIcon(SubMenu->x+13+22+35*i  + AddHorz, SubMenu->y+217+35*k + AddVert, put_item.item_no);
				}
			}
		}
	}
	
	if(	(SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fRButtonDown)
	&& !g_MouseItemType )
	{	//[3][3][8] 이기 때문에 이렇게 됩니다 y좌표로 계산
		const int ItemX = ( g_pointMouseY - (SubMenu->y + SubMenu->nField[j].y) ) / 35;
		const int ItemY = ( g_pointMouseX - (SubMenu->x + SubMenu->nField[j].x) ) / 35;
		if( ItemY >= 8 || ItemY < 0 || ItemX >= 3 || ItemX < 0 ) 
		{
			return;
		}
																			
		const int xCheck = ( g_pointMouseX - (SubMenu->x + SubMenu->nField[j].x) ) % 35;
		const int yCheck = ( g_pointMouseY - (SubMenu->y + SubMenu->nField[j].y) ) % 35;
		if( (xCheck < 2) || (xCheck > 30) || (yCheck < 2) || (yCheck > 30 ) )  
		{
			return;
		}

		const ItemAttr &item= InvItemAttr[first_para][ItemX][ItemY];
		if( LButtonDown || !item.item_no ) 
		{
			return;
		}

		if( SMenu[MN_GAMBLE].bActive )				// 아이템 메이크 창에 재료로 올라간 아템은 선택되지 않는다.
		{
			for( i=0; i<20; i++ )
			{
				const POS &pos = make_item_list[i].pos ;
				const ItemAttr &Item = make_item_list[i].attr;
				if( Item.item_no 
				&& pos.type == INV
				&& pos.p1 == first_para
				&& pos.p2 == ItemX 
				&& pos.p3 == ItemY )
				{
					ItemExplain(item);
					break;
				}
			}
			if( i!=20 ) 
			{
				return;
			}
		}
		else if( IsExchangeNow() )		// 아이템 교환창에 올려진 아이템
		{
			for( i=0; i<MAX_EXCHANGE_ITEM; i++ )
			{
				const POS &pos = item_give[i].item.item_pos ;
				const ItemAttr &Item = item_give[i].item.item_attr;
				if( Item.item_no 
				&& pos.type == INV
				&& pos.p1 == first_para
				&& pos.p2 == ItemX 
				&& pos.p3 == ItemY )
				{
					ItemExplain(item);
					break;
				}
			}
			if( i!=20 )
			{
				return;
			}
		}

		if( SMenu[MN_GM_2ND_SKILL].bActive )				// 아이템 메이크 창에 재료로 올라간 아템은 선택되지 않는다.
		{
			for( i=0; i<MAX_UPGRADE_RARE; i++ )
			{
				const POS &pos = gm_make_item_list[i].pos;
				const ItemAttr &Item = gm_make_item_list[i].attr;
				if( Item.item_no 
				&& pos.type == INV
				&& pos.p1 == first_para
				&& pos.p2 == ItemX 
				&& pos.p3 == ItemY ) 
				{
					return;
				}
			}
			if( i!=20 ) 
			{
				return;
			}
		}
		if( SMenu[MN_SKILL_MASTER_MAKE].bActive )
		{
			int a;
			for( a=0; a<2; a++ )
			{
				const POS &pos = g_MultiRareSrc.MultiRareSrc[a].pos;
				const ItemAttr &Item = g_MultiRareSrc.MultiRareSrc[a].attr;
				if( Item.item_no 
				&& pos.type == INV
				&& pos.p1 == first_para
				&& pos.p2 == ItemX 
				&& pos.p3 == ItemY ) 
				{
					return;
				}
			}
			if( a!=2 ) 
			{
				return;
			}
		}
		if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
		{
			const POS pos = Auction.GetSellItemPos();//팔겠다고 정한건 어둡게 찍음
			if(pos.type == INV
			&& pos.p1 ==first_para
			&& pos.p2 == ItemX
			&& pos.p3 ==ItemY)
			{//아예 찍지도 않는다.. 무섭다 ㅡㅡ;..
				return;
			}
		}
		// 040105 YGI
		CItem *t = ItemUnit( item );
		if( t && t->GetItemKind() == IK_TREASURE_MAP )
		{
			CTreasureMapItem *attr = (CTreasureMapItem *)&item.attr[IATTR_TREASURE_MAP];
			POS pos;
			SetItemPos( INV, first_para, ItemX, ItemY, &pos );
			SendGetTreasureMapDetail( &pos );
		}
		//////////////////////////////
		::ItemExplain(item);																		// 아이템 설명
		::PutItemIcon(SubMenu->x+13+22+35*ItemY + AddHorz, SubMenu->y+217+35*ItemX + AddVert, item.item_no, 8);	// 밝게 표시

		int type= item.item_no/1000;
		if( type == POTION )				// 포션일 경우 에니메이션
		{
			::PutItemIconAni( SubMenu->x+13+22+35*ItemY + AddHorz, SubMenu->y+217+35*ItemX + AddVert, item.item_no );
		}
	}
	return;
}

void DisplayQuickMenu(SMENU *SubMenu ,int j)
{
	const int gabx = SubMenu->nField[j].x;
	const int gaby = SubMenu->nField[j].y;
	int i=0, k=0 ;
	for(k=0;k<5;k++) //단축 인벤토리창의 가로 최대 크기는 5 가 되었습니다.
	{
		if(QuickItemAttr[k].item_no )
		{
			if( IsExchangeNow() )
			{
				int put = 1;
				for( int a=0; a<MAX_EXCHANGE_ITEM; a++ )
				{
					if(item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type==QUICK && item_give[a].item.item_pos.p3==k )
					{
						PutItemIcon(SubMenu->x+15+gabx +33*k , SubMenu->y+16+gaby, QuickItemAttr[k].item_no, 15, 1 );
						put=0;
						break;
					}
				}
				if( put ) 
				{
					PutItemIcon(SubMenu->x+15+gabx +33*k , SubMenu->y+16+gaby, QuickItemAttr[k].item_no);
				}
			}
			else 
			{
				PutItemIcon(SubMenu->x+15+gabx +33*k , SubMenu->y+16+gaby, QuickItemAttr[k].item_no);
			}
		}										
	}	//for문 끝
	
	const int ItemX = ( g_pointMouseX - (SubMenu->x + SubMenu->nField[j].x) ) / 33;
	if( 0 > ItemX || 5 <= ItemX)//5개만 디스플레이(마우스 영역 체크
	{
		return;
	}
	if( !QuickItemAttr[ItemX].item_no ) 
	{
		return;
	}

	if( SMenu[MN_GAMBLE].bActive )				// 아이템 메이크 창에 재료로 올라간 아템은 선택되지 않는다.
	{
		for( i=0; i<20; i++ )
		{
			if( make_item_list[i].item_no && make_item_list[i].pos.type == QUICK && make_item_list[i].pos.p3 == ItemX )
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=20 ) 
		{
			return;
		}
	}
	else if( IsExchangeNow() )		// 아이템 교환창에 올려진 아이템
	{
		for( i=0; i<MAX_EXCHANGE_ITEM; i++ )
		{
			if( item_give[i].item.item_attr.item_no && item_give[i].item.item_pos.type == QUICK && item_give[i].item.item_pos.p3 == ItemX ) 
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=MAX_EXCHANGE_ITEM ) 
		{
			return;
		}
	}
	else if( SMenu[MN_GM_2ND_SKILL].bActive )				// 아이템 메이크 창에 재료로 올라간 아템은 선택되지 않는다.
	{
		for( i=0; i<MAX_UPGRADE_RARE; i++ )
		{
			if( gm_make_item_list[i].item_no && gm_make_item_list[i].pos.type == QUICK && gm_make_item_list[i].pos.p3 == ItemX )
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=20 )
		{
			return;
		}
	}

	if( !g_MouseItemType && (SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fRButtonDown) )
	{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }

		if( !LButtonDown ) ItemExplain(QuickItemAttr[ItemX]);
		PutItemIcon(SubMenu->x+15+gabx +33*ItemX , SubMenu->y+16+gaby, QuickItemAttr[ItemX].item_no, 8);

		int type= QuickItemAttr[ItemX].item_no/1000;
		if( type == POTION )				// 포션일 경우 에니메이션
		{
			PutItemIconAni( SubMenu->x+15+gabx +33*ItemX , SubMenu->y+16+gaby, QuickItemAttr[ItemX].item_no);
		}
	}
	return;
}

void DisplayCandleMenu(SMENU *SubMenu ,int j)
{
	const int gabx = SubMenu->nField[j].x;
	const int gaby = SubMenu->nField[j].y;
	int i=0, k=0 ;
	if(QuickItemAttr[5].item_no )
	{
		if( IsExchangeNow() )
		{
			int put = 1;
			for( int a=0; a<MAX_EXCHANGE_ITEM; a++ )
			{
				if(item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type==QUICK && item_give[a].item.item_pos.p3==k )
				{
					PutItemIcon(SubMenu->x+15+gabx , SubMenu->y+16+gaby, QuickItemAttr[k].item_no, 15, 1 );
					put=0;
					break;
				}
			}
			if( put ) PutItemIcon(SubMenu->x+15+gabx  , SubMenu->y+16+gaby, QuickItemAttr[k].item_no);
		}
		else 
		{
			PutItemIcon(SubMenu->x+15+gabx  , SubMenu->y+16+gaby, QuickItemAttr[k].item_no);
		}
	}										
	
	const int ItemX = 5;
	
	if( !QuickItemAttr[ItemX].item_no ) 
	{
		return;
	}

	if( SMenu[MN_GAMBLE].bActive )				// 아이템 메이크 창에 재료로 올라간 아템은 선택되지 않는다.
	{
		for( i=0; i<20; i++ )
		{
			if( make_item_list[i].item_no && make_item_list[i].pos.type == QUICK && make_item_list[i].pos.p3 == ItemX )
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=20 ) 
		{
			return;
		}
	}
	else if( IsExchangeNow() )		// 아이템 교환창에 올려진 아이템
	{
		for( i=0; i<MAX_EXCHANGE_ITEM; i++ )
		{
			if( item_give[i].item.item_attr.item_no && item_give[i].item.item_pos.type == QUICK && item_give[i].item.item_pos.p3 == ItemX ) 
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=MAX_EXCHANGE_ITEM ) 
		{
			return;
		}
	}
	else if( SMenu[MN_GM_2ND_SKILL].bActive )				// 아이템 메이크 창에 재료로 올라간 아템은 선택되지 않는다.
	{
		for( i=0; i<MAX_UPGRADE_RARE; i++ )
		{
			if( gm_make_item_list[i].item_no && gm_make_item_list[i].pos.type == QUICK && gm_make_item_list[i].pos.p3 == ItemX )
			{
				ItemExplain( QuickItemAttr[ItemX] );
				break;
			}
		}
		if( i!=20 )
		{
			return;
		}
	}

	if( !g_MouseItemType && (SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fRButtonDown) )
	{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }

		if( !LButtonDown ) ItemExplain(QuickItemAttr[ItemX]);
		PutItemIcon(SubMenu->x+15+gabx +33*ItemX , SubMenu->y+16+gaby, QuickItemAttr[ItemX].item_no, 8);

		int type= QuickItemAttr[ItemX].item_no/1000;
		if( type == POTION )				// 포션일 경우 에니메이션
		{
			PutItemIconAni( SubMenu->x+15+gabx +33*ItemX , SubMenu->y+16+gaby, QuickItemAttr[ItemX].item_no);
		}
	}
	return;
}

void DisplayTacGauge(SMENU *SubMenu, int j, const int x, const int y, const int iMax, const int iNow, const int iLv,int refresh,int first_exp)
{
	int &old_exp = SubMenu->nField[j].nSHideNomalCount;
	int &old_exp2 = SubMenu->nField[j].nSHideNomalStart;
	int &plus = SubMenu->nField[j].nShideNomalPlus;
//	int &old_exp_for_100 = SubMenu->nField[j].nWillDo;		// 100 랩 이상의 경험치 보여주기를 위하여
//	int &point_count = SubMenu->nField[j].nSpecialWillDo;
	
	if( refresh || !SubMenu->CheakType )
	{
		SubMenu->CheakType = 1;
		old_exp = iNow;
		//old_exp_for_100 = old_exp = exp;
		old_exp2 = iNow;
		plus = 0;
		//point_count = 0;
	}
	if( old_exp < iNow )
	{
		bool refresh_plus = false;
		if( old_exp2 < iNow ) refresh_plus = true;
		old_exp2 = iNow;

		if( !plus || refresh_plus )
		{
			plus = (int )((iNow - old_exp)/30.0f);
			if( !plus ) plus = 1;
		}											
		old_exp += plus;
		if( old_exp > iNow ) old_exp = iNow;
	}
	else
	{
		old_exp = iNow;
		plus = 0;
	}

	if( 0 < iMax-first_exp  )
	{
		int exp_rate = (int)( (double)(old_exp-first_exp)*118.00 / (double)(iMax-first_exp) );		// 118은 찍어줄 게이지 총 길이
		if( exp_rate >= 0 )
		{
		//	for( int i=0; i< 118 - exp_rate; i++ )
			for( int i=0; i< exp_rate; i++ )
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+i, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
			}
		}
	}

	int add_point = 0;
	if( SubMenu->nField[j].nType != FT_DISPLAY_SKILL_EXP ) 
	{//스킬이 아니면 1 더해서 디스플레이
		add_point = 1;
	}
	Hcolor( 190, 250, 190 );
	SetHangulAlign(TA_RIGHT);
	Hprint2( SubMenu->x+SubMenu->nField[j].x+88+30, SubMenu->y+SubMenu->nField[j].y+2, g_DestBackBuf,"%3d",iLv+add_point);
	return;//경험치 전체 값은 안찍어
}

void DisplayEmote(const int x,const int y,const int iSmileNo)//020515 lsw
{
	const int ImageNo = iSmileNo+1;
	if(34 < ImageNo ||1 > ImageNo) //Eleval 22/06/09 - Put 34 for 5 new smileys
	{
		return;
	}
	FieldTypeNomalPut(x, y, 0, 0, ImageNo , NEW_EMOTE_IMG); // 031021 kyo
}

static bool bOn = false;

void CallWisperList(const bool bOpen)
{//켜고 꺼질때 따로 처리 할거 있으면 여기다 할것
	SMenu[MN_WISPER_LIST].bActive = bOpen;
	if(!bOpen)
	{	
		return;	
	}	
}

void ClearQuickMemory()
{
	memset( SaveMemory, 0, sizeof(QUICKMEMORY) * MAX_MAGIC_MEMORY );
}

void CheckDuplicateQuickMemory(const int iType, const int iNo, const int iBuffer)
{
	for(int i = 0; i<MAX_MAGIC_MEMORY;i++)
	{
		if((iType==SaveMemory[i].iType ) &&	(iNo == SaveMemory[i].iNo))
		{
			SaveMemory[i].iType = 0;
			SaveMemory[i].iNo = 0;//중복 지우기
		}
	}
}

void SetQuickMemoryByKeyInput(const int iType, const int iNo, const int iBuffer)
{
	if( !(g_aCurrentKeys[DIK_LCONTROL] & 0x80)  )
	{
		return;//컨트롤키 안누르면 안됨
	}	
	if( g_aCurrentKeys[DIK_F1] & 0x80 )	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[0].SetQuickMemory(Hero->name,iType,iNo);}
	if( g_aCurrentKeys[DIK_F2] & 0x80 )	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[1].SetQuickMemory(Hero->name,iType,iNo);}
	if( g_aCurrentKeys[DIK_F3] & 0x80 )	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[2].SetQuickMemory(Hero->name,iType,iNo);}
	if( g_aCurrentKeys[DIK_F4] & 0x80 )	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[3].SetQuickMemory(Hero->name,iType,iNo);}
	if( g_aCurrentKeys[DIK_F5] & 0x80 )	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[4].SetQuickMemory(Hero->name,iType,iNo);}
	if( g_aCurrentKeys[DIK_F6] & 0x80 )	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[5].SetQuickMemory(Hero->name,iType,iNo);}
	if( g_aCurrentKeys[DIK_F7] & 0x80 )	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[6].SetQuickMemory(Hero->name,iType,iNo);}
	if( g_aCurrentKeys[DIK_F8] & 0x80 )	{CheckDuplicateQuickMemory(iType,iNo,0);SaveMemory[7].SetQuickMemory(Hero->name,iType,iNo);}
}

void DisplayQuickMemory(const int iType, const int iTargetNo, const int iX, const int iY)
{
	for(int i = 0;i < MAX_MAGIC_MEMORY; i++)
	{
		if( iTargetNo == SaveMemory[i].iNo && iType == SaveMemory[i].iType )//찍을 수 있는 상황
		{
			if(!strcmp(SaveMemory[i].OwnerName,Hero->name))//이름이 같아야 하지.
			{
				HprintBold( iX, iY, FONT_COLOR_SOLID_YELLOW, FONT_COLOR_BLACK, "F%d", i+1 );//5 는 F5 부터니까
			//	break;
			}
		}
	}	
	return;
}

void DoQuickmemory(const int iQMNo)
{
	if( (g_aCurrentKeys[DIK_LCONTROL] & 0x80)  )
	{
		return;//030611 lsw 컨트롤키 누르면 퀵메모리 가동 안됨
	}

	if( 0 > iQMNo || MAX_MAGIC_MEMORY <= iQMNo)
	{
		return;
	}

	const int iNo = SaveMemory[iQMNo].iNo;
	const int iType = SaveMemory[iQMNo].iType;
	switch(iType)
	{
	case QM_MAGIC:
		{
			const int nMagic = g_mgrBattle.Convert(iNo);
			if (GetMagic()->IsEnable(nMagic))
			{	//< CSD-TW-030606
				g_mgrBattle.SendMagicSelect(iNo);
				RectImageOn = true;
				MagicOnFlag.SetMagicOn(iNo);
			}	//> CSD-TW-030606
		}break;
	case QM_SKILL:
		{
//			if(SKILLTARGETTYPE_2ND_GM  == SkillTable[ iNo ].Howto)//soto-030903 일단 GM 스킬을 막음.
//				break;

			int para = 1;			
			if( skill[iNo].series==1 ) 
			{
				para = 0;	
			}
			if(SelectSkill( iNo )==1)
			{
				break;
			}
			HotKeyMemory.SetQuickMemory(Hero->name,iType,iNo);//020701 lsw

			SkillOn = 0;
			CanMakeItemListSet( iNo );		// 만들수 있는 아이템 갯수
		}break;
	case QM_COMBATSKILL:
		{
			const int nNo = CombatSkill[iNo].iSkillNo+100;

			if (GetCombat()->IsEnable(nNo))
			{
				g_mgrBattle.SendCombatSelect(nNo);
				RectImageOn = true;
				MagicOnFlag.SetMagicOn(nNo);
			}

      break;
		}
	default:
		{
		}break;
	}
	return;	
}

void DoQuickmemoryByKeyInput(const int iInputKey,bool bIsDikCheck)
{
	if(Hero)
	{
		switch (Hero->viewtype)
		{	//< CSD-030515
		case VIEWTYPE_GHOST_:
		case VIEWTYPE_OBSERVE_:
			{
				return;
			}
		}	//> CSD-030515
	}

	if(bIsDikCheck)
	{
		if(g_aCurrentKeys[DIK_F1] & 0x80 ) {DoQuickmemory(0);}
		if(g_aCurrentKeys[DIK_F2] & 0x80 ) {DoQuickmemory(1);}
		if(g_aCurrentKeys[DIK_F3] & 0x80 ) {DoQuickmemory(2);}
		if(g_aCurrentKeys[DIK_F4] & 0x80 ) {DoQuickmemory(3);}
		if(g_aCurrentKeys[DIK_F5] & 0x80 ) {DoQuickmemory(4);}
		if(g_aCurrentKeys[DIK_F6] & 0x80 ) {DoQuickmemory(5);}
		if(g_aCurrentKeys[DIK_F7] & 0x80 ) {DoQuickmemory(6);}
		if(g_aCurrentKeys[DIK_F8] & 0x80 ) {DoQuickmemory(7);}
	}
	else
	{
		switch (iInputKey)
		{
		case VK_F1:{DoQuickmemory(0);}break;
		case VK_F2:{DoQuickmemory(1);}break;
		case VK_F3:{DoQuickmemory(2);}break;
		case VK_F4:{DoQuickmemory(3);}break;
		case VK_F5:{DoQuickmemory(4);}break;
		case VK_F6:{DoQuickmemory(5);}break;
		case VK_F7:{DoQuickmemory(6);}break;
		case VK_F8:{DoQuickmemory(7);}break;
		default:{} break;
		}
	}
}

int CheckLegendSetComplete(const int iLegendGroupNo, const int iLegendGroupIndex)//입력된 레전드 셋트가 다 모였는가
{
	int aLegendGNo[ITEM_EQUIP_MAX] = {0,};
	int aLegendGIndex[ITEM_EQUIP_MAX] = {0,};
	int i;
	for(i = 0; i <ITEM_EQUIP_MAX; i++)
	{
		const LPRareMain pRare = (LPRareMain)&EquipItemAttr[i].attr[IATTR_RARE_MAIN];
		if( H_LV_LEGEND_ITEM != pRare->iHighLevel){continue;}//레전드가 아닐 경우
		aLegendGNo[i]	= pRare->soksung2;
		aLegendGIndex[i]= pRare->soksung3;
	}

	for(i = 0; i <ITEM_EQUIP_MAX; i++)//체크해서 차고 있는게 레전드 아이템 이라면
	{
		const int iGNo		= aLegendGNo[i];
		const int iGIndex	= aLegendGIndex[i];
		if(!iGNo || !iGIndex){continue;}
		const int iNeeds = g_aItemLegend[iGNo].aSub[iGIndex].iNeeds;
		int iLegendResult = 0;
		for(int aa = 0; aa <ITEM_EQUIP_MAX; aa++)//	needs 체크후 다시 8개를 검색해서 셋트가 되었는지 본다
		{
			if( aLegendGNo[aa]		== iGNo	//내 무기가 iGNo가 같고
			&&	aLegendGIndex[aa]	== iGIndex)//인덱스도 일치 한다면
			{
				int iCmpKey = 0;
				switch(aa)
				{
				case WT_WEAPON:	{iCmpKey = LEGEND_NEED_WEAPON;}break;
				case WT_SHIELD:	{iCmpKey = LEGEND_NEED_SHIELD;}break;
				case WT_HELMET:	{iCmpKey = LEGEND_NEED_HELMET;}break;
				case WT_ARMOR:	{iCmpKey = LEGEND_NEED_ARMOR;}break;
				case WT_NECK:	{iCmpKey = LEGEND_NEED_NECK;}break;
				case WT_SHOES:	{iCmpKey = LEGEND_NEED_SHOES;}break;
				case WT_UNIQUE1:{iCmpKey = LEGEND_NEED_UNIQUE;}break;
				case WT_UNIQUE2:{iCmpKey = LEGEND_NEED_UNIQUE;}break;
				}
				if( iNeeds & iCmpKey)//현재 니드가 aa를 요구 하면
				{
					iLegendResult |= iCmpKey;//레전드에 셋팅을 했다
					aLegendGNo[aa] = 0;//중복 체크가 안되게 하기 위해서 지운다
					aLegendGIndex[aa] = 0;//중복 체크가 안되게 하기 위해서 지운다
				}
			}
		}//for(int aa = 0; aa <ITEM_EQUIP_MAX; aa++)
		if(iNeeds == iLegendResult)
		{//다 찼네.. 굿.
			if(	iLegendGroupNo == iGNo 
			&&	iLegendGroupIndex == iGIndex)
			{//내가 찾던 셋트다
				return 1;//셋트 잘 모았어
			}
		}//(iNeeds == iLegendResult)
	}//for(i = 0; i <ITEM_EQUIP_MAX; i++)//체크해서 차고 있는게 레전드 아이템 이라면
	return 0;
}

void DisplayRareExplainSub(const int x,const int y,int &iCount , int &iReultLineCount, int &iBlk,const int iLGab, const int  iWidth,const RareMain *pRareValue,const int ct,const WORD wForceColor)
{
	int iIndex = 0;
	switch(ct)
	{
	case 0:{iIndex = pRareValue->soksung1;}break;
	case 1:{iIndex = pRareValue->soksung2;}break;
	case 2:{iIndex = pRareValue->soksung3;}break;
	default:{return;}break;
	}
	if(!iIndex){return;}

	if(wForceColor)	{::Hcolor(wForceColor);}
	else{::Hcolor(FONT_COLOR_RARE_DISPLAY);}

	if(pRareValue->IsDynamicRare)
	{
		t_ItemFunction *pIF = &ItemFunction[iIndex];
		switch(pIF->iExpMark%10)
		{
		case 0:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s  ",pIF->Exp);//뒤에 숫자 색
			}break;
		case 1:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %d",pIF->Exp, pIF->GRADE[pRareValue->grade]);//뒤에 숫자 색
			}break;
		case 2:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %d%%",pIF->Exp, pIF->GRADE[pRareValue->grade]);//뒤에 숫자 색
			}break;
		case 3:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %dSec",pIF->Exp, pIF->GRADE[pRareValue->grade]);//뒤에 숫자 색
			}break;
		}
		switch(ItemFunction[iIndex].iExpMark/10)
		{
		case 1://1이면 +
			{
				::Hcolor(FONT_COLOR_RARE_PLUS_POINT);//부호색	
				::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s +",pIF->Exp);// 1 : name
			}break;
		case 2://2이면 -
			{
				::Hcolor(FONT_COLOR_RARE_MINUS_POINT);//부호색	
				::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s -",pIF->Exp);// 1 : name
			}break;
		}
		
		if(wForceColor)	{::Hcolor(wForceColor);}
		else{::Hcolor(FONT_COLOR_RARE_DISPLAY);}
		
		iCount += iReultLineCount;//020801 lsw
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,lan->OutputMessage(0,40),pIF->iTime[pRareValue->grade]);//설명만
		iCount += iReultLineCount;
	}//if(pRareValue->IsDynamicRare)
	else
	{
		t_ItemRare *pIR = &ItemRare[iIndex];
		switch(pIR->iExpMark%10)
		{
		case 0:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s  ",pIR->ExplainAdd);//뒤에 숫자 색
			}break;
		case 1:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %d",pIR->ExplainAdd, pIR->GRADE[pRareValue->grade]);//뒤에 숫자 색
			}break;
		case 2:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %d%%",pIR->ExplainAdd, pIR->GRADE[pRareValue->grade]);//뒤에 숫자 색
			}break;
		case 3:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCount,iWidth,5,"%s   %dSec",pIR->ExplainAdd, pIR->GRADE[pRareValue->grade]);//뒤에 숫자 색
			}break;
		}
		switch(pIR->iExpMark/10)
		{
		case 1://1이면 +
			{
				::Hcolor(FONT_COLOR_RARE_PLUS_POINT);//부호색	
				::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s +",pIR->ExplainAdd);// 1 : name
			}break;
		case 2://2이면 -
			{
				::Hcolor(FONT_COLOR_RARE_MINUS_POINT);//부호색	
				::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s -",pIR->ExplainAdd);// 1 : name
			}break;
		}

		switch(pRareValue->iHighLevel)//메인 설명 색
		{
		case H_LV_NO_HIGH_ITEM:
			{
				::Hcolor(FONT_COLOR_RARE_DISPLAY);
			}break;
		case H_LV_HIGH_ITEM:
			{
				::Hcolor(FONT_COLOR_HIGH_RARE_DISPLAY);
			}break;
		case H_LV_LEGEND_ITEM:
			{
				if(wForceColor)	{::Hcolor(wForceColor);}
				else{::Hcolor(FONT_COLOR_RARE_DISPLAY);}
			}break;
		default:
			{
			}break;
		}
		::Hprint2(x+5,y+iBlk+iLGab*iCount,g_DestBackBuf,"%s", pIR->ExplainAdd);//설명만
		iCount += iReultLineCount;
	}
}

void DisplayRareExplain(const int x,const int y,int &iCount , int &iReultLineCount, int &iBlk,const int iLGab, const int  iWidth,const RareMain *pRareValue)
{
	switch(pRareValue->iHighLevel)
	{
	case H_LV_NO_HIGH_ITEM:
		{
			::Hcolor(FONT_COLOR_RARE_MAIN);
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCount,iWidth,0,lan->OutputMessage(2,790));// 1 : name
			iCount += iReultLineCount;
			for(int ct = 0; 3 > ct; ct++)
			{
				::DisplayRareExplainSub(x,y,iCount, iReultLineCount,iBlk,iLGab,iWidth,pRareValue,ct);
			}
		}break;
	case H_LV_HIGH_ITEM:
		{
			Hcolor( FONT_COLOR_HIGH_RARE_MAIN );
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCount,iWidth,0,lan->OutputMessage(2,791));// 1 : name
			iCount += iReultLineCount;
			::DisplayRareExplainSub(x,y,iCount, iReultLineCount,iBlk,iLGab,iWidth,pRareValue,0);//1개만 표현해
		}break;
	case H_LV_LEGEND_ITEM:
		{
			::Hcolor( FONT_COLOR_HIGH_RARE_MAIN );
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCount,iWidth,0,lan->OutputMessage(2,792));// 1 : name
			iCount += iReultLineCount;
			::DisplayRareExplainSub(x,y,iCount, iReultLineCount,iBlk,iLGab,iWidth,pRareValue,0);//기본 하나만
			
			::Hcolor( FONT_COLOR_HIGH_RARE_MAIN );
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCount,iWidth,0,lan->OutputMessage(2,793));// 1 : name
			iCount += iReultLineCount;

			WORD wForceColor =0;

			if(CheckLegendSetComplete(pRareValue->soksung2, pRareValue->soksung3))//입력된 레전드 셋트가 다 모였는가
			{
				wForceColor =0;
			}
			else
			{
				wForceColor =FONT_COLOR_RED;
			}
			
			for(int xx = 0;MAX_ITEM_LEGEND_SUB_BONUS_MAX>xx;xx++)
			{
				for(int yy = 0;MAX_ITEM_LEGEND_BONUS_KIND>yy;yy++)
				{
					RareMain TempRare = {0,};
					TempRare.grade		= g_aLegendBonus[g_aItemLegend[pRareValue->soksung2].aSub[pRareValue->soksung3].aiBonus[xx]].aiGrade[yy];
					TempRare.soksung1	= g_aLegendBonus[g_aItemLegend[pRareValue->soksung2].aSub[pRareValue->soksung3].aiBonus[xx]].aiKind[yy];
					TempRare.iHighLevel = pRareValue->iHighLevel;
					
					::DisplayRareExplainSub(x,y,iCount, iReultLineCount,iBlk,iLGab,iWidth,&TempRare,0,wForceColor);
				}
			}
		}break;
	default:
		{
			return;
		}break;
	}
}

void CallLoginMenu(const bool bForceLoginActive)
{//021021 lsw
	::Release( &connections );	// 초기화
	::Initialize();
	//::Sleep(1000);
#ifdef _DEBUG
	#ifdef KOREA_LOCALIZING_
	if(!bForceLoginActive)//true가 들어오면 릴리즈모드와 똑같이 동작
	{
		// ini를 체크해서 값이 있으면 
		const int count = CheckSelectServer();
		//if( GetSysInfo( SI_GAME_MAKE_MODE ) ) count = 0;
		
		if( count )
		{	// 서버 선택 메뉴를 띄운다.
			SMenu[MN_SELECT_SERVER].bActive = true;
			SMenu[MN_SELECT_SERVER].work = count;
			SMenu[MN_SELECT_SERVER].nFieldCount = 3+count;
			if( SMenu[MN_SELECT_SERVER].nFieldCount > 8 ) 
			{
				SMenu[MN_SELECT_SERVER].nFieldCount = 8;
			}
			for( int i=0; i<count && i<5; i++ )
			{
				SMenu[MN_SELECT_SERVER].nField[3+i].nType = FT_SELECT_SERVER_BUTTON;
			}
		}
	return;
	}
#endif
#endif
#ifndef _DEBUG1
	#ifdef	JAPAN_LOCALIZING_//일본은 바로 로그인.
		char* pTID = g_pBill->GetJapanTID();
		::memset(sId,0,sizeof(sId));
		::memset(sPassword,0,sizeof(sPassword));
		::memcpy(sId,pTID,19);//여기다가 ID pass 를 준다.
		::memcpy(sPassword,pTID+19,19);////여기다가 ID pass 를 셋팅 해야 SWD_LOGON_Clicked 를
		::SWD_LOGON_Clicked(MN_LOGIN);
		return;
	#endif
#endif

	strcpy( sId ,"");//021001 lsw
	strcpy( sPassword,"" );
	EWndMgr.ClearAllTxt();//021001 lsw
	SetFocus2(HWND_1);//021001 lsw
	SMenu[MN_LOGIN].bActive=true;
	SMenu[MN_SELECT_CHARACTER].bActive=false;
	id_password = true;
}

void SWD_LOGON_Clicked(const int nMenuIndex)
{ 
	g_bLogOnFlag = false;//010909 lsw 접속전에 로그온 플래그를 죽여 준다
	int ret =0;
	if( !sId[0] || !sPassword[0]) return;

	ret = CheckAgentServer( &connections, sId, sPassword );
	
	if( ret ==1 )
	{
#ifdef _DEBUG // finito 060507
		if( GetSysInfo( SI_NOT_CONNECT_MODE ) )
		{
			SMenu[MN_SELECT_CHARACTER].bActive=true;
			SMenu[MN_LOGIN].bActive=false;//020515 lsw
			SCharSource = LoadCh[character_active];
		}
#endif
	}
	else 
	{
		switch( ret )
		{
			case -1: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,71),  0 );	break;//lsw
			case -3: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,75),  0 );	break;
			case -4: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,76),  0 );	break;//lsw
			case -5: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,77),  0 );	break;//lsw

			case -6: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,78),  0 );	break;//lsw
			case -7: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,79),  0 );	break;
			case -8: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,80),  0 );	break;//lsw
			case -9: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,84),  0 );	break;
			case -18: ::CallOkCancelMessageBox(nMenuIndex, 0, 0, lan->OutputMessage(5, 101), 0);	break;
			case -10: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,85), 0 );	break;//lsw
			case -11: ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(3,205),0 );	break;
			default : ::CallOkCancelMessageBox( nMenuIndex, 0,0, lan->OutputMessage(5,86), 0 );	break;//lsw
		}						
		MP3( SN_WARNING );
		
#ifdef	JAPAN_LOCALIZING_//일본은 바로 로그인.
		::Sleep(1000);
		::ExitApplication(EA_JAPAN_LOGIN_ERROR);
#endif
	}
}

void CallDelCharComform()
{
	EWndMgr.ClearAllTxt();//021001 lsw
	y_MenuFocus = MN_DEL_CHAR_COMFORM;
	SMenu[MN_DEL_CHAR_COMFORM].bActive=true;
}

void CallSelectLoginTutorialMenu()// 020815-2 lsw
{
	y_MenuFocus = MN_SELECT_TUTORIAL_LOGIN;
	SMenu[MN_SELECT_TUTORIAL_LOGIN].bActive=true;
}

void ExplainAuctionItem( int x, int y, ItemAttr *olg_item ,const bool bIsDurDraw)//020821-2 lsw
{
	const int trans = 20;
	char temp_str[255] = {0,};
	const int item_type = olg_item->item_no/1000;
	const RareMain *pRareValue = (RareMain*)(&olg_item->attr[IATTR_RARE_MAIN]);
	
	CItem *item = ItemUnit( *olg_item );
	if( !item ) return;
	if( !olg_item->item_no ) return;

	const int kind = item->GetItemKind();

	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_NORMAL_WIDTH;//021001 lsw
	
	int iCt = 0;
	int iReultLineCount = 0;
	
	int iBlk = 0;
	int iReultBlankSize = 0;
	
	const int iLGab = TxtOut.GetLineGab();
	const int iBlkSize = TxtOut.GetBlankGabSize();
	
	ExplainItemName(x , y, olg_item,iCt, iBlk, false);
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )//아이템 이름 출력
	{
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"%s     ( %d )",item->GetItemHanName(), olg_item->item_no);
		iCt += iReultLineCount;
	}
#endif
	if(!bIsDurDraw){return;}
	const int rate = item->GetItemWeight();
	if( rate )
	{
		sprintf( temp_str, lan->OutputMessage(7,153), rate );
		if( item->GetRButton() == DIVIDE_ITEM )
		{
			sprintf( temp_str, lan->OutputMessage(7,154), temp_str, olg_item->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max, d_curr;
			GetItemDuration( *olg_item, d_curr, d_max );
			sprintf( temp_str, lan->OutputMessage(7,155), temp_str, d_curr/10, d_max/10 );
		}
	}
	else
	{
		if( item->GetRButton() == DIVIDE_ITEM )
		{
			if( kind == IK_MONEY )
				sprintf( temp_str, "%d CELL", olg_item->attr[IATTR_MUCH] );
			if( kind == IK_NEW_MONEY )
				sprintf( temp_str, "%d CRIT", olg_item->attr[IATTR_MUCH] );
			else
				sprintf( temp_str, lan->OutputMessage(7,156), olg_item->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max, d_curr;
			GetItemDuration( *olg_item, d_curr, d_max );
			sprintf( temp_str, lan->OutputMessage(7,157), d_curr/10, d_max/10 );
		}
	}

	Hcolor( FONT_COLOR_NUMBER );
	iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str);
	iCt += iReultLineCount;

}

char* GetTacticName(const int iTacticKind)
{
	if( TACTICS_MAX<iTacticKind || TACTICS_START> iTacticKind){return "Error";}

	int iType = 0, iNum = 0;
	switch(iTacticKind)
	{
	case TACTICS_START:					{ iType = 2,iNum = 740;	}break;//택틱요구안함
	case TACTICS_Crapple:				{ iType = 9,iNum = 451;	}break;
	case TACTICS_swordmanship:			{ iType = 9,iNum = 452;	}break;
	case TACTICS_Archery:				{ iType = 9,iNum = 453;	}break;
	case TACTICS_Fencing:				{ iType = 9,iNum = 454;	}break;
	case TACTICS_Mace_fighting:			{ iType = 9,iNum = 455;	}break;
	case TACTICS_Pierce:				{ iType = 9,iNum = 456;	}break;
	case TACTICS_Whirl:					{ iType = 9,iNum = 457;	}break;
	case TACTICS_Hurl:					{ iType = 9,iNum = 458;	}break;
	case TACTICS_Parrying:				{ iType = 9,iNum = 459;	}break;
	case TACTICS_Double_Swordmanship:	{ iType = 9,iNum = 460;	}break;
	case TACTICS_Double_MaceFighting:	{ iType = 9,iNum = 461;	}break;
	case TACTICS_Magery:				{ iType = 9,iNum = 462;	}break;
	case TACTICS_Orison:				{ iType = 9,iNum = 463;	}break;
	default:
		{
			{return "Error";}
		}break;
	}

	return lan->OutputMessage(iType, iNum );
}

bool CanMoveItemToMenu(const int iFieldType)
{	//아이템을 드래그 해서 놓을 수 있는 필드
	switch(iFieldType)
	{//021030 lsw
	case FT_INVENTORY :
	case FT_ACCEL_INVENTORY :
	case FT_ARMOR_INVENTORY :
	case FT_BANK_INVENTORY :
	case FT_PARTY:
	case FT_MAKE_ITEM :
	case FT_PARTY_FACE_PUT :
	case FT_GUILD_ITEM_PUT:
	case FT_GM_2ND_SKILL_MAKE_ITEM	:
	case FT_SKILL_MASTER_MAKE_ITEM://021111 lsw
	case FT_EXCHANGE_BOND_MONEY_ITEM:
	case FT_MERCHANT_SELL_ITEM_DROP_ZONE:
	case FT_EXCHANGE_ITEM  :
		{
			return true;
		}break;
	default:
		{
			return false;
		}break;
	}
	return false;
}

void DisplayGmRegistRequest(const int iGmQuestIndex, const int iMainX, const int iMainY)
{
	GM_QUEST *pGmMain = &g_GmMain[iGmQuestIndex];

	Hcolor( FONT_COLOR_NAME );
	::SetHangulAlign( TA_CENTER );
	int iLineCt = 0;
	::Hprint2( iMainX+137, iMainY+127+iLineCt*18, g_DestBackBuf, "%s", pGmMain->name );
	iLineCt++;		// 직업 이름 출력

	::Hcolor( RGB16( 200, 50, 200 ) );
	::SetHangulAlign( TA_LEFT );

	#ifdef CHANGE_MONEY_
	::Hprint2( iMainX+45, iMainY+135+iLineCt*20, g_DestBackBuf, lan->OutputMessage(7,90), pGmMain->need_money );
	#else
	::Hprint2( iMainX+45, iMainY+135+iLineCt*20, g_DestBackBuf, lan->OutputMessage(7,91), pGmMain->need_money );
	#endif

	iLineCt++;
	for( int a=0; a<MAX_GM_NEED_ITEM_COUNT; a++ )
	{
		::Hcolor( RGB16( 200, 250, 200 ) );
		const int iItemNo = pGmMain->need_item[a];
		if( iItemNo )
		{
			::Hprint2( iMainX+45, iMainY+140+iLineCt*20, g_DestBackBuf, lan->OutputMessage(7,92), GetItemName(iItemNo), pGmMain->need_quantity[a]);
			iLineCt++;
		}
	}
}

void DisplayMailIconExplain(const int iX, const int iY,const int iMailType)
{
	const int iWidth = TXT_BG_SHORT_WIDTH;//021001 lsw
	int iCount = 0;
	int iReultLineCount = 0;
	
	int iBlankGab = 0;
	int iReultBlankSize = 0;
	
	const int iLineGab = TxtOut.GetLineGab();
	const int iBlankGabSize = TxtOut.GetBlankGabSize();

	TxtOut.Init(COLOR_BLACK,TXT_BG_SHORT_WIDTH,20);
	TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);

	int iKrLanNo = 0;
	switch(iMailType)
	{
	case CMailMgr::NORMAL_MAIL:				{iKrLanNo = 960;}break;
	case CMailMgr::GUILD_MAIL:				{iKrLanNo = 961;}break;
	case CMailMgr::GAMEMASTER_MAIL:			{iKrLanNo = 962;}break;
	case CMailMgr::NORMAL_MAIL_REPLY:		{iKrLanNo = 963;}break;
	case CMailMgr::GAMEMASTER_MAIL_REPLY:	{iKrLanNo = 964;}break;
	default:								{iKrLanNo = 960;}break;
	}

	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,lan->OutputMessage(2,iKrLanNo));
	iCount += iReultLineCount;
	TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
}