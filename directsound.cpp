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
#include "stdio.h"
#include "dragon.h"
#include "directsound.h"
#include "../directxlib/Include/d3drm.h"
#include "gameproc.h"
//////////////////////// SoundUp lkh ���� //////////////////////////
#include "menu.h"
#include "Music.h"
#include "Tool.h"
#include "Hong_Sub.h"

#define MemAlloc(x, y)				__Alloc( (char **)(&##x), (y), __FILE__, __LINE__ )
#define CHECKSUM_ERROR_SOUNDLIST	20009

#define WAVE_ENCODING_   10

extern void __Alloc( char **s, int size, const char *file, int line );

///////////////////////////////////////////////////////////////////////////////
// global variables

SOUNDINFO				g_SoundInfo;
SOUNDLIST*				g_SoundList;
D3DVECTOR				d3dvPos;

extern int	RainAmount;
extern BOOL	Statue_Weather;
extern int	MapNumber;
int			BGM_SoundVolume=0;
BOOL		BGM_FadeInOut=0;

///////////////////////////////////////////////////////////////////////////////
// function prototypes

BOOL	WaveInit( LPWAVEDATA* lplpWaveData, LPDIRECTSOUND lpDirectSound, LPSTR lpName );	//ȭ�Ϸ� ���� Wav ȭ�� �о�鿩 ���ۿ� ����
BOOL	WaveGetBuffers( LPWAVEDATA lpWaveData, LPDIRECTSOUNDBUFFER* lplpDirectSoundBuffer, LPDIRECTSOUND3DBUFFER* lplpDirectSound3DBuffer );	//���̷�Ʈ ���� ���ۿ��� �ش� wav�� ������
void	WaveFree( LPWAVEDATA lpWaveData );		//wav ���������� �� �̸� / ���� ������ ������
void	WaveReleaseSoundBuffer( LPDIRECTSOUNDBUFFER* lplpDirectSoundBuffer, LPDIRECTSOUND3DBUFFER* lplpDirectSound3DBuffer );	//���� ���� ������ ������(**DirectSoundBuffer/**DirectSoundBuffer3DBuffer)
BOOL	WaveReload( LPWAVEDATA lpWaveData );	//�������� ���� wave ȭ�� ��ε�
BOOL	DSFillSoundBuffer( IDirectSoundBuffer* pDSB, BYTE* pbWaveData, DWORD cbWaveSize );		//���� ���ۿ� ���̺� ������ ä���
BOOL	DSReloadSoundBuffer( IDirectSoundBuffer* pDSB, LPCTSTR lpName );						//���� wave ȭ�� ��ε�
int		WaveOpenFile( char* pszFileName, HMMIO* phmmioIn, WAVEFORMATEX** ppwfxInfo, MMCKINFO* pckInRIFF, int *encoding );
int		WaveStartDataRead( HMMIO* phmmioIn, MMCKINFO* pckIn, MMCKINFO* pckInRIFF );
int		WaveReadFile( HMMIO hmmioIn, UINT cbRead, BYTE *pbDest, MMCKINFO *pckIn, UINT *cbActualRead, int type	);
int		WaveCloseReadFile( HMMIO *phmmio, WAVEFORMATEX **ppwfxSrc );
int		WaveLoadFile( char* pszFileName, UINT* cbSize, WAVEFORMATEX** ppwfxInfo, BYTE** ppbData );

BOOL	InitSfx( HWND hWnd );
void	FreeSfx( void );
BOOL	InitGlobals( HWND hWnd );
void	FreeGlobals( void );		//1������/3D ����/���̷�Ʈ ���� ��ü ������
BOOL	InitFixedSounds( void );	//�����ε� wave ȭ���� ��Ÿƽ ���� ����/����
void	FreeFixedSounds( void );
BOOL	InitAutoSounds( int index );
void	FreeAutoSounds( int index );

void	SetListener( int x, int y );
void	PlayWave( int index, int x, int y, int flag );
void	StopWave( int index );
void	MoveWave( int index, int x, int y );
void	SetVolume( int volume );
void	SetVolumeMusic( int volume );
BOOL	IsPlaying( int index );

///////////////////////////////////////////////////////////////////////////////
// functions
BOOL	LoadSoundList( void )
{
	FILE*	file;
	
	////////////// 0330 lkh ���� //////////////////
	g_SoundList=NULL;
	MemAlloc( g_SoundList, sizeof(SOUNDLIST)*MAX_SOUND );

	file = Fopen( "./SOUND/SOUNDLIST.DAT", "rt" );				//read only+text file
	
	if(file)
	{
		for(int i=0; i<MAX_SOUND; i++)		//0-1999
		{
			fscanf( file, "%d", &g_SoundList[i].list_Index );
			fscanf( file, "%d", &g_SoundList[i].static_Sound );
			fscanf( file, "%s\n", &g_SoundList[i].sound_FileName);
		}
		fclose( file );

		file=Fopen( "./SOUND/SOUNDLIST.BIN", "wb" );
	
		if(file)
		{
			char *tt = (char *)g_SoundList;			//Ư�� ȭ�������� 1����Ʈ ������ �о� ���̱� ���� ĳ��Ʈ ����ȯ(Byte point)
			char crc = 0;
			for( int i = 0 ;  i < sizeof(SOUNDLIST)*MAX_SOUND ;  i ++)		//Byte�� ��ŭ ����
			{
				crc += (*tt);		//�����Ͱ� ����Ű�� ���� ���� ������Ŵ
				tt ++;				//���� �����ͷ�	
			}

			fwrite( &crc, 1,1, file );
			fwrite( g_SoundList, sizeof(SOUNDLIST), MAX_SOUND, file);
			fwrite( &crc, 1,1, file );
			
			fclose(file);
		}
	}
	else
	{
		char *tt = (char *)g_SoundList;
		char crc = 0, crc1, crc2;
		file = Fopen( "./SOUND/SOUNDLIST.BIN", "rb" );
		///////////////////// 0615 LKH �߰� /////////////////////////
		if( file == NULL )
		{
			MessageBox(NULL, "./SOUND/SOUNDLIST.BIN is Null", 0, MB_OK);//010215 lsw
			return FALSE;
		}

		fread( &crc1, 1,1, file );			//CheckSum �� ����Ʈ�� �о�ͼ� Header������ ����
		fread( g_SoundList,  sizeof(SOUNDLIST), MAX_SOUND, file);
		fread( &crc2, 1,1, file );			//CheckSum �� ����Ʈ�� �о�ͼ� Teil������ ����
		fclose(file);

		for( int i = 0 ;  i < sizeof(SOUNDLIST)*MAX_SOUND ;  i ++)
		{
			crc += *tt;
			tt ++;
		}
		if( crc  != crc1 ) if( CheckSumError == 0 )  CheckSumError = CHECKSUM_ERROR_SOUNDLIST;
		if( crc  != crc2 ) if( CheckSumError == 0 )  CheckSumError = CHECKSUM_ERROR_SOUNDLIST;
	}
	return TRUE;
}


BOOL		//ȭ�Ϸ� ���� Wav ȭ�� �о�鿩 ���ۿ� ����
WaveInit( LPWAVEDATA* lplpWaveData, LPDIRECTSOUND lpDirectSound, LPSTR lpName )
{
	DSBUFFERDESC	dsbd = { 0 };
	LPBYTE			lpbWaveData;
	LPWAVEDATA		lpWaveData;
	HRESULT			hResult;

	lpWaveData = ( *lplpWaveData ) = ( LPWAVEDATA )GlobalAlloc( GPTR, sizeof( WAVEDATA ) );
	lpWaveData->lpDirectSoundBuffer = NULL;
	lpWaveData->lpName = NULL;
	lpWaveData->lpDS = NULL;

	lpWaveData->lpName = ( LPSTR )GlobalAlloc( GPTR, strlen( lpName ) + 1 );
	if ( lpWaveData->lpName != NULL )
	{
		strcpy( lpWaveData->lpName, lpName );

		if ( !WaveLoadFile( lpWaveData->lpName, (unsigned int *)&dsbd.dwBufferBytes, &dsbd.lpwfxFormat, &lpbWaveData ) )
		{
			dsbd.dwSize = sizeof( DSBUFFERDESC );
			dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;	//| DSBCAPS_CTRLPAN (CTRL3D�� �Բ� ���� �ȵ�)
			hResult = IDirectSound_CreateSoundBuffer( lpDirectSound, &dsbd, &lpWaveData->lpDirectSoundBuffer, NULL );
			
			if ( hResult == DS_OK )
			{
				if ( DSFillSoundBuffer( lpWaveData->lpDirectSoundBuffer, lpbWaveData, dsbd.dwBufferBytes ) )
				{
					GlobalFree(lpbWaveData);
					GlobalFree(dsbd.lpwfxFormat);
					lpWaveData->lpDS = lpDirectSound;
					return	TRUE;
				}

				IDirectSoundBuffer_Release( lpWaveData->lpDirectSoundBuffer );
				lpWaveData->lpDirectSoundBuffer = NULL;
			}
		}

		GlobalFree(lpWaveData->lpName );
		lpWaveData->lpName = NULL;
	}	

	GlobalFree( *lplpWaveData );
	*lplpWaveData = NULL;

	return	FALSE;
}

BOOL		//���̷�Ʈ ���� ���ۿ��� �ش� wav�� ������
WaveGetBuffers( LPWAVEDATA lpWaveData, LPDIRECTSOUNDBUFFER* lplpDirectSoundBuffer, LPDIRECTSOUND3DBUFFER* lplpDirectSound3DBuffer )
{
	*lplpDirectSound3DBuffer = NULL;
	*lplpDirectSoundBuffer = lpWaveData->lpDirectSoundBuffer;
	
	if ( lpWaveData->lpDirectSoundBuffer->QueryInterface( IID_IDirectSound3DBuffer, ( LPVOID* )lplpDirectSound3DBuffer ) != DS_OK )
	{
		MessageBox(NULL, "DirectSound3DBuffer Query Error!", 0, MB_OK );
		WaveReleaseSoundBuffer( lplpDirectSoundBuffer, lplpDirectSound3DBuffer );
		return	FALSE;
	}

	return	TRUE;
}

void 
WaveFree( LPWAVEDATA lpWaveData )		//wav ���������� �� �̸� / ���� ������ ������
{
	if ( lpWaveData != NULL )
	{
		if ( lpWaveData->lpName != NULL )
		{
			GlobalFree( lpWaveData->lpName );
			lpWaveData->lpName = NULL;
		}

		GlobalFree( lpWaveData );
		lpWaveData = NULL;
	}
}

void		//���� ���� ������ ������(**DirectSoundBuffer/**DirectSoundBuffer3DBuffer)
WaveReleaseSoundBuffer( LPDIRECTSOUNDBUFFER* lplpDirectSoundBuffer, LPDIRECTSOUND3DBUFFER* lplpDirectSound3DBuffer )
{
    if ( *lplpDirectSoundBuffer != NULL )
    {
		IDirectSoundBuffer_Release( *lplpDirectSoundBuffer );
		*lplpDirectSoundBuffer = NULL;
    }

	if ( *lplpDirectSound3DBuffer != NULL )
	{
		IDirectSoundBuffer_Release( *lplpDirectSound3DBuffer );
		*lplpDirectSound3DBuffer = NULL;
	}
}

BOOL
WaveReload( LPWAVEDATA lpWaveData )
{
	return	DSReloadSoundBuffer( lpWaveData->lpDirectSoundBuffer, lpWaveData->lpName );
}

BOOL
DSFillSoundBuffer( IDirectSoundBuffer* pDSB, BYTE* pbWaveData, DWORD cbWaveSize )
{
	if ( pDSB && pbWaveData && cbWaveSize )
	{
		LPVOID	pMem1, pMem2;
		DWORD	dwSize1, dwSize2;

		if ( SUCCEEDED( IDirectSoundBuffer_Lock( pDSB, 0, cbWaveSize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0 ) ) )
		{
			ZeroMemory( pMem1, dwSize1 );
			CopyMemory( pMem1, pbWaveData, dwSize1 );

			if ( dwSize2 != 0 )
			{
				CopyMemory( pMem2, pbWaveData+dwSize1, dwSize2 );
			}

			IDirectSoundBuffer_Unlock( pDSB, pMem1, dwSize1, pMem2, dwSize2 );

			return	TRUE;
		}
	}
	return	FALSE;
}

BOOL
DSReloadSoundBuffer( IDirectSoundBuffer* pDSB, LPCTSTR lpName )
{
	BOOL	result = FALSE;
	BYTE*	pbWaveData;
	DWORD	cbWaveSize;

	if ( !WaveLoadFile( ( char* )lpName, (unsigned int *)&cbWaveSize, NULL, &pbWaveData ) )
	{
		if ( SUCCEEDED( IDirectSoundBuffer_Restore( pDSB ) ) )
		{
			if ( DSFillSoundBuffer( pDSB, pbWaveData, cbWaveSize ) )
			{
				result = TRUE;
			}
		}
	}
	return	result;
}

int
WaveOpenFile( char* pszFileName, HMMIO* phmmioIn, WAVEFORMATEX** ppwfxInfo, MMCKINFO* pckInRIFF, int *encoding )
{
	HMMIO           hmmioIn;
	MMCKINFO        ckIn;
	PCMWAVEFORMAT   pcmWaveFormat;
	WORD            cbExtraAlloc;
	int             nError;

	*ppwfxInfo = NULL;
	nError = 0;
	hmmioIn = NULL;
	
	if ( ( hmmioIn = mmioOpen( pszFileName, NULL, MMIO_ALLOCBUF | MMIO_READ ) ) == NULL )
	{
		nError = ER_CANNOTOPEN;
		goto	ERROR_READING_WAVE;
	}

	if ( ( nError = ( int )mmioDescend( hmmioIn, pckInRIFF, NULL, 0 ) ) != 0 )
	{
		goto	ERROR_READING_WAVE;
	}


	if( pckInRIFF->ckid == mmioFOURCC('D', 'R', 'A', ' ') )
	{
		*encoding = WAVE_ENCODING_;
		pckInRIFF->ckid = FOURCC_RIFF;
	}

	if ( ( pckInRIFF->ckid != FOURCC_RIFF ) || ( pckInRIFF->fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) )
	{
		nError = ER_NOTWAVEFILE;
		goto	ERROR_READING_WAVE;
	}
			
    ckIn.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
    if ( ( nError = ( int )mmioDescend( hmmioIn, &ckIn, pckInRIFF, MMIO_FINDCHUNK ) ) != 0 )
	{
		goto	ERROR_READING_WAVE;
	}
					
    if ( ckIn.cksize < ( long )sizeof( PCMWAVEFORMAT ) )
	{
		nError = ER_NOTWAVEFILE;
		goto	ERROR_READING_WAVE;
	}
															
    if ( mmioRead( hmmioIn, ( HPSTR )&pcmWaveFormat, ( long )sizeof( pcmWaveFormat ) ) != ( long )sizeof( pcmWaveFormat ) )
	{
		nError = ER_CANNOTREAD;
		goto	ERROR_READING_WAVE;
	}
							

	if ( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{
		cbExtraAlloc = 0;
	}
	else
	{
		if ( mmioRead( hmmioIn, ( LPTSTR )&cbExtraAlloc, ( long )sizeof( cbExtraAlloc ) ) != ( long )sizeof( cbExtraAlloc ) )
		{
			nError = ER_CANNOTREAD;
			goto	ERROR_READING_WAVE;
		}
	}
							
	if ( ( *ppwfxInfo = (WAVEFORMATEX*)GlobalAlloc( GMEM_FIXED, sizeof( WAVEFORMATEX ) + cbExtraAlloc ) ) == NULL )
	{
		nError = ER_MEM;
		goto	ERROR_READING_WAVE;
	}

	memcpy( *ppwfxInfo, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
	( *ppwfxInfo )->cbSize = cbExtraAlloc;

	if ( cbExtraAlloc != 0 )
	{
		if ( mmioRead( hmmioIn, ( LPTSTR )( ( ( BYTE* )&( ( *ppwfxInfo )->cbSize ) ) + sizeof( cbExtraAlloc ) ), ( long )( cbExtraAlloc ) ) != ( long )( cbExtraAlloc ) )
		{
			nError = ER_NOTWAVEFILE;
			goto	ERROR_READING_WAVE;
		}
	}

	if ( ( nError = mmioAscend( hmmioIn, &ckIn, 0 ) ) != 0 )
	{
		goto	ERROR_READING_WAVE;
	}

	goto	TEMPCLEANUP;

ERROR_READING_WAVE:

	if ( *ppwfxInfo != NULL )
	{
		GlobalFree( *ppwfxInfo );
		*ppwfxInfo = NULL;
	}               

	if ( hmmioIn != NULL )
	{
		mmioClose( hmmioIn, 0 );
		hmmioIn = NULL;
	}
	
TEMPCLEANUP:

	*phmmioIn = hmmioIn;
	return	nError;
}

int
WaveStartDataRead( HMMIO* phmmioIn, MMCKINFO* pckIn, MMCKINFO* pckInRIFF )
{
	int		nReturn = 0;

	if ( ( nReturn = mmioSeek( *phmmioIn, pckInRIFF->dwDataOffset + sizeof( FOURCC ), SEEK_SET ) ) == -1 )
	{
		return	nReturn;
	}

	nReturn = 0;
	pckIn->ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if ( ( nReturn = mmioDescend( *phmmioIn, pckIn, pckInRIFF, MMIO_FINDCHUNK ) ) != 0 )
	{
		return	nReturn;
	}

	return	nReturn;
}




int		//stream ���ۿ��� �÷��̽�
WaveReadFile( HMMIO hmmioIn, UINT cbRead, BYTE *pbDest, MMCKINFO *pckIn, UINT *cbActualRead, int type )
{
	MMIOINFO	mmioinfoIn;
	int			nError;
	UINT		cT, cbDataIn;

	nError = 0;

	if ( nError = mmioGetInfo( hmmioIn, &mmioinfoIn, 0 ) != 0 )
	{
		goto	ERROR_CANNOT_READ;
	}
				
	cbDataIn = cbRead;
	if ( cbDataIn > pckIn->cksize )
	{
		cbDataIn = pckIn->cksize;
	}

	pckIn->cksize -= cbDataIn;
	

	if( type == WAVE_ENCODING_ )
	{
		char crc = 0;
		for ( cT = 0; cT < cbDataIn; cT++ )
		{
			if ( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
			{
				if ( ( nError = mmioAdvance( hmmioIn, &mmioinfoIn, MMIO_READ ) ) != 0 )
				{
					goto	ERROR_CANNOT_READ;
				} 
				if ( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
				{
					nError = ER_CORRUPTWAVEFILE;
					goto	ERROR_CANNOT_READ;
				}
			}
				
			*( ( BYTE* )pbDest+cT ) = *( ( BYTE* )mmioinfoIn.pchNext ++ ) ^ crc;
			crc ++;
		}
	}
	else
	{

		for ( cT = 0; cT < cbDataIn; cT++ )
		{
			if ( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
			{
				if ( ( nError = mmioAdvance( hmmioIn, &mmioinfoIn, MMIO_READ ) ) != 0 )
				{
					goto	ERROR_CANNOT_READ;
				} 
				if ( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
				{
					nError = ER_CORRUPTWAVEFILE;
					goto	ERROR_CANNOT_READ;
				}
			}
				
			*( ( BYTE* )pbDest+cT ) = *( ( BYTE* )mmioinfoIn.pchNext ++ );
		}
	}

	if ( ( nError = mmioSetInfo( hmmioIn, &mmioinfoIn, 0 ) ) != 0 )
	{
		goto	ERROR_CANNOT_READ;
	}

	*cbActualRead = cbDataIn;
	goto	FINISHED_READING;

ERROR_CANNOT_READ:

	*cbActualRead = 0;

FINISHED_READING:

	return	nError;
}

int
WaveCloseReadFile( HMMIO *phmmio, WAVEFORMATEX **ppwfxSrc )
{
	if ( *ppwfxSrc != NULL )
	{
		GlobalFree( *ppwfxSrc );
		*ppwfxSrc = NULL;
	}

	if ( *phmmio != NULL )
	{
		mmioClose( *phmmio, 0 );
		*phmmio = NULL;
	}

	return	0;
}

int
WaveLoadFile( char* pszFileName, UINT* cbSize, WAVEFORMATEX** ppwfxInfo, BYTE** ppbData )
{
	HMMIO		hmmioIn;        
	MMCKINFO	ckInRiff;
	MMCKINFO	ckIn;
	int			nError;
	UINT		cbActualRead;
	int			encoding = 0;

	*ppbData = NULL;
	*ppwfxInfo = NULL;
	*cbSize = 0;
	
	if ( ( nError = WaveOpenFile( pszFileName, &hmmioIn, ppwfxInfo, &ckInRiff, &encoding ) ) != 0 )
	{
		goto	ERROR_LOADING;
	}

	if ( ( nError = WaveStartDataRead( &hmmioIn, &ckIn, &ckInRiff ) ) != 0 )
	{
		goto	ERROR_LOADING;
	}

	if ( ( *ppbData = ( BYTE* )GlobalAlloc( GMEM_FIXED, ckIn.cksize ) ) == NULL )
	{
		nError = ER_MEM;
		goto	ERROR_LOADING;
	}

	if ( ( nError = WaveReadFile( hmmioIn, ckIn.cksize, *ppbData, &ckIn, &cbActualRead, encoding ) ) != 0 )
	{
		goto	ERROR_LOADING;
	}        
	
	*cbSize = cbActualRead;
	goto	DONE_LOADING;

ERROR_LOADING:

	//// 0615 LKH Ȥ�� ���� ������ ȭ���� �������� �ʴ� ��츦 ã�� ���� �׽�Ʈ ��ƾ(������Ʈ�ϸ� �ȵ�)  ////
	//char	error_message[40];
	//sprintf(error_message, "%s ȭ�Ͽ� ������ �ֽ��ϴ�.", pszFileName);
	//MessageBox(NULL, error_message, 0, MB_OK);
	
	if ( *ppbData != NULL )
	{
		GlobalFree( *ppbData );
		*ppbData = NULL;
	}
	
	if ( *ppwfxInfo != NULL )
	{
		GlobalFree( *ppwfxInfo );
		*ppwfxInfo = NULL;
	}
			
DONE_LOADING:

	if ( hmmioIn != NULL )
	{
		mmioClose( hmmioIn, 0 );
		hmmioIn = NULL;
	}

	return	nError;
}

///////////////////////////////////////////////////////////////////////////////
//

BOOL
InitSfx( HWND hWnd )
{
	g_SoundInfo.bInitialized = FALSE;
	
	if ( !InitGlobals( hWnd ) )
	{
		return	FALSE;
	}
	
	if ( !InitFixedSounds( ) )		//�ʱ⿡ ��Ÿƽ ���� ������ �о�ͼ� ���ۿ� ����
	{
		FreeGlobals( );
		return	FALSE;
	}
	
	g_SoundInfo.bInitialized = TRUE;
	
	return	TRUE;
}

void
FreeSfx( void )
{
	if ( g_SoundInfo.bInitialized )
	{
		FreeFixedSounds( );
		FreeGlobals( );
	}
}

//////////////// SoundUp lkh ���� ////////////////// ( BGM�� ��� ���׷��� / ȿ������ ��� ��� : 2������ ���� �ʿ�)
BOOL	InitGlobals( HWND hWnd )
{
	PCMWAVEFORMAT	pcmwf;
	DSBUFFERDESC	dsbdesc;

	if ( DirectSoundCreate( NULL, &g_SoundInfo.lpDirectSound, NULL ) != DS_OK )		//IDirectSound�������̽��� ������ �ʱ�ȭ
    {
//		MessageBox(NULL, "IDirectSound�������̽� ���� ����", 0, MB_OK);
		return	FALSE;
	}

	// PCM ���̺� ������ ����� �����Ϳ� ���� ����
	memset( &pcmwf, 0, sizeof( PCMWAVEFORMAT ) );

	pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels = 2;					//1:mono    2:stereo
	pcmwf.wf.nSamplesPerSec = 22050;
	pcmwf.wf.nBlockAlign = 4;
	pcmwf.wf.nAvgBytesPerSec = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample = 16;
	
	// ���̷�Ʈ ���� ������ Ư���� ���
	memset( &dsbdesc, 0, sizeof( DSBUFFERDESC ) );

	dsbdesc.dwSize = sizeof( DSBUFFERDESC );
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_MUTE3DATMAXDISTANCE; //| DSBCAPS_CTRLPAN (2D������ ���)
	//| DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY ;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	if ( IDirectSound_SetCooperativeLevel( g_SoundInfo.lpDirectSound, hWnd, DSSCL_NORMAL ) == DS_OK )													//���� ��ġ�� ����� ���� ���μ��� ������ ���õ� ���� ����
	{
		if ( IDirectSound_CreateSoundBuffer( g_SoundInfo.lpDirectSound, &dsbdesc, &g_SoundInfo.lpPrimaryBuffer, NULL ) == DS_OK )						//���̷�Ʈ ���� ���� ��ü�� ����
		{
			if ( g_SoundInfo.lpPrimaryBuffer->QueryInterface( IID_IDirectSound3DListener, ( void** )&g_SoundInfo.lpDirectSound3DListener ) == DS_OK )	//lpDirectSound3DListener �������̽��� ���� ������ ����
			{
				if ( IDirectSound3DListener_SetPosition( g_SoundInfo.lpDirectSound3DListener, 0.0f, 0.0f, 0.0f, DS3D_DEFERRED ) == DS_OK )				//lpDirectSound3DListener�� ��ġ ����(default:X:0,Y:0,Z:0)
				{
					if( IDirectSound3DListener_SetDistanceFactor( g_SoundInfo.lpDirectSound3DListener, 1.f, DS3D_DEFERRED ) == DS_OK )					//�Ÿ��� ������ ���� �� ��Ʈ�� 50Cm ������
					{
						if ( IDirectSound3DListener_SetDopplerFactor( g_SoundInfo.lpDirectSound3DListener, DS3D_DEFAULTDOPPLERFACTOR, DS3D_DEFERRED ) == DS_OK )		//���÷� ���� ����
						{
							//if ( IDirectSound3DListener_SetRolloffFactor( g_SoundInfo.lpDirectSound3DListener, DS3D_DEFAULTROLLOFFFACTOR*5, DS3D_DEFERRED ) == DS_OK )	//�ѿ��� ����(�Ÿ��� ���� �Ҹ��� ���� ����) ����
							if ( IDirectSound3DListener_SetRolloffFactor( g_SoundInfo.lpDirectSound3DListener, DS3D_DEFAULTROLLOFFFACTOR*10, DS3D_DEFERRED ) == DS_OK )	//�ѿ��� ����(�Ÿ��� ���� �Ҹ��� ���� ����) ����
							{
								if ( IDirectSound3DListener_CommitDeferredSettings( g_SoundInfo.lpDirectSound3DListener ) == DS_OK )						//���� ��� ������ �� �޼ҵ带 ȣ���ϹǷμ� ���� ó�� �ȴ�.
								{
									return	TRUE;
								}
								else	MessageBox(NULL, "IDirectSound3DListener_CommitDeferredSettings Fail", 0, MB_OK);
							}
							else	MessageBox(NULL, "IDirectSound3DListener_SetRolloffFactor Fail", 0, MB_OK);
						}
						else	MessageBox(NULL, "IDirectSound3DListener_SetDopplerFactor Fail", 0, MB_OK);
					}
					else	MessageBox(NULL, "IDirectSound3DListener_SetDistanceFactor Fail", 0, MB_OK);
				}
				else	MessageBox(NULL, "IDirectSound3DListener_SetPosition Fail", 0, MB_OK);	
			}
			else	MessageBox(NULL, "lpDirectSound3DListener InterFace Fail", 0, MB_OK);	
		}
		else	MessageBox(NULL, "IDirectSound_CreateSoundBuffer Create Fail", 0, MB_OK);
	}
	else	MessageBox(NULL, "IDirectSound_SetCooperativeLevel Fail", 0, MB_OK);


	//���������� 
	FreeGlobals( );

	return	FALSE;
}


void
FreeGlobals( void )		//1������/3D ����/���̷�Ʈ ���� ��ü ������
{
	if ( g_SoundInfo.lpPrimaryBuffer != NULL )
	{
		g_SoundInfo.lpPrimaryBuffer->Release( );
		g_SoundInfo.lpPrimaryBuffer = NULL;
	}

	if ( g_SoundInfo.lpDirectSound3DListener != NULL )
	{
		IDirectSound3DListener_Release( g_SoundInfo.lpDirectSound3DListener );
		g_SoundInfo.lpDirectSound3DListener = NULL;
	}

	if ( g_SoundInfo.lpDirectSound != NULL )
	{
		IDirectSound_Release( g_SoundInfo.lpDirectSound );
		g_SoundInfo.lpDirectSound = NULL;
	}
}

BOOL
InitFixedSounds( void )		//�����ε� wave ȭ���� ��Ÿƽ ���� ����/����
{
	char	szFilePath[ _MAX_PATH ];
	int		index;
	BOOL	bRetVal = TRUE;

	FreeFixedSounds( );

	for( index=0 ; index < MAX_SOUND ; index++ )
	{			
		if( g_SoundList[index].static_Sound )
		{		
			wsprintf( szFilePath, "%s/sound/%s", GetCurrentWorkingDirectory(), g_SoundList[index].sound_FileName);
				
			if ( WaveInit( &g_SoundInfo.lpWaveData[ index ], g_SoundInfo.lpDirectSound, szFilePath ) == FALSE )
			{	
				//bRetVal = FALSE;
				//break;
				continue;
			}	
				
			if ( !WaveGetBuffers( g_SoundInfo.lpWaveData[ index ], &g_SoundInfo.lpDirectSoundBuffer[ index ], &g_SoundInfo.lpDirectSound3DBuffer[ index ] ) )
			{	
				bRetVal = FALSE;
				break;
			}	
		}		
	}			
	return	bRetVal;
}				


void
FreeFixedSounds( void )
{
	for ( int i = 0; i < MAX_SOUND ; i++ )
	{
		if( g_SoundList[i].static_Sound )
		{
			if ( g_SoundInfo.lpWaveData[ i ] )
			{
				WaveFree( g_SoundInfo.lpWaveData[ i ] );
				g_SoundInfo.lpWaveData[ i ] = NULL;
			}
		}
	}
}

void
FreeAutoSounds( int index )
{
	if ( g_SoundInfo.lpDirectSound3DBuffer[ index ]!=NULL )
	{
		IDirectSoundBuffer_Release( g_SoundInfo.lpDirectSound3DBuffer[ index ] );
		g_SoundInfo.lpDirectSound3DBuffer[ index ] = NULL;
	}

	if ( g_SoundInfo.lpDirectSoundBuffer[ index ]!=NULL )
	{
		IDirectSoundBuffer_Release( g_SoundInfo.lpDirectSoundBuffer[ index ] );
		g_SoundInfo.lpDirectSoundBuffer[ index ] = NULL;
	}

	if ( g_SoundInfo.lpWaveData[ index ]!=NULL )
	{
		WaveFree( g_SoundInfo.lpWaveData[ index ] );
		g_SoundInfo.lpWaveData[ index ] = NULL;
	}
}

void	SetListener( int x, int y )
{
	IDirectSound3DListener_SetPosition( g_SoundInfo.lpDirectSound3DListener, P2M( x * TILE_SIZE ), P2M( y * TILE_SIZE ), 0.f, DS3D_IMMEDIATE );
}


// flag == 1 : looping.
// flag != 0 : �ش� Ƚ����ŭ �ݺ�.
// flag == 0 : 1 time.



///////////////////// SoundUp lkh ���� ////////////////////
BOOL PlayListAutoSounds( int index, int x, int y, BOOL flag)		//index->1~999����
{
	if(g_Volume_Off!=0)		return FALSE;

	if(index >= MAX_SOUND || index <= 0 )	return FALSE; 

	char	szFilePath[ _MAX_PATH ];
					
	if(!g_SoundList[index].static_Sound)
	{				
		if( g_SoundInfo.bInitialized == FALSE || g_SoundInfo.lpDirectSound == NULL ) return 0;
		//////////////// ���� ���ۿ� �̹� �ε��Ǿ� �ִ� ������ ��� ��ε����� �ʵ��� �Ұ� /////////////////////
		if( g_SoundInfo.lpDirectSoundBuffer[ index ] == NULL )//!IsPlaying(index) )
		{			
			if( checkbeta == '1' )
			{	//< CSD-031030
				wsprintf( szFilePath, "%s/sound_b5/%s", GetCurrentWorkingDirectory(), g_SoundList[index].sound_FileName);
			}	//> CSD-031030
			else wsprintf( szFilePath, "%s/sound/%s", GetCurrentWorkingDirectory(), g_SoundList[index].sound_FileName);
		
			//���� wav ȭ���� �о�� ����ƽ ���ۿ� �ε�
			if ( WaveInit( &g_SoundInfo.lpWaveData[ index ], g_SoundInfo.lpDirectSound, szFilePath ) == FALSE )	
			{
				if( checkbeta == '1' )
				{
					wsprintf( szFilePath, "%s/sound/%s", GetCurrentWorkingDirectory(), g_SoundList[index].sound_FileName);
					if ( WaveInit( &g_SoundInfo.lpWaveData[ index ], g_SoundInfo.lpDirectSound, szFilePath ) == FALSE )	return false;
				}
				else return FALSE;
			}
			//���� �ش� wav ȭ���� �÷��̽�Ű�µ� ������ ������ üũ
			if ( !WaveGetBuffers( g_SoundInfo.lpWaveData[ index ], &g_SoundInfo.lpDirectSoundBuffer[ index ], &g_SoundInfo.lpDirectSound3DBuffer[ index ] ) )
			{
				return FALSE;
			}
		}
	}
	
	//////////////////////// 0209 lkh �߰� ///////////////////////////
	else		//���� static buffer ������ ���
	{
		if( g_SoundInfo.bInitialized == FALSE || g_SoundInfo.lpDirectSound == NULL ) return 0;
		/*
		if( g_SoundInfo.lpDirectSoundBuffer[ index ] == NULL )		//��Ÿƽ ���ۿ� ���� ���� ����Ÿ ���� ���
		{
			wsprintf( szFilePath, "%s/sound/%s", GetCurrentWorkingDirectory(), g_SoundList[index].sound_FileName);		

			//���� wav ȭ���� �о�� ����ƽ ���ۿ� �ε�
			if ( WaveInit( &g_SoundInfo.lpWaveData[ index ], g_SoundInfo.lpDirectSound, szFilePath ) == FALSE )	
			{
				return FALSE;
			}
			//���� �ش� wav ȭ���� �÷��̽�Ű�µ� ������ ������ üũ
			if ( !WaveGetBuffers( g_SoundInfo.lpWaveData[ index ], &g_SoundInfo.lpDirectSoundBuffer[ index ], &g_SoundInfo.lpDirectSound3DBuffer[ index ] ) )
			{
				return FALSE;
			}
		}
		*/
	}
	
	PlayWave( index, x, y, flag );
	return TRUE;
}

BOOL AdjustStaticSounds( int index, int volume, bool loop)
{

	if ( g_SoundInfo.lpDirectSoundBuffer[ index ])
	{
		IDirectSoundBuffer_SetVolume( g_SoundInfo.lpDirectSoundBuffer[ index ], volume * 100 + DSBVOLUME_MIN );
	}

	if(loop)
		PlayWave( index, 0, 0, 1 );
	else
		PlayWave( index, 0, 0, 0 );
	return TRUE;
}


void	AdjustSounds( int index, int volume, bool loop)
{
		
	if( g_SoundInfo.bInitialized == FALSE || g_SoundInfo.lpDirectSound == NULL ) return;
	if(index >= MAX_SOUND || index <= 0 )	return;
	if(g_Volume_Off!=0)						return;

	BOOL	result;


	if( g_SoundList[index].static_Sound )		//static sound �� ���
		result = AdjustStaticSounds( index, volume, loop);
	else
		result = AdjustAutoSounds( index, volume, loop);

////////////////// 0621 lkh test�� /////////////////
//	if(!result)
//	{
//		sprintf(error_message, "%d.wavȭ�� �÷��� Error", index);
//		MessageBox(NULL, error_message, 0, MB_OK);
//	}
}

BOOL AdjustAutoSounds( int index, int volume, bool loop)		//index->1~999����
{
	char	szFilePath[ _MAX_PATH ];

	if(!g_SoundList[index].static_Sound)
	{
		if( g_SoundInfo.bInitialized == FALSE || g_SoundInfo.lpDirectSound == NULL ) return 0;
			
		///////////////////// 0508 lkh �߰� ///////////////////////
		if( g_SoundInfo.lpDirectSoundBuffer[ index ] == NULL )		//��Ÿƽ ���ۿ� ���� ���� ����Ÿ ���� ���
		{
			wsprintf( szFilePath, "%s/sound/%s", GetCurrentWorkingDirectory(), g_SoundList[index].sound_FileName);			

			//���� wav ȭ���� �о�� ����ƽ ���ۿ� �ε�
			if ( WaveInit( &g_SoundInfo.lpWaveData[ index ], g_SoundInfo.lpDirectSound, szFilePath ) == FALSE )	
			{
				return FALSE;
			}
			//���� �ش� wav ȭ���� �÷��̽�Ű�µ� ������ ������ üũ
			if ( !WaveGetBuffers( g_SoundInfo.lpWaveData[ index ], &g_SoundInfo.lpDirectSoundBuffer[ index ], &g_SoundInfo.lpDirectSound3DBuffer[ index ] ) )
			{
				return FALSE;
			}
		}
		
		/*
		//////////////// ���� ���ۿ� �̹� �ε��Ǿ� �ִ� ������ ��� ��ε����� �ʵ��� �Ұ� /////////////////////
		if( g_SoundInfo.lpDirectSoundBuffer[ index ] == NULL )//!IsPlaying(index) )
		{
			wsprintf( szFilePath, "%s/sound/%s", GetCurrentWorkingDirectory(), g_SoundList[index].sound_FileName);		

			//���� wav ȭ���� �о�� ����ƽ ���ۿ� �ε�
			if ( WaveInit( &g_SoundInfo.lpWaveData[ index ], g_SoundInfo.lpDirectSound, szFilePath ) == FALSE )
			{
				return FALSE;
			}
			//���� �ش� wav ȭ���� �÷��̽�Ű�µ� ������ ������ üũ
			if ( !WaveGetBuffers( g_SoundInfo.lpWaveData[ index ], &g_SoundInfo.lpDirectSoundBuffer[ index ], &g_SoundInfo.lpDirectSound3DBuffer[ index ] ) )
			{
				return FALSE;
			}
		}
		*/
	}

	if ( g_SoundInfo.lpDirectSoundBuffer[ index ])
	{
		IDirectSoundBuffer_SetVolume( g_SoundInfo.lpDirectSoundBuffer[ index ], volume * 100 + DSBVOLUME_MIN );
	}

	if(loop)
		PlayWave( index, 0, 0, 1 );
	else
		PlayWave( index, 0, 0, 0 );
	return TRUE;
}

extern int SoundRangeX;		// LTS SOUND
extern int SoundRangeY;		// LTS SOUND

void
PlayWave( int index, int x, int y, BOOL flag )
{
	if( index > MAX_SOUND || index < 0 ) return;

	if ( g_SoundInfo.bInitialized )
	{
		if( g_SoundInfo.lpDirectSound3DBuffer[ index ] )
		//if ( IDirectSound3DBuffer_SetPosition( g_SoundInfo.lpDirectSound3DBuffer[ index ], P2M( x * TILE_SIZE ), P2M( y * TILE_SIZE ), D3DVAL( 1 ), DS3D_DEFERRED ) == DS_OK )
		if ( IDirectSound3DBuffer_SetPosition( g_SoundInfo.lpDirectSound3DBuffer[ index ], x/SoundRangeX,y/SoundRangeY/*P2M( x*3 ), P2M( y*3 )*/, D3DVAL( 0 ), DS3D_DEFERRED ) == DS_OK )	// LTS SOUND
		{
			if ( IDirectSoundBuffer_SetCurrentPosition( g_SoundInfo.lpDirectSoundBuffer[ index ], 0 ) == DS_OK )
			{
				if ( IDirectSound3DListener_CommitDeferredSettings( g_SoundInfo.lpDirectSound3DListener ) == DS_OK )
				{
					//���� ����ƽ sound buffer�� ����Ǿ� �ִ� ȭ���� ��� ���ۿ��� ���� Play��Ű��, �����ϴ� ��� �ٽ� ���ۿ� �о� �鿩 Play
					if ( IDirectSoundBuffer_Play( g_SoundInfo.lpDirectSoundBuffer[ index ], 0, 0, flag * DSBPLAY_LOOPING ) == DSERR_BUFFERLOST )
					{
						WaveReload( g_SoundInfo.lpWaveData[ index ] );
					}
				}
			}
		}
	}
}

//021030 YGI
void
StopWave( int index )
{
	if( index == SN_START )
	{
		StartSound(1); 
		return;
	}
	if( g_SoundInfo.lpDirectSoundBuffer[ index ] )
		IDirectSoundBuffer_Stop( g_SoundInfo.lpDirectSoundBuffer[ index ] );
}

void
MoveWave( int index, int x, int y )
{
	if( g_SoundInfo.lpDirectSound3DBuffer[ index ] )
		IDirectSound3DBuffer_SetPosition( g_SoundInfo.lpDirectSound3DBuffer[ index ], P2M( x * TILE_SIZE/4 ), P2M( y * TILE_SIZE/4 ), D3DVAL( 1 ), DS3D_IMMEDIATE );
} 


void SetVolumeOne( int index, int volume )
{
	if ( g_SoundInfo.lpDirectSoundBuffer[ index ])
	{
		IDirectSoundBuffer_SetVolume( g_SoundInfo.lpDirectSoundBuffer[ index ], volume * 100 + DSBVOLUME_MIN );
	}
}	
	
	
// volume : 0 - 100
void						//ȿ���� ���� ������ ���� ����
SetVolume( int volume )
{	
	if(volume == 0)
		volume = DSBVOLUME_MIN;
	else
		volume = (100 - (100 - volume)/5)* 100 + DSBVOLUME_MIN;
	for ( int i = 0; i < MAX_SOUND; i++ )
	{
		if( i < SN_START || i > SN_BGMEND )		//������� ���۸� ������ ������ ȿ���� ���� ���� DOWN
		{
			if ( g_SoundInfo.lpDirectSoundBuffer[ i ] )
			{
				IDirectSoundBuffer_SetVolume( g_SoundInfo.lpDirectSoundBuffer[ i ], volume );
			}
		}
	}
}	
	
void						//��� ������ ���� ����(201->�������̽�~206�ΰ�����)
SetVolumeMusic( int volume )
{
	if(volume == 0)
		volume = DSBVOLUME_MIN;
	else
		volume = (100 - (100 - volume)/5)* 100 + DSBVOLUME_MIN;
	//////////////////////// SoundUp lkh ���� //////////////////////////
	for ( int i = SN_START; i <= SN_BGMEND; i++)
	{
		if( g_SoundInfo.lpDirectSoundBuffer[i] )
			IDirectSoundBuffer_SetVolume( g_SoundInfo.lpDirectSoundBuffer[ i ], volume );
	}
}	
	
BOOL IsPlaying( int index )			//���ϰ��� true->�÷������� / false->Not playing
{	
	DWORD	dwStatus;
	
	if ( g_SoundInfo.lpDirectSoundBuffer[ index ] == NULL )		//���۰� ��� �ִ� ���(���� �ҽ��� �ε��� �ȵ� ����)
	{
		////////////////////// SoundUp lkh ���� ////////////////////////
		return FALSE;
	}
	
	if ( IDirectSoundBuffer_GetStatus( g_SoundInfo.lpDirectSoundBuffer[ index ], &dwStatus ) == DS_OK )
	{
		if ( dwStatus & DSBSTATUS_PLAYING )
		{
			return	TRUE;
		}
	}
	
	return	FALSE;
}	
	
//////////////// SoundUp lkh ���� /////////////////
void CheckAutoWave( void )
{		
	for ( int i = 0; i<MAX_SOUND; i++ )
	{	
		if( !g_SoundList[i].static_Sound )		//static sound�� �ƴ� ���
		{
			if ( g_SoundInfo.lpDirectSoundBuffer[ i ] != NULL )
			{
				if ( !IsPlaying( i ) )
				{
					FreeAutoSounds( i );
				}
			}
		}
	}
}	
	
/*	
short int g_Current_Volume			//������ ���� ����
short int g_To_Volume				//��ȯ�Ǿ�� �ϴ� (����������) ��ǥ ���� ����
short int g_Change_Volume			//���� ��ġ ��ȯġ(-+ ��ȭ��->Fade Out/In)
*/	
	
void VolumeTranslate()
{	
	if( g_Current_Volume == g_To_Volume)
	{
		g_Change_Volume=0;
		return;
	}
	
	if(g_Change_Volume>0)		//fade in(+)
	{
		if(g_Current_Volume < g_To_Volume)
		{
			g_Current_Volume += g_Change_Volume;
			if(g_Current_Volume > g_To_Volume)
			{
				g_Current_Volume = g_To_Volume;
				g_Change_Volume=0;
				if(g_Current_Volume>100)	g_Current_Volume=100;
			}
		}
	}
	else if(g_Change_Volume<0)	//fade Out(-)
	{
		if(g_Current_Volume > g_To_Volume)	
		{
			g_Current_Volume += g_Change_Volume;
			if(g_Current_Volume < g_To_Volume)
			{
				g_Current_Volume = g_To_Volume;
				g_Change_Volume=0;
				if(g_Current_Volume<0)		g_Current_Volume=0;
			}
		}
	}
	SetVolume( g_Current_Volume );
}	
	
void VolumeSet(short int change_Volume, short int to_Volume)
{
	if(change_Volume==0 || to_Volume==g_Current_Volume)
	{
		g_Change_Volume = 0;
		return;
	}

	if(change_Volume>0 && (g_To_Volume<g_Current_Volume) )	//�Ҹ� ���� ���̷��ϴµ� ���� ������ ��ǥ�������� �� ���� ���
	{
		g_Change_Volume = 0;
		return;
	}
	if(change_Volume<0 && (g_To_Volume>g_Current_Volume) )	//�Ҹ� ���� ���߷��ϴµ� ���� ������ ��ǥ�������� �� ���� ���
	{
		g_Change_Volume = 0;
		return;
	}
	
	g_Change_Volume	=	change_Volume;
	g_To_Volume		=	to_Volume;
}	
	
	
static	int		old_Volume;
void	WeatherFXSound(void)			//RainAmount : 0 ~ 900
{	
	/*if( Statue_Weather==0 && IsPlaying(SOUND_RAINY))	//�� ���� �ʰ� �񳻸��� �Ҹ��� �÷��� �ǰ� �ִ� ���
		StopWave(SOUND_RAINY);
	
	if( IsPlaying(SOUND_RAINY) )
	{
		//////////////////////// õ�ռҸ� ���� ////////////////////////
		if( rand()%(4700-RainAmount*5)==1 && !IsPlaying(SOUND_LIGHTNING1) )
		{
			if (rand()%10>5)
			PlayListAutoSounds( SOUND_LIGHTNING1, rand()%100+100, rand()%100+50, 0 );	//3D sound�� ǥ��	// LTS SOUND
			else
			PlayListAutoSounds( SOUND_LIGHTNING1, (rand()%100+100)*-1, rand()%100+50, 0 );	//3D sound�� ǥ��
		}
		if( rand()%(4700-RainAmount*5)==1 && !IsPlaying(SOUND_LIGHTNING2) )
		{
			if (rand()%10>5)	// LTS SOUND
			PlayListAutoSounds( SOUND_LIGHTNING2, rand()%100+100, rand()%100+50, 0 );	//3D sound�� ǥ��
			else
			PlayListAutoSounds( SOUND_LIGHTNING2, (rand()%100+100)*-1, rand()%100+50, 0 );	//3D sound�� ǥ��
		}
			
		if(old_Volume!=RainAmount/16)
		{
			MoveWave( SOUND_RAINY, 0, 15-(RainAmount/60));
			return;
		}
		else						return;
	}
	
	switch(Statue_Weather)
	{
	case 1:		//��Ҹ�(& �����Ҹ����� ����)
		PlayListAutoSounds( SOUND_RAINY, 0, 0/*15-(RainAmount/60)*//*, 1 );	//3D sound�� ǥ�� // LTS SOUND
		break;
	case 2:		//���ö� �ٶ��Ҹ�

		break;
	}
	old_Volume = RainAmount/16;*/ //Disabled by Eleval cause made another system
}	
	
static	int		temp_Old_Volume;

void	BGMFadeInOut(void)				//��������� fade in/out ó���� ���� �Լ�
{
	return;
	/*
	int		fadeOut_Volume	= 100-BGM_SoundVolume;
	int		fadeIn_Volume	= BGM_SoundVolume;

	if(BGM_FadeInOut)
	{
		int	fadeOut_SoundNum = ReturnBGMNumber(OldMapNumber);
		if(!fadeOut_SoundNum)	fadeOut_SoundNum = 1;
		
		if(fadeOut_Volume<=0)
		{
			StopWave( fadeOut_SoundNum );
			OldMapNumber= MapNumber;
			BGM_FadeInOut = 0;
		}
		if(fadeIn_Volume>=100)
			BGM_SoundVolume=100;

		if( IsPlaying( fadeOut_SoundNum) )
			MoveWave( fadeOut_SoundNum, 0, 16-fadeOut_Volume/6 );
		if( IsPlaying(ReturnBGMNumber(MapNumber)) )
			MoveWave( ReturnBGMNumber(MapNumber), 0, 16-fadeIn_Volume/6 );
		BGM_SoundVolume+=3;
	}*/
}	
	
void StartSound( int type )		// �ʱ� ���� ����
{	//< CSD-030520
	static int nCount;
	if( type == 1 )		// ��ó��
	{
		nCount = 100;
	}

	if( nCount <= 0 ) return;

	if( IsPlaying( 1) )
	{
		MoveWave( 1, 0, 16 - nCount/6 );
	}
	nCount -= 3;
	if( nCount < 0 ) 
	{
		if( g_SoundInfo.lpDirectSoundBuffer[ 1 ] )
			IDirectSoundBuffer_Stop( g_SoundInfo.lpDirectSoundBuffer[ 1 ] );
	}
}	//> CSD-030520