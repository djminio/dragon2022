	/*

	*/

	#ifndef	__GAMEPROC_H__
	#define	__GAMEPROC_H__

	#include "dragon.h"
	#include "CurrentMsgMgr.h"//021025 lsw

	#define	MAX_ORDERTABLE	1000

	typedef struct	tagORDERTABLE
	{
		short		show;
		WORD		wType;
		int			x,y;
		int			height;
		int			offset;
		SIZE		size;
		LPVOID		lpvData;
		LPVOID		*lppvData;
		int			type;
		int			alpha;
		DWORD		rgb;
		int			rank;
		int			sort;
		void		*p;
		int			sprno;

	} ORDERTABLE, *LPORDERTABLE;

	typedef struct	tagORDERINFO
	{
		int			count;
		ORDERTABLE	order[ MAX_ORDERTABLE ];
	} ORDERIFNO, *LPORDERINFO;

	#ifdef __cplusplus
	extern "C"
	{
	#endif

		
	extern BOOL		GameProc( LPDIRECTDRAWINFO lpDirectDrawInfo );
	extern BOOL		GameTestProc( LPDIRECTDRAWINFO lpDirectDrawInfo );
	extern BOOL		StartMenuProc( LPDIRECTDRAWINFO lpDirectDrawInfo );

	extern int		ProtocolProc( t_connection *c, bool flag = true );
	extern int		ProtocolWritingProc( t_connection *c );
	extern void		KeyboardProc( void );
	extern void		MouseProc( void );
	extern void		MapDisplay(int nType);
	extern void		DisplaySpriteUnderTile( void );
	extern void		DisplaySprite( void );
	extern void		DisplaySpriteTrans( void );
	extern void		DisplaySpriteCeiling( void );
	extern void		DisplaySpriteBottom( void );
	//extern void	BuffTimer();
	extern void ShowBuff();
	extern void		AttackProc( LPCHARACTER lpCharacter, int minr, int maxr );
	extern int CheckAgentServer( t_connection *c, char *id, char *password );
////
	
	extern void		smallmap_CalcOrder( void );
	extern void		smallmap_DisplaySprite( void );
	extern void small_DisplaySpriteUnderTile( void );

////
	extern void UnDisplayDestPoing( LPCHARACTER ch );

	//////////////////// SoundUp lkh 수정 //////////////////////
	extern void BackgroundMusic( void );
	extern int	ReturnBGMNumber(int map_Number);
	////////////////////////////////////////////////////////////

	extern bool CheckCharName( char *name );

	extern void RecvDebugReqCharInfo( t_debug_server_req_char_info *p );
	extern void _DisplayDebugCharInfo( void );

	extern ORDERIFNO		g_OrderInfo;
	extern int				g_OldOrderInfoCount;
	
	extern int				NewOrderC;
	extern ORDERTABLE		neworder[ 2000];

	//////////////////// 0430 lkh 추가 //////////////////////////
	extern BOOL				g_AttackSkill_Trace1;
	extern BOOL				g_AttackSkill_Trace2;

	extern int				YouCanHeroActionProc;
	extern int				YouCanViewTipsCheck;
	extern int				ViewTipToggle;		// 0818 khs

	extern int				Moveoldx, Moveoldy;
	extern DWORD			Movetime;


	extern int ChangeMapFlag;					// 0825 KHS
	extern t_server_change_map ChangeMapPacket;

	// 020428 YGI acer
	extern bool IsHidenGhost( LPCHARACTER ch );


	#ifdef __cplusplus
	}
	#endif

	#endif	// __GAMEPROC_H__
