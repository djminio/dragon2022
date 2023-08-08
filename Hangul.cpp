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
#include <Windows.h>  // ^^^^ 0126
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "Dragon.h"
#include <Ddraw.h>
#include "tOOL.H"
#include "Music.h"



#define __HANGUL_H__
#include "hangul.h"


#define HBSIZE  32        // ÇÑ±Û ÇÑ ±ÛÀÚ¿¡ ÇÊ¿äÇÑ ¹ÙÀÌÆ® ¼ö
#define NTYPES1 8         // ÃÊ¼ºÀÇ ¹ú¼ö
#define NTYPES2 4         // Áß¼ºÀÇ ¹ú¼ö
#define NTYPES3 4         // Á¾¼ºÀÇ ¹ú¼ö

#define HANGUL_SIZE     16
#define ENGLISH_SIZE    8
#define Y_SIZE          16

int		NHangulColor;
HDC		DDText_dc;
int		Cliping_sx,	Cliping_ex,	Cliping_sy,	Cliping_ey;


static BYTE BaAscFont[ 96 ][ 16 ];
static BYTE BaHanFont1[ NTYPES1 ][ 19 ][ HBSIZE ];
static BYTE BaHanFont2[ NTYPES2 ][ 21 ][ HBSIZE ];
static BYTE BaHanFont3[ NTYPES3 ][ 27 ][ HBSIZE ];


HFONT	__HFontDial[10];
HRGN	MainHrgn;
HFONT	hFontOld;
HRGN		hGrnOld;



static char *h_DestBuf;
static int HangulFontType = 2;
static int HangulAlign;


/* ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
 ÇÔ¼ö¸í : LoadHnagulEnglishFont
 ÀÎ  ÀÚ : char *caHangulFileName
          char *caEngFileName
 ¸®ÅÏ°ª : 0 = ½ÇÆÐ, 1 = ¼º°ø
 ¼³  ¸í : ÇÑ±Û°ú ¿µ¹®¿¡ ÇØ´çÇÏ´Â ÆùÆ®¸¦ È­ÀÏ¿¡¼­ ÀÐ¾î¿Â´Ù.
¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡*/
// 031110 YGI
int LoadHangulEnglishFont( char *caHangulFileName, char *caEngFileName )
{
#ifndef _SDL
	EndFont();
#else
	SDL_EndFont();
#endif
/*
    FILE *fp;
    int i;


    // ÇÑ±Û È­ÀÏÀ» ¿ÀÇÂ
    fp = fopen( caHangulFileName, "rb" );

    // ÃÊ¼ºÀ» ÀÐ¾îµéÀÓ
    for( i = 0; i < 8; i++ ) {
        fseek( fp, 32, SEEK_CUR );
        fread( (void *)BaHanFont1[ i ], 19 * 32, 1, fp );
    }

    // Áß¼ºÀ» ÀÐ¾îµéÀÓ
    for( i = 0; i < 4; i++ ) {
        fseek( fp, 32, SEEK_CUR );
        fread( (void *)BaHanFont2[ i ], 21 * 32, 1, fp );
    }

    // Á¾¼ºÀ» ÀÐ¾îµéÀÓ
    for( i = 0; i < 4; i++ ) {
        fseek( fp, 32, SEEK_CUR );
        fread( (void *)BaHanFont3[ i ], 27 * 32, 1, fp );
    }
    fclose( fp );

    // ¿µ¹® ÆùÆ®¸¦ ÀÐ¾îµéÀÓ
    fp = fopen( caEngFileName, "rb" );
    fread( (void *)BaAscFont, 16, 96, fp );

    fclose( fp );
*/


//////////////////////////////////////////////////////////////////////////////010727 lms
#ifdef CHINA_LOCALIZING_//010528 lms ´Ù±¹¾î ÆùÆ® Àû¿ë
	char *DR_FONT =  "ËÎÌå";//010527 lms Áß±¹¾î ÆùÆ®
#endif

#if defined (TAIWAN_LOCALIZING_) || defined (HONGKONG_LOCALIZING_)
	char *DR_FONT =  "²Ó©úÅé";//010511 lms ´ë¸¸ ÆùÆ®
#endif

#ifdef KOREA_LOCALIZING_
	char *DR_FONT =  "±¼¸²Ã¼";
#endif

#ifdef USA_LOCALIZING_
	char *DR_FONT =  "MS Mincho";
#endif
// THAI YGI
#ifdef THAI_LOCALIZING_
	DR_FONT =  "MS Sans Serif";		// BBD 031214 ÅÂ±¹µµ USA_LOCALIZING_ÀÌ ¼±¾ðµÈ´Ù
	int DR_FONT_SIZE = 15;
#else
	#ifdef USA_LOCALIZING_
		int DR_FONT_SIZE =	12;
	#else
		int DR_FONT_SIZE = 12;
	#endif

#endif

#ifdef JAPAN_LOCALIZING_
	char *DR_FONT =  "MS Mincho"//"FixedSys"@//020325 lms
#endif

//////////////////////////////////////////////////////////////////////////////

	char temp_font_name[50] = {0,};
	GetPrivateProfileString( "font", "name", "", temp_font_name, 50, "./font.ini" );
	if( temp_font_name[0] )
	{
		DR_FONT = temp_font_name;
	}
	int temp_font_size = 0;
	temp_font_size = GetPrivateProfileInt( "font", "size", 0, "./font.ini" );
	if( temp_font_size )
	{
		DR_FONT_SIZE = temp_font_size;
	}

// Windows Font Setting----------------------------------------------------
	//< CSD-030324
	__HFontDial[0] = CreateFont( DR_FONT_SIZE, 0, 0, 0,					// ÆùÆ® ¼³Á¤ÇÏ±â...
					FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					VARIABLE_PITCH,
					DR_FONT );

	__HFontDial[1] = CreateFont( DR_FONT_SIZE, 0, 0, 0,					// ÆùÆ® ¼³Á¤ÇÏ±â...
					FW_LIGHT, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					VARIABLE_PITCH,
					DR_FONT );


	__HFontDial[2] = CreateFont( DR_FONT_SIZE, 0, 0, 0,					// ÆùÆ® ¼³Á¤ÇÏ±â...
					FW_LIGHT, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					VARIABLE_PITCH,
					DR_FONT );



	__HFontDial[3] = CreateFont( DR_FONT_SIZE-3, 0, 0, 0,					// ÆùÆ® ¼³Á¤ÇÏ±â... 0116 YGI
					FW_LIGHT, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					VARIABLE_PITCH,
					DR_FONT );


	__HFontDial[4] = CreateFont( DR_FONT_SIZE+2, 0, 0, 0,					// ÆùÆ® ¼³Á¤ÇÏ±â...
					FW_LIGHT, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					VARIABLE_PITCH,
					DR_FONT );
	//> CSD-030324

// Windows Font Setting----------------------------------------------------

	return (1);
}


void EndFont(void )
{
	for( int i = 1 ; i < 7 ; i ++)
		DeleteObject( __HFontDial[i] );
}


void SDL_EndFont(void)
{
#ifdef _SDL2
	HDC	hdc = GetDC(GetHMain());
	{
		for (int i = 0; i < 10; ++i)
		{
			HFONT hFontOld = SelectFont(hdc, __HFontDial[i]);
			SelectFont(hdc, hFontOld);
			DeleteFont(__HFontDial[i]);
		}
	}
	ReleaseDC(GetHMain(), hdc);
#endif
}
void  HangulOutputArea( int lx, int rx, int ty, int by )
{
	Cliping_sx = lx, Cliping_ex = rx, Cliping_sy = ty, Cliping_ey = by;
}



/*¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
 ÇÔ¼ö¸í : memor
 ÀÎ  ÀÚ : s1 = ¼Ò½º ¹öÆÛ, s2 = Å¸°Ù ¹öÆÛ, len = ±æÀÌ
 ¸®ÅÏ°ª : ¾øÀ½
 ¼³  ¸í : s2ÀÇ ³»¿ë°ú s1ÀÇ ³»¿ëÀ» ³í¸®Àû ÇÕÀ» ÇÑ´ÙÀ½¿¡ s1ÀÇ ¹öÆÛ¿¡ ÀúÀåÇÑ´Ù.
¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡*/
static void memor( BYTE *s1, BYTE *s2, int len )
{
    do *s1++ |= *s2++; while( --len );
}

/*¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
 ÇÔ¼ö¸í : GetHanImage
 ÀÎ  ÀÚ : c = ¿øÇÏ´Â ÀÌ¹ÌÁö¿¡ ÇØ´çÇÏ´Â ÇÑ±Û ÄÚµå
 ¸®ÅÏ°ª : ¹öÆÛÀÇ Æ÷ÀÎÅÍ
 ¼³  ¸í : c ¹®ÀÚ¿¡ ÇØ´çÇÏ´Â ¹öÆÛÀÇ ÀÌ¹ÌÁö¸¦ ¸®ÅÏÇÑ´Ù.
¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡*/
BYTE *GetHanImage( WORD c )
{
    WORD h1, h2, h3, type1, type2, type3;
    static BYTE ib[ HBSIZE ];        // ÇÑ±Û ÀÚÇü ¹öÆÛ
    static BYTE idxtbl1[] = {        // ÃÊ¼º ÀÎµ¦½º Å×ÀÌºí
        0,  0,  1,  2,  3,  4,  5,  6,
        7,  8,  9, 10, 11, 12, 13, 14,
       15, 16, 17, 18, 19,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0
    };

    static BYTE idxtbl2[] = {        // Áß¼º ÀÎµ¦½º Å×ÀÌºí
        0,  0,  0,  1,  2,  3,  4,  5,
        0,  0,  6,  7,  8,  9, 10, 11,
        0,  0, 12, 13, 14, 15, 16, 17,
        0,  0, 18, 19, 20, 21,  0,  0
    };

    static BYTE idxtbl3[] = {        // Á¾¼º ÀÎµ¦½º Å×ÀÌºí
        0,  0,  1,  2,  3,  4,  5,  6,
        7,  8,  9, 10, 11, 12, 13, 14,
       15, 16,  0, 17, 18, 19, 20, 21,
       22, 23, 24, 25, 26, 27,  0,  0
    };

    static char type1tbl_no[] = {    // Áß¼º¿¡ µû¸¥ ÃÊ¼º Á¾·ù, ¹ÞÄ§ ¾øÀ½
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
        3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0
    };

    static char type1tbl_yes[] = {   // Áß¼º¿¡ µû¸¥ ÃÊ¼º Á¾·ù, ¹ÞÄ§ ÀÖÀ½
        5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7,
        7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5
    };

    static char type3tbl[] = {       // Áß¼º¿¡ µû¸¥ Á¾¼º Á¾·ù
        0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0,
        2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1
    };


    h1 = ( c >> 10 ) & 0x1f;        // ÃÊ¼º ÄÚµå
    h2 = ( c >> 5  ) & 0x1f;        // Áß¼º ÄÚµå
    h3 = c           & 0x1f;        // Á¾¼º ÄÚµå

    h1 = idxtbl1[ h1 ];             // ÃÊ¼º ÀÎµ¦½º ÄÚµå
    h2 = idxtbl2[ h2 ];             // Áß¼º ÀÎµ¦½º ÄÚµå
    h3 = idxtbl3[ h3 ];             // Á¾¼º ÀÎµ¦½º ÄÚµå

    type1 = h3 ? type1tbl_yes[ h2 ] : type1tbl_no[ h2 ];
    type2 = ( (h1 == 0 || h1 == 1 || h1 == 16) ? 0 : 1) + (h3 ? 2 : 0);
    type3 = type3tbl[ h2 ];

    if( h1 )
        memcpy( ib, BaHanFont1[ type1 ][ h1 - 1 ], HBSIZE );
    else
        memset( ib, 0, HBSIZE );

    if( h2 ) memor( ib, BaHanFont2[ type2 ][ h2 - 1 ], HBSIZE );
    if( h3 ) memor( ib, BaHanFont3[ type3 ][ h3 - 1 ], HBSIZE );

    return ib;
}

/*¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
 ÇÔ¼ö¸í : PutHangul
 ÀÎ  ÀÚ : x,y = ¹®ÀÚÀÇ ÁÂÇ¥, s = ÀÌ¹ÌÁö
 ¸®ÅÏ°ª : ¾øÀ½
 ¼³  ¸í : È­¸é¿¡ ÇÑ±Û ÄÚµå¿¡ ÇØ´çÇÏ´Â ÀÌ¹ÌÁö¸¦ Ãâ·ÂÇÑ´Ù.
          °ÔÀÓ¸ðµå¸¦ ¹Ù²Ü°æ¿ì ÀÌ ÇÔ¼ö¸¦ ¼öÁ¤ÇÏ¸é µÈ´Ù.
¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡*/
void PutHangul( int x, int y, WORD color, BYTE *s )
{
    int i, j, k, xx;
    BYTE c;

	if( x <= Cliping_sx || x >= Cliping_ex - 20 || y <= Cliping_sy || y >=  Cliping_ey - 20 ) return;

    for( i = 0; i < Y_SIZE; i++, y++ ) {
        for( j = 0, xx = x; j < 2; j++ ) {
            c = *s++;
            for( k = 0; k < 8; k++, xx++ ) {
                if( c & 0x80 ) *(WORD*)(h_DestBuf + y * 1280 + xx *2 ) = color;
                c <<= 1;
            }
        }
    }
}




/*¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
 ÇÔ¼ö¸í : PutEnglish
 ÀÎ  ÀÚ : x,y = ¹®ÀÚÀÇ ÁÂÇ¥, s = ÀÌ¹ÌÁö
 ¸®ÅÏ°ª : ¾øÀ½
 ¼³  ¸í : È­¸é¿¡ ¿µ¹® ÄÚµå¿¡ ÇØ´çÇÏ´Â ÀÌ¹ÌÁö¸¦ Ãâ·ÂÇÑ´Ù.
          °ÔÀÓ¸ðµå¸¦ ¹Ù²Ü°æ¿ì ÀÌ ÇÔ¼ö¸¦ ¼öÁ¤ÇÏ¸é µÈ´Ù.
¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡*/
void PutEnglish( int x, int y, WORD color, BYTE *s )
{
    BYTE k;
    int i, j, xx;

	if( x <= Cliping_sx || x >= Cliping_ex - 20 || y <= Cliping_sy || y >=  Cliping_ey - 20 ) return;


    for( i = 0; i < Y_SIZE; i++, y++ ) {
        for( j = 0, xx = x, k = 0x80; j < ENGLISH_SIZE; j++, xx++, k >>= 1 ) {
            if( (*s & k) != 0 ) *(WORD*)(h_DestBuf + y * 1280 + xx *2 ) = color;
        }
        s++;
    }
}





#define LETTER_VER_SIZE		16
/*¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
 ¼³¸í :	´Ü¼øÈ÷ Ãâ·ÂÀ»ÇÑ´Ù. ( hangul.fnt, eng.fnt ÇÊ¿ä )
 ÀÎÀÚ :
 ¸®ÅÏ :
¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡*/
/*void Hprint( int x, int y, char *destbuf, char *temp )
{
	int  i;
	WORD k;
	BYTE *t, *kk;
	int  tchar;
	int  yy, oldx;
	WORD c,c2,kscode, index;

	h_DestBuf = destbuf;

	oldx = x;
	i = 0; yy = 0;
	for( ; *(temp+i); i++ )
	{
		if( *(temp+i) & 0x80 ) {        // ÇÑ±ÛÀÏ °æ¿ì
			kscode = *(WORD *)(temp+i++);
			c = kscode >> 8;
			c2 = kscode & 0xff;
			index = ( (int)c2 - 0xb0 ) * 94 + ( (int)c  - 0xa1 );
			kk = (BYTE *)&NaKsTable[ index * 2 ];
			k = *(kk)<<8;
			k += (*(++kk));

			t = GetHanImage(k);
			PutHangul( x, y, NHangulColor, t );

			x += HANGUL_SIZE;
		}
		else { // ¿µ¹®ÀÏ °æ¿ì
			tchar = *(temp+i);
			if( tchar == 32 )  goto NEXT_00;
 			if( tchar == 92 || tchar == 47 ) { goto NEXT_01; } // 92 = \  47 = /
			t = BaAscFont[ tchar -32 ];
			PutEnglish( x, y, NHangulColor, t );

			NEXT_00:
			x += ENGLISH_SIZE;
		  }
NEXT_01:;
	}
}		*/



int HR, HB, HG;



void Hcolor( WORD color )
{
	HR = (color>>11 & 0x1f) << 3;
	HG = (color>>6  & 0x1f) << 3;
	HB = (color     & 0x1f) << 3;
}


void Hcolor( int r, int g, int b )
{
	HR = r, HG = g, HB = b;
}


void Getcolor( int *r, int *g, int *b )
{
	*r = HR, *g = HG, *b = HB;
}

int MakeRGB( const WORD wColor)
{
	const int iR = (wColor>>11 & 0x1f) << 3;
	const int iG = (wColor>>6  & 0x1f) << 3;
	const int iB = (wColor     & 0x1f) << 3;
	return RGB(iR,iG,iB);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	<:color:%d> ¿Í °°Àº Çü½ÄÀÌ¸é ´ÙÀ½Ãâ·ÂµÇ´Â Color¸¦ ¹Ù²Û´Ù.
//	(O--:111222333444) 111,222 - 333,444ÀÇ ¼±À» ±×¸®´Âµ¥ O---¸ð¾çÀÌ µÈ´Ù.
//	(---:111222333444) 111,222 - 333,444ÀÇ ¼±À» ±×¸®´Âµ¥ ----¸ð¾çÀÌ µÈ´Ù.
//	(-->:111222333444) 111,222 - 333,444ÀÇ ¼±À» ±×¸®´Âµ¥ --->¸ð¾çÀÌ µÈ´Ù.
//	(¼º¿ì:000) 000.mp3¸¦ Ãâ·ÂÇÑ´Ù.
static void hprintcolor( HDC hdc, int percent, int x, int y, char *s )
{
	if( s == NULL ) return;

	int slen = strlen( s );
	char *t;
	int c,c1;
	char temps[ FILENAME_MAX];
	static int r,g,b;
	static int oldmp3 = -1, mp3 = -1;
	DWORD bc;
	int sx,sy,ex,ey;

	bc = RGB( 255 * percent/100,255 * percent/100,255 * percent/100 );
	while( *s )
	{
		c=0;
		if( strncmp("<:color:", s, 8) == 0 )
		{
			strncpy( temps, s+8,   3 );	temps[3] = 0;	r = atoi( temps );
			r = r*percent/100;
			strncpy( temps, s+8+3, 3 );	temps[3] = 0;	g = atoi( temps );
			g = g*percent/100;
			strncpy( temps, s+8+6, 3 );	temps[3] = 0;	b = atoi( temps );
			b = b*percent/100;
			bc = RGB( r,g,b );
			s = s+8+9+1;
		}

		if( strncmp( "(O--:", s, 5 ) == 0 )
		{
			strncpy( temps, s+5,   3 );	temps[3] = 0;	sx = atoi( temps );
			strncpy( temps, s+5+3, 3 );	temps[3] = 0;	sy = atoi( temps );
			strncpy( temps, s+5+6, 3 );	temps[3] = 0;	ex = atoi( temps );
			strncpy( temps, s+5+9, 3 );	temps[3] = 0;	ey = atoi( temps ); s = s+5+12+1;

			TextOut( hdc, sx-5, sy-5, "¡Ü ", strlen("¡Ü "));

			extern WORD RGB16( int rr, int rg, int rb );
			Line( Mapx+sx, Mapy+sy, Mapx+ex, Mapy+ey, RGB16(r,g,b) );

			continue;

		}
		else if( strncmp( "(---:", s, 5 ) == 0 )
		{
			strncpy( temps, s+5,   3 );	temps[3] = 0;	sx = atoi( temps );
			strncpy( temps, s+5+3, 3 );	temps[3] = 0;	sy = atoi( temps );
			strncpy( temps, s+5+6, 3 );	temps[3] = 0;	ex = atoi( temps );
			strncpy( temps, s+5+9, 3 );	temps[3] = 0;	ey = atoi( temps ); s = s+5+12+1;

			extern WORD RGB16( int rr, int rg, int rb );
			Line( Mapx+sx, Mapy+sy, Mapx+ex, Mapy+ey, RGB16(r,g,b) );

			continue;

		}
		else if( strncmp( "(-->:", s, 5 ) == 0 )
		{
			strncpy( temps, s+5,   3 );	temps[3] = 0;	sx = atoi( temps );
			strncpy( temps, s+5+3, 3 );	temps[3] = 0;	sy = atoi( temps );
			strncpy( temps, s+5+6, 3 );	temps[3] = 0;	ex = atoi( temps );
			strncpy( temps, s+5+9, 3 );	temps[3] = 0;	ey = atoi( temps ); s = s+5+12+1;

			if( abs( sx - ex ) < abs( sy - ey ) )
			{
				if( sy > ey )	TextOut( hdc, ex-5, ey-5, "¡ã ", strlen("¡ã "));
				else			TextOut( hdc, ex-5, ey-5, "¡å ", strlen("¡å "));
			}
			else
			{
				if( sx < ex )	TextOut( hdc, ex-5, ey-5, "¢º", strlen("¢º "));
				else			TextOut( hdc, ex-5, ey-5, "¢¸", strlen("¢¸ "));
			}

			extern WORD RGB16( int rr, int rg, int rb );
			Line( Mapx+sx, Mapy+sy, Mapx+ex, Mapy+ey, RGB16(r,g,b) );
			continue;
		}
		else if( strncmp( "(play:", s, 6) == 0 )
		{
			strncpy( temps, s+6,   3 );	temps[3] = 0;	mp3 = atoi( temps ); s = s+6+12+1;

			if( oldmp3 != mp3 )
			{
				oldmp3 = mp3;
				sprintf( temps, "./sound/%03d.mp3", mp3 );
				if ( pMusic->OpenFile(temps) )	pMusic->Play();
			}

			continue;
		}
		else if( strncmp( "(stop:", s, 6) == 0 )
		{
			s = s+6+1;

			if( oldmp3 == mp3 )
			{
				pMusic->Stop();

				oldmp3	= -1;
				mp3		=  0;
			}
			continue;
		}

		for( c1=0; (strncmp( s+c1, "<:color:", 8) &&
					strncmp( s+c1, "(O--:", 5) &&
					strncmp( s+c1, "(-->:", 5) &&
					strncmp( s+c1, "(---:", 5) &&
					strncmp( s+c1, "(stop:",6) &&
					strncmp( s+c1, "(play:",6)			) && *(s+c1) != '\0' ; c1++ )	temps[c1] = *( s+c1); temps[c1] = 0;
		s = s+c1;
#ifndef _SDL
		SetTextColor(hdc, bc);
		t = (char*)temps;
		TextOut(hdc, x, y, temps, strlen(t));
#else
		SDL_Color _bc = { (char)(bc >> 24), (char)(bc >> 16), (char)(bc >> 8), (char)bc };

		SDL_Hprint(x - 1, y, NULL, temps, _bc, HangulAlign);
		SDL_Hprint(x + 1, y, NULL, temps, _bc, HangulAlign);
		SDL_Hprint(x, y - 1, NULL, temps, _bc, HangulAlign);
		SDL_Hprint(x, y + 1, NULL, temps, _bc, HangulAlign);

		SDL_Color _fc = { HR, HG, HB }; //{ (char)fc, (char)(fc>>8), (char)(fc>>16), (char)(fc>>24) };

		SDL_HprintBold(x, y, NULL, temps, _fc, HangulAlign);
#endif
		x += (6 * strlen(t));

		//SetTextColor( hdc, bc );
		//t = (char *)temps;
		//TextOut( hdc, x, y, temps, strlen( t ));
		//x += (6*strlen(t));
	}
}



void HprintC( int x, int y, char *destbuf, char *temp )
{
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;

	if( lpSurface == NULL ) return;

	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		int			nBkMode;
		nBkMode = ::SetBkMode( hdc, TRANSPARENT );

		HRGN MainHrgn	= CreateRectRgn( Cliping_sx, Cliping_sy, Cliping_ex, Cliping_ey );		// Å¬¸®ÇÎ ¿µ¿ª ¼³Á¤ÇÏ±â..
		HFONT	hFontOld= ( HFONT )SelectObject( hdc, __HFontDial[HangulFontType] );
		HRGN	hGrnOld = ( HRGN )SelectObject( hdc, MainHrgn );

		SetTextColor( hdc, RGB( HR, HG, HB ));
		TextOut( hdc, x, y, temp, strlen( temp ));
		::SetBkMode( hdc, nBkMode );

		SelectObject( hdc, hFontOld );
		SelectObject( hdc, hGrnOld );

		DeleteObject( MainHrgn );
		DeleteObject( __HFontDial[HangulFontType] );

		lpSurface->ReleaseDC( hdc );
	}
}
void Hprint( const int x, const int y, char *destbuf, const char *szStr )
{
	//HDC		hdc;
	//LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;
#ifdef _SDL
	SDL_Color _color = { HR, HG, HB };
	SDL_Hprint(x, y, destbuf, szStr, _color, HangulAlign);
#else
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;

	if (lpSurface == NULL) return;

	if (lpSurface->GetDC(&hdc) == DD_OK)
	{
		int			nBkMode;
		nBkMode = ::SetBkMode(hdc, TRANSPARENT);

		SelectObject(hdc, __HFontDial[HangulFontType]);

		SetTextColor(hdc, RGB(HR, HG, HB));
		SetTextAlign(hdc, HangulAlign);
		TextOut(hdc, x, y, szStr, strlen(szStr));
		::SetBkMode(hdc, nBkMode);

		lpSurface->ReleaseDC(hdc);
	}
#endif
	/*if( lpSurface == NULL ) return;

	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		int			nBkMode;
		nBkMode = ::SetBkMode( hdc, TRANSPARENT );

		SelectObject( hdc, __HFontDial[HangulFontType] );

		SetTextColor( hdc, RGB( HR, HG, HB ));
		SetTextAlign( hdc, HangulAlign );
		TextOut( hdc, x, y, szStr, strlen( szStr ));
		::SetBkMode( hdc, nBkMode );

		lpSurface->ReleaseDC( hdc );
	}*/
}







void HprintC2( int x, int y, char *destbuf, char *s, ... )
{
    BYTE temp[ 1000]={0,};
	char *txt;

    va_list arg;
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;

	if( lpSurface == NULL ) return;

    va_start( arg, s );
	if (arg == NULL)
	{
		sprintf((char *)temp, "%s", s);
	}
	else
	{
		vsprintf( (char *)temp, s, arg );
	}
    va_end( arg );

	txt = (char *)temp;


	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		int			nBkMode;
		nBkMode = ::SetBkMode( hdc, TRANSPARENT );

		SelectObject( hdc, __HFontDial[HangulFontType] );

		SetTextColor( hdc, RGB( HR, HG, HB ));
		TextOut( hdc, x, y, txt, strlen( txt ));
		::SetBkMode( hdc, nBkMode );

		lpSurface->ReleaseDC( hdc );
	}
}

void Hprint2( const int x, const int y, char *destbuf, char *s,... )
{//020828 lsw
    BYTE temp[ 1000]={0,};
    va_list arg;
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;
#ifdef _SDL
	SDL_Color _color = { HR, HG, HB };
	SDL_Hprint(x, y, destbuf, s, _color, HangulAlign);
#else // _SDL

	if( lpSurface == NULL ) return;

    va_start( arg, s );
	if (arg == NULL)
	{
		sprintf((char *)temp, "%s", s);
	}
	else
	{
		vsprintf( (char *)temp, s, arg );
	}
    va_end( arg );

	const char *szStr = (char *)temp;

	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		int			nBkMode;
		nBkMode = ::SetBkMode( hdc, TRANSPARENT );

		::SelectObject( hdc, __HFontDial[HangulFontType] );

		::SetTextAlign( hdc, HangulAlign );
		::SetTextColor( hdc, RGB( HR, HG, HB ));
		::TextOut( hdc, x, y, szStr, ::strlen( szStr ));
		::SetBkMode( hdc, nBkMode );
		lpSurface->ReleaseDC( hdc );
	}
#endif
}

/*
	Added by Tya:

		This function returns the width of the printed text
*/
LONG GetTextWidth(char *s, int length, ...)
{
    BYTE temp[ 1000]={0,};
    va_list arg;
	SIZE sz;
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;

	if( lpSurface == NULL ) return 0;

    va_start( arg, length );
	if (arg == NULL)
	{
		sprintf((char *)temp, "%s", s);
	}
	else
	{
		vsprintf( (char *)temp, s, arg );
	}
    va_end( arg );

	const char *szStr = (char *)temp;

	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		::SelectObject( hdc, __HFontDial[HangulFontType] );

		::SetTextAlign( hdc, HangulAlign );
		::GetTextExtentPoint32( hdc, szStr, length,&sz);
		lpSurface->ReleaseDC( hdc );
	}
	return sz.cx;
}

void TextOutInRect(char *text, int xBase, int yBase, int width, int height)
{
	char ch;
	int x;
	int y = 0;
	char *beginText = text;
	int len = 0;

	SIZE sz;
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;

	if( lpSurface == NULL ) return;

	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		int			nBkMode;
		nBkMode = ::SetBkMode( hdc, TRANSPARENT );
		::SelectObject( hdc, __HFontDial[HangulFontType] );
		::SetTextColor( hdc, RGB( 255, 255, 255 ));
		::SetTextAlign( hdc, HangulAlign );
		while(1)
		{
			ch = *text++;
			if(!ch)
				break;

			if(ch == ' ' && x == 0)
			{
				len++;
				continue;			// ignore spaces at the beginning of a line.
			}

			::GetTextExtentPoint32( hdc, beginText, len,&sz);

			x = sz.cx;

			if(ch != '\n')
				::TextOut( hdc, x+xBase, y+yBase, &ch, 1);

			if(ch == '\n' || x >= width-3)
			{
				y += height;
				len = 0;
				beginText = text;
			}
			else
				len++;
		}
		::SetBkMode( hdc, nBkMode );
		lpSurface->ReleaseDC( hdc );
	}
}


void GetCursorLocation(char *text, int length, int &x, int &y, int width, int height)
{
	char ch;
	char *beginText = text;
	int len = 0;

	SIZE sz;
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;

	x=0;
	y=0;

	length++;

	if( lpSurface == NULL ) return;

	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		::SelectObject( hdc, __HFontDial[HangulFontType] );
		while(length--)
		{
			ch = *text++;
			if(ch == ' ' && x == 0)
			{
				len++;
				continue;			// ignore spaces at the beginning of a line.
			}

			::GetTextExtentPoint32( hdc, beginText, len,&sz);

			x = sz.cx;

			if(!ch)
				break;			// shouldn't happen since it's the cursor pos.. But whatever :D

			if(ch == '\n' || x >= width-3)
			{
				y += height;
				len = 0;
				beginText = text;
				x=0;
			}
			else
				len++;
		}
		lpSurface->ReleaseDC( hdc );
	}
}

void MakeCursorUpDown(char *text, int &newCurPos, bool up)
{
	int xTarget,yTarget;
	int x,y;
	int width,height;
	char *beginText = text;
	int max = strlen(text);


	width = 315;		/* hardcoded.. If the mail system changes width, need to update this value. */
	height = 15;

	x=0;
	y=0;

	GetCursorLocation(text,newCurPos,xTarget,yTarget,width,height);

	newCurPos=0;

	while(1)
	{
		if(newCurPos >= max)
			break;
		GetCursorLocation(text,newCurPos,x,y,width,height);
		if(up)
		{
			if(y >= yTarget - height && x >= xTarget)
			{
				//newCurPos--;					// we stop if the line is good and the X pos is going to be too high next char s
				break;		
			}	
			if(y > yTarget)
			{									// if the Y pos is too high already, we position at the last char of last line.
				newCurPos--;
				break;
			}
		}
		else
		{
			if(y >= yTarget + height && x >= xTarget)
			{
				//newCurPos--;					// we stop if the line is good and the X pos is going to be too high next char s
				break;		
			}	
			if(y >= yTarget + 2*height)
			{									// if the Y pos is too high already, we position at the last char of last line.
				newCurPos--;
				break;
			}
		}
		newCurPos++;
	}
}

void HprintBold( const int x, const int y, const int fc, const int bc, char *s, ... )
{//020828 lsw
    BYTE temp[ 1000]={0,};

    va_list arg;
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;

	if( lpSurface == NULL ) return;

    va_start( arg, s );
	if (arg == NULL)
	{
		sprintf((char *)temp, "%s", s);
	}
	else
	{
		vsprintf( (char *)temp, s, arg );
	}
    va_end( arg );

	const char *txt = (char *)temp;
#ifdef _SDL2
	SDL_Color _bc = { (char)(bc >> 24), (char)(bc >> 16), (char)(bc >> 8), (char)bc };

	SDL_Hprint(x - 1, y, NULL, szStr, _bc, HangulAlign);
	SDL_Hprint(x + 1, y, NULL, szStr, _bc, HangulAlign);
	SDL_Hprint(x, y - 1, NULL, szStr, _bc, HangulAlign);
	SDL_Hprint(x, y + 1, NULL, szStr, _bc, HangulAlign);

	SDL_Color _fc = { (char)fc, (char)(fc >> 8), (char)(fc >> 16), (char)(fc >> 24) };

	SDL_HprintBold(x, y, NULL, szStr, _fc, HangulAlign);
#else
	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		int			nBkMode;
		HFONT	hFontOld = ( HFONT )SelectObject( hdc, __HFontDial[HangulFontType] );
		nBkMode = ::SetBkMode( hdc, TRANSPARENT );
		const int iStringSize = strlen( txt );

		::SetTextAlign( hdc, HangulAlign );
		if(fc == RGB(0, 0, 0))
		{
			::SetTextColor(hdc, RGB(70, 70, 70));
		}
		else
		{
			::SetTextColor( hdc, bc );
		}
		::TextOut( hdc, x-1, y, txt, iStringSize);
		::TextOut( hdc, x+1, y, txt, iStringSize);
		::TextOut( hdc, x, y-1, txt, iStringSize);
		::TextOut( hdc, x, y+1, txt, iStringSize);
		::SetTextColor( hdc, fc );
		::TextOut( hdc, x, y, txt, iStringSize);

//		::SetBkMode( hdc, nBkMode );
		SelectObject( hdc, hFontOld );
//		DeleteObject( __HFontDial[HangulFontType] );

		lpSurface->ReleaseDC( hdc );
	}
#endif
}


void HprintBoldcolor( int x, int y, int percent, char *s, ... )
{
    BYTE temp[ 1000]={0,};
	char *txt;

    va_list arg;
	HDC		hdc;
	LPDIRECTDRAWSURFACE	lpSurface = g_DirectDrawInfo.lpDirectDrawSurfaceBack;

	if( lpSurface == NULL ) return;

    va_start( arg, s );
	if (arg == NULL)
	{
		sprintf((char *)temp, "%s", s);
	}
	else
	{
		vsprintf( (char *)temp, s, arg );
	}
    va_end( arg );

	txt = (char *)temp;

	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		int			nBkMode;
		HFONT	hFontOld = ( HFONT )SelectObject( hdc, __HFontDial[HangulFontType] );
		nBkMode = ::SetBkMode( hdc, TRANSPARENT );

		hprintcolor( hdc, percent, x, y, txt );

//		::SetBkMode( hdc, nBkMode );
		SelectObject( hdc, hFontOld );
//		DeleteObject( __HFontDial[HangulFontType] );

		lpSurface->ReleaseDC( hdc );
	}
}

/*¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
 ÇÔ¼ö¸í : ±×³É Ãâ·ÂÇÑ´Ù... Hprint2´Â ...ÀÎÀÚ¸¦ ¹Þ°í, Hprint´Â String¸¸ ¹Þ´Â´Ù.
 ÀÎ  ÀÚ : x,y = Ãâ·ÂÇÒ ¹®ÀÚÀÇ À§Ä¡,
          offset = È­¸é»ó¿¡ Ãâ·ÂÇÒ ºê°¡ÀÇ ¿É¼Â
          s = Ãâ·ÂÇÒ ¹®ÀÚ
 ¸®ÅÏ°ª : ¾øÀ½
 ¼³  ¸í : x,y À§Ä¡¿¡ ¹®ÀÚ¸¦ Ãâ·ÂÇÑ´Ù.
¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡*/
/*void Hprint2( int x, int y, char *destbuf, char *s, ... )
{
    BYTE temp[ 1000]={0,};
    va_list arg;

    va_start( arg, s );
    vsprintf( (char *)temp, s, arg );
    va_end( arg );

	Hprint( x, y, destbuf, (char *)temp );
}*/





void SetHangulFont( int type )
{
	HangulFontType = type;
}


void SetHangulAlign( int type )
{
	HangulAlign = type;
}

// thai2 YGI
bool IsHangul( char check )
{	//< CSD-030324
#ifdef THAI_LOCALIZING_
	return false;
#endif

	if( check & 0x80 ) return true;
	return false;
}	//> CSD-030324























