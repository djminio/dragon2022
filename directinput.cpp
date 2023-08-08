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
#include "dragon.h"
#include "directinput.h"
#include "Stdio.h"


#include "Tool.h"
#include "../directxlib/Include/dinput.h"








///////////////////////////////////////////////////////////////////////////////
// Global variables

IDirectInput*			g_lpDirectInput = NULL;
IDirectInputDevice*		g_lpKeyboard = NULL;

BYTE					g_aCurrentKeys[ 256 ];
BYTE					g_aPrevKeys[ 256 ];

///////////////////////////////////////////////////////////////////////////////
// Function prototypes

HRESULT		InitDirectInput( HWND hWnd, HINSTANCE hInstance, BOOL bActive );
HRESULT		SetAcquire( BOOL bFlag );
HRESULT		UpdateInputState( void );
HRESULT		CleanupDirectInput( void );



///////////////////////////////////////////////////////////////////////////////
// Functions

HRESULT
InitDirectInput( HWND hWnd, HINSTANCE hInstance, BOOL bActive )
{	
	HRESULT	hResult;
	
	hResult = DirectInputCreate( hInstance, 0x0300, &g_lpDirectInput, NULL );
	if ( hResult != DI_OK )
	{
		return	hResult;
	}
	
//	keyboard
//	rval = lpDID->QueryInterface(IID_IDirectInputDevice2, (LPVOID*)&m_Device);

	hResult = g_lpDirectInput->CreateDevice( GUID_SysKeyboard, &g_lpKeyboard, NULL );
	if ( hResult != DI_OK )
	{
		return	hResult;
	}
	
	hResult = g_lpKeyboard->SetDataFormat( &c_dfDIKeyboard );
	if ( hResult != DI_OK )
	{
		return	hResult;
	}
	
	hResult = g_lpKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if ( hResult != DI_OK )
	{
		return	hResult;
	}
	
	
	SetAcquire( bActive );
	
	return	DI_OK;
}	
	
HRESULT
SetAcquire( BOOL bFlag )
{	
	if ( g_lpKeyboard == NULL )
	{
		return	S_FALSE;
	}
	
	if ( bFlag )
	{
		g_lpKeyboard->Acquire( );
	} 
	else 
	{
		g_lpKeyboard->Unacquire( );
	}
	
	return	DI_OK;
}	
	
HRESULT
UpdateInputState( void )
{		
	HRESULT		hResult;
			
	// keyboard
	hResult = DIERR_INPUTLOST;
	if ( g_lpKeyboard != NULL )
	{			
		while ( hResult == DIERR_INPUTLOST )
		{							
			hResult = g_lpKeyboard->GetDeviceState( sizeof( g_aCurrentKeys ), g_aCurrentKeys );
			if ( hResult == DIERR_INPUTLOST )
			{				
				hResult = g_lpKeyboard->Acquire( );
				if ( hResult != DI_OK )
				{			
					return	hResult;
				}
			}
		}	
			
		if ( hResult != DI_OK )
		{	
			return	hResult;
		}
	}	

	return	DI_OK;
}		
		
HRESULT	
CleanupDirectInput( void )
{		
	if ( g_lpKeyboard != NULL )
	{	
		g_lpKeyboard->Unacquire( );
		g_lpKeyboard->Release( );
		g_lpKeyboard = NULL;
	}	
		
	if ( g_lpDirectInput != NULL )
	{	
		g_lpDirectInput->Release( );
		g_lpDirectInput = NULL;
	}	
			
	return	DI_OK;
}		
		

















