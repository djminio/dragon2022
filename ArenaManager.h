// ArenaManager.h: interface for the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENAMANAGER_H__D0BB1D83_A22A_410A_AC80_B02C0AE0C5EE__INCLUDED_)
#define AFX_ARENAMANAGER_H__D0BB1D83_A22A_410A_AC80_B02C0AE0C5EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
//
class CArenaLobby
{
public:
	typedef list<WORD>            LIST_MEMBER;
	typedef LIST_MEMBER::iterator ITOR_MEMBER;

public:
	CArenaLobby();
	~CArenaLobby();

public:
	void AddMember(WORD idMember);
	void DelMember(WORD idMember);
	void ClearMember();

public:
	LIST_MEMBER& GetLobbyMemberSet()
	{
		return m_ltMember;
	}

	bool IsEmptyMember() const
	{
		return m_ltMember.empty();
	}

	bool IsExistMember(WORD idMember)
	{
		ITOR_MEMBER itor = find(m_ltMember.begin(), m_ltMember.end(), idMember);
		return (itor != m_ltMember.end()) ? true:false;
	}

private:
	LIST_MEMBER m_ltMember;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaTeam
{
public:
	typedef list<WORD>            LIST_MEMBER;
	typedef LIST_MEMBER::iterator ITOR_MEMBER;

public:
	explicit CArenaTeam(int nTeam);
	~CArenaTeam();

public:
	void AddMember(WORD idMember);
	void DelMember(WORD idMember);
	void ClearMember();

	void AddGambler(WORD idGambler);
	void DelGambler(WORD idGambler);
	void ClearGambler();

	void AddObserve(WORD idObserve);
	void DelObserve(WORD idObserve);
	void ClearObserve();

public:
	void SetBattingMoney(DWORD dwMoney) { m_dwBattingMoney = dwMoney; }
	void SetTeamProbability(float fProbability) { m_fTeamProbability = fProbability; }

	int GetTeamNumber() const { return m_nTeamNumber; }
	DWORD GetBattingMoney() const { return m_dwBattingMoney; }
	float GetTeamProbability() const { return m_fTeamProbability; }

	LIST_MEMBER& GetMemberSet() { return m_ltMember; }
	LIST_MEMBER& GetGamblerSet() { return m_ltGambler; }
	LIST_MEMBER& GetObserveSet() { return m_ltObserve; }
	
	int GetMemberCount() const { return m_ltMember.size(); }
	int GetGamblerCount() const { return m_ltGambler.size(); }
	int GetObserveCount() const { return m_ltObserve.size(); }

	bool IsEmptyMember() const { return m_ltMember.empty(); }
	bool IsEmptyGambler() const { return m_ltGambler.empty(); }
	bool IsEmptyObserve() const { return m_ltObserve.empty(); }

	bool IsExistMember(WORD idMember)
	{
		ITOR_MEMBER itor = find(m_ltMember.begin(), m_ltMember.end(), idMember);
		return (itor != m_ltMember.end()) ? true:false;
	}

	bool IsExistGambler(WORD idGambler)
	{
		ITOR_MEMBER itor = find(m_ltGambler.begin(), m_ltGambler.end(), idGambler);
		return (itor != m_ltGambler.end()) ? true:false;
	}

	bool IsExistObserve(WORD idObserve)
	{
		ITOR_MEMBER itor = find(m_ltObserve.begin(), m_ltObserve.end(), idObserve);
		return (itor != m_ltObserve.end()) ? true:false;
	}

private:
	LIST_MEMBER m_ltMember;
	LIST_MEMBER m_ltGambler;
	LIST_MEMBER m_ltObserve;
	int         m_nTeamNumber;
	DWORD       m_dwBattingMoney;
	float       m_fTeamProbability;
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CArenaManager  
{
	enum {TIME_READY = 11};

public:
	typedef vector<CArenaTeam*>   VECTOR_TEAM;
	typedef VECTOR_TEAM::iterator ITOR_TEAM;

public:
	CArenaManager();
	virtual ~CArenaManager();

public:
	void Clear();
	void Reset();

	void SendEnterLobby(int nGame);
	void SendForceEnterLobby(int nServerID, int nGame);
	void SendLeaveLobby();

	void SendEnterTeam(int nTeam);
	void SendForceEnterTeam(int nServerID, int nTeam);
	void SendLeaveTeam();

	void SendReadyGame(WORD idMember);

	void SendEnterObserve(int nTeam);

	void SendEnterGambling(int nTeam, DWORD dwMoney);

	void RecvAddLobby(t_arena_add_lobby* pPacket);
	void RecvDeleteLobby(t_arena_delete_lobby* pPacket);
	void RecvAddTeam(t_arena_add_team* pPacket);
	void RecvDeleteTeam(t_arena_delete_team* pPacket);
	void RecvAddGambling(t_arena_add_gambling* pPacket);
	void RecvDeleteGambling(t_arena_delete_gambling* pPacket);
	void RecvAddObserve(t_arena_add_observe* pPacket);
	void RecvDeleteObserve(t_arena_delete_observe* pPacket);

	void RecvStartGame(t_arena_start_game* pPacket);

	void RecvLeaderInfo(t_arena_leader_info* pPacket);
	void RecvProbabilityInfo(t_arena_probability_info* pPacket);
	void RecvGamblingInfo(t_arena_gambling_info* pPacket);
	void RecvDividendInfo(t_arena_dividend_info* pPacket);
	void RecvReadyInfo(t_arena_ready_info* pPacket);
	void RecvGameInfo(t_arena_game_info* pPacket); // CSD-TW-030701

	void RecvGamblingResult(t_arena_gambling_result* pPacket);

	void RecvArenaGameMessage1(t_arena_game_message1* pPacket);
	void RecvArenaGameMessage2(t_arena_game_message2* pPacket);
	void RecvArenaGameMessage3(t_arena_game_message3* pPacket);
	void RecvArenaGameMessage4(t_arena_game_message4* pPacket); // CSD-TW-030627

	CArenaTeam* GetArenaTeam(int nTeam);
	
	CArenaTeam* GetMemberTeam(WORD idMember);
	CArenaTeam* GetObserveTeam(WORD idObserve);

	int GetGamblingTeam(WORD idGambler);

	bool IsEnableGambling(WORD idGambler);
	bool IsEnableObserve(WORD idGambler);

public:
	CArenaLobby* GetLobbyInfo()
	{
		return m_pLobby;
	}

	VECTOR_TEAM& GetTeamInfo()
	{
		return m_vtTeam;
	}

	void SetLeader(WORD idLeader) { m_idLeader = idLeader; }
	void SetObserveTeam(int nTeam) { m_nObserveTeam = nTeam; }
	void SetGamblingTeam(int nTeam) { m_nGamblingTeam = nTeam; }

	int GetObserveTeam() const { return m_nObserveTeam; }

	DWORD GetBattingMoney() const { return m_dwBattingMoney; }
	DWORD GetDividendMoney() const { return m_dwDividendMoney; }
	DWORD GetReadyTime() const { return m_dwReadyTime; }

	DWORD GetBattingMoney(int nTeam) const
	{
		if (nTeam < 0 || nTeam > 1)
		{
			return 0;
		}

		return m_vtTeam[nTeam]->GetBattingMoney();
	}

	float GetTeamProbability(int nTeam) const
	{
		if (nTeam < 0 || nTeam > 1)
		{
			return 0;
		}

		return m_vtTeam[nTeam]->GetTeamProbability();
	}

	bool IsLeader(WORD idMember) const
	{
		return (m_idLeader == idMember) ? true:false;
	}

	bool IsReadyGame() const
	{
		return (m_dwReadyTime > 0 && m_dwReadyTime < TIME_READY) ? true:false;
	}

private:
	bool IsSelectGame() const
	{
		return (m_nSelectGame <= 0) ? false:true;
	}

protected:
	CArenaLobby*  m_pLobby;
	VECTOR_TEAM   m_vtTeam;
	WORD          m_idLeader;

private:
	int           m_nSelectGame;
	DWORD         m_dwBattingMoney;
	DWORD         m_dwDividendMoney;
	DWORD         m_dwReadyTime;
	int           m_nObserveTeam;
	int           m_nGamblingTeam;
};
//
///////////////////////////////////////////////////////////////////////////////
extern CArenaManager* g_pArenaManager;

#endif // !defined(AFX_ARENAMANAGER_H__D0BB1D83_A22A_410A_AC80_B02C0AE0C5EE__INCLUDED_)
