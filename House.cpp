#include "stdafx.h"
#include <Ddraw.h>
#include <Math.h>
	
	
#include "sys/stat.h"

#include "Dragon.h"	
#include "Hong_Sprite.h"
#include "Hong_Sub.h"
#include "char.h"  
#include "map.h"
#include "Path.h"

#include "DirectSound.h"
#include "CharDataTable.h"

#include "Effect.h"

//#include "MenuDef.h"
#include "ItemTable.h"

#include "NetWork.h"
#include "GameProc.h"
#include "Hangul.h"
#include "Tool.h"
#include "House.h"


ABOUTHOUSE			Housing;	
int					CurOpenHouse;



////////////////////////////////////////////////////////////////////////
///
///		Functions Declaration..
///
void SendRoofCloseHouse( int roofno );
void SendRoofCloseHouse( int roofno );
void RecvRoofOpen( int roofno );


////////////////////////////////////////////////////////////////
///
///		User Functions.
///

// Character의 발생장소가  정해져 있기 때문에 
// 집안에게 LogOut해도, 다음에 LogIn할때 지붕을 열어놓을 필요가 없다. ( 2000.1.22 )

void SendRoofOpenHouse( int roofno )
{
	t_packet p;

//	return;/////////////  2000.1.22
	
	p.h.header.type		= CMD_ROOF_OPEN;
	p.u.roof_open.roofno= roofno;
	p.h.header.size		= sizeof( t_roof_open );
	
	QueuePacket( &p, 1 );
}
void SendRoofCloseHouse( int roofno )
{
	t_packet p;

//	return;/////////////  2000.1.22
	
	p.h.header.type		  = CMD_ROOF_CLOSE;
	p.u.roof_close.roofno = roofno;
	p.h.header.size		  = sizeof( t_roof_close );
	
	QueuePacket( &p, 1 );
}

/*
case CMD_ROOF_OPEN :  RecvRoofOpen( p.u.server_roof_open.roofno );
	break;
*/

// 010505 KHS
void RecvRoofOpen( int roofno )
{
	LPROOFGROUP		lpRoofGroup;
	LPROOF			lpRoof;
	int				mapno;

	if( roofno == -1 ) return;

	mapno = roofno >> 8 ;
	if( mapno != MapNumber ) return;

	if( Hero )
		if( TileMap[ Hero->x/TILE_SIZE ][ Hero->y/TILE_SIZE ].attr_inside == 0 ) return;

	roofno = roofno & 0xff;
	lpRoofGroup = FindRoofGroup( &g_RoofHeader, roofno );
	if ( lpRoofGroup != NULL )
	{
		lpRoof = lpRoofGroup->lpFirst;
		while ( lpRoof )
		{
			TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 1;
			lpRoof = lpRoof->lpNext;
		}
	}
}

