// MagicSys.h: interface for the CMagicSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICSYS_H__1277B8A6_FEDA_4A35_ACE9_0317F6A0B9E9__INCLUDED_)
#define AFX_MAGICSYS_H__1277B8A6_FEDA_4A35_ACE9_0317F6A0B9E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Battle.h"
///////////////////////////////////////////////////////////////////////////////
// 마법 관련 클래스
class CMagicSys : public CBattle  
{  
public:
	CMagicSys();
	virtual ~CMagicSys();

public:
  virtual int Action(LPCHARACTER pCaster);
  virtual void Effect(MAGICLIST* pEffect);

public: 
  bool Ready(LPCHARACTER pCaster, int nMagic, int nReady);
  bool Apply(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic, int nX, int nY, bool bFail = false);
  
  bool IsEnable(int nMagic) const;

protected:
  bool AddEffect(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic, int nX, int nY, bool bFail);

private:
  bool IsContinue(int nMagic) const;
};
//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MAGICSYS_H__1277B8A6_FEDA_4A35_ACE9_0317F6A0B9E9__INCLUDED_)
