// ExchangeMgr.cpp: implementation of the CExchangeMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Menu.h"
#include "dragon.h"
#include "gameproc.h"
#include "MenuSet.h"
#include "char.h"
#include "Map.h"
#include "Math.h"
#include "ExchangeMgr.h"
#include "CharDataTable.h"
#include "SmallMenuSet.h"
DefaultItem item_give[MAX_EXCHANGE_ITEM]={0,};
DefaultItem item_recv[MAX_EXCHANGE_ITEM]={0,};

CExchangeMgr ExchangeMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CExchangeMgr::CExchangeMgr()
{
	Clear();
	ClearItemData();
}

CExchangeMgr::~CExchangeMgr()
{
	Clear();
	ClearItemData();
}
void CExchangeMgr::Clear()
{	
	m_iExchageType			 = 0;//거래 타입
	m_iExchageTargetId		 = 0;//거래중인 상대의 cn 값
	m_bOtherSideConformState = 0;//상대방이 거래 Ok 사인을 보냈는가.(거래인터페이스 닫히거나 열릴때 무조건 0 으로 만들어 줘라.
	m_bMySideConformState	 = 0;//030108 lsw
	m_bIsIAmMerchant		 = false;
	SetExchangeState(EXCHANGE_STATE_READY);
}

void CExchangeMgr::ClearItemData()
{
	::memset( item_recv, 0, sizeof( DefaultItem )*MAX_EXCHANGE_ITEM );
	::memset( item_give, 0, sizeof( DefaultItem )*MAX_EXCHANGE_ITEM );//모두 지우고 보자
}

bool IsExchangeNow()//교환 부분
{
	return ( EXCHANGE_STATE_EXCHANGE_NOW == ExchangeMgr.GetExchangeState() )?true:false;
}

void CExchangeMgr::CallExchangeMenu(const bool bIsRecvSide, const int iExchageType)//내부에서 클리어 되니까 조심하고
{
	Clear();
	ClearItemData();
	SetExchangeType(iExchageType);

	::CloseAllMenu();//메뉴룰 모두 닫고

	fRMoveMenu = MM_ITEM_OPEN;		// 위의 if문을 만족할 때만
	nItemOther = MN_EXCHANGE_ITEM;
	y_MenuFocus = 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//해당 클라이언트가 보내는 것
void SendExchangeItemWithOtherCh( short int you_id ,const int iExchangeType)
{//교환 하자고 한다(거래)
	t_packet p;
	p.h.header.type = CMD_EXCHANGE_ITEM_READY;
	p.u.kein.exchange_item_start.you_id			= you_id;
	p.u.kein.exchange_item_start.iExchangeType	= iExchangeType;//021030 lsw
	p.h.header.size = sizeof( k_exchange_item_start );
	QueuePacket( &p, 1 );
}

void CExchangeMgr::SendExchangeResult()
{	//교환 결과를 보낸다
	t_packet p;
	switch(GetExchangeType())
	{
	case EXCHANGE_TYPE_NORMAL:
	case EXCHANGE_TYPE_MERCHANT_DIRECT:
		{
			p.h.header.type = CMD_EXCHANGE_ITEM;
			p.u.kein.client_other_ch_inv.other_ch = ExchangeMgr.GetExchangeTargetId();
			p.h.header.size = sizeof( kein_client_other_ch_inv );
			QueuePacket( &p, 1 );
		}break;
	default:
		{
			return;
		}break;
	}	
}

void CExchangeMgr::SendExchangeStateSign(const bool bOkSign)
{
	t_packet p;

	LPCHARACTER	pCh = ::FindCharacter( &g_CharacterList, GetExchangeTargetId() );
	if( pCh //he is near me
	&&	!strcmp(pCh->name,GetExchangeTargetName()) 	)
	{
		if(bOkSign)
		{
			p.h.header.type = CMD_EXCHANGE_OK_SIGN;
		}
		else
		{
			p.h.header.type = CMD_EXCHANGE_CANCEL_SIGN;
		}
		
		p.u.kein.client_other_ch_inv.other_ch = GetExchangeTargetId();
		p.h.header.size = sizeof( kein_client_other_ch_inv );
		::QueuePacket( &p, 1 );
		return;
	}
	else
	{
		::CallServer( CMD_EXCHANGE_CANCEL );
	}
}

void SendExchangeItemEach( ItemAttr &item, POS &pos, short int you_id )
{
	if( pos.type < HAND && pos.type > BOX ){return;}
	t_packet p;
	p.h.header.type = CMD_EXCHANGE_ITEM_EACH;
	p.u.kein.exchange_item_start.item.item_attr = item;
	p.u.kein.exchange_item_start.item.item_pos = pos;
	p.u.kein.exchange_item_start.you_id = you_id;
	p.h.header.size = sizeof( k_exchange_item_start );
	QueuePacket( &p, 1 );
}

void SendExchangeItemDel( POS pos, short int you_id )
{
	t_packet p;

	p.h.header.type = CMD_EXCHANGE_ITEM_DEL;
	p.u.kein.exchange_item_del.pos = pos;
	p.u.kein.exchange_item_del.you_id = you_id;
	p.h.header.size = sizeof( k_exchange_item_del );
	QueuePacket( &p, 1 );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//해당 클라이언트가 받는 것
void RecvExchangeItemOk( k_exchange_item_start *exchange_item )
{
	const int iTargetId = exchange_item->you_id;
	
	if( ::FindCharacter( &g_CharacterList, iTargetId ) )
	{
		ExchangeMgr.CallExchangeMenu(false,EXCHANGE_TYPE_NORMAL);//내부에서 클리어 되니까 조심하고
		
		if(EXCHANGE_TYPE_MERCHANT_DIRECT== exchange_item->iExchangeType)
		{//내부에서 클리어 되어서 정보 사라집니다. ExchangeMgr.CallExchangeMenu(false,EXCHANGE_TYPE_NORMAL); 하단에 놓으십시요
			ExchangeMgr.m_bIsIAmMerchant = true;
		}

		::SetExchageItemData(0,exchange_item->item.item_attr,exchange_item->item.item_pos);
		ExchangeMgr.SetExchangeTargetId(iTargetId);//021030 lsw
		ExchangeMgr.SetExchangeState(EXCHANGE_STATE_EXCHANGE_NOW);//021030 lsw
		ExchangeMgr.SetExchangeType(exchange_item->iExchangeType);//021030 lsw
	}
}

void RecvExchangeItemReq( k_exchange_item_start *exchange_item )//교환 요구
{	
	const int iTargetId = exchange_item->you_id;

	if( ::FindCharacter( &g_CharacterList, iTargetId ) )
	{
		ExchangeMgr.CallExchangeMenu(true,EXCHANGE_TYPE_NORMAL);//내부에서 클리어 되니까 조심하고

		::RecvExchangeEach(&exchange_item->item);//아이템 받은거 셋팅 하고
		ExchangeMgr.SetExchangeTargetId(iTargetId);//021030 lsw
		ExchangeMgr.SetExchangeState(EXCHANGE_STATE_EXCHANGE_NOW);//021030 lsw
		ExchangeMgr.SetExchangeType(exchange_item->iExchangeType);//021030 lsw
	}
}

void RecvExchangeItemResult( int ret )	// 서버 전송에 따른 에러 메시지 처리
{
	ExchangeMgr.ClearItemData();//021030 lsw

	switch( ret )
	{
		case 0:		Kein_PutMessage( KM_FAIL, lan->OutputMessage(5,31)); break;//010215 lsw
		case 1:		Kein_PutMessage( KM_FAIL, lan->OutputMessage(5,32)); break;//
		case 2:		Kein_PutMessage( KM_FAIL, lan->OutputMessage(5,33)); break;//
		case 3:		Kein_PutMessage( KM_FAIL, lan->OutputMessage(5,34)); break;//
		case 4:		Kein_PutMessage( KM_FAIL, lan->OutputMessage(5,35)); break;//
		case 5:		Kein_PutMessage( KM_FAIL, lan->OutputMessage(5,36)); break;//010215 lsw
	}
	// 완전히 취소 시킨다.
	::CloseAllMenu();
}

void RecvExchangeCancel()
{
	ExchangeMgr.Clear();
	ExchangeMgr.ClearItemData();
	SMenu[MN_EXCHANGE_ITEM].nField[3].nSHideNomalCount=0;
	SMenu[MN_EXCHANGE_ITEM].nField[4].nSHideNomalCount=0;
	SMenu[MN_EXCHANGE_ITEM].work = 0;
	MenuSoundClose( MN_EXCHANGE_ITEM );

	CallOkCancelMessageBox( 0, 0, 0,lan->OutputMessage(5,37), 0 );//010215 lsw

	if( EXCHANGE_STATE_EXCHANGE_NOW != ExchangeMgr.GetExchangeState() )
	{//컴폼 상태가 아닌데 이런 메세지가 들어온다??
		COMMENT ::AddCurrentStatusMessage(FONT_COLOR_RED,"Critical Error");
	}
}

// 020320 YGI
void RemoveExchangeItemStatus()
{
	ExchangeMgr.Clear();
	ExchangeMgr.ClearItemData();
	
	SMenu[MN_EXCHANGE_ITEM].nField[3].nSHideNomalCount=0;
	SMenu[MN_EXCHANGE_ITEM].nField[4].nSHideNomalCount=0;
	SMenu[MN_EXCHANGE_ITEM].work = 0;
}

void RecvExchange()		//교환 성공
{	// 020320 YGI // 세금및 아이템 목록은 이곳에서...
	RemoveExchangeItemStatus();//클리어를 시키고
	MenuSoundClose( MN_EXCHANGE_ITEM );
	CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(5,38), 0 );//010215 lsw
}

void GetXANDYByExchangeItemIndex(const int iExchangeIndex, int &iX, int &iY)
{
	iX= (iExchangeIndex%5)*32 +(iExchangeIndex%5)*3;
	iY= (iExchangeIndex/5)*32 +(iExchangeIndex/5)*3;//놓을때 정렬을 위해서
}

void RecvExchangeEach( const K_ITEM *lpItem )
{
	for( int iExchangeIndex=0; MAX_EXCHANGE_ITEM > iExchangeIndex; iExchangeIndex++)
	{
		DefaultItem *pTarget = &item_recv[iExchangeIndex];
		if( !pTarget->item.item_attr.item_no )//빈 공간이구나
		{
			pTarget->item = *lpItem;//아이템 넣고

			int iItemX = 0, iItemY = 0;
			::GetXANDYByExchangeItemIndex(iExchangeIndex,iItemX,iItemY);//놓을때 정렬을 위해서
		
			pTarget->x = iItemX;//좌표 셋팅
			pTarget->y = iItemY;
			SetRect( &pTarget->rect, 
				pTarget->x-15,	pTarget->y-15,
				pTarget->x+15,	pTarget->y+15 );
			break;
		}
	}
}

void RecvExchangeItemDelWhereRecv( K_ITEM &Item )					// 저쪽에서 아이템을 빼면 이쪽에서도 삭제한다.
{//030108 lsw
	for( int i=0; i<MAX_EXCHANGE_ITEM; i++)
	{
		if( item_recv[i].item.item_attr.item_no )
		{
			if( !memcmp(&item_recv[i].item,&Item,sizeof(K_ITEM)) )
			{
				::memset( &item_recv[i], 0, sizeof(DefaultItem ) ); 
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 1:1 결투 시스템
void SendReqManToMan( char *name )		// 결투 신청
{//결투시 아이템 바꾸지 않는다
	if( MapNumber == 30 )
	{
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,28));//010215 lsw
		return;
	}

	if( SMenu[MN_MAN_TO_MAN].bActive ) 
	{
		MP3( SN_WARNING ); 
		if( SMenu[MN_MAN_TO_MAN].work )
				AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,29));//010215 lsw
		else	AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,30));//010215 lsw
		return;
	}
	if( IsDead( Hero ) )
	{
		MP3( SN_WARNING ); 
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,31));//010215 lsw
		return;
	}
	if( Hero->fight_flag )
	{
		MP3( SN_WARNING ); 
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,32));//010215 lsw
		return;
	}
	if( !strcmp( SCharacterData.sCharacterName, name ) )
	{
		MP3( SN_WARNING ); 
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,33));//010215 lsw
		return;
	}
	LPCHARACTER target = ExistHe( name );
	if( !target ) 
	{
		MP3( SN_WARNING ); 
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,34));//010215 lsw
		return;
	}
	if( IsDead( target ) )
	{
		MP3( SN_WARNING ); 
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,35));//010215 lsw
		return;
	}
	if( target->id > 10000 ) 
	{
		MP3( SN_WARNING ); 
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,36));//010215 lsw
		return;
	}
	if( target->lv > SCharacterData.nLevel+10 ) 
	{
		MP3( SN_WARNING ); 
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,37));//010215 lsw
		return;
	}
	int line_len = (int )( sqrt( pow((float)(target->x - Hero->x), (float)2)+pow((float)(target->y - Hero->y), (float)2) ) / TILE_SIZE);
	if( line_len > 5 )
	{
		MP3( SN_WARNING ); 
		AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(3,38));//010215 lsw
		return;
	}
	ServerConnectMenuClose();		// 복사 방지

	SMenu[MN_MAN_TO_MAN].key = target->id;
	
	strcpy( SMenu[MN_MAN_TO_MAN].nField[VS_RIGHT_DISPLAY_FIELD_NO].temp, SCharacterData.sCharacterName);
	strcpy( SMenu[MN_MAN_TO_MAN].nField[VS_LEFT_DISPLAY_FIELD_NO].temp, name );
	SMenu[MN_MAN_TO_MAN].work = 0;		// 신청을 했다..
	//////////////////////////////////////////////////////

	t_packet p;		// 서버로 신청

	p.h.header.type = CMD_REQ_MAN_TO_MAN;
	p.u.kein.default_short_int = target->id;
	p.h.header.size = sizeof( short int );
	QueuePacket( &p, 1 );
}

void RecvReqManToMan( short int target_id )
{
	char *target_name = NULL;
	LPCHARACTER target = ReturnCharacterPoint( target_id );
	if( !target )
	{
		SendReqManToManResult( target_id, 5 );
		return;
	}
	target_name = target->name;
	if( SMenu[MN_MAN_TO_MAN].bActive || IsExchangeNow() )//021014 lsw
	{
		SendReqManToManResult( target_id, 0 );
		return;
	}

	int line_len = (int) (sqrt( pow((float)(target->x - Hero->x), (float)2)+pow((float)(target->y - Hero->y), (float)2) ) / TILE_SIZE);
	if( line_len > 5 )
	{
		SendReqManToManResult( target_id, 6 );
		return;
	}
	//	결투... 거부
	AddCurrentStatusMessage( 250, 250, 250, lan->OutputMessage(3,39), target_name );//010215 lsw

	ExchangeMgr.ClearItemData();//021030 lsw

	CallMenu( MN_MAN_TO_MAN );
	SMenu[MN_MAN_TO_MAN].key = target_id;

	strcpy( SMenu[MN_MAN_TO_MAN].nField[VS_LEFT_DISPLAY_FIELD_NO].temp, target_name );
	strcpy( SMenu[MN_MAN_TO_MAN].nField[VS_RIGHT_DISPLAY_FIELD_NO].temp, SCharacterData.sCharacterName);
	SMenu[MN_MAN_TO_MAN].work = 1;		// 신청을 받아 들였다...
}

void SendReqManToManResult( short int target_id, int ret )
{
	t_packet p;		// 서버로 신청

	p.h.header.type = CMD_REQ_MAN_TO_MAN_RESULT;
		p.u.kein.req_man_to_man_result.id = target_id;
		p.u.kein.req_man_to_man_result.ret = ret;
	p.h.header.size = sizeof( k_req_man_to_man_result );
	QueuePacket( &p, 1 );
}

void RecvReqManToManResult( char ret )
{
	bool bCanOpen = false;
	switch( ret )
	{
	case 2:	{::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,23)); }break;//010215 lsw
	case 3:	{::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,24)); }break;//010215 lsw
	case 4:	{::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,25)); }break;//010215 lsw
	case 5: {::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,26)); }break;//010215 lsw
	case 6: {::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,27)); }break;//010215 lsw
	case 7: {::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,292)); }break;// finito 25/08/07 blocking duel requests
	case 0: {::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,22)); }break;//010215 lsw
	case 1: 
		{
			::AddCurrentStatusMessage( 250, 250, 250, lan->OutputMessage(3,21)); 
			bCanOpen = true;
		}break;//010215 lsw
	}

	if(bCanOpen)
	{
		::CallMenu( MN_MAN_TO_MAN );
	}
	else
	{
		::CloseAllMenu();
	}
}

void SendManToManRefusal( short int target_id )
{
	t_packet p;		

	p.h.header.type = CMD_REQ_MAN_TO_MAN_REFUSAL;		// 거절
		p.u.kein.default_short_int = target_id;
	p.h.header.size = sizeof( short int );
	QueuePacket( &p, 1 );
}

void SendManToManStart( short int target_id  )
{
	t_packet p;		// 서버로 신청

	p.h.header.type = CMD_REQ_MAN_TO_MAN_CONSENT;		// 승낙
		p.u.kein.default_short_int = target_id;
	p.h.header.size = sizeof( short int );
	QueuePacket( &p, 1 );
}

void RecvStartManToMan( int target_id )
{
	LPCHARACTER target = ReturnCharacterPoint( target_id );
	if( !target ) return;

	int x = Hero->x + ( target->x - Hero->x ) / 2;
	int y = Hero->y + ( target->y - Hero->y ) / 2;

	SmallMenuClose();
	MainMenuClose();

	//SMenu[MN_MAN_TO_MAN].bActive = false;
	///////////////////////
	// start 1:1 함수 호출
	MP3( SN_START_FIGHT );
	man_to_man_start = 80;

	SetFightMode( target_id, x, y );
}

void RecvReqManToManRefusal( )
{
	AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(3,40));//010215 lsw
	MenuSoundClose( MN_MAN_TO_MAN );
}

void SendManToManEachItem( short int target_id )
{
	t_packet p;	

	p.h.header.type = CMD_MAN_TO_MAN_ITEM_EACH;
		p.u.kein.default_short_int = target_id;
	p.h.header.size = sizeof( short int );
	QueuePacket( &p, 1 );
}

void SetExchageItemData(const int iExchangeIndex,ItemAttr &item, POS &pos, const bool bSendToServer)//아이템 데이타 셋팅
{
	if(!IsExchangeNow())
	{
//		return;
	}

	if(ExchangeMgr.GetExchangeType() == EXCHANGE_TYPE_MERCHANT_DIRECT
	&&	ExchangeMgr.m_bIsIAmMerchant )
	{//내가 판매자면 아이템 못 올리게 처리 해야하는데..
		return;
	}
	
	if(ExchangeMgr.IsMySideConformStateOk())
	{//if ok state is return
		return;
	}

	int iItemX = 0, iItemY = 0;
	::GetXANDYByExchangeItemIndex(iExchangeIndex,iItemX,iItemY);//놓을때 정렬을 위해서
	DefaultItem *pTarget = &item_give[iExchangeIndex];
	pTarget->x = iItemX;
	pTarget->y = iItemY;
	
	::SetRect( &pTarget->rect, 
		pTarget->x-15,pTarget->y-15,
		pTarget->x+15,pTarget->y+15 );

	pTarget->item.item_attr = item;
	pTarget->item.item_pos = pos;		// 인벤토리 내의 장소를 기억한다.
	::ItemSoundOfGetItem( item.item_no );
	if(bSendToServer)
	{
		::SendExchangeItemEach( item, pos, ExchangeMgr.GetExchangeTargetId() );
		::CheckFaultExchageItemList();
	}
}

void CheckFaultExchageItemList()//주는 리스트중에 이상한 점이 있는가.
{
	for( int t=0; t<MAX_EXCHANGE_ITEM; t++ )
	{
	//	if( t != a && item_give[t].item.item_attr.item_no )
		if( item_give[t].item.item_attr.item_no )
		{
			ItemAttr *ii = ::GetItemByPos( item_give[t].item.item_pos );
			if( !ii || !ii->item_no )
			{
				memset( &item_give[t], 0, sizeof( DefaultItem ) );
			}
		}
	}
}

void GetExplainItemCheckGab(int &iX,int &iY,const int iWillDo)
{
	iX = SMenu[MN_EXCHANGE_ITEM].x+15+5+
		((RIGHT==iWillDo)?SMenu[MN_EXCHANGE_ITEM].nField[1].x:SMenu[MN_EXCHANGE_ITEM].nField[2].x);

	iY = SMenu[MN_EXCHANGE_ITEM].y+15+5+
		((RIGHT==iWillDo)?SMenu[MN_EXCHANGE_ITEM].nField[1].y:SMenu[MN_EXCHANGE_ITEM].nField[2].y);
}

void DisplayExchangeItemList(const int iWillDo, const int iIsRectOn)
{
	const DefaultItem *pTarget = ((RIGHT==iWillDo)?item_give:item_recv);
	
	int iGabX = 0, iGabY = 0;
	::GetExplainItemCheckGab(iGabX ,iGabY,iWillDo);
	
	for( int i=0; i<MAX_EXCHANGE_ITEM; i++ )		// 화면 찍기
	{
		const int iItemNo = pTarget[i].item.item_attr.item_no;
		if( iItemNo )
		{
			::PutItemIcon( iGabX+pTarget[i].x, iGabY+pTarget[i].y, iItemNo );

			if( !g_MouseItemType				
			&&	iIsRectOn
			&&	MouseInRectCheak( iGabX, iGabY, pTarget[i].rect ) )
			{
				::PutItemIcon( iGabX+pTarget[i].x, iGabY+pTarget[i].y, iItemNo, 8);
				::ItemExplain( pTarget[i].item.item_attr );		// 아이템 설명
			}
		}
	}
}

void DeleteItemAtExchangeGiveItemList(const int iWillDo)
{//함부로 호출 하면 안됩니다. case FT_EXCHANGE_ITEM : 에서 딱 한번만 호출 하세요
	if(!IsExchangeNow())
	{
		return;
	}

	if(ExchangeMgr.IsMySideConformStateOk())//My Side Now Ok state
	{
		return;
	}

	if(iWillDo != RIGHT)
	{
		return;
	}

	DefaultItem *pTarget = item_give;
	
	int iGabX = 0,iGabY = 0;
	::GetExplainItemCheckGab(iGabX ,iGabY,iWillDo);

	for( int i=0; i<MAX_EXCHANGE_ITEM; i++ )
	{
		if( !g_MouseItemType//아이템을 들고 있지 않아야 하고
		&&	pTarget[i].item.item_attr.item_no
		&&	::MouseInRectCheak( iGabX, iGabY, pTarget[i].rect ) 
		)	// 내가 준 아이템
		{
			POS pos_t = pTarget[i].item.item_pos;
			ItemAttr *source = ::GetItemByPos( pos_t );

			if( !source || !source->item_no ){break;}
			g_MouseItemType = 1;//손에 들었다
			HandItemAttr = *source;
			IP_base = pos_t;

			::SetItemPos(HAND, &pos_t);
			::SendMoveItem( HandItemAttr.item_no, IP_base, pos_t );
			::SendExchangeItemDel( pTarget[i].item.item_pos, ExchangeMgr.GetExchangeTargetId());
			::memset( &pTarget[i], 0, sizeof( DefaultItem ) );
			::DeleteItem( IP_base );
			break;
		}
	}
}

void TradeONOFF( char *t )//020808 YGI 
{
	t_packet p;
	p.h.header.type = CMD_TRADE_MODE_CHANGE;
	p.h.header.size = sizeof(t_TradeModeChange);
	if( _stricmp( t, "ON" ) == 0 )
	{
		g_Menu_Variable.m_bTradeON = false;
		p.u.Hwoa.rang.TradeModeChange.bIsTradeOn = false;
	}
	else if( _stricmp( t, "OFF" ) == 0 )
	{
		g_Menu_Variable.m_bTradeON = true;
		p.u.Hwoa.rang.TradeModeChange.bIsTradeOn = true;
	}
	else
	{
		g_Menu_Variable.m_bTradeON = !g_Menu_Variable.m_bTradeON;
		p.u.Hwoa.rang.TradeModeChange.bIsTradeOn = g_Menu_Variable.m_bTradeON;
	}
	p.u.Hwoa.rang.TradeModeChange.iCn = Hero->id;
	::QueuePacket( &p, 1 );
}

void RecvCMD_TRADE_MODE_CHANGE(t_packet *p)//030127 lsw //Trade On/Off 셋팅
{
	const bool bIsTradeOn = p->u.Hwoa.rang.TradeModeChange.bIsTradeOn;
	Kein_PutMessage( KM_INFO, kein_GetMenuString( 206 ), bIsTradeOn?"OFF":"ON" );
}