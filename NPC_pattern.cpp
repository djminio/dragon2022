#include "StdAfx.h"

#include <Stdio.h>
#include "Dragon.h"	

#include "Path.h"
#include "CharDataTable.h"
#include "Hong_Sub.h"
#include "CounSeler.h"
#include "Item.h"

#define __NPC_PATTERN_H__
#include "NPC_Pattern.h"

#include "NetWork.h"
#include "Map.h"
#include "Hong_Sprite.h"

extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
extern struct CItem_List Item_Ref ;
extern void SendNPCAttack( t_connection c[], int npc );
extern void calcNewAbility(CHARACTER *) ;
extern int  getMaxHungry(CHARACTER *chr) ;
extern void SetDamageOfHero( CHARACTER *ch ); // OP_Chr_Status.cpp
extern ItemAttr GenerateItem( int type, int item_no, DWORD intattr2, int success_rate );
/////////////// 0124 이규훈 //////////////////
extern int  calcAC( CHARACTER *ch );
extern int  calcDefenceRate( CHARACTER *ch );
extern void SendViewTypeChange( int cn, int viewtype );


extern void NPC_Pattern_Tammed( CHARACTER *n );
extern void NPC_Pattern_GUARD( LPCHARACTER n);
extern void NPC_Pattern_MAUL( LPCHARACTER n);
extern void NPC_Pattern_8( CHARACTER *n );
extern void NPC_Pattern_SealStone( LPCHARACTER n );

//extern NPCName_by_Gender	NPC_Name_Ref[Num_Of_NPC_Name];
//extern NPC_Info				NPC_Info_Ref[Num_Of_NPC_Type];
//extern NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev] ;
//extern NPC_Generation		NPC_Gen_Ref[Num_Of_NPC_Generation] ;

extern int writedebug;
extern int g_DontPlayNPC;

/*
#define NPC_PATTERN_NO_MOVE_			  0		// 가만히 있는다.
#define NPC_PATTERN_HOSTILE_			 10		// 적대적으로 바뀐다. 
#define NPC_PATTERN_PEACE_				 20		// 평화적으로 바뀐다.
#define NPC_PATTERN_FIND_CLOSE_PC_		 30		// 가까운 pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_FIND_WEAK_PC_		 40		// 약한  pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_FIND_STRONG_PC_		 50		// 강한  pc를 찾는다.(그쪽을 바라본다. )
#define NPC_PATTERN_ACCESS_PC_			 60		// 찾은 PC에 적근한다. ( Hostile일경우 공격범위에 있으면 공격한다. peace일경우 접근해도 가만히 접근하는놈을 바라본다.  
#define NPC_PATTERN_ACCESS_PC_ATTACKED_	 70		// 나를 공격한 Pc에 접근한다. 
#define NPC_PATTERN_WANDER_RANGE_1_		 80		// 배회한다. ( 1번 배회범위가 1이다. )
#define NPC_PATTERN_WANDER_RANGE_2_		 90		// 배화한다. ( 1번 배회범위가 2이다. )
#define NPC_PATTERN_WANDER_RANGE_3_		100  	// 배화한다. ( 1번 배회범위가 3이다. )
#define NPC_PATTERN_ESCAPE_PC_			110		// 찾은 PC로 부터 도망간다. 
#define NPC_PATTERN_ESCAPE_BATTLE_PC_	120		// 근처에 공격 PC가 있으면 멀리 도망간다. 
*/


////////////////////////////////////////////////////////////////////////////////////////////
///////////   Variables..

int NpcPatternTable[20] = { NPC_PATTERN_HOSTILE_,
							NPC_PATTERN_FIND_CLOSE_PC_,
							NPC_PATTERN_ACCESS_PC_,
							NPC_PATTERN_ATTACK_PC_,
							NPC_PATTERN_BACKDRAW_,
							NPC_PATTERN_RETURN_,		
};


// 0811 NPC KHS
short NPCAccessTable[400][4];



static int movep;





///////////////////////////////////////////////////////////////////////////////////////////////////
///
///				User Function Declarations..
///
void InitMoveDelayTable( void );
int Get_MoveDelayTime( CHARACTER *ch );
int InDistance( CHARACTER *s, CHARACTER *d, int dis );
int NPC_NearRoundCh( CHARACTER *n, CHARACTER *ch, int *x, int *y );
int NPC_NearCh( CHARACTER *n, CHARACTER *ch, int *x, int *y );
int NPC_NearPosition( CHARACTER *n, CHARACTER *c, int *x, int *y );
int NPC_NearBackCh( CHARACTER *n, CHARACTER *ch, int *x, int *y, int dist );
int NPC_GetAttackRange( CHARACTER *n );
int NPC_GetPatternTable( CHARACTER *n );
int NPC_GetDir( int sx, int sy, int ex, int ey );
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how );
int NPC_MakePathBumn( CHARACTER *n, int tx, int ty, int how );
inline bool NPC_IsMoving(CHARACTER *n); // CSD-040107
int NPC_Hostile( CHARACTER *n );
int NPC_StillAttack( CHARACTER *n);
int NPC_IsAttackableRange( CHARACTER *n );
int NPC_IsInBossRange( CHARACTER *n );
int NPC_IsInHostRange( CHARACTER *n );
int NPC_IsWhoNearRedPC( CHARACTER *npc, int tilerange  );
int NPC_IsWhoNearPC( CHARACTER *npc, int tilerange );
int NPC_WhoIsAttackMyBaby( CHARACTER *n );
int NPC_WhoIsBoss( CHARACTER *npc );
int NPC_WhoIsBlocking( int sx, int sy );
CHARACTER *NPC_ReturnCharListPoint( int id );
CHARACTER *ReturnCharListPoint( char *name );
void RecvTameCommand( int cn, t_tame_command *p );
void SendTameResult( int cn, int result  );
void SendHostName( int cn, CHARACTER *n );
void CheckNPCTammingTimeElapsed( CHARACTER *n );
	
	
////////////////////////////////////////////////////////////////////////////////////////////////
////
////			   User Function
////
		

void SendNPC_parameter( int id, int type, int data )
{
	t_packet p;

	p.h.header.type = CMD_SEND_NPC_PARAMETER;

	p.u.send_npc_parameter.id	= id;
	p.u.send_npc_parameter.type	= type;
	p.u.send_npc_parameter.data = data;

	p.h.header.size = sizeof( t_send_npc_parameter );
	
	QueuePacket( &p, 1 );
}

void RecvNPC_parameter(int id, int type, int data)
{	//< CSD-031106
	LPCHARACTER ch = ReturnCharacterPoint(id);
	if (ch == NULL) return;

	switch (type)
	{
	case NPC_PARAMETER_TARGETID:
		{
			ch->targetid = data;
			break;
		}
	case NPC_PARAMETER_BOSSID:
		{
			ch->bossid = data;
			break;
		}
	case NPC_PARAMETER_HOSTID:
		{
			ch->HostId = data;
			break;
		}
	case NPC_PARAMETER_TAME_CMD:
		{
			ch->tame_cmd = data;
			break;
		}
	case NPC_PARAMETER_PATTERNTYPE:
		{
			ch->patterntype	= data;
			break;
		}
	case NPC_PARAMETER_OLDPATTERNTYPE:
		{
			ch->oldpatterntype	= data;
			break;
		}
	case NPC_PARAMETER_ATTRIBUTE:
		{
			ch->nAttr = data;
			break;
		}
	}
}	//> CSD-031106

void RecvNPC_stringparameter( int id, int type, char *data )
{
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;

	switch( type )
	{
	case NPC_PARAMETER_TARGETNAME	:	strcpy( ch->TargetName, data );
		break;

	default : break;
	}
}	
	
	
	
	
	
int InDistance( CHARACTER *s, CHARACTER *d, int dis )
{	
	if( s == NULL ) return 0;
	if( d == NULL ) return 0;
	
 	if( IsDead( s ) ) return 0;
	if( IsDead( d ) ) return 0;
	
	int sx = s->x;
	int sy = s->y;
	int dx = d->x;
	int dy = d->y;

	if( dis * dis > (dx-sx)*(dx-sx)+(dy-sy)*(dy-sy) ) return 1;
	
	return 0;
}	



void SendModifyPositionNPC( int id )
{
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if (ch == NULL) return; // finito null check

	t_packet p;

	if( ch->patterntype == 0 && ch->HostName[0] == 0 ) return;

	p.h.header.type = CMD_NPC_MODIFY_POS;

		p.u.npc_modify_pos.id = id;
		p.u.npc_modify_pos.mx = ch->x / TILE_SIZE;
		p.u.npc_modify_pos.my = ch->y / TILE_SIZE;

	p.h.header.size = sizeof( t_npc_modify_pos );

	QueuePacket( &p, 1 );
}




void SendNPCChatArea( int id, char *data )
{
	t_packet p;
	int Len;

	if( data == NULL ) return;

	Len = strlen( data );
	if( Len <= 0 ) return;

	p.h.header.type = CMD_NPC_CHAT_DATA;
		p.u.npc_chat_data.id = id;
		strcpy(p.u.npc_chat_data.data, data);
	p.h.header.size = sizeof( t_npc_chat_data ) - TEXT_LENGTH + Len;

	
	QueuePacket( &p, 1 );
}

void SendNPCAttack( int attacker_npc_id )
{
	t_packet	p;
	int			magic_no=0;			//일반적으로 마법을 구사하는 몬스터만 마법 번호 부여


	LPCHARACTER attacker_npc_ch = ReturnCharacterPoint( attacker_npc_id );
	if( attacker_npc_ch == NULL ) return;
	if( attacker_npc_ch->targetid == -1 ) return;
	LPCHARACTER diffender_npc_ch = ReturnCharacterPoint( attacker_npc_ch->targetid );

	
	if( diffender_npc_ch == NULL ) return;

	if( attacker_npc_ch->viewtype == VIEWTYPE_STONE_ ) return;	//001215 KHS
	if( diffender_npc_ch->viewtype == VIEWTYPE_STONE_ ) return;	//001215 KHS
	if( diffender_npc_ch->accessory[0] == 114 )  return;

	//switch(attacker_npc_ch->sprno)
	//{
	//case 40:						//맨스콜피언->화살류 발사
		//magic_no = 30+rand()%3;
		//break;
	//}

	if( magic_no )							//특정 마법을 구사는 NPC인 경우
	{
		LPCHARACTER		target_ch = ReturnCharacterPoint( attacker_npc_ch->targetid );
		if(target_ch==NULL)		return;
		return;
	}
	
	
	DIRECTION dir;	
	int x, y;		
	int	t_X, t_Y;	
	x = diffender_npc_ch->x;
	y = diffender_npc_ch->y;
	int how;
	
	switch(rand()%6)
	{
	case 0:
	case 1:
	case 2:
		how = 0;
		break;
	case 3:
		how	= 12;
		break;
	case 4:
		how = 18;
		break;
	case 5:
		how = 24;
		break;
	}
	
	ChangeDirection( &dir, attacker_npc_ch->x, attacker_npc_ch->y, x, y );

	switch( dir )
	{
		case DIRECTION_DOWN		:	t_X= 0;		t_Y=how;		break;
		case DIRECTION_LEFTDOWN	:	t_X=-how;	t_Y=how;		break;
		case DIRECTION_LEFT		:	t_X=-how;	t_Y=0;			break;
		case DIRECTION_LEFTUP	:	t_X=-how;	t_Y=-how;		break;
		case DIRECTION_UP		:	t_X= 0;		t_Y=-how;		break;
		case DIRECTION_RIGHTUP	:	t_X=how;	t_Y=-how;		break;
		case DIRECTION_RIGHT	:	t_X=how; 	t_Y=0;			break;
		case DIRECTION_RIGHTDOWN:	t_X=how;	t_Y=how;		break;
		default					:	dir = DIRECTION_SAME; t_X=0;	t_Y=0;		break;
	}

	if( t_X || t_Y )	{	x += t_X;	y += t_Y;	}
	else				{	x =0;		y = 0;		}						//뒤로 밀리는 연출 일어나지 않도록
	{
    p.h.header.type = CMD_NPC_ATTACK;
		p.u.strike.npc_attack.idCaster = WORD(attacker_npc_id);
		p.u.strike.npc_attack.idTarget = WORD(attacker_npc_ch->targetid);
		
		if (FreeTile(diffender_npc_ch, x>>5, y>>5, x>>5, y>>5))			//실제 갈수 있는 지점인가?
		{
			p.u.strike.npc_attack.nX = x;
			p.u.strike.npc_attack.nY = y;
		}
		else
		{
			p.u.strike.npc_attack.nX = 0;
			p.u.strike.npc_attack.nY = 0;
		}
		
		p.h.header.size = sizeof( t_npc_attack );
		QueuePacket( &p, 1 );    
  }
}

int NPC_NearPosition( CHARACTER *n, CHARACTER *c, int *x, int *y )
{	
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;
	
	if( IsDead( c ) ) return 0;
	
	// 공격 대상이 없어..
	
	tx = c->x / TILE_SIZE;
	ty = c->y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 1 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}
			if( FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )  
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}
	
	return 0;
}

int NPC_NearRoundCh( CHARACTER *n, CHARACTER *ch, int *x, int *y )
{	
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;
	
	if( n->targetid == -1 ) 
	{
		n->targetid = NPC_IsWhoNearPC( n, 10 );
		if( n->targetid == -1 )  return 0;
	}
	
	LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
	if (tch == NULL) return 0; // finito null check
	
	// 공격 대상이 없어..
	if( IsDead( tch ) ) return 0;
	
	tx = tch->x / TILE_SIZE;
	ty = tch->y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 1 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}
			if( FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )  
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}
	return 0;
}	

	
// 공격대상에 대해 접근할때 사용된다. 
/*int NPC_NearCh( CHARACTER *n, CHARACTER *ch, int *x, int *y )
{
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;

	if( n->targetid == -1 ) 
	{
		n->targetid = NPC_IsWhoNearPC( n, 10 );
		if( n->targetid == -1 )	return 0;
	}

	// 공격 대상이 없어..
	if( !connections[n->targetid].socket || connections[n->targetid].state < CONNECT_JOIN ) return 0;
	if( connections[n->targetid].chrlst.bAlive >= DEAD_ ) return 0;

	tx = connections[ n->targetid].chrlst.x / TILE_SIZE;
	ty = connections[ n->targetid].chrlst.y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 2 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}
			if( FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )  
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}

	return 0;
}*/
// 만약 목표캐릭이 이동중이면 도달목표점을 리턴해준다. 
// 단지 그곳의 근처에 간다. ( Tamming에 사용된다. )









#define HOME_DIST_1AREA		245
#define HOME_DIST_2AREA		625



int NPC_NearCh( CHARACTER *n, CHARACTER *ch, int *x, int *y )
{			
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;
		
	if( ch == NULL )			return 0;
		
		
	tx = ch->x / TILE_SIZE;
	ty = ch->y / TILE_SIZE;
		
	if( ch->sprno == 0 || ch->sprno == 1 ) // PC라면 
	{	
		if( (rand()%100) < 70 )
		{
			tx = ch->gox / TILE_SIZE;
			ty = ch->goy / TILE_SIZE;
		}
	}	


	DWORD area1, area2;

	switch( n->patterntype )
	{
//------------------------------------------------------------------------------------------------------------

	case  NPC_PATTERN_ACCESS_PC_38_		:
	case  NPC_PATTERN_ATTACK_PC_38_		:
	case  NPC_PATTERN_WANDER_38_		:
	case  NPC_PATTERN_MURI_38_			:
	case  NPC_PATTERN_BACKDRAW_PC_38_	:	area1 = (n->x/TILE_SIZE - n->homex)*(n->x/TILE_SIZE - n->homex) + (n->y/TILE_SIZE - n->homey)*(n->y/TILE_SIZE - n->homey);
											area2 = (tx - n->homex)*(tx - n->homex) + (ty - n->homey)*(ty - n->homey);
											if( area1 > HOME_DIST_1AREA )
											{	
												if( area2 > HOME_DIST_2AREA )
												{
													tx = n->homex;
													ty = n->homey;
												}
											}	
								break;
/*
	case NPC_PATTERN_WANDER_8_			:
	case NPC_PATTERN_IAM_BOSS_8_		:
	case NPC_PATTERN_TOBOSS_8_			:
	case NPC_PATTERN_MURI_8_			:`
	case NPC_PATTERN_ACCESS_PC_8_		:
	case NPC_PATTERN_ATTACK_PC_8_		:
	case NPC_PATTERN_BACKDRAW_8_		:	

	case NPC_PATTERN_IAM_BOSS_28_		:
	case NPC_PATTERN_TOBOSS_28_			:
	case NPC_PATTERN_WANDER_28_			:
	case NPC_PATTERN_MURI_28_			:
	case NPC_PATTERN_BACKDRAW_PC_28_	:	

	case  NPC_PATTERN_IAM_BOSS_GUARD_	:
	case  NPC_PATTERN_TOBOSS_GUARD_		:
	case  NPC_PATTERN_WANDER_GUARD_		:
	case  NPC_PATTERN_MURI_GUARD_		:
	case  NPC_PATTERN_ACCESS_PC_GUARD_	:
	case  NPC_PATTERN_ATTACK_PC_GUARD_	:
	case  NPC_PATTERN_BACKDRAW_PC_GUARD_:	

	case NPC_PATTERN_TAME_				:	
	case NPC_PATTERN_EVENTING_MAUL_		:
	case NPC_PATTERN_WANDER_MAUL_		:
		break;
		*/
	}

	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 1 ; j < 10 ; j ++)
	{		
		for( i = 0 ;  i < 5 ; i ++)
		{	
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
				case 0 :			tty +=j; break;
				case 1 : ttx -=j;	tty +=j; break;
				case 2 : ttx -=j;			 break;
				case 3 : ttx -=j;	tty -=j; break;
				case 4 :			tty -=j; break;
				case 5 : ttx +=j;	tty -=j; break;
				case 6 : ttx +=j;			 break;
				case 7 : ttx +=j;	tty +=j; break;
			}
			if( FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )  
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}	
	}		
			
	return 0;
}







#define TILE_BLOCK_SIZE  11


BYTE TileBlock[ TILE_BLOCK_SIZE][ TILE_BLOCK_SIZE] = {
	{ 0,0,0,0,1,1,1,0,0,0,0 },
	{ 0,0,1,1,1,1,1,1,1,0,0 },
	{ 0,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,0 },
	{ 1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1 },
	{ 0,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,0 },
	{ 0,0,1,1,1,1,1,1,1,0,0 },
	{ 0,0,0,0,1,1,1,0,0,0,0 }};

BYTE TileBlockBuf[ TILE_BLOCK_SIZE][ TILE_BLOCK_SIZE];

void GetTileBlock( int x, int y )
{

	int sx, sy, ex, ey;

	sx = x - (TILE_BLOCK_SIZE / 2 );
	sy = y - (TILE_BLOCK_SIZE / 2 );
	ex = x + (TILE_BLOCK_SIZE / 2 );
	ey = y + (TILE_BLOCK_SIZE / 2 );

	
	for( ; sx <= ex ; sx ++ )
	{
		for( ; sy <= ey ; sy ++ )
		{

		}
	}
}


void PutTileBlock( int x, int y, int xl, int yl )
{
}



// 주의에 퍼진 위치를 찾는다. 
int NPC_NearChPart( CHARACTER *n, CHARACTER *ch, int *x, int *y )
{
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;

	if( n->targetid == -1 ) 
	{
		n->targetid = NPC_IsWhoNearPC( n, 10 );
		if( n->targetid == -1 )  return 0;
	}

	// 공격 대상이 없어..
	LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
	if (tch == NULL) return 0; // finito null check
	if( IsDead( tch ) ) return 0;

	tx = tch->x / TILE_SIZE;
	ty = tch->y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
	for( j = 1 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}
			if( FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )  
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}
	return 0;
}	
	
	
// 현재위치에서 dist타일만큼  뒤로 빠진다. 
int NPC_NearBackCh( CHARACTER *n, CHARACTER *ch, int *x, int *y, int dist )
{	
	int dirt[ 5] = { 0,1,-1,-2,2 };
	int d, i, j, tempdir;
	int tx, ty;
	int ttx, tty;
	
	if( n->targetid == -1 ) 
	{
//		n->targetid = NPC_IsWhoNearPC( n, 10  );
//		if( n->targetid == -1 )  
		return 0;
	}
	
	// 공격 대상이 없어..
	LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
	if (tch == NULL) return 0; // finito null check
	if( IsDead( tch ) ) return 0;

	tx = tch->x / TILE_SIZE;
	ty = tch->y / TILE_SIZE;
	*x = n->x / TILE_SIZE;  *y = n->y / TILE_SIZE;
	tempdir = NPC_GetDir( tx, ty, n->x / TILE_SIZE, n->y / TILE_SIZE );
	
 	switch( tempdir )
	{
		case 0 :			ty +=dist;	break;
		case 1 : tx -=dist;	ty +=dist;	break;
		case 2 : tx -=dist;				break;
		case 3 : tx -=dist;	ty -=dist;	break;
		case 4 :			ty -=dist;	break;
		case 5 : tx +=dist;	ty -=dist;	break;
		case 6 : tx +=dist;				break;
		case 7 : tx +=dist;	ty +=dist;	break;
	}
	
	for( j = 0 ; j < 10 ; j ++)
	{
		for( i = 0 ;  i < 5 ; i ++)
		{
			ttx = tx;	tty = ty;
			d = ( tempdir + 8 + dirt[i] ) %8;
			switch( d )
			{
			case 0 :			tty +=j; break;
			case 1 : ttx -=j;	tty +=j; break;
			case 2 : ttx -=j;			 break;
			case 3 : ttx -=j;	tty -=j; break;
			case 4 :			tty -=j; break;
			case 5 : ttx +=j;	tty -=j; break;
			case 6 : ttx +=j;			 break;
			case 7 : ttx +=j;	tty +=j; break;
			}

			if( FreeTile( n, n->x / TILE_SIZE, n->y / TILE_SIZE, ttx, tty ) )  
			{
				*x = ttx;  *y = tty;
				return 1;
			}
		}
	}
	
	return 0;
}	
	
	
	
	
//	접근 거리 
int NPC_GetAttackRange( CHARACTER *n )
{	
	// 나중에 Table값으로 대치......
	switch( n->sprno )
	{
	case 8 :	return 50 * 50 * 2;
	case 38 :	return 80 * 80 * 2;
	default : 	return 80 * 80 * 2; 
	}
}	
	
	
	
int NPC_GetPatternTable( CHARACTER *n )
{	
	if( n->patterntype >= 20 ) n->patterntype = 0;
	return NpcPatternTable[n->patterntype];
}	
/********************
      3  4  5 
       \ | /
    2- sx,sy -6
       / | \ 
	  1  0  7   
*********************/		 
int NPC_GetDir( int sx, int sy, int ex, int ey )
{	
	if( sx > ex )  // 1/2/3
	{
		if( sy > ey )		return 3;
		else if( sy < ey )	return 1; 
		else				return 2;
	}
	else if( sx < ex ) // 5/6/7
	{
		if( sy > ey )		return 5;
		else if( sy < ey )	return 7; 
		else				return 6;
	}
	else 
	{
		if( sy > ey )		return 4;
		else if( sy < ey )	return 0;
	}
		
	return -1;
}		
			
	
	
	
	
/*	
	
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how = 0 )
{		
	int dir;
	int c, i;
	int nx, ny, px, py;
		
	n->tx = tx;
	n->ty = ty;
	n->how = how;
	
	if( n->pathcount < 2 )
	{
		if( PathBuild( n, tx, ty ) == false ) { return 0; }
	
		int bOdd = 0;
	
		if( n->pathcount )
		{
			px = n->path[ n->pathcount -1][0];
			py = n->path[ n->pathcount -1][1];
	
			c = 0;
			for( i = n->pathcount -2 ; i >= 0 ; i -- )
			{		
				nx = n->path[i][0];
				ny = n->path[i][1];
					
				if( nx > px ) 
				{	
					if( ny > py )		{ dir = 7; }
					else if( ny < py )	{ dir = 5; }
					else				{ dir = 6; }
				}	
				else if( nx < px )
				{	
					if( ny > py )		{ dir = 1; }
					else if( ny < py )	{ dir = 3; }
					else				{ dir = 2; }
				}	
				else 
				{	
					if( ny > py )		{ dir = 0; }
					else				{ dir = 4; }
				}	
	
	
				if (  bOdd == 0){	n->MoveDirTbl[ (c/2) ] = dir << 4;	}
				else			{	n->MoveDirTbl[ (c/2) ] += dir;		}
				c++;
				bOdd = !bOdd;
				px = nx;
				py = ny;
			}
	
			n->MoveLength		= c;
			n->pathcount	= 0;
			n->MoveGox			= tx * TILE_SIZE + 16 -5 + Random(10);
			n->goy			= ty * TILE_SIZE + 16 -5 + Random(10);
			n->WalkTime			= g_ClientTime; // ::timeGetTime();
			n->MoveType			= 0;
	
			
			return 1;
		}
	
		return 0;
	}		
	
	
	return 0;
}				
	
	
*/	
	

					
						
// 목적지가 적당한곳인자 아닌지를 Check한다. 
// NPC는 문근처에 설수 없다. 

// 갈수 없는곳이면 0을 리턴한다. 
bool CheckDestinationOk( int tx, int ty )
{					
	LPITEMGROUND  i = g_Item;

	return 1;
					
	while( i != NULL )
	{				
		switch( i->type )
		{			
		case ITEMTYPE_DOOR : 
			{		
				if( Distance( i->x, i->y, tx, ty ) < 18432 ) //타일 3개 차이...
				{	
//					return 0;
				}	
			}		

		}			
		i = i->next;
	}	

	return 1;

}		
					
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how = 0 )
{					
	CHARACTER t;
	// 아래 상태에서는 이동이 불가능
  if (n->condition&CON_STONE)   return 0;
	if (n->eventnpc)              return 0;

	if( CheckDestinationOk( tx * TILE_SIZE, ty * TILE_SIZE ) == 0 ) return 0;

	static DWORD time;
	static int   count;
	static DWORD addtime;
	static DWORD temptime;

	time = ::timeGetTime();


			
	memcpy( &t, n , sizeof( CHARACTER ) );
	t.moveFlag = TRUE;
	t.movetype = 0;
	t.gox = tx * TILE_SIZE + 16;
	t.goy = ty * TILE_SIZE + 16;
	t.destx = tx, t.desty = ty;

	n->MoveP	= 0;

	if ( PathBuild( &t ) )
	{	
			t.moveFlag = FALSE;
			t.movetype += 100;
			SendMoveData( &t );	
			t.movetype -= 100;

			temptime =  ::timeGetTime() - time;
			addtime += temptime;
			count ++;

			// 010414 KHS deletion for "temptime.txt" file generation...
			//fp = Fopen( "temptime.txt", "at+" );
			//fprintf( fp, "%3d  : %5d    %d \n", count, addtime, addtime / count );
			//fclose(fp);
		
			return 1;
	}		
	
	n->MoveP	= 0;

			temptime =  ::timeGetTime() - time;
			addtime += temptime;
			count ++;

			// 010414 KHS deletion for "temptime.txt" file generation...
			//fp = Fopen( "temptime.txt", "at+" );
			//fprintf( fp, "%3d  : %5d    %d \n", count, addtime, addtime / count );
			//fclose(fp);

	return 0;
}
	
	
	
/*	
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how = 0 )
{		
	int dir;
	int c, i;
	int nx, ny, px, py;


	if( g_block_move )
	{
		return 0;
	}

	n->destx = tx;
	n->desty = ty;
	n->how = how;
	if( n->pathcount < 2 )
	{
		if( PathBuild( n, tx, ty ) == false ) { return 0; }
	
		int bOdd = 0;
	
		if( n->pathcount )
		{
			px = n->path[ n->pathcount -1][0];
			py = n->path[ n->pathcount -1][1];
	
			c = 0;
			for( i = n->pathcount -2 ; i >= 0 ; i -- )
			{		
				nx = n->path[i][0];
				ny = n->path[i][1];
					
				if( nx > px ) 
				{	
					if( ny > py )		{ dir = 7; }
					else if( ny < py )	{ dir = 5; }
					else				{ dir = 6; }
				}	
				else if( nx < px )
				{	
					if( ny > py )		{ dir = 1; }
					else if( ny < py )	{ dir = 3; }
					else				{ dir = 2; }
				}	
				else 
				{	
					if( ny > py )		{ dir = 0; }
					else				{ dir = 4; }
				}	
	
	
				if (  bOdd == 0){	n->MoveDirTbl[ (c/2) ] = dir << 4;	}
				else			{	n->MoveDirTbl[ (c/2) ] += dir;		}
				c++;
				bOdd = !bOdd;
				px = nx;
				py = ny;
			}
	
			n->MoveLength		= c;
			n->pathcount	= 0;
			n->MoveGox			= tx * TILE_SIZE + 16 -5 + Random(10);
			n->goy			= ty * TILE_SIZE + 16 -5 + Random(10);
			n->WalkTime			= g_ClientTime; // ::timeGetTime();
			n->MoveType			= 0;
	
	

			return 1;
		}
	

		return 0;

	}		
	
	

	return 0;
}			*/
	
/*	
int NPC_MakePath( CHARACTER *n, int tx, int ty, int how = 0 )
{
	return NPC_MakePathBumn( n, tx, ty, how );
}
*/



int NPC_MakePathBumn( CHARACTER *n, int tx, int ty, int how = 0 )
{
	CHARACTER t;

	// 010515 KHS
	if( n->eventnpc ) return 0;

	if( CheckDestinationOk( tx * TILE_SIZE, ty * TILE_SIZE ) == 0 ) return 0;
			
	memcpy( &t, n , sizeof( CHARACTER ) );
	t.moveFlag = TRUE;
	t.movetype = 0;
	t.gox = tx * TILE_SIZE + 16;
	t.goy = ty * TILE_SIZE + 16;
	t.destx = tx, t.desty = ty;

	n->MoveP = 0;

	if ( PathBuild( &t ) )
	{	
			t.moveFlag = FALSE;
			t.movetype += 100;
			SendMoveData( &t );	
			t.movetype -= 100;
			
			//n->sendmove = 1;

			return 1;
	}		

	//n->sendmove = 0;
	return 0;

	
}			


int NPC_MakePathGuard( CHARACTER *n, int tx, int ty, int how )
{
	CHARACTER t;

	// 010515 KHS
	if( n->eventnpc ) return 0;

	if( CheckDestinationOk( tx * TILE_SIZE, ty * TILE_SIZE ) == 0 ) return 0;
			
	memcpy( &t, n , sizeof( CHARACTER ) );
	t.moveFlag = TRUE;
	t.movetype = 0;
	t.gox = tx * TILE_SIZE + 16;
	t.goy = ty * TILE_SIZE + 16;
	t.destx = tx, t.desty = ty;

	t.MoveP = 0;

	if ( PathBuild( &t ) )
	{	
			t.moveFlag = FALSE;
			t.movetype += 100;
			SendMoveData( &t );	
			t.movetype -= 100;

			return 1;
	}		

	return 0;
}			


int NPC_MakePathMaul( CHARACTER *n, int tx, int ty, int how )
{
	CHARACTER t;
					
	// 010515 KHS
	if( n->eventnpc ) return 0;

	if( CheckDestinationOk( tx * TILE_SIZE, ty * TILE_SIZE ) == 0 ) return 0;
			
	memcpy( &t, n , sizeof( CHARACTER ) );
	t.moveFlag = TRUE;
	t.movetype = 0;
	t.gox = tx * TILE_SIZE + 16;
	t.goy = ty * TILE_SIZE + 16;
	t.destx = tx, t.desty = ty;

	t.MoveP = 0;

	if ( PathBuild( &t ) )
	{		
		t.moveFlag = FALSE;
		t.movetype += 100;

		SendMoveData( &t );	
		t.movetype -= 100;
		
		return 1;
	}		

	return 0;
}			



/*

int NPC_MakePathBumn( CHARACTER *n, int tx, int ty, int how )
{
	int dir;
	int oldx, oldy;
	int num;
	int i;

	n->tx = tx;
	n->ty = ty;
	n->how = how;

	if( n->pathcount < 2 )
	{
		int x = n->x / TILE_SIZE;
		int y = n->y / TILE_SIZE;

		dir  = NPC_GetDir( x, y, tx, ty );

		for( i = 0 ; i < how ; i ++)
		{
			oldx = x; oldy = y;
			
			switch( dir )
			{
			case 0 :	   y ++; break;
			case 1 : x --; y ++; break;
			case 2 : x --;       break;
			case 3 : x --; y --; break;
			case 4 :       y --; break;
			case 5 : x ++; y --; break;
			case 6 : x ++;       break;
			case 7 : x ++; y ++; break;
			}
			
			if( FreeTile( n->x / TILE_SIZE, n->y / TILE_SIZE, x, y ) )
			{
				if (  i%2 == 0){	n->MoveDirTbl[ (i/2) ] = dir << 4;	}
				else			{	n->MoveDirTbl[ (i/2) ] += dir;		}
			}
			else 
			{
				x = oldx;
				y = oldy;
				break;
			}
		}	
/		n->MoveLength		= i;
		n->pathcount	= 0;
		n->MoveGox			= x * TILE_SIZE + 16;
		n->goy			= y * TILE_SIZE + 16;
		n->WalkTime			= g_ClientTime; // ::timeGetTime();
		n->MoveType			= 0;

		printf( "NPC Move... \n");
	}		
			
	return i;
}			
*/			
	
/*
	Added by Tya @ 12 Mar 2006
extern allows the client to be compiled with optimizations on. If not a linker error will be thrown.
*/		
extern inline bool NPC_IsMoving(CHARACTER* n)
{	//< CSD-040107
	return (n->desttime > g_ClientTime) ? true:false;
}	//> CSD-040107
			
int NPC_Hostile( CHARACTER *n )
{			
	return n->peacests;
}			
				
			
int NPC_StillAttack( CHARACTER *n)
{			
	if( n->attackcount > 6 +Random(5) )
	{		
		n->attackcount = 0;
		return 0;
	}		
			
	return 1;
}			
			
				
int NPC_IsAttackableRange( CHARACTER *n )
{			
	CHARACTER *ch;
	int t;	
			
	int x;	
	int y;	
	int tx;	
	int ty;	
			
	
	if( n->targetid < 0 ) 	goto FAIL_;
	
	ch = ReturnCharacterPoint( n->targetid );
	if( ch == NULL ) goto FAIL_;
	
	if(	IsDead( ch ) ) goto FAIL_;
	
	x = n->x;
	y = n->y;
	tx = ch->x;
	ty = ch->y;
	t = (tx-x)*(tx-x) + (ty-y)*(ty-y);
	if( t < NPC_GetAttackRange(n)  )  // ### 몬스터의 사정거리 필요..
	{		
		// ###만약 사이에 어떤게 끼어 있으면 return 0;
	
		return 1;
	}	
	
FAIL_:
	
	return 0;
}		
		
int NPC_IsInBossRange( CHARACTER *n )
{		
	
	if( n->bossid == -1 ) return 0;
		
	LPCHARACTER boss = ReturnCharacterPoint( n->bossid );
	if( boss == NULL ) return 0;


	if( !IsDead( boss ) )
	{
		if( BoxAndDotCrash( boss->x / TILE_SIZE-4, boss->y / TILE_SIZE - 4, 8, 8, n->x / TILE_SIZE, n->y / TILE_SIZE ))
		{
			return 1;
		}
	}	
		
	return 0;
}	
	
int NPC_IsInHostRange( CHARACTER *n )
{		
	if( n->HostId == -1 ) return 0;

	CHARACTER *host = ReturnCharacterPoint( n->HostId );
	if( host == NULL ) return 0;

	if( strcmp( host->name, n->HostName ) ) return 0; // ID와 이름이 달라......
	
	if( !IsDead( host ) )
	{	
		if( BoxAndDotCrash( host->x / TILE_SIZE-4, host->y / TILE_SIZE - 4, 8, 8, n->x / TILE_SIZE, n->y / TILE_SIZE ))
		{
			return 1;
		}
	}	
		
	return 0;
}


// 현재 경비병이 사용하며 pk한 놈을 찾는다. 
//
int NPC_IsWhoNearRedPC(CHARACTER *npc, int range)
{	//< CSD-040107
	LPCHARACTER tempch = NULL;
	
#ifdef _NATION_APPLY_
	const int nation = MapInfo[MapNumber].nation;
	// 먼저 다른 나라놈이 있으면 그놈을 공격한다.
	if (MapNumber != 30)
	{
		if (npc->nRace == GUARD)
		{
			tempch = Hero;

			while (tempch)
			{
				if (!IsDead(tempch) && tempch->lv > 7 && !tempch->IsCounselor())
				{
					if (tempch->sprno == 1 || tempch->sprno == 0)
					{
						if (IsNK(tempch, nation) >= 5 || IsEnemyNation(tempch->name_status.nation, nation))
						{
							return tempch->id;
						}
					}
				}
				
				tempch = tempch->lpNext;
			}
		}
	}
#endif
	tempch = Hero;

	while (tempch)
	{
		if (!IsDead(tempch))
		{
			if (IsNK(tempch) >= 5 || tempch->dummy_pk > 10)
			{
				return tempch->id;
			}
		}
		
		tempch = tempch->lpNext;
	}
	
	return -1;
}	//> CSD-040107


// NPC를 보는 PC가 있는가?
int NPC_IsTherePC( CHARACTER *npc, int range )
{
	LPCHARACTER tempch = Hero;
	while( tempch )
	{
		if( !IsDead( tempch )) 
		if( tempch->type != SPRITETYPE_NPC && tempch->type != SPRITETYPE_MONSTER )
		{
			return tempch->id;
		}

		tempch = tempch->lpNext;
	}
	return -1;
}




int NPC_IsWhoNearOtherNationPc( int npc_nation, LPCHARACTER npc, int range )
{
	int x = npc->x;		
	int y = npc->y;		
	int tx, ty;			
	DWORD tmin, min = 0xFFFFFFFF;
	int	findch = -1;	
	int	sx, ex;			
	int sy, ey;			
						
	LPCHARACTER tempch;	
						
	int mx  = x / TILE_SIZE;
	int my  = y / TILE_SIZE;
						
	sx = mx - range;	if( sx < 0 ) sx = 0;
	ex = mx + range;    if( ex >= g_Map.file.wWidth )  ex = g_Map.file.wWidth-1;
	sy = my - range;	if( sy < 0 ) sy = 0;
	ey = my + range;    if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight -1;
						
	tempch = Hero;		
	while( tempch )		
	{					
		if( !IsDead( tempch )) 
		{				
			switch( tempch->sprno )
			{			
			case 0 :	
			case 1 :	
//			case 18 :	
//			case 19 :	
//			case 20 :	
						if( npc_nation == tempch->name_status.nation ) break;
						tx = tempch->x;
						ty = tempch->y;
						tmin = (tx-x)*(tx-x) + (ty-y)*(ty-y);
						if( tmin < min ) 
						{
							min = tmin;
							findch = tempch->id;
						}
					break;
			}			
		}				
		tempch = tempch->lpNext;
	}					
						
	return findch;		
}

int NPC_IsWhoNearPC( CHARACTER *npc, int range  )
{							
	int x = npc->x;		
	int y = npc->y;		
	int tx, ty;			
	DWORD tmin, min = 0xFFFFFFFF;
	int	findch = -1;	
	int	sx, ex;			
	int sy, ey;			
						
	LPCHARACTER tempch;	
						
	int mx  = x / TILE_SIZE;
	int my  = y / TILE_SIZE;
						
	sx = mx - range;	if( sx < 0 ) sx = 0;
	ex = mx + range;    if( ex >= g_Map.file.wWidth )  ex = g_Map.file.wWidth-1;
	sy = my - range;	if( sy < 0 ) sy = 0;
	ey = my + range;    if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight -1;
						
	tempch = Hero;		
	while( tempch )		
	{	
		if (!IsDead(tempch) && !(tempch->condition&CON_STONE) && !(tempch->viewtype == VIEWTYPE_OBSERVE_))
		{	//< CSD-TW-030606
			switch( tempch->sprno )
			{			
			case 0:	
			case 1:	
				{
					tx = tempch->x;
					ty = tempch->y;
					tmin = (tx-x)*(tx-x) + (ty-y)*(ty-y);
					
					if( tmin < min ) 
					{
						min = tmin;
						findch = tempch->id;
					}

					break;
				}
			}			
		}	//> CSD-TW-030606

		tempch = tempch->lpNext;
	}					
						
	return findch;		
}						
							
int NPC_WhoIsAttackMyBaby( CHARACTER *n )
{							
	LPCHARACTER tempch = Hero;
	while( tempch )		
	{					
		if( !IsDead( tempch ) && n != tempch && tempch->type == SPRITETYPE_MONSTER )
		{					
			if( tempch->bossid == n->id )
			{			
				if( tempch->attacked != -1 )
				{		
					tempch->attacked = -1;
					return tempch->id;
				}		
			}			
		}				
		tempch = tempch->lpNext;
	}					
							
	return -1;
}			
			
			
			
			
			
int NPC_WhoIsBoss( CHARACTER *n )
{					
	DWORD	maxexp = 0;	
	int		x = n->x, y = n->y;
	int		bossid = -1;
	int		ex, ey;
	DWORD	dis;
	int		c = 0;
				
//	n->bossid = -1;	
				
	LPCHARACTER tempch = Hero;
	while( tempch )
	{			
		if( !IsDead( tempch ) && tempch->type == SPRITETYPE_MONSTER )
		{		
			if( n->sprno == tempch->sprno )
			{	
				ex = tempch->x;	ey = tempch->y;
				dis = (ex-x)*(ex-x)+(ey-y)*(ey-y);
				if( dis < (DWORD)(TILE_SIZE * 10)*(DWORD)(TILE_SIZE * 10) )
				{	
					c++;
					if( tempch->Exp > maxexp )
					{
						maxexp		= tempch->Exp;
						bossid		= tempch->id;
					}
				}	
			}	
		}		
		tempch = tempch->lpNext;
	}			

	if( c == 1 ) bossid = -1;	// 나밖에 Checkking이 되지 않았다는 얘기 ............
	return bossid;
}




int NPC_WhoIsBlocking( int sx, int sy )
{
	LPCHARACTER tempch = Hero;
	while( tempch )
	{
		if( !IsDead( tempch ) )
		{
			if( tempch->x / TILE_SIZE && tempch->y/ TILE_SIZE )
			{
				return tempch->id;
			}
		}

		tempch = tempch->lpNext;
	}
	return -1;
}

CHARACTER *NPC_ReturnCharListPoint( int id )
{
	if( id < 0 ) return NULL;

	LPCHARACTER tempch = ReturnCharacterPoint( id );
	if( tempch == NULL ) return NULL;

	if( IsDead( tempch) ) return NULL;
		
	return tempch;
}
		
CHARACTER *ReturnCharListPoint( char *name )
{		
	if( name == NULL ) return NULL;

	LPCHARACTER tempch = Hero;
	while( tempch )
	{	
		if( strcmp( tempch->name, name ) == 0 && tempch->type != SPRITETYPE_MONSTER )
		{
			return tempch;
		}

		tempch = tempch->lpNext;
	}
	
	return NULL;
}
		
		
		
		
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		User Functions.....
//


void NPC_Pattern_28( LPCHARACTER n)
{			
	int ex, ey;
			
	switch( n->patterntype )
	{		
	case NPC_PATTERN_WANDER_28_ :

			int bossid;
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%1000);
			}
			else break;

				// 그냥 배회한다. 
			if( NPC_IsMoving( n ) )
			{
			}
			else 
			{
				int targetid = NPC_IsWhoNearPC( n, 10 );
				if( targetid != -1 )
				{
					if( n->bossid == -1 )
					{
						n->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
						n->targetid = targetid;
						break;
					}		
					else	
					{		
						LPCHARACTER tempch = Hero;
						while( tempch )
						{	
							if( tempch->patterntype != NPC_PATTERN_TAME_ )
							if( !IsDead( tempch ) && tempch->type == SPRITETYPE_MONSTER ) 
							{
								if( tempch->bossid == n->bossid )
								{
									tempch->targetid	= n->targetid;
									tempch->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
								}
							}
							tempch = tempch->lpNext;
						}	
						break;
					}			
				}			
				else		
				{			
					bossid = NPC_WhoIsBoss( n );
					if( bossid != -1 )	// Yes.. 보스를 찾았어!!
					{		
						if( bossid != n->id )	// 음. 나보다 Exp가 높은 놈이 있군.. 
						{	
							n->bossid = bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid );
							n->patterntype = NPC_PATTERN_TOBOSS_28_;
							n->targetid = -1;
							break;
						}	
						else  // 내가 Boss이다. 
						{	
							n->bossid = bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid );
							n->patterntype = NPC_PATTERN_IAM_BOSS_28_;
							n->targetid = -1;
							break;
						}	
					}		
					else	// 잉. 보스가 없어...
					{		
						int dir;
						int how;
						int tx, ty;
							
						dir = Random(8);
						how = Random(8)+2;
						tx = n->x / TILE_SIZE;
						ty = n->y / TILE_SIZE;
						switch( dir )
						{
							case 0 :			ty +=how; break;
							case 1 : tx -=how;	ty +=how; break;
							case 2 : tx -=how;            break;
							case 3 : tx -=how;	ty -=how; break;
							case 4 :			ty -=how; break;
							case 5 : tx +=how;	ty -=how; break;
							case 6 : tx +=how;		      break;
							case 7 : tx +=how;	ty +=how; break;
						}
						NPC_MakePathBumn( n, tx, ty, how );
					}
				}
			}
			
		break;

	case NPC_PATTERN_IAM_BOSS_28_ :
		{
			int bossid;

			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%1000);
				if( NPC_IsMoving( n ) )
				{


				}
				else 
				{
					bossid = NPC_WhoIsBoss( n );
					if( bossid == -1 )	 // 나혼자군..
					{	
						n->patterntype	= NPC_PATTERN_WANDER_28_;

						n->targetid		= -1;
						n->bossid		= -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID,   -1);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1);
						break;
					}
					else if( bossid != n->id )	// dld음. 나보다 Exp가 높은 놈이 있군.. 
					{
							n->bossid		= bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid );
							n->patterntype	= NPC_PATTERN_TOBOSS_28_;

							break;
					}
					else  // 역쉬 내가 Boss다 !  그럼 보스의 일을 해야지.
					{
						n->bossid = bossid;


						int attacker = NPC_WhoIsAttackMyBaby( n );
						if( attacker != -1) // 내 아가들중 맞는애가 있으면..
						{
							LPCHARACTER tempch = Hero;
							while( tempch )
							{	
								if( !IsDead( tempch ) && tempch->type == SPRITETYPE_MONSTER && tempch != n ) 
								{
									if( tempch->bossid == n->bossid )
									{
										tempch->targetid	= n->targetid;
										SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID,n->targetid );
										tempch->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
									}
								}
								tempch = tempch->lpNext;
							}	
							break;
						}
						else
						{
							n->targetid = NPC_IsWhoNearPC( n, 6 );
							LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
							if (tempch) // finito null check
							{
								if( n->targetid == -1 )
								{
									if( (Random(15)) == 0 )
									{// 보스지만 약간씩 문직인다. 
										int dir;
										int how;
										int tx, ty;

										dir = Random(8);
										how = Random(2)+2;
										tx = n->x / TILE_SIZE;
										ty = n->y / TILE_SIZE;
										switch( dir )
										{
											case 0 :			ty +=how; break;
											case 1 : tx -=how;	ty +=how; break;
											case 2 : tx -=how;            break;
											case 3 : tx -=how;	ty -=how; break;
											case 4 :			ty -=how; break;
											case 5 : tx +=how;	ty -=how; break;
											case 6 : tx +=how;		      break;
											case 7 : tx +=how;	ty +=how; break;
										}
		
										//n->MoveP = Random(50) + 50;
										NPC_MakePathBumn( n, tx, ty, how );
									}

								}
								else
								{
									if( IsDead( tempch ) )
									{
										n->targetid = -1;
									}
									else
									{
										if( InDistance( n, tempch,  TILE_SIZE * 7 ) )	// 그 PC가  5 타일안에 있다면.
										{	
											LPCHARACTER tempch = Hero;
											while( tempch )
											{	
												if( !IsDead( tempch ) && tempch->type == SPRITETYPE_MONSTER && tempch != n ) 
												{
													if( tempch->bossid == n->bossid )
													{
														tempch->targetid	= n->targetid;
														SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
														tempch->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
													}
												}
												tempch = tempch->lpNext;
											}	
										}
									}
								}
							}
						}
					}
				}
			}
		}
		break;

	case NPC_PATTERN_TOBOSS_28_ :
		{
			int ex, ey;
			int bossid;

			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 5000  + (rand()%2000);

				if( NPC_IsMoving( n ) )
				{

				}
				else 
				{
					int targetid = NPC_IsWhoNearPC( n, 10 );
					if( targetid != -1 )
					{
						n->patterntype = NPC_PATTERN_BACKDRAW_PC_28_;
						n->targetid = targetid;
						break;
					}

					if( NPC_IsInBossRange( n ) )	// Boss안에 들어왔으면..
					{
						bossid = NPC_WhoIsBoss( n );
						if( bossid != -1 )	// 혼자라는 얘기.
						{	
							if( bossid != n->id )	// 음. 나보다 Exp가 높은 놈이 있군.. 
							{
								n->bossid = bossid;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
								n->patterntype = NPC_PATTERN_TOBOSS_28_;

								goto TO_BOSS_;
							}
							else  // 내가 Boss이다. 
							{
								n->bossid = bossid;
								n->patterntype = NPC_PATTERN_IAM_BOSS_28_;

								n->targetid = -1;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
								SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );
							}
						}
						else 
						{
							n->patterntype = NPC_PATTERN_WANDER_28_;

							n->targetid = -1;
							n->bossid	= -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, -1 );
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );

							break;
						}
					}
					else //  Boss곁으로 가야지...
					{
TO_BOSS_:				if( n->bossid != -1 )
						{
							LPCHARACTER tempch = ReturnCharacterPoint( n->bossid );
							if (tempch) // finito null check
							{
								if( NPC_NearPosition( n, tempch, &ex, &ey ))
								{
									NPC_MakePath( n, ex, ey, Random(6));
									break;
								}
							}
						}
						else
						{
							n->patterntype = NPC_PATTERN_MURI_28_;
							n->targetid = -1;
							break;
						}
					}
				}
			}
		}
		break;
			
		//	무리지어다닌다... 28
		case NPC_PATTERN_MURI_28_ :
		{	
			int bossid = -1;
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 2000  + (rand()%1000);

				if( NPC_IsMoving( n ) )
				{
				}
				else
				{
					bossid = NPC_WhoIsBoss( n );
					if( bossid != -1 )	// 혼자라는 얘기.
					{	
						if( bossid != n->id )	// 음. 나보다 Exp가 높은 놈이 있군.. 
						{
							n->bossid = bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
							n->patterntype = NPC_PATTERN_TOBOSS_28_;
						}
						else  // 내가 Boss이다. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_IAM_BOSS_28_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );

						}
					}
					else 
					{
						n->patterntype = NPC_PATTERN_WANDER_28_;
						n->bossid = -1;
						n->targetid = -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, -1);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1);

					}
				}
			}	
		}		
		break;	
				
								
		case NPC_PATTERN_BACKDRAW_PC_28_ :
			{					
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%400);
								
					if( NPC_IsMoving( n ) )
					{			
								
					}			
					else		
					{			
						if( n->targetid != -1 )
						{		

							LPCHARACTER tempch  = ReturnCharacterPoint( n->targetid );
							if (tempch) // finito null check
							{
								if( InDistance( n, tempch,  TILE_SIZE * 10 ) )	// 그 PC가  10 타일안에 있다면.
								{	
									if( NPC_NearBackCh( n, tempch, &ex, &ey, 10 ) )
									{	
										n->MoveP = 140 + Random(120);
										if( NPC_MakePath( n, ex, ey, Random(10)+5 ) )
										{
										}
										else 
										{
											n->targetid = -1;
											n->patterntype = NPC_PATTERN_WANDER_28_;
										}
									}	
									else 
									{
									}
								}
								else	// 충분히 멀리있군.
								{
									n->patterntype = NPC_PATTERN_WANDER_28_;
								}
							}
						}		
						else	
						{		
							n->patterntype = NPC_PATTERN_WANDER_28_;
						}		
					}				
				}				
				break;			
			}				
		}

}


void NPC_Pattern_38( LPCHARACTER n)
{
	int ex, ey;

	switch( n->patterntype )
	{
	case NPC_PATTERN_WANDER_38_ :

			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 2000  + (rand()%500);
			}
			else break;

				// 그냥 배회한다. 
			if( NPC_IsMoving( n ) )
			{
			}
			else 
			{
				int targetid = NPC_IsWhoNearPC( n, 15 );
				if( targetid != -1 )
				{
					n->patterntype = NPC_PATTERN_ACCESS_PC_38_;
					n->targetid = targetid;
				}
				else 
				{
						int dir;
						int how;
						int tx, ty;

						dir = Random(8);
						how = Random(8)+2;
						tx = n->x / TILE_SIZE;
						ty = n->y / TILE_SIZE;
						switch( dir )
						{
							case 0 :			ty +=how; break;
							case 1 : tx -=how;	ty +=how; break;
							case 2 : tx -=how;            break;
							case 3 : tx -=how;	ty -=how; break;
							case 4 :			ty -=how; break;
							case 5 : tx +=how;	ty -=how; break;
							case 6 : tx +=how;		      break;
							case 7 : tx +=how;	ty +=how; break;
						}

						NPC_MakePathBumn( n, tx, ty, how );

				}
			}
			
		break;

		case NPC_PATTERN_ACCESS_PC_38_ :
			{	
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
					n->aitimedelay = g_ClientTime;
					
					n->aidelayhowmuch = PCFINDACCESS(n);  // 0813 NPC KHS

					if( NPC_IsMoving( n ) )
					{
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if( tempch )
						{
							if( !IsDead( tempch ) )
							{
								if( NPC_IsAttackableRange( n ) )
								{
									SendModifyPositionNPC( n->id ); 
									SendNPCAttack( n->id );
									n->attackcount ++;
									n->aitimedelay = g_ClientTime;		//  이게 없으니까.. 연타가 나온다. 
									n->aidelayhowmuch = ATTACKACCESS(n)  + (rand()%300);
									n->patterntype = NPC_PATTERN_ATTACK_PC_38_;
									break;
								}
							}
						}
					}
					else
					{
//						if( NPC_Hostile( n ) ) // 공격모드이면...
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if( tempch )
						{
							if( !IsDead( tempch ) )
							{
								if( NPC_IsAttackableRange( n ) )// 공격 가능한 거리까지 왔나 ?
								{
									SendNPCAttack( n->id );
									n->attackcount ++;
									n->aitimedelay = g_ClientTime;		//  이게 없으니까.. 연타가 나온다. 
									n->aidelayhowmuch = ATTACKACCESS(n)  + (rand()%600);
									n->patterntype = NPC_PATTERN_ATTACK_PC_38_;

									break;
								}
								else	// 공격가능한 거리가 아니면 근처까지 간다. 
								{
									if( NPC_NearCh( n, tempch, &ex, &ey ) )
									{
										n->MoveP = 60 + Random(30 );
										if( NPC_MakePath( n, ex, ey, Random(6) + 2 ) )
										{
											n->patterntype = NPC_PATTERN_ATTACK_PC_38_;
										}
										else
										{
											n->patterntype = NPC_PATTERN_WANDER_38_;
										}
									}
									else
									{
										n->patterntype = NPC_PATTERN_WANDER_38_;

	//									n->bossid = -1;
	//									n->server_id = -1;
									}
								}
							}	
							else 
							{
								n->patterntype = NPC_PATTERN_WANDER_38_;
								n->targetid = -1;
								break;
							}
						}
					}	
				}		
			}			
			break;		
						
		case NPC_PATTERN_ATTACK_PC_38_ :
			{		
				LPCHARACTER tempch;
				if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
				{		
					n->aitimedelay = g_ClientTime;
					n->aidelayhowmuch =  ATTACKACCESS(n)  + (rand()%600);
				}	
				else break;
					
					
				if( NPC_IsMoving(n) )	//  현재 이동중이면 
				{	
					LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
					if( tempch )
					{
						if( !IsDead( tempch ) )
						if( NPC_IsAttackableRange( n ) )	// 이동 도중 공격대상캐릭를 공격할수 있다면 공격한다. 
						{
							goto ATTACK_38_NEXT__;
						}
					}
					break;
				}	
					
				tempch = ReturnCharacterPoint( n->targetid );
				if (tempch) // finito null check
				{
					if( NPC_IsAttackableRange( n ) )// 공격 가능한 거리까지 왔나 ?
					{					
ATTACK_38_NEXT__:	
						if( NPC_StillAttack(n) )
						{											
							SendNPCAttack(  n->id );
							n->attackcount ++;
							//n->patterntype = NPC_PATTERN_WANDER_38_;	
						}								
						else							
						{											
							if( NPC_NearBackCh( n, tempch, &ex, &ey, 5 ) )
							{													
								n->MoveP = 70 + Random(30 );
								if( NPC_MakePath( n, ex, ey, Random(2)+2 ) ) 
								{
									n->patterntype =NPC_PATTERN_ACCESS_PC_38_;		//WANDER_8_;
								}
								else
								{
									n->targetid = -1;
									n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
								}
							}									
							else						
							{							
								n->patterntype =NPC_PATTERN_ACCESS_PC_38_;		//WANDER_8_;
							}		
						}			
					}				
					else			
					{				
						if( n->targetid != -1 )
						{			
							if( !IsDead( tempch ) )
							{		
								if( tempch == NULL ) 
								{
									n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
								}
								else if( NPC_NearCh( n, tempch, &ex, &ey ) )
								{		
									//if( NPC_EnemyOnMyWay( 		
									// 니독네 공격상대에게 접근하기 위한 루틴을 넣는다. ( 'ㄱ' 'ㄴ'으로 
									n->MoveP = 60 + Random(30 );
									if( NPC_MakePath( n, ex, ey, Random(6)+2 ) ) 
									{
										n->patterntype =NPC_PATTERN_ACCESS_PC_38_;		//WANDER_8_;
									}
									else
									{
										n->targetid = -1;
										n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
									}	
								}			
								else		
								{	
									n->targetid = -1;
									n->patterntype = NPC_PATTERN_WANDER_38_;
	//								n->bossid	 = -1;
	//								n->server_id = -1;
	//								JustMessage( " %d의 Pattern번호 %d ", n->server_id, n->patterntype );
	//								JustMessage( " %d의 Pattern번호 %d ", n->patterntype );
								}				
							}
							else 
							{
								n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
							}
						}		
						else
						{
							n->patterntype =NPC_PATTERN_WANDER_38_;		//WANDER_8_;
						}
					}
				}				
			break;					
			}
		
		case NPC_PATTERN_BACKDRAW_PC_38_ :
			{					
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 500  + (rand()%500);

				LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
				if (tempch) // finito null check
				{
					if( NPC_IsMoving( n ) )
					{			
								
					}			
					else		
					{			
						if( n->targetid != -1 )
						{		
							if( InDistance( n, tempch,  TILE_SIZE * 10 ) )	// 그 PC가  10 타일안에 있다면.
							{	
								n->patterntype = NPC_PATTERN_WANDER_38_;
								if( NPC_NearBackCh( n, tempch, &ex, &ey, 10 ) )
								{	
									n->MoveP = 60 + Random(30 );
									NPC_MakePath( n, ex, ey, Random(10)+5 );
								}	
								else 
								{
								
								}
							}
							else	// 충분히 멀리있군.
							{
								n->patterntype = NPC_PATTERN_WANDER_38_;

							}
						}
						else
						{
							n->patterntype = NPC_PATTERN_WANDER_38_;
						}
					}
				}
			}
				break;			
			}
	}
}


void Check_NPC_Pattern( LPCHARACTER  n )
{
	if( n->type != SPRITETYPE_MONSTER ) return;

	if( IsDead( n ) ) return;

	switch( n->patterntype )
	{
//------------------------------------------------------------------------------------------------------------
	case NPC_PATTERN_WANDER_8_			:
	case NPC_PATTERN_IAM_BOSS_8_		:
	case NPC_PATTERN_TOBOSS_8_			:
	case NPC_PATTERN_MURI_8_			:
	case NPC_PATTERN_ACCESS_PC_8_		:
	case NPC_PATTERN_ATTACK_PC_8_		:
	case NPC_PATTERN_BACKDRAW_8_		:	NPC_Pattern_8(n);
		break;

	case NPC_PATTERN_IAM_BOSS_28_		:
	case NPC_PATTERN_TOBOSS_28_			:
	case NPC_PATTERN_WANDER_28_			:
	case NPC_PATTERN_MURI_28_			:
	case NPC_PATTERN_BACKDRAW_PC_28_	:	NPC_Pattern_28(n);
		break;

	case  NPC_PATTERN_ACCESS_PC_38_		:
	case  NPC_PATTERN_ATTACK_PC_38_		:
	case  NPC_PATTERN_WANDER_38_		:
	case  NPC_PATTERN_MURI_38_			:
	case  NPC_PATTERN_BACKDRAW_PC_38_	:	NPC_Pattern_38(n);
		break;

	case  NPC_PATTERN_IAM_BOSS_GUARD_	:
	case  NPC_PATTERN_TOBOSS_GUARD_		:
	case  NPC_PATTERN_WANDER_GUARD_		:
	case  NPC_PATTERN_MURI_GUARD_		:
	case  NPC_PATTERN_ACCESS_PC_GUARD_	:
	case  NPC_PATTERN_ATTACK_PC_GUARD_	:
	case  NPC_PATTERN_BACKDRAW_PC_GUARD_:	NPC_Pattern_GUARD(n);
		break;

	case NPC_PATTERN_TAME_				:	NPC_Pattern_Tammed(n);
		break;

	case NPC_PATTERN_EVENTING_MAUL_		:
	case NPC_PATTERN_WANDER_MAUL_		:	NPC_Pattern_MAUL(n);
		break;

	case NPC_PATTERN_SEALSTONE_NO_OPERATE_ : 
	case NPC_PATTERN_SEALSTONE_ATTACK_ 	:	NPC_Pattern_SealStone( n );
		break;
	}
}


void RecvNK( int id, int nk3, int nk4, int nk6 )
{
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;
	ch->nk3 = nk3;
	ch->nk4 = nk4;
	ch->nk6 = nk6;
	ch->dummy_pk = 0;
}


void RecvServerKillPc(  int id, int killpc )
{	
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;
	ch->killpc = killpc;
}				
								
//  기능 NPC을 PK할때 캐릭으이 dummy_pk룰 적용한다. 
void CheckNPCPk( int aid, int did )
{				
	LPCHARACTER dch = ReturnCharacterPoint( did );	
	if( dch == NULL ) return;

	switch (dch->nRace)
	{	//< CSD-TW-030606
	case GUARD:
		{
			LPCHARACTER ach = ReturnCharacterPoint(aid);
			if (ach == NULL) return; // finito null check
			
			if (ach && ach->id < 10000)
			{
				if (IsNK( ach, MapInfo[MapNumber].nation ) < 5)
				{
					if (ach->name_status.nation == MapInfo[MapNumber].nation)
					{
						++ach->dummy_pk;
						char* temp = NULL;
						
						switch (dch->id%10)
						{
							case 0:
							{
								if( ach->dummy_pk	   == rand()%3  )		{  temp =  lan->OutputMessage(4,41); }//lsw
								else if( ach->dummy_pk == rand()%3 +1 )		{  temp =  lan->OutputMessage(4,42); }
								else if( ach->dummy_pk == rand()%3 + 2 )	{  temp =  lan->OutputMessage(4,43); }
								else if( ach->dummy_pk == rand()%3 + 3  )	{  temp =  lan->OutputMessage(4,44); }
								else if( ach->dummy_pk == rand()%3 + 4  )	{  temp =  lan->OutputMessage(4,45); }
								else if( ach->dummy_pk == rand()%3 + 5  )	{  temp =  lan->OutputMessage(4,46) ; }
								else if( ach->dummy_pk == rand()%3 + 6  )	{  temp =  lan->OutputMessage(4,47); ach->dummy_pk += 10; }
								else if( ach->dummy_pk == rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,48);  ach->dummy_pk += 10; }
								else if( ach->dummy_pk >= rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,49);  ach->dummy_pk += 10; }//lsw
								break;
							}
							case 2:
							{
								if( ach->dummy_pk	   == rand()%3  )		{  temp =  lan->OutputMessage(4,51); }//lsw
								else if( ach->dummy_pk == rand()%3 +1 )		{  temp =  lan->OutputMessage(4,52); }
								else if( ach->dummy_pk == rand()%3 + 2 )	{  temp =  lan->OutputMessage(4,53); }
								else if( ach->dummy_pk == rand()%3 + 3  )	{  temp =  lan->OutputMessage(4,54); }
								else if( ach->dummy_pk == rand()%3 + 4  )	{  temp =  lan->OutputMessage(4,55); }
								else if( ach->dummy_pk == rand()%3 + 5  )	{  temp =  lan->OutputMessage(4,56); }
								else if( ach->dummy_pk == rand()%3 + 6  )	{  temp =  lan->OutputMessage(4,57); ach->dummy_pk += 10; }
								else if( ach->dummy_pk == rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,58) ;   ach->dummy_pk += 10; }
								else if( ach->dummy_pk >= rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,59);   ach->dummy_pk += 10; }
								break;
							}
							case 3:
							{
								if( ach->dummy_pk	   == rand()%3  )		{  temp =  lan->OutputMessage(4,61); }//lsw
								else if( ach->dummy_pk == rand()%3 +1 )		{  temp =  lan->OutputMessage(4,62); }
								else if( ach->dummy_pk == rand()%3 + 2 )	{  temp =  lan->OutputMessage(4,63); }
								else if( ach->dummy_pk == rand()%3 + 3  )	{  temp =  lan->OutputMessage(4,64); }
								else if( ach->dummy_pk == rand()%3 + 4  )	{  temp =  lan->OutputMessage(4,65); }
								else if( ach->dummy_pk == rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,66);  ach->dummy_pk += 10; }
								else if( ach->dummy_pk >= rand()%3 + 8  )	{  temp =  lan->OutputMessage(4,67);   ach->dummy_pk += 10; }//lsw
								break;
							}
						}
						// 0810 KHS
						if (temp && (rand()%25) == 0)
						{	
							SendNPCChatArea(dch->id, temp);
						}	
					}
					else // 서로 다른 나라일경우.. 경비는..
					{
						ach->dummy_pk += 2;

						if( (rand()%100) < 50 )
						{
							switch (MapInfo[MapNumber].nation)
							{
							case N_VYSEUS:
								{
									switch (rand()%5)
									{
										case 0:	SendNPCChatArea(dch->id, lan->OutputMessage(4,71) );	break;//lsw
										case 1:	SendNPCChatArea(dch->id, lan->OutputMessage(4,72) );	break;
									}

									break;
								}
							case N_ZYPERN:
								{
									switch (rand()%5)
									{
										case 0:	SendNPCChatArea(dch->id, lan->OutputMessage(4,73));	break;
										case 1:	SendNPCChatArea(dch->id, lan->OutputMessage(4,74));	break;//lsw
									}

									break;
								}
							}
						}
					}
				}
			}

			break;
		}
	}	//> CSD-TW-030606
}




// 0811 NPC KHS

void SaveNPCAcessTable( void )
{
	FILE *fp = Fopen("./char/011105.spr","wb" );
	if( fp )
	{
		fwrite( NPCAccessTable, sizeof( short ) * 400 * 4, 1, fp );
		fclose(fp);
	}

	FILE *wp = Fopen( "./char/011105.spr", "wt" );
	if( wp )
	{
		for( int i = 0 ; i < 400 ;  i ++)
		{	//< CSD-030419
			fprintf(fp, "%3d(%20s): %3d %3d %3d %3d \n", 
				    i, 
					GetNpcName(i),
				    NPCAccessTable[i][0], 
					NPCAccessTable[i][1], 
					NPCAccessTable[i][2], 
					NPCAccessTable[i][3]);
		}	//> CSD-030419

		fclose(wp );
	}
}

void LoadNPCAccessTable( void )
{
	FILE *fp = Fopen("./char/011105.spr","rb" );
	if( fp )
	{
		fread( NPCAccessTable, sizeof( short ) * 400 * 4, 1, fp );
		fclose(fp);
	}
}	
	
void LoadItemAccessTable(void)
{
	FILE* fp = Fopen("011105.spr", "rb");
	if (fp)
	{
		fread(NPCAccessTable, sizeof(short) * 400 * 4, 1, fp);
		fclose(fp);
	}
}


void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD( t_packet *p )
{	
	memcpy( &NPCAccessTable[  0][0] ,p->u.server_npc_access_table_reload.data, sizeof( short )* 400 );	
}
void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD2( t_packet *p )
{	
	memcpy( &NPCAccessTable[100][0] ,p->u.server_npc_access_table_reload.data, sizeof( short )* 400 );	
}
void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD3( t_packet *p )
{	
	memcpy( &NPCAccessTable[200][0] ,p->u.server_npc_access_table_reload.data, sizeof( short )* 400 );
}
void Recv_CMD_SERVER_NPC_ACCESS_TABLE_RELOAD4( t_packet *p )
{	
	memcpy( &NPCAccessTable[300][0] ,p->u.server_npc_access_table_reload.data, sizeof( short )* 400 );
	SaveNPCAcessTable();
}





static char monname_temp[ 17];
char *LoadMonNameTable( int namenumber )
{
	FILE *fp = Fopen( "./Data/Mon_Name.bin", "rb" );
	if( fp == NULL ) return "";

	fseek( fp, namenumber * 17, SEEK_SET );
	fread( monname_temp, 17, 1, fp );
	fclose(fp);
	return (((char*)monname_temp)?(char*)monname_temp:"");
}

	


















