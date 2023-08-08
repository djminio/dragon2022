/**
 * @file	Prison.h.
 *			interface for the CPrison class.
 */
#pragma once

/**
 * @class	CPrison.
 * @brief	���� �ý��� class
 *			���� �ý����� �۵� ��Ű�� ���� class.
 */
//< LTH-040727-KO.
class CPrison  
{
public:
	/**
	 * @def		enum _ePRISON_SYS_RESULT.
	 * @brief	���� �ý��� ó���� ��� ���.
	 */
	//< LTH-040727-KO.
	typedef enum _ePRISON_SYS_RESULT
	{
		PSR_ERROR						= 0,	/**< ����� ����. */
		PSR_ALREADY_REGISTERED_BAD_USER	= 1,	/**< �̹� �ҷ� ������ ��ϵǾ� ���� ���. */
		PSR_MOVE_TO_PRISON				= 2,	/**< �������� �̵� �Ѵ�. */
		PSR_NOT_BAD_USER				= 3,	/**< ������ ���� ������ �ƴϴ� */
		PSR_MOVE_TO_VILLAGE				= 4,	/**< ���� ������ �̵��Ѵ�. */
		PSR_NOT_MOVE					= 5,	/**< ������ ���������� �̵� �� �� ����. */
		PSR_MAX							= 6,	/**< ����� �ִ� ����. */
		PSR_CANNOT_USE					 = 7,
	} ePRISON_SYS_RESULT;
	//> LTH-040727-KO.
	
public:
	void SendUpdateImprisonTerm();
	void BadUserRelease(const char* szCharName);
	void BadUserImprison(const char* szCharName);
	void RecvPrisonSystemResult(t_packet* tagpPacket);
	/**
	 * @fn		CPrison::IsBadUser().
	 * @brief	���� ������ �ҷ� �����ΰ�?.
	 * @return	bool. true�̸� �ҷ� ����!.
	 */
	//< LTH-040806-KO.
	bool IsBadUser() 
	{ 
		return m_bIsBadUser; 
	}	//> LTH-040806-KO.

	/**
	 * @fn		CPrison::SetBadUser().
	 * @brief	���� ������ �ҷ� �����ΰ� �ƴѰ� ����.
	 * @param	true�̸� �ҷ� ������ ����.
	 * @return	void.
	 */
	//< LTH-040806-KO.
	void SetBadUser(const bool bIsBadUser)
	{
		m_bIsBadUser = bIsBadUser;
	}	//> LTH-040806-KO.

public:
	CPrison();
	virtual ~CPrison();

protected:
	bool m_bIsBadUser;
};
//> LTH-040727-KO.
