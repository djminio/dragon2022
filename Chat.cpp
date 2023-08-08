// Chat.cpp: implementation of the CChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chat.h"
#include "Hangul.h"
#include "menu.h"
#include "dragon.h"
#include "Hong_Sub.h"
#include "Map.h"
#include "CharDataTable.h"
#include "TeamBattle.h"
#include "GameProc.h"
#include "LineCommand.h"
#include "Tool.h"
#include "Hong_Sprite.h"
#include "directsound.h"
#include "SmallMenu.h"
#include "TextOutMgr.h"
#include "CurrentMsgMgr.h"
#include "QuestFunction.h" // 021128 kyo
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int g_LocalWarBegin;
extern int GetNationIndex(int Nation);
CChatMgr ChatMgr;
WORD CHAT_COLOR_NORMAL				= FONT_COLOR_WHITE;
WORD CHAT_COLOR_GUILD				= FONT_COLOR_RARE_MAIN;
WORD CHAT_COLOR_COLOSSUS			= FONT_COLOR_BLIGHT_VIOLET;
WORD CHAT_COLOR_WHISPER				= FONT_COLOR_GREEN;	
WORD CHAT_COLOR_WAR					= FONT_COLOR_BLIGHT_ORANGE;
WORD CHAT_COLOR_NATION				= FONT_COLOR_BLIGHT_ORANGE;
WORD CHAT_COLOR_PARTY				= FONT_COLOR_BORA;
WORD CHAT_COLOR_NOTICE				= FONT_COLOR_SOLID_YELLOW;
extern TCHAR g_szInfoFile[ _MAX_PATH ];

void CChatMgr::AddString(const char* szName, const char* szChatData, const int iChatType)//채팅 데이타 추가
{
	CChatData *InputData = new CChatData;
	InputData->SetName(FONT_COLOR_NAME,szName);//이름셋팅
	
	switch( iChatType )
	{
	case CHATTYPE_MERCHANT_BBS:
	case CHATTYPE_NORMAL:	
		{//이름 셋팅은 공통이라서 위에서 된다
			InputData->SetChatTime(); // Finto sets recv time of chat for later use with /note
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iNormalCounter);
			InputData->SetData(CHAT_COLOR_NORMAL,szChatData);	
			InputData->SetHeader(CHAT_COLOR_NORMAL,lan->OutputMessage(0,600));
		}break;
	case CHATTYPE_GUILD:	
		{
			InputData->SetChatTime(); // Finto sets recv time of chat for later use with /note
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iGuildCounter);
			InputData->SetData(CHAT_COLOR_GUILD,szChatData);	
			InputData->SetHeader(CHAT_COLOR_GUILD,lan->OutputMessage(0,601));// "길드");
		}break;//귓말은 이제부터옥색
	case CHATTYPE_COLOSSUS_GLOBAL: 
		{
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iColosussCounter);
			InputData->SetData(CHAT_COLOR_COLOSSUS,szChatData);	
			InputData->SetHeader(CHAT_COLOR_COLOSSUS,lan->OutputMessage(0,602));//"아레나1"
		}break;
	case CHATTYPE_COLOSSUS	: 
		{
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iColosussCounter);
			InputData->SetData(CHAT_COLOR_COLOSSUS,szChatData);	
			InputData->SetHeader(CHAT_COLOR_COLOSSUS,lan->OutputMessage(0,603));//"아레나2"
		}break;
	case CHATTYPE_WHISPER:	
		{
			InputData->SetChatTime(); // Finto sets recv time of chat for later use with /note
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iWhisperCounter);
			InputData->SetData(CHAT_COLOR_WHISPER,szChatData);	
			InputData->SetHeader(CHAT_COLOR_WHISPER,lan->OutputMessage(0,604));//"귓말"
		}break;//귓말은 이제부터옥색
	case CHATTYPE_LOCAL_WAR :
		{
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iLocalWarCounter);
			InputData->SetData(CHAT_COLOR_WAR,szChatData);	
			InputData->SetHeader(CHAT_COLOR_WAR,lan->OutputMessage(0,605));//"국지전"
		}break;
	case CHATTYPE_NATION_CHAT://reece sends/receive Nation chat with /n
	{
		InputData->SetChatType(iChatType, ++m_iTotalCounter, ++m_iNationChatCounter);
		InputData->SetData(CHAT_COLOR_NATION, szChatData);
		InputData->SetHeader(CHAT_COLOR_NATION, lan->OutputMessage(0, 600));//"국지전"
	}break;
	case CHATTYPE_NATION_WAR :
		{
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iNationWarCounter);
			InputData->SetData(CHAT_COLOR_WAR,szChatData);	
			InputData->SetHeader(CHAT_COLOR_WAR,lan->OutputMessage(0,606));//"국가전"
		}break;
	case CHATTYPE_PARTY:
		{
			InputData->SetChatTime(); // Finto sets recv time of chat for later use with /note
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iPartyCounter);
			InputData->SetData(CHAT_COLOR_PARTY,szChatData);	
			InputData->SetHeader(CHAT_COLOR_PARTY,lan->OutputMessage(0,607));//"파티"
		}break;

	case CHATTYPE_GUILD_BBS:
		{
			InputData->SetChatTime(); // Finto sets recv time of chat for later use with /note
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iGuildBBSCounter);
			InputData->SetData(CHAT_COLOR_GUILD,szChatData);	
			InputData->SetHeader(CHAT_COLOR_GUILD,lan->OutputMessage(0,608));//"길드공지"
		}break;
	case CHATTYPE_YES_OR_NO:
		{
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iYesNoCounter);
			InputData->SetData(FONT_COLOR_BLIGHT_BLUE,szChatData);	
			InputData->SetHeader(FONT_COLOR_RED,lan->OutputMessage(0,611));//"네/아니오"
		}break;
	case CHATTYPE_LMS:
	case CHATTYPE_NOTICE:
		{
		//	::AddCurrentStatusMessage( 200,200,0,lan->OutputMessage(3,131) );//010215 lsw
		//	::AddCurrentStatusMessage( 255,255,0, szChatData );
			InputData->SetChatType(iChatType,++m_iTotalCounter,++m_iYesNoCounter);
			InputData->SetData(CHAT_COLOR_NOTICE,szChatData);	
			InputData->SetHeader(CHAT_COLOR_NOTICE,lan->OutputMessage(0,612));//"공지사항"
		}break;
	
	default:
		{
			InputData->SetChatType(iChatType,++m_iTotalCounter,m_iTotalCounter);
			InputData->SetData(FONT_COLOR_WHITE,szChatData);	
			InputData->SetHeader(FONT_COLOR_WHITE,"");
		}break;
	}
	InputData->MakeTotalData();
	ChatList.push_back(InputData);//
	int iCount =0;
	for( chatitor itor = ChatList.end() ; itor != ChatList.begin();)
	{
		itor--;
		if( !(*itor)->IsDisplayAble() ){continue;}
		const int iResult = GetRectTextLine((*itor)->GetTotalData(),m_iWidth);//단순히 라인수 세는 부분
		iCount += iResult;//이 줄을 움직이지 마세요 항상 최 하단
		if(iCount > MAX_CHAT_COUNT )
		{
			(*itor)->SetDisplayAble(false);
		}
	}
}

void CChatMgr::DisplayPastData(const int iType)
{
	for( chatitor itor = ChatList.begin() ; itor != ChatList.end();itor++)
	{	//저장 되어 있고 현재 화면에 안나오는것
		if( iType == (*itor)->GetChatType() && !(*itor)->IsDisplayAble() )
		{
			((*itor)->IsHaveHeader())?
			AddCurrentStatusMessage((*itor)->GetHeaderColor(),"[%s] [%s] %s: %s",(*itor)->GetTime(),(*itor)->GetHeader(),(*itor)->GetName(),(*itor)->GetData())
			:AddCurrentStatusMessage((*itor)->GetHeaderColor(),"[%s] %s: %s",(*itor)->GetTime(),(*itor)->GetName(),(*itor)->GetData());
		}
	}
}

void CChatMgr::DeleteCheck()//iIndex2에 따라 각각의 저장 갯수가 넘으면
{
	for( chatitor itor = ChatList.begin() ; itor != ChatList.end();)
	{
		int iCompareCount = 0;//기록 남겨야 할 시작점
		switch((*itor)->GetChatType())
		{
		case CHATTYPE_NORMAL:			{iCompareCount = m_iNormalCounter	- MAX_NORMAL_COUNT;		}break;
		case CHATTYPE_GUILD:			{iCompareCount = m_iGuildCounter	- MAX_GUILD_COUNT;		}break;
		case CHATTYPE_COLOSSUS_GLOBAL: 
		case CHATTYPE_COLOSSUS	:		{iCompareCount = m_iColosussCounter	- MAX_COLOSUSS_COUNT;	}break;
		case CHATTYPE_WHISPER:			{iCompareCount = m_iWhisperCounter	- MAX_WHISPER_COUNT;	}break;
		case CHATTYPE_LOCAL_WAR:		{iCompareCount = m_iLocalWarCounter	- MAX_LOCALWAR_COUNT;	}break;
		case CHATTYPE_NATION_CHAT:      {iCompareCount = m_iNationChatCounter - MAX_NATIONCHAT_COUNT;}break;
		case CHATTYPE_NATION_WAR:		{iCompareCount = m_iNationWarCounter- MAX_NATIONWAR_COUNT;	}break;
		case CHATTYPE_PARTY:			{iCompareCount = m_iPartyCounter	- MAX_PARTY_COUNT;		}break;
		case CHATTYPE_GUILD_BBS:		{iCompareCount = m_iGuildBBSCounter	- MAX_GUILDBBS_COUNT;	}break;
		case CHATTYPE_YES_OR_NO:		{iCompareCount = m_iYesNoCounter	- MAX_YES_NO_COUNT;		}break;
		case CHATTYPE_NOTICE:			{iCompareCount = m_iNoticeCounter	- MAX_NOTICE_COUNT;		}break;
		case CHATTYPE_LMS:				{iCompareCount = m_iNoticeCounter	- MAX_NOTICE_COUNT;		}break;
		default:{iCompareCount= 0;}break;//무조건 지워 져라
		}
		const int iMyCount = (*itor)->GetIndex2();
		if( iMyCount < iCompareCount)//저장될 Max갯수를 넘어갔으면 지운다
		{
			SAFE_DELETE((*itor));
			itor = ChatList.erase(itor);
		}
		else
		{
			itor++;
		}
	}
}

void CChatMgr::DisplayCheck()//전체 인덱스를 체크해서 9줄 넘을거 같으면 DisplayAble을 false로 해 준다
{//DeleteCheck() 여기서 자동으로 지우기를 한다,
	const DWORD dwNowTime = ::timeGetTime();
	
	int iCount = 0;
	for( chatitor itor = ChatList.begin() ; itor != ChatList.end();itor++)
	{
		if( !(*itor)->IsDisplayAble() ){continue;}
		const DWORD dwMaxDisplayTime =	(*itor)->GetMaxDisplayTime();
		const int iDisplayAbleCounter =   GetDisplayAbleCounter();
		if	( (dwNowTime > dwMaxDisplayTime) //디스플레이 시간 오버이거나
//		||	( (*itor)->GetIndex() <= iDisplayAbleCounter)		//화면 출력 해야 하는데 카운터가 모자라면
			)
		{
			(*itor)->SetDisplayAble(false);
		}
	}

	if( m_dwSelectWaitTime && dwNowTime > m_dwSelectWaitTime)
	{//시간 초과 
		ClearYesNo();//클리어~
	}
}

int CChatMgr::DrawChatData(CChatData* pData,const int iLineCount,const int iX, const int iY)//글자 찍는 부분을 뗀것입니다.//찍은 줄 수를 리턴합니다.
{//순수 출력 함수.
	const int iTextX = iX+2;
	const int iTextY = iY+2;

	const int iResult =  GetRectTextLine(pData->GetTotalData(),m_iWidth);//단순히 라인수 세는 부분
	for(int iCt = 0 ; iCt < iResult;iCt++)
	{
		FieldTypeNomalPutFx3( iX, iY+(iLineCount+iCt)*m_iLineGab, 0,0, 11, NEW_CHAT_BACK_IMG, m_iAlpha, m_iBlendType );//이거 어떻게 찍냐 ㅡㅡ;
	}
	Hcolor(pData->GetDataColor());//021030 lsw
	TxtOut.RcTXTOutBNoBG(iTextX, iTextY+iLineCount*m_iLineGab, m_iWidth, 0,"%s",pData->GetTotalData());//색깔별로 찍는 부분 전체

	if(pData->IsHaveHeader())
	{
		Hcolor(pData->GetNameColor());
		TxtOut.RcTXTOutBNoBG(iTextX, iTextY+iLineCount*m_iLineGab, m_iWidth, 0,"%s %s",pData->GetHeader(),pData->GetName());//색깔별로 찍는 부분 전체
		
		Hcolor(pData->GetHeaderColor());
		TxtOut.RcTXTOutBNoBG(iTextX, iTextY+iLineCount*m_iLineGab, m_iWidth, 0,"%s",pData->GetHeader());//색깔별로 찍는 부분 전체
	}
	else
	{
		Hcolor(pData->GetNameColor());
		TxtOut.RcTXTOutBNoBG(iTextX, iTextY+iLineCount*m_iLineGab, m_iWidth, 0,"%s",pData->GetName());//색깔별로 찍는 부분 전체
	}
	return iResult;
}

void CChatMgr::DisplayData( const int iX, const int iY ,const int iOption)
{
	DisplayCheck();//지금 출력 될 수있는 애들 체크//시간이나 라인 숫자를 세어 볼수 있고 없음을 결정.
	DeleteCheck();//지워져야할 애들 체크
	int iNormalGroupCount = 0;
	int iGuildGroupCount = 0;
	int iWhisperGroupCount = 0;
	
	int iNoticeGroupCount = 0;

	//노말, 길드, 귓말 3개다 ON 이면 4 4 2 비율
	//노말, 길드 --> 5 5 비율
	//노말, 귓말 --> 7,3 비율
	//길드, 귓말 --> 7,3 비율
	{//라인수 뽑기 //출력 전혀 없음
		int iLineCount = MAX_CHAT_COUNT;
		// 1. 노말그룹, 길드그룹, 귓말그룹 현재 찍힐수있는 데이타의 라인수를 뽑는다.
		// 2. 귓말그룹, 길드그룹, 노말그룹 순으로 아래에서 부터 찍는다.
		// 3. 귓말 찍을때 위에 길드랑 노말 카운트 더하기 해보고 찍도록 한다.
		for( rchatitor itor = ChatList.rbegin(); itor != ChatList.rend();itor++)//귓말그룹 찍기
		{
			if(!(*itor)->IsDisplayAble()){continue;}//디스플레이 가능한 애들만 보여 준다
			const int iChatType = (*itor)->GetChatType();
			if(!IsDisplayAbleType(iChatType)){continue;	}

			const int iResult = GetRectTextLine((*itor)->GetTotalData(),m_iWidth);//단순히 라인수 세는 부분
			switch(iChatType)
			{
			case CHATTYPE_NORMAL			:
			case CHATTYPE_COLOSSUS_GLOBAL	:
			case CHATTYPE_COLOSSUS			:
			case CHATTYPE_LOCAL_WAR			:
			case CHATTYPE_NATION_CHAT		:
			case CHATTYPE_NATION_WAR		:
				{
					iNormalGroupCount += iResult;
				}break;
			case CHATTYPE_WHISPER			:
			case CHATTYPE_PARTY				:
				{
					iWhisperGroupCount += iResult;
				}break;
			case CHATTYPE_GUILD				:
			case CHATTYPE_GUILD_BBS			://길드 공지는 무조건 들어온다
				{
					iGuildGroupCount += iResult;
				}break;
			case CHATTYPE_LMS				:
			case CHATTYPE_YES_OR_NO			:
			case CHATTYPE_NOTICE			:
				{
					iNoticeGroupCount += iResult;
				}break;
			default:{}break;
			}
		}
	}
	
	int iLineCount = MAX_CHAT_COUNT;//아래에서 부터 찍기 때문에
{
	for( rchatitor itor = ChatList.rbegin(); itor != ChatList.rend();itor++)//공지 찍기
	{
		if(!(*itor)->IsDisplayAble()){continue;}//디스플레이 가능한 애들만 보여 준다
		const int iChatType = (*itor)->GetChatType();
		if(!IsDisplayAbleType(iChatType)){continue;}

		const int iResult = GetRectTextLine((*itor)->GetTotalData(),m_iWidth);//단순히 라인수 세는 부분
		switch(iChatType)
		{
		case CHATTYPE_YES_OR_NO:
			{
				if(!IsCheckYesNoAble())//지금 YesNo 아닌 상태면 출력치 않음
				{
					continue;
				}
			}
		case CHATTYPE_LMS:
		case CHATTYPE_NOTICE:
			{
				const int iCutLineCount = 5;//5줄 다 찍는 수가 있다
				const int iOtherLineCount = MAX_CHAT_COUNT-iCutLineCount;
				if( iCutLineCount < MAX_CHAT_COUNT - iLineCount )//커트라인 까지 찍고
				{
					if(iOtherLineCount <= iNormalGroupCount+iGuildGroupCount)
					{//남은 그릴 수 있는 칸을 넘어선다면
						continue;
					}
					else
					{
						break;//칸을 넘어서지만 않으면 그냥 더 그린다.
					}
				}
			}break;
		default:{continue;}break;
		}
		iLineCount -= iResult;
		DrawChatData((*itor),iLineCount,iX,iY);//이 줄을 움직이지 마세요 항상 최하단 바로위
		if( 0 > iLineCount ){break;}//라인 끝나면 스톱.//최하단
	}
}
{
	for( rchatitor itor = ChatList.rbegin(); itor != ChatList.rend();itor++)//귓말그룹 찍기
	{
		if(!(*itor)->IsDisplayAble()){continue;}//디스플레이 가능한 애들만 보여 준다
		const int iChatType = (*itor)->GetChatType();
		if(!IsDisplayAbleType(iChatType)){continue;}
		const int iResult = GetRectTextLine((*itor)->GetTotalData(),m_iWidth);//단순히 라인수 세는 부분
		switch(iChatType)
		{ 
		case CHATTYPE_WHISPER			:
		case CHATTYPE_PARTY				:
			{
				const int iCutLineCount = (m_iNormalChatView&&m_iGuildChatView)?2:3;//길드 노말 둘다 켜져 있으면 2줄, 아니면 3줄
				const int iOtherLineCount = MAX_CHAT_COUNT-iCutLineCount;
				if( iCutLineCount < MAX_CHAT_COUNT - iLineCount )//커트라인 까지 찍고
				{
					if(iOtherLineCount <= iNormalGroupCount+iGuildGroupCount)
					{//남은 그릴 수 있는 칸을 넘어선다면
						continue;
					}
					else
					{
						break;//칸을 넘어서지만 않으면 그냥 더 그린다.
					}
				}
			}break;
		default:{continue;}break;
		}
		iLineCount -= iResult;
		DrawChatData((*itor),iLineCount,iX,iY);//이 줄을 움직이지 마세요 항상 최하단 바로위
		if( 0 > iLineCount ){break;}//라인 끝나면 스톱.//최하단
	}
}
{
	for( rchatitor itor = ChatList.rbegin(); itor != ChatList.rend();itor++)//길드그룹 찍기
	{
		if(!(*itor)->IsDisplayAble()){continue;}//디스플레이 가능한 애들만 보여 준다
		const int iChatType = (*itor)->GetChatType();
		if(!IsDisplayAbleType(iChatType)){continue;}

		const int iResult = GetRectTextLine((*itor)->GetTotalData(),m_iWidth);//단순히 라인수 세는 부분
		switch(iChatType)
		{
		case CHATTYPE_GUILD:
		case CHATTYPE_GUILD_BBS:
			{//노말 만 꺼지면 7줄, 귓말만 꺼지면 5줄.
				int iCutLineCount =0;
				if(m_iNormalChatView && !m_iWhisperChatView)
				{//노말과 길드가 켜져있다.
					iCutLineCount = 5;
				}
				if(!m_iNormalChatView && m_iWhisperChatView)
				{//길드와 귓말이 켜져있다.
					iCutLineCount = 7;
				}
				if(m_iNormalChatView && m_iWhisperChatView)
				{//전부다 켜져있다.
					iCutLineCount = 4;
				}//노말과 길드 꺼진건 상관 말자 continue가 있으니까.

				const int iOtherLineCount = MAX_CHAT_COUNT-iCutLineCount;
				if( iCutLineCount < MAX_CHAT_COUNT - iLineCount )//커트라인 까지 찍고
				{
					if(iOtherLineCount <= iNormalGroupCount+iWhisperGroupCount)
					{//남은 그릴 수 있는 칸을 넘어선다면
						continue;
					}
					else
					{
						break;//칸을 넘어서지만 않으면 그냥 더 그린다.
					}
				}
			}break;
		default:{continue;}break;
		}
		iLineCount -=iResult;
		DrawChatData((*itor),iLineCount,iX,iY);//이 줄을 움직이지 마세요 항상 최하단 바로위
		if(0 > iLineCount  ){break;}//라인 끝나면 스톱.//최하단
	}
}
{
	for( rchatitor itor = ChatList.rbegin(); itor != ChatList.rend();itor++)//노말그룹 찍기
	{
		if(!(*itor)->IsDisplayAble()){continue;}//디스플레이 가능한 애들만 보여 준다
		const int iChatType = (*itor)->GetChatType();
		if(!IsDisplayAbleType(iChatType)){continue;}
		const int iResult = GetRectTextLine((*itor)->GetTotalData(),m_iWidth);//단순히 라인수 세는 부분
		switch(iChatType)
		{
		case CHATTYPE_NORMAL			:
		case CHATTYPE_COLOSSUS_GLOBAL	:
		case CHATTYPE_COLOSSUS			:
		case CHATTYPE_LOCAL_WAR			:
		case CHATTYPE_NATION_CHAT		:
		case CHATTYPE_NATION_WAR		:
			{//노말 만 꺼지면 7줄, 귓말만 꺼지면 5줄.
				int iCutLineCount =0;
				if(m_iGuildChatView && !m_iWhisperChatView)
				{//노말과 길드가 켜져있다.
					iCutLineCount = 5;
				}
				if(!m_iGuildChatView && m_iWhisperChatView)
				{//노말과 귓말이 켜져있다.
					iCutLineCount = 7;
				}
				if(m_iGuildChatView && m_iWhisperChatView)
				{//전부다 켜져있다.
					iCutLineCount = 4;
				}//길드와 귓말 꺼진건 상관 말자 continue가 있으니까.

				const int iOtherLineCount = MAX_CHAT_COUNT-iCutLineCount;
				if( iCutLineCount < MAX_CHAT_COUNT - iLineCount )//커트라인 까지 찍고
				{
					if(iOtherLineCount <= m_iGuildChatView+iWhisperGroupCount)
					{//남은 그릴 수 있는 칸을 넘어선다면
						continue;
					}
					else
					{
						break;//칸을 넘어서지만 않으면 그냥 더 그린다.
					}
				}
			}break;
		default:{continue;}break;
		}
		iLineCount -=iResult;
		DrawChatData((*itor),iLineCount,iX,iY);//이 줄을 움직이지 마세요 항상 최하단 바로위
		if(0 > iLineCount  ){break;}//라인 끝나면 스톱.//최하단
	}
}
}

int CChatMgr::IsDisplayAbleType(const int iChatType)
{
	switch(iChatType)
	{
	case CHATTYPE_NORMAL			:
	case CHATTYPE_COLOSSUS_GLOBAL	:
	case CHATTYPE_COLOSSUS			:
	case CHATTYPE_LOCAL_WAR			:
	case CHATTYPE_NATION_CHAT		:

	case CHATTYPE_NATION_WAR		:
		{
			return (m_iNormalChatView)?1:0;
		}break;
	case CHATTYPE_WHISPER			:
		{
			return (m_iWhisperChatView)?1:0;
		}break;
	case CHATTYPE_GUILD				:
		{
			return (m_iGuildChatView)?1:0;
		}
	case CHATTYPE_PARTY				: // Finito for use with new party chat type
		{
			return (m_iPartyChatView)?1:0;
		}
	case CHATTYPE_LMS:
	case CHATTYPE_YES_OR_NO:
	case CHATTYPE_NOTICE:
	case CHATTYPE_GUILD_BBS:
		{//공지는 무조건 들어온다
			return 1;
		}break;
	default:{}break;
	}	
	return 0;
}

void CChatMgr::Clear()
{//카운터도 초기화 한다
	m_iTotalCounter		= 0;
	m_iNormalCounter	= 0;
	m_iWhisperCounter	= 0;	
	m_iGuildCounter		= 0;
	m_iGuildBBSCounter	= 0;
	m_iPartyCounter		= 0;
	m_iNationWarCounter	= 0;
	m_iLocalWarCounter	= 0;
	m_iNationChatCounter = 0;
	m_iColosussCounter	= 0;
	m_iYesNoCounter		= 0;
	m_iNoticeCounter	= 0;
	
	for( chatitor itor = ChatList.begin() ; itor != ChatList.end();)
	{
		SAFE_DELETE((*itor));
		itor = ChatList.erase(itor);
	}
	ChatList.clear();

	{//귓말 관련
		m_iRefuseAllWhisper = 0;
		for(vpcitor itor = vRefuseList.begin(); itor !=vRefuseList.end();)
		{
			SAFE_DELETE((*itor));
			vRefuseList.erase(itor);//이름을 제거
		}
	
		vRefuseList.clear();
	}
	ClearYesNo();
	ClearLimiteBBS();	// 031021 kyo //kyo
}

int CChatMgr::GetViewChat(const int iChatType )
{
	switch(iChatType)
	{
	case CHATTYPE_NORMAL:
		{
			return m_iNormalChatView;
		}break;
	case CHATTYPE_WHISPER:
		{
			return !IsRefuseAllWhisper();
		}break;
	case CHATTYPE_GUILD:
		{
			return m_iGuildChatView;
		}break;
	
	case CHATTYPE_PARTY:
		{
			return m_iPartyChatView;
		}break;
	}
	return 0;
}
void CChatMgr::SetViewChat(const int iChatType , const int iIsView)
{
	switch(iChatType)
	{
	case CHATTYPE_NORMAL:
		{
			if(iIsView)
			{
				AddCurrentStatusMessage(FONT_COLOR_GREEN,lan->OutputMessage(2,621));
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_RED,lan->OutputMessage(2,622));
			}
			m_iNormalChatView = iIsView;
		}break;
	case CHATTYPE_WHISPER:
		{
			(iIsView)?SetAcceptWhiper(NULL):SetRefuseWhiper(NULL);
		}break;
	case CHATTYPE_GUILD:
		{
			if(iIsView)
			{
				AddCurrentStatusMessage(FONT_COLOR_GREEN,lan->OutputMessage(2,623));
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_RED,lan->OutputMessage(2,624));
			}
			m_iGuildChatView = iIsView;
		}break;
	case CHATTYPE_PARTY:
		{
			if(iIsView)
			{
				AddCurrentStatusMessage(FONT_COLOR_GREEN,lan->OutputMessage(2,702));
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_RED,lan->OutputMessage(2,703));
			}
			m_iPartyChatView = iIsView;
		}break;
	default:
		{
		}break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendChatData( LPSTR lpStr , const int iChatType)
{		
	if ( lpStr == NULL ){return;}
	
	const int iTempLen = ::strlen( lpStr );
	if ( 0				>= iTempLen 
	||	TEXT_LENGTH		<= iTempLen ){return;}
	
	const int Len = iTempLen+1;//널 문자 포함

	if( !CheckLanguageForMenu( lpStr ) ){return;}	// 서버로 보내지 않는다.

	static DWORD time = 0;
	if( (::timeGetTime() - time) < PACKET_SEND_MINIMUM_TIME ) return;
	time = ::timeGetTime();
	
	t_packet	packet;
	packet.h.header.type = CMD_CHAT_DATA;
	packet.h.header.size = sizeof( t_client_chat_data ) - TEXT_LENGTH + Len;
	packet.u.client_chat_data.type = iChatType; // 전체에게 보낸다. //020701 lsw
	packet.u.client_chat_data.length = Len;
	::strncpy( packet.u.client_chat_data.text, lpStr, Len );
	::QueuePacket( &packet, 1 );

	//서버에서 Data를 받지않고 쓰는즉시 보여준다. 내가쓴것만.. 
	t_server_chat_data p;
	if( Hero )
	{
		p.server_id = Hero->id;
		p.type = iChatType;
		p.length = Len;
		if(iChatType == CHATTYPE_PARTY)
		{
			sprintf( p.text, "%s: %s", Hero->name, lpStr);
		}
		else
		{
			::strcpy( p.text, lpStr );
		}
		::RecvChatData( &p );//데이타를 받은것 처럼
	} 

	if(Hero->viewtype == VIEWTYPE_GHOST_)
	{
		if(	(!::stricmp( lpStr,lan->OutputMessage(5,53) )) 
		|| 	(!::stricmp( lpStr,"The name of Edelblhoy with cosmos and the great storm")))		// 0521 YGI//010216 lsw
		{
			char type = 1;
			::CallServer( CMD_PLEASE_GIVE_LIFE, (char *)&type, sizeof( char ) );
		}
		else if( ::stricmp( lpStr,kein_GetMenuString(32) ) == 0 )		// "붉은 장미와 정의의 오렘의 이름으로"
		{
			char type = 2;
			::CallServer( CMD_PLEASE_GIVE_LIFE, (char *)&type, sizeof( char ) );
		}
#ifdef _DEBUG
		if(	!::stricmp( lpStr, "goodboy" ) )
		{
			packet.h.header.type = CMD_PLEASE_GIVE_LIFE1;
			packet.h.header.size = 0; 
			::QueuePacket( &packet, 1 );
		}
#endif
	}
		
	if( !::stricmp( lpStr, kein_GetMenuString( 33 )  ) )		// 마법 배우기
	{	
		const int mx = Hero->x / TILE_SIZE;
		const int my = Hero->y / TILE_SIZE;
		
		//&& 현재 위치가 마법사 길드이면 
		if(	SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL
			&& MapNumber == 0 && BoxAndDotCrash( 34-3,72-3, 6,6, Hero->x / TILE_SIZE, Hero->y / TILE_SIZE ) && TileMap[ Hero->x / TILE_SIZE][Hero->y / TILE_SIZE].attr_inside )
		{
			::CallSmallMenu(MN_LEARN_MAGIC_BOOK);
		}
		//&& 현재 위치가 세가지 신전중 하나이면
		if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL
			&& MapNumber == 0 &&  TileMap[ mx][ my].attr_inside &&
		(  BoxAndDotCrash( 288-6, 18-6, 15,11, mx, my ) ||
		   BoxAndDotCrash( 327-6, 84-6, 15,11, mx, my ) ||
		   BoxAndDotCrash( 217-6,317-6, 15,11, mx, my ) ) ) 
		{
			CallSmallMenu(MN_LEARN_MAGIC_BOOK);
		}
	}	

	if(!stricmp( lpStr,lan->OutputMessage(5,54) ))
	{
		const int mx = Hero->x / TILE_SIZE;
		const int my = Hero->y / TILE_SIZE;
		if(BoxAndDotCrash( 256,133, 15, 11, mx, my ))
		{
			packet.h.header.type = CMD_QUEST_JOB_FINISH;
			packet.h.header.size = 0; 
			QueuePacket( &packet, 1 );
		}
	}
	// 021127 kyo 
	g_cQuest.SetSpellWord( lpStr ); //script spellmapmove에서 사용
}

void RecvChatData( t_server_chat_data *p )
{	//< CSD-030520		
	char temp[MAX_CHAT_STRING + 20];
		
	if (p->server_id == 1000)
	{
		p->server_id = (short )Hero->id;
	}
		
	LPCHARACTER	ch = ::ReturnCharacterPoint(p->server_id);

	if (ch == NULL)
	{
		return;
	}

	const int iRecvType = p->type;
	if(ch){ch->nChatType = iRecvType;}
	
	switch( iRecvType )			// 010326 YGI
	{
	case CHATTYPE_NORMAL : 	
		{
			if( ch == NULL ) { /*JustMsg( " 어러..." ); */ return; }
			if( Hero->viewtype != VIEWTYPE_GHOST_ )//나는 고스트가 아니고		(난 사람)
			{
				if(	ch->viewtype == VIEWTYPE_GHOST_ ) //다른 사람은 고스트 이며	(딴사람은 유령)
				{
					if(!CanViewGhost(Hero)) //내가 고스르틑 볼 수 없을때				(내가 유령을 못 보면 말도 못해)
					{
						break;
					}
				}
			}
			strcpy( ch->szChatMessage, p->text );
			*(ch->szChatMessage + p->length) = 0;

			switch(GetDisplayChatType(ch))
			{
			case CHAT_COLOR_TYPE_HIGHLEVEL	:
			case CHAT_COLOR_TYPE_NPC		:	
			case CHAT_COLOR_TYPE_GUILDMASTER:	
			case CHAT_COLOR_TYPE_DUAL_CLASS	:	
			case CHAT_COLOR_TYPE_GM			:	
			case CHAT_COLOR_TYPE_SIT_DOWN	:		//011215 lsw
			case CHAT_COLOR_TYPE_ANI2		:	
			case CHAT_COLOR_TYPE_ANI3		:	
				{
					ch->ChatDelay	= CHAT_DELAY_TYPE_LONG_PC;
				}break;
			case CHAT_COLOR_TYPE_TUTORIAL_NPC:	
				{
					ch->ChatDelay	=CHAT_DELAY_TYPE_NPC;
				}break;
			default:
				{
					ch->ChatDelay = CHAT_DELAY_TYPE_DEFAULT_PC;
				}break;
			}
			ChatMgr.AddString( ch->name, p->text, iRecvType );		// 채팅창으로
		}break;
	case CHATTYPE_MERCHANT_BBS://상인의 경우.
		{
			if(	ch->viewtype == VIEWTYPE_GHOST_ ) //다른 사람은 고스트 이며	(딴사람은 유령)
			{
				break;
			}
			wsprintf( temp, lan->OutputMessage(5,55), p->text );//lsw
			
			::strcpy( ch->szChatMessage, p->text );
			*(ch->szChatMessage + p->length) = 0;
			ch->ChatDelay = CHAT_DELAY_TYPE_MERCHANT_PC;
//			ChatMgr.AddString( ch->name, p->text, iRecvType );//채팅창에 띄우지는 않는다
		}break;
	case CHATTYPE_PARTY :	
		{
			::wsprintf( temp, lan->OutputMessage(5,55), p->text );//lsw
			ChatMgr.AddString( NULL, temp, iRecvType );//020612 lsw
		}break;
	case CHATTYPE_WHISPER :	
		{
			::wsprintf(temp, "%s", p->text );
			ChatMgr.AddString( NULL, temp, iRecvType );//021001 lsw
		}break;
	case CHATTYPE_COLOSSUS_GLOBAL:
	case CHATTYPE_COLOSSUS:
		{	
			char* name = ch->name;
			strcpy(ch->szChatMessage, p->text);
			*(ch->szChatMessage + p->length) = 0;
			ch->ChatDelay = 70;

			ChatMgr.AddString(name, p->text, p->type);
			break;
		}
#ifdef _DEBUG
	case CHATTYPE_LMS:
		{
			FILE *fp;
			char temp[MAX_PATH];
			SYSTEMTIME time;
			GetLocalTime(&time);
			sprintf_s( temp, sizeof(temp), "LastManStanding-%2d-%2d-%4d.txt", time.wDay, time.wMonth, time.wYear);
			fp = fopen( temp, "at+" );
			if( !fp ) return;
			fprintf(fp,"%s\n", p->text);
			fclose( fp );

			ChatMgr.AddString(ch->name, p->text, iRecvType);
		}
#endif
	}
}	//> CSD-030520	

void SendChatSet( char chattype, char *name )
{	
	t_packet p;
	char *temp;
	int len;

	switch( chattype )
	{
		case CHATTYPE_GUILD :
		{
			p.h.header.type = CMD_CHAT_SET;
			{
				p.u.chat_set.chattype = chattype;
				p.u.chat_set.chatname[0] = 0;
				len = 0;
			}
			p.h.header.size = sizeof( t_chat_set ) - 21 + len;
			QueuePacket( &p, 1 );
		break;
		}
		case CHATTYPE_PARTY :
		{
			p.h.header.type = CMD_CHAT_SET;
			{
				p.u.chat_set.chattype = chattype;
				p.u.chat_set.chatname[0] = 0;
				len = 0;
			}
			p.h.header.size = sizeof( t_chat_set ) - 21 + len;
			QueuePacket( &p, 1 );
		break;
		}
		case CHATTYPE_WHISPER :
		{
			if( name == NULL ) 
			{
				len = 0;
			}
			else
			{
				len = strlen( name );
				if( len > 20 )
				{
					AddCurrentStatusMessage( 255,0,0, lan->OutputMessage(5,57), name ); //lsw
					Lc_ChatType = CHATTYPE_NORMAL;
					return; 
				}
			}
			char *t = strtok( name, " \t\n" );
			if( t == NULL )
				temp = NULL;
			else 
				temp = name;

			
			if( chattype == CHATTYPE_WHISPER && temp != NULL )
			{
				strcpy( Lc_TempChatName, temp );
			}
				
			p.h.header.type = CMD_CHAT_SET;
				p.u.chat_set.chattype = chattype;
				if( temp == NULL )
				{
					p.u.chat_set.chatname[0] = 0;
					len = 0;
				}
				else 
				{
					strcpy( p.u.chat_set.chatname, temp );
					len = strlen( temp );
				}
			p.h.header.size = sizeof( t_chat_set ) - 21 + len;
			QueuePacket( &p, 1 );
		break;
		}
	}
}

void RecvChatSetResult( t_chat_set_result *p )
{
	switch( p->result ) 
	{
		case 0 :	AddCurrentStatusMessage( 255,0,0, lan->OutputMessage(5,91), Lc_TempChatName );	Lc_ChatType = CHATTYPE_NORMAL;		break; //010216 lsw
		case 1 :	AddCurrentStatusMessage( 255,0,255, lan->OutputMessage(5,92), Lc_TempChatName );	Lc_ChatType = CHATTYPE_WHISPER; 	break;
		case 2 :	AddCurrentStatusMessage( 255,0,255, lan->OutputMessage(5,93) );					Lc_ChatType = CHATTYPE_NORMAL;		break;
		case 3 :	AddCurrentStatusMessage( 255,0,255, lan->OutputMessage(5,93) );					Lc_ChatType = CHATTYPE_NORMAL;		break;

		case 10 :	AddCurrentStatusMessage( 255,0,210, lan->OutputMessage(5,94) );		Lc_ChatType = CHATTYPE_PARTY;		break;
		case 11 :	AddCurrentStatusMessage( 255,0,150, lan->OutputMessage(5,95) );		Lc_ChatType = CHATTYPE_NORMAL;		break;

		case 20 : 	AddCurrentStatusMessage( 100,40,150, lan->OutputMessage(5,96) );									Lc_ChatType = CHATTYPE_NORMAL;		break;
		case 21 :	AddCurrentStatusMessage( 100,40,210, lan->OutputMessage(5,97) );		Lc_ChatType = CHATTYPE_GUILD;		break;//010216 lsw
	}
}	
	
// 001127 KHS		


//010928 lsw
extern Spr *GetSprOfMenu( int type, int nImageNumber );
Spr *chatimage[3*10];

void LoadChatImage()
{
	for( int i = 0; i <NEW_CHAT_SPR_MAX ; i ++)
	{
		chatimage[i]  = GetSprOfMenu(NEW_CHAT_IMG, i);
	}
}

void LoadChatColours() // finito 12/07/07
{
	string strChatSelect;
	stringstream ss;

	int nR,nG,nB;
	for(int i = 0; i <= 7; i++)
	{
		ss << "chat" << i << "r";
		strChatSelect = ss.str();
		nR = GetPrivateProfileInt( "chatcolours", strChatSelect.c_str(), -1, "ChatColour.ini" );
		ss.str("");
		if (nR < 0) continue;

		ss << "chat" << i << "g";
		strChatSelect = ss.str();
		nG = GetPrivateProfileInt( "chatcolours", strChatSelect.c_str(), -1, "ChatColour.ini" );
		ss.str("");
		if (nG < 0) continue;

		ss << "chat" << i << "b";
		strChatSelect = ss.str();
		nB = GetPrivateProfileInt( "chatcolours", strChatSelect.c_str(), -1, "ChatColour.ini" );
		ss.str("");
		if (nB < 0) continue;

		switch (i)
		{
		case 0:
			CHAT_COLOR_NORMAL = ConvertColor(nR,nG,nB);
			break;
		case 1: 
			CHAT_COLOR_GUILD = ConvertColor(nR,nG,nB);
			break;
		case 2:
			CHAT_COLOR_COLOSSUS = ConvertColor(nR,nG,nB);
			break;
		case 3: 
			CHAT_COLOR_WHISPER = ConvertColor(nR,nG,nB);
			break;
		case 4:
			CHAT_COLOR_WAR = ConvertColor(nR,nG,nB);
			break;
		case 5: 
			CHAT_COLOR_PARTY = ConvertColor(nR,nG,nB);
			break;
		case 6:
			CHAT_COLOR_NOTICE = ConvertColor(nR,nG,nB);
			break;

		case 7:
			CHAT_COLOR_NATION = ConvertColor(nR, nG, nB);
		default:
			break;
		}
	}
}

//011215 lsw
int GetDisplayChatType( LPCHARACTER ch )
{	
//	return 12;//디폴트 짧은 그림 일반 저렙
	if( ch->type ==SPRITETYPE_CHARACTER)
	{
		if(ch->nChatType == CHATTYPE_MERCHANT_BBS)
		{
			return CHAT_COLOR_TYPE_MERCHANT_BBS; 
		}
		if(ch->nCurrentAction==ACTION_SITDOWN)
		{
			return CHAT_COLOR_TYPE_SIT_DOWN;	
		}
		if( CheckGuildMaster( ch ) )
		{
			return CHAT_COLOR_TYPE_GUILDMASTER;
		}
		if(	ch->call_job_lv == 100)	
		{
			return CHAT_COLOR_TYPE_GM;
		}//지엠이네..
		if(ch->lv>61)
		{
			return CHAT_COLOR_TYPE_HIGHLEVEL;
		}//까만색 긴거
	}
	else
	{
		if( ch->sprno == 80	||  ch->sprno == 22){	return CHAT_COLOR_TYPE_TUTORIAL_NPC;	}
		if( ch->sprno == 18	||  ch->sprno == 20){	return CHAT_COLOR_TYPE_TUTORIAL_NPC;	}
		
		return CHAT_COLOR_TYPE_NPC;
	}
	return CHAT_COLOR_TYPE_DEFAULT;//디폴트 짧은 그림 일반 저렙
}

void DisplayChatMessage( LPCHARACTER ch )//메세지, 칼라 플래그,
{		
	if( tool_ViewChat ){return;}
	if( 0  >= ch->ChatDelay ){return;}

	int	iPosMoveX = 0, iPosMoveY = 0;
	const int iChatImageType = GetDisplayChatType(ch);
	
	switch(iChatImageType)
	{
	case CHAT_COLOR_TYPE_MERCHANT_BBS://021126 lsw
	case CHAT_COLOR_TYPE_SIT_DOWN	:
		{
			iPosMoveX = -95;
			iPosMoveY = -80;
		}break;
	default:
		{
			iPosMoveX = +13;
			iPosMoveY = -65;
		}break;
	}

	if( ch->viewtype == VIEWTYPE_GHOST_ )
	{	
		iPosMoveY = -90;
	}
	char *pMsg = ch->szChatMessage;
	
	const int iX = ch->x - Mapx+iPosMoveX;
	const int iY = ch->y - Mapy+iPosMoveY;

	int iLCt = 0;
	int iChatWidth = 0;

	int iLGab = CHAT_BG_HEIGHT;//줄 간격

	switch(iChatImageType)
	{//배경 찍기
	case CHAT_COLOR_TYPE_DEFAULT:
		{
			iChatWidth = 135;
		}break;
	case CHAT_COLOR_TYPE_MERCHANT_BBS:
		{
			iLGab = MERCHANT_CHAT_BG_HEIGHT;
		}//break; 없음	
	default:
		{
			iChatWidth = 180;
		}break;
	}

	const int iGab = 10;//들여 쓰기

	TxtOut.Init(COLOR_BLACK, TXT_BG_NORMAL_WIDTH,12);
		
	::Hcolor(FONT_COLOR_WHITE);
	iLCt = TxtOut.RcChatOut(iX, iY, iChatWidth,iGab,iChatImageType,"%s",pMsg);

	::Hcolor(FONT_COLOR_GREEN);
	TxtOut.RcChatHeaderOut(iX, iY-iLGab*(iLCt+1), iChatWidth,iGab,iChatImageType,"%s",ch->name);//iLCt+1 인 이유는 이름 때문
	return;
}

void RecvCMD_LOCALWAR_CHAT(t_packet* p)						// LTS NEW LOCALWAR
{
#ifdef _DEBUG
	AddCurrentStatusMessage( 255,255,0,"Received a Local War Message..."); //Eleval - To see if we actually received it...
#endif
	ChatMgr.AddString( NULL, p->u.LocalWar.LocalWarChat.Message, CHATTYPE_LOCAL_WAR );	//020515 lsw//020702 lsw
}


void RecvCMD_NATION_CHAT(t_packet* p)						// LTS NEW LOCALWAR
{
#ifdef _DEBUG
	AddCurrentStatusMessage(255, 255, 0, "Received a Nation Message..."); //Eleval - To see if we actually received it...
#endif
	ChatMgr.AddString(NULL, p->u.LocalWar.NationChat.Message, CHATTYPE_NATION_CHAT);	//020515 lsw//020702 lsw
}
void RecvCMD_SQUAD_CHAT(t_packet* p)
{
//	char tempMessage[120];
//	char* tempPointer=tempMessage;

//	memcpy(tempMessage,p->u.NationWar.SquadChat.Message,p->u.NationWar.SquadChat.Size);
//	*(tempPointer+p->u.NationWar.SquadChat.Size)='\0';

	p->u.NationWar.SquadChat.Message[MAX_STRING_PK-1] = 0;

	ChatMgr.AddString( NULL, p->u.NationWar.SquadChat.Message, CHATTYPE_NATION_WAR );	//020515 lsw//020702 lsw
	
	switch (p->u.NationWar.SquadChat.Type)
	{
	case 0 :
	case 1 :
	case 2 :
	case 3 :
		PlayListAutoSounds(50,0,0,0);
		break;
	case 4 :
		PlayListAutoSounds(64,0,0,0);
		break;
	}
}

void RecvGuildAllMsg( t_packet *p )
{
	MP3( SN_EAR_MESSAGE_RECV );
	ChatMgr.AddString("!", p->u.kein.default_msg, CHATTYPE_GUILD_BBS);//021001 lsw
}

void CChatMgr::ReplyWhisper( const WORD dSenderServerIndex, const WORD dSenderCn, const char cResult )//  받은 메시지에 대한 결과
{
	t_packet p;
	p.h.header.type = CMD_SEND_EAR_MESSAGE_RESULT;
	p.h.header.size = sizeof( k_send_ear_message_result );

	p.u.kein.send_ear_message_result.send_game_server	=	dSenderServerIndex;
	p.u.kein.send_ear_message_result.send_server_id		=	dSenderCn;
	p.u.kein.send_ear_message_result.ret				=	cResult;

	QueuePacket( &p, 1 );
}

void CChatMgr::RecvWhisper( const char *szName, char *szMsg, const WORD dSenderServerIndex, const WORD dSenderCn)//귓속말 받습니다.
{
	// Finito - Altered if statements so that GM's and RC's can whisper dead / blocking whisper characters
	LPCHARACTER pWhisperChar = ExistHe((char*)szName);
	char cResult = 1;//1은 성공 값이다 초기치를 바꾸지 말것
	if (pWhisperChar == NULL)
	{
		if( ::IsHeroDead()) 
		{
			cResult = 2;
		}//죽었기 때문에 
		
		if( IsRefuseAllWhisper()) 
		{
			cResult = 3;
		}
		if( IsRefuseGuy(szName)) 
		{
			cResult = 3;
		}
	}
	else
	{
		if( ::IsHeroDead() && !pWhisperChar->IsCounselor() && !pWhisperChar->IsReporter() ) 
		{
			cResult = 2;
		}//죽었기 때문에 
		
		if( IsRefuseAllWhisper() && !pWhisperChar->IsCounselor() && !pWhisperChar->IsReporter() ) 
		{
			cResult = 3;
		}
		if( IsRefuseGuy(szName) && !pWhisperChar->IsCounselor() && !pWhisperChar->IsReporter() ) 
		{
			cResult = 3;
		}
	}
	switch(cResult)
	{
	case 1:
		{
			ReplyWhisper( dSenderServerIndex, dSenderCn, cResult );//결과 전송
		}break;
	default:
		{
			ReplyWhisper( dSenderServerIndex, dSenderCn, cResult );//결과 전송
			return;//리턴 시켜버림
		}break;
	}	
	::ConvString( szMsg );
	::MP3( SN_EAR_MESSAGE_RECV );	

	AddString(szName,szMsg,CHATTYPE_WHISPER);//021001 lsw
	::InsertWisperList(szName);//귓말 보낸 사람의 이름 저장
}

int GetChatMode()
{
	if(g_bCommandMode == CM_MESSAGE)
	{
		
	}
	else
	{
		
	}	
	return g_bCommandMode;
}

void SetChatMode(const int iMode,bool bIsForce)//강제로 포커스를 게임메인으로 둘것인지를 셋팅 합니다.
{
	switch(iMode)
	{
	case CM_MESSAGE://엔터 한번에 사짐
		{
			if( SMenu[MN_SKILLGUILD_REPAIR].bActive
			||	SMenu[MN_SKILLGUILD_BUY].bActive 
			||	SMenu[MN_SKILLGUILD_SELL].bActive
			||	SMenu[MN_SKILLGUILD_SKILL].bActive 
			||	SMenu[MN_MEM_BOOK].bActive 
			||	SMenu[MN_MAIL_WRITE].bActive
			||	SMenu[MN_MAIL_READ].bActive
			||	SMenu[MN_MAIL_MENU].bActive
			||	SMenu[MN_DIVIDE_ITEM].bActive
			||	SMenu[MN_FRIEND_REGIST].bActive
			||	SMenu[MN_BANK_REPAYMENT].bActive
			||	SMenu[MN_MERCHANT_REGISTER].bActive
			)
			{
				g_bCommandMode = CM_COMMAND;//커멘드 모드로 만들어 준다
				SMenu[MN_MAININTERFACE].nField[44].nType = FT_NONE;
				SMenu[MN_MAININTERFACE].nField[47].nType = FT_NONE;
				SetFocus2(HWND_GAME_MAIN);//021001 lsw 위 인터페이스상태에서는 채팅 불가능이다.
				return;
			}
//			EWndMgr.ClearAllTxt();//021001 lsw
			
			if(Hero&&Hero->viewtype == VIEWTYPE_OBSERVE_) // 030522 kyo
			{
				return;
			}
			g_bCommandMode = iMode;
		}break;
	case CM_COMMAND://커멘드 모드
		{
			SMenu[MN_MAININTERFACE].nField[44].nType = FT_NONE;
			SMenu[MN_MAININTERFACE].nField[47].nType = FT_NONE;
			g_bCommandMode = iMode;
			if(bIsForce)
			{
				SetFocus2(HWND_GAME_MAIN);//021001 lsw
			}
			return;
		}break;
	default:
		{

		}break;
	}
}

bool IsChatBoxActive()
{
	return (GetChatMode() == CM_MESSAGE)?true:false;
}

int GetChatTarget()//현재 채팅 상대 가지고 오기
{
	const int iType =SMenu[MN_WISPER_LIST].work;
	return iType;
}

bool IsAbleChatTargetName(const int iType)//이칸이 올바른 칸인가
{
	switch(iType)
	{
	case CHAT_TARGET_NORMAL	:
	case CHAT_TARGET_GUILD	:
	case CHAT_TARGET_PARTY	:
	case SELECT_WISPER_MODE	:
		{
			return true;
		}break;
	case CHAT_TARGET_WISPER1			:
	case CHAT_TARGET_WISPER2			:
	case CHAT_TARGET_WISPER3			:
	case CHAT_TARGET_WISPER4			:
	case CHAT_TARGET_WISPER5			:
		{
			if( SMenu[MN_WISPER_LIST].nField[iType].temp[0] )
			{
				return true;
			}
		}break;
	default:
		{
		}break;
	}
	return false;
}

bool IsChatBoxLock()
{
	return (SMenu[MN_MAININTERFACE].nField[47].nWillDo)?true:false;
}


bool SendChatNormal(char* msg)
{
	const int iSize = strlen( msg );
	if(0 >= iSize ){return true;}

	EatRearWhiteChar( msg );//020725 lsw

	if( IsLineCommand( msg, iSize ) ) 
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		return false;		// 명령어라면 처리후 리턴
	}
	ConvString( msg );// 욕설 필터	//020725 lsw
	
	Hero->ChatDelay		= 60;
	strcpy( Hero->szChatMessage, msg );
	EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
	SendChatData( msg );								// 서버에 메시지를 보낸다.
	return true;
}

struct k_guild_chat_map
{
 short int guild_code;
 char name[20];
 char msg[1024];
};

bool SendChatGuild( char *msg )
{
	//if( IsDead( Hero ) ){return true;} //Enable guildchat while dead
	EatRearWhiteChar( msg );//020725 lsw

	const int iSize = strlen( msg );
	if(0 > iSize ){	return true;}
	else if( 0 == iSize )
	{
		SetChatTarget(CHAT_TARGET_GUILD);
		return false;
	}
	
	if( IsLineCommand( msg, iSize) ) 
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		return false;		// 명령어라면 처리후 리턴
	}

	ConvString( msg );// 욕설 필터	//020725 lsw

	g_Menu_Variable.m_bGuildChatOn = true;

	/*packet.h.header.type = 10376; //Eleval to test a bug
	packet.h.header.size = iSize;
	strcpy( (char *)packet.u.data, msg );
	k_guild_chat_map guild_chat_map;
	guild_chat_map = reinterpret_cast<k_guild_chat_map*>(packet.u.data);
	QueuePacket( &packet, 1 );*/

	/*int guild_code = 38;

	 t_packet packet;
	 packet.h.header.type = 10376;
	 packet.h.header.size = iSize + 2 + 20;
	 int offset = 0;
	 memcpy(packet.u.data + offset, &guild_code, 2);
	 offset+=2;
	 memcpy(packet.u.data + offset, "GMTesting", 20);
	 offset+=20;
	 memcpy(packet.u.data + offset, msg, strlen(msg));
	 QueuePacket( &packet, 1 );*/
	
	t_packet packet;
	packet.h.header.type = CMD_GUILD_CHAT;
	packet.h.header.size = iSize;
	strcpy( (char *)packet.u.data, msg );
	QueuePacket( &packet, 1 );
	return true;
}

bool SendChatWisper(const char *name, char *msg)
{
	const int iSize = strlen( msg );
	if(0 >= iSize ){return true;}

	EatRearWhiteChar( msg );//020725 lsw

	if( IsLineCommand( msg, iSize) ) 
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		return false;		// 명령어라면 처리후 리턴
	}

	ConvString( msg );// 욕설 필터//020725 lsw

	const int iNameLen	= strlen(name);
	const int iMsgLen	= strlen(msg);
	if( (20 < iNameLen ) || !iNameLen || (200 <= iMsgLen) )
	{
		AddCurrentStatusMessage( FONT_COLOR_YELLOW,lan->OutputMessage(0,501));//"대상의 이름이 잘못 되었거나, 귓속말이 너무 깁니다."
		return false;//메세지 보내지 않음 
	}
	
	InsertWisperList(name);

	t_packet p;
	p.h.header.type = CMD_SEND_EAR_MESSAGE;
	p.h.header.size = sizeof( k_send_ear_message )-200+iMsgLen;
	strcpy( p.u.kein.send_ear_message.name, name);
	strcpy( p.u.kein.send_ear_message.message, msg);
	QueuePacket( &p, 1 );
	return true;
}

void SendWisperByLineCommand( char *str_buf )//SendEarMessageByLineCommand이거를 변형 해서 현재 채팅창에서 메세지 가도록
{
	char name[20]={0,};
	char msg[200]={0,};

	str_buf = EatFrontWhiteChar(str_buf);
	sscanf( str_buf, "%20s %s", name, msg );

	if(msg[0])//이름 입력 되었고 메세지 첫 부분이 있다면
	{
		char *pdest = strstr( str_buf, msg );
//		const int  result = pdest - string + 1;
		if( NULL != pdest )
		{
			SendChatWisper(name,pdest);
		}
	}
	else
	{
		SetChatMode(CM_MESSAGE);//채팅 입력 대기 상태
		SetChatTarget(SELECT_WISPER_MODE);//귓속말 상대 입력 모드
		EWndMgr.SetTxt(HWND_CHAT_TARGET_INPUT,name);//021001 lsw//입력창은 클리어//이름 입력 해 주고
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw//입력창은 클리어
		if(name[0])//이름 입력이 있다면
		{
			SetFocus2(HWND_CHAT_INPUT);//021001 lsw
		}
		else//이름입력도 없나..
		{	//포커스를 이름 입력으로 준다
			SetFocus2(HWND_CHAT_TARGET_INPUT);//021001 lsw
		}
	}
}

bool SendPartyChatByLineCommand( char *str_buf )//SendEarMessageByLineCommand이거를 변형 해서 현재 채팅창에서 메세지 가도록
{
	const int iSize = strlen( str_buf );
	if(0 >= iSize ){return true;}
	else if( 0 == iSize )
	{
		SetChatTarget(CHAT_TARGET_PARTY);
		return false;
	}

	if( IsLineCommand( str_buf, iSize) ) 
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		return false;		// 명령어라면 처리후 리턴
	}

	if(str_buf[0])//이름 입력 되었고 메세지 첫 부분이 있다면
	{
		EWndMgr.ClearTxt(HWND_CHAT_INPUT);//021001 lsw
		SendChatData( str_buf, CHATTYPE_PARTY);
	}
	return true;
}

void BannedUser(const char* szCharName)
{
	t_packet packet;
	ZeroMemory(&packet, sizeof(packet));
	/*const char* szReason = packet.u.BanAccount.reason;
	const char* duration = packet.u.BanAccount.duration;*/


	packet.h.header.type = CMD_BAN_ACCOUNT;
	packet.h.header.size = sizeof(t_ban_account);
	::QueuePacket(&packet, 1);

	char szTmpName[MAX_NAME_CHARACTER] = { 0, };
	//g_pHero->GetName(szTmpName);
	ExistHe(szTmpName);
	/*if (strcmp(szTmpName, szCharName) == 0)
		m_bIsBannedUser = false;*/
}	//> LTH-040805-KO.

void SendNationChat(char* Arg)		// LTS NEW LOCALWAR	
{
	/*if (g_LocalWarBegin)
	{
		if (Hero->JoinLocalWar)
		{*/
	t_packet packet;
	packet.h.header.type = CMD_NATION_CHAT;

	packet.u.LocalWar.NationChat.NationIndex = GetNationIndex(Hero->name_status.nation);		// Broad Cast Use

	sprintf(packet.u.LocalWar.NationChat.Message, "%s : %s", Hero->name, Arg);
	ConvString(packet.u.LocalWar.NationChat.Message);// 욕설 필터
	EatRearWhiteChar(packet.u.LocalWar.NationChat.Message);

	packet.h.header.size = strlen(packet.u.LocalWar.NationChat.Message) + 1;
	QueuePacket(&packet, 1);

#ifdef _DEBUG
	AddCurrentStatusMessage(255, 255, 0, "Sent a Nation Message..."); //Eleval - To see if we actually sent it...
#endif
}

void SendLocalWarChat(char* Arg)		// LTS NEW LOCALWAR	
{
	if (g_LocalWarBegin)
	{
		if (Hero->JoinLocalWar)
		{
			t_packet packet;
			packet.h.header.type = CMD_LOCALWAR_CHAT;

			packet.u.LocalWar.LocalWarChat.NationIndex = GetNationIndex(Hero->name_status.nation);		// Broad Cast Use

			sprintf(packet.u.LocalWar.LocalWarChat.Message, "%s : %s", Hero->name, Arg);
			ConvString(packet.u.LocalWar.LocalWarChat.Message);// 욕설 필터
			EatRearWhiteChar(packet.u.LocalWar.LocalWarChat.Message);

			packet.h.header.size = strlen(packet.u.LocalWar.LocalWarChat.Message) + 1;
			QueuePacket(&packet, 1);

#ifdef _DEBUG
			AddCurrentStatusMessage(255, 255, 0, "Sent a Local War Message..."); //Eleval - To see if we actually sent it...
#endif
		}
		else
		{
			AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 370));//"국지전에 참여하셔야만 사용할수 있습니다.");
		}
	}
	else AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 371));//"국지전 상황에서만 사용할수 있습니다.");
}
/////////////////////////////////////////////////////////////////////////////////////////////
//Yes or No 시스템
// yes no system ----------------------------------------------------------
//-------------------------------------------------------------------------
int CChatMgr::RecvNeedYesNoMsg( const int iType, const int iDemanderCn, const char *szMsg, ... ) 
{
	if(IsCheckYesNoAble())
	{
#ifdef _DEBUG
		::AddCurrentStatusMessage( FONT_COLOR_BLIGHT_BLUE, "recv Type = %d", iType );
#endif
		::AddCurrentStatusMessage( FONT_COLOR_BLIGHT_BLUE, lan->OutputMessage(2,760));
		return 0;
	}
	char szTemp[512] = {0,};
    va_list arg;
    va_start( arg, szMsg );
	if (arg == NULL)
	{
		sprintf(szTemp, "%s", szMsg);
	}
	else
	{
		vsprintf( szTemp, szMsg, arg );
	}
    va_end( arg );

	m_iYesOrNoType			= iType;
	m_iYesOrNoDemanderCn	= iDemanderCn;
	m_dwSelectWaitTime		= ::timeGetTime()+MAX_CHAT_DISPLAY_TIME;
	
	AddString(NULL,szTemp,CHATTYPE_YES_OR_NO);//메세지 맏은걸 집어 넣어라
	return 1;
}
//////////////////////////////////////////////////////////////////////////		yex_no system

void CChatMgr::CheckYesNo( const int iIsYes )
{
	if(!ChatMgr.IsCheckYesNoAble()){return;}//시간 초과 
	switch( m_iYesOrNoType )
	{
	case YN_GUILD_JOIN_OK : 
		{
			(iIsYes)?
				SendGuildJoinConfirmOk( m_iYesOrNoDemanderCn ):
				SendGuildJoinConfirmNot( m_iYesOrNoDemanderCn );
		}break;
	default:
		{//여기 오면 에러인데.. 
		}break;
	}
	ClearYesNo();//클리어를 시켜준다
}

int CheckLanguageForMenu( LPSTR lpStr ) // 메뉴에서 사용할 언어 시스템
{
	if( ChatMgr.IsCheckYesNoAble() )		// yes, no를 선택해야 할때
	{
		if( stricmp(lpStr,lan->OutputMessage(7,96))==0 
		||	stricmp(lpStr,lan->OutputMessage(7,97))==0 
		||	stricmp(lpStr,lan->OutputMessage(7,98))==0 
		||	stricmp(lpStr,"YES")==0 )
		{
			ChatMgr.CheckYesNo( 1 );
			return 0;
		}
		if(	stricmp(lpStr,lan->OutputMessage(7,99))==0 
		||	stricmp(lpStr,"NO")==0 
		|| 	stricmp(lpStr,lan->OutputMessage(7,100))==0  )//010215 lsw
		{
			ChatMgr.CheckYesNo( 0 );
			return 0;
		}
	}

	if( SCharacterData.believe_god )//&& SCharacterData.faith > 50  )
	{
		switch( SCharacterData.believe_god )//String method for temple
		{
			case EDELBLHOY		:	if( !stricmp( lpStr, lan->OutputMessage(7,93) )) CallServer( CMD_DIVINE_UP ); break;//010215 lsw
			case TEFFERY		:	if( !stricmp( lpStr, lan->OutputMessage(7,94))) CallServer(CMD_DIVINE_UP ); break;//010215 lsw
			case LETTY			:	if( !stricmp( lpStr, lan->OutputMessage(7,95))) CallServer( CMD_DIVINE_UP ); break;//010215 lsw
//			case OREM			:	if( !stricmp( lpStr, lan->OutputMessage(7,95))) CallServer( CMD_DIVINE_UP ); break;//010215 lsw
			default : break;
		}
	}

	const int Len = strlen( lpStr );
	if( g_FightMapStart )
	{
		t_packet packet;
		packet.h.header.type = CMD_CHAT_DATA;
		packet.h.header.size = sizeof( t_client_chat_data ) - TEXT_LENGTH + Len;

		packet.u.client_chat_data.type = CHATTYPE_COLOSSUS; // 전체에게 보낸다. 
		packet.u.client_chat_data.length = Len;
		memcpy( packet.u.client_chat_data.text, lpStr, Len );
		
		QueuePacket( &packet, 1 );
		return 0;
	}
	languageSystem( lpStr );			// 0605 Woo
	return 1;
}

void RecvWhisperReply( const int type, const char *szName )		// 귓속말..		// 같은 서버에 있을 경우 결과
{
	switch( type )
	{
		case CMD_SEND_EAR_MESSAGE_OK	:	::AddCurrentStatusMessage( 255, 0, 255, lan->OutputMessage(5,41), szName); break;//010215 lsw
		case CMD_SEND_EAR_MESSAGE_NOT	:	::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,42), szName); break;//010215 lsw
	}
}

void RecvWhisperReply2( const int ret )
{
	switch( ret )
	{
	case 1:		::AddCurrentStatusMessage( 255, 0, 255, lan->OutputMessage(5,43));	break;//010215 lsw
	case 2:		::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,44));	break;//죽은
	case 3:		::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,45));	break;//거절 
	case 4:		::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,46));	break;//다른국가
	default :	::AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(5,47));	break;//전달실패
	}
}

void RecvWhisperReplyEtc( t_packet *p )
{
	const int ret = p->u.kein.default_char;
	switch( ret )
	{
		case -1 : ::AddCurrentStatusMessage( 255, 10, 20, lan->OutputMessage(3,51)); break;//010215 lsw
		default : break;
	}
}

void CChatMgr::DisplayRefuseList()//귓말 거부 리스트 출력
{
	if( ChatMgr.IsRefuseAllWhisper() ) 
	{
		::AddCurrentStatusMessage( 200, 255 , 200 , lan->OutputMessage(7,120) );//010215 lsw
		return;
	}
	else
	{
		::AddCurrentStatusMessage( 255, 200 , 255 , lan->OutputMessage(7,121) );//010215 lsw
	}

	for(vpcitor itor = vRefuseList.begin();vRefuseList.end() != itor;itor++)
	{
		::AddCurrentStatusMessage( 100, 200 , 100 , lan->OutputMessage(7,122), (*itor) );//010215 lsw
	}		
}

void SetChatTarget(const int iType)
{
	const int iNow = GetChatTarget();
	if(IsAbleChatTargetName(iType) )
	{
		SMenu[MN_WISPER_LIST].work = iType;
		if(SELECT_WISPER_MODE==iType)
		{
			EWndMgr.ClearTxt(HWND_CHAT_TARGET_INPUT);//021001 lsw
			SetFocus2(HWND_CHAT_TARGET_INPUT);//021001 lsw
		}
	}
	else
	{
		SMenu[MN_WISPER_LIST].work = CHAT_TARGET_NORMAL;
	}
}

int InsertWisperList(const char* name)//이름 저장후 저장된 자리를 리턴
{
	int i = CHAT_TARGET_WISPER_START;
	for(;i<CHAT_TARGET_WISPER_END;i++)
	{
		if(!strcmp(SMenu[MN_WISPER_LIST].nField[i].temp,name))//비교결과 이름이 다르다면
		{	//이미 그 이름은 저장 하고있어
			return i;//관둬
		}
		if(!SMenu[MN_WISPER_LIST].nField[i].temp[0])//아예 이름이 없다면
		{
			strcpy(SMenu[MN_WISPER_LIST].nField[i].temp,name);//없는자리에 넣고
			return i;// 넣고 관둬라
		}					
	}
	if(i==CHAT_TARGET_WISPER_END)//넣을 자리가 빽빽 하면
	{
		int activeWhisper;

//		strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER9].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER8].temp);
//		strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER8].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER7].temp);
//		strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER7].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER6].temp);
//		strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER6].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER5].temp);
		if(IsChatBoxActive())
			activeWhisper = GetChatTarget();
		else
			activeWhisper = -1;

		if(activeWhisper != CHAT_TARGET_WISPER5)
		{
			if(activeWhisper != CHAT_TARGET_WISPER4)
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER5].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER4].temp);
			else
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER5].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp);
		}
		if(activeWhisper != CHAT_TARGET_WISPER4)
		{
			if(activeWhisper != CHAT_TARGET_WISPER3)
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER4].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp);
			else
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER4].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp);
		}
		if(activeWhisper != CHAT_TARGET_WISPER3)
		{
			if(activeWhisper != CHAT_TARGET_WISPER2)
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp);
			else
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER1].temp);
		}
		if(activeWhisper != CHAT_TARGET_WISPER2)
		{
			if(activeWhisper != CHAT_TARGET_WISPER1)
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER1].temp);
			else
			{
				strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp,name);
				i = CHAT_TARGET_WISPER2;
			}
		}
		if(activeWhisper != CHAT_TARGET_WISPER1)
		{
			strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER1].temp,name);
			i = CHAT_TARGET_WISPER1;
		}
/*		if(k != CHAT_TARGET_WISPER3) strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER3].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp);
		if(k != CHAT_TARGET_WISPER2) strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER2].temp,SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER1].temp);
		if(k != CHAT_TARGET_WISPER1) strcpy(SMenu[MN_WISPER_LIST].nField[CHAT_TARGET_WISPER1].temp,name);
		i = CHAT_TARGET_WISPER1;*/
	}
	return i;
}

//////////////////////////////////////////////////////////////////////
//1.3 공지 제한
/*
int CChatMgr::CompareString(string szLef, string szRig)//kyo 나중에 checkLimitedBBSName고칠때 삭제한다.
{
	return szLef.compare( szRig);
}

int CompareString(string szLef, string szRig)//kyo 나중에 checkLimitedBBSName고칠때 삭제한다.
{
	return szLef.compare( szRig);
}
*/
void CChatMgr::ClearLimiteBBS()
{
	m_bIsActiveBBS = true;	
	m_szLimiteName.clear();
}

bool CChatMgr::AddLimintedBBSName(char* szLimitList)
{
	if( NULL == szLimitList|| strlen(szLimitList) <= 0)
	{		
		m_szLimiteName.clear();
		::AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(0,511) );
		return true;
	}
	
	int nSize = m_szLimiteName.size();
	char *szName;
	szName = strtok( szLimitList, " " );
	while( szName!= NULL )
	{
		if( MAX_LIMIT_USER < nSize)
		{//제한인원에 걸린다. 
			return false;
		}
		if( AddLimintedBBSNameOne( szName ) )
		{
			nSize++;
		}
		szName = strtok( NULL, " " );
	}
	return true;
}

bool CChatMgr::AddLimintedBBSNameOne( const string szName)
{
	if( !IsLimitedBBSName( szName ) )
	{
		m_szLimiteName.push_back(szName);
		sort(m_szLimiteName.begin(), m_szLimiteName.end());
		return true;
	}	
	return false;
}

bool CChatMgr::IsLimitedBBSName( const string szName )
{
	if( binary_search(m_szLimiteName.begin(), m_szLimiteName.end(), szName))//, ::CompareString) ) //kyo 나중에 Cmp를 다를 함
	{
		return true;
	}
	
	return false;
}

void CChatMgr::ShowLimitedList()
{// not use now.. 031021
	string szNameList;
	for( vector<string>::iterator it = m_szLimiteName.begin(); it != m_szLimiteName.end(); it++)
	{
		szNameList += (*it);
		szNameList +=" ";
	}
	::AddCurrentStatusMessage(0,174,239, "Limited User is %s", szNameList.c_str() );
}

//
//////////////////////////////////////////////////////////////////////