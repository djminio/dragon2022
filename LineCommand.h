#if !defined(AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_)
#define AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
	
class CSmallTipCount
{
public:
	char m_szKey[50];		// 020925 YGI
	int	m_nPage;
	CSmallTipCount *next;

public:
	CSmallTipCount() { next = NULL; m_nPage = 0; }
	~CSmallTipCount() { SAFE_DELETE(next); }

	int AddData( int number, char *str );
	int GetPage( char *key );
};

enum LineCommandIndex
{
	LC_NO_COMMAND				=0,
	LC_GO_NORTH					=1,
	LC_GO_WEST					=2,
	LC_GO_EAST					=3,
	LC_GO_SOUTH					=4,
	LC_EAT_ITEM					=5,
	LC_NOT_GO					=6,
	LC_CHAT_WHISPER				=7,
	LC_CHAT_PARTY				=8,
	LC_CHAT_GUILD				=9,
	LC_TOTAL_MAP_CONNECTIONS	=10,
	LC_TOTAL_CONNECTIONS		=11,
	LC_MAKE_PARTY				=12,
	LC_DEFAULT_WALK				=13,
	LC_DEFAULT_RUN				=14,
	LC_HI						=15,
	LC_HELLO					=16,
	LC_ENTERHOUSE				=17,
	LC_TAME_STOP				=18,
	LC_TAME_ATTACK				=19,
	LC_TAME_FOLLOWME			=20,
	LC_TAME_NAME				=21,
	LC_GO_STOP					=22,
	LC_GO_RANDOM				=23,
	LC_OPEN_TIMEMENU			=24,	// 0328 YGI
	LC_CLOSE_TIMEMENU			=25,
	LC_CLOSE_ALL				=26,
	LC_EAR_MESSAGE_REFUSE		=27,	// Ư����� �ӼӸ� ��� �ź�
	LC_EAR_MESSAGE_OK			=28,	// �ͼӸ� ���
	LC_REPRINT_EAR_MESSAGE		=29,	// ���� �޸� �ٽ� ����
	LC_EAR_MESSAGE_REFUSE_WHO	=30,	// �ӼӸ��ź� ���
	LC_GREETING					=40,
	LC_EXPLAIN_OPEN				=41,
	LC_EXPLAIN_CLOSE			=42,
	LC_REQ_MAN_TO_MAN			=43,
	LC_RESET_MAGIC_ARRAY		=44,
	LC_MSG_ALL_NATION			=45,
	LC_GUILD_EXIT				=46,	// ���� Ż��
	LC_GUILD_JOIN				=47,	// ���� ����
	LC_GUILD_ALL_MSG			=48,	// ��� ��ü �޽���
	LC_GUILD_CHANGE_EACH_LEVEL	=49,	// ��峻 ����� ��ȯ ��Ų��.
	LC_GUILD_CMD_INFO			=50,	// ��� ���� ���
	LC_CHANGE_JOB				=51,	// �����ִ� ������ �ٲ۴�.
	LC_STOP_WAR					=52,	// ���� ��û�Ѵ�.
	LC_SCAN_TARGET_VYSEUS		=53,	// ���� �����Ѵ�. ( ���̼��� )
	LC_SCAN_TARGET_ZYPERN		=54,	// ���� �����Ѵ�. ( ������ )
	LC_KILL_CHARACTER			=55,	// ��� ���� ���
	LC_DIRECTIONS				=56,
	LC_OPEN_DISPLAY_SKILL_EXP	=57,
	LC_CLOSE_DISPLAY_SKILL_EXP	=58,
	LC_OPEN_DISPLAY_TAC_EXP		=59,
	LC_CLOSE_DISPLAY_TAC_EXP	=60,
	LC_LIGHTOFRESCUE			=61,	// '������ ��'
	LC_AUTOLEVELUP_CLOSE		=62,	// 001130 YGI
	LC_AUTOLEVELUP_OPEN			=63,	// 001130 YGI
	LC_ANNONCE_ON				=64,
	LC_ANNONCE_OFF				=65,
	LC_EAR_COMMAND_INFO			=66,
	LC_FIGHTMAP_ALL_CHAT		=68,
	LC_SMALL_TIPS				=69,
	LC_EMOTION					=70,	// ����
	LC_EXIT_GUILD				=71,	// ��� Ż��
	LC_VIEW_NK					=72,
	LC_SQUAD_LEADER				=73,	// ��ɰ� -> ���� �δ���
	LC_SQUAD					=74,	// ��ɰ� -> �δ�� 
	LC_SQUAD_MEMBER				=75,	// �δ��� -> �δ�� 
	LC_SQUAD_MEMBER_TO_MEMBER	=76,	// �δ�� -> �δ�� 
	LC_WAR_MENU_OPEN			=77,	// ��������� �޴� ���� 
	LC_WAR_MENU_NOTICE_OPEN		=78,	// ���� ���� // LTS TEMP
	LC_GUILD_MAIL				=79,	// ������
	LC_SQUAD_CHAT				=80,	
	LC_GUILD_CHAT				=81,	// ��� ü��
	LC_COMMANDER_INFO			=82,
	LC_SQUAD_INFO				=83,
	LC_GUILD_CHAT_ON			=84,	
	LC_GUILD_CHAT_OFF			=85,	
	LC_RESET_ABILITY			=86,
	LC_WAR_GIVE_LIFE			=87,	// 011015 LTS
	LC_PARTY_ALL_FREE			=88,	
	LC_SEND_GUILD_MAIL			=89,
	LC_SEND_MAIL				=90,
	LC_SHOW_ME_DUAL				=91,
	LC_REPORTER_MODE			=92,
	LC_FRIEND_MENU				=93,
	LC_WAR_GIVE_LIFE2			=94,	// 011213 LTS
	LC_SCAN_TARGET_YILSE		=95,	// 011217 LTS
	LC_GETCOLOSSUS_INFO			=96,
	LC_INVITE_COLOSSUS			=97,
	LC_JOIN_COLOSSUS			=98,
	LC_PK_ON_OFF				=99,
	LC_SET_NORMAL_CHAT_MODE		=109, // lsw 020620
	LC_COMBAT_SKILL_ON_OFF		=100,
	LC_LOCALWAR_CHAT			=111,		// LTS NEW LOCALWAR
	LC_QUESTMODE				=112,		// LTS DRAGON MODIFY
	LC_WEATHER_ON = 113, // CSD-CN-031215
	LC_WEATHER_OFF = 114, // CSD-CN-031215
	LC_COMBAT_RESET				=115, // CSD-020611
	LC_MAGIC_DISPLAY_ON			=116,// CSD-020620
	LC_MAGIC_DISPLAY_OFF		=117, // CSD-020620
	LC_REPRINT_NORMAL_CHAT		=118, // lsw 020620
	LC_REPRINT_GUILD_CHAT		=119,// lsw 020620
	LC_REPRINT_GUILD_BBS		=120,
	LC_TRADE					=121,	//020808 YGI 
	LC_CHECK_DUAL_FAME			=122,		// LTS DUAL FAME
	LC_WHAT_QUEST_IN_MAP		=123,		// 021022 kyo network7���� ��������
	LC_MERCHANT_BBS				=124,		// 021022 �����ŷ�
	LC_MERCHANT_EXCHANGE		=125,		// 021022 �����ŷ�
	LC_EVENT_MOVE_MAP			=126,		// 021107 YGI
	//<< 031021 kyo
	LC_EMOTI_LOVE				=127,
	LC_EMOTI_SMILE,	
	LC_EMOTI_HAPPY,
	LC_EMOTI_SHY,
	LC_EMOTI_CRY,
	LC_EMOTI_GOOD,
	LC_EMOTI_SORRY,
	LC_EMOTI_ANGER,
	LC_EMOTI_THANKS,
	LC_EMOTI_TELL,
	LC_GAME_SCISSOR				=137,
	LC_GAME_ROCK,
	LC_GAME_PAPER,
	LC_GAME_GO,
	LC_GAME_HELP,
	LC_GAME_PARTY,
	LC_GAME_HEAL,
	LC_GAME_TRANSACTION,
	LC_GAME_BEG,
	LC_GAME_CONGRAGULATION		=146,

	LC_BBS_LIMITED_USER_ADD		=147,
	LC_BBS_ON					=148,
	LC_BBS_OFF					=149,
	//>> 031021 kyo
	LC_RAID_START				=150, // CSD-040310
	LC_RAID_END					=151,	 // CSD-040310
	LC_GET_SMP					=152,
	LC_REPRINT_PARTY_CHAT		=153,	// Finito for /party note
	LC_DENY_DUEL_REQ			=154,	// Finito 25/08/07 command to deny dual requests
	LC_ALLOW_DUEL_REQ			=155,	// Finito 25/08/07 command to allow dual requests
	LC_TOGGLE_DROP				=156,	// Eleval 15/05/09 command to allow / block drop (/drop function)
	LC_TAME_AUTO_ATTACK			=157,	// Eleval 22/05/09 command to enable / disable auto-attack for tamed monsters
	LC_TAME_AUTO_DEFENCE		=158,	//Eleval 23/05/09 command to enable / disable auto-defence for tamed monsters
	LC_TAME_ALL_FOLLOW			=159,	// Eleval 24/05/09 command to tell all tamed creatures to follow their master (/tame follow)
	LC_TAME_ALL_STOP			=160,	// Eleval 24/05/09 command to tell all tamed creatures to stop what they are doing (/tame stop)
	LC_REPORTER_TELEPORT		=161,	//Eleval 15/08/09 - Command for RCs to Teleport themselves anywhere
	LC_HIDE						=162,	//Eleval 18/08/09 - To hide some stuff like hats, etc...
	LC_HONOR					=163,	//Eleval 06/09/09 - To Show the Player's current honor
	LC_EXP_RESET				=164, // CSD-020611
	LC_NATION_CHAT				=165, // CSD-020611
	LC_WORLDBBS_CHAT			= 166, // CSD-020611
	LC_USER_IMPRISON = 167,	// LTH-040812-KO 1.04p ������ �����Ѵ� "/���� ĳ����"
	LC_USER_RELEASE = 168,	// LTH-040812-KO 1.04p �������� �����Ų�� "/��� ĳ����"
	LC_USER_BAN = 169,
};

#define TAME_FAIL_CHANGE_NAME		0 // �̸��ٲٱ⸦  ����. 
#define TAME_NO_CHAR				1 // �׷��̸����� ����� ������ �����ϴ�.  
#define TAME_NO_NAME_CHAR			2 // �׷� �̸��� ���� ����� �����ϴ�. 
#define TAME_FOLLOW					3 // ���󰩴ϴ�. 
#define TAME_STOP					4 // ���� ���ϴ�. 
#define TAME_ATTACK					5 // �����մϴ�. 
#define TAME_SUCCESS_CHANGE_NAME	6 // �̸��� �ٲپ����ϴ�. 
// ���ڿ��� ����Ʈ
class CTextCmd
{
public :
	char *m_txt;
	CTextCmd *next;

public :
	CTextCmd() { m_txt = NULL; next = NULL; }
	~CTextCmd() 
	{ 
		SAFE_DELETE(next);
		SAFE_DELETE(m_txt);
	}
	void AddText( char *txt );
	char *GetLastText();
	char *GetFirstText();
};

class CEmotionCmd		// ����ǥ�� ��ɾ��
{
public : 
	char *m_Cmd;
	CEmotionCmd *next;

public :
	CEmotionCmd() { m_Cmd = NULL; next = NULL; }
	~CEmotionCmd()
	{
		SAFE_DELETE(next);
		SAFE_DELETE(m_Cmd);
	}
	void AddCmd( char *cmd );
	int GetCmd( char *txt );
};

/////////////////////////////////////////////////////////////////////////////////////
extern char	Lc_ChatType;
extern char	Lc_TempChatName[ 31];
extern char	Lc_DefaultMoveType;
extern char GreetingStr[ FILENAME_MAX];
extern int IsLineCommand( char *message, int nsize );
extern void SendTotalMapConnections( void );
extern void SendTotalConnections( void );
extern void RecvTotalMapConnections( int no);
extern void RecvTotalConnections( int no);
extern void RecvTameCommandResult( int result );
extern void RecvTameNameChange( t_tame_name_change *p );
extern void RecvHostName( t_tame_host_name *p );
extern void RecvHostEnd(  tame_end *p );

extern void SendBBS( char *bbs);		// �������� ����...
extern void SendBBSAll( char *bbs);

extern void RecvBbs_quest( char *bbs );					// 0810 lhs ����Ʈ ����

extern void SendChatDataGlobalFightMap( char *lpStr );
extern void CallViewSmallTips( char *txt );
extern void SendLcEmotion( char *txt );

extern int LoadEmotionCmd( CEmotionCmd *lpEmotionCmd );
extern void SendExitGuild( );
extern void ViewNk( char *name );
extern bool SendChatGuild( char *msg );
extern void RecvGuildChatData( t_packet *p );
extern void CheckShowMeDual();
extern void SendJoinColossusMap();
extern void TradeONOFF( char *t );//020808 YGI 
extern int LoadSmallTipsPage();
#endif //AFX_LINECOMMAND_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_