// 021014 �޽����� ����ϴ°� 
// ����Ʈ �޽����� �ѷ��ֱ� ���� ���ȴ�. 

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
		//MAX_MSG_NUM			= 500,		//txt�� ����ִ� �޽������� // 021128 kyo
		MAX_VARIABLE		= 10,		// 021128 kyo
		/*
		//AREA_EN		= 0,		//������ clanguage.h����
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

	DWORD		WinAreaCode;			//���� �ڵ� �״�� �����°� 
	int			iArea;
	char *m_szFilePath;
	char *m_szFileName;

//funciton
	bool	Init();			//���� �����ؼ� ���ϸ� ����
	bool	LoadMessage();

	int		CheckArea();	//���� ���� CLaguage���� �״�� �����͵�.
	char*	GetTextMessage( const int& iType, const int& iNum );	// 021128 kyo
	char*	DividedMsg( char *tmp);

	char*	GetFileName(){ return m_szFileName;};
	char*	GetFilePath(){ return m_szFilePath;};
	//BOOL	DelAllMessage();
		
	
};

extern CMessage g_cMsg;

#endif //_CMESSAGE_H_