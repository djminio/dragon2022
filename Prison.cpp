/**
 * @file	Prison.cpp.
 *			implementation of the CPrison class.
 */
#include "stdafx.h"
#include "network.h"
#include "Prison.h"
#include "Menu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern LPCHARACTER g_pHero;
extern void AddCurrentStatusMessage( const int R, const int G, const int B, char *msg, ... );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrison::CPrison()
: m_bIsBadUser(false)
{

}

CPrison::~CPrison()
{

}

/**
 * @fn		CPrison::BadUserImprison().
 * @brief	불량 유저를 감옥으로 보내는 패킷 전송 (C -> MS).
 * @param	const char* szCharName. 캐릭터 이름.
 * @return	void.
 */
//< LTH-040727-KO.

void CPrison::BadUserImprison(const char* szCharName)
{
	t_packet packet;
	ZeroMemory(&packet, sizeof(packet));
	CCharacter c;
	packet.h.header.type = CMD_BAD_USER_IMPRISON;
	strcpy(packet.u.kein.tagBadUserInfo.szName, szCharName);
	packet.h.header.size = sizeof(t_BadUserInfo);
	::QueuePacket(&packet, 1);

	char szTmpName[MAX_NAME_CHARACTER]={0,};

	//LPCHARACTER ch;
	ExistHe(szTmpName);

	if (strcmp(szTmpName, szCharName) == 0)
		m_bIsBadUser = true;
}	//> LTH-040727-KO.

/**
 * @fn		CPrison::RecvPrisonSystemResult().
 * @brief	감옥 시스템 처리에 대한 결과를 전송 받아 메시지 처리.
 * @param	t_packet* tagpPacket. 패킷변수.
 * @return	void.
 */
//< LTH-040805-KO.
void CPrison::RecvPrisonSystemResult(t_packet *tagpPacket)
{
	CPrison::ePRISON_SYS_RESULT ePrisonSysResult = (CPrison::ePRISON_SYS_RESULT)(tagpPacket->u.kein.tagPrisonSysProcResult.nReason);

	switch(ePrisonSysResult)
	{
	case CPrison::PSR_ALREADY_REGISTERED_BAD_USER:
		{
			::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0,631));		// "이미 불량 유저로 등록되어 있습니다."
		}
		break;

	case CPrison::PSR_MOVE_TO_PRISON:
		{
			::AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0,632));		// "유저님은 게임 규정에 어긋나는 행동을 하여 감옥으로 이동합니다."
			m_bIsBadUser = true;
		}
		break;

	case CPrison::PSR_NOT_BAD_USER:
		{
			::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0, 637));		// "감옥에 갇혀있는 유저가 아닙니다."
		}
		break;

	case CPrison::PSR_MOVE_TO_VILLAGE:
		{
			::AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 634));		// "감옥에서 풀려나 마을로 이동합니다."
			m_bIsBadUser = false;
		}
		break;

	case CPrison::PSR_NOT_MOVE:
		{
			::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0, 659));		// "감옥에서는 할 수 없는 일입니다"
		}
		break;
	case CPrison::PSR_CANNOT_USE:
	{
		::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0, 660));		// "감옥에서는 할 수 없는 일입니다"
	}
	break;
	default:
		{
			::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0,649));		// "감옥 시스템 처리 에러!!."
		}
		break;
	}
}	//> LTH-040805-KO

/**
 * @fn		CPrison::BadUserImprison().
 * @brief	불량 유저를 감옥으로 보내는 패킷 전송 (C -> MS).
 * @param	const char* szCharName. 캐릭터 이름.
 * @return	void.
 */
//< LTH-040805-KO.
void CPrison::BadUserRelease(const char* szCharName)
{
	t_packet packet;
	ZeroMemory(&packet, sizeof(packet));

	packet.h.header.type = CMD_BAD_USER_RELEASE;
	strcpy(packet.u.kein.tagBadUserInfo.szName, szCharName);
	packet.h.header.size = sizeof(t_BadUserInfo);
	::QueuePacket(&packet, 1);

	char szTmpName[MAX_NAME_CHARACTER]={0,};
	//g_pHero->GetName(szTmpName);
	ExistHe(szTmpName);
	if (strcmp(szTmpName, szCharName) == 0)
		m_bIsBadUser = false;
}	//> LTH-040805-KO.

/**
 * @fn		CPrison::SendUpdateImprisonTerm().
 * @brief	감옥에 수감되어 있는 기간을 갱신한다.
 * @return	void.
 */
//< LTH-040810-KO.
void CPrison::SendUpdateImprisonTerm()
{
	t_packet packet;

	packet.h.header.type = CMD_UPDATE_IMPRISON_TERM;
	packet.h.header.size = 0;
	::QueuePacket(&packet, 1);
}	//> LTH-040810-KO.