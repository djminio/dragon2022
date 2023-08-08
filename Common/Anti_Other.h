
//检查系统是否运行了Softice,TRW
BOOL IsSofticeLoad()
{   HANDLE hFile;
	hFile = CreateFile( "\\\\.\\SICE", GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( hFile != INVALID_HANDLE_VALUE ) {  CloseHandle(hFile); return true; }
	hFile = CreateFile( "\\\\.\\NTICE", GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( hFile != INVALID_HANDLE_VALUE )  { CloseHandle(hFile);return true;}
	hFile = CreateFile( "\\\\.\\TRW", GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( hFile != INVALID_HANDLE_VALUE )  { CloseHandle(hFile);return true;}
	hFile = CreateFile( "\\\\.\\TRWDEBUG", GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( hFile != INVALID_HANDLE_VALUE )  { CloseHandle(hFile);return true;}
	return false;
}

//检查系统是否安装了Softice
BOOL IsSofticeInstall()
{ 
	//  '-#1: HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Uninstall\SoftICE
	//'　-#2: HKEY_LOCAL_MACHINE\Software\NuMega\SoftICE
	//'　-#3: HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\App Paths\Loader32.Exe
	//'　-#4: HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\NTice
	HKEY hKEY;
	LPCTSTR data_Set="Software\\NuMega\\SoftICE";
	long ret0=(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set, 0, KEY_READ, &hKEY)); 
	if(ret0!=ERROR_SUCCESS) { ::RegCloseKey(hKEY); return false;}
	
	data_Set="Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\SoftICE";
	ret0=(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set, 0, KEY_READ, &hKEY));
	if(ret0!=ERROR_SUCCESS) { ::RegCloseKey(hKEY); return false;}
	
	::RegCloseKey(hKEY);
	return true;
}


//检查是否被Ring3类调试软件调试,NT系统使用
/*
BOOL Check_Debugger() 
{ 	
	HANDLE     hProcessSnap = NULL; 
	char Expchar[] ="\\EXPLORER.EXE"; 
	char szBuffer[MAX_PATH] = {0}; 
	char FileName[MAX_PATH] = {0};  
	PROCESSENTRY32 pe32     = {0}; 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //得到所有进程的列表快照 
	if (hProcessSnap == INVALID_HANDLE_VALUE)   
		return FALSE;           
	
	pe32.dwSize = sizeof(PROCESSENTRY32); 	
	if (!Process32First(hProcessSnap, &pe32))
	{ 
		CloseHandle (hProcessSnap); 
		return FALSE;  
	} 

	do
	{ 
		if(pe32.th32ProcessID==GetCurrentProcessId() )//判断是否是自己的进程？ 
		{ 
			HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ParentProcessID); //打开父进程 
			if (hProcess)  
			{  
				if (GetModuleFileNameEx(hProcess, NULL, FileName,  MAX_PATH) ) // 得到父进程名 
				{  
					GetWindowsDirectory(szBuffer,MAX_PATH); //得到系统所在目录 
					strcat(szBuffer,Expchar);            //组合成类似的字串D:\Winnt\Explorer.EXE 
					if(strcmpi (FileName,szBuffer))  // 比较当前是否为Explorer.EXE进程 
						return TRUE;   // 父进程若不是Explorer.EXE，则是调试器 
				}  
				else  
				{  
					return FALSE; // 无法获得进程名 
					
				}  
				CloseHandle (hProcess);  
			}  
			else  
			{  
				return FALSE;//无权访问该进程  
			}  
		
		} 	
	} 
	while (Process32Next(hProcessSnap, &pe32)); 
	
	CloseHandle (hProcessSnap); 
	return FALSE;  

} 
*/