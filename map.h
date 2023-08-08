/*

*/

#ifndef	__MAP_H__
#define	__MAP_H__

#include "dragon.h"

#pragma pack	(push, 1 )






#define TILE_MAX_   3600
///////////////////// 0430 lkh 추가 ///////////////////////////
#define HOUSETILE_MAX_	1200






/*
typedef struct	tagTILE
{
	DWORD	back			: 12;
	DWORD	front			: 12;
	DWORD	fronttype		:  1;
	DWORD	rooftype		:  1;
	DWORD	attr_dont		:  1;
	DWORD	attr_map		:  1;
	DWORD	attr_roof		:  1;
	DWORD	attr_door		:  1;
	DWORD	attr_tree		:  1;
	DWORD	attr_music		:  1;

	DWORD	roof			: 12;
	DWORD	show_roof		:  1;
	DWORD	frontoffset		:  9;
	DWORD	attr_scroll		:  1;
	DWORD	attr_padding	:  9;
} TILE, *LPTILE;
*/	
	
typedef struct	tagTILE
{	
	DWORD	back			: 12;
	DWORD	front			: 12;
	DWORD	empty__1		:  1;	// fronttype
	DWORD	rooftype		:  1;
	DWORD	attr_dont		:  1;
	DWORD	attr_map		:  1;
	DWORD	attr_room		:  2;
	DWORD	empty__2		:  1;		// attr_music
	DWORD	attr_skill		:  1;
	
	DWORD	roof			: 12;
	DWORD	show_roof		:  1;
	DWORD	frontoffset		: 10;
	
	DWORD	occupied		:  1;
	DWORD	canmatch        :  1;  // 이곳에서는 싸워도 PK가 되지 않는다. 
	DWORD	empty__4		:  1;
	DWORD	attr_light		:  1;
	DWORD	attr_inside		:  1;
	DWORD	attr_river		:  1;
	DWORD	attr_magic		:  1;
	DWORD	attr_no_battle  :  1;
	DWORD	empty__6		:  1;
	
} TILE, *LPTILE;
	
	
typedef struct tagSOU2
{	
	short	tile_no;
	char	source_no;
	
} SOU2, *LPSOU2;
	
#pragma pack	(pop)	
#pragma pack	(push, 8 )
	
typedef struct	tagMAPFILE
{	
	WORD		wType;
	WORD		wWidth, wHeight;
	char		cOdd, cEven;
} MAPFILE, *LPMAPFILE;
	
typedef struct	tagMAP
{	
	SURFACEINFO		SurfaceInfo;
	int				x, y;
	WORD			wScreenWidth, wScreenHeight;
	int				tox, toy;
	int				oX, oY;							//타일 기준 좌표와 함께 도트단위 표현을 위한 맵옵셋 좌표
	int             display_X, display_Y;           //도트 단위 스크롤 연출을 위한 절대(도트) 맵 좌표 

	MAPFILE			file;
	Spr				sp;
} MAP, *LPMAP;
	
// roof
typedef struct	tagROOF
{	
	WORD			x, y;
	
	struct tagROOF	*lpPrev, *lpNext;
} ROOF, *LPROOF;
	
typedef struct	tagROOFGROUP
{	
	WORD					count;
	
	LPROOF					lpFirst, lpLast;
	struct tagROOFGROUP		*lpPrev, *lpNext;
} ROOFGROUP, *LPROOFGROUP;
	
typedef struct	tagROOFHEADER
{	
	WORD			count;
	
	LPROOFGROUP		lpFirst, lpLast;
} ROOFHEADER, *LPROOFHEADER;
	
// event
typedef struct	tagEVENTLIST
{	
	WORD				x, y;
	WORD				index;
	struct tagEVENTLIST	*lpPrev, *lpNext;
} EVENTLIST, *LPEVENTLIST;

typedef struct	tagEVENTLISTHEADER
{
	WORD			wCount;
	LPEVENTLIST		lpFirst, lpLast;
} EVENTLISTHEADER, *LPEVENTLISTHEADER;

#pragma	pack	(pop)

extern MAP					g_Map;
extern TILE					TileMap[ VILLAGE_SIZE ][ VILLAGE_SIZE ];
//////////////// 0501 lkh 추가 //////////////////
extern TILE					HouseTileMap[ VILLAGE_SIZE ][ VILLAGE_SIZE ];

extern ROOFHEADER			g_RoofHeader;
extern EVENTLISTHEADER		g_EventListHeaderMap;
extern EVENTLISTHEADER		g_EventListHeaderRoom;
extern EVENTLISTHEADER		g_EventListHeaderMusic;


extern Spr Sou[ TILE_MAX_];
extern BYTE *SouBuf;

extern char MapName[ NM_LENGTH];



extern int LimitMapSx;
extern int LimitMapSy;
extern int LimitMapEx;
extern int LimitMapEy;

extern int OldEventMx, OldEventMy, EventMx, EventMy;

extern int StartMapx, StartMapy, StartMapFlag;

extern int MapNumber;
///////////// SoundUp lkh 추가 //////////////
extern BOOL	g_BGE_Flag;

extern int MapNumberX, MapNumberY;
extern int MapNumberFloor;
extern char MapNumberName[ 20];

extern int SmallMapX,  SmallMapY;


extern int TotalTileNo; // Map Source의 전체 읽어들인 타일의 수..

#ifdef __cplusplus
extern "C"
{
#endif

extern BOOL		MapBuild( LPMAP lpMap, char *mapname );
extern void		MapDestroy( LPMAP lpMap );
extern BOOL		MapCreateSurface( LPDIRECTDRAW lpDirectDraw, LPMAP lpMap );
extern void		MapCleanupSurface( LPMAP lpMap );
extern BOOL		MapRestoreSurface( LPMAP lpMap );
extern BOOL		MapLoadImage( LPDIRECTDRAW lpDirectDraw, LPMAP lpMap, char* lpszFilePath );
extern BOOL		MapReLoadImage( LPDIRECTDRAWSURFACE* lplpSurface, char* lpszFilePath );
extern BOOL		MapReadFile( LPMAPFILE lpMapFile, char* pszFileName );
extern BOOL		MapWirteFile( LPMAPFILE lpMapFile, char* pszFileName );
extern void		MapDisplay( int nType );
extern void		MapDisplay_Attr( void );


extern BOOL			BuildRoofHeader( LPROOFHEADER lpRoofHeader, char* lpszFileName );
extern void			DestroyRoofHeader( LPROOFHEADER lpRoofHeader );
extern LPROOFGROUP	AddRoofGroup( LPROOFHEADER lpRoofHeader );
extern void			DeleteRoofGroup( LPROOFHEADER lpRoofHeader, LPROOFGROUP lpRoofGroup );
extern LPROOF		AddRoof( LPROOFGROUP lpRoofGroup, WORD x, WORD y );
extern void			DeleteRoof( LPROOFGROUP lpRoofGroup, LPROOF lpRoof );
extern LPROOFGROUP	FindRoofGroup( LPROOFHEADER lpRoofHeader, WORD index );
extern LPROOF		FindRoof( LPROOFGROUP lpRoofGroup, WORD x, WORD y );
extern BOOL			ReadRoofData( LPROOFHEADER lpRoofHeader, char* lpszFileName );
										
extern BOOL			BuildEventList( char* lpszFileName );
extern void			DestroyEventList( void );
extern LPEVENTLIST	AddEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y, WORD wIndex );
extern void			DeleteEventList( LPEVENTLISTHEADER lpEventListHeader, LPEVENTLIST lpEventList );
extern LPEVENTLIST	FindEventList( LPEVENTLISTHEADER lpEventListHeader, WORD x, WORD y );
extern BOOL			ReadEventList( char* lpszFileName );

extern BOOL		IsScreenArea( LPCHARACTER lpCharacter );
extern BOOL		IsScreenAreaMapObject( int x, int y, SIZE size );
extern BOOL		IsScreenAreaCSP( int x, int y, Spr *sp );

////////////////////// 0808 lkh 추가 ////////////////////////////
extern BOOL		IsScreenAreaView( int x, int y );

extern BOOL		IsScreenArea_( POINT position );
extern void		StartSetView( LPCHARACTER ch );
extern void		SetView( LPCHARACTER ch, int range );
extern void		SetHeroPosition( int );
extern int		MoveScreen( DIRECTION direction );


extern void		MapObjectTouchOrder( void );

extern void		MakeSizeofSOU2( void );
extern void		LoadMapSource( char *filename );
extern void		LoadMapSource2( char *filename );
///////////////// 0430 lkh 추가 /////////////////
extern void		LoadHouseMapSource( char *filename );

extern void		FreeMapSource( void );


extern int		ReturnEventNo( int mx, int my );
extern int		ChangeEventNo( int mx, int my, int eventno  );

extern int		CheckEventWith_NPC(  LPCHARACTER hero, LPCHARACTER oppo );
extern int		CheckEventAttrTile( void );


extern void		LoadSmallMapBack( void );
extern void		PutSmallMapBack( int x, int y );
extern void		OutputSmallMap( void );


extern void LoadingLoadMaskTable( void );
extern void ViewLoadMask( void );
extern void ViewRoofData(void );
extern void MaskMap( int x, int y, Spr *sp );
extern void MaskMapItem( int x, int y, int offy, Spr *sp );
extern void MaskMapEffect( int x, int y, int height, Spr *sp );
extern void SettingTripAround( void );
extern void DeSettingTripAround( void );
extern char *GetMapNameByPort( int port );
extern char *GetMapNameByMapfile( char *map_file );
extern char *GetMapNameByIndex( int i );		// 021011 //추가안해놔서리.
extern int GetIndexByMapfile( char *map_file );
extern void CheckEnterHouse( LPCHARACTER ch );
extern void CheckRoof(); // 031110 YGI
extern bool IsCollision(int nX, int nY);
extern bool IsMovable(int nX, int nY);
#ifdef __cplusplus
}
#endif

#endif	// __MAP_H__
