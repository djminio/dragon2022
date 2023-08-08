// MergeData.h: interface for the CMergeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MERGEDATA_H__C26F0549_BF98_4633_9902_DE44A113CD4A__INCLUDED_)
#define AFX_MERGEDATA_H__C26F0549_BF98_4633_9902_DE44A113CD4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//typedef list<const char*>		STR_LIST;
typedef list<string>		STR_LIST;
typedef STR_LIST::iterator		STR_ITOR;

//typedef vector<const char*>		STR_VEC;
typedef vector<string>			STR_VEC;
typedef STR_VEC::iterator		STR_IT;

//typedef vector<FILE*>		FILE_VEC;
typedef pair<FILE*, const char*>	FILE_PAIR;
//typedef vector< FILE_PAIR >			FILE_VEC;
typedef vector< FILE* >			FILE_VEC;
typedef FILE_VEC::iterator			FILE_IT;


//typedef map< const char*, FILE*>	FILE_MAP;
//typedef FILE_MAP::iterator			FILE_IT_MAP;

typedef struct HEADER_DATA_
{
	//int				nType;	//data type
	int				iFlag;	//압축이나 암호화 
	//unsigned long	dwSize;	//데이타 사이즈
}H_Data;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileMerge																					//////////

class CFileMerge
{
public:
	CFileMerge(){};
	virtual ~CFileMerge();

//operator override
	CFileMerge& operator=( CFileMerge &rhs);		

//function
	void	Init( STR_VEC& vFile, const char* szExtention);
	//void	Init( STR_LIST& vFile, const char* szType );
	void	Clear();
	//void	SetFileName( STR_LIST& vFile );
	void	SetFileName( STR_VEC& vFile, const char* szExtention );
	void	SetPosition( size_t pos, const int nType);
	FILE*	OpenFile(const char* szType);
	FILE*	OpenFile(const int nNum, const char* szType);
	void	CloseFile();
	void	CloseFile( const int nFileNum);

	unsigned int GetSize() const { return m_vFileName.size(); }
	FILE*	GetFileHandle( const int nPosition );
	const char* GetFileName( const int nPosition );
	//STR_LIST& GetFileList();
	STR_VEC& GetFileList();
	
private:
	FILE_VEC	m_vFileHandle;
	//STR_LIST	m_lFileName;
	STR_VEC		m_vFileName;

	const CFileMerge& operator+=( const CFileMerge &rhs){};
	CFileMerge& operator+=( CFileMerge &rhs){};
};

// CFileMerge																					//////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMergeData																					//////////

class CMergeData
{
public:
	CMergeData();
	//CMergeData(STR_LIST& vSFile, STR_LIST& vTFile, const char *szSType,const char *szTType);
	CMergeData(STR_VEC& vSFile, STR_VEC& vTFile,const char* szSExtention, const char* szTExtention, const char *szSType,const char *szTType);
	virtual ~CMergeData();

	//init
	bool	Init( STR_VEC& vSFile, STR_VEC& vTFile, const char* szSExtention, const char* szTExtention);
	//void	Init( STR_LIST& vSFile, STR_LIST& vTFile, const char *szSType,const char *szTType);
	
	int		WriteTargetFile( const int nFileNum, void *buf, size_t size);
	int		WriteTargetFile( const int nFileNum, FILE* fp, const int nSize );
	bool	MergeFile(const int iFlag=0);
	char*   GetFileBuf(const int nSprite_Num, DWORD& dwSize);			
	//char*   GetFileBuf(const char* szFileName, DWORD& dwSize);			
	FILE*	GetEffectFileHandle(const int nSprite_Num, DWORD& dwSize);
	//FILE*	GetEffectFileHandle(const char* szFileName, DWORD& dwSize);

private:
	CFileMerge m_vTarget;
	CFileMerge m_vSource;
};

#endif // !defined(AFX_MERGEDATA_H__C26F0549_BF98_4633_9902_DE44A113CD4A__INCLUDED_)
