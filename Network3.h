#define		CCD_ARIGEMENT							1			//가치관 정보 요구
#define		CCD_PLAYSOUND_EVENT1					851			//행사용 이벤트용 사운드
#define		CCD_PLAYSOUND_EVENT2					853
////////////////////////////// 0803 yjs 추가 ///////////////////////////////////
#define		CCD_FIGHT_LOSE							10000		//진경우
#define		CCD_FIGHT_DRAW							10001		//비긴경우
#define		CCD_FIGHT_LOGOUT						10002		//상대가 로그아웃한 경우

///////////////////// 0810 lhs 추가 ////////////////////////
#define		CCD_START_QUEST							15000			//퀘스트 시작 효과음
#define		CCD_END_QUEST							15001
#define		CCD_QUEST_SALUTE						15002			//퀘스트 축포 효과
#define		CCD_QUEST_GETSKILL						15003
#define		CCD_QUEST_GETITEM						15004
#define		CCD_QUEST_MONSTER						15005
#define		CCD_START_QUEST_BBS						15006
#define		CCD_END_QUEST_BBS						15007
#define		CCD_COMPLETE_QUEST						15008

#define		CMD_SERVER_MAGIC_POSITION				20000
#define		CMD_SERVER_CHARACTER_CONDITION			20001
#define		CMD_CLIENT_CHARACTER_DEMAND				20002
#define		CMD_SERVER_ARIGEMENT_RESPONSE			20003
#define		CMD_SERVER_MAGICCOMMAND					20004
#define		CMD_SERVER_MAGICTRAP_EXPOSE				20005
#define		CMD_SERVER_MAGICAL_DETECT				20006
#define		CMD_SERVER_MAGICVIEWTYPE				20007
////////////////////// 0802 yjs 추가 //////////////////////
#define		CMD_SERVER_FIGHT_STONE					20010		//1:1 결투..

////////////////////// 0816 lkh 추가 //////////////////////
#define		CMD_HEXAEDITING							20011		//헥사에디팅을 시도한 경우 서버에 알린다.

#define		CMD_SERVER_CHARACTER_CONDITION2			20012

///////////////// 0810 lhs 퀘스트용 디파인  ///////////////////////////////////
#define		CMD_BBS_QUEST							25000
#define		CMD_QSF_COMMAND							25001
#define		QSF_COMMAND_TELEPORT					1
/////////////////////////////////////////////////////////////////


///////////////////// 0802 yjs 추가 ////////////////////
typedef struct fight_stone
{
	BOOL		fight_flag;	//상태 플래그..
	int			fight_id;
	short int	x;			//x좌표.. 
	short int	y;			//y좌표.. 
}	t_fight_stone;


typedef struct  client_character_demand
{
	short int	command_type;
	short int	target_id;
}   t_client_character_demand;

typedef struct	server_arigement_response
{
	short int	target_id;
	short int	arigement;
}	t_server_arigement_response;

struct t_server_magic_position
{
  BYTE nType; // 출력할 이펙트 번호
  WORD idTarget; // 마법을 사용한 대상자
  __int16 nX; // 이동할 위치의 X좌표
  __int16 nY; // 이동할 위치의 Y좌표
};

struct t_server_character_condition
{
  WORD idTarget;
  __int32 nLife;
  BYTE nState;
};

typedef struct  server_magic_command
{
	//short int	target_id;		//대상 ID
	short int	command;				//대상 클라이언트에 실제 효력을 미치는 대상관련 명령(slot1)
	short int	sub_command;		//                   "									서브 명령(slot2)
	short int	sub_term;				//                   "   효력을 미치는 기간				(slot3)
}	t_server_magic_command;

typedef struct  server_magictrap_expose
{
	short int	target_id;
	short int	x;
	short int	y;
	short int	trap_Type;
}	t_server_magictrap_expose;

typedef struct	server_magical_detect
{
	short int	tile_X;
	short int	tile_Y;
	short int	type;
}	t_server_magical_detect;

typedef struct	server_magicviewtype
{
	short int	target_id;
	short int	view_Type;
	short int	second_Time;
}	t_server_magicviewtype;

//////////// 0810 lhs //////////////////
typedef struct	qsf_command				//퀘스트에서 범용으로 사용하기 위한 구조체
{
	short int	command;
	short int	target_id;
	short int	x;
	short int	y;
}	t_qsf_command;


/////////// 0816 lkh 추가 ///////////
typedef struct client_editing_table
{
	short int	type;					//체크섬 에러가 난 화일의 무엇인지!
	short int	id;						//어떤 놈이 헥사에디팅을 시도했는지!
}	t_client_editing_table;

typedef union	angra_pub
{
	t_server_character_condition		server_character_condition;
	t_server_magic_position				server_magic_position;
	t_client_character_demand			client_character_demand;
	t_server_arigement_response			server_arigement_response;
	t_server_magic_command				server_magic_command;
	t_server_magictrap_expose			server_magictrap_expose;
	t_server_magical_detect				server_magical_detect;
	t_server_magicviewtype				server_magicviewtype;
	////////////////////// 0802 yjs 추가 /////////////////////////////
	t_fight_stone						server_fight_stone;	//
	///////////////////// 퀘스트 용 컴맨드... 0810 lhs //////////////////
	t_qsf_command						qsf_command;

	////////////////////// Hexa Edit 시도한 경우 서버로 알리기 위해 ////////////////////
	t_client_editing_table				client_editing_table;

}	t_angra;




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	                                      KBS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CMD_RM_CHANGE_WEATHER		26000


// 011212 KBS
typedef struct rm_change_weather
{
	BYTE	bWeather;		//0이면 정상, 1이면 비, 2이면 눈 
	BYTE	bStopWeather;
	DWORD	dwAmount;

}t_rm_change_weather;
//


extern void RecvChangeWeather( t_rm_change_weather* p );