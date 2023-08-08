/*

*/

#ifndef	__DIRECTDRAW_H__
#define	__DIRECTDRAW_H__

#include "ddraw.h"

typedef struct 
{
	RGNDATAHEADER	rgnheader;
	RECT			rect[ 4 ];
} CLIPLIST, *LPCLIPLIST;

typedef struct	tagDIRECTDRAWINFO
{
	LPDIRECTDRAW			lpDirectDraw;
	LPDIRECTDRAWSURFACE 	lpDirectDrawSurfacePrimary;
	LPDIRECTDRAWSURFACE 	lpDirectDrawSurfaceBack;
	LPDIRECTDRAWCLIPPER		lpClipper;
	DWORD					dwFillColor;
	RECT					rectPrimarySurface;
	BOOL					bFullscreen;
	LPDIRECTDRAWCOLORCONTROL lpColorCtrl;
	int						error;
} DIRECTDRAWINFO, *LPDIRECTDRAWINFO;

typedef struct	tagSURFACEINFO
{
	LPDIRECTDRAWSURFACE	lpSurface;
	RECT					rect;
	DWORD					dwFillColor;
} SURFACEINFO, *LPSURFACEINFO;

#ifdef __cplusplus
extern "C"
{
#endif

extern BOOL					InitDirectDraw( HWND hWnd, LPDIRECTDRAWINFO lpDirectDrawInfo );
extern void					CleanupDirectDraw( LPDIRECTDRAWINFO lpDirectDrawInfo );
extern LPDIRECTDRAWSURFACE 	CreateSurface( LPDIRECTDRAW  lpDirectDraw, DWORD dwWidth, DWORD dwHeight );
extern void					CleanupSurface( LPDIRECTDRAWSURFACE lpSurface );
extern BOOL					RestoreSurface( LPDIRECTDRAWSURFACE lpSurface );
extern BOOL					RestoreAllSurfaces( LPDIRECTDRAWINFO lpDirectDrawInfo );
extern void					FlipScreen( LPDIRECTDRAWINFO lpDirectDrawInfo );
extern void					EraseScreen( LPDIRECTDRAWINFO lpDirectDrawInfo, WORD color );

extern BOOL		DDLoadBitmap( LPDIRECTDRAW lpDirectDraw, LPSURFACEINFO lpSurfaceInfo, char* lpszFilePath );
extern BOOL		DDReLoadBitmap( LPDIRECTDRAWSURFACE lpSurface, char* lpszFilePath );
extern BOOL		DDCopyBitmap( LPDIRECTDRAWSURFACE lpSurface, HBITMAP hBitmap, int x, int y, int dx, int dy );
extern DWORD	DDColorMatch( LPDIRECTDRAWSURFACE lpSurface, COLORREF rgb );
extern HRESULT	DDSetColorKey( LPDIRECTDRAWSURFACE lpSurface, COLORREF rgb );

extern void		StretchBltScreen( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc, DWORD dwColorFill, int nDirection );
extern void		BltGrid( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc );
extern void		BltTrans( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc );
extern int		TransAlpha( LPDIRECTDRAWSURFACE lpSurfaceDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LONG x, LONG y, RECT rectSrc, WORD wAlphaValue );


// GammaControl..
extern void		InitGammaControl( void );
extern void		ResetGammaCtrl( void );
extern int		ColorCtrlBrightness( long v );


#ifdef __cplusplus
}
#endif

#endif	// __DIRECTDRAW_H__
