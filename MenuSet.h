#ifndef __MENUSET_H__
#define __MENUSET_H__

#include "menunetwork.h"		//021030 YGI

#define DUR_RATIO		.70		// repair에서 사용할 duration 비율
#define RARE_DUR_RATIO	.50	//020505 lsw 레어 에서 사용됨

#define RED_COLOR		RGB(255,0,0)
#define YELLOW_COLOR	RGB( 255, 255, 0 )
#define BLUE_COLOR		RGB( 0, 0, 255 )
#define GREEN_COLOR		RGB( 0, 255, 0 )
#define GRAY_COLOR		RGB( 100, 100, 100 )

extern const int MAX_START;//020815-2 lsw

extern int GetItemMuchByMuchItem( int item_no );		// 수량이 있는 아이템의 갯수 파악하기

#define SCRIPT_IS_READ_NATION_BBS		46
////////////////////////////////////
#define MAX_BUY_ITEM	50
extern void MenuSet();
extern void SmallMenuSet();
extern bool	g_MagicMenuOn;
extern bool	g_explain_skill_on;
extern int	g_explain_skill;
extern bool	g_explain_magic_on;
extern int	g_explain_magic;
extern bool	g_explain_menu_on;
extern char	g_explain_menu[MAX_STRING];
extern int	magic_plus[MAX_MAGIC];		// 내가 사용한 매직 누적
extern int	magic_plus_count;			// 총 갯수
extern int	magic_plus_start;			// 찍을 이미지 start부분
extern bool recv_learn_item;
extern int	danger_by_hunter;
extern int	man_to_man_start;
extern int	fight_map_stop;

extern int before_skill_no;

extern char explain_tactics[13][20];


extern Spr open_spr[75];
extern Spr spr_alphabet[36];

extern Spr	ch_armor_image_after[7][80];
extern Spr	ch_body_image_after[7][80];
extern Spr	ch_line_image_after[7][80];
extern Spr	ch_armor_image_after_b[7][80];
extern Spr	ch_body_image_after_b[7][80];
extern Spr	ch_line_image_after_b[7][80];

extern Spr	skill_spr[MAX_SKILL];

extern	BOOL LButtonDown;       
extern	POINT StateMousePoint;	
extern	SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];   
extern	SCharacter SCharacterData;
extern	CHARACTERLIST g_CharacterList;
extern	char y_temp[20];           

extern bool	no_active_box_yet;				// 서버에서 값을 전송 받으면 false set.
extern	int cDiceCount;			
extern	int y_delay;			
extern	int y_MenuFocus;		
								
extern	char sId[30];		
extern	char sPassword[30];
extern	SMENU SMenu[MAX_MENU];

extern	BOOL	RButtonDown;
extern	BOOL	MouseDrag;
extern	int		LButtonCount;		

extern	int		y_PutBarSize;       
extern	bool	id_password;  
extern	bool	MessageOn;
extern	char	Message[MAX_STRING];

extern	RECT	rScreenRect;      
extern	int		YouGotItemFlag;

extern	int		MouseItemInRectOn;	
extern	POINT	MouseItemInRect;

extern	int		cRMoveMenu;       
extern	int		cLMoveMenu;       
extern	int		fRMoveMenu;   
extern	int		fLMoveMenu;	

extern	int		nPartyOther;
extern	int		nPartyState;
extern	int		nPartyShow;	

extern	struct InterfaceFlag	main_interface;

extern	int RectImageOn;		
extern	int nBankTemp;			

extern	int WearAble[9];
extern	int HandBackUp;
extern	int HandBackUptype;
extern	int HandItemOn;

extern	int aMagicItem[3][4][3][5];
extern	ItemAttr	aItemInventoryFull;		
extern	int SkillInventory[2][2][4];
extern	int Skill_Know_Inv[3][3];
extern	int aSellBuyItemPos[MAX_BUY_ITEM*3*5];
extern	int aSellBuyItemMax;

extern	bool	LeftShiftOn; 

extern	int		AbilityToInt[20];
extern	ItemAttr aSellBuyItemAttr[MAX_BUY_ITEM*3*5];
extern	int		storelistindex ;

extern ItemAttr itemMallAttr[MAX_BUY_ITEM*3*5];
extern int itemMallMax;

extern	bool	is_buymenu_On;
extern	int		motherSkillImageNo[2];
extern	int		motherSkillImageType[2];  // 스킬 배우기에서 제목판 이미지 타입
extern	int		SkillInventory[2][2][4];

extern	Spr		ch_armor_image_after[7][80];
extern	Spr		ch_body_image_after[7][80];
extern	Spr		ch_line_image_after[7][80];
extern	Spr		ch_armor_image_after_b[7][80];
extern	Spr		ch_body_image_after_b[7][80];
extern	Spr		ch_line_image_after_b[7][80];
extern	Spr		spr2[MAX_IMAGE2];	

extern	int		nItemOther;
extern	K_ITEM	i_DivideItem;		// 아이템의 속성와 위치가 있다.
extern	const int y_ItemSubMenu[];//021014 lsw

extern	int		memorize[12];

extern void PutMapBright2( LPVOID dest, LPVOID alpha);
//===================================================================================================================================
extern void kein_TextPut(int x, int y, char *sSource);
extern void FieldTypeTextPut(int mx,int my,int x,int y,char *temp);  
extern void LoadMenuData(int nNum);                 
extern void FreeMenuData(int nNum);                 
extern void PutImageCenter(int mx, int my, int x, int y, int nImageNumber);
extern inline BOOL MouseInRectCheak(int x, int y, RECT Box, BOOL flag = TRUE );
extern void SetCharacterData();  
extern void SetRect(RECT &rect,int left, int top, int right, int bottom);
extern void MoveTextPut(int x, int y, char *sSource, int type=0);

extern void SendBankInfo(int type,  DWORD money );
extern void RecvBankDeposit(DWORD);
extern void RecvBankRepayment(DWORD, DWORD, DWORD);
extern int	YesOrNo( char *s, char *title );
extern char	SpaceBarPress();
extern	void	CountingByKeyboard( int input_key, int &count, int plus );

extern void		ItemRead();
extern void		PutItemIcon(int x, int y, int item_number, int delay=0, int effect = 2, int is_not_equip = 0);
extern void		PutItemIconAni(int x, int y, int item_number);
extern int		GetItemAttr(int item_id, int attr);
extern char		*GetItemName( int item );

extern void		SendMoveItem( int item_no, POS s, POS t, bool sound = true);
extern void		GetItemAnimationNumber(int num, int &a, int &b, int &c, int &d, int &e, int &f, int &g);

extern bool		ReadSkillTable( );
extern bool		ReadHouseTable();
extern bool		LoadItemJoinTable();

extern void		PutMagicIcon( int x, int y, int num, int flag=0, int type=2 );
extern void		PutSkillIcon( int x, int y, int num, int flag=0, int flag2=2 );
extern void		ChangeSkillMenuInventory( int x );

extern	BOOL	MenuFocus(int menu);
extern	inline void RightClose();
extern	bool	IsLeftWindowOpen( );
extern	bool	IsRightWindowOpen();
extern	void	AbilityOpenWhenLevelUp();
extern	void	SetPartyMenu( int type );

extern	void	ChangeItem( int start );	// 임시 아이템 바꾸기 
extern	void	ItemSound( int item );
extern	void	EquipSound( int item_no );

extern	int		GetItemValueBuy( int item_id );
extern	int		GetItemValueSell( ItemAttr item );

extern	void	GetLearnSkillInv( int type, int start = 0 );
extern	void	GetLearnKnowlageInv( int start = 0 );
extern	int		mother[32][2];
extern	bool	IsLearnSkill( int m );
extern	void	MagicSetting();

extern	int		CalcTextToPage( int width, int height, char *source );
extern	void	BbsTextPut( int x, int y, int width, int height, char *source, int page );
extern	void	BbsTextPut2( int x, int y, int width, int max_line, char *source, int page );
//extern	void	CallOkCancelMessageBox( int menu, int x, int y, char *source, bool ok_cancel=true, int param = 0 );
extern	void	CallItemMessageBox( int menu, int item_num, char *source );
extern	void	MenuButtonRightExplain( int x, int y, int &count, char * title );
extern	void	MenuButtonLeftExplain( int x, int y, int &count, char * title );
extern	void	LeftMenuClose();
extern	int		dw_TotalImageSize;
extern	void	LoofSetHeroAbility( int ability[] );

//extern	void	SendReCallItemFromServer( int type );

extern	void	FreeChImage();
extern	void	FreeOpenningSpr( BYTE **buf );

extern	void	SetItemMakeOk( ItemAttr item );

extern	int		UseItemByRbutton(POS pos_s, ItemAttr &item_attr);						// 아이템을 오른쪽 버튼으로 클릭했을 때

extern  int		Call_GM_2ND_Skill_Menu();//021014 lsw
extern  int		Call_Skill_Master_Menu();//021111 lsw
extern  int		Call_MERCHANT_REGISTER_Menu();//021126 lsw
extern  int		Call_EXCHANGE_BOND_MONEY_Menu();

extern	void	SetLearnItemData( int menu, ItemJoinSelect list[]);	//아이템 배우기 리스트 배열 셋팅
extern	int		GetResultItemBySkill( int skill, int item[10] );	// 그 스킬로 만들수 있는 아이템 목록 가져오기 
extern	bool	FindLearnItem( int item );							// 내가 배운 아이템인지 판단
extern	ItemAttr *GetItemByPos( POS item );
extern	bool	DivideItem( K_ITEM *item, int much );
extern	int		NotNeededMouseLButtonClick( void );		// 마우스가 활성화된 메뉴 안에 있는지를 체크
extern	int		GetWeight( ItemAttr item );
extern	int		SearchLearnItem( int item );			// 내가 배운 아이템위치 찾기
extern	void	PrintTextOnMenu2();
extern	void	PrintTextOnMenu();


extern Spr	openhouse[MAX_HOUSE];
extern Spr	closehouse[MAX_HOUSE];
extern void LoadHouseImage( int is_open );
extern void FreeHouseImage( );
extern void PutHouseImage( int x, int y, int num, bool is_open );
extern void LoadHouse( );


///  매직 배우기 용 메소드들...

extern int SearchNextMagicNumByClass( int num, int cls );		// 어떤 매직 다음의 같은 계열의 마법을 찾는다.
extern int GetMagicMaxByClass( int cls );						// 그계열의 총 매직 수를 구한다.
extern int GetLearnableMagicMaxByClass( int cls );				// 그 계열에서 내가 배울 수 있는  마법 수를 구한다. (이미 배운 마법 포함 )
extern bool CanLearnMagic( int num );							// 그 마법을 배울수 있나?

extern MagicByClass	magic_by_class;
extern void GetDivedeMagicByClass( );			// 계열당 배울수 있는 마법... 셋

extern void ServerConnectMenuClose();	// 서버와 관련이 있는 메뉴를 닫을 때..

extern DWORD GetMoneyByItem( );		// 인벤토리 창에 있는 돈 계산
extern DWORD GetNewMoneyByItem( );
extern int SearchCoinItem( int &a, int &b, int &c);
extern int SearchNewCoinItem( int &a, int &b, int &c);
extern int SearchItemPosByNumber(ItemAttr *item, int &a, int &b, int &c);
extern int SubtractMoney( DWORD money );
extern int AddMoney( DWORD money );
extern void SetMemorizeArray( int num );


extern int DoMemorize( int magic_num );
extern void InitMemorize( );	// 메모라이즈 초기화

extern int MemorizeYet( int magic_num );
extern int GetNoLearnMagic( int cls, int &magic_num );	// 못배우는 마법 이름과 그 이유를 알아오는 함수	// magic_num으로 마법 번호를 넘겨 준다.
extern void CheckPartyFace( );		// 파티원들이 있는 지를 체크해서 화면에 뿌려준다.
extern void SetExistMeOfParty( );		// 파티원중 나는 항상 조인한 상태다..

extern int SearchMyPartyOfMySight( LpCharacterParty party );	// 내 시야에 파티원이 있느냐?
extern void SetHangulFont( int type );
extern void ViewSmallMapCheck();

//extern	K_ITEM give_item;	// 교환용 아이템중 내가 주는 아이템
//extern	K_ITEM recv_item;	// 교환용 아이템중 내가 받는 아이템 

extern inline int CheckMouseInMenu( int i );
extern void DropItemSound( int item );		// 아이템 땅에 떨어트리는 소리
extern inline void LeftClose(); // 왼쪽 창 모두 닫기

extern int dexDefenseCor( int dex );
extern void MenucheckWhenHeroDie( );	// 죽었을때 메뉴체킹
extern void SendSmileFace( int smileno );
extern bool g_bMagicSelect;
extern int isCrashSmallMapByOther();
extern void CheckFocusMenu();
extern void GrobalValueSeet();
extern int CanLearnMagicByCount( int num );
extern void InitPartyState( );

extern void LoadSmallMenuImage2( int num );		// 추가 메뉴 이미지 로딩및 해제..
extern void FreeSmallMenuImage2( );

extern void FreeMagicIcon( );

extern int GetBelieveGodByMenu( int menu, char *God );		// 신전 메뉴에서 믿는 신으로...
extern void CallGodMenuMessage( char *msg, int believe );
extern inline int GetMenuNumByBelieveGod( int god );
extern void PutSmallmenuImage(int mx, int my, int x,int y, int image_number, int type);
extern void FreeSmallMenuETC();
extern int QuickKey( int menu, HWND active_hwnd = NULL);


extern bool bCallGodMeetingMenu;
extern Spr *GetSprOfMenu( int type, int imagenumber );
extern int GetHourToRate( int h );

extern void PrintQuickKey( char *text);
extern int GetMoveMenuDefineByMenuNumber( int menu, int is_open );
extern void CheckAutoDivideMoney( ItemAttr *item );

extern int LoadMagicName( );

//extern int GetAbilityMax( int ability_type );

extern void OpenRecvEarMessageMenu();
extern void CloseRecvEarMessageMenu();

extern int ExistBox( DWORD box_id );
extern int GetItemAttrWeapon( int item );		// 장착가능한 것중에 정말 장착이 가능한지 확인하는 것.
//extern int ExplainItemBySelectItemAfter( int x, int y, int item_number, char explain[5][40] );
extern void GetSupperItem( int god );
extern int ViewSmallMap2( int x, int y);
extern void SetSmallMapMenu( );
extern void PrintNoteMeeting( int g_Note_Meeting );
extern void CheckErrorMessage( LPSTR str );
extern int OnlyAlphabet( char *str, int len );
extern int OnlyNumber( char *str, int len );
extern void PutDot( int x, int y, WORD color );

extern int ExplainBackView( int &x, int &y, int count );	// 찍을 좌표를 가져간다.
extern void ShowExplain();							// 각종 설명문 보여주기 

extern void CheckMagicExplain( int magic_no );			// 매직 설명
extern void CheckSkillExplain( int skill_no );			// 스킬 설명

extern int ItemExplainAfterMenuDisplay( int prev );			// 나중에 설명을 띄운다.
extern void SetMenuExplain( int type, int menu, int start_field );

extern void ExplainMenuSet( char *temp );
extern int InsertIntArray( int node, int *int_array, int &curr_count, int array_max );
extern int DeleteIntArray( int node, int *int_array, int &curr_count, int array_max );
extern int IsJustViewMenuWhenDead( int dest_menu );
extern void PutMagicIconAll();
extern int QuickKeyFirst( int menu );
extern int IsExistNodeToArray( int node, int *int_array, int curr_count );
extern void SetLearnItem();
extern int ConvStoreListToMenu( int prev, bool is_store );

extern void lsw_menu_set( );//010830 lsw

extern bool CanLearnItemByNeedSkill( int join_no );

extern void SmallMapClose();
extern void GetTatics();
extern int SetQuickSkill( int *index, int start, int type );

extern void LoadFilterWord( );
extern bool CheckNumber( char *text, int len );		// 숫자 이외의 값이 있는지를 체크한다.	있으면 true 없으면 false
extern void kh_menu_set( );
extern void ItemSoundOfDropItem( int item_no );
extern void StopWaveOfMenu();
extern void GetNationBBS( char *filename );
extern void OpenFightMapSignMenuOpen( int tema[] );
extern int	IsSameNation( int type, LpCharacterParty lpParty );
extern void LoadTextDefault( char *filename, char *text, int text_max );
extern int CheckRegistCandidate( );
extern void CallMessageBox(BOOL bBig, int x, int y, char *source, BOOL bType=FALSE);
extern void MessagePut();

extern void	LottoMenuSet(); //soto-030425
extern void	LuckyMenuSet();	//soto-030429
extern void	LuckyEventSet(); //soto-030502

extern void	LuckyMenuOpen();//soto-030429
extern void	LottoMenuOpen();//soto-030429
extern void	WinnerMenu();//soto-030502

//020530 lsw
#define MAX_MAGIC_MEMORY	8//020420 lsw
#define QM_MAGIC			1//QM_ QuickMemory
#define QM_SKILL			2
#define QM_COMBATSKILL		3
typedef struct QUICKMEMORY
{
	char OwnerName[21];//020701 lsw
	int iType;// 1 마법 2 스킬 3 전투스킬
	int iNo;//실질 넘버
	int iBuffer;
	void SetQuickMemory(const char* name ,const int iTypeValue,const int iTypeNo)
	{
		if(name)
		{ 
			const int iLen = strlen(name);
			if(21 < iLen|| 0 >= iLen )
			{
				Clear();
				return;
			}
			strcpy(OwnerName, name);
			iType = iTypeValue;
			iNo = iTypeNo;
		}
		else
		{
			Clear();
		}
		return;
	}
	void Clear()
	{
		iType	= 0;
		iNo		= 0;
	}
}t_QuickMemory,*LPQUICKMEMORY;

extern t_QuickMemory SaveMemory[MAX_MAGIC_MEMORY];		// 단축키로 기억시켜둔 목록 //020530 lsw
extern t_QuickMemory HotKeyMemory;	// 버튼 한개로 사용 하는 Ram 같은 변수
extern void ClearQuickMemory();
extern void DisplayQuickMemory(const int iType, const int iTargetNo, const int iX, const int iY);
extern void SetQuickMemoryByKeyInput(const int iType, const int iNo, const int iBuffer = 0);

extern int ViewTipsFlag;
extern void ExplainItem2( int x, int y, ItemAttr *olg_item );
extern void LoadManList();
extern int GetItemExplainLine( ItemAttr *olg_item );
extern void GetWORDOfDWORD( DWORD dword, WORD &word1, WORD &word2 );

extern int bar1_r, bar1_g, bar1_b;
extern int bar2_r, bar2_g, bar2_b;
extern int g_RecvLoanInfo;


extern int g_BattleStatusView;
extern char g_szMoneyName[];//021014 lsw

/////////////////////////////////////////////
#define AT_JOIN_COLOSSUS		1// 콜로서스 참여에 동의한다.
struct k_assent		// 유저가 뭔가에 동의할때를 대비한 저장 변수
{
	int		type;
	DWORD	time;		// 시간

	union 
	{
		char	data[512];
		WORD	default_word;
	}u;

	k_assent()
	{
		type = 0;
	}
};

/////////////////////////////////////////////
class CGuildItem
{
public :
	int m_Max;
	int old_page;
	DWORD *m_pIndex;
	ItemAttr *m_pItem;
	DWORD	m_BoxMoney;

public :
	CGuildItem()
	{
		m_pIndex = 0;
		m_pItem = 0;
		clear();
	}
	int clear()
	{
		m_Max = 0;
		if( m_pIndex ) delete m_pIndex;
		if( m_pItem ) delete m_pItem;
		m_pIndex = 0;
		m_pItem = 0;
		old_page = 0;
		m_BoxMoney = -1;		// -1일땐 가져오기 루틴을 실행시키지 않는다.

		return 1;
	}
	int clear( int ct )
	{
		memset( &m_pItem[ct], 0, sizeof( ItemAttr ) );
		m_pIndex[ct] = 0;
		return 1;
	}
	~CGuildItem()
	{
		clear();
	}
	DWORD GetIndex( int ct )		// 인덱스값 가져오기
	{
		if( !m_pIndex ) return 0;
		if( ct > m_Max ) return 0;
		return m_pIndex[ct];
	}
	int InsertItem( int ct, int index, ItemAttr *item )	// 아이템 삽입
	{
		if( !m_pIndex ) return 0;
		if( ct > m_Max ) return 0;
		m_pIndex[ct] = index;
		m_pItem[ct] = *item;
		return 1;
	}
	int SerchFirstIndex( int find_index )	// 특정 인덱스값을 가지고 있는 번호를 찾는다.
	{
		if( !m_pIndex ) return -1;
		for( int i=0; i<m_Max; i++ )
		{
			if( m_pIndex[i] >= find_index ) return i;
		}
		return -1;
	}
	ItemAttr *GetItemByParm( int ct )		// 파라메터로 아이템 가져오기
	{
		if( !m_pIndex ) return 0;
		if( ct > m_Max ) return 0;
		return &m_pItem[ct]; 
	}
	int GetMax() { return m_Max; }
};

/////////////////////////////////////////////
#define MAX_POTAL_MAP 40

class CPotalMenuUnit
{
public :
	char map_name[50];
	int map_number;
	int money;
	int type;
	int img_type;
	int level;
	char *explain;
	CPotalMenuUnit()
	{
		explain = 0;
	}
	int clear()
	{
		if( explain ) delete explain;
		explain = 0;
		return 1;
	}
	~CPotalMenuUnit()
	{
		clear();
	}
};

class CPotalMenu
{
	CPotalMenuUnit m_Map[MAX_POTAL_MAP];
	int m_Count;
public :
	~CPotalMenu()
	{
		clear();
	}
	CPotalMenu()
	{
		m_Count = 0;
	}
	int clear()
	{
		m_Count = 0;
		for( int i=0; i<MAX_POTAL_MAP; i++ )
		{
			m_Map[i].clear();
		}
		return 1;
	}
	int GetMax() { return m_Count; }
	int GetMapNumber( int num ) { return m_Map[num].map_number; } 
	int GetType( int num ) { return m_Map[num].type; }
	int GetImgType( int num ) { return m_Map[num].img_type; }
	int GetMapMoney( int num ) { return m_Map[num].money; }
	char *GetMapName( int num ) { return m_Map[num].map_name; }
	char *GetMapExplain( int num ) { return m_Map[num].explain; }
	int GetMapLevel( int num ) { return m_Map[num].level; }
	int LoadPotalMap( int nation );
	int SetExplain( int num, char *str, int size )
	{
		m_Map[num].clear();
		m_Map[num].explain = new char[size+1];
		strcpy( m_Map[num].explain, str );
		return 1;
	}
};


/////////////////////////////////////////////
struct CFriend
{
	int m_bLogin;		// 접속 중이다 아니다.
	char name[20];
	RECT rect1;		// 귓속말
	RECT rect2;		// 메일

	CFriend()
	{
		clear();
	}
	int clear()
	{
		m_bLogin = 0;
		name[0] = 0;
		SetRect( rect1, 0, 0, 0, 0 );
		SetRect( rect2, 0, 0, 0, 0 );
		return 1;
	}
};

const int MAX_FRIEND_GROUP = 30;
class CFriendGrup
{
public :
	int count;
	CFriend m_Friend[MAX_FRIEND_GROUP];
public :
	CFriendGrup()
	{
		count = 0;
	}

	int SetFriendName( int cn, char *name );
	int SetFriendLogin( int cn, int login );
	char *GetFriend( int cn, int &login );
	int InsertFriend( char *name, int login );
	int clear();
	int SerchName( const char *name );//030504 lsw
	int DeleteName( int cn );
	int SetRect( int cn, int type, int left, int top, int right, int bottom );
	char *CheckRect( int cn, int type, int x, int y );
};

/////////////////////////////////////////////
#define MAX_RUNE 25

struct CRuneExplain
{
	char m_szTitle[4][MAX_RUNE+1][128];
	char m_szExplain[4][MAX_RUNE+1][2048];
	string	m_szExplainTitle[4];
public :
	CRuneExplain() 
	{ 
		memset( m_szTitle, 0, sizeof( char )*4*(MAX_RUNE+1)*128 ); 
		memset( m_szExplain, 0, sizeof( char )*4*(MAX_RUNE+1)*2048 );
		LoadLuneText();
	};

public :	
	int LoadLuneText();
	int LoadLuneText2( int lune_num );
	LPCTSTR GetTitleExplain( int lune_num );
};

struct k_RuneMenu
{
	int m_nRuneNum;				// 룬 번호
	int m_nLeftRuneImage;		// 왼쪽에 찍을 룬 번호
	int m_nRightRuneImage;		// 오른쪽에 찍을 룬 번호
	int x;
	int y;
};

class CRuneMenu
{
public :
	int m_nQuestStep;
	int m_nSelectRune;
	k_RuneMenu m_stRune[MAX_RUNE];	
public :
	CRuneMenu();
	int LoadRuneMenuXY();
};

struct k_guild_degree
{
	char m_pszGuildDegree[5][20];
};
struct k_guild_regist_explain
{
	char m_pString[100][256];
};
struct k_char_rgb
{
	int		body_r;			//  외모 색
	int		body_g;					
	int		body_b;
	int		cloth_r;
	int		cloth_g;
	int		cloth_b;
	
	k_char_rgb() { set(); }
	~k_char_rgb();
	int set();
};

struct k_view_nation_money
{
	DWORD money3;
	DWORD money4;
	DWORD money6;
	k_view_nation_money()
	{
		money3= 0;
		money4= 0;
		money6= 0;
	}

};

class CNameOfGuildMemberList
{
public:
	int m_iDegree;		// 등급
	bool m_bRecv;		// 이미 받은적 있다. 
	int m_iTotal;
	int *m_bpIsConnect;		// 접속해 있는 캐릭인지 확인
	string	*m_lpszName;

public :
	CNameOfGuildMemberList()
	{
		m_lpszName = 0;
		m_bpIsConnect = 0;
		clear();
	}
	int clear()
	{
		if( m_lpszName ) delete[] m_lpszName;
		if( m_bpIsConnect ) delete[] m_bpIsConnect;
		m_bRecv = 0;
		m_iTotal = 0;
		m_bRecv = 0;
		m_lpszName = 0;
		m_bpIsConnect = 0;
		return 1;
	}
	~CNameOfGuildMemberList()
	{
		clear();
	}
};
class CGuildMemberName
{
public :
	int m_nSubMasterNumber_degree;		// 서버 마스터가 속해 있는 곳의 등급
	int m_nSubMasterNumber;				
	CNameOfGuildMemberList m_degree[5];
public :
	int clear()
	{
		m_nSubMasterNumber_degree = -1;
		for( int i=0; i<5; i++ ) m_degree[i].clear();
		return 1;
	}
	~CGuildMemberName()
	{
		clear();
	}
	int GetRecv( int degree )
	{
		return m_degree[degree].m_bRecv;
	}
	int SetRecv( int degree )
	{
		m_degree[degree].m_bRecv = 1;
		return 1;
	}
	int SetCount( int degree, int count, k_get_guild_memeber_list_name *data );
	int GetCount( int degree )
	{
		return m_degree[degree].m_iTotal;
	}
	const char *GetName( int degree, int count )
	{
		if( count >= GetCount(degree ) ) return NULL;
		return m_degree[degree].m_lpszName[count].c_str();
	}
	int GetIsConnect( int degree, int count )
	{
		if( count >= GetCount(degree ) ) return NULL;
		return m_degree[degree].m_bpIsConnect[count];
	}
	bool GetSubMasterNumber( int &degree, int &number )
	{
		degree = m_nSubMasterNumber_degree;
		number = m_nSubMasterNumber;
		if( degree == -1 ) return 0;
		return 1;
	}
};


class CGuildHouseInfo
{
public:
	int m_max;
	k_guild_house_info_unit *m_data;

public:
	CGuildHouseInfo()
	{
		m_max =0 ;
		m_data = 0;
	}
	CGuildHouseInfo( int max )
	{
		m_max = max;
		if( !max ) return;
		m_data = new k_guild_house_info_unit[max];
	}
	~CGuildHouseInfo()
	{
		if( m_data ) delete [] m_data;
	}
	int CopyInfo( int index, k_guild_house_info_unit *data )
	{
		if( !m_data ) return 0;
		if( index >= m_max ) return 0;
		m_data[index] = *data;
		return 1;
	}
	bool IsExistGuild( int guild_code )
	{
		for( int i=0; i<m_max; i++ )
		{
			if( guild_code == m_data[i].guild_code ) return true;
		}
		return false;
	}
	int GetGuildCode( int index ) { return m_data[index].guild_code; }
	bool CheckSignet( int index ) 
	{
		if( GetItemMuchByMuchItem( IN_BLUE_SIGNET ) < m_data[index].blue ) return false;
		if( GetItemMuchByMuchItem( IN_RED_SIGNET ) < m_data[index].red ) return false;
		return true;
	}
};

///////////////////////////////////////
// m_PartySelect 변수에 사용할 값 정의
#define PS_EXP			10
#define PS_TACTIC		20
#define PS_MAGIC_TAC	30
//////////////////////////////////////


// 020428 YGI acer
////////////////////////////////////////////////
// 배열번호를 참조 했는지 않했는지를 파악하는 클레스 선언
class CAlreadyCheck
{
private : 
	int m_nProcessId;
	CAlreadyCheck *next;
public :
	CAlreadyCheck()
	{
		m_nProcessId = 0;
		next = 0;
	}
	~CAlreadyCheck()
	{
		if( next ) delete next;
	}
	bool CheckProcessId( int check_id )
	{
		CAlreadyCheck *temp = next;
		while( next )
		{
			if( m_nProcessId == check_id ) return true;
			temp = temp->next;
		}
		return false;
	}
	int AddProceessId( int add_id ) 
	{
		CAlreadyCheck *prev = this;

		CAlreadyCheck *temp = next;
		for( ; temp; prev = temp, temp = temp->next )
		{
			if( temp->m_nProcessId == add_id ) return false;
		}
		CAlreadyCheck *data = new CAlreadyCheck;
		data->m_nProcessId = add_id;
		prev->next = data;
		return true;
	}
};

class CScenarioTrapInfo
{
public :
	int m_nTotalTrap;
	int m_nActiveTrap;
	int m_nWaitTimeOfDungeon;
	int m_nWaitTimeOfTrap;
public : 
	CScenarioTrapInfo()
	{
		memset( &m_nTotalTrap, 0, sizeof(int)*4 );
	}
};
// thai2 YGI
#include "kein_stl.h"
typedef TKeinMap<POINTS> CMapDinamicTile;
// 021107 YGI--------------------
extern DWORD g_curr_time;
class CEventMoveMap
{
public : 
	k_event_move_map_rep	data;
	DWORD					m_dwRecvTime;

public :
	CEventMoveMap()
	{
		memset( &data, 0, sizeof( k_event_move_map_rep ) );
		m_dwRecvTime = 0;
	}
	void Set( k_event_move_map_rep *pData )
	{
		data = *pData;
	}
	CEventMoveMap operator = ( k_event_move_map_rep &data )
	{
		m_dwRecvTime = g_curr_time;
		Set( &data );
		return *this;
	}
	bool IsCanMove()
	{
		if( !m_dwRecvTime ) return 0;
		if( g_curr_time - m_dwRecvTime > 120 ) return 0;		// 120 분이 지났다.
		return 1;
	}
};

////////////////////////////////////////////////////
class CMenuVariable		// 메뉴 한군데에서만 사용할 전역 변수 모임
{
	//021030 YGI
public : 
	enum
	{
		ACTIVE_EVENT_OBJECT = 0x01,
		ACTIVE_EVENT_SOUND = 0x02,
	};

public :
	CRuneMenu *m_pRune;
	int m_bLoadGuildMark;
	int	m_GuildSaveId;
	char *m_pSubGuildMaster;
	k_guild_degree *m_pGuildDegree; // 길드 직책 입력 임시 변수
	k_guild_regist_explain *m_pGuildRegistExplain;
	//char (*m_ppGuildRegistExplain)[256];
	int	m_bGuildChatOn;
	CRuneExplain	*m_clsRuneExplain;
	k_char_rgb		*m_stRGB;
	bool			m_bChangeColor;
	k_view_nation_money	*m_stNationMoney;
	bool			bCheckSalvation;
	MakeItem		*pGambleItem;
	BYTE			m_GambleRune;
	int				m_GambleRand;		// 룬돌아가는 최소한의 방법
	CGuildMemberName m_GuildMemberName;
	char			*m_szGuildPublicNotics;
	CFriendGrup		*m_FriendGrup;
	ItemAttr		*m_TodayGambleItem;
	char			m_PartySelect;
	CPotalMenu		*m_PotalMap;
	char			m_bOXTime;
	DWORD			m_OXTime;
	CGuildItem		*m_pGuildItem;
	POS				*m_pPutGuildItem;
	int				m_nCheckSubMaster;
	string			m_szFileTextPut;
	k_nation_item	m_nation_item;
	k_assent		m_assent;
	CGuildHouseInfo	*m_pGuildHouseInfo;
	CAlreadyCheck	*m_CheckProcess;
	//acer5
	k_char_info		*m_pOtherChar;
	// 020620 YGI
	CMapDinamicTile *m_pDinamicTile;
	CScenarioTrapInfo	*m_pScenarioTrapInfo;
	k_scenario_trap_key	*m_pScenarioTrapKey;		// 020701 YGI
	bool			m_bTradeON;		//020808 YGI 	

	k_event_object_sound	*m_pEventObjcet;		//021030 YGI
	CEventMoveMap			*m_pEventMoveMap;		// 021107 YGI
			
public :
	CMenuVariable()
	{
		m_bTradeON = 1;		//020808 YGI 
		m_pGuildDegree = NULL;
		m_pSubGuildMaster = NULL;
		m_pGuildRegistExplain =NULL;
		m_bGuildChatOn = 1;
		m_bLoadGuildMark = 0;
		m_pRune = NULL;
		m_clsRuneExplain = 0;
		m_stRGB = 0;
		m_bChangeColor = 0;
		m_stNationMoney = 0;
		bCheckSalvation = 0;
		pGambleItem = 0;
		m_GambleRune = 0;
		m_GambleRand = 0;
		m_GuildMemberName.clear();
		m_szGuildPublicNotics = 0;
		m_FriendGrup = 0;
		m_TodayGambleItem = 0;
		m_PartySelect = 0;
		m_PotalMap = 0;
		m_bOXTime = 0;
		m_OXTime = 0;
		m_pGuildItem = 0;
		m_pPutGuildItem = 0;
		m_nCheckSubMaster = 0;
		m_szFileTextPut = "";
		m_nation_item.clear();
		m_pGuildHouseInfo = 0;
		//acer5
		m_pOtherChar = 0;
		// 020620 YGI
		m_pDinamicTile = 0;		
		m_pScenarioTrapInfo = 0;
		m_pScenarioTrapKey = 0;		// 020701 YGI
		m_pEventObjcet = 0;		//021030 YGI
		m_pEventMoveMap = 0;
	}
	~CMenuVariable()
	{
		clear4();
		clear2();
	}
	int clear();		// 메뉴가 종료 될때 사라진다.
	int clear2();		// 게임이 다시 시작될때 사라진다. clear3() clear() 를 포함한다. 
	int clear3();		// 맵이 바뀔때.. clear() 를 포함하고 있다.
	int clear4();
	int LoadingGuildRegistExplain( char *szFileName );
	int GetEventSound();		//021030 YGI
};


extern CMenuVariable g_Menu_Variable;
extern int CallGuildChangeInfo( );
extern DWORD ReturnBlendRGB( int r, int g, int b );

extern char *ChangeString( char *str, char sor, char tag );
extern char *ChangeString2( char *str, char *sor, char *tag );
extern DWORD GetNationMoney( int nation);
extern void LoadMemorize();
extern void SaveMemorize();


#include "menufunctions.h"


#endif
