
#ifndef _CScriptTimer_H_
#define _CScriptTimer_H_

#include "network.h"
#include <string>
using namespace std;

//script에 사용될 enum
enum SCRIPT
{
	SCRIPT_COUNTER_TYPE__NPCKILL = 0,
	SCRIPT_COUNTER_TYPE__ITEMCOUNT = 1,
};

///////CLASS CScriptTimer
class CScriptTimer
{
	//서버와 달리 클라이언트는 화면에 일정 타이머를 뿌려주기만 하면 된다.
	//그래서 id랑 name이 필요가 없다.

public:
	CScriptTimer(){ClearTimer();};
	~CScriptTimer(){};

private:
	DWORD	m_dwTimerTime;		//할당된 타임
	DWORD	m_dwSpendTime;		//사용된 시간. 동기화를 위해
	bool	m_bStart;			//타이머가 시동되었는가?
	DWORD	m_dwSynchTime;		//얼마만의 시간마다 동기화 하는가? 아니면 이 시기마다 어떤 함수를 실행할것인가?
	bool	m_bShow;

public:
	DWORD	m_dwOld;			//카운터 시작했을때 값 저장
	DWORD	m_dwSynch;			

	void	SetTimerTime( DWORD dwTime);				//타이머의 시간을 설정한다. 
	void	SetSynchroneTime( DWORD dwSyncTime );		//동기화 하는 시간을 설정

	DWORD	GetCurretTimerTime() const { return m_dwTimerTime;};			//현재 남은 시간을 알아본다.
	DWORD	GetCurretSynchTime() const { return m_dwSynchTime;};		//설정한 동기화 시간을 알아본다.
	DWORD	GetSpendTime() const { return m_dwSpendTime;};
	bool	GetShowMenu() const { return m_bShow; };

	bool	IsRunning();				//진행중인가?
	bool	IsStart() const { return m_bStart;};
	void	ClearTimer();								//타이머 값들을 전부 0으로
	void	FormatTimer( const DWORD& dwTime, char* szTime );
	bool	ConfirmSyncTimer( DWORD dwRecvTime );		//동기화 시간이랑 현 사용시간이랑 비교

	void	SendSCRIPT_TIEMER( DWORD dwSpendTime , int iType);		//싱크륾 맞추기 위한 패킷을 보낸다.
	void	RecvSCRIPT_TIMER_SYNC( t_script_timer *tp );//동기화 패킷을 받을때.
	
	bool	StartTimer();				//시간이 설정되어 있으면 m_bStart = true한다.
	void	RunTimer();					//계쏙해서 실행
	void	EndTimer();
	void	ShowTimer( char* szMsg);	//타이머 쁘려줌..
	void	ShowTimerMenu();
	void	HideTimerMenu();
};


///////CLASS CScriptCounter
class CScriptCounter
{
public:
	CScriptCounter(){};
	~CScriptCounter(){};

private:
	int		m_iNum;
	int		m_iType;
	char	m_szType[21];
	int		m_iMax;
	int		m_iCounter;
	bool	m_bStart;
	bool	m_bShow;
	int		m_iFlag;			// 030108 클라이언트 카운터가 끝이 났다는것을 알린다

public:
	void	SetNum( const int num ) { m_iNum = num;};
	void	SetType( const int type );// { m_iType = type; if( m_iType ==};
	void	SetMax( const int max ) { m_iMax = max;};
	void	SetCounter( const int con) { m_iCounter = con;};
	void	SetFlag( const int flag) { m_iFlag = flag;};

	int		GetNum() const { return m_iNum;};
	int		GetType() const { return m_iType;};
	int		GetMax() const { return m_iMax;};
	int		GetCounter() const { return m_iCounter;};
	bool	GetShowMenu() const { return m_bShow; };

	void	ClearCounter(){ m_iNum=0; m_iType=0; m_iMax=0; m_iCounter=0; m_bStart= false; m_szType[0] = '\0'; m_iFlag=0;};
	void	ShowCounter( char* szMsg);

	bool	StartCounter();
	void	AddCounter(){ m_iCounter++;};
	void	EndCounter();
	void	ShowCountMenu();
	void	HideCountMenu();
	void	SendSyncCounter(int iCountNum);	// 030108 //서버에게 현재의 카운트값을 보낸다. 
};


//////CLASS CQuestMgr
class CQuestMgr
{
public:
	CQuestMgr(){};
	~CQuestMgr(){};


//script timer관련
	CScriptTimer	*m_cTimer;
	void	InitScriptTimer( const DWORD& dwTimerTime, const DWORD& dwSyncTime);
	void 	RunScriptTimer( char *szMsg);	//돌면서 메뉴에 뿌릴 글자를 만든다.
	void	DeleteScriptTimer();

//script counter관련
	CScriptCounter	*m_cCounter;
	void	InitScriptCounter( t_script_counter *tp );//const int& iNum, const int& iType, const int& iMuch);
	void	RunScriptCounter( char *szMsg);//돌면서 메뉴에 뿌릴 글자를 만든다.
	void	SyncScriptCounter( t_script_counter *tp );	//monster kill일때
	void	CheckItemCount();							//item search 일때
	void	DeleteScriptCounter();

//spellmapmove관련
	string	m_szSpell;
  void	SetSpellWord( const char* szWord ){ m_szSpell.assign( szWord ); };	//치틑 문자를 모두 기억해라.
	void	EmptySpellWord();													//주문 내용을 맵이동 할때 지워라	
	void	CheckSpellWord( t_script_spellmapmove *spell);						//주문이 맞는지 확인해서 맵에게 결과 보냄
	void	RecvSpellWord_Suc( char* szMap );
	void	RecvSpellWord_Fail();
	void	SendSpellWord(const t_script_spellmapmove *spell);
};

extern CQuestMgr	g_cQuest;

#endif//	_CScriptTimer_H_