/*
	-	1999/??/??	ReCreated by Hongdoit;
*/

#ifndef	__OBJECT_H__
#define	__OBJECT_H__

#include "dragon.h"

///////////////////////////////////////////////////////////////////////////////
//
//
#define MAX_MAPOBJECT_				5000
#define MAX_MAPOBJECTIMAGE			2000
#define ITEM_FRAME_MAX_				10

#define	OBJECTATTR_BACK_ABOVE		1
#define	OBJECTATTR_BACK_BELOW		2
#define	OBJECTATTR_FRONT_ABOVE		3
#define	OBJECTATTR_FRONT_BELOW		4
#define	OBJECTATTR_ROOF_ABOVE		5
#define	OBJECTATTR_ROOF_BELOW		6

#define	OBJECTDIRECTION_RIGHTUP		1
#define OBJECTDIRECTION_RIGHTDOWN	2
#define	OBJECTDIRECTION_LEFTDOWN	3
#define	OBJECTDIRECTION_LEFTUP		4

#define	OBJECTANIMATION_STOP		1
#define	OBJECTANIMATION_LOOP		2
#define	OBJECTANIMATION_PLAY		3


#define OBJECTANI_NO				1
#define OBJECTANI_RETURNLOOP		2	
#define OBJECTANI_LOOP				3
#define OBJECTANI_1_RETURNLOOP		4
#define OBJECTANI_1_LOOP			5
#define OBJECTANI_TOGGLE_ANI		6


#define OBJECTSPRITE_NORMAL			0
#define OBJECTSPRITE_TRANS			1
#define OBJECTSPRITE_ALPHA     		2

typedef struct	tagOBJECTINFO
{				
	WORD	wID;
	WORD	wFrameCount;
	SIZE	sizeFrame;
	POINT	origin;
				

} OBJECTINFO, *LPOBJECTINFO;

typedef struct	tagOBJECTATTR
{
	WORD	wID;				//	saf화일명..
	WORD	wAnimationCount;	//  
	WORD	wAnimationOrder;	//
	BYTE	cAnimationType;		//  정지 .. Animation...
	BYTE	cAttr;
	BYTE	cAttr1;
	BYTE	cAttr2;
	WORD	wCurrentFrame;		//
	WORD	wDirection;			
	POINT	position;			// 타일의 위치.
	POINT	offset;				// 타일에서의 변이값..
	POINT	origin;				//  // 현재는 사용 않함..
} OBJECTATTR, *LPOBJECTATTR;

typedef struct	tagOBJECT
{
	OBJECTATTR			ObjectAttr;

	struct tagOBJECT	*lpPrev, *lpNext;
} OBJECT, *LPOBJECT;

typedef struct	tagOBJECTHEADER
{
	int						nCount;
	OBJECTINFO				ObjectInfo;
	LPDIRECTDRAWSURFACE 	lpSurface;
	
	LPOBJECT				lpHead, lpTail;
	struct tagOBJECTHEADER	*lpPrev, *lpNext;
} OBJECTHEADER, *LPOBJECTHEADER;
//---------------------------------------------------
		
typedef struct tagMAPITEM
{		
	int nFrameCount;
	Spr	 spr[ ITEM_FRAME_MAX_];
}MAPOBJECTIMAGE, *LPMAPOBJECTIMAGE;
			
			
			
typedef struct tagMAPOBJECT_old{	
	WORD  id;
	BYTE  status;
	WORD  objectoritem;
	char  curframe;
	WORD  dum;
	BYTE  animationType;
	int   x, y;
	BYTE  attr;
	BYTE  attr1;
	BYTE  attr2;
	WORD  dir;
	int   offx, offy;
	short dx0[ITEM_FRAME_MAX_], dy0[ITEM_FRAME_MAX_];
	short dx1[ITEM_FRAME_MAX_], dy1[ITEM_FRAME_MAX_];
			
	BYTE  light;
	short lightx, lighty;
	char  delay;
	char  anidir;

}MAPOBJECT_old, *LPMAPOBJECT_old;
			
			

typedef struct tagMAPOBJECT{								
	WORD  id;						
	BYTE  status;										
	WORD  objectoritem;								
	char  curframe;											
	WORD  dum;										
	BYTE  animationType;									
	int   x, y;												
	BYTE  attr;											
	BYTE  attr1;										
	BYTE  attr2;								
	WORD  dir;											
	int   offx, offy;													
	short dx0[ITEM_FRAME_MAX_], dy0[ITEM_FRAME_MAX_];
	short dx1[ITEM_FRAME_MAX_], dy1[ITEM_FRAME_MAX_];
																	
	BYTE  light;												
	short lightx, lighty;											
	char  delay;												
	char  anidir;															
																	
	////////////////// SoundUp lkh 추가 /////////////////
	unsigned short soundno;
	unsigned short soundframe;
	unsigned short sounddelay;
																				
// 0109 ItemDontBox													
//	short dtx0, dty0;												
//	short dtx1, dty1;										
//	short dtx2, dty2;											
//	short dtx3, dty3;											
																
}MAPOBJECT, *LPMAPOBJECT;														
																		
			
			
///////////// 0203 lkh 추가 //////////////////
typedef struct tagBOARDDATA
{			
	WORD	boardNumber;
	char	str_boardData[80];
}	BOARDDATA;
			
// THAI YGI
#define MAX_DSD_STRING	260
struct DIRECTBOARDDATA
{	//< CSD-030324			
	int		boardNumber;
	char	str_Title[MAX_DSD_STRING];
	char	str_East[MAX_DSD_STRING];
	char	str_West[MAX_DSD_STRING];
	char	str_South[MAX_DSD_STRING];
	char	str_North[MAX_DSD_STRING];
};	//> CSD-030324
	
extern MAPOBJECTIMAGE		MoImage[ MAX_MAPOBJECTIMAGE * 10];
extern WORD					TotalMapObjectID;
extern WORD					TotalMapObject;
extern MAPOBJECT			Mo[ MAX_MAPOBJECT_];

extern MAPOBJECTIMAGE		TempMoImage;

extern int OldSoundNo, OldSoundFrame, OldSoundDealy;

///////////////////////////////////////////////////////////////////////////////
//		
//
#ifdef __cplusplus
extern "C"
{		
#endif	
		
extern int  SaveTOI2( char *filename );
extern int	LoadTOI( char *filename );
extern void FreeTOI( void );

extern void CheckMapObject( LPMAPOBJECT m );
extern void DisplayMapObject( LPMAPOBJECT m );
extern void DisplayMapObjectAfter( LPMAPOBJECT m, int selected );

extern void InsertMapObject( int moid, int x, int y );
extern void DeleteMapObject( int no );
extern void DeleteMapObjectAll( int no );
extern void LoadTempSprite( int id );

#ifdef __cplusplus
}	
#endif
	
#endif	// __OBJECT_H__
	