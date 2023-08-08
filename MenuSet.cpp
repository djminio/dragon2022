#include "stdafx.h"
//#include "MenuDef.h"
#include "Itemtable.h"
#include "Menu.h"
#include "menuset.h"
#include "hong_sub.h"
#include "dragon.h"
#include "menudef2.h"
#include "smallmenuset.h"
#include "smallmenu.h"


extern void MenuSet2();//020515 lsw
extern void MenuSet3();	// 021020 kyo

extern char* EatRearWhiteChar( char* pStr );
extern SMENU SMenu[MAX_MENU];
extern SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];

extern void SetRect(RECT &rect,int left, int top, int right, int bottom);    //메뉴 구성요소의 충돌체크 rect형에 값 대입
extern SystemInfo	system_info;



////////////////////////////////////////////////////////////////////////////////////////////
CMenuImageMgr *g_MenuImageMgr = NULL;
int LoadMenuImage()
{
	char *filename = "./data/menu_image.txt";
	
	FILE *fp = Fopen( filename, "rt" );
	if( fp )
	{
		char temp[512];
		int index, image_max;
		char file_name[40];
		int max_index = 0;
		CMenuImageLink link;
		while( fgets( temp, 512, fp ) )
		{
			if( CheckContinueLine( temp[0] ) ) continue;
			char *token;
			token = strtok( temp, "\t\n" );
			if( !token ) continue;
			index = atoi( token );
			if( index > max_index ) max_index = index;		// 10000 이상이어야 한다.
			
			token = strtok( NULL, "\t\n" );
			image_max = atoi( token );
			token = strtok( NULL, "\t\n" );
			strcpy( file_name, token );
			link.InputData( index, image_max, file_name );
		}
		SAFE_DELETE(g_MenuImageMgr);
		
		if( image_max )
		{
			g_MenuImageMgr = new CMenuImageMgr( max_index-10000+1 );
			
			CMenuImageLink *find;
			link.SetFind();
			while( find = link.Next() )
			{
				g_MenuImageMgr->InsetData( find->GetParam(), find->GetDataPoint() );
			}
		}
		
		fclose( fp );
		return 1;
	}
	else
	{
		JustMsg( "Error! Not Found 'menu_image.txt'");
		return 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
// CMenuImage

int CMenuImage::clear()
{
	if (buf) 
	{
		FreeOpenningSpr(&buf);
		buf = NULL;
	}
	
	if (spr) 
	{
		delete [] spr;
		spr = NULL;
	}
	
	return 1;
}

// 020701 YGI
int CMenuImage::LoadMenuSpr()
{
	LoadOpenningSpr( file_name, &buf, spr, image_max );					// 투표
	return 1;
}
// 020701 YGI
Spr *CMenuImage::GetSpr( int img_number )
{
	if( img_number > image_max ) return NULL;
	if( !buf ) LoadMenuSpr();
	return &spr[img_number];
}

////////////////////////////////////////////////////////////////////////////////////////////
int CGuildMemberName::SetCount( int degree, int count, k_get_guild_memeber_list_name *data )
{
	if( m_degree[degree].m_iTotal ) m_degree[degree].clear();
	m_degree[degree].m_iTotal = count;
	if( count )
	{
		m_degree[degree].m_lpszName = new string[count];
		m_degree[degree].m_bpIsConnect = new int[count];
		int code = Hero->GetGuildCode(); // CSD-030324
		for( int i=0; i<count; i++ )
		{
			m_degree[degree].m_lpszName[i] = data[i].name;
			m_degree[degree].m_bpIsConnect[i] = data[i].is_connect;
			if( IsSubMaster( code, data[i].name ) )
			{
				m_nSubMasterNumber_degree = degree;
				m_nSubMasterNumber = i;
			}
		}
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////
DWORD GetNationMoney( int nation )
{
	if( !g_Menu_Variable.m_stNationMoney ) return 0;
	
	switch( nation )
	{
	case N_VYSEUS	:	return g_Menu_Variable.m_stNationMoney->money3;
	case N_ZYPERN	: 	return g_Menu_Variable.m_stNationMoney->money4;
	case N_YILSE	:	return g_Menu_Variable.m_stNationMoney->money6;
	}
	return 0;
}

// 주석 처리
bool CheckContinueLine( char check )
{
	if( check == 0 || check == ' ' || check == ';' || check == '#' || check == '\n' ) return true;
	return false;
}

///////////////////////////////////////////////////////////////////
// 포탈 메뉴
int CPotalMenu::LoadPotalMap( int nation )
{
	int nation_temp;
	int map_number;
	int money, x, y, type, img_type, level;

	char map_name[50] = {0,};
	
	FILE *fp = Fopen( "./data/map_potal.txt","rt" );
	if( !fp ) return 0;
	
	char temp[2048];
	while( fgets( temp, 2048, fp) )
	{
		if( ::CheckContinueLine( temp[0] ) ) continue;
		char *token = strtok( temp, "\t\n" );
		if( !token ) continue;
		map_number = atoi( token );
		
		token = strtok( NULL, "\t\n" );
		strcpy( map_name, token );
		
		token = strtok( NULL, "\t\n" );
		nation_temp = atoi( token );
		
		token = strtok( NULL, "\t\n" );
		money = atoi( token );
		
		token = strtok( NULL, "\t\n" );
		x = atoi( token );
		
		token = strtok( NULL, "\t\n" );
		y = atoi( token );
		
		token = strtok( NULL, "\t\n" );
		type = atoi( token );
		
		token = strtok( NULL, "\t\n" );
		img_type = atoi( token );
		
		token = strtok( NULL, "\t\n" );
		level = atoi( token );
		
		fgets( temp, 2048, fp);
		
		if( nation_temp == nation )
		{
			m_Map[m_Count].map_number = map_number;
			m_Map[m_Count].money = money;
			m_Map[m_Count].type = type;
			m_Map[m_Count].img_type = img_type;
			m_Map[m_Count].level = level;
			strcpy( m_Map[m_Count].map_name, map_name );
			int len = strlen( temp );
			if( len )
			{
				SetExplain( m_Count, temp, len );
			}
			m_Count++;
		}
	}
	fclose( fp );
	return 1;
}	

///////////////////////////////////////////////////////////////////
// 친구 시스템 클레스
int CFriendGrup::SetFriendName( int cn, char *name )
{
	strcpy( m_Friend[cn].name, name);
	return 1;
}
int CFriendGrup::SetFriendLogin( int cn, int login )
{
	m_Friend[cn].m_bLogin = login;
	return 1;
}
char *CFriendGrup::GetFriend( int cn, int &login )
{
	login = m_Friend[cn].m_bLogin;
	return m_Friend[cn].name;
}
int CFriendGrup::InsertFriend( char *name, int login )
{
	int cn = -1;
	for( int i=0; i<MAX_FRIEND_GROUP; i++ )
	{
		if( !m_Friend[i].name[0] )
		{
			if( cn == -1 ) cn = i;
			continue;
		}
		if( strcmp( m_Friend[i].name, name ) == 0 )
		{
			SetFriendLogin( i, login );
			break;
		}
	}
	if( cn != -1 )
	{
		SetFriendName( cn, name );
		SetFriendLogin( cn, login );
		count++;
	}
	
	SMENU *SubMenu = &SMenu[MN_FRIEND_MENU];
	int &page_status = SubMenu->work;
	WORD page;
	WORD page_max;
	GetWORDOfDWORD( (DWORD)page_status, page, page_max );
	
	int max_line = 10;
	page_max = (count-1)/10.0;
	page_status = (int )MAKELONG( page, page_max+1 );
	return 1;
}
int CFriendGrup::clear()
{
	for( int i=0; i<MAX_FRIEND_GROUP; i++ )
	{
		m_Friend[i].name[0] = 0;
		m_Friend[i].m_bLogin= 0;
	}
	count = 0;
	return 1;
}
int CFriendGrup::SerchName( const char *name )//030504 lsw
{
	for( int i=0; i<MAX_FRIEND_GROUP; i++ )
	{
		if( ::stricmp( name, m_Friend[i].name ) == 0 )
		{
			return i;
		}
	}
	return -1;
}
int CFriendGrup::DeleteName( int cn )
{
	m_Friend[cn].clear();
	count--;
	return 1;
}

int CFriendGrup::SetRect( int cn, int type, int left, int top, int right, int bottom )
{
	switch( type )
	{
	case 1: 
		::SetRect( m_Friend[cn].rect1, left, top, right, bottom );
		break;
	case 2: 
		::SetRect( m_Friend[cn].rect2, left, top, right, bottom );
		break;
	}
	return 1;	
}

char *CFriendGrup::CheckRect( int cn, int type, int x, int y )
{
	switch( type )
	{
	case 1:
		if( MouseInRectCheak( x, y, m_Friend[cn].rect1 ) )
		{
			return m_Friend[cn].name;
		}
		break;
	case 2:
		if( MouseInRectCheak( x, y, m_Friend[cn].rect2 ) )
		{
			return m_Friend[cn].name;
		}
		break;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////
LPCTSTR CRuneExplain::GetTitleExplain( int lune_num )
{
	return m_szExplainTitle[lune_num].c_str();
}

int CRuneExplain::LoadLuneText2( int lune_num )
{
	if( !m_szExplainTitle[lune_num].empty() ) return 0;
	
	char file[128];
	sprintf( file, "./data/rune_%02d.txt", lune_num+1 );
	
	FILE *fp = Fopen( file, "rt" );
	char temp[4096];
	if( !fp ) return 0;
	while( fgets( temp, 4096, fp ) )
	{
		ChangeString2( temp, "\\n", " \n" );
		m_szExplainTitle[lune_num] += temp;
	}	
	fclose( fp );
	return 1;
}
int CRuneExplain::LoadLuneText( )
{
	FILE *fp = Fopen( "./data/rune_ex.txt", "rt" );
	if( !fp ) return 0;
	
	char temp[4096];
	int step;
	int rune_count;
	while( fgets( temp, 4096, fp ) )
	{
		if( temp[0] == ' '||temp[0] =='\n'||temp[0] =='#'||temp[0] == ';'|| temp[0] == NULL )
			continue;
		
		char *token = strtok( temp, "\t\n" );
		if( !token ) continue;
		step = atoi( token );
		
		token = strtok( NULL, "\t\n" );
		if( !token ) continue;
		rune_count= atoi( token );
		
		token = strtok( NULL, "\t\n" );
		if( !token ) continue;
		strcpy( m_szTitle[step-1][rune_count], token );
		
		token = strtok( NULL, "\t\n" );
		if( !token ) continue;
		strcpy( m_szExplain[step-1][rune_count], token );
	}
	fclose( fp );
	return 1;
}

int k_char_rgb::set()
{
	body_r = SCharacterData.body_r ;
	body_g = SCharacterData.body_g ;
	body_b = SCharacterData.body_b ;
	cloth_r= SCharacterData.cloth_r;
	cloth_g= SCharacterData.cloth_g;
	cloth_b= SCharacterData.cloth_b;
	return 1;
}

k_char_rgb::~k_char_rgb()
{
	SCharacterData.body_r = body_r ;
	SCharacterData.body_g = body_g ;
	SCharacterData.body_b = body_b ;
	SCharacterData.cloth_r= cloth_r;
	SCharacterData.cloth_g= cloth_g;
	SCharacterData.cloth_b= cloth_b;
	g_Menu_Variable.m_bChangeColor = true;
}
///////////////////////////////////////////////////////////////////
int CMenuVariable::clear()
{
	if( m_pGuildRegistExplain )
	{
		delete m_pGuildRegistExplain;
		m_pGuildRegistExplain = NULL;
	}
	if( m_pRune )
	{
		delete m_pRune;
		m_pRune = NULL;
	}
	if( m_stRGB )
	{
		delete m_stRGB;
		m_stRGB = 0;
	}
	bCheckSalvation = 0;
	if( pGambleItem )
	{
		delete[] pGambleItem; 
		pGambleItem = 0;
	}
	m_GambleRune = 0;
	if( m_szGuildPublicNotics )
	{
		delete[] m_szGuildPublicNotics;
		m_szGuildPublicNotics = 0 ;
	}
	if( m_TodayGambleItem )
	{
		delete m_TodayGambleItem;
		m_TodayGambleItem = 0;
	}
	if( m_pPutGuildItem )
	{
		delete m_pPutGuildItem;
		m_pPutGuildItem = 0;
	}
	if( m_pSubGuildMaster )
	{
		delete[] m_pSubGuildMaster;
		m_pSubGuildMaster = NULL;
	}
	
	m_GuildMemberName.clear();
	m_szFileTextPut = "";
	m_nation_item.clear();
	
	if( m_pGuildHouseInfo )
	{
		delete m_pGuildHouseInfo;
		m_pGuildHouseInfo = 0;
	}
	
	//acer5
	if( m_pOtherChar ) 
	{
		delete m_pOtherChar;
		m_pOtherChar = 0;
	}
	// 020620 YGI
	if( m_pScenarioTrapInfo )
	{
		delete m_pScenarioTrapInfo;
		m_pScenarioTrapInfo = 0;
	}
	
	// 020701 YGI
	if( m_pScenarioTrapKey )
	{
		delete m_pScenarioTrapKey;
		m_pScenarioTrapKey = 0;
	}
	
	
	return 1;
}

int CMenuVariable::clear2()
{
	clear();
	clear3();
	
	m_GuildSaveId = 0;
	m_bGuildChatOn = 1;
	m_bLoadGuildMark = 0;
	
	if( m_clsRuneExplain )
	{
		delete m_clsRuneExplain;
		m_clsRuneExplain = 0;	
	}
	if( m_stNationMoney )
	{
		delete m_stNationMoney;
		m_stNationMoney = 0;	
	}
	//m_GuildMemberName.clear();
	if( m_FriendGrup )
	{
		delete m_FriendGrup;
		m_FriendGrup = 0;
	}
	m_PartySelect = 0;
	m_nCheckSubMaster = 0;
	m_bTradeON = 1;		//020808 YGI 
	
	//021030 YGI
	if( m_pEventObjcet )
	{
		delete m_pEventObjcet;
		m_pEventObjcet = 0;
	}
	
	return 1;
}

int CMenuVariable::clear3()
{
	clear();
	
	m_bOXTime = 0;
	m_OXTime = 0;
	
	if( m_PotalMap )
	{
		delete m_PotalMap;
		m_PotalMap = 0;
	}
	if( m_pGuildDegree )
	{
		delete m_pGuildDegree;
		m_pGuildDegree = NULL;
	}
	m_assent.type = 0;
	
	// 020620 YGI
	if( m_pDinamicTile )
	{
		delete m_pDinamicTile;
		m_pDinamicTile = 0;
	}
	
	// 021107 YGI
	if( m_pEventMoveMap )
	{
		delete m_pEventMoveMap;
		m_pEventMoveMap = 0;
	}
	
	return 1;
}
int CMenuVariable::clear4()
{
	// 020428 YGI acer
	if( m_CheckProcess )
	{
		delete m_CheckProcess;
		m_CheckProcess= 0;
	}
	return 1;
}

//021030 YGI
int CMenuVariable::GetEventSound()
{
	if( m_pEventObjcet && ( m_pEventObjcet->active & ACTIVE_EVENT_SOUND ) )
	{
		return m_pEventObjcet->sound;
	}
	return 0;
}

int CMenuVariable::LoadingGuildRegistExplain( char *szFileName )
{
	if( !szFileName ) return 0;
	int nLine = GetTextLine( szFileName );
	if( !nLine ) return 0;
	
	if( m_pGuildRegistExplain ) 
	{
		delete m_pGuildRegistExplain;
	}
#ifdef _DEBUG // finito 060507	
	if( GetSysInfo( SI_GAME_MAKE_MODE  ) )
	{
		if( nLine >= 100 ) 
		{
			JustMsg( "guild_explain1.txt 에 설명이 너무 많습니다." ); 
			return 0;
		}
	}
#endif
	m_pGuildRegistExplain = new k_guild_regist_explain;
	memset( m_pGuildRegistExplain, 0, sizeof( k_guild_regist_explain ) );
	
	FILE *fp = Fopen( szFileName, "rt" );
	if( !fp ) return 0;
	
	
	char szTemp[256];
	int nIndex = 0;
	while( fgets( szTemp, 256, fp ) )
	{
		if( !szTemp[0] ) continue;
		EatRearWhiteChar( szTemp );
		strcpy( m_pGuildRegistExplain->m_pString[nIndex], szTemp );
		nIndex++;
	}
	fclose( fp );
	return nLine;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// CRuneMenu Member Fuctions
CRuneMenu::CRuneMenu()
{
	m_nQuestStep = 0;
	m_nSelectRune = 0;
	LoadRuneMenuXY();
}
int CRuneMenu::LoadRuneMenuXY()
{
	FILE *fp = Fopen( "./data/rune_xy.txt", "rt" );
	if( fp )
	{
		char temp[256];
		int count;
		while( fgets( temp, 256, fp ) )
		{
			if( temp[0] == ';' || temp[0] == '#' ) continue;
			char *token = strtok( temp, "\t\n" );
			count = atoi( token );
			m_stRune[count].m_nRuneNum = count;
			
			token = strtok( NULL, "\t\n" );
			m_stRune[count].x = atoi( token );
			
			token = strtok( NULL, "\t\n" );
			m_stRune[count].y = atoi( token );
			
			token = strtok( NULL, "\t\n" );
			m_stRune[count].m_nLeftRuneImage = atoi( token );
			
			token = strtok( NULL, "\t\n" );
			m_stRune[count].m_nRightRuneImage = atoi( token );
		}
		fclose( fp );
	}
	return 1;
}



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void MenuSet()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//////// 메인 인터페이스 파티원 메뉴에서 선택 라이트 출력
	SHideNomal[HN_PARTY_SELECT][0].nType=FT_NOMAL_PUT;
	SHideNomal[HN_PARTY_SELECT][0].bShow=FALSE;
	SHideNomal[HN_PARTY_SELECT][0].x=46;
	SHideNomal[HN_PARTY_SELECT][0].y=12;
	SHideNomal[HN_PARTY_SELECT][0].nImageNumber=465;
	SetRect(SHideNomal[HN_PARTY_SELECT][0].rCheakBox,18,44,104,69);
	
	SHideNomal[HN_PARTY_SELECT][1].nType=FT_NOMAL_PUT;
	SHideNomal[HN_PARTY_SELECT][1].bShow=FALSE;
	SHideNomal[HN_PARTY_SELECT][1].x=146;
	SHideNomal[HN_PARTY_SELECT][1].y=12;
	SHideNomal[HN_PARTY_SELECT][1].nImageNumber=466;
	SetRect(SHideNomal[HN_PARTY_SELECT][1].rCheakBox,117,44,203,69);
	
	SHideNomal[HN_PARTY_SELECT][2].nType=FT_NOMAL_PUT;
	SHideNomal[HN_PARTY_SELECT][2].bShow=FALSE;
	SHideNomal[HN_PARTY_SELECT][2].x=245;
	SHideNomal[HN_PARTY_SELECT][2].y=12;
	SHideNomal[HN_PARTY_SELECT][2].nImageNumber=467;
	SetRect(SHideNomal[HN_PARTY_SELECT][2].rCheakBox,216,44,302,69);
	
	////////////////////////////////////////////////////////////////////////////////////////////
	// SKILL 메인 메뉴 선택 글자.
	SHideNomal[HN_SKILL_MAIN_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SKILL_MAIN_TEXT][0].y=122;
	wsprintf(SHideNomal[HN_SKILL_MAIN_TEXT][0].temp,"CLASS SKILL");
	SHideNomal[HN_SKILL_MAIN_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SKILL_MAIN_TEXT][1].y=159;
	wsprintf(SHideNomal[HN_SKILL_MAIN_TEXT][1].temp,"JOB SKILL");
	SHideNomal[HN_SKILL_MAIN_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SKILL_MAIN_TEXT][2].y=196;
	wsprintf(SHideNomal[HN_SKILL_MAIN_TEXT][2].temp,"GENERAL SKILL");
	SHideNomal[HN_SKILL_MAIN_TEXT][3].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SKILL_MAIN_TEXT][3].y=233;
	wsprintf(SHideNomal[HN_SKILL_MAIN_TEXT][3].temp,"KNOWLEDGE");
	
	////////////////////////////////////////////////////////////////////////////////////////////
	// 메인인터페이스의 system메뉴에서 글자
	SHideNomal[HN_SYSTEM_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SYSTEM_TEXT][0].y=87;
	wsprintf(SHideNomal[HN_SYSTEM_TEXT][0].temp,"OPTION");
	SHideNomal[HN_SYSTEM_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SYSTEM_TEXT][1].y=116;
	wsprintf(SHideNomal[HN_SYSTEM_TEXT][1].temp,"HELP");
	SHideNomal[HN_SYSTEM_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SYSTEM_TEXT][2].y=145;
	wsprintf(SHideNomal[HN_SYSTEM_TEXT][2].temp,"LOGOUT");
	
	////////////////////////////////////////////////////////////////////////////////////////////
	// 메인인터페이스의 system메뉴에서 help글자
	SHideNomal[HN_SYSTEM_HELP_TEXT][0].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SYSTEM_HELP_TEXT][0].y=87;
	wsprintf(SHideNomal[HN_SYSTEM_HELP_TEXT][0].temp,"KEY");
	SHideNomal[HN_SYSTEM_HELP_TEXT][1].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SYSTEM_HELP_TEXT][1].y=116;
	wsprintf(SHideNomal[HN_SYSTEM_HELP_TEXT][1].temp,"MESSAGE");
	SHideNomal[HN_SYSTEM_HELP_TEXT][2].nType=FT_TEXT_PUT_CENTER;
	SHideNomal[HN_SYSTEM_HELP_TEXT][2].y=145;
	wsprintf(SHideNomal[HN_SYSTEM_HELP_TEXT][2].temp,"MACRO");
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 사제 메뉴
	//메인 메뉴 뛰우기
	/*	SMenu[MN_RELATION].x=480;
	SMenu[MN_RELATION].y=0;
	SMenu[MN_RELATION].nImageNumber=464;
	//	SMenu[MN_RELATION].nImageNumber=849;
	SMenu[MN_RELATION].nFieldCount=13;
	
	  // RELEASE button
	  SMenu[MN_RELATION].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
	  SMenu[MN_RELATION].nField[0].nSpecialWillDo=SWD_PARTY_RELEASE;
	  SMenu[MN_RELATION].nField[0].nWillDo=12;
	  SMenu[MN_RELATION].nField[0].x=244;
	  SMenu[MN_RELATION].nField[0].y=205;
	  SMenu[MN_RELATION].nField[0].nRectImage=78;
	  SMenu[MN_RELATION].nField[0].nImageNumber=474;
	  SetRect(SMenu[MN_RELATION].nField[0].rCheakBox,244,205,292,253);
	  
		//자신을 닫기
		SMenu[MN_RELATION].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
		SMenu[MN_RELATION].nField[1].nSpecialWillDo=SWD_RMENUCLOSE;
		SMenu[MN_RELATION].nField[1].x=236;
		SMenu[MN_RELATION].nField[1].y=290;
		SMenu[MN_RELATION].nField[1].nRectImage=79;
		SMenu[MN_RELATION].nField[1].nImageNumber=475;
		SetRect(SMenu[MN_RELATION].nField[1].rCheakBox,236,290,299,311);
		
		  // RELATION 버튼 출력
		  SMenu[MN_RELATION].nField[2].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
		  SMenu[MN_RELATION].nField[2].nSpecialWillDo=SWD_PARTY_BUTTON;
		  //SMenu[MN_RELATION].nField[2].nSpecialWillDo=SWD_SORRY;
		  SMenu[MN_RELATION].nField[2].nWillDo=RELATION;
		  SMenu[MN_RELATION].nField[2].nSHideNomalNumber=12;
		  SMenu[MN_RELATION].nField[2].x=18;
		  SMenu[MN_RELATION].nField[2].y=42;
		  SMenu[MN_RELATION].nField[2].nRectImage=467;
		  SMenu[MN_RELATION].nField[2].nImageNumber=476;
		  SetRect(SMenu[MN_RELATION].nField[2].rCheakBox,18,44,104,69);
		  
			// PARTY 버튼 출력
			SMenu[MN_RELATION].nField[3].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			SMenu[MN_RELATION].nField[3].nSpecialWillDo=SWD_PARTY_BUTTON;
			SMenu[MN_RELATION].nField[3].nWillDo=PARTY;
			SMenu[MN_RELATION].nField[3].nSHideNomalNumber=12;
			SMenu[MN_RELATION].nField[3].x=117;
			SMenu[MN_RELATION].nField[3].y=42;
			SMenu[MN_RELATION].nField[3].nRectImage=466;
			SMenu[MN_RELATION].nField[3].nImageNumber=477;
			SetRect(SMenu[MN_RELATION].nField[3].rCheakBox,117,44,203,69);
			
			  // EMPROYMENT 버튼 출력
			  SMenu[MN_RELATION].nField[4].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			  //SMenu[MN_RELATION].nField[4].nSpecialWillDo=SWD_PARTY_BUTTON;
			  SMenu[MN_RELATION].nField[4].nSpecialWillDo=SWD_SORRY;
			  SMenu[MN_RELATION].nField[4].nWillDo=EMPLOYMENT;
			  SMenu[MN_RELATION].nField[4].nSHideNomalNumber=12;
			  SMenu[MN_RELATION].nField[4].x=216;
			  SMenu[MN_RELATION].nField[4].y=42;
			  SMenu[MN_RELATION].nField[4].nRectImage=465;
			  SMenu[MN_RELATION].nField[4].nImageNumber=478;
			  SetRect(SMenu[MN_RELATION].nField[4].rCheakBox,216,44,302,69);
			  
				/////////////////////////////////////////////////
				// 파티 제목 찍기
				SMenu[MN_RELATION].nField[5].nType=FT_NO_CHECK;
				//	SMenu[MN_RELATION].nField[5].nType=FT_NOMAL_PUT;
				SMenu[MN_RELATION].nField[5].x=90;
				SMenu[MN_RELATION].nField[5].y=10;
				SMenu[MN_RELATION].nField[5].nImageNumber=850;
				
				  //다른 파티원들 출력 :
				  // 먼저 가까이만 가도 숫자 필드의 숫자에 불이 들어온다. 1번
				  // 다른 캐릭터와 관계가 있다면 nSpecialWillDo를 체크(true)해 둔다			// origin 좌표로 이미지를 찍는다.
				  SMenu[MN_RELATION].nField[6].nType=FT_PARTY;
				  SMenu[MN_RELATION].nField[6].nWillDo=0;
				  SMenu[MN_RELATION].nField[6].nSpecialWillDo=12;
				  SMenu[MN_RELATION].nField[6].x=55;
				  SMenu[MN_RELATION].nField[6].y=217;
				  SMenu[MN_RELATION].nField[6].nRectImage=468;
				  SMenu[MN_RELATION].nField[6].nImageNumber=468;
				  SetRect(SMenu[MN_RELATION].nField[6].rCheakBox,25,182,85,252);
				  
					// 2번
					SMenu[MN_RELATION].nField[7].nType=FT_PARTY;
					SMenu[MN_RELATION].nField[7].nWillDo=1;
					SMenu[MN_RELATION].nField[7].nSpecialWillDo=12;
					SMenu[MN_RELATION].nField[7].x=124;
					SMenu[MN_RELATION].nField[7].y=217;
					SMenu[MN_RELATION].nField[7].nImageNumber=469;
					SMenu[MN_RELATION].nField[7].nRectImage=469;
					SetRect(SMenu[MN_RELATION].nField[7].rCheakBox,94,182,154,252);
					
					  // 3번
					  SMenu[MN_RELATION].nField[8].nType=FT_PARTY;
					  SMenu[MN_RELATION].nField[8].nWillDo=2;
					  SMenu[MN_RELATION].nField[8].nSpecialWillDo=12;
					  SMenu[MN_RELATION].nField[8].x=193;
					  SMenu[MN_RELATION].nField[8].y=217;
					  SMenu[MN_RELATION].nField[8].nImageNumber=470;
					  SMenu[MN_RELATION].nField[8].nRectImage=470;
					  SetRect(SMenu[MN_RELATION].nField[8].rCheakBox,163,182,233,252);
					  
						//4번
						SMenu[MN_RELATION].nField[9].nType=FT_PARTY;
						SMenu[MN_RELATION].nField[9].nWillDo=3;
						SMenu[MN_RELATION].nField[9].nSpecialWillDo=12;
						SMenu[MN_RELATION].nField[9].x=55;
						SMenu[MN_RELATION].nField[9].y=297;
						SMenu[MN_RELATION].nField[9].nImageNumber=471;
						SMenu[MN_RELATION].nField[9].nRectImage=471;
						SetRect(SMenu[MN_RELATION].nField[9].rCheakBox,25,262,85,332);
						
						  //5번
						  SMenu[MN_RELATION].nField[10].nType=FT_PARTY;
						  SMenu[MN_RELATION].nField[10].nWillDo=4;
						  SMenu[MN_RELATION].nField[10].nSpecialWillDo=12;
						  SMenu[MN_RELATION].nField[10].x=124;
						  SMenu[MN_RELATION].nField[10].y=297;
						  SMenu[MN_RELATION].nField[10].nImageNumber=472;
						  SMenu[MN_RELATION].nField[10].nRectImage=472;
						  SetRect(SMenu[MN_RELATION].nField[10].rCheakBox,94,262,154,332);
						  
							//6번
							SMenu[MN_RELATION].nField[11].nType=FT_PARTY;
							SMenu[MN_RELATION].nField[11].nWillDo=5;
							SMenu[MN_RELATION].nField[11].nSpecialWillDo=12;
							SMenu[MN_RELATION].nField[11].x=193;
							SMenu[MN_RELATION].nField[11].y=297;
							SMenu[MN_RELATION].nField[11].nImageNumber=473;
							SMenu[MN_RELATION].nField[11].nRectImage=473;
							SetRect(SMenu[MN_RELATION].nField[11].rCheakBox,163,262,233,332);
							
							  // 관계된 사람을 클릭했을때, 얼굴과 스킬 보여주기
							  SMenu[MN_RELATION].nField[12].nType=FT_PARTY_FACE;
							  //	SMenu[MN_RELATION].nField[12].nWillDo = 현재 선택한 파티원;
							  SMenu[MN_RELATION].nField[12].x=80;
							  SMenu[MN_RELATION].nField[12].y=124;
							  SMenu[MN_RELATION].nField[12].nImageNumber=0;
*/





//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//스테이터스나 어빌리티 메뉴에서 가치관 문장을 선택했을때 나오는 메뉴

//메인 메뉴 뛰우기
SMenu[MN_ARIGEMENT].x=0;
SMenu[MN_ARIGEMENT].y=0;
SMenu[MN_ARIGEMENT].nImageNumber=172;
SMenu[MN_ARIGEMENT].nFieldCount=7;    
SMenu[MN_ARIGEMENT].CheakType=0;

//떠있는 가치관 심볼 보여주기
SMenu[MN_ARIGEMENT].nField[0].nType=FT_NOMAL_PUT;
SMenu[MN_ARIGEMENT].nField[0].x=132;
SMenu[MN_ARIGEMENT].nField[0].y=32;

//가치관 심볼 보여주기
SMenu[MN_ARIGEMENT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_PUT;
SMenu[MN_ARIGEMENT].nField[1].nSpecialWillDo=SWD_ARIGEMENT_STATUS;
SMenu[MN_ARIGEMENT].nField[1].x=132;
SMenu[MN_ARIGEMENT].nField[1].y=32;
SMenu[MN_ARIGEMENT].nField[1].fLButtonDown=FALSE;
SMenu[MN_ARIGEMENT].nField[1].fCheakFlag=FALSE;
SetRect(SMenu[MN_ARIGEMENT].nField[1].rCheakBox,117,31,200,109);

//왼쪽 불꽃 찍기
SMenu[MN_ARIGEMENT].nField[2].nType=FT_FIRE;
SMenu[MN_ARIGEMENT].nField[2].x=38;
SMenu[MN_ARIGEMENT].nField[2].y=55;
SMenu[MN_ARIGEMENT].nField[2].fLButtonDown=TRUE;
SMenu[MN_ARIGEMENT].nField[2].fCheakFlag=TRUE;

//오른족 불꽃 찍기
SMenu[MN_ARIGEMENT].nField[3].nType=FT_FIRE;
SMenu[MN_ARIGEMENT].nField[3].x=253;
SMenu[MN_ARIGEMENT].nField[3].y=55;
SMenu[MN_ARIGEMENT].nField[3].fLButtonDown=TRUE;
SMenu[MN_ARIGEMENT].nField[3].fCheakFlag=TRUE;

//가치관 데이타 찍기
SMenu[MN_ARIGEMENT].nField[4].nType=FT_DATA_PUT;
SMenu[MN_ARIGEMENT].nField[4].x=102;
SMenu[MN_ARIGEMENT].nField[4].y=129;
SMenu[MN_ARIGEMENT].nField[4].nImageNumber=DP_ARIGEMENT;

//게이지 찍기
SMenu[MN_ARIGEMENT].nField[5].nType=FT_DATA_PUT;
SMenu[MN_ARIGEMENT].nField[5].nImageNumber=DP_ARIGEMENT_BAR;

//게이지 껍데기 찍기
SMenu[MN_ARIGEMENT].nField[6].nType=FT_NOMAL_PUT;
SMenu[MN_ARIGEMENT].nField[6].x=151;
SMenu[MN_ARIGEMENT].nField[6].y=162;
SMenu[MN_ARIGEMENT].nField[6].nImageNumber=311;


/*
////////////////////////////////////////////////////////////////////////////////////////////
//dairy메뉴

  //메인메뉴 뛰우기
  SMenu[MN_DAIRY_DAIRY].x=0;
  SMenu[MN_DAIRY_DAIRY].y=0;
  SMenu[MN_DAIRY_DAIRY].nImageNumber=212;
  SMenu[MN_DAIRY_DAIRY].nFieldCount=8;
  SMenu[MN_DAIRY_DAIRY].CheakType=0;
  
	//스스로 닫기
	SMenu[MN_DAIRY_DAIRY].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_PUT;
	SMenu[MN_DAIRY_DAIRY].nField[0].nSpecialWillDo=SWD_UNMOUSEFOCUS;
	SMenu[MN_DAIRY_DAIRY].nField[0].fCheakFlag=FALSE;
	SMenu[MN_DAIRY_DAIRY].nField[0].fLButtonDown=FALSE;
	SMenu[MN_DAIRY_DAIRY].nField[0].x=16;
	SMenu[MN_DAIRY_DAIRY].nField[0].y=222;
	SMenu[MN_DAIRY_DAIRY].nField[0].nImageNumber=216;
	SetRect(SMenu[MN_DAIRY_DAIRY].nField[0].rCheakBox,16,222,97,245);
	
	  //WRITE글자 찍기
	  SMenu[MN_DAIRY_DAIRY].nField[1].nType=FT_NOMAL_PUT;
	  SMenu[MN_DAIRY_DAIRY].nField[1].x=553;
	  SMenu[MN_DAIRY_DAIRY].nField[1].y=118;
	  SMenu[MN_DAIRY_DAIRY].nField[1].nImageNumber=221;
	  
		//READ글자 찍기
		SMenu[MN_DAIRY_DAIRY].nField[2].nType=FT_NOMAL_PUT;
		SMenu[MN_DAIRY_DAIRY].nField[2].x=555;
		SMenu[MN_DAIRY_DAIRY].nField[2].y=200;
		SMenu[MN_DAIRY_DAIRY].nField[2].nImageNumber=222;
		
		  //MN_DAIRY_DAIRY_WRITE메뉴 열기
		  SMenu[MN_DAIRY_DAIRY].nField[3].nType=FT_HIDE_WILLDO_PUT;
		  SMenu[MN_DAIRY_DAIRY].nField[3].nWillDo=MN_DAIRY_DAIRY_WRITE;
		  SMenu[MN_DAIRY_DAIRY].nField[3].fCheakFlag=FALSE;
		  SMenu[MN_DAIRY_DAIRY].nField[3].fLButtonDown=FALSE;
		  SMenu[MN_DAIRY_DAIRY].nField[3].x=553;
		  SMenu[MN_DAIRY_DAIRY].nField[3].y=118;
		  SMenu[MN_DAIRY_DAIRY].nField[3].nImageNumber=227;
		  SetRect(SMenu[MN_DAIRY_DAIRY].nField[3].rCheakBox,553,118,616,141);
		  
			//MN_DAIRY_DAIRY_READ메뉴 열기
			SMenu[MN_DAIRY_DAIRY].nField[4].nType=FT_HIDE_WILLDO_PUT;
			SMenu[MN_DAIRY_DAIRY].nField[4].nWillDo=MN_DAIRY_DAIRY_READ;
			SMenu[MN_DAIRY_DAIRY].nField[4].fCheakFlag=FALSE;
			SMenu[MN_DAIRY_DAIRY].nField[4].fLButtonDown=FALSE;
			SMenu[MN_DAIRY_DAIRY].nField[4].x=555;
			SMenu[MN_DAIRY_DAIRY].nField[4].y=200;
			SMenu[MN_DAIRY_DAIRY].nField[4].nImageNumber=228;
			SetRect(SMenu[MN_DAIRY_DAIRY].nField[4].rCheakBox,555,200,618,223);
			
			  //MN_DAIRY_LETTER메뉴 열기
			  SMenu[MN_DAIRY_DAIRY].nField[5].nType=FT_HIDE_WILLDO_PUT;
			  SMenu[MN_DAIRY_DAIRY].nField[5].nWillDo=MN_DAIRY_LETTER;
			  SMenu[MN_DAIRY_DAIRY].nField[5].fCheakFlag=FALSE;
			  SMenu[MN_DAIRY_DAIRY].nField[5].fLButtonDown=FALSE;
			  SMenu[MN_DAIRY_DAIRY].nField[5].x=200;
			  SMenu[MN_DAIRY_DAIRY].nField[5].y=200;
			  SMenu[MN_DAIRY_DAIRY].nField[5].nImageNumber=0;
			  SetRect(SMenu[MN_DAIRY_DAIRY].nField[5].rCheakBox,8,145,102,213);
			  
				//MN_DAIRY_CARD메뉴 열기
				SMenu[MN_DAIRY_DAIRY].nField[6].nType=FT_HIDE_WILLDO_PUT;
				SMenu[MN_DAIRY_DAIRY].nField[6].nWillDo=MN_DAIRY_CARD;
				SMenu[MN_DAIRY_DAIRY].nField[6].fCheakFlag=FALSE;
				SMenu[MN_DAIRY_DAIRY].nField[6].fLButtonDown=FALSE;
				SMenu[MN_DAIRY_DAIRY].nField[6].x=200;
				SMenu[MN_DAIRY_DAIRY].nField[6].y=200;
				SMenu[MN_DAIRY_DAIRY].nField[6].nImageNumber=0;
				SetRect(SMenu[MN_DAIRY_DAIRY].nField[6].rCheakBox,9,77,103,147);
				
				  //종이 찍기
				  SMenu[MN_DAIRY_DAIRY].nField[7].nType=FT_NOMAL_PUT;
				  SMenu[MN_DAIRY_DAIRY].nField[7].x=104;
				  SMenu[MN_DAIRY_DAIRY].nField[7].y=10;
				  SMenu[MN_DAIRY_DAIRY].nField[7].nImageNumber=213;
*/
///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//메인인터페이스에서 시스템 메뉴 에서 option

SMenu[MN_SYSTEM_OPTION].x=176+GABX_SCREEN;
SMenu[MN_SYSTEM_OPTION].y=0+GABY_SCREEN;
SMenu[MN_SYSTEM_OPTION].nImageNumber=764;
SMenu[MN_SYSTEM_OPTION].nFieldCount=18;
SMenu[MN_SYSTEM_OPTION].CheakType=0;

//ok
SMenu[MN_SYSTEM_OPTION].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
SMenu[MN_SYSTEM_OPTION].nField[0].nSpecialWillDo=SWD_SMALLMENU_END;
SMenu[MN_SYSTEM_OPTION].nField[0].x=108;
SMenu[MN_SYSTEM_OPTION].nField[0].y=293;
SMenu[MN_SYSTEM_OPTION].nField[0].nRectImage=771;
SMenu[MN_SYSTEM_OPTION].nField[0].nImageNumber=772;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[0].rCheakBox,108,293,174,327);

//sound왼
SMenu[MN_SYSTEM_OPTION].nField[1].nType=FT_SYSTEM_OPTION_SCROLL;
SMenu[MN_SYSTEM_OPTION].nField[1].x=138;//129;
SMenu[MN_SYSTEM_OPTION].nField[1].y=79;//70;
SMenu[MN_SYSTEM_OPTION].nField[1].nRectImage=782;
SMenu[MN_SYSTEM_OPTION].nField[1].nImageNumber=783;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[1].rCheakBox,135,74,160,97);

//sound오른
SMenu[MN_SYSTEM_OPTION].nField[2].nType=FT_SYSTEM_OPTION_SCROLL;
SMenu[MN_SYSTEM_OPTION].nField[2].x=243;
SMenu[MN_SYSTEM_OPTION].nField[2].y=78;
SMenu[MN_SYSTEM_OPTION].nField[2].nRectImage=785;
SMenu[MN_SYSTEM_OPTION].nField[2].nImageNumber=786;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[2].rCheakBox,240,74,268,98);

//music왼
SMenu[MN_SYSTEM_OPTION].nField[3].nType=FT_SYSTEM_OPTION_SCROLL;
SMenu[MN_SYSTEM_OPTION].nField[3].x=138;
SMenu[MN_SYSTEM_OPTION].nField[3].y=109;
SMenu[MN_SYSTEM_OPTION].nField[3].nRectImage=782;
SMenu[MN_SYSTEM_OPTION].nField[3].nImageNumber=783;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[3].rCheakBox,134,103,157,127);

//music오른
SMenu[MN_SYSTEM_OPTION].nField[4].nType=FT_SYSTEM_OPTION_SCROLL;
SMenu[MN_SYSTEM_OPTION].nField[4].x=243;
SMenu[MN_SYSTEM_OPTION].nField[4].y=108;
SMenu[MN_SYSTEM_OPTION].nField[4].nRectImage=785;
SMenu[MN_SYSTEM_OPTION].nField[4].nImageNumber=786;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[4].rCheakBox,240,103,268,127);

//mouse왼
SMenu[MN_SYSTEM_OPTION].nField[5].nType=FT_SYSTEM_OPTION_SCROLL;
SMenu[MN_SYSTEM_OPTION].nField[5].x=138;
SMenu[MN_SYSTEM_OPTION].nField[5].y=140;
SMenu[MN_SYSTEM_OPTION].nField[5].nRectImage=782;
SMenu[MN_SYSTEM_OPTION].nField[5].nImageNumber=783;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[5].rCheakBox,134,134,157,158);

//mouse오른
SMenu[MN_SYSTEM_OPTION].nField[6].nType=FT_SYSTEM_OPTION_SCROLL;
SMenu[MN_SYSTEM_OPTION].nField[6].x=243;
SMenu[MN_SYSTEM_OPTION].nField[6].y=139;
SMenu[MN_SYSTEM_OPTION].nField[6].nRectImage=785;
SMenu[MN_SYSTEM_OPTION].nField[6].nImageNumber=786;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[6].rCheakBox,240,134,268,158);

//scroll왼
SMenu[MN_SYSTEM_OPTION].nField[7].nType=FT_SYSTEM_OPTION_SCROLL;
SMenu[MN_SYSTEM_OPTION].nField[7].x=138;
SMenu[MN_SYSTEM_OPTION].nField[7].y=171;
SMenu[MN_SYSTEM_OPTION].nField[7].nRectImage=782;
SMenu[MN_SYSTEM_OPTION].nField[7].nImageNumber=783;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[7].rCheakBox,134,165,157,189);

//scroll오른
SMenu[MN_SYSTEM_OPTION].nField[8].nType=FT_SYSTEM_OPTION_SCROLL;
SMenu[MN_SYSTEM_OPTION].nField[8].x=243;
SMenu[MN_SYSTEM_OPTION].nField[8].y=170;
SMenu[MN_SYSTEM_OPTION].nField[8].nRectImage=785;
SMenu[MN_SYSTEM_OPTION].nField[8].nImageNumber=786;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[8].rCheakBox,240,165,268,189);

//scroll gauge 보여주기
SMenu[MN_SYSTEM_OPTION].nField[9].nType=FT_OPTION_GAUGE;
SMenu[MN_SYSTEM_OPTION].nField[9].x=159;
SMenu[MN_SYSTEM_OPTION].nField[9].y=82;
SMenu[MN_SYSTEM_OPTION].nField[9].nImageNumber=781;

// 점찍기
SMenu[MN_SYSTEM_OPTION].nField[10].nType=FT_HIDE_SPECIAL_WILLDO_PUT;		// 클릭후 어떤 필드를 ft_nomal put으로 만든다.
SMenu[MN_SYSTEM_OPTION].nField[10].nSpecialWillDo=SWD_NORMAL_CHECK_PUT;
SMenu[MN_SYSTEM_OPTION].nField[10].nWillDo=11;
SMenu[MN_SYSTEM_OPTION].nField[10].x=42;
SMenu[MN_SYSTEM_OPTION].nField[10].y=247;
SMenu[MN_SYSTEM_OPTION].nField[10].nImageNumber=0;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[10].rCheakBox,39,237,56,267);

SMenu[MN_SYSTEM_OPTION].nField[11].nType=system_info.gore;
SMenu[MN_SYSTEM_OPTION].nField[11].x=42;
SMenu[MN_SYSTEM_OPTION].nField[11].y=247;
SMenu[MN_SYSTEM_OPTION].nField[11].nImageNumber=0;

SMenu[MN_SYSTEM_OPTION].nField[12].nType=FT_HIDE_SPECIAL_WILLDO_PUT;		// 클릭후 어떤 필드를 ft_nomal put으로 만든다.
SMenu[MN_SYSTEM_OPTION].nField[12].nSpecialWillDo=SWD_NORMAL_CHECK_PUT;
SMenu[MN_SYSTEM_OPTION].nField[12].nWillDo=13;
SMenu[MN_SYSTEM_OPTION].nField[12].x=105;
SMenu[MN_SYSTEM_OPTION].nField[12].y=247;
SMenu[MN_SYSTEM_OPTION].nField[12].nImageNumber=0;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[12].rCheakBox,100,237,120,267);

SMenu[MN_SYSTEM_OPTION].nField[13].nType=FT_NOMAL_PUT;
SMenu[MN_SYSTEM_OPTION].nField[13].x=105;
SMenu[MN_SYSTEM_OPTION].nField[13].y=247;
SMenu[MN_SYSTEM_OPTION].nField[13].nImageNumber=0;

SMenu[MN_SYSTEM_OPTION].nField[14].nType=FT_HIDE_SPECIAL_WILLDO_PUT;		// 클릭후 어떤 필드를 ft_nomal put으로 만든다.
SMenu[MN_SYSTEM_OPTION].nField[14].nSpecialWillDo=SWD_NORMAL_CHECK_PUT;
SMenu[MN_SYSTEM_OPTION].nField[14].nWillDo=15;
SMenu[MN_SYSTEM_OPTION].nField[14].x=166;
SMenu[MN_SYSTEM_OPTION].nField[14].y=247;
SMenu[MN_SYSTEM_OPTION].nField[14].nImageNumber=0;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[14].rCheakBox,160,237,182,267);

SMenu[MN_SYSTEM_OPTION].nField[15].nType=FT_NOMAL_PUT;
SMenu[MN_SYSTEM_OPTION].nField[15].x=166;
SMenu[MN_SYSTEM_OPTION].nField[15].y=247;
SMenu[MN_SYSTEM_OPTION].nField[15].nImageNumber=0;

SMenu[MN_SYSTEM_OPTION].nField[16].nType=FT_HIDE_SPECIAL_WILLDO_PUT;		// 클릭후 어떤 필드를 ft_nomal put으로 만든다.
SMenu[MN_SYSTEM_OPTION].nField[16].nSpecialWillDo=SWD_NORMAL_CHECK_PUT;
SMenu[MN_SYSTEM_OPTION].nField[16].nWillDo=17;
SMenu[MN_SYSTEM_OPTION].nField[16].x=230;
SMenu[MN_SYSTEM_OPTION].nField[16].y=247;
SMenu[MN_SYSTEM_OPTION].nField[16].nImageNumber=0;
SetRect(SMenu[MN_SYSTEM_OPTION].nField[16].rCheakBox,225,240,246,263);

SMenu[MN_SYSTEM_OPTION].nField[17].nType=FT_NOMAL_PUT;
SMenu[MN_SYSTEM_OPTION].nField[17].x=230;
SMenu[MN_SYSTEM_OPTION].nField[17].y=247;
SMenu[MN_SYSTEM_OPTION].nField[17].nImageNumber=0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//메인인터페이스에서 시스템 메뉴에서 help메뉴


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//메인인터페이스에서 시스템 메뉴에서 help메뉴에서 key

///////////////////////////////////////////////////////////////////////////////
SMenu[MN_LEVELUP].x=1;
SMenu[MN_LEVELUP].y=371+GABY_SCREEN*2-33;//020515 lsw
SMenu[MN_LEVELUP].nFieldCount = 1;
SMenu[MN_LEVELUP].nImageNumber = 0;

// level up button
SMenu[MN_LEVELUP].nField[0].nType=FT_LEVELUP_BUTTON;
SMenu[MN_LEVELUP].nField[0].x=0;
SMenu[MN_LEVELUP].nField[0].y=0;
SMenu[MN_LEVELUP].nField[0].nRectImage=864;
SMenu[MN_LEVELUP].nField[0].nImageNumber=863;
SetRect(SMenu[MN_LEVELUP].nField[0].rCheakBox,0,0,50,50);


///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// 공구 창
// 0908 kkh 참고 필요 

SMenu[MN_ITEM_MAKE].nImageType = ITEM_MAKE_IMAGE;
SMenu[MN_ITEM_MAKE].x=265;
SMenu[MN_ITEM_MAKE].y=227;
SMenu[MN_ITEM_MAKE].nFieldCount = 5;
SMenu[MN_ITEM_MAKE].nImageNumber= 0;

// MAKE button
SMenu[MN_ITEM_MAKE].nField[0].nImageType = ITEM_MAKE_IMAGE;
SMenu[MN_ITEM_MAKE].nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
SMenu[MN_ITEM_MAKE].nField[0].nSpecialWillDo=SWD_ITEM_MAKE;
SMenu[MN_ITEM_MAKE].nField[0].x=12;
SMenu[MN_ITEM_MAKE].nField[0].y=127;
SMenu[MN_ITEM_MAKE].nField[0].nRectImage=1;
SMenu[MN_ITEM_MAKE].nField[0].nImageNumber=2;
SetRect(SMenu[MN_ITEM_MAKE].nField[0].rCheakBox,12,127,102,147);

// CANCEL button
SMenu[MN_ITEM_MAKE].nField[1].nImageType = ITEM_MAKE_IMAGE;
SMenu[MN_ITEM_MAKE].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT ;
SMenu[MN_ITEM_MAKE].nField[1].nSpecialWillDo=SWD_ITEM_MAKE_CANCEL;
SMenu[MN_ITEM_MAKE].nField[1].x=111;
SMenu[MN_ITEM_MAKE].nField[1].y=127;
SMenu[MN_ITEM_MAKE].nField[1].nRectImage=3;
SMenu[MN_ITEM_MAKE].nField[1].nImageNumber=4;
SetRect(SMenu[MN_ITEM_MAKE].nField[1].rCheakBox,111,127,201,147);

SMenu[MN_ITEM_MAKE].nField[2].nImageType = ITEM_MAKE_IMAGE;
SMenu[MN_ITEM_MAKE].nField[2].nType = FT_DATA_PUT;
SMenu[MN_ITEM_MAKE].nField[2].nImageNumber=DP_NORMAL_PUT;// 나중에 스킬에 따른 구분을 준다 그래서 

// 결과물 아이템 계속 보여주기
SMenu[MN_ITEM_MAKE].nField[3].nType = FT_DATA_PUT;
SMenu[MN_ITEM_MAKE].nField[3].nWillDo = 8;			// 밝게 찍기
SMenu[MN_ITEM_MAKE].nField[3].x=13;
SMenu[MN_ITEM_MAKE].nField[3].y=44;
SMenu[MN_ITEM_MAKE].nField[3].nImageNumber=DP_ITEM_PUT;
SMenu[MN_ITEM_MAKE].nField[3].nValue=&item_select_result.item_no;		// 아이템을 찍기

// item 놓아서 출력 
SMenu[MN_ITEM_MAKE].nField[4].nType = FT_MAKE_ITEM ;
//	SetRect(SMenu[MN_ITEM_MAKE].nField[2].rCheakBox,10,10,194,159);
SMenu[MN_ITEM_MAKE].nField[4].nImageType = ITEM_MAKE_IMAGE;
SetRect(SMenu[MN_ITEM_MAKE].nField[4].rCheakBox,70,45,190,95);





////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//	스킬 사용시 필요 아이템 선택 창

SMenu[MN_ITEM_SELECT].x=197+GABX_SCREEN;
SMenu[MN_ITEM_SELECT].y=20+GABY_SCREEN;
SMenu[MN_ITEM_SELECT].nImageNumber=638;
SMenu[MN_ITEM_SELECT].nFieldCount=7;

// ok, cancel  Button 
SMenu[MN_ITEM_SELECT].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
SMenu[MN_ITEM_SELECT].nField[0].nSpecialWillDo=SWD_ITEM_USE_SELECT_OK;
SMenu[MN_ITEM_SELECT].nField[0].x=26;
SMenu[MN_ITEM_SELECT].nField[0].y=270;
SMenu[MN_ITEM_SELECT].nField[0].nRectImage=294;
SMenu[MN_ITEM_SELECT].nField[0].nImageNumber=295;
SetRect(SMenu[MN_ITEM_SELECT].nField[0].rCheakBox,26,270,66 ,310);

SMenu[MN_ITEM_SELECT].nField[1].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
SMenu[MN_ITEM_SELECT].nField[1].nSpecialWillDo=SWD_SKILL_CANCEL;
SMenu[MN_ITEM_SELECT].nField[1].x=180;
SMenu[MN_ITEM_SELECT].nField[1].y=270;
SMenu[MN_ITEM_SELECT].nField[1].nRectImage=292;
SMenu[MN_ITEM_SELECT].nField[1].nImageNumber=293;
SetRect(SMenu[MN_ITEM_SELECT].nField[1].rCheakBox,180,270,220,310);

// 아이템 화면 출력
SMenu[MN_ITEM_SELECT].nField[2].nType=FT_ITEM_USE_SELECT;
SMenu[MN_ITEM_SELECT].nField[2].x=32;
SMenu[MN_ITEM_SELECT].nField[2].y=66;
SMenu[MN_ITEM_SELECT].nField[2].nSHideNomalCount=5;
SMenu[MN_ITEM_SELECT].nField[2].nSHideNomalStart=0;
SMenu[MN_ITEM_SELECT].nField[2].nShideNomalPlus=1;
SetRect(SMenu[MN_ITEM_SELECT].nField[2].rCheakBox,32,66,210,142);

// scroll button
SMenu[MN_ITEM_SELECT].nField[3].nType=FT_HIDE_DSCROLL_AUTO_PUT;
SMenu[MN_ITEM_SELECT].nField[3].nWillDo=2;
SMenu[MN_ITEM_SELECT].nField[3].x=171;
SMenu[MN_ITEM_SELECT].nField[3].y=161;
SMenu[MN_ITEM_SELECT].nField[3].nRectImage=641;
SMenu[MN_ITEM_SELECT].nField[3].nImageNumber=642;
SetRect(SMenu[MN_ITEM_SELECT].nField[3].rCheakBox,171,161,197,177);
// down
SMenu[MN_ITEM_SELECT].nField[4].nType=FT_HIDE_ISCROLL_AUTO_PUT;
SMenu[MN_ITEM_SELECT].nField[4].nWillDo=2;
SMenu[MN_ITEM_SELECT].nField[4].x=200;
SMenu[MN_ITEM_SELECT].nField[4].y=161;
SMenu[MN_ITEM_SELECT].nField[4].nRectImage=643;
SMenu[MN_ITEM_SELECT].nField[4].nImageNumber=644;
SetRect(SMenu[MN_ITEM_SELECT].nField[4].rCheakBox,200,161,226,177);

// 제목 띄우기	
SMenu[MN_ITEM_SELECT].nField[5].nType=FT_NOMAL_PUT;
SMenu[MN_ITEM_SELECT].nField[5].x=80;
SMenu[MN_ITEM_SELECT].nField[5].y=6;
SMenu[MN_ITEM_SELECT].nField[5].nImageNumber=640;		//상황에 따라 바뀜

// 선택 아이템 출력
SMenu[MN_ITEM_SELECT].nField[6].nType=FT_DATA_PUT;
SMenu[MN_ITEM_SELECT].nField[6].x=65;
SMenu[MN_ITEM_SELECT].nField[6].y=195;
SMenu[MN_ITEM_SELECT].nField[6].nRectImage=155;
SMenu[MN_ITEM_SELECT].nField[6].nImageNumber=DP_USE_SELECT;


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//	파티원들 메인 화면에 얼굴 띄우기	// 메인 메뉴에 물려 둔다... --> str 표시하는 곳에..
//  1번 파티..
SMenu[MN_PARTY_FACE1].x=22;
SMenu[MN_PARTY_FACE1].y=20;
SMenu[MN_PARTY_FACE1].nImageNumber=1022;
SMenu[MN_PARTY_FACE1].nFieldCount=1;

// ok, cancel  Button 
SMenu[MN_PARTY_FACE1].nField[0].nType=FT_PARTY_FACE_PUT;
SMenu[MN_PARTY_FACE1].nField[0].nWillDo=0;						// 파티원의 위치...
SMenu[MN_PARTY_FACE1].nField[0].x=0;
SMenu[MN_PARTY_FACE1].nField[0].y=0;
SMenu[MN_PARTY_FACE1].nField[0].nRectImage=1026;
SMenu[MN_PARTY_FACE1].nField[0].nImageNumber=1024;				// hp 게이지
SetRect(SMenu[MN_PARTY_FACE1].nField[0].rCheakBox,0,0,35,41);

////////////////////////////////////////////////////////////////////////////////////////////
//	파티원들 메인 화면에 얼굴 띄우기	// 메인 메뉴에 물려 둔다... --> str 표시하는 곳에..
//  2번 파티..
SMenu[MN_PARTY_FACE2].x=70;
SMenu[MN_PARTY_FACE2].y=20;
SMenu[MN_PARTY_FACE2].nImageNumber=1022;
SMenu[MN_PARTY_FACE2].nFieldCount=1;

// ok, cancel  Button 
SMenu[MN_PARTY_FACE2].nField[0].nType=FT_PARTY_FACE_PUT;
SMenu[MN_PARTY_FACE2].nField[0].nWillDo=1;						// 파티원의 위치...
SMenu[MN_PARTY_FACE2].nField[0].x=0;
SMenu[MN_PARTY_FACE2].nField[0].y=0;
SMenu[MN_PARTY_FACE2].nField[0].nRectImage=1026;
SMenu[MN_PARTY_FACE2].nField[0].nImageNumber=1024;				// hp 게이지
SetRect(SMenu[MN_PARTY_FACE2].nField[0].rCheakBox,0,0,35,41);

////////////////////////////////////////////////////////////////////////////////////////////
//	파티원들 메인 화면에 얼굴 띄우기	// 메인 메뉴에 물려 둔다... --> str 표시하는 곳에..
//  3번 파티..
SMenu[MN_PARTY_FACE3].x=118;
SMenu[MN_PARTY_FACE3].y=20;
SMenu[MN_PARTY_FACE3].nImageNumber=1022;
SMenu[MN_PARTY_FACE3].nFieldCount=1;

// ok, cancel  Button 
SMenu[MN_PARTY_FACE3].nField[0].nType=FT_PARTY_FACE_PUT;
SMenu[MN_PARTY_FACE3].nField[0].nWillDo=2;						// 파티원의 위치...
SMenu[MN_PARTY_FACE3].nField[0].x=0;
SMenu[MN_PARTY_FACE3].nField[0].y=0;
SMenu[MN_PARTY_FACE3].nField[0].nRectImage=1026;
SMenu[MN_PARTY_FACE3].nField[0].nImageNumber=1024;				// hp 게이지
SetRect(SMenu[MN_PARTY_FACE3].nField[0].rCheakBox,0,0,35,41);

////////////////////////////////////////////////////////////////////////////////////////////
//	파티원들 메인 화면에 얼굴 띄우기	// 메인 메뉴에 물려 둔다... --> str 표시하는 곳에..
//  4번 파티..
SMenu[MN_PARTY_FACE4].x=166;
SMenu[MN_PARTY_FACE4].y=20;
SMenu[MN_PARTY_FACE4].nImageNumber=1022;
SMenu[MN_PARTY_FACE4].nFieldCount=1;

// ok, cancel  Button 
SMenu[MN_PARTY_FACE4].nField[0].nType=FT_PARTY_FACE_PUT;
SMenu[MN_PARTY_FACE4].nField[0].nWillDo=3;						// 파티원의 위치...
SMenu[MN_PARTY_FACE4].nField[0].x=0;
SMenu[MN_PARTY_FACE4].nField[0].y=0;
SMenu[MN_PARTY_FACE4].nField[0].nRectImage=1026;
SMenu[MN_PARTY_FACE4].nField[0].nImageNumber=1024;				// hp 게이지
SetRect(SMenu[MN_PARTY_FACE4].nField[0].rCheakBox,0,0,35,41);

////////////////////////////////////////////////////////////////////////////////////////////
//	파티원들 메인 화면에 얼굴 띄우기	// 메인 메뉴에 물려 둔다... --> str 표시하는 곳에..
//  5번 파티..
SMenu[MN_PARTY_FACE5].x=214;
SMenu[MN_PARTY_FACE5].y=20;
SMenu[MN_PARTY_FACE5].nImageNumber=1022;
SMenu[MN_PARTY_FACE5].nFieldCount=1;

// ok, cancel  Button 
SMenu[MN_PARTY_FACE5].nField[0].nType=FT_PARTY_FACE_PUT;
SMenu[MN_PARTY_FACE5].nField[0].nWillDo=4;						// 파티원의 위치...
SMenu[MN_PARTY_FACE5].nField[0].x=0;
SMenu[MN_PARTY_FACE5].nField[0].y=0;
SMenu[MN_PARTY_FACE5].nField[0].nRectImage=1026;
SMenu[MN_PARTY_FACE5].nField[0].nImageNumber=1024;				// hp 게이지
SetRect(SMenu[MN_PARTY_FACE5].nField[0].rCheakBox,0,0,35,41);

////////////////////////////////////////////////////////////////////////////////////////////
//	파티원들 메인 화면에 얼굴 띄우기	// 메인 메뉴에 물려 둔다... --> str 표시하는 곳에..
//  6번 파티..
SMenu[MN_PARTY_FACE6].x=262;
SMenu[MN_PARTY_FACE6].y=20;
SMenu[MN_PARTY_FACE6].nImageNumber=1022;
SMenu[MN_PARTY_FACE6].nFieldCount=1;

// ok, cancel  Button 
SMenu[MN_PARTY_FACE6].nField[0].nType=FT_PARTY_FACE_PUT;
SMenu[MN_PARTY_FACE6].nField[0].nWillDo=5;						// 파티원의 위치...
SMenu[MN_PARTY_FACE6].nField[0].x=0;
SMenu[MN_PARTY_FACE6].nField[0].y=0;
SMenu[MN_PARTY_FACE6].nField[0].nRectImage=1026;
SMenu[MN_PARTY_FACE6].nField[0].nImageNumber=1024;				// hp 게이지
SetRect(SMenu[MN_PARTY_FACE6].nField[0].rCheakBox,0,0,35,41);


////////////////////////////////////////////////////////////////////////////////////////////
//		스몰 멥 메뉴

SMenu[MN_SMALL_MAP].x=0;
SMenu[MN_SMALL_MAP].y=0;
SMenu[MN_SMALL_MAP].nImageNumber=0;
//	SMenu[MN_SMALL_MAP].nImageType=SMALL_MAP;//이거 있으면 막 찍힘 
SMenu[MN_SMALL_MAP].nFieldCount=2;

SMenu[MN_SMALL_MAP].nField[0].nType = FT_VIEW_SMALL_MAP;

// 드래그를 이용한 메뉴 이동
//	SMenu[MN_SMALL_MAP].nField[1].nType=FT_MENU_MOVE_DIRECT;
//	SetRect(SMenu[MN_SMALL_MAP].nField[1].rCheakBox,0,0,123,79);
//////////////////////////////////////////////////////////////////////////////////////////
//	이정표 메뉴
SMenu[MN_DISTANCE_TABLE].x=0;
SMenu[MN_DISTANCE_TABLE].y=0;
SMenu[MN_DISTANCE_TABLE].nImageNumber=0;
SMenu[MN_DISTANCE_TABLE].nFieldCount=4;

SMenu[MN_DISTANCE_TABLE].nField[0].nType = FT_PUT_DISTANCE;
SMenu[MN_DISTANCE_TABLE].nField[0].nWillDo=1;
SMenu[MN_DISTANCE_TABLE].nField[0].x=0;
SMenu[MN_DISTANCE_TABLE].nField[0].y=163;
SMenu[MN_DISTANCE_TABLE].nField[0].nImageNumber=1035;

SMenu[MN_DISTANCE_TABLE].nField[1].nType = FT_PUT_DISTANCE;
SMenu[MN_DISTANCE_TABLE].nField[1].nWillDo=1;
SMenu[MN_DISTANCE_TABLE].nField[1].x=0;
SMenu[MN_DISTANCE_TABLE].nField[1].y=198;
SMenu[MN_DISTANCE_TABLE].nField[1].nImageNumber=1035;

SMenu[MN_DISTANCE_TABLE].nField[2].nType = FT_PUT_DISTANCE;
SMenu[MN_DISTANCE_TABLE].nField[2].nWillDo=1;
SMenu[MN_DISTANCE_TABLE].nField[2].x=0;
SMenu[MN_DISTANCE_TABLE].nField[2].y=233;
SMenu[MN_DISTANCE_TABLE].nField[2].nImageNumber=1035;

SMenu[MN_DISTANCE_TABLE].nField[3].nType = FT_PUT_DISTANCE;
SMenu[MN_DISTANCE_TABLE].nField[3].nWillDo=1;
SMenu[MN_DISTANCE_TABLE].nField[3].x=0;
SMenu[MN_DISTANCE_TABLE].nField[3].y=268;
SMenu[MN_DISTANCE_TABLE].nField[3].nImageNumber=1035;

///////////////////////////////////////////////////////////////////////////////////////////
//	상점 간판 메뉴
SMenu[MN_STORE_SIGN].x=440+GABX_SCREEN;
SMenu[MN_STORE_SIGN].y=5;
SMenu[MN_STORE_SIGN].nImageNumber=1040;
SMenu[MN_STORE_SIGN].nFieldCount=1;

SMenu[MN_STORE_SIGN].nField[0].nType = FT_STORE_SIGN;
SMenu[MN_STORE_SIGN].nField[0].x=20;
SMenu[MN_STORE_SIGN].nField[0].y=12;

///////////////////////////////////////////////////////////////////////////////////
// 팀 배틀 전광판
SMenu[MN_FIGHT_MAP_SIGN].x=170;
SMenu[MN_FIGHT_MAP_SIGN].y=0;
SMenu[MN_FIGHT_MAP_SIGN].nImageNumber=24;
SMenu[MN_FIGHT_MAP_SIGN].nImageType=GUILD_WAIT;
SMenu[MN_FIGHT_MAP_SIGN].nFieldCount=5;

SMenu[MN_FIGHT_MAP_SIGN].nField[0].nType = FT_FIGHT_MAP_SIGN;

// 레티
SMenu[MN_FIGHT_MAP_SIGN].nField[1].nType = FT_DATA_PUT;
SMenu[MN_FIGHT_MAP_SIGN].nField[1].x=69;
SMenu[MN_FIGHT_MAP_SIGN].nField[1].y=21;
SMenu[MN_FIGHT_MAP_SIGN].nField[1].nSpecialWillDo = TA_CENTER;		// 정렬값
SMenu[MN_FIGHT_MAP_SIGN].nField[1].nWillDo = FONT_COLOR_NUMBER;		// color 값
SMenu[MN_FIGHT_MAP_SIGN].nField[1].nValue=&fight_map_live_man[0];
SMenu[MN_FIGHT_MAP_SIGN].nField[1].nImageNumber=DP_DATAPUT_DEFAULT;

// 화렌차
SMenu[MN_FIGHT_MAP_SIGN].nField[2].nType = FT_DATA_PUT;
SMenu[MN_FIGHT_MAP_SIGN].nField[2].x=166;
SMenu[MN_FIGHT_MAP_SIGN].nField[2].y=21;
SMenu[MN_FIGHT_MAP_SIGN].nField[2].nSpecialWillDo = TA_CENTER;		// 정렬값
SMenu[MN_FIGHT_MAP_SIGN].nField[2].nWillDo = FONT_COLOR_NUMBER;		// color 값
SMenu[MN_FIGHT_MAP_SIGN].nField[2].nValue=&fight_map_live_man[1];
SMenu[MN_FIGHT_MAP_SIGN].nField[2].nImageNumber=DP_DATAPUT_DEFAULT;

// 게덴
SMenu[MN_FIGHT_MAP_SIGN].nField[3].nType = FT_DATA_PUT;
SMenu[MN_FIGHT_MAP_SIGN].nField[3].x=263;
SMenu[MN_FIGHT_MAP_SIGN].nField[3].y=21;
SMenu[MN_FIGHT_MAP_SIGN].nField[3].nSpecialWillDo = TA_CENTER;		// 정렬값
SMenu[MN_FIGHT_MAP_SIGN].nField[3].nWillDo = FONT_COLOR_NUMBER;		// color 값
SMenu[MN_FIGHT_MAP_SIGN].nField[3].nValue=&fight_map_live_man[2];
SMenu[MN_FIGHT_MAP_SIGN].nField[3].nImageNumber=DP_DATAPUT_DEFAULT;

// 카리스 누멘
SMenu[MN_FIGHT_MAP_SIGN].nField[4].nType = FT_DATA_PUT;
SMenu[MN_FIGHT_MAP_SIGN].nField[4].x=357;
SMenu[MN_FIGHT_MAP_SIGN].nField[4].y=21;
SMenu[MN_FIGHT_MAP_SIGN].nField[4].nSpecialWillDo = TA_CENTER;		// 정렬값
SMenu[MN_FIGHT_MAP_SIGN].nField[4].nWillDo = FONT_COLOR_NUMBER;		// color 값
SMenu[MN_FIGHT_MAP_SIGN].nField[4].nValue=&fight_map_live_man[3];
SMenu[MN_FIGHT_MAP_SIGN].nField[4].nImageNumber=DP_DATAPUT_DEFAULT;


////////////////////////////////////////////////////////////////////////////////////
// 선거 상황 판
SMenu[MN_VOTE_SIGN].x=170;
SMenu[MN_VOTE_SIGN].y=0;
SMenu[MN_VOTE_SIGN].nImageNumber=29;
SMenu[MN_VOTE_SIGN].nImageType=MAIN_ETC;
SMenu[MN_VOTE_SIGN].nFieldCount=4;

// 1
SMenu[MN_VOTE_SIGN].nField[0].nType=FT_VOTE_SCORE_PUT;
SMenu[MN_VOTE_SIGN].nField[0].x=105;
SMenu[MN_VOTE_SIGN].nField[0].y=28;
SMenu[MN_VOTE_SIGN].nField[0].nWillDo=0;

// 2
SMenu[MN_VOTE_SIGN].nField[1].nType=FT_VOTE_SCORE_PUT;
SMenu[MN_VOTE_SIGN].nField[1].x=247;
SMenu[MN_VOTE_SIGN].nField[1].y=28;
SMenu[MN_VOTE_SIGN].nField[1].nWillDo=1;

// 3
SMenu[MN_VOTE_SIGN].nField[2].nType=FT_VOTE_SCORE_PUT;
SMenu[MN_VOTE_SIGN].nField[2].x=387;
SMenu[MN_VOTE_SIGN].nField[2].y=28;
SMenu[MN_VOTE_SIGN].nField[2].nWillDo=2;

////////////////////////////////////////////////////////////////////////////////////
// tip 보기 버튼
SMenu[MN_VIEW_TIP].x=1;
SMenu[MN_VIEW_TIP].y=1;
SMenu[MN_VIEW_TIP].nImageNumber=0;
SMenu[MN_VIEW_TIP].nFieldCount=1;

// tip을 반짝이게 한다.
SMenu[MN_VIEW_TIP].nField[0].nType=FT_VIEW_TIP;
SMenu[MN_VIEW_TIP].nField[0].x=0;
SMenu[MN_VIEW_TIP].nField[0].y=0;
SMenu[MN_VIEW_TIP].nField[0].nRectImage=37;
SMenu[MN_VIEW_TIP].nField[0].nImageNumber=36;
SMenu[MN_VIEW_TIP].nField[0].nImageType=MAIN_ETC;

// Tip을 띄운다.
/*	SMenu[MN_VIEW_TIP].nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
SMenu[MN_VIEW_TIP].nField[0].nSpecialWillDo=SWD_VIEW_TIP;
SMenu[MN_VIEW_TIP].nField[0].x=0;
SMenu[MN_VIEW_TIP].nField[0].y=0;
SMenu[MN_VIEW_TIP].nField[0].nRectImage=37;
SMenu[MN_VIEW_TIP].nField[0].nImageNumber=36;
SMenu[MN_VIEW_TIP].nField[0].nImageType=MAIN_ETC;
SetRect(SMenu[MN_VIEW_TIP].nField[0].rCheakBox,0,0,80,23);
*/

////////////////////////////////////////////////////////////////////////////////////
MenuSet2();//020515 lsw
MenuSet3();// 021020 kyo
LuckyMenuSet();	//soto-030429
}




