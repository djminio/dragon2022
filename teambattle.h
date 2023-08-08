#ifndef __teambattle_h__
#define __teambattle_h__

class CBattleTeam 
{
public : 
	int count;
	k_team_member_info_unit member[MAX_TEAM_MEMBER];

public :
	CBattleTeam();
	void ReSetClass();
	int AddMember( k_team_member_info_unit *info );
	int DeleteMember( char *name );
};

class CBattleZone
{
public : 
	CBattleTeam		m_team[ATI_MAX_TEAM];			// 최대 4팀

public : 
	CBattleZone();
	void ReSetClass();
	int SetAllInfo( k_team_member_info_all *info );
	int AddMember( int team, k_team_member_info_unit *info );
	int GetCount( int team );
	int IsExist( char *name );
};


//////////////////////////////////////////////////////////////

#define MAX_FIGHT_MAP_CH	20

struct WINLOSEPOINT
{
	DWORD win			: 13;
	DWORD defeat		: 12;
	DWORD disconnect	: 7 ;
};

class CFightMapCharInfo
{
public :
	DWORD				m_TotalId;
	WINLOSEPOINT		m_WinDefeat;
	DWORD				m_dwLadderScore;
	int					type;
	char				m_szCallLevel[20];
	WORD				m_wColor;


	CFightMapCharInfo	*next;

public :
	CFightMapCharInfo() { m_TotalId=0, next = 0; }
	~CFightMapCharInfo() { if( next ) delete next; }

	// 여기서 type은 서버 전송을 받기 전 받은 후를 나타낸다.
	void AddCharInfo( DWORD total_id, DWORD win_defeat, DWORD ladder, int level, int Class, int type, int dual_cls, int cls_step );
	CFightMapCharInfo *GetFightMapCharInfo( DWORD total_id );
	void DeleteAll() { m_TotalId = 0; delete next; next = 0; }


	
};

////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////
// 길드 설명 메뉴 호출시 사용하는 define
#define GE_REGIST_IMSI_GUILD		1		// 임시 길드 등록 설명
#define GE_REGIST_GUILD_MARK		2		// 길드 마크 등록에 관한 설명
#define GE_REGIST_GUILD_MENUAL		3		// 길드 등록에 관한 전반적인 설명 메뉴
#define GE_REGIST_GUILD				4		// 정식 글드 등록
#define GM_INPUT_SUB_MASTER			5		// 부길마 입력란
#define GM_CHANGE_INFO_MENUAL		6		// 길드 정보 변경 설명




class CGuildBasic
{
public :
	int  m_bInfo;			// 서버로 그 길드의 정보를 요청한 적이 있다 없다의 플레그
	int	 m_bActive;			// 없는 길드이다 아니다.
	int	 m_nCode;				// 길드 코드
	char m_szName[20];			// 길드 명
	char m_szGuldMasterName[20];			// 길마 이름
	char m_szSubGuldMasterName[20];			// 서브 길마 이름
	char m_aDegree[5][20];		// 직책	1: 길마 ~ 0 : 일반

public :
	CGuildBasic()
	{
		InitData();
	}
	char *CallGuildDegreeName( int nDegree )
	{
		if( nDegree <0 || nDegree >= 5 ) return NULL;
		return m_aDegree[nDegree];
	}
	int InitData();
};

class CGuild : public CGuildBasic		// text로 필요한 것만 로딩해올때.. 만약 bin에서 로딩 할 경우엔 다르게.....
{
public :
	int m_nLevel;				// 길드 등급

	int	first;
	int mark_num;
	int make;
	int info_num;

	int m_nAllianceGuildCode;	// 동맹 길드 코드	
	int m_nHostilityGuildCode;	// 적대 길드 코드

	int m_nRader;				// 레더 점수
	int m_nWin;				// 길드전 이긴 수
	int m_nLoss;				// 길드전에서 진 수 

	bool save_flag;
	Spr m_Spr;					// 길드 마크
	Spr m_Spr_save;					// 길드 마크

	char *m_szExplain;			// 길드 설명

public : 
	CGuild(){ InitData(); }		// 초기화
	~CGuild(){ if( m_szExplain ) delete m_szExplain; }
	int InitData();
};

// thai2 YGI
#include "kein_stl.h" // CSD-030324
extern TKeinGuildMap< CGuild > g_GuildMgr; // CSD-030324

// 길드 클레스와 관련된 함수들
extern char *CallGuildDegreeName( int guild_num, int degree_num );		// 길드 직책을 리턴...
extern char *GetGuildName( int guild_num );
extern int GetGuildCode( LPCHARACTER ch );

///////////////////////////////////////////////////////////////////////////////////////////////////
// 길드 소개 할때 사용할 국가별 길드 정보

class CGuildIntroductionGuild
{
public : 
	int x;
	BYTE line;
	BYTE page;
	BYTE size;

	RECT rect;
	int	 m_nCode;				// 길드 코드
	char m_szName[20];			// 길드 명
};

class CGuildIntroduction
{	//< CSD-030324
public :
	int m_Nation;
	int m_Count;
	TKeinGuildMap < CGuildIntroductionGuild > m_Guild;
public :
	CGuildIntroduction()
	{
		m_Count = 0;
	}
};	//> CSD-030324

//< soto-030312
#define		MAX_GUARDIAN_GUILD	3

#define		GUARDIAN_GUILD_1ST_TEST	379		//바이서스(최종병기 그녀)		길마 : DR성직자 030318현재
#define		GUARDIAN_GUILD_2ST_TEST	197		//일스(농심새우깡)				길마 : 전설의 마궁수 030318현재
#define		GUARDIAN_GUILD_3ST_TEST	241		//자이펀(Glory of Streak)		길마 : 이슈리나

#define		GUARDIAN_GUILD_1ST	27		//바이서스 : 해븐					길마 : 밤의대통령3   	GuildCode : 27
#define		GUARDIAN_GUILD_2ST	343		//자이펀 : 자이펀연합				길마 : 부건전사  	GuildCode : 343
#define		GUARDIAN_GUILD_3ST	9		//일스 : 져스티스  기사단			길마 : 디지탈		GuildCode : 9


// 지키미 길드. // 복장이 바뀌는 길드는 1위길드만 바뀐다.
class CGuardianGuild
{
private:
	int m_nGuildCount; // 현재 지키미 길드의 수를 나타낸다.
	int	m_n1stGuildCode; // 1위인 길드 코드를 알아낸다.
	vector<int>m_vtGuildList;
	
	// 안의코드를 검사하지 않고 길드 코드를 등록한다.(중복의 위험이 있다)
	int	ForcingAddGuild(int nGuildCode)
	{		
		m_vtGuildList.push_back(nGuildCode);
		return ++m_nGuildCount;
	}
public:
	CGuardianGuild() //생성자
	{ 
		m_vtGuildList.reserve(MAX_GUARDIAN_GUILD);
		m_nGuildCount = 0;
		m_n1stGuildCode = -1; 
	} // 생성자.
	
	bool IsGuardianGuild(int nGuildCode)// 이 길드코드가 지키미 길드의 코드이냐?
	{
		for(int i = 0;i < m_nGuildCount;i++)
		{
			if(m_vtGuildList[i] == nGuildCode) return true;
		}
		return false;
	}	
	
	int AddGuild(int nGuildCode)//중복되지 않게 추가 시킨다.
	{
		if(!IsGuardianGuild(nGuildCode))
		{
			m_nGuildCount = ForcingAddGuild(nGuildCode);
		}

		return m_nGuildCount;
	}
	
	int FirstGuild()//1등 길드코드를 알아낸다.// 없을때는 -1을 리턴.
	{
		if(!m_nGuildCount)return -1;

		return m_n1stGuildCode;
	}

	int GetCount()
	{
		return m_vtGuildList.size();
	}

	int	GetGuildCode(int nIndex)
	{
		if(nIndex < m_nGuildCount)
			return m_vtGuildList[nIndex];
		return -1;
	}

	// 일등인 길드는 하나가 아니라.. 각국가별로 하나로 바뀌었다. 당근 3개지.
	bool	Is1StGuild(int nGuildCode){ return IsGuardianGuild(nGuildCode); }// soto-030318

	void Set1stGuild(int nGuildCode)//1등인 길드를 등록한다.
	{
		if(!m_nGuildCount)//현재 길드가 아무도 등록 되어 있지 않을때.
		{
			ForcingAddGuild(nGuildCode);
			m_n1stGuildCode = nGuildCode;
		}
		else // 하나 이상의 길드가 등록되어 있을때.
		{
			if(IsGuardianGuild(nGuildCode)) // 리스트에 등록된 코드인가?
			{
				m_n1stGuildCode = nGuildCode;
			}
			else
			{
				ForcingAddGuild(nGuildCode);
				m_n1stGuildCode = nGuildCode;
			}
		}
	}

	void ClearGuild()
	{ 
		m_vtGuildList.clear();
		m_n1stGuildCode = -1;
		m_nGuildCount = 0;
	}
};


extern bool	CheckGuardianGuildWear();// 시야의 사람들의 옷을체크 하여 길드복으로 갈아 입힌다.


#define		GUILD_WEAR01		102//지키미 길드복

extern CGuardianGuild	g_CGuardianGuild; // 지키미 길드 관리 전역 클래스. // soto
extern CBattleZone	g_BattleZone;
extern void RecvAllTeamInfo( t_packet *p );
extern void SendAddMeOfTeamBattle( int team );
extern void RecvAddMemberInfo( t_packet *p );

extern bool	g_FightMapStart;
extern void StartTembattleWaitMenu(int nGame); // CSD-030506
extern void ViewFightMapCharInfo( char *name, DWORD total_id, int x, int y );
extern void ChangeEquipForFightMapStart();
extern void RecvFightRemainTime( t_packet *p );
extern void FighMapReset();
extern void ResetMagicFlag( LPCHARACTER ch );
extern int g_save_load_flag;
extern void LoadGuildMarkEx();
extern CGuildIntroduction *g_Guild_Introduction;


// 그 스톤을 어느팀이 찾이 했는가?
#define STONE_SCORE_FIRST	IA2_NOTMOVE		//  1: 초기 일때 
#define STONE_SCORE_2	IA2_OPENED			//	2: 자리
#define STONE_SCORE_1	IA2_BROKEN			// 첫째자리


#define GetAttr2( attr2, a )		((attr2) & (a))
#define SetAttr2( attr2, b )		{(attr2) |=(b);}
extern int GetTeamByStone( DWORD attr );
extern WORD GetRGBByTeam( int team, int &r, int &g, int &b );

extern int GetArenaObserverTarget();
extern int GetArenaBettingTarget();
extern void SetArenaObserverTarget(const int iTeamType);
extern void SetArenaBettingTarget(const int iTeamType);
extern void SendObserverTeamSelect();
extern void SendMyArenaBettingMoney(const DWORD dwMoney);

extern DWORD GetTeamBettingMoney(const int iTeamType);//팀에 걸린 돈
extern float GetTeamWinRate(const int iTeamType);//팀에 걸린 돈
extern DWORD GetMyBettingMoney();//배팅금
extern DWORD GetMyBettingResult();//배당금
extern DWORD GetAreanWaitTime();
extern bool CanCallBettingMenu();
extern bool CanCallObserverMenu();
#endif