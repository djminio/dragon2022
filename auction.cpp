// Auction.cpp: implementation of the CAuction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Auction.h"
#include "Menu.h"
#include "dragon.h"
#include "gameproc.h"
#include "MenuSet.h"
#include "Chat.h"
#include "Skill.h"
#include "CurrentMsgMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAuction Auction;

const SUPERMONEY SM1(BOND_MONEY_1000	,1000000000);
const SUPERMONEY SM2(BOND_MONEY_500		,500000000);
const SUPERMONEY SM3(BOND_MONEY_100		,100000000);
const SUPERMONEY SM4(BOND_MONEY_50		,50000000);
const SUPERMONEY SM5(BOND_MONEY_10		,10000000);
const SUPERMONEY SM6(BOND_MONEY_5		,5000000);

const SUPERMONEY aBondMoney[BOND_MONEY_MAX] = {SM1,SM2,SM3,SM4,SM5,SM6};


CAuction::CAuction()
{
	Clear();
}

CAuction::~CAuction()
{
	Clear();
}

void CAuction::Clear()
{
	ClearFK();
	::memset(&m_SearchList,0,sizeof(SEARCHRESULTLIST));
	m_iWaitPageRefresh = 0;
	m_iBuyerViewPage = 0;
	m_iSellerViewPage = 0;
	m_iResultViewPage = 0;
	m_iDeleteItemIndex= 0;
	m_iBuyItemIndex = 0;
	m_iTakeItemIndex= 0;

	this->m_nPeriod = 0;

	strcpy(m_szKeyWord,"");
	strcpy(m_szMerchantName,"");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//구매자가 아이템 검색 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_BUY_LIST_REQUEST(const int iIndex, const int iNeedIndexOver)//리스트 요청(구매자가 보는것//검색기가 호출
{
	//soto-030611 긴급수정.
//	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,82));
//	return;
	
	

	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,500));
	//패킷 보내기
	t_packet p;
	p.h.header.type = CMD_MERCHANT_BUY_ITEM_SEARCH;
	p.h.header.size = sizeof(SEARCHPACKETCLIENT);
		
	if(m_bChkKeyWord)
	{
		strcpy(p.u.SearchPacketClient.szKeyWord,m_szKeyWord);
	}
	else
	{
		strcpy(p.u.SearchPacketClient.szKeyWord,"");
	}

	if(m_bChkMerchantName)
	{
		strcpy(p.u.SearchPacketClient.szMerchant,m_szMerchantName);
	}
	else
	{
		strcpy(p.u.SearchPacketClient.szMerchant,"");
	}	
	p.u.SearchPacketClient.nPeriod  = m_nPeriod;
	p.u.SearchPacketClient.iIndex	= iIndex;
	p.u.SearchPacketClient.iKey		= iNeedIndexOver;

	m_iWaitPageRefresh = 1;

	::QueuePacket(&p,1);
	::CloseAllMenu();

	//>soto-030511 수정 해야함.	
}

void CAuction::RecvCMD_MERCHANT_BUY_LIST_REQUEST_RESULT(t_packet &p)//리스트 결과(구매자가 보는것
{
	Clear();
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,502));
	m_SearchList = p.u.SearchResultList;
	m_iBuyerViewPage  = (p.u.SearchResultList.iKey)?AUCTION_PAGE_MIN:AUCTION_PAGE_MAX;
	m_iWaitPageRefresh = 0;

	::CallSmallMenu(MN_MERCHANT_BUY);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//구매자가 아이템 검색 하기
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//구입하기 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_ITEM_BUY(SEARCHRESULT &SR)//구매하기(구매자가 선택
{
	if(!strcmp( SR.szSellerName,Hero->name))
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,503));
		return;
	}

	int iChatTarget = CHAT_TARGET_NORMAL;
	if(!SR.iSellValue)
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,504));
		iChatTarget = ::InsertWisperList(SR.szSellerName);
	}

	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,505));

	t_packet p;
	p.h.header.type = CMD_MERCHANT_ITEM_BUY;
	p.h.header.size = sizeof(MERCHANT_ITEM_BUY);

	p.u.MerchantItemBuy.iIndex = SR.iIndex;//구입할 아이템의 인덱스 값
	p.u.MerchantItemBuy.iCn = Hero->id;//커넥션 값
	strcpy(p.u.MerchantItemBuy.szSellerName,SR.szSellerName);
	strcpy(p.u.MerchantItemBuy.szBuyerName,Hero->name);//구매자 이름
	p.u.MerchantItemBuy.dwSellValue = SR.iSellValue;//판매금액은 20억을 넘기지 않습니다.
	p.u.MerchantItemBuy.SellItem	= SR.m_ResultItem;
	p.u.MerchantItemBuy.iKey = 0;
	::QueuePacket(&p,1);
	CloseAllMenu();
	if(!SR.iSellValue)
	{
		::SetChatMode(CM_MESSAGE);
		::SetChatTarget(iChatTarget);
	}
}

void CAuction::RecvCMD_MERCHANT_ITEM_BUY_RESULT(t_packet &p)//구매하기(구매자가 선택
{//구매결과가 온다
	//알아서 처리 ㅡㅡ;.//iKey 값으로 판별 합니다.//가져가기를 바라는 상태 //뭐든 할 수 있는 상태
	Clear();
	const int iResult = p.u.MerchantItemBuy.iKey;
	switch(iResult)
	{
	case IS_END_WAIT_TAKE://구매 완료 상태
		{
			::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,506));
		}break;
	case IS_END_ALL_RIGHT://구매전 상태네.. 구입 불가능 했나보다
		{
			::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,507));
		}break;
	default://
		{
			::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,508),iResult);
		}break;
	/*
	case IS_END_DELETE_COMPLETE:
	case IS_END_DELETING:
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,"구입에 실패 했습니다. 판매자가 지우고 있습니다.");
		}break;
	case IS_END_BUYING:
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,"누군가가 구매중입니다.");
		}break;
	*/
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//구입하기 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//내가 판매하는 리스트 요청 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_SELL_LIST_REQUEST(const int iIndex, const int iNeedIndexOver)//리스트 요청(판매자가 보는것
{	
	if(!::IsMerchant())
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,509));
		return;
	}
	if(iIndex && !iNeedIndexOver)
	{
		if(AUCTION_PAGE_MIN == GetSellerViewPage())
		{
			for(int xx = 0; MAX_VIEW_ABLE_PER_PAGE > xx; xx++)
			{
				if(!m_SearchList.ResultList[xx].m_ResultItem.item_no)
				{
					::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
					return;
				}
			}		
		}
	}

	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,511));
	t_packet p;
	p.h.header.type = CMD_MERCHANT_SELL_ITEM_INFO_REQUEST;
	p.h.header.size = sizeof(SELLERITEMREQUEST);
	p.u.SellerItemRequest.iCn = Hero->id;
	strcpy(p.u.SellerItemRequest.szName,Hero->name);

	p.u.SellerItemRequest.iIndex = iIndex;
	p.u.SellerItemRequest.iKey = iNeedIndexOver;
	m_iWaitPageRefresh = 1;
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_SELL_LIST_REQUEST_RESULT(t_packet &p)//리스트 결과(판매자가 보는것
{
	Clear();

	if(!::IsMerchant())
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,509));
		return;
	}

	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,512));
	m_SearchList = p.u.SearchResultList;
	if(p.u.SearchResultList.iKey)
	{
		m_iSellerViewPage  = AUCTION_PAGE_MIN;
	}
	else
	{
		if(m_SearchList.ResultList[0].m_ResultItem.item_no)
		{
			m_iSellerViewPage = AUCTION_PAGE_MIN;
		}
		if(m_SearchList.ResultList[6].m_ResultItem.item_no)
		{
			m_iSellerViewPage = AUCTION_PAGE_MIN+1;
		}
		if(m_SearchList.ResultList[12].m_ResultItem.item_no)
		{
			m_iSellerViewPage = AUCTION_PAGE_MIN+2;
		}
		if(m_SearchList.ResultList[18].m_ResultItem.item_no)
		{
			m_iSellerViewPage = AUCTION_PAGE_MAX;
		}
	}
	m_iWaitPageRefresh = 0;
	::CallSmallMenu(MN_MERCHANT_SELL_INFO);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//내가 판매하는 리스트 요청 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래물 판매취소 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_SELL_ITEM_DELETE()//판매 아이템을 삭제
{
	t_packet p;
	p.h.header.type = CMD_MERCHANT_SELL_ITEM_DELETE;
	p.h.header.size = sizeof(SELLERITEMDELETE);
	
	SEARCHRESULT *SR = NULL;
	int i;
	for(i = 0; 24 > i; i++)
	{
		if(m_iDeleteItemIndex == m_SearchList.ResultList[i].iIndex )
		{
			SR = &m_SearchList.ResultList[i];
			break;
		}
	}
	if( !m_iDeleteItemIndex || 24 == i )
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,513));
		return;
	}
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,514));
	p.u.SellerItemDelete.iIndex = SR->iIndex;//판매물품의 인덱스
	p.u.SellerItemDelete.iCn	= Hero->id;
	strcpy(p.u.SellerItemDelete.szName,SR->szSellerName);	
	p.u.SellerItemDelete.dwSellValue  = SR->iSellValue;
	p.u.SellerItemDelete.SellItem  = SR->m_ResultItem;
	p.u.SellerItemDelete.iKey = 0;
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_SELL_ITEM_DELETE_RESULT(t_packet &p)//판매 아이템을 삭제
{
	Clear();
	if(IS_END_DELETE_COMPLETE == p.u.SellerItemDelete.iKey)
	{//성공적으로 지급 받았습니다.
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,515));
	}
	else
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,516));
	}
	SendCMD_MERCHANT_SELL_LIST_REQUEST();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래물 판매취소 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래물 등록하기 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_SELL_ITEM_REGISTER()//판매 아이템을 등록
{//판매아이템을 등록 합니다.
	t_packet p;
	p.h.header.type = CMD_MERCHANT_SELL_ITEM_REGISTER;
	p.h.header.size = sizeof(SELLERITEMREGISTER);
	
	p.u.SellerItemRegister.iCn = Hero->id;
	strcpy(p.u.SellerItemRegister.szName,Hero->name);	

	p.u.SellerItemRegister.pos		= m_SellItemPos;//pos 값
	p.u.SellerItemRegister.SellItem	= m_SellItemAttr;//아이템 실제값
	
	p.u.SellerItemRegister.dwSellValue = m_iSellValue;
//	if(!m_iSellValue)
//	{
//		COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,"직접거래로 등록 되었습니다.");
//		COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,"아직은 지원하지 않는 기능입니다.");
//		return;
//	}
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,517));
	//패킷 값 설정해서 보낼것
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_SELL_ITEM_REGISTER_RESULT(t_packet &p)//판매 아이템을 등록
{
	Clear();
	//내 리스트를 다시 요청 하는것을 넣습니다.
	if(1==p.u.SellerItemRegister.iKey)
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,518));
	}
	else
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,519));
	}
	SendCMD_MERCHANT_SELL_LIST_REQUEST();//다시 나의 판매 목록 리스트를 요청합니다.
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래물 등록하기 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래 결과물 리스트 받아오기 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_RESULT_LIST_REQUEST(const int iIndex, const int iNeedIndexOver)
{
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,520));
	t_packet p;
	p.h.header.type = CMD_MERCHANT_RESULT_LIST_REQUEST;
	p.h.header.size = sizeof(SELLERITEMREQUEST);
	p.u.SellerItemRequest.iCn = Hero->id;
	strcpy(p.u.SellerItemRequest.szName,Hero->name);
	p.u.SellerItemRequest.iIndex = iIndex;
	p.u.SellerItemRequest.iKey = iNeedIndexOver;
	m_iWaitPageRefresh = 1;
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_RESULT_LIST_REQUEST_RESULT(t_packet &p)
{
	Clear();
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,521));
	m_SearchList = p.u.SearchResultList;
	m_iResultViewPage = (p.u.SearchResultList.iKey)?AUCTION_PAGE_MIN:AUCTION_PAGE_MAX;
	m_iWaitPageRefresh = 0;
	::CallSmallMenu(MN_MERCHANT_RESULT_TAKE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래 결과물 리스트 받아오기 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래 결과물 찾기 시작
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_RESULT_TAKE(SEARCHRESULT &SR)
{
	t_packet p;
	p.h.header.type = CMD_MERCHANT_RESULT_TAKE;
	p.h.header.size = sizeof(MERCHANTRESULTTAKE);
	if(!SR.iIndex)
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,524));
		return;
	}
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,523));
	p.u.MerchantResultTake.iIndex	=	SR.iIndex;
	p.u.MerchantResultTake.iCn		=	Hero->id;
	strcpy(p.u.MerchantResultTake.szMyName,Hero->name);
	strcpy(p.u.MerchantResultTake.SellerName,SR.szSellerName);
	strcpy(p.u.MerchantResultTake.BuyerName,SR.szBuyerName);
	
	p.u.MerchantResultTake.dwSellValue	= SR.iSellValue;
	p.u.MerchantResultTake.SellItem		= SR.m_ResultItem;
	p.u.MerchantResultTake.iSellerTake	= 0;
	p.u.MerchantResultTake.iBuyerTake	= 0;
	p.u.MerchantResultTake.iKey			= 0;

	//인덱스 번호부터해서 자료 셋팅
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_RESULT_TAKE_RESULT(t_packet &p)
{
	SendCMD_MERCHANT_RESULT_LIST_REQUEST();
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,525));
	Clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//거래 결과물 찾기 끝
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CAuction::SetSellItem(const POS &pos,const ItemAttr Item)
{
	if(INV != pos.type) 
	{
		COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE , lan->OutputMessage(2,530));//인벤토리 아템을 넣으 십쇼..
		return 0;
	}
	if(!Item.item_no)
	{
		return 0;
	}
	m_SellItemPos = pos;
	m_SellItemAttr = Item;
	return 1;
}

int CAuction::SetBondItem(const POS &pos,const ItemAttr Item)
{
	if(INV != pos.type) 
	{
		COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE , lan->OutputMessage(2,530));//인벤토리 아템을 넣으 십쇼..
		return 0;
	}
	if(!Item.item_no)
	{
		return 0;
	}
	m_BondItemPos = pos;
	m_BondItemAttr = Item;
	return 1;
}

int	CAuction::IncBuyerViewPage() 
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MAX == GetBuyerViewPage())//3이 되었으면 가진 리스트는 끝이다..
	{	//리스트 요청 했을때의 값이 왔어야 하고

		const int iSendIndex = SR[MAX_SEARCH_RESULT_LIST-1].iIndex;
		if(iSendIndex)//리스트가 꽉 차있다.. 그래야만 다음 페이지를 요청하지
		{
			SendCMD_MERCHANT_BUY_LIST_REQUEST(iSendIndex,true);
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"첫 목록 부터 다시 신청합니다.");
			SendCMD_MERCHANT_BUY_LIST_REQUEST(0,true);	
		}
	}
	else
	{
		int xx =0;
		for(xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
		{
			if(!SR[xx].iIndex){break;}//목록 중에 인덱스가 0인 자리찾기
		}

		if((xx)/MAX_VIEW_ABLE_PER_PAGE > GetBuyerViewPage())//다음 페이지에 못가도록 0부터 세기 때문에 -1을 해준것이다
		{
			m_iBuyerViewPage++;//다음 페이지로
		}
	}
	return 0;
}

int	CAuction::DecBuyerViewPage() 
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MIN >=GetBuyerViewPage())//0페이지 일때 한번더 Dec시키면
	{
		if(SR[0].iIndex)//있으면 보내는데 인덱스가 0이면안된다
		{
			SendCMD_MERCHANT_BUY_LIST_REQUEST(SR[0].iIndex,false);//목록 요청을 하는데
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"첫 목록 부터 다시 신청합니다.");
			SendCMD_MERCHANT_BUY_LIST_REQUEST(0,true);				
		}
	}
	else
	{
		m_iBuyerViewPage--;//이전 페이지로
	}
	return 0;
}

int	CAuction::IncResultViewPage() 
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MAX == GetResultViewPage())//3이 되었으면 가진 리스트는 끝이다..
	{	//리스트 요청 했을때의 값이 왔어야 하고

		const int iSendIndex = SR[MAX_SEARCH_RESULT_LIST-1].iIndex;
		if(iSendIndex)//리스트가 꽉 차있다.. 그래야만 다음 페이지를 요청하지
		{
			SendCMD_MERCHANT_RESULT_LIST_REQUEST(iSendIndex,true);
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"첫 목록 부터 다시 신청합니다.");
			SendCMD_MERCHANT_RESULT_LIST_REQUEST(0,true);	
		}
	}
	else
	{
		int xx =0;
		for(xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
		{
			if(!SR[xx].iIndex){break;}//목록 중에 인덱스가 0인 자리찾기
		}

		if((xx)/MAX_VIEW_ABLE_PER_PAGE > GetResultViewPage())//다음 페이지에 못가도록 0부터 세기 때문에 -1을 해준것이다
		{
			m_iResultViewPage++;//다음 페이지로
		}
	}
	return 0;
}

int	CAuction::DecResultViewPage() 
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MIN >=GetResultViewPage())//0페이지 일때 한번더 Dec시키면
	{
		if(SR[0].iIndex)//있으면 보내는데 인덱스가 0이면안된다
		{
//<! BBD 040303 음수일때의 이상동작 방지
			if(m_iResultViewPage <= 0)
			{
				m_iResultViewPage = 0;
				return 0;
			}
			else
			{
				SendCMD_MERCHANT_RESULT_LIST_REQUEST(SR[0].iIndex,false);//목록 요청을 하는데
			}
//> BBD 040303 음수일때의 이상동작 방지
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"첫 목록 부터 다시 신청합니다.");
			SendCMD_MERCHANT_RESULT_LIST_REQUEST(0,true);				
		}
	}
	else
	{
		m_iResultViewPage--;//이전 페이지로
	}
	return 0;
}

int	CAuction::IncSellerViewPage()
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MAX == GetSellerViewPage())//3이 되었으면 가진 리스트는 끝이다..
	{	//리스트 요청 했을때의 값이 왔어야 하고

		const int iSendIndex = SR[MAX_SEARCH_RESULT_LIST-1].iIndex;
		if(iSendIndex)//리스트가 꽉 차있다.. 그래야만 다음 페이지를 요청하지
		{
			SendCMD_MERCHANT_SELL_LIST_REQUEST(iSendIndex,true);
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"첫 목록 부터 다시 신청합니다.");
			SendCMD_MERCHANT_SELL_LIST_REQUEST(0,true);	
		}
	}
	else
	{
		int xx =0;
		for(xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
		{
			if(!SR[xx].iIndex){break;}//목록 중에 인덱스가 0인 자리찾기
		}

		if((xx)/MAX_VIEW_ABLE_PER_PAGE > GetSellerViewPage())//다음 페이지에 못가도록 0부터 세기 때문에 -1을 해준것이다
		{
			m_iSellerViewPage++;//다음 페이지로
		}
	}
	return 0;
}

int	CAuction::DecSellerViewPage()
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MIN >=GetSellerViewPage())//0페이지 일때 한번더 Dec시키면
	{
		if(SR[0].iIndex)//있으면 보내는데 인덱스가 0이면안된다
		{
			SendCMD_MERCHANT_SELL_LIST_REQUEST(SR[0].iIndex,false);//목록 요청을 하는데
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"첫 목록 부터 다시 신청합니다.");
			SendCMD_MERCHANT_SELL_LIST_REQUEST(0,true);
		}
	}
	else
	{
		m_iSellerViewPage--;//이전 페이지로
	}
	return 0;
}

int CAuction::SendCMD_MERCHANT_EXCHANGE_LIST_REQUEST(char* szSellerName,const int iIndex, const int iNeedIndexOver,const bool bIsDirectExchange)
{//리스트 요청
	#ifdef KOREA_LOCALIZING_ 
	{	//< CSD-TW-030625
		LPCHARACTER pch = ::ExistHe(szSellerName);//내 시야에 있는 사람이야?
	//	if(!pch){return 0;}	//사람이 내 시야에 있구나.
		if(!strcmp(szSellerName,Hero->name))
		{
			COMMENT ::AddCurrentStatusMessage(255,255,255,"자기자신의 목록을 신청 할 수 없습니다.");
			return 0;
		}

		t_packet p;
		p.h.header.type = CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET;
		p.h.header.size = sizeof(MERCHANTEXCHANGEREQUEST);

		p.u.MerchantExchangeRequest.iIndex	=	iIndex;
		p.u.MerchantExchangeRequest.iCn		=	Hero->id;
		::strcpy(p.u.MerchantExchangeRequest.szMyName,Hero->name);
		::strcpy(p.u.MerchantExchangeRequest.szSellerName,szSellerName);
		
		p.u.MerchantExchangeRequest.iKey	= iNeedIndexOver;
	//	p.u.MerchantExchangeRequest.iKey2	= bIsDirectExchange;
		::QueuePacket(&p,1);
	}	//> CSD-TW-030625
#endif
	return 0;
}

int CAuction::RecvCMD_MERCHANT_EXCHANGE_LIST_REQUEST_RESULT(t_packet &p)
{//리스트 받음
	Clear();
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,527));
	
	m_SearchList = p.u.SearchResultList;
	m_iBuyerViewPage  = ((p.u.SearchResultList.iKey)?AUCTION_PAGE_MIN:AUCTION_PAGE_MAX);
	m_iWaitPageRefresh = 0;
	::CallSmallMenu(MN_MERCHANT_DIRECT_EXCHAGE_LIST);
//	::CallSmallMenu(MN_MERCHANT_BUY);
	
	return 0;
}

void CAuction::SendCMD_EXCHANGE_BOND_MONEY()//021126 lsw
{
	if(!Auction.GetBondItemAttr())
	{
		return;
	}
	t_packet p;
	p.h.header.type =	CMD_EXCHANGE_BOND_MONEY;
	p.h.header.size =	sizeof(t_ExchangeBondMoney);
	p.u.Hwoa.rang.ExBondMoney.iCn			= Hero->id;
	p.u.Hwoa.rang.ExBondMoney.pos			= Auction.GetBondItemPos();
	p.u.Hwoa.rang.ExBondMoney.BondMoneyItem	= *Auction.GetBondItemAttr();
	p.u.Hwoa.rang.ExBondMoney.iKey = 0;
	ClearBondItem();
	::QueuePacket( &p, 1 );
}

void CAuction::IncPeriod(int nInc)
{
	m_nPeriod += nInc;
	if(m_nPeriod > 2)m_nPeriod = 2;
}

void CAuction::DecPeriod(int nDec)
{
	m_nPeriod -= nDec;
	if(m_nPeriod < 0)m_nPeriod = 0;
}
//////////////////////////////////////////////////////////////////////////////////////
// BBD 040303	머천트 백업 테이블을 이용하는 함수군 시작							//
//////////////////////////////////////////////////////////////////////////////////////
//창이 뜨자마자 등록되어 있는 리스트를 요청한다
void CAuction::SendCMD_MERCHANT_BACKUP_LIST_REQUEST(const int iIndex, const int iNeedIndexOver)
{
	Clear();	// 혹시 모르니 리스트를 날려버리고 요청한다
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,520));
	SELLERITEMREQUEST	data;

	data.iCn = Hero->id;
	strcpy(data.szName,Hero->name);
	data.iIndex = iIndex;
	data.iKey = iNeedIndexOver;
	m_iWaitPageRefresh = 1;

	SendDirectDB(CMD_MERCHANT_BACKUP_LIST_REQUEST, &data, sizeof(data));
}
// 요청한 리스트를 받고 출력한다.
void CAuction::RecvCMD_MERCHANT_BACKUP_LIST_RESPONSE(t_packet &p)
{
	Clear();	// 혹시 모르니 리스트를 날리고 받아서 출력하자
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,521));

	memcpy(&m_SearchList, p.u.data, sizeof(m_SearchList));

	m_iResultViewPage = (m_SearchList.iKey)?AUCTION_PAGE_MIN:AUCTION_PAGE_MAX;
	m_iWaitPageRefresh = 0;
	::CallSmallMenu(MN_MERCHANT_BACKUP_MAIN);
}

int CAuction::IncPageBackupList()	// 페이지 Down 버튼 클릭시 호출됨
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MAX == GetResultViewPage())//3이 되었으면 가진 리스트는 끝이다..
	{	//리스트 요청 했을때의 값이 왔어야 하고

		const int iSendIndex = SR[MAX_SEARCH_RESULT_LIST-1].iIndex;
		if(iSendIndex)//리스트가 꽉 차있다.. 그래야만 다음 페이지를 요청하지
		{
			SendCMD_MERCHANT_BACKUP_LIST_REQUEST(iSendIndex,true);
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"첫 목록 부터 다시 신청합니다.");
			SendCMD_MERCHANT_BACKUP_LIST_REQUEST(0,true);	
		}
	}
	else
	{
		int xx =0;
		for(xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
		{
			if(!SR[xx].iIndex){break;}//목록 중에 인덱스가 0인 자리찾기
		}

		if((xx)/MAX_VIEW_ABLE_PER_PAGE > GetResultViewPage())//다음 페이지에 못가도록 0부터 세기 때문에 -1을 해준것이다
		{
			m_iResultViewPage++;//다음 페이지로
		}
	}
	return 0;
}

int CAuction::DecPageBackupList()	// 페이지 Up 버튼 클릭시 호출됨
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MIN >=GetResultViewPage())//0페이지 일때 한번더 Dec시키면
	{
		if(SR[0].iIndex)//있으면 보내는데 인덱스가 0이면안된다
		{
			if(m_iResultViewPage <= 0)	// 페이지가 0이면
			{
				m_iResultViewPage = 0;
				return 0;	// 암것도 하지마라
			}
			else
			{
				SendCMD_MERCHANT_BACKUP_LIST_REQUEST(SR[0].iIndex,false);//목록 요청을 하는데
			}
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"첫 목록 부터 다시 신청합니다.");
			SendCMD_MERCHANT_BACKUP_LIST_REQUEST(0,true);				
		}
	}
	else
	{
		m_iResultViewPage--;//이전 페이지로
	}
	return 0;
}

// 선택한 아이템을 요청한다
void CAuction::SendCMD_MERCHANT_BACKUP_TAKE_REQUEST(SEARCHRESULT &SR)	
{
	if(!SR.iIndex)
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,524));
		return;
	}
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,523));

	Clear();	// 혹시 모르니 리스트를 날려버리고 요청한다

	t_MerchantResultTake	data;

	data.iIndex	=	SR.iIndex;
	data.iCn		=	Hero->id;
	strcpy(data.szMyName,Hero->name);
	strcpy(data.SellerName,SR.szSellerName);
	strcpy(data.BuyerName,SR.szBuyerName);
	
	data.dwSellValue	= SR.iSellValue;
	data.SellItem		= SR.m_ResultItem;
	data.iSellerTake	= 0;
	data.iBuyerTake	= 0;
	data.iKey			= 0;

	m_iWaitPageRefresh = 1;
	// 일단 DB까정 날리고 보자
	SendDirectDB(CMD_MERCHANT_BACKUP_TAKE_REQUEST, &data, sizeof(t_MerchantResultTake));
}
//////////////////////////////////////////////////////////////////////////////////////
// BBD 040303	머천트 백업 테이블을 이용하는 함수군 끝								//
//////////////////////////////////////////////////////////////////////////////////////