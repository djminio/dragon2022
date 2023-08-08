// 020927
#ifndef _NETWORK7_H_
#define _NETWORK7_H_

enum KYO_CODE
{
	MAX_MSG_NUM					=	500,   // kyo

	//LC_WHAT_QUEST_IN_MAP		=   123,	// 021007 //맵별 퀘스트정보 021022 이동 시켜따.
	CMD_RM_NOW_REPAIR			=	14000,	//rmtool로 복구중인가?
	CMD_WHAT_QUEST_IN_MAP		=	14001,	//isLinecommand에서 "/퀘스트 맵이름" 할때
	CMD_SCRIPT_SHOW_COLOR_MSG	=	14002,	
	
	//타이머용
	CMD_SCRIPT_TIMER_START		=	14110,
	CMD_SCRIPT_TIMER_SYNC		=	14111,
	CMD_SCRIPT_TIMER_END_SUC	=	14112,
	CMD_SCRIPT_TIMER_END_FAL	=	14113,

	CMD_SCRIPT_COUNTER_START	=	14120,
	CMD_SCRIPT_COUNTER_SYNC		=	14121,
	CMD_SCRIPT_COUNTER_END		=	14122,

	CMD_SCRIPT_SPELL_MAPMOVE	=	14200,	// 021128 kyo
	CMD_SCRIPT_SPELL_MAPMOVE_SUC=	14201,	// 021128 kyo
	CMD_SCRIPT_SPELL_MAPMOVE_FAIL=	14202,	// 021128 kyo

	DEFAULT_TIMER_SYNC_TIME		=	5000,	//5초
	MAX_TIMER_TIME				= 5 * 3600 * 1000,		//5시간이 제한이다.
	MIN_SYNCH_TIME				= 2 * 1000,				//2초이하다.
};


//packet type
typedef struct DATA_INTEGER
{
	int	iData;
}t_int;

typedef struct DATA_CHAR
{
	char cData;
}t_char;

typedef struct DATA_DATA
{
	char szData[30];
}t_data;

typedef struct QUEST_IN_MAP
{
	char	name[21];
	int		iMapnumber;
	int		iQuestInfo;

}t_quest_in_map;

typedef struct SHOW_COLOR_MSG
{
	int		R;
	int		G;
	int		B;
	char	szMsg[0xff];
	int		iMsgNum;
}t_show_msg;

typedef struct SCRIPT_TIMER
{
	DWORD	dwTime;
}t_script_timer;

typedef struct SCRIPT_COUNTER	//여러군대 많이쓰기위해서
{
	int iNum;
	int	iType;
	int iMuch;
	int iCounter;
}t_script_counter;

typedef struct SCRIPT_SPELLMAPMOVE	// // 021128 kyo
{
	int cn;
	int	iX;
	int iY;
	char szMap[20];
	char szSpell[128];
	int isSuc;
}t_script_spellmapmove;

//extern 
extern void SendWhatQuestInMap( char * szMapName);		//해당 맵에서 어떤 퀘스트를 수행하고 있으며 어떤내용을 수행중인가?
extern int GetMapNumberByMapName( const char *szMapName);	//맵이름을 받아서 맵번호를 반환, 에러시 -1 반환
extern void RecvQuestInMap( t_quest_in_map *tp );			
extern void RecvSHOW_COLER_MSG( t_show_msg *tp );		//해상 색으로 매시지를 출력한다. ( AddCurrentStatusMessage사용 )

#endif //_NETWORK7_H_