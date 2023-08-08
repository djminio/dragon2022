// Profiler.h: interface for the CProfiler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILER_H__51BF228B_59F8_4DD4_9E18_4038900CD9DA__INCLUDED_)
#define AFX_PROFILER_H__51BF228B_59F8_4DD4_9E18_4038900CD9DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#pragma pack(push, 4)

typedef struct PROFILER
{
	char		szCounterName[32];
	//__int32		nCounterID:
	//char		*szCounterName;
	LARGE_INTEGER n64CounterStart;
	LARGE_INTEGER n64CounterEnd;
	int			nCounterGroup;
	PROFILER()
	{
		memset( (void*)(&szCounterName), 0, 32);
		n64CounterStart.QuadPart	=0;
		n64CounterEnd.QuadPart		=0;
		nCounterGroup				=-1;
	};

} ProFiler;

#pragma pack(pop,4)
*/
typedef struct PROFILE_TYPE
{
	bool	bParent;
	bool	bCurrent;
	bool	bMax;
	bool	bAvr;
	bool	bMin;
	bool	bRate;
} ProFileShowType;

class CProFile
{
public:
	CProFile();	
	~CProFile();

//function
	void StartCounter();
	void StopCounter();

	DWORD	GetCurrent(){ return m_n64CounterEnd.QuadPart-m_n64CounterStart.QuadPart; };
	DWORD	GetMaximum(){ return m_dwMaximum; };
	DWORD	GetAverage(){ return m_dwAverage; };
	DWORD	GetMinimum(){ return m_dwMinimum; };
	string	GetParentName()	{ return m_szParent; };

	void	SetParentName( string szName ) { m_szParent = szName;};
	void	SetMaxMinTime();		//현재 저장된 값 중에 최고/최저값을 세팅해준다.

//variable
private:
	string	m_szParent;
	bool	m_IsStart;				//카운터가 열린 상태에서 두번 열리지 안토록한다. 
	DWORD	m_dwCounter;			//카운터가 몇번 호출되었는가? 
	DWORD	m_dwAccumulator;		//누적 카운터

	DWORD	m_dwCurrent;
	DWORD	m_dwAverage;
	DWORD	m_dwMaximum;
	DWORD	m_dwMinimum;


//속도를 위해 바로 값을 넣는걸로 한다.
	LARGE_INTEGER m_n64CounterStart;
	LARGE_INTEGER m_n64CounterEnd;		
};

//test용 입니다.

class CProFileMgr  
{
public:
enum FOR_PRO_FILE
{
	DEFAULT_COUNTER_NUM		=	10,
	CLOCK_LOOP				=	10,
	CLOCK_TIME				=	1000,

	SHOW_PROFILE_NAME		=	1<<0,
	GROUP_EFFECT_SHOW		=	1<<1,
};

public:
	
	typedef map< string, CProFile*>		MAP_PROFILE;
	typedef MAP_PROFILE::iterator	PROFILE_ITOR;
	typedef MAP_PROFILE::value_type	PAIR_PROFILE;

	typedef vector< MAP_PROFILE >		VECTOR_PROFILE;
	typedef VECTOR_PROFILE::iterator	VECTOR_PROFILE_ITOR;

public:
	CProFileMgr();
	CProFileMgr(const int nCounterNum);
	virtual ~CProFileMgr();
//implement
	void	Init();

	bool	AddCounter(string szParent, string szName);
	bool	AddCounter(const char* szParent, const char* szName);

	bool	DeleteCounter(string szName);	
	int		GetCounterID(int nCounterGroup, const char* szCounterName);	

	// Start and Stop a counter.
	void	StartCounter(string szName);
	void	StopCounter(string szName);	

	
	const char*	GetShowingBuffer(const char* szName);		//보여질 화면버퍼를 가져온다.
	void	DrawCounter();				//카운터를 화면에 보여준다.
	void	DrawCounter(ProFileShowType sType); 	

private:
	bool	Create(const int nCounterNum);
	void	Destory();

	DWORD	GetCpuClock();	// 평균 cpu클럭을 계산하기 위해서 sleep(	CLOCK_TIME )를 CLOCK_LOOP번 돌린 평균을 계산한다.
	DWORD	m_dwCpuClock;	// GetCpuClock로 계산된 결과로, sleep(CLOCK_TIME)을 수행할때의 cpu클럭수

	DWORD	GetSpendTime( DWORD dwTime) {	return dwTime / m_dwCpuClock;};
	int		GetCount(){		return m_mProFile.size();};
	
	//show
	//void (*pGetProfileNamefun)(const char* szName);
	const char*	GetParentName(const char* szName) { return m_mProFile[szName]->GetParentName().c_str();};
	DWORD	GetCurrTime(const char* szName) { return m_mProFile[szName]->GetCurrent()/m_dwCpuClock; };
	DWORD	GetMaxTime(const char* szName) { return m_mProFile[szName]->GetMaximum()/m_dwCpuClock; };
	DWORD	GetAverTime(const char* szName) { return m_mProFile[szName]->GetAverage()/m_dwCpuClock; };
	DWORD	GetMinTime(const char* szName) { return m_mProFile[szName]->GetMinimum()/m_dwCpuClock; };
	float	GetRate(const char* szName) { return (float)m_mProFile[szName]->GetCurrent()/m_mProFile[ m_mProFile[szName]->GetParentName() ]->GetCurrent()*100;}; //최적화 필요 string의 중복을 피하자.

	bool	IsExist(const char* szName) const {	return (m_mProFile.end() != m_mProFile.find(szName));};

	MAP_PROFILE	m_mProFile;	
};

extern ProFileShowType g_ProFileType;
extern CProFileMgr g_ProFileMgr;
extern bool g_IsActiveProFiler;
#endif // !defined(AFX_PROFILER_H__51BF228B_59F8_4DD4_9E18_4038900CD9DA__INCLUDED_)
