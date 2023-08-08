// 021014 메시지를 출력하는거 
// 퀘스트 메시지를 뿌려주기 위해 사용된다. 

#ifndef _CMESSAGE_H_
#define _CMESSAGE_H_

#include <string>
#include "network7.h"
using namespace std;


class CMessage
{
	enum
	{
		MAX_SIZE			= 128,
		//MAX_MSG_NUM			= 500,		//txt에 들어있는 메시지갯수 // 021128 kyo
		MAX_VARIABLE		= 10,		// 021128 kyo
		/*
		//AREA_EN		= 0,		//내용은 clanguage.h참조
		AREA_KR		= 1,
		AREA_CH		= 2,
		AREA_EN		= 3,
		AREA_JP		= 4,
		AREA_TH		= 5
		*/
	};

public:
	CMessage();
	~CMessage();
	
//variable
	string		m_Msg[MAX_VARIABLE][MAX_MSG_NUM];

	DWORD		WinAreaCode;			//국가 코드 그대로 가져온것 
	int			iArea;
	char *m_szFilePath;
	char *m_szFileName;

//funciton
	bool	Init();			//국가 세팅해서 파일명 지정
	bool	LoadMessage();

	int		CheckArea();	//국가 지정 CLaguage에서 그대로 가져와따.
	char*	GetTextMessage( const int& iType, const int& iNum );	// 021128 kyo
	char*	DividedMsg( char *tmp);

	char*	GetFileName(){ return m_szFileName;};
	char*	GetFilePath(){ return m_szFilePath;};
	//BOOL	DelAllMessage();
		
	
};

extern CMessage g_cMsg;

#endif //_CMESSAGE_H_