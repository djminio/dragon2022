// 020927
#ifndef _NETWORK7_H_
#define _NETWORK7_H_

enum KYO_CODE
{
	MAX_MSG_NUM					=	500,   // kyo

	//LC_WHAT_QUEST_IN_MAP		=   123,	// 021007 //�ʺ� ����Ʈ���� 021022 �̵� ���ѵ�.
	CMD_RM_NOW_REPAIR			=	14000,	//rmtool�� �������ΰ�?
	CMD_WHAT_QUEST_IN_MAP		=	14001,	//isLinecommand���� "/����Ʈ ���̸�" �Ҷ�
	CMD_SCRIPT_SHOW_COLOR_MSG	=	14002,	
	
	//Ÿ�̸ӿ�
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

	DEFAULT_TIMER_SYNC_TIME		=	5000,	//5��
	MAX_TIMER_TIME				= 5 * 3600 * 1000,		//5�ð��� �����̴�.
	MIN_SYNCH_TIME				= 2 * 1000,				//2�����ϴ�.
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

typedef struct SCRIPT_COUNTER	//�������� ���̾������ؼ�
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
extern void SendWhatQuestInMap( char * szMapName);		//�ش� �ʿ��� � ����Ʈ�� �����ϰ� ������ ������� �������ΰ�?
extern int GetMapNumberByMapName( const char *szMapName);	//���̸��� �޾Ƽ� �ʹ�ȣ�� ��ȯ, ������ -1 ��ȯ
extern void RecvQuestInMap( t_quest_in_map *tp );			
extern void RecvSHOW_COLER_MSG( t_show_msg *tp );		//�ػ� ������ �Ž����� ����Ѵ�. ( AddCurrentStatusMessage��� )

#endif //_NETWORK7_H_