#include "stdafx.h"

#pragma once
enum eIntoNationNoticeType
{
	ALL_CLEAR	=0x0000,  
	FORCE_TYPE	=0x0001, //iPosition 1-3 
	QUEUE_TYPE	=0x0002, 
};

enum eINNTQueueType
{
	FIRST_IN	=0x0001,
	LAST_IN		=0x0002,	
};

extern int CallNationYesOrNo(int iPrevType, int temp);
extern bool DoButtonNationOkCancel_lsw(int iOkType);
extern bool NationSupportCheckSquadJoin();
extern bool NationSupportCheckCanWillCommander();
extern bool DoButtonCheckOfMenu_lsw(int i, int j);
extern void NationSupportNationNow1(t_packet *p);
extern void NationSupportNationNow2(t_packet *p);
extern void NationSupportNationNow3(t_packet *p);
extern void NationSupportNationSquadLeader(t_packet *p);
extern void NationSupportNationSquadLeader2(t_packet *p);
extern void NationSupportRecvNationWarResult(t_packet *p);
extern void WarMenuNoticeOpen();
extern void TodayNoticeButtonIntoColor(int color);
extern void	IntoNationNotice(const eIntoNationNoticeType iStyleFlag,int iPosition, char *Msg);//QUEUE Style //INTOPOSTION Style // ALL Clear // 