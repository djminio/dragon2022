/*

*/

#ifndef	__DRAGON_H__
#define	__DRAGON_H__

#include <windows.h>
#include <windowsx.h>
#include <time.h>

#include "directdraw.h"
#include "directinput.h"
//#include "MenuDef.h"
#include "Network.h"
#include "item.h"
#ifdef _SDL

#include "SDLRenderer.h"
#endif // _SDL

#define	CM_COMMAND			0
#define	CM_MESSAGE			1

#define CONNECT_REFUSE		10101010

////////////// 0427 lkh 추가 /////////////////
#define	DISPLAY_EARTHQUAKE	10
#define	DISPLAY_WAVEEARTH	1

extern HWND					g_hwndMain;
extern HINSTANCE			g_hInstance;
//extern void		SetHMain(const HWND hWndMain);
extern TCHAR				g_szTitle[ MAX_LOADSTRING ];
extern TCHAR				g_szWindowClass[ MAX_LOADSTRING ];
extern TCHAR				g_szCWD[ _MAX_PATH ];

extern BOOL					g_bIsActive;
extern BOOL					g_bCommandMode;

extern DIRECTDRAWINFO		g_DirectDrawInfo;

extern POINT				g_pointMouse, g_pointFuzz;
extern long					g_pointMouseX, g_pointMouseY;
extern int					g_nLButtonState, g_nRButtonState;
extern int					g_nLDButtonState, g_nRDButtonState;
extern int					g_nOldLButtonState, g_nOldRButtonState;
extern int					g_nSensitivity;

extern int					LButtonDownIng, RButtonDownIng;
//extern int					LButtonDown, RButtonDown;
extern DWORD				g_CurrentTime;
extern DWORD				g_ClientTime;	// GameProc()에서 계속적으로 Check 한다. 
extern DWORD				g_ServerClientTimeGap;

extern DWORD				g_curr_time;  //  서버에서보내주는 현재의 시간. 초단위..
extern DWORD				g_packet_recv_send_checktime;
extern DWORD				g_Current_Server_Time;
extern DWORD                g_Current_Server_Day;
////////////////////// 0128 lkh /////////////////////////
extern bool					g_Volume_Off;	//완전히 소리를 죽일까?

extern int					g_Operator_Function;

// ----------------------- khs 0804

extern char				  *g_DestBackBuf, *g_OldDestBackBuf;
extern int					Mox, Moy;
extern int					Mapx, Mapy;
extern LPCHARACTER			Hero;

//-----------------------------------

extern int					QuitFlag;

extern DWORD				g_RecvBytes, g_SendBytes;

extern bool					MouseCursorOnOff;

extern DWORD				g_Inside;

extern int					NowLoading;




/////////////////// 0126 이규훈 //////////////////////
extern short int	g_Current_Volume;	//현재의 사운드 볼륨(0~100사이값)
extern short int	g_To_Volume		;	//변환되어야 하는 (최종적으로) 목표 사운드 볼륨
extern short int	g_Change_Volume	;		//볼륨 수치 변환치(-+ 변화값->Fade Out/In)

extern char IIDD[MAX_PATH];
extern char PPWW[MAX_PATH];

extern int	CheckSumError;	

extern int recvAttackResultCount;
extern FILE *keyplayfp;
extern FILE *keykeyplayfp;

extern bool		g_bIsChristmas;

typedef struct	tagGAMEINFO
{
	char		proxy_ip[3][ IP_LENGTH ];
	int			proxy_port;
	char		agent_ip[IP_LENGTH];
	int			agent_port;
	int			version;

	int			map_port; // 맵서버의 port번호 Port를 알면 MapName도 알수 있다. 

	time_t		timeGame;
	BYTE		cWeather;
	BYTE		cWindVelocity;
	BYTE		cHumidity;

	POINT		pointMouse, pointFuzz;
	int			nLButtonState, nRButtonState;
	int			nOldLButtonState, nOldRButtonState;
	int			nSensitivity;

	
	DWORD		dwFrameRate;
	DWORD		dwFrameCount, dwFrameCount0;
	DWORD		dwFrameTime, dwFrameTime0;

	RECT		rectView, rectScreen;

	LPCHARACTER  lpcharacter;
	LPCHARACTER  lpoldcharacter;

	int			nSelectedSpriteType;
	LPVOID		lpvSelectedSprite;

	int			 PopupFaceCount;
	int			 checkcount; // 마우스와 충돌하면 Count고 5상이되면 화면에 이름을 보여준다. 
	
	// 배경의 다양한 출력을 위한 플레그
	int			displayOption;		
	int			displayOptionDelay;
	//< 040326 kjy
	int			preventSelectYilse;
	//> 040326 kjy
	
} GAMEINFO, *LPGAMEINFO;

extern GAMEINFO		g_GameInfo;

#ifdef __cplusplus
extern "C"
{
#endif

extern void		SetCurrentWorkingDirectory( void );
extern char*	GetCurrentWorkingDirectory( void );
extern void		SetInfoFile( void );
extern char*	GetInfoFile( void );
extern char*	EatFrontWhiteChar( char* );
extern char*	EatRearWhiteChar( char* );
extern BOOL		ShowErrorMessage( char* lpszMessage );

enum eExitAppType
{
	EA_NORMAL = 0,//정상적인 종료.
	EA_CHECK_ACCEL= 1,
	EA_BEFORE_EXE= 2,
	EA_BEFORE_EXE2= 3,
	EA_LOAD_CHAR_SPR_DATA_SUB= 4,
	EA_LOAD_RIDER_SPR_DATA_SUB= 5,
	EA_LOAD_RIDER_BAND_SPR_DATA_SUB= 6,
	EA_LOAD_RIDER_MANTLE_SPR_DATA_SUB= 7,
	EA_LOAD_WEAPON_SPR_DATA= 8,
	EA_LOAD_GUARD_SPR_DATA= 9,
	EA_LOAD_LOACL_WAR_INFO_SPR_DATA= 10,
	EA_LOAD_SPRITE_INFO= 11,
	EA_LOAD_RIDER_GRADE_SPR_DATA_SUB= 12,
	EA_LOAD_HORSE_SPR_DATA_SUB= 13,
	EA_LOAD_HORSE_SADDEL_SPR_DATA_SUB= 14,
	EA_LOAD_HORSE_GRADE_SPR_DATA_SUB= 15,
	EA_LOAD_LOACL_CHAIR_SPR_DATA= 16,
	EA_LOAD_LOACL_SQUAD_SPR_DATA= 17,
	EA_RECV_CONNECT_INFO= 18,
	EA_CPU_CLOCK_ERROR = 19,
	EA_CRACK_THREAD_FIND_CRACK = 20,
	EA_LANGUAGE_ERROR = 21,
	EA_JAPAN_LOGIN_ERROR = 22,
};

extern void		ExitApplication( const eExitAppType eEAType );

extern char*	EatRearWhiteChar( char* pStr );

enum eLoadingGameDataStep
{
	LGDS_FIRST = 0,
	LGDS_STEP1 = 1,
	LGDS_STEP2 = 2,
	LGDS_STEP3 = 3,
	LGDS_STEP4 = 4,
	LGDS_STEP5 = 5,
	LGDS_STEP6 = 6,
	LGDS_STEP7 = 7,
	LGDS_STEP8 = 8,
	LGDS_STEP9 = 9,
	LGDS_STEP10 = 10,
	LGDS_STEP11 = 11,
	LGDS_STEP12 = 12,
	LGDS_END	= 13,
};
extern int		LoadingGameData();

extern bool		chinese_input;
extern void		CheckChineseInput( HWND hwnd, LPARAM lParam );
// lsw
extern bool IsChatBoxActive();
extern void SetChatMode(const int iMode, bool bIsForce = false);

enum eCHATTARGETNORMAL
{
	CHAT_TARGET_NORMAL		= 0,//보통 채팅 모드
	CHAT_TARGET_GUILD		= 1,//길드 채팅 모드
	CHAT_TARGET_PARTY		= 2,// Finito for use with new pary chat type
	SELECT_WISPER_MODE		= 3,//귓말상대 선택 하기
	CHAT_TARGET_WISPER1		= 4,//귓말상대 선택 하기	//필드넘버와 일치
	CHAT_TARGET_WISPER2		= 5,//귓말상대 선택 하기
	CHAT_TARGET_WISPER3		= 6,//귓말상대 선택 하기
	CHAT_TARGET_WISPER4		= 7,//귓말상대 선택 하기
	CHAT_TARGET_WISPER5		= 8,//귓말상대 선택 하기
	CHAT_TARGET_WISPER6		= 9,//귓말상대 선택 하기
	CHAT_TARGET_WISPER7		= 10,//귓말상대 선택 하기
	CHAT_TARGET_WISPER8		= 11,//귓말상대 선택 하기
	CHAT_TARGET_WISPER9		= 12,//귓말상대 선택 하기
	CHAT_TARGET_WISPER10	= 13,//귓말상대 선택 하기
	
	
};
const int CHAT_TARGET_WISPER_START	= CHAT_TARGET_WISPER1;//귓말상대 선택 하기 시작점
const int CHAT_TARGET_WISPER_END	= CHAT_TARGET_WISPER5;//귓말상대 선택 하기 끝점

#ifdef __cplusplus
}
#endif

#endif	// __DRAGON_H__


