#ifndef	__SmallMenuSet_h__
#define	__SmallMenuSet_h__

extern void SmallMenuSet();
extern int GetCurrentBankItemPay();	// 현재 은행의 무게에 따른 보관료

extern int GetBankItemWeight();		// 무게에 따른 보관료....

extern	SCharacter SCharSource;
extern	void	PrintDate(SMENU* SubMenu , int j, DWORD time, int width);
extern	void	SmallMenuChecking();
extern	void	MenuSubProcessType(SMENU *SubMenu );								//메뉴 구성요소에 타입을 판별해서 실행
extern	void	PutMouseItem();													//마우스가 아이템을 얻으면 마우스를 따라서 아이템을 찍어준다
extern	void	kein_KeyProc();													// 스페이스 바를 누르면 모든 메뉴는 사라짐. (독립적인 메뉴 덧붙여야함.)
extern	void	ChangeEquip( LPCHARACTER lpCharacter, ItemAttr *equip_item, int type);		// 장착화면을 바꿀때.
extern	void	MoveMenu();														// 메뉴 움직일 때 
extern	int		ItemDrop();															// 아이템 드롭 시킬때
extern	void	PrintMoney(SMENU* SubMenu, int j, DWORD result, int BoxSize);		// 박스 안에 돈 출력할때(천단위마다 , 표시)

extern	void	ItemExplain(ItemAttr item);
extern	void	ItemExplain(int item);
extern	void	PrintDate(SMENU* SubMenu , int j, DWORD time, int width);
extern	void	PrintText(SMENU* , int, char*, int ); // 문자열 출력
extern	int		IsMagicWindowOn();		// 매직 윈도우가 열렸는지..
extern	void	MagicExplain(int num);												// 매직 아이콘 설명

extern	void	DoButtonCheckOfMenu( int i, int j );
extern	void	DoLButtonDownOfMenu( int i, int j );
extern	void	DoRButtonDownOfMenu( int i, int j );
extern	void	DoRButtonCheckOfMenu( int i, int j );
extern	void	DoLButtonDoubleClickOfMenu( int i, int j );

extern	void	PutMouseItemCheck();
extern	void	InitMenu();				// 시작할때, 셋업을 상황에 맞춰 해야 할 경우 
extern	void	Small_KeyProc() ; // 스몰 메뉴가 떴을때 사용할...
extern	char	*GetMotherSkillName( int s );

extern	void	PutCharImage( int x, int y, int num, int flag=0, int size = FS_SMALL, int flag2 =2, int Gender = 100);
extern	int		IsSameCharOfParty( int type, LpCharacterParty ch );		// 파티를 맺을때 같은 사람이 있는지를 판단.
extern	int		IsCheckLevelOfParty( int type, LpCharacterParty man, int pos );
extern	int		GetTextImageWidth( char *buf );	// 문자열의 그림 이미지  총 넓이 구하기

extern BYTE *open_spr_buf;
extern void	LoadOpenningSpr( char *filename, BYTE **buf, Spr *spr, int nMaxCount = 0 );		// 020701 YGI

extern int		chat_scroll_count;

extern BYTE		*divine_gauge;
extern Spr		spr_divine_gauge[45];

extern BYTE		*buf_bird;
extern Spr		spr_bird[5];

extern void ConvertSkillTable( );

#define MT_DUAL_CHANGE 100
#define MT_DUAL_DIVIDE 200

extern int CallDualInterFace(int type);
extern void DoDualOKButton(int type, int select);
extern int CallInitAbility();
extern void DoInitAbility();
const int HOTKEY_ICON_POS_X = 800-42;
const int HOTKEY_ICON_POS_Y = -(42*2)+1;

extern bool SendChatWisper(const char *name, char *msg);
extern bool SendChatNormal(char* msg);
extern bool SendChatGuild(char *msg );//020515 lsw
extern void SetChatTarget(const int iType);
extern bool IsChatBoxLock();
extern int GetChatTarget();//현재 채팅 상대 가지고 오기
extern void DoQuickmemory(const int iQMNo);//020530 lsw
extern void DoQuickmemoryByKeyInput(const int iInputKey,bool bIsDikCheck);//020530 lsw



//메인 인터페이스 필드 번호
const int TODAY_NOTICE_FIELD = 16;	
const int COMBAT_SKILL_POINT_FIELD = 18;
const int PEACE_BATTLE_FIELD	=17;
const int MAGIC_FIELD =19;
const int INVENTORY_FIELD =20;
const int PARTY_FIELD =21;
const int OPTION_FIELD = 22;
const int SKILL_FIELD =23;
const int NATION_BBS_FIELD =24;
const int HOT_KEY_FIELD = 28;
const int FRIEND_FIELD =46;
const int MAP_FIELD =15;
const int MAIL_FIELD = 33;
const int STATUS_FIELD = 0;

extern void CallLoginMenu(const bool bForceLoginActive = false);//020710 lsw
extern void CallDelCharComform();//020802 lsw
#endif


