// TextOutMgr.cpp: implementation of the CTextOutMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <ddraw.h>
#include "Hangul.h"
#include "dragon.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "SmallMenuSet.h"
#include "stdio.h"
#include "directsound.h"
#include "directinput.h"
#include "Char.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "skill.h"
#include "Item.h"
#include "MenuNetwork.h"
#include "MouseCursor.h"
#include "CharDataTable.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "skill_lsw.h"
#include "menudef.h"
#include "menudef2.h"
#include "TextOutMgr.h"
#include "Chat.h"
extern int HR, HB, HG;

// THAI YGI
ThaiBreakIterator TBreak((unsigned char*) "ftwbrk.lex"); // CSD-030324

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTextOutMgr TxtOut ;
CTextOutMgr::CTextOutMgr()
{
	m_iBlankGabSize = 3;
	m_iLineGab		= DEFAULT_LINE_GAB;
	SetAlphaValue(23);
	m_iBlendType	= 1;
	
	Init(COLOR_BLACK,TXT_BG_NORMAL_WIDTH);
}

void CTextOutMgr::SetAlphaValue(const int iValue)
{
	m_iAlpha	= iValue;
}

int CTextOutMgr::SetBgImgColor(const int iColor)
{
	switch(iColor)
	{
	case COLOR_BLACK :
		{
			m_iImgGroup = TXT_BG_BLACK_IMG;
		}break;
	case COLOR_BLUE:
		{
			m_iImgGroup = TXT_BG_BLUE_IMG;
		}break;
	case COLOR_RED:
		{
			m_iImgGroup = TXT_BG_RED_IMG;
		}break;
	case COLOR_BLUEGREEN:
		{
			m_iImgGroup = TXT_BG_BLUEGREEN_IMG;
		}break;
	case COLOR_BROWN:
		{
			m_iImgGroup = TXT_BG_BROWN_IMG;
		}break;
	case COLOR_GREEN:
		{
			m_iImgGroup = TXT_BG_GREEN_IMG;
		}break;
	default:
		{
			m_iImgGroup = TXT_BG_BLACK_IMG;
			return 0;
		}break;
	}
	return 1;
}

CTextOutMgr::~CTextOutMgr()
{
}

int CTextOutMgr::Init(const WORD iColor, const int iBGWidth, const int iAlphaValue)
{//라인 갭을 바꿔봤자 백그라운드가 찍히는것은 라인 갭이 조절 될수 없습니다. 이유는 갭을 찍어야 하기 때문입니다.
	SetBgImgColor(iColor);
	SetAlphaValue(iAlphaValue);
	switch(iBGWidth)
	{
	case TXT_BG_SHORT_WIDTH://150 길이 
		{
			m_iHeadImgNo = 17;
			m_iBodyImgNo = 15;	
			m_iTailImgNo = 16;
			m_iBlkImgNo	 = 5;
		}break;
	case TXT_BG_LONG_WIDTH://250 길이
		{
			m_iHeadImgNo = 23;
			m_iBodyImgNo = 21;	
			m_iTailImgNo = 22;
			m_iBlkImgNo	 = 7;
		}break;
	case TXT_BG_VERY_LONG_WIDTH://300 길이
		{
			m_iHeadImgNo = 26;
			m_iBodyImgNo = 24;	
			m_iTailImgNo = 25;
			m_iBlkImgNo	 = 8;
		}break;
	case TXT_BG_NORMAL_WIDTH://200 길이
	default:
		{
			m_iHeadImgNo = 20;
			m_iBodyImgNo = 18;	
			m_iTailImgNo = 19;
			m_iBlkImgNo	 = 6;
		}break;
	}
	return 0;
}

int	CTextOutMgr::RcTXTOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...)
{
	char temp[ MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf( temp, msg, arg );
	}
    va_end( arg );

	const int line	= GetLineRectTextPut( iWidth-iGab, temp);
	for( int i = 0; i<line; i++ )
	{	// m_iImgGroup
		FieldTypeNomalPutFx3( iExplainX-4, iExplainY+BG_HEIGHT*i-1, 0,0,m_iBodyImgNo , m_iImgGroup,GetAlpha(), GetBlendType() );
	}
	RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY, iWidth-iGab, temp,BG_HEIGHT);
	return line;
}

int	CTextOutMgr::RcTXTOutNoBG(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...)
{
	char temp[ MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf(temp, msg, arg );
	}
    va_end( arg );

	const int line	= GetLineRectTextPut( iWidth-iGab, temp);
	RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY, iWidth-iGab, temp,GetLineGab());
	return line;
}

int	CTextOutMgr::RcTXTOutB(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...)
{
	char temp[ MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf(temp, msg, arg );
	}
    va_end( arg );

	const int line	= GetLineRectTextPut( iWidth-iGab, temp);
	for( int i = 0; i<line; i++ )
	{	// m_iImgGroup
		FieldTypeNomalPutFx3( iExplainX-4, iExplainY+BG_HEIGHT*i-1, 0,0, m_iBodyImgNo, m_iImgGroup,GetAlpha(), GetBlendType() );
	}
	RectBoldTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY, iWidth-iGab, temp,BG_HEIGHT);//020823 lsw
	return line;
}

int	CTextOutMgr::RcTXTOutBNoBG(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...)
{
	char temp[ MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf(temp, msg, arg );
	}
    va_end( arg );

	const int line	= GetLineRectTextPut( iWidth-iGab, temp);
	RectBoldTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY, iWidth-iGab, temp,GetLineGab());//020823 lsw
	return line;
}

int CTextOutMgr::RcChatHeaderOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,const int iChatColorType,char* msg,...)
{
	char temp[ MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf(temp, msg, arg );
	}
    va_end( arg );

	const int iGabY = 10;
	switch(iChatColorType)
	{
	case CHAT_COLOR_TYPE_DEFAULT:
		{
			::PutCompressedImageFX( iExplainX+ spr[72].ox,	iExplainY+spr[72].oy+2,	&spr[72], GetAlpha(), 1 );//보통
		}break;
	case CHAT_COLOR_TYPE_SIT_DOWN:
		{
			::PutCompressedImageFX( iExplainX+ 96,			iExplainY+11,			chatimage[iChatColorType], GetAlpha(), 1 );//커스텀
		}break;
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			::FieldTypeNomalPutFx3( iExplainX,				iExplainY-12, 0,0,0,		MERCHANT_BBS_BACK_IMG, GetAlpha(), GetBlendType() ); //상인
		}break;
	default:
		{
			::PutCompressedImageFX( iExplainX+ 96,			iExplainY+11,			chatimage[iChatColorType], GetAlpha(), 1 );//커스텀
		}break;
	}
	
	switch(iChatColorType)
	{
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			RectTextPutNotEatWhiteChar(iExplainX+iGab+60,iExplainY+iGabY-14, iWidth-iGab, lan->OutputMessage(4,562),BG_HEIGHT);
		}break;
	default:
		{
			RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY+iGabY, iWidth-iGab, temp,BG_HEIGHT);
		}break;
	}
	return 1;
}

int CTextOutMgr::RcChatOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab, const int iChatColorType, char* msg,...)
{
	char temp[ MAX_PATH] = {0,};
    va_list arg;
    va_start( arg, msg );
	if (arg == NULL)
	{
		sprintf(temp, "%s", msg);
	}
	else
	{
		vsprintf(temp, msg, arg );
	}
    va_end( arg );

	const int x = iExplainX;
	const int y = iExplainY;
	
	const int iLG = 18;
	const int iGabY = 4;
	
	const int iLCt = GetLineRectTextPut( iWidth-iGab, temp);

	int line  =0;
	
	switch(iChatColorType)
	{
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			line = (((iLCt*iLG)+iGabY) /MERCHANT_CHAT_BG_HEIGHT);
		}break;
	default:
		{
			line = (((iLCt*iLG)+iGabY) /CHAT_BG_HEIGHT)+1;
		}break;
	}
	for(int i = line; 0 < i; i--)
	{	
		switch(iChatColorType)
		{
		case CHAT_COLOR_TYPE_DEFAULT:
			{
				if(1==i)//마지막 줄일때.
				{
					::PutCompressedImageFX( x+spr[70].ox-6, y+spr[ 70].oy -(CHAT_BG_HEIGHT*i), &spr[ 70], GetAlpha(), 1 ); //풍선끝
				}
				else
				{
					::PutCompressedImageFX( x+spr[71].ox, y+spr[ 71].oy -(CHAT_BG_HEIGHT*i), &spr[ 71], GetAlpha(), 1 ); 
				}				
			}break;
		case CHAT_COLOR_TYPE_SIT_DOWN:
			{
				if(1==i)//마지막 줄일때.
				{
					::PutCompressedImageFX( x+98, y+13-(CHAT_BG_HEIGHT*i), chatimage[iChatColorType+2], GetAlpha(), 1 ); 
				}
				else
				{
					::PutCompressedImageFX( x+96, y+10-(CHAT_BG_HEIGHT*i), chatimage[iChatColorType+1], GetAlpha(), 1 ); 
				}
			}break;
		case CHAT_COLOR_TYPE_MERCHANT_BBS://상인만 채팅 바닥이 다릅니다.
			{
				::FieldTypeNomalPutFx3( x, y-(MERCHANT_CHAT_BG_HEIGHT*i), 0,0,1,MERCHANT_BBS_BACK_IMG, GetAlpha(), GetBlendType() ); 
				if(1==i)//마지막 줄일때.
				{
					::FieldTypeNomalPutFx3( x, y-(MERCHANT_CHAT_BG_HEIGHT*(i-1)), 0,0,2,MERCHANT_BBS_BACK_IMG, GetAlpha(), GetBlendType() ); 
				}
			}break;
		default:
			{
				if(1==i)//마지막 줄일때.
				{
					::PutCompressedImageFX( x+93, y+13-(CHAT_BG_HEIGHT*i), chatimage[iChatColorType+2], GetAlpha(), 1 ); 
				}
				else
				{
					::PutCompressedImageFX( x+96, y+10-(CHAT_BG_HEIGHT*i), chatimage[iChatColorType+1], GetAlpha(), 1 ); 
				}
			}break;
		}
	}
	switch(iChatColorType)
	{
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY-(line*MERCHANT_CHAT_BG_HEIGHT)+iGabY, iWidth-iGab, temp,iLG);
		}break;
	default:
		{
			RectTextPutNotEatWhiteChar(iExplainX+iGab,iExplainY-(line*CHAT_BG_HEIGHT)+iGabY, iWidth-iGab, temp,iLG);
		}break;
	}
	return line;
}

int	CTextOutMgr::RcBlankOut(const int iExplainX, const int iExplainY, const int iWidth,const int iBlankCount)
{
	for( int i = 0; i<iBlankCount; i++ )
	{
		FieldTypeNomalPutFx3( iExplainX-4, iExplainY+m_iBlankGabSize*i-1, 0,0, m_iBlkImgNo	, m_iImgGroup,GetAlpha(), GetBlendType() );
	}
	return iBlankCount*m_iBlankGabSize;
}

void CTextOutMgr::PutTextHeaderBGImg(const int iExplainX, const int iExplainY, const int iWidth)
{	//-5 는 헤더 테두리 사이즈
	FieldTypeNomalPutFx3( iExplainX-4, iExplainY-1-5, 0,0, m_iHeadImgNo, m_iImgGroup,GetAlpha(), GetBlendType() );
}

void CTextOutMgr::PutTextTailBGImg(const int iExplainX, const int iExplainY, const int iWidth)
{
	FieldTypeNomalPutFx3( iExplainX-4, iExplainY-1, 0,0, m_iTailImgNo, m_iImgGroup,GetAlpha(), GetBlendType() );
}

void RectPrintModul( int x, int y, char *pStr, int size, bool bBold, bool bEat )
{	//< CSD-030324
	char szText[500];
	strncpy( szText, pStr, size );
	szText[size] = 0;
	if( bBold )
	{
		HprintBold( x, y, RGB( HR, HG, HB ),RGB(0,0,0),"%s",bEat?Kein_EatFrontWhiteChar(szText):szText);
	}
	else
	{
		Hprint2( x, y, NULL,"%s", bEat?Kein_EatFrontWhiteChar(szText):szText );
	}
}	//> CSD-030324

int RectTextPutBasic ( 
	const int x, const int y,
	int nWidth, char *sSource, 
	const int hight, const int line_max,
	bool bBold,
	bool bEat,
	char **pEnd
)
{	//< CSD-030324
	if( !sSource ) return 0;
	if( nWidth <= 0  ) nWidth = 100;
	int nLenghtTotal = 0,nLenghtStart = 0,nLenghtEnd = 0,nLine = 0;
	nLenghtTotal=strlen(sSource);

	int gab;

	// THAI YGI
#ifdef THAI_LOCALIZING_
	BYTE rgbBrk[256] = {0,};
	ThaiBreak( sSource ,&rgbBrk[0]);//020806 lms
	int ct=0;
	int iNow =0;
#endif//#ifdef THAI_LOCALIZING_

	while(1)
	{
		if( line_max && nLine >= line_max ) 
		{
			if( pEnd ) *pEnd = sSource+nLenghtStart;
			return nLine+1;
		}
		
		gab = 1;
		if( IsHangul( *(sSource+nLenghtEnd) ) ) { gab = 2; }

		nLenghtEnd+=gab;
		if( nLenghtEnd > nLenghtTotal)
		{
			nLenghtEnd-=gab;
			if(hight) RectPrintModul( x, y+nLine*hight, sSource+nLenghtStart, nLenghtEnd-nLenghtStart, bBold, bEat);
			nLenghtStart=nLenghtEnd;
			break;
		}
		if( (nLenghtEnd-nLenghtStart)*FONTSIZE_ > nWidth)
		{
			nLenghtEnd-=gab;
#ifdef THAI_LOCALIZING_
				for(; 255 > ct; ct++)
				{
					iNow += rgbBrk[ct];
					if(iNow > (nLenghtEnd))
					{
						if((rgbBrk[ct])*FONTSIZE_ <= nWidth )
						{
							iNow -=rgbBrk[ct];//단어의 시작점
						}
						else 
						{
							ct++;
						}
						nLenghtEnd = iNow;
						break;
					}
					else if(iNow == (nLenghtEnd))
					{
						ct++;
						break;
					}
				}
#endif//ifdef THAI_LOCALIZING_
			if(hight) RectPrintModul( x, y+nLine*hight, sSource+nLenghtStart, nLenghtEnd-nLenghtStart, bBold, bEat );
			nLenghtStart=nLenghtEnd;
			nLine++;
			continue;
		}
		if( ( gab==1) && ( *(sSource+nLenghtEnd) == '\n') )
		{
			if(hight) RectPrintModul( x, y+nLine*hight, sSource+nLenghtStart, nLenghtEnd-nLenghtStart, bBold, bEat );
			nLenghtStart=nLenghtEnd;				
			nLine++;
			continue;
		}
	}
	return nLine+1;
}	//> CSD-030324

int RectTextPutNotEatWhiteChar(const int x, const int y,int nWidth, char *sSource, const int hight, const int line_max)//
{	//< CSD-030324
	return RectTextPutBasic( x, y, nWidth, sSource, hight, line_max, false, false, NULL );
}	//> CSD-030324

int RectTextPut(const int x, const int y,int nWidth, char *sSource, const int hight, const int line_max)
{	//< CSD-030324
	return RectTextPutBasic( x, y, nWidth, sSource, hight, line_max, false, true, NULL  );
}	//> CSD-030324

int RectBoldTextPutNotEatWhiteChar(const int x, const int y,int nWidth, char *sSource, const int hight, const int line_max)//
{	//< CSD-030324
	return RectTextPutBasic( x, y, nWidth, sSource, hight, line_max, true, false, NULL );
}	//> CSD-030324

int RectBoldTextPut(const int x, const int y,int nWidth, char *sSource, const int hight, const int line_max)//
{	//< CSD-030324
	return RectTextPutBasic( x, y, nWidth, sSource, hight, line_max, true, true, NULL );
}	//> CSD-030324

int GetRectTextLine( char *sSource, int nWidth, const int line_max)//020703 lsw
{	//< CSD-030324
	return RectTextPutBasic( 0, 0, nWidth, sSource, 0, line_max, false, false, NULL  );
}	//> CSD-030324

void ThaiBreak( char *msg,BYTE *brk)
{	//< CSD-030324
	TBreak.FindThaiWordBreak(msg, strlen(msg), brk, 256, 0);
}	//> CSD-030324