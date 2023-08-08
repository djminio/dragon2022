///////////////////////////////////////////////////////////////////////////////
// 듀얼 클래스에 관한 Packet 정의 7501 ~ 7599
#ifndef __NETWORK5__
#define __NETWORK5__
///////////////////////////////////////////////////////////////////////////////
//
#define CMD_NPC_ATTACK		       86 
#define CMD_ATTACK				       6200
#define CMD_ATTACK_RESULT  	     6110
#define CMD_ATTACK_RESULT_D	     6111

#define CMD_NPC_THROW  		       8903
#define CMD_THROW_ATTACK         6112
#define CMD_THROW_RESULT         6113
#define CMD_THROW_RESULT_D       6114

#define CMD_EFFECT_RESULT 7615 // CSD-031007

#define	CMD_NPC_MAGIC      20008
#define CMD_MAGIC_SELECT   7590 // CSD-TW-030606
#define CMD_MAGIC_CASTING  7591 // CSD-TW-030606
#define CMD_MAGIC_EXECUTE  7592 // CSD-TW-030606
#define CMD_MAGIC_RESULT   7593 // CSD-TW-030606
#define CMD_MAGIC_RESULT_T 7594 // CSD-TW-030606
#define CMD_MAGIC_RESULT_H 7595 // CSD-TW-030606
#define CMD_MAGIC_RESULT_M 7596 // CSD-TW-030606
#define CMD_MAGIC_RESULT_D 7597 // CSD-TW-030606

#define CMD_COMBAT_CLEAR         7608
#define CMD_COMBAT_OBTAIN        7609
#define CMD_COMBAT_REQUEST       7610
#define CMD_COMBAT_LEARN         7611
#define CMD_COMBAT_SELECT        7612
#define CMD_COMBAT_ATTACK        7613
#define CMD_COMBAT_RESULT        7614
#define CMD_COMBAT_RESULT_C      7615
#define CMD_COMBAT_RESULT_H      7616
#define CMD_COMBAT_RESULT_M      7617
#define CMD_COMBAT_RESULT_S      7618
#define CMD_COMBAT_RESULT_T      7619
#define CMD_COMBAT_RESET         7620
#define CMD_COMBAT_INIT          7621
#define CMD_COMBAT_SYNC			 7622 // 030517 kyo

#define CMD_BATTLE_DAMAGE        7606
#define CMD_BATTLE_MESSAMGE      7607

#define CMD_DUAL_ENABLE          7501
#define CMD_DUAL_CHANGE          7502
#define CMD_DUAL_DIVIDE          7503
#define CMD_RESET_ABILITY        7504

#define CMD_CHAR_INFO_TACTIC 5009 // CSD-TW-030624

struct t_char_info_tactic
{	//< CSD-TW-030624
	WORD wKind;
	DWORD dwExperience;
	BYTE nStep;
};	//> CSD-TW-030624

struct t_battle_message
{
    BYTE nType;   // 메세지의 분류
    BYTE nKind;   // 메세지의 종류
    WORD nNumber; // 메세지의 번호
};

struct t_battle_damage
{
	WORD    idTarget; // 대상자 
	BYTE    nKind;    // 데미지 종류
	__int32 nDamage;  // 데미지
};

union t_battle
{
	t_battle_message battle_message;
	t_battle_damage  battle_damage;
};

// 단중거리 무기로 공격시
struct t_npc_attack
{
	WORD idCaster;
	WORD idTarget;
	__int16 nX;
	__int16 nY;
};

struct t_player_attack
{	//< CSD-TW-030606
	WORD idTarget;
	bool bWanted;
};	//> CSD-TW-030606

struct t_attack_result
{	//< CSD-031001
	WORD idTarget;
	__int32 nDamage;
	__int16 nX;
	__int16 nY;
	WORD wdEffect;
	BYTE nCombat;
	BYTE nResult;
};	//> CSD-031001

struct t_attack_result_d
{	//< CSD-031001
	WORD idCaster;
	WORD idTarget;
	__int32 nDamage;
	__int16 nX;
	__int16 nY;
	WORD wdEffect;
	BYTE nCombat;
	BYTE nResult;
};	//> CSD-031001
// 장거리 무기로 공격시
struct t_npc_throw
{
	WORD idCaster;
	WORD idTarget;
	__int16 nX;
	__int16 nY;
};

struct t_client_throw_attack
{	//< CSD-TW-030606
	WORD idTarget;
	bool bWanted;
};	//> CSD-TW-030606

struct t_server_throw_attack
{	//< CSD-TW-030606
	WORD idCaster;
	WORD idTarget;
	__int16 nX;
	__int16 nY;
	BYTE nCombat;
};	//> CSD-TW-030606

struct t_client_throw_result
{ 
	WORD idTarget; // 마법을 사용한 대상자
	__int16 nX;    // 마법이 발생된 위치의 X좌표
	__int16 nY;    // 마법이 발생된 위치의 Y좌표
	bool bWanted;
};

struct t_server_throw_result
{	//< CSD-031001
	WORD idTarget;
	__int16 nX;
	__int16 nY;
	__int32 nDamage;
	WORD wdEffect;
	BYTE nCombat;
	BYTE nResult;
};	//> CSD-031001

struct t_throw_result_d
{	//< CSD-031001
	WORD idCaster;
	WORD idTarget;
	__int32 nDamage;
	__int16 nX;
	__int16 nY;
	WORD wdEffect;
	BYTE nCombat;
	BYTE nResult;
};	//> CSD-031001

struct t_client_effect_result
{	//< CSD-031007
	WORD wdEffect;
	WORD idTarget; // 전투스킬 대상자
	__int16 nX;    // 전투스킬이 발생된 위치의 X좌표
	__int16 nY;    // 전투스킬이 발생된 위치의 Y좌표
	bool bWanted;  // 대상자의 헌상범 여부
};	//> CSD-031007

struct t_server_effect_result
{	//< CSD-031007
	WORD idTarget;
	__int32 nDamage;
	BYTE nResult;
};	//> CSD-031007

union t_strike
{	//< CSD-031007
	t_player_attack        player_attack;
	t_attack_result        attack_result;
	t_client_throw_attack  client_throw_attack;
	t_server_throw_attack  server_throw_attack;
	t_client_throw_result  client_throw_result;
	t_server_throw_result  server_throw_result;
	t_npc_attack           npc_attack;
	t_attack_result_d      attack_result_d;
	t_npc_throw            npc_throw;
	t_throw_result_d       throw_result_d;
	t_client_effect_result client_effect_result;
	t_server_effect_result server_effect_result;
};	//> CSD-031007

// 마법 관련
struct t_client_npc_magic
{
	BYTE nMagic;   // 마법의 번호
	WORD idCaster; // 마법 시전자
	WORD idTarget; // 마법 대상자
	__int16 nX;    // 마법이 발생된 위치의 X좌표
	__int16 nY;    // 마법이 발생된 위치의 Y좌표
	bool bTimeDisable;	// LTS DRAGON MODIFY
};

struct t_server_npc_magic
{
	BYTE nMagic;   // 마법의 번호
	WORD idCaster; // 마법 시전자
	WORD idTarget; // 마법을 사용한 대상자
	__int16 nX;    // 마법이 발생된 위치의 X좌표
	__int16 nY;    // 마법이 발생된 위치의 Y좌표
	bool bFailed;
};

struct t_client_magic_select
{	//< CSD-TW-030606
	BYTE nMagic;   // 마법의 번호
};	//> CSD-TW-030606

struct t_server_magic_select
{	//< CSD-TW-030606
	BYTE nMagic;   // 마법의 번호
	WORD idCaster; // 마법 시전자
};	//> CSD-TW-030606

struct t_client_magic_casting
{	//< CSD-TW-030606
	BYTE nMagic;   // 마법의 번호
};	//> CSD-TW-030606

struct t_server_magic_casting
{	//< CSD-TW-030606
	BYTE nMagic;   // 마법의 번호
	WORD wReady;   // 마법 준비시간
	WORD idCaster; // 마법 시전자
};	//> CSD-TW-030606

struct t_client_magic_execute
{	//< CSD-TW-030606
	BYTE nMagic;   // 마법의 번호
	WORD idTarget; // 마법 대상자
	__int16 nX;    // 마법이 발생된 위치의 X좌표
	__int16 nY;    // 마법이 발생된 위치의 Y좌표
};	//> CSD-TW-030606

struct t_server_magic_execute 
{	//< CSD-TW-030606
	BYTE nMagic;   // 마법의 번호
	WORD idCaster; // 마법 시전자
	WORD idTarget; // 마법을 사용한 대상자
	__int16 nX;    // 마법이 발생된 위치의 X좌표
	__int16 nY;    // 마법이 발생된 위치의 Y좌표
	bool bFailed;
};	//> CSD-TW-030606

struct t_client_magic_result
{
	BYTE nMagic;   // 사용된 마법의 번호
	WORD idTarget; // 마법을 사용한 대상자
	__int16 nX;    // 마법이 발생된 위치의 X좌표
	__int16 nY;    // 마법이 발생된 위치의 Y좌표
	bool bWanted; 
};

struct t_server_magic_result
{	//< CSD-TW-030606
	BYTE nMagic; // 사용된 마법의 번호
	WORD idTarget; // 마법을 사용한 대상자
	BYTE nState; // 마법이 적용된 후 상태
	BYTE nResult; // 마법 사용 결과
};	//> CSD-TW-030606

struct t_server_magic_result_t : public t_server_magic_result
{	//< CSD-TW-030606
	WORD wDuration; // 유지시간
};	//> CSD-TW-030606

struct t_server_magic_result_h : public t_server_magic_result
{	//< CSD-TW-030606
	__int32 nLife; // 마법 사용으로 변화된 Life의 량
};	//> CSD-TW-030606

struct t_server_magic_result_m : public t_server_magic_result
{	//< CSD-TW-030606
	__int32 nLife; // 마법 사용으로 변화된 Life의 량
	__int32 nMana; // 마법 사용으로 변화된 Mana의 량
};	//> CSD-TW-030606

struct t_magic_result_d
{
	BYTE nMagic;   // 사용된 마법의 번호
	WORD idCaster; // 마법 시전한 NPC
	WORD idTarget; // 마법을 사용한 대상자
	__int16 nX;    // 마법이 발생된 위치의 X좌표
	__int16 nY;    // 마법이 발생된 위치의 Y좌표 
};

union t_magic
{
	t_client_magic_select  client_magic_select;    // CSD-TW-030606
	t_server_magic_select  server_magic_select;    // CSD-TW-030606
	t_client_magic_casting client_magic_casting;   // CSD-TW-030606
	t_server_magic_casting server_magic_casting;   // CSD-TW-030606
	t_client_magic_execute client_magic_execute;   // CSD-TW-030606
	t_server_magic_execute server_magic_execute;   // CSD-TW-030606
	t_client_magic_result  client_magic_result;
	t_server_magic_result  server_magic_result;    // CSD-TW-030606
	t_server_magic_result_t server_magic_result_t; // CSD-TW-030606
	t_server_magic_result_h server_magic_result_h; // CSD-TW-030606
	t_server_magic_result_m server_magic_result_m; // CSD-TW-030606
	t_client_npc_magic     client_npc_magic;
	t_server_npc_magic     server_npc_magic;
	t_magic_result_d       magic_result_d;
};

// 전투스킬 관련
struct t_combat_init
{	//< CSD-TW-030606
	WORD idMaster; // 전투스킬 습득자
	BYTE nType;    // 전투스킬 타입
};	//> CSD-TW-030606

struct t_combat_clear
{
	WORD idMaster; // 전투스킬 습득자
	BYTE nType;    // 전투스킬 타입(Active : 0, Passive : 1)
};

struct t_client_combat_obtain
{
	WORD idMaster; // 전투스킬 포인터 습득자
	BYTE nPara;    // INV 위치
	BYTE nPosX;    // INV의 X자리 
	BYTE nPosY;    // INV의 Y자리
	BYTE nPoint;   // 전투스킬 획득 포인터
};

struct t_server_combat_obtain
{
	WORD idMaster; // 전투스킬 포인터 습득자
	BYTE nPara;    // INV 위치
	BYTE nPosX;    // INV의 X자리
	BYTE nPosY;    // INV의 Y자리
	BYTE nTotal;   // 총 전투스킬 포인터
};

struct t_client_combat_request
{
	WORD idMaster; // 전투스킬 습득자
};

struct t_server_combat_request
{
	WORD idMaster;   // 전투스킬 습득자
	BYTE nRemain;    // 남은 전투스킬 포인터
	BYTE aLevel[36]; // 전투스킬 정보
};

struct t_client_combat_learn
{
	WORD idMaster; // 전투스킬 습득자
	BYTE nSkill;   // 전투스킬 번호
	BYTE nPoint;   // 전투스킬 포인터
};

struct t_server_combat_learn
{
	WORD idMaster; // 전투스킬 습득자
	BYTE nSkill;   // 전투스킬 번호
	BYTE nLevel;   // 전투스킬 레벨
	BYTE nRemain;  // 남은 전투스킬 포인터
};

struct t_client_combat_clear
{	//< CSD-TW-030606
	BYTE nType;
};	//> CSD-TW-030606

struct t_server_combat_clear
{	//< CSD-TW-030606
	WORD idMaster; // 전투스킬 습득자
	BYTE nType;
};	//> CSD-TW-030606

struct t_client_combat_select
{	//< CSD-TW-030606
	BYTE nCombat;  // 전투스킬 번호
};	//> CSD-TW-030606

struct t_server_combat_select
{	//< CSD-TW-030606
	BYTE nCombat;  // 전투스킬 번호
	WORD idMaster; // 전투스킬 시전자
};	//> CSD-TW-030606

struct t_client_combat_attack
{
	BYTE nSkill;   // 전투스킬 번호
	WORD idTarget; // 전투스킬 대상자
	__int16 nX;    // 전투스킬이 발생된 위치의 X좌표
	__int16 nY;    // 전투스킬이 발생된 위치의 Y좌표
};

struct t_server_combat_attack
{
	BYTE nSkill;   // 전투스킬 번호
	BYTE nLevel;   // 전투스킬 레벨
	WORD idCaster; // 전투스킬 시전자
	WORD idTarget; // 전투스킬 대상자
	WORD wAmount;  // 전투스킬 사용 가능량
	__int16 nX;    // 전투스킬이 발생된 위치의 X좌표
	__int16 nY;    // 전투스킬이 발생된 위치의 Y좌표
	bool bFailed;  // 전투스킬 실패여부
};

struct t_client_combat_result
{
	BYTE nSkill;   // 전투스킬 번호
	WORD idTarget; // 전투스킬 대상자
	__int16 nX;    // 전투스킬이 발생된 위치의 X좌표
	__int16 nY;    // 전투스킬이 발생된 위치의 Y좌표
	bool bWanted;  // 대상자의 헌상범 여부
};

struct t_server_combat_result
{
	BYTE nSkill;   // 전투스킬 번호
	BYTE nResult;  // 전투스킬 사용 결과
	WORD idTarget; // 전투스킬 대상자
};

struct t_server_combat_result_h : public t_server_combat_result
{
	BYTE nState;   // 전투스킬 적용 상태
	__int32 nLife; // 전투스킬 사용후 변화된 Life의 량
	bool bChangeAni; //모습변동이 있는가? 	// 030415 kyo
};

struct t_server_combat_result_m : public t_server_combat_result
{
	__int32 nMana; // 전투스킬 사용후 변화된 Mana의 량
};

struct t_server_combat_result_s : public t_server_combat_result
{
	__int32 nHungry; // 전투스킬 사용후 변화된 Mana의 량
};

struct t_server_combat_result_t : public t_server_combat_result
{
	BYTE nState;    // 전투스킬 적용 상태
	WORD wMaintain; // 전투스킬 속성 유지시간
};

union t_combat
{
	t_combat_init            combat_init;
	t_client_combat_obtain   client_combat_obtain;
	t_server_combat_obtain   server_combat_obtain;
	t_client_combat_request  client_combat_request;
	t_server_combat_request  server_combat_request;
	t_client_combat_learn    client_combat_learn;
	t_server_combat_learn    server_combat_learn;
	t_client_combat_clear    client_combat_clear; // CSD-TW-030606  
	t_server_combat_clear    server_combat_clear; // CSD-TW-030606
	t_client_combat_select   client_combat_select;
	t_server_combat_select   server_combat_select;
	t_client_combat_attack   client_combat_attack;
	t_server_combat_attack   server_combat_attack;
	t_client_combat_result   client_combat_result;
	t_server_combat_result   server_combat_result;  
	t_server_combat_result_h server_combat_result_h;
	t_server_combat_result_m server_combat_result_m;
	t_server_combat_result_s server_combat_result_s;
	t_server_combat_result_t server_combat_result_t;
};

// 듀얼 클래스 관련
struct t_client_dual_enable
{
	WORD idMaster; // 듀얼 클래스 요청자
	BYTE nPara;    // INV 위치
	BYTE nPosX;    // INV의 X자리 
	BYTE nPosY;    // INV의 Y자리
};

struct t_server_dual_enable
{
	WORD idMaster; // 듀얼 클래스 요청자
	BYTE nPara;    // INV 위치
	BYTE nPosX;    // INV의 X자리
	BYTE nPosY;    // INV의 Y자리
};

struct t_client_dual_change
{
	WORD idMaster;  // 듀얼 클래스가 되는 Player
	BYTE nPara;     // INV 위치
	BYTE nPosX;     // INV의 X자리
	BYTE nPosY;     // INV의 Y자리
	BYTE nNext;     // 변경할 듀얼 클래스
};

struct t_server_dual_change
{
	WORD  idMaster; // 듀얼 클래스가 되는 Player
	BYTE  nDual;    // 듀얼 클래스
	DWORD dwFame;   // 국가전 fame 점수
	WORD  wStr;     // Str
	WORD  wCon;     // Con
	WORD  wDex;     // Dex
	WORD  wWis;     // Wis
	WORD  wInt;     // Int
	WORD  wMovep;   // Movep
	WORD  wCha;     // Cha
	WORD  wEndu;    // Endu
	WORD  wMor;     // Mor
	WORD  wLuck;    // Luck
	WORD  wWsPs;    // Ws or Ps
	WORD  wPoint;   // 남아있는 Ability Point 개수
};

struct t_client_dual_divide
{
	WORD idMaster; // 듀얼 클래스가 되는 Player
	BYTE nNext;    // 분배할 듀얼 클래스
};

struct t_server_dual_divide
{
	WORD idMaster; // 듀얼 클래스가 되는 Player
	BYTE nDual;    // 듀얼 클래스
	WORD wStr;     // Str
	WORD wCon;     // Con
	WORD wDex;     // Dex
	WORD wWis;     // Wis
	WORD wInt;     // Int
	WORD wMovep;   // Movep
	WORD wCha;     // Cha
	WORD wEndu;    // Endu
	WORD wMor;     // Mor
	WORD wLuck;    // Luck
	WORD wWsPs;    // Ws or Ps
	WORD wPoint;   // 남아있는 Ability Point 개수
};

union t_dual
{
	t_client_dual_enable client_dual_enable;
	t_server_dual_enable server_dual_enable;
	t_client_dual_change client_dual_change;
	t_server_dual_change server_dual_change;
	t_client_dual_divide client_dual_divide;
	t_server_dual_divide server_dual_divide;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//< CSD-030522
#define CMD_ARENA_ENTER_LOBBY 10701
#define CMD_ARENA_LEAVE_LOBBY 10702

#define CMD_ARENA_ENTER_TEAM 10703
#define CMD_ARENA_LEAVE_TEAM 10704

#define CMD_ARENA_ENTER_OBSERVE 10705
#define CMD_ARENA_LEAVE_OBSERVE 10706

#define CMD_ARENA_ENTER_GAMBLING 10707
#define CMD_ARENA_LEAVE_GAMBLING 10708

#define CMD_ARENA_ADD_LOBBY 10709
#define CMD_ARENA_DELETE_LOBBY 10710

#define CMD_ARENA_ADD_TEAM 10711
#define CMD_ARENA_DELETE_TEAM 10712

#define CMD_ARENA_ADD_GAMBLING 10713
#define CMD_ARENA_DELETE_GAMBLING 10714

#define CMD_ARENA_ADD_OBSERVE 10715
#define CMD_ARENA_DELETE_OBSERVE 10716

#define CMD_ARENA_LEADER_INFO 10717
#define CMD_ARENA_PROBABILITY_INFO 10718
#define CMD_ARENA_GAMBLING_INFO 10719
#define CMD_ARENA_DIVIDEND_INFO 10720
#define CMD_ARENA_READY_INFO 10721
#define CMD_ARENA_GAME_INFO	10730 // CSD-TW-030701

#define CMD_ARENA_READY_GAME 10722
#define CMD_ARENA_START_GAME 10723
#define CMD_ARENA_END_GAME 10724

#define CMD_ARENA_GAMBLING_RESULT 10725

#define CMD_ARENA_GAME_MESSAGE1 10726
#define CMD_ARENA_GAME_MESSAGE2 10727
#define CMD_ARENA_GAME_MESSAGE3 10728
#define CMD_ARENA_GAME_MESSAGE4 10729 // CSD-TW-030627

struct t_arena_enter_lobby
{
	int nArenaGame; // 아레나 경기 ID
};

struct t_arena_enter_team
{
	int nArenaGame; // 아레나 경기 번호
	int nArenaTeam; // 아레나 팀 번호
};

struct t_arena_leave_team
{
	int nArenaGame; // 아레나 경기 번호
};

struct t_arena_enter_observe
{
	int nArenaGame; // 아레나 경기 번호
	int nArenaTeam; // 아레나 팀 번호
};

struct t_arena_leave_observe
{
	int nArenaGame; // 아레나 경기 번호
};

struct t_arena_enter_gambling
{
	int   nArenaGame;     // 아레나 경기 번호
	int   nArenaTeam;     // 아레나 팀 번호
	DWORD dwBattingMoney; // 배팅금액
};

struct t_arena_leave_gambling
{
	int nArenaGame; // 아레나 경기 번호
};

struct t_arena_add_lobby
{
	WORD idMember;
};

struct t_arena_delete_lobby
{
	WORD idMember;
};

struct t_arena_add_team
{
	WORD idMember;
	int  nTeam;
};

struct t_arena_delete_team
{
	WORD idMember;
	int  nTeam;
};

struct t_arena_add_gambling
{
	WORD idGambler;
	int  nTeam;
};

struct t_arena_delete_gambling
{
	WORD idGambler;
	int  nTeam;
};

struct t_arena_add_observe
{
	WORD idObserve;
	int  nTeam;
};

struct t_arena_delete_observe
{
	WORD idObserve;
	int  nTeam;
};

struct t_arena_leader_info
{
	WORD idLeader;
};

struct t_arena_probability_info
{
	int   nArenaTeam;   // 아레나 팀 번호
	float fProbability; // 아레나 팀 승률
};

struct t_arena_gambling_info
{
	int nArenaTeam;     // 아레나 팀 번호
	DWORD dwBattingMoney; // 아레나 팀에 걸려 있는 배팅금액
};

struct t_arena_dividend_info
{
	DWORD dwBattingMoney;  // 배팅금
	DWORD dwDividendMoney; // 배당금
};

struct t_arena_ready_info
{
	DWORD dwReadyTime;
};

struct t_arena_ready_game
{
	DWORD dwDelay; // 지연시간
};

struct t_arena_game_info
{	//< CSD-TW-030701
	WORD wPort;		   // 그 맵의 포트 번호
	WORD wMemberCount; // 현재 인원
	char szLeader[20]; // 방장
};	//> CSD-TW-030701

struct t_arena_start_game
{
	WORD idLeader;
};

struct t_arena_gambling_result
{
	int nPayment;
	DWORD dwMoney;
};

struct t_arena_game_message1
{
	char nTeamNumber;
	char nMsgType;
	int	 nGameData;
};

struct t_arena_game_message2
{
	char nTeamNumber1;
	char szTeamMember1[20];	
	char nTeamNumber2;
	char szTeamMember2[20];
};

struct t_arena_game_message3
{
	char nTeamNumber;
	char nMsgType;
	char nStoneNumber;
	char szTeamMember[20];
};

struct t_arena_game_message4
{	//< CSD-TW-030627
	DWORD dwRemainMinute;
};	//> CSD-TW-030627

union t_arena
{
	t_arena_enter_lobby      arena_enter_lobby;
	
	t_arena_enter_team       arena_enter_team;
	t_arena_leave_team       arena_leave_team;
	
	t_arena_enter_observe    arena_enter_observe;

	t_arena_enter_gambling   arena_enter_gambling;

	t_arena_add_lobby        arena_add_lobby; 
	t_arena_delete_lobby     arena_delete_lobby;
	
	t_arena_add_team         arena_add_team; 
	t_arena_delete_team      arena_delete_team;

	t_arena_add_gambling	 arena_add_gambling;
	t_arena_delete_gambling  arena_delete_gambling;
	
	t_arena_add_observe  	 arena_add_observe;
	t_arena_delete_observe   arena_delete_observe;

	t_arena_leader_info      arena_leader_info;
	t_arena_probability_info arena_probability_info;
	t_arena_gambling_info    arena_gambling_info;
	t_arena_dividend_info    arena_dividend_info;
	t_arena_ready_info       arena_ready_info; 
	t_arena_game_info        arena_game_info; // CSD-TW-030701

	t_arena_ready_game       arena_ready_game;
	t_arena_start_game       arena_start_game;

	t_arena_gambling_result  arena_gambling_result;
	
	t_arena_game_message1    arena_game_message1;
	t_arena_game_message2    arena_game_message2;
	t_arena_game_message3    arena_game_message3;
	t_arena_game_message4    arena_game_message4; // CSD-TW-030627
};
//
///////////////////////////////////////////////////////////////////////////////
#endif