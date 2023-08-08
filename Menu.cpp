// menu.cpp
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
#include "Item.h"
#include "MenuNetwork.h"
#include "MouseCursor.h"
#include "CharDataTable.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "NationSys.h" // 010904 LTS  // For Nation War Interface
#include "NationSupport.h" // 010904 LTS  // For Nation War Interface
//011001 lsw
#include "skill_lsw.h"
#include "linecommand.h"
#include "hong_light.h"
#include "menudef2.h"
#include "MagicSys.h"
#include "chat.h"//020704 lsw
#include "CurrentMsgMgr.h"
#include "Auction.h"
#include "MailMgr.h"
#include "HunterSystem.h"
#include "ExchangeMgr.h"
extern bool isNewWarfieldServer();
#include "QuestFunction.h"	// 021025 kyo 
#include "SymbolMgr_Client.h"//soto-1.02
#include "LottoMgr.h"//soto-030501
#include "ArenaManager.h"

extern CSymbolMgr_Client	g_SymBolMgr;//soto-1.02
//< LTH-040811-KO 1.04p °¨¿Á ½Ã½ºÅÛÀ» À§ÇØ Ãß°¡
#include "Prison.h"

extern CPrison g_cPrison;
//> LTH-040811-KO
extern void MenuSubProcessType2(SMENU *SubMenu,const int j);	//021022 kyo

extern int g_iScenarioOn;
extern void lsw_MenuSubProcessType(SMENU *SubMenu,const int j);//020214 lsw
extern void lsw_DoLButtonDownOfMenu(const int i,const int j);
extern void lsw_DoRButtonDownOfMenu(const int i,const int j);
extern void MenuSubProcessType_FT_DO(SMENU *SubMenu,int j);
extern void	DisplayInventoryMenu(SMENU *SubMenu ,int j);//ÀÎº¥Åä¸® µð½ºÇÃ·¹ÀÌ
extern void ButtonDownAtInventory(const int i, const int j);//021111 lsw
extern void DisplayQuickMenu(SMENU *SubMenu ,int j);//ÄüÃ¢ µð½ºÇÃ·¹ÀÌ
extern void DisplayCandleMenu(SMENU *SubMenu ,int j);
extern void LocalWarDoneBoxText(bool Button);						// LTS LOCALWAR MENU
extern void SendWarMove();
extern void SendWarStatusRefresh();

int g_SelectMail = 0;
int hammer = 0; // Add by rogerio
int hammer_itemid; // Add by rogerio
int hammer_itempos; // Add by rogerio

/*
 * Rogério Segovia
 * 06/05/2009
 * Adicionado para o item magico
*/

int magicItem = 0; 
int magicItem_itemid;
int magicItem_itempos;

CGuildIntroduction *g_Guild_Introduction;

CMenuVariable g_Menu_Variable;
char *g_szScale;

#ifdef CHANGE_MONEY_
	char g_szMoneyName[] = "Crit";
#else
	char g_szMoneyName[] = "Cell";
#endif

//#define PAGE_NUMBER__	 2//020515 lsw

extern void SendNationPacket(int type);//010730 lsw
extern bool isNationWarfieldServer();
extern int CheckNationStatus(int StatusNo);
extern int WarResultAniCount;
extern void	lsw_FT_DATA_PUT(SMENU *SubMenu ,int iDP_TYPE);


//extern int GetItemKind( int itemno );
RareMain Soksungattr_gm; 

extern MakeItem gm_make_item_result;
extern int gm_make_item_result_needitem[2];

extern MakeItem gm_make_item_list[MAX_UPGRADE_RARE];

extern bool DoButtonCheckOfMenu_lsw(int i , int j);


//_____________________________________________________________________	// 010904
extern int g_Squad;														// LTS TEMP
extern int g_Commander;

extern void SendCMD_NW_MAP_MOVE(int Type,int WarfieldNo); 
extern void SendCMD_SET_SQUAD_LODING_POINT(int SquadNo,int LoadingPoint); 
extern void SendCMD_REQUEST_SQUAD_INFO(int SquadNo);
extern void SendCMD_REQUEST_COMMANDER_VOTE_DATA(int CandidaterNo);
extern int  GetSquadNo();
extern void SendCMD_CHECK_ABILITY_CHANGE_MONEY();						//011201 LTS		// LTS MENU
extern void SendCMD_LOCALWAR_MAPMOVE();									// LTS NEW LOCALWAR
extern void CloseLocalWarInfo();										// LTS NEW LOCALWAR
extern void LocalWarMapMoveClick(int MapNo);							// LTS NEW LOCALWAR
extern int GetLocalWarMapMoveIndex();									// LTS NEW LOCALWAR	
extern void DrawLocalWarDefeat(int ImageType,int MenuNumber,int BaseX,int BaseY);	// LTS NEW LOCALWAR
extern void SendCMD_LOCALWAR_JOIN();									// LTS LOCALWAR
extern void SendCMD_EVENT_LOCALWAR_JOIN();								// 020115 LTS
extern void SendCMD_EVENT_LOCALWAR_MOVE();								// 020115 LTS
extern void DrawLocalWarJoinInterface(int ImageType,int MenuNumber,int BaseX,int BaseY);
//_____________________________________________________________________	// 010904


char g_szSearchLadderRankName[20];
bool bSmallTips = false;
int g_RemainTimeType; 
int g_RemainTimeDelay;

#define MAX_SALVAITON_MONEY 3000000000

int bbs_money[2][2] = { {100, 1000}, {1500, 15000} };
int g_BattleStatusView;

int g_RecvLoanInfo;
int	g_CurrUsedMagic;		// ÇöÀç Ä³½ºÆÃ µÇ°í ÀÖ´Â ¸¶¹ý
int refresh_magery_time = 100;

int TotalSalvation;

char man_list[MAX_MAP_][MAN_LIST_MAX_][20];		// »óÁ¡ ÁÖÀÎ ÀÌ¸§µé
int AutoLevelUpOn;
DWORD AutoLevelUpCount;

t_QuickMemory SaveMemory[MAX_MAGIC_MEMORY];		// ´ÜÃàÅ°·Î ±â¾ï½ÃÄÑµÐ ¸ñ·Ï//020530 lsw
t_QuickMemory HotKeyMemory;						// ¹öÆ° ÇÑ°³·Î »ç¿ë ÇÏ´Â Ram °°Àº º¯¼ö

/*//020515 lsw
bool bDisplaySkillExp;
bool bDisplayTacticExp;
*/
int before_skill_no;
//int g_DisplaySkillExp;
int g_VoteSignExist;	// ¼±°Å ÇöÈ² º¸¿©ÁÖ´Â°¡ ¸¶´Â°¡..

int g_CanRegistCandidate;		// ÈÄº¸ µî·ÏÀ» ÇÒ¼ö ÀÖ´Â »óÅÂ°¡ ¾Æ´Ï¸é -1, ÅõÇ¥¸¦ ÇÒ¼ö ÀÖ´Â »óÈ²ÀÌ ¾Æ´Ï¸é -2
int	g_Candidate_max;
int g_CurrVote;	// ÇöÀç ¼±°Å ÁøÇà »óÅÂ 0:ÃÊ±â »óÅÂ( µ¥ÀÌÅ¸¸¦ ¹Þ¾Æ ¿Â´Ù), -1:ÀÔÈÄº¸ ÇÒ¼ö ¾ø´Ù. 1:ÀÔÈÄº¸ »óÅÂ 2:ÅõÇ¥ »óÅÂ
k_client_candidate		Candidate[3];	// ÀÔÈÄº¸ Á¤º¸

Spr scenario_spr;		// ½Ã³ª¸®¿À

int	fight_map_live_man[4];		// »ì¾Æ ÀÖ´Â »ç¶÷
char nation_bbs[2000];
char nation_bbs2[8][8][50];
int man_to_man_start=0;
int fight_map_stop=0;
int g_fight_win;
int g_fight_lose;

int fight_who;		// 1:1 ´ë°áÇÏ´Â »ç¶÷ÀÇ ¾ÆÀÌµð
int	danger_by_hunter;

int skill_index[5] = {0,};
CServerText		public_bbs;			// ¿î¿µÀÚ°¡ Å¬¶óÀÌ¾ðÆ®·Î ¹Ù·Î °øÁö»çÇ×À» º¸³¾¶§, 

bool	g_MagicMenuOn;
bool	g_ExplainMenuOn;

int		magic_plus[MAX_MAGIC];		// ³»°¡ »ç¿ëÇÑ ¸ÅÁ÷ ´©Àû
int		magic_plus_count;			// ÃÑ °¹¼ö
int		magic_plus_start;			// ÂïÀ» ÀÌ¹ÌÁö startºÎºÐ

bool	g_explain_menu_on = false;
char	g_explain_menu[MAX_STRING];

bool	g_explain_magic_on = false;
int		g_explain_magic;

bool	g_explain_skill_on = false;
int		g_explain_skill;

//bool	g_ExplainMessageOn;		// ¼³¸íÃ¢À» »ç¿ëÇß´Â°¡..
DWORD g_alarm ;

CServerText g_ServerText;		// ¼­¹ö¿¡¼­ Àü¼Û¹Þ´Â ÅØ½ºÆ®¸¦ À§ÇÑ ±¸Á¶Ã¼

bool g_bTimeMenuOn;
bool bCallGodMeetingMenu;			// ¿¹¹è ¸Þ´º ¶ç¿ì±â

Spr spr2[MAX_IMAGE2];		// Ãß°¡ ¸Þ´º ÀÌ¹ÌÁö 

bool g_bMagicSelect = true;  //	¸¶¹ý °³³äÀÌ ¸Þ¸ð¶óÀÌÁî °³³äÀÎÁö ¾Æ´ÑÁö ÆÇ´Ü...  true -> ¸Þ¸ð¶óÀÌÁî...
bool bSoundOn = false;

bool g_DropOnHero = false;	// »ç¶÷¿¡°Ô ¾ÆÀÌÅÛÀ» ¶³¾î¶ß·ÈÀ» °æ¿ì true set
int g_MouseInMenu = 0;	// 031110 YGI

int memorize[12] = {0,};			// ¸Þ¸ð¶óÀÌÁî ºÏ¿¡¼­ »ç¿ë...
int SkillOn=-1;				// ½ºÅ³ÀÌ ³¡³ª°Ô µÇ¸é -1·Î ¼ÂÆÃÇØ ÁØ´Ù.

PosItem		skill_use_item[5][10];
PosItem		skill_use_item_result;

int con_to_img[7] = { 0, 263, 263, 265, 265, 265, 264 };		// Ä³¸¯ÅÍ »óÅÂ ±×¸²À» ³Ö´Â´Ù.

K_ITEM	i_DivideItem;		// ¾ÆÀÌÅÛÀÇ ¼Ó¼º¿Í À§Ä¡°¡ ÀÖ´Ù.
ItemJoinSelect learn_item_list[MAX_LEARN_ITEM] = {0,};

OtherCharacter	You;

int Hero_Ability[20];

MakeItem make_item_list[20];
MakeItem make_item_result;
int		selected_skill;		// ¼±ÅÃµÇ¾îÁø ½ºÅ³

CharacterParty mouse_ch;		// ¸¶¿ì½º°¡ µé°í ÀÖ´Â »ç¶÷...

int		storelistindex; // »óÁ¡¿¡¼­ »ç¿ëÇÒ ÀÓ½Ã º¯¼ö
bool	is_buymenu_On;	// buy ¸Þ´º°¡ ¶ã°æ¿ì ¼ÂÆÃÇØ ÁØ´Ù.

int		nSkillLearnResult;	// ½ºÅ³ ¹è¿ì±âÀÇ °á°ú 1:´É·ÂÀÌ ¸ðÀÚ¶÷, 2:µ·ÀÌ ¸ðÀÚ¶÷, 3: Àß ÀÍÇûÀ½
int		motherSkillImageNo[2];  // ½ºÅ³ ¹è¿ì±â¿¡¼­ Á¦¸ñÆÇ ÀÌ¹ÌÁö ¹øÈ£
int		motherSkillImageType[2];  // ½ºÅ³ ¹è¿ì±â¿¡¼­ Á¦¸ñÆÇ ÀÌ¹ÌÁö Å¸ÀÔ

BOOL	RButtonDown;
BOOL	MouseDrag;
int			LButtonCount;		// ¸¶¿ì½º ¹öÆ°À» ´©¸£°í ÀÖ¾ú´ø ½Ã°£

int			y_PutBarSize=0;              //HP³ª MP¸¦ Ãâ·ÂÇÒ ¸·´ë ±×·¡ÇÁÀÇ ³ôÀÌ¸¦ °è»êÇÑ °ªÀ» ÀúÀåÇÑ´Ù.
bool		id_password = true;  // ¾ÆÀÌµð ÆÐ½º¿öµå Ä­ ¿Ô´Ù °¬´ÙÇÏ±â...

bool		MessageOn=false;			// MessageOnÀÌ Æ®·ç·Î Ã¼Å© µÇ¸é ¸ÞÀÎ ÀÎÅÍÆäÀÌ½º ¼³¸íÃ¢¿¡ ¸Þ½ÃÁö°¡ ¶á´Ù
char		Message[MAX_STRING];

int			g_MouseItemNumber=0;           //Å¸ÀÔ¿¡ µû¶ó °¡Áö°í ÀÖ´Â ¾ÆÀÌÅÛÀÇ ¹øÈ£  0ÀÌ¸é ¾ÆÀÌÅÛÀÌ ¾ø´Ù´Â ¶æ
int			g_MouseItemType=0;            //0ÀÌ¸é ¸¶¿ì½º°¡ ¾î¶² ¾ÆÀÌÅÛµµ °¡Áö°í ÀÖÁö ¾Ê´Ù´Â ¶æ(¸¶¹ý,¾ÆÀÌÅÛ) 1=ÀÏ¹Ý ¾ÆÀÌÅÛ,2=¸¶¹ý ¹®¼­
RECT		rScreenRect;                  //¸Þ´º¸¦ Á¦¿ÜÇÑ ½ÇÁ¦ °ÔÀÓÈ­¸éÀÇ ÁÂÇ¥ ¸¶¿ì½º ¿ÞÂÊ¹öÆ°À» ³õÀ»¶§ ¾ÆÀÌÅÛÀÌ ¶³¾îÁö´Â °ÍÀ» Ã¼Å©ÇÑ´Ù
int			YouGotItemFlag=false;
int			g_BoadAndSignNumber; // CSD-030324 // thai2 YGI

int			MouseItemInRectOn;			// ¾Æ¸Ó Ã¢ ±ÙÃ³·Î ¾ÆÀÌÅÛÀÌ ´Ù°¡¿ÔÀ» ¶§ Ã³¸®
POINT		MouseItemInRect;

int			cRMoveMenu=0;                 //¸ÞÀÎ ÀÎÅÍÆäÀÌ½º¿¡¼­ ¿À¸¥ÂÊ¿¡ µîÀåÇÏ´Â ¸Þ´º(¾ÆÀÌÅÛ,¸¶¹ý)°¡ ¿òÁ÷ÀÏ¶§ »ç¿ëÇÏ´Â Ä«¿îÆ®
int			cLMoveMenu=0;                 //¸ÞÀÎ ÀÎÅÍÆäÀÌ½º¿¡¼­ ¿ÞÂÊ¿¡ µîÀåÇÏ´Â ¸Þ´º(½ºÆ¼ÀÌÅÍ½º,¾îºô¸®Æ¼,°¡Ä¡°ü)°¡ ¿òÁúÀÏ¶§ »ç¿ëÇÏ´Â Ä«¿îÆ®
int			fRMoveMenu=0;                 //¸Þ´ºÀÇ Á¾·ù,¿©´èÀÌ »óÈ²¿¡ µû¶ó ±× »óÅÂ¸¦ ÀúÀåÇÏ´Â º¯¼ö 0ÀÏ¶© ¾Æ¹«ÀÏµµ ¾ø´Ù 
int			fLMoveMenu=0;					// ¿ÞÂÊ ¸Þ´º ´Ý±â 

int			nItemOther;						// ¾ÆÀÌÅÛ ¸Þ´º°¡ ¿­¸±¶§ °°ÀÌ ¶°¾ß ÇÏ´Â ¸Þ´º
int			nPartyOther=0;					// ÆÄÆ¼ ¸Þ´º°¡ ´ÝÈ÷¸é ¶°¾ßÇÒ ¸Þ´º 
int			nPartyState = 0;				// ÆÄÆ¼ ¸Þ´º ¶ß±â Àü¿¡ ¹Ì¸® ¶° ÀÖ¾ú´ø ¸Þ´º
int			nPartyShow=1;					// ¸ÞÀÎ ÀÎÅÍÆäÀÌ½º ÆÄÆ¼ ¸Þ´º¿¡¼­ ¼±ÅÃ ¶óÀÌÆ® Ç¥½ÃÇÏ±â º¯¼ö

struct		InterfaceFlag main_interface;
SystemInfo	system_info;

CHouse		*house;				// Áý ¼³¸í -> »èÁ¦ ´ë»ó °í·Á
CMenuStack	MenuStack;
int			chat_scroll_count;

POS			IP_base;					//  ¾ÆÀÌÅÛÀÌ ¿ø·¡ ÀÖ¾ú´ø ÀÚ¸®

ItemAttr	InvItemAttr[3][3][8];
ItemAttr	EquipItemAttr[ITEM_EQUIP_MAX];//021119
ItemAttr	QuickItemAttr[ITEM_QUICK_MAX+1];
ItemAttr	BankItemAttr[5][3][6];
ItemAttr	HandItemAttr;
ItemAttr	SupperItemAttr[3][15];

ItemAttr	InvAttrBackUp[3][3][8];
ItemAttr	EquipAttrBackUp[8];
ItemAttr	QuickAttrBackUp[7];
ItemAttr	HandAttrBackUp;
ItemAttr	aItemInventoryFull;		// ÀÎº¥Åä¸® Ã¢¿¡ ²Ë Ã¡À» ¶§

bool		LeftShiftOn;		// ¿ÞÂÊ ½ÃÇÁÆ® Å°°¡ ´­·Á Á³´ÂÁö true : ´­¸², 
// int		AbilityToInt[20];

int			RectImageOn=0;		//¸ÅÁ÷ È°¼ºÈ­°¡ µÆÀ»¶§, ±ô¹ÚÀÓ
int			nBankTemp=0;					//ÀºÇà ¸Þ´º¿¡¼­ µ· °è»êÇÏ±â À§ÇÑ º¯¼ö

int			WearAble[9] = { 2, 3, 4, 5, 6, 7, 8, 8, 9 };		// ÀåÂø À§Ä¡ º¯È¯ º¯¼ö 
int			HandBackUp;
int			HandBackUptype;
int			HandItemOn = false;

bool		no_active_box_yet;				// ¼­¹ö¿¡¼­ °ªÀ» Àü¼Û ¹ÞÀ¸¸é false set.

int			aMagicItem[3][4][3][5];

int			SkillInventory[2][2][4];	// ½ºÅ³ ÀÎº¥Åä¸®...
int			SkillInventory2[2][2][4];	// ¹è¿ìÁö ¾ÊÀº ½ºÅ³
int			Skill_Know_Inv[3][3];

ItemJoinSelect Item_Select[MAX_LEARN_SKILL_PAGE][10];
ItemJoinSelect item_select_result;

int aSellBuyItemPos[MAX_BUY_ITEM*3*5];			// inventroy ¾î´À°÷¿¡ ÀÖ´ø ¹°°ÇÀÎÁö...
ItemAttr aSellBuyItemAttr[MAX_BUY_ITEM*3*5];

ItemAttr itemMallAttr[MAX_BUY_ITEM*3*5]; // Mall item inventory
int itemMallMax; // The amount of items in the mall

int aSellBuyItemMax;
const int y_ItemSubMenu[] = //021014 lsw
{
	MN_ITEM_MAKE,
	MN_ITEM_MAKE_OK, 
	MN_ITEM_MAKE_RARE_SELECT,
	MN_GM_2ND_SKILL,
	MN_SKILL_MASTER_MAKE,//021111 lsw
	MN_DIVIDE_ITEM,
	MN_CHARACTER_ITEM,
	MN_GAMBLE,
	MN_MERCHANT_REGISTER,
	MN_EXCHANGE_BOND_MONEY,
};

const int mainmenu[]=
{
	MN_MAININTERFACE,
	MN_MAIN_EXPLAIN,

	MN_QUICK_MAGIC_PLUS,
	MN_QUICK_MAGIC_SCROLL,
	MN_TIME_DISPLAY_ON_OFF,
	MN_QUICK_SKILL,

	MN_TIME_DISPLAY_MENU,
	MN_COMBAT_SKILL_DOING,//020420 lsw
	MN_COMBAT_SKILL_POINT,//020707 lsw

	MN_EXCHANGE_ITEM,
	MN_MAN_TO_MAN,		// °áÅõ
	MN_SMALL_MAP,
	MN_DIVIDE_ITEM,
	MN_ITEM,
	MN_PARTY,

	MN_STATUS,
	MN_ABILITY,
	MN_ARIGEMENT,
	MN_TACTICS,

	MN_GUILD_ITEM_GET, 
	MN_GUILD_ITEM_PUT,

	MN_CHATTING,

	MN_FACE,
	MN_WISPER_LIST,

	MN_ARENA_LOBBY, 
	MN_OBSERVER_MODE,
	MN_BETTING,

	MN_SYSTEM,
	MN_SYSTEM_OPTION,
	MN_BANK_CUSTODY,

	MN_ITEM_MAKE,
	MN_ITEM_MAKE_SELECT,
	MN_ITEM_MAKE_OK,
	MN_ITEM_MAKE_RARE_SELECT,
	MN_COMBAT_SKILL_DOING,
	MN_COMBAT_SKILL_POINT,
	MN_GM_2ND_SKILL,
	MN_SKILL_MASTER_MAKE,//021111 lsw

	MN_MERCHANT_REGISTER,
	MN_EXCHANGE_BOND_MONEY,

	MN_CHARACTER_ITEM,
	MN_ITEM_SELECT,
	MN_VOTE_SIGN,

	MN_MESSAGEBOX_BIG,

	MN_PARTY_FACE1,
	MN_PARTY_FACE2,
	MN_PARTY_FACE3,
	MN_PARTY_FACE4,
	MN_PARTY_FACE5,
	MN_PARTY_FACE6,

 	MN_SMALL_TIPS,
	
	MN_DISPLAY_SKILL_EXP,
	MN_DISPLAY_TAC_EXP, 
	MN_DISPLAY_MAGIC_EXP, 

	MN_GAMBLE,

	MN_FIGHT_MAP_SIGN, 
	MN_DISTANCE_TABLE,
	MN_STORE_SIGN,

	MN_OK_MESSAGEBOX,
	MN_OKCANCEL_MESSAGEBOX,
	MN_ITEM_MESSAGEBOX,

	MN_DEL_CHAR_COMFORM,

	MN_SYSTEM_LOGOUT,
	MN_EFFECT_TOOL, 

	NM_SCRIPT_TIMMER,	// 021021 kyo
	NM_SCRIPT_COUNTER,	// 021022 kyo
				
	
	MN_VIEW_TIP,
	MN_AWAKE,
	MN_REVIVAL,
	MN_LEVELUP,// ¸Ç ¸¶Áö¸· À§Ä¡ÇØ¾ßÇÔ
};

const int smallmenu[] = 
{
	MN_MAININTERFACE,
	MN_CHATTING,

	MN_SKILLGUILD_BUY			,
	MN_SKILLGUILD_SELL			,
	MN_SKILLGUILD_SKILL			,
	MN_SKILLGUILD_ITEM			,
	MN_SKILLGUILD_REPAIR		,
	MN_SKILLGUILD_KNOWLAGE		,

	MN_BANK_DEPOSIT,
	MN_BANK_DEFRAYAL,
	MN_BANK_CUSTODY,
	MN_BANK_MAIN,
	MN_BANK_LOAN,
	MN_BANK_REPAYMENT,
	MN_BANK_AUCTION,
	MN_BANK_AUCTION_SALE,

	MN_BLACKSMITH,			
	MN_FARM	,	
	MN_RESTAURANT,
	MN_HERBSHOP,	
	MN_SHAMBLES	,
	MN_CANDLE	,
	MN_SLOPSHOP	,
	MN_ALCHEMY	,
	MN_WOODWORKING,
	MN_BOWCRAFT	,
	MN_SHIPYARD	,
	MN_MAGIC_GUILD, 
	MN_ACADEMY,
	
	MN_VISA_MENU, 

	MN_LADDER_RANK,

	MN_FAME,
	MN_QUEST,
	MN_QUEST_INDEX,
	MN_CHARACTER_COLOR_CHANGE,

	MN_GOD_EDELBLHOY,
	MN_GOD_TEFFERY,
	MN_GOD_LETTY,
	MN_GOD_OREM,

	MN_GOD_INTRUDUCTION,
	MN_GOD_MEETING,
	MN_GOD_REGIST1,
	MN_GOD_PREACH,
	MN_GOD_REGIST_ERROR,
	MN_GOD_REGIST_MESSAGE,
	MN_GOD_PREACH_RESULT,
	MN_GOD_PREACH_ERROR1,
	MN_GOD_MEETING_MAIN,
	MN_GOD_SUPPER, 

	MN_GM_2ND_SKILL,
	MN_SKILL_MASTER_MAKE,//021111 lsw
	MN_SKIN,

	MN_NATION_BATTLE_BBS,	
	MN_NATION_INFO	,	
	MN_NATION_VOTE	,	
	MN_NATION_BUDAE	,	
	MN_NATION_NOW	,	
	MN_NATION_LADDER,	
	MN_NATION_JOIN,	
	MN_NATION_GUEST,
	MN_NATION_GOTO_WAR_MAP,	
	MN_NATION_DONEBOX,		
	MN_NATION_MSGBOX,		
	MN_NATION_TODAY_NOTICE,
	MN_NATION_WAR_RESULT,
	MN_NATION_WAR_START,
	
	MN_GHOST_TO_ALIVE,

	MN_TUTORIAL_DISPLAY,
	MN_EXCHANGE_BOND_MONEY,//021126 LSW
	MN_ITEM_MAKE_RARE_SELECT,

	MN_TOWNHALL ,
    MN_TOWNHALL_PUBLICNOTICE ,
	MN_TOWNHALL_BUILDING,
	MN_TOWNHALL_COMMERCE1,
	MN_TOWNHALL_COMMERCE2,
	MN_TOWNHALL_COMMERCE3,
	MN_TOWNHALL_HOUSEREPAIR,
	MN_TOWNHALL_GUILD,
	MN_TOWNHALL_FORUM,
	MN_TOWNHALL_EVENT,
	MN_TOWNHALL_RANK1,
	MN_TOWNHALL_RANK2,
	MN_TOWNHALL_NOTICE4,
	MN_TOWNHALL_NOTICE3,
	MN_TOWNHALL_NOTICE2,
	MN_TOWNHALL_NOTICE1,
	MN_TOWNHALL_COMMERCE4,
	MN_TOWNHALL_CONSTRUCTION,
	MN_TOWNHALL_BBS1,
	MN_TOWNHALL_BBS2,
	MN_TOWNHALL_BBS3,

	MN_NATION_MAIN_VA, 
	MN_NATION_MAIN_ZY,
	MN_NATION_MAIN_ILL,
	
	MN_GUILD_MAIN,
	MN_GUILD_EXPLAIN,
	MN_GUILD_REGIST_IMSI,
	MN_GUILD_REGIST_DEGREE,
	MN_GUILD_REGIST,
	MN_GUILD_INTRODUCTION,
	MN_GUILD_CHANGE_INFO,
	MN_DUAL_MAIN,
	MN_INIT_ABILITY,
	MN_RUNE,
	MN_CHANGE_MONEY,

	MN_NATION_ITEM,
	MN_GUILD_HOUSE,
	MN_GUILD_HOUSE_INFO,

 	MN_GUILD_MEMBER_LIST,

	MN_MESSAGEBOX_BIG,
	MN_HTML_MESSAGEBOX,

	MN_VOTE_REGIST,
	MN_VOTE_SUPPORT,

	MN_PUBLIC_BBS, 
	MN_NATION_BBS, 

	MN_GOD1,
	MN_GOD2,
	MN_GOD3,

	MN_FRIEND_MENU,
	MN_FRIEND_REGIST,
	
	MN_POTAL_MENU,
	MN_POTAL_SELECT,

	MN_MAGICBOOK,
	MN_FUNCTION,
	
	MN_GUILD_ITEM_OR_MEMBER_LIST,

	MN_DRINK_MAIN,
	MN_DRINK_RUMOR,	
	MN_DRINK_RUMOR_VIEW,
	MN_DRINK_BUY,	

	MN_SHOP_MAIN,
	MN_SHOP_SELL,
	MN_SHOP_BUY,	

	MN_HEAL_MAIN,
	MN_HEAL_HEALING,
	MN_HEAL_PREVENTING,
	MN_HEAL_LEARN,

	MN_MERCHANT_MAIN,
	MN_MERCHANT_BBS,
	//021014 lsw-->
	MN_MERCHANT_FIND,//¹°Ç° °Ë»ö
	MN_MERCHANT_BUY,//°Ë»ö °á°ú
	MN_MERCHANT_SELL_INFO,//ÆÇ¸ÅÀÚ¸¸ ¾²´Â ¾ÆÀÌÅÛ Á¤º¸
	MN_MERCHANT_REGISTER,
	MN_MERCHANT_RESULT_TAKE,
	MN_MERCHANT_DIRECT_EXCHAGE_LIST,
//021014 lsw<--
	MN_INN_MAIN,
	MN_INN_REST,
	MN_INN_SLEEP,
	
	MN_SALVATION,
	MN_SALVATION2,

	MN_SCALE,

	MN_GUILD_ADMIN,
	MN_GM_LIST,
	MN_GM_ITEM_EXPLAIN,
	NM_MERCHANT_QUEST,//021030 lsw
	MN_SKILL_MASTER_LIST,

	MN_LEARN_MAGIC_BOOK,
	MN_MEM_BOOK,

	MN_HUNTER_MAIN,
	MN_HUNTER_REGIST,
	MN_HUNTER_LIST,

	MN_MAIL_WRITE,
	MN_MAIL_MENU,
	MN_MAIL_READ,

	MN_ABILITY_REDISTRIBUTION,		
 	MN_LOCALWAR_JOIN,				
	MN_LOCALWAR_OK,					
	MN_EVENT_LOCALWAR_OK,			
	MN_EVENT_LOCALWAR_MOVE,			

	MN_PARTY_FACE1,
	MN_PARTY_FACE2,
	MN_PARTY_FACE3,
	MN_PARTY_FACE4,
	MN_PARTY_FACE5,
	MN_PARTY_FACE6,

	MN_DISPLAY_SKILL_EXP,
	MN_DISPLAY_TAC_EXP, 
	MN_DISPLAY_MAGIC_EXP, 
	MN_EFFECT_TOOL, 
	
	MN_WAR_START,
	MN_WAR_MOVE,
	MN_WAR_STATUS,

	MN_SCENARIO,
	MN_DEFAULT_MESSAGEBOX,
	MN_DEFAULT_MESSAGEBOX2,
	
	MN_SCENARIO_TRAP_INFO,
	MN_SCENARIO_TRAP_KEY,		

	MN_LOTTO_MENU,
	MN_LUCKY_MENU,
	MN_WINNER_MENU,

	MN_OK_MESSAGEBOX,
	MN_OKCANCEL_MESSAGEBOX,
	
	MN_DEL_CHAR_COMFORM,

	MN_SYSTEM_LOGOUT,
	MN_ITEM_MESSAGEBOX,

	MN_SMALL_TIPS, 
	MN_VIEW_TIP,
	MN_AWAKE,
	MN_REVIVAL, 
	MN_LEVELUP,		// ¸Ç ¸¶Áö¸· À§Ä¡ÇØ¾ßÇÔ
	MN_MERCHANT_BACKUP_MAIN,	// BBD 040303
	MN_MANTLE_COLOR_CHANGE,
	MN_ITEM_MALL,
};

const int aNoCloseMenu[] = 
{
	MN_MAININTERFACE,
	MN_SMALL_MAP,
	MN_CHATTING,
	MN_FACE,
	MN_QUICK_MAGIC_PLUS, 
	MN_QUICK_MAGIC_SCROLL,
	MN_TIME_DISPLAY_ON_OFF,
	MN_STATUS,
	MN_ABILITY,
	MN_ARIGEMENT,
	MN_TACTICS,
	MN_SYSTEM,
	MN_SYSTEM_OPTION,
	MN_FIGHT_MAP_SIGN,
	MN_PARTY_FACE1,
	MN_PARTY_FACE2,
	MN_PARTY_FACE3,
	MN_PARTY_FACE4,
	MN_PARTY_FACE5,
	MN_PARTY_FACE6,
	MN_STORE_SIGN,
	MN_DISTANCE_TABLE,
	MN_NATION_WAR_RESULT,
	MN_MAIL_MENU,
	MN_MAIL_WRITE,
	MN_MAIL_READ,
	MN_GHOST_TO_ALIVE,
	MN_REVIVAL, 
	MN_OK_MESSAGEBOX,
	MN_OKCANCEL_MESSAGEBOX,
	MN_SYSTEM_LOGOUT,
	MN_LEVELUP,		// ¸Ç ¸¶Áö¸· À§Ä¡ÇØ¾ßÇÔ
};

const int aNoCloseOfMainMenu[] = 
{
	MN_MAININTERFACE,
	MN_MAIN_EXPLAIN,

	MN_EXCHANGE_ITEM,
	MN_SMALL_MAP,
	MN_CHATTING,

	MN_PARTY_FACE1,
	MN_PARTY_FACE2,
	MN_PARTY_FACE3,
	MN_PARTY_FACE4,
	MN_PARTY_FACE5,
	MN_PARTY_FACE6,

	MN_COMBAT_SKILL_DOING,
	MN_MAIL_MENU,
	MN_MAIL_WRITE,
	MN_MAIL_READ,
	MN_SYSTEM_LOGOUT,

	MN_DISTANCE_TABLE,
	MN_STORE_SIGN,
	MN_REVIVAL,
	MN_LEVELUP,		// ¸Ç ¸¶Áö¸· À§Ä¡ÇØ¾ßÇÔ
};

bool is_mainmenu_active = true;
const int MAX_NO_CLOSE_MENU = sizeof( aNoCloseMenu ) / sizeof( int );
const int MAX_SMALL		= sizeof(smallmenu) / sizeof(int);
const int MAX_MAIN		= sizeof(mainmenu) / sizeof(int);
const int max_submenu	= sizeof(y_ItemSubMenu) / sizeof(int);
const int MAX_NO_CLOSE_MAIN_MENU = sizeof( aNoCloseOfMainMenu ) / sizeof( int );

int xTemp=0;
int yTemp=0;


//soto-030430
bool	DoButtonCheckOfMenu_SWD(int nMenuIndex,int nFieldIndex)
{
	bool	bFlag = false;
	
	switch(SMenu[nMenuIndex].nField[nFieldIndex].nSpecialWillDo)
	{
		case SWD_LOTTO_OPEN :
			{

				if(LottoMgr())
				{
					LottoMgr()->SendOpenLottoMenu();
				}
				bFlag = true;
			}
			break;
		case SWD_LUCKY_EVENT :
			{
				bFlag = true;
				AddCurrentStatusMessage(0,255,255,lan->OutputMessage(4,99));
			}
			break;
		case SWD_COMFIRMATION_WINNER :
			{
				if(LottoMgr())LottoMgr()->SendOpenWinnerMenu();
				bFlag = true;
			}
			break;
		case SWD_LOTTO_NUMBER :
			{
				if(LottoMgr())
				{
					int nRet = 0;
					if(nRet = LottoMgr()->InsertNumber(nFieldIndex + 1))
					{
						//JustMsg(lan->OutputMessage(4,91));
						if(nRet == 1)
							AddCurrentStatusMessage(255,0,0,lan->OutputMessage(4,91));
						else
							AddCurrentStatusMessage(255,0,0,lan->OutputMessage(4,92));						
					}
				}
				bFlag = true;
			}
			break;
		case SWD_LOTTO_AUTO_GEN :
			{
				bFlag = true;
				if(LottoMgr())
				{
					LottoMgr()->Generate6Number();
				}
			}
			break;
		case SWD_LOTTO_OK :
			{
				bFlag = true;
				//¿©±â¼­ µî·ÏÀ» ÇÑ´Ù.
				if(LottoMgr())
				{
					if(LottoMgr()->SendBuyLotto())
					{
						LottoMgr()->Clear();
					}
				}
			}
			break;

		//¾ÆÀÌÅÛÀ» Áö±Þ ¹ÞÀÚ.
		case SWD_WINNER_OK:
			{	
				if(LottoMgr())
				{
					LottoMgr()->SendCheckWinner();
				}
				bFlag = true;
				CloseAllMenu();
			}
			break;
		//<soto-LottoÃß°¡È®Àå.
		case SWD_LOTTO_NUM_LEFTSEEK:  //ÀÌÀü ÆäÀÌÁö¸¦ ´Ù¿À.
			{
				LottoMgr()->SendLottoViewSeek(false);
				bFlag = true;
			}
			break;
		case SWD_LOTTO_NUM_RIGHTSEEK: //´ÙÀ½ ÆäÀÌÁö¸¦ ´Ù¿À.
			{
				LottoMgr()->SendLottoViewSeek();
				bFlag = true;
			}
			break;
		//>soto-LottoÃß°¡È®Àå.
	}

	return bFlag;
}
//<soto-030430


int ViewCheckRoutine2( int type, int t )
{	
	return 1 ;

#ifdef _DEBUG
	switch( type )
	{
	case 1 :  return 1;
	case 2: break;
	default : return 1;
	}

	static int TimeCheckCount;
	static int TickCount, OldTickCount, TickFlag;
	static int currentfps;
	
	if( t == 0 ) TickFlag = 0;
	if( TickFlag == 0 )	{	OldTickCount = TickCount = ::timeGetTime();		TickFlag = 1; 	}
	else
	{
		OldTickCount= TickCount;
		TickCount	= ::timeGetTime();
	}
	char temp[ FILENAME_MAX];
	sprintf( temp, "Menu %d : %d \n", t, TickCount - OldTickCount );
	OutputDebugString( temp );
	return OldTickCount - TickCount;

#else

	return 0;

#endif
}					


//HWND hwnd;
/*********************************ÇÔ¼ö ¼Ò½º*******************************************/
///////////////////////////////////////////////////////////////////////////////////////
void MenuDisplay()
{
//	hwnd = GetFocus();
	SetHangulAlign( TA_LEFT );
	SetHeroPosition(0); // È­¸é °»½Å
	ShowExplain();							// °¢Á¾ ¼³¸í¹® º¸¿©ÁÖ±â 
	MoveMenu();

	static int flag=false;
	static int flag2=true;

	int i; 

	LoofSetHeroAbility( Hero_Ability );	// Ä³¸¯ÅÍ ´É·ÂÄ¡ ¾÷ µ¥ÀÌÆ®( 4°¡Áö¸¦ ´Ù ÇÕÄ£´Ù. ) 
	//ViewSmallMapCheck();
	CheckPartyFace( );			// ÆÄÆ¼¿ø Ã¼Å©

	ViewCheckRoutine2( 1, 0 );
	if( is_mainmenu_active )
	{
		int x;
		for( x=0; x<MAX_MAIN; x++ ) 
			if( SMenu[mainmenu[x]].bActive ) QuickKey( mainmenu[x] );		// ´ÜÃàÅ°

		for(x=0;x<MAX_MAIN;x++)
		{			
			i=mainmenu[x];
			if(SMenu[i].bActive)   //i¹øÂ° ¸Þ´º°¡ È°¼ºÈ­ µÇÀÖ³Ä?
			{
				if( CheckMouseInMenu( i ) ) g_MouseInMenu = i;	// È°¼ºÈ­µÈ ¸Þ´º ¾ÈÀ¸·Î ~
				
				UpperMenuNomalPut(SMenu[i].x, SMenu[i].y, SMenu[i].nImageNumber, SMenu[i].nImageType );
				MenuSubProcessType( &SMenu[i] );
				if(LottoMgr())LottoMgr()->LottoMenuTextDisplay(i);//soto-030501
				ViewCheckRoutine2(1, i);
			}
		}
	}
	else 
	{
		int x;
		for( x=0; x<MAX_SMALL; x++ ) 
			if( SMenu[smallmenu[x]].bActive ) QuickKey( smallmenu[x] );		// ´ÜÃàÅ°

		for(x=0;x<MAX_SMALL;x++)
		{
			
			i=smallmenu[x];
			if(SMenu[i].bActive)   //i¹øÂ° ¸Þ´º°¡ È°¼ºÈ­ µÇÀÖ³Ä?
			{	
				if( CheckMouseInMenu( i ) ) 
				{
					g_MouseInMenu = i;
				}
	//=========================================
#ifdef _DEBUG // finito 060507
				if( GetSysInfo( SI_GAME_MAKE_MODE ) && i != MN_MAININTERFACE )
				{
					switch( SpaceBarPress() )
					{
						case 'r' :  flag = !flag; break;
						case 'p' :	if( flag ) 
									{
										if( flag2 ) SMenu[i].x++; 
										else SMenu[i].y++; 
									}
									break;
						case 'm' :	if( flag ) 
									{
										if( flag2 ) SMenu[i].x--; 
										else SMenu[i].y--; 
										xTemp = SMenu[i].x;
										yTemp = SMenu[i].y;
									}
									break;
						case 'o' : flag2 = !flag2; break;
					}
				}
#endif
	//=========================================
				UpperMenuNomalPut(SMenu[i].x, SMenu[i].y, SMenu[i].nImageNumber, SMenu[i].nImageType );				
				MenuSubProcessType( &SMenu[i] );		// ¸ÞÀÎ ÀÎÅÍÆäÀÌ½º´Â Âï±â¸¸ ÇÑ´Ù.
				if(LottoMgr())LottoMgr()->LottoMenuTextDisplay(i);//soto-030501
				ViewCheckRoutine2(1, i);
			}
		}
	}
	MessagePut();
	PutMouseItem();  //¸¶¿ì½º°¡ ¾ÆÀÌÅÛÀ» ¾òÀ¸¸é ±×°É Âï´Â´Ù

	PrintTextOnMenu();
	PrintTextOnMenu2();
	return;
}

//###########################################################################################//
//¸¶¿ì½º»óÈ²¿¡ µû¸¥ ¸Þ´ºÀÇ Ã³¸®
void MenuChecking()
{
	int i,j,x;
	bool f_ReturnOn=false;
 	g_MouseInMenu = 0;

	if (Hero->viewtype == VIEWTYPE_OBSERVE_)
	{	//< CSD-030515
		return;
	}	//> CSD-030515

	kein_KeyProc();

	if( !GetSysInfo( SI_GAME_MAKE_MODE ) )
	{	
		if (IsHeroDead())
		{	
			MenucheckWhenHeroDie();
			return;
		}
	}

	if( !is_mainmenu_active )	// ¸ÞÀÎ ¸Þ´º¿Í ½º¸ô ¸Þ´º°¡ °°ÀÌ ¶ã¶§¿Í ¿¬°è°¡ µÇ¾î ÀÖÀ» °æ¿ì Àß Ã¼Å© , ¹ðÅ©-¾ÆÀÌÅÛ,  ·±½ºÅ³-½ºÅ³ 
	{
		for( x=0; x<MAX_MAIN;x++ )
		{
			i=mainmenu[x];
			if( !SMenu[i].bActive ) SMenu[i].CheakType = 0;
			for(j=0;j<SMenu[i].nFieldCount;j++)
			{
				SMenu[i].nField[j].fRectMouse=false;
				SMenu[i].nField[j].RectCount = 0;
			}
		}
		SmallMenuChecking();
		return ;
	}
	else 
	{
		for( x=0; x<MAX_SMALL;x++ )
		{
			i = smallmenu[x];
			if(!SMenu[i].bActive) 
			{
				SMenu[i].CheakType = 0;
				SMenu[i].work = 0;		// ÀÌ°Ô ¹»±î?
				SMenu[i].nTemp = 0;
			}
			for(j=0;j<SMenu[i].nFieldCount;j++)
			{
				SMenu[i].nField[j].fRectMouse=false;
				SMenu[i].nField[j].RectCount = 0;
			}
		}
	}

	for(x=0;x<MAX_MAIN;x++)
	{
		i=mainmenu[x];
		if( mainmenu[x] == MN_MAININTERFACE && SkillStatus != SKILL_OFF ) continue;
		if( !MenuFocus(i) ) 
		{
			if(SMenu[i].bActive)   
			{
				for(j=0;j<SMenu[i].nFieldCount;j++)
				{
					if(MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
					{
						SMenu[i].nField[j].fRectMouse=TRUE;
						SMenu[i].nField[j].RectCount++;
						g_MouseInMenu = i;			// mouse°¡ menu¾È¿¡ µé¾î¿Ô´Ù.
					}
					else	
					{
						SMenu[i].nField[j].fRectMouse=false; // ±¸¼º ¿ä¼Ò ¾È¿¡ ÀÖÀ¸¸é TRUE, ¾Æ´Ï¸é FALSE
						SMenu[i].nField[j].fLButtonDown=false;
						SMenu[i].nField[j].fCheakFlag=false;
						SMenu[i].nField[j].RectCount = 0;
					}
				}
			}
			else
			{
				SMenu[i].CheakType = 0;		// ÃÊ±âÈ­¸¦ À§ÇØ	±× ¸Þ´º°¡ Ã³À½À¸·Î ¾×Æ¼ºê µÉ¶§ 1·Î ¼ÂÆÃÇÏ¸é¼­ ÃÊ±âÈ­ ½ÃÄÑÁØ´Ù.
				for(j=0;j<SMenu[i].nFieldCount;j++)
				{
					SMenu[i].nField[j].fRectMouse=false;
					SMenu[i].nField[j].RectCount = 0;
				}
			}
		}
		else
		{
			if( !SMenu[i].bActive ) SMenu[i].CheakType = 0;		
			for(j=0;j<SMenu[i].nFieldCount;j++)
			{
				SMenu[i].nField[j].fRectMouse=false;
				SMenu[i].nField[j].RectCount = 0;
			}
		}
	}


	if( fLMoveMenu || fRMoveMenu ) return;	// ¸Þ´º°¡ ¿òÁ÷ÀÌ°í ÀÖ´Ù¸é ¸¶¿ì½º Ã¼Å©¸¦ ÇÏÁö ¾Ê´Â´Ù.
	
	if(g_nRButtonState==STATE_BUTTON_PRESSED)				// When right button pressed
	{
		RButtonDown=true;
		if( g_MouseItemType==1 && (GetItemAttr(HandItemAttr.item_no, WEAR_ABLE) == WEAR_BELT) )			// µé°í ´Ù´Ï´Â ¾ÆÀÌÅÛÀÌ ÀÖÀ» °æ¿ì ±× ¾ÆÀÌÅÛÀÌ Æ÷¼ÇÀÌ¸é ¸¶½Å´Ù.
		{
			POS pos_s;
			SetItemPos(HAND, &pos_s);
			UseItemByRbutton( pos_s, HandItemAttr );
			g_MouseItemType = 0;
			g_MouseItemNumber = 0;
		}
		else
		{
			for(x=0;x<MAX_MAIN;x++)
			{
				i=mainmenu[x];
				if( mainmenu[x] == MN_MAININTERFACE && SkillStatus != SKILL_OFF ) continue;
				if( MenuFocus(i) ) continue;
				if( SMenu[i].bActive )
				{
					for(j=0; j<SMenu[i].nFieldCount; j++)
					{
						if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) )
						{
							SMenu[i].nField[j].fRButtonDown = true;
							DoRButtonDownOfMenu( i, j );
						}
//						else SMenu[i].nField[j].fRButtonDown = false;
					}
				}
			}
		}
	}
	else if( g_nRButtonState==STATE_BUTTON_STILL && RButtonDown ) 
	{
		for(x=0;x<MAX_MAIN;x++)
		{
			i=mainmenu[x];
			if( mainmenu[x] == MN_MAININTERFACE && SkillStatus != SKILL_OFF ) continue;
			for(j=0; j<SMenu[i].nFieldCount; j++)
			{
				if( !SMenu[i].nField[j].fRectMouse )
				{
					SMenu[i].nField[j].fRButtonDown = false;
//					RButtonDown = false;
				}
			}
		}
	}
	else if( g_nRButtonState == STATE_BUTTON_RELEASED )
	{
		RButtonDown=false;
		for(x=0;x<MAX_MAIN;x++)
		{
			i=mainmenu[x];
			if( mainmenu[x] == MN_MAININTERFACE && SkillStatus != SKILL_OFF ) continue;
			for(j=0; j<SMenu[i].nFieldCount; j++)
			{
				if( MenuFocus(i) ) continue;
				if( SMenu[i].nField[j].fRButtonDown )
				{
//					if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) )
					if( SMenu[i].nField[j].fRectMouse )
					{
						SMenu[i].nField[j].fRButtonCheck = true;
						DoRButtonCheckOfMenu( i, j );
					}
				}
				SMenu[i].nField[j].fRButtonDown = false;
			}
		}
	}

	if( g_nLDButtonState == STATE_BUTTON_DOUBLECLICK )	
	{
		for(x=0;x<MAX_MAIN;x++)
		{
			i=mainmenu[x];
			if( !MenuFocus(i) && SMenu[i].bActive )   
				for(j=0; j<SMenu[i].nFieldCount;j++) 
				{
					if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) && 
						MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox, FALSE))  //¸¶¿ì½º°¡ ¸Þ´º±¸¼º ¿ä¼ÒÀÇ rect¾È¿¡¼­ L¹öÆ°ÀÌ ´­·È³Ä //FALSE ÀÏ °æ¿ì´Â µå·¡±× ½ÃÀÛÁ¡À¸·Î ÆÇ´Ü
						DoLButtonDoubleClickOfMenu( i, j );					//  Å¸ÀÔ¿¡ µû¸¥ ½ÇÇà

				}
		}
	}

	if(g_nLButtonState==STATE_BUTTON_PRESSED)		//   ¿ÞÂÊ ¸¶¿ì½º ¹öÆ°ÀÌ ´­·È¾ú´ÂÁö¸¦ Ã¼Å©
	{
		LButtonDown=TRUE;
		StateMousePoint = g_pointMouse;
//		RectImageOn = false;
	}

	if(g_nLButtonState==STATE_BUTTON_STILL && LButtonDown)   // ÀÏ´Ü ¸¶¿ì½º°¡ ¾îµð¼±°¡ ¹æ±Ý ´­·È´Ù. ±×°Ô ¹üÀ§ ¾È¿¡¼­ ´­·È´ÂÁö, ¾Æ´ÑÁö ÆÇ´Ü ºÎºÐ
	{
		LButtonCount++;
		MouseDrag = TRUE;
		for(x=MAX_MAIN;x>=0;x--)		// ¸¶¿ì½º Ã¼Å©´Â °Å²Ù·Î
		{
			i=mainmenu[x];
			if( mainmenu[x] == MN_MAININTERFACE && SkillStatus != SKILL_OFF ) continue;
			if(!MenuFocus(i)) 
			{
				if(SMenu[i].bActive)   
				{
					for(j=0; j<SMenu[i].nFieldCount;j++) 
					{
						if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) && 
							MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox, FALSE))  //¸¶¿ì½º°¡ ¸Þ´º±¸¼º ¿ä¼ÒÀÇ rect¾È¿¡¼­ L¹öÆ°ÀÌ ´­·È³Ä //FALSE ÀÏ °æ¿ì´Â µå·¡±× ½ÃÀÛÁ¡À¸·Î ÆÇ´Ü
						{
							SMenu[i].nField[j].fLButtonDown=TRUE;
							DoLButtonDownOfMenu( i, j );					//  Å¸ÀÔ¿¡ µû¸¥ ½ÇÇà
							SMenu[i].nField[j].DragCount ++;
						}
						else if(MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
						{
							if(i == MN_FACE) 
							{
								SMenu[i].nField[j].fLButtonDown=TRUE;
								DoLButtonDownOfMenu( i, j );
							}
							else if(i == MN_ITEM && ( j==5 || j==6) ) 
							{
								SMenu[i].nField[j].fLButtonDown=TRUE;
								DoLButtonDownOfMenu( i, j );
								SMenu[i].nField[j].DragCount ++;
							
								if( g_MouseItemType==1 && SMenu[i].nField[j].DragCount == 10 ) 
								{
									DoButtonCheckOfMenu( i, j );
									SMenu[i].nField[j].DragCount=0;
								}
//								else SMenu[i].nField[j].DragCount = 0;
							}
						}
						else 
						{
							SMenu[i].nField[j].DragCount = 0;
						}
					}
				}
			}
		}
	}
	else 
	{
		LButtonCount = 0;
		if(g_nLButtonState==STATE_BUTTON_RELEASED) //¸¶¿ì½º°¡ ¹æ±Ý ¶³¾îÁø »óÅÂ³Ä?
		{
			bSoundOn = false;
			YouGotItemFlag = 0;	
			HandItemOn = false;
			int kkk = ItemDrop();
			LButtonDown=FALSE;

			for(x=MAX_MAIN;x>=0;x--)
			{
				i=mainmenu[x];
				if( mainmenu[x] == MN_MAININTERFACE && SkillStatus != SKILL_OFF ) continue;
				if( MenuFocus(i) )  continue;   //¿øÇÏ´Â ¸Þ´º ÇÏ³ª¿¡¸¸ ¸¶¿ì½º Æ÷Ä¿½º¸¦ µÐ´Ù
				if(! SMenu[i].bActive) continue;
				for(j=0; j<SMenu[i].nFieldCount;j++)
				{
					SMenu[i].nField[j].DragCount = 0;
					if( !MouseDrag || SMenu[i].nField[j].fLButtonDown )
					{
						if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
						{
							SMenu[i].nField[j].fCheakFlag=TRUE;
							DoButtonCheckOfMenu( i, j );
							f_ReturnOn = true;
						}						
					}
					else if( g_MouseItemType && CanMoveItemToMenu(SMenu[i].nField[j].nType))
					{
						if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
						{
							SMenu[i].nField[j].fCheakFlag=TRUE;
							DoButtonCheckOfMenu( i, j );
							f_ReturnOn = true;
						}
					}
					SMenu[i].nField[j].fLButtonDown = false;
				}
				if( f_ReturnOn ) 
				{
					MouseDrag = FALSE;
					return;
				}
			}
			if( SMenu[MN_FACE].bActive )
			{
				//<< 031022 kyo 
				SMenu[MN_FACE].nField[21].fCheakFlag=TRUE;
				//DoButtonCheckOfMenu( MN_FACE, 8 );
				//>> 031022 kyo
				return;
			}
			MouseDrag = FALSE;
		}
	}	
	PutMouseItemCheck();  // ¸¶¿ì½º µå·¡±× Áß ¶³¾î ¶ß¸±¶§
}


void MenuSetting()
{
	g_StartMenuOn = false;
	int x;
	for(x=0;x<MAX_MAIN;x++)  //ÀÏ´Ü ¸ðµç ¸Þ´º ±¸Á¶Ã¼µéÀ» ºñÈ°¼ºÀ¸·Î ¼³Á¤
	{
		SMenu[mainmenu[x]].bActive=FALSE;
		SMenu[mainmenu[x]].Id = mainmenu[x];
	}
	for(x=0;x<MAX_SMALL;x++)
	{
		SMenu[smallmenu[x]].bActive=FALSE;
		SMenu[smallmenu[x]].Id = smallmenu[x];
	}
	LoadHangulEnglishFont( "./data/han.fnt", "./data/eng.fnt" );
	HangulOutputArea( 0, SCREEN_WIDTH-1, 0, SCREEN_HEIGHT-1);

	InitMenu();
	ChatMgr.Clear();//020704 lsw//Çß´ø ´ëÈ­ »èÁ¦
	CurrentMsgMgr.Clear();//020704 lsw//Ä¿·±Æ® ¸Þ¼¼Áö »èÁ¦
	SetChatTarget(CHATTYPE_NORMAL);//020705 lsw
	HotKeyMemory.Clear();

	//ÀÌºÎºÐÀº ¸Å´º°¡ Ã³À½ ¼¼ÆÃ µÇ´Â °÷.( ·Î±×ÀÎÀÏ¶§)
	g_cQuest.DeleteScriptTimer();		// 021025 kyo ·Î±äÇÒ¶§ ¾ÈÁö¿öÁø Å¸ÀÌ¸Ó,Ä«¿îÅÍ ÀÌ¾²¸é Áö¿î´Ù. 
	g_cQuest.DeleteScriptCounter();		// 021025 kyo 
}


/**************************************************************************************/
//È°¼ºÈ­µÈ ¸Þ´ºÀÇ ±¸¼º¿ä¼ÒµéÀ» ¼öÇà
//Ãâ·ÂÀ» ÇÏ´Â ºÎºÐ
/**************************************************************************************/
void MenuSubProcessType(SMENU *SubMenu )
{
	int i,j,k;
	int code;
	int lenght;	
	int LenghtTotal;
	int ItemX,ItemY;													//¾ÆÀÌÅÛÀ» ¿Å±æ¶§ »ç¿ëÇÏ´Â Ã¼Å© À§Ä¡
	int nWeapon_Type;													//¹«±â Á¾·ù ÆÇ´ÜÀ» À§ÇÑ º¯¼ö (¹«±â, ¹æÆÐ, Çï¸ä, °©¿Ê)
	BOOL flag=TRUE; int & ability=nWeapon_Type;							// ÁÖÀÎ°ø ´É·ÂÄ¡ Ãâ·ÂÀ» À§ÇÑ ÀÓ½Ã º¯¼ö

	int &iMainX		= SubMenu->x;//soto_030511
	int &iMainY		= SubMenu->y;//soto_030511
	
	static int nCursor_Flicker=0;   //±ô¹ÚÀÌ´Â Ä¿¼­¸¦ À§ÇÑ º¯¼ö

	i = SubMenu->Id;
	for(j=0;j<SubMenu->nFieldCount;j++)   //¸Þ´º¿¡ ±¸¼º¿ä¼ÒµéÀ» ¼öÇàÇØ¶ó
	{
		if( !SubMenu->bActive ) break;
		Hcolor( FONT_COLOR_NUMBER );
		SetHangulAlign( TA_LEFT );

		switch(SubMenu->nField[j].nType)
		{
		case FT_LOCALWAR_JOIN :			// LTS MENU		// LTS LOCALWAR		// LTS NEW LOCALWAR
			{
				if (j==0)			// Å¬¸°ÇÑ°Å¸¦ ¼±ÅÃÇÑ´Ù.
				{
					int Index=GetLocalWarMapMoveIndex();
					//				AddCurrentStatusMessage(255,255,0,"%d¹øÀÌ ¼±ÅÃµÇ¾ú½À´Ï´Ù.",GetLocalWarMapMoveIndex());
					Spr *s = GetSprOfMenu( SubMenu->nField[Index].nImageType, SubMenu->nField[Index].nRectImage );
					if( s )	PutCompressedImage( SubMenu->x+SubMenu->nField[Index].x+s->ox, SubMenu->y+SubMenu->nField[Index].y+s->oy, s);	// °á°è¼® »Ç°³Áø°Å 
					DrawLocalWarDefeat(SubMenu->nField[j].nImageType,j,SubMenu->x,SubMenu->y);	// °á°è¼® »Ç°³Áø°Å
				}
				
				if(SubMenu->nField[j].fRectMouse)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType );
					Spr *s = GetSprOfMenu( SubMenu->nField[j].nImageType, SubMenu->nField[j].nRectImage );
					if( s )	PutCompressedImage( SubMenu->x+SubMenu->nField[j].x+s->ox, SubMenu->y+SubMenu->nField[j].y+s->oy, s);		// ¼±ÅÃµÈ°Å
					s=GetSprOfMenu( SubMenu->nField[j].nImageType, 8 );
					if (s) PutCompressedImage(SubMenu->x+335+s->ox,SubMenu->y+275+s->oy, s );		// Á¡¼ö 
					if (SubMenu->nField[j].fLButtonDown) LocalWarMapMoveClick(j);
					DrawLocalWarJoinInterface(SubMenu->nField[j].nImageType,j,SubMenu->x,SubMenu->y);	// °á°è¼® »Ç°³Áø°Å
					DrawLocalWarDefeat(SubMenu->nField[0].nImageType,0,SubMenu->x,SubMenu->y);	// °á°è¼® »Ç°³Áø°Å
				}
				
				break;
			}
		case FT_NOMAL_PUT : if( SubMenu->nField[j].nImageNumber || SubMenu->nField[j].nImageType || SubMenu->Id == MN_SYSTEM_OPTION )
							{
								if( SubMenu->Id == MN_MAININTERFACE && j == 17 && SCharacterData.nLevel <= 8 )
								{
									static int flag = 0;
									if( flag%24 < 12 )
									{
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j+1].nRectImage, SubMenu->nField[j+1].nImageType );
										Spr *s = GetSprOfMenu( SubMenu->nField[j+1].nImageType, SubMenu->nField[j+1].nRectImage );
										if( s )
										PutCompressedImageFX( SubMenu->x+SubMenu->nField[j].x+s->ox, SubMenu->y+SubMenu->nField[j].y+s->oy, s, 4, 2 );
									}
									else 
									{
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
									}
									flag++;
								}
								else
								{
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
									if( (SubMenu->Id == MN_ARIGEMENT && j==0 )|| (SubMenu->Id == MN_STATUS && j==6) || (SubMenu->Id == MN_ABILITY && j==2) )
									{
										static int stch=0;
										static int de=0;
										FieldTypeNomalPutFx(SubMenu->x+11, SubMenu->y+6, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharacterData.nCharacterData[ARIGEMENT],de, 2);
										if(stch > 15) de-=2;
										else de+=2;
										stch++;
										stch %= 32;
									}
								}
							}
							break;
		case FT_NOMAL_PUT_CENTER : { 
										Spr *s = GetSprOfMenu(SubMenu->nField[j].nImageType, SubMenu->nField[j].nImageNumber );
										if( !s ) break;
										Spr *main = GetSprOfMenu(SubMenu->nImageType, SubMenu->nImageNumber );
										if( !main ) break;
										PutCompressedImage(SubMenu->x+main->ox, SubMenu->y+SubMenu->nField[j].y+s->oy, s);

										break;
								   }

		case FT_SYSTEM_OPTION_SCROLL :
		case FT_HIDE_AUTO_PUT :	if(SubMenu->nField[j].fRectMouse)
								{
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
//									if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
								}
		case FT_HIDE_PUT :	 if(SubMenu->nField[j].fLButtonDown)
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
							 break;

		case FT_HIDE_WILLDO_AUTO_PUT_MAGIC :	if(SubMenu->nField[j].fRectMouse)
												{
//													if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
													if( SubMenu->nField[j].nSpecialWillDo == 100 ) PrintQuickKey( "PGUP" );
													else PrintQuickKey( "PGDN" );
												}
		case FT_HIDE_WILLDO_AUTO_PUT :	if(SubMenu->nField[j].fRectMouse)
										{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }

											if(SubMenu->nField[j].nRectImage == 16 || SubMenu->nField[j].nRectImage == 23 || SubMenu->nField[j].nRectImage == 758)
											{
												static int tt;
												static int de =10;
												if( tt ) {	de ++;	if( de > 12 ) { tt = !tt; }	}
												else { de --; if( de < 3 ) { tt = !tt; } } 
												FieldTypeNomalPutFx(SubMenu->x,SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y , SubMenu->nField[j].nRectImage , de, 2 );
											}
											else FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
										}
		case FT_HIDE_WILLDO_PUT :		if(SubMenu->nField[j].fLButtonDown)
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
										}
										break;

		case FT_PUT_BUTTON : 
			{
				int image1 = SubMenu->nField[j].nSHideNomalCount;	// Æò¼Ò ¹öÆ°
				int image2 = SubMenu->nField[j].nRectImage;			// ¹àÀº ¹öÆ°
				int image3 = SubMenu->nField[j].nImageNumber;		// ´­¸° ¹öÆ°
				int image_type = SubMenu->nField[j].nImageType;		// ÀÌ¹ÌÁö Å¸ÀÔ

				if(SubMenu->nField[j].fLButtonDown)
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, image3, image_type );
				else if( SubMenu->nField[j].fRectMouse)
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, image2, image_type );
				else
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, image1, image_type );

				break;
			 }

		case FT_BBS_GO :{
							int para = SubMenu->nField[SubMenu->nField[j].nSHideNomalStart].nSHideNomalStart+SubMenu->nField[j].nSHideNomalCount;
							if( !(*c_BbsTitle[para].m_Name) ) break;

							if( SubMenu->nField[j].fRectMouse )
							{
//								if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
								static int tt;
								static int de =10;
								if( tt ) {	de ++;	if( de > 12 ) { tt = !tt; }	}
								else { de --; if( de < 3 ) { tt = !tt; } } 
								FieldTypeNomalPutFx(SubMenu->x,SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y , SubMenu->nField[j].nRectImage, de, 2 );
							}
							int t = SubMenu->work;
							if( t )
							{
								static int start = SubMenu->nField[SubMenu->nField[j].nSHideNomalStart].nSHideNomalStart;
								if( start != SubMenu->nField[SubMenu->nField[j].nSHideNomalStart].nSHideNomalStart )
								{
									SubMenu->work = 0;
									start = SubMenu->nField[SubMenu->nField[j].nSHideNomalStart].nSHideNomalStart;
									break;
								}
									
								FieldTypeNomalPut(SubMenu->x,SubMenu->y, SubMenu->nField[t].x, SubMenu->nField[t].y , SubMenu->nField[t].nImageNumber);
								para = SubMenu->nField[SubMenu->nField[t].nSHideNomalStart].nSHideNomalStart+SubMenu->nField[t].nSHideNomalCount;
								int date = SubMenu->nField[t].nShideNomalPlus;

								Hcolor( FONT_COLOR_NAME );
								Hprint2( SubMenu->x+SubMenu->nField[date].x, SubMenu->y+SubMenu->nField[date].y, g_DestBackBuf, "            %s", c_BbsTitle[para].m_Name);

								Hcolor( FONT_COLOR_NUMBER );
								Hprint2( SubMenu->x+SubMenu->nField[date].x, SubMenu->y+SubMenu->nField[date].y, g_DestBackBuf, "%04d/%02d/%02d", c_BbsTitle[para].m_Year, c_BbsTitle[para].m_Month, c_BbsTitle[para].m_Day );
							}
							break;
						}

		case FT_MAIN_MENU : {
								int &bLButtonDownOn = SubMenu->nField[j].nShideNomalPlus;

								if( (SubMenu->nField[j].RectCount > 5) && !bLButtonDownOn )
								{
									Hcolor( 0 );
									SetHangulAlign( TA_CENTER );
									switch( SubMenu->nField[j].nSpecialWillDo )
									{
										case SWD_MAININTERFACE_ITEM	 :	MenuButtonRightExplain( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, "Item Inventory" );
																		PrintQuickKey( "S" );
																		break;
										case SWD_MAININTERFACE_MAGIC :	MenuButtonRightExplain( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, "Magic Select" );
																		PrintQuickKey( "W" );
																		break;								
										case SWD_MAININTERFACE_PARTY :	MenuButtonLeftExplain( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, "Party" );
																		PrintQuickKey( "Q" );
																		break;
										case SWD_MAININTERFACE_SKILL :	MenuButtonLeftExplain( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, "SKILL" );
																		PrintQuickKey( "A" );
																		break;

										case SWD_MENU_OPEN : switch( SubMenu->nField[j].nWillDo )
															 {
																case MN_SYSTEM :	MenuButtonRightExplain( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, "System Option" );
																					PrintQuickKey( "X" );
																					break;
															 }
															 break;
									}
								}
								else SubMenu->nField[j].nSHideNomalCount = 100;

								if( SubMenu->nField[j].fLButtonDown || SubMenu->nField[j].fCheakFlag ) bLButtonDownOn = true;
								if( !SubMenu->nField[j].fRectMouse ) bLButtonDownOn = false;

							}
		case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT :{
												if(SubMenu->nField[j].fRectMouse)
												{
//													if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
													switch( SubMenu->nField[j].nSpecialWillDo )
													{
														case SWD_MAININTERFACE_STATUS : PrintQuickKey( "C" ); break;
														case SWD_INFO_FLAG_SET : PrintQuickKey( "M" ); break;
																				break;
													}												

													if( (SubMenu->nField[j].nImageType==DEFAULT_MENU) && ( SubMenu->nField[j].nRectImage == 16 || SubMenu->nField[j].nRectImage == 23 || SubMenu->nField[j].nRectImage == 758) )
													{
														static int tt;
														static int de =10;
														if( tt ) {	de ++;	if( de > 15 ) { tt = !tt; }	}
														else { de --; if( de < 7 ) { tt = !tt; } } 
														FieldTypeNomalPutFx(SubMenu->x,SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y , SubMenu->nField[j].nRectImage, de, 2 );
													}
													else FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
												}
											  }
		case FT_HIDE_SPECIAL_WILLDO_PUT :		
			if(SubMenu->nField[j].fLButtonDown)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
				if( (SubMenu->Id == MN_ARIGEMENT && j==1 )|| (SubMenu->Id == MN_STATUS && j==7) || (SubMenu->Id == MN_ABILITY && j==3) )
				{
					static int stch=0;
					static int de=0;
					FieldTypeNomalPutFx(SubMenu->x+11, SubMenu->y+6, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharacterData.nCharacterData[ARIGEMENT],de, 2);
					if(stch > 15) de-=2;
					else de+=2;
					stch++;
					stch %= 32;
				}
			}break;
		case FT_MAKE_RARE_MORE://011013 lsw
		case FT_RARE_GROUP_SCROLL://011014 lsw 
		//011004 lsw >
		case FT_SHOW_SPECIAL_WILLDO_AUTO_PUT :
			{
					if(SubMenu->nField[j].fRectMouse)
					{
						if(SubMenu->nField[j].fLButtonDown)//¹öÆ° Å¬¸¯ ÀÌº¥Æ®
						{
							FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType );
						}
						else
						{
							FieldTypeNomalPut   (SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y , SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
							FieldTypeNomalPutFx3(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y , SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType, 3, 2 );
						}
					}
					else
					{
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
					}					
			}
			break;
		//011004 lsw <
		case FT_MENU_MOVE_DEFAULT : 
								{							// ¸Þ´º°¡ ¿òÁ÷ÀÎ´Ù~~
									if( SubMenu->nTemp && MouseDrag)
									{
										static int mox, moy;

										int &GabX = SubMenu->nField[j].nSHideNomalCount;
										int &GabY = SubMenu->nField[j].nSHideNomalStart;

										Spr *s = GetSprOfMenu( SubMenu->nImageType, SubMenu->nImageNumber );
										if( !s ) return;

										if( g_pointMouseX < 0 || g_pointMouseX > GAME_SCREEN_XSIZE || g_pointMouseY < 0 || g_pointMouseY > GAME_SCREEN_YSIZE ) 
										{
											Box_( Mapx+mox, Mapy+moy, s->xl, s->yl, RGB16(255,255,255) );
											break;
										}						

										if( g_pointMouseX - GabX < 0 )	mox = 0;
										else if( g_pointMouseX+(s->xl-GabX) > GAME_SCREEN_XSIZE )	mox = GAME_SCREEN_XSIZE - s->xl;
										else	mox = g_pointMouseX - GabX;
										
										if( g_pointMouseY - GabY < 0 )	moy = 0;
										else if( g_pointMouseY+(s->yl-GabY) > GAME_SCREEN_YSIZE )	moy = GAME_SCREEN_YSIZE - s->yl;
										else	moy = g_pointMouseY - GabY;

										Box_( Mapx+mox, Mapy+moy, s->xl, s->yl, RGB16(255,255,255) );
#ifdef _DEBUG // finito 060507
										if( GetSysInfo( SI_GAME_MAKE_MODE ) )
										{
											SetHangulAlign( TA_CENTER );
											Hcolor( 255, 255, 255 );
											Hprint2( 280, 80, g_DestBackBuf, "( %d, %d )", SubMenu->x, SubMenu->y );
										}
#endif
										SubMenu->nField[j].nShideNomalPlus = mox*1000 + moy;
									}

									if( SubMenu->nTemp && !LButtonDown )
									{
										SubMenu->x = SubMenu->nField[j].nShideNomalPlus / 1000;
										SubMenu->y = SubMenu->nField[j].nShideNomalPlus % 1000;
										SubMenu->nTemp = false;
										SubMenu->nField[j].nSHideNomalCount = 0;
										SubMenu->nField[j].nSHideNomalStart = 0;
										SubMenu->nField[j].nShideNomalPlus = 0;
									}
									break;
								}

		case FT_MENU_MOVE : {							// ¸Þ´º°¡ ¿òÁ÷ÀÎ´Ù~~
								if( !SMenu[ MN_ITEM ].bActive ) 
								{
									SmallMenuClose();
									break;
								}
								if( SubMenu->nTemp && MouseDrag)
								{
									static int mox, moy;

									int &GabX = SubMenu->nField[j].nSHideNomalCount;
									int &GabY = SubMenu->nField[j].nSHideNomalStart;

									if( g_pointMouseX < 0 || g_pointMouseX > 320+GABX_SCREEN*2 || g_pointMouseY < 0 || g_pointMouseY > 352+GABY_SCREEN*2 ) 
									{
										Box_( Mapx+mox, Mapy+moy, spr[SubMenu->nImageNumber].xl, spr[SubMenu->nImageNumber].yl, RGB16(255,255,255) );
										break;
									}						

									if( g_pointMouseX - GabX < 0 )	mox = 0;
									else if( g_pointMouseX+(spr[SubMenu->nImageNumber].xl-GabX) > 320+GABX_SCREEN*2  )	mox = 320+GABX_SCREEN*2  - spr[SubMenu->nImageNumber].xl;
									else	mox = g_pointMouseX - GabX;
									
									if( g_pointMouseY - GabY < 0 )	moy = 0;
									else if( g_pointMouseY+(spr[SubMenu->nImageNumber].yl-GabY) > GAME_SCREEN_YSIZE )	moy = GAME_SCREEN_YSIZE - spr[SubMenu->nImageNumber].yl;
									else	moy = g_pointMouseY - GabY;

									Box_( Mapx+mox, Mapy+moy, spr[SubMenu->nImageNumber].xl, spr[SubMenu->nImageNumber].yl, RGB16(255,255,255) );
#ifdef _DEBUG // finito 060507									
									if( GetSysInfo( SI_GAME_MAKE_MODE ) )
									{
										SetHangulAlign( TA_CENTER );
										Hcolor( 255, 255, 255 );
										Hprint2( 280, 80, g_DestBackBuf, "( %d, %d )", SubMenu->x, SubMenu->y );
									}
#endif
									SubMenu->nField[j].nShideNomalPlus = mox*1000 + moy;
								}

								if( SubMenu->nTemp && !LButtonDown )
								{
									SubMenu->x = SubMenu->nField[j].nShideNomalPlus / 1000;
									SubMenu->y = SubMenu->nField[j].nShideNomalPlus % 1000;
									SubMenu->nTemp = false;
									SubMenu->nField[j].nSHideNomalCount = 0;
									SubMenu->nField[j].nSHideNomalStart = 0;
									SubMenu->nField[j].nShideNomalPlus = 0;
								}
								break;
							}
		case FT_MENU_MOVE_DIRECT : 							// ¸Þ´º°¡ ¿òÁ÷ÀÎ´Ù~~
							{
								if( !SubMenu->nTemp ) break;
								if( SubMenu->key != j ) break;
								if( IsRightWindowOpen() && g_pointMouseX > 321+GABX_SCREEN) break;
								if( IsLeftWindowOpen() && g_pointMouseX < 321  ) break;
								if( MouseDrag )
								{
									int move_right = SCREEN_WIDTH;
									int move_left = 0;
									bool is_right_on = IsRightWindowOpen();
									bool is_left_on = IsLeftWindowOpen();

									if( is_right_on && is_left_on ) break;
									else if( is_right_on ) move_right = 321+GABX_SCREEN;
									else if( is_left_on ) move_left = 321;

									static int mox, moy;

									int &GabX = SubMenu->nField[j].nSHideNomalCount;
									int &GabY = SubMenu->nField[j].nSHideNomalStart;

									if( g_pointMouseX < 0 || g_pointMouseX > SCREEN_WIDTH || g_pointMouseY < 0 || g_pointMouseY > GAME_SCREEN_YSIZE ) 
									{
										break;
									}

									Spr *s = GetSprOfMenu( SubMenu->nImageType, SubMenu->nImageNumber );
									if( !s ) break;

									if( g_pointMouseX - GabX < move_left )	mox = move_left;
									else if( g_pointMouseX+(s->xl-GabX) > move_right )	mox = move_right - s->xl;
									else	mox = g_pointMouseX - GabX;
									
									if( g_pointMouseY - GabY < 0 )	moy = 0;
									else if( g_pointMouseY+(s->yl-GabY) > GAME_SCREEN_YSIZE )	moy = GAME_SCREEN_YSIZE- s->yl;
									else	moy = g_pointMouseY - GabY;

									SubMenu->x = mox;
									SubMenu->y = moy;
									SubMenu->nField[j].nShideNomalPlus = mox*1000 + moy;
									//int ret = isCrashSmallMapByOther();
								}
								else 
								{
									SubMenu->nTemp = 0;
									SubMenu->nField[j].nShideNomalPlus = 0;
								}
								break;
							}

		case FT_HIDE_ISCROLL_AUTO_PUT :	if( ((SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalStart) + (SubMenu->nField[SubMenu->nField[j].nWillDo].nShideNomalPlus)) >= (SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalCount) )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
											break;
										}
										if(SubMenu->nField[j].fRectMouse)
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
										}
		case FT_HIDE_ISCROLL_PUT :	if(SubMenu->nField[j].fLButtonDown)
									{   //½ºÅ©·Ñ µÉ°æ¿ì¸¦ »ý°¢ÇØ ¹Ì¸® ¹üÀ§¸¦ Ã¼Å©ÇØ ½ºÅ©·ÑÀ» ÇØ¼± ¾ÈµÇ¸é breakÇÑ´Ù
//										if( ((SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalStart) + (SubMenu->nField[SubMenu->nField[j].nWillDo].nShideNomalPlus)) >= (SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalCount) )
//											break;
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
										
									}
									break;

		case FT_HIDE_DSCROLL_AUTO_PUT :	if( ((SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalStart) - (SubMenu->nField[SubMenu->nField[j].nWillDo].nShideNomalPlus)) < 0 )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
											break;
										}
										if(SubMenu->nField[j].fRectMouse)
										{
											if( SubMenu->Id == MN_ITEM ) PrintQuickKey( "PGUP" );
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
										}
		
		case FT_HIDE_DSCROLL_PUT :	 if(SubMenu->nField[j].fLButtonDown)
									 {
//										if( ((SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalStart) - (SubMenu->nField[SubMenu->nField[j].nWillDo].nShideNomalPlus)) < 0 )
//											break;
									 if( SubMenu->Id == MN_ITEM ) PrintQuickKey( "PGDN" );
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
									 }
									 break;

		case FT_PARTY : {
							if( !SubMenu->CheakType )
							{
								SubMenu->CheakType = 1;
								Kein_PutMessage( KM_INFO, kein_GetMenuString( 77 ) );
								Kein_PutMessage( KM_INFO, kein_GetMenuString( 126 ) );
								Kein_PutMessage( KM_INFO, kein_GetMenuString( 127 ) );
							}
							int para = SubMenu->nField[j].nWillDo;
							CharacterParty target={0,};

							switch( SubMenu->nTemp )
							{
								case PARTY :		target = SCharacterData.party[para]; break;
								case RELATION :		target = SCharacterData.relation[para]; break;
								case EMPLOYMENT :	target = SCharacterData.employment[para]; break;
							}
							
							if( *target.m_Name )
							{
								if( SubMenu->nField[j].fLButtonDown )
									PutCharImage( SubMenu->x+SubMenu->nField[j].x-30, SubMenu->y+SubMenu->nField[j].y-35, target.m_ImageNo, 6, FS_SMALL,2, target.m_Gender );
								else PutCharImage( SubMenu->x+SubMenu->nField[j].x-30, SubMenu->y+SubMenu->nField[j].y-35, target.m_ImageNo, 0, FS_SMALL, 0, target.m_Gender );
							}
							break;
						}

		case FT_PARTY_FACE: {
								CharacterParty leader={ 0, };
								int para = SubMenu->nField[j].nWillDo;

								FieldTypeNomalPut( SubMenu->x, SubMenu->y, 148, 110, 27, MAIN_ETC );		// ·¹º§, strÂï´Â ÆÇ..

								switch( SubMenu->nTemp )
								{
									case PARTY :		leader = SCharacterData.party[para]; break;
									case RELATION :		leader = SCharacterData.relation[para]; break;
									case EMPLOYMENT :	leader = SCharacterData.employment[para]; break;
								}

								if( *leader.m_Name )
								{
									PutCharImage( SubMenu->x+SubMenu->nField[j].x-30, SubMenu->y+SubMenu->nField[j].y-35, leader.m_ImageNo, 0, FS_SMALL, 0, leader.m_Gender );
									SetHangulAlign( TA_RIGHT );
									if( !leader.m_Str )				// 0620 YGI
									{
										//Hprint2( SubMenu->x+275, SubMenu->y+115,g_DestBackBuf, "Unknown" );
									}
									else
									{
										//Hprint2( SubMenu->x+278, SubMenu->y+116,g_DestBackBuf, "%d", leader.m_Str );
										SetHangulAlign( TA_LEFT );
										//Hprint2( SubMenu->x+209, SubMenu->y+116,g_DestBackBuf, "%d", leader.m_Lv );
										//PutLvNameByColor( SubMenu->x+162, SubMenu->y+116, leader.m_Lv , leader.m_Class );		// 0811 YGI
									}
									
									Hcolor( FONT_COLOR_DEFAULT );
									SetHangulAlign( TA_CENTER );
									Hprint2( SubMenu->x+236, SubMenu->y+144, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][leader.m_Class].temp );

									Hcolor( FONT_COLOR_NAME );
									Hprint2( SubMenu->x+221, SubMenu->y+99 ,g_DestBackBuf, "%s", leader.m_Name );
								}
								
								// ÆÄÆ¼·Î ÅëÇØ ¾ò´Â °æÇèÄ¡ º¸¿©ÁÖ±â
								// 2, 3, 4
								SubMenu->nField[2].nType = FT_PUT_BUTTON;
								SubMenu->nField[3].nType = FT_PUT_BUTTON;
								SubMenu->nField[4].nType = FT_PUT_BUTTON;

								switch( g_Menu_Variable.m_PartySelect )
								{
									case 0 :
									case PS_EXP :
										SubMenu->nField[2].nType = FT_NOMAL_PUT;
										break;

									case PS_TACTIC :
										SubMenu->nField[3].nType = FT_NOMAL_PUT;
										break;

									case PS_MAGIC_TAC :
										SubMenu->nField[4].nType = FT_NOMAL_PUT;
										break;
								}
								break;
							}
	
		case FT_LEVELUP_BUTTON : {
									static int ct=-1;
									ct++;
									if(ct > 20)
									{
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
										if(ct > 40)	ct = 0;
									}

									if(SubMenu->nField[j].fRectMouse) PrintQuickKey( "U" );
									if(SubMenu->nField[j].fLButtonDown)
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									break;
								 }
		case FT_LEVELUP :	
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nSpecialWillDo,SubMenu->nField[j].nImageType);
				if(SubMenu->nField[j].fRectMouse)
				{
//								if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage,SubMenu->nField[j].nImageType);//020515 lsw
				}
				if(SubMenu->nField[j].fLButtonDown)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber,SubMenu->nField[j].nImageType);//020515 lsw
				}
			}break;

		case FT_FIRE :	y_delay++;
						if(y_delay==5)
						{
							cDiceCount++; // ´ÙÀÌ½º¿¡¼­ Ä«¿îÅÍ¸¦ Ã¼Å©ÇÏ´Â º¯¼ö¸¦ ÀÌ¿ë
							y_delay=0;
						}
						if(cDiceCount==33) cDiceCount=0; //ÃÑ 33ÇÁ·¹ÀÓ
						FieldTypeNomalPutFx(SubMenu->x,SubMenu->y,SubMenu->nField[j].x, SubMenu->nField[j].y-spr[cDiceCount+173].yl , cDiceCount+173, 15, 2 ); 
						break;

		case FT_GOD_INTRUDUCTION : {		//	½ÅÀü ¼³¸í Ãâ·Â
										char *text;

										text ="";
										switch( SubMenu->nTemp )
										{
											case MN_GOD_EDELBLHOY : text = lan->OutputMessage(7,76); break;//010215 lsw
											case MN_GOD_TEFFERY : text = lan->OutputMessage(7,77); break;//010215 lsw
											case MN_GOD_LETTY : text = lan->OutputMessage(7,78); break;//010215 lsw
											case MN_GOD_OREM : text = kein_GetMenuString( 35 ); break;
											default : break;
										}

										if( !*text ) break;
										RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage, text); 
										break;
								   }
		case FT_GOD_MEETING :{
								char *text;
								text ="";
								int &count = SubMenu->nField[j].nSHideNomalStart;

								if( !SubMenu->CheakType )
								{
									SubMenu->CheakType = 1;
									count = 0;		// ½ºÅ©·ÑÀ» Ã³À½À¸·Î µ¹·Á ³õ´Â´Ù.
								}
								
								switch( SubMenu->nTemp )
								{
									case MN_GOD_EDELBLHOY : 
										{
											switch( count )
											{
												case 0:	text = lan->OutputMessage(7,79);break;//010215 lsw
												case 1: text = lan->OutputMessage(7,80); break;//010215 lsw
											}
											break;
										}	
									case MN_GOD_TEFFERY : 
										{
											switch( count )
											{
												case 0:	text = lan->OutputMessage(7,81);break;
												case 1: text = lan->OutputMessage(7,82); break;
											}
											break;
										}	
									case MN_GOD_LETTY : 
										{
											switch( count )
											{
												case 0:	text = lan->OutputMessage(7,83);break;//010215 lsw
												case 1: text = lan->OutputMessage(7,84); break;//010215 lsw
											}
											break;
										}	
									case MN_GOD_OREM :
										{
											switch( count )
											{
												case 0:	text = lan->OutputMessage(7,83);break;//010215 lsw
												case 1: text = lan->OutputMessage(7,84); break;//010215 lsw
											}
											break;
										}
									default : break;
								}


								if( !*text ) break;
								RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage, text); 
								break;
							}
		case FT_GOD_REGIST_OKCANCEL	: {
										char text[20]="";
										char text2[128];

										GetBelieveGodByMenu( SubMenu->nTemp, text );
										if( !*text ) break;

										sprintf( text2, lan->OutputMessage(7,85), text );//010215 lsw
										RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage, text2); 
										break;
									  }
		case FT_PREACH_NAME : 
			{// ÀÌ¸§ Âï´Â Ä­
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
				int &count = SubMenu->nField[j].nSHideNomalCount;
				char *name = SubMenu->nField[j].temp;
				SetFocus2(HWND_3);
				EWndMgr.GetTxt(HWND_3, name, 20);//021001 lsw
				if( count % 16 < 8 ) 
				{
					char name2[30];
					sprintf(name2, "%s_", name );
					RectTextPut(SubMenu->x+SubMenu->nField[j].x+7,SubMenu->y+SubMenu->nField[j].y+9,SubMenu->nField[j].nRectImage, name2);
				}
				else RectTextPut(SubMenu->x+SubMenu->nField[j].x+7,SubMenu->y+SubMenu->nField[j].y+9,SubMenu->nField[j].nRectImage, name);
				count++;
			}break;
		case FT_GOD_REGIST_ERROR : {
										char text[1024];
										char god[40];
										int belive = GetBelieveGodByMenu( SubMenu->nTemp, god );
										
										if( SCharacterData.believe_god == belive )
										{
											sprintf( text, lan->OutputMessage(7,86) );//010215 lsw
										}
										else sprintf( text, lan->OutputMessage(7,87), god );//010215 lsw
										
										RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage, text);
										break;
								   }
			//------------------------------------------
		case FT_PUBLIC_BBS : {
								if( !public_bbs.m_pszMessage ) break;
								int &work = SubMenu->key;
								if( !SubMenu->CheakType )
								{
									SubMenu->CheakType = 1;
									work = 0;
								}

								// 6ÁÙ¸¸ Âï´Â´Ù.
								Hcolor( 44, 44, 34 ); 
								SkipRectTextPut( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nWillDo, work*6, public_bbs.m_pszMessage, 6, 0 );	// 7ÁÙÀ» ±âÁØÀ¸·Î ÆäÀÌÁö¸¦ ³Ñ±ä´Ù.
								SkipRectTextPut( SubMenu->x+1+SubMenu->nField[j].x, SubMenu->y+1+SubMenu->nField[j].y, SubMenu->nField[j].nWillDo, work*6, public_bbs.m_pszMessage, 6, 0 );	// 7ÁÙÀ» ±âÁØÀ¸·Î ÆäÀÌÁö¸¦ ³Ñ±ä´Ù.
								break;
							 }

		case FT_PUBLIC_BBS_NEXT : {
									if( SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fLButtonDown )
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
									}
									else 
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType );
									break;
								  }
		case FT_BELIEVE_GOD	:{
								if( !SubMenu->CheakType )		// ¸ÇÃ³À½ ¿­·ÈÀ»¶§..
								{
									SubMenu->CheakType = 1;
								}

								if( SCharacterData.believe_god ) 
								{
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );		// ¸ÕÀú Ä­À» Âï°í
									char *god;
									switch( SCharacterData.believe_god ) 
									{
										case EDELBLHOY	: god = "EDELBLHOY"; break;
										case TEFFERY	: god = "TEFFERY"; break;
										case LETTY		: god = "LETTY"; break;
										case OREM		: god = "OREM"; break;
										default : god = ""; break;
									}
									SetHangulAlign( TA_RIGHT );
									Hcolor( FONT_COLOR_NAME );
									Hprint2( SubMenu->x+SubMenu->nField[j].x+140, SubMenu->y+SubMenu->nField[j].y+6, g_DestBackBuf, "%s", god );
									Hcolor( FONT_COLOR_NUMBER );
									Hprint2( SubMenu->x+SubMenu->nField[j].x+286, SubMenu->y+SubMenu->nField[j].y+6, g_DestBackBuf, "%d", SCharacterData.faith);
								}
								break;
							 }
		case FT_QUICK_SKILL_SCROLL : {
										if( SubMenu->nField[j].fLButtonDown || SubMenu->nField[j].fRectMouse )
										{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );		// ¸ÕÀú Ä­À» Âï°í
										}
										break;
									 }
		case FT_QUICK_SKILL_SCROLL_LR :{
										int specialwilldo = SubMenu->nField[j].nSpecialWillDo;
										int count = SubMenu->nField[specialwilldo].nSHideNomalCount;
										int start = SubMenu->nField[specialwilldo].nSHideNomalStart;

										int willdo = SubMenu->nField[j].nWillDo;
										if( willdo )
										{
											if( start+1 >= count ) 
											{
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
												break;
											}
										}
										else 
										{
											if( start-1 <= 0 ) 
											{
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
												break;
											}
										}

										if( SubMenu->nField[j].fLButtonDown )
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber+2, SubMenu->nField[j].nImageType );
										else if( SubMenu->nField[j].fRectMouse )
										{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber+1, SubMenu->nField[j].nImageType );
										}
										else FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );

										break;
									   }
		case FT_HUNTER_REGIST :{//021001 lsw
									int willdo = SubMenu->nField[j].nWillDo;
									int &count = SubMenu->nField[j].nSHideNomalCount;

									static DWORD old_money = 0;

									if( !SubMenu->CheakType )
									{
										SubMenu->CheakType = 1;

										SetChatMode(CM_COMMAND);

										SetFocus2(HWND_MAIL1);//021001 lsw
										SubMenu->work = 1;
										old_money = 0;
									}
									char text[2048];
									int max;
									int iHWndIndex = 0;
									switch( willdo )
									{
										case 1: iHWndIndex = HWND_MAIL1; max = 20;   break;
										case 2: iHWndIndex = HWND_MAIL2; max = 29;  break;
										case 3: iHWndIndex = HWND_MAIL3; max = 1000; break; 
									}
									if( !iHWndIndex ){ break; }

									EWndMgr.GetTxt( iHWndIndex, text,max );
									switch(iHWndIndex)
									{
									case HWND_MAIL3:
										{
											int line = GetLineRectTextPut( SubMenu->nField[j].nRectImage, text );
											if( line > 4 ) 
											{
												char *divide_text = DivideStringByRect( SubMenu->nField[j].nRectImage, text, 4 );
												if( divide_text  ) *divide_text = NULL;
												EWndMgr.SetTxt(HWND_MAIL3,text);//021001 lsw
											}
										}break;
									case HWND_MAIL2:
										{
											int flag = false;
											DWORD money=0;
											if( CheckNumber( text, strlen( text ) ) ) flag = true;
											else money = (DWORD)atoi( text );
											if( flag || money > SCharacterData.nMoney ) 
											{
												if( text[0] == ' ') 
												{
													old_money = 0;
													break;
												}
												MP3( SN_WARNING );
												itoa( old_money, text, 10 );
												EWndMgr.SetTxt(HWND_MAIL2,text);//021001 lsw
											}
											else old_money = money;
										}break;
									}

									if( SubMenu->work == willdo )
									{
										SetFocus2( iHWndIndex );
										if( count %16 > 8 ) 
										{
											sprintf( text, "%s_", text );
										}
										count++;
									}

									RectTextPut( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, text );
									break;
							   }

		case FT_HUNTER_LIST : {
								if( !SubMenu->CheakType )
								{
									SubMenu->CheakType = 1;
									SAFE_DELETE(hunter_list);
									hunter_list = new CHunterList;
									curr_hunter_list = hunter_list;
									SendGetHunterList( 0 );
								}

								if( curr_hunter_list )
								{
									if( curr_hunter_list->print( SubMenu->x, SubMenu->y ) == -1 ) 
									{
									//	CHunterList_Prev( curr_hunter_list );
										//¸Ç¸¶Áö¸·À» °æ¿ìÀÇ Ã³¸®
									}
								}
								break;
							  }

		case FT_HUNTER_LIST_SCROLL:	{
										int willdo = SubMenu->nField[j].nWillDo;
										if( willdo )		// next
										{
											if( curr_hunter_list->is_end )
											{
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
												break;
											}
										}
										else
										{
											if( curr_hunter_list == hunter_list )
											{
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
												break;
											}
										}
									}
		case FT_DEFAULT_BUTTON: {
									if(SubMenu->nField[j].fLButtonDown)
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
									else if(SubMenu->nField[j].fRectMouse)
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
									}
									break;
								}
		case FT_NATION_BBS : {
								if( !SCharacterData.nation ) 
								{
									SubMenu->bActive = false;
									SmallMenuClose();
									return;
								}
								int max_line = 8;
								if( nation_bbs2[0][0][0] == NULL )
								{
									switch( SCharacterData.nation )
									{
										case N_VYSEUS :			GetNationBBS( "./data/nation_b_bbs.txt" ); break;
										case N_ZYPERN :			GetNationBBS( "./data/nation_z_bbs.txt" ); break;
										case N_YILSE :			GetNationBBS( "./data/nation_y_bbs.txt" ); break;
										case N_HEGEMONIA :		GetNationBBS( "./data/nation_h_bbs.txt" ); break;
										default :
												SubMenu->bActive = false;
												SmallMenuClose();
												return;
									}
									SubMenu->key = 7;//GetLineRectTextPut( SubMenu->nField[j].nRectImage, nation_bbs ) / max_line + 1;
								}
								int img_num[] = {0, 0, 0, 9, 10, 11, 12};
								FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, img_num[SCharacterData.nation] , SubMenu->nField[j].nImageType);
								Hcolor( 0 );
								SetHangulAlign( TA_CENTER );
								for( int i=0; i<8; i++ )
								{
									Hprint2( SubMenu->x+225, SubMenu->y+SubMenu->nField[j].y+35+i*18, g_DestBackBuf, "%s", nation_bbs2[SubMenu->work][i] ); 
								}
								//BbsTextPut2( SubMenu->x+225, SubMenu->y+SubMenu->nField[j].y+35, SubMenu->nField[j].nRectImage, max_line, nation_bbs, SubMenu->work );
								//BbsTextPut2( SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+35, SubMenu->nField[j].nRectImage, max_line, nation_bbs, SubMenu->work );
								break;
							 }
		case FT_NATION_BUTTON : {
									//int block = 1;
									if( !SubMenu->CheakType )
									{
										MP3( SN_OPEN_NATION_BBS );
										SubMenu->CheakType = 1;
									}
									int willdo = SubMenu->nField[j].nWillDo;
									if( (SubMenu->work == 0 && willdo == 1 ) ||
										(SubMenu->work+1 == SubMenu->key && willdo == 2 ) ||
										(SubMenu->work+1 != SubMenu->key && willdo == 0 ) ) break;


									if( willdo == 2 ) // next button
									{
										int &delay = SubMenu->nField[j].nSHideNomalCount;
										if( (delay%12) < 6 ) FieldTypeNomalPut(SubMenu->x, SubMenu->y, 385, 180, 14);	// È­»ìÇ¥ ±ô¹ÚÀÌ±â
										delay++;
									}

									if(SubMenu->nField[j].fLButtonDown)
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
									else if(SubMenu->nField[j].fRectMouse)
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
									break;
								}		
		case FT_FIGHT_MAP_SIGN :{
//									SubMenu->key--;
									if( SubMenu->key <= 0 )
									{
										SubMenu->key= 0;
										MenuSoundClose( SubMenu->Id );
										return;
									}
									break;
								}

		case FT_DISPLAY_SCENARIO : {
										int is_start = 0;
										if( !SubMenu->CheakType )
										{
											is_start = 1;
											SubMenu->CheakType = 1;
										}

										if( !scenario_spr.img ) 
											LoadSpriteOnlyOne( &scenario_spr, "./data/nation_map.dat" );

										PutCompressedImage( scenario_spr.ox,scenario_spr.oy, &scenario_spr );

										int &delay = SubMenu->nField[j].nSHideNomalCount;
										int &curr_line = SubMenu->work;
										static char scenario_text[10001];
										static int line_max;
										int width = 400;
										if( scenario_text[0] == NULL )
										{
											LoadTextDefault( "./data/scenario.txt", scenario_text, 10000 );
											line_max = GetLineRectTextPut( width, scenario_text );
										}
										int max_line_to_page = 20;		// ÇÑÆäÀÌÁö´ç Ãâ·Â°¡´ÉÇÑ ¶óÀÎ

										int y;
										static int old_y;
										int skip = curr_line - max_line_to_page;
										
										//Hcolor( RGB16(240, 160, 66) );
										Hcolor( RGB16(255, 255, 255) );
										y=SubMenu->y+47-skip*18;
										if( is_start ) old_y = y;
										if( old_y != y ) 
										{
											static bool turn =false;
											if( turn ) old_y--;
											turn = !turn;
										}
										if( skip > 0 )
												SkipRectTextPut( SubMenu->x+200, old_y+skip*18 , width, skip, scenario_text, max_line_to_page, 0 );
										else	SkipRectTextPut( SubMenu->x+200, old_y , width, skip, scenario_text, curr_line?curr_line:1, 0 );


										if( line_max - curr_line ) 
										{
											delay ++;
											if( (delay%36) == 0 ) curr_line++;
										}

										
										static int  old_rbutton = RButtonDown;
										static int  old_button = LButtonDown;
										if( (g_aCurrentKeys[DIK_ESCAPE] & 0x80) || /*(old_button == 1 && LButtonDown == 0) || */(old_rbutton == 1 && RButtonDown == 0 ) )
										{
											old_button = 0;
											old_rbutton = 0;
											SmallMenuClose();
											return;
										}
										old_button = LButtonDown;
										old_rbutton = RButtonDown;
										break;
								   }
		case FT_VOTE_REGIST :{
								FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
								FieldTypeNomalPut(SubMenu->x, SubMenu->y, 9, 222, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
								FieldTypeNomalPut(SubMenu->x, SubMenu->y, 208, 222, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
								FieldTypeNomalPut(SubMenu->x, SubMenu->y, 52, 223, SubMenu->nField[j].nWillDo, SubMenu->nField[j].nImageType);

								RectTextPut( SubMenu->x+30, SubMenu->y+60, 200, lan->OutputMessage(7,88) );//010215 lsw
								break;
							 }
		case FT_VOTE_SUPPORT : {
								int work = SubMenu->work;
								FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);

								if( Candidate[work].name[0] )
								{
									PutCharImage( SubMenu->x+39, SubMenu->y+86, Candidate[work].face, 0, FS_BIG, 0, Candidate[work].gender);

									Hcolor( FONT_COLOR_SPECIAL );
									Hprint2( SubMenu->x+148, SubMenu->y+80, g_DestBackBuf, "%d. %s", work+1, NationName[Candidate[work].nation] );

									//PutLvNameByColor( SubMenu->x+148, SubMenu->y+116, Candidate[work].lv, Candidate[work].class_type );

									// ±æµå Ãâ·Â
									Hcolor( FONT_COLOR_NUMBER );
									Hprint2( SubMenu->x+148, SubMenu->y+132, g_DestBackBuf, "%s", g_GuildMgr[Candidate[work].guild_code].m_szName); // CSD-030324

									SetHangulAlign( TA_CENTER );
									Hcolor( FONT_COLOR_NAME );
									Hprint2( SubMenu->x+180, SubMenu->y+98, g_DestBackBuf, "%s", Candidate[work].name );
								}

								/////////////////////////
								// ½ºÅ©·Ñ ¹öÆ° Ã³¸®								
								if( work == 0 ) SubMenu->nField[3].nType = FT_NOMAL_PUT;
								else SubMenu->nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;

								if( work >= g_Candidate_max-1 ) SubMenu->nField[4].nType = FT_NOMAL_PUT;
								else SubMenu->nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;

								break;
							   }

		case FT_VOTE_SCORE_PUT:{
								g_VoteSignExist--;
								int willdo = SubMenu->nField[j].nWillDo;
								if( willdo < 0 || willdo >= g_Candidate_max ) break;

								SetHangulAlign( TA_CENTER );

								if( Candidate[willdo].name[0] )
								{
									Hcolor( FONT_COLOR_NAME );
									Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y-18, g_DestBackBuf, "%s", Candidate[willdo].name );
									
									Hcolor( FONT_COLOR_NUMBER );
									Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+5, g_DestBackBuf, "%d", Candidate[willdo].score );
								}
								if( g_aCurrentKeys[DIK_ESCAPE] & 0x80 ) g_VoteSignExist = -1;	// ´ÜÃàÅ°·Î ²ô±â

								break;
							   }
		case FT_VIEW_TIP : {
							int &count = SubMenu->nField[j].nSHideNomalCount;
							int delay[17] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 10 };
							//int delay[17] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 10, 10 };

							static int  d = delay[count];
							if( d <= 0 ) 
							{
								count++;
								count %= 17;
								d = delay[count];
							}
							d--;
							FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, count, HELP_PCX );
							
							break;
						   }
			/*
		case FT_TEMBATLE_INFO : {
									int willdo = SubMenu->nField[j].nWillDo;
									int max = g_BattleZone.GetCount( willdo );
									if( !max ) break;
									char *name;
									int count=0;
									for( int i=0; i<MAX_TEAM_MEMBER; i++ )
									{
										name = g_BattleZone.m_team[willdo].member[i].name;
										if( name[0] )
										{
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, name );
											count++;
										}
									}
									break;
								}*/
		case FT_BANK_LOAN : {
								if( !g_RecvLoanInfo ) return;
								break;
							}
		case FT_DELAY_BUTTON : {
									SubMenu->work++;		// ¸Þ´º°¡ Ã³À½ ¶ã¶§ 0À¸·Î ÃÊ±âÈ­ µÇ¾î ÀÖ´Ù´Â Á¡À» ÀÌ¿ëÇÑ´Ù.
									if( SubMenu->work < SubMenu->nField[j].nWillDo ) return;
									break;
							   }
		case FT_SALVATION_MONEY_SCROLL : {
											if(SubMenu->nField[j].fLButtonDown)
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+2, SubMenu->nField[j].y+2, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
											else if(SubMenu->nField[j].fRectMouse)
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+2, SubMenu->nField[j].y+2, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
											break;
										 }
		case FT_CHANGE_MONEY : 
			{
				if( !SubMenu->CheakType )
				{
					SubMenu->CheakType = 1;
					nBankTemp = 0;
					SetChatMode(CM_COMMAND);
					SetFocus2( HWND_3 );//021001 lsw
					CallServer( CMD_REQ_SALVATION_MONEY );
					TotalSalvation = 0;
				}
				//021001 lsw
				char temp[20] = {0,};
				SetFocus2(HWND_3);
				EWndMgr.GetTxt( HWND_3, temp,20 );
				EatRearWhiteChar( temp );
				nBankTemp = atol( temp );
				int refresh = 0;
				if( nBankTemp<0)
				{
					nBankTemp = 0;
					refresh = 1;
				}
				DWORD old_money = GetMoneyByItem( );
				if( nBankTemp>old_money )
				{
					nBankTemp = old_money;
					refresh = 1;
				}

				if( refresh || CheckNumber( temp, strlen( temp ) ) )
				{	
					sprintf( temp, "%d", nBankTemp );
					EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
				}
				PrintMoney( SubMenu, j, nBankTemp, SubMenu->nField[j].nRectImage );
				PrintMoney( SubMenu, j+1, nBankTemp*1, SubMenu->nField[j].nRectImage );//020730 lsw µ·Àº 1:1 ±³È¯
		   }break;
		case FT_CHECK_SALVATION :
			{
				if( !SubMenu->CheakType )
				{
					nBankTemp = 0;
					SetChatMode(CM_COMMAND);
					SetFocus2( HWND_3 );//021001 lsw
					CallServer( CMD_REQ_SALVATION_MONEY );
					SendDirectDB( CMD_CHECK_SALVATION_NAME, SCharacterData.sCharacterName, strlen(SCharacterData.sCharacterName) );
					char nation = Hero->name_status.nation;
					CallServer( CMD_GET_NATION_MONEY, &nation, sizeof( char ) );
					TotalSalvation = 0;
					SubMenu->CheakType = 1;

					if( !g_Menu_Variable.m_stNationMoney )
						g_Menu_Variable.m_stNationMoney = new k_view_nation_money;
				}

				int x = SubMenu->x+SubMenu->nField[j].x;
				int y = SubMenu->y+SubMenu->nField[j].y;
				if( !g_Menu_Variable.bCheckSalvation )
				{
					Hprint2( x, y, NULL, kein_GetMenuString(83) );
					return;
				}
				else
				{
					DWORD money = GetNationMoney(Hero->name_status.nation);
					Hprint2( x, y, NULL, kein_GetMenuString(81), money, g_szMoneyName );
				}
				break;
			}

		case FT_SALVATION_MONEY : 
			{
				char temp[20]= {0,};
				SetFocus2(HWND_3);
				EWndMgr.GetTxt( HWND_3, temp,20 );
				EatRearWhiteChar( temp );
				nBankTemp = atol( temp );
				int refresh = 0;
				if( nBankTemp<0)
				{
					nBankTemp = 0;
					refresh = 1;
				}
				if( nBankTemp>SCharacterData.nMoney )
				{
					nBankTemp = SCharacterData.nMoney;
					refresh = 1;
				}

				if( refresh || CheckNumber( temp, strlen( temp ) ) )
				{	
					sprintf( temp, "%d", nBankTemp );
					EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
				}
				PrintMoney( SubMenu, j, nBankTemp, SubMenu->nField[j].nRectImage );
			}break;
		case FT_TEXT_BUTTON_EXIT : 
			{
				char *text = SubMenu->nField[j].temp;
				RECT *rt = &SubMenu->nField[j].rCheakBox;
				int x1 = SubMenu->x+rt->left;
				int y1 = SubMenu->y+rt->top;
				int x2 = SubMenu->x+rt->right;
				int y2 = SubMenu->y+rt->bottom;

				SetHangulAlign( TA_CENTER );
				int center = rt->left+(rt->right - rt->left) / 2;

				if( SubMenu->nField[j].fRectMouse )
				{
					Box( x1+1, y1+1, x2-1, y2-1, RGB16( 250, 250,250 ));
					Box( x1, y1, x2, y2,   RGB16( 200, 200,200 ));
					HprintBold( SubMenu->x+center, SubMenu->y+rt->top+3, RGB( 0x00, 0xff, 0xff),  RGB( 0x3f, 0x3f, 0x3f), text );
				}
				else
				{
					Box( x1+1, y1+1, x2-1, y2-1, RGB16( 150, 150,150 ));
					Box( x1, y1, x2, y2,   RGB16( 100, 100,100 ));
					HprintBold( SubMenu->x+center, SubMenu->y+rt->top+3, RGB( 0xff, 0xff, 0xff),  RGB( 0x7f, 0x7f, 0x7f), text );
				}
		   }break;
		case FT_ANIMATION : {
								int &count = SubMenu->nField[j].nSHideNomalCount;
								int delay_max = SubMenu->nField[j].nSHideNomalStart;
								static int delay = delay_max;
								delay--;
								Spr *ani = GetSprOfMenu( SubMenu->nField[j].nImageType, count );
								if( !ani ) break;
								if( ani->img )
								{
									PutCompressedImage( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, ani );
								}
								else
								{
									count = 0;
									ani = GetSprOfMenu( SubMenu->nField[j].nImageType, count );
									if( ani )
									PutCompressedImage( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, ani );
								}
									
								delay--;
								if( delay<= 0 ) 
								{
									count++;
									delay = delay_max;
								}

								break;
							}
		case FT_NORMAL_PUT_CENTER2 : {
										Spr *s = (Spr *)SubMenu->nField[j].m_lpTemp;
										if( s )
										{
											PutCompressedImage( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, s );
										}
										break;
									 }
		case FT_SCROLL_TEXT_PUT : {
									int &page_status = SubMenu->work;
									int line = SubMenu->key;
									WORD page;
									WORD page_max;
									GetWORDOfDWORD( (DWORD)page_status, page, page_max );

									int nWidth = SubMenu->nField[j].nRectImage;
									char *lpszText = (char*)SubMenu->nField[j].m_lpTemp;

									if( !lpszText ) 
									{
										if( !LoadDefaultStringByMenu( SubMenu->Id, lpszText ) ) 
										{
											if( SubMenu->nField[j].nSpecialWillDo )	SubMenu->CheakType = 1;
											break;
										}
										SubMenu->nField[j].m_lpTemp = lpszText;
									}

									if( !SubMenu->CheakType )
									{
										page = 0;
										int line_max = GetLineRectTextPut( nWidth, lpszText );
										if( line )
										{
											page_max = (line_max-1) / line + 1;
										}
										else page_max = 1;
										page_status = (int )MAKELONG( page, page_max );
										if( SubMenu->nField[j].nSpecialWillDo )		
										{
											// ÃÊ±âÈ­ ·çÆ¾À» ¾Èµé¾î¿À°Ô ¼³Á¤ÇÑ´Ù.
											SubMenu->CheakType = 1;
										}
									}

									//Hcolor( FONT_COLOR_DEFAULT );
									int willdo = SubMenu->nField[j].nWillDo;
									Hcolor( willdo );
									SkipRectTextPut( SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,nWidth, page*line, lpszText, line ); 
									break;
								  }
		case FT_SCROLL_BUTTON2 :
			{
				int type = SubMenu->nField[j].nWillDo;
				int field = SubMenu->nField[j].nSpecialWillDo;
				int &page = SubMenu->nField[field].nSHideNomalStart;
				int &page_max = SubMenu->nField[field].nSHideNomalCount;

				if( type == SCROLL_UP )
				{
					if( page > 0 )	// À¯È¿ÇÏ´Ù.
					{
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, SubMenu->nField[j].nImageType);
						if(SubMenu->nField[j].fLButtonDown)
							FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].y+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
						else if(SubMenu->nField[j].fRectMouse)
							FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].y+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
					}
				}
				else 
				{
					if( page < page_max-1 )	// À¯È¿ÇÏ´Ù.
					{
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, SubMenu->nField[j].nImageType);
						if(SubMenu->nField[j].fLButtonDown)
							FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].y+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
						else if(SubMenu->nField[j].fRectMouse)
							FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].y+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
					}
				}
				break;
			}
		case FT_SCROLL_BUTTON : {
									int type = SubMenu->nField[j].nWillDo;
									int page_status = SubMenu->work;

									WORD page;
									WORD page_max;
									GetWORDOfDWORD( (DWORD)page_status, page, page_max );

									if( type == SCROLL_UP )
									{
										if( page > 0 )	// À¯È¿ÇÏ´Ù.
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, SubMenu->nField[j].nImageType);
											if(SubMenu->nField[j].fLButtonDown)
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].y+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
											else if(SubMenu->nField[j].fRectMouse)
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].y+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
										}
									}
									else 
									{
										if( page < page_max-1 )	// À¯È¿ÇÏ´Ù.
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nSHideNomalCount, SubMenu->nField[j].nImageType);
											if(SubMenu->nField[j].fLButtonDown)
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].y+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
											else if(SubMenu->nField[j].fRectMouse)
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].y+SubMenu->nField[j].nShideNomalPlus, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
										}
									}
									break;
								}

		case FT_TEXT_PUT :    //¿µ¾î ¹®ÀÚ¿­ Ãâ·Â
							 FieldTypeTextPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].temp);
							 break;

				 //ACER7
		case FT_TEXT_PUT_CENTER :   //¹®ÀÚ¿­À» Áß°£ºÎºÐ¿¡ Ãâ·Â
			{
				int not_english = 0;
				char *str = SubMenu->nField[j].temp;
				for( int a=0; a<strlen(str); a++ )
				{
					if( str[a] != ' ' && ( str[a] > 'Z' || str[a]<'A' ) )
					{
						not_english = 1;
						break;
					}	
				}
				if( not_english )
				{
					SetHangulAlign( TA_CENTER );
					Hprint2( SubMenu->x+spr[SubMenu->nImageNumber].xl/2, SubMenu->y+SubMenu->nField[j].y, NULL, "%s", str );
					break;
				}

				 i=0;
				 LenghtTotal=0;
			 			 
				 //¹®ÀÚ¿­ ÀüÃ¼ ±æÀÌ¸¦ °è»ê
				 while( (SubMenu->nField[j].temp[i]) != '\0' )
				 {
					 code=SubMenu->nField[j].temp[i];
					 if(code==32) LenghtTotal+=4;
					 else
					 {
						 code-=27;
					     LenghtTotal+=spr[code].xl;
					 }
					 i++;
				 }

				 // Áß°£ºÎÅÍ ½ÃÀÛÇÏ±âÀ§ÇÑ ÁÂÇ¥ °è»ê
				 lenght=(spr[SubMenu->nImageNumber].xl-LenghtTotal)/2;
			
				 FieldTypeTextPut(SubMenu->x, SubMenu->y, lenght, SubMenu->nField[j].y, SubMenu->nField[j].temp);
		         break;
			}

		case FT_VISA :{
						;
						char nation = SubMenu->key;
						if( !SubMenu->CheakType )
						{
							SubMenu->CheakType = 1;
							SendReqVisa( nation );
							SubMenu->work = -1;
						}
						for( int i=0; i<4; i++ )
						{
							if( NationVisa[nation].item_no[i] )
							{
								int gab = i*41;
								PutItemIcon( SubMenu->x+SubMenu->nField[j].x+13, SubMenu->y+SubMenu->nField[j].y+14+gab, NationVisa[nation].item_no[i] );
								if( i == SubMenu->work )	// »ç±â À§ÇØ ¼±ÅÃÇÑ ¾ÆÀÌÄÜ
								{
									PutItemIcon( SubMenu->x+SubMenu->nField[j].x+13, SubMenu->y+SubMenu->nField[j].y+14+gab, NationVisa[nation].item_no[i], 8 );
									HprintBold( SubMenu->x+SubMenu->nField[j].x+50, SubMenu->y+SubMenu->nField[j].y+gab, RGB( 200, 250, 200), RGB( 0,0,0 ), GetItemName( NationVisa[nation].item_no[i] ) );
									Hcolor( RGB16( 255, 255, 255 ) );
									Hprint2( SubMenu->x+SubMenu->nField[j].x+50, SubMenu->y+SubMenu->nField[j].y+18+gab, NULL, "%d %s", NationVisa[nation].item_price[i], g_szMoneyName );
									Hcolor( FONT_COLOR_NUMBER );
								}
								else
								{									
									HprintBold( SubMenu->x+SubMenu->nField[j].x+50, SubMenu->y+SubMenu->nField[j].y+gab, FONT_COLOR_NAME, RGB( 0,0,0 ), GetItemName( NationVisa[nation].item_no[i] ) );
									Hprint2( SubMenu->x+SubMenu->nField[j].x+50, SubMenu->y+SubMenu->nField[j].y+18+gab, NULL, "%d %s", NationVisa[nation].item_price[i], g_szMoneyName );
								}
							}
						}
						break;
					  }

		case FT_SMALL_TIPS_BUTTON : {
										if( !SubMenu->CheakType )
										{
											SubMenu->CheakType = 1;
											ViewSmallTipsLoad( SubMenu->key );
										}
										int nWillDo = SubMenu->nField[j].nWillDo;
										Spr *back_spr = GetSprOfMenu( MAIN_ETC, 55 );
										if( !back_spr ) return;
										
										int button_color1;
										int button_color2;

										if( SubMenu->nField[j].fLButtonDown || SubMenu->nField[j].fRectMouse )
										{
											Hcolor( RGB16( 255, 255, 0 ) );
											button_color1 = RGB16( 250, 250,250 );
											button_color2 = RGB16( 200, 200,200 );
										}
										else
										{
											Hcolor( RGB16( 255, 255,255 ) );
											button_color1 = RGB16( 150, 150,150 );
											button_color2 = RGB16( 100, 100,100 );
										}

										RECT r = SubMenu->nField[j].rCheakBox;
										switch( nWillDo )
										{
											case 0:
											case 1: 
											case 2: 
												{	
													Box( Mapx+SubMenu->x+r.left+1, Mapy+SubMenu->y+r.top+1, Mapx+SubMenu->x+r.right, Mapy+SubMenu->y+r.bottom, button_color1 );
													Box( Mapx+SubMenu->x+r.left, Mapy+SubMenu->y+r.top, Mapx+SubMenu->x+r.right+1, Mapy+SubMenu->y+r.bottom+1, button_color2 );
													SetHangulAlign( TA_CENTER );
													Hprint2( SubMenu->x+r.left+( r.right - r.left )/2, SubMenu->y+r.top+5, g_DestBackBuf, "%s", SubMenu->nField[j].temp );
													break;
												}
											case 3:	ViewSmallTip( ); break;
										}
										break;
									}
		case FT_VIEW_EFFECT_TOOL : {
										if( !SubMenu->CheakType )
										{
											SubMenu->CheakType = 1;
					 						SetFocus2( HWND_3 );//021001 lsw
										}

										int nWillDo = SubMenu->nField[j].nWillDo;
										static Spr *back_spr = GetSprOfMenu( MAIN_ETC, 55 );
										RECT &r = SubMenu->nField[j].rCheakBox;

										int button_color1;
										int button_color2;

										if( SubMenu->nField[j].fLButtonDown || SubMenu->nField[j].fRectMouse )
										{
											Hcolor( RGB16( 255, 255, 0 ) );
											button_color1 = RGB16( 250, 250,250 );
											button_color2 = RGB16( 200, 200,200 );
										}
										else
										{
											Hcolor( RGB16( 255, 255,255 ) );
											button_color1 = RGB16( 150, 150,150 );
											button_color2 = RGB16( 100, 100,100 );
										}

										switch( nWillDo )
										{
										case 0 :
											PutCompressedImageFX( SubMenu->x+back_spr->ox, SubMenu->y+back_spr->oy, back_spr, 20, 1 );
											PutCompressedImageFX( SubMenu->x+back_spr->ox, SubMenu->y+back_spr->oy+back_spr->yl, back_spr, 20, 1 );
											SetFocus2(HWND_3);
											EWndMgr.GetTxt( HWND_3, SubMenu->nField[0].temp,20 );//021001 lsw
											SubMenu->work = atoi( SubMenu->nField[0].temp );
											sprintf( SubMenu->nField[0].temp, "%d", SubMenu->work );
											EWndMgr.SetTxt(HWND_3,SubMenu->nField[0].temp);//021001 lsw
											Hprint2( SubMenu->x+5, SubMenu->y+3, g_DestBackBuf, "ÀÔ·Â°ª : %d", SubMenu->work );
											Hprint2( SubMenu->x+5, SubMenu->y+3+back_spr->yl, g_DestBackBuf, "ÇöÀç°ª : %d", SubMenu->key);
											break;
										case 1:
											Box( Mapx+SubMenu->x+r.left+1, Mapy+SubMenu->y+r.top+1, Mapx+SubMenu->x+r.right, Mapy+SubMenu->y+r.bottom, button_color1 );
											Box( Mapx+SubMenu->x+r.left, Mapy+SubMenu->y+r.top, Mapx+SubMenu->x+r.right+1, Mapy+SubMenu->y+r.bottom+1, button_color2 );
											SetHangulAlign( TA_CENTER );
											Hprint2( SubMenu->x+r.left+( r.right - r.left )/2, SubMenu->y+r.top+5, g_DestBackBuf, "%s", SubMenu->nField[j].temp );
											break;
										}

										break;
								   }
		case FT_VIEW_LADDER_RANK : {//021001 lsw
										int &page = SubMenu->nField[j].nSHideNomalStart;
										if( !SubMenu->CheakType )
										{
											SubMenu->CheakType = 1;
											g_bSendLadderRank = 0;
											g_szSearchLadderRankName[0] = NULL;
											page = 0;
										}
										static int count = 0;
										char *temp = SubMenu->nField[j].temp;
										SetFocus2( HWND_3 );//021001 lsw
										EWndMgr.GetTxt( HWND_3, temp,17 );
										if(count%12 < 5) 
										{
											sprintf( temp, "%s_", temp );
										}
										Hprint2(SubMenu->x+176, SubMenu->y+340, g_DestBackBuf, temp );
										count++;

										int &page_max = SubMenu->nField[j].nSHideNomalCount;
										k_get_ladder_score_rank_unit *rank_data;
										int is_end;//021030 lsw
										
										int ret = g_lpRankLadderBBS->GetRankLadderData( page+1, rank_data, is_end );
										if( ret == 1 )
										{
											if( is_end ) page_max = page+1;
											else page_max = page+2;

											for( int i=0; i<10; i++ )
											{
												if( rank_data[i].rank )
												{
													Hcolor( FONT_COLOR_NUMBER );

													Hprint2( SubMenu->x+19, SubMenu->y+82+i*25, g_DestBackBuf, "%d", rank_data[i].rank );			//19, 82, 62
													
													Hprint2( SubMenu->x+172, SubMenu->y+82+i*25, g_DestBackBuf, "%s", NationName[rank_data[i].nation]);	//172, 82, 64
													Hprint2( SubMenu->x+240, SubMenu->y+82+i*25, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][rank_data[i].cls].temp );			//240, 82, 53
													//PutLvNameByColor( SubMenu->x+298, SubMenu->y+82+i*25, rank_data[i].lv, rank_data[i].cls );
													Hprint2( SubMenu->x+391, SubMenu->y+82+i*25, g_DestBackBuf, "%u", rank_data[i].score );			//391, 82, 45

													if( strcmp( rank_data[i].name, g_szSearchLadderRankName ) == 0 )
													{
														Hcolor( FONT_COLOR_BLUE );
													} 
													else if( strcmp( rank_data[i].name, SCharacterData.sCharacterName ) == 0 )
													{
														Hcolor( FONT_COLOR_NAME );
													}
													Hprint2( SubMenu->x+66, SubMenu->y+82+i*25, g_DestBackBuf, "%s", rank_data[i].name );					//66, 82, 100													
												}
											}
										}
										else
										{
											SendReqLadderRank( page );
											page_max = page+1;				// Àü¼ÛÇÑ °ªÀº ¹ÞÀº ÈÄ¿¡ ¶Ç ¿äÃ»ÇÒ ¼ö ÀÖ°Ô Ã³¸®ÇÑ´Ù.
										}
										break;
								   }
		case FT_VIEW_TEXT_BY_HTML : 
			{
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)SubMenu->work, page, page_max );
				int line_max = SubMenu->key2;
				int r, g, b;
				Getcolor( &r,&g,&b );
				if( !g_Menu_Variable.m_pGuildRegistExplain ) break;
				int ct = 0;
				for( int a = line_max*page; (a<line_max*(page+1))&&(a<SubMenu->key); a++, ct++ )
				{
					HprintBoldcolor( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+ct*16, 
						100, g_Menu_Variable.m_pGuildRegistExplain->m_pString[a] );
				}
				//Hcolor( r,g,b );		// ¿ø·¡·Î µÇµ¹¸®±â
				break;
			}
		case FT_GUILD_DEGREE : {
									if( !SubMenu->CheakType )
									{
										SubMenu->CheakType = 1;
										SetFocus2( HWND_3 );//021001 lsw

										if( !g_Menu_Variable.m_pGuildDegree )
											g_Menu_Variable.m_pGuildDegree = new k_guild_degree;

										memset( g_Menu_Variable.m_pGuildDegree, 0, sizeof( k_guild_degree ));
										int code = Hero->GetGuildCode(); // CSD-030324
										if( code )
										{
											strcpy( g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[0], CallGuildDegreeName( code, 1 ) );
											strcpy( g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[1], CallGuildDegreeName( code, 2 ) );
											strcpy( g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[2], CallGuildDegreeName( code, 3 ) );
											strcpy( g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[3], CallGuildDegreeName( code, 4 ) );
											strcpy( g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[4], CallGuildDegreeName( code, 0 ) );

											EWndMgr.SetTxt(HWND_3,CallGuildDegreeName( code, 1 ));//021001 lsw
										}
									}
									int willdo = SubMenu->nField[j].nWillDo;
									if( willdo == SubMenu->work )
									{
										int &count = SubMenu->nField[j].nSHideNomalCount;
										SetFocus2(HWND_3);
										//021030 YGI
										EWndMgr.GetTxt( HWND_3, g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[willdo],19 );
										if( ( (count++)%12 < 5 ) ) 
										{
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%s_", g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[willdo] );
										}
										else
										{
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%s", g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[willdo] );
										}
									}
									else
										Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%s", g_Menu_Variable.m_pGuildDegree->m_pszGuildDegree[willdo] );

									break;
							   }
		case FT_INPUT_GUILD_MASTER ://021001 lsw
			{
				if( !SubMenu->CheakType )
				{
					SubMenu->CheakType = 1;
					nBankTemp = 0;
					SetChatMode(CM_COMMAND);
					SetFocus2( HWND_3 );//021001 lsw
				}
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );// °ËÀº ÆÇµ¥±â Âï¾îÁÖ±â

				int &nCount = SubMenu->nField[j].nSHideNomalCount;
				
				char temp[20] = {0,};// ÀÔ·Â ¹Þ±â
				EWndMgr.GetTxt( HWND_3, temp,20 );//021121 lsw
				EatRearWhiteChar( temp );
				if( nCount % 16 < 8 ) 
				{
					Hprint2( SubMenu->x+SubMenu->nField[j].rCheakBox.left+5, SubMenu->y+SubMenu->nField[j].y+5, NULL, temp );
				}
				else
				{
					Hprint2( SubMenu->x+SubMenu->nField[j].rCheakBox.left+5, SubMenu->y+SubMenu->nField[j].y+5, NULL, "%s_", temp );
				}
				nCount++;
			}break;
		case FT_POTION_BOX :
			{
//				if( SMenu[MN_TIME_DISPLAY_MENU].bActive ) break;
				int item_no, much;
				GetItemByPotionBox( item_no, much );
				if( item_no && much )
				{
				//	PutItemIcon//Ani( SubMenu->x+SubMenu->nField[j].x-60, SubMenu->y+SubMenu->nField[j].y, item_no );
					PutItemIcon( SubMenu->x+SubMenu->nField[j].x-60, SubMenu->y+SubMenu->nField[j].y, item_no );//020420 lsw
					Hprint2( SubMenu->x+SubMenu->nField[j].x-50, SubMenu->y+SubMenu->nField[j].y,NULL, "%d", much );
				}

				break;
			}
		case FT_ROULETTE :
			{
				int count = 0;
				if( !SubMenu->key ) break;

				int &start_count = SubMenu->nField[j].nSpecialWillDo;
				if( SubMenu->key == 1 )
				{
					count= 1;
					start_count -- ;
					if( start_count == 0 )
					{
						CallServer( CMD_GAMBLE_START );
						start_count = -1;
					}
				}
				else if( SubMenu->key == 100 )
				{
					count = 5;
					start_count = 100;
				}
				else if( SubMenu->key == 200 ) count = 1;

				static int ing = 0;
				int &img = SubMenu->work;// ·é ¹àÀº ÀÌÀÌ¹ÌÁö
				int &end_count = SubMenu->nField[j].nWillDo;

				int xy[][2] = { { 136, 109}, {213, 142}, {240, 211}, {213, 283}, {141, 315}, {66, 282}, {37, 213}, {61, 142} };
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, xy[img][0], xy[img][1], img+1, SubMenu->nField[j].nImageType);

				ing++;
				if( ing >= count ) 
				{
					ing = 0;
					if( ( SubMenu->key != 100 && SubMenu->key != 200 ) || g_Menu_Variable.m_GambleRune != img || g_Menu_Variable.m_GambleRand > 0 )		// ¿Ï·á
					{
						img++;
						MP3(1254 );
						if( img >= 8 ) img = 0;
					}
					else
					{
						SubMenu->key = 200;
						if( end_count == 10 )
						{
							// effect;
							int &img = SubMenu->nField[j].nSHideNomalStart;
							Spr *s = GetSprOfMenu( GAMBLE_EFFECT, img/2 );
							if( !s || !s->img )
							{
								// ¼­¹ö·Î 
								SendGameItem( make_item_list );
								SubMenu->key = 0;
								SubMenu->work = 0;
								//SmallMenuClose();
								//SMenu[MN_GAMBLE].bActive = false;

								for( int i=0; i<20; i++ )		// °ø±¸Ã¢¿¡ ÀÖ´Â Àç·á´Â ¸ðµÎ »ç¶óÁø´Ù.
								{
									if( make_item_list[i].item_no ) 
									{
										DeleteItem( make_item_list[i].pos );
										memset( &make_item_list[i], 0, sizeof( MakeItem ) );
									}
									else break;
								}
								img = 0;
							}
							else 
							{
								PutCompressedImageFX( SubMenu->x+156, SubMenu->y+237, s, 17, 2 );
								if( img == 2 ) MP3( 2118 );
								if( img == 24 ) MP3( 2211 );
							}

							img++;
						}
						else end_count ++;
					}					
					if( SubMenu->key == 100 )
					{
						g_Menu_Variable.m_GambleRand -- ;
					}
				}
				break;
			}
		case FT_GUILD_MEMBER_LIST_DEGREE_TAB :
			{
				int tab = SubMenu->key;

				int x = SubMenu->nField[j].x;
				int y = SubMenu->nField[j].y;

				static int tab_to_img[] = { 0, 10, 11, 12, 13 };
				if( tab )
					FieldTypeNomalPut( SubMenu->x, SubMenu->y, 0, 0, 10, IIT_GUILD_MEMBER_LIST );

				if( tab > 1 )
					FieldTypeNomalPut( SubMenu->x, SubMenu->y, x, y+1, tab_to_img[tab], IIT_GUILD_MEMBER_LIST );

				int guild_code = Hero->GetGuildCode(); // CSD-030324
				if( !guild_code ) break;

				// ±æµå ÀÌ¹ÌÁö Âï±â
				PutGuildImage( SubMenu->x+27+8, SubMenu->y+17+5, guild_code );
				// ±æµå¸í 
				Hcolor( RGB16( 134, 255, 0 ) );
				Hprint2( SubMenu->x+324, SubMenu->y+35, NULL, "%s", GetGuildName( guild_code ) );
				// Á÷Ã¥
				//Hcolor( RGB16( 0, 254, 247 ) );
				static int tab_to_degree[] = { 1, 2, 3, 4, 0 };
				
				if( !g_GuildMgr[guild_code].first )
				{	//< CSD-030324
					PutGuildLvByColor( SubMenu->x+41+20, SubMenu->y+144+10, guild_code, tab_to_degree[tab] );
					//Hprint2( SubMenu->x+41, SubMenu->y+144, NULL, CallGuildDegreeName( guild_code, tab_to_degree[tab] ) );
				}	//> CSD-030324

				if( !tab ) // ±æµå ¸¶½ºÅÍ °ü·Ã 
				{
					// guild_member_list ¿¡¼­ ¹Þ¾Æ¿Â ±æµå ¸¶½ºÅÍ
					const char *name = g_Menu_Variable.m_GuildMemberName.GetName( tab_to_degree[tab], 0 );
					// guild_list ¿¡¼­ ¹Þ¾Æ¿Â ±æµå ¸¶½ºÅÍ
					char *name2 = g_GuildMgr[guild_code].m_szGuldMasterName; // CSD-030324
					
					if( !name || strcmp( name, name2 ) != 0 ) // ±æµå ¸¶½ºÅÍ ÀÌ¸§ÀÌ ¾ø°Å³ª µÑÀÌ ´Ù¸£¸é
						Hcolor( FONT_COLOR_PLUS );						
					else if( g_Menu_Variable.m_GuildMemberName.GetIsConnect( tab_to_degree[tab], 0 ) )	// ±æµå ¸¶½ºÅÍ°¡ Á¢¼ÓÇØ ÀÖ´Ù¸é
						Hcolor( FONT_COLOR_NAME );		
					else Hcolor( FONT_COLOR_NUMBER );

					//Hcolor( FONT_COLOR_NAME );
					Hprint2( SubMenu->x+48, SubMenu->y+187, NULL, "%s",  name2 );
				}
				else		// ±æµå¿ø Âï±â
				{
					char &count = SubMenu->nField[j].temp[tab_to_degree[tab]*2];
					int page_status = SubMenu->work;
					WORD page;
					WORD page_max;
					GetWORDOfDWORD( (DWORD)page_status, page, page_max );

					count = page;
					int page_max_name = 27;
					Hcolor( FONT_COLOR_NUMBER );

					int degree, number;		// ¼­ºê ¸¶½ºÅÍÀÇ À§Ä¡
					g_Menu_Variable.m_GuildMemberName.GetSubMasterNumber( degree, number );

					for( int a=0; a<page_max_name; a++ )
					{
						int para = page*page_max_name+a;
						const char *name = g_Menu_Variable.m_GuildMemberName.GetName( tab_to_degree[tab], para );
						if( !name ) break;
						int is_connect = g_Menu_Variable.m_GuildMemberName.GetIsConnect( tab_to_degree[tab], para);

						if( is_connect )
							Hcolor( FONT_COLOR_NAME );
						else Hcolor( FONT_COLOR_NUMBER );

						if( degree != -1 )
						{
							if( degree == tab_to_degree[tab] && number == para )
								Hcolor( FONT_COLOR_BLUE );
						}

						Hprint2( SubMenu->x+37+(a%3*20*7), SubMenu->y+192+(a/3*15), NULL, "%s", name );

					}
				}
				break;
			}
		case FT_GAMBLE_TODAY	:
			{
				if( !g_Menu_Variable.m_TodayGambleItem ) break;
				ItemAttr *item = g_Menu_Variable.m_TodayGambleItem;
				if( !item ->item_no ) break;
				PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+15, item->item_no );

				if( SubMenu->nField[j].fRectMouse )
				{
					ExplainItem2( g_pointMouseX+200, g_pointMouseY, item );
				}
				
				Hcolor( FONT_COLOR_NAME );
				SetHangulFont(4);
				SetHangulAlign( TA_CENTER );
				Hprint2( SubMenu->x+185, SubMenu->y+71, NULL, "%s", GetItemName( item->item_no ) );
				SetHangulFont(2);

				break;
			}

		case FT_FRIEND_NAME :
			{
				if( !g_Menu_Variable.m_FriendGrup ) return; 

				int &page_status = SubMenu->work;
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)page_status, page, page_max);

				char *name;
				int login;
				int count = 0;
				int real_count = 0;
				static int max_line = 10;
				Hcolor( RGB16( 0, 0, 255 ));
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
						int x = SubMenu->x+SubMenu->nField[j].x;
						int y = SubMenu->y+SubMenu->nField[j].y+20*count;

						Hprint2( x ,y,  NULL, "%s", name );

						g_Menu_Variable.m_FriendGrup->SetRect( a, 1, x+123, y+1, x+123+13, y+1+11 );
						g_Menu_Variable.m_FriendGrup->SetRect( a, 2, x+143, y+1, x+143+13, y+1+11 );

						name = g_Menu_Variable.m_FriendGrup->CheckRect( a, 2, 0, 0 );
						if( name )
						{
							FieldTypeNomalPut( x+143, y, 1, 2, 3, IIT_FRIEND );		// ¸ÞÀÏ 
						}
						else
							FieldTypeNomalPut( x+143, y+1, 0, 0, 3, IIT_FRIEND );		// ¸ÞÀÏ 

						name = g_Menu_Variable.m_FriendGrup->CheckRect( a, 1, 0, 0 );
						if( name )
							FieldTypeNomalPut( x+123, y, 1, 2, 4, IIT_FRIEND );		// ±Ó¼Ó¸»
						else
							FieldTypeNomalPut( x+123, y+1, 0, 0, 4, IIT_FRIEND );		// ±Ó¼Ó¸»


						real_count++;
						count++;
					}
				}

				if( count > max_line ) break;

				FieldTypeNomalPut( SubMenu->x+SubMenu->nField[j].x-4, SubMenu->y+SubMenu->nField[j].y+20*count, 0, 0, 2, IIT_FRIEND );

				Hcolor( RGB16( 255, 0, 0 ));
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
						int x = SubMenu->x+SubMenu->nField[j].x;
						int y = SubMenu->y+SubMenu->nField[j].y+10+20*count;

						Hprint2( x, y, NULL, "%s", name );

						g_Menu_Variable.m_FriendGrup->SetRect( a, 1, 0, 0, 0, 0 );
						g_Menu_Variable.m_FriendGrup->SetRect( a, 2, x+143, y, x+143+13, y+11 );

						char *name = g_Menu_Variable.m_FriendGrup->CheckRect( a, 2, 0, 0 );
						if( name )
							FieldTypeNomalPut( x+143, y, 1, 1, 3, IIT_FRIEND );		// ¸ÞÀÏ 
						else
							FieldTypeNomalPut( x+143, y, 0, 0, 3, IIT_FRIEND );		// ¸ÞÀÏ 

						count++;
						real_count++;
					}
				}
				break;
			}

		case FT_POTAL_MAP : 
			{
				int max;
				int line = SubMenu->key;
				int &page_status = SubMenu->work;
				WORD page;
				WORD page_max;

				if( !g_Menu_Variable.m_PotalMap )
				{
					g_Menu_Variable.m_PotalMap = new CPotalMenu;
					// ÇØ´ç ¸ÊÀÇ ±¹°¡ ¸Þ´º¸¦ ¶ç¿î´Ù. Áß¸³ ¸ÊÀÏ °æ¿ì ÀÚ±â ³ª¶óÀÇ Æ÷Å» ¸Þ´º¸¦ ¶ç¿î´Ù.
					int nation = MapInfo[ MapNumber].nation;
					if( !nation ) nation = Hero->name_status.nation;
					g_Menu_Variable.m_PotalMap->LoadPotalMap( nation );
					max = g_Menu_Variable.m_PotalMap->GetMax();					

					GetWORDOfDWORD( (DWORD)page_status, page, page_max );
					
					page = 0;
				}
				else
				{
					max = g_Menu_Variable.m_PotalMap->GetMax();
					GetWORDOfDWORD( (DWORD)page_status, page, page_max );
				}
				page_max = max/(line+1)+1;
				page_status = MAKELONG( page, page_max );
				if( !max ) break;

				SetHangulAlign( TA_CENTER );
				Hprint2( SubMenu->x+79, SubMenu->y+58, NULL, "%s", kein_GetMenuString( 106 ) );
				Hprint2( SubMenu->x+155, SubMenu->y+58, NULL, "%s", kein_GetMenuString( 107 ) );
				Hprint2( SubMenu->x+200, SubMenu->y+58, NULL, "%s", kein_GetMenuString( 122 ) );
				Hprint2( SubMenu->x+258, SubMenu->y+58, NULL, "%s", kein_GetMenuString( 108 ) );

				int type, money;

				for( int a=0 ; a<line && a+line*page<max; a++ )
				{
					//map_number = g_Menu_Variable.m_PotalMap->GetMapNumber( a );
					type = g_Menu_Variable.m_PotalMap->GetType( a+line*page );
					money = g_Menu_Variable.m_PotalMap->GetMapMoney( a+line*page );

					Hprint2( SubMenu->x+79 , SubMenu->y+104+20*a, NULL, "%s", g_Menu_Variable.m_PotalMap->GetMapName( a+line*page ) );
					Hprint2( SubMenu->x+155, SubMenu->y+104+20*a, NULL, "%s", kein_GetMenuString( 109+type ) );
					Hprint2( SubMenu->x+200, SubMenu->y+104+20*a, NULL, "%d", g_Menu_Variable.m_PotalMap->GetMapLevel(a+line*page) );
					Hprint2( SubMenu->x+258, SubMenu->y+104+20*a, NULL, "%d %s", money, g_szMoneyName );
				}
				break;
			}
		case FT_POTAL_SELECT : 
			{
				if( !SubMenu->CheakType )
				{
					SubMenu->CheakType = 1;
					SubMenu->nTemp = -1;
				}

				int work = SubMenu->nTemp;
				int x = SubMenu->nField[j].x;
				int y = SubMenu->nField[j].y;

				int line = SubMenu->key;
				int &page_status = SubMenu->work;
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)page_status, page, page_max );
				int max = g_Menu_Variable.m_PotalMap->GetMax();

				static int old_page = page;
				if( page != old_page )
				{
					old_page = page;
					SubMenu->nTemp = -1;
				}

				if( work != -1 ) 
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, x,y+20*(work%line), SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);

				if( SubMenu->nField[j].fRectMouse )
				{
					int a = ( g_pointMouseY-SubMenu->y-y )/20;
					if( a<line && a+line*page<max )
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, x,y+20*a, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
				}

				break;
			}
		case FT_POTAL_SELECT_PUT: 
			{
				int &page_status = SubMenu->work;
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)page_status, page, page_max );
				int line = SubMenu->key;
				int n = SubMenu->nFieldCount;
				int para = SubMenu->nField[n].nWillDo;
				char *map_name = g_Menu_Variable.m_PotalMap->GetMapName( para );
				int map_num = g_Menu_Variable.m_PotalMap->GetMapNumber( para );
				int type = g_Menu_Variable.m_PotalMap->GetType( para );
				int money = g_Menu_Variable.m_PotalMap->GetMapMoney( para );
				char *explain = g_Menu_Variable.m_PotalMap->GetMapExplain( para );
				int level = g_Menu_Variable.m_PotalMap->GetMapLevel( para );
				int img_num =  SubMenu->nField[j].nImageNumber+g_Menu_Variable.m_PotalMap->GetImgType( para );

				if( !SubMenu->CheakType )
				{
					SubMenu->CheakType = 1;
					page = 0;
					int line2 = GetLineRectTextPut( 240, explain );
					page_max = line2/(line+1) +1;
					page_status = MAKELONG( page, page_max );
				}

				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x ,SubMenu->nField[j].y, img_num ,SubMenu->nField[j].nImageType);
				Hprint2( SubMenu->x+36, SubMenu->y+265, NULL, kein_GetMenuString( 116), money, g_szMoneyName );

				if( explain && strlen( explain ) ) 
				{
					SkipRectTextPut( SubMenu->x+36, SubMenu->y+155, 240, page*line, explain, line );
				}

				SetHangulAlign( TA_CENTER );
				Hprint2( SubMenu->x+197, SubMenu->y+81, NULL, "%s", map_name );
				Hprint2( SubMenu->x+197, SubMenu->y+101, NULL, kein_GetMenuString( 115), GetNationName( MapInfo[map_num].nation ) );
				Hprint2( SubMenu->x+197, SubMenu->y+121, NULL, kein_GetMenuString( 209), level );

				break;
			}
		case FT_GUILD_ITEM_GET : 
			{
				if( !g_Menu_Variable.m_pGuildItem ) break;

				int line = SubMenu->key;
				int &page_status = SubMenu->work;
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)page_status, page, page_max );

				int &old_page = g_Menu_Variable.m_pGuildItem->old_page;
				if( page > old_page ) 
				{
					old_page = page;
					SendDBForGuildItem( g_Menu_Variable.m_pGuildItem->m_pIndex[page*line] );
				}

				int max = g_Menu_Variable.m_pGuildItem->GetMax();
				ItemAttr *pItem;

				int base_x = SubMenu->x+SubMenu->nField[j].x;
				int base_y = SubMenu->y+SubMenu->nField[j].y;
				for( int a=0; a<line && a+page*line< max; a++ )
				{
					pItem = g_Menu_Variable.m_pGuildItem->GetItemByParm( a+page*line );
					if( !pItem || !pItem->item_no ) continue;
					PutItemIcon( base_x+15+(a%5)*35 , base_y+15+(a/5)*35, pItem->item_no );
				}

				if( SubMenu->nField[j].fRectMouse )
				{
					int first = (g_pointMouseX-base_x)/35;
					int second = (g_pointMouseY-base_y)/35;
					int para = second*5+first+page*line;

					pItem = g_Menu_Variable.m_pGuildItem->GetItemByParm( para );
					if( pItem && pItem->item_no )
					{
						ItemExplain( *pItem );
					}
				}
				break;
			}
		case FT_GUILD_ITEM_GET_ETC :
			{
				int guild_code = Hero->GetGuildCode(); // CSD-030324
				if( !guild_code ) break;

				PutGuildImage( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, guild_code );
				SetHangulAlign( TA_CENTER );
				Hprint2( SubMenu->x+130, SubMenu->y+62, NULL, "%s", GetGuildName( guild_code ) );

				if( !g_Menu_Variable.m_pGuildItem ) break;
				SetHangulAlign( TA_RIGHT );
				int money = g_Menu_Variable.m_pGuildItem->m_BoxMoney;
				if( money == -1 ) money = 0;
				Hprint2( SubMenu->x+157, SubMenu->y+82, NULL, "%u", money );
				break;
			}
		case FT_GUILD_ITEM_PUT:
			{
				if( !g_Menu_Variable.m_pPutGuildItem )
				{
					g_Menu_Variable.m_pPutGuildItem = new POS;
					memset( g_Menu_Variable.m_pPutGuildItem, 0, sizeof( POS ) );
				}

				if( g_Menu_Variable.m_pPutGuildItem->type == HAND ) break;
				ItemAttr *pItem = ::GetItemByPos( *g_Menu_Variable.m_pPutGuildItem );
				//if( !pItem ) break;
				if( !pItem->item_no ) break;
				static ItemAttr old_item;
				if( !EqualItem( &old_item, pItem ) )
				{
					old_item = *pItem;
					FieldTypeNomalPut( SubMenu->x, SubMenu->y, 19, 112, 6, IIT_GUILD_ITEM );
				}
				PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+15, pItem->item_no );
				break;
			}
		case FT_GUILD_ITEM_PUT_BLANK :
			{
				if( !g_Menu_Variable.m_pGuildItem ) break;
				int nWillDo = SubMenu->nField[j].nWillDo;
				Hcolor( nWillDo );
				SetHangulAlign( TA_RIGHT );
				sprintf( SubMenu->nField[j].temp, "(%d)", 150-g_Menu_Variable.m_pGuildItem->GetMax() );
				RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage, SubMenu->nField[j].temp ); 
				break;
			}
			// 020701 YGI
		case FT_SCENARIO_TRAP_INFO :
			{
				if( !SubMenu->CheakType )
				{
					CallServer( CMD_TRAP_INFO );
					SubMenu->CheakType = 1;
				}
				if( !g_Menu_Variable.m_pScenarioTrapInfo ) break;

				SetHangulAlign( TA_CENTER );
				Hprint2( SubMenu->x+311, SubMenu->y+67, NULL, "%d", g_Menu_Variable.m_pScenarioTrapInfo->m_nTotalTrap );
				Hprint2( SubMenu->x+311, SubMenu->y+122, NULL, "%d", g_Menu_Variable.m_pScenarioTrapInfo->m_nActiveTrap );
				int time = g_Menu_Variable.m_pScenarioTrapInfo->m_nWaitTimeOfDungeon ;
				// 031110 YGI
				if( time > 0 ) 
					Hprint2( SubMenu->x+311, SubMenu->y+172, NULL, "%d:%d", time/60, time%60 );
				Hprint2( SubMenu->x+311, SubMenu->y+225, NULL, "%d", g_Menu_Variable.m_pScenarioTrapInfo->m_nWaitTimeOfTrap );


				break;
			}
			// 020701 YGI
		case FT_SCENARIO_TRAP_KEY :
			{
				if( !SubMenu->CheakType )
				{
					CallServer( CMD_SCENARIO_TRAP_KEY );
					SubMenu->CheakType = 1;
				}

				k_scenario_trap_key *pData = g_Menu_Variable.m_pScenarioTrapKey;
				if( !pData ) break;

				static int itemxy[][2] = { { 49,155 }, {71,107}, {116,64}, {177,58}, {238,64}, {283,107}, {305,155} };

				int item_no = 0;
				int sum = 0;
				for( int i=0; i<7; i++ )
				{
					if( pData->qube[i] )
					{
						switch( pData->qube[i] )
						{
							case 3: item_no = 10210; break; 
							case 5: item_no = 10211; break; 
							case 7: item_no = 10212; break; 
						}
						PutItemIcon( SubMenu->x+itemxy[i][0]+17, SubMenu->y+itemxy[i][1]+17, item_no );		// 020707 YGI
						sum += pData->qube[i];
					}
					else break;
				}
				SetHangulAlign( TA_CENTER );
				Hprint2( SubMenu->x+148+46, SubMenu->y+155+10, NULL, "%d", sum );
				Hprint2( SubMenu->x+260+50, SubMenu->y+214+5, NULL, "%d", pData->sum);
				break;
			}

			case FT_DATA_PUT :		//¸Þ´ºµî¿¡ µ¥ÀÌÅÍ¸¦ Ãâ·ÂÇØ¾ß ÇÒ°æ¿ì
						flag=TRUE;			// ÇÃ·¹±× ¼³Á¤ - ÇöÀç ½ºÅ×ÀÌÅÍ½º Ã¢ÀÇ ´É·Â Ãâ·Â ºÎºÐ, ¾ÆÀÌµð ÆÐ½º¿öµå Ã¢¿¡ »ç¿ë
						switch(SubMenu->nField[j].nImageNumber)
						{ 
							case DP_NORMAL_PUT : 	{// 0909 kkh ex ¸¸µé ¾ÆÀÌÅÛ ³Ñ¹ö¸¦ ¹Þ¾Æ ¿Í¼­ ±×¹øÈ£·Î ºÐ·ùÇÏ¿© ¾ÆÀÌÅÛ Á¢¸ñ ÀÌ¹ÌÁö¸¦ ¸¸µç´Ù
								switch ( SkillNo )
								{
								case 43:	FieldTypeNomalPut(SubMenu->x, SubMenu->y, 5, -118, 5, SubMenu->nField[j].nImageType);
											if( SubMenu->work == 5 ) break;		// °°À» °æ¿ì
											SubMenu->work = 5;
											SetRect(SubMenu->nField[4].rCheakBox,40, -113,163,-33);
											{
												RECT rect = SubMenu->nField[4].rCheakBox;
												WORD x = 5+( rect.right - rect.left ) /2 + 15;
												WORD y = -118+( rect.bottom - rect.top ) /2;
												SubMenu->key = MAKELONG( x, y );
											}
											break;//Ã¶ ¾ÆÀÌÅÛ (¸ð·ç)

								case 48:	//È°	
								case 49:	//È­»ì
								case 42:	FieldTypeNomalPut(SubMenu->x, SubMenu->y, 18, -77, 6, SubMenu->nField[j].nImageType);
											if( SubMenu->work == 6 ) break;		
											SubMenu->work = 6;
											SetRect(SubMenu->nField[4].rCheakBox,21, -72,200,4);
											{
												RECT rect = SubMenu->nField[4].rCheakBox;
												WORD x = 18+( rect.right - rect.left ) /2;
												WORD y = -77+( rect.bottom - rect.top ) /2;
												SubMenu->key = MAKELONG( x, y );
											}
											break;//³ª¹«¿Í °ü·ÃµÈ ¾ÆÀÌÅÛ ¸¸µé±â (¸ñÀç Á¦ÀÛÆÇ)

								case 37:	FieldTypeNomalPut(SubMenu->x, SubMenu->y, 42, -100, 8, SubMenu->nField[j].nImageType);
											if( SubMenu->work == 8 ) break;	
											SubMenu->work = 8;											
											SetRect(SubMenu->nField[4].rCheakBox,57, -87,147,-7);
											{
												RECT rect = SubMenu->nField[4].rCheakBox;
												WORD x = 42+( rect.right - rect.left ) /2;
												WORD y = -100+( rect.bottom - rect.top ) /2;
												SubMenu->key = MAKELONG( x, y );
											}
											break;//¾àÃÊ¸¦ °¡°øÇÏ¿© °¡·ç·Î.. (Á¾Áö±â)

								case 50:	FieldTypeNomalPut(SubMenu->x, SubMenu->y, 39, -142, 12, SubMenu->nField[j].nImageType);
											if( SubMenu->work == 12 ) break;
											SubMenu->work = 12;
											SetRect(SubMenu->nField[4].rCheakBox,74, -105,144,-60);
											{
												RECT rect = SubMenu->nField[4].rCheakBox;
												WORD x = 39+( rect.right - rect.left ) /2;
												WORD y = -142+( rect.bottom - rect.top ) /2+30;
												SubMenu->key = MAKELONG( x, y );
											}
											break;//Æ÷¼Ç	(¾à Ç×¾Æ¸®) 

								case 52:	//¾àº´ 
								case 53:	FieldTypeNomalPut(SubMenu->x, SubMenu->y, 39, -130, 7, SubMenu->nField[j].nImageType);
											if( SubMenu->work == 7 ) break;
											SubMenu->work = 7;											
											SetRect(SubMenu->nField[4].rCheakBox,72, -113,149,-58);
											{
												RECT rect = SubMenu->nField[4].rCheakBox;
												WORD x = 39+( rect.right - rect.left ) /2;
												WORD y = -130+( rect.bottom - rect.top ) /2;
												SubMenu->key = MAKELONG( x, y );
											}
											break;//¾çÃÊ  (ºÒ Ç×¾Æ¸®)

								case 51:	//ÇÕ±Ý 
								case 27:	FieldTypeNomalPut(SubMenu->x, SubMenu->y, 16, -84, 9, SubMenu->nField[j].nImageType);
											if( SubMenu->work == 9 ) break;
											SubMenu->work = 9;											
											SetRect(SubMenu->nField[4].rCheakBox,25, -72,200,-6);//
											{
												RECT rect = SubMenu->nField[4].rCheakBox;
												WORD x = 16+( rect.right - rect.left ) /2;
												WORD y = -84+( rect.bottom - rect.top ) /2;
												SubMenu->key = MAKELONG( x, y );
											}
											break;//±¤¼®¿¡¼­ ±Ý¼ÓÀÇ·Î...   (¿ë±¤·Î)

								case 45:	//Ãµ 
								case 47:	FieldTypeNomalPut(SubMenu->x, SubMenu->y, 13, -71, 10, SubMenu->nField[j].nImageType);
											if( SubMenu->work == 10 ) break;
											SubMenu->work = 10;											
											SetRect(SubMenu->nField[4].rCheakBox,12, -65,203,2);//
											{
												RECT rect = SubMenu->nField[4].rCheakBox;
												WORD x = 13+( rect.right - rect.left ) /2;
												WORD y = -71+( rect.bottom - rect.top ) /2;
												SubMenu->key = MAKELONG( x, y );
											}
											break;//¿Ê	(¿Ê°¨ Á¦ÀÛÆÇ)

								case 44:	FieldTypeNomalPut(SubMenu->x, SubMenu->y, 8, -96, 11, SubMenu->nField[j].nImageType);
											if( SubMenu->work == 11 ) break;
											SubMenu->work = 11;											
											SetRect(SubMenu->nField[4].rCheakBox,20,-78,133,-5);//
											{
												RECT rect = SubMenu->nField[4].rCheakBox;
												WORD x = 8+( rect.right - rect.left ) /2;
												WORD y = -96+( rect.bottom - rect.top ) /2;
												SubMenu->key = MAKELONG( x, y );
											}
											break;//À½½Ä ¾ÆÀÌÅÛ (ÈÄ¶óÀÌÆæ)
								}
								int join_no = item_select_result.join_no;		// ¸¸µé°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛÀÌ ¸î¹øÂ° ¾ÆÀÌÅÛÀÎ°¡...
								int resource1 = item_join[join_no].resource1;
								int resource2 = item_join[join_no].resource2;
								int resource3 = item_join[join_no].resource3;
								int resource4 = item_join[join_no].resource4;
								//SelectObject( hdc, __HFontDial[HangulFontType] );
																	
								Hcolor(100,204,100);
								Hprint2( SubMenu->x+60,SubMenu->y+43, g_DestBackBuf, "%s", item_join[join_no].item_name );
								Hcolor(230,230,230);
								if( resource1 ) 
								{
									Hprint2( SubMenu->x+62,SubMenu->y+61, g_DestBackBuf, "%s", GetItemName(resource1) );
									Hprint2( SubMenu->x+62+110,SubMenu->y+61, g_DestBackBuf, "  %d", item_join[join_no].resource1_amount);
								
								}
								if( resource2 ) 
								{
									Hprint2( SubMenu->x+62,SubMenu->y+61+14, g_DestBackBuf, "%s", GetItemName(resource2) );
									Hprint2( SubMenu->x+62+110,SubMenu->y+61+14, g_DestBackBuf, "  %d", item_join[join_no].resource2_amount);
								}
								if( resource3 ) 
								{
									Hprint2( SubMenu->x+62,SubMenu->y+61+28, g_DestBackBuf, "%s", GetItemName(resource3) );
									Hprint2( SubMenu->x+62+110,SubMenu->y+61+28, g_DestBackBuf, "  %d", item_join[join_no].resource3_amount);
								}
								if( resource4 ) 
								{
									Hprint2( SubMenu->x+62,SubMenu->y+61+42, g_DestBackBuf, "%s", GetItemName(resource4) );
									Hprint2( SubMenu->x+62+110,SubMenu->y+61+42, g_DestBackBuf, "  %d", item_join[join_no].resource4_amount);
								}

								break;
								}
						case DP_DATAPUT :   
							{
								SetHangulAlign( TA_CENTER );
								Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%d", *SubMenu->nField[j].nValue);
							}break; 

						case DP_DATAPUT_DEFAULT:
											if( SubMenu->nField[j].nSpecialWillDo )
												SetHangulAlign( SubMenu->nField[j].nSpecialWillDo );
											if( SubMenu->nField[j].nWillDo )
												Hcolor( (WORD)SubMenu->nField[j].nWillDo );
											if( SubMenu->nField[j].nValue )
												Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%d", *SubMenu->nField[j].nValue);
											break; 

						case DP_DATAPUT_DEFAULT_STRING:
											if( SubMenu->nField[j].nSpecialWillDo )
												SetHangulAlign( SubMenu->nField[j].nSpecialWillDo );
											if( SubMenu->nField[j].nWillDo )
												Hcolor( (WORD)SubMenu->nField[j].nWillDo );
											if( SubMenu->nField[j].m_lpTemp )
												RectTextPut( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, (char *)SubMenu->nField[j].m_lpTemp);
											break; 
						case DP_STRING_PUT :{
												int type = j;
												PrintText( SubMenu, j, SubMenu->nField[j].temp ,SubMenu->nField[j].nWillDo);
												break;
											}

						/*-- ³»Áý ¼³¸í¹® Âï±â --*/
						case DP_IDEDITBOX :		Hcolor( FONT_COLOR_DEFAULT );
												EWndMgr.GetTxt( HWND_1, sId, 17);
												if(id_password) 
												{
													if((nCursor_Flicker%=32) < 16)
																Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s_", sId );
													else Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", sId );
												}
												else Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", sId );
												nCursor_Flicker++;
												break;

						case DP_PASSEDITBOX :	Hcolor( FONT_COLOR_DEFAULT );
												EWndMgr.GetTxt( HWND_2, sPassword, 22);
												if(!id_password)
												{
													if((nCursor_Flicker%=32) < 16)
														Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s_", sPassword);
													else Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", sPassword);
												}
												else Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", sPassword);

												nCursor_Flicker++;
												break;

						case DP_SELECT_SELECT :	
							if(!item_select_result.item_no)
							{
								strcpy(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,  "0" );
		 					}
							
							if( ItemMutantKind[GetItemAttr( item_select_result.item_no, ITEM_KIND)].upgrade_type == 0 )//·¹¾î ¾ÈµÊ
							{
								SMenu[MN_ITEM_MAKE_SELECT].nField[7].nType	=	0;
							}
							else
							{
								SMenu[MN_ITEM_MAKE_SELECT].nField[7].nType	=	FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;
							}							

							if( item_select_result.item_no && GetItemAttr( item_select_result.item_no, IMAGE_NUMBER ) )
							{
											
								PutItemIcon( SubMenu->x+SubMenu->nField[j].x+10, SubMenu->y+SubMenu->nField[j].y+26, item_select_result.item_no );
								PutItemIcon( SubMenu->x+SubMenu->nField[j].x+10, SubMenu->y+SubMenu->nField[j].y+26, item_select_result.item_no, 8, 2 );

								ItemExplain( item_select_result.item_no );
		//						SetHangulAlign( TA_CENTER );
								Hcolor( FONT_COLOR_NAME );
								Hprint2( SubMenu->x+SubMenu->nField[j].x+50, SubMenu->y+SubMenu->nField[j].y+21, g_DestBackBuf, "%s", GetItemName( item_select_result.item_no ) );
						
								int join_no = item_select_result.join_no;		// ¸¸µé°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛÀÌ ¸î¹øÂ° ¾ÆÀÌÅÛÀÎ°¡...
								
								int resource1 = item_join[join_no].resource1;
								int resource2 = item_join[join_no].resource2;
								int resource3 = item_join[join_no].resource3;
								int resource4 = item_join[join_no].resource4;

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

								int MakeMuch =0;
								//0011030 lsw >
								GetSkillResorceAmount(r1count,r2count,r3count,r4count);
								//0011030 lsw <
								if( r1amount ) 
								{r1 =r1count /r1amount;}
								if( r2amount ) 
								{r2 =r2count /r2amount;}
								if( r3amount ) 
								{r3 =r3count /r3amount;}
								if( r4amount ) 
								{r4 =r4count /r4amount;}

								int linecount =0;
							//SetHangulAlign( TA_CENTER);
								if( r1amount)
								{
								Hcolor( FONT_COLOR_WHITE );
								
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "%s",
										GetItemName( resource1));
								Hcolor( FONT_COLOR_MAX );
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                    %7d",
										r1count);
								Hcolor( FONT_COLOR_PLUS);
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                           /  *");
								Hcolor( FONT_COLOR_WHITE );
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                            %2d %s",
										r1amount,SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp);
								linecount++;
								}
								if( r2amount)
								{
								Hcolor( FONT_COLOR_WHITE );
								
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "%s",
										GetItemName( resource2));
								Hcolor( FONT_COLOR_MAX );
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                    %7d",
										r2count);
								Hcolor( FONT_COLOR_PLUS);
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                           /  *");
								Hcolor( FONT_COLOR_WHITE );
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                            %2d %s",
										r2amount,SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp);
								linecount++;
								}
								if(r3amount)
								{
								Hcolor( FONT_COLOR_WHITE );
								
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "%s",
										GetItemName( resource3));
								Hcolor( FONT_COLOR_MAX );
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                    %7d",
										r3count);
								Hcolor( FONT_COLOR_PLUS);
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                           /  *");
								Hcolor( FONT_COLOR_WHITE );
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                            %2d %s",
										r3amount,SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp);
								linecount++;
								}
								if(r4amount)
								{
								Hcolor( FONT_COLOR_WHITE );
								
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "%s",
										GetItemName( resource4));
								Hcolor( FONT_COLOR_MAX );
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                    %7d",
										r4count);
								Hcolor( FONT_COLOR_PLUS);
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                           /  *");
								Hcolor( FONT_COLOR_WHITE );
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+42+linecount*14, g_DestBackBuf,  "                            %2d %s",
										r4amount,SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp);
								linecount++;
								}
							}
					break;
					//011013 lsw <
						case DP_USE_SELECT :	if( skill_use_item_result.item_no )	
												{
													PutItemIcon( SubMenu->x+SubMenu->nField[j].x-25, SubMenu->y+SubMenu->nField[j].y+26, skill_use_item_result.item_no );
													PutItemIcon( SubMenu->x+SubMenu->nField[j].x-25, SubMenu->y+SubMenu->nField[j].y+26, skill_use_item_result.item_no, 8, 2 );

													POS pos = skill_use_item_result.pos;
													ItemAttr *ii = ::GetItemByPos( pos ) ;
													if( ii && ii->item_no )	
													{
														ItemExplain( *ii );
														SetHangulAlign( TA_CENTER );
														Hcolor( FONT_COLOR_NAME );
														Hprint2( SubMenu->x+SubMenu->nField[j].x+50, SubMenu->y+SubMenu->nField[j].y+21, g_DestBackBuf, "%s", GetItemName( skill_use_item_result.item_no ) );
													}
												}
												break;
						case DP_RECT_STRING_PUT :
							{
								//DP_RECT_STRING_PUT´Â nRectImage¿¡ ¹®ÀÚ¿­ÀÌ ÂïÈú »ç°¢ÇüÀÇ °¡·Î±æÀÌ¸¦ ³Ö´Â´Ù.
						//		const int nWillDo = SubMenu->nField[j].nWillDo;
						//		if( nWillDo )
						//		{
						//			Hcolor( nWillDo );
						//		}
								RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage,SubMenu->nField[j].temp); 
							}break;
						case DP_HEALING_EXPLAIN : {
														Hcolor( FONT_COLOR_DEFAULT );
														static int delay;
														static bool flag=true;
														PutSkillIcon( SubMenu->x+35+15, SubMenu->y+63+15, 86 /* Èú¸µ ¹øÈ£ */ );
														PutSkillIcon( SubMenu->x+35+15, SubMenu->y+63+15, 86, delay );
														if( flag ) delay++;
														else delay -- ;
														if( delay == 8 ) flag = false;
														else if(delay == 0 ) flag = true;
														Hcolor( FONT_COLOR_NAME );
														Hprint2( SubMenu->x+94, SubMenu->y+73, g_DestBackBuf, "HEALING" );
														Hcolor( FONT_COLOR_DEFAULT );
														RectTextPut( SubMenu->x+40, SubMenu->y+103, 145 , "This skill will do up your or anyone's HP." );
														char  temp[127];
														//011012 lsw >
														if( SCharacterData.SkillId[86] ) wsprintf(temp, "%s: %d", "You've got this skill", SCharacterData.nJobSkill[skill[86].inclusive].skillexp / 10000 );
														//011012 lsw <
														else wsprintf(temp, "%s", "Would you like to learn this skill?");
														Hprint2( SubMenu->x+34, SubMenu->y+145, g_DestBackBuf , "%s", temp );

														SetHangulAlign( TA_RIGHT );
														Hcolor( FONT_COLOR_NUMBER );
														if( SCharacterData.SkillId[86] ) Hprint2( SubMenu->x+97+53, SubMenu->y+190, g_DestBackBuf, "%d", 0 );
														else Hprint2( SubMenu->x+97+53, SubMenu->y+190, g_DestBackBuf, "%d", skill[86].money );
														break;
												  }
						case DP_TOWNGALL_TITLE : {
													int para = SubMenu->nField[j].nSHideNomalStart;
													int width = SubMenu->nField[j].nWillDo;
													int gab_y = SubMenu->nField[j].nSpecialWillDo;

													if( para == 0 && !(*c_BbsTitle[para].m_Name) )
													{
														Hcolor( FONT_COLOR_PLUS );
														Hprint( SubMenu->x+SubMenu->nField[j].x+10, SubMenu->y+SubMenu->nField[j].y+gab_y, g_DestBackBuf,  lan->OutputMessage(6,64));//010215 lsw
														SubMenu->nField[j+2].nType = FT_NOMAL_PUT;
													}
													else
													{
														for( int a=0; a<3; a++ )
														{
															if( *c_BbsTitle[para+a].m_Name )
															{
																Hcolor( FONT_COLOR_DEFAULT );
																Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+a*gab_y, g_DestBackBuf, "%s", c_BbsTitle[para+a].sp_BbsTextTitle );
															}
														}
													}
													if( para > SubMenu->nField[j].nSHideNomalCount-1 ) break;
													if( *c_BbsTitle[para+SubMenu->nField[j].nShideNomalPlus].m_Name )
													{
														SubMenu->nField[j+2].nType = FT_HIDE_ISCROLL_AUTO_PUT;
													}
													else SubMenu->nField[j+2].nType = FT_NOMAL_PUT;
													break;
												 }
						case DP_BBS_PUT :	{
												Hcolor( FONT_COLOR_PLUS );
//												RectTextPut( SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nRectImage,s_BbsTextBuf );
												int para = SubMenu->work;
												if( c_BbsTitle[para].is_first )
												{	

												}
												else BbsTextPut( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nWillDo, c_BbsTitle[para].m_szMessage , SubMenu->nField[j].nSHideNomalStart ); break;
												break;
											}
						case DP_BBS_WRITE_PUT : 
							{
								char temp[1024]={0,};
								int width = SubMenu->nField[j].nRectImage;
								
								if( GetFocus() == g_hwndMain )
								{
									SetFocus2(HWND_BBS);//021001 lsw // Æ÷Ä¿½º¸¦ ¸ÞÀÎ À©µµ¿ì¿¡ ¹ÝÈ¯ÇÑ »óÅÂ¶ó¸é...
								}
								
								EWndMgr.GetTxt( HWND_BBS, temp, 1023 );

								int line;
								line = GetLineRectTextPut( width , temp );
								int page_line = line - line%6;
								Hcolor( 0 );
								SkipRectTextPut( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, width , page_line, temp );
							}break;
						case DP_HOUSE_PUT : {
												int para = SubMenu->nField[j].nSHideNomalStart;
												CHouse temp_house;
												switch( SubMenu->nField[j].nWillDo )
												{
													case RESIDING : SubMenu->nField[j].nSHideNomalCount = house_res_ct;
																	temp_house = house_sub_residing[para];
																	break;
													case GUILD :SubMenu->nField[j].nSHideNomalCount = house_guild_ct;
																temp_house = house_sub_guild[para];
																break;
													case NOTRESIDING :  SubMenu->nField[j].nSHideNomalCount = house_not_res_ct;
																		temp_house = house_sub_not_res[para];
																		break;
													default : return;
												}
												PutHouseImage( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, temp_house.m_Image, SubMenu->work?true:false );

												SetHangulAlign( TA_RIGHT );
												Hprint2( SubMenu->x+74+74, SubMenu->y+229, g_DestBackBuf, "%d", temp_house.m_Price);
												Hprint2( SubMenu->x+74+74, SubMenu->y+256, g_DestBackBuf, "%d", SCharacterData.nMoney);
												break;
											}
						case DP_ITEM_MESSAGE : {
													int item = SubMenu->nField[j].nWillDo;
													static int delay=1;
													static bool flag=true;
													PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+15, item );
													PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+15, item, delay );
													if( flag ) delay++;
													else delay -- ;
													if( delay == 10 ) flag = false;
													else if(delay == 1 ) flag = true;

													
													Hcolor( FONT_COLOR_DEFAULT );
													RectTextPut(SubMenu->x+29,SubMenu->y+60,SubMenu->nField[j].nRectImage,SubMenu->nField[j].temp);
													
													Hcolor( FONT_COLOR_SPECIAL );
													SetHangulAlign( TA_CENTER );
													Hprint2( SubMenu->x+63+68, SubMenu->y+34, g_DestBackBuf, "%s", GetItemName( item ) );
													break; 
											   }
						
						case DP_MAGIC_PUT:	//soto-030518
							{
								int willdo = SubMenu->nField[j].nWillDo;
								int para = SubMenu->nField[SubMenu->nTemp].nSHideNomalStart + willdo;		// °è¿­´ç ¸ÅÁ÷ + ¿ÞÂÊ, ¿À¸¥ÂÊ
								
								int sptype = 0; 
								if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) sptype = 150;

								if( para>=0 && para < magic_by_class.max[SubMenu->nTemp] ) 
								{
									int magic_num = magic_by_class.magic[SubMenu->nTemp][para];
									if( !magic_num ) break;
									
									PutMagicIcon( SubMenu->x+SubMenu->nField[j].x+20, SubMenu->y+SubMenu->nField[j].y+20, magic_num );
									if( !SCharacterData.MagicId[magic_num] ) 
										PutMagicIcon( SubMenu->x+SubMenu->nField[j].x+20, SubMenu->y+SubMenu->nField[j].y+20, magic_num,8,2 );	
									Hcolor( 0 );
									RectTextPut( SubMenu->x+(willdo?287:37) , SubMenu->y+103, 198, magic[magic_num].GetExplain() );
																
									
									Hcolor( 30, 70, 5 );
									if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL )
										Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+150+35 , g_DestBackBuf,lan->OutputMessage(7,52) , g_lpMT[magic_num+sptype].require_WP);//010215 lsw
									else if( g_lpMT[magic_num+sptype].require_IT )
										Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+150+35, g_DestBackBuf, lan->OutputMessage(7,53), g_lpMT[magic_num+sptype].require_IT);
									if( g_lpMT[magic_num+sptype].basic_Magery )
									{
										if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL )
											Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+165+35, g_DestBackBuf,lan->OutputMessage(7,54),  g_lpMT[magic_num+sptype].basic_Magery);
										else
											Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+165+35, g_DestBackBuf,lan->OutputMessage(7,55) , g_lpMT[magic_num+sptype].basic_Magery);
									}
									if( g_lpMT[magic_num+sptype].point_WS)
										Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+180+35, g_DestBackBuf, lan->OutputMessage(7,56), g_lpMT[magic_num+sptype].point_WS);
									if( g_lpMT[magic_num+sptype].Price )
#ifdef CHANGE_MONEY_
										Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+195+35, g_DestBackBuf,lan->OutputMessage(7,57) , g_lpMT[magic_num+sptype].Price);
#else
									Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+195+35, g_DestBackBuf, lan->OutputMessage(7,58), g_lpMT[magic_num+sptype].Price);//010215 lsw
#endif
									
									Hcolor( FONT_COLOR_NAME );
									SetHangulAlign( TA_CENTER );
									Hprint( SubMenu->x+(willdo?390:130) , SubMenu->y+86 , g_DestBackBuf, magic[magic_num].GetName() );
								}
								else if( para == magic_by_class.max[SubMenu->nTemp] )		// ¹è¿ìÁö ¸øÇÏ´Â ¸¶¹ý ¾îµÓ°Ô º¸¿©ÁØ´Ù.
								{
									int magic_num=0;
									int ret = GetNoLearnMagic( SubMenu->nTemp, magic_num );	
									if( ret != 1000 )
									{
										char *buf;
										switch( ret )
										{
										case 1 :	buf = lan->OutputMessage(7,101);	break;//010215 lsw
										case 2 :	buf = lan->OutputMessage(7,102);	break;
										case 3 :	buf = lan->OutputMessage(7,103);	break;
										case 4 :	buf = lan->OutputMessage(7,104);	break;
										case 5 :	buf = lan->OutputMessage(7,105);	break;
										case 6 :	buf = lan->OutputMessage(7,106);	break;
										case 7 :	buf = lan->OutputMessage(7,107);	break;
										case 8 :	
											{	if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )
												{
													buf = lan->OutputMessage(7,108);
												}
												else
												{
													buf = lan->OutputMessage(7,109);
												}
											}
											break;
//<soto-030702
										case 9 :
											{
												sprintf(g_DestBackBuf, lan->OutputMessage(4,136),g_lpMT[magic_num + sptype].nDualStep);
												buf = g_DestBackBuf;
											}
											break;
										case 10 :
											{	//< CSD-030723
												sprintf(g_DestBackBuf, lan->OutputMessage(4,137),g_lpMT[magic_num + sptype].nDualStep);
												buf = g_DestBackBuf;
											}	//> CSD-030723
//>soto-030702
											break;
										default :	buf = lan->OutputMessage(7,110); break;//010215 lsw
										}
										
										PutMagicIcon( SubMenu->x+SubMenu->nField[j].x+20, SubMenu->y+SubMenu->nField[j].y+20, magic_num, 8,1 );
										SetHangulAlign( TA_CENTER );
										Hcolor( 30, 70, 5 );
										Hprint( SubMenu->x+(willdo?390:130) , SubMenu->y+86 , g_DestBackBuf, magic[magic_num].GetName() );
										
										SetHangulAlign( TA_LEFT );
										
										Hcolor( 0 );
										RectTextPut( SubMenu->x+(willdo?287:37) , SubMenu->y+105, 198, magic[magic_num].GetExplain() );
										
										int sptype = 0; 
										if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) sptype = 150;
										
										Hcolor( 180, 0, 0 ); 
										RectTextPut( SubMenu->x+(willdo?287:37) , SubMenu->y+170, 198, buf );
										
				

																		

										if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL )
											Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+150+35 , g_DestBackBuf, lan->OutputMessage(7,52), g_lpMT[magic_num+sptype].require_WP);//010215 lsw
										else if( g_lpMT[magic_num+sptype].require_IT )
											Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+150+35, g_DestBackBuf, lan->OutputMessage(7,53), g_lpMT[magic_num+sptype].require_IT);
										if( g_lpMT[magic_num+sptype].basic_Magery )
											Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+165+35, g_DestBackBuf, lan->OutputMessage(7,59), g_lpMT[magic_num+sptype].basic_Magery);
										if( g_lpMT[magic_num+sptype].point_WS)
											Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+180+35, g_DestBackBuf, lan->OutputMessage(7,56), g_lpMT[magic_num+sptype].point_WS);


										if( g_lpMT[magic_num+sptype].Price )
#ifdef CHANGE_MONEY_
											Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+195+35, g_DestBackBuf, lan->OutputMessage(7,57), g_lpMT[magic_num+sptype].Price);
#else
										Hprint2( SubMenu->x+(willdo?390:130)-93 , SubMenu->y+195+35, g_DestBackBuf, lan->OutputMessage(7,58), g_lpMT[magic_num+sptype].Price);//010215 lsw
#endif
										
									}
								}
								
								break;
							}
						case DP_ITEM_PUT:	{
												if( !SubMenu->nField[j].nValue ) return;
												PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+15, *SubMenu->nField[j].nValue );
												int alpha = SubMenu->nField[j].nWillDo;
												if( alpha ) PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+15, *SubMenu->nField[j].nValue, alpha );

												break;
											}
						case DP_DISEASE_PUT : {
												;	// º´ ÀÌ¸§Àº SHideNomal[HN_HEAL_PREVENTING][?].temp¸¦  ÂüÁ¶ 
												int y_len[3] = {72,107,142};		// yÃà ÁÂÇ¥
												int x_len = 224;					// xÃà ³ÐÀÌ
												
												int &scroll = SubMenu->nField[j].nSHideNomalStart;
												if( !SubMenu->CheakType )
												{
													scroll = 0;
													SubMenu->work = 0;
													SubMenu->CheakType = 1;
												}
												int cn=0;
												int take[6]={0,};
												int a;
												for( a=0; a<6; a++ )		// ³»°¡ °É¸° º´ Ã£±â		==> ÃÊ±â ¼¼ÆÃ
												{
													if( SCharacterData.disease[a] )
													{
														take[cn] = a;
														cn++;
													}									
												}
												SubMenu->nField[j].nSHideNomalCount = cn;

												int half;
												if( cn )
												{
													if( scroll )
													{
														for( a=3; a<cn; a++ )
														{
															char *buf = SHideNomal[HN_HEAL_PREVENTING][take[a]].temp;
															int total_len = GetTextImageWidth( buf );
															half=(x_len - total_len) /2;
															FieldTypeTextPut(SubMenu->x, SubMenu->y, half, y_len[a-3], buf);
														}
													}
													else
													{
														int max =3;
														if( cn < 3 ) max = cn;
														for( a=0; a<max; a++ )
														{
															char *buf = SHideNomal[HN_HEAL_PREVENTING][take[a]].temp;
															int total_len = GetTextImageWidth( buf );
															half=(x_len - total_len) /2;
															FieldTypeTextPut(SubMenu->x, SubMenu->y, half, y_len[a], buf);
														}

													}
													if(SubMenu->work) SubMenu->nField[j].nWillDo = take[SubMenu->work-1+scroll];
													else SubMenu->nField[j].nWillDo = 0;
												}
												else 
												{
													Hcolor( FONT_COLOR_SPECIAL );
													RectTextPut(SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, "You are very good now."); // CSD-030324
												}

												break;
											  }
						case DP_HEAL_MONEY : {
												if( !SubMenu->work ) break;
												int willdo = SubMenu->nField[j].nWillDo;	// ½ºÅ©·Ñ ¿©ºÎ È®ÀÎ
												int disease_num = SubMenu->nField[willdo].nWillDo;		// Èú¸µÀº willdo¾È¿¡ Á¤È®ÇÑ Áúº´ ¹øÈ£°¡ µé¾î ÀÖÀ½
												SetHangulAlign( TA_RIGHT );
												Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%d", disease_tbl[disease_num].price_heal );
												break;
											 }
						case DP_PREVENT_MONEY : {
													if( !SubMenu->work ) break;
													int willdo = SubMenu->nField[j].nWillDo;	// ½ºÅ©·Ñ ¿©ºÎ È®ÀÎ
													int disease_num = SubMenu->work-1+ SubMenu->nField[willdo].nSHideNomalStart;
													SetHangulAlign( TA_RIGHT );
													Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%d", disease_tbl[disease_num].price_heal );
													break;
												 }
						case DP_INN : {
										if( !SubMenu->CheakType )
										{
											SubMenu->CheakType = 1;
											SubMenu->work = INN_SINGLE;
											break;
										}
										int num = 0;
										switch( SubMenu->work )
										{
											case 0 : break;
											case INN_SINGLE	:	num = 4; break;		// ÇÊµå ¹øÈ£
											//case INN_SINGLE	:	num = 3; break;		// ÇÊµå ¹øÈ£
											//case INN_DOUBLE	:	num = 4; break;
											//case INN_MULTI	:	num = 5; break;
										}
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[num].x, SubMenu->nField[num].y, SubMenu->nField[num].nImageNumber);

										int cell ; // ÁöºÒÇØ¾ßÇÒ ¹æ °ª
										if( i == MN_INN_REST )
										{
											if( DayLightControl > 15 )
											{
												switch( SubMenu->work )
												{
													case INN_SINGLE	: cell = 4; break;
													//case INN_DOUBLE	: cell = 7; break;
													//case INN_MULTI	: cell = 9; break;
													default : cell = 0; break;
												}
											}
											else
											{
												switch( SubMenu->work )
												{
													case INN_SINGLE	: cell = 6; break;
													//case INN_DOUBLE	: cell = 9; break;
													//case INN_MULTI	: cell = 11; break;
													default : cell = 0; break;
												}
											}
										}
										else
										{
											switch( SubMenu->work )
											{
												case INN_SINGLE	: cell = 10; break;
												//case INN_DOUBLE	: cell = 16; break;
												//case INN_MULTI	: cell = 20; break;
												default : cell = 0; break;
											}
										}
										SetHangulAlign( TA_RIGHT );
										Hprint2( SubMenu->x+56+87, SubMenu->y+218, g_DestBackBuf, "%d", cell );
										break;
									}
						case DP_MAGIC_CLASS :{
												int magic_class = SubMenu->nTemp;
												int cls_to_img[] = {1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,};
												int img = cls_to_img[magic_class];
												if( !spr[img].img ) 
												{
													LoadSmallMenuImage( img );
												}
												PutCompressedImage( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, &spr[img] );
												break;
											 }
						case DP_MEMORIZE_POINT : {
													if( SubMenu->work )		// ¼±ÅÃ ¾ÆÀÌÄÜÀÌ ÀÖÀ» °æ¿ì
													{
														int magic_num = memorize[SubMenu->work-1];	// ¸¶¹ý ¸í
														SetHangulAlign( TA_CENTER );
														Hcolor( FONT_COLOR_NAME );
														Hprint( SubMenu->x+SubMenu->nField[j].x+48, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, magic[magic_num].GetName() );

														SetHangulAlign( TA_RIGHT );		// ¿ä±¸ wsps
														Hcolor( FONT_COLOR_NUMBER );
														Hprint2( SubMenu->x+310+58, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%d", SCharacterData.nCharacterData[SPELL]?g_lpMT[150+magic_num].exhaust_WS:g_lpMT[magic_num].exhaust_WS );
													}
													else
													{
														SetHangulAlign( TA_RIGHT );
														Hcolor( FONT_COLOR_NUMBER );
													}
													// ¸Þ¸ð¶óÀÌÁî Æ÷ÀÎÆ® ÃÖ´ë ÃÖ¼Ò
													Hprint2( SubMenu->x+278+55+35, SubMenu->y+166+26, g_DestBackBuf, "%3d/%3d", SCharacterData.memorize_point, SCharacterData.memorize_point_max );
													break;
												 }
						case DP_TITLE_PUT : {
												int nWillDo = SubMenu->nField[j].nWillDo;
												SetHangulAlign( TA_CENTER );
												Hcolor( nWillDo );
												SetHangulFont(4);
												//void HprintBold( int x, int y, int fc, int bc, char *s, ... )
												//HprintBold( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y,RGB( 255, 255, 255 ), 0, SubMenu->nField[j].temp );
												Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%s", SubMenu->nField[j].temp );
												SetHangulFont(2);
												break;
											}

//<soto-030512
						case DP_AUCTION_DATE :
							{
								switch(Auction.m_nPeriod)
								{
								case 0:
									{
										strcpy(SubMenu->nField[j].temp,lan->OutputMessage(4,600));
									}
									break;
								case 1:
									{
										strcpy(SubMenu->nField[j].temp,lan->OutputMessage(4,601));
									}
									break;
								case 2:
									{
										strcpy(SubMenu->nField[j].temp,lan->OutputMessage(4,602));
									}
									break;
								}

								Hprint2
									(
									SubMenu->x+SubMenu->nField[j].x + 5,
									SubMenu->y+SubMenu->nField[j].y + 15,
									g_DestBackBuf, "%s", 
									SubMenu->nField[j].temp
									);
							}
							break;
//>soto-030512

						//011011 lsw
						default:
							{
								lsw_FT_DATA_PUT(SubMenu  ,j);
							}
						//011011 lsw <
							break;
						}
						break;

		case FT_INVENTORY :
			{
				DisplayInventoryMenu(SubMenu,j);
			}break;

		case FT_ACCEL_INVENTORY :
			{
				DisplayQuickMenu(SubMenu,j);
			}break;
		case FT_CANDLE_INVENTORY :
			{
				DisplayCandleMenu(SubMenu,j);
			}break;
		case FT_ARMOR_INVENTORY :{
									if(j == 24)// soto-1.02µà¾ó.
										break;
									int nWeapon_Type = SubMenu->nField[j].nWillDo;
									int wear_position = SubMenu->nField[j].nSpecialWillDo;

									if( SubMenu->nField[j].fRectMouse ) 
									{
										if( g_MouseItemType==1 )
										{
											MouseItemInRectOn = j;
											MouseItemInRect.x = SubMenu->x+SubMenu->nField[j].x+17;
											MouseItemInRect.y = SubMenu->y+SubMenu->nField[j].y+17;
										}
										else 
										{
											MouseItemInRectOn = 0;
											if( g_MouseItemType==0 && j != 24)//soto-1.02µà¾ó 24´Â µà¾óÀÌ ÀÖÀ»°÷ÀÌ´Ù.
												if( !LButtonDown && EquipItemAttr[nWeapon_Type].item_no ) ItemExplain(EquipItemAttr[nWeapon_Type]);
												
										}
									}
									else
									{
										if( MouseItemInRectOn == j ) MouseItemInRectOn = 0;
									}
									if( g_MouseItemType==1 )
									{
										int item_position = GetItemAttr( HandItemAttr.item_no , WEAR_ABLE);
										if( (wear_position==item_position || ((wear_position==WEAR_RIGHT_HAND || wear_position==WEAR_LEFT_HAND) && item_position==WEAR_TWO_HAND)) && GetItemAttr(HandItemAttr.item_no, EQUIPABLE) )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
											FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, 2 ,2);
										}
										else 
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j]./*nRectImage*/nImageNumber+1);
											FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j]./*nRectImage*/nImageNumber+1,2,2);
										}
									}
									
									//µà¾ó ¾ÆÀÌÅÛÀ» Âï¾îÁØ´Ù.
									g_SymBolMgr.PutSymbolItem(); //1.02;
									if( SubMenu->nField[24].fRectMouse )
									{
										g_SymBolMgr.Explain(426,175);
									}

									break;
								 }
		//ÀÎº¥Åä¸®Ã¢¿¡ ¾ÆÀÌÅÛÀ» ³õ°í ¾ò´Â ÇÊµå
		case FT_BANK_INVENTORY :{
									for(k=0;k<3;k++) 
										for(i=0;i<6;i++)  
											if( BankItemAttr[SubMenu->nField[0].nSHideNomalStart][k][i].item_no )
												  PutItemIcon( SubMenu->x+15+9+35*i, SubMenu->y+15+209+35*k, BankItemAttr[SubMenu->nField[0].nSHideNomalStart][k][i].item_no);

									ItemX = ( g_pointMouseX - (SubMenu->x + 7) ) / 35;
									ItemY = ( g_pointMouseY - (SubMenu->y + 206) ) / 35;
																										
									int xCheck = ( g_pointMouseX - (SubMenu->x + 7) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
									int yCheck = ( g_pointMouseY - (SubMenu->y + 206) ) % 35;
									if( (xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ) )  
									{
										break;	
									}
                  if( (3 > ItemY  && 0 <= ItemY )
										&& (6 > ItemX  && 0 <= ItemX ))//ÀÎµ¦½º °è»ê ÀÌ»ó Ã¼Å© 
									{
									  ItemAttr &item = BankItemAttr[SubMenu->nField[0].nSHideNomalStart][ItemY][ItemX];
									  if( !item.item_no ) break;
									  if( !g_MouseItemType &&  SubMenu->nField[j].fRectMouse ) 
									  {
  //										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }

										  PutItemIcon( SubMenu->x+15+9+35*ItemX, SubMenu->y+15+209+35*ItemY, item.item_no, 8, 2);
										  ItemExplain(item);
									  }
                  }
									break;
								}
		case FT_JOB_ITEM :	{		
								SubMenu->nField[j].nSHideNomalCount = aSellBuyItemMax/15+1;
								int first = SubMenu->nField[j].nSHideNomalStart;
								int param;
								for(int a=0;a<3;a++)	// È­¸é Ãâ·Â
								{
									for(int b=0;b<5;b++)  
									{
										param = first*3*5+a*5+b;
										if(aSellBuyItemAttr[param].item_no)
											PutItemIcon( SubMenu->x+15+26+35*b, SubMenu->y+15+37+35*a, aSellBuyItemAttr[param].item_no);
									}
								}

								ItemX = ( g_pointMouseX - (SubMenu->x + 25) ) / 35;
								ItemY = ( g_pointMouseY - (SubMenu->y + 35) ) / 35;
																									
								int xCheck = ( g_pointMouseX - (SubMenu->x + 25) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
								int yCheck = ( g_pointMouseY - (SubMenu->y + 35) ) % 35;
								if( (xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ) )  break;	

								param = SubMenu->nField[j].nSHideNomalStart*3*5+ItemY*5+ItemX;
								ItemAttr *pItem= &aSellBuyItemAttr[param];	// ¸¶¿ì½º°¡ Å¬¸¯ÇÑ °÷
								if( !pItem->item_no ) break;
							
								if(SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fLButtonDown)
								{
									PutItemIcon( SubMenu->x+15+26+35*ItemX, SubMenu->y+15+37+35*ItemY, pItem->item_no, 8, 2);
									if( *SubMenu->nField[j].temp )
									{
										SetHangulAlign( TA_LEFT ); 
										Hcolor( FONT_COLOR_DEFAULT );
										kein_TextPut( SubMenu->x+32, SubMenu->y+159, SubMenu->nField[j].temp );
									}
								}
								break;
							}
		case FT_MALL_ITEM :	{		
								SubMenu->nField[j].nSHideNomalCount = itemMallMax/15;
								if (itemMallMax % 15)
								{
									SubMenu->nField[j].nSHideNomalCount += 1;
								}

								int first = SubMenu->nField[j].nSHideNomalStart;
								int param;
								int base_x = SubMenu->x+SubMenu->nField[j].x;
								int base_y = SubMenu->y+SubMenu->nField[j].y;
								for (int a = 0; a < 3; a++)	// È­¸é Ãâ·Â
								{
									for (int b = 0; b < 5; b++)  
									{
										param = first*3*5+a*5+b;
										if (itemMallAttr[param].item_no)
										{
											PutItemIcon(base_x+15+35*b, base_y+15+35*a, itemMallAttr[param].item_no);
										}
									}
								}

								ItemX = ( g_pointMouseX - (base_x) ) / 35;
								ItemY = ( g_pointMouseY - (base_y) ) / 35;
																									
								int xCheck = ( g_pointMouseX - (base_x) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
								int yCheck = ( g_pointMouseY - (base_y) ) % 35;
								if ( (xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ) )  break;	

								param = SubMenu->nField[j].nSHideNomalStart*3*5+ItemY*5+ItemX;
								ItemAttr *pItem= &itemMallAttr[param];	// ¸¶¿ì½º°¡ Å¬¸¯ÇÑ °÷
								if (!pItem->item_no) break;
							
								if (SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fLButtonDown)
								{
									PutItemIcon(base_x+15+35*ItemX, base_y+15+35*ItemY, pItem->item_no, 8, 2);
								}
								break;
							}
		case FT_SUPPER_INV :	{
									if( !SubMenu->CheakType )
									{
										SubMenu->CheakType = 1;
										GetSupperItem( SCharacterData.believe_god );	// SupperItemAttr À» ¼ÂÆÃ ÇØÁØ´Ù.
									}

									int gabx = SubMenu->nField[j].x+4;
									int gaby = SubMenu->nField[j].y+2;

									for(k=0;k<3;k++)
										for(i=0;i<5;i++)
											if( SupperItemAttr[k][i].item_no )
												  PutItemIcon( SubMenu->x+gabx+15+35*i, SubMenu->y+gaby+15+35*k, SupperItemAttr[k][i].item_no);

									ItemX = ( g_pointMouseX - (SubMenu->x + gabx) ) / 35;
									ItemY = ( g_pointMouseY - (SubMenu->y + gaby) ) / 35;
																										
									int xCheck = ( g_pointMouseX - (SubMenu->x + gabx) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
									int yCheck = ( g_pointMouseY - (SubMenu->y + gaby) ) % 35;
									if( (xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ) )  
									{
										break;
									}
									ItemAttr &item = SupperItemAttr[ItemY][ItemX];
									if( !item.item_no ) break;
									if( !g_MouseItemType &&  SubMenu->nField[j].fRectMouse ) 
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										PutItemIcon( SubMenu->x+gabx+15+35*ItemX, SubMenu->y+gaby+15+35*ItemY, item.item_no, 8, 2);
										ItemExplain(item);
									}
									break;
								}

		case FT_SKILL_PUT	:	{
									int first = SubMenu->nField[j].nWillDo;
									int second= g_pointMouseY - (SubMenu->y+SubMenu->nField[j].y);
									int third = g_pointMouseX - (SubMenu->x+SubMenu->nField[j].x);

									if( (second < 36) && (second > 2) ) second = 0;
									else if( (second > 37) && (second < 72 ) ) second = 1;
									else break;

									if( third < 35 )  third = 0;
									else if( third > 42 && third < 76 ) third = 1;
									else if( third > 83 && third < 117 ) third = 2;
									else if( third > 124 && third < 158 ) third = 3;
									else break;
									int s = SkillInventory[first][second][third];
									int s2 = SkillInventory2[first][second][third];

									if( !s || !skill[s].num ) break;
									if( SubMenu->nField[j].fRectMouse )
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
//										if( s )
//										{
//											if( main_interface.data[IF_MAP] )	skill[s].ExplainSkillWhenMapOn(SubMenu->x+SubMenu->nField[j].x+30+41*third, SubMenu->y+SubMenu->nField[j].y+30+second*36);
											CheckSkillExplain( s );
//										}
//										else if( s2 )
//										{
//											if( main_interface.data[IF_MAP] )	skill[s2].ExplainSkillWhenMapOn(SubMenu->x+SubMenu->nField[j].x+30+41*third, SubMenu->y+SubMenu->nField[j].y+30+second*36);
//											skill[s2].ExplainSkill();
//										}
									}
							
									if( SubMenu->nField[j].fLButtonDown || SubMenu->nField[j].fRButtonDown )
											PutSkillIcon(SubMenu->x+SubMenu->nField[j].x+15+41*third , SubMenu->y+SubMenu->nField[j].y+15+second*36, s, 8);
									break;
								}
		case FT_KNOWLEDGE_ICON_PUT :{
										int index=0;
										int skill_inv[3][3] = {0, };
										for( int start = 78; start < 87; start++)
										{
											if( SCharacterData.SkillId[start] )
											{
												*( (int *)skill_inv + index ) = start;
												PutSkillIcon( SubMenu->x+SubMenu->nField[j].x+15+( index%3 )*46, SubMenu->y+SubMenu->nField[j].y+15+( index/3 )*46, start );	// Ãâ·Â
											}
											index++;
											/*else 
											{
												*( (int *)skill_inv + index ) = start;
												PutSkillIcon( SubMenu->x+SubMenu->nField[j].x+15+( index%3 )*46, SubMenu->y+SubMenu->nField[j].y+15+( index/3 )*46, start , 8 ,2 );	// Ãâ·Â
												index++;
											}*/
										}
										memcpy( Skill_Know_Inv, skill_inv, sizeof( int[3][3] ) );
										int first = g_pointMouseY - (SubMenu->y+SubMenu->nField[j].y);
										int secound = g_pointMouseX - (SubMenu->x+SubMenu->nField[j].x);

										if( first < 35 )  first = 0;
										else if( first > 46 && first < 81 )  first = 1;
										else if( first > 92 && first < 127 ) first = 2;
										else break;

										if( secound < 35 )  secound = 0;
										else if( secound > 46 && secound < 81 )  secound = 1;
										else if( secound > 92 && secound < 127 ) secound = 2;
										else break;

										int s = skill[skill_inv[first][secound]].num;
										if( !s ) break;

										if( SubMenu->nField[j].fRectMouse )
										{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											CheckSkillExplain(s );
										}
										if( SubMenu->nField[j].fLButtonDown )
												PutSkillIcon( SubMenu->x+SubMenu->nField[j].x+15+secound*46, SubMenu->y+SubMenu->nField[j].y+15+first*46, s, 8 );	// Ãâ·Â
										break;
									}
		case FT_CHARACTER_ITEM :	{
										if( !SMenu[MN_ITEM].bActive ) 
										{
											MenuSoundClose( SubMenu->Id );
											break;
										}
										int para = SubMenu->nField[j].nSHideNomalStart;
										int a;
										for( a=0; a<15; a++)
											if( You.inv[15*para+a].item_no )
												PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15+35*(a%5), SubMenu->y+SubMenu->nField[j].y+15+35*(a/5), You.inv[15*para+a].item_no );

										if( SubMenu->nField[j].fRectMouse && !g_MouseItemType )
										{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											int ItemX = ( g_pointMouseX - (SubMenu->x + SubMenu->nField[j].x) ) / 35;
											int ItemY = ( g_pointMouseY - (SubMenu->y + SubMenu->nField[j].y) ) / 35;
																												
											int xCheck = ( g_pointMouseX - (SubMenu->x + SubMenu->nField[j].x) ) % 35;
											int yCheck = ( g_pointMouseY - (SubMenu->y + SubMenu->nField[j].y) ) % 35;

											if( (xCheck < 2) || (xCheck > 30) || (yCheck < 2) || (yCheck > 30 ) )  break;
											ItemAttr &item = You.inv[15*para+a];
//											if( !LButtonDown && item.item_no ) ItemExplain(item.item_no);
										}
										break;
									}
		case FT_MAKE_ITEM : {
								if( !SMenu[MN_ITEM].bActive ) 
								{
									SubMenu->bActive = false;
									MP3( SN_MENU_CLOSE );
									break;
								}
								for( int a=0; a<20; a++ )
								{
//									if( make_item_list[a].item_no ) PutItemIcon( SubMenu->x+make_item_list[a].x, SubMenu->y+make_item_list[a].y, make_item_list[a].item_no);
									if( make_item_list[a].item_no ) PutItemIcon( SubMenu->x+make_item_list[a].x, SubMenu->y+make_item_list[a].y, make_item_list[a].item_no,30,1 );
								}
								break;
							}
		case FT_MAKE_ITEM_OK : {
									if( !SMenu[MN_ITEM].bActive ) 
									{
										MenuSoundClose( SubMenu->Id );
										break;
									}
									int &st = SubMenu->nField[j].nSHideNomalCount;		// ÃÊ±â°ª 20
									int &st1 = SubMenu->nField[j].nSHideNomalStart;		// ÃÊ±â°ª 0
									int &st2 = SubMenu->nField[j].nShideNomalPlus;
									static bool turn = true;
									if( st )
									{
										for( int a=0; a<20; a++ )
										{
											if( make_item_list[a].item_no ) PutItemIcon( SubMenu->x+make_item_list[a].x, SubMenu->y+make_item_list[a].y, make_item_list[a].item_no, st, 1 );
										}
										st--;
									}
									else 
									{
//										if( st1 < 20 )
//										{
//											if( make_item_result.item_no ) PutItemIcon( SubMenu->x+make_item_result.x, SubMenu->y+make_item_result.y, make_item_result.item_no, st1, 1 );
//											st1++;
//										}
//										else
										{
											if( make_item_result.item_no ) 
											{
												PutItemIcon( SubMenu->x+make_item_result.x, SubMenu->y+make_item_result.y, make_item_result.item_no, st2, 1 );
											}
											if( turn ) 
											{
												st2++;
												if( st2 > 32 )	st2 = 32;
											}
											turn = !turn;
										}
									}
									
									if( st < 1 ) st = 0;

									break;
							   }

				case FT_ITEM_MAKE_SELECT :	{
										int &old_skillno = SubMenu->key;
										int &first = SubMenu->nField[j].nSHideNomalStart;

										if( !SubMenu->CheakType )
										{
											SubMenu->CheakType = 1;
											if( SkillNo != old_skillno )
											{
												old_skillno = SkillNo;
												first = 0;
											}
										}
										
										for( int a=0; a<10; a++ )
										{
											if( Item_Select[first][a].item_no )
											{
												PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15+37*(a%5), SubMenu->y+SubMenu->nField[j].y+15+30*(a/5), Item_Select[first][a].item_no, 15,2 );
											}
											else break;
										}

										ItemX = g_pointMouseX - (SubMenu->x+SubMenu->nField[j].x); 
										ItemY = g_pointMouseY - (SubMenu->y+SubMenu->nField[j].y);

										if( ItemX < 0 ) break;
										else if( ItemX < 32 ) ItemX = 0;
										else if( ItemX > 37 && ItemX < 69 ) ItemX = 1;
										else if( ItemX > 74 && ItemX < 106 ) ItemX = 2;
										else if( ItemX > 111 && ItemX < 143 ) ItemX = 3;
										else if( ItemX > 148 && ItemX < 180 ) ItemX = 4;
										else break;

										if( ItemY < 0 ) break;
										else if( ItemY < 32 ) ItemY = 0;
										else if( ItemY > 32 && ItemY < 64 ) ItemY = 1;
										else break;

										int item = Item_Select[first][ItemX+ItemY*5].item_no;

										if( !item) break;

										for(int ji = 0 ; ji < ITEM_JOIN_MAX ;ji++)
												{
													if( item_join[ji].item_id == item_select_result.item_no)
													{
														item_select_result.join_no = ji;
														break;
													}
												}
										if(SubMenu->nField[j].fLButtonDown )
										{
											PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15+37*ItemX, SubMenu->y+SubMenu->nField[j].y+15+30*ItemY, item, 8 );
											skillmakeiteminfo.MakeMuch = 0;	
											SMenu[MN_ITEM_MAKE_SELECT].nField[10].nWillDo = 0;
										}
										break;
									}
		case FT_ITEM_USE_SELECT :	{
										int first = SubMenu->nField[j].nSHideNomalStart;
										for( int a=0; a<10; a++ )
										{
											if( skill_use_item[first][a].item_no )
											{
												PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15+37*(a%5), SubMenu->y+SubMenu->nField[j].y+15+42*(a/5), skill_use_item[first][a].item_no, 15,2 );
											}
											else break;
										}

										ItemX = g_pointMouseX - (SubMenu->x+SubMenu->nField[j].x); 
										ItemY = g_pointMouseY - (SubMenu->y+SubMenu->nField[j].y);

										if( ItemX < 0 ) break;
										else if( ItemX < 32 ) ItemX = 0;
										else if( ItemX > 37 && ItemX < 69 ) ItemX = 1;
										else if( ItemX > 74 && ItemX < 106 ) ItemX = 2;
										else if( ItemX > 111 && ItemX < 143 ) ItemX = 3;
										else if( ItemX > 148 && ItemX < 180 ) ItemX = 4;
										else break;

										if( ItemY < 0 ) break;
										else if( ItemY < 32 ) ItemY = 0;
										else if( ItemY > 42 && ItemY < 74 ) ItemY = 1;
										else break;

										int item = skill_use_item[first][ItemX+ItemY*5].item_no;
										if( !item) break;
										if(SubMenu->nField[j].fLButtonDown )
										{
											PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15+37*ItemX, SubMenu->y+SubMenu->nField[j].y+15+42*ItemY, item, 8 );
										}
										break;
									}

		case FT_LEARN_ITEM : {
								int para = SubMenu->nField[j].nSHideNomalStart;
								int item = learn_item_list[para].item_no;
								int join = learn_item_list[para].join_no;
								
								if( item && GetItemAttr( item, IMAGE_NUMBER ) )
								{
									bool is_learn;
									if( FindLearnItem( item ) ) is_learn = true;		// ¹è¿î ¾ÆÀÌÅÛÀÎÁö ¾Æ´ÑÁö ¸ÕÀú ÆÇ´Ü.
									else is_learn = false;

									PutItemIcon( SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+15, item );
									
									Hcolor( FONT_COLOR_NUMBER );
									int line = 0;			// Ãâ·Â ÇÒ ¶óÀÎ ¼ö¸¦ Ä«¿îÆ® 
									if( is_learn )
									{
										if( item_join[join].resource1 )
										{
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+110+line*14, g_DestBackBuf, "%s %d", GetItemName( item_join[join].resource1 ), item_join[join].resource1_amount );
											line++;
											
											if( item_join[join].resource2 )
											{
												Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+110+line*14, g_DestBackBuf, "%s %d", GetItemName( item_join[join].resource2 ), item_join[join].resource2_amount );
												line++;

												if( item_join[join].resource3 )
												{
													Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+110+line*14, g_DestBackBuf, "%s %d", GetItemName( item_join[join].resource3 ), item_join[join].resource3_amount );
													line++;
													if( item_join[join].resource4 )
													{
														Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+110+line*14, g_DestBackBuf, "%s %d", GetItemName( item_join[join].resource4 ), item_join[join].resource4_amount );
													}
												}
											}
										}
										else 
										{
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+110, g_DestBackBuf, "%s", lan->OutputMessage(7,64));//010215 lsw
											line++;
										}

									}
									else 
									{
										Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+110, g_DestBackBuf, "%s", lan->OutputMessage(7,65));//010215 lsw
										line++;
									}

									if( item_join[join].need_skill1 )
									{
										Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+118+line*14, g_DestBackBuf, "%s  %d", skill[item_join[join].need_skill1].name, item_join[join].skill1_minimum );
										line++;
										if( item_join[join].need_skill2 )
										{
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+118+line*14, g_DestBackBuf, "%s  %d", skill[item_join[join].need_skill2].name, item_join[join].skill2_minimum );
											line++;
										}										
									}
									
									SetHangulAlign( TA_CENTER );			// ÀÌ¸§ Ãâ·Â
									Hcolor( FONT_COLOR_NAME );
									char *name = GetItemName( item );
									Hprint2( SubMenu->x+138, SubMenu->y+79, g_DestBackBuf, "%d. %s", para, name );
									
									ItemExplain( item );
									
									SetHangulAlign( TA_RIGHT );			// ¹è¿üÀ½, ¾È ¹è¿üÀ½
									Hcolor( RGB16( 255, 0, 0 ));
									Hprint2( SubMenu->x+217, SubMenu->y+65, g_DestBackBuf, "%s", is_learn?"KNOWN":"UNKNOWN" );

									SetHangulAlign( TA_RIGHT );
									Hcolor( FONT_COLOR_NUMBER );
									int price = 0;
									if( !is_learn ) price = item_join[join].tech_price;
									Hprint2( SubMenu->x+83+74, SubMenu->y+229, g_DestBackBuf, "%d", price );

									SetHangulAlign( TA_LEFT );
									Hcolor( RGB16( 255, 255, 255 ));
									if( item_join[join].explain[0] )
										RectTextPut( SubMenu->x+25, SubMenu->y+130+line*14, 186, item_join[join].explain );
								}
								break;
							 }
		case FT_HOME_EDITBOX : {	
									static int ct=-1;
									ct++;
									if(ct > 10)
									{
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
										if(ct > 20)	ct = 0;
									}
									break;
								}

		case FT_DIVIDE_ITEM_PUT :{
									int &much_sub = SMenu[MN_DIVIDE_ITEM].nField[0].nShideNomalPlus;
									DWORD much = i_DivideItem.item_attr.attr[IATTR_MUCH];
									if( !SubMenu->CheakType )
									{
										SetChatMode(CM_COMMAND);

										SubMenu->CheakType = 1;
										SetFocus2( HWND_3 );//021001 lsw
									}
								
									{
										char temp[20] = {0,};
										SetFocus2(HWND_3);
										EWndMgr.GetTxt( HWND_3, temp, 20 );
										EatRearWhiteChar( temp );
										much_sub = atol( temp );
										int refresh = 0;
										if( much_sub < 0 )
										{
											much_sub = 0;
											refresh = 1;
										} 
										if( much_sub > much )
										{
											much_sub = much;
											refresh = 1;
										}

										if( refresh || CheckNumber( temp, strlen( temp ) ) )
										{	
											sprintf( temp, "%d", much_sub );
											EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
										}
									}
									
									SetHangulAlign( TA_RIGHT );
									Hcolor( FONT_COLOR_DEFAULT );
									Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nWillDo, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%d/%d", much_sub, much );
									break;
								 }
		case FT_MAGIC_CLASS	:	{
									FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);

									if( SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fLButtonDown || SubMenu->nTemp == j)
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										Spr *s = GetSprOfMenu( SubMenu->nField[j].nImageType, SubMenu->nField[j].nImageNumber );
										if( s )
											PutCompressedImage(SubMenu->x+SubMenu->nField[j].x+15, SubMenu->y+SubMenu->nField[j].y+10, s );
									}
									break;
								}
		case FT_DSCROLL_BOOK:	{
									if( SubMenu->nField[j].nWillDo == 100 )
									{
										int img[] = { 965,971,970,969,968,967/*,966*/ };
										for( int i=0; i<6; i++ )
											if( SubMenu->nImageNumber == img[i] )
											{
												if( i==5 )
												{
													SubMenu->nImageNumber = img[0];
													SubMenu->nField[j].nWillDo = 0;
												}
												else SubMenu->nImageNumber = img[i+1];
												return;// ´ÙÀ½ ÇÊµå´Â Ã³¸®ÇÏÁö ¾Ê´Â´Ù.
											}
										break;
									}
									if( SubMenu->nField[SubMenu->nTemp].nSHideNomalStart - 2 < 0 ) 
									{
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
										break;
									}
									if( SubMenu->nField[j].fRectMouse )	
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										PrintQuickKey( "PGUP" );
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType );
									}
									if( SubMenu->nField[j].fLButtonDown )	FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
									break;
								}

		case FT_ISCROLL_BOOK:	{
									if( SubMenu->nField[j].nWillDo == 100 )
									{
										int img[] = { 965,966,967,968,969,970,971 };
										for( int i=0; i<7; i++ )
											if( SubMenu->nImageNumber == img[i] ) 
											{
												if( i==6 )
												{
													SubMenu->nImageNumber = img[0];
													SubMenu->nField[j].nWillDo = 0;
												}
												else SubMenu->nImageNumber = img[i+1];
												return;
											}
										break;
									}
									if( SubMenu->nField[SubMenu->nTemp].nSHideNomalStart + 2 >= SubMenu->nField[SubMenu->nTemp].nSHideNomalCount ) 
									{
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x-5, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
										break;
									}
									if( SubMenu->nField[j].fRectMouse )	
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										PrintQuickKey( "PGDN" );
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType );
									}
									if( SubMenu->nField[j].fLButtonDown )	FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x-5, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
									break;
								}
		case FT_LEARN_MAGIC	:	{
									// Learn ¹öÆ° Âï±â(¿ÞÂÊ)
									int willdo = SubMenu->nField[j].nWillDo;
									int para = SubMenu->nField[SubMenu->nTemp].nSHideNomalStart + willdo;// °è¿­´ç ¾ÆÀÌÅÛ + ¿ÞÂÊ, ¿À¸¥ÂÊ
									int magic_num = magic_by_class.magic[SubMenu->nTemp][para];
									if( !magic_num ) break;

//									if( !SCharacterData.MagicId[ magic_num ] )
									{
										if( SubMenu->nField[j].fLButtonDown || SubMenu->nField[j].fRectMouse )
										{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber );
										}
									}
									break;
									
								}
		case FT_ISCROLL_MEM_BOOK:{
									SubMenu->y = 50;
									if( SubMenu->nField[j].nWillDo == 100 )
									{
										
										int img[] = { 0,6,5,4,3,2,1 };
										int y[] = { 0,0,0,0,0,0,0 };			// y ÁÂÇ¥ÀÇ gab
										for( int i=0; i<6; i++ )
											if( SubMenu->nImageNumber == img[i] )
											{
												if( i==5 )
												{
													SubMenu->nImageNumber = img[0];
													SubMenu->nField[j].nWillDo = 0;
													SubMenu->y += y[i];
												}
												else 
												{
													SubMenu->nImageNumber = img[i+1];
													SubMenu->y += y[i+1];
												}
												return;// ´ÙÀ½ ÇÊµå´Â Ã³¸®ÇÏÁö ¾Ê´Â´Ù.
											}
										break;
									}
									int max;
									if( SCharacterData.nCharacterData[SPELL] ) max = 10;
									else max = 12;
									if( SubMenu->nTemp >= max ) 
									{
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x-5, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber );
										break;
									}
									if( SubMenu->nField[j].fRectMouse )	
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										PrintQuickKey( "PGUP" );
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage );
									}
									if( SubMenu->nField[j].fLButtonDown )	FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x-5, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber );
									break;
								}
		case FT_DSCROLL_MEM_BOOK:{
									SubMenu->y = 50;
									if( SubMenu->nField[j].nWillDo == 100 )
									{
										int img[] = { 0,1,2,3,4,5,6 };
										int y[] = { 0,0,0,0,0,0,0 };			// y ÁÂÇ¥ÀÇ gab
										for( int i=0; i<7; i++ )
											if( SubMenu->nImageNumber == img[i] ) 
											{
												if( i==6 )
												{
													SubMenu->nImageNumber = img[0];
													SubMenu->nField[j].nWillDo = 0;
													SubMenu->y += y[i];
												}
												else 
												{
													SubMenu->nImageNumber = img[i+1];
													SubMenu->y += y[i+1];
												}
												return;
											}
										break;
									}
									if( SubMenu->nTemp -1 < 0 ) 
									{
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber );
										break;
									}
									if( SubMenu->nField[j].fRectMouse )	
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										PrintQuickKey( "PGDN" );
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage );
									}
									if( SubMenu->nField[j].fLButtonDown )	FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber );
									break;
								}
		case FT_MEM_BOOK_ETC: {
								FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber );
								if( !SubMenu->CheakType )
								{
									SubMenu->CheakType = 1;
									SubMenu->nTemp = 0;				// °è¿­¹øÈ£
									SubMenu->work = 0;				// ¼±ÅÃ ¸¶¹ý ¹è¿­ ¹øÈ£..
									SetMemorizeArray( 0 );
								}
								
								break;
							  }
		case FT_MEM_MAGIC_ICON : {
									int num = SubMenu->nField[j].nWillDo;
									if( !memorize[num-1] ) break;

									if( MemorizeYet( memorize[num-1] ) ) 
									{
										SubMenu->nField[j].nSpecialWillDo = 1;
										PutMagicIcon( SubMenu->x+SubMenu->nField[j].x+20, SubMenu->y+SubMenu->nField[j].y+20, memorize[num-1], 16, 1);
									}
									else 
									{
										SubMenu->nField[j].nSpecialWillDo = 0;
										PutMagicIcon( SubMenu->x+SubMenu->nField[j].x+20, SubMenu->y+SubMenu->nField[j].y+20, memorize[num-1] );
									}
									if( SubMenu->work == num )
										PutMagicIcon( SubMenu->x+SubMenu->nField[j].x+20, SubMenu->y+SubMenu->nField[j].y+20, memorize[num-1], 8 );
									break;
								 }
		case FT_PARTY_FACE_PUT :{
									int num = SubMenu->nField[j].nWillDo;	// ÆÄÆ¼ ¹è¿­ ¹øÈ£
									CharacterParty	*character = &SCharacterData.party[num];
									int &ct = SubMenu->nField[j].nSHideNomalCount;
									int &join_prev = character->m_JoinPrev;
									int &delay = SubMenu->nField[j].nShideNomalPlus;
									int &con_prev = SubMenu->nField[j].nSpecialWillDo;
									int &delay2 = SubMenu->nField[j].nSHideNomalStart;					

//									if( !SubMenu->key )
//									{
//										for( i=0; i<6; i++ )
//											SearchMyPartyOfMySight( &SCharacterData.party[i]); // ÃÊ±â ½Ã¾ß±Ç¾È¿¡ ÆÄÆ¼ ÀÖ´ÂÁö Ã¼Å©..
//										SubMenu->key = 1;
//									}

									// »óÅÂ Ãâ·Â
									if( !SubMenu->CheakType )
									{
										SubMenu->CheakType =1;
										ct = 0;
										join_prev = character->m_Join;
										delay = 0;
										delay2 = 0;
										con_prev = GetIndexOfCondition( character->m_Condition );
									}

									if( character->m_Join )
										PutCharImage( SubMenu->x+3, SubMenu->y+3, character->m_ImageNo, 0, FS_MINI, 0, character->m_Gender );
									else PutCharImage( SubMenu->x+3, SubMenu->y+3, character->m_ImageNo, 8, FS_MINI, 1, character->m_Gender );
									if( join_prev != character->m_Join )
									{
										if( delay > 8 )
										{
											delay = 0;
											join_prev = character->m_Join;
											//MP3( ÆÄÆ¼ Á¢¼Ó ³»Áö ÇØÁ¦.. );
										}
										else if( delay%2 ) PutCharImage( SubMenu->x+3, SubMenu->y+3, character->m_ImageNo, 6, FS_MINI, 2, character->m_Gender );
										delay++;
									}	

									SetHangulAlign( TA_CENTER );			// ÀÌ¸§ Ãâ·Â..
#ifdef _DEBUG // finito 060507
									if(GetSysInfo( SI_GAME_MAKE_MODE )) Hprint2( SubMenu->x+17, SubMenu->y-12, g_DestBackBuf, "%d", character->m_Id);
#endif								
									Hcolor( FONT_COLOR_NAME );
									SetHangulFont( 3 );	//	10 font
									/*
									char name[11];
									strncpy( name, character->m_Name, 10 );		// ÇÑ±Û 6ÀÚ¸¸ º¸¿©ÁØ´Ù.
									name[10] = NULL;
									int start = 0;
									for( int i=10; i>=0; i-- )
									{
										if( name[i] & 0x80 ) start = i;		// ÇÑ±ÛÀÌ ½ÃÀÛµÇ´Â Ã³À½À» Ã£¾Æ³½´Ù.
										else if( start ) break;		// Àü¿¡ ÇÑ±ÛÀÌ ÀÖ´Â °÷ÀÌ ÀÖ´Âµ¥ ÇÑ±Û ¾Æ´Ñµ¥°¡ ³ª¿À¸é...
									}
									if( start )
									{
										int flag = 0;
										for( i=start;i<10 ;i++ )
										{
											if( name[i] & 0x80 ) flag++;
										}
										if( flag % 2) name[9] = NULL;
									}

									Hprint2( SubMenu->x+17, SubMenu->y-12, g_DestBackBuf, name );*/

									Hprint2( SubMenu->x+17, SubMenu->y+44+10*( (SubMenu->Id-MN_PARTY_FACE1)%2), g_DestBackBuf, "%s", character->m_Name);
									SetHangulFont( 2 );	//	12 font
									
									if( character->xy_on > 0 )		// ÁÂÇ¥Ãâ·Â
									{
										character->xy_on --;
										Hcolor( 255, 255, 255 );
										Hprint2( SubMenu->x+17, SubMenu->y-12, g_DestBackBuf, "(%3d,%3d)", character->x, character->y);
										
									}

									if( character->m_Sight )		// HP Ãâ·Â
									{
										if( character->ch->_HpMax > 0 ) 
										{
											int perCent = (100*character->ch->_Hp) / character->ch->_HpMax;
											int max = (33 * character->ch->_Hp) / character->ch->_HpMax;
											if(max > 33)
												max = 33;
											if(max < 0)
												max = 0;

											if( perCent > 70 )
											{
												//Green
												for( int a=0; a< max; a++ )
													FieldTypeNomalPut( SubMenu->x, SubMenu->y, a, 41, SubMenu->nField[j].nImageNumber );		// x ÃàÀÇ ÃÑ ±æÀÌ 35
											}
											else if( perCent > 35 )
											{
												//Orange
												for( int a=0; a< max; a++ )
													FieldTypeNomalPut( SubMenu->x, SubMenu->y, a, 41, SubMenu->nField[j].nImageNumber+1 );		// x ÃàÀÇ ÃÑ ±æÀÌ 35	// ¹ß°£»ö °ÔÀÌÁö ¹Ù
											}
											else
											{
												//Blink orange
												for( int a=0; a< max; a++ )
												{
													FieldTypeNomalPut( SubMenu->x, SubMenu->y, a, 41, SubMenu->nField[j].nImageNumber+1 );
													if( ct%2 ) FieldTypeNomalPutFx( SubMenu->x, SubMenu->y, a, 41, SubMenu->nField[j].nImageNumber+1, 8, 2 );	// ¹øÂ½ ¹øÂ½~
												}
												ct++;
											}
											
										}

										// Ä³¸¯ÅÍ »óÅÂÃ¢ Ç¥½Ã
										int con_to_img[] = { 1034, 1032, 1027,1030,1033,1028,1031,1026 };
										int con = GetIndexOfCondition( character->m_Condition );
										if( !character->ch->_Hp ) con = CC_DEATH;
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, 3, 3, con_to_img[con] );
										if( con != con_prev )
										{
											if( delay2 > 8 )
											{
												delay2 = 0;
												con_prev = con;
											}
											else if( delay2%2 ) FieldTypeNomalPutFx( SubMenu->x, SubMenu->y, 3, 3, con_to_img[con], 16, 2 );
											delay2++;
										}
									}
									break;
								}
		case FT_REVIVAL_BUTTON :
			{
				if( IsRightWindowOpen() ) return;
				if( Hero->viewtype != VIEWTYPE_GHOST_ ) return;		// Á×¾úÀ»¶§¸¸ ¶á´Ù.
				if( !g_FightMapStart )	// 031110 YGI
				{
					SmallMenuClose();
					return;
				}

				static int ct=-1;
				ct++;
				if(ct > 10)
				{
					::FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage,SubMenu->nField[j].nImageType);
					if(ct > 20)	ct = 0;
				}
				if(SubMenu->nField[j].fLButtonDown)
				{
					::FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage,SubMenu->nField[j].nImageType);
					::FieldTypeNomalPutFx3(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType, 16, 2);
				}
				break;
			}
		case FT_AWAKE_BUTTON : {
									memset( BrightBuf, 16, (GAME_SCREEN_XSIZE/2)* (GAME_SCREEN_YSIZE/2));
									PutMapBright2( g_DestBackBuf, BrightBuf );
									if( IsRightWindowOpen() ) return;

									static int ct=-1;
									ct++;
									if(ct > 20)
									{
										if( SCharacterData.sleep == CSC_SLEEP )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
										}
										else if( SCharacterData.sleep == CSC_REST )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
										}
										if(ct > 40)	ct = 0;
									}
									if(SubMenu->nField[j].fLButtonDown)
									{
										if( SCharacterData.sleep == CSC_SLEEP )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
											FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, 16, 2);
										}
										else if( SCharacterData.sleep == CSC_REST )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
											FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, 16, 2);
										}
									}
									break;
							   }
		case FT_FIRST_QUICK_KEY :	{
										QuickKeyFirst( SubMenu->Id );		// °¡Àå ¿ì¼±¼øÀ§
										break;
									}
			
		case FT_SMALL_MAP_HOUSE : {
									int x = SubMenu->x+SubMenu->nField[j].x;
									int y = SubMenu->y+SubMenu->nField[j].y;

									int &count = SubMenu->nField[j].nSHideNomalCount;
									int &flag = SubMenu->nField[j].nSHideNomalStart;

									if( count < 4 )
									{
										count = 4;
										flag = 1;
									}

									if( count > 10 ) 
									{
										flag = 0;
									}

									if( flag ) count++;
									else count--;

									if( SubMenu->nField[j].fRectMouse || LeftShiftOn ) 
									{
//										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										int gabx = x;
										int gaby = y-15;

										if( gaby < 0  ) 
										{
											gaby += 15;
											gabx += 30;
										}

										Hcolor( 255,255,255 );
										if( !LeftShiftOn || SubMenu->nField[j].fRectMouse )
											Hprint2( gabx, gaby, g_DestBackBuf, "%s", SubMenu->nField[j].temp );
										
										Spr *s = &small_map_spr[j];
										PutCompressedImageFX( x+s->ox+1, y+s->oy+1, s, count, 2 );

/*										if( j == SubMenu->nFieldCount-1 && SubMenu->key )		// ¸Ç¸¶Áö¸·ÀÏ °æ¿ì 
										{
											int para = SubMenu->key;

											x = SubMenu->x+SubMenu->nField[para].x;
											y = SubMenu->y+SubMenu->nField[para].y;
											gabx = x;
											gaby = y-15;
											if( gaby < 0  ) 
											{
												gaby += 15;
												gabx += 30;
											}
											Hcolor( 0,0,255 );											
											Hprint2( gabx, gaby, g_DestBackBuf, SubMenu->nField[para].temp );
											Spr *s = &small_map_spr[para];
											PutCompressedImageFX( x+s->ox+1, y+s->oy+1, s, count, 2 );
										}*/
									}

/*									if( j == 1 )		// ¸ÇÃ³À½ ÀÏ°æ¿ì 
									{
										SubMenu->key = 0;
									}

									if( SubMenu->nField[j].fRectMouse && LeftShiftOn )
										SubMenu->key = j;
	*/								
									break;
								  }
		case FT_USED_MAGIC_EXPLAIN :{
										if( Hero->ready_Magic && g_CurrUsedMagic ) // ½ÃÀüÁß
										{
											Spr *back_spr = GetSprOfMenu( MAIN_ETC, 55 );
											if( back_spr )
											{
												PutCompressedImageFX( GAME_SCREEN_XSIZE/2, SubMenu->y-52-back_spr->oy, back_spr, 15, 1 );
											//	if( g_CurrUsedMagic >= 150 ) g_CurrUsedMagic -= 150;
												const char *name = magic[g_CurrUsedMagic].GetName();
#ifdef CHINA_LOCALIZING_
												Hprint2( GAME_SCREEN_XSIZE/2 - back_spr->ox +3, SubMenu->y-52-back_spr->yl+3, g_DestBackBuf, "\"%s\" ×¼±¸Ê©·¨", name );
#else
	#if defined (TAIWAN_LOCALIZING_) || defined (HONGKONG_LOCALIZING_) 
												Hprint2( GAME_SCREEN_XSIZE/2 - back_spr->ox +3, SubMenu->y-52-back_spr->yl+3, g_DestBackBuf, "\"%s\" ·Ç³Æ¬Iªk", name );
	#else
												Hprint2( GAME_SCREEN_XSIZE/2 - back_spr->ox +3, SubMenu->y-52-back_spr->yl+3, g_DestBackBuf, "\"%s\" Casting", name );
	#endif
#endif
///////////////////////////////////////////////////////////////////////////////
												Hcolor( FONT_COLOR_NAME );
												Hprint2( GAME_SCREEN_XSIZE/2 - back_spr->ox +3, SubMenu->y-52-back_spr->yl+3, g_DestBackBuf, "\"%s\"", name );
											}
										}
										break;
									}

		case FT_QUICK_MAGIC_ARRAY_SCROLL :	{
												int willdo = SubMenu->nField[j].nWillDo;
												if( magic_plus_start+1 > magic_plus_count-5 && willdo == 0) 
												{
													break;
												}												
												if( magic_plus_start-5 < 0 && willdo == 1 ) 
												{
													break;
												}

												if( SubMenu->nField[j].fLButtonDown )
												{													
													FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType  );
												}
												else if( SubMenu->nField[j].fRectMouse )
												{
//													if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
													FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType  );
												}
												break;
											}
		case FT_TIME_DISPLAY_ON_OFF	:{
										if( SubMenu->nField[j].fLButtonDown )
										{
											FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType  );
										}
										break;
									 }
		case FT_CHECK_COMBO2 : 
		case FT_CHECK_COMBO : {
								int *lpValue = SubMenu->nField[j].nValue;
								int nWillDo = SubMenu->nField[j].nWillDo;
								if( *lpValue == nWillDo )
								{
									FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
								}
								if( SubMenu->nField[j].nType == FT_CHECK_COMBO2 )
								{
									int gabx = SubMenu->nField[j].nSHideNomalCount;
									int gaby = SubMenu->nField[j].nSHideNomalStart;
                  
									SetHangulAlign(TA_CENTER);
									Hprint2( SubMenu->x+SubMenu->nField[j].x+gabx+40, SubMenu->y+SubMenu->nField[j].y+gaby, 
										g_DestBackBuf, "%s", SubMenu->nField[j].temp );

								}
								break;
							  }
		case FT_VIEW_BBS_MONEY:	{
									int money = bbs_money[SubMenu->key][SubMenu->work];
									PrintMoney( SubMenu, j, money, SubMenu->nField[j].nRectImage );
									break;
								}
		case FT_WRITE_BBS : {
								if( !SubMenu->CheakType )
								{
									SetChatMode(CM_COMMAND);

									SubMenu->CheakType = 1;
									SetFocus2(HWND_1);//021001 lsw
								}
								int &count = SubMenu->nField[j].nSHideNomalCount;
								char *temp = SubMenu->nField[j].temp;
								EWndMgr.GetTxt( HWND_3, temp, 100);
								temp[99] = 0;//021202 lsw
								if( EWndMgr.IsFocus(HWND_3) && count%12 < 5 ) 
								{
									sprintf( temp, "%s_", temp );
								}
								RectTextPut(SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, temp, 18, SubMenu->nField[j].nImageNumber );
								count++;
							}break;
		case FT_WRITE_BBS_ITEM_NAME : 
			{//021001 lsw
				int &count = SubMenu->nField[j].nSHideNomalCount;
				char *temp = SubMenu->nField[j].temp;
				EWndMgr.GetTxt( HWND_1, temp, 30 );
				if( EWndMgr.IsFocus(HWND_1) && count%12 < 6 ) sprintf( temp, "%s_", temp );
				Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, temp );
				count++;
			}break;
		case FT_WRITE_BBS_ITEM_MONEY : 
			{
				int &count = SubMenu->nField[j].nSHideNomalCount;
				SetHangulAlign( TA_RIGHT );
				char *temp = SubMenu->nField[j].temp;
				EWndMgr.GetTxt( HWND_2, temp, 10 );
				DWORD temp_money = atol( temp );
				sprintf( temp, "%u", temp_money );
				EWndMgr.SetTxt(HWND_2,temp);//021001 lsw

				if( EWndMgr.IsFocus(HWND_2) && count%12 < 6 ) sprintf( temp, "%u_", temp_money );
				else sprintf( temp, "%u ", temp_money );
				
				Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, temp );
				count++;
			 }break;
		case FT_GUILD_INTRODUCTION	: {
										static int save_page = SubMenu->work;
										if( !SubMenu->CheakType )
										{
											int nation = Hero->name_status.nation;
											static int count_to_nation[] = { N_VYSEUS, N_ZYPERN, N_YILSE };
											if( !nation ) nation = count_to_nation[rand()%3];

											switch( nation )
											{
												case N_VYSEUS : SMenu[MN_GUILD_INTRODUCTION].nField[1].nImageNumber =41; break;
												case N_ZYPERN : SMenu[MN_GUILD_INTRODUCTION].nField[1].nImageNumber =43; break;
												case N_YILSE : SMenu[MN_GUILD_INTRODUCTION].nField[1].nImageNumber =42; break;
											}
											static int save_nation;
											//if( save_nation != nation )
											{
												save_nation = nation;
												SendReqGuildIntroduction( nation );
											}
											
											SubMenu->CheakType = 1;
											SubMenu->work = save_page;
										}
										save_page= SubMenu->work;
										if( !g_Guild_Introduction ) break;

										int &page_status = SubMenu->work;

										WORD page;
										WORD page_max;
										GetWORDOfDWORD( (DWORD)page_status, page, page_max );

										int continue_flag = 0;
										for( int a=0; a<g_Guild_Introduction->m_Count; a++ )
										{
											if( g_Guild_Introduction->m_Guild[a].page == page )
											{
												if( ( SubMenu->nField[j].fRectMouse )
													&& MouseInRectCheak( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_Guild_Introduction->m_Guild[a].rect, true ) )
												{
													Hcolor( FONT_COLOR_ORANGE );
													Hprint2( SubMenu->x+SubMenu->nField[j].x+g_Guild_Introduction->m_Guild[a].x,
														SubMenu->y+SubMenu->nField[j].y+g_Guild_Introduction->m_Guild[a].line*20,
														g_DestBackBuf, "%s", g_Guild_Introduction->m_Guild[a].m_szName );

													Hcolor( FONT_COLOR_NUMBER );
												}
												else
												{
													Hprint2( SubMenu->x+SubMenu->nField[j].x+g_Guild_Introduction->m_Guild[a].x,
														SubMenu->y+SubMenu->nField[j].y+g_Guild_Introduction->m_Guild[a].line*20,
														g_DestBackBuf, "%s", g_Guild_Introduction->m_Guild[a].m_szName );
												}

												continue_flag = 1;
											}
											else if( continue_flag ) break;
										}
										break;
									  }

			//acer4
		case FT_RETURN_FUCTION : 
			{
				int willdo = SubMenu->nField[j].nWillDo;
				switch( willdo )
				{
				case 1: PutFieldOtherCharInfo();
					break;
				}

				break;
			}


		case FT_DO :  //¹«Á¶°Ç ½ÇÇàÇØ¾ß ÇÒ¶§, ÇÑ¸¶µð·Î ¶ß³»±âµé ºÙÀÌ´Â °÷, case¹® Áßº¹ ÇÇÇØ¾ßÇÔ.
			switch(SubMenu->nField[j].nWillDo)
			{
				case DO_DIVIDE_SCROLL_UP :	{
												if( !SMenu[MN_ITEM].bActive ) 
												{
													MenuSoundClose( SubMenu->Id );
													break;
												}
												if( !SubMenu->nField[j].fRectMouse && !SubMenu->nField[j].fLButtonDown && !SubMenu->nField[j].fCheakFlag ) SubMenu->nField[0].nSHideNomalCount = 0; 
												if(SubMenu->nField[j].fRectMouse)
												{
//													if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
													FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
												}
												if(SubMenu->nField[j].fLButtonDown)
														FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
												break;
											}

				case DO_DIVIDE_SCROLL_DOWN :{
												if( !SubMenu->nField[j].fRectMouse && !SubMenu->nField[j].fLButtonDown && !SubMenu->nField[j].fCheakFlag ) SubMenu->nField[0].nSHideNomalStart = 0;		// ¹þ¾î³ª ÀÖÀ¸¸é ÁõÆø º¯¼ö¸¦ ÃÊ±âÈ­
												if(SubMenu->nField[j].fRectMouse)
												{
//													if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
													FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
												}
												if(SubMenu->nField[j].fLButtonDown)
														FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
												break;
											}
				case DO_BANK_SCROLL_UP :
				case DO_BANK_SCROLL_DOWN :
				case DO_BANK_TIME_SCROLL_UP :
				case DO_BANK_TIME_SCROLL_DOWN :
				case DO_BANK_SCROLL_LEFT :
				case DO_BANK_SCROLL_RIGHT :	if(SubMenu->nField[j].fRectMouse)
											{
//												if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
											}
											if(SubMenu->nField[j].fLButtonDown)
													FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
											break;
			
				case DO_LEARNSKILL_ICON : {
											int para = SubMenu->nField[ 6 ].nSHideNomalStart;
											SubMenu->nField[ 6 ].nImageNumber = motherSkillImageNo[para];
											SubMenu->nField[ 6 ].nImageType = motherSkillImageType[para];

											bool exp_on = true;
											int width = 194;		// °¡·Î ¼³¸í Ãâ·Â ±æÀÌ

											SetHangulAlign( TA_LEFT );
											Hcolor( FONT_COLOR_DEFAULT );
											switch( nSkillLearnResult )
											{
												case 1:		RectTextPut(SubMenu->x+23, SubMenu->y+163, 170 ,lan->OutputMessage(3,141) ); exp_on = false; break;//010215 lsw
												case 2:		RectTextPut(SubMenu->x+23, SubMenu->y+163, 170 ,lan->OutputMessage(3,142) ); exp_on = false;break;
												case 3:		break;//RectTextPut(SubMenu->x+23, SubMenu->y+163, 200 , "´ç½ÅÀº ÀÌ ±â¼úµéÀ» ÀÍÈ÷´Âµ¥ ¼º°øÇß½À´Ï´Ù"); exp_on = false;break;
												case 4:		RectTextPut(SubMenu->x+23, SubMenu->y+163, 170 ,lan->OutputMessage(3,143) ); exp_on = false;break;
												case 5:		RectTextPut(SubMenu->x+23, SubMenu->y+163, 170 ,lan->OutputMessage(3,144) ); exp_on = false; break;//010215 lsw
															break;
											}								

											int a=SubMenu->nField[ 6 ].nSHideNomalStart;
											int b=SubMenu->nField[j].nSHideNomalStart;
											int c;
											if( !SkillInventory[a][0][0] ) 
											{
												Hcolor( FONT_COLOR_PLUS );
												RectTextPut( SubMenu->x+23, SubMenu->y+158, width, lan->OutputMessage(3,145) );//010215 lsw
												break;
											}
											else
											{
												for( c=0; c<4; c++)
												{
													if( skill[ SkillInventory[a][b][c] ].num )
													{
														PutSkillIcon(SubMenu->x+SubMenu->nField[j].x+15+45*c , SubMenu->y+SubMenu->nField[j].y+15, SkillInventory[a][b][c]);
													}
													//else break;
												}
												SetHangulAlign( TA_RIGHT );
												Hprint2( SubMenu->x+85+74, SubMenu->y+229, g_DestBackBuf, "%d", SkillTable[SkillInventory[a][b][0]].money );
											}    

											c = g_pointMouseX - (SubMenu->x+SubMenu->nField[j].x);
											int skip = 0;
											if( c>0 && c<35 ) c = 0;
											else if( c>45 && c<80 ) c = 1;
											else if( c>90 && c<125 ) c = 2;
											else if( c>140 && c<175 ) c = 3;
											else skip = 1;

											if( !skip && SubMenu->nField[j].fRectMouse && skill[ SkillInventory[a][b][c] ].num )
											{
//												if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
												PutSkillIcon(SubMenu->x+SubMenu->nField[j].x+15+45*c , SubMenu->y+SubMenu->nField[j].y+15, skill[ SkillInventory[a][b][c] ].num, 8);
												if( !nSkillLearnResult ) 
												{
													exp_on = false;
													CheckSkillExplain(SkillInventory[a][b][c]);
//													skill[SkillInventory[a][b][c]].ExplainSkill(SubMenu->x+23, SubMenu->y+153, 160 );	// ½ºÅ³ ¼³¸í¹®
												}
												else nSkillLearnResult = 0;
											}
  
											if(exp_on)
											{
												SetHangulAlign( TA_LEFT );
												Hcolor( FONT_COLOR_NAME );
												Hprint2( SubMenu->x+22, SubMenu->y+157, g_DestBackBuf, "%s", lan->OutputMessage(3,146) );//010215 lsw

												Hcolor( FONT_COLOR_NUMBER );
//												if( skill[SkillInventory[a][b][0]].series == 1 )
												{
													char *text;
													switch( skill[SkillInventory[a][b][0]].inclusive )
													{
														case ANIMAL_LORE :	text = lan->OutputMessage(3,147); break;
														case TAIMING :		text = lan->OutputMessage(3,148); break;
														case COOKING :		text =  lan->OutputMessage(3,149); break;
														case BLACKSMITHY :	text = lan->OutputMessage(3,150) ; break;
														case CANDLEMAKING :	text = lan->OutputMessage(3,151) ; break;
														case CARPENTRY :		
														case BOWCRAFTING :	text =lan->OutputMessage(3,152) ; break;
														case TAILORING :	text = lan->OutputMessage(3,153) ; break;
														case ALCHEMING :	text =  lan->OutputMessage(3,154); break;
														default :			text = lan->OutputMessage(3,155); break;
													}
													RectTextPut( SubMenu->x+22, SubMenu->y+157+18, width, text ); 
												}
											}
											break;
										  }
				case DO_FACE_MENU : if( SubMenu->nField[j].fRectMouse ) 
									{
										if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										PrintQuickKey( "P" ); break;
									}break;
				default:
					{
						MenuSubProcessType_FT_DO(SubMenu,j);//020515 lsw
					}break;
			}
			break;

		case FT_MAGIC_BOOK_ETC:	{
									// EXIT ¹öÆ° Âï±â
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, -55, 273, 15, MAGIC_BOOK_MENU );
									if( !SubMenu->CheakType )
									{
										SubMenu->CheakType=1;	//	Ã³À½À¸·Î ¸Þ´º°¡ ¶ä
										SubMenu->nTemp = 1;		//	1°è¿­·Î ÃÊ±âÈ­
										GetDivedeMagicByClass( );		//magic_by_class º¯¼ö¸¦ ¼¼ÆÃÇÏ´Â ÇÔ¼ö
										for( int a=1; a<12; a++ )
										{
											int magic_num=0;
											if( GetNoLearnMagic( a, magic_num )==1000 )
											{
												SubMenu->nField[a].nSHideNomalCount = magic_by_class.max[a];
											}
											else
											{
												// °¢°¢ÀÇ ¸¶¹ý ¼ø¼­ ÃÊ±âÈ­ // +1Àº ¸ø¹è¿ì´Â ¸¶¹ý 1¸¦ º¸¿©ÁÖ±â À§ÇØ
												SubMenu->nField[a].nSHideNomalCount = magic_by_class.max[a]+1;
											}

											SubMenu->nField[a].nSHideNomalStart = 0;
										}
									}
									break;
								}

		case FT_OPTION_GAUGE : {
									int start=0;
									int gaby=31;
									float rate = 0.8f;
									for( start=0 ; start < rate*system_info.sound; start++)
											FieldTypeNomalPut(SubMenu->x+start, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									for( start=0 ; start < rate*system_info.music; start++)
											FieldTypeNomalPut(SubMenu->x+start, SubMenu->y+gaby*1, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									for( start=0 ; start < rate*system_info.mouse_speed; start++)
											FieldTypeNomalPut(SubMenu->x+start, SubMenu->y+gaby*2, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									for( start=0 ; start < rate*system_info.scroll_speed; start++)
											FieldTypeNomalPut(SubMenu->x+start, SubMenu->y+gaby*3, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);

									break;
							   }

		case FT_TOWN_CONSTRUCTION :	if(SubMenu->nField[j].nImageNumber)  //ÂïÁö ¾Ê°í ³Ñ¾î°¡·Á¸é ÀÌ¹ÌÁö ¹øÈ£¸¦ 0À¸·Î...
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									break;
		
		case FT_TEXT_BUTTON	: {
								int spwilldo = SubMenu->nField[j].nSpecialWillDo;		// ¸î¹øÂ° ±Û¾¾ Ãâ·ÂÀÎ°¡¸¦ À§ÇÑ... 1: 1¹øÂ°
								int willdo = SubMenu->nField[j].nWillDo;
								int &scroll = SubMenu->nField[willdo].nSHideNomalStart;

								if( !SubMenu->CheakType )	// Ã³À½ ½ÃÀÛÇÏ´Â °Å³Ä?
								{
									SubMenu->CheakType = 1;
									SubMenu->work = 0;
									scroll = 0;
								}
								else
								{
									static int flag = scroll;
									if( flag != scroll )		// ½ºÅ©·Ñ ¹öÆ°ÀÌ ´­·¯Á³³Ä?
									{
										SubMenu->work = 0;
										flag = scroll;
									}
								}
								
								if( spwilldo == SubMenu->work )		// ¹Ì¸® Å¬¸®ÇÑ °ÍÀº °è¼Ó º¸¿©ÁØ´Ù.
								{
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									break;
								}
								if( SubMenu->nField[j].fRectMouse )
								{
									if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
									static int tt;
									static int de =10;
									if( tt ) {	de ++;	if( de > 14 ) { tt = !tt; }	}
									else { de --; if( de < 7 ) { tt = !tt; } } 
									FieldTypeNomalPutFx(SubMenu->x,SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y , SubMenu->nField[j].nRectImage , de, 2 );
								}
								else if( SubMenu->nField[j].fLButtonDown ) FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);		// »ç°¢Çü Âï±â
								break;
							  }
		case FT_HEAL_TEXT_BUTTON :	{
										int spwilldo = SubMenu->nField[j].nSpecialWillDo;		// ¸î¹øÂ° ±Û¾¾ Ãâ·ÂÀÎ°¡¸¦ À§ÇÑ... 1: 1¹øÂ°
										int willdo = SubMenu->nField[j].nWillDo;
										int &scroll = SubMenu->nField[willdo].nSHideNomalStart;
										int cn = SubMenu->nField[willdo].nSHideNomalCount;		// ÃÖ´ë

										static int flag = scroll;
										if( flag != scroll )		// ½ºÅ©·Ñ ¹öÆ°ÀÌ ´­·¯Á³³Ä?
										{
											SubMenu->work = 0;
											flag = scroll;
										}

										if( scroll ) 
										{
											if( spwilldo > cn-3 ) break;
										}
										else
										{
											if( spwilldo > cn ) break;
										}

										if( spwilldo == SubMenu->work )		// ¹Ì¸® Å¬¸¯ÇÑ °ÍÀº °è¼Ó º¸¿©ÁØ´Ù.
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
											break;
										}
										if( SubMenu->nField[j].fRectMouse )
										{
											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											static int tt;
											static int de =10;
											if( tt ) {	de ++;	if( de > 14 ) { tt = !tt; }	}
											else { de --; if( de < 7 ) { tt = !tt; } } 
											FieldTypeNomalPutFx(SubMenu->x,SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y , SubMenu->nField[j].nRectImage , de, 2 );
										}
										else if( SubMenu->nField[j].fLButtonDown ) FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);		// »ç°¢Çü Âï±â
										break;
									}
		case FT_INN	:	if(SubMenu->nField[j].fRectMouse)
						{
							if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
							static int tt;
							static int de =10;
							if( tt ) {	de ++;	if( de > 14 ) { tt = !tt; }	}
							else { de --; if( de < 7 ) { tt = !tt; } } 
							FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, de, 2);
						}
						if(SubMenu->nField[j].fLButtonDown)
								FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
						break;

		case FT_VIEW_SMALL_MAP :{	//011004 lsw ¿ø»ó º¹±Í
									if( !ViewSmallMap2( SubMenu->x, SubMenu->y ) )
									{
										//SmallMenuClose( );
										SmallMapClose();
									}
									break;
								}
		case FT_PUT_DISTANCE :	{
									int willdo = SubMenu->nField[j].nWillDo;
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+7, SubMenu->nField[j].y+6, SubMenu->nField[j].nImageNumber+willdo);
									Hprint2( SubMenu->x+SubMenu->nField[j].x+39, SubMenu->y+SubMenu->nField[j].y+10, g_DestBackBuf,"%s",SubMenu->nField[j].temp );
									if( j==0 )
									{
										SubMenu->work--;
										if( SubMenu->work < 0 ) 
										{
											SubMenu->bActive = false;
											MP3( SN_TOUCH );
										}										
									}
									// °ËÀº ¸®º» ¶ç¿ì±â
									//FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+135, SubMenu->nField[j].y, 60, MAIN_ETC );
									break;
								}
		case FT_STORE_SIGN : {
								Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s",SubMenu->nField[j].temp );
								SubMenu->work--;
								if( SubMenu->work < 0 ) 
								{
									SubMenu->bActive = false;
									MP3( SN_TOUCH );
								}
								break;
							 }
		
		case FT_VIEW_GOD_MEETING : {
										if( !SubMenu->CheakType )
										{
											g_ServerText.m_cPrintCount = 0;
											SubMenu->key = 0;
											SubMenu->CheakType = 1;
										}

										if( !g_ServerText.m_pszMessage ) break;
										int &work = SubMenu->key;
										g_ServerText++;		// => m_cPrintCount++

										if( !g_ServerText.m_cPrintCount ) work = 0;
										if( !(g_ServerText.m_cPrintCount % 100) ) work++;

										SkipRectTextPut( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, (work - work%7), g_ServerText.m_pszMessage, work%7+1, 0 );	// 7ÁÙÀ» ±âÁØÀ¸·Î ÆäÀÌÁö¸¦ ³Ñ±ä´Ù.
										MP3( SN_GOD_MEETING, 1);										
										break;
								   }
		case FT_NO_CHECK :	{
								if( SubMenu->Id == MN_MAININTERFACE && j == 17 && SCharacterData.nLevel <= 8 )
								{
									static int flag = 0;
									if( flag%32 < 16 )
									{
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j+1].nRectImage, SubMenu->nField[j+1].nImageType );
									}

									flag++;
								}
								break;
							}
		case FT_GUILD_PUBLIC_NOTICE : 
			{
				int tab = SubMenu->key;
				if( tab ) break;	// ¸ÇÃ³À½ È­¸é¿¡¼­¸¸ Ã³¸® µÈ´Ù.
				int nWillDo = SubMenu->nField[j].nWillDo;
				if(SubMenu->nField[j].fLButtonDown)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
				}
				else if(SubMenu->nField[j].fRectMouse)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
				}
				if( !g_Menu_Variable.m_szGuildPublicNotics ) break;

				switch( nWillDo )
				{
				case 0 :	// ¾²±â // ³»¿ëÀ» º¸¿©ÁØ´Ù.
					if( EWndMgr.IsFocus(HWND_MAIL3) )
					{
						char temp[1024];
						EWndMgr.GetTxt( HWND_MAIL3, temp, 1000 );
						ChangeString( temp, '\r', ' ');
						int &count = SubMenu->nField[j].nSHideNomalCount;
						if( count%20 > 10 )
							RectTextPut( SubMenu->x+44, SubMenu->y+254, 370, temp, 18, 5 );
						else
						{
							sprintf( temp, "%s_", temp );
							RectTextPut( SubMenu->x+44, SubMenu->y+254, 370, temp, 18, 5 );
						}
						count ++;
					}
					else
					{
						RectTextPut( SubMenu->x+44, SubMenu->y+254, 370, g_Menu_Variable.m_szGuildPublicNotics, 18, 5 );
					}
				default : break;
				}
				break;
			}
		case FT_FRIEND_REGIST_NAME :
			{// Ä£±¸ µî·Ï
				Hcolor( 0 );
				Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, NULL, "%s", SubMenu->nField[j].temp );
				//Hcolor( FONT_COLOR_NAME );
				char temp[30] = {0,};
				SetFocus2( HWND_3 );//021001 lsw
				EWndMgr.GetTxt( HWND_3, temp, 20 );
				Hprint2( SubMenu->x+SubMenu->nField[j].x+35, SubMenu->y+SubMenu->nField[j].y, NULL, "%s", temp );
			}break;
		case FT_FILE_TEXT_PUT : 
			{
				if( g_Menu_Variable.m_szFileTextPut.empty() )
				//if( !g_Menu_Variable.m_szFileTextPut )
				{
					LoadFileText( SubMenu->nField[j].temp );
				}
				if( !g_Menu_Variable.m_szFileTextPut.empty() )
					RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage,(char *)g_Menu_Variable.m_szFileTextPut.c_str()); 

				break;
			}
		case FT_NATION_ITEM : 
			{
				int item_no = g_Menu_Variable.m_nation_item.item_no;
				if( !item_no ) break;

				static ItemAttr item;
				if( !SubMenu->CheakType )
				{
					item = GenerateItem( item_no );
					SubMenu->CheakType = 1;
				}
				PutItemIcon( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, item_no );
				if( SubMenu->nField[j].fRectMouse )
					ItemExplain( item );

				Hcolor( FONT_COLOR_NAME );
				SetHangulAlign( TA_CENTER );

				sprintf( SubMenu->nField[j].temp, "%d %s", g_Menu_Variable.m_nation_item.give_much, 
					g_Menu_Variable.m_nation_item.give_type?"Fame":g_szMoneyName );

				Hprint2( SubMenu->x+146, SubMenu->y+63, NULL, SubMenu->nField[j].temp );
				break;
			}
		case FT_GUILD_HOUSE :
			{
				CGuildHouseInfo *pData = g_Menu_Variable.m_pGuildHouseInfo;
				if( !pData ) break;

				int line_max = SubMenu->key;
				int &page_status = SubMenu->work;
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)page_status, page, page_max );

				if( !pData->m_max ) break;
				int x = SubMenu->x+SubMenu->nField[j].x;
				int y = SubMenu->y+SubMenu->nField[j].y;

				SetHangulAlign( TA_CENTER );
				int offset = page*line_max;
				for( int a=0; a<line_max && (offset+a)<pData->m_max; a++ )
				{
					Hprint2( x+52, y+a*18, NULL, "%d", pData->m_data[offset+a].id );
					char *mapname = GetMapNameByMapfile( pData->m_data[offset+a].map_name );
					if( mapname ) Hprint2( x+93, y+a*18, NULL, "%s", mapname );
					Hprint2( x+133, y+a*18, NULL, "%d", pData->m_data[offset+a].lv );
					int time = pData->m_data[offset+a].time;
					//Hprint2( x+188, y+a*18, NULL, "%s", GetGuildName( pData->m_data[offset+a].guild_code ) );
					
					if( !pData->m_data[offset+a].guild_code ) time = 0;

					Hprint2( x+254, y+a*18, NULL, "%d", time );
					// ¼ÒÀ¯ »óÈ²
					Hprint2( x+188, y+a*18, NULL, "%s", GetIsGotGuildHouse( time ) );
				}

				if( SubMenu->key2 != -1  )
				{
					FieldTypeNomalPut( x+25, y+SubMenu->key2*18-2, 0,0, 5, SubMenu->nField[j].nImageType);
				}
				// ¸¶¿ì½º ¼±ÅÃ Ã³¸®
				if( SubMenu->nField[j].fRectMouse )
				{
					y = SubMenu->y+SubMenu->nField[j].y;		// ½ÃÀÛ À§Ä¡
					int offset_y = g_pointMouseY - y;
					if( offset_y < 0 || offset_y >= 18 * line_max ) break;
					int para = offset_y/18;
					if( offset+para < pData->m_max )
						FieldTypeNomalPutFx3( x+25, y+para*18-2, 0,0, 5, SubMenu->nField[j].nImageType, 8, 2);
				}
				break;
			}
		case FT_GUILD_HOUSE_INFO :
			{
				int index = SubMenu->work;		// ¼±ÅÃÇÑ ±æµåÁý ÀÎµ¦½º ¹øÈ£
				// ¿¥Æ÷¸®¾Æ ±×¸² Âï±â
				CGuildHouseInfo *pData = g_Menu_Variable.m_pGuildHouseInfo;
				if( !pData ) break;
				int lv2image[] = { 0, 8, 9, 0, 0 };		// ·¹º§¿¡ µû¶ó ´Þ¶óÁü...
				if( lv2image[pData->m_data[index].lv] )
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, 34, 63, lv2image[pData->m_data[index].lv], SubMenu->nField[j].nImageType);
				
				// ½Ã±×³ÝÂï±â
				Hprint2( SubMenu->x+154, SubMenu->y+116, NULL, "%d", pData->m_data[index].blue );
				Hprint2( SubMenu->x+234, SubMenu->y+116, NULL, "%d", pData->m_data[index].red );
				
				// ¿¥Æ÷¸®¾Æ ¼³¸í Âï±â
				RectTextPut( SubMenu->x+35, SubMenu->y+153, 250, lan->OutputMessage(7, 173) );

				// ½ÅÃ» ÇÏ°Ú´Â°¡?
				Hprint2( SubMenu->x+31, SubMenu->y+268, NULL, "%s", kein_GetMenuString( 167 ) );

				SetHangulAlign( TA_CENTER );

				// ¼ÒÀ¯»óÈ²
				int time = pData->m_data[index].time;
				if( !pData->m_data[index].guild_code ) time = 0;
				Hprint2( SubMenu->x+215, SubMenu->y+70, NULL, "%s", GetIsGotGuildHouse( time ) );

				// ¼ÒÀ¯ ±æµå
				Hcolor( FONT_COLOR_NAME );
				Hprint2( SubMenu->x+215, SubMenu->y+86, NULL, "%s", GetGuildName( pData->m_data[index].guild_code ) );

				break;
			}
		
		case FT_HIDE_NOMAL_GROUP_PUT :
			for(i=SubMenu->nField[j].nSHideNomalStart; i<(SubMenu->nField[j].nSHideNomalStart)+(SubMenu->nField[j].nShideNomalPlus) && i<SubMenu->nField[j].nSHideNomalCount; i++)
			{
				if(SubMenu->nField[j].nType==FT_HIDE_NOMAL_GROUP_PUT) goto label_1; //GROUP´Â ¸ðµÎ º¸¿©ÁÖ±â¶§¹®¿¡ Ã¼Å©¸¦ ¾ÈÇØµµ µÈ´Ù
				if(SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].bShow) //ONE´Â Ã¼Å© µÈ°Í¸¸ º¸¿©ÁØ´Ù
				{
					label_1 :
					switch(SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nType)
					{
						case FT_TEXT_PUT :	FieldTypeTextPut(SubMenu->x, SubMenu->y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].x, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].temp);  
											break;

						case FT_TEXT_PUT_CENTER :
												{
													k=0;
													LenghtTotal=0;
													 //¹®ÀÚ¿­ ÀüÃ¼ ±æÀÌ¸¦ °è»ê
													while( (SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].temp[k]) != '\0' )
													{
														code=SHideNomal[SubMenu->nField[j].nSHideNomalNumber ][i].temp[k];
			 											if(code==32) LenghtTotal+=4;
														else
														{
															code-=27;
															LenghtTotal+=spr[code].xl;
														}
														k++;
													}
													 // Áß°£ºÎÅÍ ½ÃÀÛÇÏ±âÀ§ÇÑ ÁÂÇ¥ °è»ê
													Spr *s = GetSprOfMenu( SubMenu->nImageType, SubMenu->nImageNumber );
													if( s )
													{
														lenght=( s->xl-LenghtTotal)/2;
														FieldTypeTextPut(SubMenu->x, SubMenu->y, lenght, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].temp);
													}
													break;
												}
						case FT_TEXT_PUT_CENTER_X :
												{
													k=0;
													LenghtTotal=0;
													 //¹®ÀÚ¿­ ÀüÃ¼ ±æÀÌ¸¦ °è»ê
													while( (SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].temp[k]) != '\0' )
													{
														code=SHideNomal[SubMenu->nField[j].nSHideNomalNumber ][i].temp[k];
			 											if(code==32) LenghtTotal+=4;
														else
														{
															code-=27;
															LenghtTotal+=spr[code].xl;
														}
														k++;
													}
													// Áß°£ºÎÅÍ ½ÃÀÛÇÏ±âÀ§ÇÑ ÁÂÇ¥ °è»ê
													lenght=( SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].x - LenghtTotal)/2;
													FieldTypeTextPut(SubMenu->x, SubMenu->y, lenght, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].temp);  
													break;
												}

						case FT_NOMAL_PUT :		FieldTypeNomalPut(SubMenu->x, SubMenu->y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].x, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].nImageNumber);
												break;

						case FT_NOMAL_PUT_CENTER :	lenght= (spr[ SubMenu->nImageNumber ].xl - spr[ SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].nImageNumber ].xl)/2;  
			 										FieldTypeNomalPut(SubMenu->x, SubMenu->y, lenght, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].nImageNumber);
													break;

			 			case FT_INVENTORY :
							 //´ÜÁö .nSHideNomalCount  .nSHideNomalStart   .nSHideNomalPlus¸¸À» À§ÇÑ ºÎºÐÀÌ´Ù
							 break;
					}
				}
			}break;
			case  FT_NOMAL_PUT_BY_NWILLDO:
			{
				if(SubMenu->nField[j].nWillDo )//willdo¿¡ °ªÀÌ ÀÖ¾î¾ß Ãâ·Â
				{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
				}
			}
			break;
			//011008 lsw >
			case FT_NOMAL_PUT_BY_NWILLDO_CUSTOM:
			{
				if( SubMenu->nField[j].nWillDo )//willdo¿¡ °ªÀÌ ÀÖ¾î¾ß Ãâ·Â
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
					if(SubMenu->nField[j].nWillDo==2)
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
						if(SubMenu->nField[j].nWillDo ==1 )//ÀÌº¥Æ® ÀÖÀ½
						{
							FieldTypeNomalPutFx3(SubMenu->x,SubMenu->y,SubMenu->nField[j].x,SubMenu->nField[j].y,SubMenu->nField[j].nImageNumber, SubMenu->nImageType,iAlpha,2);
						}
					}
				}
				break;
			}
			//011014 lsw >

//<soto-030511
			case FT_WRITE_MERCHANT:
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

					char text[128] = {0,};
					int max = 0;
					int iHWndIndex = 0;//021001 lsw
					switch( willdo )
					{
						case 1: iHWndIndex = HWND_MAIL1; max = 20;   break;
						case 2: iHWndIndex = HWND_MAIL2; max = 20;  break;
						default: break;
					}

					EWndMgr.GetTxt(iHWndIndex, text, max );

					if(Auction.m_bChkKeyWord)
					{
						if(iHWndIndex == HWND_MAIL1)
						{
							strcpy(Auction.m_szKeyWord,text);
						}
					}
					else if(Auction.m_bChkMerchantName)
					{
						if(iHWndIndex == HWND_MAIL2)
						{
							strcpy(Auction.m_szMerchantName,text);
						}
					}

//					EWndMgr.SetTxt(iHWndIndex,text);
					
					ChangeString( text, '\r', ' ');

					if( SubMenu->work == willdo )
					{
						SetFocus2( iHWndIndex );
						if( count %16 > 8 ) 
						{
							sprintf( text, "%s_", text );
						}
						count++;
					}
					int &iFieldX	= SubMenu->nField[j].x;//soto_030511
					int &iFieldY	= SubMenu->nField[j].y;//soto_030511
					int &iRcImg		= SubMenu->nField[j].nRectImage;//soto_030511

					RectTextPut(iFieldX+iMainX+5, iFieldY+iMainY+15,iRcImg, text, 15, 15);
				}
				break;
			case FT_CHECK_MERCHANT_FIND:
				{
					if(SMenu[i].nField[j].nWillDo)
					{
						FieldTypeNomalPut(
							SMenu[i].x, SMenu[i].y,
							SMenu[i].nField[j].x + 6, SMenu[i].nField[j].y + 6,
							SMenu[i].nField[j].nImageNumber, SMenu[i].nField[j].nImageType
							);						
					}
				}
				break;
//>soto-030511
			default :
				{
					lsw_MenuSubProcessType(SubMenu,j);
					MenuSubProcessType2(SubMenu, j);	// 021022 kyo					
					break;
				}
		}

		CallKhMenuProc( SubMenu->Id, j );
	}
	SetHangulAlign( TA_LEFT );
	
}



//###################################################### ÁÖ¿ä ±â´É #############################################################
void DoRButtonDownOfMenu( int i, int j )
{
	switch( SMenu[i].nField[j].nType )
	{
		case FT_QUICK_MAGIC_PLUS : 
			{
				if( SCharacterData.nCharacterData[SPELL] != PRIEST_SPELL ) break;
				if( !magic_plus_count ) break;
				if( IsHeroDead() ) break;		// Á×¾úÀ» °æ¿ì Å¬¸¯µÇ¸é ¾ÊµÈ´Ù.

				int x_count = (g_pointMouseX - SMenu[i].nField[j].rCheakBox.left) / 44;
				int x_check = (g_pointMouseX - SMenu[i].nField[j].rCheakBox.left) % 44;
				if( x_check < 2 || x_check > 41 ) break;
				int para = magic_plus_start+x_count;
				if( !magic_plus[para] ) break;
				InsertMagicQuickArray( magic_plus[para] );			// »õ·Î Ãß°¡µÈ Äü ¸ÅÁ÷ ¹è¿­¿¡ ¼ÂÆÃ
				//DeleteMagicQuickArray( magic_plus[para] );			// ¸ÅÁ÷Ã¢¿¡¼­ ¸ÅÁ÷À» ¾ø¿£´Ù.
				break;
		   }
		default : 
			{
				lsw_DoRButtonDownOfMenu(i,j);//020420 lsw
				break;
			}
	}
	return;
}

void DoRButtonCheckOfMenu( int i, int j )
{
	int menu_num = i;		// i, menu_num¸¦ °°ÀÌ ¾´´Ù.
	switch( SMenu[i].nField[j].nType )
	{
	case FT_INVENTORY :	
		{
			if( SMenu[MN_BANK_CUSTODY].bActive ) break;			// ¹ðÅ© ¸Þ´º°¡ ¿­·Á ÀÖÀ¸¸é ´õÀÌ»ó ÁøÇàÇÏÁö ¾Ê´Â´Ù.
			int ItemX = ( g_pointMouseX - (SMenu[i].x + 19) ) / 35;
			int ItemY = ( g_pointMouseY - (SMenu[i].y + 210) ) / 35;
			if( ItemY >= 3 || ItemY < 0 || ItemX >= 8 || ItemX < 0 ) break;
			
			int xCheck = ( g_pointMouseX - (SMenu[i].x + 19) ) % 35;
			int yCheck = ( g_pointMouseY - (SMenu[i].y + 210) ) % 35;
			if( (xCheck < 2) || (xCheck > 30) || (yCheck < 2) || (yCheck > 30 ) )  break;
			int first_para = SMenu[i].nField[PAGE_NUMBER__].nSHideNomalStart;
			ItemAttr &item_attr = InvItemAttr[first_para][ItemY][ItemX];
			
			if( SMenu[MN_GAMBLE].bActive )	// ÀÛ¾÷´ë°¡ È°¼ºÈ­ µÆÀ» °æ¿ì ÀÛ¾÷´ë¿¡ ³õ¿© ÀÖ´Â ¾ÆÀÌÅÛÀº ¼±ÅÃµÇ¾îÁöÁö ¾Ê´Â´Ù.
			{
				int a;
				for(a=0; a<20; a++)
				{
					if( make_item_list[a].item_no && make_item_list[a].pos.type == INV
						&& make_item_list[a].pos.p1== first_para
						&& make_item_list[a].pos.p2 == ItemY && make_item_list[a].pos.p3 == ItemX )
						break;
				}
				if( a != 20 ) break;
			}
			//010708 lsw
			if( SMenu[MN_GM_2ND_SKILL].bActive )	// ÀÛ¾÷´ë°¡ È°¼ºÈ­ µÆÀ» °æ¿ì ÀÛ¾÷´ë¿¡ ³õ¿© ÀÖ´Â ¾ÆÀÌÅÛÀº ¼±ÅÃµÇ¾îÁöÁö ¾Ê´Â´Ù.
			{
				int a;
				for(a=0; a<20; a++)
				{
					if( gm_make_item_list[a].item_no && gm_make_item_list[a].pos.type == INV
						&& gm_make_item_list[a].pos.p1== first_para
						&& gm_make_item_list[a].pos.p2 == ItemY && gm_make_item_list[a].pos.p3 == ItemX)
						break;
				}
				if( a != 20 ) break;
			}
			if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
			{
				const POS pos = Auction.GetSellItemPos();
				if(pos.type == INV
				&& pos.p1 ==first_para
				&& pos.p2 == ItemY
				&& pos.p3 ==ItemX)
				{
					break;
				}
			}
			if( IsExchangeNow() )
			{
				int a;
				for(a=0; a<MAX_EXCHANGE_ITEM; a++)
				{
					if( item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type == INV
						&& item_give[a].item.item_pos.p1 == first_para
						&& item_give[a].item.item_pos.p2 == ItemY && item_give[a].item.item_pos.p3 == ItemX )
						break;
				}
				if( a != MAX_EXCHANGE_ITEM ) break;
			}
			
			POS pos_s;
			SetItemPos(INV, first_para, ItemY, ItemX, &pos_s);
			
			switch (item_attr.item_no)
			{ //< CSD-021003
				case 3105: // ÀüÅõ½Ã¼ú¼­
				{
					g_mgrBattle.SendCombatObtain(Hero, first_para, ItemX, ItemY, 1);
					break;
				}
				case 4028: // µà¾óÃ¼ÀÎÁö ¾ÆÀÌÅÛ
				{
					ItemSound(item_attr.item_no);
					g_mgrDual.SendDualEnable(Hero, first_para, ItemX, ItemY);
					break;
				}
				case 10290: //deveria ser o hammer, mas nao sei o codigo agora
				{
					CursorNo(160);
					hammer = 1;
					break;
				}
				case 10309:
				{
					CursorNo(160);
					magicItem = 1;
					break;		
				}
				default:
				{
					//qndo o cara clica com o botao direto do mouse ele passa por aqui !
					if (hammer == 1){
						hammer = 0;
						CItem *t = ::ItemUnit(item_attr.item_no);
						if (t->GetRButton() != DIVIDE_ITEM){
							CallOkCancelMessageBox(TRUE,0,0,"Do you want to restore this item to your full durability",TRUE);
							hammer_itemid = item_attr.item_no;
							//SetItemPos(INV, first_para, ItemY, ItemX, &hammer_itempos);															
							hammer_itempos = first_para*24+ItemY*8+(ItemX+1);					
							break;	
						}else{							
							UseItemByRbutton(pos_s, item_attr);
						}
						/*
						 * Rogério Segovia
						 * 06/05/2009
						 * Make the item clicked rare
						*/
					}else if (magicItem == 1){

						magicItem = 0;
						CItem *t = ::ItemUnit(item_attr.item_no);
						CallOkCancelMessageBox(TRUE,0,0,"Do you want to make this item rare? You may lose it!",TRUE);
						magicItem_itemid = item_attr.item_no;
						magicItem_itempos = first_para*24+ItemY*8+(ItemX+1);					

					}else{
						UseItemByRbutton(pos_s, item_attr);
					}
					break;
				}
			} //> CSD-021003
			
			break;
		}
	case FT_ACCEL_INVENTORY :
		{
			const int ItemX = ( g_pointMouseX - (SMenu[i].x + SMenu[i].nField[j].x) ) / 33;//020515 lsw
			if( 0 > ItemX || 5 <= ItemX)//020515 lsw
			{
				break;
			}
			ItemAttr &item_attr = QuickItemAttr[ItemX];
			
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
			
			LeftMenuClose();
			POS pos_s;
			SetItemPos(QUICK, ItemX, &pos_s);
			if( !UseItemByRbutton( pos_s, item_attr ) )	break;
			break;
		 }
	}
	SMenu[i].nField[j].fRButtonDown = false;
	SMenu[i].nField[j].fRButtonCheck = false;
}

void DoLButtonDownOfMenu( int i, int j )
{
	switch( SMenu[i].nField[j].nType )
	{
	case FT_INVENTORY :	
		{
			ButtonDownAtInventory(i,j);//021111 lsw
		}break;
	case FT_ARMOR_INVENTORY :{
								if(j == 24)// soto-1.02µà¾ó.
									break;
								int nWeapon_Type = SMenu[i].nField[j].nWillDo;   // ¹«±â, ¹æÆÐ, Çï¸ä, °©¿Ê, unique, neck, shoesÀÇ ÆÇ´Ü
								if( g_MouseItemType )	break;
								// ¾ÆÀÌÅÛ ¼±ÅÃÇÒ ¶§, Âï¾î³»±â Áï Àåºñ ÇØÁ¦
								if( EquipItemAttr[nWeapon_Type].item_no )
								{
									ItemSoundOfGetItem( EquipItemAttr[nWeapon_Type].item_no );

									g_MouseItemType=1;
									g_MouseItemNumber = EquipItemAttr[nWeapon_Type].item_no;
									SetItemPos( EQUIP, nWeapon_Type, &IP_base );
									HandItemAttr = EquipItemAttr[nWeapon_Type];
									DeleteItem( &EquipItemAttr[nWeapon_Type] );
									POS pos_t;
									SetItemPos(HAND, &pos_t);
									SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
									CallItemVirtualFunction( VF_RELEASE_ITEM, g_MouseItemNumber );
								}
								break;
							}
	case FT_BANK_INVENTORY :{
								int ItemX = ( g_pointMouseX - (SMenu[i].x+ 7) ) / 35;
								int ItemY = ( g_pointMouseY - (SMenu[i].y + 206) ) / 35;
																									
								int xCheck = ( g_pointMouseX - (SMenu[i].x + 7) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
								int yCheck = ( g_pointMouseY - (SMenu[i].y + 206) ) % 35;
								if( (xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ) )  
								{
									break;	
								}

                if( (3 > ItemY  && 0 <= ItemY )
										&& (6 > ItemX  && 0 <= ItemX ))//ÀÎµ¦½º °è»ê Ã¼Å©
								{
								  ItemAttr &item = BankItemAttr[SMenu[i].nField[0].nSHideNomalStart][ItemY][ItemX];
								  if( g_MouseItemType || !item.item_no) break;


								  ItemSoundOfGetItem( item.item_no );
								  SetItemPos(BANK, SMenu[i].nField[0].nSHideNomalStart,ItemY , ItemX, &IP_base);	//ÇöÀç À§Ä¡ ÀúÀå
								  g_MouseItemType=1;
								  g_MouseItemNumber = item.item_no;
								  HandItemAttr = item;
								  POS pos_t;
								  SetItemPos(HAND, &pos_t);
								  SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
								  DeleteItem( &item );
                }
								break;
							}
	case FT_CHARACTER_ITEM :	{ 
									int ItemX = ( g_pointMouseX - (SMenu[i].x + SMenu[i].nField[j].x) ) / 35;
									int ItemY = ( g_pointMouseY - (SMenu[i].y + SMenu[i].nField[j].y) ) / 35;
																										
									int xCheck = ( g_pointMouseX - (SMenu[i].x + SMenu[i].nField[j].x) ) % 35;
									int yCheck = ( g_pointMouseY - (SMenu[i].y + SMenu[i].nField[j].y) ) % 35;

									if( (xCheck < 2) || (xCheck > 30) || (yCheck < 2) || (yCheck > 30 ) )  break;
									ItemAttr &item = You.inv[ ItemY*5+ItemX ];

									if( g_MouseItemType )	break;

									if( item.item_no )
									{
										ItemSoundOfGetItem( item.item_no );
										g_MouseItemType=1;
										g_MouseItemNumber = item.item_no;

										HandItemAttr = item;
										POS pos_s, pos_t;
										SetItemPos(OTHER_CH, You.id, &pos_s);
										SetItemPos(HAND, &pos_t);
										int nItemNo = item.item_no;
										memset(&item, 0, sizeof(ItemAttr) );
										
										SendMoveItem( nItemNo, pos_s, pos_t );
									}
									break;
								}

	case FT_MAKE_ITEM :	{
							if( g_MouseItemType ) break;
							for( int a=0; a<20; a++ )
							{
								if( make_item_list[a].item_no && MouseInRectCheak( SMenu[i].x, SMenu[i].y, make_item_list[a].rect ) )
								{
									g_MouseItemType = 1;
									g_MouseItemNumber = make_item_list[a].item_no;

									HandItemAttr = make_item_list[a].attr;
									IP_base = make_item_list[a].pos;
									POS pos_t;
									SetItemPos(HAND, &pos_t);
									SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
									memset( &make_item_list[a], 0, sizeof( MakeItem ) );
									DeleteItem( &InvItemAttr[ IP_base.p1 ][ IP_base.p2 ][ IP_base.p3 ] );
									break;
								}
							}
							break;
						}
	case FT_MAKE_ITEM_OK:{
							if( g_MouseItemType ) break;
							
							if( make_item_result.item_no && MouseInRectCheak( SMenu[i].x, SMenu[i].y, make_item_result.rect ) )
							{
								g_MouseItemType = 1;
								g_MouseItemNumber = make_item_result.item_no;
								HandItemAttr = make_item_result.attr;

								POS pos_s, pos_t;
								SetItemPos(QUICK, 6, &pos_s );
								SetItemPos(HAND, &pos_t);
								SendMoveItem( QuickItemAttr[6].item_no, pos_s, pos_t );
								memset(&QuickItemAttr[6], 0, sizeof( ItemAttr ) );
								memset( &make_item_result, 0, sizeof( MakeItem ) );
								break;
							}
							break;
						}
	case FT_PARTY :	{
						if( SMenu[i].nField[j].DragCount > 10 )
						{
							int para = SMenu[i].nField[j].nWillDo;  // ¹è¿­ ÂüÁ¶¿¡ ¾²ÀÏ Ã·ÀÚ
							LpCharacterParty lpcharacter;				// <- ÆÄÆ¼¸¦ ¸ÎÀ» »ç¶÷¸¦ ÀúÀåÇÒ º¯¼ö

							switch( SMenu[i].nTemp )
							{
								case PARTY :		lpcharacter = &SCharacterData.party[para]; break;
								case RELATION :		lpcharacter = &SCharacterData.relation[para]; break;
								case EMPLOYMENT :	lpcharacter = &SCharacterData.employment[para]; break;
							}

							if( *lpcharacter->m_Name )
							{
								g_MouseItemType = PARTY;
								g_MouseItemNumber = lpcharacter->m_ImageNo;
								mouse_ch = *lpcharacter;

								SendDeleteParty( SMenu[i].nTemp, para );
								memset( lpcharacter, 0, sizeof( CharacterParty ) );
							}
						}

						break;
					}
	case MN_PARTY_FACE1 :{
							break;
						 }

	//case FT_HIDE_SPECIAL_WILLDO_DELAY_AUTO_PUT :
//	case FT_MAIN_MENU :
	case FT_PUT_BUTTON :
	case FT_SHOW_SPECIAL_WILLDO_AUTO_PUT ://010910 lsw
	case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT :
	case FT_HIDE_SPECIAL_WILLDO_PUT :
	case FT_SPECIAL_WILLDO :				
								switch( SMenu[i].nField[j].nSpecialWillDo )
								{
								case SWD_MAININTERFACE_STATUS :	{
																	if( SMenu[MN_PARTY].bActive && SMenu[i].nField[j].DragCount > 10 )
																	{
																		g_MouseItemType = PARTY;
																		g_MouseItemNumber = SCharacterData.nCharacterData[FACE];
																		mouse_ch.m_Id = Hero->id;
																		mouse_ch.m_ImageNo = SCharacterData.nCharacterData[FACE];
																		strcpy( mouse_ch.m_Name, SCharacterData.sCharacterName );
																		mouse_ch.m_Str = Hero_Ability[STR];
																		mouse_ch.m_Lv = SCharacterData.nLevel;
																		mouse_ch.m_Class = SCharacterData.nCharacterData[CLASS];
																		mouse_ch.m_Gender = SCharacterData.nCharacterData[GENDER];
																		break;
																	}
																}
																break;
								}
								break;
									
	case FT_DO :  //¹«Á¶°Ç ½ÇÇàÇØ¾ß ÇÒ¶§, ÇÑ¸¶µð·Î ¶ß³»±âµé ºÙÀÌ´Â °÷, case¹® Áßº¹ ÇÇÇØ¾ßÇÔ.
			switch( SMenu[i].nField[j].nWillDo )
			{
				case DO_DIVIDE_SCROLL_UP :	{
												int &temp = SMenu[i].nField[0].nSHideNomalCount;
												int &much = SMenu[i].nField[0].nShideNomalPlus;
												temp++;
												much += temp;
											
												if( much > (int) i_DivideItem.item_attr.attr[IATTR_MUCH] )
												{
													much = i_DivideItem.item_attr.attr[IATTR_MUCH] ;
													temp=0;
												}
												{
												char temp[20] = {0,};
												sprintf( temp, "%d", much);
												EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
												}
											}break;
				case DO_DIVIDE_SCROLL_DOWN :{
												int &temp = SMenu[i].nField[0].nSHideNomalStart;
												int &much = SMenu[i].nField[0].nShideNomalPlus;
												temp++;
												much -= temp;

												if( much < 1 ) 
												{
													much=0;
													temp = 0;
												}

												char szTxt[20] = {0,};//021001 lsw
												sprintf( szTxt, "%d", much);
												EWndMgr.SetTxt(HWND_3,szTxt);
											}break;
				case DO_BANK_SCROLL_UP :{
											static int count = 1;
											if( count == LButtonCount-1 ) count++;
											else 
											{
//												if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
												LButtonCount = 1;
												count = 1;
											}
											if(count > 5) nBankTemp += (count/50)? count * count * count/50 : count * count;
											
											if( nBankTemp >= 1000000000 ) nBankTemp = 1000000000;
											
											if((SMenu[i].Id == MN_BANK_DEPOSIT)  && ((DWORD)nBankTemp > SCharacterData.nMoney))
														nBankTemp = SCharacterData.nMoney;
											// 020925 YGI
											else if( (SMenu[i].Id == MN_BANK_DEPOSIT)  && ( nBankTemp + SCharacterData.BankMoney > 4000000000 ) )
											{
												if( SCharacterData.BankMoney > 4000000000 ) nBankTemp = 0;
												else
													nBankTemp = 4000000000 - SCharacterData.BankMoney;												
											}
											else if((SMenu[i].Id == MN_BANK_DEFRAYAL) && ((DWORD)nBankTemp > SCharacterData.BankMoney))
														nBankTemp = SCharacterData.BankMoney;
											else if(SMenu[i].Id == MN_BANK_LOAN) 
											{
												if( SCharacterData.LoanTotal < SCharacterData.LastLoan )
													nBankTemp = 0;
												else if( (DWORD)nBankTemp > ( SCharacterData.LoanTotal - SCharacterData.LastLoan)  )
														nBankTemp = SCharacterData.LoanTotal-SCharacterData.LastLoan;
											}
											else if(SMenu[i].Id == MN_BANK_REPAYMENT) 
											{
												if((DWORD)nBankTemp > SCharacterData.nMoney)
														nBankTemp = SCharacterData.nMoney;
												if(nBankTemp > SCharacterData.LastLoan)
														nBankTemp = SCharacterData.LastLoan;
											}
											else if( SMenu[i].Id == MN_SALVATION || SMenu[i].Id == MN_SALVATION2 )
											{
												if((DWORD)nBankTemp > SCharacterData.nMoney) 
													nBankTemp = SCharacterData.nMoney;
												//if( (nBankTemp + TotalSalvation) > MAX_SALVAITON_MONEY )
												//	nBankTemp = MAX_SALVAITON_MONEY - TotalSalvation;
											}
											

											char temp[20] = {0,};
											sprintf( temp, "%d", nBankTemp);
											EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
										}break;
				case DO_BANK_SCROLL_DOWN :{
											static int count = 1;
											if( count == LButtonCount-1 ) count++;
											else 
											{
												LButtonCount = 1;
												count = 1;
											}
											if(count > 5) nBankTemp -= (count/50)? count * count * count/50 : count * count;
											if(nBankTemp < 1 ) nBankTemp = 0;
											char temp[20] = {0,};
											sprintf( temp, "%d", nBankTemp);
											EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
										}break;

				case DO_BANK_TIME_SCROLL_UP :{
												static int count = 1;
												if( count == LButtonCount-1 ) count++;
												else 
												{
//													if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
													LButtonCount = 1;
													count = 1;
												}
												if(count > 5) nBankTemp += (count/100)? count * count * count/100 : count * count;
												if(nBankTemp < 1 ) nBankTemp = 0;
												break;										 
											 }

				case DO_BANK_TIME_SCROLL_DOWN :{
													static int count = 1;
													if( count == LButtonCount-1 ) count++;
													else 
													{
//														if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
														LButtonCount = 1;
														count = 1;
													}
													if(count > 5) nBankTemp += (count/100)? count * count * count/100 : count * count;
													break;
											   }

				case DO_BANK_SCROLL_LEFT :	break;
				case DO_BANK_SCROLL_RIGHT :	break;
										   
				case DO_FACE_MENU :		//if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
										SMenu[MN_FACE].bActive=TRUE;
										y_MenuFocus=MN_FACE;			//¸¶¿ì½º ÀÌº¥Æ®¸¦ µ¶Á¡ÇÑ´Ù.
										break;
			}
			break;
		case FT_SYSTEM_OPTION_SCROLL :	{
											int num = LButtonCount;
											//if(num > 3)
											{
												num /= 10;
												switch(j)
												{
													case 1: system_info.sound-=num*num;		if(system_info.sound < 0 )		system_info.sound=0; break;	//sound¿Þ
													case 2:	system_info.sound+=num*num;		if(system_info.sound >= 100 )	system_info.sound=100; break;	//sound¿À¸¥
													case 3:	system_info.music-=num*num;		if(system_info.music < 0 )		system_info.music=0; break;	//music¿Þ
													case 4:	system_info.music+=num*num;		if(system_info.music >= 100 )	system_info.music=100; break;	//music¿À¸¥
													case 5:	system_info.mouse_speed-=num*num;	if(system_info.mouse_speed < 0 )	system_info.mouse_speed=0; break;	//mouse¿Þ
													case 6:	system_info.mouse_speed+=num*num;	if(system_info.mouse_speed >= 100 )	system_info.mouse_speed=100; break;	//mouse¿À¸¥
													case 7:	system_info.scroll_speed-=num*num; if(system_info.scroll_speed < 0 )  system_info.scroll_speed=0; break;	//scroll¿Þ
													case 8:	system_info.scroll_speed+=num*num; if(system_info.scroll_speed >= 100 ) system_info.scroll_speed=100; break;	//scroll¿À¸¥
												}
											}
											break;
										}
		case FT_MENU_MOVE_DIRECT : 	{							// ¸Þ´º°¡ ¿òÁ÷ÀÎ´Ù~~
										if( SMenu[i].nTemp ) break;
										if( IsRightWindowOpen() && g_pointMouseX > 321+GABX_SCREEN ) break;
										if( IsLeftWindowOpen() && g_pointMouseX < 321  ) break;

										SMenu[i].nTemp = true;
										SMenu[i].key = j;		// ÇÑ ¸Þ´º¿¡ µÎ°³ÀÇ FT_MENU_MOVE_DIRECT°æ¿ì ´ëºñ
										SMenu[i].nField[j].nSHideNomalCount = abs(g_pointMouseX - SMenu[i].x);
										SMenu[i].nField[j].nSHideNomalStart = abs(g_pointMouseY - SMenu[i].y);
										SMenu[i].nField[j].nShideNomalPlus = SMenu[i].x * 1000 + SMenu[i].y;
										break;
									}
		case FT_MENU_MOVE_DEFAULT :
		case FT_MENU_MOVE : {							// ¸Þ´º°¡ ¿òÁ÷ÀÎ´Ù~~
								if( SMenu[i].nTemp ) break;
								SMenu[i].nTemp = true;
								SMenu[i].work = 0;
								SMenu[i].nField[j].nSHideNomalCount = abs(g_pointMouseX - SMenu[i].x);
								SMenu[i].nField[j].nSHideNomalStart = abs(g_pointMouseY - SMenu[i].y);
								break;
							}
		default:{ lsw_DoLButtonDownOfMenu(i,j); }break;//020214 lsw
	}
}

int SoundOn;
void DoButtonCheckOfMenu( int i, int j )
{
	MouseDrag = FALSE;
	SkillOn = 1; // Add by rogerio
	SoundOn = true;
	SMENU *SubMenu = &SMenu[i];
	switch( SMenu[i].nField[j].nType )
	{
		case FT_HIDE_WILLDO_AUTO_PUT_MAGIC :
		case FT_HIDE_WILLDO_PUT :
		case FT_WILLDO_PUT :	SMenu[i].bActive=FALSE;  //ÀÚ½ÅÀº »ç¶óÁü
								SMenu[SMenu[i].nField[j].nWillDo].bActive=TRUE;  //´Ù¸¥¸Þ´º È°¼ºÈ­
								MP3( SN_MENU_OPEN );
								break;
		case FT_INVENTORY :	{
								int ItemX = ( g_pointMouseX - (SMenu[i].x + 19) ) / 35;
								int ItemY = ( g_pointMouseY - (SMenu[i].y + 210) ) / 35;
								if( ItemY >= 3 || ItemY < 0 || ItemX >= 8 || ItemX < 0 ) break;
																									
								int xCheck = ( g_pointMouseX - (SMenu[i].x + 19) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
								int yCheck = ( g_pointMouseY - (SMenu[i].y + 210) ) % 35;
								if( (xCheck < 2) || (xCheck > 30) || (yCheck < 2) || (yCheck > 30 ) )  break;	

								int first = SMenu[i].nField[PAGE_NUMBER__].nSHideNomalStart;
								int second = ItemY;
								int third = ItemX;
								ItemAttr &item = InvItemAttr[first][second][third];

								if( g_MouseItemType != 1)
								{
									if( !item.item_no)//»óÀÎ°Å·¡½Ã¿¡´Â ´õºíÅ¬¸¯ ¾È¸Ô¾î
									{break;}
									if( ::IsExchangeNow() )		// ±³È¯ÁßÀÎ ¹°°ÇÀº ¿Å±æ¼ö ¾ø´Ù.
									{
										int a;
										for(a=0; a<MAX_EXCHANGE_ITEM; a++)
										{
											if( item_give[a].item.item_attr.item_no 
											&&  item_give[a].item.item_pos.type == INV
											&&  item_give[a].item.item_pos.p1 == first
											&&  item_give[a].item.item_pos.p2 == ItemY 
											&&  item_give[a].item.item_pos.p3 == ItemX )
											{
												MP3( SN_WARNING ); break;
											}
										}
										if( a != MAX_EXCHANGE_ITEM ) break;

										for( a=0; a<MAX_EXCHANGE_ITEM; a++ )	// ±³È¯Ã¢À¸·Î ÀÚµ¿ ±¸Çö
										{
											if( !item_give[a].item.item_attr.item_no )
											{
												::SetItemPos( INV, first, second, third, &IP_base );
												::SetExchageItemData(a,item,IP_base,true);//021121 lsw
												break;
											}
										}
										break;
									}
									else if( SMenu[MN_BANK_CUSTODY].bActive )		// ÀºÇàÀ¸·Î ÀÚµ¿ÀÌµ¿
									{
										int a, b, c;
										int ret = SearchEmptyBank( a, b, c );
										if( ret == 0 ) break;		// ²ËÃ¡´Ù.

										POS pos_s, pos_t;
										SetItemPos(INV, first, second, third, &pos_s);
										SetItemPos(BANK, a, b, c, &pos_t);
										SendMoveItem( item.item_no, pos_s, pos_t );
										BankItemAttr[a][b][c] = item;
										DeleteItem( &item );
										break;
									}
									else if( SMenu[MN_GAMBLE].bActive )		// ÀÛ¾÷´ë·Î ÀÚµ¿ÀÌµ¿ ±¸Çö
									{
										int is_use = false;
										int a;
										for(a=0; a<20; a++)		// ÇÑ¹ø »ç¿ëµÇ¾îÁø Àç·áµé
										{
											if( make_item_list[a].item_no && make_item_list[a].pos.type == INV
												&& make_item_list[a].pos.p1== first
												&& make_item_list[a].pos.p2 == second && make_item_list[a].pos.p3 == third)
											{
												is_use = true;
												break;
											}
										}
										if( is_use ) break;

										int is_full = true;
										for( a=0; a<20; a++ )
										{
											if( !make_item_list[a].item_no ) 
											{
												is_full = false;
												break;
											}
										}
										if( is_full ) break; // ²Ë Ã¡´Ù. 

										short int centerX = 50;
										short int centerY = 175;

										make_item_list[a].item_no = item.item_no;
										make_item_list[a].x = centerX+(a%7)*30+15;
										make_item_list[a].y = centerY-7+((a/7))*30 ;
										SetRect( &make_item_list[a].rect, make_item_list[a].x-15,make_item_list[a].y-15,make_item_list[a].x+15,make_item_list[a].y+15 );
										make_item_list[a].attr = item;
										SetItemPos(INV, first, second, third, &IP_base);
										make_item_list[a].pos = IP_base;		// ÀÎº¥Åä¸® ³»ÀÇ Àå¼Ò¸¦ ±â¾ïÇÑ´Ù.
										break;
									}

									// Æ÷¼Ç ÀÚµ¿ÀÌµ¿ ±¸Çö
									static int not_menu[] = {
														MN_EXCHANGE_ITEM,														
														MN_DIVIDE_ITEM,
														MN_BANK_CUSTODY,
														MN_ITEM_MAKE,
														MN_ITEM_MAKE_SELECT,
														MN_ITEM_MAKE_OK, 
														MN_GM_2ND_SKILL,//010708 lsw
														MN_CHARACTER_ITEM,
														MN_ITEM_SELECT,
														MN_GAMBLE,
														MN_MERCHANT_REGISTER,};
									int not_menu_max = sizeof( not_menu ) / sizeof( int );

									int ret = 0;
									for( int a=0; a<not_menu_max ; a++ )
										if( SMenu[not_menu[a]].bActive ) 
										{
											ret = 1;
											break;
										}
									if( ret ) break;

									int equip_pos = GetItemAttr(item.item_no, WEAR_ABLE);
									if( equip_pos == WEAR_BELT ) 
									{
										int quick_para = SearchEmptyQuickItem();	// ºñ¾îÀÖ´Â ÄüÀÎº¥Ã¢À» Ã£°í
										if( quick_para == -1 ) break;

										QuickItemAttr[quick_para] = item;
										memset( &item, 0, sizeof( ItemAttr ) );

										POS pos_s, pos_t;
										SetItemPos(INV, first, second, third, &pos_s);
										SetItemPos(QUICK, quick_para, &pos_t);
										SendMoveItem( QuickItemAttr[quick_para].item_no, pos_s, pos_t );
									}
									else if( GetItemAttrWeapon(item.item_no) )		// ¹«±â Ã¢À¸·Î ÀÚµ¿ÀÌµ¿
									{
										POS pos_s, pos_t;
										int equip_para;
										int is_equip_able = 1;
										switch( GetItemAttr(item.item_no, WEAR_ABLE) )
										{
											case WEAR_TWO_HAND	:	if( EquipItemAttr[WT_SHIELD].item_no ) is_equip_able = 0;
											case WEAR_RIGHT_HAND:	equip_para = WT_WEAPON; break;
											case WEAR_LEFT_HAND	:	if( GetItemAttr(EquipItemAttr[WT_WEAPON].item_no, WEAR_ABLE) == WEAR_TWO_HAND ) is_equip_able = 0;
																	equip_para = WT_SHIELD;
																	break;
											case WEAR_HEAD		:	equip_para = WT_HELMET; break;
											case WEAR_BODY		:	equip_para = WT_ARMOR; break;
											case WEAR_NECK		:	equip_para = WT_NECK; break;
											case WEAR_SHOES		:	equip_para = WT_SHOES; break;
											case WEAR_UNIQUE	:	equip_para = (EquipItemAttr[WT_UNIQUE1].item_no)?WT_UNIQUE2:WT_UNIQUE1; break;
											default : is_equip_able=0; break;
										}
										if( is_equip_able )
										{
											SetItemPos(INV, first, second, third, &pos_s);
											SetItemPos(EQUIP, equip_para, &pos_t);
											SendMoveItem( item.item_no, pos_s, pos_t );

											CallItemVirtualFunction( VF_RELEASE_ITEM, EquipItemAttr[equip_para].item_no );
											CallItemVirtualFunction( VF_EQUIP_ITEM, item.item_no );

											EquipItemAttr[equip_para] = item;											
											memset( &item, 0, sizeof( ItemAttr ) );
											ChangeEquip( Hero, EquipItemAttr, 0 );
										}
										else
										{
											MP3( SN_WARNING );
										}
									}
									break;
								}

								if( SMenu[MN_GAMBLE].bActive )		// ÀÛ¾÷´ë°¡ ¶¸À» °æ¿î Àç·á´ë ÇÏ°í¸¸ ¿Ô´Ù°¬´Ù°¡ °¡´ÉÇÏ´Ù.
								{
									MP3( SN_WARNING );// ¸¶¿ì½º °æ°íÀ½
									break;
								}
								//010708 lsw
								if( SMenu[MN_GM_2ND_SKILL].bActive )		// ÀÛ¾÷´ë°¡ ¶¸À» °æ¿î Àç·á´ë ÇÏ°í¸¸ ¿Ô´Ù°¬´Ù°¡ °¡´ÉÇÏ´Ù.
								{
									MP3( SN_WARNING );// ¸¶¿ì½º °æ°íÀ½
									break;
								}
								if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
								{
									MP3( SN_WARNING );// ¸¶¿ì½º °æ°íÀ½
									break;
								}


								if( item.item_no )
								{
									if( IsExchangeNow() )		// ±³È¯ÁßÀÎ ¹°°ÇÀº ¿Å±æ¼ö ¾ø´Ù.
									{
										int a;
										for(a=0; a<MAX_EXCHANGE_ITEM; a++)
										{
											if( item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type == INV
												&& item_give[a].item.item_pos.p1== first
												&& item_give[a].item.item_pos.p2 == ItemY && item_give[a].item.item_pos.p3 == ItemX )
											{
												MP3( SN_WARNING ); break;
											}
										}
										if( a != MAX_EXCHANGE_ITEM ) break;
									}
									int kind1 = GetItemAttr(HandItemAttr.item_no, ITEM_KIND);
									int kind2 = GetItemAttr(item.item_no, ITEM_KIND);
									if( ( kind1 == IK_NEW_MONEY && kind2 == IK_NEW_MONEY )
									|| ( kind1 == IK_MONEY &&  kind2 == IK_MONEY )	// µ¿Àü°ú ¹¶Ä¡µ·µµ ÇÕÃÄÁ®¾ßÇÑ´Ù.
									|| ( item.item_no==HandItemAttr.item_no && item.attr[IATTR_RARE_MAIN] == HandItemAttr.attr[IATTR_RARE_MAIN] 
										&& GetItemAttr( item.item_no, DO_RBUTTON )==DIVIDE_ITEM )					// ³ª´­¼ö ÀÖ´Â ¹°°ÇÀº ÇÕÃÄ¾ßÇÑ´Ù.
									// Æ÷¼ÇÀ» ¾ÆÀÌÅÛ ÁÖ¸Ó´Ï·Î ¿Å±â±â 
									// µé°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ¸ÔÀ» ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÌ°í ÁÖ¸Ó´Ï ¼Ó¼ºÀº ¾Æ´Ï¸é¼­ ´ë»ó ¾ÆÀÌÅÛÀÌ ÁÖ¸Ó´Ï ÀÏ°æ¿ì									
									|| ( GetItemAttr( HandItemAttr.item_no, DO_RBUTTON ) == USE_ITEM &&
										kind1 != IK_POTION_BOX && kind2 == IK_POTION_BOX ) )
									{
										int item_no, much;
										GetItemByPotionBox( item_no, much );		// 020527 YGI
										
										if( ( much<50 ) && ( !much || item_no == HandItemAttr.item_no ) )
										{
											POS pos_s, pos_t;
											SetItemPos(HAND, &pos_s);
											SetItemPos(INV, first, ItemY, ItemX, &pos_t);
											SendMoveItem( HandItemAttr.item_no, pos_s, pos_t );
											g_MouseItemNumber=0;
											g_MouseItemType=0;
											DeleteItem( &HandItemAttr );
											break;
										}
									}

									ItemAttr attr_temp = item;
									item = HandItemAttr;
									HandItemAttr = attr_temp;

									g_MouseItemType=1;
									g_MouseItemNumber = HandItemAttr.item_no;
									POS pos_s, pos_t;
									SetItemPos(HAND, &pos_s);
									SetItemPos(INV, first, ItemY, ItemX, &pos_t);
									SendMoveItem( item.item_no, pos_s, pos_t );
								}
								else
								{
									item = HandItemAttr;

									g_MouseItemType=0;
									g_MouseItemNumber=0;

									POS pos_s, pos_t;
									SetItemPos(HAND, &pos_s);
									SetItemPos(INV, first, ItemY, ItemX, &pos_t);
									SendMoveItem( item.item_no, pos_s, pos_t );
									DeleteItem( &HandItemAttr );
								}
								break;
							}

		case FT_ACCEL_INVENTORY :{
									const int ItemX = ( g_pointMouseX - (SMenu[i].x + SMenu[i].nField[j].x) ) / 35;

									if((g_MouseItemType != 1) || (GetItemAttr(HandItemAttr.item_no, WEAR_ABLE)!=WEAR_BELT)) break;

									if( SMenu[MN_GAMBLE].bActive )		// ÀÛ¾÷´ë°¡ ¶¸À» °æ¿ì ´Ù¸¥ °÷À¸·Î ¾ÆÀÌÅÛ ÀÌµ¿Àº ¾ÈµÈ´Ù.
									{
										MP3( SN_WARNING ); 
										break;
									}
									//010708 lsw
									if( SMenu[MN_GM_2ND_SKILL].bActive )		// ÀÛ¾÷´ë°¡ ¶¸À» °æ¿ì ´Ù¸¥ °÷À¸·Î ÀÌµ¿Àº ¾ÈµÈ´Ù.
									{
										MP3( SN_WARNING );
										break;
									}
									if(SMenu[MN_MERCHANT_REGISTER].bActive)//021014 lsw
									{
										MP3( SN_WARNING );// ¸¶¿ì½º °æ°íÀ½
										break;
									}
									if( 0 > ItemX || 5<=ItemX)//020515 lsw // 0~4 ±îÁö¸¸ Å¬¸¯µÇµµ·Ï
									{
										break;
									}
									if(QuickItemAttr[ItemX].item_no)
									{
										if( IsExchangeNow() )
										{
											int a;
											for(a=0; a<MAX_EXCHANGE_ITEM; a++)
											{
												if( item_give[a].item.item_attr.item_no && item_give[a].item.item_pos.type == QUICK && item_give[a].item.item_pos.p3 == ItemX )
												{
													MP3( SN_WARNING ); break;
												}
											}
											if( a != MAX_EXCHANGE_ITEM) break;
										}

										ItemAttr item_attr = QuickItemAttr[ItemX];
										QuickItemAttr[ItemX] = HandItemAttr;
										HandItemAttr = item_attr;

										g_MouseItemType=1;
										g_MouseItemNumber = HandItemAttr.item_no;

										POS pos_s, pos_t;
										SetItemPos(HAND, &pos_s);
										SetItemPos(QUICK, ItemX, &pos_t);
										SendMoveItem( QuickItemAttr[ItemX].item_no, pos_s, pos_t );
									}
									else
									{
										g_MouseItemType=0;
										g_MouseItemNumber=0;

										QuickItemAttr[ItemX] = HandItemAttr;
										POS pos_s, pos_t;
										SetItemPos(HAND, &pos_s);
										SetItemPos(QUICK, ItemX, &pos_t);
										SendMoveItem( QuickItemAttr[ItemX].item_no, pos_s, pos_t );
										DeleteItem( &HandItemAttr );
									}
									break;
								 }

		case FT_ARMOR_INVENTORY :{
									int nWeapon_Type = SMenu[i].nField[j].nWillDo;   // ¹«±â, ¹æÆÐ, Çï¸ä, °©¿Ê, unique, neck, shoesÀÇ ÆÇ´Ü

									int wear_position = SMenu[i].nField[j].nSpecialWillDo;
									int item_position = GetItemAttr(HandItemAttr.item_no, WEAR_ABLE);
									int equip_position = GetItemAttr( EquipItemAttr[WT_WEAPON].item_no, WEAR_ABLE);

									if( g_MouseItemType != 1 || !GetItemAttr(HandItemAttr.item_no, EQUIPABLE) ) break;

									if( SMenu[MN_GAMBLE].bActive )		// ÀÛ¾÷´ë°¡ ¶¸À» °æ¿ì ´Ù¸¥ °÷À¸·Î ÀÌµ¿Àº ¾ÈµÈ´Ù.
									{
										MP3( SN_WARNING );
										break;
									}

									CItem *hand_item = ItemUnit( HandItemAttr );
									if( !hand_item ) break;

									int hand_item_kind = GetItemAttr( HandItemAttr.item_no, ITEM_KIND );
									int equip_item_kind = GetItemAttr( EquipItemAttr[WT_WEAPON].item_no, ITEM_KIND );

									if( (wear_position == item_position) || ((wear_position == WEAR_RIGHT_HAND || wear_position == WEAR_LEFT_HAND) && (item_position == WEAR_TWO_HAND)) )
									{
										int a, b, c; 
//										ItemSound( HandItemAttr.item_no );
										if(wear_position == WEAR_LEFT_HAND && item_position == WEAR_TWO_HAND )  // ¾ç¼Õ°ËÀ» ¹æÆÐÀÚ¸®¿¡ ³ÖÀ» ¶§ 
										{
											wear_position = WEAR_RIGHT_HAND;
											nWeapon_Type = WT_WEAPON;
										}

										if( (item_position==WEAR_TWO_HAND) && (EquipItemAttr[WT_SHIELD].item_no) )	// ¹æÆÐ¸¦ µé°í ÀÖÀ»¶§, ¾ç¼Õ°ËÀ» Âø¿ëÇÏ¸é ¹æÆÐ¸¦ ¹þ´Â´Ù.
										{
											if( SearchEmpty(a, b, c) )				// ºó°÷ÀÌ ÀÖÀ¸¸é ±× ºó°÷¿¡ ¹æÆÐ¸¦ ¿Å±ä´Ù.
											{
												InvItemAttr[a][b][c] = EquipItemAttr[WT_SHIELD];
												DeleteItem( &EquipItemAttr[WT_SHIELD] );
//												sheild_unequip = true;
												
												POS pos_s, pos_t;
												SetItemPos(EQUIP, WT_SHIELD ,&pos_s);
												SetItemPos(INV, a, b, c, &pos_t);
												SendMoveItem( InvItemAttr[a][b][c].item_no, pos_s, pos_t, false );
												CallItemVirtualFunction( VF_RELEASE_ITEM, InvItemAttr[a][b][c].item_no );
											}
											else 
											{
												if (hand_item->GetItemKind() == IK_NOEXCHANGE) 
												{
													Kein_PutMessage( KM_FAIL, kein_GetMenuString(243) );
												}
												else
												{
													SendDropItem( HandItemAttr.item_no, Hero->x, Hero->y, EquipItemAttr[WT_SHIELD].attr );
													DeleteItem( &EquipItemAttr[WT_SHIELD] );
												}
											}
										}

										// °¡´ÉÇÏµµ·Ï ÀÌ°ÍÀú°Í ¹Ù²ÛÈÄ...
										if( !hand_item->is_equipable2( nWeapon_Type ) )	break;		// ¼­¹ö¿Í °°Àº ·çÆ¾, ¼Õ¿¡ µç°Ô ÀåÂøÀÌ °¡´ÉÇÏ³Ä?

										if(EquipItemAttr[nWeapon_Type].item_no)		// ¹¹°¡ ÀÖ´Ù.
										{
											ItemAttr item_attr = EquipItemAttr[nWeapon_Type];
											EquipItemAttr[nWeapon_Type] = HandItemAttr;

											g_MouseItemNumber=item_attr.item_no;
											HandItemAttr = item_attr;

											g_MouseItemType=1;

											POS pos_s, pos_t;
											SetItemPos(HAND, &pos_s);
											SetItemPos(EQUIP, nWeapon_Type, &pos_t);
											SendMoveItem( EquipItemAttr[nWeapon_Type].item_no, pos_s, pos_t );
											CallItemVirtualFunction( VF_RELEASE_ITEM, HandItemAttr.item_no );
											CallItemVirtualFunction( VF_EQUIP_ITEM, EquipItemAttr[nWeapon_Type].item_no );

											int a,b,c;
											if( SearchEmpty( a, b, c ) )
											{
												SetItemPos( INV, a, b, c, &IP_base );
												InvItemAttr[a][b][c] = HandItemAttr;
												memset( &HandItemAttr, 0, sizeof( ItemAttr ) );
												g_MouseItemNumber =0;
												g_MouseItemType = 0;
											}
										}
										else
										{
											g_MouseItemType=0;
											g_MouseItemNumber=0;
											EquipItemAttr[nWeapon_Type] = HandItemAttr;

											POS pos_s, pos_t;
											SetItemPos(HAND, &pos_s);
											SetItemPos(EQUIP, nWeapon_Type, &pos_t);
											SendMoveItem( EquipItemAttr[nWeapon_Type].item_no, pos_s, pos_t );
											CallItemVirtualFunction( VF_EQUIP_ITEM, EquipItemAttr[nWeapon_Type].item_no );
											DeleteItem( &HandItemAttr );
										}

									}
									break;
								 }
		case FT_BANK_INVENTORY :{
									int ItemX = ( g_pointMouseX - (SMenu[i].x + 7) ) / 35;
									int ItemY = ( g_pointMouseY - (SMenu[i].y + 206) ) / 35;
																										
									int xCheck = ( g_pointMouseX - (SMenu[i].x + 7) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
									int yCheck = ( g_pointMouseY - (SMenu[i].y + 206) ) % 35;
									if( (xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ) )  
									{
										break;	
									}
                  if( (3 > ItemY  && 0 <= ItemY )
										&& (6 > ItemX  && 0 <= ItemX ))//ÀÎµ¦½º °è»ê Ã¼Å©
									{	
									  int first = SMenu[i].nField[0].nSHideNomalStart;
									  ItemAttr &item = BankItemAttr[first][ItemY][ItemX];
									  if(g_MouseItemType != 1 )	
									  {
										  if( !item.item_no ) break;
										  // ÀÎº¥Åä¸® Ã¢À¸·Î ÀÚµ¿ÀÌµ¿
										  {
											  int a, b, c;
											  int para = SearchEmpty( a, b, c );
											  if( para == 0 ) break;

											  POS pos_s, pos_t;
											  SetItemPos(BANK, first, ItemY, ItemX, &pos_s);
											  SetItemPos(INV, a, b, c, &pos_t);
											  SendMoveItem( item.item_no, pos_s, pos_t );
											  InvItemAttr[a][b][c] = item;
											  DeleteItem( &item );
											  break;
										  }
										  break;
									  }

									  if( item.item_no )
									  {
										  // YGI 020527
//										  ItemAttr attr_temp = item;
										  item = HandItemAttr;
										  g_MouseItemNumber=0;
										  memset( &HandItemAttr, 0, sizeof(ItemAttr) );
										  g_MouseItemType=0;

										  POS pos_s, pos_t;
										  SetItemPos(HAND, &pos_s);
										  SetItemPos(BANK, SMenu[i].nField[0].nSHideNomalStart,ItemY , ItemX, &pos_t );
										  SendMoveItem( item.item_no, pos_s, pos_t );
									  }
									  else
									  {
										  g_MouseItemType=0;
										  g_MouseItemNumber=0;

										  item = HandItemAttr;
										  POS pos_s, pos_t;
										  SetItemPos(HAND, &pos_s);
										  SetItemPos(BANK, SMenu[i].nField[0].nSHideNomalStart,ItemY , ItemX, &pos_t );
										  SendMoveItem( item.item_no, pos_s, pos_t );
										  DeleteItem( &HandItemAttr );
									  }
                  }
									break;
								}
		case FT_MAKE_ITEM : {
								if( g_MouseItemType != 1) 
								{
									for( int a=0; a<20; a++ )
									{
										if( make_item_list[a].item_no && MouseInRectCheak( SMenu[i].x, SMenu[i].y, make_item_list[a].rect ) )
										{
											memset( &make_item_list[a], 0, sizeof( MakeItem ) );
											break;
										}
									}
									break;
								}
								
								int ct=0;
								int a;
								for( a=0; a<20; a++ )
								{
									if( !make_item_list[a].item_no ) 
									{
										ct++;
										break;
									}
								}
								if( ct==20) break; // ²Ë Ã¡´Ù. 
								make_item_list[a].item_no = HandItemAttr.item_no;
								make_item_list[a].x = g_pointMouseX-SMenu[i].x;
								make_item_list[a].y = g_pointMouseY-SMenu[i].y;
								SetRect( &make_item_list[a].rect, make_item_list[a].x-15,make_item_list[a].y-15,make_item_list[a].x+15,make_item_list[a].y+15 );
								make_item_list[a].attr = HandItemAttr;
								make_item_list[a].pos = IP_base;		// ÀÎº¥Åä¸® ³»ÀÇ Àå¼Ò¸¦ ±â¾ïÇÑ´Ù.

								PutMouseItemCheck();		// 020716 YGI

//								g_MouseItemType = 0;
//								g_MouseItemNumber = 0;
								break;
							}
		case FT_ITEM_MAKE_SELECT :	{
										int first = SMenu[i].nField[j].nSHideNomalStart;

										int ItemX = g_pointMouseX - (SMenu[i].x+SMenu[i].nField[j].x); 
										int ItemY = g_pointMouseY - (SMenu[i].y+SMenu[i].nField[j].y);

										if( ItemX < 0 ) break;
										else if( ItemX < 32 ) ItemX = 0;
										else if( ItemX > 37 && ItemX < 69 ) ItemX = 1;
										else if( ItemX > 74 && ItemX < 106 ) ItemX = 2;
										else if( ItemX > 111 && ItemX < 143 ) ItemX = 3;
										else if( ItemX > 148 && ItemX < 180 ) ItemX = 4;
										else break;

										if( ItemY < 0 ) break;
										else if( ItemY < 32 ) ItemY = 0;
										else if( ItemY > 42 && ItemY < 74 ) ItemY = 1;
										else break;

										ItemJoinSelect item;
										item = Item_Select[first][ItemX+ItemY*5];
										if( item.item_no )  item_select_result= item;


										for(int ji = 0 ; ji < ITEM_JOIN_MAX ;ji++)
										{
											if( item_join[ji].item_id == item_select_result.item_no)
											{
												item_select_result.join_no = ji;
												break;
											}
										}
										skillmakeiteminfo.join_no = item_select_result.join_no;
										skillmakeiteminfo.MakeMuch = 0;
										sprintf(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,"%d ",0);
										
										break;
									}
		case FT_ITEM_USE_SELECT :	{
										int first = SMenu[i].nField[j].nSHideNomalStart;

										int ItemX = g_pointMouseX - (SMenu[i].x+SMenu[i].nField[j].x); 
										int ItemY = g_pointMouseY - (SMenu[i].y+SMenu[i].nField[j].y);

										if( ItemX < 0 ) break;
										else if( ItemX < 32 ) ItemX = 0;
										else if( ItemX > 37 && ItemX < 69 ) ItemX = 1;
										else if( ItemX > 74 && ItemX < 106 ) ItemX = 2;
										else if( ItemX > 111 && ItemX < 143 ) ItemX = 3;
										else if( ItemX > 148 && ItemX < 180 ) ItemX = 4;
										else break;

										if( ItemY < 0 ) break;
										else if( ItemY < 32 ) ItemY = 0;
										else if( ItemY > 42 && ItemY < 74 ) ItemY = 1;
										else break;

										PosItem		item;
										item = skill_use_item[first][ItemX+ItemY*5];
										if( item.item_no ) skill_use_item_result = item;
										break;
									}

		case FT_BBS_GO :	SMenu[i].work = j; break;
		case FT_PARTY :	{
							int para = SMenu[i].nField[j].nWillDo;  // ¹è¿­ ÂüÁ¶¿¡ ¾²ÀÏ Ã·ÀÚ
							LpCharacterParty character;				// <- ÆÄÆ¼¸¦ ¸ÎÀ» »ç¶÷¸¦ ÀúÀåÇÒ º¯¼ö

							int para2 = SMenu[i].nField[j].nSpecialWillDo;	// FT_PARTY_FACE ¸¦ °¡¸®Å´
							int &target_para = SMenu[i].nField[para2].nWillDo;

							switch( SMenu[i].nTemp )
							{
								case PARTY :		character = &SCharacterData.party[para]; break;
								case RELATION :		character = &SCharacterData.relation[para]; break;
								case EMPLOYMENT :	character = &SCharacterData.employment[para]; break;
							}
							
							if( g_MouseItemType == PARTY )
							{
								SendInputParty( SMenu[i].nTemp, para, &mouse_ch );
								g_MouseItemType = 0;
								memset( &mouse_ch, 0, sizeof( CharacterParty ) );
							}
							else if( !g_MouseItemType )		// ´Ü¼ø Å¬¸¯
							{
								target_para = para;						// Á¤º¸ Ãâ·Â°ú ÇØÁ¦¸¦ À§ÇØ	// ¼­¹ö¿¡¼­ Àç Àü¼Û
								if( *character->m_Name  ) SendReqPartyInfo( SMenu[i].nTemp, para );
							}

						break;
					}

		case FT_REVIVAL_BUTTON : 
			{
				if( IsRightWindowOpen() ) return;
				if( Hero->viewtype != VIEWTYPE_GHOST_ ) return;		// Á×¾úÀ»¶§¸¸ ¶á´Ù.
				SMenu[i].bActive = false;
				CallServer( CMD_REVIVAL_COLOSSUS ); 
				//ºÎÈ°À» ¼­¹ö·Î ¿äÃ»ÇÑ´Ù.
				break;
			}
		case FT_AWAKE_BUTTON :	if( IsRightWindowOpen() ) return; 
								CallServer( CMD_SERVER_WAKE_UP ); 
								SMenu[i].bActive = false;
								break;

		case FT_PREACH_NAME : 
			{
				SetFocus2( HWND_3 );//021001 lsw
			}break;
		case FT_PUBLIC_BBS_NEXT : {
									if( !public_bbs.m_pszMessage ) break;

									int &work = SMenu[i].key;
									work++;
									int line = GetLineRectTextPut( SMenu[i].nField[j].nWillDo, public_bbs.m_pszMessage );
									if( work > (line/6+1) )		// 6: ¿©¼¸ ¶óÀÎ..
									{
										SmallMenuClose();
									}
									break;
								  }
		//010910 lsw
		case FT_WILLDO_WORK :
			{
				if (SMenu[i].work==1)
				{
					SMenu[i].work = 2;
				}
				else 
				{
					SMenu[i].work = 1;
				}
			}break;
      case FT_QUICK_MAGIC_PLUS: 
		{
			if( !magic_plus_count ) break;
			if( magic_plus_start < 0 ) break;
			if( IsHeroDead() ) break;		// Á×¾úÀ» °æ¿ì Å¬¸¯µÇ¸é ¾ÊµÈ´Ù.

			int x_count = (g_pointMouseX - SMenu[i].nField[j].rCheakBox.left) / 44;
			int x_check = (g_pointMouseX - SMenu[i].nField[j].rCheakBox.left) % 44;
			if( x_check < 2 || x_check > 41 ) break;
			int para = magic_plus_start+x_count;

			const int iMagicNo = magic_plus[para];
			if( !iMagicNo ) break;

			HotKeyMemory.SetQuickMemory(Hero->name,QM_MAGIC,iMagicNo);// quick magic¿¡ ³Ñ°ÜÁÜ//020701 lsw

			const int nMagic = g_mgrBattle.Convert(iMagicNo);

			if (GetMagic()->IsEnable(nMagic))
			{	//< CSD-TW-030606
				RectImageOn = true;
				MagicOnFlag.SetMagicOn(iMagicNo);		// ±ô¹ÚÀÌ´Â °Å ¼ÂÆÃ
				g_mgrBattle.SendMagicSelect(iMagicNo); 
			}	//> CSD-TW-030606

			break;					   
		}
		case FT_QUICK_MAGIC_ARRAY_SCROLL :	{
												if( magic_plus_count <= 5 ) break;

												int willdo = SMenu[i].nField[j].nWillDo;
												if( willdo == 0 )	//up ¹öÆ°
													magic_plus_start+=5;
												else magic_plus_start-=5;		// down button
 
												if( magic_plus_start >= magic_plus_count-5 ) magic_plus_start = magic_plus_count-5;
												else if( magic_plus_start < 0 ) magic_plus_start = 0;

												break;
											}
		case FT_TIME_DISPLAY_ON_OFF	:{
										g_bTimeMenuOn = !g_bTimeMenuOn;
										break;
									 }

		case FT_QUICK_SKILL_PUT : 
			{
				int x = SMenu[i].x+SMenu[i].nField[j].x;
				int ItemX = ( g_pointMouseX - (x+4) )/38;
				int s = skill_index[ItemX];
				
				if( !s || !skill[s].num ) break;
				int para = 1;
				if( skill[s].series==1 ) 
				{
					para = 0;
				}
				//1004 zhh ÇöÁ¦ ½ºÅ³ÀÌ ¼±ÅÃ µÈ°ÍÀÌ ÀÖÀ¸¸é Ãë¼ÒÇÏ±â Àü¿¡´Â ´Ù¸¥ ½ºÅ³À» ¼±ÅÃÇÏÁö ¸øÇÑ´Ù
				//¶ÇÇÑ ½ºÅ³¿¡ ÇÊ¿äÇÑ Àåºñµµ °¡Áö°í ÀÖ¾î¾ß ÇÑ´Ù.
				if(SelectSkill( s )==1)
				{
					break;
				}
				HotKeyMemory.SetQuickMemory(Hero->name,QM_SKILL,s);//020701 lsw
				SkillOn = para;
				CanMakeItemListSet( s );		// ¸¸µé¼ö ÀÖ´Â ¾ÆÀÌÅÛ °¹¼ö
			}break;
		case FT_QUICK_SKILL_SCROLL : {
										int willdo = SMenu[i].nField[j].nWillDo;
										if( willdo )	// scroll down
										{
											SMenu[i].work++;
											if( SMenu[i].work > 3 ) 
											{
												MP3( SN_WARNING );
												SMenu[i].work = 3;
											}
										}
										else	// scroll up
										{
											SMenu[i].work--;
											if( SMenu[i].work < 0 ) 
											{
												MP3( SN_WARNING );
												SMenu[i].work = 0;
											}
										}
										break;
									 }
		case FT_QUICK_SKILL_SCROLL_LR :{
										int field = SMenu[i].nField[j].nSpecialWillDo;
										int willdo = SMenu[i].nField[j].nWillDo;
										int &scroll = SMenu[i].nField[field].nSHideNomalStart;
										int max = SMenu[i].nField[field].nSHideNomalCount;
										if( !willdo )	// ¿ÞÂÊ ½ºÅ©·Ñ
										{
											scroll--;
											if( scroll < 0 )
											{
												scroll = 0 ;
												MP3( SN_WARNING );
											}
										}
										else			// ¿À¸¥ÂÊ ½ºÅ©·Ñ
										{
											scroll++;
											if( scroll >= max )
											{
												scroll = max-1;
												MP3( SN_WARNING );
											}
										}
										break;
									   }
		case FT_SCROLL_BUTTON2 :
			{
				int type = SMenu[i].nField[j].nWillDo;
				int param = SMenu[i].nField[j].nSpecialWillDo; 
				int &page = SubMenu->nField[param].nSHideNomalStart;
				int &page_max = SubMenu->nField[param].nSHideNomalCount;
				int &plus= SubMenu->nField[param].nShideNomalPlus;

				if( type == SCROLL_UP )
				{
					if( page > 0 ) page-=plus;
				}
				else
				{
					if( page < page_max-plus )	page+=plus;
				}
				break;
			}
		case FT_SCROLL_BUTTON : {
									int type = SMenu[i].nField[j].nWillDo;
									int &page_status = SMenu[i].work;

									WORD page;
									WORD page_max;
									GetWORDOfDWORD( (DWORD)page_status, page, page_max );

									if( type == SCROLL_UP )
									{
										if( page > 0 ) page--;
									}
									else
									{
										if( page < page_max-1 )	page++;
									}
									page_status = MAKELONG( page, page_max );
									break;
								}
        case FT_CHECK_COMBO2 :
			{	//< CSD-030723
				if (i == MN_DUAL_MAIN)
				{
					const int nWillDo = SMenu[i].nField[j].nWillDo;
					
					switch (SCharacterData.nCharacterData[SPELL])
					{
					case WIZARD_SPELL: // ¸¶¹ý»ç ½ºÆçÀ» °¡Áö°í ÀÖ´Â °æ¿ì
						{ 
							string strExplain;
							  
							if (nWillDo == PRIEST)
							{
								strExplain = lan->OutputMessage(1, 39);
							}
							else
							{
								strExplain = g_mgrDual.GetDulaExplain(Hero->class_type, nWillDo);  
							}
							  
							strcpy(SMenu[MN_DUAL_MAIN].nField[6].temp,  strExplain.c_str());  
							  
							break;
						}
					case PRIEST_SPELL: // ¼ºÁ÷ÀÚ ½ºÆçÀ» °¡Áö°í ÀÖ´Â °æ¿ì
						{ 
							string strExplain;
						  
							if (nWillDo == WIZARD)
							{
								strExplain = lan->OutputMessage(1, 40);
							}
							else
							{
								strExplain = g_mgrDual.GetDulaExplain(Hero->class_type, nWillDo);
							}							  
							strcpy(SMenu[MN_DUAL_MAIN].nField[6].temp, strExplain.c_str());
							break;
						}
					}
				}
			}	//> CSD-030723
		case FT_CHECK_COMBO : {
								;
								int *lpValue = SMenu[i].nField[j].nValue;
								int nWillDo = SMenu[i].nField[j].nWillDo;
								MP3( SN_CLICK_START );
								*lpValue = nWillDo;
								break;
							  }
		case FT_WRITE_BBS :				
			{
				SetFocus2( HWND_3 );//021001 lsw
			}break;
		case FT_WRITE_BBS_ITEM_NAME :	
			{
				SetFocus2( HWND_1 );//021001 lsw
			}break;
		case FT_WRITE_BBS_ITEM_MONEY :	
			{
				SetFocus2( HWND_2 );//021001 lsw
			}break;

		case FT_GUILD_INTRODUCTION	: 
			{
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)SMenu[i].work, page, page_max );
				int guild_code = 0;

				int continue_flag = 0;
				for( int a=0; a<g_Guild_Introduction->m_Count; a++ )
				{
					if( g_Guild_Introduction->m_Guild[a].page == page )
					{
						if( MouseInRectCheak( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_Guild_Introduction->m_Guild[a].rect, true ) )
						{
							guild_code = g_Guild_Introduction->m_Guild[a].m_nCode;
							break;
						}
						continue_flag = 1;
					}
					else if( continue_flag ) 
					{
						break;
					}
				}
				if( !guild_code ) break;
				SendReqGuildInfo1( guild_code );
				CallGuildInfoMenu( i, guild_code, 1 );
				k_req_guild_master_and_sub_master data;
				data.guild_code = guild_code;
				data.type = 1;
				SendDirectDB(CMD_GUILD_MASTER_AND_SUBMASTER, 
					         &data,
							 sizeof(k_req_guild_master_and_sub_master) - 40);
				SMenu[i].bActive = false;
				break;
			  }

		case FT_GUILD_ITEM_GET : 
			{
				// ¸¶¿ì½º·Î i, j °ª ¾ò¾î³»±â
				int base_x = SubMenu->x+SubMenu->nField[j].x;
				int base_y = SubMenu->y+SubMenu->nField[j].y;

				int first = (g_pointMouseX-base_x)/35;
				int second = (g_pointMouseY-base_y)/35;

				int line = SubMenu->key;
				int &page_status = SubMenu->work;
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)page_status, page, page_max );

				int para = second*5+first+page*line;
				if( !g_Menu_Variable.m_pGuildItem ) break;
				if( g_Menu_Variable.m_pGuildItem->m_BoxMoney ) 
				{
					Kein_PutMessage( KM_FAIL, kein_GetMenuString( 134 ) );
					break;
				}
				
				if( !CheckGuildPower( GDP_GUILD_BOX, Hero ) )		// ±ÇÇÑÀÌ ÀÖ³ª?
				{
					Kein_PutMessage( KM_FAIL, kein_GetMenuString( 146 ) );	// ±ÇÇÑÀÌ ¾ø½À´Ï´Ù.
					break;
				}

				if( para > g_Menu_Variable.m_pGuildItem->GetMax() ) break;
				ItemAttr *pItem = g_Menu_Variable.m_pGuildItem->GetItemByParm( para );
				if( !pItem->item_no ) break;

				DWORD index = g_Menu_Variable.m_pGuildItem->GetIndex( para );
				if( !index ) break;

				CallServer( CMD_GET_GUILD_ITEM_CLIENT, &index, sizeof( DWORD ) );
				g_Menu_Variable.m_pGuildItem->clear( para );
				break;
			}
		
		case FT_LOTTO_NUMBER://soto-030428
		case FT_PUT_BUTTON :
		case FT_TEXT_BUTTON_EXIT :
		case FT_MAIN_MENU :
		case FT_SHOW_SPECIAL_WILLDO_AUTO_PUT :	//010910 lsw
		case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT :	
		case FT_HIDE_SPECIAL_WILLDO_PUT :
		case FT_SPECIAL_WILLDO :
		{
			bool MainOn = false;	// Å¬¸¯ÇÑ ¸Þ´º¸¦ ³²°Ü¾ß ÇÒ°æ¿ì´Â TRUE ½ÃÄÑÁØ´Ù.
			switch(SMenu[i].nField[j].nSpecialWillDo)
			{
				
				case SWD_TURN_BUTTON :	
				{
					if (Hero->nCurrentAction!=17)	// LTS SITDOWN				// ¾ÉÀº »óÅÂ ±âº» ÆòÈ­»óÅÂ 
					{
						main_interface.data[IF_BATTLE_PEACE] = !main_interface.data[IF_BATTLE_PEACE];	
						SettingBasicAction( main_interface.data[IF_BATTLE_PEACE] );
						//if( main_interface.data[IF_BATTLE_PEACE] == BATTLE_MODE ) MP3(SN_FIGHT );
						if( main_interface.data[IF_BATTLE_PEACE] == BATTLE_MODE ) 
						{
							MP3(SN_FIGHT );
							AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(0,2) );//010304 lsw /¿©±ä°¡??
						}
						else
						{
							AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(0,1) );//010304 lsw /¿©±ä°¡?? ¸ðµå Ã¼ÀÎÁö ÇÕ´Ï´Ù. 
						}
					}
					else 
						AddCurrentStatusMessage( 255,255,255, kein_GetMenuString(96) );

					MainOn = true; 
					break;    		
 				}		
				case SWD_ARIGEMENT_STATUS :	SMenu[MN_ARIGEMENT].bActive=FALSE;
											MenuStack.DelMenu(MN_ARIGEMENT);
											SMenu[MN_STATUS].bActive=TRUE;
											MP3( SN_MENU_OPEN );
											MenuStack.PutMenu(MN_STATUS);
											MainOn = true; break;

				case SWD_MAININTERFACE_STATUS : {
													switch( g_MouseItemType )
													{
														case PARTY :	{
																			for(int a=0; a<6; a++ )
																			{
																				if( !*SCharacterData.party[a].m_Name )
																				{
																					break;
																				}
																			}
																			break;
																		}

														case 0 :	if(SMenu[MN_ARIGEMENT].bActive)
																	{
																		SMenu[MN_STATUS].bActive=FALSE;
																		MenuStack.DelMenu(MN_STATUS);
																		SMenu[MN_ABILITY].bActive=FALSE;
																		MenuStack.DelMenu(MN_ABILITY);
																		fLMoveMenu=MM_ARIGEMENT_CLOSE;
																		MP3( SN_MOVE_CLOSE );
																		MainOn = true; break;
																	}
																	if(SMenu[MN_STATUS].bActive)
																	{
																		fLMoveMenu=MM_STATUS_CLOSE;
																		MP3( SN_MOVE_CLOSE );
																		MainOn = true; break;
																	}
																	if(SMenu[MN_ABILITY].bActive)
																	{
																		fLMoveMenu=MM_ABILITY_CLOSE;
																		MP3( SN_MOVE_CLOSE );
																		MainOn = true; break;
																	}
																	if(SMenu[MN_TACTICS].bActive)
																	{
																		fLMoveMenu=MM_TACTICS_CLOSE;
																		MP3( SN_MOVE_CLOSE );
																		MainOn = true; break;
																	}
																	fLMoveMenu=MM_STATUS_OPEN;
																	OpenStatus( );
																	// Å¬¸¯Àü ¸Þ´º½Éº¼ 
 																	SMenu[MN_STATUS].nField[6].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+154;
																	// Å¬¸¯ÈÄ ¸Þ´º½Éº¼
																	SMenu[MN_STATUS].nField[7].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+163;
																	break; 
													}
													MainOn = true;
													break;
												}
				
				case SWD_PARTY_BUTTON :		{	
					/*
												int field = SMenu[i].nField[j].nSHideNomalNumber;		// ¿ª½Ã 12¹ø ÇÊµå¸¦ °¡¸®Å²´Ù.
												int &para = SMenu[i].nField[field].nWillDo;
												para = 0;
												SetPartyMenu( SMenu[i].nField[j].nWillDo ); 
												SMenu[i].key = SMenu[i].nField[j].nWillDo;
												*/
												g_Menu_Variable.m_PartySelect = SMenu[i].nField[j].nWillDo;
												SendSelectPartyExp( g_Menu_Variable.m_PartySelect );
												MainOn = true; break;
											}
				case SWD_PARTY_RELEASE :	{
												int field = SMenu[i].nField[j].nWillDo;
												int &para = SMenu[i].nField[field].nWillDo;
												LpCharacterParty target;
												bool bCallAble = false;
												switch( SMenu[i].nTemp )
												{
													case PARTY :		target = &SCharacterData.party[para]; bCallAble = true;break;
													case RELATION :		target = &SCharacterData.relation[para]; bCallAble = true;break;
													case EMPLOYMENT :	target = &SCharacterData.employment[para]; bCallAble = true;break;
												}
												if(bCallAble)
												{
													SendDeleteParty( SMenu[i].nTemp, para );		// ÇØÁ¦
													memset( target, 0, sizeof( CharacterParty ) );
													para = 0;
												}
												MainOn = true; break;
											}

 				case SWD_MAININTERFACE_MAGIC :
												if( !SMenu[MN_QUICK_SKILL].bActive )
													g_MagicMenuOn = !g_MagicMenuOn;			// ¸ÅÁ÷ ¸Þ´º ÄÑ°í ²ô±â
												else
												{
													MenuSoundClose( MN_QUICK_SKILL );
													g_MagicMenuOn = true;
												}

												MainOn = true; break;   


				case SWD_MAININTERFACE_ITEM :	if(SMenu[MN_ITEM].bActive)
												{
													nPartyOther=0;
													fRMoveMenu=MM_ITEM_CLOSE;
													MP3( SN_MOVE_CLOSE );
												}
												else 
												{
													int t = IsMagicWindowOn(); 
													if( t )	// ¸¶¹ý¸Þ´º°¡ È°¼ºÈ­ µÇ¾î ÀÖ´À³Ä?
													{
														SMenu[t].bActive=FALSE;
														MenuStack.DelMenu( t );
														SMenu[MN_ITEM].bActive=TRUE;
														MP3( SN_MENU_OPEN );
														MenuStack.PutMenu(MN_ITEM);
													}
													else if(SMenu[MN_PARTY].bActive)
													{
														nPartyOther = MN_ITEM;
														fRMoveMenu = MM_PARTY_CLOSE;
														MP3( SN_MOVE_CLOSE );
													}
													else 
													{
														fRMoveMenu=MM_ITEM_OPEN;
														MP3( SN_MOVE_OPEN );
													}
												}												
												MainOn = true; break;   

				case SWD_MAININTERFACE_SKILL :{
												if( SMenu[MN_QUICK_SKILL].bActive )
												{
													MenuSoundClose( MN_QUICK_SKILL );
													MainOn = true; break;
												}
												CallMenu( MN_QUICK_SKILL );
												SkillSettingClear();
												MainOn = true; break;
											  }

				case SWD_MAININTERFACE_PARTY :	{
													if(SMenu[MN_PARTY].bActive) 		   // 1´Ü°è. ÀÌ¹Ì ¶° ÀÖ³ª?
													{
														nPartyOther=nPartyState;
														fRMoveMenu=MM_PARTY_CLOSE;         // ´Ý°í ³¡³¿.
														MP3( SN_MOVE_CLOSE );
														MainOn = true; break;
													}
													
													if( SMenu[MN_ITEM].bActive )
													{
														nPartyOther=0;
														nPartyState=MN_ITEM;
														fRMoveMenu=MM_PARTY_OPEN;
														MP3( SN_MOVE_OPEN );
														if( !SMenu[MN_PARTY].nTemp )  SMenu[MN_PARTY].nTemp = PARTY;
														MainOn = true; 
														break;
													}

													if( MainOn ) break;
													nPartyState = 0;		// ¾Æ¹«°Íµµ ¾È¶°ÀÖÀ»¶§
													nPartyOther = 0;		// ³ªÁß¿¡ ¶ã ¸Þ´ºµµ ¾ø´Ù.
													fRMoveMenu=MM_PARTY_OPEN;
													if( !SMenu[MN_PARTY].nTemp )  SMenu[MN_PARTY].nTemp = PARTY;
													MP3( SN_MOVE_OPEN );
													MainOn = true; break;
												}
															 
				case SWD_MAININTERFACE_ARIGEMENT :	SMenu[MN_ARIGEMENT].bActive=TRUE;
													MP3( SN_MENU_OPEN );
													MenuStack.PutMenu(MN_ARIGEMENT);
													MenuSoundClose( MN_STATUS );

													MenuStack.DelMenu(MN_STATUS);
													SMenu[MN_ARIGEMENT].nField[0].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+154;
													SMenu[MN_ARIGEMENT].nField[1].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+163;
													MainOn = true; break;

				//011014 lsw >
				case SWD_INFO_FLAG_SET: main_interface.data[IF_MAP] = !main_interface.data[IF_MAP];
										if(::isNationWarfieldServer() || ::isNewWarfieldServer())
										{
											CloseAllMenu();
											::SendNationPacket(BUTTON_NATION_STATUS);//021203 lsw
										}
										else
										{	//< CSD-030324
											// thai2 YGI
											extern int g_SmallMapNumber;
											if( main_interface.data[IF_MAP] )
											{
												g_SmallMapNumber = MapNumber;
												CallMenu(  MN_SMALL_MAP );
												SMenu[MN_SMALL_MAP].nTemp = 0;
											}
											else 
											{
												MenuSoundClose( MN_SMALL_MAP );
												SMenu[MN_SMALL_MAP].nTemp = 0;
											}
										}	//> CSD-030324

										MainOn = true; break;

				case SWD_FACE_CHOOSE :	SCharacterData.nCharacterFace=j; //ÇÊµå ¹øÈ£·Î Ç¥Á¤¹øÈ£¸¦ È®ÀÎÇÑ´Ù.
										SendSmileFace( j );		// ´Ù¸¥ Ä³¸¯ÅÍ¿¡°Ô ³» Ç¥Á¤À» º¸³»ÁØ´Ù.
										y_MenuFocus=0;
										break;

				case SWD_NORMAL_CHECK_PUT :	
					switch( SMenu[i].nField[j].nWillDo )
					{
						case 11 :	if( system_info.gore ) SMenu[i].nField[SMenu[i].nField[j].nWillDo].nType = FT_NO_CHECK; 
									else SMenu[i].nField[SMenu[i].nField[j].nWillDo].nType = FT_NOMAL_PUT; 
									system_info.gore = !system_info.gore;
									break;
						case 13 :	if( system_info.counter ) SMenu[i].nField[SMenu[i].nField[j].nWillDo].nType = FT_NO_CHECK;
									else SMenu[i].nField[SMenu[i].nField[j].nWillDo].nType = FT_NOMAL_PUT; 
									system_info.counter = !system_info.counter;
									break;
						case 15 :	if( system_info.action ) SMenu[i].nField[SMenu[i].nField[j].nWillDo].nType = FT_NO_CHECK;
									else SMenu[i].nField[SMenu[i].nField[j].nWillDo].nType = FT_NOMAL_PUT; 
									system_info.action = !system_info.action;
									break;
						case 17 :	if( system_info.whisper ) SMenu[i].nField[SMenu[i].nField[j].nWillDo].nType = FT_NO_CHECK;
									else SMenu[i].nField[SMenu[i].nField[j].nWillDo].nType = FT_NOMAL_PUT;
									system_info.whisper = !system_info.whisper;
									if(!system_info.whisper)
									{
										ChatMgr.SetRefuseWhiper(NULL);//021026 lsw
									}
									else
									{
										ChatMgr.SetAcceptWhiper(NULL);//021026 lsw
									}

									if( ChatMgr.IsRefuseAllWhisper() )//021026 lsw
										AddCurrentStatusMessage( 200, 20 ,20 , lan->OutputMessage(3,156) );//010215 lsw
									else 
										AddCurrentStatusMessage( 255, 255 ,255 , lan->OutputMessage(3,157));//010215 lsw
									break;
					}
					MainOn = true; break;

				case SWD_LMENUCLOSE :	LeftMenuClose();
										MP3( SN_MOVE_CLOSE );
										MainOn = true; break;

				case SWD_RMENUCLOSE :
					{//021025 lsw
						if(SMenu[MN_ITEM].bActive)	 
						{
							fRMoveMenu=MM_ITEM_CLOSE;
						}
						else if(SMenu[MN_PARTY].bActive)
						{
							 fRMoveMenu=MM_PARTY_CLOSE;
							 nPartyOther=nPartyState;
						}
						MP3( SN_MOVE_CLOSE );
						MainOn = true; 
					}break;

				case SWD_MOVE_OPEN_CLOSE :	
					{
						fRMoveMenu = fLMoveMenu = SMenu[i].nField[j].nWillDo;
						if( SMenu[i].nField[j].nSHideNomalCount ) MP3( SN_MOVE_OPEN ); // 0 : close, 1:open
						else MP3( SN_MOVE_CLOSE );
						MainOn = true; break;
					}
				case SWD_TWO_MENU_OPEN :{					// µÎ°³ÀÇ ¸Þ´º¸¦ µ¿½Ã¿¡ ¶ç¿ï¶§ nWillDo¿Í nSHideNomalNumber¸¦ »ç¿ëÇÑ´Ù. ¹°·Ð ´­·¶´ø ¸Þ´º´Â ¾ø¾îÁø´Ù
											y_MenuFocus = 0;
											CallMenu( SMenu[i].nField[j].nWillDo );
											CallMenu( SMenu[i].nField[j].nSHideNomalNumber );
											break;
										}
				case SWD_MENU_OPEN	:	{
											MainMenuClose( );
											CallMenu( SMenu[i].nField[j].nWillDo );
											if( i==MN_MAININTERFACE && j==23 ) 
											{
												SkillSettingClear();
												//AllReqSkillExp( );
											}
											y_MenuFocus=SMenu[i].nField[j].nWillDo;
											MainOn = true; break;
										}

				case SWD_SMALL_MENU_WITH_SWD_TO_WORK : {
															SMenu[SMenu[i].nField[j].nWillDo].key = SubMenu->nField[j].nSHideNomalNumber;
													   }
				case SWD_SMALL_MENU : {
										CallSmallMenu( SMenu[i].nField[j].nWillDo );
										break;
									  }

				case SWD_SIMPLE_CLOSE : {
											SmallMenuClose( );
											CallMenu( SMenu[i].nField[j].nWillDo );
											MainOn = true; break;
										}

				case SWD_SKILLGUILD_LEARN:	{
												if( SMenu[i].nField[j].nWillDo == MN_SKILLGUILD_SKILL )
												{
													nSkillLearnResult = 0;
													SMenu[MN_SKILLGUILD_SKILL].work = 0;
													GetLearnSkillInv( i );
													//GetLearnSkillInv( SMenu[i].nField[j].nSHideNomalCount );
												}
												else if( SMenu[i].nField[j].nWillDo == MN_SKILLGUILD_ITEM )
												{
													SetLearnItemData( i, learn_item_list );
													SMenu[MN_SKILLGUILD_ITEM].nField[3].nSHideNomalStart = 0;
												}
												else if( SMenu[i].nField[j].nWillDo == MN_SKILLGUILD_KNOWLAGE )
												{
													nSkillLearnResult = 0;
													SMenu[MN_SKILLGUILD_KNOWLAGE].work = 0;
													GetLearnKnowlageInv();
												}												
												CallMenu( SMenu[i].nField[j].nWillDo );
												y_MenuFocus = 0;
												MP3( SN_MENU_OPEN );
												SMenu[SMenu[i].nField[j].nWillDo].nTemp = i;		//¾îµð¼­ ¿Ô´ÂÁö ±â·Ï
												break;
											}
				case SWD_LEARNSKILL_OTHER:	{
												if( i == MN_SKILLGUILD_SKILL || i == MN_SKILLGUILD_KNOWLAGE )
												{
													SMenu[i].nField[6].nSHideNomalStart = !SMenu[i].nField[6].nSHideNomalStart;
													SMenu[i].nField[2].nSHideNomalStart=0;

													SMenu[i].work = 0;
													nSkillLearnResult = 0;
													
													if( !SkillInventory[SMenu[i].nField[6].nSHideNomalStart][1][0] )		// µÎ¹øÂ° ¹è¿­ Ã³À½ÀÌ ¾Æ¹«°Íµµ ¾øÀ¸¸é ¹öÆ°À» ´©¸£Áö ¸øÇÏ°Ô ÇÑ´Ù.
													{
														SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
													}
												}
												else if( i == MN_SKILLGUILD_ITEM )
												{
													int para = SMenu[i].nField[j].nWillDo;
													int is_left = SMenu[i].nField[j].nSHideNomalCount;

													int max = SMenu[i].nField[para].nSHideNomalCount;
													int &curr = SMenu[i].nField[para].nSHideNomalStart;

													if( is_left == RIGHT )
													{
														curr++;
														if( curr >= max ) curr = max - 1;
													}
													else
													{
														curr--;
														if( curr <= 0 ) curr = 0;
													}

													if( curr+1 >= max ) SMenu[MN_SKILLGUILD_ITEM].nField[2].nType=FT_NOMAL_PUT;
													else SMenu[MN_SKILLGUILD_ITEM].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;

													if( curr-1 < 0 ) SMenu[MN_SKILLGUILD_ITEM].nField[4].nType=FT_NOMAL_PUT;
													else SMenu[MN_SKILLGUILD_ITEM].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
												}
												MainOn = true; break;
											}
				case SWD_BANK_ITEM :{
										is_mainmenu_active = true;
										y_MenuFocus = 0;
										fRMoveMenu = MM_ITEM_OPEN;
										nItemOther = SMenu[i].nField[j].nWillDo;
										CallServer( CMD_SEND_BANK_INV );			// bank ¾ÆÀÌÅÛ ¼­¹ö¿¡¼­ Àü¼Û¹ÞÀ½.
										MP3( SN_MOVE_OPEN );
										break;
									}
				case SWD_BANK_ITEM_END :	if(SMenu[MN_ITEM].bActive) 
											{
												fRMoveMenu = MM_ITEM_CLOSE;
												MP3( SN_MOVE_CLOSE );
												MainOn = true; break;
											}
											break;
				case SWD_SELL_BUY :	{	
									memset( aSellBuyItemAttr, 0, sizeof( aSellBuyItemAttr ) );
									int menu =  SMenu[i].nField[j].nWillDo;
									int shop = i-MN_BLACKSMITH;
									switch( menu )
									{
										case MN_SHOP_BUY	:		shop = STORE; is_buymenu_On = true; break;
										case MN_DRINK_BUY	:		shop = PUB;	 is_buymenu_On = true; break;
										case MN_SKILLGUILD_BUY :	
											{
												if( i == MN_DRINK_MAIN )	shop = PUB;
												else if( i== MN_SHOP_MAIN )	shop = STORE;
												else if( i== MN_MAGIC_GUILD ) shop = MAGIC_GUILD;
												else shop = i-MN_BLACKSMITH; 
												is_buymenu_On = true; break;
											}
										case MN_SHOP_SELL	:		shop = STORE; is_buymenu_On = false; break;
										case MN_SKILLGUILD_SELL :	
											{
												if( i== MN_SHOP_MAIN )	shop = STORE; 
												else shop = i-MN_BLACKSMITH; 
												SMenu[menu].work = -1;
												is_buymenu_On = false; 
												break;
											}
										case MN_SKILLGUILD_REPAIR : shop = i-MN_BLACKSMITH; is_buymenu_On = false; break;
									}
									ReqSmallMenu( menu, shop );
									y_MenuFocus = menu;
									SmallMenuClose( );
									SMenu[menu].nTemp = i; // -> ¹«½¼ ±æµåÀÎÁö¸¦ ÀúÀå
									break;
								} 

				case SWD_ITEM_MAKE : {
										// ÀÛ¾÷´ë À§¿¡ ³õ¿© ÀÖ´Â ¾ÆÀÌÅÛÀº make_item_list[]
										SendSkillMatter( make_item_list );
										SendSkillInfo( SkillNo, Mox, Moy, SKILLTARGETTYPE_TABLE_, item_select_result.item_no);
										for( int i=0; i<20; i++ )		// °ø±¸Ã¢¿¡ ÀÖ´Â Àç·á´Â ¸ðµÎ »ç¶óÁø´Ù.
											if( make_item_list[i].item_no ) DeleteItem( make_item_list[i].pos ) ;
											else break;

										SkillSettingClear( );
										break;
									 }
				case SWD_GABLE_ITEM :
					{
						if( !SMenu[i].key )
						{
							SMenu[i].key = 1;
						}
						MainOn = true;
						break;
					}
				case SWD_ITEM_MAKE_CANCEL : {
												SkillStatus				= SKILL_OFF;
//												SkillNogadaCount = 0;		//  ÀÌ Ä«¿îÆ®°¡  Max¿¡ ÀÌ¸£¸é SendSkillInfo()¸¦ È£ÃúÇÑ´Ù. 
//												SkillMouseDontMoveFlag = false;	//	³ë°¡´Ù¸¦ ÇØ¾ß ÇÏ´Â ±â¼úÀº ÇÑ¹ø ±×°÷À» ¼±ÅÃÇÏ¸é   ¿À¸¥ÂÊ¸¶¿ì½º·Î Ãë¼ÒÇÏµç°¡ NogadaCount°¡ Max°¡ µÉ¶§±îÁö ±â´Ù·Á¾ß ÇÑ´Ù. 
												nItemOther = 0 ;
												//SkillNo =  1230 KHS
												SkillSettingClear( );
												SmallMenuClose();
												break;
											}

				case SWD_ITEM_MAKE_SELECT_OK : 
					{
						if( !item_select_result.item_no ) 
						{
							MainOn = true;
							break;
						}
						//011025 lsw >
						if( !skillmakeiteminfo.MakeMuch ) 
						{
							MainOn = true;
							break;
						}
						//011025 lsw <

						//011001 lsw
						for(int ji = 0 ; ji < ITEM_JOIN_MAX ;ji++)
						{
							if( item_join[ji].item_id == item_select_result.item_no)
							{
								item_select_result.join_no = ji;
								break;
							}
						}
						SMenu[i].nTemp = 0;
						y_MenuFocus = 0;
						//011013 lsw >
						Send2NDSKILLMAKE(0,0);
						SkillSettingClear();
						//011013 lsw <
						break;
					   }
					case	SWD_ITEM_MAKE_SELECT_RARE:
					{
						if( item_select_result.join_no && skillmakeiteminfo.MakeMuch)
						{
							skillmakeiteminfo.join_no=item_select_result.join_no;
						//	skillmakeiteminfo.MakeMuch =1;//020530 lsw Make Multi
							SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[0].nWillDo =-1;
							
							CItem *t = ItemUnit(item_select_result.item_no);
							if(!t)break;
													
							const int iItemKind = t->GetItemKind();
							if(MAX_ITEM_MUTANT_KIND <= iItemKind  || 0 > iItemKind )
							{
								break;
							}
							const int iUpgradeType = ItemMutantKind[iItemKind].upgrade_type;
							switch (iUpgradeType )
							{
							case 1:
								{
									SMenu[MN_ITEM_MAKE_RARE_SELECT].key = 0;
								}break;
							case 2:
							case 3:
							case 4:
								{
									SMenu[MN_ITEM_MAKE_RARE_SELECT].key = iUpgradeType;
								}break;
							default:
								{
									return;
								}break;
							}
							
							for(int iCount =0; iCount<8;iCount++ )//020214 lsw
							{
								SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[iCount+16].nImageNumber = 0;
								strcpy(SMenu[MN_ITEM_MAKE_RARE_SELECT].nField[iCount+16].temp," ");
							}
							CallSmallMenu(MN_ITEM_MAKE_RARE_SELECT);
						}
						else
						{
							AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,502));
						}
						MainOn = true;
						break;
					}
				//011024 lsw >
				case	SWD_ITEM_MAKE_SELECT_RARE_OK:
					{
						MainOn = true;
						int iGroup  = SMenu[i].nField[j].nWillDo;//·¹¾î ±×·ì °ª;
						int itemLv = item_join[skillmakeiteminfo.join_no].skill1_minimum;
						
						int sno = SkillTable[SkillNo].MotherSkillType;
						int myskillLv = SCharacterData.nJobSkill[sno].skillexp/10000;
						//011030 lsw >
						if(SCharacterData.nJobSkill[sno].rare + RareProbability[myskillLv - itemLv].bonus_suc > 999 )
						{
						//	skillmakeiteminfo.MakeMuch =1;//020530 lsw Make Multi
						//	AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,252));//252
						//	AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,253));//253
							
							strcpy(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,  "0" );
							if( iGroup )
							{
								//011031 lsw >
								t_SkillExp2 *dayexp ;
								dayexp = (t_SkillExp2*)&SCharacterData.nJobSkill[sno-14];

								if( dayexp->makecount >= RareProbability[myskillLv].max_rare_day)
								{
								//	AddCurrentStatusMessage(255,255,0,lan->OutputMessage(2,257));
								}
								else
								{
								//	AddCurrentStatusMessage(255,255,0,lan->OutputMessage(2,258),
								//		RareProbability[myskillLv].max_rare_day , dayexp->makecount+1 );//253
								}
								const int iIsDynamicRare = SMenu[MN_ITEM_MAKE_RARE_SELECT].key;
								Send2NDSKILLMAKE(1+(iIsDynamicRare*10),iGroup);
								CloseAllMenu();
								//011031 lsw <
							}
							else
							{
								AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,505));
							}
							
						}
						else
						{
							AddCurrentStatusMessage(255,255,255,lan->OutputMessage(2,251));//"·¹¾î ¼º°ø·üÀÌ ¸ðÀÚ¶ø´Ï´Ù. ´Ù¸¥ ¾ÆÀÌÅÛÀ» ¸¸µé¾î ¼º°ø·üÀ» ³ôÀÌ¼¼¿ä."
						}

						

						//						
						//011030 lsw <
					}
					break;
				//011024 lsw <
				case	SWD_ITEM_MAKE_SELECT_RARE_CANCEL:
					{
						if( SkillStatus != SKILL_OFF )
						{
							CursorNo( 1 );
							SkillStatus = SKILL_OFF;
						}
						y_MenuFocus=0;
						SkillSettingClear( );
						
						skillmakeiteminfo.MakeMuch=0;
						strcpy(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,  "0" );
					//	SkillStatus = SKILL_ON;
					//	SkillOn = true;
						CloseAllMenu();
						CanMakeItemListSet( SkillNo );
					}break;
					//011013 lsw <
				case SWD_ITEM_USE_SELECT_OK : {
												if( !skill_use_item_result.item_no ) 
												{
													MainOn = true;
													break;
												}
												SMenu[i].nTemp = 0;
												y_MenuFocus = 0;

												// ½ºÅ³ »ç¿ë....		KHS
												SkillItemNo =  skill_use_item_result.item_no;
												SkillItemPOS=  skill_use_item_result.pos;

												// 1227 KHS
												SkillStatus = SKILL_READY;


												break;
											   }
				case SWD_ITEM_MAKE_AFTER :	{
												if( make_item_result.item_no )
												{
													QuickItemAttr[6] = make_item_result.attr;		// ¼­¹ö¿Í ¿¬°áµÇ¸é ºüÁ®¾ß ÇÒ ²¬?
													DropItemOnHero( QuickItemAttr[6] );
													memset(&QuickItemAttr[6], 0, sizeof( ItemAttr ) );
													memset( &make_item_result, 0, sizeof( MakeItem ) );
												}
												break;
											}
				case SWD_DIVIDE_OK:	{
										int &much = SMenu[i].nField[0].nShideNomalPlus;
										DivideItem( &i_DivideItem, much );
										SetFocus2(HWND_GAME_MAIN);
										EWndMgr.ClearAllTxt();//021001 lsw
										y_MenuFocus = 0;
										break;
									}
				case SWD_TOWN_CONSTRUCTION :	{
													SMenu[i].work = !SMenu[i].work;		// ÁöºØ ¿­¾ú´Ù ´Ý¾ú´Ù°¡ ÇÑ´Ù.
													MainOn = true; break;
												}
				case SWD_LEARN	: {
									if( i == MN_HEAL_LEARN )
									{		// º´¿ø¿¡¼­ healing ¹è¿ì±â
										if( SCharacterData.SkillId[86] ) 
										{
											MainOn = true; break;
										}
										else
										{
											SCharacterData.SkillId[86] = true;
											SendLearnSkill( 86, HOSPITAL ); // ¼­¹ö Àü¼Û 
											MP3( SN_MENU_OPEN );
											MainOn = true; break;
										}
									}
									break;
								  }

				case SWD_EDIT_BOX : SMenu[SMenu[i].nField[j].nWillDo].bActive=TRUE;
									MP3( SN_MENU_OPEN );
					 				SetFocus2(HWND_1);//021001 lsw
									id_password = true;
									break;

				case SWD_BANK_OK : {
									int menu = SMenu[i].Id;
									if( !nBankTemp )		// µ·ÀÌ 0 ÀÏ¶§ OK¹öÆ°Àº ÀºÇà ¸Þ´º¸¦ ´Ý¾Æ ¹ö¸°´Ù.
									{
										SmallMenuClose();
										break;
									}

									MP3( SN_GET_MONEY );
									switch(menu)
									{
										case MN_BANK_DEPOSIT :	SCharacterData.BankMoney += nBankTemp;
																SendBankInfo(REQ_MY_DEPOSIT_INFO,  nBankTemp);
																nBankTemp = 0;
																MainOn = true; break; 
										
										case MN_BANK_DEFRAYAL :	SCharacterData.BankMoney -= nBankTemp;
																SendBankInfo(CMD_DEFRAYAL, nBankTemp);
																nBankTemp = 0;
																MainOn = true; break; 
										
										case MN_BANK_LOAN :		SendReqLoan( nBankTemp );
																SCharacterData.LastLoan += nBankTemp;
																nBankTemp = 0;
																g_RecvLoanInfo = 0;
																MainOn = true; break; 

										case MN_BANK_REPAYMENT :	SendReqRepayment( nBankTemp );
																	SCharacterData.LastLoan += nBankTemp;
																	nBankTemp = 0;
																	g_RecvLoanInfo = 0;
																	MainOn = true; break; 
									}

									char temp[20] = {0,};
									sprintf( temp, "%d", nBankTemp );
									EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
								   }break;

				case SWD_LEARNSKILL_OK : {
											nSkillLearnResult = 0;
											int a=SMenu[i].nField[ 6 ].nSHideNomalStart;
											int &s = SkillInventory[a][0][0];	
											if( s )
											{
												nSkillLearnResult = skill[s].CanLearnSkill( SMenu[i].nTemp - MN_BLACKSMITH );	// ¼­¹ö ÄÝµµ ÇÔ¼ö¾È¿¡¼­ °°ÀÌ ÇÑ´Ù.
											}
											MainOn = true; break; 
										 }
				case SWD_LEARNITEM_OK : {
											int para = SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalStart;
											int result = SendLearnItem( learn_item_list[para] );
											
											switch( result )
											{
												case -1 : MP3( SN_WARNING ); CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,158),0 ); break;//010215 lsw
												case -2 : MP3( SN_WARNING ); CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,159),0 ); break;
												case -3 : MP3( SN_WARNING ); CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,160),0 ); break;//010215 lsw
											}
											MainOn = true; break; 

											
										}

				case SWD_ONLY_OK :
				case SWD_OKCANCEL_CANCEL : 
					{
						int is_break = 0;
						if (hammer_itemid != 0){ // Add by rogerio
							hammer_itemid = 0;	
							is_break = 1;
						}
						/*
						 * Rogério Segovia
						 * make items rare
						*/
						if (magicItem_itemid != 0){
							magicItem_itemid = 0;	
							is_break = 1;
						}
						
						switch( y_MenuFocus=SMenu[i].nTemp )
						{
						case MN_DEFAULT_MESSAGEBOX :		// 020701 YGI
							{
								char data = 0;
								CallServer( CMD_DRAGONROAD_VOTE, &data, 1 );
								is_break = 1;
								break;
							}
						case MN_SALVATION2 :
						case MN_SALVATION :		// YGI 020527
						case MN_NATION_ITEM :
						case MN_GUILD_CHANGE_INFO :
						case MN_GUILD_REGIST :
							is_break = 1;
							break;
						case MN_MAIL_WRITE :
							{
								EWndMgr.ClearAllTxt();//021001 lsw
								is_break = 1;
							}break;
						}
						if( is_break ) break;
					}
					
				case SWD_OKCANCEL_OK :		
					{
						// se o hammer tiver on e o item for restaurado e o cara tiver o hammer na bag, ele manda pro server
						// pra restaurar o item x, o problema agora esta sendo, saber qual item é se o cara tiver 
						// mais de 1 item na bag...como fazer ?

						// cancel ¹öÆ° ´­·¶À» °æ¿ì ok¿Í ¶Ò°°ÀÌ µÇÁö ¾ÊÀ¸·Á¸é À§¿¡ Ãß°¡
						MP3( SN_MENU_CLOSE ); 
						SoundOn = false;
						y_MenuFocus=SMenu[i].nTemp;
						int check_param = SMenu[i].key;
						if (hammer_itemid != 0){ // Add by rogerio
							SendItemRepairHammer( hammer_itempos, 0 );								
						}
						/*
						 * Rogério Segovia
						 * make items rare
						*/
						if (magicItem_itemid != 0){
							SendMagicItem( magicItem_itempos, 0 );								
						}
						switch( y_MenuFocus )
						{
						case MN_DEFAULT_MESSAGEBOX :		// 020701 YGI
							{
								if( check_param == 1 )		// µå·¡°ï ·Îµå Âù¼º ¹Ý´ë ¸Þ´ºÀÌ´Ù.
								{
									char data = 1;		// Âù¼º 
									CallServer( CMD_DRAGONROAD_VOTE, &data, 1 );
								}
								SmallMenuClose();
								break;
							}
						case MN_GUILD_CHANGE_INFO :
							{
								if( check_param == 3 )		// ±æµå¸¦ »èÁ¦ ½ÃÅ²´Ù.
								{
									if( !CheckGuildMaster( Hero ) )
									{
										SmallMenuClose();
										break;
									}
									CallServer( CMD_DELETE_GUILD, NULL, 0 );
									SmallMenuClose();
									break;
								}
								else if( check_param == 5 )	// ±æµå¿¡¼­ Å»ÅðÇÑ´Ù.
								{
									if( CheckGuildMaster( Hero ) )
									{
										SmallMenuClose();
										break;
									}
									SendExitGuild();
									SmallMenuClose();
									break;
								}
								else if( check_param == 4 )		// ¼­¹ö ¸¶½ºÅÍ¸¦ ±æ¸¶·Î µî·ÏÇÑ´Ù.
								{
									if( CheckGuildMaster( Hero ) )
									{
										CGuild *pCurrGuild = &g_GuildMgr[Hero->GetGuildCode()]; // CSD-030324

										if( pCurrGuild->m_szSubGuldMasterName[0] )
										{
											// ºÎ±æµå ¸¶½ºÅÍ¸¦ Ã¼Å©ÇÑ´Ù. ¹®Á¦ ¾øÀ¸¸é ¹Þ´Â °÷¿¡¼­ ¹Ù·Î ±³Ã¼ ¸í·ÉÀ» ¼­¹ö·Î º¸³½´Ù.
											g_Menu_Variable.m_nCheckSubMaster = CS_CHANGE_MASTER;
											SendCheckSubGuildMaster( pCurrGuild->m_szSubGuldMasterName );
										}
										else
										{
											Kein_PutMessage( KM_FAIL, kein_GetMenuString(138) );
										}
									}
									else if( IsSubMaster( Hero ) )		// ºÎ ±æ¸¶ ÀÌ°Åµç...
									{	//< CSD-030324
										SendChangeGuildMasterBySubMaster( CMD_CHANGE_GUILD_MASTER_SUB, Hero->GetGuildCode(), Hero->name );
									}	//> CSD-030324

									SmallMenuClose();
									break;
								}
								break;
							}
						case MN_LEARN_MAGIC_BOOK :
							SMenu[MN_LEARN_MAGIC_BOOK].CheakType= 0;		// ¸ÅÄ¢ºÏ ¸Þ´º¸¦ ´Ù½Ã ÃÊ±âÈ­
							break;
						case MN_SALVATION :
						case MN_SALVATION2 :
							{
								if( nBankTemp < 1000 ) break;		// 1000 cellÀÌ»ó ±âºÎ ¸øÇÔ..
								SendSalvationMoney( nBankTemp );		// ±âºÎÇÑ´Ù.
								TotalSalvation += nBankTemp;
								nBankTemp = 0;
								char temp[20];
								sprintf( temp, "%d", nBankTemp );
								EWndMgr.SetTxt(HWND_3,temp);//021001 lsw
								g_Menu_Variable.bCheckSalvation = 0;	// YGI 020527
							}break;
						case MN_GUILD_REGIST :
							SmallMenuClose();
							break;
						case MN_NATION_ITEM :
							{
								CallServer( CMD_BUY_NATION_ITEM );
								break;
							}
						case MN_MAIL_MENU :
							{
								g_MailMgr.SendDelete(0);
							}break;
						case MN_MAIL_READ :
							{
								g_MailMgr.SendDelete(1);
							}break;
//						case MN_MAIL_READ ://030701 lsw
//							{
//								g_MailMgr.SendDelete(SubMenu->nField[i].nWillDo);
//							}break;
						}						
						break;
					}

				case SWD_CALL_OKCANCEL_BOX_ETC :		// ÁöÁ¤ ÇÔ¼ö¸¦ ½ÇÇà ½ÃÅ²ÈÄ ¾Æ·¡ SWD_CALL_OKCANCEL_BOX ºÎºÐ ½ÇÇà
					{
						void (*MenuFuction)() = SubMenu->nField[j].m_pFunction;
						if( MenuFuction )
						{
							(*MenuFuction)();		
						}
						else 
						{
							MainOn = true; break;
						}
					}
				case SWD_CALL_OKCANCEL_BOX :
					CallOkCancelMessageBox( y_MenuFocus,0,0, SMenu[i].nField[j].temp,1, j );
					MainOn = true; break;
				case SWD_SORRY :			CallOkCancelMessageBox( y_MenuFocus,0,0, lan->OutputMessage(3,161),0 );
											MainOn = true; break; 
				case SWD_SMALLMENU_CLOSE :		// ´ÜÃàÅ°
				case SWD_SMALLMENU_END ://soto-030501
					{
						
						if(LottoMgr() && i == MN_LOTTO_MENU && (LottoMgr()->GetCurLottoNumbers())[0])
						{
							LottoMgr()->ClearCurLottoNumbers();
							MainOn = true;
						}
						else
						{
							SmallMenuClose();
						}
						break;
					}
				case SWD_UNMOUSEFOCUS:		y_MenuFocus=0;
											MP3( SN_MENU_CLOSE );
											break;

				case SWD_QUIT :		
					{	//< CSD-040310
						if (Hero != NULL) 
						{
							if( Hero->start_Casting > g_ClientTime )
							{
								const int nOffset = Hero->start_Casting - g_ClientTime;
								
								if (nOffset > 0)
								{
									if (nOffset > 3000)  Hero->start_Casting = g_ClientTime + 3000;
									MainOn = true;
									break;
								}
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
					}break;
				case SWD_GM_2ND_OK : //ÀÌÁ¦ ¸¸µé°Ú´Ù´Â ½ÅÈ£¸¦ º¸³»´Â °Í
					{
//						MainOn = true;
						SkillOn = false;
						Send2NDGMMAKE(SkillNo);
					}break;
				case SWD_GM_2ND_CANCEL :
					{
						SkillStatus				= SKILL_OFF;
						nItemOther = 0 ;
						SkillSettingClear();
						SmallMenuClose(); 
					}break;
//011004 lsw <
				case SWD_ITEM_SELLBUY_OK :{
											int &item = SMenu[i].work;			// »ç°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛ 
											int &item_pos = SMenu[i].work;		// sellbuy ¾ÆÀÌÅÛ¿¡¼­ÀÇ À§Ä¡ ( ÆÈ±â ¸ðµå ÀÏ¶§ )
											if( SMenu[i].nField[j].nWillDo == ITEM_BUY )
											{
												
												int price = GetItemAttr(item, VALUE);
												int a, b, c;
												if( !SearchEmpty( a, b, c) ) 
												{
													MP3( SN_WARNING );
													CallOkCancelMessageBox( MN_SKILLGUILD_BUY, 0,0, lan->OutputMessage(3,162), 0);//010215 lsw
													MainOn = true; break;
												}
												//4003 - pot
												// if abaixo verifica se o item eh item do mall para nao debitar money do char
												// caso o tonto do hacker conseguir mudar o item do jogo, vai ser debitado grana 
												// e a yellow sgnet :P

												if (item == 10290 ||item == 10292 || item == 10298 || item == 10291 || item == 1053 || item == 10301 || item == 8144 || item == 1046 || item == 10304 || item == 3105 || item == 6430 || item == 6431 || item == 6432 || item == 6433 || item == 6434 || item == 6435 || item == 6436 || item == 6437 || item == 6438 || item == 6439 || item == 6440 || item == 6441 || item == 6442 || item == 6443 || item == 6444 || item == 6445 || item == 6446 || item == 6447 || item == 6448 || item == 6449 || item == 6450 || item == 6451 || item == 8042 || item == 8043 || item == 9044 || item == 10223 || item == 6960 || item == 6961 || item == 6962 || item == 10192 || item == 8318 || item == 8317 || item == 8319 || item == 8320 || item == 8144 || item == 8307 || item == 8306 || item == 10302 || item == 10296 || item == 10282 || item == 10235 || item==31 || item==32 || item==33 || item==25 ||item==26 ||item==27 || item==28 || item==29 || item==30){
													if( GetItemMuchByMuchItem( 10057 ) < price )
													{
														MP3( SN_WARNING );
														CallOkCancelMessageBox( MN_SKILLGUILD_BUY, 0,0, lan->OutputMessage(3,207), 0 );//010215 lsw
														MainOn = true; break;
													}else{														
														SendItemBuy( item, ((a<<5) | (b<<3) | c), storelistindex );
														MP3( SN_ITEM_BUY );	// ¾ÆÀÌÅÛ »ç±â
														AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(3,206), GetItemName( item ), price );
													}
												}else{
													if( item && ( price > 0 ) )
													{
														if( SCharacterData.nMoney >= (DWORD)price )
														{														
															
															// InvItemAttr[a][b][c] = Item_Number2Attr( item ); Àü¼Û ¹Þ´Â´Ù...
															SubtractMoney( price );
															SendItemBuy( item, ((a<<5) | (b<<3) | c), storelistindex );
															MP3( SN_ITEM_BUY );	// ¾ÆÀÌÅÛ »ç±â
															//ItemSoundOfGetItem( g_MouseItemNumber );		// sound
															#ifdef CHANGE_MONEY_
																AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(3,163), GetItemName( item ), price );//010215 lsw
															#else
																AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(3,164), GetItemName( item ), price );//010215 lsw
															#endif
															//item = 0;		// ¾ÆÀÌÅÛ »ç±â¿¡ ¼º°øÇÏ¸é À½¾Ç°ú ÇÔ²² ¼±ÅÃ ¾ÆÀÌÅÛ »ç¶óÁø´Ù.
														}
														else if(!MessageOn)
														{
															MP3( SN_WARNING );
															CallOkCancelMessageBox( MN_SKILLGUILD_BUY, 0,0, lan->OutputMessage(3,165), 0 );//010215 lsw
															MainOn = true; break;
														}
													}
												}
											}
											else if( SMenu[i].nField[j].nWillDo == ITEM_SELL )
											{
												if( SMenu[i].work == -1 ) 
												{
													MainOn = true; 
													break;
												}
												int pos = aSellBuyItemPos[item_pos];		// ÀÎº¥Åä¸®¿¡¼­ÀÇ ¾ÆÀÌÅÛ À§Ä¡ 
												int i_a = (pos& 0x00000060) >> 5 ;
												int i_b = (pos& 0x00000018) >> 3 ;
												int i_c = (pos& 0x00000007) ;

												int item_no = InvItemAttr[i_a][i_b][i_c].item_no;
												if( !InvItemAttr[i_a][i_b][i_c].item_no  )
												{
													CallOkCancelMessageBox( i, 0,0, lan->OutputMessage(3,166), 0 );//010215 lsw
													SMenu[i].work = -1;
													MainOn = true;
													break;
												}
												DWORD money = GetItemValueSell( InvItemAttr[i_a][i_b][i_c] );
												if( !money ) 
												{ 
													//AddCurrentStatusMessage( 200,50, 50 , "°¡°ÝÀ» ´Ù½Ã È®ÀÎÇÏ½Ê½Ã¿ä.", GetItemName( item_no ), money );
													SMenu[i].work = -1;
													MainOn = true; 
													break; 
												}
												MP3( SN_ITEM_SELL );
												SendItemSell( pos, storelistindex );
//												AddMoney( money );
#ifdef CHANGE_MONEY_
												AddCurrentStatusMessage( 255,255, 0 , lan->OutputMessage(3,167), GetItemName( item_no ), money );//010215 lsw
#else
												AddCurrentStatusMessage( 255,255, 0 , lan->OutputMessage(3,168), GetItemName( item_no ), money );//010215 lsw
#endif
												DeleteItem( &InvItemAttr[i_a][i_b][i_c] );
												DeleteItem( &aSellBuyItemAttr[item_pos] );
												SMenu[i].work = -1;
											}
											else if( SMenu[i].nField[j].nWillDo == ITEM_REPAIR )
											{
												if( SMenu[i].work == -1 ) 
												{
													MainOn = true; 
													break;
												}
												int pos = aSellBuyItemPos[item_pos];		// ÀÎº¥Åä¸®¿¡¼­ÀÇ ¾ÆÀÌÅÛ À§Ä¡ 
												int i_a = (pos & 0x00000060) >> 5 ;
												int i_b = (pos & 0x00000018) >> 3 ;
												int i_c = (pos & 0x00000007) ;

												ItemAttr &item = InvItemAttr[i_a][i_b][i_c];

												if( item.item_no )
												{
													WORD d_curr;
													WORD d_max;
													GetItemDuration( item, d_curr, d_max );
													if( d_curr == d_max )
													{
														MP3( SN_WARNING );
														CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,169),0 );//010215 lsw
													}
													else SendItemRepair( pos, storelistindex );
												}
												SMenu[i].work = -1;
											}
											MainOn = true; break;
										}

				case SWD_ITEM_SELLBUY_CANCEL :	{
													CallSmallMenu( SMenu[i].nTemp );
													for( int k=0; k< SMenu[i].nFieldCount; k++)
													{
														if( SMenu[i].nField[k].nType == FT_HIDE_ISCROLL_AUTO_PUT )
														{
															SMenu[i].nField[SMenu[i].nField[k].nWillDo].nSHideNomalStart = 0;
															break;
														}
													}													
													break;
												}
				case SWD_MALL_ITEM_BUY: // Buying of an item from the ingame item mall
					{
						int a, b, c;
						if( !SearchEmpty( a, b, c) ) 
						{
							MP3( SN_WARNING );
							CallOkCancelMessageBox( MN_SKILLGUILD_BUY, 0,0, lan->OutputMessage(3,162), 0);//010215 lsw
							MainOn = true; break;
						}

						int &item = SMenu[i].work;
						int rajaEmblemPrice = GetItemAttr(item, RAJA_EMBLEM);
						
						if (item && rajaEmblemPrice > 0)
						{
							
							if (GetItemMuchByMuchItem(10057) < rajaEmblemPrice)
							{
								MP3(SN_WARNING);
								CallOkCancelMessageBox(MN_SKILLGUILD_BUY, -90, 20, lan->OutputMessage(3, 207), 0);//010215 lsw
								MainOn = true; break;
							}
							else
							{														
								SendMallItemBuy(item, ((a<<5) | (b<<3) | c));
								MP3(SN_ITEM_BUY);
								AddCurrentStatusMessage(255, 255, 0 , lan->OutputMessage(3,206), GetItemName(item), rajaEmblemPrice);
							}
						}

						MainOn = true; break;
					}
					break;
				case SWD_CALLMENU	:	
					{
						CallMenu( SMenu[i].nField[j].nWillDo, i, (bool)true );						
					}
					break;

				case SWD_SUBMENU_BACK : EWndMgr.ClearAllTxt();//021001 lsw
										CallMenu( SMenu[i].nTemp, 0, (bool)true );
										break;
				case SWD_MANTLEDYE_CANCEL:
										if (EquipItemAttr[ WT_NECK].attr[IATTR_RESERVED1] > 0)
										{
											int color = EquipItemAttr[ WT_NECK].attr[IATTR_RESERVED1];
											int r, g, b;
											r = (color>>16);
											color -= (r<<16);
											g = (color>>8);
											color -= (g<<8);
											b = color;
											Hero->mantlecolor = ReturnBlendRGB(r, g, b);
										}
										else
										{
											Hero->mantlecolor = GetItemAttr( Hero->mantle, ITEM_COLOR );
										}
										break;
				case SWD_SKILL_CANCEL : //SMenu[SMenu[i].nTemp].bActive = true;
										//y_MenuFocus = SMenu[i].nTemp;

										// ½ºÅ³ Ãë¼Ò		KHS
										if( SkillStatus != SKILL_OFF )
										{
											CursorNo( 1 );
											SkillStatus = SKILL_OFF;
										}
										y_MenuFocus=0;
										//011025 lsw >
										skillmakeiteminfo.MakeMuch = 0;
										SMenu[MN_ITEM_MAKE_SELECT].nField[10].nWillDo	=0;
										//011025 lsw <
										SkillSettingClear( );
										break;

				case SWD_TOWNHALL_TITLE:	{
												int count = 0;	// 10ÀÌ ³Ñ¾î°¥¶§ ¿äÃ»

												switch( SMenu[i].nField[j].nWillDo )
												{
													case	MN_TOWNHALL_PUBLICNOTICE :	
													case	MN_TOWNHALL_FORUM :			
													case	MN_TOWNHALL_EVENT :			
													case	MN_TOWNHALL_RANK1 :		ReqMessageTitle( SMenu[i].nField[j].nWillDo, count );	// ¼­¹ö ÄÝ ;
												}

												CallMenu( SMenu[i].nField[j].nWillDo, i, true );
												break;
											}
				case SWD_BBS_READ : {
										if( !SMenu[i].work ) return;
										int index = SMenu[i].nField[SMenu[i].nField[SMenu[i].work].nSHideNomalStart].nSHideNomalStart+SMenu[i].nField[SMenu[i].work].nSHideNomalCount ; // ¹¹°¡ ¸¹ÀÌ º¹ÀâÇÏÁö¸é °á±¹ ¼±ÅÃµÈ Ç×¸ñÀÇ ¹øÈ£¸¦ °¡¸®Å´

										SMenu[i].work = 0;
										int next = SMenu[i].nField[j].nWillDo;
										CallMenu( next, i, true);
										SMenu[next].work = index;
										
										if( c_BbsTitle[index].is_first )
										{
											ReqMessageBbs( i, c_BbsTitle[index].m_Number ); // ¼­¹ö ÄÝ index ¸¦ ³Ñ°ÜÁÖ°í µ¥ÀÌÅ¸ s_BbsTextBuf[ MAX_STRING_ ] ¸¦ ¹Þ¾Æ ¿Â´Ù.
											sprintf( s_BbsTextBuf ,lan->OutputMessage(7,50) , index );		// ÀÓ½Ã//010215 lsw
											SMenu[next].nField[0].nSHideNomalCount = CalcTextToPage( SMenu[next].nField[0].nRectImage, SMenu[next].nField[0].nWillDo, s_BbsTextBuf );		// Ã³¸®ÇØ¾ßÇÒ ¹®Á¦
										}
										else 
										{
											SMenu[next].nField[0].nSHideNomalCount = CalcTextToPage( SMenu[next].nField[0].nRectImage, SMenu[next].nField[0].nWillDo, c_BbsTitle[index].m_szMessage );
										}
										SMenu[next].nField[0].nSHideNomalStart = 0;

										break;
									}
				case SWD_SELECT_HOUSEKIND : {
												int value = SMenu[i].nField[j].nWillDo;
												int para = SMenu[i].nField[j].nSHideNomalCount;
												SMenu[i].nField[para].nWillDo = value;
												SMenu[i].nField[para].nSHideNomalStart = 0;

												switch( value ) 
												{
													case RESIDING :		LoadHouse( );	break;
													case GUILD :		LoadHouse( );	break;	// ÀÓ½Ã
													case NOTRESIDING :	LoadHouse( );	break;	// ÀÓ½Ã
												}
												int para2 = SMenu[i].nField[j].nSHideNomalStart; // ¼±ÅÃ¼± À¯Áö
												SMenu[i].nField[para2].nImageNumber = SMenu[i].nField[j].nImageNumber;
												SMenu[i].nField[para2].x = SMenu[i].nField[j].x;
												SMenu[i].nField[para2].y = SMenu[i].nField[j].y;

												MainOn = true; break;
											}
				case SWD_HEALING_OK :	{
											if( !SMenu[i].work ) 
											{
												MP3( SN_WARNING );
												MainOn = true; break;
											}
											int willdo = SMenu[i].nField[j].nWillDo;	// ½ºÅ©·Ñ ¿©ºÎ È®ÀÎ
											int disease_num = SMenu[i].nField[willdo].nWillDo;

											if( SCharacterData.nMoney < (DWORD)disease_tbl[disease_num].price_heal )
											{
												MP3( SN_WARNING );
												CallOkCancelMessageBox( i, 0,0, lan->OutputMessage(3,165), 0 );//010215 lsw
												MainOn = true; break;
											}
											SendClientCureDisease( disease_num );		// Ä¡·á¸¦ À§ÇÑ ¼­¹ö ÄÝ
											int menu = SMenu[i].nTemp;
											CallMenu( menu );
											if( !is_mainmenu_active ) y_MenuFocus = menu;
											break;
										}
				case SWD_PREVENTING_OK :{
											if( !SMenu[i].work ) 
											{
												MP3( SN_WARNING );
												MainOn = true; break;
											}
											int willdo = SMenu[i].nField[j].nWillDo;	// ½ºÅ©·Ñ ¿©ºÎ È®ÀÎ
											int disease_num = SMenu[i].work-1+ SMenu[i].nField[willdo].nSHideNomalStart;

											if( SCharacterData.nMoney < (DWORD)disease_tbl[disease_num].price_prevent )
											{
												MP3( SN_WARNING );
												CallOkCancelMessageBox( i, 0,0, lan->OutputMessage(3,165), 0 );//010215 lsw
												MainOn = true; break;
											}
											SendClientPreventing( disease_num );
									
											int menu = SMenu[i].nTemp;
											CallMenu( menu );
											if( !is_mainmenu_active ) y_MenuFocus = menu;
											break;
										}

				case SWD_FOCUS_WILLDO : 
										if( SMenu[i].nField[j].nWillDo == MN_GOD_SUPPER )
										{
											int god = i - MN_GOD_EDELBLHOY+1;
											if( !SCharacterData.believe_god || SCharacterData.believe_god != god ) 
											{
												AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(3,172)); break;//010215 lsw
												MP3( SN_WARNING );
												MainOn = true; break;
											}
											CallServer( CMD_OPEN_SUPPER );
											break;
										}
										y_MenuFocus = SMenu[i].nField[j].nWillDo;
				case SWD_WILLDO :	{
										int menu = SMenu[i].nField[j].nWillDo;
										switch( menu )
										{
											case MN_TOWNHALL_COMMERCE2 : 
											case MN_BANK_DEPOSIT  :	nBankTemp = 0; SendBankInfo(REQ_MY_DEPOSIT_INFO, 0); break;
											case MN_BANK_REPAYMENT:	nBankTemp = 0; SendBankInfo(REQ_MY_REPAYMENT_INFO, 0); break;
											case MN_BANK_DEFRAYAL :	nBankTemp = 0; SendBankInfo(REQ_MY_DEPOSIT_INFO, 0); break;
											//case MN_BANK_LOAN :	nBankTemp = 0; break;
											case MN_BANK_AUCTION : nBankTemp = 0; break;
										}
										CallMenu( menu, i );
										if( !is_mainmenu_active ) y_MenuFocus = menu;
										break;
									}
				case SWD_MEMORIZE_BUTTON :{
											if( SMenu[i].work ) 
											{
												int num = memorize[SMenu[i].work-1];
												if( !DoMemorize( num ) ) 
												{
													MP3( SN_WARNING );
													CallOkCancelMessageBox(i, 0,0,lan->OutputMessage(3,174),0);//010215 lsw
												}
												else 
												{
													MP3( SN_DO_MEMORIZE );
//													int sound = ReturnCastingWord( Hero, num+SCharacterData.nCharacterData[SPELL]*150 );
//													MP3( sound );													
													AddCurrentStatusMessage(255,255,255, lan->OutputMessage(3,175), magic[num].GetName());//010215 lsw
//													MagicSetting();	// ¸ÞÁ÷Ã¢ Àç ¼ÂÆÃ...
													SMenu[i].work = 0;
												}												
											}
											MainOn= true; break;
										 }
				case SWD_INN_OK	: {
									// ¼­¹ö Àü¼Û
									if( SMenu[i].work )
									{
										if( i == MN_INN_SLEEP )	
										{
											SCharacterData.sleep = CSC_SLEEP;
											SendClientInn( SMenu[i].work );
											MP3( SN_SLEEP, 1 );	 // ÀáÀÚ±â À½¾Ç...
										}
										else 
										{
											SCharacterData.sleep = CSC_REST;
											SendClientInn( SMenu[i].work+1 );
											MP3( SN_REST, 1 );	 // ½¬±â À½¾Ç...
										}
										SmallMenuClose();
									}
									else 
									{
										MP3( SN_WARNING );
										MainOn=true; 
									}
									break;
								  }
			case SWD_ITEM_EXCHANGE_CANCEL :	{
												CallServer( CMD_EXCHANGE_CANCEL );
												break;
											}
			case SWD_GO_BBS_WRITE : 
				{
					int willdo = SMenu[i].nField[j].nWillDo;
					CallSmallMenu( willdo );
					SetFocus2(HWND_BBS);//021001 lsw
				}break;
			case SWD_BBS_WRITE_SEND :
			case SWD_BBS_WRITE_EXIT :
				{
					EWndMgr.ClearAllTxt();//021001 lsw
					SmallMenuClose();
					SetFocus2(HWND_GAME_MAIN);//021001 lsw
				}break;
			case SWD_GOD_REGIST_OK :{
										int next_menu;
										if( !SCharacterData.believe_god )
										{
											next_menu = MN_GOD_PREACH;
					 						SetFocus2( HWND_3 );//021001 lsw
											MP3( SN_MENU_OPEN );
										}
										else
										{
											next_menu = MN_GOD_REGIST_ERROR;//menu = ;
											MP3( SN_WARNING );
										}
										CallMenu( next_menu, SMenu[i].nTemp, true);
										
										break;
									}
			case SWD_PREACH_OK :{//021001 lsw
									const int believe = GetBelieveGodByMenu( SMenu[i].nTemp, NULL );
									const int willdo = SMenu[i].nField[j].nWillDo;
									char *name = SMenu[i].nField[willdo].temp;
									EWndMgr.GetTxt(HWND_3, name, 20 );
									SendPreachName( name, believe);
									EWndMgr.ClearAllTxt();
									SmallMenuClose();
								}break;
			case SWD_PREACH_CANCEL : {
										EWndMgr.ClearAllTxt();//021001 lsw
										int believe = GetBelieveGodByMenu( SMenu[i].nTemp, NULL );
										SendGodRegistOk( believe );
										SCharacterData.believe_god = believe;
										SCharacterData.faith = 10;		// ½Å¾Ó µî·Ï ±âº» ¼öÄ¡
										MP3( SN_GOD_REGIST );
										CallGodMenuMessage( lan->OutputMessage(3,176), believe );//010215 lsw

										break;
									 }
			case SWD_GOD_PREACH_RESULT_OK : {
												int believe_god = GetBelieveGodByMenu( SMenu[i].nTemp, NULL );
												SendGodRegistOk( believe_god );				// ³­ ±× ½ÅÀü¿¡ µî·ÏÇß´Ù.
												SendPreachNameOk( SMenu[i].nField[j].temp, believe_god );	// ³ª¸¦ ÀüµµÇÑ »ç¶÷¿¡°Ô ÇìÅÃÀ» ÁØ´Ù.
												SCharacterData.believe_god = believe_god;
												SCharacterData.faith = 10;		// ½Å¾Ó µî·Ï ±âº» ¼öÄ¡
												MP3( SN_GOD_REGIST );
												
												// µî·ÏµÇ¾ú½À´Ï´Ù. ¶ó´Â ¸Þ½ÃÁö ¶ç¿ì±â
												CallGodMenuMessage( lan->OutputMessage(3,176), believe_god );
												break;
											}

			case SWD_GOD_REGIST_ERROR_OK : {
												int next_menu;
												int believe = GetBelieveGodByMenu( SMenu[i].nTemp, NULL );
												if( SCharacterData.believe_god == believe )		// ÀÌ¹Ì µî·ÏµÈ°ÍÀ» Ãë¼Ò ½ÃÅ²´Ù.
												{
													CallServer( CMD_GOD_REGIST_CANCEL );
													SCharacterData.believe_god = 0;
													SCharacterData.faith = 0;
													CallGodMenuMessage( lan->OutputMessage(3,177), believe );//010215 lsw
												}
												else 
												{
													// »õ·Î µî·Ï
													SendGodRegistOk( believe );		// believe°¡ 0ÀÎ °æ¿ì´Â Ã¼Å©ÇØÁÖÀÚ
													next_menu = MN_GOD_PREACH;
					 								SetFocus2( HWND_3 );//021001 lsw
													CallMenu( next_menu, SMenu[i].nTemp, true );
												}
												break;
										   }
			case SWD_SUPPER_BACK :	{
										int menu = 0;
										switch( SCharacterData.believe_god )
										{
											case EDELBLHOY :	menu = MN_GOD_EDELBLHOY; break;
											case TEFFERY :		menu = MN_GOD_TEFFERY; break;
											case LETTY	:		menu = MN_GOD_LETTY; break;
											case OREM :			menu = MN_GOD_OREM; break;
											default : break;
										}
										if( menu )
										{
											CallMenu( menu, 0, true );
										}
										else SmallMenuClose();

										break;
									}
			case SWD_TIMEMENU_CLOSE : {
										g_bTimeMenuOn = false;
										if( time_spr[0].img )
										{
											FreeOpenningSpr( &time_spr_buf );		// ½Ã°£ Ç¥½Ã ¸Þ´º ÇÁ¸®
											for( int a= 0 ; a< TIME_MENU_IMG_MAX; a++ ) time_spr[a].img = NULL;
										}
										break;
									  }
			case SWD_HUNTER_REGIST_OK : {
											char name[20]={0, };
											char temp[30]={0, };
											DWORD money;
											char text[1024]={0, };
											EWndMgr.GetTxt( HWND_MAIL1, name, 20 );
											if( !name[0] ) 
											{
												MainOn = true; 
												break;
											}
											if( strcmp( SCharacterData.sCharacterName, " " ) == 0 )
											{
												MainOn = true; 
												break;
											}
											if( strcmp( SCharacterData.sCharacterName, name ) == 0 )
											{
												CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,179) );//010215 lsw
												MainOn = true; 
												break;
											}
											int ret = CheckCharName( name );
											if( !ret )
											{
												CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,180) );//010215 lsw
												MainOn = true; 
												break;
											}
											else
											{
												EWndMgr.GetTxt( HWND_MAIL2, temp, 30 );
												money = (DWORD)atoi(temp);

												static int Max_Money = 5000000;
												if( money < Max_Money ) 
												{
													char temp[512];													
#ifdef CHANGE_MONEY_
													sprintf( temp, lan->OutputMessage(3,181), Max_Money );//010215 lsw`
#else
													sprintf( temp, lan->OutputMessage(3,182), Max_Money );//010215 lsw													
#endif
													CallOkCancelMessageBox( i, 0, 0, temp );
													MainOn = true; 
													break;
												}

												EWndMgr.GetTxt( HWND_MAIL3, text, 1000 );
												ChangeString( text, '\r', ' ');

												if( SubtractMoney( money ) == 1 )
													SendHunterRegist( name, money, text );
											}
											CallMenu( SMenu[i].nTemp, 0, true );
											break;
										}
			case SWD_GO_VOTE:{
								int ret = CheckRegistCandidate( );
								if( ret == -1 ) 
								{
									SendCheckVote( );
								}
								
								SmallMenuClose();
								return;
							 }
			case SWD_VOTER_REGIST_OK : {
										CallServer( CMD_REGIST_CANDIDATE );
										SmallMenuClose();
										return;											
									   }
			case SWD_VOTER_SUPPORT_OK : {		// ÅõÇ¥Çß´Ù.
											SendVoteSupport( SMenu[i].work+1 );
											SmallMenuClose();
											// ÅõÇ¥ »óÈ²ÆÇÀ» ¶ç¿ìÁØ´Ù.
											break;
										}

			case SWD_VOTER_SUPPORT_SCROLL : {	// ´ÙÀ½ »ç¶÷ º¸¿©ÁÖ±â
												int scroll = SMenu[i].nField[j].nWillDo;
												int &work = SMenu[i].work;
												if( scroll == LEFT )
												{
													if( work <= 0 ) 
													{
														MP3( SN_WARNING );
														MainOn = true; 
														break;
													}
													work--;
												}
												else
												{
													if( work >= g_Candidate_max )
													{
														MP3( SN_WARNING );
														MainOn = true;
														break;
													}
													work++;
												}
												MainOn = true; break;
											}

			case SWD_MAN_TO_MAN_CANCEL : {
											SendManToManRefusal( SMenu[i].key );
											break;
										 }
			case SWD_HUNTER_LIST_OK : {
										if( !curr_hunter_list->CompName( SCharacterData.sCharacterName ) )
										{
											MP3( SN_WARNING );
											CallOkCancelMessageBox( i, 0,0, lan->OutputMessage(3,183), 0 );//010215 lsw
											MainOn = true; break;
										}
										if( !curr_hunter_list->CompTargetName( SCharacterData.sCharacterName ) )
										{
											MP3( SN_WARNING );
											CallOkCancelMessageBox( i, 0,0, lan->OutputMessage(3,184), 0 );
											MainOn = true; break;
										}
										if( curr_hunter_list->my_regist || my_hunter_list->CompTargetId( curr_hunter_list->list.index ) )
										{
											MP3( SN_WARNING );
											CallOkCancelMessageBox( i, 0,0, lan->OutputMessage(3,185), 0 );//010215 lsw
											MainOn = true; break;
										}
										CHunterList *temp = my_hunter_list->AddNode( &curr_hunter_list->list );
										if( temp )		// µî·Ï ¼º°ø
										{
											CallOkCancelMessageBox( i, 0,0, lan->OutputMessage(3,186), 0 );
											curr_hunter_list->my_regist = 1;
										}
										else 
										{
											MP3( SN_WARNING );
											CallOkCancelMessageBox( i, 0,0, lan->OutputMessage(3,185), 0 );//010215 lsw
										}
										MainOn = true; break;
									  }
			case SWD_GM_LIST_SCROLL : {
										int &scroll = SMenu[i].work;	// ½ºÅ©·Ñ º¯¼ö										
										int up_down = SMenu[i].nField[j].nWillDo;

										if( up_down == LEFT )		// À§·Î ¿Ã¸®±â
										{
											if( scroll-1 >= 0 ) scroll -- ;
										}
										else if( up_down == RIGHT )	// ¾Æ·¡·Î ³»¸®±â
										{
											if( scroll+1 < GM_temp_list_count ) scroll ++;
										}
										MainOn = true; break;
									  }
			case SWD_GUILD_EXPLAIN_SCROLL :{		// È­»ìÇ¥ ±ô¹ÚÀÌ±â
											int &scroll = SMenu[i].work;	// ½ºÅ©·Ñ º¯¼ö
											int page_max = SMenu[i].nField[0].nSHideNomalCount;		// ÆäÀÌÁö ¸Æ½º
											int up_down = SMenu[i].nField[j].nWillDo;

											if( up_down == LEFT )		// À§·Î ¿Ã¸®±â
											{
												if( scroll-1 >= 0 ) scroll -- ;
											}
											else if( up_down == RIGHT )	// ¾Æ·¡·Î ³»¸®±â
											{
												if( scroll+1 < page_max ) scroll ++;
											}
											MainOn = true; break;
										   }
			case SWD_GM_REGIST:{
								const int ret = ::CanGmRegist(GM_temp_list[SMenu[i].work]);
								switch( ret )
								{
									case  1: SendGMRegist(GM_temp_list[SMenu[i].work]); break; // GM¿¡ µî·ÏÇÒ¼ö ÀÖ´Ù... Ã¼Å© ¿ä¸Á
									case -1: AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(3,187) ); MP3(SN_WARNING );break;//010215 lsw
									case -2: AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(3,188) ); MP3(SN_WARNING );break;//010215 lsw
									case -3: AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(3,189) ); MP3(SN_WARNING );break;//010215 lsw
									case -4: AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(3,190) ); MP3(SN_WARNING );break;//010215 lsw
								}
								if( ret != 1 ) 
								{
									CheckAndSendGmRegist(GM_temp_list[SMenu[i].work]);
								}
								SmallMenuClose();
								return;
//								MainOn = true; break;
							   }
			case SWD_HELP : {
								ViewTipsFlag = 1;
								ViewTipToggle = 1;
								break;
							}
			case SWD_CHANGE_MONEY_OK : {
											SendChangeMoney( nBankTemp );		// ±âºÎÇÑ´Ù.
											SmallMenuClose();
											return;
									   }
			case SWD_SALVATION_OK : {
										if( g_Menu_Variable.bCheckSalvation )		// YGI 020527
										{
											if( nBankTemp < 1000 ) 
											{
												MP3( SN_WARNING );
	#ifdef CHANGE_MONEY_
												CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,191), false );//010215 lsw
	#else
												CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,192), false );//010215 lsw
	#endif
											}
											else CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,193) );//010215 lsw
										}
										MainOn = true;
										break;
									}

			case SWD_RUMOR_OK : {
									if( SCharacterData.nMoney < 500 ) 
									{
										MP3( SN_WARNING );
										wsprintf(SMenu[MN_DRINK_RUMOR].nField[0].temp,lan->OutputMessage(3,194));
										MainOn = true; break;
									}
									wsprintf(SMenu[MN_DRINK_RUMOR_VIEW].nField[0].temp,lan->OutputMessage(3,195));//010215 lsw
									CallServer( CMD_PAY_RUMOR );
									y_MenuFocus = MN_DRINK_RUMOR_VIEW;
									SMenu[MN_DRINK_RUMOR_VIEW].bActive = true;
									break;
								}
			case SWD_BATTLE_REGIST:
				{	//< CSD-030522
					if (g_pArenaManager->IsReadyGame())
					{
						break;
					}

					if( g_FightMapStart )		// ÀÌ¹Ì ½ÃÀÛ ÁßÀÌ¸é
					{
						AddCurrentStatusMessage( 250,0,0, kein_GetMenuString(24) );		// ÀÌ¹Ì °æ±â°¡ ½ÃÀÛµÇ¾ú´Ù.
						MainOn = true;
						break;
					}

					// ¼­¹ö¿¡¼­ Á¤º¸¸¦ ¹ÞÀºÈÄ¿¡ Ã¼Å©ÇÒ¼ö ÀÖ´Ù.
					if( Hero->fight_map_team_num ) 
						CallOkCancelMessageBox( 0,0,0,kein_GetMenuString( 25 ) );
					else
					{
						if( SMenu[i].key )
						{
							const int nTeam = SubMenu->nField[j].nWillDo;
							g_pArenaManager->SendEnterTeam(nTeam);
						}
						else
						{
							CallOkCancelMessageBox( 0,0,0,kein_GetMenuString( 1 ) );
						}
					}
					MainOn = true;
					break;
				}	//> CSD-030522
			case SWD_FIGHTMAP_WAIT_MEMBER:
				{	//< CSD-030522
					if (g_pArenaManager->IsReadyGame())
					{
						break;
					}

					if( Hero->fight_map_team_num )
					{
						g_pArenaManager->SendLeaveTeam();
						MainOn = true;
					}
												
					break;
				}	//> CSD-030522
			case SWD_FIGHT_WAIT_EXIT:
				{	//< CSD-030522
					if (g_pArenaManager->IsReadyGame())
					{
						break;
					}

					g_pArenaManager->SendLeaveLobby();
					g_BattleStatusView = 0;
					SmallMenuClose();
					g_pArenaManager->Reset();
					FighMapReset();
					break;
				}	//> CSD-030522
			case SWD_REQ_LOAN : {
									CallServer( CMD_REQ_LOAN_INFO );
									int menu = SMenu[i].nField[j].nWillDo;
									nBankTemp = 0;
									CallMenu( menu, i );
									if( !is_mainmenu_active ) y_MenuFocus = menu;

									g_RecvLoanInfo = 0;
									break;
								}

			case SWD_MN_MERCHANT_BBS_OK : 
				{
					int money = bbs_money[SMenu[i].key][SMenu[i].work];
					if( !SubMenu->key )
					{
						if( SCharacterData.nMoney < money )
						{
							// µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù.
							MP3( SN_WARNING );
							CallOkCancelMessageBox( 0, 0, 0, kein_GetMenuString( 21 ) );
							MainOn = true; break;
						}
						char *temp = SMenu[i].nField[j].temp;
						EWndMgr.GetTxt( HWND_2, temp, 100 );
						DWORD money = atol( temp );
						EWndMgr.GetTxt( HWND_1, temp, 30 );
						if( !temp[0] )		// ³»¿ëÀÌ ¾øÀ¸¸é
						{
							MP3( SN_WARNING );
							CallOkCancelMessageBox( 0, 0, 0, kein_GetMenuString( 29 ) );		// ³»¿ëÀÌ ¾ø½À´Ï´Ù.
							MainOn = true; break;
						}
						ConvString(temp);
						SendMerchantItemMessage( temp, money, SubMenu->work );
						EWndMgr.ClearAllTxt();//021001 lsw
						break;
					}
					else
					{
						if( SCharacterData.nMoney < money )
						{
							// µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù.
							MP3( SN_WARNING );
							CallOkCancelMessageBox( 0, 0, 0, kein_GetMenuString( 21 ) );
							MainOn = true; break;
						}
						int type;
						if( SubMenu->work ) type = CMD_MERCHANT_BBS_ALL;
						else type = CMD_MERCHANT_BBS;

						char *temp = SMenu[i].nField[j].temp;
						SetFocus2( HWND_3 );//021001 lsw
						EWndMgr.GetTxt( HWND_3, temp, 100 );
						temp[99] = 0;
						ConvString(temp);
						if( !temp[0] )
						{
							MP3( SN_WARNING );
							CallOkCancelMessageBox( 0, 0, 0, kein_GetMenuString( 29 ) );		// ³»¿ëÀÌ ¾ø½À´Ï´Ù.
							MainOn = true; break;
						}
						CallServer( type, temp, strlen(temp) );
						EWndMgr.ClearAllTxt();//021001 lsw
					}
				}break;
			case SWD_BUY_VISA : {
									;
									int nation = SMenu[i].key;
									int select = SMenu[i].work;
									if( SCharacterData.nMoney < NationVisa[nation].item_price[select] )
									{
										CallOkCancelMessageBox( 0, 0, 0, kein_GetMenuString( 21 ) );
									}
									else
										SendBuyVisa( nation, select );
									MainOn = true;
									break;
								}
			case SWD_GUILD_EXPLAIN_YES : 
				{
					switch( SMenu[i].nTemp )
					{
						case GE_REGIST_IMSI_GUILD :	// ¿¹ºñ ±æµå µî·Ï ¸Þ´º¸¦ ¶ç¿î´Ù.
							{
								int nStr = CanRegistGuild( 1 );
								if( nStr == 0 )		// ´ÙÀ½ ¸Þ´º·Î
								{
									SMenu[ MN_GUILD_REGIST_IMSI ].bActive = true;
									y_MenuFocus = MN_GUILD_REGIST_IMSI;
								}
								else if( nStr > 0 )
								{
									CallOkCancelMessageBox( 0, 0, 0, kein_GetMenuString( nStr ) ); 
								}
								else 
								{ 
									SmallMenuClose();
									return;
								}
								break;
							}
						case GE_REGIST_GUILD :	// Á¤½Ä ±æµå µî·Ï ¸Þ´º¸¦ ¶ç¿î´Ù.
							{
								int nStr = CanRegistGuild( 2 );		// Á¤½Ä ±æµå¸¦ µî·ÏÇÒ ¼ö ÀÖ´Â°¡?
								if( nStr == 0 )		// ´ÙÀ½ ¸Þ´º·Î
								{
									// CallGuildExplain( GM_INPUT_SUB_MASTER );
									// ±×¿Ü Á¶°Ç¿¡ ºÎÇÕÇÑÁö ¼­¹ö¿¡ ¿äÃ»ÇÑ´Ù. ·¹º§ 30 ÀÌ»óÀÇ ±æµå¿øÀÌ 10¸í ÀÌ»óÀÎÁö
									short int guild_code = Hero->GetGuildCode(); // CSD-030324
									SendDirectDB( CMD_REGIST_GUILD_CHECK, &guild_code, sizeof( short int ) );
								}
								else if( nStr > 0 )
								{
									CallOkCancelMessageBox( 0, 0, 0, kein_GetMenuString( nStr ) ); 
								}
								else 
								{ 
									SmallMenuClose();
									return;
								}
								break;
							}
						case GM_CHANGE_INFO_MENUAL :
						case GE_REGIST_GUILD_MARK :
							{
								int &page_status = SubMenu->work;

								WORD page;
								WORD page_max;
								GetWORDOfDWORD( (DWORD)page_status, page, page_max );
								if( page == 0 )
								{
									// ´ÙÀ½ÀåÀ» ÀÐ¾î º¸±â¸¦ ±ÇÇÑ´Ù.
									MainOn = true;
									Kein_PutMessage( KM_FAIL, kein_GetMenuString(68) );
									break;									
								}

								// ÀÌ¹ÌÁö¸¦ ÀÐ¾î ¿Í¼­ Âï¾î ÁØ´Ù.
								int ret = guild_mark_main.LoadBMP( "./mark/mark.bmp" );
								if( ret != 1 )
								{
									// ÀÐ¾î ¿À±â ½ÇÆÐ ¸Þ½ÃÁö ¹Ú½º Ãâ·Â
									MainOn = true;
									Kein_PutMessage( KM_FAIL, kein_GetMenuString(67) );
									break;									
								}
								else
								{
									if( ( guild_mark_main.m_Spr.xl < 20 ) ||
										( guild_mark_main.m_Spr.xl > 30 ) ||
										( guild_mark_main.m_Spr.yl < 25 ) ||
										( guild_mark_main.m_Spr.yl > 35 ) ) 
									{
										MainOn = true;
										Kein_PutMessage( KM_FAIL, kein_GetMenuString(66) );
										break;
									}
									
									if( !g_GuildMgr[Hero->GetGuildCode()].m_szGuldMasterName[0] )
									{	//< CSD-030324
										strcpy( g_GuildMgr[Hero->GetGuildCode()].m_szGuldMasterName, Hero->name );
									}	//> CSD-030324
									
									CallGuildInfoMenu( SubMenu->Id, Hero->GetGuildCode(), 0, SubMenu->nTemp ); // CSD-030324
								}
								break;
							}
						case GM_INPUT_SUB_MASTER :
							{
								char temp[20];
								SetFocus2( HWND_3 );//021001 lsw
								EWndMgr.GetTxt( HWND_3, temp, 20 );
								EatRearWhiteChar( temp );
								if( !temp[0] ) 
								{
									MainOn = true;
									break;
								}

								SendCheckSubGuildMaster( temp );
								SmallMenuClose();
								break;
							}
					}
					break;
				 }
			case SWD_REGIST_GUILD_IMSI_SEND	: {
												char guild_name[20];
												EWndMgr.GetTxt( HWND_MAIL1, guild_name, 20 );
												if( !guild_name[0] ) return;

												char explain[1000];
												EWndMgr.GetTxt( HWND_MAIL3, explain, 1000 );
												ChangeString( explain, '\r', ' ');
												
												SendRegistGuildImsi( guild_name, explain );

												EWndMgr.ClearAllTxt();//021001 lsw
												SmallMenuClose();
												break;
											  }
			case SWD_REGIST_GUILD_SEND : {
											if( SMenu[i].key == 0 ) 
											{
												MainOn = true;
												break;
											}
											// ÀÌ¹ÌÁö¸¦ Àü¼ÛÇÑ´Ù.
											switch( SMenu[i].key )
											{	//< CSD-030324
												default :				// ±æµå µî·Ï ¸Þ´º
													if( !g_Menu_Variable.m_pSubGuildMaster )
													{
														Kein_PutMessage( KM_FAIL, kein_GetMenuString( 65 ) );
														MainOn = true;
														break;
													}
													SendRegistGuild( Hero->GetGuildCode(), g_Menu_Variable.m_pSubGuildMaster );
													//acer5
													SendGuildMark( Hero->GetGuildCode(), &guild_mark_main.m_Spr );
													break;//020511 lsw

												case GM_CHANGE_INFO_MENUAL :	// ±æµå ÀÌ¹ÌÁö¸¸ ¾÷µ¥ÀÌÆ® ÇÑ´Ù.
													{
														// ÇÊ¿äÇÑ ¾ÆÀÌÅÛÀÌ ÀÖ´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù.
														SendCheckGuildChange( 1 );		// ±æµå ¸¶Å© º¯°æ
														//SendGuildMark( Hero->GetGuildCode(), &guild_mark_main.m_Spr );
														break;
													}
												
												case MN_GUILD_CHANGE_INFO :
													{
														SendCheckGuildChange( 2 );		// ±æµå Á÷±Þ º¯°æ
														//SendChangeGuildDegree( Hero->GetGuildCode(), g_Menu_Variable.m_pSubGuildMaster );
														break;
													}
											}	//> CSD-030324

											SmallMenuClose();
											return;
										 }
			case SWD_SEARCH_LADDER_RANK : {
											EWndMgr.GetTxt( HWND_3, g_szSearchLadderRankName, 17 );
											EWndMgr.ClearTxt(HWND_3);//021001 lsw
											EatRearWhiteChar( g_szSearchLadderRankName );
											if( !g_szSearchLadderRankName[0] )		// ³»¿ëÀÌ ¾øÀ¸¸é
											{
												MainOn = true; 
												break;
											}
											int page = g_lpRankLadderBBS->SearchName( g_szSearchLadderRankName );
											if( page )
											{
												SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalStart = page-1;
												SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalCount = page;
											}
											else
											{
												if( !g_bSendLadderRank )
												{
													g_bSendLadderRank = 1;
													SendDirectDB( CMD_REQ_SEARCH_RANK_LADDER, g_szSearchLadderRankName, strlen( g_szSearchLadderRankName ) );
												}
											}
											MainOn = true;
											break;
										  }
			case SWD_DUAL_OK : 
				{
					int select = SMenu[i].work;

					switch (SCharacterData.nCharacterData[SPELL])
					{
					case WIZARD_SPELL: // ¸¶¹ý»ç ½ºÆçÀ» °¡Áö°í ÀÖ´Â °æ¿ì
						{ 
							if (select == PRIEST)  select = -1;
							break;
						}
					case PRIEST_SPELL: // ¼ºÁ÷ÀÚ ½ºÆçÀ» °¡Áö°í ÀÖ´Â °æ¿ì
						{
							if (select == WIZARD)  select = -1;
							break;
						}
					}

					if( select == -1 )
					{
						MainOn = 1;
						break;
					}

					DoDualOKButton( SMenu[i].nTemp, select );
					SmallMenuClose();
					break;
				}
      //< CSD-011008
      case SWD_INIT_ABILITY_OK:
      {
			DoInitAbility();
			SmallMenuClose();
			break;
      }
      //> CSD-011008
			case SWD_GUILD_DEGREE_OK :
				{
					if( SMenu[i].nTemp == MN_GUILD_CHANGE_INFO )
					{
						CallGuildInfoMenu( SubMenu->Id, Hero->GetGuildCode(), 1, SubMenu->nTemp ); // CSD-030324
						break;
					}
				}
			case SWD_CALL_GUILD_EXPLAIN:
				{
					int type = SMenu[i].nField[j].nWillDo;
					CallGuildExplain( type );
					break;
				}
			case SWD_CALL_GUILD_CHANGE_DEGREE : 
				{
					//if( !g_Menu_Variable.m_pGuildDegree )
					//	g_Menu_Variable.m_pGuildDegree = new k_guild_degree;

					SMenu[MN_GUILD_REGIST_DEGREE].nTemp = i;
					//SMenu[MN_GUILD_REGIST_DEGREE].CheakType = 1;
					//SMenu[MN_GUILD_REGIST_DEGREE].bActive = true;
					//y_MenuFocus = MN_GUILD_REGIST_DEGREE;
					
					CallHtmlMessageBox( MN_GUILD_REGIST_DEGREE, "./data/guild_explain7.txt" );
					break;
				}
			case SWD_CALL_GUILD_CHANGE : 
				{
					CallGuildChangeInfo();
					break;
				}
			case SWD_CALL_SKIN_MENU :
				{
					int willdo = SMenu[i].nField[j].nWillDo;
					CallMenu( MN_CHARACTER_COLOR_CHANGE, i, true );
					SMenu[MN_CHARACTER_COLOR_CHANGE].key = willdo;
					SMenu[MN_CHARACTER_COLOR_CHANGE].work = 1;		// µ·À» ³»°Ô ÇÑ´Ù.
					break;
				}
			case SWD_CHARACTER_COLOR_CHANGE	:
				{
					if( SMenu[i].work )		// µ·À» ÁöºÒÇÑ´Ù.
					{
						if( SCharacterData.nMoney < NPC_Lev_Ref[Hero->lv].nDyeingPrice )
						{
							// µ·ÀÌ ºÎÁ·ÇÏ´Ù.
							Kein_PutMessage( KM_FAIL, kein_GetMenuString( 21 ) );
							SmallMenuClose();
							return;
						}
					}
					FieldTypeNomalPut(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].x+20, SMenu[i].nField[j].y+20, 5, SMenu[i].nField[j].nImageType);		
					if( !SMenu[i].key )
					{		// ¹Ùµð Ä®¶ó
						Hero->bodycolor = ReturnBlendRGB(SCharacterData.body_r,SCharacterData.body_g,SCharacterData.body_b);
						SendChangeColor( SMenu[i].work, 1, SCharacterData.body_r,SCharacterData.body_g,SCharacterData.body_b);
					}
					else 
					{		// ¿Ê Ä®¶ó
						Hero->clothcolor = ReturnBlendRGB(SCharacterData.cloth_r,SCharacterData.cloth_g,SCharacterData.cloth_b);
						SendChangeColor( SMenu[i].work, 0,SCharacterData.cloth_r,SCharacterData.cloth_g,SCharacterData.cloth_b);
					}
					SmallMenuClose();
					break;
				}			
			case SWD_MANTLE_COLOR_CHANGE	:
				{
					FieldTypeNomalPut(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].x+20, SMenu[i].nField[j].y+20, 5, SMenu[i].nField[j].nImageType);		
					Hero->mantlecolor = ReturnBlendRGB(SCharacterData.mantle_r,SCharacterData.mantle_g,SCharacterData.mantle_b);
					SendMantleDye(SCharacterData.mantle_r,SCharacterData.mantle_g,SCharacterData.mantle_b);
					SmallMenuClose();
					break;
				}	
			case SWD_ABILITY_REDISTRIBUTION :		// 011201 LTS		// LTS MENU
				{
					SendCMD_CHECK_ABILITY_CHANGE_MONEY();	// ¾îºô¸® ÀçºÐ¹è¸¦ ÇÏ·Á ÇÏ´Âµ¥ µ·ÀÌ ÀÖ³Ä?
					SmallMenuClose();
					break;
				}
			case SWD_LOCALWAR_JOIN :				// 011201 LTS
				{
				if (true == g_cPrison.IsBadUser())
				{
					t_packet tagPacket;
					tagPacket.u.kein.tagPrisonSysProcResult.nReason = (int)(CPrison::PSR_NOT_MOVE);
					g_cPrison.RecvPrisonSystemResult(&tagPacket);
					break;
				}
				
					SendCMD_LOCALWAR_JOIN();
					SmallMenuClose();
					break;
				}
			case SWD_LOCALWAR_MAPMOVE :				// LTS NEW LOCALWAR
				{
					SendCMD_LOCALWAR_MAPMOVE();
					CloseLocalWarInfo();
					SmallMenuClose();
					break;
				}

			case SWD_LOCALWAR_END :					// LTS NEW LOCALWAR
				{
					CloseLocalWarInfo();
					SmallMenuClose();
					break;
				}
			case SWD_LW_DONE_PREV :
				{
					LocalWarDoneBoxText(false);
					MainOn = true; break;
				}
			case SWD_LW_DONE_NEXT :
				{
					LocalWarDoneBoxText(true);
					MainOn = true; break;
				}
			case SWD_EVENT_LOCALWAR_JOIN :			// 020115 LTS
				{
					SendCMD_EVENT_LOCALWAR_JOIN();
					SmallMenuClose();
					break;
				}
			case SWD_EVENT_LOCALWAR_MOVE :			// 020115 LTS
				{
					SendCMD_EVENT_LOCALWAR_MOVE();
					SmallMenuClose();
					break;
				}
			case SWD_WAR_STATUS_REFRESH :
				{
					SendWarStatusRefresh();
					break;
				}
			case SWD_REGIST_FRIEND : 
				{
					char temp[30] = {0,};
					EWndMgr.GetTxt( HWND_3, temp, 20 );
					
					if( !temp[0] )
					{
						MainOn = true;
						break;
					}

					::RegistFriend(temp);
					
					EWndMgr.ClearAllTxt();//021001 lsw
					CallMenu( SMenu[i].nTemp, 0, true );
				}break;
			case SWD_DELETE_FRIEND : 
				{
					char temp[30] = {0,};
					EWndMgr.GetTxt( HWND_3, temp, 20 );
					int ret = g_Menu_Variable.m_FriendGrup->SerchName( temp );
					if( ret == -1 )		// µî·ÏµÇ¾î ÀÖÁö ¾Ê´Â Ä³¸¯ÀÌ´Ù.
					{
						// ¿¡·¯ ¸Þ½ÃÁö Ãâ·Â
						Kein_PutMessage( KM_FAIL, kein_GetMenuString( 103 ) );
					}
					else
					{
						// Àü¼Û
						k_delete_friend data;
						strcpy( data.name1, Hero->name );
						strcpy( data.name2, temp );
						SendDirectDB( CMD_DELETE_FRIEND, &data, sizeof( k_delete_friend ) );
						g_Menu_Variable.m_FriendGrup->DeleteName( ret );
					}
					EWndMgr.ClearAllTxt();//021001 lsw
					CallMenu( SMenu[i].nTemp, 0, (bool)true );
					break;
				}
			case SWD_POTAL_MENU : 
				{
					if( SMenu[i].nTemp == -1 )
					{
						MainOn = true;
						break;
					}
					int m = SMenu[i].nField[j].nWillDo;
					int n = SMenu[m].nFieldCount;
					SMenu[m].nField[n].nWillDo = SMenu[i].nTemp;
					CallMenu( m, i, true );
					break;
				}
			case SWD_GOTO_POTAL :
				{
					int n = SMenu[i].nFieldCount;
					int para = SMenu[i].nField[n].nWillDo;

					// ·¹º§ È®ÀÎ
					int level = g_Menu_Variable.m_PotalMap->GetMapLevel( para );
					if( Hero->lv < level )
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString( 210 ) );
						MainOn = true; break;
					}

					// °¡°Ý È®ÀÎ
					int money = g_Menu_Variable.m_PotalMap->GetMapMoney( para );
					if( SCharacterData.nMoney < money )
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString( 21 ) );
						MainOn = true; break;
					}
					char cn = para;
					CallServer(CMD_GOTO_POTAL, &cn, 1 );
					break;
				}
			case SWD_PUT_GUILD_ITEM	:
				{
					if( !g_Menu_Variable.m_pPutGuildItem )
					{
						MainOn = true;
						break;
					}
					ItemAttr *pItem = ::GetItemByPos( *g_Menu_Variable.m_pPutGuildItem );//021121 lsw
					if( !pItem || !pItem->item_no ) 
					{
						MainOn = true;
						break;
					}
					CItem* i = ItemUnit(pItem->item_no); // finito 070707 check if item is noechange an therefore can't drop
					if (!i) {MainOn = true; break;}

					if (i->GetItemKind() == IK_NOEXCHANGE) 
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString(246) );
						MainOn = true;
						break;
					}
					if( g_Menu_Variable.m_pPutGuildItem->type != INV ) // ÀÎº¥¸¸ °¡´É
					{
						MainOn = true;break;
					}
					if( g_Menu_Variable.m_pGuildItem )
					{
						// ±æµå ¼¼±ÝÀÌ Á¸ÀçÇÒ¶§, µ· ¾ÆÀÌÅÛÀÌ ¾Æ´Ï¸é º¸°ü ½ÃÅ°Áö ¸øÇÏ°Ô ÇÑ´Ù.
						if( g_Menu_Variable.m_pGuildItem->m_BoxMoney > 0 && pItem->item_no != GetMoneyItemNumber() )
						{
							Kein_PutMessage( KM_FAIL, kein_GetMenuString( 134 ) );
							MainOn = true; break;
						}
						else if( g_Menu_Variable.m_pGuildItem->GetMax() == 150 )// Ã¢°í°¡ ²Ë Ã£À» °æ¿ì º¸°ü ½ÃÅ°Áö ¸øÇÏ°Ô ÇÑ´Ù.
						{
							// Áï, ¼¼±Ýµµ ¾ø°í ÀÚ¸®°¡ ²Ë Ã£À» °æ¿ì¿£ ¾î¶²°Íµµ ÀúÀå½ÃÅ°Áö ¸øÇÑ´Ù.
							Kein_PutMessage( KM_FAIL, kein_GetMenuString( 135 ) );
							MainOn = true; break;
						}
					}

					Kein_PutMessage( KM_OK, kein_GetMenuString( 131 ), GetItemName( pItem->item_no ) );
					SendInsertItem( g_Menu_Variable.m_pPutGuildItem );
					
					short int guild_code = Hero->GetGuildCode(); // CSD-030324

					if( pItem->item_no == GetMoneyItemNumber() )
					{
						SendDirectDB( CMD_CHECK_GUILD_BOX_MONEY, &guild_code, sizeof( short int ) );	// º¸°ü·á¸¦ Á¤»êÇÑ´Ù.
					}
					SendDirectDB( CMD_GET_GUILD_ITEM_COUNT, &guild_code, sizeof( short int ) );		// ¾ÆÀÌÅÛ °¹¼ö °»½Å
					DeleteItem( pItem );
					MainOn = true;
					break;
				}
			case SWD_SMALL_MENU_CLOSE_CALL_FUCTION :
				{
					SmallMenuClose();
					void (*MenuFuction)() = SubMenu->nField[j].m_pFunction;
					if( MenuFuction )
						(*MenuFuction)();
					break;
				}
			case SWD_SET_SUB_MASTER : 
				{
					SmallMenuClose();
					if( CheckGuildMaster( Hero ) )
					{
						// ¼­ºê ¸¶½ºÅÍ¸¦ µî·ÏÇÏ´Â ¸Þ´º È£Ãâ
						g_Menu_Variable.m_nCheckSubMaster = CS_CHANGE_SUB_MASTER;
						CallGuildExplain( GM_INPUT_SUB_MASTER );
					}
					else
					{
						// ±æµå ¸¶½ºÅÍ¸¸ »ç¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù.
					}
					break;
				}
			case SWD_DEFAULT_MESSAGEBOX_OK :
				{
					if( !SubMenu->nTemp ) SmallMenuClose();
					else
					{
						y_MenuFocus = SubMenu->nTemp;
						SMenu[y_MenuFocus].bActive = true;
					}
					break;
				}
			case SWD_NATION_ITEM_OK :
				{
					if( g_Menu_Variable.m_nation_item.item_no )
					{
						int much = g_Menu_Variable.m_nation_item.give_much;
						if( g_Menu_Variable.m_nation_item.give_type )	// fame
						{
							if( Hero->fame-much < 1000 )
							{
								// ¸í¼ºÀÌ ºÎÁ·ÇÕ´Ï´Ù.
								Kein_PutMessage( KM_FAIL, kein_GetMenuString( 148 ) );
							}
							else
							{
								CallOkCancelMessageBox( i, 0, 0, kein_GetMenuString( 150 ) ); 
								//CallServer( CMD_BUY_NATION_ITEM );
							}
						}
						else // money
						{
							if( SCharacterData.nMoney < much )
							{
								// µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù.
								Kein_PutMessage( KM_FAIL, kein_GetMenuString( 21 ) );
							}
							else
							{
								CallOkCancelMessageBox( i, 0, 0, kein_GetMenuString( 150 ) ); 
								//CallServer( CMD_BUY_NATION_ITEM );
							}
						}

					}
					else
					{
						MainOn = true;
						break;
					}
					SmallMenuClose();
					break;
				}
			case SWD_GUILD_HOUSE_OK :
				{
					int para = SMenu[i].key2;
					if( para == -1 ) 
					{
						MainOn = true;
						break;
					}
					int line_max = SMenu[i].key;
					int page_status = SubMenu->work;
					WORD page;
					WORD page_max;
					GetWORDOfDWORD( (DWORD)page_status, page, page_max );
					int offset = page*line_max;

					y_MenuFocus = MN_GUILD_HOUSE_INFO;
					SMenu[MN_GUILD_HOUSE_INFO].bActive = true;
					SMenu[MN_GUILD_HOUSE_INFO].work = para+offset;
					break;
				}
			case SWD_GUILD_HOUSE_BUY :
				{
					char id = SMenu[i].work;

					CGuildHouseInfo	*pData = g_Menu_Variable.m_pGuildHouseInfo;
					int guild_code = pData->GetGuildCode( id );
					if( !guild_code )
					{
						// ½ÅÃ»
						if( pData->IsExistGuild( GetGuildCode( Hero ) ) )
						{
							// ÀÌ¹Ì °¡Áö°í ÀÖ´Â ¿¥Æ÷¸®¾Æ°¡ ÀÖ½À´Ï´Ù.
							Kein_PutMessage( KM_FAIL, kein_GetMenuString( 177 ) );
							MainOn = true; break;
						}
					}
					else if( guild_code != GetGuildCode( Hero ) )
					{
						// ´Ù¸¥ ±æµå ÀÔ´Ï´Ù.
						Kein_PutMessage( KM_FAIL, kein_GetMenuString( 174 ) );
						MainOn = true; break;
					}
					else if( !CheckGuildPower( GDP_BUY_GUILD_HOUSE , Hero ) )
					{
						// ±ÇÇÑÀÌ ¾ø´Ù.
						Kein_PutMessage( KM_FAIL, kein_GetMenuString( 176 ) );
						MainOn = true; break;
					}
					// ½Ã±×³Ý Ã¼Å©
					if( !pData->CheckSignet( id ) )
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString( 175 ) );
						MainOn = true; break;
					}

					// acer7
					if( pData->m_data[id].time > 30 )
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString( 179 ) );
						MainOn = true; break;
					}

					char house_id = pData->m_data[id].id;

					CallServer( CMD_BUY_GUILD_HOUSE, &house_id, sizeof( char ) );
					SmallMenuClose();
					break;
				}
			case SWD_WAR_MOVE :
				{
					SendWarMove();
				}
				break;
			case 0 : break;
			default :
				{
					MainOn = DoButtonCheckOfMenu_SWD( i,j );//soto-030430
					if(MainOn) break;//soto-030501
					MainOn = DoButtonCheckOfMenu_SpecialWillDo_lsw( i,j);//020410 lsw
				}
				break;
			}
			if( !MainOn ) 
			{
				SMenu[i].bActive=FALSE;
			}break;
		}break;//020530 lsw

		case FT_DO :  //¹«Á¶°Ç ½ÇÇàÇØ¾ß ÇÒ¶§, ÇÑ¸¶µð·Î ¶ß³»±âµé ºÙÀÌ´Â °÷, case¹® Áßº¹ ÇÇÇØ¾ßÇÔ.
			switch(SMenu[i].nField[j].nWillDo)
			{
				case DO_DIVIDE_SCROLL_UP :	{
												int &much = SMenu[i].nField[0].nShideNomalPlus;
												much++;
												SMenu[i].nField[0].nSHideNomalCount=0;
												if( (DWORD)much >= i_DivideItem.item_attr.attr[IATTR_MUCH] ) much = i_DivideItem.item_attr.attr[IATTR_MUCH];
												break;
											}
				case DO_DIVIDE_SCROLL_DOWN:	{
												int &much = SMenu[i].nField[0].nShideNomalPlus;
												SMenu[i].nField[0].nSHideNomalCount=0;
												much--;
												if( much < 1 ) much=0;
												break;
											}
				case DO_BANK_SCROLL_UP :{
											nBankTemp ++;
											if( nBankTemp >= 1000000000 ) nBankTemp = 1000000000;

											if((SMenu[i].Id == MN_BANK_DEPOSIT)  && ((DWORD)nBankTemp > SCharacterData.nMoney))
														nBankTemp = SCharacterData.nMoney;
											// 020925 YGI
											else if( (SMenu[i].Id == MN_BANK_DEPOSIT)  && ( nBankTemp + SCharacterData.BankMoney > 4000000000 ) )
											{
												if( SCharacterData.BankMoney > 4000000000 ) nBankTemp = 0;
												else
													nBankTemp = 4000000000 - SCharacterData.BankMoney;
											}
											else if((SMenu[i].Id == MN_BANK_DEFRAYAL) && ((DWORD)nBankTemp > SCharacterData.BankMoney))
														nBankTemp = SCharacterData.BankMoney;
											else if(SMenu[i].Id == MN_BANK_LOAN) 
											{
												if( SCharacterData.LoanTotal < SCharacterData.LastLoan )
													nBankTemp = 0;
												else if( (DWORD)nBankTemp > ( SCharacterData.LoanTotal - SCharacterData.LastLoan)  )
														nBankTemp = SCharacterData.LoanTotal-SCharacterData.LastLoan;
											}
											else if(SMenu[i].Id == MN_BANK_REPAYMENT) 
											{
												if((DWORD)nBankTemp > SCharacterData.nMoney)
														nBankTemp = SCharacterData.nMoney;
												if(nBankTemp > SCharacterData.LastLoan)
														nBankTemp = SCharacterData.LastLoan;
											}
											else if( SMenu[i].Id == MN_SALVATION || SMenu[i].Id == MN_SALVATION2 )
											{
												if((DWORD)nBankTemp > SCharacterData.nMoney) 
													nBankTemp = SCharacterData.nMoney;
												//if( (nBankTemp + TotalSalvation) > MAX_SALVAITON_MONEY )
												//	nBankTemp = MAX_SALVAITON_MONEY - TotalSalvation;
											}

											char temp[20];
											sprintf( temp, "%d", nBankTemp);
											EWndMgr.SetTxt(HWND_3,temp );//021001 lsw
										}break;
				case DO_BANK_SCROLL_DOWN :	nBankTemp --;
											if(nBankTemp < 1 ) nBankTemp = 0;

											char temp[20];
											sprintf( temp, "%d", nBankTemp);
											EWndMgr.SetTxt(HWND_3,temp );//021001 lsw
											break;

				case DO_BANK_TIME_SCROLL_UP :	nBankTemp ++;
												if(nBankTemp < 1 ) nBankTemp = 0;

				case DO_BANK_TIME_SCROLL_DOWN :	break;
				case DO_BANK_SCROLL_LEFT :	break;	
				case DO_BANK_SCROLL_RIGHT :	break;
										
/*				case DO_LEARNSKILL_ICON : {
											int a=SMenu[i].nField[ 6 ].nSHideNomalStart;
											int b=SMenu[i].nField[j].nSHideNomalStart;
											int c;
											c = g_pointMouseX - (SMenu[i].x+SMenu[i].nField[j].x);

											if( c>0 && c<35 ) c = 0;
											else if( c>45 && c<80 ) c = 1;
											else if( c>90 && c<125 ) c = 2;
											else if( c>140 && c<175 ) c = 3;
											else break; 

											if( !SkillInventory[a][b][c] )	break;
											SMenu[i].work = SkillInventory[a][b][c];
//											nSkillLearnResult = skill[SkillInventory[a][b][c]].CanLearnSkill( SMenu[i].nTemp - MN_BLACKSMITH );	// ¼­¹ö ÄÝµµ ÇÔ¼ö¾È¿¡¼­ °°ÀÌ ÇÑ´Ù.
											break;
										  }
*/
			}
			break;
		case FT_SKILL_PUT	:	
			{
				int first = SMenu[i].nField[j].nWillDo;
				int second= g_pointMouseY - (SMenu[i].y+SMenu[i].nField[j].y);
				int third = g_pointMouseX - (SMenu[i].x+SMenu[i].nField[j].x);

				if( (second < 36) && (second > 2) ) second = 0;
				else if( (second > 37) && (second < 72 ) ) second = 1;
				else break;

				if( third < 35 )  third = 0;
				else if( third > 42 && third < 76 ) third = 1;
				else if( third > 83 && third < 117 ) third = 2;
				else if( third > 124 && third < 158 ) third = 3;
				else break;
				const int s = SkillInventory[first][second][third];

				if( !s || !skill[s].num ) break;
				HotKeyMemory.SetQuickMemory(Hero->name,QM_SKILL,s);//020701 lsw
				MenuSoundClose( i );
				y_MenuFocus = 0;
				
				SkillSettingClear( );
			}break;
		case FT_KNOWLEDGE_ICON_PUT :
			{
				int first = g_pointMouseY - (SMenu[i].y+SMenu[i].nField[j].y);
				int secound = g_pointMouseX - (SMenu[i].x+SMenu[i].nField[j].x);

				if( first < 35 )  first = 0;
				else if( first > 46 && first < 81 )  first = 1;
				else if( first > 92 && first < 127 ) first = 2;
				else break;

				if( secound < 35 )  secound = 0;
				else if( secound > 46 && secound < 81 )  secound = 1;
				else if( secound > 92 && secound < 127 ) secound = 2;
				else break;

				const int s = skill[Skill_Know_Inv[first][secound]].num;
				if( !s ) break;
				HotKeyMemory.SetQuickMemory(Hero->name,QM_SKILL,s);//020701 lsw
				MenuSoundClose( i );
				y_MenuFocus = 0;
				
			}break;

		case FT_HIDE_ISCROLL_AUTO_PUT :	
		case FT_HIDE_ISCROLL_PUT :	(SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalStart)+=(SMenu[i].nField[SMenu[i].nField[j].nWillDo].nShideNomalPlus);
									if(SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalStart >= (SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalCount))
											SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalStart-=SMenu[i].nField[SMenu[i].nField[j].nWillDo].nShideNomalPlus;
									break;

		case FT_HIDE_DSCROLL_AUTO_PUT :
		case FT_HIDE_DSCROLL_PUT :	SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalStart-=SMenu[i].nField[SMenu[i].nField[j].nWillDo].nShideNomalPlus;
									if(SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalStart < 0)
											SMenu[i].nField[SMenu[i].nField[j].nWillDo].nSHideNomalStart=0;
									break;

		case FT_LEVELUP	://	if( LeftShiftOn && SCharacterData.LvUpPoint )
						//	{
						//		if(	SCharacterData.EachUp[SMenu[i].nField[j].nWillDo] && SCharacterData.LvUpPoint < 10 )
						//		{
						//			SendLvDownPointEach( SMenu[i].nField[j].nWillDo );
						//		}
						//	}
						//	else
							{
								if(	SCharacterData.EachUp[SMenu[i].nField[j].nWillDo] < 9999 )
								{
				                  if (SCharacterData.nCharacterAbility[SMenu[i].nField[j].nWillDo] >= g_mgrDual.GetAbility(SMenu[i].nField[j].nWillDo))		// ·¹º§¾÷ ÇÑ°èÄ¡
									{
										MP3( SN_WARNING );
										break;
									}
									SCharacterData.EachUp[SMenu[i].nField[j].nWillDo]++;
									SCharacterData.nCharacterAbility[SMenu[i].nField[j].nWillDo]++;
									SCharacterData.LvUpPoint--;
									MP3( SN_LV_EACH_UP );

									SendLvUpPointEach( SMenu[i].nField[j].nWillDo );
//									SendLevelUpPoint( SCharacterData.EachUp );		// Æ÷ÀÎÅÍ°¡ 0ÀÌ µÇ¾úÀ» ¶§¸¸ ¼­¹ö ÄÝÀ» ÇÑ´Ù.
//									AbilityOpenWhenLevelUp();
									
									if(!SCharacterData.LvUpPoint) 
									{
										memset( SCharacterData.EachUp, 0, sizeof( SCharacterData.EachUp ) );
									}
									AutoLevelUpCount = 0;		// ÀÚµ¿ºÐ¹è ¸øÇÏ°Ô ÇÑ´Ù.
								}
							}
							break; 

		case FT_LEVELUP_BUTTON :	if( !SCharacterData.LvUpPoint || IsLeftWindowOpen() ) break;
									CallMenu( MN_ABILITY );
//									CallServer( CMD_OPEN_ABILITY );	// ability ¸Þ´º°¡ ¿Â µÉ¶§ ¼­¹ö¿¡¼­ ¹Þ¾Æ¾ßÇÒ µ¥ÀÌÅ¸ ¿äÃ»
									/*	//020515 lsw
									SMenu[MN_STATUS].nField[6].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+154;
									SMenu[MN_STATUS].nField[7].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+163;
									SMenu[MN_ABILITY].nField[2].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+154;
									SMenu[MN_ABILITY].nField[3].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+163;
									*/
									break;

		case FT_SYSTEM_OPTION_SCROLL :	switch(j)
										{
											case 1: system_info.sound--;		if(system_info.sound < 0 )		system_info.sound=0; break;	//sound¿Þ
											case 2:	system_info.sound++;		if(system_info.sound >= 100 )		system_info.sound=100; break;	//sound¿À¸¥
											case 3:	system_info.music--;		if(system_info.music < 0 )		system_info.music=0; break;	//music¿Þ
											case 4:	system_info.music++;		if(system_info.music >= 100 )		system_info.music=100; break;	//music¿À¸¥
											case 5:	system_info.mouse_speed--;	if(system_info.mouse_speed < 0 )	system_info.mouse_speed=0; break;	//mouse¿Þ
											case 6:	system_info.mouse_speed++;	if(system_info.mouse_speed >= 100 )	system_info.mouse_speed=100; break;	//mouse¿À¸¥
											case 7:	system_info.scroll_speed--; if(system_info.scroll_speed < 0 )	system_info.scroll_speed=0; break;	//scroll¿Þ
											case 8:	system_info.scroll_speed++; if(system_info.scroll_speed >= 100 )	system_info.scroll_speed=100; break;	//scroll¿À¸¥
										}
										break;

		case FT_JOB_ITEM :	{
								int ItemX = ( g_pointMouseX - (SMenu[i].x + 25) ) / 35;
								int ItemY = ( g_pointMouseY - (SMenu[i].y + 35) ) / 35;
																									
								int xCheck = ( g_pointMouseX - (SMenu[i].x + 25) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
								int yCheck = ( g_pointMouseY - (SMenu[i].y + 35) ) % 35;
								if( (xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ) )  break;	
								int param = SMenu[i].nField[j].nSHideNomalStart*3*5+ItemY*5+ItemX;

								ItemAttr item_attr = aSellBuyItemAttr[param];
								if( item_attr.item_no )			// ¾ÆÀÌÅÛÀÌ ÀÖ´Â °÷À» Å¬¸¯ÇßÀ»¶§
								{
									switch( SMenu[i].nField[j].nWillDo )
									{
										case ITEM_BUY :	SMenu[i].work = item_attr.item_no;
														ReqItemValue( SMenu[i].work );		// »ì¶§ °¡°Ý °¡Á®¿À±â
														break;
										case ITEM_REPAIR :
										case ITEM_SELL: {
															ReqItemValue( item_attr.item_no );		// »ì¶§ °íÄ¥¶§ °¡°Ý °¡Á® ¿À±â
															SMenu[i].work = param;
															break;
														}
									}
								}
								break;
							}
		case FT_MALL_ITEM :	{
								int base_x = SubMenu->x+SubMenu->nField[j].x;
								int base_y = SubMenu->y+SubMenu->nField[j].y;

								int ItemX = ( g_pointMouseX - (base_x) ) / 35;
								int ItemY = ( g_pointMouseY - (base_y) ) / 35;
																									
								int xCheck = ( g_pointMouseX - (base_x) ) % 35;
								int yCheck = ( g_pointMouseY - (base_y) ) % 35;

								if ((xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ))  break;	
								int param = SMenu[i].nField[j].nSHideNomalStart*3*5+ItemY*5+ItemX;

								ItemAttr item_attr = itemMallAttr[param];
								if (item_attr.item_no)
								{
									SMenu[i].work = item_attr.item_no;
									ReqMallItemPrice(SMenu[i].work);
								}
								break;
							}
		case FT_SUPPER_INV :	{
									int gabx = SMenu[i].nField[j].x+4;
									int gaby = SMenu[i].nField[j].y+2;

									int ItemX = ( g_pointMouseX - (SMenu[i].x + gabx) ) / 35;
									int ItemY = ( g_pointMouseY - (SMenu[i].y + gaby) ) / 35;
																										
									int xCheck = ( g_pointMouseX - (SMenu[i].x + gabx) ) % 35;			//°æ°è¼±ÀÇ Å¬¸¯Àº ¹«½Ã
									int yCheck = ( g_pointMouseY - (SMenu[i].y + gaby) ) % 35;
									if( (xCheck < 2) || (xCheck > 25) || (yCheck < 2) || (yCheck > 25 ) )  break;

									ItemAttr &item = SupperItemAttr[ItemY][ItemX];
									if( !item.item_no ) break;
									
									if( SCharacterData.much_supper )
									{
										ItemAttr *empty_item = SearchEmpty();
										if( !empty_item )
										{
											MP3(SN_WARNING );
											CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(3,196)); break;//010215 lsw
										}

										SendIGetItemOfSupper( item.item_no);
										SCharacterData.much_supper--;
										DeleteItem( &item );
									}
									else SmallMenuClose();

									break;
								}

		case FT_MAGIC_CLASS	:	{
									SMenu[i].nTemp = j;
									int max;
									max = GetLearnableMagicMaxByClass( j );
									SMenu[i].nField[j].nSHideNomalCount = max;
									break;
								}

		case FT_DSCROLL_BOOK:	{
									if( SMenu[i].nField[j].nWillDo == 100 ) break;
									SMenu[i].nField[SMenu[i].nTemp].nSHideNomalStart-=2;
									if( SMenu[i].nField[SMenu[i].nTemp].nSHideNomalStart < 0 ) 
											SMenu[i].nField[SMenu[i].nTemp].nSHideNomalStart+=2;
									else	
									{
										SMenu[i].nField[j].nWillDo = 100;
										MP3( SN_BOOK_NEXT );
									}
									break;
								}
		case FT_ISCROLL_BOOK:	{
									if( SMenu[i].nField[j].nWillDo == 100 ) break;
									SMenu[i].nField[SMenu[i].nTemp].nSHideNomalStart+=2;
									if( SMenu[i].nField[SMenu[i].nTemp].nSHideNomalStart >= SMenu[i].nField[SMenu[i].nTemp].nSHideNomalCount ) 
											SMenu[i].nField[SMenu[i].nTemp].nSHideNomalStart-=2;
									else 
									{
										SMenu[i].nField[j].nWillDo = 100;
										MP3( SN_BOOK_NEXT );
									}
									break;
								}
		case FT_DSCROLL_MEM_BOOK:{
									if( SMenu[i].nField[j].nWillDo == 100 ) break;
									SMenu[i].nTemp --;
									if( SMenu[i].nTemp < 0 ) SMenu[i].nTemp = 0;
									else 
									{
										SMenu[i].nField[j].nWillDo = 100;
										SetMemorizeArray( SMenu[i].nTemp );
										SMenu[i].work = 0;
										MP3( SN_BOOK_NEXT );
									}
									break;
								}
		case FT_ISCROLL_MEM_BOOK:{
									if( SMenu[i].nField[j].nWillDo == 100 ) break;
									SMenu[i].nTemp ++ ;
									int max;
									if( SCharacterData.nCharacterData[SPELL] ) max = 10;
									else max = 12;
									
									if( SMenu[i].nTemp >= max ) SMenu[i].nTemp = max-1 ;
									else 
									{
										SMenu[i].nField[j].nWillDo = 100;
										SetMemorizeArray( SMenu[i].nTemp );
										SMenu[i].work = 0;
										MP3( SN_BOOK_NEXT );
									}
									break;
								}
		case FT_MEM_MAGIC_ICON :{
									int num = memorize[ SMenu[i].nField[j].nWillDo-1 ];
									if( num )
									{
										if( !SMenu[i].nField[j].nSpecialWillDo )		// ÀÌ¹Ì ¸Þ¸ð¶óÀÌÁî¸¦ ÇÏÁö ¾Ê¾ÒÀ» °æ¿ì¸¸...
										{
											SMenu[i].work = SMenu[i].nField[j].nWillDo;
//											int sound = ReturnCastingWord( Hero, num+SCharacterData.nCharacterData[SPELL]*150 );
											int sound = ReturnStartingWord( Hero, num+SCharacterData.nCharacterData[SPELL]*150 );
//											int sound = ReturnRoonWord( Hero, num+SCharacterData.nCharacterData[SPELL]*150 );
											if( sound ) MP3( sound );
										}
									}
									break;
								}
		case FT_LEARN_MAGIC	: {
								int cls = SMenu[i].nTemp;
								int num = SMenu[i].nField[cls].nSHideNomalStart;
								num += SMenu[i].nField[j].nWillDo;	// left, right ±¸ºÐ...
								int magic_num = magic_by_class.magic[cls][num];
								
								if (magic_num && !SCharacterData.MagicId[magic_num])
								{	//< CSD-030723
								  	if (CanLearnMagic(magic_num))
									{
										SendLearnMagic(magic_num);
										MP3(SN_LEARN_ITEM);
									}
									else
									{
//										MP3( SN_WARNING );
									}
								}	//> CSD-030723

								break;
							  }
		case FT_HEAL_TEXT_BUTTON :	{
										int willdo = SMenu[i].nField[j].nWillDo;
										int &scroll = SMenu[i].nField[willdo].nSHideNomalStart;
										int cn = SMenu[i].nField[willdo].nSHideNomalCount;		// ÃÖ´ë
										int spwilldo = SMenu[i].nField[j].nSpecialWillDo;		// ¸î¹øÂ° ±Û¾¾ Ãâ·ÂÀÎ°¡¸¦ À§ÇÑ... 1: 1¹øÂ°

										if( scroll ) 
										{
											if( spwilldo > 3 || spwilldo > cn ) break;
										}
										else
										{
											if( spwilldo > cn ) break;
										}
										SMenu[i].work = spwilldo;
										break;
									}
		case FT_TEXT_BUTTON	: {
								int spwilldo = SMenu[i].nField[j].nSpecialWillDo;		// ¸î¹øÂ° ±Û¾¾ Ãâ·ÂÀÎ°¡¸¦ À§ÇÑ... 1: 1¹øÂ°
								SMenu[i].work = spwilldo;
								break;
							  }
		case FT_INN	:	{
							SMenu[i].work = SMenu[i].nField[j].nWillDo;
							break;
						}
		case FT_PARTY_FACE_PUT :{
									int num = SMenu[i].nField[j].nWillDo;	// ÆÄÆ¼ ¹è¿­ ¹øÈ£
									CharacterParty	*character = &SCharacterData.party[num];
									int &ct = SMenu[i].nField[j].nSHideNomalCount;
									int &join_prev = SMenu[i].nField[j].nSHideNomalStart;
									int &delay = SMenu[i].nField[j].nShideNomalPlus;
									
									/*
									//acer
									if( character->m_Sight && character->ch )
									{
										if( g_MouseItemType == 1 )	// ¾ÆÀÌÅÛÀ» µé°í ÀÖ¾úÀ» °æ¿ì...
										{
											POS pos_s, pos_t;
											SetItemPos(HAND,  &pos_s);
											SetItemPos(IM_CHARACTER, character->ch->id, &pos_t);
											SendMoveItem( HandItemAttr.item_no, pos_s, pos_t );
											AddCurrentStatusMessage( 200, 200, 0, lan->OutputMessage(3,197),GetItemName(HandItemAttr.item_no), character->m_Name  );//010215 lsw
											g_MouseItemType = 0;
											g_MouseItemNumber = 0;
											DeleteItem( &HandItemAttr );
										}
									}*/
									if(character->ch)
										SendReqHpOfParty( character->ch->name );
									else
										SendReqHpOfParty( character->m_Name );
									break;
								}
		case FT_MENU_MOVE_DIRECT : {
										if( !SMenu[i].nTemp ) break;
										if( IsRightWindowOpen() && g_pointMouseX > 321+GABX_SCREEN*2) break;
										if( IsLeftWindowOpen() && g_pointMouseX < 321+GABX_SCREEN*2  ) break;
										//SMenu[i].x = SMenu[i].nField[j].nShideNomalPlus / 1000;
										//SMenu[i].y = SMenu[i].nField[j].nShideNomalPlus % 1000;
										SMenu[i].nTemp = false;
										SMenu[i].key = 0;
										SMenu[i].nField[j].nSHideNomalCount = 0;
										SMenu[i].nField[j].nSHideNomalStart = 0;
										SMenu[i].nField[j].nShideNomalPlus = 0;
										break;
								   }
		case FT_HUNTER_LIST_SCROLL : {
										int willdo = SMenu[i].nField[j].nWillDo;
										if( willdo )
										{
											if( curr_hunter_list->is_end ) break;

											if( curr_hunter_list->next )
											{
												CHunterList_Next( curr_hunter_list );
											}
											else
											{
												int id = curr_hunter_list->list.index;
												if( id && !curr_hunter_list->next )
													SendGetHunterList( id );
											}
										}
										else CHunterList_Prev( curr_hunter_list );
										break;
									 }
		case FT_NATION_BUTTON : {
									//int block = 1;
									int willdo = SubMenu->nField[j].nWillDo;
									if( (SubMenu->work == 0 && willdo == 1 ) ||
										(SubMenu->work+1 == SubMenu->key && willdo == 2 ) ||
										(SubMenu->work+1 != SubMenu->key && willdo == 0 ) ) break;

									switch( willdo )
									{
										case 1:			// previous button
											SubMenu->work--;
											if( SubMenu->work < 0 ) SubMenu->work = 0;
											break;
										case 2:			// next button
											SubMenu->work++;
											if( SubMenu->work > SubMenu->key ) SubMenu->work = SubMenu->key;
											break;

										case 0:			// ok button
											SendChangeScriptValue( SCRIPT_IS_READ_NATION_BBS, 100 );
										case 3:			// exit button
											SubMenu->bActive = false;
											SmallMenuClose();
											return;
									}
									break;
								}
		/*case FT_TEMBATLE_ADD : {
								int willdo = SMenu[i].nField[j].nWillDo;
								if( !g_BattleZone.IsExist( SCharacterData.sCharacterName ))
								{
									SendAddMeOfTeamBattle( willdo );
								}
								else
								{
									MP3( SN_WARNING );
									// ÀÌ¹Ì µî·ÏÇß´Ù.
								}
								break;
							   }*/

		case FT_GM_ITEM_EXPLAIN_BUTTON:{
										int nWillDo = SMenu[i].nField[j].nWillDo;
										CallMenu( nWillDo , i, (bool)true);
										SMenu[i].bActive = false;
										int gm_list_count = SMenu[i].work;
										int count = SMenu[i].nField[j].nSpecialWillDo;

										int item;
										switch( count )
										{//021030 lsw
										case 1:
										case 2:
										case 3: 
											{
												item = g_GmMain[GM_temp_list[gm_list_count]].need_item[count]; break;
											}break;
										}

										int para = g_GM_Item.SearchById( item );
										if( para == -1 ) break;			// ´­·¯ Áö¸é ¾ÈµÇ´Â°Çµ¥...
										SMenu[nWillDo ].key = para;		// ¾î¶² ¾ÆÀÌÅÛÀ» ¼³¸íÇØÁÖ´Â°¡...
										return;
									   }break;
		case FT_FIGHTMAP_START:
			{	//< CSD-TW-030618
				if (!g_pArenaManager->IsReadyGame())
				{
					g_pArenaManager->SendReadyGame(Hero->id);					
				}

				break;
			}	//> CSD-TW-030618
		case FT_SELECT_VISA :{
								SMenu[i].work = SMenu[i].nField[j].nWillDo;
								break;
							 }

		case FT_SMALL_TIPS_BUTTON : {
										int nWillDo = SMenu[i].nField[j].nWillDo;
										switch( nWillDo )
										{
											case 0 : bSmallTips = false; return;
											case 1 :	SMenu[i].key--;
														if( SMenu[i].key < 0 ) SMenu[i].key = 0;
														else
															if( ViewSmallTipsLoad( SMenu[i].key ) == -1 ) 
															{
																MP3( SN_WARNING );
															}
														break;
											case 2 : SMenu[i].key++;
													 if( ViewSmallTipsLoad( SMenu[i].key ) == -1 ) 
													 {
														 SMenu[i].key--;
														 MP3( SN_WARNING );
													 }
													 break;
										}
										break;
									}
		case FT_VIEW_EFFECT_TOOL : 
			{
				const int nWillDo = SMenu[i].nField[j].nWillDo;
				switch( nWillDo )
				{
				case 0 : 
					{
						SetFocus2( HWND_3 );//021001 lsw
						EWndMgr.ClearTxt(HWND_3);//021001 lsw
					}break;
				case 1 : 
					{
						InsertMagic( Hero, Hero, SubMenu->work, 0,0,0,0, 0 ); 
						SubMenu->key = SubMenu->work;
						sprintf( SubMenu->nField[0].temp, "%d", SubMenu->work );
						EWndMgr.SetTxt(HWND_3,SubMenu->nField[0].temp );//021001 lsw
					}break;
				}
			}break;
		case FT_GUILD_DEGREE : 
			{
				int willdo = SMenu[i].nField[j].nWillDo;
				SubMenu->work = willdo;
				SetFocus2( HWND_3 );//021001 lsw
				EWndMgr.ClearTxt(HWND_3);//021001 lsw
		   }break;
		case FT_INPUT_GUILD_MASTER :
			{
				SetFocus2( HWND_3 );//021001 lsw
			}break;
		case FT_GUILD_MEMBER_LIST_DEGREE_TAB :
			{
				int x = g_pointMouseX - (SMenu[i].x+SMenu[i].nField[j].rCheakBox.left);
				int pos = (float)x / ((SMenu[i].nField[j].rCheakBox.right - SMenu[i].nField[j].rCheakBox.left) / 5.0);
				if( pos > 5 ) break;
				if( pos < 0 ) break;
				static int pos_to_degree[] = {1, 2, 3, 4, 0 };
				if( SMenu[i].key != pos )
				{
					SMenu[i].key= pos;

					int &page_status = SMenu[i].work;
					WORD page;
					WORD page_max;
					GetWORDOfDWORD( (DWORD)page_status, page, page_max );
					page = SMenu[i].nField[j].temp[pos_to_degree[pos]*2];
					page_max= SMenu[i].nField[j].temp[pos_to_degree[pos]*2+1];
					page_status = (int )MAKELONG( page, page_max );
					
					if( !g_Menu_Variable.m_GuildMemberName.GetRecv( pos_to_degree[pos] ) )
					{
						// ¼­¹ö ¿äÃ»
						k_get_guild_member_list_req data;
						data.guild_code = Hero->GetGuildCode(); // CSD-030324
						data.degree = pos_to_degree[pos];
						SendDirectDB( CMD_GET_NAME_GUILD_MEMBER_LIST, &data, sizeof( k_get_guild_member_list_req ) );
						g_Menu_Variable.m_GuildMemberName.SetRecv( pos_to_degree[pos] );
					}
				}
				break;
			}
			case FT_GUILD_PUBLIC_NOTICE : 
				{
					int tab = SubMenu->key;
					if( tab ) break;	// ¸ÇÃ³À½ È­¸é¿¡¼­¸¸ Ã³¸® µÈ´Ù.
					if( !CheckGuildMaster( Hero ) )	break;	// ±æ¸¶°¡ ¾Æ´Ï¸é »ç¿ëÇÒ ¼ö ¾ø´Ù.
					int nWillDo = SubMenu->nField[j].nWillDo;

					switch( nWillDo )
					{
						case 0 :	// ¾²±â  
							{
								SetFocus2(HWND_MAIL3);//021001 lsw
								EWndMgr.ClearTxt(HWND_MAIL3);//021001 lsw
							}
							break;
						case 1:		// µî·Ï
							{
								k_guild_notice data;
								data.guild_code = Hero->GetGuildCode();
								EWndMgr.GetTxt( HWND_MAIL3, data.msg, 1000 );
								ChangeString( data.msg, '\r', ' ');
								SendDirectDB( CMD_SEND_GUILD_NOTICE, &data, 2+strlen( data.msg )+1 );
								t_packet p;
								strcpy( p.u.kein.default_msg, data.msg );
								RecvGetGuildNotice( &p );
								EWndMgr.ClearAllTxt();//021001 lsw
							}break;
						case 2:		// Ãë¼Ò
							{
								EWndMgr.ClearAllTxt();//021001 lsw
								break;
							}
					}
					break;
				}
		case FT_POTAL_SELECT :
			{
				int y = SMenu[i].nField[j].y;
				int a = ( g_pointMouseY-SMenu[i].y-y )/20;

				int line = SMenu[i].key;
				int &page_status = SMenu[i].work;
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)page_status, page, page_max );
				int max = g_Menu_Variable.m_PotalMap->GetMax();
				
				if( a<line && a+line*page<max ) SMenu[i].nTemp = a+line*page;
				break;
			}
		case FT_GUILD_ITEM_PUT :
			{
				if(g_MouseItemType != 1 ) break;
				if( !g_Menu_Variable.m_pPutGuildItem ) break;
				if( IP_base.type != INV ) break;

				*g_Menu_Variable.m_pPutGuildItem = IP_base;
				break;
			}
		case FT_GUILD_HOUSE :
			{
				CGuildHouseInfo *pData = g_Menu_Variable.m_pGuildHouseInfo;
				if( !pData ) break;

				int line_max = SMenu[i].key;
				int page_status = SubMenu->work;
				WORD page;
				WORD page_max;
				GetWORDOfDWORD( (DWORD)page_status, page, page_max );
				int offset = page*line_max;

				int y = SubMenu->y+SubMenu->nField[j].y;		// ½ÃÀÛ À§Ä¡
				int offset_y = g_pointMouseY - y;
				if( offset_y < 0 || offset_y >= 18 * line_max ) break;
				int para = offset_y/18;
				if( para+offset >= pData->m_max ) break;
				SubMenu->key2 = para;
				break;
			}
		case FT_NO_CHECK :	break;

//<soto-030511
		case FT_WRITE_MERCHANT:
			{
				SMenu[i].work = SMenu[i].nField[j].nWillDo;
			}
			break;
		case FT_CHECK_MERCHANT_FIND:
			{
				if(SMenu[i].nField[j].nSpecialWillDo == 0)
				{
					Auction.m_bChkKeyWord = !Auction.m_bChkKeyWord;
					SMenu[i].nField[j].nWillDo = Auction.m_bChkKeyWord;
				}
				else
				{
					Auction.m_bChkMerchantName = !Auction.m_bChkMerchantName;
					SMenu[i].nField[j].nWillDo = Auction.m_bChkMerchantName;
				}
			}
			break;
//>soto-030511

		default:
		{
			if(!DoButtonCheckOfMenu_kh( i, j ))
			{
				DoButtonCheckOfMenu_lsw( i, j );
			}
		}
	}
//	SMenu[i].nField[j].fCheakFlag = false;
	if( SoundOn ) MP3( SN_CLICK );
}


void DoLButtonDoubleClickOfMenu( int i, int j )
{
	int menu = i;
//	switch( SMenu[i].nField[j].nType )
//	{
//	default:
//		{
//		}break;
//	}
}

//----------------------------------------------------------------------------------------------------
// ½º¸ô ¸Þ´º°¡ È£Ãâ µÆÀ» ¶§... Ã¼Å©ÇÏ´Â ºÎºÐÀ» ÀÌ°÷À¸·Î ¹Ù²Û´Ù.
//----------------------------------------------------------------------------------------------------
void SmallMenuChecking()
{
	int i,j,x;
	bool f_ReturnOn = false;
	Small_KeyProc(); 


	for(x=1;x<MAX_SMALL;x++)		// ¸ÞÀÎ ¸Þ´º´Â Ã¼Å©ÇÏÁö ¾Ê´Â´Ù.
	{
		i=smallmenu[x];
		if( y_MenuFocus && y_MenuFocus != i ) 
		{	
			for(j=0;j<SMenu[i].nFieldCount;j++)
			{
				SMenu[i].nField[j].fRectMouse=false;
				if( !SMenu[i].bActive ) SMenu[i].CheakType = 0;
			}
			continue;
		}
		if(SMenu[i].bActive)
		{
			for(j=0;j<SMenu[i].nFieldCount;j++)
			{
				if(MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
				{
					SMenu[i].nField[j].fRectMouse=TRUE;
					g_MouseInMenu = i;			// mouse°¡ menu¾È¿¡ µé¾î¿Ô´Ù.
				}
				else
				{
					SMenu[i].nField[j].fRectMouse=FALSE; // ±¸¼º ¿ä¼Ò ¾È¿¡ ÀÖÀ¸¸é TRUE, ¾Æ´Ï¸é FALSE
					SMenu[i].nField[j].fLButtonDown=false;
					SMenu[i].nField[j].fCheakFlag=false;
				}
			}
		}
		else
		{
			SMenu[i].CheakType = 0;		// ÃÊ±âÈ­¸¦ À§ÇØ	±× ¸Þ´º°¡ Ã³À½À¸·Î ¾×Æ¼ºê µÉ¶§ 1·Î ¼ÂÆÃÇÏ¸é¼­ ÃÊ±âÈ­ ½ÃÄÑÁØ´Ù.
			for(j=0;j<SMenu[i].nFieldCount;j++)
			{
				SMenu[i].nField[j].fRectMouse=false;
			}
		}
	}

	if( fLMoveMenu || fRMoveMenu ) return;	
	
	if(g_nRButtonState==STATE_BUTTON_PRESSED)
	{
		RButtonDown=true;
		if( g_MouseItemType==1 && (GetItemAttr(HandItemAttr.item_no, WEAR_ABLE) == WEAR_BELT) )
		{
			POS pos_s;
			SetItemPos(HAND, &pos_s);
			UseItemByRbutton( pos_s, HandItemAttr );
			g_MouseItemType = 0;
			g_MouseItemNumber = 0;
		}
		else
		{
			for(x=1;x<MAX_SMALL;x++)
			{
				i=smallmenu[x];
				if( SMenu[i].bActive )
				{
					for(j=0; j<SMenu[i].nFieldCount; j++)
					{
						if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) )
						{
							SMenu[i].nField[j].fRButtonDown = true;
							DoRButtonDownOfMenu( i, j );
						}
					}
				}
			}
		}
	}
	else if( g_nRButtonState==STATE_BUTTON_STILL && RButtonDown ) 
	{
		for(x=1;x<MAX_SMALL;x++)
		{
			i=smallmenu[x];
			for(j=0; j<SMenu[i].nFieldCount; j++)
			{
				if( !MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) )
				{
					SMenu[i].nField[j].fRButtonDown = false;
					RButtonDown = false;
				}
			}
		}
	}
	else if( g_nRButtonState == STATE_BUTTON_RELEASED )
	{
		RButtonDown=false;
		for(x=1;x<MAX_SMALL;x++)
		{
			i=smallmenu[x];
			for(j=0; j<SMenu[i].nFieldCount; j++)
			{
				if( SMenu[i].nField[j].fRButtonDown )
				{
					SMenu[i].nField[j].fRButtonCheck = true;
					DoRButtonCheckOfMenu( i, j );
				}
				SMenu[i].nField[j].fRButtonDown = false;
			}
		}
	}

	if( g_nLDButtonState == STATE_BUTTON_DOUBLECLICK )	if( g_nLDButtonState == STATE_BUTTON_DOUBLECLICK )
	{
		for(x=1;x<MAX_SMALL;x++)
		{
			i=smallmenu[x];
			if( y_MenuFocus && y_MenuFocus != i )  continue;
			if(SMenu[i].bActive) 
				for(j=0; j<SMenu[i].nFieldCount;j++) 
				{
					if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) && 
						MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox, FALSE))  //¸¶¿ì½º°¡ ¸Þ´º±¸¼º ¿ä¼ÒÀÇ rect¾È¿¡¼­ L¹öÆ°ÀÌ ´­·È³Ä //FALSE ÀÏ °æ¿ì´Â µå·¡±× ½ÃÀÛÁ¡À¸·Î ÆÇ´Ü
						DoLButtonDoubleClickOfMenu( i, j );					//  Å¸ÀÔ¿¡ µû¸¥ ½ÇÇà
				}
		}
	}

	if(g_nLButtonState==STATE_BUTTON_PRESSED)
	{
		LButtonDown=TRUE;
		StateMousePoint = g_pointMouse;
	}
	if(g_nLButtonState==STATE_BUTTON_STILL && LButtonDown)
	{
		LButtonCount++;
		MouseDrag = TRUE;
		for(x=1;x<MAX_SMALL;x++)
		{
			i=smallmenu[x];
			if( y_MenuFocus && y_MenuFocus != i )  continue;
			if(SMenu[i].bActive)   
			{
				for(j=0; j<SMenu[i].nFieldCount;j++) 
				{
					if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) && 
						MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox, FALSE))  //¸¶¿ì½º°¡ ¸Þ´º±¸¼º ¿ä¼ÒÀÇ rect¾È¿¡¼­ L¹öÆ°ÀÌ ´­·È³Ä //FALSE ÀÏ °æ¿ì´Â µå·¡±× ½ÃÀÛÁ¡À¸·Î ÆÇ´Ü
					{
						SMenu[i].nField[j].fLButtonDown=TRUE;
						DoLButtonDownOfMenu( i, j );					//  Å¸ÀÔ¿¡ µû¸¥ ½ÇÇà
						SMenu[i].nField[j].DragCount ++;
					}
					else 
					{
						SMenu[i].nField[j].DragCount = 0;
					}
				}
			}
		}
	}
	else 
	{
		LButtonCount = 0;
		if(g_nLButtonState==STATE_BUTTON_RELEASED)
		{
			YouGotItemFlag = 0;	
			HandItemOn = false;
			int kkk = ItemDrop();
			LButtonDown=FALSE;

			for(x=1;x<MAX_SMALL;x++)
			{
				i=smallmenu[x];
				if( y_MenuFocus && y_MenuFocus != i )  continue; 
				if( !SMenu[i].bActive ) continue;
				for(j=0; j<SMenu[i].nFieldCount;j++)
				{
					SMenu[i].nField[j].DragCount = 0;
					if( !MouseDrag || SMenu[i].nField[j].fLButtonDown )
					{
						SMenu[i].nField[j].fLButtonDown=false;
						if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
						{
							SMenu[i].nField[j].fCheakFlag=TRUE;
							DoButtonCheckOfMenu( i, j );
							f_ReturnOn = true;
						}						
					}
					SMenu[i].nField[j].fLButtonDown = false;
				}
				if( f_ReturnOn ) return;
			}
			MouseDrag = FALSE;
		}
	}	
	PutMouseItemCheck(); 
}



//----------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------




void MenuButtonRightExplain( int x, int y, int &count, char * title )
{
	return;
	int len = strlen(title);
	static int ct = len/2;
	if( ct != len/2 ) ct = count;
	if( ct == 100 ) ct = len/2;
	char *str = title;
	int max = len - ct*2;
	char *str2 = new char[max+1];
	str += ct;
	strncpy(str2, str, max);
	str2[max] = NULL;

	int g_gab = len/2 * (1+spr[526].xl);
	int mi = strlen(str2)/2 * (1+spr[526].xl);
	int gabX;

	PutCompressedImage( x+38+spr[525].ox+g_gab -mi, y+24 , &spr[525] );
	int a;
	for( a=0; a< max; a++)
	{
		gabX = 38+spr[525].xl+ spr[526].ox+g_gab+a*(1+spr[526].xl)-mi;
		PutCompressedImage( x+gabX, y+22 , &spr[526] );
	}
	gabX = spr[525].xl+ spr[526].ox+g_gab-mi;
	gabX+=a*(1+spr[526].xl);
	PutCompressedImage( x+38+gabX, y+24, &spr[527] );
	
	SetHangulAlign( TA_CENTER );
	Hprint2( x+38+spr[525].xl+g_gab , y+16, g_DestBackBuf, "%s", str2);

	ct -=4;
	if( ct < 0 ) ct = 0;
	count = ct;
	SAFE_DELETE_ARRAY(str2);
}		

void MenuButtonLeftExplain( int x, int y, int &count, char * title )
{
	return;
	int len = strlen(title);
	int total_size = (15+spr[525].xl*2 + len * spr[526].xl );
	static int ct = len/2;
	if( ct != len/2 ) ct = count;
	if( ct == 100 ) ct = len/2;
	char *str = title;
	int max = len - ct*2;
	char *str2 = new char[max+1];
	str += ct;
	strncpy(str2, str, max);
	str2[max] = NULL;

	int g_gab = len/2 * (1+spr[526].xl);
	int mi = strlen(str2)/2 * (1+spr[526].xl);
	int gabX;

	PutCompressedImage( x-total_size+spr[525].ox+g_gab -mi, y+24 , &spr[525] );
	int a;
	for( a=0; a< max; a++)
	{
		gabX = -total_size+spr[525].xl+ spr[526].ox+g_gab+a*(1+spr[526].xl)-mi;
		PutCompressedImage( x+gabX, y+22 , &spr[526] );
	}
	gabX = spr[525].xl+ spr[526].ox+g_gab-mi;
	gabX+=a*(1+spr[526].xl);
	PutCompressedImage( x-total_size+gabX, y+24, &spr[527] );
	
	SetHangulAlign( TA_CENTER );
	Hprint2( x-total_size+spr[525].xl+spr[525].ox+g_gab , y+16, g_DestBackBuf, "%s", str2);

	ct -=4;
	if( ct < 0 ) ct = 0;
	count = ct;
	SAFE_DELETE_ARRAY(str2);
}		
    
    
    
