// ArenaManager.cpp: implementation of the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArenaManager.h"

#include "Network.h"
#include "Menu.h"
#include "CharDataTable.h"
#include "CurrentMsgMgr.h"
#include "TeamBattle.h"
#include "Map.h"

extern char FightTeam[5][13];
extern int fight_map_live_man[4];
extern void OpenFightMapSignMenuOpen(int tema[]);

///////////////////////////////////////////////////////////////////////////////
// Global Member
///////////////////////////////////////////////////////////////////////////////

CArenaManager g_mgrArena;
CArenaManager* g_pArenaManager = &g_mgrArena;

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaLobby::CArenaLobby()
{
	m_ltMember.clear();
}

CArenaLobby::~CArenaLobby()
{
	ClearMember();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaLobby::AddMember(WORD idMember)
{
	m_ltMember.push_back(idMember);
}

void CArenaLobby::DelMember(WORD idMember)
{
	m_ltMember.remove(idMember);
}

void CArenaLobby::ClearMember()
{
	m_ltMember.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaTeam::CArenaTeam(int nTeam) : m_nTeamNumber(nTeam)
{
	m_ltMember.clear();
	m_ltGambler.clear();
	m_ltObserve.clear();
	m_dwBattingMoney = 0;
	m_fTeamProbability = 0.0;
}

CArenaTeam::~CArenaTeam()
{
	ClearMember();
	ClearGambler();
	ClearObserve();
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaTeam::AddMember(WORD idMember)
{
	m_ltMember.push_back(idMember);
}

void CArenaTeam::DelMember(WORD idMember)
{
	m_ltMember.remove(idMember);
}

void CArenaTeam::ClearMember()
{
	m_ltMember.clear();
}

void CArenaTeam::AddGambler(WORD idGambler)
{
	m_ltGambler.push_back(idGambler);
}

void CArenaTeam::DelGambler(WORD idGambler)
{
	m_ltGambler.remove(idGambler);
}
	
void CArenaTeam::ClearGambler()
{
	m_ltGambler.clear();
}

void CArenaTeam::AddObserve(WORD idObserve)
{
	m_ltObserve.push_back(idObserve);
}

void CArenaTeam::DelObserve(WORD idObserve)
{
	m_ltObserve.remove(idObserve);
}
	
void CArenaTeam::ClearObserve()
{
	m_ltObserve.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CArenaManager::CArenaManager()
{
	Clear();
}

CArenaManager::~CArenaManager()
{
	Reset();

	for_each(m_vtTeam.begin(), m_vtTeam.end(), TDelete());
	m_vtTeam.clear();

	if (m_pLobby != NULL)
	{
		delete m_pLobby;
		m_pLobby = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CArenaManager::Clear()
{
	m_pLobby = new CArenaLobby;

	m_vtTeam.clear();
	m_vtTeam.reserve(2);
	m_vtTeam.push_back(new CArenaTeam(0));
	m_vtTeam.push_back(new CArenaTeam(1));

	m_nSelectGame = 0;
	m_idLeader = 0;

	m_dwBattingMoney = 0;
	m_dwDividendMoney = 0;
	m_dwReadyTime = 0;
	m_nObserveTeam = ATI_RED_TEAM;
	m_nGamblingTeam = ATI_RED_TEAM;
}

void CArenaManager::Reset()
{
	m_pLobby->ClearMember();
	
	for (ITOR_TEAM i = m_vtTeam.begin(); i != m_vtTeam.end(); ++i)
	{
		(*i)->ClearMember();
		(*i)->ClearGambler();
		(*i)->ClearObserve();
		(*i)->SetBattingMoney(0);
		(*i)->SetTeamProbability(0.0);
	}
	// 선택된 아레나 경기장 초기화
	m_nSelectGame = 0;
	m_idLeader = 0;

	m_dwBattingMoney = 0;
	m_dwReadyTime = 0;
	m_dwDividendMoney = 0;
	m_nObserveTeam = ATI_RED_TEAM;
	m_nGamblingTeam = ATI_RED_TEAM;
}

void CArenaManager::SendEnterLobby(int nGame)
{
	t_packet packet;
	packet.h.header.type = CMD_ARENA_ENTER_LOBBY;
	packet.h.header.size = sizeof(t_arena_enter_lobby);
	packet.u.arena.arena_enter_lobby.nArenaGame = nGame;
	::QueuePacket(&packet, 1);
	// 아레나 경기장 선택
	m_nSelectGame = nGame;
}

void CArenaManager::SendLeaveLobby()
{
	::CallServer(CMD_ARENA_LEAVE_LOBBY);
}

void CArenaManager::SendEnterTeam(int nTeam)
{
	t_packet packet;		
	packet.h.header.type = CMD_ARENA_ENTER_TEAM;
	packet.h.header.size = sizeof(t_arena_enter_team);
	packet.u.arena.arena_enter_team.nArenaGame = m_nSelectGame;
	packet.u.arena.arena_enter_team.nArenaTeam = nTeam;
	::QueuePacket(&packet, 1);
}

void CArenaManager::SendLeaveTeam()
{
	t_packet packet;		
	packet.h.header.type = CMD_ARENA_LEAVE_TEAM;
	packet.h.header.size = sizeof(t_arena_leave_team);
	packet.u.arena.arena_leave_team.nArenaGame = m_nSelectGame;
	::QueuePacket(&packet, 1);
}

void CArenaManager::SendReadyGame(WORD idMember)
{	//< CSD-TW-030618
	LPCHARACTER pMember = ::ReturnCharacterPoint(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	if (IsLeader(idMember))
	{
		t_packet packet;
		packet.h.header.type = CMD_ARENA_READY_GAME;
		packet.h.header.size = sizeof(t_arena_ready_game);
		packet.u.arena.arena_ready_game.dwDelay = TIME_READY;
		::QueuePacket(&packet, 1);
	}
}	//> CSD-TW-030618

void CArenaManager::SendEnterObserve(int nTeam)
{
	t_packet packet;		
	packet.h.header.type = CMD_ARENA_ENTER_OBSERVE;
	packet.h.header.size = sizeof(t_arena_enter_observe);
	packet.u.arena.arena_enter_observe.nArenaGame = m_nSelectGame;
	packet.u.arena.arena_enter_observe.nArenaTeam = nTeam;
	::QueuePacket(&packet, 1);

	m_nObserveTeam = nTeam;
}

void CArenaManager::SendEnterGambling(int nTeam, DWORD dwMoney)
{
	t_packet packet;		
	packet.h.header.type = CMD_ARENA_ENTER_GAMBLING;
	packet.h.header.size = sizeof(t_arena_enter_gambling);
	packet.u.arena.arena_enter_gambling.nArenaGame = m_nSelectGame;
	packet.u.arena.arena_enter_gambling.nArenaTeam = nTeam;
	packet.u.arena.arena_enter_gambling.dwBattingMoney = dwMoney;
	::QueuePacket(&packet, 1);

	m_nGamblingTeam = nTeam;
}

void CArenaManager::RecvAddLobby(t_arena_add_lobby* pPacket)
{	
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idMember = pPacket->idMember;
	LPCHARACTER pMember = ::ReturnCharacterPoint(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	if (m_pLobby->IsExistMember(idMember))
	{
		return;
	}
	
	m_pLobby->AddMember(idMember);
}

void CArenaManager::RecvDeleteLobby(t_arena_delete_lobby* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idMember = pPacket->idMember;
	LPCHARACTER pMember = ::ReturnCharacterPoint(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	if (!m_pLobby->IsExistMember(idMember))
	{
		return;
	}

	m_pLobby->DelMember(idMember);
}

void CArenaManager::RecvAddTeam(t_arena_add_team* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const DWORD idMember = pPacket->idMember;
	LPCHARACTER pMember = ::ReturnCharacterPoint(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	const int nTeam = pPacket->nTeam;

	if (nTeam < 0 || nTeam > 1)
	{
		return;
	}

	if (m_vtTeam[nTeam]->IsExistMember(idMember))
	{
		return;
	}

	m_vtTeam[nTeam]->AddMember(idMember);

	::AddCurrentStatusMessage(220, 220, 
		                      0, 
		                      lan->OutputMessage(3,44), 
							  pMember->name, 
							  FightTeam[nTeam]);
	::OpenFightMapSignMenuOpen(fight_map_live_man);
}
	
void CArenaManager::RecvDeleteTeam(t_arena_delete_team* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idMember = pPacket->idMember;
	LPCHARACTER pMember = ::ReturnCharacterPoint(idMember);
	
	if (pMember == NULL)
	{
		return;
	}

	const int nTeam = pPacket->nTeam;

	if (nTeam < 0 || nTeam > 1)
	{
		return;
	}

	if (!m_vtTeam[nTeam]->IsExistMember(idMember))
	{
		return;
	}

	pMember->fight_map_team_num = 0;
	m_vtTeam[nTeam]->DelMember(idMember);

	::AddCurrentStatusMessage(200, 50, 
		                      50, 
							  lan->OutputMessage(3,45), 
							  pMember->name, 
							  FightTeam[nTeam]);
	
	if (IsLeader(idMember))
	{
		m_idLeader = 0;
	}

	::OpenFightMapSignMenuOpen(fight_map_live_man);
}

void CArenaManager::RecvAddGambling(t_arena_add_gambling* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idGambler = pPacket->idGambler;
	LPCHARACTER pGambler = ::ReturnCharacterPoint(pPacket->idGambler);
	
	if (pGambler == NULL)
	{
		return;
	}

	const int nTeam = pPacket->nTeam;

	if (nTeam < 0 || nTeam > 1)
	{
		return;
	}

	if (m_vtTeam[nTeam]->IsExistGambler(idGambler))
	{
		return;
	}

	m_vtTeam[nTeam]->AddGambler(idGambler);
}

void CArenaManager::RecvDeleteGambling(t_arena_delete_gambling* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idGambler = pPacket->idGambler;
	LPCHARACTER pGambler = ::ReturnCharacterPoint(idGambler);
	
	if (pGambler == NULL)
	{
		return;
	}

	const int nTeam = pPacket->nTeam;

	if (nTeam < 0 || nTeam > 1)
	{
		return;
	}

	if (!m_vtTeam[nTeam]->IsExistGambler(idGambler))
	{
		return;
	}

	m_vtTeam[nTeam]->DelGambler(idGambler);
}

void CArenaManager::RecvAddObserve(t_arena_add_observe* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idObserve = pPacket->idObserve;
	LPCHARACTER pObserve = ::ReturnCharacterPoint(pPacket->idObserve);
	
	if (pObserve == NULL)
	{
		return;
	}

	const int nTeam = pPacket->nTeam;

	if (nTeam < 0 || nTeam > 1)
	{
		return;
	}

	if (m_vtTeam[nTeam]->IsExistObserve(idObserve))
	{
		return;
	}

	m_vtTeam[nTeam]->AddObserve(idObserve);
}
	
void CArenaManager::RecvDeleteObserve(t_arena_delete_observe* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idObserve = pPacket->idObserve;
	LPCHARACTER pObserve = ::ReturnCharacterPoint(pPacket->idObserve);
	
	if (pObserve == NULL)
	{
		return;
	}

	const int nTeam = pPacket->nTeam;

	if (nTeam < 0 || nTeam > 1)
	{
		return;
	}

	if (!m_vtTeam[nTeam]->IsExistObserve(idObserve))
	{	//< CSD-030522
		return;
	}	//> CSD-030522

	m_vtTeam[nTeam]->DelObserve(idObserve);
}

void CArenaManager::RecvLeaderInfo(t_arena_leader_info* pPacket)
{	

	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idLeader = pPacket->idLeader;
	LPCHARACTER pLeader = ::ReturnCharacterPoint(idLeader);
	
	if (pLeader == NULL)
	{
		return;
	}
	// 리더 설정
	m_idLeader = idLeader;
}

void CArenaManager::RecvProbabilityInfo(t_arena_probability_info* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const int nTeam = pPacket->nArenaTeam;
	
	if (nTeam < 0 || nTeam > 1)
	{
		return;
	}

	m_vtTeam[nTeam]->SetTeamProbability(pPacket->fProbability);
}

void CArenaManager::RecvGamblingInfo(t_arena_gambling_info* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const int nTeam = pPacket->nArenaTeam;
	
	if (nTeam < 0 || nTeam > 1)
	{
		return;
	}

	m_vtTeam[nTeam]->SetBattingMoney(pPacket->dwBattingMoney);
}

void CArenaManager::RecvDividendInfo(t_arena_dividend_info* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	m_dwBattingMoney = pPacket->dwBattingMoney;
	m_dwDividendMoney = pPacket->dwDividendMoney;
}

void CArenaManager::RecvReadyInfo(t_arena_ready_info* pPacket)
{
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	
	m_dwReadyTime = pPacket->dwReadyTime;
	
	if (IsReadyGame())
	{	//< CSD-031106
		MP3(SN_FIGHT_TIME);
	}	//> CSD-031106
}

void CArenaManager::RecvGameInfo(t_arena_game_info* pPacket)
{	//< CSD-TW-030701
	const char* pMapName = ::GetMapNameByPort(pPacket->wPort);
	
	if (pMapName == NULL)
	{
		return;
	}
	
	const int nCount = pPacket->wMemberCount;
	const char* pLeader = pPacket->szLeader;
	// [%s] : 총[%d]명	    방장 : [%s]
	::Kein_PutMessage(KM_INFO, kein_GetMenuString(151), pMapName, nCount, pLeader);
}	//> CSD-TW-030701

void CArenaManager::RecvStartGame(t_arena_start_game* pPacket)
{	//< CSD-TW-030618
	if (!IsSelectGame())
	{
		OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 114));
		return;
	}

	const WORD idLeader = pPacket->idLeader;
	LPCHARACTER pLeader = ::ReturnCharacterPoint(idLeader);
	
	if (pLeader == NULL)
	{
		return;
	}
	
	::CallServer(CMD_ARENA_START_GAME);
	// 리더 초기화
	m_idLeader = 0;
}	//> CSD-TW-030618

void CArenaManager::RecvGamblingResult(t_arena_gambling_result* pPacket)
{
	switch (pPacket->nPayment)
	{
    case 0:	
		{
			::AddCurrentStatusMessage(255, 180, 190, 
				                      lan->OutputMessage(1, 118), pPacket->dwMoney);
			break;
		}
	case 1:	
		{
			::AddCurrentStatusMessage(255, 180, 190, 
				                      lan->OutputMessage(1, 120), pPacket->dwMoney);
			break;
		}
	default:
		break;
	}
}

void CArenaManager::RecvArenaGameMessage1(t_arena_game_message1* pPacket)
{	//< CSD-030523
	const int nTeamNumber = pPacket->nTeamNumber;
	const int nMsgType = pPacket->nMsgType;
	const int nGameData = pPacket->nGameData;

	switch (nMsgType)
	{
	case 1: 
		{
			AddCurrentStatusMessage(220, 220, 0, lan->OutputMessage(3,41), FightTeam[nTeamNumber], nGameData); //010215 lsw
			fight_map_live_man[nTeamNumber] = nGameData;
			::OpenFightMapSignMenuOpen(fight_map_live_man);
			break;
		}
	case 2:
		{
			AddCurrentStatusMessage(220, 220, 0, kein_GetMenuString(159), FightTeam[nTeamNumber], nGameData); //010215 lsw
			fight_map_live_man[nTeamNumber] = nGameData;
			::OpenFightMapSignMenuOpen(fight_map_live_man);
			break;
		}
	case 6:
		{
			fight_map_live_man[nTeamNumber] = nGameData;
			::OpenFightMapSignMenuOpen(fight_map_live_man);
			break;
		}
	default:
		break;
	}
}	//> CSD-030523

void CArenaManager::RecvArenaGameMessage2(t_arena_game_message2* pPacket)
{
	const int nTeamNumber1 = pPacket->nTeamNumber1;
	const int nTeamNumber2 = pPacket->nTeamNumber2;
	
	char* pTeamMember1 = pPacket->szTeamMember1;
	char* pTeamMember2 = pPacket->szTeamMember2;
	// %s(%s)님이 %s(%s)님에게 공격 당해 죽었습니다.
	MP3( SN_KILL_WHO );
	Kein_PutMessage(KM_FAIL, kein_GetMenuString(160),
				    pTeamMember2, FightTeam[nTeamNumber2], 
					pTeamMember1, FightTeam[nTeamNumber1]);
}

void CArenaManager::RecvArenaGameMessage3(t_arena_game_message3* pPacket)
{
	const int nTeamNumber = pPacket->nTeamNumber;
	const int nMsgType = pPacket->nMsgType;
	const int nStoneNumber = pPacket->nStoneNumber;
	char* pTeamMember = pPacket->szTeamMember;

	switch (nMsgType)
	{
	case 4:
		{
			int r, g, b;
			GetRGBByTeam(nTeamNumber, r, g, b);
			AddCurrentStatusMessage(r, g, b, 
				                    kein_GetMenuString(161), 
									pTeamMember, 
									FightTeam[nTeamNumber]);
			break;
		}
	case 5:
		{
			int r, g, b;
			GetRGBByTeam(nTeamNumber, r, g, b);
			AddCurrentStatusMessage(r, g, b, 
				                    kein_GetMenuString(162), 
									FightTeam[nTeamNumber], 
									pTeamMember, 
									nStoneNumber);
			break;
		}
	default:
		break;
	}
}

void CArenaManager::RecvArenaGameMessage4(t_arena_game_message4* pPacket)
{	//< CSD-TW-030627
	const DWORD dwMinute = pPacket->dwRemainMinute;
	OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 121), dwMinute);
}	//> CSD-TW-030627

CArenaTeam* CArenaManager::GetArenaTeam(int nTeam)
{
	if (nTeam < 0 || nTeam > 1)
	{
		return NULL;
	}

	return m_vtTeam[nTeam];
}

CArenaTeam* CArenaManager::GetMemberTeam(WORD idMember)
{
	for (ITOR_TEAM i = m_vtTeam.begin(); i != m_vtTeam.end(); ++i)
	{
		CArenaTeam* pTeam = *i;

		if (pTeam->IsExistMember(idMember))
		{
			return pTeam;
		}
	}

	return NULL;
}

CArenaTeam* CArenaManager::GetObserveTeam(WORD idObserve)
{
	for (ITOR_TEAM i = m_vtTeam.begin(); i != m_vtTeam.end(); ++i)
	{
		CArenaTeam* pTeam = *i;

		if (pTeam->IsExistObserve(idObserve))
		{
			return pTeam;
		}
	}

	return NULL;
}

int CArenaManager::GetGamblingTeam(WORD idGambler)
{	//< CSD-030521
	for (ITOR_TEAM i = m_vtTeam.begin(); i != m_vtTeam.end(); ++i)
	{
		CArenaTeam* pTeam = *i;

		if (pTeam->IsExistMember(idGambler))
		{
			m_nGamblingTeam = pTeam->GetTeamNumber();
			return m_nGamblingTeam;
		}

		if (pTeam->IsExistObserve(idGambler))
		{
			m_nGamblingTeam = pTeam->GetTeamNumber();
			return m_nGamblingTeam;
		}
	}

	return m_nGamblingTeam;
}	//> CSD-030521

bool CArenaManager::IsEnableGambling(WORD idGambler)
{
	for (ITOR_TEAM i = m_vtTeam.begin(); i != m_vtTeam.end(); ++i)
	{
		if ((*i)->IsExistMember(idGambler))
		{
			return false;
		}
	}

	return true;
}

bool CArenaManager::IsEnableObserve(WORD idGambler)
{
	for (ITOR_TEAM i = m_vtTeam.begin(); i != m_vtTeam.end(); ++i)
	{
		if ((*i)->IsExistMember(idGambler))
		{
			return false;
		}
	}

	return true;
}
