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
#define GDP_JOINING				0	// ��� ����				
#define GDP_BBS					1	// ��� ��ü �޽���			
#define GDP_CHANGE_DEGREE		2	// ���� ��å����			
#define GDP_GUILD_BOX			3	// ��� â�� �̿�			
#define GDP_EXIT_GUILD_HIM		4	// ���� ��� Ż�� ��Ű��	
#define GDP_CHANGE_MARK			5	// ��� ��ũ ����			
#define GDP_CHANGE_DEGREE_INFO	6	// ��� ��å �� ����		
#define GDP_GUILD_CHANGE_MASTER	7	// ��� ������ ����			
#define GDP_GUILD_BREAK			8	// ��� ����				
#define GDP_REGIST_SUB_MASTER	9	// �α渶 ���				
#define GDP_EXIT_GUILD_SELF		10	// ������ ��� Ż��			
#define GDP_BUY_GUILD_HOUSE		11	// �������� ��� �ִ� ����
#define GDP_MAX					11	// �ƽ� ó��			


extern bool CheckGuildPower( int type, LPCHARACTER ch );
extern void LoadFileText( char *file_name );
extern void CallGuildHouseInfo();
extern char *GetIsGotGuildHouse( int time );		// �ð��� ������ ���� ������� �Ǵ�.


#ifdef LIGHT_VERSION_
#ifndef __LIGHT_VERSION__
#define __LIGHT_VERSION__

struct k_light_version_weapon
{
	// �ӵ� ���鿡�� �ٲ�
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
	// �� �̵� ����
	string *m_szlpMap;
	int m_nCount;

	// ���� �ε� ����
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