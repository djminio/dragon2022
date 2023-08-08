#include <sqlext.h>
#include "Menudefine.h"
//#include "MyTypes.h"

#ifndef __MenuDef_h__
#define __MenuDef_h__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FONTSIZE_			6           //글자 1개의 가로길이(영문) strlen함수로 바이트를 얻고 거기에 6을 곱한다

#define ConvertColor(r,g,b)	(	(r>>3)<<11 | (g>>3)<<6 | b>>3	)//020610 lsw

const WORD FONT_COLOR_DEFAULT			= ConvertColor(141,144, 132);		//	보통 글자 (한글, 영문)
const WORD FONT_COLOR_BLACK				= ConvertColor(0	,0	, 0);		//	보통 글자 (한글, 영문)

const WORD FONT_COLOR_SOLID_YELLOW		= ConvertColor(255,255, 0);		//	진짜 노랑
const WORD FONT_COLOR_SOLID_RED			= ConvertColor(255,0, 0);		//	진짜 빨강
const WORD FONT_COLOR_SOLID_BLUE		= ConvertColor(0,0, 255);		//	진짜 파랑
const WORD FONT_COLOR_SOLID_GREEN		= ConvertColor(0,255, 0);		//	진짜 초록
const WORD FONT_COLOR_SOLID_VIOLET		= ConvertColor(255,0, 255);		//	진짜 보라
const WORD FONT_COLOR_SOLID_JADE		= ConvertColor(0,255, 255);		//	진짜 옥색

const WORD FONT_COLOR_PLUS				= ConvertColor(255,61	, 65);		//	능력치에서 플러스 된 숫자	(붉은 색)
const WORD FONT_COLOR_NAME				= ConvertColor(255,220, 100)	;	//	이름 (금색글자)

const WORD FONT_COLOR_YELLOW			= ConvertColor(200,170, 75)	;	//	이름 (금색글자)
const WORD FONT_COLOR_ORANGE			= ConvertColor(255,220, 80)	;	//	검정색바탕에서 튀는 색 (주황색)

const WORD FONT_COLOR_BLIGHT_GREEN		= ConvertColor(150	,220, 30)	;	//	연두색
const WORD FONT_COLOR_BLIGHT_GREEN2		= ConvertColor(30	,220, 150)	;	//
const WORD FONT_COLOR_BLIGHT_BLUE		= ConvertColor(30	,150, 220);		//	밝은파란색
const WORD FONT_COLOR_BLIGHT_VIOLET		= ConvertColor(220	,30	, 150);		//	밝은 보라색
const WORD FONT_COLOR_BLIGHT_ORANGE		= ConvertColor(220	,150, 30)	;	//	밝은 오랜지색
const WORD FONT_COLOR_BLIGHT_RED		= ConvertColor(220	,150,150);
const WORD FONT_COLOR_BLIGHT_YELLOW		= ConvertColor(30	,220,30);

const WORD FONT_COLOR_GREEN				= ConvertColor(50	,230, 50);		//	초록색
const WORD FONT_COLOR_RED				= ConvertColor(230,50	, 50);		//	빨강
const WORD FONT_COLOR_BLUE				= ConvertColor(50	,50	, 230)	;	//	파란색

const WORD FONT_COLOR_SPECIAL			= ConvertColor(101,169, 18)	;	//	특별한 경우 ( 녹색계통)
const WORD FONT_COLOR_NUMBER			= ConvertColor(221,230, 206);		//	보통 숫자

const WORD FONT_COLOR_NORMAL			= ConvertColor(141,144, 132);
const WORD FONT_COLOR_RECT				= ConvertColor(221,230, 206);
const WORD FONT_COLOR_BUTTON			= ConvertColor(255,61	, 65);

const WORD FONT_COLOR_SKILL				= ConvertColor(4	,152, 54);		//	스킬 설명

const WORD FONT_COLOR_MAX				= ConvertColor(255,255, 0	);		//  능력치 최고일경우 ( 노란색 )

const WORD FONT_COLOR_BORA				= ConvertColor(230,90	, 230)	;		// party chat color

const WORD FONT_COLOR_HIGH_RARE_MAIN	= ConvertColor(230	, 230 , 80)	;		//옥색
const WORD FONT_COLOR_HIGH_RARE_DISPLAY	= ConvertColor(150	, 230 , 50);		//레어 프러스 포인트

const WORD FONT_COLOR_RARE_MAIN			= ConvertColor(0	,240, 240)	;		//옥색
const WORD FONT_COLOR_RARE_DISPLAY		= ConvertColor(80	,183, 255);		//레어 프러스 포인트 
const WORD FONT_COLOR_RARE_PLUS_POINT	= ConvertColor(255,255, 75);		//레어 프러스 포인트 
const WORD FONT_COLOR_RARE_MINUS_POINT	= ConvertColor(255,0	, 0	);		//옥색

const WORD FONT_COLOR_WHITE				= ConvertColor(233,233,233);//020725 lsw
const WORD FONT_COLOR_EXP_PERCENT		= ConvertColor(220,230,9);//020725 lsw

extern DWORD Convert16to32(const WORD wColor);

////////////////////////////////////////////////////////////////////////////////////////////////////
#define FS_BIG		0		// 큰 얼굴
#define FS_MIDDLE	1		// 중간 얼굴
#define FS_SMALL	2		// 작은 얼굴 ( default )
#define FS_MINI		3		// 파티원 얼굴
#define FS_MICRO	4		

/////////////////////////////////////////////////
#define INN_SINGLE	10
#define INN_DOUBLE	20
#define INN_MULTI	30

///////////////////////////////////
#define LEFT		0
#define RIGHT		1

#define SCROLL_UP			0
#define SCROLL_DOWN			1

/////////// 얼굴 이미지 /////////////////////
#define FACE_MAX 10
#define ADD_CHA_PIC 20	// 0906 kkh 추가 

#define IIT_FRIEND					920
#define IIT_POTAL					930
#define IIT_PARTY_BUTTON			940
#define IIT_GUILD_ITEM				950
#define IIT_GUILD_CHANGE_INFO		960
//Menu_Image.txt 의 내용입니다. 겹쳐서는 아니됩니다.(겹치게 쓰시면 컴파일 자체가 안됩니다 ㅡㅡ;..)
enum ImageFileIndex
{
	DEFAULT_MENU				=	0	,
	GOD_MENU					=	100,
	TIME_MENU					=	110,
	MEM_BOOK					=	120,
	MAIL_MENU					=	130,
	START_ETC					=	140,
	RESET_JOB_ETC				=	141,
	CHAT_MENU					=	150,
	MAGIC_BOOK_MENU				=	160,
	MAIN_ETC					=	170,
	SMALL_MAP					=	180,
	START_MAP					=	190,
	SMALL_ETC					=	200,
	SKILL_MENU					=	210,
	PUBLIC_BBS					=	220,
	HUNTER_MENU					=	230,
	QUEST_MENU					=	240,
	FAME_MENU					=	250,
	CHARACTER_COLOR_CHANGE		=	260,
	ITEM_MAKE_IMAGE				=	270,
	NATION_BBS_MENU				=	280,
	START_FIGHT_MAP				=	290,
	STOP_FIGHT_MAP				=	300,
	VOTE_MENU					=	310,
	GUILD_MARK					=	320,
	GUILD_ADMIN					=	330,
	GUILD_LIST					=	340,
	START_ETC2					=	350,
	GUILD_ADMIN_ETC				=	360,
	HELP_PCX					=	370,
	BANK_PCX					=	380,
	SALVATION_PCX				=	390,
	SALVATION_PCX_ANI			=	400,
	TEAM_BATTLE					=	410,
	SELECT_SERVER				=	420,
	CHANGE_MONEY				=	430,
	GUILD_WAIT					=	440,
	MERCHANT_IMG				=	450,
	FIGHT_MAP_TIME				=	460,
	FIGHT_MAP_WIN				=	470,
	FIGHT_MAP_LOSE				=	480,
	MAIN_ETC3					=	490,
	GUILD_MENU_IMG				=	500,
	GM_2ND_SKILL_IMG			=	510,
	NATION_BATTLE_BBS_IMG		=	520,
	NATION_INFO_IMG				=	530,
	NATION_VOTE_IMG				=	540,
	NATION_BUDAE_IMG			=	550,
	NATION_NOW_IMG				=	560,
	NATION_LADDER_IMG			=	570,
	NATION_JOIN_IMG				=	580,
	NATION_GOTO_WAR_MAP_IMG		=	590,
	NATION_GUEST_IMG			=	600,
	NATION_MSGBOX_IMG			=	610,
	NATION_DONEBOX_IMG			=	620,
	NATION_TODAY_NOTICE_IMG		=	630,
	NATION_WAR_RESULT_IMG		=	640,
	NATION_WAR_START_IMG		=	650,
	DUAL_MARK_IMG				=	730,
	ITEM_MAKE_RARE_SELECT_IMG	=	740,
	NEW_CHAT_IMG				=	750,
	DUAL_IMG					=	760,
	TUTORIAL_DISPLAY_IMG		=	770,
	RUNE_IMG					=	780,
	RUNE_EFFECT1				=	790,
	RUNE_EFFECT2				=	800,
	RUNE_EFFECT3				=	810,
	RUNE_EFFECT4				=	820,
	RUNE_EFFECT_DEFAULT			=	830,
	SKIN_MENU					=	840,
	GAMBLE_IMG					=	850,
	IIT_EXP_GAUGE				=	860,
	GAMBLE_EFFECT				=	870,
	TUTORIAL_DISPLAY_TMP_IMG	=	900,
	IIT_GUILD_MEMBER_LIST		=	910,
	ABILITY_REDISTRIBUTION_IMG	=	880,
	LOCALWAR_JOIN_IMG			=	881,
	LOCALWAR_OK_IMG				=	882,
	EVENT_LOCALWAR_OK_IMG		=	883,
	EVENT_LOCALWAR_MOVE_IMG		=	884,
	WAR_START_IMG				=	885,
	WAR_MOVE_IMG				=	886,
	WAR_STATUS_IMG				=	887,
	MONSTER_WAR_IMG				=	888,	// BBD 040311 몬스터 국가전 상태 화면 우상단 표시용 이미지

//여기서 부터 Menu_Image.txt 의 내용입니다. 겹쳐서는 아니됩니다.(겹치게 쓰시면 컴파일 자체가 안됩니다 ㅡㅡ;..)
	COMBAT_SKILL_ICON_IMG		=	10002,
	COMBAT_SKILL_POINT_IMG		=	10003,
	MAINSTART_LOGIN_IMG			=	10005,
	START_LOGO_IMG				=	10006,
	PARTY_IMG					=	10007,	
	STATUS_IMG					=	10008,	
	INVENTORY_IMG				=	10009,	
	MAININTERFACE_IMG			=	10010,
	CONNECTING_IMG				=	10011,
	SHOW_CREDIT_IMG				=	10012,
	SELECT_CHARACTER_IMG		=	10013,	
	MANA_IMG					=	10014,	
	DIVINE_IMG					=	10015,	
	HP_IMG						=	10016,	
	NEW_START_MAP_IMG			=	10017,	
	NEW_EMOTE_IMG				=	10018,	
	NEW_GHOST_TO_ALIVE_IMG		=	10019,	
	NEW_SYSTEM_IMG				=	10020,	
	NEW_CHAT_BACK_IMG			=	10021,
	SCENARIO_IMG				=	10022,
	MERCHANT_FINDER_IMG			=	10023,
	MERCHANT_BUY_IMG			=	10024,//021008 lsw
	NEW_DEST_POINT_IMG			=	10025,
	TXT_BG_BLACK_IMG			=	10026,
	NEW_TUTORIAL_TEMP2			=	10027,
	DEL_CHAR_COMFORM_IMG		=	10028,
	TXT_BG_BLUE_IMG				=	10029,	
	TXT_BG_RED_IMG				=	10030,
	TXT_BG_BLUEGREEN_IMG		=	10031,	
	TXT_BG_BROWN_IMG			=	10032,	
	TXT_BG_GREEN_IMG			=	10033,	
	SELECT_TUTORIAL_LOGIN_IMG	=	10034,
	MERCHANT_SELL_INFO_IMG		=	10035,
	MERCHANT_SELL_REGISTER_IMG	=	10036,
	MERCHANT_RESULT_TAKE_IMG	=	10037,	
	SCRIPT_TIMER_COUNTER_IMG	=	10038,	//021025 kyo 타이머랑 카운터그림
	MERCHANT_DIRECT_EXCHANGE_IMG =	10039,
	EXCHANGE_IMG				=	10040,//교환창 그림
	MAN_TO_MAN_IMG				=	10041,//결투 그림
	SKILL_MASTER_QUEST_IMG		=	10042,//
	SKILL_MASTER_KIND_SELECT_IMG=	10043,//기성 종류 고르기
	SKILL_MASTER_MAKE_IMG		=	10044,//만들기 그림
	EXCHANGE_BOND_MONEY_IMG		=	10045,//돈교환창
	SKILL_MASTER_MARK_IMG		=	10046,//기성 상인 마크 
	MERCHANT_BBS_BACK_IMG		=	10047,//상인공지 채팅창
	NEW_BANK_IMG				=	10048,//새로운 은행 

	LUCK_MENU_IMG				=	10049,//행운 메뉴
	LOTTO_MENU_IMG				=	10050,//로또 메뉴.
	LUCKY_EVENT_IMG				=	10051,//당첨 확인창.

	ARENA_IMG					=	10052,
	OBSERVER_IMG				=	10053,
	ITEMMALL_IMG				=	10054,
	MANTLEDYE_IMG				=	10056, // Fintio 23/06/08 - mantle dye menu

};

#define NOT		0
#define OK		1

////////////////////////////////////////////////////////////////////////////
// class define 
enum ClassType//021008 lsw
{
	CLASSTYPEMIN = -1,
	WARRIOR		= 0,
	THIEF		= 1,
	ARCHER		= 2,
	WIZARD		= 3,
	PRIEST		= 4,
	CLASSTYPEMAX = 5
};

/////////////////////////////////////////////////////////////////////////////
// job define 
#define		J_FAMER				20
#define		J_MINNER			21
#define		J_FISHER			22
#define		J_LUMBERJACK		23
#define		J_BUTCHER			24
#define		J_HERB_DIGGER		25
#define		J_HERDSMAN			26
#define		J_CARPENTER			27
#define		J_BLACKSMITH		28
#define		J_COOKER			29
#define		J_TAILOR			30
#define		J_BOWCRAFT			31
#define		J_ALCHEMIST			32
#define		J_CANDLEMAKER		33
#define		J_MERCHANT			34

/////////////////////////////////////////////////////////
//	마을 선택 define ( location )
#define		L_VILLAGE			0
#define		L_HELTENT			1
#define		L_HUDAIN			2
#define		L_GREV				3


/////////////////////////////////////////////////////////
// 나라 define
#define N_NEUTRAL			0
#define N_VYSEUS			3
#define	N_ZYPERN			4
#define	N_HEGEMONIA			5
#define	N_YILSE				6
#define N_NATION_ALL		7


///////////마을 선택에 따른 마을 번호 선언 //////////////
//					( select city )
enum eSelectMap
{
	SC_HELTERNT_CASTLE	=1,
	SC_HELTERNT_VILLAGE	=2,
	SC_HUDAIN_RIVER		=3,
	SC_GRAY_MOUNTAIN	=4,
	SC_CALAHIL			=5,
	SC_BAISUS_IMPEL		=6,
	SC_RENUS		 	=7,
	SC_DEL_HAPPA		=8,
	SC_IRAMUS			=9,
	SC_DIPH_UBEN		=10,
	SC_SCHOLIUM			=11,
	SC_TYREN			=12,
	SC_BARANTAN			=13,	
	SC_SCHOLIUM2		=14,		// 추가시 StartMapPosition도 추가한다.	// 20개 까지다..//020815-2 lsw
	SC_RECENT_LOGOUT = 99,
};

//파티 메뉴와 아이템, 매직 메뉴와의 관계
#define EDIT_NAME					0
#define EDIT_ID						1
#define EDIT_PASS					2

#define ITEM_BUY		100			// BUY 메뉴 콜할때..
#define ITEM_SELL		200			// SELL 메뉴 콜할때..
#define ITEM_REPAIR		300			// REPAIR 메뉴..
#define ITEM_BUY_MALL	400

// 캐릭터 nCharacterAbility의 배열 첨자에 사용하기 위한 define
enum CharInfoType
{
	STR			=0,		// 힘
	DEX			=1,		// 건강
	CON			=2,		// 민첩성
	WIS			=3,		// 지능
	INT_		=4,		// 지혜
	CHA			=5,		// 매력
	MOVP		=6,		// 이동력
	ENDU		=7,		// 인내
	MOR			=8,		// 사기
	WSPS		=9,		// 마법 신법
	LUCK		=10,		// 행운
	HEALTH		=11,		
	MOV			=12,
	AC			=13,
	FAME		=14,
	DAMAGE		=15,
	HIT			=16,
	TACTICE		=17,
	MONEY		=18,
	LVPOINT		=19,
	LEV			=20,
	EXP			=21,
	HP			=22,
	MAX_HP		=23,
	MP			=24,
	MAX_MP		=25,
	SP			=26,
	MAX_SP		=27,
	CP	  		=28,
	MAX_CP		=29,
	BP			=30,
	MAX_BP		=31,
	R_FIR		=32,
	R_ICE		=33,
	R_ELE		=34,
	DAM_MIN		=35,
	DAM_MAX		=36,
	HIT_RAT		=37,
	DEF_RAT		=38,	
	TAC_L1		=39,
	TAC_L2		=40,
	TAC_L3		=41,
	TAC_L4		=42,
	TAC_L5		=43,
	TAC_L6		=44,
	TAC_L7		=45,
	TAC_L8		=46,
	TAC_L9		=47,
	TAC_L10		=48,
	TAC_L11		=49,
	TAC_L12		=50,
	TAC_L13		=51	,
	TAC_E1		=52,
	TAC_E2		=53,
	TAC_E3		=54,
	TAC_E4		=55,
	TAC_E5		=56,
	TAC_E6		=57,
	TAC_E7		=58,
	TAC_E8		=59,
	TAC_E9		=60,
	TAC_E10		=61,
	TAC_E11		=62,
	TAC_E12		=63,
	TAC_E13		=64,
	BANKMONEY	=65,
	FAITH		=66,
	VIEWGHOST	=67,//020314 lsw
	VIEWBOMB	=68,
	VIEWTRAP	=69,
	CIB_POTION_BOX	=70,		// YGI 020418
};
//######## SCharacter 구조체안에 nCharacterData 배열에 사용할 구조체 ##########

#define GENDER				0
#define FACE				1     
#define ARIGEMENT			2
#define CLASS				3
#define SPELL				4
#define TACTICS_MAN			9
#define TACTICS_WOMAN		10 
#define JOB					11  

//######## SCharacter 구조체안에 disease 배열에 사용할 디파인 ( OR 사용 ) ##########
#define		DISEASE1		0x00000001
#define		DISEASE2		0x00000002
#define		DISEASE3		0x00000004
#define		DISEASE4		0x00000008
#define		DISEASE5		0x00000010

#define		PREVENTION1		0x00000020
#define		PREVENTION2		0x00000040
#define		PREVENTION3		0x00000080
#define		PREVENTION4		0x00000100
#define		PREVENTION5		0x00000200


//######## tactics 에 사용 #############
#define		CRAPPLE				0
#define		SWORD				1
#define		ARCHERY				2		
#define		FENCING				3
#define		MACE				4	
#define		PIERCE				5		
#define		WHIRL				6		
#define		HURL 				7		
#define		PARRYING			8	
#define		D_SWORD				9			
#define		D_MACE				10		
#define		MAGERY				11
#define		ORISON				12

///////////////////////////////////////////////////
#define		WIZARD_SPELL				0
#define		PRIEST_SPELL				1		
#define   ACTIVE_COMBAT       3
#define   PASSIVE_COMBAT      4
#define		RECOVERY_COMBAT   5  	// 030415 kyo
//////////////////////////////////////////////////
#define		FEMALE				0
#define		MALE				1


// 전신 에니메이션에서 쓰임
#define		WARRIOR			0
#define		THIEF			1
#define		ARCHER			2
#define		WIZARD			3
#define		PRIEST			4

////////////////////////////////////////////////////////////////
// 신 ( GOD )
#define		EDELBLHOY		1
#define		TEFFERY			2
#define		LETTY			3
#define		OREM			4


/////////////////////////////////////////////////////////////////
// 메뉴 설명 type
#define EXPLAIN_MAININTERFACE		1
#define EXPLAIN_TACTICS_MAN			2
#define EXPLAIN_TACTICS_WOMAN		3

//////////////////////////////////////////////////////////////

//메뉴관련 구조체 선언
//FT_HIDE_NOMAL_GROUP_PUT ,FT_HIDE_NOMAL_ONE_PUT 에 사용할 구조체 선언
typedef struct 
{
	int nType;                //메뉴 구성요소 처럼 타입을 가질수 있다. 단 FT_TEXT_PUT, FT_TEXT_PUT_CENTER, FT_NOMAL_PUT, FT_NOMAL_PUT_CENTER만 가능
	int x;                    //자신을 포함하는 메뉴구성요소의 상위 메뉴에 대해 상대적인 x축출력 좌표
	int y;					  //자신을 포힘하는 메뉴구성요소의 상의 메뉴에 대해 상대적인 y축출력 좌표
	char temp[MAX_STRING];    //문자를 출력할 경우 사용할 것
	int nImageNumber;         //이미지 번호
	int nData;                //이미지 외에 다른 정보를 기록
	BOOL bCheak;              //영역체크를 할지 그냥 출력을 할지를 나타냄
	RECT rCheakBox;           //마우스를 사용해서 체크할경우사용할 좌표
	BOOL bShow;               //이 이미지를 출력해야할지를 나타냄
	bool bRect;				// Rect에 마우스 충돌할 경우 
}SHN;

//메뉴 구성요소 구조체 선언
typedef struct
{
	int nType;			       //메뉴 구성요소가 해야할 타입번호 저장
	int x;					   //구성요소가 찍힐 상위 메뉴에 대한 상대적인 x축 좌표
	int y;					   //구성요소가 찍힐 상위 메뉴에 대한 상대적인 y축 좌표
	int nImageNumber;		   //구성요소의 이미지 번호, DP_ 에서도 사용한다
	int nRectImage;				// 마우스 충돌때의 이미지
	int	nImageType;				//찍어야할 이미지의 스프라이트 변수 타입
	char temp[MAX_STRING];     //구성요소의 문자 내용
	int nWillDo;	           //구성요소가 선택됐을경우 해야할일(다른 메뉴호출...),FT_HIDE_SCROLL_PUT일때는 스크롤할 FT_HIDE_NOMAL_ 구성요소를 설정
	int nSpecialWillDo;        //단순히 다른메뉴호출이 아닌 특별한 일을 해야할 경우 사용
	BOOL fLButtonDown;         //왼쪽 버튼이 눌렸었는지를 나타냄
	BOOL fCheakFlag;           //구성요소가 체크됐는지를 나타냄 (마우스가 눌렸다가 떨어져야 체크) //FT_DATA_PUT에서는 구성요소가 가지고 있는값이 문자인지 숫자인지 나타내준다
	BOOL fRButtonDown;
	BOOL fRButtonCheck;
	BOOL fRectMouse;		   //범위 안에 마우스 포인터가 있는지 없는지...
	int RectCount;
	int DragCount;				// 버튼을 누르고 있는 카운트 
	RECT rCheakBox;	           //자신의 상위 메뉴에 상대적인 영역 체크값을 저장하고 있다.(배열형 체크이면 값이 없을수도 있다)
	int nSHideNomalNumber;     //FT_HIDE_NOMAL_ 에 사용할 구조체 번호
	int nSHideNomalCount;      //FT_HIDE_NOMAL_ 에 사용할 구조체의 갯수
	int nSHideNomalStart;      //FT_HIDE_NOMAL_ 에 사용할때 여러개의 목록중 몇번부터 출력을 시작할지를 결정
	int nShideNomalPlus;       //한번에 보여줄수 있는 최대치 nSHideNomalCount와 같은 값일수도 있다
	int *nValue;				// 변수를 받기위해
	void *m_lpTemp;
	void (*m_pFunction)();
} SFIELD;

//메뉴 구조체 선언
typedef struct
{
	int		Id;
	bool	bActive;            //현재 메뉴가 활성화 됐는지를 나타냄
 	int		x;					//현재 메뉴가 찍힐 화면에 상대적인 x축 좌표
	int		y;					//현재 메뉴가 찍힐 화면에 상대적인 y축 좌표
	int		nImageNumber;       //현재 메뉴의 이미지 번호
	int		nImageType;
	BYTE	CheakType;          // 시작메뉴 : 범위체크를 rect하는지 배열로 하는지를 판단, 메인메뉴 : 맨처음에 한번만 처리할수 있게
	int		nFieldCount;        //현재 메뉴가 가지고 있는 메뉴 구성요소의 갯수
	int		nTemp;				//전 단계 메뉴 번호를 가지고 있다.
	int		work;				//여러 일을 할 임시 변수
	int		key;				// 지워지지 않는 임시 변수 
	int		key2;				//단축키 역할을 할...
    SFIELD nField[MAX_FIELD];	//메뉴구성 요소 배열
} SMENU;


//##########################################################
//					구조체 및 클레스                      //
//##########################################################

#define		MAX_NAME			21
#define		MAX_MOTHER_SKILL	45
/////////////////////////////////////////////////////////////
//		내집 시스템에서 사용할 디파인 
/////////////////////////////////////////////////////////////

#define		RESIDING		0			// 주거용
#define		GUILD			1			// 길드용
#define		NOTRESIDING		2			// 비주거용

#define		MAX_FAMILY		20
#define		MAX_MATE		20
#define		MAX_GUEST		20
#define		MAX_NAME_		31

#define		MAX_HOUSE		40			// 이미지 최대
	
#define		HOUSE_MAX_TABLE	32			// 테이블 최대


/////////// 집 지붕 열리기 기능에서 사용 /////////////////
#define OPEN_HOUSE	0
#define CLOSE_HOUSE	1



//////////// 건물 클레스 ///////////////
class CHouse
{
protected :

public :

	int		m_Id;
	int		m_Image;
	char	m_Name[MAX_NAME_];
	char	m_HanName[MAX_NAME_];
	int		m_Width;				// 넓이
	int		m_Wall;					// 벽의 재질
	int		m_Roof;					// 지붕 재질
	int		m_LV;					
	int		m_ForResiding;			// 주거용
	int		m_ForGuild;				// 길드용
	int		m_ForNotResiding;		// 비주거용
	int		m_Forforth;				// 예비
	int		m_ForFifth;				// 예비
	int		m_Item;					// 집의 아이템화
	int		m_Price;				// 가격
	int		m_Duration;				// 내구도
	int		m_DurationPerTime;		// 내구도 감소비		(*1000)
	int		m_Custody;				// 보관량
	int		m_Safety;				// 안전도
	int		m_Maxfamily;			// 가족지정 최대량
	int		m_MaxMate;				// 친구 최대량
	int		m_Hiling;				// 시간당 할당량

	char	m_Explain1[30];
	char	m_Explain2[30];
	char	temp[30];

	char	m_Owner[MAX_NAME];		// 그 건물 주인
	char	m_Kind[MAX_NAME_];		// 건물 종류
	int		m_Kind2;
	char	m_HouseKind[30];
	int		m_DurationMax;
	int		m_Key;					// 그 건물 열쇠

	char	m_Family[MAX_FAMILY][MAX_NAME];
	char	m_Mate[MAX_MATE][MAX_NAME];
	char	m_Guest[MAX_GUEST][MAX_NAME];


public:
	CHouse();
	char	*GetDur();
	void	SetExplain(char *, char *);
	void	ReadHouse( char * );
};


class CMyHouse
{
protected : 
public :
	CHouse		data;		//	CHouse class를 가리킴
	DWORD		date;		//	그 건물의 생성 날짜를 기록한다.
	CMyHouse	*next;
	CMyHouse	*prev;

public :
	CMyHouse() { next = NULL; prev = NULL; }
	void FreeAll();
	void AddHouse( CHouse house, int *count );
	void DeleteHouse( int *count );
	int	Repair();
	int GetImageNumber( ) { return data.m_Image; }
};

typedef struct tagCharacterParty
{
	int		m_Id;
	char	m_Name[MAX_NAME];
	int		m_Gender;
	int		m_ImageNo;
	int		m_Lv;
	int		m_Str;
	int		m_Class;
	BYTE	m_Condition;
	int		m_Hp;
	int		m_HpMax;
	
	int		m_Join;			// 조인한 상태냐...? 0->1 : 접속, 1->0 접속 끊음
	int		m_JoinPrev;

	int		m_JoinOk;		// 둘다 파티를 맺는다.
	int		m_Sight;		// 내 시야에 들어 왔다.

	int		x;
	int		y;
	int		xy_on;
	
	LPCHARACTER ch;		// 시야에 들어와 있을때, 
}CharacterParty, *LpCharacterParty;
typedef struct tagNationParty
{	//< CSD-030723
	short int	On;
	int nationcode;
	LPCHARACTER ch;
}NationParty,*LpNationParty;	//> CSD-030723
#define DN_TYPHOID		0	// 장티푸스 (Typhoid Fever)		DN_ ( Disease Name )
#define DN_YUT			1	// 유행성 출혈열(yut)
#define DN_SITOTOXISM	2	// 식중독 (sitotoxism)
#define DN_SPIRITLESS	3	// 무기력증 (Spiritless)
#define DN_COLD			4	// 감기(Cold)
#define	DN_VENEREAL		5	// 성병(Venereal Disease)

#define CC_NORMAL	0
#define CC_POISON	1	// 독에 걸린 상태
#define CC_STONE	2	// 석화
#define CC_DAZE		3
#define CC_FROZEN 4	// 느려진다.
#define CC_CURSE	5	// 활동적인 상태
#define CC_SLOW		6
#define CC_DEATH	7   // 죽어있는 상태.

////////////////// ( character sleep condition ) //////////////////////////
#define CSC_AWAKE		0			// 깨어 있는 상태
#define CSC_SLEEP		1			// 잠자고 있는 상태
#define CSC_REST		2			// 휴식을 취하고 있는 상태

const int  MAX_SKILL_ID  = 200;

typedef struct SkillExp
{
	DWORD skillexp	: 20;
	DWORD rare		: 12;
}t_SkillExp,*LPSKILLEXP;

typedef struct SkillExp2
{
	DWORD	skillexp	:	20	;//스킬 경험치
	DWORD	makecount	:	6	;//만든 갯수
	DWORD	day 		:	3	;//저장된 날짜
	DWORD	month 		:	3	;//저장된 날짜
}t_SkillExp2,*LPSKILLEXP2;

typedef struct SkillExp3//021030 lsw
{
	DWORD	skillType	:	8	;//스킬타입
	DWORD	year		:	12	;//년도
	DWORD	day 		:	12	;//날짜
}t_SkillExp3,*LPSKILLEXP3;

enum eRESIST_TYPE
{
	RT_POISON	=0,
	RT_CURSE	=1,
	RT_HOLY 	=2,
	RT_FIRE		=3,
	RT_ICE		=4,
	RT_ELECT	=5,
	MAX_RESIST_TYPE = 6
};

//-----------------------------------------------------------
//	캐릭터 테이블	-	메뉴에서 사용할 것만
//-----------------------------------------------------------
typedef struct 
{
	__int16 nLevel;
	DWORD	nExp;				
	DWORD	nExpNext;

	char	sCharacterName[MAX_NAME];
	char	nGuild[MAX_NAME];

	int		nCharacterData[MAX_CHARACTERDATA];  // 성별,얼굴,가치관,클래스,스펠,머리색,피부색,옷색,장식색,택틱스남,택틱스여,직업
	int		nCharacterAbility[20];				//STR,CON,DEX,WIS,INT,CHA,MOVP,ENDU,MOR,CONV,LUCK, WSPS,
	int		nAbilityMax[20];					//STR,CON,DEX,WIS,INT,CHA,MOVP,ENDU,MOR,CONV,LUCK, WSPS,
	
	DWORD	nMoney;		// 돈

	int		nCharacterFace;						// 캐릭터 얼굴 표정을 저장 총8가지가 있다
	int		nCharacterHP;						// HP와 MP를 저장하는 변수
	int		nCharacterMP;
	int		nCharacterMAXHP;					// HP와 MP의 최대치
	int		nCharacterMAXMP;
	int		nCharacterSP;				
	int		nCharacterMAXSP;

	BYTE	condition;		// 캐릭터의 상태를 저장하는 변수 0이면 정상이다. 

	int		nWeight;					// 들고 다니는 아이템의 무게
	DWORD	BankMoney;					//	
	int		LoanTotal;
	int		LastLoan;
	DWORD	LastLoan_Time;

//	int		nJobSkill_Lv[31];				// 캐러의 직업스킬 능력치
//	DWORD	nJobSkill[MAX_MOTHER_SKILL];					// 기술 경험치
	t_SkillExp	nJobSkill[MAX_MOTHER_SKILL];					// 기술 경험치//011013 lsw 
//	int		nJopSkillHit[31];				// 성공률

	BYTE	MagicId[MAX_MAGIC];					// 사용할 수 있는 매직
	BYTE	SkillId[MAX_SKILL_ID];					// 할수 있는 스킬
	short int	ItemId[MAX_LEARN_ITEM];			// 만들수 있는 아이템
	int		ItemIdCount;					// 내가 만들수 있는 아이템 수...

	int		curr_magic[MEMORIZE_MAX];		// 현재 메모라이즈 한 마법
	int		curr_magic_ct;					//						   의 총수

	int		memorize_point;					// 현재 메모라이즈 하고 남은 포인트
	int		memorize_point_max;				// 메모라이즈 할 수 있는 포인트 최대치	// 현재 wsps+3
	

//	int		TacticsLv[13];					// 택틱스 레벨
	DWORD	TacticsExp[13];					// 택틱스 경험치
	DWORD	TacticsExpNext[13];

	short naBasicResist[MAX_RESIST_TYPE];
	short naExtentionResist[MAX_RESIST_TYPE];

	int		LvUpPoint;
	WORD 	EachUp[11];

	int		body_r;			//  외모 색
	int		body_g;					
	int		body_b;
	int		cloth_r;
	int		cloth_g;
	int		cloth_b;

	int		mantle_r; // finito 23/06/08 - mantle dye
	int		mantle_g;
	int		mantle_b;

	int		nk3, nk4, nk6;		// 국가 적대감

	int		age;			// 나이

	int		tatics;			// 현재 착용 아이템의 텍틱스
	int		beat;			// 내가 죽인 몬스터의 수
	int		ac;				// 방어력
	int		mov;			// 스테이터스 창에 나타나는 이동력

	int		accessory[4];	// 현재 착용하고 있는 갑옷, 무기 등등...

	BYTE	believe_god;			// 섬기는 신 0: 없음... ~
	int		faith;					// 신앙심
	char	much_supper;			// 신전에서 음식 제공할때 그 갯수
	
	int		dmg_count;		// first	// 3d2 ( 1~2인 주사위를 3번 던진다.)
	int		dmg_max;		// second

//	BYTE	dmg_count_of_inv[8];
//	BYTE	dmg_max_of_inv[8];		// 굳이 잡을 필요없이 타격 계산할때 그때마다 사용한다.

	bool	disease[6];			// 질병에 걸렸는지...
	DWORD	disease_no_time[6];

	DWORD	time_maintain1;				// 저항 효과 지속 시간
	DWORD	time_maintain2;				// 능력 효과 지속 시간
	DWORD	time_maintain3;				// 상태 변환 효과 지속 시간

	CharacterParty	party[6];			// 맺은 인물의 이름
	CharacterParty	relation[3];
	CharacterParty	employment[6];
	NationParty     NationParty;
	int		nAbilityPlusOfPotion[20];	// 먹는 거에 의한 파워업 ( 겹치기는 안 된다. )
	DWORD	dwAbilityPluseTime[20];		// 먹는 거에 의한 파워업일 경우 그 유지 시간.
	int		nAbilityPlusOfWeapon[20];	// 아이템 장착에 의한 파워업 ( 겹치기 가능 )
	// 매직 부분 필요

	BYTE	magic_casting_time[200];		// 단위:초*10

	int		sleep;			// 자고 일어 났거나 새로 시작했을 경우...

	int		class_famous;
	int		job_famous;
	int		nation;

	//char aStepInfo[20];
	int m_nQuestStep;	// 스파이 퀘스트
	int m_nSelectRune;
	int potion_box1;		// 포션박스

	// 020428 YGI acer
	DWORD attack_time;

}SCharacter;

typedef struct tag_client_candidate		// 후보 구조체
{
	char		name[20];
	short int	number;				// 선거 번호

	int			lv;
	int			class_type;
	int			gender;
	int			nation;
	int			guild_code;
	int			face;
	
	int			score;		// 득표수 
}k_client_candidate;

extern CHouse	House[HOUSE_MAX_TABLE];
extern CHouse	*house_sub_residing;
extern CHouse	*house_sub_guild;
extern CHouse	*house_sub_not_res;

extern int		house_res_ct;
extern int		house_guild_ct;
extern int		house_not_res_ct;

extern SCharacter SCharacterData;
extern int g_MouseInMenu;	// 031110 YGI
extern int IsMouseOnMenu();




extern int CheckAndSetPartyOnMySight( LPCHARACTER ch, int type );		// 파티원 시야에 들어왔는지 나간지를 판단.		1: 들어올때, 0: 나갈때
extern inline bool CheckGuildCode( int guild_code );
extern inline bool CheckGuildMaster( LPCHARACTER ch );
extern inline bool IsSubMaster( LPCHARACTER ch );
extern inline bool IsSubMaster( int guild_code, char *name );
#endif
