#include "stdafx.h"
#include "QuestFunction.h"
#include "menu.h"
#include "CMessage.h"
#include "map.h"

CQuestMgr	g_cQuest;

//extern char* GetMapNameByMapfile( char *map_file );
extern void AddCurrentStatusMessage( const WORD wColor, char *msg, ... );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CScriptTimer

bool CScriptTimer::StartTimer()
{
	//�ð� ���� äũ�Լ� ������
	//�ð��� �����ִ°� �ϴ°�?

	if( m_bStart == true) return false;

	m_bStart = true;
	ShowTimerMenu();
	return m_bStart;
}

void CScriptTimer::EndTimer()
{
	m_bStart = false; 
	HideTimerMenu();
	ClearTimer();
}

void CScriptTimer::ClearTimer()
{
	m_dwTimerTime = 0;
	m_dwSynchTime = 0;
	m_dwSpendTime = 0;
	m_dwOld		  = 0;
	m_dwSynch     = 0;
	m_bStart	  = false;
	m_bShow		  = false;
}

bool CScriptTimer::IsRunning()
{
	if( m_bStart== true && m_dwTimerTime > 0 && m_dwSpendTime > 0 )
		return true;
	return false;
}

void CScriptTimer::SetTimerTime( DWORD dwTime )
{
	//_ASSERTE( dwTime < MAX_TIMER_TIME );
	//_ASSERTE( dwTime > 0 );
	if( dwTime > MAX_TIMER_TIME || dwTime < 0 )
		m_dwTimerTime = MAX_TIMER_TIME;
	else
		m_dwTimerTime = dwTime;
}

void CScriptTimer::SetSynchroneTime( DWORD dwSynchTime )
{
	//_ASSERTE( dwSynchTime < MAX_TIMER_TIME );
	//_ASSERTE( dwSynchTime > MIN_SYNCH_TIME );

	if( dwSynchTime < MIN_SYNCH_TIME || dwSynchTime > MAX_TIMER_TIME )
		m_dwSynchTime = MIN_SYNCH_TIME;
	else
		m_dwSynchTime = dwSynchTime;
}

void CScriptTimer::RunTimer( )	//�� �Լ��� very long�̴ϱ� .75�ʸ��� ����ȴٴ°� �����ϵ���
{
	if( m_dwOld == 0 ) 
		m_dwOld = ::timeGetTime();

	DWORD dwCurrent = ::timeGetTime();
	m_dwSpendTime = dwCurrent - m_dwOld;		//�󸶳� �ð��� �����°�?

	if( m_dwSpendTime > m_dwTimerTime )		//�ð��� �� ������ ������� ����� ������. 
	{		
		//Ŭ���� Ÿ�̸Ӱ� ������ ������ ����� �˸���.
		//������ ������� ���� ������ ����̴� ������. 
		if( m_dwOld == 0 ) SendSCRIPT_TIEMER( m_dwSpendTime, CMD_SCRIPT_TIMER_END_SUC);	
		m_dwOld = 0;
		m_dwSynch = 0;
		EndTimer();
		return;
	}
	
	//������ �ð����� (*sync)�Լ� ����
	if( (m_dwSpendTime - m_dwSynch) > m_dwSynchTime )
	{
		m_dwSynch = m_dwSpendTime;
		SendSCRIPT_TIEMER( m_dwSpendTime, CMD_SCRIPT_TIMER_SYNC);
	}
	
	//�׻� (*func)�Լ� ����
	//(*m_func)();
	//ShowTimer();
}

void CScriptTimer::RecvSCRIPT_TIMER_SYNC( t_script_timer *tp )
{
	//���� �ð��̶� Ŭ���� �ð��̶� ���ؼ� ���̰� �� �̻� ����.  
	if( !tp ) return;
	if( 0 >= tp->dwTime || m_dwTimerTime < tp->dwTime ) return;	// 030731 kyo

	if( !ConfirmSyncTimer( tp->dwTime ) )
	{		 
		m_dwSpendTime = tp->dwTime;	// 030821 kyo
	}
}

bool CScriptTimer::ConfirmSyncTimer( DWORD dwRecvTime )
{
	//dwRecvTime�̶� ����ð��̶� +-X�� �̻� ���̳��� ����ȭ ���д�..
	if( 10000 > (m_dwSpendTime-dwRecvTime) ) // 030821 kyo
	{
		return false;
	}
	return true;
}

void CScriptTimer::SendSCRIPT_TIEMER( DWORD dwSpendTime , int iType)
{
	t_packet packet;
	packet.h.header.type	= iType;
	packet.h.header.size	= sizeof( t_script_timer);
	packet.u.script_timer.dwTime = dwSpendTime;	
	QueuePacket( &packet, 1 );
}

//const char* CScriptTimer::ShowTimer()
void	CScriptTimer::ShowTimer( char* szMsg)
{
	char szCT[10];
	char szET[10];

	FormatTimer( m_dwTimerTime, szET );
	FormatTimer( m_dwSpendTime, szCT );
	sprintf( szMsg, "%s    /   %s", szCT, szET );
}

void  CScriptTimer::FormatTimer( const DWORD& dwTime, char* szTime )
{
	int iSec = dwTime/1000;
	int iMin = iSec/60;
	int iHour = iMin/60;
	if( iHour >= 100 ) iHour = 99;
	iMin = iMin % 60;
	iSec = iSec % 60;
	sprintf( szTime, "%02d:%02d:%02d", iHour, iMin, iSec);
}

void CScriptTimer::ShowTimerMenu()
{
	m_bShow = true;
	//SMenu[NM_SCRIPT_TIMMER].bActive =true;
}

void CScriptTimer::HideTimerMenu()
{
	m_bShow = false;
	//SMenu[NM_SCRIPT_TIMMER].bActive =false;
}


//CScriptTimer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CScriptCounter
void CScriptCounter::SetType( const int type )
{
	m_iType = type;
	if( m_iType == SCRIPT_COUNTER_TYPE__ITEMCOUNT)
		strcpy( m_szType, "Item Search" );
	else if( m_iType == SCRIPT_COUNTER_TYPE__NPCKILL)
		strcpy( m_szType, "Monster Kill" );
}

bool CScriptCounter::StartCounter()
{
	if( m_bStart == true) return false;

	m_bStart = true;
	ShowCountMenu();
	return m_bStart;
}

void CScriptCounter::EndCounter()
{
	m_bStart = false; 
	HideCountMenu();
	ClearCounter();
}

//char* CScriptCounter::ShowCounter()
void CScriptCounter::ShowCounter( char* szMsg)
{
	//�Ϸ����� Ȯ���ؼ� �ѷ��ش�.
	if( GetCounter() >= GetMax() )
	{
		char szEnd[20];
		strcpy( szEnd, "Complete" );
		sprintf( szMsg, "      %s", szEnd );
		// 020308 kyo Ŭ���̾�Ʈ ī����(������ī����) �� �����ٰ� �˸���.
		if( m_iFlag == 0 )
			SendSyncCounter( GetCounter() );
	}	
	else
	{
//		sprintf( szMsg, "%s    /   %s", szCT, szET );
		sprintf( szMsg, "     %03d    /     %03d", m_iCounter, m_iMax);
	}
}

void CScriptCounter::SendSyncCounter( int iCounterNum)
{//�������� Ŭ���̾�Ʈ�� ���� ī���� ���ڸ� ������.

	m_iFlag=1;	//�÷��׸� ���� �ѹ��� ������.
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_COUNTER_SYNC;
	packet.h.header.size	= sizeof( t_script_counter);
	memset( (void**)&packet.u.script_counter, 0x00, sizeof( t_script_counter));
	packet.u.script_counter.iCounter = iCounterNum;
	
	QueuePacket( &packet, 1 );
}

void CScriptCounter::ShowCountMenu()
{
	//SMenu[NM_SCRIPT_COUNTER].bActive =true;
	m_bShow = true;
}
void CScriptCounter::HideCountMenu()
{
	//SMenu[NM_SCRIPT_COUNTER].bActive =false;
	m_bShow = false;
}

//CScriptCounter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CQuestMgr

void CQuestMgr::InitScriptTimer( const DWORD& dwTimerTime, const DWORD& dwSyncTime)
{
	if( m_cTimer != NULL ) return;
	m_cTimer = new CScriptTimer;

	m_cTimer->SetTimerTime( dwTimerTime);
	m_cTimer->SetSynchroneTime( dwSyncTime);
	m_cTimer->StartTimer();
}

void CQuestMgr::RunScriptTimer( char *szMsg)
{
	if( szMsg == NULL ) return;

	g_cQuest.m_cTimer->RunTimer();
	if( !g_cQuest.m_cTimer->IsStart() )
	{//ī���Ͱ� ����ڽ��� ī���͵� ���� �����Ų��.
		DeleteScriptTimer();
		DeleteScriptCounter();
		szMsg[0]='\0';
	}
	else
		g_cQuest.m_cTimer->ShowTimer(szMsg);
}

void CQuestMgr::DeleteScriptTimer( )
{
	if( g_cQuest.m_cTimer == NULL) return;
	
	g_cQuest.m_cTimer->EndTimer();
	delete g_cQuest.m_cTimer;
	g_cQuest.m_cTimer = NULL;
}

//counter
void CQuestMgr::InitScriptCounter( t_script_counter *tp)//const int& iNum, const int& iType, const int& iMuch)
{
	if( !tp ) return;
	//if( m_cCounter != NULL ) return;
	if( m_cCounter != NULL ) return;
	m_cCounter = new CScriptCounter;
	
	m_cCounter->ClearCounter();
	m_cCounter->SetNum( tp->iNum);
	m_cCounter->SetType( tp->iType);
	m_cCounter->SetMax( tp->iMuch);

	m_cCounter->StartCounter();
}

void CQuestMgr::RunScriptCounter( char* szMsg)
{
	//_ASSERTE( szMsg != NULL );
	if( szMsg == NULL ) return;

	//������ ī���� �϶��� ������ī���͸� ����ؼ� ���� Ȯ���Ѵ�. npcŸ�̸Ӵ� �������� ���� ������ Ȯ���Ѵ�.
	if( m_cCounter->GetType() == SCRIPT_COUNTER_TYPE__ITEMCOUNT ) CheckItemCount();

	m_cCounter->ShowCounter( szMsg);
	//strcpy( szMsg, m_cCounter->ShowCounter() );
}

void CQuestMgr::SyncScriptCounter( t_script_counter *tp )
{//npc���̱� ī���Ϳ����� ���δ�. ���� ������ ����ȭ
	//_ASSERTE( m_cCounter != NULL );
	//_ASSERTE( tp != NULL );

	if( !tp || !m_cCounter) return;
	
	if( tp->iType == SCRIPT_COUNTER_TYPE__ITEMCOUNT ||
		m_cCounter->GetType() == SCRIPT_COUNTER_TYPE__ITEMCOUNT ) return;
	
	m_cCounter->SetCounter(tp->iCounter);
}

void CQuestMgr::CheckItemCount()
{
//ItemAttr	InvItemAttr[3][3][8];

	if( m_cCounter==NULL) return;
	if( m_cCounter->GetType() != SCRIPT_COUNTER_TYPE__ITEMCOUNT ) return;

	int iCounter = 0;
	for( int a=0; a<3; a++)
		for( int b=0; b<3; b++)
			for( int c=0; c<8; c++)
				if( InvItemAttr[a][b][c].item_no == m_cCounter->GetNum() )
					iCounter++;
					//m_cCounter->AddCounter();

	if( m_cCounter->GetCounter() > iCounter )	// 030110 kyo	//ī���Ͱ� ����������..
	{
		m_cCounter->SendSyncCounter( iCounter );				//�������� 
		m_cCounter->SetFlag( 0 );
	}

	m_cCounter->SetCounter( iCounter);
}

void CQuestMgr::DeleteScriptCounter()
{
	if( m_cCounter == NULL ) return;

	m_cCounter->EndCounter();
	delete m_cCounter;
	m_cCounter = NULL;
}

void CQuestMgr::CheckSpellWord( t_script_spellmapmove *spell)
{// 021128 kyo
	if( spell == NULL ) 
	{
		spell->isSuc = false;
		SendSpellWord( spell );
		return;
	}
	if( m_szSpell.size() <= 0 )
	{
		spell->isSuc = false;
		SendSpellWord( spell );
		return;
	}
	if( strcmp( m_szSpell.c_str(), spell->szSpell ) ==0 )
		spell->isSuc = true;
	else
		spell->isSuc = false;

	SendSpellWord( spell );
}

void CQuestMgr::SendSpellWord(const t_script_spellmapmove *spell)
{// 021128 kyo
	t_packet packet;
	packet.h.header.type	= CMD_SCRIPT_SPELL_MAPMOVE;
	packet.h.header.size	= sizeof( t_script_spellmapmove);
	memset( (void**)&packet.u.spell_mapmove, 0x00, sizeof( t_script_spellmapmove));
	memcpy( (void**)&packet.u.spell_mapmove, spell, sizeof( t_script_spellmapmove));
	
	QueuePacket( &packet, 1 );
}

void CQuestMgr::RecvSpellWord_Suc( char* szMap )
{// 021128 kyo
	//���� �̵��մϴ�.
	if( szMap == NULL ) return;
		
	AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, g_cMsg.GetTextMessage( 1, 0), ::GetMapNameByMapfile( szMap ) );
	//AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, g_cMsg.GetTextMessage( 1, 0), szMsg );
}

void CQuestMgr::RecvSpellWord_Fail()
{// 021128 kyo
	AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, g_cMsg.GetTextMessage( 1, 1) );
	//�ֹ�����
}

void CQuestMgr::EmptySpellWord()
{// 021206 kyo ���̵�, �α��ν� �ֹ� ����
	m_szSpell.erase( m_szSpell.begin(), m_szSpell.end());//clear();
}

//CQuestMgr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
