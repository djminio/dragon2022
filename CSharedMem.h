#ifndef _SHAREDMEMORY_H
#define	_SHAREDMEMORY_H

#define SHARED_LENGTH 128

static char cstrSMName[] = "SharedMemory";
class CSharedMemory
{
protected:
	HANDLE	m_hSharedMemoryFile;
public:
	void *	m_pwData;
protected:
	DWORD	dwMaximumSizeHigh; 
	DWORD	dwMaximumSizeLow;
	DWORD	dwNumberOfBytesToMap;
	BOOL	m_bInit;
	BOOL	m_bAlreadyExist;

   char csMutexName[50];
   HANDLE m_hMutex;

public:
   char csName[50]; 
public:
	void* GetData()
	{
		if( m_bInit )
			return m_pwData;
		else
			return NULL;
	}
	CSharedMemory(){strcpy(csName,cstrSMName);m_bInit = FALSE;m_bAlreadyExist = FALSE;wsprintf(csMutexName,"%sMutex",csName);}
	CSharedMemory(LPCSTR cs){strcpy(csName,cs);m_bInit = FALSE;m_bAlreadyExist = FALSE;wsprintf(csMutexName,"%sMutex",csName);}
	CSharedMemory(int size){strcpy(csName,cstrSMName);m_bAlreadyExist = FALSE;wsprintf(csMutexName,"%sMutex",csName);Init(size);}
	CSharedMemory(LPCSTR cs, int size){strcpy(csName,cs);m_bAlreadyExist = FALSE;wsprintf(csMutexName,"%sMutex",csName); Init(size);}
	BOOL Init(LPCSTR cs, int size){strcpy(csName,cs);m_bAlreadyExist = FALSE;wsprintf(csMutexName,"%sMutex",csName); return Init(size);}
	BOOL Init(int size ,LPCSTR cs){strcpy(csName,cs);m_bAlreadyExist = FALSE;wsprintf(csMutexName,"%sMutex",csName); return Init(size);}
	BOOL Init( int size )
	{
      m_hMutex = CreateMutex(NULL,FALSE,csMutexName);
		dwNumberOfBytesToMap = size;
		m_hSharedMemoryFile = CreateFileMapping((HANDLE)0xFFFFFFFF,
										NULL,
										PAGE_READWRITE,
										0/*dwMaximumSizeHigh*/,
										dwNumberOfBytesToMap/*dwMaximumSizeLow*/,
										csName);
		if(m_hSharedMemoryFile == NULL)
		{
			m_bAlreadyExist = FALSE;
			m_bInit = FALSE;
			return FALSE;
		}
		else
		{
       if(GetLastError() == ERROR_ALREADY_EXISTS)
          m_bAlreadyExist = TRUE;
		}
		m_pwData = MapViewOfFile(m_hSharedMemoryFile,
								FILE_MAP_WRITE,
								0/*dwFileOffsetHigh*/,
								0/*dwFileOffsetLow*/,
								dwNumberOfBytesToMap);
		if(m_pwData == NULL)
		{
			m_bInit = FALSE;
			CloseHandle(m_hSharedMemoryFile);
         return FALSE;
		}
		else
			m_bInit = TRUE;
      return TRUE;
	}
	~CSharedMemory()
	{
		if (m_bInit)
		{
			UnmapViewOfFile(m_pwData);
			CloseHandle(m_hSharedMemoryFile);
		}
	}
public:
   BOOL AlreadyExist()
   {
      return m_bAlreadyExist;
   }
   struct Locker
   {
      Locker(CSharedMemory* sm)
      {
         m_sm = sm;
         m_sm->Lock();
      }
      Locker(CSharedMemory& sm)
      {
         m_sm = &sm;
         m_sm->Lock();
      }
      ~Locker()
      {
         m_sm->Unlock();
      }
      CSharedMemory* m_sm;
   };

   BOOL Lock(DWORD dwMilliSec = INFINITE)
   {
      if( WaitForSingleObject(m_hMutex,dwMilliSec) == WAIT_OBJECT_0)
         return TRUE;
      return FALSE;
   }
   BOOL Unlock(DWORD dwMilliSec = INFINITE)
   {
      return ReleaseMutex(m_hMutex);
   }
   
};
#endif