/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/
#define __CLIENT_PROGRAM__

#if !defined(AFX_DRAGON_PROTOCOL_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_)
#define AFX_DRAGON_PROTOCOL_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FD_SETSIZE		500
#include <winSock2.h>
#pragma comment(lib,"Ws2_32.lib")
#include <MsWSock.h>
#pragma comment(lib,"MsWSock.lib")
#include <WS2TcpIp.h>

#ifdef __CLIENT_PROGRAM__
	#include "ItemTable.h"
#else
	#include "charlist.h" 
	#include "ItemList.h"

#endif

#define SIZE_OF_WS				(200 * sizeof( unsigned char ))
#define SIZE_OF_PS				(200 * sizeof( unsigned char ))
#define SIZE_OF_SKILL			(200 * sizeof( unsigned char ))	//1219 YGI
#define SIZE_OF_SKILL_EXP		(45  * sizeof( unsigned int	 ))	
#define SIZE_OF_TAC_SKILL_EXP	(13  * sizeof( unsigned int	 ))	

#define SIZE_OF_SCRIPT_VAR		(300 * sizeof( int ))	
#define	SIZE_OF_INV				(3*3*8* sizeof( ItemAttr )) 
#define	SIZE_OF_EQUIP			(    8* sizeof( ItemAttr )) 
#define	SIZE_OF_QUICK			(    6* sizeof( ItemAttr )) 
#define SIZE_OF_BANKITEM		(5*3*6* sizeof( ItemAttr )) 

#define	SIZE_OF_PARTY			(    31 * 6 )
#define	SIZE_OF_RELATION		(    31 * 3 ) 
#define	SIZE_OF_EMPLOYMENT		(    31 * 6 ) 
#define	SIZE_OF_ITEMINDEX		(  1000 * sizeof( short int ) )




#define MAX_CHECK_IP_		3

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------


#define MAX_STORE_LIST			30		//1213
#define MAX_STORE_ITEM_LIST    300		// 0710 YGI

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------



// 010505 KHS


#define EFFECT_TABLE_VERSION			907
//--------------------------------------------------------


#define LOGIN_QUEUE						5
#define DRAGON_QUEUE					5

//--------------------------------------------------------
#ifdef _SERVER_MAKE_MODE_	

	#define LOGIN_MAX_CONNECTIONS		15
	#define DRAGON_MAX_CONNECTIONS		35
	
#else 

	#define LOGIN_MAX_CONNECTIONS		150
	#define DRAGON_MAX_CONNECTIONS		500

#endif



#define DRAGON_CONNECTIONS_START		2

#define LOGIN_SERVER_CONNECT			0	// °øÁö»çÇ× ¸ðµç ¼­¹ö¿¡ ¿¬°áÇÏ±â À§ÇÑ ¼­¹ö wka
#define DB_DEMON						1	//	°ÔÀÓ¼­¹ö¿¡ ÇÒ´çµÈ DBµ¥¸ó


#define PACKEET_SEND_TRY				10

//--------------------------------------------------------		// YGI 0521
#define CONNECT_FAIL					0
#define CONNECT_OK						1
#define CONNECT_LOGIN_KEIN				2	// ¸Ê°£ ÀÌµ¿
#define CONNECT_LOGIN					3
#define CONNECT_JOIN					4
#define CONNECT_JOIN_START				5


//--------------------------------------------------------
#define MAX_CHARACTEROFID				4 
//------------------- NEW_APPEND -------------------------
#define MAX_PACKET_SIZE					3000
//#define MAX_PACKET_SIZE					512

#define MAX_SERVER_QUEUE_OUT_BUFFER		(65536)
#define MAX_CLIENT_QUEUE_OUT_BUFFER		16584


#define MAX_DIR_TABLE					256

#define ID_LENGTH						20
#define PW_LENGTH						20
#define NM_LENGTH						20
#define IP_LENGTH						16
#define TEXT_LENGTH						255

//--------------------------------------------------------
#define CMD_NONE						0

#define CMD_INVALID_ID					1
#define CMD_INVALID_PW					2
#define CMD_INVALID_DB					3
#define CMD_INVALID_PAY					4
										 
#define CMD_PING						5
#define CMD_BANNED_PLAYER   					18

#define CMD_ACCESS_LOGIN				6066 //010707 lsw ¼öÁ¤
#define CMD_ACCEPT_LOGIN				6067 //010707 lsw ¼öÁ¤
#define CMD_ACCESS_LOGOUT				12
#define CMD_ACCEPT_CREATE				13
#define CMD_USED_ID						14
#define CMD_ACCEPT_DELETE				15
#define CMD_INVALID_VERSION				16
#define CMD_INVALID_AGE					17	// 030929 kyo //ÅÂ±¹ ³ªÀÌ Á¦ÇÑ

#define CMD_USER_ADD					20
#define CMD_USER_REMOVE					21
#define CMD_YOU_REMOVE					22

#define CMD_ACCESS_JOIN					30
#define CMD_ASSIGN_SERVER_ID			31
#define CMD_ACCEPT_JOIN					32
typedef struct server_accept_join
{
	short int x, y;
}t_server_accept_join;
#define CMD_ALL_READY					33
typedef struct all_ready
{
	short int x,y;

}t_all_ready;


//< soto-030331
#define CMD_GUARDIANLIST				1110

#define MAX_GUARDIAN					15
typedef struct GUARDAINLIST
{
	short int	nCount;
	short int	anGuildCodes[MAX_GUARDIAN];
}t_GUARDIANLIST;
//> soto-030331



#define CMD_USER_DB						40
#define CMD_ENVIR_INFO					41

#define CMD_CREATE_CHAR					50
#define CMD_DELETE_CHAR                 52

#define	CMD_NPC_ADD						60	
#define	CMD_NPC_REMOVE					61

#define	CMD_ITEM_ADD 					70
#define	CMD_ITEM_ADD_ZIPED				7001//this lsw ¾ÐÃàÀÓÀ» ³ªÅ¸³¿

#define	CMD_ITEM_REMOVE					71
#define	CMD_ITEM_DROP					72
#define	CMD_ITEM_TAKE					73
#define	CMD_ITEM_EQUIP					74
#define	CMD_YOU_GOT_ITEM				75
#define CMD_ITEM_STATUS					76
#define CMD_CHANGE_EQUIP				77
#define CMD_ITEM_ATTR_CHANGE			78
#define CMD_ITEM_DOOR_OPEN_CLOSE		79
typedef struct item_door_open_close			
{
	short int item_id;
	char  openclose;
}t_item_door_open_close;

#define CMD_ITEM_BOX_BREAK				210		
typedef struct client_item_box_break
{
	short int		item_id;
}t_client_item_box_break;
typedef struct server_item_box_break
{
	short int		server_id;
	short int		item_id;
	char			type;

}t_server_item_box_break;

											
#define CMD_ITEM_BOX_BREAK_RESULT		212	
typedef struct client_item_box_break_result	
{														
	short int		item_id;				
}t_client_item_box_break_result;				
											
#define CMD_ITEM_BOX_REFRESH			211	
											
// ¸ÅÁ÷À¸·Î ¾ÆÀÌÅÛÀ» ºÎ¼ú¶§ »ç¿ëÇÑ´Ù.		
#define CMD_ITEM_BOX_MAGIC_BREAK		214		
typedef struct client_item_box_magic_break	
{													
	short int		item_id;					
}t_client_item_box_magic_break;						
											
#define CMD_ITEM_BOX_MAGIC_BREAK_RESULT	213 
typedef struct server_item_box_magic_break_result
{												
	short int		item_id;							
	char			type;								
}t_server_item_box_magic_break_result;							
											

#define CMD_JUST_ATTACK_ANIMATION		215
typedef struct client_just_attack_animation{
	short int		item_id;
}t_client_just_attack_animation;										

#define CMD_MOVE						6100	//010707 lsw ¼öÁ¤
#define CMD_CHAT_DATA					6250	//010707 lsw ¼öÁ¤
#define CMD_WEATHER						83	

#define CMD_INSERT_MAGIC				624
typedef struct server_insert_magic
{
	short int item_id;
	short int effectno;
	short int tx, ty;
}t_server_insert_magic;


	


//------------ ÀºÇà ¸Þ´º Ãß°¡ -------------------------------

#define	REQ_MY_DEPOSIT_INFO				90
#define	REP_YOUR_DEPOSIT_INFO			91

#define	REQ_MY_REPAYMENT_INFO			92
#define	REP_YOUR_REPAYMENT_INFO			93

//------------------------------------------------------------------

#define	CMD_CHR_ITEM_INFO_0				5000	// Inventory
#define	CMD_CHR_ITEM_INFO_1				5001	// Equip
#define	CMD_CHR_ITEM_INFO_2				5002	// quick.

// server_char_basic_data
#define CMD_CHAR_INFO_BASIC 			5003
typedef struct char_info_basic
{	short int kind;
	DWORD  data;
	char   password;

}t_char_info_basic;



#define CMD_CHAR_INFO_MAGIC				5004
typedef struct char_info_magic{	
	unsigned char magic[ 200 ];
}t_char_info_magic;


#define CMD_CHAR_INFO_SKILL				5005
typedef struct char_info_skill{	
	unsigned char skill[ 200 ];
}t_char_info_skill;


#define CMD_OTHER_INFO_BASIC 			5006
typedef struct other_info_basic
{	short int server_id;
	short int kind;
	DWORD  data;
}t_other_info_basic;



#define CMD_GIVE_ITEM					5007
typedef struct server_give_item	
{
	char p1, p2, p3;
	ItemAttr item;
}t_server_give_item;

typedef struct client_item_inv // 1218 YGI
{
	short int count;
}k_client_item_inv;



#define CMD_CHAR_INFO_TAC_SKILL_EXP		5008
typedef struct char_info_tac_skillexp
{
	unsigned int tac_skillEXP[ 13];
	char	aStepInfo[20];

}t_char_info_tac_skillexp;




#define CMD_ITEM_EXCHANGE				88
#define CMD_ITEM_EXCHANGE_REFUSE		89

#define CMD_ITEM_EQUIP_OK				98

#define CMD_FOOT_ON_TRAP_0_ITEM			99
#define CMD_FOOT_ON_TRAP_1_ITEM			621
#define CMD_STEAL_ON_TRAP				9010//011212 lsw 

struct t_client_foot_on_trap0
{
  WORD idServer;
};

struct t_server_foot_on_trap0
{
  WORD idServer;
  WORD idItem;
  __int32 nLife;
};

struct t_client_foot_on_trap1
{
  WORD idServer;
};

struct t_server_foot_on_trap1
{
  WORD idServer;
  WORD idItem;
  __int32 nLife;
};

//011212 lsw >
struct t_server_steal_on_trap
{
  WORD idServer;
  __int32 nLife;
};
//----------- Event ---------------------------------
#define CMD_EVENT						100
#define CMD_DOOR_OPEN					101
#define CMD_DOOR_CLOSE					102

// ------- Change Map	----------------------------
#define CMD_CHANGE_MAP					110



#define CMD_MODIFY_POSITION				111
typedef struct server_modidy_position
{
	short int server_id;
	short int gox, goy;
}t_server_modify_position;
typedef struct client_modify_posotion
{
	short int x, y;
}t_client_modify_position;

#define CMD_MODIFY_POSITION_ABSOLUTE	112
typedef struct server_modify_position_absolute
{
	short int id;
	short int x, y;
}t_server_modify_position_absolute;










// ----------  Ãß°¡ Data ¿ä±¸---------------------
// º¸³»Áö ¾ÊÀº user/npc/item¿¡ ´ëÇÑ Á¤º¸°¡ ¿ÔÀ»¶§ 
// ±âº» Data¸¦ º¸³»´Þ¶ó°í ¿ä±¸ÇÑ´Ù. 
#define REQ_NPC_ADD						1000	
#define REQ_PC_ADD						1001
#define REQ_ITEM_ADD					1002

#define CMD_LOST_CONNECTION				127

#define CMD_CONNECT_INFO				200
#define CMD_CONNECT_INFO1				10000		// 1218 YGI

#define CMD_INTERFACE_PEACE				300
#define CMD_INTERFACE_BATTLE			301


// ¸Þ´º,..
#define CMD_CALL_SMALLMENU				2000
#define CMD_STORE_ITEMLIST				2001

#define CMD_DUAL_MAGIC_LEARN_BOOK		2111//soto-030421



#define CMD_ITEM_BUY_REQ				128
#define CMD_ITEM_SELL_REQ				129
#define CMD_MENU_MESSAGE				140

//------------------------------------------------------------------------------------
//±æµå Menu 
#define CMD_GUILD_INFO					7000
typedef struct server_guild_info
{
	short int guildindex;
	short int storeindex;
	short int repairindex;
}t_server_guild_info;


#define CMD_LEARN_SKILL					7010
typedef struct client_learn_skill
{ 
	short int guildindex;
	short int skillno;
}t_client_learn_skill;
//			( ¾î¶»°Ô..¹è¿ìÁö? ½Ç½ÀÀ» ÇÏ³ª? )
typedef struct server_learn_skill
{
	short int guildindex;
	short int skillno;
	short int money;
	short int skillexp;
}t_server_learn_skill;

	
#define CMD_LEARN_SKILLITEM				7020		
typedef struct server_learn_skillitem
{
	short int		skillitemno[ 30];
	short int		shillmoney[ 30];
	unsigned char	required_lv[30];
}t_server_learn_skillitem;


// Å¬¶óÀÌ¾ðÆ® -> ¼­¹ö : 
// ±æµå¿¡¼­ Item RepairÇÏ°íÀÚ ÇÏ´Â List¸¦ ¿ä±¸ÇÑ´Ù.   
#define CMD_ITEM_REPAIR_REQ				7030

#define CMD_REPAIR_ITEMLIST				7040
typedef struct server_repair_itemlist
{
	char  array;
	short int item[ 10]; // ±æµå¿¡¼­ Ãë±ÞÇÏ´Â Item¸ñ·Ï.
}t_server_repair_itemlist;


// 
#define CMD_ITEM_REPAIR					7050
typedef struct client_item_repair
{
	short int repairindex;
	short int itemposition;
}t_client_item_repair;
typedef struct server_item_repair
{
	short int	 itemposition;
	ItemAttr	 item;
	unsigned int money;
}t_server_item_repair;	


#define MM_RUMOR_				0		
#define MM_PUBLIC_NOTICE_		1
#define MM_RANK_				2
#define MM_FORUM_				3
#define MM_EVENT_				4
#define MM_MYDIARY_				5
#define MM_BBS_					6
#define MM_ETC_					7


#define CMD_RUMOR_REQ					7060
#define CMD_RUMOR_INFO					7070
#define CMD_I_NEED_RUMOR				7080
typedef struct server_rumor_info
{
	short int money;
}t_server_rumor_info;



#define CMD_PUBLIC_NOTICE_TITLE_REQ		7090
#define CMD_RANK_TITLE_REQ				7100
#define CMD_FORUM_TITLE_REQ				7110
#define CMD_EVENT_TITLE_REQ				7120
#define CMD_MYDIARY_TITLE_REQ			7130
#define CMD_BBS_TITLE_REQ				7140
#define CMD_ETC_TITLE_REQ				7150

#define CMD_MENU_MESSAGE_TITLE			7160

#define CMD_PUBLIC_NOTICE_TEXT_REQ		7170
#define CMD_RANK_TEXT_REQ				7180
#define CMD_FORUM_TEXT_REQ				7190
#define CMD_EVENT_TEXT_REQ				7200
#define CMD_MYDIARY_TEXT_REQ			7210
#define CMD_BBS_TEXT_REQ				7220
#define CMD_ETC_TEXT_REQ				7230

#define CMD_MENU_MESSAGE_TEXT			7240

typedef struct server_menu_message_title
{
	unsigned char   type;
	unsigned char   no;
	unsigned int    date;
	unsigned char   name[  NM_LENGTH ];
	char			title[ TEXT_LENGTH];
}t_server_menu_message_title;
typedef struct client_menu_message_title
{
	unsigned char type;
	unsigned char no;
}t_client_menu_message_title;
typedef struct server_menu_message_text
{
	char			text[ TEXT_LENGTH];
}t_server_menu_message_text;




//---------------------------------------------------------------------------------------



#define CMD_NOTIFY_JOIN_OK				400


#define CMD_LEVELUP_POINT				410
#define CMD_LEVELUP_POINT_OK			420
#define CMD_LEVELUP_POINT_NO			430

//-----------------  Animation---------------------
//-----------------  Animation---------------------

#define CMD_TURN_DIR					500



// -----------------   House -------------------------
// -----------------   House -------------------------
// -----------------   House -------------------------

#define CMD_ROOF_OPEN					8000
#define CMD_ROOF_CLOSE					8001
typedef struct roof_open
{
	short int roofno;
}t_roof_open;
typedef struct roof_close
{
	short int roofno;
}t_roof_close;







/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
typedef struct  server_interface_peace
{
	short int	client_id ;
}t_server_interface_peace;

typedef struct  server_interface_battle
{
	short int	client_id ;
}t_server_interface_battle;


//--------------------------------------------------------
typedef struct server_weather
{
	unsigned int	weather;
	unsigned int	another;
	unsigned int	rainstart;
	unsigned int	rainend;
	short			amount;
	short			temperature;
} t_server_weather;

struct t_server_npc_add
{	//< CSD-030419
	DWORD nX:15;
	DWORD nY:15;
	DWORD nStatus:2;
  
	DWORD nHomeX:10;
	DWORD nHomeY:10;
	DWORD nSprNo:9;
	DWORD nEventNpc:1;
	DWORD nAIType:1; // LTS AI 0 : Å¬¶óÀÌ¾ðÆ® AI, 1 : ¼­¹ö AI
	DWORD nRecall:1; 

	DWORD idNpc:16;
	DWORD nNameNo:13;
	DWORD nMutant:3;

	DWORD nViewType:4;
	DWORD nAttr:4;
	DWORD nRace:4;
};	//> CSD-0304198

typedef struct server_npc_remove
{	
	short int		server_id;
} t_server_npc_remove;
//........... Magic............................................
struct t_server_change_equip
{	//< CSD-031001
	unsigned int	server_id;
	unsigned char	equip[4];
	short int		mantle;		// 011018 KHS ¸Á¶Ç
	unsigned char   jjing;
	short int		ChairNum;		// LTS SITDOWN BUG
	char			Horse[4];		// LTS HORSERIDER
	int				mantleDyeColour; // Used with Mantle Dye
	WORD idWeaponItem;
};	//> CSD-031001

typedef struct t_client_change_equip
{
	unsigned char	equip[4];
}client_change_equip;


// Attack........................................
// Attack........................................
// Attack........................................
typedef struct client_attack										
{														
	short int		target_id;								
} t_client_attack;											
														
typedef struct server_attack							
{														
	short int		server_id;						
	short int		target_id;					
} t_server_attack;	




typedef struct send_item
{
	char		type;	//0 背包，1 银行					
	char		targetName[20];
	ITEMATTR	item;
} t_send_item;

												
											
// Item......................................
// Item......................................
// Item......................................
typedef struct server_item_add 		
{
	short int		item_id;
	ItemAttr		item;
	short int		offx, offy;
	short int		x, y;
} t_server_item_add;

// »õ·Î¿î ½ºÆ®·°ÃÄ this lsw
typedef struct server_item_add_ziped
{
	BYTE zipedpack[50];
} t_server_item_add_ziped;

typedef struct server_item_remove
{
	short int		item_id;
} t_server_item_remove;
typedef struct server_you_got_item
{
	short int		item_id;
} t_server_you_got_item;
typedef struct client_item_drop
{
	ItemAttr		item;
	short int		x,y;
} t_client_item_drop;
typedef struct client_item_take
{
	short int		item_id;
} t_client_item_take;
typedef struct client_item_status
{
	short int		item_id;
	unsigned int 	attr[6];
} t_client_item_status;
typedef struct server_item_status
{
	short int		item_id;
	unsigned int 	attr[6];
} t_server_item_status;
typedef struct chr_item_info0
{
	char        ar0;
	char        ar1;
	ItemAttr	inv[8];
} t_chr_item_info0;
typedef struct chr_item_info1
{
	ItemAttr	equip[8];
} t_chr_item_info1;
typedef struct chr_item_info2
{
	ItemAttr	quick[6];
} t_chr_item_info2;
typedef struct chr_name_info
{
	char	name[ NM_LENGTH];
	char	login_id[20];
} t_chr_name_info;

//---





// Bank........................................
// Bank........................................
// Bank........................................
typedef struct client_bank		
{
	int				total_money;
	int				money;
	int				char_money;
	DWORD			time;
} t_client_bank;



// Event ................................................ 
// Event ................................................
// Event ................................................

typedef struct client_event
{
	short int type;
	short int event_no;
} t_client_event;

typedef struct item_attr_change
{
	short int		item_id;
	char			array;
	unsigned int	attr;
}t_item_attr_change;





/// Change Map .........................................
/// Change Map .........................................
/// Change Map .........................................
typedef struct server_change_map
{
	short int		server_id;
	char			mapname[ NM_LENGTH ];
	short int		x;
	short int		y;
	int				port;
} t_server_change_map;


//acer7---------------------------
const int MAX_MY_STRING = 20;
typedef struct CLIENTACCESSLOGIN
{
	HSEL_INITIAL	init;
	char			id[ID_LENGTH];  
	char			pw[PW_LENGTH];
	short int		version;
	char			ip[IP_LENGTH];		//Added by zoung
	WORD			wPort;		// Added by chan78 at 2000/12/17
	BYTE			Foo;
	short int		Corp_Code;	//¾÷Ã¼ ÄÚµå
	char			mystring[MAX_MY_STRING];
	char			User_ID[ID_LENGTH];	//Á¾·®Á¦ ¾÷Ã¼¿ë ID
	char			GateWayIP[IP_LENGTH];
	short int		mycode;	
	int				GateWayPORT;
}t_ClientAccessLogin, *LPCLIENTACCESSLOGIN;


//--------------------------------------------------------
typedef struct server_accept_login
{
	short int		server_id;

	char			name[ MAX_CHARACTEROFID][NM_LENGTH];
	unsigned char	level[ MAX_CHARACTEROFID];
	unsigned char	job[ MAX_CHARACTEROFID];
	unsigned char	cla[ MAX_CHARACTEROFID];
	unsigned char	gender[ MAX_CHARACTEROFID];
	
	unsigned char   bodyr[ MAX_CHARACTEROFID];
	unsigned char   bodyg[ MAX_CHARACTEROFID];
	unsigned char   bodyb[ MAX_CHARACTEROFID];

	unsigned char   clothr[ MAX_CHARACTEROFID];
	unsigned char   clothg[ MAX_CHARACTEROFID];
	unsigned char   clothb[ MAX_CHARACTEROFID];

	short int		age[ MAX_CHARACTEROFID];
	unsigned int	money[ MAX_CHARACTEROFID];
	unsigned char	acc_equip1[ MAX_CHARACTEROFID];
	unsigned char	acc_equip2[ MAX_CHARACTEROFID];
	unsigned char	acc_equip3[ MAX_CHARACTEROFID];
	unsigned char	acc_equip4[ MAX_CHARACTEROFID];
	char			nation;		

	short int		remained_day;


	char			id[ ID_LENGTH];
	bool bIsBadUser;
} t_server_accept_login;

//--------------------------------------------------------
typedef struct client_connect_info
{
	char			name[NM_LENGTH];
	char			startposition;
	char			startmap;
} t_client_connect_info;

typedef struct server_connect_info	// »ç¿ë¾ÈÇÔ.
{
	char			startposition;
	int				port;
	char			ip[3][IP_LENGTH];
} t_server_connect_info;
	
//--------------------------------------------------------
typedef struct client_chat_data
{	
	short int		range;
	unsigned char	type;
	unsigned char	length;
	char			text[TEXT_LENGTH];
} t_client_chat_data;
	
//--------------------------------------------------------
typedef struct server_chat_data
{	
	short int		server_id;
	unsigned char   type;
	unsigned char	length;
	char			text[TEXT_LENGTH];
} t_server_chat_data;
	
//--------------------------------------------------------
typedef struct client_move
{	
	short int		server_id;
	short int		length;
	short int		sx;
	short int		sy;
	short int		gox;
	short int		goy;
	unsigned char	movetype;
	short int       movep;
	unsigned char	dir_tbl[MAX_DIR_TABLE];
} t_client_move;
	
//--------------------------------------------------------
typedef struct server_move
{	
	short int		server_id;
	short int		length;
	short int		sx;
	short int		sy;
	short int		gox;
	short int		goy;
	unsigned char	movetype;
	short int		targetid;
	unsigned int	desttime;
	unsigned char	dir_tbl[MAX_DIR_TABLE];
} t_server_move;
	
//--------------------------------------------------------
typedef struct client_create_char
{	
	char			name[NM_LENGTH];
	unsigned char	gender;
	unsigned char	face;
	short int		age;
	unsigned char	arigement;
	unsigned char	body_r, body_g, body_b;
	unsigned char	cloth_r, cloth_g, cloth_b;
	unsigned char	tactics;
	unsigned char	Class;
	unsigned char	job;
	unsigned char	spell;
} t_client_create_char;


typedef struct client_delete_char//020809 lsw
{	
	char	szSecretKeyCode[51];
	char	name[NM_LENGTH];
}t_client_delete_char;
	
typedef struct ACCESSJOIN
{	
	char			name[NM_LENGTH];
	char			ip[IP_LENGTH];
	char			id[ID_LENGTH];
	char			pw[PW_LENGTH];
	short			nBillingType;
	char			startposition;		// 99ÀÌ¸é ¸Ç ¸¶Áö¸·¿¡ LogOutÇÑÀÚ¸®¿¡¼­ ³ªÅ¸³­´Ù. 
}t_AccessJoin,*LPACCESSJOIN;
	
typedef struct server_assign_server_id
{	
	short int		x, y;
	short int		server_id;
}t_server_assign_server_id;
	
//// LoginServer¿Í GameServer°£ÀÇ Access Join°ü°è..
#define CMD_REQ_INSERT_USERID			34
#define CMD_REQ_DELETE_USERID			35
#define CMD_JOINABLE					36
#define CMD_NOT_JOINABLE				37
#define CMD_UPDATING					38
	
typedef struct gs_req_insert_userid
{	
	short int server_id;
	char id[ ID_LENGTH];
	char pw[ PW_LENGTH];
	char name[ NM_LENGTH];
	char mapname[ NM_LENGTH];
}t_gs_req_insert_userid;
	
typedef struct gs_req_delete_userid
{	
	short int server_id;
	char id[ ID_LENGTH];
	char mapname[ NM_LENGTH];
}t_gs_req_delete_userid;
	
	
	
	
	
	
typedef struct ls_joinable
{	
	short int server_id;
}t_ls_joinable;
	
typedef struct ls_not_joinable
{	
	short int server_id;
	
}t_ls_not_joinable;
	
	
typedef struct ls_updating		// LoginServer ->  GameServer.	¿äÃ»ÇÑ User´Â ÇöÀç Data¸¦ UpdatingÇÏ°í ÀÖ±â ¶§¹®¿¡ Àá½Ã ±â´Ù·Á¾ß ÇÕ´Ï´Ù. 
{	
	short int server_id;
	
}t_ls_updating;

struct t_server_user_db_data
{	//< CSD-031001
	short int lev;
	DWORD exp;
	char name[NM_LENGTH];
	short int Attr[10]; //MAX_CHARACTERDATA];
	short int Avility[20];
	DWORD money;

	int nLife;
	int nMaxHp;
	int nMana;
	int nMaxMp;
	int nHungry;
	int nMaxHungry;
	
	char spell;
	BYTE condition;
	
	short int sprno;
	short int x;
	short int y;
	
	unsigned char acc_equip1;
	unsigned char acc_equip2;
	unsigned char acc_equip3;
	unsigned char acc_equip4;
	
	char mapname[NM_LENGTH];
	
	unsigned char bodyr, bodyg, bodyb;
	unsigned char clothr,clothg,clothb;
	int sight;
	short int Age;
	
	short int openhouse;
	short int reserved_point;
	
	char viewtype;
	
	short int social_status;
	short int fame;
	short int fame_pk;		// 010915 LTS			//DB¿¡´Â NWCharcter°¡ fame_PK´ë½Å ÀúÀåµÈ´Ù
	NW_Character NWCharacter;	// 010915 LTS
	short int nk3;
	short int nk4;
	short int nk6;

	DWORD nation;			//1004 YGI
	char aStepInfo[20];
};	//> CSD-031001

	
//--------------------------------------------------------
typedef struct server_envir_info
{	
	DWORD			currtime_count;
	DWORD			weather_count;
	DWORD			rainstart;
	DWORD			rainend;
	short			amount;
	short			temperature;
	char			mapname[NM_LENGTH];
} t_server_envir_info;
	
struct t_server_user_add
{	//< CSD-031007
	DWORD nk3:16;
	DWORD server_id:15;
	DWORD peacests:1;

	DWORD body_r:5;
	DWORD body_g:5;
	DWORD body_b:5;
	DWORD cloth_r:5;
	DWORD cloth_g:5;
	DWORD cloth_b:5;
	DWORD bAlive:2;
	
	DWORD x:11;
	DWORD y:11;
	DWORD face:4;
	DWORD viewtype:4;
	DWORD spr_no:1;
	DWORD spell:1;

	DWORD equip0:8;
	DWORD equip1:8;
	DWORD equip2:8;
	DWORD equip3:8;

	DWORD namelen:5;
	DWORD lv:8;				// 0807 YGI
	DWORD nation:3;		// ¼ÓÇÑ³ª¶ó	0 : ¾øÀ½, 1, 2, 3: ¹ÙÀÌ¼­½º 4: ÀÚÀÌÆÝ 5:Çì°Ô¸ð´Ï¾Æ 6: ÀÏ½º		// 1004 YGI
	DWORD guild_master:3;		// ±æµå ¸¶½ºÅÍ
	DWORD king:1;		// ³ª¶ó ¿Õ
	DWORD imsi:9;		// ±æµå ÄÚµå ( 512°³ )	
	DWORD counselor:2;		// 0 : ¾øÀ½, 1: È¸»ç ¿î¿µÀÚ, 2: °ÔÀÓ³» ¿î¿µÀÚ
 	DWORD reporter:1;		// ±âÀÚ´Ü...

	DWORD nk4:16;
	DWORD nk6:16;

	DWORD SquadNo:3;	//	ºÎ´ë¹øÈ£
	DWORD isSquadLeader:1;	//	ºÎ´ëÀåÀÎ°¡?	
	DWORD YL_JoinNation:4;	//	ÀÏ½ºÀÎ°æ¿ì ÀÚÀÌÆÝ¿¡Çß³ª? ¹ÙÀÌ¼­½º¿¡ Çß³ª?  // ±¹°¡¹øÈ£
	DWORD nDualCls:4;
	DWORD nClsStep:4;
	DWORD mantle:16;	//  011018 KHS ¸Á¶Ç 

	DWORD isCommander:1;	//	»ç·É°üÀÎ°¡?
	DWORD jjing:4; //  011030 KHS JJING
 	DWORD JoinLocalWar:1;	// ±¹ÁöÀü¿¡ Âü¿© Çß´Ù.	// LTS LOCALWAR
	DWORD nCurrentAction:1;	// LTS SITDOWN BUG
	DWORD ChairNum:2;
	DWORD HorseNo:2;	// LTS HORSERIDER
	DWORD RiderNo:2;	// LTS HORSERIDER
	DWORD HorseGrade:2;	// LTS HORSERIDER
	DWORD RiderGrade:2;	// LTS HORSERIDER
	DWORD DragonLordWarTeam:2;	// LTS DRAGONLORD
	DWORD guild_code:16;

	DWORD idWeaponItem:16;
	DWORD class_type:3;		// 0807 YGI

	char name[20]; //Eleval put 20 to prevent any leak

	int MantleDyeColour; // Finito mantle dye

	//Char's options by Eleval
	bool hat_is_hidden;
	//End of it
};	//> CSD-031007

//--------------------------------------------------------
typedef struct server_user_remove
{	
	short int		server_id;
} t_server_user_remove;
	
//--------------------------------------------------------


typedef struct req_pc_add
{
	short int server_id;
}t_req_pc_add;

typedef struct req_npc_add
{
	short int server_id;
}t_req_npc_add;

typedef struct req_item_add
{
	short int server_id;
}t_req_item_add;

typedef struct element {
	unsigned char var_kind ;
	short int value ;
} t_element ;

typedef struct mass_data {
	unsigned char num ;
	t_element elements[100] ;
} t_mass_data ;


////////////////////////////////////////////////////////////////  Binary Update..................................
////////////////////////////////////////////////////////////////  Binary Update..................................
////////////////////////////////////////////////////////////////  Binary Update..................................
////////////////////////////////////////////////////////////////  Binary Update..................................
////////////////////////////////////////////////////////////////  Binary Update..................................
////////////////////////////////////////////////////////////////  Binary Update..................................


#define CMD_UPDATE_CHAR_DB			8500
#define CMD_UPDATE_BINARY_DATA0		8501
#define CMD_UPDATE_BINARY_DATA1		8502
#define CMD_UPDATE_SCRIPT_DATA		8503
#define CMD_UPDATE_INV_DATA			8504	
#define CMD_UPDATE_ITEM_DATA		8505
#define CMD_UPDATE_BANKITEM_DATA	8506	


struct t_update_char_db
{	//< CSD-030807
	char name[NM_LENGTH];
	char id[NM_LENGTH];
	short Level;
	short nGuildCode;
	short social_status; // ½ÅºÐ
	short fame;	// ¸í¼ºÄ¡
	NW_Character NWCharacter; // ±¹°¡Àü °ü·Ã ÇÊµå // 010915 LTS
	short Str;	
	short Con;	
	short Dex;
	short Wis;	
	short Int;
	short MoveP;	  
	short Char;
	short Endu;
	short Moral;	  
	short Luck ;	 
	short wsps;
	BYTE Tactics; // ¼±ÅÃÇÑ ÀüÅõ±â¼ú (º¸¿©ÁÖ±â À§ÇÔ)
	DWORD nation; // 1004 YGI
	DWORD Money;
	int	Hp;
	int	HpMax ;
	int	Mana;
	int	ManaMax;
	int Hungry;	
	int	HungryMax;
	BYTE Condition;	// Ä³¸¯ÅÍ »óÅÂ
	char MapName[NM_LENGTH];
	int Sight;		// YGI 020418
	short Age;
	BYTE bAlive; // Ä³¸¯ÅÍÀÇ »ý»ç¿©ºÎ
	DWORD killmon;	// 1°è¿­¸ó½ºÅÍ Å³¸µ¼ö
	DWORD killanimal; // µ¿¹° Å³¸µ¼ö
	DWORD killpc; // »ç¶÷, NPC Å³¸µ¼ö
	short nPoison; // ÀúÁÖ°è¿­ ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â
	short nCurse; // ÀúÁÖ°è¿­ ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â	
	short nFire; // ºÒ°è¿­ °ø°Ý ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â
	short nIce;	// ¾óÀ½°è¿­ °ø°Ý ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â
	short nElect; // Àü°Ý°è¿­ °ø°Ý ¸¶¹ý¿¡ ´ëÇÑ ÀúÇâ·Â
	short nHoly; // ½Å·Â°è¿­ °ø°Ý ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â
	short X;
	short Y;
	BYTE accessory[4];
	short mantle; // 011018 KHS ¸Á¶Ç
	short openhouse;
	short reserved_point ;
	DWORD BankMoney;
	DWORD win_defeat;
	DWORD LadderScore;
	DWORD LastLoan;
	DWORD Exp;
	char disease[6];					
	char viewtype;
	short nk3;
	short nk4;
	short nk6;
	char aStepInfo[20];
	int nCombatAmount;
	int	check_crc; //acer5
};	//> CSD-030807

typedef struct  server_update_binary_data0
{
	char            name[ NM_LENGTH];
	char            id[ NM_LENGTH];

	UCHAR			Ws[SIZE_OF_WS];
	UCHAR			Ps[SIZE_OF_PS];
	UCHAR			Skill[SIZE_OF_SKILL];
	UCHAR			skillexp[SIZE_OF_SKILL_EXP];
	UCHAR			tac_skillEXP[SIZE_OF_TAC_SKILL_EXP];
}t_server_update_binary_data0;

typedef struct  server_update_script_data
{
	char            name[ NM_LENGTH];
	char            id[ NM_LENGTH];
	UCHAR			script_var[SIZE_OF_SCRIPT_VAR];
} t_server_update_script_data;

typedef struct  server_update_binary_data1
{
	char            name[ NM_LENGTH];
	char            id[ NM_LENGTH];

	UCHAR			equip[SIZE_OF_EQUIP];
	UCHAR			quick[SIZE_OF_QUICK];
	UCHAR			party[SIZE_OF_PARTY];
	UCHAR			relation[SIZE_OF_RELATION];
	UCHAR			employment[SIZE_OF_EMPLOYMENT];		
} t_server_update_binary_data1;


typedef struct  server_update_inv_data
{
	char            name[ NM_LENGTH];
	char            id[ NM_LENGTH];
	UCHAR			inv[SIZE_OF_INV];
} t_server_update_inv_data;

typedef struct  server_update_item_data
{
	char            name[ NM_LENGTH];
	char            id[ NM_LENGTH];
	UCHAR			Item[SIZE_OF_ITEMINDEX];
} t_server_update_item_data;

typedef struct  server_update_bankitem_data
{
	char            name[ NM_LENGTH];
	char            id[ NM_LENGTH];
	char			mapname[ NM_LENGTH];
	UCHAR			bankitem[SIZE_OF_BANKITEM];
} t_server_update_bankitem_data;

////////////////////////////////////////////////////////////////  Binary Update..................................
////////////////////////////////////////////////////////////////  Binary Update..................................
////////////////////////////////////////////////////////////////  Binary Update..................................
////////////////////////////////////////////////////////////////  Binary Update..................................



// Character Basic Data*********************************************
typedef struct chr_status_info
{
	short int		Str;				//  Èû
	bool			fStr ;
	short int		Con;				//  °Ç°­
	bool			fCon ;
	short int		Dex;				//	¹ÎÃ¸¼º
	bool			fDex ;
	short int		Wis;				//	ÁöÇý
	bool			fWis ;
	short int		Int;				//	Áö´É
	bool			fInt ;
	short int		MoveP;				//	ÀÌµ¿·Â
	bool			fMoveP ;
	short int		Char;				//	¸Å·Â
	bool			fChar ;
	short int		Endu;				//  ÀúÇâ·Â
	bool			fEndu ;
	short int		Health;				//	Ã¼·Â
	bool			fHealth ;
	short int		Moral;				//	»ç±â
	bool			fMoral ;
	short int		Luck;				//	Çà¿î
	bool			fLuck ;

	short int		Resis_Poision;		//	µ¶/Á×À½ ÀúÇ×·Â
	bool			fPoison ;
	short int		Resis_Stone;		//	¼®È­ ¸¶ºñ ÀúÇ×·Â
	bool			fStone ;
	short int		Resis_Magic;		//	¸¶¹ýÀúÇ×·Â
	bool			fMagic ;
	short int		Resis_Fire;			//	ºÒ ÀúÇ×·Â
	bool			fFire ;
	short int		Resis_Ice;			//	¾óÀ½ÀúÇ×·Â
	bool			fIce ;
	short int		Resis_Elect;		//	Àü±âÀúÇâ·Â
	bool			fElect ;

	short int		Ap ;
	bool			fAp ;
		
	short int		Hp ;
	bool			fHp ;

	short int		HpMax ;
	bool			fHpMax ;
	
	short int		Mp ;
	bool			fMp ;
	short int		MpMax ;
	bool			fMpMax ;

	short int		Ac ;
	bool			fAc ;
	short int		Hit ;
	bool			fHit ;
	short int		Dam_Min, Dam_Max ;
	bool			fDamage ;
	short int		Tactics ;
	bool			fTactic ;
	
	short int		WsPs ;
	bool			fWsPs ;

	int		Sight ;		// 020418 YGI
	bool			fSight ;

} t_chr_status_info ;


typedef struct item_ex_info				// ¸Þ´º¿¡¼­ »ç¿ë (¾ÆÀÌÅÛ)( ¼­¹ö¿ë )
{
//	ItemAttr item_attr;
	int		itemno ;
	POS		pos_s;
	POS		pos_t;
} t_item_ex_info;

typedef struct item_exchange			// Å¬¶óÀÌ¾ðÆ®¿ë...
{
	int		item_no;
	POS		pos_s;
	POS		pos_t;
} t_item_exchange;
// Menu..
// ===========================================================================================
// ¼³¸í: Å¬¶óÀÌ¾ðÆ®°¡ ¾î¶² ¸Þ´º¸¦ ¶ç¿ïÁö ÀÌº¥Æ®¿¡ µû¶ó ¸Þ´ºID¸¦ ¾Ë·ÁÁÖ±âÀ§ÇÑ ±¸Á¶Ã¼
// Àü¼Û: SERVER ==> CLIENT
// Çì´õ: CMD_CALL_SMALLMENU
// -------------------------------------------------------------------------------------------
typedef struct server_call_smallmenu
{
	short int	menu;
	short int   storelistindex;
} t_server_call_smallmenu;


typedef struct tag_store_list_item		// °´Ã¼ // 0928 YGI
{
	short int item;
//	short int price;
}k_store_list_item;

typedef struct server_store_itemlist
{
	short int		no;
	short int		smallmenu;
	short int		storelistindex;
	k_store_list_item	store_item[MAX_STORE_ITEM_LIST];		// 0928 YGI
}t_server_store_itemlist;

typedef struct client_item_buy
{
	short int store_id;
	short int item_no;								//### 1217
	short int inv_position;
	int		  npc_id; // finito 12/07/07

}t_client_item_buy;
typedef struct client_item_sell
{
	short int store_id;
	short int item_position;
	int		  npc_id; // finito 25/06/08
}t_client_item_sell;

#define MM_RUMOR_				0
#define MM_ALL_BROADCAST_TEXT	1
#define MM_RANK					2
typedef struct server_menu_message
{
	unsigned char	type;
	char			text[TEXT_LENGTH];
}t_server_menu_message;
	

struct t_client_levelup_point
{
  WORD aPoint[11];
};

struct t_server_levelup_point
{
  WORD idServer;
  __int16 nPoint;
  __int16 nLevel;
  __int32 nLife;
  char aStepInfo[20];
};
//------------  Animation --------------------------------
typedef struct server_turn_dir
{
	short int  server_id;
	DIRECTION  dir;
}t_server_turn_dir;

// ----------------- Skill -------------------------------
// ----------------- Skill -------------------------------
// ----------------- Skill -------------------------------
// ----------------- Skill -------------------------------
// ----------------- Skill -------------------------------
//011004 lsw > 
/*
#define CMD_2ND_GM_MAKE					5600//010708 lsw
typedef struct client_2nd_gm_make
{
	short int skillno;
}t_client_2nd_gm_make;
*/
//011004 lsw < 

#define CMD_SKILL_INFO					6150 //010707 lsw ¼öÁ¤

typedef struct server_skill_info
{
	short int server_id;
	short int skillno;
	short int x, y;
	short int targettype;
	short int target_id;
}t_server_skill_info;

typedef struct client_skill_info
{
	short int skillno;
	short int x, y;
	short int targettype;
	short int target_id;
	POS		  s;
	char	TargetName[31];//020531 lsw
	int		iKey;//030102 lsw
}t_client_skill_info;



#define CMD_SKILL_RESULT				610
typedef struct server_skill_result
{
	short int skillno;
	short int x, y;
	short int result;
}t_server_skill_result;
typedef struct client_skill_result
{
	short int skillno;
	short int x, y;
	short int targettype;
	short int target_id;
}t_client_skill_result;


//1019 zhh	//defineÇÏ°í structureÃß°¡
#define CMD_SKILL_ANIMAL_LORE			701
typedef struct server_skill_animal_lore
{
	short int skillno;
	short int x, y;
	char  name[ NM_LENGTH];
	unsigned long int NowLife;
	unsigned long int MaxLife;
	unsigned long int exp;
	long int loyalty;
	long int damage;
	short int result;
	short int Level;
}t_server_skill_animal_lore;

const int CMD_OUTPUT_MESSAGE				=704;
const int CMD_SOUND_OUT						=705;
typedef struct server_output_message
{
	DWORD para1;
	DWORD para2;
}t_server_output_message;

#define CMD_SKILL_ITEM_MATTER			612
typedef struct client_skill_item_matter
{
	POS pos[20];
}t_client_skill_item_matter;


//011023 lsw >
#define CMD_SKILL_ITEMMAKE_RESULT		614
typedef struct server_skill_itemmake_result
{
	short int skillno;
	ItemAttr item;
	int			MakeItemCount;//011014 lsw
	int			TryItemCount;//011014 lsw
}t_server_skill_itemmake_result;
//011023 lsw <


#define CMD_SKILL_RESULT_FAIL			618
typedef struct server_skill_result_fail
{
	short int skillno;
	char      why;
}t_server_skill_result_fail;



#define	CMD_SKILL_RESULT_TOOTHER		619
typedef struct server_skill_result_toother
{	
	short int id;
	short     skillno;
	char	  result;
	
}t_server_skill_result_toother;
	


#define CMD_SKILL_CURSOR_VIEW			611

typedef struct client_skill_cursor_view
{
	short int skillno;
	short int x, y;
}t_client_skill_cursor_view;


#define CMD_SKILL_GAUGE					613

typedef struct client_skill_gauge
{
	short int skillno;
	short int x, y;

}t_client_skill_gauge;
typedef struct server_skill_gauge
{
	short int skillno;
	short int x, y;
	char      min;

}t_server_skill_gauge;


#define CMD_VIEWTYPE_CHANGE				620
typedef struct viewtype_change
{
	short int   server_id; 
	char		viewtype;
}t_viewtype_change;


#define CMD_VIEW_CHAR_INFO				622
typedef struct view_char_info
{
	short int server_id;
	short int type;
	DWORD data;
	char  killer[40];
}t_view_char_info;



// Chatting.................................................................
#define CMD_CHAT_SET				7500
typedef struct chat_set
{
	char chattype;
	char chatname[ NM_LENGTH];
}t_chat_set;
#define CMD_CHAT_SET_RESULT			7510		
typedef struct chat_set_result	// ¼³Á¤Çß´Âµ¥ »ó´ë°¡ ¾ø´Ù¸é ¾ø´Ù°í ¾ê±â ÇØÁØ´Ù. 
{
	char result;
}t_chat_set_result;
// .........................................................................

#define CMD_TOTAL_MAP_CONNECTIONS	7520
typedef struct  total_map_connections
{
	short int no;
}t_total_map_connections;

#define CMD_TOTAL_CONNECTIONS		7530
typedef struct  total_connections
{
	short int no;
}t_total_connections;


#define CMD_ISTHERE_CHARNAME		7540
typedef struct client_isthere_charname
{		
	char name[ NM_LENGTH];
}t_client_isthere_charname;
#define CMD_THEREIS_NO_CHARNAME		7541
#define CMD_THEREIS_CHARNAME		7542
		
//------------ NEW_APPEND -------------------------------
#define CMD_IM_GAME_SERVER         	9011
typedef struct im_game_server
{		
	int port;
}t_im_game_server;

#define CMD_TAME_COMMAND			8100
typedef struct tame_command
{
	BYTE	cmd;
	char	mybaby[ NM_LENGTH];
	char    toname[ NM_LENGTH]; 
}t_tame_command;

//-------------------------------------------------------
#define CMD_TAME_NAME_CHANGE		8101
typedef struct tame_name_change 
{
	short int id;
	char name[ NM_LENGTH];
}t_tame_name_change;

#define CMD_TAME_HOST_NAME			8102
typedef struct tame_host_name
{
	short int id;
	char hostname[ NM_LENGTH];
}t_tame_host_name;

#define CMD_TAME_COMMAND_RESULT		8103
typedef struct tame_command_result
{
	char	result;
}t_tame_command_result;

#define CMD_TAME_END				8104
typedef struct tame_end
{
	short int id;
}t_tame_end;

#define CMD_YOU_DIE					8200
typedef struct
{
	short int	id;
	short int	t;
}t_you_die;

#define CMD_DEBUG_REQ_CHAR_INFO		8300

typedef struct debug_server_req_char_info
{
	short int id;
	int		lv;
	int		exp;
	int		 ac;
	int		Str;				//  Èû
	int		Con;				//  °Ç°­
	int		Dex;				//	¹ÎÃ¸¼º
	int		Wis;				//	ÁöÇý
	int		Int;				//	Áö´É
	int		MoveP;				//	ÀÌµ¿·Â
	int		Char;				//	¸Å·Â
	int		Endu;				//  ÀúÇâ·Â
	int		Moral;				//	»ç±â
	int		Luck;				//	Çà¿î
	int		wsps;				//  ¸¶¹ý·Â->¸¶¹ý ¸Þ¸ð¶óÀÌÁî

	int     Hp, HpMax;
	int     Mp, MpMax;
	int		Hungry,HungryMax;		

	short int event_no;			// 0131 YGI

}t_debug_server_req_char_info;

typedef struct debug_client_req_char_info
{
	short int id;
}t_debug_client_req_char_info;


#define CMD_PLEASE_GIVE_LIFE		8400
#define CMD_PLEASE_GIVE_LIFE1		8401

struct t_please_give_life
{
	short int	id;
	short int	who;
	__int32   nLife;
	__int32   nMana;
	__int32   nHungry;
	
};

#define CMD_GOTO_EDELBROY_TOGETLIFE		8402
//////////////// lhs 0810 Á÷¾÷°ú Å¬·¡½º¿¡ °üÇÑ ÃÊº¸ Äù½ºÆ® ¼­¾à //////////
#define CMD_QUEST_JOB_FINISH			8409
///////////////////////////////////////////

#define CMD_SMILE_FACE			8410
typedef struct client_smile_face	
{
	char smileno;
}t_client_smile_face;
typedef struct server_smile_face	
{
	short int id;
	char  smileno;
}t_server_smile_face;

// °øÁö»çÇ× °ø°í....
#define		CMD_BBS					8411
typedef struct server_bbs
{
	char bbs[ MAX_PATH];
}t_server_bbs;

#define CMD_ACCESS_JOIN_YOU_ALREADY_IN_MAP			8412	// id¿Í Password¸¦ µé°í Á¢±ÙÇÏÁö¸¸. ´ç½ÅÀº ÀÌ¹Ì ¸Ê¿¡ ÀÖ´Ù. ¶Ç´Â ÇöÀç UpdateÁßÀÌ´Ù. 


#define CMD_HOW_MANY_IN_MAP							8413
typedef struct how_many_in_map
{
	char		map[ NM_LENGTH ];
	short int	how;
}t_how_many_in_map;

// °ÔÀÓ ¿î¿µÀÚ¿ë
#define CMD_BLOCK_ATTACK_START						8414
#define CMD_BLOCK_ATTACK_END						8415
#define CMD_BLOCK_MOVE_START 						8416
#define CMD_BLOCK_MOVE_END							8417
#define CMD_BLOCK_MAGIC_START						8418
#define CMD_BLOCK_MAGIC_END							8419
#define CMD_BLOCK_GIVE_LIFE_START					8420
#define CMD_BLOCK_GIVE_LIFE_END						8421

#define CMD_BLOCK_SKILL_START						8530//020821 lsw
#define CMD_BLOCK_SKILL_END							8531//020821 lsw

#define CMD_BLOCK_ANTI_TEAM_START					8532//reece
#define CMD_BLOCK_ANTI_TEAM_END						8533//reece

//GM commands, hijacking opcode - Thralas
#ifdef _DEBUG
#define CMD_CONFORM_SADONIX		9009
typedef struct CONFORMSADONIX
{
	int		cn;
	char	name[20];
	int		kind;
	ItemAttr item;
	int		x;
	int		y;
}t_ConformSadonix,*LPCONFORMSADONIX;
#endif
#define CMD_GM_GENERATE_ITEM				40653

#define CMD_SUMMON_MONSTER							8532
typedef struct summon_monster
{
	unsigned short id;
	unsigned short amount;
	unsigned short x;
	unsigned short y;
	unsigned short loot;
	unsigned short lootAmount;
	bool spread;
	bool summon;
	
}t_summon_monster;

#define CMD_SUMMON_KILLALL							8533

#define CMD_REMOVE_NK								8534
typedef struct remove_nk
{
	unsigned short id;
}t_remove_nk;

#define CMD_ABSOLUTE_LOGOUT							8422
typedef struct absolute_logout
{
	short id;
}t_absolute_logout;

#define CMD_MAKE_GHOST								8423
typedef struct make_ghost
{
	short id;
}t_make_ghost;


#define CMD_GIVE_LIFE								8425
typedef struct give_life 
{
	short id;
}t_give_life;




#define CMD_ITEM_DURATION_CHANGE					8426
typedef struct item_duration_change
{
	BYTE pos;
	WORD dur;
}t_item_duration_change;



////////////////////////////////////////////////////////////////////////////////////////
//
// NPC¸¦ Á¦¾îÇÏ±â À§ÇÑ ÇÁ·ÎÅäÄÝ
//

#define CMD_NPC_CHAT_DATA							8427
typedef struct npc_chat_data
{
	short int id;
	char      data[ TEXT_LENGTH];
}t_npc_chat_data;





#define CMD_NPCCTRL_SEALSTONE_			8901
typedef struct npcctrl_sealstone
{								
	short int id;				
	short int patterntype;		
}t_npcctrl_sealstone;				




#define CMD_NPCCTRL_MURI_8							8428
typedef struct npcctrl_muri_8	
{								
	short int id;				
	short int oldpatterntype;		
	short int patterntype;		
	short int bossid;
	DWORD	  Exp;
}t_npcctrl_muri_8;				
								
								
#define CMD_NPCCTRL_MURI_18							8429
typedef struct npcctrl_muri_18	
{								
	short int id;				
	short int oldpatterntype;	
	short int patterntype;		
	short int bossid;
	DWORD	  Exp;
}t_npcctrl_muri_18;				
								
									
#define CMD_NPCCTRL_MURI_28							8430
typedef struct npcctrl_muri_28 
{								
	short int id;				
	short int oldpatterntype;	
	short int patterntype;		
	short int bossid;
	DWORD	  Exp;

}t_npcctrl_muri_28;				

#define CMD_NPCCTRL_WANDER_MAUL						8431
typedef struct npcctrl_wander_maul 
{
	short int id;
	short int oldpatterntype;
	short int patterntype;
}t_npcctrl_wander_maul;

#define CMD_NPCCTRL_NO_MOVE							8432
typedef struct npcctrl_no_move 
{
	short int id;
	short int oldpatterntype;
	short int patterntype;
}t_npcctrl_no_move;

#define CMD_NPCCTRL_WANDER_GUARD					8433
typedef struct npcctrl_wander_guard 
{	
	short int id;
	short int oldpatterntype;
	short int patterntype;
}t_npcctrl_wander_guard;

#define CMD_NPCCTRL_WANDER_38						8434
typedef struct npcctrl_wander_38 
{
	short int id;
	short int oldpatterntype;
	short int patterntype;
	short int bossid;
}t_npcctrl_wander_38;


#define CMD_NPCCTRL_TAME	   						8435
typedef struct npcctrl_tame
{
	short int id;
	short int oldpatterntype;
	short int patterntype;
	short int bossid;
	short int HostId;
	char	  TargetName[31];
	short int tame;
	BYTE	  tame_cmd;
					
}t_npcctrl_tame;	
	
#define CMD_NPC_MODIFY_POS							8436
typedef struct npc_modify_pos
{				 	
	short int id;	
	short int mx, my;
}t_npc_modify_pos;	
					
							
#define CMD_SEND_NPC_PARAMETER						8437
typedef struct send_npc_parameter
{					
	short id;
	short type;
	short data;
}t_send_npc_parameter;
						
#define CMD_SEND_NPC_STRING_PARAMETER				8438
typedef struct send_npc_string_parameter
{							
	short id;			
	short type;				
	char data[ 31];		
}t_send_npc_string_parameter;
						
						
#define CMD_START_GOODBAD							8439
typedef struct start_goodbad
{						
	short int id;		
	short int nk3;
	short int nk4;
	short int nk6;
						
}t_start_goodbad;			
						
#define CMD_ENDOF_GOODBAD							8440
typedef struct endof_goodbad
{							
	short int id;			
}t_endof_goodbad;				
							
#define	CMD_LOGIN_BBS								8450
typedef struct login_bbs
{
	char	bbs[ MAX_PATH];
}t_login_bbs;



#define	CMD_CHAR_IN_GAME							8460	
typedef struct char_in_game							
{
	char name[ NM_LENGTH];
}t_char_in_game;

// Login Server -> Game Server
// id¸¦ °¡Áø ³ðÀ» ²÷¾î¶ó...
#define CMD_CLOSE_LOGIN_ID							8462
typedef struct login_close_login_id 
{
	char id[ NM_LENGTH];
}t_login_close_login_id;



#define CMD_UPDATE_VERY_IMPORTANT_STATUS			8465
typedef struct update_very_important_status
{
	char            name[ NM_LENGTH];
	
	short int		Level;

	short int		Str  ;	
	short int		Con  ;	
	short int		Dex  ;	
	short int		Wis  ;	
	short int		Int  ;	
	short int		MoveP;	  
	short int		Char ;	 
	short int		Endu ;	 
	short int		Moral;	  
	short int		Luck ;	 
	short int		wsps ;	 

	int		HpMax ;
	int		ManaMax;
	int		HungryMax;

	short int		reserved_point;

	DWORD			Exp;

}t_update_very_important_status;



#define CMD_UPDATE_VERY_IMPORTANT_TACTICS		8466
typedef struct update_very_important_tactics
{
	char name[ NM_LENGTH];
	char tac_skillEXP[ SIZE_OF_TAC_SKILL_EXP];
}t_update_very_important_tactics;

#define CMD_TRIP_AROUND		8468
typedef struct trip_around
{
	int x, y;
}t_trip_around;

#define CMD_TACTICS_PARRYING_EXP		8470
typedef struct tactics_parrying_exp
{
	DWORD exp;
	char step;
}t_tactics_parrying_exp;
	
#define CMD_DUAL_PROPOSAL			8472
typedef struct dual_proposal	
{	
	char name[ NM_LENGTH];
}t_dual_fight_proposal;
	
#define CMD_DUAL_PROPOSAL_ACCEPT	8472
typedef struct dual_proposal_accept
{	
	char name[NM_LENGTH];
}t_dual_proposal_accept;

#define CMD_DUAL_PROPOSAL_REFUSE	8472

#define CMD_DUAL_SURRENDER			8476



#define CMD_SERVER_ITEMTABLE_RELOAD	 8480


#define  CMD_DEBUG_CHANGE_SKILL_EXP		8490
typedef struct debug_change_skill_exp
{
	short int id;
	DWORD			skillexp[45];		// 1219 YGI °æÇèÄ¡


}t_debug_change_skill_exp;

#define CMD_SERVER_NPC_ACCESS_TABLE_RELOAD	8800
#define CMD_SERVER_NPC_ACCESS_TABLE_RELOAD2	8801
#define CMD_SERVER_NPC_ACCESS_TABLE_RELOAD3	8802
#define CMD_SERVER_NPC_ACCESS_TABLE_RELOAD4	8803

typedef struct server_npc_access_table_reload
{
	char data[ 800];
}t_server_npc_access_table_reload;



#define CMD_ABSOLUTE_MOVE_DUNGEON_KEEPER	8810

#define CMD_SEALSTONE_WAR_LEFTTIME			8850
typedef struct sealstone_warlefttime
{				
	char  nation;
	DWORD lefttime;
}t_sealstone_warlefttime;


#define CMD_SEALSTONE_WAR_START				8851
typedef struct sealstone_war_start
{				
	char  nation;
}t_sealstone_war_start;


typedef struct sealstone_client_warlefttime
{				
	DWORD lefttime;
}t_sealstone_client_warlefttime;


#define CMD_WAR_START_BLOCK					8852


	
#define CMD_SEALSTONE_RE_GENTIME			8860
typedef struct sealstone_regentime
{	
	char nation;
	DWORD regentime;
}t_sealstone_regentime;
 
 
#define CMD_SEALSTONE_START_RE_GENTIME		8869
#define CMD_SEALSTONE_END_RE_GENTIME		8870
 
 
#define CMD_SEALSTONE_RESULT				8880
#define CMD_SEALSTONE_RESULT_DEFEAT			8881
#define CMD_SEALSTONE_RESULT_VICTORY		8882
#define CMD_SEALSTONE_RESULT_DRWAW			8883

 
typedef struct sealstone_result
{	
	char aresult;
	char anation;
	
	char bresult;
	char bnation;
}t_sealstone_result;


#define CMD_SEALSTONEFROMKING				8890
typedef struct	sealstone_fromking
{
	char nation;
}t_sealstone_fromking;


#define CMD_SEALSTONE_STATUS				8892
typedef struct sealstone_status
{
	char sealstonesprno;
	short int id;
	char mapnumber;
	short int x,y;
	char status;

}t_sealstone_status;

typedef struct sealstone_client_status
{
	char mapnumber;
	short int x,y;
	char status;

}t_sealstone_client_status;


#define CMD_STEALSTONE_BREAKABLE			8894
#define CMD_STEALSTONE_NON_BREAKABLE		8896
#define CMD_STEALSTONE_NON_BREAKABLE_PEACE  8898

#define CMD_NATION2NATION_RELATION			8910

typedef struct nation2nation_relation
{
	char a,b;
	char relation;
	short int year, mon, day, hour;
	DWORD howlong;

}t_nation2nation_relation;

#define CMD_NATION2NATION_RELATION_RESULT		8912

typedef struct nation2nation_relation_result
{
	BYTE why;

}t_nation2nation_relation_result;

#define CMD_JUST_ANI				8900
typedef struct just_ani
{			
	short int id;
	char ani;
}t_just_ani;
			
			
#define CMD_FACE_DIRECTIONS			8920
typedef struct face_directions
{			
	char dir;
	short int id;
			
}t_face_directions;
			
			
			
#define CMD_AGENT_SERVER_IP			8930
typedef struct agent_server_ip
{			
	char ip[IP_LENGTH];
}t_agent_server_ip;
			
#define CMD_CHANGE_MAP_NOT				8940
#define CMD_CHANGEMAP_DATA_RECEIVED		8950

#define CMD_MANTLE_CANDLE				8960
typedef struct mantle_candle
{
	DWORD id		: 16;
	DWORD mantleno	: 14;
	DWORD candle	:  1;

}t_mantle_candle;


#define CMD_CANDLE_ON					8965
typedef struct candle_on
{
	WORD	id;
}t_candle_on;

#define CMD_CANDLE_OFF					8966
typedef struct candle_off
{
	WORD	id;
}t_candle_off;

#define CMD_INSTALL_TYPE				9000
typedef struct client_install_type
{
	BYTE type;

}t_client_install_type;

#define CMD_CLOSE_CONNECTION_NORMAL					13001
#define CMD_CLOSE_CONNECTION_ABNORMAL				13002
#define CMD_CLOSE_CONNECTION_SAME_ID_LOGON			13003
#define CMD_CLOSE_CONNECTION_SLEPT_TOO_LONG_TIME	13004
#define CMD_SO_MUCH_LOGINUSER_WAIT_FOR_A_MIN		13005
#define CMD_LOGIN_FAIL_MASSAGE						13006
#define	CMD_ORDER_DISCONNECT						9007
			
#pragma pack (push,1)
			
#include "network2.h"
#include "Network3.h"
#include "NetWork4.h"						// 010904 LTS
#include "Network5.h" 
#include "network6.h"
#include "Network7.h"		// 021015 kyo

// ´ë¸¸¿ë ·Î±×ÀÎ ½ÇÆÐ ¸Þ½ÃÁö 
typedef struct client_login_fail_reason
{
	int	dwType;				//·Î±×ÀÎÀÌ ¾î¶²ÀÌÀ¯¿¡¼­ ¾ÈµÇ´ÂÁöÀÇ ¿øÀÎ . switch¹® ¿ë
	char	szExpireDate[12];		//³¡³ª´Â ³¯Â¥°¡ µé¾î°£´Ù.
	char	cPaytype;			//°áÁ¦ÀÇ Á¾·ù°¡ µé¾î°£´Ù. ex) Æ÷ÀÎÆ®, ³¯Â¥. µîµî
	long	dwPoint;			//³²¾ÆÀÖ´Â Æ÷ÀÎÆ®
	char	cReserverdData;			//¸¸¾à °°Àº Å¸ÀÔÀÌ¶óµµ °æ¿ì¿¡ µû¶ó¼­ ´Ù¸¥ ¸Þ½ÃÁö°¡ ÂïÇô¾ß ÇÒ °æ¿ì¸¦ ´ëºñÇÑ º¯¼ö.
}t_client_login_fail_reason;

typedef struct ServerTime_
{
	DWORD nTime;

}t_ServerTime;
typedef struct ServerDate_
{
	DWORD g_wday;

}t_ServerDate;
typedef struct FriendStatus_
{
	bool online;
	char name[20];

}t_FriendStatus;

#define CMD_DUEL_ALLOWDENY 26384
typedef struct DuelAllowDeny_
{
	bool bIsDuelAllow;
}t_DuelAllowDeny;
		
typedef struct MantleDye_
{
	int mantleColor;
} t_MantleDye;

typedef struct MantleDyeOther_
{
	int mantleColor;
	int serverId;
} t_MantleDyeOther;

typedef struct MallItemList_
{
	short int mallItemCount; // Amount of items in the mall
	k_store_list_item mallItems[MAX_STORE_ITEM_LIST]; // Defines the items inside the mall
}t_MallItemList;

typedef struct MallItemBuy_
{
	short int itemNo;
	short int invPos;
	int		  npcId;
	int discount;

} t_MallItemBuy;
typedef struct MallItem
{
	ItemAttr itemAttr;
	int price; // The amount of raja emblems it costs to buy the item
	int	time;  // Some mall items have a time limit on them
	int discount;
}t_MallItemBuyNEW;

typedef struct dragon_header
{			
	short int		type;
	short int		size;
	char			crc;
} t_header;	

typedef struct application_of_exile_ //Eleval 12/06/09
{
	WORD idMaster;
} t_application_of_exile;

typedef struct check_guilds_ //Eleval 12/06/09
{
	short int idPlayer;
	WORD guildCode;
	char id[20];
} t_check_guilds;

typedef struct sex_change_ //Eleval 12/06/09 - Sex Change
{
	WORD idUser;
	short int iSex;
	char cId[20];
} t_sex_change;

typedef struct tool_play_sound_ //Eleval 19/06/09 - Tool Play Sound
{
	int sound_number;
} t_tool_play_sound;

typedef struct gm_toggle_santa_ //eleval 22/06/09 - Santa Power
{
	DWORD santa_power;
} t_gm_toggle_santa;

typedef struct lms_set_lvl_ //Eleval 18/07/09 - To set LVL Range in LMS Arena (Tool)
{
	short int min;
	short int max;
} t_lms_set_lvl;

typedef struct set_exp_event_ //Eleval 22/07/09 - To set an exp event (tool)
{
	short int multiplier;
	short int days[7];
	short int range;
	short int active;
} t_set_exp_event;

typedef struct tool_toggle_pk_ //Eleval 24/07/09 - To set if it's possible to attack or not
{
	short int type;
	short int lvl_min;
	short int lvl_max;
	short int range;
	short int restr;
} t_tool_toggle_pk;

typedef struct tool_quest_check_ //Eleval 13/08/09 - To check quest NPC
{
	bool force;
	int nX;
	int nY;
} t_tool_quest_check;

typedef struct change_char_options_ //Eleval 18/08/09 - To change the char's options
{
	int type;
	bool flag;

	DWORD CasterID;
} t_change_char_options;
typedef struct Test_
{
unsigned short itemno;
unsigned short	quantity;

} t_test;
typedef struct BanAccount_
{
	char characterName[20];
	char reason[255];
	char duration[255];

} t_ban_account;

typedef struct ForceNationWar_
{
	bool isStart;
	

} t_ForceNationWar;

typedef struct ForceDungeon_
{
	int index;
	bool isStart;
	int port;


} t_ForceDun;
typedef struct Generate_Item_
{
	int itemno;
	int type;
	int soksung1;
	int soksung2;
	int soksung3;
	int grade;
	int amount;
	int isHigh;
	bool isDynamicRare;
} t_generate_item;
//typedef struct BuffTime_
//{
//	int spellid;
//	int duration;//from mapserver
//	Spr* Icon;
//
//} t_buff_time;

//--------------------------------------------------------
typedef struct packet
{			
	union	
	{		
		char					data[sizeof(t_header)];
		t_header				header;
	} h;	
			
	union			
	{				
		char					data[MAX_PACKET_SIZE];
						
		t_server_weather		server_weather;
		t_server_npc_add		server_npc_add;
		t_server_npc_remove		server_npc_remove;
    t_client_attack			client_attack;
		t_server_attack			server_attack;

		t_server_modify_position				server_modify_position;
		t_client_modify_position				client_modify_position;
		t_server_modify_position_absolute		server_modify_position_absolute;

		t_server_item_add		server_item_add;
		t_server_item_remove	server_item_remove;
		t_server_you_got_item	server_you_got_item;
		t_client_item_drop		client_item_drop;
		t_client_item_take		client_item_take;
		t_client_item_status	client_item_status;
		t_server_item_status	server_item_status;

		t_client_bank			client_bank;

		CLIENTACCESSLOGIN		ClientAccessLogin;
		t_server_accept_login	server_accept_login;
		ACCESSJOIN				AccessJoin;
		t_server_accept_join	server_accept_join;
		t_server_assign_server_id server_assign_server_id;
		t_all_ready				all_ready;

		t_client_connect_info	client_connect_info;
		t_server_connect_info	server_connect_info;

		t_client_chat_data		client_chat_data;
		t_server_chat_data		server_chat_data;
		t_client_move			client_move;
		t_server_move			server_move;
		t_client_create_char	client_create_char;
		t_client_delete_char	client_delete_char;


		t_client_event							client_event; 
		t_item_attr_change						item_attr_change;
		t_item_door_open_close					item_door_open_close;
		t_client_item_box_break					client_item_box_break;
		t_server_item_box_break		 			server_item_box_break;
		t_client_item_box_break_result			client_item_box_break_result;

		t_client_item_box_magic_break			client_item_box_magic_break;
		t_server_item_box_magic_break_result	server_item_box_magic_break_result;

		t_client_just_attack_animation			client_just_attack_animation;

		t_server_change_map						server_change_map;


		t_server_foot_on_trap0		server_foot_on_trap0;
		t_client_foot_on_trap0		client_foot_on_trap0;
		t_server_foot_on_trap1		server_foot_on_trap1;
		t_client_foot_on_trap1		client_foot_on_trap1;

		t_server_steal_on_trap		server_steal_on_trap;//011212 lsw

		t_chr_item_info0			chr_item_info0;
		t_chr_item_info1			chr_item_info1;
		t_chr_item_info2			chr_item_info2;
		t_char_info_basic			char_info_basic;
		t_char_info_tactic          char_info_tactic; // CSD-TW-030624
		t_char_info_magic			char_info_magic;
		t_char_info_skill			char_info_skill;
		t_other_info_basic			other_info_basic;
		t_server_give_item			server_give_item;
		k_client_item_inv			client_item_inv;	// 1218
		t_char_info_tac_skillexp	char_info_tac_skillexp;

		t_req_pc_add				req_pc_add;
		t_req_npc_add				req_npc_add;
		t_req_item_add				req_item_add;


		t_server_user_db_data		server_user_db_data;
		t_server_user_add			server_user_add;
		t_server_envir_info			server_envir_info;
		t_server_user_remove		server_user_remove;

		t_item_ex_info				item_ex_info ;
		t_item_exchange				item_exchange;
		t_chr_status_info			chr_status_info ;

    t_server_change_equip		server_change_equip ;
		t_client_change_equip		client_change_equip ;
		t_mass_data					mass_data ;
		t_server_insert_magic		server_insert_magic;
		//t_inv_info				inv_info ;
		//t_equip_info				equip_info ;

		t_server_interface_battle	server_interface_battle ;
		t_server_interface_peace	server_interface_peace ;

		// Menu..
		t_server_call_smallmenu		server_call_smallmenu ;
		t_server_store_itemlist		server_store_itemlist;
		t_client_item_buy			client_item_buy ;
		t_client_item_sell			client_item_sell ;
		t_server_menu_message		server_menu_message;

		t_server_guild_info			server_guild_info;
		t_client_learn_skill		client_learn_skill;
		t_server_learn_skill		server_learn_skill;
		t_server_learn_skillitem	server_learn_skillitem;				//### 1214 ¼öÁ¤ 
		t_server_repair_itemlist	server_repair_itemlist;
		t_client_item_repair		client_item_repair;
		t_server_item_repair		server_item_repair;
		t_server_rumor_info			server_rumor_info;
		t_server_menu_message_title server_menu_message_title;
		t_client_menu_message_title client_menu_message_title;
		t_server_menu_message_text	server_menu_message_text;

		t_client_levelup_point		client_levelup_point;
		t_server_levelup_point		server_levelup_point;

		// Animation..
		t_server_turn_dir			server_turn_dir;

		// Skill..
		t_server_skill_info			server_skill_info;
		t_client_skill_info			client_skill_info;
		t_server_skill_result		server_skill_result;
		t_client_skill_result		client_skill_result;

		t_server_skill_animal_lore  server_skill_animal_lore;

		t_server_output_message		server_output_message;

		t_client_skill_item_matter		client_skill_item_matter;
		t_server_skill_itemmake_result	server_skill_itemmake_result;
		t_server_skill_result_fail		server_skill_result_fail;
		t_server_skill_result_toother	server_skill_result_toother;
		t_client_skill_cursor_view		client_skill_cursor_view;

		t_client_skill_gauge			client_skill_gauge;
		t_server_skill_gauge			server_skill_gauge;

		t_viewtype_change				viewtype_change;
		t_view_char_info				view_char_info;

		t_roof_open						roof_open;
		t_roof_close					roof_close;


		t_chat_set_result				chat_set_result;
		t_chat_set						chat_set;

		t_total_map_connections			total_map_connections;
		t_total_connections				total_connections;

		t_client_isthere_charname		client_isthere_charname;

		t_tame_command					tame_command;
		t_tame_name_change				tame_name_change;
		t_tame_host_name				tame_host_name;
		t_tame_command_result			tame_command_result;
		t_tame_end						tame_end;


		t_you_die						you_die;

		t_debug_server_req_char_info	debug_server_req_char_info;
		t_debug_client_req_char_info	debug_client_req_char_info;

		t_please_give_life				please_give_life;

		t_client_smile_face				client_smile_face;
		t_server_smile_face				server_smile_face;

		// °øÁö»çÇ×..
		t_server_bbs					server_bbs;


		// °ÔÀÓ¼­¹ö ÀÎ½Ä 
		t_im_game_server				im_game_server;

		// ¿î¿µÀÚ¿ë 
		t_absolute_logout				absolute_logout;
		t_make_ghost					make_ghost;
		t_give_life						give_life;
#ifdef _DEBUG
		t_ConformSadonix				ConformSadonix;
#endif
		t_summon_monster				summon_monster;
		t_remove_nk						remove_nk;

		// Update to LoginServer....
		//t_update_char_db				update_char_db;
		t_server_update_binary_data0	server_update_binary_data0;
		t_server_update_binary_data1	server_update_binary_data1;
		t_server_update_script_data		server_update_script_data;
		t_server_update_inv_data		server_update_inv_data;
		t_server_update_item_data		server_update_item_data;
		t_server_update_bankitem_data	server_update_bankitem_data;


		// GameServer¿Í  LoginServer¿ÍÀÇ Access Join°ü°è..
		t_gs_req_insert_userid	gs_req_insert_userid;
		t_gs_req_delete_userid	gs_req_delete_userid;
		t_ls_joinable			ls_joinable;
		t_ls_not_joinable		ls_not_joinable;
		t_ls_updating			ls_updating;

		t_how_many_in_map		how_many_in_map;

		// Item Duration °¨¼ÒÃ³¸®....
		t_item_duration_change	item_duration_change;


		// Client¿¡¼­ NPC°ü¸®¸¦ À§ÇÑ ÇÁ·ÎÅäÄÝ
//		t_npc_attack			npc_attack;
		t_npc_chat_data			npc_chat_data;

		t_npcctrl_sealstone		npcctrl_sealstone;
		t_npcctrl_muri_8		npcctrl_muri_8;
		t_npcctrl_muri_18		npcctrl_muri_18;
		t_npcctrl_muri_28		npcctrl_muri_28;		
		t_npcctrl_wander_38		npcctrl_wander_38;
		t_npcctrl_wander_maul	npcctrl_wander_maul;
		t_npcctrl_no_move		npcctrl_no_move;
		t_npcctrl_wander_guard	npcctrl_wander_guard;
		t_npcctrl_tame			npcctrl_tame;

		t_npc_modify_pos					npc_modify_pos;
		t_send_npc_parameter				send_npc_parameter;
		t_send_npc_string_parameter			send_npc_string_parameter;

		t_start_goodbad						start_goodbad;
		t_endof_goodbad						endof_goodbad;

		t_login_bbs							login_bbs;

		t_char_in_game						char_in_game;	// °ÔÀÓ¼­¹ö¿¡ ±×·¯ÇÑ ³ðÀÌ ÀÕ´Â°¡ ¾ø´Â°¡¸¦ CheckÇÑ´Ù. 
		t_login_close_login_id				login_close_login_id;

		t_update_very_important_status		update_very_important_status;
		t_update_very_important_tactics		update_very_important_tactics;

		t_tactics_parrying_exp				tactics_parrying_exp;

		t_debug_change_skill_exp			debug_change_skill_exp;

		// 0811 NPC KHS
		t_server_npc_access_table_reload	server_npc_access_table_reload;

		// ±¹°¡Àü¿¡ ´ëÇÑ packet.
		t_sealstone_warlefttime				sealstone_warlefttime;
		t_sealstone_war_start				sealstone_war_start;
		t_sealstone_client_warlefttime		sealstone_client_warlefttime;
		t_sealstone_result					sealstone_result;
		t_sealstone_fromking				sealstone_fromking;
		t_sealstone_status					sealstone_status;			
		t_sealstone_client_status			sealstone_client_status;

		t_nation2nation_relation_result		nation2nation_relation_result;
		t_nation2nation_relation			nation2nation_relation;
		
		t_just_ani							just_ani;
		t_face_directions					face_directions;

		t_agent_server_ip					agent_server_ip;


		t_mantle_candle						mantle_candle;

		t_candle_on							candle_on;
		t_candle_off						candle_off;


		t_client_install_type				client_install_type;
		t_server_item_add_ziped				server_item_add_ziped;

		t_server_check_accel				server_check_accel;//010604 lsw
		t_SkillRareUpgrade					SkillRareUpgrade;
		t_SkillRareUpgradeResult			SkillRareUpgradeResult;
		t_server_call_smallmenucustom		CallSmallMenuCustom;
		t_RareUpgradeBBS					RareUpgradeBBS;
		t_DayLight							DayLight;//020610 lsw
		t_NowSkillPoint						NowSkillPoint;//020815 lsw

		t_OnConnectHselInit					OnConnectHSELInit;
		t_client_login_fail_reason	client_login_fail_reason;

		t_angra angra;
		t_kein kein;
		t_NationWar	NationWar; // 010904 LTS
		t_LocalWar	LocalWar;	// LTS LOCALWAR
		t_battle battle;
		t_strike strike;
		t_magic  magic;
		t_combat combat;
		t_dual   dual;
		t_arena  arena; // CSD-030506
		t_rm_change_weather RmChangeWeather;
		t_AddEffect AddEffect;//020314 lsw 
	  
		t_SearchPacketClient		SearchPacketClient;
		t_SearchResultList			SearchResultList;
		t_MerchantItemBuy			MerchantItemBuy;

		t_SellerItemRequest			SellerItemRequest;
		t_SellerItemRegister		SellerItemRegister;
		
		t_SellerItemDelete			SellerItemDelete;
		t_MerchantResultTake		MerchantResultTake;//021025 lsw
		t_MerchantExchangeRequest	MerchantExchangeRequest;
		t_Hwoarang					Hwoa;//021111 lsw

	  	t_quest_in_map	quest_in_map;		// 021010 kyo
		t_show_msg		show_msg;		// 021014 kyo
		t_script_timer		script_timer;		// 021022 kyo
		t_script_counter	script_counter;		// 021023 kyo
		t_script_spellmapmove	spell_mapmove;	// 021128 kyo
		t_GUARDIANLIST		GuardianList; // soto-030331
		t_SYMBOL_ITEM_MSG	Symbol_Item;//soto-030418
		t_BUY_LOTTO			Lotto_Buy;
		t_CHECK_WINNER		Check_Winner;
		t_WINNER_MENU		Lotto_Winner_Menu;
		t_OPEN_LOTTO_MENU	Lotto_Menu_Open;
		t_ACCEL_TYPE		Accel_Type; // CSD-030808

		t_LOTTERY_INFO		Lotto_BroadCast;//soto-HK030924

		t_LOTTOPAPER_SEEK	Lotto_Seek;//soto-LottoÃß°¡.
		t_ServerTime		ServerTime;
		t_ServerDate        ServerDate;
		t_FriendStatus		FriendStatus; // finito 09/08/07
		t_DuelAllowDeny		DuelAllowDeny; // finito 25/08/7 
		t_MantleDye			MantleDye; // finito 25/06/08
		t_MantleDyeOther	MantleDyeOther; // finito 25/06/08
		t_MallItemList		MallItemList; // finito 29/06/08
		t_MallItemBuy		MallItemBuy; // finito 29/06/08
		t_MallItemBuyNEW   MallItemBuyNew;
		t_application_of_exile	application_of_exile; //Eleval 12/06/09
		t_check_guilds			check_guilds; //Eleval 12/06/09

		t_sex_change			sex_change; //Eleval 12/06/09 - Sex Change

		t_tool_play_sound		tool_play_sound;	//Eleval 19/06/09 - Tool Play Sound
		t_gm_toggle_santa		gm_toggle_santa;	//Eleval 22/06/09 - To let GMs removing their Santa Power
		t_lms_set_lvl			lms_set_lvl;		//Eleval 18/07/09 - To set level range in LMS Arena
		t_set_exp_event			set_exp_event;		//Eleval 22/07/09 - To set an Exp Event
		t_tool_toggle_pk		tool_toggle_pk;		//Eleval 24/07/09 - To set pk or battle in a map
		t_tool_quest_check		tool_quest_check;	//Eleval 13/08/09 - To check quest NPC

		t_change_char_options	change_char_options; //eleval 18/08/09 - To change a char's options

		t_ban_account		BanAccount;
		t_generate_item		GenerateItem;
		t_ForceNationWar    ForceNationWar;
		t_ForceDun          ForceDungeon;
		//t_buff_time         BuffTime;
		
	} u;
	
	struct packet *next;

} t_packet;


#pragma pack (pop)



//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------

//#endif
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------

typedef struct connection 
{
	SOCKET			socket;
	struct			sockaddr_in addr;
	int				state;
	t_packet		*inbuf;
	t_packet		*outbuf;
	int				receive;
	int				receive_count;
    DWORD			connect_time;
	DWORD			monitor_time;
	DWORD			send_bytes;
	DWORD			receive_bytes;
	int				send_try;		// º¸³»±â È½¼ö...

	int				send_addlen;

	char			ip_address[128];

	int				last_year;
	int				last_mon;
	int				last_day;
	int				last_hour;
	int				last_min;

	int				login_year;
	int				login_mon;
	int				login_day;
	int				login_hour;
	int				login_min;
	int				login_sec;

	t_packet		packet;
	int				packet_pnt;
	int				packet_size;
	int				packet_status;
	int				packet_count[2];
	char			len[2]; // t_packet¾Õ¿¡ ¿À´Â packet length...
	WORD			*udwp_len;

	int				kick_out;
	DWORD			kick_out_time;

	int				save_db;
	DWORD			save_db_time;

	char			id[ID_LENGTH];
	char			pw[PW_LENGTH];
	char			name[NM_LENGTH];
	char			mapname[NM_LENGTH];

	//------------ NEW_APPEND -------------------------------
	int				server_check;
	//-------------------------------------------------------

#ifndef __CLIENT_PROGRAM__
    CHARLIST		chrlst;
#endif


#ifdef _GAME_SERVER_
	AREA			area_tbl[MAX_AREA_BUFFER];
#endif

	char		*SendBuf;				// send()·Î º¸³¾ ³»¿ëÀ» ´ÙÀ½¿¡ ´ã¾Æ ³õ´Â´Ù. 
	int			SendBufSize;			// send()·Î º¸³¾ ³»¿ëÀÇ Å©±â. 
	int			SendErr;				// send()·Î º¸³¾ ³»¿ëÀÇ Å©±â. 

} t_connection;

extern t_connection	connections;
extern DWORD 		g_ping_delay;
extern DWORD 		g_ping_time;


///////////////////////////////////////////////////////////////////////////////
//

#ifdef __cplusplus
extern "C"
{
#endif
	extern int		ClearServer( t_connection *c );
	extern bool		Initialize( void );//¼ÒÄÏ ÃÊ±âÈ­
	extern void		Release( t_connection *c );//¼ÒÄÏ ÇØÁ¦
	extern int		ConnectServer( t_connection *c, LPSTR lpIP, int nPort );
	extern int		ConnectLogin( t_connection *c, LPSTR ID, LPSTR PW ,int LineNum );

	extern void		CheckBytesRoutine(void);
	extern void		InitBytesRoutine(void);

	extern void		SendChatData( LPSTR lpStr ,const int iChatType = CHATTYPE_NORMAL);//020701 lsw
	extern void		RecvChatData( t_server_chat_data *p );
	extern void		SendChatSet( char chattype, char *name );
	extern void		SendMoveData( LPCHARACTER ch );
	extern void		RecvMoveData( t_server_move *p );
	extern void		RecvWeatherData( t_server_weather *p );
	
	extern void     SendMagicData( int id, int x, int y, int magic_index );
	extern void     RecvMagicData( int genid, int tarid, int x, int y, int magic_index );
	
	extern void		SendDropItem( int itemno, int x, int y, unsigned int attr[6] );
	extern void		SendTakeItem( int item_id );
	
	
	extern int		SendCreateCharacter( SCharacter *s );
	extern void		SendAllReady( void );
	extern void		SendInstallType( void );
	extern void		SendLogOut( void );
	
	
	extern void		RecvUserAdd( t_server_user_add *c );
	extern void		RecvUserRemove( int server_id );
	extern int		SendUserRemove( void );
	
	extern int		GetNumFromServerID( int server_id );
	extern int		GetAliveCount( void );
	extern int		QueuePacket( t_packet *packet, int type );
	extern int		DequeuePacket( t_packet *packet, int type );
	extern int		HandleCommand(  t_connection *c);
	extern int		HandleCommand2(t_packet &packet,t_connection *c);//011212 lsw 

	extern int		HandleReading( void );
	extern int		HandleWriting( t_connection *c );
	extern void		SendAliveCheckRoutine( void );
	extern int		HandleRunning( t_connection *c, bool flag = true );
	
	extern int		SendChoiceCharacter( int ch );

	extern void		SendEvent( int type, int id );

	extern void		SendMoveItem( POS s, POS t);
	extern void		RecvChrItemInfo_0( t_chr_item_info0 *i );
	extern void		RecvChrItemInfo_1( t_chr_item_info1 *i );
	extern void		RecvChrItemInfo_2( t_chr_item_info2 *i );
	extern void		RecvItemEquip( t_chr_status_info *chr );		// °èÀÎ Ãß°¡

	extern void		SendChangeEquip( LPCHARACTER ch );

	extern void CalcTacticsSkillExp( int tac_skill, DWORD exp, char step );


	extern void		SendMagicInfo( int target_id, int x, int y, int magic_no );

	extern void		SendPeaceBattle( LPCHARACTER ch );
	extern void		RecvPeaceBattle( int server_id, int peacests );
	extern int GetAgentServerIPAddress( t_connection *c );
#ifdef __cplusplus
}
#endif

#endif//AFX_DRAGON_PROTOCOL_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_
    
