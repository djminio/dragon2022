/*

*/

#ifndef	__DIRECTINPUT_H__
#define	__DIRECTINPUT_H__

#define DIRECTINPUT_VERSION 0x300
#include "dinput.h"

///////////////////////////////////////////////////////////////////////////////
// Global variables

extern BYTE		g_aCurrentKeys[ 256 ];
extern BYTE		g_aPrevKeys[ 256 ];

///////////////////////////////////////////////////////////////////////////////
// Function prototypes

#ifdef __cplusplus
extern "C"
{
#endif

extern HRESULT		InitDirectInput( HWND hWnd, HINSTANCE hInstance, BOOL bActive );
extern HRESULT		SetAcquire( BOOL bFlag );
extern HRESULT		UpdateInputState( void );
extern HRESULT		CleanupDirectInput( void );

#ifdef __cplusplus
}
#endif

#endif	// __DIRECTINPUT_H__
