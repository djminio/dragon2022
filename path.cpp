/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/

#include "stdafx.h"
#include "path.h"
#include "map.h"
#include "Dragon.h"
#include "Hong_Sub.h"
#include "Item.h"
#include "CharDataTable.h"




extern int tool_Pathfind;

///////////////////////////////////////////////////////////////////////////////
//

int			PATHCOUNT;
int			SHORTPATH[ MAX_SHORTPATH ][ 2 ];
LPSP_NODE	GLPSP_OPEN, GLPSP_CLOSED;
LPSP_STACK	GLPSP_STACK;


///////////////////////////////////////////////////////////////////////////////
//

BOOL		PathBuild( LPCHARACTER lpCharacter );
void		PathDestroy( void );
LPSP_NODE	PathFind( LPCHARACTER lpcharacter, int sx, int sy, int dx, int dy );
LPSP_NODE	PathFind_Monster( LPCHARACTER lpcharacter, int sx, int sy, int dx, int dy );
void		GenerateSucc( LPSP_NODE lpspNode, int x, int y, int dx, int dy, int dist );
void		PropagateDown( LPSP_NODE lpspNode, int dist );
LPSP_NODE	CheckOPEN( int tilenum );
LPSP_NODE	CheckCLOSED( int tilenum );
void		Insert( LPSP_NODE lpspNode );
void		Push( LPSP_NODE lpspNode );
LPSP_NODE	Pop( void );
	
///////////////////////////////////////////////////////////////////////////////
//	
int  PathCollisionLineCount; //   ±æ¯±â¿¡ »ç¿ëµÉ ¾ÆÀÌÅÛÀ» ¸ð¾Æ ³õ´Â´Ù.
COLLISIONLINE PathCollisionLine[ MAX_ITEM_COLLISION ];
	
// 010724 KHS	
BOOL PathBuild( LPCHARACTER lpCharacter )
{	
	LPSP_NODE		lpspNode;
	int				sx, sy;
	int				dx, dy;


	dx = lpCharacter->destx, dy = lpCharacter->desty;

	if(lpCharacter->nCurrentAction == ACTION_MAGIC_BEFORE)	return 0;

	sx = lpCharacter->x / TILE_SIZE;
	sy = lpCharacter->y / TILE_SIZE;
	

	bool occupied_flag = false;
	TILE *pT = &TileMap[ lpCharacter->position.x][lpCharacter->position.y];

	if( pT->occupied ) 
	{
		pT->occupied = 0;
		occupied_flag = true;
	}


	switch( lpCharacter->sprno )
	{		
		case 0 :
		case 1 :	lpspNode = PathFind( lpCharacter, sx, sy, dx, dy );
			break;
		default : 	lpspNode = PathFind_Monster( lpCharacter, sx, sy, dx, dy );
			break;
	}		

	if( occupied_flag )	pT->occupied = 1;
			
	if ( lpspNode == NULL )
	{
		PathDestroy( );
		return	FALSE;
	}
	
	lpCharacter->path[ 0 ][ 0 ] = dx;
	lpCharacter->path[ 0 ][ 1 ] = dy;
	lpCharacter->pathcount = 1;
	
	while ( lpspNode->lpParent != NULL )
	{
		if ( lpCharacter->pathcount >= MAX_SHORTPATH )
		{
			break;
		}
	
		lpspNode = lpspNode->lpParent;
		lpCharacter->path[ lpCharacter->pathcount ][ 0 ] = lpspNode->x;
		lpCharacter->path[ lpCharacter->pathcount ][ 1 ] = lpspNode->y;
		lpCharacter->pathcount++;
	}
	
	PathDestroy( );
	
	if ( lpCharacter->pathcount > 1 )
	{	
		return	TRUE;
	}	
		
	return	FALSE;
}		
		
void	
PathDestroy( void )
{		
	LPSP_NODE	lpspNode, lpspTemp;
		
	if ( GLPSP_OPEN != NULL && GLPSP_CLOSED != NULL && GLPSP_STACK != NULL )
	{	
		lpspNode = GLPSP_OPEN->lpNext;
		while( lpspNode != NULL )
		{
			lpspTemp = lpspNode->lpNext;
			GlobalFree( lpspNode );
			lpspNode = lpspTemp;
		}
		
		lpspNode = GLPSP_CLOSED->lpNext;
		while( lpspNode != NULL )
		{
			lpspTemp = lpspNode->lpNext;
			GlobalFree( lpspNode );
			lpspNode = lpspTemp;
		}
		
		GlobalFree( GLPSP_OPEN );
		GlobalFree( GLPSP_CLOSED );
		GlobalFree( GLPSP_STACK );
		GLPSP_OPEN = NULL;
		GLPSP_CLOSED = NULL;
		GLPSP_STACK = NULL;
	}	
}		


// 010724-1 KHS		
int FreeTile( LPCHARACTER ch, int sx, int sy, int x, int y )
{		
	TILE  *pT;
	pT = &TileMap[ x ][ y ];
	if( pT->attr_dont ) 	return 0;
	
	if (!ch->JoinLocalWar && pT->occupied)
	{	//< CSD-040317
		return 0;
	}	//> CSD-040317
		
#define MAX_AREA_X_		21	// Map ServerÀÇ Area.hÀÇ REM_AREA_X¿Í °°´Ù.
#define MAX_AREA_Y_		18
		
	if( ch )
	{	
		int tx, ty;
		tx = ch->x / TILE_SIZE - MAX_AREA_X_;
		ty = ch->y / TILE_SIZE - MAX_AREA_Y_;
			
		if( x < tx ) return 0;
		if( y < ty ) return 0;
			
		if( x > tx + MAX_AREA_X_ + MAX_AREA_X_ ) return 0;
		if( y > ty + MAX_AREA_Y_ + MAX_AREA_Y_ ) return 0;
			
			
		if( ch->notcomeinside ) //¸Ê¼Ó¼ºÁß inside¼Ó¼º¿¡ µé¾î¿ÀÁö ¸øÇÏ´Â Ä³¸¯ÅÍ°¡ ¾Ò´Ù. 
		{	
			if( pT->attr_inside ) return 0;
		}	
	}
			
	if( x < 0 ) return 0;
	if( y < 0 ) return 0;
	if( x >= g_Map.file.wWidth  ) return 0;
	if( y >= g_Map.file.wHeight ) return 0;
			
	//	ÁÖÀÎ°øÀÌ Ghost°¡ µÇ¸é ¹®À» ¿­ÇÊ¿ä ¾øÀÌ Áö³ª´Ù´Ò¼ö ÀÖ´Ù. 
	if( ch )
	if( Hero )
	{	
		if( ch == Hero && Hero->viewtype == VIEWTYPE_GHOST_ ) 
		{
			return 1;
		}
	}	
		
	LPITEMGROUND  i = g_Item;
	while( i != NULL )
	{	
		switch( i->type )
		{	
		case ITEMTYPE_BOX  : 
			break;
		
		case ITEMTYPE_DOOR : 
			if( i->Anitype == ITEMANIMATION_CLOSED ) 
			{
				if( CheckIntersect(	i->ddx[ 0], i->ddy[ 0], 
									i->dsx[ 0], i->dsy[ 0],  (sx<<5) + 16, (sy<<5) + 16, (x<<5) + 16, (y<<5) + 16 ) > 0)
				{
					return 0;
				}
			}
		}
		i = i->next;
	}	
		
	return 1;
}		
			

///////////////////////// 0812 yjs ¼öÁ¤ ///////////////////////////////////////////
//1Å¸ÀÏ¾¿ °¡¸é¼­ ¸ø°¥¶§ ±îÁö Ã¼Å©...
int Tile_Collision(LPCHARACTER ch, DIRECTION dir, int Power) //Ãà±¸°ø, ¹æÇâ, Èû..
{
	LPCHARACTER lpCharacter=Hero->lpNext;//¹«Á¶°Ç hero°¡ Á¦ÀÏ ¾Õ¿¡ ÀÖ´Ù..
	
	int t_sx = ch->x/TILE_SIZE;// Å¸ÀÏ ÁÂÇ¥·Î ¸¸µç´Ù..
	int t_sy = ch->y/TILE_SIZE;

	int	how_X=0, how_Y=0;

	switch(dir)
	{
		case DIRECTION_DOWN:		how_X=0;	how_Y=1;	break;
		case DIRECTION_LEFTDOWN:	how_X=-1;	how_Y=1;	break;
		case DIRECTION_LEFT:		how_X=-1;	how_Y=0;	break;
		case DIRECTION_LEFTUP:		how_X=-1;	how_Y=-1;	break;
		case DIRECTION_UP:			how_X=0;	how_Y=-1;	break;
		case DIRECTION_RIGHTUP:		how_X=1;	how_Y=-1;	break;
		case DIRECTION_RIGHT:		how_X=1;	how_Y=0;	break;
		case DIRECTION_RIGHTDOWN:	how_X=1;	how_Y=1;	break;
	}

	while ( lpCharacter != NULL )	///////////////// ³ªÀÚ½ÅÀÌ °ø¿¡ ºÎµúÄ¡¸é ±×³É Åë°úÇÏ¶ó...
	{
		for(int i=1; i<Power; i++)  //////////////  don'tÅ¸ÀÏ ÀÎ°¡? Ã¼Å©..///////////
		{							
			t_sx += how_X;
			t_sy += how_Y;
			
			if( TileMap[ t_sx ][ t_sy ].attr_dont )
			{
				if(i == 1)
					return -1;	//°ø°ú don'tÅ¸ÀÏÀÌ ºÙ¾îÀÖ´Â °æ¿ì......
				else
					return (i-1); ////////////// °øÀÌ °¥¼ö ÀÖ´Â Å¸ÀÏ°Ú¼ö.......
			}
			
			if( lpCharacter != ch )
				if( ch->x/TILE_SIZE + t_sx == lpCharacter->x/TILE_SIZE && ch->y/TILE_SIZE + t_sy == lpCharacter->y/TILE_SIZE)
					return(i-1);	//³ª ÀÌ¿ÜÀÇ ´Ù¸¥ °Í°ú ºÎµúÄ¡¸é ¸ø°£´Ù..
		}
		lpCharacter = lpCharacter->lpNext;
	}
	return Power;
}


/////////////////////////// 0727 yjs ¸¸µë ///////////////////////////////
////////Æ¨±â´Â ÇÔ¼ö..

/*void Reaction(LPCHARACTER ch, DIRECTION& old_dir, int& power)  //Ãà±¸°ø, º®¿¡ ¸ÂÀ»¶§ ¹æÇâ, Æ¨±æ¶§ ¹æÇâ, Èû(Å¸ÀÏ´ÜÀ§)
{
	int t_power; // ÆÄÀÏ´ÜÀ§..
	int check;
//	int t_X;
//	int t_Y;
	int count=0;		
	
	switch(old_dir)
	{
		case 	DIRECTION_DOWN:
			do	//do...while¹® Å¸ÀÏ´ÜÀ§·Î Ã¼Å©ÇØ¼­..
			{
				switch(rand()%3)
				{
					case 0:	t_power = Tile_Collision(ch, DIRECTION_LEFTUP, power);  //DIRECTION_LEFTUP
							check = power - t_power;
					break;

					case 1: t_power = Tile_Collision(ch, DIRECTION_UP, power);  //DIRECTION_UP:
							check = power - t_power;
					break;

					case 2: t_power = Tile_Collision(ch, DIRECTION_RIGHTUP, power);  //DIRECTION_RIGHTUP:
							check = power - t_power;
					break;

					default	:	power = 0;		break;
				}

				count++;
				if(count == 6)		power = 0;//3¹æÇâ´Ù ¸ø°¡´Â ¼ö°¡ ÀÖ±â¶§¹®..

			}while(check == 0 || count > 6);
		break;
					
		case	DIRECTION_LEFTDOWN:
			do
			{
				switch(rand()%2)
				{
					case 0: t_power = Tile_Collision(ch, DIRECTION_UP, power);  //DIRECTION_UP:
							check = power - t_power;
					break;

					case 1: t_power = Tile_Collision(ch, DIRECTION_RIGHTUP, power);  //DIRECTION_RIGHTUP:
							check = power - t_power;
					break;

					default	:	power = 0;		break;
				}
			
				count++;
				if(count == 6)		power = 0;//2¹æÇâ´Ù ¸ø°¡´Â ¼ö°¡ ÀÖ±â¶§¹®..

			}while(check == 0 || count > 6);
		break;

		case	DIRECTION_LEFT:	
			do
			{
				t_power = Tile_Collision(ch, DIRECTION_RIGHT, power);
				check = power - t_power;

				count++;
				if(count == 6)		power = 0;//¸ø°¡´Â ¼ö°¡ ÀÖ±â¶§¹®..

			}while(check == 0 || count > 6);
		break;
			
		case	DIRECTION_LEFTUP:
			do
			{
				switch(rand()%2)
				{
					case 0: t_power = Tile_Collision(ch, DIRECTION_DOWN, power);  //DIRECTION_DOWN:
							check = power - t_power;
					break;

					case 1: t_power = Tile_Collision(ch, DIRECTION_RIGHTDOWN, power);  //DIRECTION_RIGHTUP:
							check = power - t_power;
					break;

					default	:	power = 0;		break;
				}
			
				count++;
				if(count == 6)		power = 0;//2¹æÇâ´Ù ¸ø°¡´Â ¼ö°¡ ÀÖ±â¶§¹®..

			}while(check == 0 || count > 6);
		break;			

		case	DIRECTION_UP:
			do
			{
				switch(rand()%2)
				{
					case 0: t_power = Tile_Collision(ch, DIRECTION_LEFT, power);
							check = power - t_power;

					case 1: t_power = Tile_Collision(ch, DIRECTION_DOWN, power);  //DIRECTION_DOWN:
							check = power - t_power;
					break;

					case 2: t_power = Tile_Collision(ch, DIRECTION_RIGHTDOWN, power);  //DIRECTION_RIGHTUP:
							check = power - t_power;
					break;

					default	:	power = 0;		break;
				}
			
				count++;
				if(count == 6)		power = 0;//3¹æÇâ´Ù ¸ø°¡´Â ¼ö°¡ ÀÖ±â¶§¹®..

			}while(check == 0 || count > 6);
		break;				

		case	DIRECTION_RIGHTUP:
			do
			{
				switch(rand()%2)
				{
					case 0: t_power = Tile_Collision(ch, DIRECTION_DOWN, power);  //DIRECTION_DOWN:
							check = power - t_power;
					break;

					case 1: t_power = Tile_Collision(ch, DIRECTION_LEFTDOWN, power);  //DIRECTION_RIGHTUP:
							check = power - t_power;
					break;

					default	:	power = 0;		break;
				}
			
				count++;
				if(count == 6)		power = 0;//2¹æÇâ´Ù ¸ø°¡´Â ¼ö°¡ ÀÖ±â¶§¹®..

			}while(check == 0 || count > 6);
		break;				

		case	DIRECTION_RIGHT:
			do
			{
				t_power = Tile_Collision(ch, DIRECTION_LEFT, power);
				check = power - t_power;

				count++;
				if(count == 6)		power = 0;//¸ø°¡´Â ¼ö°¡ ÀÖ±â¶§¹®..

			}while(check == 0 || count > 6);
		break;

		case	DIRECTION_RIGHTDOWN:
			do
			{
				switch(rand()%2)
				{
					case 0: t_power = Tile_Collision(ch, DIRECTION_UP, power);  
							check = power - t_power;
					break;

					case 1: t_power = Tile_Collision(ch, DIRECTION_LEFTUP, power);  
							check = power - t_power;
					break;

					default	:	power = 0;		break;
				}
			
				count++;
				if(count == 6)		power = 0;//2¹æÇâ´Ù ¸ø°¡´Â ¼ö°¡ ÀÖ±â¶§¹®..

			}while(check == 0 || count > 6);
		break;

		default				:	power = 0;		break;							
	}
}
*//////////////////////////////////////////////////////////////////////////////////////////


		
int FreeTileLineCheck( LPCHARACTER ch, int sx, int sy, int x, int y )
{		

		

	
	if( TileMap[ x ][ y ].attr_dont ) 	return 0;
	
	if( x < 0 ) return 0;
	if( y < 0 ) return 0;
	if( x >= g_Map.file.wWidth  ) return 0;
	if( y >= g_Map.file.wHeight ) return 0;
	
	if( ch )
	if( ch->notcomeinside ) //¸Ê¼Ó¼ºÁß inside¼Ó¼º¿¡ µé¾î¿ÀÁö ¸øÇÏ´Â Ä³¸¯ÅÍ°¡ ¾Ò´Ù. 
	{
		if( TileMap[x][y].attr_inside ) return 0;
	}
	
	int mx = x * TILE_SIZE + 15;
	int my = y * TILE_SIZE + 15;
				
	LPITEMGROUND  i = g_Item;		
	while( i != NULL )
	{	
		switch( i->type )
		{	
		case ITEMTYPE_DOOR : 
			if( i->Anitype == ITEMANIMATION_CLOSED ) 
			{
				if( CheckIntersect(	i->ddx[ 0], i->ddy[ 0], 
									i->dsx[ 0], i->dsy[ 0],  (sx<<5) + 16, (sy<<5) + 16, (x<<5) + 16, (y<<5) + 16 ) > 0)
				{
					return 0;
				}
			}
		}
		i = i->next;
	}	
	
	return 1;
}		
		
		
int FreeTileOther( LPCHARACTER ch, int x, int y )
{		

		
	LPCHARACTER lpCharacter = Hero;
	DWORD iid;//, tid;
		
	iid = ch->id;
	if( iid >= 10000 ) iid -= 10000;
		
	if( TileMap[ x ][ y ].attr_dont ) return 0;
		
	int mx = x * TILE_SIZE + 15;
	int my = y * TILE_SIZE + 15;
		
	LPITEMGROUND  i = g_Item;
		
	while( i != NULL )
	{	
		switch( i->type )
		{
		case ITEMTYPE_BOX  : 
			if( i->Anitype == ITEMANIMATION_CLOSED ) 
			{
				if( IsInBox( i->dx[0], i->dy[0],  i->dx[1], i->dy[1],  i->dx[2], i->dy[2],  i->dx[3], i->dy[3], mx, my ) > 0 )
					return 0;
			}
		}
		i = i->next;
	}	
		
	return 1;
}	
	
LPSP_NODE
PathFind_Monster( LPCHARACTER lpcharacter, int sx, int sy, int dx, int dy )
{	
	LPMAPFILE	lpMapFile = &g_Map.file;
	LPSP_NODE	lpspNode, lpspTemp;
	int			nDestTile = dy * g_Map.file.wWidth + dx;
	int			x, y;
	int         c = 0;
	int			oldx, oldy;
	GLPSP_OPEN = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
	GLPSP_CLOSED = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ));
	GLPSP_STACK = ( LPSP_STACK )GlobalAlloc( GPTR, sizeof( SP_STACK ) );
	lpspNode = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
	if ( GLPSP_OPEN == NULL || GLPSP_CLOSED == NULL || GLPSP_STACK == NULL || lpspNode == NULL )
	{
		return	NULL;
	}

	lpspNode->g = 0;
	lpspNode->h = 1;//( sx - dx ) * ( sx - dx ) + ( sy - dy ) * ( sy - dy );
	lpspNode->f = lpspNode->g + lpspNode->h;
	lpspNode->number = sy * g_Map.file.wWidth + sx;
	lpspNode->x = sx;
	lpspNode->y = sy;

	GLPSP_OPEN->lpNext = lpspNode;
	while ( 1 )
	{
		if ( GLPSP_OPEN->lpNext == NULL )
		{
			return	NULL;
		}

		c++;
		if( c > 1000 ) 	
		{
			return NULL;
		}
	
		lpspTemp = GLPSP_OPEN->lpNext;
		GLPSP_OPEN->lpNext = lpspTemp->lpNext;
		lpspTemp->lpNext = GLPSP_CLOSED->lpNext;
		GLPSP_CLOSED->lpNext = lpspTemp;
	
		if ( lpspTemp->number == nDestTile )
		{
			break;
		}
	
		oldx = lpspTemp->x, oldy = lpspTemp->y;
	
		// upper
		x = lpspTemp->x, y = lpspTemp->y - 1;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy, 2 );
		}
		// left
		x = lpspTemp->x - 1, y = lpspTemp->y;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
		// right
		x = lpspTemp->x + 1, y = lpspTemp->y;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
		// down
		x = lpspTemp->x, y = lpspTemp->y + 1;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ) )
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
	
	
	
		// upper-right
		x = lpspTemp->x + 1, y = lpspTemp->y - 1;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
		// right-down
		x = lpspTemp->x + 1, y = lpspTemp->y + 1;
		if ( FreeTile( lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
		// left-down
		x = lpspTemp->x - 1, y = lpspTemp->y + 1;
		if ( FreeTile( lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
		// left-up
		x = lpspTemp->x - 1, y = lpspTemp->y - 1;
		if ( FreeTile( lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
	}
	
	return	lpspTemp;
}	
	
	
	
	
	
LPSP_NODE
PathFind( LPCHARACTER lpcharacter, int sx, int sy, int dx, int dy )
{	
	LPMAPFILE	lpMapFile = &g_Map.file;
	LPSP_NODE	lpspNode, lpspTemp;
	int			nDestTile = dy * g_Map.file.wWidth + dx;
	int			x, y;
	int         c = 0;
	int			oldx, oldy;
	GLPSP_OPEN = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
	GLPSP_CLOSED = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ));
	GLPSP_STACK = ( LPSP_STACK )GlobalAlloc( GPTR, sizeof( SP_STACK ) );
	lpspNode = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
	if ( GLPSP_OPEN == NULL || GLPSP_CLOSED == NULL || GLPSP_STACK == NULL || lpspNode == NULL )
	{
		return	NULL;
	}
	
	lpspNode->g = 0;
	lpspNode->h = 1;//( sx - dx ) * ( sx - dx ) + ( sy - dy ) * ( sy - dy );
	lpspNode->f = lpspNode->g + lpspNode->h;
	lpspNode->number = sy * g_Map.file.wWidth + sx;
	lpspNode->x = sx;
	lpspNode->y = sy;
	
	GLPSP_OPEN->lpNext = lpspNode;
	while ( 1 )
	{
		if ( GLPSP_OPEN->lpNext == NULL )
		{
			return	NULL;
		}

		c++;
		if( c > 1000 ) 	
		{
			return NULL;
		}

		lpspTemp = GLPSP_OPEN->lpNext;
		GLPSP_OPEN->lpNext = lpspTemp->lpNext;
		lpspTemp->lpNext = GLPSP_CLOSED->lpNext;
		GLPSP_CLOSED->lpNext = lpspTemp;

		if ( lpspTemp->number == nDestTile )
		{
			break;
		}

		oldx = lpspTemp->x, oldy = lpspTemp->y;

		// upper
		x = lpspTemp->x, y = lpspTemp->y - 1;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy, 2 );
		}
		// left
		x = lpspTemp->x - 1, y = lpspTemp->y;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
		// right
		x = lpspTemp->x + 1, y = lpspTemp->y;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}
		// down
		x = lpspTemp->x, y = lpspTemp->y + 1;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ) )
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,2 );
		}



		// upper-right
		x = lpspTemp->x + 1, y = lpspTemp->y - 1;
		if ( FreeTile(lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,3 );
		}
		// right-down
		x = lpspTemp->x + 1, y = lpspTemp->y + 1;
		if ( FreeTile( lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,3 );
		}
		// left-down
		x = lpspTemp->x - 1, y = lpspTemp->y + 1;
		if ( FreeTile( lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,3 );
		}
		// left-up
		x = lpspTemp->x - 1, y = lpspTemp->y - 1;
		if ( FreeTile( lpcharacter, oldx, oldy, x, y ))
		{
			GenerateSucc( lpspTemp, x, y, dx, dy,3 );
		}
	}

	return	lpspTemp;
}

void
GenerateSucc( LPSP_NODE lpspNode, int x, int y, int dx, int dy, int dist )
{
	int			g, nTile;
	int			i;
	LPSP_NODE	lpspOld, lpspSucc = 0;

	g = lpspNode->g + dist;
	nTile = y * g_Map.file.wWidth + x;

	if ( ( lpspOld = CheckOPEN( nTile ) ) != NULL )
	{
		for ( i = 0; i < 8; i++ )
		{
			if ( lpspNode->lpChild[ i ] == NULL )
			{
				break;
			}
		}

		if( i == 8 ) 
		{
			__asm nop;
		}


		lpspNode->lpChild[ i ] = lpspOld;
		if ( g < lpspOld->g )
		{
			lpspOld->lpParent = lpspNode;
			lpspOld->g = g;
			lpspOld->f = g + lpspOld->h;
		}
	}
	else if ( ( lpspOld = CheckCLOSED( nTile ) ) != NULL )
	{
		for ( i = 0; i < 8; i++ )
		{
			if ( lpspNode->lpChild[ i ] == NULL )
			{
				break;
			}
		}
		if( i == 8 ) 
		{
			__asm nop;
		}

		lpspNode->lpChild[ i ] = lpspOld;
		if ( g < lpspOld->g )
		{
			lpspOld->lpParent = lpspNode;
			lpspOld->g = g;
			lpspOld->f = g + lpspOld->h;

			PropagateDown( lpspOld, dist );
		}
	}
	else
	{
		lpspSucc = ( LPSP_NODE )GlobalAlloc( GPTR, sizeof( SP_NODE ) );
		lpspSucc->lpParent = lpspNode;
		lpspSucc->g = g;
	
//		if( rand()% tool_Pathfind )	lpspSucc->h = rand()%50 + 1;
//		else						lpspSucc->h = 1;//( x - dx ) * ( x - dx ) + ( y - dy ) * ( y - dy );
//		if( rand()% tool_Pathfind )	lpspSucc->h = rand()%50 + 1;
		lpspSucc->h =dist;//( x - dx ) * ( x - dx ) + ( y - dy ) * ( y - dy );

		lpspSucc->f = g + lpspSucc->h;
		lpspSucc->x = x;
		lpspSucc->y = y;
		lpspSucc->number = nTile;

		Insert( lpspSucc );

		for( i = 0; i < 8; i++ )
		{
			if ( lpspNode->lpChild[ i ] == NULL )
			{
				break;
			}
		}

		if( i == 8 ) 
		{
			__asm nop;
		}


		lpspNode->lpChild[ i ] = lpspSucc;
	}
}

void
PropagateDown( LPSP_NODE lpspNode, int dist )
{
	int			i;
	int			g = lpspNode->g;
	LPSP_NODE	lpspChild, lpspParent;

	for ( i = 0; i < 8; i++ )
	{
		if ( ( lpspChild = lpspNode->lpChild[ i ] ) == NULL )
		{
			break;
		}

		if ( g + dist < lpspChild->g )
		{
			lpspChild->g = g + dist;
			lpspChild->f = lpspChild->g + lpspChild->h;
			lpspChild->lpParent = lpspNode;
			Push( lpspChild );
		}
	}

	while ( GLPSP_STACK->lpNext != NULL )
	{
		lpspParent = Pop( );
		for ( i = 0; i < 8; i++ )
		{
			if ( ( lpspChild = lpspParent->lpChild[ i ] ) == NULL )
			{
				break;
			}
			if ( lpspParent->g + dist < lpspChild->g )
			{
				lpspChild->g = lpspParent->g + dist;
				lpspChild->f = lpspChild->g + lpspChild->h;
				lpspChild->lpParent = lpspParent;
				Push( lpspChild );
			}
		}
	}
}

LPSP_NODE
CheckOPEN( int tilenum )
{
	LPSP_NODE	lpspNode;

	lpspNode = GLPSP_OPEN->lpNext;
	while ( lpspNode != NULL )
	{
		if ( lpspNode->number == tilenum )
		{
			return	lpspNode;
		}
		else
		{
			lpspNode = lpspNode->lpNext;
		}
	}

	return	NULL;
}

LPSP_NODE
CheckCLOSED( int tilenum )
{
	LPSP_NODE	lpspNode;

	lpspNode = GLPSP_CLOSED->lpNext;
	while ( lpspNode != NULL )
	{
		if ( lpspNode->number == tilenum )
		{
			return	lpspNode;
		}
		else
		{
			lpspNode = lpspNode->lpNext;
		}
	}

	return	NULL;
}

void
Insert( LPSP_NODE lpspNode )
{
	LPSP_NODE	lpspTmp1, lpspTmp2;
	int			f;

	if ( GLPSP_OPEN->lpNext == NULL )
	{
		GLPSP_OPEN->lpNext = lpspNode;
		return;
	}

	f = lpspNode->f;
	lpspTmp1 = GLPSP_OPEN;
	lpspTmp2 = GLPSP_OPEN->lpNext;
	while ( ( lpspTmp2 != NULL ) && ( lpspTmp2->f < f ) )
	{
		lpspTmp1 = lpspTmp2;
		lpspTmp2 = lpspTmp2->lpNext;
	}

	lpspNode->lpNext = lpspTmp2;
	lpspTmp1->lpNext = lpspNode;
}

void
Push( LPSP_NODE lpspNode )
{
	LPSP_STACK	lpspStack = 0;

	lpspStack = ( LPSP_STACK )GlobalAlloc( GPTR, sizeof( SP_STACK ) );
	lpspStack->lpNode = lpspNode;
	lpspStack->lpNext = GLPSP_STACK->lpNext;
	GLPSP_STACK->lpNext = lpspStack;
}

LPSP_NODE
Pop( void )
{
	LPSP_NODE	lpspNode;
	LPSP_STACK	lpspStack;

	lpspStack = GLPSP_STACK->lpNext;
	lpspNode = lpspStack->lpNode;
	GLPSP_STACK->lpNext = lpspStack->lpNext;
	GlobalFree( lpspStack );

	return	lpspNode;
}





// ÀÌµ¿¸ñÇ¥Á¡±îÁö¸¦ Á÷¼±À¸·Î ±×¾î Ãæµ¹»ý±â´Â°ÍÀÌ ÀÖ³ª ¾Ë¾Æº»´Ù. 
//  TileMap[][]À» Ã¼Å©ÇÏ¿© Ãæµ¹µÇ¸é 0À» ¸®ÅÏ.

// 010724 KHS
int CheckLineCollision( LPCHARACTER ch, int sx, int sy, int ex, int ey )
{
	int t,distance;
	int xerr = 0, yerr = 0, delta_x, delta_y;
	int incx, incy;

	
	if( sx < 0 ) return 0;
	if( sy < 0 ) return 0;
	if( ex >= g_Map.file.wWidth  ) return 0;
	if( ey >= g_Map.file.wHeight ) return 0;

	delta_x = ex - sx;
	delta_y = ey - sy;
	
	incx = (delta_x > 0) ?  1 : ((delta_x == 0) ? 0 : -1) ;
	incy = (delta_y > 0) ?  1 : ((delta_y == 0) ? 0 : -1) ;
	
	delta_x = abs(delta_x);
	delta_y = abs(delta_y);
	
	distance = (delta_x > delta_y) ? delta_x : delta_y ;


	bool occupied_flag = false;
	TILE *pT = &TileMap[ ch->position.x][ ch->position.y];
	if( pT->occupied ) 
	{
		pT->occupied = 0;
		occupied_flag = true;
	}

	if( !FreeTile( ch, sx, sy , sx, sy ) ) goto FALSE_;
	
	for(t=0; t<=distance; t++)
	{
		xerr += delta_x;
		yerr += delta_y;
		if(xerr >= distance)    {        xerr -= distance;       sx += incx;      }
		if(yerr >= distance)    {        yerr -= distance;       sy += incy;      }
		if( !FreeTile( ch, sx, sy , sx, sy ) ) goto FALSE_;
	}

	if( occupied_flag )	pT->occupied = 1;

	return 1;


FALSE_:

	if( occupied_flag )	pT->occupied = 1;

	return 0;
}	






















////////////////////////////////////////////////////////////






// int onefxpal, int old_color, int new_color)
/*
int PaintPath(int x, int y, int ex, int ey )
{
	struct s_v{
		int  x,
			 y;
	} s_val, c_val;
		
	int stack_add[640];
	int ri,rj,i,j,first,sp;
		
	sp=0;
		
	s_val.x = x;
	s_val.y = y;
	sp += 2;
		
	stack_add[sp] = s_val.x;
	stack_add[sp+1] = s_val.y;
	while(sp!=0){
		c_val.x = stack_add[sp];
		c_val.y = stack_add[sp+1];
		sp -= 2;
		
		i = c_val.x;
		j = c_val.y;
		
		while( FreeTile( NULL, i,j, i,j) ) i++;
		ri = i - 1;
		first = 1;
		i = ri;
		
		while( FreeTile( NULL, i,j, i,j) ){
			//PutPixel(i,j,new_color);
			if( FreeTile( NULL, i,j+1, i,j+1) ) first = 1;
			else{
				if((first == 1) && ((GetPixel(i,j+1)) != new_color)){
					s_val.x = i;
					s_val.y = j+1;
					sp +=  2;

					stack_add[sp] = s_val.x;
					stack_add[sp+1] = s_val.y;
					first = 0;
				}
			}
			i --;
		}
		rj = i;

		first = 1;
		i = ri;

		while(rj<i){
			if((GetPixel(i,j-1)) != old_color) first = 1;
			else{
				if((first == 1) && ((GetPixel(i,j-1)) !=new_color)){
					s_val.x = i;
					s_val.y = j-1;
					sp += 2;
					stack_add[sp] = s_val.x;
					stack_add[sp+1] = s_val.y;
					first = 0;
				}
			}
			i --;
		}
	}
}*/

