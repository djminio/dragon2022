// Music.h: interface for the CMusic class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <objbase.h>
//#include <afxtempl.h>
#include <strmif.h>
#include <control.h>
//#include <evcode.h>
#include <uuids.h>
#include <assert.h>
//#include <reftime.h>

#if !defined(AFX_MUSIC_H__86E74FCC_F274_11D2_BBB1_00E02917BC84__INCLUDED_)
#define AFX_MUSIC_H__86E74FCC_F274_11D2_BBB1_00E02917BC84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMusic  
{
public:
	CMusic();
	virtual ~CMusic();

	void Play();
	void Pause();
	void Stop();
	void AbortStop();
	BOOL OpenFile(LPCTSTR);
	BOOL CreateFilterGraph( void );
	void DeleteContents( void );

    HANDLE GetGraphEventHandle( ) { return m_hGraphNotifyEvent; };
    void OnGraphNotify();
    enum {Uninitialized, Stopped, Paused, Playing } m_State;

    IGraphBuilder *m_pGraph;
    
	BOOL CanPlay(){ return m_State==Stopped || m_State==Paused; };
    BOOL CanStop(){ return m_State==Playing || m_State==Paused; };
    BOOL CanPause(){return m_State==Playing || m_State==Stopped; };
	int  GetState() { return m_State; }									// 010915 LTS
    BOOL IsInitialized(){ return m_State!=Uninitialized; };

protected:
    HANDLE m_hGraphNotifyEvent;

};


//전역변수
extern CMusic cMusic;
extern CMusic* pMusic;




#endif // !defined(AFX_MUSIC_H__86E74FCC_F274_11D2_BBB1_00E02917BC84__INCLUDED_)
