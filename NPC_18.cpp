#include "StdAfx.h"

#include "Dragon.h"	
#include "Path.h"
#include "NetWork.h"
#include "Hong_Sub.h"
#include "CharDataTable.h"
#include "NPC_Pattern.h"
#include "Map.h"
#include "lineCommand.h"

extern CHARACTER *NPC_ReturnCharListPoint( int id );
extern void calcNewAbility(CHARACTER *) ;
extern int getMaxHungry(CHARACTER *chr) ;

void NPC_Pattern_8( CHARACTER *n )
{
	switch( n->patterntype )
	{
	case NPC_PATTERN_WANDER_8_ :
		{
			int bossid;
						
			int attacker = NPC_WhoIsAttackMyBaby( n );
				
			if( n->attacked != -1 ) 
			{
				n->patterntype = NPC_PATTERN_ATTACK_PC_8_;
				break;
			}
					
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 200  + (rand()%500);
				// �׳� ��ȸ�Ѵ�. 
				if( NPC_IsMoving( n ) )
				{
				}
				else 
				{
					bossid = NPC_WhoIsBoss( n );
					if( bossid != -1 )	// Yes.. ������ ã�Ҿ�!!
					{	
						if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_TOBOSS_8_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );

							break;
						}
						else  // ���� Boss�̴�. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_IAM_BOSS_8_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );

							break;
						}
					}
					else	// ��. ������ ����...
					{
						int dir;
						int how;
						int tx, ty;

						dir = Random(8);
						how = Random(4)+2;
						tx = n->x/TILE_SIZE;
						ty = n->y/TILE_SIZE;
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
		}
		break;



	case NPC_PATTERN_IAM_BOSS_8_ :
		{
			int bossid;


			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%500);
				if( NPC_IsMoving( n ) )
				{
				}
				else 
				{
					int attacker = NPC_WhoIsAttackMyBaby( n );
					if( attacker != -1) // �� �ư����� �´¾ְ� ������..
					{
						
						LPCHARACTER tch = Hero;
						while( tch )
						{
							if( tch != n && !IsDead( tch ) && tch->type == SPRITETYPE_MONSTER )
							{

								if( tch->bossid == n->bossid  )
								{
									tch->targetid			= attacker;
									tch->patterntype	= NPC_PATTERN_ATTACK_PC_8_;
								}
							}
							tch = tch->lpNext;
						}
					}
					else
					{
					}


					bossid = NPC_WhoIsBoss( n );
					if( bossid == -1 )	 // ��ȥ�ڱ�..
					{	
						n->patterntype	= NPC_PATTERN_WANDER_8_;

						n->targetid		= -1;
						n->bossid		= -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, -1 );
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, -1 );

						break;
					}
					else if( bossid != n->id )	// dld��. ������ Exp�� ���� ���� �ֱ�.. 
					{
							n->bossid		= bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, bossid);
							n->patterntype	= NPC_PATTERN_TOBOSS_8_;

							break;
					}
					else  // ���� ���� Boss�� !  �׷� ������ ���� �ؾ���.
					{
						n->bossid = bossid;
						n->targetid = NPC_IsWhoNearPC( n, 10 );

						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );


						if( n->targetid != -1 )
						{
							LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
							if (tch == NULL) break; // Finito null check

							if( InDistance( n, tch,  TILE_SIZE * 10 ) )	// �� PC��  5 Ÿ�Ͼȿ� �ִٸ�.
							{	
								LPCHARACTER tch = Hero;
								while( tch )
								{
									if( tch != n && !IsDead( n ) && tch->type == SPRITETYPE_MONSTER )
									{
										if( tch->bossid == n->bossid  )
										{
											tch->targetid    = n->targetid;
											tch->patterntype = NPC_PATTERN_ACCESS_PC_8_;
										}
									}
									tch = tch->lpNext;
								}
								break;
							}
							else if( InDistance( n, tch,  TILE_SIZE * 15 ) )	// �� PC��  10 Ÿ�Ͼȿ� �ִٸ�.
							{				
								LPCHARACTER tch = Hero;
								while( tch )
								{			
									if( tch != n && !IsDead( n ) && tch->type == SPRITETYPE_MONSTER )
									{		
										if( tch->bossid == n->id  )
										{	
											int ex, ey;
											ex = n->x/TILE_SIZE, ey = n->y/TILE_SIZE;
											//LPCHARACTER targetch = ReturnCharacterPoint( n->id );
											NPC_NearCh( n, tch, &ex, &ey );
											tch->targetid = n->targetid;
											tch->patterntype = NPC_PATTERN_TOBOSS_8_;
											n->MoveP = 60 + Random(80 );
											NPC_MakePath( n, ex, ey, Random(6) + 2 );
										}	
									}		
									tch = tch->lpNext;
								}			
							}				
							else
							{
								if( Random(5) == 0 )
								{// �������� �ణ�� �����δ�. 
									int dir;
									int how;
									int tx, ty;

									dir = Random(8);
									how = Random(2)+2;
									tx = n->x/TILE_SIZE;
									ty = n->y/TILE_SIZE;
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

								// ��.. �ƴ��͵� �����ʴ´�. 
							}
						}
						else	// �ٽ� �������Է� ���̶�� �Ѵ�. 
						{
						}
					}
				}
			}
		}
		break;

	case NPC_PATTERN_TOBOSS_8_ :
		{
			int ex, ey;
			int bossid;

			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1500  + (rand()%500);

				if( NPC_IsMoving( n ) )
				{

				}
				else 
				{
					if( NPC_IsInBossRange( n ) )	// Boss�ȿ� ��������..
					{
						bossid = NPC_WhoIsBoss( n );
						if( bossid != -1 )	
						{	
							if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
							{
								n->bossid = bossid;
								n->patterntype = NPC_PATTERN_TOBOSS_8_;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
		


								goto TO_BOSS_;
							}
							else  // ���� Boss�̴�. 
							{
								n->bossid = bossid;
								n->patterntype = NPC_PATTERN_IAM_BOSS_8_;
								n->targetid = -1;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
								SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

							}
						}
						else 
						{
							n->patterntype = NPC_PATTERN_WANDER_8_;

							n->targetid = -1;
							n->bossid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

							break;
						}
					}
					else //  Boss������ ������...
					{
TO_BOSS_:
						if( n->bossid != -1 )
						{
							LPCHARACTER tempch = ReturnCharacterPoint( n->bossid );
							if( tempch )
							{
								if( NPC_NearPosition( n, tempch, &ex, &ey ))
								{
									NPC_MakePath( n, ex, ey, Random(6) + 2 );
										break;
								}
							}
							else
							{
								n->patterntype = NPC_PATTERN_WANDER_8_;
								n->bossid = -1;
								n->targetid = -1;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
								SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
							}
						}
						else
						{

							int targetid = NPC_IsWhoNearPC( n, 10 );
							if( targetid != -1 )
							{
								if (n->bossid != -1 )
								{
									LPCHARACTER tch = Hero;

									while( tch )
									{
										if( !IsDead( tch ) && tch->type ==SPRITETYPE_MONSTER )
										{
											if( tch->bossid == n->bossid )
											{
												tch->targetid	= targetid;
												tch->patterntype= NPC_PATTERN_ATTACK_PC_8_;
											}
										}
									}
									tch = tch->lpNext;
								}
								else 
								{
									n->patterntype = NPC_PATTERN_WANDER_8_;
									n->bossid = -1;
									n->targetid = -1;
									SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
									SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

								}
							}
							else 
							{
									n->patterntype = NPC_PATTERN_WANDER_8_;
									n->bossid = -1;
									n->targetid = -1;
									SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
									SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
							}
						}
					}
				}
			}

		}
		break;
			
		//	��������ٴѴ�... 8
		case NPC_PATTERN_MURI_8_ :
		{	
			int bossid = -1;
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%500);

				if( NPC_IsMoving( n ) )
				{
				}
				else
				{
					bossid = NPC_WhoIsBoss( n ); // ȥ�ڶ�� ���.
					if( bossid != -1 )	
					{	
						if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
						{
							n->bossid = bossid;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
							n->patterntype = NPC_PATTERN_TOBOSS_8_;

						}
						else  // ���� Boss�̴�. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_IAM_BOSS_8_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

						}
					}
					else 
					{
						n->patterntype = NPC_PATTERN_WANDER_8_;

						n->bossid = -1;
						n->targetid = -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

					}
				}
			}	
		}		
		break;	
				
		case NPC_PATTERN_ACCESS_PC_8_ :
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
									n->patterntype = NPC_PATTERN_ATTACK_PC_8_;
									break;
								}
							}
						}
					}	
					else
					{	
						LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
						if( tch )
						{
							if( !IsDead( tch ) )
							{
								if( NPC_IsAttackableRange( n ) )// ���� ������ �Ÿ����� �Գ� ?
								{
									SendNPCAttack( n->id );
									n->attackcount ++;
									n->patterntype = NPC_PATTERN_ATTACK_PC_8_;
									break;
								}
								else	// ���ݰ����� �Ÿ��� �ƴϸ� ��ó���� ����. 
								{
									int ex,ey;
									LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
									if (tch)
									{
										if( NPC_NearCh( n, tch, &ex, &ey ) )
										{
											n->MoveP = 60 + Random(30 );
											NPC_MakePath( n, ex, ey, Random(6) + 2 );
										}
										else
										{
		//									n->patterntype = NPC_PATTERN_WANDER_8_;

		//									n->bossid = -1;
		//									n->id = -1;
										}
									}
								}
							}	
							else 
							{
								n->patterntype = NPC_PATTERN_TOBOSS_8_;

								n->bossid	= -1;
								n->targetid = -1;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
								SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

								break;
							}
						}
					}	
				}		
			}			
			break;		
						
		case NPC_PATTERN_ATTACK_PC_8_ :
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = ATTACKACCESS(n)  + (rand()%200);
				
				if( NPC_IsMoving(n))	
				{		
					LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
					if( tempch )
					{
						if( !IsDead( tempch ) )
						{
							if( NPC_IsAttackableRange( n ) )	// �̵� ���� ���ݴ��ĳ���� �����Ҽ� �ִٸ� �����Ѵ�. 
							{
								goto ATTACK_8_NEXT__;
							}
						}
					}
					break;					
				}						
										
				if( n->bossid != -1)								
				{											
					LPCHARACTER tch = ReturnCharacterPoint( n->bossid );
					if( tch )
					{
						if( !IsDead( tch ) )// Ȥ�� Boss�� ������....
						{												
							LPCHARACTER tch = Hero; // ReturnCharacterPoint( n->bossid );
							while( tch )
							{				
								if( tch->type == SPRITETYPE_MONSTER && !IsDead( tch ) )
								{
									if( tch->bossid == n->bossid )
									{
												tch->bossid   = -1;
												tch->targetid = -1;
												tch->patterntype = NPC_PATTERN_BACKDRAW_8_;
												SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
												SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

									}
								}
								tch = tch->lpNext;
							}
							break;								
						}
					}
				}							
												
												
				if( NPC_IsAttackableRange( n )  )// ���� ������ �Ÿ����� �Գ� ?
				{											
ATTACK_8_NEXT__:					
					if( NPC_StillAttack(n) )
					{											
						SendNPCAttack( n->id );
						n->attackcount ++;
					}								
					else							
					{			
						int ex, ey;
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if (tempch)
						{
							if( NPC_NearBackCh( n, tempch, &ex, &ey, 5 ) )
							{										
								n->MoveP = 60 + Random(30 );
								NPC_MakePath( n, ex, ey, Random(2)+2 );

							}									
							else						
							{							
								n->patterntype =NPC_PATTERN_TOBOSS_8_;		//WANDER_8_;

							}		
						}
					}			
				}				
				else			
				{				

Attack_access_pc_:
					if( n->targetid != -1 )
					{	
						int ex, ey;
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if (tempch)
						{
							if( !IsDead( tempch )) 
							{		
								if( NPC_NearCh( n, tempch, &ex, &ey ) )
								{		
									//if( NPC_EnemyOnMyWay( 		
									// �ϵ��� ���ݻ�뿡�� �����ϱ� ���� ��ƾ�� �ִ´�. ( '��' '��'���� 

									NPC_MakePath( n, ex, ey, Random(6)+2 );
									
								}			
								else		
								{	
	//								n->patterntype = NPC_PATTERN_WANDER_8_;

	//								n->bossid	 = -1;
	//								n->id = -1;
	//								JustMessage( " %d�� Pattern��ȣ %d ", n->id, n->patterntype );
	//								JustMessage( " %d�� Pattern��ȣ %d ", n->patterntype );
								}				
							}
							else 
							{
								n->patterntype =NPC_PATTERN_TOBOSS_8_;		//WANDER_8_;

							}
						}
					}					
					else
					{
						n->targetid = NPC_IsWhoNearPC( n, 10 );
						if( n->targetid != -1 ) goto Attack_access_pc_;
						else 
						{
						}
					}
				}
			}						
			break;					
								
		case NPC_PATTERN_BACKDRAW_8_ :
			{					
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1500  + (rand()%500);

								
					if( NPC_IsMoving( n ) )
					{			
								
					}			
					else		
					{			
						if( n->targetid != -1 )
						{		
							int ex, ey;
							LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
							if (tempch)
							{
								if( InDistance( n, tempch,  TILE_SIZE * 10 ) )	// �� PC��  10 Ÿ�Ͼȿ� �ִٸ�.
								{	
									if( NPC_NearBackCh( n, tempch, &ex, &ey, 7 ) )
									{	
										n->MoveP = 60 + Random(30 );
										NPC_MakePath( n, ex, ey, Random(5)+5 );
									}	
									else 
									{
									
									}
								}
								else	// ����� �ָ��ֱ�.
								{
									n->patterntype = NPC_PATTERN_WANDER_8_;

								}
							}
						}		
						else	
						{		
							n->patterntype = NPC_PATTERN_WANDER_8_;
						}		
					}				
				}				
				break;			
			}				

	} // switch
}

////////////////////////////////////////////////////////////////////////////////////////////////

void NPC_Pattern_18( CHARACTER *n )
{
//	int movep;
	int ex, ey;
				
	switch( n->patterntype )
	{			
	case NPC_PATTERN_WANDER_18_ :
		{		
			int bossid;
			int attacker = NPC_WhoIsAttackMyBaby( n );
				
			if( n->attacked != -1 ) 
			{	
				n->patterntype = NPC_PATTERN_ATTACK_PC_18_;
				break;
			}	
				
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 100 + (rand()%100);
				// �׳� ��ȸ�Ѵ�. 
				if( NPC_IsMoving( n ) )
				{
				}
				else 
				{
					bossid = NPC_WhoIsBoss( n );
					if( bossid != -1 )	// Yes.. ������ ã�Ҿ�!!
					{	
						if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_TOBOSS_18_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

							break;
						}
						else  // ���� Boss�̴�. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_IAM_BOSS_18_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
							break;
						}
					}
					else	// ��. ������ ����...
					{
						int dir;
						int how;
						int tx, ty;
							
						dir = Random(8);
						how = Random(4)+2;
						tx = n->x/TILE_SIZE;
						ty = n->y/TILE_SIZE;
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
		}		
		break;	
				
	case NPC_PATTERN_IAM_BOSS_18_ :
		{		
			int bossid;
				
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 100  + (rand()%400);
				if( NPC_IsMoving( n ) )
				{
				}
				else 
				{
					int attacker = NPC_WhoIsAttackMyBaby( n );
					if( attacker != -1) // �� �ư����� �´¾ְ� ������..
					{
						LPCHARACTER tch = Hero; // ReturnCharacterPoint( n->bossid );
						while( tch )
						{				
							if( tch != n )
							if( tch->type == SPRITETYPE_MONSTER && !IsDead( tch ) )
							{
								if( tch->bossid == n->id )
								{
									tch->targetid		= attacker;
									tch->patterntype	= NPC_PATTERN_ATTACK_PC_18_;
								}
							}
							tch = tch->lpNext;
						}
					}
					else
					{
					}
					
				
					bossid = NPC_WhoIsBoss( n );
					if( bossid == -1 )	 // ��ȥ�ڱ�..
					{	
						n->patterntype	= NPC_PATTERN_WANDER_18_;
						
						n->targetid		= -1;
						n->bossid		= -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

						break;
					}
					else if( bossid != n->id )	// dld��. ������ Exp�� ���� ���� �ֱ�.. 
					{
							n->bossid		= bossid;
							n->patterntype	= NPC_PATTERN_TOBOSS_18_;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
									
							break;
					}
					else  // ���� ���� Boss�� !  �׷� ������ ���� �ؾ���.
					{
						n->bossid = bossid;
						n->targetid = NPC_IsWhoNearPC( n, 10 );
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
					
						if( n->targetid != -1 )
						{
							LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
							if (tch)
							{
								if( InDistance( n, tch, TILE_SIZE * 10 ) )	// �� PC��  5 Ÿ�Ͼȿ� �ִٸ�.
								{	
									LPCHARACTER tempch = Hero;
									while( tempch )
									{
										if( tempch != n )
										if( tempch->type == SPRITETYPE_MONSTER && !IsDead( tempch ))
										{
											if( tempch->bossid == n->bossid )
											{
												tempch->targetid	= n->targetid;
												SendNPC_parameter( tempch->id, NPC_PARAMETER_TARGETID, tempch->targetid );

												tempch->patterntype = NPC_PATTERN_ACCESS_PC_18_;
											}
										}
										tempch = tempch->lpNext;
									}
									break;
								}
								else if( !InDistance( n, tch,  TILE_SIZE * 15 ) )	// �� PC��  15 Ÿ�Ͼȿ� ���ٸ�..
								{
									LPCHARACTER tempch = Hero;
									while( tempch )
									{
										if( tempch != n )
										if( tempch->type == SPRITETYPE_MONSTER && !IsDead( tempch ))
										{
											if( tempch->bossid == n->id )
											{
												LPCHARACTER temp2ch = ReturnCharacterPoint(n->bossid );
												if (temp2ch)
												{
													if( NPC_NearPosition( tempch, temp2ch, &ex, &ey ))
													{
														tempch->targetid = n->targetid;
														SendNPC_parameter( tempch->id, NPC_PARAMETER_TARGETID, n->targetid );

														tempch->patterntype = NPC_PATTERN_TOBOSS_18_;
														tempch->MoveP = 60 + Random(30 );
														NPC_MakePath( tempch, ex, ey, Random(6) + 2 );
														break;
													}
												}
											}
										}
										tempch = tempch->lpNext;
									}
								}
								else
								{
									if( Random(5) == 0 )
									{// �������� �ణ�� �����δ�. 
										int dir;
										int how;
										int tx, ty;
					
										dir = Random(8);
										how = Random(2)+2;
										tx = n->x/TILE_SIZE;
										ty = n->y/TILE_SIZE;
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
									// ��.. �ƴ��͵� �����ʴ´�. 
								}
							}
						}
						else	// �ٽ� �������Է� ���̶�� �Ѵ�. 
						{
						}
					}
				}
			}
		}	
		break;
			
	case NPC_PATTERN_TOBOSS_18_ :
		{	
			int bossid;
			
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%400);
					
				if( NPC_IsMoving( n ) )
				{
						
				}
				else 
				{
					if( NPC_IsInBossRange( n ) )	// Boss�ȿ� ��������..
					{
						bossid = NPC_WhoIsBoss( n );
						if( bossid != -1 )	
						{	
							if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
							{
								n->bossid = bossid;
								n->patterntype = NPC_PATTERN_TOBOSS_18_;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
		

								
								goto TO_BOSS_;
							}
							else  // ���� Boss�̴�. 
							{
								n->bossid = bossid;
								n->patterntype = NPC_PATTERN_IAM_BOSS_18_;
								n->targetid = -1;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
								SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
							}
						}
						else 
						{
							n->patterntype = NPC_PATTERN_WANDER_18_;
							
							n->targetid = -1;
							n->bossid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

							break;
						}
					}
					else //  Boss������ ������...
					{
TO_BOSS_:
						if( n->bossid != -1 )
						{
							int ex, ey;
							LPCHARACTER tempch = ReturnCharacterPoint( n->bossid );
							if (tempch)
							{
								if( NPC_NearPosition( n, tempch, &ex, &ey ))
								{
									 NPC_MakePath( n, ex, ey, Random(6) + 2 );
										break;
								}
								}
						}
						else
						{
							int targetid = NPC_IsWhoNearPC( n, 10 );
							if( targetid != -1 )
							{
								if (n->bossid != -1 )
								{
									LPCHARACTER tempch = Hero;
									while( tempch )
									{
										if( tempch->type == SPRITETYPE_MONSTER && !IsDead( tempch ))
										{
											if( tempch->bossid == n->bossid )
											{
												tempch->targetid = targetid;
												SendNPC_parameter( tempch->id, NPC_PARAMETER_TARGETID, targetid );
												tempch->patterntype = NPC_PATTERN_ATTACK_PC_18_;
											}
										}

										tempch  = tempch->lpNext;
									}
									///
								}
								else 
								{
									n->patterntype = NPC_PATTERN_WANDER_18_;
									n->bossid = -1;
									n->targetid = -1;
									SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
									SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

								}
							}
							else 
							{
									n->patterntype = NPC_PATTERN_WANDER_18_;
									n->bossid = -1;
									n->targetid = -1;
									SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
									SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
	
							}
						}
					}	
				}		
			}			
		}				
		break;			
						
		//	��������ٴѴ�... 8
		case NPC_PATTERN_MURI_18_ :
		{				
			int bossid = -1;
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{			
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 500  + (rand()%400);
						
				if( NPC_IsMoving( n ) )
				{		
				}		
				else		
				{		
					int bossid = NPC_WhoIsBoss( n ); // ȥ�ڶ�� ���.
					if( bossid != -1 )	
					{	
						if( bossid != n->id )	// ��. ������ Exp�� ���� ���� �ֱ�.. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_TOBOSS_18_;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
						}
						else  // ���� Boss�̴�. 
						{
							n->bossid = bossid;
							n->patterntype = NPC_PATTERN_IAM_BOSS_18_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
						}
					}	
					else 
					{	
						n->patterntype = NPC_PATTERN_WANDER_18_;
						
						n->bossid = -1;
						n->targetid = -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
					}	
				}		
			}			
		}				
		break;				
						
		case NPC_PATTERN_ACCESS_PC_18_ :
			{			
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{			
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = PCFINDACCESS(n); // 0813 NPC KHS
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
									n->patterntype = NPC_PATTERN_ATTACK_PC_18_;
									break;
								}
							}
						}
					}	
					else
					{	
//						if( NPC_Hostile( n ) ) // ���ݸ���̸�...
							
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if (tempch)
						{
							if( !IsDead( tempch ) )
							{
								if( NPC_IsAttackableRange( n ) )// ���� ������ �Ÿ����� �Գ� ?
								{
									SendNPCAttack( n->id );
									n->attackcount ++;
									n->patterntype = NPC_PATTERN_ATTACK_PC_18_;

									break;
								}
								else	// ���ݰ����� �Ÿ��� �ƴϸ� ��ó���� ����. 
								{
									int ex, ey;
									if( NPC_NearCh( n, tempch, &ex, &ey ) )
									{
										n->MoveP = 60 + Random(30 );
										NPC_MakePath( n, ex, ey, Random(6) + 2 );
									}
									else
									{
	//									n->patterntype = NPC_PATTERN_WANDER_18_;
										
	//									n->bossid = -1;
	//									n->id = -1;
									}
								}
							}	
							else 
							{
								n->patterntype = NPC_PATTERN_TOBOSS_18_;
								
								n->bossid	= -1;
								n->targetid = -1;
								SendNPC_parameter( n->id, NPC_PARAMETER_BOSSID, n->bossid);
								SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );

								break;
							}
						}
					}	
				}		
			}			
			break;		
						
		case NPC_PATTERN_ATTACK_PC_18_ :
							
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{			
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = ATTACKACCESS(n)  + (rand()% 600 );
						
				if( NPC_IsMoving(n))
				{		
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if( tempch )
						{
							if( !IsDead( tempch ) )
							{
								if( NPC_IsAttackableRange( n ) )
								{
									SendNPCAttack( n->id );
									n->attackcount ++;
									break;
								}
							}
						}
					break;
				}		
						
				if( n->bossid != -1)  
				{		
					LPCHARACTER tempch = ReturnCharacterPoint( n->bossid );
					if (tempch)
					{
						if( IsDead( tempch ) ) // Ȥ�� Boss�� ������....
						{	
							tempch = Hero;
							while( tempch )
							{
								if( tempch->type == SPRITETYPE_MONSTER && !IsDead( tempch ))
								{
									if( tempch->bossid == n->bossid )
									{
										tempch->bossid   = -1;
										tempch->targetid = -1;
										tempch->patterntype = NPC_PATTERN_BACKDRAW_18_;
										SendNPC_parameter( tempch->id, NPC_PARAMETER_BOSSID, n->bossid);
										SendNPC_parameter( tempch->id, NPC_PARAMETER_TARGETID, n->targetid );

									}
								}
								tempch = tempch->lpNext;
							}
							break;
						}
					}
				}		
						
						
				if( NPC_IsAttackableRange( n )  )// ���� ������ �Ÿ����� �Գ� ?
				{		
					if( NPC_StillAttack(n) )
					{	
						SendNPCAttack(  n->id );
						n->attackcount ++;
					}									
					else							
					{								
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if (tempch)
						{
							if( NPC_NearBackCh( n, tempch, &ex, &ey, 5 ) )
							{										
								n->MoveP = 60 + Random(30 );
								NPC_MakePath( n, ex, ey, Random(2)+2 );
								n->MoveP = 0;
							}									
							else						
							{							
								n->patterntype =NPC_PATTERN_TOBOSS_18_;		//WANDER_18_;
							}	
						}
					}			
				}				
				else			
				{				
Attack_access_pc_:
					if( n->targetid != -1 )
					{		
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if (tempch)
						{
							int ex, ey;
							if( !IsDead( tempch ) )
							{		
								if( NPC_NearCh( n, tempch, &ex, &ey ) )
								{		
									//if( NPC_EnemyOnMyWay( 		
									// �ϵ��� ���ݻ�뿡�� �����ϱ� ���� ��ƾ�� �ִ´�. ( '��' '��'���� 
									n->MoveP = 60 + Random(30 );
									NPC_MakePath( n, ex, ey, Random(6)+2 );
								}			
								else		
								{	
	//								n->patterntype = NPC_PATTERN_WANDER_18_;
	//								Debug( "%d :  patterntype :%d ", n->id, n->patterntype );
	//								n->bossid	 = -1;
	//								n->id = -1;
	//								JustMessage( " %d�� Pattern��ȣ %d ", n->id, n->patterntype );
	//								JustMessage( " %d�� Pattern��ȣ %d ", n->patterntype );
								}				
							}
							else 
							{
								n->patterntype =NPC_PATTERN_TOBOSS_18_;		//WANDER_18_;
								
							}
						}
					}					
					else
					{
						n->targetid = NPC_IsWhoNearPC( n, 10 );
						if( n->targetid != -1 ) goto Attack_access_pc_;
						else 
						{
						}
					}
				}
			}						
			break;					
								
		case NPC_PATTERN_BACKDRAW_18_ :
			{
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1500  + (rand()%1000);
								
					if( NPC_IsMoving( n ) )
					{			
								
					}			
					else		
					{			
						if( n->targetid != -1 )
						{		
							int ex, ey;
							LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
							if (tempch)
							{
								if( InDistance( n, tempch,  TILE_SIZE * 10 ) )	// �� PC��  10 Ÿ�Ͼȿ� �ִٸ�.
								{	
									if( NPC_NearBackCh( n, tempch, &ex, &ey, 7 ) )
									{	
										n->MoveP = 60 + Random(30 );
										NPC_MakePath( n, ex, ey, Random(5)+5 );

									}	
									else 
									{
									
									}
								}
								else	// ����� �ָ��ֱ�.
								{
									n->patterntype = NPC_PATTERN_WANDER_18_;
									
								}
							}
						}		
						else	
						{		
							n->patterntype = NPC_PATTERN_WANDER_18_;
							
						}		
					}				
				}				
				break;			
			}				
	}
}						
					
						
					
						
					
void NPC_Pattern_Tammed( CHARACTER *n )
{					
//	int movep;		
//	int ex, ey;			
//	int i;
	CHARACTER *ch;	
					
	switch( n->patterntype )
	{					
	case NPC_PATTERN_TAME_ :
		{															
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%1000);
																						
				//	�׳� ��ȸ�Ѵ�.
				if( NPC_IsMoving( n ) )	
				{								
					switch( n->tame_cmd )		
					{							
						case LC_TAME_STOP :		
												if( n->pathcount ) //���� �̵����̸� ���� Stop��Ų��. 
												{
													SendModifyPositionNPC(  n->id ); // ����ġ�� ���� �̵���Ų��. 
												}
												n->pathcount = 0;
						break;					
					}								
				}								
				else							
				{								
					switch( n->tame_cmd )
					{							
						case LC_TAME_STOP		:	
							_asm nop;						
							
							
							break;
						case LC_TAME_ATTACK		:	
						{						
							int ex, ey;
								
							ch = NPC_ReturnCharListPoint( n->targetid );
							if( ch ) // ���ݴ���� �ִٸ�
							{							
								if( strcmp( ch->name, n->TargetName ) == 0 ) // �� ���ݴ���� �̸��� ���ݴ�� ID�� ��ġ�ϸ�.
								{
									if( NPC_IsAttackableRange( n )  ) // ���� ����� ���ݰ��ɹ����� ������..
									{
										SendNPCAttack(  n->id );	// �����Ѵ�. 
										n->attackcount ++;	
										return;
									}
									else	// ���ݻ����� �ۿ� �ִٸ�.
									{
										// �װ����� �̵��Ѵ�. 
										if( NPC_NearCh( n, ch, &ex, &ey ) )
										{
											n->MoveP = 60 + Random(30 );
											if( NPC_MakePath( n, ex, ey, Random(6) + 2 ) )	
											{
												

											}
											else
											{
												if( (rand()%4) == 2 ) n->tame_cmd = LC_TAME_STOP;
												SendNPC_parameter( n->id, NPC_PARAMETER_TAME_CMD, LC_TAME_STOP );
											}
											n->MoveP = 0;
										}
										else 
										{
											if( (rand()%4) == 2 ) n->tame_cmd = LC_TAME_STOP;
											else break;
										}
									}	
								}
								else
								{
									n->tame_cmd = LC_TAME_STOP;
									SendNPC_parameter( n->id, NPC_PARAMETER_TAME_CMD, LC_TAME_STOP );

									return;
								}
							}
							else // ���ݴ���� ���ٸ�.
							{
								n->tame_cmd = LC_TAME_STOP;
								SendNPC_parameter( n->id, NPC_PARAMETER_TAME_CMD, LC_TAME_STOP );

								return;
							}
						break;
						}	
						case LC_TAME_FOLLOWME : 
						{	
							ch = NPC_ReturnCharListPoint( n->HostId );		// ���� ������ �� �ʿ� �ְ� 

							if( ch == NULL )
							{
								n->tame_cmd = LC_TAME_STOP;
								SendNPC_parameter( n->id, NPC_PARAMETER_TAME_CMD, LC_TAME_STOP );
								break;
							}
							else if( strcmp( ch->name, n->HostName ) == 0 )	// �� ���ݴ���� �̸��� ���ݴ�� ID�� ��ġ�ϸ�.
							{
								goto NEXT_FOLLOWME_;
							}
							else 
							{
								LPCHARACTER tempch = Hero;

								while( tempch )
								{
									if( strcmp( n->HostName, tempch->name ) == 0 ) // ���� ������ ������. 
									{
										if( BoxAndDotCrash( n->x/TILE_SIZE - 12,  n->y/TILE_SIZE - 12, 24,24, tempch->x/TILE_SIZE, tempch->x/TILE_SIZE ) )	// ���� ������ ���� �þ߹������� �ִ�. 
										{
											n->HostId = tempch->id;
											goto NEXT_FOLLOWME_;
										}
									}
									tempch = tempch->lpNext;
								}

								n->tame_cmd = LC_TAME_STOP;
								SendNPC_parameter( n->id, NPC_PARAMETER_TAME_CMD, LC_TAME_STOP );
								return;
							}
NEXT_FOLLOWME_:

							if( NPC_IsInHostRange( n ) )	// Host�ȿ� ��������..
							{
								break;
							}
							else  //  Boss������ ������...
							{
								if( n->HostId != -1 )
								{
									int ex, ey;
									LPCHARACTER tempch = ReturnCharacterPoint( n->HostId );
									if (tempch)
									{
										if( NPC_NearPosition( n, tempch, &ex, &ey ))
										{
											NPC_MakePath( n, ex, ey, Random(6) + 2 );

												break;
										}
									}
								}
							}
							
						break;
						}		
					}					
				}
			}		
		}		
		break;	
	}		
}			
			
void NPC_Pattern_GUARD(LPCHARACTER n)
{			
	switch (n->patterntype)
	{					
	case NPC_PATTERN_WANDER_GUARD_:
		{	
			if (g_ClientTime - n->aitimedelay > n->aidelayhowmuch)
			{
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000 + (rand()%1000);
			}
			else
			{
				break;
			}
			
			// �׳� ��ȸ�Ѵ�.
			if (!NPC_IsMoving(n))
			{		
				int targetid = NPC_IsWhoNearRedPC( n, 10 );
				if( targetid != -1 )
				{			
					n->patterntype = NPC_PATTERN_ACCESS_PC_GUARD_;
					n->targetid = targetid;
					
					if( n->targetid >= 10000 )
					{		
						
					}		
					else 
					{
						if( rand()%100 < 30 )
						{
							LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
							if (tch)
							{
#ifdef _NATION_APPLY_
								if(  tch->name_status.nation  == MapInfo[ MapNumber].nation || MapNumber == 30  )
#else
								if( 1 )
#endif
								{
									switch( rand()%3 )
									{		
									case 0:	SendNPCChatArea(n->id, lan->OutputMessage(4, 37)); break;//lsw
									case 1:	SendNPCChatArea(n->id, lan->OutputMessage(4, 38));	break;//lsw
									}					
								}
								else if( rand()%100 < 50 )
								{
									LPCHARACTER tch = ReturnCharacterPoint( n->targetid );
									if( tch )
									{
										switch( tch->name_status.nation )		//1004 YGI
										{
										case 3 : SendNPCChatArea( n->id, lan->OutputMessage(4,39) );	break;//lsw
										case 4 : SendNPCChatArea( n->id, lan->OutputMessage(4,40) );	break;//lsw
										}
									}
								}
							}
						}
					}
				}							
				else											
				{						
					if (rand()%100 < 90)  //  ��ȸ�� ���� ���� �ʰ� ���ڸ��� �ִ´�. 
					{							
						return;				
					}							
					
					int dir;						
					int how;								
					int tx, ty;							
					
					dir = Random(8);						
					how = Random(2)+2;						
					tx = n->x/TILE_SIZE;	
					ty = n->y/TILE_SIZE;			
					
					switch (dir)
					{									
					case 0:	ty +=how; break;
					case 1: tx -=how; ty +=how; break;
					case 2: tx -=how; break;
					case 3: tx -=how; ty -=how; break;
					case 4:	ty -=how; break;
					case 5: tx +=how; ty -=how; break;
					case 6: tx +=how; break;
					case 7: tx +=how; ty +=how; break;
					}
					
					NPC_MakePathGuard(n, tx, ty, how);
				}							
			}											
			
			break;							
		}
	case NPC_PATTERN_ACCESS_PC_GUARD_ :
		{						
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{						
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = PCFINDACCESS(n); // 0813 NPC KHS
				if( NPC_IsMoving( n ) )
				{				
					LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
					if( tempch )
					{
						if( !IsDead( tempch )) 
						{			
							if( NPC_IsAttackableRange( n ) )
							{		
								SendModifyPositionNPC( n->id );
								SendNPCAttack(  n->id );
								n->attackcount ++;
								n->patterntype = NPC_PATTERN_ATTACK_PC_GUARD_;
								break;
							}		
						}	
					}
				}				
				else			
				{				
					//						if( NPC_Hostile( n ) ) // ���ݸ���̸�...
					int ex,ey;	
					LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
					if( tempch )
					{			
						if( !IsDead( tempch ) )
						{		
							if( NPC_IsAttackableRange( n ) )// ���� ������ �Ÿ����� �Գ� ?
							{	
								SendNPCAttack(  n->id );
								n->attackcount ++;
								n->patterntype = NPC_PATTERN_ATTACK_PC_GUARD_;
								break;
							}	
							else	// ���ݰ����� �Ÿ��� �ƴϸ� ��ó���� ����..................
							{	
								if( NPC_NearCh( n, tempch, &ex, &ey ) )
								{
									n->MoveP = 100 + Random(80);
									NPC_MakePath( n, ex, ey, Random(6) + 2 );
									n->patterntype = NPC_PATTERN_ATTACK_PC_GUARD_;
								}
								else
								{
									n->patterntype = NPC_PATTERN_WANDER_GUARD_;
									n->targetid = -1;
									//									n->bossid = -1;
									//									n->id = -1;
								}
							}	
						}		
						else	
						{		
							n->patterntype = NPC_PATTERN_WANDER_GUARD_;
							n->targetid = -1;
							SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
							break;
						}		
					}			
					else		
					{			
						n->patterntype = NPC_PATTERN_WANDER_GUARD_;
						n->targetid = -1;
						break;	
					}			
				}				
			}			
		}				
		break;			
								
	case NPC_PATTERN_ATTACK_PC_GUARD_ :
		
		if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
		{	
			n->aitimedelay = g_ClientTime;
			n->aidelayhowmuch = ATTACKACCESS(n)  + (rand()%250);
		}
		else break;
		
		if( NPC_IsAttackableRange( n ) )	// �̵� ���� ���ݴ��ĳ���� �����Ҽ� �ִٸ� �����Ѵ�. 
		{
			goto ATTACK_GUARD_NEXT__;
		}
		
		if( NPC_IsMoving(n))	break;
		
		if( NPC_IsAttackableRange( n ) )// ���� ������ �Ÿ����� �Գ� ?
		{					
ATTACK_GUARD_NEXT__:
		if( 1 ) // if( NPC_StillAttack(n) ) 0430
		{
			SendNPCAttack(  n->id );
			n->attackcount ++;
		}								
		else							
		{								
			int ex,ey;
			LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
			if (tempch)
			{
				if( NPC_NearBackCh( n, tempch, &ex, &ey, 5 ) )
				{
					n->MoveP = 60 + Random(30 );
					NPC_MakePath( n, ex, ey, Random(2)+2 );
					n->patterntype =NPC_PATTERN_ACCESS_PC_GUARD_;		//WANDER_8_;
				}
				else
				{
					n->patterntype =NPC_PATTERN_ACCESS_PC_GUARD_;		//WANDER_8_;
					//Debug( "%d :  patterntype :%d ", n->id, n->patterntype );
				}
			}
		}
		}
		else
		{
			if( n->targetid != -1 )
			{
				int ex,ey;
				LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
				if (tempch)
				{
					if( !IsDead( tempch ) )
					{		
						if( NPC_NearCh( n, tempch, &ex, &ey ) )
						{		
							//if( NPC_EnemyOnMyWay( 		
							// �ϵ��� ���ݻ�뿡�� �����ϱ� ���� ��ƾ�� �ִ´�. ( '��' '��'���� 
							n->MoveP = 60 + Random(30 );
							NPC_MakePath( n, ex, ey, Random(6)+2 );
							
						}			
						else		
						{	
							n->patterntype =NPC_PATTERN_ACCESS_PC_GUARD_;		//WANDER_8_;
							//								n->patterntype = NPC_PATTERN_WANDER_8_;
							//								Debug( "%d :  patterntype :%d ", n->id, n->patterntype );
							//								n->bossid	 = -1;
							//								n->id = -1;
							//								JustMessage( " %d�� Pattern��ȣ %d ", n->id, n->patterntype );
							//								JustMessage( " %d�� Pattern��ȣ %d ", n->patterntype );
						}				
					}
					else 
					{
						n->patterntype =NPC_PATTERN_WANDER_GUARD_;		//WANDER_8_;
						n->targetid = -1;
						SendNPC_parameter( n->id, NPC_PARAMETER_TARGETID, n->targetid );
					}
				}
			}					
			else 
			{
				n->patterntype =NPC_PATTERN_WANDER_GUARD_;		//WANDER_8_;
				n->targetid = -1;
			}
		}
		break;					
		
	case NPC_PATTERN_BACKDRAW_PC_GUARD_ :
		{					
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 500  + (rand()%500);
				
				if( NPC_IsMoving( n ) )
				{			
					
				}			
				else		
				{			
					if( n->targetid != -1 )
					{		
						
						int ex,ey;
						LPCHARACTER tempch = ReturnCharacterPoint( n->targetid );
						if (tempch)
						{
							if( InDistance( n, tempch,  TILE_SIZE * 10 ) )	// �� PC��  10 Ÿ�Ͼȿ� �ִٸ�.
							{	
								n->patterntype = NPC_PATTERN_WANDER_GUARD_;
								if( NPC_NearBackCh( n, tempch, &ex, &ey, 10 ) )
								{	
									NPC_MakePath( n, ex, ey, Random(10)+5 );
								}	
								else 
								{
								}
							}
							else	// ����� �ָ��ֱ�.
							{
								n->patterntype = NPC_PATTERN_WANDER_GUARD_;
								
							}
						}
					}
					else
					{
						n->patterntype = NPC_PATTERN_WANDER_GUARD_;
						
					}
				}
			}
			break;			
		}
	}
}

void NPC_Pattern_MAUL( LPCHARACTER n )
{		
	switch( n->patterntype )
	{	
	case NPC_PATTERN_EVENTING_MAUL_ :
					
		//if(  n��	���� ��ũ��Ʈ�� �������̸�..
		break;		
					
	case NPC_PATTERN_ACCESS_PC_MAUL_ : 
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = PCFINDACCESS(n); // 0813 NPC KHS
			}
			else break;

			if( NPC_IsMoving( n ) )
			{		
			}		
			else		
			{		
//-------------------
				/*
				int targetid = NPC_IsWhoNearRedPC( n, 15 );
				if( targetid != -1 )
				{	
					n->patterntype = NPC_PATTERN_ACCESS_PC_MAUL_;
					n->targetid = targetid;
					

					switch( n->SprNo )
					{

					switch( rand()%10 )
					{
						case 0 :	SendNPCChatArea( n->id, "�� �׷�����.." );									break;
						case 1 :	SendNPCChatArea( n->id, "�׷��� ���ð� �ۿ��ִ� ���͸� �����ּ���." );		break;
						case 2 :	SendNPCChatArea( n->id, "�ڲ� �׷��ø� ��ſ��� ������ ���Ȱ� �ϰھ��." );	break;
						case 3 :	SendNPCChatArea( n->id, "�ʹ��ϴ±���.. ��.. ���� ���Ƶ���..." );				break;
						case 4 :	SendNPCChatArea( n->id, "�ο�°��� ���̻� �뼭 �Ҽ� ����." );				break;
					}
				}					
				else			
				{					
					int dir;
					int how;
					int tx, ty;
					
					dir = Random(8);
					how = Random(2)+2;
					tx = n->x/TILE_SIZE;
					ty = n->y/TILE_SIZE;
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
					
					if( NPC_MakePathBumn( n, tx, ty, how ) )
					SendMoveNPCArea(  n->id );
					
					//  PK�� ĳ���Ͱ� ���ٸ� ������ �ִ´�. 
				}				
			}					
								
		break;				
//-------------------------
				*/
			}
	
		break;


	case NPC_PATTERN_ATTACK_MAUL_	:
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = ATTACKACCESS(n)  + (rand()%1000);
			}
			else break;

			if( NPC_IsMoving( n ) )
			{
			}
			else 
			{

			}

		
		break;
	case NPC_PATTERN_BACKDRAW_MAUL_	:
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%1000);
			}
			else break;


		break;
			
	case NPC_PATTERN_WANDER_MAUL_	:


			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 1000  + (rand()%400);
			}
			else break;

				// �׳� ��ȸ�Ѵ�. 
			if( NPC_IsMoving( n ) )
			{
			}
			else 
			{
				return;
				if( rand()%100 < 90 )  //  ��ȸ�� ���� ���� �ʰ� ���ڸ��� �ִ´�. 
				{
					return;
				}

				int dir;
				int how;
				int tx, ty;

				dir = Random(8);
				how = Random(3)+1;
				tx = n->x/TILE_SIZE;
				ty = n->y/TILE_SIZE;
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

				if( tx >= 0 && ty >= 0 && tx < g_Map.file.wWidth && ty < g_Map.file.wHeight )
				if( TileMap[n->x/TILE_SIZE][ n->y/TILE_SIZE].attr_inside == TileMap[ tx][ ty].attr_inside )
				{
					NPC_MakePathMaul( n, tx, ty, how );
				}
			}

		break;

	}
}	
	
	
void Recv_FaceDirections( t_face_directions *tp )
{
	LPCHARACTER ch = ReturnCharacterPoint( tp->id );
	if( ch == NULL ) return;
	ch->todir = (DIRECTION) tp->dir;
}
	
void Send_JustAni( int id, int anino )
{	
	t_packet packet;
	
	packet.h.header.type = CMD_JUST_ANI;
	packet.h.header.size = sizeof( t_just_ani );
		packet.u.just_ani.id  = id;
		packet.u.just_ani.ani = anino;
		
	QueuePacket( &packet, 1 );
}
		
void Recv_JustAni( t_just_ani *p )
{		
	LPCHARACTER ch = ReturnCharacterPoint( p->id );
	if( ch == NULL ) return;
		
	if( p->id >= 10000 )
	{	
		ch->just_ani = true;
		CharDoAni( ch, ch->direction, p->ani );
	}	
}		
		
void NPC_Pattern_SealStone( LPCHARACTER n )
{
	int nation;

						
	switch( n->patterntype )
	{					
	case NPC_PATTERN_SEALSTONE_NO_OPERATE_ :
							
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch )
			{
				n->aitimedelay = g_ClientTime;
				n->aidelayhowmuch = 5000 + (rand()%1000);
			}
			else break;

			if( n->attacked > 30 )
				if( rand()%2 == 0 )
				{
					n->attacked = 0;
				}
				else
				{
					n->attacked = 0;

					switch( n->sprno ) // ��輮�� �����ڵ�..
					{
						case 98 : nation = 3;	break;
						case 99 : nation = 4;	break;
						case 91 : nation = 6;	break;
					}

					int targetid = 	NPC_IsWhoNearOtherNationPc( nation, n, 10);
					if( targetid != -1 )
					{
						n->patterntype = NPC_PATTERN_SEALSTONE_ATTACK_;
						n->targetid = targetid;
						Send_JustAni( n->id, 1 );
						n->aitimedelay = g_ClientTime;
						n->aidelayhowmuch = 1000;
					}
				}
		break;

	case NPC_PATTERN_SEALSTONE_ATTACK_:
			
			if( g_ClientTime - n->aitimedelay > n->aidelayhowmuch ) 
			{	
				n->aitimedelay		= g_ClientTime;
				n->aidelayhowmuch	= 3000;
			}
			else break;
			
			LPCHARACTER ch = ReturnCharacterPoint( n->targetid );
			if( ch )	
			{
				n->patterntype = NPC_PATTERN_SEALSTONE_NO_OPERATE_;
			}
			break;					
	}

}