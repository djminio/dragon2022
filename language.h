#pragma once

#include <wtypes.h>


#define MAX_KIND					10			//����
#define MAX_ONE_LINE_CHAR_NUM		4096		//���ٿ� �� �� �ִ� �ִ��� ����
#define MAX_FILE_LENGTH				2048		//txtȭ���� �ִ� ���� path�� �ִ밪
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


	DWORD		m_dwTotalLength;						//DataAddress�� ������ ����.�� ������ ������ �� + m_dwSumOfTotalLine;

	DWORD		m_dwSumOfTotalLine;						//m_dwTotalLineOfKind�� ��
	DWORD		m_dwTotalLineOfKind[MAX_KIND];			//�� �������� ������ �ִ� ������ ��

	DWORD		m_dwSumOfMaxLineNumber;					//m_dwMaxLineNumberOfKind�� ��
	DWORD		m_dwMaxLineNumberOfKind[MAX_KIND];		//�� �������� ������ �ִ� ���� ū ��ȣ�� ����( ��ȣ )

	char*		DataAddress;
	char*		IndexAddress;	

	int			CheckArea();							//� �� ������ üũ�ϴ� �Լ�
	int			LoadingByAreaCode();					//AreaCode�� ���ؼ� �ش��ϴ� ȭ���� �о� ���δ�.

	int			Preloading();							//�󸶸�ŭ�� �޸𸮰� �ʿ����� ����Ѵ�
	int			Loading();								//�޸𸮸� ��� �ش��ϴ� ���� �ؽ�Ʈ�� ���� �ִ´�.

	int			CheckHeader(DWORD &Header,char* data);				//�����ΰ��� �Ǵ�	.���° ��ȣ���� ��ȯ
	int			CheckNumber(DWORD &Number,char* data);				//��ȣ�� ������� Ȯ���Ѵ�.��ȣ ��ȯ
	int			CheckSentence(void * sentence,char* data);			//������ �󸶳� �䰡�� Ȯ���Ѵ�.������ ���� ��ȯ

	int			GetOneLine(void * data,FILE* fp);

public:
	char		ReturnBuffer[MAX_ONE_LINE_CHAR_NUM];
	char*		OutputMessage(int NumOfSentence);	//�ش��ϴ� �޽����� Sentence���� �ְ� ������ Sentence���� �ƹ����� ����.
	char*		OutputMessage(int Kind,int Num);	//�ش��ϴ� �޽����� Sentence���� �ְ� ������ Sentence���� �ƹ����� ����.

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