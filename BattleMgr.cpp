// BattleMgr.cpp: implementation of the CBattleMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleMgr.h"

#include <algorithm>
#include "MouseCursor.h"
#include "Char.h"
#include "CharDataTable.h"
#include "Map.h"
#include "Effect.h"
#include "TeamBattle.h"
#include "Skill_lsw.h"
#include "gameproc.h"
#include "DirectSound.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "StrikeSys.h"
#include "MagicSys.h"
#include "CombatSys.h"
#include "LineCommand.h"
#include "CharDataTable.h"
#include "NumberAniMgr.h"
using namespace battle;

extern int g_LocalWarBegin;
extern int g_CurrCharStatus;
extern void PrintMagicExplain(int magic_no);
extern BOOL ChangeDirection(DIRECTION* direction, int sx, int sy, int dx, int dy);
extern int AttackAction(LPCHARACTER lpCharacter);
extern void CheckNPCPk(int aid, int did);
extern int AttackRangeTable[2][CHARACTER_ACCESSORY_MAX_][10];
extern int GetLeveByExp(int nStep, DWORD exp);
extern void SkillSettingClear();
extern void SendModifyPositionNPC(int id);
extern int UseItemByRbutton(POS pos_s, ItemAttr &item_attr);

//Attributes added by Eleval
bool g_autoAttack = true; //22/05/09 for /auto attack
bool g_autoDefence = true; //22/05/09 for /auto defence
//End of Them
//New Functions added by Eleval 22/05/09
bool CheckIfRecoveryMagic(BYTE nMagic)
{
	switch(nMagic)
	{
		case LIFE_UP: return true; break;
		case PROTECTION: return true; break;
		case AUTHORITY_RECOVER_SIGHT: return true; break;
		case AUTHORITY_SIMUNIAN: return true; break;
		//case AUTHORITY_ATHYAS: return true; break;
		case SHIELD: return true; break;
		case HEAL_DISEASE: return true; break;
		case GREAT_HEAL: return true; break;
		case ENCHANT_WEAPON: return true; break;
		case REMOVE_CURSE: return true; break;
		case RESTORE_AUTHORITY: return true; break;
		case PROTECT_DANGER: return true; break;
		case BLESS: return true; break;
		case RECOVER_POISON: return true; break;
		case CURE_DISEASE: return true; break;
		case MIRACLE_RECOVER: return true; break;
		case PERFECT_PROTECT: return true; break;
		case GREAT_SHIELD: return true; break;
		case CANCEL_DIVINE: return true; break;
		case RECURRENCE_DIVINE: return true; break;
		case ANTHEM: return true; break;
		case RELIEF_AUTHORITY: return true; break;
		case GREAT_PRAY: return true; break;
		case PRAY_POWER: return true; break;
		case APPRECIATE: return true; break;
		case HOLY_CURE: return true; break;
		default: return false; break;
	}
	return false;
}
bool isInWarMap() //Added by Eleval to check if the player is in a Local War Map
{
	switch(MapNumber)
	{
		case 4: return true; break; //Hudyne River
		case 13: return true; break; //Hudyne Valley
		case 17: return true; break; //Kalaile Out
		case 22: return true; break; //Renus Out
		case 43: return true; break; //The Great Grassland
		case 55: return true; break; //Big Tree
		case 61: return true; break; //Seddless
		
		default:
			{
				return false;
				break;
			}
	}

	return false;
}
//End of them
///////////////////////////////////////////////////////////////////////////////
// Global Member
///////////////////////////////////////////////////////////////////////////////

CBattleMgr g_mgrBattle;

CStrikeSys* GetStrike()
{
	return (CStrikeSys*)(g_mgrBattle.GetBattle(0));
}

CMagicSys* GetMagic()
{
	return (CMagicSys*)(g_mgrBattle.GetBattle(1));
}

CCombatSys* GetCombat()
{
	return (CCombatSys*)(g_mgrBattle.GetBattle(2));
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CBattleMgr::CBattleMgr()
{
	Clear();
}

CBattleMgr::~CBattleMgr()
{
	for_each(m_vtBuffer.begin(), m_vtBuffer.end(), Delete());
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CBattleMgr::Clear()
{	//< CSD-TW-030624
	ClearDelay();
	ClearSwitchDelay();

	m_vtBuffer.reserve(3);
	m_vtBuffer.push_back(new CStrikeSys);
	m_vtBuffer.push_back(new CMagicSys);
	m_vtBuffer.push_back(new CCombatSys);
	m_bDisplay = true;
}	//> CSD-TW-030624

void CBattleMgr::ClearDelay()
{
	m_dwDelay = 0;
}

void CBattleMgr::ClearSwitchDelay()
{	//< CSD-TW-030624
	m_dwSwitchDelay = 0;
}	//> CSD-TW-030624

int CBattleMgr::Convert(int nKind)
{
	switch (SCharacterData.nCharacterData[SPELL])
	{
	case WIZARD_SPELL:               break; // �������� ���
	case PRIEST_SPELL: nKind += 150; break; // �������� ���
	default:						 break;
	}
	
	return nKind;
}

int CBattleMgr::Return(int nEffect, LPCHARACTER pCaster)
{
	switch (pCaster->sprno)
	{ 
    case 0:
    case 1:
		{
			if (nEffect < 500)
			{
				switch (pCaster->spell)
				{
				case WIZARD_SPELL:                 break; // �������� ���
				case PRIEST_SPELL: nEffect -= 150; break; // �������� ���
				default:						   break;
				}
			}
			else
			{
				nEffect -= 300;
			}
			
			break;
		}
	default:
		break;
	} 
	
	return nEffect;
}

int CBattleMgr::Action(LPCHARACTER pCaster)
{
	const int nResult = GetBattle(2)->Action(pCaster);
	return (nResult == ACTION_NONE) ? GetBattle(0)->Action(pCaster):nResult;
}

void CBattleMgr::Display(int nKind)
{
	if (m_bDisplay)
	{
		g_GameInfo.displayOption = nKind;
		g_GameInfo.displayOptionDelay = 15; 
	}
}

void CBattleMgr::Effect(MAGICLIST* pEffect)
{
	const int nIndex = GetIndex(pEffect->magicSerial);
	GetBattle(nIndex)->Effect(pEffect);
}

void CBattleMgr::Message(int nType, const char* pContext, ...)
{
	char szBuffer[1000];
	// �������ڰ��� ���ۿ� ����
	va_list vaList;
	va_start(vaList, pContext);
	if (vaList == NULL)
	{
		sprintf(szBuffer, "%s", pContext);
	}
	else
	{
		vsprintf( szBuffer, pContext, vaList );
	}
	va_end(vaList);
	
	switch (nType)
	{
    case MK_NORMAL:	
		{ // ����
			::AddCurrentStatusMessage(255, 180, 190, szBuffer);
			break;
		}
	case MK_WARNING:	
		{ // ����
			::AddCurrentStatusMessage(255, 40, 60, szBuffer);
			break;
		}
	case MK_SHORTAGE:	
		{	// ����
			::AddCurrentStatusMessage(150, 150, 255, szBuffer);
			break;
		}
	case MK_INFORMATION:	
		{ // �˸�
			::AddCurrentStatusMessage(200, 200, 200, szBuffer);
			break;
		} 
	default:
		break;
	}
}

void CBattleMgr::ActAnimation(int nType, CHARACTER* pTarget)
{ 
	switch (pTarget->sprno)
	{ 
    case 0:
    case 1:
		{
			::CharDoAni(pTarget , pTarget->direction, nType);
			break;
		}
	default:
		break;
	}
}

void CBattleMgr::ActCasting(int nMagic, CHARACTER* pTarget)
{
	switch (pTarget->sprno)
	{ 
    case 0:
    case 1:
		{
			::CharDoAni(pTarget, pTarget->direction, ACTION_MAGIC_CASTING);
			break;
		}
    case 87: // ������Ʈ
		{
			switch (nMagic) 
			{
			case FIRE_EXPLOSION: // ���̾� ����Ʈ
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_MAGIC);
					break;
				}
			default:
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_MAGIC);
					break;
				}
			}
		}
    case 88: // ����
		{
			switch (nMagic) 
			{
			case BLITZ_SPEAR: // ����â
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_ATTACK);
					break;
				}
			default:
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_MAGIC);
					break;
				}
			}
			
			break;
		}
	case 26:
	case 126:
		{	//< CSD-031112 : ��ũ ������(��)
			switch (nMagic) 
			{
			case FLAME_ARROW:
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_ATTACK);
					break;
				}
			default:
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_MAGIC);
					break;
				}
			}
			
			break;
		}	//> CSD-031112
	case 173:
	case 175:
		{	//< CSD-040202
			switch (nMagic) 
			{
			case FLAME_ARROW:
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_ATTACK);
					break;
				}
			default:
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_MAGIC);
					break;
				}
			}
			
			break;
		}	//> CSD-040202
    default:
		{
			::CharDoAni(pTarget, pTarget->direction, MON1_BATTLE_MAGIC);
			break;
		}
	}

	if (pTarget->nRace == GUARDTOWER)
	{	//< CSD-040202 
		pTarget->nCurrentAction = MON1_BATTLE_MAGIC;
		pTarget->sealstoneani = 0;
	}	//> CSD-040202
}

void CBattleMgr::SendPcAttack(LPCHARACTER pCaster, LPCHARACTER pTarget)
{	//< CSD-TW-030623
	if (pCaster->condition&CON_STONE)
	{
		return;
	}
	
	if (IsThrow())
	{	// ��Ÿ� ������ ���
		SendThrowAttack(pCaster, pTarget);
	}
	else
	{	// ���߰Ÿ� ������ ���
		SendStrikeAttack(pCaster, pTarget);
	}
}	//> CSD-TW-030623

void CBattleMgr::SendNpcAttack(LPCHARACTER pCaster, LPCHARACTER pTarget)
{
	t_packet packet;
	packet.h.header.type = CMD_NPC_ATTACK;
	packet.h.header.size = sizeof(t_npc_attack);
	packet.u.strike.npc_attack.idCaster = WORD(pCaster->id);
	packet.u.strike.npc_attack.idTarget = WORD(pTarget->id);	
	packet.u.strike.npc_attack.nX = pTarget->x;
	packet.u.strike.npc_attack.nY = pTarget->y;
	::QueuePacket(&packet, 1);
}

void CBattleMgr::SendNpcThrow(LPCHARACTER pCaster, LPCHARACTER pTarget)
{
	t_packet packet;
	packet.h.header.type = CMD_NPC_THROW;
	packet.h.header.size = sizeof(t_npc_throw);
	packet.u.strike.npc_attack.idCaster = WORD(pCaster->id);
	packet.u.strike.npc_attack.idTarget = WORD(pTarget->id);	
	packet.u.strike.npc_attack.nX = pTarget->x;
	packet.u.strike.npc_attack.nY = pTarget->y;
	::QueuePacket(&packet, 1);
}

void CBattleMgr::SendStrikeAttack(LPCHARACTER pCaster, LPCHARACTER pTarget)
{	
	if( !IsAttackAble( pTarget ) )
	{
		return;
	}
	
	if (pCaster->attackanimationON)
	{
		switch (g_GameInfo.nSelectedSpriteType)
		{
		case SPRITETYPE_ITEM:
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_CHARACTER:
			{
				if (g_GameInfo.lpcharacter != Hero && Hero->peacests)
				{
					g_queue.lb	= 1;
					g_queue.x	= Mox;
					
					g_queue.y	= Moy;
					g_queue.ch	= g_GameInfo.lpcharacter;
				}
				
				break;
			}
		default:
			break;
		}
		
		return;
	}
	// 020428 YGI acer
	if( SCharacterData.attack_time == g_curr_time )
	{
		return;
	}
	else
	{
		SCharacterData.attack_time = g_curr_time;
	}
	
	pCaster->attackanimationON = 10;

	if (pTarget->type == SPRITETYPE_MONSTER) // finito 14/07/07
	{
		switch (pTarget->nRace)
		{
		case HUMAN:
		case GUARD:
			{	
				Hero->eventnpcid.PutEncrypted(pTarget->id);
				break;
			}
		}
	}

	t_packet packet;
	packet.h.header.type = CMD_ATTACK;
	packet.h.header.size = sizeof(t_player_attack);
	packet.u.strike.player_attack.idTarget = WORD(pTarget->id);
	packet.u.strike.player_attack.bWanted = pTarget->hunter_target;
	::QueuePacket(&packet, 1);

	if(g_autoAttack)
		SendTameCommand(Hero, pTarget); //Eleval 22/05/09 - to set tamed creatures to attack the player's target
}

void CBattleMgr::SendThrowAttack(LPCHARACTER pCaster, LPCHARACTER pTarget)
{	
	if (!IsAttackAble(pTarget))
	{
		return;
	}
	
	if (pCaster->attackanimationON)
	{
		switch (g_GameInfo.nSelectedSpriteType)
		{
		case SPRITETYPE_ITEM:
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_CHARACTER:
			{
				if (g_GameInfo.lpcharacter != Hero && Hero->peacests)
				{
					g_queue.lb	= 1;
					g_queue.x	= Mox;
					
					g_queue.y	= Moy;
					g_queue.ch	= g_GameInfo.lpcharacter;
				}
				
				break;
			}
		default:
				break;
		}
		
		return;
	}
	
	switch (pCaster->nActiveCombat.GetDecrypted())
	{
    case FLEET_MISSILE:
    case MULTIPLE_FIRE:
    case FIRE_EXTREME:
    case ICING_BLAST:
    case ICE_EXTREME:
		{
			pCaster->nCurrentFrame.PutEncrypted(0); //pCaster->nCurrentFrame = 0;
			pCaster->attackanimationON = 15;
			break;
		}
    case CONVERTING_ARMOR:
		{
			pCaster->nCurrentFrame.PutEncrypted(0); //pCaster->nCurrentFrame = 0;
			pCaster->attackanimationON = 10;
			break;
		}
    case SHARK_MISSILE:
		{
			pCaster->nCurrentFrame.PutEncrypted(0); //pCaster->nCurrentFrame = 0;
			
			switch (pCaster->nActiveLevel.GetDecrypted())
			{
			case 1:  pCaster->attackanimationON = 10;  break;
			case 2:  pCaster->attackanimationON = 8;   break;
			case 3:  pCaster->attackanimationON = 6;   break;
			default: pCaster->attackanimationON = 12;  break;
			}
			
			break;
		}
    default:
		{
			pCaster->nCurrentFrame.PutEncrypted(0); //pCaster->nCurrentFrame = 0;
			pCaster->attackanimationON = 10;
			break;
		}
	}

	if (pTarget->type == SPRITETYPE_MONSTER) // finito 14/07/07
	{
		switch (pTarget->nRace)
		{
		case HUMAN:
		case GUARD:
			{	
				Hero->eventnpcid.PutEncrypted(pTarget->id);
				break;
			}
		}
	}
	
	t_packet packet;
	packet.h.header.type = CMD_THROW_ATTACK;
	packet.h.header.size = sizeof(t_client_throw_attack);
	packet.u.strike.client_throw_attack.idTarget = WORD(pTarget->id);
	packet.u.strike.client_throw_attack.bWanted = pTarget->hunter_target;
	::QueuePacket(&packet, 1);
}

void CBattleMgr::SendNpcMagic(BYTE nMagic, LPCHARACTER pCaster, LPCHARACTER pTarget, int nX, int nY)
{
	::ChangeDirection(&pCaster->direction, pCaster->x, pCaster->y, pTarget->x, pTarget->y);
	pCaster->todir = pCaster->direction;
	// ���� ���� Packet ����
	t_packet packet;
	packet.h.header.type = CMD_NPC_MAGIC;
	packet.h.header.size = sizeof(t_client_npc_magic);
	packet.u.magic.client_npc_magic.nMagic = nMagic;
	packet.u.magic.client_npc_magic.idCaster = WORD(pCaster->id);
	packet.u.magic.client_npc_magic.idTarget = WORD(pTarget->id);
	packet.u.magic.client_npc_magic.nX = nX;
	packet.u.magic.client_npc_magic.nY = nY;
	::QueuePacket(&packet, 1);
}

void CBattleMgr::SendMagicSelect(BYTE nMagic)
{	//< CSD-030930
	if (!::TimeCheck(m_dwSwitchDelay))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	else
	{
		m_dwSwitchDelay = g_ClientTime + 1000;
	}

	if (!::IsSelectCombat())
	{	
		SendMagicCasting(nMagic);
		return;
	}
	// ������ų�� ���õ� ���¶��
	t_packet packet;
	packet.h.header.type = CMD_MAGIC_SELECT;
	packet.h.header.size = sizeof(t_client_magic_select);
	packet.u.magic.client_magic_select.nMagic = nMagic;
	::QueuePacket(&packet, 1);
}	//> CSD-030930

void CBattleMgr::SendMagicCasting(BYTE nMagic)
{	//< CSD-TW-030606
	::PrintMagicExplain(nMagic);
	// ĳ������ ���� ���� ���
	if (!::TimeCheck(Hero->end_Casting)) 
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 3));
		return;
	}
	
	if (SCharacterData.nCharacterData[SPELL] && nMagic <= 150)  
	{	// ������ ������ ��� 150�� ����
		nMagic += 150;
	}
	
	if (!IsPlaceMagic(nMagic, Hero->x>>5, Hero->y>>5))
	{	//< CSD-040223
		OUTPUT(MK_WARNING, lan->OutputMessage(1, 4));
		return;
	}	//> CSD-040223
	// �����ð� �ʱ�ȭ
	ClearDelay();
	
	if (!IsMagic(nMagic))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 29));
		return;
	}	
	// ���� ���� Packet ����
	t_packet packet;
	packet.h.header.type = CMD_MAGIC_CASTING;
	packet.h.header.size = sizeof(t_client_magic_casting);
	packet.u.magic.client_magic_casting.nMagic = nMagic;
	::QueuePacket(&packet, 1);
}	//> CSD-TW-030606

void CBattleMgr::SendMagicExecute(BYTE nMagic, LPCHARACTER pTarget, int nX, int nY)
{	//< CSD-TW-030606
	if (pTarget == NULL)  pTarget = Hero;
	// ���� ����� 5�� �̳����� �ٽ� ĳ���� �Ҽ� ������ ����
	Hero->castStartingTime = g_ClientTime + 4000;
	// �̵� ��� ������ ��� �̵����� ���� �˻�
	switch (nMagic)
	{ 
    case PHASING:
    case MEMORIZING_LOCATION:
    case TELEPORT:
    case MULTI_PORT:
    case TOWN_PORTAL:
    case VAMPIRE_DODGE:
		{
			if (pTarget->sprno != 50 && !::IsMovable(nX>>5, nY>>5))
			{
				OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 26));
				return;
			}
			
			break;
		}
	default:
		break;
	} 
	// ���� ���� ����� ����
	int nOffset = 0;
	
	switch (nMagic)
	{
	case AUTHORITY_CHARISNUMEN:
		{
			nOffset = 2000;
			break;
		}
	default:
		{
			nOffset = 800;
			break;
		}
	}
	
	if (Hero->condition&CON_FROZEN)  
	{
		nOffset *= 2;
	}
	
	if (IsDelay(nOffset))
	{	// ���� ���� Packet ����
		t_packet packet;
		packet.h.header.type = CMD_MAGIC_EXECUTE;
		packet.h.header.size = sizeof(t_client_magic_execute);
		packet.u.magic.client_magic_execute.nMagic = nMagic;
		packet.u.magic.client_magic_execute.idTarget = WORD(pTarget->id);
		packet.u.magic.client_magic_execute.nX = nX;
		packet.u.magic.client_magic_execute.nY = nY;
		::QueuePacket(&packet, 1); 

		if(pTarget != Hero && !CheckIfRecoveryMagic(nMagic) && g_autoAttack)
			SendTameCommand(Hero, pTarget); //Eleval 22/05/09 - to set tamed creatures to attack the player's target (when use an offensive magic on it)
	}
}	//> CSD-TW-030606

void CBattleMgr::SendMagicResult(WORD wdEffect, LPCHARACTER pCaster, WORD idTarget, int nX, int nY)
{	//< CSD-031007
	if (!IsIgnoreBattle())
	{	// ����â�� Ȱ��ȭ ���� ������ ���� ����� ������ ����
		return;
	}

	LPCHARACTER pTarget = ::ReturnCharacterPoint(idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
	
	t_packet packet;
	
	if (pCaster == Hero)
	{
		switch (wdEffect)
		{	//< CSD-031014
		case THROW_ARROW:
		case THROW_KNIFE:
		case THROW_FIRE_ARROW:
		case THROW_ICE_KNIFE:
		case THROW_SNOW_BALL:
		case THROW_WATER_BALL: 
		case THROW_BIG_GULP:
		case MULTIPLE_FIRE:
		case FLEET_MISSILE:
		case CONVERTING_ARMOR:
		case FIRE_EXTREME:
		case ICING_BLAST: 
		case SHARK_MISSILE:
		case ICE_EXTREME:
			{
				packet.h.header.type = CMD_THROW_RESULT;
				packet.h.header.size = sizeof(t_client_throw_result);
				packet.u.strike.client_throw_result.idTarget = idTarget;
				packet.u.strike.client_throw_result.nX = nX;
				packet.u.strike.client_throw_result.nY = nY;
				packet.u.strike.client_throw_result.bWanted = pTarget->hunter_target;
				break;
			}
		case ITEM_EFFECT_FIRE:
		case ITEM_EFFECT_ICE:
		case ITEM_EFFECT_LIGHTNING:
			{
				packet.h.header.type = CMD_EFFECT_RESULT;
				packet.h.header.size = sizeof(t_client_effect_result);
				packet.u.strike.client_effect_result.wdEffect = wdEffect;
				packet.u.strike.client_effect_result.idTarget = idTarget;
				packet.u.strike.client_effect_result.nX = nX;
				packet.u.strike.client_effect_result.nY = nY;
				packet.u.strike.client_effect_result.bWanted = pTarget->hunter_target;
				break;
			}
		default:
			{
				switch (GetIndex(wdEffect))
				{
				case 1:
					{
						switch (wdEffect)
						{
						case SCORCHER:
						case FLAME_PILLAR:
						case FLAME_POUR:
						case ICE_BREATH2: 	// 030415 kyo
							{
								if (!pTarget->bCrash)
								{
									pTarget->bCrash = true;
									break;
								}
								
								return;
							}
						default:
							break;
						}
						
						packet.h.header.type = CMD_MAGIC_RESULT;
						packet.h.header.size = sizeof(t_client_magic_result);
						packet.u.magic.client_magic_result.nMagic = static_cast<BYTE>(wdEffect);
						packet.u.magic.client_magic_result.idTarget = idTarget;
						packet.u.magic.client_magic_result.nX = nX;
						packet.u.magic.client_magic_result.nY = nY;
						packet.u.magic.client_magic_result.bWanted = pTarget->hunter_target;
						break;          
					}
				case 2:
					{
						switch (wdEffect)
						{	
						case FIRE_WALL:
						case ICE_WALL:
						case BLOOD_EARTH:
							{
								if (!pTarget->bCrash)
								{
									pTarget->bCrash = true;
									break;
								}
								
								return;
							}
						default:
							break;
						}
						
						packet.h.header.type = CMD_COMBAT_RESULT;
						packet.h.header.size = sizeof(t_client_combat_result);
						packet.u.combat.client_combat_result.nSkill = static_cast<BYTE>(wdEffect);
						packet.u.combat.client_combat_result.idTarget = idTarget;
						packet.u.combat.client_combat_result.nX = nX;
						packet.u.combat.client_combat_result.nY = nY;
						packet.u.combat.client_combat_result.bWanted = pTarget->hunter_target;
						break;
					}
				}
				
				break;
			} 
		}	//> CSD-031014
		
		if (::IsApplyPK(wdEffect))
		{
			::CheckNPCPk(Hero->id, idTarget);
		}

		::QueuePacket(&packet, 1);
	}
	else
	{	
		switch (wdEffect)
		{
		case SCORCHER:
		case FLAME_PILLAR:
		case FLAME_POUR:
		case ICE_BREATH2: 	// 030415 kyo
			{
				if (!pTarget->bCrash)
				{
					pTarget->bCrash = true;  
				}
				
				break;
			}
		}
	}
	
	switch (pCaster->sprno)
	{
    case 0:
    case 1:
		{
			break;
		}
    default:
		{
			if (pTarget == Hero)
			{
				packet.h.header.type = CMD_MAGIC_RESULT_D;
				packet.h.header.size = sizeof(t_magic_result_d);
				packet.u.magic.magic_result_d.nMagic  = static_cast<BYTE>(wdEffect);
				packet.u.magic.magic_result_d.idCaster = WORD(pCaster->id);
				packet.u.magic.magic_result_d.idTarget = idTarget;
				packet.u.magic.magic_result_d.nX = nX;
				packet.u.magic.magic_result_d.nY = nY;
				::QueuePacket(&packet, 1);
			}
			
			break;
		}
	}
}	//> CSD-031007

void CBattleMgr::SendCombatClear()
{	//< CSD-TW-030606
	t_packet packet;
	packet.h.header.type = CMD_COMBAT_CLEAR;
	packet.h.header.size = sizeof(t_client_combat_clear);
	packet.u.combat.client_combat_clear.nType = 0;
	::QueuePacket(&packet, 1);
}	//> CSD-TW-030606

void CBattleMgr::SendCombatObtain(LPCHARACTER pMaster, BYTE nPara, BYTE nX, BYTE nY, BYTE nPoint)
{
	POS pos;
	// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
	::SetItemPos(INV, nPara, nY, nX, &pos);
	// ������ų ������ ȹ�� ������ Ȯ��
	ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];
	
	if (rItemAttr.item_no == 3105)
	{ // Packet ����
		t_packet packet;
		packet.h.header.type = CMD_COMBAT_OBTAIN;
		packet.h.header.size = sizeof(t_client_combat_obtain);
		packet.u.combat.client_combat_obtain.idMaster = WORD(pMaster->id);
		packet.u.combat.client_combat_obtain.nPara = nPara;
		packet.u.combat.client_combat_obtain.nPosX = nX;
		packet.u.combat.client_combat_obtain.nPosY = nY;
		packet.u.combat.client_combat_obtain.nPoint = nPoint;
		::QueuePacket(&packet, 1);  
	} 
}

void CBattleMgr::SendCombatRequest(LPCHARACTER pMaster)
{
	if (pMaster == NULL)  return;
	// Packet ����
	t_packet packet;
	packet.h.header.type = CMD_COMBAT_REQUEST;
	packet.h.header.size = sizeof(t_client_combat_request);
	packet.u.combat.client_combat_request.idMaster = WORD(pMaster->id);
	::QueuePacket(&packet, 1);
}

void CBattleMgr::SendCombatLearn(BYTE nCombat, LPCHARACTER pMaster)
{
	if (pMaster == NULL)  return;  
	// Packet ����
	t_packet packet;
	packet.h.header.type = CMD_COMBAT_LEARN;
	packet.h.header.size = sizeof(t_client_combat_learn);
	packet.u.combat.client_combat_learn.idMaster = WORD(pMaster->id);
	packet.u.combat.client_combat_learn.nSkill = nCombat;
	packet.u.combat.client_combat_learn.nPoint = 1;
	::QueuePacket(&packet, 1);
}

void CBattleMgr::SendCombatSelect(BYTE nCombat)
{	//< CSD-TW-030623 : ���� ��ų ��� ���ɼ�  �ʱ�ȭ
	if (!::TimeCheck(m_dwSwitchDelay))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	else
	{
		m_dwSwitchDelay = g_ClientTime + 1000;
	}
	// ĳ���õ��ȿ��� �����Ҽ� ���� 
	if (!::TimeCheck(Hero->end_Casting))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	// �����迭�� ��ȣ�迭�� ������� �� �����ð� �� ������ų ��� �Ұ��� ����
	if (!::TimeCheck(Hero->continue_RunMagic))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	// ���� ���� ����� 5�� ��� �� �ٽ� ������ų ����
	if (!::TimeCheck(Hero->castStartingTime))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}

	LPCHARACTER pCaster = ::ReturnCharacterPoint(Hero->id);
	if (pCaster == NULL)  return;
	// �����ð� �ʱ�ȭ
	ClearDelay();
	// ���� ��ų ��� ���ɼ�  �ʱ�ȭ
	if (!IsCombat(nCombat))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 61));
		return;
	}	
	// Packet ����
	t_packet packet;
	packet.h.header.type = CMD_COMBAT_SELECT;
	packet.h.header.size = sizeof(t_client_combat_select);
	packet.u.combat.client_combat_select.nCombat = nCombat;
	::QueuePacket(&packet, 1);
}	//> CSD-TW-030623

void CBattleMgr::SendCombatAttack(BYTE nCombat, LPCHARACTER pTarget, int nX, int nY)
{	//< CSD-TW-030623
	if (!::TimeCheck(m_dwSwitchDelay))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	else
	{
		m_dwSwitchDelay = g_ClientTime + 1000;
	}

	CCombatSys* pCombat = (CCombatSys*)GetBattle(2);
	// ������ų ���� ��� �˻�
	int nOffset = pCombat->GetCastContinue(nCombat)*1000;
	
	if (Hero->condition&CON_FROZEN)  
	{
		nOffset *= 2;
	}
    
	if (Hero->continue_RunMagic > g_ClientTime)
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	// ���� ���� ����� 5�� ��� �� �ٽ� ������ų ����
	if(g_ClientTime < Hero->castStartingTime)		
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	
	if (Hero->continue_Time > g_ClientTime)
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	
	if (!IsDelay(nOffset))
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 69));
		return;
	}
	
	if (pCombat->IsEnable(nCombat))
	{ 
		if (pTarget == NULL)  
		{
			pTarget = Hero;
		}
		
		if (nCombat == CHARGING)
		{
			::SendModifyPosition(Hero);
		}
		// ���� ���� Packet ����
		t_packet packet;
		packet.h.header.type = CMD_COMBAT_ATTACK;
		packet.h.header.size = sizeof(t_client_combat_attack);
		packet.u.combat.client_combat_attack.nSkill = nCombat;
		packet.u.combat.client_combat_attack.idTarget = WORD(pTarget->id);
		packet.u.combat.client_combat_attack.nX = nX;
		packet.u.combat.client_combat_attack.nY = nY;
		::QueuePacket(&packet, 1);
	}
}	//> CSD-TW-030623

void CBattleMgr::SendCombatResult(BYTE nCombat, LPCHARACTER pCaster, LPCHARACTER pTarget, int nX, int nY)
{
	if (pCaster == Hero)
	{
		t_packet packet;
		packet.h.header.type = CMD_COMBAT_RESULT;
		packet.h.header.size = sizeof(t_client_combat_result);
		packet.u.combat.client_combat_result.nSkill = nCombat;
		packet.u.combat.client_combat_result.idTarget = WORD(pTarget->id);
		packet.u.combat.client_combat_result.nX = nX;
		packet.u.combat.client_combat_result.nY = nY;
		packet.u.combat.client_combat_result.bWanted = pTarget->hunter_target;
		if (::IsApplyPK(nCombat))  ::CheckNPCPk(Hero->id, pTarget->id);
		::QueuePacket(&packet, 1);
	}
}

void CBattleMgr::SendModifyPosition(LPCHARACTER pTarget)
{ //< CSD-021107
	if (pTarget->pathcount > 2 || pTarget->moveFlag == TRUE)	
	{	// �̵����̾�����	 
		pTarget->pathcount = 0;	// �ϴ� ���缭�� �Ѵ�.
		pTarget->moveFlag = 0;		
		
		switch (pTarget->sprno)
		{
		case 0:
		case 1:
			{
				::SendModifyPosition(pTarget);
				break;
			}
		default:
			{
				::SendModifyPositionNPC(pTarget->id);
				break;
			}
		}
	}
	
} //> CSD-021107

void CBattleMgr::SendTameCommand(LPCHARACTER pCaster, LPCHARACTER pTarget, int command) // Eleval 24/05/09 - Added 3rd argument for /tame follow and /tame stop commands
{
	if (pCaster == NULL || pTarget == NULL)  return;
	
	LPCHARACTER pMaster = pCaster;       // ����
	LPCHARACTER pTame = pCaster->lpNext; // Ÿ�ֵ̹� NPC
	
	while (pTame)
	{
		if (strcmp(pMaster->name, pTame->HostName) == 0)
		{ //< CSD-021024
			t_packet packet;
			packet.h.header.type = CMD_TAME_COMMAND;
			packet.h.header.size = sizeof(t_tame_command);
			packet.u.tame_command.cmd = command; //Eleval 24/05/09, was packet.u.tame_command.cmd = LC_TAME_ATTACK; before
			strcpy(packet.u.tame_command.mybaby, pTame->name);
			if(command == LC_TAME_ATTACK) strcpy(packet.u.tame_command.toname, pTarget->name); //Eleval 24/05/09, was no if before
			else packet.u.tame_command.toname[0] = 0;
			::QueuePacket(&packet, 1);
		}	//> CSD-021024
		
		pTame = pTame->lpNext;
	}
}

void CBattleMgr::RecvCharInfoTactic(t_char_info_tactic* pPacket)
{	//< CSD-TW-030624
	const WORD wKind = pPacket->wKind;
	const DWORD dwExperience = pPacket->dwExperience;
	const BYTE nStep = pPacket->nStep;
	const int nLevel = ::GetLeveByExp(nStep, dwExperience);
		
	if (wKind >= 0 && wKind < 13)
	{	
		Hero->aStepInfo[wKind + TAC_INDEX] = nStep;
		SCharacterData.TacticsExp[wKind] = dwExperience;
		SCharacterData.SkillId[wKind + 88] = nLevel;
		SCharacterData.TacticsExpNext[wKind] = NPC_Lev_Ref[nLevel].nMaxExp;
	}
}	//> CSD-TW-030624

void CBattleMgr::RecvBattleMessage(t_battle_message* pPacket)
{
	const string strMsg = ::lan->OutputMessage(pPacket->nKind, pPacket->nNumber);
	Message(pPacket->nType, strMsg.c_str());
}

void CBattleMgr::RecvBattleDamage(t_battle_damage* pPacket)
{	//< CSD-021019 : hp����ȭ�� ���� ����
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;
	
	const int nDamage = pPacket->nDamage;

	if (nDamage < 0)  
	{
		switch (pPacket->nKind)
		{
		case 0: ::InsertMagic(pTarget, pTarget, 603, 0, 0, 0, pTarget->x, pTarget->y); break;
		}
		
		if (pTarget == Hero)
		{
			SCharacterData.nCharacterHP += nDamage;
			
			if (SCharacterData.nCharacterHP < 0)	
			{
				SCharacterData.nCharacterHP = 0;
			}
			
			if (SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP)
			{
				SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP;
			}

			Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
		}
		
		if (pPacket->idTarget < 10000)
		{
			::CharDoAni(pTarget, pTarget->direction, ACTION_ATTACKED);
		}
		else
		{	
			::CharDoAni(pTarget, pTarget->direction, MON1_ATTACKED);
		}
		
		ShowNumber(-nDamage, pTarget);
	}
}	//> CSD-021019

void CBattleMgr::RecvAttackResult(WORD idCaster, t_attack_result* pPacket)
{	//< CSD-TW-030624
	const int nDamage = pPacket->nDamage;
	const BYTE nResult = pPacket->nResult;
	LPCHARACTER pCaster = ::ReturnCharacterPoint(idCaster);
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;

	const int nReturn = AttackRangeTable[pCaster->sprno][pCaster->accessory[2]][4];
	
	if (nReturn != ACTION_ATTACK_BOW && nReturn != ACTION_THROW)
	{		
		if(pTarget->sprno != 97)// �౸���� ���ġ�� �������� �ʴ´�. 
		{	
			::InputHpUpDown(pPacket->idTarget, nDamage, nResult);
		}	
		
		pCaster->lpAttacked = (nResult) ? (void*)pTarget:NULL;	// 001215 KHS ���� �̽����� �������� ����. 
	}		
	else
	{
		pCaster->lpAttacked		= (void*)pTarget;	// 001215 KHS ���� �̽����� �������� ����. 
	}
	
	pCaster->nAttackedType	= pTarget->type;
	pCaster->attacksuccess = (nResult)  ? true:false;
	pCaster->nActiveCombat.PutEncrypted(pPacket->nCombat);

	::ChangeDirection(&pCaster->direction, pCaster->x, pCaster->y, pTarget->x, pTarget->y);
	pCaster->todir = pCaster->direction;
	pCaster->nCurrentAction = AttackAction(pCaster);
	pCaster->nCurrentFrame.PutEncrypted(0); //pCaster->nCurrentFrame	= 0;
	pCaster->attackanimationON = 10;		// �̼�ġ�� 0�̵Ǹ� �ٽ� �����Ҽ� �ִ�.
	// �̵����ΰ�� �����.
	if (pCaster->id >= 10000) 
	{
		pCaster->gox = pCaster->x;
		pCaster->goy = pCaster->y;
		pCaster->position.x = pCaster->x / TILE_SIZE;
		pCaster->position.y = pCaster->y / TILE_SIZE;
		pCaster->pathcount = pCaster->oldpathcount = 0;
	}
	
	pTarget->attacker_ID = idCaster;	// SoundUp lkh �߰�
	pTarget->NumberAnimationValue.PutEncrypted(nDamage);
	pTarget->attackedresult = nResult;
	pTarget->toflag = (pPacket->nX || pPacket->nY);

	if (pTarget->sprno == 96)
	{
		if (rand()%200 == 0)
		{
			AddCurrentStatusMessage(255,255,255, lan->OutputMessage(5,100));
		}
	}
	
	if (pTarget == Hero)
	{			
		if (pCaster && pCaster->name[0])	
		{
			AddCurrentStatusMessage(255,0,0, lan->OutputMessage(5,98), pCaster->name);
		}
		else 
		{
			AddCurrentStatusMessage(255,0,0, lan->OutputMessage(5,99));
		}
		// ���ֵ̹� NPC�� ���� ���� ���ϸ� �����ϵ��� ��
		if(g_autoDefence)
			SendTameCommand(pTarget, pCaster); //Eleval 23/05/09 - Added the if for the /auto defence command
	}
	else if (nReturn != ACTION_ATTACK_BOW && nReturn != ACTION_THROW)
	{
		::CheckNPCPk(pCaster->id, pTarget->id);
	}
	
	switch (nResult)
	{	
	case HIT_AND_DEAD:
		{
			::SkillSettingClear();

			if (pTarget->willdie <= 0)
			{
				pTarget->willdie = 7;
			}

			if (pTarget == Hero)
			{
				if (pCaster->id > 10000)
				{	
					pTarget->feeltype = FT_BAD_;
					pTarget->feeldelay = 50;
				}	
			}		
					
			break;
		}		
	case HIT_AND_NOTDEAD:
		{
			if (pTarget->toflag == true)
			{	
				pTarget->tox = pPacket->nX;
				pTarget->toy = pPacket->nY;
				SendModifyPosition(pTarget); // CSD-021107
				// �и��� �ٽ� ���콺 ��ġ���� ������ �ְ� �Ѵ�.
				Moveoldx = 0;										
				Moveoldy = 0;
				Movetime = 0;
			}
			
			pTarget->attacked++;
			
			if (nDamage < 0)
			{
				ShowAnimation(ANI_STROKE, pTarget);
			}

			ShowEffectItem(pPacket->wdEffect, pCaster, pTarget); // CSD-031001
			break;	
		}
    case HIT_FAILED: 
		{
			ShowAnimation(ANI_STONE, pTarget);
			break;
		}
	}				
}	//> CSD-TW-030624

void CBattleMgr::RecvAttackResult(t_attack_result_d* pPacket)
{	//< CSD-TW-030606
	const int nDamage = pPacket->nDamage;
	const BYTE nResult = pPacket->nResult;
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idCaster);
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pCaster == NULL || pTarget == NULL)  return; // CSD-031127
	
	const int nReturn = AttackRangeTable[pCaster->sprno][pCaster->accessory[2]][4];
	
	if (nReturn != ACTION_ATTACK_BOW && nReturn != ACTION_THROW)
	{		
		if(pTarget->sprno != 97)// �౸���� ���ġ�� �������� �ʴ´�. 
		{	
			::InputHpUpDown(pPacket->idTarget, nDamage, nResult);
		}	
		
		pCaster->lpAttacked = (nResult) ? (void*)pTarget:NULL;
	}	
	else
	{
		pCaster->lpAttacked	= (void*)pTarget;	// 001215 KHS ���� �̽����� �������� ����. 
	}
	
	pCaster->nAttackedType	= pTarget->type;
	pCaster->attacksuccess = (nResult) ? true:false;
	pCaster->nActiveCombat.PutEncrypted(pPacket->nCombat); // CSD-TW-030606
	
	::ChangeDirection(&pCaster->direction, pCaster->x, pCaster->y, pTarget->x, pTarget->y);
	pCaster->todir = pCaster->direction;
	pCaster->nCurrentAction = ::AttackAction(pCaster);
	pCaster->nCurrentFrame.PutEncrypted(0); //pCaster->nCurrentFrame	= 0;
	pCaster->attackanimationON = 10;		// �̼�ġ�� 0�̵Ǹ� �ٽ� �����Ҽ� �ִ�.
	
	CStrikeSys* pStrike = (CStrikeSys*)GetBattle(0);
	pStrike->EffectHit(pCaster, pTarget);
	
	if (pCaster->id >= 10000) 
	{
		pCaster->gox = pCaster->x;
		pCaster->goy = pCaster->y;
		pCaster->position.x = pCaster->x / TILE_SIZE;
		pCaster->position.y = pCaster->y / TILE_SIZE;
		pCaster->pathcount = pCaster->oldpathcount = 0;
	}
	
	pTarget->attacker_ID = pPacket->idCaster;
	pTarget->NumberAnimationValue.PutEncrypted(nDamage);
	pTarget->attackedresult = nResult;
	pTarget->toflag = (pPacket->nX || pPacket->nY);
	
	if (pTarget == Hero)
	{			
		if (pCaster->name[0])	
		{
			::AddCurrentStatusMessage(255,0,0, lan->OutputMessage(5,98), pCaster->name);
		}
		else 
		{
			::AddCurrentStatusMessage(255,0,0, lan->OutputMessage(5,99));
		}
		// ���ֵ̹� NPC�� ���� ���� ���ϸ� �����ϵ��� ��
		if(g_autoDefence)
			SendTameCommand(pTarget, pCaster); //Eleval 23/05/09 - Added the if for the /auto defence command
	}
	else if (nReturn != ACTION_ATTACK_BOW && nReturn != ACTION_THROW)
	{
		::CheckNPCPk(pCaster->id, pTarget->id);
	}
	
	if (pTarget->sprno == 96)
	{
		if (rand()%200 == 0)
		{
			::AddCurrentStatusMessage(255,255,255, lan->OutputMessage(5,100));
		}
	}
	
	switch (nResult)
	{				
	case HIT_AND_DEAD:
		{
			::SkillSettingClear();

			if (pTarget->willdie <= 0)
			{
				pTarget->willdie = 7; 
			}

			if (pTarget == Hero)
			{
				if (pCaster->id > 10000)
				{	
					pTarget->feeltype = FT_BAD_;
					pTarget->feeldelay = 50;
				}
			}

			break;
		}		
	case HIT_AND_NOTDEAD:
		{
			if (pTarget->toflag == true) // ĳ���� ���ݴ��� �и��� ���� ���� �ɼ� ��ǥ ����
			{	
				pTarget->tox = pPacket->nX;
				pTarget->toy = pPacket->nY;
				SendModifyPosition(pTarget); // CSD-021107
				// �и��� �ٽ� ���콺 ��ġ���� ������ �ְ� �Ѵ�.
				Moveoldx = 0;										
				Moveoldy = 0;
				Movetime = 0;
			}
			
			pTarget->attacked++;
			
			if (nDamage < 0)  
			{
				ShowAnimation(ANI_STROKE, pTarget);
			}
			
			ShowEffectItem(pPacket->wdEffect, pCaster, pTarget); // CSD-031001
			break;	
		}
    case HIT_FAILED: 
		{
			ShowAnimation(ANI_STONE, pTarget);
			break;
		}
	}	
}	//> CSD-TW-030606

void CBattleMgr::RecvThrowAttack(t_server_throw_attack* pPacket)
{	//< CSD-TW-030606
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idCaster);
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;  
	
	const int nNo = ::GetCharAccessory(pCaster); // CSD-030723

	const int nReturn = AttackRangeTable[pCaster->sprno][nNo][4];
	const int nX = pPacket->nX;
	const int nY = pPacket->nY;
	const int nCombat = pPacket->nCombat;
	
	switch (nReturn)
	{
    case ACTION_THROW:
    case ACTION_ATTACK_BOW:
		{	
			::ChangeDirection(&pCaster->direction, pCaster->x, pCaster->y, pTarget->x, pTarget->y);
			pCaster->todir = pCaster->direction;
			pCaster->lpAttacked = pTarget;
			pCaster->nAttackedType	= pTarget->type;
			pCaster->nCurrentAction = ::AttackAction(pCaster);
			pCaster->nCurrentFrame.PutEncrypted(0); //pCaster->nCurrentFrame = 0;
			pCaster->nActiveCombat.PutEncrypted(nCombat);
			break;
		}
    default: 
		{
			return;
		}
	}
	
	switch (nCombat)
	{
    case FLEET_MISSILE:
    case MULTIPLE_FIRE:
    case FIRE_EXTREME:
    case ICING_BLAST:
    case ICE_EXTREME:
		{
			pCaster->bShoot = false;
			pCaster->attackanimationON = 15;
			break;
		}
    case CONVERTING_ARMOR:
		{
			pCaster->bShoot = false;
			pCaster->attackanimationON = 10;
			break;
		}
    case SHARK_MISSILE:
		{
			pCaster->bShoot = false;
			
			switch (pCaster->nActiveLevel.GetDecrypted())
			{
			case 1:  pCaster->attackanimationON = 10;  break;
			case 2:  pCaster->attackanimationON = 8;   break;
			case 3:  pCaster->attackanimationON = 6;   break;
			default: pCaster->attackanimationON = 12;  break;
			}
			
			break;
		}
    default:
		{
			pCaster->bShoot = true;
			pCaster->attackanimationON = 10;
			break;
		}
	}
	// �̵����ΰ�� �����.
	if (pCaster->id >= 10000) 
	{
		pCaster->gox = pCaster->x;
		pCaster->goy = pCaster->y;
		pCaster->position.x = (pCaster->x>>CELL);
		pCaster->position.y = (pCaster->y>>CELL);
		pCaster->pathcount = pCaster->oldpathcount = 0;
	}
	
	pTarget->attacker_ID = pPacket->idCaster;
	pTarget->toflag = (nX || nY); // Ư�� �и��� ��ǥ�� �Ѿ�� ��츸 �и��� ó��
}	//> CSD-TW-030606

void CBattleMgr::RecvThrowResult(WORD idCaster, t_server_throw_result* pPacket)
{	//< CSD-TW-030624
	const int nDamage = pPacket->nDamage;
	const BYTE nResult = pPacket->nResult;
	LPCHARACTER pCaster = ::ReturnCharacterPoint(idCaster);
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
		
	pCaster->attacksuccess = (nResult)  ? true:false;
	pCaster->nActiveCombat.PutEncrypted(pPacket->nCombat);
	
	pTarget->attacker_ID = idCaster;	// SoundUp lkh �߰�
	pTarget->NumberAnimationValue.PutEncrypted(nDamage);
	pTarget->attackedresult = nResult;
	pTarget->toflag = (pPacket->nX || pPacket->nY);

	if(pTarget->sprno != 97)// �౸���� ���ġ�� �������� �ʴ´�. 
	{	
		::InputHpUpDown(pPacket->idTarget, nDamage, nResult);
	}

	if (pTarget->sprno == 96)
	{
		if (rand()%200 == 0)
		{
			::AddCurrentStatusMessage(255,255,255, lan->OutputMessage(5,100));
		}
	}
	
	if (pTarget == Hero)
	{			
		if (pCaster->name[0])	
		{ 
			switch (pCaster->accessory[2])
			{	//< CSD-030723
			case 120:
				{
					::AddCurrentStatusMessage(255, 255, 255, lan->OutputMessage(0, 11), pCaster->name);
					break;
				}
			case 127:
				{
					::AddCurrentStatusMessage(255, 255, 255, lan->OutputMessage(0, 12), pCaster->name);
					break;
				}
			case 128:
				{
					::AddCurrentStatusMessage(255, 255, 255, lan->OutputMessage(0, 13), pCaster->name);
					break;
				}
			default:
				{
					::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(5, 98), pCaster->name);
					break;
				}
			}	//> CSD-030723
		} 
		// ���ֵ̹� NPC�� ���� ���� ���ϸ� �����ϵ��� ��
		if(g_autoDefence)
			SendTameCommand(pTarget, pCaster); //Eleval 23/05/09 - Added the if for the /auto defence command
	}
	else
	{
		::CheckNPCPk(pCaster->id, pTarget->id);
	}
	
	switch (nResult)
	{	
	case HIT_AND_DEAD:
		{
			::SkillSettingClear();

			if (pTarget->willdie <= 0)
			{
				pTarget->willdie = 7;
			}

			if (pTarget == Hero)
			{
				if (pCaster->id > 10000)
				{	
					pTarget->feeltype = FT_BAD_;
					pTarget->feeldelay = 50;
				}
			}

			break;		
		}		
	case HIT_AND_NOTDEAD:
		{
			if (pTarget->toflag)
			{	
				pTarget->tox = pPacket->nX;
				pTarget->toy = pPacket->nY;
				SendModifyPosition(pTarget); // CSD-021107
				// �и��� �ٽ� ���콺 ��ġ���� ������ �ְ� �Ѵ�.
				Moveoldx = Moveoldy = Movetime = 0;
			}
			
			pTarget->attacked++;
			
			if (nDamage < 0)  
			{
				ShowAnimation(ANI_STROKE, pTarget);
			}
			
			ShowEffectItem(pPacket->wdEffect, pCaster, pTarget); // CSD-031001
			break;	
		}
    case HIT_FAILED: 
		{
			ShowAnimation(ANI_STONE, pTarget);
			break;
		}
	}
}	//> CSD-TW-030624

void CBattleMgr::RecvThrowResult(t_throw_result_d* pPacket)
{	//< CSD-TW-030606
	const int nDamage = pPacket->nDamage;
	const BYTE nResult = pPacket->nResult;
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idCaster);
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;

	pCaster->attacksuccess = (nResult) ? true:false;
	pCaster->nActiveCombat.PutEncrypted(pPacket->nCombat); 
	
	if (pTarget->sprno != 97) // �౸���� Ÿ��ġ�� �������� �ʴ´�.
	{	
		::InputHpUpDown(pPacket->idTarget, nDamage, nResult);
	}	

	if (pTarget->sprno == 96)
	{
		if (rand()%200 == 0)
		{
			::AddCurrentStatusMessage(255,255,255, lan->OutputMessage(5,100));
		}
	}
	
	pTarget->attacker_ID = pPacket->idCaster;
	pTarget->NumberAnimationValue.PutEncrypted(nDamage);
	pTarget->attackedresult = nResult;
	pTarget->toflag = (pPacket->nX || pPacket->nY);	// Ư�� �и��� ��ǥ�� �Ѿ�� ��츸 �и��� ó��
	
	if (pTarget == Hero)
	{			
		if (pCaster->name[0])
		{ 
			switch (pCaster->accessory[2])
			{	//< CSD-030723
			case 120:
				{
					::AddCurrentStatusMessage(255, 255, 255, lan->OutputMessage(0, 11), pCaster->name);
					break;
				}
			case 127:
				{
					::AddCurrentStatusMessage(255, 255, 255, lan->OutputMessage(0, 12), pCaster->name);
					break;
				}
			case 128:
				{
					::AddCurrentStatusMessage(255, 255, 255, lan->OutputMessage(0, 13), pCaster->name);
					break;
				}
			default:
				{
					::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(5, 98), pCaster->name);
					// ���ֵ̹� NPC�� ���� ���� ���ϸ� �����ϵ��� ��
					if(g_autoDefence)
						SendTameCommand(pTarget, pCaster); //Eleval 23/05/09 - Added the if for the /auto defence command
					break;
				}
			}	//> CSD-030723
		} 
		// ���� ���� �����̰� �ƴ϶�� ���ֵ̹� NPC�� ���� ���� ���ϸ� �����ϵ��� ��
		if (pCaster->accessory[2] != 120 && g_autoDefence)  SendTameCommand(pTarget, pCaster); //Eleval 22/05/09 - Added g_autoDefence to the if for the /auto defence command
	}
	else
	{
		::CheckNPCPk(pCaster->id, pTarget->id);
	}
	
	switch (nResult)
	{	
	case HIT_AND_DEAD:
		{
			::SkillSettingClear();

			if (pTarget->willdie <= 0)
			{	// �� ��ġ�� �ٿ�Ǵٰ� 1�� �Ǹ� �״´�.
				pTarget->willdie = 7; 
			}

			if (pTarget == Hero) 
			{	
				if (pCaster->id > 10000)
				{	
					pTarget->feeltype = FT_BAD_;
					pTarget->feeldelay = 50;
				}
			}
			
			break;
		}		
	case HIT_AND_NOTDEAD:
		{
			if (pTarget->toflag)
			{	
				pTarget->tox = pPacket->nX;
				pTarget->toy = pPacket->nY;
				SendModifyPosition(pTarget); // CSD-021107
				// �и��� �ٽ� ���콺 ��ġ���� ������ �ְ� �Ѵ�.
				Moveoldx = Moveoldy = Movetime = 0;
			}
			
			pTarget->attacked++;
			
			if (nDamage < 0)
			{
				ShowAnimation(ANI_STROKE, pTarget);
			}
			
			ShowEffectItem(pPacket->wdEffect, pCaster, pTarget); // CSD-031001
			break;	
		}
    case HIT_FAILED: 
		{
			ShowAnimation(ANI_STONE, pTarget);
			break;
		}
	}
}	//> CSD-TW-030606
void CBattleMgr::RecvEffectResult(t_server_effect_result* pPacket)
{	//< CSD-031007
	int nLife = pPacket->nDamage;
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;
	
	switch (pPacket->nResult)
	{
	case HIT_AND_ALIVE:   
		{      
			if (pTarget->hp <= 0 || pTarget->condition == CON_DEATH)
			{ // �׾��ִ� ���
				return;
			}
			
			if (nLife <= 0)  
			{ // �׾��ִ� ���
				return;
			}
			
			++pTarget->attacked;
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ��� 
			{
				SCharacterData.nCharacterHP = nLife;
				
				if (SCharacterData.nCharacterHP <= 0) 
				{
					SCharacterData.nCharacterHP = 1;	//����������   
				}
				
				if (SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP)
				{
					SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP;
				}

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}
			
			SetViewType(pTarget);
			pTarget->hp	= nLife;
			break;
		}
    case HIT_AND_NOTDEAD:
		{
			if (pTarget->hp <= 0 || pTarget->condition == CON_DEATH)
			{ // �׾��ִ� ���
				return;
			}
			
			++pTarget->attacked;
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ��� 
			{
				SCharacterData.nCharacterHP += nLife;
				
				if (SCharacterData.nCharacterHP <= 0) 
				{
					SCharacterData.nCharacterHP = 1;	//����������   
				}
				
				if (SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP) 
				{
					SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP;
				}

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}
			
			SetViewType(pTarget);
			pTarget->hp	+= nLife;
			if (pTarget->hp <= 0)  pTarget->hp = 1;
			if (pTarget->hp > pTarget->hpmax)  pTarget->hp = pTarget->hpmax;
			if (nLife < 0)  ShowAnimation(ANI_STROKE, pTarget);
			
			break;
		}
    case HIT_AND_DEAD:
		{
			pTarget->changeActionFlag = 0;
			ShowAnimation(ANI_DEATH, pTarget);
			pTarget->pathcount = 0;	// �������� �ʰ�
			pTarget->moveFlag = 0;  // �׾�ƴٴ��� ���ϰ�
			
			if (pTarget == Hero)		
			{ // �״� ĳ���Ͱ� �ڽ��� ���
				SCharacterData.nCharacterHP = 0;
				SCharacterData.nCharacterMP = 0;
				SCharacterData.condition = CON_DEATH;

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}	
			
			pTarget->hp = 0;
			pTarget->mp	= 0;
			pTarget->condition = CON_DEATH;
			pTarget->viewtype = VIEWTYPE_NORMAL_;
			break;
		}
	}		
	
	ShowNumber(nLife, pTarget);
}	//> CSD-031007

void CBattleMgr::RecvNpcMagic(t_server_npc_magic* pPacket)
{
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idCaster);
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	
	if (pCaster != NULL && pTarget != NULL)
	{
		//< CSD-021021
		::ChangeDirection(&pCaster->direction, pCaster->x, pCaster->y, pTarget->x, pTarget->y);
		pCaster->todir = pCaster->direction;
		//> CSD-021021
		const int nMagic = pPacket->nMagic;
		pCaster->excute_MagicNum = nMagic;
		pCaster->ready_Magic = 2;
		
		CMagicSys* pMagic = (CMagicSys*)GetBattle(1);
		pMagic->Apply(pCaster, pTarget, nMagic,
			pPacket->nX, pPacket->nY, pPacket->bFailed);
	}
}

void CBattleMgr::RecvMagicSelect(t_server_magic_select* pPacket)
{	//< CSD-TW-030606
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idCaster);

	if (pCaster == NULL)
	{
		return;
	}
	// ������ų ���� ���
	pCaster->nActiveCombat.PutEncrypted(0);
	pCaster->nPassiveCombat = 0;
	pCaster->nActiveLevel.PutEncrypted(0);
	pCaster->nPassiveLevel = 0;
	
	pCaster->ready_Magic = 0;
	pCaster->excute_MagicNum = 0;
	pCaster->end_Casting = 0;
	
	if (pCaster == Hero)
	{ 
		iCSPCMax = iCSPCNow = 0;
		::SetSCDMainAndOtherClear(0);
		SendMagicCasting(pPacket->nMagic);
	}
}	//> CSD-TW-030606

void CBattleMgr::RecvMagicCasting(t_server_magic_casting* pPacket)
{	//< CSD-TW-030606
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idCaster);
	
	if (pCaster != NULL)  
	{ 
		WORD wReady = pPacket->wReady;
		
		if (pCaster->condition&CON_FROZEN)
		{
			wReady *= 2;
		}

		CMagicSys* pMagic = (CMagicSys*)GetBattle(1);
		pMagic->Ready(pCaster, pPacket->nMagic, wReady);
		connections.receive = 1;
	} 
}	//> CSD-TW-030606

void CBattleMgr::RecvMagicExecute(t_server_magic_execute* pPacket)
{	//< CSD-030930
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idCaster);
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;

	LPCHARACTER pTemp = Hero;
	
	while (pTemp = pTemp->lpNext)
	{
		pTemp->bCrash = false;
	}
	
	const int nMagic = pPacket->nMagic;
	const bool bFail = pPacket->bFailed;
	pCaster->excute_MagicNum = nMagic;
	pCaster->ready_Magic = 2;
	
	CMagicSys* pMagic = (CMagicSys*)GetBattle(1);
	pMagic->Apply(pCaster, pTarget, nMagic, pPacket->nX, pPacket->nY, bFail);
}	//> CSD-CSD-030930

void CBattleMgr::RecvMagicResult(t_server_magic_result* pPacket)
{	//< CSD-TW-030627 : �ַ� �Ͻ����� ȿ������ ��Ÿ���� ����� �����
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;
	
	switch (pPacket->nResult)
	{ 
    case HIT_SUMMONING:
    case HIT_AND_NOTDEAD:
		{
			++pTarget->attacked;
			::ConditionSet(pTarget, pPacket->nState); // ���� ����
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ���
			{	
				SCharacterData.condition = pTarget->condition;
			}
			
			SetViewType(pTarget);
			break;
		}
	case HIT_AND_DEAD:
		{
			pTarget->changeActionFlag = 0;
			ShowAnimation(ANI_DEATH, pTarget);
			pTarget->pathcount = 0;	// �������� �ʰ�
			pTarget->moveFlag = 0;  // �׾�ƴٴ��� ���ϰ�
			
			if (pTarget == Hero)		
			{ // �״� ĳ���Ͱ� �ڽ��� ���
				SCharacterData.nCharacterHP = 0;
				SCharacterData.nCharacterMP = 0;
				SCharacterData.condition = CON_DEATH;

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}	
			
			pTarget->hp = 0;
			pTarget->mp	= 0;
			pTarget->condition = CON_DEATH;
			pTarget->viewtype = VIEWTYPE_NORMAL_;
			break;
		}
	}		
}	//> CSD-TW-030627

void CBattleMgr::RecvMagicResultT(t_server_magic_result_t* pPacket)
{	//< CSD-TW-030606 : Life�� Mana�� ��ȭ�� ������� �������� ������ ���
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;
	
	switch (pPacket->nResult)
	{		
	case HIT_AND_NOTDEAD:
		{
			++pTarget->attacked;
			::ConditionSet(pTarget, pPacket->nState);
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ��� 
			{
				SCharacterData.condition = pTarget->condition;
			}
			
			switch (pPacket->nMagic)
			{
			case GREAT_SHIELD:
				{
					DWORD dwPeriod = g_ClientTime + pPacket->wDuration*1000;
					::InsertMagic(pTarget, pTarget, 354, 0, 0, 0, pTarget->x, pTarget->y, false, 0, false, dwPeriod);
					break;
				}
			case GREAT_PRAY:
				{
					DWORD dwPeriod = g_ClientTime + pPacket->wDuration*1000;
					::InsertMagic(pTarget, pTarget, 353, 0, 0, 0, pTarget->x, pTarget->y, false, 0, false, dwPeriod);
					break;
				}
			default:
				{
					MAGICLIST* pMagic = ::FindList(g_lpML, pPacket->nMagic, pPacket->idTarget);
					
					if (pMagic != NULL)
					{	// �и�������� ��ȯ
						pMagic->magic_Runtime = g_ClientTime + pPacket->wDuration*1000;
					}
					
					break;
				}
			}
			
			SetViewType(pTarget);
			break;
		}
	}		
}	//> CSD-TW-030606

void CBattleMgr::RecvMagicResultH(t_server_magic_result_h* pPacket)
{	//< CSD-TW-030606
	int nLife = pPacket->nLife;
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;
	
	switch (pPacket->nResult)
	{
	case HIT_AND_ALIVE:   
		{      
			if (pTarget->hp <= 0 || pTarget->condition == CON_DEATH)
			{ // �׾��ִ� ���
				return;
			}
			
			if (nLife <= 0)  
			{ // �׾��ִ� ���
				return;
			}
			
			++pTarget->attacked;
			::ConditionSet(pTarget, pPacket->nState);	// ���¸� �缳��
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ��� 
			{
				SCharacterData.condition = pTarget->condition;
				SCharacterData.nCharacterHP = nLife;
				
				if (SCharacterData.nCharacterHP <= 0) 
				{
					SCharacterData.nCharacterHP = 1;	//����������   
				}
				
				if (SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP)
				{
					SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP;
				}

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}
			
			SetViewType(pTarget);
			pTarget->hp	= nLife;
			break;
		}
    case HIT_AND_NOTDEAD:
		{
			if (pTarget->hp <= 0 || pTarget->condition == CON_DEATH)
			{ // �׾��ִ� ���
				return;
			}
			
			++pTarget->attacked;
			::ConditionSet(pTarget, pPacket->nState);	// ���¸� �缳��
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ��� 
			{
				SCharacterData.condition = pTarget->condition;
				SCharacterData.nCharacterHP += nLife;
				
				if (SCharacterData.nCharacterHP <= 0) 
				{
					SCharacterData.nCharacterHP = 1;	//����������   
				}
				
				if (SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP) 
				{
					SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP;
				}

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}
			
			SetViewType(pTarget);
			pTarget->hp	+= nLife;
			if (pTarget->hp <= 0)  pTarget->hp = 1;
			if (pTarget->hp > pTarget->hpmax)  pTarget->hp = pTarget->hpmax;
			if (nLife < 0)  ShowAnimation(ANI_STROKE, pTarget);
			
			break;
		}
    case HIT_AND_DEAD:
		{
			pTarget->changeActionFlag = 0;
			ShowAnimation(ANI_DEATH, pTarget);
			pTarget->pathcount = 0;	// �������� �ʰ�
			pTarget->moveFlag = 0;  // �׾�ƴٴ��� ���ϰ�
			
			if (pTarget == Hero)		
			{ // �״� ĳ���Ͱ� �ڽ��� ���
				SCharacterData.nCharacterHP = 0;
				SCharacterData.nCharacterMP = 0;
				SCharacterData.condition = CON_DEATH;

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}	
			
			pTarget->hp = 0;
			pTarget->mp	= 0;
			pTarget->condition = CON_DEATH;
			pTarget->viewtype = VIEWTYPE_NORMAL_;
			break;
		}
	}		
	
	ShowNumber(nLife, pTarget);
}	//> CSD-TW-030606

void CBattleMgr::RecvMagicResultM(t_server_magic_result_m* pPacket)
{	//< CSD-TW-030606
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;
	
	switch (pPacket->nResult)
	{		
	case HIT_AND_DEAD:
		{
			pTarget->changeActionFlag = 0;
			ShowAnimation(ANI_DEATH, pTarget);
			pTarget->pathcount= 0; // �������� �ʰ�
			pTarget->moveFlag = 0; // �׾�ƴٴ��� ���ϰ�
			pTarget->hp = 0;
			pTarget->mp = 0;
			pTarget->condition = CON_DEATH;
			pTarget->viewtype = VIEWTYPE_NORMAL_;
			// ���¾� �״� ���� �ٷ� ���� ���
			if (pTarget == Hero)
			{
				SCharacterData.nCharacterHP = 0;
				SCharacterData.nCharacterMP = 0;
				SCharacterData.condition = CON_DEATH;

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}	
			
			break;
		}	
	case HIT_AND_NOTDEAD:
		{
			if( pTarget->hp <= 0 || pTarget->condition == CON_DEATH)
			{ // �׾��ִ� �����
				break;
			}
			
			++pTarget->attacked;
			::ConditionSet(pTarget, pPacket->nState);	// ���¸� �缳��
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ��� 
			{
				SCharacterData.condition = pTarget->condition;
				SCharacterData.nCharacterHP += pPacket->nLife;
				SCharacterData.nCharacterMP += pPacket->nMana;
				
				if (SCharacterData.nCharacterHP <= 0) 
				{
					SCharacterData.nCharacterHP = 1;	//����������   
				}
				
				if (SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP) 
				{
					SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP;
				}
				
				if (SCharacterData.nCharacterMP < 0) 
				{
					SCharacterData.nCharacterMP = 0;
				}
				
				if (SCharacterData.nCharacterMP > SCharacterData.nCharacterMAXMP) 
				{
					SCharacterData.nCharacterMP = SCharacterData.nCharacterMAXMP;
				}

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}
			
			SetViewType(pTarget);
			pTarget->hp += pPacket->nLife;
			if (pTarget->hp <= 0)  pTarget->hp = 1;			
			if (pTarget->hp > pTarget->hpmax)  pTarget->hp = pTarget->hpmax;
			pTarget->mp += pPacket->nMana;
			if (pTarget->mp < 0)  pTarget->mp = 0;
			if (pTarget->mp > pTarget->mpmax)  pTarget->mp = pTarget->mpmax;
			if (pPacket->nLife < 0)  ShowAnimation(ANI_STROKE, pTarget);
			
			break;
		}	
	case HIT_AND_ALIVE:
		{
			if (pTarget == Hero)
			{
				SCharacterData.nCharacterHP += pPacket->nLife;
				SCharacterData.nCharacterMP += pPacket->nMana;
				SCharacterData.condition = CON_NORMAL;
				
				if (SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP) 
				{
					SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP;
				}
				
				if (SCharacterData.nCharacterMP > SCharacterData.nCharacterMAXMP) 
				{
					SCharacterData.nCharacterMP = SCharacterData.nCharacterMAXMP;	
				}

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}
			
			pTarget->hp	+= pPacket->nLife;
			pTarget->mp	+= pPacket->nMana;
			pTarget->condition = CON_NORMAL;
			pTarget->viewtype = VIEWTYPE_NORMAL_;
			pTarget->dieani = 0;
			pTarget->blood_Set = false;
			if (pTarget==Hero) ::PlayListAutoSounds(2408, 0, 0, 0);		// LTS SOUND
			else
			{
				int distx=pTarget->x-Hero->x;
				int disty=pTarget->y-Hero->y;
				::PlayListAutoSounds(2408, distx, disty, 0);
			}
			
			if (pTarget->hp > pTarget->hpmax)  pTarget->hp = pTarget->hpmax;
			if (pTarget->mp > pTarget->mpmax)  pTarget->mp = pTarget->mpmax;
			if (pTarget->changeActionFlag == 1)  pTarget->changeActionFlag=0;
			ShowAnimation(ANI_NONE, pTarget);
			break;
		}
	}
	ShowNumber(pPacket->nLife, pTarget);
}	//> CSD-TW-030606

void CBattleMgr::RecvCombatInit(t_combat_init* pPacket)
{	//< CSD-TW-030606
	LPCHARACTER pMaster = ::ReturnCharacterPoint(pPacket->idMaster);
	
	if (pMaster == NULL)
	{
		return;
	}
	// ������ų �ʱ�ȭ
	switch (pPacket->nType)
	{
    case ACTIVE_COMBAT: 
		{
			pMaster->nActiveCombat.PutEncrypted(0);
			pMaster->nActiveLevel.PutEncrypted(0);
			break;
		}
    case PASSIVE_COMBAT: 
		{
			pMaster->nPassiveCombat = 0;
			pMaster->nPassiveLevel = 0;
			break;
		}
	case RECOVERY_COMBAT: 
		{
			pMaster->nRecoveryCombat = 0;
			pMaster->nRecoveryLevel = 0;
			break;
		}
	}
}	//> CSD-TW-030606

void CBattleMgr::RecvCombatClear(t_server_combat_clear* pPacket)
{	//< CSD-TW-030606
	LPCHARACTER pMaster = ::ReturnCharacterPoint(pPacket->idMaster);
	
	if (pMaster == NULL)
	{
		return;
	}
	// ������ų ���� ���
	pMaster->nActiveCombat.PutEncrypted(0);
	pMaster->nPassiveCombat = 0;
	pMaster->nRecoveryCombat = 0;
	pMaster->nActiveLevel.PutEncrypted(0);
	pMaster->nPassiveLevel = 0;
	pMaster->nRecoveryLevel = 0;
	
	pMaster->ready_Magic = 0;
	pMaster->excute_MagicNum = 0;
	pMaster->end_Casting = 0;
	
	if (pMaster == Hero)
	{ 
		iCSPCMax = iCSPCNow = 0;
		::SetSCDMainAndOtherClear(0);
	}
}	//> CSD-TW-030606

void CBattleMgr::RecvCombatObtain(t_server_combat_obtain* pPacket)
{
	LPCHARACTER pMaster = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pMaster == NULL)  return;
	
	if (pMaster == Hero)
	{
		const int nPara = pPacket->nPara;
		const int nX = pPacket->nPosX; 
		const int nY = pPacket->nPosY; 
		const int nTotal = pPacket->nTotal;
		// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
		POS pos;
		::SetItemPos(INV, nPara, nY, nX, &pos);
		// ������ų ������ ȹ�� ������ ����
		ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];
		
		if (rItemAttr.item_no == 3105)
		{
			::UseItemByRbutton(pos, rItemAttr);
			pMaster->aStepInfo[CSP_STEP] = iCSReservPoint = nTotal + 1;
			OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 51));
		}
	}
	// ������ų ������ ȹ�� ����Ʈ ���
	::InsertMagic(pMaster, pMaster, 500, 0, 0, 0, 0, 0);
}

void CBattleMgr::RecvCombatRequest(t_server_combat_request* pPacket)
{
	LPCHARACTER pMaster = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pMaster == NULL)  return;
	
	pMaster->aStepInfo[CSP_STEP] = pPacket->nRemain;
	
	for (int i = LIGHTNING_BOOM; i <= WIND_EXTREME; ++i)
	{
		SCharacterData.SkillId[i - 100] = pPacket->aLevel[i - LIGHTNING_BOOM];
	}
	
	::CallCombatSkillPointMenu();
}

void CBattleMgr::RecvCombatLearn(t_server_combat_learn* pPacket)
{
	LPCHARACTER pMaster = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pMaster == NULL)  return;
	
	const int nCombat = pPacket->nSkill;
	
	if (IsCombat(nCombat))
	{
		const int nLevel = pPacket->nLevel;
		
		if (nLevel <= 1)
		{
			OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 52));
		}
		else
		{
			OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 53));
		}
		
		SCharacterData.SkillId[nCombat - 100] = nLevel;
		CombatSkill[nCombat - 214].iSkillLv = nLevel;
		pMaster->aStepInfo[CSP_STEP] = iCSReservPoint = pPacket->nRemain;
	}
}

void CBattleMgr::RecvCombatSelect(t_server_combat_select* pPacket)
{	//< CSD-TW-030606
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pCaster == NULL)  return;
	
	const int nCombat = pPacket->nCombat;
	// �����ϴ� ������ų���� �˻�
	if (!IsCombat(nCombat))
	{ 
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 61));
		return;
	}
	// ���� ĳ�������� ���¿����� ������ų�� ��� ���ϵ��� ����
	if (!::TimeCheck(pCaster->end_Casting))
	{
		if (Hero == pCaster)
		{ 
			OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 65));
		}

		return;
	}

	WORD wReady = 1000;
	CCombatSys* pCombat = (CCombatSys*)GetBattle(2);
	
	if (!pCombat->Ready(pCaster, nCombat, wReady))
	{
		connections.receive = 1;
		return;
	}
	
	if (pCaster == Hero)
	{ 
		switch (g_lpMT[nCombat].spell_Type)
		{
		case ACTIVE_COMBAT: 
			{
				pCaster->nActiveCombat.PutEncrypted(0);
				iCSPCNow = 0;
				iCSPCMax = g_lpMT[nCombat].nCombatCount;
				break;
			}
		case PASSIVE_COMBAT: 
			{
				pCaster->nPassiveCombat = 0;
				break;
			}
		}

		::SetSCDMainAndOtherClear(nCombat - 214);
	}
}	//> CSD-TW-030606

void CBattleMgr::RecvCombatAttack(t_server_combat_attack* pPacket)
{
	LPCHARACTER pCaster = ::ReturnCharacterPoint(pPacket->idCaster);
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pCaster == NULL || pTarget == NULL)  return;
	
	const int nCombat = pPacket->nSkill;
	pCaster->excute_MagicNum = nCombat;
	
	const int nX = pPacket->nX;
	const int nY = pPacket->nY;
	const bool bFail = pPacket->bFailed;
	CCombatSys* pCombat = (CCombatSys*)GetBattle(2);
	
	if (pCombat->Apply(pCaster, pTarget, nCombat, nX, nY, bFail))
	{	// ��� ������ų ����
		if (bFail == false)
		{
			const int nLevel = pPacket->nLevel;
			
			switch (g_lpMT[nCombat].spell_Type)
			{
			case ACTIVE_COMBAT: 
				{
					pCaster->nActiveCombat.PutEncrypted(nCombat);
					pCaster->nActiveLevel.PutEncrypted(nLevel);
					break;
				}
			case PASSIVE_COMBAT: 
				{
					pCaster->nPassiveCombat = nCombat; 
					pCaster->nPassiveLevel = nLevel;
					break;
				}
			case RECOVERY_COMBAT: 	// 030415 kyo
				{	//������ų cpȸ���� �ߵ��ɶ� 
					pCaster->nActiveCombat.PutEncrypted(0); // 030523 kyo
					pCaster->nActiveLevel.PutEncrypted(0); // 030523 kyo
					pCaster->nRecoveryCombat = nCombat; 
					pCaster->nRecoveryLevel = nLevel;
					break;
				}
			}
		}
	}
	
	if (pCaster == Hero)
	{  
		iCSPNow = pPacket->wAmount;
	}
}

void CBattleMgr::RecvCombatResult(t_server_combat_result* pPacket)
{
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;
	
	const int nCombat = pPacket->nSkill;

	SendReqHpOfParty(pTarget->name);
	
	switch (pPacket->nResult)
	{		
	case HIT_AND_NOTDEAD:
		{	
			if(pTarget != Hero)
				SendReqHpOfParty(pTarget->name);
			break;
		}	
	}
}

void CBattleMgr::RecvCombatResultH(t_server_combat_result_h* pPacket)
{
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;

	switch (pPacket->nResult)
	{		
	case HIT_AND_DEAD:
		{
			pTarget->changeActionFlag = 0;
			ShowAnimation(ANI_DEATH, pTarget);
			pTarget->pathcount= 0; // �������� �ʰ�
			pTarget->moveFlag = 0; // �׾�ƴٴ��� ���ϰ�
			pTarget->hp = 0;
			pTarget->mp = 0;
			pTarget->condition = CON_DEATH;
			pTarget->viewtype = VIEWTYPE_NORMAL_;
			// ���¾� �״� ���� �ٷ� ���� ���
			if (pTarget == Hero)
			{
				SCharacterData.nCharacterHP = 0;
				SCharacterData.nCharacterMP = 0;
				SCharacterData.condition = CON_DEATH;

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}	
			
			break;
		}	
	case HIT_AND_NOTDEAD:
		{
			if( pTarget->hp <= 0 || pTarget->condition == CON_DEATH)
			{ // �׾��ִ� �����
				break;
			}
			
			++pTarget->attacked;
			::ConditionSet(pTarget, pPacket->nState);	// ���¸� �缳��
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ��� 
			{
				SCharacterData.condition = pTarget->condition;
				SCharacterData.nCharacterHP += pPacket->nLife;
				
				if (SCharacterData.nCharacterHP <= 0) 
				{
					SCharacterData.nCharacterHP = 1;	//����������   
				}
				
				if (SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP) 
				{
					SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP;
				}	

				Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
			}
			
			SetViewType(pTarget);
			pTarget->hp += pPacket->nLife;
			if (pTarget->hp <= 0)  pTarget->hp = 1;			
			if (pTarget->hp > pTarget->hpmax)  pTarget->hp = pTarget->hpmax;
			if (pPacket->nLife < 0)  
			{
				if( pPacket->bChangeAni )	// 030415 kyo
					ShowAnimation(ANI_STROKE, pTarget);
			}
			
			break;
		}	
	}
	
	ShowNumber(pPacket->nLife, pTarget);
}

void CBattleMgr::RecvCombatResultT(t_server_combat_result_t* pPacket)
{
	LPCHARACTER pTarget = ::ReturnCharacterPoint(pPacket->idTarget);
	if (pTarget == NULL)  return;
	
	switch (pPacket->nResult)
	{		
	case HIT_AND_DEAD:
		{
			break;
		}	
	case HIT_AND_NOTDEAD:
		{
			::ConditionSet(pTarget, pPacket->nState);	// ���¸� �缳��
			
			if (pTarget == Hero) // ������ ����� �ٷ� ���� ��� 
			{
				SCharacterData.condition = pTarget->condition;
			}
			
			SetViewType(pTarget);
			break;
		}	
	}
}

extern int GetItemAttr(int item_id, int attr);

bool CBattleMgr::IsThrow() const
{
	const int nKind = ::GetItemAttr(EquipItemAttr[WT_WEAPON].item_no, ITEM_KIND);
	
	switch (nKind)
	{
    case IK_HURL:      // ��Ÿ� ���� 
    case IK_SHORT_BOW: 
	case IK_DUAL_BOW://030102 lsw
    case IK_LONG_BOW:  
    case IK_SNOW_BALL: return true;
	}
	
	return false;
}

bool CBattleMgr::IsThrow(int nEffect, bool bCast) const
{	//< CSD-031014
	switch (nEffect)
	{
    case FIRE_ARROW:
    case ICE_ARROW:
    case THROW_ARROW:
    case THROW_KNIFE:
	case THROW_FIRE_ARROW:
	case THROW_ICE_KNIFE:
	case THROW_SNOW_BALL:
	case THROW_WATER_BALL:
	case THROW_BIG_GULP:
		{
			return true;
		}
	}	

	if (!bCast)
	{
		switch (nEffect)
		{
		case MULTIPLE_FIRE:
		case FLEET_MISSILE:
		case CONVERTING_ARMOR:
		case FIRE_EXTREME:
		case ICING_BLAST:
		case SHARK_MISSILE:
		case ICE_EXTREME:  return true;
		}
	}
	
	return false;
}	//> CSD-031014

extern bool g_bWarBegin;
extern bool CheckNotClickNation();
extern bool isOurSquad(LPCHARACTER hero,LPCHARACTER ch);

bool CBattleMgr::IsAttack(LPCHARACTER pCaster, LPCHARACTER pTarget, int nMagic) const
{ //< CSD-021021
	if (pCaster == NULL || pTarget == NULL)  return false;
	if (pTarget->type != SPRITETYPE_CHARACTER)  return true;
	if (g_GameInfo.lpcharacter == pTarget)  return true;
	if (pCaster->lpAttacked == pTarget)  return true;
	if (!IsThrow(nMagic, false))  return true;
	//> CSD-021021
	bool bResult = false;
	
	if (g_FightMapStart) /* ���� �ʿ��� ���� �ٸ� ���ϰ�� ��Ʈ�� Ű ���̵� ������ �����ϰ�...*/
	{
		if (pCaster->fight_map_team_num  && pTarget->fight_map_team_num && 
			pCaster->fight_map_team_num != pTarget->fight_map_team_num)  
		{
			bResult = true;
		}
		else if (MapNumber == 81) //LMS Arena
		{
			bResult = true;
		}
	}
	else if (pCaster->name_status.nation == pTarget->name_status.nation && GetBattlePeace() == PK_MODE)
	{	//< CSD-031212
		bResult = true;
	}	//> CSD-031212
#ifdef _NATION_APPLY_
	else if (pTarget->name_status.nation != pCaster->name_status.nation && pTarget->lv >= 1)
	{	//< CSD-040414
		if (!::CheckNotClickNation())
		{
			bResult = true;

			if (pCaster == Hero)
			{
				for (int i = 0; i < 6; ++i)
				{
					if (SCharacterData.party[i].ch == pTarget)
					{
						bResult = false;
						break;
					}
				}
			}

			if(pCaster == Hero && GetBattlePeace() == BATTLE_MODE && !Hero->fight_flag && !Hero->JoinLocalWar && pTarget) //Added by Eleval for Knife bug 31/05/09
			{
				bResult = false;
			}

			if (pTarget == Hero)
			{
				for (int i = 0; i < 6; ++i)
				{
					if (SCharacterData.party[i].ch == pCaster)
					{
						bResult = false;
						break;
					}
				}
			}
		}
	}	//> CSD-040414
#endif
	else if (::IsNK(pTarget) >= 5)  bResult = true;
	else if (Hero->fight_flag && Hero->fight_id == pTarget->id)  bResult = true;
	
	if (g_bWarBegin)	// �������̸�	// 011022 LTS
	{
		if (::isOurSquad(pCaster, pTarget))  bResult = false; //�������̰� ��Ÿ����̸�
		else  bResult = true;		
	}
	
	if (g_LocalWarBegin && isInWarMap())
	{ 
		if (pCaster->JoinLocalWar && pTarget->JoinLocalWar)
		{
			if (pCaster->name_status.nation == pTarget->name_status.nation)  
			{
				bResult = false;
			}
			else
			{
				bResult = true;
			}
		}
		else if (!pCaster->JoinLocalWar && !pTarget->JoinLocalWar && GetBattlePeace() == PK_MODE) //Eleval 01/06/09
		{
			bResult = true;
		}
		else if (!pCaster->JoinLocalWar && !pTarget->JoinLocalWar && GetBattlePeace() == BATTLE_MODE) //Eleval 01/06/09
		{
			bResult = false;
		}
		else
		{
			bResult = false;
		}
	}
	
	return bResult;
}

bool CBattleMgr::IsSound(LPCHARACTER pCaster) const
{
	switch (pCaster->condition)
	{
    case CON_STONE:  return false;
	}
	
	return true;
}

bool CBattleMgr::IsDelay(int nOffset) const
{
	if (m_dwDelay > 0 && m_dwDelay + nOffset > g_ClientTime)
	{
		return false;
	}
	// ���� �ð� �缳��
	m_dwDelay = g_ClientTime;
	return true;
}

bool CBattleMgr::IsLimit(LPCHARACTER pCaster) const
{
	int nCount = pCaster->aStepInfo[CSP_STEP];
	
	for (int i = LIGHTNING_BOOM; i <= WIND_EXTREME; ++i)
	{
		nCount = SCharacterData.SkillId[i - 100];
	}
	
	const int nLevel = pCaster->lv;
	return (nCount > NPC_Lev_Ref[nLevel].nCspMax) ? true:false;
}

bool CBattleMgr::IsSelect(LPCHARACTER pCaster) const
{	//< CSD-TW-030606 : ���콺�� ���õ� �� �ִ� ���� �˻�
	if (pCaster == Hero)
	{
		if (::IsSelectCombat())
		{ // ������ų�� ���õ� ���
			return true;
		}
	}
	
	if (Hero->ready_Magic == 0)
	{	// ������ �������� ���� ���
		return true;
	}
	
	return false;
}	//> CSD-TW-030606

bool CBattleMgr::IsPickUp(LPCHARACTER pCaster, LPCHARACTER pTarget) const
{ //< CSD-021128 : ���� ������ ��� ���� ��� ���ɿ���
	if (pTarget == NULL)
	{
		return false;
	}
	
	switch (pCaster->excute_MagicNum)
	{
    case RELIEF_AUTHORITY:
		{
			break;
		}
    default:
		{
			if (pTarget->viewtype == VIEWTYPE_GHOST_)
			{
				return false;
			}
			
			break;
		}
	}
	
	return true;
} //> CSD-021128

bool CBattleMgr::IsMapAttr(LPCHARACTER pTarget, int nX, int nY) const
{ //< CSD-021128 : �ʼӼ��� ���� ���� ��� ���ɿ���
	switch (pTarget->excute_MagicNum)
	{
    case FIRE_WALL:
    case ICE_WALL:
		{
			if (TileMap[nX][nY].attr_inside == 1)
			{
				return false;
			}
			
			break;
		}
    case LIGHTNING_BOLT:
    case LIGHTNING_5TILE_RADIUS:
		{
			if (TileMap[nX][nY].attr_dont == 1 || TileMap[nX][nY].attr_inside == 1)
			{
				return false;
			}
			
			break;
		}
	}
	
	return true;
} //> CSD-021128

bool CBattleMgr::IsMagic(int nNo) const
{
	return (GetIndex(nNo) == 1) ? true:false;
}

bool CBattleMgr::IsCombat(int nNo) const
{
	return (GetIndex(nNo) == 2) ? true:false;
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

int CBattleMgr::GetIndex(int nBattle) const
{
	int nIndex = 0;
	
	switch (g_lpMT[nBattle].order_Type/10)
	{      
    case 0:
    case 1:
    case 2: 
    case 3: nIndex = 1; break;
    case 6: nIndex = 2; break;
	}
	
	return nIndex;
}

void CBattleMgr::SetViewType(LPCHARACTER pTarget)
{
	if (pTarget->condition == CON_NORMAL)
	{
		pTarget->viewtype	= VIEWTYPE_NORMAL_;
		pTarget->dieani = 0;
		pTarget->blood_Set = false;
	}
	
	if (pTarget->condition&CON_SLOW)    pTarget->viewtype	= VIEWTYPE_SLOW_;
	if (pTarget->condition&CON_POISON)  pTarget->viewtype	= VIEWTYPE_POISON_;
	if (pTarget->condition&CON_CURSE)   pTarget->viewtype	= VIEWTYPE_CURSE_;
	if (pTarget->condition&CON_DAZE)    pTarget->viewtype	= VIEWTYPE_CONFUSION_;
	if (pTarget->condition&CON_FROZEN)  pTarget->viewtype	= VIEWTYPE_FROZEN_;
	if (pTarget->condition&CON_STONE)   pTarget->viewtype	= VIEWTYPE_STONE_;
}

void CBattleMgr::ShowTaticsInfo(int nKind, int nStep, DWORD dwExperience)
{
	if (nKind >= 0 && nKind < 13 )
	{	
		const int nLevel = SCharacterData.SkillId[TACTICS_Crapple + nKind];
		Hero->aStepInfo[TAC_INDEX + nKind] = nStep;
		SCharacterData.TacticsExp[nKind] = dwExperience;
		SCharacterData.TacticsExpNext[nKind] = NPC_Lev_Ref[nLevel].nMaxExp;
		SCharacterData.SkillId[TACTICS_Crapple + nKind] = ::GetLeveByExp(nStep, dwExperience);
	}
}

void CBattleMgr::ShowAnimation(int nType, LPCHARACTER pTarget)
{
	switch (nType)
	{
    case ANI_NONE:
		{
			switch (pTarget->animationType)
			{
			case ANIMATION_TYPE_MAN:
			case ANIMATION_TYPE_WOMAN:
				{
					::CharDoAni(pTarget, pTarget->direction, ACTION_NONE);
					break;
				}
			case ANIMATION_TYPE_MON1:  
				{
					::CharDoAni( pTarget, pTarget->direction, MON1_NONE);
					break;
				}
			}
			
			break;
		}
    case ANI_STROKE: // �´� ���ϸ��̼��� ���
		{
			switch (pTarget->animationType)
			{
			case ANIMATION_TYPE_MAN:
			case ANIMATION_TYPE_WOMAN: 
				{
					if (pTarget->nPassiveCombat)
					{
						CCombatSys* pCombat = (CCombatSys*)GetBattle(2);
						pCombat->Effect(NULL, pTarget);
					}
					
					if (!pTarget->nActiveCombat.GetDecrypted())
					{
						::CharDoAni(pTarget, pTarget->direction, ACTION_ATTACKED); 
					}
					
					break;
				}
			case ANIMATION_TYPE_MON1:
				{
					switch (pTarget->nAttr)
					{ //< CSD-031027
					case AT_FIRE:
					case AT_ICE:
					case AT_LIGHTING:
					case AT_UNDEAD:
					case AT_PIERCING:
					case AT_ANTIMAGIC:
					case AT_BOSS:
					case AT_SEMIBOSS:
						{
							break;
						}
					default:
						{
							::CharDoAni( pTarget, pTarget->direction, MON1_ATTACKED);	 
							break;
						}
					} //> CSD-031027
					
					break;
				}
			}
			
			break;
		}
    case ANI_DEATH: // �״� ���ϸ��̼��� ���
		{
			switch (pTarget->animationType)		
			{
			case ANIMATION_TYPE_MAN:
			case ANIMATION_TYPE_WOMAN: 
				{
					::CharDoAni(pTarget, pTarget->direction, ACTION_DIE); 
					break;
				}
			case ANIMATION_TYPE_MON1:  
				{
					::CharDoAni(pTarget, pTarget->direction, MON1_DIE);
					break;
				}
			}
			
			break;
		}
    case ANI_STONE:
		{
			switch (pTarget->animationType)		
			{
			case ANIMATION_TYPE_MAN:
			case ANIMATION_TYPE_WOMAN: 
				{
					CCombatSys* pCombat = (CCombatSys*)GetBattle(2);
					pCombat->Effect(NULL, pTarget);
					break;
				}
			}
			
			break;
		}
	}
}

void CBattleMgr::ShowNumber(int nLife, LPCHARACTER pTarget)
{
	SendReqHpOfParty(pTarget->name);
	if (nLife < 0)
	{
		::AddNumberAni(NUMBER_ANIMATION_TYPE_DOWN_, 
			pTarget->y + 1 - pTarget->height,
			pTarget->x, pTarget->y - 1 - pTarget->height, abs(nLife));
	}
	
	if (nLife > 0)	
	{
		::AddNumberAni(NUMBER_ANIMATION_TYPE_UP_, 
			pTarget->y + 1 - pTarget->height, 
			pTarget->x, pTarget->y - 1 - pTarget->height, abs(nLife));
	}
}

void CBattleMgr::ShowEffectItem(WORD wdEffect, LPCHARACTER pCaster, LPCHARACTER pTarget)
{	//< CSD-031007
	if (wdEffect <= 0)
	{
		return;
	}
	
	::InsertMagic(pCaster, pTarget, wdEffect, 0, pTarget->x, pTarget->y, pTarget->x, pTarget->y);
}	//> CSD-031007

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

bool CBattleMgr::IsIgnoreBattle() const
{	//< CSD-TW-030618
	return (!g_bIsActive) ? false:true;
}	//> CSD-TW-030618

bool CBattleMgr::IsPlaceMagic(BYTE nMagic, int nX, int nY) const
{	//< CSD-040223
	if (::IsHarmfulMagic(nMagic) && TileMap[Hero->x>>5][Hero->y>>5].attr_no_battle)
	{
		return false;
	}

	return true;
}	//> CSD-040223