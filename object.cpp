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
#include <stdio.h>
#include <io.h>
#include "object.h"
#include "map.h"
#include "Hong_Sub.h"
#include "convert565to555.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Tool.h"
#include "Item.h"




///////////////////////////////////////////////////////////////////////////////
//

MAPOBJECTIMAGE      MoImage[ MAX_MAPOBJECTIMAGE * 10];
WORD				TotalMapObjectID;
WORD				TotalMapObject;

MAPOBJECT			Mo[   MAX_MAPOBJECT_];


MAPOBJECTIMAGE		TempMoImage;

//////////// SoundUp lkh 추가 ////////////
extern void		ObjectSoundPlay(void);

///////////////////////////////////////////////////////////////////////////////
//

int OldSoundNo, OldSoundFrame, OldSoundDealy;
				
extern void changemapobjectname( char *oldname, char *newname );  // map.cpp
extern void changesourcename( char *oldname, char *newname );  // map.cpp
		
int LoadMapObjectCSP( int id , LPMAPOBJECTIMAGE mi )
{			
	FILE *fp;
	char filename[ FILENAME_MAX];
	char tempmapobject[ FILENAME_MAX];
	short ox, oy;
	int   i;


	if( id == 1447 ) {
		_asm nop;
	}
			
	if( mi->spr[0].img != NULL ) return 0;
			
	changemapobjectname( MapName, tempmapobject );
			
	int lev = id / 1000;
			
	if( lev == 1 ) lev = 0;
			
	for( i = 0 ; i < ITEM_FRAME_MAX_ ; i ++ )
	{		
			
#ifdef _MAPOBJECT_DIRECTORY_CHANGE_
		{	
			if( i == 0 )	sprintf( filename, "./object/%d/%04d.csp", lev , id );
			else 			sprintf( filename, "./object/%d/%04d%02d.csp", lev , id, i );
		}	
#else		
		{	
			if( i == 0 )	sprintf( filename, "./object/%s/%04d.csp", tempmapobject , id );
			else 			sprintf( filename, "./object/%s/%04d%02d.csp", tempmapobject , id, i );
		}	
#endif		
			
		fp = Fopen( filename, "rb" );						
		if( fp == NULL ) return i;								
			fread( &mi->spr[i].xl, sizeof( short ), 1, fp);	
			fread( &mi->spr[i].yl, sizeof( short ), 1, fp);	
//			fseek( fp, 2, SEEK_CUR ); //&mi->spr[0].ox, sizeof( short ), 1, fp);
//			fseek( fp, 2, SEEK_CUR ); // fread( &mi->spr[0].oy, sizeof( short ), 1, fp);
			fread( &mi->spr[i].ox, sizeof( short ), 1, fp);	
			fread( &mi->spr[i].oy, sizeof( short ), 1, fp);	
															
//		mi->spr[0].ox = lpObjectImage->wOriginx;			
//		mi->spr[0].oy = lpObjectImage->wOriginy;			
															
			fread( &mi->spr[i].size, sizeof( unsigned int ), 1, fp);
			mi->spr[i].img = NULL;
			MemAlloc( mi->spr[i].img, mi->spr[i].size );
			fread( mi->spr[i].img, mi->spr[i].size, 1, fp);
			convert565to555( &mi->spr[i] );
		fclose(fp);	
	}		
			
	return i;
			
	sprintf( filename, "./object/%04d.org", id );
	fp = Fopen( filename, "rb" );
	if( fp )
	{		
		fseek( fp, 4, SEEK_SET );
		fread( &ox, 2,1, fp );
		fread( &oy, 2,1, fp );
			
		mi->spr[0].ox = ox;
		mi->spr[0].oy = oy;
		fclose(fp);
	}		
}			
/*		

  


ftp://ftp1.bora.net/pub/sw/game_sw/e2soft/DragonRajaBeta_001216.zip

ftp://ftp1.bora.net/pub/sw/game_sw/e2soft/DragonRajaBeta_001216_2.exe






int LoadMapObjectCSP( int id , LPMAPOBJECTIMAGE mi )
{	
	FILE *fp;
	char filename[ FILENAME_MAX];
	char tempmapobject[ FILENAME_MAX];
	short ox, oy;
	int   i;
	
	if( mi->spr[0].img != NULL ) return 0;
	
	changemapobjectname( MapName, tempmapobject );
	
	int lev = id / 1000;
	if( lev == 1 ) lev = 0;
					
	for( i = 0 ; i < ITEM_FRAME_MAX_ ; i ++)
	{											

#ifdef _MAPOBJECT_DIRECTORY_CHANGE_
		{
			if( i == 0 )	sprintf( filename, "./object/%d/%04d.csp", lev , id );
			else 			sprintf( filename, "./object/%d/%04d%02d.csp", lev , id, i );
		}
#else
		{
			if( i == 0 )	sprintf( filename, "./object/%s/%04d.csp", tempmapobject , id );
			else 			sprintf( filename, "./object/%s/%04d%02d.csp", tempmapobject , id, i );
		}
#endif
		
		fp = Fopen( filename, "rb" );						
		if( fp == NULL ) return i;								
			fread( &mi->spr[i].xl, sizeof( short ), 1, fp);	
			fread( &mi->spr[i].yl, sizeof( short ), 1, fp);	
//			fseek( fp, 2, SEEK_CUR ); //&mi->spr[0].ox, sizeof( short ), 1, fp);
//			fseek( fp, 2, SEEK_CUR ); // fread( &mi->spr[0].oy, sizeof( short ), 1, fp);
			fread( &mi->spr[i].ox, sizeof( short ), 1, fp);	
			fread( &mi->spr[i].oy, sizeof( short ), 1, fp);	
															
//		mi->spr[0].ox = lpObjectImage->wOriginx;			
//		mi->spr[0].oy = lpObjectImage->wOriginy;			
															
			fread( &mi->spr[i].size, sizeof( unsigned int ), 1, fp);
			mi->spr[i].img = NULL;
			MemAlloc( mi->spr[i].img, mi->spr[i].size );
			fread( mi->spr[i].img, mi->spr[i].size, 1, fp);
			convert565to555( &mi->spr[i] );
		fclose(fp);	
	}	
		
	return i;
		
	sprintf( filename, "./object/%04d.org", id );
	fp = Fopen( filename, "rb" );
	if( fp )
	{	
		fseek( fp, 4, SEEK_SET );
		fread( &ox, 2,1, fp );
		fread( &oy, 2,1, fp );
								
		mi->spr[0].ox = ox;
		mi->spr[0].oy = oy;
		fclose(fp);
	}
}
*/
								
																
void DisplayMapObject( LPMAPOBJECT mo )
{																
	int mx = mo->x + mo->offx - Mapx;
	int my = mo->y + mo->offy - Mapy;
	int fspflag = 0;									
															
	Spr *sp, *fsp;													
	sp =  &MoImage[ mo->id-1].spr[ mo->curframe];
										

	if( MapNumber == 2 )			
	{									
		if( 4040 <= mo->id && mo->id <= 4043 )  { PutCompressedImageWaveTile( mx, my, sp ); goto END_;	}
		if( 4045 == mo->id )					{ PutCompressedImageWaveTile( mx, my, sp ); goto END_;	}
	}										
	fsp = &MoImage[ mo->id-1].spr[0];
	
	// 020305 YGI
	// 디폴트 화면을 찍을 필요가 없다.
	if( !GetSysInfo( SI_NOT_CONNECT_MODE ) && mo->objectoritem == ITEMTYPE_COLOSSUS_STONE )	goto END_;

	switch( mo->animationType )		
	{									
	case OBJECTANI_NO			:	
	case OBJECTANI_RETURNLOOP	:	
	case OBJECTANI_LOOP			:	
	case OBJECTANI_TOGGLE_ANI	:	switch( mo->attr1 & 0x0003)
									{
										case OBJECTSPRITE_NORMAL :	PutCompressedImage( mx, my, sp );			break;
										case OBJECTSPRITE_TRANS  :  PutCompressedImageFX( mx, my, sp, mo->attr1 >>3, 1 );	break;
										case OBJECTSPRITE_ALPHA	 :  
											if( mo->light && DayLightControl >= 31 )
											{
											}
											else 
											{
												PutCompressedImageFX( mx, my, sp, mo->attr1 >>3, 2 );	break;
											}
									}
		break;							
												
	case OBJECTANI_1_RETURNLOOP	:			
	case OBJECTANI_1_LOOP		:	PutCompressedImage( mx, my, fsp );
									fspflag = 1;

									// Light를 가지고 있다면..

									if( mo->light && DayLightControl >= 31 )
									{
									}
									else 
									{
										switch( mo->attr1 & 0x0003 )
										{
											case OBJECTSPRITE_NORMAL :	PutCompressedImage( mx, my, sp );						break;
											case OBJECTSPRITE_TRANS  :  PutCompressedImageFX( mx, my, sp, mo->attr1 >>3, 1 );	break;
											case OBJECTSPRITE_ALPHA	 :  PutCompressedImageFX( mx, my, sp, mo->attr1 >>3, 2 );	break;
										}
									}
		break;											
	}											
																
#ifdef _DEBUG											
	if( mo->attr2 && GetSysInfo( SI_GAME_MAKE_MODE ) )
	{															
//		Box_( mx - fsp->ox, my - fsp->oy, fsp->xl, fsp->yl );
		Line(	mo->dx0[0] + mo->x , mo->dy0[0] + mo->y , mo->dx1[1] + mo->x , mo->dy1[1] + mo->y , RGB16( 0x00,0xff,0x00 ));
	}																	
#endif													
													
														
END_:											
																	
	int i,j;													
	int tsx, tsy, tex, tey;								
											
	int sx	= mx - sp->ox + Mapx;
	int sy	= my - sp->oy + Mapy;
	int ex  = sx+sp->xl + TILE_SIZE - 1;
	int ey  = sy+sp->yl + TILE_SIZE - 1;

	if( fspflag )
	{
		tsx	= mx - fsp->ox + Mapx;
		tsy	= my - fsp->oy + Mapy;
		tex  = sx+fsp->xl + TILE_SIZE - 1;
		tey  = sy+fsp->yl + TILE_SIZE - 1;

		if( tsx < sx ) sx = tsx;	if( tsy < sy ) sy = tsy;
		if( tex > ex ) ex = tex;	if( tey > ey ) ey = tey;
	}
	ex += (( ex - sx )/2);

	 sx	/= TILE_SIZE;
	 sy	/= TILE_SIZE;
	 ex /= TILE_SIZE;
	 ey /= TILE_SIZE;

	 my += Mapy;

	if( sx < 0 ) sx = 0;
	if( sy < 0 ) sy = 0;
	if( ex >= g_Map.file.wWidth  ) ex = g_Map.file.wWidth;
	if( ey >= g_Map.file.wHeight ) ey = g_Map.file.wHeight;

	
	for( i = sx ; i < ex ; i ++ )
		for( j = sy ; j < ey ; j ++ )
		{
			int front = TileMap[ i][ j].front;
			if(  front )
			if(  TileMap[ i][ j].frontoffset * TILE_SIZE >= (DWORD )mo->y )
			{ 
				if( Sou[ front].xl == -1 )		PutTile( i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  Sou[front].img, g_DestBackBuf );
				else							PutCompressedImage(  i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  &Sou[ front] );
			}
		}
}
/*
			if( mo->attr2 )
				{
					if( Determinant( mo->x + mo->dx0[0], mo->y + mo->dy0[0], 
									 mo->x + mo->dx1[0], mo->y + mo->dy1[0], i * TILE_SIZE, TileMap[ i][ j].frontoffset * TILE_SIZE ) > 0 )
					{
						if( Sou[ front].xl == -1 )		PutTile( i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  Sou[front].img, g_DestBackBuf );
						else							PutCompressedImage(  i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  &Sou[ front] );
					}
				}
				else 
				{
					if(  TileMap[ i][ j].frontoffset * TILE_SIZE >= mo->y )
					{ 
						if( Sou[ front].xl == -1 )		PutTile( i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  Sou[front].img, g_DestBackBuf );
						else							PutCompressedImage(  i*TILE_SIZE - Mapx,  j*TILE_SIZE - Mapy,  &Sou[ front] );
					}
				}
	*/

void DisplayMapObjectAfter( LPMAPOBJECT mo, int selected )
{					
	int mx = mo->x + mo->offx - Mapx;
	int my = mo->y + mo->offy - Mapy;
					
	Spr *sp, *fsp;	
	fsp = &MoImage[ mo->id-1].spr[0];
	sp =  &MoImage[ mo->id-1].spr[ mo->curframe];
					
	if( selected )	
	{				
		PutCompressedImageBlendRGB( mx, my, sp, ReturnBlendRGB( 0xff,0,0 ));
	}				
					
	int x = mo->x;							
	int y = mo->y;							
												
	Line( x-5, y, x+5, y, RGB16( 0xff, 0x1f, 0x1f));
	Line( x, y-5, x, y+5, RGB16( 0xff, 0x1f, 0x1f));
												
	if( MapNumber == 2 )	// firedun1....
	{										
		if( (4040 <= mo->id && mo->id <= 4043) || mo->id == 4045 )  { return; }
	}									
												
	switch( mo->animationType )			
	{									
	case OBJECTANI_NO			:		
	case OBJECTANI_RETURNLOOP	:		
	case OBJECTANI_LOOP			:		
	case OBJECTANI_TOGGLE_ANI	:	switch( mo->attr1 )
									{	
										case OBJECTSPRITE_NORMAL :	break;
										case OBJECTSPRITE_TRANS  :  break;
										case OBJECTSPRITE_ALPHA	 :  break;
									}	
		break;							
												
	case OBJECTANI_1_RETURNLOOP	:			
	case OBJECTANI_1_LOOP		:	/*PutCompressedImage( mx, my, fsp );*/
									switch( mo->attr1 )
									{
										case OBJECTSPRITE_NORMAL :	/*PutCompressedImage( mx, my, sp );				*/break;
										case OBJECTSPRITE_TRANS  :  /*PutCompressedImageFX( mx, my, sp, 20, 1 );	*/break;
										case OBJECTSPRITE_ALPHA	 :  /*PutCompressedImageFX( mx, my, sp, 20, 2 );	*/break;
									}
		break;											
	}									
													
									
	if( mo->attr2 )	
	{				
		Line(mo->dx0[ mo->curframe] + mo->x , mo->dy0[ mo->curframe] + mo->y, 
			 mo->dx1[ mo->curframe] + mo->x , mo->dy1[ mo->curframe] + mo->y, RGB16( 0x00,0xff,0x00 ));
	}				
}					




void CheckMapObject( LPMAPOBJECT mo )
{
	if( --mo->delay < 0 ) { mo->delay = 3;  }
	else return;

	int totalframe = MoImage[ mo->id-1].nFrameCount;

	switch( mo->animationType )
	{
	case OBJECTANI_NO			:	if( mo->curframe < 0 || mo->curframe >= totalframe ) 
									{
										mo->curframe = 0;
									}
		break;

	case OBJECTANI_RETURNLOOP	:	if( mo->anidir )
									{	
										mo->curframe ++; 
										if( mo->curframe >= totalframe )  { mo->anidir = !mo->anidir; mo->curframe = totalframe-1; }
									}
									else 
									{
										mo->curframe --;
										if( mo->curframe < 0 )  { mo->anidir = !mo->anidir; mo->curframe = 0; }
									}
		break;

	case OBJECTANI_LOOP			:	mo->curframe ++; 
									if( mo->curframe >= totalframe )  { mo->curframe = 0; }
		break;

	case OBJECTANI_TOGGLE_ANI	:	mo->curframe = 0;
		break;
	
	case OBJECTANI_1_RETURNLOOP	:	if( mo->anidir )
									{	
										mo->curframe ++; 
										if( mo->curframe >= totalframe )  { mo->anidir = !mo->anidir; mo->curframe = totalframe-1; }
									}
									else 
									{
										mo->curframe --;
										if( mo->curframe < 1 )  { mo->anidir = !mo->anidir; mo->curframe = 1; }
									}
		break;
	case OBJECTANI_1_LOOP		:	mo->curframe ++; 
									if( mo->curframe >= totalframe )  { mo->curframe = 1; }
		break;
	}
}





/////////////////////////////////////////////////////////////////////////////////////////////

int TempMoImageID[ 2000];
	

#include "menuset.h"		// 021030 YGI
	
////////////////////// SoundUp lkh 수정 ///////////////////////	
// newtoi == true  이면   사운드부분이들어간것을 말한다. 
int LoadTOI( char *filename )
{			
	FILE *fp = 0;		// 021030 YGI
	char szFilePath[ FILENAME_MAX];
	char temptoiname[ FILENAME_MAX];
	WORD i,j;
	int  no;
	int  count = 0;
	int  ox, oy;
	short id;
	MAPOBJECT_old moold;
	MAPOBJECT mo;
	bool newtoi;

	CharUpper( filename );												
																				
	changemapobjectname( filename, temptoiname );										
																								
	if( g_Menu_Variable.m_pEventObjcet )
	{
		if (true == g_bIsChristmas) // Use the christmas toi2 during december
		{
			sprintf( szFilePath, "./object/%s/christmas_%s", temptoiname, g_Menu_Variable.m_pEventObjcet->object_toi );
			fp = Fopen( szFilePath, "rb" );
			if (NULL == fp)
			{
				sprintf( szFilePath, "./object/%s/%s", temptoiname, g_Menu_Variable.m_pEventObjcet->object_toi );
				fp = Fopen( szFilePath, "rb" );
			}
		}
		else
		{
			sprintf( szFilePath, "./object/%s/%s", temptoiname, g_Menu_Variable.m_pEventObjcet->object_toi );
			fp = Fopen( szFilePath, "rb" );
		}
	}

	if( !fp )
	{
		if (true == g_bIsChristmas) // Use the christmas toi2 during december
		{
			sprintf( szFilePath, "./object/%s/christmas_%s.toi2", temptoiname, filename );
			fp = Fopen( szFilePath, "rb" );
			if (NULL == fp)
			{
				sprintf( szFilePath, "./object/%s/%s.toi2", temptoiname, filename );
				fp = Fopen( szFilePath, "rb" );
			}
		}
		else
		{
			sprintf( szFilePath, "./object/%s/%s.toi2", temptoiname, filename );
			fp = Fopen( szFilePath, "rb" );
		}
	}

	if( fp )													
	{																		
		fread ( &TotalMapObject,	2,1, fp );
				
		if( TotalMapObject == 0xffff )
		{
			newtoi = true;
			fread ( &TotalMapObject,	2,1, fp ); 
		}
		else
			newtoi = false;
			
		// 021117 YGI
		if( TotalMapObject > MAX_MAPOBJECT_ ) 
		{
			Error( "Too Many MapObject" );
		}
			
		fread ( &TotalMapObjectID,	2,1, fp );															
		fseek( fp, TotalMapObject * sizeof( int ), SEEK_CUR );										
			
		for( i = 0 ; i < TotalMapObject ; i ++)
		{	
			if( count == 140 )
			{
				_asm nop;
			}


			if( newtoi == true )
			{
				fread( &mo, sizeof( MAPOBJECT ), 1, fp );
			}
			else
			{
				fread( &moold, sizeof( MAPOBJECT_old ), 1, fp );
				memcpy( &mo, &moold, sizeof( MAPOBJECT_old ) );
				mo.soundno		= 0;
				mo.soundframe	= 0;
				mo.sounddelay	= 0;
			}
			
			// object 를 서버와 연결 안하고 보여주려면.....
			if( mo.objectoritem == 0 || SysInfo.loaditemfromtoi2	|| 
				mo.objectoritem == ITEMTYPE_WORKTABLE_MINING		||	// 작업대 용광로(71)
				mo.objectoritem == ITEMTYPE_WORKTABLE_BATTLE		||
				mo.objectoritem == ITEMTYPE_WORKTABLE_SUNBAN		||
				mo.objectoritem == ITEMTYPE_WORKTABLE_MORU			||
				mo.objectoritem == ITEMTYPE_WORKTABLE_HWADUK		||	
				mo.objectoritem == ITEMTYPE_WORKTABLE_SIHUMKWAN		||	
				mo.objectoritem == ITEMTYPE_WORKTABLE_MULAE			||	
				mo.objectoritem == ITEMTYPE_WORKTABLE_BULDAE		||	
				mo.objectoritem == ITEMTYPE_SIGNBOARD				|| 
				mo.objectoritem == ITEMTYPE_DIRECTBOARD				||
				mo.objectoritem == ITEMTYPE_HP_UP					||
				mo.objectoritem == ITEMTYPE_MP_UP					||
				mo.objectoritem == ITEMTYPE_SCALE					||// 010614 khs
				mo.objectoritem == ITEMTYPE_NATION_STONE			||
				mo.objectoritem == ITEMTYPE_GUILD_STONE				||
				mo.objectoritem == ITEMTYPE_SENARIO_STONE			||
				mo.objectoritem == ITEMTYPE_COLOSSUS_STONE				)
			{
				//if( mo.id == 52428 ) continue;

				memcpy( &Mo[count], &mo, sizeof( MAPOBJECT ) );
													
				int frame = LoadMapObjectCSP( Mo[count].id, &MoImage[ Mo[count].id-1] );
				if( frame ) MoImage[ Mo[count].id-1].nFrameCount = frame;

				if (MoImage[ Mo[count].id-1].nFrameCount != 0)
				{
					Mo[count].curframe = Random(MoImage[ Mo[count].id-1].nFrameCount);
				}
				else
				{
					Mo[count].curframe = 0;
				}

				count ++;	
			}											
													
			if ( HandleRunning(&connections )  <= 0 )
			{										
				Release( &connections);
				return( 0 );
			}											
		}											
		fclose(fp);
			
		TotalMapObject = count;
			
		return 1;
	}		
	else	
	{		
		sprintf( szFilePath, "./object/%s/%s.toi", temptoiname, filename );
		fp = Fopen( szFilePath, "rb" );
		if( fp == NULL ) 
		{	
			TotalMapObject = 0;
		}	
		return 0;
	}			
			
	fread( &TotalMapObjectID, 2,1, fp );
	fseek( fp, 4 + 4 + 2, SEEK_CUR );
				
	for( i = 0 ; i < TotalMapObjectID ; i++ )
	{		
		fread( &no, 4,1, fp );
		fseek( fp, sizeof( SIZE ) + sizeof( WORD ) + sizeof( POINT ) + sizeof( LPDIRECTDRAWSURFACE) + sizeof( LPOBJECT ) * 2 +sizeof( struct tagOBJECTHEADER* ) * 2  + 2, SEEK_CUR );
			
		TempMoImageID[i] = no;
		for( j = 0 ; j < no ; j ++)
		{	
//			fread( &sizeof(t), sizeof( OBJECT ),1, fp);
			fread( &id,							2,1, fp );

			Mo[count].id	= id;		

			fread( &Mo[count].objectoritem,		2,1, fp );
			fread( &Mo[count].dum,				2,1, fp );
			fread( &Mo[count].animationType ,	1,1, fp );
			fread( &Mo[count].attr,				1,1, fp );
			fread( &Mo[count].attr1,			1,1, fp );
			fread( &Mo[count].attr2,			1,1, fp );
			
			fseek( fp, 2, SEEK_CUR );	
			fread( &Mo[count].dir,		2,1, fp );
			fseek( fp, 2, SEEK_CUR );	
			fread( &Mo[count].x,		4,1, fp );
			fread( &Mo[count].y,		4,1, fp );
			fread( &Mo[count].offx,		4,1, fp );
			fread( &Mo[count].offy,		4,1, fp );
			fread( &ox,	4,1, fp );		
			fread( &oy,	4,1, fp );
			fseek( fp, 8, SEEK_CUR );
			
			Mo[count].x = Mo[count].x * TILE_SIZE;
			Mo[count].y = Mo[count].y * TILE_SIZE;
			
			Mo[count].objectoritem = 0; // Mapobject... 

			if( j == 0 )
			{
//				MoImage[ Mo[count].id-1].spr[0].ox = ox;
//				MoImage[ Mo[count].id-1].spr[0].oy = oy;
							
				MoImage[ Mo[count].id-1].nFrameCount = LoadMapObjectCSP( Mo[count].id, &MoImage[ Mo[count].id-1] );
			}

			Mo[count].curframe = Random(MoImage[ Mo[count].id-1].nFrameCount );

			count ++;
		}	
	}				
			
	TotalMapObject = count;
	fclose(fp);
			
	return 1;
}		





int SaveTOI2( char *filename )
{
	FILE *fp, *wfp;
	char szFilePath[ FILENAME_MAX];
	char temptoiname[ FILENAME_MAX];
	WORD i,j,k;
	int  no = 0;
	int  count = 0;
	int  obcount = 0;
	int  code = 0xffff;

	typedef struct tt
	{
		int id;
		int animationType;
		int objectitem;
		int attr2;
		int light;
		int nFrameCount;
		int dx0[10],  dy0[10],  dx1[10],  dy1[10];
	}t_object;

	MAPOBJECT  **ob;

	changemapobjectname( filename, temptoiname );
	sprintf( szFilePath, "./object/%s/%s.toi2", temptoiname, filename );


	fp = Fopen( szFilePath, "rb" );
	if( fp ) 
	{
		fclose(fp);
		DWORD fileattr = GetFileAttributes( szFilePath );
		if( fileattr & FILE_ATTRIBUTE_READONLY )
		{
			JustMsg( "'%s'은 '읽기전용'으로 되어 있습니다. \n 저장하지 않았습니다.", szFilePath );
			return 0;
		}
	}

	fp = Fopen( szFilePath, "wb" );
	if( fp == NULL ) return 0;
					
	fwrite ( &code, 2,1, fp );
	fwrite ( &TotalMapObject, 2,1, fp );
	fwrite ( &TotalMapObjectID, 2,1, fp );
	fwrite ( TempMoImageID, TotalMapObject, sizeof( int ), fp );

	for( i = 0 ; i < TotalMapObject ; i++ )
	{
		////////////// 0810 lkh 추가 //////////////
		Mo[i].soundno = Mo[i].soundno%1000;		//하위 3자리가 실제 사운드 번호이고 상위 3자리는 사운드 카운트이기 때문
		fwrite( &Mo[i], sizeof( MAPOBJECT ), 1, fp );
	}
	fclose(fp);		
							
	sprintf( szFilePath, "./object/%s/%s_toi2.txt", temptoiname, filename );
	fp = Fopen( szFilePath, "wt" );
	if( fp == NULL ) 
	{
		fp = Fopen( szFilePath, "rb" );
		if( fp ) 
		{
			fclose(fp);
			DWORD fileattr = GetFileAttributes( szFilePath );
			if( fileattr & FILE_ATTRIBUTE_READONLY )
			{
				JustMsg( "'%s'은 '읽기전용'으로 되어 있습니다. \n 저장하지 않았습니다.", szFilePath );
				return 0;
			}
		}
		return 0;
	}
							
	fprintf( fp, "# 0:NO Animation  1:왕복  2:반복  3:문열림   4:문열림상태   5:문닫힘   6:문닫힌상태\n" );
	fprintf( fp, "# 집을수있다 AnimationType Type   Dir  찍는방법 찍는순서 그림자 Light  Lightxy            DontBox            총그림수		\n" );
					
	ob = NULL;
	MemAlloc( ob, sizeof( MAPOBJECT * ) * TotalMapObject );
	for( i = 0 ; i < TotalMapObject ; i ++)
		MemAlloc( ob[i], sizeof( MAPOBJECT ) );
	
	obcount =0;
	
	for( j = 0 ; j < TotalMapObject ; j ++)
	{		
		if( Mo[ j].objectoritem ) // item이면...
		{	
			for( i = 0 ; i < obcount ; i ++)
			{
				if( ob[i]->id == Mo[ j].id ) goto CONTINUE_;
			}

			memcpy( ob[ obcount], &Mo[j], sizeof( MAPOBJECT ) );
			
			fprintf( fp, "%d   0   %d   %d  1   %d 0  1  %d 0  -30   -20 0 0 -20 20 0 0 20     %d  ",
					Mo[ j].id + 13000,
					Mo[ j].animationType,
					Mo[ j].objectoritem,
					Mo[ j].attr2,
					Mo[ j].light,
					MoImage[ Mo[ j].id -1].nFrameCount );
					
				for( k = 0 ; k < MoImage[ Mo[ j].id-1 ].nFrameCount ; k ++)
					fprintf( fp, "%d %d %d %d   ", Mo[ j].dx0[k],  Mo[ j].dy0[k],  Mo[ j].dx1[k],  Mo[ j].dy1[k] );
				fprintf( fp, "\n"  );

				obcount++;
		}	
CONTINUE_:;
	}			

	fclose(fp);	



// Binary Data Save.....
	sprintf( szFilePath, "./object/%s/%s_toi2.c", temptoiname, filename );
	fp = Fopen( szFilePath, "wb" );
	if( fp == NULL ) return 0;
							
//		fprintf( fp, "# 0:NO Animation  1:왕복  2:반복  3:문열림   4:문열림상태   5:문닫힘   6:문닫힌상태\n" );
//		fprintf( fp, "# 집을수있다 AnimationType Type   Dir  찍는방법 찍는순서 그림자 Light  Lightxy            DontBox            총그림수		\n" );

	int t;
	fwrite( &obcount, sizeof( int ), 1, fp );
	for( i = 0 ; i < obcount ; i ++)
	{
		t = ob[ i]->id + 13000;						fwrite( &t, sizeof( int ), 1, fp);
		t = 0;										fwrite( &t, sizeof( int ), 1, fp);
		t = ob[ i]->animationType;					fwrite( &t, sizeof( int ), 1, fp);
		t = ob[ i]->objectoritem;					fwrite( &t, sizeof( int ), 1, fp);
		t = 1 ; 									fwrite( &t, sizeof( int ), 1, fp);
		t = ob[i]->attr2; 							fwrite( &t, sizeof( int ), 1, fp);
		t = 0 ; 									fwrite( &t, sizeof( int ), 1, fp);
		t = 1;  									fwrite( &t, sizeof( int ), 1, fp);
		t = ob[i]->light;							fwrite( &t, sizeof( int ), 1, fp);
		t = 0;  									fwrite( &t, sizeof( int ), 1, fp);
		t = -30;									fwrite( &t, sizeof( int ), 1, fp);
		t = -20;									fwrite( &t, sizeof( int ), 1, fp);
		t = 0; 										fwrite( &t, sizeof( int ), 1, fp);
		t = 0; 										fwrite( &t, sizeof( int ), 1, fp);
		t = -20;									fwrite( &t, sizeof( int ), 1, fp);
		t = 20 ;									fwrite( &t, sizeof( int ), 1, fp);
		t = 0; 										fwrite( &t, sizeof( int ), 1, fp);
		t = 0; 										fwrite( &t, sizeof( int ), 1, fp);
		t = 20;;									fwrite( &t, sizeof( int ), 1, fp);
		t = MoImage[ ob[i]->id -1].nFrameCount;		fwrite( &t, sizeof( int ), 1, fp); // 20개 * sizeof( int ) = 80;

		fwrite( &ob[i]->dx0, sizeof( short ), ITEM_FRAME_MAX_, fp );
		fwrite( &ob[i]->dy0, sizeof( short ), ITEM_FRAME_MAX_, fp );
		fwrite( &ob[i]->dx1, sizeof( short ), ITEM_FRAME_MAX_, fp );
		fwrite( &ob[i]->dy1, sizeof( short ), ITEM_FRAME_MAX_, fp );	// sizeof( int ) * 40  = 160///
	}
	fclose(fp );

	for( i = 0 ; i < obcount ; i ++)
		MemFree( ob[i] );
	MemFree( ob );

	// 체크섬을 집어넣는다. 
	sprintf( szFilePath, "./object/%s/%s_toi2.c", temptoiname, filename );
	fp = Fopen( szFilePath, "rb" );
	sprintf( szFilePath, "./object/%s/%s_toi2.b", temptoiname, filename );
	wfp = Fopen( szFilePath, "wb" );

	DWORD fl = filelength( fileno( fp ) );

	char crc = 0, tcrc;
	for( k = 0 ; k < fl ; k ++)
	{
		fread( &tcrc, 1,1, fp );
		crc += tcrc;
		fwrite( &tcrc, 1,1, wfp );
	}

	fwrite( &crc, 1,1, wfp );

	fclose( fp );
	fclose(wfp );
											
	return 1;				
}
//////////////////////////////// 여기까지 ////////////////////////////////

						
void FreeTOI( void )
{
	for( int i = 0 ; i < MAX_MAPOBJECTIMAGE ; i ++)
	{
		for( int j = 0 ; j < ITEM_FRAME_MAX_ ; j ++)
		{
			MemFree( MoImage[i].spr[j].img );
		}
	}
}


///////////////////////// SoundUp lkh 수정 /////////////////////////		
void InsertMapObject( int moid, int x, int y )
{	
	int i;
	
	if( TotalMapObject >= MAX_MAPOBJECT_ )
	{	
		JustMsg( "음,, 벌서 MapObject갯수가 5000넘었어요.. 더이상은 무리에요.." );
		return;
	}	
	
	for( i = 0 ; i < TotalMapObject ; i ++)
	{	
		if( Mo[i].id == moid ) break;
	}	
	
	if( i == TotalMapObject )
	{	
		memset( &Mo[TotalMapObject], 0, sizeof( MAPOBJECT ) );
	}	
	else
	{	
		memcpy( &Mo[ TotalMapObject], &Mo[i], sizeof( MAPOBJECT ));
	}		
	
	Mo[TotalMapObject].id	= moid;
	Mo[TotalMapObject].x	= x;
	Mo[TotalMapObject].y	= y;

	Mo[TotalMapObject].animationType = OBJECTANI_NO;
	
	LPMAPOBJECT mo = &Mo[TotalMapObject];

									
	int frame = LoadMapObjectCSP( mo->id, &MoImage[ mo->id-1] );
	if( frame ) MoImage[ mo->id-1].nFrameCount = frame;

	Mo[TotalMapObject].soundno		= 0;
	Mo[TotalMapObject].soundframe	= 0;
	Mo[TotalMapObject].sounddelay	= 0;
	TotalMapObject ++;
}

void DeleteMapObject_sub( int no )
{	
	MAPOBJECT mo;	
	
	memcpy( &mo, &Mo[ no], sizeof( MAPOBJECT ) );
	memmove(   &Mo[no],   &Mo[no+1], sizeof(MAPOBJECT) * (MAX_MAPOBJECT_ - no - 1 ));
	TotalMapObject --;
	
	int c = 0;
	int i;
	for(i = 0 ; i < TotalMapObject ; i ++)
	{	
		if( Mo[i].id == mo.id ) c++;
	}	
	if( c == 0 )
	{
		for( i = 0 ; i < ITEM_FRAME_MAX_ ; i ++)
			MemFree( MoImage[ mo.id-1].spr[i].img );
	}
}	

void DeleteMapObject( int no )
{	

	
	if( no == -1 ) return;
	
	if( YesOrNo( "맵오브젝트를 없애려구 ?", "혹시나.." ) == IDYES )
	{}
	else
	{
		return;
	}

	DeleteMapObject_sub( no );
}	



void DeleteMapObjectAll( int no )
{	
	int j;

	
	if( no == -1 ) return;
	
	if( YesOrNo( "맵에서 no번 맵오브젝트를 모두 없애려구 ????", "혹시나.." ) == IDYES )
	{}
	else
	{
		return;
	}
	
	for( j = 0 ; j < TotalMapObject ; j ++)
	{
		if( Mo[ j].id == no )
		{
			DeleteMapObject_sub( j );
			j--;
		}
	}
}	
	
void LoadTempSprite( int id )
{	
	static int	mapobjectno = -1;
	if( id == mapobjectno ) return;
	mapobjectno = id;
	
	char	filename[ FILENAME_MAX];
	FILE   *fp;
	int    lev;
	
	MemFree( TempMoImage.spr[0].img );
	
	lev = id / 1000;
	if( lev == 1 ) lev = 0;
	
	sprintf( filename, "./object/%d/%04d.csp", lev, id );
	fp = Fopen( filename, "rb" );
	if( fp == NULL ) return;
		fread( &TempMoImage.spr[0].xl, sizeof( short ), 1, fp);
		fread( &TempMoImage.spr[0].yl, sizeof( short ), 1, fp);
		fread( &TempMoImage.spr[0].ox, sizeof( short ), 1, fp);
		fread( &TempMoImage.spr[0].oy, sizeof( short ), 1, fp);
		fread( &TempMoImage.spr[0].size, sizeof( unsigned int ), 1, fp);
		TempMoImage.spr[0].img = NULL;
		MemAlloc( TempMoImage.spr[0].img, TempMoImage.spr[0].size );
		fread( TempMoImage.spr[0].img, TempMoImage.spr[0].size, 1, fp);
		convert565to555( &TempMoImage.spr[0] );
		fclose(fp);	
}
