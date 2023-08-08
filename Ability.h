// Ability.h: interface for the CAbility class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITY_H__893DF137_6634_4AE7_BC35_F2B34294522C__INCLUDED_)
#define AFX_ABILITY_H__893DF137_6634_4AE7_BC35_F2B34294522C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
//
class CAbility
{
public:
	CAbility();

public:
  WORD GetStr() const { return m_wStr; }
  WORD GetDex() const { return m_wDex; }
  WORD GetCon() const { return m_wCon; }
  WORD GetWis() const { return m_wWis; }
  WORD GetInt() const { return m_wInt; }
  WORD GetCha() const { return m_wCha; }
  WORD GetEndu() const { return m_wEndu; }
  WORD GetMovp() const { return m_wMovp; }
  WORD GetMor() const { return m_wMor; }
  WORD GetLuck() const { return m_wLuck; }
  WORD GetWs() const { return m_wWs; }
  WORD GetPs() const { return m_wPs; }
  
  void SetStr(WORD wValue) { m_wStr = wValue; }
  void SetDex(WORD wValue) { m_wDex = wValue; }
  void SetCon(WORD wValue) { m_wCon = wValue; }
  void SetWis(WORD wValue) { m_wWis = wValue; }
  void SetInt(WORD wValue) { m_wInt = wValue; }
  void SetCha(WORD wValue) { m_wCha = wValue; }
  void SetEndu(WORD wValue) { m_wEndu = wValue; }
  void SetMovp(WORD wValue) { m_wMovp = wValue; }
  void SetMor(WORD wValue) { m_wMor = wValue; }
  void SetLuck(WORD wValue) { m_wLuck = wValue; }
  void SetWs(WORD wValue) { m_wWs = wValue; }
  void SetPs(WORD wValue) { m_wPs = wValue; }

private:
  WORD m_wStr;
  WORD m_wDex;
  WORD m_wCon;
  WORD m_wWis;
  WORD m_wInt;
  WORD m_wCha;
  WORD m_wEndu;
  WORD m_wMovp;
  WORD m_wMor;
  WORD m_wLuck;
  WORD m_wWs;
  WORD m_wPs;
};

class CAbilityDivide : public CAbility
{
public:
  CAbilityDivide();

public:
  WORD GetBeforeClass() const { return m_wBefore; }
  WORD GetAfterClass() const { return m_wAfter; }
  
  void SetBeforeClass(WORD wValue) { m_wBefore = wValue; }
  void SetAfterClass(WORD wValue) { m_wAfter = wValue; }
  
private:
  WORD m_wBefore;
  WORD m_wAfter;
};

class CAbilityLimit : public CAbility
{
public:
  CAbilityLimit();

public:
  WORD GetClass() const { return m_wClass; }
  WORD GetStep() const { return m_wStep; }
  WORD GetMaxTactics() const { return m_wMaxTactic; }
  
  void SetClass(WORD wValue) { m_wClass = wValue; }
  void SetStep(WORD wValue) { m_wStep = wValue; }
  void SetMaxTactics(WORD wValue) { m_wMaxTactic = wValue; }

private:
  WORD m_wClass;
  WORD m_wStep;
  WORD m_wMaxTactic;
};

template <class T, int N1, int N2> class TAbilityMgr
{
public:
  TAbilityMgr() { m_cMagic = 1; }

public:
  T* GetCell(WORD wOne, WORD wTwo)
  {
    return &m_aBank[wOne][wTwo];
  }

  bool Load(const char* pFile);
  bool Save(const char* pFile);

protected:
  int GetCheckSum(char* pDst, int nSize);
  void Encode(char* pData, int nSize);
  void Decode(char* pData, int nSize);

private:
  char m_cMagic;
  T    m_aBank[N1][N2];
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

template <class T, int N1, int N2>
bool TAbilityMgr<T, N1, N2>::Load(const char* pFile)
{ // 파일 읽어오기
  const int nSize = sizeof(T);
  ifstream fin(pFile, ios::in|ios::binary);

  for (int i = 0; i < N1; ++i)
  {
    for  (int j = 0; j < N2; ++j)
    {
      int nSum = 0;
      fin.read((char*)(&nSum), sizeof(nSum));
      // 데이타를 읽어와 암호해독
      fin.read((char*)(&m_aBank[i][j]), nSize);
      Decode((char*)(&m_aBank[i][j]), nSize);
      
      if (GetCheckSum((char*)(&m_aBank[i][j]), nSize) != nSum)
      {
        fin.close();
        return false;
      }
    }
  }

  fin.close();
  return true;
}

template <class T, int N1, int N2>
bool TAbilityMgr<T, N1, N2>::Save(const char* pFile)
{ // 파일 저장하기
  const int nSize = sizeof(T);
  ofstream fout(pFile, ios::out|ios::binary);
  T t;
  memset(&t, 0, nSize);

  for (int i = 0; i < N1; ++i)
  {
    for  (int j = 0; j < N2; ++j)
    {
      memcpy(&t, &m_aBank[i][j], nSize);
      const int nSum = GetCheckSum((char*)(&t), nSize);
      fout.write((char*)(&nSum), sizeof(int));
      // 암호화를 하여 데이타 저장
      Encode((char*)(&t), nSize);
      fout.write((char*)(&t), nSize);
    }
  }

  fout.close();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

template <class T, int N1, int N2>
int TAbilityMgr<T, N1, N2>::GetCheckSum(char* pData, int nSize)
{
  int nSum = 0;

  while (nSize--)
  {
    nSum += *pData++;
  }

  return nSum;
}

template <class T, int N1, int N2>
void TAbilityMgr<T, N1, N2>::Encode(char* pData, int nSize)
{
  while (nSize--)
  {
    *pData++ = (*pData + m_cMagic);
  }
}

template <class T, int N1, int N2>
void TAbilityMgr<T, N1, N2>::Decode(char* pData, int nSize)
{
  while (nSize--)
  {
    *pData++ = (*pData - m_cMagic);
  }
}

#define ABILITYLIMIT  TAbilityMgr<CAbilityLimit, 5, 6>  
#define ABILITYDIVIDE TAbilityMgr<CAbilityDivide, 5, 5>
extern ABILITYLIMIT  g_mgrLimit;
extern ABILITYDIVIDE g_mgrDivide;

#endif // !defined(AFX_ABILITY_H__893DF137_6634_4AE7_BC35_F2B34294522C__INCLUDED_)
