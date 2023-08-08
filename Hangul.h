#define MAX_STRING_IN_DIALBOX	158
//#define Hcolor( a )     ( NHangulColor = a )
extern int	LoadHangulEnglishFont( char*hanfontname, char *engfontname );
extern void  HangulOutputArea( int lx, int rx, int ty, int by );

extern void  EndFont(void );

extern void	Hprint( const int x, const int y, char *destbuf, const char *szStr );
extern void	Hprint2( const int x, const int y, char *destbuf, char *s,... );
extern void	HprintBold(const int x, const int y, const int fc, const int bc, char *s, ... );

extern void	HprintC( int x, int y, char *destbuf, char *s );
extern void	HprintC2( int x, int y, char *destbuf, char *s,... );
extern void  HprintBoldcolor( int x, int y, int percent,  char *s, ... );


extern void Hcolor( WORD color );
extern void Hcolor( int r, int g, int b );
extern void Getcolor( int *r, int *g, int *b );


extern int   NHangulColor;
extern HDC	DDText_dc;
extern int	Cliping_sx,	Cliping_ex,	Cliping_sy,	Cliping_ey;

extern HFONT	__HFontDial[10];
extern HRGN	MainHrgn;
extern HFONT	hFontOld;
extern HRGN	hGrnOld;
extern void SetHangulAlign( int type );
extern int MakeRGB( const WORD wColor);
extern void  SDL_EndFont(void);
extern 	int		NHangulColor;
extern 	HDC		DDText_dc;
extern 	int		Cliping_sx,	Cliping_ex,	Cliping_sy,	Cliping_ey;
extern bool IsHangul( char check ); // CS-030324 // thai2 YGI

/*
	Added by Tya
*/
extern LONG GetTextWidth(char *s, int length, ...);		/* This function returns the width of the printed text */
extern void GetCursorLocation(char *text, int length, int &x, int &y, int width, int height);
extern void MakeCursorUpDown(char *text, int &newCurPos, bool up);
extern void TextOutInRect(char *text, int x, int y, int width, int height);
