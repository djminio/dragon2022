#include "teambattle.h"

#ifndef __SMALLMENU_H__
#define __SMALLMENU_H__




#define	MAX_BBS_TITLE		10	// 게시판에 쓰일 한번에 읽어 들일 제목 개수

class  CBbsTitle
{
public :
	DWORD	m_Time;				// 시간
	char	m_Name[ 31 ];		// 올린 사람
	char	*sp_BbsTextTitle;			// 게시판에서 쓰일 제목
	short	m_Year;
	short	m_Month;
	short	m_Day;
	short	m_Number;
	bool	is_first;
	char	*m_szMessage;

public :
	CBbsTitle() { is_first = true; }
	void	SetBbsTitle( short no, DWORD time, char *name, char *title );


};

extern CBbsTitle	c_BbsTitle[ MAX_BBS_TITLE ];
extern char			s_BbsTextBuf[ MAX_STRING_ ];	// 게시판 본문


class CMenuStack			// ESC 키를 위한 메뉴가 들어간 순서를 기억
{
protected :
	int m_Menu[5];
	int top;

public :
	CMenuStack() { top = 0; }
	void DelMenu( int menu )
	{
		for(int i=0;i<top;i++)
			if(m_Menu[i] == menu)
			{
				if(i != top-1)
				{
					for(int k=i;k<top;k++)
					{
						m_Menu[k] = m_Menu[k+1];
					}
				}
				top--;
				return;
			}
		return;
	}
	void PutMenu(int menu)
	{
		DelMenu(menu);
		m_Menu[top] = menu;
		top++;
	}
	int GetMenu()
	{
		if(top == 0) return -1;
		top--;
		return ( m_Menu[top] );
	}
};

class CMagicOnCheck
{
protected :
	int		num;
	int		x;
	int		y;
public :
	int		type;

public :
	void SetMagicOn( int n );
	void SetMagicOn(int n, int a, int b) { num = n; x=a; y=b; }
	void SetMagicOn(int n, int a, int b, int t) { num = n; x=a; y=b; type = t;}
};
extern CMagicOnCheck	MagicOnFlag;


class SystemInfo
{
public:
	int		sound;
	int		music;
	int		mouse_speed;
	int		scroll_speed;
	bool	gore;				// 피튀기는 effect
	bool	counter;			// 상대와 나의 HP 표시
	bool	action;				// 행동 표현 문구
	bool	whisper;			// 

public:
	SystemInfo()
	{
		// option default	// 서버에서 받게 된다. 
		gore	= true;
		counter = true;
		action  = true;
		whisper = true;

		sound = 100;
		music = 100;
		mouse_speed = 100;
		scroll_speed = 100;
	}
};

#define		BLACKSMITH		0		// 대장간
#define		FARM			1		// 농장
#define		RESTAURANT		2		// 음식점
#define		HERBSHOP		3		// 약초점
#define		SHAMBLES		4		// 푸주간
#define		CANDLE			5		// 양초점
#define		SLOPSHOP		6		// 옷가게 
#define		ALCHEMY			7		// 활 화살
#define		WOODWORKING		8		// 목공소
#define		BOWCRAFT		9		// 활 화살
#define		SHIPYARD		10		// 조선소
#define		PUB				11		// 주점
#define		HOSPITAL		12		// 병원
#define		STORE			13		// 잡화점
#define		INN				14		// 여관
#define		MAGIC_GUILD		15		// 마법사 길드


#define		STORE_TOTAL		17


/*

class CDisease 
{
public :
	char	name[30];
	int 	money;



public :

}

*/
// 질병 테이블	//0104
typedef struct tag_disease_table
{
	short int	disease_number	;
	char		han_name[31] ;	
	char		name[31]	;
	short int	lv;
	short int	nut1_1;
	short int	nut2_1;	
	short int	nut3_1;	
	short int	probability_1	;
	short int	andor_1	;
	short int	checktime_1	;
	short int	contact_char	;
	short int	talk_char	;
	short int	fuck_char	;
	short int	contact_obj	;
	short int	eat_obj	;
	short int	nut1_2	;
	short int	nut2_2	;
	short int	nut3_2	;
	short int	probability_2	;
	short int	checktime_2	;
	short int	andor_2	;
	short int	dec_str	;
	short int	dec_dex	;
	short int	dec_con	;
	short int	dec_wis	;
	short int	dec_int	;
	short int	dec_movp	;
	short int	dec_char	;
	short int	dec_endu	;
	short int	dec_mor	;
	short int	dec_luck	;
	short int	dec_ws	;
	short int	dec_ps	;
	short int	recoverable	;
	short int	life	;
	short int	health	;
	short int	mp	;
	short int	dp	;
	short int	max_	;
	short int	damage	;
	short int	att_rate	;
	short int	depen_rate	;
	short int	ac	;
	short int	mad	;
	short int	poison	;
	short int	stony	;
	short int	sluggish	;
	short int	active	;
	short int	active_time	;
	short int	maintain_time	;
	short int	natural_recovery	;
	short int	nut1_3	;
	short int	nut2_3	;
	short int	nut3_3	;
	short int	probability_3	;
	short int	checktime_3	;
	short int	andor_3	;
	short int	price_heal	;
	short int	price_prevent	;

}DiseaseTable;

class CQuestSmallmapDot		// 스몰 메뉴에서 위치 지정
{
public :
	int map_num;
	int x;
	int y;
	int on;

public :
	void Setting( int num, int xx, int yy );
	void Setting( char *map_name, int xx, int yy );
	int operator--( int ){ on--; if( on< 0 ) on = 0; return on; }
};

extern WORD RGB16( int r, int g, int b );
class CColor
{
public:
	int r;
	int g;
	int b;
public:
	CColor() { Set(0,0,0); }
	CColor( int red, int green, int blue ) { r=red, g=green, b=blue;}
	void Set( int red, int green, int blue ) { r=red, g=green, b=blue;}
	int GetColorOne() { return( (r>>3)<<11 | (g>>3)<<6 | b>>3 ); }
	WORD GetRGB16() { return RGB16( r, g, b ); }
	int GetRGB() { return RGB( r, g, b ); }

};


typedef struct tag_CallNameByLv
{
	char name[20];
	char han_name[20];
}k_CallNameByLv;

extern int recv_nation_msg_count;				// 나라 메세지
extern char recv_nation_msg_text[MAX_MSG];
////////////////////////////
// 적 스켄 변수들
extern int scan_target_count;
extern k_x_y scan_target_xy[50];
extern int scan_target_view_time;

extern bool	g_FightMapStart;
extern int fight_map_acc_num[4];

extern DiseaseTable	disease_tbl[6];
extern int CheckHandleByKein( t_packet *p );
extern SystemInfo	system_info;
extern void LoadDiseaseTbl();
extern bool SearchStrStr( const char *szSource, const char *szWhite );
extern void FreeMenuData2( );
extern void SetVolumeBySleep( int volume );
extern char *FindSpaceWord( char *szText, int len );
extern CQuestSmallmapDot quest_dot;
extern CColor Lv_Color[256];
extern void LoadLvColor();
extern void LoadCallNameByLv();
extern bool IsASCII( char *szText );
extern void PutGuildImage( int x, int y, int guild_num );
extern void PutGuildLvByColor( int x, int y, int guild_code, int guild_master );

extern DWORD	jobColor[100];
extern void LoadJobLvColor();

extern LPDIRECTDRAWINFO	curr_direct_draw_info;
extern int CanGmRegist( int guild_count );
extern void ShowComeOutGM( char *name );

extern void GetSelectableJob();
extern inline int CheckNation( int nation );
extern char PosOfNation[][20];


#define MAX_SERVER_SET	20

struct CServerSet
{
	char server_name[20];
	char server_ip1[20];	
	char server_ip2[20];	
};

//////////////////////////////////////////////////

#define MAX_MENU_STRING		255

class CMenuString
{
public :
	char *str;
public :
	CMenuString() { str = NULL; }
	~CMenuString() { if( str ) delete str; }
	char *GetString() { return str; }
	void AddString( char *add );
};

class CRankLadder		// 10개 단위 묶음
{
public :
	int m_Page;
	int is_end;		// 이게 마지막인가?
	int recv;		// recv 받았는가?
	k_get_ladder_score_rank_unit	rank_data[10];	// 실제 게시판 정보
	CRankLadder *next;

public :
	CRankLadder() { next = NULL; is_end = 0; recv = 0; }
	~CRankLadder() { if( next ) delete next; }
	int GetRankLadderData( int page, k_get_ladder_score_rank_unit *&t, int &end );//021030 lsw
	int AddData( int end, k_get_ladder_score_rank_unit *dt );
	int SetFinish( );		// 요청했지만 더이상 데이타가 없다면 가장 마지막 데이타를 end로 처리한다.
	int SearchName( char *name );

};


extern CRankLadder	*g_lpRankLadderBBS;


extern CServerSet server_set[MAX_SERVER_SET];
extern int server_count;
extern int CheckSelectServer();

extern CMenuString		g_MenuStr[MAX_MENU_STRING];
extern void LoadMenuStr();

extern void SetChangeMapOfMenu();


extern void ViewSmallTip();
extern int ViewSmallTipsLoad( int n );


extern void LogFile( char *text );
extern char temp_menu_string[];
extern int LoadDefaultStringByMenu( int menu, char *&str );

extern char *ChangeString( char *str, char sor, char tag );
extern char *ChangeString2( char *str, char *sor, char *tag );
extern bool	g_SokSungFlag;

#define DIB_HEADER_MARKER	((WORD)('M'<<8) | 'B' )

#define SPR_COLOR_BYTE		2
#define MAX_ALIAS_BUFFER_X     200
#define MAX_ALIAS_BUFFER_Y     200
#define MAX_RAW_BUF_X         200
#define MAX_RAW_BUF_Y         200


typedef struct 
{  
	BYTE r;
	BYTE g;
	BYTE b;
}RGB24;

class CBitBuf
{
public :
//	WORD m_pRawBuf[MAX_ALIAS_BUFFER_X*MAX_ALIAS_BUFFER_Y];
	WORD m_pComBuf[MAX_RAW_BUF_X*MAX_RAW_BUF_Y];
	WORD m_pBmpBuf[MAX_RAW_BUF_X*MAX_RAW_BUF_Y];

	Spr	m_Spr;

	int m_iImageX;
	int m_iImageY;

public :
	int LoadBMP( char * file_name );
	int DeviceBmp16( WORD *pBmpbuf, WORD *compressbuf, WORD keycolor );
	//int DeviceBmp16( BYTE *aliasbuf, BYTE *compressbuf );
	int ConvertImgFromSurface( WORD *buffer, WORD *sour, int xl, int yl, int keycolor);
	//int ConvertImgFromSurface( BYTE *buffer, BYTE *sour, int xl, int yl, int keycolor );
	WORD RGB16(WORD r, WORD g, WORD b);
};



extern CBitBuf guild_mark_main;

extern int CallGuildExplain( int type );
extern void SaveGuildInfoEx( );

extern int g_OtherNationClick;
extern bool CheckNotClickNation();

extern	CHouse		*house;
extern	CMenuStack	MenuStack;

extern void CallGuildInfoMenu( int menu, short int guild_code, int if_guild, int info_type = 0 );
extern inline Spr *GetGuildSpr( int guildCode );
extern void LoadGuildListEx(int nGuild); // CSD-030324
extern int GetItemMuchByMuchItem( int item_no );		// 수량이 있는 아이템의 갯수 파악하기

extern int CanRegistGuild( int type );
extern void CallGambleMenu();

//define return value		// login message
#define LM_FAIL_		0 //return login fail , unknow reason
#define LM_LOGIN_OK		1 //return login ok
#define LM_FAIL_DB		2 //return login fail , because query db error
#define LM_FAIL_POINT	3 //return login fail , because point no enough or expire date coming
#define LM_FAIL_BAN		4 //return login fail , because user be baned



extern void ClickCharacter();
extern int LoadMenuImage();
extern void GetRealTime( int &mon, int &day, int &hour );
extern void GetItemByPotionBox( int &item_no, int &much );

//acer5
extern void PutFieldOtherCharInfo();
extern void SetBattlePeace( const int type );
extern bool CaptureScreenForJpg( void );
extern int GetBattlePeace();

extern	int InsertWisperList(const char* name);
extern void DisplayRareExplain(const int x,const int y,int &iCount , int &iReultLineCount, int &iBlk,const int iLGab, const int  iWidth,const RareMain *pRareValue);//020730 lsw
extern void DisplayRareExplainSub(const int x,const int y,int &iCount , int &iReultLineCount, int &iBlk,const int iLGab, const int  iWidth,const RareMain *pRareValue,const int ct,const WORD wForceColor = 0);
extern void PutCharImage( int x, int y, int num, int flag, int size, int flag2, int Gender  );

extern void ExplainItemName(const int x , const int y, ItemAttr *pItem, int &iCt, int &iBlk, bool bIsBgOut);
extern ItemAttr ExplainItemAttr;

extern void DecMallItemTime();
#endif

