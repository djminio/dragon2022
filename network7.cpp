
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 021010
	//����Ʈ�� ���� �����Լ��� �Ǵ� kyo�� ÷���� �����

	extern�� network7.h��
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "stdafx.h"
#include "network.h"
#include "gameproc.h"
#include "map.h"
#include "CMessage.h"
#include "menudef2.h"
#include "hangul.h"
#include "QuestFunction.h"
#include "Menu.h"//soto-030429

extern int y_MenuFocus;//soto-030429

int GetMapNumberByMapName( const char *szMapName);	//���̸����� �ʹ�ȣ�˾ƿ��� ������ -1��ȯ


//�ʿ� � ����Ʈ�� �����������Ǿ��°� ?
void SendWhatQuestInMap( char * szMapName);		//������ȣ�� ���� �ش� ������ ����Ʈ���������� �ȴ�.	
void RecvQuestInMap( t_quest_in_map *tp);		//����Ʈ ���������� �����Ѵ�.
void RecvSHOW_COLER_MSG( t_show_msg *tp );		//�ػ� ������ �Ž����� ����Ѵ�. ( AddCurrentStatusMessage��� )


int GetInt4Test( const char* szApp, const char* szKey, const char* szFile)
{// 030513 kyo //for test
	if( szApp == NULL || szKey == NULL )
	{
		return 1;
	}

	return GetPrivateProfileInt( szApp, szKey, 1, szFile );
}

void GetString4Test( char* szString,  const char* szApp, const char* szKey, const char* szFile)
{// 030513 kyo //for test
	if( szApp == NULL || szKey == NULL )
	{
		strcpy( szString, "");
	}
	GetPrivateProfileString( szApp, szKey, "", szString, 1024, szFile );
}


void SendWhatQuestInMap( char * szMapName )	//������ȣ�� ���� �ش� ������ ����Ʈ���������� �ȴ�.
{
	t_packet packet;
	packet.h.header.type = CMD_WHAT_QUEST_IN_MAP;
	packet.h.header.size = sizeof(t_quest_in_map);
	packet.u.quest_in_map.iQuestInfo = 0;		//
	strcpy( packet.u.quest_in_map.name, Hero->name );
	packet.u.quest_in_map.name[20] = '\0';	//������ ����


	//map�̸��� ������ ���� ������ �� �ִ� ���� ����Ʈ�� ������ �����ش�. 
	if( strlen( szMapName ) == 0 )
	{
		packet.u.quest_in_map.iMapnumber = MapNumber;	//MpaNumber�� Client�� �� �ִ� �ʹ�ȣ
		QueuePacket(&packet,1);	
	}
	//map�̸��� all�� �ָ� ��� ����Ʈ ������ �����ش�. (�̱���)
	else if( 0 ==strcmp( szMapName , "all") )
	{
		;
	}
	else
	{
		//���̸����� �ʹ�ȣ�� �޾ƿͼ� ����
		packet.u.quest_in_map.iMapnumber = GetMapNumberByMapName( szMapName );
		//���̸��� �߸� �������
		if( packet.u.quest_in_map.iMapnumber < 0 ) 
		{
			//255,255,0, lan->OutputMessage(0,219));
			//const char *tmp = g_cMsg.GetTextMessage( 1);
			AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, (char*)g_cMsg.GetTextMessage( 0,0) );// 021128
			AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, (char*)g_cMsg.GetTextMessage( 0,1) );// 021128 
			return;
		}
		
		QueuePacket(&packet,1);	
	}
	
		
}

int GetMapNumberByMapName( const char * szMapname)
{
	for( int i =0; i< MAX_MAP_; i++)
	{
		if( strcmp( MapInfo[i].mapname, szMapname) == 0 )
			return i;
	}

	return -1;
}

void RecvQuestInMap( t_quest_in_map *tp)
{
	if( !tp ) return;
	//char szMap[30];	//t_mapinfo���� ����
	//strcpy( szMap, GetMapNameByIndex( tp->iMapnumber ), strlen( GetMapNameByIndex( tp->iMapnumber ) );

	char szMsg[128];
	if( 0 < tp->iQuestInfo  && tp->iQuestInfo < MAX_MSG_NUM )
	{
		sprintf( szMsg, "<< %s >>", GetMapNameByIndex( tp->iMapnumber ) );
		AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, szMsg );	
		AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW, (char*)g_cMsg.GetTextMessage( 0, tp->iQuestInfo) ) ; // 021128
	}
}

void RecvSHOW_COLER_MSG( t_show_msg *tp  )
{
	//������ ���ų� �� ������ Ʋ���� �����Ѵ�.
	if( tp->iMsgNum == NULL ) return;
	if( 0 <= tp->R || tp->R <= 256 || 0 <= tp->G || tp->G < 256 || 0 <= tp->B || tp->B < 256 )
	{
		AddCurrentStatusMessage( tp->R,tp->G,tp->B, tp->szMsg );
	}
}

int HandleCommand3( t_packet& packet, t_connection *c)
{
	switch( packet.h.header.type)
	{

		case CMD_WHAT_QUEST_IN_MAP:			// 021009 kyo	//������ �� ����Ʈ�� �󸸳�?
		{
			RecvQuestInMap( (&packet.u.quest_in_map) );
			
		}break;
		
		case CMD_SCRIPT_SHOW_COLOR_MSG:		// 021015 KYO	// �� ������ ���ڸ� ������
		{
			RecvSHOW_COLER_MSG( (&packet.u.show_msg) );
		}break;

			
		case CMD_SCRIPT_TIMER_START:		// 021022 kyo script Ÿ�̸ӿ� ���Ѱ�
		{
			g_cQuest.InitScriptTimer( packet.u.script_timer.dwTime, 50000 );
		}break;

		case CMD_SCRIPT_TIMER_SYNC:				// 021022 kyo script Ÿ�̸ӿ� ���Ѱ�
		{	
			if( g_cQuest.m_cTimer == NULL ) return false;	// 030821 kyo
			g_cQuest.m_cTimer->RecvSCRIPT_TIMER_SYNC( (&packet.u.script_timer) );			
		}break;

		case CMD_SCRIPT_TIMER_END_SUC:			// 021022 kyo script Ÿ�̸ӿ� ���Ѱ�
		{
			g_cQuest.DeleteScriptTimer();
		}break;

		case CMD_SCRIPT_TIMER_END_FAL:			// 021022 kyo script Ÿ�̸ӿ� ���Ѱ�
		{
			g_cQuest.DeleteScriptTimer();
		}break;

		case CMD_SCRIPT_COUNTER_START:			// 021023 kyo ī����
		{
			g_cQuest.InitScriptCounter( (&packet.u.script_counter) );
		}break;

		case CMD_SCRIPT_COUNTER_SYNC:
		{
			g_cQuest.SyncScriptCounter( (&packet.u.script_counter) );
		}break;

		case CMD_SCRIPT_COUNTER_END:
		{
			g_cQuest.DeleteScriptCounter();
		}break;

		case CMD_SCRIPT_SPELL_MAPMOVE:
		{// 021128 kyo
			g_cQuest.CheckSpellWord( (&packet.u.spell_mapmove) );
		}break;

		case CMD_SCRIPT_SPELL_MAPMOVE_SUC:
		{// 021128 kyo
			g_cQuest.RecvSpellWord_Suc( packet.u.spell_mapmove.szMap );
		}break;

		case CMD_SCRIPT_SPELL_MAPMOVE_FAIL:
		{// 021128 kyo
			g_cQuest.RecvSpellWord_Fail();
		}break;


	default: {return 0;}break;
	}
	return 1;
}
void MenuSet3()	// 021020 kyo �޴� ����
{
	const int M_TIMMER_X = 600;
	const int M_TIMMER_Y = 25;
	//const int M_TIMMER_SIZE_X = 197;
	//const int M_TIMMER_SIZE_Y = 44;
	const int M_EDITER_X = M_TIMMER_X +30;
	const int M_EDITER_Y = M_TIMMER_Y +15;

	{//ScriptTimer
		const int iMnIndex	= NM_SCRIPT_TIMMER;
		const int iImg		= SCRIPT_TIMER_COUNTER_IMG;
		int iFtNum = 0;
		SMenuReg(iMnIndex, M_TIMMER_X, M_TIMMER_Y, iImg,0,10);	
		SMenuFTReg( iMnIndex,iFtNum++, FT_SCRIPT_TIMMER_COUNT, 0, 
							M_EDITER_X, M_EDITER_Y, 150, 40,
							iImg, 5, 6,0);
	}
	{//ScriptCounter
		const int iMnIndex	= NM_SCRIPT_COUNTER;
		const int iImg		= SCRIPT_TIMER_COUNTER_IMG;
		int iFtNum = 0;
		SMenuReg(iMnIndex, M_TIMMER_X, M_TIMMER_Y+50, iImg,0,10);
		SMenuFTReg( iMnIndex,iFtNum++, FT_SCRIPT_COUNTER_COUNT, 0, 
							M_EDITER_X, M_EDITER_Y+50, 150, 40,
							iImg+1, 5, 6,0);
	}

}

//<soto-030425
void	LottoMenuSet()
{	
	const int nButtonStartX	= 16;
	const int nButtonStartY	= 57;
	const int nBtnGab	= 5;
	const int nNumBtnWidth	= 36;
	const int nNumBtnHeight	= 30;

	int		nX  = nButtonStartX;
	int		nY	= nButtonStartY;
	int		nImgNum = 1;
	int		nFieldNumber = 0;

	::SMenuReg(MN_LOTTO_MENU,CENTER_X,100,LOTTO_MENU_IMG,0,30);// �ζ� ����â.

	// ���⼭ 21���� ���� ��Ű��. 0~23
	for(int j = 0; j < 3; j++)
	{
		for(int i = 0; i < 7; i++)
		{
			::SMenuFTReg(MN_LOTTO_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LOTTO_NUMBER,nX,nY,nNumBtnWidth,nNumBtnHeight, LOTTO_MENU_IMG,nImgNum,nImgNum+1);nImgNum+=2;
			nX += (nBtnGab + nNumBtnWidth);
			nFieldNumber++;
		}
		nX	= nButtonStartX;
		nY += (nBtnGab + nNumBtnHeight);
	}
	//������ 3���� ���ý�Ų��.
	for(int i = 0;i < 3;i++)
	{
		::SMenuFTReg(MN_LOTTO_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LOTTO_NUMBER,nX,nY,nNumBtnWidth,nNumBtnHeight, LOTTO_MENU_IMG,nImgNum,nImgNum+1);nImgNum+=2;
		nX += (nBtnGab + nNumBtnWidth);
		nFieldNumber++;
	}
	nX += (nBtnGab + nNumBtnWidth);
	//�ڵ�������ư.
	::SMenuFTReg(MN_LOTTO_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LOTTO_AUTO_GEN,nX,nY,118,30, LOTTO_MENU_IMG,nImgNum,nImgNum+1);nImgNum+=2;
	nFieldNumber++;
	
	//OK��ư.
	nX =	40;
	nY +=	97;
	::SMenuFTReg(MN_LOTTO_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LOTTO_OK,nX,nY,84,30, LOTTO_MENU_IMG,nImgNum,nImgNum+1);nImgNum+=2;
	nFieldNumber++;
	
	nX +=	150;
	
	//CANCEL��ư.
	::SMenuFTReg(MN_LOTTO_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALLMENU_END,nX,nY,84,30, LOTTO_MENU_IMG,nImgNum,nImgNum+1);
}

void LuckyEventSet()
{
//<soto-031010 LottoȮ�� �߰���.
	int nX = 34;
	int nY = 201;

	::SMenuReg(MN_WINNER_MENU,CENTER_X,100,LUCKY_EVENT_IMG,1,4);// Ȯ�� ����â.


	

	//Ȯ�� ��ư.
	nX = 34;nY = 277;
	::SMenuFTReg(MN_WINNER_MENU,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_WINNER_OK,nX,nY,84,30, LUCKY_EVENT_IMG,5,6);
		
	//CANCEL��ư.
	nX = 245;nY = 277;
	::SMenuFTReg(MN_WINNER_MENU,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALLMENU_END,nX,nY,84,30, LUCKY_EVENT_IMG,7,8);


	//���� ������ ��ư.
	nX = 62;nY = 237;
	::SMenuFTReg(MN_WINNER_MENU,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LOTTO_NUM_LEFTSEEK,nX,nY,26,26, LUCKY_EVENT_IMG,9,10);

	//���� ������ ��ư.
	nX = 274;nY = 237;
	::SMenuFTReg(MN_WINNER_MENU,3,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LOTTO_NUM_RIGHTSEEK,nX,nY,26,26, LUCKY_EVENT_IMG,11,12);
//>soto-031010 LottoȮ�� �߰���.
}
//>soto-030502

void	LuckyMenuSet()
{
	const	int	nMainX = CENTER_X;
	const	int nMainY = 100;
	
	const	int nBtnPosX= 30;
	int		nBtnPosY	= 53;
	const	int	nGabY	= 19;
	
	const	int nBtnWidth = 160;
	const	int	nBtnHeight= 34;

	const	int nCancelPosX	= 56;
	const	int	nCancelPosY = 212;

	const	int nCancelWidth = 110;
	const	int	nCancelHeight = 30;
		
	int		nImgNum = 1;
	int		nFieldNumber = 0;

	::SMenuReg(MN_LUCKY_MENU,nMainX,nMainY,LUCK_MENU_IMG,0,4);// ��� �޴�.
	
	::SMenuFTReg(MN_LUCKY_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LOTTO_OPEN,nBtnPosX,nBtnPosY,nBtnWidth,nBtnHeight, LUCK_MENU_IMG,nImgNum,nImgNum+1);
	nImgNum+=2;nFieldNumber++;nBtnPosY += (nGabY+nBtnHeight);

	::SMenuFTReg(MN_LUCKY_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_LUCKY_EVENT,nBtnPosX,nBtnPosY,nBtnWidth,nBtnHeight, LUCK_MENU_IMG,nImgNum,nImgNum+1);
	nImgNum+=2;nFieldNumber++;nBtnPosY += (nGabY+nBtnHeight);

	::SMenuFTReg(MN_LUCKY_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_COMFIRMATION_WINNER,nBtnPosX,nBtnPosY,nBtnWidth,nBtnHeight, LUCK_MENU_IMG,nImgNum,nImgNum+1);
	nImgNum+=2;nFieldNumber++;nBtnPosY += (nGabY+nBtnHeight);

	::SMenuFTReg(MN_LUCKY_MENU,nFieldNumber,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,SWD_SMALLMENU_END,nCancelPosX,nCancelPosY,nCancelWidth,nCancelHeight, LUCK_MENU_IMG,nImgNum,nImgNum+1);

	LottoMenuSet();

	LuckyEventSet();

}


void	LuckyMenuOpen()
{
	::CloseAllMenu();
	SMenu[MN_LUCKY_MENU].bActive=TRUE;
	MP3( SN_MENU_OPEN );
	y_MenuFocus = MN_LUCKY_MENU;
}

void	LottoMenuOpen()
{
	::CloseAllMenu();
	SMenu[MN_LOTTO_MENU].bActive=TRUE;
	MP3( SN_MENU_OPEN );
	y_MenuFocus = MN_LOTTO_MENU;
}

void	WinnerMenu()
{
	::CloseAllMenu();
	SMenu[MN_WINNER_MENU].bActive=TRUE;
	MP3( SN_MENU_OPEN );
	y_MenuFocus = MN_WINNER_MENU;
}
//>soto-030425

void MenuSubProcessType2(SMENU *SubMenu,const int j)	//021022 kyo
{
	switch(SubMenu->nField[j].nType)
	{
		case FT_SCRIPT_TIMMER_COUNT:
		{
			if(  g_cQuest.m_cTimer == NULL ) return;

			char msg[128];
			g_cQuest.RunScriptTimer( msg );
			
			Hcolor( FONT_COLOR_SOLID_YELLOW );
			Hprint2( SubMenu->nField[j].x, SubMenu->nField[j].y, g_DestBackBuf , msg);			
		}break;

		case FT_SCRIPT_COUNTER_COUNT:
		{
			if(  g_cQuest.m_cCounter == NULL ) return;

			char msg[128];
			g_cQuest.RunScriptCounter( msg );
			
			Hcolor( FONT_COLOR_SOLID_YELLOW );
			Hprint2( SubMenu->nField[j].x, SubMenu->nField[j].y, g_DestBackBuf , msg);
		}break;

		default: break;
	}
		
}