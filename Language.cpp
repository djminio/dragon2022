#include "stdafx.h"
#include <stdio.h>

#include "Language.h"
#include "hong_sub.h"
/*////////////////////////////////////////////////////////////////////////////////////////////
// 구성 : CLan.cpp,CLan.h 화일.                                                             //
// 목적 : txt화일로 부터 스트링을 읽어서 메모리에 저장. 외부에 제공하는 OutputMessage함수를 //
//        통해서 해당 스트링을 제공.                                                        //
// 구현 : txt화일을 윈도우에 맞게 해당 언어의 txt를 찾아서 로딩. txt화일을 한번 분석하여    //
//        필요한 메모리를 계산, new를 한번만 호출하여 인덱스용 테이블과 스트링을 함께 저장. //
// 특징 : 예전에는 스트링별로 new를 호출하고 인덱스의 생성에도 new를 사용하여 메모리의 단편 //
//        화 및 memory leak을 유발하였으며, 3단 포인터의 사용으로, 유지보수의 어려움 발생.  //
//        현제는 new를 한번만 호출함으로서 memory leak의 방지 및 메모리 단편화를 방지한다.  //
//        All code is made by zoung. zoung@esofnet.com ICQ : 66924157  Written at 2002/4/12 //
*/////////////////////////////////////////////////////////////////////////////////////////////

LanguagePack* lan = NULL;

char    LanguageInt[6][3]={ "en", "kr", "ch", "en", "jp","th"};
char	header[10][20]={ "[melee]","[magic]","[skill]","[notice]","[item]","[etc]","[justmsg]","[sprintf]","[yesorno]","[temp]"};	

int		CLanguage::CheckArea()			//어떤 언어를 쓰는지 체크하는 함수
{
	AreaCode=1;	// KR
	
#ifdef JAPAN_LOCALIZING_
	AreaCode=4;	// JP
#endif

#if defined TAIWAN_LOCALIZING_  || defined CHINA_LOCALIZING_ || defined HONGKONG_LOCALIZING_
	AreaCode=2;	// TH
#endif

#ifdef USA_LOCALIZING_
	AreaCode=0;	// EN
	/*#ifndef MALAYSIA_LOCALIZING_ // 031013 kyo
		#ifndef THAI_LOCALIZING_	// BBD 040114

			const short WinAreaCode=GetUserDefaultLangID();
			switch(WinAreaCode)
			{
			case 0x0404:// Chinese (Taiwan Region) 
			case 0x0804:// Chinese (PRC) 
			case 0x0c04:// Chinese (Hong Kong SAR, PRC) 
			case 0x1004:// Chinese (Singapore) 
			case 0x0411:// Japanese 
				{
					::ExitApplication(EA_LANGUAGE_ERROR);
					return 0;
				}break;
			}

		#endif	// BBD 040114
	#endif*/	// 031013 kyo
#endif

	return(1);
}

int CLanguage::LoadingByAreaCode()	//AreaCode에 의해서 해당하는 화일을 읽어 들인다.
{
	char path[MAX_FILE_LENGTH];
	wsprintf(LoadedFileName,"%s_lan.txt",LanguageInt[AreaCode]);
	wsprintf(path, "./data/%s", LoadedFileName );	
	wsprintf(this->LoadedFileName,"%s",path);

	FILE * fp=Fopen(path,"rt");
	if(fp==NULL)	{	MessageBox(NULL,"Can't Find Language Text","Language Error",NULL);	return -1;}
	fclose(fp);
	return(1);
}

char*	CLanguage::OutputMessage(int NumOfSentence)	//해당하는 메시지를 Sentence에다 넣고 없으면 Sentence에는 아무값도 없다.
{
	int kind=0,number=0;
	kind=NumOfSentence/100000;
	number=NumOfSentence%100000;
	return this->OutputMessage(kind,number);
}
char*	CLanguage::OutputMessage(int Kind,int Num)	//해당하는 메시지를 Sentence에다 넣고 없으면 Sentence에는 아무값도 없다.
{
	if (Kind == 1 && Num == 25)
	{
		int k = 0;
	}

	if( Kind < 0 || Kind >=MAX_KIND )
		return this->szNullString;

	if( Num  <=0 || Num  > this->m_dwMaxLineNumberOfKind[Kind])
		return this->szNullString;

	DWORD MaxNumber = 0;
	if( Kind == 0 )
		MaxNumber = 0;
	else
	{
		for( int i=0;i<Kind;i++)
			MaxNumber += this->m_dwMaxLineNumberOfKind[i];
	}
	DWORD Number = MaxNumber*4 + Num*4;	
	DWORD ReturnAddress = 0;
	memcpy(&ReturnAddress,(this->IndexAddress + Number),4);
	if( ReturnAddress!=NULL)//020501 lsw
	{
		return (char*)ReturnAddress;
	}
	else
	{
		return this->szNullString;
	}
	
	return this->szNullString;
}


CLanguage::CLanguage()
{
	memset(this,0,sizeof( *this ) );
	this->Init();
}

CLanguage::~CLanguage()
{
	SAFE_DELETE(this->IndexAddress);
}

int CLanguage::Init()
{	
	if( this->CheckArea()	< 0 )			return -1;
	if( this->LoadingByAreaCode() < 0 )		return -2;	
	if( this->Preloading()	< 0 )			return -3;
	if( this->Loading()		< 0	)			return -4;
	return 1;
}

int	CLanguage::Preloading()
{
	FILE* fp=NULL;
	fp = Fopen(this->LoadedFileName,"rt");
	if( fp == NULL)
		return -1;

	DWORD Header=0,Number=0;
	
	while(!feof(fp))		//화일의 끝이 나올때 까지 계속 반복
	{
		int i=0;
		char Original_Sentence[MAX_ONE_LINE_CHAR_NUM]={0,},Temp_Sentence[MAX_ONE_LINE_CHAR_NUM]={0,},Sentence[MAX_ONE_LINE_CHAR_NUM]={0,};
		if( GetOneLine(Original_Sentence,fp) <0)
			continue;		
		//여기까지 왔다는 것은 ;이 없는 라인이라는 뜻
		strcpy(Temp_Sentence,Original_Sentence);	//일단 원본을 백업해 둔다

		//일단 해더를 확인한다.		//해더이면 마이너스. 헤더가 아니면 플러스
		if( CheckHeader(Header,Temp_Sentence) < 0 )
			continue;		

		//번호를 확인한다.//번호가 부적절하거나 없으면 마이너스
		if( CheckNumber(Number,Temp_Sentence) < 0 )
			continue;		

		//내용물을 확인한다.//내용물의 내용이 없가나 "가 2개가 아니면 마이너스
		if( CheckSentence(Sentence,Temp_Sentence) < 0 )
			continue;		

		this->m_dwMaxLineNumberOfKind[Header] = 
			this->m_dwMaxLineNumberOfKind[Header] > Number ? this->m_dwMaxLineNumberOfKind[Header] : Number;
		this->m_dwTotalLineOfKind[Header]++;
		this->m_dwSumOfTotalLine++;
		this->m_dwTotalLength = this->m_dwTotalLength + strlen(Sentence) + 1;
	}
	for( int i=0;i<MAX_KIND;i++)
		this->m_dwSumOfMaxLineNumber += this->m_dwMaxLineNumberOfKind[i];

	fclose(fp);
	return 1;
}

// 020620 YGI
void ConvertSpecialWord(char *txt,const char *delTarget,const char AfterConvert)
{
	char *pdest = txt;
	while( pdest = strstr(pdest,delTarget) )
	{
		*pdest = ' '; pdest++;
		*pdest = AfterConvert;
	} 
}


int	CLanguage::Loading()
{
	FILE* fp=NULL;
	fp = Fopen(this->LoadedFileName,"rt");
	if( fp == NULL)
		return -1;

	DWORD Header=0,Number=0;
	DWORD DataPos=0;

	this->IndexAddress = new char [this->m_dwSumOfMaxLineNumber*4 + this->m_dwTotalLength + 4];
	if( IndexAddress == NULL)
		return -1;
	this->DataAddress = this->IndexAddress + this->m_dwSumOfMaxLineNumber*4 + 4;
	memset(this->IndexAddress , 0 ,this->m_dwSumOfMaxLineNumber*4 + this->m_dwTotalLength + 4);
	
	while(!feof(fp))		//화일의 끝이 나올때 까지 계속 반복
	{
		int i=0;
		char Original_Sentence[MAX_ONE_LINE_CHAR_NUM]={0,},Temp_Sentence[MAX_ONE_LINE_CHAR_NUM]={0,},Sentence[MAX_ONE_LINE_CHAR_NUM]={0,};
		if( GetOneLine(Original_Sentence,fp) <0)
			continue;		
		//여기까지 왔다는 것은 ;이 없는 라인이라는 뜻
		strcpy(Temp_Sentence,Original_Sentence);	//일단 원본을 백업해 둔다

		//일단 해더를 확인한다.		//해더이면 마이너스. 헤더가 아니면 플러스
		if( CheckHeader(Header,Temp_Sentence) < 0 )
			continue;

		//번호를 확인한다.//번호가 부적절하거나 없으면 마이너스
		if( CheckNumber(Number,Temp_Sentence) < 0 )
			continue;		

		//내용물을 확인한다.//내용물의 내용이 없가나 "가 2개가 아니면 마이너스
		if( CheckSentence(Sentence,Temp_Sentence) < 0 )
			continue;

		//여기까지 왔다는 것은 제대로 된 메시지라는 것. 따라서 때려 박는다.^^
//		ConvertSpecialWord(Sentence,"%%s",'%s');
//		ConvertSpecialWord(Sentence,"%%s",'%d');
//		ConvertSpecialWord(Sentence,"%%s",'%u');

		ConvertSpecialWord(Sentence,"\\n",'\n');
		ConvertSpecialWord(Sentence,"\\r",'\r');
		ConvertSpecialWord(Sentence,"\\t",'\t');
		DWORD Sentence_Length = strlen(Sentence);
		DWORD BaseAddress = 0;
		
		if( Header > 0 )
			for( int i=0;i<Header;i++)
				BaseAddress += this->m_dwMaxLineNumberOfKind[i];
		else
			BaseAddress = 0;

		strcpy(this->DataAddress + DataPos , Sentence);

		DWORD TempDataAddress = (DWORD)((this->DataAddress + DataPos));
		memcpy(this->IndexAddress + Number*4 + BaseAddress*4 ,&TempDataAddress,4);

		DataPos+=Sentence_Length+1;
	}
	fclose(fp);
	return 0;
}

int CLanguage::GetOneLine(void * data,FILE* fp)
{
	int i=0,ret=1;
	bool loop=true;
	while(loop)
	{
		if( feof(fp) )
			break;
		char chardata = fgetc(fp);
		((char*)(data))[i++] = chardata;
		switch(chardata)
		{
		case 10:		//행이 바뀌면
			loop=false;
			break;
		case 59:		//;이 나오면
			ret=-1;
			break;
		default:
			break;
		}
	}
	return ret;
}


int	CLanguage::CheckHeader(DWORD &Header,char* data)				//종류인가를 판단	.몇번째 번호인지 반환
{
	for(int i=0;i<MAX_KIND;i++)
	{
		if( strlen(header[i]) > strlen(data) )
			continue;
		if( memicmp(header[i],data,strlen(header[i])) )			//만약 일치하는 header가 있으면
			continue;
		Header = i;
		return -1;		//헤더를 찾으면 -를 반환
	}
	return 1;
}
int	CLanguage::CheckNumber(DWORD &Number,char* data)				//번호가 몇번인지 확인한다.번호 반환
{
	char *semi = NULL;
	semi = strchr(data,':');
	if( semi == NULL)		//:가 없으면
		return -1;
	char temp[MAX_ONE_LINE_CHAR_NUM]={0,};
	memcpy(temp,data,( (DWORD)(semi-data) ) );
	Number = atoi(temp);
	return 1;
}
int	CLanguage::CheckSentence(void * sentence,char* data)			//문장이 얼마나 긴가를 확인한다.문장의 길이 반환
{
	DWORD length = strlen(data);
	char *start = NULL,*end=NULL;
	start = strchr(data,'"');
	if( start == NULL)		//"가 없으면
		return -1;

	//최소한 끝에서 2번째 이어야 한다.
	if( start-data+1 >= length )
		return -2;

	end = strchr(start+1,'"');
	if( end == NULL)
		return -3;

	//최소한 끝에서 첫번째 이어야 한다
	if( end-data >= length )
		return -4;

	if( start+1 > end -1 )
		return -5;

	//여기까지 왔다는 것은 "으로 보호되는 문장이 있다는 이야기가 된다. 시작은 start, 끝은 end이다
	memset(sentence,0,end-start+2);
	memcpy(sentence,start+1,end-start-1);
	return 1;
}

void	CLanguage::DisplayAllData()
{
	int count=0;
	int i;
	for( i=0;i<MAX_KIND;i++)
	{
		for( int j=0;j<this->m_dwMaxLineNumberOfKind[i]+1;j++)
		{
			char* data = (char*)this->OutputMessage(i,j);
			if( data[0] != 0 )	printf("[%2d:%6d] %s\n",i,j,data );
		//	if( count++%100 == 0 )
		//		getchar();
		}
	}
	printf("\nTotal Summary\n");
	for( i=0;i<MAX_KIND;i++)
	{		
		printf("Line 1 : Max Line Number : %d , Total Line Number : %d\n",this->m_dwMaxLineNumberOfKind[i],this->m_dwTotalLineOfKind[i]);				
	}
	printf("\nSum of Max Line Number : %d Sum of Total Line : %d , Total Length : %d" , this->m_dwSumOfMaxLineNumber,
		this->m_dwSumOfTotalLine,
		this->m_dwTotalLength); 

}

// 020428 YGI acer
extern int CheckSumError;

bool LoadLanguageText()
{
	lan = new LanguagePack();
	return (lan != NULL);
}
