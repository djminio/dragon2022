
#define NATION_RELATION_NO_					0
// ���ΰ��� �����Ҽ� ���»���.. ���̵� O ��輮 �μ��� ����.
#define NATION_RELATION_ENEMY_				1
// ���ΰ��� �����Ҽ� ���»���.. ���̵� X ��輮 �μ��� ����.
#define NATION_RELATION_FRIEND_				2
#define NATION_RELATION_MASTER_SERVANT_		3


// ����� ���������� ���輳���� �Ұ��ϴ�. 
#define NATION_RELATION_FAIL_				100
// �׷� ����� �����ϴ�. 
#define NATION_RELATION_FAIL_NO_NATION_		101
// ��û�� �������谡 �����Ǿ����ϴ�. 
#define NATION_RELATION_SUCCESS_			102
// ���� �̹� �׷��� ������ �Ǿ� �ִ»����Դϴ�. 
#define NATION_RELATION_SAME_FAIL_			103


#define MAX_NATION_							8

								
/*------------------------------------------
�� ���󺰷� ��輮�� �����Ѵ�.		
------------------------------------------*/
#define MAX_SEALSTONE_NATION_		100
									
/*------------------------------------------
��Լ��� �ٽ� �������� �����̸� �̶� GameServer�� 
CMD_SEALSTONE_WEAKTIME_START�� ������.
------------------------------------------*/
#define SEALSTONE_WEAKTIME_START_	17
									
/*------------------------------------------
��Լ��� �ٽ� �������� �����̸� �̶� GameServer�� 
CMD_SEALSTONE_WEAKTIME_END�� ������.
------------------------------------------*/
#define SEALSTONE_WEAKTIME_END_	    5
									
									
//	�����ڵ�..						
#define SEALSTONE_VYSEUS	3		
#define SEALSTONE_ZYPERN	4		
									
//-----------------------------------------------------	
//	�������� ���.					
//	SEAL STONE RESULT				
//-----------------------------------------------------	

// �츮������ ��� ��輮�� �μ�����. 
#define SSR_FAIL								0
// �츮�� ����� ��輮�� ��� ���ݴ�. 
#define SSR_VICTORY								1
// ���ºδ�.							
#define SSR_DRAW								2

	
#define SEALSTONE_WARTIME_			(3600*6)
#define SEALSTONE_WAR_AFTERTIME_	(60*10 ) 
#define SEALSTONE_RE_GENTIME_		(60*20 ) 
	
	
typedef struct sealstone
{	
	DWORD warlefttime;	// ���� �����ð�.
	DWORD waraftertime;
	DWORD regentime;	// ��輮 �����ɶ����� �����ð�.

	int status[ MAX_SEALSTONE_NATION_];	// ���� ��輮�� ���� 0: ����. 1 : �������.
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





