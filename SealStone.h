
#define NATION_RELATION_NO_					0
// 서로간에 공격할수 없는상태.. 맵이동 O 결계석 부술수 있음.
#define NATION_RELATION_ENEMY_				1
// 서로간에 공격할수 없는상태.. 맵이동 X 결계석 부술수 없음.
#define NATION_RELATION_FRIEND_				2
#define NATION_RELATION_MASTER_SERVANT_		3


// 현재는 전쟁중으로 관계설정이 불가하다. 
#define NATION_RELATION_FAIL_				100
// 그런 나라는 없습니다. 
#define NATION_RELATION_FAIL_NO_NATION_		101
// 요청한 국가관계가 설정되었습니다. 
#define NATION_RELATION_SUCCESS_			102
// 현재 이미 그렇게 설정이 되어 있는상태입니다. 
#define NATION_RELATION_SAME_FAIL_			103


#define MAX_NATION_							8

								
/*------------------------------------------
각 나라별로 결계석이 존재한다.		
------------------------------------------*/
#define MAX_SEALSTONE_NATION_		100
									
/*------------------------------------------
결게석이 다시 강해지는 시점이며 이때 GameServer에 
CMD_SEALSTONE_WEAKTIME_START를 보낸다.
------------------------------------------*/
#define SEALSTONE_WEAKTIME_START_	17
									
/*------------------------------------------
결게석이 다시 강해지는 시점이며 이때 GameServer에 
CMD_SEALSTONE_WEAKTIME_END를 보낸다.
------------------------------------------*/
#define SEALSTONE_WEAKTIME_END_	    5
									
									
//	국가코드..						
#define SEALSTONE_VYSEUS	3		
#define SEALSTONE_ZYPERN	4		
									
//-----------------------------------------------------	
//	국가전의 결과.					
//	SEAL STONE RESULT				
//-----------------------------------------------------	

// 우리지역의 모든 결계석이 부셔졌다. 
#define SSR_FAIL								0
// 우리가 놈들의 결계석을 모두 없앴다. 
#define SSR_VICTORY								1
// 무승부다.							
#define SSR_DRAW								2

	
#define SEALSTONE_WARTIME_			(3600*6)
#define SEALSTONE_WAR_AFTERTIME_	(60*10 ) 
#define SEALSTONE_RE_GENTIME_		(60*20 ) 
	
	
typedef struct sealstone
{	
	DWORD warlefttime;	// 전쟁 남은시간.
	DWORD waraftertime;
	DWORD regentime;	// 결계석 리젠될때까지 남은시간.

	int status[ MAX_SEALSTONE_NATION_];	// 현재 결계석의 상태 0: 없음. 1 : 살아있음.
}t_sealstone;


#ifndef __SEALSTONE__
#define __SEALSTONE__

	extern t_sealstone  SealStone[ MAX_NATION_];
	extern int antination[ MAX_NATION_];

	extern void RecvSealStoneStatus( int sealstonesprno, int id, int status );
	extern void RecvSealStoneFromKing( int nation );
	extern void SendResultNationWar( int aresult, int anation, int bresult, int bnation );
	extern void SendSealStoneReGenStart( void );
	extern void SendEndOfReGenTime( void );
	extern void SendWarLeftTime_sub( int naion );
	extern int  CheckVictory( int nation );
	extern void CheckWarLeftTime( void );
	extern void CheckSealStoneWarAfterTime( void );
	extern void CheckReGenTime( void );
	extern void CheckSealStoneStatus( void );
	extern void CheckNationWar( void );

#else 

	t_sealstone  SealStone[ MAX_NATION_];
	int antination[ MAX_NATION_]= { 0,0,0, SEALSTONE_ZYPERN, SEALSTONE_VYSEUS };
	
#endif





