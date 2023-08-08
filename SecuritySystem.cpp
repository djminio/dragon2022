// SecuritySystem.cpp: implementation of the CSecuritySystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SecuritySystem.h"

#include "resource.h"
#include <atlbase.h>

#include "Common/Common.h"

#define ONE "0x3e28e1, 0xda26, 0x11e6, 0x1d, 0xd0, 0x1, 0xe0, 0x4c, 0x52, 0xf3, 0xe2"
#define MsgBox(msg) MessageBox(NULL, msg, "Dragonraja", MB_OK)

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CSecuritySystem::CSecuritySystem()
{
	m_hSafeWnd = NULL;
}

CSecuritySystem::~CSecuritySystem()
{
	Disconnect();
}

///////////////////////////////////////////////////////////////////////////////
// Public Methdo
///////////////////////////////////////////////////////////////////////////////

int CSecuritySystem::Connect(HWND hWnd)
{
	HANDLE hObject = ::CreateMutex(NULL, FALSE, ONE);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		::CloseHandle(hObject);
		return -1;
	}

	TCHAR szTempPath[_MAX_PATH];
    DWORD dwResult = ::GetTempPath(_MAX_PATH, szTempPath);
    Dbg_Assert(dwResult);
	
    UINT nResult = ::GetTempFileName(szTempPath, _T ("~Dr"), 0, m_szTempName);
    Dbg_Assert(nResult);

	::DeleteFile(m_szTempName);	

	/*
	if (!MakeExeFile(IDR_ETC_DATA1, m_szTempName))
	{	
		MsgBox("Run Error! \nERR.CODE.00-200!");
		return -2;
	}
	*/

	char szTemp[24];
	//±NÀs±ÚHWND¶Ç»¼¨ì¤º¦s¦@¨É¤å¥ó,¥H«K¤Ï¥~±¾Åª¨ú (Let DR-HWND send to memory common-share document,in order to read anti-hacking tool easily.)
	sprintf(szTemp, "%d", hWnd);
	m_GamehWndSM.Init(SM_GAMEHWND, SHARED_LENGTH);
   	m_GamehWndSM.Lock();
   	strcpy((LPSTR)m_GamehWndSM.GetData(), szTemp);
   	m_GamehWndSM.Unlock();
	//<-PowerZ 20040302 Write *.Log to CurrentDirectory
	char LogPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,LogPath);
	strcat(LogPath,"\\CustomerService");
	WriteProfileString("DragonRaja", "LogPath", LogPath);
	//->PowerZ 20040302
	STARTUPINFO SI;
	ZeroMemory(&SI, sizeof(STARTUPINFO));
	PROCESS_INFORMATION  PI;
	ZeroMemory(&PI, sizeof(PROCESS_INFORMATION));
	SI.cb = sizeof(STARTUPINFO);
	char szCmdLine[100] = " D375D21B2";

	if (!::CreateProcess(m_szTempName, szCmdLine, NULL, NULL, FALSE, NULL, NULL, NULL, &SI, &PI))
	{
		::DeleteFile(m_szTempName);
		return -3;
	}

	CloseHandle(PI.hProcess);
	CloseHandle(PI.hThread);

	//::Sleep(1000);

	memset(szTemp,0,24);
	//±q¦@¨É°ÏÅªSafeMemªºHWND////// (read the HWND of SafeMen from common-share area )
    char* pBuffer;
	CSharedMemory m_SafehWndSM;
	m_SafehWndSM.Init(SM_SAFEHWND, SHARED_LENGTH);
   	m_SafehWndSM.Lock();
	pBuffer = (char*)m_SafehWndSM.GetData();
	strcpy(szTemp,pBuffer);
	m_SafehWndSM.Unlock();
	
	if (pBuffer!=NULL) 
	{	
		const int nWindow = atoi(szTemp);
		m_hSafeWnd = (HWND)nWindow;
	}

	if (!::IsWindow(m_hSafeWnd))
	{ 
		return -4;
	}

	return 1;
}

void CSecuritySystem::Disconnect()
{
	::DeleteFile(m_szTempName);		
}

bool CSecuritySystem::CheckFileName(const char* pFileName)
{
	// ÇöÀç ½ÇÇà ÆÄÀÏ¸í ÀüÃ¼ °æ·Î·Î °¡Á®¿À±â
	char szPath[MAX_PATH]; 
	::GetModuleFileName(NULL, szPath, sizeof(szPath));
	// ÆÄÀÏ¸íÀ» ÀÌ¸§°ú È®ÀåÀÚ·Î ºÐ¸®
	char szName[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(szPath, NULL, NULL, szName, szExt);
	// ÆÄÀÏ¸í ºñ±³
	char szFileName[_MAX_FNAME + _MAX_EXT];
	sprintf(szFileName, "%s%s", szName, szExt);
	return (stricmp(szFileName, pFileName) == 0) ? true:false;
}

HWND CSecuritySystem::GetDragonHwnd()
{ 	//< PowerZ-030116 : Check DR process, mainly to check the old version DR	
	HWND hDragon = ::FindWindow("DRAGONRAJA_CLASS", NULL);  
    
	if (hDragon == NULL)
	{
		hDragon = ::FindWindow(NULL, "DragonRaja");
	}

	return hDragon;
}	//> PowerZ-030116

///////////////////////////////////////////////////////////////////////////////
// Protected Methdo
///////////////////////////////////////////////////////////////////////////////

bool CSecuritySystem::MakeExeFile(int idResource, const char* pFileName)
{   
	if (::GetFileAttributes(pFileName) == -1) 
	{  
		//hExe = AfxGetResourceHandle();//application name
		//if (hExe == NULL)  return false; 
		HINSTANCE hExe = NULL;
		HRSRC hRes = ::FindResource(hExe, MAKEINTRESOURCE(idResource), "ETC_DATA"); 

		if (hRes == NULL)
		{
			return false;
		}
		// Load the dialog box into global memory. 
		HMODULE hResLoad = (HMODULE)::LoadResource(hExe, hRes); 
		
		if (hResLoad == NULL)
		{
			return false;
		}
		// Lock the dialog box into global memory. 
		char* lpResLock = (char*)::LockResource(hResLoad);
		
		if (lpResLock == NULL)
		{
			return false;
		}

		DWORD dwLength = ::SizeofResource(hExe, hRes); 
		
		if (dwLength == 0)
		{
			return false;
		}
			
		HANDLE hFile = ::CreateFile(pFileName, 
			                        GENERIC_WRITE, 
									FILE_SHARE_WRITE, 
									NULL, 
									CREATE_NEW,
								    FILE_ATTRIBUTE_TEMPORARY|FILE_ATTRIBUTE_HIDDEN,
									NULL);

		DWORD dwSize = 0;
		BOOL bSucess = ::WriteFile(hFile, lpResLock, dwLength, &dwSize, NULL);
		if (!bSucess)
		{
			return false;
		}

		::CloseHandle(hFile);
		::FreeResource(hRes);
	} 

	return true;
}