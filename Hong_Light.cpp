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
#include <Math.h>
#include <stdio.h>


#include "Dragon.h"
#include "GameProc.h"
#include "Char.h"
#include "Hong_Sprite.h"
#include "Map.h"
#include "Hangul.h"
#include "Hong_Sub.h"
#include "Tool.h"
#include "MouseCursor.h"
#include "Effect.h"
#include "Hong_Light.h"


extern CHARACTERLIST		g_CharacterList;
int LightT[LIGHT_ROW][LIGHT_COL] = {0,};
int LightTableX = 0;
int LightTableY = 0;

int DayLightControl = 31;
char *BrightSprBuf[ 21][2] = {0,};

int   LightCount = 0;
LIGHT Light[ LIGHT_MAX_]= {0,};
LIGHT g_OutLight[ MAX_OUTLIGHT_]= {0,};	

char BrightBuf[ (GAME_SCREEN_XSIZE/2) * (GAME_SCREEN_YSIZE/2) ]= {0};
WORD rgbTable[32 * 65536] = {0,};

void MakergbTable( void )
{
	WORD c;
	BYTE j;

	WORD i;
	int  t;

	if( _PixelInfo.BitMaskR == 0x7C00 )	// 555
	{
		for( t = 0, i = 0 ; t < 65536 ; t++, i ++)
		{
			for( j = 0 ; j < 32 ; j ++)
			{
			_asm {
					xor		eax,eax;
					mov		ax,i;
					ror		eax,10;
					shl		ax,11;
					rol		eax,5;
					shl		ax,6;
					rol		eax,5;

					xor     ebx, ebx;
					mov     bl, j;
					imul	eax,ebx;
					shr		eax,5;
					and		eax,0x03e0fc1f;
					
					ror		eax,5;
					shr		ax,6;
					ror		eax,5;
					shr		ax,11;
					rol		eax,10;
					
					mov		c,ax;
			}
				rgbTable[j * 65536 + i] = c;
			}
		}
	}
	else								// 565
	{
		for( t = 0, i = 0 ; t < 65536 ; t++, i ++)
		{
			for( j = 0 ; j < 32 ; j ++)
			{
			_asm {
					xor		eax,eax;
					mov		ax,i;
					ror		eax,11;
					shl		ax,10;
					rol		eax,6;
					shl		ax,5;
					rol		eax,5;

					xor     ebx, ebx;
					mov     bl, j;
					imul	eax,ebx;
					shr		eax,5;
					and		eax,0x03e0fc1f;
					
					ror		eax,5;
					shr		ax,5;
					ror		eax,6;
					shr		ax,10;
					rol		eax,11;
					
					mov		c,ax;
			}
				rgbTable[j * 65536 + i] = c;
			}
		}
	}
}

void PutMapBright( LPVOID dest, LPVOID alpha)
{	
	DWORD	k = 1280,j;
	LPVOID	Dtmp = (LPVOID)(dest);
	
	j = SCREEN_WIDTH;
	
	_asm{
		push	es;

		push	ds;
		pop		es;

		xor		ebx,ebx;

		xor		eax,eax;
		xor		ecx,ecx;

		mov		cx, GAME_SCREEN_YSIZE

		mov		esi, alpha

	loop_1:
		mov		edi,Dtmp;
	
		push    cx
		xor		eax,eax;
		mov		cx,SCREEN_WIDTH;	//load data num
	
loop_alpha:
		xor		eax,eax;
		mov		eax,[edi];
		shr		eax,5;
			
		mov     bl, byte ptr[ esi];
		inc		esi;
		imul	eax,ebx;
		
		mov		[edi],eax;
		
		add		edi,4;
		loop	loop_alpha;
		
		pop		cx
		dec		cx
		jz      exit_1;
		
		mov		eax,Dtmp;
		add		eax,k;
		mov		Dtmp,eax;
		
		jmp		loop_1;
		
	exit_1:
		pop		es;
	};	
}	
	
void PutMapBright2( LPVOID dest, LPVOID alpha)
{		
	DWORD	k = (GAME_SCREEN_XSIZE*2),j;
	LPVOID	Dtmp = (LPVOID)(dest);
		
	j = SCREEN_WIDTH;
		
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		ebx,ebx;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		mov		cx, (GAME_SCREEN_YSIZE/2)
		
		mov		esi, alpha
						
	loop_1:				
		mov		edi,Dtmp;
						
		push    cx		
		mov		cx,(GAME_SCREEN_XSIZE/2);	//load data num
						
loop_alpha:					
						
		movzx   dx, byte ptr[ esi];
		cmp     dl, 31	
		jz      skip_1;	
		shl     edx, 16	
		movzx	eax, word ptr [edi+2];
		add		edx, eax;
		shl		edx, 1;	
		mov     ax, word ptr [offset  rgbTable + edx  ];
		shl		eax, 16	
							
		
		movzx     dx, byte ptr[ esi];
		shl     edx, 16	
		mov		bx,[edi];
		add		edx, ebx;
		shl		edx, 1;	
		mov     ax, word ptr [offset  rgbTable + edx  ];
						
		mov     [edi],eax;

skip_1:
		add		edi,4;
		inc     esi
			
		dec		cx;
		jnz 	loop_alpha;

	//--------------

		sub     esi, (GAME_SCREEN_XSIZE/2)  // 1 라인의 데이타를 가지고 두번찍을것이기때문.

		mov		edx,Dtmp;
		add		edx,k;
		mov		Dtmp,edx;
		mov		edi,Dtmp;

		mov		cx,(GAME_SCREEN_XSIZE/2);	//load data num

loop_alpha2:			
		
		movzx   dx, byte ptr[ esi];
		cmp     dl, 31
		jz      skip_2;
		xor     eax,eax;
		shl     edx, 16
		mov		ax,[edi+2];
		add		edx, eax;
		shl		edx, 1;
		mov     ax, word ptr [offset  rgbTable + edx  ];
		shl		eax, 16

		movzx   dx, byte ptr[ esi];
		shl     edx, 16
		mov		bx,[edi];
		add		edx, ebx;
		shl		edx, 1;
		mov     ax, word ptr [offset  rgbTable + edx  ];

		mov     [edi],eax;


skip_2:
		add		edi,4;
		inc     esi

		dec		cx
		jnz 	loop_alpha2;
		
	//------------------
		pop		cx
		dec		cx
		jz      exit_1;
		
		mov		edx,Dtmp;
		add		edx,k;
		mov		Dtmp,edx;

		jmp		loop_1;
		
	exit_1:
		pop		es;
	};	
}		

void LoadBrightDataSub( char *filename, char **buf0, char **buf1 )
{			
	FILE *fp;
	DWORD xl, yl;
	BYTE *p0, *p1;
			
	fp = Fopen( filename, "rb" );
	fseek(fp, 1, SEEK_SET );
	fread( &xl, sizeof( DWORD ), 1, fp );
	fread( &yl, sizeof( DWORD ), 1, fp );
			
	MemAlloc(*buf0, (xl * yl) + 8 );
	MemAlloc(*buf1, (xl * yl) + 8 );
			
	*(DWORD *)(*buf0) = xl;
	*(DWORD *)(*buf0 + 4 ) = yl;
			
	fseek( fp, 8, SEEK_CUR );
	fread( *buf0 + 8, xl * yl , 1, fp );
	memcpy( *buf1, *buf0, xl * yl + 8 );
			
	p0 = (BYTE *)*buf0;
	p1 = (BYTE *)*buf1;
	for( DWORD i = 8 ; i < xl * yl; i ++)
	{		
		*( p0 + i ) = *( p0 + i ) / 8;
		*( p1 + i ) = *( p1 + i ) / 9;
	}		
			
	fclose(fp);
}
			
void LoadBrightData( void )
{	
	char temp[ FILENAME_MAX];

	for( int i = 0 ; i < 21 ; i ++)
	{
		sprintf( temp, "./data/lig%03d.spr", i );
		LoadBrightDataSub( temp, &BrightSprBuf[ i][0], &BrightSprBuf[ i][1] );
	}
}
			
void PutBright( int x, int y, char *light )
{		
	int xl, yl;
	int lx, rx, ty, by;
	char *sou;
	int i,j;
	char *p;
	
  if (!light) return; // LTS DRAGON BUG
 	xl = *(DWORD *)light;
	yl = *(DWORD *)(light + 4 );
	
	x >>= 1;
	y >>= 1;
	
	light += 8;
		
	x -= (xl/2);
	y -= (yl/2);
		
	if( x >= (GAME_SCREEN_XSIZE/2)  )			return; 
	else if( x + xl < 0 )	return; 
	if( y >= (GAME_SCREEN_YSIZE/2) )			return; 
	else if( y + yl < 0  )	return;
		
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
		
	if( x + xl >= (GAME_SCREEN_XSIZE/2) )
	{	
		rx -= x + xl - (GAME_SCREEN_XSIZE/2);
	}	
	if( x < 0 )
	{	
		lx  = 0 - x;
		x   = 0;
	}	
	if( y + yl >= (GAME_SCREEN_YSIZE/2) )
	{	
		by -= y + yl - (GAME_SCREEN_YSIZE/2);
	}	
	if( y < 0 )
	{	
		ty  = 0 - y;
		y   = 0;
	}	
		
	sou = light + lx + ty * xl;
	for( i = ty ; i < by ; i ++)
	{	
		p = BrightBuf + x + y * (GAME_SCREEN_XSIZE/2);
		for( j = lx ; j < rx ; j ++, p++, sou++ )
		{
			if( *sou )
			{
				*(p) += *sou;
				if( *(p) > 31 ) *(p) = 31;
			}
		}
		sou += ( xl - ( rx - lx ) );
		y ++;
	}	
}		
	
	

void OutputLight( int x, int y, int lightness, int color)
{		
	if( LightCount + 1 >= LIGHT_MAX_ ) return;
		
	int mx, my;
		
	mx = x/TILE_SIZE;
	my = y/TILE_SIZE;
		
	if( TileMap[mx ][my].roof && TileMap[mx][my].show_roof == 0 )	return;
		
		
	for(int i=0;i<LightCount;i++)
	{	
		if( BoxAndDotCrash( Light[i].x - 48, Light[i].y - 48, 96, 96, x, y ) )
		{
			return;
		}
	}	
		
	//distance_Light = sqrt(pow(Light[i].x - x,2)+pow(Light[i].y - y,2));
	//}
	Light[LightCount].x = x+(rand()%5-2);
	Light[LightCount].y = y+(rand()%5-2);
	Light[LightCount].light = lightness;		//0~16
	Light[LightCount].flag = color;
	LightCount ++;
}	


///////////////////////// 0807 lkh 추가(PC/NPC/MONSTER 광원을 출력하는 함수) ////////////////////////////	
void	PutCharacterLight(LPCHARACTER	lpCharacter, int diff_Time)
{
	char *tt;
	
	if( g_Inside != TileMap[ lpCharacter->position.x][lpCharacter->position.y].attr_inside )	return;

	tt = NULL;
	switch( lpCharacter->animationType )
	{									
	case ANIMATION_TYPE_MAN   :
	case ANIMATION_TYPE_WOMAN : 	
		{
			if( Hero->viewtype == VIEWTYPE_GHOST_ )
			{
				if( lpCharacter->viewtype == VIEWTYPE_GHOST_ )	//유령인 경우
				{
					if( !IsHidenGhost(lpCharacter) )//히든 고스트가 아니라면 광원을 찍는다.
					{
						tt = BrightSprBuf[18-diff_Time][0];
					}
				}
				else
				{
					tt = BrightSprBuf[18-diff_Time][0];			// 내가 Ghost가 되면 모든 Light는 볼수 있다. 
				}
			}
			else
			{

				if( lpCharacter->viewtype != VIEWTYPE_TRANSPARENCY_ || lpCharacter == Hero)
				{
					if( lpCharacter->viewtype == VIEWTYPE_GHOST_ )	//유령인 경우	
					{
						if( !IsHidenGhost(lpCharacter) && CanViewGhost(Hero) )//020320 lsw
						{
							tt = BrightSprBuf[18-diff_Time][0];		// 상대가 Ghost지만 볼수 있는 기술을 사용하면 볼수 있다. 
						}
					}
					else											//살아 있는 경우
					{	
						if( lpCharacter == Hero )		tt = BrightSprBuf[18-diff_Time][0];	// 나도 Ghsot가 아니고 상대도 Ghost가 아니면 볼수 있다. 
						else
						{
							tt = BrightSprBuf[18-diff_Time][0];	
							/******************************************** 나의 파티원들만 서로 볼수 있다. 
							for(int i=0; i<6; i++)
							{
								if( SCharacterData.party[i].m_Sight == 1 )
								{
									if( lpCharacter == SCharacterData.party[i].ch )
									{
										tt = BrightSprBuf[12-diff_Time][0];	// 나도 Ghsot가 아니고 상대도 Ghost가 아니면 볼수 있다. 
										break;
									}
								}
							}
							***********************************************************/
						}
					}
				}
									}

			break;
		}
	case ANIMATION_TYPE_MON1:
		{
			if (lpCharacter->nRace == HUMAN)
			{	//< CSD-030419
				tt = BrightSprBuf[15][0];
			}	//> CSD-030419

			break;
		}
	}
	
	if( tt )
	{
		if( Hero->lostSight )
		{
			if( lpCharacter == Hero )
			{
				PutBright( ( lpCharacter->x - Mapx ), ( lpCharacter->y - Mapy - 32 ),  tt  );
			}
		}
		else
		{	
			if (lpCharacter->nRace == HUMAN)	
			{	//< CSD-030419
				PutBright(lpCharacter->x - Mapx, lpCharacter->y - Mapy - 32,  tt);
			}	//> CSD-030419
		}
	}
}	
	
	
///////////////////// 0807 lkh 수정 //////////////////////
int MapBright( void )
{				
	int i;
	LPCHARACTER lpCharacter = Hero;
		
	static int toggle = 0;
		
	static int oldmx;
	static int oldmy;
		
	static int mx = g_Map.x;
	static int my = g_Map.y;
			
	int		temp_DayLight=DayLightControl;
	int		diff_Time=0;

	//////////////////// 0313 lkh 추가 (죽은 경우) //////////////////////////
	if( (Hero->condition == CON_DEATH || Hero->hp == 0) && Hero->lostSight != 0 )
	{
		Hero->sight = 10;
		Hero->lostSight = 0;
	//	Hero->lightness		= 0;//010530 lsw
		Hero->lostSightTime = 0;
	}

	//////////////////// 0322 lkh 추가 /////////////////////
/*	if(Hero->lightness && DayLightControl < 29 && !Hero->lostSight)		//낮이 아니고 시야상실 상태가 아니며, Hero가 빛 마법을 사용한 경우
	{
		if(Hero->lightnessTime <= g_curr_time)
		{
			Hero->light			= 0;
			Hero->lightness		= 0;
			Hero->lightnessTime	= 0;
		}
		temp_DayLight = rand()%3+28;
	}
*/
	///////////////////////////////////////////////////////

	//////////////////// 0201 이 규훈 //////////////////////
	if(Hero->lostSightTime)			//시야상실 시간이 셋팅되어 있으면
	{
		diff_Time=abs((long)(Hero->lostSightTime - g_curr_time));		//현재 시간과의 차이시간 계산
		if(diff_Time>15)			diff_Time=15;				//차이가 15초 이상이면 어둡기를 15 하한선으로 셋	
	}
		
	if( Hero->lostSight )									//시야 상실 상태
	{		
		if( Hero->lostSightTime <= g_curr_time)					//시야 상실 유효시간 경과시
		{			
			Hero->lostSight		= 0;
			Hero->lostSightTime	= 0;
			diff_Time			= 0;
			Hero->sight			= 10;
		}
		if(diff_Time<15)										//종료시간과 15초 이내인 경우
		{
			Hero->lostSight = (diff_Time*2) ;
			if(Hero->lostSight < 0)	Hero->lostSight=0;
		}
		temp_DayLight = DayLightControl - Hero->lostSight;		//DayLightControl=>0~31
		if(temp_DayLight < 0)	temp_DayLight=0;
	}
	else if( DayLightControl >= 31)
		return 0;
	///////////////////// 여기까지 //////////////////////
			
	toggle = !toggle;

	mx = Mapx;
	my = Mapy;

	if(  toggle || ( mx != oldmx || my != oldmy ) )
	{	
		oldmx = mx, oldmy = my;
		
		memset( BrightBuf, temp_DayLight, (GAME_SCREEN_XSIZE/2) * (GAME_SCREEN_YSIZE/2) );
		

		while( lpCharacter != NULL )
		{
			PutCharacterLight(lpCharacter, diff_Time);
			lpCharacter = lpCharacter->lpNext;
		}

//////////////////////////////// 이규훈 1029 //////////////////////////////////
		for(i=0; i<MAX_OUTLIGHT_;i++)
		{
			if(g_OutLight[i].light>0)		//소멸 광원 출력을 위해 Light구조체에 값을 셋팅
			{
				if( LightCount + 1 >= LIGHT_MAX_ ) break;
				
				Light[LightCount].x		=		g_OutLight[i].x;
				Light[LightCount].y		=		g_OutLight[i].y;
				Light[LightCount].light	=		g_OutLight[i].light;
				Light[LightCount].flag	=		g_OutLight[i].flag;
				Light[LightCount].frame	=		g_OutLight[i].frame;
				Light[LightCount].terminateFrame=g_OutLight[i].terminateFrame;
				
				if( Light[LightCount].frame >= Light[LightCount].terminateFrame)
					g_OutLight[i].light-=2;
				else
					g_OutLight[i].frame++;

//				if(g_OutLight[i].light<=0)	
//				{
//					memset(&g_OutLight[i],0,sizeof(LIGHT));
//				}
				LightCount++;
			}
			else
			{
				memset(&Light[LightCount],0,sizeof(LIGHT));
			}
		}
		

		// 001216 KHS 색광원을 찍는다.  InsertLightOut()함수 참조..  effect_table.et-> light......

		for(i=0; i<LightCount;i++)			//일반 이동 광원 물리기
		{
			LIGHT *l = &Light[i];

			switch(l->flag)
			{
			case 0:		//default -White
				//PutBright( l->x - Mapx, l->y - Mapy, BrightSprBuf[l->light] );
				//break;
			case 1:		//높이값을 더하여 이미지와 동일 높이에 광원 찍음
				if(!BrightSprBuf[l->light][0])continue;//soto-031031
				PutBright( l->x - Mapx, l->y - Mapy, BrightSprBuf[l->light][0] );
				break;
			case 2:		//Red Light 	Effspr[sprite_Num].EffSpr[i].img
				////////////////// 0206 lkh 수정 ////////////////////
//				PutCompressedImageFX( l->x - Mapx, l->y - Mapy, &Effspr[78].EffSpr[l->light], 2, 2 );   // 나중에 컴사양이 올라가면 remark를푼다. 
				PutBright( l->x - Mapx, l->y - Mapy, BrightSprBuf[l->light][0] );
				break;
			case 3:		//Blue Light
				////////////////// 0206 lkh 수정 ////////////////////
//				PutCompressedImageFX( l->x - Mapx, l->y - Mapy, &Effspr[79].EffSpr[l->light], 2, 2);	// 나중에 컴사양이 올라가면 remark를푼다. 
				PutBright( l->x - Mapx, l->y - Mapy, BrightSprBuf[l->light][0] );
				break;
			}
		}
///////////////////////////////////////////////////////////////////////////////
	}
	
	LightCount = 0;
	
	
	PutMapBright2( g_DestBackBuf, BrightBuf );
	
	
	return 0;
}