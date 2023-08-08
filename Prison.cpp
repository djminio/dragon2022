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
 * @brief	�ҷ� ������ �������� ������ ��Ŷ ���� (C -> MS).
 * @param	const char* szCharName. ĳ���� �̸�.
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
 * @brief	���� �ý��� ó���� ���� ����� ���� �޾� �޽��� ó��.
 * @param	t_packet* tagpPacket. ��Ŷ����.
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
			::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0,631));		// "�̹� �ҷ� ������ ��ϵǾ� �ֽ��ϴ�."
		}
		break;

	case CPrison::PSR_MOVE_TO_PRISON:
		{
			::AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0,632));		// "�������� ���� ������ ��߳��� �ൿ�� �Ͽ� �������� �̵��մϴ�."
			m_bIsBadUser = true;
		}
		break;

	case CPrison::PSR_NOT_BAD_USER:
		{
			::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0, 637));		// "������ �����ִ� ������ �ƴմϴ�."
		}
		break;

	case CPrison::PSR_MOVE_TO_VILLAGE:
		{
			::AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0, 634));		// "�������� Ǯ���� ������ �̵��մϴ�."
			m_bIsBadUser = false;
		}
		break;

	case CPrison::PSR_NOT_MOVE:
		{
			::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0, 659));		// "���������� �� �� ���� ���Դϴ�"
		}
		break;
	case CPrison::PSR_CANNOT_USE:
	{
		::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0, 660));		// "���������� �� �� ���� ���Դϴ�"
	}
	break;
	default:
		{
			::AddCurrentStatusMessage(255, 0, 0, lan->OutputMessage(0,649));		// "���� �ý��� ó�� ����!!."
		}
		break;
	}
}	//> LTH-040805-KO

/**
 * @fn		CPrison::BadUserImprison().
 * @brief	�ҷ� ������ �������� ������ ��Ŷ ���� (C -> MS).
 * @param	const char* szCharName. ĳ���� �̸�.
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
 * @brief	������ �����Ǿ� �ִ� �Ⱓ�� �����Ѵ�.
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