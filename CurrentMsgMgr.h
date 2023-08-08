// CurrentMsgMgr.h: interface for the CCurrentMsgMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURRENTMSGMGR_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_)
#define AFX_CURRENTMSGMGR_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define NUM_STATUSMESSAGE    30
#define STATUSMESSAGE_X		 10
#define STATUSMESSAGE_Y		 120
	
typedef struct tagCurrentStatusMessage
{	
	char		msg[MAX_PATH+1];
	int			x, y;
	int			delay;
	COLORREF	dwColor;
}CURRENTSTATUSMESSAGE;
class CCurrentMsgMgr  
{
public:
	CCurrentMsgMgr();
	virtual ~CCurrentMsgMgr();
	void AddString( const WORD wColor, char *msg, ... );
	void AddString( const int R, const int G, const int B, char *msg, ... );
	void Clear();
	void ViewCurrentStatusMessage();
	void CheckCurrentStatusMessage();

private:
	char m_szTempData[MAX_PATH];
	int m_iCurStsMsgCount;
	int	m_iCSMCounter;
	int m_iCheckCounter;
	CURRENTSTATUSMESSAGE  CurStsMsg[NUM_STATUSMESSAGE];

private:
	CURRENTSTATUSMESSAGE* GetCurStsMsg(const int iIndex)
	{
		if(0 > iIndex || NUM_STATUSMESSAGE <= iIndex){return NULL;} 
		return &CurStsMsg[iIndex];
	}
};
extern CCurrentMsgMgr  CurrentMsgMgr;
extern void AddCurrentStatusMessage( const int R, const int G, const int B, char *msg, ... );
extern void AddCurrentStatusMessage( const WORD wColor, char *msg, ... );
#endif // !defined(AFX_CURRENTMSGMGR_H__589944BD_DCE5_4B05_9D6B_82C08591AC88__INCLUDED_)
