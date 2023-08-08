#include "stdafx.h"
#include <ddraw.h>
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sprite.h"
#include "stdio.h"
#include "directsound.h"
#include "directinput.h"
#include "Char.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "convert565to555.h"
#include "Math.h"
#include "skill.h"
#include "menunetwork.h"
#include "CharDataTable.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "chat.h"
#include "item.h"
#include "ExchangeMgr.h"
#include "Battle.h"
#include "ArenaManager.h"
using namespace battle;
#include "Prison.h"
extern CPrison g_cPrison;
int g_iScenarioOn = 0;
int g_iDASexChange = 0; //Eleval
extern int hammer_itempos;
extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
extern void	SendReCallItemFromServer(int type);

void SendItemSell( int item_pos, int store_id )
{
	t_packet packet;
	
	packet.h.header.type =  CMD_ITEM_SELL_REQ;
	packet.u.client_item_sell.store_id = (short int) store_id;
	packet.u.client_item_sell.item_position = (short int) item_pos;
	packet.u.client_item_sell.npc_id = Hero->eventnpcid.GetDecrypted();
	packet.h.header.size = sizeof( t_client_item_sell );
	
	QueuePacket( &packet, 1 );
}

void SendItemRepair( int item_pos, int store_id )
{
	t_packet packet;
	
	packet.h.header.type = CMD_ITEMREPAIR_REQ;
	packet.u.client_item_sell.store_id = (short int) store_id;
	packet.u.client_item_sell.item_position = (short int) item_pos;
	packet.h.header.size = sizeof( t_client_item_sell );
	
	QueuePacket( &packet, 1 );
}

void SendMagicItem( int item_pos, int store_id )
{
	t_packet packet;

	packet.h.header.type = CMD_MAGIC_ITEM;
	packet.u.client_item_sell.store_id = (short int) store_id;
	packet.u.client_item_sell.item_position = (short int) item_pos;
	packet.h.header.size = sizeof( t_client_item_sell );

	QueuePacket( &packet, 1 );
}

void SendItemRepairHammer( int item_pos, int store_id )
{
	t_packet packet;

	packet.h.header.type = CMD_ITEMREPAIR_HAMMER_REQ;
	packet.u.client_item_sell.store_id = (short int) store_id;
	packet.u.client_item_sell.item_position = (short int) item_pos;
	packet.h.header.size = sizeof( t_client_item_sell );

	QueuePacket( &packet, 1 );
}

void SendItemBuy( int num, int inv_pos, int store_id )
{
	t_packet packet;
		
	packet.h.header.type =  CMD_ITEM_BUY_REQ;
	packet.u.client_item_buy.store_id = (short int) store_id;
	packet.u.client_item_buy.item_no = (short int) num;	
	packet.u.client_item_buy.inv_position = (short int) inv_pos;
	packet.u.client_item_buy.npc_id = Hero->eventnpcid.GetDecrypted();
	packet.h.header.size = sizeof( t_client_item_buy );
	
	QueuePacket( &packet, 1 );
}

void SendMallItemBuy(int num, int inv_pos)
{
	t_packet packet;
	packet.h.header.type =  CMD_MALLITEM_BUY;
	packet.u.MallItemBuy.itemNo = (short int)num;	
	packet.u.MallItemBuy.invPos = (short int)inv_pos;
	packet.u.MallItemBuy.npcId = Hero->eventnpcid.GetDecrypted();
	packet.h.header.size = sizeof(t_MallItemBuy);
	QueuePacket( &packet, 1 );
}

void SendReCallItemFromServer( int type )
{
	t_packet packet;
	{
		packet.h.header.type = type;
		packet.h.header.size = 0;
	}
	QueuePacket( &packet, 1 );
}


void RecvPublicNoticeTitle( t_server_menu_message_title *p )
{
	if( p->no )
	{
		int para = (p->no-1) % 10;
		c_BbsTitle[para].SetBbsTitle( p->no, p->date, (char *)p->name, p->title );		// bbs 셋팅
	}
}

void ReqMessageTitle( int type, int count )
{
	t_packet packet;
	
	int t;
	switch( type )
	{
	case MN_TOWNHALL_PUBLICNOTICE :		t = CMD_PUBLIC_NOTICE_TITLE_REQ	; break;
	case MN_TOWNHALL_FORUM :			t = CMD_FORUM_TITLE_REQ			; break;
	case MN_TOWNHALL_EVENT :			t = CMD_EVENT_TITLE_REQ			; break;
	case MN_TOWNHALL_RANK1 :			t = CMD_RANK_TITLE_REQ			; break;
	default : return;
	}
	packet.h.header.type = t;
	{
		packet.u.kein.req_bbs_title.count = count;
	}
	packet.h.header.size = sizeof( k_req_bbs_title );
	QueuePacket( &packet, 1 );
}

void ReqMessageBbs( int type, int count )
{
	t_packet packet;
	
	int t;
	switch( type )
	{
	case MN_TOWNHALL_PUBLICNOTICE :		t = CMD_PUBLIC_NOTICE_BBS_REQ	; break;
	case MN_TOWNHALL_FORUM :			t = CMD_FORUM_BBS_REQ			; break;
	case MN_TOWNHALL_EVENT :			t = CMD_EVENT_BBS_REQ			; break;
	case MN_TOWNHALL_RANK1 :			t = CMD_RANK_BBS_REQ			; break;
	default : return;
	}
	packet.h.header.type = t;
	{
		packet.u.kein.req_bbs_title.count = count;
	}
	packet.h.header.size = sizeof( k_req_bbs_title );
	QueuePacket( &packet, 1 );
}


void ReqCharList( )
{
	t_packet packet;
	
	packet.h.header.type = CMD_RESEND_CHARLIST;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}

void RecvCharInfomation( t_server_user_db_data *p )
{
	SCharacter &ch = SCharacterData;
	
	ch.nCharacterHP	= p->nLife;
	ch.nCharacterMAXHP =  p->nMaxHp;
	ch.nCharacterMP	= p->nMana;
	ch.nCharacterMAXMP = p->nMaxMp;
	ch.nCharacterSP	= p->nHungry;	
	ch.nCharacterMAXSP = p->nMaxHungry;
	// 능력치 추가 ######################################################
	for( int a=0; a<11; a++ )
		ch.nCharacterAbility[a] = p->Avility[a];
	
	ch.nMoney = p->money;
	ch.nLevel = p->lev;
	ch.nExp = p->exp;
	ch.nCharacterData[GENDER] =		p->Attr[0];
	ch.nCharacterData[FACE] =		p->Attr[1];
	//ch.nCharacterData[ARIGEMENT] =	p->Attr[2]; THAI2 YGI
	ch.nCharacterData[CLASS] =		p->Attr[3];
	ch.nCharacterData[SPELL] =		p->Attr[4];
	ch.nCharacterData[TACTICS_WOMAN]=p->Attr[5]; 
	ch.nCharacterData[TACTICS_MAN] = p->Attr[5];
	
	ch.condition = p->condition;
	
	//	#################################################################
}

void SendShowItemOfOtherCh( int ch_id )
{
	t_packet packet;
	
	packet.h.header.type = CMD_OTHER_CH_INV;
	{
		packet.u.kein.client_other_ch_inv.other_ch = ch_id;
	}
	packet.h.header.size = sizeof( kein_client_other_ch_inv );
	QueuePacket( &packet, 1 );
	
	You.id = ch_id;
}

//<soto-tai
void RecvOtherChInv( kein_server_other_ch_inv *inv )
{
	int count = inv->item_count;
	memset( You.inv, 0, sizeof(ItemAttr) * OTHER_INV_MAX  );
	for( int a=0; a<count; a++ )
		You.inv[a] = inv->item[a];
	
	fRMoveMenu = MM_ITEM_OPEN ;
	nItemOther = MN_CHARACTER_ITEM;
	You.id = inv->target_id;
}
//>soto-tai


void SendDivideItem( K_ITEM *item, int much )
{
	t_packet packet;
	
	packet.h.header.type = CMD_DIVIDE_ITEM;
	{
		packet.u.kein.item_attr_pos.item_attr = item->item_attr;
		packet.u.kein.item_attr_pos.item_pos = item->item_pos;
		packet.u.kein.item_attr_pos.much	= much;
	}
	packet.h.header.size = sizeof( kein_item_attr_pos );
	QueuePacket( &packet, 1 );
}

void SendGenrateItemByTool( k_client_create_item p )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CREATE_ITEM;
	{
		packet.u.kein.client_create_item = p;
	}
	packet.h.header.size = sizeof( k_client_create_item );
	QueuePacket( &packet, 1 );
}

void RecvDeleteItem( POS pos, int is_broken )
{
	ItemAttr *temp = ::GetItemByPos( pos );//021121 lsw
	if( !temp ) JustMsg(lan->OutputMessage(6,12) ); //010215 lsw
	else 
	{
		if(pos.type == HAND ) g_MouseItemType = 0;
		if( is_broken )
			AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(6,13), GetItemName( temp->item_no ) );//010215 lsw
		DeleteItem( temp );
	}	
}

void RecvDecreaseItem( k_server_decrease_item *item )
{
	ItemAttr *temp = ::GetItemByPos( item->item_pos );//021121 lsw
	if( !temp ) JustMsg(lan->OutputMessage(6,14) ); //010215 lsw
	else temp->attr[IATTR_DURATION] = item->dur;
}

void SendLvUpPointEach( int type )		// 1포인트 올라 갈때마다..
{
	t_packet packet;
	
	packet.h.header.type = CMD_LVUP_POINT_EACH;
	{
		packet.u.kein.client_lvup_point_each.type  = ( BYTE ) type;
	}
	packet.h.header.size = sizeof( client_lvup_point_each );
	QueuePacket( &packet, 1 );
}
void SendLvDownPointEach( int type )
{
	t_packet packet;
	
	packet.h.header.type = CMD_LVDOWN_POINT_EACH;
	{
		packet.u.kein.client_lvup_point_each.type  = ( BYTE ) type;
	}
	packet.h.header.size = sizeof( client_lvup_point_each );
	QueuePacket( &packet, 1 );
}

void ReqLvUpPoint( )	// 임시 레벨업 포인트 콜...
{
	t_packet packet;
	
	packet.h.header.type = REQ_LEVELUP_POINT;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}

void ReqItemInfo0()
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHR_ITEM_INFO_0;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}

void ReqItemInfo1( )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHR_ITEM_INFO_1;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}

void ReqItemInfo2( int count )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHR_ITEM_INFO_2;
	{
		packet.u.client_item_inv.count = (short int)count;
	}
	packet.h.header.size = sizeof( client_item_inv ) ;
	QueuePacket( &packet, 1 );
}

void ReqMagicInfo()
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHAR_INFO_MAGIC;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}

void ReqSkillInfo()
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHAR_INFO_SKILL;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}

void ReqConnectInfo()
{
	t_packet packet;
	
	packet.h.header.type = CMD_CONNECT_INFO1;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}


void SendLearnSkill( int skill_no, int guild_no )
{
	t_packet packet;
	
	packet.h.header.type = CMD_LEARN_SKILL;
	{
		packet.u.client_learn_skill.skillno = (short int) skill_no;
		packet.u.client_learn_skill.guildindex = (short int) guild_no;
	}
	packet.h.header.size = sizeof( t_client_learn_skill );
	
	QueuePacket( &packet, 1 );
}

void RecvLearnSkill( int s )
{
	MP3( SN_LEARN_SKILL );
	AddCurrentStatusMessage(255,255,0,lan->OutputMessage(2,500));
	SCharacterData.SkillId[s] = true;
}


int SendLearnItem( ItemJoinSelect item )		// 아이템 배우기.. 
{
	int is_learned = FindLearnItem( item.item_no );			// 내가 배운 아이템인지 판단
	if( is_learned ) return -1;		// 이미 배웠음.
	
	if( SCharacterData.nMoney < (DWORD) item_join[item.join_no].tech_price  ) return -2;	// 돈이 부족
	if( !CanLearnItemByNeedSkill( item.join_no) ) return -3;
	
	
	t_packet packet;
	packet.h.header.type = CMD_LEARN_ITEM;
	{
		packet.u.kein.client_learn_item.item_no = item.item_no;
		packet.u.kein.client_learn_item.join_no = item.join_no;
	}
	packet.h.header.size = sizeof( k_client_learn_item );
	QueuePacket( &packet, 1 );
	
	return 1;	// 서버로 전송 했음.
}

void RecvLearnItemOk( k_item_no item )
{
	MP3( SN_LEARN_ITEM );
	int pos = SearchLearnItem( item.item_no );			// 내가 배운 아이템인지 판단
	if( pos == -1 ) 
	{
		JustMsg( lan->OutputMessage(6,15) );//010215 lsw
		return;
	}
	
	SCharacterData.ItemId[SCharacterData.ItemIdCount] = item.item_no;
	SubtractMoney( GetItemAttr( item.item_no, VALUE )*4 );
	SCharacterData.ItemIdCount++;
	
	MP3( SN_LEARN_ITEM);
	CallOkCancelMessageBox( MN_SKILLGUILD_ITEM, 0, 0,lan->OutputMessage(6,16),0 );//010215 lsw
}


void RecvItemRepar( k_server_item_repair_ok *r )
{
	int pos = r->item_pos;
	int money = r->money;
	DWORD dur = r->dur;
	
	int a = (pos & 0x00000060) >> 5 ;	//0105
	int b = (pos & 0x00000018) >> 3 ;
	int c = (pos & 0x00000007) ;
	
	ItemAttr &item = InvItemAttr[a][b][c];
	item.attr[IATTR_DURATION] = dur;
	SubtractMoney( money );
	MP3( SN_ITEM_REPAR );
}

void RecvItemReparHammer( k_server_item_repair_ok *r )
{
	int pos = r->item_pos;	
	DWORD dur = r->dur;
	int x,y,z;
	int pos_conta;

	if (pos == 24 || pos == 48 ||  pos == 72 ){   
        pos_conta = pos -1;
    }else{
        pos_conta = pos;
    }

	x =  pos_conta / 24;
    y = ((pos_conta - (24*x) )/ 8 );


	if ((pos % 8 ) == 0){
        z = 7;
    }else{
        z = (x+1)*24;
        z = z - pos_conta;
        z = 24 - z;
        z = ((y+1)*8)-z;
        z = 8 - z;
        z = z - 1;
    }
   
    if ((pos % 8 ) == 0 && (pos % 24 ) != 0){
        y = y -1;
    }
	
	
	//a = (hammer_itempos / 24);
	//b = (hammer_itempos - (a*24))/8;
	//c = hammer_itempos-(a*24);
	//c = c - (b*8);
	//c = c / 2;

	/*int a = (pos & 0x00000060) >> 5 ;	//0105
	int b = (pos & 0x00000018) >> 3 ;
	int c = (pos & 0x00000007) ;*/

	

	ItemAttr &item = InvItemAttr[x][y][z];
	item.attr[IATTR_DURATION] = dur;	
	MP3( SN_ITEM_REPAR );
}

void RecvServerResult( k_server_result *p )
{
	int type = p->type;
	int result = p->result;
	
	switch( type )
	{
	case CM_REPAIR:
		{
			int menu;
			if( SMenu[MN_SKILLGUILD_REPAIR].bActive ) menu = MN_SKILLGUILD_REPAIR;
			else menu = 0;
			switch( result )
			{
			case 1 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,2),0 );break;//010215 lsw
			case 2 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,3),0 );break;//010215 lsw
			case 3 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,4),0 );break;//010215 lsw
			case 4 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,5),0 );break;//010215 lsw
			default :	JustMsg( lan->OutputMessage(5,6)); break;//010215 lsw
			}
			break;
		}
	case CM_LEARNSKILL : 
		{
			int menu;
			if( SMenu[MN_SKILLGUILD_SKILL].bActive )
			{
				menu = MN_SKILLGUILD_SKILL;
				GetLearnSkillInv( SMenu[menu].nTemp, SMenu[menu].nField[ 6 ].nSHideNomalStart );
			}
			else if( SMenu[MN_HEAL_LEARN].bActive ) menu = MN_HEAL_LEARN;
			else if( SMenu[MN_SKILLGUILD_SKILL].bActive )
			{
				menu = MN_SKILLGUILD_KNOWLAGE;
				GetLearnKnowlageInv( SMenu[menu].nField[ 6 ].nSHideNomalStart );
			}
			else menu = 0;
			switch( result )
			{
			case 1 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,7),0 );break;//010215 lsw
			case 2 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,8),0 );break;//010215 lsw
			case 3 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,9),0 );break;//010215 lsw
			case 4 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,10),0 );break;//010215 lsw
			default :	JustMsg( lan->OutputMessage(5,6)); break;
			}
			break;
		}
	case CM_LEARNMAGIC :
		{
			int menu;
			if( SMenu[MN_LEARN_MAGIC_BOOK].bActive )	menu = MN_LEARN_MAGIC_BOOK;
			else menu = 0;
			switch( result )
			{
			case 2 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,11),0 );break;//010215 lsw
			case 3 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,12),0 );break;//010215 lsw
			case 4 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,13),0 );break;//010215 lsw
				//					case 5 :	CallOkCancelMessageBox( menu, 0, 0, " ",0 );break;
			default :	JustMsg(lan->OutputMessage(5,6) ); break;
			}
			break;
		}
	case CM_PREVENTING :
		{
			int menu = 0;
			if( SMenu[MN_HEAL_MAIN].bActive ) menu = MN_HEAL_MAIN;
			switch( result )
			{
			case 2 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,14),0 );break;//010215 lsw
			default :	JustMsg( lan->OutputMessage(5,6)); break;
			}
			break;
		}
	case CM_CURE_DISEASE :
		{
			int menu = 0;
			if( SMenu[MN_HEAL_MAIN].bActive ) menu = MN_HEAL_MAIN;
			switch( result )
			{
			case 2 :	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,15),0 );break;//010215 lsw
			default :	JustMsg( lan->OutputMessage(5,6)); break;
			}
			break;
		}
	case CM_INN_SLEEP	:
	case CM_INN_REST	:	switch( result )
							{
	case 2 :	CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(5,16),0 );break;//010215 lsw
	default :	JustMsg( lan->OutputMessage(5,6)); break;
							}
								break;
	case CM_OPEN_SUPPER	:	switch( result )
							{
	case 10 :	CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(5,17),0 );break;//010215 lsw
	case 20 :	CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(5,18),0 );break;//010215 lsw
	case 30 :	CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(5,19),0 );break;//010215 lsw
	default :	JustMsg( lan->OutputMessage(5,6)); break;
							}
		
	}
}


int GetLeveByExp(int nStep, DWORD exp)
{
	for (int i = 1; i < Num_Of_NPC_Lev; ++i)
	{
		if (nStep == NPC_Lev_Ref[i].nStep && exp <= NPC_Lev_Ref[i].nMaxExp)
			return i;
	}
	
	return Num_Of_NPC_Lev;
}

void RecvTacSkillExp( t_char_info_tac_skillexp *s )		// 13개
{
	memcpy( &SCharacterData.TacticsExp, s, 13 * sizeof( DWORD ) );
	memcpy( Hero->aStepInfo, s->aStepInfo, 20* sizeof( char ) );
	
	for( int a=0; a<13; a++ )
	{
		SCharacterData.SkillId[a+88] = GetLeveByExp(Hero->aStepInfo[a + TAC_INDEX], SCharacterData.TacticsExp[a]);
		int tac_lv = SCharacterData.SkillId[a+88];
		SCharacterData.TacticsExpNext[a] = NPC_Lev_Ref[tac_lv].nMaxExp;
	}
}

void ReqTacSkillExp( )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHAR_INFO_TAC_SKILL_EXP;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}

void CallServer( int type, void *msg, int size )
{
	t_packet packet;
	packet.h.header.type = type;
	packet.h.header.size = 0;
	if( size && msg )
	{
		memcpy( packet.u.data, msg, size );
		packet.h.header.size = size;
	}
	QueuePacket( &packet, 1 );
}

void RecvSkillExp( int type, t_SkillExp exp )		//31개 짜리
{//021111 lsw
	SCharacterData.nJobSkill[type] = exp;
}

void AllReqSkillExp( int is_end )
{
	static int just_one = 0;
	
	if( is_end )	// 재시작 할때, 
	{
		just_one = 0;
		return;
	}
	
	if( !just_one )	// 처음으로 스킬 경험치를 요청한다.
	{
		memset( SCharacterData.nJobSkill, 0, sizeof( DWORD )*MAX_MOTHER_SKILL ) ;
		just_one = 1;
		CallServer( CMD_REQ_SKILL_ALL );
	}
}

void OpenStatus()
{
	CallServer( CMD_OPEN_STATUS_REQ );
	CallServer( CMD_REQ_DMG );
}

void GetTatics()
{
	SCharacterData.tatics = 88;
	ItemAttr t = EquipItemAttr[WT_WEAPON];
	if( !t.item_no ) return;
	if( t.item_no/1000 == TOOL ) return;
	
	int type, num;
	GetItemNumber( t.item_no, type, num );
	CItem *item = ItemUnit( type, num );
	if( !item )	return;
	
	SCharacterData.tatics = item->GetSkillAbility();
}

void RecvStatusOpen( k_server_status_open *p )
{
	SCharacterData.beat = p->killmon;
	SCharacterData.ac	= p->ac;
	SCharacterData.mov = p->mov;
	SCharacterData.nCharacterAbility[FAME] = p->fame;
}

void RecvBankInventory( k_server_bank_item *i )
{
	int a = i->first;
	int b = i->second;
	
	memcpy( BankItemAttr[a][b], i->inv, sizeof(ItemAttr)*6 );
}

//RecvCharDisease( &p->u.kein.server_disease ); break;
void RecvParty( k_server_party *p )
{
	SCharacter *ch = &SCharacterData;
	for( int i=0; i<6; i++ )
	{
		strcpy( ch->party[i].m_Name, p->name[i] );
		ch->party[i].m_Gender = p->Gender[i];
		ch->party[i].m_ImageNo = p->Face[i];
		ch->party[i].m_Lv = p->Level[i];
		ch->party[i].m_Str = p->Str[i];
		ch->party[i].m_Class = p->Class[i];
	}
	/*
	*/
	
	//	CallServer( CMD_REQ_MY_PARTY );			// 파티원들중 이미 들어 온 놈이 있는지...
	//	SetExistMeOfParty( );					// 파티원중 나는 항상 조인한 상태다.
}

void RecvRelation( k_server_relation *p )
{
	SCharacter *ch = &SCharacterData;
	for( int i=0; i<3; i++ )
	{
		strcpy( ch->relation[i].m_Name, p->name[i] );
		ch->relation[i].m_Gender = p->Gender[i];
		ch->relation[i].m_ImageNo = p->Face[i];
		ch->relation[i].m_Lv = p->Level[i];
		ch->relation[i].m_Str = p->Str[i];
		ch->relation[i].m_Class = p->Class[i];
	}
}

void RecvEmployment( k_server_party *p )
{
	SCharacter *ch = &SCharacterData;
	for( int i=0; i<6; i++ )
	{
		strcpy( ch->employment[i].m_Name, p->name[i] );
		ch->employment[i].m_Gender = p->Gender[i];
		ch->employment[i].m_ImageNo = p->Face[i];
		ch->employment[i].m_Lv = p->Level[i];
		ch->employment[i].m_Str = p->Str[i];
		ch->employment[i].m_Class = p->Class[i];
	}
}

int SendInputParty( int type, int ct, LpCharacterParty ch )		// 파티원 맺기 서버 전송
{
	if (ch->ch != NULL)
	{ //< CSD-020628 : 캐릭터 이외에는 파티 불가
		switch (ch->ch->sprno)
		{ 
		case 0:
		case 1:
			{
				break;
			}
		default:
			{ 
				return 0;
			}
		} 
	} //> CSD-020628
	
	if( strcmp( SCharacterData.sCharacterName, mouse_ch.m_Name ) == 0 ) // 자기 자신은 맺을수 없당.
	{
		MP3( SN_WARNING );
		if( type == PARTY )
			AddCurrentStatusMessage( 178, 189, 255, lan->OutputMessage(5,21) ); //010215 lsw
		else AddCurrentStatusMessage( 178, 189, 255, lan->OutputMessage(5,22) ); //010215 lsw
		return 0;
	}
	if( IsSameCharOfParty( type, &mouse_ch ) )	return 0; // 같은 캐릭터가 있는가?
	if( !IsCheckLevelOfParty( type, &mouse_ch, ct) ) // 파티 등의 관계를 가질수 있는 레벨인가 확인
	{
		MP3( SN_WARNING );
		if( type == PARTY )
			AddCurrentStatusMessage( 178, 189, 255,lan->OutputMessage(5,23) ); //010215 lsw
		return 0;
	}
	// Finito - removed to allow all nations to party
	//<! BBD 040308		중립맵에서는 나라가 달라도 파티허용
/*	if(MapInfo[ MapNumber].nation)
	{
		if( !IsSameNation( type, &mouse_ch ) )
		{
			MP3( SN_WARNING );
			AddCurrentStatusMessage( 178, 189, 255, lan->OutputMessage(5,24) );//010215 lsw
			return 0;
		}
	}
*/
	//> BBD 040308		중립맵에서는 나라가 달라도 파티허용
	
	t_packet packet;
	
	packet.h.header.type = CMD_DO_PARTY;
	{
		packet.u.kein.client_do_party.type= type;
		packet.u.kein.client_do_party.ct= (char) ct;
		strcpy( packet.u.kein.client_do_party.name, ch->m_Name );
	}
	packet.h.header.size = sizeof( k_client_do_party );
	QueuePacket( &packet, 1 );
	
	return 1;
}

void RecvPartyInfo( k_party_info *p )
{
	LpCharacterParty character;
	
	switch( p->type )
	{
	case PARTY :		character = &SCharacterData.party[p->index];		break;
	case RELATION :		character = &SCharacterData.relation[p->index];		break;
	case EMPLOYMENT	:	character = &SCharacterData.employment[p->index];	break;
	}
	
	strcpy( character->m_Name, p->m_Name );
	character->m_Gender	 = p->m_Gender;
	character->m_ImageNo = p->m_ImageNo;
	character->m_Lv		 = p->m_btLevel; // CSD-030806
	character->m_Str	 = p->m_Str;
	character->m_Class	 = p->m_Class;
	character->m_Join	 = (int) p->m_Join;	
	SetExistMeOfParty( );					// 파티원중 나는 항상 조인한 상태다.
}

void RecvDeleteParty_server( int type, int index )
{
	LpCharacterParty character;
	
	switch( type )
	{
	case PARTY :		character = &SCharacterData.party[index];		break;
	case RELATION :		character = &SCharacterData.relation[index];	break;
	case EMPLOYMENT	:	character = &SCharacterData.employment[index];	break;
	}
	
	memset( character, 0, sizeof( CharacterParty ) );
}

void SendDeleteParty( int type, int ct )
{
	t_packet packet;
	
	packet.h.header.type = CMD_DELETE_PARTY;
	{
		packet.u.kein.client_do_party.type= type;
		packet.u.kein.client_do_party.ct= (char) ct;
	}
	packet.h.header.size = sizeof( k_client_delete_party );
	QueuePacket( &packet, 1 );
}

void SendLearnMagic( int magic_num )
{
	t_packet packet;
	
	packet.h.header.type = CMD_REQ_LEARN_MAGIC;
	{
		packet.u.kein.client_learn_magic.num = magic_num;
	}
	packet.h.header.size = sizeof( k_client_learn_magic );
	QueuePacket( &packet, 1 );
}

void RecvLearnMagicOk( int num )
{
	MP3( SN_LEARN_MAGIC );
	int menu;
	if( SMenu[MN_LEARN_MAGIC_BOOK].bActive )	menu = MN_LEARN_MAGIC_BOOK;
	else menu = 0;
	CallOkCancelMessageBox( menu, 0, 0, lan->OutputMessage(5,25),0 );//010215 lsw
	
	SCharacterData.MagicId[num] = true;
	if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) num += 150;
	SubtractMoney( g_lpMT[num].Price );
	
	if( g_bMagicSelect ) 
	{
		if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL && SCharacterData.nLevel > CHOBO_LEVEL ) MagicSetting();		// 위자드 개열일때만
		else MagicSetting2();
	}
	else MagicSetting2();		// 시범 서비스 임시...
}

void SendClientPreventing( int num )		// 질병 번호
{
	t_packet packet;
	
	packet.h.header.type = CMD_PREVENTING;
	{
		packet.u.kein.client_disease.disease_num = num;
	}
	packet.h.header.size = sizeof( k_client_disease );
	QueuePacket( &packet, 1 );
}

void RecvPreventingOk( short int num )
{
	int menu = 0;
	int money = 100;
	
	// 예방에 관한 돈 가져와야 한다.
	money = disease_tbl[num].price_prevent;
	
	if( SMenu[MN_HEAL_MAIN].bActive ) menu = MN_HEAL_MAIN;
	
	SCharacterData.disease_no_time[num] = 15*24*3600/6;
	SubtractMoney( money );
	char buf[127];
	sprintf(buf, lan->OutputMessage(5,26), SHideNomal[HN_HEAL_PREVENTING][num].temp );//010215 lsw
	CallOkCancelMessageBox( menu, 0, 0, buf ,0 );
}

void SendClientCureDisease( int num )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CURE_DISEASE;
	{
		packet.u.kein.client_disease.disease_num = num;
	}
	packet.h.header.size = sizeof( k_client_disease );
	QueuePacket( &packet, 1 );
}

void RecvCureDiseaseOk( short int num )
{
	int menu = 0;
	int money = 100;
	
	// 예방에 관한 돈 가져와야 한다.
	money = disease_tbl[num].price_heal;
	
	if( SMenu[MN_HEAL_MAIN].bActive ) menu = MN_HEAL_MAIN;
	
	SubtractMoney( money );
	SCharacterData.disease[num] = 0;	// 치료
	char buf[127];
	sprintf(buf,lan->OutputMessage(5,27), SHideNomal[HN_HEAL_PREVENTING][num].temp );//010215 lsw
	CallOkCancelMessageBox( menu, 0, 0, buf ,0 );
}

//
//	item 사고 팔기 고치기 가격 요청및 전송 받는 함수들
//
void ReqItemValue_Sell( int type, POS pos )		// 살때 고칠때 가격 가져 오기
{
	t_packet packet;
	switch( type )
	{
	case ITEM_SELL :	packet.h.header.type = CMD_REQ_ITEM_VALUE_SELL;		break;
	case ITEM_REPAIR :	packet.h.header.type = CMD_REQ_ITEM_VALUE_REPAIR;	break;
	}
	{
		packet.u.kein.server_delete_item.item_pos = pos;
	}
	packet.h.header.size = sizeof( k_server_delete_item );
	QueuePacket( &packet, 1 );
}

void ReqItemValue( int item_no )
{
	t_packet packet;
	
	packet.h.header.type = CMD_REQ_ITEM_VALUE;
	{
		packet.u.kein.client_item_no.item_no = item_no;
	}
	packet.h.header.size = sizeof( k_client_item_no );
	QueuePacket( &packet, 1 );
}

void RecvItemValue( int item_no, int money )
{
	int type, num;
	GetItemNumber( item_no, type, num );
	
	CItem *item = ItemUnit(type, num );
	if( !item ) return;
	
	item->curr_value = money;
}

void RecvItemValue_Sell( int item_no, int money )
{
	int type, num;
	GetItemNumber( item_no, type, num );
	
	CItem *item = ItemUnit(type, num );
	if( !item ) return;
	
	item->curr_value_sell = money;
}

void RecvItemValue_Repair( int item_no, int money )
{
	int type, num;
	GetItemNumber( item_no, type, num );
	
	CItem *item = ItemUnit( type, num );
	if( !item ) return;
	
	item->curr_value_repair = money;
}

void ReqMallItemPrice(int item_no)
{
	t_packet packet;
	
	packet.h.header.type = CMD_REQ_MALL_ITEM_PRICE;
	{
		packet.u.kein.client_item_no.item_no = item_no;
	}
	packet.h.header.size = sizeof(k_client_item_no);
	QueuePacket( &packet, 1 );
}

void RecvMallItemPrice(int item_no, int rajaEmblem)
{
	int type, num;
	GetItemNumber( item_no, type, num );
	
	CItem *item = ItemUnit(type, num );
	if( !item ) return;
	
	item->curr_value = rajaEmblem;
}


void SendReqPartyInfo( int type, int num )		// 파티원 능력 전송 요청 루틴
{
	t_packet packet;
	
	packet.h.header.type =  CMD_SERVER_PARTY;
	{
		packet.u.kein.client_delete_party.type= (short int) type;
		packet.u.kein.client_delete_party.ct= (char) num;
	}
	packet.h.header.size = sizeof( k_client_delete_party );
	
	QueuePacket( &packet, 1 );
}

void SendClientInn( int type )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CLIENT_INN;
	{
		packet.u.kein.client_inn.type= (char ) type;
	}
	packet.h.header.size = sizeof( k_client_inn );
	QueuePacket( &packet, 1 );
}

void RecvServerWakeUp()
{
	SCharacterData.sleep = CSC_AWAKE;
	StopWave( SN_SLEEP );
	StopWave( SN_REST );
	SetVolumeMusic( system_info.music );
	SetVolume( system_info.sound );
}

void RecvJoinGameWho( char *name )		// 이름으로 할까? 아님, 번호로 할까?~~
{
	for( int i=0; i<6; i++ )
	{
		if( !strcmp(SCharacterData.party[i].m_Name, name) )
		{
			SCharacterData.party[i].m_Join = 1;
			SendReqPartyInfo( PARTY, i );					// 0620			YGI
			return;
		}
	}
	//	if( i==6) CallServer( CMD_REQ_PARTY_MEMBER );		// 재전송 요청...
}

void RecvExitGameWho( char *name )		// 파티원중 누군가 나갔다..
{
	for( int i=0; i<6; i++ )
	{
		if( !strcmp(SCharacterData.party[i].m_Name, name) )
		{
			SCharacterData.party[i].m_Join = 0;
			return;
		}
	}
}

void RecvMyPartyExist( char *name )
{
	for( int i=0; i<6; i++ )
	{
		if( !strcmp(SCharacterData.party[i].m_Name, name) )
		{
			SCharacterData.party[i].m_Join = 1;
			return;
		}
	}
}

void RecvPartyJoinOk( char *name )
{
	for( int i=0; i<6; i++ )
	{
		if( !strcmp(SCharacterData.party[i].m_Name, name) )
		{
			SCharacterData.party[i].m_JoinOk = 1;
			SearchMyPartyOfMySight( &SCharacterData.party[i]); // 초기 시야권안에 파티 있는지 체크..
			return;
		}
	}
}

void RecvPartyJoinRelease( char *name )
{
	for( int i=0; i<6; i++ )
	{
		if( !strcmp(SCharacterData.party[i].m_Name, name) )
		{
			SCharacterData.party[i].m_JoinOk = 0;
			return;
		}
	}
}

void RecvServerEachItem( K_ITEM *item )
{ 
	ItemAttr *target = ::GetItemByPos( item->item_pos );
	if(!target) DropItemOnHero(item->item_attr);
	else *target = item->item_attr;
}

void RecvServerResist( k_server_resist *p )
{
	SCharacterData.naBasicResist[RT_POISON	]	=	p->naBasicResist[RT_POISON	];//030227 lsw
	SCharacterData.naBasicResist[RT_CURSE	]	=	p->naBasicResist[RT_CURSE	];
	SCharacterData.naBasicResist[RT_HOLY 	]	=	p->naBasicResist[RT_HOLY 	];
	SCharacterData.naBasicResist[RT_FIRE	]	=	p->naBasicResist[RT_FIRE	];
	SCharacterData.naBasicResist[RT_ICE		]	=	p->naBasicResist[RT_ICE		];
	SCharacterData.naBasicResist[RT_ELECT	]	=	p->naBasicResist[RT_ELECT	];
	
	SCharacterData.naExtentionResist[RT_POISON	]	=	p->naExtentionResist[RT_POISON	];//030227 lsw
	SCharacterData.naExtentionResist[RT_CURSE	]	=	p->naExtentionResist[RT_CURSE	];
	SCharacterData.naExtentionResist[RT_HOLY 	]	=	p->naExtentionResist[RT_HOLY 	];
	SCharacterData.naExtentionResist[RT_FIRE	]	=	p->naExtentionResist[RT_FIRE	];
	SCharacterData.naExtentionResist[RT_ICE		]	=	p->naExtentionResist[RT_ICE		];
	SCharacterData.naExtentionResist[RT_ELECT	]	=	p->naExtentionResist[RT_ELECT	];
}

void SendBankInfo(int type,  DWORD money )
{
	t_packet	packet;
	
	packet.u.kein.client_bank_deposit.money  = money;
	packet.h.header.type = type;
	packet.h.header.size = sizeof( k_client_bank_deposit );
	QueuePacket( &packet, 1 );
}

void SendCharInfoBasic( int kind, DWORD data )
{
#ifdef _DEBUG
	t_packet p;
	
	p.h.header.type = CMD_CHAR_INFO_BASIC;
	{
		p.u.char_info_basic.password = (char)111;
		p.u.char_info_basic.kind = (char) kind;
		p.u.char_info_basic.data = data;
	}
	p.h.header.size = sizeof( t_char_info_basic );
	QueuePacket( &p, 1 );
#endif
}

extern DWORD g_ClientTimeNoGap;		//011122 lsw

void RecvCharInfoBasic( t_char_info_basic *p )
{
	if( p->kind < 0 ) return;
	
	DWORD d = p->data;
	
	switch( p->kind )
	{
	case STR		:
	case CON		:
	case DEX		:
	case WIS		:
	case INT_		:
	case MOVP		:
	case CHA		:	
	case ENDU		:
	case MOR		:	
	case LUCK		:
	case WSPS		:
	case HEALTH		:
	case MOV		:	
	case AC			:
	case FAME		:
	case DAMAGE		:
	case HIT		:	
	case TACTICE	:	SCharacterData.nCharacterAbility[p->kind] = d;
		break;
		
	case MONEY		:	SCharacterData.nMoney = d;				break;
		
	case HP			:	SCharacterData.nCharacterHP	= Hero->hp = d; 	break;				// HP와 MP를 저장하는 변수
	case MAX_HP		:	SCharacterData.nCharacterMAXHP	= d;	break;
	case MP			:	SCharacterData.nCharacterMP		= d;	break;				// HP와 MP의 최대치
	case MAX_MP		:	SCharacterData.nCharacterMAXMP	= d;	break;
	case SP			:   SCharacterData.nCharacterSP		= d;	break;	
	case MAX_SP		:  	SCharacterData.nCharacterMAXSP	= d;	break;
	case CP:
		{	//< CSD-TW-030624
			switch (Hero->nRecoveryCombat)
			{
			case BURNING_BLOODY: InsertMagic(Hero, Hero, 659, 0, 0, 0, 0, 0); break;
			case ACUQUIRE_DARK: InsertMagic(Hero, Hero, 660, 0, 0, 0, 0, 0); break;
			}
				
			iCSPNow = d;
			break;	
		}	//> CSD-TW-030624
	case MAX_CP: iCSPMax = d;	 break;
	case BP:     iCSPCNow = d; break;
	case MAX_BP: iCSPCMax = d; break;
	case BANKMONEY	:	SCharacterData.BankMoney		= d;	break;
	case EXP		:	SCharacterData.nExp				= d;	break;
	case FAITH		:	SCharacterData.faith			= d;	break;
		
	case CIB_POTION_BOX: SCharacterData.potion_box1 = d;	break;
		
		//020314 lsw
	case VIEWGHOST	:
		{
			(d)?Hero->bCanViewGhost = true:Hero->bCanViewGhost = false;
			Hero->dwCanViewGhostTime	= g_ClientTimeNoGap +(d*1000);
		}break; 
	case VIEWBOMB	:	
		{
			(d)?Hero->bCanViewBomb	= true:Hero->bCanViewBomb	= false;
			Hero->dwCanViewBombTime		= g_ClientTimeNoGap	+(d*1000);
		}break;
	case VIEWTRAP	:
		{
			(d)?Hero->bCanViewTrap	= true:Hero->bCanViewTrap	= false;
			Hero->dwCanViewTrapTime		= g_ClientTimeNoGap	+(d*1000);	
		}break;
	default			:
		{
			char buf[255];
			sprintf(buf, lan->OutputMessage(7,63), p->kind );//010215 lsw
			JustMsg(buf); 
			break;
		}
	}
}

void RecvItemAttrChange( int type, k_server_item_attr *p )
{
	ItemAttr *item = ::GetItemByPos( p->pos );
	if( !item ) return;		// 다시 전송해 달라고 해야하나?
	
	switch( type )
	{
	case CIA_DURATION	: item->attr[IATTR_DURATION] = p->attr; break;
	case CIA_MUCH		: item->attr[IATTR_MUCH] = p->attr; break;
	}
	return;
}

void RecvOtherGhost( )
{
	memset( You.inv, 0, sizeof( ItemAttr ) * OTHER_INV_MAX );
}

void RecvCreateAbility( )		// 주사위 돌리기전 기본수치 초기화
{
	int basic_ability[5][11] = {
		{ 50, 10, 30, 10, 10, 10, 10, 10, 10, 10, 10 },
		{ 10, 40, 20, 10, 10, 10, 20, 10, 10, 10, 20 },
		{ 10, 50, 20, 10, 10, 10, 20, 10, 10, 10, 10 },
		{ 10, 10, 15, 10, 40, 10, 10, 20, 10, 25, 10 },
		{ 10, 10, 20, 35, 10, 10, 10, 20, 10, 25, 10 },
	};
	
	for( int i=STR; i<=LUCK; i++ )
		SCharSource.nCharacterAbility[i] = basic_ability[SCharSource.nCharacterData[CLASS]][i];
	
	g_GoDice= true;
}

void RecvServerMagery(char nStep, DWORD magery)
{
	int type;
	if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL ) type = MAGERY;
	else if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) type = ORISON;
	else return;
	
	Hero->aStepInfo[type + TAC_INDEX] = nStep;
	SCharacterData.TacticsExp[type] = magery;
	SCharacterData.SkillId[type+88] = GetLeveByExp(nStep,
		SCharacterData.TacticsExp[type]);
	
	int tac_lv = SCharacterData.SkillId[type+88];
	SCharacterData.TacticsExpNext[type] = NPC_Lev_Ref[tac_lv].nMaxExp;
}

void RecvThrowDice( start_throw_dice *p )
{
	g_GoDiceRecv = true;
	
	static int	class_ability[5][2] = { 
								{ STR,	CON	, },
								{ DEX,	MOVP, },
								{ DEX,	CON, },	
								{ INT_,	WSPS, },
								{ WIS,	WSPS, },	};
								
								int first = SCharSource.nCharacterData[CLASS];
								
								SCharSource.nAbilityMax[class_ability[first][0]] = p->ability1;
								SCharSource.nAbilityMax[class_ability[first][1]] = p->ability2;
}

void SendThrowDice( char type )
{
	t_packet p;
	
	p.h.header.type = CMD_THROW_DICE;
	{
		p.u.kein.client_throw_dice.type = type;
	}
	p.h.header.size = sizeof( k_client_throw_dice );
	QueuePacket( &p, 1 );
	
#ifdef _DEBUG // finito 060507	
	if( SysInfo.notconectserver ) 
	{
		start_throw_dice p;
		p.ability1 = 3;
		p.ability2 = 2;
		RecvThrowDice( &p );
	}
#endif
}

void RecvServerMessage( k_get_server_text *p )		// 공지 사항
{
	static int count = 0;
	int para = p->ct%10 - 1;
	if( c_BbsTitle[para].is_first ) 
	{
		c_BbsTitle[para].m_szMessage = new char[p->len+1];
		memset( c_BbsTitle[para].m_szMessage,0, p->len+1 );
		int max = 512;
		if( p->len < 512 ) max = p->len;
		memcpy( c_BbsTitle[para].m_szMessage, p->text, max );
		c_BbsTitle[para].is_first = false;
		count = 0;
	}
	else
	{
		count++;
		int max = 512;
		if( p->len < (count+1)*512 ) max = p->len % 512;
		memcpy( c_BbsTitle[para].m_szMessage + 512 * count, p->text, max );
	}
}

void SendPreachName( char *name, BYTE god )		// 날 전도한 사람이 있는지 판단 요청
{
	t_packet p;
	
	p.h.header.type = CMD_PREACH_NAME;
	{
		sprintf(p.u.kein.server_preach_name.name , name );
		p.u.kein.server_preach_name.believe_god = god;
	}
	p.h.header.size = sizeof( k_server_preach_name );
	QueuePacket( &p, 1 );	
}

void SendPreachNameOk( char *name, BYTE god )	 // 가입 할 경우 전도한 사람에게 신앙심의 +를 위해 패킷 전송
{
	t_packet p;
	
	p.h.header.type = CMD_PREACH_NAME_OK;
	{
		sprintf(p.u.kein.server_preach_name.name , name );
		p.u.kein.server_preach_name.believe_god = god;
	}
	p.h.header.size = sizeof( k_server_preach_name );
	QueuePacket( &p, 1 );	
}

void SendGodRegistOk( int type )
{
	t_packet p;
	
	p.h.header.type = CMD_GOD_REGIST_OK;
	{
		p.u.kein.client_god_regist_ok.type = (char) type;
	}
	p.h.header.size = sizeof( k_client_god_regist_ok);
	QueuePacket( &p, 1 );	
}

void RecvPreachName(  k_server_client_preach_name *p  )
{
	if( !p->believe_god) JustMsg(lan->OutputMessage(6,21));//010215 lsw
	int pre_menu = GetMenuNumByBelieveGod( p->believe_god);
	
	switch( p->result )
	{
	case 1	:	{	// 성공
		CallSmallMenu( MN_GOD_PREACH_RESULT, pre_menu );
		sprintf( SMenu[MN_GOD_PREACH_RESULT].nField[2].temp, lan->OutputMessage(6,22), p->name );//010215 lsw
		sprintf(SMenu[MN_GOD_PREACH_RESULT].nField[0].temp, p->name );
		break;
				}
	case 2	:	{	// 그런 사람 없다. 
		CallSmallMenu( MN_GOD_PREACH_ERROR1, pre_menu );
		sprintf( SMenu[MN_GOD_PREACH_ERROR1].nField[2].temp, lan->OutputMessage(6,23) );//010215 lsw
		break;
				}
	case 3	:	{	// 그 사람은 우리 신전 사람이 아니다.
		CallSmallMenu( MN_GOD_PREACH_ERROR1, pre_menu );
		sprintf( SMenu[MN_GOD_PREACH_ERROR1].nField[2].temp, lan->OutputMessage(6,24) );//010215 lsw
		break;
				}
	case 4	:	{
		;
		CallOkCancelMessageBox( 0,0,0, lan->OutputMessage(6,25) );//010215 lsw
		break;
				}
	default :		break;//
	}
	return;
}

void RecvGodMeetinMainStart( )
{
	g_ServerText.ReSet();
	bCallGodMeetingMenu = true;
}

void RecvGodMeetingMainEnd( )
{
	bCallGodMeetingMenu = false;
	StopWave( SN_GOD_MEETING );
}

void SendToolPassword( char *password )
{
	int ret = -1;
	if( !strcmp( password,		"plant")	)	ret = 0;
	else if( !strcmp( password, "mineral") )	ret = 1;
	else if( !strcmp( password, "herb")	)	ret = 2;
	else if( !strcmp( password, "cook")	)	ret = 3;
	else if( !strcmp( password, "potion")	)	ret = 4;
	else if( !strcmp( password, "tool")	)	ret = 5;
	else if( !strcmp( password, "weapon")	)	ret = 6;
	else if( !strcmp( password, "disposable"))	ret = 7;
	else if( !strcmp( password, "armor")	)	ret = 8;
	else if( !strcmp( password, "accessory"))	ret = 9;
	else if( !strcmp( password, "etc" ) )		ret = 10;
	
	
	if( ret != -1 )
	{
		k_client_create_item gener_item;
		
		gener_item.dur = 1000001;
		gener_item.gennum = 1;
		
		for( int j=0; j<Item_Ref.nItem[ret]; j++ )
		{
			gener_item.type = ret;
			gener_item.num = j;
			
			SendGenrateItemByTool( gener_item );
		}
		
		return;
	}
	
	t_packet p;
	p.h.header.type = CMD_TOOL_PASSWORD;
	{
		strcpy( p.u.kein.tool_password.password, password );
	}
	p.h.header.size = sizeof( k_tool_password );
	QueuePacket( &p, 1 );	
}

extern int				ListenCommand;
int SendCheckNewChar( char *name )
{
	t_packet p;
	
	p.h.header.type = CMD_CHECK_NEW_CHAR;
	{
		strcpy( p.u.kein.check_new_char.name, name);
	}
	p.h.header.size = sizeof( k_check_new_char );
	QueuePacket( &p, 1 );
	
	DWORD StartTime = ::timeGetTime( );
	ListenCommand = 0;
	
	while ( 1 )
	{	
		DWORD Duration = ::timeGetTime( ) - StartTime;
		if ( Duration > WAIT_TIME_RESPONSE_DURATION )
		{
			return( -2 );
		}
		
		if ( !HandleRunning(&connections ) )
		{
			Release(&connections );
			return( -3 );
		}
		
		if ( ListenCommand == 100 )		// 정상 연결
		{
			break;
		}
		else if( ListenCommand == 10 )
		{
			return(-4);					// 기존의 캐릭터 이다
		}
	}
	return 1;
}

void RecvServerMessage( k_get_server_text *p, char *text )		// 신전 예배
{
	static int count = 0;
	int para = p->ct%10 - 1;
	if( c_BbsTitle[para].is_first ) 
	{
		c_BbsTitle[para].m_szMessage = new char[p->len+1];
		memset( c_BbsTitle[para].m_szMessage,0, p->len+1 );
		int max = 512;
		if( p->len < 512 ) max = p->len;
		memcpy( c_BbsTitle[para].m_szMessage, p->text, max );
		c_BbsTitle[para].is_first = false;
		count = 0;
	}
	else
	{
		count++;
		int max = 512;
		if( p->len < (count+1)*512 ) max = p->len % 512;
		memcpy( c_BbsTitle[para].m_szMessage + 512 * count, p->text, max );
	}
}

void RecvServerMessageDefault( k_server_text_default *p, CServerText *text )		// 서버로 요청할 때 초기화 필요
{
	static int count = 0;
	if( text->is_first )
	{
		text->m_pszMessage= new char[p->len+1];
		memset( text->m_pszMessage,0, p->len+1 );
		int max = 512;
		if( p->len < 512 ) max = p->len;
		memcpy( text->m_pszMessage, p->text, max );
		text->is_first = false;
		count = 0;
	}
	else
	{
		count++;
		int max = 512;
		if( p->len < (count+1)*512 ) max = p->len % 512;
		memcpy( text->m_pszMessage + 512 * count, p->text, max );
	}
}

void RecvCallPublicBbs( k_server_text_default *p )		// 서버에서 클라이언트로 공지사항 내용을 보낼때..
{
	if( !SMenu[MN_PUBLIC_BBS].bActive ) 
	{
		public_bbs.ReSet();
		CallSmallMenu( MN_PUBLIC_BBS );
	}
	RecvServerMessageDefault( p, &public_bbs );
}

void RecvCharInfoPlus( k_server_to_client_char_info_plus *p )
{
	SCharacterData.believe_god	=	p->believe_god;
	SCharacterData.faith		=	p->faith;
}

void RecvCanMemorizeOk()
{
	if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )		// 위자드 스펠일 경우만...
	{
		InitMemorize();
		MagicSetting();
		
		CallSmallMenu( MN_MEM_BOOK, 0, SN_MAGIC_BOOK_OPEN ); 
	}
}

void SendChangeMap_kein( short int port )	// 맵 전환 - 첨자: 이동할 게임서버의 포트번호
{
	t_packet p;
	p.h.header.type = CMD_CHANGE_MAP_KEIN;
	{
		p.u.kein.send_game_port.game_port = port;
	}
	p.h.header.size = sizeof( k_send_game_port );
	QueuePacket( &p, 1 );
}

void RecvOpenSupper( char much )
{
	SCharacterData.much_supper = much;
	CallSmallMenu(MN_GOD_SUPPER);
}

void SendIGetItemOfSupper( short int item_no )
{
	t_packet packet;									// 내가 가질려는 아이템 번호를 보내고
	// 인증 받게 되면 그 아이템을 서버에서 받는다.
	
	packet.h.header.type = CMD_I_GET_ITEM_OF_SUPPER;
	{
		packet.u.kein.i_get_item_of_supper.item_no = item_no;
	}
	packet.h.header.size = sizeof( k_i_get_item_of_supper );
	QueuePacket( &packet, 1 );	
}


extern int g_Note_Meeting;
extern char g_Note_Meeting_message[256];

void RecvSetNoteMeetingMessage( short int god, short int type )
{
	g_Note_Meeting = 1;
	char *god_name = NULL;
	
	switch( god ) 
	{
	case EDELBLHOY	: god_name = "EDELBLHOY"; break;
	case TEFFERY	: god_name = "TEFFERY"; break;
	case LETTY		: god_name = "LETTY"; break;
	case OREM		: god_name = "OREM"; break;
	default : god_name = NULL; break;
	}
	
	switch( type ) 
	{
	case 3:	// 연 예배
		if( !god_name ) return;
		sprintf( g_Note_Meeting_message, lan->OutputMessage(6,26), god_name);//010215 lsw
		break;
		
	case 2:	// 월 예배 
		if( !god_name ) return;
		sprintf( g_Note_Meeting_message, lan->OutputMessage(6,27), god_name );//010215 lsw
		break;
	}
}


void SendReqCharInfo( int id )		// 커서를 대고 있는 동안 캐릭터의 정보를 가져온다.
{
	t_packet p;
	
	p.h.header.type = CMD_REQ_CHAR_INFO;
	p.u.debug_client_req_char_info.id = id;
	p.h.header.size = sizeof( t_debug_client_req_char_info );
	
	QueuePacket( &p, 1 );
}


void RecvReqCharInfo( k_server_req_char_info *p )
{
	LPCHARACTER ch = ReturnCharacterPoint( p->id );
	if( ch == NULL ) return;
	
	ch->_Hp			=	p->Hp;
	ch->_HpMax		=	p->HpMax;
	ch->viewdebugcharinfo  =  75;
}			

void SendReqHpOfParty( char *party_name )
{
	int i;
	bool ok=false;
	t_packet p;

	for(i=0;i<6 && !ok;i++)
	{
		if(!strcmp(SCharacterData.party[i].m_Name,party_name))
			ok = true;
	}

	if(!ok)
		return;
	
	p.h.header.type = CMD_REQ_PARTY_INFO;
	strcpy( p.u.kein.req_party_info.party_name,  party_name );
	p.h.header.size = sizeof( k_req_party_info );
	
	QueuePacket( &p, 1 );
}

void RecvReqPartyInfo( k_rep_party_info *p )
{
	bool charOk;
	for( int i=0; i<6; i++ )
	{
		charOk = false;
		if(SCharacterData.party[i].ch)
		{
			// Character is near, update HP.
			if(!strcmp( SCharacterData.party[i].ch->name, p->party_name) )
			{
				SCharacterData.party[i].ch->_Hp = p->hp;
				charOk = true;		// we don't need to re-check the char name
			}
		}
		if(charOk || !strcmp( SCharacterData.party[i].m_Name, p->party_name) )
		{
			SCharacterData.party[i].x = p->x / TILE_SIZE;
			SCharacterData.party[i].y = p->y / TILE_SIZE;
			SCharacterData.party[i].xy_on = 150;
			break;
		}
	}
}

bool recv_learn_item;		// 서버에서 배운 아이템을 전송 받고 스킬 데이타를 로드해오고 그런다음에 배우고 시작하는 아이템을 셋팅해야 한다.
void RecvLearnedItem( k_server_learn_item *p )
{
	memcpy( SCharacterData.ItemId, p->item, sizeof( short int )*p->item_count );
	SCharacterData.ItemIdCount = p->item_count;
	
	if( recv_learn_item ) SetLearnItem( );
	else recv_learn_item = 1;
}
void RecvPartyNotByLevel( short int type )
{
	switch( type )
	{
	case PARTY		: AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(5,82) ); break;//010215 lsw
	case RELATION	:
	case EMPLOYMENT	: break;
	}	
}

void RecvDoPartyOk( char *name, short int type )
{
	switch( type )
	{
	case PARTY		: AddCurrentStatusMessage( 255, 255, 255, lan->OutputMessage(5,83), name ); break;//010215 lsw
	case RELATION	:
	case EMPLOYMENT	: break;
	}	
}



///////////////////////////////////////////////////////////////////////////////////////////////
void SendReqQuestIndex( int page)
{
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver )
	{
		k_rep_quest_index  QUEST_p;
		for (int i = 0 ; i < 14 ; i++){
			QUEST_p.quest[i].index = rand()%13+50;
			QUEST_p.quest[i].ret = 5;
			if( QUEST_p.quest[i].ret == rand()%6 ) QUEST_p.quest[i].ret = 2000;	
		}
		QUEST_p.max = 14;
		
		RecvQuestIndex(&QUEST_p);
	}
	else
#endif
	{
		t_packet p;
		
		p.h.header.type = CMD_REQ_QUEST_INDEX;
		p.u.kein.default_char = page;
		p.h.header.size = sizeof( char );
		QueuePacket( &p, 1 );
		
		//		CallServer( CMD_REQ_QUEST_INDEX );
	}
}

void RecvQuestIndex( k_rep_quest_index *p )
{
/*
short int index[6];
short int ret[6];
	*/
	
	int max = p->max;
	for( int i=0; i<max; i++ )
	{
		int ret = 0;
		for( int j=0; j<QUEST_COUNT; j++ )
		{
			if( quest_num[j] == p->quest[i].index-50 ) 
			{
				if( p->quest[i].ret == 2000 ) SetEndQuest(j);
				ret = 1;
				break;
			}
		}
		if( ret ) continue;
		if( p->quest[i].index <= 50 ) continue;
		
		if( p->quest[i].index )	{
			// 발생한 퀘스트 에다 퀘스트를 넣는다.
			if( QUEST_COUNT == 200) return;
			quest_num[QUEST_COUNT++] = p->quest[i].index-50;		// 50부터 시작
			
			// 스텝이 2000 이면 끝난 퀘스트라구 호승씨가 그랬음  
			if( p->quest[i].ret == 2000 ) 
			{
				SetEndQuest(QUEST_COUNT-1);
				// 성공한 퀘스트 
			}
		}
		else	{
			if( QUEST_COUNT == 200) return;
			QUEST_COUNT++;
		}
		
		// 퀘스트가 없구 그러면 버튼도 넘어 가면 않되니깐 
	}
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////
void SendPacketDefault(const int type, void *data, const int size )
{
	t_packet p;		// 서버로 신청
	p.h.header.type = type;		// 승낙
	p.h.header.size = 0;
	
	if( data && size )
	{
		::memcpy( p.u.data, data, size );
		p.h.header.size = size;
	}
	::QueuePacket( &p, 1 );
}

void SendResetJob( char job, char cls, char *name )
{
	
	if( job < J_FAMER || job > J_MERCHANT ) return;
	
	t_packet p;	
	
	p.h.header.type = CMD_RESET_JOB;
	p.u.kein.reset_job.job= job;
	p.u.kein.reset_job.cls= cls;
	strcpy( p.u.kein.reset_job.name, name );
	p.h.header.size = sizeof( k_reset_job ) - 20+strlen( name )+1;
	QueuePacket( &p, 1 );
}

void SendSelectNation( int nation, char *name )
{
	if( nation <= 0 || nation > N_YILSE ) return;
	
	t_packet p;	
	p.h.header.type = CMD_SELECT_NATION;
	p.u.kein.default_char= nation;
	//		strcpy( p.u.kein.select_nation.name, name );
	p.h.header.size = sizeof( char);
	//	p.h.header.size = sizeof( k_select_nation) - 20+strlen( name )+1;
	QueuePacket( &p, 1 );
}

void SendChangeScriptValue( int script_num, int value )
{
	t_packet p;	
	p.h.header.type = CMD_SCRIPT_CHANGE;
	p.u.kein.script_change.script_num	= script_num;
	p.u.kein.script_change.value		= value;
	p.h.header.size = sizeof( k_script_change );
	QueuePacket( &p, 1 );
}

void RecvChangeDuration( t_packet *p )
{
	POS *pos = &p->u.kein.change_dur.pos;
	DWORD	dur = p->u.kein.change_dur.dur;
	
	ItemAttr *target = ::GetItemByPos( *pos );
	if( target && target->item_no )
	{
		target->attr[IATTR_DURATION] = dur;
	}
	return;
}

void SendIsThereCharNameForNation( char *name )		// 나라 비교를 위해 전송
{	
	t_packet p;
	p.h.header.type = CMD_ISSAME_MY_NATION;
	strcpy( p.u.kein.default_name, name );
	p.h.header.size = strlen( name )+1;
	QueuePacket( &p, 1 );
}	

int CheckCharNameAndNation( char *name )
{
	if( name == NULL ) 
	{
		return 0;		
	}
	
	if( ::stricmp( name, kein_GetMenuString( 47 ) ) == 0 ) 
	{
		return 1;
	}
	
	double		StartTime, Duration;
	
	SendIsThereCharNameForNation( name );
	
	StartTime = ::timeGetTime( );
	ListenCommand = CMD_NONE;
	while( 1 )			
	{					
		Duration = ::timeGetTime( ) - StartTime;	// 5초를 기다린다. 
		if ( Duration > WAIT_TIME_RESPONSE_DURATION )		
		{				
			return FALSE; 	
		}				
		
		if ( !HandleRunning(&connections ) )
		{					
			Release(&connections );	
			return 0;	
		}				
		else if( ListenCommand == CMD_THEREIS_NO_CHARNAME )
		{	
			if( Hero->IsCounselor() ){return 1;}
			return 0;			// 다른 나라이다
		}	
		else if( ListenCommand == CMD_THEREIS_CHARNAME )
		{	
			return 1;			// 같은 나라이다
		}	
	}			
}			

void RecvCallRegistCandidate( )
{
	CallSmallMenu( MN_VOTE_REGIST );
}

void RecvRegistCandidateNot()
{
	g_CanRegistCandidate = -1;		// 등록 할수 있는 사람이 아니다..
	SendCheckVote();		// 투표 메뉴를 띄울지 아닐지...
}

void RecvCallVoteSupport( t_packet *p )
{
	const int ret = p->u.kein.default_char;
	/*	switch( ret )
	{
	default : break;
	}
	*/	RecvRegistCandidateNot();
}

void RecvCheckVoteOk( t_packet *p )		// 후보 정보를 가지고 와서 뿌려준다.
{
	int count = p->u.kein.check_vote_client_ok.candidate_count;
	k_candidate *target = p->u.kein.check_vote_client_ok.candidate;
	
	if( count < 0 || count >=3 ) return;
	int nation = target->nation;
	
	memset( Candidate, 0, sizeof( k_client_candidate )*3 );
	g_Candidate_max = count;
	
	for( int i=0; i<count; i++ )
	{
		Candidate[i].number = target[i].number;
		Candidate[i].lv = target[i].lv;
		Candidate[i].class_type = target[i].class_type;
		Candidate[i].gender = target[i].gender;
		Candidate[i].nation = target[i].nation;
		Candidate[i].guild_code = target[i].guild_code;
		Candidate[i].face = target[i].face;
		strcpy( Candidate[i].name, target[i].name );
	}
	
	CallSmallMenu( MN_VOTE_SUPPORT );
}

int CanVote()
{
	if( !SCharacterData.nation ) return -2;
	return g_CurrVote;
}

void SendCheckVote( )
{
	switch( g_CurrVote )
	{
	case -2: AddCurrentStatusMessage( 250, 30, 30, lan->OutputMessage(3,52));MP3( SN_WARNING ); return;//010215 lsw
		//		case -1: 
		//			AddCurrentStatusMessage( 250, 30, 30, "선거 기간이 아닙니다.");MP3( SN_WARNING ); 
		//			g_CurrVote=0;		// 재차 선거 기간인지를 확인할때를 대비해서
		//			return;
	case -3: 
		{
			// 투표 현황판 보여주기
			SendReqVoteScore();
			return;
		}
	default : CallServer( CMD_CHECK_VOTE ); break;
	}	
}

void RecvCheckVoteNot( t_packet *p )
{
	g_CurrVote = p->u.kein.default_char;
	
	switch( g_CurrVote )
	{
	case -2 : CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(3,61), 0 ); break;//010215 lsw
	case -1 : CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(3,62), 0 ); break;//010215 lsw
	case -3 : 
		{
			AddCurrentStatusMessage( 240, 60, 60, lan->OutputMessage(3,63));//010215 lsw
			if( SendReqVoteScore() == -1 ) MP3( SN_WARNING );
			break;
		}
	}
}

bool CanVoteSupport( short int num ) 
{
	if( num > g_Candidate_max ) return false;
	//	if( g_CanRegistCandidate < 0 ) return false;
	if( !SCharacterData.nation ) return false;
	
	return true;
}

void SendVoteSupport( short int num )		// 후보 번호
{
	if( !CanVoteSupport( num ) ) 
		AddCurrentStatusMessage( 240, 60, 60, lan->OutputMessage(3,64));//010215 lsw
	
	t_packet p;	
	p.h.header.type = CMD_VOTE_GAME2LOGIN;
	p.u.kein.default_char	= (char) num;
	p.h.header.size = sizeof( char );
	QueuePacket( &p, 1 );
}

int SendReqVoteScore()
{
	if( !SCharacterData.nation ) g_CurrVote = -2;
	
	switch( g_CurrVote )
	{
	case -2 : AddCurrentStatusMessage( 250, 30, 30, lan->OutputMessage(3,65)); return -1;//010215 lsw
		//case -1 : AddCurrentStatusMessage( 250, 30, 30, "선거 기간이 아닙니다.", 0 ); return -1;
	}
	
	CallServer( CMD_REQ_VOTE_SCORE );
	return 1;
}

int RecvVoteScore( t_packet *p )
{
	int max = p->u.kein.client_rep_vote_score.max;
	k_score_and_name *target = p->u.kein.client_rep_vote_score.name_score;
	
	g_Candidate_max = 0;
	for( int i=0; i<max; i++ )
	{
		Candidate[i].score = target[i].score;
		if( target[i].name[0] )
		{
			strcpy( Candidate[i].name, target[i].name);
			g_Candidate_max ++;
		}
	}
	
	g_VoteSignExist = 400;		// 현황판 보여주기
	return 1;
}

void SendAllMsgNation( char *text )
{
	if( !Hero->name_status.king )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 220,30,30, lan->OutputMessage(3,66));//010215 lsw
		return;
	}
	
	int len = strlen( text );
	if( len >= MAX_MSG ) AddCurrentStatusMessage( 220,30,30, lan->OutputMessage(3,67));//010215 lsw
	
	if( !text[0] ) return;
	ConvString( text );
	t_packet p;
	
	p.h.header.type = CMD_SEND_ALL_MSG;
	strcpy( p.u.kein.default_msg, text );
	p.h.header.size = len+1  ;
	QueuePacket( &p, 1 );
}

void RecvNationMassage( char *text )
{
	MP3( SN_EAR_MESSAGE_RECV );
	recv_nation_msg_count = 250;
	strcpy( recv_nation_msg_text, text );
}

void RecvYouAreKing( int is_king )
{
	if( is_king )
	{
		MP3( SN_LOGOUT );
		AddCurrentStatusMessage( 255,255,255,lan->OutputMessage(3,69) );//010215 lsw
		AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(3,70));//010215 lsw
		AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(3,73));//010215 lsw
		Hero->name_status.nation = 1;
	}
	else
	{
		MP3( SN_LOGOUT );
		AddCurrentStatusMessage( 255,0,0, lan->OutputMessage(3,71));//010215 lsw
		AddCurrentStatusMessage( 255,0,0, lan->OutputMessage(3,72));//010215 lsw
		AddCurrentStatusMessage( 255,0,0, lan->OutputMessage(3,73));//010215 lsw
		Hero->name_status.nation = 0;
	}
}


//////////////////////////////////////////////////////////////////////////////
//
//	길드 명령어들
//
void SendGuildExit( char * name )		// 길드원을 탈퇴시킨다.
{
	if( !CheckGuildPower( GDP_EXIT_GUILD_HIM, Hero ) )		// 권한이 있나?
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 146 ) );	// 권한이 없습니다.
		return;
	}
	LPCHARACTER ch = ExistHe( name );
	if( !ch )
	{
		// 서버로 날린다.		// 주변에 없을때...
		t_packet p;
		
		p.h.header.type = CMD_GUILD_EXIT_COMMAND_GLOBAL;
		strcpy( p.u.kein.default_name, name );
		p.h.header.size = strlen( name )+1;
		QueuePacket( &p, 1 );
		return;
		/*
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,82) );//010215 lsw
		return;
		*/
	}
	if( strcmp( ch->name, Hero->name ) == 0 )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,83) );//010215 lsw
		return;
	}

	if( Hero->GetGuildCode() != ch->GetGuildCode() )
	{	//< CSD-030324
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,84) );//010215 lsw
		return;
	}	//> CSD-030324
	
	t_packet p;
	p.h.header.type = CMD_GUILD_EXIT_COMMAND;
	p.u.kein.default_short_int = ch->id;
	p.h.header.size = sizeof( short int );
	QueuePacket( &p, 1 );
}

void RecvGuildExistYou( )
{	//< CSD-031001
	MP3(SN_WARNING );
	AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,85) );//010215 lsw
	Hero->SetGuildCode(0);
	Hero->name_status.guild_master = 0;	
}	//> CSD-031001

void RecvGuildExistOk(  t_packet *p )
{	//< CSD-031001
	LPCHARACTER target = ReturnCharacterPoint( p->u.kein.default_short_int);
	if( !target ) return;
	
	MP3(SN_WARNING );
	AddCurrentStatusMessage( 200,60,60,lan->OutputMessage(3,86), target->name );//010215 lsw
	target->SetGuildCode(0);
	target->name_status.guild_master = 0;	
}	//> CSD-031001

void RecvGuildExistWho( t_packet *p )
{	//< CSD-031001
	LPCHARACTER target = ReturnCharacterPoint( p->u.kein.default_short_int );
	if( !target ) return;
	
	if( !CheckGuildCode( target->GetGuildCode() ) ) 
	{	
		return;
	}	
	 
	MP3(SN_WARNING );
	AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,87), target->name, g_GuildMgr[target->GetGuildCode()].m_szName ); // CSD-030324
	target->SetGuildCode(0); 
	target->name_status.guild_master = 0;
}	//> CSD-031001

// 길드 가입
void SendGuildJoin( char *name )
{
	if( !name[0] )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,88) );//010215 lsw
		return;
	}
	
	if( !CheckGuildPower( GDP_JOINING, Hero ) )		// 권한이 되나?
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 146 ) );
		//MP3( SN_WARNING );
		//AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,89) );//010215 lsw
		return;
	}
	LPCHARACTER ch = ExistHe( name );
	if( !ch )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,90) );//010215 lsw
		return;
	}
	if( strcmp( ch->name, Hero->name ) == 0 )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,91) );//010215 lsw
		return;
	}
	if( CheckGuildCode( ch->GetGuildCode() ) )
	{	//< CSD-030324
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,92) );//010215 lsw
		return;
	}	//> CSD-030324
	if( ch->name_status.nation != Hero->name_status.nation )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,93) );//010215 lsw
		return;
	}
	
	
	
	t_packet p;
	
	p.h.header.type = CMD_GUILD_JOIN_SERVER;
	p.u.kein.default_short_int = ch->id;
	p.h.header.size = sizeof( short int );
	QueuePacket( &p, 1 );
}

void RecvGuildJoinOk( t_packet *p )
{	//< CSD-031001
	int target_id = p->u.kein.guild_join_ok.target_id;
	int guild_code = p->u.kein.guild_join_ok.guild_code;
	
	LPCHARACTER target = ReturnCharacterPoint( target_id );
	if( !target ) return;
	
	MP3( SN_QUEST_END );
	AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,94), target->name, g_GuildMgr[guild_code].m_szName );
	
	target->SetGuildCode(guild_code);
	target->name_status.guild_master = 0;
}	//> CSD-031001

void RecvGuildJoinFail( t_packet *p )
{
	int target_id = p->u.kein.default_short_int;
	LPCHARACTER target = ReturnCharacterPoint( target_id );
	if( !target ) return;
	
	AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,95), target->name );//010215 lsw
}

// 020620 YGI
void RecvGuildJoinConfirm( t_packet *p )
{	//< CSD-030324
	const int guild_ma_id = p->u.kein.default_short_int;		// 길드 마스터
	LPCHARACTER guild_ma = ReturnCharacterPoint( guild_ma_id );
	
	if( !guild_ma ) {return;}
	if( !CheckGuildPower( GDP_JOINING, guild_ma ) ) return;		// 길드 마스터가 아닌경우
	
	ChatMgr.RecvNeedYesNoMsg( YN_GUILD_JOIN_OK, guild_ma_id, lan->OutputMessage(3,96), g_GuildMgr[guild_ma->GetGuildCode()].m_szName, guild_ma->name );//010215 lsw
}	//> CSD-030324

void SendGuildJoinConfirmOk( short int target_id )		// 길드 가입을 허락했다.
{
	t_packet p;
	
	p.h.header.type = CMD_GUILD_JOIN_CONFIRM_OK;
	p.u.kein.default_short_int = target_id;
	p.h.header.size = sizeof( short int );
	QueuePacket( &p, 1 );
}

void SendGuildJoinConfirmNot( short int target_id )		// 길드 가입을 거부했다.
{
	t_packet p;
	
	p.h.header.type = CMD_GUILD_JOIN_CONFIRM_NOT;
	p.u.kein.default_short_int = target_id;
	p.h.header.size = sizeof( short int );
	QueuePacket( &p, 1 );
}

////////////////////
// 적 스켄
int scan_target_count;		// 보여줄 갯수
k_x_y scan_target_xy[MAX_ENEMY_SCAN_TARGET];
int scan_target_view_time;

void RecvScanTarget( t_packet *p )
{
	scan_target_view_time = 500;
	scan_target_count = p->u.kein.scan_target.count;
	
	memcpy( scan_target_xy, p->u.kein.scan_target.xy, sizeof( k_x_y )*scan_target_count );
	AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,101) );//010215 lsw
}

void SendGuildAllMsg( char *msg )		// 길드 공지
{
	ConvString( msg );
	if( !msg[0] )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,97) );//010215 lsw
		return;
	}
	
	if( !CheckGuildPower( GDP_BBS, Hero ) )		// 권한이 되나?
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 146 ) );
		//MP3( SN_WARNING );
		//AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,98) );//010215 lsw
		return;
	}
	
	t_packet p;
	
	p.h.header.type = CMD_GUILD_ALL_MSG;
	strcpy( p.u.kein.default_msg, msg );
	p.h.header.size = strlen(msg)+1;
	QueuePacket( &p, 1 );
}

void  SendGuildChangeEachLevel( char *text )		// 길드원의 등급을 바꾼다.
{
	if( !CheckGuildPower( GDP_CHANGE_DEGREE, Hero ) )		// 권한이 되나?
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 146 ) );		// 2이상만 가능
		return;
	}
	
	AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,99) );//010215 lsw
	
	int each_level_to_real_level[] = { 0, 1, 2, 3, 4, 0 };
	LPCHARACTER target;
	
	int lv;
	char name[20];
	if( text[0] == NULL ) goto ERR_;
	char *token;
	token = strtok( text, " " );
	if( !token ) goto ERR_;
	lv = atoi( token );
	if( !lv ) goto ERR_;
	
	token = strtok( NULL, " \n" );
	if( !token ) goto ERR_;
	
	strncpy( name, token, 19 );
	name[19] = 0;
	
	if( lv <= 0 || lv > 5 ) 
	{
		AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,100) );//010215 lsw
		return;
	}
	if( name[0] == NULL ) 
	{
		AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,102) );//010215 lsw
		return;
	}
	target = ExistHe( name );
	if( !target ) 
	{
		AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,103) );//010215 lsw
		return;
	}
	if (target->GetGuildCode() != Hero->GetGuildCode())
	{	//< CSD-030324
		AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,104) );//010215 lsw
		return;
	}	//> CSD-030324
	if( target == Hero )
	{
		AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,105) );//010215 lsw
		return;
	}
	
	if( target->name_status.guild_master == each_level_to_real_level[lv] )
	{
		AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,106) );//010215 lsw
		return;
	}
	
	if( lv == 1 )
	{
		AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,107) );//010215 lsw
		return;
	}
	
	t_packet p;			// 서버로 전송
	
	p.h.header.type = CMD_GUILD_EACH_LEVEL;
	p.u.kein.guild_each_level.target_id = target->id;
	p.u.kein.guild_each_level.each_level = lv;
	p.h.header.size = sizeof( k_guild_each_level );
	QueuePacket( &p, 1 );
	return;
	
	
ERR_ :
	AddCurrentStatusMessage( 160,160,250, lan->OutputMessage(3,108) );//010215 lsw
	return;
	
	
}

void RecvGuildEachLevel( t_packet *p )		// 누군가 등급이 바꼈다.
{
	int each_level = p->u.kein.guild_each_level.each_level;
	int target_id =  p->u.kein.guild_each_level.target_id;
	
	LPCHARACTER target = ReturnCharacterPoint( target_id );
	if( !target ) return;
	
	int each_level_to_real_level[] = { 0, 1, 2, 3, 4, 0 };
	int old_level = target->name_status.guild_master;
	target->name_status.guild_master = each_level_to_real_level[each_level];
	
	
	if( target == Hero )		// 자기 자신이면
	{	//< CSD-030324
		int real_level_to_each_level[]={ 5, 1, 2, 3, 4};
		if( each_level < real_level_to_each_level[old_level] ) // 승격 
		{
			AddCurrentStatusMessage( 60,60,250,lan->OutputMessage(3,109), CallGuildDegreeName( target->GetGuildCode(), target->name_status.guild_master ) );//010215 lsw
		}
		else
		{
			AddCurrentStatusMessage( 60,60,250, lan->OutputMessage(3,110), CallGuildDegreeName( target->GetGuildCode(), target->name_status.guild_master ) );//010215 lsw
		}
	}	//> CSD-030324
	else
	{
		AddCurrentStatusMessage( 60,60,250, lan->OutputMessage(3,111), target->name );//010215 lsw
	}	
}

void RecvGuildJoinConfirmIng( )
{
	AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,112));//010215 lsw
}

// gm에 등록할만한 자격이 안되는 아이 중 몇몇만 추려서 서버로 보내 화일로 만든다.
void CheckAndSendGmRegist( int guild_count)	
{
	static char name[21]; 
	if( strcmp( name, SCharacterData.sCharacterName ) == 0 )		// 이미 전송했다.
	{
		return;
	}
	//011012 lsw >
	if( SCharacterData.nJobSkill[guild_count].skillexp >= 950000 )	// 스킬중 어느것이 95 이상인지...
		//011012 lsw <
	{
		SendGMRegist( guild_count, CMD_GM_REGIST2 );
		strcpy( name, SCharacterData.sCharacterName );
	}
	//strcpy( name, SCharacterData.sCharacterName );
}

void SendGMRegist( int guild_count, int type  )
{
	t_packet p;			// 서버로 전송
	
	p.h.header.type = type;
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		p.h.header.type = CMD_GM_REGIST_GAMEMAKEMODE;
	}
#endif
	p.u.kein.gm_regist.gm_list_index= guild_count;
	p.h.header.size = sizeof( k_gm_regist );
	QueuePacket( &p, 1 );
	return;
}

void RecvBbsAll( t_packet *p )
{
	int type = p->u.kein.bbs_all.type;		// 어떤 공지사항이냐?
	char *name = p->u.kein.bbs_all.name;
	
	switch( type )
	{
	case BA_GM_REGIST : ShowComeOutGM( name ); break;
	}
}

// 보여주는 직업 바꾸기
int GetJobNumberByName( char *name )
{//021030 lsw
	for( int i=0; i< MAX_GM_MAIN;i++ )
	{
		if( ::strcmp( g_GmMain[i].name, name ) == 0 ) return i;//021111 lsw
	}
	return -1;
}

int GetNationByNationName( char *text )
{
	int ret = -1;
	if( !text[0] ) return ret;
	
	for( int i=3; i<7; i++ )
	{
		if( strcmp( NationName[i], text ) == 0 ) return i;
	}
	return ret;
}

void SendReqStopWar( char *text )
{
	if( Hero->name_status.king != 1 ) return;		// 지도자만 사용할수 있다.
	int nation = GetNationByNationName( text );
	
	if( nation == -1 || nation == Hero->name_status.nation )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(3,114) );//010215 lsw
		return;
	}
	
	t_packet p;			// 서버로 전송
	p.h.header.type = CMD_REQ_STOP_WAR_TO_LOGIN;
	p.u.kein.default_char = nation;
	p.h.header.size = sizeof( char );
	QueuePacket( &p, 1 );
	
	AddCurrentStatusMessage( 255, 255, 255, lan->OutputMessage(3,115) );//010215 lsw
}

int ScanTarget( int nation )		// 적을 스켄
{
	if( Hero->name_status.nation != nation ) return 0;
	
	//acer6
	static DWORD time = 0;
	if( (g_curr_time - time) < 20 ) return 0;
	time = g_curr_time;
	
	if (!Hero->JoinLocalWar) return 0;
	if( ViewSmallMap2(0,0) == 0 )		// 지도가 없다.
	{
		AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,117) );//010215 lsw
		return 1;
	}
	
	main_interface.data[IF_MAP] = true;
	CallMenu(  MN_SMALL_MAP );
	SMenu[MN_SMALL_MAP].nTemp = 0;
	
	CallServer( CMD_SCAN_TARGET );
	return 1;
}

void RecvScriptMessageBox( t_packet *p )
{
	KeinMessageBox( 0, p->u.kein.default_msg );		// 020701 YGI
}

void SendSalvationMoney( DWORD money )
{
	t_packet p;			// 서버로 전송
	
	p.h.header.type = CMD_SALVATION_MONEY;
	p.u.kein.default_dword= money;
	p.h.header.size = sizeof( DWORD );
	QueuePacket( &p, 1 );
}

void RecvRepVationMoney( t_packet *p )
{
	DWORD money = p->u.kein.default_dword;
	TotalSalvation = money;
}

void RecvSalvationTotalMoneyMsg( t_packet *p )
{
	int nation = p->u.kein.total_salvation_all_msg.nation;
	DWORD money = p->u.kein.total_salvation_all_msg.money;
	double totalmoney = p->u.kein.total_salvation_all_msg.total_money;
	char *name = p->u.kein.total_salvation_all_msg.name;
	
	if( !CheckNation( nation ) ) return;
	
	MP3( SN_BUY_HOUSE );
#ifdef CHANGE_MONEY_
	AddCurrentStatusMessage( 255,255, 255, lan->OutputMessage(3,120), NationName[nation], name, money, totalmoney);//010215 lsw
	AddCurrentStatusMessage( 255,255, 255, lan->OutputMessage(3,121) );//010215 lsw
#else
	AddCurrentStatusMessage( 255,255, 255, lan->OutputMessage(3,122), NationName[nation], name, money, totalmoney);//010215 lsw
	AddCurrentStatusMessage( 255,255, 255, lan->OutputMessage(3,123) );//010215 lsw
#endif
}

void SendChangeMoney( DWORD old_money )
{
	t_packet p;			// 서버로 전송
	
	p.h.header.type = CMD_CHANGE_MONEY;
	p.u.kein.chang_money.money = old_money;
	p.h.header.size = sizeof( k_chang_money );
	QueuePacket( &p, 1 );
}

void RecvChangeMoney( t_packet *p )
{
	DWORD money = p->u.kein.default_dword;
#ifdef CHANGE_MONEY_
	AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,124) );//010215 lsw
#else
	AddCurrentStatusMessage( 200,60,60, lan->OutputMessage(3,125));//010215 lsw
#endif
}

void RecvEatChocolateOk( t_packet *p )
{
	int t = p->u.kein.default_int;
	AddCurrentStatusMessage( 200,60,60,lan->OutputMessage(3,126), t );//010215 lsw
}

void RecvRumor( t_packet *p )
{
	char *name = p->u.kein.check_chocolate.name;
	int chocolate_count = p->u.kein.check_chocolate.count;
	
	if( strcmp( name, SCharacterData.sCharacterName) == 0 )
	{
		sprintf(SMenu[MN_DRINK_RUMOR_VIEW].nField[0].temp,lan->OutputMessage(3,127), chocolate_count);//010215 lsw
	}
	else sprintf(SMenu[MN_DRINK_RUMOR_VIEW].nField[0].temp,lan->OutputMessage(3,128), name, chocolate_count);//010215 lsw
}

void RecvRumorNot()
{
	sprintf(SMenu[MN_DRINK_RUMOR_VIEW].nField[0].temp,lan->OutputMessage(3,129));//010215 lsw
}

void RecvTEAMBATTLE_INFO()		// 팀배틀 정보를 얻어 왔다.
{
	SMenu[MN_ARENA_LOBBY].key = 1;
	g_BattleStatusView = true;
}

void RecvOpenSchoolResult( t_packet *p )		// 010815 YGI
{
	int result = p->u.kein.openschool_data_result.number;
	int npc_id = p->u.kein.openschool_data_result.npc_id+10000;
	
	t_server_chat_data data;
	
	data.server_id = npc_id;
	data.type = CHATTYPE_NORMAL;
	char *text;
	//1 : 영화, 2, 캐리비안, 0 : 꽝 
	static int caribian[] = { 
		4, 3, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 3, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 3, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 3, 4, 2, 2, 2, 2, 4, 
			3, };
		
		if( result <= 0 )
		{
			// 당신은 이미 등록되어 있군요., 더이상 가져 와도 소용없습니다. 다른 사람에게 주는 것도 좋은 방법인데..
			text = kein_GetMenuString( 5 );
			data.length = strlen( text );
			strcpy( data.text, text );
		}
		else if( result >= 41 )
		{
			sprintf( data.text, kein_GetMenuString( 4 ) );
			data.length = strlen( data.text );
		}
		else
		{
			sprintf( data.text, kein_GetMenuString( caribian[result] ) );
			data.length = strlen( data.text );
		}
		
		RecvChatData( &data );
}

void RecvLoanInfo( t_packet *p )
{
	SCharacterData.LoanTotal = p->u.kein.loan_info.max_loan;
	SCharacterData.LastLoan = p->u.kein.loan_info.last_loan;
	
	g_RecvLoanInfo = 1;
}

void SendReqLoan( DWORD loan )
{
	t_packet p;			// 서버로 전송
	
	p.h.header.type = CMD_REQ_LOAN;
	p.u.kein.default_dword= loan;
	p.h.header.size = sizeof( DWORD );
	QueuePacket( &p, 1 );
}

void SendReqRepayment( DWORD repayment )
{
	t_packet p;			// 서버로 전송
	
	p.h.header.type = CMD_REQ_REPAYMENT;
	p.u.kein.default_dword= repayment;
	p.h.header.size = sizeof( DWORD );
	QueuePacket( &p, 1 );
}


void RecvMerchantMessage( t_packet *p )
{	//< CSD-030723
	char *name = p->u.kein.name_message.name;
	char *message = p->u.kein.name_message.message;

	// 031021 kyo	
	if( !ChatMgr.IsActiveBBS() || ChatMgr.IsLimitedBBSName( name ) )//제한
	{
		return;
	}


	MP3( SN_RECV_BBS );
	AddCurrentStatusMessage( 0,174,239, kein_GetMenuString( 20 ), name );
	AddCurrentStatusMessage( 0,174,239, "%s",message );
}	//> CSD-030723

void SendMerchantItemMessage( char *item_name, DWORD money, int map_type )		// 아이템 공지 내용을 보낼때,
{
	ConvString(item_name);
	t_packet p;			// 서버로 전송
	
	if( map_type ) p.h.header.type = CMD_MERCHANT_ITEM_BBS_ALL;
	else p.h.header.type = CMD_MERCHANT_ITEM_BBS;
	
	{
		p.u.kein.merchant_item_bbs.dwMoney = money;
		strcpy( p.u.kein.merchant_item_bbs.szItemName, item_name );
	}
	p.h.header.size = sizeof( k_merchant_item_bbs )-20+strlen(item_name);
	QueuePacket( &p, 1 );
}

void RecvMerchantItemBbs( t_packet *p )
{
	DWORD	dwMoney		= p->u.kein.recv_merchant_item_bbs.dwMoney;
	int		map_number	= p->u.kein.recv_merchant_item_bbs.map_number;
	int		pos_x		= p->u.kein.recv_merchant_item_bbs.pos_x;
	int		pos_y		= p->u.kein.recv_merchant_item_bbs.pos_y;
	char *	szItemName	= p->u.kein.recv_merchant_item_bbs.szItemName;
	char *	szCharName	= p->u.kein.recv_merchant_item_bbs.szCharName;
	
	MP3( SN_RECV_BBS );	
	
	AddCurrentStatusMessage( 0,174,239, kein_GetMenuString( 26 ), szCharName );			//	## '%s'님의 [아이템공지] ##
	AddCurrentStatusMessage( 0,174,239, kein_GetMenuString( 27 ), szItemName, dwMoney );				//	'%s' 아이템을 %u 가격에 팔기를 원합니다.
	AddCurrentStatusMessage( 0,174,239, kein_GetMenuString( 28 ), MapInfo[map_number].mapname, pos_x, pos_y );		//	현재 [%s](%d, %d)에 있는 경매상인근처에 있습니다.
}

void RecvAliveFailResult( t_packet *p )
{
	int type = p->u.kein.default_char;
	switch( type )
	{
	case 1:
		AddCurrentStatusMessage( 255,255,255, kein_GetMenuString( 30 ) );
		break;
	case 2:
		AddCurrentStatusMessage( 255,255,255, kein_GetMenuString( 31 ) );
		break;
	}
}

bool g_bSendLadderRank;		// 랭크에 관련된 서버 요청... 요청후에 재 요청하지 않도록 한다.
void SendReqLadderRank( int page )
{
	if( g_bSendLadderRank ) return;
	g_bSendLadderRank = 1;
	
	t_packet p;
	p.h.header.type = CMD_REQ_RANK_LADDER;
	p.u.kein.default_int = page;
	p.h.header.size = sizeof( int );
	QueuePacket( &p, 1 );
}

void RecvRankLadderData( t_packet *p )
{
	g_bSendLadderRank = 0;
	k_db_get_ladder_score_rank	*t = &p->u.kein.db_get_ladder_score_rank;
	g_lpRankLadderBBS->AddData( t->is_end, t->unit );
	SMenu[MN_LADDER_RANK].nField[1].nSHideNomalStart = t->unit[0].rank/10;
	SMenu[MN_LADDER_RANK].nField[1].nSHideNomalCount = t->unit[0].rank/10+1;
}

void RecvRankLadderFinish( )
{
	g_bSendLadderRank = 0;
	g_lpRankLadderBBS->SetFinish();
}

void SendDirectDB( int type, void *msg, int size )
{
	g_bSendLadderRank = 0;
	
	t_packet p;
	p.h.header.type = CMD_DIRECT_CLIENT_TO_DB;
	p.u.kein.send_db_direct_client.type = type;
	if( msg )
	{
		memcpy( p.u.kein.send_db_direct_client.data, msg, size );
	}
	else size = 0;
	p.h.header.size = size+2;
	QueuePacket( &p, 1 );
}

void RecvSearchRankLadderFail()
{
	MP3( SN_WARNING );
	g_bSendLadderRank = 0;
	if( SMenu[MN_LADDER_RANK].bActive )
		CallOkCancelMessageBox( MN_LADDER_RANK, 0, 0, kein_GetMenuString(34),0 );//010215 lsw
}

void RecvtEffectByScript( t_packet *p )
{
	int target_id = p->u.kein.send_effect.target_id;
	int effect_num = p->u.kein.send_effect.effect_num;
	
	if( effect_num > 500 ) return;
	LPCHARACTER ch = ReturnCharacterPoint( target_id );
	if( !ch ) return;
	
	InsertMagic( ch, ch, effect_num, 0, 0, 0, 0, 0 );
}

k_visa_item NationVisa[7];
int SendReqVisa( int nation )
{
	if( NationVisa[nation].item_no[0] ) return true;	// 서버로 전송 안해도 된다.
	
	CallServer( CMD_REQ_VISA, &nation, sizeof( char ) );
	return false;		// 서버로 전송했다.
}

void RecvReqVisaItem( t_packet *p )
{
	int nation = SMenu[MN_VISA_MENU].key;
	if( SMenu[MN_VISA_MENU].bActive )
	{
		NationVisa[nation] = p->u.kein.visa_item;
	}
}

int		g_VisaBuyFlag;
void SendBuyVisa( int nation, int select )
{
	if( g_VisaBuyFlag ) return;
	
	t_packet packet;
	packet.h.header.type = CMD_BUY_VISA;
	packet.u.kein.visa_buy.item_no= NationVisa[nation].item_no[select];
	packet.u.kein.visa_buy.nation = nation;
	packet.h.header.size = sizeof( k_visa_buy );
	QueuePacket( &packet, 1 );
	
	// 얼마얼마에 사려고 합니다.
	AddCurrentStatusMessage( 255,255,0 , kein_GetMenuString(38), GetItemName(NationVisa[nation].item_no[select]), NationVisa[nation].item_price[select], g_szMoneyName );
	
	g_VisaBuyFlag = 1;
}

int CheckGuildName( char *name )
{
	double		StartTime, Duration;
	if( name == NULL ) 
	{
		return 1;
	}
	
	SendDirectDB( CMD_CHECK_GUILD_NAME, name, strlen( name )+1 );
	StartTime = ::timeGetTime( );
	ListenCommand = CMD_NONE;
	while( 1 )			
	{					
		Duration = ::timeGetTime( ) - StartTime;	// 5초를 기다린다. 
		if ( Duration > WAIT_TIME_RESPONSE_DURATION )		
		{				
			return 1; 	
		}				
		
		if ( HandleRunning(&connections )  <= 0 )
		{					
			Release(&connections );	
			return 1;
		}				
		else if( ListenCommand == CMD_THEREIS_NO_CHARNAME )
		{	
			return 0;
		}	
		else if( ListenCommand == CMD_THEREIS_CHARNAME )
		{	
			return 1;
		}	
	}			
}

int CheckOtherCharacterGuild()
{
	double	StartTime, Duration;
	
	SendDirectDB( CMD_CHECK_GUILD_OTHER_CHARACTER, Hero->name, strlen( Hero->name )+1 );
	StartTime = ::timeGetTime( );
	ListenCommand = CMD_NONE;
	while( 1 )			
	{					
		Duration = ::timeGetTime( ) - StartTime;	// 5초를 기다린다. 
		if ( Duration > WAIT_TIME_RESPONSE_DURATION )		
		{				
			return 1; 	
		}				
		
		if ( HandleRunning(&connections )  <= 0 )
		{					
			Release(&connections );	
			return 1;
		}				
		else if( ListenCommand == CMD_THEREIS_NO_CHARNAME )
		{	
			return 0;
		}	
		else if( ListenCommand == CMD_THEREIS_CHARNAME )
		{	
			return 1;
		}	
	}
}

void SendRegistGuildImsi( char *guild_name, char *guild_explain )
{
	if( CheckGuildCode( Hero->GetGuildCode()) ) 
	{	//< CSD-030324
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 50 ) );
		return;
	}	//> CSD-030324
	if( Hero->lv < 40 ) 
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 51 ) );
		return;
	}
	if( GetItemMuchByMuchItem( IN_BLUE_SIGNET ) < 40 )		// 파란 시그넷 수량
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 52 ) );
		return;
	}
	
	if( CheckGuildName( guild_name ) )
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 104 ) );
		return ;
	}
	
	t_packet packet;
	packet.h.header.type = CMD_REGIST_GUILD_IMSI;
	strcpy( packet.u.kein.regist_guild_imsi.guild_name ,  guild_name );
	strcpy( packet.u.kein.regist_guild_imsi.guild_explain , guild_explain );
	packet.h.header.size = sizeof( k_regist_guild_imsi ) - 1024 + strlen( guild_explain );
	QueuePacket( &packet, 1 );
}

void RecvWhoRegistGuildOk( t_packet *p )
{	//< CSD-031001
	int who_id = p->u.kein.send_regist_imsi_guild_info.server_id;
	int guild_code = p->u.kein.send_regist_imsi_guild_info.guild_code;
	int make = p->u.kein.send_regist_imsi_guild_info.make;
	char *guild_name = p->u.kein.send_regist_imsi_guild_info.guild_name;
	
	LPCHARACTER ch = ReturnCharacterPoint( who_id );
	if( !ch ) return;
	
	g_save_load_flag = 1;		// 종료시 새로 저장한다.
	
	ch->SetGuildCode(guild_code);
	ch->name_status.guild_master = 1;
	g_GuildMgr[guild_code].first = 1;
	g_GuildMgr[guild_code].mark_num = 1;
	g_GuildMgr[guild_code].make = make;
	strcpy( g_GuildMgr[guild_code].m_szName, guild_name );
	g_GuildMgr[guild_code].m_nCode = guild_code;
	
	if( ch == Hero )
	{
		Kein_PutMessage( KM_OK, kein_GetMenuString( 44 ) );
		Kein_PutMessage( KM_OK, kein_GetMenuString( 45 ) );
	}
	else
	{
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 46 ), ch->name, guild_name );
	}
	return;
}	//> CSD-031001

const int MAX_GUILD = SHRT_MAX;

void SendReqGuildInfo1( int guild_code )
{	//< CSD-030423
	if (guild_code < 0 || guild_code > MAX_GUILD)
	{	
		g_DBGLog.Log(LOG_LV1,"Invaild guild code : %d", guild_code);
		return;
	}	

	t_packet packet;
	packet.h.header.type = CMD_GUILD_INFO1_REQ;
	packet.u.kein.req_guild_info1.guild_code = guild_code;
	packet.u.kein.req_guild_info1.make = g_GuildMgr[guild_code].make;
	packet.u.kein.req_guild_info1.mark_num = g_GuildMgr[guild_code].mark_num;
	packet.u.kein.req_guild_info1.info_num = g_GuildMgr[guild_code].info_num;
	packet.u.kein.req_guild_info1.first = g_GuildMgr[guild_code].first;
	packet.h.header.size = sizeof( k_req_guild_info1 );
	QueuePacket( &packet, 1 );
	g_GuildMgr[guild_code].m_bInfo = 1;
}	//> CSD-030423

void RecvGuildInfoName( t_packet *p )
{	//< CSD-030324
	g_save_load_flag = 1;		// 종료시 새로 저장한다.
	
	int guild_code	= p->u.kein.guild_info_name.guild_code	;
	int make		= p->u.kein.guild_info_name.make		;
	int first		= p->u.kein.guild_info_name.first		;
	char *guild_name= p->u.kein.guild_info_name.guild_name	;
	
	g_GuildMgr[guild_code].m_nCode = guild_code;
	g_GuildMgr[guild_code].make = make;
	g_GuildMgr[guild_code].first = first;
	strcpy( g_GuildMgr[guild_code].m_szName, guild_name );
}	//> CSD-030324

void RecvGuildExplain( t_packet *p )
{	//< CSD-030324
	int guild_code = p->u.kein.guild_explain.guild_code;
	char *explain = p->u.kein.guild_explain.explain;
	
	SAFE_DELETE(g_GuildMgr[guild_code].m_szExplain);
	
	
	int len = strlen( explain );
	g_GuildMgr[guild_code].m_szExplain = new char[len+1];
	strcpy( g_GuildMgr[guild_code].m_szExplain, explain );
	
	if( SMenu[MN_GUILD_REGIST].bActive )
	{
		SMenu[MN_GUILD_REGIST].nField[5].m_lpTemp = g_GuildMgr[guild_code].m_szExplain;
	}
}	//> CSD-030324

void SendGuildMark( int guild_code, Spr *guild_mark )
{
	if( !guild_mark->img ) return;
	
	if( !guild_code ) return;
	
	
	DWORD size = guild_mark->size;
	
	int count = size/IMAGE_UNIT_SIZE+ 1;
	
	k_regist_guild_mark regist_guild_mark;
	
	regist_guild_mark.guild_code = guild_code;
	regist_guild_mark.total = count;
	regist_guild_mark.size = size;
	regist_guild_mark.xl = guild_mark->xl;
	regist_guild_mark.yl = guild_mark->yl;
	
	int sub_size;
	for( int i=1; i<=count; i++ )
	{
		if( i == count ) sub_size = size % IMAGE_UNIT_SIZE;
		else sub_size = IMAGE_UNIT_SIZE;
		
		regist_guild_mark.sub_count = i;
		memcpy( regist_guild_mark.img, guild_mark->img+IMAGE_UNIT_SIZE*(i-1), sub_size );
		
		int packet_size = sizeof( k_regist_guild_mark )-IMAGE_UNIT_SIZE + sub_size;
		
		SendDirectDB( CMD_REGIST_GUILD_MARK, &regist_guild_mark, packet_size );
	}
}


struct k_image_header
{
	DWORD size;
	short int xl;
	short int yl;
};

char *imsi_guild_mark_image;
void RecvGuildMarkImage( t_packet *p )
{
	int guildCode = p->u.kein.guild_image.guild_code;
	int total = p->u.kein.guild_image.total;
	int subCount = p->u.kein.guild_image.sub_count;
	int mark_num = p->u.kein.guild_image.mark_num;
	char *img = p->u.kein.guild_image.img;
	
	DWORD total_size = 0;
	if( subCount == 1 )
	{
		memcpy( &total_size, img, sizeof( DWORD ) );
		total_size += sizeof( k_image_header );
		
		SAFE_DELETE(imsi_guild_mark_image);
		imsi_guild_mark_image = new char[total_size];
	}
	else
	{
		memcpy( &total_size, imsi_guild_mark_image, sizeof(DWORD ) );
		total_size += sizeof( k_image_header );
	}
	
	int subSize = 0;
	if( subCount == total )
	{
		subSize = total_size % IMAGE_UNIT_SIZE;
		memcpy( imsi_guild_mark_image+(subCount-1)*IMAGE_UNIT_SIZE, img, subSize );
		k_image_header imageHeader;
		memcpy( &imageHeader, imsi_guild_mark_image, sizeof( k_image_header ) );
		
		CGuild *curr_guild = &g_GuildMgr[guildCode]; // CSD-030324
		curr_guild->mark_num = mark_num;
		curr_guild->m_Spr.xl = imageHeader.xl;
		curr_guild->m_Spr.yl = imageHeader.yl;
		curr_guild->m_Spr.size = imageHeader.size;
		curr_guild->m_Spr.ox = curr_guild->m_Spr.xl >> 1;
		curr_guild->m_Spr.oy = curr_guild->m_Spr.yl >> 1;
		SAFE_DELETE(curr_guild->m_Spr.img);
		SAFE_DELETE(curr_guild->m_Spr_save.img);
		
		curr_guild->m_Spr.img = new char[imageHeader.size];
		memcpy( curr_guild->m_Spr.img, imsi_guild_mark_image+sizeof( k_image_header ), imageHeader.size );
		
		CopySprToSpr( &curr_guild->m_Spr_save, &curr_guild->m_Spr );
		convert565to555( &curr_guild->m_Spr );
		
		curr_guild->save_flag = true;
		g_save_load_flag = true;
		
		SAFE_DELETE(imsi_guild_mark_image);
	}
	else 
	{
		subSize = IMAGE_UNIT_SIZE;
		memcpy( imsi_guild_mark_image+(subCount-1)*IMAGE_UNIT_SIZE, img, subSize );
	}
}

//021030 YGI
void SendRegistGuild( int guild_code, char *szSubMasterName )
{
	k_guild_regist guildRegist;
	
	for( int i=0; i<5; i++ )
	{
		g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[i][19] = 0;
		EatRearWhiteChar(g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[i] );
	}
	
	guildRegist.guild_code = guild_code;
	strcpy( guildRegist.sub_name, szSubMasterName  );
	
	strcpy( guildRegist.guild_degree[0], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[4] );
	strcpy( guildRegist.guild_degree[1], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[0] );
	strcpy( guildRegist.guild_degree[2], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[1] );
	strcpy( guildRegist.guild_degree[3], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[2] );
	strcpy( guildRegist.guild_degree[4], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[3] );
	
	SendPacketDefault(CMD_REGIST_GUILD, &guildRegist, sizeof( k_guild_regist ));
} 

//021030 YGI
void SendChangeGuildDegree( int guild_code )
{
	k_guild_change_degree guildRegist;
	
	for( int i=0; i<5; i++ )
	{
		g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[i][19] = 0;
		EatRearWhiteChar(g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[i] );
	}
	
	guildRegist.guild_code = guild_code;
	strcpy( guildRegist.guild_degree[0], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[4] );
	strcpy( guildRegist.guild_degree[1], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[0] );
	strcpy( guildRegist.guild_degree[2], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[1] );
	strcpy( guildRegist.guild_degree[3], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[2] );
	strcpy( guildRegist.guild_degree[4], g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[3] );
	
	SendDirectDB(CMD_CHANGE_GUILD_DEGREE, &guildRegist, sizeof( k_guild_change_degree ));
} 

void RecvGuildDegreeInfo( t_packet *p )
{	//< CSD-030324
	int guildCode = p->u.kein.guild_degree_info.guild_code;
	g_GuildMgr[guildCode].info_num = p->u.kein.guild_degree_info.info_num;
	g_save_load_flag = true;
	memcpy( g_GuildMgr[guildCode].m_aDegree, p->u.kein.guild_degree_info.guild_degree, sizeof( char) *5*20 );
}	//> CSD-030324

void SendReqGuildIntroduction( int nation )
{
	SAFE_DELETE(g_Guild_Introduction);
	g_Guild_Introduction = new CGuildIntroduction;
	g_Guild_Introduction->m_Nation = nation;
	CallServer( CMD_REQ_GUILD_INTRODUCTION );
}

void RecvReqGuildIntroduction( int is_first, t_packet *p )
{
	if( !g_Guild_Introduction ) return;
	int count = p->u.kein.guild_instroduction.count;
	int first = 0;
	if( is_first ) g_Guild_Introduction->m_Count = count; 
	else
	{
		first = g_Guild_Introduction->m_Count;
		g_Guild_Introduction->m_Count += count;
	}
	
	static int box_x = 280;	// 길드 이미지 번호 32번의 검정 박스의 크기
	//static int box_y = 120;
	
	CGuildIntroductionGuild *pGuild = 0;
	CGuildIntroductionGuild *pGuildPrev;
	int ct = 0;
	for( int i=first; i<g_Guild_Introduction->m_Count; i++ )
	{
		pGuild = &g_Guild_Introduction->m_Guild[i];
		
		pGuild->m_nCode = p->u.kein.guild_instroduction.guild[ct].guild_code;
		strcpy(pGuild->m_szName, p->u.kein.guild_instroduction.guild[ct].guild_name);
		//< CSD-030401
		strcpy(g_GuildMgr[pGuild->m_nCode].m_szName, 
			   p->u.kein.guild_instroduction.guild[ct].guild_name);
		//> CSD-030401
		int len = strlen( pGuild->m_szName );
		pGuild->size = (len+2)*FONTSIZE_;
		
		if( i == 0 )
		{
			pGuild->x = 0;
			pGuild->line = 0;
			pGuild->page = 0;
		}
		else
		{
			pGuildPrev = &g_Guild_Introduction->m_Guild[i-1];
			pGuild->x = pGuildPrev->x+pGuildPrev->size;
			if( pGuild->x+pGuild->size > box_x )
			{
				pGuild->x = 0;
				pGuild->line = pGuildPrev->line+1;
				if( pGuild->line > 5 )
				{
					pGuild->line = 0;
					pGuild->page = pGuildPrev->page+1;
				}
				else pGuild->page = pGuildPrev->page;
			}
			else 
			{
				pGuild->line = pGuildPrev->line;
				pGuild->page = pGuildPrev->page;
			}			
		}
		SetRect( pGuild->rect, pGuild->x, pGuild->line*20, pGuild->x+pGuild->size, (pGuild->line+1)*20 );
		ct++;
	}
	if( pGuild )
	{
		int &page_status = SMenu[MN_GUILD_INTRODUCTION].work;
		WORD page;
		WORD page_max;
		
		page = 0;
		page_max = pGuild->page + 1;
		page_status = MAKELONG( page, page_max );
	}
}

void RecvGetGuildMasterName( t_packet *p )
{
	k_req_guild_master_and_sub_master *pInfo = &p->u.kein.req_guild_master_and_sub_master;
	CGuild *pCurrGuild = &g_GuildMgr[pInfo->guild_code]; // CSD-030324

	switch( pInfo->type )
	{
	case 1 :		// 길마만
		strcpy(pCurrGuild->m_szGuldMasterName, pInfo->name1);
		break;
	case 2 :		// 부길마만
		strcpy( pCurrGuild->m_szSubGuldMasterName, pInfo->name1 );
		break;
	case 3 :		// 둘다
		strcpy( pCurrGuild->m_szGuldMasterName, pInfo->name1 );
		strcpy( pCurrGuild->m_szSubGuldMasterName, pInfo->name2 );
		break;
	}
	
	if( SMenu[MN_GUILD_CHANGE_INFO].bActive )
	{
		if( IsSubMaster(Hero) )
		{
			// 길드 마스터 변경 버튼 active시킨다.
			SMenu[MN_GUILD_CHANGE_INFO].nField[4].nType=FT_PUT_BUTTON;
		}	
	}	
}

void RecvPutMenuString( t_packet *p )
{
	int type = p->u.kein.put_menu_string.type;
	int num = p->u.kein.put_menu_string.str_num;
	
	Kein_PutMessage( type, kein_GetMenuString( num ) );
}

void RecvRegistGuildImsiFail( t_packet *p )
{
	int type = p->u.kein.default_char;
	switch( type )
	{
	case 1 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 53 ) ); break;
	case 2 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 54 ) ); break;
	case 3 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 55 ) ); break;
	case 4 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 62 ) ); break;
	}
}

void RecvRegistGuildCheckOk()
{
	CloseAllMenu();
	g_Menu_Variable.m_nCheckSubMaster = CS_INPUT;		// 새로 서브 마스터를 등록하기 위한 메뉴를 띄운다.
	CallGuildExplain( GM_INPUT_SUB_MASTER );
}

void RecvRegistGuildCheckFail( t_packet *p )
{
	int result = p->u.kein.default_char;
	switch( result )
	{
	case 1 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 58 ) ); break;
	case 2 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 59 ) ); break;
	case 3 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 60 ) ); break;
	case 4 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 61 ) ); break;
	case 5 : Kein_PutMessage( KM_FAIL, kein_GetMenuString( 76 ), p->u.data[1] ); break;
	}
}

void RecvCheckGuildCode( t_packet *p )
{	//< CSD-031001
	k_guild_mail_unit *guild_info = (k_guild_mail_unit *)p->u.data;
	const int nGuildCode = guild_info->guild_code;
	Hero->SetGuildCode(nGuildCode);
	const int nDegree = guild_info->degree;
	Hero->name_status.guild_master = nDegree;
	
	if (Hero->GetGuildCode() != 0)
	{
		SendReqGuildInfo1(Hero->GetGuildCode());
	}
}	//> CSD-031001

void SendCheckSubGuildMaster( char *sub_master_name )
{
	k_guild_instroduction_basic check_info;
	int len = strlen( sub_master_name );
	check_info.guild_code = Hero->GetGuildCode(); // CSD-030324
	strcpy( check_info.guild_name, sub_master_name );
	SendDirectDB( CMD_CHECK_SUB_MASTER, &check_info, sizeof(k_guild_instroduction_basic)-20+len );
}

void RecvCheckSubMasterFail( t_packet *p )
{
	int result = p->u.kein.default_char;
	switch( result )
	{
	case 1:			// 같은 길드 사람이 아니거나 없는 캐릭터 이다.
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 63 ) ); break;
		
	case 2:			// 부길마 레벨이 30 이상이 아니다.
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 64 ) ); break;
	}
	
	switch( g_Menu_Variable.m_nCheckSubMaster )
	{
	case CS_INPUT : 
		CallGuildExplain( GM_INPUT_SUB_MASTER ); 
		break;
	}		
}

void RecvCheckSubMasterOk( t_packet *p )
{	//< CSD-030324
	char *name = p->u.kein.default_name;
	
	switch( g_Menu_Variable.m_nCheckSubMaster )
	{
	case CS_INPUT : 
		CallSmallMenu( MN_GUILD_REGIST_DEGREE );
		if( g_Menu_Variable.m_pSubGuildMaster )
			delete[] g_Menu_Variable.m_pSubGuildMaster;
		
		g_Menu_Variable.m_pSubGuildMaster = new char[20];
		strcpy( g_Menu_Variable.m_pSubGuildMaster, name );
		break;
		
	case CS_CHANGE_MASTER : 
		{
			// 길드 마스터 변경을 서버로 요청한다.
			CallServer( CMD_CHANGE_GUILD_MASTER );
			int guild_code = Hero->GetGuildCode(); 
			g_GuildMgr[guild_code].m_szGuldMasterName[0]=0;
			g_GuildMgr[guild_code].m_szSubGuldMasterName[0]=0;
			break;
		}
	case CS_CHANGE_SUB_MASTER : // 부길마를 db에 등록한다.
		{
			// 등록에 성공했습니다.
			Kein_PutMessage( KM_OK, kein_GetMenuString( 143 ) );
			SendChangeGuildMasterBySubMaster( CMD_REGIST_SUB_GUILD_MASTER, Hero->GetGuildCode(), name ); // CSD-030324
		}
	}
	return;
}	//> CSD-030324

void RecvRegistGuildOk( t_packet *p )
{
	int guild_code = p->u.kein.guild_instroduction_basic.guild_code;
	char *guild_name = p->u.kein.guild_instroduction_basic.guild_name;
	
	Kein_PutMessage( KM_OK, kein_GetMenuString( 69 ), guild_name );
	RecvRefreshGuild(guild_code);
}
void RecvRefreshGuild( int guild_code )
{	//< CSD-030324
	g_GuildMgr[guild_code].m_bInfo = 0;
}	//> CSD-030324

void RecvResetAbility( t_packet *p )
{
	SCharacterData.nCharacterAbility[STR]  = p->u.kein.reset_ability.Str;
	SCharacterData.nCharacterAbility[CON]  = p->u.kein.reset_ability.Con;
	SCharacterData.nCharacterAbility[DEX]  = p->u.kein.reset_ability.Dex;
	SCharacterData.nCharacterAbility[WIS]  = p->u.kein.reset_ability.Wis;
	SCharacterData.nCharacterAbility[INT_] = p->u.kein.reset_ability.Int;
	SCharacterData.nCharacterAbility[MOVP] = p->u.kein.reset_ability.MoveP;
	SCharacterData.nCharacterAbility[CHA]  = p->u.kein.reset_ability.Char;
	SCharacterData.nCharacterAbility[ENDU] = p->u.kein.reset_ability.Endu;
	SCharacterData.nCharacterAbility[MOR]  = p->u.kein.reset_ability.Moral;
	SCharacterData.nCharacterAbility[LUCK] = p->u.kein.reset_ability.Luck;
	SCharacterData.nCharacterAbility[WSPS] = p->u.kein.reset_ability.wsps;
	
	SCharacterData.LvUpPoint = p->u.kein.reset_ability.reserved_point;
}

void RecvDeleteGuild( t_packet *p )
{	//< CSD-031001
	int guild_code = p->u.kein.default_short_int;
	
	if( CheckGuildCode( Hero->GetGuildCode() ) )
	{	
		if( Hero->GetGuildCode() == guild_code )
		{
			Kein_PutMessage( KM_FAIL, kein_GetMenuString( 72 ) );
			Hero->SetGuildCode(0);
			Hero->name_status.guild_master = 0;
		}
	}	

	g_GuildMgr[guild_code].m_bActive = 0;
}	//> CSD-031001

void RecvGhostQuestInfo( t_packet *p )
{
	int ghost_quest_step = p->u.data[0];
	int ghost_quest_count = p->u.data[1];
	
	static int step = 0;
	static int count = 1;

#ifdef _DEBUG // finito 060507	 
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		ghost_quest_step = step;
		ghost_quest_count = count;
	}
#endif
	
	if( g_Menu_Variable.m_pRune )
	{
		g_Menu_Variable.m_pRune->m_nQuestStep = ghost_quest_step;
		g_Menu_Variable.m_pRune->m_nSelectRune = ghost_quest_count;
		SCharacterData.m_nQuestStep = ghost_quest_step;
		SCharacterData.m_nSelectRune = ghost_quest_count;
	}
	count ++;
	if( count > 24 ) 
	{
		step++;
		count = 0;
	}
	SMenu[MN_RUNE].nField[0].nSHideNomalStart = g_Menu_Variable.m_pRune->m_nQuestStep;
}

void SendChangeColor( int type1, int type2, int r, int g, int b )
{
	t_packet packet;
	packet.h.header.type = CMD_CHANGE_COLOR;
	packet.u.kein.change_color.type1 = type1;
	packet.u.kein.change_color.type2 = type2;
	packet.u.kein.change_color.r = r;
	packet.u.kein.change_color.g = g;
	packet.u.kein.change_color.b = b;
	packet.h.header.size = sizeof( k_change_color );
	QueuePacket( &packet, 1 );
}

void RecvChangeColor( t_packet *p )
{
	int server_id = p->u.kein.change_color_map2client.server_id;
	int type = p->u.kein.change_color_map2client.type;
	int r = p->u.kein.change_color_map2client.r;
	int g = p->u.kein.change_color_map2client.g;
	int b = p->u.kein.change_color_map2client.b;
	
	LPCHARACTER ch = ReturnCharacterPoint( server_id );
	if( !ch ) return;
	
	if( type )
	{
		SCharacterData.body_r = r;
		SCharacterData.body_g = g;
		SCharacterData.body_b = b;
		ch->bodycolor = ReturnBlendRGB( r, g, b );
	}
	else
	{
		SCharacterData.cloth_r = r;
		SCharacterData.cloth_g = g;
		SCharacterData.cloth_b = b;
		ch->clothcolor = ReturnBlendRGB( r, g, b );
	}
}

void RecvSalvationAddFame( t_packet *p )
{
	int fame = p->u.kein.default_char;
	SCharacterData.nCharacterAbility[FAME] += fame;
	Kein_PutMessage( KM_OK, kein_GetMenuString( 80 ), fame );
	//char nation = Hero->name_status.nation;
	//CallServer( CMD_GET_NATION_MONEY, &nation, sizeof( char ) );
	SmallMenuClose();
}

void RecvRepNationMoney( t_packet *p )
{
	if( !g_Menu_Variable.m_stNationMoney )
		g_Menu_Variable.m_stNationMoney = new k_view_nation_money;
	
	int nation_type = p->u.kein.nation_money.nation_type;
	switch( nation_type )
	{
	case N_VYSEUS : 
		g_Menu_Variable.m_stNationMoney->money3= p->u.kein.nation_money.nation_money[0];
		break;
	case N_ZYPERN :
		g_Menu_Variable.m_stNationMoney->money4 = p->u.kein.nation_money.nation_money[0];
		break;
	case N_YILSE :
		g_Menu_Variable.m_stNationMoney->money6 = p->u.kein.nation_money.nation_money[0];
		break;
	case N_NATION_ALL :
		g_Menu_Variable.m_stNationMoney->money3 = p->u.kein.nation_money.nation_money[0];
		g_Menu_Variable.m_stNationMoney->money4 = p->u.kein.nation_money.nation_money[1];
		g_Menu_Variable.m_stNationMoney->money6 = p->u.kein.nation_money.nation_money[2];
		break;
	}
}

void RecvCheckSalvationName( t_packet *p )
{
	int ret = p->u.kein.default_char;
	if( ret == 1 )		// 존재한다.
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 84 ) );
		SmallMenuClose();
	}
	else
	{
		g_Menu_Variable.bCheckSalvation = 1;		// 기부 메뉴를 실행시킨다.
	}
}

void SendGameItem( MakeItem *matter )
{
	t_packet p;
	int count = 0;
	p.h.header.type = CMD_GAMBLE_ITEM;
	{				
		for( int i = 0 ; i < 20 ; i ++)
		{
			if( !matter[i].item_no ) continue;
			p.u.kein.gamble_item.pos[count] = matter[i].pos;
			count ++;
		}
	}
	
	if( !count ) 
	{
		Kein_PutMessage( KM_FAIL , kein_GetMenuString( 85 ) );
		return;
	}
	p.u.kein.gamble_item.count = count;
	p.h.header.size = sizeof( k_gamble_item ) - sizeof( POS )*(20-count);
	QueuePacket( &p, 1 );
}

void RecvGambleItemResult( t_packet *p )
{
	int result = p->u.kein.default_char;
	switch( result )
	{
	case 1:	
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 85 ) );
		break;
	case 2 : 
		{
			int item_no = p->u.kein.gamble_item_result.item_no;
			Kein_PutMessage( KM_OK, kein_GetMenuString( 86 ), GetItemName( item_no ) );
			break;
		}
	}
}

void RecvGambleStart( t_packet *p )
{
	BYTE rune = p->u.kein.default_char;
	SMenu[MN_GAMBLE].key = 100;
	g_Menu_Variable.m_GambleRune = rune;
	g_Menu_Variable.m_GambleRand = rand()%20;
}

void SendLogOutCharacter( char *szName )
{
#ifdef _DEBUG
	t_packet p;
	p.h.header.type = CMD_SEND_LOGOUT_CHAR;
	{
		strcpy( p.u.kein.default_name , szName);
	}
	p.h.header.size = strlen( szName );
	QueuePacket( &p, 1 );
#endif
}

void RecvReporterMode( t_packet *p )
{
	int mode = p->u.kein.default_char;
	Hero->reporter_mode = mode;
	return;
}

void RecvGuildMemberName( t_packet *p )
{
	k_get_guild_memeber_list *data = (k_get_guild_memeber_list *)p->u.data;
	int degree = data->degree;
	int count = data->count;
	g_Menu_Variable.m_GuildMemberName.SetCount( degree, count, data->name );
	
	
	int &page_status = SMenu[MN_GUILD_MEMBER_LIST].work;
	WORD page;
	WORD page_max;
	GetWORDOfDWORD( (DWORD)page_status, page, page_max );
	
	page = 0;
	//SMenu[MN_GUILD_MEMBER_LIST].nField[0].temp[degree*2] = count;
	page_max = SMenu[MN_GUILD_MEMBER_LIST].nField[0].temp[degree*2+1] = char(count/14.0); //Eleval 21/07/09 - Fixed Guild member list bug, was 27+0.5 before
	
	page_status = (int )MAKELONG( page, page_max );
}

void RecvGetGuildNotice( t_packet *p )
{
	char *text = p->u.kein.default_msg;
	int len = strlen(text);
	if( g_Menu_Variable.m_szGuildPublicNotics ) delete [] g_Menu_Variable.m_szGuildPublicNotics;
	
	if( !len )
	{
		text = kein_GetMenuString( 95 );
		len = strlen( text );
	}
	g_Menu_Variable.m_szGuildPublicNotics = new char[len+1];
	strcpy( g_Menu_Variable.m_szGuildPublicNotics, text );
}

void RecvRegistFriendResult( t_packet *p )
{
	int type = p->u.kein.default_char;
	
	switch( type )
	{
	case 0 :
		Kein_PutMessage( KM_FAIL,lan->OutputMessage(6,23) ); // 없는 사람입니다.
		break;
	case 1:
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 100 ) ); // 친구는 최대 30명까지 등록할 수 있습니다.
		break;
	case 2:
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 101 ) ); // 다른 국가 사람입니다.
		break;
	case 3:
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 102 ) ); // 이미 등록한 사람입니다.
		break;
		
	}
}

void RecvAddFriend( t_packet *p )
{
	int login = p->u.kein.add_friend.login;
	char *name = p->u.kein.add_friend.name;
	
	g_Menu_Variable.m_FriendGrup->InsertFriend( name, login );
}

void RecvGetTodayGambleItem( t_packet *p )
{
	if( g_Menu_Variable.m_TodayGambleItem )
	{
		delete g_Menu_Variable.m_TodayGambleItem;
	}
	
	g_Menu_Variable.m_TodayGambleItem = new ItemAttr;
	*g_Menu_Variable.m_TodayGambleItem = p->u.kein.default_item;
}

extern void ReturnNationName(int Nation,char* Message);
void RecvTodyaItemGetWho( t_packet *p )
{
	// 누군가 겜블 아이템을 뽑아 갔다.
	ItemAttr item = p->u.kein.today_gamble_item.item;
	int nation = p->u.kein.today_gamble_item.nation;
	char *name = p->u.kein.today_gamble_item.name;

	if( g_Menu_Variable.m_TodayGambleItem )
	{
		memset( g_Menu_Variable.m_TodayGambleItem, 0, sizeof( ItemAttr ) );
	}
	
	char nation_name[30];
	ReturnNationName( nation, nation_name );
	char item_name[256];
	
	RareMain sok;
	memcpy( &sok, &item.attr[IATTR_RARE_MAIN], sizeof( DWORD ) );
	sprintf( item_name, "%s", GetItemName( item.item_no ) );
	if( sok.soksung1 )//this4 lsw
	{
		if(sok.soksung1 > 0)
		{
			sprintf( item_name, "%s, %s:%d", item_name, ItemRare[sok.soksung1].LocalName, sok.grade);
		}
		if(sok.soksung2 >0)
		{
			sprintf( item_name, "%s, %s:%d", item_name, ItemRare[sok.soksung1].LocalName, sok.grade );
		}
	}
	
	Kein_PutMessage( KM_OK, kein_GetMenuString( 105 ), nation_name, name, item_name );
}

void SendOXEventAboutMap( char type )
{
	CallServer( CMD_SET_MAP_TYPE, &type, 1 );
}

void SendOXQuestion( char *str )
{
	t_packet p;
	int len;
	
	if( str == NULL ) return;
	len = strlen( str );
	if( len == 0 ) return;
	if( len >= MAX_MSG-1 ) return;
	
	p.h.header.type = CMD_OX_QUESTION;
	strcpy( p.u.kein.default_msg, str );
	p.h.header.size = len;
	
	QueuePacket( &p, 1 );
}

void RecvOXQuestion( t_packet *p )
{
	AddCurrentStatusMessage( 200,200,0, kein_GetMenuString( 119 ) );//010215 lsw
	AddCurrentStatusMessage( 255,255,0, p->u.kein.default_msg );
}

void RecvSetOXTime( t_packet *p )
{
	int hour = p->u.kein.set_time.hour;
	int minute = p->u.kein.set_time.minute;
	
	g_Menu_Variable.m_bOXTime = 1;
	g_Menu_Variable.m_OXTime = g_curr_time + 60*hour+60*minute;	
}

void RecvGuildItemFirst( t_packet *p )
{
	int max = p->u.kein.get_guild_item_first.max;
	DWORD *item_index = p->u.kein.get_guild_item_first.item_index;
	
	if( g_Menu_Variable.m_pGuildItem )
	{
		delete g_Menu_Variable.m_pGuildItem ;
	}
	
	if( max )
	{
		g_Menu_Variable.m_pGuildItem = new CGuildItem;
		
		g_Menu_Variable.m_pGuildItem->m_Max = max;
		g_Menu_Variable.m_pGuildItem->m_pIndex = new DWORD[max];
		g_Menu_Variable.m_pGuildItem->m_pItem = new ItemAttr[max];
		memcpy( g_Menu_Variable.m_pGuildItem->m_pIndex, item_index, sizeof( DWORD )*max );
		
		int line = SMenu[MN_GUILD_ITEM_GET].key;
		
		int &page_status = SMenu[MN_GUILD_ITEM_GET].work;
		WORD page;
		WORD page_max;
		
		page = 0;
		page_max = (max-1)/line + 1;
		page_status = MAKELONG( page, page_max );
		
		
		// 첫번째 장을 요청한다.
		SendDBForGuildItem( g_Menu_Variable.m_pGuildItem->m_pIndex[0] );
	}
}

void RecvGuildItemCount( t_packet *p )
{
	BYTE max = p->u.kein.default_char;
	if( g_Menu_Variable.m_pGuildItem )
		g_Menu_Variable.m_pGuildItem->m_Max = max;
}

void SendDBForGuildItem( DWORD index )
{
	k_req_guild_item data;
	data.index = index;
	data.guild_code = Hero->GetGuildCode(); // CSD-030324
	SendDirectDB( CMD_GET_GUILD_ITEM, &data, sizeof( k_req_guild_item ) );
}

void RecvGuildItem( t_packet *p )
{
	int count = p->u.kein.get_guild_item.count; // 가져온 아이템의 갯수
	
	DWORD *pIndex = p->u.kein.get_guild_item.index;
	ItemAttr *pItem = p->u.kein.get_guild_item.item;
	
	if( !g_Menu_Variable.m_pGuildItem ) return;		// 메뉴가 닫쳤다.
	if( !count ) return;
	
	int index = pIndex[0];
	int ct = g_Menu_Variable.m_pGuildItem->SerchFirstIndex( index );
	for( int i=0; i<count; i++ )
	{
		g_Menu_Variable.m_pGuildItem->InsertItem( i+ct, pIndex[i], &pItem[i] );
	}
}

void SendInsertItem( POS *pPos )
{
	t_packet packet;
	packet.h.header.type = CMD_PUT_GUILD_ITEM;
	packet.u.kein.default_pos = *pPos;
	packet.h.header.size = sizeof( ItemAttr );
	QueuePacket( &packet, 1 );
}

void RecvGuildBoxMoney( t_packet *p )
{
	DWORD money = p->u.kein.default_int;
	if( !g_Menu_Variable.m_pGuildItem ) return;		// 메뉴가 닫쳤다.
	g_Menu_Variable.m_pGuildItem->m_BoxMoney = money;
}

void RecvChangeGuildDegree( t_packet *p )		// hero 의 길드 코드와 degree 값을 변경한다.
{	//< CSD-031001
	int server_id = p->u.kein.chanage_guild_degree.server_id;
	int guild_code = p->u.kein.chanage_guild_degree.guild_code;
	int degree= p->u.kein.chanage_guild_degree.degree;
	
	LPCHARACTER ch = ReturnCharacterPoint( server_id );
	if( !ch ) return;
	
	ch->SetGuildCode(guild_code);
	ch->name_status.guild_master = degree;
	
	if( degree == 1 )		// 길드 마스터
	{
		Kein_PutMessage( KM_OK, kein_GetMenuString(142), ch->name );
	}
}	//> CSD-031001

void SendChangeGuildMasterBySubMaster( int type, int guild_code, char *name )
{
	k_guild_notice data;
	data.guild_code = guild_code;
	strcpy( data.msg, name );
	SendDirectDB( type, &data, sizeof( short int )+strlen(data.msg)+1 );
}

void SendCheckGuildChange( int type )
{
	// type : 1( 길드 마크 변경 ), 2( 길드 디그리 정보 변경 )
	t_packet packet;
	packet.h.header.type = CMD_CHECK_CHANGE_GUILD_INFO;
	packet.u.kein.default_char = type;
	packet.h.header.size = sizeof( char );
	QueuePacket( &packet, 1 );
}

void RecvCheckGuildChange( t_packet *p )
{
	int type = p->u.kein.check_guild_change.type;
	int ret = p->u.kein.check_guild_change.ret;
	
	if( !ret )		// 실패
	{
		MP3( SN_WARNING );
		CallOkCancelMessageBox(0,0,0, kein_GetMenuString( 145 ), 0 );
		//Kein_PutMessage( KM_FAIL, kein_GetMenuString( 145 ) );
		return;
	}
	
	switch( type )
	{
	case 1 :		// 길드 마크 변경
		SendGuildMark( Hero->GetGuildCode(), &guild_mark_main.m_Spr ); // CSD-030324
		break;
	case 2 :		// 길드 정보 변경
		SendChangeGuildDegree( Hero->GetGuildCode() ); // CSD-030324
	default : 
		break;
	}
}

void RecvGetNationItem( t_packet *p )
{
	g_Menu_Variable.m_nation_item = p->u.kein.nation_item;
}

void RecvBuyNationItemOk( t_packet *p )
{
	int type = p->u.kein.default_char;
	switch( type )
	{
	case 1:	// ok
		Kein_PutMessage( KM_OK, kein_GetMenuString( 149 ) );
		break;
	case 2: // 돈이 부족
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 21 ) );
		break;
	case 3: // 명성치 부족
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 148 ) );
		break;
	}
}

void RecvCheckColossusMapReply( t_packet *p )
{
	WORD port = p->u.kein.colossus_info_map_reply.port;
	int user = p->u.kein.colossus_info_map_reply.ch_count;
	char *leader = p->u.kein.colossus_info_map_reply.name;
	
	// 151	[%s] : 총[%d]명	    방장 : [%s]
	char *mapname = GetMapNameByPort( port );
	if( mapname )
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 151 ), mapname, user, leader );
}

void RecvInviteColossusReply( t_packet *p )
{
	char *name = p->u.kein.invite_colossus_map.name;		// 방장이름
	WORD port =  p->u.kein.invite_colossus_map.port;		// 맵 포트
	
	char *mapname = GetMapNameByPort( port );
	
	if (mapname)
	{	//< CSD-030521
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 155 ), mapname, name );
	}	//> CSD-030521

	g_Menu_Variable.m_assent.type = AT_JOIN_COLOSSUS;		// '/참여' 라는 명령어를 이용할때
	g_Menu_Variable.m_assent.time = g_curr_time;
	g_Menu_Variable.m_assent.u.default_word = port;	
}

void SendJoinColossusMap()
{
	if( IsDead( Hero ) ) return;		// 죽었을 경우 못 움직이게 한다.	// 020927 YGI
	
	if( g_Menu_Variable.m_assent.type != AT_JOIN_COLOSSUS ) 
	{
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 157 ) );
		return;
	}
	//< LTH-040811-KO 감옥에 있을때에는 아레나 참여를 막는다!!
	if (true == g_cPrison.IsBadUser())
	{
		t_packet tagPacket;
		tagPacket.u.kein.tagPrisonSysProcResult.nReason = (int)(CPrison::PSR_NOT_MOVE);
		g_cPrison.RecvPrisonSystemResult(&tagPacket);
		return;
	}
	if( g_curr_time - g_Menu_Variable.m_assent.time > 60 ) 
	{
		g_Menu_Variable.m_assent.type = 0;
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 157 ) );
		return;		// 1분이 지나면 취소 시킨다.
	}
	
	WORD port = g_Menu_Variable.m_assent.u.default_word;
	char *mapname = GetMapNameByPort( port );
	if( mapname )
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 158 ), mapname );
	
	t_packet packet;
	packet.h.header.type = CMD_JOIN_COLOSSUS_MAP;
	packet.u.kein.default_short_int = port;
	packet.h.header.size = sizeof( WORD );
	QueuePacket( &packet, 1 );
}

void RecvAttackColossusStoneResult( t_packet *p )
{
	int item_id = p->u.kein.attack_colossus_stone_result.item_id;
	int server_id = p->u.kein.attack_colossus_stone_result.server_id;
	DWORD attr = p->u.kein.attack_colossus_stone_result.attr;
	
	LPITEMGROUND i = FindItemList( item_id );
	if( i == NULL ) return;
	LPCHARACTER ch = ReturnCharacterPoint( server_id );
	if( ch == NULL ) return;
	
	i->attr[IATTR_ATTR] = attr;		// 속성 변화
	
	//if( ch != Hero ) // 누구든지
	{
		ch->nAttackedType	= SPRITETYPE_ITEM;
		ch->lpAttacked		= (VOID *)i;
		
		int ani = AttackAction( ch );
		if( ani == ch->nCurrentAction )	return;
		else  ch->nCurrentAction = ani;
		ChangeDirection( &ch->direction, ch->x, ch->y, i->x, i->y );
		ch->todir = ch->direction;
		ch->nCurrentFrame.PutEncrypted(0);
	}
}

void RecvCallMainMenu( t_packet *p )
{
	int menu = p->u.kein.default_int;
	CallMenu( menu );
}

void RecvGuildHouseInfo( t_packet *p )
{
	CallSmallMenu( MN_GUILD_HOUSE );
	k_guild_house_info *pData = (k_guild_house_info *)p->u.data;
	if( !pData->count ) return;
	if( g_Menu_Variable.m_pGuildHouseInfo ) 
	{
		delete g_Menu_Variable.m_pGuildHouseInfo;
		g_Menu_Variable.m_pGuildHouseInfo = 0;
	}
	if( pData->count > 100 ) return;
	
	g_Menu_Variable.m_pGuildHouseInfo = new CGuildHouseInfo( pData->count );
	for( int i=0; i<pData->count; i++ )
	{
		g_Menu_Variable.m_pGuildHouseInfo->CopyInfo( i, &pData->data[i] );
	}
	
	int &page_status = SMenu[MN_GUILD_HOUSE].work;
	int line_max = SMenu[MN_GUILD_HOUSE].key;
	WORD page;
	WORD page_max;
	GetWORDOfDWORD( (DWORD)page_status, page, page_max );
	page = 0; page_max = pData->count/line_max+1;
	page_status = MAKELONG( page, page_max );
	SMenu[MN_GUILD_HOUSE].key2 = -1;		// 선택바
}

void RecvSetPkOnOff( t_packet *p )
{
	int pk_on_off = p->u.kein.default_char;
	if( pk_on_off == 1 )		// ON
		Kein_PutMessage( KM_OK, kein_GetMenuString( 169 ) );
	else
		Kein_PutMessage( KM_OK, kein_GetMenuString( 170 ) );
}

void RecvGetPkOnOff( t_packet *p )
{
	int pk_on_off = p->u.kein.default_char;
	char *str = (pk_on_off?"\'ON\'":"\'OFF\'");
	Kein_PutMessage( KM_INFO, kein_GetMenuString( 172 ), str );
}

//acer5
void RecvSeachOtherChar( t_packet *p )
{
	CallSmallMenu( MN_DEFAULT_MESSAGEBOX2 );
	
	if( !g_Menu_Variable.m_pOtherChar )
	{
		g_Menu_Variable.m_pOtherChar = new k_char_info;
	}
	*g_Menu_Variable.m_pOtherChar = p->u.kein.char_info;
	
	SMenu[MN_DEFAULT_MESSAGEBOX2].nField[1].nWillDo = 1;
}

void RecvTeleportToCharResult( t_packet *p ) //Eleval 19/08/09 - Tool to teleport to another char
{
	int map_number = p->u.kein.char_info.map_number;
	int coord_x = p->u.kein.char_info.x;
	int coord_y = p->u.kein.char_info.y;

	t_packet packet;
	packet.u.kein.direct_map_move.map_number = map_number;
	packet.u.kein.direct_map_move.x = coord_x;
	packet.u.kein.direct_map_move.y = coord_y;
	strcpy(packet.u.kein.direct_map_move.name, "");
	packet.h.header.type = CMD_DIRECT_MAP_MOVE;
	packet.h.header.size = sizeof(k_direct_map_move) - 20;// + strlen(temp);
	QueuePacket(&packet, 1);
}

//---------------------// 020620 YGI
void RecvSettingMapTile( t_packet *p )
{
	k_setting_map_tile *data = &p->u.kein.setting_map_tile;
	if( data->range == 0 || data->index == 0 || data->x < 0 || data->y<0 || data->x>720||data->y>720 ) 
		return;
	
	if( !g_Menu_Variable.m_pDinamicTile )
	{
		g_Menu_Variable.m_pDinamicTile = new CMapDinamicTile;
	}
	CMapDinamicTile *pDT = g_Menu_Variable.m_pDinamicTile;
	
	DeleteDinamicTile( data->index );		// first delete
	POINTS *pTile;
	int targetX, targetY;
	for( int i=-data->range; i<=data->range; i++ )
	{
		targetX = data->x+i;
		if( targetX>0 && targetX<720 )
		{
			int max = data->range-abs(i);
			for( int j=-max; j<=max; j++ )
			{
				targetY = data->y+j;
				if( targetY > 0 && targetY < 720 )
				{
					int saveTile = 0;
					switch( data->type )
					{
					case 1 :
						if( !TileMap[ targetX ][ targetY ].attr_dont )
						{
							TileMap[ targetX ][ targetY ].attr_dont = 1;
							saveTile = 1;
						}
						break;
					case 2 :
						if( !TileMap[ targetX ][ targetY ].attr_map )
						{
							TileMap[ targetX ][ targetY ].attr_map = 1;
							
							LPEVENTLIST	lpEventList;
							lpEventList = FindEventList( &g_EventListHeaderMap, targetX, targetY );
							if( lpEventList )
							{
								lpEventList->index = data->number;
							}
							else
							{
								AddEventList( &g_EventListHeaderMap, targetX, targetY, data->number);
							}
							saveTile = 1;
						}
						break;
					}
					if( saveTile )
					{
						pTile = new POINTS;
						pTile->x = targetX;
						pTile->y = targetY;
						pDT->AddNode( pTile, data->index );
					}
				}
			}
		}
	}
}

void DeleteDinamicTile( int index )
{
	CMapDinamicTile *pDT = g_Menu_Variable.m_pDinamicTile;
	
	if( !pDT ) return;
	if( pDT->SetFind( index ) )
	{
		POINTS *data;
		while( data = pDT->GetFindNext() )
		{
			if( TileMap[data->x][data->y].attr_map )
			{
				LPEVENTLIST lpEventList = FindEventList(&g_EventListHeaderMap, data->x, data->y);
				if( lpEventList )
				{
					DeleteEventList(&g_EventListHeaderMap, lpEventList );
				}
				TileMap[data->x][data->y].attr_map = 0;
			}
			else if( TileMap[data->x][data->y].attr_dont )
			{
				TileMap[data->x][data->y].attr_dont = 0;
			}
		}
	}
	pDT->DeleteList( index );
}

void RecvDeleteMapTileEvent( t_packet *p )
{
	int index = p->u.kein.default_char;
	DeleteDinamicTile( index );
}

#include "nationsupport.h"
enum eDungeonHistoryType
{
	NORMAL_TRAP_OPEN	= 1,		// 보통의 트랩
	NORMAL_TRAP_CLOSE	= 2,			
	MIDDLE_BOSS_OPEN	= 3,			// 중간 보스로 가는 포탈
	MIDDLE_BOSS_CLOSE	= 4,			
	LAYER_POTAL_OPEN	= 5,			// 다음 층으로 가는 포탈
	LAYER_POTAL_CLOSE	= 6,			
	DUNGEON_POTAL_OPEN	= 7,			// 밖으로 나가는 포탈
	DUNGEON_POTAL_CLOSE	= 8,
	DUNGEON_OPEN		= 9,				// 시나리오에 의해서 던전이 오픈되고 안되고
	DUNGEON_CLOSE		= 10,
	DUNGEON_WHEN_OPEN_CLOSE_NOTICE = 11,//시나리오가 언제 열리는지
};

char* GetDayOfWeekByNo(const int iDOWIndex)
{
	return lan->OutputMessage(3,250+(iDOWIndex%7));
}

void RecvGetScenarioInfo( t_packet *p )
{
	const int count = p->u.kein.get_scenario_info.count;
	if( !count ) return;
	
	char tempMessage[500]={0,};
	char tempMessage2[500]={0,};
	
	int param = 0;
	const char *title = kein_GetMenuString( 195 );
	if( ::strncmp( &SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp[2], title, sizeof( title ) ) == 0 )
	{
		param = 1;
	}
	else if( ::strncmp( &SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp[2], title, sizeof( title ) ) == 0 )
	{
		param = 2;
	}
	else if( ::strncmp( &SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp[2], title, sizeof( title ) ) == 0 )
	{
		param = 3;
	}
	
	sprintf(tempMessage, "3 %s               ", title );

	for( int i=0; i<count; i++ )
	{
		int msg_number = 0;
		k_get_scenario_info_basic *pSCIB = &p->u.kein.get_scenario_info.data[i];

		const int	day			= pSCIB->day;

		const int	nSHour		= pSCIB->SHour;
		const int	nSMin		= pSCIB->SMin;
		
		const int	nEHour		= pSCIB->EHour;
		const int	nEMin		= pSCIB->EMin;
		
		const int	type		= pSCIB->type;
		const int	layer		= pSCIB->layer;
		
		char*		dungeon_map	= pSCIB->dungeon_map;

		const char *map = ::GetMapNameByMapfile( dungeon_map );

		switch( type )
		{
		case NORMAL_TRAP_OPEN :		
			{
				::sprintf( tempMessage2, ::kein_GetMenuString( 185), day, nSHour, nSMin, (map)?map:" " );
			}break;
		case NORMAL_TRAP_CLOSE :	{continue;}break;
		case MIDDLE_BOSS_OPEN :		{continue;}break;
		case MIDDLE_BOSS_CLOSE :	{continue;}break;
		case LAYER_POTAL_OPEN :		
			{
				::sprintf( tempMessage2, ::kein_GetMenuString( 186 ), day, nSHour, nSMin, (map)?map:" " );
			}break;
		case LAYER_POTAL_CLOSE :	{continue;}break;
		case DUNGEON_POTAL_OPEN :	{continue;}break;
		case DUNGEON_POTAL_CLOSE :	{continue;}break;
		case DUNGEON_OPEN :			
			{
				::sprintf( tempMessage2, ::kein_GetMenuString( 182 ), day, nSHour, nSMin, (map)?map:" " );
			}break;
		case DUNGEON_CLOSE :		
			{
				::sprintf( tempMessage2, ::kein_GetMenuString( 183 ), day, nSHour, nSMin, (map)?map:" " );
			}break;
		case DUNGEON_WHEN_OPEN_CLOSE_NOTICE:
			{			
				::sprintf( tempMessage2, lan->OutputMessage(3,240),
							map,
							nSHour, nSMin, nEHour, nEMin, ::GetDayOfWeekByNo(day));
			}break;
		default : continue;
		}
		::sprintf( tempMessage, "%s%s\n", tempMessage, tempMessage2 );
	}
	
	param = 3;	// BBD 040315	평소에는 국지전 포인트가 보여지므로 무조건 덮어찍게 하자
	if( param )
	{
		::IntoNationNotice(FORCE_TYPE, param, tempMessage);
	}
	else
	{
		::IntoNationNotice(QUEUE_TYPE, LAST_IN, tempMessage);
	}
	::CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

// 020705 YGI
void RecvScenarioMessage( t_packet *p )
{
	int type = p->u.kein.scenario_message.type;
	int port = p->u.kein.scenario_message.port;
	
	switch( type )
	{
	case 1:						// 시나리오가 끝났다.
		{
			char *map_name = GetMapNameByPort( port );
			if( !map_name ) return;
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 181 ), map_name );
			g_iScenarioOn = 1;
			break;
		}
		
	case 2:						// 시나리오가 시작됐다.
		{
			char *map_name = GetMapNameByPort( port );
			if( !map_name ) return;
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 180 ), map_name );
			g_iScenarioOn = 1;
			break;
		}
	case 3:		// 암호를 가르쳐 준다.
		{
			int pass = p->u.data[1];
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 196 ), pass );
			break;
		}
	case 4:
		{
			char *name = &p->u.data[1];
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 197 ), name );
			break;
		}
	case 5:
		{
			char *name = &p->u.data[1];
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 198 ), name );
			break;
		}
		// 020707 YGI
	case 7:		// 드래곤 출현
		{
			char data = p->u.data[1];
			if( data )		// 드래곤 로드
			{
				Kein_PutMessage( KM_INFO, kein_GetMenuString( 201 ));
			}
			else Kein_PutMessage( KM_INFO, kein_GetMenuString( 200 ));
			break;
		}
		//----------------------
	case 8:		// 보스 몬스터 출현
		{
			POINTS *xy = (POINTS *)&p->u.data[1];
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 199 ), xy->x, xy->y );
			break;
		}
	case 10:		// 드래곤 로드의 메세지		// 맨처음 만났을때
		{
			SMenu[MN_OKCANCEL_MESSAGEBOX].nTemp = MN_DEFAULT_MESSAGEBOX;
			SMenu[MN_OKCANCEL_MESSAGEBOX].key = 1;
			strcpy(SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].temp, kein_GetMenuString( 202 ) );
			KeinMessageBox( MN_OKCANCEL_MESSAGEBOX, " " );
			SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].m_lpTemp  = lan->OutputMessage(9,151);
			break;
		}
	case 11:		// 드래곤 로드의 메세지	// 모두 찬성
		{
			POINTS *xy = (POINTS *)&p->u.data[1];
			KeinMessageBox( 0, lan->OutputMessage(9,152), xy->x, xy->y );
			break;
		}
	case 12:		// 드래곤 로드의 메세지		// 모두 반대 했을때 
		{
			POINTS *xy = (POINTS *)&p->u.data[1];
			KeinMessageBox( 0, lan->OutputMessage(9,153), xy->x, xy->y );
			break;
		}
	case 13:		// 드래곤 로드의 메세지		// 결투 시작 // 찬반이 나뉘었을때
		{
			POINTS *xy = (POINTS *)&p->u.data[1];
			KeinMessageBox( 0, lan->OutputMessage(9,154), xy->x, xy->y );
			break;
		}
	case 14:		// 드래곤 로드의 메세지		// 결투가 안끝났을때 
		{
			KeinMessageBox( 0, lan->OutputMessage(9,155) );
			break;
		}
	case 15:		// 드래곤 로드의 메세지		// 드래곤 로드의 패배
		{
			KeinMessageBox( 0, lan->OutputMessage(9,156) );
			break;
		}
	case 16:		// 층 포탈 타고 이동했을때 나오는 메세지
		{
			short *pTime = (short *)&p->u.data[1];
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 203 ), *pTime );
			break;
		}
	case 17:		// 그 층이 활성화 됐을 때
		{
			short *layer = (short *)&p->u.data[1];
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 204 ), *layer );
			break;
		}
		// 020707 YGI
	case 18: // 그 층이 활성화 됐을 때
		{	//< CSD-030419
			k_invite_colossus_map *pData = (k_invite_colossus_map*)&p->u.data[1];
			Kein_PutMessage(KM_INFO, 
				            kein_GetMenuString(205), 
							pData->name, 
							g_infNpc[pData->port].szName); 
			break;
		}	//> CSD-030419
	}
}


void RecvScenarioTrapInfo( t_packet *p )
{
	
	
	if( !g_Menu_Variable.m_pScenarioTrapInfo ) 
		g_Menu_Variable.m_pScenarioTrapInfo = new CScenarioTrapInfo;
	
	int type = p->u.kein.scenario_detail_info.type;
	switch( type )
	{
	case 0 : 
		g_Menu_Variable.m_pScenarioTrapInfo->m_nTotalTrap = 0;
		break;
	case 1 : 
		g_Menu_Variable.m_pScenarioTrapInfo->m_nTotalTrap = p->u.kein.scenario_detail_info.total_trap;
		g_Menu_Variable.m_pScenarioTrapInfo->m_nActiveTrap = p->u.kein.scenario_detail_info.active_trap;
		g_Menu_Variable.m_pScenarioTrapInfo->m_nWaitTimeOfTrap = p->u.kein.scenario_detail_info.time;
		break;
	case 2 : 
		g_Menu_Variable.m_pScenarioTrapInfo->m_nWaitTimeOfDungeon = p->u.kein.scenario_detail_info.time;
		break;
	}
	return;
}

// 020701 YGI
void RecvScenarioTrapKey( t_packet *p )
{
	if( !g_Menu_Variable.m_pScenarioTrapKey ) 
		g_Menu_Variable.m_pScenarioTrapKey = new k_scenario_trap_key;
	
	*g_Menu_Variable.m_pScenarioTrapKey = p->u.kein.scenario_trap_key;
}

// 020830 YGI
void RecvGetScenarioTime( t_packet *p )
{
#ifdef _DEBUG

//< 030307 soto 
	k_server_scenario_time_info* pData = new k_server_scenario_time_info;// 메모리를 할당만 시켜 주었다.
																		 // 반드시 헤제 시켜주었는지 확인 할것.
	pData->idDungeon = p->u.kein.server_scenario_time.idDungeon;
	pData->idStartUp = p->u.kein.server_scenario_time.idStartUp;
	pData->nApplyDay = p->u.kein.server_scenario_time.nApplyDay;
	pData->nEndHour = p->u.kein.server_scenario_time.nEndHour;
	pData->nEndMinute = p->u.kein.server_scenario_time.nEndMinute;

	pData->nStartHour = p->u.kein.server_scenario_time.nStartHour;
	pData->nStartMinute = p->u.kein.server_scenario_time.nStartMinute;

	if(!AddTimeList((void*)pData))
	{
		delete pData;
	}
//> 030307 soto
#endif
}

// 021008 YGI
void RecvMoveMapForLightVersion( t_packet *p )
{
#ifdef LIGHT_VERSION_
	k_map_move *data = &p->u.kein.map_move;
	if( g_pLightVersion->IsGotoMap( data->mapname ) )
	{
		QueuePacket( p, 1 );
	}
	else
	{
		//  메시지 출력 후 끝
		Kein_PutMessage( KM_FAIL, lan->OutputMessage( 5,28 ) );
		Kein_PutMessage( KM_FAIL, lan->OutputMessage( 5,29 ) );
	}
#else
	QueuePacket( p, 1 );
#endif
}


// 021107 YGI
void SendCheckEventMoveMap()
{
	k_event_move_map_req data;
	data.sex = Hero->sprno;
	data.class_num = Hero->class_type;
	data.nation =Hero->name_status.nation ;
	SendDirectDB( CMD_CHECK_EVENT_MOVE_MAP, &data, sizeof( k_event_move_map_req ) );
}

void RecvCheckEventMoveMap( t_packet *p )
{
	k_event_move_map_rep *pData = (k_event_move_map_rep *)p->u.data;
	
	SAFE_DELETE( g_Menu_Variable.m_pEventMoveMap );
	g_Menu_Variable.m_pEventMoveMap = new CEventMoveMap;
	*g_Menu_Variable.m_pEventMoveMap = *pData;
	char temp[512];
	char *mapname = GetMapNameByMapfile(g_Menu_Variable.m_pEventMoveMap->data.map_name);
	if( strcmp( mapname, MapName ) == 0 ) return;
	
	sprintf( temp, kein_GetMenuString( 211 ), mapname?mapname:g_Menu_Variable.m_pEventMoveMap->data.map_name );
	CallOkCancelMessageBox( 0, 0, 0, temp, 0 );
}

// 040105 YGI ---------------------
void SendCheckTreasureXY( int x, int y )
{
	t_packet packet;
	
	packet.h.header.type = CMD_CHECK_TREASURE_XY;
	packet.u.kein.check_treasure_xy.x = x;
	packet.u.kein.check_treasure_xy.y = y;
	packet.h.header.size = sizeof( k_check_treasure_xy );
	
	QueuePacket( &packet, 1 );
}

void CheckTreasure( int x, int y )
{
	// 서버로 보낼지 안 보낼지 체크 루틴
	int tile_x = x/TILE_SIZE;
	int tile_y = y/TILE_SIZE;
	for( int a=0; a<3; a++ )
	{
		for( int b=0; b<3; b++ )
		{
			for( int c=0; c<8; c++ )
			{
				ItemAttr *pItem = &InvItemAttr[a][b][c];
				CItem *t = ItemUnit( pItem->item_no );
				if( !t ) continue;
				if( t->GetItemKind() != IK_TREASURE_MAP ) continue;// 보물 지도가 아니다.

				CTreasureMapItem *pAttr = (CTreasureMapItem *)&pItem->attr[IATTR_TREASURE_MAP];
				if( !pAttr->isChecked || pAttr->mapnumber != MapNumber ) continue;

				int off_x, off_y;
				switch( pAttr->offset2 )
				{
				case 0 : off_x = pAttr->offset; off_y = pAttr->offset; break;
				case 1 : off_x = pAttr->offset; off_y = -pAttr->offset; break;
				case 2 : off_x = -pAttr->offset; off_y = pAttr->offset; break;
				case 3 : off_x = -pAttr->offset; off_y = -pAttr->offset; break;
				}

				if( (abs((long)(tile_x-(pAttr->x+off_x))>12)) || (abs((long)(tile_y-(pAttr->y+off_y))>12)) ) continue;
				SendCheckTreasureXY(x, y);
				return;
			}
		}
	}
}

void RecvCMD_SEND_EFFECT( t_packet *p )
{
	int effect_no = p->u.kein.send_effect.effect_num;
	int id = p->u.kein.send_effect.target_id;
	int x =  p->u.kein.send_effect.x;
	int y =  p->u.kein.send_effect.y;
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( !ch ) ch = Hero;
	InsertMagic( ch, ch, effect_no, 0,x,y,x,y ); 
	//InsertMagic( ch, ch, effect_no, 0,0,0,0,0 ); 
}

void SendGetTreasureMapDetail( POS *pPos )
{
	ItemAttr *item = GetItemByPos(*pPos);
	if( !item ) return;

	CTreasureMapItem *attr = (CTreasureMapItem* )&item->attr[IATTR_TREASURE_MAP];
	if( attr->isChecked != 0 ) return;	// 이미 보냈다.
	attr->isChecked = 1;

	t_packet packet;
	
	packet.h.header.type = CMD_GET_TREASURE_MAP_DETAIL;
	packet.u.kein.default_pos = *pPos;
	packet.h.header.size = sizeof( POS );
	
	QueuePacket( &packet, 1 );
}

void SendMantleDye(int r, int g, int b) // finito 25/06/08
{
	t_packet packet;
	packet.h.header.type = CMD_MANTLE_DYE;
	packet.u.MantleDye.mantleColor = (65536 * r + 256 * g + b);
	packet.h.header.size = sizeof(t_MantleDye);
	QueuePacket( &packet, 1 );
}

void RecvMantleDye(t_packet *p) // finito 25/06/08
{
	int serverId = p->u.MantleDyeOther.serverId;
	int color = p->u.MantleDyeOther.mantleColor;
	
	LPCHARACTER ch = ReturnCharacterPoint(serverId);
	if( !ch ) return;
		
	// Update the colour of mantle
	int r, g, b;
	r = (color>>16);
	color -= (r<<16);
	g = (color>>8);
	color -= (g<<8);
	b = color;
	ch->mantlecolor = ReturnBlendRGB( r, g, b );
}

void SendApplicationOfExile(LPCHARACTER pMaster) //Eleval 12/06/09
{
	t_packet packet;
	packet.h.header.type = CMD_APPLICATION_OF_EXILE;
	packet.h.header.size = sizeof(t_application_of_exile);
	packet.u.application_of_exile.idMaster = pMaster->id;
	QueuePacket( &packet, 1 );
}

void RecvApplicationOfExileResult( t_packet *p ) //Eleval 12/06/09
{
	if(p->h.header.type == CMD_EXILE_NOT) //Character's still in guild, sending him a message he cannot transfer (sad, no? :p)
	{
		AddCurrentStatusMessage(255, 10, 10, lan->OutputMessage(3,299));
	}
	else if(p->h.header.type == CMD_EXILE_OK) //Everything worked, going to log out character
	{
		if (Hero != NULL) 
		{					
			ResetCharacter(Hero);
			ResetEffect(Hero);
			g_pArenaManager->Reset();
		}	//> CSD-040310

		QuitFlag = SWD_LOGON;	// logout
		MP3( SN_LOGOUT );
		StopWaveOfMenu();
		SendLogOut();
		Release( &connections );
		CrackMgr.InitCrackMgr();
	}
}

void SendSexChange(LPCHARACTER pUser) //Eleval - Sex Change
{
	if(!strcmp(pUser->name, "Dark_Assassin") && g_iDASexChange < 2)
	{
		if(g_iDASexChange == 0)
			KeinMessageBox(0, "Errr, I'm Sorry D_A, you're too sexy as a girl to become a man :) -the Sign Painter");
		if(g_iDASexChange == 1)
			KeinMessageBox(0, "Well, maybe I'll consider about it if you buy me a box of chocolates :) -the SignPainter");

		g_iDASexChange++;
	}
	else
	{
		t_packet packet;
		packet.h.header.type = CMD_SEX_CHANGE;
		packet.h.header.size = sizeof(t_sex_change);
		packet.u.sex_change.idUser = pUser->id;
		QueuePacket( &packet, 1);
	}
}

void RecvSexChangeResult( t_packet *p ) //Eleval 12/06/09 - Sex Change
{
	
		if (Hero != NULL) 
		{
			if( Hero->start_Casting > g_ClientTime )
			{
				const int nOffset = Hero->start_Casting - g_ClientTime;
								
			}
							
			ResetCharacter(Hero);
			ResetEffect(Hero);
			g_pArenaManager->Reset();
		}	//> CSD-040310

		QuitFlag = SWD_LOGON;	// logout
		MP3( SN_LOGOUT );
		StopWaveOfMenu();
		SendLogOut();
		Release( &connections );
		CrackMgr.InitCrackMgr();
}

void RecvToolPlaySound( t_packet *p ) // Eleval - Tool Play Sound
{
	int played_sound = p->u.tool_play_sound.sound_number;

	MP3( played_sound);
}

void RecvCharOptionsChange( t_packet *p ) //Eleval 18/08/09 - To change a char's options
{
	LPCHARACTER pCaster = ::ReturnCharacterPoint(p->u.change_char_options.CasterID);

#ifdef _DEBUG
	if(!pCaster)
	{
		AddCurrentStatusMessage(255, 0,0, "Char is NULL !");
		return;
	}
#endif

	switch(p->u.change_char_options.type)
	{
	case 1:
		pCaster->hat_is_hidden = p->u.change_char_options.flag;
		break;
	}
}

void RecvReqHonorAmount( t_packet *p )
{
	int honor = p->u.kein.default_int;

	AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,300), honor);
}