#include "stdafx.h"
#include "gameproc.h"
#include "NetWork.h"
#include "NationSys.h"
#include "menu.h"
#include "SmallMenu.h"
#include "map.h"
#include "LineCommand.h"
#include "DirectSound.h"
#include "tool.h"
#include "nationsys.h"
#include "nationSUPPORT.h"
#include "MenuDef2.h"
#include "menuset.h"//021008 lsw
#include "Skill.h"//021008 lsw
#include "Auction.h"//021008 lsw
#include "MailMgr.h"//021008 lsw
//< LTH-040812-KO 1.04p 감옥 시스템을 위해 추가
#include "Prison.h"

extern CPrison g_cPrison;
extern void SendMoveToHomeTown();
extern void SendMoveToHomeTownWithAlive();
extern int CallFriendMenu();
extern cNation*	g_pNation;
extern int g_Squad;						// 나중에 클래스에 포함한다. 
extern int g_Commander;
extern int g_AttackNation;
extern int g_DefenceNation;
extern int g_CommanderCandidaterLevel;   //나중에 바꾼다..
extern bool g_bWarBegin;  // RecvCMD_WAR_START,END에서 사용 

extern int g_SquadLoadingPoint;//로딩포인트 번호 
extern int g_SquadNum;			//부대메뉴 

extern int WarResultAniCount;
extern bool isNationWarfieldServer();
extern void SendCMD_NW_MAP_MOVE(int Type,int WarfieldNo);
extern bool isAttacker();
extern void SendNationPacket(int type);
extern void SendCMD_REQUEST_SQUAD_INFO(int SquadNo);				// Request Warfield Squad Information
extern void SendCMD_REQUEST_COMMANDER_VOTE_DATA(int CandidaterNo);	// Request Commander Candidater Information, Parameter : Candidater No
extern void SendCMD_SET_SQUAD_LODING_POINT(int SquadNo,int LoadingPoint) ;
extern int GetSquadNo();
extern int CheckNationStatus(int StatusNo);
extern void GetTime(DWORD RemainTime,WORD& Hour,WORD& Min,WORD& Sec);
void NationWarResultDoneButton();//결과 에서 done 누르면 들어옴

extern void SendCMD_CHECK_AM_I_DEFEAT();			//임시함수 // 011015 LTS
extern void SendCMD_LOCALWAR_JOIN();									// LTS LOCALWAR
extern void DrawLocalWarJoinInterface(int ImageType,int MenuNumber,int BaseX,int BaseY);
extern bool isNewWarfieldServer();
extern void SendCMD_CHECK_DEFEAT();
extern void CallMapMoveMenu(int WarfieldNo);

//010924 lsw
int g_TodayNoticeStatus[3]	= {0,};
int g_TodayNoticeMsgNo[3] ={300,300,300};

void NationSupportRecvNationWarResult(t_packet *p)
{
	WarResultAniCount = 0;
 	int WinNation=p->u.NationWar.WarBBS.Arg[0];
	int AttackerWin=p->u.NationWar.WarBBS.Arg[1];
	//___________________________	// 010927 LTS
	WORD BYCount;
	WORD ZYCount;
	WORD YL_BYCount;
	WORD BYDeathCount;
	WORD ZYDeathCount;
	WORD YL_ZYCount;
	//011014 lsw >
	g_bWarBegin =0;
	//011014 lsw <

	//___________________________	// 010927 LTS
	memcpy(&BYCount,		&p->u.NationWar.WarBBS.Arg[2],sizeof(WORD));	// 12바이트... 멤버카운트이다.
	memcpy(&ZYCount,		&p->u.NationWar.WarBBS.Arg[4],sizeof(WORD));	// 12바이트... 멤버카운트이다.
	memcpy(&YL_BYCount,		&p->u.NationWar.WarBBS.Arg[6],sizeof(WORD));	// 12바이트... 멤버카운트이다.

	memcpy(&BYDeathCount,	&p->u.NationWar.WarBBS.Arg[8],sizeof(WORD));	// 12바이트... 멤버카운트이다.
	memcpy(&ZYDeathCount,	&p->u.NationWar.WarBBS.Arg[10],sizeof(WORD));	// 12바이트... 멤버카운트이다.
	memcpy(&YL_ZYCount,		&p->u.NationWar.WarBBS.Arg[12],sizeof(WORD));	// 12바이트... 멤버카운트이다.

	//011014 LSW >
	SMenu[MN_NATION_WAR_RESULT].nField[1].nWillDo = BYDeathCount;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nWillDo = ZYDeathCount;

	SMenu[MN_NATION_WAR_RESULT].nField[3].nWillDo = BYCount;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nWillDo = YL_BYCount;

	SMenu[MN_NATION_WAR_RESULT].nField[5].nWillDo =	ZYCount;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nWillDo = YL_ZYCount;
	//011014 LSW <

	int iamwin = 0;

	if( Hero->name_status.nation==NW_YL)
	{
		 if( Hero->NWCharacter.YL_JoinNation==WinNation)
		 {
			iamwin = 1;
		 }
	}
	if( Hero->name_status.nation==WinNation)//승리 대상
	{
		iamwin = 1;
	}

	
	if( iamwin )//승리 대상
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 0;
		//PlayListAutoSounds(62,0,0,0);			
		if(WinNation==NW_BY)//바이서스가 이김
		{
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 1;
		}
		else if(WinNation==NW_ZY)//자이펀이 이김
		{
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 3;
		}
	}
	else//패배 대상
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 1;
		//PlayListAutoSounds(62,0,0,0);
		//010927 lsw 패배 하는거
		if(WinNation==NW_BY)//자이펀이 짐
		{
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 4;
		}
		else if(WinNation==NW_ZY)//바이서스가 짐
		{
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 2;
		}
	}	

	CallSmallMenu(MN_NATION_WAR_RESULT);
	return;
}

int CallNationYesOrNo(int iPrevType,int temp)
{
	MP3(258);
	switch(iPrevType)
	{
	//010928 lsw
	case SWD_NATION_BUDAE_JOIN://부대 등록 묻기 krlan을 빼오자 
		if(g_bWarBegin == 1 )//전쟁중이면
		{
			CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,197), FALSE);
			return 0;
		}
		else
		{
			sprintf(SMenu[MN_NATION_MSGBOX].nField[2].temp,lan->OutputMessage(0,143),temp);
		}
		break;
	
	case SWD_NATION_VOTE_I_AM_HERO://부대 등록 묻기 krlan을 빼오자 
		sprintf(SMenu[MN_NATION_MSGBOX].nField[2].temp,lan->OutputMessage(0,144));
		break;
	}
	SMenu[MN_NATION_MSGBOX].nField[1].nWillDo=iPrevType;//이 값을 참조 하여 일을 한다
	
	CallSmallMenu(MN_NATION_MSGBOX,MN_NATION_BATTLE_BBS);//메세지 박스 호출
	return 0;
}

// Style Flag	---> 0x0000 ALL_CLEAR 0x0001 FORCE_TYPE 0x0002 QUEUE_TYPE 
// iPositionFlag---> 
//					At QUEUE_TYPE
//						0x0001 FIRST_IN 0x0002	LAST_IN
//
//

//020620 YGI  삭제


//011013 lsw>
void IntoNationNotice(const eIntoNationNoticeType iStyleFlag, int iPosition, char *Msg)//QUEUE Style //INTOPOSTION Style // ALL Clear // 
{	
	switch(iStyleFlag )
	{
	case ALL_CLEAR:
		{
			ZeroMemory( SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp,sizeof(char)*MAX_STRING);
			ZeroMemory( SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp,sizeof(char)*MAX_STRING);
			ZeroMemory( SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp,sizeof(char)*MAX_STRING);
			ZeroMemory( SMenu[MN_NATION_TODAY_NOTICE].nField[4].temp,sizeof(char)*MAX_STRING);
			ZeroMemory( SMenu[MN_NATION_TODAY_NOTICE].nField[5].temp,sizeof(char)*MAX_STRING);
			ZeroMemory( SMenu[MN_NATION_TODAY_NOTICE].nField[6].temp,sizeof(char)*MAX_STRING);
		}
		break;
	case FORCE_TYPE:
		{
			strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[iPosition].temp, Msg);
			memcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[iPosition+3].temp,(SMenu[MN_NATION_TODAY_NOTICE].nField[iPosition].temp+1), 16);//작은칸 셋팅
			SMenu[MN_NATION_TODAY_NOTICE].nField[iPosition+3].nWillDo  = (strlen(SMenu[MN_NATION_TODAY_NOTICE].nField[iPosition+3].temp)-1);
			strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[7].temp, lan->OutputMessage(0, 399));	// BBD 040315
		}break;
	case QUEUE_TYPE:
		{
			switch(iPosition )
			{
			case FIRST_IN:
				{
					// 020706 YGI -----------------------
					if( !SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp[0] )
					{
						IntoNationNotice( FORCE_TYPE, 1, Msg );
						return;
					}
					memset( SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp, 0, sizeof( SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp ) );
					strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp, SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp);
					memset( SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp, 0, sizeof( SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp ) );
					strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp, SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp);
					memset( SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp, 0, sizeof( SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp ) );
					strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp, Msg);
					// ----------------------------------------

					memcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[4].temp,(SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp+1), 16);//작은칸 셋팅
					memcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[5].temp,(SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp+1), 16);//작은칸 셋팅
					memcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[6].temp,(SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp+1), 16);//작은칸 셋팅

					SMenu[MN_NATION_TODAY_NOTICE].nField[4].nWillDo  = (strlen(SMenu[MN_NATION_TODAY_NOTICE].nField[4].temp)-1);
					SMenu[MN_NATION_TODAY_NOTICE].nField[5].nWillDo  = (strlen(SMenu[MN_NATION_TODAY_NOTICE].nField[5].temp)-1);
					SMenu[MN_NATION_TODAY_NOTICE].nField[6].nWillDo  = (strlen(SMenu[MN_NATION_TODAY_NOTICE].nField[6].temp)-1);

					//011030 lsw >
					strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[7].temp, lan->OutputMessage(0, 399));
					//011030 lsw <
				}break;
			case LAST_IN:
				{
					// 020706 YGI -----------------------
					if( !SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp[0] )
					{
						IntoNationNotice( FORCE_TYPE, 3, Msg );
						return ;
					}
					memset( SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp, 0, sizeof( SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp ) );
					strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp, SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp);
					memset( SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp, 0, sizeof( SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp ) );
					strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp, SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp);
					memset( SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp, 0, sizeof( SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp ) );
					strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp, Msg);
					// ----------------------------------------

					memcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[4].temp,(SMenu[MN_NATION_TODAY_NOTICE].nField[1].temp+1), 16);//작은칸 셋팅
					memcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[5].temp,(SMenu[MN_NATION_TODAY_NOTICE].nField[2].temp+1), 16);//작은칸 셋팅
					memcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[6].temp,(SMenu[MN_NATION_TODAY_NOTICE].nField[3].temp+1), 16);//작은칸 셋팅

					SMenu[MN_NATION_TODAY_NOTICE].nField[4].nWillDo  = (strlen(SMenu[MN_NATION_TODAY_NOTICE].nField[4].temp)-1);
					SMenu[MN_NATION_TODAY_NOTICE].nField[5].nWillDo  = (strlen(SMenu[MN_NATION_TODAY_NOTICE].nField[5].temp)-1);
					SMenu[MN_NATION_TODAY_NOTICE].nField[6].nWillDo  = (strlen(SMenu[MN_NATION_TODAY_NOTICE].nField[6].temp)-1);

					strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[7].temp, lan->OutputMessage(0, 399));
				}break;
			}
		}break;
	}

	int *MasterImage	;
	int *SlaveImage		;
	int	*WillDo			;

	for(int pos = 1; pos < 4; pos++)
	{
		MasterImage		= &SMenu[MN_NATION_TODAY_NOTICE].nField[pos].nImageNumber;
		SlaveImage		= &SMenu[MN_NATION_TODAY_NOTICE].nField[pos].nRectImage;
		WillDo			= &SMenu[MN_NATION_TODAY_NOTICE].nField[pos].nWillDo;
		char 	Status	=	SMenu[MN_NATION_TODAY_NOTICE].nField[pos].temp[0];
			
		switch (Status)
		{
		case '0'://이벤트 없음
			{
				*MasterImage	=	9;
				*SlaveImage		=	10;
				*WillDo			=	1;
			}
			break;
		case '1'://전투 관련상태
			{	//전투 관련 상태
				*MasterImage	=	5;	
				*SlaveImage		=	6;
				*WillDo			=	1;
			}
		break;
		case '2'://공지 관련
			{	//공지관련.. (투표나.. 등등)
				*MasterImage	=	7;	
				*SlaveImage		=	8;
				*WillDo			=	1;
			}
			break;
		case '3'://이벤트 관련 
			{	//이벤트 관련
				*MasterImage	=	3;	
				*SlaveImage		=	4;
				*WillDo			=	1;
			}
			break;
		default:
			{
				*MasterImage	=	9;
				*SlaveImage		=	10;
				*WillDo			=	1;
			}
			break;
		}
	}
	return;
}
//011013 lsw <

bool DoButtonNationOkCancel_lsw(int iOkType)
{
	switch(iOkType)
	{
	case SWD_NATION_BUDAE_JOIN:
		SendNationPacket(BUTTON_SQUAD_JOIN);
		break;
	case SWD_NATION_VOTE_I_AM_HERO:
		SendNationPacket(BUTTON_COMMANDER_REG);
		break;
	}
	return false;
}

//부대에 등록 할 조건이 되는가 체크
bool NationSupportCheckSquadJoin()
{
	return true;
}

//사령관이 될수 있는가를 체크
bool NationSupportCheckCanWillCommander()
{
	return true;
}


void NationSupportNationNow1(t_packet *p)
{
	char  GuardStoneSize=p->u.data[0];
	char  CastleSize=p->u.data[GuardStoneSize+1];
	char* GuardStoneStatus=new char[GuardStoneSize];
	char* CastleStatus=new char[CastleSize];

	SMenu[MN_NATION_NOW].nField[20].nWillDo		= 1;
	SMenu[MN_NATION_NOW].nField[21].nWillDo		= 1;
	SMenu[MN_NATION_NOW].nField[22].nWillDo		= 1;
	SMenu[MN_NATION_NOW].nField[23].nWillDo		= 1;

	switch( CheckNationStatus(NW_WAR))
	{	
	case	0://산중OK
		//좌표및 세팅		
		SMenu[MN_NATION_NOW].nField[3].nImageNumber = 9;
		SMenu[MN_NATION_NOW].nField[4].nType = 0;//이름 바닥꺼 씀
		/*
		[64.132]-1번수호석
		[91.123]-2번수호석
		[129.85]-3번수호석
		[141.58]-4번수호석
		[99.150]-1번성문
		[136.113]-2번성문
		[174.75]-3번성문
		*/

		//성문 4짝
		SMenu[MN_NATION_NOW].nField[24].x = 99 +19;
		SMenu[MN_NATION_NOW].nField[24].y = 150+45;
		
		SMenu[MN_NATION_NOW].nField[25].x = 136+19;
		SMenu[MN_NATION_NOW].nField[25].y = 113+45;
		
		SMenu[MN_NATION_NOW].nField[26].x = 174+19;
		SMenu[MN_NATION_NOW].nField[26].y = 75 +45;
		
		SMenu[MN_NATION_NOW].nField[27].nWillDo = 0;
		//결계석 4개 
		SMenu[MN_NATION_NOW].nField[28].x = 64 +19;
		SMenu[MN_NATION_NOW].nField[28].y = 132+45;
		

		SMenu[MN_NATION_NOW].nField[29].x = 91 +19;
		SMenu[MN_NATION_NOW].nField[29].y = 123 +45;
		
		SMenu[MN_NATION_NOW].nField[30].x = 129+19;
		SMenu[MN_NATION_NOW].nField[30].y = 85 +45;
		
		SMenu[MN_NATION_NOW].nField[31].x = 141+19;
		SMenu[MN_NATION_NOW].nField[31].y = 58 +45;
		
		break;
	case	1://평원 OK
		SMenu[MN_NATION_NOW].nField[3].nImageNumber = 10;
		SMenu[MN_NATION_NOW].nField[4].nImageNumber = 3;//또는 4
		/*
		[64.114]-4번수호석
		[113.69]-3번수호석
		[163.116]-2번수호석
		[113.164]-1번수호석
		[110.94]-최종수호석
		[89.134]-1번성문
		[149.136]-2번성문
		[153.75]-3번성문
		*/
		SMenu[MN_NATION_NOW].nField[24].x = 89 +19;
		SMenu[MN_NATION_NOW].nField[24].y = 134+45;
		
		SMenu[MN_NATION_NOW].nField[25].x = 149 +19;
		SMenu[MN_NATION_NOW].nField[25].y = 136+45;
		
		SMenu[MN_NATION_NOW].nField[26].x = 153+20;
		SMenu[MN_NATION_NOW].nField[26].y = 75 +50;
		
		SMenu[MN_NATION_NOW].nField[27].nWillDo = 0;
		//결계석 5개 
		SMenu[MN_NATION_NOW].nField[28].x = 113 +19;
		SMenu[MN_NATION_NOW].nField[28].y = 164+45;
		
		SMenu[MN_NATION_NOW].nField[29].x = 163+19;
		SMenu[MN_NATION_NOW].nField[29].y = 116 +45;
		
		SMenu[MN_NATION_NOW].nField[30].x = 113+19;
		SMenu[MN_NATION_NOW].nField[30].y = 69 +45;
		
		SMenu[MN_NATION_NOW].nField[31].x = 64+19;
		SMenu[MN_NATION_NOW].nField[31].y = 114+45;
		break;
	case	2://섬 OK
		SMenu[MN_NATION_NOW].nField[3].nImageNumber = 11;
		SMenu[MN_NATION_NOW].nField[4].nImageNumber = 4;//또는 4
		/*
		[125.102]-3번수호석
		[102.123]-4번수호석
		[125.144]-1번수호석
		[150.125]-2번수호석
		[75.78]-1번성문
		[98.59]-2번성문
		[164.143]-3번성문
		*/
		SMenu[MN_NATION_NOW].nField[24].x = 75 +19;
		SMenu[MN_NATION_NOW].nField[24].y = 78 +45;

		
		SMenu[MN_NATION_NOW].nField[25].x = 98 +19;
		SMenu[MN_NATION_NOW].nField[25].y = 59 +45;
		
		SMenu[MN_NATION_NOW].nField[26].x = 164+19;
		SMenu[MN_NATION_NOW].nField[26].y = 143+45;
		
		SMenu[MN_NATION_NOW].nField[27].nWillDo = 0;
		//결계석 5개 
		SMenu[MN_NATION_NOW].nField[28].x = 125+19;
		SMenu[MN_NATION_NOW].nField[28].y = 144+45;
		
		SMenu[MN_NATION_NOW].nField[29].x = 150+19;
		SMenu[MN_NATION_NOW].nField[29].y = 125+45;
		
		SMenu[MN_NATION_NOW].nField[30].x = 125+19;
		SMenu[MN_NATION_NOW].nField[30].y = 102+45;

		SMenu[MN_NATION_NOW].nField[31].x = 102+19;
		SMenu[MN_NATION_NOW].nField[31].y = 123+45;
		break;
	}
	memcpy(GuardStoneStatus,(p->u.data+1),GuardStoneSize);
	memcpy(CastleStatus,(p->u.data+GuardStoneSize+2),CastleSize);
	
	if( isNationWarfieldServer() )
	{
		switch(SCharacterData.nCharacterData[CLASS])
		{
			case WARRIOR:
			SMenu[MN_NATION_NOW].nField[33].nImageNumber =33;
				break;
			case	THIEF:
			SMenu[MN_NATION_NOW].nField[33].nImageNumber =34;
				break;
			case	ARCHER:
			SMenu[MN_NATION_NOW].nField[33].nImageNumber =35;
				break;
			case	WIZARD:
			SMenu[MN_NATION_NOW].nField[33].nImageNumber =36;
				break;
			case	PRIEST:	
			SMenu[MN_NATION_NOW].nField[33].nImageNumber =37;
				break;
		}

		SMenu[MN_NATION_NOW].nField[33].nWillDo =1;
		//011014 lsw >
		int mapx =	Hero->x/TILE_SIZE; 
		int mapy =	Hero->y/TILE_SIZE; 
		//011014 lsw <

		if(CheckNationStatus(NW_WAR) ==1)
		{
			mapx -=300;
			mapy -=300;
		}
		//축적 변환
		//011010 lsw >
		float fmapx = ( 280.000 * float(mapx) / 420.000 );
		float fmapy = ( 280.000 * float(mapy) / 420.000 );
/*
		switch( CheckNationStatus(NW_WAR))
		{
		case  0://산중 
			fmapx += ;
			fmapy += ;
			break;
		case  1://평원 
			fmapx += -15;
			fmapy += -20;
			break;
		case  2://섬
			fmapx += ;
			fmapy += ;
			break;
		}
*/

		SMenu[MN_NATION_NOW].nField[33].x = (int)fmapx+10;//+SMenu[MN_NATION_NOW].nField[3].x;		// 011023 LTS
		SMenu[MN_NATION_NOW].nField[33].y = (int)fmapy+24;//+SMenu[MN_NATION_NOW].nField[3].y;		// 011023 LTS
		//011010 lsw <
	}
	else
	{
		SMenu[MN_NATION_NOW].nField[33].nWillDo = 0;
	}

	switch(Hero->name_status.nation)
	{
	case NW_BY:
		SMenu[MN_NATION_NOW].nField[20].nImageNumber = 21;
		SMenu[MN_NATION_NOW].nField[21].nImageNumber = 22;
		SMenu[MN_NATION_NOW].nField[22].nImageNumber = 23;
		SMenu[MN_NATION_NOW].nField[23].nImageNumber = 24;
		break;
	case NW_ZY:
		SMenu[MN_NATION_NOW].nField[20].nImageNumber = 25;
		SMenu[MN_NATION_NOW].nField[21].nImageNumber = 26;
		SMenu[MN_NATION_NOW].nField[22].nImageNumber = 27;
		SMenu[MN_NATION_NOW].nField[23].nImageNumber = 28;
		break;
	case NW_YL:
		SMenu[MN_NATION_NOW].nField[20].nImageNumber = 29;
		SMenu[MN_NATION_NOW].nField[21].nImageNumber = 30;
		SMenu[MN_NATION_NOW].nField[22].nImageNumber = 31;
		SMenu[MN_NATION_NOW].nField[23].nImageNumber = 32;
		break;
	}

	//011008 lsw >
	int squadno = Hero->NWCharacter.SquadNo;
	if(squadno)
	{
		SMenu[MN_NATION_NOW].nField[19+squadno].nWillDo = 2;
	}


	SMenu[MN_NATION_NOW].nField[24].nWillDo = (CastleStatus[0]&&CastleStatus[1]);
	SMenu[MN_NATION_NOW].nField[25].nWillDo	= (CastleStatus[2]&&CastleStatus[3]);
	SMenu[MN_NATION_NOW].nField[26].nWillDo	= (CastleStatus[4]&&CastleStatus[5]);
	SMenu[MN_NATION_NOW].nField[27].nWillDo	= 0;
	
	SMenu[MN_NATION_NOW].nField[28].nWillDo	= GuardStoneStatus[0];
	SMenu[MN_NATION_NOW].nField[29].nWillDo	= GuardStoneStatus[1];
	SMenu[MN_NATION_NOW].nField[30].nWillDo	= GuardStoneStatus[2];
	SMenu[MN_NATION_NOW].nField[31].nWillDo	= GuardStoneStatus[3];
}

void NationSupportNationNow2(t_packet *p)
{
	DWORD RemainTime=p->u.NationWar.WarfieldData2.RemainTime;
	WORD  Hour, Min, Sec;		// 011020 LTS
	GetTime(RemainTime,Hour,Min,Sec);
	SMenu[MN_NATION_NOW].nField[5].nRectImage = RemainTime;//남은 시간
	
	if( isAttacker() )//내가 공격자 인가?
	{
		sprintf(SMenu[MN_NATION_NOW].nField[6].temp,  "%d",p->u.NationWar.WarfieldData2.AttackerCount );//아군 숫자
		sprintf(SMenu[MN_NATION_NOW].nField[7].temp,  "%d",p->u.NationWar.WarfieldData2.DefencerCount );//적군 숫자
	}
	else	//내가 방어자?
	{
		sprintf(SMenu[MN_NATION_NOW].nField[6].temp,  "%d",p->u.NationWar.WarfieldData2.DefencerCount);//아군 숫자
		sprintf(SMenu[MN_NATION_NOW].nField[7].temp,  "%d",p->u.NationWar.WarfieldData2.AttackerCount);//적군 숫자
	}
	
	
}

void NationSupportNationNow3(t_packet *p)
{
//	AddCurrentStatusMessage( 255,255,0, "부대종류 : %d",p->u.NationWar.WarfieldData3.Type);
	//	일반인들
	//	AddCurrentStatusMessage( 255,255,0, "0부대 대원 수 : %d",p->u.NationWar.WarfieldData3.MemberCount[0]);
	//011030 lsw >
	sprintf(SMenu[MN_NATION_NOW].nField[8].temp,	"%d",p->u.NationWar.WarfieldData3.MemberCount[1] );//부대원 수 1
	sprintf(SMenu[MN_NATION_NOW].nField[9].temp,	"%2d", NW_SQUAD1_MAN_MAX );
	sprintf(SMenu[MN_NATION_NOW].nField[10].temp,	lan->OutputMessage(0,408),1 );
	sprintf(SMenu[MN_NATION_NOW].nField[11].temp,	"%d"	,p->u.NationWar.WarfieldData3.MemberCount[2] );//부대원 수 2
	sprintf(SMenu[MN_NATION_NOW].nField[12].temp,	"%2d" ,NW_SQUAD2_MAN_MAX);
	sprintf(SMenu[MN_NATION_NOW].nField[13].temp,	lan->OutputMessage(0,408),2);
	sprintf(SMenu[MN_NATION_NOW].nField[14].temp,	"%d"	,p->u.NationWar.WarfieldData3.MemberCount[3] );//부대원 수 3
	sprintf(SMenu[MN_NATION_NOW].nField[15].temp,   "%2d",NW_SQUAD3_MAN_MAX );
	sprintf(SMenu[MN_NATION_NOW].nField[16].temp,	lan->OutputMessage(0,408),3 );
	sprintf(SMenu[MN_NATION_NOW].nField[17].temp,	"%d"	,p->u.NationWar.WarfieldData3.MemberCount[4] );//부대원 수 4
	sprintf(SMenu[MN_NATION_NOW].nField[18].temp,   "%2d",NW_SQUAD4_MAN_MAX );
	sprintf(SMenu[MN_NATION_NOW].nField[19].temp,	lan->OutputMessage(0,408),4 );
	//011030 lsw <
	CallSmallMenu(MN_NATION_NOW,MN_NATION_BATTLE_BBS);//실질적 호출
}

void NationSupportNationSquadLeader(t_packet* p)
{
	//011004 lsw
	int sqn = p->u.NationWar.SquadInfo.SquadNo;
	sprintf(SMenu[MN_NATION_BUDAE].nField[6].temp,  lan->OutputMessage(0,408),sqn );//011030 lsw
	
	switch (sqn)
	{
	case 1:
		sprintf(SMenu[MN_NATION_BUDAE].nField[9].temp,  "%2d-%2d" ,NW_SQUAD1_LEVEL_MIN ,NW_SQUAD1_LEVEL_MAX);//레벨
		break;
	case 2:
		sprintf(SMenu[MN_NATION_BUDAE].nField[9].temp,  "%2d-%2d" ,NW_SQUAD2_LEVEL_MIN ,NW_SQUAD2_LEVEL_MAX);//레벨
		break;
	case 3:
		sprintf(SMenu[MN_NATION_BUDAE].nField[9].temp,  "%2d-%2d" ,NW_SQUAD3_LEVEL_MIN ,NW_SQUAD3_LEVEL_MAX);//레벨
		break;
	case 4:
		sprintf(SMenu[MN_NATION_BUDAE].nField[9].temp,  "%2d-%2d" ,NW_SQUAD4_LEVEL_MIN ,NW_SQUAD4_LEVEL_MAX);//레벨
		break;
	}
	
	SMenu[MN_NATION_BUDAE].nField[7].nType = FT_NOMAL_PUT;
	switch(Hero->name_status.nation)
	{
	case NW_BY:
		SMenu[MN_NATION_BUDAE].nField[7].nImageNumber = 20+sqn;
		break;
	case NW_ZY:
		SMenu[MN_NATION_BUDAE].nField[7].nImageNumber = 24+sqn;
		break;
	case NW_YL:
		SMenu[MN_NATION_BUDAE].nField[7].nImageNumber = 28+sqn;
		break;
	}

	if(	p->u.NationWar.SquadInfo.ChipID !=0  )
	{
		sprintf(SMenu[MN_NATION_BUDAE].nField[8].temp,  "%s",p->u.NationWar.SquadInfo.ChipName);
		sprintf(SMenu[MN_NATION_BUDAE].nField[17].temp,  "%s" ,ItemRare[p->u.NationWar.SquadInfo.Sok[0]].LocalName);
		sprintf(SMenu[MN_NATION_BUDAE].nField[18].temp,  "%s" ,ItemRare[p->u.NationWar.SquadInfo.Sok[1]].LocalName);
		if( sqn  == 3 || sqn ==4 )
		{
			memset(SMenu[MN_NATION_BUDAE].nField[19].temp,0,500);
		}
		else 
		{
			sprintf(SMenu[MN_NATION_BUDAE].nField[19].temp,  "%s" ,ItemRare[p->u.NationWar.SquadInfo.Sok[2]].LocalName);
		}
		
	}
	else
	{
//		SMenu[MN_NATION_BUDAE].nField[7].nType = 0;//부대 마크 안뜸
		//011030 lsw >
		sprintf(SMenu[MN_NATION_BUDAE].nField[8].temp,	lan->OutputMessage(0,405) 	);//,p->u.NationWar.SquadInfo.ChipName);
		sprintf(SMenu[MN_NATION_BUDAE].nField[17].temp, lan->OutputMessage(0,405)	);//,p->u.NationWar.SquadInfo.Sok);
		sprintf(SMenu[MN_NATION_BUDAE].nField[18].temp, lan->OutputMessage(0,405)	);//,p->u.NationWar.SquadInfo.Sok);
		sprintf(SMenu[MN_NATION_BUDAE].nField[19].temp, lan->OutputMessage(0,405)	);//,p->u.NationWar.SquadInfo.Sok);
		//011030 lsw <
	}
}

void NationSupportNationSquadLeader2(t_packet *p)		// 011023 LTS 함수교체
{
	char msg[100];

	//011030 lsw >
	sprintf(msg,lan->OutputMessage(0,406),1,p->u.NationWar.SquadInfo2.SquadCount[1], NW_SQUAD1_MAN_MAX);
	strcpy(SMenu[MN_NATION_BUDAE].nField[12].temp,  msg );
	sprintf(msg,lan->OutputMessage(0,406),2,p->u.NationWar.SquadInfo2.SquadCount[2], NW_SQUAD2_MAN_MAX);
	strcpy(SMenu[MN_NATION_BUDAE].nField[13].temp,  msg );
	sprintf(msg,lan->OutputMessage(0,406),3,p->u.NationWar.SquadInfo2.SquadCount[3], NW_SQUAD3_MAN_MAX);
	strcpy(SMenu[MN_NATION_BUDAE].nField[14].temp,  msg );
	sprintf(msg,lan->OutputMessage(0,406),4,p->u.NationWar.SquadInfo2.SquadCount[4], NW_SQUAD4_MAN_MAX);
	strcpy(SMenu[MN_NATION_BUDAE].nField[15].temp,  msg );

	int squadno = Hero->NWCharacter.SquadNo;
	
	switch (squadno)
	{
	case 1:
		sprintf(msg,lan->OutputMessage(0,407),squadno,p->u.NationWar.SquadInfo2.SquadCount[squadno],NW_SQUAD1_MAN_MAX);
		strcpy(SMenu[MN_NATION_BUDAE].nField[squadno+11].temp,  msg );
		break;
	case 2:
		sprintf(msg,lan->OutputMessage(0,407),squadno,p->u.NationWar.SquadInfo2.SquadCount[squadno],NW_SQUAD2_MAN_MAX);
		strcpy(SMenu[MN_NATION_BUDAE].nField[squadno+11].temp,  msg );
		break;
	case 3:
		sprintf(msg,lan->OutputMessage(0,407),squadno,p->u.NationWar.SquadInfo2.SquadCount[squadno],NW_SQUAD3_MAN_MAX);
		strcpy(SMenu[MN_NATION_BUDAE].nField[squadno+11].temp,  msg );
		break;
	case 4:
		sprintf(msg,lan->OutputMessage(0,407),squadno,p->u.NationWar.SquadInfo2.SquadCount[squadno],NW_SQUAD4_MAN_MAX);
		strcpy(SMenu[MN_NATION_BUDAE].nField[squadno+11].temp,  msg );
		break;
	}
	//011030 lsw <
	
	sprintf(msg,"%s",p->u.NationWar.SquadInfo2.CommanderName);//사령관 명
	strcpy(SMenu[MN_NATION_BUDAE].nField[16].temp,  msg );

	CallSmallMenu(MN_NATION_BUDAE,MN_NATION_BATTLE_BBS);							// Squad Interface Open
}

extern BOOL IsNeoWarfieldServer();		// LTH-040303-KO

//011001 lsw
void NationWarResultDoneButton()//결과 에서 done 누르면 들어옴
{
	if (isNationWarfieldServer())
		SendCMD_CHECK_AM_I_DEFEAT();
	//< LTH-040303-KO 1.4 Patch. 신 전쟁터 추가
	if (isNewWarfieldServer() || IsNeoWarfieldServer())
		SendCMD_CHECK_DEFEAT();
	//> LTH-040303-KO
}

void TodayNoticeButtonIntoColor(int color)//1이면 흰색 2 이면 빨간색
{
	if (color > 2 || color < 0 ){ return; }

	SMenu[MN_TIME_DISPLAY_MENU].nField[3].nWillDo = color;
	return;
}

bool DoButtonCheckOfMenu_SpecialWillDo_lsw(int i, int j)//020515 lsw
{
	const int type =  SMenu[i].nField[j].nSpecialWillDo;
	int &nWillDo	= SMenu[i].nField[j].nWillDo;
	int &iWork		= SMenu[i].work;

	switch(type)
	{					
	case SWD_NATION_INFO: //전쟁터정보 버튼  //현재는 공격로지정으로 사용중..
		SendNationPacket(BUTTON_NATION_INFO);
		break;

	case SWD_NATION_LADDER://명예의 전당 
		SendNationPacket(BUTTON_HALLOFFAME);
		//011025 lsw >
		CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(1,402), FALSE);
		//011025 lsw <
		break;
	//010928 lsw
	case SWD_NATION_WEAPON_BUY:
		//011025 lsw >
		CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(1,402), FALSE);
		//011025 lsw <
		break;

	case SWD_NATION_DECSRIPT://전쟁 설명:
		SendNationPacket(BUTTON_WAR_DESCRIPTION);
		break;

	case SWD_NATION_NOW://전쟁상황 
		SendNationPacket(BUTTON_NATION_STATUS);    
		break;

	case SWD_NATION_JOIN: //전쟁참여 
		if (true == g_cPrison.IsBadUser())
		{
			t_packet tagPacket;
			tagPacket.u.kein.tagPrisonSysProcResult.nReason = (int)(CPrison::PSR_CANNOT_USE);
			g_cPrison.RecvPrisonSystemResult(&tagPacket);
			break;
		}
		SendNationPacket(BUTTON_NATION_JOIN);
		break;
	case SWD_NATION_BUDAE: //부대편성 
		if (true == g_cPrison.IsBadUser())
		{
			t_packet tagPacket;
			tagPacket.u.kein.tagPrisonSysProcResult.nReason = (int)(CPrison::PSR_CANNOT_USE);
			g_cPrison.RecvPrisonSystemResult(&tagPacket);
			break;
		}
		SendNationPacket(BUTTON_NATION_SQUAD);
		break;
	case SWD_NATION_GUEST://용병으로 참가
		if (true == g_cPrison.IsBadUser())
		{
			t_packet tagPacket;
			tagPacket.u.kein.tagPrisonSysProcResult.nReason = (int)(CPrison::PSR_CANNOT_USE);
			g_cPrison.RecvPrisonSystemResult(&tagPacket);
			break;
		}
		SendNationPacket(BUTTON_JOIN_FREELANCER);
		break;
		
	case SWD_NATION_GOTO_WAR_MAP://전쟁터 맵으로 이동

		{
		if (true == g_cPrison.IsBadUser())
		{
			t_packet tagPacket;
			tagPacket.u.kein.tagPrisonSysProcResult.nReason = (int)(CPrison::PSR_CANNOT_USE);
			g_cPrison.RecvPrisonSystemResult(&tagPacket);
			break;
		}

			int WarfieldNo  = CheckNationStatus(NW_WAR);

			if (WarfieldNo==-1)		// 전쟁중이 아닐때
				CallSmallMenu(MN_NATION_GOTO_WAR_MAP,MN_NATION_BATTLE_BBS); 
			else					// 전쟁중일때
				 CallMapMoveMenu(WarfieldNo); 
		}
		break;
	case SWD_NATION_VOTE: //사령관투표
		SendNationPacket(BUTTON_NATION_CHECK_COMMANDER);
		break;

	//로딩 포인트 호출 부분
	case SWD_NATION_ATTACK_WAY_POINT:
		SendNationPacket(BUTTON_NATION_LOADING_POINT);
		break;

	//국가전 메인 인터 페이스 아닌데있는 필드
	case SWD_NATION_JOIN_JOIN_WAR: //전쟁참여투표 
		{
		if (true == g_cPrison.IsBadUser())
		{
			t_packet tagPacket;
			tagPacket.u.kein.tagPrisonSysProcResult.nReason = (int)(CPrison::PSR_NOT_MOVE);
			g_cPrison.RecvPrisonSystemResult(&tagPacket);
			break;
		}
			SendNationPacket(BUTTON_NATIONWAR_JOIN_VOTE);  //참여한 후.. 닫는다..
		//Send_NATION_JOIN_JOIN_WAR();
		}break;
	case SWD_NATION_NOW_REFRESH:  // 전쟁현황 리프레쉬 
		//wsprintf(SMenu[MN_NATION_NOW].nField[2].temp,"아아아 리프레쉬~");//여기에 문자열이 들어 갑니다.
		{	
			SendNationPacket(BUTTON_NATION_STATUS);    
		}break;				
	case SWD_NATION_NOW_BUDAE:
		{
			//전쟁터 현황에서 부대를 클릭하면 들어 오는곳
			//입맛에 맞게 선택 하세요
			//Send_NATION_NOW_REFRESH();
		}

/********************************************************************
					부대 관련 버튼 
********************************************************************/
	//이전
	case SWD_NATION_BUDAE_PREV_REFRESH	:
		{
		SendCMD_REQUEST_SQUAD_INFO(--g_Squad);
		}break;
	//다음
	case SWD_NATION_BUDAE_NEXT_REFRESH	:
		{
		SendCMD_REQUEST_SQUAD_INFO(++g_Squad);
		}break;
	//부대에 등록
	case SWD_NATION_BUDAE_JOIN			:  //부대에 조인 
		if(NationSupportCheckSquadJoin())
		{
			//010928 lsw
			CallNationYesOrNo(SWD_NATION_BUDAE_JOIN,g_Squad); 
		}
		else 
		{
			//011030 lsw >
			CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0, 403), FALSE);
			//011030 lsw <
		}
		break;
	case SWD_NATION_VOTE_PREV_REFRESH	:// 1은 나중에 현재 부대 갱신에 쓸 예정
		{
			SendCMD_REQUEST_COMMANDER_VOTE_DATA(--g_Commander);//이전 -> -1 다음 -> 1
		}break;

	case SWD_NATION_VOTE_NEXT_REFRESH	:
		{
			SendCMD_REQUEST_COMMANDER_VOTE_DATA(++g_Commander);//이전 -> -1 다음 -> 1
		}break;
	case SWD_NATION_VOTE_I_AM_HERO	: //사령관등록 
		if(NationSupportCheckCanWillCommander())
		{
			CallNationYesOrNo(SWD_NATION_VOTE_I_AM_HERO,0);
		}
		else
		{
			CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0, 404), FALSE);
		}
		break;
	case SWD_NATION_VOTE_SELECT_YOU	:  // 사령관지지 
		{
			SendNationPacket(BUTTON_COMMANDER_VOTE);
		}break;
	case SWD_NATION_INFO_SELECT	://국가전 로딩 포인트 선택
		{// 여기에 1 2 3 4 가 들어 있으니 그냥 함수에 넘기면 됩니다.
			SendCMD_SET_SQUAD_LODING_POINT(GetSquadNo(),nWillDo); // 010715 LTS TEMP
		}break;
	case	SWD_NATION_GUEST_BUDAE:
		{
			SendNationPacket(BUTTON_FREELANCER_SQUAD);
		}break;
	case	SWD_NATION_GUEST_JOIN:
		{
			SendNationPacket(BUTTON_FREELANCER_SQUAD+nWillDo);
		}break;
	case	SWD_NATION_TODAY_NOTICE_IMAGE:
		{
			char *msg = SMenu[i].nField[j].temp;
			switch( msg[0] )
			{
			case '1': 
				MP3(54);
				break;
			case '2': 
				MP3(50);
				break;
			case '3': 
				MP3(51);
				break;
			}
			strcpy(SMenu[MN_NATION_TODAY_NOTICE].nField[7].temp, msg+18 );
		}break;

	case	SWD_NATION_MSGBOX_OK:
		{
			DoButtonNationOkCancel_lsw(SMenu[MN_NATION_MSGBOX].nField[1].nWillDo);//이 값을 참조 하여 일을 한다
		}break;
	case		SWD_NATION_WAR_RESULT:
		{
			NationWarResultDoneButton();
			SMenu[i].bActive = false;
		}break;
	case	SWD_NATION_GOTO_WAR_MAP_TARGET:
		{
			SendCMD_NW_MAP_MOVE(0,nWillDo);//1 2 3 이 넘어옴 전쟁터 넘버
		}break;
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//		전투스킬 관련
	case SWD_CSP_GROUP_ICON:
		{
			const int iSelectGroup = nWillDo;
			SMenu[i].work = iSelectGroup;
			SetCSPButtonType(iSelectGroup);
			SetCSPGroupButtonImage(iSelectGroup);
			SetCSPIconImageANDnWillDo(iSelectGroup);
			SMenu[MN_COMBAT_SKILL_POINT].nField[3].nWillDo	= 0;//Done 버튼을 초기화 시킨다
			SMenu[MN_COMBAT_SKILL_POINT].nField[15].nWillDo = 0;//어빌리티 업 버튼 초기화
		}break;
	case SWD_CSP_GROUP_SELECT:
		{	//아무것도 안 배웠을때 그룹을 선택하면 nWillDo 값(그룹 타입)을 Done에 넘겨주면
			//Done은 메인 인터페이스의 work(그룹넘버)와 받아놓은 nWillDo를 가지고 버튼들(6개)를 셋팅
			SMenu[MN_COMBAT_SKILL_POINT].nField[3].nWillDo = nWillDo;//3번이 Done버튼
			
			//누르면 버튼 그림을 바꾼다 원래 대로
			SMenu[MN_COMBAT_SKILL_POINT].nField[13].nImageNumber = 33;
			SMenu[MN_COMBAT_SKILL_POINT].nField[13].nRectImage	 = 34;

			SMenu[MN_COMBAT_SKILL_POINT].nField[14].nImageNumber = 32;
			SMenu[MN_COMBAT_SKILL_POINT].nField[14].nRectImage	 = 35;
			
			SMenu[i].nField[j].nImageNumber = SMenu[i].nField[j].nRectImage;//3번이 Done버튼
		}break;
	case SWD_CSP_GROUP_SELECT_DONE:
		{
			const int iForceGroupType = nWillDo;
			if( !iForceGroupType )
			{
				CloseAllMenu();
				break;
			}

			///Done은 메인 인터페이스의 work(그룹넘버)와 받아놓은 nWillDo를 가지고 버튼들(6개)를 셋팅
			SetCSPButtonTypeWhenSelectGroupType();
			
			SetCSPIconImageANDnWillDo(SMenu[MN_COMBAT_SKILL_POINT].work,iForceGroupType);
		}break;			
	case SWD_PARTY_ALL_DEL://파티 해제
		{
			memset( SCharacterData.party, 0, sizeof( CharacterParty ) * 6 );
			CallServer( CMD_PARTY_ALL_FREE ); 
		}break;
	case SWD_CALL_COMBAT_SKILL_POINT:
		{ //< CSD-020611
			g_mgrBattle.SendCombatRequest(Hero);//021001 lsw
		}break; //> CSD-020611
	case SWD_MAININTERFACE_ABILITYTOSTATUS :	
			{
				SMenu[MN_ABILITY].bActive=FALSE;
				MenuStack.DelMenu(MN_ABILITY);
				SMenu[MN_STATUS].bActive=TRUE;
				MP3( SN_MENU_OPEN );
				SMenu[MN_STATUS].x = 0;
				MenuStack.PutMenu(MN_STATUS);
				/*	//020515 lsw
				SMenu[MN_STATUS].nField[6].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+154;
				SMenu[MN_STATUS].nField[7].nImageNumber=SCharacterData.nCharacterData[ARIGEMENT]+163;
				*/
				return true; 
			}break;   
	case SWD_MAININTERFACE_ABILITY :	
		{
			SMenu[MN_STATUS].bActive=FALSE;
			SMenu[MN_TACTICS].bActive=FALSE;//020828 lsw
			MenuStack.DelMenu(MN_STATUS);
			MenuStack.DelMenu(MN_TACTICS);//020828 lsw
			SMenu[MN_ABILITY].bActive=TRUE;
			MP3( SN_MENU_OPEN );
//			CallServer( CMD_OPEN_ABILITY );			// ability 메뉴가 온 될때 서버에서 전송받아야 할것 들 요청
			MenuStack.PutMenu(MN_ABILITY);													
			return true; 
		}break;
	case SWD_STATUS_TACTICS :	
		{
			SMenu[MN_STATUS].bActive=FALSE;
			SMenu[MN_ABILITY].bActive=FALSE;//020828 lsw
			MenuStack.DelMenu(MN_STATUS);
			MenuStack.DelMenu(MN_ABILITY);//020828 lsw
			SMenu[MN_TACTICS].bActive=TRUE;
			MP3( SN_MENU_OPEN );
			MenuStack.PutMenu(MN_TACTICS);
			return true; 
		}break;
	case SWD_TACTICS_STATUS :	
		{
			SMenu[MN_TACTICS].bActive=FALSE;
			SMenu[MN_ABILITY].bActive=FALSE;//020828 lsw
			MenuStack.DelMenu(MN_TACTICS);
			MenuStack.DelMenu(MN_ABILITY);//020828 lsw
			SMenu[MN_STATUS].bActive=TRUE;
			MP3( SN_MENU_OPEN );
			MenuStack.PutMenu(MN_STATUS);
			return true;//Main On이다 
		}break;   
	case SWD_TACTICS_ABILITY:
		{
			SMenu[MN_TACTICS].bActive=FALSE;
			MenuStack.DelMenu(MN_TACTICS);
			SMenu[MN_ABILITY].bActive=TRUE;
			MP3( SN_MENU_OPEN );
			MenuStack.PutMenu(MN_ABILITY);
			return true;//Main On이다
		}break;
	case SWD_CALL_FRIEND_MENU:
		{
			CallFriendMenu();
		}break;
	case SWD_MOVE_TO_HOMETOWN_WITH_ALIVE:
		{
			if( 20 >= Hero->lv  && 0 < Hero->lv)//20 이하만 사용 가능
			{
				SendMoveToHomeTownWithAlive();	
				return false;
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(0,20));
			}
		}break;
	case SWD_MOVE_TO_HOMETOWN:
		{
			if( 20 >= Hero->lv  && 0 < Hero->lv)//20 이하만 사용 가능
			{
				SendMoveToHomeTown();
				return false;
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(0,20));
			}
		}break;
	case SWD_SEND_GET_MERCHANT_BUY_LIST://soto-030511
		{
			Auction.SendCMD_MERCHANT_BUY_LIST_REQUEST();//구매자가 판매물품을 검색
		}break;
	case SWD_SEND_GET_MERCHANT_SELL_LIST:
		{
			Auction.SendCMD_MERCHANT_SELL_LIST_REQUEST();//판매자가 자신의 정보를 요청
		}break;
	case	SWD_SEND_MERCHANT_BUY_THIS_ITEM:
		{
			SEARCHRESULT *SR = Auction.GetSearchList().ResultList;
			const int iBuyItemIndex = Auction.GetBuyItemIndex();
			if(!iBuyItemIndex){break;}
			for(int xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
			{
				if(SR[xx].iIndex == iBuyItemIndex)
				{
					Auction.SendCMD_MERCHANT_ITEM_BUY(SR[xx]);//판매자가 자신의 정보를 요청
					break;
				}
			}
		}break;
	case	SWD_SEND_MERCHANT_SELL_ITEM_DELETE:
		{
			Auction.SendCMD_MERCHANT_SELL_ITEM_DELETE();//판매자가 등록된 아이템을 판매취소함
		}break;
	case	SWD_SEND_MERCHANT_SELL_ITEM_REGISTER:
		{
			Auction.SendCMD_MERCHANT_SELL_ITEM_REGISTER();//판매자가 아이템을 등록함
			::CallSmallMenu(MN_MERCHANT_SELL_INFO);
		}break;
	case SWD_MERCHANT_SELL_TYPE_SET:
		{//021113 lsw
			nWillDo = !nWillDo;
			if(!nWillDo)//직접거래다
			{
				Auction.SetSellValue(0);
				EWndMgr.SetTxt( HWND_1,"0");//021001 lsw
			}
		}break;
	case	SWD_CALL_MERCHANT_REGISTER:
		{
			::Call_MERCHANT_REGISTER_Menu();
		}break;
	case SWD_SEND_MERCHANT_RESULT_ITEM_TAKE:
		{
			SEARCHRESULT *SR = Auction.GetSearchList().ResultList;
			const int iTakeItemIndex = Auction.GetTakeItemIndex();
			for(int xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
			{
				if(SR[xx].iIndex == iTakeItemIndex)
				{
					Auction.SendCMD_MERCHANT_RESULT_TAKE(SR[xx]);//결과물 받기
					break;
				}
			}
		}break;
	case SWD_CALLSMALLMENU	:	
		{
			CallSmallMenu( nWillDo, i);
		}break;
	case SWD_SEND_GET_MERCHANT_RESULT_LIST:
		{
			Auction.SendCMD_MERCHANT_RESULT_LIST_REQUEST();//판매자가 아이템을 등록함
		}break;
//<! BBD 040303	상인 백업테이블에서 물건찾기
	case SWD_SEND_MERCHANT_BACKUP_ITEM_TAKE:
		{
			SEARCHRESULT *SR = Auction.GetSearchList().ResultList;
			const int iTakeItemIndex = Auction.GetTakeItemIndex();
			for(int xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
			{
				if(SR[xx].iIndex == iTakeItemIndex)
				{
					Auction.SendCMD_MERCHANT_BACKUP_TAKE_REQUEST(SR[xx]);//결과물 받기
					break;
				}
			}
		}break;
	case SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_UP:
		{
			Auction.DecPageBackupList();
		}break;
	case SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_DOWN:
		{
			Auction.IncPageBackupList();
		}break;
//> BBD 040303	상인 백업테이블에서 물건찾기
//<soto-030511
	case SWD_AUCTION_UPDOWN_BTN:
		{
			if( 0 == nWillDo ) Auction.DecPeriod();
			else Auction.IncPeriod();
		}
		break;
//>soto-030511

	case SWD_AUCTION_FIND_KEY_RARE_SET:
		{
//			( 1 == nWillDo )?Auction.FKRareTypeInc():Auction.FKRareTypeDec();
		}break;
	case SWD_AUCTION_FIND_KEY_LV_SET:
		{
//			( 1 == nWillDo )?Auction.FKLevelInc():Auction.FKLevelDec();
		}break;
	case SWD_AUCTION_FIND_KEY_TACTIC_SET:
		{
//			( 1 == nWillDo )?Auction.FKTacticTypeInc():Auction.FKTacticTypeDec();
		}break;
	case SWD_AUCTION_FIND_KEY_WEAR_SET:
		{
//			( 1 == nWillDo )?Auction.FKWearTypeInc():Auction.FKWearTypeDec();
		}break;	
	case SWD_SEND_MERCHANT_BUY_LIST_SCROLL_UP:
		{
			Auction.DecBuyerViewPage() ;
		}break;
	case SWD_SEND_MERCHANT_BUY_LIST_SCROLL_DOWN:
		{
			Auction.IncBuyerViewPage() ;
		}break;
	case SWD_SEND_MERCHANT_SELL_LIST_SCROLL_UP:
		{
			Auction.DecSellerViewPage();
		}break;
	case SWD_SEND_MERCHANT_SELL_LIST_SCROLL_DOWN:
		{
			Auction.IncSellerViewPage();
		}break;
	case SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_UP:
		{
			Auction.DecResultViewPage();
		}break;
	case SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_DOWN:
		{
			Auction.IncResultViewPage() ;			
		}break;
	case SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_UP://0211121 lsw
		{
			Auction.DecBuyerViewPage() ;
		}break;
	case SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_DOWN:
		{
			Auction.IncBuyerViewPage() ;
		}break;
	case SWD_CALL_MERCHANT_OR_SKILL_MASTER:
		{
			const int iResult = ::CheckMerchantOrSkillMaster();//될 수 있는게 뭔가를 체크 하고
			switch(iResult)
			{
			case GM_QUEST_MERCHANT://상인
				{
					::Call_MERCHANT_QUEST_MENU(iResult);//상인 퀘스트 메뉴를 불러라
				}break;
			case GM_QUEST_BLACKSMITH_MASTER://대장장이 기성
			case GM_QUEST_CARPENTER_MASTER:	//목수 기성    
			case GM_QUEST_BOWCRAFT_MASTER:	//활제작사 기성
			case GM_QUEST_TAILOR_MASTER	:	//재단사 기성
				{//기성이 될 수 있네.. 기성 고르는걸 넣어라
					::CallSmallMenu(MN_SKILL_MASTER_LIST);	
				//	::Call_MERCHANT_QUEST_MENU(nWillDo);
				}break;
			default:
				{
					::AddCurrentStatusMessage( FONT_COLOR_YELLOW, lan->OutputMessage(2,770));
				}break;
			}//switch(iResult)
		}break;//case SWD_CALL_MERCHANT_OR_SKILL_MASTER:
	case SWD_CALL_SKILL_MASTER_QUEST:
		{
			const int iResult = ::CheckMerchantOrSkillMaster();//될 수 있는게 뭔가를 체크 하고
			switch(iResult)
			{
			case GM_QUEST_MERCHANT://상인
				{
					::Call_MERCHANT_QUEST_MENU(iResult);//상인 퀘스트 메뉴를 불러라
				}break;
			case GM_QUEST_BLACKSMITH_MASTER://대장장이 기성
			case GM_QUEST_CARPENTER_MASTER:	//목수 기성    
			case GM_QUEST_BOWCRAFT_MASTER:	//활제작사 기성
			case GM_QUEST_TAILOR_MASTER	:	//재단사 기성
				{//기성이 될 수 있네.. 퀘스트 메뉴를 띄워
					::Call_MERCHANT_QUEST_MENU(nWillDo);
				}break;
			default:
				{
					::AddCurrentStatusMessage( FONT_COLOR_YELLOW, lan->OutputMessage(2,770));
				}break;
			}//switch(iResult)
		}break;//case SWD_CALL_SKILL_MASTER_QUEST:
	case SWD_GM_REGIST_2:
		{//021108 lsw
			const int ret = ::CanGmRegist(iWork);
			switch( ret )
			{
			case  1: ::SendGMRegist(iWork); break; // GM에 등록할수 있다... 체크 요망
			case -1: ::AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(3,187) ); MP3(SN_WARNING );break;//010215 lsw
			case -2: ::AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(3,188) ); MP3(SN_WARNING );break;//010215 lsw
			case -3: ::AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(3,189) ); MP3(SN_WARNING );break;//010215 lsw
			case -4: ::AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(3,190) ); MP3(SN_WARNING );break;//010215 lsw
			}
			::SmallMenuClose();
		}break;
	case SWD_SKILL_MASTER_MULTIRARE_OK:
		{
			ItemAttr Item1= g_MultiRareSrc.MultiRareSrc[0].attr;
			ItemAttr Item2= g_MultiRareSrc.MultiRareSrc[1].attr;
			
			if(!Item1.item_no ||!Item2.item_no){break;}//아이템 번호는 반드시있어야 하고
			if(Item1.item_no != Item2.item_no){break;}//아이템 번호가 같아야지

			const RareMain *pTempRare1 =  (RareMain *)&Item1.attr[IATTR_RARE_MAIN];
			const RareMain *pTempRare2 =  (RareMain *)&Item2.attr[IATTR_RARE_MAIN];
			if(	pTempRare1->IsDynamicRare
			||	pTempRare2->IsDynamicRare
			||	pTempRare1->iHighLevel//021119 lsw
			||	pTempRare2->iHighLevel){break;}//하이나 기능 아템은 출력 안시켜.. 틀린거 거든
			
			RareMain RareResult;
			{//결과물 셋팅
				RareResult.grade = (pTempRare1->grade+pTempRare2->grade)/2;
				RareResult.soksung1=pTempRare1->soksung1;
				RareResult.soksung2=pTempRare2->soksung1;
				RareResult.soksung3=0;
				RareResult.iHighLevel = 0;
				RareResult.IsDynamicRare = false;
			}
			const RareMain *pRare = &RareResult;
			const int iGrade = pRare->grade;

			bool bIsHaveResource = true;
			for(int xx= 0;MAX_IMR_FIELD_CT>xx;xx++)
			{
				int iNationAddCt = 0;
				switch(Hero->name_status.nation)
				{
				case NW_BY:{if(xx == 0){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				case NW_ZY:{if(xx == 1){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				case NW_YL:{if(xx == 2){ iNationAddCt = aIMR[iGrade].iAddCt;}}break;
				}
				const int iNeedItemNo = aIMR[iGrade].aItemNo[xx];
				const int iNeedItemCt = aIMR[iGrade].aItemCt[xx]+iNationAddCt;
				if(iNeedItemNo && iNeedItemCt)
				{
					CItem *t = ItemUnit( iNeedItemNo );
					if(!t){continue;}
					const int iNowCt = ::CheckInventory(iNeedItemNo,iNeedItemCt);//내가 원한 값에서 현재 결과를 주니까 -값이 나올 수 있다
					if(0> iNowCt)
					{
						bIsHaveResource = false;
					}
				}
			}
			if(bIsHaveResource)//021113 lsw test
			{
				::SendCMD_SKILL_MASTER_MULTI_RARE_MAKE(SkillNo);
				SkillStatus	= SKILL_OFF;
				nItemOther = 0 ;
				SkillSettingClear();
				SmallMenuClose(); 
			}
		}break;
	case SWD_EXCHANGE_BOND_MONEY_OK://021126 lsw
		{
			Auction.SendCMD_EXCHANGE_BOND_MONEY();//021202 lsw
//			::CloseAllMenu();
		}break;
	case SWD_CALL_EXCHANGE_BOND_MONEY:
		{
			::Call_EXCHANGE_BOND_MONEY_Menu();
		}break;
	case SWD_MAIL_WRITE_SEND :
		{
			char szTarget[100] = {0,};
			char szTitle[60] = {0,};
			char szBody[1000]= {0,};

			EWndMgr.GetTxt( HWND_MAIL1, szTarget,	100);
			EWndMgr.GetTxt( HWND_MAIL2, szTitle,	60);
			EWndMgr.GetTxt( HWND_MAIL3, szBody,		1000);
			// 031110 YGI
			char *div_str = DivideStringByRect( 315, szBody, 16 );	// My YGI
			if( div_str)
			{
				*( div_str-1 ) = NULL;		// \n 처리를 위해서..
			}
//			int ret = CheckCharNameAndNation( name );		// 그 사람과 내가 같은 나라인가?
//			if( !ret ) 
//			{
//				CallOkCancelMessageBox( i, 0, 0, lan->OutputMessage(3,178) );//010215 lsw
//			}
//			else 
//			{
//				ChangeString( body, '\r', ' ');
//				SendMail( name, title, body );
//				EWndMgr.ClearAllTxt();//021001 lsw
//				SmallMenuClose();
//				break;
//			}
			
			::ChangeString( szTitle, '\r', ' ');
			::ChangeString( szBody, '\r', ' ');

			if(g_MailMgr.Send(g_MailMgr.GetTempMailType(),szTarget,szTitle,szBody))
			{
				::CallOkCancelMessageBox( i,100, 100, lan->OutputMessage(3,290), true);
			}
		}break;
	case SWD_MAIL_WRITE_DELETE : 
		{
			EWndMgr.ClearTxt(HWND_MAIL1);//021001 lsw
			EWndMgr.ClearTxt(HWND_MAIL2);//021001 lsw
			EWndMgr.ClearTxt(HWND_MAIL3);//021001 lsw
			SMenu[i].work = 1;		// 이름 쓰는 칸으로 돌려 놓는다.
		 }break;
	case SWD_MAIL_WRITE_EXIT:
		{
			EWndMgr.ClearAllTxt();//021001 lsw
			CallMenu( SMenu[i].nTemp );
		 }break;
	case SWD_CALL_MAILMENU:
		{
			g_MailMgr.SendReqMailList(nWillDo,0);
		}break;
	case SWD_MAIL_DELETE://지우기 
		{
			::CallOkCancelMessageBox( i,100, 100, lan->OutputMessage(3,291), true);
			// ok만 있는 건지 ok_cancel이 있는 메시지 박스인지를 선택,  
			//menu는 호출한 메뉴를 넣는다
		}break;
//	case SWD_MAIL_DELETE://지우기 
//		{
//			g_MailMgr.SendDelete(nWillDo);
//		}break;
	case SWD_MAIL_SAVE://저장하기
		{
			g_MailMgr.SaveMailToLocal();
		}break;
	case SWD_CALL_MAIL_WRITE://편지쓰기
		{
			::Call_MAIL_WRITE_MENU(g_MailMgr.GetTempMailSender(),nWillDo?true:false);
		}break;
	case SWD_MAIL_REPAIR:
		{
			g_MailMgr.SendRepair();
			g_MailMgr.SendReqMailList(nWillDo,0);
		}break;
	case SWD_MAIL_SCROLL:
		{
			g_MailMgr.SendReqOtherPage(nWillDo);
		}break;
	case SWD_MAIL_SELECT_ALL:
		{
			g_MailMgr.SelectAll();
		}break;
	case SWD_TEMP_MAIL_TYPE_SCROLL:
		{
			switch(nWillDo)
			{
			case SCROLL_UP:
				{
					g_MailMgr.IncTempMailType();
				}break;
			case SCROLL_DOWN:
				{
					g_MailMgr.DecTempMailType();
				}break;
			}
		}break;
	case SWD_MAIL_SENDER_REGIST_FRIEND:
		{
			::RegistFriend(g_MailMgr.GetTempMailSender());
		}break;
	case SWD_OBSERVER_TEAM_SELECT:
		{
			::SendObserverTeamSelect();
			CloseAllMenu();//닫힙니다.
		}break;
	case SWD_SEND_MY_ARENA_BETTING_MONEY:
		{
			char temp[20] = {0,};
			EWndMgr.GetTxt( HWND_3, temp,20 );
			::EatRearWhiteChar( temp );
			DWORD dwMoney = ::atol( temp );

			::SendMyArenaBettingMoney(dwMoney);
			CloseAllMenu();//닫힙니다.
		}break;
	case SWD_CALL_ARENA_OBSERVER_MENU:
		{	//< CSD-030521
			if (CanCallObserverMenu()) 
			{
				CallMenu( SMenu[i].nField[j].nWillDo, i, (bool)true );
			}
			else
			{
				OUTPUT(MK_INFORMATION, lan->OutputMessage(1, 101));
			}

			break;
		}	//> CSD-030521
	case SWD_CALL_ARENA_BETTING_MENU:
		{
			if(CanCallBettingMenu())
			{
				CallMenu( SMenu[i].nField[j].nWillDo, i, (bool)true );
			}			
		}break;
	default :
		{
			return false;
		}
	}
	return true;//항상 메인 온
}

void SendMoveToHomeTown()
{
	t_packet packet;
	packet.h.header.type = CMD_MOVE_TO_HOMETOWN;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}

void SendMoveToHomeTownWithAlive()
{
	t_packet packet;
	packet.h.header.type = CMD_MOVE_TO_HOMETOWN_WITH_ALIVE;
	packet.h.header.size = 0;
	QueuePacket( &packet, 1 );
}
