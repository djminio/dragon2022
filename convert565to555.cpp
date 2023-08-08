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
#include <ddraw.h>
#include "dragon.h"
#include "Hong_Sprite.h"
#include "Convert565to555.h"

extern	TPixelInfo	_PixelInfo;

int  convert565to555(Spr *sp)
{
	int FFFFCheak=0;
	int ConvertCount;
	int i;
	WORD *p;
	int tt;

	
	if( _PixelInfo.BitMaskR != 0x7C00 )
	{
		return 0;
	}

	if( sp->xl == 0 ) return -1;
	if( sp->yl == 0 ) return -2;
	if( sp->size == 0 ) return -3;

	 p = (WORD *)sp->img;

	while(1)
	{
AGAIN_:
		tt = 0;
		if(*p == 0xFFFF)
		{
			tt = *p;
			FFFFCheak++;
			p++;

			if(FFFFCheak == sp->yl ) break;
		}

		if( tt == 0xffff ) goto AGAIN_;

		p++;   //투명색 건너 뛰기
		ConvertCount=*p;   //색 갯수 체크
		p++;    //체크후 포인터 이동

		for(i=0; i < ConvertCount;i+=2)
		{
			*p=((*p)&0xFFC0)>>1 | ((*p)&0x1F);
			p++;
		}

		if(*p == 0xFFFF)
		{
			FFFFCheak++;
			p++;

			if(FFFFCheak == sp->yl ) break;
		}
	}

	return 0;
}


int  convert565to555_LoadSprite(Spr *sp)
{
	int FFFFCheak=0;
	int ConvertCount;
	int i;
	WORD *p;
	int tt;

	
	if( _PixelInfo.BitMaskR != 0x7C00 )
	{
		return 0;
	}

	if( sp->xl == 0 ) return -1;
	if( sp->yl == 0 ) return -2;
	if( sp->size == 0 ) return -3;

	 p = (WORD *)sp->img;

	while(1)
	{
AGAIN_:
		tt = 0;
		if(*p == 0xFFFF)
		{
			tt = *p;
			FFFFCheak++;
			p++;

			if(FFFFCheak == sp->yl ) break;
		}

		if( tt == 0xffff ) goto AGAIN_;

		p++;   //투명색 건너 뛰기
		ConvertCount=*p;   //색 갯수 체크
		p++;    //체크후 포인터 이동

		for(i=0; i < ConvertCount;i+=2)
		{
			*p=((*p)&0xFFC0)>>1 | ((*p)&0x1F);
			p++;
		}

		if(*p == 0xFFFF)
		{
			FFFFCheak++;
			p++;

			if(FFFFCheak == sp->yl ) break;
		}
	}
	return 0;
}





void convert565to555RawTile( char *img )
{
	int i;
	WORD *p;

	if( _PixelInfo.BitMaskR != 0x7C00 )		return;

	 p = (WORD *)img;

	 for( i = 0 ; i < TILE_SIZE * TILE_SIZE ; i ++)
	 {
		 *p=((*p)&0xFFC0)>>1 | ((*p)&0x1F);
		 p++;
	 }
}


// 스프라이트 끼리 카피한다.
// 주의 사항은 img 의 메모리 카피를 의식해야 한다.
void CopySprToSpr( Spr *target, const Spr *source )
{
	target->xl = source->xl;
	target->yl = source->yl;
	target->ox = source->ox;
	target->oy = source->oy;
	target->size = source->size;
	target->img = new char[target->size];
	memcpy( target->img, source->img, target->size );
}
