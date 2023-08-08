// ExchangeMgr.h: interface for the CExchangeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCHANGEMGR_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
#define AFX_EXCHANGEMGR_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum eVersusInterFaceFieldNo
{
	VS_LEFT_DISPLAY_FIELD_NO  = 0,
	VS_RIGHT_DISPLAY_FIELD_NO  = 1,
};

enum ExchangeType
{
	EXCHANGE_TYPE_NO = 0,//거래아님 
	EXCHANGE_TYPE_NORMAL = 1,		//보통 거래
	EXCHANGE_TYPE_MERCHANT_DIRECT = 2,	//상인과 직접 거래
};

enum ExchangeStateType
{
	EXCHANGE_STATE_READY = 0,//거래 대기 상태
	EXCHANGE_STATE_EXCHANGE_NOW  ,	//거래중 상태 GetExchangeState에서 씀
};

class CExchangeMgr  
{
public:
	CExchangeMgr();
	virtual ~CExchangeMgr();
private:
	int m_iExchageType;//거래 타입

	int m_iExchageServerConformState;//지금 서버에게허가 받고 있는 상태야
	int m_iExchageState;//거래중인 상태
	int m_iExchageTargetId;//거래중인 상대의 cn 값
	bool m_bMySideConformState;//상대방이 거래 Ok 사인을 보냈는가.(거래인터페이스 닫히거나 열릴때 무조건 0 으로 만들어 줘라.
	bool m_bOtherSideConformState;//상대방이 거래 Ok 사인을 보냈는가.(거래인터페이스 닫히거나 열릴때 무조건 0 으로 만들어 줘라.
	char m_szTargetName[31];
	
public:
	bool	m_bIsIAmMerchant;
	int		GetExchangeType()const{return m_iExchageType;}// 거래 상대의 cn을 리턴
	void	SetExchangeType(const int iType){m_iExchageType = iType;}// 거래 상대의 cn을 리턴

	int		GetExchangeTargetId()const{return m_iExchageTargetId;}// 거래 상대의 cn을 리턴
	void	SetExchangeTargetId(const int iCn)
	{
		LPCHARACTER	pCh = ::FindCharacter( &g_CharacterList, iCn);
		if( pCh )
		{
			m_iExchageTargetId = iCn;
			SetExchangeTargetName( pCh->name );
		}
		else
		{
			::CallServer( CMD_EXCHANGE_CANCEL );
		}
	}// 거래 상대의 cn을 리턴

	void 	SetOtherSideConformState(const bool bIsOkSign)	{m_bOtherSideConformState = bIsOkSign;}
	void	SetMySideConformState(const bool bIsOkSign)		{m_bMySideConformState = bIsOkSign;}

	bool	IsOtherSideConformStateOk()const	{return m_bOtherSideConformState;}
	bool	IsMySideConformStateOk()const	{return m_bMySideConformState;}

	void	SetExchangeTargetName( const char* szName )
	{
		::sprintf(m_szTargetName,"%s",szName);
	}
	char*	GetExchangeTargetName()
	{
		return m_szTargetName;
	}
	
	void	SetExchangeState(const int iValue){m_iExchageState = iValue;}
	int		GetExchangeState()const{return m_iExchageState;}//now Exchage State

	void	SendExchangeResult();
	void	SendExchangeStateSign(const bool bOkSign);//030108 lsw
	void	CallExchangeMenu(const bool bIsRecvSide, const int iExchageType);

	void Clear();
	void ClearItemData();

};
extern CExchangeMgr ExchangeMgr;

const int MAX_EXCHANGE_ITEM = 20;
extern DefaultItem item_give[MAX_EXCHANGE_ITEM];
extern DefaultItem item_recv[MAX_EXCHANGE_ITEM];

extern bool IsExchangeNow();

extern void SendExchangeItemWithOtherCh( short int you_id ,const int iExchangeType);
extern void RecvExchangeItemReq( k_exchange_item_start *exchange_item );
extern void SendExchangeResult();		// 서버 전송에 따른 에러 메시지 처리
extern void RecvExchangeItemOk( k_exchange_item_start *exchange_item );
extern void RecvExchangeItemResult( int ret );		// 아이템 교환을 위한 준비 (서버 전송에 관한 결과 )
extern void RecvExchangeCancel();
extern void RecvExchange();
extern void RecvExchangeEach( const K_ITEM *lpItem );
extern void SendExchangeItemEach( ItemAttr &item, POS &pos, short int you_id );
extern void SendExchangeItemDel( POS pos, short int you_id );
extern void SetExchageItemData(const int iExchangeIndex,ItemAttr &item, POS &pos,const bool bSendToServer = false);//아이템 데이타 셋팅
extern void CheckFaultExchageItemList();
extern void DisplayExchangeItemList(const int iWillDo, const int iIsRectOn);
extern void DeleteItemAtExchangeGiveItemList(const int iWillDo);
#endif // !defined(AFX_EXCHANGEMGR_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
