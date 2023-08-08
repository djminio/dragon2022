// TextOutMgr.h: interface for the CTextOutMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTOUTMGR_H__D3413C1D_4433_4E1B_9D49_EBB7EF293965__INCLUDED_)
#define AFX_TEXTOUTMGR_H__D3413C1D_4433_4E1B_9D49_EBB7EF293965__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//020828-2 lsw
const int TXT_BG_SHORT_WIDTH	= 140;//150 길이 
const int TXT_BG_NORMAL_WIDTH	= 192;//200 길이
const int TXT_BG_LONG_WIDTH		= 240;//250 길이
const int TXT_BG_VERY_LONG_WIDTH= 290;//300 길이


const int COLOR_BLACK		= 0;
const int COLOR_BLUE		= 1;
const int COLOR_RED			= 2;
const int COLOR_BLUEGREEN	= 3;
const int COLOR_BROWN		= 4;
const int COLOR_GREEN		= 5;

const int DEFAULT_ALPHA_VALUE	= 23;

const int DEFAULT_LINE_GAB		= 14;
const int BG_HEIGHT				= 14;//14가 안바뀌게 하세요//배경 찍히는거 때문에 그렇습니다.

const int CHAT_BG_HEIGHT			= 24;
const int MERCHANT_CHAT_BG_HEIGHT	= 18;

class CTextOutMgr  
{
public:
	CTextOutMgr();
	virtual ~CTextOutMgr();
private:
	int m_iBlankGabSize;
	int m_iLineGab;//줄간격
	int m_iAlpha;
	int m_iBlendType;

	int m_iImgGroup;
	int m_iHeadImgNo;
	int m_iBodyImgNo;
	int m_iTailImgNo;
	int m_iBlkImgNo;

public:
	int SetBgImgColor(const int iColor);
	void SetAlphaValue(const int iValue);

	int Init(const WORD iColor, const int iBGWidth, const int iAlphaValue = DEFAULT_ALPHA_VALUE);//021001 lsw
	
	int GetAlpha(){return m_iAlpha;}
	int GetBlendType(){return m_iBlendType;}

	int GetLineGab(){return m_iLineGab;}

	int GetBlankGabSize(){return m_iBlankGabSize;}

	int	RcTXTOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...);
	int	RcTXTOutB(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...);

	int	RcTXTOutNoBG(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...);//020821-2 lsw
	int	RcTXTOutBNoBG(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,char* msg,...);//020821-2 lsw
	
	int	RcBlankOut(const int iExplainX, const int iExplainY, const int iWidth,const int iBlankCount);
	
	void PutTextHeaderBGImg(const int iExplainX, const int iExplainY, const int iWidth);//그림 위에 테두리.
	void PutTextTailBGImg(const int iExplainX, const int iExplainY, const int iWidth);//그림 아래 테두리.

	int  RcChatHeaderOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab,const int iChatColorType,char* msg,...);
	int	 RcChatOut(const int iExplainX, const int iExplainY, const int iWidth, const int iGab, const int iChatColorType, char* msg,...);
};
extern CTextOutMgr TxtOut;
#endif // !defined(AFX_TEXTOUTMGR_H__D3413C1D_4433_4E1B_9D49_EBB7EF293965__INCLUDED_)
