// BattleMgr.h: interface for the CBattleMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEMGR_H__7914A30B_1960_4D3A_AEED_E33E9F2B4FFE__INCLUDED_)
#define AFX_BATTLEMGR_H__7914A30B_1960_4D3A_AEED_E33E9F2B4FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack (push,1)
#include "Network5.h"
#pragma pack (pop)

#define MK_NORMAL      0
#define MK_WARNING     1
#define MK_SHORTAGE    2
#define MK_INFORMATION 3
#define OUTPUT g_mgrBattle.Message
///////////////////////////////////////////////////////////////////////////////
//
struct Delete
{
	template<typename T> void operator()(const T* pT) const
	{
		if (pT != NULL)  
		{
			delete pT;
			pT = NULL;
		}
	}
};
//
///////////////////////////////////////////////////////////////////////////////
const int CELL = 5;

struct USEEFFECTLIST;
struct MAGICLIST;

class CBattle;
class CStrikeSys;
class CMagicSys;
class CCombatSys;
//////////////////////////////////////////////////////////////////////////////
// 전투 관련 클래스
class CBattleMgr
{
	typedef vector<CBattle*> VECTOR;
	
	enum 
	{
		ANI_NONE = 0,
		ANI_STROKE = 1,
		ANI_DEATH = 2,
		ANI_STONE = 3,
	};
	
public:
	CBattleMgr();
	virtual ~CBattleMgr();
	
public:
	void Clear();
	void ClearDelay();
	void ClearSwitchDelay();
	
	int Convert(int nKind);
	int Return(int nEffect, LPCHARACTER pCaster);
	int Action(LPCHARACTER pCaster);
	void Display(int nKind);
	void Effect(MAGICLIST* pEffect);
	
	void Message(int nType, const char* pContext, ...);
	void ActAnimation(int nType, CHARACTER* pTarget);
	void ActCasting(int nMagic, CHARACTER* pTarget);
	
	void SendPcAttack(LPCHARACTER pCaster, LPCHARACTER pTarget);
	
	void SendNpcAttack(LPCHARACTER pCaster, LPCHARACTER pTarget);
	void SendNpcThrow(LPCHARACTER pCaster, LPCHARACTER pTarget);
	void SendStrikeAttack(LPCHARACTER pCaster, LPCHARACTER pTarget);
	void SendThrowAttack(LPCHARACTER pCaster, LPCHARACTER pTarget);
	
	void SendNpcMagic(BYTE nMagic, LPCHARACTER pCaster, LPCHARACTER pTarget, int nX, int nY);
	void SendMagicSelect(BYTE nMagic);  // CSD-TW-030606
	void SendMagicCasting(BYTE nMagic); // CSD-TW-030606
	void SendMagicExecute(BYTE nMagic, LPCHARACTER pTarget, int nX, int nY); // CSD-TW-030606
	void SendMagicResult(WORD wdEffect, LPCHARACTER pCaster, WORD idTarget, int nX, int nY); // CSD-031007
	
	void SendCombatClear(); // CSD-021128
	void SendCombatObtain(LPCHARACTER pMaster, BYTE nPara, BYTE nX, BYTE nY, BYTE nPoint);
	void SendCombatRequest(LPCHARACTER pMaster);
	void SendCombatLearn(BYTE nCombat, LPCHARACTER pMaster);
	void SendCombatSelect(BYTE nCombat);
	void SendCombatAttack(BYTE nCombat, LPCHARACTER pTarget, int nX, int nY);
	void SendCombatResult(BYTE nCombat, LPCHARACTER pCaster, LPCHARACTER pTarget, int nX, int nY);
	
	void SendModifyPosition(LPCHARACTER pTarget); // CSD-021107
	void SendTameCommand(LPCHARACTER pTame, LPCHARACTER pTarget, int command = 19);  //Eleval 24/05/09 - Was void SendTameCommand(LPCHARACTER pTame, LPCHARACTER pTarget); before

	void RecvCharInfoTactic(t_char_info_tactic* pPacket); // CSD-TW-030624
	void RecvBattleMessage(t_battle_message* pPacket);
	void RecvBattleDamage(t_battle_damage* pPacket); // CSD-021019
	
	void RecvAttackResult(WORD idCaster, t_attack_result* pPacket);
	void RecvAttackResult(t_attack_result_d* pPacket);
	
	void RecvThrowAttack(t_server_throw_attack* pPacket);
	void RecvThrowResult(WORD idCaster, t_server_throw_result* pPacket);
	void RecvThrowResult(t_throw_result_d* pPacket);

	void RecvEffectResult(t_server_effect_result* pPacket); // CSD-031007
	
	void RecvNpcMagic(t_server_npc_magic* pPacket);
	void RecvMagicSelect(t_server_magic_select* pPacket); // CSD-TW-030606
	void RecvMagicCasting(t_server_magic_casting* pPacket); // CSD-TW-030606
	void RecvMagicExecute(t_server_magic_execute* pPacket); // CSD-TW-030606
	void RecvMagicResult(t_server_magic_result* pPacket); // CSD-TW-030606
	void RecvMagicResultT(t_server_magic_result_t* pPacket); // CSD-TW-030606
	void RecvMagicResultH(t_server_magic_result_h* pPacket); // CSD-TW-030606
	void RecvMagicResultM(t_server_magic_result_m* pPacket); // CSD-TW-030606
	
	void RecvCombatInit(t_combat_init* pPacket);
	void RecvCombatClear(t_server_combat_clear* pPacket); // CSD-TW-030606
	void RecvCombatObtain(t_server_combat_obtain* pPacket);
	void RecvCombatRequest(t_server_combat_request* pPacket);
	void RecvCombatLearn(t_server_combat_learn* pPacket);
	void RecvCombatSelect(t_server_combat_select* pPacket);
	void RecvCombatAttack(t_server_combat_attack* pPacket); 
	void RecvCombatResult(t_server_combat_result* pPacket); 
	void RecvCombatResultH(t_server_combat_result_h* pPacket); 
	void RecvCombatResultT(t_server_combat_result_t* pPacket); 
	
	bool IsThrow() const;
	bool IsThrow(int nEffect, bool bCast) const;
	bool IsAttack(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic) const;
	bool IsSound(LPCHARACTER pCaster) const;
	bool IsDelay(int nOffset) const;
	bool IsLimit(LPCHARACTER pCaster) const;
	bool IsSelect(LPCHARACTER pCaster) const; // CSD-021024
	bool IsPickUp(LPCHARACTER pCaster, LPCHARACTER pTarget) const; // CSD-021128
	bool IsMapAttr(LPCHARACTER pTarget, int nX, int nY) const; // CSD-021128
	bool IsMagic(int nNo) const;
	bool IsCombat(int nNo) const;
	
public:
	CBattle* GetBattle(int nIndex) const
	{
		return m_vtBuffer[nIndex];
	}
	
	void SetDisplay(bool bOn)
	{
		m_bDisplay = bOn;
	}
	
protected:
	int GetIndex(int nBattle) const;
	void SetViewType(LPCHARACTER pTarget);
	void ShowTaticsInfo(int nKind, int nStep, DWORD dwExperience);
	void ShowAnimation(int nType, LPCHARACTER pTarget);
	void ShowNumber(int nLife, LPCHARACTER pTarget);
	void ShowEffectItem(WORD wdEffect, LPCHARACTER pCaster, LPCHARACTER pTarget); // CSD-031001

private:
	bool IsIgnoreBattle() const; // CSD-TW-030618
	bool IsPlaceMagic(BYTE nMagic, int nX, int nY) const; // CSD-040223

protected:
	VECTOR m_vtBuffer;
	
private:
	mutable DWORD m_dwDelay;
	mutable DWORD m_dwSwitchDelay;
	bool          m_bDisplay;
};
//
///////////////////////////////////////////////////////////////////////////////
extern CBattleMgr g_mgrBattle;
extern CStrikeSys* GetStrike();
extern CMagicSys* GetMagic();
extern CCombatSys* GetCombat();
#endif // !defined(AFX_BATTLEMGR_H__7914A30B_1960_4D3A_AEED_E33E9F2B4FFE__INCLUDED_)
