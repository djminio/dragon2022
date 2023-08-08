#ifndef _NATION_SYS_H_
#define _NATION_SYS_H_

#define	NW_NATION_COUNT					3


// 010730 lsw
// Nation War Main Interface Button No Definition
//_______________________________
#define	BUTTON_NATION_INFO				100		// Nation Explanation
#define	BUTTON_NATION_STATUS			101		// Nation Warfield Status
#define	BUTTON_NATION_SQUAD				102		// Nation Warfield Squad Organization
#define	BUTTON_NATION_JOIN				103		// Nation War Participation
#define	BUTTON_HALLOFFAME				104		// Hall of Fame	
#define BUTTON_HALLOFWAR				105		// Hall of War
#define	BUTTON_JOIN_FREELANCER			106
#define BUTTON_WAR_DESCRIPTION			107
#define BUTTON_NATION_NOTICE			108

//010901 lsw
#define BUTTON_NATION_LOADING_POINT		109


#define BUTTON_NATION_CHECK_COMMANDER	110		// Commander Election 
#define BUTTON_SQUAD_JOIN				111		// Squad Join
#define BUTTON_SQUAD_LODINGPOINT		112		// Set Loading Point

#define BUTTON_COMMANDER_REG			120		// Commander Regist
#define BUTTON_COMMANDER_VOTE			121		// Commander Elect

#define BUTTON_NATIONWAR_JOIN_VOTE		130		// NATION JOIN Interface

#define BUTTON_WARFIELD_STATUS_REFRESH	140		// NATION Warfield Status Refresh 

#define BUTTON_FREELANCER_SQUAD			150		// Nation Freelancer Squad 
#define	BUTTON_BY_JOIN					151		// Join BY FreeLancer
#define BUTTON_ZY_JOIN					152		// Join ZY FreeLancer
//_______________________________	

// War BBS BBS Type Define
//_______________________________	
#define		BBS_GENERAL						0
#define		BBS_WAR_BEGIN					1
#define		BBS_GUARDSTONE_BROKED			2
#define		BBS_CASTLEGATEWAY_BROKED		3
#define		BBS_WAR_VICTORY					4
#define		BBS_WAR_DEFEAT					5
#define		BBS_COMMANDER_ELECT				6
#define		BBS_WAR_JOIN_VOTE				7
#define		BBS_PEACE						8
#define		BBS_WAR_PREPARE					9
#define		BBS_COMMANDER_DEATH				10
#define		BBS_COMMANDER_CHAT				11
#define		BBS_BONUS_OPEN					12
#define		BBS_KILLER						13
#define		BBS_SQUADLEADER_ELECT			14 
#define		BBS_NATION_MONEY_USE			15
#define		BBS_NATION_MONEY_NOT_USE		16
//_______________________________	

#define MAX_MONSTER_POINT					6			// 011025 LTS

//________________________________ // 010906 LTS
#define	MAP0_133							32		
#define	MAP0_138							24
#define	MAP0_203							24
#define	MAP0_198							34
#define	MAP0_263							32
#define	MAP0_268							23

#define	MAP1_450							32		
#define	MAP1_527							32
#define	MAP1_524							32

#define	MAP2_114							28//32	// 011015 LTS	
#define	MAP2_119							32//26  // 011015 LTS
#define	MAP2_151							33//28  // 011015 LTS
#define	MAP2_156							26//23  // 011015 LTS 
#define	MAP2_274							36//30  // 011015 LTS
#define	MAP2_279							20//19  // 011015 LTS

//________________________________ // 010906 LTS

#define	NW_BY							3
#define NW_ZY							4
#define NW_YL							6

#define	WARFIELD_PORT1					5991
#define	WARFIELD_PORT2					5992
#define	WARFIELD_PORT3					5993

#define BASE_WARFIELD_PORT				5991
#define BASE_NEW_WARFIELD_PORT			5980
#define MAX_NEW_WARFIELD				1

#define	WARFIELD_COUNT					3
#define NEW_WARFIELD_COUNT				1

/**
 * @def		enum _eWARFIELD_PORT.
 * @brief	Warfield MapServer Port 식별자.
 */
//< LTH-040120-KO.
typedef enum _eWARFIELD_PORT
{
	WP_BASE_PORT			= BASE_WARFIELD_PORT,	/**< 기본이 되는 전쟁터 포트. */
	WP_MOUNTAIN_WARFIELD	= 5991,					/**< 산중 전쟁터. */
	WP_PLAIN_WARFIELD		= 5992,					/**< 평원 전쟁터. */
	WP_ISLAND_WARFIELD		= 5993,					/**< 섬 전쟁터. */
	WP_SNOWY_WARFIELD		= 5980,					/**< 설원 전쟁터. */
	WP_HADES_WARFIELD		= 5994,					/**< 지하 전쟁터. */
	WP_MAX_WARFIELD_PORT	= 5995,					/**< 최대 전쟁터 포트 번호. */
} eWARFIELD_PORT;									/**< 전쟁터 포트 대표 enum group. */
//> LTH-040120-KO

/**
 * @def		enum _eNEO_WARFIELD_ID.
 * @brief	Identity Each Warfield Map and Equal MapServer Port.
 */
//< LTH-040126-KO.
typedef enum _eNEO_WARFIELD_ID
{
	WI_MOUNTAIN_WARFIELD	= 0,	/**< 산중 전쟁터	(5991 - BASE_WARFIELD_PORT = 0). */
	WI_PLAIN_WARFIELD		= 1,	/**< 평원 전쟁터	(5992 - BASE_WARFIELD_PORT = 1). */
	WI_ISLAND_WARFIELD		= 2,	/**< 섬 전쟁터		(5993 - BASE_WARFIELD_PORT = 2). */
	WI_SNOWY_WARFIELD		= 3,	/**< 설원 전쟁터	(WI_ISLAND_WARFIELD + 1 = 3). */
	WI_HADES_WARFIELD		= 4,	/**< 지하 전쟁터	(5994 - BASE_WARFIELD_PORT + 1= 4). */
	WI_MAX_WARFIELD			= 5,	/**< 최대 전쟁터 개수. */
} eNEO_WARFIELD_ID;					/**< 전쟁터 번호 대표 enum group. */
//> LTH-040126-KO

#define	NW_PEACE						1
#define	NW_VOTE							2
#define	NW_PREPARE						3
#define	NW_WAR							4
#define	NW_WAIT_VOTE					7
#define	NW_WAIT_PREPARE					8
#define	NW_WAIT_WAR						9
#define	NW_WAIT_PEACE					10
#define	NW_SOPEN						11				// LTH-040226-KO 비밀지역 오픈
#define NW_MAX_STATE					NW_SOPEN + 1	// = 12 LTH-040203-KO

// War Condition

///////////////////////////////////////////////////////////////////////////lms 011226
#if defined (CHINA_LOCALIZING_) || defined (TAIWAN_LOCALIZING_) || defined (HONGKONG_LOCALIZING_)		// 020115 LTS

	#define		NW_NATION_VOTE_MIN				50				// 011028 LTS
	#define		NW_SQUADLEADER_LEVEL_MIN		60
	#define		NW_SQUADLEADER_LADDER_MIN		0
	#define		NW_COMMANDER_LEVEL_MIN			70
	#define		NW_COMMANDER_LADDER_MIN			2000
	#define		NW_COMMANDER_FAME_MIN			1200

	// Squad Condition
	#define		NW_SQUAD1_LEVEL_MIN				60
	#define		NW_SQUAD1_LEVEL_MAX				255
	#define		NW_SQUAD2_LEVEL_MIN				60
	#define		NW_SQUAD2_LEVEL_MAX				255			// 011028 LTS
	#define		NW_SQUAD3_LEVEL_MIN				60			// 011028 LTS
	#define		NW_SQUAD3_LEVEL_MAX				255			// 011028 LTS
	#define		NW_SQUAD4_LEVEL_MIN				60			// 011028 LTS
	#define		NW_SQUAD4_LEVEL_MAX				255			// 011213 LTS

	#define		NW_SQUAD1_MAN_MIN				5
	#define		NW_SQUAD1_MAN_MAX				70			// 011028 LTS
	#define		NW_SQUAD2_MAN_MIN				5
	#define		NW_SQUAD2_MAN_MAX				70			// 011028 LTS
	#define		NW_SQUAD3_MAN_MIN				5
	#define		NW_SQUAD3_MAN_MAX				70			// 011028 LTS
	#define		NW_SQUAD4_MAN_MIN				5
	#define		NW_SQUAD4_MAN_MAX				70			// 011028 LTS
//< LTH-040426-KO 예전에 맵서버에 고친 내용과 일치 시키려고 추가
#elif defined (THAI_LOCALIZING_)

	#define		NW_NATION_VOTE_MIN				50
	#define		NW_SQUADLEADER_LEVEL_MIN		20;
	#define		NW_SQUADLEADER_LADDER_MIN		0;
	#define		NW_COMMANDER_LEVEL_MIN			70;
	#define		NW_COMMANDER_LADDER_MIN			2000;
	#define		NW_COMMANDER_FAME_MIN			1200;

	#define		NW_SQUAD1_LEVEL_MIN				20;
	#define		NW_SQUAD1_LEVEL_MAX				50;
	#define		NW_SQUAD2_LEVEL_MIN				40;
	#define		NW_SQUAD2_LEVEL_MAX				70;		
	#define		NW_SQUAD3_LEVEL_MIN				60;		
	#define		NW_SQUAD3_LEVEL_MAX				90;		
	#define		NW_SQUAD4_LEVEL_MIN				80;		
	#define		NW_SQUAD4_LEVEL_MAX				255;	

	#define		NW_SQUAD1_MAN_MIN				5;
	#define		NW_SQUAD1_MAN_MAX				30;		
	#define		NW_SQUAD2_MAN_MIN				5;
	#define		NW_SQUAD2_MAN_MAX				60;		
	#define		NW_SQUAD3_MAN_MIN				5;
	#define		NW_SQUAD3_MAN_MAX				80;		
	#define		NW_SQUAD4_MAN_MIN				5;
	#define		NW_SQUAD4_MAN_MAX				90;	
//> LTH-040426-KO
#else

	#define		NW_NATION_VOTE_MIN				10//50				// 011028 LTS
	#define		NW_SQUADLEADER_LEVEL_MIN		20
	#define		NW_SQUADLEADER_LADDER_MIN		0
	#define		NW_COMMANDER_LEVEL_MIN			70
	#define		NW_COMMANDER_LADDER_MIN			2000
	#define		NW_COMMANDER_FAME_MIN			1200

	// Squad Condition

	//< LTH-040421-KO 밸런스를 위해 부대 레벨제한을 수정
	#define		NW_SQUAD1_LEVEL_MIN				20
	#define		NW_SQUAD1_LEVEL_MAX				60
	#define		NW_SQUAD2_LEVEL_MIN				50
	#define		NW_SQUAD2_LEVEL_MAX				90			// 011028 LTS
	#define		NW_SQUAD3_LEVEL_MIN				80			// 011028 LTS
	#define		NW_SQUAD3_LEVEL_MAX				120			// 011028 LTS
	#define		NW_SQUAD4_LEVEL_MIN				110			// 011028 LTS
	#define		NW_SQUAD4_LEVEL_MAX				150			// 011213 LTS
	//> LTH-040421-KO	

	#define		NW_SQUAD1_MAN_MIN				5
	#define		NW_SQUAD1_MAN_MAX				80			// 011028 LTS
	#define		NW_SQUAD2_MAN_MIN				5
	#define		NW_SQUAD2_MAN_MAX				70			// 011028 LTS
	#define		NW_SQUAD3_MAN_MIN				5
	#define		NW_SQUAD3_MAN_MAX				60			// 011028 LTS
	#define		NW_SQUAD4_MAN_MIN				5
	#define		NW_SQUAD4_MAN_MAX				50			// 011028 LTS

#endif

// Castle GateWay 
#define		NW_WARFIRLD1					6
#define		NW_WARFIRLD2					3
#define		NW_WARFIRLD3					6

#define		NW_SQUAD_KIND					3

#define		NW_ATTACKER						0
#define		NW_DEFENCER						1
#define		NW_REINFORCE					2		


// 011017 LTS <
#define		NW_NOTICE_JOIN_ATTACK			1
#define		NW_NOTICE_JOIN_DEFENCE			2
#define		NW_NOTICE_JOIN_REINFORCE		3

#define		NW_NOTICE_PREPARE1				4			// In Case, Not Unified Warfield
#define		NW_NOTICE_PREPARE1_REINFORCE	5

#define		NW_NOTICE_PREPARE2				6
#define		NW_NOTICE_PREPARE2_REINFORCE	7

#define		NW_NOTICE_PREPARE3				8			// In Case, Unified Warfield
#define		NW_NOTICE_PREPARE3_REINFORCE	9

#define		NW_NOTICE_PREPARE4				10
#define		NW_NOTICE_PREPARE4_REINFORCE	11

#define		NW_NOTICE_WAR1A					12			// In Case, Not Unified Warfield
#define		NW_NOTICE_WAR1_REINFORCE		13

#define		NW_NOTICE_WAR2A					14			// In Case, Unified Warfield
#define		NW_NOTICE_WAR2_REINFORCE		15

#define		NW_NOTICE_PEACE1_VICTORY		16
#define		NW_NOTICE_PEACE1_DEFEAT			17
#define		NW_NOTICE_PEACE1_REINFORCE		18

#define		NW_NOTICE_PEACE2				19
#define		NW_NOTICE_PEACE2_REINFORCE		20

#define		NW_NOTICE_WAR1D					21			// In Case, Not Unified Warfield
#define		NW_NOTICE_WAR2D					22			// In Case, Unified Warfield

#define		NW_NOTICE_SOPEN					23
#define		NW_NOTICE_SOPEN_ERROR			24
// 011017 LTS >


//#define		MAX_NEW_WARFIELD				1		// LTH-040226-KO 위에 있기때문에 지웠다.



typedef struct MonPoint
{
	int		MonsterIndex;
	int		Size;
	POINTS* Points;
}	t_MonPoint;


class cNation
{
	BYTE m_Status[WARFIELD_COUNT];
	BYTE m_Possession[WARFIELD_COUNT];				// 소유
	t_MonPoint	m_tMonsterPoint[NW_NATION_COUNT][MAX_MONSTER_POINT];
	BYTE m_NewStatus[MAX_NEW_WARFIELD];
	BYTE m_btNeoStatus[WI_MAX_WARFIELD];		// LTH-040226-KO 1.4 패치 후 새롭게 국가전 상태를 관리하는 곳.

public :
	cNation();
	~cNation();

	void SetStatus(int WarfieldNo, int Status);
	int  GetStatus(int WarfieldNo);
	void SetNewStatus(int WarfieldNo,int Status);
	int  GetNewStatus(int WarfieldNo);
	void SetWarfieldPoss(int WarfieldNo,int Nation);
	int  GetWarfieldPoss(int WarfieldNo);
	bool LoadMonsterPoint();
	void SaveMonsterPoint();
	void SetTileDont(int WarfieldNo,int MonNo,int type); //몬스터 SQL 필드 INDEX, 0,1 : 
	void TileDont(int WarfieldNo,int TypeNum,int Value);
};

typedef struct DONTDATA
{
	int		FieldSize;
	POINT*	lpPoint;
}*LPDONTDATA;

class CDontManager
{
	int			m_iSize;
	LPDONTDATA	m_pDontData;

public :

	CDontManager();
	~CDontManager();

	void Clear();
	void LoadData();
	void ActiveDont(int DontNo,int Type);
};

extern CDontManager*	g_pDontManager;
#endif