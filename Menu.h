#if !defined(AFX_MENU_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
#define AFX_MENU_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "network.h"
#include "MenuNetwork.h"
#include "word.h"
#include "EditWndMgr.h"//021001 lsw



//////////////////////////////////////
// 눈에 보이는 갑옷 accessory define
#define AT_ARMOR	0
#define AT_HELMET	1
#define AT_WEAPON	2
#define AT_SHIELD	3


// SetPartyMenu()에서 사용할 정의
#define RELATION	100
#define PARTY		200
#define EMPLOYMENT	300
#define NATIONPARTY	500
#define MAN_LIST_MAX_	100		// 상점 주인들 ( 이벤트 번호 최대 )



/////////////////////////////////////////////////////////////////////////////////
#define MAX_LEARN_SKILL_PAGE	50	// 아이템 만들기 페이지 맥스

//ACER7
#define PK_MODE	2
#define BATTLE_MODE 1
#define PEACE_MODE  0


//////////////////////////////////////////////////
#define CHOBO_LEVEL		7


////////////////////////////////////////
// sound

#define SN_CONGRATULATIONS		62
#define SN_TOUCH_START			1214
#define SN_CLICK_START			1212
#define SN_SELECT_START			1211
#define SN_DICE					1210//	주사위 굴릴때 


//-- 추가 : 1206 (모든 아이템 습득시) 1207 (모든 아이템을 버릴 경우)
#define SN_GET_ITEM		1206
#define SN_DROP_ITEM	1207

/////////////////////////////
// 콜로세스에서 쓰이는 사운드
#define SN_FIGHT_WIN			251
#define SN_FIGHT_LOSE			252
#define SN_FIGHT_TIME			253
#define SN_FIGHT_STOP			254
#define SN_FIGHT_START_READY	255
#define SN_FIGHT_START_FIGHT	256


// ( 장착 )
#define SN_WEAPON	1200	//	아이템 중 무기
#define SN_SHIELD	1201	//	방패
#define SN_ARMOR	1202	//	갑옷
#define SN_CLOTHE	1215	//	갑옷( 의류 )
#define SN_HELMET	1203	//	헬멧
#define SN_UNIQ		1204	//	유니크 아이템
#define SN_NECK		1204	//	목걸이
#define SN_SHOES	1205	  //  신발		등의 아이템을  장착할경우


/// 살때, 팔때
#define SN_ITEM_BUY				1236
#define SN_ITEM_SELL			1237	//아이템을 팔때

//// 
#define SN_EAT_BREAD			1208	//	빵먹을때 (소비)
#define SN_EAT_POTION			1209	//	포션 먹을때 (소비)
#define SN_DRINK_BEER			1226	//	맥주 마시기(소비)


///////////////////
#define SN_WARNING				1213	//	경고음

#define SN_BOX_OPEN				1219//	박스 아이템 열경우
#define SN_BOX_CLOSE			1220///	박스 아이템 닫을 경우


#define SN_MAGIC_BOOK_OPEN		1221//	마법책 열경우
#define SN_MAGIC_BOOK_CLOSE		1222//	마법책 닫을 경우
#define SN_GET_BOOK				1223//	// 책 빼내는 소리
#define SN_BOOK_NEXT			1224//	// 책 넘기는 소리

// MENU 
#define SN_MENU_OPEN	1216//	메뉴가 열릴때
#define SN_MENU_CLOSE	1217//	메뉴가 닫칠때

// 메뉴
#define SN_MOVE_OPEN	SN_MENU_OPEN // 1214//	메뉴가 움직이면서 오픈될경우( 대리석 움직이는 듯한 소리 )
#define SN_MOVE_CLOSE	SN_MENU_CLOSE //	메뉴가 움직이면서 닫칠경우
///////////////////////////////////////////////
// 마우스 관련
#define SN_TOUCH		1212	//	마우스가 버튼에 다가갈경우
#define SN_CLICK		SN_CLICK_START//1211	///	클릭할경우

//전투
#define SN_FIGHT		1218
#define SN_LOGOUT		1240   // 로그아웃할때

#define SN_RECV_BBS		64
#define SN_FACE_PUT		64		// 표정 연출
#define SN_MAGIC_BALL	1225	// 마법구 반짝이는 소리( 마우스가 가까이 갔을 때만 처리하는게 좋겠다 )

#define SN_SKILL		1218//	// 스킬 아이콘 클릭할 경우
//#define SN_DEFAULT		 9

#define SN_SLEEP				1250//	여관에서 잠자기
#define SN_REST					1250//	여관에서 쉴때
#define SN_TENT					1250//	텐트에서 쉴때

//#define SN_TOUCH_POTION			//삭제//	포션을 들고 있거나 마우스가 포션에 가까이 갔을경우

//-- 수정 (삭제)

#define SN_IRON_DRUM			1227	// 쇳물통
#define SN_DRUM					1228	// 물통
#define SN_NEW_CHARACTER		1229	//	새로운 캐릭터를 만들었을 경우

// #define SN_SELL_ITEM ?
// #define SN_BUY_ITEM ?

#define SN_BUY_HOUSE			1230//	집을 샀을 경우
#define SN_ITEM_REPAR			1231//	아이템을 고칠때
#define SN_LEARN_ITEM			1232//	기술을 위해 아이템을 배울때
#define SN_LEARN_MAGIC			1232//	마법을 배울때
#define SN_MAGIC				1218    //매직 아이콘 클릭할 경우
#define SN_LEARN_SKILL			1232	// 스킬을 배울때
#define SN_LV_EACH_UP			1233	// 각각의 레벨업 포인트를 올릴때, 
#define SN_LV_EACH_DOWN			1233	// 각각의 레벨업 포인트를 내릴때, 

//#define SN_DO_MEMORIZE			수정		//	임시	// 메모라이즈를 했을 경우 -- 메모라이즈 마법 계열별로 데이타가 틀려짐..리스트 참조...or 1230번 데이타로 통일..
#define SN_DO_MEMORIZE			1230	// 임시	// 메모라이즈를 했을 경우

#define SN_GET_MONEY			1206	//돈을 들때
#define SN_PUT_MONEY			1236	///돈을 놓을때


//- 수정 : 구입한 아이템이 아이템 창에 놓일때의 소리 (즉 각 아이템 마다 아이템 창에 놓일때의 소리가 다를 경우 그 해당 효과음 발생..)
#define SN_GAMEEND				1240	//게임을 끝낼때
	
#define SN_GOD_REGIST			1230	//신전에 등록할때
#define SN_GOD_MEETING			1252	// 예배 음악

#define SN_EAR_MESSAGE_RECV		1241	// 귓속말 받았을 때

#define SN_SEND_MAIL			1243	// 메일 보낼때
#define SN_RECV_MAIL			1242	// 메일 받을때


////////////////////////////////////////////////////////////
#define	SN_KILL_HUNTER_OK		1230		// 현상범을 사냥하는데 성공했을 경우
#define	SN_KILL_HUNTER_NOT		1213		// 현상범을 누가 먼저 사냥해 가버린 경우 
#define SN_START_FIGHT			SN_FIGHT	// 1:1 결투 시작

/////////////////////////////////////////////////////////////

//#define SN_FIGHT_READY				61
//#define SN_FIGHT_START				68

#define SN_KILL_WHO					55
////////////////////////////////////////////////

#define SN_OPEN_NATION_BBS			67
/////////// 0808 lkh 수정 ///////////
#define	SN_START		1	//	게임 시작할때
#define	SN_BGMEND		6		//마지막 배경음악번호
#define SN_X_MAS_BGM	9			// 크리스마스 배경음

/////////////////////////////////////////////////


#define SN_QUEST_START	50
#define SN_QUEST_END	51

#define SN_GM_REGIST_OK	51

#define SN_INFO			400
#define SN_DUAL_MUSIC  67
#define SN_DUAL_SELECT 151
#define SN_DUAL_OK     2409


//////////////////////////////////////////////////////////////////////////////////////
// 추가 하고 싶은것
// #define SN_KILL_PC		?
// #define SN_END_FIGHT		?





extern void MenuDisplay();      //메뉴의 활성여부를 판단해 출력하는 함수
extern void MenuSetting();      //게임을 시작하기 전에 메뉴 구조체들을 초기화  GamePorc() 전에 호출
extern void StartMenuSetting();      //게임을 시작하기 전에 메뉴 구조체들을 초기화  GamePorc() 전에 호출
extern void MenuChecking();     //메뉴 구성요소의 충돌체크

extern void StartMenuDisplay();      //메뉴의 활성여부를 판단해 출력하는 함수
extern void StartMenuChecking();     //메뉴 구성요소의 충돌체크
extern void LoadMenuData(int nNum);                   //필요한 메뉴 이미지 데이타를 메모리에 로드
extern void FreeMenuData(int nNum);                    //필요 없는 메뉴 이미지 데이타 메모리를 반환

extern bool g_DropOnHero;
extern int StartPosition ;
extern int StartMap;
//021014 lsw
extern SMENU	SMenu[MAX_MENU]; 
extern const int mainmenu[];
extern const int smallmenu[];
extern const int aNoCloseMenu[];
extern const int aNoCloseOfMainMenu[];

extern const int MAX_MAIN;
extern const int MAX_SMALL;
extern const int max_submenu;
extern const int MAX_NO_CLOSE_MENU;
extern const int MAX_NO_CLOSE_MAIN_MENU;
extern bool CanMoveItemToMenu(const int iFieldType);

extern int		RectImageOn;
extern int		DayLightControl;		// 15 보다 크면 낮 , 작으면 밤
extern bool		is_mainmenu_active;
extern bool		g_bDualMagicBookOpen;//soto-030421
extern int		g_StartMenuOn;
extern BOOL		MouseDrag;

extern	CharacterParty mouse_ch;
extern bool g_bTimeMenuOn;			//	time menu open or close
extern bool	g_ExplainMenuOn;		//  Do open or close for explain menu 

extern	ItemJoinSelect Item_Select[MAX_LEARN_SKILL_PAGE][10];		// 아이템 만들기 전에 선택하는 목록
extern	ItemJoinSelect item_select_result;		// 아이템 만들기에서 선택한 아이템

extern	PosItem		skill_use_item[5][10];
extern	PosItem		skill_use_item_result;
extern	int SkillOn;							// quick skill 0n
extern	bool g_GoDice;						// 주사위를 돌려도 되는가 안되는가...
extern	bool g_GoDice2;
extern	int nDiceNumber[5];
extern	bool g_GoDiceRecv;

extern	k_client_candidate		Candidate[3];	// 입후보 정보
extern int	g_Candidate_max;
extern int g_CurrVote;
extern int g_CanRegistCandidate;

extern char FightTeam[5][13];
extern	DWORD FightTeamColor[5];

extern BYTE	*buf_alphabet;
extern Spr	spr_alphabet[36];


extern BYTE	*facebuf_man_b;
extern BYTE	*facebuf_woman_b;
extern BYTE	*facebuf_man_m;
extern BYTE	*facebuf_woman_m;
extern BYTE	*facebuf_man_s;
extern BYTE	*facebuf_woman_s;
extern BYTE	*facebuf_man_p;
extern BYTE	*facebuf_woman_p;
extern BYTE	*facebuf_man_mm;
extern BYTE	*facebuf_woman_mm;


extern Spr spr_face_man_b[FACE_MAX];
extern Spr spr_face_woman_b[FACE_MAX];
extern Spr spr_face_man_m[FACE_MAX];
extern Spr spr_face_woman_m[FACE_MAX];
extern Spr spr_face_man_s[FACE_MAX];
extern Spr spr_face_woman_s[FACE_MAX];
extern Spr spr_face_man_p[FACE_MAX];
extern Spr spr_face_woman_p[FACE_MAX];
extern Spr spr_face_man_mm[FACE_MAX];
extern Spr spr_face_woman_mm[FACE_MAX];


// 0906 kkh 추가 
extern Spr spr_face_man_b_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_b_dash[ADD_CHA_PIC];
extern Spr spr_face_man_m_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_m_dash[ADD_CHA_PIC];
extern Spr spr_face_man_s_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_s_dash[ADD_CHA_PIC];
extern Spr spr_face_man_p_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_p_dash[ADD_CHA_PIC];
extern Spr spr_face_man_mm_dash[ADD_CHA_PIC];
extern Spr spr_face_woman_mm_dash[ADD_CHA_PIC];

extern BYTE	*facebuf_man_b_dash;
extern BYTE	*facebuf_woman_b_dash;
extern BYTE	*facebuf_man_m_dash;
extern BYTE	*facebuf_woman_m_dash;
extern BYTE	*facebuf_man_s_dash;
extern BYTE	*facebuf_woman_s_dash;
extern BYTE	*facebuf_man_p_dash;
extern BYTE	*facebuf_woman_p_dash;
extern BYTE	*facebuf_man_mm_dash;
extern BYTE	*facebuf_woman_mm_dash;


///////////////////////////////////////////////////
extern BYTE	*god_spr_buf;
extern Spr	god_spr[GOD_MENU_IMG_MAX];		// 신전 메뉴 

extern BYTE	*time_spr_buf;
extern Spr	time_spr[TIME_MENU_IMG_MAX];

extern BYTE	*membook_spr_buf;
extern Spr	membook_spr[MEMBOOK_MENU_IMG_MAX];		

extern BYTE	*magicbook_spr_buf;
extern Spr	magicbook_spr[MAGICBOOK_MENU_IMG_MAX];		

extern BYTE	*mail_spr_buf;
extern Spr	mail_spr[MAIL_MENU_IMG_MAX];	

extern BYTE	*start_etc_buf;
extern Spr	start_etc_spr[START_ETC_IMG_MAX];	

extern BYTE	*start_etc2_buf;
extern Spr	start_etc2_spr[START_ETC2_IMG_MAX];	

extern BYTE	*job_etc_buf;
extern Spr	job_etc_spr[JOB_ETC_IMG_MAX];

extern BYTE	*select_server_buf;
extern Spr	select_server_spr[SELECT_SERVER_IMG_MAX];	

extern BYTE	*start_map_buf;
extern Spr	start_map_spr[START_MAP_IMG_MAX];	

extern BYTE	*chat_menu_buf;
extern Spr	chat_menu_spr[CHAT_MENU_IMG_MAX];	

extern BYTE	*main_etc_buf;
extern Spr	main_etc_spr[MAIN_ETC_IMG_MAX];	

extern BYTE	*main_etc3_buf;
extern Spr	main_etc3_spr[MAIN_ETC3_IMG_MAX];	

extern BYTE	*skill_menu_buf;
extern Spr	skill_menu_spr[SKILL_MENU_IMG_MAX];	

extern BYTE	*small_map_buf;
extern Spr	small_map_spr[SMALL_MAP_MAX];

extern BYTE	*small_etc_buf;
extern Spr	small_etc_spr[SMALL_ETC_MAX];

extern BYTE	*public_bbs_buf;
extern Spr	public_bbs_spr[PUBLIC_BBS_MENU_IMG_MAX];

extern BYTE	*hunter_buf;
extern Spr	hunter_spr[HUNTER_MENU_IMG_MAX];

extern BYTE	*questmenu_buf;
extern Spr	questmenu_spr[QUEST_MENU_MAX];
// 경하가 
extern BYTE	*famemenu_buf;
extern Spr	famemenu_spr[FAME_MENU_MAX];

// 0905 kkh 추가 
extern BYTE	*chcolorcha_buf;
extern Spr	chcolorcha_spr[COLOR_CHANGE_MENU_MAX];
//	0908 KKH 추가 
extern BYTE	*item_make_image_buf;
extern Spr	item_make_image_spr[ITEM_MAKE_IMAGE_MAX];

extern BYTE	*nation_buf;
extern Spr	nation_spr[NATION_MENU_IMG_MAX];

extern BYTE	*start_fight_buf;
extern Spr	start_fight_spr[START_FIGHT_IMG_MAX];

extern BYTE	*stop_fight_buf;
extern Spr	stop_fight_spr[STOP_FIGHT_IMG_MAX];

extern BYTE	*fightmap_time_buf;
extern Spr	fightmap_time_spr[FIGHTMAP_TIME_IMG_MAX];

extern BYTE	*fightmap_win_buf;
extern Spr	fightmap_win_spr[FIGHTMAP_WIN_IMG_MAX];

extern BYTE	*fightmap_lose_buf;
extern Spr	fightmap_lose_spr[FIGHTMAP_LOSE_IMG_MAX];

extern Spr scenario_spr;		// 시나리오..

extern BYTE	*vote_buf;
extern Spr	vote_spr[VOTE_MENU_IMG_MAX];

extern BYTE	*guild_mark_buf;
extern Spr	guild_mark_spr[GUILD_MARK_MAX];

extern BYTE	*guild_admin_buf;
extern Spr	guild_admin_spr[GUILD_ADMIN_MAX];

extern BYTE	*guild_skill_list_buf;
extern Spr	guild_skill_list_spr[GUILD_SKILL_LIST_MAX];

extern BYTE	*guild_admin_etc_buf;
extern Spr	guild_admin_etc_spr[GUILD_ADMIN_ETC_MAX];

extern BYTE	*help_pcx_buf;
extern Spr	help_pcx_spr[HELP_PCX_MAX];

extern BYTE	*bank_buf;
extern Spr	bank_spr[BANK_MAX];

extern BYTE	*salvation_buf;
extern Spr	salvation_spr[SALVATION_MAX];

extern BYTE	*salvation_ani_buf;
extern Spr	salvation_ani_spr[SALVATION_ANI_MAX];

extern BYTE	*team_battle_buf;
extern Spr	team_battle_spr[TEAM_BATTLE_MAX];

extern BYTE	*cahnge_money_buf;
extern Spr	cahnge_money_spr[CAHNGE_MONEY_MAX];

extern BYTE	*guild_wait_buf;
extern Spr	guild_wait_spr[GUILD_WAIT_SPR_MAX];

extern BYTE	*merchant_buf;
extern Spr	merchant_spr[MERCHANT_SPR_MAX];

extern BYTE	*guild_menu_buf;
extern Spr guild_menu_spr[GUILD_MENU_SPR_MAX];

extern BYTE	*dual_img_buf;
extern Spr dual_img_spr[DUAL_IMG_SPR_MAX];


///////////////////////////////////////////////////////

extern int g_VoteSignExist;
extern int BetaTest;
extern int SoundOn;

extern void SetRect(RECT &rect,int left, int top, int right, int bottom);
extern void FieldTypeNomalPut(int mx, int my, int x, int y, int nImageNumber, int type = 0);
extern void FieldTypeNomalPutFx(int mx, int my, int x, int y, int nImageNumber, int t, int j);
extern void FieldTypeNomalPutFx3(int mx, int my, int x,int y,int nImageNumber, int type , int t, int j);//010910 lsw

//######## 메인 인터페이스에서 셋팅할 데이터 정의 ##########
#define IF_MAP				0		// IF -> info_flag
#define IF_CHAT_MESSAGE		1
#define IF_BATTLE_PEACE		2		//	0:peace, 1:battle

#define INFO_FLAG_MAX		3		// 정보 플래그 갯수

struct	InterfaceFlag
{
	//acer7
	int data[INFO_FLAG_MAX];
};

extern struct	InterfaceFlag main_interface;
extern bool		IsMenuActive();					// 활성화된 메뉴안에 마우스가 있는지를 판단
extern void		LoadSmallMenuImage( int num );	// 없는 그림 로드해온다.
extern void		CallMenu(int Menu, int x, int y, BOOL bMenuFocus=FALSE);
extern void		CallMenu( int Menu, int temp=0, bool is_focus = false );




extern	void	SendLevelUpPoint( WORD p[11] ) ;
extern	void	RecvLevelUpPoint( t_server_levelup_point *p );
extern	void	RecvLevelUpPointResult( int ok );
extern	void	RecvCallSmallMenu( t_server_call_smallmenu *p );
extern	void	RecvCallDualMagicLearnBook();//soto-030421
extern	void	ReqSmallMenu( int menu, int storelistindex );
extern	void	RecvStoreItemList( t_server_store_itemlist *p );
extern  void	RecvMallItemList(t_MallItemList *p);

extern	void	SendItemBuy( int item_pos, int inv_pos, int store_id );
extern	void	SendItemSell( int item_pos, int store_menu );
extern	void	SendMallItemBuy(int num, int inv_pos);
extern	int		SendDeleteCharacter( const char *szName, const char *szSecretKeyCode);

extern	int		GetMagicCastingTime( int magic_no, SCharacter &ch );
extern	int		CanMakeItemListSet( int s , int menu=0);
extern	void	DropItemOnHero( ItemAttr &item_attr );
extern	void	CallOtherCharacterInv( ItemAttr inv[3][3][8], ItemAttr equip[8], ItemAttr quick[7] );



extern	int		SearchEmpty(int&, int&, int&);
extern	ItemAttr *SearchEmpty();
extern	int SearchEmptyQuickItem();
extern	int SearchEmptyBank(int &a, int &b, int &c);

extern	ItemJoinSelect learn_item_list[MAX_LEARN_ITEM];
extern	void	CallSmallMenu(int menu, int pre=0, int sound=SN_MENU_OPEN);											// 서브 메뉴 콜할때 사용


extern	void	CallServer( int type, void *msg =NULL, int size=0 );			// 서버로 뭔가를 요청할때

extern	int		g_MouseItemNumber;
extern	int		g_MouseItemType;

extern int GetIndexOfCondition( BYTE condition );
extern int RectAndRectCrash( const RECT *box1, const RECT *box2 );
extern int RectAndDotCrash( const RECT *box1, const int x, const int y );

extern void FreeOpenningSpr( BYTE **buf );
extern void ReStartMenu( );

extern int IsMouseOnMenu();
extern void SmallMenuClose( );
extern void MainMenuClose( );


extern Spr spr2[MAX_IMAGE2];		// 추가 메뉴 이미지 
extern void FieldTypeNomalPutFx2(int mx, int my, int x,int y,int nImageNumber, int t, int j);
extern void FieldTypeNomalPut2(int mx, int my, int x,int y,int nImageNumber);
extern void LoadSmallMenuImage2( int num );
extern inline void UpperMenuNomalPut(int &x, int y,int nImageNumber, int type);


extern bool bCallGodMeetingMenu;
extern void CloseAllMenu();
extern int CheckLanguageForMenu( LPSTR lpStr );

extern HWND SetFocus2( const int iIndex);//021001 lsw
extern int GetDiceAbility( int DiceNumber );		// 다이스 돌리기

extern void CutHangul( char *text, int max );
extern void AddWaveOfEndString( char *text );		// 물결을 붙여준다.		문자열은 넉넉이 잡혀 있어야 한다.

extern int isSmallMenuOn();
extern int RectTextPut(const int x, const int y, int nWidth, char *sSource, const int hight = 18, const int line_max = 0);
extern int RectTextPutNotEatWhiteChar(const int x, const int y,int nWidth, char *sSource,const int hight = 18, const int line_max = 0);
extern int RectBoldTextPut(const int x, const int y,int nWidth, char *sSource, const int hight=18, const int line_max= 0);//020730 lsw
extern int RectBoldTextPutNotEatWhiteChar(const int x, const int y,int nWidth, char *sSource, const int hight=18, const int line_max= 0);//020730 lsw
extern int GetRectTextLine( char *sSource, int nWidth, const int line_max = 0);//020703 lsw
extern char *DivideStringByRect( int nWidth, char *sSource, int max_line = 0 );
extern bool CanActiveMessageLine();
extern int GetLineRectTextPut( int nWidth, char *sSource ); // line의 수를 체크하여..
extern int SkipRectTextPut( int x, int y, int nWidth, int skip, char *sSource, int line_max = 0, int fx_type = 0 ); // line의 수를 체크하여..
extern void GameEndFree();
extern void RegistProgram();
extern int LoadSmallMapSprData(int mapnumber); // CSD-030324 // thai2 YGI
extern int  PutHpGauge( int x, int y, LPCHARACTER ch );
extern int  PutHeroHpGauge( const int x, const int y);//020626 lsw
extern int  PutHeroMpGauge( const int x, const int y);
extern int GetTextLine( char *filename );
extern void InsertMagicQuickArray( int magic_no );
extern void DeleteMagicQuickArray( int magic_no );
extern void ResetMagicQuickArray( );
extern void MagicSetting2();
extern bool	ReadMagicTable();
extern char *GetNameById( int id );		// 아이디로 이름을 가져 온다.
extern int ConvMapnameToMapnumber( char *mapname );
extern Spr *GetSprOfMenu( int type, int nImageNumber );		// 스프라이트 데이타
extern void MP3( int n, int ing=0 );
extern void PutLvNameByColor( int x, int y, LPCHARACTER ch );
extern void PutDualMark(int x, int y, LPCHARACTER ch);
extern bool IsHeroDead();
extern char NationName[8][20];
extern const DWORD NationColor[7];
extern const WORD NationColor16[7];
extern int	fight_map_live_man[4];
extern void FreeAllOfMenu();		// 0927 YGI
extern void DeleteMouseCh( char *name );
extern void SetMouseCh( LPCHARACTER lpTarget );
extern LPCHARACTER ExistHe( char *name );		// 내 시야에 존재하는 넘인지 알아보는 루틴
extern LPCHARACTER ExistHeCase( char *name );
extern void InitMenuByChangeMap();
extern void Check_Kein_Menu( int type );		// 툴에서 메뉴
extern int LoadGuildTableEx( );
extern void PutJobLv( int x, int y, int job, int call_job_lv );		// 1004 YGI( 직업 등급 출력 )
extern int GetCallJob( int call_job_lv );		// 직업 능력 단위 가져오기

extern void ViewGuildCommandInfo( );		// 길드 명령 설명
extern char *Kein_EatFrontWhiteChar( char *pStr );

extern void ViewEarCommandInfo();

extern BOOL LaunchApplication(LPCTSTR pCmdLine );
extern BOOL LaunchApplication_kein( char *app_name, char *arg );
extern void KeinMessageBox( int next_menu, char *msg, ... );// 020701 YGI

extern int AutoLevelUpOn;
extern DWORD AutoLevelUpCount;
extern void AutoLevelUp();
extern char man_list[MAX_MAP_][MAN_LIST_MAX_][20];		// 상점 주인 이름들
extern int TotalSalvation;

extern void FreeAndReload();
extern int g_SetGuildMarkViewOn;
extern int g_GuildMax;

extern void PrintMagicExplain( int magic_no );
extern int	g_CurrUsedMagic;		// 현재 캐스팅 되고 있는 마법

// ok만 있는 건지 ok_cancel이 있는 메시지 박스인지를 선택,  menu는 호출한 메뉴를 넣는다
extern void CallOkCancelMessageBox( int menu, int x, int y, const char *source, bool ok_cancel = true, int param = 0 );

extern int g_fight_win;
extern int g_fight_lose;

extern int g_RemainTimeType; 
extern int g_RemainTimeDelay;

extern int  ConvString( char *szStr ); // 욕설 필터
extern char checkbeta;
extern bool bSmallTips;
extern char *kein_GetMenuString( int index );

extern char *g_szScale;


#define KM_INFO		0		// Kein_PutMessage
#define KM_FAIL		1
#define KM_OK		2
#define KM_BBS		3

extern void Kein_PutMessage( int type, char *msg, ... );
extern int g_SelectServerIndex;


#define SI_GAME_MAKE_MODE		0
#define SI_NOT_CONNECT_MODE		1

extern inline int GetSysInfo( int type );
extern bool CheckTempChar( char ch, char *white );
extern bool IsReporterMode();

#define PAGE_NUMBER__	 2//020515 lsw 이동 했습니다

extern bool g_bLogOnFlag;
extern SCharacter		LoadCh[4];								// 서버에서 캐러 정보를 전송받는다extern SCharacter LoadCh[4];								
extern t_client_login_fail_reason g_client_login_fail_reason;		// 로그인 시 메시지

extern void RecvChatSetResult( t_chat_set_result *p );
extern CHARACTERLIST	g_CharacterList;											
extern SCharacter		LoadCh[4];								// 서버에서 캐러 정보를 전송받는다extern SCharacter LoadCh[4];								
// 서버에서 캐러 정보를 전송받는다

extern int		YouGotItemFlag;
extern BOOL		MouseDrag;


extern ItemAttr InvItemAttr[3][3][8];		//### 1214 YGI

extern void SettingChangMapMenu( int ct ); // smallmenu.cpp
extern void RecvPleaseGiveLife( int id, int hp, int mana, int hungryp, int who );

extern void	RecvQSF_Command( t_qsf_command	qc );

extern int CheckHandleByNationWar(t_packet *p);  // 010904 LTS

extern void Recv2NDGMMAKEResult(t_SkillRareUpgradeResult *p);//011004 lsw
extern void RecvCallSmallMenuCustom(t_server_call_smallmenucustom *p);//011011 lsw


extern bool	DoButtonCheckOfMenu_SWD(int nMenuIndex,int nFieldIndex);//soto-030430


extern  int		GetItemAttr( int item_id, int attr);				//011018 KHS 망또
extern  int		GetItemAttr2(int sprno, int item_id, int attr);		//011018 KHS 망또


extern void SendRequestTileDontData();		// 010904 LTS
extern void SendCMD_REQUEST_COMPARE_PHASE();	// LTS LOCALWAR	
extern void SetLocalWarArmor(t_server_change_equip *p,LPCHARACTER ch);	// LTS LOCALWAR
extern void SetLocalWarArmor(t_server_user_add *c);	// LTS LOCALWAR
extern bool g_bWarBegin; 
extern int	g_LocalWarBegin;					// LTS LOCALWAR
extern void SetWarArmor(t_server_change_equip *p,LPCHARACTER ch);
extern void SetWarArmor(t_server_user_add *c);	// 010904 LTS
extern int GetLeveByExp(int nStep, DWORD exp);
extern ItemAttr *GetItemByPos( POS item ); // itemtable.cpp
extern 	SCharacter			SCharSource;		
extern 	int					character_active;
extern	int					g_LocalWarBegin;			// 011215 LTS	
#endif//AFX_MENU_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_