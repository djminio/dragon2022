// FileMgr.h: interface for the CFileMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMGR_H__8410F295_2C38_46C6_ACE6_EE7F1880660D__INCLUDED_)
#define AFX_FILEMGR_H__8410F295_2C38_46C6_ACE6_EE7F1880660D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// 화일 관련 클래스
template <typename T, int N> class TFileMgr
{
  typedef map<int, T*>           hash;
  typedef map<int, int>          check;
  typedef typename map<int, T*>::iterator index;
 
  union
  {
    struct
    {
      __int16 m_nIndex;
      __int16 m_nMagic;
    };

    DWORD m_dwHandle;
  };

  //020808 YGI  ---------------
  int m_nClassSize;	// sizeof( T )
  int m_nMarkSize;	// 마킹 할 데이타가 100 바이트가 넘으면 100 바이트로 제한한다.
  T *m_pCommon;
  int m_nCommonIndex;		// 같은 인덱스로 들어오면 있는 거 쓰자.
  char *m_pMark;
  //------------------------------

public:
  TFileMgr() { Create(); }
  ~TFileMgr() { Destroy(); }

public:
  bool Load(const char* pFile);
  bool Load(int nIndex, const char* pFile);
  bool LoadFromClass(int nIndex, T *pData, short int check_sum );
  bool Save(const char* pFile);

public:
  void Create()
  {
	  //020808 YGI  ------------------
	  m_pCommon = new T;
	  m_nClassSize = sizeof( T );
	  m_nMarkSize = (m_nClassSize >100)?100:m_nClassSize;
	  m_pMark = new char[m_nMarkSize];
	  m_nCommonIndex = -1;
	for( int i=0; i<m_nMarkSize; i++ )
	{
		m_pMark[i] = (BYTE)( (rand() % 69) + 1 );
	}
	// ------------------------------

    m_dwHandle = 0; 
    m_nError = 0;
    m_pBuffer = new T[N];
    memset(m_pBuffer, 0, m_nClassSize*N);		//020808 YGI 
  }

  void Destroy()
  {
    if (m_pBuffer != NULL)
    {
      delete [] m_pBuffer;
      m_pBuffer = NULL;
    }

    m_mpBank.clear();
	//020808 YGI 
	delete m_pCommon;
	delete [] m_pMark;
  }

  void AddNew(int nKey)
  {
    T* pT = GetBuffer(nKey);

    if (pT != NULL)
    {
      m_mpCheck.insert(check::value_type(nKey, pT->Check()));
	  m_mpBank.insert(hash::value_type(nKey, pT));
    }
  }
  //020808 YGI 
  void AddNew(int nKey, int check_num)
  {
    T* pT = GetBuffer(nKey);

    if (pT != NULL)
    {
      m_mpCheck.insert(check::value_type(nKey, check_num));
	  m_mpBank.insert(hash::value_type(nKey, pT));
    }
  }

  bool IsError() const
  {
    if (m_nError )
    {
      ::closesocket(connections.socket);
      return true;
    }

    return false;
  }

public:
  T* GetBuffer(int nIndex)
  {
    return &m_pBuffer[nIndex];
  }

  //020808 YGI 
  T& operator[](int nIndex)
  {
	  if( m_nCommonIndex != nIndex )
	  {
		  memcpy( m_pCommon, m_mpBank[nIndex], m_nClassSize );
		  char *pSource = (char * )m_pCommon;
		  for( int i=0; i<m_nMarkSize; i++ )
		  {
			  pSource[i] ^= m_pMark[i];
		  }
		  m_nCommonIndex = nIndex;
	  }

    m_nError += abs(m_mpCheck[nIndex] - m_pCommon->Check());
    m_nError <<= 15;
	  m_nError += m_nError?4:0;
    //return m_nError?*(m_mpBank[nIndex + m_nError]):*m_pCommon;
	  if( m_nError )
	  {
		  return *(m_mpBank[nIndex + m_nError]);
	  }
	  return *m_pCommon;
  }

protected:
  void Encode(char* pData, size_t nSize);
  void Decode(char* pData, size_t nSize);

private:
  T*           m_pBuffer;
  check        m_mpCheck;
  hash         m_mpBank;
  int          m_nError;
  CHSEL_STREAM m_hsel;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool TFileMgr<T, N>::Load(const char* pFile)
{
  ifstream fin(pFile, ios::in|ios::binary);
  if( !fin.is_open() ) return false;
  // 헤더 읽어오기
  int nVersion = 0;
  fin.read((char*)(&nVersion), sizeof(nVersion)); 
  if (m_hsel.GetVersion() != nVersion)  return false;
  HselInit deinit;
  fin.read((char*)(&deinit), sizeof(HselInit));
  if (!m_hsel.Initial(deinit))  return false;
  // 파일 읽어오기
  T* pT = NULL;

  for (int i = 0; i < N; ++i)	
  { 
    pT = GetBuffer(i);
    if (pT == NULL)  return false;
    // 데이타를 읽어와 암호해제
    fin.read((char*)(pT), sizeof(T));
    fin.read((char*)(&m_dwHandle), sizeof(m_dwHandle));
    Decode((char*)(pT), sizeof(T));
    // 해킹여부 검사
    if (m_hsel.GetCRCConvertShort() != m_nIndex)  return false;
    if (pT->Check() != m_nMagic)  return false;

	//020808 YGI  --------------------
	char *pSource = (char *)pT;
	int check = pT->Check();
	for( int j=0; j< m_nMarkSize; j++ )
	{
		pSource[j] ^= m_pMark[j];
	}
    // 레코트 추가
    AddNew(i, check);
	//--------------------------------
  }

  fin.close();
  return true;
}

template <typename T, int N>
bool TFileMgr<T, N>::Load(int nIndex, const char* pFile)
{
  ifstream fin(pFile, ios::in|ios::binary);
  T* pT = GetBuffer(nIndex);
  if (pT == NULL)  return false;

  if( fin.is_open() ) 
  {
	  // 헤더 읽어오기
	  int nVersion = 0;
	  fin.read((char*)(&nVersion), sizeof(nVersion)); 
	  if (m_hsel.GetVersion() != nVersion)  return false;
	  HselInit deinit;
	  fin.read((char*)(&deinit), sizeof(HselInit));

	  if (!m_hsel.Initial(deinit))  return false;
	  // 데이타를 읽어와 암호해제
	  fin.read((char*)(pT), sizeof(T));
	  fin.read((char*)(&m_dwHandle), sizeof(m_dwHandle));
	  Decode((char*)(pT), sizeof(T));
	  // 해킹여부 검사
	  if (m_hsel.GetCRCConvertShort() != m_nIndex)  return false;
	  if (pT->Check() != m_nMagic)  return false;
	  fin.close();
  }
  else
  {
	  T temp;
	  memcpy( pT, &temp, sizeof( T ) );
	  m_nMagic = pT->Check();
  }

	//020808 YGI  --------------------
	char *pSource = (char *)pT;
	int check = m_nMagic;
	for( int j=0; j< m_nMarkSize; j++ )
	{
		pSource[j] ^= m_pMark[j];
	}
  // 레코트 추가
  AddNew(nIndex, check);
//--------------------------------
  
  return true;
}

template <typename T, int N>
bool TFileMgr<T, N>::LoadFromClass(int nIndex, T *pData, short int check_sum )
{
	T* pT = GetBuffer(nIndex);
	if (pT == NULL)  return false;
	memcpy( pT, pData, sizeof( T ) );
	m_nMagic = check_sum;
	if (pT->Check() != m_nMagic)  return false;
	char *pSource = (char *)pT;
	for( int j=0; j< m_nMarkSize; j++ )
	{
		pSource[j] ^= m_pMark[j];
	}
	AddNew(nIndex, m_nMagic);
	return true;
}


template <typename T, int N>
bool TFileMgr<T, N>::Save(const char* pFile)
{
  ofstream fout(pFile, ios::out|ios::binary);
  // CRC 구하기
  HselInit eninit;
	eninit.iEncryptType	=	HSEL_ENCRYPTTYPE_RAND;
	eninit.iDesCount = HSEL_DES_TRIPLE;
	eninit.iCustomize	=	HSEL_KEY_TYPE_DEFAULT;
	eninit.iSwapFlag = HSEL_SWAP_FLAG_ON;
	if (!m_hsel.Initial(eninit))  return false;
  // 헤더 저장하기
  HselInit deinit;
  deinit = m_hsel.GetHSELCustomizeOption();
  const int nVersion = m_hsel.GetVersion();
  fout.write((char*)(&nVersion), sizeof(nVersion));
  fout.write((char*)(&deinit), sizeof(HselInit)); 
  // 파일 저장하기
  T* pT = NULL;

  for (int i = 0; i < N; ++i)	
  {
    pT = (m_mpBank[i] != NULL) ? m_mpBank[i]:GetBuffer(i);
    if (pT == NULL)  return false;
    // 데이타 암호화 및 저장
    m_nMagic = pT->Check();
    Encode((char*)pT, sizeof(T));
    fout.write((char*)(pT), sizeof(T));
    m_nIndex = m_hsel.GetCRCConvertShort();
    fout.write((char*)(&m_dwHandle), sizeof(m_dwHandle)); 
  }

  fout.close();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
void TFileMgr<T, N>::Encode(char* pData, size_t nSize)
{
  m_hsel.Encrypt(pData, nSize);
}

template <typename T, int N>
void TFileMgr<T, N>::Decode(char* pData, size_t nSize)
{
  m_hsel.Decrypt(pData, nSize);
}

#endif // !defined(AFX_FILEMGR_H__8410F295_2C38_46C6_ACE6_EE7F1880660D__INCLUDED_)
