/*		

*/		
		
#include "stdafx.h"
#include <Stdio.h>
#include "io.h"
		
		
#include "map.h"
#include "Hong_Sprite.h"
#include "Hong_Sub.h"
		
#include "Char.h"
#include "GameProc.h"
#include "Convert565to555.h"
#include "Tool.h"
#include "Hangul.h"
#include "Hong_Light.h"
#include "Menu.h"
#include "Resource.h"
#include "SkillTool.h"
#include "MouseCursor.h"
#include "DirectSound.h"
#include "House.h"
#include "CharDataTable.h"
#include "SmallMenu.h"

#include "crtdbg.h"
#include "LottoMgr.h"//soto-030501
		
		
MAP					g_Map;
TILE				TileMap[ VILLAGE_SIZE ][ VILLAGE_SIZE ];
ROOFHEADER			g_RoofHeader;
EVENTLISTHEADER		g_EventListHeaderMap;
EVENTLISTHEADER		g_EventListHeaderRoom;
EVENTLISTHEADER		g_EventListHeaderMusic;
		
Spr Sou[ TILE_MAX_];
BYTE *SouBuf;


///////////////////// 0430 lkh 추가 ///////////////////////////
Spr		SouHouse[ HOUSETILE_MAX_];
BYTE*	SouHouseBuf;
///////////////////////////////////////////////////////////////


char MapName[ NM_LENGTH];

int LimitMapSx;
int LimitMapSy;
int LimitMapEx;
int LimitMapEy;

int OldEventMx, OldEventMy, EventMx, EventMy;		


int StartMapx, StartMapy, StartMapFlag;

int  MapNumber;

///////////// SoundUp lkh 추가 /////////////
BOOL	g_BGE_Flag = 0;

int  MapNumberX, MapNumberY;
int  MapNumberFloor;
char MapNumberName[ 20];

int  TotalTileNo;



//////////////////////// 0430 lkh 추가 ///////////////////////////
int	TotalHouseTileNo;


int SmallMapX = 100,  SmallMapY = 100;





	
///////////////////////////////////////////////////////////////////////////////
//		
		
BOOL	MapBuild( LPMAP lpMap, char *filename );
void	MapDestroy( LPMAP lpMap );
BOOL	MapCreateSurface( LPDIRECTDRAW lpDirectDraw, LPMAP lpMap );
void	MapCleanupSurface( LPMAP lpMap );
BOOL	MapRestoreSurface( LPMAP lpMap );
BOOL	MapLoadImage( LPDIRECTDRAW lpDirectDraw, LPMAP lpMap, char* lpszFilePath );
BOOL	MapReLoadImage( LPDIRECTDRAWSURFACE* lplpSurface, char* lpszFilePath );
BOOL	MapReadFile( LPMAPFILE lpMapFile, char* pszFileName );
BOOL	MapWirteFile( LPMAPFILE lpMapFile, char* pszFileName );
void	MapDisplay( int nType );

		
BOOL		BuildRoofHeader( LPROOFHEADER lpRoofHeader, char* lpszFileName );
void		DestroyRoofHeader( LPROOFHEADER lpRoofHeader );
LPROOFGROUP	AddRoofGroup( LPROOFHEADER lpRoofHeader );
void		DeleteRoofGroup( LPROOFHEADER lpRoofHeader, LPROOFGROUP lpRoofGroup );
LPROOF		AddRoof( LPROOFGROUP lpRoofGroup, WORD x, WORD y );
void		DeleteRoof( LPROOFGROUP lpRoofGroup, LPROOF lpRoof );
LPROOFGROUP	FindRoofGroup( LPROOFHEADER lpRoofHeader, WORD index );
LPROOF		FindRoof( LPROOFGROUP lpRoofGroup, WORD x, WORD y );
BOOL		ReadRoofData( LPROOFHEADER lpRoofHeader, char* lpszFileName );
		
BOOL		BuildEventList( char* lpszFileName );
void		DestroyEventList();
LPEVENTLIST	AddEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y, WORD wIndex );
void		DeleteEventList( LPEVENTLISTHEADER lpEventListHeader, LPEVENTLIST lpEventList );
LPEVENTLIST	FindEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y );
BOOL		ReadEventList( char* lpszFileName );

DWORD		ReturnSizeOfSource2( char *source2name );
BOOL		IsScreenArea( LPCHARACTER ch );
BOOL		IsScreenArea_( POINT position );
void		SetView( LPCHARACTER ch, int range );
int			MoveScreen( DIRECTION direction );

extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419

///////////////////////////////////////////////////////////////////////////////
//

void LoadMapInfo()			// 010310 YGI
{
	FILE *fp;
	fp = Fopen( "./data/map_info.bin", "rb" );
	if( !fp ) return;

	fread( MapInfo, sizeof(t_MapInfo), MAX_MAP_, fp );
	fclose( fp );
}

int GetIndexByPort( int port )
{
	for( int i=0; i<MAX_MAP_; i++ )
	{
		if( MapInfo[i].port == port )
		{
			return i;
		}
	}
	return -1;
}
int GetIndexByMapfile( char *map_file )
{
	EatRearWhiteChar(map_file );
	CharUpper( map_file );
	for( int i=0; i<MAX_MAP_; i++ )
	{
		if( strcmp( MapInfo[i].mapfile, map_file ) == 0 )
		{
			return i;
		}
	}
	return -1;
}

char *GetMapNameByIndex( int i )
{
	if( i==-1 ) return NULL;
	return MapInfo[i].mapname;
}

char *GetMapNameByPort( int port )
{
	return GetMapNameByIndex( GetIndexByPort( port ) );
}
char *GetMapNameByMapfile( char *map_file )
{
	return GetMapNameByIndex( GetIndexByMapfile( map_file ) );
}

// 010724 KHS
BOOL MapBuild( LPMAP lpMap, char *mapname )
{	
	lpMap->wScreenWidth = 20;
	lpMap->wScreenHeight = 11;
	char szFileName[ FILENAME_MAX];
	int  c;

	CharUpper( mapname );
	for( c =0; MapInfo[c].mapfile[0] ; c ++)
	{
		if( strcmp( mapname, MapInfo[c].mapfile ) == 0 )
		{	//< CSD-030509
			strcpy( MapNumberName,MapInfo[c].mapname);	
			MapNumber = c; 
			break;
		}	//> CSD-030509
	}
	if( !MapInfo[c].mapname[0] ) Error( lan->OutputMessage(3,133), mapname );//010216 lsw
		
		
	InitWeather();
		
	wsprintf( szFileName, "%s/map/%s.map", GetCurrentWorkingDirectory( ), MapName );
	if ( !MapReadFile( &lpMap->file, szFileName ) )
	{	
		Error( "[%s] LoadError!", szFileName );
		return	FALSE;
	}	

	for( int i = 0 ; i < g_Map.file.wHeight ; i ++)
		for( int j = 0 ;  j < g_Map.file.wWidth ; j ++)
		{
			TileMap[ j ][ i].show_roof = 0;
			TileMap[ j ][ i].occupied  = 0;
		}

		
	LoadSkillMapTable();
		
	return	TRUE;
}	




	
void MapDestroy( LPMAP lpMap )
{	
	MapCleanupSurface( lpMap );
}	
	
BOOL MapCreateSurface( LPDIRECTDRAW lpDirectDraw, LPMAP lpMap )
{	
	lpMap->SurfaceInfo.lpSurface = CreateSurface( lpDirectDraw, SCREEN_WIDTH, SCREEN_HEIGHT );
	if ( lpMap->SurfaceInfo.lpSurface == NULL )
	{
		return	FALSE;
	}
	
	return	TRUE;
}	
	
void MapCleanupSurface( LPMAP lpMap )
{	
	if ( lpMap->SurfaceInfo.lpSurface != NULL )
	{
		CleanupSurface( lpMap->SurfaceInfo.lpSurface );
		lpMap->SurfaceInfo.lpSurface = NULL;
	}
}	
	
BOOL MapRestoreSurface( LPMAP lpMap )
{	
//	char	szFileName[ _MAX_PATH ];
		
//	wsprintf( szFileName, "%s/art/back%04d.bmp", __szCurrentDirectory, __GameInfo.nStageMajor );
//	if ( !ReLoadMapImage( &lpMap->lpSurfaceBack, szFileName ) )
//	{
//		return	FALSE;
//	}
	
//	wsprintf( szFileName, "%s/art/tile%04d.bmp", __szCurrentDirectory, __GameInfo.nStageMajor );
//	if ( !ReLoadMapImage( &lpMap->lpSurfaceTile, szFileName ) )
//	{
//		return	FALSE;
//	}
	
	return	TRUE;
}		
	
BOOL MapLoadImage( LPDIRECTDRAW lpDirectDraw, LPMAP lpMap, char* lpszFilePath )
{	
	return	DDLoadBitmap( lpDirectDraw, &lpMap->SurfaceInfo, lpszFilePath );
}			
		
BOOL MapReLoadImage( LPDIRECTDRAWSURFACE* lplpSurface, char* lpszFilePath )
{		
	return	DDReLoadBitmap( *lplpSurface, lpszFilePath );
}		
		
BOOL MapReadFile( LPMAPFILE lpMapFile, char* pszFileName )
{		
	FILE *fp;
	int		t = 0, t1;
	double *cc = NULL;
	DWORD	ver;	
	
	if( fp = Fopen( pszFileName, "rb" ) )
	{
		fread( &ver, sizeof( ver ), 1, fp );
		if( ver == 0xffff )
		{	
			fread( &lpMapFile->wType, sizeof( lpMapFile->wType ), 1, fp );
			fread( &lpMapFile->wWidth, sizeof( lpMapFile->wWidth ), 1, fp );
			fread( &lpMapFile->wHeight, sizeof( lpMapFile->wHeight ), 1, fp );
			
			for ( int y = 0; y < lpMapFile->wHeight; y++ )
			{
				for ( int x = 0; x < lpMapFile->wWidth; x++ )
				{
					fread( &TileMap[ x ][ y ], sizeof( TILE ), 1, fp );

#ifdef _DEBUG //Eleval
			
					cc = (double*)&TileMap[ x][ y];
					*cc -= (x+y);
					cc = ((double*)&TileMap[ x][ y]) + 1;
					*cc -= (y);
#endif
					t += *(int*)&TileMap[ x][ y];
				}
				HandleRunning(&connections );
			}
			
			fread( &t1, sizeof( t ), 1, fp );
			CheckSumError =  t1 - t;
			fread( &lpMapFile->cOdd, sizeof( lpMapFile->cOdd ), 1, fp );
			fread( &lpMapFile->cEven, sizeof( lpMapFile->cEven ), 1, fp );
		}	
		else
		{	
			fseek( fp, 0, SEEK_SET );
			
			fread( &lpMapFile->wType, sizeof( lpMapFile->wType ), 1, fp );

			fread( &lpMapFile->wWidth, sizeof( lpMapFile->wWidth ), 1, fp );

			fread( &lpMapFile->wHeight, sizeof( lpMapFile->wHeight ), 1, fp );

			
			for ( int y = 0; y < lpMapFile->wHeight ; y++ )
			{
				for ( int x = 0; x < lpMapFile->wWidth ; x++ )
				{
					fread( &TileMap[ x ][ y ], sizeof( TILE ), 1, fp );
				}
				HandleRunning(&connections );
			}
			fread( &lpMapFile->cOdd, sizeof( lpMapFile->cOdd ), 1, fp );
			fread( &lpMapFile->cEven, sizeof( lpMapFile->cEven ), 1, fp );
		}
		fclose( fp );
	}
	
	return	TRUE;
}		
		
		
BOOL MapWirteFile( LPMAPFILE lpMapFile, char* pszFileName )
{	
	FILE   *fp;
	int		t = 0;
	int     count=0;
	DWORD	ver = 0xffff;
		
//	JustMsg( " 저장할수 없습니다. 전 실행화일을이용하세요..." );
//	return FALSE;
		
	DWORD fileattr = GetFileAttributes( pszFileName );
	if( fileattr & FILE_ATTRIBUTE_READONLY )
	{	
		JustMsg( lan->OutputMessage(6,17) );//010216 lsw
	}	
		
	fp = Fopen( pszFileName, "wb" );
	{	
		fwrite( (char *)&lpMapFile->wType, sizeof( lpMapFile->wType ), 1, fp );
		fwrite( (char *)&lpMapFile->wWidth, sizeof( lpMapFile->wWidth ), 1, fp );
		fwrite( (char *)&lpMapFile->wHeight, sizeof( lpMapFile->wHeight ), 1, fp );
		
		for ( int y = 0 ; y < lpMapFile->wHeight ; y++ )
		{
			for ( int x = 0 ; x < lpMapFile->wWidth ; x++ )
			{
				fwrite( (char *)&TileMap[ x][ y], sizeof( TILE ), 1, fp );
			}
		}
		
		
		fwrite( (char *)&lpMapFile->cOdd, sizeof( lpMapFile->cOdd ), 1, fp );
		fwrite( (char *)&lpMapFile->cEven, sizeof( lpMapFile->cEven ), 1, fp );
				
		fclose( fp );
	}			
				
				
	strcat( pszFileName, "C" );
	fp = Fopen( pszFileName, "wb" );
	{			
		fwrite( (char *)&ver, sizeof( ver ), 1, fp );
		fwrite( (char *)&lpMapFile->wType, sizeof( lpMapFile->wType ), 1, fp );
		fwrite( (char *)&lpMapFile->wWidth, sizeof( lpMapFile->wWidth ), 1, fp );
		fwrite( (char *)&lpMapFile->wHeight, sizeof( lpMapFile->wHeight ), 1, fp );
				
		for ( int y = 0 ; y < lpMapFile->wHeight ; y++ )
		{		
			for ( int x = 0 ; x < lpMapFile->wWidth ; x++ )
			{	
				t += *(int*)&TileMap[ x][ y];
//				cc = (double*)&TileMap[ x][ y];
//				*cc += (x+y);
//				cc = ((double*)&TileMap[ x][ y]) + 1;
//				*cc += (y);
				fwrite( (char *)&TileMap[ x][ y], sizeof( TILE ), 1, fp );
			}	
		}		
				
		fwrite( (char *)&t, sizeof( t ), 1, fp );
		fwrite( (char *)&lpMapFile->cOdd, sizeof( lpMapFile->cOdd ), 1, fp );
		fwrite( (char *)&lpMapFile->cEven, sizeof( lpMapFile->cEven ), 1, fp );
				
		fclose( fp );
	}				
				
				
	for ( int y = 0; y < lpMapFile->wHeight; y++ )
	{			
		for ( int x = 0; x < lpMapFile->wWidth; x++ )
		{		
//			cc = (double*)&TileMap[ x][ y];
//			*cc -= (x+y);
//			cc = ((double*)&TileMap[ x][ y]) + 1;
//			*cc -= (y);
		}	
	}		
						
	return	TRUE;
}							
							
							

			

					
				
void  MapDisplay( int nType )
{
	if( g_StartMenuOn ) return;			// 0201 YGI

	register int	x, y;
	int				nIndex;
	LPTILE			t;
	int				mapx, mapy;
	int				temp_X,temp_Y;
	
	int				mapTileWidth,mapTileHeight;
	int				displayPoint_X=0,displayPoint_Y=0;

	if(g_Map.oX==0)
	{
		mapx		= g_Map.x;
		displayPoint_X = 0;
		mapTileWidth= SCREEN_WIDTH;		//640
	}
	else if(g_Map.oX > 0)
	{
		mapx		= g_Map.x - 1;
		displayPoint_X = -32+g_Map.oX;
		mapTileWidth= displayPoint_X + (SCREEN_WIDTH+TILE_SIZE);	//672
	}
	else		//음수(좌측)
	{
		mapx		= g_Map.x;
		displayPoint_X = g_Map.oX;
		mapTileWidth= displayPoint_X + (SCREEN_WIDTH+TILE_SIZE);
	}
	temp_X			= mapx;
	
	if(g_Map.oY==0)
	{
		mapy		 = g_Map.y;
		displayPoint_Y = 0;
		mapTileHeight= GAME_SCREEN_YSIZE;
	}
	else if(g_Map.oY > 0)
	{
		mapy		 = g_Map.y - 1;
		displayPoint_Y = -32+g_Map.oY;
		mapTileHeight= displayPoint_Y + (GAME_SCREEN_YSIZE+TILE_SIZE);
	}
	else
	{
		mapy		 = g_Map.y;
		displayPoint_Y = g_Map.oY;
		mapTileHeight= displayPoint_Y + (GAME_SCREEN_YSIZE+TILE_SIZE);//GAME_SCREEN_YSIZE+TILE_SIZE;
	}
	temp_Y			 = mapy;

	//PutTile( -8, -8, Sou[ TileMap[ mapx ][ mapy ].back ].img, g_DestBackBuf );
	//PutTile( 0, 32, Sou[ TileMap[ mapx ][ mapy ].back ].img, g_DestBackBuf );

	for ( y = displayPoint_Y; y < mapTileHeight ; y += TILE_SIZE, mapy ++ )
	{	
		for ( x = displayPoint_X; x < mapTileWidth ; x += TILE_SIZE, mapx ++ )
		{
			t = &TileMap[ mapx ][ mapy ];
			switch( nType )
			{
			case 0 :
			{
#ifdef _DEBUG
				if( tool_DrawMap ) return;
#endif
				if ( nType == 0 )
				{
					// 바닥
					if ( t->roof == 0	||	 t->show_roof != 0	||	 t->rooftype == 1 )
					{			
						{		
							nIndex = t->back;
							if ( nIndex && nIndex < TotalTileNo )
							{	
								if( Sou[nIndex].xl == -1 )
								{
									//PutTileCliping(  x , y, Sou[ nIndex].img, g_DestBackBuf , 0, 32, 0, 32 );
									PutTile( x, y, Sou[ nIndex].img, g_DestBackBuf );
									//if(g_GameInfo.displayOption==1)
									//	PutTile( x+rand()%5-2 , y+rand()%5-2, Sou[ nIndex].img, g_DestBackBuf );
								}
								else 
								{
									PutCompressedImage( x, y, &Sou[nIndex] );
									//if(g_GameInfo.displayOption==1)
									//	PutCompressedImage( x+rand()%5-2, y+rand()%5-2, &Sou[nIndex] );
								}
							}
						}
					}
				}
				break;
			}	
			case 1 :	// 지붕
			{	
				nIndex = t->roof;
				if ( nIndex && nIndex < TotalTileNo )
				{
					if ( t->show_roof == 0 )
					{
						if( Sou[nIndex].xl == -1 )
						{
							PutTile( x, y, Sou[ nIndex].img, g_DestBackBuf );
						//if(g_GameInfo.displayOption==1)
							//	PutTile( x+rand()%5-2, y+rand()%5-2, Sou[ nIndex].img, g_DestBackBuf );
						}
						else 
						{
							PutCompressedImage( x, y, &Sou[nIndex] );
							//if(g_GameInfo.displayOption==1)
							//	PutCompressedImage( x+rand()%5-2, y+rand()%5-2, &Sou[nIndex] );
						}
					}
				}
				break;
			}	
			case 2 :
			{	// 벽
#ifdef _DEBUG
				if( tool_DrawFrontMap ) return;
#endif					
				if ( t->roof == 0	||	 t->show_roof || t->rooftype == 1 )
				{		
					nIndex = t->front;
					if ( nIndex && nIndex < TotalTileNo )
					{	
						if( Sou[nIndex].xl == -1 )
								PutTile( x, y, Sou[ nIndex].img, g_DestBackBuf );
						else 
								PutCompressedImage( x, y, &Sou[nIndex] );
					}	
				}
				break;
			}	
			}// switch
		}
		mapx = temp_X;
	}
}		
			
			
		
//////////////////////////////////////////////////////
/////  View Mapattr..

// 010314 KHS 함수전체... .SKB의 NPC속성에서 겹쳐져 있는 것도 표현도록 하였다. 
void  MapDisplay_Attr( void )
{			
	register int	x, y;
	int				mapx, mapy;
	int sx, sy;
	static int to;
	int range_X;
	int range_Y;
		
	
#ifndef _DEBUG
	return;
#endif					

	if( tool_ViewAttrRoofOnOff  ||
		tool_ViewAttrRiver		||
		tool_ViewAttrGEffect	||
		tool_ViewAttrOccupied	||
		tool_ViewAttrInside		||
		tool_ViewAttrNoBattle	||
		tool_ViewAttrCanMatch	||
		tool_ViewAttrEventFlag  ||
		tool_ViewAttrDontFlag	||
		tool_ViewAttrFrontFlag	||
		tool_ID_INPUT_MAP_ATTR  ||
		tool_ID_SKILL_INPUT		||
		tool_ViewAttrTop		||			//	010906 LTS
		tool_ID_INPUT_MYHOUSE
		) goto  GO_PROCESS_;

	 return;

GO_PROCESS_:
		
	 to = !to;
		
	 LPEVENTLIST		lpEventList;
	mapy = g_Map.y;
	for ( y = 0; y < GAME_SCREEN_YSIZE ; y += TILE_SIZE, mapy ++ )
	{	
		for ( mapx = g_Map.x, x = 0; x < GAME_SCREEN_XSIZE ; x+= TILE_SIZE, mapx ++ )
		{
			if( tool_ViewAttrRoofOnOff )
			if( TileMap[ mapx ][ mapy ].attr_room )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx+4, sy+4, sx + TILE_SIZE - 8, sy + TILE_SIZE - 8, RGB16( 0,255,0 ));
				
				lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )mapx, ( WORD )mapy );
				if( lpEventList )
				{		
					if( TileMap[ mapx ][ mapy ].attr_room  == 1 ) // 열어라....
					{
						Hcolor( 0 );
						Hprint2( sx- Mapx, sy-Mapy, g_DestBackBuf, lan->OutputMessage(7,10), lpEventList->index );//010216 lsw
						Hcolor( 0,255,0 );
						Hprint2( sx-Mapx-1, sy-Mapy-1, g_DestBackBuf, lan->OutputMessage(7,10), lpEventList->index );//010216 lsw
					}
					else 
					{
						Hcolor( 0 );
						Hprint2( sx- Mapx, sy-Mapy, g_DestBackBuf, lan->OutputMessage(7,29), lpEventList->index );//010216 lsw
						Hcolor( 255,255,0 );
						Hprint2( sx-Mapx-1, sy-Mapy-1, g_DestBackBuf, lan->OutputMessage(7,29), lpEventList->index );//010216 lsw
					}
				}
			}
			if( tool_ViewAttrRiver )
			if( TileMap[ mapx ][ mapy ].attr_river )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx, sy, sx + TILE_SIZE, sy + TILE_SIZE, RGB16( 0,100,200 ));
			}
			if( tool_ViewAttrGEffect )
			if( TileMap[ mapx ][ mapy ].attr_light )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx +2 , sy+2, sx + TILE_SIZE-4, sy + TILE_SIZE-4, RGB16( 255,255,255 ));
			}
			if( tool_ViewAttrEventFlag )
			if( TileMap[ mapx ][ mapy ].attr_map )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx+3, sy+3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16( 0,0,255 ));
				Hprint2( sx-Mapx, sy-Mapy, g_DestBackBuf, "%d", ReturnEventNo( mapx, mapy ));
			}		
			if( tool_ViewAttrInside )
			if( TileMap[ mapx ][ mapy ].attr_inside )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx+3, sy+3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16( 255,100,100 ));
			}		
			if( tool_ViewAttrNoBattle )
			if( TileMap[ mapx ][ mapy ].attr_no_battle )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx+3, sy+3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16( 100,255,100 ));
			}		
			if( tool_ViewAttrCanMatch )
			if( TileMap[ mapx ][ mapy ].canmatch )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx+3, sy+3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16( 100,0,100 ));
			}		

			//___________________________________________________________________	// 010904 LTS
			if( tool_ViewAttrTop )			// 국가전에서의 높낮이 타일.
			if( TileMap[ mapx ][ mapy ].empty__4 )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx+4, sy+4, sx + TILE_SIZE - 5, sy + TILE_SIZE - 5, RGB16( 255,255,0 ));
			}		

			if( tool_ViewAttrOccupied )		// 길찾기에서의  점유타일 
			if( TileMap[ mapx ][ mapy ].occupied )
			{
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
				Box( sx+3, sy+3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16( 0, 255,100 ));
			}		
			//___________________________________________________________________	// 010904 LTS


			if( tool_ViewAttrDontFlag )
			{
				if( TileMap[ mapx ][ mapy ].occupied )
				{
					sx = mapx*TILE_SIZE;
					sy = mapy*TILE_SIZE;
					Box( sx+3, sy+3, sx + TILE_SIZE - 6, sy + TILE_SIZE - 6, RGB16( 0, 255,100 ));
				}		


				if( TileMap[ mapx ][ mapy ].attr_dont )
				{		
					sx = mapx*TILE_SIZE;
					sy = mapy*TILE_SIZE;
					Box( sx+1, sy+1, sx + 30, sy + 30, RGB16( 255,0,0 ));
				}		
			}
			if( tool_ViewAttrFrontFlag )
			if( TileMap[ mapx ][ mapy ].front )
			{		
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;

				if( to && tMapAttr_Offset_Modify == IDC_ATTR_OFFSET_YES2 && tMapAttrX == mapx && tMapAttrY == mapy )
				{	
					
				}	
				else 
				{	
					Box( sx+5, sy+5, sx + 24, sy + 24, RGB16( 255,0,255 ));
				}	
				Hcolor( 0xffff );
				Hprint2( sx-Mapx, sy-Mapy+10, g_DestBackBuf, "%3d", TileMap[ mapx ][ mapy ].frontoffset );
			}		



			
		
			if( tool_ID_INPUT_MYHOUSE )
			{
				//MouseProcess( Message, wParam, lParam);
				//마우스 버튼 클릭 드래그되다가 버튼 up시 
				//Box(g_DragMouse.sx,g_DragMouse.sy,g_DragMouse.ex,g_DragMouse.ey, RGB16(255,100,100));

				int select_Tile_sx=(g_DragMouse.sx)/TILE_SIZE;//(g_DragMouse.sx+32-mapx)/TILE_SIZE;
				int select_Tile_sy=(g_DragMouse.sy)/TILE_SIZE;//(g_DragMouse.sy+32-mapy)/TILE_SIZE;
				int select_Tile_ex=(g_DragMouse.ex)/TILE_SIZE;//(g_DragMouse.ex-mapx)/TILE_SIZE;
				int select_Tile_ey=(g_DragMouse.ey)/TILE_SIZE;//(g_DragMouse.ey-mapy)/TILE_SIZE;
				
				int temp_XY;
				int temp_sx=select_Tile_sx;
				int temp_sy=select_Tile_sy;
				int temp_ex=select_Tile_ex;
				int temp_ey=select_Tile_ey;
		
				if(temp_sx>temp_ex)	
				{
					temp_XY=temp_sx;
					temp_sx=temp_ex;
					temp_ex=temp_XY;
				}
				if(temp_sy>temp_ey)
				{
					temp_XY=temp_sy;
					temp_sy=temp_ey;
					temp_ey=temp_XY;
				}
				
				g_MyhouseTool.sx=temp_sx;
				g_MyhouseTool.sy=temp_sy;
				g_MyhouseTool.ex=temp_ex;
				g_MyhouseTool.ey=temp_ey;
				g_MyhouseTool.object_Num=(temp_ex-temp_sx)*(temp_ey-temp_sy);

				char	temp[FILENAME_MAX];
				if(g_MyhouseDlgOpen==1)
				{
					sprintf (temp, "%d", g_MyhouseTool.sx);			Edit_SetText (GetDlgItem(InputMyhouseHdlg, IDC_VIEW_SX ), temp);
					sprintf (temp, "%d", g_MyhouseTool.sy);			Edit_SetText (GetDlgItem(InputMyhouseHdlg, IDC_VIEW_SY ), temp);
					sprintf (temp, "%d", g_MyhouseTool.ex);			Edit_SetText (GetDlgItem(InputMyhouseHdlg, IDC_VIEW_EX ), temp);
					sprintf (temp, "%d", g_MyhouseTool.ey);			Edit_SetText (GetDlgItem(InputMyhouseHdlg, IDC_VIEW_EY ), temp);
					sprintf (temp, "%d", g_MyhouseTool.object_Num);	Edit_SetText (GetDlgItem(InputMyhouseHdlg, IDC_VIEW_TILENUM ), temp);
				}
				
				Box( temp_sx*TILE_SIZE,temp_sy*TILE_SIZE,temp_ex*TILE_SIZE,temp_ey*TILE_SIZE,RGB16(255,50,50));
			}
		

			lpMAPSKILLTABLE	result;
			DWORD		lineColor;
			int nx, ny;

			nx = (mapx-g_Map.x)*TILE_SIZE;
			ny = (mapy-g_Map.y)*TILE_SIZE;
			
			if( tool_ID_SKILL_INPUT )
			if( TileMap[ mapx ][ mapy ].attr_skill )
			{
				int count = 0;
				sx = mapx*TILE_SIZE;
				sy = mapy*TILE_SIZE;
			
				while( (result=FindSkill(&Header[(int)(mapx/(int)((g_Map.file.wWidth+7)/8))][(int)(mapy/(int)((g_Map.file.wHeight+7)/8))],mapx,mapy, count) ) )
				if(result!=NULL)
				{
					count ++;
					switch(result->skillno)
					{
						case TOOL_DONTSKILL :
							Hcolor(150,255,150);
							Hprint2( (mapx-g_Map.x)*32+2, (mapy-g_Map.y)*32+10, g_DestBackBuf, "Don't");
							Box( sx+2, sy+2, sx + TILE_SIZE - 2, sy + TILE_SIZE - 2, RGB16( 255,50,50));	
							break;
						case TOOL_MINING :
						{
							switch(result->subType)
							{
							case 0:	lineColor=RGB16(203,185,156);	break;
							case 1:	lineColor=RGB16(158,139,117);	break;
							case 2:	lineColor=RGB16(118,100,85);	break;
							case 3:	lineColor=RGB16(82,68,60);		break;
							case 4:	lineColor=RGB16(51,42,41);		break;
							}
							break;
						}
						case TOOL_FARMING :
						{
							switch(result->subType)
							{
							case 0:	lineColor=RGB16(221,189,140);			break;
							case 1:	lineColor=RGB16(200,164,110);			break;
							case 2:	lineColor=RGB16(169,131,79);			break;
							case 3:	lineColor=RGB16(132,92,42);				break;
							case 4:	lineColor=RGB16(91,55,20);				break;
							case 5:	lineColor=RGB16(165,84,5);				break;
							case 6:	lineColor=RGB16(155,77,0);				break;
							case 7:	lineColor=RGB16(197,79,0);				break;
							case 8:	lineColor=RGB16(74,33,6);				break;
							case 9:	lineColor=RGB16(55,23,2);				break;
							}
							break;
						}
						case TOOL_HUB :
						{
							switch(result->subType)
							{
							case 0:	lineColor=RGB16(137,202,157);		break;
							case 1:	lineColor=RGB16(47,180,114);		break;
							case 2:	lineColor=RGB16(0,105,63);			break;
							case 3:	lineColor=RGB16(0,75,44);			break;
							case 4:	lineColor=RGB16(31,64,50);			break;
							case 5:	lineColor=RGB16(66,109,91);			break;
							case 6:	lineColor=RGB16(82,132,19);			break;
							case 7:	lineColor=RGB16(60,102,8);			break;
							case 8:	lineColor=RGB16(103,181,4);			break;
							case 9:	lineColor=RGB16(35,52,14);			break;
							}
							break;
						}
						case TOOL_NPC_GENER :
							lineColor=RGB16(50,50,255);
							break;
						case TOOL_BUILDHOUSE :
							lineColor=RGB16(0,0,0);
							break;
					}

					if(result->skillno!=TOOL_DONTSKILL)
					{

						{
							int xl, yl;
							xl = (result->tile_Range*2*TILE_SIZE)+TILE_SIZE;
							yl = (result->tile_Range*2*TILE_SIZE)+TILE_SIZE;
							range_X=(mapx-result->tile_Range)*TILE_SIZE;
							range_Y=(mapy-result->tile_Range)*TILE_SIZE;

							Box( sx+1, sy+1, sx + TILE_SIZE - 1, sy + TILE_SIZE - 1, RGB16( 255,255,0));
							switch(radio_Statue)
							{
							case 0:
									Box( range_X, range_Y, xl, yl, lineColor);
								break;
							case 1:
								if(result->tile_Range!=0 && result->skillno==TOOL_FARMING)
											Box( range_X, range_Y, xl, yl, lineColor);
								break;
							case 2:
								if(result->tile_Range!=0 && result->skillno==TOOL_MINING)
									Box( range_X, range_Y, xl, yl, lineColor);
								break;
							case 3:
								if(result->tile_Range!=0 && result->skillno==TOOL_FISHING)
									Box( range_X, range_Y, xl, yl, lineColor);
								break;
							case 4:
								if(result->tile_Range!=0 && result->skillno==TOOL_CHOPPING)
									Box( range_X, range_Y, xl, yl, lineColor);
								break;
							case 5:
								if(result->tile_Range!=0 && result->skillno==TOOL_HUB)	
									Box( range_X, range_Y, xl, yl, lineColor);
								break;
							case 6:

								break;
							case 7:
								if(result->tile_Range!=0 && result->skillno==TOOL_BUILDHOUSE)
									Box( range_X, range_Y, xl, yl, lineColor);
								break;
							}
						}

						if( result->skillno == 6 )
						{
							// 유효범위(발생수)  성공확률(Script)   NPC고유번호	하위관련Data 
							static char othername[ 6][20]={" ", "Enchant", "Mad", "Boss"," ", " " };
							HprintBold( nx, ny,		RGB(200,200,200), RGB16(0,0,0 ), lan->OutputMessage(7,35), result->tile_Range , result->probability );//010216 lsw

							int tmon;
							int tenchant;
							//< CSD-030419
							if (result->type_Num < MAX_CHARACTER_SPRITE_)
							{ 
								tmon = result->type_Num;		
								tenchant = 0; 
							}
							else
							{ 
								tmon = result->type_Num%100;	
								tenchant = result->type_Num/100; 
							}
							//> CSD-030419
							HprintBold( nx, ny+16,  RGB(200,50 * tenchant + 50,50 * tenchant + 50 ), RGB16(0,0,0 ), "%d:%s %s", result->type_Num, othername[tenchant], g_infNpc[tmon].szName); // CSD-030419
						}
						else
						{
							HprintBold( nx, ny,		RGB(200,200,200), RGB16(0,0,0 ), "%d-%d-%d-%d", result->skillno, result->subType, result->tile_Range, result->probability);
						}
					}
				}

				if( count > 1 )	HprintBold( nx - 40, ny, RGB(200,200,200), RGB16(0,0,0 ), "<<%d>>", count );
			}
		}			
	}
}


	
	
///////////////////////////////////////////////////////////////////////////////
	
///////////////////////////////////////////////////////////////////////////////
// roof
	
BOOL
BuildRoofHeader( LPROOFHEADER lpRoofHeader, char* mapname )
{	
	char lpszFileName[ FILENAME_MAX];

	lpRoofHeader->lpFirst = lpRoofHeader->lpLast = NULL;
	lpRoofHeader->count = 0;

	wsprintf( lpszFileName, "%s/event/%s.rof", GetCurrentWorkingDirectory( ), MapName );

	FILE *fp;
	fp = Fopen( lpszFileName, "rb" );
	if( fp == NULL ) return FALSE;
	
	if ( !ReadRoofData( lpRoofHeader, lpszFileName ) )
	{
		Error( "[%s] \n LoadError", lpszFileName );
		return	FALSE;
	}
	
	return	TRUE;
}
	
// 지붕 이벤트의 xy를 가지고 있는 리스트에서 값을 찾아 그 포인트를 넘겨 준다.
// 031110 YGI
LPROOFGROUP
FindRoofGroupByXY( LPROOFHEADER lpRoofHeader, int x, int y )
{
	LPROOFGROUP		lpRoofGroup, lpRoofGroupNext;
	LPROOF			lpRoof, lpRoofNext;

	lpRoofGroup = lpRoofGroupNext = lpRoofHeader->lpFirst;

	while ( lpRoofGroup )
	{
		lpRoof = lpRoofNext = lpRoofGroup->lpFirst;
		while ( lpRoof )
		{
			if( lpRoof->x == x && lpRoof->y == y ) 
			{
				return lpRoofGroup;
			}
			lpRoofNext = lpRoof->lpNext;
			lpRoof = lpRoofNext;
		}

		lpRoofGroupNext = lpRoofGroup->lpNext;
		lpRoofGroup = lpRoofGroupNext;
	}
	return NULL;
}
	
void
DestroyRoofHeader( LPROOFHEADER lpRoofHeader )
{
	LPROOFGROUP		lpRoofGroup, lpRoofGroupNext;
	LPROOF			lpRoof, lpRoofNext;

	lpRoofGroup = lpRoofGroupNext = lpRoofHeader->lpFirst;

	while ( lpRoofGroup )
	{
		lpRoof = lpRoofNext = lpRoofGroup->lpFirst;

		while ( lpRoof )
		{
			lpRoofNext = lpRoof->lpNext;
			DeleteRoof( lpRoofGroup, lpRoof );
			lpRoof = lpRoofNext;
		}

		lpRoofGroupNext = lpRoofGroup->lpNext;
		DeleteRoofGroup( lpRoofHeader, lpRoofGroup );
		lpRoofGroup = lpRoofGroupNext;
	}
}

LPROOFGROUP  AddRoofGroup( LPROOFHEADER lpRoofHeader )
{
	LPROOFGROUP		lpRoofGroup;

	if ( ( lpRoofGroup = ( LPROOFGROUP )GlobalAlloc( GPTR, sizeof( ROOFGROUP ) ) ) == ( LPROOFGROUP )NULL )
	{
		return	NULL;
	}

	if ( lpRoofHeader->lpLast == NULL )
	{
		lpRoofGroup->lpPrev = lpRoofGroup->lpNext = NULL;
		lpRoofHeader->lpFirst = lpRoofHeader->lpLast = lpRoofGroup;
	}
	else
	{
		lpRoofGroup->lpPrev = lpRoofHeader->lpLast;
		lpRoofGroup->lpNext = NULL;

		lpRoofHeader->lpLast->lpNext = lpRoofGroup;
		lpRoofHeader->lpLast = lpRoofGroup;
	}
	lpRoofHeader->count++;

	return	lpRoofGroup;
}

void
DeleteRoofGroup( LPROOFHEADER lpRoofHeader, LPROOFGROUP lpRoofGroup )
{
	if ( lpRoofGroup != NULL )
	{
		LPROOF		lpRoof, lpRoofNext;
		
		lpRoof = lpRoofNext = lpRoofGroup->lpFirst;
		while( lpRoof )
		{
			lpRoofNext = lpRoof->lpNext;
			DeleteRoof( lpRoofGroup, lpRoof );
			lpRoof = lpRoofNext;
		}

		if ( lpRoofGroup->lpPrev != NULL )
		{
			lpRoofGroup->lpPrev->lpNext = lpRoofGroup->lpNext;
		}
		if ( lpRoofGroup->lpNext != NULL )
		{
			lpRoofGroup->lpNext->lpPrev = lpRoofGroup->lpPrev;
		}

		if ( lpRoofHeader->lpLast == lpRoofGroup )
		{
			lpRoofHeader->lpLast = lpRoofGroup->lpPrev;
		}
		if ( lpRoofHeader->lpFirst == lpRoofGroup )
		{
			lpRoofHeader->lpFirst = lpRoofGroup->lpNext;
		}

		lpRoofHeader->count--;

		GlobalFree( lpRoofGroup );
		lpRoofGroup = NULL;
	}
}

LPROOF
AddRoof( LPROOFGROUP lpRoofGroup, WORD x, WORD y )
{
	LPROOF		lpRoof;

	if ( ( lpRoof = ( LPROOF )GlobalAlloc( GPTR, sizeof( ROOF ) ) ) == ( LPROOF )NULL )
	{
		return	NULL;
	}

	lpRoof->x = x;
	lpRoof->y = y;

	if ( lpRoofGroup->lpLast == NULL )
	{
		lpRoof->lpPrev = lpRoof->lpNext = NULL;
		lpRoofGroup->lpFirst = lpRoofGroup->lpLast = lpRoof;
	}
	else
	{
		lpRoof->lpPrev = lpRoofGroup->lpLast;
		lpRoof->lpNext = NULL;

		lpRoofGroup->lpLast->lpNext = lpRoof;
		lpRoofGroup->lpLast = lpRoof;
	}
	lpRoofGroup->count++;

	return	lpRoof;
}

void
DeleteRoof( LPROOFGROUP lpRoofGroup, LPROOF lpRoof )
{
	if ( lpRoof != NULL )
	{
		if ( lpRoof->lpPrev != NULL )
		{
			lpRoof->lpPrev->lpNext = lpRoof->lpNext;
		}
		if ( lpRoof->lpNext != NULL )
		{
			lpRoof->lpNext->lpPrev = lpRoof->lpPrev;
		}

		if ( lpRoofGroup->lpLast == lpRoof )
		{
			lpRoofGroup->lpLast = lpRoof->lpPrev;
		}
		if ( lpRoofGroup->lpFirst == lpRoof )
		{
			lpRoofGroup->lpFirst = lpRoof->lpNext;
		}

		lpRoofGroup->count--;

		GlobalFree( lpRoof );
		lpRoof = NULL;
	}
}

LPROOFGROUP
FindRoofGroup( LPROOFHEADER lpRoofHeader, WORD index )
{
	LPROOFGROUP		lpRoofGroup = lpRoofHeader->lpFirst;
	int				nCount = 0;

	while ( lpRoofGroup )
	{
		if ( nCount++ == index )
		{
			return	lpRoofGroup;
		}

		lpRoofGroup = lpRoofGroup->lpNext;
	}

	return	NULL;
}

LPROOF
FindRoof( LPROOFGROUP lpRoofGroup, WORD x, WORD y )
{
	LPROOF		lpRoof = lpRoofGroup->lpFirst;

	while ( lpRoof )
	{
		if ( lpRoof->x == x && lpRoof->y == y )
		{
			return	lpRoof;
		}

		lpRoof = lpRoof->lpNext;
	}

	return	NULL;
}

BOOL
ReadRoofData( LPROOFHEADER lpRoofHeader, char* lpszFileName )
{
	FILE *fp;
	WORD	headercount, groupcount;
	WORD	x, y;

	fp = Fopen( lpszFileName, "rb" );
	{
		fread( &headercount, sizeof( headercount ), 1, fp );
		for ( int i = 0; i < headercount; i++ )
		{
			fread( &groupcount, sizeof( groupcount ), 1, fp );

			AddRoofGroup( lpRoofHeader );

			for ( int j = 0; j < groupcount; j++ )
			{
				fread( &x, sizeof( x ), 1, fp );
				fread( &y, sizeof( y ), 1, fp );

				AddRoof( lpRoofHeader->lpLast, x, y );
			}
		}

		fclose( fp );
		return	TRUE;
	}

	return	FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// event

BOOL
BuildEventList( char* mapname )
{
	char szFileName[ FILENAME_MAX];

	wsprintf( szFileName, "%s/event/%s.ent", GetCurrentWorkingDirectory( ), MapName );
	g_EventListHeaderMap.lpFirst = g_EventListHeaderMap.lpLast = NULL;
	g_EventListHeaderMap.wCount = 0;

	g_EventListHeaderRoom.lpFirst = g_EventListHeaderRoom.lpLast = NULL;
	g_EventListHeaderRoom.wCount = 0;

	g_EventListHeaderMusic.lpFirst = g_EventListHeaderMusic.lpLast = NULL;
	g_EventListHeaderMusic.wCount = 0;

	if ( !ReadEventList( szFileName ) )
	{
		return	FALSE;
	}
	
	return	TRUE;
}	
	
void
DestroyEventListSub( LPEVENTLISTHEADER lpEventListHeader )
{	
	LPEVENTLIST		lpEventList, lpEventListNext;
	
	lpEventList = lpEventListNext = lpEventListHeader->lpFirst;
	while ( lpEventList )
	{
		lpEventListNext = lpEventList->lpNext;
		DeleteEventList( lpEventListHeader, lpEventList );
		lpEventList = lpEventListNext;
	}
}	
	
	
	
void DestroyEventList( void )
{	
	DestroyEventListSub( &g_EventListHeaderMap );
	DestroyEventListSub( &g_EventListHeaderRoom );
	DestroyEventListSub( &g_EventListHeaderMusic);
	
}	
	
	
LPEVENTLIST
AddEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y, WORD wIndex )
{	
	LPEVENTLIST		lpEventList;
	
	if ( ( lpEventList = ( LPEVENTLIST )GlobalAlloc( GPTR, sizeof( EVENTLIST ) ) ) == ( LPEVENTLIST )NULL )
	{
		return	NULL;
	}
	
	if ( lpEventListHeader->lpLast == NULL )
	{
		lpEventList->lpPrev = lpEventList->lpNext = NULL;
		lpEventListHeader->lpFirst = lpEventListHeader->lpLast = lpEventList;
	}
	else
	{
		lpEventList->lpPrev = lpEventListHeader->lpLast;
		lpEventList->lpNext = NULL;
		lpEventListHeader->lpLast->lpNext = lpEventList;
		lpEventListHeader->lpLast = lpEventList;
	}
	lpEventListHeader->wCount++;
	
	lpEventList->x = x;
	lpEventList->y = y;
	lpEventList->index = wIndex;
	
	return	lpEventList;
}	
	
void
DeleteEventList( LPEVENTLISTHEADER lpEventListHeader, LPEVENTLIST lpEventList )
{	
	if ( lpEventList != NULL )
	{
		if ( lpEventList->lpPrev != NULL )
		{
			lpEventList->lpPrev->lpNext = lpEventList->lpNext;
		}
		if ( lpEventList->lpNext != NULL )
		{
			lpEventList->lpNext->lpPrev = lpEventList->lpPrev;
		}
	
		if ( lpEventListHeader->lpFirst == lpEventList )
		{
			lpEventListHeader->lpFirst = lpEventList->lpNext;
		}
		if ( lpEventListHeader->lpLast == lpEventList )
		{
			lpEventListHeader->lpLast = lpEventList->lpPrev;
		}
	
		lpEventListHeader->wCount--;
	
		GlobalFree( lpEventList );
		lpEventList = NULL;
	}
}	
	
LPEVENTLIST
FindEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y )
{	
	LPEVENTLIST		lpEventList = lpEventListHeader->lpFirst;
	
	while ( lpEventList )
	{
		if ( lpEventList->x == x && lpEventList->y == y )
		{
			return	lpEventList;
		}
	
		lpEventList = lpEventList->lpNext;
	}
	
	return	NULL;
}	
	
BOOL
ReadEventList( char* lpszFileName )
{		
	FILE	*fp;
	WORD	count;
	WORD	x, y;
	WORD	index;
	int		i;
		
	fp = Fopen( lpszFileName, "rb" );
	if( fp )
	{	
		fread( &count, sizeof( WORD ), 1, fp );
		for ( i = 0; i < count; i++ )
		{
			fread( &x, sizeof( WORD ) , 1, fp);
			fread( &y, sizeof( WORD ) , 1, fp);
			fread( &index, sizeof( WORD ) , 1, fp);
		
			AddEventList( &g_EventListHeaderMap, x, y, index );
		}
		
		fread( &count, sizeof( WORD ), 1,fp );
		for ( i = 0; i < count; i++ )
		{
			fread( &x, sizeof( WORD ) , 1, fp);
			fread( &y, sizeof( WORD ) , 1, fp);
			fread( &index, sizeof( WORD ) , 1, fp);
		
			AddEventList( &g_EventListHeaderRoom, x, y, index );
		}
		
		fread( &count, sizeof( WORD ), 1, fp);
		for ( i = 0; i < count; i++ )
		{
			fread( &x, sizeof( WORD ), 1, fp);
			fread( &y, sizeof( WORD ), 1, fp);
			fread( &index, sizeof( WORD ), 1, fp);
		
			AddEventList( &g_EventListHeaderMusic, x, y, index );
		}
		
		fclose( fp );
		return	TRUE;
	}		
		
	return	FALSE;
}			
	
	
	
	
	
	
	
// ---------------------------------------마우스에 의한 Map Scroll...
	
	
BOOL IsScreenArea_( POINT position )
{	
	return	PtInRect( &g_GameInfo.rectScreen, position );
}	

BOOL IsScreenArea( LPCHARACTER ch )
{	
	Spr *sp = ch->sp;
	DWORD	mx = ch->position.x;
	DWORD	my = ch->position.y;
	
  if (ch->sprno==50||ch->sprno==51||ch->sprno==52)	// LTS DRAGON BUG
	{
		ch->direction=DIRECTION_DOWN;
		sp=ch->sp=&CharSpr[ ch->sprno ].sp[ ch->direction ][ch->nCurrentFrame.GetDecrypted()];
	}

	if( sp == NULL ) return 0;

	if( mx < 0 ) return 0;
	if( my < 0 ) return 0;
	if( mx > g_Map.file.wWidth  ) return 0;
	if( my > g_Map.file.wHeight ) return 0;
	
	if( g_Inside != TileMap[ mx ][ my ].attr_inside ) return 0;

	int t0 =	BoxAndBoxCrash(	Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, ch->x - sp->ox, ch->y - sp->oy-ch->height, sp->xl, sp->yl );
	int t1 =	BoxAndBoxCrash(	Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, ch->x - sp->ox, ch->y - sp->oy, sp->xl, sp->yl );

	if( t0 || t1 ) return 1;
	
	return 0;
}		

BOOL IsScreenAreaMapObject( int x, int y, SIZE size )
{	
	int xl =  size.cx;
	int yl =  size.cy;
	
	if( BoxAndBoxCrash(	Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, 	x, y, xl, yl ) ) return 1;
	
	return 0;
}	

BOOL IsScreenAreaCSP( int x, int y, Spr *sp )
{	
	int xl = sp->xl;
	int yl = sp->yl;
	if( BoxAndBoxCrash(	Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, x-sp->ox, y-sp->oy, xl, yl ) ) return 1;
	return 0;
}

///////////////////////////// 0808 lkh 추가 ////////////////////////////////////
BOOL IsScreenAreaView( int x, int y )		//오브젝트의 중점 좌표
{
	if( BoxAndDotCrash(	Hero->x-(OBJECTSOUNDPLAY_XSIZE/2), Hero->y-(OBJECTSOUNDPLAY_YSIZE/2), OBJECTSOUNDPLAY_XSIZE, OBJECTSOUNDPLAY_YSIZE, x, y) ) return 1;
	return 0;
}
	
///////////////////////// 0625 lkh 수정 /////////////////////////////
void SetView( LPCHARACTER ch, int range )
{	
	int mx,  my;
		
	ch->sight = range;

	int rangex = range;
	int rangey = range-2;
		
	mx = ch->position.x - (GAME_SCREEN_XSIZE/32/2)-1;		//10
	my = ch->position.y - (GAME_SCREEN_YSIZE/32/2)-1;		//7
	
	if( g_Map.x < g_Map.tox ) g_Map.x ++;
	if( g_Map.x > g_Map.tox ) g_Map.x --;
	if( g_Map.y < g_Map.toy ) g_Map.y ++;
	if( g_Map.y > g_Map.toy ) g_Map.y --;

////////////////////////
////////////////////////

	if( g_Map.x < 0 ) g_Map.x = 0;
	if( g_Map.y < 0 ) g_Map.y = 0;

	
	LimitMapSx = mx - rangex+1;	if( LimitMapSx  < 0 ) LimitMapSx = 0;
	LimitMapSy = my - rangey+1;	if( LimitMapSy  < 0 ) LimitMapSy = 0;
	LimitMapEx = mx + rangex;	if( LimitMapEx  > g_Map.file.wWidth  - (GAME_SCREEN_XSIZE/32) )  LimitMapEx = g_Map.file.wWidth  - (GAME_SCREEN_XSIZE/32) ;
	LimitMapEy = my + rangey;	if( LimitMapEy  > g_Map.file.wHeight - (GAME_SCREEN_YSIZE/32)-1 )  LimitMapEy = g_Map.file.wHeight - (GAME_SCREEN_YSIZE/32) -1;
	
	if( g_Map.tox < LimitMapSx ) g_Map.tox = LimitMapSx;
	if( g_Map.tox > LimitMapEx ) g_Map.tox = LimitMapEx;
	
	if( g_Map.toy < LimitMapSy ) g_Map.toy = LimitMapSy;
	if( g_Map.toy > LimitMapEy ) g_Map.toy = LimitMapEy;
}	



// 메뉴가 뜨면 그에 따라 카메라의 취치를 설정한다. 	
//////////////////////////////// 0625 lkh 수정 ////////////////////////////////////
// 메뉴가 뜨면 그에 따라 카메라의 취치를 설정한다. 	
void SetHeroPosition( int flag )		// 0110
{	
	int lc = 0, rc = 0;
	int guild=0;
	static WORD changed;
	WORD t = 0;

	int sm[] = {	// 0626_2 YGI
		MN_BLACKSMITH, MN_BLACKSMITH, MN_FARM, MN_RESTAURANT, MN_HERBSHOP, MN_SHAMBLES, MN_CANDLE, MN_SLOPSHOP, 
		MN_ALCHEMY, MN_WOODWORKING, MN_BOWCRAFT, MN_SHIPYARD, MN_SHOP_MAIN, MN_DRINK_MAIN, MN_INN_MAIN, 
		MN_MAGIC_GUILD, MN_ACADEMY, MN_TOWNHALL ,MN_HEAL_MAIN,MN_SKILLGUILD_SKILL, MN_SKILLGUILD_KNOWLAGE, 
		MN_SKILLGUILD_ITEM, MN_MAIL_WRITE,
		MN_MAIL_MENU, MN_MAIL_READ, MN_BANK_DEPOSIT, MN_BANK_DEFRAYAL, MN_BANK_MAIN, MN_BANK_LOAN,
		MN_BANK_REPAYMENT, MN_BANK_AUCTION, MN_BANK_AUCTION_SALE, MN_DRINK_RUMOR, MN_HEAL_HEALING, 
		MN_HEAL_PREVENTING, MN_HEAL_LEARN, MN_INN_REST, MN_INN_SLEEP,
	};
	int sm_max = sizeof( sm ) / sizeof( int);


	if( Hero == NULL ) return;



	t |= ( SMenu[ MN_ITEM           ].bActive ) <<  0;
	t |= ( SMenu[ MN_ARIGEMENT		].bActive ==TRUE ) <<  7;
	t |= ( SMenu[ MN_ABILITY		].bActive ==TRUE ) <<  8;
	t |= ( SMenu[ MN_STATUS			].bActive ==TRUE ) <<  9;
	t |= ( SMenu[ MN_PARTY			].bActive ==TRUE ) << 10;
	t |= ( SMenu[ MN_TACTICS		].bActive ==TRUE ) << 11;

	for( int i=0; i<sm_max; i++ )
		if( SMenu[sm[i]].bActive ) 
		{
			t |= 1 << 12;
			guild=1;
			break;
		}
		else t|= 0 << 12;
	
	if( flag == 0 && changed == t )  return;
	changed = t;
	
	if(	SMenu[ MN_ITEM          ].bActive ==TRUE ||
		SMenu[ MN_PARTY			].bActive ==TRUE ||
		guild )		rc = 1;
	
	if(  SMenu[ MN_TACTICS		].bActive ==TRUE ||
		 SMenu[ MN_ARIGEMENT	].bActive ==TRUE ||
		 SMenu[ MN_ABILITY		].bActive ==TRUE ||
		 SMenu[ MN_STATUS		].bActive ==TRUE )		lc = 2;

	POINT center_Tile;
	center_Tile.x = (GAME_SCREEN_XSIZE/32/2);
	center_Tile.y = (GAME_SCREEN_YSIZE/32/2);
	switch( rc + lc )
	{
	case 3 :
	case 0 :	g_Map.tox = Hero->position.x - center_Tile.x;
				g_Map.toy = Hero->position.y - center_Tile.y;

				if( g_Map.tox < 0 ) g_Map.tox = 0;
				if( g_Map.toy < 0 ) g_Map.toy = 0;

				SetView( Hero, Hero->sight );
			return;
	case 1 : 	g_Map.tox = Hero->position.x -  center_Tile.x/2;
				g_Map.toy = Hero->position.y -  center_Tile.y;

				if( g_Map.tox < 0 ) g_Map.tox = 0;
				if( g_Map.toy < 0 ) g_Map.toy = 0;

				SetView( Hero, Hero->sight );
			return;
	case 2 :	g_Map.tox = Hero->position.x - ((center_Tile.x*2) - (center_Tile.x/2));
				g_Map.toy = Hero->position.y -  center_Tile.y;

				if( g_Map.tox < 0 ) g_Map.tox = 0;
				if( g_Map.toy < 0 ) g_Map.toy = 0;

				SetView( Hero, Hero->sight );
			return;


	
	default : return;
	}
}




//////////////////////// 0625 lkh 수정 //////////////////////////
void StartSetView( LPCHARACTER ch )
{	
	g_Map.tox = g_Map.x = ch->position.x - (GAME_SCREEN_XSIZE/32/2);
	g_Map.toy = g_Map.y = ch->position.y - (GAME_SCREEN_YSIZE/32/2);
				
	SetView( ch, ch->sight );
}



int MoveScreen( DIRECTION direction )
{	
	int oldtox, oldtoy;

	oldtox = g_Map.tox;
	oldtoy = g_Map.toy;

	switch ( direction )
	{
	case	DIRECTION_UP:		if ( g_Map.toy > LimitMapSy  )	g_Map.toy--;		break;
	case	DIRECTION_DOWN:		if ( g_Map.toy < LimitMapEy  )	g_Map.toy++;		break;
	case	DIRECTION_LEFT:		if ( g_Map.tox > LimitMapSx  )	g_Map.tox--;		break;
	case	DIRECTION_RIGHT:	if ( g_Map.tox < LimitMapEx  )	g_Map.tox++;		break;
	}

	if( oldtox != g_Map.tox || oldtoy != g_Map.toy )
	{
		return 1;
	}
	
	return 0;
}					

void changemapobjectname( char *oldname, char *newname )
{	
	strcpy( newname, oldname );
	CharUpper( newname );

	if( strcmp( "ICE-W02", newname ) == 0 )			strcpy( newname, "ICE-W01" );
	else if(  strcmp( "ICE-W03", newname )  == 0 )	strcpy( newname, "ICE-W01" );
	else if(  strcmp( "MANDUN2", newname )  == 0 )	strcpy( newname, "MANDUN1" );
	else if(  strcmp( "MANDUN3", newname )  == 0 )	strcpy( newname, "MANDUN1" );
	else if(  strcmp( "FIREDUN2", newname ) == 0 )	strcpy( newname, "FIREDUN1");
	else if(  strcmp( "FIREDUN3", newname ) == 0 )	strcpy( newname, "FIREDUN1");
	else if(  strcmp( "HU_VM", newname )    == 0 )	strcpy( newname, "SOURCE" );
	else if(  strcmp( "GRAY", newname )     == 0 )	strcpy( newname, "SOURCE" );

#ifdef _MAPOBJECT_DIRECTORY_CHANGE_

			 if( strcmp( "MA-IN", newname ) == 0 )				strcpy( newname, "0" );	
		else if(  strcmp( "SOURCE", newname ) == 0 )			strcpy( newname, "6" );
		else if(  strcmp( "K_SUNG2", newname ) == 0 )			strcpy( newname, "7" );
		else if(  strcmp( "FIREDUN1", newname ) == 0 )			strcpy( newname, "4" );
		else if(  strcmp( "ICE-W01", newname ) == 0 )			strcpy( newname, "3" );
		else if(  strcmp( "MANDUN1", newname ) == 0 )			strcpy( newname, "5" );
		else if(  strcmp( "SUNG_TILE_00", newname ) == 0 )		strcpy( newname, "2" );

		else													strcpy( newname, "0" );

#endif 

}		

void changesourcename( char *oldname, char *newname )
{		
	strcpy( newname, oldname );
	CharUpper( newname );
		
	if( strcmp( "ICE-W02", newname ) == 0 )			strcpy( newname, "ICE-W01" );
	else if(  strcmp( "ICE-W03", newname ) == 0 )	strcpy( newname, "ICE-W01" );
	else if(  strcmp( "MANDUN2", newname ) == 0 )	strcpy( newname, "MANDUN1" );
	else if(  strcmp( "MANDUN3", newname ) == 0 )	strcpy( newname, "MANDUN1" );
	else if(  strcmp( "FIREDUN2", newname ) == 0 )	strcpy( newname, "FIREDUN1" );
	else if(  strcmp( "FIREDUN3", newname ) == 0 )	strcpy( newname, "FIREDUN1" );
}		


/*
void LoadSmallMap( char *filename )
{
	FILE *fp;
	char temp[ FILENAME_MAX];
	Spr *s;

	switch( MapNumber )
	{
	case 0 :	break;
	default :	return;
	}

	sprintf( temp, "./map/%s.sml", filename );
	fp = Fopen( temp, "rb" );
	if( fp )
	{	
		s = &g_Map.sp;
		fread( &s->xl, sizeof( short ), 1, fp);
		fread( &s->yl, sizeof( short ), 1, fp);
		fread( &s->ox, sizeof( short ), 1, fp);
		fread( &s->oy, sizeof( short ), 1, fp);
//		s->ox = s->oy = 0;
		fread( &s->size, sizeof( unsigned int ), 1, fp);
		s->img = NULL;
		MemAlloc( s->img, s->size );
		fread( s->img, s->size, 1, fp );
		convert565to555( s );
		fclose(fp);
	}	
}*/


		
void LoadMapSource( char *filename )
{			
	char temp[ FILENAME_MAX];
	short no;
	int i;
	Spr *s;
	DWORD sysfilelength, size;
	FILE *fp;
	DWORD nOff = 0;
	char tempsource[ FILENAME_MAX];
		
	changesourcename( filename, tempsource );
		
	sprintf( temp, "./map/%s.sou", tempsource );
	fp = Fopen( temp, "rb" );
	if( fp == NULL )
	{
		return; 
	}
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR );  // 오프셋 스킵
	sysfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;
		
	TotalTileNo = no;
	MemAlloc( SouBuf, sysfilelength );



		
	for( i = 0 ; i < no ; i++)
	{	
		fread( &size, 4,1, fp);
		s = &Sou[i];
		
		fread( &s->xl, sizeof( short ), 1, fp);
		fread( &s->yl, sizeof( short ), 1, fp);
		
		if( s->xl == -1 )
		{
			s->size = TILE_SIZE * TILE_SIZE * 2;
			s->ox = 0;
			s->oy = 0;
			fread( SouBuf + nOff, s->size,1 ,fp);
			s->img = (char *)SouBuf + nOff;
			convert565to555RawTile( s->img );
		}
		else
		{
			fread( &s->ox,	sizeof( short ), 1, fp);
			fread( &s->oy,	sizeof( short ), 1, fp);
			fread( &s->size, sizeof( unsigned int ), 1, fp);
			fread( SouBuf + nOff, s->size,1 ,fp);
			s->img = (char *)SouBuf + nOff;
			convert565to555( s );
		}
		
		nOff += size;
		if( (i%100) == 0 ) HandleRunning(&connections );
	}	
	fclose(fp);
		
	HandleRunning( &connections);

}			
		
void FreeMapSource( void )
{														
	MemFree( g_Map.sp.img );												
	MemFree( SouBuf );	
	
	for( int i = 0 ; i < 3600 ; i ++)
	{
		memset( &Sou[i], 0, sizeof( Spr ) );
	}
}																			
	
	
//#define MAX_SOURCE_NO_			10
#define MAX_SOURCE_NO_			11	// 031013 kyo 1.3
	
char mapsourcename[ MAX_SOURCE_NO_][ 20]=
	{ "MA-IN","K_SUNG2","MANDUN1","FIREDUN1","GRAY","SUNG_TILE_00","HU_VM","ICE-W01","SOURCE", "10", "11" };// 031013 kyo 1.3
	
typedef struct 
{
	char sourcename[ 30];
	int  size;
}t_sourcesize;

/////////////////////////////////////////////////////////////////////////////

int ReturnSizeOfSource_bin( char * source2name )
{
	int totalsize = 0;
	int s;

	CharUpper( source2name );

	FILE *fp = Fopen( "./data/sizeofsou2.bin", "rb" );
	if( fp == NULL ) return 0;

	t_sourcesize sousize;
	
	while( 1 )
	{
		s = fread( &sousize, sizeof( t_sourcesize ), 1, fp );
		if( s < 1 ) break;

		if( strcmp( source2name, sousize.sourcename ) == 0 ) { totalsize = sousize.size; break; }
	}
	
	fclose(fp);
	
	return totalsize;
}	
	
void MakeSizeofSOU2( void )
{	
	FILE *fp;
	t_sourcesize writesousize;
	char temp[ FILENAME_MAX];
	int i;
	int size;
	
	if( SysInfo.makesou2size == 0 ) return;
	
	fp = Fopen( "./data/sizeofsou2.bin", "wb" );
	if( fp == NULL ) return;
	
	for( i = 0 ; i < MAX_MAP_ ; i ++)
	{
		sprintf( temp, "./map/%s.sou2", MapInfo[i].mapfile );
		CharUpper( temp );
		size = ReturnSizeOfSource2( temp );
	
		strcpy( writesousize.sourcename, temp );
		writesousize.size = size;
		fwrite( &writesousize, sizeof( t_sourcesize ), 1, fp );
		JustMsg( lan->OutputMessage(6,18), i+1, MAX_MAP_, temp, size );//010216 lsw
	}
	
	fclose(fp);
}	
	

DWORD ReturnSizeOfSource2( char *source2name )
{	
	char tempsourcename[FILENAME_MAX];
	FILE *sou2fp;
	FILE *fp[ MAX_SOURCE_NO_];
	SOU2 sou2;
	DWORD totalsize = 0, size = 0;
	DWORD Off;
	
	sou2fp = Fopen( source2name, "rb" );
	if( sou2fp == NULL ) 
	{
		return 0;
	}
	
	int i;
	for( i = 0 ; i < MAX_SOURCE_NO_ ; i ++)	
	{																
		sprintf( tempsourcename, "./Map/%s.sou", mapsourcename[ i] );
		fp[i] = Fopen( tempsourcename, "rb" );
	}
	
	DWORD fs;
	DWORD no;
	
	fread( &fs, 4,1, sou2fp );
	fread( &no, 4,1, sou2fp );
	fseek( sou2fp, 6, SEEK_CUR );
	
	for( i = 0 ; i < no ; i++)
	{	
		short int tt,ts;
	
		fread( &tt,  sizeof( short int ), 1, sou2fp );	
		fread( &ts,  sizeof( short int ), 1, sou2fp );	
	
		sou2.tile_no = tt;
		sou2.source_no = ts-1;
	
		int source = sou2.source_no;
		int tile =   sou2.tile_no;
	
//		_ASSERT(  0 <= sou2.tile_no && sou2.tile_no < 3600 );					
//		_ASSERT(  0 <= sou2.source_no && sou2.source_no < MAX_SOURCE_NO_ );
	
		if(  0 <= sou2.tile_no && sou2.tile_no < 3600 ){}else continue;
		if(  0 <= sou2.source_no && sou2.source_no < MAX_SOURCE_NO_ ){}else continue;
	
		fseek(fp[source],	4 * tile + 2, SEEK_SET);
		fread( &Off, sizeof( DWORD ), 1, fp[ source] );
		fseek(fp[source], Off, SEEK_SET);
	
		fread( &size, sizeof( DWORD ), 1, fp[ source] );
		
		totalsize += size;

		
	}	
		
	fclose( sou2fp );
	for( i = 0 ; i < MAX_SOURCE_NO_ ; i ++)
		if( fp[ i] )
		{
			fclose( fp[i] );
		}
	
	return totalsize;
}	
	
void LoadMapSource2( char *mapname )
{	
	char  temp[ FILENAME_MAX];															
	DWORD no;																	
	int   i;																								
	Spr  *s;																				
	DWORD size;												
	FILE *fp[ MAX_SOURCE_NO_], *sou2fp;
	DWORD nOff = 0;																							
	char  tempsourcename[ FILENAME_MAX];											
	SOU2  sou2;													
	DWORD filesize;
	
	sprintf( temp, "./map/%s.sou2", mapname );
	CharUpper( temp );
	sou2fp = Fopen( temp, "rb" );
	if( sou2fp == NULL )
	{
		LoadMapSource( mapname );
		return;
	}
	
	for( i = 0 ; i < MAX_SOURCE_NO_ ; i ++)	
	{
		sprintf( tempsourcename, "./Map/%s.sou", mapsourcename[ i] );
		fp[i] = Fopen( tempsourcename, "rb" );
	}
	
	//filesize = ReturnSizeOfSource_bin( temp );
	//if( filesize == 0 )	
	filesize = ReturnSizeOfSource2( temp );
		
	DWORD fs;
	DWORD offset = 0;
	
	fread( &fs, 4,1, sou2fp );
	fread( &no, 4,1, sou2fp );
	fseek( sou2fp, 6, SEEK_CUR );

	MemAlloc( SouBuf, filesize );
																							
	for( i = 0 ;  i < no  ; i ++ )											
	{				
		short int tt,ts;
	
		fread( &tt,  sizeof( short int ), 1, sou2fp );	
		fread( &ts,  sizeof( short int ), 1, sou2fp );	

		sou2.tile_no = tt;
		sou2.source_no = ts-1;

//		_ASSERT(  0 <= sou2.tile_no && sou2.tile_no < 3600 );
//		_ASSERT(  0 <= sou2.source_no && sou2.source_no < MAX_SOURCE_NO_ );
		if(  0 <= sou2.tile_no && sou2.tile_no < 3600 ){}else { Sou[i].xl = 0; Sou[i].img = NULL; continue; }
		if(  0 <= sou2.source_no && sou2.source_no < MAX_SOURCE_NO_ ){}else { Sou[i].xl = 0; Sou[i].img = NULL; continue; }

		int source = sou2.source_no;
		int tile   = sou2.tile_no;

		if( tile == 0 ) continue;

		fseek(fp[source], 4 * tile + 2, SEEK_SET);
		fread( &offset, sizeof( DWORD ), 1, fp[ source] );
		fseek(fp[source], offset, SEEK_SET);

		fread( &size, sizeof( DWORD ), 1, fp[ source] );
		s = &Sou[i];

		fread( &s->xl, sizeof( short ), 1, fp[ source]);
		fread( &s->yl, sizeof( short ), 1, fp[ source]);

		if( s->xl == -1 )
		{
			s->size = TILE_SIZE * TILE_SIZE * 2;
			s->ox = 0;
			s->oy = 0;
			fread( SouBuf + nOff, s->size,1 ,fp[ source]);
			s->img = (char *)SouBuf + nOff;
			convert565to555RawTile( s->img );
		}
		else
		{
			fread( &s->ox,	sizeof( short ), 1, fp[ source]);
			fread( &s->oy,	sizeof( short ), 1, fp[ source]);
			fread( &s->size, sizeof( unsigned int ), 1, fp[ source]);
			fread( SouBuf + nOff, s->size,1 ,fp[ source]);
			s->img = (char *)SouBuf + nOff;	
			convert565to555( s );
		}

		nOff += size;

		if( (i%100) == 0 ) HandleRunning(&connections );
	}
	TotalTileNo = i;
	
	fclose(sou2fp);

	for( i = 0 ; i < MAX_SOURCE_NO_ ; i ++)	
	{
		if( fp[i] ) fclose( fp[i] );
	}
}		
		
		
void FreeMapSource2( void )
{		
	MemFree( g_Map.sp.img );
	MemFree( SouBuf );
}		


/////////////////////////////////////////////////////////////////////////////


int ReturnEventNo( int mx, int my )
{
	LPEVENTLIST	lpEventList;

	if( TileMap[ mx][ my ].attr_map )
	{
		lpEventList = FindEventList( &g_EventListHeaderMap, ( WORD )mx, ( WORD )my );
		if( lpEventList == NULL ) return -1;
		return lpEventList->index;
	}

	return -1;
}


int ChangeEventNo( int mx, int my, int eventno )
{				
	LPEVENTLIST	lpEventList;
				
	if( TileMap[ mx][ my ].attr_map )
	{			
		lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )mx, ( WORD )my );
		lpEventList->index = eventno;
				
		return TRUE;
	}			
				
	return FALSE;
}				
				
				
////////////////////////////////////////////////////////////////////////////
/////   Event가  발생되는가를 check한다. 
						
int CheckEventWith_NPC( LPCHARACTER hero, LPCHARACTER oppo )
{										
	DIRECTION dir = (DIRECTION )0;	
			
	
	if( hero->eventdelay ) 
	{
		PlayListAutoSounds( 82,0,0,0 ); 
		return 1;
	}

	if( IsDead( oppo ) ) return 0; // 죽어있는것과는 Event가 발생하지 않는다. 

	switch( oppo->type )			
	{												
		case SPRITETYPE_MONSTER :	
		case SPRITETYPE_NPC		:	
		{
			if( oppo->sealstone )
			{
			}
			else 
			{
				if( oppo->sprno != 100 && oppo->sprno != 101 )
				{
					ChangeDirection( &dir, oppo->x, oppo->y, hero->x, hero->y );
					oppo->todir = dir;
					ChangeDirection( &dir, hero->x, hero->y, oppo->x, oppo->y );
					hero->todir = dir;
				}

				SendEvent( EVENT_TYPE_NPC, oppo->id );
				hero->eventdelay = 15;
				
			}
			return 1;				
		}break;								
	}												
														
	return 0;
}			
			
			
			
int CheckEventAttrTile( void )
{
	int mx = Hero->x / TILE_SIZE;
	int my = Hero->y / TILE_SIZE;
	int eventno;
	static int OntheEventTile;

	if( TileMap[ mx][my].attr_map )
	{
		if( OntheEventTile == 0 )
		{
			OntheEventTile = 1;
			eventno = ReturnEventNo( mx, my );
			if( eventno != -1 )
			{
//				JustMsg( "int CheckEventAttr( void ) %d", eventno );
				SendEvent( EVENT_TYPE_MAPTILE, eventno );

				return 1;
			}
		}
	}
	else 
	{
		OntheEventTile = 0;
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

Spr SmallMapBackSpr;

void LoadSmallMapBack( void )
{	
	FILE *fp;
	Spr *s;
	
	fp = Fopen( "./map/add.csp", "rb" );
	if( fp == NULL ) return;
	
	s = &SmallMapBackSpr;
	fread( &s->xl, sizeof( short ), 1, fp);
	fread( &s->yl, sizeof( short ), 1, fp);
	fread( &s->ox, sizeof( short ), 1, fp);
	fread( &s->oy, sizeof( short ), 1, fp);
	s->ox = s->oy = 0;
	fread( &s->size, sizeof( unsigned int ), 1, fp);
	s->img = NULL;
	MemAlloc( s->img, s->size );
	fread( s->img, s->size, 1, fp );
	convert565to555( s );
	fclose(fp);
}	
	
void PutSmallMapBack( int x, int y )
{	
	PutCompressedImage( x, y, &SmallMapBackSpr );
}	
	

	
#define SmallMapXL		125
#define SmallMapYL		77





BYTE ZXTABLE[ SCREEN_WIDTH];
BYTE ZYTABLE[ SCREEN_HEIGHT];
	
void MakeZoomTable(int xl,int yl,int zx,int zy)
{	
	int xDivision,yDivision;
	int xRemain,yRemain;
	int i;

	xDivision = zx / xl;
	yDivision = zy / yl;

	xRemain = zx % xl;
	yRemain = zy % yl;

	memset(ZXTABLE,xDivision,xl);
	memset(ZYTABLE,yDivision,yl);

	for( i = 0 ; i < xRemain ; i ++ )
	  ZXTABLE[ (int)((long)i* xl / xRemain) ] ++;

	for( i = 0 ; i < yRemain ; i ++ )
	  ZYTABLE[(int)((long) i* yl / yRemain) ] ++;
}			
			
	

#define MAP_SMALL_SIZE_BAEYULX	8
#define MAP_SMALL_SIZE_BAEYULY	8
#define GAME_SCREEN_YSIZE_OLD		472
void OutputSmallMap( void )//020730 lsw
{			
	int yl = g_Map.file.wHeight * TILE_SIZE;
	int xl = g_Map.file.wWidth  * TILE_SIZE;
	int x, y;
	int curx, sx, sy;
	int ii, jj, xx, yy;

	xl = xl/ MAP_SMALL_SIZE_BAEYULX;
	yl = yl/ MAP_SMALL_SIZE_BAEYULX;
			
	MakeZoomTable( SCREEN_WIDTH, GAME_SCREEN_YSIZE_OLD, SCREEN_WIDTH/ MAP_SMALL_SIZE_BAEYULX, GAME_SCREEN_YSIZE_OLD/MAP_SMALL_SIZE_BAEYULX );
			
	const int  size=  xl * yl * 2 ;

	BYTE *buf = new BYTE[size + 17], *tb = buf;
		
	for ( y = 0; y < g_Map.file.wHeight ; y+= 15 )
	{		
		for( x = 0; x < g_Map.file.wWidth ; x+= 25 )
		{	
#ifndef _SDL

			g_DestBackBuf = GetSurfacePointer(g_DirectDrawInfo.lpDirectDrawSurfaceBack);
			EraseScreen(&g_DirectDrawInfo, RGB(0, 0, 0));
#endif // !_SDL

			g_Map.x = x;
			g_Map.y = y;
			Mapx = g_Map.x * TILE_SIZE;
			Mapy = g_Map.y * TILE_SIZE;

			smallmap_CalcOrder();
			small_DisplaySpriteUnderTile();
			MapDisplay(0);
			MapDisplay(2);
			//
			smallmap_DisplaySprite();
			MapDisplay(1);

////////////////////////////

			curx = sx = SCREEN_WIDTH / MAP_SMALL_SIZE_BAEYULX * (x/25);
			sy = GAME_SCREEN_YSIZE_OLD / MAP_SMALL_SIZE_BAEYULX * (y/15);
		
			tb = buf +( sx + sy * xl ) * 2;

			for ( yy = 0 ; yy < GAME_SCREEN_YSIZE_OLD ; yy++ )
			{
				for( jj= 0; jj < ZYTABLE[yy] ; jj ++)
				{
					for( xx = 0 ; xx < SCREEN_WIDTH ; xx ++ )
					{
						for( ii = 0 ; ii <  ZXTABLE[xx] ; ii++)
						{
							if( curx >= xl ) continue;
							*((WORD *)( buf + ( curx + sy * xl ) * 2 )) = 
								*(( WORD*)( g_DestBackBuf + (xx + yy * SCREEN_WIDTH) * 2 ));
							curx++;
						}
					}
		
					curx=sx;
					sy++;
					if( sy >= yl ) goto NEXT__;
				}
			}
		NEXT__:

#ifndef _SDL
			FlipScreen(&g_DirectDrawInfo);
#else // !_SDL
			SDL_FlipScreen();
#endif		
			
////////////////////////////
		}
	}

	char temp[ FILENAME_MAX];
	sprintf( temp, "./map/%s.bmp", MapName );
	FILE *fp = Fopen( temp, "wb" );

	if(NULL == fp) { return; }// 파일 로드 실패면 리턴
	
	BITMAPFILEHEADER BmpHeader;
	BmpHeader.bfType		=DIB_HEADER_MARKER;
	BmpHeader.bfSize		=xl*yl*sizeof(RGB24);
	BmpHeader.bfReserved1	=0;
	BmpHeader.bfReserved2	=0;
	BmpHeader.bfOffBits		=54;

	fwrite( &BmpHeader, sizeof( BITMAPFILEHEADER ), 1, fp );//비트맵 헤더를 읽는다
	
	BITMAPINFOHEADER	BmpInfoHead;

	BmpInfoHead.biSize			=40;
	BmpInfoHead.biWidth			=xl;
	BmpInfoHead.biHeight		=yl;
	BmpInfoHead.biPlanes		=1;
	BmpInfoHead.biBitCount		=24;
	BmpInfoHead.biCompression	=0;
	BmpInfoHead.biSizeImage		=xl*yl*sizeof(RGB24)-54;
	BmpInfoHead.biXPelsPerMeter	=0;
	BmpInfoHead.biYPelsPerMeter	=0;
	BmpInfoHead.biClrUsed		=0;
	BmpInfoHead.biClrImportant	=0;

	fwrite( &BmpInfoHead, sizeof(BITMAPINFOHEADER), 1, fp );//위에다 잡은 메모리에 비트맵 본체 로딩(위에서 헤더를 뜯었다 이미)

	RGBQUAD Pal[1] = {0,};
	fwrite( Pal, sizeof( RGBQUAD ), 1, fp );

	RGB24* p24DIB = new RGB24[xl*yl];
	
	unsigned char r,g,b;
	const int iPixelCount = xl * yl;

	WORD *p16DIB = (WORD *)(buf);
	for( int i = 0; yl > i ; i++)
	{
//		int iReadPos = (i*xl);
		int iWritePos = ((yl-i-1)*xl);
		for( int j = 0; xl > j ; j++)
		{
			const WORD cc = *(p16DIB++);
		
			if( _PixelInfo.BitMaskR != 0x7C00) // 555가 아니면.
			{
				r = (0xf800 & cc )>> 8;
				g = (0x07c0 & cc )>> 3;
				b = (0x001f & cc )<< 3;
			}	
			else 
			{	
				r = (0x7c00 & cc )>> 7;
				g = (0x03e0 & cc )>> 2;
				b = (0x001f & cc )<< 3;
			}	
				
			p24DIB[iWritePos].r = g;
			p24DIB[iWritePos].g = r;
			p24DIB[iWritePos].b = b;
			iWritePos++;
		}
	}
	fwrite( p24DIB, sizeof(RGB24)*xl*yl, 1, fp );
	delete []p24DIB;
	delete []buf;
	p24DIB = NULL;
	buf = NULL;

	fclose(fp);

	JustMsg( "%s OK", MapName );
}			


BYTE *LoadMaskBuf, *LoadMaskBackBuf;
BYTE  LoadMaskFlag;
void LoadingLoadMaskTable( void )
{
	FILE *fp;
	int i,j;
	char temp[MAX_PATH];


	if( SCREEN_WIDTH == 640 )
	{
		int loadtable = rand()%9;
		if( loadtable == 1 ) loadtable = 3;
		sprintf( temp, "./data/lt%03d.spr", loadtable );

		fp = Fopen( temp, "rb" );
		fseek( fp, 13, SEEK_SET );
	}

	MemFree( LoadMaskBuf );
	MemFree( LoadMaskBackBuf );
	MemAlloc( LoadMaskBuf, SCREEN_WIDTH * SCREEN_HEIGHT );

	if( SCREEN_WIDTH == 640 )
		fread( LoadMaskBuf, SCREEN_WIDTH,SCREEN_HEIGHT, fp );
	else 
	{
		//memset( LoadMaskBuf, rand, SCREEN_WIDTH*SCREEN_HEIGHT );
		for( i = 0 ; i < SCREEN_HEIGHT ; i ++)
		{
			for( j = 0 ; j < SCREEN_WIDTH ; j ++)
			{
				int t;
				if( j > SCREEN_WIDTH/2 )
				{
					t = ( j * 256 / SCREEN_WIDTH);
					//if( t ) t = rand()%t;
					if( t > 1 && t < 252 ) t += (rand()%3);
				}
				else 
				{
					t = ( (SCREEN_WIDTH-j) * 256 / SCREEN_WIDTH);
				//	if( t ) t = rand()%t;
					if( t > 1 && t < 252 ) t += (rand()%3);
				}

				*( LoadMaskBuf + j + SCREEN_WIDTH *i) = t;// *( LoadMaskBuf + i ) >> 3;
			}
		}
	}

	MemAlloc( LoadMaskBackBuf, dDxSize * SCREEN_HEIGHT );
	
	g_DestBackBuf = (char *)LoadMaskBackBuf; 
	for ( i = 0; i < NewOrderC; i++ ){ neworder[i].show = 1;  neworder[i].sort = 1;  }
	g_OrderInfo.count = g_OldOrderInfoCount;
	for ( i = 0; i < g_OrderInfo.count; i++ ){ g_OrderInfo.order[ i ].show = 1;  }

	g_OrderInfo.count = 0;
	extern void CalcOrder( void );
	CalcOrder();

	DisplaySpriteUnderTile();
	MapDisplay( 0 );
	DisplaySpriteBottom();
	MapDisplay( 2 );
	DisplaySprite();
	MapDisplay( 1 );
	DisplaySpriteTrans();
	DisplaySpriteCeiling();
	RainAnimationOutput();
	SnowAnimationOutput();
	MapBright();
	MenuDisplay();     //메뉴 구성요소의 충돌체크MenuChecking();     //메뉴 구성요소의 충돌체크MenuDisplay();      //메뉴의 활성여부를 판단해 출력하는 함수	

	ViewVersion( g_GameInfo.version );
	

	WORD *t;
	for( j = 31 ; j >= 14 ; j--)
	{
#ifndef _SDl
		g_DestBackBuf = GetSurfacePointer(g_DirectDrawInfo.lpDirectDrawSurfaceBack);
		memcpy(g_DestBackBuf, LoadMaskBackBuf, dDxSize * SCREEN_HEIGHT);

		t = (WORD*)g_DestBackBuf;
		for (i = 0; i < (int)(dDxSize * SCREEN_HEIGHT); i += 2, t++) {
			*t = rgbTable[j * 65536 + *t];
		}

		FlipScreen(&g_DirectDrawInfo);
#endif // !_SDl

		
	}

	t = (WORD *)LoadMaskBackBuf;
	for( i = 0 ; i < (int)(dDxSize * SCREEN_HEIGHT) ; i +=2, t++)	*t = rgbTable[ j * 65536 + *t];		

	for( i = 0 ; i < SCREEN_WIDTH*SCREEN_HEIGHT ; i ++)
		*( LoadMaskBuf + i ) = *( LoadMaskBuf + i ) >> 3;
	
	
	if( SCREEN_WIDTH == 640 )
		fclose(fp);

	LoadMaskFlag = 30;

	g_OrderInfo.count = 0;
	



}

void ViewLoadMask( void )
{
	if( LoadMaskFlag == 0 ) return;

	WORD *buf, *backbuf;
	BYTE *lm;
	int i,j;

	for( i = 0 ; i < SCREEN_HEIGHT ; i ++)
	{
		backbuf = (WORD *)(LoadMaskBackBuf + dDxSize * i);
		buf = (WORD *)(g_DestBackBuf + dDxSize * i);
		lm = LoadMaskBuf + SCREEN_WIDTH * i;
		for( j = 0 ; j < SCREEN_WIDTH ; j ++)
		{
			if( *lm )
			{
				(*lm) -=1;
				*buf = *backbuf;
			}
			buf ++;
			lm ++;
			backbuf++;
		}
	}

	LoadMaskFlag --;


	if( LoadMaskFlag == 0 )
	{
		MemFree( LoadMaskBackBuf );
		MemFree( LoadMaskBuf );
	}
}


void MaskMap( int x, int y, Spr *sp )
{		
	int i,j;
		
	int sx	= x - sp->ox;
	int sy	= y - sp->oy;
	int ex  = sx+sp->xl + TILE_SIZE - 1;
	int ey  = sy+sp->yl + TILE_SIZE - 1;
		
	ex += (( ex - sx )/2);
		
	 sx	/= TILE_SIZE;
	 sy	/= TILE_SIZE;
	 ex /= TILE_SIZE;
	 ey /= TILE_SIZE;

		if( sx < 0 ) sx = 0;
		if( sy < 0 ) sy = 0;
		if( ex >= g_Map.file.wWidth  ) ex = g_Map.file.wWidth;
		if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight;

		
	for( i = sx ; i < ex ; i ++ )
		for( j = sy ; j < ey ; j ++ )
		{
			int front = TileMap[ i][ j].front;
			if(  front )
//			if(  TileMap[ i][ j].frontoffset * TILE_SIZE >= y )
			{ 
				if( Sou[ front].xl == -1 )		PutTile( i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  Sou[front].img, g_DestBackBuf );
				else							PutCompressedImage(  i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  &Sou[ front] );
			}
		}
}

void MaskMapItem( int x, int y, int offy, Spr *sp )
{		
	int i,j;
		
	int sx	= x - sp->ox;
	int sy	= y - sp->oy;
	int ex  = sx+sp->xl + TILE_SIZE - 1;
	int ey  = sy+sp->yl + TILE_SIZE - 1;
		
	//ex += (( ex - sx )/2); // 그림자를 위한 X길이 확장.
		
	 sx	/= TILE_SIZE;
	 sy	/= TILE_SIZE;
	 ex /= TILE_SIZE;
	 ey /= TILE_SIZE;

 		if( sx < 0 ) sx = 0;
		if( sy < 0 ) sy = 0;
		if( ex >= g_Map.file.wWidth  ) ex = g_Map.file.wWidth;
		if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight;


	for( i = sx ; i < ex ; i ++ )
		for( j = sy ; j < ey ; j ++ )
		{
			int front = TileMap[ i][ j].front;
			if(  front )
			if(  (int)TileMap[ i][ j].frontoffset * TILE_SIZE >= offy )
			{ 
				if( Sou[ front].xl == -1 )		PutTile( i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  Sou[front].img, g_DestBackBuf );
				else							PutCompressedImage(  i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  &Sou[ front] );
			}
		}
}

void MaskMapEffect( int x, int y, int height, Spr *sp )
{
	int i,j;

	int sx	= x - sp->ox;
	int sy	= y - sp->oy - height;
	int ex  = sx+sp->xl + TILE_SIZE - 1;
	int ey  = sy+sp->yl + TILE_SIZE - 1;

//	if( !TileMap[ x/TILE_SIZE][ y/TILE_SIZE].front )
//	{
//		return;
//	}

//	int offset = TileMap[ x/TILE_SIZE][ y/TILE_SIZE].frontoffset;

//	ex += (( ex - sx )/2);

	sx	/= TILE_SIZE;
	sy	/= TILE_SIZE;
	ex	/= TILE_SIZE;
	ey	/= TILE_SIZE;

	if( sx < 0 ) sx = 0;
	if( sy < 0 ) sy = 0;
	if( ex >= g_Map.file.wWidth  ) ex = g_Map.file.wWidth;
	if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight;

	//	my += Mapy;

	for( i = sx ; i < ex ; i ++ )
		for( j = sy ; j < ey ; j ++ )
		{
			int front = TileMap[ i][ j].front;
			if(  front )
			if(  (int)TileMap[ i][ j].frontoffset * TILE_SIZE >= y )
			{
				if( Sou[ front].xl == -1 )		PutTile( i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  Sou[front].img, g_DestBackBuf );
				else							PutCompressedImage(  i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  &Sou[ front] );
			}
		}
}


















void ViewRoofData( void )
{
	if( tool_ViewAttrRoofOnOff == 0 ) return;

	int sx, sy;
	int tmx = Mox / TILE_SIZE;
	int tmy = Moy / TILE_SIZE;
	static int mx, my;

	static int tindex = -1;
	static int to;
	to = !to;

	if ( TileMap[ mx ][ my ].attr_room  )
	{
		if ( TileMap[ mx ][ my ].attr_room == 1 )
		{				
			LPEVENTLIST		lpEventList;
			LPROOFGROUP		lpRoofGroup;
			LPROOF			lpRoof;
						
			lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )mx, ( WORD )my );
			if ( lpEventList != NULL )
			{			
				lpRoofGroup = FindRoofGroup( &g_RoofHeader, lpEventList->index );
				if ( lpRoofGroup != NULL )
				{		
					lpRoof = lpRoofGroup->lpFirst;
					while ( lpRoof )
					{	
						sx = lpRoof->x*TILE_SIZE;
						sy = lpRoof->y*TILE_SIZE;
						Box( sx+4, sy+4, sx + TILE_SIZE - 8, sy + TILE_SIZE - 8, RGB16( 0,255,0 ));
						Box( sx+5, sy+5, sx + TILE_SIZE -10, sy + TILE_SIZE -10, RGB16( 30,255,30 ));
						Hcolor( 0 );
						Hprint2( sx - Mapx, sy- Mapy,  g_DestBackBuf, "%d", lpEventList->index );
						Hcolor( 0xffff );
						Hprint2( sx - Mapx-1, sy- Mapy-1,  g_DestBackBuf, "%d", lpEventList->index );
						//						TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 1;
						lpRoof = lpRoof->lpNext;
					}
				}
			}
		}
		else
		{
			LPEVENTLIST		lpEventList;
			LPROOFGROUP		lpRoofGroup;
			LPROOF			lpRoof;
				
			lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )mx, ( WORD )my );
			if ( lpEventList != NULL )
			{
				lpRoofGroup = FindRoofGroup( &g_RoofHeader, lpEventList->index );
				if ( lpRoofGroup != NULL )
				{
					lpRoof = lpRoofGroup->lpFirst;
					while ( lpRoof )
					{
						sx = lpRoof->x*TILE_SIZE;
						sy = lpRoof->y*TILE_SIZE;
						if( to ) Box( sx+4, sy+4, sx + TILE_SIZE - 8, sy + TILE_SIZE - 8, RGB16( 0,255,0 ));
						else Box( sx+5, sy+5, sx + TILE_SIZE -10, sy + TILE_SIZE -10, RGB16( 30,255,30 ));
						Hcolor( 0 );
						Hprint2( sx - Mapx, sy- Mapy,  g_DestBackBuf, "%d", lpEventList->index );
						Hcolor( 0xffff );
						Hprint2( sx - Mapx-1, sy- Mapy-1,  g_DestBackBuf, "%d", lpEventList->index );
						//						TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 1;
//						TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 0;
						lpRoof = lpRoof->lpNext;
					}
				}
			}
		}
	}

	if ( TileMap[ tmx ][ tmy ].attr_room  )
	{
		mx = tmx, my = tmy;
	}
}




///////////////////////////////////////////////////////////////////////////
// 로딩이나 맵이동후 지붕이 닫쳐 있으면 연다.
// 031110 YGI
void CheckRoof()
{
	LPCHARACTER ch = Hero;

	LONG	mx = ch->position.x;
	LONG	my = ch->position.y;

	if( TileMap[mx][my].roof )
	{
		LPROOFGROUP		lpRoofGroup;
		LPROOF			lpRoof;

		lpRoofGroup = FindRoofGroupByXY( &g_RoofHeader, mx, my );
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
}

///////////////////////////////////////////////////////////////////////////

// 나의 Avata가 집안으로 들어 갔는지를 Check 하여 들어갔으면 
// 지붕을 연다.
void CheckEnterHouse( LPCHARACTER ch )
{
	// 주인공이 아니면 Check할필요 없다. 
	if( ch != Hero ) return;

	LONG	mx = ch->position.x;
	LONG	my = ch->position.y;

	// 같은 자리를 두번 Check할 필요는 없다. 
	if( mx == ch->oldposition.x && my == ch->oldposition.y ) return;
	
	if ( TileMap[ mx ][ my ].attr_room  )
	{
		if ( TileMap[ mx ][ my ].attr_room == 1 )	
		{
			LPEVENTLIST		lpEventList;
			LPROOFGROUP		lpRoofGroup;
			LPROOF			lpRoof;

			lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )mx, ( WORD )my );
			if ( lpEventList != NULL )
			{
				SendRoofOpenHouse( lpEventList->index );
				lpRoofGroup = FindRoofGroup( &g_RoofHeader, lpEventList->index );
				if ( lpRoofGroup != NULL )
				{
					lpRoof = lpRoofGroup->lpFirst;
					while ( lpRoof )
					{
						TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 1;
						lpRoof = lpRoof->lpNext;
					}
					ToggleRainHide(true); //Eleval 16/08/09
					ToggleSnowHide(true); //Eleval 16/08/09
				}
			}
		}
		else
		{
			LPEVENTLIST		lpEventList;
			LPROOFGROUP		lpRoofGroup;
			LPROOF			lpRoof;
				
			lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )mx, ( WORD )my );
			if ( lpEventList != NULL )
			{
				SendRoofCloseHouse( lpEventList->index );
				lpRoofGroup = FindRoofGroup( &g_RoofHeader, lpEventList->index );
				if ( lpRoofGroup != NULL )
				{
					lpRoof = lpRoofGroup->lpFirst;
					while ( lpRoof )
					{
						TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 0;
						lpRoof = lpRoof->lpNext;
					}
					ToggleRainHide(false); //Eleval 16/08/09
					ToggleSnowHide(false); //Eleval 16/08/09
				}
			}
		}
	}
}












void SettingTripAround( void )
{
	
}

void DeSettingTripAround( void )
{
	
}

bool IsCollision(int nX, int nY)
{
  return (TileMap[nX][nY].attr_dont || 
          TileMap[nX][nY].empty__4  || 
          TileMap[nX][nY].attr_light) ? true:false;
}

bool IsMovable(int nX, int nY)
{ //< CSD-020706
	return (TileMap[nX][nY].attr_dont || TileMap[nX][nY].attr_inside) ? false:true;
} //> CSD-020706