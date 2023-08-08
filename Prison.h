/**
 * @file	Prison.h.
 *			interface for the CPrison class.
 */
#pragma once

/**
 * @class	CPrison.
 * @brief	감옥 시스템 class
 *			감옥 시스템을 작동 시키기 위한 class.
 */
//< LTH-040727-KO.
class CPrison  
{
public:
	/**
	 * @def		enum _ePRISON_SYS_RESULT.
	 * @brief	감옥 시스템 처리의 결과 상수.
	 */
	//< LTH-040727-KO.
	typedef enum _ePRISON_SYS_RESULT
	{
		PSR_ERROR						= 0,	/**< 결과값 에러. */
		PSR_ALREADY_REGISTERED_BAD_USER	= 1,	/**< 이미 불량 유저로 등록되어 있을 경우. */
		PSR_MOVE_TO_PRISON				= 2,	/**< 감옥으로 이동 한다. */
		PSR_NOT_BAD_USER				= 3,	/**< 감옥에 갇힌 유저가 아니다 */
		PSR_MOVE_TO_VILLAGE				= 4,	/**< 메인 마을로 이동한다. */
		PSR_NOT_MOVE					= 5,	/**< 감옥에 있을때에는 이동 할 수 없다. */
		PSR_MAX							= 6,	/**< 결과값 최대 개수. */
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
	 * @brief	현재 유저는 불량 유저인가?.
	 * @return	bool. true이면 불량 유저!.
	 */
	//< LTH-040806-KO.
	bool IsBadUser() 
	{ 
		return m_bIsBadUser; 
	}	//> LTH-040806-KO.

	/**
	 * @fn		CPrison::SetBadUser().
	 * @brief	현재 유저를 불량 유저인가 아닌가 세팅.
	 * @param	true이면 불량 유저로 세팅.
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
