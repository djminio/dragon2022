// DualMgr.h: interface for the CDualMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUALMGR_H__690DA1C4_817F_408E_BFCA_9547849ECCF7__INCLUDED_)
#define AFX_DUALMGR_H__690DA1C4_817F_408E_BFCA_9547849ECCF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack (push,1)
#include "Network5.h"
#pragma pack (pop)
///////////////////////////////////////////////////////////////////////////////
// 듀얼을 위한 클래스
class CDualMgr  
{
public:
	CDualMgr();
	virtual ~CDualMgr();

public:
  void Load(const char* pFile);
  void Save(const char* pFile);
  
  string GetClassName(int nBefore, int nAfter, int nStep, int nLevel) const;
  string GetDulaExplain(int nBefore, int nAfter) const; 

	void SendDualEnable(LPCHARACTER pMaster, BYTE nPara, BYTE nX, BYTE nY);
  void SendDualChange(LPCHARACTER pMaster, BYTE nNext);
  void SendDualDivide(LPCHARACTER pMaster, BYTE nNext);
  void RecvDualEnable(t_server_dual_enable* pPacket);
  void RecvDualChange(t_server_dual_change* pPacket);
  void RecvDualDivide(t_server_dual_divide* pPacket);

public:
  int GetAbility(BYTE nType);
  
public:
	BYTE GetPara() const { return m_nPara; }
	BYTE GetPosX() const { return m_nPosX; }
	BYTE GetPosY() const { return m_nPosY; }

private:
  struct HERO_NAME
  {
    BYTE nBefore;      // 이전 클래스
    BYTE nAfter;       // 듀얼 클래스
    BYTE nStep;        // 클래스 단계
    BYTE nLimit;       // 가능한 레벨
    string strName;    // 명칭
    string strExplain; // 설명 
  } m_aBank[5][15];
	
	BYTE m_nPara;
	BYTE m_nPosX;
	BYTE m_nPosY;
};

extern CDualMgr g_mgrDual;
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DUALMGR_H__690DA1C4_817F_408E_BFCA_9547849ECCF7__INCLUDED_)
