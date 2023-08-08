// MergeData.cpp: implementation of the CMergeData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MergeData.h"


//////////////////////////////////////////////////////////////////////
// CFileMerge

CFileMerge::~CFileMerge()
{
	Clear();
}

CFileMerge& CFileMerge::operator=( CFileMerge &rhs)
{
	if( this == &rhs)
	{
		return *this;
	}
	this->Clear();
	//this->SetFileName( rhs.GetFileList() );
//	this->OpenFile("wrb"); 

	return *this;
}

void CFileMerge::Clear()
{
	if( !m_vFileHandle.empty() )
	{
		CloseFile();
	}
	m_vFileHandle.clear();
}

FILE* CFileMerge::OpenFile(const char* szType)
{
	m_vFileHandle.clear();
	for(STR_IT it=m_vFileName.begin();it!=m_vFileName.end();it++)
	{		
		m_vFileHandle.push_back( fopen( (*it).c_str(), szType ) );
	}	
	return NULL;
}

FILE* CFileMerge::OpenFile(const int nFileNum, const char* szType)
{
	//FILE *fp = fopen( m_vFileName[nFileNum].c_str(), szType );
	//return (m_vFileHandle[nFileNum] = fp);
	return (m_vFileHandle[nFileNum] = fopen( m_vFileName[nFileNum].c_str(), szType ) );
}

void CFileMerge::CloseFile()
{
	for(FILE_IT it=m_vFileHandle.begin();it!=m_vFileHandle.end();it++)
	{
		if((*it))
		{
			fclose( (*it) );
		}
	}
}

void CFileMerge::CloseFile( const int nFileNum)
{
	if( m_vFileHandle[nFileNum] )
	{
		fclose(m_vFileHandle[nFileNum]);
	}
}

//void CFileMerge::SetFileName(STR_LIST& vFile)
void CFileMerge::SetFileName(STR_VEC& vFile, const char* szExtention )
{
	m_vFileName.clear();
	for(STR_IT it = vFile.begin(); it!=vFile.end(); it++) 
	{
		(*it) += szExtention;
		m_vFileName.push_back( (*it) );
	}
}

void CFileMerge::SetPosition( size_t pos, const int nType)
{
	for(FILE_IT it=m_vFileHandle.begin();it!=m_vFileHandle.end();it++)
	{
		fseek( (*it), pos, nType );
	}
}

void CFileMerge::Init( STR_VEC& vFile, const char* szExtention)//, const char* szType )
{	
	SetFileName( vFile, szExtention);
	m_vFileHandle.reserve( vFile.size() );
//	OpenFile( szType);
}

FILE* CFileMerge::GetFileHandle( const int nPosition )
{
	return m_vFileHandle[nPosition];
}

const char* CFileMerge::GetFileName( const int nPosition )
{
	return m_vFileName[nPosition].c_str();
}

//STR_LIST& CFileMerge::GetFileList()
STR_VEC& CFileMerge::GetFileList()
{
	return m_vFileName;
}

// CFileMerge
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CMergeData
CMergeData::CMergeData()
{

}

//CMergeData::CMergeData(STR_LIST& vSFile, STR_LIST& vTFile, const char *szSType,const char *szTType)
CMergeData::CMergeData(STR_VEC& vSFile, STR_VEC& vTFile,const char* szSExtention, const char* szTExtention, const char *szSType,const char *szTType)
{
	Init(vSFile, vTFile, szSExtention, szTExtention);
}

CMergeData::~CMergeData()
{

}

bool CMergeData::Init( STR_VEC& vSFile, STR_VEC& vTFile, const char* szSExtention, const char* szTExtention)
{
	if( vSFile.empty() || vTFile.empty() || vSFile.size() < vTFile.size() )
	{
		return false;
	}

	m_vSource.Init( vSFile, szSExtention);
	m_vTarget.Init( vTFile, szSExtention);

	if( 0 >= m_vSource.GetSize() || 0 >= m_vTarget.GetSize() || m_vSource.GetSize() < m_vTarget.GetSize() )
	{
		return false;
	}
	return true;
}

int CMergeData::WriteTargetFile( const int nFileNum, void *buf, size_t size)
{
	FILE *fp = m_vTarget.GetFileHandle( nFileNum );
	if( NULL == m_vTarget.GetFileHandle( nFileNum ) )
	{
		m_vTarget.OpenFile( nFileNum, "wb");
	}
	return fwrite( buf, sizeof(char), size, m_vTarget.GetFileHandle( nFileNum ) );
}

int CMergeData::WriteTargetFile( const int nFileNum, FILE* fp, const int nSize )
{
	char buf[0xff];	
	int ret=0, nFullSize=0, nRead=nSize;
	while(1)
	{
		if( nRead > sizeof( buf) )
		{
			nRead = sizeof( buf);
		}

		ret = fread( buf, sizeof(char), nRead, fp);
		ret = WriteTargetFile( nFileNum, buf, ret);

		nFullSize += ret;
		if( nFullSize < nSize )
		{
			nRead = nSize - nFullSize;
			continue;
		}
		else
			break;		
	}
	return nFullSize;
}

bool CMergeData::MergeFile( const int iFlag)
{
	FILE *fp;
	if( !(fp = fopen( "tmp.bin", "wb+")))	
	{
		return false;
	}

	const int nSSize = m_vSource.GetSize();
	const int nTSize = m_vTarget.GetSize();
	if( nSSize < 0 || nTSize <0 )
	{
		return false;
	}

	//H_Data auto_ptr<hData> = new H_Data[nSSize];
	//unsigned long *hData = new unsigned long[nSSize];
	//H_Data header;
	//header.iFlag = iFlag;
	
	unsigned long arrSize[700] ={0,};//here~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//DWORD *arrSize = new DWORD[nSSize];
	
	char buf[2048];
	int ret = 0;
	int i;
	for( i=0; i<nSSize;i++)
	{
		if( m_vSource.OpenFile(i, "rb") )
		{
			while(1)
			{		
				if(!(ret = fread(buf, sizeof(char), 2048, m_vSource.GetFileHandle( i ) )))
					break;
				if(!(ret = fwrite(buf, sizeof(char), ret, fp )))
					break;
				//arrSize[i].dwSize += ret;
				arrSize[i] += ret;

			}
			m_vSource.CloseFile(i);
		}
	}
	
	m_vTarget.OpenFile("wb");
	//save header
	//for( i=0;i<nTSize;i++)
	//{
		//WriteTargetFile( i/(nSSize/nTSize), (void*)(&header), sizeof(H_Data) );	
	//}	
	//save size
	for( i=0;i<nSSize;i++)
	{
		WriteTargetFile( i/(nSSize/nTSize), (void*)(&arrSize[i]), sizeof(unsigned long) );	
	}
	
	//save data
	fseek( fp, 0, SEEK_SET);
	for( i=0;i<nSSize;i++)
	{
		WriteTargetFile( (i/(nSSize/nTSize)), fp, arrSize[i] ) ;		
	}
	
	fclose( fp );	
	DeleteFile("tmp.bin");
	m_vTarget.CloseFile();

	return true;
}

char* CMergeData::GetFileBuf(const int nSprite_Num, DWORD& dwDataSize)
{
	const int nFileNum = nSprite_Num /(m_vSource.GetSize()/m_vTarget.GetSize()) ;
	const int nFileSizePosition = nSprite_Num %(m_vSource.GetSize()/m_vTarget.GetSize()) ;;
	FILE *fp = m_vTarget.OpenFile( nFileNum, "rb");
	if( NULL == fp )
	{
		return NULL;
	}

	int ret=0;
	//read header
	//H_Data header;
	//if( !(ret = fread( (void*)(&header), sizeof(char), sizeof(H_Data), fp)) )
	//{
	//	return NULL;
	//}
	//헤더에 대한 처리를 한다. 

	//read size and position
	DWORD dwSize=0, dwPosition=0, dwPrevious=0;
	for( int i=0;i<=nFileSizePosition ;i++)
	{
		dwPrevious = dwSize;
		if( !(ret = fread( (void*)(&dwSize), sizeof(char), sizeof(unsigned long), fp)) )
		{
			break;
		}		
		dwPosition += dwPrevious;
	}
	ret = fseek( fp, 0, SEEK_SET);
	ret = fseek( fp, sizeof(unsigned long)*(m_vSource.GetSize()/m_vTarget.GetSize()), SEEK_SET);
	ret = fseek( fp, dwPosition, SEEK_CUR);

	dwDataSize = dwSize;

	char *buf =  new char[dwSize];	//나중에 해제되어야 하는 메
	fread( buf, sizeof(char), dwSize, fp );
	m_vTarget.CloseFile(nFileNum);
	return buf;
}

FILE* CMergeData::GetEffectFileHandle(const int nSprite_Num, DWORD& dwDataSize)
{
	const int nFileNum = nSprite_Num /(m_vSource.GetSize()/m_vTarget.GetSize()) ;
	const int nFileSizePosition = nSprite_Num %(m_vSource.GetSize()/m_vTarget.GetSize());	

	FILE *fp = m_vTarget.OpenFile( nFileNum, "rb");
	if( NULL == fp )
	{
		return NULL;
	}
	int ret=0;
	//read header
	//H_Data header;
	//if( !(ret = fread( (void*)(&header), sizeof(char), sizeof(H_Data), fp)) )
	//{
	//	return NULL;
	//}

	//헤더에 대한 처리를 한다. 
	//if( header.iFlag == )

	//read size and position
	DWORD dwSize=0, dwPosition=0, dwPrevious=0;
	for( int i=0;i<=nFileSizePosition ;i++)
	{
		dwPrevious = dwSize;
		if( !(ret = fread( (void*)(&dwSize), sizeof(char), sizeof(unsigned long), fp)) )
		{
			break;
		}		
		dwPosition += dwPrevious;
	}
	ret = fseek( fp, 0, SEEK_SET);
	ret = fseek( fp, sizeof(unsigned long)*(m_vSource.GetSize()/m_vTarget.GetSize()), SEEK_SET);
	ret = fseek( fp, dwPosition, SEEK_CUR);

	dwDataSize = dwSize;
	return fp;
}
// CMergeData
//////////////////////////////////////////////////////////////////////



