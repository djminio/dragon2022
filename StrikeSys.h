// StrikeSys.h: interface for the CStrikeSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRIKESYS_H__B784D363_88AB_4604_8B25_2BD9369E65FA__INCLUDED_)
#define AFX_STRIKESYS_H__B784D363_88AB_4604_8B25_2BD9369E65FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 물리적 공격 관련 클래스
class CStrikeSys : public CBattle
{
public:
	CStrikeSys();
	virtual ~CStrikeSys();

public:
  virtual int Action(LPCHARACTER pCaster);
  virtual void Effect(MAGICLIST* pEffect);

public:
  void Effect(LPCHARACTER pCaster, LPCHARACTER pTarget);
  void Effect(LPCHARACTER pCaster, LPCHARACTER pTarget, int nSrcX, int nSrcY, int nDstX, int nDstY);
  void EffectThrow(MAGICLIST* pThrow, USEEFFECTLIST* pEffect);
  void EffectHit(LPCHARACTER pCaster, LPCHARACTER pTarget);

protected:
  void Shoot(LPCHARACTER pCaster, LPCHARACTER pTarget);
  void Throw(LPCHARACTER pCaster, LPCHARACTER pTarget);
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_STRIKESYS_H__B784D363_88AB_4604_8B25_2BD9369E65FA__INCLUDED_)
