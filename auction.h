// Auction.h: interface for the CAuction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)
#define AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "itemTable.h"
#include "Network.h"

enum BondMoneyNo
{
	BOND_MONEY_1000 = 10219,	//10억 크릿 본드	
	BOND_MONEY_500	= 10220,	//5억 크릿 본드	
	BOND_MONEY_100	= 10221,	//1억 크릿 본드	
	BOND_MONEY_50	= 10222,	//5천만 크릿 본드	
	BOND_MONEY_10	= 10223,	//1천만 크릿 본드	
	BOND_MONEY_5	= 10224,	//5백만 크릿 본드	
};

typedef struct SUPERMONEY
{
	int iItemNo;
	int iValue; 
	SUPERMONEY(const int iInput,const int iInput2)
	{
		iItemNo =iInput;
		iValue	=iInput2;
	}
}t_SuperMoney,*lpSUPERMONEY;

const int BOND_MONEY_MAX = 6;
extern const SUPERMONEY aBondMoney[BOND_MONEY_MAX];

enum FKType
{
	FK_RARE = 1,
	FK_LV ,
	FK_TACTIC ,
	FK_WEAR ,
};

enum ItemType
{
	NORMAL_ITEM	= 0,
	RARE_ITEM	= 1,
	HIGH_ITEM	= 2,
	DYNAMIC_RARE_ITEM	= 3,
	SET_ITEM	= 4
};

class CAuction  
{
	enum IsEndType
	{
		IS_END_ALL_RIGHT	= 0,	//뭐든 할 수 있는 상태
		IS_END_DELETING		= 1,	//삭제중
		IS_END_BUYING		= 3,	//구매중
		IS_END_WAIT_TAKE	= 4	,	//가져가기를 바라는 상태
		IS_END_DELETE_COMPLETE = 5,	// BBD 040303
		IS_END_GIVING		= 6,	// BBD 040303 지급프로세스에 들어가있다.
	};

	enum ItemLevelType
	{
		LVMIN		= 0,
		LV10TO20	= 1,
		LV20TO30	= 2,
		LV30TO40	= 3,
		LV40TO50	= 4,
		LV50TO60	= 5,
		LV60TO70	= 6,
		LV70TO80	= 7,
		LV80TO90	= 8,
		LV90TO100	= 9,
		LV100TO110	= 10,
		LV110TO120	= 11,
		LV120TO130	= 12,
		LV130TO140	= 13,
		LV140TO150	= 14,
		LVMAX		= 15
	};
public:
	CAuction();
	virtual ~CAuction();
private://검색기용 변수
	int	m_iFKRareType;
	int	m_iFKLevel;
	int	m_iFKTacticType;
	int m_iFKWearType;
	
	int			m_iDeleteItemIndex;//자체 인덱스(SQL)
	int			m_iBuyItemIndex;//자체 인덱스(SQL)
	int			m_iTakeItemIndex;
	int			m_iSellValue;
	POS			m_SellItemPos;
	ItemAttr	m_SellItemAttr;

	int	m_iWaitPageRefresh;

	int	m_iBuyerViewPage;
	int	m_iSellerViewPage;
	int	m_iResultViewPage;

	POS			m_BondItemPos;
	ItemAttr	m_BondItemAttr;

private://검색결과물
	SEARCHRESULTLIST m_SearchList;//구조체 내부가 배열입니다.
	void Clear();
public://판매 아이템 등록용
	int	GetBuyerViewPage()const{return m_iBuyerViewPage;}
	int	GetSellerViewPage()const{return m_iSellerViewPage;}
	int	GetResultViewPage()const{return m_iResultViewPage;}

	int	IncBuyerViewPage() ;
	int	IncSellerViewPage();
	int	IncResultViewPage() ;

	int	DecBuyerViewPage() ;
	int	DecSellerViewPage();
	int	DecResultViewPage() ;

	void SetSellValue(const int iSellValue){m_iSellValue =iSellValue;}

	int SetSellItem(const POS &pos,const ItemAttr Item);	
	POS GetSellItemPos(){return m_SellItemPos;}
	ItemAttr *GetSellItemAttr(){return (m_SellItemAttr.item_no)?&m_SellItemAttr:NULL;}

	int SetBondItem(const POS &pos,const ItemAttr Item);	
	POS GetBondItemPos(){return m_BondItemPos;}
	ItemAttr *GetBondItemAttr(){return (m_BondItemAttr.item_no)?&m_BondItemAttr:NULL;}

	void ClearSellItem()
	{
		m_iSellValue = 0;
		memset(&m_SellItemPos,0,sizeof(POS));
		memset(&m_SellItemAttr,0,sizeof(ItemAttr));
	}

	void ClearBondItem()
	{
		memset(&m_BondItemPos,0,sizeof(POS));
		memset(&m_BondItemAttr,0,sizeof(ItemAttr));
	}
	
	void SetDeleteItemIndex(const int iIndex)
	{
		m_iDeleteItemIndex = iIndex;
	}
	
	void SetBuyItemIndex(const int iIndex)
	{
		m_iBuyItemIndex = iIndex;
	}

	void SetTakeItemIndex(const int iIndex)
	{
		m_iTakeItemIndex = iIndex;
	}	

	int GetDeleteItemIndex()
	{
		return m_iDeleteItemIndex;
	}
	int GetBuyItemIndex()
	{
		return m_iBuyItemIndex;
	}
	int GetTakeItemIndex()
	{
		return m_iTakeItemIndex;
	}
	
public:
	char m_szMerchantName[21];
	char m_szKeyWord[21];
	void DecPeriod(int nDec = 1);
	void IncPeriod(int nInc = 1);
	int m_nPeriod;
	bool m_bChkMerchantName;
	bool m_bChkKeyWord;
	SEARCHRESULTLIST GetSearchList()const{return m_SearchList;}
	void ClearFK()//FK들을 초기화 시킵니다.
	{
		m_iFKRareType	= NORMAL_ITEM;
		m_iFKLevel		= LVMIN;
		m_iFKTacticType = TACTICS_MIN;
		m_iFKWearType	= WEAR_ANYTHING;
	}

	void FKRareTypeInc()//아아템 타입 조절
	{
		if(SET_ITEM <= m_iFKRareType){m_iFKRareType = NORMAL_ITEM;}//현재 노말 아이템이거나 그거랑 같으면
		else{ m_iFKRareType++;}
	}
	void FKRareTypeDec()
	{
		if(NORMAL_ITEM >= m_iFKRareType){m_iFKRareType = SET_ITEM;}//현재 노말 아이템이거나 그거랑 같으면
		else{ m_iFKRareType--;}
	}

	void FKLevelInc()//아아템 레벨 조절
	{
		if(LVMAX <= m_iFKLevel){m_iFKLevel = LVMIN;}//현재 노말 아이템이거나 그거랑 같으면
		else{ m_iFKLevel++;}
	}
	void FKLevelDec()
	{
		if(LVMIN >= m_iFKLevel){m_iFKLevel = LVMAX;}//현재 노말 아이템이거나 그거랑 같으면
		else{ m_iFKLevel--;}
	}

	void FKTacticTypeInc()
	{
		if(TACTICS_MAX <= m_iFKTacticType)
		{
			m_iFKTacticType = TACTICS_START;
		}
		else
		{
			m_iFKTacticType++;
		}
	}
	void FKTacticTypeDec()
	{
		if(TACTICS_START >= m_iFKTacticType)
		{
			m_iFKTacticType = TACTICS_MAX;
		}
		else
		{
			m_iFKTacticType--;
		}
	}

	void FKWearTypeInc()
	{
		if(WEAR_TWO_HAND <= m_iFKWearType){m_iFKWearType = WEAR_ANYTHING;}//현재 노말 아이템이거나 그거랑 같으면
		else{ m_iFKWearType++;}
	}
	void FKWearTypeDec()
	{
		if(WEAR_ANYTHING >= m_iFKWearType){m_iFKWearType = WEAR_TWO_HAND;}//현재 노말 아이템이거나 그거랑 같으면
		else{ m_iFKWearType--;}
	}

	int GetFKRareType()const{return m_iFKRareType;}
	int GetFKLevel()const{return m_iFKLevel;}
	int GetFKTacticType()const{return m_iFKTacticType;}
	int GetFKWearType()const{return m_iFKWearType;}
//---->구매자용
	void SendCMD_MERCHANT_BUY_LIST_REQUEST(const int iIndex = 0, const int iNeedIndexOver = 1);//리스트 요청(구매자가 보는것//검색기가 호출
	void RecvCMD_MERCHANT_BUY_LIST_REQUEST_RESULT(t_packet &p);//리스트 결과(구매자가 보는것

	void SendCMD_MERCHANT_ITEM_BUY(SEARCHRESULT &SR);//구매하기(구매자가 선택
	void RecvCMD_MERCHANT_ITEM_BUY_RESULT(t_packet &p);//구매하기(구매자가 선택
//<----

//---->판매자용
	void SendCMD_MERCHANT_SELL_LIST_REQUEST(const int iIndex = 0, const int iNeedIndexOver = 1);//리스트 요청(판매자가 보는것
	void RecvCMD_MERCHANT_SELL_LIST_REQUEST_RESULT(t_packet &p);//리스트 결과(판매자가 보는것

	void SendCMD_MERCHANT_SELL_ITEM_DELETE();//판매 아이템을 삭제
	void RecvCMD_MERCHANT_SELL_ITEM_DELETE_RESULT(t_packet &p);//판매 아이템을 삭제
	
	void SendCMD_MERCHANT_SELL_ITEM_REGISTER();//판매 아이템을 등록
	void RecvCMD_MERCHANT_SELL_ITEM_REGISTER_RESULT(t_packet &p);//판매 아이템을 등록
//<----

//---->거래결과
	void SendCMD_MERCHANT_RESULT_LIST_REQUEST(const int iIndex = 0, const int iNeedIndexOver =1);//
	void RecvCMD_MERCHANT_RESULT_LIST_REQUEST_RESULT(t_packet &p);

	void SendCMD_MERCHANT_RESULT_TAKE(SEARCHRESULT &SR);
	void RecvCMD_MERCHANT_RESULT_TAKE_RESULT(t_packet &p);
//<----

//---->구매자가 상인과 거래를 요청
	int SendCMD_MERCHANT_EXCHANGE_LIST_REQUEST(char* szSellerName, const int iIndex, const int iNeedIndexOver, const bool bIsDirectExchange);
	int RecvCMD_MERCHANT_EXCHANGE_LIST_REQUEST_RESULT(t_packet &p);
//<----
	void SendCMD_EXCHANGE_BOND_MONEY();

//<! BBD 040303 백업테이블에서 등록물 찾기
	void SendCMD_MERCHANT_BACKUP_LIST_REQUEST(const int iIndex = 0, const int iNeedIndexOver = 1);
	void RecvCMD_MERCHANT_BACKUP_LIST_RESPONSE(t_packet &p);
	int IncPageBackupList();
	int DecPageBackupList();
	void SendCMD_MERCHANT_BACKUP_TAKE_REQUEST(SEARCHRESULT &SR);

//> BBD 040303 백업테이블에서 등록물 찾기
};
extern CAuction Auction;
#endif // !defined(AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)