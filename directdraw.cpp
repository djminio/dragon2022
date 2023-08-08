/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/

#include "stdafx.h"
#include <Stdio.h>
#include "dragon.h"
#include "directdraw.h"

#include "MouseCursor.h"

///////////////////////////////////////////////////////////////////////////////
//

BOOL					InitDirectDraw( HWND hWnd, LPDIRECTDRAWINFO lpDirectDrawInfo );
void					CleanupDirectDraw( LPDIRECTDRAWINFO lpDirectDrawInfo );
LPDIRECTDRAWSURFACE		CreateSurface( LPDIRECTDRAW lpDirectDraw, DWORD dwWidth, DWORD dwHeight );
void					CleanupSurface( LPDIRECTDRAWSURFACE lpSurface );
BOOL					RestoreSurface( LPDIRECTDRAWSURFACE lpSurface );
BOOL					RestoreAllSurfaces( LPDIRECTDRAWINFO lpDirectDrawInfo );
void					FlipScreen( LPDIRECTDRAWINFO lpDirectDrawInfo );
void					EraseScreen( LPDIRECTDRAWINFO lpDirectDrawInfo, WORD color );

BOOL	DDLoadBitmap( LPDIRECTDRAW lpDirectDraw, LPSURFACEINFO lpSurfaceInfo, char* lpszFilePath );
BOOL	DDReLoadBitmap( LPDIRECTDRAWSURFACE lpSurface, char* lpszFilePath );
BOOL	DDCopyBitmap( LPDIRECTDRAWSURFACE lpSurface, HBITMAP hBitmap, int x, int y, int dx, int dy );
DWORD	DDColorMatch( LPDIRECTDRAWSURFACE lpSurface, COLORREF rgb );
HRESULT	DDSetColorKey( LPDIRECTDRAWSURFACE lpSurface, COLORREF rgb );

void	StretchBltScreen( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc, DWORD dwColorFill, int nDirection );
void	BltGrid( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc );
void	BltTrans( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc );
int		TransAlpha( LPDIRECTDRAWSURFACE lpSurfaceDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LONG x, LONG y, RECT rectSrc, WORD wAlphaValue );


void	InitGammaControl( void );
void	ResetGammaCtrl( void );
int		ColorCtrlBrightness( long v );


///////////////////////////////////////////////////////////////////////////////
//	
	
BOOL
InitDirectDraw( HWND hWnd, LPDIRECTDRAWINFO lpDirectDrawInfo )
{	
	LPDIRECTDRAW			lpDirectDraw = NULL;
	DDSCAPS					ddscaps;
	DDSURFACEDESC			ddsd;
	CLIPLIST				tClipList;
	RECT					rectClipper;
	HRESULT					hResult;
	
	if(lpDirectDrawInfo->lpDirectDraw != NULL)	return TRUE;

	hResult = DirectDrawCreate( NULL, &lpDirectDrawInfo->lpDirectDraw, NULL );
	if( hResult != DD_OK )
	{
		return	ShowErrorMessage( "DirectDrawCreate Failed!" );
	}
	
//	hResult = lpDirectDraw->QueryInterface( IID_IDirectDraw4, ( LPVOID* )&lpDirectDrawInfo->lpDirectDraw );
//	if ( hResult != DD_OK )
//	{
//		return	ShowErrorMessage( "QueryInterface Failed!" );
//	}
//	lpDirectDraw->Release( );
//	lpDirectDraw = NULL;
	
	if ( lpDirectDrawInfo->bFullscreen )
	{
		hResult = lpDirectDrawInfo->lpDirectDraw->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
		if ( hResult != DD_OK )
		{
			return	ShowErrorMessage( "SetCooperativeLevel Failed!" );
		}
	
#ifdef ALT_TAB_BLOCK
//#ifndef _DEBUG
		while( lpDirectDrawInfo->lpDirectDraw->SetDisplayMode( SCREEN_WIDTH, SCREEN_HEIGHT, 16 ) != DD_OK )
		{
			Sleep( 1000 );
			//count++;
			//if( count > 100 ) return false;
		}
//#endif
//#else
//		hResult = lpDirectDrawInfo->lpDirectDraw->SetDisplayMode( SCREEN_WIDTH, SCREEN_HEIGHT, 16 );
//		if ( hResult != DD_OK )
//		{
//			return	ShowErrorMessage( "SetDisplayMode Failed!" );
//		}
#endif
	
		ZeroMemory( &ddsd, sizeof( ddsd ) );
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_SYSTEMMEMORY;
		ddsd.dwBackBufferCount = 1;
		hResult = lpDirectDrawInfo->lpDirectDraw->CreateSurface( &ddsd, &lpDirectDrawInfo->lpDirectDrawSurfacePrimary, NULL );
		if ( hResult != DD_OK )
		{
			return	ShowErrorMessage( "CreateSurface(Primary) Failed!" );
		}
	
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		hResult = lpDirectDrawInfo->lpDirectDrawSurfacePrimary->GetAttachedSurface( &ddscaps, &lpDirectDrawInfo->lpDirectDrawSurfaceBack );
		if ( hResult != DD_OK )
		{
			return	ShowErrorMessage( "GetAttachedSurface Failed!" );
		}
	
/*	
		DDSURFACEDESC       DDSDesc;
		DDSDesc.dwSize = sizeof(DDSDesc2);
		
		lpDirectDrawInfo->lpDirectDrawSurfacePrimary->Lock(NULL, &DDSDesc, 0, NULL);   // 
		lpDirectDrawInfo->lpDirectDrawSurfacePrimary->Unlock(NULL);
	
		DxSize = DDSDesc.lPitch;

*/
	}
	else
	{
		hResult = lpDirectDrawInfo->lpDirectDraw->SetCooperativeLevel( hWnd, DDSCL_NORMAL );
		if ( hResult != DD_OK )
		{
			return	ShowErrorMessage( "SetCooperativeLevel Failed!" );
		}

		/*
		DWORD	dwStyle;
		RECT	rectWork, rectClient;
		dwStyle = GetWindowStyle( hWnd );
		dwStyle &= ~WS_POPUP;
		dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
		SetWindowLong( hWnd, GWL_STYLE, dwStyle );

		SetRect( &rectClient, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

		AdjustWindowRectEx( &rectClient, GetWindowStyle( hWnd ), GetMenu( hWnd ) != NULL, GetWindowExStyle( hWnd ) );
		SetWindowPos( hWnd, NULL, 0, 0, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
		SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

		SystemParametersInfo( SPI_GETWORKAREA, 0, &rectWork, 0 );
		GetWindowRect( hWnd, &rectClient );
		if ( rectClient.left < rectWork.left )
		{
			rectClient.left = rectWork.left;
		}
		if ( rectClient.top  < rectWork.top )
		{
			rectClient.top  = rectWork.top;
		}
		SetWindowPos( hWnd, NULL, rectClient.left, rectClient.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
		*/

		ZeroMemory( &ddsd, sizeof( ddsd ) );
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_SYSTEMMEMORY;
		hResult = lpDirectDrawInfo->lpDirectDraw->CreateSurface( &ddsd, &lpDirectDrawInfo->lpDirectDrawSurfacePrimary, NULL );
		if ( hResult != DD_OK )
		{
			return	ShowErrorMessage( "CreateSurface(Primary Failed!" );
		}

		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;	
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |  DDSCAPS_SYSTEMMEMORY;
		ddsd.dwWidth = SCREEN_WIDTH;
		ddsd.dwHeight = SCREEN_HEIGHT;
		hResult = lpDirectDrawInfo->lpDirectDraw->CreateSurface( &ddsd, &lpDirectDrawInfo->lpDirectDrawSurfaceBack, NULL );
		if ( hResult != DD_OK )
		{
			return	ShowErrorMessage( "CreateSurface(Back) Failed!" );
		}
	}

	hResult = lpDirectDrawInfo->lpDirectDraw->CreateClipper( 0, &lpDirectDrawInfo->lpClipper, NULL );
	if ( hResult != DD_OK )
	{
		return	ShowErrorMessage( "CreateClipper Failed!" );
	}

	SetRect( &rectClipper, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	tClipList.rgnheader.dwSize = sizeof( RGNDATAHEADER );
	tClipList.rgnheader.iType = RDH_RECTANGLES;
	tClipList.rgnheader.nCount = 1;
	tClipList.rgnheader.nRgnSize = 0;
	memcpy( &tClipList.rgnheader.rcBound, &rectClipper, sizeof( RECT ) );
	memcpy( &tClipList.rect, &rectClipper, sizeof( RECT ) );

	hResult = lpDirectDrawInfo->lpClipper->SetClipList( ( LPRGNDATA )&tClipList, 0 );
	if ( hResult != DD_OK )
	{
		return	ShowErrorMessage( "SetClipList Failed!" );
	}

	InitGammaControl();

	return	TRUE;
}

void
CleanupDirectDraw( LPDIRECTDRAWINFO lpDirectDrawInfo )
{
	if ( !lpDirectDrawInfo->bFullscreen && lpDirectDrawInfo->lpClipper )
	{
		lpDirectDrawInfo->lpClipper->SetClipList( NULL, 0 );
		lpDirectDrawInfo->lpClipper->Release( );
		lpDirectDrawInfo->lpClipper = NULL;
	}

	if ( !lpDirectDrawInfo->bFullscreen && ( lpDirectDrawInfo->lpDirectDrawSurfaceBack != NULL ) )
	{
		lpDirectDrawInfo->lpDirectDrawSurfaceBack->Release( );
		lpDirectDrawInfo->lpDirectDrawSurfaceBack = NULL;
	}

	if ( lpDirectDrawInfo->lpDirectDrawSurfacePrimary != NULL )
	{
		lpDirectDrawInfo->lpDirectDrawSurfacePrimary->Release( );
		lpDirectDrawInfo->lpDirectDrawSurfacePrimary = NULL;
	}

	if( lpDirectDrawInfo->lpDirectDraw != NULL )
	{
		lpDirectDrawInfo->lpDirectDraw->Release( );
		lpDirectDrawInfo->lpDirectDraw = NULL;
	}

//	ResetGammaCtrl();
}

LPDIRECTDRAWSURFACE
CreateSurface( LPDIRECTDRAW lpDirectDraw, DWORD dwWidth, DWORD dwHeight )
{
	LPDIRECTDRAWSURFACE		lpSurface;
	DDSURFACEDESC			ddsd;

	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |  DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = dwWidth;
	ddsd.dwHeight = dwHeight;

	if ( lpDirectDraw->CreateSurface( &ddsd, &lpSurface, NULL ) != DD_OK )
	{
		return	NULL;
	}

	return	lpSurface;
}

void
CleanupSurface( LPDIRECTDRAWSURFACE lpSurface )
{
	if ( lpSurface )
	{
		lpSurface->Release( );
		lpSurface = NULL;
	}
}

BOOL
RestoreSurface( LPDIRECTDRAWSURFACE lpSurface )
{
	if ( lpSurface->Restore( ) != DD_OK )
	{
		return	FALSE;
	}

	return	TRUE;
}

BOOL
RestoreAllSurfaces( LPDIRECTDRAWINFO lpDirectDrawInfo )
{
	if ( !RestoreSurface( lpDirectDrawInfo->lpDirectDrawSurfacePrimary ) )
	{
		char	temp[30];
		sprintf(temp, lan->OutputMessage(7,61));//010215 lsw
		MessageBox( g_hwndMain, temp, 0, MB_OK);
		return	FALSE;
	}

	if ( !RestoreSurface( lpDirectDrawInfo->lpDirectDrawSurfaceBack ) )
	{
		char	temp[30];
		sprintf(temp, lan->OutputMessage(7,62));//010215 lsw
		MessageBox( g_hwndMain, temp, 0, MB_OK);
		return	FALSE;
	}				
					
	return	TRUE;	
}					
					
void				
FlipScreen( LPDIRECTDRAWINFO lpDirectDrawInfo )
{					
	HRESULT		hResult;
					
	extern bool chinese_input;
	if ( lpDirectDrawInfo->bFullscreen )
	{				
		if( chinese_input )
		{
			lpDirectDrawInfo->lpDirectDrawSurfacePrimary->SetClipper( lpDirectDrawInfo->lpClipper );

			hResult = lpDirectDrawInfo->lpDirectDrawSurfacePrimary->Blt( &lpDirectDrawInfo->rectPrimarySurface,
				lpDirectDrawInfo->lpDirectDrawSurfaceBack, NULL, DDBLT_WAIT, NULL );
		}
		else
		while( 1 )	
		{			
			hResult = lpDirectDrawInfo->lpDirectDrawSurfacePrimary->Flip( NULL, DDFLIP_WAIT );
			if ( hResult == DD_OK )
			{		
				break;
			}		
			if ( hResult == DDERR_SURFACELOST )
			{		
				hResult = RestoreAllSurfaces(  lpDirectDrawInfo );
				if ( hResult != DD_OK )
				{	
					return;
				}	
			}		
			if ( hResult != DDERR_WASSTILLDRAWING )
			{		
				break;
			}		
		}			
	}				
	else			
	{				
//		lpDirectDrawInfo->lpDirectDrawSurfacePrimary->SetClipper( lpDirectDrawInfo->lpClipper );
			
		hResult = lpDirectDrawInfo->lpDirectDrawSurfacePrimary->Blt( &lpDirectDrawInfo->rectPrimarySurface,
			lpDirectDrawInfo->lpDirectDrawSurfaceBack, NULL, DDBLT_WAIT, NULL );
	}				
}					
					
void				
EraseScreen( LPDIRECTDRAWINFO lpDirectDrawInfo, WORD color )
{					
	DDBLTFX		ddbltfx;
	HRESULT		hResult;
					
	ddbltfx.dwSize = sizeof( ddbltfx );
	ddbltfx.dwFillColor = color;//	RGB16( 0, 0, 255 );

	if( lpDirectDrawInfo == NULL ) return;
					
	while( 1 )		
	{				
		hResult = lpDirectDrawInfo->lpDirectDrawSurfaceBack->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
		if ( hResult == DD_OK )
		{			
			break;	
		}			
		if ( hResult == DDERR_SURFACELOST )
		{			
			hResult = RestoreAllSurfaces( lpDirectDrawInfo );
			if ( hResult != DD_OK )
			{		
				return;
			}		
		}			
		if ( hResult != DDERR_WASSTILLDRAWING )
		{			
			return;	
		}			
	}				
}					
	
///////////////////////////////////////////////////////////////////////////////
//

BOOL
DDLoadBitmap( LPDIRECTDRAW lpDirectDraw, LPSURFACEINFO lpSurfaceInfo, char* lpszFilePath )
{
	HBITMAP		hbm;
	BITMAP		bm;

	hbm = ( HBITMAP )LoadImage( NULL, lpszFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	if ( hbm == NULL )
	{
		return	FALSE;
	}
	GetObject( hbm, sizeof( bm ), &bm );

	//DDSURFACEDESC			ddsd;
	//LPDIRECTDRAWSURFACE	pdds;

	//ZeroMemory( &ddsd, sizeof( ddsd ) );
	//ddsd.dwSize = sizeof( ddsd );
	//ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	//ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	//ddsd.dwWidth = bm.bmWidth;
	//ddsd.dwHeight = bm.bmHeight;
	//if ( lpDirectDraw->CreateSurface( &ddsd, &pdds, NULL ) == DD_OK )
	//{
	//	lpSurfaceInfo->lpSurface = pdds;
	//}
	//else
	//{
	//	lpSurfaceInfo->lpSurface = NULL;
	//}

	lpSurfaceInfo->rect.left = 0;
	lpSurfaceInfo->rect.top = 0;
	lpSurfaceInfo->rect.right = bm.bmWidth;
	lpSurfaceInfo->rect.bottom = bm.bmHeight;

	lpSurfaceInfo->lpSurface = CreateSurface( lpDirectDraw, bm.bmWidth, bm.bmHeight );

	DDCopyBitmap( lpSurfaceInfo->lpSurface, hbm, 0, 0, 0, 0 );
	DeleteObject( hbm );

//	lpSurfaceInfo->dwFillColor = DDColorMatch( lpSurfaceInfo->lpSurface, RGB( 255, 0, 255 ) );
//	DDSetColorKey( lpSurfaceInfo->lpSurface, lpSurfaceInfo->dwFillColor );
	DDSetColorKey( lpSurfaceInfo->lpSurface, RGB( 255, 0, 255 ) );

	return	TRUE;
}

BOOL
DDReLoadBitmap( LPDIRECTDRAWSURFACE lpSurface, char* lpszFilePath )
{
	HBITMAP		hBitmap;

	hBitmap = ( HBITMAP )LoadImage( NULL, lpszFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	if ( hBitmap == NULL )
	{
		return	FALSE;
	}

	DDCopyBitmap( lpSurface, hBitmap, 0, 0, 0, 0 );
	DeleteObject( hBitmap );

	return	TRUE;
}

BOOL
DDCopyBitmap( LPDIRECTDRAWSURFACE lpSurface, HBITMAP hBitmap, int x, int y, int dx, int dy )
{
	HDC				hdcImage;
	HDC				hdc;
	BITMAP			bm;
	DDSURFACEDESC	ddsd;

	if ( hBitmap == NULL || lpSurface == NULL )
	{
		return	FALSE;
	}

	lpSurface->Restore( );

	hdcImage = CreateCompatibleDC( NULL );
	if ( !hdcImage )
	{
		return	FALSE;
	}
	SelectObject( hdcImage, hBitmap );

	GetObject( hBitmap, sizeof( bm ), &bm );
	dx = dx == 0 ? bm.bmWidth : dx;
	dy = dy == 0 ? bm.bmHeight : dy;

	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	lpSurface->GetSurfaceDesc( &ddsd );
	if ( lpSurface->GetDC( &hdc ) == DD_OK )
	{
		StretchBlt( hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY );
		lpSurface->ReleaseDC( hdc );
	}

	DeleteDC( hdcImage );

	return	TRUE;
}

DWORD
DDColorMatch( LPDIRECTDRAWSURFACE lpSurface, COLORREF rgb )
{
	COLORREF		rgbT;
	HDC				hdc;
	DWORD			dw = CLR_INVALID;
	DDSURFACEDESC	ddsd;
	HRESULT			hResult;

	if ( rgb != CLR_INVALID && lpSurface->GetDC( &hdc ) == DD_OK )
	{
		rgbT = GetPixel( hdc, 0, 0 );
		SetPixel( hdc, 0, 0, rgb );
		lpSurface->ReleaseDC( hdc );
	}

	ddsd.dwSize = sizeof( ddsd );
	while ( ( hResult = lpSurface->Lock( NULL, &ddsd, 0, NULL ) ) == DDERR_WASSTILLDRAWING )
	{
		;
	}

	if ( hResult == DD_OK )
	{
		dw = *( DWORD* )ddsd.lpSurface;
		if ( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 )
		{
			dw &= ( 1 << ddsd.ddpfPixelFormat.dwRGBBitCount ) - 1;
		}

		lpSurface->Unlock( NULL );
	}

	if ( rgb != CLR_INVALID && lpSurface->GetDC( &hdc ) == DD_OK )
	{
		SetPixel( hdc, 0, 0, rgbT );
		lpSurface->ReleaseDC( hdc );
	}

	return	dw;
}

HRESULT
DDSetColorKey( LPDIRECTDRAWSURFACE lpSurface, COLORREF rgb )
{
	DDCOLORKEY	ddck;

	ddck.dwColorSpaceLowValue = DDColorMatch( lpSurface, rgb );
	ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;

	return	lpSurface->SetColorKey( DDCKEY_SRCBLT, &ddck );
}

void
StretchBltScreen( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst,
				  LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc,
				  DWORD dwColorFill, int nDirection )
{
	DDBLTFX		ddbltfx;

	ZeroMemory( &ddbltfx, sizeof( ddbltfx ) );
	ddbltfx.dwSize = sizeof( ddbltfx );
	ddbltfx.dwFillColor = dwColorFill;

	lpSurfaceDst->Blt( lpRectDst, lpSurfaceSrc, lpRectSrc, DDBLT_KEYSRC | DDBLT_DDFX | DDBLT_WAIT, &ddbltfx );
}

void
BltGrid( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc )
{
	DDSURFACEDESC	ddsdSrc, ddsdDst;
	register int	x, y;
	WORD			wColorKey;

	ZeroMemory( &ddsdSrc, sizeof( ddsdSrc ) );
	ddsdSrc.dwSize = sizeof( ddsdSrc );
	ddsdSrc.dwFlags = DDSD_PIXELFORMAT;
	if ( FAILED( lpSurfaceSrc->GetSurfaceDesc( &ddsdSrc ) ) )
	{
		return;
	}

	ZeroMemory( &ddsdDst, sizeof( ddsdDst ) );
	ddsdDst.dwSize = sizeof( ddsdDst );
	if ( FAILED( lpSurfaceDst->GetSurfaceDesc( &ddsdDst ) ) )
	{
		return;
	}

	DDPIXELFORMAT	ddpfSrc;
	ZeroMemory( &ddpfSrc, sizeof( ddpfSrc ) );
	ddpfSrc.dwSize = sizeof( ddpfSrc );
	lpSurfaceSrc->GetPixelFormat( &ddpfSrc );
	switch ( ddpfSrc.dwRBitMask )
	{
	case	0x7C00:		// RGB 5:5:5
		wColorKey  = ( 255 >> 3 );			// Blue
		wColorKey += (   0 >> 3 ) << 5;		// Green
		wColorKey += ( 255 >> 3 ) << 10;	// Red
		break;

	case	0xF800:		// RGB 5:6:5
		wColorKey  = ( 255 >> 3 );			// Blue
		wColorKey += (   0 >> 2 ) << 5;		// Green
		wColorKey += ( 255 >> 3 ) << 11;	// Red
		break;
	}

	if ( FAILED( lpSurfaceSrc->Lock( NULL, &ddsdSrc, DDLOCK_WAIT, NULL ) ) )
	{
		return;
	}

	if ( FAILED( lpSurfaceDst->Lock( NULL, &ddsdDst, DDLOCK_WAIT, NULL ) ) )
	{
		lpSurfaceSrc->Unlock( NULL );
		return;
	}

	LPWORD	lpwSrc = ( LPWORD )ddsdSrc.lpSurface + lpRectSrc->top * ( ddsdSrc.lPitch / sizeof( WORD ) ) + lpRectSrc->left;
	LPWORD	lpwDst = ( LPWORD )ddsdDst.lpSurface + lpRectDst->top * ( ddsdDst.lPitch / sizeof( WORD ) ) + lpRectDst->left;
	int		cx = lpRectDst->right - lpRectDst->left;
	int		cy = lpRectDst->bottom - lpRectDst->top;

	for ( y = 0; y < cy; y++ )
	{
		for ( x = 0; x < cx; x++ )
		{
			if ( y % 2 == 0 )
			{
				if ( x % 2 == 0 )
				{
					if ( *( lpwSrc + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x ) != wColorKey )
					{
						*( lpwDst + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x ) = *( lpwSrc + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x );
					}
				}
			}
			else
			{
				if ( x % 2 == 1 )
				{
					if ( *( lpwSrc + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x ) != wColorKey )
					{
						*( lpwDst + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x ) = *( lpwSrc + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x );
					}
				}
			}
		}
	}
	
	lpSurfaceSrc->Unlock( NULL );
	lpSurfaceDst->Unlock( NULL );
}

void
BltTrans( LPDIRECTDRAWSURFACE lpSurfaceDst, LPRECT lpRectDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LPRECT lpRectSrc )
{
	DDSURFACEDESC	ddsdSrc, ddsdDst;
	register int	x, y;
	WORD			wColorKey;
	WORD			wSrcRed, wSrcGreen, wSrcBlue;
	WORD			wDstRed, wDstGreen, wDstBlue;
	WORD			wImageSrc, wImageDst;

	ZeroMemory( &ddsdSrc, sizeof( ddsdSrc ) );
	ddsdSrc.dwSize = sizeof( ddsdSrc );
	ddsdSrc.dwFlags = DDSD_PIXELFORMAT;
	if ( FAILED( lpSurfaceSrc->GetSurfaceDesc( &ddsdSrc ) ) )
	{
		return;
	}

	ZeroMemory( &ddsdDst, sizeof( ddsdDst ) );
	ddsdDst.dwSize = sizeof( ddsdDst );
	if ( FAILED( lpSurfaceDst->GetSurfaceDesc( &ddsdDst ) ) )
	{
		return;
	}

	DDPIXELFORMAT	ddpfSrc;
	ZeroMemory( &ddpfSrc, sizeof( ddpfSrc ) );
	ddpfSrc.dwSize = sizeof( ddpfSrc );
	lpSurfaceSrc->GetPixelFormat( &ddpfSrc );
	switch ( ddpfSrc.dwRBitMask )
	{
	case	0x7C00:		// RGB 5:5:5
		wColorKey  = ( 255 >> 3 );			// Blue
		wColorKey += (   0 >> 3 ) << 5;		// Green
		wColorKey += ( 255 >> 3 ) << 10;	// Red
		break;

	case	0xF800:		// RGB 5:6:5
		wColorKey  = ( 255 >> 3 );			// Blue
		wColorKey += (   0 >> 2 ) << 5;		// Green
		wColorKey += ( 255 >> 3 ) << 11;	// Red
		break;
	}

	if ( FAILED( lpSurfaceSrc->Lock( NULL, &ddsdSrc, DDLOCK_WAIT, NULL ) ) )
	{
		return;
	}

	if ( FAILED( lpSurfaceDst->Lock( NULL, &ddsdDst, DDLOCK_WAIT, NULL ) ) )
	{
		lpSurfaceSrc->Unlock( NULL );
		return;
	}

	LPWORD	lpwSrc = ( LPWORD )ddsdSrc.lpSurface + lpRectSrc->top * ( ddsdSrc.lPitch / sizeof( WORD ) ) + lpRectSrc->left;
	LPWORD	lpwDst = ( LPWORD )ddsdDst.lpSurface + lpRectDst->top * ( ddsdDst.lPitch / sizeof( WORD ) ) + lpRectDst->left;
	int		cx = lpRectDst->right - lpRectDst->left;
	int		cy = lpRectDst->bottom - lpRectDst->top;

	LPWORD	lpwImageSrc, lpwImageDst;

	for ( y = 0; y < cy; y++ )
	{
		lpwImageSrc = lpwSrc + y * ( ddsdSrc.lPitch / sizeof( WORD ) );
		lpwImageDst = lpwDst + y * ( ddsdSrc.lPitch / sizeof( WORD ) );

		for ( x = 0; x < cx; x++ )
		{
			//wSrc = *( lpwSrc + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x );
			//lpwImageSrc = ( LPWORD )*( lpwSrc + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x );

			if ( *lpwImageSrc != wColorKey )
			{
				wImageSrc = *lpwImageSrc;
				wImageDst = *lpwImageDst;

				//wDst = *( lpwDst + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x );
				//lpwImageDst = ( LPWORD )*( lpwDst + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x );

				//wSrcRed   = ( ( wSrc )       ) & ( 0x1F );
				//wSrcGreen = ( ( wSrc ) >> 5  ) & ( 0x1F );
				//wSrcBlue  = ( ( wSrc ) >> 10 ) & ( 0x1F );
				//wSrc = ( ( ( wSrcBlue * 11 ) + ( wSrcGreen * 59 ) + ( wSrcRed * 30 ) ) / 100 );
				//wSrc = ( ( WORD )( ( ( BYTE )( wSrc ) | ( ( WORD )( ( BYTE )( wSrc ) ) << 5 ) ) | ( ( ( DWORD )( BYTE )( wSrc ) ) << 10 ) ) );

				//wDstRed   = ( ( wDst )       ) & ( 0x1F );
				//wDstGreen = ( ( wDst ) >> 5  ) & ( 0x1F );
				//wDstBlue  = ( ( wDst ) >> 10 ) & ( 0x1F );
				//wDst = ( ( ( wDstBlue * 11 ) + ( wDstGreen * 59 ) + ( wDstRed * 30 ) ) / 100 );
				//wDst = ( ( WORD )( ( ( BYTE )( wDst ) | ( ( WORD )( ( BYTE )( wDst ) ) << 5 ) ) | ( ( ( DWORD )( BYTE )( wDst ) ) << 10 ) ) );

				wSrcBlue  = ( wImageSrc        ) & ( 0x001f );
				wSrcGreen = ( wImageSrc >>  5  ) & ( 0x001f );
				wSrcRed   = ( wImageSrc >> 10  ) & ( 0x001f );

				wDstBlue  = ( wImageDst        ) & ( 0x001f );
				wDstGreen = ( wImageDst >>  5  ) & ( 0x001f );
				wDstRed   = ( wImageDst >> 10  ) & ( 0x001f );

				//wPixel = ( ( ( wBlue * 11 ) + ( wGreen * 59 ) + ( wRed * 30 ) ) / 100 );
				//wPixel = ( ( WORD )( ( ( BYTE )( Pixel ) | ( ( WORD )( ( BYTE )( wPixel ) ) << 5 ) ) | ( ( ( DWORD )( BYTE )( wPixel ) ) << 10 ) ) );
//				wPixel = ( ( WORD )( ( ( BYTE )( wRed ) | ( ( WORD )( ( BYTE )( wGreen ) ) << 5 ) ) | ( ( ( DWORD )( BYTE )( wBlue ) ) << 10 ) ) );
//				wPixel = ( ( wBlue >> 3 ) | ( ( wGreen >> 3 ) << 5 ) | ( ( wRed >> 3 ) << 10 ) );

//				*( lpwDst + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x ) = ( ( wBlue ) | ( ( wGreen ) << 5 ) | ( ( wRed ) << 10 ) );
//				*( lpwDst + y * ( ddsdSrc.lPitch / sizeof( WORD ) ) + x ) = ( ( wBlue ) | ( ( wGreen ) << 5 ) | ( ( wRed ) << 10 ) );

				//*lpwImageDst = ( ( wSrcBlue ) | ( ( wSrcGreen ) << 5 ) | ( ( wSrcRed ) << 10 ) );
				*lpwImageDst = ( ( wSrcBlue >> 3 ) | ( ( wSrcGreen >> 3 ) << 5 ) | ( ( wSrcRed >> 3 ) << 10 ) );
			}

			lpwImageSrc++, lpwImageDst++;
		}
	}
	
	lpSurfaceSrc->Unlock( NULL );
	lpSurfaceDst->Unlock( NULL );
}

int
TransAlpha( LPDIRECTDRAWSURFACE lpSurfaceDst, LPDIRECTDRAWSURFACE lpSurfaceSrc, LONG x, LONG y, RECT rectSrc, WORD wAlphaValue )
{
	DDSURFACEDESC	ddsdSrc, ddsdDst;
	register int	i, j;
	int				height, width;
	LPBYTE			lpSrc, lpDst;
	WORD			wSrcPitch, wDstPitch;
	DWORD			dwSrcTemp, dwDstTemp;
	DWORD			dwSrcRed, dwSrcGreen, dwSrcBlue, dwDstRed, dwDstGreen, dwDstBlue;
	WORD			wSrcPadding, wDstPadding;
	DWORD			dwResult;
	BOOL			bOdd = FALSE;
	DWORD			dwColorKey;
	
	if ( wAlphaValue < 0 )
	{
		wAlphaValue = 0;
	}
	else if ( wAlphaValue > 256 )
	{
		wAlphaValue = 256;
	}

	height = rectSrc.bottom - rectSrc.top;
	width = rectSrc.right - rectSrc.left;

	ZeroMemory( &ddsdSrc, sizeof( ddsdSrc ) );
	ddsdSrc.dwSize = sizeof( ddsdSrc );
	ddsdSrc.dwFlags = DDSD_PIXELFORMAT;
	if ( FAILED( lpSurfaceSrc->GetSurfaceDesc( &ddsdSrc ) ) )
	{
		return	-1;
	}

	ZeroMemory( &ddsdDst, sizeof( ddsdDst ) );
	ddsdDst.dwSize = sizeof( ddsdDst );
	if ( FAILED( lpSurfaceDst->GetSurfaceDesc( &ddsdDst ) ) )
	{
		return	-1;
	}

	DDPIXELFORMAT	ddpfSrc;
	ZeroMemory( &ddpfSrc, sizeof( ddpfSrc ) );
	ddpfSrc.dwSize = sizeof( ddpfSrc );
	lpSurfaceSrc->GetPixelFormat( &ddpfSrc );
	switch ( ddpfSrc.dwRBitMask )
	{
	case	0x7C00:		// RGB 5:5:5
		dwColorKey  = ( 255 >> 3 );				// Blue
		dwColorKey += (   0 >> 3 ) << 5;		// Green
		dwColorKey += ( 255 >> 3 ) << 10;		// Red
		break;

	case	0xF800:		// RGB 5:6:5
		dwColorKey  = ( 255 >> 3 );				// Blue
		dwColorKey += (   0 >> 2 ) << 5;		// Green
		dwColorKey += ( 255 >> 3 ) << 11;		// Red
		break;
	}

	if ( FAILED( lpSurfaceSrc->Lock( NULL, &ddsdSrc, DDLOCK_WAIT, NULL ) ) )
	{
		return	-1;
	}

	if ( FAILED( lpSurfaceDst->Lock( NULL, &ddsdDst, DDLOCK_WAIT, NULL ) ) )
	{
		lpSurfaceSrc->Unlock( NULL );
		return	-1;
	}

	wSrcPitch = ( WORD )ddsdSrc.lPitch;
	wDstPitch = ( WORD )ddsdDst.lPitch;

	lpSrc = ( LPBYTE )ddsdSrc.lpSurface;
	lpDst = ( LPBYTE )ddsdDst.lpSurface;

	switch ( ddpfSrc.dwRBitMask )
	{
	case	0x7C00:
		lpSrc += ( rectSrc.top * wSrcPitch ) + ( rectSrc.left * 2 );
		lpDst += ( y * wDstPitch) + ( x * 2 );

		wSrcPadding = ( WORD )( wSrcPitch - ( width * 2 ) );
		wDstPadding = ( WORD )( wDstPitch - ( width * 2 ) );

		if ( width % 2 == 1 )
		{
			bOdd = TRUE;
			width = ( width - 1 ) / 2;
		}
		else
		{
			width = width / 2;
		}

		i = height;
		do
		{
			if ( bOdd )
			{
				dwSrcTemp = *( ( LPWORD )lpSrc );

				if ( dwSrcTemp != dwColorKey )
				{
					dwDstTemp = *( ( LPWORD )lpDst );

					dwSrcBlue  = dwSrcTemp & 0x1f;
					dwDstBlue  = dwDstTemp & 0x1f;
					dwSrcGreen = ( dwSrcTemp >> 5 ) & 0x1f;
					dwDstGreen = ( dwDstTemp >> 5 ) & 0x1f;
					dwSrcRed   = ( dwSrcTemp >> 10 ) & 0x1f;
					dwDstRed   = ( dwDstTemp >> 10 ) & 0x1f;

					*( ( LPWORD )lpDst ) = ( WORD )( ( wAlphaValue * ( dwSrcBlue - dwDstBlue ) >> 8 ) + dwDstBlue	|
										   ( ( wAlphaValue * ( dwSrcGreen - dwDstGreen ) >> 8 ) + dwDstGreen ) << 5	|
										   ( ( wAlphaValue * ( dwSrcRed - dwDstRed ) >> 8 ) + dwDstRed ) << 10 );
				}

				lpSrc += 2, lpDst += 2;
			}

			j = width;
			do
			{
				dwSrcTemp = *( ( LPDWORD )lpSrc );

				if ( dwSrcTemp != ( ( DWORD )dwColorKey | ( ( DWORD )dwColorKey << 16 ) ) )
				{
					dwDstTemp = *( ( LPDWORD )lpDst );
					dwResult = dwDstTemp;

					if ( ( dwSrcTemp & 0xffff ) != dwColorKey )
					{
						dwSrcBlue  = dwSrcTemp & 0x1f;
						dwDstBlue  = dwDstTemp & 0x1f;
						dwSrcGreen = ( dwSrcTemp >> 5 ) & 0x1f;
						dwDstGreen = ( dwDstTemp >> 5 ) & 0x1f;
						dwSrcRed   = ( dwSrcTemp >> 10 ) & 0x1f;
						dwDstRed   = ( dwDstTemp >> 10 ) & 0x1f;

						dwResult = dwResult & 0xffff0000;

						dwResult |= ( DWORD )( ( wAlphaValue * ( dwSrcBlue - dwDstBlue ) >> 8 ) + dwDstBlue		|
									( ( wAlphaValue * ( dwSrcGreen - dwDstGreen ) >> 8 ) + dwDstGreen ) << 5	|
									( ( wAlphaValue * ( dwSrcRed - dwDstRed ) >> 8 ) + dwDstRed ) << 10 );
					}

					if ( ( ( dwSrcTemp >> 16 ) & 0xffff ) != dwColorKey )
					{
						dwSrcBlue  = ( dwSrcTemp >> 16 ) & 0x1f;
						dwDstBlue  = ( dwDstTemp >> 16 ) & 0x1f;
						dwSrcGreen = ( dwSrcTemp >> 21 ) & 0x1f;
						dwDstGreen = ( dwDstTemp >> 21 ) & 0x1f;
						dwSrcRed   = ( dwSrcTemp >> 26 ) & 0x1f;
						dwDstRed   = ( dwDstTemp >> 26 ) & 0x1f;

						dwResult = dwResult & 0xffff;

						dwResult |= ( DWORD )( ( wAlphaValue * ( dwSrcBlue - dwDstBlue ) >> 8 ) + dwDstBlue		|
									( ( wAlphaValue * ( dwSrcGreen - dwDstGreen ) >> 8 ) + dwDstGreen ) << 5	|
									( ( wAlphaValue * ( dwSrcRed - dwDstRed ) >> 8 ) + dwDstRed ) << 10 ) << 16;
					}

					*( ( LPDWORD )lpDst ) = dwResult;
				}

				lpSrc += 4, lpDst += 4;
			} while ( --j > 0 );

			lpSrc += wSrcPadding, lpDst += wDstPadding;

		} while ( --i > 0 );
		break;

	case	0xF800:
		lpSrc += ( rectSrc.top * wSrcPitch ) + ( rectSrc.left * 2 );
		lpDst += ( y * wDstPitch) + ( x * 2 );

		wSrcPadding = ( WORD )( wSrcPitch - ( width * 2 ) );
		wDstPadding = ( WORD )( wDstPitch - ( width * 2 ) );

		if ( width % 2 == 1 )
		{
			bOdd = TRUE;
			width = ( width - 1 ) / 2;
		}
		else
		{
			width = width / 2;
		}

		i = height;
		do
		{
			if ( bOdd )
			{
				dwSrcTemp = *( ( LPWORD )lpSrc );

				if ( dwSrcTemp != dwColorKey )
				{
					dwDstTemp = *( ( LPWORD )lpDst );

					dwSrcBlue  = dwSrcTemp & 0x1f;
					dwDstBlue  = dwDstTemp & 0x1f;
					dwSrcGreen = ( dwSrcTemp >> 5 ) & 0x3f;
					dwDstGreen = ( dwDstTemp >> 5 ) & 0x3f;
					dwSrcRed   = ( dwSrcTemp >> 11 ) & 0x1f;
					dwDstRed   = ( dwDstTemp >> 11 ) & 0x1f;

					*( ( LPWORD )lpDst ) = ( WORD )( ( wAlphaValue * ( dwSrcBlue - dwDstBlue ) >> 8 ) + dwDstBlue	|
										   ( ( wAlphaValue * ( dwSrcGreen - dwDstGreen ) >> 8 ) + dwDstGreen ) << 5	|
										   ( ( wAlphaValue * ( dwSrcRed - dwDstRed ) >> 8 ) + dwDstRed ) << 11 );
				}

				lpSrc += 2, lpDst += 2;
			}

			j = width;
			do
			{
				dwSrcTemp = *( ( LPDWORD )lpSrc );

				if ( dwSrcTemp != ( ( DWORD )dwColorKey | ( ( DWORD )dwColorKey << 16 ) ) )
				{
					dwDstTemp = *( ( LPDWORD )lpDst );
					dwResult = dwDstTemp;

					if ( ( dwSrcTemp & 0xffff ) != dwColorKey )
					{
						dwSrcBlue  = dwSrcTemp & 0x1f;
						dwDstBlue  = dwDstTemp & 0x1f;
						dwSrcGreen = ( dwSrcTemp >> 5 ) & 0x3f;
						dwDstGreen = ( dwDstTemp >> 5 ) & 0x3f;
						dwSrcRed   = ( dwSrcTemp >> 11 ) & 0x1f;
						dwDstRed   = ( dwDstTemp >> 11 ) & 0x1f;

						dwResult = dwResult & 0xffff0000;

						dwResult |= ( DWORD )( ( wAlphaValue * ( dwSrcBlue - dwDstBlue ) >> 8 ) + dwDstBlue		|
									( ( wAlphaValue * ( dwSrcGreen - dwDstGreen ) >> 8 ) + dwDstGreen ) << 5	|
									( ( wAlphaValue * ( dwSrcRed - dwDstRed ) >> 8 ) + dwDstRed ) << 11 );
					}

					if ( ( ( dwSrcTemp >> 16 ) & 0xffff ) != dwColorKey )
					{
						dwSrcBlue  = ( dwSrcTemp >> 16 ) & 0x1f;
						dwDstBlue  = ( dwDstTemp >> 16 ) & 0x1f;
						dwSrcGreen = ( dwSrcTemp >> 21 ) & 0x3f;
						dwDstGreen = ( dwDstTemp >> 21 ) & 0x3f;
						dwSrcRed   = ( dwSrcTemp >> 27 ) & 0x1f;
						dwDstRed   = ( dwDstTemp >> 27 ) & 0x1f;

						dwResult = dwResult & 0xffff;

						dwResult |= ( DWORD )( ( wAlphaValue * ( dwSrcBlue - dwDstBlue ) >> 8 ) + dwDstBlue		|
									( ( wAlphaValue * ( dwSrcGreen - dwDstGreen ) >> 8 ) + dwDstGreen ) << 5	|
									( ( wAlphaValue * ( dwSrcRed - dwDstRed ) >> 8 ) + dwDstRed ) << 11 ) << 16;
					}

					*( ( LPDWORD )lpDst ) = dwResult;
				}

				lpSrc += 4, lpDst += 4;
			} while ( --j > 0 );

			lpSrc += wSrcPadding, lpDst += wDstPadding;
		} while ( --i > 0 );
		break;
	}

	lpSurfaceSrc->Unlock( NULL );
	lpSurfaceDst->Unlock( NULL );

	return	0;
}


//--------------------------- Gamma Control------------------------
/*******************************************************************

HRESULT GetColorControls(
  LPDDCOLORCONTROL lpColorControl  
);


typedef struct _DDCOLORCONTROL {
    DWORD   dwSize;
				-> sizeof( DDCOLORCONTROL );
    DWORD   dwFlags;
				->	DDCOLOR_BRIGHTNESS 
						The lBrightness member contains valid data. 
					DDCOLOR_COLORENABLE 
						The lColorEnable member contains valid data. 
					DDCOLOR_CONTRAST 
						The lContrast member contains valid data. 
					DDCOLOR_GAMMA 
						The lGamma member contains valid data. 
					DDCOLOR_HUE 
						The lHue member contains valid data. 
					DDCOLOR_SATURATION 
						The lSaturation member contains valid data. 
						DDCOLOR_SHARPNESS 
					The lSharpness member contains valid data. 

    LONG    lBrightness;
    LONG    lContrast;
    LONG    lHue;
    LONG    lSaturation;
    LONG    lSharpness;
    LONG    lGamma;
    LONG    lColorEnable;
    DWORD   dwReserved1;
} DDCOLORCONTROL, FAR *LPDDCOLORCONTROL;
 


  

*******************************************************************/


DDCOLORCONTROL g_DisplayGamma, g_OldDisplayGamma;

// 게임전의 값을 읽어 저장한다. 
void InitGammaControl( void )
{
	g_DisplayGamma.dwSize  = sizeof( DDCOLORCONTROL );
	g_DisplayGamma.dwFlags = DDCOLOR_BRIGHTNESS;

//	GetColorControls( &g_DisplayGamma );
	g_OldDisplayGamma = g_DisplayGamma;
}


// 게임전의 값을 다시 Setting한다. 
void ResetGammaCtrl( void )
{
//	g_DirectDrawInfo.lpColorCtrl-> lpColorControl->SetColorControls( &g_OldDisplayGamma );  
}

// 0 - 10000
int ColorCtrlBrightness( long v )
{
	g_DisplayGamma.dwSize  = sizeof( DDCOLORCONTROL );
	g_DisplayGamma.dwFlags = DDCOLOR_BRIGHTNESS;
	g_DisplayGamma.lBrightness = v;
//	SetColorControls( &g_DisplayGamma );
	return v;
}
















