#if !defined(AFX_MENUDEFINE_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
#define AFX_MENUDEFINE_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_

#define CENTER_X			(-1)
#define RIGHT_X				(-2)
#define GABX_SCREEN			80		// 800*600 으로 바뀔때 640*480과의 차이
#define GABY_SCREEN			60

#define MAX_MONEY			1000000	// 돈아이템이 가질수 있는 아이템 수량;
#define MAX_FIELD			50 //하나의 메뉴에 들어갈수 있는 최대 구성요소
#define MAX_STRING			500 //구성요소가 가지고 있을수 있는 문자열 최대 길이
#define MAX_CHEAKARRAY		3 //배열방식으로 체크할때 사용하는 배열의 최대갯수
#define MAX_SHN				50  //FT_HIDE_NOMAL_  에 사용할 구조체 최대 갯수
#define MAX_SHN_FIELD		15  //FT_HIDE_NOMAL_  에 사용할 구조체가 가질수 있는 최대 요소수
#define MAX_CHARACTERDATA	12 //nCharacterData에 들어갈 최대 캐릭터 데이터수
#define MAX_MENUDATA		40  //한블럭의 그림 최대 개수
#define MAX_FILENAME		100
#define MAX_LEARN_ITEM		1000

#define MEMORIZE_MAX			200		// 현재 메모라이즈한 마법의 최대값...

#define MAX_SYSDAT				1100		// sysmenu.dat 에서 사용하는 이미지 총 갯수, 이 수 다음은 업데이트용이다...
#define MAX_IMAGE2				40
#define MAX_MAGIC				251			// 매직 최대수 //020511 lsw

#define GOD_MENU_IMG_MAX		50
#define TIME_MENU_IMG_MAX		10
#define MEMBOOK_MENU_IMG_MAX	10
#define MAGICBOOK_MENU_IMG_MAX	20
#define MAIL_MENU_IMG_MAX		60
#define START_ETC_IMG_MAX		40			
#define START_ETC2_IMG_MAX		10
#define JOB_ETC_IMG_MAX			20
#define START_MAP_IMG_MAX		35			// 15개 사용중
#define CHAT_MENU_IMG_MAX		10
#define MAIN_ETC_IMG_MAX		70
#define MAIN_ETC3_IMG_MAX		10
#define SMALL_MAP_MAX			50			// 스몰 맵 스프라이트 맥스		
#define SMALL_ETC_MAX			20
#define SKILL_MENU_IMG_MAX		30			// 단축 스킬 맵..
#define PUBLIC_BBS_MENU_IMG_MAX 10			// 공지 사항( 게시판 )
#define HUNTER_MENU_IMG_MAX		40			// 현상범 게시판
#define QUEST_MENU_MAX			10
#define FAME_MENU_MAX			1
#define COLOR_CHANGE_MENU_MAX	10			// 0905 kkh 추가 
#define ITEM_MAKE_IMAGE_MAX		15			// 0908 KKH 추가 
#define NATION_MENU_IMG_MAX		30			// 국가 헌장문 게시판 메뉴
#define START_FIGHT_IMG_MAX		25			// 결투 시작
#define STOP_FIGHT_IMG_MAX		15			// 결투 끝났을때..
#define FIGHTMAP_TIME_IMG_MAX	50			// 시간 표시 할때
#define FIGHTMAP_WIN_IMG_MAX	120			// 이겼을때
#define FIGHTMAP_LOSE_IMG_MAX	90			// 졌을때

#define VOTE_MENU_IMG_MAX		20			// 결투 끝났을때..
#define GUILD_MARK_MAX			512			// guild mark

#define GUILD_ADMIN_MAX			20			// guild 관리소 메인
#define GUILD_SKILL_LIST_MAX	20			// 그랜드 마스터 등록
#define GUILD_ADMIN_ETC_MAX		10			// guild 관리소 메인 추가 이미지
#define HELP_PCX_MAX			20			// tip 버튼 그림
#define BANK_MAX				10			// 은행 한글화 
#define SALVATION_MAX			20			// 구세군 메뉴
#define SALVATION_ANI_MAX		10			// 적십자 캐릭터
#define TEAM_BATTLE_MAX			10			// 팀배틀 
#define SELECT_SERVER_IMG_MAX	10			// 서버 선택 메뉴
#define CAHNGE_MONEY_MAX		10

#define GUILD_WAIT_SPR_MAX		30
#define MERCHANT_SPR_MAX		30	//021007 lsw
#define GUILD_MENU_SPR_MAX		80
#define DUAL_IMG_SPR_MAX		30
#define DUAL_MARK_SPR_MAX		10

#define TUTORIAL_DISPLAY_SPR_MAX 30

//011013 lsw >
#define ITEM_MAKE_RARE_SELECT_SPR_MAX 30
//011013 lsw <

//011014 lsw >
#define TUTORIAL_DISPLAY_SPR_TMP_MAX 30
//011014 lsw <

#define RUNE_SPR_MAX			70
#define RUNE_EFFECT_SPR_MAX		20 
#define SKIN_MENU_MAX			20


#define	GM_2ND_SKILL_SPR_MAX			30	//010708 lsw 2dn GM 인터 페이스
#define NATION_BATTLE_BBS_SPR_MAX		35  // 010730 lsw 전쟁 관리소
#define	NATION_INFO_SPR_MAX				35	// 010730 lsw
#define	NATION_BUDAE_SPR_MAX			35	// 010730 lsw	
#define	NATION_LADDER_SPR_MAX			35	// 010730 lsw	
#define	NATION_VOTE_SPR_MAX				35	// 010730 lsw
#define	NATION_JOIN_SPR_MAX				35	// 010730 lsw
#define	NATION_NOW_SPR_MAX				45	// 010730 lsw
#define	NATION_GUEST_SPR_MAX			35	// 010730 lsw
#define	NATION_GOTO_WAR_MAP_SPR_MAX		35	// 010730 lsw
#define	NATION_MSGBOX_SPR_MAX			35	// 010730 lsw
#define	NATION_DONEBOX_SPR_MAX			35	// 010730 lsw
#define	NATION_TODAY_NOTICE_SPR_MAX		35	// 010730 lsw
#define	NATION_WAR_RESULT_SPR_MAX		35  // 010910 lsw
#define	NATION_WAR_START_SPR_MAX		35	// 010925 lsw

#define	NEW_CHAT_SPR_MAX		30 // 010928 lsw 3의 배수 일것
#define MAX_MESSAGE				200
#define MAX_SKILL_KIND			45

#define GAMBLE_SPR_MAX			15
#define EXP_GAUGE_MAX			15

#define GAMBLE_EFFECT_MAX				20
#define	ABILITY_REDISTRIBUTION_SPR_MAX	5	// 011201 LTS	// LTS MENU
#define IM_GUILD_MEMBER_LIST			20	// IMAGE_MAX
#define LOCALWAR_JOIN_SPR_MAX			40	// 011201 LTS
#define IM_FRIEND_MENU					20
#define	LOCALWAR_OK_SPR_MAX				50	// LTS NEW LOCALWAR
#define EVENT_LOCALWAR_OK_SPR_MAX		20	// 020115 LTS
#define	EVENT_LOCALWAR_MOVE_SPR_MAX		20

#define WAR_START_SPR_MAX				20
#define WAR_MOVE_SPR_MAX				20
#define WAR_STATUS_SPR_MAX				30

#define MONSTERWAR_INFO_SPR_MAX			10	// BBD 040311

#define IM_POTAL_MENU					30
#define IM_PARTY_BUTTON					10
#define IM_GUILD_ITEM					20
#define IM_GUILD_CHANGE_INFO			20
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//메뉴 번호 설정


enum MenuIndex
{
	MN_MAKECHARACTER_BASIC          = 0,    //케릭터 만들기 메뉴에서 가장 바닥에 깔리는 부분
	MN_MAINSTART_BASIC              = 1,     //메인 스타트 메뉴의 시작부분
	MN_MAINSTART_START              = 2,    //메인 스타트 메뉴에서 START버튼을 누르면 나오는 메뉴
	MN_MAKECHARACTER_CHEAK1         = 3,     //캐릭터 만들기 메뉴에서 설정확인창 1번
	MN_MAKECHARACTER_CHEAK3         = 4,     //캐릭터 만들기 메뉴에서 설정확인창 3번
	MN_MAKECHARACTER_GENDER         = 5,     //캐릭터 만들기 메뉴에서 성별설정메뉴
	MN_MAKECHARACTER_NAME           = 6,     //캐릭터 만들기 메뉴에서 이름설정메뉴
	MN_MAKECHARACTER_FACE           = 7,     //캐릭터 만들기 메뉴에서 얼굴설정메뉴
	MN_MAKECHARACTER_ARIGEMENT      = 8,     //캐릭터 만들기 메뉴에서 가치관설정메뉴
	MN_MAKECHARACTER_CLASS          = 9,     //캐릭터 만들기 메뉴에서 클래스설정메뉴
	MN_MAKECHARACTER_SPELL          = 10,    //캐릭터 만들기 메뉴에서 마법설정메뉴
	MN_MAKECHARACTER_COLOR          = 11,    //캐릭터 만들기 메뉴에서 색깔설정메뉴
	MN_MAKECHARACTER_TACTICS_MAN    = 12,    //캐릭터 만들기 메뉴에서 남자 전투기술설정메뉴
	MN_MAKECHARACTER_TACTICS_WOMAN  = 13,    //캐릭터 만들기 메뉴에서 여자 전투기술설정메뉴
	MN_MAKECHARACTER_JOB            = 14,    //캐릭터 만들기 메뉴에서 직업설정메뉴
	MN_MAKECHARACTER_ABILITY        = 15,    //캐릭터 만들기 메뉴에서 능력치 설정메뉴
	MN_MAININTERFACE                = 16,    //메인 인터페이스 메뉴
	MN_STATUS						= 17,    //스테이터스 메뉴
	MN_ABILITY                      = 18,    //어빌리티 메뉴
	MN_NATION_MAIN_VA				= 19,		// 국가 메뉴
	MN_NATION_MAIN_ZY				= 20,
	MN_NATION_MAIN_ILL				= 21,
	MN_VISA_MENU					= 22,		// 비자 메뉴
	MN_SCALE						= 23,		// 균형추 메뉴
	MN_GUILD_ITEM_OR_MEMBER_LIST	= 24,
	MN_ITEM                         = 25,    //아이템 메뉴
	MN_ARIGEMENT                    = 26,    //스테이터스나 어빌리티 메뉴에서 문장을 선택하면 나오는 메뉴
	MN_RUNE							= 27,	
	MN_SKIN							= 28,
	MN_GAMBLE						= 29,
	MN_GUILD_MAIN					= 30,		// 길드 메뉴
	MN_GUILD_EXPLAIN				= 31,		// 길드 등록 설명 메뉴
	MN_GUILD_REGIST_IMSI			= 32,
	MN_GUILD_REGIST_DEGREE			= 33,		// 길드 직첵 메뉴
	MN_GUILD_REGIST					= 34,		// 정식 길드 신청시 정보 출력 메뉴
	MN_GUILD_INTRODUCTION			= 35,
	MN_GUILD_CHANGE_INFO			= 36,		// 길드 정보 변경 메뉴
	MN_FRIEND_MENU					= 37,		// 친구 시스템
	MN_FRIEND_REGIST				= 38,
	MN_POTAL_MENU					= 39,
	MN_DEFAULT_MESSAGEBOX			= 40,		// default message box
	MN_VIEW_TIP						= 41,		// tip 보기 버튼
	MN_SALVATION2					= 42,
	MN_CHANGE_MONEY					= 43,
	MN_SELECT_SERVER				= 44,		// 서버 선택 메뉴
	MN_DRINK_RUMOR_VIEW				= 45,
	MN_ARENA_LOBBY					= 46,
	MN_MERCHANT_MAIN				= 47,		// 상인 메뉴 메인
	MN_MERCHANT_BBS					= 48,		// 공지 메뉴
	MN_SALVATION					= 49,		// 구세군 메뉴
	MN_FACE                         = 50,    //메인 인터페이스 메뉴에서 얼굴 표정 선택 메뉴 뛰우기
	MN_BANK_DEPOSIT					= 51,    //은행 메뉴에서 예금
	MN_BANK_DEFRAYAL                = 52,    //은행 메뉴에서 출금
	MN_BANK_CUSTODY                 = 53,    //은행 메뉴에서 보관
	MN_BANK_MAIN	                = 54,    //은행 메뉴에서 찾기
	MN_BANK_LOAN                    = 55,    //은행 메뉴에서 대출
	MN_BANK_REPAYMENT               = 56,    //은행 메뉴에서 상환
	MN_BANK_AUCTION                 = 57,    //은행 메뉴에서 경매
	MN_POTAL_SELECT					= 59,		// 맵 이동시 마을 설명 메뉴
	MN_PARTY						= 60,    //메인 인터페이스 메뉴중 파티원 메뉴
	MN_GUILD_ITEM_GET				= 61,
	MN_GUILD_ITEM_PUT				= 62,
	MN_GUILD_MEMBER_LIST			= 63,
	MN_OKCANCEL_FILE_TEXT			= 64,
	MN_HTML_MESSAGEBOX				= 65,		// html 형식으로 만든 파일을 출력해주는 메뉴 CallHtmlMessageBox() 이용
	MN_NATION_ITEM					= 66,
	MN_REVIVAL						= 67,
	MN_GUILD_HOUSE					= 68,		// 엠포리아 구입 매뉴
	MN_SMALL_TIPS					= 69,		// 도움말 메뉴
	MN_LOGO_START					= 70,		//회사 로고
	MN_LODING						= 71,		// 메인 화면으로 넘어가기전에 로딩 화면
	MN_LOGIN						= 72,		// 새로 만들 것인가 로드할 것인가...
	MN_LOGON						= 73,		// 로드할때, 뜨는 메뉴, 여기서 직접 게임으로 들어갈 수 있음.
	MN_SELECT						= 74,		//접속 모드
	MN_LOCATION						= 75,		//시작 장소 선택
	MN_BANK_AUCTION_SALE			= 76,		// 경매 메뉴중 팔기 메뉴
	MN_PARTY_FACE1					= 77,		// 파티원들 조그만 얼굴 창
	MN_PARTY_FACE2					= 78,
	MN_PARTY_FACE3					= 79,
	MN_PARTY_FACE4					= 80,
	MN_PARTY_FACE5					= 81,
	MN_PARTY_FACE6					= 82,
	MN_GOD_EDELBLHOY				= 83,		// 에델 브로이 신전
	MN_GOD_TEFFERY					= 84,		// 테페리 신전
	MN_GOD_LETTY					= 85,		// 레띠 신전
	MN_EFFECT_TOOL					= 86,		// effect tool 메뉴 
	MN_GOD_OREM						= 87,		// 오렘 신전
	MN_MESSAGEBOX_BIG				= 88,		// 독립적으로 호출할 수 있는 메시지 박스
	MN_MESSAGEBOX_SMALL				= 89,		
	MN_YESNO						= 90,		//YES-NO MENU
	MN_DISTANCE_TABLE				= 91,		//	이정표
	MN_STORE_SIGN					= 92,		//	간판(?)
	MN_MAGIC_GUILD					= 93,		// 마법사 길드
	MN_ACADEMY						= 94,		// 아카데미
	MN_SKILLGUILD_KNOWLAGE			= 95,		// Learn Knowlage
	MN_AWAKE						= 96,		// 휴식에서 , 잠자기에서 깨어나는 메뉴... 버튼..
	MN_TOWNHALL_BBS1				= 97,		// ok, cancel있는 게시판
	MN_TOWNHALL_BBS2				= 98,		// join, cancel 있는 게시판
	MN_TOWNHALL_BBS3				= 99,		// write, exit 있는 게시판
	MN_BLACKSMITH					= 100,		// 직업 길드 메뉴
	MN_FARM							= 101,
	MN_RESTAURANT					= 102,
	MN_HERBSHOP						= 103,
	MN_SHAMBLES						= 104,
	MN_CANDLE						= 105,
	MN_SLOPSHOP						= 106,
	MN_ALCHEMY						= 107,
	MN_WOODWORKING					= 108,
	MN_BOWCRAFT						= 109,
	MN_SHIPYARD						= 110,
	MN_SKILLGUILD_BUY				= 111,
	MN_SKILLGUILD_SELL				= 112,
	MN_SKILLGUILD_SKILL				= 113,
	MN_SKILLGUILD_ITEM				= 114,
	MN_SKILLGUILD_REPAIR			= 115,
	MN_ITEM_EXPLAN					= 116,
	MN_TOWNHALL                     = 117,    //TOWNHALL인터페이스
	MN_TOWNHALL_PUBLICNOTICE        = 118,    //TOWNHALL메뉴에서 PUBLICNOTICE메뉴
	MN_TOWNHALL_BUILDING            = 119,    //TOWNHALL메뉴에서 BUILDING메뉴
	MN_TOWNHALL_COMMERCE1           = 120,    //TOWNHALL메뉴에서 COMMERCE1메뉴
	MN_TOWNHALL_COMMERCE2           = 121,    //TOWNHALL메뉴에서 COMMERCE2메뉴
	MN_TOWNHALL_COMMERCE3           = 122,    //TOWNHALL메뉴에서 COMMERCE3메뉴       //설명창
	MN_TOWNHALL_HOUSEREPAIR         = 123,    //TOWNHALL메뉴에서 HOUSE REPAIR메뉴
	MN_TOWNHALL_GUILD               = 124,    //TOWNHALL메뉴에서 Organize Guild메뉴
	MN_TOWNHALL_FORUM               = 125,    //TOWNHALL메뉴에서 FORUM메뉴
	MN_TOWNHALL_EVENT               = 126,    //TOWNHALL메뉴에서 EVENT메뉴
	MN_TOWNHALL_RANK1               = 127,    //TOWNHALL메뉴에서 RANK1메뉴
	MN_TOWNHALL_RANK2               = 128,    //TOWNHALL메뉴에서 RANK2메뉴
	MN_TOWNHALL_NOTICE4             = 129,    //TOWNHALL메뉴에서 NOTICE4메뉴
	MN_TOWNHALL_NOTICE3             = 130,    //TOWNHALL메뉴에서 NOTICE3메뉴
	MN_TOWNHALL_NOTICE2             = 131,    //TOWNHALL메뉴에서 NOTICE2메뉴
	MN_TOWNHALL_NOTICE1             = 132,    //TOWNHALL메뉴에서 NOTICE1메뉴
	MN_TOWNHALL_COMMERCE4           = 133,    //TOWNHALL메뉴에서 COMMERCE4메뉴   //설명창
	MN_TOWNHALL_CONSTRUCTION        = 134,    //TOWNHALL메뉴에서 CONSTRUCTION메뉴
	MN_GUILD_HOUSE_INFO				= 135,
	MN_SCENARIO_TRAP_INFO			= 136,		// 시나리오
	MN_SCENARIO_TRAP_KEY			= 137,		// 020701 YGI
	
	MN_WAR_MOVE						= 138,
	MN_WAR_STATUS					= 139,

	MN_CHATTING						= 140,		// 채팅창		
	MN_GOD1                         = 141,     //신전메뉴1
	MN_GOD2                         = 142,     //신전메뉴2
	MN_GOD3                         = 143,     //신전메뉴3
	MN_LADDER_RANK					= 144,		// 길드전 ladder 점수 랭킹 게시판
	MN_MAGICBOOK                    = 145,     //마법책
	MN_FUNCTION                     = 146,     //행사 접수실
	MN_SYSTEM                       = 147,     //시스템 메뉴
	MN_SYSTEM_OPTION                = 148,     //시스템 메뉴에서 옵션메뉴
	MN_DEFAULT_MESSAGEBOX2			= 149,
	
	MN_DEL_CHAR_COMFORM				= 150,
	MN_SELECT_TUTORIAL_LOGIN		= 151,
	MN_WAR_START					= 152,

	MN_SYSTEM_LOGOUT                = 153,
	MN_DRINK_MAIN					= 154,		// 주점
	MN_DRINK_RUMOR					= 155,		//
	MN_DRINK_BUY					= 156,		//
	MN_SHOP_MAIN					= 157,		// 잡화상
	MN_SHOP_BUY						= 158,		//
	MN_SHOP_SELL					= 159,		//
	MN_HEAL_MAIN					= 160,		// 병원
	MN_HEAL_HEALING					= 161,		// 치료
	MN_HEAL_PREVENTING				= 162,		// 예방
	MN_HEAL_LEARN					= 163,		// 스킬 배움
	MN_SKILL_LEARN					= 164,		// 스킬 배움
	MN_INN_MAIN						= 165,
	MN_INN_REST						= 166,
	MN_INN_SLEEP					= 167,
	MN_SELECT_CHARACTER				= 168,		// 캐러 선택창
	MN_TACTICS						= 169,		// 택틱스 남
	MN_LEVELUP						= 170,		// 단지 멥에서 클릭 했을 때 주인공이 움직이는 것을 막기 위해
	MN_MAKECHARACTER_AGE			= 171,		// 캐릭터 만들기에서 나이 설정
	MN_MAKECHARACTER_SPELL2			= 172,		// wizard나 priest에서 spell 결정할 때...
	MN_INFO							= 173,
	MN_CREDIT						= 174,
	MN_OKCANCEL_MESSAGEBOX			= 175,		// OK_CANCEL 달린 메시지 창
	MN_OK_MESSAGEBOX				= 176,		// OK만 달린 메시지 창
	MN_ITEM_MESSAGEBOX				= 177,		// 아이템 설명 창
	MN_DIVIDE_ITEM					= 179,		// 아이템을 값을 나누는 창
	MN_MAGIC_BOOK					= 180,		// 매직 책
	MN_ITEM_MAKE					= 181,		// 아이템 만들기용 작업대
	MN_ITEM_MAKE_SELECT				= 182,		// 만들수 있는 아이템 출력
	MN_ITEM_MAKE_OK					= 183,		// 아이템 만들기에 성공했을 경우 
	MN_CHARACTER_ITEM				= 184,		// 죽은 캐릭터 뒤지거나 훔쳐보기 기술을 사용했을 때 뜨는 인벤창
	MN_ITEM_SELECT					= 185,		// 스킬 선택시 사용 아이템 보여주기 선택 창 
	MN_LEARN_MAGIC_BOOK				= 186,		// 매직 배우기...
	MN_MEM_BOOK						= 187,		// 메모라이즈
	MN_SMALL_MAP					= 188,		// 뷰 스몰 멥
	MN_EXCHANGE_ITEM				= 189,		// 아이템 교환
	MN_DUAL_MAIN					= 190,		// 듀얼 인터페이스
	MN_INIT_ABILITY					= 191,
	MN_GOD_INTRUDUCTION				= 192,		// 신전 설명 메뉴
	MN_GOD_MEETING					= 193,		// 신전 예배 안내
	MN_GOD_REGIST1					= 194,		// 등록 설정 wizard
	MN_GOD_PREACH					= 195,		// 전도한 사람 등록 메뉴
	MN_GOD_REGIST_ERROR				= 196,		// 신전 등록시 예외상황 ( 이미 등록되어 있다. 다른 신전에 등록되어 있다..
	MN_GOD_REGIST_MESSAGE			= 197,		// 등록 취소 등의 메시지 처리
	MN_GOD_PREACH_ERROR1			= 198,
	MN_GOD_PREACH_RESULT			= 199,
	MN_GOD_MEETING_MAIN				= 200,		//	시간이 되면 뜨는 예배 메뉴
	MN_GOD_SUPPER					= 201,		//	신전에서 음식 제공 메뉴
	MN_TIME_DISPLAY_MENU			= 202,		// 시간 표시 메뉴
	MN_MAIL_MENU					= 203,
	MN_MAIL_WRITE					= 204,
	MN_MAIL_READ					= 205,
	MN_RESIST_FRIEND				= 207,		// 내 친구 등록 메뉴
	MN_MAIN_EXPLAIN					= 208,		// 메인 메뉴 설명 창
	MN_QUICK_MAGIC_SCROLL			= 209,		// 퀵 매직 배열에서 5개 이상 넘어갈경우 스크롤 처리
	MN_QUICK_MAGIC_PLUS				= 210,		// 저장 매직 배열 메뉴 
	MN_TIME_DISPLAY_ON_OFF			= 211,		// 메뉴 디스플레이 메뉴 활성화 버튼을 가지고 있는 메뉴
	MN_START_MAP					= 212,		// 처음 시작 메뉴 선택
	MN_PUBLIC_BBS					= 213,		// 공지사항 메뉴
	MN_QUICK_SKILL					= 214,		// 퀵스킬 창...
	MN_HUNTER_MAIN					= 215,		// 현상범 게시판 메인
	MN_HUNTER_REGIST				= 216,		// 현상범 등록 메뉴
	MN_HUNTER_LIST					= 217,		// 현상범 리스트 메뉴
	MN_QUEST						= 218,		// 퀘스트 메뉴
	MN_FAME							= 219,		// 명성 메뉴 
	MN_QUEST_INDEX					= 220,		// 퀘스트 인덱스 
	MN_RELATION						= 221,		// 사제 메뉴
	MN_MAN_TO_MAN					= 222,		// 1:1 결투 시스템
	MN_RESET_JOB					= 223,		// 직업 재 선택
	MN_RESET_JOB_EXPLAIN			= 224,		// 직업 재 선택 설명 글
	MN_CHARACTER_COLOR_CHANGE		= 225,		// 0905 kkh 추가 
	MN_SELECT_NATION				= 226,		// 나라 선택
	MN_NATION_EXPLAIN				= 227,		// 나라 설명
	MN_NATION_BBS					= 228,		// 나라 헌장문 게시판
	MN_FIGHT_MAP_SIGN				= 229,		// 배틀존 전광판 
	MN_SCENARIO						= 230,		// 시나리오 출력 메뉴
	MN_VOTE_REGIST					= 231,		// 후보 등록
	MN_VOTE_SUPPORT					= 232,		// 투표
	MN_VOTE_SIGN					= 233,		// 득표
	MN_GUILD_ADMIN					= 234,		// 길드 관리소 메인
	MN_GM_LIST						= 235,		// 그랜드 마스터 등록
	MN_GM_ITEM_EXPLAIN				= 236,		// 그랜드 마스터 조건 아이템 설명
	MN_DISPLAY_SKILL_EXP			= 238,		// 스킬 경험치 보여주기 메뉴
	MN_DISPLAY_TAC_EXP				= 239,
	MN_DISPLAY_MAGIC_EXP			= 240,
	MN_GM_2ND_SKILL					= 241,
	MN_NATION_BATTLE_BBS			= 242,
	MN_NATION_INFO					= 243,
	MN_NATION_VOTE					= 244,
	MN_NATION_BUDAE					= 245,
	MN_NATION_NOW					= 246,
	MN_NATION_LADDER				= 247,
	MN_NATION_JOIN					= 248,
	MN_NATION_GUEST					= 249,
	MN_NATION_GOTO_WAR_MAP			= 250,
	MN_NATION_MSGBOX				= 251,
	MN_NATION_DONEBOX				= 252,
	MN_NATION_TODAY_NOTICE			= 253,
	MN_NATION_WAR_RESULT			= 254,
	MN_NATION_WAR_START				= 255,
//< soto-030425
	MN_LUCKY_MENU					= 256,
	MN_LOTTO_MENU					= 257,
	MN_WINNER_MENU					= 258,//당첨자 확인.
//> soto-030425


	MN_MERCHANT_FIND				,//아이템 검색-->MN_MERCHANT_BUY로 연결(찾기의 결과물)
	MN_MERCHANT_BUY					,//아이템 구매
	MN_MERCHANT_SELL_INFO			,//아이템 판매(상인이 자기정보 보는것)
	MN_MERCHANT_REGISTER			,//아이템 등록
	MN_MERCHANT_RESULT_TAKE			,//결과물 찾기
	MN_MERCHANT_DIRECT_EXCHAGE_LIST	,//상인과 직접 거래 리스트
	NM_SCRIPT_TIMMER,	// 021020	kyo		스크립트용 타이머랑 카운터
	NM_SCRIPT_COUNTER,	// 021020	kyo
	
	NM_MERCHANT_QUEST,	// 021020	kyo


	
	MN_ITEM_MAKE_RARE_SELECT		,
	MN_COMBAT_SKILL_POINT			,
	MN_COMBAT_SKILL_DOING			,
	MN_ABILITY_REDISTRIBUTION		,
	MN_LOCALWAR_JOIN				,
	MN_LOCALWAR_OK					,
	//MN_EVENT_LOCALWAR_OK			,
	//MN_EVENT_LOCALWAR_MOVE			,
	MN_WISPER_LIST					,
	MN_GHOST_TO_ALIVE				,
	MN_SKILL_MASTER_LIST			,
	MN_SKILL_MASTER_MAKE			,//기성 아이템 멀티레어 만들기
	MN_TUTORIAL_DISPLAY				= 280,
	MN_EXCHANGE_BOND_MONEY			= 281,//고정입니다.

	MN_OBSERVER_MODE,
	MN_BETTING,
	MN_EVENT_LOCALWAR_OK			=293, // 03813 kyo
	MN_EVENT_LOCALWAR_MOVE			=294,
	MN_MERCHANT_BACKUP_MAIN			=295,	// BBD 040303
	MN_MAINSTART_BACK, 
	MN_MANTLE_COLOR_CHANGE,
	MN_ITEM_MALL,
	MAX_MENU//최대 메뉴 갯수//이 둘은 enum의 맨 마지막에 위치해야 합니다.
};
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//FT_HIDE_NOMAL_에 사용할 구조체 번호 설정
#define HN_MAKECHARACTER_GENDER_RECT          0     //캐릭터 만들기 메뉴에서 성별 설정때 나오는 선택 사각형 
#define HN_MAKECHARACTER_GENDER_TEXT          20    //캐릭터 만들기 메뉴에서 성별 설정때 나오는 선택 글자

#define HN_MAKECHARACTER_FACE                 1     

#define HN_MAKECHARACTER_ARIGEMENT_TEXT       21    //캐릭터 만들기 메뉴에서 가치관 설정때 나오는 글자
#define HN_MAKECHARACTER_ARIGEMENT_RECT       2     //캐릭터 만들기 메뉴에서 가치관 설정때 나오는 사각형

#define HN_MAKECHARACTER_CLASS_TEXT           22    //캐릭터 만들기 메뉴에서 클래스 설정대 나오는 글자
#define HN_MAKECHARACTER_CLASS_RECT           3     //캐릭터 만들기 메뉴에서 클래스 설정때 나오는 사가형

#define HN_MAKECHARACTER_SPELL_RECT           4     //캐릭터 만들기 메뉴에서 SPELL 설정때 나오는 사각형
#define HN_MAKECHARACTER_SPELL_TEXT           23    //캐릭터 만들기 메뉴에서 SPELL 설정때 나오는 글자

#define HN_MAKECHARACTER_COLOR_SELECT           5	  //머리색깔과 장식색깔의 점찍기
//#define HN_MAKECHARACTER_COLOR_HAIR           5     //캐릭터 만들기 메뉴에서 선택된 머리 색깔의 점찍기
//#define HN_MAKECHARACTER_COLOR_SKIN           6     //캐릭터 만들기 메뉴에서 선택된 피부 색깔의 점찍기
//#define HN_MAKECHARACTER_COLOR_CLOTHES        7     //캐릭터 만들기 메뉴에서 선택된 옷 색깔의 점찍기
//#define HN_MAKECHARACTER_COLOR_ORNAMENT       8     //캐릭터 만들기 메뉴에서 선택된 장식 색깔의 점찍기

#define HN_MAKECHARACTER_TACTICS_MAN_RECT     9    //캐릭터 만들기 메뉴에서 택틱스 설정때 나오는 남자 사각형
#define HN_MAKECHARACTER_TACTICS_MAN_TEXT     24    //캐릭터 만들기 메뉴에서 택틱스 설정때 나오는 남자 글자

#define HN_MAKECHARACTER_TACTICS_WOMAN_RECT   10    //캐릭터 만들기 메뉴에서 택틱스 설정때 나오는 여자 사각형
#define HN_MAKECHARACTER_TACTICS_WOMAN_TEXT   25    //캐릭터 만들기 메뉴에서 택틱스 설정때 나오는 여자 글자

#define HN_MAKECHARACTER_JOB_RECT             11    //캐릭터 만들기 메뉴에서 직업 설정때 나오는 사각형
#define HN_MAKECHARACTER_JOB_TEXT             26    //캐릭터 만들기 메뉴에서 직업 설정때 나오는 글자

#define HN_MAKECHARACTER_ABILITY              12    //캐릭터 만들기 메뉴에서 능력치 설정때 나오는 능력치글자

#define HN_TEMPLE_INTRODUTION_TEXT            13    //설명 생략
#define HN_TEMPLE_OPERATION_TEXT              15    //
#define HN_TEMPLE_TEMPLE1_TEXT                17    //
#define HN_TEMPLE_TEMPLE2_TEXT                19    //

#define HN_GUILD_INTRODUTION_TEXT             27    //설명 생략
#define HN_GUILD_OPERATION_TEXT               28    //
#define HN_GUILD_GUILD1_TEXT                  29    //
#define HN_GUILD_GUILD2_TEXT                  30    //
#define HN_INVENTORY                          31    //인벤토리창의 스크롤을 위한 필드 단지 .nSHideNomalCount  .nSHideNomalPlus   .nSHideNomalStart만을 위해 사용한다
#define HN_PARTY_SELECT						  32  // 메인 인터페이스, 파티 메뉴에서 선택 버튼 라이트

#define HN_SKILL_MAIN_TEXT					33		// 스킬 메뉴중 메인 화면 글자

#define HN_TOWNHALL_TEXT                      34    //타운홀 메뉴에서 메뉴 글자
#define HN_TOWNHALL_CONSTRUCTION_TEXT         35    //타운홀 메뉴에서 construction메뉴에서 글자

#define HN_SYSTEM_TEXT                        36    //메인인터페이스의 시스템 메뉴에서  글자
#define HN_SYSTEM_HELP_TEXT                   37    //메인인터페이스의 시스템의 헬프에서 글자

#define HN_DRINK_MAIN_TEXT					38		// 독립메뉴 주점에서 선택 문자
#define HN_SHOP_MAIN_TEXT					39		// 잡화상
#define HN_HEAL_MAIN_TEXT					40		// 병원
#define HN_INN_MAIN_TEXT					41		// 여관
#define HN_INN_REST_TEXT					42		//

#define HN_HEAL_PREVENTING					43		// 병원에서 질병 예방

/////////////////////////////////////////////////////////////////////////////////
//특별히 일어나는 일 설정 예를 들면 동시에 여러 메뉴를 할성화 시키거나 어떤 조건을 체크해 메뉴를 열거나
enum SpecailWillDoIndex//FT_xxx_SPECIAL_WILLDO 류에서 클릭시 쓰임
{
	SWD_NONE							= 0,
	SWD_ENTER_MAKECHARACTER				,      //매인스타트에서 캐릭터 만들기 모드로 들어간다
	SWD_MAKECHARACTER_ENTERNAME			,     //성별메뉴에서 이름설정 메뉴로 들어갈때 
	SWD_MAKECHARACTER_SPELLORCOLOR		,      //클래스 메뉴에서 선택된 클래스를 판단해 SPELL이나 칼라로 갈때
	SWD_MAKECHARACTER_TACTICS			,      //성별을 판단해  TACTICS목록을 보여준다
	SWD_MAKECHARACTER_ABILITY			,      //JOB에서 ABILITY메뉴로 가기
	SWD_FOCUS_WILLDO					,
	SWD_CALLMENU						,
	SWD_MN_MERCHANT_BBS_OK				,		// 공지 날리기
	SWD_CALLSMALLMENU					,
	SWD_MAININTERFACE_STATUS			,      //메인 인터페이스에서 스테이터스 메뉴 뛰우기
	SWD_MAININTERFACE_ABILITYTOSTATUS	,     //어빌리티 메뉴에서 스테이터스 메뉴를 뛰울때
	SWD_MAININTERFACE_MAGIC				,     //메인 인터페이스에서 매직 메뉴 뛰우기(프리스트나 위자드 메뉴에1로 뜬다)
	SWD_MAININTERFACE_ITEM				,     //메인 인터페이스에서 아이템 메뉴 뛰우기
	SWD_MAININTERFACE_ABILITY			,     //메인 인터페이스의 스테이터스에서 어빌리티 메뉴 뛰우기    
	SWD_MAININTERFACE_ARIGEMENT			,     //스테이터스 메뉴나 어빌리티 메뉴에서 가치관 메뉴 뛰우기
	SWD_MAININTERFACE_PARTY				,     //메인 인터페이스에서 파티원 메뉴 띄우기
	SWD_ARIGEMENT_STATUS				,     //아무일도 안하고 그냥 자신만 닫는다
	SWD_MAININTERFACE_FACE				,     //메인 인터페이스 에서 얼굴 표정 선택 창 뛰우기
	SWD_FACE_CHOOSE						,     //표정 선택후 나가기
	SWD_FACE_AUTOCLOSE					,     //얼굴표정 선택 메뉴가 자동으로 사라지게 한다
	SWD_UNMOUSEFOCUS					,     //마우스 포커스를 풀어 모든 메뉴를 사용가능하게 한다
	SWD_LMENUCLOSE						,     //메인 인터페이스에서 사용하는 메뉴중 왼쪽에 뜨는 메뉴를 움직이면서 닫을대 사용
	SWD_RMENUCLOSE						,    //메인 인터페이스에서 사용하는 메뉴중 오른쪽에 드는 메뉴를 움직이면서 닫을때 사용
	SWD_YESNO_YES						,		// When select "YES" at YES-NO MENU
	SWD_YESNO_NO						,		// When select "NO" at YES-NO MENU
	SWD_CALL_OTHER						,		// 자기와는 상관없는 곳의 메뉴 호출 하기
	SWD_IDPASS							,		//아이디 패스워드 에디트 박스 만들기
	SWD_TURN_BUTTON						,		//메인 인터페이스에서 번갈아 출력하는 버튼
	SWD_IDEDITBOX						,				// 아이디 출력
	SWD_PASSEDITBOX						,				// 패스워드 출력
	SWD_MAKECHARACTER_GENDERBACK		,		// 성별 결정 메뉴에서 스타트 메뉴로 돌아가기
	SWD_SMALLMENU_END					,		// 독립메뉴 끝낼때		// 단축키
	SWD_BANK_OK							,
	SWD_BANK_CANCEL						,		// 은행 메뉴에서 ok, cancel 버튼
	SWD_BANK_ITEM						,
	SWD_BANK_ITEM_END					,
	SWD_TWO_MENU_OPEN					,		// 두개의 메뉴를 동시에 띄울때 nWillDo와 nSHideNomalNumber를 사용한다. 현재는 경매 사기 메뉴에서 팔기 메뉴로 바꿀때만 사용
	SWD_SIMPLE_CLOSE					,		// 메인 인터페이스와 띄우려는 것만 놔두고 모든 메뉴를 닫을 때.. (두개 이상 띄워져 있을 경우 사용)
	SWD_SMALL_MENU						,
	SWD_MENU_OPEN						,		// 메인 인터페이스에서 그냥 띄우기
	SWD_ITEM_SELLBUY_OK					,		// 사고 팔기 메뉴에서 OK 버튼 눌렀을 때
	SWD_JOBSKILL_SCROLL_UP				,		//	사고 팔기 메뉴에서의 스크롤 버튼
	SWD_JOBSKILL_SCROLL_DOWN			,		//				"
	SWD_ITEM_SELLBUY_CANCEL				,		// 길드에서 사고 팔기 메뉴
	SWD_LOGON							,		// 스타트 메뉴에서 로그온 할때 아이디 패스워드 체크
	SWD_LOGON_OK						,		
	SWD_TOWN_CONSTRUCTION				,
	SWD_EDIT_BOX						,		// 메인에서 에디트 박스 띄우기
	SWD_MY_HOME							,		// MN_MY_HOME으로 돌아가기
	SWD_INFO_FLAG_SET					,		// 클릭되면 인터페이스 정보 플레그를 셋팅 해줄때
	SWD_NORMAL_CHECK_PUT				,		// 클릭후 다른 필드의 타입을 바꾼다.
	SWD_QUIT							,		// 게임 끝낼때
	SWD_SELL_BUY						,
	SWD_LEARN							,		// skill 배우기
	SWD_CHARACTER_SELECT				,		// 캐릭터 선택하기
	SWD_CHARACTER_DEL_ADD				,
	SWD_STATUS_TACTICS					,		// 스테이터스에서 택틱스로
	SWD_TACTICS_STATUS					,		// 택틱스에서 스테이터스로
	SWD_PARTY_BUTTON					,
	SWD_BACK_SKILL						,
	SWD_SKILLGUILD_LEARN				,		// 스킬 배우기, 아이템 배우기
	SWD_LEARNSKILL_OTHER				,		// 
	SWD_TOWNHALL_TITLE					,		// 각종 게시판 항목 찍기 
	SWD_TOWNGALL_TITLE_READ				,		// 게시판 제목으로 내용 출력 창으로 들어가기
	SWD_OKCANCEL_OK						,		
	SWD_OKCANCEL_CANCEL					,
	SWD_BBS_READ						,		// 마을 회관에서 공지나 등등의 글을 읽을 때
	SWD_SUBMENU_BACK					,		// 자식메뉴에서 어미 메뉴로 돌아오기 nTemp를 이용
	SWD_EVENT_JOIN						,		// 이벤트 비비에서 조인하기 
	SWD_PARTY_RELEASE					,		// 파티원 해제
	SWD_SELECT_HOUSEKIND				,		// 집고르기 선택
	SWD_BUY_HOUSE						,		// 집 사기기
	SWD_WILLDO							,		// 다음 메뉴를 띄우기 전 셋팅을 한다. 
	SWD_MOVE_OPEN_CLOSE					,		// 움직이며 열리거나 닫히는 메뉴 셋팅. nWillDo를 사용
	SWD_DIVIDE_OK						,		// 아이템 나누기 창에서 OK 버튼 
	SWD_ITEM_MAKE						,		// 아이템 메이크 버튼을 누를 경우  
	SWD_ITEM_MAKE_CANCEL				,		// 아이템 메이크 버튼을 누를 경우  
	SWD_ITEM_MAKE_SELECT_OK				,		// 아이템을 고른 후에 작업대 출력
	SWD_ITEM_MAKE_AFTER					,		// 아이템 만든후 작업대 닫기
	SWD_LEARNSKILL_OK					,		// 스킬 배우기...
	SWD_LEARNITEM_OK					,		// 아이템 배우기...
	SWD_ITEM_USE_SELECT_OK				,		// 스킬 사용시 사용할 아이템 선택 ok
	SWD_SKILL_CANCEL					,		// 아이템 선택중 cancel 버튼 ==> 스킬 취소...
	SWD_HEALING_OK						,		// 병원에서 치료
	SWD_PREVENTING_OK					,		// 병원에서 예방 
	SWD_INN_OK							,		// 병원에서의 기능.....
	SWD_MEMORIZE_BUTTON					,		// 메모라이즈 행위 버튼
	SWD_SELECT_LOCATION					,		// 시작 위치 지정
	SWD_ITEM_EXCHANGE_CANCEL			,		// 아이템 교환 취소 버튼
	SWD_BETA_TEST_GO					,		// 배타 테스트 설명 메뉴로 이동
	SWD_SORRY							,		
	SWD_GO_BBS_WRITE					,		// 쓰기 메뉴 활성화
	SWD_BBS_WRITE_SEND					,		// 서버로 전송
	SWD_BBS_WRITE_EXIT					,		// 쓰기 취소
	SWD_GOD_REGIST_OK					,		// 등록 하겠습니다.
	SWD_PREACH_OK						,		// 전도한 사람 등록
	SWD_PREACH_CANCEL					,		// 전도한 사람이 없음.
	SWD_GOD_REGIST_ERROR_OK				,		
	SWD_GOD_PREACH_RESULT_OK			,
	SWD_TIMEMENU_CLOSE					,		// time display menu close
	SWD_ONLY_OK							,		// this is ok-button of 'okcancel message box'
	SWD_CONNECT_RECENT					,		// 최근 로그아웃한 자리에서 접속한다.
	SWD_READ_TO_WRITE					,		// 답장 보내기
	SWD_SUPPER_BACK						,		// 신전 슈퍼에서 뒤로 돌아가기 
	SWD_WILLDO_WORK						,		// willdo의 값을 work로 옮긴다.
	SWD_CONNECT							,		// 선택한 곳으로 연결
	SWD_MAININTERFACE_SKILL				,		// 메인 메뉴에서 스킬을 열자
	SWD_HUNTER_REGIST_OK				,		// 현상범 등록 메뉴에서...
	SWD_HUNTER_LIST_OK					,		// 내 목록에 현상범을 등록 시킨다.
	SWD_MAN_TO_MAN_CANCEL				,		// 1:1 결투 메뉴 취소
	SWD_RESET_JOB						,		// 직업 재 선택하는 메뉴를 띄울지를 선택한다.
	SWD_RESET_JOB_EXPLAIN				,		// 직업 재 선택시 설명글 다시 보여주기
	SWD_CHARACTER_COLOR_CHANGE			,		// 0905 kkh 추가 
	SWD_GO_VOTE							,		// vote 메뉴 띄우기( 후보 등록 메뉴를 띄울지 아닐지를 결정한다. )
	SWD_VOTER_REGIST_OK					,		// 후보 등록하기
	SWD_VOTER_SUPPORT_OK				,
	SWD_VOTER_SUPPORT_SCROLL			,
	SWD_GM_LIST_SCROLL					,		// 그랜드 마스터 등록 스크롤
	SWD_GUILD_EXPLAIN_SCROLL			,		// 그랜드 마스터 등록 조건 아이템 설명 스크롤
	SWD_GM_REGIST						,		// 그랜드 마스터에 등록한다.
	SWD_GM_REGIST_2						,		// 상인및 기성에 등록 //021030 lsw
	SWD_NEW_ID							,		// 시작메뉴에서 now id 를 클릭했을경우 
	SWD_DEFAULT_MESSAGEBOX_OK			,		// ok button of defualt messagebox
	SWD_RE_LOGON						,		// 다시 4개의 캐릭터 선택 화면으로 들어간다.
	SWD_HELP							,		// tip 보기 띄우기...
	SWD_SALVATION_OK					,		// 기부하기
	SWD_CHANGE_MONEY_OK					,		// 돈 바꾼다.
	SWD_RUMOR_OK						,		// 누가 초콜렛을 가장 많이 주나...
	SWD_BATTLE_REGIST					,		// 길드전 등록
	SWD_REQ_LOAN						,		// 대출 메뉴 열때
	SWD_FIGHTMAP_WAIT_MEMBER			,		// 다시 대기자 명단으로 돌아간다.
	SWD_FIGHT_WAIT_EXIT					,		// 길드전 대기자 명단에서 빠지기..
	SWD_SEARCH_LADDER_RANK				,		//	래더 랭킹 게시판에서 사람 찾기
	SWD_SMALL_MENU_WITH_SWD_TO_WORK		,		// nSHideNomalNumber에 있는 값을 새로 뜨는 메뉴의 WORK에 넘겨준다.
	SWD_BUY_VISA						,		// 비자 사기
	SWD_GUILD_EXPLAIN_YES				,		// 
	SWD_REGIST_GUILD_IMSI_SEND			,		// 임시 길드 등록을 서버에 요청한다.
	SWD_GM_2ND_CANCEL					, 
	SWD_GM_2ND_OK						, 
	SWD_SKILL_MASTER_MULTIRARE_OK		,//021111 lsw 기성이 아템 업글 하것다
	SWD_NATION_INFO						, 
	SWD_NATION_BUDAE					, 
	SWD_NATION_JOIN						, 
	SWD_NATION_NOW						, 
	SWD_NATION_LADDER					, 
	SWD_NATION_VOTE						, 
	SWD_NATION_GUEST					,			
	SWD_NATION_GOTO_WAR_MAP				,
	SWD_NATION_DECSRIPT					,
	SWD_NATION_WEAPON_BUY				,
	SWD_NATION_ATTACK_WAY_POINT			,
	SWD_NATION_JOIN_					, //010730 lsw
	SWD_NATION_JOIN_JOIN_WAR			,
	SWD_NATION_NOW_						, //010730 lsw
	SWD_NATION_NOW_REFRESH				,
	SWD_NATION_NOW_BUDAE				,
	SWD_NATION_LADDER_					, //010730 lsw
	SWD_NATION_VOTE_					, //010730 lsw
	SWD_NATION_VOTE_SELECT_YOU			,
	SWD_NATION_VOTE_I_AM_HERO			,
	SWD_NATION_BUDAE_PREV_REFRESH		,
	SWD_NATION_BUDAE_NEXT_REFRESH		,
	SWD_NATION_BUDAE_JOIN				,
	SWD_NATION_VOTE_PREV_REFRESH		,
	SWD_NATION_VOTE_NEXT_REFRESH		,
	SWD_NATION_INFO_SELECT				,
	SWD_NATION_GUEST_BUDAE				,
	SWD_NATION_GUEST_JOIN				,
	SWD_NATION_GOTO_WAR_MAP_TARGET		,
	SWD_NATION_TODAY_NOTICE_IMAGE		,	
	SWD_NATION_WAR_RESULT				,		//011001 lsw
	SWD_REGIST_GUILD_SEND				,
	SWD_NATION_MSGBOX_OK				,
	SWD_DUAL_OK							,
	SWD_CALL_GUILD_EXPLAIN				,		// 길드 설명 메뉴를 호출한다.
	SWD_CALL_GUILD_CHANGE_DEGREE		,		// 길드 등급 변경 며뉴 띄우기
	SWD_GUILD_DEGREE_OK					,		// 길드 디그리 변경
	SWD_CALL_OKCANCEL_BOX				,		// 메세지 박스 띄우기
	SWD_CALL_GUILD_CHANGE				,
	SWD_INIT_ABILITY_OK					,
	SWD_CALL_SKIN_MENU					,
	
	SWD_SEND_GET_MERCHANT_SELL_LIST		,//판매물품 정보 받아오기 Sendxxxx_xxxx_xx()이런 함수 호출//상인용
	SWD_SEND_GET_MERCHANT_BUY_LIST		,//구매자 
	
	SWD_SEND_MERCHANT_BUY_LIST_SCROLL_UP,
	SWD_SEND_MERCHANT_BUY_LIST_SCROLL_DOWN,

	SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_UP,
	SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_DOWN,
	
	SWD_SEND_MERCHANT_SELL_LIST_SCROLL_UP,
	SWD_SEND_MERCHANT_SELL_LIST_SCROLL_DOWN,

	SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_UP,
	SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_DOWN,

	SWD_SEND_GET_MERCHANT_RESULT_LIST	,//거래결과물 리스트 받기

	SWD_SEND_MERCHANT_BUY_THIS_ITEM		,//구매자가 아이템 구입을 합니다.

	SWD_SEND_MERCHANT_SELL_ITEM_DELETE	,//판매자가 아이템을 판매취소 합니다.
	SWD_SEND_MERCHANT_SELL_ITEM_REGISTER,//판매자가 아이템 등록 합니다.
	SWD_MERCHANT_SELL_TYPE_SET			,//판매 방식 선택
	

	SWD_SEND_MERCHANT_RESULT_ITEM_TAKE,//결과물 가져옵니다.
	
	SWD_CALL_MERCHANT_REGISTER			,//

	SWD_ITEM_MAKE_SELECT_RARE			,
	SWD_ITEM_MAKE_SELECT_RARE_OK		,
	SWD_ITEM_MAKE_SELECT_RARE_CANCEL	,
	SWD_GABLE_ITEM						,
	SWD_ABILITY_REDISTRIBUTION			,		// 011201 LTS		// LTS MENU
	SWD_LOCALWAR_JOIN					,		// 011201 LTS
	SWD_REGIST_FRIEND					,
	SWD_DELETE_FRIEND					,
	SWD_SMALLMENU_CLOSE					,		// SWD_SMALLMENU_END 와 똑같다. 단지 단축키(ESC)가 적용이 안된다.
	SWD_POTAL_MENU						,
	SWD_GOTO_POTAL						,
	SWD_EVENT_LOCALWAR_JOIN				,		// 020115 LTS
	SWD_EVENT_LOCALWAR_MOVE				,		// 020115 LTS
	SWD_PUT_GUILD_ITEM					,
	SWD_SMALL_MENU_CLOSE_CALL_FUCTION	,
	SWD_CALL_OKCANCEL_BOX_ETC			,
	SWD_SET_SUB_MASTER					,
	SWD_NATION_ITEM_OK					,
	SWD_GUILD_HOUSE_OK					,
	SWD_GUILD_HOUSE_BUY					,
	SWD_CSP_GROUP_ICON					,//CSP == CombatSkillPoint
	SWD_CSP_GROUP_SELECT				,
	SWD_CSP_GROUP_SELECT_DONE			,
	SWD_SC_ICON_POINT					,//PONIT 에서 사용 될때의 클릭시 기능
	SWD_SC_ICON_DOING					,//DOING 에서 사용 될때의 클릭시 기능
	SWD_CALL_COMBAT_SKILL_POINT			,
	SWD_LOCALWAR_MAPMOVE				,// LTS NEW LOCALWAR
	SWD_LOCALWAR_END					,// LTS NEW LOCALWAR
	SWD_PARTY_ALL_DEL					,//020515 lsw
	SWD_TACTICS_ABILITY					,// 택틱스에서 어빌리티로
	SWD_CALL_FRIEND_MENU				,//친구 메뉴 부르기
	SWD_MOVE_TO_HOMETOWN				,
	SWD_MOVE_TO_HOMETOWN_WITH_ALIVE		,
	SWD_LW_DONE_PREV					,// LTS LOCALWAR MENU
	SWD_LW_DONE_NEXT					,
	SWD_DELETE_CHARATER					,//020725 lsw
	SWD_DELETE_CHARATER_CLOSE			,//020725 lsw
	SWD_SELECT_TUTORIAL_OK				,//020815-2 lsw
	SWD_SET_LOGIN_TARGET				,//020815-2 lsw
	SWD_AUCTION_FIND_KEY_RARE_SET,
	SWD_AUCTION_FIND_KEY_LV_SET,
	SWD_AUCTION_FIND_KEY_TACTIC_SET,
	SWD_AUCTION_FIND_KEY_WEAR_SET,
	SWD_WAR_MOVE						,
	SWD_WAR_STATUS_REFRESH,				
	SWD_CALL_MERCHANT_QUEST,
	SWD_CALL_MERCHANT_OR_SKILL_MASTER,
	SWD_CALL_SKILL_MASTER_QUEST,
	SWD_EXCHANGE_BOND_MONEY_OK,
	SWD_CALL_EXCHANGE_BOND_MONEY,
	
	SWD_LOTTO_NUMBER					,//soto-030428
	SWD_LOTTO_AUTO_GEN					,//soto-030428
	SWD_LOTTO_OK						,//soto-030428

	SWD_LOTTO_OPEN						,//soto-030429 로또창 열기.
	SWD_LUCKY_EVENT						,//soto-030429 행운이벤트 창열기.
	SWD_COMFIRMATION_WINNER				,//soto-030429 당첨자 확인하기.

	SWD_WINNER_OK						,//soto-030501 당첨자확인창 OK. 이걸 누르면.걸렸을때 아이템 지급.
	SWD_LOTTO_NUM_LEFTSEEK				,//soto-Lotto추가 로또 자신의 번호 페이지 왼쪽으로..
	SWD_LOTTO_NUM_RIGHTSEEK				,//soto-Lotto추가 로또 자신의 번호 페이지 오른쪽으로

	SWD_MAIL_SAVE,
	SWD_MAIL_WRITE,
	
	SWD_MAIL_WRITE_EXIT,		// 메일 보내기 메뉴 빠져 나가기
	SWD_MAIL_WRITE_SEND,		// 서버로 전송
	SWD_MAIL_WRITE_DELETE,		// 내용 지우기
	SWD_MAIL_CALL_RECV_LIST,	//받은 편지함
	SWD_MAIL_CALL_DELETE_LIST,	//지운 편지함
	SWD_MAIL_SELECT_ALL,		//전체 선택
	SWD_CALL_MAILMENU,		//메일 메뉴 부르기
	SWD_CALL_MAIL_WRITE,	//메일 쓰기 메뉴
	SWD_MAIL_REPAIR,
	SWD_MAIL_DELETE,
	SWD_MAIL_SCROLL,
	SWD_TEMP_MAIL_TYPE_SCROLL,//임시메일 타입 전환
	SWD_MAIL_SENDER_REGIST_FRIEND,//보낸이를 친구로 등록 하기
	SWD_OBSERVER_TEAM_SELECT,//옵저버 팀 선택했다고 통보
	SWD_SEND_MY_ARENA_BETTING_MONEY,

	SWD_AUCTION_UPDOWN_BTN,
	SWD_CALL_ARENA_OBSERVER_MENU,
	SWD_CALL_ARENA_BETTING_MENU,

	SWD_SEND_MERCHANT_BACKUP_ITEM_TAKE,			// BBD 040303
	SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_UP,	// BBD 040303
	SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_DOWN,	// BBD 040303
	SWD_MANTLE_COLOR_CHANGE,
	SWD_MALL_ITEM_BUY,							// Item mall buying item
	SWD_MANTLEDYE_CANCEL,					// Item mall cancel button
};

//메뉴 구성요소 타입번호 설정
enum FieldTypeIndex
{
	FT_NONE								= 0,	//아무것도 아님//020420 lsw
	FT_NOMAL_PUT                        ,  //이미지만 찍는 구성요소
	FT_NOMAL_PUT_CENTER                 ,  //이미지만 찍으면서 상위 메뉴 x축가운데 찍기
	FT_TEXT_PUT                         ,  //글자를 출력
	FT_TEXT_PUT_CENTER                  ,  // 상위메뉴 x축 가운데 부분에 영어 문자열을 출력
	FT_HIDE_PUT	                        ,  //마우스 L버튼이 눌렸을때 이미지를 바꿔서 보여주는 구성요소
	FT_HIDE_PUT_CENTER                  ,  //FT_HIDE_PUT 와 같으나 상위 메뉴 x축가운데 찍힌다
	FT_HIDE_PUT_AUTO                    ,  //마우스가 가까이만 가면 버튼을 누르지 않아도 영역체크
	FT_HIDE_PUT_CENTER_AUTO             ,  //FT_HIDE_PUT_AUTO와 같으나 x축 가운데에 찍힌다
	FT_HIDE_NOMAL_ONE_PUT               ,  //메뉴구성요소중 여러개의 이미지를 가지고 있을때 여러개중1가지를 선택해서 보여줘야 하는 구성요소
	FT_HIDE_NOMAL_GROUP_PUT             , //메뉴구성요소중 여러개의 이미지를 가지고 있을때 여러개를 선택해서 보여줘야 하는 구성요소
	FT_HIDE_WILLDO_PUT                  , //마우스 L버튼으로 눌렀다 때면 다른메뉴를 호출하는 형식
	FT_HIDE_SPECIAL_WILLDO_PUT  		, //단순히 메뉴호출만 하는게 아니라 여러가지 특별한 일을 할경우
	FT_SPECIAL_WILLDO                   , //이미지를 찍지 않고 작동한다
	FT_HIDE_ISCROLL_PUT                 , //FT_HIDE_NOMAL_ 와 연결된 스크롤 버튼을 눌렀을때 FT_HIDE_NOMAL_가 찍힐 범위를 증가 설정
	FT_HIDE_DSCROLL_PUT                 , //FT_HIDE_NOMAL_ 와 연결된 스크롤 버튼을 눌렀을때 FT_HIDE_NOMAL_가 찍힐 범위를 감소 설정
	FT_THROWDICE                        , //주사위를 던져주는 타입
	FT_DATA_PUT                 		, //한글영어 숫자 모두 출력 가능 nImageName에 DP_ 를 대입해 이용한다
	FT_FIRE                             , //불꽃을 찍어주는 타입
	FT_INVENTORY                        , //아이템 메뉴에서 인벤토리 창과 관련된 필드 마우스이베트를 체크해 아이템 이동을 한다
	FT_NO_CHECK                         ,  //그룹 필드에서 어느것도 출력을 하지 않을때 사용.
	FT_ACCEL_INVENTORY                  ,  //마우스 이벤트를 체크하는 단축 아이템창 
	FT_ARMOR_INVENTORY                  ,  // 무기 장착 화면에서 마우스 이벤트 체크
	FT_WILLDO_PUT				        ,  // 마우스 클릭후 그냥 다른 메뉴 호출 할때
	FT_DO						        ,	// 그냥 뭔가를 할때,
	FT_GAUGE							,	// 게이지 바에 뭔가를 채울때(현재는 로딩 화면만, 필요하면 변수 설정및 보수 필요)
	FT_DO_DEFAULT						,	// FT_DO 에서 화면이 스크롤 되며 뜰때 
	FT_IDEDITBOX						,	//아이디 입력 에디터 박스 호출
	FT_PASSEDITBOX						,	//패스워드 입력 에디터 박스 호출
	FT_NOMAL_PUT_TALK_BATTLE_PEACE		,		// TALK, PEACE, BATTEL 버튼 출력을 위한...
	FT_LODING_LIGHT					    ,		//로딩할때, 연출...
	FT_MAGIC_ITEM_PUT				    ,		// 매직 아이템 출력
	FT_NOMAL_PUT_CENTER_1			    ,		// _1이 없는 것이 트랜스 출력
	FT_HOTKEY_ICON					    ,		// 단축 출력및 사용 가능 하게 하는것 t_QuickMemory 를 이용함
	FT_COLOR_GAUGE						,		// 외모 색 결정
	FT_MENU_MOVE					    ,		// 메뉴를 드래그로 옮길때
	FT_HIDE_WILLDO_AUTO_PUT			    ,		//	마우스 충돌때 그림을 바꿔 보여주고 클릭할때 또 그림을 바꾸면서 다른 메뉴를 콜할때
	FT_HIDE_SPECIAL_WILLDO_AUTO_PUT     ,		// 마우스 충돌 그림을 보여주고 클릭하면 여러가지 일을 할때
	FT_HIDE_ISCROLL_AUTO_PUT		    ,
	FT_HIDE_DSCROLL_AUTO_PUT		    ,	
	FT_BANK_INVENTORY				    ,
	FT_TEXT_PUT_CENTER_X				,	// x 변수에 넓이를 넣어 준다.
	FT_HIDE_AUTO_PUT				    ,
	FT_JOB_ITEM					    	,
	FT_TOWN_CONSTRUCTION		        ,	// 동서남북 체크 
	FT_HOME_EDITBOX				        ,	// 내집 시스템에서 메시지 창 띄우기 버튼 깜박임
	FT_SWITCH_PUT				        ,	// 번갈아 가며 그림을 찍을 때
	FT_SKILL_PUT					    ,	// 스킬 실행
	FT_RETURN_FUCTION					,
	FT_OPTION_GAUGE				    	,	// 옵션 메뉴에서 게이지 바 보여주기
	FT_SYSTEM_OPTION_SCROLL		        ,	// 시스템 옵션 메뉴에서 스크롤 바 움직이기
	FT_KNOWLEDGE_ICON_PUT		        ,
	FT_INN						        ,	// 여관에서 사용할 타입
	FT_LEVELUP_BUTTON			        ,	// 게임 화면에 뜨는 버튼
	FT_LEVELUP					        ,	// 어빌리창에 뜨는 + 버튼들
	FT_NOMAL_PUT_FX				        ,	// 메시지 창 뒤에 뜨는 화면 효과
	FT_BBS_GO						    ,	// 각각의 공지를 보기위한 필드
	FT_PARTY					    	,	// 파티메뉴에관한 필드 타입
	FT_PARTY_FACE				    	,	// 관계얼굴 보기
	FT_DIVIDE_ITEM_PUT			    	,	// 돈 나누기 창에서 아이템 갯수 표시
	FT_MAKE_ITEM						,	// 아이템 만들기 창에서 아이템 출력및 작업
	FT_ITEM_MAKE_SELECT			        ,	// 만들수 있는 아이템 목록
	FT_MAIN_MENU				        ,	// 메인 메뉴에서 파생되는 메뉴 버튼들... // FT_HIDE_SPECIAL_WILLDO_AUTO_PUT 과 같이 쓰인다.
	FT_MAKE_ITEM_OK				        ,	// 만들어지는 아이템 연출 
	FT_LEARN_ITEM				        ,	// 직업길드에서 아이템 배우기
	FT_CHARACTER_ITEM			    	,	// 다른 캐릭터 아이템 띄우기
	FT_TEXT_BUTTON				        ,	// 글자를 클릭하면 글자색을 달리 보여준다.
	FT_ITEM_USE_SELECT			        ,	// 스킬 사용시 사용할 아이템 선택
	FT_SELECT_FACE					    ,	// 캐릭터 만들기에서 얼굴 선택
	FT_MAGIC_CLASS				        ,	// 매직 계열
	FT_DSCROLL_BOOK				        ,
	FT_ISCROLL_BOOK				        ,	// 마법책 넘기기
	FT_MAGIC_BOOK_ETC			        ,	
	FT_LEARN_MAGIC						,	// 마법책 인터페이스에서 마법 배우기
	FT_HEAL_TEXT_BUTTON			        ,	//	치료할 병 버튼 출력
	FT_DSCROLL_MEM_BOOK			        ,	//
	FT_ISCROLL_MEM_BOOK			        ,	// 메모라이즈 마법 책 넘기기
	FT_MEM_MAGIC_ICON			        ,	// 메모라이즈 마법 책에서 아이콘 버튼 
	FT_MEM_BOOK_ETC				    	,	// 메모라이즈 마법 책에서 처음 항상 띄워놓아야할 이미지 출력
	FT_PARTY_FACE_PUT			        ,	// 파티 얼굴 메뉴에서 사용
	FT_VIEW_SMALL_MAP			        ,	// 뷰 스몰 멥
	FT_MENU_MOVE_DIRECT				    ,	// 메뉴를 직접 움직일때
	FT_EXCHANGE_ITEM			        ,	// 아이템 보여주기
	FT_EXCHANGE_ITEM_CHECK		        ,	// 아이템 check
	FT_EXCHANGE_ITEM_ETC		        ,
	FT_MAN_TO_MAN_INFO_DISPLAY			,	//1:1 결투 누가 싸우나 정보
	FT_TEST_CHECK				        ,	// 베타 테스트 관련~
	FT_BETA_TEST_VIEW					,	// 베타 테스터에 대한 글 출력
	FT_CREDIT_PUT				    	,
	FT_INFO_PUT					    	,
	FT_PUT_DISTANCE				    	,	// 이정표 출력
	FT_STORE_SIGN				    	,	// 상점 간판
	FT_AWAKE_BUTTON				        ,	//	깨어나는 버튼..
	FT_GOD_INTRUDUCTION			        ,	// 신전 설명
	FT_GOD_MEETING				        ,	// 신전 예배 설명
	FT_GOD_REGIST_OKCANCEL			    ,	// 등록 하시겠습니까?
	FT_PREACH_NAME						,	// 전도한 사람 이름을 적을 수 있게
	FT_GOD_REGIST_ERROR				    ,	// 등록 에러
	FT_QUICK_KEY					    ,	// 단축키
	FT_VIEW_GOD_MEETING				    ,	// 음악이나 예배 내용 출력
	FT_NATION_DISPLAY				    ,	// 국가 표시 
	FT_HIDE_WILLDO_AUTO_PUT_MAGIC	    ,	// 메직 메뉴에서 QuickKey 출력을 위해
	FT_MENU_MOVE_DEFAULT			    ,	// 메뉴 움직이기
	FT_WILLDO_WORK					    ,	// willdo의 값을 work에 옮긴다.
	FT_BELIEVE_GOD					    ,	// 믿는신과 신앙심 찍기
	FT_SUPPER_INV					    ,	// 신전에서 음식 제공 메뉴
	FT_SMALL_MAP_HOUSE				    ,	// 스몰맵에서 커서 가까이 가면 밝게 빛나며 집 이름 띄워주기
	FT_EXPLAIN_MENU					    ,	// 메뉴 설명 보여주기
	FT_QUICK_MAGIC_PLUS				    ,	// 사용한 마법 저장
	FT_QUICK_MAGIC_ARRAY_SCROLL		    ,		// 퀵 마법 배열 스크롤 다운 
	FT_XY_DISPLAY						,		// 좌표 디스플레이 
	FT_TIME_DISPLAY_ON_OFF				,		// 시간 메뉴 활성화 버튼
	FT_CHAT_SCROLL						,		// 채팅시 스크롤 버튼 
	FT_SELECT_CITY						,		// 시작화면에서 연결할 마을 선택
	FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2	,		// 두개를 같이 찍을때
	FT_FIRST_QUICK_KEY					,		// 가장 우선 순위로 키 처리를 해야 할때( 다른 경우를 배제 하고 꼭 이루어 져야 할 단축키 )
	FT_PUBLIC_BBS						,		// 공지사항 띄우기
	FT_QUICK_SKILL_PUT					,		// 스킬 메뉴
	FT_QUICK_SKILL_SCROLL				,		// 스킬 스크롤 - 제목판
	FT_QUICK_SKILL_SCROLL_LR			,		// 스킬 스크롤 - 왼쪽 오른쪽
	FT_PUBLIC_BBS_NEXT					,
	FT_HUNTER_REGIST					,		// 현상범 메뉴 보여주가
	FT_HUNTER_LIST						,		// 현상범 리스트 보여주기
	FT_HUNTER_LIST_SCROLL				,		// 현상범 리스트 스크롤
	FT_DEFAULT_BUTTON					,
	FT_QUEST_OK							,
	FT_QUEST_SCROLL						,
	FT_QUEST_INDEX						,
	FT_QUEST_INDEX_SCROLL				,
	FT_MAN_TO_MAN_CHECK					,		// 일대일 대결 체크 필드
	FT_RESET_JOB_EXPLAIN				,		// 직업 재 선택 설명 필드
	FT_RESET_JOB_SCROLL					,		// 직업 재 선택에 따른 설명 스크롤 필드
	FT_COLOR_CHANGE_GAUGE				,		// 0905 kkh 추가 
	FT_SELECT_NATION					,		// 나라 선택
	FT_SELECT_NATION_EXPLAIN			,		// 나라 설명
	FT_SELECT_NATION_SCROLL				,
	FT_NATION_BBS						,		// 나라 헌장문 출력
	FT_NATION_BUTTON					,		// 나라 헌장문 버튼들
	FT_FIGHT_MAP_SIGN					,		// 파이트 맵 전광판 사라지는것 확인
	FT_DISPLAY_SCENARIO					,		// 시나리오 출력
	FT_VOTE_REGIST						,		// 등록 메뉴 설명글 출력
	FT_VOTE_SUPPORT						,		// 투표하기
	FT_VOTE_SCORE_PUT					,		// 득표상황 보여주기
	FT_GM_LIST							,		// 그랜드 마스터 등록하기
	FT_GM_ITEM_EXPLAIN					,		// 그랜드 마스터 등록 조건 아이템 설명해주기
	FT_GM_ITEM_EXPLAIN_BUTTON			,		// 버튼 클릭
	FT_TEXT_BUTTON_EXIT					,		// exit 버튼
	FT_DISPLAY_EXP						,		// skill경험치 보여주기 (게이지 포함)
	FT_VIEW_TIP							,		// tip 버튼 반짝이게
	FT_PUT_TACTICS						,		// tactics 선택창에서 한글 택틱 그림 보여주기
	FT_SCROLL_TEXT_PUT					,		// work : 페이지 수, key : 한 페이지에 출력 가능한 라인,  scroll	// cheaktype 을 1로 셋팅해주는데가 따로 존재해야 한다.
	FT_SCROLL_BUTTON					,		// 스크롤 버튼
	FT_SALVATION_MONEY					,		// 기부금액
	FT_SALVATION_MONEY_SCROLL			,		// 기부금액 스크롤
	FT_USED_MAGIC_EXPLAIN				,		// 사용 마법 설명 보여주기
	FT_ANIMATION						,
	FT_TEMBATLE_INFO					,		// 팀배틀에 들어온 사람 정보 보여주기
	FT_SELECT_SERVER_BUTTON				,		// 서버 선택 메뉴 중 텍스트 버튼 
	FT_SELECT_SERVER_SCROLL				,		
	FT_TEMBATLE_ADD						,		// 팀에 들어간다.
	FT_CHANGE_MONEY						,		// 환전소
	FT_BATTLE_WAIT						,		// 팀 배틀 등록
	FT_VIEW_BATTLE_MEMBER				,		// 팀 멤버 보여주기
	FT_DELAY_BUTTON						,		// 딜레이 버튼
	FT_BANK_LOAN						,
	FT_VIEW_BATTLE_WAIT_MEMBER			,		// 대기자 명단 보여주기
	FT_FIGHTMAP_START					,		// 배틀을 시작한다.
	FT_CHECK_COMBO						,		// 라디오버튼 구현 nValue 와 nWillDo 를 이용하여 구현
	FT_VIEW_BBS_MONEY					,		// 공지 가격 출력
	FT_WRITE_BBS						,		// 에디터 박스 하나 띄우고 그 입력 스트링을 디폴트하게 출력시켜준다.
	FT_WRITE_BBS_ITEM_NAME				,		// 아이템 이름 출력
	FT_WRITE_BBS_ITEM_MONEY				,		// 아이템 돈 출력
	FT_SMALL_TIPS_BUTTON				,
	FT_VIEW_LADDER_RANK					,
	FT_VIEW_EFFECT_TOOL					,		// effect tool
	FT_VISA								,		// 비자 내용과 가격 보여주기
	FT_SELECT_VISA						,		// 비자 사기 위해 선택
	FT_INPUT_GUILD_INFO					,		// 길드 정보 입력하기
	FT_GUILD_DEGREE						,		// 길드 직책 입력 메뉴
	FT_NORMAL_PUT_CENTER2				,		// 임시 변수 m_lpTemp 의 spr 을 가운데 이미지로 찍어 준다.
	FT_GUILD_INTRODUCTION				,		// 길드 
	FT_VIEW_TEXT_BY_HTML				,		// html형식으로 화면 출력한다.
	FT_INPUT_GUILD_MASTER				,		// 서브 길마
	FT_GM_2ND_SKILL_MAKE_ITEM			,//2001 이었음 201 로 교체 
	FT_SKILL_MASTER_MAKE_ITEM			,//021113 lsw
	FT_EXCHANGE_BOND_MONEY_ITEM			,//본드 머니 드롭존021126 lsw
	FT_TODAY_NOTICE						,
	FT_NOMAL_PUT_BY_NWILLDO				,//010910 lsw winlldo 값에 따라 그림이 나옴
	FT_SHOW_SPECIAL_WILLDO_AUTO_PUT		,//RectImage 는 버튼 클릭 nImageNumber보통때와 밝게
	FT_PUT_BUTTON						,	// 다음 필드들의 버튼을 찍어 준다. 
	FT_CHECK_COMBO2						,		// 콤보 박스와 똑같지만, 글자를 찍어 주는 기능을 가지고 있다.
	FT_NOMAL_PUT_BY_NWILLDO_CUSTOM		,//011008 lsw 
	FT_VIEW_RUNE_LEFT					,
	FT_VIEW_RUNE_RIGHT					,
	FT_SCROLL_BUTTON2					,			// 특정 필드를 변수로 처리 그냥 FT_SCROLL_BUTTON 의 경우엔 key 값을 변수로 사용
	FT_SKIN_MONEY						,
	FT_RARE_GROUP						,//011013 lsw
	FT_RARE_GROUP_SCROLL				,//011013 lsw
	FT_MAKE_RARE_MORE					,//011013 lsw
	FT_CHECK_SALVATION					,
	FT_ROULETTE							,
	FT_LOCALWAR_JOIN					,			// LTS MENU
	FT_GUILD_MEMBER_LIST_DEGREE_TAB		,
	FT_GUILD_PUBLIC_NOTICE				,
	FT_FRIEND_NAME						,
	FT_FRIEND_REGIST_NAME				,
	FT_GAMBLE_TODAY						,
	FT_POTAL_MAP						,
	FT_POTAL_SELECT						,
	FT_POTAL_SELECT_PUT					,
	FT_GUILD_ITEM_GET					,
	FT_GUILD_ITEM_GET_ETC				,
	FT_GUILD_ITEM_PUT_BLANK				,	// 길드 창고에 남은 공간 보여주기
	FT_GUILD_ITEM_PUT					,	// 길드 창고에 아이템 넣기
	FT_RARE_GROUP_MEMBER_DISPLAY		,
	FT_FILE_TEXT_PUT					,
	FT_NATION_ITEM						,
	FT_REVIVAL_BUTTON					,
	FT_GUILD_HOUSE						,
	FT_GUILD_HOUSE_INFO					,
	FT_POTION_BOX						,
	FT_CS_ICON							, //CS == CombatSkill//CSP 와 CSP 가 공통으로 사용 스페샬 윌두 타입으로 구분
	FT_CSD_ICON							, //전투스킬 두잉, nWillDo의 전투 스킬 인덱스만을 참조하여 이미지찍기와 영역설정, 하는 일을 정한다
	FT_CSD_MAIN_ICON					, 
	FT_CSD_POINT_LINE_BAR				, //게이지 바
	FT_CSD_POINT_COUNT_BAR				, //카운트 바
	FT_CSP_POINT_UP						, //CS == CombatSkill//CSP 와 CSP 가 공통으로 사용 스페샬 윌두 타입으로 구분
	FT_CS_GROUP_EXPLAIN					,	//전투 스킬 그룹 설명 하는 아이콘 클릭 이벤트는 없음 //인터페이스의 work값과 nWillDo의 그룹 타입 으로 찍어냄
	FT_CS_EXPLAIN						,//전투 스킬 설명 하는 글 클릭 이벤트는 없음 //nWillDo 에는 전투스킬 인덱스 값이 들어감
	FT_LOCALWAR_MAPMOVE					,	// LTS NEW LOCALWAR	// 국지전 맵이동 
	FT_ANIMATION2						,
	FT_PEACE_BATTLE_PUT					,
	FT_CANDLE_INVENTORY					,
	FT_DISPLAY_TAC_EXP					,
	FT_DISPLAY_MAGIC_EXP				,
	FT_DISPLAY_SKILL_EXP				,
	FT_ALPHA_PUT						,
	FT_SELECT_WISPER_TARGET				,
	FT_SELECT_WISPER					,
	FT_SELECT_CHAT_TYPE					,
	FT_CHAT_LOCK						,
	FT_CHAT_VIEW_SET					,
	FT_DRAG_ZONE						,
	FT_SCENARIO_TRAP_INFO				,		
	FT_SCENARIO_TRAP_KEY				,
	FT_INPUT_DEL_CHAR_COMFORM_KEY_VIEW	,				// 채팅창에 대화 보여주기//020802 lsw
	FT_HUNTER_VERSUS_DISPLAY			,//011001 lsw
	FT_MERCHANT_FINDER_LIST,//상인 아이템 검색 필드
	FT_MERCHANT_SELL_ITEM_EXPLAIN,
	FT_MERCHANT_SELL_ITEM_DROP_ZONE,
	FT_MERCHANT_SELL_ITEM_CHECK,//체크버튼
	FT_MERCHANT_BUY_ITEM_CHECK,//사는놈의 체크버튼
	FT_MERCHANT_DIRECT_BUY_ITEM_CHECK,//사는놈의 체크버튼
	FT_MERCHANT_RESULT_ITEM_CHECK,//사는놈의 체크버튼
	FT_MERCHANT_SELL_ITEM_VALUE_INPUT,

	FT_SCRIPT_TIMMER_COUNT,			// 021021 kyo 타이머 숫자 보여주기
	FT_SCRIPT_COUNTER_COUNT,		// 021021 kyo 카운터 숫자 보여주기
	FT_DISPLAY_GM_REGIST,//GM등록 관련 정보 디스플레이 하기
	
	FT_LOTTO_RUN,//soto-030425 로또창 실행 버튼
	FT_LOTTO_NUMBER,//soto-030425 로또의 숫자 버튼.

	FT_MAIL_LIST,
	FT_MAIL_WRITE_RECV				    , // 메일 보내기 버튼
	FT_MAIL_WRITE_TITLE					,
	FT_MAIL_WRITE_BODY				    , // 메일 보내기 메뉴 닫기
	FT_MAIL_WRITE					    ,	
	FT_MAIL_READ					    ,	// 받은 메일 내용 보기
	FT_SELECT_OBSERVER_TARGET_TEAM		,//아레나 팀선택 버튼
	FT_ARENA_BETTING					,//아레나 배팅 금액
	FT_SELECT_BETTING_TARGET_TEAM		,//
	FT_WRITE_MERCHANT					,//상인 검색 창중 에디트 필드. soto-030511
	FT_CHECK_MERCHANT_FIND				,
	FT_MAININTERFACE_MAIL_CALL			,//메인 인터페이의 메일 메뉴 콜
	FT_MERCHANT_BACKUP_GET_LIST			,// BBD 040303	
	FT_MANTLE_COLOR_CHANGE_GAUGE		, // Finito 23/06/08 - for dyeing mantle
	FT_MALL_ITEM						, // mall item inventory
}; 

/////////////////////////////////////////////////////////////////////////////////
//출력관련 정의  DP_와 FT_는 비슷하지만 FT_는 항상 같은것을 보여주는데 쓰고 DP_는 수치등과 같이 수시로 바뀌는걸 보여줄때 슨다
enum DisplayIndex
{
	DP_NONE							= 0 ,
	DP_GENDER                     	,            //성별
	DP_NAME						  	,           //이름
	DP_ARIGEMENT				  	,          //가치관
	DP_CLASS					  	,             //클래스
	DP_SPELL					  	,         //스펠
	DP_TACTICS					  	,        //택틱스남
	DP_JOB						  	,       //직업
	DP_STR						  	,      //STR
	DP_CON						  	,     //
	DP_DEX						  	,    //
	DP_WIS						  	,	        //
	DP_INT						  	,            //
	DP_CHA						  	,            //
	DP_MOVP						  	,            //
	DP_ENDU						  	,		    //
	DP_MOR						  	,            //
	DP_WSPS						  	,            //
	DP_LUCK						  	,            //  
	DP_NAMEEDITBOX                	,            //이름이 써지는 칸
	DP_FACE                       	,            //얼굴 표정을 출력
	DP_INVENTORY                  	,            //아이템 메뉴에서 아이템몰록을 출력
	DP_LEVEL                      	,            //레벨을 출력
	DP_EXP                        	,            //경험치를 출력
	DP_ARIGEMENT_BAR              	,            //가치관이 나오는 메뉴에서 출력되는 막대그래프
	DP_HP_BAR                     	,            //메인 인터페이스 에서 HP막대 게이지를 보여준다
	DP_MP_BAR                     	,            //메인 인터페이스 에서 MP막대 게이지를 보여준다.
	DP_HP                         	,            //HP수치를 보여준다
	DP_MP                         	,            //MP수치를 보여준다.
	DP_MAIN_LEVEL			      	,			//메인 인터페이스에서 레벨값을 보여준다.
	DP_MAIN_STR				      	,			//메인 인터페이스에서 STR값을 보여준다.
	DP_ACCEL_INVENTORY            	,             //단축 아이템창에서 실제로 아이템 이미지를 보여준다
	DP_ARMOR_INVENTORY            	,             //무기 장비 창에 실제로 아이템 이미지를 보여준다
	DP_CONDITION                  	,            //메인 인터페이스에서 캐릭터 상태를 나타내는 아이콘 출력
	DP_FACEIMAGE					,				// 얼굴 이미지 찍기
	DP_GUILD						,
	DP_TACTICE						,
	DP_DAMAGE						,
	DP_HEALTH						,
	DP_MOV							,
	DP_HEAT							,
	DP_AC							,
	DP_FAME							,
	DP_MONEY						,
	DP_IDEDITBOX					,				// 아이디 출력
	DP_PASSEDITBOX					,				// 패스워드 출력
	DP_HEALTH_GAUGE					,
	DP_LEVEL_LOGON              	,            //로그온 화면에서 레벨을 출력
	DP_TEMP_MONEY					,				// 은행에서 입력받는 창에 금액 출력
	DP_BANK_SAVE					,				//
	DP_BANK_TOTAL					,				// 은행에서 총예금액 출력
	DP_BANK_INVENTORY				,
	DP_WEIGHT						,				// 무게 출력
	DP_BANK_CURRENT_PAY				,				// 아이템 보관료
	DP_BANK_PAY						,
	DP_CREDIT_TEXT					,				// 신용 등급 출력
	DP_LOAN_TOTAL					,				// 대출에 총액
	DP_LOAN_ABLE					,				// 대출 가능액
	DP_DAY_TEXT						,				// 상환 날짜 표시
	DP_REPAYMENT_MONEY				,				// 먼저 갚아야할 금액
	DP_REPAYMENT_DATE				,				// 먼저 상환해야할 기한
	DP_GUILD_INVENTORY				,
	DP_SELECT_ITEM					,
	DP_CHARACTER_MONEY				,				// 캐러가 가지고 있는 돈 출력 -> nWillDo에 출력할 칸의 길이  x,y 에 좌표값을 넣어준다
	DP_STRING_PUT					,				// 문자열 출력
	DP_RECT_STRING_PUT          	,				//신전메뉴1번에서 MEssage출력
	DP_SKILL_ICON					,				// 스킬메뉴에서 스킬 아이콘 찍기
	DP_HEALING_EXPLAIN				,				// HEALING 기술 설명
	DP_NAME_SELECT					,				// 캐릭터 선택창에서 사용할 데이터
	DP_LAST_LOAN					,
	DP_FACE_PUT						,				//010925 lsw 얼굴 이미지 찍기 타입이 Gender 렉트 이미지가 얼굴 번호 미들 크기
	DP_RECT_STRING_PUT_CUSTOM		,//011218 lsw
	DP_TACTICS_DATA					,
	DP_ABILITY						,				// 어빌리티 창에서 사용
	DP_DATAPUT						,				// 레벨업 포인트		// nWilldo에는 어떤 능력치 인지, nRectImage 에는 길이를 넣는다.
	DP_RESIST						,				// 어빌리티 창에서 resist에서 사용
	DP_EXP_NEXT						,
	DP_AGE_MAKE						,				// 나이 결정
	DP_AGE							,				// 나이 
	DP_TOWNGALL_TITLE				,				// 게시판 제목 출력
	DP_TOWNGALL_TITLE_DATE			,				// 게시판 날짜 
	DP_BBS_PUT						,				// 게시판 내용 찍기 
	DP_BBS_WRITE_PUT				,
	DP_HOUSE_PUT					,				// 빌딩보여주기
	DP_HOUSE_REPAIR					,				// 건물 수리메뉴 
	DP_ITEM_MESSAGE					,				// item 설명 창
	DP_SELECT_SELECT				,				// 아이템 만들기 메뉴에서 선택한 아이템 표시
	DP_SELECT_SKILL					,				// 스킬 배우기에서 배우고자 하는 스킬을 보여준다.
	DP_USE_SELECT					,				// 스킬 사용시 선택된 사용 아이템 보여주기
	DP_MAGIC_PUT					,				// 마법책 인터페이스에서 매직 아이콘 보여주기
	DP_ITEM_PUT						,				// 정해진 좌표에 아이템 찍기
	DP_DISEASE_PUT					,				//	내가 걸린 병 출력
	DP_HEAL_MONEY					,				// 치료비
	DP_PREVENT_MONEY				,				// 예방비
	DP_INN							,				// 여관에서... 선택 막대 계속 보여주기
	DP_MAGIC_CLASS					,				// 메모라이즈 메뉴에서 계열 보여주기...
	DP_MEMORIZE_POINT				,			// 메모라이즈 메뉴에서 포이트 관련 수치
	DP_NORMAL_PUT					,			// 0908 kkh 추가  보여주기
	DP_DATAPUT_DEFAULT				,			// 가운데 정렬로 값 보여주기 value변수 사용, 칼라값 표현
	DP_MONEY2						,			// , 를 이용해 가지고 있는 현재 돈 출력..
	DP_BACK_IMAGE_VALUE_TEXT		,			// 투명 막 뒤에 그리고 그 위에 value 변수의 값을 출력한다.
	DP_TITLE_PUT					,			// 메뉴등에 타이틀을 찍기 위해...
	DP_STATUS_IMAGE					,			//010830 lsw 상황에 따른 이미지 변경
	DP_NATION_JOIN_STATUS_BAR		,			//선거용 막대바
	DP_DATAPUT_DEFAULT_STRING		,			// 스트링 찍어 주기 m_lpTemp 변수 이용
	DP_GM_2ND_SKILL_TARGET_ITEM_PUT		,		
	DP_NATION_WAR_RESULT_BAR			,		
	DP_REMAIN_TIME_PUT					,
	DP_NATION_WAR_RESULT_ANIMATION		,
	DP_RARE_UPGRADE_RESULT				,		
	DP_RARE_SUCCESS_RATE_DISPLAY			,//011013 lsw
	DP_RARE_GROUP_MEMBER_DISPLAY			,//011013 lsw
	DP_RARE_SUCCESS_RATE_DISPLAY_NO_TOTAL	,
	DP_LEVEL_IN_TIME_MENU					,           //레벨을 출력
	DP_NAME_CENTER							,//020515 lsw 이름을 가운데 정렬로 찍기
	
	DP_AUCTION_FK_RARE,//상인메뉴에서 검색기에 따른 한글 출력
	DP_AUCTION_FK_LV,
	DP_AUCTION_FK_TACTIC,
	DP_AUCTION_FK_WEAR,

	DP_SKILL_MASTER_RESULT_ITEM,//기성의 결과물 아이템 디스플레이
	DP_BETTING_MONEY,		//배팅된 총 금액
	DP_MY_BETTING_MONEY,	//나의 배팅 금액
	DP_ARENA_WAIT_TIME,		//아레나를 시작 하는데 까지 걸리는 시간 카운터
	DP_TEMP_MAIL_ICON,//
	DP_NOW_VIEW_MAIL_PAGE,
	DP_ARENA_TEAM_WIN_RATE,//승리율

	DP_AUCTION_DATE,//soto-030512 상인 메뉴에서의 기간 검색기의 텍스트 디스플레이.
	DP_CHARACTER_RAJA_EMBLEM,
};
// 이후 부턴 menudef2.h를 이용



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// FT_DO에 쓰일 정의(1번, 3번, 26번은 사용중)
enum DoIndex
{
	DO_CHARACTER				= 10,				//캐릭터 전신 찍기
	DO_TEXT_PUT					= 11,				//메시지 박스에서 문자열 출력
	DO_BANK_SCROLL_UP			= 35,				// 은행 돈 올리고 내리기 
	DO_BANK_SCROLL_DOWN			= 36,
	DO_BANK_TIME_SCROLL_UP		= 100,				// 경매 날짜 조정하기
	DO_BANK_TIME_SCROLL_DOWN	= 101,
	DO_BANK_SCROLL_LEFT			= 102,				// 경매 물건 찾기
	DO_BANK_SCROLL_RIGHT		= 103,
	DO_SKILL_ICON_SELECT		= 114,				// 스킬 메뉴 스크롤 할 경우 이미지 바꿔주기
	DO_SKILL_ICON				= 115,				// 아이콘과 타이틀 바를 보여주고 아이콘 실행시키기
	DO_FACE_MENU				= 200,				// 표정 선택 창
	DO_SELECT_LOAD_CHAR			= 210,				// 로드할 캐릭터정보ㅣ
	DO_CHATTING_VIEW			= 300,				// 채팅창에 대화 보여주기
	DO_CHATTING_EDITBOX			= 310,				// 채팅창에 글 쓰기 에디터 박스
	DO_LEARNSKILL_ICON			= 320,				// 배울수 있는 스킬 보여주기
	DO_DIVIDE_SCROLL_UP			= 330,				// 돈 나누기 창에서 스크롤 버튼
	DO_DIVIDE_SCROLL_DOWN		= 340
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//움직여야 하는 메뉴 관련 선언
#define MM_STATUS_OPEN                1             //움직이는 메뉴중 스테이터스 메뉴가 등장
#define MM_STATUS_CLOSE               2             //움직이는 메뉴중 스테이터스 메뉴가 사라짐
#define MM_ABILITY_CLOSE              3             //움직이는 메뉴중 어빌리티 메뉴가 사라짐
#define MM_ARIGEMENT_CLOSE            4             //움직이는 메뉴중 가치관 메뉴가 사라짐
#define MM_ITEM_OPEN                  13            //    "
#define MM_ITEM_CLOSE                 14            //    "
#define MM_PARTY_OPEN				  15			//움직이는 메뉴중 파티메뉴 등장
#define MM_PARTY_CLOSE				  16			//움직이는 메뉴중 파티메뉴 사라짐
#define MM_TACTICS_CLOSE			  17			//움직이는 메뉴중 파티메뉴 사라짐

#define MOVE_OPEN						1		
#define MOVE_CLOSE						0
extern char* GetDayOfWeekByNo(const int iDOWIndex);
#endif//AFX_MENUDEFINE_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_
