#ifndef __MIDI_H__
#define __MIDI_H__

extern UINT DeviceID;

extern int	MciSend (UINT DeviceID, UINT wMessage, DWORD dwParam1, DWORD dwParam2);
extern UINT OpenMidi (HWND hwnd, char * FileName, DWORD dwFlags);
extern int	PlayMidi (HWND hwnd, DWORD dwFlags, DWORD dwFrom,DWORD dwTo);
extern void CloseMidi (HWND hwnd);



extern void	PlayBackMusic( HWND hwnd, int no );
extern void	StopBackMusic(  HWND hwnd );
extern void	VolumeBackMusic( int vol );



extern int CurMidi;


#else

UINT DeviceID;

int		MciSend (UINT DeviceID, UINT wMessage, DWORD dwParam1, DWORD dwParam2);
UINT	OpenMidi (HWND hwnd, char * FileName, DWORD dwFlags);
int		PlayMidi (HWND hwnd, DWORD dwFlags, DWORD dwFrom,DWORD dwTo);
void	CloseMidi (HWND hwnd);

void	PlayBackMusic( HWND hwnd, int no );
void	StopBackMusic(  HWND hwnd );
void	VolumeBackMusic( int vol );

int CurMidi = -1;

#endif