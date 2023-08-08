#ifndef __UTIL_H__
#define __UTIL_H__

#define NO_DEBUG		0
#define DEBUG			1
#define AD_DEBUG		2

#define _DEBUG_LOG	DEBUG

void WriteLog(char *fmt,...);
char * GetErrString(char *msg, DWORD msg_size, DWORD err);
int GetFilePath(char *fname, char *path);

#endif
