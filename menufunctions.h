extern void CallGuildMemberListMenu();
extern int CallFriendMenu();
extern void SendWisperByLineCommand( char *str_buf );
extern bool SendPartyChatByLineCommand( char *str_buf );//020701 lsw
extern char *GetNationName( int nation );
extern void CallGetGuildItem();
extern void CallGuildItemPut();
extern int GetMoneyItemNumber();
extern void CallChangeMaster();
extern void SendGetGuildMasterAndSubMasterName( int guild_code, int type );
extern void CallHtmlMessageBox( int next_menu, char *file_name );
extern void CallNationItem();


// guild_degree_power
#define GDP_JOINING				0	// 길드 가입				
#define GDP_BBS					1	// 길드 전체 메시지			
#define GDP_CHANGE_DEGREE		2	// 길드원 직책변경			
#define GDP_GUILD_BOX			3	// 길드 창고 이용			
#define GDP_EXIT_GUILD_HIM		4	// 강제 길드 탈퇴 시키기	
#define GDP_CHANGE_MARK			5	// 길드 마크 변경			
#define GDP_CHANGE_DEGREE_INFO	6	// 길드 직책 명 변경		
#define GDP_GUILD_CHANGE_MASTER	7	// 길드 마스터 변경			
#define GDP_GUILD_BREAK			8	// 길드 제거				
#define GDP_REGIST_SUB_MASTER	9	// 부길마 등록				
#define GDP_EXIT_GUILD_SELF		10	// 스스로 길드 탈퇴			
#define GDP_BUY_GUILD_HOUSE		11	// 엠포리아 살수 있는 권한
#define GDP_MAX					11	// 맥스 처리			


extern bool CheckGuildPower( int type, LPCHARACTER ch );
extern void LoadFileText( char *file_name );
extern void CallGuildHouseInfo();
extern char *GetIsGotGuildHouse( int time );		// 시간을 가지고 소유 비소유를 판단.


#ifdef LIGHT_VERSION_
#ifndef __LIGHT_VERSION__
#define __LIGHT_VERSION__

struct k_light_version_weapon
{
	// 속도 측면에서 바꿈
	int weapon[4];
	/*int man1;
	int woman1;
	int man2;
	int woman2;*/

	k_light_version_weapon()
	{
		memset( weapon, 0, sizeof( int ) *4 );
/*		man1 = 0;
		woman1 = 0;
		man2 = 0;
		woman2 = 0;*/
	}
};

class CLightVersion
{
public :
	// 맵 이동 관련
	string *m_szlpMap;
	int m_nCount;

	// 무기 로딩 관련
	k_light_version_weapon	*m_pWeapon;
	int m_nWeaponCount;

public :
	CLightVersion()
	{
		m_nCount = 0;
		m_szlpMap = 0;
		m_pWeapon = 0;
		m_nWeaponCount = 0;
	}
	~CLightVersion()
	{
		Clear();
	}
	void Clear()
	{
		m_nCount =0;
		if( m_szlpMap ) delete [] m_szlpMap;
		m_szlpMap = 0;
		if( m_pWeapon ) delete [] m_pWeapon;
		m_nWeaponCount = 0;
	}

	int LoadLightVersionMap();
	int IsGotoMap( char *mapname );
	int LoadLightVersionWeapon();
	int GetLightVersionWeapon( int sex, int weapon_num );
};

extern CLightVersion *g_pLightVersion;

#endif	// __LIGHT_VERSION__
#endif	// LIGHT_VERSION_

//021030 YGI
extern int RecvEventObject( k_event_object *pData );
extern void SendEvnetMoveMap();	// 021107 YGI