#ifndef _CHIGAMEC_H_
#define _CHIGAMEC_H_

#define BLOCK_SIZE			160

typedef struct _SessionParameter {
		char	lpszUserName[32];
		char	lpszPassword[32];
		char	lpszIPAddress[32];
		int		nPortNum;
} SessionParameter;

extern DWORD WINAPI WaitForSessionParameter(SessionParameter* lpSP);
extern DWORD WINAPI SendEncryptedData(LPCTSTR strID, LPCTSTR strPwd, SOCKET socket);

#endif // _CHIGAMEC_H_