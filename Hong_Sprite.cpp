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
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <Io.h>
#include <Math.h>


#include "Dragon.h"
#include "Char.h"
#include "GameProc.h"
#include "Map.h"
#include "Hangul.h"
#include "Tool.h"

#include "DirectSound.h"

#define __HONG_SPRITE_H__
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"

#include "Convert565to555.h"
#include "MouseCursor.h"
#include "Effect.h"


extern CHARACTERLIST		g_CharacterList;

WORD	AddTableR[128];
WORD	AddTableG[256];
WORD	AddTableB[128];

WORD	SubTableR[64];
//WORD	SubTableg[64];
WORD	SubTableG[128];
WORD	SubTableB[64];

bool	StartLoadCharSpriteDataThreadFlag;
bool	LoadingCharSpriteData;
bool g_bStopWeatherSystem = false; // CSD-CN-031215
bool g_bOffWeatherSystem = false; // CSD-CN-031217

extern bool g_IsThunderStorm; //Eleval 12/08/09 - Thunderstorm
extern bool bLowRain; //Eleval 12/08/09 - Thunderstorm
extern int last_thunder_time; //Eleval 12/08/09 - Thunderstorm
extern int thunder_wait_time; //Eleval 12/08/09 - Thunderstorm

extern BOOL		Statue_Weather;

/////////////////////
char *GetSurfacePointer(LPDIRECTDRAWSURFACE  pSurface)
{	
    char *buf;
    DDSURFACEDESC ddSurfaceDesc;
    
	memset(&ddSurfaceDesc, 0, sizeof(DDSURFACEDESC));
    ddSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
    HRESULT result;
    result = pSurface->Lock(NULL, &ddSurfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	
	buf = (char *)ddSurfaceDesc.lpSurface;
	
	if ( buf == NULL )
	{
		result = RestoreAllSurfaces(  &g_DirectDrawInfo );
		memset(&ddSurfaceDesc, 0, sizeof(DDSURFACEDESC));
		ddSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
		HRESULT result;
		result = pSurface->Lock(NULL, &ddSurfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	}
	
    pSurface->Unlock( NULL ); //ddSurfaceDesc.lpSurface);
	
	return buf;
}	



//		
// Return:	8	256 Color
//			16	High Color
//			24	24Bits Color
//			32	32Bits Color
int	GetBitsPerPixel( void )
{	
	HDC		hDC;
	int		nBitPerPixel;
	
	hDC = GetDC( NULL );
	nBitPerPixel = GetDeviceCaps( hDC, PLANES ) * GetDeviceCaps( hDC, BITSPIXEL );
	ReleaseDC( NULL, hDC );
	
	return	nBitPerPixel;
}		





void	CheckColorBitMask(  LPDIRECTDRAWSURFACE  surface )
{		
	DDSURFACEDESC ddSurfaceDesc;
	
	ZeroMemory( &ddSurfaceDesc, sizeof( DDSURFACEDESC ) );
	ddSurfaceDesc.dwSize  = sizeof( ddSurfaceDesc );
	//ddSurfaceDesc.dwFlags = DDSD_PIXELFORMAT | DDSD_PITCH;
	HRESULT hResult = surface->GetSurfaceDesc( &ddSurfaceDesc );
	
	if( g_DirectDrawInfo.bFullscreen )
	{	
		//FILE *fp = Fopen( "c:/dxsize.txt", "wt" );
		wSurDxSize = (WORD)ddSurfaceDesc.lPitch;
		dSurDxSize = (DWORD)ddSurfaceDesc.lPitch;
		
		//fprintf( fp, "%d %d \n", wDxSize, dSurDxSize );
		//fclose(fp);
	}	
	else 
	{	
		wDxSize = SCREEN_WIDTH * 2;
		dDxSize = SCREEN_WIDTH * 2;
	}	
	
	wDxSize = SCREEN_WIDTH * 2;
	dDxSize = SCREEN_WIDTH * 2;
	
	if (hResult != 0)//DD_OK)
		return;
	
	_PixelInfo.BitMaskR = ddSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	_PixelInfo.BitMaskG = ddSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	_PixelInfo.BitMaskB = ddSurfaceDesc.ddpfPixelFormat.dwBBitMask; 
	
	if (_PixelInfo.BitMaskR == 0xF800) 
	{	
		// 요거이 RGB 5:6:5 포맷이니끼리 그렇게 알라우.
		//			 4444----3333----2222----1111----
		//           84218421842184218421842184218421
		//           00000000000000011111011111011110;
		_PixelInfo.ShiftRightR = 11;
		_PixelInfo.ShiftLeftR  = 0;
		
		_PixelInfo.ShiftRightG = 5;
		_PixelInfo.ShiftLeftG  = 5;
		
		_PixelInfo.ShiftRightB = 0;
		_PixelInfo.ShiftLeftB  = 11;
	}else
		if (_PixelInfo.BitMaskR == 0x7C00) 
		{	
			// 요고 RGB 5:5:5 포맷 아이가.
			//			   4444----3333----2222----1111----
			//             84218421842184218421842184218421
			//             00000000000000001111101111011110;
			_PixelInfo.ShiftRightR = 10;
			_PixelInfo.ShiftLeftR  = 1;
			
			_PixelInfo.ShiftRightG = 5;
			_PixelInfo.ShiftLeftG  = 6;
			
			_PixelInfo.ShiftRightB = 0;
			_PixelInfo.ShiftLeftB  = 11;
		}else
			if (_PixelInfo.BitMaskR == 0x001F)
			{	
				// 아따 요것 BGR 5:6:5 포맷이것제.
				//			   4444----3333----2222----1111----
				//             84218421842184218421842184218421
				_PixelInfo.ShiftRightR = 0;
				_PixelInfo.ShiftLeftR  = 11;
				
				_PixelInfo.ShiftRightG = 5;
				_PixelInfo.ShiftLeftG  = 5;
				
				_PixelInfo.ShiftRightB = 11;
				_PixelInfo.ShiftLeftB  = 0;
			}	
}		

void SDL_CheckColorBitMask()
{
	wDxSize = SCREEN_WIDTH * 2;
	dDxSize = SCREEN_WIDTH * 2;

	_PixelInfo.BitMaskR = 0xF800;

	if (_PixelInfo.BitMaskR == 0xF800)
	{
		// 요거이 RGB 5:6:5 포맷이니끼리 그렇게 알라우.
		//			 4444----3333----2222----1111----
		//           84218421842184218421842184218421
		//           00000000000000011111011111011110;
		_PixelInfo.ShiftRightR = 11;
		_PixelInfo.ShiftLeftR = 0;

		_PixelInfo.ShiftRightG = 5;
		_PixelInfo.ShiftLeftG = 5;

		_PixelInfo.ShiftRightB = 0;
		_PixelInfo.ShiftLeftB = 11;
	}
	else {
		if (_PixelInfo.BitMaskR == 0x7C00)
		{
			// 요고 RGB 5:5:5 포맷 아이가.
			//			   4444----3333----2222----1111----
			//             84218421842184218421842184218421
			//             00000000000000001111101111011110;
			_PixelInfo.ShiftRightR = 10;
			_PixelInfo.ShiftLeftR = 1;

			_PixelInfo.ShiftRightG = 5;
			_PixelInfo.ShiftLeftG = 6;

			_PixelInfo.ShiftRightB = 0;
			_PixelInfo.ShiftLeftB = 11;
		}
		else {
			if (_PixelInfo.BitMaskR == 0x001F)
			{
				// 아따 요것 BGR 5:6:5 포맷이것제.
				//			   4444----3333----2222----1111----
				//             84218421842184218421842184218421
				_PixelInfo.ShiftRightR = 0;
				_PixelInfo.ShiftLeftR = 11;

				_PixelInfo.ShiftRightG = 5;
				_PixelInfo.ShiftLeftG = 5;

				_PixelInfo.ShiftRightB = 11;
				_PixelInfo.ShiftLeftB = 0;
			}
		}
	}
}


void	InitSpriteTransTable( LPDIRECTDRAWSURFACE  surface )
{		

#ifndef _SDL
	CheckColorBitMask(surface);
	#else
	SDL_CheckColorBitMask();
#endif

	WORD	MaskG;
	if( _PixelInfo.BitMaskR == 0x7c00)
	{	
		MaskG = 0x1f;
	}	
	else
	{	
		MaskG = 0x3f;
	}	
	
	//Make ADD Table
	int i;
	for(i=0; i < 128; i++){
		if(i<32)
		{
			AddTableR[i] = i << _PixelInfo.ShiftRightR;
			AddTableB[i] = i << _PixelInfo.ShiftRightB;
		}	
		else
		{
			AddTableR[i] = 0x1f << _PixelInfo.ShiftRightR;
			AddTableB[i] = 0x1f << _PixelInfo.ShiftRightB;
		}
	}	
	for(i=0; i < 256; i++){
		if(i<=MaskG)
		{
			AddTableG[i] = i << _PixelInfo.ShiftRightG;
		}
		else
		{
			AddTableG[i] = MaskG << _PixelInfo.ShiftRightG;
		}
	}	
	
	//Make SUB Table
	for( i=0 ; i < 64 ; i++ ){
		if(i<32)
		{
			SubTableR[i] = 0;
			SubTableB[i] = 0;
		}
		else
		{
			SubTableR[i] = (i - 32) << _PixelInfo.ShiftRightR;
			SubTableB[i] = (i - 32) << _PixelInfo.ShiftRightB;
		}
		//SubTableG[i] = i << _PixelInfo.ShiftRightG;
		//SubTableg[i] = 0;
	}
	for(i=0; i < 128; i++){
		if(i<64)
		{
			SubTableG[i] = 0;
		}
		else
		{
			SubTableG[i] = (i - 64) << _PixelInfo.ShiftRightG;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//           Put Sprite Transperensy..
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void PutCmprsImgClipingTrans565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, WORD MSK)
{
	int		l, kkk = lx << 1;
	LPVOID	Dtmp = (LPVOID)( (char *)dest + y * wDxSize + (x * 2 ));
	rx <<= 1;
	lx <<= 1;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		mov		esi,Stmp;
		
		mov		cx,word ptr ty;
		
		or		cx,cx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		
		mov		ax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		inc		esi;
		inc		esi;
		loop	loop_1;
exit_1:
	}
	_asm{
		mov		bx,word ptr by;		//
loop_for1:
		or		bx,bx;		//for(i=0;i<by;i++)
		jz		exit_for1;	//
		xor		eax,eax;
		xor		edx,edx;
		xor		ecx,ecx;
		mov		edi,Dtmp;
		
		mov		ax,word ptr lx;	//if(lx)
		or		ax,ax;			//
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		add		esi,2;
		cmp		dx,word ptr lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		dx,word ptr lx;		//j-=lx
		
		cmp		dx,word ptr rx;
		jg		jmp_00001;
		
		add		edi,edx;			//dtmp+=j
		
		lodsw;						//k=*stmp
		add		dx,ax;				//j+=k
		mov		cx,ax;				//
		
		cmp		dx,word ptr rx;	//new
		jle		jmp_0000;
		add		cx,word ptr rx;
		sub		cx,dx;
		
		jmp		xxxxxxx1;
jmp_00001:
		lodsw;
		add		esi,eax;
		jmp		ext_loop;
		
jmp_if1:///////////////////////////////////////
		lodsw;				//
		add		dx,ax;		//
		add		esi,eax;	//
		cmp		dx,word ptr lx;		//
		jle		loop_while1;
		sub		dx,word ptr lx;
		sub		esi,edx;
		mov		cx,dx;
		
		cmp		dx,word ptr rx;	//new
		jle		jmp_0000;		//"
		mov		cx,word ptr rx;	//"
xxxxxxx1:
jmp_0000:					//"
		
		//************ move	data ***********
		shr		cx,1;//*********************************************************
		or		cx,cx;
		jz		exit_movedata1;
loop_TR1:
		lodsw
			and		ax,1111101111011110b;
		and		word ptr [edi],1111101111011110b;
		add		ax,[edi];
		shr		ax,1;
		mov		[edi],ax;
		add		edi,2;
		loop	loop_TR1;
exit_movedata1:
		
		cmp		dx,word ptr rx;	//new		
		jle		exit_while1;	//"
		mov		eax,edx;		//"
		sub		ax,word ptr rx;	//"
		add		esi,eax;		//new
		
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		lodsw;
		
		add		edi,eax;	//dtmp+=*stmp
		add		dx,ax;		//j+=*stmp
		
		lodsw;
		mov		cx,ax;		//l=k=*stmp
		mov		word ptr l,ax;
		
		add		ax,dx;
		cmp		ax,word ptr rx;
		jle		exit_if0;
		cmp		dx,word ptr rx;
		jg		exit_if0;
		//mov		ax,cx;
		mov		cx,word ptr rx;
		sub		cx,dx;
exit_if0:
		cmp		dx,word ptr rx;
		jg		else_if;
		
		add		dx,word ptr l;	//j+=l;
		sub		word ptr l,cx;	//l-k;
		
		//************ move	data ***********
		shr		cx,1;//*********************************************************
		or		cx,cx;
		jz		exit_movedata2;
loop_TR2:
		lodsw
			and		ax,1111101111011110b;
		and		word ptr [edi],1111101111011110b;
		add		ax,[edi];
		shr		ax,1;
		mov		[edi],ax;
		add		edi,2;
		loop	loop_TR2;
exit_movedata2:
		
		mov		ax,word ptr l;
		add		esi,eax;
		jmp		loop_while2;
		
else_if:
		mov		ax,word ptr l;
		add		esi,eax;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		add		esi,2;
		lodsw
			add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		add		esi,2;
		
		mov		ax,wDxSize;
		add		dword ptr Dtmp,eax;
		dec		bx;
		jmp		loop_for1;
exit_for1:
		
		pop		es;
	};
}

void PutCmprsImgNOTClipingTrans565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, WORD MSK)
{
	DWORD	j, tmpclr;
	LPVOID	Dtmp = (LPVOID)( (char *)dest + y * dDxSize + (x * 2));
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		or		ebx,ebx;	// if(SY==0)
		jz		exit_1;
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		shr		ecx,1;
		
loop_TR:
		mov		ax,[esi];
		and		eax,1110011100011100b;
		mov		tmpclr,eax;
		
		mov		ax,[edi];
		and		eax,1110011100011100b;
		add		eax,tmpclr;
		
		shr		eax,2;
		mov		[edi],ax;
		add		esi,2;
		add		edi,2;
		loop	loop_TR;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,dDxSize;
		dec		bx;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}

void PutCmprsImgClipingTrans555( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, WORD MSK)
{
	int		l, kkk = lx << 1;
	LPVOID	Dtmp = (LPVOID)( (char *)dest + y * wDxSize + (x * 2 ));
	rx <<= 1;
	lx <<= 1;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		mov		esi,Stmp;
		
		mov		cx,word ptr ty;
		
		or		cx,cx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		
		mov		ax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		inc		esi;
		inc		esi;
		loop	loop_1;
exit_1:
	}
	_asm{
		mov		bx,word ptr by;		//
loop_for1:
		or		bx,bx;		//for(i=0;i<by;i++)
		jz		exit_for1;	//
		xor		eax,eax;
		xor		edx,edx;
		xor		ecx,ecx;
		mov		edi,Dtmp;
		
		mov		ax,word ptr lx;	//if(lx)
		or		ax,ax;			//
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		add		esi,2;
		cmp		dx,word ptr lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		dx,word ptr lx;		//j-=lx
		
		cmp		dx,word ptr rx;
		jg		jmp_00001;
		
		add		edi,edx;			//dtmp+=j
		
		lodsw;						//k=*stmp
		add		dx,ax;				//j+=k
		mov		cx,ax;				//
		
		cmp		dx,word ptr rx;	//new
		jle		jmp_0000;
		add		cx,word ptr rx;
		sub		cx,dx;
		
		jmp		xxxxxxx1;
jmp_00001:
		lodsw;
		add		esi,eax;
		jmp		ext_loop;
		
jmp_if1:///////////////////////////////////////
		lodsw;				//
		add		dx,ax;		//
		add		esi,eax;	//
		cmp		dx,word ptr lx;		//
		jle		loop_while1;
		sub		dx,word ptr lx;
		sub		esi,edx;
		mov		cx,dx;
		
		cmp		dx,word ptr rx;	//new
		jle		jmp_0000;		//"
		mov		cx,word ptr rx;	//"
xxxxxxx1:
jmp_0000:					//"
		
		//************ move	data ***********
		shr		cx,1;//*********************************************************
		or		cx,cx;
		jz		exit_movedata1;
loop_TR1:
		lodsw
			and		ax,1111101111011110b;
		and		word ptr [edi],1111101111011110b;
		add		ax,[edi];
		shr		ax,1;
		mov		[edi],ax;
		add		edi,2;
		loop	loop_TR1;
exit_movedata1:
		
		cmp		dx,word ptr rx;	//new		
		jle		exit_while1;	//"
		mov		eax,edx;		//"
		sub		ax,word ptr rx;	//"
		add		esi,eax;		//new
		
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		lodsw;
		
		add		edi,eax;	//dtmp+=*stmp
		add		dx,ax;		//j+=*stmp
		
		lodsw;
		mov		cx,ax;		//l=k=*stmp
		mov		word ptr l,ax;
		
		add		ax,dx;
		cmp		ax,word ptr rx;
		jle		exit_if0;
		cmp		dx,word ptr rx;
		jg		exit_if0;
		//mov		ax,cx;
		mov		cx,word ptr rx;
		sub		cx,dx;
exit_if0:
		cmp		dx,word ptr rx;
		jg		else_if;
		
		add		dx,word ptr l;	//j+=l;
		sub		word ptr l,cx;	//l-k;
		
		//************ move	data ***********
		shr		cx,1;//*********************************************************
		or		cx,cx;
		jz		exit_movedata2;
loop_TR2:
		lodsw
			and		ax,1111101111011110b;
		and		word ptr [edi],1111101111011110b;
		add		ax,[edi];
		shr		ax,1;
		mov		[edi],ax;
		add		edi,2;
		loop	loop_TR2;
exit_movedata2:
		
		mov		ax,word ptr l;
		add		esi,eax;
		jmp		loop_while2;
		
else_if:
		mov		ax,word ptr l;
		add		esi,eax;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		add		esi,2;
		lodsw
			add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		add		esi,2;
		
		mov		ax,wDxSize;
		add		dword ptr Dtmp,eax;
		dec		bx;
		jmp		loop_for1;
exit_for1:
		
		pop		es;
	};
}

void PutCmprsImgNOTClipingTrans555(int x, int y, int yl, LPVOID Stmp, LPVOID dest, WORD MSK)
{
	DWORD	j, tmpclr;
	LPVOID	Dtmp = (LPVOID)( (char *)dest + y * dDxSize + (x * 2));
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		or		ebx,ebx;	// if(SY==0)
		jz		exit_1;
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		shr		ecx,1;
		
loop_TR:
		mov		ax,[esi];
		and		eax,111001110011100b;
		mov		tmpclr,eax;
		
		mov		ax,[edi];
		and		eax,111001110011100b;
		add		eax,tmpclr;
		
		shr		eax,2;
		mov		[edi],ax;
		add		esi,2;
		add		edi,2;
		loop	loop_TR;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,dDxSize;
		dec		bx;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}



void PutCompressedImageTrans(int x, int y, Spr *sp, WORD MSK)
{
#ifdef _SDL
	SDL_Lock();
#endif
	int		lx, rx, ty, by , IsC=0;
	
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( xl == 0 ) return;
	if( yl == 0 ) return;
	
	
	x -= sp->ox;
	y -= sp->oy;
	
	
	if( x >= SCREEN_WIDTH  ) return; 
	else if( x + xl < 0 ) return; 
	
	if( y >= SCREEN_HEIGHT ) return; 
	else if( y + yl < 0  ) return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	
	
	if( x + xl >= SCREEN_WIDTH )
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if( x < 0 )
	{
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}
	
	if( y + yl >= SCREEN_HEIGHT )
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if( y < 0 )
	{
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}	
	
	
	if( _PixelInfo.BitMaskR == 0x7C00)		
	{
		if( IsC )
			PutCmprsImgClipingTrans555( x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, MSK);
		else
			PutCmprsImgNOTClipingTrans555( x, y, yl, sp->img, g_DestBackBuf, MSK);
	}
	else
	{
		if( IsC )
			PutCmprsImgClipingTrans565( x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, MSK);
		else
			PutCmprsImgNOTClipingTrans565( x, y, yl, sp->img, g_DestBackBuf, MSK);
	}
#ifdef _SDL2
	SDL_UnLock();
#endif
	return;
}
/////////////////////////////////////////////////////////////////////////// Put Sprite Transperensy..
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//			Put Sprite Normally...
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void	PutCmprsImgCliping(int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by)
{
	int		l;
	LPVOID Dtmp = (LPVOID)((char*)dest + y * dDxSize + (x * 2));
	
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		dx,ax;		//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		
		shr		ecx,2;		//move DATA
		jnc		Next2_1;	//
		movsw;				//
Next2_1:				//
		rep		movsd;		//
		
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		mov		ax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		ebx;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void	PutCmprsImgNOTCliping(int x, int y, int yl, LPVOID Stmp, LPVOID dest )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)( (char*)dest + y * dDxSize + (x * 2 ));
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		or		bx,bx;		// if(SY==0)
		jz		exit_1;
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,2;		//move DATA
		jnc		Next2;		//
		movsw;				//
Next2:					//
		rep		movsd;		//
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,dDxSize;
		dec		bx;
		jmp		loop_1;
exit_1:
		pop		es;
	}; 
}

void PutCompressedImage(int x, int y, Spr *sp )
{	
#ifdef _SDL
	SDL_Lock();
#endif
	int		lx, rx, ty, by , IsC=0;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( xl == 0 ) return;
	if( yl == 0 ) return;
	
	x -= sp->ox;
	y -= sp->oy;
	
	
	if( x >= SCREEN_WIDTH  ) return; 
	else if( x + xl < 0 ) return; 
	
	if( y >= SCREEN_HEIGHT ) return; 
	else if( y + yl < 0  ) return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	
	
	if( x + xl >= (SCREEN_WIDTH-1) )
	{
		rx -= x + xl - (SCREEN_WIDTH-1);
		IsC = 1;
	}
	if( x < 0 )
	{
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}
	
	if( y + yl >= SCREEN_HEIGHT )
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if( y < 0 )
	{
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}	
	
	
	if( sp->img == NULL ) return;
	
	
	if( IsC )
	{	
		PutCmprsImgCliping( x,  y, sp->img, g_DestBackBuf, lx, rx+1, ty, by);
	}	
	else
	{	
		PutCmprsImgNOTCliping( x,  y, sp->yl, sp->img, g_DestBackBuf );
	}	
#ifdef _SDL
	SDL_UnLock();
#endif
}		



//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//			Put Sprite Normally...
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void	PutCmprsImgClipingOneColor(int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, WORD color)
{
	int		l;
	LPVOID Dtmp = (LPVOID)((char*)dest + y * dDxSize + (x * 2));
	
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		dx,ax;		//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		mov		ax, color
			shl		eax, 16
			mov		ax, color
			
			add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		
		add		esi, ecx;
		sub		edi, 2
			add		ecx, 4
			shr		ecx,2;		//move DATA
		jnc		Next2_1;	//
		
		stosw;	//
		
Next2_1:				//
		
		rep		stosd;		//
		sub		edi, 2
			
			add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		mov		ax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		ebx;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void	PutCmprsImgNOTClipingOneColor(int x, int y, int yl, LPVOID Stmp, LPVOID dest, WORD color )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)( (char*)dest + y * dDxSize + (x * 2 ));
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		//	mov		ebx,j;
		
		mov     ax, color;
		shl		eax, 16;
		mov		ax, color;
		
		xor		ebx,ebx;
		xor		ecx,ecx;
loop_1:
		cmp 	j,0;		// if(SY==0)
		jz		exit_1;
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		bx,[esi];
		add		edi,ebx;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		add		esi, ecx;
		sub		edi, 2
			add		ecx, 4;
		shr		ecx, 2;
		jnc		Next2;		//
		
		stosw;				//
Next2:					//
		rep		stosd;		//
		sub		edi, 2
			
			jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,dDxSize;
		sub		j, 1;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}	
void PutCompressedImageOneColor(int x, int y, Spr *sp, WORD color )
{		
#ifdef _SDL
	SDL_Lock();
#endif
	int		lx, rx, ty, by , IsC=0;
	
	int		ey = SCREEN_HEIGHT - 128;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( xl == 0 ) return;
	if( yl == 0 ) return;
	
	x -= sp->ox;
	y -= sp->oy;
	
	
	if( x >= SCREEN_WIDTH -1 ) return; 
	else if( x + xl < 1 ) return; 
	
	if( y >= ey ) return; 
	else if( y + yl < 0  ) return;
	
	rx = xl;
	lx = 1;
	ty = 0;
	by = yl;
	
	
	
	if( x + xl >= SCREEN_WIDTH-1)
	{
		rx -= x + xl - (SCREEN_WIDTH-1);
		IsC = 1;
	}
	if( x < 1 )
	{
		lx  = 1 - x;
		x   = 1;
		rx -= lx;
		IsC = 1;
	}
	
	if( y + yl >= SCREEN_HEIGHT )
	{
		by -= y + yl - ey;
		IsC = 1;
	}
	if( y < 0 )
	{
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}	
	
	
	if( IsC )
	{	
		PutCmprsImgClipingOneColor( x,  y, sp->img, g_DestBackBuf, lx, rx, ty, by, color);
	}	
	else
	{	
		PutCmprsImgNOTClipingOneColor( x,  y, sp->yl, sp->img, g_DestBackBuf, color );
	}	
#ifdef _SDL
	SDL_UnLock();
#endif
}		





///////////////////////////////////////////////////////////////////////////// Put Sprite Normally...
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void	PutCmprsImgVoidCliping(int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by)
{
	int		l;
	LPVOID Dtmp = (LPVOID)((char*)dest + y * dDxSize + (x * 2));
	
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		dx,ax;		//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		
		//			shr		ecx,2;		//move DATA
		//			jnc		Next2_1;	//
		//			movsw;				//
		//		Next2_1:				//
		///			rep		movsd;		//
		//
		
		
		shr		ecx,2;		//move DATA
		jnc		Next2_1;		//
		mov		ax, word ptr [edi]
			mov		word ptr [edi-2], ax
			add		esi, 2
			add		edi, 2
			
Next2_1:	//
		
		cmp		cx, 0
			jz		NextVoid_2;
		
NextVoid:
		
		mov		eax, [edi];
		mov		[edi-2], eax
			add		esi, 4
			add		edi, 4
			
			loop	NextVoid
			
NextVoid_2:
		
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		mov		ax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		ebx;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void	PutCmprsImgVoidNOTCliping(int x, int y, int yl, LPVOID Stmp, LPVOID dest )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)( (char*)dest + y * dDxSize + (x * 2 ));
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		or		bx,bx;		// if(SY==0)
		jz		exit_1;
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax
			mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,2;		//move DATA
		jnc		Next2;		//
		mov		ax, word ptr [edi]
			mov		word ptr [edi-2], ax
			add		esi, 2
			add		edi, 2
			
Next2:	//
		
		cmp		cx, 0
			jz		loop_2;
		
NextVoid:
		
		mov		eax, [edi];
		mov		[edi-2], eax
			add		esi, 4
			add		edi, 4
			
			loop	NextVoid
			
			//		rep		movsd;		//
			
			jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,dDxSize;
		dec		bx;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}	

void PutCompressedImageVoid(int x, int y, Spr *sp )
{		
#ifdef _SDL
	SDL_Lock();
#endif
	int	lx, rx, ty, by , IsC=0;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( xl == 0 ) return;
	if( yl == 0 ) return;
	
	x -= sp->ox;
	y -= sp->oy;
	
	if( x >= (SCREEN_WIDTH-2)  )	return; 
	else if( x + xl < 0 )			return; 
	
	if( y >= SCREEN_HEIGHT )		return; 
	else if( y + yl < 0  )			return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	if( x + xl >= (SCREEN_WIDTH-2) )
	{
		rx -= x + xl - (SCREEN_WIDTH-2);
		IsC = 1;
	}
	if( x < 0 )
	{
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}
	
	if( y + yl >= SCREEN_HEIGHT )
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if( y < 0 )
	{
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}	
	
	if( IsC )
		PutCmprsImgVoidCliping( x,  y, sp->img, g_DestBackBuf, lx, rx, ty, by);
	else
		PutCmprsImgVoidNOTCliping( x,  y, sp->yl, sp->img, g_DestBackBuf );
#ifdef _SDL
	SDL_UnLock();
#endif
}		


void PutCompressImageApart( int x, int y, Spr *sp, int sx, int sy, int ex, int ey, int alpha )
{
#ifdef _SDL
	SDL_Lock();
#endif
	int		lx, rx, ty, by , IsC=0;
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	if( xl == 0 ) return;
	if( yl == 0 ) return;
	
	//	x -= sp->ox;
	//	y -= sp->oy;
	
	
	if( x >= ex  ) return; 
	else if( x + xl < sx ) return; 
	
	if( y >= ey ) return; 
	else if( y + yl < sy  ) return;
	
	rx = xl;
	lx = sx;
	ty = sy;
	by = yl;
	
	if( x + xl >= ex )
	{
		rx -= x + xl - ex;
	}
	if( x < sx )
	{
		lx  = sx - x;
		x   = sx;
		rx -= lx;
	}
	
	if( y + yl >= ey )
	{
		by -= y + yl - ey;
	}
	if( y < sy )
	{
		ty  = sy - y;
		y   = sy;
		by -= ty;
	}	
	
	if( alpha > 31 )
	{
		PutCmprsImgCliping( x,  y, sp->img, g_DestBackBuf, lx, rx, ty, by);
	}
	else 
	{
		if( _PixelInfo.BitMaskR == 0x7C00)
		{
			PutCmprsImgClipingBlend555(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
		}
		else
		{
			PutCmprsImgClipingBlend565(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
		}
	}
#ifdef _SDL
	SDL_UnLock();
#endif
}





void PutCompressImageSubApart( int x, int y, Spr *sp, int sx, int sy, int ex, int ey, int alpha )
{
#ifdef _SDL
	SDL_Lock();
#endif
	int		lx, rx, ty, by , IsC=0;
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	if( xl == 0 ) return;
	if( yl == 0 ) return;
	
	//	x -= sp->ox;
	//	y -= sp->oy;
	
	
	if( x >= ex  ) return; 
	else if( x + xl < sx ) return; 
	
	if( y >= ey ) return; 
	else if( y + yl < sy  ) return;
	
	rx = xl;
	lx = sx;
	ty = sy;
	by = yl;
	
	if( x + xl >= ex )
	{
		rx -= x + xl - ex;
	}
	if( x < sx )
	{
		lx  = sx - x;
		x   = sx;
		rx -= lx;
	}
	
	if( y + yl >= ey )
	{
		by -= y + yl - ey;
	}
	if( y < sy )
	{
		ty  = sy - y;
		y   = sy;
		by -= ty;
	}	
	
	if( alpha > 31 )
	{
		PutCmprsImgCliping( x,  y, sp->img, g_DestBackBuf, lx, rx, ty, by);
	}
	else 
	{
		if( _PixelInfo.BitMaskR == 0x7C00)
		{
			
			PutCmprsImgClipingSub555( x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
			
			//			PutCmprsImgClipingBlend555(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
		}
		else
		{
			PutCmprsImgClipingSub565( x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
			
			//			PutCmprsImgClipingBlend565(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
		}
	}
#ifdef _SDL
	SDL_UnLock();
#endif
}












////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//  Put Sprite FX ---------------

//  Put Sprite FX ---------------

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void PutCmprsImgClipingHBlend565(
								 int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by)
{
	int		l, kkk = lx << 1;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2 ));
	rx <<= 1;
	lx <<= 1;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		mov		esi,Stmp;
		
		mov		cx,word ptr ty;
		
		or		cx,cx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		
		mov		ax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		inc		esi;
		inc		esi;
		loop	loop_1;
exit_1:
	}
	_asm{
		mov		bx,word ptr by;		//
loop_for1:
		or		bx,bx;		//for(i=0;i<by;i++)
		jz		exit_for1;	//
		xor		eax,eax;
		xor		edx,edx;
		xor		ecx,ecx;
		mov		edi,Dtmp;
		
		mov		ax,word ptr lx;	//if(lx)
		or		ax,ax;			//
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		add		esi,2;
		cmp		dx,word ptr lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		dx,word ptr lx;		//j-=lx
		
		cmp		dx,word ptr rx;
		jg		jmp_00001;
		
		add		edi,edx;			//dtmp+=j
		
		lodsw;						//k=*stmp
		add		dx,ax;				//j+=k
		mov		cx,ax;				//
		
		cmp		dx,word ptr rx;	//new
		jle		jmp_0000;
		add		cx,word ptr rx;
		sub		cx,dx;
		
		jmp		xxxxxxx1;
jmp_00001:
		lodsw;
		add		esi,eax;
		jmp		ext_loop;
		
jmp_if1:///////////////////////////////////////
		lodsw;				//
		add		dx,ax;		//
		add		esi,eax;	//
		cmp		dx,word ptr lx;		//
		jle		loop_while1;
		sub		dx,word ptr lx;
		sub		esi,edx;
		mov		cx,dx;
		
		cmp		dx,word ptr rx;	//new
		jle		jmp_0000;		//"
		mov		cx,word ptr rx;	//"
xxxxxxx1:
jmp_0000:					//"
		
		//************ move	data ***********
		shr		cx,1;//*********************************************************
		or		cx,cx;
		jz		exit_movedata1;
loop_TR1:
		lodsw
			and		ax,1111011111011110b;
		and		word ptr [edi],1111011111011110b;
		add		ax,[edi];
		shr		ax,1;
		mov		[edi],ax;
		add		edi,2;
		loop	loop_TR1;
exit_movedata1:
		
		cmp		dx,word ptr rx;	//new		
		jle		exit_while1;	//"
		mov		eax,edx;		//"
		sub		ax,word ptr rx;	//"
		add		esi,eax;		//new
		
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		lodsw;
		add		edi,eax;	//dtmp+=*stmp
		add		dx,ax;		//j+=*stmp
		
		lodsw;
		mov		cx,ax;		//l=k=*stmp
		mov		word ptr l,ax;
		
		add		ax,dx;
		cmp		ax,word ptr rx;
		jle		exit_if0;
		cmp		dx,word ptr rx;
		jg		exit_if0;
		//mov		ax,cx;
		mov		cx,word ptr rx;
		sub		cx,dx;
exit_if0:
		cmp		dx,word ptr rx;
		jg		else_if;
		
		add		dx,word ptr l;	//j+=l;
		sub		word ptr l,cx;	//l-k;
		
		//************ move	data ***********
		shr		cx,1;//*********************************************************
		or		cx,cx;
		jz		exit_movedata2;
loop_TR2:
		lodsw
			and		ax,1111011111011110b;
		and		word ptr [edi],1111011111011110b;
		add		ax,[edi];
		shr		ax,1;
		mov		[edi],ax;
		add		edi,2;
		loop	loop_TR2;
exit_movedata2:
		
		mov		ax,word ptr l;
		add		esi,eax;
		jmp		loop_while2;
		
else_if:
		mov		ax,word ptr l;
		add		esi,eax;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		add		esi,2;
		lodsw
			add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		add		esi,2;
		
		mov		ax, wDxSize;
		add		dword ptr Dtmp,eax;
		dec		bx;
		jmp		loop_for1;
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingHBlend565(int x, int y, int yl, LPVOID Stmp, LPVOID dest)
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		or		ebx,ebx;	// if(SY==0)
		jz		exit_1;
loop_1:
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		shr		ecx,1;
		
loop_TR:
		mov		eax,[esi];
		mov		ebx,[edi];
		shr		eax,2;
		and		eax,0011000111000110b;
		and		ebx,1111011111011110b;
		
		add		eax,ebx;
		
		shr		eax,1;
		mov		[edi],ax;
		add		esi,2;
		add		edi,2;
		loop	loop_TR;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,dDxSize;
		dec		j;
		jnz		loop_1;
exit_1:
		pop		es;
	};
}

void PutCmprsImgClipingHBlend555(
								 int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by)
{
	int		l, kkk = lx << 1;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2 ));
	rx <<= 1;
	lx <<= 1;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		mov		esi,Stmp;
		
		mov		cx,word ptr ty;
		
		or		cx,cx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		
		mov		ax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		inc		esi;
		inc		esi;
		loop	loop_1;
exit_1:
	}
	_asm{
		mov		bx,word ptr by;		//
loop_for1:
		or		bx,bx;		//for(i=0;i<by;i++)
		jz		exit_for1;	//
		xor		eax,eax;
		xor		edx,edx;
		xor		ecx,ecx;
		mov		edi,Dtmp;
		
		mov		ax,word ptr lx;	//if(lx)
		or		ax,ax;			//
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		add		esi,2;
		cmp		dx,word ptr lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		dx,word ptr lx;		//j-=lx
		
		cmp		dx,word ptr rx;
		jg		jmp_00001;
		
		add		edi,edx;			//dtmp+=j
		
		lodsw;						//k=*stmp
		add		dx,ax;				//j+=k
		mov		cx,ax;				//
		
		cmp		dx,word ptr rx;	//new
		jle		jmp_0000;
		add		cx,word ptr rx;
		sub		cx,dx;
		
		jmp		xxxxxxx1;
jmp_00001:
		lodsw;
		add		esi,eax;
		jmp		ext_loop;
		
jmp_if1:///////////////////////////////////////
		lodsw;				//
		add		dx,ax;		//
		add		esi,eax;	//
		cmp		dx,word ptr lx;		//
		jle		loop_while1;
		sub		dx,word ptr lx;
		sub		esi,edx;
		mov		cx,dx;
		
		cmp		dx,word ptr rx;	//new
		jle		jmp_0000;		//"
		mov		cx,word ptr rx;	//"
xxxxxxx1:
jmp_0000:					//"
		
		//************ move	data ***********
		shr		cx,1;//*********************************************************
		or		cx,cx;
		jz		exit_movedata1;
loop_TR1:
		lodsw
			and		ax,111101111011110b;
		and		word ptr [edi],111101111011110b;
		add		ax,[edi];
		shr		ax,1;
		mov		[edi],ax;
		add		edi,2;
		loop	loop_TR1;
exit_movedata1:
		
		cmp		dx,word ptr rx;	//new		
		jle		exit_while1;	//"
		mov		eax,edx;		//"
		sub		ax,word ptr rx;	//"
		add		esi,eax;		//new
		
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		lodsw;
		add		edi,eax;	//dtmp+=*stmp
		add		dx,ax;		//j+=*stmp
		
		lodsw;
		mov		cx,ax;		//l=k=*stmp
		mov		word ptr l,ax;
		
		add		ax,dx;
		cmp		ax,word ptr rx;
		jle		exit_if0;
		cmp		dx,word ptr rx;
		jg		exit_if0;
		//mov		ax,cx;
		mov		cx,word ptr rx;
		sub		cx,dx;
exit_if0:
		cmp		dx,word ptr rx;
		jg		else_if;
		
		add		dx,word ptr l;	//j+=l;
		sub		word ptr l,cx;	//l-k;
		
		//************ move	data ***********
		shr		cx,1;//*********************************************************
		or		cx,cx;
		jz		exit_movedata2;
loop_TR2:
		lodsw
			and		ax,111101111011110b;
		and		word ptr [edi],111101111011110b;
		add		ax,[edi];
		shr		ax,1;
		mov		[edi],ax;
		add		edi,2;
		loop	loop_TR2;
exit_movedata2:
		
		mov		ax,word ptr l;
		add		esi,eax;
		jmp		loop_while2;
		
else_if:
		mov		ax,word ptr l;
		add		esi,eax;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		add		esi,2;
		lodsw
			add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		add		esi,2;
		
		mov		ax, wDxSize;
		add		dword ptr Dtmp,eax;
		dec		bx;
		jmp		loop_for1;
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingHBlend555(int x, int y, int yl, LPVOID Stmp, LPVOID dest)
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		or		ebx,ebx;	// if(SY==0)
		jz		exit_1;
loop_1:
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		shr		ecx,1;
		
loop_TR:
		mov		eax,[esi];
		mov		ebx,[edi];
		shr		eax,2;
		and		eax,001100011000110b;
		and		ebx,111101111011110b;
		
		add		eax,ebx;
		
		shr		eax,1;
		mov		[edi],ax;
		add		esi,2;
		add		edi,2;
		loop	loop_TR;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,dDxSize;
		dec		j;
		jnz		loop_1;
exit_1:
		pop		es;
	};
}



void PutCmprsImgClipingBlend565(
								int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2 ));
	
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		mov		ebx,alpha;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		eax, by;		//
		or		eax,eax;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		xor		eax,eax;
		mov		ax,[esi];
		ror		eax,11;
		xor		edx,edx;
		shl		ax,10;
		rol		eax,6;
		shl		ax,5;
		rol		eax,5;
		
		mov		dx,[edi];
		ror		edx,11;
		shl		dx,10;
		rol		edx,6;
		shl		dx,5;
		rol		edx,5;
		
		imul	eax,ebx;
		shr		eax,5;
		and		eax,0x03e0fc1f;
		add		eax,edx;
		imul	edx,ebx;
		shr		edx,5;
		and		eax,0x03e0fc1f;
		
		sub		eax,edx;
		
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
		
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingBlend565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD alpha)
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		xor		eax,eax;
		mov		ax,[esi];
		ror		eax,11;
		xor		edx,edx;
		shl		ax,10;
		rol		eax,6;
		shl		ax,5;
		rol		eax,5;
		
		mov		dx,[edi];
		ror		edx,11;
		shl		dx,10;
		rol		edx,6;
		shl		dx,5;
		rol		edx,5;
		
		imul	eax,ebx;
		shr		eax,5;
		and		eax,0x03e0fc1f;
		add		eax,edx;
		imul	edx,ebx;
		shr		edx,5;
		and		eax,0x03e0fc1f;
		
		sub		eax,edx;
		
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};	
}		

void PutCmprsImgClipingBlend555(
								int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2 ));
	
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		mov		ebx,alpha;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		eax, by;		//
		or		eax,eax;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		xor		eax,eax;
		mov		ax,[esi];
		ror		eax,10;
		xor		edx,edx;
		shl		ax,11;
		rol		eax,5;
		shl		ax,6;
		rol		eax,5;
		
		mov		dx,[edi];
		ror		edx,10;
		shl		dx,11;
		rol		edx,5;
		shl		dx,6;
		rol		edx,5;
		
		imul	eax,ebx;
		shr		eax,5;
		and		eax,0x07e0fc1f;
		add		eax,edx;
		imul	edx,ebx;
		shr		edx,5;
		and		eax,0x07e0fc1f;
		
		sub		eax,edx;
		
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
		
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingBlend555(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD alpha)
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		xor		eax,eax;
		mov		ax,[esi];
		ror		eax,10;
		xor		edx,edx;
		shl		ax,11;
		rol		eax,5;
		shl		ax,6;
		rol		eax,5;
		
		mov		dx,[edi];
		ror		edx,10;
		shl		dx,11;
		rol		edx,5;
		shl		dx,6;
		rol		edx,5;
		
		imul	eax,ebx;
		shr		eax,5;
		and		eax,0x03e0fc1f;
		add		eax,edx;
		imul	edx,ebx;
		shr		edx,5;
		and		eax,0x03e0fc1f;
		
		sub		eax,edx;
		
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};	
}		
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


void PutCmprsImgClipingBlendDifer565(
									 int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2 ));
	
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		mov		ebx,alpha;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		eax, by;		//
		or		eax,eax;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		xor		eax,eax;
		xor		edx,edx;
		mov		ax,[esi];
		mov		dx,[edi];
		
		cmp		ax, dx
			jz		skip_same_color_
			
			ror		eax,11;
		shl		ax,10;
		rol		eax,6;
		shl		ax,5;
		rol		eax,5;
		
		
		ror		edx,11;
		shl		dx,10;
		rol		edx,6;
		shl		dx,5;
		rol		edx,5;
		
		imul	eax,ebx;
		shr		eax,5;
		and		eax,0x03e0fc1f;
		add		eax,edx;
		imul	edx,ebx;
		shr		edx,5;
		and		eax,0x03e0fc1f;
		
		sub		eax,edx;
		
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
skip_same_color_:
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
		
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingBlendDifer565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD alpha)
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		xor		eax,eax;
		xor		edx,edx;
		mov		ax,[esi];
		mov		dx,[edi];
		
		cmp		ax, dx
			jz		skip_same_color_
			
			ror		eax,11;
		
		shl		ax,10;
		rol		eax,6;
		shl		ax,5;
		rol		eax,5;
		
		
		ror		edx,11;
		shl		dx,10;
		rol		edx,6;
		shl		dx,5;
		rol		edx,5;
		
		imul	eax,ebx;
		shr		eax,5;
		and		eax,0x03e0fc1f;
		add		eax,edx;
		imul	edx,ebx;
		shr		edx,5;
		and		eax,0x03e0fc1f;
		
		sub		eax,edx;
		
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
skip_same_color_:
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};	
}		

void PutCmprsImgClipingBlendDifer555(
									 int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2 ));
	
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		mov		ebx,alpha;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		eax, by;		//
		or		eax,eax;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		xor		eax,eax;
		xor		edx,edx;
		
		mov		ax,[esi];
		mov		dx,[edi];
		
		cmp		ax, dx
			jz		skip_same_color_
			
			ror		eax,10;
		shl		ax,11;
		rol		eax,5;
		shl		ax,6;
		rol		eax,5;
		
		
		ror		edx,10;
		shl		dx,11;
		rol		edx,5;
		shl		dx,6;
		rol		edx,5;
		
		imul	eax,ebx;
		shr		eax,5;
		and		eax,0x07e0fc1f;
		add		eax,edx;
		imul	edx,ebx;
		shr		edx,5;
		and		eax,0x07e0fc1f;
		
		sub		eax,edx;
		
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
skip_same_color_:
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
		
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingBlendDifer555(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD alpha)
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		xor		eax,eax;
		xor		edx,edx;
		
		mov		ax,[esi];
		mov		dx,[edi];
		
		cmp		ax, dx
			jz		skip_same_color_
			
			ror		eax,10;
		shl		ax,11;
		rol		eax,5;
		shl		ax,6;
		rol		eax,5;
		
		
		ror		edx,10;
		shl		dx,11;
		rol		edx,5;
		shl		dx,6;
		rol		edx,5;
		
		imul	eax,ebx;
		shr		eax,5;
		and		eax,0x03e0fc1f;
		add		eax,edx;
		imul	edx,ebx;
		shr		edx,5;
		and		eax,0x03e0fc1f;
		
		sub		eax,edx;
		
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
skip_same_color_:
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};	
}		

////////////////////////////////////////////////////////////////////////////////////////////////////////


void PutCmprsImgClipingAdd565(
							  int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{		
	int		l;
	
	
	LPVOID	Dtmp = (LPVOID)(( char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}	
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		dx,ax;	//j+=*stmp		// add		edx,eax;	//j+=*stmp  001216 KHS
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		movzx	eax,word ptr[esi];
		ror		eax,11;			//
		shl		ax,10;			//
		rol		eax,6;			//
		shl		ax,5;			
		rol		eax,5;			
		
		movzx	edx,word ptr[edi];
		ror		edx,11;			//
		shl		dx,10;			//
		rol		edx,6;			//
		shl		dx,5;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x07e1fc3f;	//
		add		eax,edx;
		
		xor		edx,edx;
		mov		dl,al;
		mov		bx,word ptr[offset AddTableB + edx*2];	//
		shr		eax,10;
		mov		dl,al;
		or		bx,word ptr[offset AddTableG + edx*2];
		shr		eax,11;
		mov		dl,al;
		or		bx,word ptr[offset AddTableR + edx*2];	//
		
		mov		[edi],bx;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		mov		ax,[esi+2];		//movzx	eax,[esi+2]; 001216 KHS
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingAdd565(int x, int y, int yl, LPVOID Stmp, LPVOID dest,  DWORD alpha)
{
	static	DWORD	ttt = 0;
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2));
	
	j = yl;
	_asm{
		mov		al,0xff
			
			movsx	eax,al;
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2; 
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		mov		ax,[esi];
		ror		eax,11;			//
		shl		ax,10;			//
		rol		eax,6;			//
		shl		ax,5;			
		rol		eax,5;			
		
		
		mov		dx,[edi];
		ror		edx,11;			//
		shl		dx,10;			//
		rol		edx,6;			//
		shl		dx,5;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x07e1fc3f;	//
		add		eax,edx;
		
		xor		edx,edx;
		mov		dl,al;
		mov		bx,word ptr[offset AddTableB + edx*2];	//
		shr		eax,10;
		mov		dl,al;
		or		bx,word ptr[offset AddTableG + edx*2];
		shr		eax,11;
		mov		dl,al;
		or		bx,word ptr[offset AddTableR + edx*2];	//
		
		mov		[edi],bx;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	}
}	

void PutCmprsImgClipingAdd555( 
							  int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{		
	int		l;
	LPVOID	Dtmp = (LPVOID)(( char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}	
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		movzx	eax,word ptr[esi];
		ror		eax,10;			//
		shl		ax,11;			//
		rol		eax,5;			//
		shl		ax,6;			
		rol		eax,5;			
		
		movzx	edx,word ptr[edi];
		ror		edx,10;			//
		shl		dx,11;			//
		rol		edx,5;			//
		shl		dx,6;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x07e1f83f;	//
		add		eax,edx;
		
		xor		edx,edx;								
		mov		dl,al;									
		mov		bx,word ptr[offset AddTableB + edx*2];	
		shr		eax,11;									
		mov		dl,al;									
		or		bx,word ptr[offset AddTableG + edx*2];	
		shr		eax,10;									
		mov		dl,al;									
		or		bx,word ptr[offset AddTableR + edx*2];	
		
		mov		[edi],bx;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}


void PutCmprsImgNOTClipingAdd555(int x, int y, int yl, LPVOID Stmp, LPVOID dest,  DWORD alpha)
{
	static	DWORD	ttt = 0;
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2));
	
	j = yl;
	_asm{
		mov		al,0xff
			
			movsx	eax,al;
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2; 
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		mov		ax,[esi];
		ror		eax,10;			//
		shl		ax,11;			//
		rol		eax,5;			//
		shl		ax,6;			
		rol		eax,5;			
		
		
		mov		dx,[edi];		
		ror		edx,10;			//
		shl		dx,11;			//
		rol		edx,5;			//
		shl		dx,6;			
		rol		edx,5;			
		
		imul	eax,alpha;		
		shr		eax,4;			
		and		eax,0x07e1f83f;	//
		add		eax,edx;		
		
		xor		edx,edx;		
		mov		dl,al;			
		mov		bx,word ptr[offset AddTableB + edx*2];	//
		shr		eax,11;			
		mov		dl,al;			
		or		bx,word ptr[offset AddTableG + edx*2];
		shr		eax,10;
		mov		dl,al;
		or		bx,word ptr[offset AddTableR + edx*2];	//
		
		mov		[edi],bx;
		
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	}
}	


void PutCmprsImgClipingSub565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		//------------------------
		xor		eax,eax;
		xor		edx,edx;
		
		mov		ax,[esi];
		ror		eax,11;			//
		shl		ax,10;			//
		rol		eax,6;			//
		shl		ax,5;			
		rol		eax,5;			
		
		
		mov		dx,[edi];
		ror		edx,11;			//
		shl		dx,10;			//
		rol		edx,6;			//
		shl		dx,5;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x03e0fc1f;	//
		
		
		or		edx,0x80100200;
		sub		edx,eax;
		mov		eax,edx;
		
		movsx	edx,al;
		mov		bx,word ptr[offset SubTableB + 64 + edx*2];	//
		shr		eax,10;
		movsx	edx,al;
		or		bx,word ptr[offset SubTableG + 128 + edx*2];
		shr		eax,11;
		movsx	edx,al;
		or		bx,word ptr[offset SubTableR + 64 + edx*2];	//
		
		mov		[edi],bx;
		
		//---------------------------
		
		/*		movzx	eax,word ptr[esi];
		ror		eax,11;			//
		shl		ax,10;			//
		rol		eax,6;			//
		shl		ax,5;			
		rol		eax,5;			
		
		  movzx	edx,word ptr[edi];
		  ror		edx,11;			//
		  shl		dx,10;			//
		  rol		edx,6;			//
		  shl		dx,5;			
		  rol		edx,5;			
		  
			imul	eax,alpha;
			shr		eax,4;
			and		eax,0x07e1fc3f;	//
			add		eax,edx;
			
			  xor		edx,edx;
			  mov		dl,al;
			  mov		bx,word ptr[offset AddTableB + edx*2];	//
			  shr		eax,10;
			  mov		dl,al;
			  or		bx,word ptr[offset AddTableG + edx*2];
			  shr		eax,11;
			  mov		dl,al;
			  or		bx,word ptr[offset AddTableR + edx*2];	//
			  
				mov		[edi],bx;
		*/
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingSub565(int x, int y, int yl, LPVOID Stmp, LPVOID dest , DWORD alpha)
{
	static	DWORD	ttt = 0;
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)( (char*)dest  + y * dDxSize + (x * 2));
	
	j = yl;
	_asm{
		mov		al,0xff
			
			movsx	eax,al;
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2; 
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		xor		eax,eax;
		xor		edx,edx;
		
		mov		ax,[esi];
		ror		eax,11;			//
		shl		ax,10;			//
		rol		eax,6;			//
		shl		ax,5;			
		rol		eax,5;			
		
		
		mov		dx,[edi];
		ror		edx,11;			//
		shl		dx,10;			//
		rol		edx,6;			//
		shl		dx,5;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x03e0fc1f;	//
		
		
		or		edx,0x80100200;
		sub		edx,eax;
		mov		eax,edx;
		
		movsx	edx,al;
		mov		bx,word ptr[offset SubTableB + 64 + edx*2];	//
		shr		eax,10;
		movsx	edx,al;
		or		bx,word ptr[offset SubTableG + 128 + edx*2];
		shr		eax,11;
		movsx	edx,al;
		or		bx,word ptr[offset SubTableR + 64 + edx*2];	//
		
		mov		[edi],bx;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	}
}

void PutCmprsImgClipingSub555( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		
		
		//---------------------------------------------
		xor		eax,eax;
		xor		edx,edx;
		
		mov		ax,[esi];
		ror		eax,10;			//
		shl		ax,11;			//
		rol		eax,5;			//
		shl		ax,6;			
		rol		eax,5;			
		
		
		mov		dx,[edi];
		ror		edx,10;			//
		shl		dx,11;			//
		rol		edx,5;			//
		shl		dx,6;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x03e0f81f;	//
		or		edx,0x80100200;
		sub		edx,eax;
		mov		eax,edx;
		
		movsx	edx,al;
		mov		bx,word ptr[offset SubTableB + 64 + edx*2];	//
		shr		eax,11;
		movsx	edx,al;
		or		bx,word ptr[offset SubTableG + 128 + edx*2];
		shr		eax,10;
		movsx	edx,al;
		or		bx,word ptr[offset SubTableR + 64 + edx*2];	//
		
		mov		[edi],bx;
		
		//----------------------------------------------
		/*		movzx	eax,word ptr[esi];
		ror		eax,10;			//
		shl		ax,11;			//
		rol		eax,5;			//
		shl		ax,6;			
		rol		eax,5;			
		
		  movzx	edx,word ptr[edi];
		  ror		edx,10;			//
		  shl		dx,11;			//
		  rol		edx,5;			//
		  shl		dx,6;			
		  rol		edx,5;			
		  
			imul	eax,alpha;
			shr		eax,4;
			and		eax,0x07e1f83f;	//
			add		eax,edx;
			
			  xor		edx,edx;
			  mov		dl,al;
			  mov		bx,word ptr[offset AddTableB + edx*2];	//
			  shr		eax,11;
			  mov		dl,al;
			  or		bx,word ptr[offset AddTableG + edx*2];
			  shr		eax,10;
			  mov		dl,al;
			  or		bx,word ptr[offset AddTableR + edx*2];	//
			  
				mov		[edi],bx;
		*/
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingSub555(int x, int y, int yl, LPVOID Stmp, LPVOID dest , DWORD alpha)
{
	static	DWORD	ttt = 0;
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)( (char*)dest  + y * dDxSize + (x * 2));
	
	j = yl;
	_asm{
		mov		al,0xff
			
			movsx	eax,al;
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2; 
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		xor		eax,eax;
		xor		edx,edx;
		
		mov		ax,[esi];
		ror		eax,10;			//
		shl		ax,11;			//
		rol		eax,5;			//
		shl		ax,6;			
		rol		eax,5;			
		
		
		mov		dx,[edi];
		ror		edx,10;			//
		shl		dx,11;			//
		rol		edx,5;			//
		shl		dx,6;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x03e0f81f;	//
		or		edx,0x80100200;
		sub		edx,eax;
		mov		eax,edx;
		
		movsx	edx,al;
		mov		bx,word ptr[offset SubTableB + 64 + edx*2];	//
		shr		eax,11;
		movsx	edx,al;
		or		bx,word ptr[offset SubTableG + 128 + edx*2];
		shr		eax,10;
		movsx	edx,al;
		or		bx,word ptr[offset SubTableR + 64 + edx*2];	//
		
		mov		[edi],bx;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	}
}

//==============================================================================================================
//========= Add only Diferrent color ===========================================================================
//==============================================================================================================
//==============================================================================================================
void PutCmprsImgClipingAddDifer565(
								   int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{		
	int		l;
	LPVOID	Dtmp = (LPVOID)(( char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}	
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		movzx	eax,word ptr[esi];
		movzx	edx,word ptr[edi];
		
		cmp		eax, edx
			jz		skip_color_
			
			ror		eax,11;			//
		shl		ax,10;			//
		rol		eax,6;			//
		shl		ax,5;			
		rol		eax,5;			
		
		
		ror		edx,11;			//
		shl		dx,10;			//
		rol		edx,6;			//
		shl		dx,5;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x07e1fc3f;	//
		add		eax,edx;
		
		xor		edx,edx;
		mov		dl,al;
		mov		bx,word ptr[offset AddTableB + edx*2];	//
		shr		eax,10;
		mov		dl,al;
		or		bx,word ptr[offset AddTableG + edx*2];
		shr		eax,11;
		mov		dl,al;
		or		bx,word ptr[offset AddTableR + edx*2];	//
		
		mov		[edi],bx;
		
skip_color_:
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingAddDifer565(int x, int y, int yl, LPVOID Stmp, LPVOID dest,  DWORD alpha)
{
	static	DWORD	ttt = 0;
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2));
	
	j = yl;
	_asm{
		mov		al,0xff
			
			movsx	eax,al;
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2; 
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		mov		ax,[esi];
		mov		dx,[edi];
		
		cmp     ax, dx
			jz      skip_color_
			
			ror		eax,11;			//
		shl		ax,10;			//
		rol		eax,6;			//
		shl		ax,5;			
		rol		eax,5;			
		
		ror		edx,11;			//
		shl		dx,10;			//
		rol		edx,6;			//
		shl		dx,5;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x07e1fc3f;	//
		add		eax,edx;
		
		xor		edx,edx;
		mov		dl,al;
		mov		bx,word ptr[offset AddTableB + edx*2];	//
		shr		eax,10;
		mov		dl,al;
		or		bx,word ptr[offset AddTableG + edx*2];
		shr		eax,11;
		mov		dl,al;
		or		bx,word ptr[offset AddTableR + edx*2];	//
		
		mov		[edi],bx;
		
skip_color_ :
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	}
}	

void PutCmprsImgClipingAddDifer555( 
								   int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD alpha)
{		
	int		l;
	LPVOID	Dtmp = (LPVOID)(( char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}	
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		movzx	eax,word ptr[esi];
		movzx	edx,word ptr[edi];
		
		cmp		eax, edx
			jz		skip_color_
			
			ror		eax,10;			//
		shl		ax,11;			//
		rol		eax,5;			//
		shl		ax,6;			
		rol		eax,5;			
		
		
		ror		edx,10;			//
		shl		dx,11;			//
		rol		edx,5;			//
		shl		dx,6;			
		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x07e1f83f;	//
		add		eax,edx;
		
		xor		edx,edx;								
		mov		dl,al;									
		mov		bx,word ptr[offset AddTableB + edx*2];	
		shr		eax,11;									
		mov		dl,al;									
		or		bx,word ptr[offset AddTableG + edx*2];	
		shr		eax,10;									
		mov		dl,al;									
		or		bx,word ptr[offset AddTableR + edx*2];	
		
		mov		[edi],bx;
		
skip_color_:
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}


void PutCmprsImgNOTClipingAddDifer555(int x, int y, int yl, LPVOID Stmp, LPVOID dest,  DWORD alpha)
{
	static	DWORD	ttt = 0;
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2));
	
	j = yl;
	_asm{
		mov		al,0xff
			
			movsx	eax,al;
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2; 
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		mov		ax,[esi];
		mov		dx,[edi];		
		
		cmp		ax, dx
			jz		skip_color_
			
			ror		eax,10;			//
		shl		ax,11;			//
		rol		eax,5;			//
		shl		ax,6;			
		rol		eax,5;			
		
		ror		edx,10;			//
		shl		dx,11;			//
		rol		edx,5;			//
		shl		dx,6;			
		rol		edx,5;			
		
		imul	eax,alpha;		
		shr		eax,4;			
		and		eax,0x07e1f83f;	//
		add		eax,edx;		
		
		xor		edx,edx;		
		mov		dl,al;			
		mov		bx,word ptr[offset AddTableB + edx*2];	//
		shr		eax,11;			
		mov		dl,al;			
		or		bx,word ptr[offset AddTableG + edx*2];
		shr		eax,10;
		mov		dl,al;
		or		bx,word ptr[offset AddTableR + edx*2];	//
		
		mov		[edi],bx;
		
skip_color_:
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	}
}

void PutCompressedImageFX(int x, int y, Spr *sp, DWORD alpha, DWORD op)
{
#ifdef _SDL
	SDL_Lock();
#endif
	int		lx, rx, ty, by , IsC=0;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( xl == 0 ) return;
	if( yl == 0 ) return;
	
	x -= sp->ox;
	y -= sp->oy;
	
	
	if( x >= SCREEN_WIDTH  ) return; 
	else if( x + xl < 0 ) return; 
	
	if( y >= SCREEN_HEIGHT ) return; 
	else if( y + yl < 0  ) return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	
	
	
	if( x + xl >= SCREEN_WIDTH )
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if( x < 0 )
	{
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}
	
	if( y + yl >= SCREEN_HEIGHT )
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}
	if( y < 0 )
	{
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}
	
	
	//-----------------------
	if( _PixelInfo.BitMaskR == 0x7C00)
	{
		if(IsC){
			switch(op){
			case 1://alpha
				PutCmprsImgClipingBlend555(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			case 2://add
				PutCmprsImgClipingAdd555(x, y,  sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			case 3://sub
				PutCmprsImgClipingSub555(x, y,  sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			case 4://H alpha
				PutCmprsImgClipingHBlend555(x, y,  sp->img, g_DestBackBuf, lx, rx, ty, by);
				break;
			case 5:// Add not put the same colors between dest and source
				PutCmprsImgClipingAddDifer555(x, y,  sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			case 6://alpha not put the same colors between dest and source
				PutCmprsImgClipingBlendDifer555(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			}
		}
		else
			switch(op){ 
			case 1://alpha
				PutCmprsImgNOTClipingBlend555(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
			case 2://add
				PutCmprsImgNOTClipingAdd555(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
			case 3://sub
				PutCmprsImgNOTClipingSub555(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
			case 4://H alpha
				PutCmprsImgNOTClipingHBlend555(x, y, yl,  sp->img, g_DestBackBuf);
				break;
			case 5://add not put the same colors between dest and source
				PutCmprsImgNOTClipingAddDifer555(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
			case 6://alpha not put the same colors between dest and source
				PutCmprsImgNOTClipingBlendDifer555(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
		}
	}
	else
	{
		if(IsC){
			switch(op){
			case 1://alpha
				PutCmprsImgClipingBlend565(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			case 2://add
				if( sp->yl == 464 )
				{
					_asm nop;
					
				}
				
				PutCmprsImgClipingAdd565(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				//PutCmprsImgClipingOneColor(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				//PutCmprsImgClipingBlend565
				//PutCmprsImgClipingSub565
				
				//PutCmprsImgClipingHBlend565(x, y,  sp->img, g_DestBackBuf, lx, rx, ty, by);
				break;
			case 3://sub
				PutCmprsImgClipingSub565(x, y,  sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			case 4://H alpha
				PutCmprsImgClipingHBlend565(x, y,  sp->img, g_DestBackBuf, lx, rx, ty, by);
				break;
			case 5://add not put the same colors between dest and source
				PutCmprsImgClipingAddDifer565(x, y,  sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			case 6://alpha not put the same colors between dest and source
				PutCmprsImgClipingBlendDifer565(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, alpha);
				break;
			}
		}
		else
			switch(op){ 
			case 1://alpha
				PutCmprsImgNOTClipingBlend565(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
			case 2://add
				PutCmprsImgNOTClipingAdd565(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
			case 3://sub
				PutCmprsImgNOTClipingSub565(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
			case 4://H alpha
				PutCmprsImgNOTClipingHBlend565(x, y, yl,  sp->img, g_DestBackBuf);
				break;
			case 5://add  not put the same colors between dest and source
				PutCmprsImgNOTClipingAddDifer565(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
			case 6://alpha not put the same colors between dest and source
				PutCmprsImgNOTClipingBlendDifer565(x, y, yl,  sp->img, g_DestBackBuf, alpha);
				break;
		}
	}
#ifdef _SDL
	SDL_UnLock();
#endif
}		

void PutCmprsImgNOTClipingShadow565(int x, int y, int yl, LPVOID Stmp, LPVOID dest)
{
	DWORD  sk, j;
	
	x +=( yl/2 );
	y += (yl/2);
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	j = yl;
	sk = dDxSize-2;
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		or		ebx,ebx;	// if(SY==0)
		jz		exit_1;
loop_1:
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		shr		ecx,1;
		
loop_TR:
		mov		ebx,[edi];
		and		ebx,1111011111011110b;
		shr		ebx,1;
		mov		[edi],bx;
		add		esi,2;
		add		edi,2;
		loop	loop_TR;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,sk;
		dec		j;
		jz		exit_1;
		
		//---------------------
		mov		edi,edx;
		
loop_22:	
		cmp		word ptr[esi],0xffff;
		jz		exit_22;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		add     esi, ecx
			jmp		loop_22;
		
exit_22:
		add		esi,2;
		
		dec		j;
		jnz		loop_1;
		//---------------------
exit_1:
		pop		es;
	};	
}																							
void PutCmprsImgNOTClipingShadow555(int x, int y, int yl, LPVOID Stmp, LPVOID dest)
{		
	DWORD  sk, j;
	
	x +=( yl/2 );
	y += (yl/2);
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	j = yl;
	sk = dDxSize-2;
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
		
		or		ebx,ebx;	// if(SY==0)
		jz		exit_1;
loop_1:
		mov		edi,edx;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		shr		ecx,1;
		
loop_TR:
		mov		ebx,[edi];
		and		ebx,111101111011110b;
		shr		ebx,1;
		mov		[edi],bx;
		add		esi,2;
		add		edi,2;
		loop	loop_TR;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,sk;
		dec		j;
		jz		exit_1;
		
		//---------------------
		mov		edi,edx;
		
loop_22:	
		cmp		word ptr[esi],0xffff;
		jz		exit_22;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		add     esi, ecx
			jmp		loop_22;
		
exit_22:
		add		esi,2;
		
		dec		j;
		jnz		loop_1;
		//---------------------
exit_1:
		pop		es;
	};	
}																							
void PutCompressedImageShadow( int x, int y, Spr *sp )
{	
#ifdef _SDL
	SDL_Lock();
#endif
	int	lx, rx, ty, by , IsC=0;
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	x -= sp->ox;
	y -= sp->oy;
	
	if( x >= SCREEN_WIDTH  )			return; 
	else if( x + xl < 0 )	return; 
	if( y >= GAME_SCREEN_YSIZE )			return; 
	else if( y + yl < 0  )	return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	if( x + xl >= SCREEN_WIDTH )
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if( x < 0 )
	{
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}
	if( y + yl >= GAME_SCREEN_YSIZE )
	{
		by -= y + yl - GAME_SCREEN_YSIZE;
		IsC = 1;
	}
	if( y < 0 )
	{
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}
	
	if( _PixelInfo.BitMaskR == 0x7C00)
	{
		if(IsC){
		}
		else 
		{
			PutCmprsImgNOTClipingShadow555( x, y, yl, sp->img, g_DestBackBuf);
		}
	}
	else
	{
		if(IsC){
		}
		else 
		{
			PutCmprsImgNOTClipingShadow565( x, y, yl, sp->img, g_DestBackBuf);
		}
	}
#ifdef _SDL
	SDL_UnLock();
#endif
}	






//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// Sprite를 불투명, Light(명암)를 넣는다. 
//        
// alpha  :  0 - n : 0이면 Sprite는 어두워진다. 
// 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void PutCmprsImgNOTClipingLight(int x, int y, int yl, LPVOID Stmp, LPVOID dest,  DWORD alpha)
{
	static	DWORD	ttt = 0;
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2));
	
	j = yl;
	_asm{
		mov		al,0xff
			
			movsx	eax,al;
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		mov		ebx,alpha;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2; 
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		mov		ax,[esi];
		ror		eax,11;			//
		shl		ax,10;			//
		rol		eax,6;			//
		shl		ax,5;			
		rol		eax,5;			
		
		
		//		mov		dx,[edi];
		//		ror		edx,11;			//
		//		shl		dx,10;			//
		//		rol		edx,6;			//
		//		shl		dx,5;			
		//		rol		edx,5;			
		
		imul	eax,alpha;
		shr		eax,4;
		and		eax,0x07e1fc3f;	//
		
		xor		edx,edx;
		mov		dl,al;
		
		mov		bx,word ptr[offset AddTableB + edx*2];	//
		shr		eax,10;
		mov		dl,al;
		or		bx,word ptr[offset AddTableG + edx*2];
		shr		eax,11;
		mov		dl,al;
		or		bx,word ptr[offset AddTableR + edx*2];	//
		
		mov		[edi],bx;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	}
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ putsprite light....



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@
//@@@
//@@@     Sprite 출력... + RGB ...
//@@@
//@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void PutCmprsImgNOTClipingBlendRGB565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		//		mov		ebx,RGB;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		xor		eax,eax;
		mov		ax,[esi];
		ror		eax,11;
		shl		ax,10;
		rol		eax,6;
		shl		ax,5;
		rol		eax,5;
		
		and     eax, RGB;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}
void PutCmprsImgClipingBlendRGB565	( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD RGB )
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		mov  	 ax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		
		///////////////////
		movzx	eax,word ptr[edi];
		ror		eax,11;
		shl		ax,10;
		rol		eax,6;
		shl		ax,5;
		rol		eax,5;
		
		and     eax, RGB;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
		//////////////////
		
		
		
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}

void PutCmprsImgNOTClipingBlendRGB555(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		//		mov		ebx,RGB;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		xor		eax,eax;
		mov		ax,[esi];
		ror		eax,10;
		shl		ax,11;
		rol		eax,5;
		shl		ax,6;
		rol		eax,5;
		
		and     eax, RGB;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}
void PutCmprsImgClipingBlendRGB555( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD RGB )
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		
		///////////////////
		movzx	eax,word ptr[esi];
		ror		eax,10;
		shl		ax,11;
		rol		eax,5;
		shl		ax,6;
		rol		eax,5;
		
		and     eax, RGB;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
		//////////////////
		
		
		mov		[edi],bx;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}



void PutCompressedImageBlendRGB( int x, int y, Spr *sp, DWORD RGB )
{
#ifdef _SDL
	SDL_Lock();
#endif
	int	lx, rx, ty, by , IsC=0;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	
	x -= sp->ox;
	y -= sp->oy;
	
	if( x >= SCREEN_WIDTH  )			return; 
	else if( x + xl < 0 )	return; 
	if( y >= SCREEN_HEIGHT )			return; 
	else if( y + yl < 0  )	return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	if( x + xl >= SCREEN_WIDTH )
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if( x < 0 )
	{
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}
	if( y + yl >= SCREEN_HEIGHT )
	{
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}	
	if(	y < 0 )
	{	
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}		
	
	if( _PixelInfo.BitMaskR == 0x7C00)
	{
		if(IsC)	PutCmprsImgClipingBlendRGB555(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, RGB );
		else 	PutCmprsImgNOTClipingBlendRGB555( x, y, yl, sp->img, g_DestBackBuf, RGB);
	}
	else
	{
		if(IsC)	PutCmprsImgClipingBlendRGB565(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, RGB );
		else 	PutCmprsImgNOTClipingBlendRGB565( x, y, yl, sp->img, g_DestBackBuf, RGB);
	}
#ifdef _SDL
	SDL_UnLock();
#endif
}		









//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void PutCmprsImgNOTClipingCharRGB565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		//		mov		ebx,RGB;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		
		
		movzx	edx,word ptr [esi];
		and     edx, 0x1f;
		//		ror		eax,11;
		//		shl		ax,10;
		//		rol		eax,6;
		//		shl		ax,5;
		//		rol		eax,5;
		
		mov		eax, RGB;
		imul    eax, edx;
		shr		eax,5;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}
void PutCmprsImgClipingCharRGB565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD RGB )
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		
		///////////////////
		movzx	eax,word ptr[esi];
		and     eax, 0x1f;
		
		mov		edx, RGB;
		imul    eax, edx;
		shr		eax, 5;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
		//////////////////
		
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingCharRGB555(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		//		mov		ebx,RGB;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		
		
		movzx	edx,word ptr [esi];
		and     edx, 0x1f;
		//		ror		eax,11;
		//		shl		ax,10;
		//		rol		eax,6;
		//		shl		ax,5;
		//		rol		eax,5;
		
		mov		eax, RGB;
		imul    eax, edx;
		shr		eax,5;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}
void PutCmprsImgClipingCharRGB555( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD RGB )
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		
		///////////////////
		movzx	eax,word ptr[esi];
		and     eax, 0x1f;
		
		mov		edx, RGB;
		imul    eax, edx;
		shr		eax, 5;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
		//////////////////
		
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}


void PutCompressedImageCharRGB( int x, int y, Spr *sp, DWORD RGB )
{
#ifdef _SDL
	SDL_Lock();
#endif
	int	lx, rx, ty, by , IsC=0;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	x -= sp->ox;
	y -= sp->oy;
	
	if( x >= SCREEN_WIDTH  )			return; 
	else if( x + xl < 0 )	return; 
	if( y >= SCREEN_HEIGHT )			return; 
	else if( y + yl < 0  )	return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	if( x + xl >= SCREEN_WIDTH )
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if( x < 0 )
	{	
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}	
	if( y + yl >= SCREEN_HEIGHT )
	{	
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}	
	if(	y < 0 )
	{	
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}		
	
	if( _PixelInfo.BitMaskR == 0x7C00)
	{
		if(IsC)		PutCmprsImgClipingCharRGB555(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, RGB );
		else 		PutCmprsImgNOTClipingCharRGB555( x, y, yl, sp->img, g_DestBackBuf, RGB);
	}
	else
	{
		if(IsC)		PutCmprsImgClipingCharRGB565(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, RGB );
		else 		PutCmprsImgNOTClipingCharRGB565( x, y, yl, sp->img, g_DestBackBuf, RGB);
	}
#ifdef _SDL
	SDL_UnLock();
#endif
}		
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@   RGB Diferent...

void PutCmprsImgNOTClipingCharRGBDifer565(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		//		mov		ebx,RGB;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		
								
		movzx	edx,word ptr [esi];
		and     edx, 0x1f;
		//		ror		eax,11;
		//		shl		ax,10;
		//		rol		eax,6;
		//		shl		ax,5;
		//		rol		eax,5;
		
		mov		eax, RGB;
		imul    eax, edx;
		shr		eax,5;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:		
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:		
		pop		es;
	};			
}
void PutCmprsImgClipingCharRGBDifer565( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD RGB )
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		
		///////////////////
		movzx	eax,word ptr[esi];
		and     eax, 0x1f;
		
		mov		edx, RGB;
		imul    eax, edx;
		shr		eax, 5;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,5;
		ror		eax,6;
		shr		ax,10;
		rol		eax,11;
		
		mov		[edi],ax;
		
		//////////////////
		
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}
void PutCmprsImgNOTClipingCharRGBDifer555(int x, int y, int yl, LPVOID Stmp, LPVOID dest, DWORD RGB )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * dDxSize + (x * 2 ));
	
	j = yl;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		esi,Stmp;
		
		//		mov		ebx,RGB;
		
		inc		j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		dec		j
			jz		exit_1;  //if(SY==0)
		mov		edi,Dtmp;
		
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		xor		eax,eax;
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,1;
		
loop_alpha:
		
		
		movzx	edx,word ptr [esi];
		and     edx, 0x1f;
		//		ror		eax,11;
		//		shl		ax,10;
		//		rol		eax,6;
		//		shl		ax,5;
		//		rol		eax,5;
		
		mov		eax, RGB;
		imul    eax, edx;
		shr		eax,5;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		mov		eax,Dtmp;
		add		eax,dDxSize;
		mov		Dtmp,eax;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}
void PutCmprsImgClipingCharRGBDifer555( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by, DWORD RGB )
{
	int		l;
	LPVOID	Dtmp = (LPVOID)((char *)dest + y * wDxSize + (x * 2));
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}	
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		add		edx,eax;	//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		or		ecx,ecx;
		jz		jmp_kkk;
		
		shr		ecx,1;		//move DATA
		push	edx;
loop_alpha:
		
		///////////////////
		movzx	eax,word ptr[esi];
		and     eax, 0x1f;
		
		mov		edx, RGB;
		imul    eax, edx;
		shr		eax, 5;
		
		and		eax,0x03e0fc1f;
		ror		eax,5;
		shr		ax,6;
		ror		eax,5;
		shr		ax,11;
		rol		eax,10;
		
		mov		[edi],ax;
		
		//////////////////
		
		
		add		edi,2;
		add		esi,2;
		loop	loop_alpha;
		pop		edx;
jmp_kkk:
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		movzx	eax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		dword ptr by;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}


void PutCompressedImageCharRGBDifer( int x, int y, Spr *sp, DWORD RGB )
{
#ifdef _SDL
	SDL_Lock();
#endif
	int	lx, rx, ty, by , IsC=0;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	x -= sp->ox;
	y -= sp->oy;
	
	if( x >= SCREEN_WIDTH  )			return; 
	else if( x + xl < 0 )	return; 
	if( y >= SCREEN_HEIGHT )			return; 
	else if( y + yl < 0  )	return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	if( x + xl >= SCREEN_WIDTH )
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if( x < 0 )
	{	
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}	
	if( y + yl >= SCREEN_HEIGHT )
	{	
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}	
	if(	y < 0 )
	{	
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}		
	
	if( _PixelInfo.BitMaskR == 0x7C00)
	{
		if(IsC)		PutCmprsImgClipingCharRGBDifer555(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, RGB );
		else 		PutCmprsImgNOTClipingCharRGBDifer555( x, y, yl, sp->img, g_DestBackBuf, RGB);
	}
	else
	{
		if(IsC)		PutCmprsImgClipingCharRGBDifer565(x, y, sp->img, g_DestBackBuf, lx, rx, ty, by, RGB );
		else 		PutCmprsImgNOTClipingCharRGBDifer565( x, y, yl, sp->img, g_DestBackBuf, RGB);
	}
#ifdef _SDL
	SDL_UnLock();
#endif
}		
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

int tilewavetable[ 400] = { 2, 2, 2, 2, 4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 8, 8, 8,10,10,10,
10,10, 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4, 2, 2, 2, 2, 
2, 2, 2, 4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 8, 8, 8,10,10,10,10,
10, 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4, 2, 2, 2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
2,2,2,2,4,4,4,4,6,6,6,6,8,8,8,8,8,10,10,10,10,10,8,8,8,8,8,6,6,6,6,6,4,4,4,4,2,2,2,
};

void	PutCmprsImgClipingWaveTile(int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by)
{
	int		l;
	LPVOID Dtmp = (LPVOID)((char*)dest + y * dDxSize + (x * 2));
	int ttt;
	
	ttt = WaveTileStart;
	
	_asm{
		mov		ecx,rx;
		shl		ecx,1;
		mov		rx,ecx;
		
		mov		ecx,lx;
		shl		ecx,1;
		mov		lx,ecx;
	}
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		xor		eax,eax;
		
		mov		esi,Stmp;
		
		mov		ecx,ty;
		or		ecx,ecx;
		jz		exit_1;
loop_1:
		
		cmp		word ptr[esi],0xffff;
		jz		exit_xxxx;
		mov		ax,[esi+2]
			add		esi,4;
		add		esi,eax;
		jmp		loop_1;
exit_xxxx:
		add		esi,2;
		loop	loop_1;
exit_1:
	}
	_asm{
		mov		ebx, by;		//
		or		ebx,ebx;
		jz		exit_for1;
loop_for1:
		
		xor		edx,edx;
		mov		edi,Dtmp;
		test	lx,0xffff;			//if(lx)
		jz		exit_if1;
		
		inc     ttt;
		push    edx
			mov		edx, ttt
			shl		edx, 2
			add     edi, dword ptr[ offset tilewavetable + edx ];
		pop		edx
			
loop_while1:
		cmp		word ptr[esi],0xffff;	//while(*stmp != 0xffff)
		jz		exit_while1;			//
		add		dx,[esi];
		cmp		edx,lx;		//if(j>lx)
		jle		jmp_if1;			//
		sub		edx,lx;		//j-=lx
		add		edi,edx;			//dtmp+=j
		
		//			inc     ttt;
		//			push    edx
		//			mov		edx, ttt
		//			shl		edx, 2
		//			add     edi, dword ptr[ offset tilewavetable + edx ];
		//			pop		edx
		
		jmp		jmp_0416;//****************************************************
		
jmp_if1:///////////////////////////////////////
		mov		ax,[esi + 2];
		add		esi,4;
		add		edx,eax;
		add		esi,eax;	//
		cmp		edx,lx;		//
		jle		loop_while1;
		sub		edx,lx;
		sub		esi,edx;
		mov		eax,edx;
		xor		edx,edx;
		jmp		jmp_0429;		//4.29
exit_while1:	//end while(*stmp != 0xffff)*/
exit_if1:	//end if(lx)
		
loop_while2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_while2;
		
		movzx	eax,word ptr[esi];
		add		edi,eax;	//dtmp+=*stmp
		
		
		inc     ttt;
		push    edx
			mov		edx, ttt
			shl		edx, 2
			add     edi, dword ptr[ offset tilewavetable + edx ];
		pop		edx
			
			
			add		dx,ax;		//j+=*stmp
jmp_0416:
		mov		ax,[esi + 2];
		add		esi,4;
jmp_0429:
		mov		ecx,eax;		//l=k=*stmp
		mov		l,eax;
		
		add		eax,edx;
		cmp		eax, rx;
		jle		exit_if0;
		cmp		edx, rx;
		jg		exit_if0;
		mov		ecx, rx;
		sub		ecx,edx;
exit_if0:
		cmp		edx, rx;
		jg		else_if;
		
		add		edx, l;			//j+=l;
		sub		l,ecx;			//l-k;
		
		shr		ecx,2;		//move DATA
		jnc		Next2_1;	//
		movsw;				//
Next2_1:				//
		rep		movsd;		//
		
		add		esi,l;
		jmp		loop_while2;
		
else_if:
		add		esi,l;
ext_loop:
		cmp		word ptr[esi],0xffff;
		jz		ext;
		mov		ax,[esi+2];
		add		esi,4;
		add		esi,eax;
		jmp		ext_loop;
ext:
		
exit_while2:
		mov		eax, dDxSize;
		add		esi,2;
		add		dword ptr Dtmp,eax;
		dec		ebx;	//for(i=0;i<by;i++)
		jnz		loop_for1;	//
exit_for1:
		
		pop		es;
	};
}




void	PutCmprsImgNOTClipingWaveTile(int x, int y, int yl, LPVOID Stmp, LPVOID dest )
{
	DWORD	j;
	LPVOID	Dtmp = (LPVOID)( (char*)dest + y * dDxSize + (x * 2 ));
	int		ttt;
	j = yl;
	
	ttt = WaveTileStart;
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov		edx,Dtmp;
		mov		esi,Stmp;
		
		mov		ebx,j;
		
		xor		eax,eax;
		xor		ecx,ecx;
loop_1:
		or		bx,bx;		// if(SY==0)
		jz		exit_1;
		mov		edi,edx;
		
		inc     ttt;
		push    edx
			mov		edx, ttt
			shl		edx, 2
			add     edi, dword ptr[ offset tilewavetable + edx ];
		pop		edx
			
loop_2:	
		cmp		word ptr[esi],0xffff;
		jz		exit_2;
		
		mov		ax,[esi];
		add		edi,eax;	//0 skip
		
		mov		cx,[esi+2];	//load data num
		add		esi,4;
		
		shr		ecx,2;		//move DATA
		jnc		Next2;		//
		movsw;				//
Next2:					//
		rep		movsd;		//
		
		jmp		loop_2;
exit_2:
		add		esi,2;
		
		add		edx,dDxSize;
		dec		bx;
		jmp		loop_1;
exit_1:
		pop		es;
	};
}	
void PutCompressedImageWaveTile(int x, int y, Spr *sp )
{		
#ifdef _SDL
	SDL_Lock();
#endif
	int		lx, rx, ty, by , IsC=0;
	
	if( sp == NULL ) return;
	if( sp->img == NULL ) return;
	
	
	int xl = sp->xl;
	int yl = sp->yl;
	
	if( xl == 0 ) return;
	if( yl == 0 ) return;
	
	x -= sp->ox;
	y -= sp->oy;
	
	
	if( x >= GAME_SCREEN_XSIZE  ) return; 
	else if( x + xl < 0 ) return; 
	
	if( y >= GAME_SCREEN_YSIZE ) return; 
	else if( y + yl < 0  ) return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	
	
	if( x + xl >= GAME_SCREEN_XSIZE )
	{
		rx -= x + xl - GAME_SCREEN_XSIZE;
		IsC = 1;
	}
	if( x < 0 )
	{
		lx  = 0 - x;
		x   = 0;
		rx -= lx;
		IsC = 1;
	}
	
	if( y + yl >= GAME_SCREEN_YSIZE )
	{
		by -= y + yl - GAME_SCREEN_YSIZE;
		IsC = 1;
	}
	if( y < 0 )
	{
		ty  = 0 - y;
		y   = 0;
		by -= ty;
		IsC = 1;
	}	
	
	
	if( IsC )
	{	
		PutCmprsImgClipingWaveTile( x,  y, sp->img, g_DestBackBuf, lx, rx+1, ty, by);
	}	
	else
	{	
		PutCmprsImgNOTClipingWaveTile( x,  y, sp->yl, sp->img, g_DestBackBuf );
	}	
#ifdef _SDL
	SDL_UnLock();
#endif
}		


























//#####################################################################################
//#####################################################################################
//#####################################################################################

//  Character Sprite  Loading....

//#####################################################################################
//#####################################################################################
//#####################################################################################



int LoadCharSpriteData_Sub( char *filename, char **buf, DWORD *length, CharacterSprite *charactersprite, int mode )
{				
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr  *s;
	
	fp = Fopen( filename, "rb" );
	if( fp == NULL ) return 0;
	
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR );  // 오프셋 스킵
	*length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	/***********ROGERIO BLOQUEIA DRAGAO****************/	
	if (filename[7] == '1' && filename[8] == '2' && filename[9] == '0' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0'){
		if (sprfilelength != 1857214){
			JustMsg( lan->OutputMessage(6,51),filename, i+1, j );
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}

	if (filename[7] == '1' && filename[8] == '2' && filename[9] == '1' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0'){
		if (sprfilelength != 4861144){
			JustMsg( lan->OutputMessage(6,51),filename, i+1, j );
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}

	if (filename[7] == '0' && filename[8] == '1' && filename[9] == '5' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0'){
		if (sprfilelength != 2279186){
			JustMsg( lan->OutputMessage(6,51),filename, i+1, j );
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}

	if (filename[7] == '0' && filename[8] == '4' && filename[9] == '8' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0'){
		if (sprfilelength != 1485521){
			JustMsg( lan->OutputMessage(6,51),filename, i+1, j );
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}
	
	if (filename[7] == '1' && filename[8] == '2' && filename[9] == '2' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0'){
		if (sprfilelength != 2211052){
			JustMsg( lan->OutputMessage(6,51),filename, i+1, j );
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}


	if (filename[7] == '1' && filename[8] == '2' && filename[9] == '3' && filename[10] == '0' && filename[11] == '0' && filename[12] == '0'){
		if (sprfilelength != 1773206){
			JustMsg( lan->OutputMessage(6,51),filename, i+1, j );
			ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			return 0;
		}
	}
	/**************************************************/

	if( *buf != NULL )  return 0;
	MemFree( *buf );		// 0201 YGI
	MemAlloc( *buf, sprfilelength );
	
	if( mode == LD_CHAR_SPRITE_ONLY_MEM_ALLOC )
	{				
		fclose(fp );
		return 0;
	}			
				
	//	return 0;//	
	for( i = 0 ; i < 8 ; i ++)
	{			
		for( j = 0 ; j < no/8 ; j ++)
		{				
			s = &charactersprite->sp[i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			//s->yl ++;		
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				//				JustMsg( " [%s]의 화일에 %d방향 %d번 \nsprite화일에 문제가 있는듯 합니다.\n\n Check하여주십시오.\n 종료합니다."
				JustMsg( lan->OutputMessage(6,51),
					filename, i+1, j );
				ExitApplication(EA_LOAD_CHAR_SPR_DATA_SUB);
			}						
			fread( *buf + nOff, s->size, 1, fp);
			s->img = *buf + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}
				
	fclose(fp);	
	return sprfilelength;	
}			





int LoadCharSpriteData( char *filename, char **buf, DWORD *length, CharacterSprite *charactersprite, int mode )
{
	int ret;
	if( StartLoadCharSpriteDataThreadFlag )
	{
		ret = 0;
		InputThreadCharSprite( filename, buf, length, charactersprite, mode );
	}
	else
	{
		ret = LoadCharSpriteData_Sub( filename, buf, length, charactersprite, mode );
	}
	return ret ;
}




void FreeCharacterSpriteData( char *buf )
{				
	if( buf == NULL ) return;
	MemFree( buf );
}			

void FreeCharacterSpriteDataAll( int s, int e )
{	
	for (int i = s; i < e; i++)
	{	
		MemFree(CharBuf[i]);
		CharBufCount[i] = 0;
		
		for (int j = 0; j < MAX_CHARACTOR_PART_; j++)
		{
			for (int k = 0; k < MAX_CHARACTER_LAYER_; k++)
			{
				MemFree(CharLayerBuf[i][j][k]);
				CharLayerBufCount[i][j][k] = 0;
			}
		}
	}	
} 

void LoadHeroClothAccessoryData( int s, int e )
{		
	int i;
	char tempfilename[ FILENAME_MAX];
	
	for( i = s ; i <= e ; i ++)
	{	
		sprintf( tempfilename, "./char/000%03d.spr", i );
		LoadCharSpriteData( tempfilename,  &CharAccessoryBuf[ 0][i], &CharAccessoryBufLength[ 0][i], &CharAccessorySpr[ 0][i], LD_CHAR_SPRITE_ALL_LOAD );
		sprintf( tempfilename, "./char/001%03d.spr", i );
		LoadCharSpriteData( tempfilename,  &CharAccessoryBuf[ 1][i], &CharAccessoryBufLength[ 1][i], &CharAccessorySpr[ 1][i], LD_CHAR_SPRITE_ALL_LOAD );
	}	
}		
void FreeHeroClothAccessoryDataOne( int sprno, int no )
{		
	if( no == 0 ) return;
	if( sprno > 1 ) return;
	if( CharAccessoryCount[sprno][ no] <= 0 ) return;
	
	CharAccessoryCount[sprno][ no] --;
	if( CharAccessoryCount[sprno][ no] <= 0 ) 
	{	
		if( CharAccessoryBuf[ sprno][no] == NULL ) return;
		GlobalFree( CharAccessoryBuf[ sprno][no] );
		CharAccessoryBuf[ sprno][no] = NULL;
	}	
}		
void LoadHeroClothAccessoryDataOne( int sprno, int no )
{		
	if( no == 0 ) return;
	if( sprno > 1 ) return;
	if( no >= CHARACTER_ACCESSORY_MAX_ ) return;	// 011018 KHS 망또	
	if( CharAccessoryBuf[ sprno][no] ) return;
	
	char tempfilename[ FILENAME_MAX];
	sprintf( tempfilename, "./char/%03d%03d.spr", sprno, no );
	
	int ret;
	
	ret = LoadCharSpriteData( tempfilename,  &CharAccessoryBuf[ sprno][no], &CharAccessoryBufLength[ sprno][no], &CharAccessorySpr[ sprno][no], LD_CHAR_SPRITE_ALL_LOAD );
}		
void FreeHeroClothAccessoryData( void )
{		
	int i;
	for( i = 1 ; i < 104 ; i ++)
	{	
		FreeCharacterSpriteData( CharAccessoryBuf[ 0][i] );
		CharAccessoryBuf[ 0][i] = NULL;
		CharAccessoryCount[0][i] = 0;
		FreeCharacterSpriteData( CharAccessoryBuf[ 1][i] );
		CharAccessoryBuf[ 1][i] = NULL;
		CharAccessoryCount[1][i] = 0;
	}	
}		

void FreeHeroClothAccessoryDataOne_tool( int sprno, int no )
{	//< CSD-030306	
	if( no == 0 ) return;
	if( sprno > 1 ) return;
	if( CharAccessoryCount[sprno][ no] <= 0 ) return;
		
	CharAccessoryCount[sprno][ no] --;
	if( CharAccessoryCount[sprno][ no] <= 0 ) 
	{	
		if( CharAccessoryBuf[ sprno][no] == NULL ) return;
		GlobalFree( CharAccessoryBuf[ sprno][no] );
		CharAccessoryBuf[ sprno][no] = NULL;
	}	
}	//> CSD-030306		

void LoadHeroClothAccessoryDataOne_tool( int sprno, int no )
{	//< CSD-030306	
	if( no == 0 ) return;
	if( sprno > 1 ) return;
	if( no >= CHARACTER_ACCESSORY_MAX_ ) return;	// 011018 KHS 망또	
	if( CharAccessoryBuf[ sprno][no] ) return;
		
	char tempfilename[ FILENAME_MAX];
	sprintf( tempfilename, "./char_test/%03d%03d.spr", sprno, no );

	int ret;

	ret = LoadCharSpriteData( tempfilename,  &CharAccessoryBuf[ sprno][no], &CharAccessoryBufLength[ sprno][no], &CharAccessorySpr[ sprno][no], LD_CHAR_SPRITE_ALL_LOAD );
}	//> CSD-030306

void LoadHeroSpriteData( void )
{			
	LoadCharSpriteData( "./char/000000.spr",  &CharBuf[0], &CharBufLength[0], &CharSpr[0], LD_CHAR_SPRITE_ALL_LOAD );
	LoadCharSpriteData( "./char/001000.spr",  &CharBuf[1], &CharBufLength[1], &CharSpr[1], LD_CHAR_SPRITE_ALL_LOAD );
	LoadHeroClothAccessoryData( 104, 107 );
}	
void FreeHeroSpriteData( void )
{		
	FreeCharacterSpriteData( CharBuf[0] );
	FreeCharacterSpriteData( CharBuf[1] );
}	









//--------------------------------- 날씨 Animation...............................
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------


#define MAX_RAIN 		900
#define MAX_RAINFRAME	1 

#define RAIN_SPRITE_		880

static int	rainsx[ MAX_RAIN], 
rainsy[ MAX_RAIN],
rainey[ MAX_RAIN],
//			rainframe[ MAX_RAIN],
//			rainfd[ MAX_RAIN],
rainspr[ MAX_RAIN],
raindownspeed[ MAX_RAIN],
raintrans[  MAX_RAIN],
raindrop[  MAX_RAIN],
rainmax;

static int  RainNumber;
static int	old_RainNumber; //Eleval 16/08/09
static bool DrawRain = true; //Eleval 16/08/09
static bool DrawSnow = true; //Eleval 16/08/09

void RainStart( int number, int flag )
{	
	if(DrawRain) RainNumber = number;
	old_RainNumber = number;
	/////////////////////  SoundUp lkh 추가 /////////////////////////
	int sound_Volume = number/9;
	DayLightControl = 30;
}	

void ThunderStart( int number, int flag )
{	
	if(DrawRain) RainNumber = number;
	old_RainNumber = number;
	/////////////////////  SoundUp lkh 추가 /////////////////////////
	int sound_Volume = number/9;
	g_IsThunderStorm = true;
	DayLightControl = 25;
}	

void RainStop( void )
{
	RainNumber = 0;
	old_RainNumber = RainNumber;
	////////////////////// SoundUp lkh 추가 /////////////////////////
	StopWave(SOUND_RAINY);
	Statue_Weather = 0;
	DayLightControl = 31;
}

void ThunderStop( void ) //eleval 12/08/09 - Thunderstorm
{
	RainNumber = 0;
	old_RainNumber = RainNumber;
	g_IsThunderStorm = false;
	StopWave(SOUND_RAINY);
	Statue_Weather = 0;
	last_thunder_time = -1;
	thunder_wait_time = 0;
	bLowRain = false;
	DayLightControl = 31;
}

void ToggleRainHide(bool flag)
{
	DrawRain = !flag;

	if(flag)
		RainNumber = 0;
	else
		RainNumber = old_RainNumber;
}

void RainAnimationSub( int i )
{
	int tt = 3;
	if		( rainey[i] < 200 )	{ raindownspeed[i] = 15 + Random( 5 );	rainspr[i] = Random( 3 ) + 6;	raintrans[i] = tt+ Random(3); }
	else if	( rainey[i] < 500 )	
	{
		switch( Random(3) )
		{
		case 0 :			{ raindownspeed[i] = 20 + Random( 4 );  rainspr[i] = Random( 3 ) + 4;	raintrans[i] = tt+ Random(3);}
			break;
		case 1 :			{ raindownspeed[i] = 22 + Random( 4 );  rainspr[i] = Random( 3 ) + 6;	raintrans[i] = tt+ Random(4);}
			break;
		default :			{ raindownspeed[i] = 27 + Random( 4 );  rainspr[i] = Random( 3 ) + 5;	raintrans[i] = tt+ Random(4);}
			break;
		}
	}	
	else						{ raindownspeed[i] = 30 + Random( 4 );  rainspr[i] = Random( 2 ) + 2;	raintrans[i] = tt+ Random(3);}
}		

void RainAnimationSetting( void )
{		
	static int start;
	int i;
	int dist;
	
	if( RainNumber == 0 && rainmax == 0 ) 
	{				
		return;
	}	
	else if (RainNumber != rainmax ) 
	{	
		if( RainNumber > rainmax )
		{	
			rainmax += 10;
			if( rainmax > 900 ) rainmax =  900;
		}
		else 
		{
			rainmax -= 10;
		}
	}	
	
	int mapstartx = g_Map.x * 32;
	int mapstarty = g_Map.y * 32;
	
	switch( start )	
	{					
	case 0 :	rainmax = 0;
		for( i = 0 ; i < MAX_RAIN ; i ++ )
		{	
			rainsx[i] = Random( SCREEN_WIDTH  + 200) - 100;
			rainsy[i] = Random( SCREEN_HEIGHT) - SCREEN_HEIGHT;
			rainey[i] = Random( 680);
			RainAnimationSub( i );
		}	
		start = 1;
		break;	
	}				
	
	for( i  = 0 ; i < rainmax ; i ++)
	{				
		if( raindrop[i] == 0 )
		{
			rainsx[ i] += Random( 3)+2;
			rainsy[ i] += raindownspeed[i];
		}
		
		dist    = 5;
		
		//		switch( ch->direction  )
		//		{
		//			case DIRECTION_DOWN		: rainsx[i] -= dist;	rainsy[i] -= dist;		break;
		//			case DIRECTION_LEFTDOWN	:            			rainsy[i] -= dist;		break;
		//			case DIRECTION_LEFT		: rainsx[i] += dist;	rainsy[i] -= dist;		break;
		//			case DIRECTION_LEFTUP	: rainsx[i] += dist;			           		break;
		//			case DIRECTION_UP		: rainsx[i] += dist;	rainsy[i] += dist;		break;
		//			case DIRECTION_RIGHTUP	:            			rainsy[i] += dist;		break;
		//			case DIRECTION_RIGHT	: rainsx[i] -= dist;	rainsy[i] += dist;		break;
		//			case DIRECTION_RIGHTDOWN: rainsx[i] -= dist;				       		break;
		//		}
		
		
		if( raindrop[i] == 0 )
		{
			if( rainsy[i]> rainey[i] )
			{
				raindrop[i] = 2;
			}
		}
		else
		{
			raindrop[i]--;
			if( raindrop[i] == 0 )
			{
				rainsx[i]	= Random( SCREEN_WIDTH  + 200 ) - 100;
				rainsy[i]	= Random( 200 ) - SCREEN_HEIGHT;
				rainey[i]	= Random( 680 );
				RainAnimationSub( i );
			}
		}
	}	
}		

void RainAnimationOutput( void )
{		
	int i;
	
	for( i  = 0 ; i < rainmax ; i ++)
	{			
		if( raindrop[i] ) 
		{		
			switch( raindrop[i] )
			{
			case 1 :	PutCompressedImageFX( rainsx[i], rainsy[i], &spr[ 87 ],  raintrans[i], 2 ); break;
			case 2 : 	PutCompressedImageFX( rainsx[i], rainsy[i], &spr[ 86 ],  raintrans[i], 2 ); break;
			}
			//			PutCompressedImageFX( rainsx[i], rainsy[i], &spr[ RAIN_SPRITE_  ], 14,1 );
			//			PutCompressedImage( rainsx[i], rainsy[i], &spr[ RAIN_SPRITE_  ]);
		}	
		else	
		{	
			if( rainspr[i] )
			{
				//PutCompressedImageTrans(int x, int y, Spr *sp, WORD MSK)
				//PutCompressedImageTrans( rainsx[i], rainsy[i], &spr[ RAIN_SPRITE_ + rainspr[i]], 0 );
				PutCompressedImageFX( rainsx[i], rainsy[i], &spr[ RAIN_SPRITE_ + rainspr[i]], raintrans[i], 2 );
			}
			else
			{
				PutCompressedImageFX( rainsx[i], rainsy[i], &spr[ RAIN_SPRITE_ + rainspr[i]] , raintrans[i], 2 );
			}
		}	
	}			
}	



/*

  
	#define MAX_SNOW  		800
	#define MAX_SNOWFRAME	1 
	
	  #define SNOW_SPRITE_		79
	  
		static int	snowsx[ MAX_SNOW], 
		snowsy[ MAX_SNOW],
		snowey[ MAX_SNOW],
		snowframe[ MAX_SNOW],
		snowfd[ MAX_SNOW],
		snowspr[ MAX_SNOW],
		snowdownspeed[ MAX_SNOW],
		snowtrans[  MAX_SNOW],
		snowflag[  MAX_SNOW],
		snowwind[  MAX_SNOW],
		snowmax;
		
		  
			static int  SnowNumber;
			
			  void SnowStart( int number, int flag )
			  {	
			  SnowNumber = number;
			  }	
			  
				void SnowStop( void )
				{	
				SnowNumber = 0;
				}	
				
				  
					void SnowAnimationSub( int my, int i )
					{			
					int tt = 5;
					
					  if		( snowey[i] - my  < 100 )	{ snowdownspeed[i] =  1 + Random( 5 );  snowspr[i] = Random( 2 ) + 5;	snowtrans[i] = tt+ Random(4);}
					  else if	( snowey[i] - my  < 150 )	{ snowdownspeed[i] =  2 + Random( 4 );  snowspr[i] = Random( 2 ) + 5;	snowtrans[i] = tt+ Random(4);}
					  else if	( snowey[i] - my  < 200 )	{ snowdownspeed[i] =  3 + Random( 4 );  snowspr[i] = Random( 2 ) + 5;	snowtrans[i] = tt+ Random(4);}
					  else if	( snowey[i] - my  < 250 )	{ snowdownspeed[i] =  4 + Random( 4 );  snowspr[i] = Random( 2 ) + 4;	snowtrans[i] = tt+ Random(4);}
					  else if	( snowey[i] - my  < 300 )	{ snowdownspeed[i] =  5 + Random( 4 );  snowspr[i] = Random( 2 ) + 4;	snowtrans[i] = tt+ Random(4);}
					  else 	{ snowdownspeed[i] = 6 + Random( 4 );  snowspr[i] = Random(2) + 5 ;				snowtrans[i] = tt+ Random(4);}
					  }			
					  
						void SnowAnimationSetting( void )
						{				
						static int start;
						int i;		
						int dist;	
						int curhero;
						static int snowwindflag;
						
						  ch		ch = ( ch )g_OrderInfo.order[ 0 ].lpvData;
						  
							if( SnowNumber == 0 && snowmax == 0 ) 
							{				
							return;
							}
							else if( SnowNumber >= snowmax )
							{
							snowmax += 10;
							}
							else 
							{
							snowmax -= 10;
							}
							
							  int mapstartx = g_Map.x * 32;
							  int mapstarty = g_Map.y * 32;
							  
								switch( start )	
								{				
								case 0 :	snowmax = 0;
								for( i = 0 ; i < MAX_SNOW ; i ++ )
								{	
								snowsx[i] = mapstartx + Random( SCREEN_WIDTH  + 200) - 100;
								snowsy[i] = mapstarty + Random( 100) - 100;
								snowey[i] = mapstarty + Random( SCREEN_HEIGHT);
								
								  SnowAnimationSub( mapstarty, i );
								  }	
								  start = 1;
								  break;		
								  }				
								  
									if( Random( 100 ) == 50 )
									{			
									snowwindflag = 40;
									}			
									
									  for( i  = 0	; i < snowmax ; i ++)
									  {				
									  if( snowwindflag )
									  {		
									  snowwindflag --;
									  if( snowwindflag == 0 )
									  {	
									  snowwind[i] = Random(30) + 30;
									  }	
									  }		
									  
										if( snowsy[i] < snowey[i] )
										{		
										if( snowwind[i] ) 
										{	
										snowwind[i] --;
										snowsx[i] -= ( Random( 10 ) + 20 );
										}	
										else 
										snowsx[ i] += ( Random( 5) -2 );
										
										  snowsy[ i] += snowdownspeed[i];
										  }		
										  else if( snowflag[i] == 0 )
										  {	
										  snowflag[i] = 1;
										  snowtrans[i] += 2;
										  if( snowtrans[i] > 15 ) snowtrans[i] = 15;
										  }		
										  else 
										  {	
										  snowtrans[i] --;
										  
											if( snowtrans[i] < 0 )
											{
											snowsx[i]	= mapstartx + Random( SCREEN_WIDTH  + 800 ) - 400;
											snowsy[i]	= mapstarty + Random( 200 ) - 200;
											snowey[i]	= mapstarty + Random( 680 );
											snowflag[i] = 0;
											snowwind[i] = 0;
											
											  SnowAnimationSub( mapstarty, i );
											  }
											  }	
											  }		
											  }			
											  
												void SnowAnimationOutput( void )
												{	
												int i;	
												
												  int mapstartx = g_Map.x * 32;
												  int mapstarty = g_Map.y * 32;
												  
													for( i  = 0 ; i < snowmax ; i ++)
													{
													PutCompressedImageFX( snowsx[i] - mapstartx, snowsy[i] - mapstarty, &spr[ SNOW_SPRITE_+ snowspr[i] ], snowtrans[i], 2 );
													}
													}				
													//-----------------------------------------------------------------------
*/

//----------------------------------------------------------------------



#define MAX_SNOW  		800
#define MAX_SNOWFRAME	1 

#define SNOW_SPRITE_		889

static int	snowsx[ MAX_SNOW],		
snowsy[ MAX_SNOW],		
snowey[ MAX_SNOW],		
snowframe[ MAX_SNOW],	
snowfd[ MAX_SNOW],		
snowspr[ MAX_SNOW],		
snowdownspeed[ MAX_SNOW],
snowtrans[  MAX_SNOW],
snowflag[  MAX_SNOW],
snowwind[  MAX_SNOW],
snowmax;


static int  SnowNumber;
static int old_SnowNumber; //Eleval 16/08/09

void SnowStart( int no, int flag )
{	
	SnowNumber = 0; 
	
	if( strcmp( MapName, "FIREDUN1" ) == 0 ) return;
	if( strcmp( MapName, "ICE-W01" ) == 0 ) return;
	if( strcmp( MapName, "MANDUN" ) == 0 ) return;
	
	if(DrawSnow)SnowNumber = no; 
	old_SnowNumber = no;
	Statue_Weather = 2;
}	

void ToggleSnowHide(bool flag)
{
	DrawSnow = !flag;

	if(flag)
		SnowNumber = 0;
	else
		SnowNumber = old_SnowNumber;
}

void SnowStop( void )
{		
	SnowNumber = 0; 
	old_SnowNumber = SnowNumber;
	Statue_Weather = 0;
}				


void SnowAnimationSub( int my, int i )
{		
	int tt = 5;
	
	if		( snowey[i] - my  < 100 )	{ snowdownspeed[i] =  1 + Random( 5 );  snowspr[i] = Random( 2 ) + 5;	snowtrans[i] = tt+ Random(4);}
	else if	( snowey[i] - my  < 150 )	{ snowdownspeed[i] =  2 + Random( 4 );  snowspr[i] = Random( 2 ) + 5;	snowtrans[i] = tt+ Random(4);}
	else if	( snowey[i] - my  < 200 )	{ snowdownspeed[i] =  3 + Random( 4 );  snowspr[i] = Random( 2 ) + 5;	snowtrans[i] = tt+ Random(4);}
	else if	( snowey[i] - my  < 250 )	{ snowdownspeed[i] =  4 + Random( 4 );  snowspr[i] = Random( 2 ) + 4;	snowtrans[i] = tt+ Random(4);}
	else if	( snowey[i] - my  < 300 )	{ snowdownspeed[i] =  5 + Random( 4 );  snowspr[i] = Random( 2 ) + 4;	snowtrans[i] = tt+ Random(4);}
	else 	{ snowdownspeed[i] = 6 + Random( 4 );  snowspr[i] = Random(2) + 5 ;				snowtrans[i] = tt+ Random(4);}
}		

void SnowAnimationSetting( void )
{				
	static int start;
	int i;		
	static int windflag;
				
	if( SnowNumber == 0 && snowmax == 0 ) 
	{	
		return;
	}	
	else if ( SnowNumber != snowmax )
	{		
		if( SnowNumber > snowmax )
		{
			snowmax += 10;
			if( snowmax > 900 ) snowmax = 900;
		}
		else 
		{
			snowmax -= 10;
			if( snowmax < 0 ) snowmax = 0;
		}
	}	
	
	int mapstartx = g_Map.x * 32;
	int mapstarty = g_Map.y * 32;
	
	switch( start )	
	{					
	case 0 :	snowmax = 0;
		for( i = 0 ; i < MAX_SNOW ; i ++ )
		{	
			snowsx[i] = mapstartx + Random( SCREEN_WIDTH  + 200) - 100;
			snowsy[i] = mapstarty + Random( 100) - 100;
			snowey[i] = mapstarty + Random( SCREEN_HEIGHT);
			
			SnowAnimationSub( mapstarty, i );
			
		}	
		start = 1;
		break;	
	}			
	
	//	if( SnowFlag ) snowmax ++ ;
	//	if( snowmax > MAX_SNOW ) 
	//	snowmax = MAX_SNOW;
	
	
	if( Random( 100 ) == 50 )
	{	
		windflag = 40;
	}	
	
	for( i  = 0 ; i < snowmax ; i ++)
	{			
		if( windflag )
		{	
			windflag --;
			if( windflag == 0 )
			{
				snowwind[i] = Random(30) + 30;
			}
		}	
		
		if( snowsy[i] < snowey[i] )
		{	
			if( snowwind[i] ) 
			{
				snowwind[i] --;
				snowsx[i] -= ( Random( 10 ) + 20 );
			}
			else 
				snowsx[ i] += ( Random( 5) -2 );
			
			snowsy[ i] += snowdownspeed[i];
		}	
		else if( snowflag[i] == 0 )
		{	
			snowflag[i] = 1;
			snowtrans[i] += 2;
			if( snowtrans[i] > 15 ) snowtrans[i] = 15;
		}		
		else 
		{	
			snowtrans[i] --;
			
			if( snowtrans[i] < 0 )
			{
				snowsx[i]	= mapstartx + Random( SCREEN_WIDTH  + 800 ) - 400;
				snowsy[i]	= mapstarty + Random( 200 ) - 200;
				snowey[i]	= mapstarty + Random( 680 );
				snowflag[i] = 0;
				snowwind[i] = 0;
				
				SnowAnimationSub( mapstarty, i );
			}
		}	
	}		
}			

void SnowAnimationOutput( void )
{	
	int i;	
	
	int mapstartx = g_Map.x * 32;
	int mapstarty = g_Map.y * 32;
	
	for( i  = 0 ; i < snowmax ; i ++)
	{
		PutCompressedImageFX( snowsx[i] - mapstartx, snowsy[i] - mapstarty, &spr[ SNOW_SPRITE_+ snowspr[i] ], snowtrans[i], 2 );
	}
}			

/*		mov		ax,[esi];
cmp		eax,0;
jz		colorkey;
shl		eax,5;
shr		ax,2;
shr		al,3;

		//ror		eax,11;			//
		//shl		ax,10;			//
		//rol		eax,6;			//
		//shl		ax,5;			
		//rol		eax,5;			
		
		  or		eax,0x00808080;
		  sub		eax,mask;
		  xor		eax,0x00808080;
		  
			movsx	edx,al;
			mov		bx,word ptr[offset SubTableB + 64 + edx*2];	//
			movsx	edx,ah;
			or		bx,word ptr[offset SubTableG + 128 + edx*2];
			shr		eax,16;
			movsx	edx,al;
			or		bx,word ptr[offset SubTableR + 64 + edx*2];	//
*/	



//-------------------------------------------------------------------------	



#define MAX_WIND		300
#define MAX_WINDFRAME	1 

#define WIND_SPRITE_		79

static int	windsx[ MAX_WIND], 
windsy[ MAX_SNOW],
windey[ MAX_WIND],
windframe[ MAX_WIND],
windfd[ MAX_WIND],
windspr[ MAX_WIND],
winddownspeed[ MAX_WIND],
windtrans[  MAX_WIND],
snowwindflag[  MAX_WIND],
windwind[  MAX_WIND],
windflag[  MAX_WIND],
windmax;

static int  WindNumber;

void WindStart( int number )
{	
	WindNumber = number;
}	

void WindStop( void )
{	
	WindNumber = 0;
}	


void WindAnimationSub( int my, int i )
{	
	int tt = 5;
	
	if		( windey[i] - my  < 100 )	{ winddownspeed[i] =  1 + Random( 5 );  windspr[i] = Random( 2 ) + 5;	windtrans[i] = tt+ Random(4);}
	else if	( windey[i] - my  < 150 )	{ winddownspeed[i] =  2 + Random( 4 );  windspr[i] = Random( 2 ) + 5;	windtrans[i] = tt+ Random(4);}
	else if	( windey[i] - my  < 200 )	{ winddownspeed[i] =  3 + Random( 4 );  windspr[i] = Random( 2 ) + 5;	windtrans[i] = tt+ Random(4);}
	else if	( windey[i] - my  < 250 )	{ winddownspeed[i] =  4 + Random( 4 );  windspr[i] = Random( 2 ) + 4;	windtrans[i] = tt+ Random(4);}
	else if	( windey[i] - my  < 300 )	{ winddownspeed[i] =  5 + Random( 4 );  windspr[i] = Random( 2 ) + 4;	windtrans[i] = tt+ Random(4);}
	else 	{ winddownspeed[i] = 6 + Random( 4 );  windspr[i] = Random(2) + 5 ;				windtrans[i] = tt+ Random(4);}
}									

void WindAnimationSetting( void )
{							
	static int start;		
	int i;					
	static int Windflag;	
	
	LPCHARACTER ch = ( LPCHARACTER  )g_OrderInfo.order[ 0 ].lpvData;
	
	if( RainNumber == 0 && rainmax == 0 ) 
	{						
		return;				
	}						
	if( RainNumber >= rainmax )
	{						
		rainmax += 10;		
	}						
	else					
	{						
		rainmax += 10;		
	}						
	
	int mapstartx = g_Map.x * 32;
	int mapstarty = g_Map.y * 32;
	
	switch( start )			
	{						
	case 0 :	windmax = 0;
		for( i = 0 ; i < MAX_WIND ; i ++ )
		{			
			windsx[i] = mapstartx + Random( SCREEN_WIDTH  + 200) - 100;
			windsy[i] = mapstarty + Random( 100) - 100;
			windey[i] = mapstarty + Random( SCREEN_HEIGHT);
			
			WindAnimationSub( mapstarty, i );
			
		}			
		start = 1;	
		break;
	}
	
	//	if( WindFlag ) windmax ++ ;
	//	if( windmax > MAX_WIND ) 
	windmax = MAX_WIND;
	
	
	if( Random( 100 ) == 50 )
	{
		Windflag = 40;
	}
	
	for( i  = 0 ; i < windmax ; i ++)
	{			
		if( Windflag )
		{	
			Windflag --;
			if( Windflag == 0 )
			{
				windwind[i] = Random(30) + 30;
			}
		}	
		
		if( windsy[i] < windey[i] )
		{	
			if( windwind[i] ) 
			{
				windwind[i] --;
				windsx[i] -= ( Random( 10 ) + 20 );
			}
			else 
				windsx[ i] += ( Random( 5) -2 );
			
			windsy[ i] += winddownspeed[i];
		}	
		else if( windflag[i] == 0 )
		{	
			windflag[i] = 1;
			windtrans[i] += 2;
			if( windtrans[i] > 15 ) windtrans[i] = 15;
		}		
		else 
		{	
			windtrans[i] --;
			
			if( windtrans[i] < 0 )
			{
				windsx[i]	= mapstartx + Random( SCREEN_WIDTH  + 800 ) - 400;
				windsy[i]	= mapstarty + Random( 200 ) - 200;
				windey[i]	= mapstarty + Random( 680 );
				windflag[i] = 0;
				windwind[i] = 0;
				
				WindAnimationSub( mapstarty, i );
			}
		}	
	}		
}			

void WindAnimationOutput( void )
{	
	int i;	
	
	int mapstartx = g_Map.x * 32;
	int mapstarty = g_Map.y * 32;
	
	for( i  = 0 ; i < windmax ; i ++)
	{
		PutCompressedImageFX( windsx[i] - mapstartx, windsy[i] - mapstarty, &spr[ WIND_SPRITE_+ windspr[i] ], windtrans[i], 2 );
	}
}			



void LighteningSetting( void )
{
	
	
	
}










///////////////////////////////////////////////////////////////////////////////
// window procedure


////////////////////// SoundUp lkh 수정(아래 전부) //////////////////////
DWORD	RainTime;
DWORD	RainStartTime, RainEndTime;
int		RainAmount;
BOOL	Statue_Weather;


int InitWeather( void )
{	
	RainStop();
	SnowStop();
	ThunderStop();
	
	return 0;
}		

void RecvChangeWeather( t_rm_change_weather* p )
{
	if (g_bOffWeatherSystem)
	{	//< CSD-CN-031217
		return;
	}	//> CSD-CN-031217

	g_bStopWeatherSystem = p->bStopWeather?true:false;
	
	switch((int)p->bWeather)
	{
		//멈춤 
	case 0:
		{
			RainStop();
			SnowStop();
			ThunderStop();
			Statue_Weather = 0;
		}
		break;
		//비 
	case 1:
		{
			DWORD amount = p->dwAmount;
			SnowStop();
			ThunderStop();
			if( amount > 900)	amount  = 900;
			RainStart( amount, 0 );
			Statue_Weather = 1;
		}
		break;
		//눈
	case 2:
		{
			RainStop();
			ThunderStop();
			SnowStart( p->dwAmount , 0 );
			Statue_Weather = 2;
		}
		break;
	case 3: //Thunderstorm, added by Eleval 12/08/09
		{
			DWORD amount = p->dwAmount;
			RainStop();
			ThunderStop();
			if( amount > 900)	amount  = 900;
			ThunderStart( amount, 0 );
			Statue_Weather = 1;
		}
		break;
		
	}
	
	
}
//		
short int g_RealDate;
void WeatherDataProcess( DWORD value, DWORD another, DWORD rainstart, DWORD rainend, int amount, int temperature )
{
		static int rainsound;
	g_CurrentTime = value;
	g_ServerClientTimeGap = g_CurrentTime - timeGetTime() + g_ping_delay;
	g_curr_time	= another;
				
	if (Hero == NULL)
	{
		return;
	}
	
	time_t lTime;
	time(&lTime);
	struct tm* today = localtime(&lTime);
	//Log( "Packet받음.txt", "%02d:%02d  %d", today->tm_min, today->tm_sec, addlen );
	if( today->tm_mon == 11 && ( today->tm_mday >=22 || today->tm_mday <= 29 ) )
	{
		if( MapInfo[ MapNumber].rain )
		{
			if( amount )
			{
				SnowStart( amount, 0 );
				Statue_Weather = 2;
			}	
			else
			{	
				SnowStop();
				//Statue_Weather = 0;
			}	
		}	
	}			
	else		
	{			
		RainTime = 0;
		RainStartTime = rainstart;
		RainEndTime = rainend;
		
		if (MapInfo[MapNumber].rain)
		{
			if (0 > rainstart && 0 < rainend)
			{
				amount *= 10;
				
				if (amount > 900)
				{
					amount  = 900;
				}

				RainAmount = amount;
				RainStart(amount, 0);
				Statue_Weather = 1;
			}
			else
			{
				//Statue_Weather = 0;
			}
		}
	}
	
	g_RealDate = temperature;
}

#define FRAME_PER_SEC		15
			  
// ani의 1초당 움직이는 
void CharWalkTime( LPCHARACTER ch, int ani )
{
	double dist = 0.0;
	double framepertime; 

	for( int d= 0 ;  d < ch->AnimationTable[ani].nCount ; d ++) //걷기에 대한 속도검사...
	  dist += (double)ch->AnimationTable[ani].dx[ d];

	// dist =  한번 Ani하는데 이동하는 Dot수.
	// ch->AnimationTable[ani].nCount  = Frame수.
	// framepertime = 32dot(1 Tile)이동하는데 걸리는 Frame수.
	// ch->walktime = 32dot(1 Tile)이동하는데 걸리는 mSEC.
	if( dist )
	{
	  framepertime = TILE_SIZE *(double) ch->AnimationTable[ani].nCount / dist;
	  ch->walktime = (int)( 1000 * framepertime / FRAME_PER_SEC); 
	}
}

int CharCalcMovP( LPCHARACTER ch )
{
	int ret = 50;
	int sx, sy, ex, ey;
	DWORD towalktime;
	
	if( (ch->desttime < g_ClientTime) ||  g_ClientTime == 0 ) return ret;
	
	sx = ch->x / TILE_SIZE;
	sy = ch->y / TILE_SIZE;
	ex = ch->path[ch->pathcount-1][0];
	ey = ch->path[ch->pathcount-1][1];
	
	DWORD d = ch->desttime - g_ClientTime;
	
	if( ch->desttime < g_ClientTime ) 
	{
		ch->movp.PutEncrypted(ret);
		return ret;
	}
	//	DWORD dist = (DWORD )sqrt( (ex-sx)*(ex-sx) + (ey-sy)*(ey-sy) );
	//	int resttile  = dist / TILE_SIZE;
	towalktime = d / (ch->pathcount +1);
	
	if( towalktime )
	{
		ch->movp.PutEncrypted((short )(ch->walktime * 50 / towalktime));
		ret = ch->movp.GetDecrypted();
	}
	
	if( ret >= 200 ){ ret = 200;	ch->movp.PutEncrypted(200);	}
	if( ret < 25 )	{ ret = 25;		ch->movp.PutEncrypted(25);  }
	
	return ret;
}

void LoadChacterAnimationData_Tool( LPCHARACTER  ch )
{
	FILE *fp;
	int i, no, count;
	char temp[ FILENAME_MAX];
	int x, y, attackframe, wave, height, effect;
	char filename[ FILENAME_MAX];
	static int tempsprno;
	char gabage[20];
	
	
	sprintf( filename, "./CHAR/%03dAniTable.txt", ch->sprno );
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )	fp = Fopen( filename, "rt" );
	else fp = NULL;
	if( fp )
	{							
		ch->TotalAni = 0;
		for( i = 0 ; i < 50 ; i ++)
		{			
			count =0;
AGAIN_:		
			if( fgets( temp, FILENAME_MAX, fp ) == NULL ) break;
			
			if( temp[0] == '#' || temp[0] == ' ' || temp[0] == 10 || temp[0] == '\n' )  
			{
				if( temp[0] == '#' )
				{
					char *p;
					if( (p = strchr( temp, 10 )) ) *p = 0;
#ifdef _DEBUG
					strncpy( AnimationName[i], temp + 1, 100 );
#endif
					ch->TotalAni ++;
				}
				
				goto AGAIN_;
			}
			else goto PROCESS__;
			
			while( fgets( temp, FILENAME_MAX, fp ) != NULL )
			{	
PROCESS__:				
			if( temp[ 0] == ' ' || temp[0] == '\n' || temp[0] == '\r' )
			{
				ch->AnimationTable[i].nCount = count;
				break;
			}
			
			sscanf( temp, "%d %d %d %d %d %d %d ", &no, &x, &y, &attackframe, &wave, &height, &effect );
			//			no = atoi( temp ) -1;
			ch->AnimationTable[i].cFrame[ count]	= no-1;
			ch->AnimationTable[i].dx[ count]		=  x;
			ch->AnimationTable[i].dy[ count]		=  y;
			ch->AnimationTable[i].attackf[ count]	=  attackframe;
			ch->AnimationTable[i].wave[ count]		=  wave;
			ch->AnimationTable[i].height[ count]	=  height;
			ch->AnimationTable[i].effect[ count]	=  effect;
			count++;
			ch->AnimationTable[i].nCount = count;
			}	
		}		
		fclose(fp);
		
		int dist = 0;
		
		sprintf( filename, "./CHAR/%03dAniTable.bin", ch->sprno );
		fp = Fopen( filename, "wb" );
		if( fp )
		{
			fwrite( gabage, 20,1, fp );
			char *tt = (char *)ch->AnimationTable;
			char crc = 0;
			for( i = 0 ; i < sizeof( PCANIMATIONTABLE ) * MAX_ANIMATION ; i ++)
			{
				crc += *tt;
				tt ++;
			}
			fwrite( &crc, 1,1, fp );
			fwrite( ch->AnimationTable, sizeof( PCANIMATIONTABLE ), MAX_ANIMATION, fp);
			fclose(fp);
		}
	}
	else 
	{
		sprintf( filename, "./CHAR/%03dAniTable.bin", ch->sprno );
		fp = Fopen( filename, "rb" );
		char *tt = (char *)ch->AnimationTable;
		char crc = 0, crc1;
		if( fp )
		{
			fseek( fp, 20, SEEK_SET );
			fread( &crc1, 1,1, fp );
			fread( ch->AnimationTable, sizeof( PCANIMATIONTABLE ), MAX_ANIMATION, fp);
			for( i = 0 ; i < sizeof( PCANIMATIONTABLE ) * MAX_ANIMATION ;  i ++)
			{
				crc += *tt;
				tt ++;
			}
			fclose(fp);
			
			if( abs(crc1 - crc) )
			{
				//if( SysInfo.gamemakemode ) JustMsg( "CHECKSUM_ERROR_ANIMATIONDATA : ch->sprno = %d ",  ch->sprno );
				CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;
			}
			
			/*sprintf(filename, "./char_test/%03dAniTable.tmp", ch->sprno);
			ofstream fout(filename);
			string strTemp;
			
			for (int j = 0; j < MAX_ANIMATION; j++)
			{
				if (ch->sprno >= 0 && ch->sprno <= 1)
				{ // PC인 경우
					switch (j)
					{
					case ACTION_NONE:                strTemp = "# 기본동작."; break;
					case ACTION_BASIC_WALK:          strTemp = "# 기본걷기."; break;
					case ACTION_BATTLE_WALK_OWNHAND: strTemp = "# 전투걷기1(한손무기)."; break;
					case ACTION_BATTLE_NONE:         strTemp = "# 전투기본1(한손무기)."; break;
					case ACTION_ATTACK_ONESWORD1:    strTemp = "# 한손공격1."; break;
					case ACTION_ATTACK_ONESWORD2:    strTemp = "# 한손공격2."; break;  
					case ACTION_ATTACK_BONG_DOWN:    strTemp = "# 중거리 무기 공격(내려치기)."; break;
					case ACTION_ATTACK_BONG_JJIRKI:  strTemp = "# 중거리 무기 공격(찌르기)."; break;
					case ACTION_ATTACK_BOW:          strTemp = "# 활쏘기."; break;      
					case ACTION_ATTACK_PENSWORD2:    strTemp = "# 펜싱자세."; break;   
					case ACTION_ATTACK_PENSING1:     strTemp = "# 펜싱공격 1."; break; 
					case ACTION_ATTACK_PENSING2:     strTemp = "# 펜싱공격 2."; break; 
					case ACTION_RUN:                 strTemp = "# 뛰기."; break;
					case ACTION_ATTACKED:            strTemp = "# 맞기."; break;
					case ACTION_DIE:                 strTemp = "# 죽기."; break;
					case ACTION_MAGIC_BEFORE:        strTemp = "# 마법시전."; break;
					case ACTION_MAGIC_CASTING:       strTemp = "# 마법캐스팅."; break;
					case ACTION_SITDOWN:             strTemp = "# 앉기."; break;
					case ACTION_THROW:               strTemp = "# 던지기공격."; break;
					case ACTION_HORSE:               strTemp = "# 말타기."; break;
					case ACTION_BATTLE_WALK_TWOHAND: strTemp = "# 전투걷기2(양손무기)."; break;
					case ACTION_BATTLE_NONE_TWOHAND: strTemp = "# 전투기본2(양손무기)."; break;
					case ACTION_ATTACK_BONG_ROUND:   strTemp = "# 중거리 무기 공격(휘두르기)."; break;
					case ACTION_MAGIC_CASTING2:      strTemp = "# 마법캐스팅2(남자)."; break;
					default: continue;
					}
				}
				else
				{ // NPC인 경우
					switch (j)
					{
					case MON1_NONE:          strTemp = "# 기본동작."; break;
					case MON1_BATTLE_WALK:   strTemp = "# 기본걷기."; break;
					case MON1_ATTACKED:		   strTemp = "# 맞기동작."; break;
					case MON1_DIE:			     strTemp = "# 죽기동작."; break;
					case MON1_BATTLE_ATTACK: strTemp = "# 공격동작."; break;
					case MON1_BATTLE_MAGIC:  strTemp = "# 마법동작."; break;
					case MON1_STAND_UP	: strTemp = "# 일어서기."; break;			// 일어서기
					case MON1_CASTING_MAGIC : strTemp = "# 마법시전."; break;		// 08마법시전			// LTS DRAGON MODIFY
					case MON1_BATTLE_ATTACK2 : strTemp = "# 공격2."; break;	// 09공격 2
					case MON1_BATTLE_ATTACK3 : strTemp = "# 공격3."; break;	// 10공격 3
					case MON1_JUST_ACTION : strTemp = "# 임의의동작."; break;		// 11임의의 동작 	
					default: continue;
					}
				}
				
				fout << strTemp.c_str() << endl; 
#ifdef _DEBUG
				strncpy(AnimationName[j], strTemp.c_str() + 1, 100);
#endif
				++ch->TotalAni;
				
				const int nCount = ch->AnimationTable[j].nCount;
				
				for (int k = 0; k < nCount; k++)
				{
					int nFrame = ch->AnimationTable[j].cFrame[k] + 1;
					int nDX = ch->AnimationTable[j].dx[k];
					int nDY = ch->AnimationTable[j].dy[k];
					int nAttack = ch->AnimationTable[j].attackf[k];
					int nWave = ch->AnimationTable[j].wave[k];
					int nHeight = ch->AnimationTable[j].height[k];
					int nEffect = ch->AnimationTable[j].effect[k];
					fout << nFrame << "\t";
					fout << nDX << "\t";
					fout << nDY << "\t";
					fout << nAttack << "\t";
					fout << nWave << "\t";
					fout << nHeight << "\t";
					fout << nEffect << endl;
				}
				
				fout << endl;
			} */
		}
		else 
		{
			if( GetSysInfo( SI_GAME_MAKE_MODE ) ) JustMsg( "CheckNumError : ch->sprno = %d ( No File )",  ch->sprno );	// 010612 KHS
			CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;			
		}
	}
}

void LoadChacterAnimationData( LPCHARACTER  ch )
{				
	FILE *fp;
	int i, no, count;
	char temp[ FILENAME_MAX];
	int x, y, attackframe, wave, height, effect;
	char filename[ FILENAME_MAX];
	static int tempsprno;
	char gabage[20];
	
	
	sprintf( filename, "./CHAR/%03dAniTable.txt", ch->sprno );
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )	fp = Fopen( filename, "rt" );
	else fp = NULL;
	if( fp )
	{							
		ch->TotalAni = 0;
		for( i = 0 ; i < 50 ; i ++)
		{			
			count =0;
AGAIN_:		
			if( fgets( temp, FILENAME_MAX, fp ) == NULL ) break;
												
			if( temp[0] == '#' || temp[0] == ' ' || temp[0] == 10 || temp[0] == '\n' )  
			{
				if( temp[0] == '#' )
				{
					char *p;
					if( (p = strchr( temp, 10 )) ) *p = 0;
#ifdef _DEBUG
					strncpy( AnimationName[i], temp + 1, 100 );
#endif
					ch->TotalAni ++;
				}
				
				goto AGAIN_;
			}
			else goto PROCESS__;
			
			while( fgets( temp, FILENAME_MAX, fp ) != NULL )
			{	
PROCESS__:				
			if( temp[ 0] == ' ' || temp[0] == '\n' || temp[0] == '\r' )
			{
				ch->AnimationTable[i].nCount = count;
				break;
			}
			
			sscanf( temp, "%d %d %d %d %d %d %d ", &no, &x, &y, &attackframe, &wave, &height, &effect );
			//			no = atoi( temp ) -1;
			ch->AnimationTable[i].cFrame[ count]	= no-1;
			ch->AnimationTable[i].dx[ count]		=  x;
			ch->AnimationTable[i].dy[ count]		=  y;
			ch->AnimationTable[i].attackf[ count]	=  attackframe;
			ch->AnimationTable[i].wave[ count]		=  wave;
			ch->AnimationTable[i].height[ count]	=  height;
			ch->AnimationTable[i].effect[ count]	=  effect;
			count++;
			ch->AnimationTable[i].nCount = count;
			}	
		}		
		fclose(fp);
		
		int dist = 0;
		
		sprintf( filename, "./CHAR/%03dAniTable.bin", ch->sprno );
		fp = Fopen( filename, "wb" );
		if( fp )
		{
			fwrite( gabage, 20,1, fp );
			char *tt = (char *)ch->AnimationTable;
			char crc = 0;
			for( i = 0 ; i < sizeof( PCANIMATIONTABLE ) * MAX_ANIMATION ; i ++)
			{
				crc += *tt;
				tt ++;
			}
			fwrite( &crc, 1,1, fp );
			fwrite( ch->AnimationTable, sizeof( PCANIMATIONTABLE ), MAX_ANIMATION, fp);
			fclose(fp);
		}
	}
	else 
	{
		sprintf( filename, "./CHAR/%03dAniTable.bin", ch->sprno );
		fp = Fopen( filename, "rb" );
		char *tt = (char *)ch->AnimationTable;
		char crc = 0, crc1;
		if( fp )
		{
			fseek( fp, 20, SEEK_SET );
			fread( &crc1, 1,1, fp );
			fread( ch->AnimationTable, sizeof( PCANIMATIONTABLE ), MAX_ANIMATION, fp);
			for( i = 0 ; i < sizeof( PCANIMATIONTABLE ) * MAX_ANIMATION ;  i ++)
			{
				crc += *tt;
				tt ++;
			}
			fclose(fp);
			
			if( abs(crc1 - crc) )
			{
				//if( SysInfo.gamemakemode ) JustMsg( "CHECKSUM_ERROR_ANIMATIONDATA : ch->sprno = %d ",  ch->sprno );
				CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;
			}
		}
		else 
		{
			if( GetSysInfo( SI_GAME_MAKE_MODE ) ) JustMsg( "CheckNumError : ch->sprno = %d ( No File )",  ch->sprno );	// 010612 KHS
			CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;			
		}
	}
}	

void LoadClothOrderData( void )
{			
	FILE *fp;
	
	fp = Fopen( "./char/00ClothOrderBasic.bin","rb" );
	if( fp )
	{		
		for( int i = 0 ; i < 8 ; i ++)
			for( int j = 0 ; j < 75 ; j ++)
				fread( &SpriteOrder[0][i][j], 1,1, fp );
			fclose(fp);
	}		
	fp = Fopen( "./char/01ClothOrderBasic.bin","rb" );
	if( fp )
	{		
		for( int i = 0 ; i < 8 ; i ++)
			for( int j = 0 ; j < 75 ; j ++)
				fread( &SpriteOrder[1][i][j], 1,1, fp );
			fclose(fp);
	}		
	fp = Fopen( "./char/00ClothOrderBow.bin","rb" );
	if( fp )
	{		
		for( int i = 0 ; i < 8 ; i ++)
			for( int j = 0 ; j < 75 ; j ++)
				fread( &SpriteOrder[2][i][j], 1,1, fp );
			fclose(fp);
	}		
	fp = Fopen( "./char/01ClothOrderBow.bin","rb" );
	if( fp )
	{		
		for( int i = 0 ; i < 8 ; i ++)
			for( int j = 0 ; j < 75 ; j ++)
				fread( &SpriteOrder[3][i][j], 1,1, fp );
			fclose(fp);
	}		
}			






//----------------------------------------

// 배경 Sprite를 찍기 위한 루틴.


void PutTileNotCliping( int x, int y, LPVOID Stmp, LPVOID dest )
{		
	LPVOID	Dtmp = (LPVOID)( (char*)dest + y * dDxSize + (x * 2 )); //removes 64bytes
	
	_asm{
		push	es;
		
		push	ds;
		pop		es;
		
		mov     ecx, dDxSize;
		sub     ecx, 64;
		
		mov		edi,Dtmp;
		mov		esi,Stmp;
		
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // subjtracting 64byte from edi =0
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 1
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 2
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 3
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 4
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 5
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 6
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 7
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 8
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 9
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 10
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 11
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 12
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 13
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 14
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 15
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 16
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 17
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 18
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 19
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 20
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 21
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 22
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 23
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 24
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 25
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 26
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 27
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 28
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 29
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		add edi, ecx; // 30
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		movsd;
		
		pop		es;
	}
}


void PutTileCliping( int x, int y, LPVOID Stmp, LPVOID dest, int lx, int rx, int ty, int by )
{
	int  xl = TILE_SIZE;
	int  yl = TILE_SIZE;
	WORD *p, *sou;
	int  i,j;
	
	sou = (WORD *)((char*)Stmp + (lx + ty * xl) * 2);
	for( i = ty ; i < by ; i ++)
	{	
		p = (WORD*)((char *)dest + x * 2 + y * dDxSize);
		for( j = lx ; j < rx ; j ++, p++, sou++ )
			*p = *sou;
		
		sou += ( xl - ( rx - lx ) );
		y ++;
	}	
}


void PutTile( int x, int y, LPVOID Stmp, LPVOID dest )
{
	int	lx, rx, ty, by , IsC=0;
	int xl = TILE_SIZE;
	int yl = TILE_SIZE;
	
	if( x >= SCREEN_WIDTH  )			return; 
	else if( x + xl < 0 )	return; 
	if( y >= SCREEN_HEIGHT )			return; 
	else if( y + yl < 0  )	return;
	
	rx = xl;
	lx = 0;
	ty = 0;
	by = yl;
	
	if( x + xl > SCREEN_WIDTH )
	{
		rx -= x + xl - SCREEN_WIDTH;
		IsC = 1;
	}
	if( x < 0 )
	{	
		lx  = 0 - x;
		x   = 0;
		//		rx -= lx;
		IsC = 1;
	}	
	if( y + yl > SCREEN_HEIGHT )
	{	
		by -= y + yl - SCREEN_HEIGHT;
		IsC = 1;
	}	
	if(	y < 0 )
	{	
		ty  = 0 - y;
		y   = 0;
		//		by -= ty;
		IsC = 1;
	}		
	
	if(IsC)		PutTileCliping( x, y, Stmp, dest, lx, rx, ty, by );
	else 		PutTileNotCliping( x, y, Stmp, dest );
}






WORD RGB16( int r, int g, int b )
{	
	r >>= 3;
	g >>= 3;
	b >>= 3;
	if( _PixelInfo.BitMaskR != 0x7C00)
	{
		return ((r)<< 11) | (g)<<6 |(b);
	}
	return ((r)<< 10) | (g)<<5 |(b);
}


DWORD ReturnBlendRGB( int r, int g, int b )
{
	WORD rgb = (WORD)RGB16( r, g, b );
	DWORD RGB;
	
	if( _PixelInfo.BitMaskR == 0x7C00)
	{
		_asm{	xor     edx, edx;
		mov		dx,rgb;
		ror		edx,10;
		shl		dx,11;
		rol		edx,5;
		shl		dx,6;
		rol		edx,5;
		mov     RGB, edx;
		}
	}
	else
	{
		_asm{	xor     edx, edx;
		mov		dx,rgb;
		ror		edx,11;
		shl		dx,10;
		rol		edx,6;
		shl		dx,5;
		rol		edx,5;
		mov     RGB, edx;
		}
	}
	return RGB;
}

void ViewVersion( int version )
{	//< CSD-030509
	return;			// 0625 YGI
	char temp[ MAX_PATH];
	sprintf( temp, "v.%03d", version );
	Hcolor( 0, 0, 0 );
	Hprint( SCREEN_WIDTH - strlen( temp) *12, 3, g_DestBackBuf, temp );
	Hcolor( 150, 150, 150 );
	Hprint( SCREEN_WIDTH - strlen( temp) *12-1, 2, g_DestBackBuf, temp );
	
	int mapx = g_Map.x + g_pointMouseX/TILE_SIZE;
	int mapy = g_Map.y + g_pointMouseY/TILE_SIZE;
	HprintBold( SCREEN_WIDTH - 30* 12 - 5, 31, 0xffff, 0,  lan->OutputMessage(6,52), MapNumberName, mapx, mapy);
}	//> CSD-030509

static int		HEAD, TAIL;

#define MAX_QUEUE_SPRITE    3000

HANDLE ghLoadCharSpriteDataThread;
DWORD  gdwLoadCharSpriteDataThread;

typedef struct 
{	
	char filename[FILENAME_MAX];
	char **buf;
	DWORD *length;
	CharacterSprite *charactersprite;
}t_queueSprite;


t_queueSprite  queueSprite[ MAX_QUEUE_SPRITE];



void ClearQueueSprite( void )
{		
	HEAD = TAIL = 0;
}		

int queueInSprite( t_queueSprite *n )
{			
	g_criticThread.Lock();
	
	if( (TAIL+1) %  MAX_MQUEUE != HEAD )
	{	
		memcpy( &queueSprite[TAIL], n, sizeof( t_queueSprite ) );
		TAIL ++;
		TAIL = TAIL % MAX_MQUEUE ;
		g_criticThread.Unlock();
		return 1;
	}	
	g_criticThread.Unlock();
	return 0;
}		

int queueOutSprite( t_queueSprite *n)
{		
	if( TAIL != HEAD )
	{	
		memcpy( n, &queueSprite[HEAD], sizeof( t_queueSprite ) );
		HEAD ++;
		HEAD = HEAD % MAX_MQUEUE;
		return 1;
	}	
	
	n = NULL;
	return 0;
}		

void InputThreadCharSprite( char *filename, char **buf, DWORD *length, CharacterSprite *charactersprite, int mode )
{	
	t_queueSprite n;
	
	strcpy( n.filename, filename );
	n.buf				= buf;
	n.length			= length;
	n.charactersprite	= charactersprite;
	
	if( queueInSprite( &n ) )
	{}
	else Error( lan->OutputMessage(6,53));//lsw
}	


DWORD __stdcall LoadCharSpriteDataThread( void *g )
{	
	t_queueSprite n;
	
	while( 1 )
	{
		if( queueOutSprite( &n) )
		{
			LoadingCharSpriteData = true;
			LoadCharSpriteData_Sub( n.filename, n.buf, n.length, n.charactersprite, LD_CHAR_SPRITE_ALL_LOAD );
			LoadingCharSpriteData = false;
		}
		Sleep(10);
    }
	
	return 0;
}	


void StartLoadCharSpriteDataThread( void )
{
	ghLoadCharSpriteDataThread=CreateThread(NULL,0, LoadCharSpriteDataThread, NULL,0,&gdwLoadCharSpriteDataThread);
	SetThreadPriority( ghLoadCharSpriteDataThread, THREAD_PRIORITY_ABOVE_NORMAL );
	
	StartLoadCharSpriteDataThreadFlag = true;
}


void EndLoadCharSpriteDataThread( void )
{
	if( StartLoadCharSpriteDataThreadFlag == true )
		TerminateThread( ghLoadCharSpriteDataThread, 0 );
	StartLoadCharSpriteDataThreadFlag = false;
}