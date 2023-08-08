#if !defined(AFX_READARGUMENT_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_)
#define AFX_READARGUMENT_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dragon.h"
#import ".\thidchk.dll" rename("EOF", "EndOfFile")
#import ".\ClientLogon.dll" no_namespace
#include "chigamec.h"
#ifdef JAPAN_LOCALIZING_
#import ".\NgcEncrypt.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#endif

//2001/02/17 zhh 설명: 미이어 웹측에서 제공하는 함수. 링크에서 Desdll.lib라는 화일을 추가하지 않으면 link error발생함.
BOOL WINAPI DecryptFunc(LPTSTR cipher_text, LPTSTR plain_text );

const int MAX_BILL_BUFFER = 255;
const int MAX_TID = 50;

const int MAX_BILL_CORP	= 16;//2001/02/17 zhh 설명: 현제 종량제 하고 있는 업체의 수. 업체가 추가될때 마다 늘려준다.

//2001/02/17 zhh 설명: WhatLineUse에 들어가는번호. 실제 서버에서는 +1해서 logintable에 집어넣는다. 
//logintable의 0은 업체 없음이다.
enum ePayCompanyType
{
	Nownuri			=0,
	Thrunet			=1,//ip로 체크한다.( 클라리언트 수정 없음 )
	Channel_i		=2,
	Unitel			=3,
	Netsgo			=4,
	KornetWorld		=5,
	Mezzy			=6,
	Wever			=7,
	WiseTop			=8,
	MediaWeb		=9,
	PcBangDotCom	=10,
	OneGame			=11,
	ExciteGame		=12,
	UnitelWeb		=13,//유니텔이긴 하지만 웹을 따로 처리
	Chollian		=14,//천리안
	HyunJuComputer	=15,//현주 컴퓨터
};

extern HINSTANCE hDll;  //전역변수나 멤버로 처리
extern SessionParameter	m_SessParam;

//Unitel
#include <process.h>

#define MAX_USERID				25
#define MAX_USERNAME			30

#define UM_CLOSENOTIFY			(WM_USER + 1052)
#define UM_TERMINATENOTIFY		(WM_USER + 1053)

#define STRING_TERMINATEEVENT	"THREAD_THERMINATEEVENT"

extern "C" typedef HWND			(WINAPI *FnGetHWNDUnimain)(void);
extern "C" typedef LPCTSTR		(WINAPI *FnUNIGetUserID)(void);
extern "C" typedef LPCTSTR		(WINAPI *FnUNIGetUserName)(void);
extern "C" typedef int			(WINAPI *FnUNIGetLoginStatus)(void);

struct STRUCTCHECKUNIPARAM
{
	HWND hWndReceiveWindow; 
	UINT nSendToMessage;
};

class CUnicheck
{
public:
	char	UnitelIP[40];
	BOOL	Uniwin_RunningStatus;
	char	Uniwin_szpUserID[100];
	char	Uniwin_szpUserName[100];

	HWND	Uniwin_m_hWnd;

	BOOL	Uniwin_StartCheckUniwin(HWND hReceiveWindow, UINT nReceiveMessage);		//유니윈 체크용 쓰레드 시작
	void	Uniwin_TerminateCheckUniwin();	//유니윈 체크용 쓰레드 종료.
	void	Uniwin_GetUniwinInfo();		//유니윈으로 부터 아이디나 기타정보를 얻어온다.
	void	Uniwin_Init(HWND hwnd);		//유니윈이 떠있는지 확인하고 정보를 얻어서 만약 인자값으로 넘어온 아이디와 지금 유니윈에서
										//로그인한 아이디가 다르면 프로그램 종료.
	char*	ReturnUniteIP(char *Name);
	CUnicheck();
	~CUnicheck();
};

enum
{
	OP_REQUESTPID,
	OP_LOGONSUCCESSED,		// Logon Successed
	OP_LOGONFAIL,			// Logon Fail
	OP_PIDNOTFOUND,			// PID Not Found
	OP_LIDNOTFOUND,			// LID Not Found
	OP_DUPLICATIONERROR,		// 중복 Error
	OP_SERVERCONNECTFAIL,// GameServer Connection Error
	OP_INCORRECTPWD,		// Password 불량
	OP_INCORRECTVERSION,		// 버전 불량
	OP_OVERUSER,			// 사용자 초과
	OP_NOMONEY,			// 요금 미납 사용자
	OP_BADBOY,				// 신용 불량자
	OP_OVEREXPIRE,			// 정액제시 사용기간 초과
	OP_SOCKETTIMEOUT,
	OP_SOCKETCLOSED,
	OP_BANNED_ACCOUNT
};

#ifndef __CClinetBill__
#define __CClinetBill__
class CClientBill
{
public:
	CClientBill(const char *szLine);
	CClientBill();
	~CClientBill();

public:
	int		CloseSocket();				//종량제용으로 열려있는 소켓을 반환
	char*	Unitel_ID();
	int		ReturnLineNum();			//어느 라인을 사용하고 있는지 WhatLineIUse를 반환한다.
	char*	ReturnCharValue(int Where);
	int		ReturnNumValue(int Where);
	int		ReturnHeader();

	char*	ReturnUserID();				//종량제 업체에 로그인 한 ID
	char*	ReturnUserPW();				//종량제 업체에 로그인 한 PW

	void	DisplayAllCommand();		//디버깅용
	void	HeartBeatToOtherSocket();	//종량제 업체용으로 열린 소켓에다 일정시간마다 메시지를 보낸다.
	void	HeartBeatToGameSocket();	//실제 게임용으로 열린소켓에다 일저잇간마다 메시지를 보내다.

	char*	ReturnIPaddress();			//게이트웨이 아이피 번호
	char*	ReturnPayIPaddress();		//게이트웨이 아이피 번호(만약 게이트 웨이로 새로운 소켓을 열어서 사용하는 것이라면 
										//게이트 웨이의 주소를 반환하고 ReturnIPaddress()는 실제 게임서버의 ip를 반환한다.
	int		ReturnPORTNumber();			//게이트웨이 포트 번호
	int		ReturnPayPORTNumber();		//종량제 업체용 port반환
	int		SendSpecifiedMessage(HWND hwnd);		//해당업체가 게이트웨이서버로 접속한 후 어떤 특정한 정보를 날려주기를 원할경우 날리는 함수
	
#ifdef JAPAN_LOCALIZING_
	char*	GetJapanTID(){return m_szJapanTID;}//일본웹에서 넘긴 ID와 패스워드 저장(암호화 되어 있음)
#endif

private:

#ifdef JAPAN_LOCALIZING_
	INgcEncryptPtr m_pNgcEncrypto;
	char	m_szJapanTID[MAX_TID];//일본웹에서 넘긴 ID와 패스워드 저장(암호화 되어 있음)
#endif
	ILogInPtr pClient;	//Excite Game을 위한 것

	int		IP_pos[MAX_BILL_CORP];	//Command[]에 ip가 들어 있는 위치
	int		Port_pos[MAX_BILL_CORP];	//Command[]에 port가 들어 있는 위치

	int		WhatLineIUse;		//어느 업체의 회선으로	-1 : 종량제로 쓰는 사람이다. -2: 이상한 라인이 넘어 왔다. 에러

	short int UserIDlength;
	char	UserID[40];
	char	UserPW[40];

	char	IP[IP_LENGTH];
	int		port;

	char	ThrunetIP[40];
	int		ThrunetPort;
	SOCKET	sockfd;				//채널아이용//과금용으로 생성된 소켓

	void	initipNport();		//IP_pos , Port_pos를 초기화 한다
	
	int		CommandNum;			//넘어온 인자값의 숫자
	int		CommandNumSlash;			//넘어온 인자값의 숫자
	char**	Command;			//Command로 넘어온 것들을 가리킬 포인터
	char**	CommandSlash;			//Command로 넘어온 것들을 가리킬 포인터
	char*	CommandLine;		//넘어온 인자값
	char*	CommandLineBackup;	//strtok를 실행하면 CommandLine이 변하므로 다시 원상복귀 시키기 위한것

	char*	CommandLineSlash;		//넘어온 인자값 / 로 구분한다
	char*	CommandLineSlashBackup;	//strtok를 실행하면 CommandLine이 변하므로 다시 원상복귀 시키기 위한것

	char	Return[MAX_BILL_BUFFER];		//인자값이나 아이피 주소들을 넣어서 반환한다.

	char	Buffer[MAX_BILL_BUFFER];
	int		Buffer_len;
	int		Buffer_pnt;
	void*	answer;

private:
	int		CheckLine();	//안에 몇개의 코맨드가 넘어 왔는지 체크
	int		PutLine();		//실제로 코맨드값을 넣는 부분
	int		DecideLine();	//주어진 라인를 분석해서 어떤 라인을 쓰고 있는지 확인한다.

	//공통 반환값	0:아니다 1:그렇다.		내부에서 WhatLineIUse값을 바꾼다
	int		Check_NowNuri();		//나 나누누리?
	int		Check_Thrunet();		//나 두루넷?
	int		Check_Channel_i();		//나 채널아이?
	int		Check_Unitel();			//나 유니텔?
	int		Check_Netsgo();			//나 넷츠고?
	int		Check_Mezzy();			//나 메지 시스템

	int		Check_Wever();			//나 위버넷?
	int		Check_WiseTop();		//나 와이즈탑?
	int		Check_MediaWeb();		//나 미디어웹?
	int		Check_PcBangDotCom();	//나 pcbang.com?
	int		Check_Onegame();		//나 원게임?
	int		Check_ExciteGame();		//나 Excite Game( HanMir )

	int		Check_UnitelWeb();		//나 웹이서 들어오는 유니텔?
	int		Check_Chollian();		//나 천리안?
	int		Check_HyunJuComputer();	//나 현주 컴퓨터?


	char*	UnitelWeb_ID();			//유니텔 웹에서 쓰는 아이디를 저장하는 함수
	int		UnitelWeb_IP();			//유니텔측의 ip를 구하는 함수
	char*	ReturnUniteWebIP(char *Name);	//유니텔 웹의 ip를 구하는 함수

	int		ExciteGame_SendMessage();

	int		WiseTop_Port();			//유니텔의 port를 반환
	int		WiseTop_IP();			//유니텔의 ip를 반환	
	int		WiseTop_ID();

	int		Check_KornetWorld();	//나 코넷월드?
	int		KornetWorld_IP();
	int		KornetWorld_Port();
	int		Kornet_SendMessage();

	int		Unitel_Port();			//유니텔의 port를 반환
	int		Unitel_IP();			//유니텔의 ip를 반환	
	int		Unitel_SendMessage();	//

	int		Netsgo_Port();			//넷츠고의 port를 반환
	int		Netsgo_IP();			//넷츠고의 ip를 반환	
	int		Netsgo_AtomTable();		//네츠고에서 넘어온 아톰을 테이블에서 조회한다
	int		Netsgo_SendMessage();	//네츠고에 접속햇을 경우 보내는 메시지

	int		Thrunet_SendMessage();	//두루넷에 접속했을때 보낼 메시지

	int		Channel_i_StartUP();	//채널아이에서 넘어온 부분. 라이브러리 로딩한다.
	int		Channel_i_SendEncryptedData();	//채널아이에서 제공한 dll에서 알아서 아이디와 암호를 전송한다.

	int		EatRearWhiteChar(char *eatit);
	int		Send(char *msg,int len);
	int		SendToPaySocket(char *msg,int len);
	int		Receive(int len);
	bool	Receive_sub( t_connection *c);

	int		MakePaySocket(int How);		//종량제 업체용으로 소켓을 하나 연다.
};
#endif	//__CClinetBill__
extern CClientBill *g_pBill;
extern CUnicheck *g_unitel;
#endif // !defined(AFX_READARGUMENT_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_)