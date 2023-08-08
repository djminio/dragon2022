#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Hong_Sprite.h"
#include "Char.h"
#include "MenuSet.h"
#include "MenuNetwork.h"
#include "object.h"
#include "linecommand.h"
#include "directsound.h"
#include "menudef2.h"
#include "menufunctions.h"
//021030 YGI
#include "gameproc.h"
#include "map.h"


extern LPCHARACTER Hero;
// 주인공 캐릭터가 길드를 신청할 수 있는지 판단
// type 이 1이면 예비 길드 type 이 2 이면 정식 길드 
// 실패시 실패 스트링 번호 반납
extern int CheckOtherCharacterGuild();
int CanRegistGuild( int type )
{	//< CSD-030324
	int nStr = -1;
	switch( type )
	{
		case 1:			// 임시 길드 등록 조건 체크
			{
				nStr = 0;
				if( Hero->GetGuildCode() )  nStr = 50;		// 이미 가입해 있다.
				else if( Hero->lv < 40 ) nStr = 51;					
				else if( GetItemMuchByMuchItem( IN_BLUE_SIGNET ) < 40 ) nStr = 52;
				else if( CheckOtherCharacterGuild() ) 
				{
					nStr = 50;
					Kein_PutMessage( KM_FAIL, kein_GetMenuString(129) ); 
				}
				break;
			}
		case 2 :		// 정식 길드 등록 조건 체크
			{
				nStr = 0;
				if( !Hero->GetGuildCode() ) nStr = 56;		// 먼저 예비 길드에 가입해야 한다.
				else if( g_GuildMgr[Hero->GetGuildCode()].first != 1 ) nStr = 50;	// 이미 다른 길드에 가입해 있다.
				else if( Hero->name_status.guild_master != 1 ) nStr = 57;  // 길드 마스터 만 정식 길드 가입을 할 수 있다.
				else if( GetItemMuchByMuchItem( IN_RED_SIGNET ) < 100 ) nStr = 52;
				break;
			}
	}

	return nStr;
}	//> CSD-030324

int CallDualInterFace(int type)
{
	for (int i = 0; i < 3; ++i)
	{
		const string strName = g_mgrDual.GetClassName(Hero->class_type,
														i, 
														Hero->aStepInfo[CLS_STEP] + 1,
														Hero->lv);
		strcpy(SMenu[MN_DUAL_MAIN].nField[i].temp, strName.c_str());
	}

	const string strNone = lan->OutputMessage(1, 38);

	switch (SCharacterData.nCharacterData[SPELL])
	{
	case WIZARD_SPELL: // 마법사 스펠을 가지고 있는 경우
		{ 
			const string strName = g_mgrDual.GetClassName(Hero->class_type,
															WIZARD, 
															Hero->aStepInfo[CLS_STEP] + 1,
															Hero->lv);
			strcpy(SMenu[MN_DUAL_MAIN].nField[WIZARD].temp, strName.c_str());
			strcpy(SMenu[MN_DUAL_MAIN].nField[PRIEST].temp, strNone.c_str());
			break;
		}
	case PRIEST_SPELL: // 성직자 스펠을 가지고 있는 경우
		{       
			const string strName = g_mgrDual.GetClassName(Hero->class_type,
															PRIEST, 
															Hero->aStepInfo[CLS_STEP] + 1,
															Hero->lv);
			strcpy(SMenu[MN_DUAL_MAIN].nField[PRIEST].temp, strName.c_str());
			strcpy(SMenu[MN_DUAL_MAIN].nField[WIZARD].temp, strNone.c_str());
			break;
		}
	}

	const string strExplain = g_mgrDual.GetDulaExplain(Hero->class_type, 0);

	strcpy(SMenu[MN_DUAL_MAIN].nField[6].temp,  strExplain.c_str());  

	SMenu[MN_DUAL_MAIN].work = 0;

	switch (type)
	{
	case MT_DUAL_CHANGE: // 듀얼	클래스로 전환
		{
			SMenu[MN_DUAL_MAIN].nImageNumber = 2;
			SMenu[MN_DUAL_MAIN].nField[5].nRectImage = 7;
			SMenu[MN_DUAL_MAIN].nField[5].nImageNumber = 6;
			break;
		}
	case MT_DUAL_DIVIDE: // 듀얼 단계 상승
		{
			SMenu[MN_DUAL_MAIN].nImageNumber = 1;
			SMenu[MN_DUAL_MAIN].nField[5].nRectImage = 10;
			SMenu[MN_DUAL_MAIN].nField[5].nImageNumber = 9;
			SMenu[MN_DUAL_MAIN].work = Hero->aStepInfo[DUAL_CLS];//030611 lsw
			break;
		}
	}

	SMenu[MN_DUAL_MAIN].nTemp = type;
	CallSmallMenu(MN_DUAL_MAIN);
	return 1;
}

void DoDualOKButton(int type, int select)
{
  MP3(SN_DUAL_SELECT);

	switch (type)
	{	//< CSD-021003
	case MT_DUAL_CHANGE: // 듀얼	클래스로 전환
		{
			MP3(SN_DUAL_OK);
			g_mgrDual.SendDualChange(Hero, select);
			break;
		}
	case MT_DUAL_DIVIDE: // 듀얼 단계 상승
		{
			MP3(SN_DUAL_OK);
			g_mgrDual.SendDualDivide(Hero, select);
			break;
		}
	} //> CSD-021003
}

int CallInitAbility()
{
	CallSmallMenu(MN_INIT_ABILITY);
	return 1;
}

void DoInitAbility()
{
  MP3(SN_DUAL_OK);
  // 어빌리티 재분배
  CallServer(CMD_RESET_ABILITY);
  // 어빌리티 인터페이스 호출
  CallMenu(MN_ABILITY);
//	CallServer(CMD_OPEN_ABILITY);	
}

void CheckGuildActive( int guild_code )
{	//< CSD-031001
	if( !guild_code ) return;
	LPCHARACTER ch = Hero;
	while( ch )
	{
		if( ch->GetGuildCode() == guild_code )
		{
			ch->SetGuildCode(0);
			ch->name_status.guild_master = 0;
		}
		ch = ch->lpNext;
	}
}	//> CSD-031001

inline bool CheckGuildCode( int guild_code )
{	//< CSD-030324
	if( !guild_code ) return 0;
	
	if( !g_GuildMgr[guild_code].m_bInfo ) 
	{
		SendReqGuildInfo1( guild_code );		// 한번 요청한다.
	}
	
	if( !g_GuildMgr[guild_code].m_bActive ) return 0;
	return 1;
}	//> CSD-030324

inline bool CheckGuildMaster( LPCHARACTER ch )
{	//< CSD-030324
	if( !ch ) return 0;
	if( ch->name_status.guild_master != 1 ) return 0;
	return CheckGuildCode( ch->GetGuildCode() );
}	//> CSD-030324

inline bool IsSubMaster( LPCHARACTER ch )
{	//< CSD-030324
	if( !ch ) return 0;
	int code = ch->GetGuildCode();
	if( !code ) return 0;
	return IsSubMaster( code, ch->name );
}	//> CSD-030324

inline bool IsSubMaster( int guild_code, char *name )
{	//< CSD-030324
	if( strcmp( g_GuildMgr[guild_code].m_szSubGuldMasterName, name ) == 0 ) return 1;
	return 0;
}	//> CSD-030324

int CallGuildChangeInfo()
{	//< CSD-030324
	if( !CheckGuildCode( Hero->GetGuildCode() ) ) 
	{
		CallOkCancelMessageBox( y_MenuFocus, 0, 0, kein_GetMenuString( 75 ), 0 );
		return 0;
	}

	//SmallMenuClose();

	// 서브 마스터의 이름을 가져온다.
	SendGetGuildMasterAndSubMasterName( Hero->GetGuildCode(), 2 );

	if( CheckGuildMaster( Hero ) )		// 길드 마스터 라면
	{
		SMenu[MN_GUILD_CHANGE_INFO].nField[1].nType=FT_PUT_BUTTON;
		SMenu[MN_GUILD_CHANGE_INFO].nField[2].nType=FT_PUT_BUTTON;		
		SMenu[MN_GUILD_CHANGE_INFO].nField[4].nType=FT_PUT_BUTTON;
		SMenu[MN_GUILD_CHANGE_INFO].nField[3].nType=FT_PUT_BUTTON;
		//SMenu[MN_GUILD_CHANGE_INFO].nField[5].nType=FT_NOMAL_PUT;		// 부 길마 정보 변경
		{
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nType=FT_PUT_BUTTON;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSpecialWillDo=SWD_SET_SUB_MASTER;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSHideNomalCount=6;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nRectImage=7;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageNumber=8;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageType=IIT_GUILD_CHANGE_INFO;
		}

		if( g_GuildMgr[Hero->GetGuildCode()].first )
		{
			SMenu[MN_GUILD_CHANGE_INFO].nField[1].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_CHANGE_INFO].nField[2].nType=FT_NOMAL_PUT;		
			SMenu[MN_GUILD_CHANGE_INFO].nField[4].nType=FT_NOMAL_PUT;
		}
	}
	else
	{
		SMenu[MN_GUILD_CHANGE_INFO].nField[1].nType=FT_NOMAL_PUT;
		SMenu[MN_GUILD_CHANGE_INFO].nField[2].nType=FT_NOMAL_PUT;
		SMenu[MN_GUILD_CHANGE_INFO].nField[3].nType=FT_NOMAL_PUT;
		SMenu[MN_GUILD_CHANGE_INFO].nField[4].nType=FT_NOMAL_PUT;
		//SMenu[MN_GUILD_CHANGE_INFO].nField[5].nType=FT_PUT_BUTTON;	// 길드 탈퇴
		{
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSpecialWillDo=SWD_CALL_OKCANCEL_BOX;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSHideNomalCount=25;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nRectImage=27;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageNumber=26;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageType=GUILD_MENU_IMG;
		}

	}

	SMenu[MN_GUILD_CHANGE_INFO].bActive = true;
	y_MenuFocus = MN_GUILD_CHANGE_INFO;
	return 1;
}	//> CSD-030324

bool CheckTempChar( char ch, char *white )
{
	int len = strlen( white );
	if( !len ) return false;

	for( int i=0; i<=len; i++ )		// NULL 까지 체크
	{
		if( ch == white[i] ) return true;
	}
	return false;
}

void DeleteBetaFile( char *dir )		// 파일을 읽어 와 폴더를 모두 지우기
{
	FILE *fp = fopen( dir, "rt" );
	if( !fp ) return;

	char temp[MAX_PATH];
	while( fgets( temp, MAX_PATH, fp ) )
	{
		EatRearWhiteChar( temp );
		if( CheckTempChar( temp[0], " ;\n#" ) ) return;

		DeleteDir( temp );
	}
	fclose( fp );
}

void RewritePathWithSpecialCharacters( char *file, char *charname) //Eleval 16/08/09 - for chars who's name has special chars
{
	char newCharName[64] = "";

	for(int i = 0; charname[i] != '\0' ; i++)
	{
		if(charname[i] != 92 && charname[i] != 47 && charname[i] != 58 && charname[i] != 42 && charname[i] != 63 && charname[i] != 34 && charname[i] != 60 && charname[i] != 62 && charname[i] != 124)
		{
			char temp[8] = "";
			sprintf(temp, "%c", charname[i]);
			strcat(newCharName, temp);
		}
		else
		{
			char SpecialChar[16] = "";
			sprintf(SpecialChar, "%c%d", 37, charname[i]);
			strcat(newCharName, SpecialChar);
		}
	}

	sprintf(file, "./character/%s.qm2", newCharName);
}


void SaveMemorize()
{
	FILE * fp;
	char file[MAX_PATH];
	sprintf( file, "./character/%s.qm2", SCharacterData.sCharacterName);//Memorize Save
	fp = Fopen( file, "wb" );
	
	if( !fp ) RewritePathWithSpecialCharacters( file, SCharacterData.sCharacterName);

	fp = Fopen( file, "wb" );
	
	if( !fp ) return;

	fwrite( &magic_plus_start, sizeof( int ), 1, fp );
	fwrite( &magic_plus_count, sizeof( int ), 1, fp );
	fwrite( magic_plus, sizeof( int ), MAX_MAGIC, fp );
	fwrite( SaveMemory, sizeof( t_QuickMemory ), MAX_MAGIC_MEMORY, fp );
	fclose( fp );
}

void LoadMemorize()
{
	FILE * fp;
	char file[MAX_PATH];
	sprintf( file, "./character/%s.qm2", SCharacterData.sCharacterName );//Memorize Save
	fp = Fopen( file, "rb" );

	if( !fp ) RewritePathWithSpecialCharacters( file, SCharacterData.sCharacterName);

	fp = Fopen( file, "rb" );

	if( !fp ) return;

	int start, plus_count;

	if( SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL )
	{
		fread( &magic_plus_start, sizeof( int ), 1, fp );
		fread( &magic_plus_count, sizeof( int ), 1, fp );
		fread( magic_plus, sizeof( int ), MAX_MAGIC, fp );
	}
	else
	{
		fread( &start, sizeof( int ), 1, fp );
		fread( &plus_count, sizeof( int ), 1, fp );
		if( magic_plus_count != plus_count )
			fseek( fp, sizeof(int)*MAX_MAGIC, SEEK_CUR );
		else
		{
			magic_plus_start = start;
			magic_plus_count = plus_count;
			fread( magic_plus, sizeof( int ), MAX_MAGIC, fp );
		}       
	}
	fread( SaveMemory, sizeof( t_QuickMemory ), MAX_MAGIC_MEMORY, fp );
	fclose( fp );
}

bool IsReporterMode()
{
	if( Hero && Hero->name_status.reporter && Hero->reporter_mode ) return true;
	else return false;
}

void CallGuildMemberListMenu()
{	//< CSD-030324
	short int guild_code = Hero->GetGuildCode();
	
	if (!guild_code) 
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 94 ) );
		return;
	}

	SendGetGuildMasterAndSubMasterName( guild_code, 3 );
	SendDirectDB( CMD_GET_GUILD_NOTICE, &guild_code, sizeof( short int ) );
	CallSmallMenu( MN_GUILD_MEMBER_LIST );

	static int pos_to_degree[] = {1, 2, 3, 4, 0 };
	int pos = SMenu[MN_GUILD_MEMBER_LIST].key;
	if( !g_Menu_Variable.m_GuildMemberName.GetRecv( pos_to_degree[pos] ) )
	{
		// 서버 요청
		k_get_guild_member_list_req data;
		data.guild_code = Hero->GetGuildCode();
		data.degree = pos_to_degree[pos];
		SendDirectDB( CMD_GET_NAME_GUILD_MEMBER_LIST, &data, sizeof( k_get_guild_member_list_req ) );
		g_Menu_Variable.m_GuildMemberName.SetRecv( pos_to_degree[pos] );
	}
}	//> CSD-030324

void SendGetGuildMasterAndSubMasterName(int guild_code, int type)
{	// 1:길마만, 2:부길마만, 3:모두
	k_req_guild_master_and_sub_master data;
	data.guild_code = guild_code;
	data.type = type;
	SendDirectDB(CMD_GUILD_MASTER_AND_SUBMASTER, &data, sizeof(k_req_guild_master_and_sub_master) - 40);
}

void GetFriendGroup()
{
	if( g_Menu_Variable.m_FriendGrup )
	{
		g_Menu_Variable.m_FriendGrup->clear();
	}
	else g_Menu_Variable.m_FriendGrup = new CFriendGrup;

	SendDirectDB( CMD_GET_FRIEND, Hero->name, strlen( Hero->name )+1 );
}

int CallFriendMenu()
{
	GetFriendGroup();

	CallSmallMenu(MN_FRIEND_MENU);
	return 1; 
}

void ClickCharacter()		//  클릭 
{
	if( SMenu[MN_FRIEND_REGIST].bActive )
	{
		EWndMgr.SetTxt(HWND_3,g_GameInfo.lpcharacter->name);//021001 lsw
	}
}

void SendSelectPartyExp( char select )
{
	SendPacketDefault( CMD_CHANGE_PARTY_SELECT_EXP, &select, 1 );
	switch( select )
	{
		case PS_EXP :
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 125 ), kein_GetMenuString( 122 ) );
			break;

		case PS_TACTIC :
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 125 ), kein_GetMenuString( 123 ) );
			break;

		case PS_MAGIC_TAC :
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 125 ), kein_GetMenuString( 124 ) );
			break;
	}
}

void CallGetGuildItem()
{
	short int guild_code = Hero->GetGuildCode(); // CSD-030324

	if( !guild_code ) 
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 75 ) );
		return;
	}
	// if( !Hero->name_status.guild_master ) return false; 	// 등급에 따른 제한

	Kein_PutMessage( KM_OK, kein_GetMenuString( 144 ) );
	if( g_Menu_Variable.m_pGuildItem )
	{
		delete g_Menu_Variable.m_pGuildItem ;
		g_Menu_Variable.m_pGuildItem = 0;
	}

	CloseAllMenu();
	
	SendDirectDB( CMD_GET_GUILD_ITEM_FIRST, &guild_code, sizeof( short int ) );
	SendDirectDB( CMD_GET_GUILD_BOX_MONEY, &guild_code, sizeof( short int ) );

	CallMenu( MN_GUILD_ITEM_GET);
	CallMenu( MN_ITEM );

	return;
}

void CallGuildItemPut()
{	//< CSD-030324
	CloseAllMenu();
	CallMenu( MN_ITEM );
	CallMenu( MN_GUILD_ITEM_PUT);
	
	short int guild_code = Hero->GetGuildCode();
	SendDirectDB( CMD_GET_GUILD_ITEM_COUNT, &guild_code, sizeof( short int ) );		// 아이템 갯수 갱신
}	//> CSD-030324

int GetMoneyItemNumber()
{
#ifdef CHANGE_MONEY_
	return IN_NEW_COINS;
	
#else
	return IN_COINS;
#endif	
}

// 길드 마스터 변경 버튼을 눌렀을때, 메시지 박스 띄우기 전 해야 할일
void CallChangeMaster()
{	//< CSD-030324
	if( CheckGuildMaster( Hero ) )		// 길마가 버튼을 눌렀을때
	{
		SendGetGuildMasterAndSubMasterName( Hero->GetGuildCode() , 2 );		// 서브 마스터 이름 가져오기
		strcpy( SMenu[MN_GUILD_CHANGE_INFO].nField[4].temp, kein_GetMenuString( 137 ) );
	}
	else		// 부 길마가 버튼을 눌렀을때
	{
		strcpy( SMenu[MN_GUILD_CHANGE_INFO].nField[4].temp, kein_GetMenuString( 139 ) );
	}
}	//> CSD-030324

void CallHtmlMessageBox( int next_menu, char *file_name )
{
	if( is_mainmenu_active ) CallSmallMenu( MN_HTML_MESSAGEBOX );
	else 
	{
		SMenu[MN_HTML_MESSAGEBOX].bActive = true;
		y_MenuFocus = MN_HTML_MESSAGEBOX;
	}
	
	int line = g_Menu_Variable.LoadingGuildRegistExplain( file_name );

	WORD page =0;
	WORD page_max = line/5+1;
	SMenu[MN_HTML_MESSAGEBOX].work = MAKELONG( page, page_max );
	SMenu[MN_HTML_MESSAGEBOX].key = line;

	SMenu[MN_HTML_MESSAGEBOX].nTemp = next_menu;
}

int GetDegree2Param( int degree )		// degree 값을 파라메터로 변경한다.
{
	static int degree2param[] = { 4, 0, 1, 2, 3 };
	return degree2param[degree];
}

// 020620 YGI
bool CheckGuildPower( int type, LPCHARACTER ch )
{	//< CSD-030324
	if( !ch ) return 0;
	if( !ch->GetGuildCode() ) return 0;
	if( type < 0 || type > GDP_MAX ) return 0;
	static bool power_table[][5] = {
		{ 1, 1, 0, 0, 0 },		// 길드 가입				// GDP_JOINING
		{ 1, 1, 0, 0, 0 },		// 길드 전체 메시지			// GDP_BBS
		{ 1, 0, 0, 0, 0 },		// 길드원 직책변경			// GDP_CHANGE_DEGREE		// 020529 acer
		{ 1, 1, 0, 0, 0 },		// 길드 창고 이용			// GDP_GUILD_BOX
		{ 1, 1, 0, 0, 0 },		// 강제 길드 탈퇴 시키기	// GDP_EXIT_GUILD_HIM
		{ 1, 0, 0, 0, 0 },		// 길드 마크 변경			// GDP_CHANGE_MARK
		{ 1, 0, 0, 0, 0 },		// 길드 직책 명 변경		// GDP_CHANGE_DEGREE_INFO
		{ 1, 0, 0, 0, 0 },		// 길드 마스터 변경			// GDP_GUILD_CHANGE_MASTER
		{ 1, 0, 0, 0, 0 },		// 길드 제거				// GDP_GUILD_BREAK
		{ 1, 0, 0, 0, 0 },		// 부길마 등록				// GDP_REGIST_SUB_MASTER
		{ 0, 1, 1, 1, 1 },		// 스스로 길드 탈퇴			// GDP_EXIT_GUILD_SELF
		{ 1, 0, 0, 0, 0 },		// 엠포리아 사기 권한		// GDP_BUY_GUILD_HOUSE
	};

	int para = GetDegree2Param( ch->name_status.guild_master );
	return power_table[type][para];
}	//> CSD-030324

void LoadFileText( char *file_name )
{
	FILE *fp = Fopen( file_name, "rt" );
	if( !fp ) return;

	/*
	if( g_Menu_Variable.m_szFileTextPut ) 
	{
		delete [] g_Menu_Variable.m_szFileTextPut;
		g_Menu_Variable.m_szFileTextPut = 0;
	}
	*/

	char temp[1024];
	while( fgets( temp, 1024, fp ) )
	{
		if( CheckContinueLine( temp[0] ) ) continue;
		g_Menu_Variable.m_szFileTextPut += temp;
		g_Menu_Variable.m_szFileTextPut += "\n";
	}

	fclose( fp );
}

void CallNationItem()
{
	CallServer( CMD_GET_NATION_ITEM );
	CallSmallMenu( MN_NATION_ITEM );
}

void CallGuildHouseInfo()
{
	char nation = Hero->name_status.nation;
	CallServer( CMD_GUILD_HOUSE_INFO );
	//SendDirectDB( CMD_GUILD_HOUSE_INFO, &nation, 1 );
}

char *GetIsGotGuildHouse( int time )		// 시간을 가지고 소유 비소유를 판단.
{
	int mon, day, hour;
	GetRealTime( mon, day, hour );
	int str = 0;
	if( !time )
	{
		str = 166; // 없음
	}
	else if( time > 30 && day >= 25 )
	{
		str = 164; // 연장
	}
	else if( time < 5 || day >= 25 )
	{
		str = 165; // 미연장						
	}
	else
	{
		str = 163; // 소유
	}
	return kein_GetMenuString( str ); 
}


//020808 YGI -----------------------------
bool DecoadFile( char *filename, void *pData, int size )
{
	CHSEL_STREAM m_hsel;
	FILE *fp = Fopen( filename, "rb" );
	if( !fp ) return false;
	
	int nVersion = 0;
	fread( (void*)(&nVersion), sizeof(int), 1, fp ); 
	if (m_hsel.GetVersion() != nVersion)  return false;
	HselInit deinit;
	fread( (void*)(&deinit), sizeof(HselInit), 1, fp );
	if (!m_hsel.Initial(deinit))  return false;
	fread( pData, 1, size, fp );
	m_hsel.Decrypt((char *)pData, size);
	fclose( fp );
	return true;
}

bool IncordFile( char *filename, void *pData, int size )
{
	CHSEL_STREAM m_hsel;
	FILE *fp = Fopen( filename, "wb" );
	if( !fp ) return false;

	HselInit eninit;
	eninit.iEncryptType	=	HSEL_ENCRYPTTYPE_RAND;
	eninit.iDesCount	=	HSEL_DES_TRIPLE;
	eninit.iCustomize	=	HSEL_KEY_TYPE_DEFAULT;
	eninit.iSwapFlag	=	HSEL_SWAP_FLAG_ON;

	if (!m_hsel.Initial(eninit)) 
	{
		return false;
	}

	HselInit deinit;
	deinit = m_hsel.GetHSELCustomizeOption();
	const int nVersion = m_hsel.GetVersion();
	fwrite((void *)(&nVersion), sizeof(nVersion), 1, fp ); 
	fwrite((void *)(&deinit), sizeof(HselInit), 1, fp ); 
	m_hsel.Encrypt((char*)pData, size);
	fwrite( pData, 1, size, fp );
	fclose( fp );
	return true;
}
// ---------------------------------------------------

#ifdef LIGHT_VERSION_
CLightVersion *g_pLightVersion;
int CLightVersion::LoadLightVersionMap()
{
	FILE *fp;
	fp = Fopen( "./data/light_map.txt", "rt" );
	if( !fp ) return 0;

	Clear();		// 변수 클리어

	char temp[255];
	while( fgets( temp, 255, fp ) )
	{
		if( CheckContinueLine( temp[0] ) ) continue;
		m_nCount++;
	}

	fseek( fp, 0, SEEK_SET );
	m_szlpMap = new string[m_nCount];

	int count = 0;
	while( fgets( temp, 255, fp ) )
	{
		if( CheckContinueLine( temp[0] ) ) continue;
		strupr( temp );
		m_szlpMap[count++] = temp;
	}

	fclose( fp );
	return 1;
}

int CLightVersion::IsGotoMap( char *mapname )
{
	for( int i=0; i<m_nCount; i++ )
	{
		if( strcmp( mapname, m_szlpMap[i].c_str() ) == 0 ) return 1;
	}
	return 0;
}

int CLightVersion::LoadLightVersionWeapon()
{
	FILE *fp;
	fp = Fopen( "./data/light_weapon.txt", "rt" );
	if( !fp ) return 0;

	Clear();		// 변수 클리어

	char temp[255];
	while( fgets( temp, 255, fp ) )
	{
		if( CheckContinueLine( temp[0] ) ) continue;
		m_nWeaponCount++;
	}

	fseek( fp, 0, SEEK_SET );
	m_pWeapon = new k_light_version_weapon[m_nWeaponCount];

	int count = 0;
	char *token;
	int data = 0;
	while( fgets( temp, 255, fp ) )
	{
		if( CheckContinueLine( temp[0] ) ) continue;
		
		token = strtok( temp, "\t\n" );
		if( !token ) continue;
		data = atoi( token );
		m_pWeapon[count].weapon[0] = data;

		token = strtok( NULL, "\t\n" );
		data = atoi( token );
		m_pWeapon[count].weapon[1] = data;

		token = strtok( NULL, "\t\n" );
		data = atoi( token );
		m_pWeapon[count].weapon[2] = data;

		token = strtok( NULL, "\t\n" );
		data = atoi( token );
		m_pWeapon[count].weapon[3] = data;

		count++;
	}

	fclose( fp );
	return 1;
}
//021030 YGI
int CLightVersion::GetLightVersionWeapon( int sex, int weapon_num )
{
	if( !weapon_num ) return 0;
	// 0이 여자 1이 남자이기 때문에 순서를 뒤집는다.

	sex = !sex;

	for( int i=0; i<m_nWeaponCount; i++ )
	{
		if( m_pWeapon[i].weapon[sex] == weapon_num ) 
			return m_pWeapon[i].weapon[sex+2];
	}
	return 0;
}

#endif


//021030 YGI
int RecvEventObject( k_event_object *pData )
{
	k_event_object_sound *pData3 = (k_event_object_sound *)pData;
	k_event_sound *pData2 = (k_event_sound *)pData;

	int sound = ReturnBGMNumber(MapNumber);
	if( sound > 0  )
	{
		StopWave(sound);
	}

	RainStop(); //Eleval 12/08/09
	ThunderStop(); //Eleval 12/08/09
	SnowStop(); //Eleval 12/08/09

	if( g_Menu_Variable.m_pEventObjcet )
	{
		delete g_Menu_Variable.m_pEventObjcet;
		g_Menu_Variable.m_pEventObjcet = 0;
	}

	if( pData->active & ( g_Menu_Variable.ACTIVE_EVENT_OBJECT | g_Menu_Variable.ACTIVE_EVENT_SOUND ) )
	{
		g_Menu_Variable.m_pEventObjcet = new k_event_object_sound;
		if( pData->active & g_Menu_Variable.ACTIVE_EVENT_OBJECT )
		{
			*g_Menu_Variable.m_pEventObjcet = *pData3;
		}
		else
		{
			g_Menu_Variable.m_pEventObjcet->active = pData2->active;
			g_Menu_Variable.m_pEventObjcet->sound = pData2->sound;
		}
	}

	return 1;
}

// 021107 YGI
void SendEvnetMoveMap()
{
	if( !g_Menu_Variable.m_pEventMoveMap ) return;
	if( !g_Menu_Variable.m_pEventMoveMap->IsCanMove() ) return;
	CallServer( CMD_EVENT_MOVE_MAP, &g_Menu_Variable.m_pEventMoveMap->data.event_no, sizeof(int) );
}