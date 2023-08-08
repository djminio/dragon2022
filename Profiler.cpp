// Profiler.cpp: implementation of the CProFileMgrr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Profiler.h"
#include "TextOutMgr.h"

bool g_IsActiveProFiler = false;
ProFileShowType g_ProFileType;
CProFileMgr g_ProFileMgr;
//////////////////////////////////////////////////////////////////////
// Glogal Function
//////////////////////////////////////////////////////////////////////

static inline void GetPerfCounter(LARGE_INTEGER &iCounter)
{
	DWORD dwLow,dwHigh;
	__asm {
		rdtsc
		mov	dwLow, eax
		mov	dwHigh, edx
	}
	iCounter.QuadPart = ( (unsigned __int64)dwHigh << 32 ) | (unsigned __int64)dwLow; 
}

//////////////////////////////////////////////////////////////////////
// CProFile
CProFile::CProFile()
{
	m_n64CounterStart.QuadPart	=0;
	m_n64CounterEnd.QuadPart		=0;
	m_dwCounter		=0;
	m_dwCurrent		=0;
	m_dwAverage		=0;
	m_dwMaximum		=0;
	m_dwMinimum		=999999;
	
	m_IsStart = false;
}

CProFile::~CProFile()
{

}

void CProFile::StartCounter()
{
	::GetPerfCounter(m_n64CounterStart);
//	assert( true != m_IsStart);
	m_IsStart = true;
}

void CProFile::StopCounter()
{
	::GetPerfCounter(m_n64CounterEnd);
	SetMaxMinTime();
	m_IsStart = false;
}

void CProFile::SetMaxMinTime()
{
	DWORD dwCurrent = GetCurrent();
	//m_dwMaximum < dwCurrent ? m_dwMaximum = dwCurrent : 
	if( m_dwMaximum < dwCurrent ) 
	{
		m_dwMaximum = dwCurrent;
	}
	if( !m_dwMinimum && m_dwMinimum > dwCurrent)
	{
		m_dwMinimum = dwCurrent;
	}

	m_dwAccumulator += dwCurrent;
	m_dwCounter++;
	m_dwAverage = m_dwAccumulator/m_dwCounter++;
}

// CProFile
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CProFileMgr  


CProFileMgr::CProFileMgr()
{

}

CProFileMgr::CProFileMgr(const int nCounterNum)
{
//	Create( nCounterNum );
}

CProFileMgr::~CProFileMgr()
{
	Destory();
}

	
bool CProFileMgr::Create(const int nCounterNum)
{
	return true;
}

void CProFileMgr::Destory()
{
	//int nCount = GetCount();
	
	//for_each(m_mProFile.begin(), m_mProFile.end(), DeleteCounter );
	
	for(PROFILE_ITOR itor = m_mProFile.begin();itor != m_mProFile.end();itor++)
	{
		CProFile *pProFile = itor->second;
		if( NULL != pProFile )
		{
			delete pProFile;
		}

		//m_mProFile.erase(itor);
	}
	m_mProFile.clear();
}

bool CProFileMgr::AddCounter(string szParent, string szName)
{
	
	PROFILE_ITOR itor = m_mProFile.find( szName );
	
	if( itor != m_mProFile.end() )
	{
		//(*itor).second
		return false;
	}
	
	CProFile *pProFile =  new CProFile;
	pProFile->SetParentName(szParent);
	m_mProFile.insert( PAIR_PROFILE( szName, pProFile) );
/*
	ProFiler pProFiler;
	//memset( pProFiler->szCounterName, 0, 32 );
	strncpy( pProFiler.szCounterName, szCounterName, strlen( szCounterName ) );
	pProFiler.nCounterGroup = nCounterGroup;
	m_mProFile.insert( PAIR_PROFILE( nCounterID, pProFiler) );
*/
	
	return true;
}

bool CProFileMgr::AddCounter(const char* szParent, const char* szName)
{
	
	PROFILE_ITOR itor = m_mProFile.find( szName );
	
	if( itor != m_mProFile.end() )
	{
		//(*itor).second
		return false;
	}
	
	CProFile *pProFile =  new CProFile;
	pProFile->SetParentName(szParent);
	m_mProFile.insert( PAIR_PROFILE( szName, pProFile) );
	
	return true;
}

int CProFileMgr::GetCounterID(int nCounterGroup, const char* szCounterName)
{
	/*
	for( int i=0;i< m_nMaxCounterNum;i++)
	{
		if( NULL == m_ProFiler[i].szCounterName )
		{
			SetCounter(i, nCounterGroup, szCounterName );
			return i;
		}
	}
	return -1;
	*/
	return 1;
}

bool CProFileMgr::DeleteCounter(string szName)
{
	
	PROFILE_ITOR itor = m_mProFile.find( szName );
	
	if( itor == m_mProFile.end() )
	{
		return false;
	}

	CProFile *pProFile = itor->second;
	if( NULL != pProFile )
	{
		delete pProFile;
	}

	m_mProFile.erase(itor);
	return true;
}

void CProFileMgr::StartCounter(string szName)
{
	m_mProFile[szName]->StartCounter();
}

void CProFileMgr::StopCounter(string szName)
{
	m_mProFile[szName]->StopCounter();
}

void CProFileMgr::Init()
{
	m_mProFile.clear();
	m_dwCpuClock = 1;
	m_dwCpuClock = GetCpuClock() / CLOCK_TIME; //ms로 계산
}

DWORD CProFileMgr::GetCpuClock()
{
	LARGE_INTEGER	StartTime, EndTime;
	DWORD dwClock=0;
	for( int i =0; i< CLOCK_LOOP; i++)
	{
		GetPerfCounter( StartTime );
		Sleep(CLOCK_TIME);
		GetPerfCounter( EndTime );		
		dwClock += (EndTime.QuadPart - StartTime.QuadPart)/CLOCK_TIME;
	}
	return dwClock/CLOCK_LOOP;
}

const char* CProFileMgr::GetShowingBuffer(const char* szName)
{
	if( NULL == szName || !IsExist(szName) )
	{
		return NULL;
	}

	char *buf = new char[0xff];
	sprintf( buf, "%s_%s\n   Curent:%10d\n   Max:%10d\n   Aver:%10d\n   Min:%10d\n    %3.2f %%",
		GetParentName(szName), szName, GetCurrTime(szName), GetMaxTime(szName), GetAverTime(szName), GetMinTime(szName), GetRate(szName) );
	return buf;
}

void CProFileMgr::DrawCounter()
{
	int i =0;
	for(PROFILE_ITOR it = m_mProFile.begin();it != m_mProFile.end(); it++)
	{
		TxtOut.RcTXTOutBNoBG(200+i++*120, 100, 500,1,"%s", GetShowingBuffer((it)->first.c_str()) );		
	}
}

enum {
	PF_TOP = 180,
	PF_LEFT = 100,

	PF_X_SIZE = 100,
	PF_Y_SIZE = 20,
};

void CProFileMgr::DrawCounter(ProFileShowType sType)
{
	if( !g_IsActiveProFiler )	// active상태에서만 보여준다. 
	{
		return;
	}

	int i =0, j=0;
	for(PROFILE_ITOR it = m_mProFile.begin();it != m_mProFile.end(); it++)
	{
		
		if( sType.bParent )
		{
			TxtOut.RcTXTOutBNoBG(PF_TOP+(PF_X_SIZE*i), PF_LEFT+(PF_Y_SIZE*++j), 100,1,"%s", GetParentName((it)->first.c_str()) );		
		}
		TxtOut.RcTXTOutBNoBG(PF_TOP+(PF_X_SIZE*i), PF_LEFT+(PF_Y_SIZE*++j), 100,1,"%s",(it)->first.c_str() );
		if( sType.bCurrent)
		{
			TxtOut.RcTXTOutBNoBG(PF_TOP+(PF_X_SIZE*i), PF_LEFT+(PF_Y_SIZE*++j), 100,1,"%d", GetCurrTime((it)->first.c_str()) );		
		}
		if( sType.bMax )
		{
			TxtOut.RcTXTOutBNoBG(PF_TOP+(PF_X_SIZE*i), PF_LEFT+(PF_Y_SIZE*++j), 100,1,"%d", GetMaxTime((it)->first.c_str()) );		
		}
		if( sType.bAvr )
		{
			TxtOut.RcTXTOutBNoBG(PF_TOP+(PF_X_SIZE*i), PF_LEFT+(PF_Y_SIZE*++j), 100,1,"%d", GetAverTime((it)->first.c_str()) );		
		}
		if( sType.bMin )
		{
			TxtOut.RcTXTOutBNoBG(PF_TOP+(PF_X_SIZE*i), PF_LEFT+(PF_Y_SIZE*++j), 100,1,"%d", GetMinTime((it)->first.c_str()) );		
		}
		if( sType.bRate )
		{
			TxtOut.RcTXTOutBNoBG(PF_TOP+(PF_X_SIZE*i), PF_LEFT+(PF_Y_SIZE*++j), 100,1,"%3.2f%%", GetRate((it)->first.c_str()) );		
		}
		i++;j=0;
	}
}

// CProFileMgr
//////////////////////////////////////////////////////////////////////
