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
#include "StdAfx.h"
#include <Ddraw.h>
#include <Stdio.h>
#include <io.h>
#include "Dragon.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"
#include "map.h"
#include "GameProc.h"
#include "Convert565to555.h"
#include "Tool.h"
#include "Skill.h"
#include "CharDataTable.h"
#include "Char.h"
#include "ItemTable.h"
#include "DirectSound.h"
#include "Effect.h"
#include "Path.h"
#include "Object.h"
#include "hangul.h"
#include "item.h"

extern MAP		g_Map;		
extern int		g_MouseItemType;	// menu.cpp
extern int		g_MouseItemNumber;
extern int		g_StartMenuOn;

LPITEMGROUND g_Item, CursorCheckedItem;
LPITEMSPRITE  g_ItemSprite;

extern void changemapobjectname( char *oldname, char *newname );

int LoadItemSprite( Spr *s, int no, int type, int order, int return_null = 0 )
{	
	char temp[ FILENAME_MAX];
	char tempitemname[ FILENAME_MAX];
	FILE *fp = NULL;

	changemapobjectname( MapName, tempitemname );
	
	if( type < 13 )		// 0811 YGI
	{	
		sprintf( temp, "./item/%07d.CSP", no ); // item
		fp = Fopen( temp, "rb" );
		if( fp == NULL )
		{
				sprintf( temp, "./item/0000000.CSP" ); // item
				fp = Fopen( temp, "rb" );
		}
	}	
	else
#ifdef _MAPOBJECT_DIRECTORY_CHANGE_
	{	
		
		int lev;

		if( order == 0 )
		{	no -= 13000;
		
			lev = no / 1000;
			if( lev == 1 ) lev = 0;

			sprintf( temp, "./object/%d/%04d.csp", lev, no );
			fp = Fopen( temp, "rb" );
			if( fp == NULL ) 
			{
				if( !return_null )
				{
					sprintf( temp, "./item/0000000.CSP" ); // item
					fp = Fopen( temp, "rb" );
				}
			}
		}	
		else 
		{
			no -= 1300000;

			lev = no / 100000;
			if( lev == 1 ) lev = 0;

			sprintf( temp, "./object/%d/%06d.csp", lev, no );
			fp = Fopen( temp, "rb" );
			if( fp == NULL ) 
			{

				if( !return_null )
				{
					sprintf( temp, "./item/0000000.CSP" ); // item
					fp = Fopen( temp, "rb" );
				}
			}
		}
	}
#else
	{
		if( order == 0 )
		{	
			sprintf( temp, "./object/%s/%04d.csp", tempitemname, no - 13000);
			fp = Fopen( temp, "rb" );
			if( fp == NULL ) 
			{
				if( !return_null )
				{
					sprintf( temp, "./item/0000000.CSP" ); // item
					fp = Fopen( temp, "rb" );
				}
			}
		}	
		else 
		{
			sprintf( temp, "./object/%s/%06d.csp", tempitemname, no - 1300000 );
			fp = Fopen( temp, "rb" );
			if( fp == NULL ) 
			{
				if( !return_null )
				{
					sprintf( temp, "./item/0000000.CSP" ); // item
					fp = Fopen( temp, "rb" );
				}
			}
		}
	}
#endif

	
	if( fp )
	{	
		fread( &s->xl, sizeof( short ), 1, fp);
		fread( &s->yl, sizeof( short ), 1, fp);
		fread( &s->ox, sizeof( short ), 1, fp);
		fread( &s->oy, sizeof( short ), 1, fp);
//		s->oy = s->yl * 80/ 100; //sizeof( short ), 1, fp);
		fread( &s->size, sizeof( unsigned int ), 1, fp);
		s->img = NULL;
		MemAlloc( s->img, s->size );
		fread( s->img, s->size, 1, fp );
		convert565to555( s );
		fclose(fp);
		return 1;
	}
	return 0;
}		
		
		
		
void FreeItemSprite( LPITEMSPRITE i)
{
	MemFree( i->sp.img );
}


LPITEMSPRITE FindItemSprite( int no )
{
	LPITEMSPRITE is = g_ItemSprite;

	while( is != NULL )
	{
		if( is->no == no ) return is;
		is = is->next;
	}

	return NULL;
}


void DeleteItemSprite( int no )
{		
	LPITEMSPRITE  t = g_ItemSprite;
			
	while( t != NULL )
	{		
		if( t->no == no )
		{	
			t->count --;
					
			if( t->count == 0 )
			{		
				if( t == g_ItemSprite )
				{	
					t = g_ItemSprite->next;
					FreeItemSprite( g_ItemSprite );
					MemFree( g_ItemSprite );
					g_ItemSprite = t;
					if( t != NULL )	g_ItemSprite->prev = NULL;
					return;
				}
				else 
				{
					if ( t->prev != NULL )
					{
						t->prev->next = t->next;
					}
					if( t->next != NULL )
					{
						t->next->prev = t->prev;
					}
					FreeItemSprite( t );
					MemFree( t );
					return;
				}
			}
		}	
		t = t->next;
	}		
}		

// 020620 YGI
LPITEMSPRITE AddItemSprite( int no, int type, int order, int return_flag = 0  )
{			
	LPITEMSPRITE temp, s = FindItemSprite( no );
			
	if( s == NULL )
	{		
		if( g_ItemSprite == NULL )
		{	
			MemAlloc( g_ItemSprite, sizeof( ITEMSPRITE ) );
			g_ItemSprite->no = no;
			g_ItemSprite->count = 1;
			
			LoadItemSprite( &g_ItemSprite->sp, no, type, order );
			g_ItemSprite->next = NULL;
			g_ItemSprite->prev = NULL;
			
			s = g_ItemSprite;
		}	
		else
		{
			MemAlloc(s,  sizeof( ITEMSPRITE ) );
			s->no = no;
			s->count = 1;
			if( LoadItemSprite( &s->sp, no, type, order, return_flag ) )
			{
				s->next = NULL;
					
				temp = g_ItemSprite;
				temp->prev = s;
				s->next = temp;
				s->prev = NULL;
				g_ItemSprite = s;
			}
			else
			{
				MemFree( s );
				s = 0;
			}
		}	
	}		
	else	
	{		
		s->count ++;
	}		
			
	return s;
}													
	
extern void changemapobjectname( char *oldname, char *newname );

#include "menuset.h"//021030 YGI							
// 020620 YGI											
void LoadItem( LPITEMGROUND i )
{										
	char temp[ FILENAME_MAX];
	char tempitemname[ FILENAME_MAX];
	FILE *fp;									
	int  j, m;
	DWORD k;									
	LPITEMSPRITE t;							
	int itemno;		
	fpos_t  pos;
	int obcount;
														
	int type = i->no / 1000;
	int no = i->no % 1000;
	int getable, anitype,itemtype, dir,where,sorting,shadow,dx1, dy1,	dx2, dy2, dx3, dy3,	dx4, dy4,framecount;
	int light, lightx, lighty;

	int dsx, dsy, ddx, ddy;
	changemapobjectname( MapName, tempitemname );
	
	if( i->no > 25000 )
	{
		i->getable	  = 1;
		i->Anitype 	  = 0;
		i->type		  = 0; 
		i->dirno	  = 0;
		i->Where	  = 0;	
		i->Sorttype   = 0;
		i->shadow	  = 0;	
		i->dx[0]	  = 0;
		i->dx[1]	  = 0;
		i->dx[2]	  = 0;
		i->dx[3]	  = 0;
		i->framecount = 1;
		i->dsx[0] = 0; i->dsy[1] = 0; i->ddx[0] = 0; i->ddy[1] = 0;

		int spr_no = i->no-25000+13000;
		type = spr_no/1000;

		// È°¼ºÈ­ ÀÏ°æ¿ì // 020701 YGI
		if( ( i->attr[IATTR_ATTR] & IA2_SCENARIO_POTAL ) && ( i->attr[IATTR_ATTR] & IA2_OPENED ) )
		{
			// ¸¶Ä¡ ¸Ê ¿ÀºêÁ§Æ®ÀÇ OBJECTANI_1_LOOP Ã³·³ Ãâ·ÂÇÏ±â À§ÇØ
			//temp_item = i;
			DWORD attr = OBJECTSPRITE_ALPHA;
			if( i->attr[IATTR_ATTR] & IA2_POISONED ) 
			{
				attr = OBJECTSPRITE_TRANS;
			}
			i->Anitype 	  = ITEMANIMATION_ACTIVE;
			int al = 15;
			al <<= 3;
			i->attr[IATTR_SPRITE_TYPE] = al;
			i->attr[IATTR_SPRITE_TYPE] = (i->attr[IATTR_SPRITE_TYPE] & 0xf8) + attr;
			for( j = 0 ; j < 99 ; j ++)
			{		
				if( j == 0 )	t = AddItemSprite( spr_no , type, 0 );
				else 			t = AddItemSprite( spr_no * 100 + j, type, j, 1 );
				if( !t ) break;
				i->spr[j] = &t->sp;
			}
			i->framecount = j;
		}
		else		// ºñÈ°¼ºÈ­ ÀÏ °æ¿ì
		{
			t = AddItemSprite( spr_no , type, 0 );
			i->spr[0] = &t->sp;
		}
		return;
	}

	if( type < 13 )
	{										
		/*# AnimationType : 0:NO Animation  1:¹Ýº¹  2:¿Õº¹   3:¹®¿­¸²   4:¹®¿­¸²»óÅÂ   5:¹®´ÝÈû   6:¹®´ÝÈù»óÅÂ		
		# Type - 0:º¸Åë Item  1: ÀÇÀÚ   2: Table   3: µµ¾î  4:Box
		# ÁýÀ»¼öÀÖ´Ù AnimationType Type   Dir  Âï´Â¹æ¹ý  Âï´Â¼ø¼­ ±×¸²ÀÚ             DontBox         ÃÑ±×¸²¼ö		*/
		i->getable	  = 1;
		
		if (i->no/1000 == 6)
		{	//< CSD-031101
			CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(i->no));

			if (pWeapon != NULL)
			{
				const int nEffect = pWeapon->effect;

				i->Anitype = (nEffect > 0) ? 1:0;
			}
			else
			{
				i->Anitype = 0;
			}
		}	
		else
		{
			i->Anitype = 0;
		}	//> CSD-031101
		
		i->type		  = 0; 
		i->dirno	  = 0;
		i->Where	  = 0;	
		i->Sorttype   = 0;
		i->shadow	  = 0;	
		i->dx[0]	  = 0;
		i->dx[1]	  = 0;
		i->dx[2]	  = 0;
		i->dx[3]	  = 0;
		i->framecount = 1;
		i->dsx[0] = 0; i->dsy[1] = 0; i->ddx[0] = 0; i->ddy[1] = 0;

		CItem *ii = ItemUnit( type, no );		//	0527 YGI
		if( !ii ) return;
		int spr_no = ii->GetObjectNumber()/100;
		
		for( m = 0 ; m < 99; m++ )			// 010509 YGI
		{	
			sprintf( temp, "./item/%05d%02d.CSP",  spr_no, m );
			fp = Fopen( temp, "rb" );
			if( fp == NULL )  break;
			fclose(fp);
		}		
		i->framecount = m;
				
		for( j = 0 ; j < m ; j ++)
		{		
			t = AddItemSprite( spr_no * 100 + j,   type, 0 );
			i->spr[j] = &t->sp;
		}		
	}								
	else								
	{			
		//021030 YGI
		fp = 0;
		if( g_Menu_Variable.m_pEventObjcet )
		{
			if (true == g_bIsChristmas) // Use the christmas toi2.b during december
			{
				sprintf( temp, "./object/christmas_%s/%s", tempitemname, g_Menu_Variable.m_pEventObjcet->object_b );
				fp = Fopen( temp,"rb" );
				if (NULL == fp) // Doesn't have a christmas toi2.b file
				{
					sprintf( temp, "./object/%s/%s", tempitemname, g_Menu_Variable.m_pEventObjcet->object_b );
					fp = Fopen( temp,"rb" );
				}
			}
			else
			{
				sprintf( temp, "./object/%s/%s", tempitemname, g_Menu_Variable.m_pEventObjcet->object_b );
				fp = Fopen( temp,"rb" );
			}
		}
		if( !fp )
		{
			if (true == g_bIsChristmas) // Use the christmas toi2.b during december
			{
				sprintf( temp, "./object/%s/christmas_%s_toi2.b", tempitemname, MapName );
				fp = Fopen( temp,"rb" );
				if (NULL == fp) // Doesn't have a christmas toi2.b file
				{
					sprintf( temp, "./object/%s/%s_toi2.b", tempitemname, MapName );
					fp = Fopen( temp,"rb" );
				}
			}
			else
			{
				sprintf( temp, "./object/%s/%s_toi2.b", tempitemname, MapName );
				fp = Fopen( temp,"rb" );
			}
		}
		// ---------------------------
		
		if( fp != NULL )
		{									
			static int check;
			check++;
			if( (check % 5)  == 2 ) // Ã¼Å©¼¶À» ±¸ÇÏ¿© ºñ±³ÇØº»´Ù. 
			{		
				DWORD fl = filelength( fileno( fp ) );
				char  *tbuf, *buf = NULL;
				char crc = 0, crc1;
				MemAlloc( buf, fl );
				fread( buf, fl-1, 1, fp );
				tbuf = buf;
				for( k = 0 ; k < fl-1 ; k ++ )
				{	
					crc += *tbuf ++;
				}	
				fread( &crc1, 1,1, fp );
				if( CheckSumError == 0 ) 
				{
					CheckSumError = abs( crc - crc1 );
					if( CheckSumError )  JustMsg( " LoadItem CheckSumError " );
				}
				fclose(fp);
				MemFree( buf );
				fp = Fopen(  temp,"rb" );
			}	
				
				
			fread( &obcount, sizeof( int), 1, fp );
					
			for( j = 0 ; j < obcount ; j ++)
			{		
				fseek(fp, j * (sizeof( int ) * 20 + sizeof( short int ) * 4 * ITEM_FRAME_MAX_ ) + sizeof( int ), SEEK_SET );
				fread( &no, sizeof( int ), 1, fp );
				if( no == i->no )
				{	
					fread( &getable,	sizeof( int ), 1, fp );
					fread( &anitype,	sizeof( int ), 1, fp );
					fread( &itemtype,	sizeof( int ), 1, fp );
					fread( &dir,		sizeof( int ), 1, fp );
					fread( &where,		sizeof( int ), 1, fp );
					fread( &sorting,	sizeof( int ), 1, fp );
					fread( &shadow,		sizeof( int ), 1, fp );
					fread( &light,		sizeof( int ), 1, fp );
					fread( &lightx,		sizeof( int ), 1, fp );
					fread( &lighty,		sizeof( int ), 1, fp );
					fread( &dx1,		sizeof( int ), 1, fp );
					fread( &dy1,		sizeof( int ), 1, fp );
					fread( &dx2,		sizeof( int ), 1, fp );
					fread( &dy2,		sizeof( int ), 1, fp );
					fread( &dx3,		sizeof( int ), 1, fp );
					fread( &dy3,		sizeof( int ), 1, fp );
					fread( &dx4,		sizeof( int ), 1, fp );
					fread( &dy4,		sizeof( int ), 1, fp );
					fread( &framecount, sizeof( int ), 1, fp );


					i->getable	  = getable;
					i->Anitype 	  = anitype;
					i->type		  = itemtype;
					i->dirno	  = dir;
					i->Where	  = where;
					i->Sorttype   = sorting;
					i->shadow	  = shadow;
					i->dx[0]	  = i->x + dx1 + i->offx, i->dy[0] = i->y + dy1 + i->offy;
					i->dx[1]	  = i->x + dx2 + i->offx, i->dy[1] = i->y + dy2 + i->offy;
					i->dx[2]	  = i->x + dx3 + i->offx, i->dy[2] = i->y + dy3 + i->offy;
					i->dx[3]	  = i->x + dx4 + i->offx, i->dy[3] = i->y + dy4 + i->offy;
					i->framecount = framecount;
					i->dsx[0] = 0; i->dsy[1] = 0; i->ddx[0] = 0; i->ddy[1] = 0;

					fread( i->dsx, sizeof( short),  ITEM_FRAME_MAX_, fp );
					fread( i->dsy, sizeof( short),  ITEM_FRAME_MAX_, fp );
					fread( i->ddx, sizeof( short),  ITEM_FRAME_MAX_, fp );
					fread( i->ddy, sizeof( short),  ITEM_FRAME_MAX_, fp );


					for( int kk = 0; kk < i->framecount ; kk ++)
					{	
						i->dsx[kk] += i->x ;
						i->dsy[kk] += i->y ;
						i->ddx[kk] += i->x ;
						i->ddy[kk] += i->y ;

						if( kk == 0 )	t = AddItemSprite( i->no, type, kk );
						else 			t = AddItemSprite( i->no * 100 + kk, type, kk );
						i->spr[kk] = &t->sp;
					}

					fclose(fp ); 
					return;
				}
			}

			fclose(fp );
			return;
		}
		else
		{
			sprintf( temp, "./object/%s/%s_toi2.txt", tempitemname, MapName );
			fp = Fopen( temp,"rt" );
			if( fp != NULL )
			{									
				fgets( temp, FILENAME_MAX, fp);
				fgets( temp, FILENAME_MAX, fp);
					
				fgetpos( fp, &pos );
				while( fgets( temp, FILENAME_MAX,fp ))
				{					
					
					sscanf( temp, "%d ", &itemno );
					if( itemno ==  i->no )
					{						
						fsetpos( fp, &pos );
						goto NEXT_;
					}					
					fgetpos( fp, &pos );
				}								
				fclose(fp);					
				return;								
	NEXT_:									
				//# ÁýÀ»¼öÀÖ´Ù AnimationType Dir  Âï´Â¹æ¹ý Âï´Â¼ø¼­ ±×¸²ÀÚ             DontBox         ÃÑ±×¸²¼ö 
				//0    1          0           1      0         0      1    -20 0 0 -20  20 0  0 20        1      0
				fscanf( fp, "%d %d %d %d %d %d %d %d %d %d "
							  "%d %d %d %d %d %d %d %d %d %d ", 
						&no, 
						&getable, 
						&anitype,
						&itemtype,
						&dir,
						&where,
						&sorting,
						&shadow,
						&light, &lightx, &lighty,
						&dx1, &dy1,		&dx2, &dy2,		&dx3, &dy3,		&dx4, &dy4,
						&framecount );
							
					i->getable	  = getable;
					i->Anitype 	  = anitype;
					i->type		  = itemtype;
					i->dirno	  = dir;
					i->Where	  = where;
					i->Sorttype   = sorting;
					i->shadow	  = shadow;
					i->dx[0]	  = i->x + dx1 + i->offx, i->dy[0] = i->y + dy1 + i->offy;
					i->dx[1]	  = i->x + dx2 + i->offx, i->dy[1] = i->y + dy2 + i->offy;
					i->dx[2]	  = i->x + dx3 + i->offx, i->dy[2] = i->y + dy3 + i->offy;
					i->dx[3]	  = i->x + dx4 + i->offx, i->dy[3] = i->y + dy4 + i->offy;
					i->framecount = framecount;
					i->dsx[0] = 0; i->dsy[1] = 0; i->ddx[0] = 0; i->ddy[1] = 0;
					for( j = 0; j < i->framecount ; j ++)
					{
						fscanf( fp, "%d %d %d %d", &dsx, &dsy, &ddx, &ddy );
						i->dsx[j] = dsx + i->x ;
						i->dsy[j] = dsy + i->y ;
						i->ddx[j] = ddx + i->x ;
						i->ddy[j] = ddy + i->y ;
						
						if( j == 0 )
						{
							t = AddItemSprite( i->no, type, j );
						}
						else 
						{
							t = AddItemSprite( i->no * 100 + j, type, j );
						}

						i->spr[j] = &t->sp;
					}
				fclose(fp);
			}
		}
	}	
}		
		
		
void AddItemList( LPITEMGROUND i )
{		
	LPITEMGROUND t, temp; 
	int mx, my;
		
	if( i->no > 13000 )
	{
		mx = (i->x )/ TILE_SIZE;
		my = (i->y )/ TILE_SIZE;
	}
	else
	{
		mx = (i->x )/ TILE_SIZE;
		my = (i->y + 20)/ TILE_SIZE;
		i->y += 20;
	}

		
	if( g_Item == NULL )	 
	{	
		MemAlloc( g_Item, sizeof( ITEMGROUND ));
		g_Item->server_id = i->server_id;
		g_Item->no	  = i->no;
		g_Item->sx    = i->sx;
		g_Item->sy	  = i->sy;
		g_Item->offx  = i->offx;
		g_Item->offy  = i->offy;
		g_Item->x     = i->x; 
		g_Item->y     = i->y; 
		g_Item->mx    = i->x / TILE_SIZE;
		g_Item->my    = i->y / TILE_SIZE;

		g_Item->attr[0]	= i->attr[0];
		g_Item->attr[1]	= i->attr[1];
		g_Item->attr[2]	= i->attr[2];
		g_Item->attr[3]	= i->attr[3];
		g_Item->attr[4]	= i->attr[4];
		g_Item->attr[5]	= i->attr[5];

		LoadItem( g_Item );  

		g_Item->prev = NULL; 
		g_Item->next = NULL; 
	}	
	else
	{	
		t = NULL;
		MemAlloc( t, sizeof( ITEMGROUND ));
		t->server_id= i->server_id;
		t->no		= i->no;
		t->sx		= i->sx;
		t->sy		= i->sy;
		t->offx		= i->offx;
		t->offy		= i->offy;
		t->x		= i->x;
		t->y		= i->y;
		t->mx		= i->x / TILE_SIZE;
		t->my		= i->y / TILE_SIZE;
		
		t->attr[0]	= i->attr[0];
		t->attr[1]	= i->attr[1];
		t->attr[2]	= i->attr[2];
		t->attr[3]	= i->attr[3];
		t->attr[4]	= i->attr[4];
		t->attr[5]	= i->attr[5];


		LoadItem( t );
		temp = g_Item;
		temp->prev = t;
		t->next = temp;
		t->prev = NULL;
		g_Item = t;
	}	
		
	// AddµÈ Item¿¡ AnimationÁ¤¸®..
//-----------------------
		if( g_Item->attr[2] & IA2_FARM0 )	g_Item->Anitype = ITEMANIMATION_FARM0;
		if( g_Item->attr[2] & IA2_FARM1 )	g_Item->Anitype = ITEMANIMATION_FARM1;
		if( g_Item->attr[2] & IA2_FARM2 )	g_Item->Anitype = ITEMANIMATION_FARM2;
		if( g_Item->attr[2] & IA2_FARM3 )	g_Item->Anitype = ITEMANIMATION_FARM3;
		if( g_Item->attr[2] & IA2_FARM4 )	g_Item->Anitype = ITEMANIMATION_FARM4;
		if( g_Item->attr[2] & IA2_FARM5 )	g_Item->Anitype = ITEMANIMATION_FARM5;
		if( g_Item->attr[2] & IA2_FARM6 )	g_Item->Anitype = ITEMANIMATION_FARM6;
		if( g_Item->attr[2] & IA2_FARM7 )	g_Item->Anitype = ITEMANIMATION_FARM7;

		if( g_Item->attr[2] & IA2_FIRE  )	g_Item->Anitype = ITEMANIMATION_FIRE;
		
		if( g_Item->attr[ IATTR_ATTR] & IA2_BOX )
		{
			g_Item->type = ITEMTYPE_BOX;
			if( g_Item->attr[ IATTR_ATTR] & IA2_OPENED )	g_Item->Anitype = ITEMANIMATION_OPENED;
			else											g_Item->Anitype = ITEMANIMATION_CLOSED;
		}
		else if( g_Item->attr[ IATTR_ATTR] & IA2_DOOR )
		{
			g_Item->type = ITEMTYPE_DOOR;
			if( g_Item->attr[ IATTR_ATTR] & IA2_OPENED )	g_Item->Anitype = ITEMANIMATION_OPENED;
			else											g_Item->Anitype = ITEMANIMATION_CLOSED;
		}
		else if( g_Item->attr[ IATTR_ATTR] & IA2_TRAPED1 )	g_Item->curframe = rand()%4+1;
		else 
		{
			CItem *t = ItemUnit( g_Item->no / 1000, g_Item->no % 1000 );
			if( t )
			{//wait i think iknow why cause the mapserver onlym generates it normally and not by weapon type plant type ect so would think its a normal itemaccessory ok
				if( t->GetMouse_MR_Effect() )
					g_Item->Anitype = ITEMANIMATION_REPEAT;
			}
		}




}		
		
		
		
void DeleteItemList( int server_id )
{		
	int i;
	LPITEMGROUND  t = g_Item;
			
	while( t != NULL )
	{		
		if( t->server_id == server_id )
		{	
			if( t == g_Item )
			{
				t = g_Item->next;
				for( i = 0 ; i < g_Item->framecount ; i ++)
					DeleteItemSprite( g_Item->no * 100 + i );
				
				MemFree( g_Item );
				g_Item = t;
				if( t != NULL )	g_Item->prev = NULL;
				return;
			}
			else 
			{
				if ( t->prev != NULL )
				{
					t->prev->next = t->next;
				}
				if( t->next != NULL )
				{
					t->next->prev = t->prev;
				}
				for( i = 0 ; i < t->framecount ; i ++)
					DeleteItemSprite( t->no * 100 + i );
				MemFree( t );
		
				return;
			}
		}	
		t = t->next;
	}		
}		
	
	
//	
LPITEMGROUND FindItemList( int server_id )
{	   	
	LPITEMGROUND t;
	t = g_Item;
	while( t != NULL )
	{	
		if( t->server_id == server_id )
		{
			return t;
		}
		t = t->next;
	}		
	return NULL;
}		
			
	
	
	
//	¸ÊÀº Àý´ëÁÂÇ¥...
LPITEMGROUND FindItemList( int x, int y )
{	   	
	LPITEMGROUND t;
	t = g_Item;
	while( t != NULL )
	{	
		if( BoxAndDotCrash( x - 16, y-16, 32, 32, t->x, t->y ) )
		{
			return t;
		}
		t = t->next;
	}		
	return NULL;
}		
	
void DestroyItemList( void )
{	
	LPITEMGROUND t;
	int i;
	t = g_Item;
	while( t != NULL )
	{	
		t = g_Item->next;
			for( i = 0 ; i < g_Item->framecount ; i ++)
		DeleteItemSprite( g_Item->no * 100 + i );
		MemFree( g_Item );
		g_Item = t;
	}		
		
	return;
}		
		
		
				
//---------------  Item Animation... 
	
	
void RecvItemAttrChange( t_item_attr_change *p )
{	
	LPITEMGROUND i = FindItemList( p->item_id );
	if( i == NULL ) return;
	int oldattr = i->attr[ p->array];
	int attr = p->attr;
	
	
 switch( p->array )
 {
	case IATTR_ATTR :
	{

		//		110       ¿ì¸®¹®¿°.wav
		//		111       ¿ì¸®¹®´ÝÀ½.wav

		if( oldattr & IA2_DOOR )// ¹®¿¡ °üÇÑ ¼Ó¼º Check....
		{
			if( oldattr & IA2_OPENED ) // ¿­·ÈÀ¸¸é..
			{
				if( ( attr & IA2_OPENED ) == 0 ) // À½ ´ÝÀ¸¶õ¸í·ÉÀÌ±º.
					i->Anitype = ITEMANIMATION_CLOSING;

				int distx=i->x-Hero->x;						// LTS SOUND
				int disty=i->y-Hero->y;
				PlayListAutoSounds( 141,distx,disty,0 );
			}
			else
			{
				if( (attr & IA2_OPENED) && (i->Anitype != ITEMANIMATION_OPENED) ) // À½ ¿­¶ó´Â ¸í·ÉÀÌ±º.
					i->Anitype = ITEMANIMATION_OPENNING;

				int distx=i->x-Hero->x;						// LTS SOUND
				int disty=i->y-Hero->y;
				PlayListAutoSounds( 140,distx,disty,0 );
			}
		}
		else
		if( oldattr & IA2_BOX )// Box¿¡ °üÇÑ ¼Ó¼º Check..
		{
			if( oldattr & IA2_OPENED ) // ¿­·ÈÀ¸¸é..
			{
				if( ( attr & IA2_OPENED ) == 0 ) // À½ ´ÝÀ¸¶õ¸í·ÉÀÌ±º.
					i->Anitype = ITEMANIMATION_CLOSING;
				int distx=i->x-Hero->x;						// LTS SOUND
				int disty=i->y-Hero->y;
				PlayListAutoSounds( 149,distx,disty,0 );

			}
			else
			{
				if( (attr & IA2_OPENED)  && (i->Anitype != ITEMANIMATION_OPENED) ) // À½ ¿­¶ó´Â ¸í·ÉÀÌ±º.
					i->Anitype = ITEMANIMATION_OPENNING;
				int distx=i->x-Hero->x;						// LTS SOUND
				int disty=i->y-Hero->y;
				PlayListAutoSounds( 148,distx,disty,0 );

			}
		}

		if( attr & IA2_FARM0 )	i->Anitype = ITEMANIMATION_FARM0;
		if( attr & IA2_FARM1 )	i->Anitype = ITEMANIMATION_FARM1;
		if( attr & IA2_FARM2 )	i->Anitype = ITEMANIMATION_FARM2;
		if( attr & IA2_FARM3 )	i->Anitype = ITEMANIMATION_FARM3;
		if( attr & IA2_FARM4 )	i->Anitype = ITEMANIMATION_FARM4;
		if( attr & IA2_FARM5 )	i->Anitype = ITEMANIMATION_FARM5;
		if( attr & IA2_FARM6 )	i->Anitype = ITEMANIMATION_FARM6;
		if( attr & IA2_FARM7 )	i->Anitype = ITEMANIMATION_FARM7;


		if( attr & IA2_FIRE  )	i->Anitype = ITEMANIMATION_FIRE;

		if( attr & IA2_TRAPED1  )		i->curframe = (rand()%4)+1;
		if( !(attr & IA2_TRAPED1)   )	i->curframe = 0;
	}
	break;

	case IATTR_DURATION :
		{
			break;
		}

 }

	i->attr[ p->array] = p->attr;
}	
	
void SendItemDoorOpenClose( LPITEMGROUND i, int openclose )
{
	t_packet p;

	p.h.header.type = CMD_ITEM_DOOR_OPEN_CLOSE;
	{
		p.u.item_door_open_close.item_id = i->server_id;
		p.u.item_door_open_close.openclose = openclose;
	}
	p.h.header.size = sizeof( t_item_door_open_close );

	QueuePacket( &p, 1 );
}



	
void ToggleItemAnimation( LPITEMGROUND i )
{	
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver )
	{
		switch( i->Anitype )
		{	
		case ITEMANIMATION_OPENED :		i->Anitype = ITEMANIMATION_CLOSING; 
			break;
		case ITEMANIMATION_CLOSED :		i->Anitype = ITEMANIMATION_OPENNING; 
			break;
		}	
	}
	else 
#endif
	{
		switch( i->type )
		{
		case ITEMTYPE_DOOR : if( i->Anitype == ITEMANIMATION_CLOSED ) { SendItemDoorOpenClose( i, 0 );	break; }
							 if( i->Anitype == ITEMANIMATION_OPENED ) { SendItemDoorOpenClose( i, 1 );	break; }
				break;
							
//		case ITEMTYPE_BOX  : if( i->Anitype == ITEMANIMATION_CLOSED ) Send_CMD_ITEM_BOX_BREAK( i );	break;
//				break;
		}
	}

//	SendOpenDoor( i->server_id );
}		
		
		
		
	
			
		
//	È­¸é¿¡ Ãâ·ÂÇÏ¸é¼­ ¸¶¿ì½º¿¡ °É¸°°ÍÀ» Ã£´Â´Ù.
void DrawItemList( void )
{			
	LPITEMGROUND t = g_Item;
	static int light;
	static int toggle;
	Spr *s;	
	LPORDERTABLE o;
			
	if( toggle ){	++light;	if( light >= 12 )	toggle = !toggle;	}
	else		{	--light;	if( light <= 0 )	toggle = !toggle;	}
			
			
			
	PathCollisionLineCount = 0; //   ±æ¯±â¿¡ »ç¿ëµÉ ¾ÆÀÌÅÛÀÇ Ãæµ¹¶óÀÎÀ» PathCollisionLine[ MAX_ITEM_COLLISION ]¿¡ ¸ð¾Æ ³õ´Â´Ù.
			
	CursorCheckedItem = NULL;
	while( t != NULL )
	{		
		s = t->spr[t->curframe/2];
		if( s == NULL ) { goto NEXT_; }
			
		if( BoxAndBoxCrash( t->x - s->ox + t->offx, t->y - s->oy + t->offy, s->xl, s->yl, Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE ) )
		{	
			o = &g_OrderInfo.order[ g_OrderInfo.count ];
			o->rgb			= 0; // ¸¶¿ì½º¿¡ ÀÇÇØ CheckµÇ¾ú´Ù. 
			
			if( (t->attr[ IATTR_ATTR] & IA2_BOX) && (t->attr[ IATTR_ATTR] & IA2_OPENED ) ) { goto NEXT_OPENED_BOX_; }
			if( t->attr[ IATTR_ATTR] & IA2_TRAPED0 || t->attr[ IATTR_ATTR] & IA2_TRAPED1) 
			{	
				// ¸¸¾à ¼û°ÜÁ® ÀÖ´Â ÆøÅºÀÌ³ª µ£À» Ã£À» ¶§¿¡´Â itemÀÌ Å¬¸¯µÇ¾î¾ß ÇÑ´Ù. 
				if( SkillStatus && (SkillNo == 16 || SkillNo == 18 || SkillNo == 5 || SkillNo == 6 ))
				{
					//1019 zhh	//6	Remove Traps //5	Find Traps	//16	Detect Bomb	//18	Remove Bomb										
					if( (CanViewBomb(Hero) && SkillNo == 18 ) || (CanViewTrap(Hero) && SkillNo == 6 ) )//020320 lsw
					{						
					//	CursorCheckedItem = t;
					//	if(o->rgb		!= 1)	o->rgb		= 1;						
					//	if(o->alpha		!= light)	o->alpha	= light;
						goto zoung_;
					}
					//1019 zhh //¿©±â±îÁö
				}
				goto NEXT_OPENED_BOX_; 
			}
zoung_:

			if( g_MouseItemType == 0 ) 
//			if( t->host == Hero->id || t->host == 100000000 ) // ³»²¨°Å³ª ÁÖÀÎÀÌ ¾ø´Â°æ¿ì.
			{
				int x = t->x-s->ox+t->offx;
				int y = t->y-s->oy+t->offy;

				if( Hero->viewtype == VIEWTYPE_GHOST_ 
					|| !IsDead( Hero ) 
					|| Hero->viewtype == VIEWTYPE_OBSERVE_)
				if( ( (Hero->x-x)*(Hero->x-x) + (Hero->y-y)*(Hero->y-y) < 30000 
					&& !(t->attr[ IATTR_ATTR] & IA2_BOX )) || (t->attr[ IATTR_ATTR] & IA2_BOX ) ) // Box¶ó¸é ¾Æ¹«¸® ¸Ö¸® ÀÖ¾îµµ ¼±ÅÃµÉ¼ö ÀÖ´Ù. 
				if( ( Hero->viewtype == VIEWTYPE_GHOST_ && ( t->attr[ IATTR_ATTR] & IA2_DOOR ) ) 
					|| Hero->viewtype != VIEWTYPE_GHOST_ )// Ghost»óÅÂ¿¡¼­´Â ItemÁß ¹®¸¸ ¿­¼ö ÀÖ´Ù. 
				if( BoxAndDotCrash( x, y, s->xl, s->yl, Mox, Moy ) )
				if( CursorCheckedItem == NULL )
				if( (Hero->canviewitem && (t->attr[ IATTR_ATTR] & IA2_HIDE) ) 
					|| !(t->attr[ IATTR_ATTR] & IA2_HIDE) )
				{
						//if( Distance( Mox, Moy, Hero->x, Hero->y ) < 10000 )
					{	//if( (t->attr[ IATTR_ATTR] & IA2_BOX) == 0 ) 
						CursorCheckedItem = t;
					}
					o->rgb		= 1;
					o->alpha	= light;
					
				}	
			}

NEXT_OPENED_BOX_:

			if( t->attr[ IATTR_ATTR] & IA2_FIRE )	OutputLight( t->x, t->y, 10 );


				
			o->wType		= SPRITETYPE_ITEM; //t->Sorttype;
			o->x			= t->x;
			o->y			= t->y;
			o->height		= 0;
			o->offset		= t->y;
			o->lpvData		= (LPVOID)t;
			o->sort			= 1;
			o->show			= 1;
			
			g_OrderInfo.count++;


			if(  t->type == ITEMTYPE_DOOR )
				if( t->Anitype == ITEMANIMATION_CLOSED ) 
				{
					PathCollisionLine[ PathCollisionLineCount].sx = t->dsx[0];
					PathCollisionLine[ PathCollisionLineCount].sy = t->dsy[0];
					PathCollisionLine[ PathCollisionLineCount].dx = t->ddx[0];
					PathCollisionLine[ PathCollisionLineCount].dy = t->ddy[0];
					PathCollisionLineCount++;
				}
		}			
		else	
		{		
			//	¸¸¾à¿¡ AREA¹Û¿¡ ÀÖ´Ù¸é DeleteItemList( t->server_id )È£ÃâÇØ ¹ö¸°´Ù.
		}		
			
		switch( t->Anitype )
		{	
		case ITEMANIMATION_NO : 
			break;
		case ITEMANIMATION_REPEAT	:	t->curframe ++;
										if( t->curframe / 2 >= t->framecount ) t->curframe = 0;
			break;
		case ITEMANIMATION_RETURN	:	if( t->anitoggle )
										{
											t->curframe ++;
											if( t->curframe / 2 >= t->framecount ) { t->curframe--; t->anitoggle = !t->anitoggle; }
										}
										else 
										{
											t->curframe --;
											if( t->curframe / 2 < 0 ) { t->curframe++; t->anitoggle = !t->anitoggle; }
										}
			break;
		case ITEMANIMATION_OPENNING	:	t->curframe ++;
			 							if( t->curframe /2 >= t->framecount ) { t->curframe = (t->framecount*2) -1; t->Anitype = ITEMANIMATION_OPENED; }
			break;
		case ITEMANIMATION_OPENED	:	t->curframe = (t->framecount*2) -1;
			break;
		case ITEMANIMATION_CLOSING	:	t->curframe --;
			 							if( t->curframe < 0  ) { t->curframe = 0; t->Anitype = ITEMANIMATION_CLOSED; }
			break;
		case ITEMANIMATION_CLOSED	:	t->curframe = 0;
			break;


		case ITEMANIMATION_FARM0	:	t->curframe = 0; break;
		case ITEMANIMATION_FARM1	:	t->curframe = 2; break;
		case ITEMANIMATION_FARM2	:	t->curframe = 4; break;
		case ITEMANIMATION_FARM3	:	t->curframe = 6; break;
		case ITEMANIMATION_FARM4	:	t->curframe = 8; break;
		case ITEMANIMATION_FARM5	:	t->curframe = 10; break;
		case ITEMANIMATION_FARM6	:	t->curframe = 12; break;
		case ITEMANIMATION_FARM7	:	t->curframe = 14; break;

		case ITEMANIMATION_ACTIVE :		// 020701 YGI
		case ITEMANIMATION_FIRE		:	t->curframe ++;
										if( t->curframe / 2 >= t->framecount ) t->curframe = 2;
				break;		
		}	 
NEXT_:        
             
		t = t->next;
	}		 	
	if( CursorCheckedItem )
	{		 
//		PutCompressedImageFX( CursorCheckedItem->x - Mapx, CursorCheckedItem->y-Mapy, &spr[ 415], light, 2 );
	}		 
}			 
////////////////////////////////////////////////////////////////////////////
			 

//  ¾ÆÀÌÅÛÀ» Áý´Â´Ù.
//  Áý¾úÀ¸¸é 1¸¦ ¸®ÅÏ.
int CheckEventWith_Item( void )
{			  
	if( g_StartMenuOn == TRUE )  return 0;
	if( IsMouseOnMenu() ) return 0;

	int x, y;
	CHARACTER t;
	if( CursorCheckedItem != NULL )
	{		  
		if( SkillStatus == SKILL_OFF )
		if( !(CursorCheckedItem->attr[2] & IA2_NOTMOVE) &&
			CursorCheckedItem->getable )
		{
			if( (TileMap[ Hero->x / TILE_SIZE][ Hero->y / TILE_SIZE].attr_inside ==
				TileMap[ CursorCheckedItem->x / TILE_SIZE][ CursorCheckedItem->y / TILE_SIZE].attr_inside ) )	//  ½Ç³»¿¡ ÀÖ°Å³ª ½Ç¿Ü¿¡ ÀÖ¾î¾ß ¹°°ÇÀ» ÁÝ°Å³ª ¹ö¸±¼ö ÀÖ´Ù. 
			if( Distance( CursorCheckedItem->x, CursorCheckedItem->y, Hero->x, Hero->y ) < 10000 )
			{	  
				SendTakeItem( CursorCheckedItem->server_id );              
				return 1;
			}			
			else 
			{	
				x = Hero->x;	y = Hero->y;
				int mx, my;

				if( ReturnXYNearPosition( &x, &y, &mx, &my, 64, Hero, CursorCheckedItem->x, CursorCheckedItem->y ) )
				{
					memcpy( &t, Hero, sizeof( CHARACTER ) );
					t.moveFlag = TRUE;
					t.movetype = 0;
					t.gox = x;
					t.goy = y;
					t.destx = mx, t.desty = my;
					if( PathBuild( &t )) 
					{	
						t.moveFlag = FALSE;
						t.gox = x;
						t.goy = y;
						SendMoveData( &t );
					}		
				}		
			}			
		}					
		else				
		{	  
			switch( CursorCheckedItem->type ) 
			{ 
			case ITEMTYPE_COLOSSUS_STONE  :
			case ITEMTYPE_BOX  :	return 0;
//			  			DoHeroAttackBox( int x, int y )
				break;
              
			case ITEMTYPE_DOOR :	
			  			ToggleItemAnimation( CursorCheckedItem );
			  	break;
			} 
		}	  
		return 1;
	}		  
			  
	return 0; 
}			




void DrawObjectItem_SealStone( void )
{
	if( tool_ViewAllCharacter == false ) return;
	if( tool_SkillTool_ITEM_SEAL == false ) return;


	return;



	int i;
	int mxl, myl;
	int px, py;

	mxl = g_Map.file.wWidth  *TILE_SIZE;
	myl = g_Map.file.wHeight *TILE_SIZE;

	for( i = 0 ; i < TotalMapObject ; i ++)
	{
		if( Mo[ i].objectoritem == ITEMTYPE_SEAL_NOR ||
			Mo[ i].objectoritem == ITEMTYPE_SEAL_SPC )
		{
			px = Mo[i].x * RADARXL / mxl;
			py = Mo[i].y * RADARYL / myl;

			Hcolor( 255,255,100 );
			Hprint2( px, py, g_DestBackBuf, lan->OutputMessage(9,10), Mo[ i].dum );//010215 lsw
		}
	}


	return;

	/*

	LPITEMGROUND t = g_Item;
	int mxl, myl;
	int px, py;
	WORD color;
	int sxl, syl;
	static int tog;
	int smx, smy, smxl, smyl;
			
	mxl = g_Map.file.wWidth  * TILE_SIZE;
	myl = g_Map.file.wHeight * TILE_SIZE;
	
	while( t != NULL )
	{		
		if( (t->attr[ IATTR_ATTR] & IA2_SEAL_NOR ) || (t->attr[ IATTR_ATTR] & IA2_SEAL_SPC ) ) 
		{	
			px = t->x * TILE_SIZE * RADARXL / mxl;
			py = t->y * TILE_SIZE * RADARYL / myl;
			Hcolor( 255,255,100 );
			Hprint2( px, py, g_DestBackBuf, "°á%d", t->attr[ IATTR_SEAL_STONE_NO] );
		}
             
		t = t->next;
	}		 	
	*/
}



void CheckTrap( void )
{			
	int x, y;
	static int mx, my;
			
	x = Hero->x;
	y = Hero->y;
	if( mx == x / TILE_SIZE && my == y/ TILE_SIZE ) return ;
	mx = x/ TILE_SIZE, my = y / TILE_SIZE;
			
	LPITEMGROUND  t = g_Item;
			
	while( t != NULL )
	{		
		if( ( t->attr[IATTR_ATTR] & IA2_TRAPED0 ) || 
			( t->attr[IATTR_ATTR] & IA2_TRAPED1 ) )
		if( BoxAndDotCrash( t->x - 16, t->y - 16, TILE_SIZE, TILE_SIZE, x, y ) )
		{	
			if( t->attr[IATTR_ATTR] & IA2_TRAPED0 ) 
			{
				SendCMD_FOOT_ON_TRAP_0_ITEM( t->server_id );
			}
			else if( t->attr[IATTR_ATTR] & IA2_TRAPED1 )
			{
				SendCMD_FOOT_ON_TRAP_1_ITEM( t->server_id );
			}
			
			return;
		}	
			
		t = t->next;
	}		
			
	return;	
}			
			
//extern int aItemInventory[3][3][8];
extern ItemAttr  InvItemAttr[3][3][8];
			
void RecvGiveItem( t_server_give_item *p )
{			
	InvItemAttr[p->p1][p->p2][p->p3]   = p->item;

	//Hero->stolen = true;
}			

	 
void Send_CMD_JUST_ATTACK_ANIMATION( LPITEMGROUND i )
{	
	t_packet p;
	
	p.h.header.type = CMD_JUST_ATTACK_ANIMATION;
		p.u.client_just_attack_animation.item_id = i->server_id;
	p.h.header.size = sizeof( t_client_just_attack_animation );
	
	QueuePacket( &p, 1 );
}


void Send_CMD_ITEM_BOX_BREAK( LPITEMGROUND i )
{	
	t_packet p;
	
	p.h.header.type = CMD_ITEM_BOX_BREAK;
	{
		p.u.client_item_box_break.item_id = i->server_id;
	}
	p.h.header.size = sizeof( t_client_item_box_break );
	
	QueuePacket( &p, 1 );
}	
	
void Recv_CMD_ITEM_BOX_BREAK( t_server_item_box_break *p )
{	
	int ani;
	LPITEMGROUND i = FindItemList( p->item_id );
	if( i == NULL ) return;
	LPCHARACTER ch = ReturnCharacterPoint( p->server_id );
	if( ch == NULL ) return;

	//if( ch != Hero ) 
	{
		ch->nAttackedType	= SPRITETYPE_ITEM;
		ch->lpAttacked		= (VOID *)i;

		ani = AttackAction( ch );

		if( ani == ch->nCurrentAction )	return;
		else  ch->nCurrentAction = ani;
		ChangeDirection( &ch->direction, ch->x, ch->y, i->x, i->y );
		ch->todir = ch->direction;
		ch->nCurrentFrame.PutEncrypted(0);
	}

	ch->boxitembreaktype = p->type;
}

void Send_CMD_ITEM_BOX_BREAK_RESULT( int item_id )
{
	t_packet p;

	p.h.header.type = CMD_ITEM_BOX_BREAK_RESULT;
		p.u.client_item_box_break_result.item_id = item_id;
	p.h.header.size = sizeof( t_client_item_box_break_result );

	QueuePacket( &p, 1 );
}

void Send_CMD_ITEM_BOX_MAGIC_BREAK( int item_id )
{
	t_packet p;

	p.h.header.type = CMD_ITEM_BOX_MAGIC_BREAK;
		p.u.client_item_box_magic_break.item_id = item_id;
	p.h.header.size = sizeof( t_client_item_box_magic_break );

	QueuePacket( &p, 1 );
}
void Recv_CMD_ITEM_BOX_MAGIC_BREAK_RESULT( t_server_item_box_magic_break_result *p )
{
	LPITEMGROUND i = FindItemList( p->item_id );
	if( i == NULL ) return;

	if( p->type == 0 ) //  ÆøÆÄ..
	{
		InsertMagic( Hero, Hero, 208,0, i->x, i->y, 0,0 );

	}
	else 
	{

	}
}

void InitItemAttrs()
{
	memset(InvItemAttr		,0,sizeof(ItemAttr)*3*3*8);
	memset(EquipItemAttr	,0,sizeof(ItemAttr)*8);
	memset(QuickItemAttr	,0,sizeof(ItemAttr)*7);
	memset(BankItemAttr		,0,sizeof(ItemAttr)*5*3*6);
	memset(&HandItemAttr	,0,sizeof(ItemAttr));
}
