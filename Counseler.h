
#define GG_ALIVE		 0
#define GG_DEATH		10
#define GG_DEATH_GO		20
#define GG_DEATH_GO1	30
#define GG_DEATH_GO2	40
#define GG_DEATH_GO3	50
#define GG_DEATH_GO4	60
#define GG_DEATH_GO5	70


#define TIPS_X		5	
#define TIPS_Y		12	
#define TIPS_LINE   33	
#define TIPS_XL		800	
#define TIPS_YL		(16 *(TIPS_LINE+2))
#define SMALL_TIPS_LINE		12
						
						
#define TIPS_PRE_X	500	
#define TIPS_PRE_Y	-98	
#define TIPS_NEX_X	500	
#define TIPS_NEX_Y	-98	


#define GG_NO_MORE_DEATH 150


#define	GG_WAV_DIALOG		2017

		
class GuideRookie
{		
public :
		
	GuideRookie( int lv )
	{	
		rookielevel = lv;
		tx =  100;
		ty = -100;
	}	
	~GuideRookie(){}
		
	void GuideGhost_OutPut( void );
	void GuideGhost_PreSet( void );
	void GuideGhost_changexy( int nation, int mapno, int *x, int *y );
	void GuideGhost_TypeTheHelp( bool t );
	int  GuideGhost_CheckGo( void );
		
	void GuideGhost_GetChatMessage( const char *msg, int d = 60 );
	void GuideGhost_DisplayChatMessage( void );
		
protected:
		
	int rookielevel;
	int gx;
	int gy;
		
	int tx;
	int ty;
		
	char szChatMessage[256];
	int  ChatDelay;
		
	short int   GG_delay;
		
	char		GG_oldstatus;
	char		GG_status;
	short int	GG_status_delay;
		
	bool		GG_TypeTheHelp;
		
};	

#ifdef __COUNSELER_CPP__

	bool g_block_move;
	bool g_block_skill;//020821 lsw
	bool g_block_attack;
	bool g_block_magic;
	bool g_block_GiveLife;
	bool g_block_anti_team;
	bool g_absolute_move;
	bool g_absolute_move_dk;
	bool g_absolute_logout;
	bool g_make_ghost;
	bool g_give_life;

	LPCHARACTER lpAbsoluteMoveCh;
	bool		AbsoluteMoveFlag;
	bool		AbsoluteMoveDKFlag;

	int			ViewTipsFlag;


	GuideRookie gr(ROOKIE_LEVEL);

	
#else

	extern bool g_block_move;
	extern bool g_block_attack;
	extern bool g_block_magic;
	extern bool g_block_GiveLife;

	extern bool g_absolute_move;
	extern bool g_absolute_move_dk;
	extern bool g_absolute_logout;
	extern bool g_make_ghost;
	extern bool g_give_life;


	extern LPCHARACTER	lpAbsoluteMoveCh;
	extern bool			AbsoluteMoveFlag;
	extern bool			AbsoluteMoveDKFlag;

	extern int			ViewTipsFlag;

	extern int			ViewTipsCount;	


	extern GuideRookie gr;
						
	extern int			ViewTipsLoad( int n );

	extern void SendBlockAttack( void );
	extern void SendBlockMagic( void );
	extern void SendBlockAntiTeam(void);
	extern void SendBlockMove( void );
	extern void SendBlockSkill(void);//020821 lsw 
	extern void SendBlockGiveLife( void );
	void ViewCounselerStatus( void );
	bool CounselerSelectChar( void );

	extern void Check_Beginer_BBS( void );
	extern void Display_Beginer_BBS( void );


	extern void ViewTips( void );
	extern void ViewTipsNext( void );
	extern int ViewTipsCheck( void );
	extern int ViewTipsLoad( int n );

#endif
