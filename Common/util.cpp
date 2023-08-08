//一些调试模块
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "util.h"

int GetFilePath(char *fname, char *path)
{
	*path =0;

	char *p =fname+strlen(fname);
	while(p > fname)
	{
		if(*p =='\\' || *p =='//')
		{
			strncpy(path,  fname, p-fname);
			path[p-fname] =0;
			return 0;
		}
		p--;
	}

	return -1;
}

char * GetErrString(char *msg, DWORD msg_size, DWORD err)
{
	*msg =0;

	if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, 0, msg, msg_size, NULL))
		return NULL;

	return msg;
}

void WriteLog(char *fmt,...)
{   
	va_list args;
	char modname[MAX_PATH];

	char temp[500];
	HANDLE hFile;

	GetModuleFileName(NULL, modname, sizeof(modname));

	SYSTEMTIME t; 
    GetLocalTime(&t); 
	char LogPath[MAX_PATH];

	::GetProfileString("DragonRaja", "LogPath", NULL,LogPath,sizeof(LogPath));
	wsprintf(LogPath,"%s\\%d-%d-%d CustomerDebug.Log",LogPath,t.wYear,t.wMonth,t.wDay);

	if((hFile =CreateFile(LogPath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) <0)
	{	
		wsprintf(LogPath,"C:\\%d-%d-%d CustomerDebug.Log",t.wYear,t.wMonth,t.wDay);
		if((hFile =CreateFile(LogPath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) <0)
		{ return; }

		return;
	}
	
	_llseek((HFILE)hFile, 0, SEEK_END);

	wsprintf(temp, "<LogLv1(%2d:%2d:%2d)> %s: ",t.wHour,t.wMinute,t.wSecond,modname);
	DWORD dw;
	WriteFile(hFile, temp, strlen(temp), &dw, NULL);
	
	va_start(args,fmt);
	vsprintf(temp, fmt, args);
	va_end(args);

	WriteFile(hFile, temp, strlen(temp), &dw, NULL);

	wsprintf(temp, "\r\n");
	WriteFile(hFile, temp, strlen(temp), &dw, NULL);

	_lclose((HFILE)hFile);
	
}