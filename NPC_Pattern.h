 
#ifndef  MAX_GENERATE_POSITION_

#define NPC_PATTERN_NO_MOVE_			 0		// 가만히 있는다.
#define NPC_PATTERN_HOSTILE_			10		// 적대적으로 바뀐다. 
#define NPC_PATTERN_PEACE_				20		// 평화적으로 바뀐다.
#define NPC_PATTERN_FIND_CLOSE_PC_		30		// 가까운 pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_FIND_WEAK_PC_		40		// 약한  pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_FIND_STRONG_PC_		50		// 강한  pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_ACCESS_PC_			60		// 찾은 PC에 적근한다. ( Hostile일경우 공격범위에 있으면 공격한다. peace일경우 접근해도 가만히 접근하는놈을 바라본다.  
#define NPC_PATTERN_ACCESS_PC_ATTACKED_	70		// 나를 공격한 Pc에 접근한다. 
#define NPC_PATTERN_ATTACK_PC_			71
#define NPC_PATTERN_WANDER_RANGE_1_		80		// 배회한다. ( 1번 배회범위가 1이다. )
#define NPC_PATTERN_WANDER_RANGE_2_		90		// 배화한다. ( 1번 배회범위가 2이다. )
#define NPC_PATTERN_WANDER_RANGE_3_		100  	// 배화한다. ( 1번 배회범위가 3이다. )
#define NPC_PATTERN_ESCAPE_PC_			110		// 찾은 PC로 부터 도망간다. 
#define NPC_PATTERN_ESCAPE_BATTLE_PC_	120		// 근처에 공격 PC가 있으면 멀리 도망간다. 
#define NPC_PATTERN_RETURN_				130		// patterntype을 0으로 Setting한다.
#define NPC_PATTERN_BACKDRAW_			140		// 공격하려는 놈뒤로 3타일 빠진다.

#define NPC_PATTERN_IAM_BOSS_8_			150
#define NPC_PATTERN_TOBOSS_8_			160
#define NPC_PATTERN_WANDER_8_			170
#define NPC_PATTERN_MURI_8_				180
#define NPC_PATTERN_ACCESS_PC_8_		190
#define NPC_PATTERN_ATTACK_PC_8_		200
#define NPC_PATTERN_BACKDRAW_8_			210

#define NPC_PATTERN_IAM_BOSS_28_		220
#define NPC_PATTERN_TOBOSS_28_			230
#define NPC_PATTERN_WANDER_28_			240
#define NPC_PATTERN_MURI_28_			250
#define NPC_PATTERN_BACKDRAW_PC_28_		260

#define NPC_PATTERN_ACCESS_PC_38_		270
#define NPC_PATTERN_ATTACK_PC_38_		280
#define NPC_PATTERN_WANDER_38_			290
#define NPC_PATTERN_MURI_38_			300
#define NPC_PATTERN_BACKDRAW_PC_38_		310


#define NPC_PATTERN_IAM_BOSS_18_		320
#define NPC_PATTERN_TOBOSS_18_			330
#define NPC_PATTERN_WANDER_18_			340
#define NPC_PATTERN_MURI_18_			350
#define NPC_PATTERN_ACCESS_PC_18_		360
#define NPC_PATTERN_ATTACK_PC_18_		370
#define NPC_PATTERN_BACKDRAW_18_		380

#define	NPC_PATTERN_TAME_				390

#define NPC_PATTERN_IAM_BOSS_GUARD_		400
#define NPC_PATTERN_TOBOSS_GUARD_		410
#define NPC_PATTERN_WANDER_GUARD_		420
#define NPC_PATTERN_MURI_GUARD_			430
#define NPC_PATTERN_ACCESS_PC_GUARD_	440
#define NPC_PATTERN_ATTACK_PC_GUARD_	450
#define NPC_PATTERN_BACKDRAW_PC_GUARD_	460


#define NPC_PATTERN_EVENTING_MAUL_		470
#define NPC_PATTERN_WANDER_MAUL_		480
#define NPC_PATTERN_ATTACK_MAUL_		490
#define NPC_PATTERN_BACKDRAW_MAUL_		500
#define NPC_PATTERN_ACCESS_PC_MAUL_		510


#define NPC_PATTERN_SEALSTONE_NO_OPERATE_	520
#define NPC_PATTERN_SEALSTONE_ATTACK_		530



// Client에서 변화된 NPC 파라메터를 Server로 보내준다. 
#define NPC_PARAMETER_TARGETID		0
#define NPC_PARAMETER_TARGETNAME	1
#define NPC_PARAMETER_BOSSID		2
#define NPC_PARAMETER_HOSTID		3
#define NPC_PARAMETER_TAME_CMD		4
#define NPC_PARAMETER_PATTERNTYPE	5
#define NPC_PARAMETER_OLDPATTERNTYPE 6
#define NPC_PARAMETER_ATTRIBUTE 7 // CSD-031106



#define MAX_GENERATE_POSITION_			100

#define ATTACKACCESS(a)		( NPCAccessTable[##a->mutant * 100 + ##a->sprno][0] )

// 0813 NPC KHS
#define PCFINDACCESS(a)		( NPCAccessTable[##a->mutant * 100 + ##a->sprno][1] )

typedef struct tNPCgeneratePosition
{
	short int curnpc;
	short int npcindex;
	short int eventno;
	short int x, y;
	short int maxno;
}sNPCgeneratePosition;

#ifndef __NPC_PATTERN_H__
#define __NPC_PATTERN_H__


/////////////////////////////////////////////////////////////
///  Variables

	extern int NpcPatternTable[20];
	extern sNPCgeneratePosition NPCgeneratePosition[ MAX_GENERATE_POSITION_];
	extern int NPCgeneratePositionMax;

	extern bool CharMovingThreadStart0;
	extern bool CharMovingThreadStart1;
	extern bool CharMovingThreadStart2;

	// 0811 NPC KHS
	extern short NPCAccessTable[400][4];

/////////////////////////////////////////////////////////////
///  User Function

extern void SendModifyPositionNPC( int id );
extern void SendNPCChatArea( int id, char *data );
extern void SendNPCAttack( int attacker_npc_id);
extern void SendNPC_parameter( int id, int type, int data );
extern void RecvNPC_parameter( int id, int type, int data );
extern void RecvNPC_stringparameter( int id, int type, char *data );



extern void InitMoveDelayTable( void );
extern int Get_MoveDelayTime( CHARACTER *ch );
extern int InDistance( CHARACTER *s, CHARACTER *d, int dis );
extern int NPC_NearRoundCh( CHARACTER *n, CHARACTER *ch, int *x, int *y );
extern int NPC_NearCh( CHARACTER *n, CHARACTER *ch, int *x, int *y );
extern int NPC_NearPosition( CHARACTER *n, CHARACTER *c, int *x, int *y );
extern int NPC_NearBackCh( CHARACTER *n, CHARACTER *ch, int *x, int *y, int dist );
extern int NPC_GetAttackRange( CHARACTER *n );
extern int NPC_GetPatternTable( CHARACTER *n );
extern int NPC_GetDir( int sx, int sy, int ex, int ey );
extern int NPC_MakePath( CHARACTER *n, int tx, int ty, int how );
extern int NPC_MakePathBumn( CHARACTER *n, int tx, int ty, int how );
extern inline bool NPC_IsMoving(CHARACTER* n); // CSD-040107
extern int NPC_Hostile( CHARACTER *n );
extern int NPC_StillAttack( CHARACTER *n);
extern int NPC_IsAttackableRange( CHARACTER *n );
extern int NPC_IsInBossRange( CHARACTER *n );
extern int NPC_IsInHostRange( CHARACTER *n );
extern int NPC_IsWhoNearRedPC( CHARACTER *npc, int tilerange  );
extern int NPC_IsWhoNearPC( CHARACTER *npc, int tilerange );
extern int NPC_WhoIsAttackMyBaby( CHARACTER *n );
extern int NPC_WhoIsBoss( CHARACTER *npc );
extern int NPC_WhoIsBlocking( int sx, int sy );
extern CHARACTER *NPC_ReturnCHARACTERPoint( int id );
extern CHARACTER *ReturnCHARACTERPoint( char *name );
extern void RecvTameCommand( int cn, t_tame_command *p );
extern void SendTameResult( int cn, int result  );
extern void SendHostName( int cn, CHARACTER *n );
extern void SendToAllHostName( int cn, CHARACTER *n );
extern void CheckNPCTammingTimeElapsed( CHARACTER *n );

	extern DWORD ReturnDestTime( LPCHARACTER ch );
	extern void SaveMoveDelayTable( void );
	extern void InitMoveDelayTable( void );


	extern void StartCharacterMovingThread( void );
	extern void EndCharacterMovingThread( void );


	extern int NPC_MakePathGuard( CHARACTER *n, int tx, int ty, int how );
	extern int NPC_MakePathMaul( CHARACTER *n, int tx, int ty, int how );
	
	extern void NPCAddBasicData(  int i, t_server_npc_add *p );

	extern void NPCGeneratePosition( int npcindex, int x, int y, int eventno, int maxno );
	extern void NPC_Create( int Num, int npcindex, int x, int y, int eventno, int maxno );
	extern int  NPC_AdjustPosition( int sprno, int *x, int *y );
	extern void NPC_AutoCreationProc( void );
	extern void NPC_CheckRemove( void );
	extern void RecvCMD_MODIFY_POSITION(  int cn,  t_client_modify_position *p );
	extern void SendReadyMoveDate( int cn );
	extern void RecvSmileFace( int cn, int smileno );

	extern int NPC_IsTherePC( CHARACTER *npc, int range );
	extern int NPC_IsWhoNearPC2( CHARACTER *npc, int range );
	extern int NPC_IsWhoNearOtherNationPc( int npc_nation, LPCHARACTER id, int range );

	extern void Check_NPC_Pattern( LPCHARACTER  n );

	extern void RecvStartGoodBad( int id );
	extern void RecvEndofGoodBad( int id );


	extern void RecvNK( int id, int nk3, int nk4, int nk6 );
	extern void RecvServerKillPc(  int id, int killpc );

	extern void CheckNPCPk( int aid, int did );


	// 0811 NPC KHS
	extern void SaveNPCAcessTable( void );
	extern void LoadNPCAccessTable( void );
	extern void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD( t_packet *p );
	extern void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD2( t_packet *p );
	extern void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD3( t_packet *p );
	extern void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD4( t_packet *p );

	extern void Recv_FaceDirections( t_face_directions *tp );
	extern void Send_JustAni( int id, int anino );
	extern void Recv_JustAni( t_just_ani *p );
	extern void NPC_Pattern_SealStone( LPCHARACTER n );

	extern char *LoadMonNameTable( int namenumber );

#else


#endif


#endif
















