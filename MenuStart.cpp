// menuStart.cpp

#include "stdafx.h"
#include <ddraw.h>
#include <io.h>
#include "Hangul.h"
//#include "MenuDef.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "dragon.h"
#include "MenuStartSet.h"
#include "stdio.h"
#include "convert565to555.h"
#include "directsound.h"
#include "directinput.h"
#include "Char.h"
#include "GameProc.h"
#include "Hong_Sub.h"
#include "tool.h"
#include "smallmenu.h"
#include "menunetwork.h"
#include "menuset.h"
#include "smallmenuset.h"		// 020701 YGI
#include "menudef2.h"			//020802 lsw
#include "chat.h"
#include "TextOutMgr.h"			//020828 lsw
#include "FrameMgr.h" //030703 lsw
#include "Prison.h"

extern CPrison g_cPrison;
extern void SWD_LOGON_Clicked(const int nMenuIndex);
extern bool PutTestAnimation(const int iGender, const int iClass, const DWORD dwBodyRGB, DWORD dwLineRGB );
extern bool PutTestAnimation2(const int iGender, const int iClass, const DWORD dwBodyRGB, DWORD dwLineRGB );
//010909 lsw 
extern void ResetCharacter(LPCHARACTER lpChar);
extern void ResetEffect(LPCHARACTER lpChar);
t_client_login_fail_reason g_client_login_fail_reason;		// 로그인 시 메시지

extern void StartMenuSubProcessType2(SMENU *SubMenu,int i, int j,int k);//020515 lsw
extern CBitBuf guild_mark_main;
int g_SelectServerIndex;

CMenuString		g_MenuStr[MAX_MENU_STRING];

char select_server_name[20];
extern TCHAR	g_szInfoFile[ _MAX_PATH ];
BYTE	*buf_alphabet;
Spr		spr_alphabet[36];

bool g_GoDiceRecv = 0;
bool g_GoDice = 0;
bool g_GoDice2 = 0;

//Spr credit;//020515 lsw
int BetaTest = 0;

int StartPosition ;
int StartMap;

Spr open_spr[75];
BYTE *open_spr_buf;

HANDLE ghThread; // 스레드 핸들
DWORD gdwThread; // 스레드 ID

HANDLE ghThread1; // 스레드 핸들
DWORD gdwThread1; // 스레드 ID


int		dw_TotalImageSize;
int		g_StartMenuOn;

//	Spr		ch_armor_image_befor[7][80];//020530 lsw
	Spr		ch_armor_image_after[7][80];
//	Spr		ch_body_image_befor[7][80];
	Spr		ch_body_image_after[7][80];
//	Spr		ch_line_image_befor[7][80];
	Spr		ch_line_image_after[7][80];

//Spr		ch_armor_image_befor_b[7][80];//020530 lsw
Spr		ch_armor_image_after_b[7][80];
//Spr		ch_body_image_befor_b[7][80];
Spr		ch_body_image_after_b[7][80];
//Spr		ch_line_image_befor_b[7][80];
Spr		ch_line_image_after_b[7][80];

char	*ani_buf[12];


int		character_active = 0;		// 선택된 캐릭터//020815-2 lsw
int		move_text_flag;

// BOOL RButtonDown;
BOOL LButtonDown;                      //마우스 왼쪽버튼이 눌렸었는지 안눌렸었는지 표시
SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];   //FT_HIDE_NOMAL_에 사용할 구조체 선언

SCharacter SCharacterData = {
								7, 3333, 10000,
								{"NoConnect"},
								{""},
								{0,1000,4,3,0,0,0,0,0,1,1,3},
								{10,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19},
								{10,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19},
								20000000,

								0,70,30,100,100,50,100,
								0,0,
								0,0,0,0,0L,
								0,
							};

SCharacter SCharSource={
							1,0,0,										// lv, exp
							{""},
							"None",
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},		//data
							{0, },										//STR,CON,DEX,WIS,INT,CHA,MOVP,ENDU,MOR,CONV 
																		//LUCK, WSPS,HEALTH,MOV,AC,FAME,DAMAGE,HEAT,TACTICE, money
							{0, },
							600,
						
							0,70,30,100,100,50,100,
							0,0,
							0,0,0,0,0L,
							0,
						};
SCharacter SCharSource1 = SCharSource;

SCharacter LoadCh[4];								// 서버에서 캐러 정보를 전송받는다

extern CHARACTERLIST g_CharacterList;

// int y_PutBarSize=0;              //HP나 MP를 출력할 막대 그래프의 높이를 계산한 값을 저장한다.
char y_temp[20];                 //캐릭터의 능력치 값을 출력할대 strlen을 이용해 그 길이를 미리 알기 위해서 사용하는 임시 저장 변수


int cDiceCount;							//다이스를 멈출 타이밍을 젠다, 불꽃의 프레임도 표시
int nDiceNumber[5]={1,1,1,1,1};			//다이스의 5개의 값 
//int nDiceNumber2[5]={1,1,1,1,1};
int PutDiceNumber;						//멈춘 다이스의 갯수

int y_delay=0;
int y_MenuFocus=0; //단 하나에 메뉴에만 마우스 체크를 하기위한 변수로 0일때는 모든메뉴에 마우스가 적용되고 
                 //원하는 MN_값을 넣어 그 메뉴만 작동하게 한다
char sId[30];
char sPassword[30];
 
SMENU SMenu[MAX_MENU];    //메뉴 구조체를 최대치 만큼 생성
POINT StateMousePoint;	// 드래그 시작점 기억

//배열방식 체크에 필요한 배열
BYTE CheakArray[MAX_CHEAKARRAY][16][33]=
	{
		{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
			{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,},
			{1,1,1,1,1,2,2,2,2,2,2,1,1,1,1,1,},
			{1,1,1,1,0,2,2,2,2,2,2,0,1,1,1,1,},
			{0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,},
			{0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,},
			{3,3,3,3,0,2,2,2,2,2,2,0,4,4,4,4,},
			{3,3,3,3,3,0,2,2,2,2,0,4,4,4,4,4,},
			{3,3,3,3,3,3,0,2,2,0,4,4,4,4,4,4,},
			{3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,},
			{3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,},
			{3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,},
			{3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,}
		},
		{
			{0,0,0,0,0,1,1,0,0,2,2,0,0,0,0,0,},
			{0,0,0,1,1,1,1,0,0,2,2,2,2,0,0,0,},
			{0,0,1,1,1,1,1,0,0,2,2,2,2,2,0,0,},
			{0,1,1,1,1,1,1,0,0,2,2,2,2,2,2,0,},
			{0,1,1,1,1,1,0,4,4,0,2,2,2,2,2,0,},
			{1,1,1,1,1,4,4,4,4,4,4,2,2,2,2,2,},
			{1,1,1,1,0,4,4,4,4,4,4,0,2,2,2,2,},
			{0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,},
			{0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,},
			{5,5,5,5,0,4,4,4,4,4,4,0,3,3,3,3,},
			{5,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,},
			{0,5,5,5,5,5,0,4,4,0,3,3,3,3,3,0,},
			{0,5,5,5,5,5,5,0,0,3,3,3,3,3,3,0,},
			{0,0,5,5,5,5,5,0,0,3,3,3,3,3,0,0,},
			{0,0,0,5,5,5,5,0,0,3,3,3,3,0,0,0,},
			{0,0,0,0,0,5,5,0,0,3,3,0,0,0,0,0,}
		},
		{
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,1,1,1,1,1,1,0,4,4,4,4,4,4,4,4,4,4,4,0,2,2,2,2,2,2,0,0,0,0},
			{0,0,0,1,1,1,1,1,1,1,0,4,4,4,4,4,4,4,4,4,4,4,0,2,2,2,2,2,2,2,0,0,0},
			{0,0,1,1,1,1,1,1,1,1,0,0,4,4,4,4,4,4,4,4,4,0,0,2,2,2,2,2,2,2,2,0,0},
			{0,1,1,1,1,1,1,1,1,1,1,0,0,4,4,4,4,4,4,4,0,0,2,2,2,2,2,2,2,2,2,2,0},
			{0,1,1,1,1,1,1,1,1,1,1,1,0,0,4,4,4,4,4,0,0,2,2,2,2,2,2,2,2,2,2,2,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		},

	};

	//메뉴에 사용해서 필요시 로드할 이미지의 번호를 저장하는 메뉴 끝부분은 9999로 값을 넣는다
	int aMenuImageNumber[8][MAX_MENUDATA*5]={
/* 0 */	{ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14,17, 18, 19, 20, 22, 24, 25, 26, 27, 28, 29, 30, 31,73,74,75,76,77,80, // 스타트
		 81,82,83,32, 33, 34, 64, 65, 66, 67, 68, 142,143,144,206,207,208,209,242,243,244,400,401,486,549,550,551,552,553,554,555,556,
		 557,558,503,510,511,512,513,514,515,516,517,153,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,543,544,
		 839,840,841,MAX_SYSDAT+3,MAX_SYSDAT+2, 844,845,846,847,76,77,468,469,470,471,472,473,305,306,307,308,309,877,878,879, 240,
		 906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930, 9999,},

/* 1 */	{ 21, 35, 36, 37, 69, 120,121,122,123,124,125,126,127,128,129,130,131,145,146,147,148,149,150,151,152,172,173,174,175,176,310, // 메인
		 177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,245,246,247,248,249,250,
		 251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,
		 281,282,283,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,
		 338,339,340,341,342,343,344,345,346,347,348,349,201,202,203,204,205,504,505,301,439,437,440,438,441,442,487,
		 9999,},

/* 2 */	{350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,	//메인
		 380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,456,457,458,459,460,461,462,463,464,
		 465,466,467,474,475,476,477,478,483,498,508,509,521,MAX_SYSDAT+4,MAX_SYSDAT+5,523,524,528,70,72,86,87,499,500,501,241,525,526,527,302 ,
		 502,506,507,488,489,492,493,494,495,496,497,479,480,481,878,879,880,881,882,883,884,885,886,887,888,889,890,891,
		 892,893,894,895,78,79,84,85,944,945,946,947,948,949,950,951,952,953,954,955,956,9999,},

/* 3 */	{132,133,134,135,136,137,138,139,140,141,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,		//스몰
		 231,232,233,234,235,236,237,238,239,284,286,287,288,289,290,291,292,293,294,295,300,303,304,490, 491,
		 433,434,435,436,443,444,445,446,447,448,449,450,451,452,453,454,455,586,587,588,589,590,591,592,593,594,595,596,
		 597,598,599,600,601,602,603,604,605,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,
		 629,630,631,632,633,634,635,636,637,9999,},

/* 4 */	{38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,90,484,485,0,296,297,298,299, 71,			// 공통	
		 154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,16,15,285,23,645,646,647,648,649,706,210,
		 211,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,758,941,942,943,
		 935,936,957,958,959,960,961,962,963,964,896,897,898,899,900,901,902,903,904,905,
		 9999,},

/* 5 */	{559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,638,639,640,	// 메인 추가	
		 641,642,643,644,760,761,762,763,764,765,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,
		 785,786,766,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,
		 874,875,876,638,639,640,641,642,643,644,732,733,734,735,736,737,738,931,932,933,934,1022,1023,1024,1025,1026,1027,1028,
		 1029,1030,1031,1032,1033,1034,1035,1036,1037,1038,1039, 1040, MAX_SYSDAT+1, 9999,},

/* 6 */	{402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,	//스몰 추가	
		 432,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,759,
		 675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,
		 705,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,729,730,731,
		 965,966,967,968,969,970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,
		 994,995,996,997,998,999,1000,9999,},

/* 7 */ {739,740,741,742,743,744,745,746,747,748,749,750,751,752,753,754,755,756,757,787,788,789,790,791,792,793,794,795,796,797,	//스몰 추가	
		 798,799,800,529,530,531,532,533,534,535,536,537,538,539,540,541,542,545,546,547,548,818,819,820,821,822,1001,1002,1003,1004,
		 1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,518,519,520,9999,},
	};


int menu[]={//	MN_MAINSTART_BACK,
			//	MN_LOGO_START_SAMSUNG,

				MN_MAKECHARACTER_BASIC,
				MN_MAINSTART_BASIC,
				MN_MAINSTART_START,
				MN_MAKECHARACTER_CHEAK1,
				MN_MAKECHARACTER_CHEAK3,
				MN_MAKECHARACTER_GENDER,
				MN_MAKECHARACTER_NAME,
				MN_MAKECHARACTER_FACE,
				MN_MAKECHARACTER_ARIGEMENT,
				MN_MAKECHARACTER_CLASS,
				MN_MAKECHARACTER_SPELL,
				MN_MAKECHARACTER_COLOR,
				MN_MAKECHARACTER_TACTICS_MAN,
				MN_MAKECHARACTER_TACTICS_WOMAN,
				MN_MAKECHARACTER_AGE,
				MN_MAKECHARACTER_JOB,
				MN_MAKECHARACTER_ABILITY,
				MN_RESET_JOB,
				MN_RESET_JOB_EXPLAIN,
				MN_SELECT_NATION,
				MN_NATION_EXPLAIN, 
				
				MN_LOGO_START,
				MN_LODING,
				MN_LOGIN,
				MN_LOGON,
				MN_SELECT,
				MN_LOCATION,
				MN_MESSAGEBOX_BIG,
				MN_MESSAGEBOX_SMALL,
				MN_YESNO,
				MN_SELECT_CHARACTER,
				MN_MAKECHARACTER_SPELL2,
				MN_START_MAP,

				MN_SELECT_SERVER,

				MN_INFO,
				MN_CREDIT,
				MN_OKCANCEL_MESSAGEBOX,
				MN_OK_MESSAGEBOX,

				MN_DEL_CHAR_COMFORM,//020802 lsw
				MN_SELECT_TUTORIAL_LOGIN,//020815-2 lsw

				MN_SYSTEM_LOGOUT,


							 };

const int MAX_START = (int) sizeof(menu)/sizeof(int);//020815-2 lsw

//010216 lsw
//char NationName[7][20] = { "중립", "","", "바이서스", "자이펀", "헤게모니아", "일스" };
char NationName[8][20];

char *GetNationName( int nation )
{
	return NationName[nation];
}

const DWORD NationColor[7] = { RGB(200,200,255), RGB( 200,200,255), RGB(200,200,255), RGB(0,0,255), RGB(255,0,0),  RGB(200,200,255), RGB(215,215,145) };
const WORD NationColor16[7] = { RGB16(200,200,255), RGB16( 200,200,255), RGB16(200,200,255), RGB16(0,0,255), RGB16(255,0,0),  RGB16(200,200,255), RGB16(215,215,145) };

void	StartMenuSubProcessType(SMENU *SubMenu );                //메뉴 구성요소에 타입을 판별해서 실행
void	SetCharacterData();                   //nCharacterData 변수값을 구조체들 안에 대입
int		k_PutImage(int x, int y, int nImageNumber, BOOL bEnd);		//x, y좌표로 이미지 연출
     //상위 메뉴의 x,상위 메뉴의 y,찍힐 상대적인 x,찍힐 상대적인 y,찍힐 이미지
void	FieldTypeNomalPutFx(int mx, int my, int x, int y, int nImageNumber, int t, int j);
void	FieldTypeTextPut(int mx,int my,int x,int y,char *temp);  //영어 문자열을 출력
void	PutImageCenter(int mx, int my, int x, int y, int nImageNumber); // 가운데 점으로 그림 출력
void	start_KeyProc();
char	SpaceBarPress();
void	LoadChImageNumber( char *filename, char **buf, Spr Ani[][80], int max_ani[7] );		// 캐릭터 에니메이션 스프라이트 로딩
void	LoadChImage();
void	MakeCharacterAbility();
int		ImgToAbility( int num_to_img[10], int img );				// 주사위 굴릴때, 이미지 번호로 능력치 값 찾기
void	DoLButtonDoubbleClickOfStartMenu( int i, int j );
void	SetDamageOfHero( SCharacter *ch );						//	기본 공격력 설정 
void	InitStartMenu();				// 외모 색 초기 설정을 위해 -1로 셋팅 
//void	LoadOpenningSpr( char *filename, BYTE **buf, Spr *spr );	// 020701 YGI


int CalCreateMoney( const SCharacter *s);
extern void LoadSpriteOnlyOne( Spr *s, char filename[MAX_FILENAME] );		// 스프라이트 화일 하나만 읽어 올때
extern void SetTacticsBySpell(SCharacter *ch );
extern bool bSoundOn;
extern void OpenSelectNationMenu();

char job_explain[7][6][1000];
char nation_explain[7][11][1000];
void LoadJobExplain();
void LoadNationExplain();
void SetJobExplainDetect( int job, int width, char explain[5][11][200] , WORD color[5][11]);
void SetNationExplainDetect( int nation, int width, char explain[7][11][200], WORD color[7][11] );
int CheckNationCity( int nation );

/*************************************************************************************/
void MP3( int n, int ing )
{
	SoundOn = false;
	if( n == SN_TOUCH || n == SN_TOUCH_START )
	{
		if( LButtonDown ) return;
	}
	
	DWORD	dwStatus;
	if( !g_SoundInfo.lpDirectSoundBuffer[ n ] ) 
	{
		PlayListAutoSounds( n, 0, 0, ing );
		return;
	}	
	if( IDirectSoundBuffer_GetStatus( g_SoundInfo.lpDirectSoundBuffer[ n ], &dwStatus ) == DS_OK )
	{
		if ( dwStatus & DSBSTATUS_PLAYING ) return;
		
		PlayListAutoSounds( n, 0, 0, ing );
	}
}


/*********************************함수 소스*******************************************/
void PutImageCenter(int mx, int my, int x, int y, int nImageNumber)  // 중간값으로 그림 출력, 필드만 출력 가능. 메뉴는 안돼요..
{
	if( !spr[nImageNumber].img )
	{
		LoadSmallMenuImage( nImageNumber );
	}
	PutCompressedImage(mx+x, my+y, &spr[nImageNumber]);
}

void FieldTypeNomalPut(int mx, int my, int x,int y,int nImageNumber, int type)
{
	Spr *s;
	s = GetSprOfMenu( type, nImageNumber );
	if( s ) PutCompressedImage(mx+x+s->ox, my+y+s->oy, s);
}

//010910 lsw
void FieldTypeNomalPutFx3(int mx, int my, int x,int y,int nImageNumber, int type , int t, int j)
{
	Spr *s = GetSprOfMenu( type, nImageNumber );
	if( s ) 
	{
		PutCompressedImageFX(mx+x+s->ox, my+y+s->oy, s, t, j);
	//	PutCompressedImage(mx+x+s->ox, my+y+s->oy, s);
	}
}


void FieldTypeNomalPutFx(int mx, int my, int x,int y,int nImageNumber, int t, int j)
{
	if( nImageNumber >= 10000 ) 
	{
		FieldTypeNomalPutFx2(mx, my, x, y, nImageNumber-10000, t, j );
		return;
	}
	if( !spr[nImageNumber].img ) 
	{
		LoadSmallMenuImage( nImageNumber );
	}
	PutCompressedImageFX(mx+x+spr[nImageNumber].ox, my+y+spr[nImageNumber].oy, &spr[nImageNumber], t, j);
}

//acer7
void FieldTypeTextPut(int mx,int my,int x,int y,char *temp)
{
	int i=0;
	int lenght=0;
	int code;

	 while( temp[i] != '\0' )
	 {
		 code = temp[i];
		 if( temp[i]==' ' || (temp[i] >='A' && temp[i] <='Z') )
		 {
			 if(code==32) lenght+=4;
			 else
			 {
				 code=code-'A';		// A는 0번이다.
				 Spr *s= &spr_alphabet[code];
				 PutCompressedImage( mx+x+lenght+s->ox, my+y+s->oy, s );
				 lenght+=s->xl;
			 }
		 }
		 i++;
	 }
}


//필요한 메뉴 이미지 데이타를 메모리에 로드
void LoadMenuData( int nNum )
{
	FILE *fp;
	char  temp[40];
	int i=0;
	DWORD Off=0;
	DWORD size;

	sprintf( temp, "./data/%s", "SysMenu.dat" );
	fp = Fopen( temp, "rb" );
	if( fp == NULL ) return;

	while(1)
	{
		if(aMenuImageNumber[nNum][i] == 9999) 
			break;

		if( aMenuImageNumber[nNum][i] > MAX_SYSDAT )
		{
			sprintf( temp, "./data/plus%03d.csp", aMenuImageNumber[nNum][i]-MAX_SYSDAT );
			LoadSpriteOnlyOne( &spr[aMenuImageNumber[nNum][i]], temp );
		}
		else 
		{
      if( spr[ aMenuImageNumber[nNum][i]].img )
			{
				int a = 0;
			}
			else
			{
				fseek(fp, 4 * aMenuImageNumber[nNum][i] + 2, SEEK_SET);
				fread( &Off, sizeof( DWORD ), 1, fp);
				fseek(fp, Off, SEEK_SET);

				fread( &size, sizeof( DWORD ), 1, fp);
				fread( &spr[ aMenuImageNumber[nNum][i] ].xl, sizeof( short ), 1, fp);
				fread( &spr[ aMenuImageNumber[nNum][i] ].yl, sizeof( short ), 1, fp);
				fread( &spr[ aMenuImageNumber[nNum][i] ].ox, sizeof( short ), 1, fp);
				fread( &spr[ aMenuImageNumber[nNum][i] ].oy, sizeof( short ), 1, fp);
				fread( &spr[ aMenuImageNumber[nNum][i] ].size, sizeof( unsigned int ), 1, fp);
				
				MemAlloc(spr[ aMenuImageNumber[nNum][i] ].img,spr[ aMenuImageNumber[nNum][i] ].size);
				fread( spr[ aMenuImageNumber[nNum][i] ].img, spr[ aMenuImageNumber[nNum][i] ].size, 1, fp);
				convert565to555( &spr[ aMenuImageNumber[nNum][i]] );
			}
		}
		dw_TotalImageSize += spr[ aMenuImageNumber[nNum][i] ].size;
		i++;
	}
	fclose(fp);
}

//필요 없는 메뉴 이미지 메모리를 반환
void FreeMenuData( int nNum )
{	
	int i=0;
	while(1)
	{
		if(aMenuImageNumber[nNum][i] == 9999) 
			break;
		if( spr[aMenuImageNumber[nNum][i]].img )
		{
			MemFree( spr[aMenuImageNumber[nNum][i]].img );
			dw_TotalImageSize -= spr[aMenuImageNumber[nNum][i]].size;
		}
		spr[aMenuImageNumber[nNum][i]].img = NULL;


		i++;
	}
}



//변경시 int GetRectTextLine을 맞 춰 줄 수 있어야 합니다 
//######################################################################################
//######################################################################################
void kein_TextPut(int x, int y, char *sSource)  //  " ` "이 있을 경우의 출력
{
	int i=0, count[20], j=1; // \n의 개수를 구해서 그 위치를 저장한다.
	int start=0;	

	count[0]=0;

//	Hcolor( FONT_COLOR_DEFAULT );
	while((unsigned int)i < strlen(sSource))
	{
		if((*(sSource + i))=='`') { count[j]=i; j++; }
		i++;
	}
	count[j]=i;			// 마지막 처리
		
	char temp[500];

	if(j==1) Hprint(x, y, g_DestBackBuf, sSource);
	else 
	{
		for(i=1;i<=j;i++)
		{
			start=count[i-1];
			memset(temp,' ',500);
			memcpy(temp,(start)?sSource+start+1:sSource,count[i]-count[i-1]-1);
			Hprint(x,y+(i-1)*15, g_DestBackBuf,temp);
		}
	}
}

//############################# 글씨가 떠오른데요~ ##########################
void MoveTextPut(int x, int y, char *sSource, int type)		 // 다 떠오르고 나서의 처리가 필요
{

	int i=0, count[20], j=1; 
	int start=0;	
	int row;
	char temp[1000];


	int y_source = y;
	static int delay = 0; 
	static int skip_count = 0;
	static int skip = 0; 
	static int y_plus=y;

	if( type )
	{
		delay = 0;
		skip_count = 0;
		skip = 0;
		y_plus=y_source;
		move_text_flag = 0;
	}

	count[0]=0;
	while((unsigned int)i < strlen(sSource))
	{
		if((*(sSource + i))=='`') { count[j]=i; j++; }
		i++;
	}
	count[j]=i;			// 마지막 처리
	row = j;			
	if(j > 20) j = 20;		//  20줄 이상은 처리하지 않는다.
	
	y = y_plus;
	delay++;
	static int limits = 4;
	if( delay >  limits )
	{
		y--;
		delay = 0;
		skip_count++;
		if( y+skip*20 < 140 /*y_source - (FONTSIZE_+skip)*(skip+1)*/)
		{
			skip++;
			if(j < row ) j++;
		}
	}
	y_plus = y;

//	HangulOutputArea( 100, 450, 200, 400 );
	if(j==1) Hprint(x, y, g_DestBackBuf, sSource);
	else 
	{
		for(i=1;i<=j;i++)
		{
			start=count[i-1];
			memset(temp,' ',1000);
			memcpy(temp,(start)?sSource+start+1:sSource,count[i]-count[i-1]-1);
			if( i > skip )
			{
				if( y+(i-1)*20 < 420 ) Hprint(x,y+(i-1)*20, g_DestBackBuf, temp);
			}
		}
	}
	switch(SpaceBarPress())
	{
		case '0' :	break;
		case 'r' :	delay = 0;
					skip_count = 0;
					skip = 0;
					y_plus=y_source;
					break;
		case 'p' :	limits++; break;
		case 'm' :  limits--; break;
	}
	Hprint2(5 , 40 , g_DestBackBuf,"%d", limits);
//	HangulOutputArea( 0, 639, 0, 479 );
}

//##############################  MENU CALL ####################################//
void CallMenu(int Menu, int x, int y, BOOL bMenuFocus)
{
	SMenu[Menu].bActive = TRUE;
	SMenu[Menu].x=x+GABX_SCREEN;
	SMenu[Menu].y=y;
	if(bMenuFocus) y_MenuFocus = Menu;
	MP3( SN_MENU_OPEN );
	SoundOn = false;
	SetChatMode(CM_COMMAND,true);//021001 lsw //인터페이스 뜨면 무조건 채팅창 닫는다
	EWndMgr.ClearAllTxt();//021001 lsw
}

void CallMenu( int Menu, int temp, bool is_focus )
{
	if(Menu == MN_MERCHANT_FIND)
	{
//		::AddCurrentStatusMessage(255,0,0,"아직 상인아이템 구입은 되지 않습니다.");
//		return; //soto-030511 추가..임시 상인검색 종료후 지워야 함.
	}

	if(Menu != MN_FIGHT_MAP_SIGN && Menu != MN_EXCHANGE_ITEM && Menu != MN_MAN_TO_MAN)
		/* To prevent arenamenu events closing chat, e.g. joining a team */
	{
		SetChatMode(CM_COMMAND,true);//021001 lsw //인터페이스 뜨면 무조건 채팅창 닫는다
		EWndMgr.ClearAllTxt();//021001 lsw
	}
	
	if( SMenu[Menu].bActive ) return;
	SoundOn = false;
	SMenu[Menu].bActive = TRUE;
	SMenu[Menu].nTemp = temp;
	switch( Menu )
	{
		case MN_YESNO				:
		case MN_LEVELUP				:
		case MN_STATUS				:
		case MN_ABILITY				:
		case MN_ITEM				:
		case MN_ARIGEMENT			: MP3( SN_MENU_OPEN); break;

		
		case MN_PARTY_FACE1			:
		case MN_PARTY_FACE2			:
		case MN_PARTY_FACE3			:
		case MN_PARTY_FACE4			:
		case MN_PARTY_FACE5			:
		case MN_PARTY_FACE6			:

		
		case MN_AWAKE				:	break;

		case MN_GOD_EDELBLHOY		:
		case MN_GOD_TEFFERY			:
		case MN_GOD_OREM			:
		case MN_GOD_LETTY			:	MP3( SN_MENU_OPEN ); break;

		case MN_DISTANCE_TABLE		:
		case MN_STORE_SIGN			:	MP3( SN_MENU_OPEN ); break;

		case MN_DIVIDE_ITEM			:	MP3( SN_BOX_OPEN ); break;

		case MN_ITEM_EXPLAN			:	
		case MN_MAGICBOOK           :	

		case MN_OKCANCEL_MESSAGEBOX	:	
		case MN_OK_MESSAGEBOX		:	
		case MN_MAGIC_BOOK			:	
		case MN_LEARN_MAGIC_BOOK	:	
		case MN_MEM_BOOK			:
		case MN_SMALL_MAP			:
		case MN_TIME_DISPLAY_MENU	:
		case MN_QUICK_MAGIC_SCROLL	:
		case MN_QUICK_MAGIC_PLUS	:
		case MN_TIME_DISPLAY_ON_OFF	:
		case MN_START_MAP			:
		case MN_QUICK_SKILL			:
		case MN_HUNTER_MAIN			:
		case MN_HUNTER_REGIST		:
		case MN_HUNTER_LIST			:
		case MN_QUEST				:
		case MN_FAME				:
		case MN_QUEST_INDEX			:
		case MN_RELATION			:
		case MN_MAN_TO_MAN			:

		default : MP3( SN_MENU_OPEN ); break;
	}

	if( is_focus ) y_MenuFocus = Menu;
	
}

//################################################################################//

void CallItemMessageBox( int menu, int item_num, char *source )
{
	y_MenuFocus = MN_ITEM_MESSAGEBOX;
	SMenu[MN_ITEM_MESSAGEBOX].nTemp = menu;
	SMenu[MN_ITEM_MESSAGEBOX].bActive = true;
	SMenu[MN_ITEM_MESSAGEBOX].nField[1].nWillDo = item_num;
	strcpy(SMenu[MN_ITEM_MESSAGEBOX].nField[1].temp, source);
	MP3( SN_MENU_OPEN );
}

void CallOkCancelMessageBox( int menu, int x, int y, const char *source, bool ok_cancel, int param )		// ok만 있는 건지 ok_cancel이 있는 메시지 박스인지를 선택,  menu는 호출한 메뉴를 넣는다
{
	x +=GABX_SCREEN;

	if( ok_cancel )
	{
		if( g_StartMenuOn )
			SMenu[MN_OKCANCEL_MESSAGEBOX].nTemp = menu;
		else SMenu[MN_OKCANCEL_MESSAGEBOX].nTemp = y_MenuFocus;
		y_MenuFocus = MN_OKCANCEL_MESSAGEBOX;
		SMenu[MN_OKCANCEL_MESSAGEBOX].bActive = true;
		SMenu[MN_OKCANCEL_MESSAGEBOX].x = x;
		SMenu[MN_OKCANCEL_MESSAGEBOX].y = y;
		strcpy(SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].temp, source);
		
		SMenu[MN_OKCANCEL_MESSAGEBOX].key = param;
	}
	else
	{
		if( g_StartMenuOn )
			SMenu[MN_OK_MESSAGEBOX].nTemp = menu;
		else SMenu[MN_OK_MESSAGEBOX].nTemp = y_MenuFocus;
		y_MenuFocus = MN_OK_MESSAGEBOX;
		SMenu[MN_OK_MESSAGEBOX].bActive = true;
		SMenu[MN_OK_MESSAGEBOX].x = x;
		SMenu[MN_OK_MESSAGEBOX].y = y;
		strcpy(SMenu[MN_OK_MESSAGEBOX].nField[3].temp, source);
		SMenu[MN_OK_MESSAGEBOX].key = param;
	}
	MP3( SN_WARNING );
}

//##############################  MessageBox CALL ################################//
void CallMessageBox(BOOL bBig, int x, int y, char *source, BOOL bType ) // 큰 메시지 박스?, x,y 좌표, 메시지, 타이핑 효과 연출?
{

	x +=GABX_SCREEN;
	y +=GABY_SCREEN;

	if(bBig)		// BigMessageBox Call
	{
		SMenu[MN_MESSAGEBOX_BIG].bActive = TRUE;
		SMenu[MN_MESSAGEBOX_BIG].x = x;
		SMenu[MN_MESSAGEBOX_BIG].y = y;
		SMenu[MN_MESSAGEBOX_BIG].nField[0].x = 30;
		SMenu[MN_MESSAGEBOX_BIG].nField[0].y = 35;
		strcpy(SMenu[MN_MESSAGEBOX_BIG].nField[0].temp, source);
	}
	else 
	{
		SMenu[MN_MESSAGEBOX_SMALL].bActive = TRUE;
		SMenu[MN_MESSAGEBOX_SMALL].x = x;
		SMenu[MN_MESSAGEBOX_SMALL].y = y;
		SMenu[MN_MESSAGEBOX_SMALL].nField[0].x = 27;
		SMenu[MN_MESSAGEBOX_SMALL].nField[0].y = 27;
		strcpy(SMenu[MN_MESSAGEBOX_SMALL].nField[0].temp, source);
	}
}
//################################################################################//


void SetCharacterData()
{
	int i,j;

	for(i=0;i<MAX_SHN;i++)
	{
		for(j=0;j<MAX_SHN_FIELD;j++)
		{
			SHideNomal[i][j].bShow=FALSE;
		}
	}
	for(i=0;i<MAX_CHARACTERDATA;i++)
	{
		if(SCharSource.nCharacterData[i]!=-1) SHideNomal[i][SCharSource.nCharacterData[i]].bShow=TRUE;
	}
}

void StartMenuSetting()
{
	int i;
	
	
	SCharSource = SCharSource1;
	InitStartMenu();
	g_StartMenuOn = true;
	character_active = 0;
	
	for(i=0;i<MAX_START;i++)  //일단 모든 메뉴 구조체들을 비활성으로 설정
	{
		SMenu[menu[i]].bActive=FALSE;
		SMenu[menu[i]].Id = menu[i];
	}

	StartMenuSet();
	CommonMenuSet();  //크기가 작은 메뉴들을 초기화
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver ) LoadCh[0] = SCharacterData;
#endif
	LoadHangulEnglishFont( "./data/han.fnt", "./data/eng.fnt" );
	HangulOutputArea( 0, SCREEN_WIDTH-1, 0, SCREEN_HEIGHT-1);

	SCharSource.nCharacterData[FACE] = -1;		// 처음 설정에 -1;
	QuitFlag = 0;

#ifdef LIGHT_VERSION_
	g_pLightVersion->LoadLightVersionWeapon();
#endif

}


inline BOOL MouseInRectCheak(int x, int y, RECT Box, BOOL flag)
{
	POINT pointMouse;
	if(flag) pointMouse = g_pointMouse;
	else pointMouse = StateMousePoint;

	if(pointMouse.x > Box.left+x  
	&& pointMouse.x < Box.right+x  
	&& pointMouse.y >Box.top+y  
	&& pointMouse.y <Box.bottom+y)
	{
		return TRUE;
	}
	return FALSE;
}

//######################################################################################################//
// 메시지등을 출력하는 함수.
// thai2 YGI
void MessagePut()
{	//< CSD-030324
	Hcolor( FONT_COLOR_NUMBER );
	static int x=99, y=357+GABY_SCREEN;
	if(SMenu[MN_MESSAGEBOX_BIG].bActive)
	{
#ifdef THAI_LOCALIZING_
	RectTextPut( SMenu[MN_MESSAGEBOX_BIG].x+SMenu[MN_MESSAGEBOX_BIG].nField[0].x+20, SMenu[MN_MESSAGEBOX_BIG].y+SMenu[MN_MESSAGEBOX_BIG].nField[0].y+20, 200, SMenu[MN_MESSAGEBOX_BIG].nField[0].temp );
#else
	kein_TextPut(SMenu[MN_MESSAGEBOX_BIG].x+SMenu[MN_MESSAGEBOX_BIG].nField[0].x, SMenu[MN_MESSAGEBOX_BIG].y+SMenu[MN_MESSAGEBOX_BIG].nField[0].y, SMenu[MN_MESSAGEBOX_BIG].nField[0].temp);
#endif

		if(LButtonDown || y_delay > 30 ) 
			if(!SMenu[MN_YESNO].bActive) 
				CallMenu(MN_YESNO,x,y,TRUE);		// 메뉴 번호는 MN_YESNO이고 마우스는 독점이다 */
		y_delay++;
	}
	else if(SMenu[MN_MESSAGEBOX_SMALL].bActive)
	{
		kein_TextPut(SMenu[MN_MESSAGEBOX_SMALL].x+SMenu[MN_MESSAGEBOX_SMALL].nField[0].x, SMenu[MN_MESSAGEBOX_SMALL].y+SMenu[MN_MESSAGEBOX_SMALL].nField[0].y, SMenu[MN_MESSAGEBOX_SMALL].nField[0].temp);
		if(LButtonDown || y_delay > 30)  CallMenu(MN_YESNO,x,y,TRUE);
			if(!SMenu[MN_YESNO].bActive) 
				CallMenu(MN_YESNO,x,y,TRUE);		// 메뉴 번호는 MN_YESNO이고 마우스는 독점이다
		y_delay++;
	}
}	//> CSD-030324


//################################ display image little-by-little ####################################//
// 서서히 나타나는 이미지.	완전히 다 나오게 하려면 bEnd에 TRUE를 넣는다.
int k_PutImage(int x, int y, int nImageNumber, BOOL bEnd)
{
	static int rx=1;

	if( !spr[nImageNumber].img ) 
	{
		LoadSmallMenuImage( nImageNumber );
	}

	if(SMenu[MN_MAINSTART_BACK].nTemp)	rx=1;
	SMenu[MN_MAINSTART_BACK].nTemp = FALSE;
	if(bEnd) rx=39;
	if(2*rx>x && 2*rx+1 < spr[nImageNumber].xl)
	{
		PutCmprsImgCliping(x,  y, spr[nImageNumber].img, g_DestBackBuf, 0, 16*rx+1, 0, (int)(12*(float)rx)+1);
	}
	rx=(rx+1)%321;
	
	return rx;
}

//#########################################################################################################//
//메뉴가 활성화 되있느냐를 판단해서 출력해 주는 함수
extern void PutStartLodingImg( );

#ifdef _DEBUG
extern int g_MyCode;//021001 lsw
#endif

void StartMenuDisplay()//020828 lsw
{
	int i,x; 
	for(x=0;x<MAX_START;x++)
	{
		i=menu[x];
		if(SMenu[i].bActive)   //i번째 메뉴가 활성화 되있냐?
		{
			//< kjy-04325
			if ( i == 44 )				// 서버 선택 화면에서 배경을 refresh한다.. 
				PutStartLodingImg();	// 마우스 포인터 잔상 효과 방지..  040325 kjy
			//> kjy-04325 
			
			UpperMenuNomalPut(SMenu[i].x, SMenu[i].y, SMenu[i].nImageNumber, SMenu[i].nImageType );
			StartMenuSubProcessType( &SMenu[i] );
		}
	}
	MessagePut();
#ifdef _DEBUG
	if( GetSysInfo( SI_GAME_MAKE_MODE ) ) 
	{
		Hcolor( FONT_COLOR_WHITE );
		SetHangulAlign( TA_LEFT );
		const int iX = 5, iY = 5;
		const int iWidth = TXT_BG_SHORT_WIDTH;//021001 lsw
		int iCount = 0;
		int iReultLineCount = 0;
		
		int iBlankGab = 0;
		int iReultBlankSize = 0;
		
		const int iLineGab = TxtOut.GetLineGab();
		const int iBlankGabSize = TxtOut.GetBlankGabSize();

		TxtOut.Init(COLOR_BLACK,TXT_BG_SHORT_WIDTH,12);
		TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"FPS:[%d]",g_FrameMgr.GetFrameCount() );
		iCount += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"MyCode:[%d]",g_MyCode );
		iCount += iReultLineCount;
		
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,BetaTest? "Beta Tester" : "Normal User" );
		iCount += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"Start MapNo:[%d]", StartPosition );
		iCount += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"Menu Focus:[%d]", y_MenuFocus );
		iCount += iReultLineCount;

		iReultBlankSize = TxtOut.RcBlankOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1);
		iBlankGab+=iReultBlankSize;

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"Proxy IP:");
		iCount += iReultLineCount;
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"[%s]", g_GameInfo.proxy_ip[0] );
		iCount += iReultLineCount;

		iReultBlankSize = TxtOut.RcBlankOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1);
		iBlankGab+=iReultBlankSize;

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"ServerSet Name:");
		iCount += iReultLineCount;
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"[%s]", select_server_name);
		iCount += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1);
		iBlankGab+=iReultBlankSize;

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"Character LV:[%d]", SCharacterData.nLevel);
		iCount += iReultLineCount;

		TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);

	}

	if( SysInfo.notconectserver )
	{
		if( g_SetGuildMarkViewOn )
		{
			EraseScreen( curr_direct_draw_info, RGB( 0x00, 0x00, 0x00 ) );
			if( ( g_SetGuildMarkViewOn-1 )*40 > g_GuildMax ) g_SetGuildMarkViewOn= 1;

			for( int i=(g_SetGuildMarkViewOn-1)*40; i<=g_GuildMax && i<(g_SetGuildMarkViewOn)*40; i++ )
			{
				int x = 200+(i%40)/7*80;
				int y = 80+(i%40)%7*50;

				PutGuildImage( x, y, i );
			}

			if(g_aCurrentKeys[DIK_PGDN] & 0x80)
			{
				g_SetGuildMarkViewOn ++;
			}
			if(g_aCurrentKeys[DIK_PGUP] & 0x80)
			{
				g_SetGuildMarkViewOn --;
				if( g_SetGuildMarkViewOn <= 0 ) g_SetGuildMarkViewOn =1;
			}
		}
	}

	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		if( guild_mark_main.m_Spr.img )
			PutCompressedImage( 100, 100, &guild_mark_main.m_Spr );
	}
#endif _DEBUG
	return;
}


//기본 베이스 메뉴를 출력
inline void UpperMenuNomalPut(int &x, int y,int nImageNumber, int type)
{
	if( !nImageNumber && !type ) return;

	Spr *s;
	s = GetSprOfMenu( type, nImageNumber );
	if( s )
	{
		if( x == CENTER_X )
			x = (SCREEN_WIDTH-s->xl)/2;
		if( x== RIGHT_X)
			x = SCREEN_WIDTH-s->xl;
		
		//<< 031022 kyo
		if( type == NEW_EMOTE_IMG )
		{
			PutCompressedImageFX( x+s->ox, y+s->oy, s, 20, 1);
		}
		else
		{
			PutCompressedImage( x+s->ox, y+s->oy, s );
		}
		//>> 031022 kyo
	}
}

//###########################################################################################//
//###########################################################################################//
//###########################################################################################//
//###########################################################################################//
//마우스상황에 따른 메뉴의 처리


void StartMenuChecking()
{
	int i,j,k,x;
	static bool b_DoubleClickOn = false;

	for(x=0;x<MAX_START;x++)
	{
		i=menu[x];
		if(SMenu[i].bActive)   //i번째 메뉴가 활성화 되있냐?
		{
			if( y_MenuFocus && y_MenuFocus != i ) continue;

			for(j=0; j<SMenu[i].nFieldCount;j++)	// rect 형
			{
//				if( i == MN_SELECT_CHARACTER ) 
//				{
//					if( j >=0 && j < 4 )	SMenu[i].CheakType =3;
//					else SMenu[i].CheakType = 0;
//				}

				switch(SMenu[i].CheakType)
				{
					default :   	{
									if(MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))	SMenu[i].nField[j].fRectMouse=TRUE;
									else	SMenu[i].nField[j].fRectMouse=false;
									
									if(SMenu[i].nField[j].nType==FT_HIDE_NOMAL_ONE_PUT)
									{
										for(k=SMenu[i].nField[j].nSHideNomalStart; k<(SMenu[i].nField[j].nSHideNomalStart)+(SMenu[i].nField[j].nShideNomalPlus) && k<SMenu[i].nField[j].nSHideNomalCount; k++)
										{
											if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].rCheakBox))
													SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].bRect=true;
											else SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].bRect=false;
										}
									}
								}
								break;
					case 1 :											// 배열형
					case 2 :{
								int ArrayX=(g_pointMouseX - (SMenu[i].x + 31) )/10;
								int ArrayY=(g_pointMouseY - (SMenu[i].y + 31) )/10;
								if( (ArrayX >= 0) && (ArrayX <= 15)  &&  (ArrayY >=0) && (ArrayY <=15) ) // 마우스를 이용해 구한 좌표가 배열에 들어가냐?
								{
									if( ( (CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) == j ) )
											SMenu[i].nField[j].fRectMouse = true;
									else 
									{
										SMenu[i].nField[j].fRectMouse=false;
										SMenu[i].nField[j].fLButtonDown=false;
										SMenu[i].nField[j].fCheakFlag=false;
									}
								}
								else	
								{
										SMenu[i].nField[j].fRectMouse=false;
										SMenu[i].nField[j].fLButtonDown=false;
								//		SMenu[i].nField[j].fCheakFlag=false;
								}
								break;
							}
//					case 3:	{
//								int ArrayX=(g_pointMouseX - (SMenu[i].x + 202) )/8;
//								int ArrayY=(g_pointMouseY - (SMenu[i].y + 390) )/13;
//								if( (ArrayX >= 0) && (ArrayX <= 15)  &&  (ArrayY >=0) && (ArrayY <=15) ) // 마우스를 이용해 구한 좌표가 배열에 들어가냐?
//								{
//									if( ( (CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) == j ) )
//											SMenu[i].nField[j].fRectMouse = true;
//									else 
//									{
//										SMenu[i].nField[j].fRectMouse=false;
//										SMenu[i].nField[j].fLButtonDown=false;
//										SMenu[i].nField[j].fCheakFlag=false;
///									}
//								}
//								else	
//								{
//										SMenu[i].nField[j].fRectMouse=false;
//										SMenu[i].nField[j].fLButtonDown=false;
//								//		SMenu[i].nField[j].fCheakFlag=false;
//								}
//								break;
//							}
				}
			}
		}
		else
		{
			for(j=0;j<SMenu[i].nFieldCount;j++)
			{
				SMenu[i].nField[j].fRectMouse=false;
			}
		}
	}

	if( g_nLDButtonState == STATE_BUTTON_DOUBLECLICK )
	{
		for(x=0;x<MAX_START;x++)
		{
			i=menu[x];
			if(SMenu[i].bActive) 
			{	
				if( y_MenuFocus && y_MenuFocus != i ) continue;
				for(j=0; j<SMenu[i].nFieldCount;j++)  
				{
					if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
					{
						DoLButtonDoubbleClickOfStartMenu(i, j);
						b_DoubleClickOn = true;
						return;
					}
					if(SMenu[i].nField[j].nType==FT_HIDE_NOMAL_ONE_PUT)
					{
						for(k=SMenu[i].nField[j].nSHideNomalStart; k<(SMenu[i].nField[j].nSHideNomalStart)+(SMenu[i].nField[j].nShideNomalPlus) && k<SMenu[i].nField[j].nSHideNomalCount; k++)
						{
							if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].rCheakBox))
							{
									DoLButtonDoubbleClickOfStartMenu( i, j );
//									MouseDrag = true;		// 이건 다음에 클릭의 영향을 주지 않기 위해서이다.. 너무하당~
									b_DoubleClickOn = true;
									return;
							}
						}
					}
				}
			}
		}
	}

	if(g_nLButtonState==STATE_BUTTON_PRESSED)
	{
		LButtonDown=TRUE;
		StateMousePoint = g_pointMouse;
	}
	if(g_nLButtonState==STATE_BUTTON_STILL && LButtonDown==TRUE)   // 일단 마우스가 어디선가 방금 눌렸다. 그게 범위 안에서 눌렸는지, 아닌지 판단 부분
	{
		MouseDrag = true;
		for(x=0;x<MAX_START;x++)
		{
			i=menu[x];
			if(SMenu[i].bActive) 
			{	
				if( y_MenuFocus && y_MenuFocus != i ) continue;
				switch(SMenu[i].CheakType)
				{
				case 0 :    //RECT형 체크 타입
					for(j=0; j<SMenu[i].nFieldCount;j++)  //메뉴의 구성요소들을 검사
					{
						if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) && 
							MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox, FALSE))  //마우스가 메뉴구성 요소의 rect안에서 L버튼이 눌렸냐
						{	
							SMenu[i].nField[j].fLButtonDown=TRUE;  //그러면 눌렸다고 표시
						}
						else
						{
							SMenu[i].nField[j].fLButtonDown=FALSE;
						}
					}

					break;

				case 1 :    //배열번호 1번으로 체크
				case 2 :    //배열번호 2번으로 체크

					int ArrayX=(g_pointMouseX - (SMenu[i].x + 31) )/10; //10메뉴가 10픽셀씩 나눠서 배열화 시켰다 (16*16)
					int ArrayY=(g_pointMouseY - (SMenu[i].y + 31) )/10;
					int StateArrayX = (StateMousePoint.x - (SMenu[i].x + 31) )/10;
					int StateArrayY = (StateMousePoint.y - (SMenu[i].y + 31) )/10;

					for(j=0;j<SMenu[i].nFieldCount;j++)
					{
						if(SMenu[i].nField[j].fLButtonDown)  //이 메뉴요소가 눌렸었냐?
						{
							if( (ArrayX >= 0) && (ArrayX <= 15)  &&  (ArrayY >=0) && (ArrayY <=15) ) // 마우스를 이용해 구한 좌표가 배열에 들어가냐?
							{
								if( ( (CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) != j ) || 
									( (CheakArray[SMenu[i].CheakType - 1][StateArrayY][StateArrayX] - 1) != j ) )   //마우스가 왼쪽버튼으로 선택했던 메뉴구성요소에서 빠져나갔냐?
								{
									SMenu[i].nField[j].fLButtonDown = FALSE;
								}
							}
							else
							{
								SMenu[i].nField[j].fLButtonDown=FALSE;   
							}
						}
						else
						{
							if( (ArrayX >= 0) && (ArrayX <= 15) && (ArrayY >=0) && (ArrayY <=15) &&   //처음 왼쪽버튼으로 눌린 이 메뉴구성 요소에 마우스가 제대로 선택했냐?
								(StateArrayX >= 0) && (StateArrayX <= 15) && (StateArrayY >=0) && (StateArrayY <=15) )
							{
								if( ( (CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) == j )  &&
									( (CheakArray[SMenu[i].CheakType - 1][StateArrayY][StateArrayX] - 1) == j ) )   //마우스가 왼쪽버튼으로 선택했던 메뉴구성요소에서 빠져나갔냐?
												SMenu[i].nField[(CheakArray[ SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1)].fLButtonDown = TRUE;
							}
						}
					}
				}
			}
		}
	}
	else if(g_nLButtonState==STATE_BUTTON_RELEASED) //마우스가 방금 떨어진 상태냐?
	{
		bSoundOn = false;
		if( b_DoubleClickOn ) 
		{
			b_DoubleClickOn = false;
			return;
		}
		LButtonDown=FALSE;
		for(x=0;x<MAX_START;x++)
		{
			bool f_ClickOn = false;		// 클릭하면 더이상 메뉴 참조는 없다.
			i=menu[x];
			if(SMenu[i].bActive)   //i번째 메뉴가 활성화 되있냐?
			{
				if( y_MenuFocus && y_MenuFocus != i ) continue;
				switch(SMenu[i].CheakType) //활성화된 메뉴의 체크 타입이 뭐냐?
				{
				case 0 :    //RECT형 체크 타입
					for(j=0; j<SMenu[i].nFieldCount;j++)  //메뉴의 구성요소들을 검사
					{
						if(SMenu[i].nField[j].nType==FT_HIDE_NOMAL_ONE_PUT)
						{
							for(k=SMenu[i].nField[j].nSHideNomalStart; k<(SMenu[i].nField[j].nSHideNomalStart)+(SMenu[i].nField[j].nShideNomalPlus) && k<SMenu[i].nField[j].nSHideNomalCount; k++)
							{
								if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].rCheakBox))
								{
									SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].bShow=TRUE;
									if(SMenu[MN_MAKECHARACTER_BASIC].bActive) 
									{
										SCharSource.nCharacterData[SMenu[i].nField[j].nSHideNomalNumber]=k;
										MP3(SN_SELECT_START);
									}
								}
								else
								{
									SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].bShow=FALSE;
								}
							}
						}
						else if( !MouseDrag || SMenu[i].nField[j].fLButtonDown )
						{
							if( MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
							{
								SMenu[i].nField[j].fLButtonDown=FALSE;
							    SMenu[i].nField[j].fCheakFlag=TRUE;
							}
							else
							{
								SMenu[i].nField[j].fLButtonDown=FALSE;
							}
						}
					}
					break;

				case 1 :    
				case 2 :	int ArrayX=(g_pointMouseX - (SMenu[i].x + 31) )/10;
							int ArrayY=(g_pointMouseY - (SMenu[i].y + 31) )/10;
							for(j=0;j<SMenu[i].nFieldCount;j++)
							{
								if(!MouseDrag || SMenu[i].nField[j].fLButtonDown)
								{
									SMenu[i].nField[j].fLButtonDown=FALSE;
									if( ( (ArrayX >= 0) && (ArrayX <= 15) ) && ( (ArrayY >=0) && (ArrayY <=15) ) )
									{
										if( (CheakArray[ SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) == j )
										{
											SMenu[i].nField[j].fCheakFlag=TRUE;
//											f_ClickOn = true;	// 체크 하나에 클릭하나
										}
									}
								}
							}
							break;
				}
			}
			if( f_ClickOn ) break;
		}
		MouseDrag = false;
	}
}



/**************************************************************************************/
//활성화된 메뉴의 구성요소들을 수행
//출력을 하는 부분
/**************************************************************************************/
void StartMenuSubProcessType(SMENU *SubMenu )
{
	int explain_start_map = 0;
	int i = 0, j = 0, k = 0;
	int code;
	int lenght;	
	int LenghtTotal;
	static int nCursor_Flicker=0;   // 깜박이는 커서를 위한 변수
	BOOL flag=TRUE;					// int & ability=nWeapon_Type;		// 주인공 능력치 출력을 위한 임시 변수
//	static id_password = TRUE;		// 아이디 패스워드 칸 왔다 갔다하기...
	int cursor1=0, cursor2=0;		// 깜박이는 커서 임시 저장
	char sPassword_temp[15]="";

	// 에디터 박스 죽이는 부분.... 아직 두 군데서만 쓰인다...
	if((!SMenu[MN_LOGIN].bActive) && (!SMenu[MN_MAKECHARACTER_NAME].bActive)) 
	{
		if(!SMenu[MN_DEL_CHAR_COMFORM].bActive)//캐릭 삭제 암호 부분이 꺼져 있으면
		{
			EWndMgr.ClearAllTxt();//021001 lsw
			SetFocus2( HWND_GAME_MAIN );//021001 lsw
		}
	}
	SetCharacterData();
	start_KeyProc();

	for(j=0;j<SubMenu->nFieldCount;j++)   //메뉴에 구성요소들을 수행해라
	{
		Hcolor( FONT_COLOR_NUMBER );
		SetHangulAlign( TA_LEFT );
		switch(SubMenu->nField[j].nType)  //구성요소가 어떤 타입이지?
		{
		case FT_NOMAL_PUT :     //이미지만 찍는 구성요소
//					if( SubMenu->Id == MN_MAINSTART_BACK ) FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, 15,1);
//					else 
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
					
						if( !SubMenu->key) 
						{
							if( *SubMenu->nField[j].temp )
							{
								Hcolor( FONT_COLOR_NAME );
								static int old_field = j;
								static int count = 0;
								if( old_field != j )
								{
									old_field = j;
									count = 0;
								}

								if( !count ) MP3( SN_MENU_OPEN );

								int curr_line = count / 30;
								int max_line = GetLineRectTextPut( 170, SubMenu->nField[j].temp );
								if( max_line ) 
								{
									if( max_line == 2 )
									{
										RectTextPut(SubMenu->x+SubMenu->nField[j].x+13,SubMenu->y+SubMenu->nField[j].y+14,170,SubMenu->nField[j].temp); // 설명문을 쓴다 
									}
									else 
									{
										curr_line %= max_line;
										SkipRectTextPut(SubMenu->x+SubMenu->nField[j].x+13,SubMenu->y+SubMenu->nField[j].y+14,170, curr_line, SubMenu->nField[j].temp, 2, 0); // 설명문을 쓴다 
									}
									count++;
								}								
							}
						}
					SubMenu->nField[j].fLButtonDown = false;
					SubMenu->nField[j].fCheakFlag = false;
					break;

		case FT_NOMAL_PUT_FX :	FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, 8, 3);
								break;

		case FT_NOMAL_PUT_CENTER_1 :
					if( !spr[SubMenu->nField[j].nImageNumber].img ) 
					{
						LoadSmallMenuImage( SubMenu->nField[j].nImageNumber );
					}
					lenght= (spr[SubMenu->nImageNumber].xl - spr[SubMenu->nField[j].nImageNumber].xl)/2;
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, lenght+1, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
					break;
		case FT_SWITCH_PUT : {
								static int stch=0;
								static int de=3;
								FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, de/3, 2);

								if(stch > 19) de--;
								else de++;
								stch++;
								stch %= 40;
							 }	

		case FT_SELECT_SERVER_SCROLL : //020828 lsw
			{
				const int willdo = SubMenu->nField[j].nWillDo;
				const int max_server = SubMenu->work;
				int &curr_page = SubMenu->key;
				const int max_page = (max_server-1)/5 + 1;
				switch(willdo)
				{
				case SCROLL_UP:
					{
						if( curr_page <= 0  ) break;//0 페이지 보다 작을 수는 없다
						if( SubMenu->nField[j].fCheakFlag)
						{
							curr_page--;
							SubMenu->nField[j].fCheakFlag = 0;
							break;
						}
					}break;
				case SCROLL_DOWN:
					{
						if( max_page <= curr_page+1  ) break;
						if( SubMenu->nField[j].fCheakFlag)
						{
							curr_page++;
							SubMenu->nField[j].fCheakFlag = 0;
							break;
						}
					}break;
				default:
					{
					}break;
				}

				int target_number;
				if( SubMenu->nField[j].fLButtonDown )
				{
					target_number = SubMenu->nField[j].nImageNumber;
				}
				else if( SubMenu->nField[j].fRectMouse )
				{
					target_number = SubMenu->nField[j].nRectImage;
				}
				else
				{
					target_number = SubMenu->nField[j].nSpecialWillDo;
				}
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, target_number, SubMenu->nField[j].nImageType );
			}break;

		case FT_HIDE_AUTO_PUT :	if(SubMenu->nField[j].fRectMouse)
								{
//									if( !bSoundOn ) { MP3( SN_TOUCH_START ); bSoundOn = true; }
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
								}
		case FT_HIDE_PUT_AUTO :
		case FT_HIDE_PUT :    //마우스 이벤트를 체크해 안보이던 이미지가 보이는 구성요소
						    if(SubMenu->nField[j].fLButtonDown)
								FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
							break;

		case FT_SELECT_SERVER_BUTTON : {
											int willdo = SubMenu->nField[j].nWillDo;
											int key = SubMenu->key;		// 스크롤 되어 있는 상태인지..
											SetHangulAlign( TA_CENTER);

											if( SubMenu->nField[j].fCheakFlag )
											{
												// ip 선택
												g_SelectServerIndex = key*5+willdo;

												strcpy( select_server_name,  server_set[g_SelectServerIndex].server_name );
												strcpy( g_GameInfo.proxy_ip[0], server_set[g_SelectServerIndex].server_ip1 );
												strcpy( g_GameInfo.proxy_ip[1], server_set[g_SelectServerIndex].server_ip2 );
	                                            //memset(g_GameInfo.proxy_ip[0],0,sizeof (g_GameInfo.proxy_ip[0]));
	                                            //strcpy(g_GameInfo.proxy_ip[0],"210.51.13.39");
												//memset(g_GameInfo.proxy_ip[1],0,sizeof (g_GameInfo.proxy_ip[1]));
	                                            //strcpy(g_GameInfo.proxy_ip[1],"210.51.13.39");
											//	SMenu[MN_MAINSTART_BACK].bActive=true;//020515 lsw
											//	SMenu[MN_LOGIN].bActive=TRUE;
												MP3( SN_MENU_OPEN );
												::CallLoginMenu(true);
												SubMenu->nField[j].fCheakFlag = false;
												SubMenu->bActive = false;
												break;
											}

											if( SubMenu->nField[j].fLButtonDown )
											{
												Hcolor( FONT_COLOR_PLUS );
											}
											else if( SubMenu->nField[j].fRectMouse )
											{
												Hcolor( FONT_COLOR_SPECIAL );
											}
											else
											{
												Hcolor( FONT_COLOR_NAME );
											}

											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%s", server_set[key*5+willdo].server_name );
											break;
									   }

		case FT_HIDE_WILLDO_AUTO_PUT :	{
												int flag = FALSE;
												int sound = 0;
												if( SubMenu->nField[j].fCheakFlag )
												{
													switch(SubMenu->nField[j].nWillDo)
													{
														case MN_MAKECHARACTER_CLASS :	SCharSource.nCharacterData[ARIGEMENT] = 4;
																						SCharSource.age = 18;
																						break;

														case MN_MAKECHARACTER_NAME	:	if(SCharSource.nCharacterData[GENDER] == -1) 
																						{
																							flag = TRUE;
																							sound = 1;
																						}
																						break;
														case MN_MAKECHARACTER_AGE	:	if(SCharSource.nCharacterData[ARIGEMENT] == -1) 
																						{
																							flag = TRUE;
																							sound = 1;
																						}
																						break;
														case MN_MAKECHARACTER_FACE  :	if( *SCharSource.sCharacterName )
																						{
																							if( SearchStrStr( SCharSource.sCharacterName, " ") ) 
																							{
																								CallOkCancelMessageBox( MN_MAKECHARACTER_NAME, 0,0,lan->OutputMessage(5,61), 0 );//lsw
																								SetFocus2(HWND_3);//021001 lsw
																								flag = true;
																								sound = 1;
																								break;
																							}

																							if( CheckCharName( SCharSource.sCharacterName ) ) 
																							{
																								CallOkCancelMessageBox( MN_MAKECHARACTER_NAME, 0,0, lan->OutputMessage(5,62), 0 );//lsw
																								SetFocus2(HWND_3);//021001 lsw
																								flag = TRUE;
																								sound = 1;
																							}
																						}
																						else flag = true;
																						break;
														case MN_MAKECHARACTER_COLOR :	if(SCharSource.nCharacterData[CLASS] == -1 || SCharSource.nCharacterData[SPELL] == -1) 
																						{
																							sound = 1;
																							flag = TRUE;
																						}
																						SetTacticsBySpell( &SCharSource );
																						break;
														case MN_MAKECHARACTER_JOB	:	
															{
																if(SCharSource.nCharacterData[CLASS] == -1 || SCharSource.nCharacterData[SPELL] == -1) //020725 lsw
																{
																	sound = 1;
																	flag = TRUE;
																}
																if( SCharSource.nCharacterData[GENDER] )
																{
																	SCharSource.body_r = 204;
																	SCharSource.body_g = 146;
																	SCharSource.body_b = 118;
																	SCharSource.nCharacterData[TACTICS_MAN] = 1;
																}
																else
																{
																	SCharSource.body_r = 237;
																	SCharSource.body_g = 183;
																	SCharSource.body_b = 156;
																	SCharSource.nCharacterData[TACTICS_WOMAN] = 1;
																}
																if(SCharSource.nCharacterData[GENDER] && SCharSource.nCharacterData[TACTICS_MAN] == -1)  
																{
																	flag = TRUE;
																	sound = 1;
																}
																else if(SCharSource.nCharacterData[GENDER]==0 && SCharSource.nCharacterData[TACTICS_WOMAN] == -1) flag = TRUE;
																break;
															}
														case MN_MAKECHARACTER_ARIGEMENT : // SCharSource.nCharacterData[FACE]=SCharSource.nCharacterData[GENDER]*1000 + 0;
																						break; //얼굴 이미지 넣어주기
													}
												}
												if(flag)
												{
													SubMenu->nField[j].fCheakFlag = FALSE;
													SubMenu->nField[j].fLButtonDown = FALSE;
													SubMenu->nField[j].fRectMouse= FALSE;
												}
												else if( sound ) MP3( SN_WARNING );
												


												if(SubMenu->nField[j].fRectMouse)
												{
//													if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
													FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
												}
										}

		case FT_HIDE_WILLDO_PUT : 	
			{
				if(SubMenu->nField[j].fRectMouse ||
					SubMenu->nField[j].fLButtonDown)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
				}
			}

		case FT_WILLDO_PUT :
			{

			if(SubMenu->nField[j].fCheakFlag)
				{
					SubMenu->bActive=FALSE;  //자신은 사라짐
					SMenu[SubMenu->nField[j].nWillDo].bActive=TRUE;  //다른메뉴 활성화
					MP3( SN_MENU_OPEN );

					switch( SubMenu->nField[j].nWillDo )
					{
						case MN_LODING :{	SMenu[MN_SELECT].bActive=FALSE;
											SMenu[MN_LOCATION].bActive=FALSE;
											if( LoadCh[ character_active ].sCharacterName[0] )
											{ 
												SendChoiceCharacter( character_active );
											} 
											break;
										}
						case MN_CREDIT :
						case MN_INFO :	//	SMenu[MN_MAINSTART_BACK].bActive = false;
											SMenu[MN_LOGIN].bActive = false;//020515 lsw
											SMenu[SubMenu->nField[j].nWillDo].bActive = true;//020515 lsw
											move_text_flag = 1;
											break;
					}
					SubMenu->nField[j].fCheakFlag=FALSE;
					SubMenu->nField[j].fLButtonDown=FALSE;
				}
			}break;

		case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2 :	{
														if( CheckNationCity( SubMenu->nField[j].nWillDo ) == NOT	) break;

														int &tt = SubMenu->nField[j].nSHideNomalStart;
														int &de = SubMenu->nField[j].nShideNomalPlus;
														if( tt ) {	de ++;	if( de > 15 ) { tt = !tt; }	}
														else { de --; if( de < 2 ) { tt = !tt; } } 

														Spr *s = GetSprOfMenu(SubMenu->nField[j].nImageType, SubMenu->nField[j].nRectImage );
														if( s )
															PutCompressedImageFX(SubMenu->x+SubMenu->nField[j].x+s->ox, SubMenu->y+SubMenu->nField[j].y+s->oy, s ,de, 2);
													}
		case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT :	
			if( SubMenu->nField[j].fRectMouse )
			{
//			   if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
				if( SubMenu->nField[j].nRectImage==30 && SubMenu->nField[j].nImageType == START_ETC)
				{
					static int tt;
					static int de = 3;
					if( tt ) {	de ++;	if( de > 9 ) { tt = !tt; }	}
					else { de --; if( de < 2 ) { tt = !tt; } } 

					Spr *s = &start_etc_spr[SubMenu->nField[j].nRectImage];
					PutCompressedImageFX(SubMenu->x+SubMenu->nField[j].x+s->ox, SubMenu->y+SubMenu->nField[j].y+s->oy, s ,de, 2);
				}
				else
				{
					if( SubMenu->Id == MN_START_MAP )
					{//020828 lsw
						Hcolor( FONT_COLOR_WHITE );
						SetHangulAlign( TA_LEFT );
						const int iX = 620;
						const int iY = 363;
						
						const int iWidth = TXT_BG_SHORT_WIDTH;
						int iCount = 0;
						int iReultLineCount = 0;
						
						int iBlankGab = 0;
						int iReultBlankSize = 0;
						
						const int iLineGab = TxtOut.GetLineGab();
						const int iBlankGabSize = TxtOut.GetBlankGabSize();

						TxtOut.Init(COLOR_BLACK,TXT_BG_SHORT_WIDTH);
						explain_start_map = 1;
						switch(SubMenu->nField[j].nSpecialWillDo)
						{
						case SWD_CONNECT:
							{
								TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
								iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,lan->OutputMessage(5,(!SubMenu->work)?63:65));
								iCount += iReultLineCount;
								TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
							}break;
						case SWD_CONNECT_RECENT:
							{
								TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
#ifdef LIGHT_VERSION_
								iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,kein_GetMenuString( 208 ));
#else
								iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,lan->OutputMessage(5,64));
#endif
								iCount += iReultLineCount;
								TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
							}break;
						default:
							{
								if( SubMenu->nField[j].nWillDo == MN_SELECT_CHARACTER )
								{
									TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
									iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,lan->OutputMessage(5,66));//lsw
									iCount += iReultLineCount;
									TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
								}
								else 
								{
									explain_start_map = 0;
								}
							}break;
						}
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
					}
					else 
					{
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
					}

				}
			}
			else if( SubMenu->Id == MN_START_MAP && !explain_start_map )
			{
				if( SubMenu->nField[j].nWillDo == MN_SELECT_CHARACTER ) 
				{//020828 lsw
					Hcolor( FONT_COLOR_WHITE );
					SetHangulAlign( TA_LEFT );
					const int iX = 620;
					const int iY = 363;
					
					const int iWidth = TXT_BG_SHORT_WIDTH;
					int iCount = 0;
					int iReultLineCount = 0;
					
					int iBlankGab = 0;
					int iReultBlankSize = 0;
					
					const int iLineGab = TxtOut.GetLineGab();
					const int iBlankGabSize = TxtOut.GetBlankGabSize();

					TxtOut.Init(COLOR_BLACK,TXT_BG_SHORT_WIDTH);
					
					TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
					iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,lan->OutputMessage(5,67));
					iCount += iReultLineCount;
					TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
				}
			}
		case FT_HIDE_SPECIAL_WILLDO_PUT : 
								{
									int flag = FALSE;
									switch(SubMenu->nField[j].nWillDo)
									{
										case MN_MAKECHARACTER_CLASS :	if(SCharSource.nCharacterData[CLASS] == -1)  flag = TRUE;
																		break;
										case MN_MAKECHARACTER_JOB :		if(SCharSource.nCharacterData[JOB] == -1)	flag = TRUE;
																		break;
									}
									if(flag)	goto Label_2;
									if(SubMenu->nField[j].fLButtonDown)
									{
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
									}
									if( SubMenu->nField[j].nType == FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2 && SubMenu->nField[j].nSHideNomalCount )
									{
										if( SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fLButtonDown )
										{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+20, SubMenu->nField[j].y+20, SubMenu->nField[j].nSHideNomalCount, SubMenu->nField[j].nImageType);
										}
									}
								}
		case FT_SPECIAL_WILLDO :
			{
				if((SubMenu->nField[j].fRectMouse ||
					SubMenu->nField[j].fLButtonDown )&& SubMenu->nField[j].nType != FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
				}
				if(SubMenu->nField[j].fCheakFlag)
				{
					switch(SubMenu->nField[j].nSpecialWillDo)
					{
					case SWD_DELETE_CHARATER:
						{
							char szKeyCode[51] = {0,};
							EWndMgr.GetTxt( HWND_1, szKeyCode, 51);//021014 lsw//한글자 더 늘려랏

							if(TRUE == SendDeleteCharacter( LoadCh[ character_active ].sCharacterName,szKeyCode))
							{
								memset( &LoadCh[ character_active ], 0, sizeof( SCharacter ) );
							}
							SCharSource = LoadCh[ character_active ];
						}
					case SWD_DELETE_CHARATER_CLOSE:
						{
							SubMenu->bActive = false;
							y_MenuFocus=0;
						}break;
						case SWD_LOGON_OK:	if(*SCharSource.sCharacterName)			// 이름이 없다면 밑의 case문 실행->캐러 새로 만들기
											{
												if( SubMenu->Id == MN_RESET_JOB )
												{
													if( SCharSource.nCharacterData[JOB] == -1 ) goto Label_2;
													char temp[255];
													sprintf( temp, lan->OutputMessage(5,68), SHideNomal[HN_MAKECHARACTER_JOB_TEXT][SCharSource.nCharacterData[JOB]].temp );//lsw
													CallOkCancelMessageBox( SubMenu->Id, 0,0,temp );
													goto Label_2;
												}
												else if( SubMenu->Id == MN_SELECT_NATION )
												{
													if( SubMenu->nField[j].nWillDo == 0 )	
													{
														SubMenu->work = 0;		// 중립국 선택
														goto Label_2;
													}
													char temp[255];
													switch( SubMenu->work )
													{
														case 0		: goto Label_2;
														case 100	: goto Label_2;
															//sprintf( temp, "중립을 선택하시겠습니까?" ); 
															//break;
														case N_VYSEUS	:
#ifdef KOREA_LOCALIZING_
															{
																sprintf( temp, lan->OutputMessage(3,300), NationName[SubMenu->work] ); //lsw
																CallOkCancelMessageBox( SubMenu->Id, 0, 0, temp );
															}break;
#endif
														case N_ZYPERN	:
														case N_HEGEMONIA:
														case N_YILSE	:	
															sprintf( temp, lan->OutputMessage(5,69), NationName[SubMenu->work] ); //lsw
															break;
														default		: goto Label_2;
													}
													for( int a=0; a<4; a++ ) LoadCh[a].nation = SubMenu->work;
													SCharSource.nation = SubMenu->work;
													CallOkCancelMessageBox( SubMenu->Id, 0,0,temp );
													goto Label_2;
												}
											}
						case SWD_CHARACTER_DEL_ADD :	//020802 lsw
							{
								if( *SCharSource.sCharacterName ) // delete
								{
									MP3( SN_WARNING );
									char temp[255];
									sprintf( temp,lan->OutputMessage(5,70), SCharSource.sCharacterName );//lsw
									CallOkCancelMessageBox( SubMenu->Id, 0,0,temp );
									goto Label_2;
								}
							}
						case SWD_RESET_JOB :
											if(*SCharSource.sCharacterName)
											{
												if( SCharSource.nCharacterData[JOB] >= 20 )
												{
													////////// 캐릭터 초기화 //////////////
													if( SCharSource.nation )
													{
#ifdef _DEBUG // finito 060507
														if( !SysInfo.notconectserver )
#endif
														{
															int ret = SendCheckNewChar( SCharSource.sCharacterName );
															switch( ret )
															{
																default :	CallOkCancelMessageBox( SubMenu->Id, 0,0, lan->OutputMessage(5,71), 0 ); goto Label_2;//lsw
																case -4 :	CallOkCancelMessageBox( SubMenu->Id, 0,0, lan->OutputMessage(5,72), 0 ); goto Label_2;//lsw
																case  1 :	break;
															}
														}
														//::CallMN_START_MAP_Menu();
														if (true == g_cPrison.IsBadUser())
														{
															if (LoadCh[character_active].sCharacterName[0])
															{
																StartPosition = 99;// 최근 로그아웃한 장소
																SMenu[MN_LODING].bActive = true;
																SubMenu->bActive = false;
																SendChoiceCharacter(character_active);
															}
														}
														else
														{
															::CallMN_START_MAP_Menu();
														}
														goto Label_2;
													}
													else	// 나라 선택 메뉴띄우기
													{
														OpenSelectNationMenu();
														break;
													}
												}
												else
												{
													SCharSource.nCharacterData[JOB] = -1;
													SMenu[MN_MAKECHARACTER_BASIC].bActive=TRUE;
													SMenu[MN_RESET_JOB].bActive = true;
													SMenu[MN_RESET_JOB_EXPLAIN].bActive = true;
													MP3( SN_MENU_OPEN );
													break;
												}
												break;
											}
						//메인 스타트 메뉴에서 캐릭터 만들기 메뉴로 들어갈때
						case SWD_ENTER_MAKECHARACTER :{
														SMenu[MN_MAKECHARACTER_ABILITY].nField[1].nSHideNomalStart=0;   // 데이터 초기화 작업
														SCharSource=SCharSource1;										//       "
														nCursor_Flicker = 0;
														for(k=0;k<MAX_START;k++) SMenu[menu[k]].bActive = FALSE ;				// 먼저 모든 화면을 닫는다.
														SMenu[MN_MAKECHARACTER_BASIC].bActive=TRUE;
														SMenu[MN_MAKECHARACTER_CHEAK1].bActive=TRUE;
//														SMenu[MN_MAKECHARACTER_CHEAK1].x=SMenu[MN_MAKECHARACTER_CHEAK1].nField[MAX_FIELD-1].x;
//														SMenu[MN_MAKECHARACTER_CHEAK1].y=SMenu[MN_MAKECHARACTER_CHEAK1].nField[MAX_FIELD-1].y;
														SMenu[MN_MAKECHARACTER_GENDER].bActive=TRUE;
														MP3( SN_MENU_OPEN );
														break;
													}
						 //기본적인 캐릭터 만들기가 끝나고 주사위를 굴려서 능력치를 설정하러 들어갈때
						case SWD_MAKECHARACTER_ABILITY :
															if( SCharSource.nCharacterData[ SPELL ] ) // case priest 
															{
																SMenu[MN_MAKECHARACTER_CHEAK3].nField[11].nType=FT_NO_CHECK;
															}
															else		// case wizard
															{
																SMenu[MN_MAKECHARACTER_CHEAK3].nField[11].nType=FT_NOMAL_PUT;
															}
															SMenu[MN_MAKECHARACTER_CHEAK1].bActive=FALSE;
															SMenu[MN_MAKECHARACTER_CHEAK3].bActive=TRUE;
															SMenu[MN_MAKECHARACTER_ABILITY].bActive=TRUE;
															SCharSource.nCharacterData[JOB] += 20;
															MakeCharacterAbility();
															MP3( SN_MENU_OPEN );
															break;
							 //캐릭터 이름을 쓰는 메뉴로 갈때
						case SWD_MAKECHARACTER_ENTERNAME ://SMenu[MN_MAKECHARACTER_NAME].bActive=TRUE;
															if( SCharSource.body_r == -1 )
															{
																if( SCharSource.nCharacterData[GENDER] )
																{
																	SCharSource.body_r = 204;
																	SCharSource.body_g = 146;
																	SCharSource.body_b = 118;
																}
																else
																{
																	SCharSource.body_r = 237;
																	SCharSource.body_g = 183;
																	SCharSource.body_b = 156;
																}
															}

							 								SetFocus2(HWND_3);//021001 lsw
															goto Label_2 ;

						 //클래스를 결정하는 메뉴에서 스펠이나 색을 결정하는 메뉴로 갈때
						 case SWD_MAKECHARACTER_SPELLORCOLOR :
																	//선택한 클래스가 스펠을 정해야 하냐 안정해도 되냐?
																if(SCharSource.nCharacterData[CLASS] == WIZARD)				//클래스가위자드냐?
																{
																	SCharSource.nCharacterData[SPELL] = WIZARD_SPELL;
																	SMenu[MN_MAKECHARACTER_SPELL2].bActive=TRUE;
																	SMenu[MN_MAKECHARACTER_SPELL2].nField[2].nType=FT_NOMAL_PUT_CENTER_1;
																	SMenu[MN_MAKECHARACTER_SPELL2].nField[3].nType=FT_NO_CHECK;
																}
																else if(SCharSource.nCharacterData[CLASS] == PRIEST)			//클래스가 프리스트냐?
																{
																	SCharSource.nCharacterData[SPELL] = PRIEST_SPELL;
																	SMenu[MN_MAKECHARACTER_SPELL2].bActive=TRUE;
																	SMenu[MN_MAKECHARACTER_SPELL2].nField[2].nType=FT_NO_CHECK;
																	SMenu[MN_MAKECHARACTER_SPELL2].nField[3].nType=FT_NOMAL_PUT_CENTER_1;
																}
																else SMenu[MN_MAKECHARACTER_SPELL].bActive=TRUE;		//아니면 색설정 메뉴호출
																MP3( SN_MENU_OPEN );
																break;

								 //색을 결정하는 메뉴에서 택틱스를 결정하는 메뉴로 갈때
						 case SWD_MAKECHARACTER_TACTICS :{
															if( SHideNomal[HN_MAKECHARACTER_COLOR_SELECT][0].bShow
																||	SHideNomal[HN_MAKECHARACTER_COLOR_SELECT][1].bShow )
															{
																if( SCharSource.nCharacterData[GENDER] )   //남자냐?
																	SMenu[MN_MAKECHARACTER_TACTICS_MAN].bActive=TRUE;
																else  SMenu[MN_MAKECHARACTER_TACTICS_WOMAN].bActive=TRUE;			//여자면
									 							MP3( SN_MENU_OPEN );	 
																break;
															}
															goto Label_2;
														 }

						case SWD_YESNO_NO:		y_MenuFocus=0;
												for(k=0;k<MAX_START;k++) SMenu[menu[k]].bActive=FALSE;
												y_delay = 0;		// 빌려쓴 변수 돌려 줘야죠?
												MakeCharacterAbility();		// 다이스로 만든 능력 다시 초기화...

//												SMenu[MN_MAKECHARACTER_ABILITY].nField[1].nSHideNomalStart=0;
												SMenu[MN_MAKECHARACTER_BASIC].bActive=TRUE;		//능력치 설정으로 돌아가기
												SMenu[MN_MAKECHARACTER_CHEAK3].bActive=TRUE;
												SMenu[MN_MAKECHARACTER_ABILITY].bActive=TRUE;
												break;

						case SWD_YESNO_YES :{
												for(k=0;k<MAX_START;k++) SMenu[menu[k]].bActive=FALSE;
												y_delay = 0;		// 빌려쓴 변수 돌려 줘야죠?

												for( k=STR; k<=LUCK; k++)
												{
													SCharSource.nCharacterAbility[k] += SCharSource.nAbilityMax[k];
													SCharSource.nAbilityMax[k]=0;
												}
												SCharSource.nMoney = CalCreateMoney( &SCharSource );
												
												SCharSource.nation = 0;
												for( k=0; k<4; k++ )
												{
													if( LoadCh[k].sCharacterName[0] && LoadCh[k].nation )
													{
														SCharSource.nation = LoadCh[k].nation;		// 나라는 따라간다..
														break;
													}
												}
												SCharacterData = SCharSource;
												int ret = SendCreateCharacter( &SCharacterData );
												if( ret == -1 ) // CMD_INVALID_DB
												{
													CallOkCancelMessageBox( 0,0,0, kein_GetMenuString( 0 ) );
												}
												else
												{
													LoadCh[ character_active ] = SCharacterData;
												}

												SMenu[MN_SELECT_CHARACTER].bActive=TRUE;
												MP3( SN_MENU_OPEN );
												y_MenuFocus=0;
												break;
											}

						case SWD_CALL_OTHER :	if( SubMenu->nField[j].nWillDo==MN_SELECT && SMenu[SubMenu->nField[j].nWillDo].bActive )  
												{
													SubMenu->nField[0].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
													SubMenu->nField[3].nType=FT_HIDE_WILLDO_AUTO_PUT;
													SMenu[SubMenu->nField[j].nWillDo].bActive=FALSE;
													SMenu[MN_LOCATION].bActive=TRUE;
													SubMenu->nField[j].nType=FT_NO_CHECK;
													MP3( SN_MENU_OPEN );
												}
												if( SubMenu->nField[j].nWillDo==MN_LOCATION && SMenu[SubMenu->nField[j].nWillDo].bActive ) 
												{
													SMenu[SubMenu->nField[j].nWillDo].bActive=FALSE;
													SubMenu->nField[j].nType=FT_NO_CHECK;
												}
												goto Label_2 ;

						case  SWD_IDEDITBOX:	
							{
								SetFocus2(HWND_1);//021001 lsw
								id_password = true;
								cursor2 = nCursor_Flicker;
								nCursor_Flicker = cursor1;
								goto Label_2;
							}break;
						case  SWD_PASSEDITBOX:	
							{
								SetFocus2(HWND_2);//021001 lsw
								id_password = false;
								cursor1=nCursor_Flicker;
								nCursor_Flicker= cursor2;
								goto Label_2; 
							}break;
						case SWD_WILLDO:		
							{
								SMenu[SubMenu->nField[j].nWillDo].bActive = true;
								MP3( SN_MENU_OPEN );
							}break;

						case SWD_IDPASS :	
							{//021001 lsw
								sId[0]			= 0;
								sPassword[0]	= 0;
								SubMenu->bActive=false;//누른놈은 죽인다//020515 lsw
								MP3( SN_MENU_OPEN );
#ifndef _DEBUG1
	#ifdef	JAPAN_LOCALIZING_//일본은 Back 누르면 종료
								::ExitApplication(EA_NORMAL);
	#endif
#endif
								::CallLoginMenu();
							}break;

						case SWD_MAKECHARACTER_GENDERBACK :		for(k=0;k<MAX_START;k++) 
																{
																	SMenu[menu[k]].bActive=FALSE;
																	SMenu[menu[k]].key = 0;		// 설명을 다시 찍기 위해
																}
																//SMenu[MN_MAINSTART_BACK].bActive=TRUE;
																SMenu[MN_SELECT_CHARACTER].bActive=TRUE;
																SCharSource = SCharSource1;  // 다시 초기화
																break;

						case SWD_RE_LOGON :	SCharSource = LoadCh[ character_active ];
											SMenu[SubMenu->nField[j].nWillDo].bActive=TRUE;
										//	SMenu[MN_MAINSTART_BACK].bActive=false;
											SMenu[MN_LOGIN].bActive=false;//020515 lsw											
											SMenu[MN_START_MAP].bActive=false;
											y_MenuFocus=0;//020815-2 lsw
											break;

						case SWD_LOGON :
							{	//021021 lsw
								//g_bLogOnFlag = false;//010909 lsw 접속전에 로그온 플래그를 죽여 준다
								::SWD_LOGON_Clicked(SubMenu->Id);
								goto Label_2;
							}break;
						case SWD_CHARACTER_SELECT : 
							{
								for( int a=4; a<=7; a++ )
								{														
									SubMenu->nField[a].nType= FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
								}
								SubMenu->nField[j].nType= FT_NOMAL_PUT;
								if ( SubMenu->nField[j+4].nSHideNomalStart /* == turn */)		// 선택화면을 계속 진행하고 있는쭝...
								{
									SubMenu->nField[j+4].nSHideNomalCount=1;
								}
								character_active = SubMenu->nField[j].nWillDo;
								SCharSource = LoadCh[ character_active ];
								MP3( SN_SELECT_START );
								goto Label_2;
							}
						case SWD_SELECT_LOCATION : {
														;
														StartPosition = SubMenu->nField[j].nWillDo;
														SMenu[MN_MAINSTART_START].nField[0].fCheakFlag = true;
														goto Label_2;
														break;
												   }

						case SWD_SMALL_MENU :	SMenu[SubMenu->nField[j].nWillDo].bActive = true;
												y_MenuFocus = SubMenu->nField[j].nWillDo;
												goto Label_2;

				case SWD_QUIT:
				{ 	
					QuitFlag = SWD_QUIT;
					//MP3( SN_GAMEEND );
					GameEndFree();
					goto Label_2;
				}

						case SWD_OKCANCEL_CANCEL : if( SubMenu->nTemp == MN_START_MAP )
												   {
														;
														SMenu[MN_START_MAP].nField[9].fCheakFlag = true;
												   }
						case SWD_SMALLMENU_END : MP3( SN_MENU_CLOSE ); y_MenuFocus=0; break;
						case SWD_ONLY_OK:	{
												if( SubMenu->nTemp == MN_SELECT_CHARACTER )	// 캐릭터 지우기 메시지와 겹침을 방지하기 위해...
												{
													MP3( SN_MENU_CLOSE ); 
													y_MenuFocus = 0; 
													break;
												}
												/*
												if( SubMenu->nTemp == MN_START_MAP )
												{
													MP3( SN_MENU_OPEN );
													StartPosition = SC_SCHOLIUM;
													SMenu[MN_LODING].bActive = true;
													SendChoiceCharacter( character_active );
													SMenu[SubMenu->nTemp].bActive = false;
													break;
												}*/
											}
						case SWD_OKCANCEL_OK : {
													MP3( SN_MENU_CLOSE );
													SubMenu->bActive = false;//020515 lsw
													switch( SubMenu->nTemp )
													{
														case MN_SELECT_NATION : 
															{
																if( !SCharSource.nation ) break;
																if( SCharSource.nation == 100 )
																{
																	SCharSource.nation = 0;
																}
																else 
																{
																	SendSelectNation( SCharSource.nation, SCharSource.sCharacterName );
																}

																SMenu[MN_MAKECHARACTER_BASIC].bActive=false;
																SMenu[MN_RESET_JOB_EXPLAIN].bActive=false;
																//::CallMN_START_MAP_Menu();
																if (true == g_cPrison.IsBadUser())
																{
																}
																else
																{
																	::CallMN_START_MAP_Menu();
																}
																goto Label_2;
															}
														case MN_RESET_JOB : 
															{
																////////// 캐릭터 초기화 //////////////
																/*int ret;
																if( !SysInfo.notconectserver )
																{
																	bool fBreak = false;
																	ret = SendCheckNewChar( SCharSource.sCharacterName );
																	switch( ret )
																	{
																		default :	CallOkCancelMessageBox( MN_LOGIN, 0,0, lan->OutputMessage(5,71), 0 ); //lsw
																					fBreak = true;
																		case -4 :	CallOkCancelMessageBox( MN_LOGIN, 0,0, lan->OutputMessage(5,72), 0 );//lsw
																					fBreak = true;
																		case  1 :	break;
																	}

																	if( fBreak )
																	{
																		SMenu[MN_MAKECHARACTER_BASIC].bActive=false;
																		SMenu[MN_RESET_JOB_EXPLAIN].bActive=false;
																		SMenu[MN_RESET_JOB].bActive = false;
 																		::CallLoginMenu();
																		break;
																	}
																} finito 060507 - removed this check is not needed anymore as there wont be any alpha characters...*/
																SCharSource.nCharacterData[JOB]+= 20;
																SendResetJob( SCharSource.nCharacterData[JOB], SCharSource.nCharacterData[CLASS], SCharSource.sCharacterName );

																if( SCharSource.nation )
																{
																	SMenu[MN_MAKECHARACTER_BASIC].bActive=false;
																	SMenu[MN_RESET_JOB_EXPLAIN].bActive=false;
																	//::CallMN_START_MAP_Menu();
																	if (true == g_cPrison.IsBadUser())
																	{
																	}
																	else
																	{
																		::CallMN_START_MAP_Menu();
																	}
																	goto Label_2;
																}
																else 
																{
																	OpenSelectNationMenu();
																	break;
																}
																break;
															}
														case MN_MAKECHARACTER_NAME : break;
														case MN_SELECT_CHARACTER :	
															{
																CallOkCancelMessageBox( SubMenu->Id, 0,0,/*193, 75,*/ lan->OutputMessage(5,87) );//lsw
																SubMenu->nTemp = MN_OKCANCEL_MESSAGEBOX;
																goto Label_2;
															}
														case MN_OKCANCEL_MESSAGEBOX :
															{
																// 020925 YGI
												/*#ifdef CHINA_LOCALIZING_		// 중국에서는 아직 delete 메뉴를 사용하지 않는다.
																if(TRUE == SendDeleteCharacter( LoadCh[ character_active ].sCharacterName, "temp" ) )
																{
																	memset( &LoadCh[ character_active ], 0, sizeof( SCharacter ) );
																}
																SCharSource = LoadCh[ character_active ];
																break;
												#else
												*///021011 lsw
																CallDelCharComform();//020802 lsw
																goto Label_2;
//												#endif
															}
														case MN_LOGIN :	if( SubMenu->key == 100 )		// 홈페이지로 이동//020822 lsw
																		{
																			SubMenu->key = 0;
																			const int ret = LaunchApplication_kein( "htmlfile", lan->OutputMessage(5,89) );//lsw
																			if( ret ) 
																			{												
																				QuitFlag = SWD_QUIT;
																				GameEndFree();
																				MP3( SN_WARNING );
																				goto Label_2;
																			}
																		}
																		else
																		{
																			EWndMgr.ClearAllTxt();//021001 lsw
					 														SetFocus2(HWND_1);//021001 lsw
																			id_password = true;
																		}
																		break;

														case MN_LODING:		//SMenu[MN_MAINSTART_BACK].bActive=true;
																		//	SMenu[MN_LOGIN].bActive=true;//020515 lsw
																			::CallLoginMenu();
																			SMenu[MN_MAINSTART_BASIC ].bActive=true;
																			SMenu[MN_SELECT_CHARACTER].bActive=false;
																			SMenu[MN_LODING].bActive=false;
																			y_MenuFocus=0;
																		break;
														case MN_START_MAP:
															{
																MP3( SN_MENU_OPEN );
																StartPosition = SC_SCHOLIUM;
																SMenu[MN_LODING].bActive = true;
																SendChoiceCharacter( character_active );
																SMenu[SubMenu->nTemp].bActive = false;
																break;
															}
													}
													y_MenuFocus = 0;
													break;
											   }
						case SWD_CONNECT : 
							{
								if( !SubMenu->work ) goto Label_2;
								
								if( LoadCh[ character_active ].sCharacterName[0] )
								{ 
									SMenu[SubMenu->nField[j].nWillDo].bActive = true;
									SubMenu->bActive = false;
									SendChoiceCharacter( character_active );
									MP3( SN_MENU_OPEN );
								} 
								else
								{
									goto Label_2;
								}
							}break;
						case SWD_CONNECT_RECENT :
							{//021022 lsw
							#ifdef LIGHT_VERSION_
								MP3( SN_WARNING );
								goto Label_2;
							#endif
								if( LoadCh[ character_active ].sCharacterName[0] )
								{ 
									StartPosition = 99;// 최근 로그아웃한 장소
									SMenu[SubMenu->nField[j].nWillDo].bActive = true;
									SubMenu->bActive = false;
									SendChoiceCharacter( character_active );
									MP3( SN_MENU_OPEN );
								} 
							}break;

						case SWD_WILLDO_WORK : {
													MP3( SN_SELECT_START );
													SubMenu->work = SubMenu->nField[j].nWillDo;
													SubMenu->key = SubMenu->nField[j].x*1000+SubMenu->nField[j].y;		// 좌표넘겨 줄때
													goto Label_2;
											   }
						case SWD_RESET_JOB_EXPLAIN : {
														SMenu[MN_RESET_JOB_EXPLAIN].nField[0].fCheakFlag = true;
														goto Label_2;
													 }
						case SWD_NEW_ID : 
							{
								CallOkCancelMessageBox( SubMenu->Id, 0, 0, lan->OutputMessage(5,89),false );//020822 lsw
//								SMenu[MN_OKCANCEL_MESSAGEBOX].key = 100;
								SMenu[MN_OK_MESSAGEBOX].key = 100;
								goto Label_2;
							}break;
						case 	SWD_SELECT_TUTORIAL_OK://020815-2 lsw
							{	//맵이동 하겠다는 패킷 날림
								int &iMyTarget = SubMenu->nField[j].nWillDo;
								switch(iMyTarget)
								{
								case -1://Cancel 메뉴는 willdo에 -1이 박혀있다
									{
										if( SCharacterData.nLevel > 1)//레벨 8 이상 아이들만 이 메뉴를 닫을 수 있다//020821 lsw
										{
											SubMenu->bActive = false;
										}
										else//8렙 이하는 Cancel이 먹히지 않음
										{
											goto Label_2;//아무일도 안해준다//포커스도 안풀고 로딩메뉴도 없게 한다
										}
									}break;
								case SC_SCHOLIUM://초보자 수련소
								case SC_SCHOLIUM2://초보자 수련소
									{
										if( LoadCh[ character_active ].sCharacterName[0] )//캐릭이름이 있다면
										{ 
											StartPosition = iMyTarget;
											SMenu[MN_LODING].bActive = true;
											SendChoiceCharacter( character_active );
											MP3( SN_MENU_OPEN );
											SMenu[MN_START_MAP].bActive = false;//맵이동 인터페이스 죽임
										} 
										else//캐릭이 없으면(이런일이 있나??
										{
											goto Label_2;//이런일은 없는데..
										}
									}break;
								default:
									{
										goto Label_2;//기본. 아무것도 없을때는 클릭 해도 이벤트가 일어나지 않음
									}break;
								}
								y_MenuFocus = 0;//포커스 풀기,
								SubMenu->bActive = false;//메뉴 닫고								
								SubMenu->nField[j].nWillDo = 0;//윌두값 제거								
				
				#ifdef TAIWAN_LOCALIZNG_
								for(int ct = 2; 5 > ct;ct++)//필드 갯수
								{//메뉴가 닫힐때는 전부 원상 복귀
									SubMenu->nField[ct].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//버튼으로 셋팅
								}
				#else
								for(int ct = 2; 6 > ct;ct++)//필드 갯수
								{//메뉴가 닫힐때는 전부 원상 복귀
									SubMenu->nField[ct].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//버튼으로 셋팅
								}
				#endif
								goto Label_2;
							}break;
						case	SWD_SET_LOGIN_TARGET://020815-2
							{
							//<soto-030915
								const int iMyLoginTarget = SubMenu->nField[j].nWillDo;//로그인 지점을
								SubMenu->nField[1].nWillDo = iMyLoginTarget;//1번에다 쓴다
								
				#ifdef TAIWAN_LOCALIZNG_
								for(int ct = 2; 5 > ct;ct++)//필드 갯수
								{
									if(iMyLoginTarget == SubMenu->nField[ct].nWillDo)//020821 lsw
									{
										SubMenu->nField[ct].nType = FT_NOMAL_PUT;//일반 그림으로 찍음
									}
									else
									{
										SubMenu->nField[ct].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//버튼으로 셋팅
									}
								}
				#else
								for(int ct = 2; 6 > ct;ct++)//필드 갯수
								{
									if(iMyLoginTarget == SubMenu->nField[ct].nWillDo)//020821 lsw
									{
										SubMenu->nField[ct].nType = FT_NOMAL_PUT;//일반 그림으로 찍음
									}
									else
									{
										SubMenu->nField[ct].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//버튼으로 셋팅
									}
								}
				#endif
								goto Label_2;
							//>soto-030915
							}break;
				} // switch{}
				 // SWD_ 의 일을 한 메뉴는 사라짐. 사라지지 않을려면 goto 문 사용
				SubMenu->bActive=FALSE;
	Label_2 :								//사라지지 않는 메뉴일 경우에 사용
				SubMenu->nField[j].fCheakFlag=FALSE;
				SubMenu->nField[j].fLButtonDown=FALSE;
				;
			 }//if 문 끝
		 }break;

		case FT_SELECT_NATION : {
									int &selected = SubMenu->work;
									int willdo = SubMenu->nField[j].nWillDo;

									if( selected == willdo )
									{
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									}
									else 
									{
										if( SubMenu->nField[j].fLButtonDown )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
										}
										else if( SubMenu->nField[j].fRectMouse )
										{
											static int tt;
											static int de =10;
								
											if( tt ){
												de ++;
												if( de > 12 ) { tt = !tt; }
											}
											else {
												de --;
												if( de < 3 ) { tt = !tt; }
											} 

											int img_num = SubMenu->nField[j].nImageNumber;
											Spr *s = &spr[img_num];
											PutCompressedImageFX( SubMenu->x+SubMenu->nField[j].x+s->ox, SubMenu->y+SubMenu->nField[j].y+s->oy, s, de, 2 );
										}
										if( SubMenu->nField[j].fCheakFlag )
										{
											selected = willdo;
											SubMenu->nField[j].fCheakFlag = false;
											SubMenu->nField[j].fLButtonDown = false;
										}
									}
									break;
								}

		case FT_SELECT_NATION_EXPLAIN :{
										;
										int para = 0;
										int ret = true;
										switch( SMenu[MN_SELECT_NATION].work )
										{
											case 100 : 
											case 0 :		break; 
											
											case N_VYSEUS	:  
											case N_ZYPERN	:
											case N_HEGEMONIA:
											case N_YILSE	: para = SMenu[MN_SELECT_NATION].work; break;
											default : ret = false; break;
										}
										if( ret )
										{
											int &scroll = SubMenu->work;
											static char explain[7][11][200];
											static WORD	color[7][11];

											if( !nation_explain[0][0][0] ) 
											{
												LoadNationExplain();
												SetNationExplainDetect( 0, SubMenu->nField[j].nRectImage, explain, color );
											}
											static int old_para = para;
											if( old_para != para )
											{
												scroll = 0;
												old_para = para;
												SetNationExplainDetect( para, SubMenu->nField[j].nRectImage, explain, color );
											}

											for( int i=0; i<11; i++ )
											{
												if( !i || color[scroll][i] != color[scroll][i-1] ) Hcolor( color[scroll][i] );
												Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+i*18, g_DestBackBuf, "%s", EatFrontWhiteChar( explain[scroll][i] ) );
											}
											if( scroll < 6 )
											{
												if( explain[scroll+1][0][0] == NULL )
													SubMenu->key = 100;
												else SubMenu->key = 0 ;
											}
										}

										break;
									   }
		case FT_SELECT_NATION_SCROLL:{
										;
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, 6,SubMenu->nField[j].nImageType );

										//// 스크롤 처리
										int block = 0;		// 버튼 보여주기 말기 플레그
										int &scroll = SubMenu->work;
										int willdo = SubMenu->nField[j].nWillDo;
										if( ( scroll == 0 && willdo == SCROLL_UP ) ||
											( willdo == SCROLL_DOWN && scroll >= 6 ) || 
											( willdo == SCROLL_DOWN && SubMenu->key == 100 ) ) block =1; // 아래 화살표 버튼 보여주지 않기

										if( !block )		// 화살표를 보여주지 않는다.
										{
											/*
											int &delay=SubMenu->nField[j].nSHideNomalStart;
											if( (delay%8) < 4 )
											{
												int x = 217;
												int y;
												if( willdo == SCROLL_UP ) y = 25;
												else y = 223;

												FieldTypeNomalPut(SubMenu->x, SubMenu->y, x, y, SubMenu->nField[j].nSHideNomalCount);
											}
											delay++;*/

											if( SubMenu->nField[j].fLButtonDown )
											{
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+4, SubMenu->nField[j].y+5, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
											}
											else if( SubMenu->nField[j].fRectMouse )
											{
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+4, SubMenu->nField[j].y+5, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
											}
											else FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+4, SubMenu->nField[j].y+5, SubMenu->nField[j].nSpecialWillDo, SubMenu->nField[j].nImageType);

											if( SubMenu->nField[j].fCheakFlag )
											{
												if( willdo == SCROLL_UP ) scroll --;
												else if( willdo == SCROLL_DOWN ) scroll ++;
											}										
										}
										SubMenu->nField[j].fCheakFlag = 0;
										break;
									  }

		case FT_RESET_JOB_EXPLAIN : {
										;
										int &scroll = SubMenu->work;
										static char explain[5][11][200];
										static WORD	color[5][11];
										if( !job_explain[0][0][0] )
										{
											LoadJobExplain();
											SetJobExplainDetect( 0, SubMenu->nField[j].nRectImage, explain, color );
										}
										int job = SCharSource.nCharacterData[JOB]+1;
										static int old_job = job;
										if( old_job != job )
										{
											scroll = 0;
											old_job = job;
											SetJobExplainDetect( job, SubMenu->nField[j].nRectImage, explain, color );
										}

										for( int i=0; i<11; i++ )
										{
											if( !i || color[scroll][i] != color[scroll][i-1] ) Hcolor( color[scroll][i] );
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+i*18, g_DestBackBuf, "%s", EatFrontWhiteChar( explain[scroll][i] ) );
										}
									
										break;
									}
		case FT_RESET_JOB_SCROLL :{
									;
									if( SCharSource.nCharacterData[JOB] >= 0 ) break;
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->key,SubMenu->nField[j].nImageType );

									//// 스크롤 처리
									int block = 0;		// 버튼 보여주기 말기 플레그
									int &scroll = SubMenu->work;
									int willdo = SubMenu->nField[j].nWillDo;
									if( ( scroll == 0 && willdo == SCROLL_UP) ||	// 위 화살표 버튼 보여주지 않기
										( scroll == 2 && willdo == SCROLL_DOWN ) ) block =1; // 아래 화살표 버튼 보여주지 않기


									if( !block )		// 화살표를 보여주지 않는다.
									{
										if( SubMenu->nField[j].fLButtonDown )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+4, SubMenu->nField[j].y+5, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
										}
										else if( SubMenu->nField[j].fRectMouse )
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+4, SubMenu->nField[j].y+5, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
										}
										else FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x+4, SubMenu->nField[j].y+5, SubMenu->nField[j].nSpecialWillDo, SubMenu->nField[j].nImageType);

										if( SubMenu->nField[j].fCheakFlag )
										{
											if( willdo == SCROLL_UP ) scroll --;
											else if( willdo == SCROLL_DOWN ) scroll ++;
										}										
									}
									SubMenu->nField[j].fCheakFlag = 0;
									break;
								  }
		case FT_HIDE_ISCROLL_AUTO_PUT:	if( SubMenu->nField[j].fRectMouse )
										{
//											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											if( ((SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalStart) + (SubMenu->nField[SubMenu->nField[j].nWillDo].nShideNomalPlus)) >= (SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalCount) ) break;
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
										}

		case FT_HIDE_ISCROLL_PUT :	{	
										;//스크롤 될경우를 생각해 미리 범위를 체크해 스크롤을 하면 안되면 break한다
										int willdo = SubMenu->nField[j].nWillDo;
										int &ct = SubMenu->nTemp;
										int &turn = SubMenu->work;

										if( SubMenu->nField[j].nSpecialWillDo == 100 )
										{
											if( turn ) ct--;
											else ct++;
											if( ct > 14 ) turn = true;
											if( ct < 1 ) turn = false;
										}

										if( ((SubMenu->nField[willdo].nSHideNomalStart) + (SubMenu->nField[willdo].nShideNomalPlus)) >= (SubMenu->nField[willdo].nSHideNomalCount) ) 
										{
											SubMenu->nField[j].fCheakFlag=FALSE;
											break;
										}
										if( SubMenu->nField[j].nSpecialWillDo == 100 )
										{
											FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, ct, 2);
										}

										if(SubMenu->nField[j].fLButtonDown)
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
										if(SubMenu->nField[j].fCheakFlag)
										{
											SubMenu->nField[willdo].nSHideNomalStart+=SubMenu->nField[willdo].nShideNomalPlus;
											if(SubMenu->nField[willdo].nSHideNomalStart >= (SubMenu->nField[willdo].nSHideNomalCount))
											{
												(SubMenu->nField[willdo].nSHideNomalStart)-=(SubMenu->nField[willdo].nShideNomalPlus);
											}
											else MP3( SN_CLICK_START );
											SubMenu->nField[j].fLButtonDown=FALSE;
											SubMenu->nField[j].fCheakFlag=FALSE;
										}
										break;
									}

		case FT_HIDE_DSCROLL_AUTO_PUT :	if(SubMenu->nField[j].fRectMouse)
										{
////											if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
											if( ((SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalStart) - (SubMenu->nField[SubMenu->nField[j].nWillDo].nShideNomalPlus)) < 0 ) break;
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
										}
		case FT_HIDE_DSCROLL_PUT :	{
										;
										int willdo = SubMenu->nField[j].nWillDo;
										if( ((SubMenu->nField[willdo].nSHideNomalStart) - (SubMenu->nField[willdo].nShideNomalPlus)) < 0 ) 
										{
											SubMenu->nField[j].fCheakFlag=FALSE;
											break;
										}

										if( SubMenu->nField[j].nSpecialWillDo == 100 ) 
										{
											int &ct = SubMenu->nTemp;
											int &turn = SubMenu->work;
//											if( turn ) ct--;
//											else ct++;
//											if( ct > 14 ) turn = true;
//											if( ct < 1 ) turn = false;
											FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, ct, 2);
										}
										if(SubMenu->nField[j].fLButtonDown)
												FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
										if(SubMenu->nField[j].fCheakFlag)
										{
											SubMenu->nField[willdo].nSHideNomalStart-=SubMenu->nField[willdo].nShideNomalPlus;
											if(SubMenu->nField[willdo].nSHideNomalStart < 0)
											{
												SubMenu->nField[willdo].nSHideNomalStart=0;
											}
											else MP3( SN_CLICK_START );
											SubMenu->nField[j].fLButtonDown=FALSE;
											SubMenu->nField[j].fCheakFlag=FALSE;
										}
										break;
									}

		case FT_THROWDICE :	{
								if( !g_GoDice ) break;
								int num_to_rate[2] ={ 1003, 1002 };

								int	class_ability[5][2] = {			// RecvThrowDice 에서도 같은 배열을 쓴다.
															{ STR,	CON	, },
															{ DEX,	MOVP, },
															{ DEX,	CON	, },
															{ INT_,	WSPS, },
															{ WIS,	WSPS, },	};

/*								int	class_ability[5][5] = { 
															{ STR,	CON,	-1,  }, 
															{ MOVP, DEX,	LUCK,	CON, -1, },
															{ DEX,	CON,	MOVP,	-1, },
															{ CON,	INT,	ENDU,	WSPS, -1,},
															{ CON,	WSPS,	WIS,	ENDU, -1,} };*/


								if( SubMenu->nField[j].fCheakFlag )
								{
									SubMenu->nField[j].fLButtonDown=FALSE; //다이스 결과가 나올때까지 키 입력을 무시

									if( !g_GoDice2 )					// 주사위 랜덤수 서버 요청
									{
										SendThrowDice( SCharSource.nCharacterData[CLASS] );
										g_GoDice2 = true;
										g_GoDiceRecv = false;
									}

									if( !g_GoDiceRecv )					// 랜덤수 서버에서 받을 때 까지 클라이언트에서는 랜덤하게 주사위 보여준다.
									{
										for(i=0;i<2;i++)
										{
											int para = class_ability[ SCharSource.nCharacterData[CLASS] ][i];
											//if( para == -1 ) break;
											SCharSource.nAbilityMax[ para ] = GetDiceAbility( num_to_rate[i]);
											MP3( SN_DICE );
										}
										break;
									}

									cDiceCount++;   //다이스가 차례대로 멈추게 되므로 그걸 기억하는 카운트다
									if( !(cDiceCount%4) )	PutDiceNumber++;

									if(PutDiceNumber >= 7)//주사위 5개가 다 맘췄냐?-> delay를 위해 7로..
									{
									//설명문// 010216 lsw 주석 처리
									//	char str[]=	" 옆의 수치는 캐릭터가 처음에 갖게 `되는 수치들입니다. ` ` 이 수치에 만족하면 'YES'를 클릭 `해서 캐릭터만들기를 완료하세요. `만일 만족하지 않는다면 'NO'를 `클릭해서 다시 주사위를 굴리세요.";

										SMenu[MN_MAKECHARACTER_ABILITY].bActive=FALSE;
										CallMessageBox(TRUE, 34, 110, lan->OutputMessage(6,9), TRUE);//lsw

										SubMenu->nField[j].fCheakFlag=FALSE;
										cDiceCount=0;
										PutDiceNumber=0;
									}
								}
								if(SubMenu->nField[j].fLButtonDown)
								{
									PutDiceNumber=0;
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
									//다이스를 랜덤하게 출력
									for(i=0;i<2;i++)
									{
										int para = class_ability[ SCharSource.nCharacterData[CLASS] ][i];
										//if( para == -1 ) break;
										SCharSource.nAbilityMax[ para ] = GetDiceAbility( num_to_rate[i] );
										MP3( SN_DICE );
									}
								}
								break;
							}
		case FT_HIDE_PUT_CENTER :
				if(SubMenu->nField[j].fLButtonDown)
				{
					static int tt;
					static int de =10;
	
					if( tt ){
						de ++;
						if( de > 12 ) { tt = !tt; }
					}
					else {
						de --;
						if( de < 3 ) { tt = !tt; }
					} 
				
					Spr *s = &spr[SubMenu->nField[j].nImageNumber];
					lenght= (spr[SubMenu->nImageNumber].xl - s->xl)/2;
	
					PutCompressedImageFX( SubMenu->x + lenght+ s->ox, SubMenu->y+ SubMenu->nField[j].y + s->oy, s, de, 2 );
				}
				break;

		case FT_SELECT_CITY :{
								for( int a = j; a< SubMenu->nFieldCount; a++ )
								{
									if( SubMenu->nField[a].nType == FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2 )
									{
										Spr *s = &start_map_spr[29];		// 화살표 찍기
										PutCompressedImage(SubMenu->x+SubMenu->nField[a].rCheakBox.left, SubMenu->y+SubMenu->nField[a].rCheakBox.top, s);
									}
								}

								// 마을 이름 찍기
								static int return_flag = false;
								static int esc_flag = false;
								
								int field_num; // 컨넥트 버튼인지 리센트 번호인지 구분..
								if( SubMenu->work ) field_num = 0;
								else field_num = 1;

								if( !y_MenuFocus || y_MenuFocus == SubMenu->Id )		// 자판 키 입력 처리
								{
									if(g_aCurrentKeys[DIK_RETURN] & 0x80 )
									{
										SubMenu->nField[field_num].fLButtonDown= true;
										return_flag = true;
										//DoButtonCheckOfMenu( SubMenu->Id, 1 );		// recent connect
									}
									else if( return_flag )
									{
										SubMenu->nField[field_num].fCheakFlag= true;
										return_flag = false;
									}
									else if( g_aCurrentKeys[DIK_ESCAPE] & 0x80 )
									{
										SubMenu->nField[9].fLButtonDown= true;
										esc_flag =true;
										
										//DoButtonCheckOfMenu( SubMenu->Id, 9 );		// 3명의 캐릭터 선택으로 돌아감
									}
									else if( esc_flag )
									{
										SubMenu->nField[9].fCheakFlag= true;
										esc_flag =false;
									}
								}

								if( !SubMenu->work ) break;

								int image[][4] = {
								// {큰도시이름, 성그림, 각 마을이름, 마을 이미지를 찍을때 옵셉값을 주느냐}
									{1, 7, 17, 1},	// SC_HELTERNT_CASTLE
									{1, 7, 17, 1},	// SC_HELTERNT_VILLAGE
									{1, 0,  0, 0},	// SC_HUDAIN_RIVER
									{1, 0,  0, 0},	// SC_GRAY_MOUNTAIN
									{1, 0,  0, 0},	// SC_CALAHIL
									{1, 0,  0, 1},	// SC_BAISUS_IMPEL
									{2, 9, 19, 1},	// SC_RENUS
									{2, 0,  0, 1},	// SC_DEL_HAPPA
									{2,11, 21, 1},	// SC_IRAMUS
									{2,12, 22, 1},	// SC_DIPH_UBEN
									{0, 0,  0, 1},	// SC_SCHOLIUM
									{2,10, 28, 1},	// SC_TYREN
									{3, 5,  6, 1},	// SC_BARANTAN
								};

								int image_a = image[SubMenu->work-1][0];
								int image_b = image[SubMenu->work-1][1];
								int image_c = image[SubMenu->work-1][2];
								int offset = image[SubMenu->work-1][3];

						//		if( image_a )	//국가명 디스 플레이 //020515 lsw 삭제
						//			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, image_a, SubMenu->nField[j].nImageType);

								int gabx = SubMenu->key / 1000;
								int gaby = SubMenu->key % 1000;

								if( image_b )
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, gabx, gaby, image_b, SubMenu->nField[j].nImageType);
								if( image_c )
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, gabx+20*offset, gaby+20*offset, image_c, SubMenu->nField[j].nImageType);

								StartPosition = SubMenu->work;
								break;
							 }

		case FT_TEXT_PUT :    //영어 문자열 출력
				 FieldTypeTextPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].temp);
				 break;

		case FT_TEXT_PUT_CENTER :   //문자열을 중간부분에 출력
			{
				int not_english = 0;
				char *str = SubMenu->nField[j].temp;
				for( int a=0; a<strlen(str); a++ )
				{
					if( str[a] != ' ' && ( str[a] > 'Z' || str[a]<'A' ) )
					{
						not_english = 1;
						break;
					}	
				}
				if( not_english )
				{
					SetHangulAlign( TA_CENTER );
					Hprint2( SubMenu->x+spr[SubMenu->nImageNumber].xl/2, SubMenu->y+SubMenu->nField[j].y, NULL, "%s", str );
					break;
				}

				 i=0;
				 LenghtTotal=0;
			 			 
				 //문자열 전체 길이를 계산
				 while( (SubMenu->nField[j].temp[i]) != '\0' )
				 {
					 code=SubMenu->nField[j].temp[i];
					 if(code==32) LenghtTotal+=4;
					 else
					 {
						 code-=27;
					     LenghtTotal+=spr[code].xl;
					 }
					 i++;
				 }

				 // 중간부터 시작하기위한 좌표 계산
				 lenght=(spr[SubMenu->nImageNumber].xl-LenghtTotal)/2;
			
				 FieldTypeTextPut(SubMenu->x, SubMenu->y, lenght, SubMenu->nField[j].y, SubMenu->nField[j].temp);
		         break;
			}

		 //메뉴등에 데이터를 출력해야 할경우
		case FT_DATA_PUT :
				flag=TRUE;			// 플레그 설정 -캐러 이름이 없을때 아무것도 띄우지 않는다.
				switch(SubMenu->nField[j].nImageNumber)
				{ 
					case DP_HEALING_EXPLAIN :{	
												break;
												if( !GetSysInfo( SI_GAME_MAKE_MODE ) ) break;
												static int index = 1;
												static int a = 1 ;

												if(g_aCurrentKeys[DIK_1] & 0x80) index = 1;
												if(g_aCurrentKeys[DIK_2] & 0x80) index = 4;
												if(g_aCurrentKeys[DIK_3] & 0x80) index = 5;

												if(g_aCurrentKeys[DIK_PGUP] & 0x80) a--;
												if(g_aCurrentKeys[DIK_PGDN] & 0x80) a++;

												if( !ch_armor_image_after_b[index][a].img ) a=1;
												PutCompressedImageCharRGB( 350+65, 100+96, &ch_body_image_after_b[index][a], 200);
												PutCompressedImage( 350+65, 100+96, &ch_armor_image_after_b[index][a]);
												PutCompressedImageCharRGB( 350+65, 100+96, &ch_line_image_after_b[index][a], 0);

												PutCompressedImageCharRGB( 350+65, 100+96, &ch_body_image_after[index][a], 200);
												PutCompressedImage( 350+65, 100+96, &ch_armor_image_after[index][a]);
												PutCompressedImageCharRGB( 350+65, 100+96, &ch_line_image_after[index][a], 0);
												break;
										 }
												
												

					case DP_FACEIMAGE :	// 얼굴 이미지 찍기
						if(SCharSource.nCharacterData[FACE] >= 0) //FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SCharSource.nCharacterData[FACE]); 
						{
							if( SubMenu->Id == MN_MAKECHARACTER_FACE )
								PutCharImage(SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SCharSource.nCharacterData[FACE], 0, FS_BIG);
							else PutCharImage(SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, SCharSource.nCharacterData[FACE], 0, FS_MIDDLE);
						}
						break;
				
					case DP_ARIGEMENT :
						Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_ARIGEMENT_TEXT][SCharSource.nCharacterData[ARIGEMENT]].temp);
						break;
					//이름 출력
					case DP_NAME :
						{
							Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", SCharSource.sCharacterName );
						}break;
					//이름쓰는 칸에 출력
					case DP_NAMEEDITBOX :
						{
							SetFocus2( HWND_3 );//021001 lsw
							EWndMgr.GetTxt(HWND_3,SCharSource.sCharacterName,17);//021001 lsw
	 						if(strlen(SCharSource.sCharacterName )*8 < 119) // 깜박이는 커서 찍기 119는 에디트 박스 넓이
							{
								nCursor_Flicker++;
								if((nCursor_Flicker%=17) < 8)
								{
									Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s_", SCharSource.sCharacterName );
									break;
								}
							}
							Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", SCharSource.sCharacterName );
						}break;

					case DP_IDEDITBOX :
						{
							EWndMgr.GetTxt( HWND_1, sId, 15);
							if( id_password && EWndMgr.IsFocus(HWND_1)) 
							{
								SetFocus2(HWND_1);//021001 lsw
							}

							if(id_password) 
							{
								if(strlen(sId)*8 < 120) // 깜박이는 커서 찍기 90은 에디트 박스 넓이
								{
									nCursor_Flicker++;
									if((nCursor_Flicker%=32) < 16)
									{
										Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s_", sId );
										break;
									}
								 }
							}
							Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", sId );
						}break;

					case DP_PASSEDITBOX :
						{
							EWndMgr.GetTxt( HWND_2, sPassword, 15);
							
							for(k=0; k < (int)strlen(sPassword); k++) 
							{
								sPassword_temp[k]='*'; sPassword_temp[k+1] = NULL;
							}
							
							Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", sPassword_temp);

							if(id_password)
							{
								SetFocus2(HWND_1);//021001 lsw
							}
							else
							{
								SetFocus2(HWND_2);//021001 lsw
								if(strlen(sPassword)*8 < 120) // 90은 에디트 박스 넓이
								{
									nCursor_Flicker++;
									if((nCursor_Flicker%=32) < 16)
									{
										Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s_", sPassword_temp);
										break;
									}
								}
							}	
						}break;

					case DP_AGE_MAKE :	if( SubMenu->nField[j].nSHideNomalStart < 15 )  SubMenu->nField[j].nSHideNomalStart = 15;
										SCharSource.age = SubMenu->nField[j].nSHideNomalStart;
					case DP_AGE :		if( SCharSource.age > 14 )
										{
											SetHangulAlign( TA_RIGHT );
											Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nWillDo, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%d", SCharSource.age);
										}
										break;

					//클래스 출력
					case DP_CLASS :
							SetHangulAlign( TA_RIGHT );
							Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][SCharSource.nCharacterData[CLASS]].temp  );
							break;
					//스펠 출력
					case DP_SPELL :
							SetHangulAlign( TA_RIGHT );
							if(SCharSource.nCharacterData[SPELL] == 0)
							{
								Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"WIZARD");
							}
							else if(SCharSource.nCharacterData[SPELL] == 1)
							{
								Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"PRIEST");
							}
							break;
					//택틱스 출력
					case DP_TACTICS :
							SetHangulAlign( TA_RIGHT );
							if( SCharSource.nCharacterData[GENDER] )
							{
								Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_TACTICS_MAN_TEXT][SCharSource.nCharacterData[TACTICS_MAN]].temp  );
							}
							else
							{
								Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_TACTICS_WOMAN_TEXT][SCharSource.nCharacterData[TACTICS_WOMAN]].temp  );
							}
							break;
					//직업 출력
					case DP_JOB :
						{
							SetHangulAlign( TA_RIGHT );
							int job = SCharSource.nCharacterData[JOB];
							if( job >= 20 )  job -= 20;
							Hprint2( SubMenu->x+SubMenu->nField[j].x+SubMenu->nField[j].nRectImage, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s",SHideNomal[HN_MAKECHARACTER_JOB_TEXT][job].temp  );
							break;
						}
					case DP_STR : 
					case DP_CON :
					case DP_DEX :
					case DP_WIS :
					case DP_INT :
					case DP_MOVP :
					case DP_CHA :
					case DP_ENDU :
					case DP_MOR :
					case DP_WSPS :
					case DP_LUCK :{
									int result = SCharSource.nCharacterAbility[ SubMenu->nField[j].nWillDo ] + SCharSource.nAbilityMax[ SubMenu->nField[j].nWillDo ];
									wsprintf(y_temp,"%d", result);
									if( SCharSource.nAbilityMax[ SubMenu->nField[j].nWillDo ] )
									{
										wsprintf(y_temp,"%d + %d", SCharSource.nCharacterAbility[ SubMenu->nField[j].nWillDo ], SCharSource.nAbilityMax[ SubMenu->nField[j].nWillDo ]);
										Hprint2( SubMenu->x+SubMenu->nField[j].x+( 58-strlen(y_temp)*FONTSIZE_ ), SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%d +", SCharSource.nCharacterAbility[ SubMenu->nField[j].nWillDo ] );

										Hcolor( FONT_COLOR_PLUS );
										wsprintf(y_temp,"%d", SCharSource.nAbilityMax[ SubMenu->nField[j].nWillDo ]);
										Hprint2( SubMenu->x+SubMenu->nField[j].x+( 58-strlen(y_temp)*FONTSIZE_ ), SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", y_temp );
									}
									else 
									{
										Hcolor( FONT_COLOR_NUMBER );
										Hprint2( SubMenu->x+SubMenu->nField[j].x+( 58-strlen(y_temp)*FONTSIZE_ ), SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", y_temp );
									}
									break;
								  }

					case DP_LEVEL_LOGON :	Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%d", SCharSource.nLevel); //오른쪽 정렬 하지 않을때
											break;

					case DP_RECT_STRING_PUT :	
							RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage,SubMenu->nField[j].temp); 
							break;

					case DP_NAME_SELECT :{	// hometown을 nation으로 바꾸기
											//	FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x-9,SubMenu->nField[j].y+118, 0, START_ETC2 ); //020515 lsw
											//020515 lsw
											if( !SCharSource.sCharacterName[0] )//그림 바꾸기네..
											{
												SMenu[MN_SELECT_CHARACTER].nField[2].nType=FT_NOMAL_PUT;
												SMenu[MN_SELECT_CHARACTER].nField[3].nRectImage= 12;
												SMenu[MN_SELECT_CHARACTER].nField[3].nImageNumber= 13;
											}
											else
											{
												SMenu[MN_SELECT_CHARACTER].nField[2].nType=FT_NO_CHECK;
												SMenu[MN_SELECT_CHARACTER].nField[3].nRectImage= 5;
												SMenu[MN_SELECT_CHARACTER].nField[3].nImageNumber= 2;
											}

											if( !LoadCh[ SubMenu->nField[j].nWillDo ].sCharacterName[0] ) break;
											const int para = SubMenu->nField[j].nWillDo ;				// 현재 선택한 캐릭터
											if( para == character_active )		// 고른놈만 에니메이션 
											{
												const int first_para = LoadCh[para].nCharacterData[ GENDER ];			// 스프라이트 인덱스 불러오기 위해	// 여자 남자
												const int second_para = LoadCh[para].nCharacterData[ CLASS ];			// 선택 클래스
												const DWORD body = ReturnBlendRGB( LoadCh[para].body_r, LoadCh[para].body_g, LoadCh[para].body_b );
												const DWORD line = ReturnBlendRGB( LoadCh[para].cloth_r, LoadCh[para].cloth_g, LoadCh[para].cloth_b );
												PutTestAnimation(first_para,second_para,body,line);
											}
											int gaby = 0;
											int job = LoadCh[ SubMenu->nField[j].nWillDo ].nCharacterData[JOB];
											if( job >= 20 )  job -= 20;
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+gaby, g_DestBackBuf,"%s", LoadCh[ SubMenu->nField[j].nWillDo ].sCharacterName ); 
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+gaby+35, g_DestBackBuf,"%s", SHideNomal[HN_MAKECHARACTER_JOB_TEXT][job].temp);
											Hprint2( SubMenu->x+SubMenu->nField[j].x+52, SubMenu->y+SubMenu->nField[j].y+gaby+57, g_DestBackBuf,"%s", SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][LoadCh[ SubMenu->nField[j].nWillDo ].nCharacterData[CLASS]].temp);
											Hprint2( SubMenu->x+SubMenu->nField[j].x+52, SubMenu->y+SubMenu->nField[j].y+gaby+78, g_DestBackBuf,"%d", LoadCh[ SubMenu->nField[j].nWillDo ].age); 
											Hprint2( SubMenu->x+SubMenu->nField[j].x+52, SubMenu->y+SubMenu->nField[j].y+gaby+101, g_DestBackBuf,"%d", LoadCh[ SubMenu->nField[j].nWillDo ].nLevel); 
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+gaby+140, g_DestBackBuf,"%s", NationName[LoadCh[ SubMenu->nField[j].nWillDo ].nation] );	// 나라 이름 찍기
											Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+gaby+163, g_DestBackBuf,"%d", LoadCh[ SubMenu->nField[j].nWillDo ].nMoney );
											break;
										 }
					case DP_BACK_IMAGE_VALUE_TEXT : {
														;
														PutCompressedImageFX(SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+spr[71].oy-3, &spr[71], 15, 1);
														SetHangulAlign( TA_CENTER);
														Hcolor( FONT_COLOR_NAME );
														Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y+3, g_DestBackBuf, "%s", select_server_name );
														break;
													}

					}
					break;

		case FT_COLOR_GAUGE :	{
									int select=2;
									for(k=0;k<2;k++)
									{
										if(SHideNomal[HN_MAKECHARACTER_COLOR_SELECT][k].bShow) select = k;
									}
									if(select == 2) break;
									int start, end; 
									int gab=0;

									switch(j-7)			// field 번호가 7번부터 시작
									{
										case 0 : switch(select)
												 {
													case 0: gab = SCharSource.body_r; break;
													case 1: gab = SCharSource.cloth_r; break;
												 }
												 break;
										case 1 : switch(select)
												 {
													case 0: gab = SCharSource.body_g; break;
													case 1: gab = SCharSource.cloth_g; break;
												 }
												 break;
										case 2 : switch(select)
												 {
													case 0: gab = SCharSource.body_b; break;
													case 1: gab = SCharSource.cloth_b; break;
												 }
												 break;
									}

									gab = (int) (gab / 255.0f * 83.0f +0.5f);
									end = gab;
									start = SubMenu->nField[j].x;

									if(SubMenu->nField[j].fLButtonDown)	 end = g_pointMouseX - (SubMenu->x+start);
									if(end < 0 ) 
									{
										gab--;
										if ( gab < 0 ) gab = 0;
									}
									else if(end > 83) 
									{
										gab++;
										if( gab > 83 ) gab = 83;
									}
									else gab = end;
								
									for(int x=start;x<gab+start;x++)
										FieldTypeNomalPut(SubMenu->x, SubMenu->y, x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);

									gab = (int) (gab / 83.0f * 255.0f + 0.5);
									switch(j-7)			// field 번호가 7번부터 시작
									{
										case 0 : switch(select)
												 {
													case 0: SCharSource.body_r = gab; break;
													case 1: SCharSource.cloth_r = gab; break;
												 }
												 break;
										case 1 : switch(select)
												 {
													case 0: SCharSource.body_g = gab; break;
													case 1: SCharSource.cloth_g = gab; break;
												 }
												 break;
										case 2 : switch(select)
												 {
													case 0: SCharSource.body_b = gab; break;
													case 1: SCharSource.cloth_b = gab; break;
												 }
												 break;
									}
									break;
								}
		// 0906 kkh 작업 해 넣을 부분.. \
		// 이부분은 그림 데이터유무를 따져서 카운트를 늘려가다 그림 데이터가 없는 부분에서 값을 넘겨준다. 
		case FT_SELECT_FACE :{
								int tempi = 1;
								int a = 0;
								int &num = SubMenu->nField[j].nSHideNomalStart;
								int &count = SubMenu->nField[j].nSHideNomalCount;

								if( SCharSource.nCharacterData[GENDER] ) 
								{
									for( a=0; a<FACE_MAX; a++ )				// 최적화를 시켜야 겠지?
										if( !spr_face_man_b[a].img )
										{
											// 0906 KKH 추가 
											for(tempi = 1; a < ADD_CHA_PIC;tempi++){
												if( !spr_face_man_b_dash[tempi].img){	
													count = a + tempi;
													break;
												}			
											}
										}
										if( !spr_face_man_b[num].img ) {
											if( !spr_face_man_b_dash[num-8].img) num--;
										}
								}
								else 
								{
									for( a=0; a<FACE_MAX; a++ )
										if( !spr_face_woman_b[a].img ) 
										{
											// 0906 KKH 추가 
											for(tempi = 1; a < ADD_CHA_PIC;tempi++){
												if( !spr_face_woman_b_dash[tempi].img){	
													count = a + tempi;
													break;
												}			
											}
										}
										if( !spr_face_woman_b[num].img ) {
											if( !spr_face_woman_b_dash[num-7].img) num--;
										}
								}

								// 0907 kkh 남자 캐릭터를 고르다 여자 캐릭터를 고를경우 그림이 없어지는 현상을 막기 위해..
								//acer4
								if( num >= count-1 ){
									num = count-2;
								}
								
								SCharSource.nCharacterData[FACE] = num ;
								break;
							}

		case FT_DO :  //무조건 실행해야 할때, 한마디로 뜨내기들 붙이는 곳, case문 중복 피해야함.
			switch(SubMenu->nField[j].nWillDo)
			{
				case MN_MAINSTART_BASIC :	{
												SMenu[MN_MAINSTART_BASIC].bActive=TRUE;
												SubMenu->nField[j].nType = FT_NO_CHECK;		//	타이틀 동영상 끝
												break;


						
												static int turn=0;
												static unsigned short int gab = 8;

												static int go_flag=0;
												static bool end_flag=true;
												static bool flag2= false;
												static int flag = 0;


												Spr *s1 = &spr[SubMenu->nField[j].nImageNumber];
												Spr *s2 = &spr[SubMenu->nField[j].nRectImage];
												int lion[]= {305, 306, 307, 308, 309,308,307,306 };
												Spr *s3 = &spr[lion[flag%8]];


												UpperMenuNomalPut( SMenu[MN_MAINSTART_BASIC].x, SMenu[MN_MAINSTART_BASIC].y, SMenu[MN_MAINSTART_BASIC].nImageNumber, 0 );
//												if( turn > 2 )		// 그림자
												{
													PutCompressedImageFX( SubMenu->x+0-turn*gab+s1->xl/*+s2->ox*/, SubMenu->y+0+s2->oy, s2, 8,3);
													PutCompressedImageFX( SubMenu->x+SubMenu->nField[j].x+s2->ox+turn*gab-s2->ox, SubMenu->y+SubMenu->nField[j].y+s2->oy, s2, 8,3);
												}
												PutCompressedImage( SubMenu->x+0-turn*gab+s1->ox, SubMenu->y+0+s1->oy, s1 );
												PutCompressedImage( SubMenu->x+0-turn*gab+94+s3->ox, SubMenu->y+0+187+s3->oy, s3 );

												PutCompressedImage( SubMenu->x+SubMenu->nField[j].x+s1->ox+turn*gab, SubMenu->y+SubMenu->nField[j].y+s1->oy, s1 );
												PutCompressedImage( SubMenu->x+SubMenu->nField[j].x+turn*gab+94+s3->ox, SubMenu->y+SubMenu->nField[j].y+187+s3->oy, s3 );
												
												if( turn*gab >= 320 ) go_flag = 2;
												if( go_flag == 2 && end_flag) 
												{
													SMenu[MN_MAINSTART_BASIC].bActive=TRUE;
													SubMenu->nField[j].nType = FT_NO_CHECK;		//	타이틀 동영상 끝
													go_flag = 0;
													turn = 0; gab = 8; end_flag = true; flag2 = false; flag = 0;
												} 
												if( LButtonDown ) go_flag =1;
												if( go_flag==1 && !LButtonDown ) go_flag=2;

												if( flag > 2 ) turn++;
												flag++;												
												break;
											}
				case DP_ARIGEMENT :		if(SCharSource.nCharacterData[ARIGEMENT] == -1) break;
										else
										{
											FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber+SCharSource.nCharacterData[ARIGEMENT]);
											static int stch=0;
											static int de=0;
											FieldTypeNomalPutFx(SubMenu->x+11, SubMenu->y+6, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharSource.nCharacterData[ARIGEMENT],de, 2);
//											switch(stch%2)
//											{
//												case 0: FieldTypeNomalPutFx(SubMenu->x+11, SubMenu->y+4, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharSource.nCharacterData[ARIGEMENT],de, 2);break;
//												case 0: FieldTypeNomalPutFx(SubMenu->x+9, SubMenu->y+5, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharSource.nCharacterData[ARIGEMENT],de, 2);break;
//												case 1: FieldTypeNomalPutFx(SubMenu->x+11, SubMenu->y+6, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharSource.nCharacterData[ARIGEMENT],de, 2);break;
//											}
											if(stch > 15) de-=2;
											else de+=2;
											stch++;
											stch %= 32;
										}
										break;

				case DO_CHARACTER :	{
										int index=0;
										if(SCharSource.nCharacterData[CLASS] == -1) break;
										switch( SCharSource.nCharacterData[ GENDER ] )
										{
											case FEMALE :	switch( SCharSource.nCharacterData[ CLASS ] )
															{
																case WARRIOR : index = 4; break;
																case THIEF	 : index = 4; break;
																case ARCHER  : index = 4; break;
																case WIZARD  : index = 5; break;
																case PRIEST  : index = 6; break;
															}
															break;
											case MALE :		switch( SCharSource.nCharacterData[ CLASS ] )
															{
																case WARRIOR : index = 1; break;
																case THIEF	 : index = 1; break;
																case ARCHER  : index = 1; break;
																case WIZARD  : index = 2; break;
																case PRIEST  : index = 3; break;

															}
										}

										static int turn = true;
										static int a=1;
										

										DWORD body = ReturnBlendRGB( SCharSource.body_r, SCharSource.body_g, SCharSource.body_b );
										DWORD line = ReturnBlendRGB( SCharSource.cloth_r, SCharSource.cloth_g, SCharSource.cloth_b );
										
										PutTestAnimation2(SCharSource.nCharacterData[ GENDER ],SCharSource.nCharacterData[ CLASS ],body,line);

										/*
										if( SMenu[ MN_MAKECHARACTER_CLASS ].bActive || SMenu[ MN_MAKECHARACTER_COLOR ].bActive )		// 에니메이션
										{
											PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_body_image_after[index][a], body);
											PutCompressedImage( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_armor_image_after[index][a]);
											PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_line_image_after[index][a], line);
											a++;
											if( !ch_body_image_after[index][a].img ) 
											{
												a=1;
												turn = true;
											}
											else turn = false;
										}
										else if( !turn )
										{
											PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_body_image_after[index][a], body);
											PutCompressedImage( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_armor_image_after[index][a]);
											PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_line_image_after[index][a], line);
											a++;
											if( !ch_body_image_after[index][a].img ) 
											{
												a=1;
												turn = true;
											}
											else turn = false;
										}
										else
										{
											static int c=1;
											PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_body_image_befor[index][c], body);
											PutCompressedImage( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_armor_image_befor[index][c]);
											PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_line_image_befor[index][c], line);
											c++;
											if( !ch_body_image_befor[index][c].img ) c=1;
											turn = true;
										}
										*/
										break;
									}

//				case DO_SELECT_LOAD_CHAR :	if(SubMenu->nField[j].nSpecialWillDo != SubMenu->nField[j].nSHideNomalStart)
//											{
//												SubMenu->nField[j].nSpecialWillDo = SubMenu->nField[j].nSHideNomalStart;
//												SCharSource = LoadCh[ SubMenu->nField[j].nSpecialWillDo ];
//											}
//											break;
						
				case FT_DO_DEFAULT : //처음 로고 화면에서만 사용..
				{//020515 lsw
					static int tt=TRUE, pp;
					static int de =0;
					if( tt )
					{
						if( pp ) de++; 
						pp=!pp;
						if( de > 25 ) { tt = !tt; }
					}
					else 
					{
						if( pp ) de--;
						pp=!pp;
						if( de < 5 ) { tt = !tt; }
					} 
					FieldTypeNomalPutFx3(SubMenu->x,SubMenu->y,SubMenu->nField[j].x,SubMenu->nField[j].y,
					SubMenu->nField[j].nImageNumber,SubMenu->nField[j].nImageType,de,1);

					static bool f_LButtonDown=0;
					if( LButtonDown ) f_LButtonDown = true;
					if( !LButtonDown && f_LButtonDown )
					{
						tt = true; de = 0; pp =false; f_LButtonDown = false;
						SubMenu->nField[1].fCheakFlag=TRUE;
					}
					break;
				}
			}
			SubMenu->nField[j].fCheakFlag=FALSE;
			SubMenu->nField[j].fLButtonDown=FALSE;
			break;
		case FT_TEST_CHECK	:{
								;
								int willdo = SubMenu->nField[j].nWillDo;
								if( BetaTest == willdo ) 
									FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, 0 );
								
								if( SubMenu->nField[j].fCheakFlag ) 
								{
									BetaTest = willdo;
									MP3( SN_CLICK_START );
									SubMenu->nField[j].fCheakFlag=FALSE;
									SubMenu->nField[j].fLButtonDown=FALSE;
								}
								break; 
							 }

		case FT_INFO_PUT :	{
								char *dr_saying = lan->OutputMessage(7,171);
								RectTextPut(SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y,SubMenu->nField[j].nRectImage,dr_saying); 
								bool go = false;
								if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 ) go = true;
								static bool count = false;
								if( LButtonDown ) 
								{
									count = true;
								}
								else if( count ) go = true;

								if( go )
								{
									count = false;
									g_aCurrentKeys[DIK_ESCAPE] = 0;
									SubMenu->bActive = false;
									::CallLoginMenu();
								}
								break;
							}
		case FT_NO_CHECK :	SubMenu->nField[j].fCheakFlag=FALSE;	// 일단 체크 플레그를 폴스로 하고 아무것도 찍지 않을때.
							SubMenu->nField[j].fLButtonDown=FALSE;
							break; 

		case FT_PUT_TACTICS : {
								;
								int img_num_man[] = { 46, 50, 51, 40, 48, 47, 42, 43, 41, 44, 45 };
								int img_num_woman[] = { 46, 50, 51, 49, 48, 47, 42, 43, 40, 41 };
								int *img_num;

								if( SCharSource.nCharacterData[SPELL] == WIZARD_SPELL )
								{
									img_num_woman[9] = img_num_man[8] = 54;
								}

								int para = SubMenu->nField[j].nSHideNomalNumber;
								if( para == HN_MAKECHARACTER_TACTICS_MAN_TEXT )
								{
									img_num = img_num_man;
								}
								else img_num = img_num_woman;
								
								int start = SubMenu->nField[j].nSHideNomalStart;
								int plus = SubMenu->nField[j].nShideNomalPlus;
								int max = SubMenu->nField[j].nSHideNomalCount;

								for( int a=start; a< start+plus && a<max; a++ )
								{
									Spr *s = GetSprOfMenu( MAIN_ETC , img_num[a] );
									if( !s ) continue;
									
									int lenght=(spr[SubMenu->nImageNumber].xl )/2;
									int y = SHideNomal[para][a].y+3;
									PutCompressedImage( SubMenu->x+lenght, SubMenu->y+y, s );
								}

								break;
							  }

		case FT_HIDE_NOMAL_ONE_PUT :{
										;
										int count;	// 현재 선택된 항목 
										int willdo = SubMenu->nField[j].nSHideNomalNumber;
										int plus = SubMenu->nField[j].nShideNomalPlus;
										int start = SubMenu->nField[j].nSHideNomalStart;
										int max = SubMenu->nField[j].nSHideNomalCount;
										//for( count = start ; count<start+plus; count++ )
										for( count = 0 ; count<max; count++ )
										{
											if( SHideNomal[willdo][count].bShow ) break;
										}

										//if( count < start+plus ) //010216 lsw
										if( count < max )
										{
											char str[10][15][256]={0,};
											
											for(int i=0;i<10; i++)
												for(int j=0;j<15; j++)
												{
													
													sprintf(str[i][j],"%s",lan->OutputMessage(9,i*15+j+31));
												}
											/*
											char str[10][15][256] = {
																	{
																		"여성을 선택하면 특유의 섬세함으로 펜싱검을 사용할 수 있습니다.",
																		"남성을 선택하면 강인한 체력으로 인해 양손검과 양손도끼를 사용할 수 있습니다.", 
																	},
																	{
																		"선하면서 정직한 가치관입니다.", 
																		"보통의 선한 가치관입니다.", 
																		"중도에서 선한쪽으로 넘어가는 혼돈의 선입니다.", 
																		"중도이지만 정직한 면을 가지고 있습니다.", 
																		"선도 악도 아닌 중도입니다.", 
																		"중도에서 악으로 넘어가는 혼돈스러운 중도", 
																		"악하지만 그래도 정직한 면이 있습니다.", 
																		"보통의 악한 가치관입니다.", 
																		"악하면서도 잔인한 가치관입니다.", 
																	}, 
																//51
																{
																		"선천적인 힘과 건강 그리고 용기를 가진 클래스입니다. 단거리전투에 능합니다. ",
																		"속임수와 잔기술 그리고 빠른발로 세상에 맞서는 클래스입니다. 훔치기 기술이 있습니다.",
																		"타고난 민첩성과 악에대한 격멸을 가진 클래스입니다. 활을 잘사용합니다.",
																		"대자연의 힘을 이용한 마법에 능한 클래스입니다. 단 초기에 체력이 약한 단점이 있습니다.",
																		"신에 힘에 의지한 마법에 능한 클래스입니다. 초기에  체력이 약하지만 죽은사람을 살릴수 있습니다.",
																	},
																	//61


																	{
																		"대자연의 힘(Mana)를 이용한 공격적인 마법을 주로 사용합니다.",
																		"기도의 힘(Divine)을 이용한 보호,치료의 마법을 주로 사용합니다. ",
																	},
																	{
																		"캐릭터의 피부색을 조정합니다.",
																		"캐릭터가 착용한 옷에 있는 색을 조정합니다. ",
																	},
																	{
																		"맨손전투능력입니다. 수치가 높으면 칼보다 강할수 있습니다. ",
																		"한손검을 잘다루는 능력입니다. 전사의 주력택틱스입니다.",
																		"활을 잘쏘는 능력입니다. 궁수의 주력택틱스가 됩니다.",
																		"방패를 이용한 방어에 능한 택틱스입니다.",
																		"도끼나 철퇴등의 육중한 무기를 다루는 능력입니다.",
																		"창을 잘 다룰수 있는 능력입니다.",
																		"봉을 잘 다루는 능력으로 마법사나 성직자의 주력택틱스입니다.",
																		"비검을 잘 던지는 능력으로 도둑의 주력택틱스입니다.",
																		"성직자 마법의 효과를 높여주는 택틱스입니다.",
																		"양손검을 잘사용하는 능력으로 남자만이 가질수 있습니다.",
																		"양손도끼를 잘 사용하는 능력으로 남자만이 가질수 있습니다.",
																	},
																	{
																		"맨손전투능력입니다. 수치가 높으면 칼보다 강할수 있습니다. ",
																		"한손검을 잘다루는 능력입니다. 전사의 주력택틱스입니다.",
																		"활을 잘쏘는 능력입니다. 궁수의 주력택틱스가 됩니다.",
																		"펜싱검을 잘다루는 능력입니다. 여자만이 선택할수 있습니다.",
																		"도끼나 철퇴등의 육중한 무기를 다루는 능력입니다.",
																		"창을 잘 다룰수 있는 능력입니다.",
																		"봉을 잘 다루는 능력으로 마법사나 성직자의 주력택틱스입니다.",
																		"비검을 잘 던지는 능력으로 도둑의 주력택틱스입니다.",
																		"방패를 이용한 방어에 능한 택틱스입니다.",
																		"마법사 마법의 효과를 높여주는 택틱스입니다.",
																	},
																	{
																		"농부는 밭을갈고 곡식을 심고 그 곡식을  재배하는 기술을 가지고 있습니다.",
																		"광부는 광맥을 찾고 거기서 각종 광물을 캐내는 기술을 가지고 있습니다.",
																		"어부는 낚시나 그믈질로 물고기를 잡는 기술을 가지고 있습니다.",
																		"나무꾼은 나무를 배고 다듬는 기술을 가지고 있습니다.",
																		"도축자는 죽은 동물에게서 고기와 가죽을 얻어내는 기술이 있습니다.",
																		"심마니는 산이나 들판에 있는 약초를 찾아서  약으로 만드는 기술이 있습니다.",
																		"목동은 동물들을 이리저리 몰고 다니는 기술이 있습니다.",
																		"목수는 목재를 이용해서 여러 아이템을 만드는 기술이 있습니다.",
																		"대장장이는 광물을 이용해서 여러 아이템을 만드는 기술이 있습니다.",
																		"요리사는 재료를 이용해 건강을 올려주는 아이템을 만드는 기술이 있습니다.",
																		"재단사는 실과 천으로 옷을 만들어 내는 능력이 있습니다.",
																		"활제작사는 목재를 이용해서 활과 화살을 전문적으로 만들어 냅니다.",
																		"연금술사는 특수한 금속이나 포션을 만들어 냅니다.",
																		"초쟁이는 밤을 밝혀주는 양초를 만들어 냅니다.",
																		"상인은 물건을 숨겨 도둑으로부터 보호하면서 먼곳을 이동할수 있습니다.",
																	},
											};
											*/

											//if( SubMenu->Id == MN_RESET_JOB ) SCharSource.nCharacterData[JOB] = count+20;

											wsprintf(str[5][8], lan->OutputMessage(7,162), SCharSource.nCharacterData[SPELL]==WIZARD_SPELL?lan->OutputMessage(7,163):lan->OutputMessage(7,164));//010216 lsw
											wsprintf(str[6][9], lan->OutputMessage(7,162), SCharSource.nCharacterData[SPELL]==WIZARD_SPELL?lan->OutputMessage(7,163):lan->OutputMessage(7,164));//010216 lsw


											SubMenu->key++;
											if( SubMenu->key>= 3 )		// 설명이 겹치지 않기 위해서
											{
												SubMenu->key= 3;
												char *text = NULL;
												switch (SubMenu->Id)
												{
													case MN_MAKECHARACTER_GENDER	:		text = str[0][count]; break;
													case MN_MAKECHARACTER_ARIGEMENT :		text = str[1][count]; break;
													case MN_MAKECHARACTER_CLASS		:		text = str[2][count]; break;
													case MN_MAKECHARACTER_SPELL		:		text = str[3][count]; break;
													case MN_MAKECHARACTER_COLOR		:		text = str[4][count]; break;
													case MN_MAKECHARACTER_TACTICS_MAN	:	text = str[5][count]; break;
													case MN_MAKECHARACTER_TACTICS_WOMAN :	text = str[6][count]; break;
													case MN_RESET_JOB				:
													case MN_MAKECHARACTER_JOB		:		text = str[7][count]; break;
													default : SubMenu->key = 0; break;
												}

												static int count_old = count;
												static int menu_old = SubMenu->Id;
												static int scroll = 0;
												if( count_old != count || menu_old != SubMenu->Id )
												{
													count_old = count;
													menu_old = SubMenu->Id;
													scroll = 0;
												}

												if( text )
												{
													int curr_line = scroll / 30;
													int max_line = GetLineRectTextPut( 170, text );
													if( !max_line ) break;
													if( max_line <= 2 )
													{
														RectTextPut(SubMenu->x+5+13,SubMenu->y+315+14,170, text);
													}
													else 
													{
														curr_line %= max_line;
														SkipRectTextPut(SubMenu->x+5+13,SubMenu->y+315+14,170, curr_line, text, 2, 0); // 설명문을 쓴다 
													}
												}
												else
												{
												}
												scroll++;
											}		 // 중간에 break;가 들어가면 안된다.
										}
										else SubMenu->key = 0;
									}
		case FT_HIDE_NOMAL_GROUP_PUT :
			for(i=SubMenu->nField[j].nSHideNomalStart; i<(SubMenu->nField[j].nSHideNomalStart)+(SubMenu->nField[j].nShideNomalPlus) && i<SubMenu->nField[j].nSHideNomalCount; i++)
			{
				static int bSound=0;
				int type = i*100+j;
				if( SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].bRect &&  SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nType==FT_NOMAL_PUT_CENTER)
				{
					if( !SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].bShow )
						if( bSound != type ) { MP3( SN_TOUCH_START ); bSound = type; }

					static int tt;
					static int de =10;
		
					if( tt ){
						de ++;
						if( de > 12 ) { tt = !tt; }
					}
					else {
						de --;
						if( de < 3 ) { tt = !tt; }
					} 
				
					Spr *s = &spr[SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].nImageNumber];
					lenght= (spr[SubMenu->nImageNumber].xl - s->xl)/2;
	
					PutCompressedImageFX( SubMenu->x + lenght+ s->ox, SubMenu->y+ SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y + s->oy, s, de, 2 );
				}
				else
				{
					if( bSound == type ) bSound = 0;
				}
				 
				if( SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].bShow || SubMenu->nField[j].nType==FT_HIDE_NOMAL_GROUP_PUT ) //ONE는 체크 된것만 보여준다
				{
					switch(SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nType)
					{
						case FT_TEXT_PUT :
							 FieldTypeTextPut(SubMenu->x, SubMenu->y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].x, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].temp);
							 break;
						case FT_TEXT_PUT_CENTER :
							 k=0;
							 LenghtTotal=0;
							 //문자열 전체 길이를 계산
							 while( (SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].temp[k]) != '\0' )
							 {
								 code=SHideNomal[SubMenu->nField[j].nSHideNomalNumber ][i].temp[k];
			 					 if(code==32) LenghtTotal+=4;
								 else
								 {
									 code-=27;
									 LenghtTotal+=spr[code].xl;
								 }
								 k++;
							 }
							 // 중간부터 시작하기위한 좌표 계산
							 lenght=(spr[SubMenu->nImageNumber].xl-LenghtTotal)/2;
							 FieldTypeTextPut(SubMenu->x, SubMenu->y, lenght, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].temp);
							 break;

						case FT_NOMAL_PUT :
							 FieldTypeNomalPut(SubMenu->x, SubMenu->y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].x, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].nImageNumber);
							 break;
						case FT_NOMAL_PUT_CENTER :
			 				 lenght= (spr[ SubMenu->nImageNumber ].xl - spr[ SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].nImageNumber ].xl)/2;
			 				 FieldTypeNomalPut(SubMenu->x, SubMenu->y, lenght+1, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].y, SHideNomal[ SubMenu->nField[j].nSHideNomalNumber ][i].nImageNumber);
							 break;
					}
				}
			}break;
		default://020515 lsw
			{
				StartMenuSubProcessType2(SubMenu,i,j,k);
			}break;		
		}
	}
}


#define MAX(x,y)	(x>y)? x : y 


extern bool g_ControlKeyOn;
extern void tempcallfont() ;
void start_KeyProc()
{

	static int flag=0;
	static int flag2=0;
	static char flag3=0;
	int index;
	int comp=0;

	int y,k, group=5;
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		if(g_aCurrentKeys[DIK_NUMPADPLUS] & 0x80)  SCharacterData.nLevel++;
		if(g_aCurrentKeys[DIK_NUMPADMINUS] & 0x80)  SCharacterData.nLevel--;

	}
#endif
	if( (g_aCurrentKeys[DIK_LCONTROL] & 0x80) 
		|| (g_aCurrentKeys[DIK_RCONTROL] & 0x80) )
	{
		g_ControlKeyOn = true;
	}
	else
	{
		g_ControlKeyOn = false;
	}

	if(g_aCurrentKeys[DIK_TAB] & 0x80) 
	{
		if( flag != 1) flag = 1;
		else if(flag == 1) return;
	}
	else if( flag == 1 ) { flag = 0; comp = 1; }

	if(g_aCurrentKeys[DIK_RETURN] & 0x80)
	{
		if( flag != 2 ) flag = 2;
		else if(flag == 2) return;
	}
	else if( flag == 2 ) { flag = 0; comp = 2; }

	if(g_aCurrentKeys[DIK_ESCAPE] & 0x80)
	{
		if( flag != 3) flag = 3;
		else if(flag == 3) return;
	}
	else if( flag == 3 ) { flag = 0; comp = 3; }

	if(!flag && !comp ) return;

	for(int x=0;x<MAX_START;x++)
	{
		index = menu[x];

		if(	EWndMgr.IsFocus(HWND_1)||//021001 lsw
			EWndMgr.IsFocus(HWND_2)||
			EWndMgr.IsFocus(HWND_3)	){return;}

		if(SMenu[index].bActive)
		{
			if( y_MenuFocus && y_MenuFocus != index ) continue;
			switch( MAX(flag,comp) )
			{
			case 2 :	/*엔터키*/	
				{
					switch(index)
					{
					case MN_MAKECHARACTER_ABILITY :
													if(comp)
													{
														SMenu[MN_MAKECHARACTER_ABILITY].nField[4].fLButtonDown=FALSE;
														SMenu[MN_MAKECHARACTER_ABILITY].nField[4].fCheakFlag=TRUE;
													}
													else	SMenu[MN_MAKECHARACTER_ABILITY].nField[4].fLButtonDown=TRUE;
													return;

					case MN_YESNO :	if(comp)
									{
										SMenu[MN_YESNO].nField[0].fCheakFlag=TRUE;
										SMenu[MN_YESNO].nField[0].fLButtonDown=FALSE;
									}
									else SMenu[MN_YESNO].nField[0].fLButtonDown=TRUE;
									return;

					case MN_SYSTEM_LOGOUT : if(comp)
											{
												SMenu[MN_SYSTEM_LOGOUT].nField[0].fCheakFlag=true;
												SMenu[MN_SYSTEM_LOGOUT].nField[0].fLButtonDown=false;
											}
											else
											{
												SMenu[MN_SYSTEM_LOGOUT].nField[0].fLButtonDown=true;
											}
											return;

					case MN_OK_MESSAGEBOX :
					case MN_OKCANCEL_MESSAGEBOX :	if( comp ) 
													{
														SMenu[index].nField[2].fCheakFlag = true;
														SMenu[index].nField[2].fLButtonDown = false;
													}
													else
													{
														SMenu[index].nField[2].fLButtonDown= true;
													}
													return;

					case MN_MAINSTART_BASIC :	if(comp)
												{
													SMenu[MN_MAINSTART_BASIC].nField[1].fCheakFlag=TRUE;
													SMenu[MN_MAINSTART_BASIC].nField[1].fLButtonDown=FALSE;
												}
												else SMenu[MN_MAINSTART_BASIC].nField[1].fLButtonDown=TRUE;
												return;

					case MN_LOGON :	
					case MN_LOGIN : if(comp)
									{
										SMenu[index].nField[0].fCheakFlag=TRUE;
										SMenu[index].nField[0].fLButtonDown=FALSE;
									}
									else SMenu[index].nField[0].fLButtonDown=TRUE;
									return;

					case MN_SELECT: if( comp )
									{
										SMenu[MN_MAINSTART_START].nField[1].fCheakFlag=true;
										SMenu[MN_MAINSTART_START].nField[1].fLButtonDown = false;
									}
									else SMenu[MN_MAINSTART_START].nField[1].fLButtonDown = true;
									return;

					case MN_LOCATION:	if( comp )
										{
											SMenu[MN_MAINSTART_START].nField[3].fCheakFlag=true;
											SMenu[MN_MAINSTART_START].nField[3].fLButtonDown=false;
										}
										else SMenu[MN_MAINSTART_START].nField[3].fLButtonDown=true;
										return;

					case MN_SELECT_CHARACTER:
						{
							if( comp )
							{
								SMenu[MN_SELECT_CHARACTER].nField[0].fCheakFlag=true;
								SMenu[MN_SELECT_CHARACTER].nField[0].fLButtonDown=false;
							}
							else 
							{
								SMenu[MN_SELECT_CHARACTER].nField[0].fLButtonDown=true;
							}
						}return;

					default :	
						{
							for(y=0;y<SMenu[index].nFieldCount;y++)
							{
					 			if(SMenu[index].nField[y].nImageNumber==64 || ( SMenu[index].nField[y].nImageType ==RESET_JOB_ETC && SMenu[index].nField[y].nImageNumber == 10 )  )	
								{
									if(comp)
									{
										SMenu[index].nField[y].fCheakFlag=TRUE;
										SMenu[index].nField[y].fLButtonDown=FALSE;
										return;
									}
									SMenu[index].nField[y].fLButtonDown=TRUE;
									return;
								}
							}
						}break;
					}
				}break;
			case 3 :	/* ESC key */
				{
					switch(index)
					{
					case MN_YESNO :	if(comp)
									{
										SMenu[MN_YESNO].nField[1].fCheakFlag=TRUE;
										SMenu[MN_YESNO].nField[1].fLButtonDown=FALSE;
									}
									else SMenu[MN_YESNO].nField[1].fLButtonDown=TRUE;
									return;

	//							case MN_LOGIN :	if(comp)
	//											{
	//												SMenu[MN_LOGIN].nField[2].fCheakFlag=TRUE;
	//												SMenu[MN_LOGIN].nField[2].fLButtonDown=FALSE;
	//											}
	//											else SMenu[MN_LOGIN].nField[2].fLButtonDown=TRUE;
	//											return;
					case MN_SYSTEM_LOGOUT : if(comp)
											{
												SMenu[MN_SYSTEM_LOGOUT].nField[1].fCheakFlag=true;
												SMenu[MN_SYSTEM_LOGOUT].nField[1].fLButtonDown=false;
											}
											else
											{
												SMenu[MN_SYSTEM_LOGOUT].nField[1].fLButtonDown=true;
											}
											return;
					case MN_MAINSTART_START :	if(comp)
												{
													SMenu[MN_MAINSTART_START].nField[2].fCheakFlag=TRUE;
													SMenu[MN_MAINSTART_START].nField[2].fLButtonDown=FALSE;
												}
												else SMenu[MN_MAINSTART_START].nField[2].fLButtonDown=TRUE;
												return;

					case MN_LOGON :	if(comp)
									{
										SMenu[MN_LOGON].nField[1].fCheakFlag=TRUE;
										SMenu[MN_LOGON].nField[1].fLButtonDown=FALSE;
									}
									else SMenu[MN_LOGON].nField[1].fLButtonDown=TRUE;
									return;

					case MN_INFO :
						{
							SMenu[index].bActive = false;
							SetFocus2(HWND_1);//021001 lsw
							::CallLoginMenu();
						}return;

					default :	
						{
							if( index == MN_MAKECHARACTER_GENDER ) 
							{
								break;
							}
							for(y=0;y<SMenu[index].nFieldCount;y++)
							{
								if(SMenu[index].nField[y].nImageNumber==65 || SMenu[index].nField[y].nImageNumber==33 || SMenu[index].nField[y].nImageNumber==837 )
								{
									if(comp)
									{
										SMenu[index].nField[y].fCheakFlag=TRUE;
										SMenu[index].nField[y].fLButtonDown=FALSE;
										return;
									}
									SMenu[index].nField[y].fLButtonDown=TRUE;
									return;
								}
							}
						}break;		// 일부러...
					}
				}break;
			case 1 :/* TAB */
				{
					if(flag == 1)
					{
						if( index == MN_SELECT_CHARACTER )
						{
							for( int a=4; a<=7; a++ )
							{														
								if( SMenu[ index ].nField[a].nType == FT_NOMAL_PUT )
								{
									int temp;
									if( a == 7 ) temp = 4;
									else temp = a+1;
									SMenu[ index ].nField[ temp ].fCheakFlag = true;
									return;
								}
							}									
						}
						if( index == MN_MAKECHARACTER_NAME )
						{
							SMenu[index].nField[10].fCheakFlag = TRUE; // 에디트 박스 띄우기
							return;
						}

						if( index == MN_SELECT_NATION )
						{
							//< kjy-040325
							if( g_GameInfo.preventSelectYilse == 0 )
							{
								switch( SMenu[index].work )
								{
									case 0:
									case 100 :	SMenu[index].nField[8].fCheakFlag = true; break;

									case N_VYSEUS : SMenu[index].nField[8].fCheakFlag = true; break;
									case N_ZYPERN : SMenu[index].nField[10].fCheakFlag = true; break;
									case N_YILSE : SMenu[index].nField[9].fCheakFlag = true; break;
								}
								return;
							}
							else
							{
								switch( SMenu[index].work )
								{
									case 0:
									case 100 :	SMenu[index].nField[9].fCheakFlag = true; break;

									case N_VYSEUS : SMenu[index].nField[9].fCheakFlag = true; break;
									case N_ZYPERN : SMenu[index].nField[10].fCheakFlag = true; break;
									//case N_YILSE : SMenu[index].nField[9].fCheakFlag = true; break;
								}
								return;
							}
							//> kjy-040325 
						}

						for( y=0; y<SMenu[index].nFieldCount; y++ )
						{
							if(SMenu[index].nField[y].nType == FT_HIDE_NOMAL_ONE_PUT)
							{
								for(k=SMenu[index].nField[y].nSHideNomalStart ; k< SMenu[index].nField[y].nSHideNomalStart+SMenu[index].nField[y].nShideNomalPlus ; k++)
								{
									if(SHideNomal[SMenu[index].nField[y].nSHideNomalNumber][k].bShow)
									{
										group = SMenu[index].nField[y].nWillDo;
										SHideNomal[SMenu[index].nField[y].nSHideNomalNumber][k].bCheak = FALSE;
										k++;
										if(k == SMenu[index].nField[y].nSHideNomalCount)
										{
											SMenu[index].nField[y].nSHideNomalStart = 0;
											SMenu[index].nField[group].nSHideNomalStart = 0;
											k=0;
										}
										SHideNomal[SMenu[index].nField[y].nSHideNomalNumber][k].bCheak = TRUE;
										if(SMenu[MN_MAKECHARACTER_BASIC].bActive)
											SCharSource.nCharacterData[SMenu[index].nField[y].nSHideNomalNumber]=k;
											
										if(k == SMenu[index].nField[y].nSHideNomalStart + SMenu[index].nField[y].nShideNomalPlus)
										{
											SMenu[index].nField[y].nSHideNomalStart += SMenu[index].nField[y].nShideNomalPlus;
											SMenu[index].nField[group].nSHideNomalStart += SMenu[index].nField[group].nShideNomalPlus;
										}
										return;
									}
								}
								if(SMenu[MN_MAKECHARACTER_BASIC].bActive)
								{
									SHideNomal[SMenu[index].nField[y].nSHideNomalNumber][SMenu[index].nField[group].nSHideNomalStart].bCheak = TRUE;
									SCharSource.nCharacterData[SMenu[index].nField[y].nSHideNomalNumber]=SMenu[index].nField[group].nSHideNomalStart;
									return;
								}
								break;
							}
						}
					}
				}break;
			}
		}
	}
}


//** 떠오르는 글씨 출력을 위해
char SpaceBarPress()
{
	static char flag = '0';
	if(g_aCurrentKeys[DIK_P] & 0x80)
	{
		if( flag == 'r') return '0';
		flag = 'r';
		return flag ;
	}
	if(g_aCurrentKeys[DIK_O] & 0x80)
	{
		if( flag == 'o') return '0';
		flag = 'o';
		return flag ;
	}
	if(g_aCurrentKeys[DIK_I] & 0x80)
	{
//		if( flag == 'i') return '0';
		flag = 'i';
		return flag ;
	}
	if(g_aCurrentKeys[DIK_NUMPADPLUS] & 0x80)
	{
//		if(flag == 'p') return '0';
		flag = 'p';
		return flag;
	}
	if(g_aCurrentKeys[DIK_NUMPADMINUS] & 0x80) 
	{
//		if(flag == 'm') return '0';
		flag = 'm';
		return flag;
	}
	flag = '0';
	return flag;
}

void LoadChImageNumber( char *filename, char **buf, Spr Ani[][80], int max_ani[7] )
{			
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	Spr  *s;
		
	fp = Fopen( filename, "rb" );
	if( fp == NULL ) return ;

	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR );  // 오프셋 스킵
	sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;
	
	MemFree( *buf );
	MemAlloc( *buf, sprfilelength );

	for( int a=1; a<7; a++ )
	{
		for( int b=1; b<=max_ani[a]; b++ )
		{
			s = &Ani[a][b];
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
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),sizeof( unsigned int ), 1, fp);
		
			if( nOff + s->size >= sprfilelength ) 
			{
				JustMsg( lan->OutputMessage(5,60),filename, a+1, b );//lsw
				exit(0);
			}
			fread( *buf + nOff, s->size, 1, fp);
			s->img = *buf + nOff;
			convert565to555( s );
			nOff += size;
		}	
	}	
	
	fclose(fp);
}		

void LoadChImage()
{
	int max_ani[2][7] = {	{ 0,18,18,19,32,22,22 },
							{ 0,46,60,35,30,39,52 }, };

	int max_ani2[7] = {	 0,55,60,35,30,39,52 };


//	LoadChImageNumber( "./character/armor_befor.ani", &ani_buf[0], ch_armor_image_befor, max_ani[0] );
	LoadChImageNumber( "./character/armor_after.ani", &ani_buf[1], ch_armor_image_after, max_ani[1] );
//	LoadChImageNumber( "./character/body_befor.ani" , &ani_buf[2], ch_body_image_befor , max_ani[0] );
	LoadChImageNumber( "./character/body_after.ani" , &ani_buf[3], ch_body_image_after , max_ani[1] );
//	LoadChImageNumber( "./character/line_befor.ani" , &ani_buf[4], ch_line_image_befor , max_ani[0] );
	LoadChImageNumber( "./character/line_after.ani" , &ani_buf[5], ch_line_image_after , max_ani[1] );

//	LoadChImageNumber( "./character/armor_befor_b.dat", &ani_buf[6], ch_armor_image_befor_b, max_ani[0] );
	LoadChImageNumber( "./character/armor_after_b.dat", &ani_buf[7], ch_armor_image_after_b, max_ani2 );
//	LoadChImageNumber( "./character/body_befor_b.dat" , &ani_buf[8], ch_body_image_befor_b , max_ani[0] );
	LoadChImageNumber( "./character/body_after_b.dat" , &ani_buf[9], ch_body_image_after_b , max_ani2 );
//	LoadChImageNumber( "./character/line_befor_b.dat" , &ani_buf[10], ch_line_image_befor_b , max_ani[0] );
	LoadChImageNumber( "./character/line_after_b.dat" , &ani_buf[11], ch_line_image_after_b , max_ani2 );


}

void FreeChImage()
{	
	int i;
	for( i=0; i< 12 ; i++)
	{
		MemFree( ani_buf[i] );
	}
}		

void MakeCharacterAbility()
{
	g_GoDice = false;
	g_GoDice2 = false;

	CallServer( CMD_CREATE_ABILITY );

	for( int i=0; i<5; i++ )	nDiceNumber[i] = 1;
	return;
/*
	for(int i=STR; i<=LUCK; i++)
	{
		SCharSource.nCharacterAbility[i] = 10;
		SCharSource.nCharacterAbility[i] += ran(3)+1;
	}
	for( i=0; i<5; i++ )	nDiceNumber[i] = 1;*/
}


int ImgToAbility( int num_to_img[10], int img )
{
	for(int i=0; i<11; i++)
		if( num_to_img[i] == img ) return i;

	return 0;
}

void DoLButtonDoubbleClickOfStartMenu( int i, int j )
{
	SMENU *SubMenu = &SMenu[i];
	switch( SMenu[i].nField[j].nType )
	{
		case FT_HIDE_NOMAL_ONE_PUT :{
										for(int y=0;y<SMenu[i].nFieldCount;y++)
										{
											if(SMenu[i].nField[y].nImageNumber==64 || ( SMenu[i].nField[y].nImageType==RESET_JOB_ETC && SMenu[i].nField[y].nImageNumber==10 ))
											{
												SMenu[i].nField[y].fCheakFlag = true;
												break;
											}
										}
										break;
									}
		case FT_SELECT_NATION: {
									SubMenu->work = SubMenu->nField[j].nWillDo;
									SubMenu->nField[4].fCheakFlag = true;
									break;
							   }
		case FT_NOMAL_PUT :		
		case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT :	
			switch( SMenu[i].nField[j].nSpecialWillDo )
			{
				case SWD_CHARACTER_SELECT : 
					{
						for( int a=4; a<=7; a++ )
						{														
							SMenu[i].nField[a].nType= FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
						}
						SMenu[i].nField[j].nType= FT_NOMAL_PUT;
						character_active = SMenu[i].nField[j].nWillDo;
						SCharSource = LoadCh[ character_active ];

						SMenu[i].nField[0].fCheakFlag = true;	// ok button on
					}break;													
			}
			break;
		case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2 : switch( SMenu[i].nField[j].nSpecialWillDo )
												 {
													 case SWD_WILLDO_WORK : if( CheckNationCity( SMenu[i].nField[j].nWillDo ) == OK )
																			{
																				SMenu[i].work = SMenu[i].nField[j].nWillDo;
																				SMenu[i].key = SMenu[i].nField[j].x*1000+SMenu[i].nField[j].y;		// 좌표넘겨 줄때
																				StartPosition = SMenu[i].work;
																				SMenu[i].nField[0].fCheakFlag = true;
																			}
																			else
																			{
																				MP3( SN_WARNING );
																			}
																			break;

												 }
												break;
							
	}
}


void SetDamageOfHero( SCharacter *ch )
{
	int level_to_para[5] = { 10, 20, 40, 70, 90 };
	int level_count[6] = { 7,3,7,5,5,3 };
	
	int level_dmg[6][7][2] = 
	{ 
		{ {15,2}, {10,3}, {6,5}, {5,6}, {3,10}, {2,15}, {1,30}, },
		{ {7,5}, {5,7}, {1,35}, },
		{ {20,2}, {10,4}, {8,5}, {5,8}, {4,10}, {2,20}, {1,40}, },
		{ {15,3}, {9,5}, {5,9}, {3,15}, {1,45}, },
		{ {25,2}, {10,5}, {5,10}, {2,25}, {1,50}, },
		{ {11,5}, {5,11}, {1,55}, },
	};
	int luck_dmg[6] = { 30, 60, 90, 140, 180, 190 };

	int lv = 5;				// level에 의한 대미지 선택 경우의 수
	int i;
	for(i=0; i<5; i++ )
	{
		if( ch->nLevel <= level_to_para[i] )
		{
			lv = i;
			break;
		}
	}

	int luck_count = 7; // 최대 7번의 기회를 준다.
	for( i=0; i<6; i++ )
	{
		if( ch->nCharacterAbility[LUCK] < luck_dmg[i] )
		{
			luck_count = i+1;
			break;
		}
	}


	int temp;
	int min = 100;
	for( i=0; i<luck_count; i++)		// 최대 기회를 줘서 가장 작은 수를 뽑아냄
	{
		temp = rand()%level_count[lv];
		if( temp < min ) min = temp;
	}

	ch->dmg_count = level_dmg[lv][min][0];		// 결론!
	ch->dmg_max = level_dmg[lv][min][1];
}

extern void LoadManList();
extern void LoadMapInfo();



void InitStartMenu()
{	// thai2 YGI
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) && GetSysInfo( SI_NOT_CONNECT_MODE ) )
	{	//< CSD-030324
		LoadFilterWord();
	}	//> CSD-030324
#endif
	LoadManList();
	///////////////////////////////////////////////////////
	//RegistProgram();
	///////////////////////////////////////////////////////
	MP3( SN_START , 1 );
	g_GoDice = false;
	g_GoDice2 = false;

	ChatMgr.SetViewChat(CHATTYPE_NORMAL		, true);//020702 lsw
	ChatMgr.SetViewChat(CHATTYPE_GUILD		, true);//021001 lsw
	ChatMgr.SetViewChat(CHATTYPE_PARTY		, true);// Finito
	ChatMgr.SetViewChat(CHATTYPE_WHISPER	, true);//021001 lsw

	SCharSource1.body_r = SCharSource.body_r = -1;

	LoadChImage();
	// 020701 YGI
	LoadOpenningSpr( "./data/openning.dat", &open_spr_buf, open_spr, 75 );

	LoadOpenningSpr( "./data/face_man_b.dat", &facebuf_man_b, spr_face_man_b, FACE_MAX );			// 캐릭터 얼굴 이미지 로드 남녀,
	LoadOpenningSpr( "./data/face_woman_b.dat", &facebuf_woman_b, spr_face_woman_b, FACE_MAX);			// 캐릭터 얼굴 이미지 로드 남녀,
	LoadOpenningSpr( "./data/face_man_m.dat", &facebuf_man_m, spr_face_man_m, FACE_MAX);			// 캐릭터 얼굴 이미지 로드 남녀,
	LoadOpenningSpr( "./data/face_woman_m.dat", &facebuf_woman_m, spr_face_woman_m, FACE_MAX);			// 캐릭터 얼굴 이미지 로드 남녀,
	//	0906 kkh 추가 
	LoadOpenningSpr( "./data/face_man_b_add.dat", &facebuf_man_b_dash, spr_face_man_b_dash, ADD_CHA_PIC);			// 캐릭터 얼굴 이미지 로드 남녀,
	LoadOpenningSpr( "./data/face_woman_b_add.dat", &facebuf_woman_b_dash, spr_face_woman_b_dash, ADD_CHA_PIC);			// 캐릭터 얼굴 이미지 로드 남녀,
	LoadOpenningSpr( "./data/face_man_m_add.dat", &facebuf_man_m_dash, spr_face_man_m_dash, ADD_CHA_PIC);			// 캐릭터 얼굴 이미지 로드 남녀,
	LoadOpenningSpr( "./data/face_woman_m_add.dat", &facebuf_woman_m_dash, spr_face_woman_m_dash, ADD_CHA_PIC);			// 캐릭터 얼굴 이미지 로드 남녀,
	
	LoadOpenningSpr( "./data/alphabet.dat", &buf_alphabet, spr_alphabet, 36 );			// 그림문자

	LoadMenuStr();
	LoadMapInfo();

#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
		guild_mark_main.LoadBMP( "./temp.bmp" );
#endif

	LoadMenuImage();		// 메뉴 관련 이미지 데이타

#ifdef LIGHT_VERSION_
	g_pLightVersion = new CLightVersion;
	g_pLightVersion->LoadLightVersionMap();
#endif
}

// 020701 YGI
void LoadOpenningSpr( char *filename, BYTE **buf, Spr *temp_spr, int nMaxCount )
{
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	Spr  *s;

	FILE *fp = Fopen( filename, "rb" );
	if( fp == NULL ) return ;

	fread( &no, 2,1, fp);
	if( nMaxCount )
	{
		if( no > nMaxCount ) Error( "Error Image count [%s]", filename );
	}
	fseek( fp, no * 4, SEEK_CUR );  // 오프셋 스킵
	sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;
	
	SAFE_DELETE_ARRAY(*buf);
	*buf = new BYTE[sprfilelength];

	for( int a=0; a<no; a++ )
	{
		s = &temp_spr[a];
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
		fread( &(s->ox),	sizeof( short ), 1, fp);
		fread( &(s->oy),	sizeof( short ), 1, fp);
		fread( &(s->size),sizeof( unsigned int ), 1, fp);
	
		if( nOff + s->size >= sprfilelength ) 
		{
			JustMsg( lan->OutputMessage(6,60),filename );//lsw
			exit(0);
		}
		fread( *buf + nOff, s->size, 1, fp);
		s->img = (char *)*buf + nOff;
		convert565to555( s );
		nOff += size;
	}	
	
	fclose(fp);
}		



void FreeOpenningSpr( BYTE **buf )
{
	SAFE_DELETE_ARRAY(*buf);
}


int CalCreateMoney( const SCharacter *s)
{
	switch( s->nCharacterData[SPELL] )
	{
		case WARRIOR :	return 1000;
		case THIEF :	return 1000;
		case ARCHER :	return 1000;
		case WIZARD :	return 1000;
		case PRIEST :	return 1000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////
//
void LoadJobExplain()
{
	FILE *fp;
	fp = Fopen( "./data/job_ex.txt", "rt" );
	if( fp )
	{
		for( int i=0; i<7; i++ )
		{
			int j=0;
			while( j<6 )
			{
				fgets( job_explain[i][j], 1000, fp );
				EatRearWhiteChar( job_explain[i][j] );
				if( job_explain[i][j][0] == ';' ) continue;
				j++;
			}
		}
		fclose( fp );
	}
}

void SetJobExplainDetect( int job, int width, char explain[5][11][200], WORD color[5][11] )
{
	//char job_explain[7][5][1000];
	WORD c[] = { RGB16(255, 0, 0), RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, FONT_COLOR_NUMBER };
	WORD c2[] = { RGB16(255, 0, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), RGB16( 0, 255, 0 ) };
	static int old_job=-1;
	if( job == old_job ) return;

	old_job = job;

	char *ex=job_explain[job][0];
	int ct = 0;
	int count=0;
	for( int i=0; i<5; i++ ) for( int j=0; j<11; j++ ) explain[i][j][0] = NULL;
	WORD *cl;
	if( !job ) cl = c;
	else cl = c2;

	while( ex )
	{
		for( int i=0; i<11; i++ )
		{
			color[ct][i] = cl[count];
			char *ex2 = DivideStringByRect( width, ex, 1 );
			if( !ex2 || ex2 == ex )
			{
				count++;
				strcpy( explain[ct][i], ex );
				if( count >= 6 ) return;
				ex = job_explain[job][count];
			}
			else 
			{
				int len = ex2 - ex;
				strncpy( explain[ct][i], ex, len );
				explain[ct][i][len] = NULL;
				ex = ex2;
			}
		}
		ct++;
	}
}


void LoadNationExplain( )
{
	FILE *fp;
	fp = Fopen( "./data/nation_ex.txt", "rt" );
	if( fp )
	{
		for( int i=0; i<7; i++ )
		{
			int j=0;
			while( j<11 )
			{
				fgets( nation_explain[i][j], 1000, fp );
				EatRearWhiteChar( nation_explain[i][j] );
				if( nation_explain[i][j][0] == ';' ) continue;
				j++;
			}
		}
		fclose( fp );
	}
}

void SetNationExplainDetect( int nation, int width, char explain[7][11][200], WORD color[7][11] )
{
	nation -= 2;
	if( nation < 0 ) nation = 0;
	WORD c[] = { RGB16(255, 0, 0), RGB16(255, 255, 0), FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER };
	WORD c2[] = { RGB16(255, 0, 0), RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER};
	static int old_nation=-1;
	if( nation == old_nation ) return;

	old_nation = nation;

	char *ex=nation_explain[nation][0];
	int ct = 0;
	int count=0;
	for( int i=0; i<7; i++ ) for( int j=0; j<11; j++ ) explain[i][j][0] = NULL;
	WORD *cl;
	if( !nation ) cl = c;
	else cl = c2;

	while( ex )
	{
		for( int i=0; i<11; i++ )
		{
			color[ct][i] = cl[count];
			char *ex2 = DivideStringByRect( width, ex, 1 );
			if( !ex2 || ex2 == ex )
			{
				count++;
				strcpy( explain[ct][i], ex );
				if( count >= 11 ) return;
				ex = nation_explain[nation][count];
			}
			else 
			{
				int len = ex2 - ex;
				strncpy( explain[ct][i], ex, len );
				explain[ct][i][len] = NULL;
				ex = ex2;
			}
		}
		ct++;
	}
}


void OpenSelectNationMenu()
{
	for(int k=0;k<MAX_START;k++) SMenu[menu[k]].bActive=FALSE;
	SMenu[MN_MAKECHARACTER_BASIC].bActive=TRUE;
	SMenu[MN_NATION_EXPLAIN].bActive = true;
	SMenu[MN_SELECT_NATION].bActive = true;
	SMenu[MN_SELECT_NATION].work = 100;		// 일단 중립국
	//SMenu[MN_RESET_JOB_EXPLAIN].bActive = true;
	MP3( SN_MENU_OPEN );
	y_MenuFocus = 0;
}

#ifdef LIGHT_VERSION_
	#define __CHECK_LIGHT_VERSION__	return 0;
#else
	#define __CHECK_LIGHT_VERSION__
#endif

int CheckNationCity( int city )
{
	switch( city )
	{
		case SC_BAISUS_IMPEL	:	__CHECK_LIGHT_VERSION__
									if( SCharacterData.nation == N_VYSEUS && SCharacterData.nLevel >= 41 ) return OK;
									break;
		case SC_IRAMUS			:	__CHECK_LIGHT_VERSION__
									if( SCharacterData.nation == N_VYSEUS && SCharacterData.nLevel >= 31 ) return OK;
									break;
		case SC_HELTERNT_VILLAGE:	if( SCharacterData.nation == N_VYSEUS ) return OK;
									break;
		case SC_DIPH_UBEN		:	__CHECK_LIGHT_VERSION__
									if( SCharacterData.nation == N_ZYPERN && SCharacterData.nLevel >= 41 ) return OK;
									break;
		case SC_TYREN			:	__CHECK_LIGHT_VERSION__
									if( SCharacterData.nation == N_ZYPERN && SCharacterData.nLevel >= 31 ) return OK;
									break;
		case SC_RENUS			:	if( SCharacterData.nation == N_ZYPERN ) return OK;
									break;
		case SC_BARANTAN		:	if( SCharacterData.nation == N_YILSE ) return OK;
									break;

		default : return NOT;
	}
	if( SCharacterData.nation == N_NEUTRAL ) return OK;
	return NOT;
}

// thai2 YGI
void PrintThaiWord( char *str )
{	//< CSD-030324
	return ;
#ifndef THAI_LOCALIZING_
	return ;
#endif

	if( !GetSysInfo( SI_GAME_MAKE_MODE ) ) return;

	FILE *fp;
	fp = fopen( "./thai_word.txt", "at++" );
	if( !fp ) return;


	fprintf( fp, "\n\n#####################################\n" );
	fprintf( fp, "%s\n", str );

	BYTE rgbBrk[256] = {0,};
	ThaiBreak( str ,rgbBrk);
	int cn = 0;
	int iNow =0;
	char word[256];
	while( rgbBrk[cn] )
	{
		memcpy( word, &str[iNow], rgbBrk[cn] );
		word[rgbBrk[cn]] = 0;
		iNow += rgbBrk[cn];
		fprintf( fp, "%s\n", word );
		cn++;
	}

	fclose( fp );
}	//> CSD-030324