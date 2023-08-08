#if !defined(AFX_READARGUMENT_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_)
#define AFX_READARGUMENT_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dragon.h"
#import ".\thidchk.dll" rename("EOF", "EndOfFile")
#import ".\ClientLogon.dll" no_namespace
#include "chigamec.h"
#ifdef JAPAN_LOCALIZING_
#import ".\NgcEncrypt.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#endif

//2001/02/17 zhh ����: ���̾� �������� �����ϴ� �Լ�. ��ũ���� Desdll.lib��� ȭ���� �߰����� ������ link error�߻���.
BOOL WINAPI DecryptFunc(LPTSTR cipher_text, LPTSTR plain_text );

const int MAX_BILL_BUFFER = 255;
const int MAX_TID = 50;

const int MAX_BILL_CORP	= 16;//2001/02/17 zhh ����: ���� ������ �ϰ� �ִ� ��ü�� ��. ��ü�� �߰��ɶ� ���� �÷��ش�.

//2001/02/17 zhh ����: WhatLineUse�� ���¹�ȣ. ���� ���������� +1�ؼ� logintable�� ����ִ´�. 
//logintable�� 0�� ��ü �����̴�.
enum ePayCompanyType
{
	Nownuri			=0,
	Thrunet			=1,//ip�� üũ�Ѵ�.( Ŭ�󸮾�Ʈ ���� ���� )
	Channel_i		=2,
	Unitel			=3,
	Netsgo			=4,
	KornetWorld		=5,
	Mezzy			=6,
	Wever			=7,
	WiseTop			=8,
	MediaWeb		=9,
	PcBangDotCom	=10,
	OneGame			=11,
	ExciteGame		=12,
	UnitelWeb		=13,//�������̱� ������ ���� ���� ó��
	Chollian		=14,//õ����
	HyunJuComputer	=15,//���� ��ǻ��
};

extern HINSTANCE hDll;  //���������� ����� ó��
extern SessionParameter	m_SessParam;

//Unitel
#include <process.h>

#define MAX_USERID				25
#define MAX_USERNAME			30

#define UM_CLOSENOTIFY			(WM_USER + 1052)
#define UM_TERMINATENOTIFY		(WM_USER + 1053)

#define STRING_TERMINATEEVENT	"THREAD_THERMINATEEVENT"

extern "C" typedef HWND			(WINAPI *FnGetHWNDUnimain)(void);
extern "C" typedef LPCTSTR		(WINAPI *FnUNIGetUserID)(void);
extern "C" typedef LPCTSTR		(WINAPI *FnUNIGetUserName)(void);
extern "C" typedef int			(WINAPI *FnUNIGetLoginStatus)(void);

struct STRUCTCHECKUNIPARAM
{
	HWND hWndReceiveWindow; 
	UINT nSendToMessage;
};

class CUnicheck
{
public:
	char	UnitelIP[40];
	BOOL	Uniwin_RunningStatus;
	char	Uniwin_szpUserID[100];
	char	Uniwin_szpUserName[100];

	HWND	Uniwin_m_hWnd;

	BOOL	Uniwin_StartCheckUniwin(HWND hReceiveWindow, UINT nReceiveMessage);		//������ üũ�� ������ ����
	void	Uniwin_TerminateCheckUniwin();	//������ üũ�� ������ ����.
	void	Uniwin_GetUniwinInfo();		//���������� ���� ���̵� ��Ÿ������ ���´�.
	void	Uniwin_Init(HWND hwnd);		//�������� ���ִ��� Ȯ���ϰ� ������ �� ���� ���ڰ����� �Ѿ�� ���̵�� ���� ����������
										//�α����� ���̵� �ٸ��� ���α׷� ����.
	char*	ReturnUniteIP(char *Name);
	CUnicheck();
	~CUnicheck();
};

enum
{
	OP_REQUESTPID,
	OP_LOGONSUCCESSED,		// Logon Successed
	OP_LOGONFAIL,			// Logon Fail
	OP_PIDNOTFOUND,			// PID Not Found
	OP_LIDNOTFOUND,			// LID Not Found
	OP_DUPLICATIONERROR,		// �ߺ� Error
	OP_SERVERCONNECTFAIL,// GameServer Connection Error
	OP_INCORRECTPWD,		// Password �ҷ�
	OP_INCORRECTVERSION,		// ���� �ҷ�
	OP_OVERUSER,			// ����� �ʰ�
	OP_NOMONEY,			// ��� �̳� �����
	OP_BADBOY,				// �ſ� �ҷ���
	OP_OVEREXPIRE,			// �������� ���Ⱓ �ʰ�
	OP_SOCKETTIMEOUT,
	OP_SOCKETCLOSED,
	OP_BANNED_ACCOUNT
};

#ifndef __CClinetBill__
#define __CClinetBill__
class CClientBill
{
public:
	CClientBill(const char *szLine);
	CClientBill();
	~CClientBill();

public:
	int		CloseSocket();				//������������ �����ִ� ������ ��ȯ
	char*	Unitel_ID();
	int		ReturnLineNum();			//��� ������ ����ϰ� �ִ��� WhatLineIUse�� ��ȯ�Ѵ�.
	char*	ReturnCharValue(int Where);
	int		ReturnNumValue(int Where);
	int		ReturnHeader();

	char*	ReturnUserID();				//������ ��ü�� �α��� �� ID
	char*	ReturnUserPW();				//������ ��ü�� �α��� �� PW

	void	DisplayAllCommand();		//������
	void	HeartBeatToOtherSocket();	//������ ��ü������ ���� ���Ͽ��� �����ð����� �޽����� ������.
	void	HeartBeatToGameSocket();	//���� ���ӿ����� �������Ͽ��� �����հ����� �޽����� ������.

	char*	ReturnIPaddress();			//����Ʈ���� ������ ��ȣ
	char*	ReturnPayIPaddress();		//����Ʈ���� ������ ��ȣ(���� ����Ʈ ���̷� ���ο� ������ ��� ����ϴ� ���̶�� 
										//����Ʈ ������ �ּҸ� ��ȯ�ϰ� ReturnIPaddress()�� ���� ���Ӽ����� ip�� ��ȯ�Ѵ�.
	int		ReturnPORTNumber();			//����Ʈ���� ��Ʈ ��ȣ
	int		ReturnPayPORTNumber();		//������ ��ü�� port��ȯ
	int		SendSpecifiedMessage(HWND hwnd);		//�ش��ü�� ����Ʈ���̼����� ������ �� � Ư���� ������ �����ֱ⸦ ���Ұ�� ������ �Լ�
	
#ifdef JAPAN_LOCALIZING_
	char*	GetJapanTID(){return m_szJapanTID;}//�Ϻ������� �ѱ� ID�� �н����� ����(��ȣȭ �Ǿ� ����)
#endif

private:

#ifdef JAPAN_LOCALIZING_
	INgcEncryptPtr m_pNgcEncrypto;
	char	m_szJapanTID[MAX_TID];//�Ϻ������� �ѱ� ID�� �н����� ����(��ȣȭ �Ǿ� ����)
#endif
	ILogInPtr pClient;	//Excite Game�� ���� ��

	int		IP_pos[MAX_BILL_CORP];	//Command[]�� ip�� ��� �ִ� ��ġ
	int		Port_pos[MAX_BILL_CORP];	//Command[]�� port�� ��� �ִ� ��ġ

	int		WhatLineIUse;		//��� ��ü�� ȸ������	-1 : �������� ���� ����̴�. -2: �̻��� ������ �Ѿ� �Դ�. ����

	short int UserIDlength;
	char	UserID[40];
	char	UserPW[40];

	char	IP[IP_LENGTH];
	int		port;

	char	ThrunetIP[40];
	int		ThrunetPort;
	SOCKET	sockfd;				//ä�ξ��̿�//���ݿ����� ������ ����

	void	initipNport();		//IP_pos , Port_pos�� �ʱ�ȭ �Ѵ�
	
	int		CommandNum;			//�Ѿ�� ���ڰ��� ����
	int		CommandNumSlash;			//�Ѿ�� ���ڰ��� ����
	char**	Command;			//Command�� �Ѿ�� �͵��� ����ų ������
	char**	CommandSlash;			//Command�� �Ѿ�� �͵��� ����ų ������
	char*	CommandLine;		//�Ѿ�� ���ڰ�
	char*	CommandLineBackup;	//strtok�� �����ϸ� CommandLine�� ���ϹǷ� �ٽ� ���󺹱� ��Ű�� ���Ѱ�

	char*	CommandLineSlash;		//�Ѿ�� ���ڰ� / �� �����Ѵ�
	char*	CommandLineSlashBackup;	//strtok�� �����ϸ� CommandLine�� ���ϹǷ� �ٽ� ���󺹱� ��Ű�� ���Ѱ�

	char	Return[MAX_BILL_BUFFER];		//���ڰ��̳� ������ �ּҵ��� �־ ��ȯ�Ѵ�.

	char	Buffer[MAX_BILL_BUFFER];
	int		Buffer_len;
	int		Buffer_pnt;
	void*	answer;

private:
	int		CheckLine();	//�ȿ� ��� �ڸǵ尡 �Ѿ� �Դ��� üũ
	int		PutLine();		//������ �ڸǵ尪�� �ִ� �κ�
	int		DecideLine();	//�־��� ���θ� �м��ؼ� � ������ ���� �ִ��� Ȯ���Ѵ�.

	//���� ��ȯ��	0:�ƴϴ� 1:�׷���.		���ο��� WhatLineIUse���� �ٲ۴�
	int		Check_NowNuri();		//�� ��������?
	int		Check_Thrunet();		//�� �η��?
	int		Check_Channel_i();		//�� ä�ξ���?
	int		Check_Unitel();			//�� ������?
	int		Check_Netsgo();			//�� ������?
	int		Check_Mezzy();			//�� ���� �ý���

	int		Check_Wever();			//�� ������?
	int		Check_WiseTop();		//�� ������ž?
	int		Check_MediaWeb();		//�� �̵����?
	int		Check_PcBangDotCom();	//�� pcbang.com?
	int		Check_Onegame();		//�� ������?
	int		Check_ExciteGame();		//�� Excite Game( HanMir )

	int		Check_UnitelWeb();		//�� ���̼� ������ ������?
	int		Check_Chollian();		//�� õ����?
	int		Check_HyunJuComputer();	//�� ���� ��ǻ��?


	char*	UnitelWeb_ID();			//������ ������ ���� ���̵� �����ϴ� �Լ�
	int		UnitelWeb_IP();			//���������� ip�� ���ϴ� �Լ�
	char*	ReturnUniteWebIP(char *Name);	//������ ���� ip�� ���ϴ� �Լ�

	int		ExciteGame_SendMessage();

	int		WiseTop_Port();			//�������� port�� ��ȯ
	int		WiseTop_IP();			//�������� ip�� ��ȯ	
	int		WiseTop_ID();

	int		Check_KornetWorld();	//�� �ڳݿ���?
	int		KornetWorld_IP();
	int		KornetWorld_Port();
	int		Kornet_SendMessage();

	int		Unitel_Port();			//�������� port�� ��ȯ
	int		Unitel_IP();			//�������� ip�� ��ȯ	
	int		Unitel_SendMessage();	//

	int		Netsgo_Port();			//�������� port�� ��ȯ
	int		Netsgo_IP();			//�������� ip�� ��ȯ	
	int		Netsgo_AtomTable();		//�������� �Ѿ�� ������ ���̺��� ��ȸ�Ѵ�
	int		Netsgo_SendMessage();	//������ �������� ��� ������ �޽���

	int		Thrunet_SendMessage();	//�η�ݿ� ���������� ���� �޽���

	int		Channel_i_StartUP();	//ä�ξ��̿��� �Ѿ�� �κ�. ���̺귯�� �ε��Ѵ�.
	int		Channel_i_SendEncryptedData();	//ä�ξ��̿��� ������ dll���� �˾Ƽ� ���̵�� ��ȣ�� �����Ѵ�.

	int		EatRearWhiteChar(char *eatit);
	int		Send(char *msg,int len);
	int		SendToPaySocket(char *msg,int len);
	int		Receive(int len);
	bool	Receive_sub( t_connection *c);

	int		MakePaySocket(int How);		//������ ��ü������ ������ �ϳ� ����.
};
#endif	//__CClinetBill__
extern CClientBill *g_pBill;
extern CUnicheck *g_unitel;
#endif // !defined(AFX_READARGUMENT_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_)