// CombatSys.h: interface for the CCombatSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBATSYS_H__45BA25A4_A2CE_4961_86FA_D9BCBE3BA573__INCLUDED_)
#define AFX_COMBATSYS_H__45BA25A4_A2CE_4961_86FA_D9BCBE3BA573__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 전투스킬 관련 클래스
class CCombatSys : public CBattle  
{
public:
	CCombatSys();
	virtual ~CCombatSys();

public:
  virtual int Action(LPCHARACTER pCaster);
  virtual void Effect(MAGICLIST* pEffect);

public:
  bool Ready(LPCHARACTER pCaster, int nCombat, int nReady);
   bool Apply(LPCHARACTER pCaster, LPCHARACTER pTarget, int nCombat, int nX, int nY, bool bFail);
  void Effect(LPCHARACTER pCaster, LPCHARACTER pTarget);
  int GetCastContinue(int nCombat) const;

  bool IsEnable(int nCombat) const;
  
protected:
  bool AddEffect(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic, int nX, int nY, bool bFail);

protected:
  int GetCombatLevel(int nCombat) const
  {
    return SCharacterData.SkillId[nCombat - 100];
  }

  void SetCombatLevel(int nCombat, int nLevel)
  {
    SCharacterData.SkillId[nCombat - 100] = nLevel;
  }
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_COMBATSYS_H__45BA25A4_A2CE_4961_86FA_D9BCBE3BA573__INCLUDED_)
