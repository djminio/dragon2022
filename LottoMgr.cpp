// LottoMgr.cpp: implementation of the CLottoMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LottoMgr.h"
#include "Hangul.h"
#include "network.h"
#include "dragon.h"
#include "CurrentMsgMgr.h"
#include "Menu.h"


extern	SMENU SMenu[MAX_MENU];
extern	char  *g_DestBackBuf;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLottoMgr*	CLottoMgr::m_pClass = NULL;

CLottoMgr::CLottoMgr()
{
	Clear();
	m_pClass = this;
	m_nMaxLottoCount = 0;
	this->m_nCrruntPage = 0;
	this->m_nTotalLottoCount = 0;
	this->m_nTotalPage = 0;
}

CLottoMgr::~CLottoMgr()
{
	m_pClass = NULL;
}

void CLottoMgr::Generate6Number()
{
	for(int i = 0;i < m_nMaxLottoCount;i++)
	{
		m_anNumbers[i] = CreateOneNumber();
	}
	m_nInsertCount = m_nMaxLottoCount;

	Sort(m_anNumbers,m_nMaxLottoCount);	
}

int CLottoMgr::GetMinNumber(int aNumbers[], int size)
{
	int nMin = 46;
	for(int i = 0;i < size;i++)
	{
		nMin = __min(aNumbers[i],nMin);
	}

	return nMin;
}


int CLottoMgr::CreateOneNumber()
{
	int nNumber = 0;
	while(1)
	{
		nNumber = RandomNumber();
		if(!IsMember(nNumber))break;
	}
	
	return nNumber;
}

int CLottoMgr::RandomNumber()
{
	int	nNumber = rand()%MAXIMUM_LOTTO_NUM + 1;
	return nNumber;
}

bool CLottoMgr::IsMember(int nNumber)
{
	for(int i = 0;i < m_nMaxLottoCount;i++)
	{
		if(m_anNumbers[i] == nNumber) return true;
	}

	return false;
}

void CLottoMgr::Sort(int aNumbers[], int size)
{
	if(size > 0)
	{
		int nMin = this->GetMinNumber(aNumbers,size);
		int i;
		for(i = 0;i < size;i++)
		{
			if(nMin == aNumbers[i]) break;
		}

		int nTemp = aNumbers[i];
		aNumbers[i] = aNumbers[0];
		aNumbers[0] = nTemp;
		Sort(aNumbers + 1,size - 1);
	}
}

int CLottoMgr::InsertNumber(int nLottoNumber)
{
	if(!IsMember(nLottoNumber))
	{
		if(m_nMaxLottoCount <= m_nInsertCount)return 1;
		m_anNumbers[m_nInsertCount] = nLottoNumber;
		m_nInsertCount++;
		Sort(m_anNumbers,m_nInsertCount);
	}
	else
	{
		if(m_nMaxLottoCount <= m_nInsertCount)return 1;
		return 2;
	}

	return 0;
}

bool CLottoMgr::IsFullNumber()
{
	return (m_nInsertCount >= m_nMaxLottoCount);
}

void CLottoMgr::Clear()
{
	memset(m_anNumbers,0,sizeof(int) * 10);
	memset(m_anWinNumbers,0,sizeof(int) * 10);
	memset(m_anBuyLotto,0,sizeof(int) * 10 * 5);
	m_nInsertCount = 0;
}

int CLottoMgr::GetCurCount()
{
	return m_nInsertCount;
}

void CLottoMgr::LottoMenuTextDisplay(int nMenuNum)
{
	if(nMenuNum == MN_LOTTO_MENU && SMenu[MN_LOTTO_MENU].bActive )
	{
		int x = 330;
		const int y = 320;
		int nGab = 40;

		switch(m_nMaxLottoCount)
		{
		case 3:
			break;
		case 4:
			x = 330;nGab = 40;
			break;
		case 5:
			x = 310;nGab = 40;
			break;
		case 6:
			x = 300;nGab = 35;
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		}

		Hcolor( FONT_COLOR_NAME );
		for(int i = 0;i < m_nMaxLottoCount;i++)
		{
			if(m_anNumbers[i])
			Hprint2(x,y,g_DestBackBuf,"%d",m_anNumbers[i]);
			x += nGab;
		}
	}
	else if(WinnerMenuTextDisplay(nMenuNum))
	{

	}
	else
	{

	}
	
}

void CLottoMgr::SetWinNumbers(int *anNumbers, int nNumCount)
{
	if(nNumCount != m_nMaxLottoCount)return;

	memcpy(m_anWinNumbers,anNumbers,sizeof(int)*m_nMaxLottoCount);
}


bool CLottoMgr::WinnerMenuTextDisplay(int nMenuNum)
{
	if(nMenuNum == MN_WINNER_MENU && SMenu[MN_WINNER_MENU].bActive )
	{
		int x = 238;
		int y = 155;//164;
		int nGabX = 40;
		int	nGabY = 20;

		Hcolor( FONT_COLOR_NAME );
		Hprint2(x,y,g_DestBackBuf,"%s", lan->OutputMessage(4,96));y+= 30;//14;
		if(m_anWinNumbers[0])
		{
			int SaveX = x;
			Hcolor( FONT_COLOR_BLIGHT_ORANGE );
			for(int i = 0;i < 10;i++)
			{
				if(m_anWinNumbers[i])
				{
					Hprint2(SaveX,y,g_DestBackBuf,"%d",m_anWinNumbers[i]);
				}
				else break;

				SaveX += 25;
			}
		}
		else
		{
			Hcolor( FONT_COLOR_BLIGHT_VIOLET );
			Hprint2(x,y,g_DestBackBuf,"%s", lan->OutputMessage(4,98));//아직 추첨이 되지 않았다.
		}
		y+= 30;//16;
		Hcolor( FONT_COLOR_NAME ); //<나의 번호> 출력.
		Hprint2(x,y,g_DestBackBuf,"%s", lan->OutputMessage(4,97));
		
		Hcolor( FONT_COLOR_NUMBER );
		Hprint2(x+130,y+170,g_DestBackBuf,"( %d / %d )",this->m_nCrruntPage,this->m_nTotalPage);//페이지출력.
		y+= 28;		
		for(int i = 0;i < 5;i++)
		{
			if(this->m_anBuyLotto[i][0])
			{
				int saveX = x + 5;
				for(int k = 0;k < m_nMaxLottoCount;k++)
				{
					Hprint2(saveX,y,g_DestBackBuf,"%d",m_anBuyLotto[i][k]);
					saveX += 30;
				}
				y+= 19;
			}
			else break;
		}		

		return true;
	}

	return false;
}

void CLottoMgr::SendOpenWinnerMenu()
{
	// 031110 YGI
#ifdef USA_LOCALIZING_ 
	return ;
#endif

	t_packet	p;
	p.h.header.type = CMD_OPEN_WINNER_MENU;
	p.h.header.size = 0;

#ifndef HONGKONG_LOCALIZING_ //<soto-HK030929 법률 문제로 패치 연기.
	::QueuePacket(&p,1);
#endif//>soto-HK030929
//	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,82));
}

void CLottoMgr::RecvOpenWinnerMenu(t_WINNER_MENU *pOpenMsg)
{
//	if(pOpenMsg->nViewCount == 0)return;//표시할게 없다.
	m_nMaxLottoCount = pOpenMsg->nMaxLottoCount;

	memset(m_anWinNumbers,0,sizeof(int)*10);

	if(pOpenMsg->anWinNumbers[0])
	{
		memcpy(m_anWinNumbers,pOpenMsg->anWinNumbers,sizeof(int)*10);
	}

	m_nViewLottoCount = pOpenMsg->nViewCount;

	memset(m_anBuyLotto,0,sizeof(int) * 10 * 5);
	for(int i = 0;i < pOpenMsg->nViewCount;i++)
	{
		for(int j = 0;j < m_nMaxLottoCount;j++)
		{
			m_anBuyLotto[i][j] = pOpenMsg->anLottoNumber[i][j];
		}
	}

	if(pOpenMsg->nViewCount == 0)
	{
		this->m_nCrruntPage = 0;
		this->m_nTotalLottoCount = 0;
		this->m_nTotalPage = 0;
	}
	else
	{
		this->m_nCrruntPage = 1;
		this->m_nTotalLottoCount = pOpenMsg->nTotalLottoCount;
		this->m_nTotalPage = this->CalPageCount(m_nTotalLottoCount);
	}

	CallSmallMenu(MN_WINNER_MENU);
}

bool CLottoMgr::SendBuyLotto()
{
	if(m_nInsertCount < m_nMaxLottoCount)
	{
		::AddCurrentStatusMessage(255,0,0,lan->OutputMessage(4,85),m_nMaxLottoCount);
		return false;
	}
	t_packet p;

	p.h.header.type = CMD_LOTTO_BUY;
	p.h.header.size = sizeof(t_BUY_LOTTO);

	memset(&p.u.Lotto_Buy,0,sizeof(t_BUY_LOTTO));
	
	strcpy(p.u.Lotto_Buy.strCharName,Hero->name);
	memcpy(&p.u.Lotto_Buy.anLottoNumber,m_anNumbers,sizeof(int) * 10);

	QueuePacket(&p,1);
	return true;
}

void CLottoMgr::RecvBuyLotto(t_BUY_LOTTO *pBuyLotto)
{
	if(pBuyLotto->anLottoNumber[0] <= 0)
	{
		if(pBuyLotto->anLottoNumber[0] == 0)//돈이 모자라다.
		{
			::AddCurrentStatusMessage(255,0,0,lan->OutputMessage(4,94));
		}
		else if(pBuyLotto->anLottoNumber[0] == -1)//사는 기간이 아니다.
		{
			::AddCurrentStatusMessage(255,0,0,lan->OutputMessage(4,89));			
		}
		else if(pBuyLotto->anLottoNumber[0] == -2)//살수 있는 만큼 다샀다.
		{
			::AddCurrentStatusMessage(255,0,0,lan->OutputMessage(4,93));			
		}
		else if(pBuyLotto->anLottoNumber[0] == -3)//중복 번호가 있다.
		{
			::AddCurrentStatusMessage(255,0,0,lan->OutputMessage(4,90));			
		}
	}
	else //샀다.
	{
		char*	pMsg = NULL;
#ifdef	KOREA_LOCALIZING_
		pMsg = "Crit";
#else
		pMsg = "Point";
#endif
		::AddCurrentStatusMessage(0,255,255,lan->OutputMessage(4,95),pBuyLotto->m_nLottoPay,pMsg);//크릿이 소비된다.
		++m_nTotalLottoCount;
		this->m_nTotalPage = this->CalPageCount(m_nTotalLottoCount);
	}	
}

void CLottoMgr::SendCheckWinner()
{
	t_packet	p;
	p.h.header.type = CMD_WINNER_CHECK;
	p.h.header.size = 0;
			
	QueuePacket(&p,1);
}

void CLottoMgr::RecvCheckWinner(t_CHECK_WINNER *pCheckWinner)
{
	if(pCheckWinner->nWinItemCount > 0)//아이템 지급.
	{
		for(int i = 0;i < 5;++i)
		{
			//kr_lan수정 할것."%개의 당첨아이템이 당신의 은행에 보관 되었습니다."
			if(pCheckWinner->anWinItemsCount[i])
			{
				::AddCurrentStatusMessage(0,255,255,lan->OutputMessage(4,88),i+1,i+1,pCheckWinner->anWinItemsCount[i]);
			}
		}
	}
	else if(pCheckWinner->nWinItemCount == 0)//꽝.
	{
		::AddCurrentStatusMessage(0,255,255,lan->OutputMessage(4,87));
	}
	else if(pCheckWinner->nWinItemCount == -1)//공간 부족.
	{
		::AddCurrentStatusMessage(0,255,255,lan->OutputMessage(4,86));//::AddCurrentStatusMessage(0,255,255,lan->OutputMessage(4,86),pCheckWinner->anWinNumbers[0]);//kr_lan수정 할것."당신의 은행에 %개의 빈공간이 필요합니다."
	}
	else if(pCheckWinner->nWinItemCount == -3)//추첨 되지 않았다.
	{
		::AddCurrentStatusMessage(0,255,255,lan->OutputMessage(4,98));
	}
	//<! BBD 040126	누군가 채킹중이다
	else if(pCheckWinner->nWinItemCount == -10)
	{
		::AddCurrentStatusMessage(0,255,255,lan->OutputMessage(4,101));
	}
	//> BBD 040126	누군가 채킹중이다
}

int* CLottoMgr::GetCurLottoNumbers()
{
	return m_anNumbers;
}

void CLottoMgr::ClearCurLottoNumbers()
{
	memset(m_anNumbers,0,sizeof(int) * 10);
	m_nInsertCount = 0;
}

void CLottoMgr::SendOpenLottoMenu()
{
	// 031110 YGI
#ifdef USA_LOCALIZING_ 
	return ;
#endif

#ifndef HONGKONG_LOCALIZING_ 
	{	//< CSD-TW-030625 //<soto-HK030929 법률문제로 패치 연기
		t_packet p;
		p.h.header.type = CMD_OPEN_LOTTOMENU;
		p.h.header.size = 0;
		::QueuePacket(&p,1); //soto-030611 긴급수정
	}	//> CSD-TW-030625 //>soto-HK030929
#endif
}

void CLottoMgr::RecvOpenLottoMenu(t_OPEN_LOTTO_MENU *pOpenLotto)
{
	this->m_nMaxLottoCount = pOpenLotto->nMaxNumberCount;
	CallSmallMenu(MN_LOTTO_MENU);
}

void CLottoMgr::SendLottoViewSeek(bool bNext)
{
	int	nPage = this->m_nCrruntPage;
	
	t_packet	p;
	p.h.header.type = CMD_LOTTO_SEEK;
	p.h.header.size = sizeof(t_LOTTOPAPER_SEEK);

	memset(&p.u.Lotto_Seek,0,sizeof(t_LOTTOPAPER_SEEK));

	
	if(bNext)
	{
		nPage++;
		if( nPage >m_nTotalPage )
		{
			return;		
		}
	}
	else
	{
		nPage--;
		if(nPage < 1) return;
	}

	p.u.Lotto_Seek.nPage = nPage;

	::QueuePacket(&p,1);
}

int CLottoMgr::GetTotalLottoCount()
{
	return this->m_nTotalLottoCount;
}

int CLottoMgr::GetCurrentPage()
{
	return this->m_nCrruntPage;
}

#define ONEPAGE_MAX	5
int CLottoMgr::CalPageCount(int nTotalLottoCount)
{
	int nTotalPage = 0;
	if(nTotalLottoCount <= 0)
	{
		nTotalPage = 0;
	}
	else
	{
		nTotalPage = nTotalLottoCount/ONEPAGE_MAX;
		nTotalPage += (nTotalLottoCount%ONEPAGE_MAX)?1:0;
	}

	return nTotalPage;
}

void CLottoMgr::RecvLottoSeek(t_LOTTOPAPER_SEEK* pSeek)
{
	if(pSeek->nPage <= 0 || pSeek->nViewCount <= 0)return;

	memset(m_anBuyLotto,0,sizeof(int) * 10 * 5);
	
	for(int i = 0;i < pSeek->nViewCount;i++)
	{
		for(int j = 0;j < m_nMaxLottoCount;j++)
		{
			m_anBuyLotto[i][j] = pSeek->anLottoNumber[i][j];
		}
	}
	
	this->m_nCrruntPage = pSeek->nPage;
}
