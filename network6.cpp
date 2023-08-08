#include "stdafx.h"
#include "String.h"
#include "mmsystem.h"
#include "Dragon.h"	
#include "network.h"
#include "char.h"
#include "Hong_Sprite.h"
#include "Hong_Sub.h"
#include "CharDataTable.h"
#include "Path.h"
#include "Hong_Light.h"
#include "Effect.h"
#include "DirectSound.h"
#include "Map.h"
#include "Object.h"
#include "Menu.H"
#include "GameProc.h"
#include "SmallMenu.h"
#include "Tool.h"
#include "Skill.h"
#include "Auction.h"
#include "MailMgr.h"


extern void RecvCMD_CONNECT_AGENT_SERVER_CONFORM(t_packet *packet); //030325 kyo 과금
extern void CallSelectLoginTutorialMenu();//020815-2 lsw
extern int menu[];
extern const int MAX_START;

//011212 lsw
extern void RecvCMD_TRADE_MODE_CHANGE(t_packet *p);//030127 lsw //Trade On/Off 셋팅
extern void RecvCMD_RARE_UPGRADE_BBS(t_packet *p);
extern void RecvCMD_ADD_EFFECT(t_packet *p);
extern void SetDayLight(const short int nValue);
extern void RecvCMD_NOW_SKILL_POINT(t_packet *p);//020815 lsw

extern int y_MenuFocus;
void RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(t_packet *p)
{
	y_MenuFocus=0;
	switch (p->u.Hwoa.rang.SelectedCharResult.iKey)
	{
	case 1:
		{
			for(int k=0;k<MAX_START;k++) 
			{
				SMenu[menu[k]].bActive=FALSE;
			}
			
			SMenu[MN_START_MAP].bActive = true;
			MP3( SN_MENU_OPEN );
			if( SCharacterData.nLevel > 0 && SCharacterData.nLevel < 8)//020815-2 lsw 레벨 1~7까지
			{
				CallSelectLoginTutorialMenu();
			}	
		}break;
	default:
		{
			CallOkCancelMessageBox( MN_SELECT_CHARACTER, 0,0, lan->OutputMessage(0,99),  0 );
		}break;
	}
}

void RecvCMD_MAP_MOVE_LV_LIMIT(t_packet *p)//030116 lsw 맵이동 레벨 제한 메세지
{
	const int iLvMin = p->u.Hwoa.rang.MapMoveLvLimit.iLvMin;
	const int iLvMax = p->u.Hwoa.rang.MapMoveLvLimit.iLvMax;
	
	if (iLvMin && iLvMax)
	{
		::AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(2,910), iLvMin,iLvMax);
	}
	if (iLvMin && !iLvMax)
	{
		::AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(2,911), iLvMin);
	}
	if (!iLvMin && iLvMax)
	{
		::AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(2,912), iLvMax);
	}
}

int HandleCommand2(t_packet &packet,t_connection *c)
{
	switch( packet.h.header.type )
	{
	case CMD_STEAL_ON_TRAP:
		{
			::RecvCMD_STEAL_ON_TRAP( &packet.u.server_steal_on_trap );
		}break;
	case CMD_RARE_UPGRADE_BBS:
		{
			::RecvCMD_RARE_UPGRADE_BBS(&packet);
		}break;
	case CMD_ADD_EFFECT:
		{
			::RecvCMD_ADD_EFFECT(&packet);
		}break;
	case CMD_DAYLIGHT:
		{
			::SetDayLight(packet.u.DayLight.nDayLightControl);
		}break;
	case CMD_NOW_SKILL_POINT://020815 lsw
		{
			::RecvCMD_NOW_SKILL_POINT(&packet);
		}break;
	case CMD_MERCHANT_ITEM_BUY_RESULT://구입 결과
		{
			Auction.RecvCMD_MERCHANT_ITEM_BUY_RESULT(packet);
		}break;
	case CMD_MERCHANT_BUY_ITEM_SEARCH_RESULT://검색 결과(구매자
		{
			Auction.RecvCMD_MERCHANT_BUY_LIST_REQUEST_RESULT(packet);
		}break;
	case CMD_MERCHANT_SELL_ITEM_INFO_REQUEST_RESULT://판매자
		{
			Auction.RecvCMD_MERCHANT_SELL_LIST_REQUEST_RESULT(packet);
		}break;
	case CMD_MERCHANT_SELL_ITEM_REGISTER_RESULT:
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_REGISTER_RESULT(packet);
		}break;
	case CMD_MERCHANT_SELL_ITEM_DELETE_RESULT:
		{
			Auction.RecvCMD_MERCHANT_SELL_ITEM_DELETE_RESULT(packet);
		}break;
	case CMD_MERCHANT_RESULT_TAKE_RESULT:
		{
			Auction.RecvCMD_MERCHANT_RESULT_TAKE_RESULT(packet);
		}break;
	case CMD_MERCHANT_RESULT_LIST_REQUEST_RESULT:
		{
			Auction.RecvCMD_MERCHANT_RESULT_LIST_REQUEST_RESULT(packet);
		}break;
	//<! BBD 040303	 상인 백업테이블에서 리스트 받기
	case CMD_MERCHANT_BACKUP_LIST_RESPONSE:
		{
			Auction.RecvCMD_MERCHANT_BACKUP_LIST_RESPONSE(packet);
			break;
		}
	//> BBD 040303	 상인 백업테이블에서 리스트 받기
	case CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET_RESULT:
		{//상인 품목 보기한 결과
			Auction.RecvCMD_MERCHANT_EXCHANGE_LIST_REQUEST_RESULT(packet);
		}break;
	case CMD_MERCHANT_EXCHANGE_RESULT:
		{ //직접거래 결과
			Auction.RecvCMD_MERCHANT_EXCHANGE_LIST_REQUEST_RESULT(packet);
		}break;
	case CMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON:
		{	
			::RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(&packet);
		}break;
	case CMD_MAP_MOVE_LV_LIMIT:
		{
			::RecvCMD_MAP_MOVE_LV_LIMIT(&packet);
		}break;
	case CMD_TRADE_MODE_CHANGE://030127 lsw 트레이드 모드 체인지
		{ 
			::RecvCMD_TRADE_MODE_CHANGE(&packet);
			break;
		}
	case CMD_CONNECT_AGENT_SERVER_CONFORM: //030325 kyo lsw가 만드거 부침
		{
			::RecvCMD_CONNECT_AGENT_SERVER_CONFORM(&packet);
			
		}break;
	case CMD_REP_RECV_MAIL_BODY		:	
		{
			g_MailMgr.RecvBody(&packet);
		}break;
	case CMD_SEND_MAIL_FAIL			:
	case CMD_SEND_MAIL_OK			:	
		{
			g_MailMgr.RecvSendResult( packet.h.header.type ); 
		}break;
	case CMD_MAIL_LIST_FROM_DBDEMON:
		{
			g_MailMgr.RecvMailList( &packet); 
		}break;
	case CMD_MAIL_REQ_BODY_RESULT:
		{
			g_MailMgr.RecvBody( &packet); 
		}break;
	case CMD_MAIL_ALERT:
	case CMD_START_GAME_MAIL:	
		{
			g_MailMgr.RecvAlert();
		}break;
	default :{return 0;}break;
	}
	return 1;
}