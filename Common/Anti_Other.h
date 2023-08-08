
//���ϵͳ�Ƿ�������Softice,TRW
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

//���ϵͳ�Ƿ�װ��Softice
BOOL IsSofticeInstall()
{ 
	//  '-#1: HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Uninstall\SoftICE
	//'��-#2: HKEY_LOCAL_MACHINE\Software\NuMega\SoftICE
	//'��-#3: HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\App Paths\Loader32.Exe
	//'��-#4: HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\NTice
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


//����Ƿ�Ring3������������,NTϵͳʹ��
/*
BOOL Check_Debugger() 
{ 	
	HANDLE     hProcessSnap = NULL; 
	char Expchar[] ="\\EXPLORER.EXE"; 
	char szBuffer[MAX_PATH] = {0}; 
	char FileName[MAX_PATH] = {0};  
	PROCESSENTRY32 pe32     = {0}; 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //�õ����н��̵��б���� 
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
		if(pe32.th32ProcessID==GetCurrentProcessId() )//�ж��Ƿ����Լ��Ľ��̣� 
		{ 
			HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ParentProcessID); //�򿪸����� 
			if (hProcess)  
			{  
				if (GetModuleFileNameEx(hProcess, NULL, FileName,  MAX_PATH) ) // �õ��������� 
				{  
					GetWindowsDirectory(szBuffer,MAX_PATH); //�õ�ϵͳ����Ŀ¼ 
					strcat(szBuffer,Expchar);            //��ϳ����Ƶ��ִ�D:\Winnt\Explorer.EXE 
					if(strcmpi (FileName,szBuffer))  // �Ƚϵ�ǰ�Ƿ�ΪExplorer.EXE���� 
						return TRUE;   // ������������Explorer.EXE�����ǵ����� 
				}  
				else  
				{  
					return FALSE; // �޷���ý����� 
					
				}  
				CloseHandle (hProcess);  
			}  
			else  
			{  
				return FALSE;//��Ȩ���ʸý���  
			}  
		
		} 	
	} 
	while (Process32Next(hProcessSnap, &pe32)); 
	
	CloseHandle (hProcessSnap); 
	return FALSE;  

} 
*/