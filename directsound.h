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

#ifndef	__DIRECTSOUND_H__
#define	__DIRECTSOUND_H__

#include "dsound.h"

#define WAVEVERSION 1

#ifndef ER_MEM
#define ER_MEM 				0xe000
#endif

#ifndef ER_CANNOTOPEN
#define ER_CANNOTOPEN 		0xe100
#endif

#ifndef ER_NOTWAVEFILE
#define ER_NOTWAVEFILE 		0xe101
#endif

#ifndef ER_CANNOTREAD
#define ER_CANNOTREAD 		0xe102
#endif

#ifndef ER_CORRUPTWAVEFILE
#define ER_CORRUPTWAVEFILE	0xe103
#endif

#ifndef ER_CANNOTWRITE
#define ER_CANNOTWRITE		0xe104
#endif

// CONVERSION MACROS:
//    These would be faster if hard-coded, but are left like this for readability.
//////////////////////// SoundUp lkh 수정 /////////////////////////
//#define SCREEN_METERS	D3DVAL( 12 )						//(How many meters wide is the screen?)
#define SCREEN_METERS	D3DVAL( 16 )						//(How many meters wide is the screen?)
#define PPM				D3DVAL( 600.f / SCREEN_METERS )		//How many pixels per meter?
#define MPP				D3DVAL( SCREEN_METERS / 600.f )		//How many meters per pixel?
#define P2M( x )		D3DVAL( D3DVAL( x ) * MPP )			//converts pixels to meters
#define M2P( x )		D3DVAL( D3DVAL( x ) * PPM )			//converts meters to pixels

//#define	MAX_FIXEDSOUNDS		100
//#define	MAX_AUTOSOUNDS		2400
#define MAX_SOUND			2500

#define	SOUND_RAINY			480
#define	SOUND_LIGHTNING1	481
#define	SOUND_LIGHTNING2	482

typedef struct
{
	LPDIRECTSOUNDBUFFER		lpDirectSoundBuffer;
	LPTSTR					lpName;
	LPDIRECTSOUND			lpDS;
} WAVEDATA, *LPWAVEDATA;

typedef struct	tagSOUNDINFO
{
	LPDIRECTSOUND				lpDirectSound;
	LPDIRECTSOUND3DLISTENER		lpDirectSound3DListener;
	LPDIRECTSOUNDBUFFER			lpPrimaryBuffer;

	LPDIRECTSOUNDBUFFER			lpDirectSoundBuffer[ MAX_SOUND ];
	LPDIRECTSOUND3DBUFFER		lpDirectSound3DBuffer[ MAX_SOUND ];
	LPWAVEDATA					lpWaveData[ MAX_SOUND ];

	BOOL						bInitialized;
} SOUNDINFO, *LPSOUNDINFO;


typedef struct tagSOUNDLIST
{
	int			list_Index;
	BOOL		static_Sound;
	char		sound_FileName[10];		//0000.wav
} SOUNDLIST;


#ifdef __cplusplus
extern "C"
{
#endif

extern SOUNDINFO	g_SoundInfo;
extern SOUNDLIST*	g_SoundList;

extern BOOL		PlayListAutoSounds( int index, int x, int y, int flag);
extern BOOL		AdjustAutoSounds( int index, int volume, bool loop=0);
extern BOOL		AdjustStaticSounds( int index, int volume, bool loop=0);
////////////// 0618 lkh 추가 //////////////
extern void		AdjustSounds( int index, int volume, bool loop=0);

extern BOOL		LoadSoundList( void );
extern BOOL		InitSfx( HWND hWnd );
extern void		FreeSfx( void );
extern BOOL		InitGlobals( HWND hWnd );
extern void		FreeGlobals( void );
extern BOOL		InitFixedSounds( void );
extern void		FreeFixedSounds( void );
extern void		FreeAutoSounds( int index );

extern void		SetListener( int x, int y );
extern void		PlayWave( int index, int x, int y, int flag );
extern void		StopWave( int index );
extern void		MoveWave( int index, int x, int y );
extern void		SetVolumeOne( int index, int volume );
extern void		SetVolume( int volume );
extern void		SetVolumeMusic( int volume );
extern BOOL		IsPlaying( int index );
extern void		WaveFree( LPWAVEDATA lpWaveData );
extern void		CheckAutoWave( void );
extern void		VolumeTranslate();
extern void		VolumeSet(short int change_Volume, short int to_Volume);
extern void		WeatherFXSound(void);
extern void		BGMFadeInOut(void);
extern void		StartSound( int type );		// 초기 사운드 멈춤		//021030 YGI

#ifdef __cplusplus
}
#endif

#endif	// __DIRECTSOUND_H__
