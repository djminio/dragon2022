#include "Stdafx.h"


#include <stdio.h>
#include "hong_sub.h"
#include "CReadArgument.h"
//#include "chigamec.h"

HINSTANCE hDll;  //���������� ����� ó��
SessionParameter	m_SessParam;
CClientBill *g_pBill;

CUnicheck *g_unitel;
//2001/02/19 zhh ����: ���ڰ��� �״�� ip�� port�� �� �� �ִ� ��� �� ���ڰ��� Command[]������ ��ġ.
//2001/02/20 zhh ����: ������ onegame�� ��� dragonraja.exe /moya IP Port ��� ���ڷ� ����ǹǷ�,
//2001/02/20 zhh ����: IP_pos[11]  = 1;	Port_pos[11]= 2; �� �ȴ�.
void	CClientBill::initipNport()
{
	int i;
	for(i=0;i<MAX_BILL_CORP;i++)	IP_pos[i]=-1;
	for(	i=0;i<MAX_BILL_CORP;i++)	Port_pos[i]=-1;	//Command[]�� port�� ��� �ִ� ��ġ

	::memset(IP,0,IP_LENGTH);
	port=0;

	sockfd=0;

	//nownuri
	IP_pos[0]  = 1;
	Port_pos[0]= 2;

	//Mezzy System
	IP_pos[6]  = 1;
	Port_pos[6]= 2;

	//WeverNet
	IP_pos[7]  = 1;
	Port_pos[7]= 2;

	IP_pos[10]  = 1;
	Port_pos[10]= 2;

	IP_pos[11]  = 1;
	Port_pos[11]= 2;

	IP_pos[14]  = 1;
	Port_pos[14]= 2;

	IP_pos[15]  = 1;
	Port_pos[15]= 2;
}

CClientBill::CClientBill(const char *szLine)
{
	initipNport();					//Ŭ���� ������ ������E�ʱ�ȭ.
	g_unitel=NULL;					//������ ��E�EŬ���� �ʱ�ȭ.
	CommandLine=NULL;
	Command=NULL;					//Command�� �Ѿ��� �͵��� ����ų ������

	CommandLineSlash=NULL;			//�Ѿ��� ���ڰ� / �� �����Ѵ�
	CommandLineSlashBackup=NULL;	//strtok�� �����ϸ�ECommandszLine�� ���ϹǷ� �ٽ� ���󺹱� ��Ű��E���Ѱ�

	CommandLineBackup = NULL;

	WhatLineIUse=-1;					//���� ��ü�� ȸ������E����. -1�̸�E�������� �ƴ϶�� ��E
	CommandNum=0;					//�Ѿ��� ���ڰ��� ����
	CommandNumSlash=0;			//�Ѿ��� ���ڰ��� ����

#ifdef	JAPAN_LOCALIZING_ //030207 lsw
	CoInitialize(NULL);
	HRESULT hr = m_pNgcEncrypto.CreateInstance(__uuidof(NgcEncrypt));
	if(FAILED(hr))
	{
		::MessageBox(NULL,"NGC init error ", "Error",0);		
	}
/*
	WCHAR* wszJapanTID= NULL;
	_bstr_t code = L"D819FA2C-4429-4511-AC6E-9823F743881F ";
	m_pNgcEncrypto->Encode(code, &wszJapanTID);
*/
//
//	::MessageBox(NULL,szLine,"In",0);
	WCHAR wszLine[100] = {0,};//�ƽ�Ű�� ���ڵ�� ��ȯ
	::MultiByteToWideChar(CP_ACP, 0, szLine, strlen(szLine), wszLine, sizeof(WCHAR)*100);

	WCHAR* result= NULL;
	m_pNgcEncrypto->Decode( wszLine, &result);
//	MessageBox(NULL,(char*)result,"Decode1",0);

	::WideCharToMultiByte(CP_ACP, 0, result,-1, m_szJapanTID, sizeof(m_szJapanTID), NULL, NULL);
//	MessageBox(NULL,m_szJapanTID,"Decode2",0);
	m_pNgcEncrypto = NULL;

	WhatLineIUse=-1;
	return;
#endif

	const int len = lstrlen(szLine)+1;	//���ڰ��� ��� �ϴ��� üũ�� �Ѵ�.
	if(len>-1)
	{
		CommandLine				= new char[len];
		CommandLineBackup		= new char[len];		//2001/02/19 zhh ����: strtok�� ��� ���� ������ �ٲپ� �����Ƿ� ����� �ʿ��ϴ�.
		CommandLineSlash		= new char[len];			//�Ѿ�� ���ڰ� / �� �����Ѵ�
		CommandLineSlashBackup	= new char[len];	//strtok�� �����ϸ� CommandszLine�� ���ϹǷ� �ٽ� ���󺹱� ��Ű�� ���Ѱ�
		::strcpy(CommandLine,szLine);
		::strcpy(CommandLineBackup,szLine);
		::strcpy(CommandLineSlash,szLine);
		::strcpy(CommandLineSlashBackup,szLine);
		CheckLine();	//� ���ڰ��� �Ѿ� �Դ��� Ȯ���ϰ�
		PutLine();		//�װ� �߶� Command[]�� �ִ´�
		DecideLine();	//�Ѿ�� ���ڰ��� �ش��ϴ� ��ü�� �մ��� Ȯ���Ѵ�.
	}
	else
	{
		WhatLineIUse=-1;	//-1�̸� �������� ���� ����̴�. �������� �ƴϴ�
	}
}

CClientBill::CClientBill()		//�׳� ����� ������ ��. ���� �ʴ´�.
{
	CommandLine=NULL;	
	Command=NULL;		//Command�� �Ѿ�� �͵��� ����ų ������

	WhatLineIUse=0;		//��� ��ü�� ȸ������ 
	CommandNum=0;		//�Ѿ�� ���ڰ��� ����
}
//2001/02/19 zhh ����: new�� ���� �͵� delete.
CClientBill::~CClientBill()
{
	if(Command!=NULL)
	{
		for(int i=0;i<CommandNum;i++)
			if(Command[i]!=NULL)
				delete Command[i];
	//	delete Command;
	}

	if(CommandSlash!=NULL)
	{
		for(int i=0;i<CommandNumSlash;i++)
			if(CommandSlash[i]!=NULL)
				delete CommandSlash[i];
	//	delete CommandSlash;
	}

	if(CommandLine)	//�Ѿ�� ���ڰ�
		delete CommandLine;
	if(CommandLineBackup)	//�Ѿ�� ���ڰ�
		delete CommandLineBackup;

	if(CommandLine)	//�Ѿ�� ���ڰ�
		delete CommandLineSlash;
	if(CommandLineBackup)	//�Ѿ�� ���ڰ�
		delete CommandLineSlashBackup;

	if(g_unitel)
		delete g_unitel;

	if(sockfd!=0)
		closesocket(sockfd);
}		

//2001/02/19 zhh ����: ���ڿ� �� ���� white space�� ���ش�.
int CClientBill::EatRearWhiteChar(char* eatit )
{	
	char*	szWhite = " \t\n\r";
	char*	pRear;
	
	pRear = eatit + strlen( eatit ) - 1;
	if ( pRear )
	{
		while ( eatit <= pRear )
		{
			if ( strchr( szWhite, *pRear ) )
			{
				*pRear-- = 0;
			}
			else
			{
				break;
			}
		}
	}	
	return 0;
}	

//2001/02/19 zhh ����: 
int CClientBill::CheckLine()
{	
	char *p;
	p=strtok(CommandLine," ");
	if(p)
		CommandNum++;
	while(p!=NULL)
	{
		p=strtok(NULL," ");
		if(p)	CommandNum++;
	}

	p=NULL;	
	p=strtok(CommandLineSlash,"/");
	if(p)
		CommandNumSlash++;
	while(p!=NULL)
	{
		p=strtok(NULL,"/");
		if(p)	CommandNumSlash++;
	}
	return 0;
}


int CClientBill::PutLine()
{
	strcpy(CommandLine,CommandLineBackup);
	strcpy(CommandLineSlash,CommandLineSlashBackup);
	
  if( 0 >= CommandNum)
	{
		return 0;
	}

	Command = new char* [CommandNum];
	CommandSlash = new char* [CommandNumSlash];

	int i;
	for(i=0;i<CommandNum;i++)	Command[i]=NULL;
	for(    i=0;i<CommandNumSlash;i++)	CommandSlash[i]=NULL;

	i=0;
	char *p;
	p=strtok(CommandLine," ");
	if(p)
	{
		Command[i] = new char [lstrlen(p)+1];
		strcpy(Command[i],p);
		i++;		
	}
	while(p!=NULL)
	{
		p=strtok(NULL," ");
		if(p && strcmp(p," "))
		{
			Command[i] = new char [lstrlen(p)+1];
			strcpy(Command[i],p);
			i++;			
		}
	}

	i=0;
	p=NULL;
	p=strtok(CommandLineSlash,"/");
	if(p)
	{
		CommandSlash[i] = new char [lstrlen(p)+1];
		EatRearWhiteChar(p);
		strcpy(CommandSlash[i],p);
		i++;		
	}
	while(p!=NULL)
	{
		p=strtok(NULL,"/");
		if(p/* && strcmp(p," ")*/)
		{
			CommandSlash[i] = new char [lstrlen(p)+1];
			EatRearWhiteChar(p);
			strcpy(CommandSlash[i],p);
			i++;			
		}
	}
	return 0;
}

char*	CClientBill::ReturnUserID()				//������ ��ü�� �α��� �� ID	//���� �𸣴� ���� NowNuri��.����Ǿ����Ƿ� �����߰�.
{
	memset(Return,0,sizeof(Return));
	if(WhatLineIUse<0)	return NULL;
	switch(WhatLineIUse)
	{
	case 0:
	case 1:
	case 2:		
		return UserID;
		break;
	case 3:
		{
			memset(UserID,0,sizeof(UserID));
			strcpy(UserID,g_unitel->Uniwin_szpUserID);
		}
	case 4:
	case 5:
	case 6:
	case 7:
		return UserID;
		break;
	case 8:
		WiseTop_ID();
		return UserID;
		break;
	case 9:
	case 10:
	case 11:
	case 12:

	case 13:
	case 14:
	case 15:	
		return UserID;
		break;

	default: return NULL;
		break;
	}
	return NULL;
}

char*	CClientBill::ReturnUserPW()				//������ ��ü�� �α��� �� PW
{
	if(WhatLineIUse<0)	return NULL;
	memset(Return,0,sizeof(Return));
	return NULL;
}


int		CClientBill::ReturnLineNum()
{
//	_asm int 3; 
	if(WhatLineIUse<0)	
		return -1;
	return WhatLineIUse;
}


char*	CClientBill::ReturnCharValue(int Where)
{
	if(Where>CommandNum)	return NULL;
	memset(Return,0,sizeof(Return));
	strcpy(Return,Command[Where]);
	return Return;
}

int		CClientBill::ReturnNumValue(int Where)
{
	if(Where>CommandNum)	return -1;
	return (atoi(Command[Where]));
}
void	CClientBill::DisplayAllCommand()	//������
{
	int i;
	for(i=0;i<CommandNum;i++)
		MessageBox(NULL,Command[i],"White Space",NULL);	
	for(    i=0;i<CommandNumSlash;i++)	
		MessageBox(NULL,CommandSlash[i],"Slash",NULL);

//	if(CommandNum==0 && CommandNumSlash==0)
//		return;
	
	char ip[IP_LENGTH];
	int  port;  
	char dis[100],linename[20];
	if(ReturnLineNum()<0)	return;
	wsprintf(ip,ReturnIPaddress());
	port = ReturnPORTNumber();
	switch(WhatLineIUse)
	{
	case 0:		strcpy(linename,"Nownuri");	break;
	case 1:		strcpy(linename,"Thrunet");	break;
	case 2:		strcpy(linename,"Channel i");	break;
	case 3:		strcpy(linename,"Unitel");	break;
	case 4:		strcpy(linename,"Netsgo");	break; 
	case 5:		strcpy(linename,"Kornet World");	break; 
	case 6:		strcpy(linename,"Mezzy System");	break; 
	case 7:		strcpy(linename,"WeverNet");	break; 
	case 8:		strcpy(linename,"WiseTop");	break; 
	case 9:		strcpy(linename,"MediaWeb");	break; 
	case 10:	strcpy(linename,"Pcbang.com");	break; 
	case 11:	strcpy(linename,"One Game");	break; 
	case 12:	strcpy(linename,"Excite Game");	break; 
	case 13:	strcpy(linename,"UnitelWeb");	break; 
	case 14:	strcpy(linename,"Chollian");	break; 
	case 15:	strcpy(linename,"HyunJuComputer");	break; 

	default: strcpy(linename,"Can't Detect Line");	break;		
	}
	sprintf(dis,"ip : %s port : %d LineName:%s",ip,port,linename);
	MessageBox(NULL,dis,linename,NULL);
}
int		CClientBill::ReturnHeader()
{
	if(WhatLineIUse<0)	return 0;
	return(WhatLineIUse+710);
}

int		CClientBill::ReturnPayPORTNumber()			//����Ʈ���� ��Ʈ ��ȣ
{
	if(WhatLineIUse<0)	return 0;

	if(IP_pos[WhatLineIUse]==-1 || Port_pos[WhatLineIUse]==-1)		//���� ���ڰ����� �����ǿ� ��Ʈ�� �Ѿ���� ������
	switch(WhatLineIUse)
	{
		case 1:
		//	if(ThrunetIP==NULL)	
		//	return 9010;
			return ThrunetPort;
		//	return 5000;
		//	return g_GameInfo.port;
			break;
		case 2:
			return m_SessParam.nPortNum;
		//	return htons(m_SessParam.nPortNum);
			break;
		case 3:
			return Unitel_Port();
		case 4:
			return Netsgo_Port();
		case 5:
			return 9000;

		default:
		//	MessageBox(NULL,"Can't Detect Line","Please Ask Release Corp.",NULL);
			break;
	}
	int port  = atoi(Command[ Port_pos[WhatLineIUse] ]);
	if(port<0) port=0;
	return (port);
}


char*	CClientBill::ReturnPayIPaddress()			//����Ʈ���� ������ ��ȣ
{
	if(WhatLineIUse<0)	return NULL;
	memset(Return,0,sizeof(Return));
	if(IP_pos[WhatLineIUse]==-1 || Port_pos[WhatLineIUse]==-1)		//���� ���ڰ����� �����ǿ� ��Ʈ�� �Ѿ���� ������
	switch(WhatLineIUse)
	{
		case 1:		
	//	if(ThrunetIP==NULL)	
			{				
				memset(Return,0,sizeof(Return));
			//	strcpy(Return, g_GameInfo.server);
			//	strcpy(Return, "203.248.248.228");				
				strcpy(Return, ThrunetIP);
				return Return;
			} 
			break;
	//	return ThrunetIP;
		case 2: 
			{
			//	_asm int 3;
				strcpy(Return, m_SessParam.lpszIPAddress);
				return Return;
			}
			break;
				
		case 3:
			Unitel_IP();
			return Return;
		case 4:
			Netsgo_IP();
			return Return;
		case 5:
			KornetWorld_IP();
			//	PostQuitMessage(0);
			return Return;		
			
		default:
		//	MessageBox(NULL,"Can't Detect Line","Please Ask Release Corp.",NULL);
			break;
	}
	memset(Return,0,sizeof(Return));
	strcpy(Return, Command[ IP_pos[WhatLineIUse] ]);
	return Return;
}



char*	CClientBill::ReturnIPaddress()			//����Ʈ���� ������ ��ȣ
{
//	_asm int 3;
	if(WhatLineIUse<0)	return NULL;
	memset(Return,0,sizeof(Return));
	if(IP[0]!=0)		//�̹� �ּҰ� ������
	{
		return IP;
	}
	if(IP_pos[WhatLineIUse]==-1 || Port_pos[WhatLineIUse]==-1)		//���� ���ڰ����� �����ǿ� ��Ʈ�� �Ѿ���� ������
	switch(WhatLineIUse)
	{
		case 1:		
	//	if(ThrunetIP==NULL)	
			{				
				memset(Return,0,sizeof(Return));
			//	strcpy(Return, g_GameInfo.server);
			//	strcpy(Return, "203.248.248.228");				
				strcpy(Return, ThrunetIP);
				return Return;
			} 
			break;
	//	return ThrunetIP;
		case 2: 
			{
			//	_asm int 3;
				strcpy(Return, m_SessParam.lpszIPAddress);
				return Return;
			}
			break;
				
		case 3:
			Unitel_IP();
			return Return;
		case 4:
			Netsgo_IP();
			return Return;
		case 5:
			KornetWorld_IP();
			strcpy(IP,Return);
			//	PostQuitMessage(0);
			return IP;
		

		case 8:
			WiseTop_IP();
			strcpy(IP,Return);
			return IP;

		case 9:
			strcpy(IP,"211.174.52.137");
			return IP;		
			
		default:
		//	MessageBox(NULL,"Can't Detect Line","Please Ask Release Corp.",NULL);
			break;
	}
	memset(Return,0,sizeof(Return));
	strcpy(Return, Command[ IP_pos[WhatLineIUse] ]);
	return Return;
}

int		CClientBill::ReturnPORTNumber()			//����Ʈ���� ��Ʈ ��ȣ
{
	if(WhatLineIUse<0)	return 0;

	if(IP_pos[WhatLineIUse]==-1 || Port_pos[WhatLineIUse]==-1)		//���� ���ڰ����� �����ǿ� ��Ʈ�� �Ѿ���� ������
	switch(WhatLineIUse)
	{
		case 1:
		//	if(ThrunetIP==NULL)
		//	return 9010;
			return ThrunetPort;
		//	return 5000;
		//	return g_GameInfo.port;
			break;
		case 2:
			return m_SessParam.nPortNum;
		//	return htons(m_SessParam.nPortNum);
			break;
		case 3:
			return Unitel_Port();
		case 4:
			return Netsgo_Port();
		case 5:
			return KornetWorld_Port();

		case 8:
			return WiseTop_Port();
		case 9:
			return 9001;
		case 12:	//excite game
			return this->port;

		case 13:
			return Unitel_Port();

		default:
		//	MessageBox(NULL,"Can't Detect Line","Please Ask Release Corp.",NULL);
			break;
	}
	int port  = atoi(Command[ Port_pos[WhatLineIUse] ]);
	if(port<0) port=0;
	return (port);
}

void CheckGameSocketAndSendHeartBeat(void* ThreadParam);	//30�ʿ� �ѹ��� ���� ������ ������ ������ ���ϵ� �ݴ´�.
int		CClientBill::SendSpecifiedMessage(HWND hwnd)		//�ش��ü�� ����Ʈ���̼����� ������ �� � Ư���� ������ �����ֱ⸦ ���Ұ�� ������ �Լ�
{
//	_asm int 3;
	if(WhatLineIUse<0) return 0;	
	unsigned long hHeartBeatSensor = _beginthread( CheckGameSocketAndSendHeartBeat, NULL,(void*) 1);
	if (hHeartBeatSensor == -1)
		return -2;
	switch(WhatLineIUse)
	{
	case 0:
		break;

	case 1:
	//	MessageBox(NULL,"asdfasdf","haha",NULL);
		if(Thrunet_SendMessage()==0)
		{
			PostQuitMessage(0);
			return -1;
		}
		else			return 1;
		break;

	case 2:
		if(Channel_i_SendEncryptedData()==0)
		{
			PostQuitMessage(0);
			return -2;
		}
		else 
			return 2;
		break;
	case 3:
		g_unitel->Uniwin_Init(hwnd);
		if(Unitel_SendMessage()==0)			return -3;
		else			return 3;
		break; 

	case 4:
		if(Netsgo_SendMessage()==0)			return -4;
		else			return 4;
		break;
	case 5:		//�ڳݿ���� ������ �ϳ� ��� 20�� ���ܿ��� �޽��� �ƹ��ų� �ϳ��� ������ �ȴ�.
	//	if(MakePaySocket(2)!=1)				return -5;
		if(Kornet_SendMessage()==0)			return -5;
		else			return 5;
		break;

	case 6:
		break;

	case 12:		
		if( ExciteGame_SendMessage() < 0)
			PostQuitMessage(0);
		break;

	default:
		break;
	}
	return 0;
}

int		CClientBill::DecideLine()	//�־��� ���θ� �м��ؼ� � ������ ���� �ִ��� Ȯ���Ѵ�.
{
//	_asm int 3;
	if(FindWindow("DRAGONRAJA_CLASS","DragonRaja Online"))
	{
		WhatLineIUse = -1; 
		return 0;//�Ǵ� ����
	}
	int Now = Check_NowNuri();		//�� ��������?
	int Thr = Check_Thrunet();		//�� �η��?
	int	Cha = Check_Channel_i();	//�� ä�ξ���?
	int	Uni = Check_Unitel();		//�� ������
	int	Net = Check_Netsgo();		//�� ������?
	int Kw  = Check_KornetWorld();	//�� �ڳݿ���?
	int Mez = Check_Mezzy();
	int Wev = Check_Wever();
	int Wis = Check_WiseTop();
	int Med = Check_MediaWeb();
	int Pcb = Check_PcBangDotCom();
	int One = Check_Onegame();
	int Exc = Check_ExciteGame();
	int	UnW = Check_UnitelWeb();		//�� ���̼� ������ ������?
	int	Cho = Check_Chollian();		//�� õ����?
	int	HJC = Check_HyunJuComputer();	//�� ���� ��ǻ��?

	Uni = UnW * Uni == 1 ? 0 : Uni;

	//���� 2�� �̻��� ��ü�� ������ ���ٰ� �Ѵٸ� ����
	if( Now + Thr + Cha + Uni + Net + Kw + Mez + Wev + Wis + Med + Pcb + One + Exc + UnW + Cho + HJC >1)
	{
		WhatLineIUse = -2;
		char haha[6];
		sprintf(haha,"%d%d%d%d%d%d",Now,Thr,Cha,Uni,Net,Kw);
	//	MessageBox(NULL,haha,"Can't Decide Line",NULL);
	//	exit(0);
	}
	else if( Now + Thr + Cha + Uni + Net + Kw + Mez + Wev + Wis + Med + Pcb + One + Exc + UnW + Cho + HJC ==1)	//��� �� ��ü�� �������� ����Ѵٴ� ��
	{
		//WhatLineIUse�� ����ü���� üũ�Ҷ� �� ��ü�� ���� �ٲ۴�.�������� ������ -2�� �ٲ۴�.
		return 1;	//�Ǵ� ����
	}
	else										//�������� �ƴ϶�� ��.
	{
		WhatLineIUse = -1;
	}
	return 0;//�Ǵ� ����\
	
}
int		CClientBill::Check_UnitelWeb()		//�� ���̼� ������ ������?
{	
	char uni[7]="P1:Web";
	for(int i=0;i<CommandNum;i++)
	{
		if( strlen(Command[i]) < 6 )
			continue;
		for( int j=0;j<strlen(Command[i])-6+1;j++)
		if(!memicmp(uni,Command[i]+j,6))		//unitel�̶�� �ܾ ������
		{
			WhatLineIUse=13;					//�������̶�� ǥ��
			this->UnitelWeb_ID();				//������ ������ ���� ���̵� ����
			this->UnitelWeb_IP();
			return 1;
		} 
	}
	return 0;
}

char*		CClientBill::UnitelWeb_ID()
{
	memset(UserID,0,sizeof(UserID));
	for(int i=0;i<CommandNum;i++)
	{
		char *p=Command[i];
		for(int len=0;len<lstrlen(Command[i])-9;len++)
		{
			if(!memcmp(Command[i]+len,"&ID:",4))
			{
				int temp=0;
				while(1)
				{
					temp++;
					char haha=(char) *(Command[i]+len+temp);
					if( haha=='&')
					{	
						memset(UserID,0,sizeof(UserID));
						memcpy(UserID,Command[i]+len+4,temp-4);
						return UserID;
					}					
				}
			}
		}
	}
	return 0;
}

int		CClientBill::UnitelWeb_IP()
{
	memset(Return,0,sizeof(Return));
	for(int i=0;i<CommandNum;i++)
	{
		char *p=Command[i];
		for(int len=0;len<lstrlen(Command[i])-9;len++)
		{
			if(!memcmp(Command[i]+len,"&IP:",4))
			{
				int temp=0;
				while(1)
				{
					temp++;
					char haha=(char) *(Command[i]+len+temp);
					if( haha=='&')
					{	
					
						memset(Return,0,sizeof(Return));
						memcpy(Return,Command[i]+len+4,temp-4);					
						this->ReturnUniteWebIP(Return);
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

char* CClientBill::ReturnUniteWebIP(char *Name)
{
	WORD version = MAKEWORD( 2, 2 );  
	WSADATA		wsaData;
	WSAStartup( version, &wsaData ); // NetWork���õȰ� �ʱ�ȭ�ϴ°�..

	LPHOSTENT lpHostEnt = gethostbyname((LPCTSTR)Name);	
	DWORD ErrorCode = WSAGetLastError();
	memset(this->IP,0,IP_LENGTH);
	if (lpHostEnt)
	{
		IN_ADDR   inAddr;
		memcpy((void*)&inAddr.S_un.S_addr, lpHostEnt->h_addr,lpHostEnt->h_length);

		wsprintf(this->IP,"%d.%d.%d.%d",
			inAddr.S_un.S_un_b.s_b1, 
			inAddr.S_un.S_un_b.s_b2, 
			inAddr.S_un.S_un_b.s_b3, 
			inAddr.S_un.S_un_b.s_b4);
		return this->IP;
	}	
	return NULL;	
}

/*
int		CClientBill::Check_UnitelWeb()		//�� ���̼� ������ ������?
{
	return 0;
}
*/
int		CClientBill::Check_Chollian()		//�� õ����?
{
	if(CommandNum!=4) return 0;
	if(!stricmp(Command[0],"chollian"))
	{
		memset(this->UserID,0,sizeof(this->UserID));
		int Len = strlen(Command[3]);
		Len = Len > 40  ? 40 : Len;
		memcpy(this->UserID,Command[3],Len);
		this->WhatLineIUse = 14;
		return 1;
	}	
	return 0;
}
int		CClientBill::Check_HyunJuComputer()	//�� ���� ��ǻ��?
{
	if(CommandNum<3) return 0;
	if(!stricmp(Command[0],"hyunju"))
	{
		memset(this->UserID,0,sizeof(this->UserID));
		this->WhatLineIUse = 15;
		return 1;
	}
	return 0;	
}

int		CClientBill::Check_Onegame()
{
	if(CommandNum<3) return 0;
	if(!stricmp(Command[0],"/moya"))
	{
		memset(this->UserID,0,sizeof(this->UserID));
		this->WhatLineIUse = 11;
		return 1;
	}
	return 0;
}


int		CClientBill::Check_PcBangDotCom()
{
	if(CommandNum<3) return 0;
	if(!stricmp(Command[0],"pcbang"))
	{
		memset(this->UserID,0,sizeof(this->UserID));
		this->WhatLineIUse = 10;
		return 1;
	}
	return 0;
}

int		CClientBill::Check_MediaWeb()
{
//	_asm int 3;
	char* cmdLine = GetCommandLine();
	if(CommandNum<1) return 0;
	if(!memcmp(cmdLine,"MDC",3))		//0��° ���ڰ��� �����̰� �Ѿ�� ���ڵ��� �� ���� 5�̸�
	{
		memset(Return,0,sizeof(Return));
		if(CommandNum==1)
			strcpy(Return,Command[0]);
		else
			strcpy(Return,Command[1]);
		TCHAR para[50];
		int ret = DecryptFunc( Return, para );	// dll function
		if( ret == TRUE)
		{
			para[49]=0;
			memset(UserID,0,40);

		//	char string[] = "A string\tof ,,tokens\nand some  more tokens";
			char seps[]   = " /";
			char *token;
			int i=0;
			token = strtok( para, seps );
			while( token != NULL )
			{
				i++;
				if(i==2)
					strcpy(UserID,token);
				token = strtok( NULL, seps );
				if(i>4)	break;
			}			
			WhatLineIUse=9;					//�̵�����̶�� ǥ��
			return 1;
		}
		else
		{
		//	MessageBox(NULL,"Fail ! MDC ","",NULL);
			return 0;
		}
	}
	return 0;
}
//WiseTop&IP:211.174.52.137&PO:9001&ID:doltwo&NA:������&
//WiseTop&IP:11.11.11.11&PO:1110&ID:doltwo&NA:������&
int		CClientBill::Check_WiseTop()		//�� ������ž?
{
	char wise[11]="WiseTop&IP",temp[7]="     ";	
	for(int i=0;i<CommandNum;i++)
	{
		if(strlen(Command[i])>10)
		if(!memicmp(wise,Command[i],10))		//unitel�̶�� �ܾ ������
		{
			WhatLineIUse=8;					//������ž�̶�� ǥ��
			return 1;
		} 
	}	
	return 0;	
}

int		CClientBill::WiseTop_IP()
{
	memset(Return,0,sizeof(Return));
	for(int i=0;i<CommandNum;i++)
	{
		char *p=Command[i]; 
		for(int len=0;len<lstrlen(Command[i])-9;len++)
		{
			if(!memcmp(Command[i]+len,"&IP:",4))
			{
				int temp=0;
				while(1)
				{
					temp++;
					char haha=(char) *(Command[i]+len+temp);
					if( haha=='&')
					{
						memset(Return,0,sizeof(Return));
						memcpy(Return,Command[i]+len+4,temp-4);
						return 1;
					}					
				}
			}
		}
	}
	return 0;
}


int		CClientBill::WiseTop_Port()
{
	for(int i=0;i<CommandNum;i++)
	{
		char *p=Command[i];
		for(int len=0;len<lstrlen(Command[i])-9;len++)
		{
			if(!memcmp(Command[i]+len,"&PO:",4))
			{
				int temp=0;
				while(1)
				{
					temp++;
					char haha=(char) *(Command[i]+len+temp);
					if( haha=='&')
					{	
						char haha[20];
						memset(haha,0,20);
						memset(haha,0,sizeof(haha));
						memcpy(haha,Command[i]+len+4,temp-4);
						return(atoi(haha));
					}					
				}
			}
		}
	}
	return 0;
}

int		CClientBill::WiseTop_ID()
{
	memset(UserID,0,sizeof(UserID));
	for(int i=0;i<CommandNum;i++)
	{
		char *p=Command[i];
		for(int len=0;len<lstrlen(Command[i])-10;len++)
		{
			if(!memcmp(Command[i]+len,"&ID:",4))
			{
				int temp=0;
				while(1)
				{
					temp++;
					char haha=(char) *(Command[i]+len+temp);
					if( haha=='&')
					{	
						memset(UserID,0,sizeof(UserID));
						memcpy(UserID,Command[i]+len+4,temp-4);
						return 1;
					}					
				}
			}
		}
	}
	return 0;
}


int		CClientBill::Check_Wever()			//�� ������?
{
	if(CommandNum!=3) return 0;
	if(!stricmp(Command[0],"WeverNet"))		//0��° ���ڰ��� �����̰� �Ѿ�� ���ڵ��� �� ���� 5�̸�
	{
		memset(UserID,0,40);
	//	strcpy(UserID,Command[3]);		//�������� ��� ���̵� ���� �� ����.
		WhatLineIUse=7;					//������� ǥ��
		return 1;
	}
	return 0;	
}


int		CClientBill::Check_Mezzy()			//�� ���� �ý���
{
	if(CommandNum!=4) return 0;
	if(!stricmp(Command[0],"mezzy"))		//0��° ���ڰ��� �����̰� �Ѿ�� ���ڵ��� �� ���� 5�̸�
	{
		memset(UserID,0,40);
		strcpy(UserID,Command[3]);
		WhatLineIUse=6;					//������� ǥ��
		return 1;
	}
	return 0;	
}

//001214 zhh
int		CClientBill::Check_KornetWorld()	//�� �ڳݿ���?
{
	if(CommandNum!=5) return 0;
	if(!stricmp(Command[0],"/KWG"))		//0��° ���ڰ��� �ڳݿ����̰� �Ѿ�� ���ڵ��� �� ���� 5�̸�
	{
		memset(UserID,0,40);
		strcpy(UserID,Command[3]);
		WhatLineIUse=5;					//�ڳݿ����� ǥ��
		return 1;
	}
	return 0;	
}
//001214 zhh
int		CClientBill::KornetWorld_IP()
{
//	char host[] = "gateway3.unitel.co.kr";
	char host[] = "game.kornetworld.com";
	LPHOSTENT lpHostEnt = gethostbyname((LPCTSTR)host);
	memset(Return,0,sizeof(Return));
	if (lpHostEnt)
	{
		IN_ADDR   inAddr;
		memcpy((void*)&inAddr.S_un.S_addr, lpHostEnt->h_addr,lpHostEnt->h_length);
		
		wsprintf(Return,"%d.%d.%d.%d",
			inAddr.S_un.S_un_b.s_b1, 
			inAddr.S_un.S_un_b.s_b2, 
			inAddr.S_un.S_un_b.s_b3, 
			inAddr.S_un.S_un_b.s_b4);
//		strcpy(IP,Return);
		return 1;
	}
	else
	{
	//	WhatLineIUse = -5;
	//	PostQuitMessage(0);
		return 0;
	}
}
//001214 zhh
int		CClientBill::KornetWorld_Port()	//�ڳݿ��� ����//
{
	return 9000;
}
//001214 zhh
///kwg 10100003 103001 yskwon checksum
int		CClientBill::Kornet_SendMessage()
{
	char   szSend[256]; 
	memset(szSend,0,256);
	strcpy(&(szSend[0]), "KwGwMGS" );//��KwGwMGS��);					<= MAGIC STRING     //ù��° ����.
	strcpy(&(szSend[8]), "CONNECT" );//									<= OPERATION        //�׻� connect��� ��������?
	strcpy(&(szSend[16]),Command[1]);// ��10100003��);					<= CPIP CODE        //�ι�° ����
	strcpy(&(szSend[32]),Command[2]);// ��103004��);						<= SVC CODE         //3��° ����
	strcpy(&(szSend[48]),Command[3]);// ��yskwon��);						<= LOGINID          //4��° ����
	strcpy(&(szSend[64]),Command[4]);// ��dkskxhdkslxlkdkdsaaaasa��);	<= CHECKSUM         //5��° ����.
//	send(socketfd, packet, 256, 0);

	int len = 256;

	if(Send(szSend,len)==0)
	{
		WhatLineIUse = -5;
		PostQuitMessage(0);
		return 0;
	}
	else
	{
		return 1;
	}
	return 0;
}

int		CClientBill::Check_NowNuri()		//�� ��������?
{
	if(CommandNum!=3) return 0;
	if(!stricmp(Command[0],"/now"))		//0��° ���ڰ��� ���촩���̰� �Ѿ�� ���ڵ��� �� ���� 3�̸�
	{
		//���⿡�� ���촩���� ip�� port���� üũ�ϴ� ��ƾ�� �� ��.
		WhatLineIUse=0;					//���촩����� ǥ��
		memset(UserID,0,40);
		return 1;
	}
	return 0;
}
/*
OP_REQUESTPID,											//0
OP_LOGONSUCCESSED,		// Logon Successed				//1
OP_LOGONFAIL,			// Logon Fail					//2
OP_PIDNOTFOUND,			// PID Not Found				//3
OP_LIDNOTFOUND,			// LID Not Found				//4
OP_DUPLICATIONERROR,		// �ߺ� Error				//5
OP_SERVERCONNECTFAIL,// GameServer Connection Error		//6
OP_INCORRECTPWD,		// Password �ҷ�				//7
OP_INCORRECTVERSION,		// ���� �ҷ�				//8
OP_OVERUSER,			// ����� �ʰ�					//9
OP_NOMONEY,			// ��� �̳� �����					//10
OP_BADBOY,				// �ſ� �ҷ���					//11
OP_OVEREXPIRE,			// �������� ���Ⱓ �ʰ�		//12
OP_SOCKETTIMEOUT,										//13
OP_SOCKETCLOSED											//14
*/

int CClientBill::ExciteGame_SendMessage()
{
//	_asm int 3;
	unsigned int nRet;
	HRESULT hr;
	hr = pClient->LogIn( connections.socket, pClient->Parameter,&nRet, 10000 );
	
	if( hr == S_FALSE )	//���������� �α��� �����ϸ�
	{
	//	char dis[128]={0,};
	//	sprintf(dis,"hr == S_FALSE      nRet = %d",nRet);
	//	MessageBox(NULL,dis,"Excite Game Error",NULL);
		pClient.Release();
		::CoUninitialize();
	//	return -12;
	}	
	

	switch( nRet )
	{
	case OP_LOGONSUCCESSED:
	//	MessageBox(NULL,"ExciteGame Error", "Logon Successed" ,MB_OK);
		return 12;
		
	case OP_LOGONFAIL:
		MessageBox(NULL,"ExciteGame Error", "Logon Fail" ,MB_OK);		
		return -1;
		
	case OP_PIDNOTFOUND:
		MessageBox(NULL,"ExciteGame Error", "PID Not Found" ,MB_OK);		
		return -2;
		
	case OP_LIDNOTFOUND:
		MessageBox(NULL,"ExciteGame Error", "LID Not Found" ,MB_OK);		
		return -3;
		
	case OP_DUPLICATIONERROR:
		MessageBox(NULL,"ExciteGame Error", "�ߺ� Error" ,MB_OK);		
		return -4;
		
	case OP_SERVERCONNECTFAIL:
		MessageBox(NULL,"ExciteGame Error", "Server Not Found" ,MB_OK);		
		return -5;
		
	case OP_INCORRECTPWD:
		MessageBox(NULL,"ExciteGame Error", "Incorrect Password" ,MB_OK);
		return -6;
		
	case OP_INCORRECTVERSION:
		MessageBox(NULL,"ExciteGame Error", "Incorrect Password" ,MB_OK);		
		return -7;
	case OP_BANNED_ACCOUNT:
		MessageBox(NULL, "ExciteGame Error", "Account Banned", MB_OK);
		return -18;
	case OP_OVERUSER: 
		MessageBox(NULL,"ExciteGame Error", " " ,MB_OK);		
		return -8;
		
	case OP_NOMONEY:
		MessageBox(NULL,"ExciteGame Error", "��� �̳���." ,MB_OK);		
		return -9;
		
	case OP_BADBOY:
		MessageBox(NULL,"ExciteGame Error", " " ,MB_OK);		
		return -10;
		
	case OP_OVEREXPIRE:
		MessageBox(NULL,"ExciteGame Error", "���� �ð� �ʰ�." ,MB_OK);		
		return -11;
		
	case OP_SOCKETTIMEOUT:
		MessageBox(NULL,"ExciteGame Error", "���� Ÿ�� Out." ,MB_OK);		
		return -12;
		
	case OP_SOCKETCLOSED:
		MessageBox(NULL,"ExciteGame Error", "������ �������ϴ�. " ,MB_OK);		
		return -13;
	default:
		return -99;
	}
}

int CClientBill::Check_ExciteGame()
{
//	_asm int 3;
	if( CommandNum<1)	return 0;
	if( memicmp(Command[0],"/EGPARAM:",9) )		//���� ExciteGame���� ���ڰ����� ������ ���� �ƴϸ� 
		return 0;
	HRESULT hr;
	
	
	if( FAILED( hr = ::CoInitialize(NULL) ) )
	{
		MessageBox(NULL,"ExciteGame�� Dll�� �ε��ϴµ� �����Ͽ����ϴ�.","Excite Game Error",MB_OK );
		PostQuitMessage(0);		
		return 0;
	}

	if( FAILED( hr = this->pClient.CreateInstance( __uuidof(LogIn) ) ) )
	{
		MessageBox(NULL,"ExciteGame�� Dll�� �ε��ϴµ� �����Ͽ����ϴ�.","������Ұ� ��ġ���� �ʾҽ��ϴ�.",MB_OK );
		PostQuitMessage(0);
		::CoUninitialize();
		return 0;
	}

	char bstrCmdLine[1024];
	strcpy(bstrCmdLine,GetCommandLine());

	pClient->Parameters = (_bstr_t)Command[0];
	
	pClient->Parameters = bstrCmdLine;
	strcpy(this->IP		, _bstr_t(pClient->IP).operator const char*( ));
	strcpy(this->UserID , _bstr_t(pClient->PID).operator const char*( ));
	this->port = pClient->Port;
	

	WhatLineIUse = 12;

	return 1;
}


int		CClientBill::Check_Thrunet()		//�� �η��?
{
//	_asm int 3;
//	return 0;

	memset(ThrunetIP,0,40);
//	_asm int 3;
	char dragon[] = ".dragon";
	if(CommandNumSlash<1)
		return 0;
	if(strlen(CommandSlash[0])<8)
		return 0;	
	if(memcmp( (CommandSlash[0]+strlen(CommandSlash[0])-7),dragon,7))
		return 0;

	memset(ThrunetIP,0,40);
	ThrunetPort=0;
	char buf[100];
	memset(buf,0,100);

	FILE *fp = Fopen(CommandSlash[0],"rt");
	if(fp==NULL)
	{
		MessageBox(NULL,"Can't Open File.","Error",NULL);
		PostQuitMessage(0);
		return 0;
	}

	fscanf(fp,"%s",buf);
//	MessageBox(NULL,buf,"ip",NULL);	
	strcpy(ThrunetIP,buf);
	if(strlen(buf)<1)
	{
		PostQuitMessage(0);
		return 0;
	}

	fscanf(fp,"%s",buf);
//	MessageBox(NULL,buf,"port",NULL);
	ThrunetPort=atoi(buf);
	if(ThrunetPort<0)
	{
		PostQuitMessage(0);
		return 0;
	}
	
	fscanf(fp,"%s",buf);
	if(strcmp(buf,"thrunet"))
	{
	//	PostQuitMessage(0);
	//	return 0;		//�η���� �ƴ� �༮�̸� �߶� ������
	}
//	MessageBox(NULL,buf,"thrunet",NULL);	

	
	

	HRESULT	hr = NOERROR;
	bool	fComInitialized = false;
	if (FAILED(hr = ::CoInitialize(NULL)))
		_com_issue_error(hr);
	fComInitialized = true;
	thrunet::_clsURLCHKPtr a;
	hr=a.CreateInstance("thrunet.clsURLCHK");
	if(FAILED(hr)) 
	{		
		PostQuitMessage(0);
	//	printf("createinstance error..\n");
		return 0;
	}
	_bstr_t result;
	_bstr_t in_s;	
	in_s = "abcd";	// �ǹ̾��� ���̹Ƿ� �׳� �Ѱ� �ش�.	
	result=a->GetID(in_s);     // GedID�� ȣ���ϸ� userid�� �Ѿ�´� 	
	if(strcmp((char*)result,"*"))	//*�� ����, �ƴϸ� �������̵�
	{
		memset(UserID,0,sizeof(UserID));
		strcpy(UserID,(char*)result);
		UserIDlength = strlen((char*)result);
		WhatLineIUse=1;
	//	a.Release();
		return 1;
	}
	else
	{
		PostQuitMessage(0);
	//	a.Release();
		return 0;
	}
	
	return 0;
}


bool WaitingTheAnswerForTHTUNET( t_connection *c, short int *answer );


int		CClientBill::Check_Channel_i()		//�� ä�ξ���?
{
//	_asm int 3;
	if(CommandNum!=1) return 0;
	if(!strcmp(Command[0],"/lgi"))
	{
		if(Channel_i_StartUP()==1)			
		{
			WhatLineIUse = 2;
			return 1;
		}
		else
		{
			PostQuitMessage(0);
			WhatLineIUse = -2;					
			return 0;
		}		
	}
	return 0;
}


int		CClientBill::Channel_i_StartUP()	//ä�ξ��̿��� �Ѿ�� �κ�. ���̺귯�� �ε��Ѵ�.
{
//	_asm int 3;
	hDll= LoadLibrary("chigamec.dll");
	if(hDll == NULL){
		// Library Loading ����. ���� ó��.
		return 0;
	}

	typedef int (CALLBACK* LPFNDLLFUNC)(SessionParameter* lpSP);
	LPFNDLLFUNC lpfnDllFunc;

	lpfnDllFunc = (LPFNDLLFUNC)GetProcAddress(hDll, "WaitForSessionParameter");
	if (!lpfnDllFunc)
	{
		// DWORD dwErr = GetLastError();
		// ���� ó��
		FreeLibrary(hDll);       
		return 0;
	}
	else
	{
		lpfnDllFunc(&m_SessParam);  //Active X�� ���� SessionParameter�� ���� ���� �� ���� ����ϰ� ��.
					    //�̿��ڰ� ��Ҹ� �Ͽ������ Return�� ������ m_SessParam�� Null���� ���� ��
	}
	if(lstrlen(m_SessParam.lpszUserName)<1 || lstrlen(m_SessParam.lpszIPAddress)<1 || lstrlen(m_SessParam.lpszPassword)<1)
		return 0;		//�̿��ڰ� �ּ��� ���
	else
	{
		memset(UserID,0,sizeof(UserID));
		strcpy(UserID,m_SessParam.lpszUserName);
		return 1;		//�ּ��� ����� �� �ִ�.
	}
}

int		CClientBill::Channel_i_SendEncryptedData()	//ä�ξ��̿��� ������ dll���� �˾Ƽ� ���̵�� ��ȣ�� �����Ѵ�.
{
//	WSADATA				wsaData;
//	SOCKET				sockfd;
//	struct sockaddr_in	serv_addr;


	//
	// winsock initialization
	//
//	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
//		goto cleanup;
//	}

//	memset(&serv_addr, 0, sizeof(serv_addr));
//	serv_addr.sin_family = AF_INET;
//	serv_addr.sin_addr.s_addr = inet_addr(m_SessParam.lpszIPAddress);
//	serv_addr.sin_port = htons(m_SessParam.nPortNum);

//	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//		goto cleanup;
//	}

//	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//		goto cleanup;
//	}else{ //connect�� ���������� �ٷ� SendEncryptedData�Լ��� �ҷ��� ��.



	typedef int (CALLBACK* LPFNDLLFUNC)(LPCTSTR, LPCTSTR, SOCKET);
	LPFNDLLFUNC lpfnDllFunc;

	lpfnDllFunc = (LPFNDLLFUNC)GetProcAddress(hDll, "SendEncryptedData"); 
	if (!lpfnDllFunc)
	{
		// ���� ó��
		FreeLibrary(hDll);
		return 0;
	}
	else
	{			
		DWORD dwRet = lpfnDllFunc(m_SessParam.lpszUserName, m_SessParam.lpszPassword, connections.socket);
	//	DWORD dwRet = lpfnDllFunc(m_SessParam.lpszUserName, m_SessParam.lpszPassword, connections[curr_connect].socket);
	//	DWORD dwRet = lpfnDllFunc(m_SessParam.lpszUserName, m_SessParam.lpszPassword, sockfd);
		//Data(ID/Pwd)�� Encryption�Ͽ� MugProxy�� ����.

		if(dwRet == 0){
			return 1;
			//����
		}else{	
			int t=0;
			while(t)
			{
			//	dwRet = lpfnDllFunc(m_SessParam.lpszUserName, m_SessParam.lpszPassword, sockfd);
				dwRet = lpfnDllFunc(m_SessParam.lpszUserName, m_SessParam.lpszPassword, connections.socket);
			//	dwRet = lpfnDllFunc(m_SessParam.lpszUserName, m_SessParam.lpszPassword, connections[curr_connect].socket);
				if(dwRet==0)
					return 1;
				else
					t++;
				if(t>10)	break;
			}
			// ����!!
			// Proxy Server�� ���� Public Key, Random Seed�� �޾� Data�� Encryption�ؼ� ������ �� ������.
			// �̶����� ���α׷��� �����ϴ���, Reconnect�� �õ� �ؾ� ��.
		}

	}

//	}

//	cleanup:

	if (connections.socket)
		closesocket(connections.socket);

//	if (connections[curr_connect].socket)
//		closesocket(connections[curr_connect].socket);

	WSACleanup();
	PostQuitMessage(0);

	return 0;
}

int		CClientBill::Check_Unitel()		//�� ������?
{
	char uni[10]="Unitel&IP",temp[7]="     ";
	for(int i=0;i<CommandNum;i++)
	{
		if(!memcmp(uni,Command[i],9))		//unitel�̶�� �ܾ ������
		{
			WhatLineIUse=3;					//�������̶�� ǥ��
			g_unitel = new CUnicheck;
			return 1;		
		} 
	}
	return 0;
}

int		CClientBill::Unitel_Port()
{
	for(int i=0;i<CommandNum;i++)
	{
		char *p=Command[i];
		for(int len=0;len<lstrlen(Command[i])-9;len++)
		{
			if(!memcmp(Command[i]+len,"&PO:",4))
			{
				int temp=0;
				while(1)
				{
					temp++;
					char haha=(char) *(Command[i]+len+temp);
					if( haha=='&')
					{	
						char haha[20];
						memset(haha,0,20);
						memset(haha,0,sizeof(haha));
						memcpy(haha,Command[i]+len+4,temp-4);
						return(atoi(haha));
					}					
				}
			}
		}
	}
	return 0;
}
//char t[100]="p1.txt Unitel&IP:88.2.100.100&PO:8888&ID:huhaha&NA:���&MO:20&P1:&P2:";
int		CClientBill::Unitel_IP()
{
	memset(Return,0,sizeof(Return));
	for(int i=0;i<CommandNum;i++)
	{
		char *p=Command[i];
		for(int len=0;len<lstrlen(Command[i])-9;len++)
		{
			if(!memcmp(Command[i]+len,"&IP:",4))
			{
				int temp=0;
				while(1)
				{
					temp++;
					char haha=(char) *(Command[i]+len+temp);
					if( haha=='&')
					{	
					//	_asm int 3;
						memset(Return,0,sizeof(Return));
						memcpy(Return,Command[i]+len+4,temp-4);
					//	strcat(Return,".unitel.co.kr");
					//	MessageBox(NULL,Return,"IP address from Argument",NULL);
						g_unitel->ReturnUniteIP(Return);
						memset(Return,0,sizeof(Return));
						strcpy(Return,g_unitel->UnitelIP);
					//	MessageBox(NULL,Return,"IP address from Unitel IP return Module",NULL);
						return 1;
					}					
				}
			}
		}
	}
	return 0;
}

//char t[100]="p1.txt Unitel&IP:88.2.100.100&PO:8888&ID:huhaha&NA:���&MO:20&P1:&P2:";
char*		CClientBill::Unitel_ID()
{
	memset(UserID,0,sizeof(UserID));
	for(int i=0;i<CommandNum;i++)
	{
		char *p=Command[i];
		for(int len=0;len<lstrlen(Command[i])-9;len++)
		{
			if(!memcmp(Command[i]+len,"&ID:",4))
			{
				int temp=0;
				while(1)
				{
					temp++;
					char haha=(char) *(Command[i]+len+temp);
					if( haha=='&')
					{	
						memset(UserID,0,sizeof(UserID));
						memcpy(UserID,Command[i]+len+4,temp-4);
						return UserID;
					}					
				}
			}
		}
	}
	return 0;
}

int		CClientBill::Unitel_SendMessage()
{
	char dis[7]="UNITEL";
//	SendAliveCheckRoutine();
//	if(Send(dis,1)==0)
//		return 0;
//	else
		return 1;
}



/*
�� N��
0		�ܺθ��� 
1		�ε����� 
2		�����ID
3		NETSGO Gateway  Server�� �ּ�		         
4		Gateway ������ Base PORT��ȣ
5		Gateway���󰹼� (Multi-Server����)
6		����Server�� �ּ�
7		���� ������  Port��ȣ
8		���� ����
//9~N-4	[ ��Ÿ�ʿ��� Argument�� ]	//�Ⱦ���
9	N-3	netsgo
10	N-2	������Atoms��ȣ(Decimal)
11	N-1	����������

�� 12��
*/

int		CClientBill::Check_Netsgo()		//�� ������?
{	
	if(CommandNum!=11)
	{
		return 0;
	}
	if(atoi(Command[10])<1000)
	{
		return 0;
	}	
	if( strcmp(Command[8],"netsgo") || atoi(Command[9])<0 )
	{
		return 0;
	}
	if(Netsgo_AtomTable()==0)
	{
		return 0;
	}
	WhatLineIUse=4;
	return 1;
}

int		CClientBill::Netsgo_AtomTable()
{
	char  cBuf[256];
	ATOM  hAtom = (ATOM)atoi(Command[9]);

	if  (GlobalGetAtomName( hAtom, cBuf, 256 ) == 0 )  
	{  // Atom���� �����ϴ��� Check 
		// ����ó�� begin
		char dis[100];
		sprintf(dis, "some error - %d\n", GetLastError() );
	//	MessageBox(NULL,dis,"",NULL);
		// ����ó�� end 
		return 0;
	} 
       
	if  (lstrcmp(Command[1], cBuf ) )  
	{  // ����� ID�� Atom�� ���� String���� ���� 
	   // ����ó�� begin
		char dis[100];
		sprintf(dis, "some error - %d\n", GetLastError() );
	//	MessageBox(NULL,dis,"",NULL);
	   // ����ó�� end 
		return 0;
	}
	memset(UserID,0,sizeof(UserID));
	strcpy(UserID,Command[1]);
	return 1;
}

int		CClientBill::Netsgo_Port()			//�������� port�� ��ȯ
{
	int SERV_PORT_NO = atoi(Command[3]);// = 40007;      /*    argv[4]�� Base Port�� */
    int SERV_PORT_CNT= atoi(Command[4]);// = 5;         /*    argv[5]�� Port����    */   
    int MAX_PORT_NO = SERV_PORT_NO + SERV_PORT_CNT - 1 ;    /* Connection Retry�� ���� */
    int serv_sock = time(0);               /*  Random ó���� ���� (Language Dependent)  */
    serv_sock  %= SERV_PORT_CNT;  /*  ������ ���� ��� (Language Dependent)     */

    int daemon_port = SERV_PORT_NO + serv_sock;   /*  Base Port�� + ������ */
    return daemon_port;
}

int		CClientBill::Netsgo_IP()			//�������� ip�� ��ȯ	
{
//	_asm int 3;
	
	LPHOSTENT lpHostEnt = gethostbyname((LPCTSTR)Command[2]);	
	if (lpHostEnt)
	{
		IN_ADDR   inAddr;
		memcpy((void*)&inAddr.S_un.S_addr, lpHostEnt->h_addr,lpHostEnt->h_length);
		memset(Return,0,sizeof(Return));
		
		wsprintf(Return,"%d.%d.%d.%d",
			inAddr.S_un.S_un_b.s_b1, 
			inAddr.S_un.S_un_b.s_b2, 
			inAddr.S_un.S_un_b.s_b3, 
			inAddr.S_un.S_un_b.s_b4);
		return 1;
	}	
	return 0;
//	strcpy(Return,"211.39.34.51");
//	sprintf(Return,"%s",Command[2]);  /*  argv[3]�� Gateway  Server�� IP Address */
	return 1;
}

int		CClientBill::Netsgo_SendMessage()
{	
//	_asm int 3;
	char argument_string[255];
	memset(argument_string,0,255);
    
	sprintf(argument_string,"DragonRaja.exe %s",CommandLineBackup);
	int len = strlen(argument_string);

	if(Send(argument_string,len)==0)
		return 0;
	if(Receive(2)==0)				//Netsgo�� 2����Ʈ�� ������
		return 0;
	if(!strcmp(Buffer,"~1"))		//multi login�̸�
	{
		MessageBox(NULL,"������ ���̵� ���� �ֽ��ϴ�.","Netsgo",NULL);
		PostQuitMessage(0);
		return 0;
	}
	else
	{
		return 1;	//���� ������� �ʾƵ� ����� �ʿ��� ������ ���󰣴�.
	}
	return 0;
}

int		CClientBill::Send(char *msg,int len)
{
	int size = len;
	char *dis = new char [size];
	int pnt;

//	_asm int 3;
	memset(dis,0, size );
	memcpy(dis,msg,size);
	

	DWORD cur_time;
	DWORD check_time = timeGetTime();
	int addlen;

	pnt = 0;
	while( 1 )
	{
		addlen = send( connections.socket, dis + pnt , size, 0 );
		if( addlen < 0 ) 
		{
			PostQuitMessage(0);
			return 0;
			// Error..
		}
		else 
		{
			size -= addlen;
			pnt += addlen;

			if( size == 0 )
			{
				//   ����.
				return 1;
				break;
			}
			else if( size < 0 ) 
			{
				PostQuitMessage(0);
				return 0;
				 // Error.,
			}
		}

		cur_time = timeGetTime();
		if( cur_time - check_time > 5000 ) 
		{
			PostQuitMessage(0);
			return 0;
			//   Error ..    5�ʵ���  Packet�� ������ ���ߴ�. 
		}
		else 
		{
			//  ������ �ð��� �������� ���� ����..

		}
	}
	return 0;
}

int		CClientBill::SendToPaySocket(char *msg,int len)
{
	int size = len;
	char *dis = new char [size];
	int pnt;

//	_asm int 3;
	memset(dis,0, size );
	memcpy(dis,msg,size);
	

	DWORD cur_time;
	DWORD check_time = timeGetTime();
	int addlen;

	pnt = 0;
	while( 1 )
	{
		addlen = send( sockfd, dis + pnt , size, 0 );	
		if( addlen < 0 ) 
		{		
			PostQuitMessage(0);
			return 0;		
		}
		else 
		{
			size -= addlen;
			pnt += addlen;

			if( size == 0 )
			{				
				return 1;
				break;
			}
			else if( size < 0 ) 
			{			
				PostQuitMessage(0);
				return 0;			
			}
		}

		cur_time = timeGetTime();
		if( cur_time - check_time > 5000 ) 
		{
			PostQuitMessage(0);
			return 0;
			//   Error ..    5�ʵ���  Packet�� ������ ���ߴ�. 
		}
		else 
		{
			//  ������ �ð��� �������� ���� ����..
		}
	}
	return 0;
}


DWORD global_time_new;
char thrunetanswerbuf[4];
int  thrunetanswerbuf_pnt;
int  thrunetanswerbuf_len;

int		CClientBill::Receive(int len)
{
	memset(Buffer,0,sizeof(Buffer));
	bool ret;	
	if ( !connections.socket )
//	if ( !connections[curr_connect].socket )
	{
		return false;
	}

	Buffer_len = len;
	Buffer_pnt = 0;

	DWORD s_time = timeGetTime();
	DWORD c_time;
	while( 1 ) 
	{
		Sleep(10);
		ret = Receive_sub( &connections );
	//	ret = Receive_sub( &connections[curr_connect] );
		if( ret == true ) 
		{
			return ret;
		}

		c_time = global_time_new;
		if( c_time - s_time > 5000 )
		{
			return false;
		}
	}
	return 0;
}






bool	CClientBill::Receive_sub( t_connection *c)
{
	struct timeval	tv;
	fd_set			rfds;
	
	global_time_new = timeGetTime();


	FD_ZERO( &rfds );
	FD_SET( c->socket, &rfds );
	tv.tv_sec  = 2;
	tv.tv_usec = 0;
	if ( select( c->socket, &rfds, NULL, NULL, &tv) < 0 )
	{
		return false;
	}

	if ( FD_ISSET( c->socket, &rfds ) )
	{
		int		addlen;
		DWORD	dwBytesRead;

		while ( 1 )
		{
			if ( ioctlsocket( c->socket, FIONREAD, &dwBytesRead ) < 0 )
			{
				return false; 
			}
			if ( dwBytesRead == 0 )
			{
				break;
			}
//Buffer_len
//Buffer_pnt
			addlen = recv( c->socket, &Buffer[Buffer_pnt], Buffer_len, 0 );
			if ( addlen < 0 )
			{
				return false;
			}
			else if ( addlen > 0 )
			{
				Buffer_len -= addlen;
				Buffer_pnt += addlen;
			
				if ( Buffer_len == 0 )
				{
					answer = (void*)Buffer[0];
					return true;
				}
				else if( Buffer_len < 0 )
				{
					return false;
				}	
			}
		}
	}
	return false;
}


/*
int CClientBill::Netsgo_connect_gatewayd(char *host_nm,int port_no)
{
	int  serv_sock;
	int  rtncd,ok;
	

	//	if ((serv_sock = connect(host_nm,port_no)) < 0)    //socket connect (socket(), connect()) 
	
	return (-1);
	return serv_sock;
}
*/



//int(2byte)      //user id  length,     
//string(length)   //user id
int		CClientBill::Thrunet_SendMessage()
{
	char test[100];
	wsprintf(test,"%s %d",UserID,UserIDlength);
	int size = 2 + UserIDlength;
	char *dis = new char [size];
	int pnt;

	memset(dis,0, size );
	memcpy(dis,&UserIDlength,2);
	memcpy(dis+2,UserID,UserIDlength);
	

	DWORD cur_time;
	DWORD check_time = timeGetTime();
	int addlen;

	pnt = 0;
	while( 1 )
	{
		addlen = send( connections.socket, dis + pnt , size, 0 );
	//	addlen = send( connections[curr_connect].socket, dis + pnt , size, 0 );
		if( addlen < 0 ) 
		{
			PostQuitMessage(0);
			return 0;
			// Error..
		}
		else 
		{
			size -= addlen;
			pnt += addlen;

			if( size == 0 )
			{
				//   ����.
				break;
			}
			else if( size < 0 ) 
			{
				PostQuitMessage(0);
				return 0;
				 // Error.,
			}
		}

		cur_time = timeGetTime();
		if( cur_time - check_time > 5000 ) 
		{
			PostQuitMessage(0);
			return 0;
			//   Error ..    5�ʵ���  Packet�� ������ ���ߴ�. 
		}
		else 
		{
			//  ������ �ð��� �������� ���� ����..

		}
	}


	bool ret;
	short int answer;

	ret = WaitingTheAnswerForTHTUNET( &connections, &answer );
//	ret = WaitingTheAnswerForTHTUNET( &connections[curr_connect], &answer );

	if( ret == false ) 
	{
		//thrunet�� ����Ȱ��� �ƴϰų�.
		//���� �������ų�,
		//�������� ���ϴ� �����̰ų�,

	}
	else	// ����� �Ѿ�Դ� !!!!
	{
		switch( answer )
		{
			case 0 :	return 1;
				break;

			case -1:	return 0;
				break;
		}
	}


	return 0;
}

void CheckGameSocketAndSendHeartBeat(void* ThreadParam)	//30�ʿ� �ѹ��� ���� ������ ������ ������ ���ϵ� �ݴ´�.
{
	if((int)ThreadParam <0)
		return;
	if(g_pBill==NULL)
		return;

	HANDLE hHeartBeat = CreateEvent(NULL, FALSE, FALSE, STRING_TERMINATEEVENT);
	if (!hHeartBeat) hHeartBeat = OpenEvent(NULL, TRUE, STRING_TERMINATEEVENT);

	while(1)
	{
		//20�ʰ� ��ٸ���.
		if (WaitForSingleObject(hHeartBeat, 20000) != WAIT_TIMEOUT)
		{
			CloseHandle(hHeartBeat);
			break;
		}
		
		if( connections.socket == 0 )
		{
			g_pBill->CloseSocket();
			return;
		}
		switch( g_pBill->ReturnLineNum() )
		{ 
		case 0:			
		case 1:
		case 2:			
		case 3:
		case 4:
			g_pBill->HeartBeatToGameSocket();
			break;
		case 5:		//kornet world
			g_pBill->HeartBeatToGameSocket();
			break;
		case 6:		//mezzy system
		case 7:		//WeverNet
		case 8:		//WiseTop
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			g_pBill->HeartBeatToGameSocket();
			break;
		default:
			char dis[100];
			sprintf(dis,"Check HeartBeat %d Corp",g_pBill->ReturnLineNum());
			MessageBox(NULL,dis,"HeartBeat Error",NULL);
		}		
	}
}

int	CClientBill::CloseSocket()				//������������ �����ִ� ������ ��ȯ
{
	if(sockfd)
		closesocket(sockfd);
	return 1;
}

int	CClientBill::MakePaySocket(int How)
{
	if(ReturnLineNum()<0)
		return 0;
	struct sockaddr_in	addr;
	int i;
	memset(&sockfd,0,sizeof(sockfd));
	
	for( i = 0 ; i < 10 ; i ++)
	{	
		sockfd = socket( AF_INET, SOCK_STREAM, 0 );  // ���ͳ�Ÿ��, ��Ʈ����	��
		if ( sockfd == INVALID_SOCKET  )										
		{
			int err = WSAGetLastError();										
			switch( err )
			{	
				case WSANOTINITIALISED :	Sleep( 30 );
											break;
			}
		}
		else break;
	}
	if( i == 10 ) return -1;
		
	addr.sin_family = AF_INET;
	addr.sin_port = htons( ReturnPayPORTNumber() );
	addr.sin_addr.s_addr = inet_addr( ReturnPayIPaddress() );
	u_long argp = 1;
	ioctlsocket ( sockfd , FIONBIO, &argp);
	int ret = connect( sockfd , ( LPSOCKADDR )&addr, sizeof( addr ) );
//	if(ret<0)
//		PostQuitMessage(0);
		
	for( i = 0 ; i < 4 ; i ++)
	{	
		fd_set wfds;
		FD_ZERO(&wfds);
		FD_SET( sockfd , &wfds);
		struct timeval t;
		t.tv_sec =  2;
		t.tv_usec = 0;
		select (0, NULL, &wfds, NULL, &t);
		if ( !FD_ISSET( sockfd , &wfds) ) // send���� �ִ� ��Ȳ�� �ƴϸ�...		
		{																			
			if( i == 9 )						// �аų� ���� ���� ��Ȳ�� 9�������� ��ٷ��ش�. 
			{		
				int err = WSAGetLastError();
				return -2;
			}
		}
		else break;
	}
    return( 1 );
}

void	CClientBill::HeartBeatToOtherSocket()	//������ ��ü������ ���� ���Ͽ��� �����ð����� �޽����� ������.
{
	SendToPaySocket("alive",5);
}

void	CClientBill::HeartBeatToGameSocket()	//���� ���ӿ����� ���� ���Ͽ��� �����ð����� �޽����� ������.
{	//���Ӽ����� �������� �ʰ� �α��θ� �� ���°� ��� ������ ��� �ƹ��� �޽����� ������ �ʱ� ������ ������ ����Ʈ���̿���
	//������ ��������� �����Ѵ�.
	SendAliveCheckRoutine();
}



////////////////////////


// ���� �������  bool *answer�� ���� ���� ����  true�� �����Ѵ�. 
// ���� ������    answer�� ���� ���� �ʰ� false�� �����Ѵ�. 
bool WaitingTheAnswerForTHTUNET_sub( t_connection *c, short int *answer )
{		
	struct timeval	tv;
	fd_set			rfds;
	
	global_time_new = timeGetTime();


	FD_ZERO( &rfds );
	FD_SET( c->socket, &rfds );
	tv.tv_sec  = 2;
	tv.tv_usec = 0;
	if ( select( c->socket, &rfds, NULL, NULL, &tv) < 0 )
	{
		return false;
	}

	if ( FD_ISSET( c->socket, &rfds ) )
	{
		int		addlen;
		DWORD	dwBytesRead;

		while ( 1 )
		{
			if ( ioctlsocket( c->socket, FIONREAD, &dwBytesRead ) < 0 )
			{
				return false; 
			}
			if ( dwBytesRead == 0 )
			{
				break;
			}
			addlen = recv( c->socket, &thrunetanswerbuf[ thrunetanswerbuf_pnt], thrunetanswerbuf_len, 0 );
			if ( addlen < 0 )
			{
				return false;
			}
			else if ( addlen > 0 )
			{
				thrunetanswerbuf_len -= addlen;
				thrunetanswerbuf_pnt += addlen;
			
				if ( thrunetanswerbuf_len == 0 )
				{
					*answer = *(short int *)thrunetanswerbuf;
					return true;
				}
				else if( thrunetanswerbuf_len < 0 )
				{
					return false;
				}	
			}
		}
	}
	return false;
}


bool WaitingTheAnswerForTHTUNET( t_connection *c, short int *answer )
{
	bool ret;

	if ( !c->socket )
	{
		return false;
	}

	thrunetanswerbuf_len = 4;
	thrunetanswerbuf_pnt = 0;

	DWORD s_time = timeGetTime();
	DWORD c_time;
	while( 1 ) 
	{
		Sleep(10);
		ret = WaitingTheAnswerForTHTUNET_sub( c, answer );
		if( ret == true ) 
		{
			return ret;
		}

		c_time = global_time_new;
		if( c_time - s_time > 5000 )
		{
			return false;
		}
	}
}


//unitel
void Uniwin_CallbackDetectFileChangeThread(void *pThreadParam)
{
	STRUCTCHECKUNIPARAM	*pParam = (STRUCTCHECKUNIPARAM	*)pThreadParam;
	STRUCTCHECKUNIPARAM	structParam;
	structParam.hWndReceiveWindow = pParam->hWndReceiveWindow;
	structParam.nSendToMessage = pParam->nSendToMessage;
	delete pParam; // parameter should be destroyed..

	HANDLE hTerminateEvent = CreateEvent(NULL, FALSE, FALSE, STRING_TERMINATEEVENT);
	if (!hTerminateEvent) hTerminateEvent = OpenEvent(NULL, TRUE, STRING_TERMINATEEVENT);

	BOOL bUniwinRunningStatus = IsWindow(FindWindow("UNIMAIN_CLASS", NULL));
	// Send Message at Trigger of (Running <-> Terminated state)
	BOOL bUniwinWndFound = FALSE;
	while(1)
	{
		// �̰��� GetUniwinInfo�� �Ϻ� ��ƾ�� �־� �������� ���¸� �ֱ���(1000 ms)���� üũ�Ͽ�
		// ���°� ���� ��� �޽����� �ֵ��� ����������.
		// ����� �������� ���� �� ��� �̸� Notify�ϱ� ���� �ۼ���.
		bUniwinWndFound = IsWindow(FindWindow("UNIMAIN_CLASS", NULL));
		if (bUniwinRunningStatus && !bUniwinWndFound)
		{
			bUniwinRunningStatus = FALSE;
			::PostMessage(structParam.hWndReceiveWindow, structParam.nSendToMessage, 0, (LPARAM)TRUE);
		}
		else if (!bUniwinRunningStatus && bUniwinWndFound)
		{
			bUniwinRunningStatus = TRUE;
		}

		// check at every 1 second
		if (WaitForSingleObject(hTerminateEvent, 1000) != WAIT_TIMEOUT)
		{
			CloseHandle(hTerminateEvent);
			return;
		}
	}
}

// EXAPLAIN 5 : Start Uniwin running check thread..
BOOL CUnicheck::Uniwin_StartCheckUniwin(HWND hReceiveWindow, UINT nReceiveMessage)
{
	// this param will destroy at thread function..
	STRUCTCHECKUNIPARAM	*pStructParam = new STRUCTCHECKUNIPARAM;
	pStructParam->hWndReceiveWindow = hReceiveWindow;
	pStructParam->nSendToMessage = nReceiveMessage;

	unsigned long hThreadHandle = _beginthread(Uniwin_CallbackDetectFileChangeThread, NULL, (void*)pStructParam);
	if (hThreadHandle == -1)
		return FALSE;

	return TRUE;
}

// EXAPLAIN 6 : Terminate uniwin check thread
void CUnicheck::Uniwin_TerminateCheckUniwin()
{
	HANDLE hTerminateEvent = OpenEvent(NULL, TRUE, STRING_TERMINATEEVENT);
	if (hTerminateEvent)
		return;

	SetEvent(hTerminateEvent);
	Sleep(100);
}

// EXAPLAIN 7 : Get Current uniwin infomation
void CUnicheck::Uniwin_GetUniwinInfo()
{
//	UpdateData(TRUE);
	
	// 1. Find Uniwin Directory...
	char *pUniwinDirectory = new char[MAX_PATH + 1];
	GetPrivateProfileString("UNITEL", "UNIWIN", "", pUniwinDirectory, MAX_PATH, "win.ini");
		// returns pUniwinDirectory >> "C:\UNIWIN98\"
		// if return "" then uniwin doesnot installed..

	// Check UniwinDirectory Fail...
	if (!lstrlen(pUniwinDirectory)) 
	{
		delete pUniwinDirectory;
		return;
	}

	// 2. Make Global.ull Full Path
	char *pGlobalUll = new char[MAX_PATH + 1];
	strcpy(pGlobalUll, pUniwinDirectory);
	strcat(pGlobalUll, "GLOBAL.ULL");
		// returns pGlobalUll >> "C:\UNIWIN98\GLOBAL.ULL"

	// 3. Prepare for obtain user info from "global.ull"
	HWND hUniwinWindow = NULL;
	BOOL bRunningStatus = 0;

	// buffer for user id
	char *pUserID = new char[MAX_USERID]; 
	memset(pUserID , 0 , MAX_USERID);
	*pUserID = '\0';

	// buffer for user name
	char *pUserName = new char[MAX_USERNAME]; 
	memset(pUserName,0,MAX_USERNAME);
	*pUserName = '\0';

	Uniwin_RunningStatus=0;
//	memset(szpUniwinDirectory,0,100);
	memset(Uniwin_szpUserID,0,100);
	memset(Uniwin_szpUserName,0,100);

	// buffer for login status
	int	  nLoginStatus = 0;

	// load global.ull
	HINSTANCE hGlobalUll = ::LoadLibrary(pGlobalUll);
	if (hGlobalUll)
	{
		// 4. Check Uniwin is running (you can use FindWindow at _CallbackDetectFileChangeThread Function.
		FnGetHWNDUnimain fnGetHWNDUnimain = (FnGetHWNDUnimain)GetProcAddress(hGlobalUll, "GetHWNDUnimain");
		if (fnGetHWNDUnimain) 
			hUniwinWindow = fnGetHWNDUnimain();

		bRunningStatus = ::IsWindow(hUniwinWindow);

		if (bRunningStatus)
		{
			// Send Unimain to close my window When Logoff
			// when logoff of uniwin then my window receives WM_CLOSE message (LPARAM is 1).
			::SendMessage(hUniwinWindow, UM_CLOSENOTIFY, (WPARAM)Uniwin_m_hWnd, (LPARAM)TRUE);

			// 5. Get User ID
			FnUNIGetUserID fnUNIGetUserID = (FnUNIGetUserID)GetProcAddress(hGlobalUll, "UNIGetUserID");
			if (fnUNIGetUserID) 
				strcpy(pUserID, fnUNIGetUserID());

			// 6. Get User Name
			FnUNIGetUserName fnUNIGetUserName = (FnUNIGetUserName)GetProcAddress(hGlobalUll, "UNIGetUserName");
			if (fnUNIGetUserName) 
				strcpy(pUserName, fnUNIGetUserName());

			// 7. Get Login Status
			FnUNIGetLoginStatus fnUNIGetLoginStatus = (FnUNIGetLoginStatus)GetProcAddress(hGlobalUll, "UNIGetLoginStatus");
			if (fnUNIGetLoginStatus) 
				nLoginStatus = fnUNIGetLoginStatus();

			Uniwin_RunningStatus = bRunningStatus;
			
	//		memcpy(szpUniwinDirectory, pUniwinDirectory,100);
			memcpy(Uniwin_szpUserID , pUserID,strlen(pUserID));
			memcpy(Uniwin_szpUserName , pUserName,strlen(pUserName));
	//		MessageBox(NULL,pUniwinDirectory,"Directory",NULL);
	//		MessageBox(NULL,pUserID,"ID",NULL);
	//		MessageBox(NULL,pUserName,"Name",NULL);
		//	MessageBox(NULL,pUserID,"ID",NULL);
		}
		::FreeLibrary(hGlobalUll);
	}

	delete pUniwinDirectory;
	delete pGlobalUll;
	delete pUserID;
	delete pUserName;
//	UpdateData(FALSE);
}

void	CUnicheck::Uniwin_Init(HWND hwnd)
{
//	_asm int 3;
	Uniwin_m_hWnd=hwnd;
	g_unitel->Uniwin_StartCheckUniwin(hwnd,UM_TERMINATENOTIFY);
//	g_unitel->Uniwin_StartCheckUniwin(NULL,UM_TERMINATENOTIFY);
	g_unitel->Uniwin_GetUniwinInfo();
	char FromUnitel[100],FromArgument[100];
	memset(FromUnitel,0,100);
	memset(FromArgument,0,100);
	strcpy(FromArgument,g_pBill->Unitel_ID());
	if(strlen(g_unitel->Uniwin_szpUserID)<1)	
		PostQuitMessage(0);
	strcpy(FromUnitel,g_unitel->Uniwin_szpUserID);
	if(strcmp(FromArgument,FromUnitel))		//���� ���ڷ� �Ѿ�� ���̵�� �����ڷκ��� ���� ���̵� �ٸ���
		PostQuitMessage(0);	
	
}

CUnicheck::CUnicheck()
{
//	Uniwin_Init(hwnd);
}

CUnicheck::~CUnicheck()
{
	Uniwin_TerminateCheckUniwin();
}



char* CUnicheck::ReturnUniteIP(char *Name)
{
	LPHOSTENT lpHostEnt = gethostbyname((LPCTSTR)Name);	
	memset(UnitelIP,0,40);
	if (lpHostEnt)
	{
		IN_ADDR   inAddr;
		memcpy((void*)&inAddr.S_un.S_addr, lpHostEnt->h_addr,lpHostEnt->h_length);
		
		
		wsprintf(UnitelIP,"%d.%d.%d.%d",
			inAddr.S_un.S_un_b.s_b1, 
			inAddr.S_un.S_un_b.s_b2, 
			inAddr.S_un.S_un_b.s_b3, 
			inAddr.S_un.S_un_b.s_b4);
		return UnitelIP;

	//	m_strResult.Format("DWORD:%X,ToSTR: %d.%d.%d.%d", 
	//		inAddr.S_un.S_addr, 
	//		inAddr.S_un.S_un_b.s_b1, 
	//		inAddr.S_un.S_un_b.s_b2, 
	//		inAddr.S_un.S_un_b.s_b3, 
	//		inAddr.S_un.S_un_b.s_b4);
	}	
	return NULL;
	
}









