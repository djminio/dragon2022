// SecuritySystem.h: interface for the CSecuritySystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECURITYSYSTEM_H__4930BDC6_69A4_4A2F_BDC4_355FEDCA3AFA__INCLUDED_)
#define AFX_SECURITYSYSTEM_H__4930BDC6_69A4_4A2F_BDC4_355FEDCA3AFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common/CSharedMem.h"

///////////////////////////////////////////////////////////////////////////////
//
class CSecuritySystem  
{
public:
	CSecuritySystem();
	virtual ~CSecuritySystem();

public:
	int Connect(HWND hWnd);
	void Disconnect();

	bool CheckFileName(const char* pFileName);
	HWND GetDragonHwnd(); // PowerZ-030116

public:
	HWND GetSafeWindowHandle() const
	{
		return m_hSafeWnd;
	}

protected:
	bool MakeExeFile(int idResource, const char* pFileName);

private:
	CSharedMemory m_GamehWndSM;
	HWND m_hSafeWnd;
	TCHAR m_szTempName[_MAX_PATH];
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SECURITYSYSTEM_H__4930BDC6_69A4_4A2F_BDC4_355FEDCA3AFA__INCLUDED_)
