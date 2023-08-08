// MagicSys.cpp: implementation of the CMagicSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MagicSys.h"

#include "Char.h"
#include "Map.h"
#include "Effect.h"
#include "Menu.h"		// 030402 kyo
using namespace battle;

extern char tool_ID_EFFECT_TEST;
extern bool IsAttackAble(LPCHARACTER pTarget);
///////////////////////////////////////////////////////////////////////////////
// Global Member
///////////////////////////////////////////////////////////////////////////////

static POINT s_aPlace[8][5] =
{
	{{ 0, -3}, {-2, -3}, { 2, -3}, {-4, -3}, { 4, -3}},
	{{-2, -2}, {-4,  0}, { 0, -4}, {-6,  2}, { 2, -6}},
	{{-3,  0}, {-3,  2}, {-3, -2}, {-3,  4}, {-3, -4}},
	{{-2,  2}, { 0,  4}, {-4,  0}, { 2,  6}, {-6, -2}},
	{{ 0,  3}, {-2,  3}, { 2,  3}, {-4,  3}, { 4,  3}},
	{{ 2,  2}, { 4,  0}, { 0,  4}, { 6, -2}, {-2,  6}},
	{{ 3,  0}, { 3,  2}, { 3, -2}, { 3,  4}, { 3, -4}},
	{{ 2, -2}, { 0, -4}, { 4,  0}, {-2, -6}, { 6,  2}}
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CMagicSys::CMagicSys()
{
	
}

CMagicSys::~CMagicSys()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

int CMagicSys::Action(LPCHARACTER pCaster)
{
	return ACTION_NONE;
}

void CMagicSys::Effect(MAGICLIST* pEffect)
{
	LPCHARACTER pCaster = pEffect->lpChar_Own;
	LPCHARACTER pTarget = pEffect->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL)  return;
	
	const int nMagic = pEffect->magicSerial;
	
	if (pEffect->magicSubNum == 8)
	{
		switch (nMagic)
		{
		case VAMPIRE_POISON:   // 뱀파이어의 독
		case FEATHER_OF_CURSE: // 마비의 깃털
		case CURSE_OF_HELL:    // 헬의 저주
			{
				EffectLocal(pEffect, g_ClientTime);
				break;
			}
		case OCTA_LIGHTNING:
		case BLOOD_SPRINKLE:
			{
				break;
			}
		default:
			{
				::InsertMagic(pCaster, pTarget,
					pEffect->next_Effect, 9,
					pEffect->x, pEffect->y,
					pEffect->tarx, pEffect->tary);
				break;
			}
		}
	}
	else
	{
		switch (nMagic)
		{
		case GREAT_THUNDER_BOLT: 
			{ 
				EffectDrop(pEffect, 4, g_ClientTime);
				break;
			}
		case GREAT_METEOR:
			{
				EffectStorm(pEffect, g_ClientTime);
				break;
			}
		case FLYING_SPARK: 
			{ // 프라잉 스파크
				EffectShoot(pEffect, 3);
				break;
			}
		case FIRE_EXPLOSION: 
			{ // 화염폭발     
				EffectMulti(pEffect, 5);
				break;
			}
		case ICE_STORM:
		case FIRE_STORM: 
			{ // 파이어 스톰
				EffectStorm(pEffect, g_ClientTime);
				break;
			}
		case SHADOW_OF_EVIL: 
			{ // 악의 그림자
				EffectSpread(pEffect);
				break;
			}
		case BLITZ_SPEAR: 
			{ // 번개창
				EffectShoot(pEffect, 5);
				break;
			}
		case LIGHTNING_CLOUD: 
			{ // 전격 구름
				EffectRound(pEffect, g_ClientTime);
				break;
			}
		case ICE_BREATH: 
			{ // 얼음 브레쓰
				EffectBreath(pEffect, 60, 0);
				break;
			}
		case LIGHTNING_BREATH: 
			{ // 전격 브레쓰
				EffectBreath(pEffect, 30, 0);
				break;
			}
		case FLAME_PILLAR:
			{ // 화염 기둥
				EffectPillar(pEffect, g_ClientTime);
				break;
			}
		case FLAME_POUR:
			{
				EffectPillar(pEffect, g_ClientTime);
				break;
			}
		case HOLY_RAGE:
			{ 
				EffectDrop(pEffect, 3, g_ClientTime);
				break;
			}
		case FIRE_BREATH:
		case AREA_PARALYSIS:
		case AREA_POISON:
		case AREA_CONFUSION:
		case AREA_SLOW:
		case AREA_BAT_ATTACK:
			{
				break;
			}
		case DEATH_FIRE:	// 030123 kyo
			{
				EffectMulti(pEffect, 5);
				break;
			}			
		case AUTHORITY_OF_WHOARENCHA: 	// 030415 kyo
			{
				EffectSpreadIrregular(pEffect, g_ClientTime, 5, 7, 2 );	//5타일간격 2번 
				break;
			}
		case ICE_BREATH2: 	// 030415 kyo
			{
				EffectPillar(pEffect, g_ClientTime, 50, true); // 030519 kyo
				break;
			}
		case BLOOD_LIGHTNING:	// 030415 kyo
			{
				EffectMulti(pEffect,3);
				break;
			}
		case FLAME_ARROW:
			{	//< CSD-031020
				EffectMulti(pEffect, 5);
				break;
			}	//> CSD-031020
		default:
			{
				::InsertMagic(pCaster, pTarget,
					pEffect->next_Effect,
					pEffect->magicSubNum,
					pEffect->x, pEffect->y,
					pEffect->tarx, pEffect->tary, 
					false, 0, false, 
					pEffect->magic_Runtime);
				break;
			}
		}
	}
}

bool CMagicSys::Ready(LPCHARACTER pCaster, int nMagic, int nReady)
{	
	switch (pCaster->nCurrentAction)
	{
	case ACTION_ATTACK_BOW:
	case ACTION_THROW:
	case ACTION_SITDOWN:
		{
			return false;
		}
	}
	// 보여지는 마법이 없고 마법 캐스팅이 가능한 경우
	if (g_lpML == NULL || pCaster->ready_Magic == 0)
	{
		if (Hero == pCaster)
		{
			OUTPUT(MK_NORMAL, lan->OutputMessage(1, 14), g_MagicName[nMagic]);
		}
		
		pCaster->excute_MagicNum = nMagic;  		      // 준비중인 마법의 고유번호를 셋팅
		pCaster->ready_Magic = 1;					            // 캐스팅 중인 상태 셋팅
		pCaster->start_Casting  = g_ClientTime + nReady + 3000;
		// 보조계열과 보호계열의 마법사용 후 일정시간 후 전투스킬 사용 불가로 설정
		if (IsContinue(nMagic))
		{
			if (pCaster->continue_RunMagic < g_ClientTime + g_lpMT[nMagic].continue_Time*1000)
			{ 
				pCaster->continue_RunMagic = g_ClientTime + g_lpMT[nMagic].continue_Time*1000;
			} 
		}
		
		pCaster->end_Casting = g_ClientTime + nReady;	// 캐스팅 완료시간 셋팅
		// 캐스팅 동작
		switch (pCaster->spell)
		{
		case WIZARD_SPELL: 
			{	// 마법사인 경우
				::InsertMagic(pCaster, pCaster, 403, 0, 0, 0, 0, 0); 
				break; 
			}
		case PRIEST_SPELL:	
			{	// 성직자인 경우
				::InsertMagic(pCaster, pCaster, 404, 0, 0, 0, 0, 0);
				break;	
			}
		}
		
		return true;
	}	
	
	if (Hero == pCaster)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 13));
	}
	
	return false;
}	

bool CMagicSys::Apply(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic, int nX, int nY, bool bFail)
{
	if (nMagic != RELIEF_AUTHORITY)	// 구원의 빛이 아니고
	{
		if (!IsAttackAble(pTarget))	// 죽어 있거나 기자 모드일 경우 
		{		
			goto MAGIC_FIAL;			// 마법 실패
		}
	}
	
	if (pTarget == NULL) pTarget = pCaster;
	// 캐스팅중 1회 가능 마법인데 복수번 사용코자 할때 blocking
	if (g_lpMT[nMagic].excute_Type == 1 && pCaster->ready_Magic == 3)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 15));
		goto MAGIC_FIAL;
	}
	
	if (Hero == pCaster)
	{	//시전 클라이턴트의 잔여 MP가 마법을 사용하는데 필요한 MP보다 많고 죽지 않은 경우 
		if (SCharacterData.nCharacterHP > 0 && SCharacterData.nCharacterMP >= g_lpMT[nMagic].exhaust_MP)
		{
			SCharacterData.nCharacterMP -= g_lpMT[nMagic].exhaust_MP;
			if (SCharacterData.nCharacterMP < 0)  SCharacterData.nCharacterMP = 0;
			pCaster->mp -= g_lpMT[nMagic].exhaust_MP;
			if (pCaster->mp < 0) Hero->mp = 0;
		}
		else 
		{
			switch (SCharacterData.nCharacterData[SPELL])
			{
			case WIZARD_SPELL:
				{	// 마법사라면
					OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 9));
					break;
				}
			case PRIEST_SPELL:
				{	// 성직자라면
					OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 10));
					break;
				}
			}
			
			return false;
		}
	}
	// 시전자가 캐스팅이 끝나고 마법 발사 준비되어 있을시 또는 맵 테스트를 위한 메뉴선택시		
	if ((g_lpMT[nMagic].excute_Type == 0 && pCaster->ready_Magic == 3) || 
		pCaster->ready_Magic == 2 || tool_ID_EFFECT_TEST == 1)
	{
		DIRECTION dir = pCaster->direction;
		
		if ((g_lpMT[nMagic].appoint_Type == 1 && pCaster == pTarget) || g_lpMT[nMagic].appoint_Type == 0)
		{
			dir = pCaster->direction;
		}
		else
		{
			::ChangeDirection(&dir, pCaster->x, pCaster->y, nX, nY); // 해당 클릭지점으로 방향 전환
		}
		
		switch (pCaster->sprno)
		{ 
		case 83: // 뱀파이어
		case 86: // 헬메이드
			{
				::CharDoAni(pCaster, dir , MON1_BATTLE_MAGIC);
				break;
			}
		default:
			{
				::CharDoAni(pCaster, dir , pCaster->nCurrentAction);
				break;
			}
		}
		
		if (!AddEffect(pCaster, pTarget, nMagic, nX, nY, bFail))
		{
			OUTPUT(MK_WARNING, lan->OutputMessage(1, 29));	
			goto MAGIC_FIAL;
		}
	}
	
	if (Hero == pCaster || Hero == pTarget)
	{
		if (bFail) 
		{	// 마법이 실패한 경우라면
			OUTPUT(MK_NORMAL, lan->OutputMessage(1, 30), g_MagicName[nMagic]);	
			goto MAGIC_FIAL;
		}
		
		switch (nMagic)
		{		
		case LOW_MONSTER_RECALL:
			{
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 31), pCaster->name);
				break;	
			}
		case MIDDLE_MONSTER_RECALL:
			{
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 32), pCaster->name);
				break;
			}
		default:
			{
			/*	if (pCaster == pTarget)
				{
					OUTPUT(MK_NORMAL, lan->OutputMessage(1, 33), pCaster->name, g_MagicName[nMagic]);
				}
				else
				{
					OUTPUT(MK_NORMAL, lan->OutputMessage(1, 34), pCaster->name, g_MagicName[nMagic], pTarget->name);
				}
			*/	
				OUTPUT(MK_NORMAL, lan->OutputMessage(1, 34), pCaster->name, g_MagicName[nMagic], pTarget->name);
				break;		
			}
		}
	}
	// 마법 준비상태에서 마법실제구현 단계로 전이 셋팅
	if (pCaster->ready_Magic == 2)
	{
		// 유지시간이 존재하는 마법인 경우
		if (g_lpMT[nMagic].continue_Time != 0)	
		{
			pCaster->ready_Magic = 3;	// 마법구현 시작
		}
		// 한번 시전에 한번 사용 가능한 마법인 경우
		if (g_lpMT[nMagic].excute_Type == 1)	
		{
			::EndCastMagic(pCaster);	// 시전자의 마법 준비상태를 초기화
		}
		
		return true;
	}
	
	OUTPUT(MK_NORMAL, lan->OutputMessage(1, 35));  	
	
MAGIC_FIAL:
	::EndCastMagic(pCaster);	// 시전자의 마법 준비상태를 초기화
	return false;
}

bool CMagicSys::IsEnable(int nMagic) const
{
	// 평화모드 상태에서는 마법 사용 불가
	if (main_interface.data[IF_BATTLE_PEACE] == 0)  
	{ // 030402 kyo
		OUTPUT(MK_NORMAL, lan->OutputMessage(1, 41));
		return false;
	}

	// 이전 마법 사용후 5초 경과 후 다시 마법 시전 가능
	if(g_ClientTime < Hero->castStartingTime)		
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 36));
		return false;
	}
	// 캐릭터가 죽어 있는 경우
	if (SCharacterData.nCharacterHP <= 0)	
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 1));
		return false;
	}
	// 캐릭터가 여관에서 자고 있는 경우
	if (SCharacterData.sleep != CSC_AWAKE)
	{
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 2));
		return false;
	}
	// 앉아 있는 상태에는 마법시전 불가
	if (Hero->nCurrentAction == 17)
	{
		return false;
	}
	/*
	// 전투스킬 선택 중인 경우
	if (Hero->bSelect)
	{
    OUTPUT(MK_WARNING, lan->OutputMessage(1, 89));
    return false;
	}
	// 마법시전 중인 경우
	if (Hero->ready_Magic != 0)
	{
	OUTPUT(MK_WARNING, lan->OutputMessage(1, 3));
	return false;
	}
	*/
	if (Hero->ready_Magic_rare != 0)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(0, 81));
		return false;
	}
	// 능력치 검사
	if (Hero_Ability[WIS] < g_lpMT[nMagic].require_WP)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}
	
	if (Hero_Ability[INT_] < g_lpMT[nMagic].require_IT)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;			
	}
	
	if (Hero_Ability[WSPS] < g_lpMT[nMagic].point_WS)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}

	if (Hero->aStepInfo[CLS_STEP] < g_lpMT[nMagic].nDualStep)
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}
	
	// 031110 YGI
	if (Hero->aStepInfo[CLS_STEP] < g_lpMT[nMagic].nDualStep)		
	{// 듀얼단계 	// 030415 kyo
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}

	if(g_lpMT[nMagic].nDualStep > QuickItemAttr[5].attr[0] ) //soto-031110 
	{
		OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 6));	
		return false;
	}

	// 택틱과 마나량 검사
	switch (SCharacterData.nCharacterData[SPELL])
	{
    case WIZARD_SPELL:
		{ // 마법사라면
			if (SCharacterData.SkillId[MAGERY + 88] + 1 < g_lpMT[nMagic].basic_Magery)
			{
				OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 7));
				return false;				
			}
			
			if (SCharacterData.nCharacterMP < g_lpMT[nMagic].exhaust_MP)
			{
				OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 9));
				return false;         				
			}
			
			break;
		}
    case PRIEST_SPELL:
		{ // 성직자라면
			if (SCharacterData.SkillId[ORISON + 88] + 1 < g_lpMT[nMagic].basic_Magery)		
			{
				OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 8));
				return false;				
			}
			
			if (SCharacterData.nCharacterMP < g_lpMT[nMagic].exhaust_MP)
			{
				OUTPUT(MK_SHORTAGE, lan->OutputMessage(1, 10));
				return false; 				
			}
			
			break;
		}
	}
	// 건물 안에 있는 경우 회복마법 이외에는 마법 사용 불가
	if (g_lpMT[nMagic].magic_Type/10 != 1)
	{
		const int nX = (Hero->x>>5);
		const int nY = (Hero->y>>5);
		
		if (TileMap[nX][nY].attr_inside == 1)
		{
			OUTPUT(MK_WARNING, lan->OutputMessage(1, 4));
			return false;
		}
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

bool CMagicSys::AddEffect(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic, int nX, int nY, bool bFail)
{
	const bool bPattern = false;
	
	switch (nMagic)
	{
		//// 마법사
	case LIFE_UP: // 체력 회복
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case PROTECTION: // 보호
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}				
			
			break;
		}
	case SHIELD: // 보호막
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case DECIPHER: // 해독마법
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case HEAL_DISEASE: // 질병 치료
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case GREAT_HEAL: // 대체력회복
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);	//서브넘버 8인 경우 시전자 시동->대상체 중간/종말
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}			
	case SLOW: // 속도저하
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case CONFUSION: // 혼란		
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case INTOXICATION: // 중독
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case STONELY: // 석화
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case ABSOLUTE_PARALYSIS: // 절대마비
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case TOXIC_CLOUD: // 독구름
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case DISEASE_INDUCE: // 질병유발
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case VAMPIRE_TOUCH: // 뱀파이어의 손길
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}	
	case SPIDER_WEB: // 거미줄
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case MAGICAL_TRAP: // 마법트랩
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case ENCHANT_WEAPON: // 인체트 웨폰
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);	//서브넘버 8인 경우 시전자 시동->대상체 중간/종말
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case TRANSPARENCY: // 투명주문
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case NULLITY_MAGIC: // 마법무효화
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, pTarget->x, pTarget->y, bPattern, 0, bFail);	//서브넘버 8인 경우 시전자 시동->대상체 중간/종말
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case MINOR_AMPLIFY: // 소증폭
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
		
    case MAGIC_AMPLIFY: // 마법증폭
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}    
	case FIRE_RESIST:
    case ICE_RESIST:
    case ELECT_RESIST:
		{
			::InsertMagic(pCaster, pTarget, nMagic, -1, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}	
	case FIRE_ARROW: // 불화살
		{
			pCaster->bow_Type = 1;
			break;
		}
	case FIRE_BALL: // 화염구
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}	
	case SCORCHER: // 화염군
		{
			ClearCrash();
			
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case CIRCLE_FLAME: // 방사화염
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case FIRE_WALL: // 화염벽
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case FLAME_WAVE: // 움직이는 화염장벽
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);	//서브넘버 8인 경우 시전자 시동->대상체 중간/종말
			break;
		}		
	case APOCALYPSE: // 지옥의 불
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case ICE_ARROW: // 얼음 화살
		{
			pCaster->bow_Type = 2;
			break;
		}			
	case ICE_BALL: // 얼음구
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}	
	case FREEZE_5TILE_RADIUS: // 반경5타일 얼림
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0,  nX, nY, bPattern, 0, bFail);
			break;
		}
	case ICE_WALL: // 얼음벽
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case CIRCLE_ICE: // 원형 얼음공격
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case FREEZE_9TILE_RADIUS: // 반경 9타일 얼림
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0,  nX, nY, bPattern, 0, bFail);
			break;
		}
	case LIGHT: // 빛
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case LIGHTNING_BOLT:
		{
			::InsertMagic(pCaster, pTarget, nMagic, 99, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case GREAT_THUNDER_BOLT:
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case CHAIN_LIGHTNING: // 유도 전격공격
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case CIRCLE_LIGHTNING: // 원형 전격공격
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case LIGHTNING: // 직선 전격공격
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case TWINKLE_STAR: // 수호성
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case LIGHTNING_5TILE_RADIUS: // 반경5타일 전격공격
		{
			::InsertMagic(pCaster, pTarget, nMagic, 99, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case LOW_MONSTER_RECALL: // 저급 몬스터 소환
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case MIDDLE_MONSTER_RECALL: // 중급 몬스터 소환	
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case PHASING: // 장소이동	
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case MEMORIZING_LOCATION: // 이동장소기억
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case TELEPORT: // 순간이동
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case MULTI_PORT: // 복수차원이동
		{
			::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
	case TOWN_PORTAL: // 차원이동
		{
			::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case MAGIC_DETECT: // 마법탐지
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case REMOVE_CURSE: // 저주해제
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case PROTECT_RANGE_WEAPON: // 장거리무기로부터 보호
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}	
	case LOST_SIGHT: // 시력상실
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case SPEED_UP: // 속도증가
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);	//서브넘버 8인 경우 시전자 시동->대상체 중간/종말
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case DRAGON_SCALE: // 드래곤비늘
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
    case PHEONIX: // 불새
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case METEOR_STRIKE: // 유성	
    case GREAT_METEOR:
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}				
		//// 몬스터
    case DEATH_FIRE: // 데쓰 파이어
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
    case SUMMONING_SKELETON: // 데쓰나이트 소환
		{	//< CSD-TW-030606
			const int nDir = pCaster->direction;
			int nPosX = 0, nPosY = 0;
			
			for (int i = 0; i < g_lpMT[nMagic].nSummonMax; ++i)
			{
				nPosX = pCaster->x + (s_aPlace[nDir][i].x<<5);
				nPosY = pCaster->y - (s_aPlace[nDir][i].y<<5);
				::InsertMagic(pCaster, pTarget, nMagic, 8, nPosX, nPosY, nPosX, nPosY, bPattern, 0, bFail);
			}
			
			break;
		}	//> CSD-TW-030606
	case FLYING_SPARK: // 프라잉 스파크
		{ 
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
    case VAMPIRE_POISON: // 뱀파이어의 독
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case VAMPIRE_DODGE: // 뱀파이어 회피
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case SUMMONING_UNDEAD: // 언데드 소환
		{ 
			const int nDir = pCaster->direction;
			int nPosX = 0, nPosY = 0;
			
			for (int i = 0; i < g_lpMT[nMagic].nSummonMax; ++i)
			{	//< CSD-030314
				nPosX = pCaster->x + (s_aPlace[nDir][i].x<<5);
				nPosY = pCaster->y - (s_aPlace[nDir][i].y<<5);
				::InsertMagic(pCaster, pTarget, nMagic, 8, nPosX, nPosY, nPosX, nPosY, bPattern, 0, bFail);
			}	//> CSD-030314
			
			break;
		}
    case RECALL_FOLLOW: // 부하 소환
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, nX, nY, nX, nY, bPattern, 0, bFail);
			break;
		}
    case FEATHER_OF_CURSE: // 마비의 깃털
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case CURSE_OF_HELL: // 헬의저주
		{
			::InsertMagic(pCaster, pTarget, nMagic, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case FIRE_EXPLOSION: // 화염폭발
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case FIRE_STORM: // 파이어 스톰	
    case ICE_STORM:
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case SHADOW_OF_EVIL: // 악의 그림자
		{ 
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
    case FREEZING: // 주위얼림 
		{ 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0,  nX, nY, bPattern, 0, bFail);
			break;
		}
	case BLITZ_SPEAR: // 번개창
		{ 
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case LIGHTNING_CLOUD: // 전격 구름
		{
			::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
    case FIRE_BREATH: // 화염 브레쓰
		{
			// 020707 YGI
			//::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, -370, 60, bPattern, 0, bFail);
			break;
		}
	case ICE_BREATH: // 얼음 브레쓰
	case LIGHTNING_BREATH: // 전격 브레쓰
		{ //< CSD-021021
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		} //> CSD-021021
	case FLAME_PILLAR: // 화염 기둥
    case FLAME_POUR: // 화염 폭풍
		{ 
			ClearCrash();
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0,  nX, nY, bPattern, 0, bFail);
			break;
		}
    case OCTA_LIGHTNING:
    case BLOOD_SPRINKLE:
		{
			::InsertMagic(pCaster, pCaster, nMagic, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case FLAME_ARROW:
		{	//< CSD-031020
			::InsertMagic(pCaster, pTarget, nMagic, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}	//> CSD-031020
    case AREA_BAT_ATTACK:
    case AREA_PARALYSIS:
    case AREA_POISON:
    case AREA_CONFUSION:
    case AREA_SLOW:
		{ //< CSD-020707
			::InsertMagic(pCaster, pCaster, nMagic, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		} //> CSD-020707
		//// 성직자
	case RESTORE_AUTHORITY:	// 회복의 권능
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, 0, 0, bPattern, 0, bFail);							
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);			
			}
			
			break;
		}
	case PROTECT_DANGER: // 위험으로부터 보호
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case BLESS:	// 신의축복
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case RECOVER_POISON: // 해독의권능
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case CURE_DISEASE:
    case PERFECT_PROTECT:
	case AUTHORITY_LETHY: // CSD-021015
    case CANCEL_DIVINE:
    case RECURRENCE_DIVINE:
    case PRAY_POWER:
    case MIRACLE_RECOVER:
    case GREAT_SHIELD:
    case GREAT_PRAY:
    case AUTHORITY_SIMUNIAN:
    case DIVINE_POWER:
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case HOLY_LIGHT: // 성스러운 빛
	case AUTHORITY_CHARISNUMEN: // CSD-021015
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case ANTHEM:
		{ //< CSD-020706
			::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		} //> CSD-020706
	case RELIEF_AUTHORITY: // 구원의 권능
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case HOLY_FIRE: // 홀리 파이어
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 1, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case INFLUENCE_DIVINE: // 신에의 감화
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case VOICE_SOUL:
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}		
    case HOLY_RAGE:
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case APPRECIATE:
    case CURSE:
    case TURN_UNDEAD:
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			break;
		}
	case AUTHORITY_RECOVER_SIGHT: //시력회복의 권능
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case AUTHORITY_LUCK: // 절대 행운의 권능
		{   
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, 0, 0, bPattern, 0, bFail);
			}
			else	
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, nMagic + 150, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			
			break;
		}
	case AUTHORITY_LOST_SIGHT: // 시력상실의 권능
		{
			::InsertMagic(pCaster, pTarget, nMagic + 150, 8, 0, 0, pTarget->x, pTarget->y, bPattern, 0, bFail);
			break;
		}
	case RAGE_STONE: // 대지의 분노
		{
			::InsertMagic(pCaster, pTarget, 358, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
    case LIFE_DRAIN: // 생명력 탈취
		{
			::InsertMagic(pCaster, pTarget, 365, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}		
	case RAGE_BLAST:	// 화신의 분노
		{
			::InsertMagic(pCaster, pTarget, 364, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case RAGE_LIGHT_BALL: // 성령의 분노
		{
			::InsertMagic(pCaster, pTarget, 366, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}						
		//// 던지는 무기
	case THROW_ARROW:
		{
			pCaster->bow_Type = 0;
			
			if (Hero != pCaster)
			{
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			//적 대상을 클릭하는냐 아니냐에 따라 호출부에서 bPattern=0; 또는 bPattern=1 셋팅
			break;
		}
	case THROW_KNIFE:
		{
			pCaster->bow_Type = 3;
			
			if (Hero != pCaster)
			{
				::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			//적 대상을 클릭하는냐 아니냐에 따라 호출부에서 bPattern=0; 또는 bPattern=1 셋팅
			//::InsertMagic( scr, dcr,magic_Num, 0, 0, 0, nX, nY, bPattern);
			break;	
		}

	case PRAY:	// 030415 kyo
		{//추가 내용 필요
			if(pCaster == pTarget)
				::InsertMagic(pCaster, pTarget, 372, 8, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case HOLY_CURE:	// 030415 kyo
		{
			if (pCaster != pTarget)
			{	// 대상자에게 마법 시행
				::InsertMagic(pCaster, pTarget, 373, 8, pTarget->x, pTarget->y, 0, 0, bPattern, 0, bFail);
			}
			else
			{ // 시전자에게 마법 시행				
				::InsertMagic(pCaster, pTarget, 373, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			}
			break;
		}
	case AUTHORITY_OF_ATHYAS:	// 030415 kyo
		{
			if(pCaster == pTarget) // 030514 kyo
			{
				::InsertMagic(pCaster, pTarget, 389, 0, 0, 0, nX, nY, bPattern, 0, bFail);			
			}
			break;
		}
	case AUTHORITY_OF_WHOARENCHA:	// 030415 kyo
		{		
			::InsertMagic(pCaster, pTarget, 390, 0, 0, 0, nX, nY, bPattern, 0, bFail);			
			break;
		}
	case SNOWSTORM:	// 030415 kyo
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case ICE_BREATH2:	// 030415 kyo
		{
			ClearCrash(); // 030430 kyo 
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case BLOOD_LIGHTNING:	// 030415 kyo
		{
			ClearCrash(); // 030516 kyo
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	case GAZER:	// 030509 kyo
		{
			::InsertMagic(pCaster, pTarget, nMagic, 0, 0, 0, nX, nY, bPattern, 0, bFail);
			break;
		}
	default:
		{
			return false;				
		}
	}												
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CMagicSys::IsContinue(int nMagic) const
{
	switch (g_lpMT[nMagic].magic_Type/10)
	{
    case 2: // 보호 마법인 경우
    case 3: // 보조 마법인 경우
		{
			return true;
		}
	}
	
	return false;
}