/*
	021015 kyo
	퀘스트에 관련된 내용을 뿌리기 위해 만든 class
	"번호 : 내용" 의 형식으로 되어있다.
	파일명		: 한국 - >"%s_Msg.txt"
	파일위치	: "./data_b5/%s"
*/


#include "stdafx.h"
#include "network7.h"
#include "CMessage.h"
#include "Hong_Sub.h"

CMessage g_cMsg;

char    LanguageInt_C[6][3]={ "en", "kr", "ch", "en", "jp","th"};

CMessage::CMessage()
{
	m_szFileName = new char[MAX_SIZE];
	m_szFilePath = new char[MAX_SIZE];

	Init();
}

CMessage::~CMessage()
{
	SAFE_DELETE_ARRAY(m_szFileName);
	SAFE_DELETE_ARRAY(m_szFilePath);
}

int CMessage::CheckArea()
{
	int AreaCode=1;	// KR

#ifdef USA_LOCALIZING_
	AreaCode=0;	// EN
#endif
	
#ifdef JAPAN_LOCALIZING_
	AreaCode=4;	// JP
#endif

#if defined TAIWAN_LOCALIZING_  || defined CHINA_LOCALIZING_ || defined HONGKONG_LOCALIZING_
	AreaCode=2;	// TH
#endif

	return AreaCode;
}
bool CMessage::Init()
{
	iArea = CheckArea();
	if( iArea < 0 ) return false; // 030512 kyo

	sprintf( m_szFileName, "%s_Msg.txt", LanguageInt_C[iArea] );
	sprintf( m_szFilePath, "./data/%s", m_szFileName );	//b3가 언제 들어갈지 고칠것

	LoadMessage();
	return true;
}
	
bool CMessage::LoadMessage( )
{// 021128 kyo
	FILE *fp;
	if( !(fp = Fopen( m_szFilePath,"r")) )return false;

	char tmp[1024];

	int i=0, j=-1;
	while( NULL != fgets( tmp, 1024, fp) )
	{
		if( strlen( tmp ) == 1 ) continue; //캐리지문자만 있는것
		if( tmp[0] == '[' )
		{
			string sz(tmp);
			if( sz.find(']') > 1 )
			{
				j++; i=0; continue;
			}
		}

		if( j < 0 || i > MAX_MSG_NUM )
		{
			fclose(fp);
			return false;
		}

		m_Msg[j][i] = DividedMsg( tmp);
		
		i++;
	}

	fclose( fp);
	return true;
}

char* CMessage::DividedMsg( char *tmp)
{
	int iLen = strlen( tmp );
	if( iLen <= 0 ) return "";

	int i=0;
	while( tmp[i] != ':' )
	{
		i++;
		if( i >= iLen ) return "";
	}

	tmp[iLen-1] = '\0';
	return (tmp+i+1);
}

char*	CMessage::GetTextMessage( const int& iType, const int& iNum )
{ // 021128 kyo
	return (char*)(m_Msg[iType][iNum].c_str());
}

