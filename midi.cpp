#include "StdAfx.h"

#include <WINDOWS.H>

#include <Stdio.h>
#define __MIDI_H__
#include "midi.h"

#include "Tool.h"
#include "Hong_Sub.h"

/*────────────────────────────────────────
 MCI에 메시지를 보낸다.
────────────────────────────────────────*/
int MciSend (UINT DeviceID, UINT wMessage, DWORD dwParam1, DWORD dwParam2)
{
	static char BufStr [256] ;
	DWORD       Error ;
	HWND      hwnd;

	Error = mciSendCommand (DeviceID, wMessage, dwParam1, dwParam2) ;//MCI 에 메시지를 보내는 함수.
	if (Error)
	{
		hwnd = (HWND) ((LPMCI_GENERIC_PARMS) dwParam2)->dwCallback ;

		if (!mciGetErrorString(Error, BufStr, sizeof (BufStr)))   // 에러가 발생할경우 처리해주는 함수.
			strcpy (BufStr, "Not Needed Known Error ") ;
//		MsgBox(BufStr);
	}
	return Error == 0 ;
}


/*────────────────────────────────────────
	MCI장치를 연다.
────────────────────────────────────────*/
UINT OpenMidi(HWND hwnd, char * FileName, DWORD dwFlags)
{
	BOOL                 bSuccess ;
	MCI_OPEN_PARMS    open ;

	open.dwCallback        = (DWORD) hwnd ;
	open.lpstrDeviceType     = "sequencer";
	open.lpstrElementName  = FileName ;
	open.lpstrAlias         = NULL ;

	bSuccess = MciSend (0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | dwFlags, 
											(DWORD) (LPMCI_OPEN_PARMS) & open) ;

	DeviceID = bSuccess ? open.wDeviceID : 0 ;
	
	return DeviceID;
}

/*────────────────────────────────────────
  MCI 장치를 연주한다.
────────────────────────────────────────*/
int PlayMidi (HWND hwnd, DWORD dwFlags, DWORD dwFrom,DWORD dwTo)
{
	MCI_PLAY_PARMS play ;

	play.dwCallback = (DWORD) hwnd ;
	play.dwFrom     = dwFrom ;
	play.dwTo       = dwTo ;

	return MciSend (DeviceID, MCI_PLAY, dwFlags, (DWORD) (LPMCI_PLAY_PARMS) &play) ;
}


/*────────────────────────────────────────
   MCI 장치를 닫는다. 메모리에 올라있는 모든 정보를 지운다.
────────────────────────────────────────*/
void CloseMidi(HWND hwnd)
{
	MCI_GENERIC_PARMS generic ;

	generic.dwCallback = (DWORD) hwnd ;

	MciSend (DeviceID,MCI_CLOSE,MCI_WAIT,(DWORD)(LPMCI_GENERIC_PARMS)&generic);
	DeviceID = 0;
}




//-----------------------
//-----------------------
//-----------------------			Midi Play....
//-----------------------
//-----------------------
//-----------------------


static int GetFileNameInNameFile( char *filename, char *namefile, int no )
{
	FILE *fp;
	int  c=0;
	
	fp = Fopen( namefile,"rt");
	if( fp )
	{
		while( fscanf( fp, "%s", filename ) != EOF )
		{
			if( c == no )	{ fclose( fp );		return 1; }
			c++;
		}
		fclose(fp);
	}
	return 0;
}
/*────────────────────────────────────────
────────────────────────────────────────*/
void StopBackMusic(  HWND hwnd )
{
	CurMidi = -1;
	CloseMidi( hwnd );
}
/*────────────────────────────────────────
//Volume의 범위 :   0 <= vol < 128
────────────────────────────────────────*/
void VolumeBackMusic( int vol )
{	
		
}	
/*────────────────────────────────────────
────────────────────────────────────────*/
void PlayBackMusic( HWND hwnd, int no )
{
	char MidiName[ MAX_PATH];
	char temp[ MAX_PATH];
	

	return;//


	if( SysInfo.music == 0 ) return;

	if( GetFileNameInNameFile( temp, "./sound/midi.nam", no ) )
	{
		sprintf( MidiName, "./sound/%s", temp );
		StopBackMusic( hwnd );
		CurMidi = no;
		OpenMidi( hwnd, MidiName,  MCI_WAIT );
		PlayMidi( hwnd, MCI_NOTIFY, 0, 0);
	}
	else 
	{
		JustMsg( "Midi Number Over.." );
	}
}



