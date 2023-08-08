#pragma once

#include <wtypes.h>


#define MAX_KIND					10			//종류
#define MAX_ONE_LINE_CHAR_NUM		4096		//한줄에 들어갈 수 있는 최대의 길이
#define MAX_FILE_LENGTH				2048		//txt화일이 있는 곳의 path의 최대값
//#define MAX_SENTENCE_LENGTH			1024

// thai2 YGI
#include "ftwbrk.h"

class CLanguage
{
private:	
	DWORD		WinAreaCode;
	DWORD		AreaCode;

	char		LoadedFileName[MAX_FILE_LENGTH];
	
	char		szNullString[2];


	DWORD		m_dwTotalLength;						//DataAddress가 가지는 길이.각 문장의 길이의 합 + m_dwSumOfTotalLine;

	DWORD		m_dwSumOfTotalLine;						//m_dwTotalLineOfKind의 합
	DWORD		m_dwTotalLineOfKind[MAX_KIND];			//각 종류별로 가지고 있는 라인의 수

	DWORD		m_dwSumOfMaxLineNumber;					//m_dwMaxLineNumberOfKind의 합
	DWORD		m_dwMaxLineNumberOfKind[MAX_KIND];		//각 종류별로 가지고 있는 가장 큰 번호의 라인( 번호 )

	char*		DataAddress;
	char*		IndexAddress;	

	int			CheckArea();							//어떤 언어를 쓰는지 체크하는 함수
	int			LoadingByAreaCode();					//AreaCode에 의해서 해당하는 화일을 읽어 들인다.

	int			Preloading();							//얼마만큼의 메모리가 필요한지 계산한다
	int			Loading();								//메모리를 잡고 해당하는 곳에 텍스트를 집어 넣는다.

	int			CheckHeader(DWORD &Header,char* data);				//종류인가를 판단	.몇번째 번호인지 반환
	int			CheckNumber(DWORD &Number,char* data);				//번호가 몇번인지 확인한다.번호 반환
	int			CheckSentence(void * sentence,char* data);			//문장이 얼마나 긴가를 확인한다.문장의 길이 반환

	int			GetOneLine(void * data,FILE* fp);

public:
	char		ReturnBuffer[MAX_ONE_LINE_CHAR_NUM];
	char*		OutputMessage(int NumOfSentence);	//해당하는 메시지를 Sentence에다 넣고 없으면 Sentence에는 아무값도 없다.
	char*		OutputMessage(int Kind,int Num);	//해당하는 메시지를 Sentence에다 넣고 없으면 Sentence에는 아무값도 없다.

	int			Init();

	void		DisplayAllData();

	CLanguage();
	~CLanguage();
};

typedef CLanguage LanguagePack;

extern LanguagePack* lan;
extern bool LoadLanguageText();

// thai YGI
extern void ThaiBreak( char *msg,BYTE *brk); // CSD-030324
extern ThaiBreakIterator TBreak; // CSD-030324