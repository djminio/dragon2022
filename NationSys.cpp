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
#include "hong_sub.h"
#include "NATIONSUPPORT.H"//010910 LSW
#include <io.h>

extern void RecvCMD_SQUAD_CHAT(t_packet* p);
// Gloabl Variable
cNation*	g_pNation=NULL;
CDontManager* g_pDontManager=NULL;
int g_Squad=1;						// 나중에 클래스에 포함한다.	//부대인터페이스 오픈 하기위한 데이터
int g_Commander=0;					// 사령관 인터테이스 오픈하기위한 데이터

int g_AttackNation=0;				// 공격국 국가번호	// 전쟁터에 들어갔을때 세트된다
int g_DefenceNation=0;				// 방어국 국가번호 

int g_ObjectWarfieldNo=0;			// 2-4기간(투표->전쟁) 까지의 대상 전쟁터 번호
int g_ObjectWarfieldStatus=0;		// 해당 전쟁터의 상태 

int g_CommanderCandidaterLevel=0;   //나중에 바꾼다..
bool g_bWarBegin=false;				// RecvCMD_WAR_START,END에서 사용 
bool g_bNewWarBegin=false;

int g_SquadLoadingPoint;			//로딩포인트 번호	// 부재메뉴에서 사용하는 임시 저장용 데이타
int g_SquadNum;						//부대메뉴			// 마찬가지	

//< LTH-040314-KO 몬스터 전쟁중 우상단 인터페이스에 쓰일 내용
DWORD g_dwRemainTime = 0;				// 전쟁 남은 시간
int g_aRemainGuard[2][4] = {0,};		// 전쟁중 남은 결계석 및 성문. 두개의 팀. 0:최종수호석 1:내성수호석 2:성문 3:외성수호석
//> LTH-040314-KO

extern int g_iScenarioOn;	// BBD 040315

extern SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];//011004 lsw

// Tile Concern__________________________________________________________________________________	// 010906 LTS
const POINTS		Map0_133[MAP0_133]={		// 010927 LTS
									{131,259},{131,260},{131,261},{131,262},{131,263},{131,264},{131,265},
									{131,266},{132,258},{132,259},{132,260},{132,261},{132,262},{132,263},
									{132,264},{132,265},{133,257},{133,258},{133,259},{133,260},{133,261},
									{133,262},{133,263},{133,264},{134,256},{134,257},{134,258},{134,259},
									{134,260},{134,261},{134,262},{134,263}		};

const POINTS		Map0_138[MAP0_138]={
									{135,255},{135,256},{135,257},{135,258},{135,259},{135,260},{135,261},
									{135,262},{136,255},{136,256},{136,257},{136,258},{136,259},{136,260},
									{136,261},{137,256},{137,257},{137,258},{137,259},{137,260},{138,257},
									{138,258},{138,259},{139,258}	};

const POINTS		Map0_203[MAP0_203] ={	
									{200,192},{200,193},{200,194},{200,195},{200,196},{200,197},{200,198},
									{200,199},{201,192},{201,193},{201,194},{201,195},{201,196},{201,197},
									{201,198},{202,193},{202,194},{202,195},{202,196},{202,197},{203,194},
									{203,195},{203,196},{204,195} 	};

const POINTS		Map0_198[MAP0_198] = {	
									{199,193},{199,194},{199,195},{199,196},{199,197},{199,198},{199,199},
									{199,200},{198,194},{198,195},{198,196},{198,197},{198,198},{198,199},
									{198,200},{198,201},{197,195},{197,196},{197,197},{197,198},{197,199},
									{197,200},{197,201},{197,202},{196,196},{196,197},{196,198},{196,199},
									{196,200},{196,201},{196,202},{196,203},{195,196},{195,197}				};

const POINTS		Map0_263[MAP0_263] ={	
									{261,133},{261,134},{261,135},{261,136},{261,137},{261,138},{261,139},
									{261,140},{262,132},{262,133},{262,134},{262,135},{262,136},{262,137},
									{262,138},{262,139},{263,131},{263,132},{263,133},{263,134},{263,135},
									{263,136},{263,137},{263,138},{264,130},{264,131},{264,132},{264,133},
									{264,134},{264,135},{264,136},{264,137}									};

const POINTS		Map0_268[MAP0_268] ={	// 010927 LTS
									{265,129},{265,130},{265,131},{265,132},{265,133},{265,134},{265,135},
									{265,136},{266,129},{266,130},{266,131},{266,132},{266,133},{266,134},
									{266,135},{267,130},{267,131},{267,132},{267,133},{267,134},{268,131},
									{268,132},{268,133}														};
//________________________________________________________________________________________________	// 010906 LTS
//________________________________________________________________________________________________	// 010915 LTS

const POINTS		Map1_450[MAP1_450]=
								{	{451,521},{451,522},{451,523},{451,524},{451,525},{451,526},
									{450,522},{450,523},{450,524},{450,525},{450,526},{450,527},
									{449,523},{449,524},{449,525},{449,526},{449,527},{449,528},
									{448,524},{448,525},{448,526},{448,527},{448,528},{448,529},
									{447,525},{447,526},{447,527},{447,528},{447,529},{446,526},
									{446,527},{446,527}											};

const POINTS		Map1_527[MAP1_527]=		// 010927 LTS
								{	{528,525},{528,526},{528,527},{528,528},{528,529},{528,530},
									{527,526},{527,527},{527,528},{527,529},{527,530},{527,531},
									{526,527},{526,528},{526,529},{526,530},{526,531},{526,532},
									{525,528},{525,529},{525,530},{525,531},{525,532},{525,533},
									{524,529},{524,530},{524,531},{524,532},{524,533},{523,530},
									{523,531},{523,532}											};

const POINTS		Map1_524[MAP1_524]=
								{	{525,451},{525,452},{525,453},{525,454},{525,455},{525,456},
									{524,452},{524,453},{524,454},{524,455},{524,456},{524,457},
									{523,453},{523,454},{523,455},{523,456},{523,457},{523,458},
									{522,454},{522,455},{522,456},{522,457},{522,458},{522,459},
									{521,455},{521,456},{521,457},{521,458},{521,459},{520,456},
									{520,457},{520,458}											};

const POINTS		Map1_E450[15]=
								{	{451,523},{451,524},{451,525},{450,524},{450,525},{450,526},
								    {449,525},{449,526},{449,527},{448,526},{448,527},{448,528},
									{447,527},{447,528},{447,529}								};
const POINTS		Map1_E527[15]=
								{	{528,527},{528,528},{528,529},{527,528},{527,529},{527,530},
									{526,529},{526,530},{526,531},{525,530},{525,531},{525,532},
									{524,531},{524,532},{524,533}								};
const POINTS		Map1_E524[15]=
								{   {525,453},{525,454},{525,455},{524,454},{524,455},{524,456},
									{523,455},{523,456},{523,457},{522,456},{522,457},{522,458},
									{521,457},{521,458},{521,459}								};
//________________________________________________________________________________________________	// 011015 LTS
const POINTS		Map2_114[MAP2_114]=
								{
									{112,165},{113,164},{114,163},{112,164},{113,163},{114,162},
									{112,163},{113,162},{114,161},{112,162},{113,161},{114,160},
									{112,161},{113,160},{114,159},{111,161},{112,160},{113,159},
									{114,158},{111,160},{112,159},{113,158},{114,157},{110,160},
									{111,159},{112,158},{113,157},{114,156}						};

const POINTS		Map2_119[MAP2_119]=
								{
									{115,162},{116,161},{117,160},{118,159},{119,158},{115,161},
									{116,160},{117,159},{118,158},{115,160},{116,159},{117,158},
									{118,157},{115,159},{116,158},{117,157},{118,156},{115,158},
									{116,157},{117,156},{118,155},{115,157},{116,156},{117,155},
									{118,154},{115,156},{116,155},{117,154},{118,153},{115,155},
									{116,154},{117,153} };


const POINTS		Map2_151[MAP2_151]=
								{
									{149,130},{150,129},{151,128},{152,127},{149,129},{150,128},
									{151,127},{152,126},{149,128},{150,127},{151,126},{152,125},
									{149,127},{150,126},{151,125},{152,124},{149,126},{150,125},
									{151,124},{152,123},{149,125},{150,124},{151,123},{152,122},
									{149,124},{150,123},{151,122},{152,121},{148,124},{149,123},
									{150,122},{151,121},{152,120}	};

const POINTS		Map2_156[MAP2_156]=
								{
									{153,126},{154,125},{155,124},{156,123},{153,125},{154,124},
									{155,123},{156,122},{153,124},{154,123},{155,122},{153,123},
									{154,122},{155,121},{153,122},{154,121},{155,120},{153,121},
									{154,120},{155,119},{153,120},{154,119},{155,118},{153,119},
									{154,118},{155,117}	};


const POINTS		Map2_274[MAP2_274]=
								{
									{272,286},{273,285},{274,284},{275,283},{272,285},{273,284},
									{274,283},{275,282},{272,284},{273,283},{274,282},{275,281},
									{272,283},{273,282},{274,281},{275,280},{272,282},{273,281},
									{274,280},{275,279},{271,282},{272,281},{273,280},{274,279},
									{275,278},{271,281},{272,280},{273,279},{274,278},{275,277},
									{270,281},{271,280},{272,279},{273,278},{274,277},{275,276}    };

const POINTS		Map2_279[MAP2_279]=
								{
									{276,282},{277,281},{278,280},{279,279},{276,281},{277,280},
									{278,279},{279,278},{276,280},{277,279},{278,278},{276,279},
									{277,278},{278,277},{276,278},{277,277},{276,277},{277,276},
									{276,276},{276,275}		};



const POINTS		Map2_E114[8]=
								{	{112,165},{113,164},{114,163},{115,162},{112,164},{113,163},
									{114,162},{115,161}											};

const POINTS		Map2_E119[8]=
								{	{116,161},{117,160},{118,159},{119,158},{116,160},{117,159},
									{118,158},{119,157}											};
const POINTS		Map2_E151[8]=
								{	{149,129},{150,128},{151,127},{152,126},{149,128},{150,127},
									{151,126},{152,125}											};

const POINTS		Map2_E156[9]=
								{	{153,125},{154,124},{155,123},{156,122},{157,121},{153,124},
									{154,123},{155,122},{156,121}								};

const POINTS		Map2_E274[8]=
								{	{272,285},{273,284},{274,283},{275,282},{272,284},{273,283},
									{274,282},{275,281}											};

const POINTS		Map2_E279[8]={	{276,281},{277,280},{278,279},{279,278},{276,280},{277,279},
									{278,278},{279,277}											};
//________________________________________________________________________________________________	// 011015 LTS



// Extern Variable Definition
extern SMENU SMenu[MAX_MENU];    
extern DEVELOPE_ENVIR	SysInfo;


// Extern Function Definition
extern void			ShowComeOutGM(char *name);
extern void			AddCurrentStatusMessage( BYTE r, BYTE g, BYTE b, char *msg, ... ); // CSD-030324
extern void			LoadHeroClothAccessoryDataOne( int sprno, int no );
extern void			FreeHeroClothAccessoryDataOne( int sprno, int no );
extern void			InsertMagic(LPCHARACTER lpChar_Own, LPCHARACTER lpChar_Target, int magicNum, int magicSubNum, int s_X, int s_Y, int t_X, int t_Y, bool firePattern=0, int next_Effect=0, bool failed=0, DWORD dwDuration = 0);
extern LPCHARACTER	ExistHe( char *name );		// 내 시야에 존재하는 넘인지 알아보는 루틴
extern int			convert565to555(Spr *sp);
extern int			convert565to555_LoadSprite(Spr *sp);
extern void			PutCompressedImageFX(int x, int y, Spr *sp, DWORD alpha, DWORD op);
extern void			PutCompressedImage(int x, int y, Spr *sp );
extern int			CheckHandleByLTSSupport(t_packet* p);		// 011201 LTS	// LTS MENU
extern void			SendCMD_LOCALWAR_STATUS();					// LTS LOCALWAR
extern int			LoadLocalWarInfoPic(char* FileName);			// LTS LOCALWAR
extern int			LoadChairPic(int ChairNum);			// LTS SITDOWN BUG
extern void			InitSitDownData();
extern void 		FreeLocalWarInfoPicBuffer();			// LTS LOCALWAR
extern void 		FreeChairPicBuffer();			// LTS SITDOWN
extern int			GetNationIndex(int Nation);		// LTS HORSERIDER





char* SquadPicBuffer=NULL;
Spr	  SquadPic[3][5];


void WarControl(char* Arg)			// 011025 LTS
{
#ifdef _DEBUG // finito 060507
	t_packet packet;				// Ex) /W 0 0   (View)	// 산중전쟁터의 상태를 본다 
	char Active;					//	   /W 1 0 1			// 산중전쟁터를 Active로 세트한다.
									//     /W 1 0 0			// 산중전쟁터를 DeActive로 세트한다.
	char Type=Arg[0]-48;
	char WarfieldNo=Arg[2]-48;	

	if (Type) Active=Arg[4]-48;	// Set		// 공란 다음 

	if( GetSysInfo( SI_GAME_MAKE_MODE ) )	// 게임제작 모드 일때만 쓴다..
	{
		if (!Type)				// View
		{
			packet.h.header.type=CMD_WAR_CONTROL;
			packet.u.NationWar.WarControl.Type=Type;
			packet.u.NationWar.WarControl.WarfieldNo=WarfieldNo;
			packet.u.NationWar.WarControl.Status=0;					// 보낼때는 필요없는 정보다
			packet.u.NationWar.WarControl.Active=0;					// View에서는 필요없는 정보이다.
			packet.h.header.size=sizeof(t_WarControl);
			AddCurrentStatusMessage( 255,255,0,"Request : Request Current Warfield Loop Status...");
		}
		else					// Set
		{
			packet.h.header.type=CMD_WAR_CONTROL;
			packet.u.NationWar.WarControl.Type=Type;
			packet.u.NationWar.WarControl.WarfieldNo=WarfieldNo;
			packet.u.NationWar.WarControl.Status=0;					// 보낼때는 필요없는 정보다
			packet.u.NationWar.WarControl.Active=Active;
			packet.h.header.size=sizeof(t_WarControl);
			AddCurrentStatusMessage( 255,255,0,"Request :Setting current warfield loop status to [%d]",Active);
		}
		QueuePacket(&packet,1);
	}
#endif
}

void WarLoopTime(char* Arg)			// 011025 LTS
{
#ifdef _DEBUG // finito 060507
	t_packet packet;
	DWORD LoopTime;

	char Type=Arg[0]-48;
	char WarfieldNo=Arg[2]-48;	// 공란다음 

	if (Type) LoopTime=atoi(&Arg[4]);	// Set	// 루프타임을 입력받는다.

	if (GetSysInfo( SI_GAME_MAKE_MODE ))
	{
		if (!Type)			// View
		{
			packet.h.header.type=CMD_WAR_LOOP_TIME;
			packet.u.NationWar.WarLoopTime.Type=Type;
			packet.u.NationWar.WarLoopTime.WarfieldNo=WarfieldNo;
			packet.u.NationWar.WarLoopTime.Status=0;		//보낼때는 필요없는 정보이다.
			packet.u.NationWar.WarLoopTime.LoopTime=0;		//보낼때는 필요없는 정보이다.
			packet.h.header.size=sizeof(t_WarLoopTime);
			AddCurrentStatusMessage( 255,255,0,"Requesting current warfield loop time...");
		}
		else				// Set
		{
			packet.h.header.type=CMD_WAR_LOOP_TIME;
			packet.u.NationWar.WarLoopTime.Type=Type;
			packet.u.NationWar.WarLoopTime.WarfieldNo=WarfieldNo;
			packet.u.NationWar.WarLoopTime.Status=0;		//보낼때는 필요없는 정보이다.
			packet.u.NationWar.WarLoopTime.LoopTime=LoopTime;
			packet.h.header.size=sizeof(t_WarLoopTime);
			AddCurrentStatusMessage( 255,255,0,"Requesting current warfield loop time to alter to [%d]",LoopTime);
		}
		QueuePacket(&packet,1);
	}
#endif
}

void SendCMD_CHECK_AM_I_DEFEAT()			//임시함수 // 011015 LTS
{
	t_packet packet;

	packet.h.header.type=CMD_CHECK_AM_I_DEFEAT;
	packet.h.header.size=0;
	QueuePacket(&packet,1);

	if (Hero->name_status.nation==NW_YL) memset(&Hero->NWCharacter,0,sizeof(DWORD));	//일스이면 초기화 시킨다	// 011020 LTS
}

void SendCMD_CHECK_DEFEAT()
{
	t_packet packet;
	packet.h.header.type=CMD_CHECK_DEFEAT;
	packet.h.header.size=0;
	QueuePacket(&packet,1);

	memset(&Hero->NWCharacter,0,sizeof(DWORD));
}

void InitSquadPic()
{
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<4;j++)
		{
			SquadPic[i][j].img=NULL; 
		}
	}
}

int LoadSquadPic(char* FileName)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr  *s;
	int length;
			
	fp = Fopen(FileName,"rb" );
	if( fp == NULL ) return 0;
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR );  // 오프셋 스킵
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;
			
	if( SquadPicBuffer != NULL )  return 0;
	MemFree( SquadPicBuffer );		// 0201 YGI
	MemAlloc( SquadPicBuffer, sprfilelength );
	for( i = 0 ; i < 3 ; i ++)
	{			
		for( j = 0 ; j < 5 ; j ++)
		{				
			s = &SquadPic[i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
							
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			//s->yl ++;		
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),sizeof( unsigned int ), 1, fp);
							
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( lan->OutputMessage(6,51),FileName, i+1, j );
				ExitApplication(EA_LOAD_LOACL_SQUAD_SPR_DATA);
			}						
			fread( SquadPicBuffer + nOff, s->size, 1, fp);
			s->img = SquadPicBuffer + nOff;
							
			convert565to555_LoadSprite( s );
							
			nOff += size;		
		}		
	}			
	fclose(fp);	
	return sprfilelength;	
}

void DeleteSquadPic()
{
	MemFree( SquadPicBuffer );
}

// Global Function
bool InitNation()					
{
	if (!LoadSquadPic("./char/WarSquad.spr")) 
	{
		JustMsg( "./char/WarSquad.spr Error!!");
		return false;
	}


	if (!LoadLocalWarInfoPic("./data/LWInfo.dat")) 
	{
		JustMsg( "./data/LWInfo.dat Error!!");
		return false;	// LTS LOCALWAR
	}
	InitSitDownData();			// LTS SITDOWN BUG
	/*if (!LoadChairPic("./char/092000.spr")) 
	{
		JustMsg( "./char/092.spr 파일에 문제가 있습니다.");
		return false;				// LTS SITDOWN
	}*/
	g_pNation=new cNation;
	if (g_pNation==NULL) return false;

	g_pDontManager=new CDontManager;
	if (g_pDontManager==NULL)
		return false;
	else
		g_pDontManager->LoadData();
	return true;
}

void CloseNation()
{
	DeleteSquadPic();
	FreeLocalWarInfoPicBuffer();				// LTS LOCALWAR
	FreeChairPicBuffer();						// LTS SITDOWN
	SAFE_DELETE(g_pNation);
	SAFE_DELETE(g_pDontManager);
}

// Very often Call Function
int CheckNationStatus(int StatusNo)
{
	int WarfieldNo=-1;
	if (g_pNation->GetStatus(0)==StatusNo) WarfieldNo=0;
	if (g_pNation->GetStatus(1)==StatusNo) WarfieldNo=1;
	if (g_pNation->GetStatus(2)==StatusNo) WarfieldNo=2;
	if (g_pNation->GetNewStatus(0)==StatusNo) WarfieldNo=3;
	//< LTH-040226-KO 설원전쟁터 이후 전쟁터 상태 반환. 1.4 포함
	if (g_pNation->GetNewStatus(WI_HADES_WARFIELD) == StatusNo) WarfieldNo = WI_HADES_WARFIELD;
	//> LTH-040226-KO
    return WarfieldNo;
}

/**
 * @fn		CheckWarfieldStatus().
 * @brief	각 전쟁터의 상태를 모두 조사하여 반환.
 * @param	int nStatusNo. 조사 할 상태 값.
 * @param	BYTE aWarfieldStatus[WI_MAX_WARFIELD]. 각 전쟁터의 상황 반환 장소.
 * @return	void.
 */
//< LTH-040414-KO.
void CheckWarfieldStatus(int nStatusNo, BYTE aWarfieldStatus[WI_MAX_WARFIELD])
{
	// 초기화
	eNEO_WARFIELD_ID eNeoWarfieldId = (eNEO_WARFIELD_ID)0;
	for (; eNeoWarfieldId < WI_MAX_WARFIELD; eNeoWarfieldId = (eNEO_WARFIELD_ID)(eNeoWarfieldId + 1))
		aWarfieldStatus[eNeoWarfieldId] = (BYTE)FALSE;

	if (g_pNation->GetStatus(WI_MOUNTAIN_WARFIELD) == nStatusNo) 
		aWarfieldStatus[WI_MOUNTAIN_WARFIELD] = (BYTE)TRUE;
	if (g_pNation->GetStatus(WI_PLAIN_WARFIELD) == nStatusNo) 
		aWarfieldStatus[WI_PLAIN_WARFIELD] = (BYTE)TRUE;
	if (g_pNation->GetStatus(WI_ISLAND_WARFIELD) == nStatusNo)
		aWarfieldStatus[WI_ISLAND_WARFIELD] = (BYTE)TRUE;
	if (g_pNation->GetNewStatus(WI_SNOWY_WARFIELD - 3) == nStatusNo)
		aWarfieldStatus[WI_SNOWY_WARFIELD] = (BYTE)TRUE;
	if (g_pNation->GetNewStatus(WI_HADES_WARFIELD) == nStatusNo)
		aWarfieldStatus[WI_HADES_WARFIELD] = (BYTE)TRUE;
}
//> LTH-040414-KO

bool isNationWarfieldServer()
{
	if (MapInfo[MapNumber].port==WARFIELD_PORT1 ||
		MapInfo[MapNumber].port==WARFIELD_PORT2 ||
		MapInfo[MapNumber].port==WARFIELD_PORT3	) return true;
	return false;
}

bool isNewWarfieldServer()
{
	int Port=MapInfo[MapNumber].port;
	if (Port<BASE_NEW_WARFIELD_PORT||Port>=(BASE_NEW_WARFIELD_PORT+MAX_NEW_WARFIELD))
		return false;
	return true;
}

//< LTH-040226-KO 1.4 설원 전쟁터 이후의 신규전쟁터인지 비교하는 함수
BOOL IsNeoWarfieldServer()
{
	INT nPort = MapInfo[MapNumber].port;
	if ((nPort < WP_HADES_WARFIELD) || (nPort >= WP_MAX_WARFIELD_PORT))
		return FALSE;
	return TRUE;
}	//> LTH-040226-KO

// LTH-040207-KO  1.4 패치 이후 새로운 전쟁터 번호인가?
BOOL IsNeoWarfield(INT nWarfieldNo)
{
	if ((WI_HADES_WARFIELD > nWarfieldNo) || (WI_MAX_WARFIELD <= nWarfieldNo))
		return FALSE;
	return TRUE;
}
//> LTH-040226-KO

bool isOurSquad(LPCHARACTER hero,LPCHARACTER ch)			// 011028 LTS
{
	if (!isNationWarfieldServer()&&!isNewWarfieldServer()) return true;

	if (hero->name_status.nation!=ch->name_status.nation)	//두유저가 다른 나라이면 사람이면 
	{
		if (Hero->name_status.nation==NW_YL)	// 011019 LTS	// 주인공이 일스이면
		{
			if (ch->name_status.nation==NW_YL)	//대상도 일스이면 
			{
				if (hero->NWCharacter.YL_JoinNation==ch->NWCharacter.YL_JoinNation) return true; // 같은 나라에 조인했다면 우리부대
				else return false;	// 다른나라에 조인했다면 우리부대 아님
			}
			else	//주인공은 일스 대상은 일스가 아니다
			{
				if (hero->NWCharacter.YL_JoinNation==ch->name_status.nation) return true;   // Join한 국가가 같으면 같은 편
				else return false; // 조인한 국가가 다르면 
			}
		}
		else	// Hero가 일스가 이니다 
		{
			if (ch->name_status.nation==NW_YL)	//대상이 일스이면 
			{
				if (Hero->name_status.nation==ch->NWCharacter.YL_JoinNation) return true;
				else return false;
			}
			else return false;	// 주인공도 대상도 일스가 아니다. 
		}
	}
	else 
	{
		if (Hero->name_status.nation==NW_YL)	// 011019 LTS	// 주인공이 일스이면
		{
			if (ch->name_status.nation==NW_YL)	//대상도 일스이면 
			{
				if (hero->NWCharacter.YL_JoinNation==ch->NWCharacter.YL_JoinNation) return true; // 같은 나라에 조인했다면 우리부대
				else return false;	// 다른나라에 조인했다면 우리부대 아님
			}
			else	//주인공은 일스 대상은 일스가 아니다
			{
				if (hero->NWCharacter.YL_JoinNation==ch->name_status.nation) return true;   // Join한 국가가 같으면 같은 편
				else return false; // 조인한 국가가 다르면 
			}
		}
		else return true;	// Hero가 일스가 이니다 
	}
}


bool isAttacker()			// 전쟁터 맵에서만 가능하다.
{
	if (Hero->name_status.nation==NW_YL)
	{
		if (Hero->NWCharacter.YL_JoinNation==g_DefenceNation) return false;
		else return true;
	}
	if (Hero->name_status.nation==g_DefenceNation) return false;
	else return true;
}

bool isAttacker(LPCHARACTER ch)				// LTS DRAGON BUG
{
	if (ch->name_status.nation==NW_YL)
	{
		if (ch->NWCharacter.YL_JoinNation==g_DefenceNation) return false;
		else return true;
	}
	if (ch->name_status.nation==g_DefenceNation) return false;
	else return true;
}

bool isEnemy(LPCHARACTER ch1,LPCHARACTER ch2)	// LTS DRAGON BUG2 reece
{
	if (isNewWarfieldServer())
	{
		int Nation1=ch1->name_status.nation;
		int Nation2=ch2->name_status.nation;
		if (Nation1==NW_YL)
			Nation1=ch1->NWCharacter.YL_JoinNation;
		if (Nation2==NW_YL)
			Nation2=ch2->NWCharacter.YL_JoinNation;

		if (Nation1==Nation2)
			return false;
		else
			return true;
	}
	else
	{
		if (isAttacker(ch1)==isAttacker(ch2)) return false;
		return true;
	}
}


int GetSquadNo()
{
	return Hero->NWCharacter.SquadNo;
}


void DrawSquadPic(int x,int y,LPORDERTABLE o,LPCHARACTER ch)	// 011019 LTS	// LTS HORSERIDER
{
	//if (!isNationWarfieldServer()) return;
	//if (!g_bWarBegin) return;
	if (ch->viewtype!=VIEWTYPE_TRANSPARENCY_&&ch->viewtype!=VIEWTYPE_GHOST_)	// 투명이거나 죽어있는놈은 제외 
	{
		int NationIndex=GetNationIndex(ch->name_status.nation);

		if (isNationWarfieldServer())
		{
			if (ch->NWCharacter.isCommander) PutCompressedImage(x,y,&SquadPic[NationIndex][4]);		// 사령관이면..
			else if (ch->NWCharacter.SquadNo) PutCompressedImage(x,y,&SquadPic[NationIndex][ch->NWCharacter.SquadNo-1]);	// 부대이면..
			else
			{
				if (ch->name_status.nation==NW_YL)
				{
					if (ch->NWCharacter.YL_JoinNation==NW_BY)		// 바이용병이면....
					{
						PutCompressedImage(x,y,&SquadPic[0][0]);
					}
					if (ch->NWCharacter.YL_JoinNation==NW_ZY)	
					{
						PutCompressedImage(x,y,&SquadPic[1][0]);	// 자이 용병이면..
					}
				}
			}
		}
		if (isNewWarfieldServer())
		{
			if (g_bWarBegin)
			{
				int Nation=ch->name_status.nation;
				if (Nation==NW_YL) Nation=ch->NWCharacter.YL_JoinNation;
				switch(Nation)
				{
				case NW_BY : PutCompressedImage(x,y,&SquadPic[0][0]); break;
				case NW_ZY : PutCompressedImage(x,y,&SquadPic[1][0]); break;
				}
			}
		}
	}
}

void WarMenuOpen()	// 인터페이스 전쟁관리소 메뉴 오픈 
{
	MP3(50);
	CallSmallMenu( MN_NATION_BATTLE_BBS );
}

void GetTime(DWORD RemainTime,WORD& Hour,WORD& Min,WORD& Sec)		// 011020 LTS
{
	DWORD temp;
	Hour=WORD(RemainTime / 3600);
	temp=DWORD(RemainTime % 3600);
	Min=WORD(temp / 60);
	Sec=WORD(temp % 60);
}

void ShowCommanderInfo()
{
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,151), FALSE); //"============= 사령관 메시지 ============="
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,152), FALSE); //"/*1 내용 : 사령관이 부대장에게 메시지를 보낼 때"
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,153), FALSE); //"/*2 내용 : 사령관이 부대 전체에게 메시지를 보낼 때"
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,154), FALSE); //"/*3 내용 : 사령관이 전쟁터 안에 있는 자국민에게 메시지를 보낼 때"
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,155), FALSE); //"/*4 내용 : 사령관이 자국민 전체에게 메시지를 보낼 때"
}			// 010924 LTS

void ShowSquadInfo()
{
	char Message[100];
	sprintf(Message,lan->OutputMessage(0,157),Hero->NWCharacter.SquadNo); //"당신은 **[[제 %d 부대]]** 에 소속돼 있습니다."
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,156), FALSE); //"============== 부대   소속 =============="
	AddCurrentStatusMessage( 255,255,0, Message, FALSE);
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,158), FALSE); //"0 부대는 아무 부대에도 소속돼지 않은 것을 나타냅니다."
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,159), FALSE); //"============== 부대 메시지 =============="
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,160), FALSE); // "/*1 내용 : 부대장이 사령관에게 메시지를 보낼 때"
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,161), FALSE); //"/*  내용 : 부대원이 자신이 소속된 부대원에게 메시지를 보낼 때"
}

void ReturnNationName(int Nation,char* Message)
{
	switch (Nation)
	{
	case 3 : sprintf(Message,lan->OutputMessage(0,205)); break; //"바이서스"
	case 4 : sprintf(Message,lan->OutputMessage(0,206)); break; //"자이펀"
	case 6 : sprintf(Message,lan->OutputMessage(0,207)); break; //"일스"
	}
}

void ReturnWarfieldName(int WarfieldNo,char* Message)
{
	switch (WarfieldNo)
	{
	case 0 : sprintf(Message,lan->OutputMessage(0,208)); break; //"산중전쟁터"
	case 1 : sprintf(Message,lan->OutputMessage(0,209)); break; //"평원전쟁터"
	case 2 : sprintf(Message,lan->OutputMessage(0,210)); break; //"섬전쟁터"
	case 3 : sprintf(Message,lan->OutputMessage(0,386)); break;	// "설원전쟁터"
	}
}


void SendCMD_SQUAD_CHAT(int type,char* Message)			// Send Squad Chat Message to Server(Warfield Server)
{
	t_packet packet;

	//< LTH-040302-KO 1.4 Patch. IsNeoWarfieldServer() Add.
	if (!isNationWarfieldServer()&&!isNewWarfieldServer()&&!IsNeoWarfieldServer()) 
	{
		AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,211), FALSE); //"[전쟁터]에서만 사용할 수 있는 기능입니다."
		return;
	}
	//> LTH-040302-KO

	if (isNationWarfieldServer())
	{
		switch(type)  //안된다는 메시지 출력 
		{
		case 0 :	// "/*1"
			if (!(Hero->NWCharacter.isCommander||Hero->NWCharacter.isSquadLeader)) 
			{
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,212), FALSE); //"[사령관], [부대장]만 보낼 수 있는 메시지 기능입니다."
				return;
			}
			break;
		case 1 :	// "/*2"	
		case 2 :	// "/*3"
		case 3 :	// "/*4"
			if (!Hero->NWCharacter.isCommander) 
			{
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,213), FALSE); //"[사령관]만 보낼 수 있는 메시지 기능입니다."
				return;
			}
			break;
		case 4 :    // "/*"
			if (Hero->NWCharacter.SquadNo==0) 
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,214), FALSE); //"[부대원]만 보낼 수 있는 메시지 기능입니다."
			
		}
	}

	packet.h.header.type=CMD_SQUAD_CHAT;
	packet.u.NationWar.SquadChat.Type=type;
	packet.u.NationWar.SquadChat.Size=strlen(Message);
	strcpy(packet.u.NationWar.SquadChat.Message,Message);
	packet.h.header.size=sizeof(t_SquadChat)-MAX_STRING_PK+packet.u.NationWar.SquadChat.Size;
	QueuePacket(&packet,1);
}


void ChangeClothe(int type)			// 옷 변경 테스트 함수, 사용하지 않는다.
{
	static int i=0;

	Hero->accessory[0] = fight_map_acc_num[i];
	Hero->accessory[1] = 1;		// 아무것도 안찍을때
	LoadHeroClothAccessoryDataOne( Hero->sprno, Hero->accessory[0] );
	LoadHeroClothAccessoryDataOne( Hero->sprno, Hero->accessory[1] ); 
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,215),i); //"현재 옷번호는 %d입니다."
	if (i>3) i=0;
	else i++;
}

void ChangeClothe2()
{
	LPCHARACTER	ch=Hero;			// 시야의 모든 사람의 옷을 바꾼다.

	while (ch!=NULL)
	{
		if (ch->sprno<=1)
		{
			if (ch->name_status.nation==NW_BY)
			{
				if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)	
					ch->accessory[0] = fight_map_acc_num[3];
				else ch->accessory[0] = fight_map_acc_num[1];
				ch->accessory[1] = 1;		// 아무것도 안찍을때
			}
			if (ch->name_status.nation==NW_ZY)
			{
				if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)	
					ch->accessory[0] = fight_map_acc_num[2];
				else ch->accessory[0] = fight_map_acc_num[0];
				ch->accessory[1] = 1;		// 아무것도 안찍을때
			}
			if (ch->name_status.nation==NW_YL)
			{
				if (ch->NWCharacter.YL_JoinNation==NW_BY)			// 바이서스로 지원군 참여
				{
					if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)	
						ch->accessory[0] = fight_map_acc_num[3];
					else ch->accessory[0] = fight_map_acc_num[1];
					ch->accessory[1] = 1;		// 아무것도 안찍을때
				}
				else
				{
					if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)	
						ch->accessory[0] = fight_map_acc_num[2];
					else ch->accessory[0] = fight_map_acc_num[0];
					ch->accessory[1] = 1;		// 아무것도 안찍을때
				}
			}
			LoadHeroClothAccessoryDataOne( ch->sprno, ch->accessory[0] );
			LoadHeroClothAccessoryDataOne( ch->sprno, ch->accessory[1] ); 
		}
		ch=ch->lpNext;
	}
}


void RecvCMD_WAR_BEGIN(t_packet* p)
{
	g_bWarBegin=true;
	ChangeClothe2();  //전쟁터에 들어왔을때..
}

void RecvCMD_WAR_END(t_packet* p)
{
	g_bWarBegin=false;	//전쟁터에서 나갈때.. 서버가 알아서 옷을 바꾸어 준다.. 전쟁이 끝나면..
}

void SetWarArmor(t_server_change_equip *p,LPCHARACTER ch)		// NetWork.cpp RecvChangeEquip() Call
{
	if (isNationWarfieldServer())
	{
		if (ch->name_status.nation==NW_BY)
		{
			if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)	p->equip[0]=fight_map_acc_num[3];
			else p->equip[0]=fight_map_acc_num[1];
			p->equip[1]=1;
		}
		if (ch->name_status.nation==NW_ZY)
		{
			if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)	p->equip[0]=fight_map_acc_num[2];
			else p->equip[0]=fight_map_acc_num[0];
			p->equip[1]=1;
		}
		if (ch->name_status.nation==NW_YL)
		{
			if (ch->NWCharacter.YL_JoinNation==NW_BY)			// 바이서스로 지원군 참여
			{
				if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)	p->equip[0] = fight_map_acc_num[3];
				else p->equip[0] = fight_map_acc_num[1];
				p->equip[1] = 1;		// 아무것도 안찍을때
			}
			else
			{
				if (ch->NWCharacter.isCommander||ch->NWCharacter.isSquadLeader)	p->equip[0] = fight_map_acc_num[2];
				else p->equip[0] = fight_map_acc_num[0];
				p->equip[1] = 1;		// 아무것도 안찍을때
			}
		}
	}

	if (isNewWarfieldServer())
	{
		switch (ch->name_status.nation)
		{
		case NW_BY :
			{
				p->equip[0]=fight_map_acc_num[1];
				p->equip[1]=1;
			}
			break;
		case NW_ZY :
			{
				p->equip[0]=fight_map_acc_num[0];
				p->equip[1]=1;
			}
			break;
		case NW_YL :
			{
				if (ch->NWCharacter.YL_JoinNation==NW_BY)
				{
					p->equip[0]=fight_map_acc_num[1];
					p->equip[1]=1;
				}
				else
				{
					p->equip[0]=fight_map_acc_num[0];
					p->equip[1]=1;
				}
			}
			break;
		}
	}

	//< LTH-040302-KO 1.4 Patch. 일스도 옷갈아입자구~~
	if (IsNeoWarfieldServer())
	{
		switch (ch->name_status.nation)
		{
		case NW_BY :
			{
				p->equip[0]=fight_map_acc_num[1];
				p->equip[1]=1;
			}
			break;
		case NW_ZY :
			{
				p->equip[0]=fight_map_acc_num[0];
				p->equip[1]=1;
			}
			break;
		case NW_YL :
			{
				p->equip[0]=fight_map_acc_num[2];
				p->equip[1]=1;
			}
			break;
		}
	}
	//> LTH-040302-KO
}

void SetWarArmor(t_server_user_add *c)		// NetWork.cpp	RecvUserAdd() Call
{
	if (isNationWarfieldServer())
	{
		if (c->nation==NW_BY)
		{
			if (c->isCommander||c->isSquadLeader) c->equip0=fight_map_acc_num[3];
			else c->equip0=fight_map_acc_num[1];
			c->equip1=1;
		}
		if (c->nation==NW_ZY)
		{
			if (c->isCommander||c->isSquadLeader) c->equip0=fight_map_acc_num[2];
			else c->equip0=fight_map_acc_num[0];
			c->equip1=1;
		}
		if (c->nation==NW_YL)
		{
			if (c->YL_JoinNation==NW_BY)			// 바이서스로 지원군 참여
			{
				if (c->isCommander||c->isSquadLeader)	c->equip0 = fight_map_acc_num[3];
				else c->equip0 = fight_map_acc_num[1];
				c->equip1 = 1;		// 아무것도 안찍을때
			}
			else
			{
				if (c->isCommander||c->isSquadLeader)	c->equip0 = fight_map_acc_num[2];
				else c->equip0 = fight_map_acc_num[0];
				c->equip1 = 1;		// 아무것도 안찍을때
			}
		}
	}

	if (isNewWarfieldServer())
	{
		switch (c->nation)
		{
		case NW_BY :
			c->equip0=fight_map_acc_num[1];
			c->equip1=1;
			break;
		case NW_ZY :
			c->equip0=fight_map_acc_num[0];
			c->equip1 = 1;
			break;
		case NW_YL :
			if (c->YL_JoinNation==NW_BY)
			{
				c->equip0=fight_map_acc_num[1];
				c->equip1=1;
			}
			else
			{
				c->equip0=fight_map_acc_num[0];
				c->equip1 = 1;
			}
			break;
		}
	}

	//< LTH-040302-KO 1.4 Patch. 일스도 옷좀 갈아입짜!!!
	if (IsNeoWarfieldServer())
	{
		switch (c->nation)
		{
		case NW_BY :
			{
				c->equip0=fight_map_acc_num[1];
				c->equip1=1;
			}
			break;
		case NW_ZY :
			{
				c->equip0=fight_map_acc_num[0];
				c->equip1 = 1;
			}
			break;
		case NW_YL :
			{
				c->equip0=fight_map_acc_num[2];
				c->equip1=1;
			}
			break;
		}
	}
	//> LTH-040302-KO
}

// LineCommand.cpp , Squad Chat Message 
void SendSquadChatMessage(int type,char* Message)
{
	switch(type)
	{
	case LC_SQUAD_LEADER				:	SendCMD_SQUAD_CHAT(0,Message);	break;
	case LC_SQUAD						:	SendCMD_SQUAD_CHAT(1,Message);	break;
	case LC_SQUAD_MEMBER				:	SendCMD_SQUAD_CHAT(2,Message);	break;
	case LC_SQUAD_MEMBER_TO_MEMBER		:	SendCMD_SQUAD_CHAT(3,Message);	break;
	case LC_SQUAD_CHAT					:	SendCMD_SQUAD_CHAT(4,Message);	break;
	}
}

// LineCommand.cpp, War Give Life
void WarGiveLife()
{
	t_packet p;

	//< LTH-040303-KO 1.4 Patch 전쟁터 추가
	if (!isNationWarfieldServer()&&!isNewWarfieldServer()&&!IsNeoWarfieldServer())
	{ 
		AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,216));
		return;
	} //"[전쟁터]에서만 사용할 수 있는 명령어 입니다."
	//> LTH-040303-KO
	if (!g_bWarBegin) {AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,217));return;} //"[전쟁중]에만 사용할 수 있는 명령어 입니다."
	if (Hero->hp>0) {AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,218)); return;} //"[유령] 상태에서만 사용할 수 있는 명령어 입니다."

	p.h.header.type=CMD_REQUEST_WAR_GIVE_LIFE;
	p.h.header.size=0;

	QueuePacket(&p,1);
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,219)); //"빨리 회복하기 위해서는 음식과 포션을 먹으셔야 합니다."
}

void WarGiveLife2(char* Arg)
{
	t_packet packet;
	int GiveLifePoint=Arg[0]-48;			// 48 ASCII '0'
	if (GiveLifePoint<0||GiveLifePoint>3)  { AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,328));  return; }
	if (!isNationWarfieldServer()) { AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,216));return;} //"[전쟁터]에서만 사용할 수 있는 명령어 입니다."
	if (!g_bWarBegin) {AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,217));return;} //"[전쟁중]에만 사용할 수 있는 명령어 입니다."
	if (Hero->hp>0) {AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,218)); return;} //"[유령] 상태에서만 사용할 수 있는 명령어 입니다."

	packet.h.header.type=CMD_WAR_GIVE_LIFE2;
	packet.u.NationWar.CommonDataC.Data=GiveLifePoint;
	packet.h.header.size=sizeof(t_CommonDataC);
	QueuePacket(&packet,1);
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,219)); //"빨리 회복하기 위해서는 음식과 포션을 먹으셔야 합니다."
}
// Interface Concern Packet Send Function

void SendCMD_REQUEST_WARFIELD_STATUS(int RequestType)		// Request Warfield Status , Parameter : Button No
{
	t_packet p;

	p.h.header.type=CMD_REQUEST_WARFIELD_STATUS;
	p.u.NationWar.CommonDataC.Data=RequestType;				// Just Checking Warfield Status
	p.h.header.size=sizeof(t_CommonDataC);
	QueuePacket( &p, 1 );
}

int CheckWarfieldLoop()
{
	if (g_pNation->GetStatus(0)>=2) return 0;
	if (g_pNation->GetStatus(1)>=2) return 1;
	if (g_pNation->GetStatus(2)>=2) return 2;
	return -1;
}

void SendCMD_REQUEST_WARFIELD_INFO()
{
	t_packet p;

	int WarfieldNo=CheckWarfieldLoop();

	if (WarfieldNo>=0)
	{
		p.h.header.type=CMD_REQUEST_WARFIELD_INFO;
		p.u.NationWar.CommonDataC.Data=WarfieldNo;
		p.h.header.size=sizeof(t_CommonDataC);
		QueuePacket( &p, 1 );
	}
}

void SendCMD_REQUEST_VOTE_DATA(int WarfieldNo)				// Request Vote Data
{															// CMD_REQUEST_WARFIELD_STATUS -> CMD_ANSWER_WARFIELD_STATUS
	t_packet p;												// CMD_REQUEST_VOTE_DATA -> CMD_ANSWER_VOTE_DATA	
	p.h.header.type=CMD_REQUEST_VOTE_DATA;					// RecvCMD_ANSWER_VOTE_DATA() -> CallSmallMenu()
	p.u.NationWar.CommonDataC.Data=WarfieldNo;
	p.h.header.size=sizeof(t_CommonDataC);
	QueuePacket( &p, 1 );
}

void RecvCMD_ANSWER_VOTE_DATA(t_packet* p)					// Nation War Join Interface Open
{															// Display Data	
	if (p->u.NationWar.VoteData.WarfieldNo>=0)
	{
		//그래프로 그리기
//		p->u.NationWar.VoteData.WarfieldNo, ->전쟁터 넘버

		switch(p->u.NationWar.VoteData.WarfieldNo)
		{
		case 0: //산중
			SMenu[MN_NATION_JOIN].nField[2].nImageNumber = 1;
			break;
		case 1: //평원
			SMenu[MN_NATION_JOIN].nField[2].nImageNumber = 3;
			break;
		case 2: //섬
			SMenu[MN_NATION_JOIN].nField[2].nImageNumber = 2;
			break;
		}
		//010927 lsw
		char msg[10];
		sprintf(msg,"%d",p->u.NationWar.VoteData.VoteCount);
		strcpy(SMenu[MN_NATION_JOIN].nField[5].temp, msg );
		SMenu[MN_NATION_JOIN].nField[4].nWillDo =p->u.NationWar.VoteData.VoteCount ;

		char msg2[20];
		ReturnNationName(g_DefenceNation,msg2);
		char msg3[20];
		ReturnWarfieldName(p->u.NationWar.VoteData.WarfieldNo,msg3);
		//010928 lsw
		sprintf(SMenu[MN_NATION_JOIN].nField[3].temp,lan->OutputMessage(0,142), msg2,msg3 ); //" [ %s ]국가로부터 [ %s ]를 탈환하기 위해 전쟁 선포 여부를 투표하는 곳입니다.\n 투표 기간은 5분이 주어지며 그 기간 안에 5명이 넘게 되면 자동으로 전쟁 선포가 이뤄집니다.\n 전쟁은 전쟁 선포 후 20분 후에 벌어지게 됩니다." 
		
		CallSmallMenu(MN_NATION_JOIN,MN_NATION_BATTLE_BBS);
	}
	else
	{
		CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,220), FALSE); //"전쟁참여 투표를 할 수 없습니다."
	}
}

//전쟁참여 투표
//인터페이스 가 열려있는지 확인 한다.. 딜레이 타임에 의해 선거기간이 지났는데도 투표를 한다면.. 
//투표를 못했다는 패킷이 날아온다..아니 날아와야 한다.
//캐릭터 리스트 및 DB에 저장해야 한다.
void SendCMD_VOTING_DATA()									// Send Nation War Join Data
{															// Ack Does Not Retrun
	t_packet p;	
	
	if (Hero->NWCharacter.DoWarJoinVote) 
	{
		CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,221), FALSE); //"이미 전쟁 참여 투표를 하셨습니다."
		return;
	}

	int WarfieldNo=CheckNationStatus(NW_VOTE);			
    if (WarfieldNo>=0) 
	{
		if (Hero->name_status.nation!=g_DefenceNation)
		{
			p.h.header.type=CMD_VOTING_DATA;
			p.u.NationWar.CommonDataC.Data=WarfieldNo;
			p.h.header.size=sizeof(t_CommonDataC);
			QueuePacket( &p, 1 );
		
			char msg[80],msg2[200];
			ReturnWarfieldName(WarfieldNo,msg);
			sprintf(msg2,lan->OutputMessage(0,222),msg); //"적국의 [ %s ] 로 전쟁에 동참 의사를 밝혔습니다."
			CallOkCancelMessageBox(TRUE, 0,0, msg2, FALSE);
			Hero->NWCharacter.DoWarJoinVote=true;
		}
		else
		{
			CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,223), FALSE); //"방어국가의 국민은 전쟁참여투표를 할 수 없습니다."
		}
	}
}

void SendCMD_REQUEST_SQUAD_INFO(int SquadNo)				// Request Warfield Squad Information
{															// Open Turn of Squad Menu
	t_packet p;												// Interface Click -> CMD_REQUEST_WARFIELD_STATUS ->
	int WarfieldNo=CheckNationStatus(NW_PREPARE);						// CMD_ANSWER_WARFIELD_STATUS -> CMD_REQUEST_SQUAD_INFO ->
	if (WarfieldNo<0) WarfieldNo=CheckNationStatus(NW_WAR);		// 010927 LTS
	if (WarfieldNo>=3)
	{
		//<! BBD 040322
		char tempMessage[200];
		switch(WarfieldNo)
		{
		case WI_SNOWY_WARFIELD:
			{
				sprintf(tempMessage, lan->OutputMessage(0,292), lan->OutputMessage(0,386));  //설원전쟁터는 부대기능을 사용하지 않습니다.
				break;
			}
		case WI_HADES_WARFIELD:
			{
				sprintf(tempMessage, lan->OutputMessage(0,292), lan->OutputMessage(0,619));
				break;
			}
		default:
			{
				break;
			}
		}
		CallOkCancelMessageBox(TRUE, 0,0,tempMessage, FALSE);
		return;
		//> BBD 040322
	}
															// CMD_ANSWER_SQUAD_INFO -> CallOkCancelMessageBox(TRUE()

	if (SquadNo<1)											// 부대 번호 0은 부대 편성이 되지 않은 유저의 모임이다.
	{
		SquadNo=1;
		g_Squad=1;
	}
	if (SquadNo>4)
	{
		SquadNo=4;
		g_Squad=4;
	}
	if (WarfieldNo>=0)
	{
		p.h.header.type=CMD_REQUEST_SQUAD_INFO;
		p.u.NationWar.RequestSquadInfo.Nation=Hero->name_status.nation;		// 일스인경우... 번호가 들어간다.
		p.u.NationWar.RequestSquadInfo.WarfieldNo=WarfieldNo;
		p.u.NationWar.RequestSquadInfo.SquadNo=SquadNo;
		p.h.header.size=sizeof(t_RequestSquadInfo);
		QueuePacket( &p, 1 );
		//AddCurrentStatusMessage( 255,255,0, "부대편성 데이터를 요청하였습니다. 부대번호:%d",g_Squad);
	}
}

void RecvCMD_ANSWER_SQUAD_INFO(t_packet* p)					// Squad Information Packet process
{
	//MN 부대 인터 페이스
	g_SquadLoadingPoint	=	p->u.NationWar.SquadInfo.LoadingPoint;
	g_SquadNum = p->u.NationWar.SquadInfo.SquadNo;

	NationSupportNationSquadLeader( p);//010910 lsw
}

void RecvCMD_ANSWER_SQUAD_INFO2(t_packet* p)
{
	NationSupportNationSquadLeader2( p);//010910 lsw
}

//사령관 투표데이타 요청
void SendCMD_REQUEST_COMMANDER_VOTE_DATA(int CandidaterNo)	// Request Commander Candidater Information, Parameter : Candidater No
{
	t_packet p;
	int WarfieldNo=CheckNationStatus(NW_PREPARE);

	if (CandidaterNo<0)								// 사령관 번호의 체크..
	{
		g_Commander=0;
		CandidaterNo=0;
	}
	if (CandidaterNo>5)
	{
		g_Commander=5;
		CandidaterNo=5;
	}

	if (WarfieldNo>=0)
	{
		p.h.header.type=CMD_REQUEST_COMMANDER_VOTE_DATA;
		p.u.NationWar.RequestCommanderVoteData.Nation=Hero->name_status.nation;
		p.u.NationWar.RequestCommanderVoteData.WarfieldNo=WarfieldNo;
		p.u.NationWar.RequestCommanderVoteData.CandidaterNo=CandidaterNo;
		p.h.header.size=sizeof(t_RequestCommanderVoteData);
		QueuePacket( &p, 1 );
		//AddCurrentStatusMessage( 255,255,0, "사령관 투표자료를 요청하였습니다. 국가번호:%d, 사령관 투표자료:%d",WarfieldNo,CandidaterNo);
	}
}
void RecvCMD_ANSWER_COMMANDER_VOTE_DATA(t_packet* p)		// Answer Commander Candidater Information
{
	//이거도 바꿔야 됨 만들어 놓았음
//	AddCurrentStatusMessage( 255,255,0, "사령관 후보 %d의 데이터입니다.",p->u.NationWar.CommanderVoteData.VoteNo);
//	AddCurrentStatusMessage( 255,255,0, "후보ID : %d, 후보이름 : %s",p->u.NationWar.CommanderVoteData.CandidaterID,p->u.NationWar.CommanderVoteData.Name);
//	AddCurrentStatusMessage( 255,255,0, "레벨 : %d, 명성치 : %d, 지지표 :%d",p->u.NationWar.CommanderVoteData.Level,p->u.NationWar.CommanderVoteData.Fame,p->u.NationWar.CommanderVoteData.VoteCount);
	g_CommanderCandidaterLevel = p->u.NationWar.CommanderVoteData.Level;
	
	if(p->u.NationWar.CommanderVoteData.CandidaterID != 0)
	{
	sprintf(SMenu[MN_NATION_VOTE].nField[5].temp,"%d",SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][p->u.NationWar.CommanderVoteData.VoteCount]);
	sprintf(SMenu[MN_NATION_VOTE].nField[6].temp,"%s",p->u.NationWar.CommanderVoteData.Name);
  const __int8 nBefore = p->u.NationWar.CommanderVoteData.Class;	// 클래스		// 011020 LTS
  const __int8 nAfter = p->u.NationWar.CommanderVoteData.nDual;	// 듀얼..		// 011020 LTS
	const __int8 nStep = p->u.NationWar.CommanderVoteData.nStep;	// 스텝			// 011020 LTS
  const string strName = g_mgrDual.GetClassName(nBefore, nAfter, nStep, g_CommanderCandidaterLevel);
	sprintf(SMenu[MN_NATION_VOTE].nField[7].temp,"%s", strName.c_str() );
	sprintf(SMenu[MN_NATION_VOTE].nField[8].temp,"%d",p->u.NationWar.CommanderVoteData.Fame);
	sprintf(SMenu[MN_NATION_VOTE].nField[9].temp,"%d",p->u.NationWar.CommanderVoteData.LadderScore);
	SMenu[MN_NATION_VOTE].nField[10].nWillDo = 1;//얼굴 이미지 켜기
	SMenu[MN_NATION_VOTE].nField[10].nImageType = p->u.NationWar.CommanderVoteData.Gender;
	SMenu[MN_NATION_VOTE].nField[10].nRectImage = p->u.NationWar.CommanderVoteData.Face;
	}
	else
	{
	SMenu[MN_NATION_VOTE].nField[10].nWillDo = 0;
	sprintf(SMenu[MN_NATION_VOTE].nField[5].temp,lan->OutputMessage(0,224)); //"등록대기"
	sprintf(SMenu[MN_NATION_VOTE].nField[6].temp,lan->OutputMessage(0,224)); //"등록대기"
	sprintf(SMenu[MN_NATION_VOTE].nField[7].temp,lan->OutputMessage(0,224)); //"등록대기"
	sprintf(SMenu[MN_NATION_VOTE].nField[8].temp,lan->OutputMessage(0,224)); //"등록대기"
	sprintf(SMenu[MN_NATION_VOTE].nField[9].temp,lan->OutputMessage(0,224)); //"등록대기"
	}
}

void RecvCMD_ANSWER_COMMANDER_VOTE_DATA2(t_packet* p)
{
	char msg[200];
	memset(msg,0,200);
	
	for ( int i = 0 ; i < 6 ; i++)//011001 lsw
	{
		sprintf(msg,"%s\n   [%d]",p->u.NationWar.CommanderVoteData2.CandidaterName[i],p->u.NationWar.CommanderVoteData2.VoteCount[i]);
		strcpy(SMenu[MN_NATION_VOTE].nField[11+i].temp, msg);
	}
	
	//011005 lsw >
	CallSmallMenu(MN_NATION_VOTE,MN_NATION_BATTLE_BBS);	// Commander Candidater Interface Open
	//011005 lsw <
}


bool CheckJoinLevel(int SquadNo)	// 011015 LTS
{
	switch (SquadNo)
	{
	case 1 :
		if (Hero->lv>=NW_SQUAD1_LEVEL_MIN&&Hero->lv<=NW_SQUAD1_LEVEL_MAX) return true;
		return false;
	case 2 :
		if (Hero->lv>=NW_SQUAD2_LEVEL_MIN&&Hero->lv<=NW_SQUAD2_LEVEL_MAX) return true;
		return false;
	case 3 :
		if (Hero->lv>=NW_SQUAD3_LEVEL_MIN&&Hero->lv<=NW_SQUAD3_LEVEL_MAX) return true;
		return false;
	case 4 :
		if (Hero->lv>=NW_SQUAD4_LEVEL_MIN&&Hero->lv<=NW_SQUAD4_LEVEL_MAX) return true;
		return false;
	}
	return true;

}
//부대를 변경 한다.. 
//부대번호는 전쟁번호와 함께 DB에 저장되어야 한다.
//이미 부대가 있는경우... //전쟁번호와 비교를 한다. // 쉬팍 부대장이면.. 안된다..
//이미 부대가 있는 경우  
//이전 부대의 번호  //캐릭터 리스트에 추가되어야 하는 사항 
//부대에 새로 조인하는 경우 
//이전 부대의 번호  //캐릭터 리스트에 추가되어야 하는 사항 
void SendCMD_CHANGE_SQUAD(int SquadNo)						// Squad Registration Change 
{
	t_packet p;
	int WarfieldNo=CheckNationStatus(NW_PREPARE);    

	if (Hero->NWCharacter.isSquadLeader) 
	{
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,225), FALSE); //"부대장은 부대를 바꾸실 수 없습니다."
		return;
	}

	if (Hero->NWCharacter.SquadNo) //문제 초기값으로 0이아닌 값이 있어야 한다. // If Hero Have Old Squad No
	{														// Send CMD_CHANGE_SQUAD Packet to Manage Server
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,226), FALSE); //"다른 부대로 등록하실 수 없습니다."
	}
	else													// Hero Does Not Have Old Squad No
	{														// Send CMD_JOIN_SQUAD Packet to Manage Server
		if (CheckJoinLevel(SquadNo))						// 011015 LTS
		{
			p.h.header.type=CMD_JOIN_SQUAD;						// Answer was CMD_JOIN_SQUAD or CMD_YOU_ARE_SQUAD_LEADER
			p.u.NationWar.JoinSquad.Nation=Hero->name_status.nation;
			p.u.NationWar.JoinSquad.WarfieldNo=WarfieldNo;    
			p.u.NationWar.JoinSquad.SquadNo=g_Squad; //바뀐다.. 인터페이스 값...
			p.h.header.size=sizeof(t_JoinSquad);
			QueuePacket( &p, 1 );

			char msg[80],msg2[200];
			ReturnWarfieldName(p.u.NationWar.JoinSquad.WarfieldNo,msg);
			sprintf(msg2,lan->OutputMessage(0,227),msg, p.u.NationWar.JoinSquad.SquadNo); //"[ %s ]의 [제 %d 부대] 부대원으로 등록됐습니다."
			PlayListAutoSounds(257,0,0,0);
			CallOkCancelMessageBox(TRUE, 0,0, msg2, FALSE);
		}
		else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,228), FALSE); //"해당 부대에 등록하기에 맞지 않은 레벨입니다."
	}
}

void RecvCMD_YOU_ARE_SQUAD_LEADER(t_packet *p)
{
	//부대장이므로.. 관련된 데이타를 세트한다.. ㅜ.ㅜ
	if (Hero->name_status.nation==NW_YL) Hero->NWCharacter.YL_JoinNation=p->u.NationWar.JoinSquad.Nation;
	Hero->NWCharacter.WarfieldNo=p->u.NationWar.JoinSquad.WarfieldNo;
	Hero->NWCharacter.SquadNo=p->u.NationWar.JoinSquad.SquadNo;
	Hero->NWCharacter.isSquadLeader=1;
	char msg[80],msg2[200];
	ReturnWarfieldName(Hero->NWCharacter.WarfieldNo,msg);
	sprintf(msg2,lan->OutputMessage(0,229),msg,Hero->NWCharacter.SquadNo); //"[ %s ]의 [제 %d 부대] 부대장에 등록됐습니다."

	CallOkCancelMessageBox(TRUE, 0,0, msg2, FALSE);
}

void RecvCMD_JOIN_SQUAD(t_packet *p)
{
	if (Hero->name_status.nation==NW_YL) Hero->NWCharacter.YL_JoinNation=p->u.NationWar.JoinSquad.Nation;
	Hero->NWCharacter.WarfieldNo=p->u.NationWar.JoinSquad.WarfieldNo;
	Hero->NWCharacter.SquadNo=p->u.NationWar.JoinSquad.SquadNo;
	char msg[80],msg2[200];
	ReturnWarfieldName(Hero->NWCharacter.WarfieldNo,msg);
	sprintf(msg2,lan->OutputMessage(0,227),msg,Hero->NWCharacter.SquadNo); //"[ %s ]의 [제 %d 부대] 부대원으로 등록됐습니다."

	CallOkCancelMessageBox(TRUE, 0,0, msg2, FALSE);

}

//부대 로딩포인트의 지정 
//부대장인지 확인한다.
void SendCMD_SET_SQUAD_LODING_POINT(int SquadNo,int LoadingPoint) 
{
	t_packet p;

	if (Hero->name_status.nation==g_DefenceNation)
	{
		CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,230), FALSE); //"[방어국가]는 공격거점을 지정하지 않으셔도 됩니다."
		return;
	}

	if (!Hero->NWCharacter.isSquadLeader)
	{
		CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,231), FALSE); //"[부대장]이 아니면 공격거점을 지정할 수 없습니다."
		return;
	}

	int WarfieldNo=CheckNationStatus(NW_PREPARE);   

	if (WarfieldNo>=0)
	{
		p.h.header.type=CMD_SET_SQUAD_LOADING_POINT;
		p.u.NationWar.SetLoadingPoint.Nation=Hero->name_status.nation;
		p.u.NationWar.SetLoadingPoint.WarfieldNo=WarfieldNo;
		p.u.NationWar.SetLoadingPoint.SquadNo=SquadNo;
		p.u.NationWar.SetLoadingPoint.LoadingPoint=LoadingPoint;
		p.h.header.size=sizeof(t_SetLoadingPoint);
		QueuePacket( &p, 1 );
		
		//011010 lsw >
		char msg[80];
		ReturnWarfieldName(WarfieldNo,msg);
		char msgbody[160];
		sprintf(msgbody, lan->OutputMessage(0,232),msg,SquadNo,LoadingPoint); //"[ %s ]의 [제 %d 부대] 공격로를 [ %d 번 ]지역으로 정했습니다."
		CallOkCancelMessageBox(TRUE, 0,0, msgbody, TRUE);
		
		SMenu[MN_NATION_INFO].nField[7].nWillDo=1;
		SMenu[MN_NATION_INFO].nField[7].x = SMenu[MN_NATION_INFO].nField[LoadingPoint].x;
		SMenu[MN_NATION_INFO].nField[7].y = SMenu[MN_NATION_INFO].nField[LoadingPoint].y;
		//011010 lsw <

		Hero->NWCharacter.WarfieldNo=WarfieldNo;
		Hero->NWCharacter.SquadNo=SquadNo;				//자신의 부대번호를 저장한다.   // 서버로 부터 확인 메시지 오지 않음 

		
	}

	g_SquadLoadingPoint = LoadingPoint;//010901 lsw
}

void SendCMD_COMMANDER_REG_DATA(int CandidaterNo) //사령관 등록 
{
	//자기자신을 후보로 처리한다... //제한조건을 검사후.. 보내지 않을 수도 있다.
	t_packet p;
	int WarfieldNo=CheckNationStatus(NW_PREPARE);
	if (WarfieldNo>=0)
	{
		if (!Hero->NWCharacter.isCommanderCandidater)
		{
			if ((Hero->lv>=NW_COMMANDER_LEVEL_MIN)&&(Hero->fame>=NW_COMMANDER_FAME_MIN)) // 010927 LTS
			{
				p.h.header.type=CMD_REQUEST_COMMANDER_REG_DATA;
				p.u.NationWar.RequestCommanderVoteData.Nation=Hero->name_status.nation;
				p.u.NationWar.RequestCommanderVoteData.WarfieldNo=WarfieldNo;
				p.u.NationWar.RequestCommanderVoteData.CandidaterNo=CandidaterNo;
				p.h.header.size=sizeof(t_RequestCommanderVoteData);
				QueuePacket( &p, 1 );
				char msg[80];
				ReturnWarfieldName(WarfieldNo,msg);
				AddCurrentStatusMessage( 255,255,0, 
				lan->OutputMessage(0,233),msg,CandidaterNo); //"[ %s ]의 [ %d번 ] 후보로 등록 요청 하셨습니다."
			}
			else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,234), FALSE); //"사령관 후보로 등록되기에는 능력이 부족합니다."
		}
		else AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,235)); //"이미 사령관 후보에 등록을 하셨습니다."
	}
}

void RecvCMD_ANSWER_COMMANDER_REG_DATA(t_packet* p)
{
	switch (p->u.NationWar.CommonDataC.Data)
	{
	case 0 :
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,236), FALSE); //"사령관 후보에 등록되지 못했습니다."
		break;
	case 1 :
		Hero->NWCharacter.isCommanderCandidater=1;
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,237), FALSE); //"[사령관] 후보에 등록을 하셨습니다."
		break;
	case 2 :
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,234), FALSE); //"사령관 후보로 등록되기에는 능력이 부족합니다."
		break;
	}
}


void SendCMD_COMMANDER_VOTING_DATA(int CandidaterNo) //사령관 투표 
{
	t_packet p;

	if (Hero->NWCharacter.DoCommanderVote)
	{
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,238), FALSE); //"이미 사령관 투표를 했습니다."
		return;
	}
	int WarfieldNo=CheckNationStatus(NW_PREPARE);
	
	if (WarfieldNo>=0)
	{
		p.h.header.type=CMD_COMMANDER_VOTING_DATA;
		p.u.NationWar.RequestCommanderVoteData.Nation=Hero->name_status.nation;
		p.u.NationWar.RequestCommanderVoteData.WarfieldNo=WarfieldNo;
		p.u.NationWar.RequestCommanderVoteData.CandidaterNo=CandidaterNo;
		p.h.header.size=sizeof(t_RequestCommanderVoteData);
		QueuePacket( &p, 1 );
		char msg[80];
		ReturnWarfieldName(WarfieldNo,msg);
		//011008 lsw >
		char msgbody[120];
		sprintf(msgbody,lan->OutputMessage(0,239),msg,CandidaterNo+1); //"[ %s ]의 사령관으로 [ %d번 ] 후보를 지지했습니다." // 011020 LTS
		CallOkCancelMessageBox(FALSE, 0, 0, msgbody, TRUE);
		//011008 lsw <
		Hero->NWCharacter.DoCommanderVote=true;
	}
}

void SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(int Type)
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_CHECK_WARFIELD_POSSESSION;
	packet.u.NationWar.CommonDataC.Data=Type;
	packet.h.header.size=sizeof(t_CommonDataC);
	QueuePacket( &packet, 1 );
}

void SendCMD_REQUEST_WARFIELD_POSSESSION()
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_WARFIELD_POSSESSION;
	packet.h.header.size=0;
	QueuePacket( &packet, 1 );
}

void SendCMD_FREELANCER_SQUAD_JOIN(int Nation)
{
	t_packet packet;

	Hero->NWCharacter.YL_JoinNation=Nation;		// 011020 LTS
	packet.h.header.type=CMD_FREELANCER_SQUAD_JOIN;
	packet.u.NationWar.CommonDataC.Data=Nation;
	packet.h.header.size=sizeof(t_CommonDataC);
	QueuePacket(&packet,1);

	PlayListAutoSounds(257,0,0,0);		// 010927 LTS

	switch (Nation)						// 010927 LTS
	{
	case NW_ZY : 
		CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,240), FALSE); //"이번 전쟁에서 [자이펀] 용병으로 소속됐습니다."
//		Hero->NWCharacter.YL_JoinNation=NW_ZY;	// 011020 LTS
		break;
	case NW_BY : 
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,241), FALSE); //"이번 전쟁에서 [바이서스] 용병으로 소속됐습니다."
//		Hero->NWCharacter.YL_JoinNation=NW_BY;	// 011020 LTS
		break;
	}
}

void SendCMD_REQUEST_WARFIELD_WAR_DATA(int WarfieldNo)
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_WARFIELD_WAR_DATA;
	packet.u.NationWar.CommonDataC.Data=WarfieldNo;
	packet.h.header.size=sizeof(t_CommonDataC);
	QueuePacket(&packet,1);
}

void SendCMD_REQUEST_NEW_WARFIELD_DATA(int WarfieldNo)
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_NEW_WARFIELD_DATA;
	packet.u.NationWar.CommonDataC.Data=WarfieldNo;
	packet.h.header.size=sizeof(t_CommonDataC);

	QueuePacket(&packet,1);
}

void SendJoinNewWarfield(int Nation)
{
	if (Hero->NWCharacter.YL_JoinNation==NW_BY||Hero->NWCharacter.YL_JoinNation==NW_ZY)
	{
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,293),FALSE);   // "이미 지원군 편성되었습니다."
		return;
	}

	t_packet packet;
	packet.h.header.type=CMD_WAR_YL_JOIN;
	packet.u.NationWar.CommonDataC.Data=Nation;
	packet.h.header.size=sizeof(t_CommonDataC);

	QueuePacket(&packet,1);
}

void RecvCMD_ANSWER_WARFIELD_STATUS(t_packet* p)			// Answer from Nation Manage Server
{
	int  WarfieldNo=-1;

	//AddCurrentStatusMessage( 255,255,0, "CMD_ANS_WARFIELD_STATUS %d,%d,%d",p->u.NationWar.WarfieldStatus.Status[0],p->u.NationWar.WarfieldStatus.Status[1],p->u.NationWar.WarfieldStatus.Status[2] );
	g_pNation->SetStatus(0,p->u.NationWar.WarfieldStatus.Status[0]);
	g_pNation->SetStatus(1,p->u.NationWar.WarfieldStatus.Status[1]);
	g_pNation->SetStatus(2,p->u.NationWar.WarfieldStatus.Status[2]);

	SendCMD_REQUEST_WARFIELD_INFO();		//전쟁터의 공격 국가와 방어 국가를 정한다. // 디펜서는 알수있지만.. 공격국가는 선전포고 후에 알수 있다. (일스,바이서스,자이펀 전쟁선포를 할 수 있다면)

	switch (p->u.NationWar.WarfieldStatus.PacketStatus)
	{
	case BUTTON_NATION_JOIN :  //전쟁참여관련 
		WarfieldNo=CheckNationStatus(NW_VOTE); //투표하는 국가확인  //추후 국적확인 // 010715 LTS TEMP
        if (WarfieldNo>=0)
		{
			if (Hero->name_status.nation!=g_DefenceNation)		// 010927 LTS
			{
				if (Hero->name_status.nation!=NW_YL)
				{
					char msg[80];
					ReturnWarfieldName(WarfieldNo,msg);
					AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,242),msg); //"[ %s ] 전쟁 선포에 대해서 국민들의 투표를 받고 있습니다."
					SendCMD_REQUEST_VOTE_DATA(WarfieldNo);
				}
				else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,243), FALSE); //"일스 국민은 전쟁 참여 투표를 할 수 없습니다."
			}
			else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,244), FALSE); //"방어국가의 국민은 전쟁 참여투표를 할 수 없습니다."
		}
		else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,245), FALSE); //"전쟁 참여 투표를 받고 있는 전쟁터가 없습니다."
		break;
	case BUTTON_NATION_SQUAD : //부대인터페이스 오픈		// 010927 LTS
		if (Hero->name_status.nation==NW_YL) 
		{
			CallOkCancelMessageBox(TRUE, 0,0, 
				lan->OutputMessage(0,246), FALSE); // "일스 국민은 [용병편성] 메뉴를 선택해서 용병으로 전쟁에 참여해야 합니다."
			return;
		}
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			char msg[80];
			ReturnWarfieldName(WarfieldNo,msg);
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,247),msg); //"[ %s ]의 전쟁에 참여 할 부대를 모집하고 있습니다."
			SendCMD_REQUEST_SQUAD_INFO(g_Squad);			// 인터페이스의 화살표에 의해 인자가 결정된다.. 맨처음은 0이다.	
		}
		else 
		{
			WarfieldNo=CheckNationStatus(NW_WAR);
			if (WarfieldNo>=0)
			{
				SendCMD_REQUEST_SQUAD_INFO(g_Squad); // 눌러지지 않는다.		 공격로 지정, 부대편성, 사령관 등록 
			}
			else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,248), FALSE); //"[전쟁준비중], [전쟁중] 기간에만 사용할 수 있습니다."
		}
		break;
	case	BUTTON_NATION_CHECK_COMMANDER :
		if (g_bWarBegin) break;			// 010927 LTS		// 전쟁중이면 눌리지 않는다.
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			SendCMD_REQUEST_COMMANDER_VOTE_DATA(g_Commander);  // 인터페이스의 화살표에 의해 인자가 결정된다.. 맨처음은 0이다.
		}
		else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,249), FALSE); //"[전쟁준비중] 기간에만 사용할 수 있습니다."
		
		break;
	case	BUTTON_COMMANDER_REG :
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			SendCMD_COMMANDER_REG_DATA(g_Commander);  //임시 // 인터페이스의 화살표에 의해 인자가 결정된다.. 맨처음은 0이다.
		}
		else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,250), FALSE); //"[사령관 투표] 기간이 아닙니다."

		break;
	case	BUTTON_COMMANDER_VOTE :
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			if (g_CommanderCandidaterLevel==0) 
				CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,251), FALSE); //"[사령관 후보]가 등록 되지 않았습니다."
			else SendCMD_COMMANDER_VOTING_DATA(g_Commander);				// 인터페이스의 화살표에 의해 인자가 결정된다.. 맨처음은 0이다.
		}
		else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,252), FALSE); //"투표가 안 됐습니다."
		break;

	case	BUTTON_SQUAD_JOIN :
		if (g_bWarBegin) break;			// 010927 LTS		// 전쟁중이면 눌리지 않는다.
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			SendCMD_CHANGE_SQUAD(g_Squad);				// 인터페이스의 화살표에 의해 인자가 결정된다.. 맨처음은 0이다.
		}
		else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,253), FALSE); //"부대에 등록이 안 됐습니다."
		break;
	case	BUTTON_SQUAD_LODINGPOINT :
		if (g_bWarBegin) break;			// 010927 LTS		// 전쟁중이면 눌리지 않는다.
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			if (Hero->name_status.nation==g_DefenceNation) 
				CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,254), FALSE); //"부대의 공격거점이 정해지지 않았습니다."
			else SendCMD_SET_SQUAD_LODING_POINT(g_Squad,1);	// 인터페이스의 화살표에 의해 인자가 결정된다.. 맨처음은 0이다.
		}
		else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,255), FALSE); //"일스 국민만 사용할 수 있습니다."
		break;
	case	BUTTON_JOIN_FREELANCER :
		if (Hero->name_status.nation==NW_YL)  // 010927 LTS
		{
			WarfieldNo=CheckNationStatus(NW_PREPARE);
			if (WarfieldNo>=0)	CallSmallMenu(MN_NATION_GUEST,MN_NATION_BATTLE_BBS);
			else 
			{
				WarfieldNo=CheckNationStatus(NW_WAR);
				if (WarfieldNo>=3)
				{	//< LTH-040316-KO 몬스터 전쟁터에서 일스의 용병참여 메뉴 처리
					if (!IsNeoWarfield(WarfieldNo))
						CallSmallMenu(MN_NATION_GUEST,MN_NATION_BATTLE_BBS);
					else
						CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,622), FALSE); //"몬스터 전쟁터에서 일스는 용병으로 참여하지 않습니다"
				}	//> LTH-040316-KO
				else
					CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,249), FALSE); //"[전쟁준비중] 기간에만 사용할 수 있습니다."
			}
		}
		else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,255), FALSE); //"일스 국민만 사용할 수 있습니다."

		break;
	case	BUTTON_FREELANCER_SQUAD :
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			//CallOkCancelMessageBox(TRUE, 0,0, "샘플맵에서는 지원되지 않는 기능입니다.", FALSE);
			SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(1);  //일스 부대 편성
		} else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,256), FALSE); //"용병부대를 편성할 수 있는 상황이 아닙니다."
		break;
	case	BUTTON_BY_JOIN :
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(4); // 바이조인..		// 011020 LTS
			//if (Hero->NWCharacter.YL_JoinNation==0) SendCMD_FREELANCER_SQUAD_JOIN(NW_BY);  // define
			//else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,257), FALSE); //"이미 용병으로 지원할 국가를 선택했습니다."
		} 
		else 
		{
			WarfieldNo=CheckNationStatus(NW_WAR);
			if (WarfieldNo>=3)
			{	//< LTH-040316-KO 일스 용병참여 처리 몬스터 국가전에서는 일스가 용병이 아니다
				if (!IsNeoWarfield(WarfieldNo))
				{
					SendJoinNewWarfield(NW_BY);
					CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,294),FALSE);	// "바이서스 지원군으로 편성되었습니다."
				}
				else
					CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,622), FALSE); //"몬스터 전쟁터에서 일스는 용병으로 참여하지 않습니다"
			}	//> LTH-040316-KO
			else
				CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,249), FALSE); //"[전쟁준비중] 기간에만 사용할 수 있습니다."
		}
		break;
	case	BUTTON_ZY_JOIN :
		WarfieldNo=CheckNationStatus(NW_PREPARE);
		if (WarfieldNo>=0)
		{
			SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(5); // 자이조인..		// 011020 LTS
			//if (Hero->NWCharacter.YL_JoinNation==0) SendCMD_FREELANCER_SQUAD_JOIN(NW_BY);  // define
			//else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,257), FALSE); //"이미 용병으로 지원할 국가를 선택했습니다."
		} 
		else 
		{
			WarfieldNo=CheckNationStatus(NW_WAR);
			if (WarfieldNo>=3)
			{	//< LTH-040316-KO 일스 용병참여 처리 몬스터 국가전에서는 일스가 용병이 아니다
				if (!IsNeoWarfield(WarfieldNo))
				{
					SendJoinNewWarfield(NW_ZY);
					CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,295),FALSE);		//  "자이펀 지원군으로 편성되었습니다."
				}
				else
					CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,622), FALSE); //"몬스터 전쟁터에서 일스는 용병으로 참여하지 않습니다"
			}	//> LTH-040316-KO
			else
				CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,249), FALSE); //"[전쟁준비중] 기간에만 사용할 수 있습니다."
		}
		break;
	case	BUTTON_NATION_STATUS :
		WarfieldNo=CheckNationStatus(NW_WAR);
		if (WarfieldNo>=0)
		{
			if (WarfieldNo>=3)
			{
				SendCMD_REQUEST_NEW_WARFIELD_DATA(WarfieldNo);
			}
			else
				SendCMD_REQUEST_WARFIELD_WAR_DATA(WarfieldNo);
		} 
		else 
		{
			SendCMD_LOCALWAR_STATUS();
			//CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,258), FALSE); //"[전쟁중] 기간에만 사용할 수 있습니다."
		}
		break;
	case BUTTON_NATION_NOTICE :
		SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(2);  //국가 공지 

		break;
	case BUTTON_NATIONWAR_JOIN_VOTE :
		SendCMD_VOTING_DATA();
		break;
	}
}

//011008 lsw  >
void 	SendCMD_GET_LOADING_POINT(int type)
{
	SMenu[MN_NATION_INFO].nField[7].nWillDo=0;

	//여기 뭐 받아 올거 있으면 받아 오세요 
	if(Hero->NWCharacter.isSquadLeader)
	{
		if(Hero->NWCharacter.SquadNo==g_SquadNum)
		{
			switch(CheckNationStatus(NW_PREPARE))
			{
			case 0: //산중
				/*
				[142.216]-2번공격경로
				[221.202]-1번공격경로
				[251.103]-3번공격경로
				*/
				SMenu[MN_NATION_INFO].nField[0].nImageNumber = 4;
				//1 번 지정
				SMenu[MN_NATION_INFO].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[1].x = 18+221;
				SMenu[MN_NATION_INFO].nField[1].y = 27+202;
				
				//2 번 지정
				SMenu[MN_NATION_INFO].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[2].x = 18+142;
				SMenu[MN_NATION_INFO].nField[2].y = 27+216;
				
				//3 번 지정
				SMenu[MN_NATION_INFO].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[3].x = 18+251;
				SMenu[MN_NATION_INFO].nField[3].y = 27+103;
				
				SMenu[MN_NATION_INFO].nField[4].nType = 0;
				break;
			case 1: //평원
				/*
				[227.200]-1번공격경로
				[225.70]-3번공격경로
				[83.218]-2번공격경로
				*/
				SMenu[MN_NATION_INFO].nField[0].nImageNumber = 6;
				//1 번 지정
				SMenu[MN_NATION_INFO].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[1].x = 18+227;
				SMenu[MN_NATION_INFO].nField[1].y = 27+200;
				
				//2 번 지정
				SMenu[MN_NATION_INFO].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[2].x = 18+83 ;
				SMenu[MN_NATION_INFO].nField[2].y = 27+218;
				
				//3 번 지정
				SMenu[MN_NATION_INFO].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[3].x = 18+225;
				SMenu[MN_NATION_INFO].nField[3].y = 27+70 ;

				SMenu[MN_NATION_INFO].nField[4].nType = 0;

				break;
			case 2: //섬
				/*
				[29.67] - 4번공격경로
				[29.7] - 3번공격경로
				[258.161]-2번공격경로
				[256.228]-1번공격경로
				*/
				SMenu[MN_NATION_INFO].nField[0].nImageNumber = 5;
				//1 번 지정
				SMenu[MN_NATION_INFO].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[1].x = 18+256;
				SMenu[MN_NATION_INFO].nField[1].y = 27+228;
				
				//2 번 지정
				SMenu[MN_NATION_INFO].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[2].x = 18+258;
				SMenu[MN_NATION_INFO].nField[2].y = 27+161;
				
				//3 번 지정
				SMenu[MN_NATION_INFO].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[3].x = 18+29;
				SMenu[MN_NATION_INFO].nField[3].y = 27+7;
				//4 번 지정
				SMenu[MN_NATION_INFO].nField[4].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
				SMenu[MN_NATION_INFO].nField[4].x = 18+29;
				SMenu[MN_NATION_INFO].nField[4].y = 27+67;
				
				break;
			}
			//011010 lsw >
			SetRect(SMenu[MN_NATION_INFO].nField[1].rCheakBox,SMenu[MN_NATION_INFO].nField[1].x - 10 ,SMenu[MN_NATION_INFO].nField[1].y-10,
			SMenu[MN_NATION_INFO].nField[1].x+24,SMenu[MN_NATION_INFO].nField[1].y+40);
			SetRect(SMenu[MN_NATION_INFO].nField[2].rCheakBox,SMenu[MN_NATION_INFO].nField[2].x - 10 ,SMenu[MN_NATION_INFO].nField[2].y-10,
			SMenu[MN_NATION_INFO].nField[2].x+24,SMenu[MN_NATION_INFO].nField[2].y+40);
			SetRect(SMenu[MN_NATION_INFO].nField[3].rCheakBox,SMenu[MN_NATION_INFO].nField[3].x - 10 ,SMenu[MN_NATION_INFO].nField[3].y-10,
			SMenu[MN_NATION_INFO].nField[3].x+24,SMenu[MN_NATION_INFO].nField[3].y+40);
			SetRect(SMenu[MN_NATION_INFO].nField[4].rCheakBox,SMenu[MN_NATION_INFO].nField[4].x - 10 ,SMenu[MN_NATION_INFO].nField[4].y-10,
			SMenu[MN_NATION_INFO].nField[4].x+24,SMenu[MN_NATION_INFO].nField[4].y+40);
			//011010 lsw <
			
			if(g_SquadLoadingPoint) 
			{
				//공격로가 되어 있는 지역
				SMenu[MN_NATION_INFO].nField[7].nWillDo=1;
				SMenu[MN_NATION_INFO].nField[7].x = SMenu[MN_NATION_INFO].nField[g_SquadLoadingPoint].x;
				SMenu[MN_NATION_INFO].nField[7].y = SMenu[MN_NATION_INFO].nField[g_SquadLoadingPoint].y;
			}
	//010927 lsw
	//		sprintf(SMenu[MN_NATION_INFO].nField[6].temp, lan->OutputMessage(0,113),g_SquadLoadingPoint+1); //"부대 공격로를 지정하셨습니다. 한번 정한 공격로는 바꿀 수 없습니다."
			CallSmallMenu(MN_NATION_INFO,MN_NATION_BATTLE_BBS);
		}
		else 
		{
			CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,259), FALSE); //"다른 부대의 공격로를 지정할 수 없습니다."
		}
	}
	else
	{
		//아니라는 메세지
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,231), FALSE); //"[부대장]이 아니면 공격거점을 지정할 수 없습니다."
	}
}
//011008 lsw  <

//-------------------------------------------------------------------------------------
// InterFace Concern
//-------------------------------------------------------------------------------------

void SendCMD_REQUEST_NEW_WARFIELD_STATUS()
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_NEW_WARFIELD_STATUS;
	packet.h.header.size=0;

	QueuePacket(&packet,1);
}

void SendNationPacket(int type)//010730 lsw 국가전 메인인터페이스 (q 누르면나오는거 ) 패킷이 날아가는곳
{
	SendCMD_REQUEST_NEW_WARFIELD_STATUS();

	switch(type)
	{
	case	BUTTON_NATION_STATUS :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_NATION_SQUAD :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_NATION_JOIN :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_HALLOFFAME :
		break;
	case	BUTTON_HALLOFWAR :
		break;
	case	BUTTON_WAR_DESCRIPTION:
		SendCMD_REQUEST_WARFIELD_POSSESSION();
		break;
	//__________Nation War Join Interface
	case	BUTTON_NATIONWAR_JOIN_VOTE :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	//__________Squad Commander Interface
	case	BUTTON_NATION_CHECK_COMMANDER :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_COMMANDER_REG :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_COMMANDER_VOTE :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	//__________Squad Interface
	case	BUTTON_SQUAD_JOIN :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_SQUAD_LODINGPOINT :						// Current Loading Point Set Menu Open
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_JOIN_FREELANCER :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_FREELANCER_SQUAD :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_BY_JOIN :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_ZY_JOIN :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;
	case	BUTTON_NATION_NOTICE :
		SendCMD_REQUEST_WARFIELD_STATUS(type);
		break;

	case	BUTTON_NATION_LOADING_POINT:
		SendCMD_GET_LOADING_POINT(type);
		break;
	}
}


void WarMenuNoticeOpen()	// 011017 LTS
{
	t_packet packet;

	packet.h.header.type=CMD_REQUEST_NOTICE;
	packet.h.header.size=0;
	QueuePacket(&packet,1);
}

LPCHARACTER FindHim(int ServerID)
{
	LPCHARACTER temp = Hero;
	while( temp )
	{
		if( temp->id== ServerID )
		{
			return temp;
		}
		temp = temp->lpNext;
	}
	return NULL;
}

LPCHARACTER FindHimBySprNo(int SprNo)		// LTS DRAGON MODIFY
{
	LPCHARACTER temp = Hero;
	while( temp )
	{
		if( temp->sprno== SprNo)
		{
			return temp;
		}
		temp = temp->lpNext;
	}
	return NULL;
}


// BBS Packet Processing Routine __________________________________________________________
void ProcessBBS_GENERAL(t_packet* p,char* Message)				// 일반적인 방송의 처리 
{
	PlayListAutoSounds(50,0,0,0);
}

void ProcessBBS_PEACE(t_packet* p,char* Message)
{
	PlayListAutoSounds(62,0,0,0);	// 010927 LTS
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,260)); //"한 동안 전쟁이 일어나지 않습니다. 바이서스와 자이펀은 휴전 상태를 유지하게 됩니다."
}

void ProcessBBS_WAR_JOIN_VOTE(t_packet* p,char* Message)
{
	char temp[80],temp2[80];
	int DefenceNation=p->u.NationWar.WarBBS.Arg[0];

	g_pNation->SetStatus(p->u.NationWar.WarBBS.WarfieldNo,NW_VOTE);
	ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo,temp);
	ReturnNationName(DefenceNation,temp2);

	PlayListAutoSounds(62,0,0,0);			// 010927 LTS
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,101),temp2,temp); //"[ %s ]국가의 [ %s ]로 쳐들어 가기 위해 투표를 하고 있습니다. 전쟁관리소 버튼을 눌러 투표에 참여해 주시기 바랍니다." 
}

void ProcessBBS_WAR_PREPARE(t_packet* p,char* Message)
{
	char temp[80],temp2[80],temp3[80];
	int AttackNation=p->u.NationWar.WarBBS.Arg[0];
	int DefenceNation=p->u.NationWar.WarBBS.Arg[1];

	g_pNation->SetStatus(p->u.NationWar.WarBBS.WarfieldNo,NW_PREPARE);
	ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo,temp);
	ReturnNationName(4,temp2);
	ReturnNationName(3,temp3);

	if (Hero->name_status.nation==AttackNation) 
	{
		switch (AttackNation)
		{
		case 3 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,102),temp2,temp); break; //"[ %s ]국가의 [[ %s ]]에 20 분 후에 전쟁을 하게 될 것입니다."
		case 4 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,102),temp3,temp); break; //"[ %s ]국가의 [[ %s ]]에 20 분 후에 전쟁을 하게 될 것입니다."
		}
		
	}
	else if (Hero->name_status.nation==DefenceNation) 
	{
		switch (DefenceNation)
		{
		case 3 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,105),temp3,temp); break; //"적국에서 전쟁 선포를 했습니다. [ %s ]국민은 [ %s ]를 방어 준비를 하시기 바랍니다."
		case 4 : AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,105),temp2,temp); break; //"적국에서 전쟁 선포를 했습니다. [ %s ]국민은 [ %s ]를 방어 준비를 하시기 바랍니다."
		}
	}
	else 
	{
		switch (AttackNation)
		{
		case 3 :
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,106),temp2,temp3,temp); //"[ %s ]국가가 [ %s ]국가에 전쟁 선포를 했습니다. 20분 후에 전쟁이 벌어질 지역은 [[ %s ]]입니다."
			break;
		case 4 :
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,106),temp3,temp2,temp); //"[ %s ]국가가 [ %s ]국가에 전쟁 선포를 했습니다. 20분 후에 전쟁이 벌어질 지역은 [[ %s ]]입니다."
			break;
		}
	
	}
	PlayListAutoSounds(271,0,0,0);
}

//011008 lsw > //011011 lsw >
void ProcessBBS_WAR_BEGIN(t_packet* p,char* Message)
{
	char tempZy[80],tempBy[80],tempField[80],tempDefNa[80],tempAttNa[80];
	//여기서 받은 것을 분석 하여 
	int iLoadingPonint;
	char SquadLoadingPoint[NW_SQUAD_KIND][NW_SQUAD_MAX];
	int DefenceNation=p->u.NationWar.WarBBS.Arg[0];
	int WarfieldCode=p->u.NationWar.WarBBS.WarfieldNo;

	g_pNation->SetStatus(WarfieldCode,NW_WAR);


	SMenu[MN_NATION_WAR_START].nField[1].nWillDo  = 0;
	SMenu[MN_NATION_WAR_START].nField[2].nWillDo  = 0;
	SMenu[MN_NATION_WAR_START].nField[3].nWillDo  = 0;
	SMenu[MN_NATION_WAR_START].nField[4].nWillDo  = 0;

	memcpy(SquadLoadingPoint,&p->u.NationWar.WarBBS.Arg[1],p->u.NationWar.WarBBS.Size-1);

	if (isAttacker()) {iLoadingPonint=SquadLoadingPoint[NW_ATTACKER][Hero->NWCharacter.SquadNo];}//여기다가 넣으면 됩니다 
	else {iLoadingPonint=SquadLoadingPoint[NW_DEFENCER][Hero->NWCharacter.SquadNo];}//여기다가 넣으면 됩니다	// 일스이면 문제가 있다.
	
	//011010 lsw >
	if(iLoadingPonint < 1 || iLoadingPonint > 4) {iLoadingPonint =1; }
	//011010 lsw <

	ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo,tempField);
	ReturnNationName(NW_ZY,tempZy);
	ReturnNationName(NW_BY,tempBy);
	ReturnNationName(DefenceNation,tempDefNa);
	
	if( DefenceNation ==NW_ZY)
	{
		ReturnNationName(NW_BY,tempAttNa);
	}
	else 
	{
		ReturnNationName(NW_ZY,tempAttNa);
	}

	if(//방어자
		(
			Hero->name_status.nation==NW_YL && 
			Hero->NWCharacter.YL_JoinNation==DefenceNation)
		||	Hero->name_status.nation==DefenceNation) //방어국
	{
		iLoadingPonint = 7;
	}

	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,261),tempDefNa,tempField); //"[ %s ]의 [ %s ]에서 전쟁이 일어났습니다. 모든 국민은 전쟁에 참여해 주시기 바랍니다."

	switch(	WarfieldCode)
	{
		case 0: //산중
			/*
			[142.216]-2번공격경로
			[221.202]-1번공격경로
			[251.103]-3번공격경로
			[94.67]-방어
			*/
			SMenu[MN_NATION_WAR_START].nField[0].nImageNumber = 4;
			//1 번 지정
			SMenu[MN_NATION_WAR_START].nField[1].x = 18+221;
			SMenu[MN_NATION_WAR_START].nField[1].y = 27+202;
			//2 번 지정
			SMenu[MN_NATION_WAR_START].nField[2].x = 18+142;
			SMenu[MN_NATION_WAR_START].nField[2].y = 27+216;
			//3 번 지정
			SMenu[MN_NATION_WAR_START].nField[3].x = 18+251;
			SMenu[MN_NATION_WAR_START].nField[3].y = 27+103;

			SMenu[MN_NATION_WAR_START].nField[7].x = 18+94;
			SMenu[MN_NATION_WAR_START].nField[7].y = 27+67;
			
			break;
		case 1: //평원
			/*
			[227.200]-1번공격경로
			[225.70]-3번공격경로
			[83.218]-2번공격경로
			[64.77]-방어
			*/
			SMenu[MN_NATION_WAR_START].nField[0].nImageNumber = 6;
			//1 번 지정
			SMenu[MN_NATION_WAR_START].nField[1].x = 18+227;
			SMenu[MN_NATION_WAR_START].nField[1].y = 27+200;
			//2 번 지정
			SMenu[MN_NATION_WAR_START].nField[2].x = 18+225;
			SMenu[MN_NATION_WAR_START].nField[2].y = 27+70;
			//3 번 지정
			SMenu[MN_NATION_WAR_START].nField[3].x = 18+83 ;
			SMenu[MN_NATION_WAR_START].nField[3].y = 27+218;
			
			SMenu[MN_NATION_WAR_START].nField[7].x = 18+64;
			SMenu[MN_NATION_WAR_START].nField[7].y = 27+77;

			break;
		case 2: //섬
			/*
			[29.67] - 4번공격경로
			[29.7] - 3번공격경로
			[258.161]-2번공격경로
			[256.228]-1번공격경로
			[143.173]-방어
			*/
			SMenu[MN_NATION_WAR_START].nField[0].nImageNumber = 5;
			//1 번 지정
			SMenu[MN_NATION_WAR_START].nField[1].x = 18+256;
			SMenu[MN_NATION_WAR_START].nField[1].y = 27+228;
			//2 번 지정
			SMenu[MN_NATION_WAR_START].nField[2].x = 18+258;
			SMenu[MN_NATION_WAR_START].nField[2].y = 27+161;
			//3 번 지정
			SMenu[MN_NATION_WAR_START].nField[3].x = 18+29;
			SMenu[MN_NATION_WAR_START].nField[3].y = 27+7;

			SMenu[MN_NATION_WAR_START].nField[4].x = 18+29;
			SMenu[MN_NATION_WAR_START].nField[4].y = 27+67;
			
			SMenu[MN_NATION_WAR_START].nField[7].x = 18+143;
			SMenu[MN_NATION_WAR_START].nField[7].y = 27+173;

			break;
	}//0 산중 1 평원 2 섬
	
	SMenu[MN_NATION_WAR_START].nField[iLoadingPonint].nWillDo = 1;//공격로 지정 //클릭 불가

	if (Hero->name_status.nation==NW_YL)
	{
		if (Hero->NWCharacter.YL_JoinNation==DefenceNation)	//방어자
		{
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,114),tempDefNa,tempField); //"[ %s ]국가의 [ %s ]에서 전쟁이 시작됐습니다. 모든 국민들은 전쟁에 참여해 주시기 바랍니다."
			sprintf(SMenu[MN_NATION_WAR_START].nField[5].temp, lan->OutputMessage(0,147),tempAttNa,tempField);//방어자꺼 지정 " [ %s ]국가에서 [ %s ]를 빼앗기 위해 전쟁을 일으켰습니다. \n 적국의 공격으로부터 성을 지키기 위해서는 성안에 있는 수호석을 부서지지 않도록 지켜야 합니다. \n 전쟁터에서 유령인 경우에 [[[ /부활 ]]]하시면 자동으로 살아나게 됩니다."
			CallSmallMenu(MN_NATION_WAR_START);// 인자가 있어야 함..
		}
		else
		{
			//010927 lsw
			sprintf(SMenu[MN_NATION_WAR_START].nField[5].temp, lan->OutputMessage(0,148),tempDefNa,tempField);//공격자꺼 지정 " [ %s ]국가의 [ %s ]를 차지하기 위한 전쟁이 시작됐습니다. \n 전쟁터를 차지하기 위해서는 성안에 있는 수호석 4개를 부순 후에 내성 안에 있는 최종 수호석을 부숴야만 합니다.\n 전쟁터에서 유령인 경우에 [[[ /부활 ]]]하시면 자동으로 살아나게 됩니다."
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,115),tempDefNa,tempField); //"[ %s ]국가의 [ %s ]로 총공격을 시작했습니다."
		}
	}
	
	if (Hero->name_status.nation==DefenceNation) //방어국
	{
		sprintf(SMenu[MN_NATION_WAR_START].nField[5].temp, lan->OutputMessage(0,147),tempAttNa,tempField);//방어자꺼 지정 " [ %s ]국가에서 [ %s ]를 빼앗기 위해 전쟁을 일으켰습니다. \n 적국의 공격으로부터 성을 지키기 위해서는 성안에 있는 수호석을 부서지지 않도록 지켜야 합니다. \n 전쟁터에서 유령인 경우에 [[[ /부활 ]]]하시면 자동으로 살아나게 됩니다."
		AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,114),tempDefNa,tempField); //"[ %s ]국가의 [ %s ]에서 전쟁이 시작됐습니다. 모든 국민들은 전쟁에 참여해 주시기 바랍니다."
		CallSmallMenu(MN_NATION_WAR_START);	// 이때도 다르다 
	}
	else //공격국
	{
		sprintf(SMenu[MN_NATION_WAR_START].nField[5].temp, lan->OutputMessage(0,148),tempDefNa,tempField);//공격자꺼 지정 " [ %s ]국가의 [ %s ]를 차지하기 위한 전쟁이 시작됐습니다. \n 전쟁터를 차지하기 위해서는 성안에 있는 수호석 4개를 부순 후에 내성 안에 있는 최종 수호석을 부숴야만 합니다.\n 전쟁터에서 유령인 경우에 [[[ /부활 ]]]하시면 자동으로 살아나게 됩니다."
		AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,115),tempDefNa,tempField); //"[ %s ]국가의 [ %s ]로 총공격을 시작했습니다."
		CallSmallMenu(MN_NATION_WAR_START);
	}
	
}
//011011 lsw <
//011008 lsw <

void ProcessBBS_COMMANDER_ELECT(t_packet* p,char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp,p->u.NationWar.WarBBS.Arg,p->u.NationWar.WarBBS.Size);
	*(temp+p->u.NationWar.WarBBS.Size)='\0';

	LPCHARACTER ch=ExistHe(temp);
	if (ch!=NULL) InsertMagic(ch,ch,452,9,0,0,ch->x,ch->y);
	if (temp!=NULL)	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,109),temp); //"이번 전쟁의 사령관으로 --==[ %s ]==--이 선출됐습니다."
}

void ProcessBBS_SQUAD_LEADER_ELECT(t_packet* p,char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp,p->u.NationWar.WarBBS.Arg,p->u.NationWar.WarBBS.Size);
	*(temp+p->u.NationWar.WarBBS.Size)='\0';
	
	LPCHARACTER ch=ExistHe(temp);
	if (ch!=NULL) InsertMagic(ch,ch,452,9,0,0,ch->x,ch->y);
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,196),temp); //"< %s >이 부대장이 됐습니다. 이제부터 부대장의 임무를 하시게 됩니다."
}

void ProcessBBS_GUARDSTONE_BROKED(t_packet* p,char* Message)	// 011022 LTS 함수 교체 
{
	int ServerID;
	WORD Index,Locationx,Locationy;				
	
	memcpy(&ServerID,&p->u.NationWar.WarBBS.Arg[1],sizeof(int));
	memcpy(&Locationx,&p->u.NationWar.WarBBS.Arg[5],sizeof(WORD));
	memcpy(&Locationy,&p->u.NationWar.WarBBS.Arg[7],sizeof(WORD));
	memcpy(&Index,&p->u.NationWar.WarBBS.Arg[9],sizeof(WORD));
	
	LPCHARACTER temp=FindHim(ServerID+10000);
	if (temp!=NULL)	
	{
		temp->GuardStoneAni=1;		// h_DisPlayNPC()에서 에니메이션을 처리하기 위한 데이터 
		temp->SetAni2Zero=1;
		temp->sealstoneani=0;
	}
	PlayListAutoSounds(260,0,0,0);
	//	else PlayListAutoSounds(261,0,0,0);
	g_GameInfo.displayOption = DISPLAY_EARTHQUAKE;  g_GameInfo.displayOptionDelay = 40;	// 지진 처리부분..
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,262),Index,Locationx,Locationy); //"전쟁터의 [ %d번 ] 수호석이 공격을 받아 부서졌습니다."
}

void ProcessBBS_CASTLEGATEWAY_BROKED(t_packet* p,char* Message)		// 011022 LTS 함수교체 
{
	int ServerID;
	WORD Index,Locationx,Locationy;				
	
	memcpy(&ServerID,&p->u.NationWar.WarBBS.Arg[1],sizeof(int));
	memcpy(&Locationx,&p->u.NationWar.WarBBS.Arg[5],sizeof(WORD));
	memcpy(&Locationy,&p->u.NationWar.WarBBS.Arg[7],sizeof(WORD));
	memcpy(&Index,&p->u.NationWar.WarBBS.Arg[9],sizeof(WORD));

	PlayListAutoSounds(262,0,0,0);
	LPCHARACTER temp=FindHim(ServerID+10000);
	if (temp!=NULL)	
	{
		InsertMagic(temp,temp,453,4,0,0,temp->x,temp->y);
		temp->GuardStoneAni=1;		// h_DisPlayNPC()에서 에니메이션을 처리하기 위한 데이터 
		temp->SetAni2Zero=1;
		temp->sealstoneani=0;
	}
	g_GameInfo.displayOption = DISPLAY_EARTHQUAKE;  g_GameInfo.displayOptionDelay = 40;	// KR-LAN바뀜
	AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,263),Index,Locationx,Locationy); //"전쟁터의 [ %d번 ] 성문이 공격을 받아 부서졌습니다. 성문이 부서져 성 안으로 침입이 가능하게 됩니다."
}

void ProcessBBS_COMMANDER_DEATH(t_packet* p,char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp,p->u.NationWar.WarBBS.Arg,p->u.NationWar.WarBBS.Size);
	*(temp+p->u.NationWar.WarBBS.Size)='\0';

	switch (p->u.NationWar.WarBBS.LanNo)
	{
		case 0 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,264),temp); break; //"[공격국가] 사령관 [ %s ]님이 사망하였습니다."
		case 1 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,265),temp); break; //"[공격국가] 부대장 [ %s ]님이 사망하였습니다."
		case 2 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,266),temp); break; //"[방어국가] 사령관 [ %s ]님이 사망하였습니다."
		case 3 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,267),temp); break; //"[방어국가] 부대장 [ %s ]님이 사망하였습니다."
	}
	
	switch(p->u.NationWar.WarBBS.LanNo)
	{
		case 0 :
		case 1 :
			if (isAttacker()) PlayListAutoSounds(259,0,0,0);
			else PlayListAutoSounds(51,0,0,0);
			break;
		case 2 :
		case 3 :
			if (isAttacker()) PlayListAutoSounds(51,0,0,0);
			else PlayListAutoSounds(259,0,0,0);
			break;
	}
}

void ProcessBBS_COMMANDER_CHAT(t_packet* p,char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp,p->u.NationWar.WarBBS.Arg,p->u.NationWar.WarBBS.Size);
	*(temp+p->u.NationWar.WarBBS.Size)='\0';

	AddCurrentStatusMessage( 255,255,0, "%s",temp);
	PlayListAutoSounds(258,0,0,0);
}

void ProcessBBS_WAR_VICTORY(t_packet* p,char* Message)
{
	int VictoryNation=p->u.NationWar.WarBBS.Arg[0];
	NationSupportRecvNationWarResult(p);
	int WarfieldNo=g_pNation->GetStatus(NW_WAR);
	g_pNation->SetStatus(WarfieldNo,NW_PEACE);
	if (Hero->name_status.nation==NW_YL)
	{
		if (Hero->NWCharacter.YL_JoinNation==VictoryNation) 
		{
			PlayListAutoSounds(273,0,0,0);	// 승리사운드 
		}
		else 
		{
			PlayListAutoSounds(274,0,0,0);	// 패배 사운드 
		}
		return;
	}
	if (Hero->name_status.nation==VictoryNation) 
	{
		PlayListAutoSounds(273,0,0,0);
	}
	else 
	{
		PlayListAutoSounds(274,0,0,0);
	}
}

void ProcessBBS_BONUS_OPEN(t_packet* p,char* Message)
{
	char temp[80];
	ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo,temp);
	PlayListAutoSounds(62,0,0,0);		// 010927 LTS
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,192),temp); //"< %s > 수호석의 힘이 약해져 비밀지역이 열렸습니다."
}

void ProcessBBS_KILLER(t_packet* p,char* Message)
{
	char temp[MAX_STRING_PK];

	memcpy(temp,p->u.NationWar.WarBBS.Arg,p->u.NationWar.WarBBS.Size);
	*(temp+p->u.NationWar.WarBBS.Size)='\0';
	
	LPCHARACTER ch=ExistHe(temp);
	if (ch!=NULL) InsertMagic(ch,ch,451,9,0,0,ch->x,ch->y);
}

void ProcessBBS_NATION_MONEY(t_packet* p,char* Message)		// 010927 LTS	// 011022 LTS 함수 교체 
{
	char temp[80],WarfieldName[80];	// 011022 LTS
	char Nation=p->u.NationWar.WarBBS.Arg[0];
	char WarfieldNo=p->u.NationWar.WarBBS.Arg[1];	// 011022 LTS
	int Size=p->u.NationWar.WarBBS.Size-1;

	ReturnWarfieldName(WarfieldNo,WarfieldName);

	memcpy(temp,&p->u.NationWar.WarBBS.Arg[2],Size);	// 011022 LTS
	*(temp+Size)='\0';


		if (Hero->name_status.nation==Nation)	//패배국 이면
		{
			if (p->u.NationWar.WarBBS.BBSType==BBS_NATION_MONEY_USE)	// 돈을 썼다
			{
				switch (p->u.NationWar.WarBBS.LanNo)
				{
				case 0 : 
					{
						char temp2[80];
						ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo,temp2);
						AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,268),temp2,temp); //"전쟁의 패배로 [ %s ]는 적국의 소유가 되었고, 적국으로 부터 < %s Crit >을 빼앗겼습니다."
						break;	// 011020 LTS
					}
				case 1 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,269),temp); break; //"사령관이 아이템 구입으로 [ %s Crit]의 금액을 소모하였습니다."
				case 2 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,270),temp); break; //"사령관이 무기 구입으로 [ %s Crit]의 금액을 소모하였습니다."
				}
			}
			else	// 돈을 쓰지 못했다
			{
				switch (p->u.NationWar.WarBBS.LanNo)
				{
					case 0 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,281),WarfieldName); break; //"전쟁의 패배로 [ %s ]는 적국의 소유가 됐으며 현재 자국가의 국고가 비어있는 상태입니다."
					case 1 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,271),temp); break; //"사령관이 아이템 구입으로 [ %s Crit]의 금액을 소모하여야 하지만 국고가 부족합니다."
					case 2 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,272),temp); break; //"사령관이 무기 구입으로 [ %s Crit]의 금액을 소모하여야 하지만 국고가 부족합니다."
				}
			}
		}
		else	//승리국
		{
			if (Hero->name_status.nation!=NW_YL)
			{
				if (p->u.NationWar.WarBBS.BBSType==BBS_NATION_MONEY_USE)	// 돈을 썼다
				{
					switch (p->u.NationWar.WarBBS.LanNo)
					{
					case 0 :
						{
						char temp2[80];		//금액이 %s로 되어 있다. -> 나중에 %d로 바꾼다. // 버그가 나왔다.
						ReturnWarfieldName(p->u.NationWar.WarBBS.WarfieldNo,temp2);
						AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,273),temp2,temp); //"전쟁의 승리로 [ %s ]는 자국의 소유가 되었고, 적국으로 부터 < %s Crit >을 빼았았습니다."
						break;
						}
					case 1 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,269),temp); break; //"사령관이 아이템 구입으로 [ %s Crit]의 금액을 소모하였습니다."
					case 2 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,270),temp); break; //"사령관이 무기 구입으로 [ %s Crit]의 금액을 소모하였습니다."
					}
				}
				else				// 쓰지 못했다
				{
					switch (p->u.NationWar.WarBBS.LanNo)
					{
					case 0 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,280),WarfieldName); break; // "전쟁의 승리로 [ %s ]는 자국의 소유가 됐지만 적국의 국고가 텅 비어있어 국고를 빼앗지 못했습니다."
					case 1 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,271),temp); break; //"사령관이 아이템 구입으로 [ %s Crit]의 금액을 소모하여야 하지만 국고가 부족합니다."
					case 2 : AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,272),temp); break; //"사령관이 무기 구입으로 [ %s Crit]의 금액을 소모하여야 하지만 국고가 부족합니다."
					}
				}
			}
		}
}

// BBS Packet Processing Routine End_______________________________________________________


//010924 lsw
void RecvCMD_WAR_BBS(t_packet* p)						// 전쟁 BBS관련 모든것을 처리한다.
{
	char msg[500];
	int Type=0;
	//sprintf(msg,lan->OutputMessage(0,300)); //"0정보없음         지금은 더이상 들어온 정보가 없습니다."

	TodayNoticeButtonIntoColor(2);
	
		
	switch (p->u.NationWar.WarBBS.BBSType)
	{
		case BBS_GENERAL				:	ProcessBBS_GENERAL(p,msg);					break;
		case BBS_PEACE					:	ProcessBBS_PEACE(p,msg);					break;
		case BBS_WAR_JOIN_VOTE			:	ProcessBBS_WAR_JOIN_VOTE(p,msg);			break;
		case BBS_WAR_PREPARE			:	ProcessBBS_WAR_PREPARE(p,msg);				break;
		case BBS_WAR_BEGIN				:	ProcessBBS_WAR_BEGIN(p,msg);				break;
		case BBS_COMMANDER_ELECT		:	ProcessBBS_COMMANDER_ELECT(p,msg);			break;
		case BBS_SQUADLEADER_ELECT		:	ProcessBBS_SQUAD_LEADER_ELECT(p,msg);		break;
		case BBS_GUARDSTONE_BROKED		:	ProcessBBS_GUARDSTONE_BROKED(p,msg);		break;
		case BBS_CASTLEGATEWAY_BROKED	:	ProcessBBS_CASTLEGATEWAY_BROKED(p,msg);		break;
		case BBS_COMMANDER_DEATH		:	ProcessBBS_COMMANDER_DEATH(p,msg);			break;
		case BBS_COMMANDER_CHAT			:	ProcessBBS_COMMANDER_CHAT(p,msg);			break;
//		case BBS_WAR_DEFEAT :
		case BBS_WAR_VICTORY			:	ProcessBBS_WAR_VICTORY(p,msg);				break;
		case BBS_BONUS_OPEN				:	ProcessBBS_BONUS_OPEN(p,msg);				break;
		case BBS_KILLER					:	ProcessBBS_KILLER(p,msg);					break;
		case BBS_NATION_MONEY_USE		:
		case BBS_NATION_MONEY_NOT_USE	:	ProcessBBS_NATION_MONEY(p,msg);				break;
	}
	//IntoNationNotice(msg);
	
	return;
}

void SendCMD_REQUEST_NATION_WAR_COUNT()
{
	t_packet p;

	p.h.header.type=CMD_REQUEST_NATION_WAR_COUNT;
	p.h.header.size=0;
	QueuePacket( &p, 1 );
}

void RecvCMD_ANSWER_NATION_WAR_COUNT(t_packet *p)
{
	Hero->NWCharacter.WarNo=p->u.NationWar.CommonDataDw.Data;
}

void SendCMD_NW_MAP_MOVE(int Type,int WarfieldNo)	// 011022 LTS		// type : 0 : old, 1:New
{
	t_packet p;

	//< LTH-040226-KO 1.4 몬스터 전쟁은 일스도 조건이 된다면 전쟁에 단독 참여 가능
	if (!IsNeoWarfield(WarfieldNo))
	{
		if (Hero->name_status.nation==NW_YL)
		{
			if (Hero->NWCharacter.YL_JoinNation==0)
			{
				CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,274), FALSE); //"전쟁터로 이동할 수 있는 상황이 아닙니다."
				return;
			}
		}
	}
	//> LTH-040226-KO

	if (Hero->lv<=10) // 011022 LTS
	{
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,282), FALSE); 
		return;
	}

	SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(0);

	p.h.header.type=CMD_NW_MAP_MOVE;
	if (Type==1)
		p.u.NationWar.NWMapMove.WarfieldNo=WarfieldNo;		// LTH-040226-KO 1.4 이후에도 고칠필요 없다. 전쟁번호 4를 보내온다
	else
		p.u.NationWar.NWMapMove.WarfieldNo=WarfieldNo-1;
	p.u.NationWar.NWMapMove.SquadNo=Hero->NWCharacter.SquadNo;
	p.h.header.size=sizeof(t_NWMapMove);
	QueuePacket( &p, 1 );
}

void RecvCMD_NW_MAP_MOVE_FAIL(t_packet *p)			// 011213 LTS
{
	switch (p->u.NationWar.CommonDataC.Data)
	{
	case 1 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,329), FALSE); break;
	case 2 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,330), FALSE); break;
	case 3 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,331), FALSE); break;	
	case 4 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,332), FALSE); break;	
	case 5 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,288), FALSE); break;	
	case 6 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,616), FALSE); break;
	case 7 : CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,626), FALSE); break;	// "전쟁이 시작된지 30분이 지나 이동할 수 없습니다"
	}
}

void RecvCMD_ANSWER_TILE_DONT(t_packet* p)
{
	g_pNation->TileDont(p->u.NationWar.TileDont.WarfieldNo,
		p->u.NationWar.TileDont.TypeNumber,
		p->u.NationWar.TileDont.Value);
}

void RecvCMD_TILE_DONT_CHANGED(t_packet* p)
{
	g_pNation->TileDont(p->u.NationWar.TileDont.WarfieldNo,
		p->u.NationWar.TileDont.TypeNumber,
		p->u.NationWar.TileDont.Value);
}

void RecvCMD_CLEAR_SQUAD_DATA(t_packet *p)			// 011020 LTS
{
	if (isNationWarfieldServer())
	{
		if (Hero->name_status.nation!=NW_YL) memset(&Hero->NWCharacter,0,sizeof(DWORD));
	}
	else memset(&Hero->NWCharacter,0,sizeof(DWORD));		// 010915 LTS
}

void RecvCMD_ANSWER_WARFIELD_INFO(t_packet* p)
{
	g_AttackNation=p->u.NationWar.WarfieldInfo.AttackWarfieldNo;
	g_DefenceNation=p->u.NationWar.WarfieldInfo.DefenceWarfieldNo;
}


void RecvCMD_ANSWER_CHECK_WARFIELD_POSSESSION(t_packet* p)
{
	bool AllPossession=true;
	int WarfieldNo;

	switch (p->u.NationWar.WarfieldPossession.RequestType)
	{
	case 1 :	//일스 부대편성
		{
		int PossessionNation=p->u.NationWar.WarfieldPossession.Possession[0];
		if (PossessionNation!=p->u.NationWar.WarfieldPossession.Possession[1]) AllPossession=false;
		if (PossessionNation!=p->u.NationWar.WarfieldPossession.Possession[2]) AllPossession=false;
		if (AllPossession)
		{
			WarfieldNo=CheckNationStatus(NW_PREPARE);
			if (WarfieldNo>=0)
			{
				char msg[80];
				ReturnWarfieldName(WarfieldNo,msg);
				AddCurrentStatusMessage( 255,255,0, lan->OutputMessage(0,247),msg); //"[ %s ]의 전쟁에 참여 할 부대를 모집하고 있습니다."
				SendCMD_REQUEST_SQUAD_INFO(g_Squad);			// 인터페이스의 화살표에 의해 인자가 결정된다.. 맨처음은 0이다.	
			}
			else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,249), FALSE); //"[전쟁준비중] 기간에만 사용할 수 있습니다."
		}
		else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,256), FALSE); //"용병부대를 편성할 수 있는 상황이 아닙니다."
		break;
		}
	case 3 :	// 사운드 플레이 
		{
			if (Hero->name_status.nation==NW_YL)
			{
				if (Hero->NWCharacter.YL_JoinNation==
					p->u.NationWar.WarfieldPossession.Possession[MapInfo[MapNumber].port-BASE_WARFIELD_PORT])
				{
					PlayListAutoSounds(7,0,0,1);	//방어국가 사운드
				}
				else
				{
					PlayListAutoSounds(8,0,0,1);	//공격국가 사운드
				}
				return;
			}
			if (Hero->name_status.nation==
				p->u.NationWar.WarfieldPossession.Possession[MapInfo[MapNumber].port-BASE_WARFIELD_PORT])
			{
				PlayListAutoSounds(7,0,0,1);	//방어국가 사운드
			}
			else
			{
				PlayListAutoSounds(8,0,0,1);	//공격국가 사운드
			}
			break;
		}

	case 4 :	// 바이조인
		{
			int PossessionNation=p->u.NationWar.WarfieldPossession.Possession[0];
			if (PossessionNation!=p->u.NationWar.WarfieldPossession.Possession[1]) AllPossession=false;
			if (PossessionNation!=p->u.NationWar.WarfieldPossession.Possession[2]) AllPossession=false;
			if (!AllPossession)
			{
				if (Hero->NWCharacter.YL_JoinNation==0) SendCMD_FREELANCER_SQUAD_JOIN(NW_BY);  // define
				else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,257), FALSE); //"이미 용병으로 지원할 국가를 선택했습니다."
			}
			else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,283), FALSE); //"용병부대를 편성할 수 있는 상황이 아닙니다."	// 011025 LTS
			break;
		}
	case 5 :	// 자이조인 
		{
			int PossessionNation=p->u.NationWar.WarfieldPossession.Possession[0];
			if (PossessionNation!=p->u.NationWar.WarfieldPossession.Possession[1]) AllPossession=false;
			if (PossessionNation!=p->u.NationWar.WarfieldPossession.Possession[2]) AllPossession=false;
			if (!AllPossession)
			{
				if (Hero->NWCharacter.YL_JoinNation==0) SendCMD_FREELANCER_SQUAD_JOIN(NW_ZY);
				else CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,257), FALSE); //"이미 용병으로 지원할 국가를 선택했습니다."
			}
			else CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,283), FALSE); //"용병부대를 편성할 수 있는 상황이 아닙니다."	// 011025 LTS
			break;
		}
	}
}

void RecvCMD_ANSWER_WARFIELD_POSSESSION(t_packet *p)
{
	// 여기서 설명창을 띄운다
	int Possession1=p->u.NationWar.WarfieldPossession.Possession[0];
	int Possession2=p->u.NationWar.WarfieldPossession.Possession[1];
	int Possession3=p->u.NationWar.WarfieldPossession.Possession[2];

	g_pNation->SetWarfieldPoss(0,Possession1);
	g_pNation->SetWarfieldPoss(1,Possession2);
	g_pNation->SetWarfieldPoss(2,Possession3);
//소유자. 
//	AddCurrentStatusMessage( 255,255,0, "평원전쟁터 : %d, 산중전쟁터 : %d , 섬전쟁터 : %d",Possession1,Possession2,Possession3);
	sprintf(SMenu[MN_NATION_DONEBOX].nField[1].temp, lan->OutputMessage(0,149),NationName[Possession1],NationName[Possession2],NationName[Possession3]); //" <<<<<<  현재 전쟁터 소유 국가 >>>>>> \n  <<산중 전쟁터>> : [ %s ] \n  <<평원 전쟁터>> : [ %s ] \n  << 섬  전쟁터>> : [ %s ] \n 전쟁 진행 방법 : 공격 국가의 전쟁 참여 투표에서 참여 투표가 높으면 양 국가에 전쟁 선포가 되며, 20분 후에 전쟁이 시작됩니다. \n 전쟁 참여 방법 : 전쟁 선포 후에 바이서스와 자이펀 부대 편성을 통해 전쟁에 참여하게 되며, 일스는 용병으로서 전쟁 참여가 가능합니다."

	CallSmallMenu(MN_NATION_DONEBOX,MN_NATION_BATTLE_BBS);
}

void RecvCMD_WARFIELD_WAR_DATA1(t_packet *p)
{
	//전쟁터 상황
	char  GuardStoneSize=p->u.data[0];
	char  CastleSize=p->u.data[GuardStoneSize+1];
	char* GuardStoneStatus=new char[GuardStoneSize];
	char* CastleStatus=new char[CastleSize];

	memcpy(GuardStoneStatus,(p->u.data+1),GuardStoneSize);
	memcpy(CastleStatus,(p->u.data+GuardStoneSize+2),CastleSize);
	NationSupportNationNow1(p);//010910 lsw

	SAFE_DELETE(GuardStoneStatus);
	SAFE_DELETE(CastleStatus);
}
void RecvCMD_WARFIELD_WAR_DATA2(t_packet* p)
{
	DWORD RemainTime=p->u.NationWar.WarfieldData2.RemainTime;
	WORD  Hour, Min, Sec;	// 011020 LTS
	GetTime(RemainTime,Hour,Min,Sec);
	NationSupportNationNow2(p);//010910 lsw
}

void RecvCMD_WARFIELD_WAR_DATA3(t_packet* p)
{
	NationSupportNationNow3(p);//010910 lsw
}

void RecvCMD_CLEAR_WAR_CHARACTER_DATA(t_packet *p)
{
	memset(&Hero->NWCharacter,0,sizeof(DWORD));
	Hero->NWCharacter.WarNo=p->u.NationWar.CommonDataC.Data;
}

void RecvCMD_YOU_ARE_SQUAD_COMMANDER(t_packet *p)
{
	Hero->NWCharacter.isCommander=1;
	if (Hero->name_status.nation==NW_YL) Hero->NWCharacter.YL_JoinNation=p->u.NationWar.CommonDataC.Data;
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,276)); //"[사령관]에 당선됐습니다. 전쟁에서 사령관의 임무를 수행하게 됩니다."
	InsertMagic(Hero,Hero,451,9,0,0,Hero->x,Hero->y);
}


void SendRequestTileDontData()								// 어떤 맵에나 처음 들어오면 패킷을 보내는 곳이다.
{
	if (isNationWarfieldServer())
	{
		t_packet packet;
		packet.h.header.type=CMD_REQUEST_TILE_DONT;
		packet.h.header.size=0;
		QueuePacket(&packet, 1 );
		SendCMD_REQUEST_CHECK_WARFIELD_POSSESSION(3);		// 사운드를 플레이 한다.
		SendCMD_REQUEST_WARFIELD_INFO();					// 공격국과 방어국을 확인 하기 위해서 보낸다
	}

	SendNationPacket(BUTTON_NATION_NOTICE);					// 데이터 갱신을 위해 

	//< LTH-040302-KO 1.4 Patch Snowy Warfield same packet
	if (isNewWarfieldServer() || IsNeoWarfieldServer())
	{
		t_packet packet;
		packet.h.header.type=CMD_REQUEST_NEW_WARFIELD_TILE_DONT;
		packet.h.header.size=0;
		QueuePacket(&packet, 1 );
		PlayListAutoSounds(7,0,0,1);
	}
	//> LTH-040302-KO

	//< LTH-040302-KO 1.4 Patch Add!
	if (!isNationWarfieldServer()&&!isNewWarfieldServer()&&!IsNeoWarfieldServer())
	{
		FreeAutoSounds(7);	// 국가전 맵이 아니면 삭제한다.
		FreeAutoSounds(8);
	}
	//> LTH-040302-KO
}

void RecvCMD_SQUAD_JOIN_RESULT(t_packet *p)
{
	switch (p->u.NationWar.CommonDataC.Data)
	{
	case 0 : CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,253), FALSE); break; //"부대에 등록이 안 됐습니다."
	case 1 : CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,277), FALSE); break; //"[부대]에 소속됐습니다."
	case 2 : CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,278), FALSE); break; //"[부대장]이 되기 위한 [능력치]가 부족합니다."
	case 3 : CallOkCancelMessageBox(TRUE, 0,0, lan->OutputMessage(0,279), FALSE); break; //"부대원 수가 모두 찼기 때문에 부대에 등록하실 수 없습니다."
	}
	
}
void ProcessNoticeJoinA(t_packet *p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	ReturnWarfieldName(p->u.NationWar.NoticeJoinVoteA.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticeJoinVoteA.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,301),WarfieldName,p->u.NationWar.NoticeJoinVoteA.VoteCount,Hour,Min);
	IntoNationNotice(ALL_CLEAR,1,NULL);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessNoticeJoinD(t_packet *p)
{
	char tempMessage[500];
	char NationName[20];
	ReturnNationName(p->u.NationWar.NoticeJoinVoteD.Nation,NationName);
	sprintf(tempMessage,lan->OutputMessage(0,302),NationName);
	IntoNationNotice(ALL_CLEAR,1,NULL);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessNoticeJoinReinforce(t_packet *p)
{
	char tempMessage[500];
	char NationName[20];
	ReturnNationName(p->u.NationWar.NoticeJoinVoteD.Nation,NationName);
	sprintf(tempMessage,lan->OutputMessage(0,303),NationName);
	IntoNationNotice(ALL_CLEAR,1,NULL);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare1(t_packet *p)
{
	char tempMessage[500];
	IntoNationNotice(ALL_CLEAR,1,NULL);
	sprintf(tempMessage,lan->OutputMessage(0,304),p->u.NationWar.NoticePrepare1.LeadCandidaterName);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	char WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	ReturnWarfieldName(p->u.NationWar.NoticePrepare1.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare1.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,305),WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare1Reinforce(t_packet *p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	char AttackNationName[20],DefenceNationName[20];
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnNationName(p->u.NationWar.NoticePrepare1Reinforce.AttackNation,AttackNationName);
	ReturnNationName(p->u.NationWar.NoticePrepare1Reinforce.DefenceNation,DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare1Reinforce.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare1Reinforce.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,306),AttackNationName,DefenceNationName,WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare2(t_packet *p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare2.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare2.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,307),p->u.NationWar.NoticePrepare2.CommanderName);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	sprintf(tempMessage,lan->OutputMessage(0,305),WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare2Reinforce(t_packet *p)
{
	char tempMessage[500];
	char AttackNationName[20],DefenceNationName[20];
	char WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare2Reinforce.WarfieldNo,WarfieldName);
	ReturnNationName(p->u.NationWar.NoticePrepare2Reinforce.AttackNation,AttackNationName);
	ReturnNationName(p->u.NationWar.NoticePrepare2Reinforce.DefenceNation,DefenceNationName);
	GetTime(p->u.NationWar.NoticePrepare2Reinforce.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,306),AttackNationName,DefenceNationName,WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare3(t_packet *p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare3.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare3.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,304),p->u.NationWar.NoticePrepare3.LeadCandidaterName);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	sprintf(tempMessage,lan->OutputMessage(0,305),WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare3Reinforce(t_packet *p)
{
	char tempMessage[500];
	char DefenceNationName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnNationName(p->u.NationWar.NoticePrepare3Reinforce.DefenceNation,DefenceNationName);
	GetTime(p->u.NationWar.NoticePrepare3Reinforce.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,308),DefenceNationName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare4(t_packet *p)
{
	char tempMessage[500];
	char WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnWarfieldName(p->u.NationWar.NoticePrepare4.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticePrepare4.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,307),p->u.NationWar.NoticePrepare4.CommanderName);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	sprintf(tempMessage,lan->OutputMessage(0,305),WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPrepare4Reinforce(t_packet *p)
{
	char tempMessage[500];
	char DefenceNationName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnNationName(p->u.NationWar.NoticePrepare4Reinforce.DefenceNation,DefenceNationName);
	GetTime(p->u.NationWar.NoticePrepare4Reinforce.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,308),DefenceNationName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar1A(t_packet *p)
{
	char tempMessage[500];
	char DefenceNationName[20],WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnNationName(p->u.NationWar.NoticeWar1A.DefenceNation,DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar1A.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticeWar1A.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,310),DefenceNationName,WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar1D(t_packet *p)
{
	char tempMessage[500];
	char AttackNationName[20],WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS

	int WarfieldNo=p->u.NationWar.NoticeWar1D.WarfieldNo;
	IntoNationNotice(ALL_CLEAR,1,NULL);
	if (WarfieldNo>=0&&WarfieldNo<=2)
	{
		ReturnNationName(p->u.NationWar.NoticeWar1D.AttackNation,AttackNationName);
		ReturnWarfieldName(p->u.NationWar.NoticeWar1D.WarfieldNo,WarfieldName);
		GetTime(p->u.NationWar.NoticeWar1D.RemainTime,Hour,Min,Sec);
		sprintf(tempMessage,lan->OutputMessage(0,309),AttackNationName,WarfieldName,Hour,Min);
	}
	else
	if (WarfieldNo>=3)
	{
		//<! BBD 040315	
		switch(WarfieldNo)
		{
		case WI_HADES_WARFIELD:		// 지하전쟁터(몬스터 전쟁터)
			{
				//< LTH-040324-KO 시간 정상적으로 표시...
				g_dwRemainTime = p->u.NationWar.NoticeWar1D.RemainTime + (g_ClientTime / 1000);
				//> LTH-040324-KO
				GetTime(p->u.NationWar.NoticeWar1D.RemainTime,Hour,Min,Sec);
				sprintf(tempMessage,lan->OutputMessage(0,617),Hour,Min);
				break;
			}
		default:	// 설원전쟁터 WarfieldNo = 3
			{
				if (Hero->name_status.nation==NW_YL)
				{
					GetTime(p->u.NationWar.NoticeWar1D.RemainTime,Hour,Min,Sec);
					sprintf(tempMessage,lan->OutputMessage(0,384),Hour,Min);
				}
				else
				{
					GetTime(p->u.NationWar.NoticeWar1D.RemainTime,Hour,Min,Sec);
					sprintf(tempMessage,lan->OutputMessage(0,385),Hour,Min);
				}
				break;
			}
		}
		//> BBD 040315
	}
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar1Reinforce(t_packet *p)
{
	char tempMessage[500];
	char AttackNationName[20],DefenceNationName[20],WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnNationName(p->u.NationWar.NoticeWar1Reinforce.AttackNation,AttackNationName);
	ReturnNationName(p->u.NationWar.NoticeWar1Reinforce.DefenceNation,DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar1Reinforce.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticeWar1Reinforce.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,311),AttackNationName,DefenceNationName,WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	sprintf(tempMessage,lan->OutputMessage(0,319));
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar2A(t_packet *p)
{
	char tempMessage[500];
	char DefenceNationName[20],WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnNationName(p->u.NationWar.NoticeWar2A.DefenceNation,DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar2A.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticeWar2A.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,322),DefenceNationName,WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar2D(t_packet *p)
{
	char tempMessage[500];
	char AttackNationName[20],WarfieldName[20],DefenceNationName[20];
	WORD Hour,Min,Sec; // 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnNationName(p->u.NationWar.NoticeWar2D.AttackNation,AttackNationName);
	ReturnNationName(p->u.NationWar.NoticeWar2D.DefenceNation,DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar2D.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticeWar2D.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,321),DefenceNationName,AttackNationName,Hour,Min,WarfieldName);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessWar2Reinforce(t_packet *p)
{
	char tempMessage[500];
	char DefenceNationName[20],WarfieldName[20];
	WORD Hour,Min,Sec;	// 011020 LTS
	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnNationName(p->u.NationWar.NoticeWar2Reinforce.DefenceNation,DefenceNationName);
	ReturnWarfieldName(p->u.NationWar.NoticeWar2Reinforce.WarfieldNo,WarfieldName);
	GetTime(p->u.NationWar.NoticeWar2Reinforce.RemainTime,Hour,Min,Sec);
	sprintf(tempMessage,lan->OutputMessage(0,323),DefenceNationName,WarfieldName,Hour,Min);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	sprintf(tempMessage,lan->OutputMessage(0,320));
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPeace1V(t_packet *p)		//011025 LTS
{
	char tempMessage[500];
	char SecretWarfieldName[20];
	WORD Hour1,Min1,Sec1;
	WORD Hour2,Min2,Sec2;
	WORD Hour3,Min3,Sec3;

	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1V.SecretWarfieldNo,SecretWarfieldName);
	sprintf(tempMessage,lan->OutputMessage(0,312),SecretWarfieldName);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
//	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1V.WarfieldNo,WarfieldName);
//	sprintf(tempMessage,lan->OutputMessage(0,313),WarfieldName,p->u.NationWar.NoticeWarPeace1V.Money);
//	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	sprintf(tempMessage,lan->OutputMessage(0,317));
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	GetTime(p->u.NationWar.NoticeWarPeace1V.Warfield1RemainTime,Hour1,Min1,Sec1);
	GetTime(p->u.NationWar.NoticeWarPeace1V.Warfield2RemainTime,Hour2,Min2,Sec2);
	GetTime(p->u.NationWar.NoticeWarPeace1V.Warfield3RemainTime,Hour3,Min3,Sec3);
	sprintf(tempMessage,lan->OutputMessage(0,325),Hour1,Min1,Hour2,Min2,Hour3,Min3);
	//<! BBD 040316
	char temp2[500];
	sprintf(temp2,lan->OutputMessage(0,618), p->u.NationWar.NoticeWarPeace.aLocalWarPoint[0],
		p->u.NationWar.NoticeWarPeace.aLocalWarPoint[1], p->u.NationWar.NoticeWarPeace.aLocalWarPoint[2]);
	sprintf(tempMessage, "%s%s", tempMessage, temp2);
	//> BBD 040316
	IntoNationNotice(FORCE_TYPE,3,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPeace1D(t_packet *p)		// 011025 LTS
{
	char tempMessage[500];
	WORD Hour1,Min1,Sec1;
	WORD Hour2,Min2,Sec2;
	WORD Hour3,Min3,Sec3;

	IntoNationNotice(ALL_CLEAR,1,NULL);
//	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1D.WarfieldNo,WarfieldName);
//	sprintf(tempMessage,lan->OutputMessage(0,314),WarfieldName,p->u.NationWar.NoticeWarPeace1D.Money);
//	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	sprintf(tempMessage,lan->OutputMessage(0,317));
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	GetTime(p->u.NationWar.NoticeWarPeace1D.Warfield1RemainTime,Hour1,Min1,Sec1);
	GetTime(p->u.NationWar.NoticeWarPeace1D.Warfield2RemainTime,Hour2,Min2,Sec2);
	GetTime(p->u.NationWar.NoticeWarPeace1D.Warfield3RemainTime,Hour3,Min3,Sec3);
	sprintf(tempMessage,lan->OutputMessage(0,325),Hour1,Min1,Hour2,Min2,Hour3,Min3);
	//<! BBD 040316
	char temp2[500];
	sprintf(temp2,lan->OutputMessage(0,618), p->u.NationWar.NoticeWarPeace.aLocalWarPoint[0],
		p->u.NationWar.NoticeWarPeace.aLocalWarPoint[1], p->u.NationWar.NoticeWarPeace.aLocalWarPoint[2]);
	sprintf(tempMessage, "%s%s", tempMessage, temp2);
	//> BBD 040316
	IntoNationNotice(FORCE_TYPE,2,tempMessage);

	CallSmallMenu(MN_NATION_TODAY_NOTICE);

}

void ProcessPeace1Reinforce(t_packet *p)	// 011025 LTS
{
	char tempMessage[500];
	char SecretWarfieldName[20],WarfieldName[20],VictoryNationName[20],DefeatNationName[20];
	WORD Hour1,Min1,Sec1;
	WORD Hour2,Min2,Sec2;
	WORD Hour3,Min3,Sec3;

	IntoNationNotice(ALL_CLEAR,1,NULL);
	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1Reinforce.SecretWarfieldNo,SecretWarfieldName);
	ReturnWarfieldName(p->u.NationWar.NoticeWarPeace1Reinforce.WarfieldNo,WarfieldName);
	ReturnNationName(p->u.NationWar.NoticeWarPeace1Reinforce.VictoryNation,VictoryNationName);
	ReturnNationName(p->u.NationWar.NoticeWarPeace1Reinforce.DefeatNation,DefeatNationName);
	sprintf(tempMessage,lan->OutputMessage(0,315),SecretWarfieldName);
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	//sprintf(tempMessage,lan->OutputMessage(0,324),WarfieldName,VictoryNationName,DefeatNationName);
	//IntoNationNotice(FORCE_TYPE,2,tempMessage);
	sprintf(tempMessage,lan->OutputMessage(0,318));
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	GetTime(p->u.NationWar.NoticeWarPeace1Reinforce.Warfield1RemainTime,Hour1,Min1,Sec1);
	GetTime(p->u.NationWar.NoticeWarPeace1Reinforce.Warfield2RemainTime,Hour2,Min2,Sec2);
	GetTime(p->u.NationWar.NoticeWarPeace1Reinforce.Warfield3RemainTime,Hour3,Min3,Sec3);
	sprintf(tempMessage,lan->OutputMessage(0,325),Hour1,Min1,Hour2,Min2,Hour3,Min3);
	//<! BBD 040316
	char temp2[500];
	sprintf(temp2,lan->OutputMessage(0,618), p->u.NationWar.NoticeWarPeace.aLocalWarPoint[0],
		p->u.NationWar.NoticeWarPeace.aLocalWarPoint[1], p->u.NationWar.NoticeWarPeace.aLocalWarPoint[2]);
	sprintf(tempMessage, "%s%s", tempMessage, temp2);
	//> BBD 040316
	IntoNationNotice(FORCE_TYPE,3,tempMessage);

	CallSmallMenu(MN_NATION_TODAY_NOTICE);

}

void GetDayName(int Day,char* lpStr)			// KR-LAN으로 수정
{
	switch (Day)
	{
	case 0 : 	strcpy(lpStr,lan->OutputMessage(9,437)); break;
	case 1 :	strcpy(lpStr,lan->OutputMessage(9,431)); break;
	case 2 : 	strcpy(lpStr,lan->OutputMessage(9,432)); break;	
	case 3 : 	strcpy(lpStr,lan->OutputMessage(9,433)); break;
	case 4 :  	strcpy(lpStr,lan->OutputMessage(9,434)); break;
	case 5 : 	strcpy(lpStr,lan->OutputMessage(9,435)); break;
	case 6 : 	strcpy(lpStr,lan->OutputMessage(9,436)); break;
	default :   strcpy(lpStr,"  "); break;
	}
}

void ProcessPeace2(t_packet *p)		// 011025 LTS
{
	char tempMessage[500];
	char tempString[3][MAX_PATH];
	char tempString2[3][MAX_PATH];

	IntoNationNotice(ALL_CLEAR,1,NULL);
	sprintf(tempMessage,lan->OutputMessage(0,317));
	IntoNationNotice(FORCE_TYPE,1,tempMessage);

	GetDayName(p->u.NationWar.NoticeWarPeace.RemainTime[0][0],tempString[0]);
	GetDayName(p->u.NationWar.NoticeWarPeace.RemainTime[1][0],tempString[1]);
	GetDayName(p->u.NationWar.NoticeWarPeace.RemainTime[2][0],tempString[2]);
	
	int WarfieldNo[2];
	WarfieldNo[0]=p->u.NationWar.NoticeWarPeace.WarNo%3;
	WarfieldNo[1]=(p->u.NationWar.NoticeWarPeace.WarNo+1)%3;
	ReturnWarfieldName(WarfieldNo[0],tempString2[0]);
	ReturnWarfieldName(WarfieldNo[1],tempString2[1]);
	ReturnWarfieldName(3,tempString2[2]);
	
	sprintf(tempMessage,lan->OutputMessage(0,325),
		tempString[0],										// 요일
		p->u.NationWar.NoticeWarPeace.RemainTime[0][1],		// 시	
		tempString2[0],										// 장소
		tempString[1],										// 요일
		p->u.NationWar.NoticeWarPeace.RemainTime[1][1],		// 시
		tempString2[1],										// 장소
		tempString[2],										// 요일
		p->u.NationWar.NoticeWarPeace.RemainTime[2][1],  	// 시
		tempString2[2] );									// 장소
	//<! BBD 040316
	char temp2[500];
	sprintf(temp2,lan->OutputMessage(0,618), p->u.NationWar.NoticeWarPeace.aLocalWarPoint[0],
		p->u.NationWar.NoticeWarPeace.aLocalWarPoint[1], p->u.NationWar.NoticeWarPeace.aLocalWarPoint[2]);
	sprintf(tempMessage, "%s%s", tempMessage, temp2);
	//> BBD 040316	
	IntoNationNotice(FORCE_TYPE,2,tempMessage);

	if (p->u.NationWar.NoticeWarPeace.SOpenWarfieldNo>=0)
	{
		memset(tempMessage,0,500);
		switch (p->u.NationWar.NoticeWarPeace.SOpenWarfieldNo)
		{
		case 0 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,208)); break;	// BBD 040315
		case 1 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,209)); break;	// BBD 040315
		case 2 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,210)); break;	// BBD 040315
		case 3 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,386)); break;	// BBD 040315
		case 4 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,619)); break;	// BBD 040315
		}
		IntoNationNotice(FORCE_TYPE,3,tempMessage);
	}

	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessPeace2Reinforce(t_packet *p)	// 011025 LTS
{
	ProcessPeace2(p);
	return;

/*	char tempMessage[500];
	WORD Hour1,Min1,Sec1;
	WORD Hour2,Min2,Sec2;
	WORD Hour3,Min3,Sec3;

	IntoNationNotice(ALL_CLEAR,1,NULL);
	sprintf(tempMessage,lan->OutputMessage(0,318));
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	GetTime(p->u.NationWar.NoticeWarPeace2.Warfield1RemainTime,Hour1,Min1,Sec1);
	GetTime(p->u.NationWar.NoticeWarPeace2.Warfield2RemainTime,Hour2,Min2,Sec2);
	GetTime(p->u.NationWar.NoticeWarPeace2.Warfield3RemainTime,Hour3,Min3,Sec3);
	sprintf(tempMessage,lan->OutputMessage(0,325),Hour1,Min1,Hour2,Min2,Hour3,Min3);
	IntoNationNotice(FORCE_TYPE,2,tempMessage);

	CallSmallMenu(MN_NATION_TODAY_NOTICE);*/
}

void ProcessSopen(t_packet* p)
{
	char tempMessage[500];
	IntoNationNotice(ALL_CLEAR,1,NULL);
	sprintf(tempMessage,lan->OutputMessage(0,317));
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	
	switch (p->u.NationWar.NoticeSOpen.WarfieldNo)
	{
	case 0 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,208)); break;
	case 1 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,209)); break;
	case 2 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,210)); break;
	case 3 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,386)); break;
	case 4 : sprintf(tempMessage,lan->OutputMessage(0,312),lan->OutputMessage(0,619)); break;	// BBD 040315
	}
	IntoNationNotice(FORCE_TYPE,2,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void ProcessSopenError(t_packet* p)
{
	char tempMessage[500];
	IntoNationNotice(ALL_CLEAR,1,NULL);
	sprintf(tempMessage,lan->OutputMessage(0,317));
	IntoNationNotice(FORCE_TYPE,1,tempMessage);
	CallSmallMenu(MN_NATION_TODAY_NOTICE);
}

void RecvCMD_ANSWER_NOTICE(t_packet *p) //투데이 노티스 
{
	char NoticeType=p->u.data[0];

	//AddCurrentStatusMessage( 255,255,0, "노티스번호 : %d ",NoticeType);

	switch (NoticeType)
	{
	case NW_NOTICE_JOIN_ATTACK				: ProcessNoticeJoinA(p);				break;
	case NW_NOTICE_JOIN_DEFENCE				: ProcessNoticeJoinD(p);				break;
	case NW_NOTICE_JOIN_REINFORCE			: ProcessNoticeJoinReinforce(p);		break;
	case NW_NOTICE_PREPARE1					: ProcessPrepare1(p);					break;
	case NW_NOTICE_PREPARE1_REINFORCE		: ProcessPrepare1Reinforce(p);			break;	
	case NW_NOTICE_PREPARE2					: ProcessPrepare2(p);					break;
	case NW_NOTICE_PREPARE2_REINFORCE		: ProcessPrepare2Reinforce(p);			break;	
	case NW_NOTICE_PREPARE3					: ProcessPrepare3(p);					break;
	case NW_NOTICE_PREPARE3_REINFORCE		: ProcessPrepare3Reinforce(p);			break;	
	case NW_NOTICE_PREPARE4					: ProcessPrepare4(p);					break;
	case NW_NOTICE_PREPARE4_REINFORCE		: ProcessPrepare4Reinforce(p);			break;	
	case NW_NOTICE_WAR1A					: ProcessWar1A(p);						break;
	case NW_NOTICE_WAR1D					: ProcessWar1D(p);						break;
	case NW_NOTICE_WAR1_REINFORCE			: ProcessWar1Reinforce(p);				break;
	case NW_NOTICE_WAR2A					: ProcessWar2A(p);						break;
	case NW_NOTICE_WAR2D					: ProcessWar2D(p);						break;
	case NW_NOTICE_WAR2_REINFORCE			: ProcessWar2Reinforce(p);				break;
	case NW_NOTICE_PEACE1_VICTORY			: ProcessPeace1V(p);					break;	
	case NW_NOTICE_PEACE1_DEFEAT			: ProcessPeace1D(p);					break;	
	case NW_NOTICE_PEACE1_REINFORCE			: ProcessPeace1Reinforce(p);			break;	
	case NW_NOTICE_PEACE2					: ProcessPeace2(p);						break;	
	case NW_NOTICE_PEACE2_REINFORCE			: ProcessPeace2Reinforce(p);			break;
	case NW_NOTICE_SOPEN					: ProcessSopen(p);						break;
	case NW_NOTICE_SOPEN_ERROR				: ProcessSopenError(p);					break;			// 이곳으로 들어오면 WarfieldNo가 잘못된것임
	}
}

void RecvCMD_WAR_CONTROL(t_packet *p)		// 011020 LTS
{
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )	// 게임제작 모드 일때만 쓴다..
	{
		AddCurrentStatusMessage( 255,255,0,"The current warfield warfield status is status : [%d], loop status : [%d].",
			p->u.NationWar.WarControl.Status,p->u.NationWar.WarControl.Active);
	}
#endif
}

void RecvCMD_WAR_LOOP_TIME(t_packet *p)
{
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )	// 게임제작 모드 일때만 쓴다..
	{
		AddCurrentStatusMessage( 255,255,0,"The current warfield warfield status is status : [%d], loop time : [%d].",
			p->u.NationWar.WarLoopTime.Status,p->u.NationWar.WarLoopTime.LoopTime);
	}
}

void RecvCMD_FAME_EXP_CHANGE(t_packet *p)		// 020115 LTS	// LTS NEW LOCALWAR
{
	short int	tempFame=p->u.NationWar.FameExpChange.GainedFame;
	DWORD		tempExp=p->u.NationWar.FameExpChange.GainedExp;
	PlayListAutoSounds(62,0,0,FALSE);
/*	AddCurrentStatusMessage( 255,255,0,"============< 전쟁 보상 >==============");
	AddCurrentStatusMessage( 255,255,0,"전쟁에서 많은 공을 세워 국가에서 아래와 같은 보상을 하게됬습니다.");
	AddCurrentStatusMessage( 255,255,0,"   [ 명    성] : %d",tempFame);
	AddCurrentStatusMessage( 255,255,0,"   [ 경 험 치] : %d",tempExp);*/
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,198));
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,199));
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,200),tempFame);
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,201),tempExp);

}

//< LTH-040318-KO 남은 가드들의 수를 받았다
void RecvCMD_CHECK_REMAIN_GUARD(t_packet *p)
{
	int nI, nJ;
	if (IsNeoWarfield(p->u.NationWar.RemainGuard.nWarfieldNo))
	{
		//< LTH-040324-KO 정확한 시간계산용. 남은 시간.
		g_dwRemainTime = p->u.NationWar.RemainGuard.dwRemainTime + (g_ClientTime /1000);
		//> LTH-040324-KO
		for (nJ = 0; nJ < MAX_TEAM; ++nJ)
		{
			for (nI = 0; nI < NEO_NUMOFGUARDTYPE; ++nI)
			{
				g_aRemainGuard[nJ][nI] = p->u.NationWar.RemainGuard.aRemainGuardCount[nJ][nI];
			}
		}
	}
}
//> LTH-040318-KO

void RecvCMD_NWAFIELD_START_WAR(t_packet *p)
{
	g_bWarBegin=true;
	g_pNation->SetNewStatus(p->u.NationWar.NewWarStart.WarfieldNo,NW_WAR);
//<! BBD 040311
	//< LTH-040130-KO 1.4 Patch
	switch (p->u.NationWar.NewWarStart.WarfieldNo)
	{
	case 0:
		SMenu[MN_WAR_START].nField[0].nRectImage=4;				// 설원 전쟁터 이미지
		SMenu[MN_WAR_START].nField[0].nImageNumber=4;
		
		SMenu[MN_WAR_START].nField[2].x=188;						// 왼쪽 독수리마크
		SMenu[MN_WAR_START].nField[2].y=125;
		
		SMenu[MN_WAR_START].nField[3].x=116;					// 화살표
		SMenu[MN_WAR_START].nField[3].y=200;
		SetRect(SMenu[MN_WAR_START].nField[3].rCheakBox,116, 200, 180, 300);
		SMenu[MN_WAR_START].nField[3].nRectImage=12;
		SMenu[MN_WAR_START].nField[3].nImageNumber=12;
		
		SMenu[MN_WAR_START].nField[4].x=55;						// 오른쪽 전갈 마크
		SMenu[MN_WAR_START].nField[4].y=120;
		AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,297)); 

		sprintf(SMenu[MN_WAR_START].nField[5].temp,lan->OutputMessage(0,387),p->u.NationWar.NewWarStart.LevelMin,p->u.NationWar.NewWarStart.LevelMax);		// 내용
		break;		//"설원전쟁터의 전쟁의 시작되었습니다."

	case WI_HADES_WARFIELD:
		SMenu[MN_WAR_START].nField[0].nImageNumber=13;			// 몬스터 전쟁터 이미지
		SMenu[MN_WAR_START].nField[2].nType=FT_NONE;			// 독수리 마크고 나발이고 다 필요없다.
		SMenu[MN_WAR_START].nField[3].nType=FT_NONE;
		SMenu[MN_WAR_START].nField[4].nType=FT_NONE;

		//< LTH-040312-KO 인터페이스의 내용
		int nTerm = (int)(p->u.NationWar.NewWarStart.dwTerm / 60);
		sprintf(SMenu[MN_WAR_START].nField[5].temp,lan->OutputMessage(0,615),nTerm ,p->u.NationWar.NewWarStart.btNationPoint);		// 내용
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0,613));	// "몬스터 전쟁터가 시작 어쩌구..."
		g_dwRemainTime = p->u.NationWar.NewWarStart.dwTerm + (g_ClientTime / 1000);
		//> LTH-040312-KO
		break;
	}
	//> LTH-040130-KO
//> BBD 040311

	CallSmallMenu(MN_WAR_START);
}

void RecvCMD_NWARFIELD_END_WAR(t_packet *p)
{
	g_bWarBegin=false;

	//< LTH-040303-KO 1.4 Patch. 지하전쟁터 추가
	INT nWarfieldNo = p->u.NationWar.CommonDataC.Data;
	if (!IsNeoWarfield(nWarfieldNo))
	{
		g_pNation->SetNewStatus(nWarfieldNo, NW_PEACE);
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0,298));		// "설원전쟁터의 전쟁의 끝났습니다."
	}
	else
	{
		g_pNation->SetNewStatus(nWarfieldNo, NW_SOPEN);
		AddCurrentStatusMessage(255, 255, 0, lan->OutputMessage(0,614));		// "몬스터 국가전이 끝났습니다."	// BBD 040311
	}
	//> LTH-040303-KO
}

void RecvCMD_FAME_CHANGE(t_packet *p)
{
	PlayListAutoSounds(62,0,0,FALSE);
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,198));
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,203));
	AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,200),p->u.NationWar.CommonDataDw.Data);
}

void CallWarResult(WAR_RESULT tWarResult,bool bAmIWin)
{
	SMenu[MN_NATION_WAR_RESULT].nField[1].nWillDo = tWarResult.BYDeathCount;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nWillDo = tWarResult.ZYDeathCount;

	SMenu[MN_NATION_WAR_RESULT].nField[3].nWillDo = tWarResult.BYCount;
	SMenu[MN_NATION_WAR_RESULT].nField[4].nWillDo = tWarResult.YLBYCount;

	SMenu[MN_NATION_WAR_RESULT].nField[5].nWillDo =	tWarResult.ZYCount;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nWillDo = tWarResult.YLZYCount;

	
	if( bAmIWin )
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 0;
		if(tWarResult.VictoryTeam==0)	//바이서스가 이김
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 1;
		else 
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 3;
	}
	else
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 1;
		if(tWarResult.VictoryTeam==0)	//자이펀이 짐
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 4;
		else
			SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 2;
	}	

	SMenu[MN_NATION_WAR_RESULT].nField[9].nImageNumber = 19;	// BBD 040311

	CallSmallMenu(MN_NATION_WAR_RESULT);
	return;
}

//< LTH-040303-KO 1.4 Patch. 새 결과 화면을 위해 다른 패킷구조를 갖는다
VOID CallWarResult2(NEO_WAR_RESULT tagNeoWarResult, BOOL bAmIWin)
{
//<! BBD 040311
	SMenu[MN_NATION_WAR_RESULT].nField[1].nWillDo = tagNeoWarResult.nBYDeathCount;
	SMenu[MN_NATION_WAR_RESULT].nField[2].nWillDo = tagNeoWarResult.nZYDeathCount;
	SMenu[MN_NATION_WAR_RESULT].nField[3].nWillDo = tagNeoWarResult.nYLDeathCount;

	SMenu[MN_NATION_WAR_RESULT].nField[4].nWillDo = tagNeoWarResult.nBYMemberCount;
	SMenu[MN_NATION_WAR_RESULT].nField[5].nWillDo =	tagNeoWarResult.nZYMemberCount;
	SMenu[MN_NATION_WAR_RESULT].nField[6].nWillDo = tagNeoWarResult.nYLMemberCount;
	
	SMenu[MN_NATION_WAR_RESULT].nField[9].nImageNumber = 20;	//  몬스터 국가전용

	if( bAmIWin )
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 0;
		SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 17;	// 승니
	}
	else
	{
		SMenu[MN_NATION_WAR_RESULT].nField[0].nImageType = 1;
		SMenu[MN_NATION_WAR_RESULT].nField[7].nImageNumber = 18;	// 폐배

	}	
//> BBD 040311
	CallSmallMenu(MN_NATION_WAR_RESULT);
}
//> LTH-040303-KO

void RecvCMD_WAR_RESULT(t_packet *p)
{
	bool AmIWin=false;
	g_bWarBegin=false;
	//< LTH-040303-KO 1.4 Patch. 새 전쟁터의 새로운 결과 창
	//< LTH-040427-KO 지하 전쟁터의 정확한 전쟁터 번호를 위해 추가
	INT nWarfieldNo = p->u.NationWar.WarResult.WarfieldPort - WP_BASE_PORT + 1;
	if (!IsNeoWarfield(nWarfieldNo))
	{
		nWarfieldNo = p->u.NationWar.NeoWarResult.wWarfieldPort - WP_BASE_PORT + 1;
		if (!IsNeoWarfield(nWarfieldNo))
			nWarfieldNo = p->u.NationWar.WarResult.WarfieldPort - WP_BASE_PORT + 1;
	}
	//> LTH-040427-KO
	if (!IsNeoWarfield(nWarfieldNo))
	{
		g_pNation->SetNewStatus(nWarfieldNo, NW_PEACE);
		switch (Hero->name_status.nation)
		{
		case NW_BY :
			if (p->u.NationWar.WarResult.VictoryTeam==0)
				AmIWin=true;
			else
				AmIWin=false;
			break;
		case NW_ZY :
			if (p->u.NationWar.WarResult.VictoryTeam==0)
				AmIWin=false;
			else
				AmIWin=true;
			break;
		case NW_YL :
			if (Hero->NWCharacter.YL_JoinNation==NW_BY)
			{
				if (p->u.NationWar.WarResult.VictoryTeam==0)
					AmIWin=true;
				else
					AmIWin=false;
			}
			else
			{
				if (p->u.NationWar.WarResult.VictoryTeam==0)
					AmIWin=false;
				else
					AmIWin=true;
			}
			break;
		default:
			{// 030506 kyo 모든국가 패배
				AmIWin = false;
				break;
			}
		}

		CallWarResult(p->u.NationWar.WarResult,AmIWin);
	}
	else
	{	//< LTH-040303-KO 1.4 Patch.
		g_pNation->SetNewStatus(nWarfieldNo, NW_SOPEN);
		if (TEAM1 == p->u.NationWar.NeoWarResult.cVictoryTeam)
			AmIWin = TRUE;
		else
			AmIWin = FALSE;

		//< LTH-040413-KO 모든 국가가 몬스터에게 습격 받았을때 몬스터 국가전 창을 띄우지 않는다.
		if (TRUE == p->u.NationWar.NeoWarResult.bAllNationJoin)
			CallWarResult2(p->u.NationWar.NeoWarResult, AmIWin);
		//> LTH-040413-KO
	}	//< LTH-040303-KO
	//> LTH-040303-KO
}

void RecvCMD_ANSWER_NEW_WARFIELD_TILE_DONT(t_packet *p)
{
	if (g_pDontManager==NULL)
		return;

	for (int i=0;i<p->u.NationWar.DontData.Size;i++)
	{
		g_pDontManager->ActiveDont(p->u.NationWar.DontData.DontNo[i],p->u.NationWar.DontData.Status[i]);
	}
}

void RecvCMD_GUARD_KILLER(t_packet *p)
{
	LPCHARACTER temp=FindHim(p->u.NationWar.GuardKiller.GuardID+10000);
	if (temp!=NULL)	
	{
		temp->GuardStoneAni=1;		// h_DisPlayNPC()에서 에니메이션을 처리하기 위한 데이터 
		temp->SetAni2Zero=1;
		temp->sealstoneani=0;
	}
	temp=FindHim(p->u.NationWar.GuardKiller.KillerID);
	if (temp!=NULL)
	{
		InsertMagic(temp,temp,451,9,0,0,temp->x,temp->y);
	}
}

void RecvCMD_DONT_CHANGE(t_packet *p)
{
	g_pDontManager->ActiveDont(p->u.NationWar.DontChange.DontNo,p->u.NationWar.DontChange.Status);
}

void RecvCMD_ANSWER_NEW_WARFIELD_STATUS(t_packet *p)
{
	//< LTH-040303-KO 1.4 Patch. 설원전쟁터는 0 이 몬스터 전쟁터는 1 + 3 = 4 가 된다
	for (int i=0;i<MAX_NEW_WARFIELD + 1;i++)
	{
		//< LTH-040414-KO 설원전쟁터이면?
		if (i == 0)
			g_pNation->SetNewStatus(i,p->u.NationWar.NWStatus.Status[i]);
		else
			g_pNation->SetNewStatus(i + 3,p->u.NationWar.NWStatus.Status[i]);
		//> LTH-040414-KO
	}
	//> LTH-040303-KO
}

int GetHeroTeamNo()
{
	int Nation=Hero->name_status.nation;
	if (Nation==NW_YL)
		Nation=Hero->NWCharacter.YL_JoinNation;

	switch(Nation)
	{
	case NW_BY : return 0;
	case NW_ZY : return 1;
	}
	return 0;
}



void RecvCMD_ANSWER_NEW_WARFIELD_DATA(t_packet *p)
{
	int HeroTeamNo=GetHeroTeamNo();
	WORD Hour,Min,Sec;

	//< LTH-040302-KO 1.4 Patch. IsNeoWarfieldServer() Add.
	if (isNewWarfieldServer() || IsNeoWarfieldServer())				// 전쟁터에 있으면 주인공을 그려준다.
	{
		//<! BBD 040315
		float InterPoleX = 0.0f;
		float InterPoleY = 0.0f;
		int PosX = 0;
		int PosY = 0;
		// 설원 전쟁터와 지하전쟁터는 맵 크기가 다르다
		if(!IsNeoWarfieldServer())
		{	// 설원 전쟁터
			InterPoleX = (Hero->x/TILE_SIZE)/(float)439;
			InterPoleY = (Hero->y/TILE_SIZE)/(float)439;
			PosX = 20+(286*InterPoleX);
			PosY = 36+(286*InterPoleY);
		}
		else
		{	// 지하 전쟁터
			InterPoleX = (Hero->x/TILE_SIZE)/(float)820;
			InterPoleY = (Hero->y/TILE_SIZE)/(float)820;
			PosX = (280*InterPoleX) + 30;
			PosY = (280*InterPoleY) + 40;
		}
		//> BBD 040315

		SMenu[MN_WAR_STATUS].nField[2].x=PosX;						//주인공의 위치
		SMenu[MN_WAR_STATUS].nField[2].y=PosY;
		
		switch (Hero->class_type)
		{
		case WARRIOR	: SMenu[MN_WAR_STATUS].nField[2].nImageNumber=13; break;
		case THIEF		: SMenu[MN_WAR_STATUS].nField[2].nImageNumber=14; break;
		case ARCHER		: SMenu[MN_WAR_STATUS].nField[2].nImageNumber=15; break;
		case WIZARD		: SMenu[MN_WAR_STATUS].nField[2].nImageNumber=16; break;
		case PRIEST		: SMenu[MN_WAR_STATUS].nField[2].nImageNumber=17; break;
		}
		SMenu[MN_WAR_STATUS].nField[2].nType=FT_NOMAL_PUT;
	}
	else
	{
		SMenu[MN_WAR_STATUS].nField[2].nType=FT_NONE;
	}
	//> LTH-040302-KO
  
	//< LTH-040302-KO 1.4 Patch. 새 전쟁터는 패킷구조가 다르다. 가드 추가마다 고쳐야하다니 -_-;
	if (!IsNeoWarfield(p->u.NationWar.NeoWarfieldData.nWarfieldNo))
	{
		//<! BBD 040311
		// 설원 전쟁터로 메뉴 이미지 변경
		SMenu[MN_WAR_STATUS].nField[0].nImageNumber = 18;	// 설원 전쟁터 이름
		SMenu[MN_WAR_STATUS].nField[1].nImageNumber = 3;	// 설원 전쟁터 맵
		SMenu[MN_WAR_STATUS].nField[15].nImageNumber = 21;	// 바이서스 자이펀 글자 이미지
		//> BBD 040311

		GetTime(p->u.NationWar.NWarfieldData1.RemainTime,Hour,Min,Sec);
		sprintf(SMenu[MN_WAR_STATUS].nField[11].temp,"%02d:%02d",Hour,Min);	// 남은시간

		sprintf(SMenu[MN_WAR_STATUS].nField[12].temp,"%d",p->u.NationWar.NWarfieldData1.TeamCount[0]);	// 팀카운트
		sprintf(SMenu[MN_WAR_STATUS].nField[13].temp,"%d",p->u.NationWar.NWarfieldData1.TeamCount[1]);	

		// 바이서스
		if (!p->u.NationWar.NWarfieldData1.GuardStatus[0][0])	// 보스 가드스톤
			SMenu[MN_WAR_STATUS].nField[3].nType=FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[3].nType=FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[0][1])	// 상태1
			SMenu[MN_WAR_STATUS].nField[4].nType=FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[4].nType=FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[0][2])	// 상태2
			SMenu[MN_WAR_STATUS].nField[5].nType=FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[5].nType=FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[0][3]||
			!p->u.NationWar.NWarfieldData1.GuardStatus[0][4])	// 상태3
			SMenu[MN_WAR_STATUS].nField[6].nImageNumber=6;
		else
			SMenu[MN_WAR_STATUS].nField[6].nImageNumber=5;

		// 자이펀
		if (!p->u.NationWar.NWarfieldData1.GuardStatus[1][0])	// 보스 가드스톤
			SMenu[MN_WAR_STATUS].nField[7].nType=FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[7].nType=FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[1][1])	// 상태1
			SMenu[MN_WAR_STATUS].nField[8].nType=FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[8].nType=FT_NOMAL_PUT;

		if (!p->u.NationWar.NWarfieldData1.GuardStatus[1][2])	// 상태2
			SMenu[MN_WAR_STATUS].nField[9].nType=FT_NONE;
		else
			SMenu[MN_WAR_STATUS].nField[9].nType=FT_NOMAL_PUT;
		
		if (!p->u.NationWar.NWarfieldData1.GuardStatus[1][3]||
			!p->u.NationWar.NWarfieldData1.GuardStatus[1][4])	// 상태3
			SMenu[MN_WAR_STATUS].nField[10].nImageNumber=6;
		else
			SMenu[MN_WAR_STATUS].nField[10].nImageNumber=5;
	}
	else
	{	//< LTH-040302-KO 1.4 Patch.

		//<! BBD 040311
		// 몬스터 전쟁터로 메뉴 이미지 변경
		SMenu[MN_WAR_STATUS].nField[0].nImageNumber = 20;	// 몬스터 전쟁터 이름
		SMenu[MN_WAR_STATUS].nField[1].nImageNumber = 19;	// 몬스터 전쟁터 맵
		SMenu[MN_WAR_STATUS].nField[15].nImageNumber = 22;	// 성문 결계석 글자 이미지

		//설원 전쟁터에서만 띄우는 상태들을 죽인다
		for(int i = 3; i <= 10; i++)
		{
			SMenu[MN_WAR_STATUS].nField[i].nType=FT_NONE;
		}
		// 화면 우상단에 띄우는 진행상황은 gameproc에서 호출된다.

		//> BBD 040311

		g_dwRemainTime = p->u.NationWar.NeoWarfieldData.dwRemainTime + (g_ClientTime / 1000);	// LTH-040314-KO 전쟁중 우상단에 띄워질 내용. 시간
		GetTime(p->u.NationWar.NeoWarfieldData.dwRemainTime, Hour, Min, Sec);
		sprintf(SMenu[MN_WAR_STATUS].nField[11].temp,"%02d:%02d",Hour,Min);	// 남은시간

		sprintf(SMenu[MN_WAR_STATUS].nField[12].temp,"%d", \
			p->u.NationWar.NeoWarfieldData.nRemainGuardCount[TEAM2][2]);	// 적군의 성문 남은 개수
		//< LTH-040314-KO 전쟁중 우상단에 띄워질 내용. 남은 가드들의 수
		int nI;
		for (nI = 0; nI < 4; ++nI)
			g_aRemainGuard[TEAM2][nI] = p->u.NationWar.NeoWarfieldData.nRemainGuardCount[TEAM2][nI];
		//> LTH-040314-KO

		sprintf(SMenu[MN_WAR_STATUS].nField[13].temp,"%d", \
			p->u.NationWar.NeoWarfieldData.nRemainGuardCount[TEAM2][1] + \
			p->u.NationWar.NeoWarfieldData.nRemainGuardCount[TEAM2][3]);	// 적군의 결계석 남은 개수. 내성 + 외성

	}	//> LTH-040302-KO
	//> LTH-040302-KO

	CallSmallMenu(MN_WAR_STATUS);
}

void RecvCMD_WAR_YL_JOIN(t_packet *p)
{
	Hero->NWCharacter.YL_JoinNation=p->u.NationWar.CommonDataC.Data;
}

void RecvCMD_GUARD_BROKE(t_packet *p)
{	// LTH-040303-KO 1.4 Patch. 기존 국가전과는 팀 구조가 다르기 때문에 나눈다.
	INT nWarfieldNo = p->u.NationWar.GuardBroke.nWarfieldNo;

	if (!IsNeoWarfield(nWarfieldNo))
	{
		switch (p->u.NationWar.GuardBroke.Type)
		{
		case 0 :																	// 수호석
			if (p->u.NationWar.GuardBroke.BrokeTeamNo==0)		//바이서스가 깨진거..
			{
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,388),lan->OutputMessage(0,205),p->u.NationWar.GuardBroke.X,p->u.NationWar.GuardBroke.Y);
			}
			else
			{
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,388),lan->OutputMessage(0,206),p->u.NationWar.GuardBroke.X,p->u.NationWar.GuardBroke.Y);
			}
			break;
		case 1 :										// 성문 
			if (p->u.NationWar.GuardBroke.BrokeTeamNo==0)		//바이서스가 깨진거..
			{
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,389),lan->OutputMessage(0,205),p->u.NationWar.GuardBroke.X,p->u.NationWar.GuardBroke.Y);
			}
			else
			{
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,389),lan->OutputMessage(0,206),p->u.NationWar.GuardBroke.X,p->u.NationWar.GuardBroke.Y);
			}
			break;
		}
	}
	else
	{	//< LTH-040303-KO 바로 이부분이 바뀐 부분이당
		switch(p->u.NationWar.GuardBroke.Type)
		{
		case 1:		// 내성 수호석
			if (TEAM2 == p->u.NationWar.GuardBroke.BrokeTeamNo)		// 유저팀이 부수었다
			{	// LTH-040316-KO
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,623), \
					p->u.NationWar.GuardBroke.X,p->u.NationWar.GuardBroke.Y);
			}
			break;

		case 2:		// 성문
			if (TEAM2 == p->u.NationWar.GuardBroke.BrokeTeamNo)		// 유저팀이 부수었다
			{	// LTH-040316-KO
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,624), \
					p->u.NationWar.GuardBroke.X,p->u.NationWar.GuardBroke.Y);
			}
			break;

		case 3:		// 외성 수호석
			if (TEAM2 == p->u.NationWar.GuardBroke.BrokeTeamNo)		// 유저팀이 부수었다
			{	// LTH-040316-KO
				AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,623), \
					p->u.NationWar.GuardBroke.X,p->u.NationWar.GuardBroke.Y);
			}
			break;
		}	//> LTH-040303-KO

		g_aRemainGuard[TEAM2][p->u.NationWar.GuardBroke.Type] = \
			p->u.NationWar.GuardBroke.aRemainGuard[TEAM2][p->u.NationWar.GuardBroke.Type];	// LTH-040314-KO 전쟁중에 나타낼 정보
	}
}	//> LTH-040303-KO

//< LTH-040317-KO 가드가 부서지지 않는 이유를 설명한다
void RecvCMD_GUARD_CAN_NOT_BROKE(t_packet *p)
{
	if (IsNeoWarfield(p->u.NationWar.GuardCanNotBroke.nWarfieldNo))
	{
		switch(p->u.NationWar.GuardCanNotBroke.nType)
		{
		case 1:		// 최종 수호석이 부서지지 않는 이유
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,620));	// "내성 수호석이 전부 부서지지 않아 공격할 수 없습니다."
			break;

		case 2:
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,625));	// "성문이 전부 부서지지 않아 공격할 수 없습니다."
			break;

		case 3:
			AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,621));	// "외성 수호석이 전부 부서지지 않아 공격할 수 없습니다."
			break;
		}
	}
}
//> LTH-040317-KO

int CheckHandleByNationWar(t_packet *p)
{
	switch (p->h.header.type)
	{
	case CMD_WAR_BBS :
								RecvCMD_WAR_BBS(p);
								break;
	case CMD_ANSWER_WARFIELD_STATUS :
								RecvCMD_ANSWER_WARFIELD_STATUS(p);
								break;
	case CMD_ANSWER_VOTE_DATA :
								RecvCMD_ANSWER_VOTE_DATA(p);
								break;
	case CMD_ANSWER_SQUAD_INFO :
								RecvCMD_ANSWER_SQUAD_INFO(p);
								break;
	case CMD_ANSWER_SQUAD_INFO2 :
								RecvCMD_ANSWER_SQUAD_INFO2(p);
								break;
	case CMD_ANSWER_COMMANDER_VOTE_DATA :
								RecvCMD_ANSWER_COMMANDER_VOTE_DATA(p);
								break;
	case CMD_ANSWER_COMMANDER_REG_DATA :
								RecvCMD_ANSWER_COMMANDER_REG_DATA(p);
								break;
	case CMD_YOU_ARE_SQUAD_LEADER :
								RecvCMD_YOU_ARE_SQUAD_LEADER(p);
								break;
	case CMD_JOIN_SQUAD :
								RecvCMD_JOIN_SQUAD(p);
								break;
	case CMD_ANSWER_NATION_WAR_COUNT :
								RecvCMD_ANSWER_NATION_WAR_COUNT(p);
								break;
	case CMD_NW_MAP_MOVE_FAIL :
								RecvCMD_NW_MAP_MOVE_FAIL(p);
								break;
	case CMD_ANSWER_TILE_DONT :
								RecvCMD_ANSWER_TILE_DONT(p);
								break;
	case CMD_TILE_DONT_CHANGED :
								RecvCMD_TILE_DONT_CHANGED(p);
								break;
	case CMD_SQUAD_CHAT :
								RecvCMD_SQUAD_CHAT(p);
								break;
	case CMD_CLEAR_SQUAD_DATA :
								RecvCMD_CLEAR_SQUAD_DATA(p);
								break;
	case CMD_ANSWER_WARFIELD_INFO :
								RecvCMD_ANSWER_WARFIELD_INFO(p);
								break;
	case CMD_WAR_BEGIN :
								RecvCMD_WAR_BEGIN(p);
								break;
	case CMD_WAR_END :
								RecvCMD_WAR_END(p);
								break;
	case CMD_ANSWER_CHECK_WARFIELD_POSSESSION :
								RecvCMD_ANSWER_CHECK_WARFIELD_POSSESSION(p);
								break;
	case CMD_ANSWER_WARFIELD_POSSESSION :
								RecvCMD_ANSWER_WARFIELD_POSSESSION(p);
								break;
	case CMD_WARFIELD_WAR_DATA1 :
								RecvCMD_WARFIELD_WAR_DATA1(p);
								break;
	case CMD_WARFIELD_WAR_DATA2 :
								RecvCMD_WARFIELD_WAR_DATA2(p);
								break;
	case CMD_WARFIELD_WAR_DATA3 :
								RecvCMD_WARFIELD_WAR_DATA3(p);
								break;
	case CMD_CLEAR_WAR_CHARACTER_DATA :
								RecvCMD_CLEAR_WAR_CHARACTER_DATA(p);
								break;
	case CMD_YOU_ARE_SQUAD_COMMANDER :
								RecvCMD_YOU_ARE_SQUAD_COMMANDER(p);
								break;
	case CMD_ANSWER_COMMANDER_VOTE_DATA2 :
								RecvCMD_ANSWER_COMMANDER_VOTE_DATA2(p);
								break;
	case CMD_SQUAD_JOIN_RESULT :
								RecvCMD_SQUAD_JOIN_RESULT(p);
								break;
	case CMD_ANSWER_NOTICE :
								RecvCMD_ANSWER_NOTICE(p);
								break;
	case CMD_WAR_CONTROL :						// 011020 LTS
								RecvCMD_WAR_CONTROL(p);
								break;
	case CMD_WAR_LOOP_TIME :					// 011025 LTS
								RecvCMD_WAR_LOOP_TIME(p);
								break;
	case CMD_FAME_EXP_CHANGE :					// 020115 LTS
								RecvCMD_FAME_EXP_CHANGE(p);
								break;
	case CMD_NWARFIELD_START_WAR :
								RecvCMD_NWAFIELD_START_WAR(p);
								break;
	case CMD_NWARFIELD_END_WAR :
								RecvCMD_NWARFIELD_END_WAR(p);
								break;
	case CMD_FAME_CHANGE :
								RecvCMD_FAME_CHANGE(p);
								break;
	case CMD_WAR_RESULT :
								RecvCMD_WAR_RESULT(p);
								break;
	case CMD_ANSWER_NEW_WARFIELD_TILE_DONT :
								RecvCMD_ANSWER_NEW_WARFIELD_TILE_DONT(p);
								break;
	case CMD_GUARD_KILLER :
								RecvCMD_GUARD_KILLER(p);
								break;
	case CMD_DONT_CHANGE	:
								RecvCMD_DONT_CHANGE(p);
								break;
	case CMD_ANSWER_NEW_WARFIELD_STATUS :
								RecvCMD_ANSWER_NEW_WARFIELD_STATUS(p);
								break;
	case CMD_ANSWER_NEW_WARFIELD_DATA :
								RecvCMD_ANSWER_NEW_WARFIELD_DATA(p);
								break;
	case CMD_WAR_YL_JOIN :
								RecvCMD_WAR_YL_JOIN(p);
								break;
	case CMD_GUARD_BROKE :
								RecvCMD_GUARD_BROKE(p);
								break;

	//< LTH-040317-KO 가드가 부서지지 않는 이유를 받았다
	case CMD_GUARD_CAN_NOT_BROKE :
								RecvCMD_GUARD_CAN_NOT_BROKE(p);
								break;
	//> LTH-040317-KO

	//< LTH-040322-KO 남은 가드들의 수를 반환한다
	case CMD_CHECK_REMAIN_GUARD:
								RecvCMD_CHECK_REMAIN_GUARD(p);
								break;
	//> LTH-040322-KO
			
	default : CheckHandleByLTSSupport(p);

	}
	return 1;
}

// cNation Class Definition

cNation::cNation()
{
	int i,j;
	m_Status[0]=1;  // Loop UROLL
	m_Status[1]=1;
	m_Status[2]=1;

	for (j=0;j<NW_NATION_COUNT;j++)
	for (i=0;i<MAX_MONSTER_POINT;i++)
	{
		m_tMonsterPoint[j][i].MonsterIndex=0;
		m_tMonsterPoint[j][i].Size=0;
		m_tMonsterPoint[j][i].Points=NULL;
	}

	LoadMonsterPoint();

	//< LTH-040226-KO 1.4 이후 새로운 전쟁터 상태 저장소를 초기화
	for (i = 0; i < WI_MAX_WARFIELD; ++i)
		m_btNeoStatus[i] = NW_PEACE;
	//> LTH-040226-KO
}


cNation::~cNation()   //디스트럭터만 여러번 호출이되다니... ㅜ.ㅜ 
{
	for (int j=0;j<NW_NATION_COUNT;j++)
	{
		for (int i=0;i<MAX_MONSTER_POINT;i++)
		{
			SAFE_DELETE_ARRAY(m_tMonsterPoint[j][i].Points);
		}
	}
}

int cNation::GetStatus(int WarfieldNo)
{
	if (WarfieldNo>=0&&WarfieldNo<=2)
		return m_Status[WarfieldNo];
	else
	{	//< LTH-040226-KO 1.4 패치후 신규전쟁터를 설원전쟁터와 구분짓는다
		if (!IsNeoWarfield(WarfieldNo))
			return m_NewStatus[WarfieldNo-3];
		else
			return m_btNeoStatus[WarfieldNo];
	}	//> LTH-040226-KO
}

void cNation::SetStatus(int WarfieldNo,int Status)
{
	if (WarfieldNo>=0&&WarfieldNo<=2)
		m_Status[WarfieldNo]=Status;
	else
	{	//< LTH-040226-KO 1.4 패치후 신규전쟁터를 설원전쟁터와 구분짓는다
		if (!IsNeoWarfield(WarfieldNo))
			m_NewStatus[WarfieldNo-3]=Status;
		else
			m_btNeoStatus[WarfieldNo] = Status;
	}	//> LTH-040226-KO
}

int cNation::GetNewStatus(int WarfieldNo)
{	//< LTH-040226-KO 1.4 패치후 새 전쟁터를 위해 새 분기를 만들었당
	if (!IsNeoWarfield(WarfieldNo))
	{
		if (WarfieldNo<0||WarfieldNo>=MAX_NEW_WARFIELD)
			return NW_PEACE;
		return m_NewStatus[WarfieldNo];
	}
	else
	{
		if ((WarfieldNo < WI_HADES_WARFIELD) || WarfieldNo >= WI_MAX_WARFIELD)
			return NW_PEACE;
		return m_btNeoStatus[WarfieldNo];
	}
}	//> LTH-040226-KO

void cNation::SetNewStatus(int WarfieldNo,int Status)
{	//< LTH-040130-KO hades War 때문에 if문추가
	if (!IsNeoWarfield(WarfieldNo))
	{
		if (WarfieldNo<0||WarfieldNo>=MAX_NEW_WARFIELD)
			return;
		m_NewStatus[WarfieldNo]=Status;
	}
	else
	{
		if ((WarfieldNo >= WI_HADES_WARFIELD) && (WarfieldNo < WI_MAX_WARFIELD))
			m_btNeoStatus[WarfieldNo] = Status;
	}
}	//> LTH-040130-KO

int cNation::GetWarfieldPoss(int WarfieldNo)
{
	if (WarfieldNo<0||WarfieldNo>=MAX_NEW_WARFIELD)
		return -1;
	return m_Possession[WarfieldNo];
}

void cNation::SetWarfieldPoss(int WarfieldNo,int Nation)
{
	m_Possession[WarfieldNo]=Nation;
}

bool cNation::LoadMonsterPoint()				// 수호석,성문에 의한 돈트의 처리 
{
	//추후 파일에서 읽어 온다. 
	// Nation 1______________________________________________________________________________________
	m_tMonsterPoint[0][0].MonsterIndex=17;
	m_tMonsterPoint[0][0].Size=MAP0_133;
	m_tMonsterPoint[0][0].Points=new POINTS[m_tMonsterPoint[0][0].Size];
	memcpy(m_tMonsterPoint[0][0].Points,Map0_133,sizeof(POINTS)*m_tMonsterPoint[0][0].Size);

	m_tMonsterPoint[0][1].MonsterIndex=18;
	m_tMonsterPoint[0][1].Size=MAP0_138;
	m_tMonsterPoint[0][1].Points=new POINTS[m_tMonsterPoint[0][1].Size];
	memcpy(m_tMonsterPoint[0][1].Points,Map0_138,sizeof(POINTS)*m_tMonsterPoint[0][1].Size);

	m_tMonsterPoint[0][2].MonsterIndex=19;
	m_tMonsterPoint[0][2].Size=MAP0_203;
	m_tMonsterPoint[0][2].Points=new POINTS[m_tMonsterPoint[0][2].Size];
	memcpy(m_tMonsterPoint[0][2].Points,Map0_203,sizeof(POINTS)*m_tMonsterPoint[0][2].Size);

	m_tMonsterPoint[0][3].MonsterIndex=20;
	m_tMonsterPoint[0][3].Size=MAP0_198;
	m_tMonsterPoint[0][3].Points=new POINTS[m_tMonsterPoint[0][3].Size];
	memcpy(m_tMonsterPoint[0][3].Points,Map0_198,sizeof(POINTS)*m_tMonsterPoint[0][3].Size);

	m_tMonsterPoint[0][4].MonsterIndex=41;
	m_tMonsterPoint[0][4].Size=MAP0_263;
	m_tMonsterPoint[0][4].Points=new POINTS[m_tMonsterPoint[0][4].Size];
	memcpy(m_tMonsterPoint[0][4].Points,Map0_263,sizeof(POINTS)*m_tMonsterPoint[0][4].Size);

	m_tMonsterPoint[0][5].MonsterIndex=42;
	m_tMonsterPoint[0][5].Size=MAP0_268;
	m_tMonsterPoint[0][5].Points=new POINTS[m_tMonsterPoint[0][5].Size];
	memcpy(m_tMonsterPoint[0][5].Points,Map0_268,sizeof(POINTS)*m_tMonsterPoint[0][5].Size);
	// ____________________________________________________________________________________________

	// Nation 2______________________________________________________________________________________
	m_tMonsterPoint[1][0].MonsterIndex=21;
	m_tMonsterPoint[1][0].Size=MAP1_450;
	m_tMonsterPoint[1][0].Points=new POINTS[m_tMonsterPoint[1][0].Size];
	memcpy(m_tMonsterPoint[1][0].Points,Map1_450,sizeof(POINTS)*m_tMonsterPoint[1][0].Size);

	m_tMonsterPoint[1][1].MonsterIndex=22;
	m_tMonsterPoint[1][1].Size=MAP1_527;
	m_tMonsterPoint[1][1].Points=new POINTS[m_tMonsterPoint[1][1].Size];
	memcpy(m_tMonsterPoint[1][1].Points,Map1_527,sizeof(POINTS)*m_tMonsterPoint[1][1].Size);

	m_tMonsterPoint[1][2].MonsterIndex=23;
	m_tMonsterPoint[1][2].Size=MAP1_524;
	m_tMonsterPoint[1][2].Points=new POINTS[m_tMonsterPoint[1][2].Size];
	memcpy(m_tMonsterPoint[1][2].Points,Map1_524,sizeof(POINTS)*m_tMonsterPoint[1][2].Size);
	// ____________________________________________________________________________________________

	// Nation 3______________________________________________________________________________________
	m_tMonsterPoint[2][0].MonsterIndex=39;
	m_tMonsterPoint[2][0].Size=MAP2_274;
	m_tMonsterPoint[2][0].Points=new POINTS[m_tMonsterPoint[2][0].Size];
	memcpy(m_tMonsterPoint[2][0].Points,Map2_274,sizeof(POINTS)*m_tMonsterPoint[2][0].Size);

	m_tMonsterPoint[2][1].MonsterIndex=40;
	m_tMonsterPoint[2][1].Size=MAP2_279;
	m_tMonsterPoint[2][1].Points=new POINTS[m_tMonsterPoint[2][1].Size];
	memcpy(m_tMonsterPoint[2][1].Points,Map2_279,sizeof(POINTS)*m_tMonsterPoint[2][1].Size);

	m_tMonsterPoint[2][2].MonsterIndex=37;
	m_tMonsterPoint[2][2].Size=MAP2_151;
	m_tMonsterPoint[2][2].Points=new POINTS[m_tMonsterPoint[2][2].Size];
	memcpy(m_tMonsterPoint[2][2].Points,Map2_151,sizeof(POINTS)*m_tMonsterPoint[2][2].Size);

	m_tMonsterPoint[2][3].MonsterIndex=38;
	m_tMonsterPoint[2][3].Size=MAP2_156;
	m_tMonsterPoint[2][3].Points=new POINTS[m_tMonsterPoint[2][3].Size];
	memcpy(m_tMonsterPoint[2][3].Points,Map2_156,sizeof(POINTS)*m_tMonsterPoint[2][3].Size);

	m_tMonsterPoint[2][4].MonsterIndex=35;
	m_tMonsterPoint[2][4].Size=MAP2_114;
	m_tMonsterPoint[2][4].Points=new POINTS[m_tMonsterPoint[2][4].Size];
	memcpy(m_tMonsterPoint[2][4].Points,Map2_114,sizeof(POINTS)*m_tMonsterPoint[2][4].Size);

	m_tMonsterPoint[2][5].MonsterIndex=36;
	m_tMonsterPoint[2][5].Size=MAP2_119;
	m_tMonsterPoint[2][5].Points=new POINTS[m_tMonsterPoint[2][5].Size];
	memcpy(m_tMonsterPoint[2][5].Points,Map2_119,sizeof(POINTS)*m_tMonsterPoint[2][5].Size);



	// ____________________________________________________________________________________________


	return true;

}

void cNation::SaveMonsterPoint()
{
}

void cNation::SetTileDont(int WarfieldNo,int MonNo,int type) //몬스터 SQL 필드 INDEX, 0,1 : 
{
	for (int i=0;i<MAX_MONSTER_POINT;i++)
	{
		if (m_tMonsterPoint[WarfieldNo][i].MonsterIndex==MonNo)
		{
			for (int j=0;j<m_tMonsterPoint[WarfieldNo][i].Size;j++)
			{
				TileMap[m_tMonsterPoint[WarfieldNo][i].Points[j].x][m_tMonsterPoint[WarfieldNo][i].Points[j].y].attr_dont=type;
			}
		}
	}
}


void cNation::TileDont(int WarfieldNo,int TypeNum,int Value)
{
	for (int i=0;i<MAX_MONSTER_POINT;i++)
	{
		if (m_tMonsterPoint[WarfieldNo][i].MonsterIndex==TypeNum)
		{
			SetTileDont(WarfieldNo,m_tMonsterPoint[WarfieldNo][i].MonsterIndex,Value);
		}
	}
}

CDontManager::CDontManager()
{
	m_pDontData=NULL;
	m_iSize=0;
}

CDontManager::~CDontManager()
{
	Clear();
}

void CDontManager::Clear()
{
	if (m_pDontData)
	{
		for (int i=0;i<m_iSize;i++)
		{
			SAFE_DELETE_ARRAY(m_pDontData[i].lpPoint);
		}
		SAFE_DELETE_ARRAY(m_pDontData);
	}
	m_iSize=0;
}

void CDontManager::LoadData()
{
	FILE* fp=Fopen("Data/LTS000.DAT","rb");
	if (!fp)
		return;

	Clear();

	fread(&m_iSize,sizeof(int),1,fp);

	m_pDontData=new DONTDATA[m_iSize];

	for (int i=0;i<m_iSize;i++)
	{
		fread(&m_pDontData[i].FieldSize,sizeof(int),1,fp);
		m_pDontData[i].lpPoint=new POINT[m_pDontData[i].FieldSize];
		fread(m_pDontData[i].lpPoint,sizeof(POINT),m_pDontData[i].FieldSize,fp);
	}
	return;
}

void CDontManager::ActiveDont(int DontNo,int type)
{
	if (m_pDontData==NULL)
		return;

	if (DontNo<0||DontNo>=m_iSize)
		return;

	POINT tempPoint;

	for (int i=0;i<m_pDontData[DontNo].FieldSize;i++)
	{
		tempPoint=m_pDontData[DontNo].lpPoint[i];
		if (type)
			TileMap[tempPoint.x][tempPoint.y].attr_dont=1;
		else 
			TileMap[tempPoint.x][tempPoint.y].attr_dont=0;
	}
}

void SendWarMove()
{
	int WarfieldNo=CheckNationStatus(NW_WAR);
	if (WarfieldNo==-1)
	{
		CallOkCancelMessageBox(TRUE, 0,0,lan->OutputMessage(0,299), FALSE); // 현재 전쟁중인 곳은 없습니다.
		return;
	}
	SendCMD_NW_MAP_MOVE(1,WarfieldNo);		// NewType
}

void CallMapMoveMenu(int WarfieldNo)
{
	if (WarfieldNo>=0&&WarfieldNo<=2)
	{
		SMenu[MN_WAR_MOVE].nField[0].nRectImage=WarfieldNo+1;			
		SMenu[MN_WAR_MOVE].nField[0].nImageNumber=WarfieldNo+1;		
		if (g_pNation->GetWarfieldPoss(WarfieldNo)==NW_BY)		// 방어자가 바이 
		{
			SMenu[MN_WAR_MOVE].nField[3].nRectImage=12;				// 공격자
			SMenu[MN_WAR_MOVE].nField[3].nImageNumber=12;	
			SMenu[MN_WAR_MOVE].nField[4].nRectImage=13;				// 방어자 
			SMenu[MN_WAR_MOVE].nField[4].nImageNumber=13;	
			SMenu[MN_WAR_MOVE].nField[5].nRectImage=11;				// 화살표
			SMenu[MN_WAR_MOVE].nField[5].nImageNumber=11;	
		}
		else													// 방어자가 자이
		{
			SMenu[MN_WAR_MOVE].nField[3].nRectImage=13;				// 공격자
			SMenu[MN_WAR_MOVE].nField[3].nImageNumber=13;	
			SMenu[MN_WAR_MOVE].nField[4].nRectImage=12;				// 방어자 
			SMenu[MN_WAR_MOVE].nField[4].nImageNumber=12;	
			SMenu[MN_WAR_MOVE].nField[5].nRectImage=11;				// 화살표
			SMenu[MN_WAR_MOVE].nField[5].nImageNumber=11;	
		}

		switch(WarfieldNo)
		{
		case 0 :	// 산중 
			SMenu[MN_WAR_MOVE].nField[3].x=186;
			SMenu[MN_WAR_MOVE].nField[3].y=207;
			SMenu[MN_WAR_MOVE].nField[4].x=84;
			SMenu[MN_WAR_MOVE].nField[4].y=106;
			SMenu[MN_WAR_MOVE].nField[5].x=135;
			SMenu[MN_WAR_MOVE].nField[5].y=157;
			break;
		case 2 :
			SMenu[MN_WAR_MOVE].nField[3].x=184;
			SMenu[MN_WAR_MOVE].nField[3].y=207;
			SMenu[MN_WAR_MOVE].nField[4].x=84;
			SMenu[MN_WAR_MOVE].nField[4].y=106;
			SMenu[MN_WAR_MOVE].nField[5].x=135;
			SMenu[MN_WAR_MOVE].nField[5].y=157;
			break;
		case 3 :
			SMenu[MN_WAR_MOVE].nField[3].x=186;
			SMenu[MN_WAR_MOVE].nField[3].y=207;
			SMenu[MN_WAR_MOVE].nField[4].x=84;
			SMenu[MN_WAR_MOVE].nField[4].y=106;
			SMenu[MN_WAR_MOVE].nField[5].x=135;
			SMenu[MN_WAR_MOVE].nField[5].y=157;
			break;
		}
	}
	else														// New Warfield
	{	//< LTH-040309-KO 1.4 Patch.
		//<! BBD 040311 설원과 몬스터에 따라 다르게 작동한다
		if(!IsNeoWarfield(WarfieldNo))
		{	// 설원전쟁터

			SMenu[MN_WAR_MOVE].nField[0].nRectImage=4;				// 설원전쟁터 그림으로 바꾼다.
			SMenu[MN_WAR_MOVE].nField[0].nImageNumber=4;
			SMenu[MN_WAR_MOVE].nField[3].nRectImage=12;				// 공격자
			SMenu[MN_WAR_MOVE].nField[3].nImageNumber=12;	
			SMenu[MN_WAR_MOVE].nField[3].x=161;
			SMenu[MN_WAR_MOVE].nField[3].y=97;
			SMenu[MN_WAR_MOVE].nField[4].nRectImage=13;				// 방어자 
			SMenu[MN_WAR_MOVE].nField[4].nImageNumber=13;	
			SMenu[MN_WAR_MOVE].nField[4].x=38;
			SMenu[MN_WAR_MOVE].nField[4].y=96;
			SMenu[MN_WAR_MOVE].nField[5].nRectImage=15;				// 화살표
			SMenu[MN_WAR_MOVE].nField[5].nImageNumber=15;	
			SMenu[MN_WAR_MOVE].nField[5].x=80;
			SMenu[MN_WAR_MOVE].nField[5].y=154;
		}
		else
		{	// 몬스터 전쟁터
			SMenu[MN_WAR_MOVE].nField[0].nImageNumber=16;				// 몬스터 전쟁터 그림으로 바꾼다.
			SMenu[MN_WAR_MOVE].nField[3].nType=FT_NONE;				// 공격자고 나발이고 표시 않한다
			SMenu[MN_WAR_MOVE].nField[4].nType=FT_NONE;
			SMenu[MN_WAR_MOVE].nField[5].nType=FT_NONE;
		}
		//> BBD 040311 설원과 몬스터에 따라 다르게 작동한다
	}	//> LTH-040309-KO

	CallSmallMenu(MN_WAR_MOVE);
}

void SendWarStatusRefresh()
{

}