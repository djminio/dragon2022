#include "stdafx.h"
#include <ddraw.h>
#include <Io.h>
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "Resource.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Hong_Sprite.h"
#include "stdio.h"
#include "directsound.h"
#include "directinput.h"
#include "Char.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "convert565to555.h"
#include "Math.h"
#include "MenuNetwork.h"
#include "GameProc.h"
#include "CharDataTable.h"
#include "object.h"
#include "Item.h"
#include <direct.h>
#include "linecommand.h"
#include "word.h"
#include "kh_cpp.h"
#include "counseler.h"
#include "skillTool.h"
#include "menudef2.h"
#include "MagicSys.h"
#include "skill_lsw.h"
#include "battle.h"
#include "Chat.h"
#include "CurrentMsgMgr.h"
#include "skill.h"
#include "TextOutMgr.h"
#include "MailMgr.h"
#include "Menu.h"
#include "HunterSystem.h"
#include "QuestFunction.h"	// 021024 kyo
#include "ExchangeMgr.h"
#include "ArenaManager.h"
#include "FrameMgr.h" //030703 lsw

extern TCHAR g_szInfoFile[ _MAX_PATH ];



using namespace battle;

extern void GetFriendGroup();

bool g_bDualMagicBookOpen = false;//soto-030421
int g_SetGuildMarkViewOn;
const char string_up_point_abilit[][10] = { "STR", "DEX", "CON", "WIS", "INT", "CHA", "ENDU", "MOVP", "MOR", "LUCK", "WS", "PS", 
"MAG_T", "DEV_T", "CASTING", "HP", "MANA", "DIVINE", "HEALTH" };

extern void GetRealTime( int &mon, int &day, int &hour );

extern void DeleteBetaFile( char *dir );
extern  int		GetItemAttr2(int sprno, int item_id, int attr);		//011018 KHS 망또
extern int g_SelectServerIndex;
extern void CheckImage( int type, int nImageNumber );
int g_GuildMax;
char string_ability[11][5] = { "STR", "DEX", "CON", "WIS", "INT", "CHA", "MOVP", "ENDU", "MOR", "WS", "LUCK" };
//char PosOfNation[][20] = { "", "", "", "The King","The King", "","The King" };
char PosOfNation[2][20] = { "The Queen", "The King" };

CQuestSmallmapDot quest_dot;


CBitBuf		guild_mark_main;
#define _RGB16BIT555(r,g,b) ((b%32) + ((g%32) << 5) + ((r%32) << 10))
#define _RGB16BIT565(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))

int g_Note_Meeting;
char g_Note_Meeting_message[256];

int	smallmenu_img2[MAX_IMAGE2]=	{ 5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21 };		// spr2 을 이용한 독립메뉴.. --> 프리할때 이용
float paytable[9] = { 1.2f, 1.1f, 1.0f, 1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f };

bool g_allowDrop = true; // Eleval for /drop command 15/05/09
/*
typedef struct tag_board			// 임시
{
char	text[4][20];
}DIRECTBOARD, *LPDIRECTBOARD;

  DIRECTBOARD board_test;
  void PutDirectBoardMenu( const LPDIRECTBOARD lpDB );
  void PutBoardMenu( int cn, char *name );
*/

extern void PutDirectBoardMenu( const DIRECTBOARDDATA *lpDB );
extern void PutBoardMenu( int cn, char *name );
extern void StartMenuFree();	// 스타트 메뉴에서 메모리 사용을 프리 시켜준다.
extern void CheckPeaceBattle();	// 플레그를 이용해서 메뉴에서 피스모드 배틀 모드임을 바꿔준다.

extern void ViewGuildCommandInfo2( );	// 길마 명령의 정보를 보여준다. 실제로 정보를 찍어 준다.
extern void ViewEarCommandInfo2( );		// 실제로 정보를 찍어 준다.


CBbsTitle	c_BbsTitle[ MAX_BBS_TITLE ];
char		s_BbsTextBuf[ MAX_STRING_ ];	// 게시판 본문

void EatItem(const ItemAttr item, const POS ItemPos)//020501 lsw 아이템 먹는 함수 무조건 이거 부르세요
{
	int not_delete = 0;
	int item_no = item.item_no, much = 0;
	CItem *t = ItemUnit( item_no );
	
	if( !t ){return;}
	
	const int iUseitemKind = t->GetItemKind();
	int iNoDelete = 0;
	switch(iUseitemKind)
	{
	case IK_POTION_BOX:// 주머니를 클릭했다.
		{
			GetItemByPotionBox( item_no, much );
			if( !much ){return;}
			iNoDelete = 1;
		}break;
	case IK_POTION_BAG:		// BBD 040213 포션백 추가
	case IK_CANDLE:
	case IK_GOOD_CANDLE:
		{
			iNoDelete = 2;
		}break;
	default:
		{
		}break;
	}
	::InsertMagic( Hero, Hero, 247, 0, 0,0,0,0 );
	
	ITEMATTR *pItem = GetItemByPos(ItemPos);
	if(pItem)//지워져 버리면 안되잖아
	{
		::CallItemVirtualFunction(VF_EAT_ITEM, item_no );
	}
	
	POS UsePos;
	SetItemPos(USE, &UsePos);
	// BBD 040213 주석만 추가: 아래놈은 아이템묵는것을 서버한테 허락받는 놈이다
	SendMoveItem( pItem->item_no, ItemPos, UsePos);
	
	if(!iNoDelete)
	{
		::DeleteItem(ItemPos);
	}
}

void CBbsTitle::SetBbsTitle( short no, DWORD time, char *name, char *title )
{
	int len;
	m_Time = time;
	m_Number = no;
	
	m_Year = (short ) (m_Time / 10000);
	m_Month = (short) ( ( m_Time - m_Year*10000 ) / 100 );
	m_Day = (short) (m_Time % 100);
	
	len = strlen(name);
	strncpy( m_Name, name, len );
	m_Name[len] = NULL;
	
	len = strlen(title);
	if(sp_BbsTextTitle) delete sp_BbsTextTitle;
	sp_BbsTextTitle = new char[len+1];
	
	strncpy( sp_BbsTextTitle, title, len );
	sp_BbsTextTitle[len] = NULL;
	
	is_first = true;
	delete m_szMessage;
}

int GetBankItemWeight()		// 무게에 따른 보관료....
{
	int weight=0;
	int a, b, c;
	for(a=0;a<5;a++)
		for(b=0;b<3;b++)
			for(c=0;c<6;c++)
				if( BankItemAttr[a][b][c].item_no ) weight += GetWeight( BankItemAttr[a][b][c] );
				return weight; 
}

int GetCurrentBankItemPay()	// 현재 은행의 무게에 따른 보관료
{ 
	return (int)( GetBankItemWeight() * paytable[4]); 
}	

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


CHouse::CHouse()
{
	//	strcpy(m_Explain1, lan->OutputMessage(7,111)); //010215 lsw
	//	strcpy(m_Explain2, lan->OutputMessage(7,112)); //010215 lsw
	strcpy(m_Explain1, "뭘봐"); //010215 lsw
	strcpy(m_Explain2, "첨봐"); //010215 lsw
	
	strcpy(m_Owner, "Kein"); 
	strcpy(m_Kind, "APT");
}
char* CHouse::GetDur()
{
	wsprintf(temp, "%d/%d", m_Duration, m_DurationMax);
	return temp;
}

void CHouse::SetExplain( char *exp1, char *exp2 )
{
	wsprintf(m_Explain1, "%s", exp1);
	wsprintf(m_Explain2, "%s", exp2);
}


void CHouse::ReadHouse( char * buf )
{
	char *token;
	token = strtok(buf, "\t\n");
	m_Id = atoi(token);
	
	token= strtok(NULL, "\t\n") ;
	m_Image = atoi(token);
	
	token= strtok(NULL, "\t\n") ;
	strcpy(m_Name, token);
	
	token= strtok(NULL, "\t\n") ;
	strcpy(m_HanName, token);
	
	int *data = &m_Width;
	
	for( int a=0; a<18; a++)
	{
		token= strtok(NULL, "\t\n") ;
		*data = atoi(token);
		data++;
	}
}

CHouse	House[HOUSE_MAX_TABLE]; 
bool ReadHouseTable()
{
	char buf[511] ;
	
	FILE *fp ;
	char	filename[MAX_FILENAME], filenameBin[MAX_FILENAME];
	
	sprintf(filename, "./itemtable/housetable.txt");
	sprintf(filenameBin, "./itemtable/housetable.bin");
	
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) && i<HOUSE_MAX_TABLE)
		{
			fgets(buf, 511, fp);
			if( *buf == ';' ) continue;
			House[i].ReadHouse(buf);
			i++;
		}
		fclose(fp);
		
		if(fp=Fopen( filenameBin, "wb" ) )
		{
			char *tt = (char *) House;
			char crc=0;
			for( int i=0; i< sizeof(CHouse)*HOUSE_MAX_TABLE; i++ )
			{
				crc += *tt;
				tt++;
			}
			fwrite( &crc, 1, 1, fp );
			fwrite( House, sizeof(CHouse), HOUSE_MAX_TABLE, fp);
			fwrite( &crc, 1, 1, fp );
			fclose(fp);
		}
	}
	
	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			char *tt = (char *) House;
			char crc=0, crc1, crc2;
			fread( &crc1, 1, 1, fp );
			fread( House, sizeof(CHouse), HOUSE_MAX_TABLE, fp);
			fread( &crc2, 1, 1, fp );
			fclose(fp);
			
			for( int i=0; i< sizeof(CHouse)*HOUSE_MAX_TABLE; i++ )
			{
				crc += *tt;
				tt++;
			}
			if( crc != crc1 || crc != crc2 ) JustMsg(lan->OutputMessage(6,68) );//CheckSumError = 1;//010215 lsw
		}
		else return 0;
	}
	return 1;
}


void CMyHouse::AddHouse( CHouse house, int *count )
{
	CMyHouse *link = this;
	CMyHouse *target;
	
	target = new CMyHouse;
	target->data = house;
	
	target->data.m_DurationMax = house.m_Duration;
	
	while( link->next )
	{
		link = link->next;		
	}
	
	link->next = target;
	target->prev = link;
	(*count)++;
	return;
}

void CMyHouse::FreeAll()
{
	CMyHouse *temp = this;
	if( temp == NULL ) return;
	
	while( temp->next )
	{
		CMyHouse *temp2 = temp->next;
		delete temp;
		temp = temp2;
	}
}

void CMyHouse::DeleteHouse( int *count )
{
	prev = next;
	(*count)--;
	delete this;
	return;
}

int CMyHouse::Repair()
{
	if( !data.m_DurationMax ) JustMsg( lan->OutputMessage(6,67) );//010215 lsw
	return(int) ((data.m_DurationMax - data.m_Duration)*data.m_Price*.9/data.m_DurationMax+.5);
}

//------------------------------------------------------------
// 스몰맵에서 퀘스트 위치 정보를 가르쳐 줄때 사용... ( 호승을 위해..)
int ConvMapnameToMapnumber( char *mapname )
{
	CharUpper( mapname );
	for( int c =0; MapInfo[c].mapfile[0] ; c ++)
		if( strcmp( mapname, MapInfo[c].mapfile ) == 0 ) return c;
		
		return -1;
}

void CQuestSmallmapDot::Setting( int num, int xx, int yy )
{
	map_num = num; 
	x = xx/TILE_SIZE*200/g_Map.file.wWidth; 
	y = yy/TILE_SIZE*200/g_Map.file.wHeight; 
	on = 160;
}

void CQuestSmallmapDot::Setting( char *map_name, int xx, int yy )
{
	int num = ConvMapnameToMapnumber( map_name );
	if( num = -1 ) return;
	
	Setting( num, xx, yy );	
}



//################################### 맵 체인지에 따른 로딩 화면 ######################################
void SettingChangMapMenu( int ct )
{
	int gauge = 211;
#ifdef _DEBUG // finito 060507	
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		char str[10];
		char ch = ct/10+'A';
		if( ch >= 'A' && ch <= 'Z' ) sprintf( str, "%c", ch );
		FieldTypeTextPut( 100, GAME_SCREEN_YSIZE-30, ct, 0, str );
	}
#endif
	for(int k=0; k < ct && ct < 440 ; k++) FieldTypeNomalPut(0, 0, 105+k, GAME_SCREEN_YSIZE-10, gauge);	// 차오르는 게이지바 찍기
	
	
	
}
//#####################################################################################################

//  매직 아이콘 깜박이기...
CMagicOnCheck	MagicOnFlag;

// pgup, pgdn 으로 스몰 함수 돌아 다니기...
const int smallmenu1[] = 
{
	MN_DIVIDE_ITEM,
		MN_ITEM,
		MN_PARTY,
		
		MN_FACE,
		
		MN_STATUS,
		MN_ABILITY,
		MN_ARIGEMENT,
		MN_TACTICS,
		
		MN_SYSTEM,
		MN_SYSTEM_OPTION,
		MN_SYSTEM_LOGOUT,
		//				MN_LEVELUP,
		
		MN_ITEM_MAKE,
		MN_ITEM_MAKE_SELECT,
		
		MN_GM_2ND_SKILL,//010708 lsw
		
		MN_OK_MESSAGEBOX,
		MN_OKCANCEL_MESSAGEBOX,
		MN_ITEM_MESSAGEBOX,
		
		MN_GOD_EDELBLHOY,
		MN_GOD_TEFFERY,
		MN_GOD_LETTY,
		MN_GOD_OREM,
		
		
		MN_SKILLGUILD_BUY			,
		MN_SKILLGUILD_SELL			,
		//				MN_SKILLGUILD_SKILL			,
		//				MN_SKILLGUILD_ITEM			,
		//				MN_SKILLGUILD_REPAIR		,
		
		MN_BANK_DEPOSIT,
		MN_BANK_DEFRAYAL,
		MN_BANK_CUSTODY,
		MN_BANK_MAIN,
		MN_BANK_LOAN,
		MN_BANK_REPAYMENT,
		MN_BANK_AUCTION,
		MN_BANK_AUCTION_SALE,
		
		MN_BLACKSMITH,			// 직업 길드
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
		
		MN_GOD1,
		MN_GOD2,
		MN_GOD3,
		
		MN_MAGICBOOK,
		MN_FUNCTION,
		
		MN_DRINK_MAIN,
		MN_DRINK_RUMOR,	
		MN_DRINK_BUY,	
		
		MN_SHOP_MAIN,
		MN_SHOP_SELL,
		MN_SHOP_BUY,	
		
		MN_HEAL_MAIN,
		MN_HEAL_HEALING,
		MN_HEAL_PREVENTING,
		MN_HEAL_LEARN,
		
		MN_INN_MAIN,
		MN_INN_REST,
		MN_INN_SLEEP,
		MN_SYSTEM_LOGOUT,
		
		MN_DIVIDE_ITEM,
		
		MN_OK_MESSAGEBOX,
		MN_OKCANCEL_MESSAGEBOX,
		MN_ITEM_MESSAGEBOX,
};
const int maxsmall = sizeof( smallmenu1 ) / sizeof(int);

void Small_KeyProc()
{
#ifdef _DEBUG // finito 060507	
	if( !GetSysInfo( SI_GAME_MAKE_MODE ) ) return;
	static int flag=0;
	if(g_aCurrentKeys[DIK_PGDN] & 0x80)
	{
		if( flag == 1 ) 
		{
			return; 
		}
		else 
		{
			flag = 1;
			for(int i=0; i < maxsmall ; i++)
			{
				if( SMenu[smallmenu1[i]].bActive ) 
				{
					SMenu[smallmenu1[i]].bActive = false;
					if( i== maxsmall-1) 
					{
						SMenu[smallmenu1[0]].bActive = true;
						y_MenuFocus = 0;
					}
					else 
					{
						SMenu[smallmenu1[i+1]].bActive = true;
						y_MenuFocus = smallmenu1[i+1];
					}
					return;
				}
			}
		}			
	}
	else  if(g_aCurrentKeys[DIK_PGUP] & 0x80)
	{
		if( flag == 2 ) 
		{
			return; 
		}
		else 
		{
			flag = 2;
			for(int i=0; i < maxsmall ; i++)
			{
				if( SMenu[smallmenu1[i]].bActive ) 
				{
					SMenu[smallmenu1[i]].bActive = false;
					if( i==0) 
					{
						SMenu[smallmenu1[maxsmall-1]].bActive = true;
						y_MenuFocus = smallmenu1[maxsmall-1];
					}
					else 
					{
						SMenu[smallmenu1[i-1]].bActive = true;
						y_MenuFocus = smallmenu1[i-1];
					}
					return;
				}
			}
		}
	}
	else flag = 0;
#endif
}


CHouse *house_sub_residing;
CHouse *house_sub_guild;
CHouse *house_sub_not_res;
int house_res_ct;
int house_guild_ct;
int house_not_res_ct;
//extern Spr credit;//020515 lsw
//extern Spr logo2;//020515 lsw
//extern Spr logo1;//020515 lsw


extern void FreeSpriteOnlyOne( Spr *s );
extern DWORD g_alarm;
extern void CheckDeleteSkill();
void InitMenu()				// 시작할때, 셋업을 상황에 맞춰 해야 할 경우 
{
	g_GuildMgr.Reset();	// CSD-030324 // thai2 YGI
	
	StartMenuFree();		// 시작 메뉴때의 이미지 프리
	GrobalValueSeet();		// 로그아웃후 다시 들어 왔을 때 글로벌 변수 셋..
	LoadManList();			// 상점 주인들..
	
	system_info.music = ::GetPrivateProfileInt("options","music",20,g_szInfoFile);//70;
	system_info.sound = ::GetPrivateProfileInt("options","sound",50,g_szInfoFile);
	system_info.gore = ::GetPrivateProfileInt("options","gore",0,g_szInfoFile); // finito 050707 reads gore option 

	MenuSet();
	SmallMenuSet();
	ReadHouseTable();
	
	LoadOpenningSpr( "./data/face_man_s.dat", &facebuf_man_s, spr_face_man_s);				// 캐릭터 얼굴 이미지 로드 남녀,
	LoadOpenningSpr( "./data/face_woman_s.dat", &facebuf_woman_s, spr_face_woman_s);			
	LoadOpenningSpr( "./data/face_man_p.dat", &facebuf_man_p, spr_face_man_p);				// 파티원 얼굴 	
	LoadOpenningSpr( "./data/face_woman_p.dat", &facebuf_woman_p, spr_face_woman_p);
	LoadOpenningSpr( "./data/face_man_mm.dat", &facebuf_man_mm, spr_face_man_mm);			// 파티원 얼굴 	
	LoadOpenningSpr( "./data/face_woman_mm.dat", &facebuf_woman_mm, spr_face_woman_mm);
	
	//	0906 kkh 추가 
	LoadOpenningSpr( "./data/face_man_s_add.dat", &facebuf_man_s_dash, spr_face_man_s_dash);				// 캐릭터 얼굴 이미지 로드 남녀,
	LoadOpenningSpr( "./data/face_woman_s_add.dat", &facebuf_woman_s_dash, spr_face_woman_s_dash);			
	LoadOpenningSpr( "./data/face_man_p_add.dat", &facebuf_man_p_dash, spr_face_man_p_dash);				// 파티원 얼굴 	
	LoadOpenningSpr( "./data/face_woman_p_add.dat", &facebuf_woman_p_dash, spr_face_woman_p_dash);
	LoadOpenningSpr( "./data/face_man_mm_add.dat", &facebuf_man_mm_dash, spr_face_man_mm_dash);			// 파티원 얼굴 	
	LoadOpenningSpr( "./data/face_woman_mm_add.dat", &facebuf_woman_mm_dash, spr_face_woman_mm_dash);
	
	
	// 클레스 포인터들 초기화############################
	house = new CHouse; // 초기화
	//####################################################
	
	////////////////////////////////////////////////////////////////////////////////////////
	// 캐릭터 초기 상태 setting
	////////////////////////////////////////////////////////////////////////////////////////
	//	SCharacterData.nCharacterData[FACE] = SCharacterData.nCharacterData[GENDER]*1000+0;
	//020515 lsw 택틱창 공용으로 씀
	if( !SCharacterData.nCharacterData[GENDER] )	//  0:여자 1:남자
	{
		SMenu[MN_ITEM].nField[7].nType=FT_NONE;
		SMenu[MN_TACTICS].nImageNumber=3;//그림 바뀜
		SMenu[MN_TACTICS].nField[6].nWillDo=EXPLAIN_TACTICS_WOMAN;
	}
	
	if( SCharacterData.nCharacterData[SPELL]==PRIEST_SPELL )
	{
		
		//		LoadOpenningSpr( "./data/divine.dat", &divine_gauge, spr_divine_gauge );//020515 lsw 데이타는 알아서 읽자
		SMenu[MN_ABILITY].nField[21].nType=FT_NOMAL_PUT;
		
		// 프리스트일때 11, 12 계열은 없다.
		//		SMenu[MN_LEARN_MAGIC_BOOK].nField[11].nType = FT_NO_CHECK;
		//		SMenu[MN_LEARN_MAGIC_BOOK].nField[12].nType = FT_NO_CHECK;
		
		strcpy( string_ability[WSPS], "PS" );
	}
	else 
	{
		strcpy( string_ability[WSPS], "WS" );
		if( !SCharacterData.nCharacterData[GENDER] )	//  0:여자 1:남자
		{//15 260  
			SMenuFTReg(MN_TACTICS,5,FT_NOMAL_PUT,0,15,260,0,0,STATUS_IMG,25,25,0);//여자
		}
		else
		{//15 236
			SMenuFTReg(MN_TACTICS,5,FT_NOMAL_PUT,0,15,236,0,0,STATUS_IMG,25,25,0);//남자
		}
	}
	// 메모라이즈 초기화...
	//	SCharacterData.sleep = true;
	//	InitMemorize();
	
	LoadOpenningSpr( "./data/main_etc.dat", &main_etc_buf, main_etc_spr);		// 메인 인터페이스~
	LoadOpenningSpr( "./data/skill_menu.dat", &skill_menu_buf, skill_menu_spr);		// 메인 인터페이스~
	LoadOpenningSpr( "./data/help.dat", &help_pcx_buf, help_pcx_spr);		// 메인 인터페이스~
	
	/////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver )
	{
		SCharacterData.age=18;
		SCharacterData.beat=3232;
		//		SCharacterData.LvUpPoint=1000;
		SCharacterData.nCharacterSP = 100;
		SCharacterData.nCharacterMAXSP = 150;
		
		c_BbsTitle[0].SetBbsTitle( 0, 1009, "kein1", "안녕1 여긴 처음");
		c_BbsTitle[1].SetBbsTitle( 1, 1009, "kein2", "안녕2 여긴 처음");
		c_BbsTitle[2].SetBbsTitle( 2, 1009, "kein3", "안녕3 여긴 처음");
		c_BbsTitle[3].SetBbsTitle( 3, 1009, "kein4", "안녕4 여긴 처음");
		c_BbsTitle[4].SetBbsTitle( 4, 1009, "kein5", "안녕5 여긴 처음");
		c_BbsTitle[5].SetBbsTitle( 5, 1009, "kein6", "안녕6 여긴 처음");
		c_BbsTitle[6].SetBbsTitle( 6, 1009, "kein7", "안녕7 여긴 처음");
		
		strcpy( SCharacterData.party[0].m_Name, lan->OutputMessage(5,81) );//010215 lsw
		//SCharacterData.party[0].m_ImageNo = 6;
		SCharacterData.party[0].m_Str = 88;
		SCharacterData.party[0].m_Lv = 56;
		
		strcpy( SCharacterData.relation[0].m_Name, lan->OutputMessage(6,32) );//010215 lsw
		strcpy( SCharacterData.employment[0].m_Name, lan->OutputMessage(6,33) );
		
		SCharacterData.dmg_count = 2;
		SCharacterData.dmg_max = 15;
	}
#endif
	////////////////////////////////////////////////////////////////////////
	// 마을회관에서 파는 집들 셋팅 
	///////////////////////////////////////////////////////////////////////
	int ct1=0, ct2=0, ct3=0;
	int a;
	for( a=0; a<HOUSE_MAX_TABLE; a++ )
	{
		if( House[a].m_ForResiding==1 )		ct1++;			
		if( House[a].m_ForGuild==1 )		ct2++;
		if( House[a].m_ForNotResiding==1 )	ct3++;
	}
	house_sub_residing=	new CHouse[ct1];
	house_sub_guild =	new CHouse[ct2];
	house_sub_not_res = new CHouse[ct3];
	
	house_res_ct	=ct1;
	house_guild_ct	=ct2;
	house_not_res_ct=ct3;
	
	ct1=ct2=ct3=0;
	for( a=0; a<HOUSE_MAX_TABLE; a++ )
	{
		if( House[a].m_ForResiding==1 )
		{
			house_sub_residing[ct1] = House[a];
			house_sub_residing[ct1].m_Image = a;
			ct1++;
		}
		if( House[a].m_ForGuild==1 )		
		{
			house_sub_guild[ct2] = House[a];
			house_sub_guild[ct2].m_Image = a;
			ct2++;
		}
		if( House[a].m_ForNotResiding==1 )	
		{
			house_sub_not_res[ct3] = House[a];
			house_sub_not_res[ct3].m_Image = a;
			ct3++;
		}
	}
	////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG // finito 060507	
	if( SysInfo.notconectserver )		// 임시 데이타
	{
		for( a=0; a<200; a++ )
		{
			SCharacterData.SkillId[a] = true;			// 스킬 아이콘 초기화
			SCharacterData.MagicId[a] = true;
		}
		for( a=0; a<13; a++ )							// 택틱스 틍력치	
		{
			SCharacterData.SkillId[a+88] = a;
			SCharacterData.TacticsExp[a] = a*10;
			SCharacterData.TacticsExpNext[a] = a*100;
		}
	}
#endif	
	LoadMagicName();	// 매직 네임을 가져온다.
	ReadSkillTable();
	LoadItemJoinTable();		// 결합 아이템 테이블
	LoadDiseaseTbl();			// 질병 테이블 읽어 오기
	
	SCharacterData.nExpNext = NPC_Lev_Ref[SCharacterData.nLevel].nMaxExp;		// 캐릭터 다음 경험치 레벨 계산 
#ifdef _DEBUG // finito 060507	
	if( GetSysInfo( SI_NOT_CONNECT_MODE ) )
	{
		SCharacterData.nExpNext = 1000000000;
		SCharacterData.nExp = 0;
		SCharacterData.nLevel = 110;
	}

	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		for( a=0; a<6; a++ )
		{
			SCharacterData.disease[a] = 1;
		}
	}
#endif
	InitPartyState( );			// 이미 들어 와 있는 지를 판단... 초기화...
	
	CallServer( CMD_REQ_JOIN_GAME );		// 나 들어 왔다...
	
	// 메모라이즈를 해야 하는지에 대한...
	if( g_bMagicSelect ) MagicSetting();
	else MagicSetting2();		// 게임 테스트용 임시~
	
	//if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	//CallOkCancelMessageBox(0,0,0, "현재는 메모라이즈 개념이 아닙니다. 바꾸려면 숫자키 7번을 눌러 주세요", 0 );
	
	//CheckAbility();		// 아이템에 의한 
	if( BetaTest )
		CallOkCancelMessageBox(0,0,0, "당신은 베타테스트 서버로 들어 오셨습니다.", 0 );//010215 lsw temp
	
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		g_ServerText.SetText( "안녕하세요. 이건 서버에서 텍스트문을 받아 출력하려고 만든 클레스입니다. 지금 줄단위 테스트를 하고 있는데"
			" 어떻게 잘 나오고 있는지 모르겠군요..  잘 출력이 되야 할텐데요..  "
			".. 1234567890"
			".. 1234567890"
			".. 1234567890"
			".. 1234567890" );	
	}
#endif
	g_bTimeMenuOn = true;				// 시간을 본다.
	
	if( SCharacterData.nLevel <= 8 )		// 메뉴 설명
		SMenu[MN_MAIN_EXPLAIN].bActive = true;
	else SMenu[MN_MAIN_EXPLAIN].bActive = false;
	
	if( SCharacterData.nLevel >= 8 ) g_ExplainMenuOn = false;
	else g_ExplainMenuOn = true;
	
	g_MagicMenuOn = false;			// 매직 창 처음에 띄우지 않는다.
	
	GetTatics();		// 택틱 설정하기
	ReqTacSkillExp( );	// 전체 택틱 수치 가져오기
	AllReqSkillExp( );	// 전체 스킬 경험치 가져오기
	
	if( recv_learn_item ) SetLearnItem( );
	else recv_learn_item = 1;
	
	LoadFilterWord();
	hunter_list	= new CHunterList;
	curr_hunter_list = hunter_list;
	my_hunter_list = new CHunterList;
	
	LoadLvColor();		// 레벨의의한 색깔을 보정해서 출력한다.
	LoadCallNameByLv();
	LoadJobLvColor();
	
	////////////////// 0813 lkh 추가 ///////////////////////
	StopWave(1);		// 021030 YGI
	g_Volume_Off=0;			//다시 소리나게
	SetVolumeMusic(system_info.music);
	BackgroundMusic();
	g_BGE_Flag = 1;
	////////////////////////////////////////////////////////

	SetVolume(system_info.sound);

	//g_FightMapMember.SetFightMapInit();		// fight_map_member 초기화
	g_FightMapStart = 0;
	
	LoadGMQuest();		// 그랜드 마스터 등록 테이블 로딩...
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver )
	{	//< CSD-030324
		g_GM_Item.GetItemExplain(1);		// bin 파일을 생성하기 위해
		g_GM_Item.GetItemExplain(2);
		// thai2 YGI
		g_GM_Item.GetItemExplain(3);
		int temp = SCharacterData.nCharacterData[SPELL];
		SCharacterData.nCharacterData[SPELL] = WIZARD_SPELL;
		ReadMagicTable();
		SCharacterData.nCharacterData[SPELL] = PRIEST_SPELL;
		ReadMagicTable();
		SCharacterData.nCharacterData[SPELL] = temp;
	}	//> CSD-030324
#endif
	g_GM_Item.GetItemExplain();		// 그랜드 마스터 등록 아이템 설명 테이블 로딩...
	
	if( SCharacterData.nLevel <= CHOBO_LEVEL )		// 초보일경우 
	{
		if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )		// 마법사인 경우
		{
			AddCurrentStatusMessage( 255, 255, 255, lan->OutputMessage(6,34) );//010215 lsw
		}
	}
	
	SetChatMode(CM_MESSAGE);//	메시지 창을 띄운다.
	AutoLevelUpCount = 0;
	
#ifdef CHANGE_MONEY_
	DWORD old_money = GetMoneyByItem( );
	if( old_money )
	{
		AddCurrentStatusMessage( 255, 255, 255,lan->OutputMessage(6,35) );//010215 lsw
	}
#endif
	// thai2 YGI
	LoadGuildListEx(Hero->GetGuildCode()); // CSD-030324
	if( g_lpRankLadderBBS ) delete g_lpRankLadderBBS;
	g_lpRankLadderBBS = new CRankLadder;
	
	AddCurrentStatusMessage( 255, 255, 0,lan->OutputMessage(3,210) );//this5 lsw
	AddCurrentStatusMessage( 255, 255, 0,lan->OutputMessage(3,211) );//this5 lsw
	
	g_alarm = g_curr_time;
	
	// 자신의 길드에 대한 정보 요청
	if( Hero->GetGuildCode() )
	{	//< CSD-030324
		short int guild_code = Hero->GetGuildCode();
		CallServer( CMD_CHECK_MY_GUILD, &guild_code, sizeof( short int ) );
		SendReqGuildInfo1( Hero->GetGuildCode());
	}	//> CSD-030324
	
	LoadMemorize();
	CheckDeleteSkill();
	
	// 020320 YGI
	RemoveExchangeItemStatus();		// 교환 상태 해제
	//acer7
	main_interface.data[IF_BATTLE_PEACE] = 0;	// 배틀 모드
	SettingBasicAction( main_interface.data[IF_BATTLE_PEACE] );
	InitCombatSkill();//020802 lsw
	EWndMgr.ClearAllTxt();//021001 lsw
	ExchangeMgr.Clear();//021216 lsw
	ExchangeMgr.ClearItemData();//021216 lsw
	g_MailMgr.Clear();

	// 021107 YGI
	SendCheckEventMoveMap();		// 이벤트 중인 맵이 있는지 확인해 본다.
	GetFriendGroup();//030504 lsw
	//< CSD-TW-030704
	g_mgrBattle.ClearDelay();
	g_mgrBattle.ClearSwitchDelay();

	// 031110 YGI
	CheckRoof();
}	//> CSD-TW-030704

bool bCharListUp = false;
bool bTackListUp = false;

k_client_create_item gener_item;
bool bGenerateItemOn = false;


int kein_kh_key_on = true;
int SaveArmor[2][8];
BOOL EquipSaveFlag[2];
extern int AutoChangeEquip( int flag_key );		// f3, f4를 눌렀을때
extern void WarMenuNoticeOpen();							// LTS TEMP
void kein_KeyProc()
{
	HWND temp = GetFocus();
	if( temp != g_hwndMain ) return ;
	
	// ########################## 위험한 기능들... ################################
#ifdef	_DEBUG
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		if( !kein_kh_key_on )
		{
			if(g_aCurrentKeys[DIK_A] & 0x80)
			{
				// 021107 YGI
				SendCheckEventMoveMap();
				//CallGambleMenu();
				//CallSmallMenu(MN_RUNE);
				//CallMenu( MN_GUILD_HOUSE );
			}
			if(g_aCurrentKeys[DIK_B] & 0x80 )
			{	
				CallSmallMenu( MN_GUILD_ITEM_OR_MEMBER_LIST );				
			}
			if(g_aCurrentKeys[DIK_C] & 0x80 )
			{	
				CallGuildItemPut();
				//CallSmallMenu( MN_GUILD_ITEM_PUT );
				//CallGetGuildItem();
			}
			if(g_aCurrentKeys[DIK_D] & 0x80 )
			{	
				//	CallGetGuildItem();
			//	CallSmallMenu(MN_GUILD_ADMIN);
				CallMenu(MN_ARENA_LOBBY);
				//CallServer( CMD_CHECK_VOTE );
				//g_VoteSignExist = 400;
			}
			if(g_aCurrentKeys[DIK_E] & 0x80 )
			{	
				CallGuildHouseInfo();
				//CallGuildMemberListMenu();
				//CallFriendMenu();
			}
			if(g_aCurrentKeys[DIK_F] & 0x80 )
			{
				Call_EXCHANGE_BOND_MONEY_Menu();
				//SendDirectDB( CMD_SAVE_GUILD_MARK_IMAGE, guild_mark_main.m_Spr.img, guild_mark_main.m_Spr.size );
				//SendReqSkillInfo( Hero );
			}
			if(g_aCurrentKeys[DIK_G] & 0x80 )
			{
				CallSmallMenu( MN_DUAL_MAIN );
			}
			if( g_aCurrentKeys[DIK_H] & 0x80 ) CallServer( CMD_SCAN_TARGET );
			if(g_aCurrentKeys[DIK_I] & 0x80 ) 
				CallSmallMenu( MN_GUILD_CHANGE_INFO );
			//bDisplaySkillExp =  !bDisplaySkillExp;
			if(g_aCurrentKeys[DIK_K] & 0x80 ) CallSmallMenu( MN_GUILD_MAIN );
			if(g_aCurrentKeys[DIK_M] & 0x80 ) CallSmallMenu( MN_QUEST_INDEX );
			{
				
			}
			if(g_aCurrentKeys[DIK_N] & 0x80 ) 
			{
				AddCurrentStatusMessage( 255,255,0,"CallMapMoveMenu");
				extern void CallMapMoveMenu(int WarfieldNo);
				CallMapMoveMenu(3);
			}
			//< CSD-011008
			/*
			if(g_aCurrentKeys[DIK_O] & 0x80 ) 
			{
			g_fight_win = 1;
			AddCurrentStatusMessage( 250,255,255, kein_GetMenuString(10) );
			}
			*/
			if(g_aCurrentKeys[DIK_LEFT] & 0x80 ) 
			{
				CallSmallMenu(280);
			}
			if(g_aCurrentKeys[DIK_O] & 0x80 ) 
			{
				extern void RecvCallSmallMenuCustom(t_server_call_smallmenucustom *p);//011011 lsw
				t_packet p;
				p.u.CallSmallMenuCustom.iMenuNo = 280;
				p.u.CallSmallMenuCustom.iPicNo = 8;
				p.u.CallSmallMenuCustom.iWidth = 436;
				p.u.CallSmallMenuCustom.iHeight = 437;
				p.u.CallSmallMenuCustom.iExt = 900;
				RecvCallSmallMenuCustom(&p.u.CallSmallMenuCustom);
			}
			//> CSD-011008
			if(g_aCurrentKeys[DIK_P] & 0x80 ) 
			{
				g_fight_lose = 1;
				AddCurrentStatusMessage( 250,0,0, kein_GetMenuString(11) );
			}
			if(g_aCurrentKeys[DIK_Q] & 0x80 ) 
			{
				static int cn=0; 
				(cn%=4)++;
				g_RemainTimeType = cn;
				g_RemainTimeDelay = 80;
			}
			if( g_aCurrentKeys[DIK_W] & 0x80 ) CallSmallMenu( MN_ITEM_MAKE_RARE_SELECT );//010730 lsw
			if( g_aCurrentKeys[DIK_V] & 0x80 ) CallSmallMenu( MN_NATION_BUDAE );//010730 lsw
			if( g_aCurrentKeys[DIK_U] & 0x80 ) CallSmallMenu( MN_NATION_VOTE );//010730 lsw
			if( g_aCurrentKeys[DIK_T] & 0x80 ) CallSmallMenu( MN_NATION_NOW );//010730 lsw
			if( g_aCurrentKeys[DIK_S] & 0x80 ) CallSmallMenu( MN_NATION_INFO );
			if( g_aCurrentKeys[DIK_C] & 0x80 ) 
			{
				strcpy(SMenu[MN_LOCALWAR_OK].nField[3].temp, lan->OutputMessage(0,378)); // LTS LOCALWAR MENU
				CallSmallMenu(MN_LOCALWAR_OK);
			}
			if(g_aCurrentKeys[DIK_J] & 0x80 ) 
			{
				//bDisplayTacticExp= !bDisplayTacticExp;//020515 lsw
		//		CallSmallMenu(MN_EVENT_LOCALWAR_OK);MN_LOCALWAR_JOIN
				CallSmallMenu(MN_REVIVAL);
			}
			
			if(g_aCurrentKeys[DIK_R] & 0x80)
			{
				CallSmallMenu(MN_ITEM_MALL);		// 퀘스트 메뉴
				//CallSmallMenu(MN_CHARACTER_COLOR_CHANGE);		// 퀘스트 메뉴
			}
			
			
			kh_KeyProc();
			return;
		}
		
		if( bCharListUp )
		{
			for( int i=STR; i<=LUCK; i++ )
			{
				DWORD data = SCharacterData.nCharacterAbility[i];
				SendCharInfoBasic( i, data );
			}
			SendCharInfoBasic( MONEY, SCharacterData.nMoney );
			SendCharInfoBasic( LVPOINT, SCharacterData.LvUpPoint );
			bCharListUp = false;
		}
		if( bGenerateItemOn ) 
		{
			SendGenrateItemByTool( gener_item );
			bGenerateItemOn = 0;
		}
		if( bTackListUp )
		{
			for( int i=CRAPPLE; i<=ORISON; i++ )
			{
				DWORD data = SCharacterData.TacticsExp[i];
				SendCharInfoBasic( i+TAC_E1, data );
			}
			ReqTacSkillExp( );
			bTackListUp = false;
		}
		
		if(g_aCurrentKeys[DIK_F12] & 0x80 )		// Refresh pc_word_table and npc_word_table 
		{
			max_pc_word = 0;			// 현재 PC가 대답할수있는 문장의 수
			max_npc_word = 0;			// 현재 NPC가 대답할수있는 문장의 수
		}
		
		if(g_aCurrentKeys[DIK_F2] & 0x80 )		// Refresh manlist 
		{
			LoadManList();
		}
		
		/////////////////////////////////
		if(g_aCurrentKeys[DIK_Z] & 0x80 )
		{	
			kein_kh_key_on = false;
			return;
		}
		
		/*
		
		  // ######### 인벤 창 Refresh 4번 키 #########
		  //QuickItemAttr
		  static bool f_resend = false;
		  if(g_aCurrentKeys[DIK_NUMPAD4] & 0x80 )
		  {	
		  if( !f_resend )	
		  {
		  CallServer( CMD_RESEND_ITEM );
		  f_resend = true;
		  }
		  }
		  else f_resend = false;
		  
			
			  // ######### 능력치 갱신 5번 키 ########
			  static bool f_time = false;
			  if(g_aCurrentKeys[DIK_NUMPAD5] & 0x80 )
			  {	
			  if( !f_time )	
			  {
			  CallServer(CMD_RESEND_CHARLIST);
			  f_time = true;
			  }
			  }
			  else f_time = false;
			  
				// ######### 파티원들 보여주기 갱신 7번 키 ########
				static bool f_party = false;
				if(g_aCurrentKeys[DIK_NUMPAD7] & 0x80 )
				{	
				if( !f_party )
				{
				CallServer( CMD_REQ_PARTY_MEMBER );
				f_party = true;
				}
				}
				else f_party = false;
				// ##################################################
				static bool f_memorize = false;
				if(g_aCurrentKeys[DIK_7] & 0x80 )
				{	
				if( !f_memorize )
				{
				g_bMagicSelect = !g_bMagicSelect;
				if( g_bMagicSelect )
				{
				CallOkCancelMessageBox(0,0,0,"마법 방식이 메모라이즈 개념으로 바뀌었습니다.",0);//010215 lsw temp
				RecvServerWakeUp();
				}
				else 
				{
				CallOkCancelMessageBox(0,0,0,"마법 방식이 능력제로 바뀌었습니다.",0);//010215 lsw temp
				MagicSetting2();
				}
				f_memorize = true;
				}
				}
				else f_memorize = false;
				
				  
					//####### 깨어나라	8번키 #######
					static bool f_sleep = false;
					if(g_aCurrentKeys[DIK_NUMPAD8] & 0x80 )
					{	
					if( !f_sleep )
					{
					CallServer( CMD_SERVER_WAKE_UP );
					f_sleep = true;
					}
					}
					else f_sleep = false;
					
					  static int flag = 0;
					  if(g_aCurrentKeys[DIK_NUMPAD1] & 0x80)
					  {
					  ItemRead();
					  }
					  
						if(g_aCurrentKeys[DIK_NUMPAD2] & 0x80)
						{
						if( flag == 0 )
						{
						SCharacterData.nCharacterData[GENDER] = !SCharacterData.nCharacterData[GENDER];
						Hero->sprno = SCharacterData.nCharacterData[GENDER];
						if( !SCharacterData.nCharacterData[GENDER] )	//  0:여자 1:남자
						{
						SMenu[MN_ITEM].nField[7].nImageNumber=483;
						SMenu[MN_TACTICS].nImageNumber=10001;
						}
						else
						{
						SMenu[MN_ITEM].nField[7].nImageNumber=120;
						SMenu[MN_TACTICS].nImageNumber=867;
						}			
						flag = 2;
						}
						}
						else flag = 0;
						
						  // ######### 캐릭터 아이템 보여주기 6번 키 #########
						  static bool f_itemshow = false;
						  if(g_aCurrentKeys[DIK_NUMPAD6] & 0x80 )
						  {	
						  if( !f_itemshow )	
						  {
						  //				CallOtherCharacterInv( InvItemAttr, EquipItemAttr, QuickItemAttr );
						  SendShowItemOfOtherCh( Hero->id );
						  f_itemshow = true;
						  }
						  }
						  else f_itemshow = false;
						  
							// ######### 바닥에 아이템 떨어뜨리기 3번 키 #########
							static bool f_item = false;
							if(g_aCurrentKeys[DIK_NUMPAD3] & 0x80 )
							{	
							if( !f_item )	
							{
							DropItemOnHero( QuickItemAttr[6] );
							f_item = true;
							}
							}
							else f_item = false;
							
*/


//######### 캐릭터 매직계열 바꾸기 F10 ##########
static int flag1 = 0;
if(g_aCurrentKeys[DIK_F10] & 0x80)
{
	if( flag1 == 0 )
	{
		SCharacterData.nCharacterData[SPELL] = !SCharacterData.nCharacterData[SPELL];
		for( int a=0; a<MAX_MAGIC_; a++)  magic[a].DeleteMagic();
		
		memset( aMagicItem, 0, sizeof( int[3][4][3][5] ) );
		/*	//020515 lsw 데이타 로딩 필요 없습니다.
		if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL  )
		{
		LoadOpenningSpr( "./data/divine.dat", &divine_gauge, spr_divine_gauge );
		}
		else FreeOpenningSpr( &divine_gauge);
		*/
		ReadMagicTable();
		MagicSetting();
	}
	flag1 = 2;
}
else flag1 = 0;


	}
	else bGenerateItemOn = false;
	
	
#endif
	//#####################################################3
	if(g_aCurrentKeys[DIK_SPACE] & 0x80)			//스페이스 바
	{
		CloseAllMenu();
	}
	
	static int enter_flag=0;
	if(g_aCurrentKeys[DIK_RETURN] & 0x80)				//	enter
	{
		enter_flag = 2;
	}
	else
	{
		if( enter_flag == 2 )
		{
			if( SMenu[MN_SYSTEM_LOGOUT].bActive ) 
			{
				DoButtonCheckOfMenu( MN_SYSTEM_LOGOUT, 0 );
				return;
			}
		}
		enter_flag = 0;
	}
	
	
	if(g_aCurrentKeys[DIK_ESCAPE] & 0x80)					// ESC key
	{
		if( fLMoveMenu || fRMoveMenu ) return;
		
		if( SMenu[MN_SYSTEM_LOGOUT].bActive ) 
		{
			DoButtonCheckOfMenu( MN_SYSTEM_LOGOUT, 1 ); 
			return;
		}
		
		SMenu[MN_MESSAGEBOX_BIG].bActive=false;
		SMenu[MN_MESSAGEBOX_SMALL].bActive =false;
		
		
		if( !IsChatBoxActive())
		{
			int menu = MenuStack.GetMenu();
			if( menu != -1 ) 
			{
				if( menu == MN_ARIGEMENT )
				{
					fLMoveMenu = MM_ARIGEMENT_CLOSE;
					MP3( SN_MOVE_CLOSE );
					return;
				}
				for(int j=0;j<SMenu[menu].nFieldCount;j++)
					if( (SMenu[menu].nField[j].nImageNumber==122) || (SMenu[menu].nField[j].nImageNumber==475) ) //122,475 -> done button
					{
						SMenu[menu].nField[j].fCheakFlag = TRUE;
						DoButtonCheckOfMenu( menu, j );
						return;
					}				
			}
			else CloseAllMenu();
		}
	}

#ifdef _DEBUG // finito 060507	
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		static int f ;
		if(g_aCurrentKeys[DIK_TAB] & 0x80)					//	tab
		{
			if( !f ) 
			{
				
				f=1;
			}
		}
		else f = 0;
	}
#endif

	if(g_aCurrentKeys[DIK_LSHIFT] & 0x80)	LeftShiftOn = true;		//	SHIFT
	else	LeftShiftOn = false;
	/*-- 임시 스몰 메뉴 띄우기 --*/
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		if(g_aCurrentKeys[DIK_B] & 0x80)
		{
			CallSmallMenu(MN_BLACKSMITH);
		}
		if(g_aCurrentKeys[DIK_D] & 0x80)
		{
			if( SysInfo.notconectserver )
			{
				SMenu[MN_EXCHANGE_ITEM].bActive = true;
			}
		}
		if(g_aCurrentKeys[DIK_E] & 0x80)
		{
			CallSmallMenu( MN_GOD_EDELBLHOY );
		}
		if(g_aCurrentKeys[DIK_F] & 0x80)
		{
			CallSmallMenu(MN_SHOP_MAIN);
		}
		if(g_aCurrentKeys[DIK_G] & 0x80)
		{
			CallMenu(MN_GAMBLE);
		}
		if(g_aCurrentKeys[DIK_H] & 0x80)
		{
			CallSmallMenu( MN_POTAL_MENU );
		}
		if(g_aCurrentKeys[DIK_I] & 0x80)
		{
			CallSmallMenu(MN_TOWNHALL);
		}
		if(g_aCurrentKeys[DIK_J] & 0x80)
		{
			g_bTimeMenuOn = false;
			CallSmallMenu(MN_DRINK_MAIN);
		}
		if(g_aCurrentKeys[DIK_K] & 0x80)
		{
			CallSmallMenu( MN_ACADEMY );
		}
		if(g_aCurrentKeys[DIK_L] & 0x80)
		{
			CallSmallMenu(MN_FUNCTION);
		}
		if(g_aCurrentKeys[DIK_M] & 0x80)
		{
			CallSmallMenu(MN_MAGIC_GUILD);
		}
		if(g_aCurrentKeys[DIK_Q] & 0x80)
		{
			int r = rand() % 2;
			if( r )	SCharacterData.nation = 3;
			else 	SCharacterData.nation = 4;
			nation_bbs[0] = NULL;
			nation_bbs2[0][0][0] = NULL;
			CallSmallMenu(MN_NATION_BBS);
		}
		if(g_aCurrentKeys[DIK_T] & 0x80)
		{
			quest_dot.Setting( MapNumber, Hero->x, Hero->y );
			CallServer( CMD_CALL_PUBLIC_BBS );
		}
		if(g_aCurrentKeys[DIK_C] & 0x80)
		{
			CallMenu( MN_OBSERVER_MODE );
			//bCallGodMeetingMenu = !bCallGodMeetingMenu;
			//StopWave( SN_GOD_MEETING );
			//CallSmallMenu(MN_MEM_BOOK);
		}
		if(g_aCurrentKeys[DIK_N] & 0x80)
		{
			CallMenu( MN_BETTING );
		}		
		
		if(g_aCurrentKeys[DIK_Q] & 0x80)
		{
			//			CallSmallMenu(MN_GOD_SUPPER);		// 음식 제공 메뉴
			//			CallSmallMenu(MN_MAIL_MENU);
		}		
		
		if(g_aCurrentKeys[DIK_R] & 0x80)
		{
			CallSmallMenu(MN_HUNTER_MAIN);
		}		
		
		
		
		if(!LeftShiftOn && g_aCurrentKeys[DIK_F12] & 0x80)
		{
			
			
			//			for( int i=0; i<ITEM_JOIN_MAX; i++)
			//			{
			//				SCharacterData.ItemId[i] = item_join[i].item_id;
			//			}
			
			//			SCharacterData.ItemIdCount = MAX_LEARN_ITEM;
		}
		else if(LeftShiftOn && g_aCurrentKeys[DIK_F12] & 0x80)
		{
			//			for( int a=0; a<SCharacterData.ItemIdCount; a++)
			//			{
			//				SCharacterData.ItemId[a] = 0;
			//			}
			
			//			SCharacterData.ItemIdCount = 0;
		}
		
		if( !LeftShiftOn && g_aCurrentKeys[DIK_F11] & 0x80)
		{
			//CallServer( CMD_LEARN_ALL );
			for( int i=0; i<200; i++ )
			{
				SCharacterData.MagicId[i] = true;
				SCharacterData.SkillId[i] = true;
			}
			MagicSetting2();
		}
		if( LeftShiftOn && g_aCurrentKeys[DIK_F11] & 0x80 )
		{
			//CallServer( CMD_LEARN_ALL_NOT );
			for( int i=0; i<200; i++ )
			{
				SCharacterData.MagicId[i] = false;
				SCharacterData.SkillId[i] = false;
			}
		}
	}

	if( SysInfo.notconectserver )
	{	//< CSD-030520
		static int nCount = -1;
		static int key = 0;
		if(g_aCurrentKeys[DIK_1] & 0x80)
		{
			key = 1;
		}
		else if( key == 1 ) 
		{
			ChangeItem( ++nCount );
			key = 0;
		}
		else if(g_aCurrentKeys[DIK_2] & 0x80)
		{
			key =2;
		}
		else if( key == 2 )
		{
			nCount--;
			if( nCount < 0 ) nCount = 0;
			ChangeItem( nCount );
			key = 0;
		}
	}	//> CSD-030520
#endif
	bool lcontrol = false;
	if( g_aCurrentKeys[DIK_LCONTROL] & 0x80 ) lcontrol = true;
	
	static bool f_change_equip1 = false;
	if(g_aCurrentKeys[DIK_F9] & 0x80 )
	{	
		if( !f_change_equip1 )
		{
			if( lcontrol )
			{
				EquipSaveFlag[0] = TRUE;
				for(int i=0;i<8;i++)
				{
					SaveArmor[0][i] = EquipItemAttr[i].item_no;
				}
			}
			else 
			{
				AutoChangeEquip( 0 );
			}
			f_change_equip1 = true;
		}
	}
	else 
	{
		f_change_equip1 = false;
	}
	
	static bool f_change_equip2 = false;
	if(g_aCurrentKeys[DIK_F10] & 0x80 )
	{	
		if( !f_change_equip2 )
		{
			if( lcontrol )
			{
				EquipSaveFlag[1] = TRUE;
				for(int i=0;i<8;i++)
				{
					SaveArmor[1][i] = EquipItemAttr[i].item_no;
				}
			}
			else 
			{
				AutoChangeEquip( 1 );
			}
			f_change_equip2 = true;
		}
	}
	else 
	{
		f_change_equip2 = false;
	}
}

int AutoChangeEquip( int flag_key )		// f3, f4를 눌렀을때
{
	if( IsDead( Hero ) ) return 0;		// 죽었을때 사용을 못하게 한다.
	if( !EquipSaveFlag[flag_key] ) return 0;
	
	if( Hero->ready_Magic == 1 ) return 0;
	ServerConnectMenuClose();		// 교환창을 종료 시킨다.
	
	static DWORD time = g_curr_time-20;
	DWORD gab = g_curr_time-time;
	if( gab < 2 )
	{
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 200, 20, 20, lan->OutputMessage(6,36), 10-gab );//010215 lsw
		return 0;
	}
	
	if( EquipItemAttr[WT_SHIELD].item_no )		// 먼저 방패를 없앤다.
	{
		int a1, b1, c1;
		int ret = SearchEmpty( a1, b1, c1 );
		if( ret )
		{
			POS pos_s, pos_t;
			SetItemPos( EQUIP, WT_SHIELD, &pos_s );
			SetItemPos(INV, a1, b1, c1, &pos_t);
			SendMoveItem( EquipItemAttr[WT_SHIELD].item_no, pos_s, pos_t );
			
			CallItemVirtualFunction( VF_RELEASE_ITEM, EquipItemAttr[WT_SHIELD].item_no );
			
			InvItemAttr[a1][b1][c1] = EquipItemAttr[WT_SHIELD];
			memset( &EquipItemAttr[WT_SHIELD], 0, sizeof( ItemAttr ) );
			
			time = g_curr_time;
		}
	}
	
	for(int i=0;i<8;i++)
	{
		if(EquipItemAttr[i].item_no != SaveArmor[flag_key][i])
		{
			for(int a=0;a<3;a++)
			{
				for(int b=0;b<3;b++)
				{
					for(int c=0;c<8;c++)
					{
						if(SaveArmor[flag_key][i])
						{
							if( InvItemAttr[a][b][c].item_no == SaveArmor[flag_key][i] )
							{
								POS pos_s, pos_t;
								SetItemPos(INV, a, b, c, &pos_s);
								SetItemPos( EQUIP, i, &pos_t );
								SendMoveItem( InvItemAttr[a][b][c].item_no, pos_s, pos_t );
								
								CallItemVirtualFunction( VF_RELEASE_ITEM, EquipItemAttr[i].item_no );
								CallItemVirtualFunction( VF_EQUIP_ITEM, InvItemAttr[a][b][c].item_no );
								
								EquipItemAttr[i] = InvItemAttr[a][b][c];
								memset( &InvItemAttr[a][b][c], 0, sizeof( ItemAttr ) );
								time = g_curr_time;
								goto kein3;
							}
						}
						else
						{
							if( EquipItemAttr[i].item_no )
							{
								int a1, b1, c1;
								int ret = SearchEmpty( a1, b1, c1 );
								if( ret )
								{
									POS pos_s, pos_t;
									SetItemPos( EQUIP, i, &pos_s );
									SetItemPos(INV, a1, b1, c1, &pos_t);
									SendMoveItem( EquipItemAttr[i].item_no, pos_s, pos_t );
									
									CallItemVirtualFunction( VF_RELEASE_ITEM, EquipItemAttr[i].item_no );
									
									InvItemAttr[a1][b1][c1] = EquipItemAttr[i];
									memset( &EquipItemAttr[i], 0, sizeof( ItemAttr ) );
									
									time = g_curr_time;
								}
							}
							goto kein3;
						}
					}
				}
			}
		}
kein3: continue;
	}
	
	
	ChangeEquip( Hero, EquipItemAttr, WT_WEAPON );
	return 1;
}


void ChangeEquip( LPCHARACTER lpCharacter , ItemAttr *equip_item, int type ) //  8개..
{
	int *a;
	int sprno;
	int changeflag = 0;
	int tno;
	
	g_SokSungFlag = 0;			// 속성 체크
	for( int i=0; i<8; i++ )
	{
		if( EquipItemAttr[i].attr[IATTR_RARE_MAIN] )
		{
			g_SokSungFlag = 1;
			break;
		}
	}
	
	GetTatics();		// 새로 착용한 무기의 택틱을 얻어 온다.
	if( lpCharacter== NULL ) return;
#ifdef _DEBUG // finito 060507	
	if( SysInfo.notconectserver )
	{
		a = lpCharacter->accessory;
		sprno = lpCharacter->sprno;
		
		if( lpCharacter  == g_CharacterList.lpFirst )
		{
			tno = GetItemAttr(equip_item[WT_ARMOR].item_no, ITEMSHOW);
			if( tno != a[0] )
			{
				changeflag = 1;
				FreeHeroClothAccessoryDataOne( sprno, a[0] );
				LoadHeroClothAccessoryDataOne( sprno, tno );
				a[0] = tno;
			}
			
			tno = GetItemAttr(EquipItemAttr[WT_HELMET].item_no, ITEMSHOW);
			if( tno != a[1] )
			{
				changeflag = 1;
				FreeHeroClothAccessoryDataOne( sprno, a[1] );
				LoadHeroClothAccessoryDataOne( sprno, tno );
				a[1] = tno;
			}
			tno = GetItemAttr(EquipItemAttr[WT_WEAPON].item_no, ITEMSHOW);
			if( tno != a[2] )
			{
				changeflag = 1;
				FreeHeroClothAccessoryDataOne( sprno, a[2] );
				LoadHeroClothAccessoryDataOne( sprno, tno );
				a[2] = tno;
			}
			// 011030 KHS JJING
			if(  (( RareMain*)(&EquipItemAttr[WT_WEAPON].attr[ IATTR_RARE_MAIN]))->grade >=5 )
			{
				changeflag = 1;
				LoadHeroClothAccessoryDataOne( sprno, tno + 200);
			}
			if(  (( RareMain*)(&EquipItemAttr[WT_WEAPON].attr[ IATTR_RARE_MAIN]))->grade >=9 )
			{
				changeflag = 1;
				LoadHeroClothAccessoryDataOne( sprno, tno + 201);
			}
			
			tno = GetItemAttr(EquipItemAttr[WT_SHIELD].item_no, ITEMSHOW);
			if( tno != a[3] )
			{
				changeflag = 1;
				FreeHeroClothAccessoryDataOne( sprno, a[3] );
				LoadHeroClothAccessoryDataOne( sprno, tno );
				a[3] = tno;
			}
			
			// 011018 KHS  망또
			if( EquipItemAttr[WT_NECK].item_no != lpCharacter->mantle )
			{		
				changeflag = 1;
				int shape = GetItemAttr2( sprno, lpCharacter->mantle, ITEMSHOW );
				FreeHeroClothAccessoryDataOne( sprno, GetItemAttr2( sprno, lpCharacter->mantle, ITEMSHOW ));
				
				// finito 09/08/07 free the extra images for mantle of dragon and mantle of raja
				if (shape == 137)
				{
					FreeHeroClothAccessoryDataOne( sprno, 138 );
				}
				else if (shape == 145)
				{
					FreeHeroClothAccessoryDataOne( sprno, 146 );
					FreeHeroClothAccessoryDataOne( sprno, 147 );
				}
				// end of free extra images

				tno = GetItemAttr2( sprno, EquipItemAttr[WT_NECK].item_no, ITEMSHOW );
				LoadHeroClothAccessoryDataOne( sprno, tno );
				lpCharacter->mantle = EquipItemAttr[WT_NECK].item_no;
				
				// finito 08/08/07 load extra images for mantle of dragon and mantle of raja
				if (tno == 137)
				{
					LoadHeroClothAccessoryDataOne(sprno, 138);
				}
				else if(tno == 145)
				{
					LoadHeroClothAccessoryDataOne(sprno, 146);
					LoadHeroClothAccessoryDataOne(sprno, 147);
				}
				// end of load extra images
			}		
		}			
		else
		{
		}
		
		if( changeflag )
		{
			//SendChangeEquip( Hero );		// 서버에서 자체 판단해서 보내준다.
		}
		
		// 0 : 한손검.
		// 1 : 양손검.
		if( lpCharacter->accessory[2] != 0 ) 
		{
			int check;
			if( GetItemAttr( EquipItemAttr[WT_WEAPON].item_no, WEAR_ABLE ) == WEAR_TWO_HAND ) check = 1;
			else check = 0;
			
			switch( check )
			{
			case 0 : lpCharacter->basicAction = ACTION_BATTLE_NONE; 
				lpCharacter->basicWalk = ACTION_BATTLE_WALK_OWNHAND; 	
				break;
				
			case 1 :
				lpCharacter->basicAction = ACTION_BATTLE_NONE_TWOHAND;
				lpCharacter->basicWalk = ACTION_BATTLE_WALK_TWOHAND;
				break;
			}
		}
		else 
		{
			lpCharacter->basicAction = ACTION_NONE;
			lpCharacter->basicWalk = ACTION_BASIC_WALK; 
		}
	}
#endif	
	SettingBasicActionSub( lpCharacter );
	
	
	if( type == WT_WEAPON )
	{
		//		GetItemAnimationNumber(aArmor_Inventory[WT_WEAPON], int &a, int &b, int &c, int &d, int &e, int &f, int &g)
	}
	
	
	///////////////////// AC 받아 오기 ////////////////////////
	if( SMenu[MN_STATUS].bActive )
	{
		CallServer( CMD_REQ_AC );
	}
	///////////////////// DMG 받아 오기 ////////////////////////
	if( SMenu[MN_STATUS].bActive )
	{
		CallServer( CMD_REQ_DMG );
	}
}	




ItemAttr  *SearchEmpty()
{
	for(int a=0;a<3;a++)
		for(int b=0;b<3;b++)
			for(int c=0;c<8;c++)
				if(!InvItemAttr[a][b][c].item_no)
					return &InvItemAttr[a][b][c];
				return NULL;
}

int SearchEmpty(int &a, int &b, int &c)
{
	for(a=0;a<3;a++)
		for(b=0;b<3;b++)
			for(c=0;c<8;c++)
				if(!InvItemAttr[a][b][c].item_no)
					return 1;
				return 0;
}

int SearchEmptyBank(int &a, int &b, int &c)
{
	for(a=0;a<5;a++)
		for(b=0;b<3;b++)
			for(c=0;c<6;c++)
				if(!BankItemAttr[a][b][c].item_no)
					return 1;
				return 0;
}

int SearchEmptyQuickItem()
{
	for( int a=0; a<5; a++ )
	{
		if( !QuickItemAttr[a].item_no ) return a;
	}
	
	return -1;
}

int IsMagicWindowOn()
{
	return 0;
}

int ItemDrop()
{	//마우스 윈쪽 버튼이 올라갈때 아이템이 마우스와 연결됬고 실제 게임스크린에 들어가 있으면 아이템을 놓는다
	int kkk = IsMagicWindowOn();
	if( g_MouseItemType )
	{   //활성화된 메뉴들을 체크한다
		if( g_MouseInMenu ){return kkk;}
		const int *m = ( is_mainmenu_active )?mainmenu:smallmenu;
		const int max = ( is_mainmenu_active )?MAX_MAIN:MAX_SMALL;
		
		for(int t=0;t<max;t++)
		{
			int menu = m[t];
			if(SMenu[menu].bActive)
			{
				RECT box;
				Spr *s = GetSprOfMenu( SMenu[menu].nImageType, SMenu[menu].nImageNumber );
				if( !s ) continue;
				SetRect(box, SMenu[menu].x, SMenu[menu].y, SMenu[menu].x+s->xl, SMenu[menu].y+s->yl);
				if(MouseInRectCheak(0,0, box)) return kkk; 
			}
		}		
		if( g_MouseItemType == 1  && !g_DropOnHero && g_allowDrop)		// 아이템일 경우만 서버 콜 //Eleval 15/05/09: Added g_allowDrop
		{
			if( IP_base.type != GROUND )
			{
				CItem* i = ItemUnit(HandItemAttr.item_no); // finito 040707 check if item is noechange an therefore can't drop
				if (!i) return kkk; 

				if (i->GetItemKind() == IK_NOEXCHANGE) 
				{
					Kein_PutMessage( KM_FAIL, kein_GetMenuString(243) );
				}
				else
				{
					DropItemSound( HandItemAttr.item_no );
					SendDropItem( HandItemAttr.item_no, Mox, Moy, HandItemAttr.attr );
					g_MouseItemType=0;
					g_MouseItemNumber=0;
					DeleteItem( &HandItemAttr );
				}
			}
		}
	}
	return kkk;
}


void MoveMenu()
{
	cLMoveMenu = 5;
	cRMoveMenu = 5;
	switch(fLMoveMenu)
	{
	case MM_STATUS_OPEN :   SMenu[MN_STATUS].bActive=TRUE;
								SMenu[MN_STATUS].x=-320+64*cLMoveMenu; 
								if(cLMoveMenu == 5 )
								{
									MenuStack.PutMenu(MN_STATUS);
									cLMoveMenu=0;
									fLMoveMenu=0;
									break;
								}
								cLMoveMenu++;
								break;   
								
	case MM_STATUS_CLOSE : 	SMenu[MN_STATUS].x=0-64*cLMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
								if(cLMoveMenu == 5 )
								{
									cLMoveMenu=0;
									fLMoveMenu=0;
									SMenu[MN_STATUS].bActive=FALSE;
									MenuStack.DelMenu(MN_STATUS);
									break;
								}
								cLMoveMenu++;
								break;
	case MM_ABILITY_CLOSE : SMenu[MN_ABILITY].x=0-64*cLMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
								if(cLMoveMenu == 5 )
								{
									cLMoveMenu=0;
									fLMoveMenu=0;
									SMenu[MN_ABILITY].x=0;
									SMenu[MN_ABILITY].bActive=FALSE;
									MenuStack.DelMenu(MN_ABILITY);
									break;
								}
								cLMoveMenu++;
								break;
	case MM_ARIGEMENT_CLOSE :   SMenu[MN_ARIGEMENT].x=0-64*cLMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
		if(cLMoveMenu == 5 )
		{
			cLMoveMenu=0;
			fLMoveMenu=0;
			SMenu[MN_ARIGEMENT].x=0;
			SMenu[MN_ARIGEMENT].bActive=FALSE;
			MenuStack.DelMenu(MN_ARIGEMENT);
			break;
		}
		cLMoveMenu++;
		break;
		
	case MM_TACTICS_CLOSE :   SMenu[MN_TACTICS].x=0-64*cLMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
		if(cLMoveMenu == 5 )
		{
			cLMoveMenu=0;
			fLMoveMenu=0;
			SMenu[MN_TACTICS].x=0;
			SMenu[MN_TACTICS].bActive=FALSE;
			MenuStack.DelMenu(MN_TACTICS);
			break;
		}
		cLMoveMenu++;
		break;
	default	:	fLMoveMenu=0; break;
		
	}
	
	
	switch(fRMoveMenu)
	{
	case MM_ITEM_OPEN :   
		{
			SMenu[MN_ITEM].bActive=TRUE;
			SMenu[MN_ITEM].x=SCREEN_WIDTH-64*cRMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
			if(cRMoveMenu == 5 )
			{
				cRMoveMenu=0;
				fRMoveMenu=0;
				RightClose();
				SMenu[MN_ITEM].bActive=TRUE;	
				if( nItemOther ) 
				{
					SMenu[nItemOther].bActive = TRUE; // 아이템과 같이 떠야하는 메뉴가 있다면 같이 뜬다.
					//					nItemOther = 0;
					LeftMenuClose();
				}
				MenuStack.PutMenu(MN_ITEM);
				break;
			}
			cRMoveMenu++;
			break;   //이렇게 해서 마우스 입력을 막는다.
		}
	case MM_ITEM_CLOSE :   
		SMenu[MN_ITEM].x=480+64*cRMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
		if(cRMoveMenu == 5 )
		{
			cRMoveMenu=0;
			fRMoveMenu=0;
			SMenu[MN_ITEM].x=480;
			SMenu[MN_ITEM].bActive=FALSE;
			if( nItemOther ) 
			{
				SMenu[nItemOther].bActive = false;
				nItemOther = 0;
			}
			if(SMenu[MN_BANK_CUSTODY].bActive)
			{
				SMenu[MN_BANK_CUSTODY].bActive = FALSE;
				//				SmallMenuClose();		MN_BANK_CUSTODY ==> 스몰 메뉴가 아닐껄
			}			
			MenuStack.DelMenu(MN_ITEM);
			break;
		}
		cRMoveMenu++;
		break;
		
		// 파티 메뉴 움직이면서 등장
	case MM_PARTY_OPEN :   
		SMenu[MN_PARTY].bActive=TRUE;
		SMenu[MN_PARTY].x=SCREEN_WIDTH-64*cRMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
		if(cRMoveMenu == 5 )
		{
			cRMoveMenu=0;
			fRMoveMenu=0;
			//			RightClose();
			SMenu[nPartyState].bActive=FALSE;
			//			MenuStack.DelMenu(nPartyState);
			SMenu[MN_PARTY].bActive=TRUE;
			MenuStack.PutMenu(MN_PARTY);
			break;
		}
		cRMoveMenu++;
		break;   //이렇게 해서 마우스 입력을 막는다.
		
		// 파티 메뉴 움직이면서 퇴장
	case MM_PARTY_CLOSE :   
		if(nPartyState) SMenu[nPartyState].bActive=TRUE;  //먼저 전에 떠있었던 메뉴를 활성화 시키고
		SMenu[MN_PARTY].x=480+64*cRMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
		if(cRMoveMenu == 5)
		{
			cRMoveMenu=0;
			fRMoveMenu=0;
			SMenu[MN_PARTY].x=480;
			SMenu[MN_PARTY].bActive=FALSE;
			MenuStack.DelMenu(MN_PARTY);
			if(nPartyState != nPartyOther || nPartyState)
			{
				if(nPartyState) 
				{
					SMenu[nPartyState].bActive=FALSE;  // 메뉴를 비 활성으로 만든후
					//					MenuStack.DelMenu(nPartyState);
				}
				if(nPartyOther) 
				{
					SMenu[nPartyOther].bActive=TRUE;  // 체크된 메뉴를 활성화 시킨다.
					//					MenuStack.PutMenu(nPartyOther);
				}
			}
			nPartyState = 0;
			nPartyOther=0;
		}
		cRMoveMenu++;
		break;
	case	MN_GM_2ND_SKILL://010708 lsw 여기서 가동 !
		{
			SMenu[MN_GM_2ND_SKILL].bActive=TRUE;
			SMenu[MN_GM_2ND_SKILL].x=SCREEN_WIDTH-64*cRMoveMenu;  //32는 타일크기, 한번에 타일크기씩 이동이다.
			if(cRMoveMenu == 5 )
			{
				cRMoveMenu=0;
				fRMoveMenu=0;
				RightClose();
				SMenu[MN_GM_2ND_SKILL].bActive=TRUE;
				if( nItemOther ) 
				{
					SMenu[nItemOther].bActive = TRUE; // 아이템과 같이 떠야하는 메뉴가 있다면 같이 뜬다.
					nItemOther = 0;
					LeftMenuClose();
				}
				MenuStack.PutMenu(MN_GM_2ND_SKILL);
				break;
			}
			cRMoveMenu++;
			break;   //이렇게 해서 마우스 입력을 막는다.
		}
	default :	fRMoveMenu=0; break;
	}	
}

void CallSmallMenu(int menu, int pre, int sound)
{
	SetChatMode(CM_COMMAND,true);//021001 lsw //인터페이스 뜨면 무조건 채팅창 닫는다
	EWndMgr.ClearAllTxt();//021001 lsw
	
	if (SMenu[MN_NATION_WAR_RESULT].bActive)
	{
		return;
	}

	if (menu) 
	{
		if (SMenu[menu].bActive)
		{
			return;		// 같은 메뉴가 떠 있을 경우 리턴...
		}
		
		CloseAllMenu();		// 모든 메뉴를 닫고...
		SMenu[menu].bActive = TRUE;

		if (pre)
		{
			SMenu[menu].nTemp = pre; // default 0
		}

		y_MenuFocus = menu;
		
		if (sound != SN_MENU_OPEN)
		{
			MP3(sound);
		}
		else 
		{
			switch (menu)
			{
			case MN_MEM_BOOK:
				{
					MP3(SN_MAGIC_BOOK_OPEN);
					break;
				}
			case MN_QUEST_INDEX:				
			case MN_LEARN_MAGIC_BOOK:
				{
					MP3(SN_GET_BOOK);
					break;
				}
			case MN_DIVIDE_ITEM:
			case MN_DISTANCE_TABLE:
			case MN_STORE_SIGN:
				{
					MP3(SN_BOX_OPEN);
					break;
				}
			case MN_NATION_BATTLE_BBS:
				{
					MP3(50);
					break;
				}
			case MN_NATION_DONEBOX:
				{
					MP3(67);
					break;
				}
			case MN_NATION_WAR_START:
				{
					MP3(67);
					break;
				}
			default:
				{
					MP3(SN_MENU_OPEN);
					break;
				}
			}
		}
	}
	else
	{
		is_mainmenu_active = 0;
		y_MenuFocus = 0;
	}

	is_mainmenu_active = false;
	SoundOn = false;
}

void PrintMoney(SMENU *SubMenu, int j, DWORD result, int BoxSize)
{
	SetHangulAlign( TA_RIGHT );
	int x = SubMenu->x+SubMenu->nField[j].x+BoxSize;
	int y = SubMenu->y+SubMenu->nField[j].y;
	if( result < 1000)
	{
		Hprint2( x, y, g_DestBackBuf,"%u", result);
	}
	else if(result < 1000000)
	{
		DWORD first, second;
		first = result / 1000 ; 
		second = result % 1000 ;
		Hprint2( x, y, g_DestBackBuf,"%u,%03u", first, second);
	}
	else// if(result < 1000000000 )
	{
		DWORD first, second, third;
		first = result / 1000000;
		second = (result - first * 1000000) / 1000;
		third = result - first * 1000000 - second*1000;
		Hprint2( x,y, g_DestBackBuf,"%u,%03u,%03u", first, second, third);
	}
}


ItemAttr ExplainItemAttr;
void ItemExplain(ItemAttr item)
{
	ExplainItemAttr = item;
	return;	
}

void ItemExplain( int item )
{
	ExplainItemAttr = GenerateItem( item );
	return;
}

int ItemExplainAfterMenuDisplay( int prev )			// 나중에 설명을 띄운다.
{
	if (Hero != NULL && Hero->viewtype == VIEWTYPE_OBSERVE_)
	{	//< CSD-TW-030618
		return 0;
	}	//> CSD-TW-030618

	int ret=0;
	static ItemAttr ExplainItem_back = ExplainItemAttr;
	
	if( (ExplainItem_back.item_no != ExplainItemAttr.item_no) || ( ExplainItem_back.attr[IATTR_DURATION] != ExplainItemAttr.attr[IATTR_DURATION] ))				// 마지막 프레임에 설명한번 더 찍기
	{
		if( ExplainItem_back.item_no && !ExplainItemAttr.item_no )
		{
			ItemAttr temp = ExplainItemAttr;
			ExplainItemAttr = ExplainItem_back;
			ExplainItem_back = temp;
		}
		else ExplainItem_back = ExplainItemAttr;
	}
	
	ItemAttr &item = ExplainItemAttr;
	if( prev || !item.item_no ) 
	{
		memset( &item, 0, sizeof( ItemAttr ) );
		return 0;
	}
	
	ExplainItem2( 0, 0, &item );
	memset( &item, 0, sizeof( ItemAttr ) );
	return 1;
	
}

void MagicExplain(int num)
{
	if( !y_MenuFocus && (g_MouseItemType != 1) && num)
	{
		const BOOL bSpell = SCharacterData.nCharacterData[SPELL];
		magic[num].Explain(Hero->x-Mapx+20,Hero->y-Mapy-80,bSpell);
	}
}

void PrintDate(SMENU* SubMenu , int j, DWORD time, int width)
{
	char date[20];
	
	int year;
	int month;
	int day;
	
	year = (int) (time / 1000 / 3600 / 24 / 360); time %= (DWORD) (1000.0 * 3600.0 * 24.0 * 360.0);
	month = (int) (time / 1000 / 3600 / 24 / 30); time %= (DWORD) (1000.0 * 3600.0 * 24.0 * 30.0);
	day = (int) (time / 1000 / 3600 / 24);
	
	wsprintf(date, "%4d:%02d:%02d\0", year, month, day);
	SetHangulAlign( TA_CENTER )	;
	int p = (int)(width / 2 +0.5);
	Hprint2( SubMenu->x+SubMenu->nField[j].x+p, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", date);
}


void PrintText(SMENU* SubMenu, int j, char *str, int width)
{
	SetHangulAlign( TA_CENTER );
	int p = (int) ( width/2 +0.5 );
	Hprint2( SubMenu->x+SubMenu->nField[j].x+p, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", str);
}

bool IsMenuActive()								// 활성화된 메뉴안에 마우스가 있는지를 판단
{
	return g_MouseInMenu?true:false;
}


inline void LeftClose() // 왼쪽 창 모두 닫기 ( 팍 하고.. )
{
	SMenu[MN_STATUS].bActive=FALSE;
	SMenu[MN_ABILITY].bActive=FALSE;
	SMenu[MN_ARIGEMENT].bActive=FALSE;
	SMenu[MN_TACTICS].bActive=FALSE;
	MenuStack.DelMenu(MN_STATUS);
	MenuStack.DelMenu(MN_ABILITY);
	MenuStack.DelMenu(MN_ARIGEMENT);
	MenuStack.DelMenu(MN_TACTICS);
}

inline void RightClose()   //오른쪽 창 완전히 다 죽일때..
{
	SMenu[MN_ITEM].bActive=FALSE;
	SMenu[MN_PARTY].bActive=FALSE;
	MenuStack.DelMenu(MN_ITEM);
	MenuStack.DelMenu(MN_PARTY);
}

BOOL MenuFocus(int menu)	//메뉴 독점인지 판단
{
	if(y_MenuFocus)
	{
		if(y_MenuFocus != menu)
		{
			return true;
			switch(menu)           
			{                   
			case MN_MAININTERFACE :
			case MN_STATUS :
			case MN_ABILITY :
			case MN_ITEM :
			case MN_ARIGEMENT :
			case MN_MAKECHARACTER_CHEAK3 :
			case MN_MAKECHARACTER_ABILITY :
			case MN_TACTICS:
				return TRUE;
			default : break;
			}
		}
		else return false;
	}
	return FALSE;
}


void PutMouseItemCheck()
{
	if( g_MouseItemType && !MouseDrag && !HandItemOn && !LButtonDown )
	{
		if( g_MouseItemType==1 )
		{
			switch ( IP_base.type )
			{
			case  EQUIP:{	
				int a, b, c;
				if( SearchEmpty(a, b, c) )
				{
					InvItemAttr[a][b][c] = HandItemAttr;
					POS pos_s, pos_t;
					SetItemPos(HAND, &pos_s);
					SetItemPos(INV, a, b, c, &pos_t);
					SendMoveItem( HandItemAttr.item_no, pos_s, pos_t, false );
				}
				else
				{
					CItem* i = ItemUnit(HandItemAttr.item_no); // finito 040707 check if item is noechange an therefore can't drop
					if (!i) break; 

					if (i->GetItemKind() == IK_NOEXCHANGE) 
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString(243) );
					}
					else
					{
						SendDropItem( HandItemAttr.item_no, Hero->x, Hero->y, HandItemAttr.attr );
					}
				}
				break;
						}
				
						/*				case  EQUIP:	if(GetItemAttr(HandItemAttr.item_no, WEAR_ABLE) == WearAble[IP_base.p3] && !EquipItemAttr[IP_base.p3].item_no)		// 무기 아이템 창에서 떼어낸 아이템
						{
						POS s; 
						EquipItemAttr[IP_base.p3]= HandItemAttr;
						
						  SetItemPos(HAND, &s);
						  SendMoveItem( HandItemAttr.item_no, s, IP_base );
						  }
						  else
						  {
						  int a, b, c;
						  if( SearchEmpty(a, b, c) )
						  {
						  InvItemAttr[a][b][c] = HandItemAttr;
						  POS pos_s, pos_t;
						  SetItemPos(HAND, &pos_s);
						  SetItemPos(INV, a, b, c, &pos_t);
						  SendMoveItem( HandItemAttr.item_no, pos_s, pos_t );
						  }
						  else
						  {
						  SendDropItem( HandItemAttr.item_no, Hero->x, Hero->y, HandItemAttr.attr );
						  }
						  }
						  break;		
				*/
			case QUICK :	if(GetItemAttr(HandItemAttr.item_no, WEAR_ABLE) == WEAR_BELT)		// 단축 아이템 창에서 떼어낸 아이템
							{
								QuickItemAttr[IP_base.p3] = HandItemAttr;
								POS s; 
								SetItemPos(HAND, &s);
								SendMoveItem(HandItemAttr.item_no,s, IP_base, false);
							}
				else
				{
					int a, b, c;
					if( SearchEmpty(a, b, c) )
					{
						InvItemAttr[a][b][c] = HandItemAttr;
						POS pos_s, pos_t;
						SetItemPos(HAND, &pos_s);
						SetItemPos(INV, a, b, c, &pos_t);
						
						SendMoveItem( HandItemAttr.item_no,pos_s, pos_t, false );
					}
					else
					{
						CItem* i = ItemUnit(HandItemAttr.item_no); // finito 040707 check if item is noechange an therefore can't drop
						if (!i) break; 

						if (i->GetItemKind() == IK_NOEXCHANGE) 
						{
							Kein_PutMessage( KM_FAIL, kein_GetMenuString(243) );
						}
						else
						{
							SendDropItem( HandItemAttr.item_no, Hero->x, Hero->y, HandItemAttr.attr );
						}
					}
				}
				break;		
				
			case BANK :{
				BankItemAttr[IP_base.p1][IP_base.p2][IP_base.p3] = HandItemAttr;
				POS pos_s;
				SetItemPos(HAND, &pos_s);
				SendMoveItem( HandItemAttr.item_no,pos_s, IP_base , false);
				break;
					   }
			case GROUND : {
				int a, b, c;
				int kind = GetItemAttr(HandItemAttr.item_no, ITEM_KIND);
				if( kind == IK_NEW_MONEY )
				{
					if( SearchNewCoinItem( a, b, c ) )
					{
						POS pos_s, pos_t;
						SetItemPos(HAND, &pos_s);
						SetItemPos(INV, a,b,c, &pos_t);
						SendMoveItem( HandItemAttr.item_no, pos_s, pos_t, false );
						//ChangeItemAttr( &InvItemAttr[a][b][c], CIA_MUCH, HandItemAttr.attr[IATTR_MUCH], 0 );
						break;
					}
				}
				else if( kind == IK_MONEY  )
				{
					if( SearchCoinItem( a, b, c ) )
					{
						POS pos_s, pos_t;
						SetItemPos(HAND, &pos_s);
						SetItemPos(INV, a,b,c, &pos_t);
						SendMoveItem( HandItemAttr.item_no, pos_s, pos_t, false );
						//ChangeItemAttr( &InvItemAttr[a][b][c], CIA_MUCH, HandItemAttr.attr[IATTR_MUCH], 0 );
						break;
					}
				}
				else if( GetItemAttr( HandItemAttr.item_no, DO_RBUTTON )==DIVIDE_ITEM && SearchItemPosByNumber( &HandItemAttr, a, b, c ) )
				{
					POS pos_s, pos_t;
					SetItemPos(HAND, &pos_s);
					SetItemPos(INV, a, b, c, &pos_t);
					SendMoveItem( HandItemAttr.item_no, pos_s, pos_t , false);
					//ChangeItemAttr( &InvItemAttr[a][b][c], CIA_MUCH, HandItemAttr.attr[IATTR_MUCH], 0 );
					break;
				} 
				if( SearchEmpty(a, b, c) )
				{
					InvItemAttr[a][b][c] = HandItemAttr;
					POS pos_s, pos_t;
					SetItemPos(HAND, &pos_s);
					SetItemPos(INV, a, b, c, &pos_t);
					SendMoveItem( HandItemAttr.item_no, pos_s, pos_t, false );
				}
				else
				{
					CItem* i = ItemUnit(HandItemAttr.item_no); // finito 040707 check if item is noechange an therefore can't drop
					if (!i) break; 

					if (i->GetItemKind() == IK_NOEXCHANGE) 
					{
						Kein_PutMessage( KM_FAIL, kein_GetMenuString(243) );
					}
					else
					{
						SendDropItem( HandItemAttr.item_no, Hero->x, Hero->y, HandItemAttr.attr );
					}
				}
				break;
						  }
			default : {
				if( InvItemAttr[IP_base.p1][IP_base.p2][IP_base.p3].item_no )
				{
					int a, b, c;
					if( SearchEmpty(a, b, c) )
					{
						InvItemAttr[a][b][c] = HandItemAttr;
						POS pos_s, pos_t;
						SetItemPos(HAND, &pos_s);
						SetItemPos(INV, a, b, c, &pos_t);
						SendMoveItem( HandItemAttr.item_no, pos_s, pos_t , false);
					}
					else
					{
						CItem* i = ItemUnit(HandItemAttr.item_no); // finito 040707 check if item is noechange an therefore can't drop
						if (!i) break; 

						if (i->GetItemKind() == IK_NOEXCHANGE) 
						{
							Kein_PutMessage( KM_FAIL, kein_GetMenuString(243) );
						}
						else
						{
							SendDropItem( HandItemAttr.item_no, Hero->x, Hero->y, HandItemAttr.attr );
						}
					}
				}
				else
				{
					InvItemAttr[IP_base.p1][IP_base.p2][IP_base.p3] = HandItemAttr;
					POS pos_s;
					SetItemPos(HAND, &pos_s);
					SendMoveItem( HandItemAttr.item_no, pos_s, IP_base , false);
					break;
				}
					  }
			}
		}
		g_MouseItemNumber = 0;
		g_MouseItemType = 0;
		//		SetItemPos(HAND, &IP_base);
		DeleteItem( &HandItemAttr );
		g_DropOnHero = false;
	}
	
}

void PutMouseItem()
{
	if( g_MouseItemType==1 )
	{
		if( MouseItemInRectOn && !SMenu[MN_ITEM].bActive ) MouseItemInRectOn = 0;
		if( MouseDrag || HandItemOn || LButtonDown )
		{
			if( MouseItemInRectOn )
				PutItemIcon(MouseItemInRect.x, MouseItemInRect.y, HandItemAttr.item_no);
			else if( (HandItemAttr.item_no / 1000) == POTION )	
			{
				//MP3( SN_TOUCH_POTION );
				PutItemIconAni(g_pointMouseX, g_pointMouseY, HandItemAttr.item_no);
			}
			else	PutItemIcon(g_pointMouseX, g_pointMouseY, HandItemAttr.item_no);
		}
		ItemExplain( HandItemAttr );
	}
	else if( g_MouseItemType == PARTY )
	{
		PutCharImage( g_pointMouseX-30, g_pointMouseY-35, mouse_ch.m_ImageNo, 0, FS_SMALL, 0, mouse_ch.m_Gender );
	}
	
	
	if(MessageOn)		// 맨마지막에 메시지 처리 
	{
		kein_TextPut(249, 20+352, Message);
	}
	
	ItemExplainAfterMenuDisplay(0);			// 맨마지막에 아이템 보여주기
}

bool IsRightWindowOpen()
{
	int rightmenu[] = {	
		MN_ITEM,
			MN_PARTY,
	};
	
	int max = sizeof(rightmenu) / sizeof(int);
	for(int a=0; a<max; a++)
	{
		if (SMenu[rightmenu[a]].bActive) return true;
	}
	return false;
}

bool IsLeftWindowOpen( )
{
	int leftmenu[] = {	MN_STATUS,
		MN_ABILITY,
		MN_ARIGEMENT,
		MN_TACTICS	};
	
	int max = sizeof(leftmenu) / sizeof(int);
	
	for(int a=0; a<max; a++)
	{
		if (SMenu[leftmenu[a]].bActive) return true;
	}
	return false;
}

void	AbilityOpenWhenLevelUp()
{
	if( SCharacterData.LvUpPoint )
	{
		for(int a=0; a<11; a++)
		{
			SMenu[MN_ABILITY].nField[26+a].nType=FT_LEVELUP;
		}
	}
	else 
	{
		for(int a=0; a<11; a++)
		{
			SMenu[MN_ABILITY].nField[26+a].nType=FT_NO_CHECK;
		}
	}
}


void LoadSmallMenuImage( int num )
{
	FILE	*fp;
	
	DWORD Off=0;
	DWORD size;
	
	if( fp = Fopen("./data/Sysmenu.dat", "rb" ) )
	{
		fseek(fp, 4 * num + 2, SEEK_SET);
		fread( &Off, sizeof( DWORD ), 1, fp);
		fseek(fp, Off, SEEK_SET);
		
		fread( &size, sizeof( DWORD ), 1, fp);
		fread( &spr[ num ].xl, sizeof( short ), 1, fp);
		fread( &spr[ num ].yl, sizeof( short ), 1, fp);
		fread( &spr[ num ].ox, sizeof( short ), 1, fp);
		fread( &spr[ num ].oy, sizeof( short ), 1, fp);
		fread( &spr[ num ].size, sizeof( unsigned int ), 1, fp);
		dw_TotalImageSize += spr[ num ].size;
		
		spr[num].img = new  char[spr[num].size];

		fread( spr[ num ].img, spr[ num ].size, 1, fp);
		
		dw_TotalImageSize += spr[ num ].size;
		
		convert565to555( &spr[ num ] );
		
		fclose(fp);
	}
	else JustMsg("SysMenu.dat File Error need Update");//010215 lsw
}

int CalcTextToPage( int width, int height, char *source )
{
	int len = strlen( source );
	int char_width = width / FONTSIZE_;	// 한줄에 들어갈수 있는 글자 갯수 	
	
	int row = len / char_width;			// 총 줄 수 
	
	int page_height = height / 18;		// 한장에 들어갈수 있는 줄수 
	
	return((row / page_height)+1) ;
}

void BbsTextPut( int x, int y, int width, int height, char *source, int page )
{
	char *target;
	
	int len = strlen( source );
	int char_width = width / FONTSIZE_;
	int row = len / char_width;
	int page_height = height / 18;
	int page_char = char_width*page_height;
	
	int start_row, end_row;		// 처음 줄과 마지막 줄
	
	start_row = page*page_height;
	end_row = (page+1)*page_height - 1;
	
	target = new char[page_char+1];
	
	strncpy(target, source+start_row*char_width, page_char );
	target[page_char] = NULL;
	
	RectTextPut( x, y, width, target );	// 시작위치를 보내준다.
	delete target;	// free 는 원래의 자리에서...
}

void BbsTextPut2( int x, int y, int width, int max_line, char *source, int page )
{
	int start_row, end_row;		// 처음 줄과 마지막 줄
	
	start_row = page*max_line;
	end_row = (page+1)*max_line - 1;
	
	SkipRectTextPut( x, y, width, start_row, source, max_line, 0 );
}

//---------------------------------------------------------------------------
// 레벨업 포인터들에 대한 서버 전송 함수
//---------------------------------------------------------------------------
void SendLevelUpPoint( WORD p[11] )
{
	t_packet packet;
	
	packet.h.header.type =  CMD_LEVELUP_POINT;
	memcpy(packet.u.client_levelup_point.aPoint, p, sizeof(WORD) * 11);
	packet.h.header.size = sizeof( t_client_levelup_point );
	
	QueuePacket( &packet, 1 );
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver ) RecvLevelUpPointResult( CMD_LEVELUP_POINT_OK );
#endif
}

void RecvLevelUpPoint(t_server_levelup_point *p)
{
	LPCHARACTER ch = FindCharacter(&g_CharacterList, p->idServer);
	if( ch == NULL ) return;
	
	if( ch==Hero )
	{    
		++SCharacterData.nLevel;
		SCharacterData.LvUpPoint = p->nPoint;
		SCharacterData.nExpNext = NPC_Lev_Ref[SCharacterData.nLevel].nMaxExp;
		
		if( SCharacterData.nLevel >= 8 ) g_ExplainMenuOn = false;	// 메뉴 설명	
		// 010815 YGI 30렙 까지 자동분배
		if( SCharacterData.nLevel <= 30 && AutoLevelUpOn ) AutoLevelUpCount = g_curr_time;
	}
	
	//< CSD-011106
	ch->lv = p->nLevel;
	ch->hp = p->nLife;
	strcpy(ch->aStepInfo, p->aStepInfo);
	//> CSD-011106
	InsertMagic(ch,ch,211,-1,0,0,0,0);
	
	if (ch == Hero && SCharacterData.nLevel == 101)
	{
		CallInitAbility();
	}
}

void RecvLevelDownResult( int type )
{
	SCharacterData.EachUp[type]--;
	SCharacterData.nCharacterAbility[type]--;
	SCharacterData.LvUpPoint++;
	MP3( SN_LV_EACH_DOWN );
}

void RecvLevelUpPointResult( int ok )
{
	int a;
	switch( ok )
	{
	case CMD_LEVELUP_POINT_OK : /*for( a=0; a<11; a++ )
								{
								SCharacterData.nCharacterAbility[a]++;
								SCharacterData.LvUpPoint--;
								//										SCharacterData.EachUp[a] = 0;
	}*/
		break;
		
	case CMD_LEVELUP_POINT_NO : for( a=0; a<11; a++ ) 
								{
									SCharacterData.EachUp[a] = 0;
									SCharacterData.LvUpPoint = 0;
								}
		break;
	}
}

//---------------------------------------------------------------------------
// 상점에 관한 서버 전송 함수
//---------------------------------------------------------------------------
inline int NotUseMenuByNK( int call_menu )
{
	static int menu[] = {
		MN_BANK_MAIN,
			MN_BLACKSMITH,
			MN_FARM,
			MN_RESTAURANT,
			MN_HERBSHOP	,	
			MN_SHAMBLES	,	
			MN_CANDLE	,	
			MN_SLOPSHOP	,	
			MN_ALCHEMY	,	
			MN_WOODWORKING	,
			MN_BOWCRAFT		,
			MN_SHIPYARD,
			MN_TOWNHALL,
			MN_DRINK_MAIN,
			MN_SHOP_MAIN,
			MN_HEAL_MAIN,
			MN_INN_MAIN,
			MN_MAGIC_GUILD,
			MN_ACADEMY,
			MN_GUILD_ADMIN,
			MN_SALVATION2,
			MN_MERCHANT_MAIN,
			MN_NATION_MAIN_VA,
			MN_NATION_MAIN_ZY,
			MN_NATION_MAIN_ILL,
	};
	
	static int max = sizeof( menu ) / sizeof( int );
	for( int i=0; i<max; i++ )
	{
		if( menu[i] == call_menu ) return 0;
	}
	return 1;
}
//011011 lsw >
void RecvCallSmallMenuCustom( t_server_call_smallmenucustom *p)
{
	int iMenuNo = 	p->iMenuNo;
	SMenu[iMenuNo].nImageNumber = p->iPicNo;
	
	if( p->iExt )
	{
		//011022 lsw >
		SMenu[iMenuNo].nImageType = p->iExt;
		//011022 lsw <
	}
	
	SMenu[iMenuNo].nField[0].x = (p->iWidth-161);
	SMenu[iMenuNo].nField[0].y = (p->iHeight);
	
	SetRect(SMenu[iMenuNo].nField[0].rCheakBox,SMenu[iMenuNo].nField[0].x,SMenu[iMenuNo].nField[0].y,
		SMenu[iMenuNo].nField[0].x+200,SMenu[iMenuNo].nField[0].y+50);
	
	CallSmallMenu( p->iMenuNo );
	return;
}
//011011 lsw <

//<soto-030421
void	RecvCallDualMagicLearnBook()
{
	SetChatMode(CM_COMMAND,true);//021001 lsw //인터페이스 뜨면 무조건 채팅창 닫는다
	EWndMgr.ClearAllTxt();//021001 lsw
	if( SMenu[MN_NATION_WAR_RESULT].bActive ) return;

	int menu = MN_LEARN_MAGIC_BOOK;
	CallSmallMenu(menu);
	
	g_bDualMagicBookOpen = true;
}
//>soto-030421

void RecvCallSmallMenu(t_server_call_smallmenu* p)
{	//< CSD-030506
	switch (p->menu)
	{
	case MN_ARENA_LOBBY:
		{
			StartTembattleWaitMenu(p->storelistindex);
			break;
		}
	case MN_GAMBLE:
		{
			CallGambleMenu();
			break;
		}
	case MN_GUILD_MEMBER_LIST:
		{
			CallGuildMemberListMenu();
			break;
		}
	case MN_NATION_ITEM:
		{
			CallNationItem();
			break;
		}
	case MN_GUILD_HOUSE:
		{
			CallGuildHouseInfo();
			break;
		}
	case MN_DUAL_MAIN:
		{
			CallDualInterFace(p->storelistindex);
			break;
		}
	default:
		{	// 나랑 다른 맵에서 nk 수치가 5이상일때 몇몇 메뉴는 이용하지 못함
			if (MapInfo[MapNumber].nation != Hero->name_status.nation && IsNK(Hero, MapInfo[MapNumber].nation) >= 5)
			{
				if (!NotUseMenuByNK(p->menu)) 
				{
					MP3(SN_WARNING);
					Kein_PutMessage(KM_FAIL, kein_GetMenuString(41));
					return;
				}
			}

			storelistindex = p->storelistindex;
			CallSmallMenu(p->menu);
			break;
		}
	}
}	//> CSD-030506

void ReqSmallMenu( int menu, int storelistindex )
{
	t_packet p;
	
	p.h.header.type = CMD_CALL_SMALLMENU;
	p.u.server_call_smallmenu.menu = menu;
	p.u.server_call_smallmenu.storelistindex = storelistindex;
	p.h.header.size = sizeof( t_server_call_smallmenu );
	QueuePacket( &p, 1 );
}		

void RecvStoreItemList( t_server_store_itemlist *p )	// 서버에서 받아온 아이템 속성과 번호를 필요한 변수에 셋팅한다.
{		
	int menu = p->smallmenu;
	aSellBuyItemMax = p->no;
	if( SMenu[MN_SKILLGUILD_ITEM].bActive )
	{	
		
	}
	else
	{
		if( is_buymenu_On )		// buy메뉴에서는 목록 그대로를 보내주고 sell메뉴에서는 내가 가지고 있는 목록에서 찾아서 보여준다.
		{
			int ct=0;
			int ret_msg = false;
			for( int i=0 ; i<p->no ; i++)
			{
				if( p->store_item[i].item < 0 )
				{
#ifdef _DEBUG // finito 060507
					if( GetSysInfo( SI_GAME_MAKE_MODE ) && !ret_msg )
					{
						JustMsg( lan->OutputMessage(6,37) );//010215 lsw
						ret_msg = true;
					}
#endif
				}
				
				if( p->store_item[i].item > 0 && GetItemAttr(p->store_item[i].item, IMAGE_NUMBER) )		// 이미지가 있는 아이템만...
				{
					CItem *t = ItemUnit( p->store_item[i].item/1000, p->store_item[i].item%1000 );
					aSellBuyItemAttr[ct].item_no = p->store_item[i].item;
					int time = GetItemAttr( p->store_item[i].item, ITEM_LIMIT );
					if( time>0 ) 
						aSellBuyItemAttr[ct].attr[IATTR_LIMIT] = g_curr_time+time;
					ct++;
				}
			}
			aSellBuyItemMax = ct;
		}
		else
		{
			if( menu == MN_SKILLGUILD_REPAIR )
			{
				int offset = 0;
				for( int a = 0; a< 3; a++ )
					for( int b = 0; b < 3 ; b++ )
						for( int c = 0; c< 8 ; c++)
						{
							int item_no = InvItemAttr[a][b][c].item_no;
							int able = GetItemAttr( item_no, REPAIR_ABLE );
							if( item_no && able )
							{	
								for( int i=0; i<p->no; i++)
								{
									if( p->store_item[i].item == InvItemAttr[a][b][c].item_no )
									{
										aSellBuyItemAttr[offset] = InvItemAttr[a][b][c];
										aSellBuyItemPos[offset] = (a<<5) | (b<<3) | c ;
										offset++;
										break;
									}
								}
							}
						}
						aSellBuyItemMax = offset;
			}
			else 
			{
				int offset = 0;
				for( int a = 0; a< 3; a++ )
					for( int b = 0; b < 3 ; b++ )
						for( int c = 0; c< 8 ; c++)
							if( InvItemAttr[a][b][c].item_no )
							{
								for( int i=0; i<p->no; i++)
								{
									if( p->store_item[i].item == InvItemAttr[a][b][c].item_no )
									{
										aSellBuyItemAttr[offset] = InvItemAttr[a][b][c];
										aSellBuyItemPos[offset] = (a<<5) | (b<<3) | c ;
										offset++;
										break;
									}
								}
							}
							aSellBuyItemMax = offset;
			}
		}
		storelistindex = p->storelistindex;
		CallSmallMenu( p->smallmenu, ConvStoreListToMenu( p->storelistindex, true ) ); 
	}
}

void RecvMallItemList(t_MallItemList *p)
{
	int itemCount = 0;
	for (int i = 0; i < p->mallItemCount; ++i)
	{
		if( p->mallItems[i].item > 0 && GetItemAttr(p->mallItems[i].item, IMAGE_NUMBER))
		{
			CItem *t = ItemUnit(p->mallItems[i].item/1000, p->mallItems[i].item%1000);
			itemMallAttr[itemCount].item_no = p->mallItems[i].item;
			++itemCount;
		}
	}
	itemMallMax = itemCount;
}

void SetPartyMenu( int type )
{
	int i;
	switch( type )
	{
		
	case RELATION :		for( i=9 ; i<12; i++ ) SMenu[MN_PARTY].nField[i].nType = FT_NO_CHECK;
		SMenu[MN_PARTY].nImageNumber=849; 
		SMenu[MN_PARTY].nField[5].nType=FT_NO_CHECK;
		
		// 1번
		SMenu[MN_PARTY].nField[6].x=55;
		SMenu[MN_PARTY].nField[6].y=258;
		SetRect(SMenu[MN_PARTY].nField[6].rCheakBox,25,223,85,292);
		// 2번
		SMenu[MN_PARTY].nField[7].x=124;
		SMenu[MN_PARTY].nField[7].y=258;
		SetRect(SMenu[MN_PARTY].nField[7].rCheakBox,94,223,154,292);
		// 3번
		SMenu[MN_PARTY].nField[8].x=193;
		SMenu[MN_PARTY].nField[8].y=258;
		SetRect(SMenu[MN_PARTY].nField[8].rCheakBox,163,223,233,292);
		// 서버 콜		// 파티원들에 대한 정보를 가져 오려면 여기서
		SendReqPartyInfo( RELATION, 0 );
		
		break;
		
	case EMPLOYMENT :	
	case PARTY :		for( i=9 ; i<12; i++ ) SMenu[MN_PARTY].nField[i].nType = FT_PARTY;
		SMenu[MN_PARTY].nImageNumber=464;
		SMenu[MN_PARTY].nField[5].nType=FT_NO_CHECK;
		
		// 1번
		SMenu[MN_PARTY].nField[6].x=55;
		SMenu[MN_PARTY].nField[6].y=217;
		SetRect(SMenu[MN_PARTY].nField[6].rCheakBox,25,182,85,252);
		// 2번
		SMenu[MN_PARTY].nField[7].x=124;
		SMenu[MN_PARTY].nField[7].y=217;
		SetRect(SMenu[MN_PARTY].nField[7].rCheakBox,94,182,154,252);
		// 3번
		SMenu[MN_PARTY].nField[8].x=193;
		SMenu[MN_PARTY].nField[8].y=217;
		SetRect(SMenu[MN_PARTY].nField[8].rCheakBox,163,182,233,252);
		
		if( type == EMPLOYMENT ) 
		{
			SMenu[MN_PARTY].nField[5].nType=FT_NOMAL_PUT;
			SendReqPartyInfo( EMPLOYMENT, 0 );
		}
		else 
		{
			SendReqPartyInfo( PARTY, 0 );
		}
		break;
	}
	SMenu[MN_PARTY].nTemp = type;
}

void LeftMenuClose()	// 움직이면서...
{
	if(SMenu[MN_STATUS].bActive) fLMoveMenu=MM_STATUS_CLOSE;
	else if(SMenu[MN_ABILITY].bActive)  fLMoveMenu=MM_ABILITY_CLOSE;
	else if(SMenu[MN_ARIGEMENT].bActive)
	{
		SMenu[MN_STATUS].bActive=FALSE;
		MenuStack.DelMenu(MN_STATUS);
		SMenu[MN_ABILITY].bActive=FALSE;
		MenuStack.DelMenu(MN_ABILITY);
		fLMoveMenu=MM_ARIGEMENT_CLOSE;
	}
	else if( SMenu[MN_TACTICS].bActive) fLMoveMenu=MM_TACTICS_CLOSE;
}

void CountingByKeyboard( int input_key, int &count, int plus )
{
	static int flag = 0;
	if(g_aCurrentKeys[DIK_NUMPAD5] & 0x80) 
	{
		if(flag!=2) 
		{
			count += plus; 
		}
		flag = 2;
	}
	else flag = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//this3 lsw 속성 출력부1-1
bool	g_SokSungFlag;
int sokAbil[8]={0,};

inline int ActiveSoksung( int ability[], RareMain &sok_item, int is_first )
{
	int type;
	if( is_first ) type = sok_item.soksung1;
	else type = sok_item.soksung2;
	
	static int ab_to_type[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS, WSPS, };
	switch( type )
	{
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
		ability[ab_to_type[type -33]] += ItemRare[type].GRADE[sok_item.grade];
		break;
	case 43:
		if( Hero->spell == WIZARD_SPELL)//010615 lsw
		{
			ability[ab_to_type[type -33]] += ItemRare[type].GRADE[sok_item.grade];
		}
		break;
	case 44:
		if( Hero->spell == PRIEST_SPELL)//010615 lsw
		{
			ability[ab_to_type[type -33]] += ItemRare[type].GRADE[sok_item.grade];
		}
		break;
	}
	
	return 1;
}

inline int LoofSetHeroAbilitySokSung(int ability[] )
{
	if( !g_SokSungFlag ) return 0;
	
	static RareMain sok_item;
	for(int i=0;i<8;i++)
	{
		memcpy( &sok_item, &EquipItemAttr[i].attr[IATTR_RARE_MAIN], sizeof( int ) );
		
		if( sok_item.soksung1 )
			ActiveSoksung( ability, sok_item, 1 );
		if( sok_item.soksung2 )
			ActiveSoksung( ability, sok_item, 0 );
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void LoofSetHeroAbility( int ability[] )	// 캐릭터의 토탈 능력치 셋
{
	if( g_Menu_Variable.m_bChangeColor )		// 옷 색깔을 원래로 되돌린다.
	{
		Hero->bodycolor = ReturnBlendRGB( SCharacterData.body_r, SCharacterData.body_g, SCharacterData.body_b );
		Hero->clothcolor= ReturnBlendRGB( SCharacterData.cloth_r, SCharacterData.cloth_g, SCharacterData.cloth_b );
		g_Menu_Variable.m_bChangeColor = 0;
	}
	
	for( int i=0; i<11; i++ )//020821-2 lsw
	{	
		ability[i] = SCharacterData.nCharacterAbility[i]+SCharacterData.nAbilityPlusOfWeapon[i];//+SCharacterData.nAbilityPlusOfPotion[i];
	}
	LoofSetHeroAbilitySokSung( ability );
	for( int iCt=0; iCt<11; iCt++ )//020821-2 lsw
	{
		if (ability[iCt] > g_mgrDual.GetAbility(iCt))
		{
			ability[iCt] = g_mgrDual.GetAbility(iCt);
		}
	}
	//< CSD-030324
	/*
	// thai2 YGI
	if( SCharacterData.nCharacterData[ARIGEMENT] < 0 || SCharacterData.nCharacterData[ARIGEMENT] > 9 )
	{
		JustMsg( lan->OutputMessage(6,40) );//010215 lsw
		SCharacterData.nCharacterData[ARIGEMENT] = 0;
	}
	*/
	//> CSD-030324
#ifdef CHANGE_MONEY_
	SCharacterData.nMoney = GetNewMoneyByItem();			// 아이템으로 계산하는 돈
#else
	SCharacterData.nMoney = GetMoneyByItem();			// 아이템으로 계산하는 돈
#endif
	if( IsExchangeNow() )//거래 중이네
	{	SMenu[MN_EXCHANGE_ITEM].bActive = true;}//교환중이네 인터페이스 열어
	else
	{	SMenu[MN_EXCHANGE_ITEM].bActive = false;}
	
	//화면 중앙 상단의 국가및 좌표 메뉴
	if( g_bTimeMenuOn || Hero->viewtype == VIEWTYPE_GHOST_ )
	{	SMenu[MN_TIME_DISPLAY_MENU].bActive = true;}
	else
	{	SMenu[MN_TIME_DISPLAY_MENU].bActive = false;}//020515 lsw
	
	//////// 서버와 연결이 끊길때 메시지 창 출력 /////////
	static int old_connect = connections.state;
	if( old_connect == 1 && connections.state == 0 )
	{
		MP3( SN_WARNING );
		CallOkCancelMessageBox( 0,0,0, lan->OutputMessage(6,42),0 );//010215 lsw
	}
	if( old_connect != connections.state )
	{	old_connect = connections.state;}
	
	////////////////// LEVEL UP MENU check ////////////////////////
	if( !SCharacterData.LvUpPoint || IsLeftWindowOpen() ) 
	{	SMenu[MN_LEVELUP].bActive = false; }
	else 
	{	SMenu[MN_LEVELUP].bActive = true; }
	CallCombatSkillDoingMenu();//020515 lsw
	SMenu[MN_MAININTERFACE].bActive=true;//무슨 일이 있어도 메인 인터페이스는안꺼진다.//020515 lsw
	SMenu[MN_CHATTING].bActive=true;//무슨 일이 있어도 메인 인터페이스는안꺼진다.//020620 lsw
	//////////////////// 깨어나기 메뉴 /////////////////////////////
	if( SCharacterData.sleep ) 
	{
		SetVolumeBySleep(90);
		MP3( SN_SLEEP );	 // 잠자기 음악...
		SMenu[MN_AWAKE ].bActive = true;
	}
	else 
	{
		SMenu[MN_AWAKE ].bActive = false;
	}	

	SetVolume( system_info.sound );
	SetVolumeMusic( system_info.music);		
	
	PrintNoteMeeting( g_Note_Meeting );		// 신전 예배가 시작하기 1시간전에 알려 준다.
	CheckPeaceBattle();		// battle, peace mode
	#ifndef _SDL
	if(::IsChatBoxActive())
	{
		SMenu[MN_QUICK_MAGIC_PLUS].y	= 494;
		SMenu[MN_QUICK_MAGIC_SCROLL].y	= 441;
		SMenu[MN_QUICK_SKILL].y			= 426;
	}
	else
	{
		SMenu[MN_QUICK_MAGIC_PLUS].y	= 494+24;
		SMenu[MN_QUICK_MAGIC_SCROLL].y	= 441+24;
		SMenu[MN_QUICK_SKILL].y			= 426+25;
	}
#else
	if (::IsChatBoxActive())
	{
		SMenu[MN_QUICK_MAGIC_PLUS].y = 494;
		SMenu[MN_QUICK_MAGIC_SCROLL].y = 441;
		SMenu[MN_QUICK_SKILL].y = 426;
	}
	else
	{
		SMenu[MN_QUICK_MAGIC_PLUS].y = 494 + 24;
		SMenu[MN_QUICK_MAGIC_SCROLL].y = 441 + 24;
		SMenu[MN_QUICK_SKILL].y = 426 + 25;
	}
#endif
	if( g_MagicMenuOn && !SMenu[MN_QUICK_SKILL].bActive )		// 매직 창 열기
	{
		SMenu[MN_QUICK_MAGIC_PLUS].bActive = true;
		SMenu[MN_TIME_DISPLAY_ON_OFF].bActive = true;
		SMenu[MN_QUICK_MAGIC_SCROLL].bActive = true;		
	}
	else 
	{
		SMenu[MN_QUICK_MAGIC_PLUS].bActive = false;
		SMenu[MN_TIME_DISPLAY_ON_OFF].bActive = false;
		SMenu[MN_QUICK_MAGIC_SCROLL].bActive = false;
	}
	
	if( g_FightMapStart ) OpenFightMapSignMenuOpen( fight_map_live_man );		// 전광판 띄워주기..
	else SMenu[MN_FIGHT_MAP_SIGN].bActive = false;
	
	CheckFocusMenu();		// 잘못된 y_MenuFocus를 바로 잡는다.
	
	if( g_VoteSignExist <= 0 )
	{SMenu[MN_VOTE_SIGN].bActive = false;}
	else 
	{SMenu[MN_VOTE_SIGN].bActive = true;}
	
	ViewGuildCommandInfo2( );	// 길마 명령의 정보를 보여준다. 실제로 정보를 찍어 준다.
	ViewEarCommandInfo2( );
	
	if( ViewTipToggle && !ViewTipsFlag )		// tip 보기
	{	SMenu[MN_VIEW_TIP].bActive = true;	}
	else
	{	SMenu[MN_VIEW_TIP].bActive = false;}
	
	if( AutoLevelUpOn && AutoLevelUpCount )
	{
		int gab = g_curr_time - AutoLevelUpCount;
		static int gab2 = -1;
		if( gab2 != gab ) 
		{
			if( gab >= 30 )		// 30 초가 지나면
			{
				AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(6,43) );//010215 lsw
				AutoLevelUp();		// 자동 분배 및 메시지 출력 까지
			}
			else if( !(gab%10) ) 
			{
				AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(6,44),30-gab );//010215 lsw
				AddCurrentStatusMessage( 255,255,0,  lan->OutputMessage(6,45));//010215 lsw
				AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(6,46) );//010215 lsw
			}
			gab2 = gab;
		}
	}
	
	if( g_BattleStatusView )		// 팀배틀 대기 메뉴
	{
		if( IsRightWindowOpen() || IsLeftWindowOpen() )
		{
			SMenu[MN_ARENA_LOBBY].bActive = false;
		}
		else
		{
			if( !SMenu[MN_ARENA_LOBBY].bActive )
			{
				CallMenu( MN_ARENA_LOBBY );
			}
		}
	}
	else
	{
		if( SMenu[MN_ARENA_LOBBY].bActive )
		{
			MenuSoundClose( MN_ARENA_LOBBY );
			//SmallMenuClose();
		}
	}

	if( SMenu[MN_SMALL_TIPS].bActive )
	{
		if( bSmallTips )
		{
			if( IsRightWindowOpen() || IsLeftWindowOpen() ) bSmallTips = false;
		}
		
		if( !bSmallTips ) MenuSoundClose( MN_SMALL_TIPS );
	}
	else if( bSmallTips)
	{
		CallMenu( MN_SMALL_TIPS );
	}
	
	// 021024 kyo	타이머랑 카운터
	if( g_cQuest.m_cTimer != NULL )	
		SMenu[NM_SCRIPT_TIMMER].bActive = g_cQuest.m_cTimer->GetShowMenu();
	else
		SMenu[NM_SCRIPT_TIMMER].bActive = false;
	
	if( g_cQuest.m_cCounter != NULL )
		SMenu[NM_SCRIPT_COUNTER].bActive = g_cQuest.m_cCounter->GetShowMenu();
	else
		SMenu[NM_SCRIPT_COUNTER].bActive = false;
}


void PrintNoteMeeting( int g_Note_Meeting )		//  신전 예배가 시작하기 1시간전에 알려 준다.
{
	if( !g_Note_Meeting ) return;
	
	AddCurrentStatusMessage( 210, 180, 0, g_Note_Meeting_message ); 
	g_Note_Meeting = 0;
	
	
	static int count = 0; 
	if( !(count%15) )	AddCurrentStatusMessage( 210, 180, 0, g_Note_Meeting_message ); 
	if( count > 75 )	// 5번 출력후
	{
		g_Note_Meeting = 0;
		count =0; 
		return;
	}
	
	count++;
}



////////////////////////////////////////////////////////////////////////////////////////
//
//		매직 관련 계산 메소드 --> 서버
//
//
////////////////////////////////////////////////////////////////////////////////////////
int GetMagicCastingTime( int magic_no, SCharacter &ch )
{
	if( ch.nCharacterData[SPELL] )
	{
		ch.magic_casting_time[magic_no] = g_lpMT[300+magic_no].cast_Time+ (g_lpMT[300+magic_no].basic_Magery - ch.nLevel);
	}
	else
	{
		ch.magic_casting_time[magic_no] = g_lpMT[magic_no].cast_Time+ (g_lpMT[magic_no].basic_Magery - ch.nLevel);
	}
	
	return ch.magic_casting_time[magic_no];
}
///////////////////////////////////////////////////////////////////////////////
// 만들어진 아이템 연출하는 창 초기화 함수 
///////////////////////////////////////////////////////////////////////////////
extern MakeItem make_item_result;
void SetItemMakeOk( ItemAttr item )
{
	SMenu[MN_ITEM_MAKE_OK].bActive=true;
	SMenu[MN_ITEM_MAKE_OK].x=100;
	SMenu[MN_ITEM_MAKE_OK].y=140;
	SMenu[MN_ITEM_MAKE_OK].nFieldCount =3 ;
	SMenu[MN_ITEM_MAKE_OK].nImageNumber=944;
	
	// MAKE button
	SMenu[MN_ITEM_MAKE_OK].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
	SMenu[MN_ITEM_MAKE_OK].nField[0].nSpecialWillDo=SWD_ITEM_MAKE_AFTER;
	SMenu[MN_ITEM_MAKE_OK].nField[0].x=15;
	SMenu[MN_ITEM_MAKE_OK].nField[0].y=182;
	SMenu[MN_ITEM_MAKE_OK].nField[0].nRectImage=945;
	SMenu[MN_ITEM_MAKE_OK].nField[0].nImageNumber=946;
	SetRect(SMenu[MN_ITEM_MAKE_OK].nField[0].rCheakBox,15,182,101,216);
	
	// item 출력 
	SMenu[MN_ITEM_MAKE_OK].nField[1].nType = FT_MAKE_ITEM_OK ;
	SMenu[MN_ITEM_MAKE_OK].nField[1].nSHideNomalCount = 30;
	SMenu[MN_ITEM_MAKE_OK].nField[1].nSHideNomalStart = 0;
	SMenu[MN_ITEM_MAKE_OK].nField[1].nShideNomalPlus = 4;
	SetRect(SMenu[MN_ITEM_MAKE_OK].nField[1].rCheakBox,21,81,180,174);
	
	make_item_result.item_no = item.item_no;
	make_item_result.attr = item;
	make_item_result.x = 92+15;
	make_item_result.y = 74+15;
	SetRect( &make_item_result.rect, make_item_result.x-15,make_item_result.y-15,make_item_result.x+15,make_item_result.y+15 );
}

int UseItemByRbutton( POS pos_s, ItemAttr &item_attr )
{
	if( item_attr.item_no && ( !MouseDrag || g_MouseItemType == 1 ) )
	{
		int type = GetItemAttr( item_attr.item_no, DO_RBUTTON );
		if( ( item_attr.item_no == 3052 && ( SCharacterData.nCharacterData[GENDER]==MALE ) ) || ( item_attr.item_no == 3053 && ( SCharacterData.nCharacterData[GENDER]==FEMALE ) ) )
		{
			type = USE_ITEM;
		}
		if(  item_attr.item_no == 10294 )//Redistribution of Combat Skill 
		{
			CallServer(CMD_COMBAT_RESET);
			type = USE_ITEM;
			PlayListAutoSounds( 67,0,0,0 ); 
		}
		
		switch( type )
		{
		case REMAIN_ITEM	:	
		case USE_ITEM		:	
			{
				for( int a=0; a<max_submenu; a++ )	SMenu[y_ItemSubMenu[a]].bActive = false;
				if( IsHeroDead() ) return 0;
				EatItem(item_attr,pos_s);
				break;
			}
		case DIVIDE_ITEM	:	{	
			if( pos_s.type == BOX )
			{
				MP3( SN_WARNING );
				break;
			}
			for( int a=0; a<max_submenu; a++ )	SMenu[y_ItemSubMenu[a]].bActive = false;
			LeftClose();							
			CallMenu(MN_DIVIDE_ITEM, 0); 
			SMenu[MN_DIVIDE_ITEM].nField[0].nSHideNomalCount = 0;
			SMenu[MN_DIVIDE_ITEM].nField[0].nSHideNomalStart = 0;
			SMenu[MN_DIVIDE_ITEM].nField[0].nShideNomalPlus = 0;
			y_MenuFocus = MN_DIVIDE_ITEM;
			i_DivideItem.item_attr = item_attr;
			i_DivideItem.item_pos = pos_s;
			break;
								}
		case MAGIC_BOOK_ITEM:{
			for( int a=0; a<max_submenu; a++ )	SMenu[y_ItemSubMenu[a]].bActive = false;
			if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) break; 
#ifdef _DEBUG // finito 060507
			if( GetSysInfo( SI_GAME_MAKE_MODE ) ) RecvCanMemorizeOk();
#endif
			CallServer( CMD_CAN_MEMORIZE );
			break;
							 }
		case DIARY_ITEM		:	break;
		case 10: // finito 23/06/08 - some of mall items need to open a menu
			{
				switch (GetItemAttr(item_attr.item_no, RBUTTON_WAVE))
				{
				case 1: // Repair durability
					break;
				case 2: // Application of exile
					SendApplicationOfExile(Hero); //Eleval 12/06/09
					break;
				case 3: // Call bank
					break;
				case 4: // Mantle Dye
					{
						CItem * item = ItemUnit(Hero->mantle);
						if (item)
						{
							int color = EquipItemAttr[WT_NECK].attr[IATTR_RESERVED1];
							if(color > 0)
							{
								int r, g, b;
								r = (color>>16);
								color -= (r<<16);
								g = (color>>8);
								color -= (g<<8);
								b = color;

								SCharacterData.mantle_r = r;
								SCharacterData.mantle_g = g;
								SCharacterData.mantle_b = b;
							}
							else
							{
								SCharacterData.mantle_r = item->GetItemColorR();
								SCharacterData.mantle_g = item->GetItemColorG();
								SCharacterData.mantle_b = item->GetItemColorB();
							}
							CallSmallMenu(MN_MANTLE_COLOR_CHANGE);
						}

						break;
					}
				case 5: //Eleval 12/06/09 - Sex change
					SendSexChange(Hero);
					break;
				}
				break;
			}
		}
		return 1;
	}
	else return 0;
}

void CallGambleMenu()
{
	CallServer( CMD_GET_TODAY_GAMBLE_ITEM );
	fRMoveMenu = MM_ITEM_OPEN;
	nItemOther = MN_GAMBLE;
	SMenu[MN_GAMBLE].key = 0;
	y_MenuFocus = 0;
	memset( make_item_list, 0, sizeof(MakeItem)*20);
}

LPDIRECTDRAWINFO curr_direct_draw_info;

void PrintTextOnMenu()
{
	SetHangulAlign( TA_LEFT );
	Hcolor( 255, 255, 0 );
	if( g_Menu_Variable.m_bOXTime )
	{
		if( g_Menu_Variable.m_OXTime > g_curr_time )
		{
			static int rand_time = g_curr_time;		// 깜박이게...
			int time = g_Menu_Variable.m_OXTime - g_curr_time;
			int hour = time/60;
			int minute = time%60;
			static int old_hour = hour;
			if( rand_time == g_curr_time )
			{
				Hprint2( 750, 20, NULL, "%02d : %02d", hour, minute );
			}
			else
			{
				if( old_hour == hour )
				{
					Hprint2( 750, 20, NULL, "%02d :", hour );
				}
				else
				{
					old_hour = hour;
				}
				rand_time = g_curr_time;
			}
		}
		else
		{
			g_Menu_Variable.m_bOXTime = 0;
		}
	}
}

// thai2 YGI
extern int g_BoadAndSignNumber;
void PrintTextOnMenu2()
{	//< CSD-030324
#ifdef _DEBUG
	if( !GetSysInfo(SI_GAME_MAKE_MODE ) )
	{
		return;
	}
	SetHangulAlign( TA_LEFT );
	Hcolor( 255, 255, 255 );
	
	const int iX = 10, iY = 10;
	const int iWidth = TXT_BG_NORMAL_WIDTH;//021001 lsw
	int iCt = 0;
	int iReultLineCount = 0;
	
	int iBlkG = 0;
	int iReultBlankSize = 0;
	
	const int iLG = TxtOut.GetLineGab();
	const int iBlankGabSize = TxtOut.GetBlankGabSize();
	
	TxtOut.Init(COLOR_BLACK,iWidth,9);
	TxtOut.PutTextHeaderBGImg(iX,iY+iBlkG+iLG*iCt,iWidth);
	
	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"FPS:[%d]",g_FrameMgr.GetFrameCount() );
	iCt += iReultLineCount;
	
	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"y_MenuFocus:[%d]", y_MenuFocus );
	iCt += iReultLineCount;
	
	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"Nation:[%s]", NationName[SCharacterData.nation] );
	iCt += iReultLineCount;
	
	// thai2 YGI
	if( g_BoadAndSignNumber )	
	{
		if( g_BoadAndSignNumber > 10000 )
		{
			iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"%s.DSD :[%d]", MapName, g_BoadAndSignNumber-10000 );
		}
		else
		{
			iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"%s.BSD :[%d]", MapName, g_BoadAndSignNumber);
		}

		iCt += iReultLineCount;	
	}
	else
	{
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"No sign" );
		iCt += iReultLineCount;
	}

	//const int iIndex = EWndMgr.GetFocusWndIndex();
	//iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"WndFocus:[%d(Index%d),API(%d)]", EWndMgr.GetHWnd(iIndex),iIndex,GetFocus());
	//iCt += iReultLineCount;
	
	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"Current Menu Number [%d]",g_MouseInMenu);
	iCt += iReultLineCount;
	
	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"Connect Status:[%d]", connections.state );
	iCt += iReultLineCount;
	
	if( SysInfo.notconectserver == 0 )
	{	
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"R:[%d] S:[%d]", g_RecvBytes, g_SendBytes );
		iCt += iReultLineCount;
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"0:[%d] 1:[%d]", connections.packet_count[0], connections.packet_count[1]);
		iCt += iReultLineCount;
	}
	else
	{
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"Connection Close");
		// 021117 YGI
		iCt += iReultLineCount;
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"TotalMapObject : [%d]", TotalMapObject );
		iCt += iReultLineCount;
	}
	
	TxtOut.PutTextTailBGImg(iX,iY+iBlkG+iLG*iCt,iWidth);
#endif
}	//> CSD-030324

void DropItemOnHero( ItemAttr &item_attr )
{
	CItem* i = ItemUnit(item_attr.item_no); // finito 040707 check if item is noechange an therefore can't drop
	if (!i) return; 

	if (i->GetItemKind() == IK_NOEXCHANGE) 
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString(243) );
	}
	else if(g_allowDrop) //Eleval 22/05/09: Added to disable drop on a char if g_allowDrop == false
	{
		SendDropItem( item_attr.item_no, Hero->x, Hero->y, item_attr.attr );
		ItemSoundOfDropItem( item_attr.item_no );		// sound
	}
}

void CallOtherCharacterInv( ItemAttr inv[3][3][8], ItemAttr equip[8], ItemAttr quick[7] )
{
	//	ItemAttr You.inv[100];
	int ct = 0;
	for( int a=0; a<3; a++)
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				You.inv[ct++] = inv[a][b][c];
			}
			
			int c;
			for( c=0; c<8; c++ )
				You.inv[ct++] = equip[c];
			
			for( c=0; c<7; c++ )
				You.inv[ct++] = quick[c];
			
			fRMoveMenu = MM_ITEM_OPEN ;
			nItemOther = MN_CHARACTER_ITEM;
			
			You.id = Hero->id;
}

#define MAX_ITEM_BY_SKILL	100

void SetLearnItemData( int menu, ItemJoinSelect list[MAX_LEARN_ITEM] )
{
	menu -= MN_BLACKSMITH;	// 상점 번호
	int i;
	memset( list ,  0, sizeof( ItemJoinSelect )*MAX_LEARN_ITEM );
	
	int count=0;
	for( i=0; i<ITEM_JOIN_MAX; i++)
	{
		if( item_join[i].item_id )
		{
			if( item_join[i].teach_place == menu_to_set[menu] )
			{
				int item_no = item_join[i].item_id;
				if( GetItemAttr( item_no, IMAGE_NUMBER) )
				{
					list[count].item_no = item_no;
					list[count].join_no = i;
					list[count].skill_min	= item_join[i].skill1_minimum;
					list[count].kind		 = item_join[i].need_skill1;
					list[count].skillability = ItemUnit(item_no)->GetSkillAbility();
					
					count ++;
				}
			}
		}
	}
	
	{
		//배울때는 배웠나 안배웠나 나오고..
		//스킬 Kind JOin 테이블 OK
		//SKill_min
		//아이템 넘버
		int i, j ;
		
		int n = count;
		
		ItemJoinSelect t;
		
		ItemJoinSelect *a = &list[0];
		
		for (i = 1; i < n;i++)
		{
			t = a[i];
			j = i;
			
			while(a[j-1].kind > t.kind && j > 0 )//삽입될 곳 찾음
			{
				a[j] = a[j-1];//뒤로 옮김
				j--;
			}
			a[j] = t;//삽입
		}
		
		for (i = 1; i < n;i++)
		{
			t = a[i];
			j = i;
			while(a[j-1].skill_min> t.skill_min&& j > 0 && a[j-1].kind == t.kind)//삽입될 곳 찾음
			{
				a[j] = a[j-1];//뒤로 옮김
				j--;
			}
			a[j] = t;//삽입
		}
		
		
		for (i = 1; i < n;i++)
		{
			t = a[i];
			j = i;
			
			while(a[j-1].item_no > t.item_no && j > 0 && a[j-1].skill_min== t.skill_min)//삽입될 곳 찾음
			{
				a[j] = a[j-1];//뒤로 옮김
				j--;
			}
			a[j] = t;//삽입
		}
		
	}
	
	
	SMenu[MN_SKILLGUILD_ITEM].nField[3].nSHideNomalStart=0;
	SMenu[MN_SKILLGUILD_ITEM].nField[3].nSHideNomalCount=count;	// MAX
	if( count > 1 ) SMenu[MN_SKILLGUILD_ITEM].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;	// other button
}

int GetResultItemBySkill( int menu, int item[MAX_ITEM_BY_SKILL] )		// 그 스킬로 만들수 있는 아이템 목록 가져오기 
{
	int count = 0;
	
	memset(item, 0, sizeof( int[MAX_ITEM_BY_SKILL] ) );
	for( int i=0; i<ITEM_JOIN_MAX; i++)
	{
		if( item_join[i].item_id )
		{
			if(item_join[i].teach_place == menu )
			{
				item[count] = item_join[i].item_id;
				count ++;
			}
		}
	}
	return count;
}

bool FindLearnItem( int item )			// 내가 배운 아이템인지 판단
{
	for( int i=0; i<SCharacterData.ItemIdCount; i++ )
	{
		if( item == SCharacterData.ItemId[i] ) return true;
	}
	return false;
}

int SearchLearnItem( int item )			// 아이템 테이블에 몇 번째 있는지 판단.
{
	for( int i=0; i< SMenu[MN_SKILLGUILD_ITEM].nField[3].nSHideNomalCount; i++ )
	{
		if( item == learn_item_list[i].item_no ) return i;
	}
	return -1;		// 배울수 있는 아이템 목록에 없음
}

bool DivideItem( K_ITEM *item, int much )
{
	ItemAttr *source = ::GetItemByPos( item->item_pos );//021121 lsw
	
	if(!source ) return false;
	if( (unsigned int) much >= source->attr[IATTR_MUCH] ) return 0;
	
	SendDivideItem( item, much );
	source->attr[IATTR_MUCH]-=much;
	memset( item, 0, sizeof( K_ITEM ) );
	return 1;
}

char skill_to_mother[31][20] = {	"SPRITUALITY", "POISONING", "ENTRAPMENT", "SCROLLING", "CUNNING", "BOMBPLAY", "STEALING",
"FARMING", "MINING", "FISHING", "CUTDOWN", "BUTCHERING", "HERBDIGGING", "HERDING", "CARPENTRY",
"BLACKSMITHY", "COOKING", "TAILORING", "BOWCRAFTING", "ALCHEMING", "CANDLEMAKIN", "REPARING",
"REUTILIZING", "MUSICIANSHI", "DETECT", "HIDE", "CAMPING", "TAMING", "ACTION", "COMMUNICATION",
"IDENTIFICATION", 
};

char *GetMotherSkillName( int s )
{
	int inc = skill[s].inclusive;
	if( inc < 0 || inc > 31 ) return NULL;
	
	return skill_to_mother[inc];
}


///////////////////////////////////////////////////////////////
//
//	캐릭터 얼굴이미지 로딩및 화면 출력 함수들
//
///////////////////////////////////////////////////////////////
BYTE	*facebuf_man_b;
BYTE	*facebuf_woman_b;
BYTE	*facebuf_man_m;
BYTE	*facebuf_woman_m;
BYTE	*facebuf_man_s;
BYTE	*facebuf_woman_s;
BYTE	*facebuf_man_p;
BYTE	*facebuf_woman_p;
BYTE	*facebuf_man_mm;
BYTE	*facebuf_woman_mm;


Spr spr_face_man_b[FACE_MAX];
Spr spr_face_woman_b[FACE_MAX];
Spr spr_face_man_m[FACE_MAX];
Spr spr_face_woman_m[FACE_MAX];
Spr spr_face_man_s[FACE_MAX];
Spr spr_face_woman_s[FACE_MAX];
Spr spr_face_man_p[FACE_MAX];
Spr spr_face_woman_p[FACE_MAX];
Spr spr_face_man_mm[FACE_MAX];
Spr spr_face_woman_mm[FACE_MAX];

Spr spr_face_man_b_dash[ADD_CHA_PIC];
Spr spr_face_woman_b_dash[ADD_CHA_PIC];
Spr spr_face_man_m_dash[ADD_CHA_PIC];
Spr spr_face_woman_m_dash[ADD_CHA_PIC];
Spr spr_face_man_s_dash[ADD_CHA_PIC];
Spr spr_face_woman_s_dash[ADD_CHA_PIC];
Spr spr_face_man_p_dash[ADD_CHA_PIC];
Spr spr_face_woman_p_dash[ADD_CHA_PIC];
Spr spr_face_man_mm_dash[ADD_CHA_PIC];
Spr spr_face_woman_mm_dash[ADD_CHA_PIC];

BYTE	*facebuf_man_b_dash;
BYTE	*facebuf_woman_b_dash;
BYTE	*facebuf_man_m_dash;
BYTE	*facebuf_woman_m_dash;
BYTE	*facebuf_man_s_dash;
BYTE	*facebuf_woman_s_dash;
BYTE	*facebuf_man_p_dash;
BYTE	*facebuf_woman_p_dash;
BYTE	*facebuf_man_mm_dash;
BYTE	*facebuf_woman_mm_dash;











/*//디바인 게이지 필요 없습니다. 020515 lsw
BYTE	*divine_gauge;
Spr		spr_divine_gauge[45];
*/

BYTE	*buf_bird;
Spr		spr_bird[5];

///////////////독립 메뉴 스프라이트/////////
BYTE	*god_spr_buf;
Spr		god_spr[GOD_MENU_IMG_MAX];		// 신전 메뉴 

BYTE	*time_spr_buf;
Spr		time_spr[TIME_MENU_IMG_MAX];

BYTE	*membook_spr_buf;
Spr		membook_spr[MEMBOOK_MENU_IMG_MAX];	

BYTE	*magicbook_spr_buf;
Spr		magicbook_spr[MAGICBOOK_MENU_IMG_MAX];		

BYTE	*mail_spr_buf;
Spr		mail_spr[MAIL_MENU_IMG_MAX];		

BYTE	*start_etc_buf;
Spr		start_etc_spr[START_ETC_IMG_MAX];	

BYTE	*start_etc2_buf;
Spr		start_etc2_spr[START_ETC2_IMG_MAX];	

BYTE	*job_etc_buf;
Spr		job_etc_spr[JOB_ETC_IMG_MAX];	

BYTE	*select_server_buf;
Spr		select_server_spr[SELECT_SERVER_IMG_MAX];	

BYTE	*start_map_buf;
Spr		start_map_spr[START_MAP_IMG_MAX];	

BYTE	*chat_menu_buf;
Spr		chat_menu_spr[CHAT_MENU_IMG_MAX];

BYTE	*main_etc_buf;
Spr		main_etc_spr[MAIN_ETC_IMG_MAX];

BYTE	*main_etc3_buf;
Spr		main_etc3_spr[MAIN_ETC3_IMG_MAX];

BYTE	*skill_menu_buf;
Spr		skill_menu_spr[SKILL_MENU_IMG_MAX];	

BYTE	*small_map_buf;
Spr		small_map_spr[SMALL_MAP_MAX];

BYTE	*small_etc_buf;
Spr		small_etc_spr[SMALL_ETC_MAX];

BYTE	*public_bbs_buf;
Spr		public_bbs_spr[PUBLIC_BBS_MENU_IMG_MAX];

BYTE	*hunter_buf;
Spr		hunter_spr[HUNTER_MENU_IMG_MAX];

BYTE	*questmenu_buf;
Spr		questmenu_spr[QUEST_MENU_MAX];
// 경하가 
BYTE	*famemenu_buf;
Spr		famemenu_spr[FAME_MENU_MAX];

// 0905 kkh 추가 
BYTE	*chcolorcha_buf;
Spr		chcolorcha_spr[COLOR_CHANGE_MENU_MAX];

// 0908 kkh 추가 
BYTE	*item_make_image_buf;
Spr		item_make_image_spr[ITEM_MAKE_IMAGE_MAX];

BYTE	*nation_buf;
Spr		nation_spr[NATION_MENU_IMG_MAX];

BYTE	*start_fight_buf;
Spr		start_fight_spr[START_FIGHT_IMG_MAX];

BYTE	*stop_fight_buf;
Spr		stop_fight_spr[STOP_FIGHT_IMG_MAX];

BYTE	*fightmap_time_buf;
Spr		fightmap_time_spr[FIGHTMAP_TIME_IMG_MAX];

BYTE	*fightmap_win_buf;
Spr		fightmap_win_spr[FIGHTMAP_WIN_IMG_MAX];

BYTE	*fightmap_lose_buf;
Spr		fightmap_lose_spr[FIGHTMAP_LOSE_IMG_MAX];

BYTE	*vote_buf;
Spr		vote_spr[VOTE_MENU_IMG_MAX];

BYTE	*guild_mark_buf;
Spr		guild_mark_spr[GUILD_MARK_MAX];

BYTE	*guild_admin_buf;
Spr		guild_admin_spr[GUILD_ADMIN_MAX];

BYTE	*guild_skill_list_buf;
Spr		guild_skill_list_spr[GUILD_SKILL_LIST_MAX];

BYTE	*guild_admin_etc_buf;
Spr		guild_admin_etc_spr[GUILD_ADMIN_ETC_MAX];

BYTE	*help_pcx_buf;
Spr		help_pcx_spr[HELP_PCX_MAX];

BYTE	*bank_buf;
Spr		bank_spr[BANK_MAX];

BYTE	*salvation_buf;
Spr		salvation_spr[SALVATION_MAX];

BYTE	*salvation_ani_buf;
Spr		salvation_ani_spr[SALVATION_ANI_MAX];

BYTE	*team_battle_buf;
Spr		team_battle_spr[TEAM_BATTLE_MAX];

BYTE	*cahnge_money_buf;
Spr		cahnge_money_spr[CAHNGE_MONEY_MAX];

BYTE	*guild_wait_buf;
Spr		guild_wait_spr[GUILD_WAIT_SPR_MAX];

BYTE	*merchant_buf;
Spr		merchant_spr[MERCHANT_SPR_MAX];

BYTE	*guild_menu_buf;
Spr		guild_menu_spr[GUILD_MENU_SPR_MAX];

BYTE	*dual_img_buf;
Spr		dual_img_spr[DUAL_IMG_SPR_MAX];

BYTE	*tutorial_display_buf;
Spr		tutorial_display_spr[TUTORIAL_DISPLAY_SPR_MAX];

//011013 lsw >
BYTE	*item_make_rare_select_buf;
Spr		item_make_rare_select_spr[ITEM_MAKE_RARE_SELECT_SPR_MAX]; 
//011013 lsw <

//011014 lsw >
BYTE	*tutorial_display_tmp_buf;
Spr		tutorial_display_tmp_spr[TUTORIAL_DISPLAY_SPR_TMP_MAX];
//011014 lsw <
//< CSD-011022
BYTE	*dual_mark_buf;
Spr   dual_mark_spr[DUAL_MARK_SPR_MAX];
//> CSD-011022


BYTE	*rune_buf;
Spr		rune_spr[RUNE_SPR_MAX];

BYTE	*rune_effect_buf1;
Spr		rune_effect_spr1[RUNE_EFFECT_SPR_MAX];

BYTE	*rune_effect_buf2;
Spr		rune_effect_spr2[RUNE_EFFECT_SPR_MAX];

BYTE	*rune_effect_buf3;
Spr		rune_effect_spr3[RUNE_EFFECT_SPR_MAX];

BYTE	*rune_effect_buf4;
Spr		rune_effect_spr4[RUNE_EFFECT_SPR_MAX];

BYTE	*rune_effect_buf;
Spr		rune_effect_spr[RUNE_EFFECT_SPR_MAX];

BYTE	*skin_menu_buf;
Spr		skin_menu_spr[SKIN_MENU_MAX];

BYTE	*gamble_menu_buf;
Spr		gamble_menu_spr[GAMBLE_SPR_MAX];

BYTE	*gamble_effect_buf;
Spr		gamble_effect_spr[GAMBLE_EFFECT_MAX];

BYTE	*exp_gauge_buf;
Spr		exp_gauge_spr[EXP_GAUGE_MAX];

BYTE	*guild_member_list_buf;
Spr		guild_member_list_spr[IM_GUILD_MEMBER_LIST];

BYTE	*friend_menu_buf;
Spr		friend_menu_spr[IM_FRIEND_MENU];

BYTE	*potal_menu_buf;
Spr		potal_menu_spr[IM_POTAL_MENU];

BYTE	*party_button_buf;
Spr		party_button_spr[IM_PARTY_BUTTON];

BYTE	*guild_item_buf;
Spr		guild_item_spr[IM_GUILD_ITEM];

BYTE	*guild_change_info_buf;
Spr		guild_change_info_spr[IM_GUILD_CHANGE_INFO];




// 010730 lsw
BYTE	*nation_battle_bbs_buf;
Spr		nation_battle_bbs_spr[NATION_BATTLE_BBS_SPR_MAX];

BYTE	*nation_info_buf; //공격로
Spr		nation_info_spr[NATION_INFO_SPR_MAX];

BYTE	*nation_budae_buf;//부대
Spr		nation_budae_spr[NATION_BUDAE_SPR_MAX];

BYTE	*nation_ladder_buf;//전쟁 기념관
Spr		nation_ladder_spr[NATION_LADDER_SPR_MAX];

BYTE	*nation_vote_buf; //사령관 뽑기
Spr		nation_vote_spr[NATION_VOTE_SPR_MAX];

BYTE	*nation_join_buf; //전쟁 참여
Spr		nation_join_spr[NATION_JOIN_SPR_MAX];

BYTE	*nation_now_buf;//현황
Spr		nation_now_spr[NATION_NOW_SPR_MAX];

BYTE	*nation_guest_buf;//용병
Spr		nation_guest_spr[NATION_GUEST_SPR_MAX];

BYTE	*nation_goto_war_map_buf;//맵이동
Spr		nation_goto_war_map_spr[NATION_GOTO_WAR_MAP_SPR_MAX];

BYTE	*nation_msgbox_buf;
Spr		nation_msgbox_spr[NATION_DONEBOX_SPR_MAX];

BYTE	*nation_donebox_buf;
Spr		nation_donebox_spr[NATION_DONEBOX_SPR_MAX];

BYTE	*nation_today_notice_buf;
Spr		nation_today_notice_spr[NATION_TODAY_NOTICE_SPR_MAX];

BYTE	*nation_war_result_buf;
Spr		nation_war_result_spr[NATION_WAR_RESULT_SPR_MAX];
//010925 lsw
BYTE	*nation_war_start_buf;
Spr		nation_war_start_spr[NATION_WAR_START_SPR_MAX];

BYTE	*gm_2nd_skill_buf;
Spr		gm_2nd_skill_spr[GM_2ND_SKILL_SPR_MAX];

//010928 lsw
BYTE	*new_chat_buf;
Spr		new_chat_spr[NEW_CHAT_SPR_MAX];

BYTE	*Ability_Redistribution_Buf;							// 011201 LTS // LTS MENU
Spr		Ability_Redistribution_Spr[ABILITY_REDISTRIBUTION_SPR_MAX];

BYTE	*LocalWar_Join_Buf;										// 011201 LTS
Spr		LocalWar_Join_Spr[LOCALWAR_JOIN_SPR_MAX];

BYTE	*LocalWar_OK_Buf;										// LTS LOCALWAR BUG
Spr		LocalWar_OK_Spr[LOCALWAR_OK_SPR_MAX];

BYTE	*EventLocalWar_OK_Buf;									// 020115 LTS
Spr		EventLocalWar_OK_Spr[EVENT_LOCALWAR_OK_SPR_MAX];

BYTE	*EventLocalWar_Move_Buf;									// 020115 LTS
Spr		EventLocalWar_Move_Spr[EVENT_LOCALWAR_MOVE_SPR_MAX];

BYTE	*WarStartBuf;
Spr		WarStartSpr[WAR_START_SPR_MAX];

BYTE	*WarMoveBuf;
Spr		WarMoveSpr[WAR_MOVE_SPR_MAX];

BYTE	*WarStatusBuf;
Spr		WarStatusSpr[WAR_STATUS_SPR_MAX];

//<! BBD 040311
BYTE	*MonsterWarInfoBuf;
Spr		MonsterWarInfoSpr[MONSTERWAR_INFO_SPR_MAX];
//> BBD 040311

////////////////////////////////////////////


void PutCharImage( int x, int y, int num, int flag, int size, int flag2, int Gender  )
{
	Spr *s;
	unsigned int temp_num; 
	SCharacter *ch;
	
	if( g_StartMenuOn )	ch = &SCharSource;
	else ch = &SCharacterData;
	
	int gabX=0, gabY=0;
	
	if( Gender==100 ) Gender = ch->nCharacterData[GENDER];
	// 0907 kkh 추가 
	if( Gender ) {
		temp_num = 8;
	}
	else {
		temp_num = 7;
	}
	
	switch( size )
	{
	case FS_BIG		:
		if( num >= temp_num ){
			if( Gender ) s = &spr_face_man_b_dash[num-temp_num];
			else s = &spr_face_woman_b_dash[num-temp_num];
		}
		else{
			if( Gender ) s = &spr_face_man_b[num];
			else s = &spr_face_woman_b[num];
		}
		gabX=30; gabY=35;
		break;
		
	case FS_MIDDLE	:
		if( num >= temp_num ){
			if( Gender ) s = &spr_face_man_m_dash[num-temp_num];
			else s = &spr_face_woman_m_dash[num-temp_num];
		}
		else {
			if( Gender ) s = &spr_face_man_m[num];
			else s = &spr_face_woman_m[num];
		}
		gabX=30; gabY=35;
		break;
		
	case FS_SMALL	:
		if( num >= temp_num ){
			if( Gender ) s = &spr_face_man_s_dash[num-temp_num];
			else s = &spr_face_woman_s_dash[num-temp_num];
		}
		else{
			if( Gender ) s = &spr_face_man_s[num];
			else s = &spr_face_woman_s[num];
		}
		gabX=30; gabY=35;
		break;
		
	case FS_MINI	:
		if( num >= temp_num ){
			if( Gender ) s = &spr_face_man_p_dash[num-temp_num];
			else s = &spr_face_woman_p_dash[num-temp_num];
		}
		else{
			if( Gender ) s = &spr_face_man_p[num];
			else s = &spr_face_woman_p[num];
		}
		gabX=14; gabY=17;
		break;
	case FS_MICRO	:
		if( num >= temp_num ){
			if( Gender ) s = &spr_face_man_mm_dash[num-temp_num];
			else s = &spr_face_woman_mm_dash[num-temp_num];
		}
		else{
			if( Gender ) s = &spr_face_man_mm[num];
			else s = &spr_face_woman_mm[num];
		}
		gabX=14; gabY=17;
		break;
		
		
	}
	
	if( flag2 != 1 ) PutCompressedImage(x+gabX, y+gabY, s);
	if( flag ) PutCompressedImageFX(x+gabX, y+gabY, s, flag, flag2); 
	
}

void LoadGuildListEx(int nGuild)	// THAI2 YGI
{	//< CSD-030324
	if( !g_Menu_Variable.m_bLoadGuildMark )
	{
		g_Menu_Variable.m_bLoadGuildMark = 1;

		LoadGuildTableEx();
	#ifdef KOREA_LOCALIZING_
		//< CSD0-030325
		CGuild *curr_guild = &g_GuildMgr[nGuild];
		Spr* s = &(curr_guild->m_Spr);

		if (!s->img) 
		{
			LoadOpenningSpr( "./data/guild_mark.dat", &guild_mark_buf, guild_mark_spr);
		}
		//> CSD0-030325
	#endif
		LoadGuildMarkEx();
	}
}	//> CSD-030324

// thai2 YGI
void PutGuildImage( int x, int y, int guild_num )
{	//< CSD-030324
	if( !guild_num ) return;
	if( !g_GuildMgr[guild_num].m_bInfo ) 
	{
		SendReqGuildInfo1( guild_num );
	}

	if( g_GuildMgr[guild_num].first ) 
	{
		SetHangulFont(4);
		Hprint2( x-5, y+10, g_DestBackBuf, "GUILD" );
		SetHangulFont( 2 );
		return;
	}
	else
	{
		Spr *s;
		s = GetGuildSpr( guild_num );
		if( !s ) return;
		
		if( ( s->xl >= 20 ) && ( s->xl <= 30 ) && ( s->yl >= 25 ) && ( s->yl <= 35 ) ) 
			PutCompressedImage(x+14, y+17, s);
	}
}	//> CSD-030324

///////////////////////////////////////////////////////////////
//
//	집이미지 로딩및 화면 출력 함수들
//
///////////////////////////////////////////////////////////////
BYTE *openbuf;						// System Sprite....
BYTE *closebuf;						// System Sprite....
Spr openhouse[MAX_HOUSE];
Spr closehouse[MAX_HOUSE];
void LoadHouseImage( int is_open )
{		
	FILE *fp;
	char  temp[ FILENAME_MAX];
	short no;
	DWORD sysfilelength, size;
	DWORD nOff = 0;
	int   i;
	BYTE **buf;
	
	Spr *s;
	
	if( is_open == OPEN_HOUSE )
	{
		sprintf( temp, "./data/%s", "openhouse.dat" );
		s = openhouse;
		buf = &openbuf;
	}
	else 
	{
		sprintf( temp, "./data/%s", "closehouse.dat" );
		s = closehouse;
		buf = &closebuf;
	}
	
	fp = Fopen( temp, "rb" );
	if( fp == NULL ) return;
	
	fread( &no, 2,1, fp);
	
	fseek( fp, no * 4, SEEK_CUR );  // 오프셋 스킵
	sysfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;
	MemFree( *buf );
	MemAlloc( *buf, sysfilelength );
	
	dw_TotalImageSize += sysfilelength;
	
	for( i = 0 ; i < no ; i++)
	{	
		fread( &size, 4,1, fp);
		
		fread( &s[i].xl, sizeof( short ), 1, fp);
		fread( &s[i].yl, sizeof( short ), 1, fp);
		fread( &s[i].ox, sizeof( short ), 1, fp);
		fread( &s[i].oy, sizeof( short ), 1, fp);
		fread( &s[i].size, sizeof( unsigned int ), 1, fp);
		
		fread( *buf + nOff, s[i].size,1 ,fp);
		s[i].img = (char *)*buf + nOff;
		
		convert565to555( &s[i]);
		nOff += size;
	}	
	fclose(fp);
}		

void FreeHouseImage( )
{	
	int i=0;
	/*	for( i=0; i<MAX_HOUSE; i++ )
	{
	if( openhouse[i].img ) 
	{
	GlobalFree( openhouse[i].img );
	openhouse[i].img = NULL;
	}
	if( closehouse[i].img ) 
	{
	GlobalFree( closehouse[i].img );
	closehouse[i].img = NULL;
	}
	i++;
	}
	*/
	//	GlobalFree(openbuf);
	//	GlobalFree(closebuf);
	MemFree( openbuf );
	MemFree( closebuf );
}

void PutHouseImage( int x, int y, int num, bool is_open )
{
	Spr *s;
	if( is_open == OPEN_HOUSE ) 
	{
		if( !openbuf ) LoadHouse();
		s = &openhouse[num];
	}
	else 
	{
		if( !closebuf ) LoadHouse();
		s = &closehouse[num];
	}
	PutCompressedImage(x+69, y+56, s);
}
void LoadHouse( )
{
	if( !openbuf )	LoadHouseImage( OPEN_HOUSE );
	if( !closebuf )	LoadHouseImage( CLOSE_HOUSE );
}



//
//	클라이언트에서 파티를 맺고 그걸 서버로 전송해 준다
//	먼저 맺은 인물이 있으면 빈곳으로 옮기고 그 자리에 새로운 파티원을 넣는다.
//	서버에서도 똑같은 작업을 함.
//
int IsSameCharOfParty( int type, LpCharacterParty ch )
{
	int s;
	switch( type )
	{
	case PARTY :		for( s = 0; s<6; s++)	if( !strcmp( SCharacterData.party[s].m_Name, ch->m_Name ) )	return 1;
		break;
	case EMPLOYMENT:	for( s = 0; s<6; s++)	if( !strcmp( SCharacterData.employment[s].m_Name, ch->m_Name ) ) return 1;
		break;
	case RELATION :		for( s = 0; s<3; s++)	if( !strcmp( SCharacterData.relation[s].m_Name, ch->m_Name ) ) return 1;
		break;
	}
	return 0;
}
//< CSD-011210
int IsCheckLevelOfParty( int type, LpCharacterParty man, int pos )
{
	switch (type)
	{
	case RELATION:	
		{
			if (pos == 0)	
			{ // 스승
				return (man->m_Lv > SCharacterData.nLevel) ? 1:0;
			}
			else			
			{ // 제자 
				return (man->m_Lv < SCharacterData.nLevel) ? 1:0;
			}
			break;
		}
	case PARTY:      
		{ //< CSD-011215
			AddCurrentStatusMessage(255, 255, 255, lan->OutputMessage(5, 82), man->m_Name);
			
			if (abs(man->m_Lv - SCharacterData.nLevel) > 35)
			{
				AddCurrentStatusMessage(178, 189, 255, lan->OutputMessage(5, 30));
			}
			
			return 1;
		} //> CSD-011215
	case EMPLOYMENT: return 1;
	}
	
	return 0;
}
//> CSD-011210
int SearchNextMagicNumByClass( int num, int cls )		// 어떤 매직 다음의 같은 계열의 마법을 찾는다.
{
	for( int i = num+1; i<MAX_MAGIC_;i++ )
		if( magic[i].GetClass() == cls ) return i;
		
		//	for( int i = 0; i<=num; i++ )						// 처음으로 돌려서 다시 찾아 본다.
		//		if( magic[i].GetClass() == cls ) return i;
		
		return -1;		// 그런 클레스 계열의 마법은 없어요...
}

int GetMagicMaxByClass( int cls )		// 그계열의 총 매직 수를 구한다.
{
	int count = 0;
	for( int i = 0; i<MAX_MAGIC_; i++ )
		if( magic[i].GetClass() == cls ) count++;
		
		return count;
}

int GetLearnableMagicMaxByClass( int cls )		// 그 계열에서 내가 배울 수 있는  마법 수를 구한다. (이미 배운 마법 포함 )
{
	int count = 0;
	for( int i=0; i<MAX_MAGIC_; i++ )
	{
		if( magic[i].GetClass() == cls ) 
		{
			if( CanLearnMagic( i ) ) count++;
		}
	}
	return count ;
}

bool CanLearnMagic( int num )		// 그 마법을 배울수 있나?
{	//< CSD-030723
	//	if( GetSysInfo( SI_GAME_MAKE_MODE ) ) return true;
	int sptype=0;
	int sp;
	if( SCharacterData.nCharacterData[SPELL] )
	{
		sptype = 150;
		sp = ORISON+88;
	}
	else sp = MAGERY+88;
	
	
	
	if( SCharacterData.MagicId[num] ) return false;		// 이미 배웠다.  -> 어둡게 보여줘야 한다.
	//	if( g_lpMT[num+sptype].basic_Level > SCharacterData.nLevel )	return false;				// 레벨이 가능한가?
	//	if( g_lpMT[num+sptype].before_Magic && !SCharacterData.MagicId[ g_lpMT[num+sptype].before_Magic ] )	return false;			// 전단계 마법을 배웠는가?
	if( g_lpMT[num+sptype].basic_Magery > SCharacterData.SkillId[sp]+1 )	return false;			// 마법기술이 충분한가?
	if( g_lpMT[num+sptype].require_WP > Hero_Ability[WIS] ) return false;		// 요구 지혜
	if( g_lpMT[num+sptype].require_IT > Hero_Ability[INT_] ) return false;		// 요구 지능
	if( g_lpMT[num+sptype].exhaust_WS > Hero_Ability[WSPS] ) return false;		// 요구 WSPS
	if( g_lpMT[num+sptype].point_WS > Hero_Ability[WSPS] ) return false;	// 요구 WSPS
	if( (DWORD) g_lpMT[num+sptype].Price > SCharacterData.nMoney ) return false;
	if( g_lpMT[num+sptype].nDualStep >  QuickItemAttr[5].attr[0]) return false;	////soto-030702 듀얼 아이템으로 해야한다.
	if( g_lpMT[num+sptype].nDualStep >Hero->aStepInfo[CLS_STEP]) return false;//soto-030714 요구 듀얼스텝
	
	return true;
}	//> CSD-030723

MagicByClass	magic_by_class;
void GetDivedeMagicByClass( )
{
	int ct[13] = {0,};
	memset( &magic_by_class	, 0, sizeof( MagicByClass ) );
	int i;
	for( i=0; i<MAX_MAGIC_; i++ )
	{
		if( magic[i].GetMagicNum() && CanLearnMagic( i ) && !SCharacterData.MagicId[i] )
		{
			magic_by_class.magic[ magic[i].GetClass() ][ ct[magic[i].GetClass()] ] = magic[i].GetMagicNum();
			ct[magic[i].GetClass()]++;
		}
	}
	
	for( i=0; i<13; i++ )
	{
		magic_by_class.max[i] = ct[i];
	}
	
	for( int magic_class=0; magic_class<12; magic_class++ )//못배우는 첫번째 마법 찾기 루틴..
	{
		for( i=0; i<MAX_MAGIC_; i++ )
		{
			if( magic[i].GetClass() == magic_class )
			{
				if( magic[i].GetMagicNum() && !CanLearnMagic( i ) && !SCharacterData.MagicId[i] )
				{
					magic_by_class.magic[ magic_class ][ magic_by_class.max[magic_class] ] = i;
					//					magic_by_class.max[magic_class]++;
					break;
				}
			}
		}
	}
}

///////////////////////////////////////////
// 깜박이는 매직 아이콘 좌표하고 번호 설정
void CMagicOnCheck::SetMagicOn( int n )
{ 
	num = n; 
	x = SMenu[MN_MAININTERFACE].x+HOTKEY_ICON_POS_X +21;
	y = SMenu[MN_MAININTERFACE].y+HOTKEY_ICON_POS_Y +21; 
}

int GetTextImageWidth( char *buf )	// 문자열의 그림 이미지  총 넓이 구하기
{
	int LenghtTotal = 0;
	int k = 0;
	while( buf[k] != '\0' )
	{
		int code=buf[k];
		if(code==32) LenghtTotal+=4;	// 빈칸 처리
		else
		{
			code-=27;					// 이미지 넓이
			LenghtTotal+=spr[code].xl;
		}
		k++;
	}
	return LenghtTotal;
}

///////////////////////////////////////
DiseaseTable	disease_tbl[6];
void LoadDiseaseTbl()
{
	FILE  *fp;
	fp = Fopen( "./data/Disease_tbl.bin", "rb" );
	if( fp )
	{	
		fread( disease_tbl, 6 , sizeof( DiseaseTable ), fp );
		fclose(fp);
	}
}

void MainMenuClose( )
{
	
	for( int i=1; i<MAX_MAIN-1; i++ )		
	{
		const int iMainMenuIndex = mainmenu[i];
		if( SMenu[iMainMenuIndex].bActive ) 
		{
			int ret=0;
			for( int j=0; j<MAX_NO_CLOSE_MAIN_MENU; j++ )
			{
				if( iMainMenuIndex == aNoCloseOfMainMenu[j] ) 
				{
					ret =1;
					break;
				}
			}
			if( !ret ) 
			{
				SMenu[iMainMenuIndex].bActive = false;
				MP3( SN_MENU_CLOSE );
			}
		}
	}
	SMenu[MN_MAININTERFACE].bActive = true;
	
	nItemOther = 0;
	fRMoveMenu = 0;
	fLMoveMenu = 0;
	bSmallTips = false;
	g_Menu_Variable.clear();		// 메뉴에서 사용하는 단기성 변수 메모리 해제
	SetChatMode(CM_COMMAND,true);//021001 lsw //인터페이스 뜨면 무조건 채팅창 닫는다
	EWndMgr.ClearAllTxt();//021001 lsw
}
const int NoCloseOfMainSmallMenu[] = 
{
	MN_MAININTERFACE,
		MN_CHATTING,
		
		MN_SYSTEM_LOGOUT,
		
		MN_OK_MESSAGEBOX,
		MN_OKCANCEL_MESSAGEBOX,
		MN_ITEM_MESSAGEBOX,
		
		MN_REVIVAL,
		
		MN_NATION_WAR_RESULT,
		MN_GHOST_TO_ALIVE
};

void SmallMenuClose( )
{
	EWndMgr.ClearAllTxt();//021001 lsw
	int max = sizeof(NoCloseOfMainSmallMenu) / sizeof( int );
	for( int i=1; i<MAX_SMALL; i++ )
	{
		if( SMenu[smallmenu[i]].bActive )
		{
			int ret = 0;
			for( int j=0; j<max; j++ )
			{ 
				if( smallmenu[i] == NoCloseOfMainSmallMenu[j] ) 
				{
					ret = 1;
					break;
				}
			}
			if( !ret ) 
			{
				SMenu[smallmenu[i]].bActive = false;
				switch (smallmenu[i])
				{
				case MN_MEM_BOOK : MP3( SN_MAGIC_BOOK_CLOSE ); break;
				default : MP3( SN_MENU_CLOSE ); break;
				}
				SMenu[smallmenu[i]].CheakType = 0;
			}
		}
	}
	FreeMenuData(3); 
	FreeMenuData(6); 
	FreeMenuData(7);
	FreeHouseImage( );		// 집 그림 프리
	
	FreeMenuData2( );	// spr2 free
	FreeSmallMenuETC();	
	
	y_MenuFocus = 0;
	nItemOther = 0;
	fRMoveMenu = 0;
	fLMoveMenu = 0;

	g_bDualMagicBookOpen = false; //soto-030421 //당연히 듀얼 매직북도 닫힌다

		
	if( SMenu[MN_NATION_WAR_RESULT].bActive )
	{
		//	is_mainmenu_active = false; //020510 lsw
	}
	else
	{
		is_mainmenu_active = true; 
	}
	
	FreeQuest();
	StopWave( SN_OPEN_NATION_BBS );
	g_Menu_Variable.clear();		// 메뉴에서 사용하는 단기성 변수 메모리 해제
	SetChatMode(CM_COMMAND,true);//021001 lsw //인터페이스 뜨면 무조건 채팅창 닫는다
	EWndMgr.ClearAllTxt();//021001 lsw
}

void ServerConnectMenuClose()	// 서버와 관련이 있는 메뉴를 닫을 때..
{
	if( EXCHANGE_STATE_EXCHANGE_NOW == ExchangeMgr.GetExchangeState() )
	{
		CallServer( CMD_EXCHANGE_CANCEL );
	}
	
	if( SMenu[MN_MAN_TO_MAN].bActive )
	{
		SendManToManRefusal( SMenu[MN_MAN_TO_MAN].key );
	}
}

void SetInputParty( int type, int num, char *name )
{
	CharacterParty character;
	strcpy( character.m_Name, name );
	if( IsSameCharOfParty( type, &character ) ) return;
	SendInputParty( type, num, &character );
}

DWORD GetMoneyByItem( )
{
	DWORD money=0;
	
	int type, num;
	CItem *t;
	
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			for( int k=0; k<8; k++ )
			{
				ItemAttr &item = InvItemAttr[i][j][k];
				if( item.item_no )
				{
					GetItemNumber( item.item_no, type, num );
					t = ItemUnit( type, num );
					if( t && t->GetItemKind() == IK_MONEY )
					{
						if( item.item_no == IN_COIN && item.attr[IATTR_MUCH] > 5)	// 동전
						{
							item.item_no = IN_COINS;		// 돈 더미로 바꿈
						}
						else if( item.item_no == IN_COINS && item.attr[IATTR_MUCH] <= 5 ) // 동전 더미
						{
							item.item_no = IN_COIN;
						}
						money += item.attr[IATTR_MUCH];
						//						CheckAutoDivideMoney( &item );		// 서버에서 처리
					}
				}
			}
			
			if( HandItemAttr.item_no ) // 핸드 부분에 있는 돈 계산
			{
				ItemAttr &item = HandItemAttr;
				GetItemNumber( item.item_no, type, num );
				t = ItemUnit( type, num );
				if( t && t->GetItemKind() == IK_MONEY )
				{
					if( item.item_no == IN_COIN && item.attr[IATTR_MUCH] > 5) item.item_no = IN_COINS;	
					else if( item.item_no == IN_COINS && item.attr[IATTR_MUCH] <= 5 ) item.item_no = IN_COIN;
					money += item.attr[IATTR_MUCH];
				}
			}
			
			return money;
}

DWORD GetNewMoneyByItem( )
{
	DWORD money=0;
	
	int type, num;
	CItem *t;
	
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			for( int k=0; k<8; k++ )
			{
				ItemAttr &item = InvItemAttr[i][j][k];
				if( item.item_no )
				{
					GetItemNumber( item.item_no, type, num );
					t = ItemUnit( type, num );
					if( t && t->GetItemKind() == IK_NEW_MONEY )
					{
						if( item.item_no == IN_NEW_COIN && item.attr[IATTR_MUCH] > 5)	// 동전
						{
							item.item_no = IN_NEW_COINS;		// 돈 더미로 바꿈
						}
						else if( item.item_no == IN_NEW_COINS && item.attr[IATTR_MUCH] <= 5 ) // 동전 더미
						{
							item.item_no = IN_NEW_COIN;
						}
						money += item.attr[IATTR_MUCH];
					}
				}
			}
			
			if( HandItemAttr.item_no ) // 핸드 부분에 있는 돈 계산
			{
				ItemAttr &item = HandItemAttr;
				GetItemNumber( item.item_no, type, num );
				t = ItemUnit( type, num );
				if( t && t->GetItemKind() == IK_NEW_MONEY )
				{
					if( item.item_no == IN_NEW_COIN && item.attr[IATTR_MUCH] > 5) item.item_no = IN_NEW_COINS;	
					else if( item.item_no == IN_NEW_COINS && item.attr[IATTR_MUCH] <= 5 ) item.item_no = IN_NEW_COIN;
					money += item.attr[IATTR_MUCH];
				}
			}
			
			return money;
}

int GetItemMuchByMuchItem( int item_no )		// 수량이 있는 아이템의 갯수 파악하기
{
	int count = 0;
	ItemAttr *inv = (ItemAttr *)InvItemAttr;
	for( int i=0; i<72; i++ )
	{
		if( inv[i].item_no == item_no ) count += inv[i].attr[IATTR_MUCH];
	}
	return count;
}

int SearchItemPosByNumber(ItemAttr *item, int &a, int &b, int &c)
{
	for(a=0;a<3;a++)
		for(b=0;b<3;b++)
			for(c=0;c<8;c++)
				if( InvItemAttr[a][b][c].item_no == item->item_no 
					&& InvItemAttr[a][b][c].attr[IATTR_RARE_MAIN] == item->attr[IATTR_RARE_MAIN] )
					return 1;
				return 0;
}

int SearchCoinItem( int &a, int &b, int &c)
{
	for(a=0;a<3;a++)
		for(b=0;b<3;b++)
			for(c=0;c<8;c++)
				if( InvItemAttr[a][b][c].item_no == IN_COIN || InvItemAttr[a][b][c].item_no == IN_COINS )
				{
					if( InvItemAttr[a][b][c].attr[IATTR_MUCH] == MAX_MONEY ) continue;
					return 1;
				}
				return 0;
}

int SearchNewCoinItem( int &a, int &b, int &c)
{
	for(a=0;a<3;a++)
		for(b=0;b<3;b++)
			for(c=0;c<8;c++)
				if( InvItemAttr[a][b][c].item_no == IN_NEW_COIN || InvItemAttr[a][b][c].item_no == IN_NEW_COINS )
				{
					if( InvItemAttr[a][b][c].attr[IATTR_MUCH] == MAX_MONEY ) continue;
					return 1;
				}
				return 0;
}

ItemAttr *SearchItemOfInvByKind( int kind, int &i, int &j, int &k )		// 돈일 경우로만 개선
{
	int type, num;
	CItem * t;
	for( i=0; i<3; i++ )
		for( j=0; j<3; j++ )
			for( k=0; k<8; k++ )
			{
				ItemAttr &item = InvItemAttr[i][j][k];
				if( item.item_no )
				{
					GetItemNumber( item.item_no, type, num );
					t = ItemUnit( type, num );
					if( t && t->GetItemKind() == kind ) 
					{
						return &item;
					}
				}
			}
			return NULL;
}

ItemAttr *SearchItemOfInvByKind( int kind )		// 돈일 경우로만 개선
{
	int a, b, c;
	return SearchItemOfInvByKind( kind, a, b, c );
}

int SubtractMoney( DWORD money )
{
	if(SCharacterData.nMoney < money ) return -1;	// 돈이 부족함
	
	if( !money ) return 1;
	return 1;		// 서버에서 처리함
}

int AddMoney( DWORD money )
{
	return 1;		// 서버에서 전송
}

void SetMemorizeArray( int num )
{
	memset( memorize, 0, sizeof( int[12] ) );
	int first = SCharacterData.nCharacterData[SPELL];
	int ct = 0;
	for( int i=0; i<12; i++ )
	{
		int magic = n_MagicBagic[first][num][i];
		if( ::magic[magic].GetMagicNum() && SCharacterData.MagicId[magic] ) memorize[ct++] = ::magic[magic].GetMagicNum();		// 배열 번호와 magicnum번호는 같지만 없는 마법때문에..
	}
}

int DoMemorize( int magic_num )
{
	int sp_type = 0;
	if( SCharacterData.nCharacterData[SPELL] ) sp_type = 150;
	
	if( SCharacterData.memorize_point < g_lpMT[magic_num+sp_type].exhaust_WS ) return 0;
	SCharacterData.memorize_point -= g_lpMT[magic_num+sp_type].exhaust_WS;
	
	InsertMagicQuickArray( magic_num );			// 새로 추가된 퀵 매직 배열에 셋팅
	return 1;		// 매직창 개념 바꿈
	
	SCharacterData.curr_magic[SCharacterData.curr_magic_ct++] = magic_num;
	magic[magic_num].SetArray();		// 매직창에 셋팅 
	return 1;
}

void InitMemorize( )	// 메모라이즈 초기화
{
	memset( SCharacterData.curr_magic, 0, sizeof( int[MEMORIZE_MAX] ) );		// 메모라이즈한 마법 초기화..
	ResetMagicQuickArray( );													// 그동안 사용하고 기억한 마법도 초기화
	
	SCharacterData.curr_magic_ct = 0;
	
	SCharacterData.memorize_point_max = SCharacterData.nCharacterAbility[WSPS] + 3;
	SCharacterData.memorize_point = SCharacterData.memorize_point_max;
}

int MemorizeYet( int magic_num )
{
	return IsExistNodeToArray( magic_num, magic_plus,  magic_plus_count );
}


int GetNoLearnMagic( int cls, int &magic_num )	// 못배우는 마법 이름과 그 이유를 알아오는 함수	// magic_num으로 마법 번호를 넘겨 준다.
{	//< CSD-030723
	cls--;
	int sptype=0;
	int sp;
	int spell = SCharacterData.nCharacterData[SPELL];
	if( spell )
	{
		if( cls > 11 ) return 0;
		sptype = 150;
		sp = ORISON+88;
	}
	else sp = MAGERY+88;
	
	for( int i = 0; i< 12; i++ )
	{
		magic_num = n_MagicBagic[spell][cls][i];
		if( !magic[magic_num].GetMagicNum() || SCharacterData.MagicId[magic_num] ) continue;
		//		if( g_lpMT[magic_num+sptype].basic_Level > SCharacterData.nLevel )	return 1;				// 레벨이 가능한가?
		//		if( g_lpMT[magic_num+sptype].before_Magic && !SCharacterData.MagicId[ g_lpMT[magic_num+sptype].before_Magic ] )	return 2;			// 전단계 마법을 배웠는가?
		if( g_lpMT[magic_num+sptype].require_WP > SCharacterData.nCharacterAbility[WIS] ) return 4;		// 요구 지혜
		if( g_lpMT[magic_num+sptype].require_IT > SCharacterData.nCharacterAbility[INT_] ) return 5;		// 요구 지능
		if( g_lpMT[magic_num+sptype].basic_Magery > SCharacterData.SkillId[sp]+1 )	return 3;				// 마법기술이 충분한가?
		if( g_lpMT[magic_num+sptype].exhaust_WS > SCharacterData.nCharacterAbility[WSPS] ) return 6;		// 요구 WSPS
		if( g_lpMT[magic_num+sptype].point_WS > SCharacterData.nCharacterAbility[WSPS] ) return 8;		// 요구 WSPS
		if( (DWORD)g_lpMT[magic_num+sptype].Price > SCharacterData.nMoney ) return 7;		// 요구 money
		
		if (g_lpMT[magic_num + sptype].nDualStep > Hero->aStepInfo[CLS_STEP]) return 10; // soto-030714
		if (g_lpMT[magic_num + sptype].nDualStep > QuickItemAttr[5].attr[0]) return 9;  // CSD-TW-030606
	}
	
	return 1000;
}	//> CSD-030723

void CheckPartyFace( )		// 파티원들이 있는 지를 체크해서 화면에 뿌려준다.
{
	static int NotMenu[] = 
	{MN_STATUS,MN_ABILITY,MN_ARIGEMENT,MN_TACTICS, 
	MN_SYSTEM_OPTION,  MN_BANK_CUSTODY, MN_SMALL_MAP, };
	static int party_face_menu[] = { MN_PARTY_FACE1, MN_PARTY_FACE2,MN_PARTY_FACE3, MN_PARTY_FACE4, MN_PARTY_FACE5, MN_PARTY_FACE6 };
	const int max = sizeof(NotMenu)/sizeof(int); 
	
	//	if( main_interface.data[IF_BATTLE_PEACE] )	// 배틀 모드인 경우
	//	{
	//		for( int a=0; a<6; a++ ) SMenu[ party_face_menu[a] ].bActive = false;
	//		return;
	//	}
	int i;
	for( i=0; i<max; i++ )
	{
		if( SMenu[NotMenu[i]].bActive )
		{
			for( int a=0; a<6; a++ ) SMenu[ party_face_menu[a] ].bActive = false;
			return;
		}
	}
	
	int ct = 0;
	for( i=0; i<6; i++ )
	{
		SMenu[ party_face_menu[i] ].bActive = false; //true;		// 먼저 폴스 시킨후...
		if( *SCharacterData.party[i].m_Name && SCharacterData.party[i].m_JoinOk )
		{
			SMenu[ party_face_menu[ct] ].bActive = true;
			SMenu[ party_face_menu[ct] ].nField[0].nWillDo = i;		// 배열 몇번째를 출력 시키는 가..
			
			//			if( !SearchMyPartyOfMySight( &SCharacterData.party[i] ) )
			//			{
			//				SCharacterData.party[i].m_Sight = 0;
			//			}
			LPCHARACTER ch = SCharacterData.party[i].ch;
			if( ch )
			{
				if( ch->viewtype == VIEWTYPE_GHOST_ )
				{
					SCharacterData.party[i].m_Condition |= CON_DEATH;
					SCharacterData.party[i].m_Hp	=	0;
					SCharacterData.party[i].m_HpMax	=	0;
				}
				else 
				{
					SCharacterData.party[i].m_Condition = ch->condition;
					SCharacterData.party[i].m_Hp	=	ch->_Hp;
					SCharacterData.party[i].m_HpMax	=	ch->_HpMax;
				}
			}
			ct++;
		}
	}
	
	return;
}

void SetExistMeOfParty( )				// 0620 YGI
{
	for( int i=0; i<6; i++ )
	{
		if( !strcmp(SCharacterData.party[i].m_Name , SCharacterData.sCharacterName) ) 
		{
			SCharacterData.party[i].m_Id		= Hero->id;;
			SCharacterData.party[i].m_Lv		= SCharacterData.nLevel;
			SCharacterData.party[i].m_Str		= SCharacterData.nCharacterAbility[STR];
			SCharacterData.party[i].m_Join		= 1;		
			SCharacterData.party[i].m_JoinOk	= 1;	
			SCharacterData.party[i].m_Sight		= 1;	
			break;
		}
	}
}

int SetPartyOnMySight( LPCHARACTER ch, LpCharacterParty party )
{
	party->m_Sight = 1;
	party->m_Join = 1;			// 시야에 있으면 접속 상태 이다.
	
	party->m_Condition=	ch->condition;
	party->m_Hp=		ch->_Hp;
	party->m_HpMax=		ch->_HpMax;
	party->ch = ch;
	
	return 1;
}

int RealesePartyOnMySight( LpCharacterParty party )
{
	party->m_Sight = 0;
	party->ch = NULL;
	
	return 1;
}

int CheckAndSetPartyOnMySight( LPCHARACTER ch, int type )		// 파티원 시야에 들어왔는지 나간지를 판단.		1: 들어올때, 0: 나갈때
{	
	
	if( ch->type == SPRITETYPE_MONSTER ) return 0;
	
	if( !ch->name_status.counselor )
	{
		if( my_hunter_list )
		{
			int ret = my_hunter_list->CheckList( ch->name, type );	// 들어오고 나갈때의 현상범 리스트 처리
			if( ret )
			{
				SendDangerOfHunter( ch->id, type );					// 그사람에게 위험을 알린다.
			}
		}
	}
	
	if( type )
	{
		if( my_hunter_list->ExistMyHunterList( ch->name ) )		// 내 목록에 있는 넘이다.
			ch->hunter_target = true;
	}
	else
	{
		DeleteMouseCh( ch->name );			// 손에 들고 있는 사람이 시야에서 사라졌을 경우 
	}
	
	if (Hero != NULL)
	{	//< CSD-030521
		CArenaTeam* pTeam = g_pArenaManager->GetMemberTeam(ch->id);

		if (pTeam != NULL)
		{
			if( type )
			{
				const int nTeam = pTeam->GetTeamNumber();
				ch->fight_map_team_num = nTeam + 1;

				if (g_FightMapStart)
				{
					ch->accessory[AT_ARMOR] = fight_map_acc_num[nTeam];
					ch->accessory[AT_HELMET] = 1;
				}
			}
		}
	}	//> CSD-030521
	
	for( int i=0; i<6; i++ )
	{
		if( *SCharacterData.party[i].m_Name && SCharacterData.party[i].m_JoinOk )
		{
			if( !strcmp(SCharacterData.party[i].m_Name, ch->name) )
			{
				if( type )
					SetPartyOnMySight( ch, &SCharacterData.party[i] );
				else RealesePartyOnMySight( &SCharacterData.party[i] );
			}
		}
	}
	
	return 1;
}

int SearchMyPartyOfMySight( LpCharacterParty party )	// 내 시야에 파티원이 있느냐?		// 매번 체크 이기때문에 바궜음.. 지금은 안쓰임
{
	LPCHARACTER ch = Hero;
	
	while( ch != NULL )
	{
		if( !strncmp( ch->name , party->m_Name, MAX_NAME ) )
		{
			SetPartyOnMySight( ch, party );
			return 1;		// 내 시야에 들어 왔다.
		}
		ch = ch->lpNext;
	}
	return 0;	// 없다.
}

int GetIndexOfCondition( BYTE condition )
{ //< CSD-Monster
	if (condition&CON_DEATH)	 return CC_DEATH;
	if (condition&CON_STONE)	 return CC_STONE;
	if (condition&CON_FROZEN)  return CC_FROZEN;
	if (condition&CON_DAZE)		 return CC_DAZE;
	if (condition&CON_CURSE)	 return CC_CURSE;
	if (condition&CON_POISON)	 return CC_POISON;
	if (condition&CON_SLOW)		 return CC_SLOW;
	return CC_NORMAL;
} //> CSD-Monster

void ViewSmallMapCheck()
{
	if( main_interface.data[IF_MAP] )
	{
		SMenu[MN_SMALL_MAP].bActive = true;
		if( isCrashSmallMapByOther() )
		{
			SMenu[MN_SMALL_MAP].bActive = false;	// 충돌
			SMenu[MN_SMALL_MAP].nTemp = 0;
		}
	}
	else 
	{
		SMenu[MN_SMALL_MAP].bActive = false;
		SMenu[MN_SMALL_MAP].nTemp = 0;
	}
}

int isCrashSmallMapByOther()		// 메인 메뉴와 충돌이 났는가?
{
	int img = SMenu[MN_SMALL_MAP].nImageNumber;
	RECT box2;
	SetRect( box2, SMenu[MN_SMALL_MAP].x,SMenu[MN_SMALL_MAP].y, SMenu[MN_SMALL_MAP].x+spr[img].xl, SMenu[MN_SMALL_MAP].y+spr[img].yl );
	
	if( is_mainmenu_active )
	{
		for(int t=1;t<MAX_MAIN;t++)
		{
			int menu=mainmenu[t];
			if( menu==MN_SMALL_MAP || menu==MN_LEVELUP || menu==MN_PARTY_FACE1 || menu==MN_PARTY_FACE2 || menu==MN_PARTY_FACE3 
				|| menu==MN_PARTY_FACE4 || menu==MN_PARTY_FACE5 || menu==MN_PARTY_FACE6 ) continue;
			if(SMenu[menu].bActive)
			{
				RECT box;
				SetRect(box, SMenu[menu].x, SMenu[menu].y, SMenu[menu].x+spr[SMenu[menu].nImageNumber].xl, SMenu[menu].y+spr[SMenu[menu].nImageNumber].yl);
				int ret;
				if( ret = RectAndRectCrash( &box, &box2 ) )
				{
					if( ret != 100 ) 
					{
						if( ret & 0x00000001 ) 
						{
							SMenu[MN_SMALL_MAP].x = box.left-spr[img].xl;
							if( SMenu[MN_SMALL_MAP].x < 0 ) SMenu[MN_SMALL_MAP].x = 0;
							else if( SMenu[MN_SMALL_MAP].x > SCREEN_WIDTH-spr[img].xl ) SMenu[MN_SMALL_MAP].x = SCREEN_WIDTH-spr[img].xl;
						}
						else if( ret & 0x00000010 ) 
						{
							SMenu[MN_SMALL_MAP].x = box.right;
							if( SMenu[MN_SMALL_MAP].x < 0 ) SMenu[MN_SMALL_MAP].x = 0;
							else if( SMenu[MN_SMALL_MAP].x > SCREEN_WIDTH-spr[img].xl ) SMenu[MN_SMALL_MAP].x = SCREEN_WIDTH-spr[img].xl;
						}
					}
					return ret;
				}
			}
		}
	}
	return 0;
}

int RectAndRectCrash( const RECT *box1, const RECT *box2 )
{
	int ret = 0;
	if( box1->right >= box2->left && box2->right >= box1->left && box1->bottom >= box2->top && box2->bottom >= box1->top ) 
	{
		if( box2->left	< box1->left	) ret |= 0x00000001 ;		// 왼쪽에서 충돌
		if( box2->right > box1->right	) ret |= 0x00000010 ;		// 오른쪽에서 충돌
		if( box2->top	> box1->top		) ret |= 0x00000100 ;		// 위에서 충돌
		if( box2->bottom< box1->bottom	) ret |= 0x00001000 ;		// 아래에서 충돌
		if( box2->left > box1->left && box2->right < box1->right ) ret = 100;
	}
	return ret;
}

int RectAndDotCrash( const RECT *box1, const int x, const int y )
{
	if( x > box1->left && x < box1->right  && y > box1->top  && y < box1->bottom )
	{
		return TRUE;
	}
	return FALSE;
}

inline int CheckMouseInMenu( int i )
{
	Spr *s;
	int type = SMenu[i].nImageType;
	int img = SMenu[i].nImageNumber;
	switch( type )
	{
	case MEM_BOOK : {
		RECT box;
		s = &membook_spr[img]; 
		SetRect( box, SMenu[i].x+35, SMenu[i].y+26, SMenu[i].x+s->xl, SMenu[i].y+s->yl );	
		return MouseInRectCheak( 0, 0, box );
					}
		
	default :	s = GetSprOfMenu( type, img );
		if( !s ) return 0;
		break;
	}
	
	RECT box;
	SetRect( box, SMenu[i].x, SMenu[i].y, SMenu[i].x+s->xl, SMenu[i].y+s->yl );
	
	return MouseInRectCheak( 0,0,box );
}

void LoadSpriteOnlyOne( Spr *s, char filename[MAX_FILENAME] )		// 스프라이트 화일 하나만 읽어 올때
{
	FILE	*fp;
	if( fp = Fopen(filename, "rb" ) )
	{
		fread( &s->xl, sizeof( short ), 1, fp);
		fread( &s->yl, sizeof( short ), 1, fp);
		fread( &s->ox, sizeof( short ), 1, fp);
		fread( &s->oy, sizeof( short ), 1, fp);
		fread( &s->size, sizeof( unsigned int ), 1, fp);
		
		MemFree( s->img );
		MemAlloc( s->img, s->size );
		fread(s->img, s->size, 1, fp);
		
		dw_TotalImageSize += s->size;
		
		convert565to555( s );
		fclose(fp);
	}
}

void FreeSpriteOnlyOne( Spr *s )
{
	MemFree( s->img );
}

void PutStartLodingImg( )//020515 lsw
{
	Spr t;
	memset( &t, 0, sizeof( Spr ) );
	Spr *s = &t;
	char filename[MAX_FILENAME];
	sprintf(filename, "./data/Loading.dat");
	LoadSpriteOnlyOne( s, filename );
	if( s->img ) 
	{
		PutCompressedImage( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, s );
	}
	FreeSpriteOnlyOne( s );
}

int dexDefenseCor( int dex )
{
	if(dex < 10 ) return 1 ;
	if(dex == 10 || dex == 11) return 2 ;
	if(dex >= 12 && dex <= 14) return 3 ;
	if(dex == 15) return 4 ; 
	if(dex == 16) return 5 ;
	if(dex >= 18 && dex <= 20) return 6 ;
	if(dex >= 21 && dex <= 24) return 7 ;
	if(dex >= 25 && dex <= 29) return 9 ;
	
	return 10 ;
}

int IsNoCheckFieldByMain( int menu, int field )
{
	if( menu != MN_MAININTERFACE ) return 0;
	
	int main_field[] = {11,12,17,18,19,20,21,23,24,25,26, 27, 33, 36 };		// 메인 인터페이스에서 체크하지 않아야 하는 필드들
	int field_max = sizeof( main_field ) / sizeof( int );
	
	for( int i=0; i<field_max; i++ )
		if( field == main_field[i] ) return 1;
		
		return 0;
}

int IsJustViewMenuWhenDead( int dest_menu )
{
	int menu[] = {	
		MN_QUICK_MAGIC_PLUS, 
			MN_QUICK_MAGIC_SCROLL,
			MN_TIME_DISPLAY_ON_OFF,
	};
	int max = sizeof( menu ) / sizeof( int );
	
	for( int i=0; i<max; i++ )
	{
		if( dest_menu == menu[i] ) return true;
	}
	
	return false;
}

void MenucheckWhenHeroDie( )	// 죽었을때 메뉴체킹
{
	if( fLMoveMenu || fRMoveMenu ) return;
	
	int i;
	for( i=1; i<MAX_MAIN; i++ )
	{
		if( SMenu[mainmenu[i]].bActive )//메인메뉴가살아 있다면
		{
			int j;
			for( j=0; j<MAX_NO_CLOSE_MENU; j++ )
			{
				if( mainmenu[i] == aNoCloseMenu[j] ) break;//꺼지면 안되는 메뉴
			}
			if( j==MAX_NO_CLOSE_MENU )
			{
				SMenu[mainmenu[i]].bActive = false;
			}
		}
	}
	
	if( g_BattleStatusView ) 
	{	// 강제 제외시켜 준다.
		CallServer( CMD_FIGHT_MEMBER_TO_EXIT );
		g_BattleStatusView = 0;
		SmallMenuClose();
	}
	
	int x, j;
	for(x=0;x<MAX_NO_CLOSE_MENU;x++)
	{
		i= aNoCloseMenu[x];
		if( !MenuFocus(i) ) 
		{
			if(SMenu[i].bActive)   
			{
				if( CheckMouseInMenu( i ) ) g_MouseInMenu = i;
				for(j=0;j<SMenu[i].nFieldCount;j++)
				{
					if( IsNoCheckFieldByMain( i, j ) ) continue;
					if(MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
					{
						SMenu[i].nField[j].fRectMouse=TRUE;
						SMenu[i].nField[j].RectCount++;
						g_MouseInMenu = i;			// mouse가 menu안에 들어왔다.
					}
					else	
					{
						SMenu[i].nField[j].fRectMouse=false; // 구성 요소 안에 있으면 TRUE, 아니면 FALSE
						SMenu[i].nField[j].fLButtonDown=false;
						SMenu[i].nField[j].fCheakFlag=false;
						SMenu[i].nField[j].RectCount = 0;
					}
				}
			}
			else
			{
				SMenu[i].CheakType = 0;		// 초기화를 위해	그 메뉴가 처음으로 액티브 될때 1로 셋팅하면서 초기화 시켜준다.
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
	
	if(g_nLButtonState==STATE_BUTTON_PRESSED)		//   왼쪽 마우스 버튼이 눌렸었는지를 체크
	{
		LButtonDown=TRUE;
		StateMousePoint = g_pointMouse;
	}
	
	if(g_nLButtonState==STATE_BUTTON_STILL && LButtonDown)   // 일단 마우스가 어디선가 방금 눌렸다. 그게 범위 안에서 눌렸는지, 아닌지 판단 부분
	{
		LButtonCount++;
		MouseDrag = TRUE;
		for(int x=0;x<MAX_NO_CLOSE_MENU;x++)
		{
			int i=aNoCloseMenu[x];
			if(!MenuFocus(i)) 
			{
				if(SMenu[i].bActive)
				{
					if( IsJustViewMenuWhenDead( i ) ) continue;
					for(int j=0; j<SMenu[i].nFieldCount;j++) 
					{
						if( IsNoCheckFieldByMain( i, j ) ) continue;
						if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) && 
							MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox, FALSE))  //마우스가 메뉴구성 요소의 rect안에서 L버튼이 눌렸냐 //FALSE 일 경우는 드래그 시작점으로 판단
						{
							SMenu[i].nField[j].fLButtonDown=TRUE;
							DoLButtonDownOfMenu( i, j );					//  타입에 따른 실행
							SMenu[i].nField[j].DragCount ++;
						}
						else if(MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
						{
							if(i == MN_FACE) 
							{
								SMenu[i].nField[j].fLButtonDown=TRUE;
								DoLButtonDownOfMenu( i, j );
							}
						}
						else SMenu[i].nField[j].DragCount = 0;
					}
				}
			}
		}
	}
	else 
	{
		LButtonCount = 0;
		if(g_nLButtonState==STATE_BUTTON_RELEASED) //마우스가 방금 떨어진 상태냐?
		{
			YouGotItemFlag = 0;	
			HandItemOn = false;
			int kkk = ItemDrop();
			LButtonDown=FALSE;
			
			for(x=0;x<MAX_NO_CLOSE_MENU;x++)
			{
				int f_ReturnOn = false;
				i=aNoCloseMenu[x];
				if( MenuFocus(i) )  continue;   //원하는 메뉴 하나에만 마우스 포커스를 둔다
				if(! SMenu[i].bActive) continue;
				for(j=0; j<SMenu[i].nFieldCount;j++)
				{
					if( IsNoCheckFieldByMain( i, j ) ) continue;
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
					else if( g_MouseItemType && CanMoveItemToMenu(SMenu[i].nField[j].nType))		//FT_INVENTORY는 떨어지기만 해도 괜찮다
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
	PutMouseItemCheck();  // 마우스 드래그 중 떨어 뜨릴때
}

void MainMenuFree()
{
	FreeItemIcon();			// 아이콘 이미지 프리
	FreeHouseImage( );		// 집 이미지 프리
	
	FreeMenuData( 1 );
	FreeMenuData( 2 );
	FreeMenuData( 5 );
	
	FreeMagicIcon( );
	
	FreeOpenningSpr( &facebuf_man_b );
	FreeOpenningSpr( &facebuf_woman_b );
	FreeOpenningSpr( &facebuf_man_s );
	FreeOpenningSpr( &facebuf_woman_s );
	FreeOpenningSpr( &facebuf_woman_b );
	FreeOpenningSpr( &facebuf_man_p );
	FreeOpenningSpr( &facebuf_woman_p );
	FreeOpenningSpr( &facebuf_man_mm );
	FreeOpenningSpr( &facebuf_woman_mm );
	
	int i;
	for( i = 0 ; i< FACE_MAX; i++ ) 
	{
		spr_face_man_b[i].img	= NULL;
		spr_face_woman_b[i].img	= NULL;
		spr_face_man_m[i].img	= NULL;
		spr_face_woman_m[i].img = NULL;
		spr_face_man_s[i].img	= NULL;
		spr_face_woman_s[i].img = NULL;
		spr_face_man_p[i].img	= NULL;
		spr_face_woman_p[i].img = NULL;
		spr_face_man_mm[i].img	= NULL;
		spr_face_woman_mm[i].img= NULL;
	}
	
	FreeOpenningSpr( &facebuf_man_b_dash );
	FreeOpenningSpr( &facebuf_woman_b_dash );
	FreeOpenningSpr( &facebuf_man_s_dash);
	FreeOpenningSpr( &facebuf_woman_s_dash);
	FreeOpenningSpr( &facebuf_man_p_dash);
	FreeOpenningSpr( &facebuf_woman_p_dash);
	FreeOpenningSpr( &facebuf_man_mm_dash);
	FreeOpenningSpr( &facebuf_woman_mm_dash);
	
	for( i=0; i<ADD_CHA_PIC; i++ )
	{
		spr_face_man_b_dash[i].img		= NULL;
		spr_face_woman_b_dash[i].img	= NULL;
		spr_face_man_m_dash[i].img		= NULL;
		spr_face_woman_m_dash[i].img	= NULL;
		spr_face_man_s_dash[i].img		= NULL;
		spr_face_woman_s_dash[i].img	= NULL;
		spr_face_man_p_dash[i].img		= NULL;
		spr_face_woman_p_dash[i].img	= NULL;
		spr_face_man_mm_dash[i].img		= NULL;
		spr_face_woman_mm_dash[i].img	= NULL;
	}
	
	//	FreeOpenningSpr( &divine_gauge );	for( i=0; i<45; i++ ) spr_divine_gauge[i].img = NULL;// 020515 lsw 디바인 게이지 로딩 하지도 않습니다.
	FreeOpenningSpr( &buf_alphabet );	for( i=0; i<36; i++ ) spr_alphabet[i].img = NULL;
	FreeOpenningSpr( &main_etc_buf );	for( i=0; i<MAIN_ETC_IMG_MAX; i++ ) main_etc_spr[i].img = NULL;
	FreeOpenningSpr( &main_etc3_buf );	for( i=0; i<MAIN_ETC3_IMG_MAX; i++ ) main_etc3_spr[i].img = NULL;
	FreeOpenningSpr( &start_fight_buf); for( i=0; i<START_FIGHT_IMG_MAX; i++ ) start_fight_spr[i].img = NULL;
	FreeOpenningSpr( &stop_fight_buf);	for( i=0; i<STOP_FIGHT_IMG_MAX; i++ ) stop_fight_spr[i].img = NULL;
	FreeOpenningSpr( &guild_mark_buf);	for( i=0; i<GUILD_MARK_MAX; i++ ) guild_mark_spr[i].img = NULL;
	FreeOpenningSpr( &help_pcx_buf );	for( i=0; i<HELP_PCX_MAX; i++ ) help_pcx_spr[i].img = NULL;
	FreeOpenningSpr( &guild_wait_buf );	for( i=0; i<GUILD_WAIT_SPR_MAX; i++ ) guild_wait_spr[i].img = NULL;
	FreeOpenningSpr( &exp_gauge_buf );	for( i=0; i<EXP_GAUGE_MAX; i++ ) exp_gauge_spr[i].img = NULL;
	//< CSD-011022
	FreeOpenningSpr(&dual_mark_buf);	for( i=0; i<DUAL_MARK_SPR_MAX; i++ ) dual_mark_spr[i].img = NULL;
	//> CSD-011022
	
	
	if( chat_menu_spr[0].img )
	{
		FreeOpenningSpr( &chat_menu_buf );
		for( int i = 0 ; i< CHAT_MENU_IMG_MAX; i++ ) chat_menu_spr[i].img = NULL;
	}
	
	FreeSmallMenuImage2( );
	FreeSmallMenuETC();			// 기능별로 묶어둔 spr 데이타 프리
	
	// 메뉴 담아놓은 스택도 비워줘야 한다.
	// 각각의 리스트들을 모두 지운다.
	if( house ) 
	{
		delete house; 
		house = NULL;
	}
	
	if( house_sub_residing ) 
	{
		delete house_sub_residing;
		house_sub_residing = NULL;
	}
	if( house_sub_guild ) 
	{
		delete house_sub_guild;
		house_sub_guild = NULL;
	}
	if( house_sub_not_res ) 
	{
		delete house_sub_not_res;
		house_sub_not_res = NULL;
	}
	
	if( hunter_list )
	{
		delete hunter_list;
		hunter_list = NULL;
	}
	if( my_hunter_list )
	{
		delete my_hunter_list;
		my_hunter_list = NULL;
	}
}

void ReStartMenu( )
{
	FighMapReset();
	
	GameEndFree();
	//	MainMenuFree();
	
	QuitFlag = 0;
	SMenu[MN_MAININTERFACE].bActive=false;
	y_MenuFocus = 0;
	g_StartMenuOn = true;
	
	AllReqSkillExp( 1 );
	
	recv_learn_item = 0;		// 배운 아이템을 서버에서 전송받았는지 플레그
	danger_by_hunter = 0;
	
	g_BGE_Flag = 0;
	g_CurrVote = 0;
	StopWave( SN_X_MAS_BGM );
	
	EquipSaveFlag[0] = 0;
	EquipSaveFlag[1] = 0;
	
	ClearQuickMemory();
	
	QestDataFree( );
	
	g_Menu_Variable.clear();
	g_Menu_Variable.clear2();
	FreeAllOfMenu();
	DeleteAllSkillData();
	InitItemAttrs();
	ChatMgr.Clear();//020704 lsw//했던 대화 삭제
	CurrentMsgMgr.Clear();//020704 lsw//커런트 메세지 삭제
	HotKeyMemory.Clear();
}

void PutDirectBoardMenu( const DIRECTBOARDDATA *lpDB )
{
	int ct= 0 ;
	if( lpDB->str_East[0] )
	{
		SMenu[MN_DISTANCE_TABLE].nField[ct].nWillDo=1;
		sprintf( SMenu[MN_DISTANCE_TABLE].nField[ct].temp, "%s", lpDB->str_East );
		ct++;
	}
	if( lpDB->str_South[0] )
	{
		SMenu[MN_DISTANCE_TABLE].nField[ct].nWillDo=2;
		sprintf( SMenu[MN_DISTANCE_TABLE].nField[ct].temp, "%s", lpDB->str_South );
		ct++;
	}
	if( lpDB->str_West[0] )
	{
		SMenu[MN_DISTANCE_TABLE].nField[ct].nWillDo=3;
		sprintf( SMenu[MN_DISTANCE_TABLE].nField[ct].temp, "%s", lpDB->str_West );
		ct++;
	}
	if( lpDB->str_North[0] )
	{
		SMenu[MN_DISTANCE_TABLE].nField[ct].nWillDo=4;
		sprintf( SMenu[MN_DISTANCE_TABLE].nField[ct].temp, "%s", lpDB->str_North );
		ct++;
	}
	
	SMenu[MN_DISTANCE_TABLE].bActive = true;
	SMenu[MN_DISTANCE_TABLE].work = 150;
	SMenu[MN_DISTANCE_TABLE].nFieldCount=ct;
}

void PutBoardMenu( int cn, char *name )
{
	SMenu[MN_STORE_SIGN].bActive = true;
	SMenu[MN_STORE_SIGN].work = 150;
	sprintf( SMenu[MN_STORE_SIGN].nField[0].temp, "%s", name );
}


int IsMouseOnMenu()
{
	return g_MouseInMenu;
}

////////////////////////////////////////////////////////////////
//
// 문자열중 특정 문자열에 포함한 문자가 있는지를 찾는 메소드 
//
// return : 있을 경우 true, 없을 경우 false를 리턴
//
///////////////////////////////////////////////////////////////
bool SearchStrStr( const char *szSource, const char *szWhite )
{
	int len = strlen( szWhite );
	
	for( int i=0; i<len; i++ )
	{
		if( strchr( szSource, szWhite[i] ) ) return true; 
	}
	
	return false;
}

int SetTactics( int spell_type, int sex, int tac_type )		// 캐릭터 메이크할경우 선택한 택틱스를 실제 배열첨자로 바꾸는 루틴..
{
	int tac_con[2][12] = { 
		{ CRAPPLE, SWORD, ARCHERY, FENCING, MACE, PIERCE, WHIRL, HURL, PARRYING, MAGERY, },		 // 여자
		{ CRAPPLE, SWORD, ARCHERY, PARRYING, MACE, PIERCE, WHIRL, HURL, MAGERY, D_SWORD, D_MACE, },	// 남자
	};
	
	int ret = tac_con[sex][tac_type];
	if( spell_type == PRIEST_SPELL && ret == MAGERY ) ret = ORISON;
	return ret;
}

void SetTacticsBySpell(SCharacter *ch )
{
	if( ch->nCharacterData[SPELL] == WIZARD_SPELL )
	{
		wsprintf(SHideNomal[HN_MAKECHARACTER_TACTICS_WOMAN_TEXT][9].temp,"MAGERY");
		wsprintf(SHideNomal[HN_MAKECHARACTER_TACTICS_MAN_TEXT][8].temp,"MAGERY");
	}
	else if( ch->nCharacterData[SPELL] == PRIEST_SPELL )
	{
		wsprintf(SHideNomal[HN_MAKECHARACTER_TACTICS_MAN_TEXT][8].temp,"ORISON");
		wsprintf(SHideNomal[HN_MAKECHARACTER_TACTICS_WOMAN_TEXT][9].temp,"ORISON");
	}
}


void CheckFocusMenu()
{
	if( y_MenuFocus && !SMenu[y_MenuFocus].bActive ) 
	{
		SmallMenuClose();
	}
}

extern void SkillSettingClear( void );
void GrobalValueSeet()
{
	y_MenuFocus = 0;
	SkillSettingClear( );
	RectImageOn = 0;
}

int CanLearnMagicByCount( int num )		// 한계열당 5개 이상의 마법은 배울 수 없다..
{
	int count =  0;
	for( int i=0; i<200; i++ )
	{
		int m_c = magic[num].GetClass();
		if( SCharacterData.MagicId[i] && m_c == magic[i].GetClass() ) count++;
	}
	if( count > 5 ) return 0;
	return 1;
}

void InitPartyState( )
{
	memset( SCharacterData.party, 0, sizeof( CharacterParty ) * 6 );
	memset( SCharacterData.relation, 0, sizeof( CharacterParty ) * 3 );
	memset( SCharacterData.employment, 0, sizeof( CharacterParty ) * 6 );
}



///////////////////////////////////////////////////////////////////////
//
// sysmenu.dat 화일이 너무 커서 두개로 나눔...
//
///////////////////////////////////////////////////////////////////

void FieldTypeNomalPutFx2(int mx, int my, int x,int y,int nImageNumber, int t, int j)
{
	int num = nImageNumber;
	if( !spr2[num].img ) 
	{
		LoadSmallMenuImage2( num );
	}
	PutCompressedImageFX(mx+x+spr2[num].ox, my+y+spr2[num].oy, &spr2[num], t, j);
}

void FieldTypeNomalPut2(int mx, int my, int x,int y,int nImageNumber)
{
	int num = nImageNumber;
	if( !spr2[num].img )
	{
		LoadSmallMenuImage2( num );
	}
	PutCompressedImage(mx+x+spr2[num].ox, my+y+spr2[num].oy, &spr2[num]);
}

void LoadSmallMenuImage2( int num )
{
	FILE	*fp;
	
	DWORD Off=0;
	DWORD size;
	
	if( fp = Fopen("./data/Sysmenu2.dat", "rb" ) )
	{
		fseek(fp, 4 * num + 2, SEEK_SET);
		fread( &Off, sizeof( DWORD ), 1, fp);
		fseek(fp, Off, SEEK_SET);
		
		fread( &size, sizeof( DWORD ), 1, fp);
		fread( &spr2[ num ].xl, sizeof( short ), 1, fp);
		fread( &spr2[ num ].yl, sizeof( short ), 1, fp);
		fread( &spr2[ num ].ox, sizeof( short ), 1, fp);
		fread( &spr2[ num ].oy, sizeof( short ), 1, fp);
		fread( &spr2[ num ].size, sizeof( unsigned int ), 1, fp);
		dw_TotalImageSize += spr2[ num ].size;
		
		MemFree( spr2[num].img );
		MemAlloc( spr2[num].img, spr2[num].size );
		fread( spr2[ num ].img, spr2[ num ].size, 1, fp);
		
		dw_TotalImageSize += spr2[ num ].size;
		
		convert565to555( &spr2[ num ] );
		
		fclose(fp);
	}
	else JustMsg("SysMenu2.dat file damaged Update Now");
}

void FreeSmallMenuImage2( )			// 싹다 지울때
{
	for( int i=0; i<MAX_IMAGE2; i++ )
	{
		if( spr2[i].img )
		{
			SAFE_DELETE_ARRAY( spr2[i].img );
		}
		spr2[i].img = NULL;
	}
}

void FreeMenuData2( )		// 스몰메뉴에서 사용하는 걸 지울때
{	
	int max = sizeof( smallmenu_img2 ) / sizeof( int );
	
	for( int i=0; i<max; i++ )
	{
		if( spr2[smallmenu_img2[i]].img )
		{
			MemFree( spr2[smallmenu_img2[i]].img );
		}
		spr2[smallmenu_img2[i]].img = NULL;
		i++;
	}
}

DWORD GetExpByLv( int lv )		// 레벨을 경험치로 변환해 준다.
{
	return NPC_Lev_Ref[lv].nMinExp + 1 ;
}

/*
void PutTextDc( HWND hWnd )
{
HDC hdc;
hdc = GetDc( hWnd );

  //	if ( lpSurface->GetDC( &hdc ) == DD_OK )
  {																	
		int			nBkMode;										
		nBkMode = ::SetBkMode( hdc, TRANSPARENT );
		
		  
			SetTextColor( hdc, RGB( HR, HG, HB ));
			
			  
				SetTextAlign( hdc, HangulAlign );
				TextOut( hdc, x, y, txt, strlen( txt ));
				::SetBkMode( hdc, nBkMode );
				
				  SelectObject( hdc, hFontOld );
				  //		SelectObject( hdc, hGrnOld );
				  
					DeleteObject( MainHrgn );
					//		DeleteObject( __HFontDial[HangulFontType] );
					
					  //		lpSurface->ReleaseDC( hdc );
					  }			
					  }
*/


///////////////////////////////////////////////////////////////////////////////////////
//일정 영역의 사각형 안에 문자열을 출력하는 함수  FONT사용
extern int RectTextPutBasic ( const int,const int,int,char *,const int,const int,bool,bool,char** );

int GetLineRectTextPut( int nWidth, char *sSource ) // line의 수를 체크하여..
{	//< CSD-030324
	return GetRectTextLine( sSource, nWidth );
}	//> CSD-030324

char *DivideStringByRect( int nWidth, char *sSource, int max_line ) // 스트링을 나눠준다.
{	//< CSD-030324
	if( !max_line ) max_line = 1;

	char *pEnd = NULL;
	RectTextPutBasic( 0, 0, nWidth, sSource, 0, max_line, false, false, &pEnd );
	return pEnd;
}	//> CSD-030324

int SkipRectTextPut( int x, int y, int nWidth, int skip, char *sSource, int line_max, int fx_type ) // line의 수를 체크하여..
{	
	int nLenghtTotal,nLenghtStart,nLenghtEnd,nLine;
	nLenghtTotal=nLenghtStart=nLenghtEnd=nLine=0; 
	//각각 출력하고자 하는 문자열의 전체 길이,한줄에 시작위치 끝위치, 몇번째 줄에 찍는지에 대한 위치 이다.
	
	nLenghtTotal=strlen(sSource); //전체 문자열의 길이를 얻어 낸다
	if( skip)
		sSource = DivideStringByRect( nWidth, sSource, skip );
	if( !sSource ) return NULL;
	// skip 하고 난후 
	return RectTextPut( x, y, nWidth, sSource+nLenghtEnd, 18, line_max);
}



#include "ijl.h"
extern DDSURFACEDESC ddSurfaceDesc;
// thai2 YGI
bool CaptureScreenForJpg2 ( int file_num, Spr *pSpr )
{	//< CSD-030324
	if( !GetSysInfo( SI_GAME_MAKE_MODE ) ) return false;
	char szFileName[MAX_PATH];
	sprintf( szFileName, "./graphic_data/%04d.jpg", file_num );

	JPEG_CORE_PROPERTIES image = {0, };

	if( ijlInit( &image ) != IJL_OK )
	{
		if( GetSysInfo( SI_GAME_MAKE_MODE ) )
			JustMsg( "Can't initialize Intel(R) JPEG library\n" );
		return false;
	}

	char *pBackup;
	pBackup = g_DestBackBuf;
	g_DestBackBuf= new char[dDxSize*SCREEN_HEIGHT];
	PutCompressedImage( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, pSpr );

    BYTE r, g, b ;
	BYTE *bitmap_buf = new BYTE[SCREEN_HEIGHT*SCREEN_WIDTH*3];
	BYTE *temp = bitmap_buf;
	
	for( int i = 0 ; i < SCREEN_HEIGHT ; i ++)
	{
		for( int j = 0 ; j < SCREEN_WIDTH * 2 ; j +=2)
		{
			if( _PixelInfo.BitMaskR != 0x7C00) // 555가 아니면.
			{
				r = (0xf800 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 8;
				g = (0x07c0 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 3;
				b = (0x001f & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))<< 3;
			}
			else 
			{
				r = (0x7c00 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 7;
				g = (0x03e0 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 2;
				b = (0x001f & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))<< 3;
			}
			
			*temp = b; temp++;
			*temp = g; temp++;
			*temp = r; temp++;
		}
	}
	
	////////////////////////////// bitmap -> jpg ///////////////////////////
	// Setup DIB
	image.DIBWidth    = SCREEN_WIDTH;
    image.DIBHeight   = SCREEN_HEIGHT;
	image.DIBBytes    = bitmap_buf;
    image.DIBPadBytes = IJL_DIB_PAD_BYTES(image.DIBWidth,3 );

    // Setup JPEG
    image.JPGFile   = const_cast<char*>( szFileName );
    image.JPGWidth  = SCREEN_WIDTH;
    image.JPGHeight = SCREEN_HEIGHT;
	image.jquality	= 75;	// default 75

	int ret = ijlWrite( &image, IJL_JFILE_WRITEWHOLEIMAGE );
    if( ret != IJL_OK )
    {
		if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	        JustMsg( "Can't write image\n" );
		goto _false;
		delete bitmap_buf;
		return false;
    }

    if( ijlFree( &image ) != IJL_OK )
    {
		if( GetSysInfo( SI_GAME_MAKE_MODE ) )
			JustMsg( "Can't free Intel(R) JPEG library\n" );
		goto _false;
    }
	/////////////////////////////////////////////////////////////////////////

	MP3( SN_LV_EACH_DOWN );
	delete bitmap_buf;
	delete g_DestBackBuf;
	g_DestBackBuf = pBackup;

	return true;

_false : 
	delete g_DestBackBuf;
	g_DestBackBuf = pBackup;
	delete bitmap_buf;
	

	return false;
}	//> CSD-030324

bool CaptureScreenForJpg ( void )//020926 lsw
{	
	int c=0;
	FILE *fp = Fopen( "capturescreen.tmp", "rt" );
	if( fp )
	{
		fscanf( fp, "%d", &c );
		fclose(fp);
	}
	SYSTEMTIME time;
	char szFileName[MAX_PATH] = {0,};
	GetLocalTime(&time);
	
	char szTargetFolder[MAX_PATH] = {0,}; 
	const char szScreenShotFolder[] = "/MyScreenshot";
	sprintf(szTargetFolder,"%s%s",GetCurrentWorkingDirectory(),szScreenShotFolder);
	
	mkdir(szTargetFolder);
	sprintf( szFileName, "%s/Save %d-%d-%d %d-%d(%d).jpg", //2002 09 25 11 15
		szTargetFolder,
		time.wYear,
		time.wMonth,
		time.wDay,
		time.wHour,
		time.wMinute,
		c);
	
	JPEG_CORE_PROPERTIES image = {0, };
	
	if( ijlInit( &image ) != IJL_OK )
	{
		if( GetSysInfo( SI_GAME_MAKE_MODE ) )
			JustMsg( "Can't initialize Intel(R) JPEG library\n" );
		return false;
	}
	
    BYTE r, g, b ;
	BYTE *bitmap_buf = new BYTE[SCREEN_HEIGHT*SCREEN_WIDTH*3];
	BYTE *temp = bitmap_buf;
	
	for( int i = 0 ; i < SCREEN_HEIGHT ; i ++)
	{
		for( int j = 0 ; j < SCREEN_WIDTH * 2 ; j +=2)
		{
			if( _PixelInfo.BitMaskR != 0x7C00) // 555가 아니면.
			{
				r = (0xf800 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 8;
				g = (0x07c0 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 3;
				b = (0x001f & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))<< 3;
			}
			else 
			{
				r = (0x7c00 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 7;
				g = (0x03e0 & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))>> 2;
				b = (0x001f & *(unsigned short *)(g_DestBackBuf + i * dDxSize + j ))<< 3;
			}
			
			*temp = b; temp++;
			*temp = g; temp++;
			*temp = r; temp++;
		}
	}
	
	////////////////////////////// bitmap -> jpg ///////////////////////////
	// Setup DIB
	image.DIBWidth    = SCREEN_WIDTH;
    image.DIBHeight   = SCREEN_HEIGHT;
	image.DIBBytes    = bitmap_buf;
    image.DIBPadBytes = IJL_DIB_PAD_BYTES(image.DIBWidth,3 );
	
    // Setup JPEG
    image.JPGFile   = const_cast<char*>( szFileName );
    image.JPGWidth  = SCREEN_WIDTH;
    image.JPGHeight = SCREEN_HEIGHT;
	image.jquality	= 75;	// default 75
	
	int ret = ijlWrite( &image, IJL_JFILE_WRITEWHOLEIMAGE );
    if( ret != IJL_OK )
    {
		if( GetSysInfo( SI_GAME_MAKE_MODE ) )
			JustMsg( "Can't write image\n" );
		delete bitmap_buf;
		return false;
    }
	
    if( ijlFree( &image ) != IJL_OK )
    {
		if( GetSysInfo( SI_GAME_MAKE_MODE ) )
			JustMsg( "Can't free Intel(R) JPEG library\n" );
		delete bitmap_buf;
		return false;
    }
	/////////////////////////////////////////////////////////////////////////
	
	MP3( SN_LV_EACH_DOWN );
	delete bitmap_buf;
	
	c++;
	fp =Fopen( "capturescreen.tmp", "wt" );
	if( fp )
	{	
		fprintf( fp, "%d", c );
		fclose(fp);
	}	
	AddCurrentStatusMessage( 50,250,160, lan->OutputMessage(7,300), szFileName );
	return true;
}

int GetBelieveGodByMenu( int menu, char *God )		// 신전 메뉴에서 믿는 신으로...
{
	switch( menu )
	{
	case MN_GOD_EDELBLHOY	: if( God ) sprintf( God, lan->OutputMessage(7,66)); return EDELBLHOY;//010215 lsw
	case MN_GOD_TEFFERY		: if( God ) sprintf( God, lan->OutputMessage(7,67)); return TEFFERY;//010215 lsw
	case MN_GOD_LETTY		: if( God ) sprintf( God, lan->OutputMessage(7,68)); return LETTY;//010215 lsw
	case MN_GOD_OREM		: if( God ) sprintf( God, lan->OutputMessage(7,69)); return OREM;
		
	default : if( God ) sprintf( God, ""); return 0;
	}
	
	return 0;
}

inline int GetMenuNumByBelieveGod( int god )
{
	static int god_menu[8] = { 0, MN_GOD_EDELBLHOY, MN_GOD_TEFFERY, MN_GOD_LETTY, MN_GOD_OREM };
	return god_menu[god];
}
void CallGodMenuMessage( char *msg, int believe )
{
	int pre_menu = GetMenuNumByBelieveGod(believe );
	CallSmallMenu( MN_GOD_REGIST_MESSAGE, pre_menu );
	sprintf( SMenu[MN_GOD_REGIST_MESSAGE].nField[2].temp, "%s", msg );
}


void FreeSmallMenuETC()
{
	int i;
	
	if( god_spr[0].img )
	{
		FreeOpenningSpr( &god_spr_buf );		// 신전메뉴 프리
		for( i = 0 ; i< GOD_MENU_IMG_MAX; i++ ) god_spr[i].img = NULL;
	}
	if( time_spr[0].img )
	{
		FreeOpenningSpr( &time_spr_buf );		// 시간 표시 메뉴 프리
		for( i = 0 ; i< TIME_MENU_IMG_MAX; i++ ) time_spr[i].img = NULL;
	}
	if( membook_spr[0].img )
	{
		FreeOpenningSpr( &membook_spr_buf );		
		for( i = 0 ; i< MEMBOOK_MENU_IMG_MAX; i++ ) membook_spr[i].img = NULL;
	}
	if( magicbook_spr[0].img )
	{
		FreeOpenningSpr( &magicbook_spr_buf );		
		for( i = 0 ; i< MAGICBOOK_MENU_IMG_MAX; i++ ) magicbook_spr[i].img = NULL;
	}
	if( mail_spr[0].img )
	{
		FreeOpenningSpr( &mail_spr_buf );		
		for( i = 0 ; i< MAIL_MENU_IMG_MAX; i++ ) mail_spr[i].img = NULL;
	}
	if( small_etc_spr[0].img )
	{
		FreeOpenningSpr( &small_etc_buf );
		for( i = 0 ; i< SMALL_ETC_MAX; i++ ) small_etc_spr[i].img = NULL;
	}
	if( public_bbs_spr[0].img )
	{
		FreeOpenningSpr( &public_bbs_buf );		
		for( i = 0 ; i< PUBLIC_BBS_MENU_IMG_MAX; i++ ) public_bbs_spr[i].img = NULL;
	}
	if( hunter_spr[0].img )
	{
		FreeOpenningSpr( &hunter_buf );		
		for( i = 0 ; i< HUNTER_MENU_IMG_MAX; i++ ) hunter_spr[i].img = NULL;
	}
	if( questmenu_spr[0].img )
	{
		FreeOpenningSpr( &questmenu_buf );
		for( i = 0 ; i< QUEST_MENU_MAX; i++ ) questmenu_spr[i].img = NULL;
	}
	if( nation_spr[0].img )
	{
		FreeOpenningSpr( &nation_buf );		
		for( i = 0 ; i< NATION_MENU_IMG_MAX; i++ ) nation_spr[i].img = NULL;
	}
	if( scenario_spr.img )
	{
		MemFree( scenario_spr.img );
	}
	if( vote_spr[0].img )
	{
		FreeOpenningSpr( &vote_buf );		
		for( i = 0 ; i< VOTE_MENU_IMG_MAX; i++ ) vote_spr[i].img = NULL;
	}
	if( guild_admin_spr[0].img )
	{
		FreeOpenningSpr( &guild_admin_buf );
		for( i=0; i<GUILD_ADMIN_MAX; i++ ) guild_admin_spr[i].img = NULL;
	}
	if( guild_skill_list_spr[0].img )
	{
		FreeOpenningSpr( &guild_skill_list_buf );
		for( i=0; i<GUILD_SKILL_LIST_MAX; i++ ) guild_skill_list_spr[i].img = NULL;
	}
	if( guild_admin_etc_spr[0].img )
	{
		FreeOpenningSpr( &guild_admin_etc_buf );
		for( i=0; i<GUILD_ADMIN_ETC_MAX; i++ ) guild_admin_etc_spr[i].img = NULL;
	}
	if( bank_spr[0].img )
	{
		FreeOpenningSpr( &bank_buf );
		for( i=0; i<BANK_MAX; i++ ) bank_spr[i].img = NULL;
	}
	if( salvation_spr[0].img )
	{
		FreeOpenningSpr( &salvation_buf );
		for( i=0; i<SALVATION_MAX; i++ ) salvation_spr[i].img = NULL;
	}
	if( cahnge_money_spr[0].img )
	{
		FreeOpenningSpr( &cahnge_money_buf );
		for( i=0; i<CAHNGE_MONEY_MAX; i++ ) cahnge_money_spr[i].img = NULL;
	}
	if( merchant_spr[0].img )
	{
		FreeOpenningSpr( &merchant_buf );
		for( i=0; i<MERCHANT_SPR_MAX; i++ ) merchant_spr[i].img = NULL;
	}
	if( guild_menu_spr[0].img )
	{
		FreeOpenningSpr( &guild_menu_buf );
		for( i=0; i<GUILD_MENU_SPR_MAX; i++ ) guild_menu_spr[i].img = NULL;
	}
	if( dual_img_spr[0].img )
	{
		FreeOpenningSpr( &dual_img_buf );
		for( i=0; i<DUAL_IMG_SPR_MAX; i++ ) dual_img_spr[i].img = NULL;
	}
	//////////////////////////////////////////
	//010730 lsw 이거 지우면 느려져여 ㅡㅡ
	if( nation_battle_bbs_spr[0].img )
	{
		FreeOpenningSpr( &nation_battle_bbs_buf );
		for( i=0; i<NATION_BATTLE_BBS_SPR_MAX; i++ ) nation_battle_bbs_spr[i].img = NULL;
	}
	
	if( nation_info_spr[0].img )
	{
		FreeOpenningSpr( &nation_info_buf);
		for( i=0; i<NATION_INFO_SPR_MAX; i++ ) nation_info_spr[i].img = NULL;
	}
	
	if( nation_budae_spr[0].img )
	{
		FreeOpenningSpr( &nation_budae_buf);
		for( i=0; i<NATION_BUDAE_SPR_MAX		; i++ ) nation_budae_spr[i].img = NULL;
	}
	
	if( nation_ladder_spr[0].img )
	{
		FreeOpenningSpr( &nation_ladder_buf);
		for( i=0; i<NATION_LADDER_SPR_MAX; i++ ) nation_ladder_spr[i].img = NULL;
	}
	
	if( nation_vote_spr[0].img )
	{
		FreeOpenningSpr( &nation_vote_buf);
		for( i=0; i<NATION_VOTE_SPR_MAX; i++ ) nation_vote_spr[i].img = NULL;
	}
	
	if( nation_join_spr[0].img )
	{
		FreeOpenningSpr( &nation_join_buf);
		for( i=0; i<NATION_JOIN_SPR_MAX; i++ ) nation_join_spr[i].img = NULL;
	}
	
	if( nation_now_spr[0].img )
	{
		FreeOpenningSpr( &nation_now_buf);
		for( i=0; i<NATION_NOW_SPR_MAX; i++ ) nation_now_spr[i].img = NULL;
	}
	
	if( nation_guest_spr[0].img )
	{
		FreeOpenningSpr( &nation_guest_buf );
		for( i=0; i<NATION_GUEST_SPR_MAX; i++ ) nation_guest_spr[i].img = NULL;
	}
	
	if( nation_goto_war_map_spr[0].img )
	{
		FreeOpenningSpr( &nation_goto_war_map_buf );
		for( i=0; i<NATION_GOTO_WAR_MAP_SPR_MAX; i++ ) nation_goto_war_map_spr[i].img = NULL;
	}
	
	if( nation_msgbox_spr[0].img )
	{
		FreeOpenningSpr( &nation_msgbox_buf);
		for( i=0; i<NATION_MSGBOX_SPR_MAX; i++ ) nation_msgbox_spr[i].img = NULL;
	}
	
	if( nation_donebox_spr[0].img )
	{
		FreeOpenningSpr( &nation_donebox_buf);
		for( i=0; i<NATION_DONEBOX_SPR_MAX; i++ ) nation_donebox_spr[i].img = NULL;
	}
	
	if( nation_today_notice_spr[0].img )
	{
		FreeOpenningSpr( &nation_today_notice_buf);
		for( i=0; i<NATION_TODAY_NOTICE_SPR_MAX; i++ ) nation_today_notice_spr[i].img = NULL;
	}
	
	if( nation_war_result_spr[0].img )
	{
		FreeOpenningSpr( &nation_war_result_buf);
		for( i=0; i<NATION_WAR_RESULT_SPR_MAX; i++ ) nation_war_result_spr[i].img = NULL;
	}
	
	if( gm_2nd_skill_spr[0].img )
	{
		FreeOpenningSpr( &gm_2nd_skill_buf );
		for( i=0; i<GM_2ND_SKILL_SPR_MAX; i++ ) gm_2nd_skill_spr[i].img = NULL;
	}
	if( tutorial_display_spr[0].img )
	{
		FreeOpenningSpr( &tutorial_display_buf);
		for( i=0; i<TUTORIAL_DISPLAY_SPR_MAX; i++ ) tutorial_display_spr[i].img = NULL;
	}
	
	
	//011013 lsw >
	if( item_make_rare_select_spr[0].img )
	{
		FreeOpenningSpr( &item_make_rare_select_buf);
		for( i=0; i<ITEM_MAKE_RARE_SELECT_SPR_MAX; i++ ) item_make_rare_select_spr[i].img = NULL;
	}
	//011013 lsw <
	if( rune_spr[0].img )
	{
		FreeOpenningSpr( &rune_buf);
		for( i=0; i<RUNE_SPR_MAX; i++ ) rune_spr[i].img = NULL;
	}
	if( rune_effect_spr1[0].img )
	{
		FreeOpenningSpr( &rune_effect_buf1);
		for( i=0; i<RUNE_EFFECT_SPR_MAX; i++ ) rune_effect_spr1[i].img = NULL;
	}
	if( rune_effect_spr2[0].img )
	{
		FreeOpenningSpr( &rune_effect_buf2);
		for( i=0; i<RUNE_EFFECT_SPR_MAX; i++ ) rune_effect_spr2[i].img = NULL;
	}
	if( rune_effect_spr3[0].img )
	{
		FreeOpenningSpr( &rune_effect_buf3);
		for( i=0; i<RUNE_EFFECT_SPR_MAX; i++ ) rune_effect_spr3[i].img = NULL;
	}
	if( rune_effect_spr4[0].img )
	{
		FreeOpenningSpr( &rune_effect_buf4);
		for( i=0; i<RUNE_EFFECT_SPR_MAX; i++ ) rune_effect_spr4[i].img = NULL;
	}
	if( rune_effect_spr[0].img )
	{
		FreeOpenningSpr( &rune_effect_buf);
		for( i=0; i<RUNE_EFFECT_SPR_MAX; i++ ) rune_effect_spr[i].img = NULL;
	}
	if( skin_menu_spr[0].img )
	{
		FreeOpenningSpr( &skin_menu_buf);
		for( i=0; i<SKIN_MENU_MAX; i++ ) skin_menu_spr[i].img = NULL;
	}
	if( gamble_menu_spr[0].img )
	{
		FreeOpenningSpr( &gamble_menu_buf);
		for( i=0; i<GAMBLE_SPR_MAX; i++ ) gamble_menu_spr[i].img = NULL;
	}
	//011014  lsw >
	if( tutorial_display_tmp_spr[0].img )
	{
		FreeOpenningSpr( &tutorial_display_tmp_buf);
		for( i=0; i<TUTORIAL_DISPLAY_SPR_TMP_MAX; i++ ) tutorial_display_tmp_spr[i].img = NULL;
	}
	//011014 lsw <
	
	if (Ability_Redistribution_Spr[0].img)			// 011201 LTS // LTS MENU
	{
		FreeOpenningSpr( &Ability_Redistribution_Buf);
		for( i=0; i<ABILITY_REDISTRIBUTION_SPR_MAX; i++ ) Ability_Redistribution_Spr[i].img = NULL;
	}
	
	if (LocalWar_Join_Spr[0].img)					// 011201 LTS
	{
		FreeOpenningSpr( &LocalWar_Join_Buf);
		for( i=0; i<LOCALWAR_JOIN_SPR_MAX; i++ ) LocalWar_Join_Spr[i].img = NULL;
	}
	
	if( guild_member_list_spr[0].img )			// LTS LOCALWAR BUG
	{
		FreeOpenningSpr( &guild_member_list_buf);
		for( i=0; i<IM_GUILD_MEMBER_LIST; i++ ) guild_member_list_spr[i].img = NULL;
	}
	
	if (LocalWar_OK_Spr[0].img)					// LTS LOCALWAR BUG
	{
		FreeOpenningSpr( &LocalWar_OK_Buf);
		for( i=0; i<LOCALWAR_OK_SPR_MAX; i++ ) LocalWar_OK_Spr[i].img = NULL;
	}
	
	if( friend_menu_spr[0].img )
	{
		FreeOpenningSpr( &friend_menu_buf);
		for( i=0; i<IM_FRIEND_MENU; i++ ) friend_menu_spr[i].img = NULL;
	}
	
	if( potal_menu_spr[0].img )
	{
		FreeOpenningSpr( &potal_menu_buf);
		for( i=0; i<IM_POTAL_MENU; i++ ) potal_menu_spr[i].img = NULL;
	}
	if( party_button_spr[0].img )
	{
		FreeOpenningSpr( &party_button_buf);
		for( i=0; i<IM_PARTY_BUTTON; i++ ) party_button_spr[i].img = NULL;
	}
	if( guild_item_spr[0].img )
	{
		FreeOpenningSpr( &guild_item_buf);
		for( i=0; i<IM_GUILD_ITEM; i++ ) guild_item_spr[i].img = NULL;
	}
	if( guild_change_info_spr[0].img )
	{
		FreeOpenningSpr( &guild_change_info_buf);
		for( i=0; i<IM_GUILD_CHANGE_INFO; i++ ) guild_change_info_spr[i].img = NULL;
	}
	
	
	
	if (EventLocalWar_OK_Spr[0].img)			// 020115 LTS
	{
		FreeOpenningSpr( &EventLocalWar_OK_Buf);
		for( i=0; i<EVENT_LOCALWAR_OK_SPR_MAX; i++ ) EventLocalWar_OK_Spr[i].img = NULL;
	}
	if (EventLocalWar_Move_Spr[0].img)			// 020115 LTS
	{
		FreeOpenningSpr( &EventLocalWar_Move_Buf);
		for( i=0; i<EVENT_LOCALWAR_MOVE_SPR_MAX; i++ ) EventLocalWar_Move_Spr[i].img = NULL;
	}
	
	if (WarStartSpr[0].img)
	{
		FreeOpenningSpr( &WarStartBuf);
		for( i=0; i<WAR_START_SPR_MAX; i++ ) WarStartSpr[i].img = NULL;
	}
	if (WarMoveSpr[0].img)
	{
		FreeOpenningSpr( &WarMoveBuf);
		for( i=0; i<WAR_START_SPR_MAX; i++ ) WarMoveSpr[i].img = NULL;
	}
	if (WarStatusSpr[0].img)
	{
		FreeOpenningSpr( &WarStatusBuf);
		for( i=0; i<WAR_STATUS_SPR_MAX; i++ ) WarStatusSpr[i].img = NULL;
	}
//<! BBD 040311	
	if (MonsterWarInfoSpr[0].img)
	{
		FreeOpenningSpr( &MonsterWarInfoBuf);
		for( i=0; i<MONSTERWAR_INFO_SPR_MAX; i++ ) MonsterWarInfoSpr[i].img = NULL;
	}
//> BBD 040311		
	
}

// thai2 YGI
int g_SmallMapNumber;
int QuickKeyByGameMakeMode( int menu, HWND active_hwnd )
{	//< CSD-030324
	switch( menu )
	{
		case MN_SMALL_MAP :
		{
			int old_key = SMenu[menu].key2;
			SMenu[menu].key2 = -1;

			if(g_aCurrentKeys[DIK_PGUP] & 0x80 ){	SMenu[menu].key2 = 'u';}//q
			if(g_aCurrentKeys[DIK_PGDN] & 0x80 ){	SMenu[menu].key2 = 'd';}//q
			if( SMenu[menu].key2 == -1 && old_key > 0 )
			{
				switch( old_key )
				{
				case 'u' :	{g_SmallMapNumber --; if(g_SmallMapNumber<0) g_SmallMapNumber = 0; }break;
				case 'd' :	{g_SmallMapNumber ++; if(g_SmallMapNumber>100) g_SmallMapNumber = 100; }break;
				}
			}
		}
	}

	return 1;
}	//> CSD-030324

int QuickKey( int menu, HWND active_hwnd )		// 단축키 // 주의: 필드번호를 직접 참조하므로 관련된 필드번호를 바꿀땐 주의한다.
{//021001 lsw
	if( y_MenuFocus && y_MenuFocus != menu ) {return 0;}
	if(!EWndMgr.IsFocus(HWND_GAME_MAIN)){return 0;}
	//< CSD-030509
	if (IsDead(Hero)) {return 0;}	

	switch (Hero->viewtype)
	{
	case VIEWTYPE_GHOST_:
	case VIEWTYPE_OBSERVE_:
		{
			return 0;
		}
	}
	//> CSD-030509
	// thai2 YGI
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )  
	{	//< CSD-030324
		return QuickKeyByGameMakeMode( menu, active_hwnd );
	}	//> CSD-030324
#endif
	if (g_aCurrentKeys[DIK_GRAVE]&0x80)
	{	//< CSD-030408 : 전투스킬 취소 단축키
		CSDMainIconClear();
		return 1;
	}	//> CSD-030408

	switch( menu )
	{
	case MN_MAININTERFACE :
		{
			int old_key = SMenu[menu].key2;
			SMenu[menu].key2 = -1;
			//	if(g_aCurrentKeys[DIK_] & 0x80 )	SMenu[menu].key2 = '';
			if(g_aCurrentKeys[DIK_Q] & 0x80 ){	SMenu[menu].key2 = 'q';}//q
			if(g_aCurrentKeys[DIK_E] & 0x80 ){	SMenu[menu].key2 = 'e';}//e
			if(g_aCurrentKeys[DIK_A] & 0x80 ){	SMenu[menu].key2 = 'a';}//a
			if(g_aCurrentKeys[DIK_S] & 0x80 ){	SMenu[menu].key2 = 's';}//s
			if(g_aCurrentKeys[DIK_D] & 0x80 ){	SMenu[menu].key2 = 'd';}//d
			if(g_aCurrentKeys[DIK_Z] & 0x80 ){	SMenu[menu].key2 = 'z';}
			if(g_aCurrentKeys[DIK_X] & 0x80 ){	SMenu[menu].key2 = 'x';}
			if(g_aCurrentKeys[DIK_O] & 0x80 ){	SMenu[menu].key2 = 'o';}
			if(g_aCurrentKeys[DIK_L] & 0x80 ){	SMenu[menu].key2 = 'l';}
			if(g_aCurrentKeys[DIK_M] & 0x80 ){	SMenu[menu].key2 = 'm';}
			if(g_aCurrentKeys[DIK_F] & 0x80 ){	SMenu[menu].key2 = 'f';}
			if(g_aCurrentKeys[DIK_TAB] & 0x80 ){SMenu[menu].key2 = 't';}				
			if(g_aCurrentKeys[DIK_SLASH]& 0x80){SMenu[menu].key2 = '/';}//등록된 퀵				
			
			if(g_aCurrentKeys[DIK_W] & 0x80 )	
			{
				if( g_aCurrentKeys[DIK_LCONTROL] & 0x80 ) 
				{
					SMenu[menu].key2 = 'c';		// 귓속말
				}
				else
				{
					SMenu[menu].key2 = 'w';//w
				}
			}
			for(int i=0;i<5;i++)							// 1 ~ 6번 숫자키//020515 lsw 퀵창은 0~5 까지
			{
				if((g_aCurrentKeys[DIK_1 + i] & 0x80) && QuickItemAttr[i].item_no && !IsMagicWindowOn() )
				{
					SMenu[menu].key2 = DIK_1 + i;
				}
			}
			
			if( SMenu[menu].key2 == -1 && old_key > 0 )
			{
				switch( old_key )
				{
				case 'q' :	{DoButtonCheckOfMenu( menu,	STATUS_FIELD); }break;
				case 'w' :	{DoButtonCheckOfMenu( menu,	INVENTORY_FIELD); }break;
				case 'e' :	{DoButtonCheckOfMenu( menu,	PARTY_FIELD); }break;
				case 'a' :	{DoButtonCheckOfMenu( menu,	MAGIC_FIELD); }break;
				case 's' :	{DoButtonCheckOfMenu( menu,	COMBAT_SKILL_POINT_FIELD); }break;
				case 'd' :	{DoButtonCheckOfMenu( menu,	SKILL_FIELD); }break;
				case 'z' :	{DoButtonCheckOfMenu( menu,	TODAY_NOTICE_FIELD); }break;
				case 'x' :	{DoButtonCheckOfMenu( menu,	NATION_BBS_FIELD); }break;
				case 'o' :	{DoButtonCheckOfMenu( menu,	OPTION_FIELD); }break;
				case 'l' :	{DoButtonCheckOfMenu( menu,	MAIL_FIELD); }break;
				case 'm' :	{DoButtonCheckOfMenu( menu,	MAP_FIELD); }break;
				case 'f' :	{DoButtonCheckOfMenu( menu,	FRIEND_FIELD); }break;
				case 't' :	{DoButtonCheckOfMenu( menu,	PEACE_BATTLE_FIELD); }break;
				case '/' :	{DoButtonCheckOfMenu( menu,	HOT_KEY_FIELD); }break;
				case 'c' :	
					{
						SendWisperByLineCommand( "" );
					}break;		//  귓속말
					
				}
				
				for(int i=0;i<5;i++)							// 1 ~ 6번 숫자키//020515 lsw 1~5 까지만 된다
				{
					if( old_key == DIK_1+i )
					{
						if( IsHeroDead() || 
							SMenu[MN_FACE].bActive ) return 0;
						
						POS ItemPos;
						SetItemPos(QUICK, i, &ItemPos);
						EatItem(QuickItemAttr[i],ItemPos);
					}
				}
			}
			break;
		}
	case MN_LEVELUP:{
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		if(g_aCurrentKeys[DIK_U] & 0x80 )		SMenu[menu].key2 = 'u';	// 레벨업 버튼
		if( SMenu[menu].key2 == -1 && old_key == 'u' ) DoButtonCheckOfMenu( menu, 0 ); //	레벨업 버튼
		break;
					}
	case MN_ABILITY : {
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		if(g_aCurrentKeys[DIK_U] & 0x80 )		SMenu[menu].key2 = 'u';	
		//							if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )		SMenu[menu].key2 = 'u';	
		if( SMenu[menu].key2 == -1 && old_key == 'u' ) DoButtonCheckOfMenu( menu, 1 ); //	어빌리티 창 닫기
		break;
	
					  }
		/* // 031029 kyo 이모티콘 단축키 없앰
	case MN_FACE :	{
		int face_num = -1;
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		
		for( int i=7; i>=0; i-- )
			if(g_aCurrentKeys[DIK_1 + i] & 0x80 )	
			{
				SMenu[menu].key2 = DIK_1+i;						
			}
			
			if(g_aCurrentKeys[DIK_P] & 0x80 )		SMenu[menu].key2 = 'p';	// 얼굴표정 메뉴 닫기
			if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )		SMenu[menu].key2 = 'p';	// 얼굴표정 메뉴 닫기
			if( SMenu[menu].key2 == -1 && old_key > 0 )
			{

				if( old_key == 'p' ) SMenu[MN_FACE].bActive = false; 
				
				for( int i=7; i>=0; i-- )
					if( old_key == DIK_1+i ) face_num = i ;
			}
			
			if( face_num != -1 ) DoButtonCheckOfMenu( menu, face_num );
			break;
					}		
		*/
	case MN_LEARN_MAGIC_BOOK: {
								int old_key = SMenu[menu].key2;
								SMenu[menu].key2 = -1;
								if(g_aCurrentKeys[DIK_PGUP] & 0x80 )		SMenu[menu].key2 = 'u';	// 책장 넘기기
								if(g_aCurrentKeys[DIK_PGDN] & 0x80 )		SMenu[menu].key2 = 'd';	// 
								
								if( SMenu[menu].key2 == -1 && old_key > 0 )
								{
									if( old_key == 'u' ) DoButtonCheckOfMenu( menu, 13 );
									if( old_key == 'd' ) DoButtonCheckOfMenu( menu, 14 );
								}
								break;
							  }
	case MN_QUICK_MAGIC_SCROLL: {
								int old_key = SMenu[menu].key2;
								SMenu[menu].key2 = -1;
								if(g_aCurrentKeys[DIK_PGUP] & 0x80 )		SMenu[menu].key2 = 'u';	// 마법스크롤 단축키
								if(g_aCurrentKeys[DIK_PGDN] & 0x80 )		SMenu[menu].key2 = 'd';	// 
								
								if( SMenu[menu].key2 == -1 && old_key > 0 )
								{
									if( old_key == 'u' ) DoButtonCheckOfMenu( menu, 0 );
									if( old_key == 'd' ) DoButtonCheckOfMenu( menu, 1 );
								}
								break;
								}
	case MN_MEM_BOOK : {
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		if(g_aCurrentKeys[DIK_PGUP] & 0x80 )		SMenu[menu].key2 = 'u';	// 책장 넘기기
		if(g_aCurrentKeys[DIK_PGDN] & 0x80 )		SMenu[menu].key2 = 'd';	// 
		if(g_aCurrentKeys[DIK_RETURN] & 0x80 )		SMenu[menu].key2 = 'y';	
		
		if( SMenu[menu].key2 == -1 && old_key > 0 )
		{									
			if( old_key == 'u' ) DoButtonCheckOfMenu( menu, 0 );
			if( old_key == 'd' ) DoButtonCheckOfMenu( menu, 1 );
			if( old_key == 'y' ) DoButtonCheckOfMenu( menu, 16 );
		}
		break;
					   }
	case MN_ITEM :{
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		if(g_aCurrentKeys[DIK_PGUP] & 0x80 )		SMenu[menu].key2 = 'u';	
		if(g_aCurrentKeys[DIK_PGDN] & 0x80 )		SMenu[menu].key2 = 'd';	
		
		if( SMenu[menu].key2 == -1 && old_key > 0 )
		{									
			if( old_key == 'u' ) DoButtonCheckOfMenu( menu, 6 );
			if( old_key == 'd' ) DoButtonCheckOfMenu( menu, 5 );
		}
		break;
				  }
	case MN_SYSTEM : {
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )	SMenu[menu].key2 = 'x';	
		if(g_aCurrentKeys[DIK_X] & 0x80 )		SMenu[menu].key2 = 'x';	
		
		if( SMenu[menu].key2 == -1 && old_key > 0 )
		{									
			if( old_key == 'x' ) DoButtonCheckOfMenu( menu, 0 );
		}
		break;
					 }
	case MN_SKILLGUILD_BUY :{
								int old_key = SMenu[menu].key2;
								SMenu[menu].key2 = -1;
								if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )	SMenu[menu].key2 = 'x';	
								if(g_aCurrentKeys[DIK_RETURN] & 0x80 )	SMenu[menu].key2 = 'y';	
								
								if( SMenu[menu].key2 == -1 && old_key > 0 )
								{									
									if( old_key == 'x' ) DoButtonCheckOfMenu( menu, 2 );
									if( old_key == 'y' ) DoButtonCheckOfMenu( menu, 1 );
								}
								break;
							}
	case MN_SKILLGUILD_SELL :{
								int old_key = SMenu[menu].key2;
								SMenu[menu].key2 = -1;
								if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )	SMenu[menu].key2 = 'x';	
								if(g_aCurrentKeys[DIK_RETURN] & 0x80 )	SMenu[menu].key2 = 'y';	
								
								if( SMenu[menu].key2 == -1 && old_key > 0 )
								{									
									if( old_key == 'x' ) DoButtonCheckOfMenu( menu, 2 );
									if( old_key == 'y' ) DoButtonCheckOfMenu( menu, 1 );
								}
								break;
							 }
	case MN_MAIL_MENU : {
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )	SMenu[menu].key2 = 'x';	
		if(g_aCurrentKeys[DIK_RETURN] & 0x80 )	SMenu[menu].key2 = 'y';	
		
		if( SMenu[menu].key2 == -1 && old_key > 0 )
		{									
			if( old_key == 'x' ) DoButtonCheckOfMenu( menu, 3 );
			if( old_key == 'y' ) DoButtonCheckOfMenu( menu, 0 );
		}
		break;
						}
	case MN_OK_MESSAGEBOX :{
								int old_key = SMenu[menu].key2;
								SMenu[menu].key2 = -1;
								if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )	SMenu[menu].key2 = 'x';	
								if(g_aCurrentKeys[DIK_RETURN] & 0x80 )	SMenu[menu].key2 = 'y';	
								
								if( SMenu[menu].key2 == -1 && old_key > 0 )
								{									
									if( old_key == 'x' ) DoButtonCheckOfMenu( menu, 2 );
									if( old_key == 'y' ) DoButtonCheckOfMenu( menu, 2 );
								}
								break;
						   }
	case MN_OKCANCEL_MESSAGEBOX:{
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )	SMenu[menu].key2 = 'x';	
		if(g_aCurrentKeys[DIK_RETURN] & 0x80 )	SMenu[menu].key2 = 'y';	
		
		if( SMenu[menu].key2 == -1 && old_key > 0 )
		{									
			if( old_key == 'x' ) DoButtonCheckOfMenu( menu, 2 );
			if( old_key == 'y' ) DoButtonCheckOfMenu( menu, 3 );
		}
		break;
								}
	case MN_DEFAULT_MESSAGEBOX : {
		int old_key = SMenu[menu].key2;
		SMenu[menu].key2 = -1;
		if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )	SMenu[menu].key2 = 'x';	
		if(g_aCurrentKeys[DIK_RETURN] & 0x80 )	SMenu[menu].key2 = 'x';	
		
		if( SMenu[menu].key2 == -1 && old_key > 0 )
		{									
			if( old_key == 'x' ) DoButtonCheckOfMenu( menu, 0 );
		}
		break;
								 }
	default : break;
	}
	
	return 1;
}

int QuickKeyFirst( int menu )
{
	switch( menu )
	{
	case MN_SYSTEM_LOGOUT:{
		int old_key = SMenu[menu].key;
		SMenu[menu].key = -1;
		if(g_aCurrentKeys[DIK_RETURN] & 0x80 )	SMenu[menu].key = 'y';	
		if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 )	SMenu[menu].key = 'n';	
		
		if( SMenu[menu].key == -1 && old_key > 0 )
		{
			if( old_key == 'y' ) DoButtonCheckOfMenu( menu, 0 );
			if( old_key == 'n' ) DoButtonCheckOfMenu( menu, 1 );
		}
		break;
						  }
	}
	
	return 1;
}

Spr *GetSprOfMenu( int type, int nImageNumber )
{
	Spr *s = NULL;
	switch( type )
	{
	case GOD_MENU : s = &god_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/sys_god.dat", &god_spr_buf, god_spr);			// 신전메뉴 이미지
		break;
	case TIME_MENU :s = &time_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/sys_time.dat", &time_spr_buf, time_spr);			// 시간 표시 메뉴 이미지
		break;
	case MEM_BOOK:	s = &membook_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/sys_membook.dat", &membook_spr_buf, membook_spr);			// 메모라이즈
		break;
	case MAGIC_BOOK_MENU :	s = &magicbook_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/sys_magic.dat", &magicbook_spr_buf, magicbook_spr);			// 마법배우기
								break;
	case MAIL_MENU : s = &mail_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/sys_mail.dat", &mail_spr_buf, mail_spr);
		break;
	case START_ETC: s = &start_etc_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/start_etc.dat", &start_etc_buf, start_etc_spr);
		break;
	case START_ETC2: s = &start_etc2_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/start_etc2.dat", &start_etc2_buf, start_etc2_spr);
		break;
	case RESET_JOB_ETC: s = &job_etc_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/skill_etc.dat", &job_etc_buf, job_etc_spr);
		break;
	case START_MAP: s = &start_map_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/start_map.dat", &start_map_buf, start_map_spr);
		break;
	case CHAT_MENU: s = &chat_menu_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/sys_chat.dat", &chat_menu_buf, chat_menu_spr);
		break;
	case MAIN_ETC:	s = &main_etc_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/main_etc.dat", &main_etc_buf, main_etc_spr);
		break;
	case MAIN_ETC3:	s = &main_etc3_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/main_etc3.dat", &main_etc3_buf, main_etc3_spr);
		break;
	case SKILL_MENU:	s = &skill_menu_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/skill_menu.dat", &skill_menu_buf, skill_menu_spr);
		break;
	case SMALL_MAP: s = &small_map_spr[nImageNumber];
		if( !s->img) LoadSmallMapSprData( g_SmallMapNumber ); // CSD-030324 // thai2 YGI
		break;
	case SMALL_ETC:	s = &small_etc_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/small_etc.dat", &small_etc_buf, small_etc_spr);
		break;
	case PUBLIC_BBS:	s = &public_bbs_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/public_bbs.dat", &public_bbs_buf, public_bbs_spr );			
		break;
	case HUNTER_MENU:	s = &hunter_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/hunter.dat", &hunter_buf, hunter_spr );					// 현상범 메뉴
		break;
	case QUEST_MENU:	s = &questmenu_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/quest_menu.dat", &questmenu_buf, questmenu_spr);			// 퀘스트 메뉴
		break;
	case CHARACTER_COLOR_CHANGE: s = &chcolorcha_spr[nImageNumber];
		if( !s->img ) LoadOpenningSpr( "./data/chcolorchauj.dat",&chcolorcha_buf, chcolorcha_spr);
		break;
	case ITEM_MAKE_IMAGE: s = &item_make_image_spr[nImageNumber];
		if( !s->img ) LoadOpenningSpr( "./data/itemmakeimageuj.dat",&item_make_image_buf, item_make_image_spr);
		break;
	case NATION_BBS_MENU:	s = &nation_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/nation.dat", &nation_buf, nation_spr );
								break;
	case START_FIGHT_MAP:	s = &start_fight_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/fight_start.dat", &start_fight_buf, start_fight_spr );
								break;
	case STOP_FIGHT_MAP	:	s = &stop_fight_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/fight_stop.dat", &stop_fight_buf, stop_fight_spr );
								break;
	case FIGHT_MAP_TIME	:	s = &fightmap_time_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/fight_time.dat", &fightmap_time_buf, fightmap_time_spr );
								break;
	case FIGHT_MAP_WIN	:	s = &fightmap_win_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/fight_win.dat", &fightmap_win_buf, fightmap_win_spr );
								break;
	case FIGHT_MAP_LOSE :	s = &fightmap_lose_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/fight_lose.dat", &fightmap_lose_buf, fightmap_lose_spr );
								break;
	case VOTE_MENU:		s = &vote_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/vote.dat", &vote_buf, vote_spr );					// 투표
		break;
	case GUILD_MARK:	{
								s = &guild_mark_spr[nImageNumber];
								break;
						}
	case GUILD_ADMIN:	s = &guild_admin_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/guild00.dat", &guild_admin_buf, guild_admin_spr );
		break;
	case GUILD_LIST:	s = &guild_skill_list_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/guild01.dat", &guild_skill_list_buf, guild_skill_list_spr);
		break;
	case GUILD_ADMIN_ETC:	s = &guild_admin_etc_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/guild_etc.dat", &guild_admin_etc_buf, guild_admin_etc_spr);
								break;
	case HELP_PCX:	s = &help_pcx_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/HELP.dat", &help_pcx_buf, help_pcx_spr);
		break;
	case BANK_PCX:	s = &bank_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/bank.dat", &bank_buf, bank_spr);
		break;
	case SALVATION_PCX:	s = &salvation_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/salvation.dat", &salvation_buf, salvation_spr);
		break;
	case SALVATION_PCX_ANI: s = &salvation_ani_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/salvation_ani.dat", &salvation_ani_buf, salvation_ani_spr);
								break;
	case TEAM_BATTLE: s =	&team_battle_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/team_battle.dat", &team_battle_buf, team_battle_spr);
								break;
	case SELECT_SERVER: s = &select_server_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/select_server.dat", &select_server_buf, select_server_spr);
		break;
	case CHANGE_MONEY:	s = &cahnge_money_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/change_money.dat", &cahnge_money_buf, cahnge_money_spr);
		break;
	case GUILD_WAIT:	s = &guild_wait_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/g_wait.dat", &guild_wait_buf, guild_wait_spr);
		break;
	case MERCHANT_IMG :	s = &merchant_spr[nImageNumber]; 
		if( !s->img ) LoadOpenningSpr( "./data/merchant.dat", &merchant_buf, merchant_spr);
		break;
	case GUILD_MENU_IMG :	s = &guild_menu_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/guild_menu.dat", &guild_menu_buf, guild_menu_spr);
								break;
								
	case NATION_BATTLE_BBS_IMG://010730 lsw 
								s = &nation_battle_bbs_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/nation_battle_bbs.dat", &nation_battle_bbs_buf, nation_battle_bbs_spr);
								break;
								
	case NATION_INFO_IMG://010730 lsw 
								s = &nation_info_spr[nImageNumber]; 
								if( !s->img ) LoadOpenningSpr( "./data/NaInfo.dat", &nation_info_buf, nation_info_spr);
								break;
  		case NATION_VOTE_IMG://010730 lsw 
			s = &nation_vote_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaVote.dat", &nation_vote_buf, nation_vote_spr);
			break;
		case NATION_BUDAE_IMG://010730 lsw 
			s = &nation_budae_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaBudae.dat", &nation_budae_buf, nation_budae_spr);
			break;
		case NATION_NOW_IMG://010730 lsw 
			s = &nation_now_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaNow.dat", &nation_now_buf, nation_now_spr);
			break;
		case NATION_LADDER_IMG://010730 lsw 
			s = &nation_ladder_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaLadder.dat", &nation_ladder_buf, nation_ladder_spr);
			break;
		case NATION_JOIN_IMG://010730 lsw 
			s = &nation_join_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/Najoin.dat", &nation_join_buf, nation_join_spr);
			break;
			
		case NATION_GOTO_WAR_MAP_IMG:	
			s = &nation_goto_war_map_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/Nagoto_war_map.dat", &nation_goto_war_map_buf, nation_goto_war_map_spr);
			break;
			
		case NATION_GUEST_IMG		:
			s = &nation_guest_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaGuest.dat", &nation_guest_buf, nation_guest_spr);
			break;
			
		case NATION_MSGBOX_IMG:	
			s = &nation_msgbox_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaMsgbox.dat", &nation_msgbox_buf, nation_msgbox_spr);
			break;
		case NATION_DONEBOX_IMG:	
			s = &nation_donebox_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaDonebox.dat", &nation_donebox_buf, nation_donebox_spr);
			break;
		case NATION_TODAY_NOTICE_IMG:	
			s = &nation_today_notice_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaTodayNotice.dat", &nation_today_notice_buf, nation_today_notice_spr);
			break;
		case NATION_WAR_RESULT_IMG:	//010910 lsw
			s = &nation_war_result_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaWarResult.dat", &nation_war_result_buf, nation_war_result_spr);
			break;
		case NATION_WAR_START_IMG:	//010925 lsw
			s = &nation_war_start_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NaWarstart.dat", &nation_war_start_buf, nation_war_start_spr);
			break;
		case GM_2ND_SKILL_IMG://010708 lsw 
			s = &gm_2nd_skill_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/gm_2nd_skill.dat", &gm_2nd_skill_buf, gm_2nd_skill_spr);
			break;
			//010928 lsw
		case NEW_CHAT_IMG :	s = &new_chat_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/NewChat.dat", &new_chat_buf, new_chat_spr);
			break;
			
		case DUAL_IMG :	s = &dual_img_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/dual_img.dat", &dual_img_buf, dual_img_spr);
			break;
		case TUTORIAL_DISPLAY_IMG:
			s = &tutorial_display_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/TutorialMenu.dat", &tutorial_display_buf, tutorial_display_spr);
			break;
			//011014 lsw >
		case TUTORIAL_DISPLAY_TMP_IMG:
			s = &tutorial_display_tmp_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/TutorialMenutmp.dat", &tutorial_display_tmp_buf, tutorial_display_tmp_spr);
			break;
			//011014 lsw <
			
			//011013 lsw >
		case ITEM_MAKE_RARE_SELECT_IMG:
			s = &item_make_rare_select_spr[nImageNumber];
			if( !s->img ) LoadOpenningSpr( "./data/ItemMakeRare.dat", &item_make_rare_select_buf, item_make_rare_select_spr);
			break;
			//011013 lsw <
		case RUNE_IMG : 
			s = &rune_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/rune.dat", &rune_buf, rune_spr);
			break;
		case RUNE_EFFECT1 : 
			s = &rune_effect_spr1[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/rune01.dat", &rune_effect_buf1, rune_effect_spr1);
			break;
		case RUNE_EFFECT2 : 
			s = &rune_effect_spr2[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/rune02.dat", &rune_effect_buf2, rune_effect_spr2);
			break;
		case RUNE_EFFECT3 : 
			s = &rune_effect_spr3[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/rune03.dat", &rune_effect_buf3, rune_effect_spr3);
			break;
		case RUNE_EFFECT4 : 
			s = &rune_effect_spr4[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/rune04.dat", &rune_effect_buf4, rune_effect_spr4);
			break;
		case RUNE_EFFECT_DEFAULT: 
			s = &rune_effect_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/rune_default.dat", &rune_effect_buf, rune_effect_spr);
			break;
		case SKIN_MENU: 
			s = &skin_menu_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/skin_menu.dat", &skin_menu_buf, skin_menu_spr);
			break;
		case GAMBLE_IMG:
			s = &gamble_menu_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/gamble_img.dat", &gamble_menu_buf, gamble_menu_spr);
			break;
		case GAMBLE_EFFECT:
			s = &gamble_effect_spr[nImageNumber]; 
			if( !s->img ) LoadOpenningSpr( "./data/gamble_effect.dat", &gamble_effect_buf, gamble_effect_spr);
			break;
			
		case IIT_EXP_GAUGE :
			{
				s = &exp_gauge_spr[nImageNumber]; 
				if( !s->img ) LoadOpenningSpr( "./data/exp_gauge.dat", &exp_gauge_buf, exp_gauge_spr);
				break;
			}
		case DUAL_MARK_IMG:	s = &dual_mark_spr[nImageNumber]; 
							{
								if( !s->img ) LoadOpenningSpr( "./data/dual_mark.dat", &dual_mark_buf, dual_mark_spr);
								break;
							}
		case ABILITY_REDISTRIBUTION_IMG :		// 011201 LTS		// LTS MENU
			{
				s=&Ability_Redistribution_Spr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/ARedist.dat",&Ability_Redistribution_Buf,Ability_Redistribution_Spr);
				break;
			}
		case LOCALWAR_JOIN_IMG :				// 011201 LTS
			{
				s=&LocalWar_Join_Spr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/LocalJoin.dat",&LocalWar_Join_Buf,LocalWar_Join_Spr);
				break;
			}
		case IIT_GUILD_MEMBER_LIST:
			{
				s = &guild_member_list_spr[nImageNumber]; 
				if( !s->img ) LoadOpenningSpr( "./data/guild_member.dat", &guild_member_list_buf, guild_member_list_spr);
				break;
			}
		case IIT_FRIEND:
			{
				s = &friend_menu_spr[nImageNumber]; 
				if( !s->img ) LoadOpenningSpr( "./data/friend_menu.dat", &friend_menu_buf, friend_menu_spr);
				break;
			}
		case IIT_POTAL:
			{
				s = &potal_menu_spr[nImageNumber]; 
				if( !s->img ) LoadOpenningSpr( "./data/potal_menu.dat", &potal_menu_buf, potal_menu_spr);
				break;
			}
		case IIT_PARTY_BUTTON:
			{
				s = &party_button_spr[nImageNumber]; 
				if( !s->img ) LoadOpenningSpr( "./data/party_button.dat", &party_button_buf, party_button_spr);
				break;
			}
		case IIT_GUILD_ITEM:
			{
				s = &guild_item_spr[nImageNumber]; 
				if( !s->img ) LoadOpenningSpr( "./data/guild_item.dat", &guild_item_buf, guild_item_spr);
				break;
			}
		case IIT_GUILD_CHANGE_INFO:
			{
				s = &guild_change_info_spr[nImageNumber]; 
				if( !s->img ) LoadOpenningSpr( "./data/guild_change_info.dat", &guild_change_info_buf, guild_change_info_spr);
				break;
			}
			
		case LOCALWAR_OK_IMG :	// LTS LOCALWAR BUG			// LOCALWAR MENU
			{
				s=&LocalWar_OK_Spr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/LWDoneBox.dat",&LocalWar_OK_Buf,LocalWar_OK_Spr);
				break;
			}
		case EVENT_LOCALWAR_OK_IMG :	// 020115 LTS
			{
				s=&EventLocalWar_OK_Spr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/NaMsgBox.dat",&EventLocalWar_OK_Buf,EventLocalWar_OK_Spr);
				break;
			}
		case EVENT_LOCALWAR_MOVE_IMG :	// 020115 LTS
			{
				s=&EventLocalWar_Move_Spr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/NaMsgBox.dat",&EventLocalWar_Move_Buf,EventLocalWar_Move_Spr);
				break;
			}
		case WAR_START_IMG :
			{
				s=&WarStartSpr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/WarStart.dat",&WarStartBuf,WarStartSpr);
				break;
			}
		case WAR_MOVE_IMG :
			{
				s=&WarMoveSpr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/WarMove.dat",&WarMoveBuf,WarMoveSpr);
				break;
			}
		case WAR_STATUS_IMG :
			{
				s=&WarStatusSpr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/WarStatus.dat",&WarStatusBuf,WarStatusSpr);
				break;
			}
		//<! BBD 040311
		case MONSTER_WAR_IMG :
			{
				s=&MonsterWarInfoSpr[nImageNumber];
				if (!s->img) LoadOpenningSpr("./data/MWarInfo.dat",&MonsterWarInfoBuf,MonsterWarInfoSpr);
				break;
			}
		//> BBD 040311
		default :
			{
				if( type >= 10000 )
				{
					s = g_MenuImageMgr->GetSpr( type-10000, nImageNumber );
				}
				break;
			}
		case DEFAULT_MENU : 
			{
				
				if( nImageNumber >= 10000 ) 
				{
					int num = nImageNumber-10000;
					s = &spr2[num];
					if( !s->img ) LoadSmallMenuImage2( num );
					break;
				}
				else 
				{
					s = &spr[nImageNumber];
					if( !s->img ) LoadSmallMenuImage( nImageNumber );
					break;
				}
				break;
			}
	}
	
	if( !s || !s->img ) 
	{
		CheckImage( type, nImageNumber );
	}
	
	return s;
}

int GetHourToRate( int h )
{
	int hour_to_rate[] = {6,1,2,3,4,5};
	return hour_to_rate[h/4];
}
void SendChatDataGlobalFightMap( char *lpStr )
{
	int Len = strlen( lpStr );
	if( g_FightMapStart )
	{
		t_packet packet;
		packet.h.header.type = CMD_CHAT_DATA;
		{
			packet.u.client_chat_data.type = CHATTYPE_COLOSSUS_GLOBAL; // 전체에게 보낸다. 
			packet.u.client_chat_data.length = Len;
			memcpy( packet.u.client_chat_data.text, lpStr, Len );
		}
		packet.h.header.size = sizeof( t_client_chat_data ) - TEXT_LENGTH + Len;
		QueuePacket( &packet, 1 );
	}
}


void CloseAllMenu()		// 스페이스바 친 효과 가져오기 ( 모든 메뉴 다 닫고.... )	/OFF
{
	ServerConnectMenuClose();
	MainMenuClose( );
	SmallMenuClose();
	SetChatMode(CM_COMMAND,true);//021001 lsw //인터페이스 뜨면 무조건 채팅창 닫는다
	EWndMgr.ClearAllTxt();//021001 lsw
}


void SetVolumeBySleep( int volume )					//잠자기 상태일때의 볼륨 조절
{
	for ( int i = 0; i < MAX_SOUND; i++ )
	{
		if( i == SN_SLEEP || i== SN_REST ) continue;
		if ( g_SoundInfo.lpDirectSoundBuffer[ i ] )
		{
			IDirectSoundBuffer_SetVolume( g_SoundInfo.lpDirectSoundBuffer[ i ], volume * 100 + DSBVOLUME_MIN );
		}
	}
}

void PrintQuickKey( char *text)
{
	return;
	Hcolor( 0, 255,0 );
	SetHangulAlign( TA_CENTER );
	Hprint2( SMenu[MN_MAININTERFACE].x+379, SMenu[MN_MAININTERFACE].y+102, g_DestBackBuf, text );
}

int GetMoveMenuDefineByMenuNumber( int menu, int is_open )
{
	switch( menu )
	{
	case MN_STATUS :	return is_open?MM_STATUS_OPEN:MM_STATUS_CLOSE;
	case MN_ABILITY :	return is_open?0:MM_ABILITY_CLOSE;
	case MN_ARIGEMENT:	return is_open?0:MM_ARIGEMENT_CLOSE;
	case MN_ITEM :		return is_open?MM_ITEM_OPEN:MM_ITEM_CLOSE;
	case MN_PARTY :		return is_open?MM_PARTY_OPEN:MM_PARTY_CLOSE;
	case MN_TACTICS :	return is_open?0:MM_TACTICS_CLOSE;
	}
	
	return 0;
}

void CheckAutoDivideMoney( ItemAttr *item )
{
	if( !item ) return;
	if( GetItemAttr( item->item_no, ITEM_KIND ) != IK_MONEY ) return;
	if( item->attr[IATTR_DURATION] > MAX_MONEY ) 
	{
		ItemAttr *new_item = SearchEmpty();
		if( !new_item )
		{
			item->attr[IATTR_DURATION] = MAX_MONEY;
		}
		else
		{
			memcpy(new_item , item , sizeof( ItemAttr ) );
			new_item->attr[IATTR_DURATION] = item->attr[IATTR_DURATION] - MAX_MONEY;
			item->attr[IATTR_DURATION] = MAX_MONEY;
			if( new_item->attr[IATTR_DURATION] > MAX_MONEY ) CheckAutoDivideMoney( new_item );
		}
	}
}

char *FindSpaceWord( char *szText, int len )	// 스트링에서 마지막으로 나오는 빈칸을 찾아 그 주소값을 넘겨 준다.
{
	while( *szText == ' ' ) 
	{
		szText++;
		len -- ;
	}
	
	char  *retStr = szText+len-1;		// 맨마지막을 가리킴
	
	int i;
	for( i=0; i<len; i++ )		// 빈칸이 나오면 리턴해줌
	{
		if( *retStr == ' ' || *retStr == '\0' || *retStr == '.' || *retStr == NULL) return retStr;
		else retStr--;
	}
	
	// 빈칸이 없을 경우에는 한글일 경우만 조심해서 넘겨준다.
	int cEnd = 0;
	retStr = szText;
	for( i=0; i<len; i++ )
	{
		if( IsHangul( *retStr ) ) // thai2 YGI
		{	//< CSD-030324 : 한글이면
			cEnd ++;
		}	//> CSD-030324
		else 
		{
			cEnd = 0;
		}

		retStr++ ;
	}
	
	if( cEnd%2 ) return ( retStr-1 );	// 끝줄에 한글 2바이트가 반씩 걸렸다.
	else return retStr;
}

int GetDiceAbility( int DiceNumber )		// 다이스 돌리기
{
	int dice_count, dice_max;
	dice_count = DiceNumber / 1000;
	dice_max = DiceNumber % 1000;
	
	int ret = 0;
	for( int i=0; i<dice_count ; i++ )
	{
		ret += rand()%dice_max + 1;
	}
	return ret;
}
////////////////////////////////////////////////////////////////////
//
// 하는 일	: 문자열을 일정한 길이로 잘라준다.
// 특징		: 한글일 경우 글자가 깨지는 것을 막아준다.	
//
////////////////////////////////////////////////////////////////////
void CutHangul( char *text, int max )
{	//< CSD-030324
	int len = strlen( text );
	if( len <= max ) return;
	
	text[max] = NULL;
	
	int start = 0;
	int i;
	for( i=max; i>=0; i-- )
	{
		if( IsHangul( text[i] ) ) start = i;			// 한글이 시작되는 처음을 찾아낸다.	// thai2 YGI
		else if( start ) break;					// 전에 한글이 있는 곳이 있는데 한글 아닌데가 나오면...
	}
	
	if( start )
	{
		int flag = 0;
		for( i=start;i<max ;i++ )
			if( IsHangul( text[i] ) ) flag++;		// count 한후	// thai2 YGI
			
		if( flag % 2) text[max-1] = NULL;		// 한글이 짤려 있으면
	}
}	//> CSD-030324

void AddWaveOfEndString( char *text )		// 물결을 붙여준다.		문자열은 넉넉이 잡혀 있어야 한다.
{
	int len = strlen( text );
	strcpy( &text[len], "~" );
}

int isSmallMenuOn()
{
	static int menu[] = {				// 혹시 활성화가 되면 항상 떠 있어야 하는 메뉴들
		MN_MAININTERFACE,
			MN_CHATTING,
			
			MN_PARTY_FACE1,
			MN_PARTY_FACE2,
			MN_PARTY_FACE3,
			MN_PARTY_FACE4,
			MN_PARTY_FACE5,
			MN_PARTY_FACE6,
			MN_AWAKE,
			MN_REVIVAL,
			MN_LEVELUP,
			MN_COMBAT_SKILL_DOING,
			MN_GOD_MEETING_MAIN,
	};
	static int max = sizeof( menu ) / sizeof( int );
	int j;

	for( int i=0; i<MAX_SMALL; i++ )
	{
		if( SMenu[smallmenu[i]].bActive )
		{
			for( j=0; j<max; j++ )
			{
				if( menu[j] == smallmenu[i] ) break;
			}
			if( j == max ) return smallmenu[i];
		}
	}
	
	return -1;
}


int ExistBox( DWORD box_id )
{
	if( !box_id ) return 0;
	
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				if( InvItemAttr[a][b][c].attr[IATTR_ID] == box_id ) return 1;
			}
			
			return 0;
}

void SetCheckWareAbleWeapon( int inv[3][3][8] )
{
	memset( inv, 0, sizeof( int )*3*3*8 );
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				ItemAttr target = InvItemAttr[a][b][c];
				if( target.item_no )
				{
					if( !GetItemAttr(target.item_no, EQUIPABLE) ) inv[a][b][c] = 1;
				}
			}
}

bool CanActiveMessageLine()			// 메시지 창을 띄워도 되냐?
{
	if( !SMenu[MN_MAININTERFACE].bActive ) 
	{
		return false;	// 본 게임에 들어왔을때만...
	}
	if( !is_mainmenu_active	) 
	{
		if( SMenu[MN_GOD_MEETING_MAIN].bActive )			// 몇몇의 메뉴는 떠야 한다.
		{
		}
		else return false;									// 스몰메뉴가 떠 있을 경우엔 메시지 창을 띄우지 않는다.	
	}
	return true;
}

void GetSupperItem( int god )
{
	//int item_kind[] = { 2001, 2006, 3024, 3017, 3008, 3012, 3016, 3020, 3019, 3021 };
	int item_kind[] = { 3007, };
	int max = sizeof( item_kind ) / sizeof( int );
	memset( SupperItemAttr, 0, sizeof( ItemAttr )*3*5 );
#ifdef _DEBUG // finito 060507	
	if( SysInfo.notconectserver ) SCharacterData.much_supper = 10;
#endif
	int much_max = SCharacterData.much_supper;
	if( much_max > 15 ) much_max = 15;
	
	for( int a=0; a< much_max; a++ )
		SupperItemAttr[a/5][a%5] = GenerateItem( item_kind[rand()%max] );
}


////////////////////////////////////////////////////////////////////////////////////////
//	스타트 메뉴에서 사용한 메모리 프리 
////////////////////////////////////////////////////////////////////////////////////////
void StartMenuFree()	// 스타트 메뉴에서 메모리 사용을 프리 시켜준다.
{
	FreeMenuData( 0 );
	
	FreeChImage();	// 선택 캐릭터 에니메이션 그림
	FreeOpenningSpr( &open_spr_buf );	// 처음에 뜨는 불 에니메이션 그림
	
	FreeOpenningSpr( &facebuf_man_m );
	FreeOpenningSpr( &facebuf_woman_m );
	// 0906 kkh 추가 
	FreeOpenningSpr( &facebuf_man_m_dash );
	FreeOpenningSpr( &facebuf_woman_m_dash );
	
	int i;
	for( i = 0 ; i< FACE_MAX; i++ ) 
	{
		spr_face_man_m[i].img	= NULL;
		spr_face_woman_m[i].img = NULL;
	}
	for( i=0; i<ADD_CHA_PIC; i++ )
	{
		spr_face_man_m_dash[i].img		= NULL;
		spr_face_woman_m_dash[i].img	= NULL;
	}
	
	
	//	FreeSpriteOnlyOne( &credit );//020515 lsw
	//	FreeSpriteOnlyOne( &logo2 );//020515 lsw
	//	FreeSpriteOnlyOne( &logo1 );//020515 lsw
	
	FreeSmallMenuImage2( );		// sysmenu2.dat 에서 로드해온 데이터 프리...
	
	if( start_etc_buf )
	{
		FreeOpenningSpr( &start_etc_buf );
		for( int i = 0 ; i< START_ETC_IMG_MAX; i++ ) start_etc_spr[i].img = NULL;
	}
	if( start_etc2_buf )
	{
		FreeOpenningSpr( &start_etc2_buf );
		for( int i = 0 ; i< START_ETC2_IMG_MAX; i++ ) start_etc2_spr[i].img = NULL;
	}
	if( start_map_buf )
	{
		FreeOpenningSpr( &start_map_buf );
		for( int i = 0 ; i< START_MAP_IMG_MAX; i++ ) start_map_spr[i].img = NULL;
	}
	if( job_etc_buf )
	{
		FreeOpenningSpr( &job_etc_buf);
		for( int i = 0 ; i< JOB_ETC_IMG_MAX; i++ ) job_etc_spr[i].img = NULL;
	}
	if( select_server_buf )
	{
		FreeOpenningSpr( &select_server_buf);
		for( int i = 0 ; i< SELECT_SERVER_IMG_MAX; i++ ) select_server_spr[i].img = NULL;
	}
	if( guild_mark_buf )	// 새로 서버 선택일 경우를 대비해서
	{
		FreeOpenningSpr( &guild_mark_buf);
		for( int i = 0 ; i<GUILD_MARK_MAX; i++ ) guild_mark_spr[i].img = NULL;
	}
}


void GameEndFree()
{
	for( int i=0; i<8; i++ )
		FreeMenuData( i );
	
	if( g_StartMenuOn )
	{
		StartMenuFree();
		// 시작 메뉴
	}
	else 
	{
		nation_bbs[0] = NULL;
		MainMenuClose();
		SmallMenuClose();
		MainMenuFree();
	
		ExchangeMgr.Clear();//021030 lsw
		ExchangeMgr.ClearItemData();//021030 lsw
		
		if( g_Guild_Introduction ) 
		{
			delete g_Guild_Introduction;
			g_Guild_Introduction = NULL;
		}
		g_Menu_Variable.clear();
		g_Menu_Variable.clear2();
		SaveMemorize();
	}
	
	// 020320 YGI
	RemoveExchangeItemStatus();
	
#ifdef LIGHT_VERSION_
	if( g_pLightVersion ) 
	{
		delete g_pLightVersion;
		g_pLightVersion = 0;
	}
#endif
}

// thai2 YGI
int ViewSmallMap2( int x, int y )
{	//< CSD-030324
	static int mapnumber = g_SmallMapNumber;
	static int real_mapnumber = g_SmallMapNumber;
	
	int ret = 1;
	if( !small_map_spr[0].img )	
	{
		ret = LoadSmallMapSprData( g_SmallMapNumber );
		real_mapnumber = g_SmallMapNumber;
	}
	else if( mapnumber != g_SmallMapNumber ) 
	{
		ret = LoadSmallMapSprData( g_SmallMapNumber );
		real_mapnumber = g_SmallMapNumber;
	}
	
	if( !ret )
	{
		if( g_SmallMapNumber != MapNumber )
		{
			if( !LoadSmallMapSprData( MapNumber ) ) return 0;
			real_mapnumber = MapNumber;
		}
		else
			return 0;		// 실질적으로 데이타가 없으면..
	}
	mapnumber = g_SmallMapNumber;
	
	Spr *s = &small_map_spr[0];
	PutCompressedImageFX( x+s->ox, y+s->oy, s ,22,1);
	{
		//int max = (g_Map.file.wWidth > g_Map.file.wHeight)?g_Map.file.wWidth:g_Map.file.wHeight;
		const int mx = Hero->x/TILE_SIZE	*	(s->xl-50)	/	(g_Map.file.wWidth)	+25;	//020502 lsw	//맵파일 보정 때문에 들어간것
		const int my = Hero->y/TILE_SIZE	*	(s->yl-50)	/	(g_Map.file.wHeight)	+25;	//020502 lsw	//테두리 보정
		
		static int count = 0;
		static bool flag = true;
		
		if( flag ) 
		{
			count+=30;
			if( count >= 255 ) 
			{
				flag = false;
				count = 255;
			}
		}
		else 
		{
			count -=30 ;
			if( count <= 0 ) 
			{
				flag = true;
				count = 0;
			}
		}
		int iClassImageNo = 0;
		switch(SCharacterData.nCharacterData[CLASS])
		{
		case WARRIOR:
			iClassImageNo = 33;
			break;
		case	THIEF:
			iClassImageNo = 34;
			break;
		case	ARCHER:
			iClassImageNo = 35;
			break;
		case	WIZARD:
			iClassImageNo = 36;
			break;
		case	PRIEST:	
			iClassImageNo = 37;
			break;
		default:
			iClassImageNo = 33;
			break;
		}
		
		Spr *chspr = 	GetSprOfMenu( NATION_NOW_IMG,iClassImageNo);
		PutCompressedImage( mx + chspr->ox-(4), my + chspr->oy-chspr->yl, chspr );
		
		if( quest_dot.map_num == MapNumber && quest_dot.on >0 )
		{
			int mx = quest_dot.x;
			int my = quest_dot.y;
			
			for( int i=-2; i<3; i++ )
				for( int j=-2; j<3; j++ )
				{
					if( abs(i)+abs(j) > 2 ) continue;
					PutDot( mx+i, my+j, RGB16(count, count, 255) );	
				}
				
				quest_dot--;
		}
		
		
		if( scan_target_view_time > 0 )
		{
			for( int i=0 ; i<scan_target_count; i++ )
			{
				int mx = scan_target_xy[i].x;
				int my = scan_target_xy[i].y;
				
				for( int i=-2; i<3; i++ )
					for( int j=-2; j<3; j++ )
					{
						if( abs(i)+abs(j) > 2 ) continue;
						PutDot( mx+i, my+j, RGB16(255, count, count) );	
					}
			}
			scan_target_view_time --;
		}
	}
#ifdef _DEBUG // finito 060507	
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		Hprint2( x, y, NULL, "%d:%d", real_mapnumber, g_SmallMapNumber );
	}
#endif
	return 1;
}	//> CSD-030324

void SetSmallMapMenu( )
{
	for( int i=1; i<MAX_FIELD; i++ )		// map field 초기화
	{
		if( !SMenu[MN_SMALL_MAP].nField[i].nType ) break;
		memset( &SMenu[MN_SMALL_MAP].nField[i], 0, sizeof( SFIELD ) );
	}
	
	char filename[FILENAME_MAX];
	sprintf( filename, "./data/map%02d.pos", MapNumber );
	FILE *fp;
	fp = Fopen( filename, "rt" );
	if( fp )
	{
		char str[1024];
		
		int count = 0;
		while( fgets(str, 1024, fp ) != NULL )
		{
			if( str[0] == ';' || str[0] == 0 || str[0] == '#' || str[0] == ' ' ) continue;
			
			char *temp;
			temp = strtok( str, "\t\n" );
			if( !temp ) continue;
			count++;
			strcpy( SMenu[MN_SMALL_MAP].nField[count].temp, temp );
			temp = strtok( NULL, "\t\n" );
			if( !temp && GetSysInfo( SI_GAME_MAKE_MODE ) ) JustMsg( lan->OutputMessage(7,123), filename, count );//010215 lsw
			int x = atoi( temp );
			SMenu[MN_SMALL_MAP].nField[count].x = x;
			
			temp = strtok( NULL, "\t\n" );
			if( !temp && GetSysInfo( SI_GAME_MAKE_MODE ) ) JustMsg( lan->OutputMessage(7,123), filename, count );//010215 lsw
			int y = atoi( temp );
			SMenu[MN_SMALL_MAP].nField[count].y = y;
			
			SMenu[MN_SMALL_MAP].nField[count].nType = FT_SMALL_MAP_HOUSE;
			SetRect(SMenu[MN_SMALL_MAP].nField[count].rCheakBox,x,y,x+25,y+25);
		}
		SMenu[MN_SMALL_MAP].nFieldCount = count+1;		
		fclose( fp );
	}
}

int GetBattlePeace()
{
	return main_interface.data[IF_BATTLE_PEACE];
}

void SetBattlePeace( const int type )
{
	if (type == PEACE_MODE)//평화모드이면
	{   //< CSD-020702
		CSDMainIconClear(); // 030428 kyo
	}   //> CSD-020702
	
	static char num2peacebattle[] = { 0, 0, 1 };
	if( num2peacebattle[main_interface.data[IF_BATTLE_PEACE]] != num2peacebattle[type] )
	{
		CallServer( CMD_PK_ON_OFF, &num2peacebattle[type], 1 );
	}
	main_interface.data[IF_BATTLE_PEACE] = type;
	CheckPeaceBattle();
}
//acer7
void CheckPeaceBattle()	// 플레그를 이용해서 메뉴에서 피스모드 배틀 모드임을 바꿔준다.
{
	int temp = main_interface.data[IF_BATTLE_PEACE]?1:0;
	if( Hero->peacests != temp ) SettingBasicAction( main_interface.data[IF_BATTLE_PEACE] );
	
	static int peace_flag = -1;
	if( peace_flag == main_interface.data[IF_BATTLE_PEACE] ) return;
	peace_flag = main_interface.data[IF_BATTLE_PEACE];
	
	if( main_interface.data[IF_BATTLE_PEACE] && !Lc_DefaultMoveType )		// 걷기 모드 에서
	{
		Lc_DefaultMoveType = 1;
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 39 ) );
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 173 ) );
	}	
}


#define REGIST_KEY  "SOFTWARE/Microsoft/Windows/CurrentVersion/App Paths/DragonRaja.exe" 
void RegistProgram()
{
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) ) return;
#endif	
	
	char full_directory[ FILENAME_MAX];		// 경로
	char default_value[ FILENAME_MAX];		// 경로에 화일명까지
	
	GetCurrentDirectory( FILENAME_MAX, full_directory );
	strcpy( default_value, full_directory );
	strcat( default_value, "/DragonRaja.exe" );
	
	HKEY hKeyResult;
	DWORD dwDisposition;
	LONG lResult = RegCreateKeyEx(	HKEY_LOCAL_MACHINE, 
		REGIST_KEY, 
		NULL,	
		"", 
		REG_OPTION_NON_VOLATILE, 
		KEY_ALL_ACCESS, 
		NULL, 
		&hKeyResult, 
		&dwDisposition );
	if( lResult != ERROR_SUCCESS )
	{
		RegCloseKey(hKeyResult );
		return;
	}
	lResult = RegSetValueEx( hKeyResult, "", 0, REG_SZ, (unsigned char *)default_value, strlen( default_value ) );
	
	if( lResult != ERROR_SUCCESS )
	{
		RegCloseKey(hKeyResult );
		return;
	}
	
	lResult = RegSetValueEx( hKeyResult, "Path", 0, REG_SZ, (unsigned char *)full_directory, strlen( full_directory ) );
	RegCloseKey(hKeyResult );
	
	//	SetupFolders();
	
	return;
}


//
//	에러 메시지 화일로 출력
//
void CheckErrorMessage( LPSTR str )
{
	char filename[] = "./MoveError.txt";
	FILE *fp = Fopen( filename, "at+" );
	if( !fp ) return;
	
	fprintf( fp, "%s\n", str );
	fclose(fp );
}


//
//	문자열을 영문자만 남기고 나머진 빈칸으로 
//
int OnlyAlphabet( char *str, int len )
{
	for( int j=0; j<len; j++ )
	{
		if( !((str[j] >= 'a' && str[j] <= 'z') || ( str[j] >= 'A' && str[j] <= 'Z' )) || str[j] == '_' )
		{
			str[j] = ' ';
		}
	}
	return 1;
}

//	숫자만 지우기
int OnlyNumber( char *str, int len )
{
	for( int j=0; j<len; j++ )
	{
		if( (str[j]<'0'||str[j]>'9') && (str[j]!='/') && (str[j]!='~') && (str[j]!='+') && (str[j]!='-') )
		{
			str[j] = ' ';
		}
	}
	return 1;
}

// thai2 YGI
int LoadSmallMapSprData( int map_number )
{	//< CSD-030324
	if( small_map_spr[0].img )
	{
		FreeOpenningSpr( &small_map_buf );
		for( int i = 0 ; i< SMALL_MAP_MAX; i++ ) small_map_spr[i].img = NULL;
	}

	char filename[FILENAME_MAX];
	sprintf( filename,  "./data/map%02d.dat", map_number );
	LoadOpenningSpr( filename, &small_map_buf, small_map_spr );
	return small_map_spr[0].img?1:0;
}	//> CSD-030324

int ExplainBackView( int &x, int &y, int count )	// 찍을 좌표를 가져간다.
{
	x = Hero->x-Mapx-spr[71].xl-20;
	y = Hero->y-Mapy-spr[71].yl*4-30;
	
	if( y >= GAME_SCREEN_YSIZE-20 ) return 0;
	
	for( int i = 0; i<count; i++ )
		FieldTypeNomalPutFx( x, y+i*24, 0,0, 71, 15, 1 );
	
	x += 4; y += 8;
	
	return 1;
}

#define HP_GAUGE_DOT_MAX	40
int PutHpGauge( int x, int y, LPCHARACTER ch )
{
	const int iMax = ch->_HpMax;
	const int iNow = ch->_Hp;
	
	if( (!iMax) || (iNow  > iMax) )
	{
		return 0;
	}
	
	const int gauge= (int)( HP_GAUGE_DOT_MAX* (float)iNow/iMax );
	
	WORD rgb = RGB16( 255, 0, 0);
	WORD rgb2 = RGB16( 0, 0, 0);
	int count = 0;
	for( ; count<gauge; count+=2 )
	{
		PutDot( x+count, y, rgb );
		PutDot( x+count, y+1, rgb );
		PutDot( x+1+count, y, rgb );
		PutDot( x+1+count, y+1, rgb );
	}
	for( ; count<HP_GAUGE_DOT_MAX; count+=2 )
	{
		PutDot( x+count, y, rgb2 );
		PutDot( x+count, y+1, rgb2 );
		PutDot( x+1+count, y, rgb2 );
		PutDot( x+1+count, y+1, rgb2 );
	}
	return 1;
}

//020626 lsw
const int HERO_MP_GAUGE_DOT_MAX = 30;
int PutHeroMpGauge( const int x, const int y)
{
	const int iMax = SCharacterData.nCharacterMAXMP;
	const int iNow = SCharacterData.nCharacterMP;
	
	if( (!iMax) || (iNow  > iMax) )
	{
		return 0;
	}
	
	const int gauge= (int)( HERO_MP_GAUGE_DOT_MAX* (float)iNow/iMax );
	
	WORD rgb = 0;
	switch(SCharacterData.nCharacterData[SPELL])
	{
	case WIZARD_SPELL:
		{
			rgb = RGB16( 50, 50, 240 );					//법사
		}break;
	case PRIEST_SPELL:
		{
			rgb = RGB16( 50, 240, 50 );					//직자
		}break;
	default:
		{
			return 0;
		}break;
	}
	
	int count;
	for( count=0; count<gauge; count+=2 )
	{
		PutDot( x+count, y, rgb );
		PutDot( x+count, y+1, rgb );
		PutDot( x+count, y+2, rgb );
		PutDot( x+1+count, y, rgb );
		PutDot( x+1+count, y+1, rgb );
		PutDot( x+1+count, y+2, rgb );
	}
	WORD rgb2 = RGB16( 0, 0, 0);					//직자
	for( ; count<HERO_MP_GAUGE_DOT_MAX; count+=2 )
	{
		PutDot( x+count, y, rgb2 );
		PutDot( x+count, y+1, rgb2 );
		PutDot( x+count, y+2, rgb2 );
		PutDot( x+1+count, y, rgb2 );
		PutDot( x+1+count, y+1, rgb2 );
		PutDot( x+1+count, y+2, rgb2 );
	}
	/*	WORD rgb3 = RGB16( 255, 255, 255);					//흰줄//020701 lsw
	PutDot( x+gauge, y, rgb3 );
	PutDot( x+gauge, y+1, rgb3 );
	PutDot( x+1+gauge, y, rgb3 );
	PutDot( x+1+gauge, y+1, rgb3 );
	*/	return 1;
}
const int HERO_HP_GAUGE_DOT_MAX = 30;
int PutHeroHpGauge( const int x, const int y)
{
	const int iMax = SCharacterData.nCharacterMAXHP;
	const int iNow = SCharacterData.nCharacterHP;
	
	if( (!iMax) || (iNow  > iMax) )
	{
		return 0;
	}
	
	const int gauge= (int)( HERO_HP_GAUGE_DOT_MAX* (float)iNow/iMax );
	
	WORD rgb = RGB16( 240, 50, 50);
	WORD rgb2 = RGB16( 0, 0, 0);
	int count = 0;
	for( ; count<gauge; count+=2 )
	{
		PutDot( x+count, y, rgb );
		PutDot( x+count, y+1, rgb );
		PutDot( x+count, y+2, rgb );
		PutDot( x+1+count, y, rgb );
		PutDot( x+1+count, y+1, rgb );
		PutDot( x+1+count, y+2, rgb );
	}
	for( ; count<HERO_HP_GAUGE_DOT_MAX; count+=2 )
	{
		PutDot( x+count, y, rgb2 );
		PutDot( x+count, y+1, rgb2 );
		PutDot( x+count, y+2, rgb2 );
		PutDot( x+1+count, y, rgb2 );
		PutDot( x+1+count, y+1, rgb2 );
		PutDot( x+1+count, y+2, rgb2 );
	}
	/*	WORD rgb3 = RGB16( 255, 255, 255);					//흰줄
	PutDot( x+gauge, y, rgb3 );
	PutDot( x+gauge, y+1, rgb3 );
	PutDot( x+1+gauge, y, rgb3 );
	PutDot( x+1+gauge, y+1, rgb3 );
	*/	return 1;
}

/////////////////////////////////////////////////////////
//	전역변수에 설명할 스킬과 매직을 등록하고 출력한다.
//	
/////////////////////////////////////////////////////////
void CheckSkillExplain( int skill_no )
{
	if( skill_no )
	{
		g_explain_skill_on = true;
		g_explain_skill = skill_no;
	}
}

void CheckMagicExplain( int magic_no )
{
	if( magic_no )
	{
		g_explain_magic_on= true;
		g_explain_magic = magic_no;
	}
}

int SkillExplainAfterMenuDisplay( int prev )		//	먼저 어떤 설명이 없었다면 설명문을 띄운다.
{
	if( prev ) 
	{
		g_explain_skill_on = false;
		g_explain_skill = 0;
		return 0;
	}
	int ret=0;
	if( g_explain_skill_on )
	{
		skill[g_explain_skill].Explain(Hero->x-Mapx+20,Hero->y-Mapy-80);
		ret = 1;
	}
	g_explain_skill_on = false;
	g_explain_skill = 0;
	
	return ret;
}

int MagicExplainAfterMenuDisplay( int prev )
{
	if( prev ) 
	{
		g_explain_magic_on  = false;
		return 0;
	}
	int ret = 0;
	if( g_explain_magic_on )
	{
		MagicExplain( g_explain_magic );
		ret =1;
	}
	g_explain_magic_on = false;
	return ret;
}

////////////////////////////////////////////
// 메뉴 설명
void ExplainMenuSet( char *temp )
{
	strcpy( g_explain_menu, temp );	
	g_explain_menu_on = true;;
}



int ExplainMenu( int prev )
{
	return 0 ;
	if( g_ExplainMenuOn == false ) return 0 ;
	
	if( prev )
	{
		g_explain_magic_on  = false;
		return 0;
	}
	
	int ret = 0;
	if( g_explain_menu_on )
	{
		int x, y;
		int line = GetLineRectTextPut(127, g_explain_menu);
		
		if( line >5 ) line --;
		if( line > 8 ) line--;
		if( line > 9 ) line--;
		if( !ExplainBackView( x, y, line ) ) return 0;
		
		Hcolor( FONT_COLOR_ORANGE );
		char *div = DivideStringByRect( 127, g_explain_menu, 0 );
		if( !div ) 
		{
#ifdef _DEBUG // finito 060507
			if( GetSysInfo( SI_GAME_MAKE_MODE ) ) JustMsg( lan->OutputMessage(7,124) );//010215 lsw
#endif	
			return 0;
		}
		*( div-1 ) = NULL;		// \n 처리를 위해서..
		
		RectTextPut( x+3, y, 127, g_explain_menu);
		
		Hcolor( FONT_COLOR_NUMBER );
		RectTextPut( x+3, y+16, 127, div);
		ret =1;
	}
	
	g_explain_menu_on = false;
	return ret;
}


///////////////////////////////////////////
// 캐릭터 중심으로 뜨는 설명
void ShowExplain()							// 각종 설명문 보여주기 
{
	int ret = 0;
	//	ret = ItemExplainAfterMenuDisplay( 0 );
	
	if( !ret )
		ret = SkillExplainAfterMenuDisplay( 0 );
	else SkillExplainAfterMenuDisplay( 1 );
	
	if( !ret )
		ret = MagicExplainAfterMenuDisplay( 0 );
	else MagicExplainAfterMenuDisplay( 1 );
	
	if( !ret )
	{
		ret = ExplainMenu( 0 );
	}
	else MagicExplainAfterMenuDisplay( 1 );
}

void SetMenuExplain( int type, int menu, int start_field )
{
	return;
	char filename[FILENAME_MAX];
	sprintf( filename, "./data/explain%02d.exp", type );
	FILE *fp;
	fp = Fopen( filename, "rt" );
	if( !fp ) return;
	
	char temp[MAX_STRING_];
	int count = 0;
	while( fgets( temp, MAX_STRING_, fp ) != NULL )
	{
		if( temp[0] == ';' ) continue;
		
		char *token;
		token = strtok( temp, "\t\n");
		if( token== NULL ) break;
		
		token = strtok( NULL, "\t\n");
		if( token== NULL ) break;
		
		SMenu[menu].nField[start_field+count].nType = FT_EXPLAIN_MENU;
		int left = atoi( token);
		
		token = strtok( NULL, "\t\n");
		int top = atoi( token);
		
		token = strtok( NULL, "\t\n");
		int right = atoi( token);
		
		token = strtok( NULL, "\t\n");
		int bottom = atoi( token);
		
		SetRect( SMenu[menu].nField[start_field+count].rCheakBox,left,top,right,bottom );
		
		char name_temp[50]="";
		token = strtok( NULL, "\t\n");
		strcpy( name_temp, token );
		
//		token = strtok( NULL, "\t\n");
#ifdef _DEBUG // finito 060507
		if( GetSysInfo( SI_GAME_MAKE_MODE ) ) 
		{
			int len = strlen( token );
			if( len > 500 ) JustMsg( "it's very long  [%d]", len );
		}
#endif		
		sprintf( SMenu[menu].nField[start_field+count].temp, "%s\n%s", name_temp, token );
		
		count++;	
	}
	SMenu[menu].nFieldCount=count+start_field;
	fclose( fp );	
}


//////////////////////////////////////////////////////////////////////
//	정수형 배열에 값 넣기
//	같은 값이 있으면 넣지 않는다.
//	값을 넣은후 max를 하나 올려 준다.
/////////////////////////////////////////////////////////////////////
int InsertIntArray( int node, int *int_array, int &curr_count, int array_max )
{
	if( curr_count >= array_max ) return 0;			// 꽉 찼다.  
	for( int i=0; i<curr_count; i++ )
	{
		if( node == int_array[i] ) 
		{	
			if( i == curr_count-1 ) return 0;
			
			memmove( &int_array[i], &int_array[i+1], sizeof( int ) * (array_max - (i+1)) );		// 이동해주는 루틴
			int_array[curr_count-1] = node;
			return 0;		// 같은 node가 존재한다.
		}
	}
	
	int_array[curr_count] = node;
	curr_count++;
	return 1;
}

//
//	정수형 배열에 값 빼기 ( 배열은 이동시킨다. )
//
int DeleteIntArray( int node, int *int_array, int &curr_count, int array_max )
{
	if( curr_count > array_max ) curr_count = array_max;
	
	for( int i=0; i<curr_count; i++ )
	{
		if( node == int_array[i] ) 
		{	
			memmove( &int_array[i], &int_array[i+1], sizeof( int ) * (array_max - (i+1)) );		// 이동해주는 루틴
			curr_count --;
			return 1;		// 같은 node가 존재한다.
		}
	}
	return 0;	// 그런 노드가 없다.
}


//
//	정수형 Array에 특정 node가 존재하는지의 판별
//
int IsExistNodeToArray( int node, int *int_array, int curr_count )
{
	for( int i=0; i<curr_count; i++ )
	{
		if( int_array[i] == node ) return true;
	}
	
	return false;
}



///////////////////////////////////////////////////////////////////////////////
void InsertMagicQuickArray( int magic_no )		// 퀵매직 배열에 사용한 매직 등록
{
	InsertIntArray( magic_no, magic_plus,  magic_plus_count, MAX_MAGIC );
	magic_plus_start = magic_plus_count-5;
	if( magic_plus_start < 0 ) magic_plus_start = 0;
}
void DeleteMagicQuickArray( int magic_no )		// 퀵매직 배열에 사용한 매직을 뺀다.
{
	DeleteIntArray( magic_no, magic_plus,  magic_plus_count, MAX_MAGIC );
	if( magic_plus_start > magic_plus_count-5 )	magic_plus_start = magic_plus_count-5;
	if( magic_plus_start < 0 ) magic_plus_start = 0;
}

void ResetMagicQuickArray( )
{
	memset( magic_plus, 0, sizeof( int)*MAX_MAGIC );
	magic_plus_count = 0;
	ClearQuickMemory();// 단축키도 초기화 시킨다.
	magic_plus_start = 0;
}

bool IsHeroDead()
{
	if( SCharacterData.condition == CON_DEATH || Hero->viewtype == VIEWTYPE_GHOST_ ) return true;
	else return false;
}


void PutMagicIconAll()
{
	return;
	for( int a=0; a<200; a++ )
	{
		Spr *s = &magic_spr[a];
		PutCompressedImage(40+a%15*40, 40+a/15*40, s);
	}
	//	PutMagicIcon( 40+a%15*40, 40+a/15*40, a );
}
/////////////////////////////////////////////////////////////////////////////
//
//	GetTextLine( filename );
//	텍스트 화일의 총 라인 구해오기 
//	remark ( ;, # 등등 )은 빼고 실제 라인만 구해올려고 했다.. ^^
//	중간 라이인이 비어있거나 하면 안 됌.
//
//////////////////////////////////////////////////////////////////////////////
int GetTextLine( char *filename )
{
	FILE *fp = Fopen( filename, "rt" );
	int line = 0;
	if( fp )
	{
		char buf[2048];
		while( fgets( buf, 2048, fp ) != NULL ) 
			if( buf[0] ) line++;
			
			fclose( fp );
	}
	
	return line;
}



// 상점번호와 전단계 메뉴 번화와의 컨버트 
// is_store 가 true일 경우 prev는 storelist이어서 menu번호를 리턴
// false일 경우 prev가 메뉴 번호이고 storelist 를 리턴
int ConvStoreListToMenu( int prev, bool is_store )
{
	int menu[] = {
		MN_BLACKSMITH, MN_FARM, MN_RESTAURANT, MN_HERBSHOP, MN_SHAMBLES, MN_CANDLE, MN_SLOPSHOP, MN_ALCHEMY,
			MN_WOODWORKING, MN_BOWCRAFT, MN_SHIPYARD, MN_DRINK_MAIN, MN_MAGIC_GUILD, MN_SHOP_MAIN,
	};
	int menu_max = sizeof( menu ) / sizeof( int );
	
	int storelist[] = { BLACKSMITH, FARM, RESTAURANT, HERBSHOP, SHAMBLES, CANDLE, SLOPSHOP, ALCHEMY, 
		WOODWORKING, BOWCRAFT, SHIPYARD, PUB, MAGIC_GUILD, STORE , 
	};
	int storelist_max = sizeof( storelist ) / sizeof( int );
	
	int *source;
	int *target;
	
	int source_max;
	int target_max;
	
	if( is_store )
	{
		source = storelist;
		source_max = storelist_max ;
		
		target = menu;
		target_max = menu_max;
	}
	else
	{
		source = menu;
		source_max = menu_max;
		
		target = storelist;
		target_max = storelist_max ;
	}
	
	////////////////////////////////////////
	for( int i=0; i<source_max; i++ )
	{
		if( prev == source[i] ) return target[i];
	}
	return 0;
}
// 귓속말 출력 ( 맨마지막에 한다 )
int recv_nation_msg_count;				// 나라 메세지
char recv_nation_msg_text[MAX_MSG];

char *GetNameById( int id )		// 아이디로 이름을 가져 온다.
{
	LPCHARACTER ch = Hero;
	while( ch )
	{
		if( ch->id == id ) return ch->name;
		ch = ch->lpNext;
	}
	
	return NULL;
}

void SmallMapClose()
{
	if( small_map_spr[0].img )
	{
		FreeOpenningSpr( &small_map_buf );
		for( int i = 0 ; i< SMALL_MAP_MAX; i++ ) small_map_spr[i].img = NULL;
	}
	
	main_interface.data[IF_MAP] = false; 
	SMenu[MN_SMALL_MAP].bActive = false;
	
	int cn = 1;
	while( SMenu[MN_SMALL_MAP].nField[cn].nType )
	{
		SMenu[MN_SMALL_MAP].nField[cn].nType = 0;
		cn++;
	}
	
	return;
}

int SetQuickSkill( int *index, int start, int type )		// 스킬 메뉴에서 보여주는 아이콘 설정..
{//021111 lsw
	memset( index, 0, sizeof( int) *5 );
	int count = 0;
	int j = 0;
	for( int i=0, j=0; i<MAX_SKILL; i++ )
	{
		if( i >= CRAPPLE+88 && i<= ORISON+88 ) continue;
		if( SCharacterData.SkillId[i] )
		{
			if( !skill[i].num ) continue;
			if( skill[i].series != type ) continue;
			if( count>= start && j<5)
			{
				index[j] = skill[i].num ;
				j++;
			}
			count++;
		}
	}
	if(1 != type)
	{
		return count+1;
	}
	SkillExp3 *pExp3 = (SkillExp3*)&SCharacterData.nJobSkill[SKILL_MASTER];
	switch(pExp3->skillType)
	{//이건 직업 스킬이기때문에 위에서 리턴
	case GM_QUEST_BLACKSMITH_MASTER	://대장장이 기성
		{
			index[j] = SKILL_BLACKSMITHY_MASTER;
			count++;
		}break;
	case GM_QUEST_CARPENTER_MASTER	://목수 기성    
		{
			index[j] = SKILL_CARPENTRY_MASTER;
			count++;
		}break;
	case GM_QUEST_BOWCRAFT_MASTER	://활제작사 기성
		{
			index[j] = SKILL_BOWCRAFTING_MASTER	;
			count++;
		}break;
	case GM_QUEST_TAILOR_MASTER		://재단사 기성
		{
			index[j] = SKILL_TAILORING_MASTER;
			count++;
		}break;
	default:{}break;
	}
	
	return count+1;
}

////////////////////////////////////////
//
//	채팅 단어를 걸러내는 메소드들
//
///////////////////////////////////////
#ifdef THAI_LOCALIZING_
#define MAX_WORD	1000
// thai2 YGI
char FILTER[MAX_WORD][256];			// 단어집합
int filter_count;				// 모든 단어의 개수
void LoadFilterWord( )
{
	FILE *fp;
	char temp[ MAX_PATH];
	char temp2[ MAX_PATH];

	strcpy( temp, "./data/filter.txt" );
	strcpy( temp2, "./data/filter.bin" );
	fp = Fopen( temp, "rt" );
	if( fp )
	{
		filter_count = 0;
		char temp[256];
		while( fgets( temp, 256, fp ) )
		{
			if( CheckContinueLine( temp[0] ) ) continue;
			CharLower( EatRearWhiteChar( temp ) );
			int bContinue = 0;
			for( int i=0; i<filter_count; i++ )
			{
				if( strcmp( FILTER[i], temp ) == 0 ) 
				{
					bContinue = 1;
					break;
				}
			}
			if( bContinue ) continue;
			strcpy( FILTER[filter_count++], temp );
		}
		
		fclose(fp);
		
		fp = Fopen( temp2, "wb" );
		char p = 0;
		if( fp )
		{
			char *lpText = FILTER[0];
			for( int ct=0; ct<MAX_WORD*256; ct++ )
			{
				char ret = (*(lpText+ct))+p;
				fwrite( &ret, 1, 1, fp );
				p++;
			}
			fclose(fp );			
		}		
	}
	else
	{
		char p = 0;
		fp = Fopen( temp2, "rb" );
		if( fp )
		{
			char *lpText = FILTER[0];
			for( int ct=0; ct<MAX_WORD*256; ct++ )
			{
				char ret;
				fread( &ret, 1, 1, fp );
				*(lpText+ct) = ret - p;
				p++;
			}
			fclose(fp );
		}
		else
		{
			JustMsg( "Wrong data [10:1800]" );
			exit(0);
		}
	}

	if( GetSysInfo( SI_GAME_MAKE_MODE ) && GetSysInfo( SI_NOT_CONNECT_MODE ) )
	{
		fp = Fopen( "./filter.txt", "wt" );
		if( fp )
		{
			for( int i=0; i<MAX_WORD; i++ )
			{
				if( FILTER[i][0] )
					fprintf( fp, "%s\n", FILTER[i] );
			}
			fclose( fp );
		}
	}
}
#else

#define MAX_WORD	200

char FILTER[MAX_WORD][20];			// 단어집합
int filter_count;				// 모든 단어의 개수
void LoadFilterWord( )
{
	FILE *fp;
	char temp[ MAX_PATH];
	char temp2[ MAX_PATH];
	
	strcpy( temp, "./data/filter.txt" );
	strcpy( temp2, "./data/filter.bin" );
	fp = Fopen( temp, "rt" );
	if( fp )
	{
		filter_count = 0;
		while( fscanf(fp, "%s", FILTER[filter_count++] ) != EOF );
		
		fclose(fp);
		
		fp = Fopen( temp2, "wb" );
		char p = 0;
		if( fp )
		{
			char *lpText = FILTER[0];
			for( int ct=0; ct<MAX_WORD*20; ct++ )
			{
				char ret = (*(lpText+ct))+p;
				fwrite( &ret, 1, 1, fp );
				p++;
			}
			fclose(fp );			
		}		
	}
	else
	{
		char p = 0;
		fp = Fopen( temp2, "rb" );
		if( fp )
		{
			char *lpText = FILTER[0];
			for( int ct=0; ct<MAX_WORD*20; ct++ )
			{
				char ret;
				fread( &ret, 1, 1, fp );
				*(lpText+ct) = ret - p;
				p++;
			}
			fclose(fp );
		}
	}
}
#endif

int ConvString( char *szStr ) // 욕설 필터
{
	int ret = 0;
	char *cut;
	char *str_small;		// 모두 소문자로 바꾼값...
	char conv = ' ';				// 교체할 문자
	int str_len = strlen( szStr );
	
	char *str_small2 = new char[str_len+1];
	
	strcpy( str_small2, szStr );
	CharLower( str_small2 );		// 소문자로 바꾼다.  //010809 lms
	str_small = str_small2;
	
	int i=0;
	while ((i<MAX_WORD)&& FILTER[i][0]) // CSD-030324
	{	// 빈칸이 아닌 문자가 있을때 까지만 
		while( 1 )
		{
			str_small = EatFrontWhiteChar(str_small);
			cut = strstr( str_small, FILTER[i] );
			if( !cut ) break;
			
			ret = 1;		// 욕설 필터에 걸렸다.
			char *check_cut = cut;		// 한글의 처음인가 체크 루틴
			int count = 0;
			int check = 0;
			while( 1 )
			{
				if( str_small >= check_cut )
				{
					if( !(count%2) ) check= 1; // 한글의 처음이다...
					break;
				}
				
				if( !( IsHangul( *check_cut ) ) )	// thai2 YGI
				{	//< CSD-030324
					if( count%2 ) check= 1; // 한글의 처음이다...
					break;
				}	//> CSD-030324

				check_cut --;
				count++;
			}
			if( !check ) 
			{
				*cut = conv;
				continue;		// 한글의 처음이 아니다.
			}
			
			int len = strlen( FILTER[i] );
			int point_gab = cut - str_small2;			// 주소의 차이
			for( int j=0; j<len; j++ )	
			{
				*(cut+j) = conv;
				*(szStr+point_gab+j) = conv;
			}
		}
		i++;
	}
	if( szStr[str_len] != 0 ) szStr[str_len]=0;
	
	delete str_small2;	
	
	return ret ;
}

/////////////////////////////////////////////////
bool CheckNumber( char *text, int len )		// 숫자 이외의 값이 있는지를 체크한다.
{
	for( int i=0; i<len; i++ )
	{
		if( text[i] < '0' || text[i] > '9' ) return true;
	}
	
	return false;
}


LPCHARACTER ExistHe( char *name )		// 내 시야에 존재하는 넘인지 알아보는 루틴
{
	LPCHARACTER temp = Hero;
	while( temp )
	{
		if( ::stricmp( name, temp->name ) == 0 )//030102 lsw
		{
			return temp;
		}
		temp = temp->lpNext;
	}
	
	return NULL;
}

LPCHARACTER ExistHeCase( char *name )
{
	LPCHARACTER temp = Hero;
	while( temp )
	{
		if( ::strcmp( name, temp->name ) == 0 )//030102 lsw
		{
			return temp;
		}
		temp = temp->lpNext;
	}
	
	return NULL;
}

CColor Lv_Color[256];
k_CallNameByLv CallNameByLv[50];
void LoadLvColor()
{
	char *filename = "./data/lv_color.txt";
	char *filename_bin = "./data/lv_color.bin";
	
	FILE *fp = Fopen( filename, "rt" );
	if( fp )
	{
		char temp[512];
		int count = 0;
		while( fgets( temp, 512, fp ) != NULL && count < 255 )
		{
			if( temp == NULL|| temp[0] == ';'  || temp[0] == '#' ) continue;
			int r, g, b;
			char *token;
			token = strtok( temp, "\t\n" );
			if( !token ) continue;
			r = atoi( token );
			token = strtok( NULL, "\t\n" );
			g = atoi( token );
			token = strtok( NULL, "\t\n" );
			b = atoi( token );
			Lv_Color[count].Set( r, g, b );
			count ++;
		}
		fclose( fp );
		
		fp = Fopen( filename_bin, "wb" );
		if( fp )
		{
			fwrite( Lv_Color, sizeof( CColor ), 256, fp );
			fclose( fp );
		}
	}
	else
	{
		FILE *fp = Fopen( filename_bin, "rb" );
		if( fp )
		{
			fread( Lv_Color, sizeof( CColor ), 256, fp );
			fclose( fp );
		}
	}
}

void LoadCallNameByLv()
{	//< CSD-031030
	if (checkbeta == '1')
	{
		g_mgrDual.Load("./data_b5/call_hero.bin");
	}
	else
	{
		g_mgrDual.Load("./data/call_hero.bin");
	}
}	//> CSD-031030

//< CSD-011022
void PutLvNameByColor(int x, int y, LPCHARACTER ch)
{
	const int color = Lv_Color[ch->lv-1].GetRGB();
	const string strName = g_mgrDual.GetClassName(ch->class_type, 
		ch->aStepInfo[DUAL_CLS], 
		ch->aStepInfo[CLS_STEP], 
		ch->lv);
	HprintBold(x, y, color, 0, (char*)strName.c_str());
}

void PutDualMark(int x, int y, LPCHARACTER ch)
{
	if (ch->aStepInfo[CLS_STEP] > 0)
	{
		Spr* pMark = GetSprOfMenu(DUAL_MARK_IMG, ch->aStepInfo[CLS_STEP]);
		
		if (pMark && pMark->img)
		{ 
			PutCompressedImage(x - pMark->ox, y - pMark->oy, pMark);
		}
	}
}
//> CSD-011022

void PutGuildLvByColor( int x, int y, int guild_code, int guild_master )		// 길드 직책 출력
{
	if( g_GuildMgr[guild_code].first )
	{	//< CSD-030324
		Hcolor( RGB16( 255, 213,0 ) );
		Hprint2( x, y, g_DestBackBuf, g_GuildMgr[guild_code].m_szName );
	}	//> CSD-030324
	else
	{
		DWORD color;
		
#if defined(TAIWAN_LOCALIZING_ ) || defined( CHINA_LOCALIZING_ ) || defined( HONGKONG_LOCALIZING_ )//010730 lms
		switch( guild_master )
		{
		case 1: color = RGB16( 0,0,0 ); break;
		case 2: color = RGB16( 0,0,255 ); break;
		case 3: color = RGB16( 0,180,0 ); break;
		case 4: color = RGB16( 255,0,0 ); break;
		default :	color = RGB16( 255, 213,0 ); break;
		}
		char *guild_degree = CallGuildDegreeName( guild_code, guild_master );
		if( guild_degree )	
		{
			Hcolor( color );
			Hprint2( x, y, g_DestBackBuf, guild_degree );
		}
#else
		switch( guild_master )
		{
		case 1: color = RGB( 0,0,0 ); break;
		case 2: color = RGB( 0,0,255 ); break;
		case 3: color = RGB( 0,180,0 ); break;
		case 4: color = RGB( 255,0,0 ); break;
		default :	color = RGB( 255, 213,0 ); break;
		}
		char *guild_degree = CallGuildDegreeName( guild_code, guild_master );
		if( guild_degree )	
		{
			HprintBold( x, y, color, 0, guild_degree );
		}
#endif
	}
}

int GetCallJob( int call_job_lv )		// 직업 능력 단위 가져오기		// 100 단위
{
	int call_job = -1;
	switch( call_job_lv/10 )
	{
	case 10 :	call_job = 7;	break;	// GM
	case 9:		call_job = 6;	break;	// master
	case 8:		call_job = 5;	break;	// expert
	case 7:		call_job = 4;	break;	// major
	case 6:		
	case 5:		call_job = 3;	break;	// senjor
	case 4:		
	case 3:		call_job = 2;	break;	// junior
	case 2:		
	case 1:		
	case 0:		call_job = 1;	break;	// pupil
	}
	return call_job;
}

void StopWaveOfMenu()
{
	for ( int i=SN_START; i<=SN_BGMEND; i++ ) StopWave(i);
}

void MenuSoundClose( int menu )
{
	if( SMenu[menu].bActive )
	{
		SMenu[menu].bActive = false;
		MP3( SN_MENU_CLOSE );
		SoundOn = false;
	}
}

bool IsASCII( char *szText )
{
	const int len = ::strlen( szText );
	for( int i=0; i<len; i++ )
	{
		if( (szText[i] >= 'A' && szText[i] <= 'Z')
		||	(szText[i] >= 'a' && szText[i] <= 'z')
		||	(szText[i] >= '0' && szText[i] <= '9'))
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void LoadTextDefault( char *filename, char *text, int text_max )
{
	FILE *fp = Fopen( filename, "rt" );
	if( fp )
	{
		FILE *temp_fp = fp;
		fseek(fp, 0, SEEK_END );
		int max_fp = fp - temp_fp;
		fseek(fp, 0, SEEK_SET );
#ifdef _DEBUG // finito 060507
		if( GetSysInfo( SI_GAME_MAKE_MODE ) ) 
		{
			if( max_fp > text_max ) JustMsg( "file : %s :::: max==>[%d], curr==>[%d] : 글자가 너무 많네요. ", filename, text_max, max_fp);
		}
#endif	
		char temp[10001];
		char *curr = text;
		int count =0;
		while( fgets( temp, 10000, fp ) != NULL )
		{
			int len = strlen( temp );
			strcpy( curr, temp );
			curr+=len;
			*curr = '\n';
			curr++;
			count += len;
		}
		*curr = NULL;
		fclose( fp );
#ifdef _DEBUG // finito 060507
		if( GetSysInfo( SI_GAME_MAKE_MODE ) ) 
		{
			if( count >= text_max ) JustMsg( "file : %s :::: max==>[%d], curr==>[%d] : word too much  ", filename, text_max, count );//010215 lsw
		}
#endif
	}
}

void GetNationBBS( char *filename )
{
	FILE *fp = Fopen( filename, "rt" );
	if( fp )
	{
		int count = 0;
		while( fgets( nation_bbs2[count/8][count%8], 50, fp ) ) 
		{
			EatRearWhiteChar( nation_bbs2[count/8][count%8] );
			count++;
		}
		
		fclose( fp );
	}
	
	//	LoadTextDefault( filename, nation_bbs, 2000 );
}

void OpenFightMapSignMenuOpen( int team[] )
{
	if( SMenu[MN_FIGHT_MAP_SIGN].bActive ) return;
	
	for( int i=0; i<4; i++ )
	{
		if( team[i] )
			SMenu[MN_FIGHT_MAP_SIGN].nField[i+1].nWillDo = FONT_COLOR_NUMBER;		// color 값
		else SMenu[MN_FIGHT_MAP_SIGN].nField[i+1].nWillDo = FONT_COLOR_PLUS;		// color 값
	}
	
	CallMenu( MN_FIGHT_MAP_SIGN );
	SMenu[MN_FIGHT_MAP_SIGN].key = 300;
}


void FreeAllOfMenu()		// 0927 YGI
{
	if( !g_StartMenuOn )
	{
		SaveGuildInfoEx();		// 길드 메뉴 저장
		SAFE_DELETE(g_Guild_Introduction);
		g_Menu_Variable.clear();
		SaveMemorize();
	}
	SAFE_DELETE(g_lpRankLadderBBS);
	SAFE_DELETE(g_MenuImageMgr);
}


int IsSameNation( int type, LpCharacterParty lpParty )
{
	if( lpParty->ch )
	{
		if( Hero->name_status.nation == lpParty->ch->name_status.nation ) return 1;		// 파티가 가능하다 
	}
	return 0;
}

void SetMouseCh( LPCHARACTER lpTarget )
{
	strcpy( mouse_ch.m_Name, lpTarget->name );
	mouse_ch.m_Id = lpTarget->id;
	mouse_ch.m_ImageNo	= lpTarget->face;
	mouse_ch.m_Gender	= lpTarget->sex;
	mouse_ch.m_Lv		= lpTarget->lv;		// 0809 YGI
	mouse_ch.ch			= lpTarget;
}

void DeleteMouseCh( char *name )
{
	if( g_MouseItemType == PARTY )
	{
		if( strcmp( mouse_ch.m_Name, name ) == 0 )
		{
			g_MouseItemType = 0;
			memset( &mouse_ch, 0, sizeof( CharacterParty ) );
		}
	}
}

void InitMenuByChangeMap()
{
	SmallMenuClose();
}

int CheckRegistCandidate( )
{
	if( g_CanRegistCandidate < 0 ) return -1;
	//	if( !GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		if( !CheckGuildMaster( Hero )  ) return -1;
		if( Hero->lv < 74 ) return -1;
	}
	CallServer(CMD_IS_THIS_MAN_CANDIDATE);			// 후보로 등록할 수 있는 사람인지를 확인
	return 1;
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// 020701 YGI
extern void CommonMenuSet();
void Check_Kein_Menu( int type )		// 툴에서 메뉴
{
	switch( type )
	{
	case ID_RESET_VOTE : CallServer( CMD_RESET_VOTE_VALUE ); break;
	case ID_CHECK_EFFECT : 
		{
			if(!SMenu[MN_EFFECT_TOOL].bActive)
			{
				CallMenu( MN_EFFECT_TOOL );
			}
			else
			{
				SMenu[MN_EFFECT_TOOL].bActive = FALSE;
				y_MenuFocus = 0;
			}
			break;
		}
	case ID_RESET_SMALL_MENU	:	SmallMenuSet(); CommonMenuSet(); break;
	case ID_RESET_MENU	:	MenuSet(); break;
	case IDM_VIEW_GUILD : 
		g_FightMapStart = true;
		g_BattleStatusView = 0;
		ChangeEquipForFightMapStart();			
		break;
	case IDM_RELOAD_MENU_DATA: 
		LoadMenuStr(); 
		LoadMenuImage();
		break;
	case ID_MENUITEM_SET_SCENARIO:
		//< 030307 soto
		StartSetScenarioTime();
		//> 030307 soto
			break;
	}
}


int guild_cmd_info_count;
void ViewGuildCommandInfo( )		// 명령 커맨드로 호출
{
	if( guild_cmd_info_count > 0 ) return;
	guild_cmd_info_count = 210;
}

void ViewGuildCommandInfo2( )		// 실제로 정보를 찍어 준다.
{
	if( guild_cmd_info_count <= 0  ) return;
	if( guild_cmd_info_count%30 == 0 ) 
	{
		switch( guild_cmd_info_count / 30 )
		{
		case 7: AddCurrentStatusMessage( 250,60,250, lan->OutputMessage(7,131)); break;//010215 lsw
		case 6:	AddCurrentStatusMessage( 250,60,250,lan->OutputMessage(7,132) ); break;
		case 5:	AddCurrentStatusMessage( 250,60,250, lan->OutputMessage(7,133) ); break;
		case 4: AddCurrentStatusMessage( 250,60,250, lan->OutputMessage(7,134) ); break;
		case 3:	AddCurrentStatusMessage( 250,60,250, lan->OutputMessage(7,135) );break;
		case 2: AddCurrentStatusMessage( 250,60,250, lan->OutputMessage(7,136) ); break;//010215 lsw
		case 1: 
			{
				char *text = NULL;
				switch( Hero->name_status.nation )
				{
				case N_VYSEUS: text = lan->OutputMessage(7,129) ; break;
				case N_ZYPERN: text = lan->OutputMessage(7,130) ; break;//010215 lsw
				}
				if( text ) AddCurrentStatusMessage( 250,60,250, lan->OutputMessage(7,128), text );//010215 lsw
				break;
			}
		}
	}
	guild_cmd_info_count--;
}

int ear_cmd_info_count;
void ViewEarCommandInfo()
{
	if( ear_cmd_info_count > 0 ) return;
	ear_cmd_info_count = 180;
}

void ViewEarCommandInfo2( )		// 실제로 정보를 찍어 준다.
{
	if( ear_cmd_info_count <= 0  ) return;
	if( ear_cmd_info_count%30 == 0 ) 
	{
		switch( ear_cmd_info_count / 30 )
		{
		case 6: AddCurrentStatusMessage( 250,60,250, kein_GetMenuString( 13 )); break;
		case 5: AddCurrentStatusMessage( 250,60,250, kein_GetMenuString( 14 )); break;
		case 4: AddCurrentStatusMessage( 250,60,250, kein_GetMenuString( 15 )); break;
		case 3:	AddCurrentStatusMessage( 250,60,250, kein_GetMenuString( 16 )); break;
		case 2: AddCurrentStatusMessage( 250,60,250, kein_GetMenuString( 17 )); break;
		case 1: AddCurrentStatusMessage( 250,60,250, kein_GetMenuString( 18 )); break;
		}
	}
	ear_cmd_info_count--;
}

char *Kein_EatFrontWhiteChar( char *pStr )
{
	char*	szWhite = " .,\t\n\r";
    if ( pStr )
    {
		while ( *pStr )
		{
			if ( strchr( szWhite, *pStr ) ) pStr++;
			else break;
		}	
    }
    return  pStr;    
}

inline int CheckNation( int nation )
{ 
	if( nation < N_VYSEUS || nation > N_YILSE ) return 0;
	return 1;
}
//////////////////////////////////////////////////////
//
// 특정 프로그램 호출하기 
//
///////////////////////////////////////////////////////
BOOL LaunchApplication(LPCTSTR pCmdLine )//, PROCESS_INFORMATION *pProcessInfo)
{
	STARTUPINFO stStartUpInfo = {0,};
	stStartUpInfo.cb = sizeof(STARTUPINFO);
	
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
	stStartUpInfo.wShowWindow = SW_SHOWDEFAULT;
	
	PROCESS_INFORMATION stProcessInfo;
	return CreateProcess(NULL, (LPTSTR)pCmdLine, NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS, NULL, 
		NULL, &stStartUpInfo, &stProcessInfo);
}

BOOL LaunchApplication_kein( char *app_name, char *arg )
{
	char value[255];
	char full_name[255];
	
	HKEY hKey;
	sprintf( value, "%s/shell/open/command", app_name );
	int ret = RegOpenKeyEx( HKEY_CLASSES_ROOT, value, 0, KEY_ALL_ACCESS, &hKey );
	if( ret != ERROR_SUCCESS ) return 0; 
	
	DWORD reg_type;			// 타입을 받아 온다.
	DWORD reg_size = 255;	// 얼만큼 카피해 올지...
	ret = RegQueryValueEx( hKey , "", NULL, &reg_type, (BYTE *) full_name, &reg_size );
	
	RegCloseKey(hKey);
	if( ret != ERROR_SUCCESS ) return 0; 
	
	sprintf( full_name, "%s %s", full_name, arg );
	ret = WinExec( full_name, SW_SHOWNORMAL );
	if( ret > 31 ) return true;
	else return false;
}

// 020701 YGI
void KeinMessageBox( int next_menu, char *msg, ... )
{ 
	char *temp = SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].temp;
	
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf(temp, msg, arg );
	}
    va_end( arg );
	
	SMenu[MN_DEFAULT_MESSAGEBOX].nField[3].m_lpTemp = temp;
	CallSmallMenu( MN_DEFAULT_MESSAGEBOX );
	SMenu[MN_DEFAULT_MESSAGEBOX].nTemp = next_menu;
}

void AutoLevelUp()
{
	static int auto_value[5][11] = 
	{// str,	dex,	con,	wis,	int,	cha,	movp,	endu,	mor,	wsps,	luck,
		{5,		0,		5,		0,		0,		0,		0,		0,		0,		0,		0},		// WARRIOR	
		{0,		3,		4,		0,		0,		0,		3,		0,		0,		0,		0},		// THIEF	
		{0,		4,		4,		0,		0,		0,		2,		0,		0,		0,		0},		// ARCHER	
		{0,		0,		3,		0,		5,		0,		0,		0,		0,		2,		0},		// WIZARD	
		{0,		0,		4,		4,		0,		0,		0,		0,		0,		2,		0},		// PRIEST	
	};
	
	int *ability = SCharacterData.nCharacterAbility;
	int class_type = SCharacterData.nCharacterData[CLASS];
	for( int i=0; i<=LUCK; i++ )
	{
		for( int j=0; j<auto_value[class_type][i]; j++ )
		{
			SCharacterData.LvUpPoint--;
			SendLvUpPointEach( i );
			ability[i]++;
		}
	}
	
	switch( class_type )
	{
	case WARRIOR :	AddCurrentStatusMessage( 255,255,0, "STR:%d, CON:%d", ability[STR], ability[CON] );
		break;
	case THIEF	 :	AddCurrentStatusMessage( 255,255,0, "DEX:%d, CON:%d, MOVP:%d", ability[DEX], ability[CON], ability[MOVP] );
		break;
	case ARCHER	 :	AddCurrentStatusMessage( 255,255,0, "DEX:%d, CON:%d, MOVP:%d", ability[DEX], ability[CON], ability[MOVP] );
		break;
	case WIZARD	 :	AddCurrentStatusMessage( 255,255,0, "CON:%d, INT:%d, WS:%d", ability[CON], ability[INT_], ability[WSPS] );
		break;
	case PRIEST	 :	AddCurrentStatusMessage( 255,255,0, "CON:%d, WIS:%d, PS:%d", ability[CON], ability[WIS], ability[WSPS] );
		break;
	}
	
	MP3( SN_LV_EACH_UP );
	AutoLevelUpCount = 0;
}

bool IsEquipAbleCheckAbleDay(int start, int end)
{	//< CSD-030812
	if(!start)	{return true;}//제한 없음 
	if(!end)	{return true;}//제한 없음 
	
	int mon, day, hour;
	GetRealTime( mon, day, hour );
	
	const int FullToday = (mon*100) + day;

	if (start <= end)
	{
		if (start <= FullToday && end >= FullToday)
		{
			return true;//착용기간이다.
		}
	}
	//< 040102 kyo
	else
	{
		if( start <= FullToday && 1231 >= FullToday)
		{
			return true;
		}
		else if( 101 <= FullToday&& end > FullToday)
		{
			return true;
		}		
	}
	//> 040102 kyo

	return false;
}	//> CSD-030812

void ExplainItemName(const int x , const int y, ItemAttr *pItem, int &iCt, int &iBlk, bool bIsBgOut)
{
	const int iWidth = TXT_BG_NORMAL_WIDTH;//021001 lsw
	
	int iReultLineCount = 0;
	int iReultBlankSize = 0;
	
	const int iLGab = TxtOut.GetLineGab();
	const int iBlkSize = TxtOut.GetBlankGabSize();
	
	const RareMain *pRareValue = (RareMain*)(&pItem->attr[IATTR_RARE_MAIN]);
	CItem *item = ItemUnit( *pItem );
	if( !item ) return;
	
	if(pItem->attr[IATTR_RARE_MAIN])//레어가 있다면 들어감
	{
		switch(pRareValue->iHighLevel)
		{
		case H_LV_NO_HIGH_ITEM:
			{
				TxtOut.Init(COLOR_BLUEGREEN,TXT_BG_NORMAL_WIDTH);
				if(bIsBgOut)
				{
					TxtOut.PutTextHeaderBGImg(x,y+iBlk+iLGab*iCt,iWidth);
				}
				Hcolor( FONT_COLOR_RARE_MAIN );
				for(int ct = 0; 3 > ct; ct++)
				{
					int iIndex = 0;
					switch(ct)
					{
					case 0:{iIndex = pRareValue->soksung1;}break;
					case 1:{iIndex = pRareValue->soksung2;}break;
					case 2:{iIndex = pRareValue->soksung3;}break;
					default:{continue;}break;
					}
					if(!iIndex){continue;}
					if(bIsBgOut)
					{
						if(pRareValue->IsDynamicRare){iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,ItemFunction[iIndex].NameAdd);}
						else{iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,ItemRare[iIndex].LocalName);}
						iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"                      %d Grade",pRareValue->grade);
						iCt += iReultLineCount;
					}
					else
					{
						if(pRareValue->IsDynamicRare){iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,ItemFunction[iIndex].NameAdd);}
						else{iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,ItemRare[iIndex].LocalName);}
						iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"                    %d Grade",pRareValue->grade);
						iCt += iReultLineCount;
					}
				}
				//아이템 이름 출력
				Hcolor( FONT_COLOR_BLIGHT_GREEN);
				if(bIsBgOut)
				{
					iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s>", item->GetItemHanName());
					iCt += iReultLineCount;
				}
				else
				{
					iReultLineCount = TxtOut.RcTXTOutBNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s>", item->GetItemHanName());
					iCt += iReultLineCount;
				}
			}break;
		case H_LV_HIGH_ITEM:
			{
				TxtOut.Init(COLOR_RED,TXT_BG_NORMAL_WIDTH);
				Hcolor( FONT_COLOR_BLIGHT_GREEN);
				if(bIsBgOut)
				{
					TxtOut.PutTextHeaderBGImg(x,y+iBlk+iLGab*iCt,iWidth);
					iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
					iBlk+=iReultBlankSize;
					
#ifdef	THAI_LOCALIZING_	//< BBD 031214 태국에서 High 글자가 오버랩됨
					iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCt,iWidth,0,"<         %s +%d>", 
						item->GetItemHanName(),
						pRareValue->grade);
#else
					iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s %s +%d>", 
						ItemRare[pRareValue->soksung1].LocalName, 
						item->GetItemHanName(),
						pRareValue->grade);
#endif						//> BBD 031214 태국에서 High 글자가 오버랩됨
					
					Hcolor(FONT_COLOR_BLIGHT_RED);
					iReultLineCount = TxtOut.RcTXTOutBNoBG(x,y+iBlk+iLGab*iCt,iWidth,0," %s", 
						ItemRare[pRareValue->soksung1].LocalName);
					iCt += iReultLineCount;
				}
				else
				{
					iReultLineCount = TxtOut.RcTXTOutBNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s %s +%d>", 
						ItemRare[pRareValue->soksung1].LocalName, 
						item->GetItemHanName(),
						pRareValue->grade);
					
					Hcolor(FONT_COLOR_BLIGHT_RED);
					iReultLineCount = TxtOut.RcTXTOutBNoBG(x,y+iBlk+iLGab*iCt,iWidth,0," %s", 
						ItemRare[pRareValue->soksung1].LocalName);
					iCt += iReultLineCount;
				}			
			}break;
		case H_LV_LEGEND_ITEM:
			{
				TxtOut.Init(COLOR_BLUE,TXT_BG_NORMAL_WIDTH);
				Hcolor( FONT_COLOR_HIGH_RARE_MAIN );
				//레전드 종류 출력
				if(bIsBgOut)
				{
					TxtOut.PutTextHeaderBGImg(x,y+iBlk+iLGab*iCt,iWidth);
					iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
					iBlk+=iReultBlankSize;
					
					Hcolor( FONT_COLOR_SOLID_YELLOW);
					iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCt,iWidth,0,"= %s %s =", 
						lan->OutputMessage(2,780),
						g_aItemLegend[pRareValue->soksung2].aSub[pRareValue->soksung3].szName
						);
					iCt += iReultLineCount;
					
					Hcolor( FONT_COLOR_BLIGHT_GREEN);
					iReultLineCount = 	TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s>",
						item->GetItemHanName());
					iCt += iReultLineCount;
				}
				else
				{
					Hcolor( FONT_COLOR_SOLID_YELLOW);
					iReultLineCount = TxtOut.RcTXTOutBNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"= %s %s =", 
						lan->OutputMessage(2,780),
						g_aItemLegend[pRareValue->soksung2].aSub[pRareValue->soksung3].szName
						);
					iCt += iReultLineCount;
					
					Hcolor( FONT_COLOR_BLIGHT_GREEN);
					iReultLineCount = 	TxtOut.RcTXTOutBNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s>",
						item->GetItemHanName());
					iCt += iReultLineCount;
				}
			}break;
		}//switch(pRareValue->HighLevel)
	}//if(pRareValue->soksung1 && pRareValue->grade )//레어가 있다면 들어감
	else//노말아이템 이름 출력
	{//헤더그림
		TxtOut.Init(COLOR_BLACK,TXT_BG_NORMAL_WIDTH);
		Hcolor( FONT_COLOR_BLIGHT_GREEN);
		if(bIsBgOut)
		{
			TxtOut.PutTextHeaderBGImg(x,y+iBlk+iLGab*iCt,iWidth);
			iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
			iBlk+=iReultBlankSize;
			iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s>", item->GetItemHanName());
			iCt += iReultLineCount;
		}
		else
		{
			iReultLineCount = TxtOut.RcTXTOutBNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s>", item->GetItemHanName());
			iCt += iReultLineCount;
		}
	}
}

void ExplainItem2( int xx, int yy, ItemAttr *olg_item )//020821-2 lsw
{
	char temp_str[255] = {0,};
	const int item_type = olg_item->item_no/1000;
	const RareMain *pRareValue = (RareMain*)(&olg_item->attr[IATTR_RARE_MAIN]);
	
	CItem *item = ItemUnit( *olg_item );
	if( !item ) return;
	const int kind = item->GetItemKind();
	
	xx = g_pointMouseX, yy = g_pointMouseY+20;
	
	yy += 7;//7
	xx -= 150;//150
	
	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_NORMAL_WIDTH;//021001 lsw
	
	int iCt = 0;
	int iReultLineCount = 0;
	
	int iBlk = 0;
	int iReultBlankSize = 0;
	
	const int iLGab = TxtOut.GetLineGab();
	const int iBlkSize = TxtOut.GetBlankGabSize();
	
	if( xx < 0 ){	xx = 0 ;	}
	if( xx+iWidth > GAME_SCREEN_XSIZE ) {xx = GAME_SCREEN_XSIZE-iWidth;}
	if( yy+60 > GAME_SCREEN_YSIZE ) {yy = GAME_SCREEN_YSIZE - 60;}
	
	static int x = xx;
	static int y = yy;
	static ItemAttr olg_item2;
	if( !EqualItem(olg_item, &olg_item2 ) )		// 다른 아이템
	{
		x = xx;
		y = yy;
		olg_item2 = *olg_item;
	}
	if( !is_mainmenu_active )
	{
		x = Hero->x-Mapx-spr[71].xl-40;
		y = Hero->y-Mapy-spr[71].yl*4-30;
	}
	else if( SMenu[MN_ITEM_MAKE_SELECT].bActive )
	{
		x = SMenu[MN_ITEM_MAKE_SELECT].x - 150;
		y = SMenu[MN_ITEM_MAKE_SELECT].y + 7;
	}//설명 뜨는 위치 보정
	
	ExplainItemName(x,y,olg_item,iCt,iBlk,true);
	
	iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
	iBlk+=iReultBlankSize;
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )//아이템 이름 출력
	{
		iReultLineCount = TxtOut.RcTXTOutB(x,y+iBlk+iLGab*iCt,iWidth,0,"%s     ( %d )",item->GetItemHanName(), olg_item->item_no);
		iCt += iReultLineCount;
	}
#endif
	// 040105 YGI
	// 보물 지도인 경우
	if( item->GetItemKind() == IK_TREASURE_MAP )
	{
		Hcolor( FONT_COLOR_NUMBER );
		CTreasureMapItem *attr = (CTreasureMapItem* )&olg_item->attr[IATTR_TREASURE_MAP];
		// 040105 YGI
		if( !attr->isChecked || attr->mapnumber == 0 )
		{
			iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0, kein_GetMenuString( 214 ) );
		}
		else
		{
			int off_x, off_y;
			switch( attr->offset2 )
			{
			case 0 : off_x = attr->offset; off_y = attr->offset; break;
			case 1 : off_x = attr->offset; off_y = -attr->offset; break;
			case 2 : off_x = -attr->offset; off_y = attr->offset; break;
			case 3 : off_x = -attr->offset; off_y = -attr->offset; break;
			}
			iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,"%s(%d, %d)", GetMapNameByIndex(attr->mapnumber), attr->x+off_x, attr->y+off_y );
#ifdef _DEBUG // finito 060507
			if( GetSysInfo( SI_GAME_MAKE_MODE ) )
			{
				iCt += iReultLineCount;
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,"<%s(%d, %d)(%d, %d)>", GetMapNameByIndex(attr->mapnumber), attr->x, attr->y, attr->offset, attr->offset2);
			}
#endif
		}
		iCt += iReultLineCount;
		TxtOut.PutTextTailBGImg(x,y+iBlk+iLGab*iCt,iWidth);//꼬리 테두리
		return;
	}
	/////////////////////////////////////////////	
	const int need_tatics = item->GetMouse_MRD();
	if( need_tatics && ((olg_item->item_no/1000) >= WEAPON ) ) // 요구 택틱이 있을때.
	{
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,139), explain_tactics[item->GetSkillAbility()-88], (need_tatics==1)?need_tatics:need_tatics+1 );
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,140), explain_tactics[item->GetSkillAbility()-88] );//020821-2 lsw
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	int need_count = 0;
	int need_type[15];
	int need_value[15];
	item->GetItemNeedAbility( need_count, need_type, need_value );
	int is_equip = item->is_equipable();
	if( need_count )		// 요구 수치가 있을경우 
	{
		const int iGender = item->GetGender();
		if( iGender )//성별 구분 있음
		{
			if( is_equip ) 
			{ 
				Hcolor( FONT_COLOR_BLIGHT_BLUE); 
				if(iGender==MALE)
				{
					iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(5,117), lan->OutputMessage(5,115),  lan->OutputMessage(5,118));//lsw
				}
				else
				{
					iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(5,117), lan->OutputMessage(5,116),  lan->OutputMessage(5,118));//lsw
				}
			}
			else 
			{ 
				Hcolor( FONT_COLOR_PLUS ); 
				if(iGender==MALE)
				{
					iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(5,117), lan->OutputMessage(5,115),  lan->OutputMessage(5,119));//lsw
				}
				else
				{
					iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(5,117), lan->OutputMessage(5,116),  lan->OutputMessage(5,119));//lsw
				}
			}
		}
		else
		{
			if( is_equip ) 
			{ 
				Hcolor( FONT_COLOR_BLIGHT_BLUE); 
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,"            %s %s", "    ", lan->OutputMessage(5,118) );
			}
			else 
			{ 
				Hcolor( FONT_COLOR_PLUS ); 
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,"            %s %s", "    ", lan->OutputMessage(5,119) );//010217 lsw
			}
		}
		
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(5,120) );//lsw
		iCt += iReultLineCount;
		
		temp_str[0] = NULL;
		for( int i=0; i<need_count; i++ )
		{
			sprintf( temp_str, "%s  %d %s", temp_str, need_value[i], string_ability[need_type[i]] ); 
		}
		//< CSD-030324
	#ifndef USA_LOCALIZING_ // thai2 YGI
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str );//lsw
		OnlyNumber( temp_str, strlen( temp_str ) );
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str);//lsw
	#else
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str );//lsw
	#endif
		//> CSD-030324
		iCt += iReultLineCount;
		//		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		//		iBlk+=iReultBlankSize;
	}
	
	if(!IsEquipAbleClass(	item->GetClassWarriorAble(),item->GetClassThiefAble(),
		item->GetClassArcherAble(),item->GetClassWizardAble(),
		item->GetClassClericAble(),Hero))
	{
		//착용 불가능 이면
		Hcolor( FONT_COLOR_PLUS );
		switch(Hero->class_type)
		{
		case WARRIOR:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(5,125));
			}break;
		case THIEF:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(5,126));
			}break;
		case ARCHER:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(5,127));
			}break;
		case WIZARD:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(5,128));
			}break;
		case PRIEST:
			{
				iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(5,129));
			}break;
		default:
			{
				iReultLineCount = 0;
			}break;
		}
		iCt += iReultLineCount;
	}
	
	const int iNeedLv = 	item->GetNeedLv() ;
	if( iNeedLv )
	{
		is_equip = IsEquipAbleCheckLv(iNeedLv, Hero);
		Hcolor( FONT_COLOR_NAME );
		if( is_equip)	{	Hcolor( FONT_COLOR_BLIGHT_BLUE );	}		//020725 lsw	
		else			{	Hcolor( FONT_COLOR_PLUS );	}
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(7,166),iNeedLv);
		iCt += iReultLineCount;
	}
	
	const int iDualStep = item->GetNeedDualStep();//듀얼 스텝 체크
	if(iDualStep)
	{
		if(IsEquipAbleDualStep(iDualStep,Hero))
		{//착용 가능이면
			Hcolor( FONT_COLOR_BLIGHT_BLUE );
		}
		else
		{//착용 불가능 이면
			Hcolor( FONT_COLOR_PLUS );
		}
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(5,124), iDualStep);
		iCt += iReultLineCount;
	}
	
	const int iNeedFame =	item->GetNeedFame() ;
	if( iNeedFame )
	{
		is_equip = IsEquipAbleCheckFame(iNeedFame, Hero);
		Hcolor( FONT_COLOR_NAME );
		if( is_equip)	{	Hcolor( FONT_COLOR_BLIGHT_BLUE );	}	//020725 lsw	
		else			{	Hcolor( FONT_COLOR_PLUS );	}
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(7,167),iNeedFame);
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	const int iNeedNation = item->GetNeedNation();
	if( iNeedNation )
	{
		char txtnation[100] = "";//	9,424 -- 바이서스	//	9,425 -- 자이펀	//	9,427 -- 일스
		BOOL bIsDraw1 = TRUE,bIsDraw2 = TRUE,bIsDraw3 = TRUE;
		if((iNeedNation%1000)/100)	
		{
			strcat(txtnation, lan->OutputMessage(9,424));
			bIsDraw1 = FALSE;
		}
		
		if((iNeedNation%100)/10)	
		{
			strcat(txtnation,", ");
			strcat(txtnation, lan->OutputMessage(9,425));
			bIsDraw2 = FALSE;
		}
		if((iNeedNation%10)/1)		
		{
			strcat(txtnation,", ");
			strcat(txtnation, lan->OutputMessage(9,427));
			bIsDraw3 = FALSE;
		}
		
		if( bIsDraw1 && bIsDraw2 && bIsDraw3)
		{
			is_equip = IsEquipAbleCheckNation(iNeedNation, Hero);
			Hcolor( FONT_COLOR_NAME );
			iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,12,lan->OutputMessage(7,165) );//lsw
			iCt += iReultLineCount;
			if( is_equip)	{	Hcolor( FONT_COLOR_BLUE );	}		
			else			{	Hcolor( FONT_COLOR_PLUS );	}
			iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,12,txtnation);
			iCt += iReultLineCount;
			
			iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
			iBlk+=iReultBlankSize;
		}
	}
	
	int		iStartDay	= 0;
	int		iEndDay		= 0;
	BOOL	bTimeLimit	= FALSE;
	BOOL	bNeedSkillLv= FALSE;
	int		iNeedSkillLv= 0;
	
	if( item )
	{
		switch(olg_item->item_no/1000)
		{
		case WEAPON	:
			{
				iStartDay	= ((CItem_Weapon*)item)->Repair_Skill2_min;
				iEndDay		= ((CItem_Weapon*)item)->Repair_Res1;
				bTimeLimit	= TRUE;
			}break;
		case ARMOR	:
			{
				iStartDay	= ((CItem_Armor*)item)->Repair_Skill2_min;
				iEndDay		= ((CItem_Armor*)item)->Repair_Res1;
				bTimeLimit = TRUE;
			}break;
		case ACCESSORY:
			{
				iStartDay	= ((CItem_Accessory*)item)->Repair_Skill2_min;
				iEndDay		= ((CItem_Accessory*)item)->Repair_Res1;
				bTimeLimit = TRUE;
			}break;
		case ETC:
			{
				iStartDay	= ((CItem_Etc*)item)->Repair_Skill2_min;
				iEndDay		= ((CItem_Etc*)item)->Repair_Res1;
				
				if( ((CItem_Etc*)item)->GetItemKind() == IK_BOMB||((CItem_Etc*)item)->GetItemKind() == IK_TRAP )
				{
					bNeedSkillLv = TRUE;
					iNeedSkillLv = ((CItem_Etc*)item)->Repair_max;
				}
				bTimeLimit = TRUE;
			}break;
		default:
			break;
		}
	}
	
	if( bTimeLimit && iStartDay && iEndDay)
	{
		is_equip = IsEquipAbleCheckAbleDay(iStartDay, iEndDay);
		Hcolor( FONT_COLOR_NAME );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(5,122));
		iCt += iReultLineCount;
		if( is_equip)	{	Hcolor( FONT_COLOR_BLUE );	}		
		else			{	Hcolor( FONT_COLOR_PLUS );	}
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(5,121),
			iStartDay/100,	iStartDay%100,
			iEndDay/100,	iEndDay%100	);
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	if( bNeedSkillLv && iNeedSkillLv)
	{
		Hcolor( FONT_COLOR_NAME );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(2,529),iNeedSkillLv);
		Hcolor( FONT_COLOR_PLUS );	
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(2,528),iNeedSkillLv);
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	const int damage = item->GetDamage();
	if( damage )
	{
		bool bIsMinus = false;
		int dmg = damage;
		if(dmg > 100000000)
		{
			dmg -= 100000000;
			bIsMinus = true;
		}//021107 lsw
		
		int min =  dmg / 10000;
		int max =  min+ dmg % 10000;    
		if(bIsMinus)
		{
			min *= -1;
			max *= -1;
		}
		
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,141), min, max);
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,142));
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	item->GetItemChangeAbility( need_count, need_type, need_value );
	if( need_count )		// 능력치를 올려주는 아이템일 경우 
	{
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,143) );
		iCt += iReultLineCount;
		
		temp_str[0] = NULL;
		int more = 0;
		
		more = (need_count>>2)+1;// 무조건 한줄은 기본
		
		int iCheckNeedCount =0;
		
		for(int pluslinecount=0; pluslinecount < more ; pluslinecount++)
		{
			for( ; iCheckNeedCount < need_count ;  )
			{
				sprintf( temp_str, "%s %s %+d", temp_str, string_up_point_abilit[need_type[iCheckNeedCount]], need_value[iCheckNeedCount] ); 
				iCheckNeedCount++;
				if(!(iCheckNeedCount%4)){break;}
			}
			Hcolor( FONT_COLOR_PLUS );
			//< CSD-030324			
		#ifdef USA_LOCALIZING_ // thai2 YGI
			Hcolor( FONT_COLOR_NUMBER );
		#endif
			//> CSD-030324
			iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str );
			//< CSD-030324
		#ifndef USA_LOCALIZING_
			Hcolor( FONT_COLOR_NUMBER );
			OnlyNumber( temp_str, strlen( temp_str ) );
			Hprint(x,y+iBlk+iLGab*iCt,g_DestBackBuf,temp_str );
		#endif
			//> CSD-030324
			iCt += iReultLineCount;
			temp_str[0] = NULL;
		}
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	const int ac = item->GetAR_or_DH();
	if( ac )
	{
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,144) , ac );
		
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,145));
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	int rate = item->GetHit_rate();
	if( rate )
	{
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,146) , rate );
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,147));
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	rate = item->GetDefense_rate();
	if( rate )
	{
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,148), rate );
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,149));
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	rate = item->GetMovePoint();
	if( rate )
	{
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,150), rate );
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,151));
		
		if(IK_SADDLE==item->GetItemKind())//
		{
			int iPlusValue = 0;
			const int iMyMovp = SCharacterData.nCharacterAbility[MOVP];
			int nBase =0;
			switch (olg_item->item_no)
			{
			case 8041://말
				{
					iPlusValue = 8*((iMyMovp-10)/30);
				}break;
			case 8042://소
				{
					iPlusValue = 9*((iMyMovp-10)/30);
				}break;
			case 8043://팬텀 HorseNo
				{
					iPlusValue = 10*((iMyMovp-10)/30);
				}break;
			}
			if(iPlusValue)//다해질 값이 있으면
			{
				Hcolor( FONT_COLOR_PLUS );
				iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"           +" );
				Hcolor( FONT_COLOR_YELLOW );
				iReultLineCount = TxtOut.RcTXTOutNoBG(x,y+iBlk+iLGab*iCt,iWidth,0,"             %d",iPlusValue);
			}
			
		}
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	int anti[6]={0,};
	if( item->GetAntiMagic( anti ) )
	{
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(7,152));
		iCt += iReultLineCount;
		
		char anti_name[6][20] = {0, };
		
		for(int ii=0;ii<6; ii++)
		{
			sprintf(anti_name[ii],"%s",lan->OutputMessage(5,ii+107));//lsw
		}
		
		for( int i=0; i<6; i++ )
		{
			if( anti[i] ) 
			{
				if( !i ) strcpy( temp_str, anti_name[i] );
				else sprintf( temp_str, "%s %s", temp_str, anti_name[i] );
				
				sprintf(temp_str, "%s %d", temp_str, anti[i] );
			}
		}
		
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str);
		iCt += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	rate = item->GetItemWeight();
	if( rate )
	{
		sprintf( temp_str, lan->OutputMessage(7,153), rate );
		if( item->GetRButton() == DIVIDE_ITEM )
		{
			sprintf( temp_str, lan->OutputMessage(7,154), temp_str, olg_item->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max, d_curr;
			GetItemDuration( *olg_item, d_curr, d_max );
			sprintf( temp_str, lan->OutputMessage(7,155), temp_str, d_curr/10, d_max/10 );
		}
	}
	else
	{
		if( item->GetRButton() == DIVIDE_ITEM )
		{
			if( kind == IK_MONEY )
				sprintf( temp_str, "%d CELL", olg_item->attr[IATTR_MUCH] );
			if( kind == IK_NEW_MONEY )
				sprintf( temp_str, "%d CRIT", olg_item->attr[IATTR_MUCH] );
			else
				sprintf( temp_str, lan->OutputMessage(7,156), olg_item->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max, d_curr;
			GetItemDuration( *olg_item, d_curr, d_max );
			sprintf( temp_str, lan->OutputMessage(7,157), d_curr/10, d_max/10 );
		}
	}
	
	Hcolor( FONT_COLOR_NUMBER );
	iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str);
	iCt += iReultLineCount;
	
	iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
	iBlk+=iReultBlankSize;
	
	
	if(	(pRareValue->soksung1 && pRareValue->grade)
		||	 pRareValue->iHighLevel == H_LV_LEGEND_ITEM )
	{
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
		DisplayRareExplain(x,y,iCt,iReultLineCount,iBlk,iLGab, iWidth,pRareValue);
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
	}
	
	if(H_LV_LEGEND_ITEM == pRareValue->iHighLevel )
	{
		Hcolor( FONT_COLOR_ORANGE );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,lan->OutputMessage(2,800) );//lsw
		iCt += iReultLineCount;
		const int iNeeds = g_aItemLegend[pRareValue->soksung2].aSub[pRareValue->soksung3].iNeeds;
		
		temp_str[0] = NULL;
		Hcolor( FONT_COLOR_BLIGHT_GREEN );
		if(iNeeds & LEGEND_NEED_WEAPON){sprintf( temp_str, "%s %s", temp_str, lan->OutputMessage(2,801) );}
		if(iNeeds & LEGEND_NEED_SHIELD){sprintf( temp_str, "%s %s", temp_str, lan->OutputMessage(2,802) );}
		if(iNeeds & LEGEND_NEED_HELMET){sprintf( temp_str, "%s %s", temp_str, lan->OutputMessage(2,803) );}
		if(iNeeds & LEGEND_NEED_ARMOR){sprintf( temp_str, "%s %s", temp_str, lan->OutputMessage(2,804) );}
		if(iNeeds & LEGEND_NEED_NECK){sprintf( temp_str, "%s %s", temp_str, lan->OutputMessage(2,805) );}
		if(iNeeds & LEGEND_NEED_SHOES){sprintf( temp_str, "%s %s", temp_str, lan->OutputMessage(2,806) );}
		if(iNeeds & LEGEND_NEED_UNIQUE){sprintf( temp_str, "%s %s", temp_str, lan->OutputMessage(2,807) );}
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str );//lsw
		iCt += iReultLineCount;
	}
	
	rate = item->GetItemClass();
	switch( rate )
	{
	case 2:	
	case 3:	
	case 4:	
	case 5:
	case 6:
	case 7:
	case 8:  rate -= 2; break;
	case 9:	 rate = 8; break;
	default : rate = 0;
	}
	if( rate )
	{
		is_equip = item->is_equipable2( rate );
	}
	else is_equip = 0;
	
	int msg = 0;
	if( is_equip )
	{
		msg = 1;
		strcpy( temp_str, lan->OutputMessage(7,158) );
	}
	else
	{
		switch( item->GetRButton() )
		{
		case REMAIN_ITEM	:	
		case USE_ITEM		:	
			{
				msg = 1;
				strcpy( temp_str, lan->OutputMessage(7,159) ); 
			}break;
		case DIVIDE_ITEM	:	
			{
				msg = 1;
				strcpy( temp_str, lan->OutputMessage(7,160) ); 
			}break;
		case MAGIC_BOOK_ITEM:	
			{
				if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )
				{
					msg = 1;
					strcpy( temp_str, lan->OutputMessage(7,161) ); 
				}
			}break;
		case DIARY_ITEM		:
		case BOX_ITEM		:
			break;
		case TIME_LIMIT		:
			{
				msg = 1;
				strcpy( temp_str, lan->OutputMessage(7,175) ); //011212 lsw
			}
			
		default :				break;
		}
		
	}
	
	switch( item->GetItemKind() )
	{
    case IK_POTION_BOX :
		{
			msg = 1;
			strcpy( temp_str, kein_GetMenuString( 168 ) );
		}break;
	}
	
	if( msg )
	{
		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
		
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str);
		iCt += iReultLineCount;
	}

	// Display the remaining time for timed mall items
	if (olg_item->attr[IATTR_MALL_TIMED] > 0 && item->GetItemKind() != IK_TREASURE_MAP)
	{
		int	d, h, mi, sec;	
		DWORD rest = olg_item->attr[IATTR_MALL_TIMED];

		d		= rest / 86400;		// +1
		rest	= rest % 86400;

		h		= rest / 3600;		//  0 
		rest	= rest % 3600;

		mi		= rest / 60;		//  0
		sec		= rest % 60;

		if (d)
		{
			if (d > 1) 
			{
				sprintf(temp_str, "Time Remaining: %d days %02d:%02d:%02d", d, h, mi, sec);
			}
			else
			{
				sprintf(temp_str, "Time Remaining: %d day %02d:%02d:%02d", d, h, mi, sec);
			}
		}
		else
		{
			sprintf(temp_str, "Time Remaining: %02d:%02d:%02d", h, mi, sec);
		}

		iReultBlankSize = TxtOut.RcBlankOut(x,y+iBlk+iLGab*iCt,iWidth,1);
		iBlk+=iReultBlankSize;
		
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(x,y+iBlk+iLGab*iCt,iWidth,0,temp_str);
		iCt += iReultLineCount;
	}
	
	TxtOut.PutTextTailBGImg(x,y+iBlk+iLGab*iCt,iWidth);//꼬리 테두리
													   /*	if(IK_POTION_BOX == kind)
													   {
													   PutCompressedImageFX( x+back_spr->ox, y+back_spr->oy+count*iLGab, back_spr, trans, 1 );
													   RectTextPut( x+6, y+3+(count-1)*iLGab, back_spr->xl-10, lan->OutputMessage(7,168) );
													   count++;
													   }
	*/
}

int GetItemExplainLine( ItemAttr *olg_item )
{
	int is_equip=0;
	int item_type = olg_item->item_no/1000;
	
	CItem *item = ItemUnit( *olg_item );
	if( !item ) return 0;
	
	int count = 1;
	int need_tatics = item->GetMouse_MRD();
	if( need_tatics && (olg_item->item_no / 1000) ) count++;
	int need_count;
	int need_type[15];
	int need_value[15];
	item->GetItemNeedAbility( need_count, need_type, need_value );
	is_equip = item->is_equipable();
	if( need_count ) count+=2;
	int damage = item->GetDamage();
	if( damage ) count++;
	
	int ac = item->GetAR_or_DH();
	if( ac ) count++;
	int rate = item->GetHit_rate();
	if( rate ) count++;
	rate = item->GetDefense_rate();
	if( rate ) count++;
	rate = item->GetMovePoint();
	if( rate ) count++;
	
	int anti[6]={0,};
	if( item->GetAntiMagic( anti ) ) count+=2;
	count++;
	
	if( is_equip && ( item_type >= TOOL ) ) count++;
	
	return count;
}

void LoadManList()
{
	//char man_list[MAX_MAP_][MAN_LIST_MAX_][20];		// 상점 주인 이름들
	FILE *fp;
	fp = Fopen( "./data/man_list.txt", "rt" );
	if( !fp ) return;
	
	char temp[256];
	int map_index = 0;
	int man_list_index;	
	while( fgets(temp, 256, fp) != NULL )
	{
		if( temp[0] == ';' ) continue;
		if( temp[0] == ':' )
		{
			map_index = atoi( &temp[1] );
			continue;
		}
		char *token;
		token = strtok( temp, "\t\n" );
		if( !token ) continue;
		man_list_index = atoi( token );
		token = strtok( NULL, "\t\n" );
		strcpy( man_list[map_index][man_list_index], token );
	}
	
	fclose( fp );
}

void GetWORDOfDWORD( DWORD dword, WORD &word1, WORD &word2 )
{
	word1 = LOWORD( dword );
	word2 = HIWORD( dword );
}

void PrintMagicExplain( int magic_no )		// +150 되기도 한다.
{
	//	Hero->ready_Magic
	g_CurrUsedMagic = magic_no;
}


struct NO_IMAGE
{
	int type;
	int number;
};

NO_IMAGE no_image[200];
int no_image_max = 0;

void CheckImage( int type, int nImageNumber )
{
	
#ifndef _DEBUG1
	return;
#endif
	
	int i;
	for( i=0; i<no_image_max; i++ )
	{
		if( no_image[i].type == type && no_image[i].number == nImageNumber ) return;
	}
	no_image[no_image_max].type = type;
	no_image[no_image_max++].number = nImageNumber;
	
	char type2name[][30] = {
		"DEFAULT_MENU "
			, "GOD_MENU"
			, "TIME_MENU"
			, "MEM_BOOK"
			, "MAIL_MENU"
			, "START_ETC"
			, "RESET_JOB_ETC"
			, "CHAT_MENU"
			, "MAGIC_BOOK_MENU"
			, "MAIN_ETC"
			, "SMALL_MAP"
			, "START_MAP"
			, "SMALL_ETC"
			, "SKILL_MENU"
			, "PUBLIC_BBS"
			, "HUNTER_MENU"
			, "QUEST_MENU"
			, "FAME_MENU"
			, "CHARACTER_COLOR_CHANGE"
			, "ITEM_MAKE_IMAGE"
			, "NATION_BBS_MENU"
			, "START_FIGHT_MAP"
			, "STOP_FIGHT_MAP"
			, "VOTE_MENU"
			, "GUILD_MARK"
			, "GUILD_ADMIN"
			, "GUILD_LIST"
			, "START_ETC2"
			, "GUILD_ADMIN_ETC"
			, "HELP_PCX"
			, "BANK_PCX"
			, "SALVATION_PCX"
			, "SALVATION_PCX_ANI"
			," CAHNGE_MONEY"
	};
	
	int type2para[] = { 0, 100, 110, 120, 130, 140, 141, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250,
		260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400, 430 };
	
	int max = sizeof( type2para ) / sizeof( int );
	
	int para = -1;
	for( i=0; i<max; i++ )
	{
		if( type2para[i] == type ) 
		{
			para = i;
			break;
		}
	}
	
	FILE *fp;
	fp = Fopen( "./check_image.txt", "at+" );
	if( !fp ) return;
	
	if( para == -1 )
		fprintf( fp, "잘못된 타입 : %26d, %d\n", type, nImageNumber );
	else
		fprintf( fp, "%40s, %5d\n", type2name[para], nImageNumber );
	
	
	fclose( fp );
}


CServerSet server_set[MAX_SERVER_SET];
int server_count;

int CheckSelectServer()
{
	server_count = GetPrivateProfileInt( "network", "server_count", 0, g_szInfoFile );
	
	if( !server_count ) return 0;
	
	char temp_server_str[20];
	for( int i=1; i<=server_count; i++ )
	{
		sprintf( temp_server_str, "name%d", i );
		GetPrivateProfileString("network", temp_server_str, "", server_set[i-1].server_name, 20, g_szInfoFile ); 
		sprintf( temp_server_str, "name%d_host1", i );
		GetPrivateProfileString("network", temp_server_str, "", server_set[i-1].server_ip1, 20, g_szInfoFile ); 
		sprintf( temp_server_str, "name%d_host2", i );
		GetPrivateProfileString("network", temp_server_str, "", server_set[i-1].server_ip2, 20, g_szInfoFile ); 
	}
	
	return server_count;
}

///////////////////////////////////////////////////////////
// CString CMenuString member function start
// 020620 YGI
void CMenuString::AddString( char *add )
{
	char *add2 = EatFrontWhiteChar(add);
	if( str ) delete str;
	int len = strlen( add2 );
	str = new char[len+1];
	strcpy( str, add2 );
}
// CString CMenuString member function end
///////////////////////////////////////////////////////////

void LoadMenuStr()
{
	FILE *fp;
	fp = Fopen( "./data/menustring.txt", "rt" );
	if( !fp ) return;
	
	char temp[1024];
	char *token;
	int index;
	while( fgets( temp, 1024, fp ) )
	{
		if( !temp[0] || temp[0]==' ' || temp[0]=='#' || temp[0]==';' ) continue;
		token = strtok( temp, " \t\n" );
		if( !token )  continue;
		index = atoi( token );
		token = strtok( NULL, "\t\n" );
		g_MenuStr[index].AddString( token );
	}
	fclose( fp );
}

char temp_menu_string[] = "... ...";//021025 ㅣㄵ
char *kein_GetMenuString( int index )
{
	char *ret = g_MenuStr[index].GetString();
	if( ret ) return ret;
	
	return temp_menu_string;
}


/////////////////////////////////////////////////////////////////////////

char SmallTips[ SMALL_TIPS_LINE][ FILENAME_MAX];
void ViewSmallTip()
{
	int X = TIPS_X + 5;
	int Y = TIPS_Y + 5;
	int r,g,b;
	
	Spr *back_spr = GetSprOfMenu( MAIN_ETC, 55 );
	if( !back_spr ) return;
	int i;
	for( i=0; i<14; i++ ) 
	{
		PutCompressedImageFX( back_spr->ox, i*back_spr->yl, back_spr, 20, 1 );		// 배경 화면
		PutCompressedImageFX( back_spr->xl+back_spr->ox, i*back_spr->yl, back_spr, 20, 1 );		// 배경 화면
	}
	
	//	SetHangulAlign( TA_CENTER );
	//	Hprint2( back_spr->xl/2, 5, g_DestBackBuf, "제목 : " );
	//	Line( 10, 320, /*back_spr->xl-10*/800, 320, RGB16( 255, 255, 255 ) );
	
	for( i = 0 ; i < SMALL_TIPS_LINE ; i ++)
	{
		Getcolor( &r,&g,&b );
		Hcolor( 70,70,70 );
		HprintBoldcolor( X+1, Y+1, 30, SmallTips[i] );
		Hcolor( 200,200,200 );
		HprintBoldcolor( X, Y, 100 , SmallTips[i] );
		Hcolor( r,g,b );
		Y += 16;
	}
}

int ViewSmallTipsLoad( int n )
{						
	static int t = -1;
	FILE  *fp;			
	int		i;			
	char s[FILENAME_MAX];
	
	if( n < 0 ) return 0;
	if( t == n ) return 0;
	t = n;
	
	fp = Fopen( "./data/small_tips.txt", "rt" );
	if( fp == NULL )return 0 ;
	int count = 0;
	while( 1 )
	{
		if( fgets( s, FILENAME_MAX, fp ) == NULL ) { fclose(fp); return -1; }
		if( *s == ':' ) 
		{
			if( count == n ) break;
			count ++;
		}
	}
	
	for( i = 0 ; i < SMALL_TIPS_LINE; i ++) SmallTips[i][0] = 0;
	for( i = 0 ; i < SMALL_TIPS_LINE; i ++)
	{
		if( fgets( s, FILENAME_MAX, fp ) == NULL ) { fclose(fp); return -1; }
		if( *s == ':' )  { break; }
		EatRearWhiteChar( s );
		strcpy( SmallTips[i], s );
	}
	
	fclose(fp);
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
// CRankLadder member function
CRankLadder	*g_lpRankLadderBBS;
int CRankLadder::GetRankLadderData( int page, k_get_ladder_score_rank_unit *&t, int &end )//021030 lsw
{
	CRankLadder *target = this;
	while( target )
	{
		if( target->recv )
		{
			if( target->m_Page == page ) 
			{
				t = target->rank_data;
				end = target->is_end;
				return 1;
			}
		}
		target = target->next;
	}
	return 0;
}

int CRankLadder::AddData( int end, k_get_ladder_score_rank_unit *dt )
{
	CRankLadder *target = this;
	CRankLadder *data;
	
	if( target->recv )
	{
		while( target->next ) target = target->next;
		data = new CRankLadder;
		target->next = data;
	}
	else data = target;
	
	data->recv = 1;
	data->is_end = end;
	memcpy( data->rank_data, dt, sizeof( k_get_ladder_score_rank_unit )*10 );
	data->m_Page = data->rank_data[0].rank/10 + 1;
	return 1;
}
int CRankLadder::SetFinish()
{
	CRankLadder *target = this;
	CRankLadder *lpLastPage = this;
	int max_page = -1;
	while( target ) 
	{
		if( target->recv )
		{
			if( target->m_Page > max_page )
			{
				lpLastPage = target;
				max_page = target->m_Page;
			}
		}
		target = target->next;
	}
	lpLastPage->recv = 1;
	lpLastPage->is_end = 1;
	return 1;
}

int CRankLadder::SearchName( char *name )
{
	CRankLadder *target = this;
	while( target)
	{
		if( target->recv )
		{
			for( int i=0; i<10; i++ )
			{
				if( strcmp( target->rank_data[i].name, name ) == 0 )
					return target->m_Page;
			}
		}
		target = target->next;
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

void LogFile( char *text )
{
	//return;
	FILE *fp;
	fp = fopen( "logfile.txt", "at+" );
	if( fp )
	{
		fprintf( fp, "%s\n", text );
		fclose( fp );
	}
	else JustMsg( "실패" );
}

int LoadScale()
{
	FILE *fp;
	fp = Fopen( "./data/scale_bbs.txt", "rt" );
	if( !fp ) return 0;
	
	char temp[2048];
	
	int len = 0;
	while( fgets( temp, 2048, fp  ) )
	{
		len += strlen( temp );
	}
	fseek( fp, 0, SEEK_SET );
	
	g_szScale = new char[len+1];
	g_szScale[0] = NULL;
	
	while( fgets( temp, 2048, fp  ) )
	{
		//rintf( g_szScale, "%s\n%s", g_szScale, temp );
		strcat( g_szScale, temp );
	}
	int t = strlen( g_szScale );
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) && len < t )
	{
		JustMsg( "LoadScale() Func Error" );
	}
#endif
	fclose( fp );
	return 1;
}

int LoadDefaultStringByMenu( int menu, char *&str )
{
	switch( menu )
	{
	case MN_SCALE : 
		if( !g_szScale )
		{
			if( !LoadScale() ) return 0;
		}
		str = g_szScale;
		break;			
	}
	
	return 0;
}

void Kein_PutMessage( int type, char *msg, ... )
{	
	if( g_StartMenuOn ) return;
	char temp[ MAX_PATH];
	static int count;
	
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf(temp, msg, arg );
	}
    va_end( arg );
	
	switch( type )
	{
	case KM_INFO:		// 정보 출력 ( 약간 하얀 )
		MP3( SN_INFO );
		AddCurrentStatusMessage( 230, 230, 230, temp );
		break;
	case KM_FAIL:		// 실패 ( 붉은 )
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 250,60,60, temp );
		break;
	case KM_OK:		// 성공 ( 노란빛 )
		MP3( SN_LEARN_SKILL );
		AddCurrentStatusMessage( 250,250,60, temp );
		break;
	case KM_BBS:	// 공지 ( 노랑 )
		MP3( SN_RECV_BBS );
		AddCurrentStatusMessage( 250, 250, 0, temp );
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
// 문자열에서 특정 문자를 원하는 문자로 교체
char *ChangeString( char *str, char sor, char tag )
{
	char *temp = str;
	while( temp = strchr( temp, sor ) ) *temp = tag;
	return str;
}

char *ChangeString2( char *str, char *sor, char *tag )
{
	char *temp = str;
	while( temp = strstr( temp, sor ) ) strncpy( temp, tag, strlen(sor) );
	return str;
}

////////////////////////////////////////////////////////////////////////////////////////////
int CBitBuf::LoadBMP( char * file_name )
{
	BITMAPFILEHEADER bmpHeader;
	LPBITMAPINFO pBitmapInfo;
	WORD RGBTemp;
	int iBitType, i, j;
	DWORD size;
	
	BYTE *tempbuf= NULL;
	RGB24 *tempbuf2= NULL;
	WORD *tempbuf3= NULL;
	
	RGBQUAD *Pal;
	WORD r, g, b;
	
	FILE *fp;
	fp = fopen( file_name, "rb" );
	if( !fp ) return 0;
	
	fread( &bmpHeader, sizeof( BITMAPFILEHEADER ), 1, fp );
	if( bmpHeader.bfType != DIB_HEADER_MARKER ) 
	{
		fclose( fp );
		return -1;
	}
	
	size = bmpHeader.bfSize-sizeof( bmpHeader );
	int size2 = bmpHeader.bfOffBits - sizeof( BITMAPFILEHEADER );
	
	char *tDib = new char[size];
	fread( tDib, size, 1, fp );
	
	pBitmapInfo = (LPBITMAPINFO)tDib;
	BITMAPINFOHEADER *BitmapInfoHeader=(BITMAPINFOHEADER *)tDib;
	iBitType = pBitmapInfo->bmiHeader.biBitCount/8;
	int line_size;	// bmp 구조에서 한라인이 사용하는 실제 byte의 수
	WORD keycolor;
	switch( iBitType )
	{
	case 0 :
		fclose( fp );
		delete pBitmapInfo;
		return -2;
	case 1:
		tempbuf = (BYTE *)(tDib+ *(LPDWORD)tDib+( 256*sizeof(RGBQUAD)));
		Pal = new RGBQUAD[256];
		memcpy(Pal, pBitmapInfo->bmiColors, sizeof( RGBQUAD ) * 256);
		
		line_size = (((pBitmapInfo->bmiHeader.biWidth%4)?1:0)+pBitmapInfo->bmiHeader.biWidth/4)*4;
		for( i=0; i<pBitmapInfo->bmiHeader.biHeight; i++ )
			for( j=0; j<pBitmapInfo->bmiHeader.biWidth; j++ )
			{
				int para = i*(line_size)+j;
				
				r = Pal[ tempbuf[para] ].rgbRed;
				g = Pal[ tempbuf[para] ].rgbGreen;
				b = Pal[ tempbuf[para] ].rgbBlue;
				
				para = (pBitmapInfo->bmiHeader.biHeight-i-1)*pBitmapInfo->bmiHeader.biWidth+j;
#ifdef _DEBUG
				if( para >= MAX_RAW_BUF_X*MAX_RAW_BUF_Y )
				{
					_asm int 3;
				}
#endif
				
				m_pBmpBuf[para] = ::RGB16(r, g, b);
			}
			keycolor = ::RGB16(0, 255, 0);
			delete Pal;
			break;
			
	case 2:
		fclose( fp );
		delete pBitmapInfo;
		return -2;
		tempbuf3 = (WORD *)(tDib+ *(LPDWORD)tDib);
		//tempbuf3  = new	WORD[ ImageSize ];
		//fread( tempbuf3, 1, ImageSize, fp );
		
		for (i = 0; i < pBitmapInfo->bmiHeader.biHeight; i++)
		{
			for (j = 0; j < pBitmapInfo->bmiHeader.biWidth; j++)
			{
				RGBTemp = tempbuf3[(i * pBitmapInfo->bmiHeader.biWidth) + j];
				
				r = g = b = RGBTemp;
				b = ((b << 10) >> 11) << 3;
				g = ((g << 5) >> 11) << 3;
				r = (r >> 11) << 3;
				
				m_pBmpBuf[ (i * pBitmapInfo->bmiHeader.biWidth) + j ] = RGB16(r, g, b);//_RGB16BIT565( r, g, b);//RGB16(r, g, b);//RGBTemp;//RGB16(r, g, b);
			}
		}
		//delete tempbuf3;
		break;
		
	case 3:
		{
			tempbuf2 = (RGB24*)(tDib + *tDib );
			char *start = (char*)tempbuf2;
			
			line_size = ((((pBitmapInfo->bmiHeader.biWidth*3)%4)?1:0)+pBitmapInfo->bmiHeader.biWidth*3/4)*4;
			for (i = 0; i < pBitmapInfo->bmiHeader.biHeight; i++)
			{
				tempbuf2 = (RGB24*)(start+ i*line_size );
				for (j = 0; j < pBitmapInfo->bmiHeader.biWidth; j++)
				{
					r = tempbuf2[j].r;
					g = tempbuf2[j].g;
					b = tempbuf2[j].b;
					
					int pos = ((pBitmapInfo->bmiHeader.biHeight-1-i) * (pBitmapInfo->bmiHeader.biWidth)) + j;
#ifdef _DEBUG
					if( pos >= MAX_RAW_BUF_X*MAX_RAW_BUF_Y )
					{
						_asm int 3;
					}
#endif
					m_pBmpBuf[pos] = RGB16(r, g, b);
				}
			}
			
			keycolor = RGB16(0, 255, 0);
			break;
		}
	}
	
	m_iImageX = pBitmapInfo->bmiHeader.biWidth;
	m_iImageY = pBitmapInfo->bmiHeader.biHeight;
	
	m_Spr.size = DeviceBmp16( m_pBmpBuf, m_pComBuf, keycolor );
	m_Spr.img = (char *)m_pComBuf;
	//convert565to555( &m_Spr );
	delete tDib;
	
	fclose( fp );
	return 1;
}
void GetMax( int x, int y, int *sx, int *sy, int *ex, int *ey )
{
	if( *sx > x ) *sx = x;
	if( *ex < x ) *ex = x;
	if( *ey < y ) *ey = y;
	if( *sy > y ) *sy = y;
}

int CBitBuf::DeviceBmp16( WORD *pBmpbuf, WORD *compressbuf, WORD keycolor )
{
    int x1, y1, x2, y2, x, y;
	int offy;
	int first = 0;
	
    keycolor = RGB16( 0, 255, 0 );		// 투명값
	
    for( y = 0; y < m_iImageY; y ++ )
    {
		//offy = y * MAX_ALIAS_BUFFER_X;
		offy = y * m_iImageX;
		for( x = 0; x < m_iImageX ; x++ )
		{
			if( pBmpbuf[ offy + x ] != keycolor)
			{
				if( first == 0) 
				{
					first = 1;
					x1 = x2 = x, y1 = y2 = y;
				}
				else
				{
					GetMax( x, y, &x1, &y1, &x2, &y2 );
				}
			}
		}
	}
	
	if( first == 0 ) return 0;
	if( x2 <= x1 ) return 0;
	if( y2 <= y1 ) return 0;
	
	m_Spr.xl = x2 - x1 + 1;
	m_Spr.yl = y2 - y1 + 1;
	
	m_Spr.ox  = m_Spr.xl >> 1;
	m_Spr.oy  = m_Spr.yl >> 1;
	
	return ConvertImgFromSurface( compressbuf, pBmpbuf, m_iImageX, m_iImageY, keycolor );
}

int CBitBuf::ConvertImgFromSurface( WORD *buffer, WORD *sour, int xl, int yl, int keycolor)
{
	LPWORD			tmp;
	WORD			*dest,clr;
	BOOL			FL=FALSE;
	DWORD			count=0,bufcnt=0;
	
	dest = buffer;
	tmp = sour;
	
	int top, bottom, left, right;
	
	top = 0; bottom = yl-1;
	left = 0; right = xl-1;
	
	int j;
	for(int i= top; i<= bottom; i++)
	{
		FL = FALSE;
		count = 0;
		for(j=left; j<= right; j++)
		{
			clr = *(tmp + i*xl + j );
			if((FL == FALSE) && (clr != keycolor))
			{
				FL = TRUE;
				dest[bufcnt] = WORD(count << 1);
				bufcnt++;
				count=0;
			}
			else
			{
				if((FL == TRUE) && (clr == keycolor))
				{
					FL = FALSE;
					dest[bufcnt] = WORD(count << 1);
					bufcnt+=1;
					for(; count >0; count--)
					{
						dest[bufcnt++] = *(tmp + i*xl + (j - count));
					}
					count = 0;
				}	
			}		
			count++;
		}			
		if(FL == TRUE)
		{			
			dest[bufcnt] = WORD(count << 1);
			bufcnt+=1;
			for(; count > 0; count--)
			{		
				dest[bufcnt++] = *(tmp + i*xl + (j - count));
			}		
		}
		dest[bufcnt] = 0xffff;
		bufcnt+=1;
	}				
	
	return bufcnt*2 ;
}

WORD CBitBuf::RGB16(WORD r, WORD g, WORD b)
{
	r = r >> 3;
	b = b >> 3;
	if( _PixelInfo.BitMaskR != 0x7C00)
	{
		g = g >> 2;
		return ((b)<< 11) | (g)<<5 |(r);
	}
	
	g = g >> 3;
	return ((b)<< 10) | (g)<<5 |(r);
}


////////////////////////////////////////////////////////////////////////////////////////////

int CallGuildExplain( int type )		// 길드 설명 메뉴 호출 하기
{
	char *szFileName = NULL;
	switch( type )
	{
	case GE_REGIST_IMSI_GUILD : 
		{
			SMenu[MN_GUILD_EXPLAIN].nField[1].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[2].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[3].nType=0;		// done 막기
			SMenu[MN_GUILD_EXPLAIN].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[6].nType=0;
			SMenu[MN_GUILD_EXPLAIN].nField[0].nImageNumber=45;
			SMenu[MN_GUILD_EXPLAIN].nField[10].nType = 0;
			szFileName = "./data/guild_explain2.txt";
			break;
		}
	case GE_REGIST_GUILD : 
		{
			SMenu[MN_GUILD_EXPLAIN].nField[1].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[2].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[3].nType=0;		// done 막기
			SMenu[MN_GUILD_EXPLAIN].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[6].nType=0;
			SMenu[MN_GUILD_EXPLAIN].nField[0].nImageNumber=46;
			SMenu[MN_GUILD_EXPLAIN].nField[10].nType = 0;
			szFileName = "./data/guild_explain3.txt";
			break;
		}
	case GM_INPUT_SUB_MASTER : 
		{
			SMenu[MN_GUILD_EXPLAIN].nField[1].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[2].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[3].nType=0;		// done 막기
			SMenu[MN_GUILD_EXPLAIN].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[6].nType=0;
			SMenu[MN_GUILD_EXPLAIN].nField[0].nImageNumber=46;
			SMenu[MN_GUILD_EXPLAIN].nField[10].nType = FT_INPUT_GUILD_MASTER;
			szFileName = "./data/guild_explain4.txt";
			break;
		}
	case GE_REGIST_GUILD_MARK :
		SMenu[MN_GUILD_EXPLAIN].nField[1].nType=FT_NOMAL_PUT;
		SMenu[MN_GUILD_EXPLAIN].nField[2].nType=FT_NOMAL_PUT;
		SMenu[MN_GUILD_EXPLAIN].nField[3].nType=0;		// done 막기
		SMenu[MN_GUILD_EXPLAIN].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
		SMenu[MN_GUILD_EXPLAIN].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
		SMenu[MN_GUILD_EXPLAIN].nField[6].nType=0;
		SMenu[MN_GUILD_EXPLAIN].nField[0].nImageNumber=40;
		SMenu[MN_GUILD_EXPLAIN].nField[10].nType = 0;
		szFileName = "./data/guild_explain5.txt";
		break;
		
	case GE_REGIST_GUILD_MENUAL :
		{
			SMenu[MN_GUILD_EXPLAIN].nField[1].nType=0;
			SMenu[MN_GUILD_EXPLAIN].nField[2].nType=0;
			SMenu[MN_GUILD_EXPLAIN].nField[3].nType=FT_NOMAL_PUT;		// done 막기
			SMenu[MN_GUILD_EXPLAIN].nField[4].nType=0;
			SMenu[MN_GUILD_EXPLAIN].nField[5].nType=0;
			SMenu[MN_GUILD_EXPLAIN].nField[6].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[0].nImageNumber=44;
			SMenu[MN_GUILD_EXPLAIN].nField[10].nType = 0;
			szFileName = "./data/guild_explain1.txt";
			break;
		}
	case GM_CHANGE_INFO_MENUAL : 
		{
			SMenu[MN_GUILD_EXPLAIN].nField[1].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[2].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[3].nType=0;		// done 막기
			SMenu[MN_GUILD_EXPLAIN].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_GUILD_EXPLAIN].nField[6].nType=0;
			SMenu[MN_GUILD_EXPLAIN].nField[0].nImageNumber=39;
			SMenu[MN_GUILD_EXPLAIN].nField[10].nType = 0;
			szFileName = "./data/guild_explain6.txt";
			break;
		}
	}
	
	if( is_mainmenu_active ) CallSmallMenu( MN_GUILD_EXPLAIN );
	else 
	{
		SMenu[MN_GUILD_EXPLAIN].bActive = true;
		y_MenuFocus = MN_GUILD_EXPLAIN;
	}
	int line = g_Menu_Variable.LoadingGuildRegistExplain( szFileName );
	WORD page =0;
	WORD page_max = line/5+1;
	SMenu[MN_GUILD_EXPLAIN].work = MAKELONG( page, page_max );
	SMenu[MN_GUILD_EXPLAIN].key = line;
	
	SMenu[MN_GUILD_EXPLAIN].nTemp = type;
	return 1;
}

// 다른 국가 사람을 클릭할수 없는 상황이냐?
int g_OtherNationClick;
bool CheckNotClickNation()
{
	if (!g_OtherNationClick && MapInfo[ MapNumber].nation)
	{
		return true;	// 국가전 시간이 아닌데 나라맵에 속해 있는 경우
	}
	
	return false;
}

void CallGuildInfoMenu( int menu, short int guild_code, int if_guild, int info_type /*변경타입*/ )
{	//< CSD-030324
	SMenu[MN_GUILD_REGIST].nTemp = menu;
	SMenu[MN_GUILD_REGIST].key = info_type;
	SMenu[MN_GUILD_REGIST].bActive = true;
	y_MenuFocus = MN_GUILD_REGIST;
	CGuild* pGuild = &g_GuildMgr[guild_code];
	SMenu[MN_GUILD_REGIST].nField[2].m_lpTemp =	g_GuildMgr[guild_code].m_szName;
	SMenu[MN_GUILD_REGIST].nField[3].m_lpTemp = g_GuildMgr[guild_code].m_szGuldMasterName;
	
	if( if_guild ) 
	{
		SMenu[MN_GUILD_REGIST].nField[4].m_lpTemp = GetGuildSpr( guild_code );
	}
	else
		SMenu[MN_GUILD_REGIST].nField[4].m_lpTemp = &guild_mark_main.m_Spr;
	
	if( !g_GuildMgr[guild_code].m_szExplain )
	{
		SendDirectDB( CMD_REQ_GUILD_EXPLAIN, &guild_code, sizeof( short int ) );
		SMenu[MN_GUILD_REGIST].nField[5].m_lpTemp = NULL;
	}
	else
	{
		// RecvGuildExplain() 함수에 똑같은 부분 있음
		// 필드 번호 수정시 같이 바꿔줘야 한다.
		SMenu[MN_GUILD_REGIST].nField[5].m_lpTemp = g_GuildMgr[guild_code].m_szExplain;
	}	
}	//> CSD-030324

inline Spr *GetGuildSpr( int guildCode )
{	//< CSD-030324
	if( g_GuildMgr[guildCode].m_Spr.img ) 
	{
		return &g_GuildMgr[guildCode].m_Spr;
	}

	return GetSprOfMenu( GUILD_MARK, guildCode );
}	//> CSD-030324

inline int GetSysInfo( int type )
{
#ifdef _DEBUG
	switch( type )
	{
	case SI_GAME_MAKE_MODE : return SysInfo.gamemakemode;
	case SI_NOT_CONNECT_MODE : return SysInfo.notconectserver;
	}
#endif
	return 0;
}

void GetItemByPotionBox( int &item_no, int &much )
{
	item_no = LOWORD( SCharacterData.potion_box1 );
	much = HIWORD( SCharacterData.potion_box1 );
}

void CheckBetaForFileName( char *filename )// 메타냐 상용에 따라서 파일 이름을 바꿔주는 루틴
{
	if(checkbeta == 2 )//한번만 호출되는 부분.. 어째되던 값이 0또는 1로 
	{
		FILE *fp = fopen("./beta.ini","r");
		if(fp!=NULL)
		{
			fread( &checkbeta, 4,4,fp );
			fclose( fp );
			
			if(checkbeta=='1')
			{	//< CSD-031030
				fp = fopen( "./data_b5/beta.txt", "r" );
				if( !fp )
				{
					MessageBox( GetActiveWindow(), "프리 베타 서버로 접속하려면 홈페이지에서 꼭 패치파일을 다운 받아야 합니다. \n http://www.dr-online.co.kr", "Information", MB_OK | MB_ICONINFORMATION );
					exit(0);
				}
				else fclose( fp );
				DeleteBetaFile( "./delete_dir.txt" );
			}	//> CSD-031030
		}
		else
		{
			checkbeta='0';
		}
	}
	if( checkbeta == '1' )
	{
		char save_name[_MAX_PATH];
		char result_name[_MAX_PATH];
		strcpy( save_name, filename );
		char *work_name = save_name;
		char *temp;
		if( *GetCurrentWorkingDirectory() )
		{			
			temp = strstr( save_name, GetCurrentWorkingDirectory() );
			if( temp ) 
			{
				work_name = temp + strlen( GetCurrentWorkingDirectory() )+1;
			}
		}
		char *conv_slash = work_name;
		while( conv_slash = strchr( conv_slash, '\\' ) )
		{
			*conv_slash = '/';
		}
		
		temp = strstr( work_name, "./" );
		if( temp ) work_name += 2;
		
		char *token = strstr( work_name, "/" );
		if( token )		// / 가 하나도 없는 경우를위해서.. 
		{	//< CSD-031030
			*token=0;
			token++;
			sprintf( result_name, "%s_b5/%s", work_name, token );
		}	//> CSD-031030
		FILE *fp = fopen( result_name, "rt" );
		if( fp )
		{
			strcpy( filename, result_name );
			fclose(fp);
			return;
		}
	}
}

#include <tlhelp32.h>
struct k_string_basic
{
	char m_pStr[50];
	
	k_string_basic()
	{
		m_pStr[0] = 0;
	}
	bool isEmpty() { return m_pStr[0]?false:true; }
	char *c_str(){ return m_pStr; }
};

#define CHECK_HACKING_TOOL_VER	1023
template < typename T, int N > class CString_kein
{
public :
	T *m_pData;
	CHSEL_STREAM m_hsel;
	int *m_npCheck;
	int m_nError;
	
	union
	{
		struct
		{
			__int16 m_nIndex;
			__int16 m_nMagic;
		};
		
		DWORD m_dwHandle;
		
	};
	
public :
	CString_kein()
	{
		m_nError = 0;
		m_pData = new T[N];
		m_npCheck = new int[N];
	}
	~CString_kein()
	{
		delete [] m_pData;
		delete [] m_npCheck;
	}
	int Check( int index )
	{
		int temp = 0;
		int max = sizeof(T);
		for( int i=0; i<max; i++ ) temp += ((char*)&m_pData[index])[i];
		temp += CHECK_HACKING_TOOL_VER;
		return temp;
	}
	void AddNew(int nKey, T* pT)
	{
		memcpy( &m_pData[nKey], pT, sizeof( T ) );
		m_npCheck[nKey] = Check( nKey );
	}
	int Save( const char* pFile)
	{
		ofstream fout(pFile, ios::out|ios::binary);
		HselInit eninit;
		eninit.iEncryptType	=	HSEL_ENCRYPTTYPE_RAND;
		eninit.iDesCount = HSEL_DES_TRIPLE;
		eninit.iCustomize	=	HSEL_KEY_TYPE_DEFAULT;
		eninit.iSwapFlag = HSEL_SWAP_FLAG_ON;
		if (!m_hsel.Initial(eninit))  return false;
		HselInit deinit;
		deinit = m_hsel.GetHSELCustomizeOption();
		const int nVersion = m_hsel.GetVersion();
		fout.write((char*)(&nVersion), sizeof(nVersion)); 
		fout.write((char*)(&deinit), sizeof(HselInit)); 
		
		for (int i = 0; i < N; ++i)	
		{
			T* pT = &m_pData[i];
			m_nMagic = Check( i );
			m_hsel.Encrypt((char*)pT, sizeof(T));
			fout.write((char*)(pT), sizeof(T));
			m_nIndex = m_hsel.GetCRCConvertShort();
			fout.write((char*)(&m_dwHandle), sizeof(m_dwHandle));
		}
		
		fout.close();
		return true;
	}
	bool Load(const char* pFile)
	{
		ifstream fin(pFile, ios::in|ios::binary);
		int nVersion = 0;
		fin.read((char*)(&nVersion), sizeof(nVersion)); 
		if(m_hsel.GetVersion() != nVersion)  return false;
		HselInit deinit;
		fin.read((char*)(&deinit), sizeof(HselInit));
		if( !m_hsel.Initial(deinit) )  return false;
		for (int i = 0; i < N; ++i)	
		{ 
			T tempT;
			fin.read((char*)(&tempT), sizeof(T));
			fin.read((char*)(&m_dwHandle), sizeof(m_dwHandle));
			m_hsel.Decrypt((char*)(&tempT), sizeof(T));
			if (m_hsel.GetCRCConvertShort() != m_nIndex)  return false;
			AddNew( i, &tempT ); 
			if (Check(i) != m_nMagic)  return false;
		}
		fin.close();
		return true;
	}
	T& operator[](int nIndex)
	{ 
		m_nError += abs(m_npCheck[nIndex] - Check(nIndex));
		return (m_pData[nIndex + m_nError]);
	}
};


#define MAX_HACKING_TOOL_NAME	50
CString_kein < k_string_basic,MAX_HACKING_TOOL_NAME > g_HackingToolName;
int hackingtoolcount;


extern bool CheckContinueLine( char check );
int LoadHackingToolName()
{
	char filename[_MAX_PATH] =		"./text/lightning.txt";
	char filename_bin[_MAX_PATH]=	"./effect/lightning.ab";
	CheckBetaForFileName( filename );
	CheckBetaForFileName( filename_bin );
#ifdef _DEBUG
	FILE *fp = Fopen( filename, "rt" );
	if( fp )
	{
		char temp[128];
		while( fgets( temp, 128, fp ) )
		{
			if( CheckContinueLine( temp[0] ) ) continue;
			EatRearWhiteChar( temp );
			CharUpper( temp );
			
			k_string_basic temp_string; 
			strcpy( temp_string.m_pStr, temp );			
			g_HackingToolName.AddNew( hackingtoolcount, &temp_string );
			
			hackingtoolcount++;
			if( hackingtoolcount >= 50 )
			{
				JustMsg( "%s : error! over max ( 50 )", filename );
				break;
			}
		}
		fclose( fp );
		g_HackingToolName.Save( filename_bin );
	}
#endif
	
	g_Menu_Variable.m_CheckProcess = new CAlreadyCheck;
	return g_HackingToolName.Load( filename_bin );
}

bool CompHackingFile( PROCESSENTRY32 *pEntry )
{
	// 비교한적이 있다.
	if( g_Menu_Variable.m_CheckProcess->AddProceessId( pEntry->th32ProcessID ) == 0 ) return true;
	
	CharUpper( pEntry->szExeFile );
	for( int i=0; !g_HackingToolName[i].isEmpty() ; i++ )
	{
		char *temp = g_HackingToolName[i].c_str();
		if( strstr( pEntry->szExeFile, temp ) ) 
		{
			g_DBGLog.Log(LOG_LV1,"Error At CompHackingFile Exe->%s Tool->%s",pEntry->szExeFile,temp);
			return false;
		}
	}
	return true;	
}

bool CheckHackingTool()
{
	HANDLE hSnapShot;
	hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);
	
	PROCESSENTRY32 pEntry;
	pEntry.dwSize =sizeof(pEntry);
	
	//Get first process
	if( !Process32First(hSnapShot,&pEntry) ) goto END__;
	
	do
	{
		//th32ProcessID
		if( !CompHackingFile( &pEntry ) ) goto END__;
	}while(Process32Next(hSnapShot,&pEntry));
	
	
	//DeleteObject( hSnapShot );
	CloseHandle( hSnapShot );
	return true;
	
END__ :
	//DeleteObject( hSnapShot );
	CloseHandle( hSnapShot );
	return false;
}

// 020428 YGI acer
void CheckingFilter()
{
#ifdef _DEBUG1
	if( CheckSumError ) return;
	if( FindWindow( "GameX_FixGame", NULL ) ) 
	{
		g_DBGLog.Log(LOG_LV1,"GameX_FixGame Window Found");
		goto ERROR1_ ;
	}
	if( !CheckHackingTool() ) 
	{
		g_DBGLog.Log(LOG_LV1,"Captured Hacking Tool");
		goto ERROR1_;
	}
	
	return;
ERROR1_ :
	CheckSumError=20007;
	return;
#endif
}


void CheckingAccelator_Sub1( void )
{
	static DWORD curr_time = g_curr_time;
	if( g_curr_time - curr_time < 5 ) return;
	curr_time = g_curr_time;
	
	CheckingFilter();
}
////////////////////////////////////////////////////////////////////////////


//acer4
void PutFieldOtherCharInfo()
{	//< CSD-030324
	int x = SMenu[MN_DEFAULT_MESSAGEBOX2].x;
	int y = SMenu[MN_DEFAULT_MESSAGEBOX2].y;
	if( !g_Menu_Variable.m_pOtherChar ) return;
	
	k_char_info *pData = g_Menu_Variable.m_pOtherChar;
	
	//Hcolor( 0);
	int plus_x = 40;
	int plus_y = 100;
	int a = 0;
	Hprint2( x+plus_x, y+plus_y+a*18, NULL, "Name : %s( Lv : %d )", pData->name, pData->btLevel ); a++; // CSD-030806
	Hprint2( x+plus_x, y+plus_y+a*18, NULL, "(%s:%d,%d)", MapInfo[pData->map_number].mapname
		, pData->x, pData->y );	a++;
	CCharRank name_status;
	memcpy( &name_status, &pData->name_status, sizeof( DWORD ) );
	Hprint2( x+plus_x, y+plus_y+a*18, NULL, "Nation : %s", GetNationName(name_status.nation ) ); a++;
	Hprint2( x+plus_x, y+plus_y+a*18, NULL, "Guild  : %s", GetGuildName(pData->guild_code ));	a++;
	Hprint2( x+plus_x, y+plus_y+a*18, NULL, "NK : %d", pData->nk );	a++;
}	//> CSD-030324

#define MAX_TEST_ANIMATION	200
BYTE	*test_animation_buf1;
Spr		test_animation_spr1[MAX_TEST_ANIMATION];
BYTE	*test_animation_buf2;
Spr		test_animation_spr2[MAX_TEST_ANIMATION];
BYTE	*test_animation_buf3;
Spr		test_animation_spr3[MAX_TEST_ANIMATION];
BYTE	*test_animation_buf4;
Spr		test_animation_spr4[MAX_TEST_ANIMATION];

BYTE	*test_animation_buf5;
Spr		test_animation_spr5[MAX_TEST_ANIMATION];


void FreeTestAnimation()
{
	if( test_animation_buf1 )
	{
		FreeOpenningSpr( &test_animation_buf1 );
		for( int i=0; i<MAX_TEST_ANIMATION; i++ )
		{
			test_animation_spr1[i].img = 0;
		}
	}
	if( test_animation_buf2 )
	{
		FreeOpenningSpr( &test_animation_buf2 );
		for( int i=0; i<MAX_TEST_ANIMATION; i++ )
		{
			test_animation_spr2[i].img = 0;
		}
	}
	if( test_animation_buf3 )
	{
		FreeOpenningSpr( &test_animation_buf3 );
		for( int i=0; i<MAX_TEST_ANIMATION; i++ )
		{
			test_animation_spr3[i].img = 0;
		}
	}
	if( test_animation_buf4 )
	{
		FreeOpenningSpr( &test_animation_buf4 );
		for( int i=0; i<MAX_TEST_ANIMATION; i++ )
		{
			test_animation_spr4[i].img = 0;
		}
	}
	if( test_animation_buf5 )
	{
		FreeOpenningSpr( &test_animation_buf5 );
		for( int i=0; i<MAX_TEST_ANIMATION; i++ )
		{
			test_animation_spr5[i].img = 0;
		}
	}
}


int iSelectedCharacter = -1;
bool PutTestAnimation(const int iGender, const int iClass, const DWORD dwBodyRGB, DWORD dwLineRGB )
{
	static const int iGabX[2][5]={{-12,-33,0,-23,-20},{2,-23,-8,-13,-53}};
	static const int iGabY[2][5]={{68,15,0,53,63},{-31,-1,-29,-34,-37}};
	//	FEMALE  0	//	MALE	1
	//	WARRIOR		0//	THIEF		1//	ARCHER		2//	WIZARD		3//	PRIEST		4
	//	static iSelected = -1;
	
	if( iGender != FEMALE && iGender != MALE)
	{
		return false;
	}
	if(WARRIOR > iClass ||PRIEST < iClass)
	{
		return false;
	}
	
	const int iOldSelected = iSelectedCharacter;
	iSelectedCharacter = iGender*10 + iClass;
	
	if(iOldSelected !=iSelectedCharacter)//선택한 에니메이션이 다르다면
	{
		FreeTestAnimation();
		
		char filename1[MAX_FILENAME] = "";
		char filename2[MAX_FILENAME] = "";
		char filename3[MAX_FILENAME] = "";
		char filename4[MAX_FILENAME] = "";
		
		sprintf(filename1,"./char/CharAniBig1%1d%1d.spr",iGender,iClass);
		sprintf(filename2,"./char/CharAniBig2%1d%1d.spr",iGender,iClass);
		sprintf(filename3,"./char/CharAniBig3%1d%1d.spr",iGender,iClass);
		sprintf(filename4,"./char/CharAniBig4%1d%1d.spr",iGender,iClass);
		
		LoadOpenningSpr( filename1, &test_animation_buf1, test_animation_spr1 );
		LoadOpenningSpr( filename2, &test_animation_buf2, test_animation_spr2 );
		LoadOpenningSpr( filename3, &test_animation_buf3, test_animation_spr3 );
		LoadOpenningSpr( filename4, &test_animation_buf4, test_animation_spr4 );
	}
	
	static int x = 400;
	static int y = 300;
	int iX = x+iGabX[iGender][iClass];
	int iY = y+iGabY[iGender][iClass];
	static int count;
	if( test_animation_buf4 )
	{
		
		PutCompressedImage( iX, iY, &test_animation_spr1[count] );
		if( test_animation_buf2 )
		{
			PutCompressedImageCharRGB( iX, iY, &test_animation_spr2[count], dwLineRGB);//020706 lsw
		}
		if( test_animation_buf3 )
		{
			PutCompressedImageCharRGB( iX, iY, &test_animation_spr3[count], dwBodyRGB);//020706 lsw
		}
		PutCompressedImageFX( iX, iY, &test_animation_spr4[count], 17, 2 );
		count++;
	}
	
	if( !test_animation_spr1[count].img ) count = 0;
	return 1;
}

bool PutTestAnimation2(const int iGender, const int iClass, const DWORD dwBodyRGB, DWORD dwLineRGB )
{
	static const int iGabX2[2][5]={{-12, -20, 3, -15, -14},
	{149 ,148 ,149 ,-16 ,-25}};
	static const int iGabY2[2][5]={{68, 39, 38, 32, 73},
	{163, 164, 154, 27, 34}};
	
	//	FEMALE  0	//	MALE	1
	//	WARRIOR		0//	THIEF		1//	ARCHER		2//	WIZARD		3//	PRIEST		4
	static int iSelected = -1;
	
	if( iGender != FEMALE && iGender != MALE)
	{
		return false;
	}
	if(WARRIOR > iClass ||PRIEST < iClass)
	{
		return false;
	}
	
	const int iOldSelected = iSelected;
	iSelected = iGender*10 + iClass;
	
	if(iOldSelected !=iSelected)//선택한 에니메이션이 다르다면
	{
		FreeTestAnimation();
		char filename1[MAX_FILENAME] = "";
		sprintf(filename1,"./char/CharAniSmall%1d%1d.spr",iGender,iClass);
		LoadOpenningSpr( filename1, &test_animation_buf5, test_animation_spr5 );
	}
	
	static int x = 580;
	static int y = 280;
	
	int iX = x+iGabX2[iGender][iClass];
	int iY = y+iGabY2[iGender][iClass];
	
	if( !test_animation_buf5 ) return 0;
	static int count;
	
	PutCompressedImage( iX, iY, &test_animation_spr5[count] );
	
	count++;
	if( !test_animation_spr5[count].img ) 
	{
		count = 0;
	}
	return 1;
}

// Decreases timer on timed mall items
void DecMallItemTime()
{
	// Need to check and ignore treasure map items 
	// because timed mall item shares the same IATTR
	CItem* item = ItemUnit(HandItemAttr.item_no); // Hand 
	if (item->GetItemKind() != IK_TREASURE_MAP &&
		HandItemAttr.attr[IATTR_MALL_TIMED] > 0)
	{
		--HandItemAttr.attr[IATTR_MALL_TIMED];
	}	

	ItemAttr *inv = (ItemAttr *)InvItemAttr;
	for( int i=0; i<72; i++ )
	{
		CItem* item = ItemUnit(inv[i].item_no);
		if (item->GetItemKind() == IK_TREASURE_MAP) continue;

		if(inv[i].attr[IATTR_MALL_TIMED] > 0)
		{
			--inv[i].attr[IATTR_MALL_TIMED];
		}
	}

	for(int i=0; i<8; i++ )			// equip inv
	{
		CItem* item = ItemUnit(EquipItemAttr[i].item_no);
		if (item->GetItemKind() == IK_TREASURE_MAP) continue;

		if(EquipItemAttr[i].attr[IATTR_MALL_TIMED] > 0)
		{
			--EquipItemAttr[i].attr[IATTR_MALL_TIMED];
		}
	}

	for(int i=0; i<6; i++ )			// quick inv
	{
		CItem* item = ItemUnit(QuickItemAttr[i].item_no);
		if (item->GetItemKind() == IK_TREASURE_MAP) continue;

		if(QuickItemAttr[i].attr[IATTR_MALL_TIMED] > 0)
		{
			--QuickItemAttr[i].attr[IATTR_MALL_TIMED];
		}
	}

	for(int a=0;a<5;a++)				// bank inv
	{
		for(int b=0;b<3;b++)
		{
			for(int c=0;c<6;c++)
			{
				CItem* item = ItemUnit(BankItemAttr[a][b][c].item_no);
				if (item->GetItemKind() == IK_TREASURE_MAP) continue;

				if(BankItemAttr[a][b][c].attr[IATTR_MALL_TIMED] > 0)
				{
					--BankItemAttr[a][b][c].attr[IATTR_MALL_TIMED];
				}
			}
		}
	}
}
