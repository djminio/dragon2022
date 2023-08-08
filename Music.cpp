// Music.cpp: implementation of the CMusic class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Music.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//전역변수
CMusic cMusic;
CMusic* pMusic=&cMusic;

CMusic::CMusic()
{

}

CMusic::~CMusic()
{

}

void CMusic::Play()
{
    if( CanPlay() )
	{
        HRESULT	hr;
        IMediaControl *pMC;

        // Obtain the interface to our filter graph
        hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **) &pMC);

        if( SUCCEEDED(hr) )
		{
            // Ask the filter graph to play and release the interface
            // default behaviour is to carry on from where we stopped last
            // time.
            // if you want it to do this, but rewind at the end then
            // define REWIND.
            // Otherwise you probably want to always start from the
            // beginning -> define FROM_START (in OnMediaStop)
		#undef REWIND
		//#define REWIND
		#define FROM_START

		#ifdef REWIND
            IMediaPosition * pMP;
            hr = m_pGraph->QueryInterface(IID_IMediaPosition, (void**) &pMP);
            if (SUCCEEDED(hr)) 
			{
                // start from last position, but rewind if near the
                // end
                REFTIME tCurrent, tLength;
                hr = pMP->get_Duration(&tLength);
                if (SUCCEEDED(hr)) 
				{
                    hr = pMP->get_CurrentPosition(&tCurrent);
                    if (SUCCEEDED(hr)) 
					{
                        // within 1sec of end? (or past end?)
                        if ((tLength - tCurrent) < 1) 
						{
                            pMP->put_CurrentPosition(0);
                        }
                    }
                }
                pMP->Release();
            }
		#endif
            hr = pMC->Run();
            pMC->Release();

            if( SUCCEEDED(hr) )
			{
                m_State=Playing;
                return;
            }
        }
    }
}

void CMusic::Pause()
{
	if( CanPause() ){
        HRESULT	hr;
        IMediaControl *pMC;

        // Obtain the interface to our filter graph
        hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **) &pMC);

        if( SUCCEEDED(hr) ){
            // Ask the filter graph to pause and release the interface
            hr = pMC->Pause();
            pMC->Release();

            if( SUCCEEDED(hr) ){
                m_State = Paused;
                return;
            }
        }
    }
}

// stop the graph without rewinding
void CMusic::AbortStop()
{
	if( CanStop() )
	{
        HRESULT	hr;
        IMediaControl *pMC;

        // Obtain the interface to our filter graph
        hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **) &pMC);

        if( SUCCEEDED(hr) )
		{
            // Ask the filter graph to stop and release the interface
            hr = pMC->Stop();
		#ifdef FROM_START
            // if we want always to play from the beginning
            // then we should seek back to the start here
            // (on app or user stop request, and also after EC_COMPLETE).
            IMediaPosition * pMP;
            hr = m_pGraph->QueryInterface(IID_IMediaPosition, (void**) &pMP);
            if (SUCCEEDED(hr)) {
                pMP->put_CurrentPosition(0);
                pMP->Release();
            }

            // no visible rewind or we will re-show the window!
		#endif

            pMC->Release();

            if( SUCCEEDED(hr) ){
                m_State = Stopped;
                return;
            }
        }
	}
}

// There are two different ways to stop a graph. We can stop and then when we
// are later paused or run continue from the same position. Alternatively the
// graph can be set back to the start of the media when it is stopped to have
// a more CDPLAYER style interface. These are both offered here conditionally
// compiled using the FROM_START definition. The main difference is that in
// the latter case we put the current position to zero while we change states

void CMusic::Stop()
{
    if( CanStop() )
	{
        HRESULT	hr;
        IMediaControl *pMC;

        // Obtain the interface to our filter graph
        hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **) &pMC);
        if( SUCCEEDED(hr) )
		{

		#ifdef FROM_START
            IMediaPosition * pMP;
            OAFilterState state;

            // Ask the filter graph to pause
            hr = pMC->Pause();

            // if we want always to play from the beginning
            // then we should seek back to the start here
            // (on app or user stop request, and also after EC_COMPLETE).
            hr = m_pGraph->QueryInterface(IID_IMediaPosition,
                                          (void**) &pMP);
            if (SUCCEEDED(hr)) {
                pMP->put_CurrentPosition(0);
                pMP->Release();
            }

            // wait for pause to complete
            pMC->GetState(INFINITE, &state);
		#endif
            // now really do the stop
            pMC->Stop();
            pMC->Release();
            m_State = Stopped;
            return;
        }
    }
}

BOOL CMusic::OpenFile(LPCTSTR lpszPathName)
{
    WCHAR	wPath[MAX_PATH];

    DeleteContents();

    if ( !CreateFilterGraph() )
        return FALSE;
    MultiByteToWideChar( CP_ACP, 0, lpszPathName,
                         -1, wPath, MAX_PATH );

    if (FAILED( m_pGraph->RenderFile(wPath, NULL) )) 
        return FALSE;
    m_State = Stopped;
    return TRUE;
}

BOOL CMusic::CreateFilterGraph(void)
{
    HRESULT hr;	// return code

    assert(m_pGraph == NULL);

    hr = CoCreateInstance(CLSID_FilterGraph, 		// get this documents graph object
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IGraphBuilder,
                          (void **) &m_pGraph);
    if (FAILED(hr)){
    	m_pGraph = NULL;
        return FALSE;
    }

    // get hold of the event notification handle so we can wait for
    // completion
    IMediaEvent *pME;
    hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void **) &pME);
    if (FAILED(hr)) {
        DeleteContents();
        return FALSE;
    }

    hr = pME->GetEventHandle((OAEVENT*) &m_hGraphNotifyEvent);

    pME->Release();

    if (FAILED(hr)) {
        DeleteContents();
        return FALSE;
    }

    return TRUE;
}

void CMusic::DeleteContents( void )
{

    if (m_pGraph != NULL) {
        m_pGraph->Release();
        m_pGraph = NULL;
    }


    // this event is owned by the filtergraph and thus is no longer valid
    m_hGraphNotifyEvent = NULL;

    m_State = Uninitialized;
}

extern HWND g_hwndMain;

