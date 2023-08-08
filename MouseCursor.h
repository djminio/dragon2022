
#define MAX_MOUSE_AREA_BUF_SIZE (150*150*2)
#define MAX_MOUSE_CURSOR		60
	
typedef struct cursorani
{
	int start, end; 
	int offx, offy;
}t_cursorani;

typedef struct tagSKILLCURSOR{	

	LPCHARACTER ch;
	short skillno;
	short cursorno;
	short curframe;
	Spr   *sp;
	short x, y;
	char  repeat;
	DWORD endtime;

	struct tagSKILLCURSOR *prev, *next;
}SKILLCURSOR, *LPSKILLCURSOR;

#define MAX_MQUEUE		64
		
typedef struct {
	int		x, y;
	LPCHARACTER ch;
	bool    lb,rb;
}t_queue;


#ifndef __MOUSE_CURSOR_H__
#define __MOUSE_CURSOR_H__


////////////////////////////////////////////////////
///   Variables..

	//////////////// 0527 lkh Ãß°¡ //////////////////
	extern t_queue		g_queue;

	extern int PmcSx,	PmcSy,PmcEx, PmcEy; 
	extern int MouseCursorFlag;
	
	extern char				MgBuf[MAX_MOUSE_AREA_BUF_SIZE];
	extern t_cursorani		cursorani[ MAX_MOUSE_CURSOR];
	extern LPSKILLCURSOR	g_SkillCursor;

	extern int CurMouseCursorNo;

	//extern t_queue queue[ MAX_MQUEUE];

////////////////////////////////////////////////////
///    User Functions..

extern void LoadCursorAni( void );
//extern int	CursorDisplay( int *newflag );
extern void CursorDisplayBack( void );
extern void PutImageBox( char *backbuf, char *ground, int sx, int sy, int ex, int ey );
extern void CursorStatus( void );
extern void CursorNo( int no  );

extern void LoadMouseCursor( char *filename );
extern void FreeMouseCursor( void );


extern void PushMouseCursor( void );
extern void PopMouseCursor( void );


extern bool IsDoingSkillCursor( LPCHARACTER ch );
extern bool StopDoingSkillCursor( LPCHARACTER ch );
extern LPSKILLCURSOR	AddSkillCursor( LPCHARACTER ch, int skilno, int cursorno, int x, int y, int repeat );
extern void				OutputSkillCursor( void );

extern void ClearQueue( void );
extern int queuein( t_queue *n );
extern int queueout( t_queue *n);


#else


	int PmcSx = SCREEN_WIDTH,	PmcSy = SCREEN_HEIGHT,PmcEx, PmcEy; 
	int MouseCursorFlag;
	
	char			MgBuf[MAX_MOUSE_AREA_BUF_SIZE];
	t_cursorani		cursorani[ MAX_MOUSE_CURSOR];
	LPSKILLCURSOR	g_SkillCursor;


	t_queue aQueue[MAX_MQUEUE];


#endif



