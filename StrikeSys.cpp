// StrikeSys.cpp: implementation of the CStrikeSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StrikeSys.h"

#include <math.h>
#include "Char.h"
#include "Map.h"
#include "Effect.h"
#include "Hong_Sub.h"
using namespace battle;

//////////////////////////////////////////////////////////////////////
// Global Member
//////////////////////////////////////////////////////////////////////

static int s_aAngle16[16] = 
{
	2, 4, 6, 8, 10, 12, 14, 0, 3, 5, 7, 9, 11, 13, 15, 1
};

static int s_aOffset[8][5] = 
{
	{0,  8, 15, 1, 7},
	{1,  9,  8, 2, 0},
	{2, 10,  9, 3, 1},
	{3, 11, 10, 4, 2},
	{4, 12, 11, 5, 3},
	{5, 13, 12, 6, 4},
	{6, 14, 13, 7, 5},
	{7, 15, 14, 0, 6}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrikeSys::CStrikeSys()
{
	
}

CStrikeSys::~CStrikeSys()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

int CStrikeSys::Action(LPCHARACTER pCaster)
{	//< CSD-030723
	const int nSprNo = pCaster->sprno;
	
	int t = ::GetCharAccessory(pCaster); 
	// LTS HORSERIDER // Flag	// 창의 사정거리로 세팅
	if ((t - 121) >= 0 && (t - 121) < 6)	
	{
		t = 75;
	}
	
	int nResult = ACTION_NONE;

	if (AttackRangeTable[nSprNo][t][5])
	{	
		nResult = (Random(2)) ? AttackRangeTable[nSprNo][t][5]:
		AttackRangeTable[nSprNo][t][4];
	}
	else	
	{		
		nResult = AttackRangeTable[nSprNo][t][4];
	}
	
	return nResult;
}	//> CSD-030723

void CStrikeSys::Effect(MAGICLIST* pEffect)
{
	
}

void CStrikeSys::Effect(LPCHARACTER pCaster, LPCHARACTER pTarget)
{
	switch (pCaster->nCurrentAction)
	{
    case ACTION_ATTACK_BOW: Shoot(pCaster, pTarget); break;
    case ACTION_THROW:      Throw(pCaster, pTarget); break;
	}
}

void CStrikeSys::Effect(LPCHARACTER pCaster, LPCHARACTER pTarget, int nSrcX, int nSrcY, int nDstX, int nDstY)
{
	const double dUnit = 0.392699; // PI를 8등분
	EFFECT_INFO infEffect;
	int nDir = 0, nOffX = 0, nOffY = 0;
	double dAngle = 0.0;
	
	for (int i = 0; i < 5; i++)
	{
		nDir = s_aOffset[pCaster->direction][i];
		nOffX = 100;
		nOffY = 100;
		dAngle = dUnit*s_aAngle16[nDir];
		Coordinates(dAngle, nOffX, nOffY);
		//
		memset(&infEffect, 0, sizeof(EFFECT_INFO));
		infEffect.pCaster = pCaster;
		infEffect.pTarget = pTarget;
		infEffect.nType = 29;
		infEffect.nEffect = 29;
		infEffect.nSub = 0;
		infEffect.nDir = nDir;
		infEffect.nSrcX = nSrcX;
		infEffect.nSrcY = nSrcY;
		infEffect.nDstX = nSrcX + nOffX;
		infEffect.nDstY = nSrcY + nOffY;
		AddEffect(&infEffect);
	}
}

void CStrikeSys::EffectThrow(MAGICLIST* pThrow, USEEFFECTLIST* pEffect)
{	//< CSD-031014
	LPCHARACTER pCaster = pThrow->lpChar_Own;
	LPCHARACTER pTarget = pThrow->lpChar_Target;
	pEffect = pThrow->EffHead;
	const int nThrow = pThrow->magicNum;
	const int nSrcX = 0;
	const int nSrcY = 0;
	const int nDstX = int(((pEffect->x + pEffect->oldincx)/32.)*32.);
	const int nDstY = int(((pEffect->y + pEffect->oldincy)/32.)*32. + 
		pThrow->spr_Height - pEffect->height);
	
	switch (nThrow)
	{
    case THROW_ARROW:
		{	// 화살
			::InsertMagic(pCaster, pTarget, 204, 9, nSrcX, nSrcY, nDstX, nDstY);
			break;
		}
    case THROW_KNIFE:
		{	// 비도
			::InsertMagic(pCaster, pTarget, 210, 9, nSrcX, nSrcY, nDstX, nDstY);
			break;
		}
	case THROW_FIRE_ARROW:
		{
			InsertMagic(pCaster, pTarget, 153, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	case THROW_ICE_KNIFE:
		{
			InsertMagic(pCaster, pTarget, 170, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	case THROW_SNOW_BALL:
		{
			InsertMagic(pCaster, pTarget, 206, 9, 0, 0, pTarget->x, pTarget->y);
			break;    
		}
	case THROW_WATER_BALL:
		{
			InsertMagic(pCaster, pTarget, 662, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	case THROW_BIG_GULP:
		{
			InsertMagic(pCaster, pTarget, 664, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	}
}	//> CSD-031014

void CStrikeSys::EffectHit(LPCHARACTER pCaster, LPCHARACTER pTarget)
{ // 타격효과 연출
	const int nSrcX = 0;
	const int nSrcY = 0;
	const int nDstX = pTarget->x;
	const int nDstY = pTarget->y;
	
	switch (pCaster->sprno)
	{ 
    case 17: // 발록윙 : 얼음효과
		{
			::InsertMagic(pTarget, pTarget, 170, 0, nSrcX, nSrcY, nDstX, nDstY);
			break;
		}
    case 83: // 뱀파이어 : 박쥐효과
		{
			::InsertMagic(pTarget, pTarget, 400, 0, nSrcX, nSrcY, nDstX, nDstY);
			break;
		}
    case 87: // 이프리트 : 화염효과
		{
			::InsertMagic(pTarget, pTarget, 457, 0, nSrcX, nSrcY, nDstX, nDstY);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CStrikeSys::Shoot(LPCHARACTER pCaster, LPCHARACTER pTarget)
{	//< CSD-031014
	bool bPoint = false;
	
	if (pCaster->nAttackedType == SPRITETYPE_ITEM)
	{
		pTarget = pCaster;
		bPoint = true;
	}
	
	const int nHeight = pTarget->AnimationTable[pTarget->nCurrentAction].height[pTarget->nCurrentAction];
	
	if (nHeight > 0 && sqrt(pow((float)pCaster->x - pTarget->x, (float)2) + pow((float)pCaster->y - pTarget->y, (float)2)) < TILE_SIZE*3)
	{
		pCaster->shoot_PointX = 0;
		pCaster->shoot_PointY = 0;
		return;
	}
	// 높이값을 가진 캐릭터를 공격
	int nX = pTarget->x, nY = pTarget->y;
	
	if (nHeight > 0)
	{
		nX = pTarget->x;
		nY = pTarget->y - pTarget->AnimationTable[pTarget->nCurrentAction].height[pTarget->nCurrentAction];
	}
	
	if (bPoint)
	{
		nX = pCaster->shoot_PointX;
		nY = pCaster->shoot_PointY;
	}
	
	switch (pCaster->bow_Type)
	{
	case 0:
		{
			switch (pCaster->accessory[2])
			{
			case 162:
			case 172:
				{
					::InsertMagic(pCaster, pTarget, THROW_FIRE_ARROW, 0, 0, 0, nX, nY);
					break;
				}
			default:
				{
					::InsertMagic(pCaster, pTarget, THROW_ARROW, 0, 0, 0, nX, nY);
					break;
				}
			}

			break;
		}
	case 1:
		{
			if ((pCaster != Hero) || (pCaster==Hero && (SCharacterData.nCharacterMP >= g_lpMT[FIRE_ARROW].exhaust_MP)))
			{
				::InsertMagic(pCaster, pTarget, FIRE_ARROW, 0, 0, 0, nX, nY);
				
				if (pCaster == Hero)
				{	//< CSD-TW-030606
					SCharacterData.nCharacterMP -= g_lpMT[FIRE_ARROW].exhaust_MP;
					Hero->mp -= g_lpMT[FIRE_ARROW].exhaust_MP;
					g_mgrBattle.SendMagicExecute(FIRE_ARROW, pTarget, nX, nY);
				}	//> CSD-TW-030606
			}
			else
			{
				pCaster->bow_Type = 0;
				::InsertMagic(pCaster, pTarget, THROW_ARROW, 0, 0, 0, nX, nY);
			}
			
			break;
		}
	case 2:
		{
			if ((pCaster != Hero) || (pCaster==Hero && (SCharacterData.nCharacterMP >= g_lpMT[ICE_ARROW].exhaust_MP)))
			{
				::InsertMagic(pCaster, pTarget, ICE_ARROW, 0, 0, 0, nX, nY);
				
				if (pCaster == Hero)
				{	//< CSD-TW-030606
					SCharacterData.nCharacterMP -= g_lpMT[ICE_ARROW].exhaust_MP;
					Hero->mp -= g_lpMT[ICE_ARROW].exhaust_MP;
					g_mgrBattle.SendMagicExecute(ICE_ARROW, pTarget, nX, nY);
				}	//> CSD-TW-030606
			}
			else
			{
				pCaster->bow_Type = 0;
				::InsertMagic(pCaster, pTarget, THROW_ARROW, 0, 0, 0, nX, nY);
			}
			
			break;
		}
	}
}	//> CSD-031014

void CStrikeSys::Throw(LPCHARACTER pCaster, LPCHARACTER pTarget)
{	//< CSD-031014
	bool bPoint = false;
	
	if (pCaster->nAttackedType == SPRITETYPE_ITEM)
	{
		pTarget = pCaster;
		bPoint = true;
	}
	
	const int nHeight = pTarget->AnimationTable[pTarget->nCurrentAction].height[pTarget->nCurrentAction];
	
	if (nHeight > 0 && sqrt(pow((float)pCaster->x - pTarget->x, (float)2) + pow((float)pCaster->y - pTarget->y, (float)2)) < TILE_SIZE*3)
	{
		pCaster->shoot_PointX = 0;
		pCaster->shoot_PointY = 0;
		return;
	}
	// 높이값을 가진 캐릭터를 공격
	int nX = pTarget->x, nY = pTarget->y;
	
	if (nHeight > 0)
	{
		nX = pTarget->x;
		nY = pTarget->y - pTarget->AnimationTable[pTarget->nCurrentAction].height[pTarget->nCurrentAction];
	}
	
	if (bPoint)
	{
		nX = pCaster->shoot_PointX;
		nY = pCaster->shoot_PointY;
	}

	int nEffect = THROW_KNIFE;
	// 이벤트를 위한 수정
	switch (pCaster->accessory[2])
	{
	case 120: nEffect = THROW_SNOW_BALL; break;
	case 127: nEffect = THROW_WATER_BALL; break;
	case 128: nEffect = THROW_BIG_GULP; break;
	case 167:
	case 177: nEffect = THROW_ICE_KNIFE; break;
	}

	::InsertMagic(pCaster, pTarget, nEffect, 0, 0, 0, nX, nY);
}	//> CSD-031014