// MailMgr.h: interface for the CMailMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAILMGR_H__E18F4BAB_960A_4A80_AB39_22247C783034__INCLUDED_)
#define AFX_MAILMGR_H__E18F4BAB_960A_4A80_AB39_22247C783034__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "network.h"
#include "menuset.h"

enum eMAIL_VIEW_TYPE
{//인터페이스의 nWillDo에도 사용 됩니다.
	MVT_RECV_LIST = 0,//받은편지함
	MVT_DELETE_LIST = 1,//지운 편지함
};

class CMailMgr  
{
public:
	enum eVALUE
	{
		LIST_LINE_GAB	= 24,
	};
	
	enum eMAIL_TYPE
	{
		NORMAL_MAIL	= 1,
		GUILD_MAIL	= 2,
		GAMEMASTER_MAIL	= 3,
		
		NORMAL_MAIL_REPLY		= 101,
		GAMEMASTER_MAIL_REPLY	= 103,
	};
public:
	CMailMgr();
	virtual ~CMailMgr();
public:
	bool Send(const int iType, const char* szTarget, const char *szTitle, const char *szBody);//보내기
	void SendDelete(const int iCallType);//지우기(리스트 번호를 보냅니다.)
	void SendRepair();//현재 선택된 리스트를 복원합니다.

	void RequestBody(const int iMailIndex);//해당 메일의 내용 받기
	void RecvBody(t_packet *p);

	void SaveMailToLocal();//선택된 메일 저장 하기.

	void RecvAlert();
	void RecvSendResult(int cmd_type);
	
public:
	void SelectAll();//모두 선택하기
	void SendReqOtherPage(const int iPageFlag);//다른 페이지 요청
	void RecvMailList(t_packet *p);
	void SendReqMailList(const int eViewType, const int iPageIndex);//메일 리스트 요청

public:
	const int GetMailIndex(const int iMailListIndex);
	const bool GetMailIsRead(const int iMailListIndex);
	const int GetMailType(const int iMailListIndex);
	const char* GetMailSender(const int iMailListIndex);
	const char* GetMailTitle(const int iMailListIndex);
	const bool GetMailDate(const int iMailListIndex,const bool bIsDetail, char* szTime);
	
	const int GetMailIconNo(const int iMailType)
	{
		switch(iMailType)
		{
		case GUILD_MAIL:
			{
				return 45;
			}break;
		case GAMEMASTER_MAIL:
			{
				return 46;
			}break;
		case NORMAL_MAIL_REPLY:
			{
				return 47;
			}break;
		case GAMEMASTER_MAIL_REPLY:
			{
				return 48;
			}break;
		case NORMAL_MAIL:
		default:
			{
				return 44;
			}break;
		}
		return 0;
	}

	const bool IsRead(const int iMailListIndex);
	const bool IsSelected(const int iMailListIndex);
	void SetSelected(const int iMailListIndex, const bool bIsSelect);

	void SetInterFaceMailData(const int iMailListIndex, const bool bIsSelect , LPMAILDATA pMailData);
	
	bool CheckIndex(const int iMailListIndex)
	{
		if( 0 > iMailListIndex
		||	MAX_MAIL_PAGE <= iMailListIndex)
		{
			return false;
		}
		return true;
	}

public:
	void ClearTempData();//임시공간을 삭제 합니다.
	
	const int GetTempMailIndex();
	const int GetTempMailType();
	
	char* GetTempMailSender();
	char* GetTempMailTitle();
	bool GetTempMailDate(const bool bIsDetail,char *szTime);
	char* GetTempMailBody();

	void SetTempMailType(const int iMailType);
	void SetTempMailReceiver(const char* szName);
	
	void IncTempMailType();
	void DecTempMailType();
	
	__int32 GetNowViewPageNo(){return m_iNowViewPageNo;}

	void SetExistNotRead(const bool bIsHave){m_bIsExistNotRead = bIsHave;}
	bool GetExistNotRead()const{return m_bIsExistNotRead;}

	void Clear();

private:
	INTERFACEMAILDATA m_MailList[MAX_MAIL_PAGE];

	MAILDATA m_TempMailData;
	MAILBODY m_TempMailBody;//받고 쓸때 임시 공간
private:
	__int32 m_iNowViewType;
	__int32 m_iNowViewPageNo;

	bool m_bIsExistNotRead;
};

extern CMailMgr g_MailMgr;

extern inline bool IsMailMenuOpen();
extern void Call_MAIL_WRITE_MENU(const char* szTargetName, const bool bIsReply = false);//메일 쓰기 메뉴 호출
extern void RegistFriend(const char *szName);
//------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MAILMGR_H__E18F4BAB_960A_4A80_AB39_22247C783034__INCLUDED_)
	