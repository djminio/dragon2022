// Chat.h: interface for the CChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAT_H__5DFB83E9_CE7C_4319_A6EF_3E15B16B0566__INCLUDED_)
#define AFX_CHAT_H__5DFB83E9_CE7C_4319_A6EF_3E15B16B0566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "network.h"
#include "menuset.h"
#include "CurrentMsgMgr.h"
//DisplayChatBackCustom와 동일 한 루틴 타입에 NEW_CHAT_IMG,0 +3n 을 넣어주면 된다 7가지 방식 가능

////////////////////////////////////////////////
// yes no 출력 디파인
enum YesOrNoType
{	//< CSD-030806
	YN_NO				=0,
	YN_GUILD_JOIN_OK	=1,		// 길드에 가입할래?
	YN_MERCHANT_REQUIRE_DIRECT_EXCHANGE = 2,//상인에게 직접거래를 요청
};	//> CSD-030806

//그림 번호니까 이거 막 바꾸면 안됩니다.
enum eCHAT_BACK_IMG_COLOR_NUMBER
{
	CHAT_COLOR_TYPE_DEFAULT			=0,
	CHAT_COLOR_TYPE_HIGHLEVEL		=3,
	CHAT_COLOR_TYPE_NPC				=6,
	CHAT_COLOR_TYPE_DUAL_CLASS		=9,
	CHAT_COLOR_TYPE_SIT_DOWN		=12,
	CHAT_COLOR_TYPE_GM				=15,
	CHAT_COLOR_TYPE_TUTORIAL_NPC	=18,
	CHAT_COLOR_TYPE_ANI2			=21,
	CHAT_COLOR_TYPE_ANI3			=24,
	CHAT_COLOR_TYPE_GUILDMASTER		=27,
	CHAT_COLOR_TYPE_MERCHANT_BBS	=30,
};

// FINITO 12/07/07 CHAT COLOURS
extern WORD CHAT_COLOR_NORMAL;
extern WORD CHAT_COLOR_GUILD;
extern WORD CHAT_COLOR_COLOSSUS;
extern WORD CHAT_COLOR_WHISPER;	
extern WORD CHAT_COLOR_WAR;
extern WORD CHAT_COLOR_NATION;
extern WORD CHAT_COLOR_PARTY;
extern WORD CHAT_COLOR_NOTICE;

const int CHAT_DELAY_TYPE_MERCHANT_PC	= 500;//상인공지는 5분 동안 보인다
const int CHAT_DELAY_TYPE_NPC			= 120;
const int CHAT_DELAY_TYPE_LONG_PC		= 80;
const int CHAT_DELAY_TYPE_DEFAULT_PC	= 40;

const int	MAX_CHAT_STRING_				= 150 ;//020515 lsw
const int	MAX_CHAT_COUNT					= 10;//채팅 라인의 최대 라인수
const int	MAX_DISPLAY_CHAT_WIDTH_BY_PIXEL	= 410;
const int	MAX_CHAT_DISPLAY_TIME			= 30000;//화면에 보일 수 있는 시간.
const int	MAX_CHAT_STAY_TIME				= 60000;//화면에는 보이지 않지만 기록이 남아있을 시간,

const int	MAX_NORMAL_COUNT		=	20;
const int	MAX_WHISPER_COUNT		=	20;	
const int	MAX_GUILD_COUNT			=	20;
const int	MAX_GUILDBBS_COUNT		=	20;
const int	MAX_PARTY_COUNT			=	20;
const int	MAX_NATIONWAR_COUNT		=	20;
const int	MAX_LOCALWAR_COUNT		=	20;
const int	MAX_COLOSUSS_COUNT		=	20;
const int	MAX_YES_NO_COUNT		=	1;
const int	MAX_NOTICE_COUNT		=	10;
const int	MAX_NATIONCHAT_COUNT	= 20;
enum ChatGroups
{
	NORMAL_CHAT_GROUP				=0,
	GUILD_CHAT_GROUP				=1,
	WHISPER_CHAT_GROUP				=2,
	CHAT_GROUP_MAX					=3
};//채팅 그룹의 마지막 이다

#include "MyTypes.h"
#include <list>
#include <vector>
using namespace std;
class CChatData
{
public:
	CChatData()
	{
		m_szData = NULL; 
		m_szTotalChatData = NULL; 
		m_szHeader[0] = NULL; 
		m_szName[0] = NULL; 
		m_szChatTime[0] = NULL;
		bIsHaveHeader = false;
	}
	~CChatData()
	{
		SAFE_DELETE_ARRAY(m_szData);
		SAFE_DELETE_ARRAY(m_szTotalChatData);
	}
public:
	int			m_iIndex;
	int			m_iIndex2;
	
private:
	int			m_iChatType;

	WORD		m_wHeaderColor;
	WORD		m_wNameColor;
	WORD		m_wDataColor;	

	bool		m_bIsDisplayAble;

	bool		bIsHaveHeader;

	char		m_szHeader[20];
	char		m_szName[20];
	char		m_szChatTime[10]; // Finito used with /note to show time of message

	char*		m_szData;
	char*		m_szTotalChatData;
	
	DWORD		m_dwWriteTime;
	DWORD		m_dwMaxDisplayTime;//가장 오랫동안 출력 될 수 있는 시간.//이시간이 넘었다고 지워지지는 않음

public:
	void SetData(const WORD wValue, const char* szValue)
	{
		SetData(szValue);
		SetDataColor(wValue);
	}
	void SetName(const WORD wValue, const char* szValue)
	{
		SetName(szValue);
		SetNameColor(wValue);
	}
	void SetHeader(const WORD wValue, const char* szValue)
	{
		SetHeader(szValue);
		SetHeaderColor(wValue);
	}

	void SetDataColor(const WORD wValue){m_wDataColor = wValue;}
	void SetNameColor(const WORD wValue){m_wNameColor = wValue;}
	void SetHeaderColor(const WORD wValue){m_wHeaderColor = wValue;}

	void SetData(const char* szValue)
	{
		const int iDataLen = strlen(szValue);
		if( 0 >= iDataLen ) {return;}//데이타 길이가 이상하면 입력 되면 안된다
		m_szData = new char[iDataLen+1];//+1 은 NULL 문자 iDataLen은 1보다 크다 고로 배열의 형태가 된다 반드시 배열로 new 해야 한다
		strcpy(m_szData,szValue);
	}

	void MakeTotalData()
	{
		char szTemp[600] = {0,};
		( IsHaveHeader() )?
		sprintf(szTemp,"%s %s %s",GetHeader(),GetName(),GetData()):		
		sprintf(szTemp,"%s %s",GetName(),GetData());
		szTemp[599] = 0;
		m_szTotalChatData = new char[strlen(szTemp)+1];
		strcpy(m_szTotalChatData,szTemp);
	}

	void SetName(const char* szValue)
	{
		if( NULL == szValue || NULL== szValue[0]){m_szName[0] = NULL; return;}//셋팅 할 수 없을때
		const int iNameLen = strlen(szValue);
		if(20 > iNameLen && 0 < iNameLen)//이름 길이가 20 보다 작을때
		{
			strcpy(m_szName,szValue);
		}
		else
		{
			{m_szName[0] = NULL; return;}//셋팅 할 수 없을때(길이 오버
		}
	}
	void SetHeader(const char* szValue)
	{
		if( NULL == szValue || NULL== szValue[0]){m_szHeader[0] = NULL; return;}//셋팅 할 수 없을때
		const int iHeaderLen = strlen(szValue);
		if(20 > iHeaderLen && 0 < iHeaderLen)//이름 길이가 20 보다 작을때
		{
			strcpy(m_szHeader,szValue);
			bIsHaveHeader = true;
		}
		else
		{
			{m_szHeader[0] = NULL; return;}//셋팅 할 수 없을때(길이 오버
		}
	}
	void SetChatType(const int iValue,const int iIndex1,const int iIndex2)	//채팅 타입을 지정 하면서 기록 시간 지워질 시간등을 셋팅 한다
	{
		m_iChatType=iValue;
		m_dwWriteTime=timeGetTime();
		m_dwMaxDisplayTime = m_dwWriteTime+MAX_CHAT_DISPLAY_TIME;//20초간 디스플레이 될 수 있음
		m_iIndex = iIndex1;
		m_iIndex2= iIndex2;
		m_bIsDisplayAble = true;
	}
	void SetDisplayAble(const bool bValue){m_bIsDisplayAble=bValue;}
		
	void SetChatTime() // Finito for use with /note gets time of when chat was sent
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		ZeroMemory(m_szChatTime, 10);
		sprintf(m_szChatTime, "%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
	}

public:
	DWORD GetWriteTime()const{return m_dwWriteTime;}
	DWORD GetMaxDisplayTime()const{return m_dwMaxDisplayTime;}

	char* GetData(){return m_szData;}
	char* GetName(){return m_szName;}
	char* GetHeader(){return m_szHeader;}
	char* GetTotalData(){return m_szTotalChatData;}
	char* GetTime(){return m_szChatTime;} // Finto for use with /note returns the time of chat ( when sent )

	bool IsHaveHeader()const{return bIsHaveHeader;}
	bool IsDisplayAble()const{return m_bIsDisplayAble;}
	
	WORD GetDataColor()const{return m_wDataColor;}
	WORD GetNameColor()const{return m_wNameColor;}
	WORD GetHeaderColor()const{return m_wHeaderColor;}
	
	int	GetChatType()const{return m_iChatType;}
	int	GetIndex()const{return m_iIndex;}
	int	GetIndex2()const{return m_iIndex2;}
	
};

typedef list<CChatData*>::iterator chatitor;
typedef list<CChatData*>::reverse_iterator rchatitor;

typedef vector<char*>::iterator vpcitor;
class CChatMgr
{
public:
	CChatMgr()
	{
		m_iAlpha = 3;
		m_iBlendType = 3;
		m_iLineGab = 15;
		m_iWidth = MAX_DISPLAY_CHAT_WIDTH_BY_PIXEL;
		Clear();
	}
	~CChatMgr(){Clear();}

private:
	int	m_iNormalChatView;//보통채팅 본다 안본다
	int	m_iWhisperChatView;//귓말채팅 본다 안본다
	int	m_iGuildChatView;//길드채팅 본다 안본다
	int	m_iPartyChatView;// Finito for use with new party chat
	//카운터들 이름보면 뭔 카운터인지 알겁니다.
	int	m_iTotalCounter;
	int	m_iNormalCounter;
	int	m_iWhisperCounter;	
	int	m_iGuildCounter;
	int	m_iGuildBBSCounter;
	int	m_iPartyCounter;
	int	m_iNationWarCounter;
	int	m_iLocalWarCounter;
	int	m_iNationChatCounter;
	int	m_iColosussCounter;
	int	m_iYesNoCounter;
	int	m_iNoticeCounter;
	
private:
	int m_iAlpha;//알파 값
	int m_iBlendType;//배경 찍히는 블렌딩 타입
	int m_iLineGab;//줄당 간격 14로 고정할 것
	int m_iWidth;//채팅이 찍히는 라인의 픽셀 길이
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//귓속말 관련
private:
	int m_iRefuseAllWhisper;
	vector<char*> vRefuseList;//
public:
	void RecvWhisper( const char *szName, char *szMsg, const WORD dSenderServerIndex, const WORD dSenderCn);// 다른 사람이 보낸 귓속말
	void ReplyWhisper( const WORD dSenderServerIndex, const WORD dSenderCn, const char cResult );
	
	void SetRefuseWhiper(const char* szName) // Finito fixed bugs in funtion added correct client messages.
	{
		if(!szName || !(*szName))//이름이 입력 되지 않으면
		{
			m_iRefuseAllWhisper = 1;//모두 귓말 거부
			m_iWhisperChatView = false;//안봄 
			//::AddCurrentStatusMessage(FONT_COLOR_RED,lan->OutputMessage(2,611));
			AddCurrentStatusMessage( FONT_COLOR_RED , lan->OutputMessage(7,114));//010215 lsw
			return;
		}
		
		if(!IsRefuseGuy(szName))//거부대상이 아니면 추가 시켜준다
		{
			char* pszTemp = new char[strlen(szName)+1];
			strcpy (pszTemp,szName);
			vRefuseList.push_back(pszTemp);
			AddCurrentStatusMessage( FONT_COLOR_GREEN , lan->OutputMessage(7,115), szName );//010215 lsw
		}
		else
			AddCurrentStatusMessage( FONT_COLOR_RED , lan->OutputMessage(7,116), szName );//010215 lsw
		/*
vRefuseList
	if( !*name || !strcmp( name,lan->OutputMessage(7,118)  ))		// 모두 거부
	{
		ChatMgr.RefuseWhiperAll();
		AddCurrentStatusMessage( 200, 0 ,0 , lan->OutputMessage(7,114));//010215 lsw
	}
	else 
	AddCurrentStatusMessage( 250, 0 ,250 , lan->OutputMessage(7,115), name );//010215 lsw
	else
	AddCurrentStatusMessage( 200, 0 ,0 , lan->OutputMessage(7,116), name );//010215 lswAddCurrentStatusMessage( 200, 0 ,0 , lan->OutputMessage(7,116), name );//010215 lsw
*/
	}

	// Finito fixed bugs in SetAcceptWhisper - Was crashing when removing a char name from refuse whisper list
	// Also added the correct client messages.
	void SetAcceptWhiper(const char* szName)
	{
		if(!szName || !(*szName))//이름이 입력 되지 않으면
		{
			m_iRefuseAllWhisper = 0;//모두 귓말 허가
			m_iWhisperChatView = true;
			//::AddCurrentStatusMessage(FONT_COLOR_GREEN,lan->OutputMessage(2,610));
			AddCurrentStatusMessage( FONT_COLOR_GREEN, lan->OutputMessage(7,117) );//021001 lsw
			return;
		}

		for(vpcitor itor = vRefuseList.begin();vRefuseList.end() != itor;itor++)
		{
			if(!strcmp( (*itor),szName))
			{
				SAFE_DELETE((*itor));
				itor = vRefuseList.erase(itor);//이름을 제거
				AddCurrentStatusMessage( FONT_COLOR_GREEN , lan->OutputMessage(7,119), szName );//010215 lsw
				break;
			}
		}
		/*
	if( !*name || !strcmp( name, lan->OutputMessage(7,118) ) )
	{
		AddCurrentStatusMessage( FONT_COLOR_GREEN, lan->OutputMessage(7,117) );//021001 lsw
		ChatMgr.AcceptWhiperAll();
	}
	else
	{
		AddCurrentStatusMessage( 200, 0 ,200 , lan->OutputMessage(7,119), name );//010215 lsw
	}
*/
	}

	void DisplayRefuseList();//귓말 거부 리스트 출력	

	int	 IsRefuseAllWhisper(){return m_iRefuseAllWhisper;}//true값이면 귓말 거부상태
	int  IsRefuseGuy(const char *szName)
	{
		for(vpcitor itor = vRefuseList.begin();vRefuseList.end() != itor;itor++)
		{
			if(!strcmp( (*itor),szName))
			{
				return 1;
			}
		}
		return 0;
	}
	
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//YesOrNo 시스템
private:
	int m_iYesOrNoType;//내가 받은 YesNo의 타입//ex>YN_GUILD_JOIN_OK
	int m_dwSelectWaitTime;//YesNo 고르는 시간(로컬 시간으로 셋팅)
	int m_iYesOrNoDemanderCn;//YesNo를 요구하는 놈의 cn
public:
	void ClearYesNo()
	{
		m_iYesOrNoType			= YN_NO;//내가 받은 YesNo의 타입//ex>YN_GUILD_JOIN_OK
		m_dwSelectWaitTime		= 0;//YesNo 고르는 시간(로컬 시간으로 셋팅)
		m_iYesOrNoDemanderCn	= 0;//YesNo를 요구하는 놈의 cn
	}
	int RecvNeedYesNoMsg( const int iType, const int iDemanderCn, const char *szMsg, ... );
	
	bool IsCheckYesNoAble()
	{

		DWORD dwTemp = m_dwSelectWaitTime;
		int	isss = m_iYesOrNoType;
		return (m_dwSelectWaitTime && m_iYesOrNoType)?true:false;
	}
	
	void CheckYesNo( const int iIsYes );
///////////////////////////////////////////////////////////////


private:
	list<CChatData*> ChatList;//
public:
	void AddString(const char* szName, const char* szChatData, const int iChatType);//채팅 데이타 추가
	void DeleteCheck();//지워질 대상 체크 
	void DisplayCheck();//지워질 대상 체크 
	void DisplayData(const int iX, const int iY,const int iOption = 0);//화면에 뿌리기//
	void SetViewChat(const int iChatType , const int iIsView);//채팅 본다 안본다 체크
	int GetViewChat(const int iChatType );//지금 볼 수 있는 타입 가져옴
	void Clear();
	void DisplayPastData(const int iType);
	int	GetDisplayAbleCounter(){return m_iTotalCounter-MAX_CHAT_COUNT;}// '-값'이되어도 상관 없다
	int	IsDisplayAbleType(const int iChatType);
	int DrawChatData(CChatData* pData, const int iLineCount, const int iX, const int iY);//글자 찍는 부분을 뗀것입니다.//찍은 줄 수를 리턴합니다.


//////////////////////////////////////////////////////////////////////
//1.3 공지 제한 //kyo
private:
	enum{
		MAX_LIMIT_USER	= 20,
	};

	vector<string> m_szLimiteName;
	bool	m_bIsActiveBBS;
public:
	void	ClearLimiteBBS();	
	void	SetActiveBBS( bool bIsActiveBBS ) {m_bIsActiveBBS = bIsActiveBBS; };
	bool	IsActiveBBS() const { return m_bIsActiveBBS ; }
	bool	AddLimintedBBSName(char* szLimitList);
	bool	AddLimintedBBSNameOne( const string szName);
	bool	IsLimitedBBSName( const string szName);	
	void	ShowLimitedList();
//
//////////////////////////////////////////////////////////////////////

};

extern CChatMgr ChatMgr;
extern int GetDisplayChatType( LPCHARACTER ch );
extern void	DisplayChatMessage( LPCHARACTER lpCharacter );

extern bool IsAbleChatTargetName(const int iType);//이칸이 올바른 칸인가(대상지정이 정확한가)
extern int GetChatMode();
extern	int InsertWisperList(const char* name);

extern bool SendChatWisper(const char *name, char *msg);
extern bool SendChatNormal(char* msg);
extern bool SendChatGuild(char *msg );//020515 lsw
extern void SetChatTarget(const int iType);
extern bool IsChatBoxLock();
extern int GetChatTarget();//현재 채팅 상대 가지고 오기

extern void RecvWhisperReply( const int type, const char *szName);		// 귓속말..
extern void RecvWhisperReply2( const int ret );
extern void RecvWhisperReplyEtc( t_packet *p );
extern Spr *chatimage[3*10];
//bool m_bIsBannedUser;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 귓속말 관련 클레스
#endif // !defined(AFX_CHAT_H__5DFB83E9_CE7C_4319_A6EF_3E15B16B0566__INCLUDED_)
