#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <math.h>
#include <algorithm>

#include "dragon.h"
#include "hong_sprite.h"
#include "char.h"
#include "Map.h"
#include "item.h"
#include "directsound.h"
#include "Hong_Sub.h"
#include "gameProc.h"
#include "CharDataTable.h"
#include "effect.h"
#include "Hong_Light.h"
#include "Convert565to555.h"
#include "Hangul.h"
#include "Menu.h"
#include "network.h"
#include "Tool.h"
#include "itemtable.h"
#include "Npc_Pattern.h"
#include "SmallMenu.h"
#include "StrikeSys.h"
#include "MagicSys.h"
#include "CombatSys.h"
#include "NumberAniMgr.h"
#include "MergeData.h"	// 030821 kyo
#include "TextOutMgr.h"

CMergeData g_MergeData;	// 030821 kyo

using namespace battle;

#define	TILE_SIZE						32
#define M_PI							3.141592653589
#define LIGHT_TERMINATE					2					//언제부터 광원 Out 시킬까?(총프레임의 1/2)
#define FIRE_DELAY						800					//발사지연시간 800밀리초
#define FIRE_DELAY2						8					//발사지연시간 8초  // 010204 YGI
#define ARROW_SPEED						32

#define SN_CLOCKBELL					66
//RectImageOn->마법 활성화 표시 전역변수
TFileMgr<CEffectTbl, MAX_EFFECT_COUNT> g_lpET;
TFileMgr<CEffectListTbl, MAX_EFFECT_COUNT> EffList; // CSD-030123

LPCHARACTER		attacked_Char=NULL;
EFFSPRITE		Effspr[MAX_EFFECT_COUNT];
bool			EffsprLoaded[ MAX_EFFECT_COUNT];
int				effSpr_Count[MAX_EFFECT_COUNT];				//현재 보여지고 있는 동일한 스프라이트의 갯수
DWORD			g_LoadSpriteTime[MAX_EFFECT_COUNT];
TFileMgr<CMagicTbl, MAX_MAGIC> g_lpMT;
DWORD			g_FireTime;
MAGICLIST*	g_lpML = NULL;

TFileMgr<CEffectTbl2, MAX_EFFECT_COUNT> g_lpET2;		//020808 YGI 

extern void SaveEffectAnimationAll( char *);
extern bool LoadEffectAnimationAll();

extern int g_CurrCharStatus;
extern void DoLButtonDown();

extern CHARACTERLIST		g_CharacterList;
extern LPITEMGROUND ReturnMagicCheckedItem( bool range_Check, MAGICLIST* lpML, int x, int y, int fire_Height, int xl, int yl );

void CollisionEffect_Keeping(USEEFFECTLIST* lpEL, MAGICLIST*	lpML, const int nRan); // 030519 kyo
bool SaveEffectSpriteAll(); // 030821 kyo 


////////////////////// 0811 YJS ////////////////////
extern void Fight_Stone_End(void);

HRESULT	out_Result;				//스프라이트 출력 결과 

//						Down / LeftDown / Left / LeftUp /  Up   / RightUp / Right / RightDown 
int point_Modify[8][2]={{0,32},{-32,26},{-42,-6},{-32,-32},{0,-48},{32,-32},{42,-6},{32,26}};	//인물의 8방향에 대한 수정좌표
//int point_Modify[8][2]={{0,24},{-36,0},{-52,-28},{36,-58},{0,-70},{36,-58},{52,-28},{36,0}};	//인물의 8방향에 대한 수정좌표
int lightning_Point[20][2]={{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,2},{-1,2},{-2,1},{-2,0},{-2,-1},{-1,-2},{0,-2},{1,-2},{2,-1},{2,0},{2,1},{1,2}};	//지역 벼락 출력을 위한 테이블
int fish_Point_Pattern1[14][2]={{0,-1},{-1,-2},{-2,-3},{-3,-5},{-3,-8},{-3,-11},{-4,-12},{-6,-13,},{-8,-11},{-9,-7},{-9,-4},{-10,1},{-10,0}};
int	fish_Point_Pattern2[14][2]={{0,-1},{1,-2},{2,-3},{3,-5},{3,-8},{3,-11},{4,-12},{6,-13,},{8,-11},{9,-7},{9,-4},{10,1},{10,0}};

bool array_Check[20]={0,};

int g_SkillNum=0;

static short FreeEffectSpriteFlagCount;						//이펙트 스프라이트를 Free하기 위한 counter(날려야 될 스프라이트 총갯수)
static short FreeEffectSpriteFlag[ MAX_EFFECT_COUNT*2 ];	//날려야될 스프라이트의 실제 번호를 기억하고 있는 변수(혹시 동시에 500이상을 프리해야하는 경우가 생긴다면? -> 그래서 X2를 했지롱)
int g_Condition_Table[8][8];

bool g_IsThunderStorm = false; //Eleval 12/08/09 - Thunderstorm
bool bLowRain = false; //Eleval 12/08/09 - Thunderstorm
int last_thunder_time = -1; //Eleval 12/08/09 - Thunderstorm
int thunder_wait_time = 0; //Eleval 12/08/09 - Thunderstorm

////////////////////// SoundUp lkh 추가 ////////////////////////
BGESOUND	g_BGES[MAX_MAP_];





////////////////////// 헥사 에디팅을 시도한 
void	SendHexaEditing(short int type, short int id)
{
	
	return ;
	
	t_packet	packet;
	packet.h.header.type = CMD_HEXAEDITING;
	packet.h.header.size = sizeof(t_client_editing_table);
	packet.u.angra.client_editing_table.type	= type;
	packet.u.angra.client_editing_table.id		= id;
	
	QueuePacket( &packet, 1 );
}

//step 1
// 시전자가 나인 경우만 출력되게 처리
////////////////////// SoundUp lkh 추가 ////////////////////////
int	ReturnRoonWord(LPCHARACTER caster, int magic_Num)
{
	//	if(caster!=Hero)	return 0;
	
	if( caster->id >= 10000 ) return 0;
	
	int wave = 0;
	
	
	if( caster->spell )
		//if(SCharacterData.nCharacterData[SPELL])	//성직자인 경우
	{
		
		if(caster->sex)		//남자
		{
			switch(rand()%3)
			{
			case 0:
				wave = 1803;
				break;
			case 1:
				wave = 1810;
				break;
			case 2:
				wave = 1811;
				break;
			}
		}
		else				//여자
		{
			switch(rand()%4)
			{
			case 0:
				wave = 1901;
				break;
			case 1:
				wave = 1909;
				break;
			case 2:
				wave = 1910;
				break;
			case 3:
				wave = 1911;
				break;
			}
		}
	}
	else										//마법사인 경우
	{
		switch(magic_Num)
		{
		case 1:
			{
				switch(rand()%2)
				{
				case 0:
					wave=1507;
					break;
				case 1:
					wave=1509;
					break;
				}
			}
			break;
		case 2:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1506;
					break;
				case 1:
					wave = 1512;
					break;
				case 2:
					wave = 1516;
					break;
				}
			}
			break;
		case 3:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1512;
					break;
				case 1:
					wave = 1516;
					break;
				case 2:
					wave = 1501;
					break;
				}
			}
			break;
		case 4:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1517;
					break;
				case 1:
					wave = 1501;
					break;
				}
			}
			break;
		case 5:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1516;
					break;
				}
			}
			break;
		case 6:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1516;
					break;
				case 2:
					wave = 1505;
					break;
				}
			}
			break;
		case 7:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1509;
					break;
				case 1:
					wave = 1517;
					break;
				}
			}
			break;
		case 10:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1502;
					break;
				}
			}
			break;
		case 11:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1520;
					break;
				case 1:
					wave = 1502;
					break;
				}
			}
			break;
		case 12:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1517;
					break;
				case 1:
					wave = 1502;
					break;
				}
			}
			break;
		case 13:
			wave = 1503;
			break;
		case 14:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1505;
					break;
				}
			}
			break;
		case 15:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1517;
					break;
				case 1:
					wave = 1519;
					break;
				case 2:
					wave = 1502;
					break;
				}
			}
			break;
		case 16:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1502;
					break;
				}
			}
			break;
		case 17:
			wave = 1520;
			break;
		case 18:
			wave = 1502;
			break;
		case 19:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;
		case 20:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1505;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;
		case 21:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1523;
					break;
				}
			}
			break;
		case 22:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1512;
					break;
				case 1:
					wave = 1513;
					break;
				}
			}
			break;
		case 23:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1508;
					break;
				case 1:
					wave = 1522;
					break;
				}
			}
			break;
		case 24:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1501;
					break;
				case 1:
					wave = 1517;
					break;
				}
			}
			break;
		case 25:
			wave = 1505;
			break;
		case 26:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1519;
					break;
				case 1:
					wave = 1522;
					break;
				}
			}
			break;
		case 27:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1512;
					break;
				case 1:
					wave = 1507;
					break;
				}
			}
			break;
		case 28:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1506;
					break;
				}
			}
			break;
		case 29:
		case 30:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1508;
					break;
				}
			}
			break;
		case 31:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1510;
					break;
				}
			}
			break;
		case 32:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1508;
					break;
				}
			}
			break;
		case 33:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1519;
					break;
				}
			}
			break;
		case 34:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;	
		case 35:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1512;
					break;
				case 2:
					wave = 1520;
					break;
				}
			}
			break;
		case 36:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1502;
					break;
				}
			}
			break;
		case 37:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1506;
					break;
				}
			}
			break;
		case 38:
		case 39:
		case 40:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1508;
					break;
				}
			}
			break;
		case 41:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1514;
					break;
				}
			}
			break;
		case 42:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;
		case 43:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1507;
					break;
				}
			}
			break;
		case 44:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1501;
					break;
				}
			}
			break;		
		case 45:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1503;
					break;
				case 1:
					wave = 1514;
					break;
				case 2:
					wave = 1505;
					break;
				}
			}
			break;
		case 46:
			wave = 1507;
			break;
		case 47:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1521;
					break;
				case 1:
					wave = 1506;
					break;
				}
			}
			break;
		case 48:
		case 49:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1521;
					break;
				case 1:
					wave = 1508;
					break;
				}
			}
			break;
		case 50:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1521;
					break;
				case 1:
					wave = 1511;
					break;
				}
			}
			break;
		case 51:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1521;
					break;
				case 1:
					wave = 1507;
					break;
				}
			}
			break;
		case 52:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1521;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;
		case 53:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1504;
					break;
				case 1:
					wave = 1522;
					break;
				}
			}
			break;
		case 54:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1521;
					break;
				case 1:
					wave = 1514;
					break;
				}
			}
			break;
		case 55:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1514;
					break;
				case 1:
					wave = 1503;
					break;
				}
			}
			break;
		case 56:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1514;
					break;
				case 1:
					wave = 1508;
					break;
				}
			}
			break;
		case 57:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1514;
					break;
				case 1:
					wave = 1521;
					break;
				}
			}
			break;
		case 58:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1514;
					break;
				case 1:
					wave = 1510;
					break;
				}
			}
			break;
		case 59:
		case 60:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1514;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;
		case 61:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1514;
					break;
				case 1:
					wave = 1505;
					break;
				case 2:
					wave = 1520;
					break;
				}
			}
			break;
		case 62:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1514;
					break;
				case 1:
					wave = 1521;
					break;
				}
			}
			break;
		case 63:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1514;
					break;
				case 1:
					wave = 1519;
					break;
				case 2:
					wave = 1520;
					break;
				}
			}
			break;
		case 64:
		case 65:
		case 66:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1512;
					break;
				case 1:
					wave = 1521;
					break;
				}
			}
			break;
		case 67:
		case 68:
		case 70:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1512;
					break;
				case 1:
					wave = 1505;
					break;
				case 2:
					wave = 1521;
					break;
				}
			}
			break;
		case 71:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1506;
					break;
				case 1:
					wave = 1505;
					break;
				}
			}
			break;
		case 73:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1510;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;
		case 74:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1522;
					break;
				case 1:
					wave = 1507;
					break;
				}
			}
			break;
		case 75:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1510;
					break;
				case 1:
					wave = 1511;
					break;
				}
			}
			break;
		case 76:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1512;
					break;
				case 1:
					wave = 1511;
					break;
				case 2:
					wave = 1520;
					break;
				}
			}
			break;
		case 77:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1506;
					break;
				case 1:
					wave = 1502;
					break;
				}
			}
			break;
		case 78:
			wave = 1522;
			break;
		case 79:
			wave = 1506;
			break;
		case 80:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1505;
					break;
				case 1:
					wave = 1524;
					break;
				}
			}
			break;
		case 81:
		case 82:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1520;
					break;
				case 1:
					wave = 1524;
					break;
				}
			}
			break;
		case 83:
			wave = 1515;
			break;
		case 84:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1505;
					break;
				case 1:
					wave = 1524;
					break;
				}
			}
			break;
		case 85:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1520;
					break;
				case 1:
					wave = 1524;
					break;
				}
			}
			break;
		case 86:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1520;
					break;
				case 1:
					wave = 1506;
					break;
				}
			}
			break;
		case 87:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1501;
					break;
				case 1:
					wave = 1508;
					break;
				}
			}
			break;
		case 88:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1520;
					break;
				case 1:
					wave = 1502;
					break;
				}
			}
			break;
		case 89:
			{
				switch(rand()%3)
				{
				case 0:
					wave = 1505;
					break;
				case 1:
					wave = 1512;
					break;
				case 2:
					wave =1524;
					break;
				}
			}
			break;
		case 90:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1512;
					break;
				case 1:
					wave = 1501;
					break;
				}
			}
			break;
		case 91:
			wave = 1501;
			break;
		case 92:
			wave = 1511;
			break;
		case 93:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1511;
					break;
				case 1:
					wave = 1509;
					break;
				}
			}
			break;
		case 94:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1511;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;
		case 95:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1518;
					break;
				case 1:
					wave = 1520;
					break;
				}
			}
			break;
		case 96:
			wave = 1511;
			break;
		case 97:
			wave = 1513;
			break;
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
			wave = 1521;
			break;
		case 105:
		case 106:
			wave = 1515;
			break;
		case 107:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1520;
					break;
				case 1:
					wave = 1521;
					break;
				}
			}
			break;
		case 108:
			wave = 1521;
			break;
		case 109:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1519;
					break;
				}
			}
			break;
		case 110:
			{
				switch(rand()%2)
				{
				case 0:
					wave = 1507;
					break;
				case 1:
					wave = 1521;
					break;
				}
			}
			break;
		default:
			break;
		}
	}
	return wave;
}

//step 2
// 시전자가 나인 경우만 출력되게 처리
////////////////////// SoundUp lkh 추가 ////////////////////////
int ReturnCastingWord(LPCHARACTER caster, int magic_Num)		//900부터 짝수번은 남자 시동어, 홀수번은 여자 시동어
{
	
	//	if(caster!=Hero)	return 0;			//일단 다른 캐릭의 시전어도 들리도록 처리
	
	
	if( caster->id >= 10000 ) return 0;
	
	int wave = 0;
	int class_Type = g_lpMT[magic_Num].order_Type;
	
	if(caster->spell )
		//if( SCharacterData.nCharacterData[SPELL])	//성직자인 경우(구분없이 랜덤)
	{
		if(caster->sex)		//남자
		{
			switch(magic_Num)
			{
			case 151:
			case 152:
			case 153:
			case 154:
			case 155:
			case 156:
			case 157:
			case 158:
				switch(rand()%2)
				{
				case 0:
					wave = 1820;
					break;
				case 1:
					wave = 1821;
					break;
				}
				break;
				case 159:
				case 160:
				case 161:
					wave = 1821;
					break;
				case 162:
				case 163:
					wave = 1822;
					break;
				case 164:
				case 165:
					wave = 1823;
					break;
				case 166:
					wave = 1824;
					break;
				case 167:
					wave = 1825;
					break;
				case 168:
				case 169:
				case 170:
					wave = 1824;
					break;
				case 171:
				case 172:
				case 173:
				case 174:
					switch(rand()%2)
					{
					case 0:
						wave = 1820;
						break;
					case 1:
						wave = 1821;
						break;
					}
					break;
					case 175:
					case 176:
					case 177:
					case 178:
						switch(rand()%2)
						{
						case 0:
							wave = 1824;
							break;
						case 1:
							wave = 1825;
							break;
						}
						break;
						case 179:
						case 180:
						case 181:
						case 182:
						case 183:
						case 184:
						case 185:
						case 186:
						case 187:
						case 188:
							wave = 1822;
							break;
						case 189:
						case 190:
						case 191:
						case 192:
							wave = 1825;
							break;
			}
		}
		else				//여자
		{
			switch(magic_Num)
			{
			case 151:
			case 152:
			case 153:
			case 154:
			case 155:
			case 156:
			case 157:
			case 158:
				switch(rand()%2)
				{
				case 0:
					wave = 1920;
					break;
				case 1:
					wave = 1921;
					break;
				}
				break;
				case 159:
				case 160:
				case 161:
					wave = 1921;
					break;
				case 162:
				case 163:
					wave = 1922;
					break;
				case 164:
				case 165:
					wave = 1923;
					break;
				case 166:
					wave = 1924;
					break;
				case 167:
					wave = 1925;
					break;
				case 168:
				case 169:
				case 170:
					wave = 1924;
					break;
				case 171:
				case 172:
				case 173:
				case 174:
					switch(rand()%2)
					{
					case 0:
						wave = 1920;
						break;
					case 1:
						wave = 1921;
						break;
					}
					break;
					case 175:
					case 176:
					case 177:
					case 178:
						switch(rand()%2)
						{
						case 0:
							wave = 1924;
							break;
						case 1:
							wave = 1925;
							break;
						}
						break;
						case 179:
						case 180:
						case 181:
						case 182:
						case 183:
						case 184:
						case 185:
						case 186:
						case 187:
						case 188:
							wave = 1922;
							break;
						case 189:
						case 190:
						case 191:
						case 192:
							wave = 1925;
							break;
			}
		}
	}
	else										//마법사인 경우(마법계열별 구분효과음)
	{
		if(caster->sex)		//남자
			wave = 1550 + (class_Type-1);
		else				//여자
			wave = 1570 + (class_Type-1);
	}
	return wave;
}

//step 3
////////////////////// SoundUp lkh 추가 ////////////////////////
int	ReturnStartingWord(LPCHARACTER caster, int magic_Num)		//마법사 1계열 시전어 남자-> 904
{
	if (magic_Num >= LIGHTNING_BOOM && magic_Num <= WIND_EXTREME)  return 0;
	if(caster!=Hero)	return 0;			//일단 다른 캐릭의 시전어도 들리도록 처리
	
	int wave = 0;
	int	class_Type = g_lpMT[magic_Num].order_Type;
	
	if(SCharacterData.nCharacterData[SPELL])	//성직자인 경우
	{
		if(caster->sex)		//남자
			wave = 1831 + (magic_Num-151);
		else				//여자
			wave = 1931 + (magic_Num-151);
	}
	else										//마법사인 경우
	{
		if (caster->sex)		//남자
		{	//< CSD-030603
			switch (magic_Num)
			{
			case 1: wave = 1642; break;
			case 2: wave = 1671; break;
			case 3: wave = 1665; break;
			case 4: wave = 1661; break;
			case 5: wave = 1617; break;
			case 6: wave = 1642; break;
			case 7: wave = 1673; break;
			case 10: wave = 1680; break;
			case 11: wave = 1615; break;
			case 12: wave = 1666; break;
			case 13: wave = 1683; break;
			case 14: wave = 1668; break;
			case 15: wave = 1614; break;
			case 16: wave = 1629; break;
			case 17: wave = 1694; break;
			case 18: wave = 1620; break;
			case 19: wave = 1682; break;
			case 20: wave = 1679; break;
			case 21: wave = 1628; break;
			case 22: wave = 1651; break;
			case 23: wave = 1667; break;
			case 24: wave = 1625; break;
			case 25: wave = 1648; break;
			case 26: wave = 1669; break;
			case 27: wave = 1635; break;
			case 28: wave = 1675; break;
			case 29: wave = 1633; break;
			case 30: wave = 1630; break;
			case 31: wave = 1678; break;
			case 32: wave = 1632; break;
			case 33: wave = 1611; break;
			case 34: wave = 1696; break;
			case 35: wave = 1634; break;
			case 36: wave = 1603; break;
			case 37: wave = 1676; break;
			case 38: wave = 1643; break;
			case 39: wave = 1644; break;
			case 40: wave = 1645; break;
			case 41: wave = 1637; break;
			case 42: wave = 1647; break;
			case 43: wave = 1612; break;
			case 44: wave = 1646; break;
			case 45: wave = 1637; break;
			case 46: wave = 1652; break;
			case 47: wave = 1677; break;
			case 48: wave = 1655; break;
			case 49: wave = 1654; break;
			case 50: wave = 1606; break;
			case 51: wave = 1662; break;
			case 52: wave = 1653; break;
			case 53: wave = 1693; break;
			case 54: wave = 1601; break;
			case 55: wave = 1685; break;
			case 56: wave = 1684; break;
			case 57: wave = 1616; break;
			case 58: wave = 1685; break;
			case 59: wave = 1640; break;
			case 60: wave = 1639; break;
			case 61: wave = 1685; break;
			case 62: wave = 1685; break;
			case 63: wave = 1685; break;
			case 64: wave = 1664; break;
			case 65: wave = 1659; break;
			case 66: wave = 1689; break;
			case 67: wave = 1692; break;
			case 68: wave = 1638; break;
			case 70: wave = 1690; break;
			case 71: wave = 1604; break;
			case 73: wave = 1623; break;
			case 74: wave = 1610; break;
			case 75: wave = 1624; break;
			case 76: wave = 1602; break;
			case 77: wave = 1670; break;
			case 78: wave = 1674; break;
			case 79: wave = 1670; break;
			case 80: wave = 1631; break; // case 80: wave = 1656; break;
			case 81: wave = 1660; break; // case 81: wave = 1618; break;
			case 82: wave = 1605; break;
			case 83: wave = 1641; break;
			case 84: wave = 1650; break;
			case 85: wave = 1619; break;
			case 86: wave = 1626; break;
			case 87: wave = 1695; break;
			case 88: wave = 1622; break;
			case 89: wave = 1613; break;
			case 90: wave = 1663; break;
			case 91: wave = 1657; break;
			case 92: wave = 1608; break;
			case 93: wave = 1609; break;
			case 94: wave = 1607; break;
			case 95: wave = 1687; break;
			case 96: wave = 1688; break;
			case 97: wave = 1658; break;
			case 100: wave = 1686; break;
			case 101: wave = 1672; break;
			case 102: wave = 1636; break;
			case 103: wave = 1681; break;
			case 104: wave = 1697; break;
			case 105: wave = 1649; break;
			case 106: wave = 1621; break;
			case 107: wave = 1627; break;
			case 108: wave = 1691; break;
			default:  wave = 0;    break;
			}
		}
		else				//여자
		{
			switch (magic_Num)
			{
			case 1: wave = 1742; break;
			case 2: wave = 1771; break;
			case 3: wave = 1765; break;
			case 4: wave = 1761; break;
			case 5: wave = 1717; break;
			case 6: wave = 1742; break;
			case 7: wave = 1773; break;
			case 10: wave = 1780; break;
			case 11: wave = 1715; break;
			case 12: wave = 1766; break;
			case 13: wave = 1783; break;
			case 14: wave = 1768; break;
			case 15: wave = 1714; break;
			case 16: wave = 1729; break;
			case 17: wave = 1794; break;
			case 18: wave = 1720; break;
			case 19: wave = 1782; break;
			case 20: wave = 1779; break;
			case 21: wave = 1728; break;
			case 22: wave = 1751; break;
			case 23: wave = 1767; break;
			case 24: wave = 1725; break;
			case 25: wave = 1748; break;
			case 26: wave = 1769; break;
			case 27: wave = 1735; break;
			case 28: wave = 1775; break;
			case 29: wave = 1733; break;
			case 30: wave = 1730; break;
			case 31: wave = 1778; break;
			case 32: wave = 1732; break;
			case 33: wave = 1711; break;
			case 34: wave = 1796; break;
			case 35: wave = 1734; break;
			case 36: wave = 1703; break;
			case 37: wave = 1776; break;
			case 38: wave = 1743; break;
			case 39: wave = 1744; break;
			case 40: wave = 1745; break;
			case 41: wave = 1737; break;
			case 42: wave = 1747; break;
			case 43: wave = 1712; break;
			case 44: wave = 1746; break;
			case 45: wave = 1737; break;
			case 46: wave = 1752; break;
			case 47: wave = 1777; break;
			case 48: wave = 1755; break;
			case 49: wave = 1754; break;
			case 50: wave = 1706; break;
			case 51: wave = 1762; break;
			case 52: wave = 1753; break;
			case 53: wave = 1793; break;
			case 54: wave = 1701; break;
			case 55: wave = 1785; break;
			case 56: wave = 1784; break;
			case 57: wave = 1716; break;
			case 58: wave = 1785; break;
			case 59: wave = 1740; break;
			case 60: wave = 1739; break;
			case 61: wave = 1785; break;
			case 62: wave = 1785; break;
			case 63: wave = 1785; break;
			case 64: wave = 1764; break;
			case 65: wave = 1792; break;
			case 66: wave = 1789; break;
			case 67: wave = 1738; break;
			case 68: wave = 1759; break;
			case 70: wave = 1790; break;
			case 71: wave = 1704; break;
			case 73: wave = 1723; break;
			case 74: wave = 1710; break;
			case 75: wave = 1724; break;
			case 76: wave = 1702; break;
			case 77: wave = 1770; break;
			case 78: wave = 1774; break;
			case 79: wave = 1770; break;
			case 80: wave = 1731; break; //case 80: wave = 1756; break;
			case 81: wave = 1760; break; //case 81: wave = 1718; break;
			case 82: wave = 1705; break;
			case 83: wave = 1741; break;
			case 84: wave = 1750; break;
			case 85: wave = 1719; break;
			case 86: wave = 1726; break;
			case 87: wave = 1795; break;
			case 88: wave = 1722; break;
			case 89: wave = 1713; break;
			case 90: wave = 1763; break;
			case 91: wave = 1757; break;
			case 92: wave = 1708; break;
			case 93: wave = 1709; break;
			case 94: wave = 1707; break;
			case 95: wave = 1787; break;
			case 96: wave = 1788; break;
			case 97: wave = 1758; break;
			case 100: wave = 1786; break;
			case 101: wave = 1772; break;
			case 102: wave = 1736; break;
			case 103: wave = 1781; break;
			case 104: wave = 1797; break;
			case 105: wave = 1749; break;
			case 106: wave = 1721; break;
			case 107: wave = 1727; break;
			case 108: wave = 1791; break;
			default:  wave = 0;    break;
			}
		}	//> CSD-030603
	}
	
	return wave;
}

////////////////////// SoundUp lkh 배경 랜덤 사운드 테이블 로딩 함수 //////////////////////
BOOL	LoadBGESoundTable(void)
{
	
	FILE*	fp;
	char *token;
	char buf[512];
	char  DataPath[FILENAME_MAX]="./data/BGE.DAT";
	int	mapnum=0;
	
	
	
	fp = Fopen( DataPath, "rt" );
	
	if(fp)
	{
		while(!feof(fp))
		{	
			fgets(buf, 511, fp);
			
			token = strtok( buf, "\t\n");
			if(token[0]=='~')
			{
				continue;
				//token = strtok(NULL, "\r");
			}
			
			g_BGES[mapnum].mapNum	= atoi(token);
			
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].dayPattern = atoi(token);
			
			//////////// 낮 데이터 /////////////
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[0] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[0] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[0] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[1] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[1] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[1] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[2] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[2] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[2] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[3] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[3] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[3] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[4] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[4] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[4] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[5] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[5] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[5] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[6] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[6] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[6] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[7] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[7] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[7] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[8] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[8] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[8] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Slot[9] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Rate[9] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].day_Type[9] = atoi(token);
			for(int	i=0; i<10; i++)
			{
				if( g_BGES[mapnum].day_Slot[i]!=0 )
					g_BGES[mapnum].use_DayNum++;
			}
			
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].nightPattern = atoi(token);
			
			//////////// 밤 데이터 /////////////g_
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[0] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[0] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[0] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[1] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[1] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[1] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[2] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[2] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[2] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[3] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[3] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[3] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[4] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[4] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[4] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[5] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[5] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[5] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[6] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[6] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[6] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[7] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[7] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[7] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[8] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[8] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Type[8] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Slot[9] = atoi(token);
			token = strtok( NULL, "\t\n");			g_BGES[mapnum].night_Rate[9] = atoi(token);
			token = strtok( NULL, "\t\n\r");		g_BGES[mapnum].night_Type[9] = atoi(token);
			for(int	ii=0; ii<10; ii++)
			{
				if(g_BGES[mapnum].night_Slot[ii]!=0)
					g_BGES[mapnum].use_NightNum++;
			}
			
			mapnum++;
		}
		
	}
	return	TRUE;
}

void StopBGE(void)
{
	for(int ii=0; ii<10; ii++)
	{
		StopWave(g_BGES[MapNumber].day_Slot[ii]);
		StopWave(g_BGES[MapNumber].night_Slot[ii]);
	}
}

static int day_night=99;
////////////////////// SoundUp lkh 추가 ///////////////////////////
void BackEffectSound2(int MapNumber)
{
	///////////////// 0813 lkh 추가 //////////////////
	if(!g_BGE_Flag)
	{
		StopBGE();
		return;
	}
	
	//낮과 밤으로 구분하여 출력
	int		patternType = 0;
	int		useSlotNum;
	
	
	
	/////////////////// 현 지역(맵)에 따른 랜덤효과음 연출 /////////////////
	int slot[10]={ 0,};				//슬롯 사운드 번호
	int rate[10]={ 0,};				//빈도
	int	type[10]={ 0,};				//출력타입
	int	i;
	if (DayLightControl > 31 ) { return; }	// 0910  YGI 
	
	if(DayLightControl > 15) // 0910 YGI
	{
		if(day_night!=DAY_)
		{
			patternType = g_BGES[MapNumber].nightPattern;
			useSlotNum	= g_BGES[MapNumber].use_NightNum;
			for(i=0; i<useSlotNum; i++)
			{
				slot[i] = g_BGES[MapNumber].night_Slot[i];
				rate[i] = g_BGES[MapNumber].night_Rate[i];
				type[i] = g_BGES[MapNumber].night_Type[i];
			}
			for(int z=0; z<useSlotNum; z++)
				if( IsPlaying(slot[z]) )
					StopWave(slot[z]);
				day_night = DAY_;
		}
	}
	else
	{
		if(day_night!=NIGHT_)
		{
			patternType = g_BGES[MapNumber].dayPattern;
			useSlotNum	= g_BGES[MapNumber].use_DayNum;
			for(i=0; i<useSlotNum; i++)
			{
				slot[i] = g_BGES[MapNumber].day_Slot[i];
				rate[i] = g_BGES[MapNumber].day_Rate[i];
				type[i] = g_BGES[MapNumber].day_Type[i];
			}
			for(int z=0; z<useSlotNum; z++)
				if( IsPlaying(slot[z]) )
					StopWave(slot[z]);
				day_night = NIGHT_;
		}
	}
	
	switch(day_night)
	{
		
	case NIGHT_:
		patternType = g_BGES[MapNumber].nightPattern;
		useSlotNum	= g_BGES[MapNumber].use_NightNum;
		for(i=0; i<g_BGES[MapNumber].use_NightNum; i++)
		{
			slot[i] = g_BGES[MapNumber].night_Slot[i];
			rate[i] = g_BGES[MapNumber].night_Rate[i];
			type[i] = g_BGES[MapNumber].night_Type[i];
		}
		break;
		
	default:
	case DAY_:
		patternType = g_BGES[MapNumber].dayPattern;
		useSlotNum	= g_BGES[MapNumber].use_DayNum;
		for(i=0; i<g_BGES[MapNumber].use_DayNum; i++)
		{
			slot[i] = g_BGES[MapNumber].day_Slot[i];
			rate[i] = g_BGES[MapNumber].day_Rate[i];
			type[i] = g_BGES[MapNumber].day_Type[i];
		}
		break;
	}
	
	switch(patternType)		//패턴에 따른 차별화된 효과음 연출
	{
	case 1:					//1 Pattern->각 슬롯의 효과음을 끝임 없이 연속적으로 랜덤하게 플레이
		for(i=0; i<useSlotNum; i++)
		{
			if(IsPlaying(slot[i]))	return;			//한개 슬롯의 효과음이라도 플레이 되고 있으면 리턴
		}
		PlayListAutoSounds(slot[rand()%useSlotNum], 0,rand()%4,0);
		break;
	default:				//특별한 패턴 없이 정해진 빈도에 따라 랜덤하게 출력
		for(i=0; i<useSlotNum; i++)
		{
			if(rate[i] == 999 && !IsPlaying(slot[i]) )	//루핑 효과음인 경우
			{
				PlayListAutoSounds(slot[i], 0, rand()%4, 1);
			}
			else if(rand()%10000<rate[i] && !IsPlaying(slot[i]) /*g_SoundInfo.lpDirectSoundBuffer[slot[i]] == NULL*/ )
			{
				PlayListAutoSounds(slot[i], 0, rand()%4, 0);
			}
		}	
		break;
	}
}	
////////////////////////////////////////////////////////////////


//////////////////////// 0625 lkh 추가 ////////////////////////////
void	ReturnDazeMove(int& dest_x, int& dest_y)
{
	int	temp_Dist_X, temp_Dist_Y;
	int distance_X, distance_Y;
	
	temp_Dist_X = abs(Hero->x - dest_x);
	temp_Dist_Y = abs(Hero->y - dest_y);
	
	if( temp_Dist_X < 32 )		temp_Dist_X=32;
	if( temp_Dist_Y < 32 )		temp_Dist_Y=32;
	
	distance_X = (temp_Dist_X*2) + ( ( rand() % temp_Dist_X / 2 ) - (temp_Dist_X/4) );
	distance_Y = (temp_Dist_Y*2) + ( ( rand() % temp_Dist_Y / 2 ) - (temp_Dist_Y/4) );
	
	switch(rand()%4)
	{
	case 0:		//x 대칭
		if(Hero->x > dest_x)
			dest_x += distance_X;
		else
			dest_x -= distance_X;
		break;
	case 1:		//y 대칭
		if(Hero->y > dest_y)
			dest_y += distance_Y;
		else
			dest_y -= distance_Y;
		break;
	case 2:		//x,y 대칭
		if(Hero->x > dest_x)
			dest_x += distance_X;
		else
			dest_x -= distance_X;
		if(Hero->y > dest_y)
			dest_y += distance_Y;
		else
			dest_y -= distance_Y;
		break;	
	case 3:		//random pattern
		dest_x = rand()%distance_X-distance_X/2;
		dest_y = rand()%distance_Y-distance_Y/2;
		break;
	}
}

////////////////////// 0618 lkh 수정 //////////////////////////
void BackEffectSound(int MapNumber)
{
	/////////////////// 현 지역(맵)에 따른 랜덤효과음 연출 /////////////////
	// 0->MA-IN, 1->K_SUNG2, 2->FIREDUN1, 3->ICE-W01, 4->SOURCE, 5->MANDUN, 6->SUNG_TILE_00
	int slot1=0, slot2=0, slot3=0, slot4=0, slot5=0, slot6=0;
	int per1=0, per2=0, per3=0, per4=0, per5=0, per6=0;
	
	int	distance1_X=0, distance1_Y=0, distance2_X=0, distance2_Y=0, distance3_X=0, distance3_Y=0;
	int distance4_X=0, distance4_Y=0, distance5_X=0, distance5_Y=0, distance6_X=0, distance6_Y=0;
	
	switch(MapNumber)
	{
	case 0:	//MA-IN->tot:16		//바람(210,211,212), 개자음(375), 닭자음(374), 소자음(376), Not Ability양자음(399)
		per1=2;			//4
		slot1=210;
		per2=1;			//3
		slot2=211;
		per4=1;
		slot4=375;
		distance4_X=rand()%41+80;
		if(rand()%2)	distance4_X = -distance4_X;
		distance4_Y=rand()%41+80;
		if(rand()%2)	distance4_Y = -distance4_Y;
		per5=2;
		slot5=374;
		distance5_X=rand()%41+80;
		if(rand()%2)	distance5_X = -distance5_X;
		distance5_Y=rand()%41+80;
		if(rand()%2)	distance5_Y = -distance5_Y;
		per6=1;
		slot6=376;
		distance6_X=rand()%41+80;
		if(rand()%2)	distance6_X = -distance6_X;
		distance6_Y=rand()%41+80;
		if(rand()%2)	distance6_Y = -distance6_Y;
		break;
		//case 1:	//K_SUNG2->tot:14	//바람,개자음,늑대자음(379) 
		//case 6:	//sung_tile
	case 4:		//SOURCE
	case 13:	//HU_VM
	case 14:	//GRAY
		per1=2;			//4
		slot1=210;
		per2=1;			//2
		slot2=211;
		per3=1;
		slot3=212;
		per4=1;
		slot4=375;
		distance4_X=rand()%41+80;
		if(rand()%2)	distance4_X = -distance4_X;
		distance4_Y=rand()%41+80;
		if(rand()%2)	distance4_Y = -distance4_Y;
		per5=1;
		slot5=379;
		distance5_X=rand()%41+80;
		if(rand()%2)	distance5_X = -distance5_X;
		distance5_Y=rand()%41+80;
		if(rand()%2)	distance5_Y = -distance5_Y;
		break;
	case 2:	//FIREDUN1->tot:18	//바람(중/강), 가고일자음(365), 좀비자음(360)
	case 7:	
	case 8:
		per1=3;			//5
		slot1=211;
		per2=1;			//2
		slot2=212;
		per3=2;
		slot3=365;
		distance3_X=rand()%41+80;
		if(rand()%2)	distance3_X = -distance3_X;
		distance3_Y=rand()%41+80;
		if(rand()%2)	distance3_Y = -distance3_Y;
		per4=1;
		slot4=360;
		distance4_X=rand()%41+80;
		if(rand()%2)	distance4_X = -distance4_X;
		distance4_Y=rand()%41+80;
		if(rand()%2)	distance4_Y = -distance4_Y;
		break;
	case 3:	//ICE-W01->tot: 	//바람(중/강), 독수리자음(400), 곰자음(366), 좀비자음(360)
	case 9:
	case 10:
		per1=3;			//5
		slot1=211;
		per2=1;			//2	
		slot2=212;
		per3=2;
		slot3=400;
		distance3_X=rand()%41+80;
		if(rand()%2)	distance3_X = -distance3_X;
		distance3_Y=rand()%41+80;
		if(rand()%2)	distance3_Y = -distance3_Y;
		per4=1;
		slot4=366;
		distance4_X=rand()%41+80;
		if(rand()%2)	distance4_X = -distance4_X;
		distance4_Y=rand()%41+80;
		if(rand()%2)	distance4_Y = -distance4_Y;
		per5=1;
		slot5=360;
		distance5_X=rand()%41+80;
		if(rand()%2)	distance5_X = -distance5_X;
		distance5_Y=rand()%41+80;
		if(rand()%2)	distance5_Y = -distance5_Y;
		break;
	case 5:	//MANDUN	//바람(약/중), 멧돼지자음(378), 오크자음(367), 관열리는소리(471)
	case 11:
	case 12:
		per1=3;			//5
		slot1=210;
		per2=1;			//2
		slot2=211;
		per3=1;
		slot3=378;
		distance3_X=rand()%41+80;
		if(rand()%2)	distance3_X = -distance3_X;
		distance3_Y=rand()%41+80;
		if(rand()%2)	distance3_Y = -distance3_Y;
		per4=2;
		slot4=367;
		distance4_X=rand()%41+80;
		if(rand()%2)	distance4_X = -distance4_X;
		distance4_Y=rand()%41+80;
		if(rand()%2)	distance4_Y = -distance4_Y;
		per5=1;
		slot5=471;
		distance5_X=rand()%41+80;
		if(rand()%2)	distance5_X = -distance5_X;
		distance5_Y=rand()%41+80;
		if(rand()%2)	distance5_Y = -distance5_Y;
		break;
	case 6:	//SUNG_TILE_00	//전투불가&몬스터발생안함
		break;
	}
	//slot 1 play
	if(rand()%1000<per1 && g_SoundInfo.lpDirectSoundBuffer[slot1] == NULL)
	{
		PlayListAutoSounds(slot1, distance1_X, distance2_Y, 0);
		return;
	}
	if(rand()%1000<per2 && g_SoundInfo.lpDirectSoundBuffer[slot2] == NULL)
	{
		PlayListAutoSounds(slot2, distance2_X, distance2_Y, 0);
		return;
	}
	if(rand()%1000<per3 && g_SoundInfo.lpDirectSoundBuffer[slot3] == NULL)
	{
		PlayListAutoSounds(slot3, distance3_X, distance3_Y, 0);
		return;
	}
	if(rand()%1000<per4 && g_SoundInfo.lpDirectSoundBuffer[slot4] == NULL)
	{
		PlayListAutoSounds(slot4, distance4_X, distance4_Y, 0);
		return;
	}
	if(rand()%1000<per5 && g_SoundInfo.lpDirectSoundBuffer[slot5] == NULL)
	{
		PlayListAutoSounds(slot5, distance5_X, distance5_Y, 0);
		return;
	}
	if(rand()%1000<per6 && g_SoundInfo.lpDirectSoundBuffer[slot6] == NULL)
	{
		PlayListAutoSounds(slot6, distance6_X, distance6_Y, 0);
		return;
	}
	return;
}
//////////////////////////////////PING///////////////////////////////////
// Original code by Eleval
#include <numeric>
#include <Array>

static std::array<int, 20> pingList;
static std::vector<int> sortedPingList;
static int pingListIdx = 0;


void InitPingList()
{
	sortedPingList.reserve(pingList.size());
	std::fill(pingList.begin(), pingList.end(), -1);
}

void PutPing(int ping)
{
	pingList[pingListIdx++] = ping;

	if (pingListIdx >= static_cast<int>(pingList.size()))
		pingListIdx = 0;

	sortedPingList.clear();

	for (int i = 0; i < static_cast<int>(pingList.size()); ++i)
		if (pingList[i] != -1)
			sortedPingList.push_back(pingList[i]);

	std::sort(sortedPingList.begin(), sortedPingList.end());
}

int GetAveragePing()
{
	int avg = 0;
	int size = sortedPingList.size();

	if (size)
		avg = std::accumulate(sortedPingList.begin(), sortedPingList.end(), 0) / size;

	return avg;
}

void NewBuffTime()

{
	int avgPing = 200;
	

	for (int i = 0; i < avgPing; i++)
	{
		HprintBold((SCREEN_WIDTH / 2) - 80, 40, RGB(20, 255, 20), 0, "Buff : %d", avgPing);
		avgPing--;
	}

		
	
	
	
}


bool ReadMagicIconImage2(int type, int num)
{
	char filename[MAX_FILENAME];
	FILE* fp;

	Spr* s;

	switch (type)
	{
	case WZD:		sprintf(filename, "./Magic/w%03d.csp", num); break;
	case PRI:		sprintf(filename, "./Magic/p%03d.csp", num); break;
	}
	s = &magic_spr[num];
	if (s->img) return 1;

	if (fp = Fopen(filename, "rb"))
	{
		fread(&s->xl, sizeof(short), 1, fp);
		fread(&s->yl, sizeof(short), 1, fp);
		fread(&s->ox, sizeof(short), 1, fp);
		fread(&s->oy, sizeof(short), 1, fp);
		fread(&s->size, sizeof(unsigned int), 1, fp);

		MemFree(s->img);
		MemAlloc(s->img, s->size);
		fread(s->img, s->size, 1, fp);
		convert565to555(s);
		fclose(fp);
	}
	else return 0;
	return 1;
}
extern void PutMagicIcon(int x, int y, int num, int flag=0, int type=2);




void DrawPing()
{
	/*if (!g_pHero->showPing)
		return;*/

	int avgPing = GetAveragePing();

	if (avgPing < 300)
	{
		HprintBold((SCREEN_WIDTH / 2) - 40, 40, RGB(20, 255, 20), 0, "Ping : %d ms", avgPing);
	}
	else
	{
		if (avgPing < 1000)
			HprintBold((SCREEN_WIDTH / 2) - 40, 40, RGB(255, 0, 20), 0, "Ping : %d ms", avgPing);
		else
			HprintBold((SCREEN_WIDTH / 2) - 40, 40, RGB(20, 255, 20), 0, "Ping : %d ms", avgPing);
	}
}


void DrawDaysofWeek()

{ 

	
	time_t rawtime;
	time(&rawtime);

	struct tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
//	int day = timeinfo.tm_wday;
	//int tttx = spr[71].xl / 2;
	int day = g_Current_Server_Day;
	day % 7;
	int x = 460;
	int y = 22;

		switch (day)
		{
		case 0:
			HprintBold(x, y, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(0, 652));

			break;

		case 1:
			HprintBold(x, y, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(0, 653));

			break;
		case 2:
			HprintBold(x, y, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(0, 654));
			break;
		case 3:
			HprintBold(x, y, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(0, 655));
			break;
		case 4:
			HprintBold(x, y, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(0, 656));
			break;
		case 5:
			HprintBold(x, y, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(0, 657));
			break;
		case 6:
			HprintBold(x, y, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(0, 658));
			break;
		default:
			break;
		
	}

	
}
///////////////////////////////////////////////////////////////////////
void	DrawClock()		//좌표는 절대 좌표임
{
	static int		bell_Count=0;
	static bool		flag=0;
	static DWORD	endTime;
	
//	if( MapNumber != 0 )	return;		//마을 맵에서만 출력
	
//	Spr*	spr_Hour;
//	Spr*	spr_Minute;			//right시침,분침 스프라이트
	
	int		y, mo,d, h, mi, sec;					//시간:0~23, 분:0~59
//	int		hour_Spr=0, minute_Spr=0;
	
//	CurrentGameDate( g_Current_Server_Time, &y, &mo, &d, &h, &mi, &sec );		//현실 시간보다 6배 빠르지!
	
	DWORD rest = g_Current_Server_Time;

	d		= rest / 86400;		// +1
	rest	= rest % 86400;

	h		= rest / 3600;		//  0 
	rest	= rest % 3600;

	mi		= rest / 60;		//  0
	sec		= rest % 60;

	Hcolor( 255,255,255 );	
	HprintBold(300, 22, RGB(255,255,255), 0, "Current Server Time : %02d:%02d:%02d", h, mi, sec);

	
	
	//시 : 12단위계(시침프레임->60F) -> 1시간 당 5 Frame(12분 마다 시침 전환)
	//분 : 60단위계(붙침프레임->12F) -> 5분 당 1 Frame

	
/*	hour_Spr	= (int)((h%12) * 5 + (int)(mi/12.));		// 현 시침의 프레임을 계산
	if(hour_Spr > 59)	hour_Spr-=60;
	minute_Spr	= (int)(mi/5.);								// 현 분침의 프레임을 계산
	if(minute_Spr > 11)	minute_Spr-=12;
	
	//////////////////////// 시계 종소리 루틴 /////////////////////////
	if( mi==0 && flag==0)							//한시간마다 종소리
	{
		bell_Count = h%12;			//(0~11)
		if(bell_Count==0)	bell_Count=12;
		flag=1;
	}
	
	if( bell_Count > 0 )		//울려야하는 종소리 카운트가 0보다 크고 이전 종소리가 끝나지 않은 경우
	{
		DWORD	currentTime=timeGetTime();	
		if ( endTime <= currentTime || endTime==0 ) 
		{
			endTime = currentTime+2300;
			int distance = (int)(sqrt(pow((float)Hero->x-CLOCK_X,(float)2)+pow((float)Hero->y-CLOCK_Y,(float)2))/800);
			
			PlayListAutoSounds(SN_CLOCKBELL, 0,  distance, 0 );
			
			bell_Count--;
			if(bell_Count<=0)	
			{
				bell_Count=0;
				endTime=0;
			}
		}
	}
	
	if( minute_Spr==11 && flag==1 )		flag=0;
	
	if( ( abs(Hero->x - CLOCK_X) > SCREEN_WIDTH && abs(Hero->y - CLOCK_Y) > SCREEN_HEIGHT ) && ( abs(Hero->x - CLOCK_X ) > SCREEN_WIDTH && abs(Hero->y - CLOCK_Y) > SCREEN_HEIGHT ) )
		return;
	
	//절대 좌표로 출력(단 시계탑의 높이 고려해서 찍을 것)
    spr_Hour	=&Effspr[HOUR_SPRITE].EffSpr[hour_Spr];
	spr_Minute	=&Effspr[MINUTE_SPRITE].EffSpr[minute_Spr];
	
	ClockOutPut(CLOCK_X, CLOCK_Y, spr_Minute);
	ClockOutPut(CLOCK_X, CLOCK_Y, spr_Hour);
	*/
}


BOOL	CheckCrackRange(LPCHARACTER lpChar)				//일정 범위 내에 적이 있는 경우 발사 안됨->1리턴
{
	LPCHARACTER		target = NULL;
	//BOOL			result;
	int				tile=2;
	
	while( target!=NULL )
	{
		if( BoxAndDotCrash(lpChar->x - TILE_SIZE*tile,lpChar->y - TILE_SIZE*tile, TILE_SIZE*tile*2, TILE_SIZE*tile*2, target->x, target->y) )
			return 1;
		else	target = target->lpNext;
	}
	return 0;	
}

LPITEMGROUND ReturnMagicCheckedItem( bool range_Check, MAGICLIST* lpML, int x, int y, int fire_Height, int xl, int yl )
{
	LPITEMGROUND	item = (LPITEMGROUND)g_Item;
	BOOL			crash_Result=0;
	
	while( item != NULL )
	{
		if(fire_Height > 32)		//사람앉은키 높이보다 높은 경우
			return 0;
		if( (item->attr[ IATTR_ATTR] & IA2_OPENED) || (item->attr[ IATTR_ATTR] & IA2_BROKEN) )	//이미 열리거나 박살나있는 경우 
		{
			item = item->next;
			continue;
		}
		
		
		if( item->spr[0] == NULL ) return 0;
		
		/*
		g_Item->server_id = i->server_id;
		g_Item->no	  = i->no;
		g_Item->sx    = i->sx;
		g_Item->sy	  = i->sy;
		g_Item->offx  = i->offx;
		g_Item->offy  = i->offy;
		g_Item->x     = i->x; 
		g_Item->y     = i->y; 
		g_Item->mx    = i->x / TILE_SIZE;
		g_Item->my    = i->y / TILE_SIZE;
		
		  g_Item->attr[0]	= i->attr[0];
		  g_Item->attr[1]	= i->attr[1];
		  g_Item->attr[2]	= i->attr[2];
		  g_Item->attr[3]	= i->attr[3];
		  g_Item->attr[4]	= i->attr[4];
		  g_Item->attr[5]	= i->attr[5];
		*/
		
		///////////////////////// Item의 종/횡축 길이 //////////////////////////////
		int item_Spr_X = item->spr[0]->xl;
		int item_Spr_Y = item->spr[0]->yl;
		
		crash_Result=BoxAndBoxCrash( x, y, xl, yl, item->x-(item_Spr_X/2), item->y-(item_Spr_Y/2), item_Spr_X, item_Spr_Y);
		
		if( crash_Result )		//충돌이 일어난 경우
		{
			//Item box을 체크하여 에니메이션까지 뿌려줘야하는 경우(한번 타격당한 Item은 또다시 타격 당하지 않게)
			if( /*range_Check==1 &&*/ !((item->attr[ IATTR_ATTR] & IA2_OPENED) || (item->attr[ IATTR_ATTR] & IA2_BROKEN)) )
			{
				if(lpML->lpChar_Own == Hero)
					Send_CMD_ITEM_BOX_MAGIC_BREAK( (int)item->server_id );
				return item;
			}
			//else	return item;
		}
		item = item->next;
	}
	return 0;
}

void RecvCMD_SERVER_MAGICTRAP_EXPOSE( t_server_magictrap_expose *p)
{
	LPCHARACTER	target_Char = FindCharacter( &g_CharacterList, p->target_id );
	if(target_Char == NULL )	return;
	
	if(target_Char->type==SPRITETYPE_CHARACTER)
	{
		CharDoAni( target_Char, target_Char->direction, ACTION_ATTACKED);
	}
	else
	{
		CharDoAni( target_Char, target_Char->direction, MON1_ATTACKED);
	}
	
	switch( p->trap_Type )
	{
	case 0:
		InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE1, 0, p->x*TILE_SIZE, p->y*TILE_SIZE, p->x*TILE_SIZE, p->y*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE1, 0, p->x*TILE_SIZE, p->y*TILE_SIZE, p->x*TILE_SIZE, p->y*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE1, 0, p->x+1*TILE_SIZE, p->y+1*TILE_SIZE, p->x+1*TILE_SIZE, p->y+1*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE1, 0, p->x+1*TILE_SIZE, p->y-1*TILE_SIZE, p->x+1*TILE_SIZE, p->y-1*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE1, 0, p->x-1*TILE_SIZE, p->y+1*TILE_SIZE, p->x-1*TILE_SIZE, p->y+1*TILE_SIZE);
		break;
	case 1:
		InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE2, 0, p->x*TILE_SIZE, p->y*TILE_SIZE, p->x*TILE_SIZE, p->y*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE2, 0, p->x*TILE_SIZE, p->y*TILE_SIZE, p->x*TILE_SIZE, p->y*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE2, 0, p->x*TILE_SIZE, p->y+1*TILE_SIZE, p->x*TILE_SIZE, p->y+1*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE2, 0, p->x-1*TILE_SIZE, p->y*TILE_SIZE, p->x-1*TILE_SIZE, p->y*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE2, 0, p->x+1*TILE_SIZE, p->y*TILE_SIZE, p->x+1*TILE_SIZE, p->y*TILE_SIZE);
		break;
	case 2:
		InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE3, 0, p->x*TILE_SIZE, p->y*TILE_SIZE, p->x*TILE_SIZE, p->y*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE3, 0, p->x*TILE_SIZE, p->y-1*TILE_SIZE, p->x*TILE_SIZE, p->y-1*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE3, 0, p->x+1*TILE_SIZE, p->y-1*TILE_SIZE, p->x+1*TILE_SIZE, p->y-1*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE3, 0, p->x-1*TILE_SIZE, p->y*TILE_SIZE, p->x-1*TILE_SIZE, p->y*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE3, 0, p->x+1*TILE_SIZE, p->y*TILE_SIZE, p->x+1*TILE_SIZE, p->y*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE3, 0, p->x-1*TILE_SIZE, p->y+1*TILE_SIZE, p->x-1*TILE_SIZE, p->y+1*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE3, 0, p->x*TILE_SIZE, p->y+1*TILE_SIZE, p->x*TILE_SIZE, p->y+1*TILE_SIZE);
		//InsertMagic(target_Char, target_Char, EFFECT_TRAP_EXPOSE3, 0, p->x+1*TILE_SIZE, p->y+1*TILE_SIZE, p->x+1*TILE_SIZE, p->y+1*TILE_SIZE);
		break;
	}
}

void RecvCMD_SERVER_MAGICAL_DETECT( t_server_magical_detect *p)
{
	//LPCHARACTER	target_Char = FindCharacter( &g_CharacterList, p->target_id );
	//if(target_Char == NULL )	return;
	
	switch( p->type )
	{
	case 1:			//마법 트랩인 경우
		InsertMagic(Hero, Hero, 121, 0, p->tile_X*32, p->tile_Y*32, 0, 0);
		break;
	case 2:			//마법 물체 및 기타 마법 효력체
		InsertMagic(Hero, Hero, 128, 0, p->tile_X*32, p->tile_Y*32, 0, 0);
		break;
	}
}

///////////////// 0201  이 규훈 ////////////////////////
// 클라이언트의 마법에 의한 특정 효과 명령을 받는 함수
void RecvCMD_SERVER_MAGICCOMMAND( t_server_magic_command *p)
{
	switch(p->command)
	{
	case SET_LOSTSIGHT:		//상태종료는 클라이언트에서 판단하여 종료
		{
			// 단지 플레그 및 시간만 셋팅하고 실제 연출은 map.cpp의 MapBright함수에서 처리
			Hero->sight			= 3;
			Hero->lostSight		= 31;//시력상실 상태를 셋팅함
			Hero->lostSightTime	= p->sub_term+g_curr_time;	//시력상실 유지 시간(서버에서 알려온 정수형 시간-초)
			break;
		}
		//////////////// 0809 lhs 추가 (퀘스트용 사운드) ////////////////
	case CCD_START_QUEST:	//퀘스트 시작용 소리	15000
		{
			PlayListAutoSounds(50,0,0,0);
			break;
		}
	case CCD_END_QUEST:		//퀘스트 종료 소리			15001	
		{
			PlayListAutoSounds(51,0,0,0);
			break;
		}
	case CCD_QUEST_SALUTE:	//퀘스트용 축포				15002
		{
			LPCHARACTER		lpChar = ReturnCharacterPoint( p->sub_command );
			if (lpChar) // Finito null check
			{
				InsertMagic( lpChar, lpChar, 374, 0, 0,0,0,0 );
			}
			break;
		}
	case CCD_QUEST_GETSKILL://기술획득 소리				15003
		{
			//PlayListAutoSounds(52,0,0,0);				// 0910 YGI
			break;
		}
	case CCD_QUEST_GETITEM:	//아템획득 소리				15004
		{
			//PlayListAutoSounds(53,0,0,0);				// 0910 YGI
			break;
		}
	case CCD_QUEST_MONSTER:	//몬스터 침입 소리			15005
		{
			PlayListAutoSounds(54,0,0,0);
			break;
		}
	case CCD_START_QUEST_BBS://공지사항(선포) 소리		15006
		{
			PlayListAutoSounds(55,0,0,0);
			break;
		}
	case CCD_END_QUEST_BBS:	//공지사항(종료) 소리		15007
		{
			PlayListAutoSounds(56,0,0,0);
			break;
		}
	case CCD_COMPLETE_QUEST://퀘스트의 종료를 알리는 효과	15008
		{
			LPCHARACTER		lpChar = ReturnCharacterPoint( p->sub_command );
			if (lpChar) // Finito null check
			{
				InsertMagic( lpChar, lpChar, 441, 0, 0,0,0,0 );
			}
			break;
		}
	case OPERATOR_PLAYSOUND_EVENT1:		//행사 시작을 알리는 축가
		{
			PlayListAutoSounds(62,0,0,0);
			break;
		}
	case OPERATOR_PLAYSOUND_EVENT2:		//박수소리
		{
			PlayListAutoSounds(65,0,0,0);
			break;
		}
	case SET_CUREBLIEND:	//시야상실 회복 처리
		{
			Hero->sight			= 10;
			Hero->lostSight		= 0;
			Hero->lostSightTime	= 0;
			break;
		}
		
	case SET_INFORMATION:
		{
			LPCHARACTER dch = FindCharacter( &g_CharacterList, p->sub_command );
			// 대상자의 정보를 나만이 볼수 있게 처리
			AddCurrentStatusMessage(90, 70, 255, "Level:%d / HP:%d / MP:%d / STR:%d / WIS:%d / INT_:%d", dch->_lev, dch->_Hp, dch->_Mp, dch->_str, dch->_Wis, dch->_Int);
			break;
		}
		
		///////////////////////////////////////////////////////////////////////1010 KKH 바뀐 부분..////////////////////////////////////////////////////////
		//////////////////// 0811 yjs 추가 ////////////////////////
	case FIGHT_RESULT:		//자신이 패한 경우
		{
			LPCHARACTER dch = FindCharacter( &g_CharacterList, p->sub_command );
			switch(p->sub_term)
			{
			case FIGHT_WIN:
				AddCurrentStatusMessage(50, 255, 120, lan->OutputMessage(3,1), Hero->name, dch->name);//010215 lsw
				// 0926 승리 이펙트 
				InsertMagic(Hero, Hero, 495, 0, 0,0,0,0);
				InsertMagic(Hero, Hero, 494, 0, 0,0,0,0);
				PlayListAutoSounds(51,0,0,0);
				InsertMagic(dch, dch, 496, 0, 0,0,0,0);
				Fight_Stone_End();
				break;	
			case FIGHT_LOSE:
				AddCurrentStatusMessage(255, 60, 80, lan->OutputMessage(3,2), Hero->name, dch->name);//010215 lsw
				// 0926 패배 이펙트 
				//InsertMagic(Hero, Hero, 496, 0, 0,0,0,0);
				InsertMagic(dch, dch, 495, 0, 0,0,0,0);
				InsertMagic(dch, dch, 494, 0, 0,0,0,0);
				PlayListAutoSounds(53,0,0,0);
				InsertMagic(Hero, Hero, 496, 0, 0,0,0,0);
				Fight_Stone_End();
				break;
			case FIGHT_DRAW:
				AddCurrentStatusMessage(80, 50, 255, lan->OutputMessage(3,3), dch->name);//010215 lsw
				Fight_Stone_End();
				break;
			case FIGHT_LOGOUT:		// 010604 YGI
				AddCurrentStatusMessage(50, 255, 120, lan->OutputMessage(3,4), Hero->name);//010215 lsw
				// 0926 패배 이펙트 
				InsertMagic(Hero, Hero, 495, 0, 0,0,0,0);
				InsertMagic(Hero, Hero, 494, 0, 0,0,0,0);
				PlayListAutoSounds(51,0,0,0);
				//InsertMagic(Hero, Hero, 496, 0, 0,0,0,0);		// 상대방이 나갔을 경우 
				Fight_Stone_End();
				break;
			}
			break;
		}
		///////////////////////////////////////////////////////////////////////1010 KKH 바뀐 부분.. 끝////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////
	}
}

// 순간 이동 마법을 사용했을 때 결과 처리
void RecvCMD_MAGIC_POSITION( t_server_magic_position *p)
{
	LPCHARACTER	ch = FindCharacter(&g_CharacterList, p->idTarget);
	if( ch == NULL ) return;
	
	ch->moveFlag=0;
	ch->pathcount=0;
	CharDoAni(ch, ch->direction, ch->basicAction);
	
	TileMap[ ch->oldposition.x][ch->oldposition.y].occupied = 0;
	TileMap[ch->position.x][ch->position.y].occupied = 0;

	ch->gox = ch->x = p->nX*32+16;
	ch->position.x	= p->nX;
	ch->goy = ch->y = p->nY*32+16;
	ch->position.y	= p->nY;

	if (ch->hp>0) TileMap[ ch->position.x][ch->position.y].occupied = 1;	// 010915 LTS
	
	switch(p->nType)
	{
	case 1:		//phasing
		{
			InsertMagic(ch,ch, 255,	9, 0, 0, ch->x, ch->y);
			break;
		}
	case 2:		//townportal
		{
			InsertMagic(ch,ch, 299,	9, 0, 0, ch->x, ch->y);
			break;
		}
	case 3:		//teleport - 이동한 후에 다른 마법을 이어 찍어주먼 렉이 걸렸을 경우 너무 늦음
		{
			InsertMagic(ch,ch, 409,	9, ch->x, ch->y, ch->x, ch->y);
			break;
		}
	case 4:		//Multiport
		{
			//InsertMagic(ch,ch, 255,			9, 0, 0, ch->x, ch->y);
			break;
		}
	case 5:		//Memorizingportal
		{
			//InsertMagic(ch,ch, 255,			9, 0, 0, ch->x, ch->y);
			break;
		}
	case 6:		// 운영자에 의한 강제이동.
		{
			if( ch == Hero )	AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(3,5) );//010215 lsw
			break;
		}
    case 7:
		{
			InsertMagic(ch,ch, 470,	0, 0, 0, ch->x, ch->y);
			break;
		}
	}
	
	if( ch == Hero )		// 010425 YGI
	{
		SendModifyPosition( ch );
		g_Map.tox = ch->position.x - (GAME_SCREEN_XSIZE/32/2);
		g_Map.toy = ch->position.y - (GAME_SCREEN_YSIZE/32/2);
		
		if (p->nType == 3)
		{
			StartSetView(ch);
			SetView(ch, ch->sight);		//순간 이동한 그 좌표를 화면의 중앙으로 이동
			SetHeroPosition( 0 );
		}
		else
			SetHeroPosition( 1 );
	}
}	

void RecvCMD_CHARACTER_CONDITION( t_server_character_condition *p, int type )
{	
	//CON_NORMAL정상0/CON_ACTIVE활동1/CON_SLOW느려짐2/CON_SLUGGISH무기력3/CON_DAZE혼란4/CON_POISON중독5/CON_STONE석화6/CON_DEATH죽음7
	bool		action_On=0;
	LPCHARACTER ch=FindCharacter( &g_CharacterList, p->idTarget);
	if(ch==NULL)  return;
	// 해당 캐릭터의 상태를 변화시키는 함수
	ConditionSet(ch, p->nState);
	
	if( type == CMD_SERVER_CHARACTER_CONDITION2 )		// 차이값을 보내주는게 아니라 그 전체를 보내준다.
	{
		p->nLife -= ch->hp;
	}
	// 0106KHS
	SendReqHpOfParty(ch->name);
	if( p->nLife < 0 )
	{
		AddNumberAni( NUMBER_ANIMATION_TYPE_DOWN_, ch->y+1, ch->x, ch->y-40, abs( p->nLife) );
		
		if( p->idTarget<10000 )
		{
			CharDoAni(ch, ch->direction, ACTION_ATTACKED );
			action_On = 1;
			ch->changeActionFlag = 1;
		}
		else
		{	
			CharDoAni(ch, ch->direction, MON1_ATTACKED );
			action_On = 1;
			ch->changeActionFlag = 1;
		}
	}
	else if( p->nLife > 0 )//020530 lsw
	{
		AddNumberAni( NUMBER_ANIMATION_TYPE_UP_, ch->y+1, ch->x, ch->y-40, abs( p->nLife) );
		ch->changeActionFlag = 0;
		CharDoAni(ch, ch->direction, ch->basicAction );
	}
	
	///////////////////////////////////// 지금부터는 캐릭터 상태에 따른 다른 처리 //////////////////////////////////////
	if( ch->condition == CON_DEATH )//p->condition == CON_DEATH )		//죽음상태로 전환시 처리
	{
		ch->changeActionFlag = 0;
		if( p->idTarget<10000 )
		{
			ch->changeActionFlag = 0;
			CharDoAni(ch, ch->direction, ACTION_DIE);
		}
		else
		{
			ch->changeActionFlag = 0;
			CharDoAni(ch, ch->direction, MON1_DIE);
		}
		
		//ch->condition=CON_DEATH;
		ch->hp			= 0;
		ch->mp			= 0;
		ch->viewtype	= VIEWTYPE_NORMAL_;		//다른 상태진행 상태에서 죽었을때(기존상태에 의한 효과 없애기 위해)
		
		if( ch==Hero )
		{
			SCharacterData.condition	 = CON_DEATH;
			SCharacterData.nCharacterHP  = 0;
			SCharacterData.nCharacterMP  = 0;

			Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
		}
		return;
	}
	
	//////////////////////////////////////////////////
	// 죽는 상태외의 상태 변화시만 처리
	// 대상체가 바로 나인 경우만 처리(메뉴부분 디스플레이)
	
	if( ch == Hero )
	{
		SCharacterData.nCharacterHP += p->nLife;
		if( SCharacterData.nCharacterHP > SCharacterData.nCharacterMAXHP ) 
			SCharacterData.nCharacterHP = SCharacterData.nCharacterMAXHP ;
		
		SCharacterData.condition = ch->condition;
		if(SCharacterData.nCharacterHP<=0)	SCharacterData.nCharacterHP	 = 1;

		Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
	}
	
	if( ch->condition == CON_NORMAL )			//상태가 살아있는 경우
	{
		if( ch->viewtype < VIEWTYPE_TRANSPARENCY_ )		//투명인간 상태 이상의 다른 상태인 경우는 그냥 현 뷰타입 지속유지
			ch->viewtype			= VIEWTYPE_NORMAL_;
		ch->hp				   += p->nLife;
		if(ch->hp<= 0) ch->hp   = 1;
		ch->dieani				= 0;
		ch->blood_Set			= false;
		if(!action_On)
		{
			ch->changeActionFlag	= 0;
			CharDoAni(ch, ch->direction, ch->basicAction);
		}
	}
	else
	{
		if (ch->viewtype < VIEWTYPE_TRANSPARENCY_)
		{ // 투명인간 상태 이상의 다른 상태인 경우는 그냥 현 뷰타입 지속유지
			if (ch->condition&CON_SLOW)    ch->viewtype = VIEWTYPE_SLOW_;
			if (ch->condition&CON_POISON)  ch->viewtype = VIEWTYPE_POISON_;
			if (ch->condition&CON_CURSE)   ch->viewtype = VIEWTYPE_CURSE_;
			if (ch->condition&CON_DAZE)    ch->viewtype = VIEWTYPE_CONFUSION_;
			if (ch->condition&CON_FROZEN)  ch->viewtype = VIEWTYPE_FROZEN_;
			if (ch->condition&CON_STONE)
			{
				ch->viewtype = VIEWTYPE_STONE_;
				ch->pathcount = 0;
				ch->moveFlag  = 0;
				ch->changeActionFlag	= 1;
			}
		}
	}
}

void RecvCMD_SERVER_MAGICVIEWTYPE( t_server_magicviewtype *p)
{	//< CSD-030516
	LPCHARACTER ch = FindCharacter(&g_CharacterList, p->target_id);
	
	if (ch == NULL)
	{
		return;
	}

	switch (p->view_Type)
	{
	case VIEWTYPE_OBSERVE_:
		{
			SetBattlePeace(PEACE_MODE);
			break;
		}
	}

	ch->viewtype = (char)p->view_Type;
}	//> CSD-030516

void SendCMD_CHARACTER_DEMAND( int command_type, int target_id)		//인자:커멘드 메시지 #define명칭,목표id
{
	t_packet p;
	
	p.h.header.type = CMD_CLIENT_CHARACTER_DEMAND;
	p.h.header.size	= sizeof( t_client_character_demand );
	{
		p.u.angra.client_character_demand.target_id		= target_id;
		p.u.angra.client_character_demand.command_type	= command_type;
		//p.u.client_arigement_demand
	}
	QueuePacket( &p, 1 );	
}

void RecvCMD_ARIGEMENT_RESPONSE( t_server_arigement_response *p )
{	//< CSD-030324
	return;	// thai2 YGI
	//> CSD-030324
	LPCHARACTER ch	= FindCharacter( &g_CharacterList, p->target_id );
	if(ch == NULL ) return;
	
	ch->arigement	= p->arigement;
	switch(p->arigement)
	{
		//case 0:
	case 1:
	case 2:
	case 3:
		InsertMagic(ch, ch, 344, 0, 0, 0, 0, 0);		//blue
		break;
	case 4:
	case 5:
	case 6:
		InsertMagic(ch, ch, 345, 0, 0, 0, 0, 0);		//Green
		break;
	case 7:
	case 8:
	case 9:
		InsertMagic(ch, ch, 346, 0, 0, 0, 0, 0);		//red
		break;
	}
}

void EndCastMagic(LPCHARACTER scr/*MAGICLIST* lpML*/)
{
	const int nSelect = scr->excute_MagicNum;
	scr->ready_Magic = (g_mgrBattle.IsCombat(nSelect)) ? 2:0;
	scr->end_Casting =0;
	scr->end_ReadyMagic =0;
	scr->end_RunMagic =0;
	scr->bow_Type =0;				//활 타입을 초기화
				
	RectImageOn=0;
}

void CheckDoingAppointMagic(void)
{			
	int		x,y;
	int		dir     = Hero->direction;	
	int		sprc    = Hero->AnimationTable[ Hero->nCurrentAction].cFrame[  Hero->nCurrentFrame.GetDecrypted() ];
	Spr		*s		= &(CharSpr[Hero->sprno].sp[ dir ][ sprc]);
	bool	firePattern = 0;	//0->직진패턴/1->유도패턴	
	int		magic_no = 0;
	int		distance = 0;
	// 마법을 발사한지 0.8초 이상 경과해야만 또다시 발사 가능
	if ( timeGetTime()-g_FireTime < FIRE_DELAY && g_FireTime !=0 )
		return;
	// 화살류 shoot_Time
	if (g_mgrBattle.IsThrow(Hero->excute_MagicNum, true))  return; // CSD-021021
	if (SCharacterData.nCharacterHP <= 0)                  return;
	///////////////////////////////////////////////////////////////////////////////////
	if(!(Hero->ready_Magic==2 || Hero->ready_Magic==3) && !tool_ID_EFFECT_TEST )
		return;
	
	if(tool_ID_EFFECT_TEST)	Hero->excute_MagicNum=30;
	
	if (!g_mgrBattle.IsPickUp(Hero, g_GameInfo.lpcharacter))
	{ //< CSD-021128 
		g_GameInfo.lpcharacter = NULL;
	} //> CSD-021128 
	
	switch (g_lpMT[Hero->excute_MagicNum].appoint_Type)
	{	//< CSD-021128 
	case 0:		//자신만 클릭 가능 마법 & 특정 목표없이 내가주체가 되는 비공격 마법
		{
			if (g_GameInfo.lpcharacter == NULL || g_GameInfo.lpcharacter == Hero)
			{
				x = Hero->x;
				y = Hero->y-Hero->height;
				firePattern = 0;
				break;
			}
			
			return;
		}
	case 1:	// 자신과 대상(PC/NPC) 모두에게 가능
		{
			switch(g_GameInfo.nSelectedSpriteType)
			{
			case SPRITETYPE_ON_THE_CEILING_CHAR:
			case SPRITETYPE_MONSTER:
			case SPRITETYPE_NPC:
			case SPRITETYPE_CHARACTER:
				{
					if (g_GameInfo.lpcharacter == Hero)
					{
						x = Hero->x;
						y = Hero->y-Hero->height;
						firePattern=0;
						break;
					}
					// 다른 대상에게 마법 사용
					if (g_GameInfo.lpcharacter != NULL)
					{
						x = g_GameInfo.lpcharacter->x;
						y = g_GameInfo.lpcharacter->y - g_GameInfo.lpcharacter->height;
						firePattern = 1; // 유도
						break;
					}
					
					return;
				}
			default:	
				{
					return;
				}
			}
			
			break;
		}
	case 2:	// 무조건 적(PC/NPC)을 클릭해야 하는 마법(적과의 거리 체크)
		{
			if (g_GameInfo.lpcharacter == NULL || g_GameInfo.lpcharacter == Hero)
			{
				return;
			}
			else if	(g_GameInfo.nSelectedSpriteType == SPRITETYPE_ITEM && IsHarmfulMagic(Hero->excute_MagicNum))	
			{ //특정 스프라이트를 클릭하고 공격마법인 경우만
				x = Mox;
				y = Moy;
				g_GameInfo.lpcharacter = Hero;
				///////////////// 2타일 내의 거리에서 발사는 안됨 /////////////////
				if (IsPointAttackMagic(Hero->excute_MagicNum) == true) // 대상 지정 공격 마법이라면
				{
					distance=(int)(sqrt(pow((float)abs(Hero->x-x),(float)2)+pow((float)abs(Hero->y-y),(float)2)));
					if(distance<TILE_SIZE*2)	return;
				}
			}
			else if (g_GameInfo.nSelectedSpriteType != SPRITETYPE_ITEM)	//대상을 정확히 클릭했을 때
			{
				if (g_GameInfo.lpcharacter->hp > 0 && Hero->excute_MagicNum == RELIEF_AUTHORITY)
				{ // 살아있는 놈에게 소생마법 쓰려할때
					return;
				}
				
				x = g_GameInfo.lpcharacter->x+rand()%7 - 3;
				y = (g_GameInfo.lpcharacter->y - g_GameInfo.lpcharacter->height) + rand()%7 - 3;
				firePattern=1;
				
				if (IsPointAttackMagic(Hero->excute_MagicNum)) 
				{ // 대상 지정 공격 마법이라면 
					distance = (int)(sqrt(pow((float)abs(Hero->x - x), (float)2) + pow((float)abs(Hero->y - y), (float)2)));
					
					if (distance < TILE_SIZE*2)
					{ // 2타일 내의 거리에서 발사는 안됨
						return;
					}
				}
			}
			
			break;
		}
	case 3: // 특정위치 및 다른 대상 및 아이템을 클릭하여 발사되는 공격 마법의 형태 & 지역마법
	case 4:	
	case 5:
		{
			if (g_GameInfo.lpcharacter == Hero)
			{
				return;
			}
			else if (g_GameInfo.lpcharacter == NULL || g_GameInfo.nSelectedSpriteType == SPRITETYPE_ITEM)
			{ // 맨 땅을 클릭하거나 아이템(상자)를 클릭한 경우
				x = Mox+rand()%11 - 5;
				y = Moy+rand()%11 - 5;
				firePattern = 0;
				
				if (!g_mgrBattle.IsMapAttr(Hero, x/32, y/32))
				{
					return;
				}
				
				if(g_GameInfo.nSelectedSpriteType == SPRITETYPE_ITEM)	
				{
					g_GameInfo.lpcharacter = NULL;
				}
				
				if (IsPointAttackMagic(Hero->excute_MagicNum)) 
				{ // 대상 지정 공격 마법이라면
					distance = (int)(sqrt(pow((float)abs(Hero->x - x), (float)2) + pow((float)abs(Hero->y - y), (float)2)));
					
					if (distance < TILE_SIZE*2)	
					{ // 2타일 내의 거리에서 발사는 안됨
						return;
					}
				}
			}
			else // 내가 아닌 특정 캐릭터를 클릭한 경우 - 0328 lkh 수정
			{
				x = g_GameInfo.lpcharacter->x + rand()%11 - 5;
				y = (g_GameInfo.lpcharacter->y - g_GameInfo.lpcharacter->height) + rand()%11 - 5;
				firePattern = 0;
				
				if (!g_mgrBattle.IsMapAttr(Hero, x/32, y/32))
				{
					return;
				}
				
				if (IsPointAttackMagic(Hero->excute_MagicNum))
				{ // 대상 지정 공격 마법이라면
					distance = (int)(sqrt(pow((float)abs(Hero->x - x), (float)2) + pow((float)abs(Hero->y - y), (float)2)));
					
					if (distance < TILE_SIZE*2)
					{ // 2타일 내의 거리에서 발사는 안됨
						return;
					}
				}
			}
			
			break;
		}
	case 6:	// 반드시 특정 위치를 클릭해야하는 경우(비공격마법)
		{
			if (g_GameInfo.lpcharacter != NULL || g_GameInfo.nSelectedSpriteType == SPRITETYPE_ITEM)
			{
				return;
			}
			
			x = Mox + rand()%21-10;
			y = Moy + rand()%21-10;
			firePattern = 0;
			break;
		}
	case 7: // 반드시 특정 물건(ITEM) 클릭해야만 함
		{
			if( g_GameInfo.nSelectedSpriteType == SPRITETYPE_ITEM )
			{
				
			}
			
			break;
		}
	default: 
		{
			x = Mox; 
			y = Moy;
			break;
		}
	}	//> CSD-021128
	
	magic_no = Hero->excute_MagicNum;
	firePattern = 0; // 무조건 직진	
	// 마법의 고유사정거리 내에 있는지 체크
	if( g_lpMT[Hero->excute_MagicNum].avail_Range != 0 )
	{	
		distance=(int)(sqrt(pow((float)abs(Hero->x-x),(float)2)+pow((float)abs(Hero->y-y),(float)2)) / 32) ;
		if( g_lpMT[Hero->excute_MagicNum].avail_Range<distance )
		{
			AddCurrentStatusMessage(255, 30, 50, lan->OutputMessage(3,6));//010215 lsw
			return;
		}
	}	
	
	///////////////// 나를 제외한 특정 대상을 정확히 클릭했는데 너무 근접한 경우
	if ( g_GameInfo.nSelectedSpriteType != SPRITETYPE_ITEM && (g_GameInfo.lpcharacter != NULL && g_GameInfo.lpcharacter != Hero) )
	{	
		///////////////////// 0704 lkh 수정 ////////////////////////
		if ( sqrt(pow((float)Hero->x - x, (float)2) + pow((float)Hero->y - y, (float)2)) < TILE_SIZE*2 )	//적과의 이격 거리가 2타일 이내인 경우
		{
			return;
		}
	}	
#ifdef _DEBUG // finito 060507	
	if(SysInfo.notconectserver)
	{	
		//////////////////// SoundUp lkh 추가(시전어 출력) //////////////////////
		int	wave_Starting = ReturnStartingWord(Hero, magic_no);
		PlayListAutoSounds(wave_Starting, 0, 0, 0);
	}	
	else		//실제 마법을 구사함을 서버에 알림
#endif
	{	
		//////////////////// SoundUp lkh 추가(시전어 출력) //////////////////////
		int	wave_Starting = ReturnStartingWord(Hero, magic_no);
		PlayListAutoSounds(wave_Starting, 0, 0, 0);
		
		g_FireTime=timeGetTime();
		
		if (g_mgrBattle.IsMagic(magic_no))
		{	//< CSD-TW-030606
			g_mgrBattle.SendMagicExecute(magic_no, g_GameInfo.lpcharacter, x, y);
		}	//> CSD-TW-030606
		
		if (g_mgrBattle.IsCombat(magic_no))
		{
			g_mgrBattle.SendCombatAttack(magic_no, g_GameInfo.lpcharacter, x, y);
		}
	}	
	return;
}		

void CheckEffectSpriteFree( void )
{		
	int i;
	
	//일단 Free해야 하는 모든 스프라이트 데이터를 날린다.
	for( i = 0; i < FreeEffectSpriteFlagCount; i ++)
	{
		FreeEffectSprite( FreeEffectSpriteFlag[i] );
	}
	FreeEffectSpriteFlagCount = 0;			//전부 날렸다는 조건이 만족할때 만
}

BOOL EffectSoundPlay(MAGICLIST*	lpML, int magic_Num, int sound_Num, int sound_loop)
{
	switch(sound_loop)
	{
		//case 0:
		//	return FALSE;
	case 99:
		if(!IsPlaying(sound_Num))
			PlayListAutoSounds(sound_Num,0,0,0);	//무한 루핑(마법이 종료될 때 까지)
		break;
	default:
		if(!IsPlaying(sound_Num) && g_lpET[magic_Num].sound_loop>0 && lpML->sound_LoopCount>0)
		{
			PlayListAutoSounds(sound_Num,0,0,0);
			lpML->sound_LoopCount--;
		}
	}
	return TRUE;
}


/*
int	ReturnTransColor(MAGICLIST* lpML, EFFECTLIST* lpEL)
{
switch(color_Trans_Type)
{
case 0:	

  lpEL->trans_Level=
  
	
	  USEEFFECTLIST
	  
		
		  if(lpML->trans_Type%10==8)		//점점 밝게 생성 / 마지막 어둡게
		  {
		  if(lpEL->FrameCount<EffList[lpEL->sprite_Num].MaxFrame-4)
		  {
		  lpEL->trans_Level=(lpML->trans_Level/(float)EffList[lpEL->sprite_Num].MaxFrame)*lpEL->FrameCount;
		  if(lpEL->trans_Level > lpML->trans_Level)	lpEL->trans_Level=(float)lpML->trans_Level;
		  }
		  else
		  {
		  lpEL->trans_Level-=(float)(lpML->trans_Level/4.);
		  if(lpEL->trans_Level < 1) lpEL->trans_Level = 1;
		  }
		  }
		  
			// 마지막 4프레임은 alpha off
			else if(lpML->trans_Type%10==9 && lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame-4)	
			{
			lpEL->trans_Level-=(lpML->trans_Level/4);//-((lpML->trans_Level/EffList[lpEL->sprite_Num].MaxFrame)*(lpEL->FrameCount-1));
			if(lpEL->trans_Level<=1)	lpEL->trans_Level=1;
			}
			
			  else if(lpML->trans_Type%10==7)	//바닥이 얼어붙는 효과
			  {
			  if(lpEL->FrameCount<=EffList[lpML->sprite_Num].MaxFrame/2)
			  {
			  lpEL->trans_Level=(float)lpEL->FrameCount;
			  if(lpEL->trans_Level>=32)	lpEL->trans_Level=32;
			  }
			  else
			  {
			  lpEL->trans_Level=(float)(EffList[lpML->sprite_Num].MaxFrame-lpEL->FrameCount);
			  if(lpEL->trans_Level<0) lpEL->trans_Level=0;
			  }
			  }
			  
				else if(g_lpET[lpML->magicNum].end_Time!=0)
				{
				lpEL->trans_Level=lpML->trans_Level*(g_lpET[lpML->magicNum].end_Time-lpEL->total_Frame)/g_lpET[lpML->magicNum].end_Time;
				if(lpEL->trans_Level<0)	lpEL->trans_Level=0;
				}
				}
*/
int CallbackCharacterData(int char_ID, DWORD index/* #define되어 있는 서버쪽 캐릭터 정보 인덱스(정수) */)
{
	return 1;
}

///////////////////////// 0529 lkh 인자 추가 ////////////////////////////
// 리턴값 : flase->충돌장애물이 있는 경우 / true->장애물이 없는 경우
BOOL TileLineCheck(int sx, int sy, int ex, int ey, BOOL check_ShootItem)			//발사된 객체의 경로상 장애물이 있는 여부 체크(타일단위) : 인자->시작점 도트 x,y & 끝점 도트 x,y
{	
	int distance;
	float check_X = 0, check_Y = 0;
	float incx=0, incy=0;
	
	//Mapx = g_Map.x * TILE_SIZE;
	sx = (int)(sx/TILE_SIZE);	//타일 단위 좌표로 전환
	sy = (int)(sy/TILE_SIZE);
	ex = (int)(ex/TILE_SIZE);
	ey = (int)(ey/TILE_SIZE);
	
	if(abs(ex-sx)>=abs(ey-sy))
	{
		if(ex>sx)	incx=1.;
		else		incx=-1.;
		if(ey>sy)	incy=(float)(abs(ey-sy)/(float)(abs(ex-sx)));
		else        incy=(float)(abs(ey-sy)/(float)(abs(ex-sx)))*-1; 
		distance=abs(ex-sx);
	}
	else 
	{
		if(ex>sx)	incx=(float)(abs(ex-sx)/(float)(abs(ey-sy)));
		else		incx=(float)(abs(ex-sx)/(float)(abs(ey-sy)))*-1;
		if(ey>sy)	incy=1.;
		else		incy=-1.;
		distance=abs(ey-sy);
	}
	check_X=(float)sx;
	check_Y=(float)sy;
	
	for(int i=0; i< distance; i++)
	{
		check_X=check_X+incx;
		check_Y=check_Y+incy;
		
		//		if( TileMap[(int)check_X][(int)check_Y].attr_light ) return FALSE;
		
		//////////////////////////// 0607 lkh 수정(충돌하는 장애물이 있는 경우, /////////////////////////////
		//		if( check_ShootItem )
		//		if( !FreeTile( NULL, check_X, check_Y,  check_X, check_Y )) return FALSE;
		// 활과 비도의 경우는 높낮이에 관계없이 사용가능하기 위해 효과벽이지만 높이 검사를 하지 않음 		
		if(TileMap[(int)check_X][(int)check_Y].attr_light || (check_ShootItem && TileMap[(int)check_X][(int)check_Y].occupied) )
		{ 
			return FALSE;	//충동하는 타일이 있는 경우
		}
	}
	return TRUE;			//충돌하는 타일이 없으므로
}

//////////////////////////// 0411 lkh 추가 //////////////////////////////
void InsertLightOut(int x, int y, int light, int terminateFrame, int color)		//color->0:white/1->red/2->blue	//소멸하는 광원을 추가 시키는 함수
{
	int		defer_X=0, defer_Y=0;	//광원 중첩되면 흔들어 주기 위한 좌표
	
	if( DayLightControl >= 31 ) return;		//낮인경우 소멸광원을 버퍼에 기억시키지 않음
	
	light -=4;
	
	if (light <= 0)  return;
	if (light > 21)  return;
	
	for(int i=0;i<MAX_OUTLIGHT_;i++)
	{
		if( (g_OutLight[i].x/TILE_SIZE==x/TILE_SIZE) && (g_OutLight[i].y/TILE_SIZE==y/TILE_SIZE) )
		{
			return;
		}
		
		if(g_OutLight[i].light==0)
		{
			g_OutLight[i].x=(x+defer_X);
			g_OutLight[i].y=(y+defer_Y);
			g_OutLight[i].light=light;
			g_OutLight[i].frame=0;
			g_OutLight[i].terminateFrame=terminateFrame;
			g_OutLight[i].flag=color;
			return;
		}
	}
}

bool RandomAttackEffect(int Effect_Type, LPCHARACTER ch, LPCHARACTER tchar)
{	//< CSD-031014
	if (ch->idWeaponItem/1000 == 6)
	{	//< CSD-031101
		CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(ch->idWeaponItem));

		if (pWeapon != NULL)
		{
			const int nEffect = pWeapon->Need3_fame;

			if (nEffect > 0)
			{
				InsertMagic(ch, tchar, nEffect, 9, 0, 0, tchar->x, tchar->y - tchar->height);
				return true;
			}
		}
	}	//> CSD-031101
	
	int spHeight=tchar->sp->yl;
	BOOL bloodtype=rand()%2;
	BOOL metaltype=rand()%2;
	int weaponType=tchar->accessory[0];
	
	if(	Effect_Type==BLOOD_EFFECT )		//피 이펙트:0
	{
		switch(rand()%8)
		{
		case 0:							//피 안튐
			break;
		case 1:							//튀는 피 형태
			{
				if(spHeight<=40)		//소형
				{
					InsertMagic(ch,tchar,263,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else if(spHeight<=75)	//중형
				{	
					InsertMagic(ch,tchar,262,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else if(spHeight<=105)	//대형
				{
					InsertMagic(ch,tchar,261,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else					//초대형
				{	
					InsertMagic(ch,tchar,260,9,0,0,tchar->x,tchar->y-tchar->height);	
				}
			}
			break;
		case 2:							//양방향 흐르는 피 형태
			{
				if(spHeight<=40)		//소형
				{
					InsertMagic(ch,tchar,271,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else if(spHeight<=70)	//중형
				{	
					InsertMagic(ch,tchar,270,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else if(spHeight<=105)	//대형
				{
					InsertMagic(ch,tchar,269,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else					//초대형
				{	
					InsertMagic(ch,tchar,268,9,0,0,tchar->x,tchar->y-5-tchar->height);
				}
			}
			break;
		case 4:
		case 3:							//전반향 흐르는 피 형태
			{
				if(spHeight<=40)		//소형
				{
					InsertMagic(ch,tchar,267,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else if(spHeight<=70)	//중형
				{	
					InsertMagic(ch,tchar,266,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else if(spHeight<=105)	//대형
				{
					InsertMagic(ch,tchar,265,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else					//초대형
				{ 	
					InsertMagic(ch,tchar,264,9,0,0,tchar->x,tchar->y-5-tchar->height);
				}
			}
			break;
		case 5:
		case 6:
		case 7:							//추가 터지는 피 패턴
			{
				if(spHeight<=40)		//소형
				{
					InsertMagic(ch,tchar,272,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else if(spHeight<=70)	//중형
				{	
					InsertMagic(ch,tchar,273,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else if(spHeight<=105)	//대형
				{
					InsertMagic(ch,tchar,274,9,0,0,tchar->x,tchar->y-tchar->height);
				}
				else					//초대형
				{ 	
					InsertMagic(ch,tchar,275,9,0,0,tchar->x,tchar->y-5-tchar->height);
				}
			}
			break;
		}
	}
	else if( Effect_Type==METAL_EFFECT )		//금속 이펙트:1
	{
		switch(rand()%13)
		{
		case 0:							
		case 1:							//사선 아래로 흐르는 금속성 스파크(투박)
			InsertMagic(ch,tchar,201,9,0,0,tchar->x,tchar->y-tchar->height-spHeight/2);
			break;
		case 2:
		case 3:							//사선 아래로 흐르는 빛속성 스파크(소프트)	
			InsertMagic(ch,tchar,203,9,0,0,tchar->x,tchar->y-tchar->height-spHeight/2);
			break;
		case 4:
		case 5:							//추가된 금속성 스파크(잔상)
			InsertMagic(ch,tchar,334,9,0,0,tchar->x,tchar->y-tchar->height-spHeight/2);
			break;
		case 6:							//타격지점에 정적으로 밝게 빛나는 효과
			InsertMagic(ch,tchar,204,9,0,0,tchar->x,tchar->y-tchar->height-spHeight/2);
			break;
		case 7:
		case 8:							//타격지점에서 터져나오는 폭파효과
			InsertMagic(ch,tchar,369,9,0,0,tchar->x,tchar->y-tchar->height-spHeight/2);
			break;
		case 9:
		case 10:
		case 11:
		case 12:
			if( ch->type == SPRITETYPE_CHARACTER )
			{
				int ret = AttackRangeTable[ch->sprno][ch->accessory[2]][4];
				switch(ret)
				{
				case ACTION_ATTACK_ONESWORD1:
				case ACTION_ATTACK_ONESWORD2:
				case ACTION_ATTACK_PENSING1:
				case ACTION_ATTACK_PENSING2:
					{	
						// SoundUp LKH 제거
						//int weapon_Type=ReturnPCAutoWave(ch,0);
						//if(weapon_Type!=21)
						InsertMagic( ch, ch, 336, 4, 0, 0, 0, 0);
						break;
					}
				case ACTION_ATTACK_BONG_DOWN:
					InsertMagic( ch, ch, 338, 4, 0, 0, 0, 0);
					break;
				case ACTION_ATTACK_BONG_JJIRKI:
					InsertMagic( ch, ch, 339, 4, 0, 0, 0, 0);
					break;
				case ACTION_ATTACK_BONG_ROUND:
					{
						if(ch->sex)
							InsertMagic( ch, ch, 337, 4, 0, 0, 0, 0);
						else
							InsertMagic( ch, ch, 357, 4, 0, 0, 0, 0);
						break;
					}
				}
			}
		}
	}
	return FALSE;
}	//> CSD-031014

bool RandomThrowEffect(int nKind, LPCHARACTER pCaster, LPCHARACTER pTarget)
{	//< CSD-0301014
	if (pCaster->idWeaponItem/1000 == 6)
	{	//< CSD-031101
		CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(pCaster->idWeaponItem));

		if (pWeapon != NULL)
		{
			const int nEffect = pWeapon->Need3_fame;

			if (nEffect > 0)
			{
				InsertMagic(pCaster, pTarget, nEffect, 9, 0, 0, pTarget->x, pTarget->y - pTarget->height);
				return true;
			}
		}
	}	//> CSD-031101
	
	if (rand()%2 == 0)
	{  
		switch (pTarget->sprno)
		{
		case 94:
		case 95:
		case 96: return false;
		}
	}
		
	switch (nKind)
	{
	case THROW_ARROW:
		{
			InsertMagic(pCaster, pTarget, 276, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	case THROW_KNIFE:
		{
			InsertMagic(pCaster, pTarget, 277, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	case THROW_FIRE_ARROW: 
		{
			InsertMagic(pCaster, pTarget, 153, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	case THROW_ICE_KNIFE: 
		{
			InsertMagic(pCaster, pTarget, 170, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	case THROW_SNOW_BALL:
		{
			InsertMagic(pCaster, pTarget, 206, 9, 0, 0, pTarget->x, pTarget->y);
			break;    
		}
	case THROW_WATER_BALL:
		{
			InsertMagic(pCaster, pTarget, 662, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	case THROW_BIG_GULP: 
		{
			InsertMagic(pCaster, pTarget, 664, 9, 0, 0, pTarget->x, pTarget->y);
			break;
		}
	}
	
	return true;
}	//> CSD-031014

bool ReadyMagic(LPCHARACTER	lpRead_Char, int trans_Type, int trans_Level)	//캐스팅이 끝나고 마법을 사용할 준비가된 인물 처리
{
	int x = lpRead_Char->x - ( g_Map.x * TILE_SIZE);
	int y = lpRead_Char->y - ( g_Map.y * TILE_SIZE);
	int sprc=lpRead_Char->AnimationTable[ lpRead_Char->nCurrentAction].cFrame[  lpRead_Char->nCurrentFrame.GetDecrypted() ];
	
	out_Result=EffectOutput( x, y, &CharSpr[lpRead_Char->sprno].sp[ lpRead_Char->direction ][ sprc], trans_Type, trans_Level);
	return	1;
}

bool TimeCheck(DWORD end_Time)	//start_Time=>lpML->create_Time , end_Time=>lpML->dlpELete_Time or ->continue_Time
{
	return ((g_ClientTime>=end_Time)?true:false);
}

// 체크된 캐릭터가 없으면 NULL 을 리턴한다.
// 인자->range_Check:1개 효과소스에 복수타격체크(대폭발)하여 타격효과 출력시 1로 셋팅
//       lpML 포인터/좌상단 좌표/길이값/default->0:내 자신은 충돌체크하지 말까?
// air_Surface : 0->지상적만 체크/1->공중적만 체크/2->지상과 공중적 모두 체크
// 마지막인자: curve -> 0:직사포, 1->곡사포

///////////////////////// 0721 lkh 수정 /////////////////////////////
LPCHARACTER ReturnMagicCheckedCharacter(int range_Check, MAGICLIST* lpML, int x, int y, int fire_Height, int xl, int yl, int my_Check, int air_Surface, BOOL curve)
{	
	LPCHARACTER		ch = g_CharacterList.lpFirst;	//주인공의 포인터
	LPCHARACTER		target = lpML->lpChar_Target;	//대상의 포인터
	int		spr_Num=0;
	int		az=0,azl=TILE_SIZE;
	int		bz=0,bzl=TILE_SIZE;
	BOOL	crash_Result=0;
	
	spr_Num=lpML->next_Effect;
	
	if(fire_Height>0)	//발사 스프라이트가 높이값을 가지고 있을 경우 
	{		
		az=fire_Height-TILE_SIZE/2;
		azl=TILE_SIZE;
	}		
	
	while( ch != NULL )
	{		
		bz=ch->height;
		
		if( my_Check == 0 )
		{	
			if( ch == lpML->lpChar_Own )
			{
				ch= ch->lpNext;
				continue;
			}
		}		
		//죽은 대상체인 경우 스킵
		if (IsDead(ch) || ch->viewtype == VIEWTYPE_OBSERVE_)
		{	//< CSD-030510
			ch = ch->lpNext;
			continue;
		}	//> CSD-030510
		
		if( !curve || (curve && ch == target) )		//직사포인 경우와 곡사포이면서 클릭한 적인 경우
		{	
			int t_Spr_X;
			int t_Spr_Y;
			
			
			if( lpML->magicSerial == METEOR_STRIKE )  // 030509 kyo 
			//if( lpML->magicSerial == 110 )  
			{
				t_Spr_X = 96;
				t_Spr_Y = 96;
			}
			else
			{
				t_Spr_X = CharSpr[ch->sprno].sp[ch->direction][ch->nCurrentFrame.GetDecrypted()].xl;
				t_Spr_Y = CharSpr[ch->sprno].sp[ch->direction][ch->nCurrentFrame.GetDecrypted()].yl;
				
				if( t_Spr_X <= 28 ) t_Spr_X=16;						//너무 작은 캐릭터는 한 타일 이상 크기로 보정
				if(	t_Spr_Y <= 28 ) t_Spr_Y=16;						//						"
				if( t_Spr_X > 28 ) t_Spr_X=(int)(t_Spr_X*.6);		//너무 크기가 큰 캐릭터의 경우 75% 크기로 적용
				if( t_Spr_Y > 28 ) t_Spr_Y=(int)(t_Spr_Y*.6);		//						"
			}
			
			if ( air_Surface==0 && ch->height<=32)				//지상의 적만 체크
			{
				if(g_mgrBattle.IsAttack(lpML->lpChar_Own, ch, lpML->magicNum))	// 030106 kyo 중복되어 있을땐 공격되는 물체만 확인해본다.
				{
					crash_Result=BoxAndBoxCrash( x, y, xl, yl, ch->x-(t_Spr_X/2), ch->y-(t_Spr_Y*2/3), t_Spr_X, t_Spr_Y*2/3);
				}
			}
			else if	( air_Surface==1 && ch->height!=0)			//공중의 적만 체크
				crash_Result=BoxAndBoxCrash( x, y-fire_Height, xl, yl, ch->x-(t_Spr_X/2), ch->y-ch->height-(t_Spr_Y), t_Spr_X, t_Spr_Y);
			else
			{
				ch = ch->lpNext;
				continue;
			}
		}	
		
		if( crash_Result )		//충돌이 일어난 경우
		{	
			LPCHARACTER pCaster = lpML->lpChar_Own;	// 시전자의 포인터
			LPCHARACTER pTarget = ch;               // 대상자의 포인터
			
			switch (pCaster->sprno)
			{
			case 0:
			case 1:  
				{
					if (!g_mgrBattle.IsPickUp(pCaster, pTarget))
					{ //< CSD-021128
						return NULL;
					} //> CSD-021128
					
					break;
				}
			default: 
				{
					if (pCaster == pTarget)
					{
						break;
					}
					
					if (pTarget->sprno == 0 || pTarget->sprno == 1)  
					{ //< CSD-021128
						if (!g_mgrBattle.IsPickUp(pCaster, pTarget))
						{
							return NULL;
						}  
						
						break;
					} //> CSD-021128
					
					return NULL;
				}
			}
			//복수의 타격 대상체를 체크하여 에니메이트까지 뿌려줘야하는 경우(공격 당한 적은 또다시 공격당하지 않게)
			//< CSD-021104
			//if( range_Check==1 && !(ch->nCurrentAction == MON1_ATTACKED || ch->nCurrentAction == ACTION_ATTACKED) )
			if (range_Check == 1)
				//> CSD-021104
			{
				switch (lpML->magicSerial)
				{
				case METEOR_STRIKE:
					{
						if(my_Check==1 && ch==lpML->lpChar_Own)		// 내 자신이 타격대상일때
						{
							InsertMagic(lpML->lpChar_Own, ch, 182, 0, 0, 0, ch->x, ch->y);
						}
						else
						{
							InsertMagic(lpML->lpChar_Own, ch, 182, 9, 0, 0, ch->x, ch->y);
						}
						
						g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, ch->id, ch->x, ch->y);
						break;
					}
				case GREAT_PRAY:
					{
						switch (ch->sprno)
						{
						case 0:
						case 1:
							{
								g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, ch->id, ch->x, ch->y-ch->height);
								break;
							}
						}
						
						break;
					}
				case GREAT_SHIELD:
					{
						switch (ch->sprno)
						{
						case 0:
						case 1:
							{
								g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, ch->id, ch->x, ch->y-ch->height);
								break;
							}
						}
						
						break;
					}
				case ICE_BREATH:      
				case LIGHTNING_BREATH:
					{ //< CSD-021021
						InsertMagic(lpML->lpChar_Own, ch, lpML->next_Effect, 9, ch->x, ch->y-ch->height, ch->x, ch->y-ch->height);
						g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, ch->id, ch->x, ch->y-ch->height);
						break;
					} //> CSD-021021
/*
				case LIGHTNING_EXTREME:
					{ // 030328 kyo
						if( lpML->lpChar_Own != ch )
						{							
							InsertMagic(lpML->lpChar_Own, ch, lpML->next_Effect, 9, ch->x, ch->y-ch->height, ch->x, ch->y-ch->height);
							g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, ch->id, ch->x, ch->y-ch->height);						
						}						
						break;
					}
					*/
				default:
					{
						switch (my_Check)
						{
						case 1:
							{
								if (ch == lpML->lpChar_Own)
								{
									InsertMagic(lpML->lpChar_Own, ch, lpML->next_Effect, 0,0,0,ch->x,ch->y);
								}
								
								break;
							}
						case 2:
							{
								if (ch != lpML->lpChar_Own)
								{
									InsertMagic(lpML->lpChar_Own, ch, lpML->next_Effect, 9,0,0,ch->x,ch->y-ch->height);
								}
								
								break;
							}
						default:
							{
								InsertMagic(lpML->lpChar_Own, ch, lpML->next_Effect, 9, 0, 0, ch->x, ch->y-ch->height);
								break;
							}
						}
						
						switch (lpML->magicNum)
						{	//< CSD-031007
						case ITEM_EFFECT_FIRE:
						case ITEM_EFFECT_ICE:
						case ITEM_EFFECT_LIGHTNING:
							{
								g_mgrBattle.SendMagicResult(lpML->magicNum, lpML->lpChar_Own, ch->id, ch->x, ch->y-ch->height);
								break;
							}
						default:
							{
								g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, ch->id, ch->x, ch->y-ch->height);
								break;
							}
						}	//> CSD-031007

						break;
					}
				}
				
				ch = ch->lpNext;
			}			
			else	//0->단지 맞은 적이 있는지만 체크하여 리턴하는 경우
			{
				return ch;
			}
		}
		
		else	ch = ch->lpNext;	//충돌하지 않은 경우
	}
	
	return NULL;
}


BOOL FileReadData (char *buf, FILE *fp)
{
	//	_ASSERT (buf != NULL);
	//	_ASSERT (fp != NULL);
	
	char temp[_MAX_PATH];
	
	while (1)
	{
		if (fscanf(fp, "%s", temp) == EOF) return (FALSE);
		if (temp[0] == ';')										// 주석 처리
		{
			fgets (temp, _MAX_PATH-1, fp);
		}
		else
		{
			strcpy (buf, temp);
			return (TRUE);
		}
	}
}
extern bool IncordFile( char *filename, void *pData, int size );
// #####################################################################################

//  Effect 용 Loader

// #####################################################################################
BOOL LoadEffectSprite( int sprite_Num, char* sprite_Name)	//반드시 스프라이트의 번호와 이름은 일치하여야 함
{	//< CSD-030123
	FILE *fp;
	DWORD sysfilelength, size;
	DWORD nOff = 0;
	int   i;
	BYTE  *Buff;
	
	if( Effspr[sprite_Num].EffSpr[0].img ) 
	{
		return FALSE;
	}
	
	fp = Fopen( sprite_Name, "rb" );
	if( fp == NULL )
	{
		return FALSE;
	}

	if (sprite_Num != 152)
	{
		//return false;//koy
		fread( &Effspr[sprite_Num].Count, 2,1, fp);						//이펙트 스프라이트의 프레임수		
		fseek( fp, Effspr[sprite_Num].Count * 4, SEEK_CUR );			//오프셋 스킵
		
		sysfilelength = _filelength( _fileno( fp) ) - Effspr[sprite_Num].Count * 4 * 2 - 2;
		
		Buff=NULL;
		MemAlloc( Buff, sysfilelength );
		//Buff = Effspr[sprite_Num].Buff;
		
		DWORD templen = 0;
		for( i = 0 ; i < Effspr[sprite_Num].Count ; i++)
		{
			fread( &size, 4,1, fp);
			
			fread( &Effspr[sprite_Num].EffSpr[i].xl, sizeof( short ), 1, fp);	//X축의 스프라이트 크기
			fread( &Effspr[sprite_Num].EffSpr[i].yl, sizeof( short ), 1, fp);	//Y축의 스프라이트 크기
			fread( &Effspr[sprite_Num].EffSpr[i].ox, sizeof( short ), 1, fp);	//X축의 offset 위치
			fread( &Effspr[sprite_Num].EffSpr[i].oy, sizeof( short ), 1, fp);	//Y축의 offset 위치
			fread( &Effspr[sprite_Num].EffSpr[i].size, sizeof( unsigned int ), 1, fp);	//스프라이트 이미지의 크기
			
			fread( Buff + nOff, Effspr[sprite_Num].EffSpr[i].size,1 ,fp);		//스프라이트의 실제 이미지
			Effspr[sprite_Num].EffSpr[i].img = (char *)Buff + nOff;
			
			convert565to555( &Effspr[sprite_Num].EffSpr[i] );
			
			templen += Effspr[sprite_Num].EffSpr[i].size;
			nOff += size;
		}
		fclose(fp);
		return TRUE;
	}
	else // finito 09/08/07 load encrypted firebird
	{
		if (_filelength( _fileno( fp) ) != 213770 )
		{
			ExitApplication(EA_NORMAL);
			return FALSE;
		}

		CHSEL_STREAM hsel;
		int nVersion = 0;
		fread((char*)(&nVersion), sizeof(nVersion), 1, fp); 
		if(hsel.GetVersion() != nVersion)
		{
			ExitApplication(EA_NORMAL);
			return FALSE;
		}
		HselInit deinit;
		fread((char*)(&deinit), sizeof(HselInit), 1, fp);
		if( !hsel.Initial(deinit) )
		{
			ExitApplication(EA_NORMAL);
			return FALSE;
		}

		BYTE  *Buff2;
		Buff2=NULL;
		MemAlloc( Buff2, 213702 );

		fread(Buff2, 1, 213702, fp);
		hsel.Decrypt((char*)Buff2, 213702);

		int nOffset = 0;
		memcpy(&Effspr[sprite_Num].Count, Buff2 + nOffset, 2);						//이펙트 스프라이트의 프레임수	
		nOffset += 2; 
		if (Effspr[sprite_Num].Count != 80)
		{
			ExitApplication(EA_NORMAL);
			return FALSE;
		}	

		nOffset += Effspr[sprite_Num].Count * 4; 

		sysfilelength = 213702 - Effspr[sprite_Num].Count * 4 * 2 - 2;

		Buff=NULL;
		MemAlloc( Buff, sysfilelength );
		
		DWORD templen = 0;
		for( i = 0 ; i < Effspr[sprite_Num].Count ; i++)
		{
			memcpy(&size, Buff2 + nOffset, 4);	
			nOffset += 4; 
		
			memcpy(&Effspr[sprite_Num].EffSpr[i].xl, Buff2 + nOffset, 2);	
			nOffset += 2; 
			memcpy(&Effspr[sprite_Num].EffSpr[i].yl, Buff2 + nOffset, 2);	
			nOffset += 2; 
			memcpy(&Effspr[sprite_Num].EffSpr[i].ox, Buff2 + nOffset, 2);	
			nOffset += 2; 
			memcpy(&Effspr[sprite_Num].EffSpr[i].oy, Buff2 + nOffset, 2);	
			nOffset += 2; 
			memcpy(&Effspr[sprite_Num].EffSpr[i].size, Buff2 + nOffset, 4);	
			nOffset += 4; 

			memcpy(Buff + nOff, Buff2 + nOffset, Effspr[sprite_Num].EffSpr[i].size);	
			nOffset += Effspr[sprite_Num].EffSpr[i].size; 

			Effspr[sprite_Num].EffSpr[i].img = (char *)Buff + nOff;
			
			convert565to555( &Effspr[sprite_Num].EffSpr[i] );
			
			templen += Effspr[sprite_Num].EffSpr[i].size;
			nOff += size;
		}

		fclose(fp);
		return TRUE;
	}
}


#ifdef _DEBUG		// 모든 *.ab파일을 하나의 파일로 만든다.
#define _LOAD_EFFECT_ANI_AB_FILE
#endif

//#define _EFFECT_DATA_USING_DIVIDED_FILE //이팩트 잘린것을 사용한다. // 030821 kyo
//#define _EFFECT_DATA_PACK				//이펙트의 팩을 한다. 


////////////////////////////////////////////////////////////////////////////////////
BOOL LoadEffectAnimation(int animate_Num, char* animate_Name)
{
	
#ifdef _LOAD_EFFECT_ANI_AB_FILE
	if( !SysInfo.keyr ) return 0;
#else
	return 1;
#endif
	
	string strFileName = animate_Name;
	
	if (checkbeta == '1')
	{	//< CSD-031030
		const int nFind = strFileName.find_last_of('/');
		strFileName = "./effect_b5/" + strFileName.substr(nFind + 1);
	}	//> CSD-031030
	else
	{
		const int nFind = strFileName.find_last_of('/');
		strFileName = "./effect/" + strFileName.substr(nFind + 1);
	}
	
	strFileName += ".ab";
	
	if (!EffList.Load(animate_Num, strFileName.c_str()))
	{
		return FALSE;
	}
	
	return TRUE;
}

bool Order(int nOne, int nTwo)
{
	const int nTemp1 = g_lpMT[nOne].basic_Level;
	const int nTemp2 = g_lpMT[nTwo].basic_Level;
	return (nTemp1 < nTemp2) ? true:false;
}

bool LoadMagicTable2(void)
{
	bool bSuccess = false;
	
	if (checkbeta == '1')
	{	//< CSD-031030
		bSuccess = g_lpMT.Load("./effect_b5/thunder.ab");
	}	//> CSD-031030
	else
	{
		bSuccess = g_lpMT.Load("./effect/thunder.ab");
	}
	
	vector<int> vtWizard[12];
	
	for (int i = LIFE_UP; i <= AREA_SLOW; ++i) //마법사 마법의 모든 종류를 넣는다. 배워지지 않는것은 wizard.txt에서 빠진다.
	{
		const int nOrder = g_lpMT[i].order_Type - 1;
		
		if (nOrder >= 0 && nOrder <= 11)
		{
			vtWizard[nOrder].push_back(i);
		}
	}
	
	for (int j = 0; j < 12; ++j)
	{
		sort(vtWizard[j].begin(), vtWizard[j].end(), Order);
		
		for (int k = 0; k < vtWizard[j].size(); ++k)
		{
			n_MagicBagic[0][j][k] = vtWizard[j][k];
		}
	}
	
	vector<int> vtPriest[12];
	
	for (int l = RESTORE_AUTHORITY; l <= RAGE_LIGHT_BALL; ++l)
	{
		const int nOrder = g_lpMT[l].order_Type - 21;
		
		if (nOrder >= 0 && nOrder <= 11)
		{
			vtPriest[nOrder].push_back(l);
		}
	}
	
	for (int m = 0; m < 12; ++m)
	{
		sort(vtPriest[m].begin(), vtPriest[m].end(), Order);
		
		for (int n = 0; n < vtPriest[m].size(); ++n)
		{
			n_MagicBagic[1][m][n] = vtPriest[m][n] - 150;
		}
	}
	
	return bSuccess;
}

/////////////////////// 1228 상태간의 중복/무효 처리를 위한 테이블값 로딩 ///////////////////////////
void LoadConditionTable(void)
{
	FILE* file;
	int buff = 0;
	
	char DataPath[MAX_PATH]="./data/condition_table.txt";
	if( GetSysInfo( SI_GAME_MAKE_MODE ) ) file = Fopen( DataPath, "rt");
	else file = NULL; 
	if(file)
	{
		int i;
		for(i=0; i<8; i++)
			for(int j=0; j<8; j++)
				fscanf(file, "%d", &g_Condition_Table[i][j] );
			fclose(file);
			
			file=Fopen("./data/condition_table.bin", "wb" );
			if(file)
			{
				char *tt = (char *)g_Condition_Table;			//특정 화일정보를 1바이트 단위로 읽어 들이기 위해 캐스트 형변환(Byte point)
				char crc = 0;
				for( i = 0 ;  i < sizeof(int)*64 ;  i++ )		//Byte수 만큼 증가
				{
					crc += (*tt);		//포인터가 가르키는 임의 수를 누적시킴
					tt ++;				//다음 포인터로	
				}
				
				fwrite( &crc, 1,1, file );
				fwrite( g_Condition_Table, sizeof(int), 64, file);
				fwrite( &crc, 1,1, file );
				
				fclose(file);
			}
	}
	else
	{
		char *tt = (char *)g_Condition_Table;
		char crc = 0, crc1, crc2;
		file = Fopen( "./data/condition_table.bin", "rb" );
		
		fread( &crc1, 1,1, file );			//CheckSum 한 바이트를 읽어와서 Header변수에 셋팅
		fread( g_Condition_Table, sizeof(int), 64, file);
		fread( &crc2, 1,1, file );			//CheckSum 한 바이트를 읽어와서 Teil변수에 셋팅
		fclose(file);
		
		for( int i = 0 ;  i < sizeof(int)*64 ;  i ++)
		{
			crc += *tt;
			tt ++;
		}
		if( crc  != crc1 ) CheckSumError = CHECKSUM_ERROR_CONDITIONTABLE;
		if( crc  != crc2 ) CheckSumError = CHECKSUM_ERROR_CONDITIONTABLE;
		if( crc2 != crc1 ) CheckSumError = CHECKSUM_ERROR_CONDITIONTABLE;
		
		fclose(file);
	}
}

void ConditionSet(LPCHARACTER ch, BYTE condition, DWORD Time)
{
	ch->condition = condition;
}

void OrderSoundFile(void)
{
	char	DataPath_Static[MAX_PATH];
	char	DataPath[MAX_PATH];
	wsprintf( DataPath_Static, "%s/sound/test/*.*", GetCurrentWorkingDirectory() );
	wsprintf( DataPath, "del %s", DataPath_Static);
	system(DataPath);
}


void	RenameSoundFile(void)
{		//soundlist 및 화일명을 영문(숫자)로 전환
	FILE	*fp;
	char	DataPath[MAX_PATH] = "./SOUND/soundList_eng.bin";
	char	str[4],name[5],number[4];
	char	DataPathold[MAX_PATH];
	char	DataPathnew[MAX_PATH];
	
	char	DeleteFileName[35][MAX_PATH] = {"./SOUND/기술예비.wav","./SOUND/기타예약.wav","./SOUND/마시동5전격.wav","./SOUND/마시동반경9.wav","./SOUND/마시동보호막.wav",
		"./SOUND/마시동불저항.wav","./SOUND/마시동빙저항.wav","./SOUND/마시동얼음구.wav","./SOUND/마시동얼음원.wav","./SOUND/마시동전저항.wav","./SOUND/마시동지옥불.wav",
		"./SOUND/마시동직선전.wav","./SOUND/마시동체력.wav","./SOUND/마시동치료.wav","./SOUND/마시동화염구.wav","./SOUND/몬스터비명_.wav","./SOUND/몽둥이.wav","./SOUND/무기예비.wav",
		"./SOUND/미닫이닫기.wav","./SOUND/번개1.wav","./SOUND/번개2.wav","./SOUND/불새.wav","./SOUND/비명예비.wav","./SOUND/시전예비.wav","./SOUND/시전예비.wav","./SOUND/예약_.wav","./SOUND/자유음예비.wav",
		"./SOUND/클릭_.wav","./SOUND/타격예비.wav","./SOUND/화로불.wav","./SOUND/좀비공격.wav","./SOUND/화살타격 .wav","./SOUND/막자찢기.wav","./SOUND/STATIC/2발작보통큰걸음.wav","./SOUND/STATIC/4발작보통큰걸음.wav"};
	
	fp = Fopen( DataPath, "rb" );			//read only+text file
	if(fp)
	{
		fclose(fp);
		strcpy(DataPath,"./SOUND/soundList.bin");
		
		for(int i=0; i < 1000; i++)
		{
			//g_SoundList[i].list_Index;
			//strcpy(DataPathold, g_SoundList[i].sound_FileName);
			
			if(g_SoundList[i].sound_FileName == "예비.wav")
			{
				sprintf(name, "0141");
				//fprintf( fp, "%d	%s.wav\n", i, name);
				goto gotolabel1;
			}
			else if(i<10)									sprintf(str, "000");
			else if(i<100)									sprintf(str, "00");
			else											sprintf(str, "0");
			
			sprintf(number,"%d", i);
			sprintf(name, "%s%s", str, number);
			//fprintf( fp, "%d	%s.wav\n", i, name);
gotolabel1:				
			if(i<100)
			{
				sprintf(DataPathold, "./SOUND/static/%s", g_SoundList[i].sound_FileName);
				sprintf(DataPathnew, "./SOUND/static/%s.wav", name);
			}
			else
			{
				sprintf(DataPathold, "./SOUND/%s", g_SoundList[i].sound_FileName);
				sprintf(DataPathnew, "./SOUND/%s.wav", name);
			}
			
			SetFileAttributes(DataPathold, FILE_ATTRIBUTE_NORMAL );
			rename( DataPathold, DataPathnew );
		}
		
		for(int j=0; j<35; j++)
		{
			SetFileAttributes(DeleteFileName[j], FILE_ATTRIBUTE_NORMAL );
			DeleteFile(DeleteFileName[j]);
		}
		
		SetFileAttributes(DataPath, FILE_ATTRIBUTE_NORMAL );
		DeleteFile( DataPath );
		
		char OldDataPath[MAX_PATH]={"./SOUND/soundlist_eng.bin"};
		rename( OldDataPath ,DataPath );
	}
	else	return;
	
	ZeroMemory( g_SoundList, sizeof(SOUNDLIST)*MAX_SOUND );
	LoadSoundList();
}

void RenameEffectFile(void)
{	//< CSD-030324
	return; // THAI YGI
}	//> CSD-030324


bool LoadEffect(void)		//인자:읽어 들일 이펙트테이블의 갯수(0 ~ n-1)
{ //020808 YGI 
	bool bSuccess = false;
	//< CSD-021015
	if (checkbeta == '1')
	{	//< CSD-031030
		if (!g_lpET.Load("./effect_b5/Thunder.dat"))
		{
			return false;
		}
		
		if (!g_lpET2.Load("./effect_b5/Lightning.dat"))  
		{
			return false;	
		}
	}	//> CSD-031030
	else
	{
		if (!g_lpET.Load("./effect/Thunder.dat"))
		{
			return false;
		}
		
		if (!g_lpET2.Load("./effect/Lightning.dat"))  
		{
			return false;	
		}
	}
	//> CSD-021015

#ifdef _EFFECT_DATA_USING_DIVIDED_FILE // 030821 kyo //kyo

	vector<string> lSource, lTarget;
	lSource.clear();
	for(int count=0;count<MAX_EFFECT_COUNT; ++count)
	{
		lSource.push_back(g_lpET2[count].sprite_Name ) ;
	}
	lTarget.push_back( "./effect/EffectDiv1" );
	lTarget.push_back( "./effect/EffectDiv2" );
	lTarget.push_back( "./effect/EffectDiv3" );
	lTarget.push_back( "./effect/EffectDiv4" );
	lTarget.push_back( "./effect/EffectDiv5" );
	lTarget.push_back( "./effect/EffectDiv6" );
	lTarget.push_back( "./effect/EffectDiv7" );

	if(!g_MergeData.Init(lSource, lTarget, ".dat", ".bin"))
	{
		//msg 추가
		return false;		
	}

	#ifdef _EFFECT_DATA_PACK
		g_MergeData.MergeFile();
	#endif
#endif
	int x=0;
	// 스태틱 버퍼에 미리 스프라이트 이미지를 가지고 있어야 하는 경우 로딩처리
	char	buf[50];
	
	char pAlreadyRead[MAX_EFFECT_COUNT] = {0, };
	for (int i = 0; i < MAX_EFFECT_COUNT; ++i)
	{
		if (g_lpET[i].static_Effect)
		{
			strcpy(buf, g_lpET2[i].sprite_Name);		//020808 YGI 
			strcat(buf, ".dat");
#ifdef _EFFECT_DATA_USING_DIVIDED_FILE // 030821 kyo		
			::LoadEffectSpriteDivFile( g_lpET[i].sprite_Num );
#else
			::LoadEffectSprite(g_lpET[i].sprite_Num, buf);
#endif
			EffsprLoaded[g_lpET[i].sprite_Num] = true;
		}
		
#ifdef _LOAD_EFFECT_ANI_AB_FILE
		if( pAlreadyRead[g_lpET[i].sprite_Num] ) continue;
		
		strcpy(buf, g_lpET2[i].sprite_Name);
		if( !LoadEffectAnimation(g_lpET[i].sprite_Num, buf ) )
		{
			return LoadEffectAnimationAll();
		}
		pAlreadyRead[g_lpET[i].sprite_Num] = 1;
#endif
	}
	
#ifdef _LOAD_EFFECT_ANI_AB_FILE
	SaveEffectAnimationAll( pAlreadyRead );
	JustMsg( "SaveEffectAnimationAll() Success!!!!" );
#endif
	
#ifndef _LOAD_EFFECT_ANI_AB_FILE
	return LoadEffectAnimationAll();
#endif
	
	return true;
}

BOOL InitEffect()
{
	InitList();
	return TRUE;
}

void InitList(void)
{
	g_lpML = NULL;
}

void DestoryList(void)
{
	//	free(NowList);
}

USEEFFECTLIST *FindList(USEEFFECTLIST *Head, int EffectNum)
{
	USEEFFECTLIST * Temp;
	
	Temp = Head;
	
	while(1)
	{
		if (Temp == NULL) return FALSE;
		
		if (Temp->sprite_Num == EffectNum) return Temp;
		
		Temp = Temp->Next;
	}
}

void DeleteList(USEEFFECTLIST **Head, USEEFFECTLIST *TempList)
{			
	USEEFFECTLIST * Temp = NULL, *Temp2 = NULL;
	
	if (*Head == TempList)
	{		
		Temp = (*Head)->Next;
		MemFree(*Head);
		//free(*Head);
		
		*Head = Temp;
		return;
	}		
	
	Temp = *Head;
	
	while(1)
	{		
		if (Temp->Next == NULL) break;
		
		if (Temp->Next == TempList)
		{	
			Temp2 = Temp->Next->Next;
			MemFree(Temp->Next);
			//free(Temp->Next);
			Temp->Next = Temp2;
			
			break;
		}	
		
		Temp = Temp->Next;
	}		
}			

void FreeEffectSprite( int sprite_Num )		//0이 아닌 경우만 호출
{			
	if( effSpr_Count[sprite_Num]==0 || EffsprLoaded[ sprite_Num ] == true )
	{		
		return;
	}		
	else	
		effSpr_Count[sprite_Num]--;
	
	// 누적 카운트를 1 감소시켰을 때 0일 경우 
	if( effSpr_Count[sprite_Num] == 0 )
	{		
		MemFree( Effspr[sprite_Num].EffSpr[0].img );
		Effspr[sprite_Num].Count = 0;
		//MemFree( EffList[sprite_Num].EffData );					//에니메이션 화일까지 지우기!
	}		
}			

void DeleteAllList(USEEFFECTLIST **Head)
{			
	USEEFFECTLIST * Temp = *Head;
	if (Temp == NULL) {return;}

	while(1)
	{		
		Temp = (*Head)->Next;
		MemFree(*Head);
		//free(*Head);
		*Head = NULL;
		
		if (Temp == NULL) {break;}
		
		*Head = Temp;
	}		
}			

void InsertList(USEEFFECTLIST **Head, USEEFFECTLIST *TempList)	//Overriding 함수
{
	TempList->Next = *Head;
	*Head = TempList;
}

void InsertMagicList(MAGICLIST **Head, MAGICLIST *TempList)	
{
	TempList->Next = *Head;
	*Head = TempList;
}

MAGICLIST *FindList(MAGICLIST *Head, int MagicNum)
{
	MAGICLIST * Temp;
	
	Temp = Head;
	
	while(1)
	{
		if (Temp == NULL) return FALSE;
		
		if (Temp->magicNum == MagicNum) return Temp;
		
		Temp = Temp->Next;
	}
}

MAGICLIST* FindList(MAGICLIST *Head, int MagicNum, int idTarget)
{
	MAGICLIST* Temp = Head;
	
	while(1)
	{
		if (Temp == NULL) return NULL;
		//if (Temp->magicSerial == MagicNum && Temp->Target_id == idTarget && Temp->end_Time != 0) return Temp;
		if (Temp->magicSerial == MagicNum && Temp->Target_id == idTarget) return Temp;
		Temp = Temp->Next;
	}
}

void DeleteList(MAGICLIST **Head, MAGICLIST *TempList)
{
	MAGICLIST *Temp, *Temp2;
	
	if (TempList == *Head)
	{
		//필요없어진 스프라이트를 날리기 위해 변수에 해당 스프라이트 번호 저장
		if( FreeEffectSpriteFlagCount < MAX_EFFECT_COUNT*2 )
		{
			FreeEffectSpriteFlag[ FreeEffectSpriteFlagCount ] = TempList->sprite_Num;
			FreeEffectSpriteFlagCount ++;
		}
		
		//FreeEffectSprite(TempList->sprite_Num);
		Temp = (*Head)->Next;
		
		DeleteAllList( &((*Head)->EffHead) );
		MemFree(*Head);
		*Head = NULL;
		*Head = Temp;
		
		return;
	}
	
	Temp = *Head;
	
	while(1)
	{
		if (Temp->Next == NULL) break;
		
		if (Temp->Next == TempList)
		{
			//필요없어진 스프라이트를 날리기 위해 변수에 해당 스프라이트 번호 저장
			if( FreeEffectSpriteFlagCount < MAX_EFFECT_COUNT*2 )
			{
				FreeEffectSpriteFlag[ FreeEffectSpriteFlagCount ] = TempList->sprite_Num;
				FreeEffectSpriteFlagCount ++;
			}
			
			Temp2 = Temp->Next->Next;
			
			if (Temp->Next->EffHead != NULL)
			{	//< CSD-TW-030606
				DeleteAllList( &(Temp->Next->EffHead) );

				if (Temp2 != NULL && Temp->Next->magic_Runtime > 0)
				{
					Temp2->magic_Runtime = Temp->Next->magic_Runtime;
				}
			}	//> CSD-TW-030606

			MemFree(Temp->Next);
			//Temp->Next = NULL;
			Temp->Next = Temp2;
			
			break;
		}
		else Temp = Temp->Next;
	}
}

void DeleteAllList(MAGICLIST **Head)
{
	MAGICLIST * Temp;
	
	while(1)
	{
		Temp = (*Head)->Next;
		MemFree(*Head);
		//free(*Head);
		*Head = NULL;
		
		if (Temp == NULL) break;
		
		*Head = Temp;
	}
}

int GetTargetHeight(int nMagic, LPCHARACTER pTarget)
{ //< CSD-020603 : 말을 탄 상태에서 위치 보정
	return 0;
	//> CSD-020603
	switch (pTarget->sprno)
	{ 
    case 0:
    case 1:  
		{
			if (pTarget->HorseNo > 0)  break;
		}
    default: 
		{
			return 0;
		}
	}
	
	switch (nMagic)
	{
    case SLOW:
    case ENCHANT_WEAPON:
    case MINOR_AMPLIFY:
    case MAGIC_AMPLIFY:
    case MIRACLE_RECOVER:
	case CONFUSION:
	case STONELY:
	case INTOXICATION:
	case FIRE_BALL:
	case SCORCHER:
	case CIRCLE_FLAME:
	case FIRE_WALL:
	case FLAME_WAVE:
	case APOCALYPSE:
	case LOW_MONSTER_RECALL:
	case MIDDLE_MONSTER_RECALL:
	case PHASING:
	case TELEPORT:
	case TOWN_PORTAL:
	case SPEED_UP:
		{
			return 0;
		}  
	}
	
	return 25;
}

void InsertMagic(LPCHARACTER lpChar_Own, LPCHARACTER lpChar_Target, int magicNum, int magicSubNum, int s_X, int s_Y, int t_X, int t_Y, bool	firePattern, int next_Effect, bool failed, DWORD dwDuration)
{
	MAGICLIST*	TempList = NULL;
	char		buf[30] = {0,};
	
	////////////////////// 0629 lkh 추가 ///////////////////////
	if( lpChar_Own==NULL || lpChar_Target==NULL || magicNum==0 )	return;
	
	MemAlloc( TempList, sizeof(MAGICLIST));
	if(TempList == NULL)
		MessageBox(NULL,"Memory Allocation Error..",0,MB_OK);
	
	TempList->magicSerial = lpChar_Own->excute_MagicNum;			//실제 마법 번호(1~18?)
	
	TempList->magicNum = magicNum;									//실제 마법에 연결된 실행되고 있는 서브 마법 번호
	TempList->magicSubNum = magicSubNum;							//같은 마법이지만 구현의 차이를 두기위한 서브 타입
	
	TempList->sprite_Num = g_lpET[magicNum].sprite_Num;
	
	TempList->spr_Speed = g_lpET[magicNum].spr_Speed;
	TempList->spr_Height = g_lpET[magicNum].spr_Height;
	TempList->target_Height = GetTargetHeight(TempList->magicSerial, lpChar_Target);
	//->AnimationTable[lpChar_Target->nCurrentAction].height[lpChar_Target->nCurrentFrame.GetDecrypted()];
	if(s_X!=0)
		TempList->x = s_X;
	else
		TempList->x = lpChar_Own->x;								//주인공의 위치가 시작위치인 경우 필요
	if(s_Y!=0)
		TempList->y = s_Y;
	else
		TempList->y = lpChar_Own->y;
	
	TempList->s_X = s_X;											//특정지점이 시작위치인 경우 필요
	TempList->s_Y = s_Y;		
	
	if(t_X!=0)
		TempList->tarx = t_X;
	else
		TempList->tarx = lpChar_Target->x;							//타겟인물의 위치가 목표위치인 경우 필요
	if(t_Y!=0)
		TempList->tary = t_Y;
	else
		TempList->tary = lpChar_Target->y-lpChar_Target->height;
	
	TempList->t_X = t_X;											//특정지점이 목표위치인 경우 필요
	TempList->t_Y = t_Y;
	
	TempList->map_Offset.x = g_Map.x;								//효과 최초 발생시의 배경 offset 좌표
	TempList->map_Offset.y = g_Map.y;
	
	TempList->dir = lpChar_Own->direction;							//이펙트의 방향
	
	TempList->trans_Type = g_lpET[magicNum].trans_Type;				//이펙트의 출력 타입
	TempList->trans_Level = g_lpET[magicNum].trans_Level;			//이펙트의 출력 레벨
	
	//mpList->body_Part = g_lpET[magicNum].body_Part;		
	
	TempList->object_Num = g_lpET[magicNum].object_Num;				//동시에 실행될 동일 이팩트수
	TempList->fallow = g_lpET[magicNum].fallow;						//주인공을 따라올것인지?
	TempList->firePattern = firePattern;							//1->발사체가 적을 유도할까?/0->최초 목표점으로 날라갈까?
	
	TempList->lpChar_Own	= lpChar_Own;
	TempList->lpChar_Target = lpChar_Target;
	
	TempList->Caster_id		= lpChar_Own->id;
	TempList->Target_id		= lpChar_Target->id;
	
	if(next_Effect!=0)			//InserMagic함수로 다음 이팩트의 번호가 인자로 넘겨진 경우 적용
		TempList->next_Effect = next_Effect;
	else						//넘겨지지 않으면 Effect_Table에서 참조
		TempList->next_Effect = g_lpET[magicNum].next_Effect;
	
	TempList->impect_Effect_Num = g_lpET[magicNum].impect_Effect_Num;
	TempList->link_Frame = g_lpET[magicNum].link_Frame;
	TempList->end_Time = g_lpET[magicNum].end_Time;
	TempList->sound_LoopCount = g_lpET[magicNum].sound_loop;
	TempList->flag = 0;
	TempList->next_Flag = 0;
	if (dwDuration > 0)  TempList->magic_Runtime	= dwDuration;
	TempList->failed = failed;
	
	//사운드 로딩
	///////////////// 0401 수정/보완 ///////////////////
	//if(g_lpET[magicNum].sound_Num!=0 && g_lpET[magicNum].sound_loof==0) 	//한번만 플레이하는 경우
	if( g_lpET[magicNum].sound_Num!=0 )
	{
		int play_Sound = g_lpET[magicNum].sound_Num % 1000;
		
		if(lpChar_Own == Hero)
			PlayListAutoSounds(g_lpET[magicNum].sound_Num, 0, 20-(g_lpET[magicNum].sound_Volume/5), 0 );
		else
		{
			int	distance_X = (lpChar_Own->x - Hero->x )/*/TILE_SIZE*/;			// 0910 YGI	// LTS SOUND
			int distance_Y = (lpChar_Own->y - Hero->y )/*/TILE_SIZE*/;			// 0910 YGI	// LTS SOUND
			
			//PlayListAutoSounds(g_lpET[magicNum].sound_Num, distance_X, distance_Y, 0 ); // LTS SOUND
			PlayListAutoSounds(g_lpET[magicNum].sound_Num,distance_X, distance_Y, 0);		//3D sound로 표현	// LTS SOUND
		}
	}
	//effSpr_Count[Temp->sprite_Num]==0;
	//스프라이트 로딩( 스프라이트 번호가 0번이 아니고 이펙트 누적 카운트가 0인 경우(최초로딩) )
	
	/////////////////////////// 스타틱 로딩 스프라이트가 아닌 경우 ////////////////////////////////
	// 010205 KHS 
	//	if( !g_lpET[magicNum].static_Effect ) // !((TempList->sprite_Num>=66 && TempList->sprite_Num<=71) || TempList->sprite_Num==78 || TempList->sprite_Num==79 || (TempList->sprite_Num>=235 && TempList->sprite_Num<=236) ) )
	if( 1 ) // !((TempList->sprite_Num>=66 && TempList->sprite_Num<=71) || TempList->sprite_Num==78 || TempList->sprite_Num==79 || (TempList->sprite_Num>=235 && TempList->sprite_Num<=236) ) )
	{
		//////////////////////// 0706 lkh 수정(버그퇴치) ////////////////////////
		if( (effSpr_Count[TempList->sprite_Num]==0 || Effspr[TempList->sprite_Num].EffSpr->img==NULL ) && TempList->sprite_Num!=0 )	//처음 로딩되는 경우(magicNum는 0~499까지)	
		{
			strcpy( buf,g_lpET2[TempList->magicNum].sprite_Name );		//020808 YGI 
			strcat( buf ,".dat" );

#ifdef _EFFECT_DATA_USING_DIVIDED_FILE // 030821 kyo //kyo
			::LoadEffectSpriteDivFile( g_lpET[magicNum].sprite_Num );
#else
			::LoadEffectSprite(g_lpET[magicNum].sprite_Num, buf);
#endif
			
			strcpy( buf,g_lpET2[TempList->magicNum].sprite_Name );		//020808 YGI  
			//strcat( buf, ".af" );
			LoadEffectAnimation( g_lpET[magicNum].sprite_Num, buf );
		}
		//이펙트 누적 카운트 더함
		effSpr_Count[g_lpET[magicNum].sprite_Num]++;
		//		effSpr_Count[TempList->sprite_Num]++;
	}
	InsertMagicList(&g_lpML, TempList);
}

void InsertEffect(MAGICLIST *Temp, int start_Frame, int nOffX, int nOffY)
{
	USEEFFECTLIST *TempList;
	
	TempList=NULL;
	MemAlloc( TempList, sizeof(USEEFFECTLIST));
	//TempList = (USEEFFECTLIST *) calloc( 1, sizeof(USEEFFECTLIST));
	switch (start_Frame)
	{
    case 0:
		{
			TempList->x = Temp->x;
			TempList->s_X = Temp->s_X;
			TempList->y = Temp->y + nOffY;
			TempList->s_Y = Temp->s_Y;
			break;
		}
    case 1:
		{
			TempList->x = Temp->x - nOffX/2;
			TempList->s_X = Temp->s_X;
			TempList->y = Temp->y + nOffY/2;
			TempList->s_Y = Temp->s_Y;
			break;
		}
    case 2:
		{
			TempList->x = Temp->x - nOffX;
			TempList->s_X = Temp->s_X;
			TempList->y = Temp->y;
			TempList->s_Y = Temp->s_Y;
			break;
		}
    case 3:
		{
			TempList->x = Temp->x - nOffX/2;
			TempList->s_X = Temp->s_X;
			TempList->y = Temp->y - nOffY/2;
			TempList->s_Y = Temp->s_Y;   
			break;
		}
    case 4:
		{
			TempList->x = Temp->x;
			TempList->s_X = Temp->s_X;
			TempList->y = Temp->y - nOffY;
			TempList->s_Y = Temp->s_Y;   
			break;
		}
    case 5:
		{
			TempList->x = Temp->x + nOffX/2;
			TempList->s_X = Temp->s_X;
			TempList->y = Temp->y - nOffY/2;
			TempList->s_Y = Temp->s_Y;   
			break;
		}
    case 6:
		{
			TempList->x = Temp->x + nOffX;
			TempList->s_X = Temp->s_X;
			TempList->y = Temp->y;
			TempList->s_Y = Temp->s_Y;   
			break;
		}
    case 7:
		{
			TempList->x = Temp->x + nOffX/2;
			TempList->s_X = Temp->s_X;
			TempList->y = Temp->y + nOffY/2;
			TempList->s_Y = Temp->s_Y;   
			break;
		}
	}
	
	TempList->tarx = Temp->tarx;
	TempList->t_X = Temp->t_X;
	TempList->tary = Temp->tary;
	TempList->t_Y = Temp->t_Y;
	
	TempList->dir = Temp->dir;
	TempList->trans_Level = (float)Temp->trans_Level;
	TempList->speed       = Temp->spr_Speed;
	
	/*
	if(g_lpET[Temp->magicNum].pattern_Num == 20)
	{		
	switch(Temp->lpChar_Own->direction)
	{
	case DIRECTION_DOWN:
	TempList->first_Angle = 0;
	break;
	case DIRECTION_LEFTDOWN:
	TempList->first_Angle = 45;
	break;
	case DIRECTION_LEFT:
	TempList->first_Angle = 90;
	break;
	case DIRECTION_LEFTUP:
	TempList->first_Angle = 135;
	break;
	case DIRECTION_UP:
	TempList->first_Angle = 180;
	break;
	case DIRECTION_RIGHTUP:
	TempList->first_Angle = 225;
	break;
	case DIRECTION_RIGHT:
	TempList->first_Angle = 270;
	break;
	case DIRECTION_RIGHTDOWN:
	TempList->first_Angle = 315;
	break;
	}
	}
	*/
	
	TempList->first_Angle=0;
	
	TempList->height      = (float)Temp->spr_Height;
	
	TempList->sprite_Num = Temp->sprite_Num;
	TempList->FrameCount = start_Frame;
	TempList->effect_Count = Temp->EffectCount;
	
	TempList->StartFlag = 0;
	TempList->flag = 0;
	
	InsertList(&Temp->EffHead, TempList);
	
	/*
	if(TempList->speed==0 && TempList->dir==0)
	TempList->RepeatCount = 0;
	else
	TempList->RepeatCount = EffList[TempList->sprite_Num].EffData[TempList->FrameCount].RepeatFrame;
	*/
}

/////////////////////////// 0126 lkh /////////////////////////////////
int	ClockOutPut(int x, int y, Spr* sp)		//좌표 -> 절대좌표
{
	LPORDERTABLE	o;
	
	o = &g_OrderInfo.order[ g_OrderInfo.count ];
	
	o->wType	= SPRITETYPE_CLOCKNIDDLE;		//19
	
	o->x		= x;
	o->y		= y;
	o->offset	= y;
	o->height	= CLOCK_HEIGHT;
	
	o->lpvData	= (LPVOID ) sp;
	
	o->show		= 1;
	o->sort		= 1;
	g_OrderInfo.count++;
	
	return 0;
}

int EffectOutput(int x, int y, Spr* sp, int type, int alpha, int height, int sort_Type, DWORD rgb )
{
	switch(type/10)		//두자리수인 경우 앞자리의 값이 trans 방법
	{	
	case 1:
		type=1;
		break;
	case 2:
		type=2;
		break;
	}
	
	LPORDERTABLE  o;
	
	o = &g_OrderInfo.order[ g_OrderInfo.count ];
	
	o->wType		= sort_Type;
	
	o->type			= type;
	
	if(type==7)
	{
		o->x			= x+(rand()%5-2) + Mapx;
		o->y			= y + Mapy;
	}
	else
	{
		o->x			= x + Mapx;
		o->y			= y + Mapy;
	}
	o->height		= height;
	o->alpha		= alpha;
	o->rgb			= rgb;
	o->offset		= y + Mapy;
	o->lpvData		= (LPVOID ) (sp);
	
	o->show			= 1;
	o->sort			= 1;
	g_OrderInfo.count++;
	
	return 0;
}


void DeleteOwnerPoint( LPCHARACTER ch )
{
	MAGICLIST *Temp = g_lpML;
	MAGICLIST *temp;
	
	while( Temp != NULL )		//시전자의 포인터가 날라간 경우
	{
		if( Temp->lpChar_Own == ch ) 
		{
			if( Temp->lpChar_Own != Temp->lpChar_Target )
			{
				Temp->lpChar_Own = Temp->lpChar_Target;
			}
			else
			{
				temp = Temp->Next;
				DeleteList( &g_lpML, Temp);
				Temp = temp;
				continue;
			}
		}
		Temp = Temp->Next;
	}
	
	while( Temp != NULL )		//대상자의 포인터가 날라간 경우	
	{
		if( Temp->lpChar_Target == ch ) 
		{
			if( Temp->lpChar_Target != Temp->lpChar_Own )
			{
				Temp->lpChar_Target = Temp->lpChar_Own;
			}
			else
			{
				temp = Temp->Next;
				DeleteList( &g_lpML, Temp);
				Temp = temp;
				continue;
			}
		}
		Temp = Temp->Next;
	}
}

void EffectProc()
{	//< CSD-TW-030606
	if (g_lpML == NULL)    return;
	if (g_lpMT.IsError())  return;
	if (g_lpET.IsError())  return;
	
	MAGICLIST* Temp = g_lpML;
	MAGICLIST* temp = NULL;
	
	while (Temp != NULL)
	{ 
		switch (g_lpET[Temp->magicNum].pattern_Num)
		{
		case CASTING_MAGIC:
			{
				Temp = CastingMagic(Temp);
				break;
			}
		case CASTING_COMBAT:
			{
				Temp = CastingCombat(Temp);
				break;
			}
		case CASTING_MAINTAIN:
			{
				Temp = CastingContinueMagic(Temp);
				break;
			}
		case STRAIGHT_FORWARD:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = SampleMagic1(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case PROTECT_WALL:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = WallCreate(Temp);
				}
				else
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case CHANGE_COLOE:
			{
				if ((ReturnCharacterPoint(Temp->Target_id) != NULL) && (Temp->lpChar_Own->hp > 0))
				{
					Temp = SampleMagic3(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case FIX_EFFECT:
			{
				LPCHARACTER	lpTarget = ReturnCharacterPoint(Temp->Target_id);
				LPCHARACTER	lpCaster = ReturnCharacterPoint(Temp->Caster_id);

				//if (lpTarget != NULL && lpCaster != NULL) // Finito null check
				//{
				Temp->lpChar_Target = lpTarget;
				Temp->lpChar_Own	= lpCaster;
				
				if (ReturnCharacterPoint(Temp->Target_id) && (Temp->lpChar_Own == Hero || (Temp->lpChar_Own != Hero && ReturnCharacterPoint(Temp->Caster_id))) && Temp->lpChar_Own->hp > 0)
				{
					Temp = SampleMagic4(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				//}
				
				break;
			}
		case CIRCLE_FORWARD:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = SampleMagic5(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case CIRCLE_RING:
			{
				if ((ReturnCharacterPoint(Temp->Target_id) != NULL) && (Temp->lpChar_Own->hp > 0))
				{
					Temp = SampleMagic6(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case SHOOT_16:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = FireArrow(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case SHOOT_8:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = SampleMagic8(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList( &g_lpML, Temp );
					Temp = temp;
				}
				
				break;
			}
		case ROUND_FIRE:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = SampleMagic9(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case STORM:
			{
				if (ReturnCharacterPoint( Temp->Target_id ) != NULL)
				{
					Temp = SampleMagic10(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case POINT_TO_POINT:
			{
				LPCHARACTER	lpCaster = ReturnCharacterPoint(Temp->Caster_id);
				Temp->lpChar_Own	= lpCaster;
				LPCHARACTER	lpTarget = ReturnCharacterPoint(Temp->Target_id);
				Temp->lpChar_Target = lpTarget;
				
				if (lpCaster != NULL && lpTarget != NULL)
				{
					Temp = OutToCharacter(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case 13:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = SampleMagic1_1(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}  
		case RAINY_OBJECT:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = RainyObject(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}  
		case MOVE_WALL:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = Firewall1(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case FISH_JUMP:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = FishJump(Temp);
				}
				else
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case HOMING:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = Homing(Temp);
				}
				else
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case SKILL_RESULT:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = SkillResult(Temp);
				}
				else
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case NOVA:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = Nova(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case TRACE:
			{
				if(ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = Trace(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
			}break;
		case PARTICLE:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = RadiantStar(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			}
		case TYPE4_LIKE:
			{
				if (ReturnCharacterPoint(Temp->Target_id) != NULL)
				{
					Temp = SampleMagic_Type4_Like(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList(&g_lpML, Temp);
					Temp = temp;
				}
				
				break;
			} 
		case LOCAL_CLASH:
			{ 
				if ((ReturnCharacterPoint(Temp->Target_id) != NULL) && (Temp->lpChar_Own->hp > 0))
				{
					Temp = SampleMagic25(Temp);
				}
				else 
				{
					temp = Temp->Next;
					DeleteList( &g_lpML, Temp );
					Temp = temp;
				}
				
				break;
			} 
		default:
			{ // 만약 일치하는 효과출력패턴이 없는 경우 -> 해당 마법을 리스트에서 지움 (0번 패턴이거나 없는 패턴인 경우)
				temp = Temp->Next;
				DeleteList(&g_lpML, Temp);
				Temp = temp;
				break;
			}
		}
	}
}	//> CSD-TW-030606

MAGICLIST* RainyObject(MAGICLIST* lpML)
{
	
	return lpML->Next;
}

MAGICLIST* SkillResult(MAGICLIST* lpML)
{
	USEEFFECTLIST*  lpEL;
	DWORD		check_Time=0;
	int			spr_Num=0;
	bool		target_Type=0;	
	int			terminateFrame=0;
	int			temp_X=0,temp_Y=0;
	//char		buf[30];
	
	lpEL=lpML->EffHead;
	
	if(lpML->magicSubNum==9)		target_Type=1;	//만약 서브 타입이 9면 적에게 효과소스 출력
	
	switch(lpML->PCount)
	{
	case 0:
		
		InsertEffect(lpML);	
		//효과리스트에 마법 하나 등록된 사실 셋팅
		
		lpML->PCount = 1;
		break;
		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거 
		{
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				CharDoAni( lpML->lpChar_Own , lpML->lpChar_Own->direction , lpML->lpChar_Own->basicAction );
				
				lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				
				if (lpEL == NULL)
				{
					return buff;
				}
			}
			
			/////////////////////////// 광원 종료 조건 확인하여 링크드 리스트에서 제외 ////////////////////////////
			if( g_lpET[lpML->magicNum].light!=0 && lpML->flag==0 && (DayLightControl < 31) )	//소멸되는 광원 초기화 시켜주기 
			{
				terminateFrame=(EffList[lpEL->sprite_Num].MaxFrame)/LIGHT_TERMINATE;
				if(terminateFrame<1)	terminateFrame=1;
				
				if(g_lpET[lpML->magicNum].light/100 == 1 )
				{
					InsertLightOut( lpML->tarx, (int)(lpML->tary-lpEL->height), g_lpET[lpML->magicNum].light%100, terminateFrame, g_lpET[lpML->magicNum].light/100 );
				}
				else
				{
					InsertLightOut( lpML->tarx, lpML->tary, g_lpET[lpML->magicNum].light%100, terminateFrame, g_lpET[lpML->magicNum].light/100 );
				}
				lpML->flag=1;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			
			///////////////////////////// 마지막 프레임까지 다다랐을 때 종료 //////////////////////////////////	
			if (lpML->magicSubNum!=0 && (abs((int)lpEL->incx)>80 || abs((int)lpEL->incy)>80) )
			{
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			else if(lpML->magicSubNum==0 && lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame)
			{
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			if(lpML->magicSubNum==0 && lpML->loop_Count%11==0 && lpML->EffectCount<=lpML->object_Num)
			{
				temp_X=lpML->x;
				temp_Y=lpML->y;
				lpML->x+=rand()%31-15;
				lpML->y+=rand()%31-15;
				InsertEffect(lpML);
				lpML->EffectCount ++;
				lpML->x=temp_X;
				lpML->y=temp_Y;
			}
			else if ( lpML->magicSubNum!=0 && lpML->loop_Count%6==0 && lpML->EffectCount<=lpML->object_Num)
			{
				InsertEffect(lpML);
				lpML->EffectCount ++;
			}
			
			if (lpEL->Next == NULL)			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
			{
				break;
			}
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
		}
		
		lpEL = lpML->EffHead;
		
		/////////////////////////////////////////////////////////////////////////////////
		
		while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
		{
			//연결프레임과 현재 프레임 동일하면
			if( lpML->link_Frame!=0 && lpML->link_Frame==lpEL->FrameCount)
			{
				if(lpML->magicSubNum==8)	InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,9,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);
				else						InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);	
			}
			
			if(lpML->trans_Type%10==8)		//점점 밝게 생성 / 마지막 어둡게
			{
				if(lpEL->FrameCount<EffList[lpEL->sprite_Num].MaxFrame-4)
				{
					lpEL->trans_Level=(lpML->trans_Level/(float)EffList[lpEL->sprite_Num].MaxFrame)*lpEL->FrameCount;
					if(lpEL->trans_Level > lpML->trans_Level)	lpEL->trans_Level=(float)lpML->trans_Level;
				}
				else
				{
					lpEL->trans_Level-=(float)(lpML->trans_Level/4.);
					if(lpEL->trans_Level < 1) lpEL->trans_Level = 1;
				}
			}
			
			// 마지막 4프레임은 alpha off
			else if(lpML->trans_Type%10==9 && lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame-4)	
			{
				lpEL->trans_Level-=(lpML->trans_Level/4);//-((lpML->trans_Level/EffList[lpEL->sprite_Num].MaxFrame)*(lpEL->FrameCount-1));
				if(lpEL->trans_Level<=1)	lpEL->trans_Level=1;
			}
			
			else if(g_lpET[lpML->magicNum].end_Time!=0)			//쉴드와 같은 장시간 유지 마법의 경우 색깔값 지속적 떨어트림
			{
				lpEL->trans_Level=(float)(lpML->trans_Level*(g_lpET[lpML->magicNum].end_Time-lpEL->total_Frame)/g_lpET[lpML->magicNum].end_Time);
				if(lpEL->trans_Level<0)	lpEL->trans_Level=0;
			}
			/////////////////////// 가속처리하지 않는 경우 이동치 조정 //////////////////////
			switch(lpML->magicSubNum)
			{
			case 1:		//S자 곡선으로 위로 올라가기
				lpEL->incx = (float)(15.+(5.*lpEL->FrameCount)*cos((lpEL->total_Frame*30)%360*M_PI/180.));
				lpEL->incy = (float)(-30-(lpEL->total_Frame*2));
				//lpEL->incy=(15+(8*lpEL->FrameCount))*sin((lpEL->total_Frame*30)%360*M_PI/180);
				break;
			case 2:		//원운동하며 위로 올라가기
				lpEL->incx = (float)((20.+(5.*lpEL->FrameCount))*cos((lpEL->total_Frame*30)%360*M_PI/180.));
				lpEL->incy = (float)((10.+(5.*lpEL->FrameCount))*sin((lpEL->total_Frame*30)%360*M_PI/180.)-(lpEL->total_Frame*2.));
				break;
			case 3:		//'ㄱ'자 반대로 이동 
				lpEL->incx +=(float)(10.-(lpEL->total_Frame/3.)-(lpEL->total_Frame));
				lpEL->incy -=(float)(11.+(lpEL->total_Frame/5.)-(lpEL->total_Frame));
				if(lpEL->incy>0)	lpEL->incy+=2.;
				break;
			case 4:		//'ㄴ'자 형태 이동
				lpEL->incx -=(float)(10.-(lpEL->total_Frame/3.)-(lpEL->total_Frame));
				lpEL->incy +=(float)(11.+(lpEL->total_Frame/5.)-(lpEL->total_Frame));
				if(lpEL->incy<0)	lpEL->incy-=4.;
				break;
			case 5:		//원운동하며 퍼져나가기
				lpEL->incx = (float)((30.+(8.*(lpEL->total_Frame/6.)))*cos((lpEL->total_Frame*30)%360*M_PI/180.));
				lpEL->incy = (float)((15.+(5.*lpEL->FrameCount))*sin((lpEL->total_Frame*30)%360*M_PI/180.));
				break;
			case 6:		//위에서 지그재그로 퍼지며 떨어짐
				if((lpEL->total_Frame/8)%2==0)	lpEL->incx +=(float)(lpEL->total_Frame/3.);
				else							lpEL->incx -=(float)(lpEL->total_Frame/3.);
				lpEL->incy=(float)(-60.+lpEL->total_Frame*2.);
				break;
			}
			
			if(lpML->magicSubNum!=0 && lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame/10)
			{
				//lpML->magicSubNum++;
				//if(lpML->magicSubNum>5)	lpML->magicSubNum=1;
				lpEL->FrameCount=0;
			}
			
			//특정 외곽 라인에 근접할수록 색깔값 사라짐
			lpEL->trans_Level=(float)(lpML->trans_Level*(( 100. - abs((int)lpEL->incx) ) / 100.));
			lpEL->trans_Level=(float)(lpEL->trans_Level*(( 100. - abs((int)lpEL->incy) ) / 100.));
			if(lpEL->trans_Level<1)	lpEL->trans_Level=1;
			
			if(lpML->flag==0)	//광원 소멸 함수 호출되지 않은 경우
				DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, target_Type);
			else
				DrawEffect(lpEL, lpML, 0, target_Type);
			
			lpEL->total_Frame++;
			lpEL = lpEL->Next;			
		}
		break;
	}
	return lpML->Next;
}

//16개 방향 화살 발사
MAGICLIST* FireArrow(MAGICLIST* lpML)//, int Num, int motion_Num, int speed, int height, int type, int levlpEL)
{
	USEEFFECTLIST*	lpEL;
	LPITEMGROUND	lpItem;
	
	//BOOL		effect_Dont=0;
	int			crash_Result=0;
	int			temp_Direction;
	DIRECTION	direction_Arrow;
	int			tx=0,ty=0,tz=0,tarx=0,tary=0,tarz=0;
	int			air_Surface=0;
	int			distanceRange=0;
	int			repeat_FireTime=0;			//민첩도에 따른 캐릭의 던지거나 쏘는데 걸리는 시간
	
	//////////////////////////// 0721 lkh 추가(곡사포 구현) ///////////////////////////
	int			distance_target=0;
	int			distance_ArrowSpeed=0;
	int			round_degree=0;
	
	if (lpML->target_Height <= 32 &&  g_mgrBattle.IsThrow(lpML->magicNum, false)) // 지상적이면서 일반활이거나 비검인 경우만 곡사포 원리 적용
	{ //< CSD-021021
		distance_target	= int(sqrt(pow((float)lpML->x - lpML->tarx, (float)2)+pow((float)lpML->y - lpML->tary, (float)2)));
		distance_ArrowSpeed	= distance_target/ARROW_SPEED;
		// 9스텝(arrow_speed:32) - > 288
		if(distance_ArrowSpeed >= 7)  round_degree = 16; // 증감 화살의 높이값
	} //> CSD-021021
	
	lpEL = lpML->EffHead;
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:
		{ //< CSD-021021
			if (!g_mgrBattle.IsThrow(lpML->magicNum, false))
			{	// 화살 계열은 원거리 어택 루틴에서 처리되므로 처리하지 않음
				ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
				lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
				switch (lpML->magicSerial)
				{
				case FLAME_ARROW: // CSD-031020
				case MULTIPLE_FIRE:
				case CONVERTING_ARMOR:
				case FIRE_EXTREME:
				case ICING_BLAST:
				case SHARK_MISSILE:
				case ICE_EXTREME:
					{
						break;
					}
				default:
					{
						g_mgrBattle.ActAnimation(lpML->lpChar_Own->nCurrentAction, lpML->lpChar_Own);
						break;
					}
				}
			}
			
			temp_Direction=lpML->lpChar_Own->direction;
			// 주인공 방향에 따른 수정좌표 셋팅(don't touch)
			if (lpML->dir != NULL && g_mgrBattle.IsThrow(lpML->magicNum, false))
			{ //< CSD-021021
				lpML->x+=point_Modify[lpML->dir][0];
				lpML->y+=point_Modify[lpML->dir][1];
			} //> CSD-021021
			//발사무기의 각도 알아내기
			ChangeDirection16((DIRECTION*)&direction_Arrow, lpML->x, lpML->y, lpML->tarx, lpML->tary);
			if(!((int)direction_Arrow>=0 && (int)direction_Arrow<=15))
				lpML->dir=lpML->lpChar_Own->direction;
			else
				lpML->dir=(int)direction_Arrow;
			
			InsertEffect(lpML);
			
			lpML->PCount = 1;
			lpML->dir=temp_Direction;		//캐릭터의 방향은 8방향이므로 화살발사각도 셋하고 다시 돌려줌
			break;
		} //> CSD-021021
	case 1:
		while(1)
		{
			if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				if( lpML->lpChar_Own->changeActionFlag == 1 )	lpML->lpChar_Own->changeActionFlag = 0;
				
				switch (lpML->magicSerial)
				{
				case FLAME_ARROW: // CSD-031020
				case MULTIPLE_FIRE:
				case CONVERTING_ARMOR:
				case FIRE_EXTREME:
				case ICING_BLAST:
				case SHARK_MISSILE:
				case ICE_EXTREME:
					{
						break;
					}
				default:
					{
						g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
						break;
					}
				}
				
				MAGICLIST* buff;
				buff = lpML->Next;
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);			//매직 링크드 리스트에서 제거
				
				if (lpEL == NULL)	return buff;
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					EndCastMagic(lpML->lpChar_Own);
					
					switch (lpML->magicSerial)
					{
					case FLAME_ARROW: // CSD-031020
					case MULTIPLE_FIRE:
					case CONVERTING_ARMOR:
					case FIRE_EXTREME:
					case ICING_BLAST:
					case SHARK_MISSILE:
					case ICE_EXTREME:
						{
							break;
						}
					default:
						{
							g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
							break;
						}
					}
				}
			}
			
			// 이펙트의 좌표가 벋어날 경우
			if(abs((int)lpEL->incx)>=SCREEN_WIDTH || abs((int)lpEL->incy)>=SCREEN_WIDTH)
			{
				DeleteList(&(lpML->EffHead), lpEL);	//이펙트 링크드 리스트에서 제거
				lpEL = lpML->EffHead;
				continue;
			}
			
			//효과 이펙드가 위치하는 타일의 속성 체크하여 이펙트 소멸
			if(lpML->target_Height==0)			//적이 지상적(높이값 없는)인 경우
			{
				if (IsCollision(lpML->magicSerial,
					lpEL->x>>5,
					lpEL->y>>5,
					(lpEL->x + (int)lpEL->incx)>>5, 
					(lpEL->y + (int)lpEL->incy)>>5) == true)
				{
					if(lpML->next_Effect == 0 )
					{ 
						switch (lpML->magicNum)
						{	//< CSD-031014
						case THROW_SNOW_BALL:
							{
								InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, 206, 9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+lpML->spr_Height-lpEL->height));
								break;    
							}
						case THROW_WATER_BALL:
							{
								InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, 662, 9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+lpML->spr_Height-lpEL->height));
								break;
							}
						case THROW_BIG_GULP:
							{
								InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, 664, 9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+lpML->spr_Height-lpEL->height));
								break;
							}
						case THROW_ARROW:
							{	// 화살
								InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, 204, 9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+lpML->spr_Height-lpEL->height));
								break;
							}
						case THROW_KNIFE:
							{	// 비도
								InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, 210, 9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+lpML->spr_Height-lpEL->height));
								break;
							}
						case THROW_FIRE_ARROW:
							{	// 비도
								InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, 153, 9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+lpML->spr_Height-lpEL->height));
								break;
							}
						case THROW_ICE_KNIFE:
							{	// 비도
								InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, 170, 9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+lpML->spr_Height-lpEL->height));
								break;
							}
						}	//> CSD-031014
					} 
					else if(!(lpML->next_Effect==181 || lpML->next_Effect==191 || lpML->next_Effect==170 ) )	//피(순수타격이팩트)를 제외한 다른 타격 이팩트 배경에 출력
					{
						InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, lpML->next_Effect,  9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+lpML->spr_Height-lpEL->height) );
					}
					
					DeleteList(&(lpML->EffHead), lpEL);	//이펙트 링크드 리스트에서 제거
					lpEL = lpML->EffHead;
					continue;
				}
			}				
			
			int check_xl = (int)(Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl*.8);		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
			int check_yl = (int)(Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl*.8);
			
			if( check_xl < 64 )	check_xl=64;
			if( check_yl < 64 ) check_yl=64;
			
			int magicNum = lpML->magicSerial;
			
			switch (lpML->magicNum)
			{	//< CSD-031014
			case THROW_SNOW_BALL:
			case THROW_WATER_BALL: 
			case THROW_BIG_GULP:   
			case THROW_ARROW:
			case THROW_KNIFE:
			case THROW_FIRE_ARROW:
			case THROW_ICE_KNIFE:
				{
					magicNum = lpML->magicNum;
					break;
				}
			}	//> CSD-031014
			//발사 이펙트가 적 케릭터에 맞았는지 체크(첫번째인자 1인 경우->복수 타일 체크 & 충돌처리를 함수에 맞김/마지막 바로전 인자->내 자신(시전자) 체크여부/마지막 인자->공중적 체크 여부)
			air_Surface= (lpML->target_Height>0) ? 1:0;
			
			BOOL	crash_Result=0;
			int check_Fire_X = (lpEL->x+(int)lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2);
			int check_Fire_Y = (lpEL->y+(int)lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-(check_yl/2);
			
			attacked_Char=ReturnMagicCheckedCharacter(0, lpML, check_Fire_X, check_Fire_Y, (int)lpEL->height, check_xl, check_yl, 0, air_Surface, (BOOL) round_degree);	//공중의 적까지 체크
			// 충돌한 인물이 있으면
			if (g_mgrBattle.IsAttack(lpML->lpChar_Own, attacked_Char, magicNum))
			{
				if(lpEL!=NULL)
				{
					g_mgrBattle.SendMagicResult(magicNum, lpML->lpChar_Own, attacked_Char->id, (int)(lpEL->x+lpEL->incx+.5), (int)(lpEL->y+lpEL->incy+.5));
				}
				
				if(lpML->next_Effect==0)
				{
					RandomThrowEffect(lpML->magicNum, lpML->lpChar_Own, attacked_Char);
					GetCombat()->Effect(lpML->lpChar_Own, attacked_Char);
				}
				else
				{
					InsertMagic(lpML->lpChar_Own,attacked_Char, lpML->next_Effect, 9,0,0,attacked_Char->x,attacked_Char->y-attacked_Char->height);
				}
				
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			else
			{
				lpItem=ReturnMagicCheckedItem(0, lpML, (int)((lpEL->x+lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2.)), (int)((lpEL->y+lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-(check_yl/2.)), (int)lpEL->height, check_xl, check_yl);
				if(lpItem!=NULL)
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
			}
			
			if (lpEL->Next == NULL)
			{
				break;
			}
			else lpEL = lpEL->Next;
			}
			
			lpEL = lpML->EffHead;
			
			tx = lpEL->x;						//최초 발사 좌표
			ty = lpEL->y;						//      "
			//tz = (int)(lpEL->height+.5);		//최초 발사 스프라이트 높이
			
			distanceRange=(int)(sqrt(pow((float)lpML->x-lpML->tarx,(float)2)+pow((float)lpML->y-lpML->tary,(float)2)));
			
			while(lpEL != NULL)
			{	
				switch (lpML->lpChar_Own->sprno)
				{
				case 0:
				case 1:
					{ //0212 특정 대상을 클릭하고 너무 근접하지 않은 경우
						if( (lpML->lpChar_Own != lpML->lpChar_Target) && distanceRange>48 )		//특정적 클릭시 반유도개념 적용
						{
							switch (lpML->magicSerial)
							{
							case FLAME_ARROW: // CSD-031020
							case MULTIPLE_FIRE:
							case CONVERTING_ARMOR:
							case FIRE_EXTREME:
							case ICING_BLAST:
							case SHARK_MISSILE:
							case ICE_EXTREME:
								{
									tarx = lpML->tarx;			//너무 근접한 경우는 충분히 떨어진 수정 좌표를 셋팅
									tary = lpML->tary;
									tarz = (lpML->lpChar_Own != lpML->lpChar_Target) ? lpML->lpChar_Target->height:0;
									break;
								}
							default:
								{
									tarx = lpML->lpChar_Target->x;		//목표좌표
									tary = lpML->lpChar_Target->y;		//-lpML->lpChar_Target->height;
									tarz = lpML->lpChar_Target->height;
									break; 
								}
							}
						}
						else							//특정 위치 클릭하거나 대상과 너무 근접한 경우
						{
							tarx = lpML->tarx;			//너무 근접한 경우는 충분히 떨어진 수정 좌표를 셋팅
							tary = lpML->tary;
							tarz = (lpML->lpChar_Own != lpML->lpChar_Target) ? lpML->lpChar_Target->height:0;
						}
						
						break;
					}
				default:
					{
						tarx = lpML->tarx;			//너무 근접한 경우는 충분히 떨어진 수정 좌표를 셋팅
						tary = lpML->tary;
						tarz = (lpML->lpChar_Own != lpML->lpChar_Target) ? lpML->lpChar_Target->height:0;
						break;
					}
				}
				
				float incx=0., incy=0., incz=0.;
				
				if(distanceRange <= 48)		//거리가 너무 근접했을 때
				{
					switch((int)lpML->lpChar_Own->direction)
					{
					case 0:
						incx=0;
						incy=ARROW_SPEED;
						break;
					case 1:
						incx=-ARROW_SPEED*2/3;
						incy=ARROW_SPEED*2/3;
						break;
					case 2:
						incx=-ARROW_SPEED;
						incy=0;
						break;
					case 3:
						incx=-ARROW_SPEED*2/3;
						incy=-ARROW_SPEED*2/3;
						break;
					case 4:
						incx=0;
						incy=-ARROW_SPEED;
						break;
					case 5:
						incx=ARROW_SPEED*2/3;
						incy=-ARROW_SPEED*2/3;
						break;
					case 6:
						incx=ARROW_SPEED;
						incy=0;
						break;
					case 7:
						incx=ARROW_SPEED*2/3;
						incy=ARROW_SPEED*2/3;
						break;
					}
				}
				
				else if(abs(tx-tarx) > abs(ty-tary))	//X축 차이가 Y축 차이보다 큰 경우
				{	
					incy = ARROW_SPEED * ( tary - ty ) / (float)(abs( tarx - tx ));
					incx = (float)((lpML->tarx < lpML->x) ? -ARROW_SPEED : ARROW_SPEED);
					incz = (float)tarz / (abs(tarx-tx)/(float)ARROW_SPEED);
				}	
				else							//X축, Y축 차이가 같거나 Y축 차이가 큰 경우
				{	
					incy = (float)((lpML->tary < lpML->y ) ? -ARROW_SPEED : ARROW_SPEED);
					incx = ARROW_SPEED * (tarx - tx) / (float)(abs( tary - ty ));
					incz = (float)tarz / (abs(tary-ty)/(float)ARROW_SPEED);
				}
				///////////// 0221 lkh 추가 ( 유도개념의 발사체의 방향 자체가 완전히 전환되는 것 막기 위해)
				lpEL->oldincx = lpEL->incx;
				lpEL->oldincy = lpEL->incy;
				
				lpEL->height += incz;
				
				if(round_degree)
				{
					if(lpEL->total_Frame <= distance_ArrowSpeed/3)
						lpEL->height += round_degree;
					else if(lpEL->total_Frame >= distance_ArrowSpeed*2/3)
						lpEL->height -= round_degree;
				}
				
				lpEL->incx += incx;
				lpEL->incy += incy/*-incz*/;		//높이값을 적용한 수치
				
				if(lpEL->FrameCount>=(EffList[lpML->sprite_Num].MaxFrame/16))
					lpEL->FrameCount=0;
				
				float temp_incx = lpEL->incx;
				lpEL->incx=	lpEL->oldincx;
				float temp_incy = lpEL->incy;
				lpEL->incy=	lpEL->oldincy;
				
				DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
				lpEL->total_Frame ++;
				
				lpEL->incx=temp_incx;
				lpEL->incy=temp_incy;
				
				lpEL = lpEL->Next;
			}	
			break;
	}			
	return lpML->Next;
}
// 전격5타일.
MAGICLIST* OutToCharacter(MAGICLIST* lpML)	//커랙터 외부에서 몸 중앙으로 소스 이동(서브넘버가 0이면 시전자/0이 아니면 대상자)
{
	switch (lpML->magicSerial)
	{  
    case GREAT_THUNDER_BOLT:
		{
			if (lpML->magic_Runtime > g_ClientTime)
			{
				return lpML->Next;
			}
			
			break;
		}
	}
	
	USEEFFECTLIST* lpEL;
	int tarx=0,tary=0;
	bool target=0;
	int temp_t_X=0,temp_t_Y=0;
	int rand_Num=0;
	int	radius_X=0,radius_Y=0;
	
	lpEL = lpML->EffHead;
	
	if(lpML->magicSubNum==9 || lpML->magicSubNum==99)	target=1;
	
	switch(lpML->PCount)
	{		
	case 0:	
		if( lpML->lpChar_Own->id < 10000 )
		{	
			ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
			lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
			CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->nCurrentAction);
		}	
		
		//CharDoAni( lpML->lpChar_Own , lpML->lpChar_Own->direction , ACTION_MAGIC_CASTING );
		if(lpML->magicSubNum < 0)
		{	
			int		start_FrameNum = EffList[lpML->sprite_Num].MaxFrame / 3 * (rand()%3);
			InsertEffect( lpML, start_FrameNum );
		}	
		else
			InsertEffect( lpML );
		
		lpML->PCount = 1;
		break;
		
	case 1:	
		if(lpML->spr_Height==0 && lpML->spr_Speed==0)		//높이값과 속도가 0 일때(평면)
		{	
			//각도 알아내기
			if(lpML->magicSubNum==0)		//시전자의 외부에서 이동
				ChangeDirection((DIRECTION*)&lpEL->dir, lpML->lpChar_Own->x, lpML->lpChar_Own->y, lpEL->x, lpEL->y);
			else							//대상체의 외부에서 이동
				ChangeDirection((DIRECTION*)&lpEL->dir, lpML->lpChar_Target->x, lpML->lpChar_Target->y, lpEL->x, lpEL->y);
		}	
		
		while(1)
		{	
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//지금 지울 매직의 다음 매직 리스트를 임시 기억
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				CharDoAni( lpML->lpChar_Own , lpML->lpChar_Own->direction , lpML->lpChar_Own->basicAction );
				lpML->EffectCount = 0;
				
				DeleteList(&g_lpML, lpML);		//전역 매직 링크드 리스트에서 현 매직리스트만 제거
				
				if (lpEL == NULL)
				{	
					return buff;				//다음 매직리스트의 포인터를 넘겨줌
				}	
			}		
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{		
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//복수번 발사 가능 마법인 경우 발사 유지 단계를 종료
				{	
					EndCastMagic(lpML->lpChar_Own);
					CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction );
				}	
			}		
			
			//마지막 프레임까지 진행되면 효과종료 시킴
			int end_Frame;
			if(lpML->magicSubNum < 0)	end_Frame = (EffList[lpEL->sprite_Num].MaxFrame)/3;
			else						end_Frame = (EffList[lpEL->sprite_Num].MaxFrame);
			
			if( lpEL->FrameCount >= end_Frame )
			{		
				//CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction );
				DeleteList(&(lpML->EffHead), lpEL);				//현 마법에 연결된 복수(단수)의 효과리스트에서 종료효과를 지워버림 
				lpEL = lpML->EffHead;
				continue;
			}		
			
			//마지막 전 프레임에서 다음 효과 link를 위한 플레그 셋팅
			if( lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame-2 ) //lpEL->FrameCount%3==0/*EffList[lpEL->sprite_Num].MaxFrame-2*/)
				lpML->flag=1;
			
			if(lpML->next_Effect!=0 && lpML->link_Frame==0)	//번개와 같은 공격계열 효과인 경우 발사 이펙트가 적 케릭터에 맞았는지 체크->타격 효과 출력
			{		
				//	이곳이 >=로 되어 있었다. 굳이 이렇게 할필요 없다.  001025 KHS
				if(lpEL->FrameCount == EffList[lpML->sprite_Num].MaxFrame-2)
				{	
					//attacked_Char=ReturnMagicCheckedCharacter(1, lpML, (lpML->x) - 200, (lpML->y) - 110, lpEL->height, 400, 240, 0);
					// REturnMagicCheckedCharacter함수 내에서 자체 타격 이펙트 출력
					g_mgrBattle.Display(DISPLAY_EARTHQUAKE);
					
					switch (lpML->magicSerial)
					{
					case LIGHTNING_BOLT:
					case GREAT_THUNDER_BOLT:
						{
							attacked_Char=ReturnMagicCheckedCharacter(0, lpML, lpEL->t_X - 32, lpEL->t_Y - 32, (int)lpEL->height, 64, 64, 0);
							
							if (attacked_Char != NULL)
							{
								PlayListAutoSounds(2210, 0, 0, 0);
							}
							
							InsertMagic(lpML->lpChar_Own, lpML->lpChar_Own, lpML->next_Effect, 0, 0, 0, lpML->tarx, lpML->tary);
							break;
						}
					default:
						{
							attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpEL->t_X - 32, lpEL->t_Y - 32, (int)lpEL->height, 64, 64, 0);
							
							if( attacked_Char== NULL && lpML->magicSerial == 54 && rand()%2==0 )
							{
								PlayListAutoSounds(2210, 0, 0, 0);
							}
							
							break;
						}
					}
				}
			}	
			
			else if(lpML->link_Frame != 0 && lpEL->FrameCount == lpML->link_Frame )
			{	
				if(lpML->magicSubNum==99 || lpML->magicSubNum==9)
					InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target, lpML->next_Effect , 9, 0, 0, 0, 0);
				else
					InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target, lpML->next_Effect , lpML->magicSubNum, 0, 0, 0, 0);
			}	
			
			//	얼음폭풍(블리자드류) 지속 연출위해
			if(lpEL->FrameCount%2)
			{	
				if( !lpML->link_Frame && lpML->magicSubNum<0 && g_lpET[lpML->magicNum].object_Num > lpML->EffectCount )
				{
					do
					{
						radius_X = lpML->lpChar_Own->x + rand()%801 - 250;
						radius_Y = lpML->lpChar_Own->y + rand()%601 - 350;
						
					}	while( sqrt(pow((float)lpML->lpChar_Own->x - radius_X, (float)2)+pow((float)lpML->lpChar_Own->y - radius_Y, (float)2)) > 500 );
					
					lpML->x = radius_X;
					lpML->y = radius_Y;
					
					int		start_Frame = EffList[lpML->sprite_Num].MaxFrame / 3 * (rand()%3);
					InsertEffect( lpML, start_Frame );
					
					lpML->EffectCount++;
				}
			}	
			
			if(g_lpET[lpML->magicNum].sound_loop!=0)
				EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
			
			if (lpEL->Next == NULL)
			{	
				if(lpML->next_Effect && lpML->link_Frame==0)
					lpML->lpChar_Own->ready_Magic=0;
				if(lpML->magicSubNum>=0)
					memset(&array_Check,0, (sizeof(bool)*20));	//배열 테입블 클리어
				break;
			}	
			else lpEL = lpEL->Next;
		}		
		
		lpEL = lpML->EffHead;
		
		while(lpEL != NULL)
		{		
			if(lpML->spr_Height==0)	//높이값 없이 단순히 평면이동시
			{	
				int tx = lpEL->x;
				int ty = lpEL->y;
				
				if(lpML->magicSubNum==0)
				{
					tarx = lpML->lpChar_Own->x;
					tary = lpML->lpChar_Own->y;
				}
				else 
				{
					tarx = lpML->lpChar_Target->x;
					tary = lpML->lpChar_Target->y;
				}
				int incx, incy;
				
				if(abs(tx-tarx) > abs(ty-tary))
				{	
					incy = lpEL->speed * ( tary - ty ) / abs( tarx - tx );
					incx = ((tarx<tx) ? -lpEL->speed : lpEL->speed);
				}	
				else 
				{	
					incy = ((tary<ty ) ? -lpEL->speed : lpEL->speed);
					incx = lpEL->speed * ( tarx - tx ) / abs( tary - ty ) ;
				}	
				
				//lpEL->incx -= incx;
				lpEL->incx += incx;
				lpEL->incy += incy;
			}
			
			else	//높이값 있고 높이 개념 있을시
			{
				//만약 얼음폭풍마법이면서 각각의 이펙트 프레임이 6프레임 이상인 경우는 더 이상 아래로 떨어지면 안된다.
				switch (lpML->magicSerial)
				{ //< CSD-020620
				case LIGHTNING_BOLT:
				case GREAT_THUNDER_BOLT:
					{
						if (lpEL->FrameCount >= 6)
						{ 
							lpEL->height -= lpML->spr_Speed;
							if (lpEL->height <= 0)	lpEL->height = 0;
						} 
						
						break;
					}
				default:
					{
						lpEL->height -= lpML->spr_Speed;
						if (lpEL->height <= 0)	lpEL->height = 0;
						break;
					}  
				} //> CSD-020620
				////////////////////////// 0705 lkh (블리자드 연출위해) ////////////////////////////
				if(lpML->magicSubNum < 0)
				{
					if(lpEL->FrameCount < 12) 
					{
						lpEL->incx -= 17 ;
						lpEL->incy += 2 ; 
					}
				}
			}	
			
			if( ( lpML->EffectCount < lpML->object_Num) && lpML->magicSubNum >= 0 && lpML->flag == 1/*&& (StartFlag == 1) && (lpML->flag==0)*/)
			{	
				if(lpML->object_Num)
				{
					temp_t_X=lpML->t_X;
					temp_t_Y=lpML->t_Y;
					//memset(&array_Check,0, (sizeof(bool)*20));
					
					rand_Num=rand()%20;
					while(array_Check[rand_Num]==1)
					{
						rand_Num=rand()%20;
					}
					array_Check[rand_Num]=1;
					
					lpML->t_X=lpML->t_X+(lightning_Point[rand_Num][0]*64);
					lpML->tarx = lpML->t_X;
					lpML->t_Y=lpML->t_Y+(lightning_Point[rand_Num][1]*64);
					lpML->tary = lpML->t_Y;
				}
				
				lpML->EffectCount++;
				
				///////////// 0106 이규훈 다음의 효과 프레임 링크 ////////////
				// 링크전 내려칠 곳이 집안인 경우(집안속성)는 링크하지 않음 //
				//////////////////////////////////////////////////////////////
				if( TileMap[lpML->t_X/32][lpML->t_Y/32].attr_dont == 0 && TileMap[lpML->t_X/32][lpML->t_Y/32].attr_inside == 0 )
					InsertEffect(lpML);
				
				lpML->t_X=temp_t_X;
				lpML->t_Y=temp_t_Y;
				lpML->flag = 0;
			}	
			
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, target);
			
			lpEL = lpEL->Next;
		}		
		break;	
	}			
	return lpML->Next;
}					

//얼음 마법		
MAGICLIST* SampleMagic11(MAGICLIST* lpML, int Num, int motion_Num, int max_Object, int speed, int height, int type, int levlpEL, int end_Frame)		//스프라이트의 고유번호
{				
	USEEFFECTLIST* lpEL;
	int		crash_Result=0;
	
	BOOL StartFlag = TRUE;
	
	lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:
		if (lpML->lpChar_Own == NULL)
		{
			MAGICLIST* buff;
			
			buff = lpML->Next;
			
			lpML->EffectCount = 0;
			DeleteList(&g_lpML, lpML);
			
			if (lpEL == NULL)
			{
				return buff;
			}
		}
		else
			lpML->PCount = 1;
		
		break;
		
		
	case 1:				
		lpML->x = lpML->lpChar_Own->x;
		lpML->y = lpML->lpChar_Own->y;
		
		InsertEffect(lpML);
		lpML->EffHead->StartFlag = 1;
		
		//매직리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 2;
		break;
		
	case 2:		//2개 등록된 경우
		while(1)
		{
			if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				buff = lpML->Next;
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				
				g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
				
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);
				
				if (lpEL == NULL) return buff;
			}
			
			if(lpEL->trans_Level <= 0)
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			if (lpEL->Next == NULL)
			{
				break;
			}
			else lpEL = lpEL->Next;
		}
		
		lpEL = lpML->EffHead;
		
		while(1)
		{
			if (lpEL == NULL) break;
			
			float temp_incx = lpEL->incx;	
			lpEL->incx=	lpEL->oldincx;
			float temp_incy = lpEL->incy;
			lpEL->incy=	lpEL->oldincy;
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, 1);
			lpEL->incx=temp_incx;
			lpEL->incy=temp_incy;
			
			if((lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame ) && (lpEL->StartFlag == 1))
			{
				lpEL->FrameCount --;
			}
			else 
			{
				if (lpEL->StartFlag == 0) lpEL->FrameCount = 0;
				
				lpEL->dir ++;    // 단순한 증가값
				
				if ((lpEL->dir == 15) && (lpEL->StartFlag == 1))
				{
					lpEL->dir = 100;
					
					if (lpEL->flag == 0)
					{
						lpEL->flag = 1;
						InsertEffect(lpML);
					}
				}
				else if (lpEL->dir >= 105)
				{
					lpEL->dir = 100;
					lpEL->trans_Level--;
					
					if (lpEL->Next != NULL)
						lpEL->Next->trans_Level--;
				}
			}
			
			if (lpEL->Next == NULL)
				break;
			else lpEL = lpEL->Next;			
		}
		break;
	}
	return lpML->Next;
}

//시전자 서있는 방향 & 일정 속도로 날아가는 이팩트(링크드리스트의 장점 최대살린 pattern/화염기둥)->반드시 종결조건을 소스의 최종프레임에 다다랐을 경우로 해야함
// 이를 어겼을시 문제는 책임 못짐!
MAGICLIST* SampleMagic1(MAGICLIST* lpML)//, int Num, int motion_Num, int max_Object, int speed, int height, int type, int levlpEL, int end_Frame)		//스프라이트의 고유번호
{
	USEEFFECTLIST* lpEL;
	LPITEMGROUND	lpItem;
	
	static int inc = 0;
	int	crash_Result=0;
	int backup_LevlpEL=0;
	
	BOOL StartFlag = TRUE;
	
	lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:				
		ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
		lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
		g_mgrBattle.ActAnimation(lpML->lpChar_Own->nCurrentAction, lpML->lpChar_Own);
		lpML->dir = lpML->lpChar_Own->direction;
		
		switch(lpML->lpChar_Own->direction)
		{
		case 0:
			lpML->y += 32;
			break;
		case 1:
			lpML->x -= 24;
			lpML->y += 24;
			break;
		case 2:
			lpML->x -= 32;
			break;
		case 3:
			lpML->x -= 24;
			lpML->y -= 24;
			break;
		case 4:
			lpML->y -= 32;
			break;
		case 5:
			lpML->x += 24;
			lpML->y -= 24;
			break;
		case 6:
			lpML->x += 32;
			break;
		case 7:
			lpML->x += 24;
			lpML->y += 24;
		}
		
		InsertEffect(lpML);
		
		//매직리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 1;
		break;
		
		case 1:		//1개 등록된 경우
			while(1)
			{
				if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
				{
					MAGICLIST* buff;
					
					buff = lpML->Next;
					
					if( lpML->lpChar_Own->changeActionFlag == 1 ) 
					{
						lpML->lpChar_Own->changeActionFlag = 0;
					}
					
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					lpML->EffectCount = 0;
					DeleteList(&g_lpML, lpML);
					
					if (lpEL == NULL) 
						return buff;
				}
				
				if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
				{
					if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
					{
						EndCastMagic(lpML->lpChar_Own);
						g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					}
				}
				
				//마지막 프레임까지 진행된 경우 종료시킬까?
				if(lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame )
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->flag=1;
					lpML->EffectCount--;
					continue;
				}
				
				//사정거리 벋어날시 소멸시킴
				if(lpEL->incx>=320 || lpEL->incy>=320)
				{	
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->flag=1;
					lpML->EffectCount--;
					continue;
				}
				//이동불가 지형타일과 충돌시 소멸시킬까?
				if (IsCollision(lpML->magicSerial,
					lpEL->x>>5,
					lpEL->y>>5,
					(lpEL->x + (int)lpEL->incx)>>5, 
					(lpEL->y + (int)lpEL->incy)>>5) == true)
				{
					InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target,lpML->next_Effect, 9, 0, 0, (int)(((lpEL->x+lpEL->oldincx)/32.)*32.+16.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+16.) );
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->flag=1;
					lpML->EffectCount--;
					continue;
				}
				
				int check_xl = (int)(Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl*.5);		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
				int check_yl = (int)(Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl*.5);
				// 대형 스프라이트의 경우 보정 필요
				
				//발사 이펙트가 적 케릭터에 맞았는지 체크(첫번째인자 1인 경우->복수 타일 체크 & 충돌처리를 함수에 맞김/마지막 인자->내 자신(시전자) 체크여부)
				attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (lpEL->x+(int)(lpEL->incx+.5)+EffList[lpEL->sprite_Num].offset_X-(check_xl/2)), (lpEL->y+(int)(lpEL->incy+.5)+EffList[lpEL->sprite_Num].offset_Y-check_yl/2), (int)lpEL->height, check_xl, check_yl );
				//< CSD-021104  
				//if(attacked_Char!=NULL && !(attacked_Char->nCurrentAction == MON1_ATTACKED || attacked_Char->nCurrentAction == ACTION_ATTACKED)) //충돌한 인물이 있으면
				if (attacked_Char != NULL) //충돌한 인물이 있으면
					//> CSD-021104
				{
					if( lpEL && lpML->magicSerial!=0 )
					{
						g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own , attacked_Char->id, lpEL->x+(int)(lpEL->incx+.5), lpEL->y+(int)(lpEL->incy+.5));
					}
					
					InsertMagic(lpML->lpChar_Own,attacked_Char, lpML->next_Effect, 9, 0, 0, attacked_Char->x, attacked_Char->y);
					
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->EffectCount--;
					
					continue;
				}				
				else
				{
					lpItem=ReturnMagicCheckedItem(0, lpML, (int)((lpEL->x+lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2.)), (int)((lpEL->y+lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-(check_yl/2.)), (int)lpEL->height, check_xl, check_yl);
					if(lpItem!=NULL)
					{
						DeleteList(&(lpML->EffHead), lpEL);
						lpEL = lpML->EffHead;
						continue;
					}
				}
				
				if (lpEL->Next == NULL)
				{
					break;
				}
				else lpEL = lpEL->Next;
			}
			
			lpEL = lpML->EffHead;
			//가속처리하지 않음->가속처리시 while문 안에 넣을 것
			switch(lpEL->dir)
			{
			case 0:	//down
				lpML->y += (int)(lpML->spr_Speed);
				break;
			case 1:	//leftdwon
				lpML->x -= (int)(lpML->spr_Speed*3/4);
				lpML->y += (int)(lpML->spr_Speed*3/4);
				break;
			case 2:	//left
				lpML->x -= (int)(lpML->spr_Speed);
				break;
			case 3:	//leftup
				lpML->x -= (int)(lpML->spr_Speed*3/4);
				lpML->y -= (int)(lpML->spr_Speed*3/4);
				break;
			case 4:	//up
				lpML->y -= (int)(lpML->spr_Speed);
				break;
			case 5:	//rightup
				lpML->x += (int)(lpML->spr_Speed*3/4);
				lpML->y -= (int)(lpML->spr_Speed*3/4);
				break;
			case 6:	//right
				lpML->x += (int)(lpML->spr_Speed);
				break;
			case 7:	//rightdown
				lpML->x += (int)(lpML->spr_Speed*3/4);
				lpML->y += (int)(lpML->spr_Speed*3/4);
				break;
			}
			
			while(1)
			{
				if (lpEL == NULL) break;
				
				if( (inc%2 == 0) && ( lpML->EffectCount < lpML->object_Num) && (StartFlag == 1) && (lpML->flag==0))				
				{
					lpML->EffectCount++;
					InsertEffect(lpML);
					StartFlag = FALSE;
				}
				
				lpEL->trans_Level=lpML->trans_Level-((lpEL->FrameCount+1)*(lpML->trans_Level/(float)EffList[lpML->sprite_Num].MaxFrame));
				if(lpEL->trans_Level<=2) lpEL->trans_Level=2;
				
				DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
				
				inc++;
				if(inc>1000)	inc=0;
				
				if (lpEL->Next == NULL)
					break;
				else lpEL = lpEL->Next;			
			}
			break;
	}
	return lpML->Next;
}

//시전자 클릭한 방향 & 일정 속도로 날아가는 이팩트(미사용)
MAGICLIST* SampleMagic1_1(MAGICLIST* lpML)//, int Num, int motion_Num, int max_Object, int speed, int height, int type, int levlpEL, int end_Frame)		//스프라이트의 고유번호
{
	USEEFFECTLIST* lpEL;
	static int inc = 0, inc2 = 0;
	int		crash_Result=0;
	
	int		first_X=lpML->x;//lpML->lpChar_Own->x;
	int 	first_Y=lpML->y;//lpML->lpChar_Own->y;
	float	illusion_Trans=0;
	
	BOOL StartFlag = TRUE;
	static int direction=0;//lpCharacter->direction;
	static int incx = 0, incy = 0; //매직생성할때 쓰임.
	
	lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	
	//각도 알아내기(360 Degree)
	ChangeDirection((DIRECTION*)&lpML->dir, lpML->x, lpML->y, lpML->t_X, lpML->t_Y);
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:
		
		switch(lpML->dir)
		{
		case 0:
			lpML->y += 32;
			break;
		case 1:
			lpML->x -= 24;
			lpML->y += 24;
			break;
		case 2:
			lpML->x -= 32;
			break;
		case 3:
			lpML->x -= 24;
			lpML->y -= 24;
			break;
		case 4:
			lpML->y -= 32;
			break;
		case 5:
			lpML->x += 24;
			lpML->y -= 24;
			break;
		case 6:
			lpML->x += 32;
			break;
		case 7:
			lpML->x += 24;
			lpML->y += 24;
		}
		
		InsertEffect(lpML);
		lpML->EffHead->StartFlag = 1;
		
		//매직리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 1;
		break;
		
		case 1:		//2개 등록된 경우
			
			while(1)
			{
				if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
				{
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					
					MAGICLIST* buff;
					
					buff = lpML->Next;
					
					if( lpML->lpChar_Own->changeActionFlag == 1 ) 
					{
						lpML->lpChar_Own->changeActionFlag = 0;
					}
					
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					lpML->EffectCount = 0;
					DeleteList(&g_lpML, lpML);
					
					if (lpEL == NULL) return buff;
				}
				
				if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
				{
					if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
					{
						EndCastMagic(lpML->lpChar_Own);
						g_mgrBattle.ActAnimation(ACTION_NONE, lpML->lpChar_Own);
					}
				}
				//효과 이펙드가 위치하는 타일의 속성 체크하여 이펙트 소멸
				if (IsCollision(lpML->magicSerial,
					lpEL->x>>5,
					lpEL->y>>5,
					(lpEL->x + (int)lpEL->incx)>>5, 
					(lpEL->y + (int)lpEL->incy)>>5) == true)
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
				
				//발사 이펙트가 적 케릭터에 맞았는지 체크(이동하므로->그때그때의 적 좌표와 비교)
				//발사 이펙트가 적 케릭터에 맞았는지 체크
				attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (lpEL->x+(int)(lpEL->incx+.5)) - 16, (lpEL->y+(int)(lpEL->incy+.5)) - 32, (int)lpEL->height, 32, 48 );
				//< CSD-021104  
				//if (attacked_Char != NULL && !(attacked_Char->nCurrentAction == MON1_ATTACKED || attacked_Char->nCurrentAction == ACTION_ATTACKED)) //충돌한 인물이 있으면
				if (attacked_Char != NULL) //충돌한 인물이 있으면
					//> CSD-021104              
				{
					if(lpEL && lpML->magicSerial!=0 )
					{
						g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x+(int)(lpEL->incx+.5), lpEL->y+(int)(lpEL->incy+.5));
					}
					
					lpEL->trans_Level-=4;//lpML->trans_Level/EffList[lpEL->sprite_Num].MaxFrame;
					if(lpEL->trans_Level<=1)
					{
						DeleteList(&(lpML->EffHead), lpEL);
						lpEL = lpML->EffHead;
						crash_Result=0;
					}
					//crash_Result=0;
					if(lpML->flag==0)	lpML->flag=1;
					
					if(crash_Result==0)	continue;
				}
				
				// 이펙트의 좌표가 벋어날 경우 소멸 
				if ((lpEL->incx >= SCREEN_WIDTH) || (lpEL->incy >=SCREEN_HEIGHT))
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
				
				if (lpEL->Next == NULL)
				{
					break;
				}
				else lpEL = lpEL->Next;
			}
			
			lpEL = lpML->EffHead;
			
			while(1)
			{
				if(lpEL == NULL) break;
				
				int tx = first_X; //lpML->x;
				int ty = first_Y; //lpML->y;
				int tarx = lpML->t_X;
				int tary = lpML->t_Y;
				int inc1x, inc1y;
				
				if(abs(tx-tarx) > abs(ty-tary))
				{	
					inc1y = lpEL->speed * ( tary - ty ) / abs( tarx - tx );
					inc1x = ((tarx<tx) ? -lpEL->speed : lpEL->speed);
				}	
				else 
				{	
					inc1y = ((tary<ty ) ? -lpEL->speed : lpEL->speed);
					inc1x = lpEL->speed * ( tarx - tx ) / abs( tary - ty ) ;
				}	
				
				if(crash_Result==0)
				{
					//lpML->x += inc1x;
					lpEL->incx += inc1x;
					//lpML->y += inc1y;
					lpEL->incy += inc1y;
				}
				
				if((inc%1 == 0) && (lpML->EffectCount < lpML->object_Num) && (lpEL->StartFlag == 1) && lpML->flag==0)
				{
					lpML->EffectCount ++;
					InsertEffect(lpML);
					
					//lpML->EffectCount ++;
					//StartFlag = FALSE;
				}
				
				illusion_Trans=lpEL->trans_Level-(lpEL->trans_Level/lpML->object_Num*(lpEL->effect_Count));
				if(illusion_Trans<=0)	illusion_Trans=1;
				lpEL->trans_Level=illusion_Trans;
				
				float temp_incx = lpEL->incx;	
				lpEL->incx=	lpEL->oldincx;
				float temp_incy = lpEL->incy;
				lpEL->incy=	lpEL->oldincy;
				DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
				lpEL->incx=temp_incx;
				lpEL->incy=temp_incy;
				
				inc++;
				
				if (lpEL->Next == NULL)
					break;
				else lpEL = lpEL->Next;			
			}
			break;
	}
	return lpML->Next;	
}

//8방향으로 방사되는 이팩트(미사용 패턴)
MAGICLIST*	SampleMagic2(MAGICLIST*	lpML, int Num, int motion_Num, int speed, int height, int type, int levlpEL, int end_Frame)	//스프라이트 고유번호
{
	USEEFFECTLIST* lpEL;
	LPCHARACTER		lpEnemy	= g_CharacterList.lpFirst->lpNext;	
	
	BOOL	StartFlag = TRUE;
	int		crash_Result=0;
	
	lpEL=lpML->EffHead;
	
	switch(lpML->PCount)
	{
	case 0:
		lpML->x += 18;	//좌표를 주인공의 중심으로 정정
		//lpML->y += 15;
		//magic _List->x=(g_Map.x*TILE_SIZE)+q_CharacterList.lpFirst->offset.x;	//최초 생성 좌표_X
		//magic _List->y=(g_Map.y*TILE_SIZE)+q_CharacterList.lpFirst->offset.y;	//최초 생성 좌표_Y
		
		InsertEffect(lpML);	//매직고유번호,모션넘버,속도->0,방향->0
		
		//매직리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 1;
		break;
		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거 루틴
		{
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				
				g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
				lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				
				if (lpEL == NULL) return buff;	
			}
			
			//종료 조건 확인하려 링크드 리스트에서 제외
			//if (EffList[lpEL->EffectNum].EffData[lpEL->FrameCount].SprNo == -1)	//해당 모션의 마지막 프레임에 다다랐을 때 
			if(end_Frame!=-1)	//마지막 프레임 정의가 되어 있으면(특정 중간 프레임인 경우)
			{
				if(lpEL->FrameCount >= end_Frame)
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
			}
			else			//defult
			{
				if(lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame)
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
			}
			
			if (IsCollision(lpML->magicSerial,
				lpEL->x>>5,
				lpEL->y>>5,
				(lpEL->x + (int)lpEL->incx)>>5, 
				(lpEL->y + (int)lpEL->incy)>>5) == true)
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			//발사 이펙트가 적 케릭터에 맞았는지 체크
			crash_Result=0;
			while( lpEnemy )	
			{
				crash_Result=BoxAndBoxCrash(lpEnemy->x-16,lpEnemy->y-32,32,32,(lpEL->x+(int)(lpEL->incx+.5)) - 16,(lpEL->y+(int)(lpEL->incy+.5)) - 16,32,32);
				if(crash_Result)
				{
					g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpEnemy->id, lpEL->x+(int)(lpEL->incx+.5), lpEL->y+(int)(lpEL->incy+.5));
					break;
				}
				lpEnemy=lpEnemy->lpNext;
			}
			
			if(crash_Result)
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				crash_Result=0;
				continue;
			}
			else
				lpEnemy=g_CharacterList.lpFirst->lpNext;
			
			if (lpEL->Next == NULL)			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
			{
				break;
			}
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
		}
		
		lpEL = lpML->EffHead;
		
		while(1)	//실제 이펙트를 연결시켜주는 루틴
		{
			if (lpEL == NULL) break;
			
			if((lpML->EffectCount < 3) && (StartFlag==1) )
			{
				InsertEffect(lpML);	//매직고유번호,모션넘버,속도->0,방향->0
				
				lpML->EffectCount ++;
				lpEL->FrameCount++;
				StartFlag=FALSE;
			}
			
			switch(lpEL->dir)
			{
			case 0:
				break;
			case 1:
				lpML->x += speed;
				lpEL->incx += lpEL->speed;
				break;
			case 2:
				lpML->x += speed;
				lpML->y += speed;
				lpEL->incx += lpEL->speed;
				lpEL->incy += lpEL->speed;
				break;
			case 3:
				lpML->y += speed;
				lpEL->incy += lpEL->speed;
				break;
			case 4:
				lpML->x -= speed;
				lpML->y += speed;
				lpEL->incx -= lpEL->speed;
				lpEL->incy += lpEL->speed;
				break;
			case 5:
				lpML->x -= speed;
				lpEL->incx -= lpEL->speed;
				break;
			case 6:
				lpML->x -= speed;
				lpML->y -= speed;
				lpEL->incx -= lpEL->speed;
				lpEL->incy -= lpEL->speed;
				break;
			case 7:
				lpML->y -= speed;
				lpEL->incy -= lpEL->speed;
				break;
			case 8:
				lpML->x += speed;
				lpML->y -= speed;
				lpEL->incx += lpEL->speed;
				lpEL->incy -= lpEL->speed;
				break;
			}
			
			float temp_incx = lpEL->incx;	
			lpEL->incx=	lpEL->oldincx;
			float temp_incy = lpEL->incy;
			lpEL->incy=	lpEL->oldincy;
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
			lpEL->incx=temp_incx;
			lpEL->incy=temp_incy;
			
			if (lpEL->Next == NULL)
				break;
			else lpEL = lpEL->Next;			
		}
		break;
	}
	return lpML->Next;
}
//시전자의 이미지색값을 변형시키는 이팩트
MAGICLIST*	SampleMagic3(MAGICLIST*	lpML)
{
	USEEFFECTLIST*	lpEL;
	int				body_Part=1;
	bool			result=0;
	static int		inc_Num=1;
	int				r=0,g=0,b=0;
	
	//static	int		repeat_Count=0;
	
	lpEL=lpML->EffHead;
	
	//if(lpML->flag==0)
	body_Part=1;	//몸 전체에 색 변화토록
	
	switch(lpML->PCount)
	{
	case 0:
		//MAGICLIST와 연결된 실제 USEEFFECTLIST의 포인터를 연결/추가 시킴
		InsertEffect(lpML);	//매직고유번호,모션넘버,속도->0,방향->0
		//매직리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 1;
		
		//지속마법인 경우 지속 시작시간 셋팅
		if(lpML->end_Time!=0 && lpML->next_Effect==0 )	//지속마법이며 다음으로 이어지는 마법이 없는 경우
		{
			lpML->magic_Runtime		= g_ClientTime + (g_lpET[lpML->magicNum].end_Time*1000);//(g_lpMT[lpML->magicNum].continue_Time/6)*1000;
		}
		
		/*		//혼란/중독     001023 KHS 지금은 서버에서 처리한다. 
		if(lpML->lpChar_Own->excute_MagicNum==11 || lpML->lpChar_Own->excute_MagicNum==15)	
		{
		if(lpML->lpChar_Own == Hero )	//시전자가 히어로인 경우만
		SendCMD_MAGIC_RESULT( lpML->lpChar_Target->id,  lpML->lpChar_Own->excute_MagicNum, lpML->lpChar_Target->x, lpML->lpChar_Target->y );
		}
		*/
		
		break;
		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거 루틴
		{
			//////////////////////////////////////////////////////////////////////////////////////////////////
			
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				
				if (lpEL == NULL) return buff;	
			}
			
			if(lpML->next_Effect!=0)				//링크 이펙트가 있는 경우
			{
				if(lpML->link_Frame!=0 && lpEL->FrameCount==lpML->link_Frame)		
				{
					//CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_MAGIC_CASTING);
					InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->s_X,lpML->s_Y,
						lpML->t_X, lpML->t_Y);
				}
			}
			
			////////////////////////// 종료 조건 확인하여 링크드 리스트에서 제외 ////////////////////////////
			if( g_lpET[lpML->magicNum].end_Time > 0)		//특정 유지시간을 가지고 있는 경우
			{
				if( lpML->magic_Runtime <= g_ClientTime )
				{
					lpML->magic_Runtime		= 0;
					
					lpEL->FrameCount=0;
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
			}
			else if( g_lpET[lpML->magicNum].end_Time < 0)	//음수값인 경우 매직테이블에서 직접 시간테이블을 참조하여 판단	
			{
				int				cast_Time		= g_lpMT[lpML->magicSerial].cast_Time;			//캐스팅 시간(절대시간)	
				int				cast_Continue	= g_lpMT[lpML->magicSerial].cast_Continue;		//캐스팅 지속 시간(절대시간)
				int				continue_Time	= g_lpMT[lpML->magicSerial].continue_Time;		//지속시간(0->순간,영원히/1~절대시간)	
				//if(시동마법인 경우는 cast_Time참조)
				//else if(시동유지마법인 경우는 cast_Continue참조)
				//else (지속마법인 경우는 continue_Time참조)
			}
			
			else if(lpEL->FrameCount==lpML->object_Num)
			{
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			if (lpEL->Next == NULL)			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				break;
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
		}
		
		lpEL = lpML->EffHead;
		
		while(1)	//실제 이펙트를 연결시켜주는 루틴
		{
			if (lpEL == NULL) break;
			
			lpEL->trans_Level+=inc_Num;
			
			if(lpEL->trans_Level>5 || lpEL->trans_Level<0)
			{
				inc_Num*=-1;
				if(lpEL->trans_Level>=5)	lpEL->trans_Level=5;
				if(lpEL->trans_Level<=0)	lpEL->trans_Level=0;
			}
			
			if(lpML->magicSubNum!=9)		//시전자 색 변형
			{
				if(lpML->trans_Type!=6)	//색을 변형시키지 않고 밝기만 깜박이게 
					DrawEffect1(lpEL, lpML, body_Part, 0);
				else							// trans_Type이 6인 경우->RGB blend
				{
					r=lpML->trans_Level/1000000;
					g=(lpML->trans_Level%1000000)/1000;
					b=lpML->trans_Level%1000;
					DrawEffect2(lpEL, lpML, body_Part, ReturnBlendRGB( r, g, b ), 0);
				}
			}
			else								//대상자 색 변형
			{
				if(lpML->trans_Type!=6)
					DrawEffect1(lpEL, lpML, body_Part, 1);
				else	// trans_Type이 6인 경우->RGB blend
				{
					r=lpML->trans_Level/1000000;
					g=(lpML->trans_Level%1000000)/1000;
					b=lpML->trans_Level%1000;
					DrawEffect2(lpEL, lpML, body_Part, ReturnBlendRGB( r, g, b ), 1);
				}
			}
			
			lpEL->FrameCount++;
			
			if (lpEL->Next == NULL)
				break;
			else lpEL = lpEL->Next;			
		}
		break;
	}
	return lpML->Next;
}
//주인공 인물 위치에 단일 프레임 겹쳐 찍어 주기
MAGICLIST*	SampleMagic4(MAGICLIST*	lpML)
{
	USEEFFECTLIST*  lpEL;
	static int	evil_Eye_Delay=12;
	int			illusion_Trans=0;
	int			terminateFrame=0;
	int			free_On=0;
	DWORD		check_Time=0;
	//char		buf[30];
	
	int			crash_Result=0;
	bool		target_Type=0;			//시전자에게 찍을까?(0) 대상체에게 찍을까?(1)
	bool		reset_Map=0;
	int			move_X=0,move_Y=0;
	int			center_TileX = lpML->tarx/TILE_SIZE;
	int			center_TileY = lpML->tary/TILE_SIZE-1;
	
	lpEL=lpML->EffHead;
	
	if(lpML->magicSubNum==9)
		target_Type=1;	//만약 서브 타입이 9면 적에게 효과소스 출력
	
	switch(lpML->PCount)
	{
	case 0:
		{
			InsertEffect(lpML);
			//효과리스트에 마법 하나 등록된 사실 셋팅
			lpML->PCount = 1;
			//지속마법인 경우 지속 시작시간 셋팅
			if (lpML->magic_Runtime == 0)
			{	//< CSD-TW-030606
				if (lpML->end_Time > 0 && lpML->next_Effect == 0)
				{	// magic 테이블의 end_Time 필드가 0보다 큰 경우
					lpML->magic_Runtime= g_ClientTime + 3000;
				}
				else if (lpML->end_Time < 0 && lpML->next_Effect == 0)
				{	// magic 테이블의 end_Time 필드가 -1인 경우
					//lpML->magic_Runtime	= g_ClientTime + g_lpMT[lpML->magicSerial].continue_Time/2*1000;
					lpML->magic_Runtime= g_ClientTime + 3000;
				}
			}	//> CSD-TW-030606
			
			if (lpML->magicSerial == FLAME_ARROW)
			{	//< CSD-031020
				g_mgrBattle.ActCasting(lpML->magicSerial, lpML->lpChar_Own);
			}	//> CSD-031020
			
			break;
		}
	case 1:
		
		switch (lpML->magicSerial)
		{
        case GREAT_THUNDER_BOLT:
        case HOLY_RAGE:
			{
				if( lpML->lpChar_Own->id < 10000 )
				{	
					ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
					lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
					//CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->nCurrentAction);
				}	
				
				break;
			}
		}
		
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거
		{
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				lpML->EffectCount = 0;
				
				switch (lpML->magicSerial)
				{   //< CSD-031020
				case FLAME_PILLAR:
				case FLAME_POUR:
				case FLAME_ARROW: 
				case EARTH_EXTREME:
				case MULTIPLE_FIRE:
				case CONVERTING_ARMOR:
				case FIRE_EXTREME:
				case ICING_BLAST:
				case SHARK_MISSILE:
				case ICE_EXTREME:
				case DARK_EXTREME:
				case ICE_BREATH2:	// 030415 kyo
					{
						g_mgrBattle.Effect(lpML);
						break;
					}
				}   //> CSD-031020
				
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림
				
				//if(lpML!=NULL || lpML->EffectCount!=NULL)
				//	lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				
				if (lpEL == NULL)
				{
					return buff;
				}
			}
			////////////////////////////////// 0112 이규훈 //////////////////////////////////////
			if( lpML->lpChar_Own == NULL || lpML->lpChar_Target == NULL )
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				
				//if(lpML!=NULL || lpML->EffectCount!=NULL)
				//	lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				
				if (lpEL == NULL)
				{
					return buff;
				}
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					EndCastMagic(lpML->lpChar_Own);
				}
			}
			
			/////////////////////////// 종료 조건 확인하여 링크드 리스트에서 제외 ////////////////////////////
			if (lpML->flag==0 && 
				(DayLightControl < 31) && 
				g_lpET[lpML->magicNum].end_Time!=-1 && 
				IsHarmfulMagic(lpML->magicSerial)) //소멸되는 광원 초기화 시켜주기 
			{
				//terminateFrame=(EffList[lpEL->sprite_Num].MaxFrame)/LIGHT_TERMINATE;
				terminateFrame = 20;
				
				if(terminateFrame<1)	terminateFrame=1;
				
				if(g_lpET[lpML->magicNum].light/100 == 1 )
					InsertLightOut( lpML->tarx, (int)(lpML->tary-lpEL->height), g_lpET[lpML->magicNum].light%100, terminateFrame, g_lpET[lpML->magicNum].light/100 );
				else
					InsertLightOut( lpML->tarx, lpML->tary, g_lpET[lpML->magicNum].light%100, terminateFrame, g_lpET[lpML->magicNum].light/100 );
				lpML->flag=1;
			}
			
			if( g_lpET[lpML->magicNum].end_Time )		//특정 유지시간을 가지고 있는 경우
			{
				if( lpML->magic_Runtime <= g_ClientTime )
				{
					if(lpML->magicNum == 420)
					{
						POINT	tab_wizard[44]={6,0, 6,-1, 5,-1, 5,-2, 5,-3, 4,-3, 3,-3, 3,-4, 2,-4, 1,-4, 1,-5, 0,-5, -1,-5, -1,-4, -2,-4, -3,-4, -3,-3,
							-4,-3, -5,-3, -5,-2, -5,-1, -6,-1, -6,0, -6,1, -5,1, -5,2, -5,3, -4,3, -3,3, -3,4, -2,4, -1,4, -1,5, 0,5, 1,5,
							1,4, 2,4, 3,4, 3,3, 4,3, 5,3, 5,2, 5,1, 6,1};
						//Dont Tile 처리
						for(int i=0;i<44;i++)
						{
							if(TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].empty__6 == 1)
							{
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_dont = 0;
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_light= 0;
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].empty__6	= 0;
							}
						}
					}
					if(lpML->magicNum == 333)
					{
						POINT	tab_priest[36]={5,0, 5,-1, 4,-1, 4,-2, 3,-2, 3,-3, 2,-3, 1,-3, 1,-4, 0,-4, -1,-4, -1,-3, -2,-3, -3,-3, -3,-2, -4,-2, -4,-1,
							-5,-1, -5,0, -5,1, -4,1, -4,2, -3,2, -3,3, -2,3, -1,3, -1,4, 0,4, 1,4, 1,3, 2,3, 3,3, 3,2, 4,2, 4,1, 5,1 };
						//Dont Tile 처리
						for(int i=0;i<36;i++)
						{
							if(TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].empty__6 == 1)
							{
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_dont = 0;
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_light= 0;
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].empty__6	= 0;
							}
						}
					}
					
					lpML->magic_Runtime	= 0;
					
					lpEL->FrameCount=0;
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					
					continue;
				}
			}
			else if( ( lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame ) || ( lpML->magicSubNum==4 && lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame/8 ))
			{
				if(lpML->next_Effect==0 || g_lpET[lpML->magicSerial].impect_Effect_Num==0)
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			if( lpML->lpChar_Own->condition == CON_DEATH || lpML->lpChar_Own->hp <= 0 )		//시전자가 죽어 있는 경우
			{// 1012 kkh 약간수정..	
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			else if( (lpML->lpChar_Target->condition == CON_DEATH || lpML->lpChar_Target->hp <= 0) && !(lpML->magicSerial == 7 || lpML->magicSerial ==164 ) )		//대상자가 죽는 경우(소생마법 제외)
			{	
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////				
			
			if (lpEL->Next == NULL /*& lpEL->total_Frame != 0*/ )		//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				break;
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
			}
			
			lpEL = lpML->EffHead;
			
			while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
			{
				//동시 출력하는 이팩트의 갯수가 복수일 때 
				if(lpML->EffectCount < lpML->object_Num-1)
				{
					InsertEffect(lpML);
					lpML->EffectCount ++;
				}
				
				//const int runMagicNum = g_mgrBattle.Return(lpML->magicNum, lpML->lpChar_Own);
				const int runMagicNum = lpML->magicSerial; 	// 030415 kyo
				
				if( lpEL->FrameCount >= 42 )
				{
					if(lpML->magicNum == 420)
					{
						POINT	tab_wizard[44]={6,0, 6,-1, 5,-1, 5,-2, 5,-3, 4,-3, 3,-3, 3,-4, 2,-4, 1,-4, 1,-5, 0,-5, -1,-5, -1,-4, -2,-4, -3,-4, -3,-3,
							-4,-3, -5,-3, -5,-2, -5,-1, -6,-1, -6,0, -6,1, -5,1, -5,2, -5,3, -4,3, -3,3, -3,4, -2,4, -1,4, -1,5, 0,5, 1,5,
							1,4, 2,4, 3,4, 3,3, 4,3, 5,3, 5,2, 5,1, 6,1};
						//Dont Tile 처리
						for(int i=0;i<44;i++)
						{
							if( TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_dont == 0 )
							{
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_dont	= 1;
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_light = 1;
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].empty__6	= 1;
							}
						}
					}
					if(lpML->magicNum == 333)
					{
						POINT	tab_priest[36]={5,0, 5,-1, 4,-1, 4,-2, 3,-2, 3,-3, 2,-3, 1,-3, 1,-4, 0,-4, -1,-4, -1,-3, -2,-3, -3,-3, -3,-2, -4,-2, -4,-1,
							-5,-1, -5,0, -5,1, -4,1, -4,2, -3,2, -3,3, -2,3, -1,3, -1,4, 0,4, 1,4, 1,3, 2,3, 3,3, 3,2, 4,2, 4,1, 5,1 };
						//Dont Tile 처리
						for(int i=0;i<36;i++)
						{
							if( TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_dont == 0 )
							{
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_dont	= 1;
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_light = 1;
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].empty__6	= 1;
							}
						}
					}
				}
				// 마법 결과에 대한 답변 요청 (화염벽/얼음벽마법은 제외)
				if(lpML->next_Effect && lpML->link_Frame==lpEL->FrameCount && lpML->magicSerial && lpML->magicSerial!=34 && lpML->magicSerial!=42 && lpML->magicSerial!=45 && lpML->magicSerial!=41 && lpML->magicSerial != 162 && lpML->magicSerial != 519 && !lpML->failed)
				{
					if (g_lpMT[lpML->magicSerial].magic_Type/10 != 5 && 
						(lpML->magicSerial == runMagicNum || runMagicNum == 215 || runMagicNum == 322))	//비공격마법이면서 시전의 링크 프레임인 경우
					{
						if (lpML->next_Flag == 0 && lpML->magicSerial < LIGHTNING_BOOM)
						{
							const int nTemp = g_lpMT[lpML->magicSerial].avail_Type;
							if (g_lpMT[lpML->magicSerial].avail_Type < 2 || 
								(g_lpMT[lpML->magicSerial].avail_Type>=2 && IsHarmfulMagic(lpML->magicSerial)))
							{
								switch (lpML->magicSerial)
								{ //< CSD-021015
								case PERFECT_PROTECT:
								case AUTHORITY_LETHY:	
								case PRAY:		// 030415 kyo
								case AUTHORITY_OF_ATHYAS: 	// 030415 kyo
								case RESTORE_AUTHORITY: // 030523 kyo
									{
										break;
									}
								case AUTHORITY_SIMUNIAN:
								case MIRACLE_RECOVER:
								case HOLY_CURE:	// 030415 kyo
									{
										::InsertMagic(lpML->lpChar_Own, lpML->lpChar_Own,lpML->next_Effect, 0, 0, 0, lpML->tarx, lpML->tary);
										g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Own->id, lpML->tarx, lpML->tary); 
										break;
									}
								case GREAT_SHIELD:
								case GREAT_PRAY:
									{
										g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Own->id, lpML->tarx, lpML->tary); 
										break;
									}
								default:
									{
										g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Target->id, lpML->tarx, lpML->tary); 
										break;
									}
								} //> CSD-021015
							}
							else
							{ 
								if(lpML->magicSerial )	//지역 공격 마법의 경우
								{
									//폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
									int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
									int check_yl = check_xl*3/4;
									attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
								}
							}
							
							lpML->next_Flag=1;
						}
						else if (lpML->next_Flag == 1)
						{
							switch (lpML->magicSerial)
							{ //< CSD-021015
							case PERFECT_PROTECT:
							case AUTHORITY_LETHY:
							case PRAY:	// 030415 kyo
							case AUTHORITY_OF_ATHYAS:	// 030415 kyo
							case RESTORE_AUTHORITY: // 030523 kyo
								{
									g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Target->id, lpML->tarx, lpML->tary);
									break;
								}
							} //> CSD-021015
							
							lpML->next_Flag=2;
						}
					}
					else
					{
						switch (lpML->magicSerial)
						{
						case TURN_UNDEAD:
							{
								g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Target->id, lpML->tarx, lpML->tary);
								break;
							}
						}
					}
				}
				
				if (lpML->link_Frame == lpEL->FrameCount && !lpML->failed && (lpML->lpChar_Own->nActiveCombat.GetDecrypted() || lpML->lpChar_Own->nPassiveCombat || lpML->lpChar_Own->nRecoveryCombat))	// 030415 kyo
				{
					switch (lpML->magicSerial)
					{
					case LIGHTNING_BOOM:
					case THUNDER_BLOW:
					case LIGHTNING_SHOCK:
					case THUNDER_STRIKE:
					case GROUND_ATTACK:
					case BLOOD_WILL:
					case SWORD_N_ROSES:
					case HORN_OF_ICEBERG:
					case DOUBLE_ATTACK:
					case CHERROY_SHADE:
					case DARK_BLADE:
					case CRITICAL_HIDING:
					//case LIGHTNING_EXTREME: // 030328 kyo
					case EARTH_EXTREME:
					case DARK_EXTREME:
					case WIND_EXTREME:
						{
							g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Target->id, lpML->tarx, lpML->tary);
							DeleteList(&(lpML->EffHead), lpEL);
							return NULL;
						}
					case FLAME_ARROW: // CSD-031020
					case MULTIPLE_FIRE:
					case FLEET_MISSILE:
					case CONVERTING_ARMOR:
					case FIRE_EXTREME:
					case ICING_BLAST:
					case SHARK_MISSILE:
					case ICE_EXTREME:
						{
							DeleteList(&(lpML->EffHead), lpEL);
							return NULL;
						}
					case LIGHTNING_SHIELD:
					case STONE_ARMOR:
					case TYBERN_GIFT:
				
					case SIMUNIAN_BOSOM:
					case ICE_SHIELD:
					case CHARGING:					
					case GUST:
						{
							InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, lpML->next_Effect, lpML->magicSubNum, 0, 0, lpML->tarx, lpML->tary);
							g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Target->id, lpML->tarx, lpML->tary);
							DeleteList(&(lpML->EffHead), lpEL);
							return NULL;
						}
					case LIGHTNING_EXTREME:
					case TWISTER:
						{	//< CSD-TW-030606
							InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, lpML->next_Effect, lpML->magicSubNum, 0, 0, lpML->tarx, lpML->tary);							
							DeleteList(&(lpML->EffHead), lpEL);
							return NULL;
						}	//> CSD-TW-030606
					}
				}
				
				if( lpML->link_Frame>0 && lpML->link_Frame==lpEL->FrameCount && !lpML->failed)
				{ 
					if (IsFuntionItemEffect(lpML->magicNum))
					{	//< CSD-030422
						g_mgrBattle.Effect(lpML);
					}	//> CSD-030422
					else if ((lpML->magicSerial == FREEZE_5TILE_RADIUS && (lpML->magicNum!=lpML->magicSerial)) ||
							 (lpML->magicSerial == FREEZE_9TILE_RADIUS && (lpML->magicNum!=lpML->magicSerial)) ||
							 (lpML->magicSerial == FREEZING && (lpML->magicNum!=lpML->magicSerial)) ||
							 (lpML->magicSerial == HOLY_LIGHT && (lpML->magicNum-150!=lpML->magicSerial)) ||
							 (lpML->magicSerial == AUTHORITY_CHARISNUMEN && (lpML->magicNum-150!=lpML->magicSerial)))
					{ //< CSD-021015
						//폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
						int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
						int check_yl = check_xl*3/4;
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
					} //> CSD-021015
					else if (lpML->magicSerial == MIRACLE_RECOVER)
					{ //< CSD-020620
						int	check_xl = g_lpMT[156].avail_Range*TILE_SIZE;	//반경 타일
						int check_yl = check_xl*3/4;
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
					} //> CSD-020620
					else if (lpML->magicSerial == GREAT_SHIELD || lpML->magicSerial == AUTHORITY_SIMUNIAN)
					{ //< CSD-020620
						int	check_xl = 3*TILE_SIZE;	//반경 타일
						int check_yl = check_xl*3/4;
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
					} //> CSD-020620
					else if (lpML->magicSerial == GREAT_PRAY)
					{ //< CSD-020620
						int	check_xl = 3*TILE_SIZE;	//반경 타일
						int check_yl = check_xl*3/4;
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
						
						if (lpML->lpChar_Own == lpML->lpChar_Target)
						{
							g_mgrBattle.Effect(lpML);
						}
					} //> CSD-020620
					else if(lpML->magicSerial == SNOWSTORM && lpML->magicNum!=lpML->magicSerial)
					{ 	// 030415 kyo//두바꾸째 돌때 뿌려주는 역확
						int	check_xl = 5*TILE_SIZE;	//반경 5타일
						int check_yl = check_xl*3/4;
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->tarx - check_xl, lpML->tary - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 2);
					}
					else if(lpML->magicSerial == HOLY_CURE && lpML->magicNum!=lpML->magicSerial)
					{ 	// 030415 kyo
						int	check_xl = 5*TILE_SIZE;	//반경 5타일
						int check_yl = check_xl*3/4;
						//attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0); //처음 찍었을때 그 반경
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->lpChar_Own->x - check_xl, lpML->lpChar_Own->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
						//attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->tarx - check_xl, lpML->tary - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 2);
					}
					else if (lpML->magicSerial == PHASING)	//장소이동 마법의 경우
					{
#ifdef _DEBUG // finito 060507
						if(SysInfo.notconectserver)
						{
							do	{
								move_X=rand()%19-9;
								move_Y=rand()%11-5;
							}	while( abs(move_X)<3 || abs(move_Y)<2);
							
							lpML->lpChar_Own->x += move_X*32;
							lpML->lpChar_Own->y += move_Y*32;
							lpML->t_X			 = lpML->lpChar_Own->x;
							lpML->t_Y			 = lpML->lpChar_Own->y;	
							
							lpML->lpChar_Own->pathcount=0;
							g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
							
							if(lpML->magicSubNum==8)	InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,9				,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);
							else						InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);
						}
#endif
					}
					else if (lpML->magicSerial == CURE_DISEASE)
					{	//< CSD-TW-030606
						g_mgrBattle.Effect(lpML);
					}	//> CSD-TW-030606
					else	//다음 연결 효과와 연결
					{
						//다음 연결 패턴이 4인 경우
						if( g_lpET[lpML->next_Effect].pattern_Num == 4 || g_lpET[lpML->next_Effect].pattern_Num == 3 ) //|| g_lpET[lpML->next_Effect].pattern_Num == 12)
						{
							//시전자의 방향과 액션 형태를 조정
							if(lpML->lpChar_Own == lpML->lpChar_Target)
							{
								lpML->lpChar_Own->changeActionFlag=0;
								g_mgrBattle.ActAnimation(ACTION_NONE, lpML->lpChar_Own);
							}
							else
							{
								lpML->lpChar_Own->changeActionFlag=0;
								g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
							}
						}
						
						///////////////////////// 0715 lkh 추가 /////////////////////////
						// 폭죽과 같은 여러개의 이펙트 효과를 랜덤하게 조합해서 찍어야하는 경우
						else if(lpML->magicSubNum/100 == 1)			//100단위수가 1인 경우 단자리 수자 만큼 이후번호의 이펙트를 10단위의 숫자 만큼 연결 출력
						{
							int	link_Count = (lpML->magicSubNum%100)/10;
							int later_Count = (lpML->magicSubNum%10);
							lpML->magicSubNum = lpML->magicSubNum - 10;
							
							int	real_MagicNum = 375 + rand()%later_Count;
							
							if(link_Count && later_Count)
								InsertMagic( lpML->lpChar_Own, lpML->lpChar_Own, real_MagicNum, lpML->magicSubNum, lpML->s_X, lpML->s_Y, 0, 0);
						}							
						else		//4패턴이 아닌 경우
						{ 
							lpML->lpChar_Own->changeActionFlag=0;
							g_mgrBattle.ActCasting(lpML->magicSerial, lpML->lpChar_Own);
						} 

						g_mgrBattle.Effect(lpML);
					}
				}
				
				if( lpML->link_Frame<0 )			//음수인 경우 해당 조건 만족할 때 다음 연결 효과 링크
				{	
					evil_Eye_Delay--;
					if(evil_Eye_Delay<=0)
					{
						//성직자 5계열 [악에 대한 보호] 마법의 경우 대상체 효과 처리(detect 범위 -> 반경 5타일내 적이 있는 경우)
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, (lpML->x) - 160, (lpML->y) - 128, (int)lpEL->height, 320, 256, 0);
						evil_Eye_Delay=10;
					}
				}
				
				if(lpML->trans_Type/10!=0 && lpML->trans_Type%10==8)		//점점 밝게 생성 / 마지막 어둡게
				{
					if(lpEL->FrameCount<EffList[lpEL->sprite_Num].MaxFrame-5)
					{
						lpEL->trans_Level=(lpML->trans_Level/(float)EffList[lpEL->sprite_Num].MaxFrame)*lpEL->FrameCount;
						if(lpEL->trans_Level > lpML->trans_Level)	lpEL->trans_Level=(float)lpML->trans_Level;
					}
					else
					{
						lpEL->trans_Level-=(float)(lpML->trans_Level/4.);
						if(lpEL->trans_Level < 1) lpEL->trans_Level = 1;
					}
				}
				
				// 마지막 4프레임은 alpha off
				else if( (lpML->trans_Type/10!=0 && lpML->trans_Type%10==9) && lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame-4)	
				{
					lpEL->trans_Level-=(lpML->trans_Level/4);//-((lpML->trans_Level/EffList[lpEL->sprite_Num].MaxFrame)*(lpEL->FrameCount-1));
					if(lpEL->trans_Level<=1)	lpEL->trans_Level=1;
				}
				
				else if(lpML->trans_Type%10==7)	//바닥이 얼어붙는 효과
				{
					if(lpEL->FrameCount<=EffList[lpML->sprite_Num].MaxFrame/2)
					{
						lpEL->trans_Level=(float)lpEL->FrameCount/2;
						if(lpEL->trans_Level>=12)	lpEL->trans_Level=12;
					}
					else
					{
						lpEL->trans_Level-=(lpEL->trans_Level/(EffList[lpML->sprite_Num].MaxFrame/2));//(float)(EffList[lpML->sprite_Num].MaxFrame-lpEL->FrameCount);
						if(lpEL->trans_Level<0) lpEL->trans_Level=0;
					}
				}
				else if(g_lpET[lpML->magicNum].end_Time>0)			//Effect Table에 특정 유지시간이 주어진 유지 마법의 경우 색깔값 지속적 떨어트림
				{
					lpEL->trans_Level = (float) ((float)lpML->trans_Level * ( (lpML->magic_Runtime - g_ClientTime) / (g_lpET[lpML->magicNum].end_Time*1000.) ) );
					if(lpEL->trans_Level<2)	lpEL->trans_Level=2;
				}
				
				else if(g_lpET[lpML->magicNum].end_Time<0)			//Magic Table에 정의된 유지 시간동안 지속되는 마법의 색깔값 변환
				{ 
					if(lpML->magic_Runtime > g_ClientTime + (g_lpMT[lpML->magicSerial].continue_Time*1000))
						lpEL->trans_Level = (float)lpML->trans_Level;
					else
						lpEL->trans_Level = (float) ((float)lpML->trans_Level * ( (lpML->magic_Runtime - g_ClientTime) / (g_lpMT[lpML->magicSerial].continue_Time*1000.) ) );
					if(lpEL->trans_Level<2)	lpEL->trans_Level=2;
				}
				////////////////// 케릭터의 높이값에 따라 가감치 더해주기 //////////////////
				//int  change_Height=(lpML->lpChar_Own->sp->yl)-(lpML->lpChar_Target->sp->yl);
				//lpEL->y=lpML->y+change_Height;
				//lpEL->t_X=lpML->t_Y+change_Height;
				////////////////////////////////////////////////////////////////////////////
				if(g_lpET[lpML->magicNum].sound_loop)
					EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
				
				if(lpML->flag==0 || g_lpET[lpML->magicNum].end_Time==-1)	//광원 소멸 함수 호출되지 않은 경우
				{
					if(!g_lpET[lpML->magicNum].end_Time)
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, target_Type);
					else
					{
						int	light_Level = g_lpET[lpML->magicNum].light/100*100+(int)( g_lpET[lpML->magicNum].light%100*((lpML->magic_Runtime - g_ClientTime) / (g_lpMT[lpML->magicSerial].continue_Time*1000.))+.5);
						if(light_Level > g_lpET[lpML->magicNum].light)		light_Level = g_lpET[lpML->magicNum].light;
						DrawEffect(lpEL, lpML, light_Level, target_Type);
					}
				}
				else
					DrawEffect(lpEL, lpML, 0, target_Type);
				
				lpEL = lpEL->Next;			
			}
			break;
	}
	return lpML->Next;
}

//원형 불(얼음) 생성 이팩트 - 지옥의 불과 원형 얼음 공격마법의 경우
MAGICLIST*	SampleMagic5(MAGICLIST*	lpML)//, int Num, int motion_Num, int speed, int height, int way, int type, int levlpEL, int end_Frame)	//스프라이트 고유번호
{
	int i=0;
	int x=0,y=0;
	int first_Inc=0;
	int	crash_Result=0;
	int bufx=0,bufy=0;
	static unsigned int inc=0;
	
	x=lpML->x;
	y=lpML->y;
	
	USEEFFECTLIST* lpEL;
	//LPCHARACTER		attacked_Char=NULL;
	
	lpEL=lpML->EffHead;
	
	first_Inc=60;
	
	switch(lpML->PCount)
	{
	case 0:
		ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
		lpML->lpChar_Own->todir	= lpML->lpChar_Own->direction;
		g_mgrBattle.ActAnimation(lpML->lpChar_Own->nCurrentAction, lpML->lpChar_Own);
		
		for(i=0;i<360;i+=first_Inc)
		{
			bufx=lpML->x;
			bufy=lpML->y;
			lpML->x=x+(int)(cos(i*M_PI/180.)*(70+lpML->spr_Speed*lpML->EffectCount));
			lpML->y=y+(int)(sin(i*M_PI/180.)*(45+lpML->spr_Speed*lpML->EffectCount));
			InsertEffect(lpML);	//매직고유번호,모션넘버,속도->0,방향->0
			lpML->x=bufx;
			lpML->y=bufy;
		}
		
		//매직리스트에 마법 등록된 사실 셋팅
		lpML->PCount = 1;
		break;
		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거 루틴
		{
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				//arDoAni( lpML->lpChar_Own , lpML->lpChar_Own->direction , ACTION_BATTLE_NONE );
				MAGICLIST* buff;
				
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				
				g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
				lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				
				if (lpEL == NULL)
				{
					inc=0;
					return buff;	
				}
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					EndCastMagic(lpML->lpChar_Own);
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
				}
			}
			
			//마지막 2/3프레임에서 지역 충돌체크 루틴 server로 Call
			if (lpEL->FrameCount == (int)(EffList[lpEL->sprite_Num].MaxFrame*2/3) && lpML->flag==0)
			{	//< CSD-031024
				lpML->flag=1;
				
				if (lpML->magicNum == ITEM_EFFECT_FIRE || lpML->magicNum == ITEM_EFFECT_ICE)
				{
					const int check_xl = 5*TILE_SIZE;			//반경 타일
					const int check_yl = check_xl*3/4;
					// 폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
					attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
				}
				else if (lpML->magicSerial != 0)
				{	// 지역 공격 마법의 경우
					const int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
					const int check_yl = check_xl*3/4;
					// 폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
					attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
				}
			}	//> CSD-031024
			
			//스프라이트의 마지막 프레임까지 찍었을 때
			if(lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame)
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			//타일 속성 체크하여 소스 소멸
			if (IsCollision(lpML->magicSerial,
				lpEL->x>>5,
				lpEL->y>>5,
				(lpEL->x + (int)lpEL->incx)>>5, 
				(lpEL->y + (int)lpEL->incy)>>5) == true)
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
#ifdef _DEBUG // finito 060507			
			if(SysInfo.notconectserver)
			{
				int check_xl = (int)(Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl*.75);		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
				int check_yl = (int)(Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl*.75);
				
				//발사 이펙트가 적 케릭터에 맞았는지 체크(지역/복수 공격 패턴인 경우의 충돌체크 루틴)
				attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (int)((lpEL->x+lpEL->incx)-(check_xl/2.)), (lpEL->y+(int)(lpEL->incy+.5))-(check_yl), (int)lpEL->height, check_xl, check_yl, 0);
				//< CSD-021104		
				//if(attacked_Char!=NULL && !(attacked_Char->nCurrentAction == MON1_ATTACKED || attacked_Char->nCurrentAction == ACTION_ATTACKED) )
				if (attacked_Char != NULL)
					//> CSD-021104
				{
					InsertMagic(lpML->lpChar_Own, attacked_Char, lpML->next_Effect, 9, 0, 0, attacked_Char->x, attacked_Char->y);
					crash_Result=0;
					//continue;
				}
			}
#endif			
			if (lpEL->Next == NULL)			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
			{
				break;
			}
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
		}
		
		lpEL = lpML->EffHead;
		
		while(1)	//실제 이펙트를 연결시켜주는 루틴
		{
			if (lpEL == NULL) break;
			
			if(lpML->EffectCount < lpML->object_Num)// && (lpEL->FrameCount==5)//inc%80==40)
			{
				if((lpML->EffectCount==0 && lpEL->FrameCount==6) || (lpML->EffectCount==1 && inc==110))
				{
					//InsertEffect(lpML, lpEL->EffectNum, lpEL->MotionNum, lpEL->speed, lpEL->dir, lpEL->tarx, lpEL->tary);
					
					for(i=0;i<360;i+=first_Inc-((lpML->EffectCount+1)*(26-(lpML->EffectCount*8))))
					{
						bufx=lpML->x;
						bufy=lpML->y;
						lpML->x=x+(int)(cos(i*M_PI/180.)*(70+lpML->spr_Speed*(lpML->EffectCount+1)));
						lpML->y=y+(int)(sin(i*M_PI/180.)*(45+lpML->spr_Speed*(lpML->EffectCount+1)));
						InsertEffect(lpML);	//매직고유번호,모션넘버,속도->0,방향->0
						lpML->x=bufx;
						lpML->y=bufy;
					}
					
					lpML->EffectCount ++;
				}
			}
			
			// 0~4 프레임까지 점점 밝아 지면서 생성
			if(lpEL->FrameCount<=4)
			{
				lpEL->trans_Level =(float)(((lpML->trans_Level / 5.)*lpEL->FrameCount)+2);
				if(lpEL->trans_Level>=lpML->trans_Level)	lpEL->trans_Level=(float)(lpML->trans_Level);
			}
			// 마지막 4프레임전부터 어두어지면 사라짐
			else if(lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame-4)
			{
				lpEL->trans_Level -= (float)(lpML->trans_Level / 4.);
				if(lpEL->trans_Level == 0 )	lpEL->trans_Level=1;
			}
			else
				lpEL->trans_Level = (float)lpML->trans_Level;
			
			//else
			//{
			//	lpEL->trans_Level-=(lpEL->FrameCount-(EffList[lpEL->sprite_Num].MaxFrame/3))*(lpML->trans_Level/(EffList[lpEL->sprite_Num].MaxFrame/3));
			//	if(lpEL->trans_Level<=0)	lpEL->trans_Level=1;
			//}
			
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
			inc++;
			
			if (lpEL->Next == NULL)
				break;
			
			else lpEL = lpEL->Next;			
		}
		break;
	}
	return lpML->Next;
}

////////////////////////// 0320 lkh Nova 패턴 ///////////////////////////////
MAGICLIST*	Nova(MAGICLIST*	lpML)
{
	int i=0;
	int first_Inc=30;	//12 Piece
	int	crash_Result=0;
	int bufx=0,bufy=0;
	static int plus=0;
	
	USEEFFECTLIST* lpEL;
	
	lpEL=lpML->EffHead;
	
	switch(lpML->PCount)
	{
	case 0:
		{
			if( lpML->lpChar_Own->id < 10000 )
			{
				ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
				lpML->lpChar_Own->todir	= lpML->lpChar_Own->direction;
				g_mgrBattle.ActAnimation(lpML->lpChar_Own->nCurrentAction, lpML->lpChar_Own);
			}
			
			for(i=0;i<360;i+=first_Inc)		//12개의 구생성
			{
				bufx=lpML->x;
				bufy=lpML->y;
				lpML->x += (int)(cos(i*M_PI/180.)*48);
				lpML->y += (int)(sin(i*M_PI/180.)*36);
				InsertEffect(lpML);	//매직고유번호,모션넘버,속도->0,방향->0
				//lpEL->oldincx	= (float)i;
				lpML->x=bufx;
				lpML->y=bufy;
				//lpML->EffectCount++;
			}
			
			//매직리스트에 마법 등록된 사실 셋팅
			lpML->PCount = 1;
			break;
		}
		
	case 1:
		{
			while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거 루틴
			{
				if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
				{
					MAGICLIST* buff;
					
					buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
					
					if( lpML->lpChar_Own->changeActionFlag == 1 )
					{
						lpML->lpChar_Own->changeActionFlag = 0;
					}
					
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
					DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림
					
					if (lpEL == NULL)
					{
						return buff;
					}
				}
				
				if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
				{
					if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
					{
						EndCastMagic(lpML->lpChar_Own);
						g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					}
				}
				
				// 만약 충돌체크를 지역개념으로 한다면 사용할것(서버에서 충돌체크 루틴 돌림)
				//마지막 2/3프레임에서 지역 충돌체크 루틴 server로 Call
				if (lpEL->FrameCount == (int)(EffList[lpEL->sprite_Num].MaxFrame*2/3) && lpML->flag==0)
				{	//< CSD-031024
					lpML->flag=1;

					if (lpML->magicNum == ITEM_EFFECT_LIGHTNING)
					{
						const int check_xl = 5*TILE_SIZE;
						const int check_yl = check_xl*3/4;
						// 폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
						attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
					}
					else if (lpML->magicSerial != 0)		//지역 공격 마법의 경우
					{	// 폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
						const int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
						const int check_yl = check_xl/4;
						attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
					}
					
				}	//> CSD-031024
				
				//스프라이트의 마지막 프레임까지 찍었을 때
				if(lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame)	//lpEL->total_Frame
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
				
				//타일 속성 체크하여 소스 소멸
				if (IsCollision(lpML->magicSerial,
					lpEL->x>>5,
					lpEL->y>>5,
					(lpEL->x + (int)lpEL->incx)>>5, 
					(lpEL->y + (int)lpEL->incy)>>5) == true)
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
				
				if (lpEL->Next == NULL)			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				{
					break;
				}
				else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
			}
			
			lpEL = lpML->EffHead;
			
			while(1)	//실제 이펙트를 연결시켜주는 루틴
			{
				if (lpEL == NULL) break;
				
				//if(lpEL->FrameCount!=0 && lpEL->total_Frame%3==0)	//3프레임마다 동심원 커짐
				{
					//for(i=0;i<=330;i+=first_Inc)
					//{
					/*
					bufx=lpML->x;
					bufy=lpML->y;
					lpML->x += (int)(cos(i*M_PI/180.)*(45+lpEL->FrameCount/3*20));
					lpML->y += (int)(sin(i*M_PI/180.)*(36+lpEL->FrameCount/3*16));
					InsertEffect(lpML);	//매직고유번호,모션넘버,속도->0,방향->0
					lpML->x=bufx;
					lpML->y=bufy;
					*/
					//if(lpEL->total_Frame%3 == 0)	lpEL->FrameCount++;
					plus+=30;
					if(plus>=360)	plus=0;
					lpEL->incx = (float) ( cos(plus*M_PI/180.f)*(lpEL->FrameCount*9.f) );
					lpEL->incy = (float) ( sin(plus*M_PI/180.f)*(lpEL->FrameCount*6.75f) );
					//}
				}
				
				// 0~4 프레임까지 점점 밝아 지면서 생성
				if(lpEL->FrameCount<=5)
				{
					lpEL->trans_Level =(float)(((lpML->trans_Level / 5.)*lpEL->FrameCount)+1);
					if(lpEL->trans_Level>=lpML->trans_Level)	lpEL->trans_Level=(float)(lpML->trans_Level);
				}
				
				// 마지막 6프레임전부터 어두어지면 사라짐
				else if(lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame-6)
				{
					lpEL->trans_Level -= (float)(lpML->trans_Level / 6.);
					if(lpEL->trans_Level < 1 )	lpEL->trans_Level=1;
				}
				
				else
					lpEL->trans_Level = (float)lpML->trans_Level;
				
				DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
				
				if (lpEL->Next == NULL)
				{
					//lpEL->total_Frame++;
					//if(lpEL->total_Frame%3 == 0)	lpEL->FrameCount++;
					break;
				}
				else lpEL = lpEL->Next;
			}
			break;
		}
	}
	return lpML->Next;
}

// 2 Circle ring 이팩트(역방향으로 회전)
MAGICLIST* SampleMagic6(MAGICLIST* lpML) //, int Num, int motion_Num, int speed, int height, int type, int levlpEL, int end_Frame)		//스프라이트의 고유번호
{
	USEEFFECTLIST* lpEL;
	
	static int inc = 0;
	int	crash_Result=0;
	
	BOOL StartFlag = TRUE;
	
	//static int direction=0;
	static int start_X=0, start_Y=0;
	static int angle=0;
	static int angle2=180;
	
	//int sprc =  lpML->lpChar_Own->AnimationTable[ lpML->lpChar_Own->nCurrentAction].cFrame[  lpML->lpChar_Own->nCurrentFrame.GetDecrypted() ];
	
	lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:				
		
		//direction=lpML->lpChar_Own->direction;
		
		start_X=lpML->x+EffList[g_lpET[lpML->magicNum].sprite_Num].offset_X;//lpML->move_X;
		start_Y=lpML->y+EffList[g_lpET[lpML->magicNum].sprite_Num].offset_Y;//lpML->move_Y;
		
		
		lpML->x=start_X+(int)(cos((float)angle)*(lpML->spr_Speed*1.5));
		lpML->y=start_Y+(int)(sin((float)angle)*(lpML->spr_Speed))-18;
		InsertEffect(lpML); //0도에서 시작하여 시계방향으로
		
		if(lpML->object_Num==1)
		{				
			lpML->x=start_X+(int)(sin((float)angle2)*(lpML->spr_Speed*1.5));
			lpML->y=start_Y+(int)(cos((float)angle2)*(lpML->spr_Speed));
			InsertEffect(lpML);//180도에서 시작하여 시계방향으로
		}				
		
		//시전동작으로 전환
		if(lpML->lpChar_Own->nCurrentAction == ACTION_MAGIC_CASTING)
		{
			/////////// 약간의 딜레이가 필요하겠지 //////////////
			lpML->lpChar_Own->changeActionFlag = 0;
			g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
		}
		
		//매직리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 1;
		
		if(lpML->end_Time!=0 && lpML->next_Effect==0 )	//지속마법이며 다음으로 이어지는 마법이 없는 경우
		{
			lpML->magic_Runtime	= g_ClientTime + (g_lpMT[lpML->magicSerial].continue_Time/2*1000);
		}
		
		break;
		
	case 1:		//2개 등록된 경우
		while(1)
		{
			if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				
				buff = lpML->Next;
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);
				
				if (lpEL == NULL)
				{
					angle=0;
					angle2=0;
					start_X=0;
					start_Y=0;
					return buff;
				}
			}
			
			//if(lpEL->FrameCount > EffList[lpEL->sprite_Num].MaxFrame )
			//	lpEL->FrameCount=0;
			
			/*
			//마지막 프레임 찍고 종료
			if(lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame )
			{
			DeleteList(&(lpML->EffHead), lpEL);
			lpEL = lpML->EffHead;
			continue;
			}
			*/
			
			//종료 조건 확인하여 링크드 리스트에서 제외
			if( g_lpET[lpML->magicNum].end_Time > 0)		//특정 유지시간을 가지고 있는 경우
			{
				if( lpML->magic_Runtime <= g_ClientTime )
				{
					lpML->magic_Runtime		= 0;
					
					lpEL->FrameCount=0;
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
			}
			else if( g_lpET[lpML->magicNum].end_Time < 0)		//특정 유지시간을 가지고 있는 경우
			{
				if( lpML->magic_Runtime <= g_ClientTime )
				{
					lpML->magic_Runtime		= 0;
					
					lpEL->FrameCount=0;
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
			}
			else if( ( lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame ) || ( lpML->magicSubNum==4 && lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame/8 ))
			{
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL); 
				lpEL = lpML->EffHead;
				continue;
			}
			
			///////////////////////////////////////// 0313 lkh 추가 ///////////////////////////////////////////////
			if( lpML->lpChar_Own->condition == CON_DEATH || lpML->lpChar_Own->hp <= 0 )		//시전자가 죽어 있는 경우
			{
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			else if( lpML->lpChar_Target->condition == CON_DEATH || lpML->lpChar_Target->hp <= 0 )		//대상자가 죽어 있는 경우
			{	
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
			if(lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame || ++lpEL->total_Frame==lpML->end_Time)
			{
			DeleteList(&(lpML->EffHead), lpEL);
			lpEL = lpML->EffHead;
			continue;
			}
			*/
			
			if (lpEL->Next == NULL)
				break;
			else lpEL = lpEL->Next;
		}
		
		lpEL = lpML->EffHead;
		
		while(1)
		{
			if (lpEL == NULL) break;
			
			angle+=360/(46-(360/lpEL->speed));		//각도 증가
			angle2-=360/(46-(360/lpEL->speed+5));	//각도 감소		
			
			if(angle>=360)	angle=0;
			if(angle2<=0) angle2=360;
			
			if(lpML->object_Num==1)
			{
				if(inc%2==0)
				{
					lpEL->incx=(float)(cos(angle*M_PI/180.)*(lpML->spr_Speed*2.));
					lpEL->incy=(float)(sin(angle*M_PI/180.)*(lpML->spr_Speed)-18.);
				}
				else
				{
					lpEL->incx=(float)(cos(angle2*M_PI/180.)*(lpML->spr_Speed*2.));
					lpEL->incy=(float)(sin(angle2*M_PI/180.)*(lpML->spr_Speed));
				}
			}
			
			else
			{
				lpEL->incx=(float)(cos(angle*M_PI/180.)*(lpML->spr_Speed*2.));
				lpEL->incy=(float)(sin(angle*M_PI/180.)*(lpML->spr_Speed));
			}
			
			if(g_lpET[lpML->magicNum].end_Time<0)			//Magic Table에 정의된 유지 시간동안 지속되는 마법의 색깔값 변환
			{
				lpEL->trans_Level = (float) ((float)lpML->trans_Level * ( (lpML->magic_Runtime - g_ClientTime) / (g_lpMT[lpML->magicSerial].continue_Time*1000.) ) );
				if(lpEL->trans_Level<2)	lpEL->trans_Level=2;
			}
			
			if(g_lpET[lpML->magicNum].sound_loop!=0)
				EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
			
			lpEL->x = lpML->lpChar_Own->x;
			lpEL->y = lpML->lpChar_Own->y - lpML->lpChar_Own->height;
			
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light/100*100+(int)( g_lpET[lpML->magicNum].light%100*((lpML->magic_Runtime - g_ClientTime) / (g_lpMT[lpML->magicSerial].continue_Time*1000.))+.5) );
			
			if(lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame)	lpEL->FrameCount=0;
			
			inc++;
			if(inc>=10000) inc=0;
			
			if (lpEL->Next == NULL)	break;
			
			else lpEL = lpEL->Next;							
			
		}
		break;
	}
	return lpML->Next;
}

//불새 마법
//시전자몸에서 특정 방향 & 속도로 날아가는 이팩트(인자:마법리스트/스프라이트 고유번호/모션넘버/생성되는 최대 효과객체수/각 오브젝트의 진행속도/효과스프라이트의 진행속도/높이/투명방법/투명도/마지먹 프레임)
MAGICLIST* SampleMagic8(MAGICLIST* lpML)		//스프라이트의 고유번호
{	//< CSD-TW-030701
	USEEFFECTLIST* lpEL;
	static int inc = 0, inc2 = 0;
	int		crash_Result=0;
	int		backup_LevlpEL=0;		
	
	int		effect_CurrentX=0;
	int		effect_CurrentY=0;
	
	int		tx=0,ty=0,tarx=0,tary=0;
	
	BOOL StartFlag = TRUE;
	static int direction=0;//lpCharacter->direction;
	//int sprc =  lpML->lpChar_Own->AnimationTable[ lpML->lpChar_Own->nCurrentAction].cFrame[ lpML->lpChar_Own->nCurrentFrame.GetDecrypted() ];
	static int incx = 0, incy = 0; //매직생성할때 쓰임.
	int		illusion_Trans=0;
	
	lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	
	switch (lpML->magicSerial)
	{
    case ICE_BREATH:
    case LIGHTNING_BREATH:
    case THUNDER_BLOW:
    case GROUND_ATTACK:
    case WHILWIND:
		{
			break;
		}
    default:
		{
			ChangeDirection((DIRECTION*)&lpML->dir, lpML->x, lpML->y, lpML->tarx, lpML->tary);
			break;
		}
	}
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:
		{
			lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
			
			switch (lpML->magicSerial)
			{
			case THUNDER_BLOW:
			case GROUND_ATTACK:
			case DARK_BLADE:
			case DARK_EXTREME:
				{
					break;
				}
			default:
				{
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->nCurrentAction, lpML->lpChar_Own);
					break;
				}
			}
			
			direction = lpML->dir;
			
			switch (lpML->dir)
			{
			case 0: lpML->y += 32, lpML->x -=  5; break;
			case 1: lpML->x -= 24, lpML->y += 24; break;
			case 2: lpML->x -= 32; break;
			case 3: lpML->x -= 24, lpML->y -= 24; break;
			case 4: lpML->y -= 32; break;
			case 5: lpML->x += 24, lpML->y -= 24; break;
			case 6: lpML->x += 32; break;
			case 7: lpML->x += 24, lpML->y += 24; break;
			}
		
			InsertEffect(lpML);
			lpML->EffHead->StartFlag = 1;
			//매직리스트에 마법 하나 등록된 사실 셋팅
			lpML->PCount = 1;
			break;
		}
	case 1:
		{
			while (1)
			{
				if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
				{
					MAGICLIST* buff = lpML->Next;
					
					if (lpML->lpChar_Own->changeActionFlag == 1)
					{
						lpML->lpChar_Own->changeActionFlag = 0;
					}
					
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					lpML->EffectCount = 0;
					DeleteList(&g_lpML, lpML);
					
					if (lpEL == NULL)
					{
						return buff;
					}
				}
				
				if (lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
				{
					if (TimeCheck(lpML->lpChar_Own->end_ReadyMagic))
					{	// 마법 발사 유지 단계를 종료
						EndCastMagic(lpML->lpChar_Own);
						
						switch (lpML->magicSerial)
						{
						case THUNDER_BLOW:
						case GROUND_ATTACK:
						case WHILWIND:
						case DARK_EXTREME:
							{
								break;
							}
						default:
							{
								g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
								break;
							}
						}
					}
				}
				// 이펙트의 좌표가 벋어날 경우 소멸 
				if (abs((int)(lpEL->incx+.5)) >= SCREEN_WIDTH || abs((int)(lpEL->incy+.5)) >= SCREEN_WIDTH)
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
				//효과 이펙드가 위치하는 타일의 속성 체크하여 이펙트 소멸
				const int mmx = (lpEL->x + (int)lpEL->incx)/TILE_SIZE;
				const int mmy = (lpEL->y + (int)lpEL->incy)/TILE_SIZE;
				
				if (mmx >= 0 && mmx < g_Map.file.wWidth && mmy >= 0 && mmy < g_Map.file.wHeight)
				{
					if (IsCollision(lpML->magicSerial, lpEL->x>>5, lpEL->y>>5, (lpEL->x + (int)lpEL->incx)>>5, (lpEL->y + (int)lpEL->incy)>>5))
					{
						InsertMagic( lpML->lpChar_Own, lpML->lpChar_Target,lpML->next_Effect, 9, 0, 0, (int)(lpEL->x + lpEL->oldincx + 16), (int)(lpEL->y + lpEL->oldincy + 16));
						DeleteList(&(lpML->EffHead), lpEL);
						lpEL = lpML->EffHead;
						continue;
					}
				}
				
				int nDirection = lpML->lpChar_Own->direction;// 030516 kyo
				bool bCrash = false;
				
				switch (lpML->magicSerial)
				{ // 효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
				case GROUND_ATTACK:
					{
						const int nRate = EffList[lpEL->sprite_Num].MaxFrame/8;
						
						if (lpEL->FrameCount%6 == 0)
						{
							bCrash = true;
						}
						
						break;
					}
				case WHILWIND: 
					{
						const int nRate = EffList[lpEL->sprite_Num].MaxFrame/8;
						
						if (lpEL->FrameCount%4 == 0)
						{
							bCrash = true;
						}
						
						break;
					}
				case BLOOD_LIGHTNING: // 030506 kyo
					{	// 나가는 이팩트중에서 1/3확률로 맞는다.
						if (4 > lpEL->FrameCount)
						{	//직선으로 나가는 이펙트에서만 충돌검사 
							break;
						}							
						if (rand()%3!= 0) // 030520 kyo 
						{
							break;
						}
						
						nDirection = lpML->dir;
						bCrash = true;
						break;
					}
				case ICE_BREATH:       // 얼음 브레쓰
				case LIGHTNING_BREATH: // 전격 브레쓰
				case THUNDER_BLOW:
				case GAZER:
					{	//< CSD-031014
						bCrash = true;
						break;
					}	//> CSD-031014
				default:
					{
						int check_xl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
						int check_yl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
						if(check_xl > 48)	check_xl=(int)(check_xl*.75);							//너무 큰 스프라이트인 경우 보정
						if(check_yl > 48)	check_yl=(int)(check_yl*.75);
						//발사 이펙트가 적 케릭터에 맞았는지 체크(첫번째인자 1인 경우->복수 타일 체크 & 충돌처리를 함수에 맞김/마지막 인자->내 자신(시전자) 체크여부)
						attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (int)((lpEL->x+lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2)), (int)((lpEL->y+lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-(check_yl/2)), (int)lpEL->height, check_xl, check_yl );
						
						if (attacked_Char!=NULL) 
						{
							if(lpML->magicSerial != PHEONIX)	//이펙트가 불새가 아닌 경우 소멸
							{
								if (lpEL->effect_Count == 0)
								{
									lpML->flag=1;	
									
									if(lpEL!=NULL && lpML->magicSerial!=0 )
									{
										g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x+(int)(lpEL->incx+.5), lpEL->y+(int)(lpEL->incy+.5));
									}
									
									InsertMagic(lpML->lpChar_Own,attacked_Char, lpML->next_Effect , 9, 0,0,attacked_Char->x,attacked_Char->y);	
								}
								
								DeleteList(&(lpML->EffHead), lpEL);
								lpEL = lpML->EffHead;
								continue;
							} 					  
							
							if (lpEL->effect_Count == 0)
							{
								if (lpEL!=NULL && lpML->magicSerial != 0)
								{
									g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x+(int)(lpEL->incx+.5), lpEL->y+(int)(lpEL->incy+.5));
								}
								
								InsertMagic(lpML->lpChar_Own, attacked_Char, lpML->next_Effect ,9,0,0,attacked_Char->x,attacked_Char->y);
							}
						}

						break;
					}
				}

				if (bCrash)
				{
					switch (nDirection)
					{
					case DIRECTION_DOWN: 
						{
							const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
							const int nOffsetX = EffList[lpEL->sprite_Num].offset_X - (nWidth>>1);
							const int nOffsetY = EffList[lpEL->sprite_Num].offset_Y + nHeight;
							attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpEL->x + nOffsetX, lpEL->y + nOffsetY, (int)lpEL->height, nWidth, nHeight);
							break;
						}
					case DIRECTION_LEFTDOWN: 
						{
							const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							int nOffsetX = 0, nOffsetY = 0, nDelta = 0;
							
							do
							{
								nDelta += nWidth;
								nOffsetX = EffList[lpEL->sprite_Num].offset_X - nDelta;
								nOffsetY = EffList[lpEL->sprite_Num].offset_Y + nDelta;
								attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpEL->x + nOffsetX, lpEL->y + nOffsetY, (int)lpEL->height, nWidth, nHeight);
								if (attacked_Char != NULL)  break;  
							}
							while (nDelta < Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl);
							
							break;
						}
					case DIRECTION_LEFT:
						{
							const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
							const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							const int nOffsetX = EffList[lpEL->sprite_Num].offset_X - nWidth;
							const int nOffsetY = EffList[lpEL->sprite_Num].offset_Y - (nHeight>>1);
							attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpEL->x + nOffsetX, lpEL->y + nOffsetY, (int)lpEL->height, nWidth, nHeight);
							break;
						}
					case DIRECTION_LEFTUP:
						{
							const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							int nOffsetX = 0, nOffsetY = 0, nDelta = 0;
							
							do
							{
								nDelta += nWidth;
								nOffsetX = EffList[lpEL->sprite_Num].offset_X - nDelta;
								nOffsetY = EffList[lpEL->sprite_Num].offset_Y - nDelta;
								attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpEL->x + nOffsetX, lpEL->y + nOffsetY, (int)lpEL->height, nWidth, nHeight);
								if (attacked_Char != NULL)  break;  
							}
							while (nDelta < Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl);
							
							break;
						}
					case DIRECTION_UP:
						{
							const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
							const int nOffsetX = EffList[lpEL->sprite_Num].offset_X - (nWidth>>1);
							const int nOffsetY = EffList[lpEL->sprite_Num].offset_Y - nHeight;
							attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpEL->x + nOffsetX, lpEL->y + nOffsetY, (int)lpEL->height, nWidth, nHeight);
							break;
						} 
					case DIRECTION_RIGHTUP:
						{                
							const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							int nOffsetX = 0, nOffsetY = 0, nDelta = 0;
							
							do
							{
								nOffsetX = EffList[lpEL->sprite_Num].offset_X + nDelta;
								nDelta += nWidth;
								nOffsetY = EffList[lpEL->sprite_Num].offset_Y - nDelta;
								attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpEL->x + nOffsetX, lpEL->y + nOffsetY, (int)lpEL->height, nWidth, nHeight);
								if (attacked_Char != NULL)  break;  
							}
							while (nDelta < Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl);
							
							break;
						}
					case DIRECTION_RIGHT:
						{
							const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
							const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							const int nOffsetX = EffList[lpEL->sprite_Num].offset_X + nWidth;
							const int nOffsetY = EffList[lpEL->sprite_Num].offset_Y - (nHeight>>1);
							attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpEL->x + nOffsetX, lpEL->y + nOffsetY, (int)lpEL->height, nWidth, nHeight);
							break;
						}
					case DIRECTION_RIGHTDOWN:
						{
							const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
							int nOffsetX = 0, nOffsetY = 0, nDelta = 0;
							
							do
							{
								nOffsetX = EffList[lpEL->sprite_Num].offset_X + nDelta;
								nDelta += nWidth;
								nOffsetY = EffList[lpEL->sprite_Num].offset_Y + nDelta;
								attacked_Char = ReturnMagicCheckedCharacter(1, lpML, lpEL->x + nOffsetX, lpEL->y + nOffsetY, (int)lpEL->height, nWidth, nHeight);
								if (attacked_Char != NULL)  break;  
							}
							while (nDelta < Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl);
							break;
						}
					}
				}
		
				if (lpEL->Next == NULL)
				{
					break;
				}
				
				lpEL = lpEL->Next;
			}
		
			lpEL = lpML->EffHead;
		
			while (1)
			{
				if (lpEL == NULL)
				{
					break;
				}
				
				tx = lpEL->x;
				ty = lpEL->y;
				
				switch (lpML->magicSerial)
				{
				case SHADOW_OF_EVIL: // 악의 그림자
				case DARK_EXTREME:
					{
						tarx = lpML->t_X;
						tary = lpML->t_Y;
						break;
					}
				default:
					{
						if (lpML->lpChar_Target == lpML->lpChar_Own)		
						{ // 특정위치를 클릭한 경우(직진개념)
							tarx = lpML->t_X;
							tary = lpML->t_Y;
						}
						else
						{ // 적 인물을 클릭한 경우(반유도개념)
							tarx = lpML->lpChar_Target->x;
							tary = lpML->lpChar_Target->y;
						}
						
						break;
					}
				}
				
				int inc1x = 0, inc1y = 0;
				
				if (abs(tx - tarx) > abs(ty - tary))
				{	
					inc1x = (tarx < tx) ? -lpEL->speed:lpEL->speed;
					inc1y = (abs(tarx - tx) == 0) ? 0:lpEL->speed*(tary - ty)/abs(tarx - tx);
				}	
				else 
				{	
					inc1x = (abs(tary - ty) == 0) ? 0:lpEL->speed*(tarx - tx)/abs(tary - ty);
					inc1y = (tary < ty ) ? -lpEL->speed:lpEL->speed;
				}	
				
				lpEL->oldincx = lpEL->incx;
				lpEL->oldincy = lpEL->incy;
				
				lpEL->incx += inc1x;
				lpEL->incy += inc1y;
				
				
				if ((inc%2 == 0) && (lpML->EffectCount < lpML->object_Num ) && (lpEL->StartFlag == 1))
				{
					lpML->EffectCount ++;
					lpML->trans_Level=lpML->trans_Level/2;
					if(lpML->trans_Level<=1) lpML->trans_Level=1;
					InsertEffect(lpML);
				}
				
				float temp_incx, temp_incy;
				
				switch (lpML->magicSerial)
				{
				case THUNDER_BLOW:
				case GROUND_ATTACK:
				case WHILWIND:
				case ICE_BREATH:       // 얼음 브레쓰
				case LIGHTNING_BREATH: // 전격 브레쓰
					{
						temp_incx = 0;
						temp_incy = 0;
						lpEL->incx = 0;
						lpEL->incy = 0;
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
						break;
					}
				case DARK_EXTREME:
					{
						temp_incx = lpEL->incx;
						temp_incy = lpEL->incy;
						lpEL->incx = lpEL->oldincx;
						lpEL->incy = lpEL->oldincy;
						lpML->fallow = true;
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, true);
						break;
					}
				default:
					{
						temp_incx = lpEL->incx;
						temp_incy = lpEL->incy;
						lpEL->incx = lpEL->oldincx;
						lpEL->incy = lpEL->oldincy;
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
						break;
					}
				}
				
				lpEL->incx=temp_incx;
				lpEL->incy=temp_incy;
				
				if(EffList[lpML->sprite_Num].MaxFrame%8==0)
				{
					if(lpEL->FrameCount%(EffList[lpML->sprite_Num].MaxFrame/8)==0)
					{
						switch (lpML->magicSerial)
						{
						case WHILWIND:
						case BLOOD_LIGHTNING:
							{
								g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
							}
						case THUNDER_BLOW:
						case GROUND_ATTACK:
						case GAZER: // 030509 kyo
							{
								DeleteList(&g_lpML, lpML);
								return NULL;
							}
						case ICE_BREATH:       // 얼음 브레쓰
						case LIGHTNING_BREATH: // 전격 브레쓰
						case SIMUNIAN_BOSOM:
							{
								DeleteList(&g_lpML, lpML);
								return NULL;
							}
						default:
							{
								lpEL->FrameCount-=(EffList[lpML->sprite_Num].MaxFrame/8);
								break;
							}
						}
					}
				}
				
				inc++;
				
				if (lpEL->Next == NULL)
				{
					break;
				}
				
				lpEL = lpEL->Next;			
			}
			
			break;
		}
	}

	return lpML->Next;
}	//> CSD-TW-030701

// 8 Way 이팩트(회돌이)
MAGICLIST* SampleMagic9(MAGICLIST* lpML)//, int Num, int motion_Num, int speed, int height, int type, int levlpEL, int end_Frame)		//스프라이트의 고유번호
{
	USEEFFECTLIST* lpEL;
	
	static int inc = 0;
	int	crash_Result=0;
	int round_incx=0, round_incy=0;
	int round_incx1=0, round_incy1=0;
	
	BOOL StartFlag = TRUE;
	
	static int start_X=0,start_Y=0;
	
	/*
	result=ChangeDirection(direction_Missile,lpML->x,lpML->y,lpML->tarx, lpML->tary);
	
	  lpEL->x=lpML->x+(int)(cos(angle)*(lpML->spr_Speed*2));
	  lpEL->y=lpML->y+(int)(sin(angle)*(lpML->spr_Speed))-18;
	*/
	
	lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:
		{
			ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
			lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
			// 좌표 설정
			int nOffX = 0, nOffY = 0;
			
			switch (lpML->magicSerial)
			{
			case WIND_EXTREME:
				{
					nOffX = nOffY = 200;
					break;
				}
			default:
				{
					g_mgrBattle.ActCasting(lpML->magicSerial, lpML->lpChar_Own);      
					break;
				}
			}
			
			lpML->dir=DIRECTION_DOWN;
			InsertEffect(lpML, 0, nOffX, nOffY);
			lpML->dir=DIRECTION_LEFTDOWN;
			InsertEffect(lpML, 1, nOffX, nOffY);
			lpML->dir=DIRECTION_LEFT;
			InsertEffect(lpML, 2, nOffX, nOffY);
			lpML->dir=DIRECTION_LEFTUP;
			InsertEffect(lpML, 3, nOffX, nOffY);
			lpML->dir=DIRECTION_UP;
			InsertEffect(lpML, 4, nOffX, nOffY);
			lpML->dir=DIRECTION_RIGHTUP;
			InsertEffect(lpML, 5, nOffX, nOffY);
			lpML->dir=DIRECTION_RIGHT;
			InsertEffect(lpML, 6, nOffX, nOffY);
			lpML->dir=DIRECTION_RIGHTDOWN;
			InsertEffect(lpML, 7, nOffX, nOffY);
			//매직리스트에 마법 하나 등록된 사실 셋팅
			lpML->PCount = 1;
			break;
		}
	case 1:		//2개 등록된 경우
		while(1)
		{
			if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				
				buff = lpML->Next;
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				
				switch (lpML->magicSerial)
				{
				case WIND_EXTREME:
					{
						break;
					}
				default:
					{
						g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
						break;
					}
				}
				
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);
				
				if (lpEL == NULL) return buff;
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					EndCastMagic(lpML->lpChar_Own);
					
					switch (lpML->magicSerial)
					{
					case WIND_EXTREME:
						{
							break;
						}
					default:
						{
							g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
							break;
						}
					} 
				}
			}
			
			// 이펙트의 좌표가 벋어날 경우 
			if(abs((int)lpEL->incx)>=380 || abs((int)lpEL->incy)>=380)
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			//지형 타일 체크(충돌하면 폭발)
			if (IsCollision(lpML->magicSerial,
				lpEL->x>>5,
				lpEL->y>>5,
				(lpEL->x + (int)lpEL->incx)>>5, 
				(lpEL->y + (int)lpEL->incy)>>5) == true)
			{
				InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target,lpML->next_Effect,  9,  0,  0,  (int)(((lpEL->x+lpEL->oldincx)/32.)*32.+16), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+16) );
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			int check_xl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
			int check_yl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
			
			//발사 이펙트가 적 케릭터에 맞았는지 체크(복수 타격 이팩트)
			attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (int)((lpEL->x+lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2)), (int)((lpEL->y+lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-(check_yl/2)), (int)lpEL->height, check_xl, check_yl, 0);
			
			if( attacked_Char )			//충돌한 인물이 있으며
			{
				if( lpEL && lpML->magicSerial )
				{
					g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x+(int)(lpEL->incx+.5), lpEL->y+(int)(lpEL->incy+.5));    
				}
				
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				
				InsertMagic(lpML->lpChar_Own, attacked_Char,lpML->next_Effect, 9, 0, 0, attacked_Char->x, attacked_Char->y);
				lpML->flag=1;
				
				crash_Result=0;
				continue;
			}
			
			if( lpEL == NULL ) break;
			
			if (lpEL->Next == NULL)
			{
				break;
			}
			else lpEL = lpEL->Next;
			}
			
			lpEL = lpML->EffHead;
			
			while(1)
			{
				if (lpEL == NULL) break;
				if (lpEL->total_Frame == EffList[lpEL->sprite_Num].MaxFrame * 3/4 )
				{
					switch (lpML->magicSerial)
					{
					case WIND_EXTREME:
						{
							break;
						}
					default:
						{
							g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
							break;
						}
					} 
				}
				//		0208 lkh 수정/추가 (방사화염의 경우 23번째 프레임에서 두번째 방사화염 발사/연결)
				if( ( lpML->EffectCount < lpML->object_Num) && (lpEL->total_Frame == EffList[lpEL->sprite_Num].MaxFrame*2/3) )
				{
					lpML->EffectCount++;
					
					lpML->dir=DIRECTION_DOWN;
					InsertEffect(lpML,0);
					lpML->dir=DIRECTION_LEFTDOWN;
					InsertEffect(lpML,1);
					lpML->dir=DIRECTION_LEFT;
					InsertEffect(lpML,2);
					lpML->dir=DIRECTION_LEFTUP;
					InsertEffect(lpML,3);
					lpML->dir=DIRECTION_UP;
					InsertEffect(lpML,4);
					lpML->dir=DIRECTION_RIGHTUP;
					InsertEffect(lpML,5);
					lpML->dir=DIRECTION_RIGHT;
					InsertEffect(lpML,6);
					lpML->dir=DIRECTION_RIGHTDOWN;
					InsertEffect(lpML,7);
					InsertEffect(lpML);
				}
				
				//round_incx = (int)((lpEL->total_Frame*5.)*cos((lpEL->total_Frame*12%360)*M_PI/180.));
				//round_incy = (int)((lpEL->total_Frame*3)*sin((lpEL->total_Frame*12%360)*M_PI/180.));
				//round_incx = (int)((lpEL->total_Frame*5.)*cos((lpEL->total_Frame*10%360)*M_PI/180.));
				//round_incy = (int)((lpEL->total_Frame*3.)*sin((lpEL->total_Frame*10%360)*M_PI/180.));
				round_incx = (int)((lpEL->total_Frame*4.)*cos((lpEL->total_Frame*9%360)*M_PI/180.));
				round_incy = (int)((lpEL->total_Frame*3.)*sin((lpEL->total_Frame*9%360)*M_PI/180.));
				round_incx1 = (int)((lpEL->total_Frame*4.)*cos((lpEL->total_Frame*9%360)*M_PI/180.));
				round_incy1 = (int)((lpEL->total_Frame*3.)*sin((lpEL->total_Frame*9%360)*M_PI/180.));
				
				/*
				if(abs(round_incx)>30)	
				{
				if(round_incx>=0)	{	round_incx=30;	round_incx1=21;	}
				else				{	round_incx=-30;	round_incy1=-21;}
				}
				if(abs(round_incy)>20)
				{
				if(round_incy>=0)	{	round_incy=20;	round_incy1=14;	}
				else				{	round_incy=-20;	round_incy1=-14;}
				}
				*/
				
				lpEL->oldincx = lpEL->incx;
				lpEL->oldincy = lpEL->incy;
				
				switch(lpEL->dir)
				{
				case DIRECTION_DOWN:
					lpEL->incx -= round_incx;
					lpEL->incy += (lpEL->speed/2)-round_incy;
					lpEL->total_Frame++;
					break;
				case DIRECTION_LEFTDOWN:
					lpEL->incx -= (int)((lpEL->speed/2)*.8);
					lpEL->incx -= round_incx1;
					lpEL->incy += (int)((lpEL->speed/2)*.8);
					lpEL->incy -= round_incy1;
					lpEL->total_Frame++;
					break;
				case DIRECTION_LEFT:
					lpEL->incx -= (lpEL->speed/2)-round_incy;
					lpEL->incy -= round_incx;
					lpEL->total_Frame++;
					break;
				case DIRECTION_LEFTUP:
					lpEL->incx -= (int)((lpEL->speed/2)*.8);
					lpEL->incx += round_incy1;
					lpEL->incy -= (int)((lpEL->speed/2)*.8);
					lpEL->incy -= round_incx1;
					lpEL->total_Frame++;
					break;
				case DIRECTION_UP:
					lpEL->incx += round_incx;
					lpEL->incy -= (lpEL->speed/2)-round_incy;
					lpEL->total_Frame++;
					break;
				case DIRECTION_RIGHTUP:
					lpEL->incx += (int)((lpEL->speed/2)*.8);
					lpEL->incx += round_incx1;
					lpEL->incy -= (int)((lpEL->speed/2)*.8);
					lpEL->incy += round_incy1;
					lpEL->total_Frame++;
					break;
				case DIRECTION_RIGHT:
					lpEL->incx += (lpEL->speed/2)-round_incy;
					lpEL->incy += round_incx;
					lpEL->total_Frame++;
					break;
				case DIRECTION_RIGHTDOWN:
					lpEL->incx += (int)((lpEL->speed/2)*.8);
					lpEL->incx -= round_incy1;
					lpEL->incy += (int)((lpEL->speed/2)*.8);
					lpEL->incy += round_incx1;
					lpEL->total_Frame++;
					break;
				}
				
				/*
				if(abs(lpEL->incx)>60)	
				{
				if(lpEL->incx>=0)	{	lpEL->incx=60;	round_incx1=42;	}
				else				{	lpEL->incx=-40;	round_incy1=-28;}
				}
				if(abs(lpEL->incy)>40)
				{
				if(lpEL->incy>=0)	{	lpEL->incy=40;	round_incy1=28;	}
				else				{	lpEL->incy=-30;	round_incy1=-21;}
				}
				*/
				
				//특정 외곽 라인에 근접할수록 색깔값 사라짐
				lpEL->trans_Level=(float)(lpML->trans_Level*(( 280. - abs((int)lpEL->incx) ) / 60.));
				lpEL->trans_Level=(float)(lpEL->trans_Level*(( 280. - abs((int)lpEL->incy) ) / 60.));
				if(lpEL->trans_Level>lpML->trans_Level)			lpEL->trans_Level=(float)lpML->trans_Level;
				if(lpEL->trans_Level<5)							lpEL->trans_Level=5;
				
				float temp_incx = lpEL->incx;
				lpEL->incx=	lpEL->oldincx;
				float temp_incy = lpEL->incy;
				lpEL->incy=	lpEL->oldincy;
				
				switch (lpML->magicSerial)
				{
				case WIND_EXTREME:
					{
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, 1);
						break;
					}
				default:
					{
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
						break;
					}
				}
				
				lpEL->incx=temp_incx;
				lpEL->incy=temp_incy;
				
				inc++;
				
				if (lpEL->Next == NULL)	break;
				
				else lpEL = lpEL->Next;			
			}
			break;
	}
	return lpML->Next;
}

// 유성 떨어지는 마법(특정 위치에서 특정 위치로)
MAGICLIST* SampleMagic10(MAGICLIST *lpML) //, int Num, int motion_Num, int speed, int height, int type, int levlpEL, int end_Frame)
{
	if (lpML->magic_Runtime > g_ClientTime)
	{
		return lpML->Next;
	}
	
	USEEFFECTLIST*	lpEL;
	int		crash_Result=0, crash_Result2=0;
	int		start_X=0;
	
	lpEL = lpML->EffHead;
	
	switch (lpML->magicSerial)
	{
    case ICE_STORM:
    case FIRE_STORM:
    case FLAME_POUR:
		{
			start_X=lpML->map_Offset.x*32+(SCREEN_WIDTH-30)+rand()%20;
			lpML->dir=0;  
			break;
		}
    default:
		{
			if(lpML->tarx<= (SCREEN_WIDTH/2)+lpML->map_Offset.x*32)
			{
				start_X=lpML->map_Offset.x*32+(SCREEN_WIDTH-30)+rand()%20;
				lpML->dir=0;
			}
			else
			{
				start_X=lpML->map_Offset.x*32+30-rand()%20;
				lpML->dir=1;		
			} 
			
			break;
		}
	}
	
	lpML->x=start_X;
	lpML->y=lpML->map_Offset.y*32 - ((GAME_SCREEN_YSIZE-10)-(lpML->tary - lpML->map_Offset.y*32));
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:	
		{
			switch (lpML->magicSerial)
			{
			case ICE_STORM:
				{
					break;
				}
			default:
				{
					ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
					lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->nCurrentAction, lpML->lpChar_Own);
					break;
				}
			}
			InsertEffect(lpML);
			lpML->PCount = 1;
			
			break;
		}
	case 1:
		while(1)
		{
			if (lpML->EffHead == NULL)
			{
				MAGICLIST* buff;
				buff = lpML->Next;
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				
				g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);
				
				if (lpEL == NULL)
					return buff;
			}
			
			int check_xl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
			int check_yl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
			
			//클릭한 지점에 떨어졌는지 확인 
			if(lpML->flag==0)
				crash_Result=BoxAndBoxCrash(lpML->t_X - (TILE_SIZE/2), lpML->t_Y - (TILE_SIZE/2), TILE_SIZE, TILE_SIZE, (int)(lpEL->x+lpEL->incx- (check_xl/2)), (int)(lpEL->y+lpEL->incy- (check_yl/2)), check_xl, check_yl);
			
			if(crash_Result==1)// || lpEL->height<=0 )
			{
				//대형 화탄 폭발 효과 링크
				InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, lpML->next_Effect, 9/*lpML->magicSubNum*/, lpML->s_X, lpML->s_Y, (int)(((lpEL->x+lpEL->incx)/32.)*32.+16), (int)(((lpEL->y+lpEL->incy)/32.)*32.+16) );
				crash_Result=0;
				lpML->flag=1;
				
				switch (lpML->magicSerial)
				{
				case METEOR_STRIKE:
				case GREAT_METEOR:
					{
						break;
					}
				default:
					{ //폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, (int)((lpEL->x+lpEL->incx) - (check_xl/2)), (int)((lpEL->y+lpEL->incy) - (check_xl/2)), 0, check_xl, check_yl, 1);
						break;
					}
				}
				
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					EndCastMagic(lpML->lpChar_Own);
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
				}
			}
			
			// 이펙트의 좌표가 벋어날 경우 
			if(abs((int)lpEL->incx)>=SCREEN_WIDTH || abs((int)lpEL->incy)>=SCREEN_HEIGHT)
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			if (lpEL->Next == NULL)
			{
				break;
			}
			else lpEL = lpEL->Next;
		}		
		
		lpEL = lpML->EffHead;
		
		while(lpEL != NULL)
		{		
			int tx = start_X;//lpML->x;
			int ty = lpML->y;
			int tarx = lpML->t_X;//lpChar_Target->x;
			int tary = lpML->t_Y;//lpChar_Target->y;
			int incx, incy;
			float inc_Height;
			
			if(abs(tx-tarx) > abs(ty-tary))
			{	
				incy = lpEL->speed * ( tary - ty ) / abs( tarx - tx );
				incx = ((tarx<tx) ? -lpEL->speed : lpEL->speed);
				inc_Height = lpEL->height / (float)(abs( tarx - tx ) / (float)lpEL->speed);
			}	
			else 
			{	
				incy = ((tary<ty ) ? -lpEL->speed : lpEL->speed);
				incx = lpEL->speed * ( tarx - tx ) / abs( tary - ty ) ;
				inc_Height = lpEL->height / (float)(abs( tary - ty ) / (float)lpEL->speed);
			}	
			
			lpEL->incx += incx;
			lpEL->incy += incy;	
			
			lpEL->height -= inc_Height;
			if(lpEL->height<0)	lpEL->height=0;
			
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
			
			if(lpEL->FrameCount>=9 )	lpEL->FrameCount=0;
			
			lpEL = lpEL->Next;
		}	
		break;
	}			
	return lpML->Next;
}

MAGICLIST*	SampleMagic25(MAGICLIST*	lpML)
{
	switch (lpML->magicSerial)
	{  
    case EARTH_EXTREME:
    case DARK_EXTREME:
    case FLAME_PILLAR:
    case FLAME_POUR:
    case HOLY_RAGE:
	case LIGHTNING_EXTREME:	// 030328 kyo
	case ICE_BREATH2:	// 030415 kyo
	case AUTHORITY_OF_WHOARENCHA:	// 030415 kyo
		{
			if (lpML->magic_Runtime > g_ClientTime)
			{
				return lpML->Next;
			}
			
			break;
		}
	}
	
	USEEFFECTLIST* lpEL = lpML->EffHead;
	int	terminateFrame = 0;
	bool target_Type = false;	// 시전자에게 찍을까?(0) 대상체에게 찍을까?(1)
	
	switch(lpML->PCount)
	{
	case 0:
		{
			InsertEffect(lpML);	// 매직고유번호,모션넘버,속도->0,방향->0
			// 지속마법인 경우 지속 시작시간 셋팅
			if (lpML->magic_Runtime == 0)
			{
				if(lpML->end_Time > 0)	
				{ // magic 테이블의 end_Time 필드가 0보다 큰 경우
					lpML->magic_Runtime= g_ClientTime + (g_lpET[lpML->magicNum].end_Time*1000);
				}
				else if(lpML->end_Time < 0)
				{ // magic 테이블의 end_Time 필드가 -1인 경우
					lpML->magic_Runtime	= g_ClientTime + (g_lpMT[lpML->magicSerial].continue_Time*1000);
				}
			}
			
			lpML->PCount = 1;   //매직리스트에 마법 하나 등록된 사실 셋팅	
			break;
		}		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거
		{
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림
				if (lpEL == NULL) 	return buff;
			}
			
			if( lpML->lpChar_Own == NULL || lpML->lpChar_Target == NULL )
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				if (lpEL == NULL)  return buff;
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					//EndCastMagic(lpML->lpChar_Own);
				}
			}
			
			if (lpML->flag==0 && 
				(DayLightControl < 31) && 
				g_lpET[lpML->magicNum].end_Time!=-1 && 
				IsHarmfulMagic(lpML->magicSerial))
			{
				terminateFrame=(EffList[lpEL->sprite_Num].MaxFrame)/LIGHT_TERMINATE;
				
				if(terminateFrame<1)	terminateFrame=1;
				
				if(g_lpET[lpML->magicNum].light/100 == 1 )
					InsertLightOut( lpML->tarx, (int)(lpML->tary-lpEL->height), g_lpET[lpML->magicNum].light%100, terminateFrame, g_lpET[lpML->magicNum].light/100 );
				else
					InsertLightOut( lpML->tarx, lpML->tary, g_lpET[lpML->magicNum].light%100, terminateFrame, g_lpET[lpML->magicNum].light/100 );
				
				switch (lpML->magicSerial)
				{ 
				case LIGHTNING_BOOM:
					{
						lpML->flag = 0;
						break;
					}
				default:
					{
						lpML->flag = 1;
						break;
					}
				}
			}
			
			if (g_lpET[lpML->magicNum].end_Time) // 특정 유지시간을 가지고 있는 경우
			{
				if( lpML->magic_Runtime <= g_ClientTime )
				{					
					if (lpML->magicSerial == TWISTER)
					{
						lpML->lpChar_Own->nActiveCombat.PutEncrypted(0);
					}
					
					lpML->magic_Runtime	= 0;
					lpEL->FrameCount = 0;
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
			}
			else if ((lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame) || (lpML->magicSubNum==4 && lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame/8))
			{
				if(lpML->next_Effect==0 || g_lpET[lpML->magicSerial].impect_Effect_Num==0)
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				
				lpEL->FrameCount=0;
				
				switch (lpML->magicSerial)
				{
				case OCTA_LIGHTNING:
				case BLOOD_SPRINKLE:
				case BLOOD_EARTH:
				case POISONING_NOVA:
				case DIVINE_POWER:
					{
						CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction);
						break;
					}
				}
				
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			if( lpML->lpChar_Own->condition == CON_DEATH || lpML->lpChar_Own->hp <= 0 )		//시전자가 죽어 있는 경우
			{// 1012 kkh 약간수정..	
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}				
			else if( (lpML->lpChar_Target->condition == CON_DEATH || lpML->lpChar_Target->hp <= 0) && !(lpML->magicSerial == 7 || lpML->magicSerial ==164 || lpML->magicSerial == 215) )		//대상자가 죽는 경우(소생마법 제외) // 030716 kyo ADD lpML->magicSerial == 215 
			{	//< CSD-030723
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}	//> CSD-030723
			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
			if (lpEL->Next == NULL)  break;
			else  lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
      }
      
      lpEL = lpML->EffHead;
      
      while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
      {
		  if(lpML->EffectCount < lpML->object_Num-1)
		  {	// 동시 출력하는 이팩트의 갯수가 복수일 때 
			  InsertEffect(lpML);
			  lpML->EffectCount++;
		  }
		  
		  const int nWidth = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;
		  const int nHeight = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
		  
		  switch (lpML->magicSerial)
		  {  
          case VAMPIRE_POISON:
          case FEATHER_OF_CURSE:
          case CURSE_OF_HELL:
			  {
				  LPCHARACTER attacked_Char = ReturnMagicCheckedCharacter(0, lpML, lpEL->tarx + EffList[lpEL->sprite_Num].offset_X - nWidth/2, lpEL->tary + EffList[lpEL->sprite_Num].offset_Y - nHeight/2, (int)lpEL->height, nWidth, nHeight);
				  
				  if (attacked_Char != NULL && attacked_Char->condition == CON_NORMAL && !attacked_Char->bCrash)
				  { // 충돌한 인물이 있으면
					  lpML->flag=1;
					  g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x + lpEL->tarx, lpEL->y + lpEL->tary);
				  }
				  
				  target_Type = true;
				  break;
			  }
          case EARTH_EXTREME:
          case DARK_EXTREME:
			  {
				  lpML->flag = 1;
				  target_Type = true;
				  break;
			  }
          case FLAME_PILLAR:
		  case ICE_BREATH2:	// 030415 kyo
			  {
				  LPCHARACTER attacked_Char = ReturnMagicCheckedCharacter(0, lpML, lpEL->tarx + EffList[lpEL->sprite_Num].offset_X - nWidth/2, lpEL->tary + EffList[lpEL->sprite_Num].offset_Y - nHeight/2, (int)lpEL->height, nWidth, nHeight/2);
				  
				  if (attacked_Char != NULL && !attacked_Char->bCrash)
				  {
					  InsertMagic(attacked_Char, attacked_Char, lpML->next_Effect, lpML->magicSubNum, 0, 0, attacked_Char->x, attacked_Char->y);
					  lpML->flag = 1;
					  g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, attacked_Char->x, attacked_Char->y);
				  }
				  
				  target_Type = true;
				  break;
			  }
          case TWISTER:
			  {
				  const int nX = lpML->lpChar_Own->x + EffList[lpEL->sprite_Num].offset_X - nWidth/2;
				  const int nY = lpML->lpChar_Own->y + EffList[lpEL->sprite_Num].offset_Y - nHeight/2;
				  LPCHARACTER attacked_Char = ReturnMagicCheckedCharacter(0, lpML, nX, nY, (int)lpEL->height, nWidth, nHeight);
				  
				  if (attacked_Char != NULL)
				  {
					  const int nCount = (g_ClientTime/100)%10;
					  // 초당 두번정도 공격을 함
					  if (nCount == 5)
					  {
						  InsertMagic(attacked_Char, attacked_Char, lpML->next_Effect, lpML->magicSubNum, 0, 0, attacked_Char->x, attacked_Char->y);
						  g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x + lpEL->tarx, lpEL->y + lpEL->tary);
					  }
				  }
				  
				  target_Type = true;
				  break;
			  }
          case LIGHTNING_CLOUD:
			  {
				  LPCHARACTER attacked_Char = ReturnMagicCheckedCharacter(0, lpML, lpML->lpChar_Own->x + lpEL->tarx + EffList[lpEL->sprite_Num].offset_X - nWidth/2, lpML->lpChar_Own->y + lpEL->tary + EffList[lpEL->sprite_Num].offset_Y - nHeight/2, (int)lpEL->height, nWidth, nHeight);
				  
				  if (attacked_Char != NULL) // 충돌한 인물이 있으면
				  {
					  InsertMagic(attacked_Char, attacked_Char, lpML->next_Effect, lpML->magicSubNum, 0, 0, attacked_Char->x, attacked_Char->y);
					  lpML->flag=1;
					  g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x + lpEL->tarx, lpEL->y + lpEL->tary);
					  DeleteList(&(lpML->EffHead), lpEL);
					  lpEL = lpML->EffHead;
					  continue;
				  }
				  
				  lpML->flag = 1;
				  target_Type = false;
				  break;
			  } 
          case TOXIC_CLOUD:
          case METEOR_STRIKE:
          case GREAT_METEOR:
          case LIGHTNING_BOLT:
          case OCTA_LIGHTNING:
          case BLOOD_SPRINKLE:
          case FIRE_BREATH:
          case AREA_PARALYSIS:
          case AREA_POISON:
          case AREA_CONFUSION:
          case AREA_SLOW:
          case AREA_BAT_ATTACK:
          case BLOOD_EARTH:
          case DIVINE_POWER:
          case POISONING_NOVA:
			  {
				  lpML->flag = 1;
				  target_Type = false;
				  break;
			  }
          case HOLY_RAGE:
			  {
				  g_mgrBattle.Display(DISPLAY_EARTHQUAKE);
				  lpML->flag = 1;
				  target_Type = false;
				  break;
			  }
		  case LIGHTNING_EXTREME:
			  {// 030328 kyo
				lpML->flag = 1;
				target_Type = true;
				CollisionEffect_Keeping(lpEL, lpML, 2); // 030519 kyo
				break;
			  } 
		   case AUTHORITY_OF_WHOARENCHA: 	// 030415 kyo 
			  {
				lpML->flag = 1;
				target_Type = true;
				CollisionEffect_Keeping(lpEL, lpML, 2); // 030519 kyo
				break;
			  } 
        } 
		
        int runMagicNum = 0;
        
        switch (lpML->lpChar_Own->sprno)
        {
		case 0:
		case 1:
			{
				//runMagicNum = g_mgrBattle.Return(lpML->magicNum, lpML->lpChar_Own);
				runMagicNum = lpML->magicSerial; 	// 030415 kyo
				break;
			}
		default:
			{
				runMagicNum = lpML->magicNum;
				break;
			}
        }
        // 마법 결과에 대한 답변 요청 (화염벽/얼음벽마법은 제외)
        if(lpML->next_Effect && lpML->link_Frame==lpEL->FrameCount && lpML->magicSerial && lpML->magicSerial!=34 && lpML->magicSerial!=42 && lpML->magicSerial!=45 && lpML->magicSerial!=41 && lpML->magicSerial != 162 && !lpML->failed)
        {
			if (g_lpMT[lpML->magicSerial].magic_Type/10 != 5 && 
				(lpML->magicSerial == runMagicNum || runMagicNum == 215))	//비공격마법이면서 시전의 링크 프레임인 경우
			{
				if (lpML->next_Flag == 0)
				{
					if (g_lpMT[lpML->magicSerial].avail_Type < 2 || 
						(g_lpMT[lpML->magicSerial].avail_Type>=2 && IsHarmfulMagic(lpML->magicSerial)))
					{
						g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Target->id, lpML->tarx, lpML->tary);
					}
					else
					{
						if(lpML->magicSerial )	//지역 공격 마법의 경우
						{ // 폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
							int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
							int check_yl = check_xl*3/4;
							attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
						}
					}

					lpML->next_Flag=1;
				}
			}
        }
        
        if( lpML->link_Frame>0 && lpML->link_Frame==lpEL->FrameCount && !lpML->failed)
        {
			switch (lpML->magicSerial)
			{
            case OCTA_LIGHTNING:
            case BLOOD_SPRINKLE:
            case FIRE_BREATH:
            case AREA_PARALYSIS:
            case AREA_POISON:
            case AREA_CONFUSION:
            case AREA_SLOW:
            case AREA_BAT_ATTACK:
            case LIGHTNING_BOOM:
            case POISONING_NOVA:
            case BLOOD_EARTH:
            case DIVINE_POWER:
				{
					int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
					int check_yl = check_xl*3/4;
					attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 2);
					break;
				}
            case EARTH_EXTREME:
            case DARK_EXTREME:
				{
					int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
					int check_yl = check_xl*3/4;
					attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->tarx - check_xl, lpML->tary - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 2);
					break;
				}
            case TOXIC_CLOUD:
				{
					CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction );
					int check_xl = 3*TILE_SIZE;	// 반경 3타일
					int check_yl = check_xl*3/4;
					attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->tarx - check_xl, lpML->tary - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 2);
					break;
				}
            case HOLY_RAGE:
				{
					CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction );
					int check_xl = 2*TILE_SIZE;	// 반경 3타일
					int check_yl = check_xl*3/4;
					attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->tarx - check_xl, lpML->tary - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 2);
					break;
				}
            case LIGHTNING_BOLT:
            case GREAT_THUNDER_BOLT:
				{
					int check_xl = 2*TILE_SIZE;	// 반경 2타일
					int check_yl = check_xl*3/4;
					attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->tarx - check_xl, lpML->tary - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 2);
					break;
				}
            case METEOR_STRIKE:
            case GREAT_METEOR:
				{
					int check_xl = 4*TILE_SIZE;	// 반경 3타일
					int check_yl = check_xl*3/4;
					attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->tarx - check_xl, lpML->tary - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 2);
					break;
				}
            case FLAME_PILLAR:
            case FLAME_POUR:
				{
					break;
				}
			case ICE_BREATH2:	// 030415 kyo
			case AUTHORITY_OF_WHOARENCHA: 	// 030415 kyo 
			case LIGHTNING_EXTREME: // 030415 kyo
				{
					CharDoAni( lpML->lpChar_Own , lpML->lpChar_Own->direction , lpML->lpChar_Own->basicAction );	  
					break;
				}
            default:
				{
					g_mgrBattle.Effect(lpML);
					break;
				}
			}
        }
		
        switch (lpML->magicSerial)
        {
		case METEOR_STRIKE:
		case RAGE_LIGHT_BALL:
			{
				g_mgrBattle.Display(DISPLAY_EARTHQUAKE);
				break;
			}
        }
        
        if(g_lpET[lpML->magicNum].sound_loop)
        {
			EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
        }
        
        if (lpML->flag == 0 || g_lpET[lpML->magicNum].end_Time == -1)	
        { // 광원 소멸 함수 호출되지 않은 경우
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, 1);
        }
        else
        {
			DrawEffect(lpEL, lpML, 0, target_Type);
        }
        
        lpEL = lpEL->Next;
      }
	  
      break;
	}
	
	return lpML->Next;
}

//시전자 서있는 방향(8방향)으로 휩쓸고 전진하는 불의벽(Wave) - 반드시 종료 조건으로 소스의 마지막 프레임에 다다랐을때 조건을 사용
//만약 사정거리로 종료시키려 한다면 큰일 날거야!

MAGICLIST* Firewall1(MAGICLIST* lpML)
{
	USEEFFECTLIST* lpEL;
	LPITEMGROUND	lpItem;
	
	static int inc = 0;
	int	crash_Result=0;
	static int start_X=0,start_Y=0;
	static int temp_Frame=0;
	
	BOOL StartFlag = TRUE;
	
	//각도 알아내기(마우스 클릭한 좌표가 목표좌표임)
	//ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
	
	lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:
		ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
		lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
		g_mgrBattle.ActAnimation(lpML->lpChar_Own->nCurrentAction, lpML->lpChar_Own);
		ChangeDirection((DIRECTION*)&lpML->dir, lpML->x, lpML->y, lpML->tarx, lpML->tary );
		InsertEffect(lpML);
		start_X=lpML->x;
		start_Y=lpML->y;
		//매직리스트에 마법 처음 등록된 사실 셋팅
		lpML->PCount = 1;
		break;
		
	case 1:		//1개 등록된 경우
		while(1)
		{
			if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				
				buff = lpML->Next;
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);
				
				if (lpEL == NULL) 
					return buff;
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					EndCastMagic(lpML->lpChar_Own);
					g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
				}
			}
			
			/*
			//이팩트가 일정거리 날라가면 소멸
			if( lpEL->incx>=360 || lpEL->incy>=360 )  
			{
			DeleteList(&(lpML->EffHead), lpEL);
			lpEL = lpML->EffHead;
			lpML->EffectCount--;
			lpML->flag=1;				//전체 연결 이팩트의 소멸을 위한 플레그 셋팅
			continue;
			}
			*/
			
			//마지막 프레임까지 진행된 경우 종료시킬까?
			if(lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame )
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				lpML->flag=1;
				lpML->EffectCount--;
				continue;
			}
			//이동불가 지형타일과 충돌시 소멸시킬까?
			if (IsCollision(lpML->magicSerial,
				lpEL->x>>5,
				lpEL->y>>5,
				(lpEL->x + (int)lpEL->incx)>>5, 
				(lpEL->y + (int)lpEL->incy)>>5) == true)
			{
				InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, lpML->next_Effect, 9, lpML->x, lpML->y, (int)(((lpEL->x+lpEL->incx)/32.)*32.+16), (int)(((lpEL->y+lpEL->incy)/32.)*32.+16) );
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				lpML->flag=1;
				lpML->EffectCount--;
				continue;
			}
			
			int check_xl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
			int check_yl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
			
			attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (int)((lpEL->x+lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2)), (int)((lpEL->y+lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-check_yl), (int)lpEL->height, check_xl, check_yl );
			//< CSD-021104
			//if( attacked_Char && lpML->lpChar_Target->height==0 && !(attacked_Char->nCurrentAction == MON1_ATTACKED || attacked_Char->nCurrentAction == ACTION_ATTACKED)) //충돌한 인물이 있으면
			if (attacked_Char && lpML->lpChar_Target->height == 0) //충돌한 인물이 있으면
				//> CSD-021104
			{
				if( lpEL && lpML->magicSerial )
				{
					g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x+(int)(lpEL->incx+.5), lpEL->y+(int)(lpEL->incy+.5));
				}
				
				InsertMagic(lpML->lpChar_Own,attacked_Char,lpML->next_Effect, 9, 0, 0,attacked_Char->x,attacked_Char->y);
				
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				lpML->flag=1;
				//lpML->EffectCount--;
				continue;
			}
			
			else
			{
				lpItem=ReturnMagicCheckedItem(0, lpML, (int)((lpEL->x+lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2.)), (int)((lpEL->y+lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-(check_yl/2.)), (int)lpEL->height, check_xl, check_yl);
				if(lpItem!=NULL)
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->flag=1;
					continue;
				}
			}
			
			if (lpEL->Next == NULL)
			{
				break;
			}
			else lpEL = lpEL->Next;
			}
			
			lpEL = lpML->EffHead;
			temp_Frame=lpEL->FrameCount;
			
			while(1)
			{
				if (lpEL == NULL) break;
				
				switch(lpML->dir)
				{
				case 0:	//down
					lpEL->incy += lpEL->speed;
					break;
				case 1:	//leftdwon
					lpEL->incx -= (int)(lpEL->speed*3/4);
					lpEL->incy += (int)(lpEL->speed*3/4);
					break;
				case 2:	//left
					lpEL->incx -= lpEL->speed;
					break;
				case 3:	//leftup
					lpEL->incx -= (int)(lpEL->speed*3/4);
					lpEL->incy -= (int)(lpEL->speed*3/4);
					break;
				case 4:	//up
					lpEL->incy -= lpEL->speed;
					break;
				case 5:	//rightup
					lpEL->incx += (int)(lpEL->speed*3/4);
					lpEL->incy -= (int)(lpEL->speed*3/4);
					break;
				case 6:	//right
					lpEL->incx += lpEL->speed;
					break;
				case 7:	//rightdown
					lpEL->incx += (int)(lpEL->speed*3/4);
					lpEL->incy += (int)(lpEL->speed*3/4);
					break;
				}
				
				if((inc%2 == 0) && (lpML->EffectCount < lpML->object_Num)) 				
				{
					if(lpML->EffectCount == 0 /*temp_Frame==2*/ && lpML->flag==0)	// 2 Step인 경우
					{
						lpML->next_Flag=1;
						switch(lpML->dir)
						{
						case 0:
							lpML->x = start_X-48;
							lpML->y = start_Y+lpEL->speed*3;
							InsertEffect(lpML);
							lpML->x = start_X;
							InsertEffect(lpML);
							lpML->x = start_X+48;
							InsertEffect(lpML);
							break;
						case 1:
							lpML->x = (int)(start_X-lpEL->speed*2)-36;
							lpML->y = (int)(start_Y+lpEL->speed*2)-36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*2);
							lpML->y = (int)(start_Y+lpEL->speed*2);
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*2)+36;
							lpML->y = (int)(start_Y+lpEL->speed*2)+36;
							InsertEffect(lpML);
							break;
						case 2:
							lpML->x = start_X-lpEL->speed*3;
							lpML->y = start_Y-48;
							InsertEffect(lpML);
							lpML->y = start_Y;
							InsertEffect(lpML);
							//lpML->x = start_X;
							lpML->y = start_Y+48;
							InsertEffect(lpML);
							break;
						case 3:
							lpML->x = (int)(start_X-lpEL->speed*2)-36;
							lpML->y = (int)(start_Y-lpEL->speed*2)+36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*2);
							lpML->y = (int)(start_Y-lpEL->speed*2);
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*2)+36;
							lpML->y = (int)(start_Y-lpEL->speed*2)-36;
							InsertEffect(lpML);
							break;
						case 4:
							lpML->x = start_X-48;
							lpML->y = start_Y-lpEL->speed*3;
							InsertEffect(lpML);
							lpML->x = start_X;
							InsertEffect(lpML);
							lpML->x = start_X+48;
							//lpML->y = start_Y;
							InsertEffect(lpML);
							break;
						case 5:
							lpML->x = (int)(start_X+lpEL->speed*2)-36;
							lpML->y = (int)(start_Y-lpEL->speed*2)-36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*2);
							lpML->y = (int)(start_Y-lpEL->speed*2);
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*2)+36;
							lpML->y = (int)(start_Y-lpEL->speed*2)+36;
							InsertEffect(lpML);
							break;
						case 6:
							lpML->x = start_X+lpEL->speed*3;
							lpML->y = start_Y-48;
							InsertEffect(lpML);
							lpML->y = start_Y;
							InsertEffect(lpML);
							//lpML->x = start_X;
							lpML->y = start_Y+48;
							InsertEffect(lpML);
							break;
						case 7:
							lpML->x = (int)(start_X+lpEL->speed*2)+36;
							lpML->y = (int)(start_Y+lpEL->speed*2)-36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*2);
							lpML->y = (int)(start_Y+lpEL->speed*2);
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*2)-36;
							lpML->y = (int)(start_Y+lpEL->speed*2)+36;
							InsertEffect(lpML);
							break;
						}
						lpML->EffectCount ++;
					}
					
					else if(lpML->EffectCount==1/*temp_Frame==3*/ && lpML->next_Flag==1 && lpML->flag==0)	//3 Step 이상인 경우
					{
						switch(lpML->dir)
						{
						case 0:
							lpML->x = start_X-96;
							lpML->y = start_Y+lpEL->speed*6;
							InsertEffect(lpML);
							lpML->x = start_X-48;
							InsertEffect(lpML);	
							lpML->x = start_X;
							InsertEffect(lpML);
							lpML->x = start_X+48;
							InsertEffect(lpML);
							lpML->x = start_X+96;
							InsertEffect(lpML);
							break;
						case 1:
							lpML->x = (int)(start_X-lpEL->speed*4)-72;
							lpML->y = (int)(start_Y+lpEL->speed*4)-72;
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*4)-36;
							lpML->y = (int)(start_Y+lpEL->speed*4)-36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*4);
							lpML->y = (int)(start_Y+lpEL->speed*4);
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*4)+36;
							lpML->y = (int)(start_Y+lpEL->speed*4)+36;
							InsertEffect(lpML);	
							lpML->x = (int)(start_X-lpEL->speed*4)+72;
							lpML->y = (int)(start_Y+lpEL->speed*4)+72;
							InsertEffect(lpML);
							break;
						case 2:
							lpML->x = start_X-lpEL->speed*6;
							lpML->y = start_Y-96;
							InsertEffect(lpML);
							lpML->y = start_Y-48;
							InsertEffect(lpML);
							lpML->y = start_Y;
							InsertEffect(lpML);
							lpML->y = start_Y+48;
							InsertEffect(lpML);
							lpML->y = start_Y+96;
							InsertEffect(lpML);
							break;
						case 3:
							lpML->x = (int)(start_X-lpEL->speed*4)+72;
							lpML->y = (int)(start_Y-lpEL->speed*4)-72;
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*4)+36;
							lpML->y = (int)(start_Y-lpEL->speed*4)-36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*4);
							lpML->y = (int)(start_Y-lpEL->speed*4);
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*4)-36;
							lpML->y = (int)(start_Y-lpEL->speed*4)+36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X-lpEL->speed*4)-72;
							lpML->y = (int)(start_Y-lpEL->speed*4)+72;
							InsertEffect(lpML);
							break;
						case 4:
							lpML->x = start_X-96;
							lpML->y = start_Y-lpEL->speed*6;
							InsertEffect(lpML);
							lpML->x = start_X-48;
							InsertEffect(lpML);
							lpML->x = start_X;
							InsertEffect(lpML);
							lpML->x = start_X+48;
							InsertEffect(lpML);
							lpML->x = start_X+96;
							InsertEffect(lpML);
							break;
						case 5:
							lpML->x = (int)(start_X+lpEL->speed*4)-72;
							lpML->y = (int)(start_Y-lpEL->speed*4)-72;
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*4)-36;
							lpML->y = (int)(start_Y-lpEL->speed*4)-36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*4);
							lpML->y = (int)(start_Y-lpEL->speed*4);
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*4)+36;
							lpML->y = (int)(start_Y-lpEL->speed*4)+36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*4)+72;
							lpML->y = (int)(start_Y-lpEL->speed*4)+72;
							InsertEffect(lpML);
							break;
						case 6:
							lpML->x = start_X+lpEL->speed*6;
							lpML->y = start_Y-96;
							InsertEffect(lpML);
							lpML->y = start_Y-48;
							InsertEffect(lpML);
							lpML->y = start_Y;
							InsertEffect(lpML);
							lpML->y = start_Y+48;
							InsertEffect(lpML);
							lpML->y = start_Y+96;
							InsertEffect(lpML);
							break;
						case 7:
							lpML->x = (int)(start_X+lpEL->speed*4)+72;
							lpML->y = (int)(start_Y+lpEL->speed*4)-72;
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*4)+36;
							lpML->y = (int)(start_Y+lpEL->speed*4)-36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*4);
							lpML->y = (int)(start_Y+lpEL->speed*4);
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*4)-36;
							lpML->y = (int)(start_Y+lpEL->speed*4)+36;
							InsertEffect(lpML);
							lpML->x = (int)(start_X+lpEL->speed*4)-72;
							lpML->y = (int)(start_Y+lpEL->speed*4)+72;
							InsertEffect(lpML);
							break;
						}
						lpML->EffectCount ++;
					}
					//StartFlag = FALSE;
				}
				
				lpEL->trans_Level=((lpML->object_Num+2-lpML->EffectCount)*(lpML->trans_Level/(float)(lpML->object_Num+1)));//*(float)((EffList[lpML->sprite_Num].MaxFrame-lpEL->FrameCount)/EffList[lpML->sprite_Num].MaxFrame);
				if(lpEL->trans_Level>=32)	lpEL->trans_Level=32;
				if(lpEL->trans_Level<=2)	lpEL->trans_Level=2;
				
				DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
				
				inc++;
				if(inc>1000)	inc=0;
				
				if (lpEL->Next == NULL)
					break;
				else lpEL = lpEL->Next;			
			}
			break;
	}
	return lpML->Next;
}

MAGICLIST*	FishJump(MAGICLIST*	lpML)
{
	USEEFFECTLIST*  lpEL;
	
	lpEL=lpML->EffHead;
	
	switch(lpML->PCount)
	{
	case 0:
		InsertEffect(lpML);	
		//효과리스트에 마법 하나 등록된 사실 셋팅
		
		lpML->PCount = 1;
		break;
		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거
		{
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				
				if (lpEL == NULL)
				{
					return buff;
				}
			}
			
			if( lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame/2 )
			{
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////////////////				
			
			if (lpEL->Next == NULL)			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				break;
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
		}
		
		lpEL = lpML->EffHead;
		
		while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
		{
			//동시 출력하는 이팩트의 갯수가 복수일 때 
			if(lpML->EffectCount < lpML->object_Num-1)
			{
				InsertEffect(lpML);
				lpML->EffectCount ++;
			}
			
			//연결프레임과 현재 프레임 동일하면
			if( lpML->link_Frame!=0 && lpML->link_Frame==lpEL->FrameCount)
			{
				//시전자의 방향과 액션 형태를 조정				
				if(lpML->magicSubNum==8)	InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,9,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);
				else							InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);	
			}
			
			if(lpML->trans_Type%10==8)		//점점 밝게 생성 / 마지막 어둡게
			{
				if(lpEL->FrameCount<EffList[lpEL->sprite_Num].MaxFrame-4)
				{
					lpEL->trans_Level=(lpML->trans_Level/(float)EffList[lpEL->sprite_Num].MaxFrame)*lpEL->FrameCount;
					if(lpEL->trans_Level > lpML->trans_Level)	lpEL->trans_Level=(float)lpML->trans_Level;
				}
				else
				{
					lpEL->trans_Level-=(float)(lpML->trans_Level/4.f);
					if(lpEL->trans_Level < 1) lpEL->trans_Level = 1;
				}
			}
			
			// 마지막 4프레임은 alpha off
			else if(lpML->trans_Type%10==9 && lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame-4)	
			{
				lpEL->trans_Level-=(lpML->trans_Level/4);//-((lpML->trans_Level/EffList[lpEL->sprite_Num].MaxFrame)*(lpEL->FrameCount-1));
				if(lpEL->trans_Level<=1)	lpEL->trans_Level=1;
			}
			
			else if(lpML->trans_Type%10==7)	//바닥이 얼어붙는 효과
			{
				if(lpEL->FrameCount<=EffList[lpML->sprite_Num].MaxFrame/2)
				{
					lpEL->trans_Level=(float)lpEL->FrameCount;
					if(lpEL->trans_Level>=32)	lpEL->trans_Level=32;
				}
				else
				{
					lpEL->trans_Level=(float)(EffList[lpML->sprite_Num].MaxFrame-lpEL->FrameCount);
					if(lpEL->trans_Level<0) lpEL->trans_Level=0;
				}
			}
			
			else if(g_lpET[lpML->magicNum].end_Time!=0)			//쉴드와 같은 장시간 유지 마법의 경우 색깔값 지속적 떨어트림
			{
				lpEL->trans_Level=(float)(lpML->trans_Level*(g_lpET[lpML->magicNum].end_Time-lpEL->total_Frame)/g_lpET[lpML->magicNum].end_Time);
				if(lpEL->trans_Level<0)	lpEL->trans_Level=0;
			}
			
			// 실제 출력
			switch(lpML->magicSubNum)	//왼쪽으로 튀어 올라가기
			{
			case 2:
				lpEL->incx+=fish_Point_Pattern1[lpEL->FrameCount][1];
				lpEL->incy+=fish_Point_Pattern1[lpEL->FrameCount][2];
				break;
			case 3:
				lpEL->incx+=fish_Point_Pattern2[lpEL->FrameCount][1];
				lpEL->incy+=fish_Point_Pattern2[lpEL->FrameCount][2];
				break;
			case 4:
				lpEL->incx-=fish_Point_Pattern1[lpEL->FrameCount][1];
				lpEL->incy+=fish_Point_Pattern1[lpEL->FrameCount][2];
				break;
			case 5:
				lpEL->incx+=fish_Point_Pattern1[lpEL->FrameCount][1];
				lpEL->incy-=fish_Point_Pattern1[lpEL->FrameCount][2];
				break;
			case 6:
				lpEL->incx-=fish_Point_Pattern2[lpEL->FrameCount][1];
				lpEL->incy+=fish_Point_Pattern2[lpEL->FrameCount][2];
				break;
			case 7:
				lpEL->incx+=fish_Point_Pattern2[lpEL->FrameCount][1];
				lpEL->incy-=fish_Point_Pattern2[lpEL->FrameCount][2];
				break;	
			}
			
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, 0);
			
			lpEL = lpEL->Next;			
		}
		break;
	}
	return lpML->Next;
}

MAGICLIST* Homing(MAGICLIST* lpML)//, int Num, int motion_Num, int max_Object, int speed, int height, int type, int levlpEL, int end_Frame)		//스프라이트의 고유번호
{
	USEEFFECTLIST* lpEL;
	
	int			crash_Result=0;
	float		two_Point_Angle=0.;		//발사체와 목표가 이루는 각도(라디안)
	int			type_Angle=0;
	float		plus_Angle=0.;
	float		distance_Digree=0.;
	float		anti_Point_Angle=0.;
	float		test=0.;
	
	BOOL StartFlag = TRUE;
	
	
	lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:				
		ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
		lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
		CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->nCurrentAction);
		//CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_MAGIC_CASTING);
		InsertEffect(lpML);
		lpML->PCount = 1;
		break;
		
	case 1:		//1개 등록된 경우
		while(1)
		{
			if (lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				buff = lpML->Next;
				
				if( lpML->lpChar_Own->changeActionFlag == 1 ) 
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				CharDoAni( lpML->lpChar_Own , lpML->lpChar_Own->direction , lpML->lpChar_Own->basicAction );
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);
				
				if (lpEL == NULL) 
					return buff;
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					EndCastMagic(lpML->lpChar_Own);
					CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_NONE); 
				}
			}
			
			//좌표가 벋어 났을 경우
			if (abs(lpML->x-lpEL->x) >=SCREEN_WIDTH || abs(lpML->y-lpEL->y) >=SCREEN_WIDTH)
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			//이동불가 지형타일과 충돌시 소멸시킬까?
			if (IsCollision(lpML->magicSerial,
				lpEL->x>>5,
				lpEL->y>>5,
				(lpEL->x + (int)lpEL->incx)>>5, 
				(lpEL->y + (int)lpEL->incy)>>5) == true)
			{
				if(!(lpML->next_Effect==181 || lpML->next_Effect==191 || lpML->next_Effect==170))	//피(순수타격이팩트)를 제외한 다른 타격 이팩트 배경에 출력
				{
					InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target, lpML->next_Effect,  9,  0,  0,  (int)(((lpEL->x+lpEL->oldincx)/32.)*32.+16), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+16) );
				}
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				lpML->flag=1;
				continue;
			}
			
			if(lpEL->total_Frame>=80 )
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			int check_xl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅 
			int check_yl = Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl;
			
			//발사 이펙트가 적 케릭터에 맞았는지 체크
			attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (int)((lpEL->x+lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2)), (int)((lpEL->y+lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-(check_xl/2)), (int)lpEL->height, check_xl, check_yl );
			//< CSD-021104
			//if( attacked_Char && !(attacked_Char->nCurrentAction == MON1_ATTACKED || attacked_Char->nCurrentAction == ACTION_ATTACKED)) //충돌한 인물이 있으면
			if (attacked_Char) //충돌한 인물이 있으면
				//> CSD-021104
			{
				if(lpEL )
				{
					g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, attacked_Char->id, lpEL->x+(int)(lpEL->incx+.5), lpEL->y+(int)(lpEL->incy+.5) );
				}
				
				InsertMagic(lpML->lpChar_Own,attacked_Char, lpML->next_Effect, 9, 0, 0,attacked_Char->x,attacked_Char->y);
				
				
				lpML->flag=1;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				lpML->EffectCount--;
				
				continue;
			}
			
			if (lpEL->Next == NULL)
			{
				break;
			}
			else lpEL = lpEL->Next;
		}
		
		lpEL = lpML->EffHead;
		
		//가속처리하지 않음->유도 루틴
		
		/////////////////////////// 발사체와 목표가 이루는 각도 산출 ////////////////////////////////
		if(  lpEL->x <= lpEL->tarx  &&  lpEL->y >= lpEL->tary )		//0~90 영역
		{
			two_Point_Angle =(float)(atan( (float)(lpEL->tarx-lpEL->x)/(float)(lpEL->y-lpEL->tary) )*180/M_PI);
		}
		
		if(  lpEL->x <  lpEL->tarx  &&  lpEL->y <  lpEL->tary )		//90~180 영역	
		{
			two_Point_Angle = (float)(atan( (float)(lpEL->tary-lpEL->y)/(float)(lpEL->tarx-lpEL->x) )*180/M_PI+90);
		}
		
		if(  lpEL->x >= lpEL->tarx  &&  lpEL->y <= lpEL->tary )		//180~270 영역
		{
			two_Point_Angle = (float)(atan( (float)(lpEL->x-lpEL->tarx)/(float)(lpEL->tary-lpEL->y) )*180/M_PI+180);
		}
		
		if(	 lpEL->x >  lpEL->tarx  &&  lpEL->y >  lpEL->tary )		//270~360 영역
		{
			two_Point_Angle = (float)(atan( (float)(lpEL->y-lpEL->tary)/(float)(lpEL->x-lpEL->tarx) )*180/M_PI+270);
		}
		///////////////////////////////////////////////////////////////////////////////////////////////
		
		////////////////////////////////// 1105 /////////////////////////////
		if(lpEL->flag==0)
			lpEL->first_Angle=(float)(((int)two_Point_Angle+180)%360);
		/////////////////////////////////////////////////////////////////////
		
		////////////////////////// 각도 접근방식과 두각의 차이 산출 ///////////////////////////////////
		distance_Digree=(float)(abs((int)(two_Point_Angle - lpEL->first_Angle)));
		
		if( lpEL->flag==0 )	//처음 발사시만
		{
			if( rand()%2==0 )	type_Angle=-1;
			else				type_Angle=1;
		}
		else if( two_Point_Angle>0 && two_Point_Angle<180)	
		{
			if(lpEL->first_Angle>=0 && lpEL->first_Angle<=180)
			{ 
				if(two_Point_Angle>=lpEL->first_Angle)		type_Angle=1;											//접근각의 부호
				else										type_Angle=-1;
			}
			else
			{
				if(lpEL->first_Angle-two_Point_Angle>=180)	type_Angle=1;
				else										type_Angle=-1;
			}
		}
		else		
		{
			if(lpEL->first_Angle>=180 && lpEL->first_Angle<=360)
			{
				if(two_Point_Angle>=lpEL->first_Angle)		type_Angle=1;
				else										type_Angle=-1;
			}
			else	
			{
				if(two_Point_Angle-lpEL->first_Angle<=180)	type_Angle=1;
				else										type_Angle=-1;
			}
		}
		
		if( distance_Digree > 120 )
			lpEL->first_Angle+=type_Angle*18;
		else if( distance_Digree > 60 )
			lpEL->first_Angle+=type_Angle*14;
		else if( distance_Digree > 10 )
			lpEL->first_Angle+=type_Angle*8;
		else if( distance_Digree >5 )
			lpEL->first_Angle+=type_Angle*4;
		else
		{
			lpEL->speed+=(lpEL->speed/6);
			if(lpEL->speed>48)	lpEL->speed=48;
		}
		
		if(lpEL->first_Angle>=360)		lpEL->first_Angle-=360;
		if(lpEL->first_Angle<0)			lpEL->first_Angle+=360;
		
		lpEL->x+=(int)((float)(lpEL->speed)*cos((lpEL->first_Angle-90)*M_PI/180.));
		lpEL->y+=(int)((float)(lpEL->speed)*sin((lpEL->first_Angle-90)*M_PI/180.));
		
		lpEL->flag=1;
		
		while(1)
		{
			if (lpEL == NULL) break;
			
			lpEL->total_Frame++;
			
			float temp_incx = lpEL->incx;
			lpEL->incx=	lpEL->oldincx;
			float temp_incy = lpEL->incy;
			lpEL->incy=	lpEL->oldincy;
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light);
			lpEL->incx=temp_incx;
			lpEL->incy=temp_incy;
			
			if (lpEL->Next == NULL)
				break;
			else lpEL = lpEL->Next;			
		}
		break;
	}
	return lpML->Next;
}

//차단 벽(화염벽/얼음벽) 생성 마법
MAGICLIST*	WallCreate(MAGICLIST*	lpML)
{
	USEEFFECTLIST*  lpEL;
	
	int			terminateFrame=0;		//광원의 소멸을 시작하는 프레임
	DWORD		check_Time=0;
	
	int			crash_Result=0;
	bool		target_Type=0;			//시전자에게 찍을까?(0) 대상체에게 찍을까?(1)
	bool		reset_Map=0;
	int			move_X=0,move_Y=0;
	int			magic_Ability=0;		//시전자의 마법구사 능력
	int			evil_Eye_Delay=0;		//성직자 악으로부터 보호마법의 지속시간 처리 위해
	
	if(lpML->magicSubNum==9)	target_Type=1;
	
	// 0221 동시 출력하는 이팩트의 갯수는 시전자의 마법 능력치에 따라 정해짐 (맞는지 확인 요망)
	magic_Ability = (int)( ( (lpML->lpChar_Own->_Int/400.f) + (lpML->lpChar_Own->_Wis/400.f) + (lpML->lpChar_Own->_wsps/500.f) ) *30.f );		// MIN:0 ~ MAX:90
	lpML->object_Num = (int)(5+(magic_Ability/20.));
	if(lpML->object_Num > 10)	lpML->object_Num=10;
	
	lpEL=lpML->EffHead;
	
	switch(lpML->PCount)
	{
	case 0:
		if(lpML->end_Time<0 && lpML->next_Effect==0)		//34
			lpML->magic_Runtime		= (DWORD)(( g_lpMT[lpML->magicSerial].continue_Time + (g_lpMT[lpML->magicSerial].continue_Time*magic_Ability/100.f) ) * 1000.f) + g_ClientTime;		//종료될 시간
								
		// 각 방향에 따른 다음 링크 이펙트의 수정 좌표 셋팅
		switch(int(lpML->lpChar_Own->direction))
		{
		case 0:
			lpML->tarx += (int)((lpML->object_Num/2.)*TILE_SIZE);
			break;
		case 1:
			lpML->tarx += (int)((lpML->object_Num/2.)*TILE_SIZE*.6);
			lpML->tary += (int)((lpML->object_Num/2.)*TILE_SIZE*.6);
			break;
		case 2:
			lpML->tary += (int)((lpML->object_Num/2.)*TILE_SIZE);
			break;
		case 3:
			lpML->tarx -= (int)((lpML->object_Num/2.)*TILE_SIZE*.6);
			lpML->tary += (int)((lpML->object_Num/2.)*TILE_SIZE*.6);
			break;
		case 4:
			lpML->tarx -= (int)((lpML->object_Num/2.)*TILE_SIZE);
			break;
		case 5:
			lpML->tarx -= (int)((lpML->object_Num/2.)*TILE_SIZE*.6);
			lpML->tary -= (int)((lpML->object_Num/2.)*TILE_SIZE*.6);
			break;
		case 6:
			lpML->tary -= (int)((lpML->object_Num/2.)*TILE_SIZE);
			break;
		case 7:
			lpML->tarx += (int)((lpML->object_Num/2.)*TILE_SIZE*.6);
			lpML->tary -= (int)((lpML->object_Num/2.)*TILE_SIZE*.6);
			break;
		}
		
		InsertEffect(lpML);
		//효과리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 1;
		break;
		
		case 1:
			while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거
			{
				if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
				{
					MAGICLIST* buff;
					buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
					
					lpML->EffectCount = 0;
					lpML->magic_Runtime		= 0;
					DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
					
					//if(lpML!=NULL || lpML->EffectCount!=NULL)
					//	lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
					
					if (lpEL == NULL)
					{
						return buff;
					}
				}
				
				if( lpML->lpChar_Own == NULL || lpML->lpChar_Target == NULL )
				{
					MAGICLIST* buff;
					buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
					
					lpML->EffectCount = 0;
					DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
					
					//if(lpML!=NULL || lpML->EffectCount!=NULL)
					//	lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
					
					if (lpEL == NULL)
					{
						return buff;
					}
				}
				
				TILE	temp_tile = TileMap[(lpEL->tarx + EffList[lpEL->sprite_Num].offset_X)/32][(lpEL->tary + EffList[lpEL->sprite_Num].offset_Y)/32];
				//////////////////////////// 0607 lkh 벽 생성시 돈트이거나 내부 속성인 경우는 제외 /////////////////
				if((temp_tile.attr_dont || temp_tile.attr_inside) && lpEL->FrameCount > 2 )
				{
					DeleteList(&(lpML->EffHead), lpEL);	//이펙트 링크드 리스트에서 제거
					lpEL = lpML->EffHead;
					continue;
				}
				
				// 종료 조건 확인하여 링크드 리스트에서 제외
				if( g_lpET[lpML->magicNum].end_Time != 0)		//특정 유지시간을 가지고 있는 경우
				{
					if( lpML->magic_Runtime <= g_ClientTime )
					{ 
						lpEL->FrameCount=0;
						DeleteList(&(lpML->EffHead), lpEL);
						
						lpEL = lpML->EffHead;
						continue;
					}
				}
				
				// 1번 루프 효과의 경우 마지막 프레임에 도달하거나 8방향 구분 효과의 경우 해당 방향의 마지막 프레임에 도달한 경우
				else if( ( lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame ) || ( lpML->magicSubNum==4 && lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame/8 ))
				{
					if(lpML->next_Effect==0 || g_lpET[lpML->magicSerial].impect_Effect_Num==0)
					{
						lpML->lpChar_Own->changeActionFlag = 0;
						g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					}
					lpEL->FrameCount=0;
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}
				
				// 충돌처리 루틴(바닥 정사각형의 타일 개념만 체크)
				int check_xl = (int)Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].xl;		//효과 스프라이트의 크기에 따른 차별적 충돌영역 셋팅
				int check_yl = check_xl;														//(int)(Effspr[lpML->sprite_Num].EffSpr[lpEL->FrameCount].yl*.8);
				
				if( check_xl < 64 )	check_xl=64;
				if( check_yl < 64 ) check_yl=64;
				
				int magicNum = lpML->magicSerial;
				
				///////////////////////// 0724 lkh 수정 ///////////////////////////
				if (magicNum == FIRE_WALL || magicNum == ICE_WALL)
				{	//< CSD-030306
					if (lpEL->total_Frame%18 == 0 && rand()%2 == 0)
					{
						//화염벽 이펙트에 적 케릭터가 다았는지 체크(첫번째인자 1인 경우->복수 타일 체크 & 충돌처리를 함수에 맞김/마지막 바로전 인자->내 자신(시전자) 체크여부/마지막 인자->공중적 체크 여부)
						attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (int)((lpEL->tarx + EffList[lpEL->sprite_Num].offset_X)-(check_xl/2.)), (int)((lpEL->tary + EffList[lpEL->sprite_Num].offset_Y)-(check_yl/2.)), (int)lpEL->height, check_xl, check_yl, 1);
						if(attacked_Char)			//충돌한 인물이 있으면
						{	
							if (lpEL)
							{
								g_mgrBattle.SendMagicResult(magicNum, lpML->lpChar_Own, attacked_Char->id, (int)(lpEL->x+lpEL->incx+.5), (int)(lpEL->y+lpEL->incy+.5));
							}
						
							InsertMagic(lpML->lpChar_Own, attacked_Char, g_lpET[lpML->magicSerial].impect_Effect_Num, 9, 0, 0, attacked_Char->x, attacked_Char->y - attacked_Char->height);
						}	
					}
				}	//> CSD-030306
				
				if (lpEL->Next == NULL )		//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				{
					break;
				}
				else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
			}
			
			lpEL = lpML->EffHead;
			
			while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
			{
				lpEL->total_Frame++;
				
				if(lpEL->total_Frame%3 == 2)
				{
					//동시 출력하는 이팩트의 갯수는 시전자의 마법 능력치에 따라 정해짐
					if(lpML->EffectCount < lpML->object_Num-1)
					{
						switch(lpML->dir)
						{
						case 0:
							lpML->tarx -= 32;
							break;
						case 1:
							lpML->tarx -= 20;
							lpML->tary -= 20;
							break;
						case 2:
							lpML->tary -= 32;
							break;
						case 3:
							lpML->tarx += 20;
							lpML->tary -= 20;
							break;
						case 4:
							lpML->tarx += 32;
							break;
						case 5:
							lpML->tarx += 20;
							lpML->tary += 20;
							break;
						case 6:
							lpML->tary += 32;
							break;
						case 7:
							lpML->tarx -= 20;
							lpML->tary += 20;
							break;
						}
						
						InsertEffect(lpML);
						lpML->EffectCount ++;
					}
					else if(lpML->flag == 0)
					{
						lpML->flag = 1;
						lpML->lpChar_Own->changeActionFlag = 0;
						g_mgrBattle.ActAnimation(lpML->lpChar_Own->basicAction, lpML->lpChar_Own);
					}
				}
				
				int runMagicNum = g_mgrBattle.Return(lpML->magicNum, lpML->lpChar_Own);
				float temp_Time = (g_lpMT[lpML->magicSerial].continue_Time + (g_lpMT[lpML->magicSerial].continue_Time*magic_Ability/100.f) )*1000.f;
				if(g_lpET[lpML->magicNum].end_Time<0)			//Magic Table에 정의된 유지 시간동안 지속되는 마법의 색깔값 변환
				{
					lpEL->trans_Level = ((lpML->magic_Runtime - g_ClientTime) / temp_Time)*lpML->trans_Level + 1;
					if(lpEL->trans_Level>31)	lpEL->trans_Level=31;
					if(lpEL->trans_Level<2)		lpEL->trans_Level=2;
				}
				
				////////////////// 케릭터의 높이값에 따라 가감치 더해주기 //////////////////
				//int  change_Height=(lpML->lpChar_Own->sp->yl)-(lpML->lpChar_Target->sp->yl);
				//lpEL->y=lpML->y+change_Height;
				//lpEL->t_X=lpML->t_Y+change_Height;
				////////////////////////////////////////////////////////////////////////////
				if(g_lpET[lpML->magicNum].sound_loop!=0)
					EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
				
				if(lpML->flag==0 || g_lpET[lpML->magicNum].end_Time==-1)	//광원 소멸 함수 호출되지 않은 경우
				{
					if(g_lpET[lpML->magicNum].end_Time==0)
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, target_Type);
					else
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light/100*100+(int)( g_lpET[lpML->magicNum].light%100*((lpML->magic_Runtime+5000 - g_ClientTime) / (g_lpMT[lpML->magicSerial].continue_Time*5000.))+.5), target_Type);
				}
				else
					DrawEffect(lpEL, lpML, 0, target_Type);
				
				lpEL = lpEL->Next;			
			}
			
			break;
	}
	return lpML->Next;
}

MAGICLIST* CastingMagic(MAGICLIST* lpML)
{
	int			illusion_Trans=0;
	int			terminateFrame=0;
	int			free_On=0;
	DWORD		check_Time=0;
	int			crash_Result=0;
	bool		target_Type=0;			//시전자에게 찍을까?(0) 대상체에게 찍을까?(1)
	bool		reset_Map=0;
	int			move_X=0,move_Y=0;
	int			wave = 0;
	USEEFFECTLIST* lpEL=lpML->EffHead;
	
	switch(lpML->PCount)
	{
	case 0:
		{ 
			if (g_mgrBattle.IsMagic(lpML->magicSerial))
			{
				wave = ReturnRoonWord(lpML->lpChar_Own, lpML->magicSerial);
				if( lpML->lpChar_Own != Hero ) // LTS SOUND
				{
					int x=(lpML->lpChar_Own->x-Hero->x)/*/TILE_SIZE*/;		// LTS SOUND
					int y=(lpML->lpChar_Own->y-Hero->y)/*/TILE_SIZE*/;		// LTS SOUND
					PlayListAutoSounds(wave,x,y,0);	
				}
				else 
				{
					PlayListAutoSounds(wave,0,0,0);	// LTS SOUND
				}
				CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_MAGIC_BEFORE);
			}
			
			if (g_mgrBattle.IsCombat(lpML->magicSerial))
			{
				CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->nCurrentAction);
			}
			lpML->lpChar_Own->changeActionFlag=0;
			InsertEffect(lpML);	
			lpML->PCount = 1;
		}break;
	case 1:
		{
			while (true)
			{
				if (g_mgrBattle.IsMagic(lpML->magicSerial))
				{
					if ((TimeCheck(lpML->lpChar_Own->end_Casting)) && lpML->lpChar_Own->ready_Magic == 1)
					{ // 현 캐스팅 단계를 종료하고 마법발사준비상태로의 전이
						lpML->lpChar_Own->ready_Magic = 2;
						lpML->lpChar_Own->end_Casting = 0;
						lpML->lpChar_Own->end_ReadyMagic = g_ClientTime + (g_lpMT[lpML->magicSerial].cast_Continue/6)*1000;
						lpEL->flag = 1;

						switch (lpML->lpChar_Own->excute_MagicNum)
						{	//< CSD-031014
						case FIRE_ARROW: lpML->lpChar_Own->bow_Type = 1;  break;
						case ICE_ARROW:  lpML->lpChar_Own->bow_Type = 2;  break;
						case THROW_ARROW: 
						case THROW_FIRE_ARROW: lpML->lpChar_Own->bow_Type = 0; break;  
						case THROW_KNIFE: 
						case THROW_ICE_KNIFE: lpML->lpChar_Own->bow_Type = 3; break;
						case THROW_SNOW_BALL: 
						case THROW_WATER_BALL:
						case THROW_BIG_GULP: lpML->lpChar_Own->bow_Type = 3;  break;
						}	//> CSD-1014

						InsertMagic(lpML->lpChar_Own, lpML->lpChar_Own, lpML->next_Effect, 0, 0, 0, 0, 0);
					}
				}
				// MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
				if (lpML->EffHead == NULL) 
				{
					MAGICLIST* buff;
					buff = lpML->Next;				 // 다음 매직리스트링트의 주소값을 임시 기억
					lpML->EffectCount = 0;
					DeleteList(&g_lpML, lpML); // 실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
					if (lpEL == NULL)	 return buff;
				}
				
				 
				//if(lpML->lpChar_Own->ready_Magic!=1)
				//{
				//DeleteList(&(lpML->EffHead), lpEL);
				//	lpEL = lpML->EffHead;
				//	continue;
				//}

				if(lpML->lpChar_Own->ready_Magic==2 )  // 030314 kyo
				{
 					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}

				if(lpML->lpChar_Own->ready_Magic==0)  // 030314 kyo
				{//마법 케스팅중에 멈춰버리면 캐스팅 동작(움직일수 없다)에서 멈추기 때문에 포츠를 기본으로 설정한다.
					DeleteList(&(lpML->EffHead), lpEL);
					lpML->lpChar_Own->ready_Magic = 2;
					lpML->lpChar_Own->end_Casting = 0;
					if( g_mgrBattle.IsMagic(lpML->magicSerial) )
					{//전투스킬일때는 된다.
						::CharDoAni( lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_NONE);
					}
					lpEL = lpML->EffHead;
					continue;
				}

				
				if (g_mgrBattle.IsCombat(lpML->magicSerial))
				{
					if (lpML->link_Frame == lpEL->FrameCount && lpML->lpChar_Own->ready_Magic == 1)
					{ 
						lpML->lpChar_Own->ready_Magic = 2;
						lpML->lpChar_Own->end_Casting = 0;
						lpML->lpChar_Own->end_ReadyMagic = 0;
						lpEL->flag = 1;
					}
				}
				
				if (lpML->lpChar_Own->nCurrentAction == ACTION_ATTACKED)//030227 lsw
				{
					const int iCancelRate = 100 - (SCharacterData.nLevel/8);
					const int iNow = ::rand()%100;
					
					if( iCancelRate < iNow )
					{
						::DeleteList(&(lpML->EffHead), lpEL);
						lpML->lpChar_Own->ready_Magic=0;
						lpML->lpChar_Own->end_Casting=0;
						
						if (lpML->lpChar_Own == Hero)
						{
							Hero->castStartingTime = 0;
						}
						
						lpEL = lpML->EffHead;
						continue;
					}
				}
				
				switch (lpML->lpChar_Own->condition)
				{
				case CON_DEATH:
				case CON_STONE:
				case CON_CURSE:
					{
						DeleteList(&(lpML->EffHead), lpEL);
						lpML->lpChar_Own->ready_Magic = 0;
						lpML->lpChar_Own->end_Casting = 0;
						lpEL = lpML->EffHead;
						continue;
					}
				default:
					{
						if (lpML->lpChar_Own->hp <= 0 || lpML->lpChar_Own->nCurrentAction == ACTION_DIE)
						{
							DeleteList(&(lpML->EffHead), lpEL);
							lpML->lpChar_Own->ready_Magic = 0;
							lpML->lpChar_Own->end_Casting = 0;
							lpEL = lpML->EffHead;
							continue;
						}
						break;
					}
				}
				// 이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				if (lpEL->Next == NULL){break;}
				lpEL = lpEL->Next;
			}
			
			lpEL = lpML->EffHead;
			
			while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
			{
				////////////////////// SoundUp lkh 시동어 출력 ///////////////////////
				if (lpEL->total_Frame == 8)
				{
					if (g_mgrBattle.IsMagic(lpML->magicSerial))
					{
						wave = ReturnCastingWord(lpML->lpChar_Own, lpML->magicSerial);		//900부터 짝수번은 남자 시동어, 홀수번은 여자 시동어
					}
					
					if( !IsPlaying(wave) )
					{
						if( lpML->lpChar_Own != Hero ) // LTS SOUND
						{
							int x=(lpML->lpChar_Own->x-Hero->x);	// LTS SOUND
							int y=(lpML->lpChar_Own->y-Hero->y);	// LTS SOUND
							PlayListAutoSounds(wave,x,y,0);	
						}
						else 
						{
							PlayListAutoSounds(wave,0,0,0);	// LTS SOUND
						}
					}
				}
				
				lpEL->total_Frame++;
				//////////////////////////////////////////////////////////////////////
				
				//동시 출력하는 이팩트의 갯수가 복수일 때 
				if(lpML->EffectCount < lpML->object_Num-1)
				{
					InsertEffect(lpML);
					lpML->EffectCount ++;
				}
				////////////////// 케릭터의 높이값에 따라 가감치 더해주기 //////////////////
				//int  change_Height=(lpML->lpChar_Own->sp->yl)-(lpML->lpChar_Target->sp->yl);
				//lpEL->y=lpML->y+change_Height;
				//lpEL->t_X=lpML->t_Y+change_Height;
				////////////////////////////////////////////////////////////////////////////
				if (g_mgrBattle.IsMagic(lpML->magicSerial))
				{
					float color_Rate =(float)( (lpML->lpChar_Own->end_Casting-g_ClientTime) / ((g_lpMT[lpML->magicSerial].cast_Time/10.f)*1000.f) );
					lpEL->trans_Level = (lpML->trans_Level+1)-(float)(lpML->trans_Level * color_Rate);
					if (lpEL->trans_Level < 2)	{lpEL->trans_Level = 2;}
				}
				
				if (g_mgrBattle.IsCombat(lpML->magicSerial))
				{
					lpEL->trans_Level = lpML->trans_Level;
				}
				
				if(g_lpET[lpML->magicNum].sound_loop!=0)
				{
					EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
				}
				// 반복루핑을 그만하고 종료 프레임으로 가볼까!
				if (lpEL->flag == 0 && TimeCheck(lpML->lpChar_Own->end_Casting - 500))
				{
					lpEL->flag=1;
					lpEL->FrameCount=10;
				}
				
				if(lpML->flag==0)	//광원 소멸 함수 호출되지 않은 경우
				{
					DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, target_Type);
				}
				else
				{
					DrawEffect(lpEL, lpML, 0, target_Type);
					
					// 1129 만약 시전동작에서의 전환에 제한을 두고자 하는 경우 relife
					//if(lpML->lpChar_Own->nCurrentAction != ACTION_MAGIC_BEFORE)
					//	CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_MAGIC_BEFORE);
					
					if (lpEL->Next == NULL) 	break;
				}
				lpEL = lpEL->Next;
			}
		}break;
	}
	
	return lpML->Next;
}

MAGICLIST* CastingCombat(MAGICLIST* lpML)
{	//< CSD-TW-030606
	int			illusion_Trans=0;
	int			terminateFrame=0;
	int			free_On=0;
	DWORD		check_Time=0;
	int			crash_Result=0;
	bool		target_Type=0;			//시전자에게 찍을까?(0) 대상체에게 찍을까?(1)
	bool		reset_Map=0;
	int			move_X=0,move_Y=0;
	int			wave = 0;
	USEEFFECTLIST* lpEL=lpML->EffHead;
	
	switch(lpML->PCount)
	{
	case 0:
		{ 
			if (g_mgrBattle.IsMagic(lpML->magicSerial))
			{
				wave = ReturnRoonWord(lpML->lpChar_Own, lpML->magicSerial);
				if( lpML->lpChar_Own != Hero ) // LTS SOUND
				{
					int x=(lpML->lpChar_Own->x-Hero->x)/*/TILE_SIZE*/;		// LTS SOUND
					int y=(lpML->lpChar_Own->y-Hero->y)/*/TILE_SIZE*/;		// LTS SOUND
					PlayListAutoSounds(wave,x,y,0);	
				}
				else 
				{
					PlayListAutoSounds(wave,0,0,0);	// LTS SOUND
				}
				CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_MAGIC_BEFORE);
			}
			
			if (g_mgrBattle.IsCombat(lpML->magicSerial))
			{
				CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->nCurrentAction);
			}

			lpML->lpChar_Own->changeActionFlag=0;
			InsertEffect(lpML);	
			lpML->PCount = 1;
			break;
		}
	case 1:
		{
			while (true)
			{
				if (g_mgrBattle.IsMagic(lpML->magicSerial))
				{
					if ((TimeCheck(lpML->lpChar_Own->end_Casting)) && lpML->lpChar_Own->ready_Magic == 1)
					{ // 현 캐스팅 단계를 종료하고 마법발사준비상태로의 전이
						lpML->lpChar_Own->ready_Magic = 2;
						lpML->lpChar_Own->end_Casting = 0;
						lpML->lpChar_Own->end_ReadyMagic = g_ClientTime + (g_lpMT[lpML->magicSerial].cast_Continue/6)*1000;
						lpEL->flag = 1;
						
						switch (lpML->lpChar_Own->excute_MagicNum)
						{	//< CSD-031014
						case FIRE_ARROW: lpML->lpChar_Own->bow_Type = 1;  break;
						case ICE_ARROW:  lpML->lpChar_Own->bow_Type = 2;  break;
						case THROW_ARROW: 
						case THROW_FIRE_ARROW:lpML->lpChar_Own->bow_Type = 0; break;  
						case THROW_KNIFE: 
						case THROW_ICE_KNIFE: lpML->lpChar_Own->bow_Type = 3; break;
						case THROW_SNOW_BALL:
						case THROW_WATER_BALL:
						case THROW_BIG_GULP: lpML->lpChar_Own->bow_Type = 3;  break;
						}	//> CSD-031014

						InsertMagic(lpML->lpChar_Own, lpML->lpChar_Own, lpML->next_Effect, 0, 0, 0, 0, 0);
					}
				}
				// MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
				if (lpML->EffHead == NULL) 
				{
					MAGICLIST* buff;
					buff = lpML->Next;				 // 다음 매직리스트링트의 주소값을 임시 기억
					lpML->EffectCount = 0;
					DeleteList(&g_lpML, lpML); // 실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
					if (lpEL == NULL)	 return buff;
				}
				
				if (lpML->lpChar_Own->ready_Magic == 2)  // 030314 kyo
				{
 					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					continue;
				}

				if (lpML->lpChar_Own->ready_Magic==0)  // 030314 kyo
				{   //마법 케스팅중에 멈춰버리면 캐스팅 동작(움직일수 없다)에서 멈추기 때문에 포츠를 기본으로 설정한다.
					DeleteList(&(lpML->EffHead), lpEL);
					lpML->lpChar_Own->ready_Magic = 2;
					lpML->lpChar_Own->end_Casting = 0;

					if (g_mgrBattle.IsMagic(lpML->magicSerial))
					{//전투스킬일때는 된다.
						::CharDoAni( lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_NONE);
					}
					lpEL = lpML->EffHead;
					continue;
				}

				if (g_mgrBattle.IsCombat(lpML->magicSerial))
				{
					if (lpML->link_Frame == lpEL->FrameCount && lpML->lpChar_Own->ready_Magic == 1)
					{ 
						lpML->lpChar_Own->ready_Magic = 2;
						lpML->lpChar_Own->end_Casting = 0;
						lpML->lpChar_Own->end_ReadyMagic = 0;
						lpEL->flag = 1;
					}
				}
				
				if (lpML->lpChar_Own->nCurrentAction == ACTION_ATTACKED)//030227 lsw
				{
					const int iCancelRate = 100 - (SCharacterData.nLevel/8);
					const int iNow = ::rand()%100;
					
					if( iCancelRate < iNow )
					{
						::DeleteList(&(lpML->EffHead), lpEL);
						lpML->lpChar_Own->ready_Magic=0;
						lpML->lpChar_Own->end_Casting=0;
						
						if (lpML->lpChar_Own == Hero)
						{
							Hero->castStartingTime = 0;
						}
						
						lpEL = lpML->EffHead;
						continue;
					}
				}
				
				switch (lpML->lpChar_Own->condition)
				{
				case CON_DEATH:
				case CON_STONE:
				case CON_CURSE:
					{
						DeleteList(&(lpML->EffHead), lpEL);
						lpML->lpChar_Own->ready_Magic = 0;
						lpML->lpChar_Own->end_Casting = 0;
						lpEL = lpML->EffHead;
						continue;
					}
				default:
					{
						if (lpML->lpChar_Own->hp <= 0 || lpML->lpChar_Own->nCurrentAction == ACTION_DIE)
						{
							DeleteList(&(lpML->EffHead), lpEL);
							lpML->lpChar_Own->ready_Magic = 0;
							lpML->lpChar_Own->end_Casting = 0;
							lpEL = lpML->EffHead;
							continue;
						}
						break;
					}
				}
				// 이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				if (lpEL->Next == NULL){break;}
				lpEL = lpEL->Next;
			}
			
			lpEL = lpML->EffHead;
			
			while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
			{
				////////////////////// SoundUp lkh 시동어 출력 ///////////////////////
				if (lpEL->total_Frame == 8)
				{
					if (g_mgrBattle.IsMagic(lpML->magicSerial))
					{
						wave = ReturnCastingWord(lpML->lpChar_Own, lpML->magicSerial);		//900부터 짝수번은 남자 시동어, 홀수번은 여자 시동어
					}
					
					if( !IsPlaying(wave) )
					{
						if( lpML->lpChar_Own != Hero ) // LTS SOUND
						{
							int x=(lpML->lpChar_Own->x-Hero->x);	// LTS SOUND
							int y=(lpML->lpChar_Own->y-Hero->y);	// LTS SOUND
							PlayListAutoSounds(wave,x,y,0);	
						}
						else 
						{
							PlayListAutoSounds(wave,0,0,0);	// LTS SOUND
						}
					}
				}
				
				lpEL->total_Frame++;
				//////////////////////////////////////////////////////////////////////
				
				//동시 출력하는 이팩트의 갯수가 복수일 때 
				if(lpML->EffectCount < lpML->object_Num-1)
				{
					InsertEffect(lpML);
					lpML->EffectCount ++;
				}
				////////////////// 케릭터의 높이값에 따라 가감치 더해주기 //////////////////
				//int  change_Height=(lpML->lpChar_Own->sp->yl)-(lpML->lpChar_Target->sp->yl);
				//lpEL->y=lpML->y+change_Height;
				//lpEL->t_X=lpML->t_Y+change_Height;
				////////////////////////////////////////////////////////////////////////////
				if (g_mgrBattle.IsMagic(lpML->magicSerial))
				{
					float color_Rate =(float)( (lpML->lpChar_Own->end_Casting-g_ClientTime) / ((g_lpMT[lpML->magicSerial].cast_Time/10.f)*1000.f) );
					lpEL->trans_Level = (lpML->trans_Level+1)-(float)(lpML->trans_Level * color_Rate);
					if (lpEL->trans_Level < 2)	{lpEL->trans_Level = 2;}
				}
				
				if (g_mgrBattle.IsCombat(lpML->magicSerial))
				{
					lpEL->trans_Level = lpML->trans_Level;
				}
				
				if(g_lpET[lpML->magicNum].sound_loop!=0)
				{
					EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
				}
				// 반복루핑을 그만하고 종료 프레임으로 가볼까!
				if (lpEL->flag == 0 && TimeCheck(lpML->lpChar_Own->end_Casting - 500))
				{
					lpEL->flag=1;
					lpEL->FrameCount=10;
				}
				
				if(lpML->flag==0)	//광원 소멸 함수 호출되지 않은 경우
				{
					DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, target_Type);
				}
				else
				{
					DrawEffect(lpEL, lpML, 0, target_Type);
					
					// 1129 만약 시전동작에서의 전환에 제한을 두고자 하는 경우 relife
					//if(lpML->lpChar_Own->nCurrentAction != ACTION_MAGIC_BEFORE)
					//	CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_MAGIC_BEFORE);
					
					if (lpEL->Next == NULL) 	break;
				}
				lpEL = lpEL->Next;
			}
		}break;
	}
	
	return lpML->Next;
}	//> CSD-TW-030606

MAGICLIST*	CastingContinueMagic(MAGICLIST*	lpML)//reece
{
	USEEFFECTLIST*  lpEL;
	
	bool		result=0;
	int			illusion_Trans=0;
	int			terminateFrame=0;
	int			free_On=0;
	DWORD		check_Time=0;
	//char		buf[30];
	
	bool		target_Type=0;			//시전자에게 찍을까?(0) 대상체에게 찍을까?(1)
	bool		reset_Map=0;
	int			move_X=0,move_Y=0;
	
	lpEL=lpML->EffHead;
	
	switch(lpML->PCount)
	{
	case 0:
		lpML->lpChar_Own->changeActionFlag=0;
		CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_NONE/*lpML->lpChar_Own->basicAction*/);
		
		InsertEffect(lpML);	
		lpML->PCount = 1;
		break;
		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거
		{
			/////////////////////////// 시간체크하여 magic state를 전환함 ////////////////////////
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					lpML->lpChar_Own->ready_Magic = 0;			//casting continue 상태를 원상태로		
					//lpML->lpChar_Own->excute_MagicNum =0;
					lpML->lpChar_Own->end_ReadyMagic =0;
					lpML->lpChar_Own->end_RunMagic =0;
					lpML->lpChar_Own->bow_Type =0;				//활 타입을 초기화
					RectImageOn=0;
					CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction); 
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				if (lpEL == NULL)	return buff;
			}
			
			// 0623 lkh 추가 (캐스팅 유지 취소 기능 추가)
			if( !(lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3) )
			{
				DeleteList(&(lpML->EffHead), lpEL);
				lpML->lpChar_Own->end_ReadyMagic =0;
				lpML->lpChar_Own->end_RunMagic =0;
				lpML->lpChar_Own->bow_Type =0;				//활 타입을 초기화
				RectImageOn=0;
				
				switch (lpML->magicSerial)
				{
				case LIGHTNING_BOLT:
				case GREAT_THUNDER_BOLT:
					{
						break;
					}
				default:
					{
						CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction/*ACTION_NONE*/);
						break;
					}
				}
				
				lpEL = lpML->EffHead;
				continue;
			}
			
			if(	lpML->lpChar_Own->condition==CON_DEATH || lpML->lpChar_Own->hp<=0 ) //|| lpML->lpChar_Own->nCurrentAction == ACTION_DIE || lpML->lpChar_Own->nCurrentAction == MON1_DIE )	//마법발사준비 덜 됐고 죽거나 얻어 맞는 경우
			{
				if( ((lpML->lpChar_Own->animationType == ANIMATION_TYPE_MAN || lpML->lpChar_Own->animationType == ANIMATION_TYPE_WOMAN) && lpML->lpChar_Own->nCurrentAction == ACTION_DIE) ||
					((lpML->lpChar_Own->animationType == ANIMATION_TYPE_MON1) && lpML->lpChar_Own->nCurrentAction == MON1_DIE) )
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpML->lpChar_Own->end_ReadyMagic =0;
					lpML->lpChar_Own->end_RunMagic =0;
					lpML->lpChar_Own->bow_Type =0;				//활 타입을 초기화
					RectImageOn=0;
					CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction/*ACTION_NONE*/);
					lpEL = lpML->EffHead;
					continue;
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////
			
			if (lpEL->Next == NULL)			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
			{
				break;
			}
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
		}
		
		lpEL = lpML->EffHead;
		
		while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
		{
			//동시 출력하는 이팩트의 갯수가 복수일 때 
			if(lpML->EffectCount < lpML->object_Num-1)
			{
				InsertEffect(lpML);
				lpML->EffectCount ++;
			}
			
			if(g_lpET[lpML->magicNum].sound_loop!=0)
				EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
			
			/////////////////////////// 시간에 따라 컬러값을 변경시켜 볼까? ////////////////////////////
			if(lpEL->FrameCount < 8 && lpEL->flag ==0 ) 
			{
				lpEL->trans_Level = (lpML->trans_Level/8.f*(lpEL->FrameCount+1));
				if(lpEL->trans_Level<2)		lpEL->trans_Level = 2;
			}
			else
			{
				float color_Rate =(float)( (lpML->lpChar_Own->end_ReadyMagic-g_ClientTime) / ((g_lpMT[lpML->magicSerial].cast_Continue/10.)*1000.) );
				lpEL->trans_Level = (float)(lpML->trans_Level * color_Rate);
				if(lpEL->trans_Level < 1)	lpEL->trans_Level=1;
				lpEL->flag=1;
			}
			
			if(lpML->flag==0)	//광원 소멸 함수 호출되지 않은 경우
				DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, target_Type);
			else
				DrawEffect(lpEL, lpML, 0, target_Type);
			
			if (lpEL->Next == NULL)
				break;
			else lpEL = lpEL->Next;
		}
		break;
	}
	return lpML->Next;
}
//주인공 인물 위치에 단일 프레임 겹쳐 찍어 주기
MAGICLIST*	SampleMagic_Type4_Like(MAGICLIST*	lpML)
{
	USEEFFECTLIST*  lpEL;
	static int	evil_Eye_Delay=12;
	int			illusion_Trans=0;
	int			terminateFrame=0;
	int			free_On=0;
	DWORD		check_Time=0;
	//char		buf[30];
	
	int			crash_Result=0;
	bool		target_Type=0;			//시전자에게 찍을까?(0) 대상체에게 찍을까?(1)
	bool		reset_Map=0;
	int			move_X=0,move_Y=0;
	int			center_TileX = lpML->tarx/TILE_SIZE;
	int			center_TileY = lpML->tary/TILE_SIZE-1;
	
	lpEL=lpML->EffHead;
	
	if(lpML->magicSubNum==9)
		target_Type=1;	//만약 서브 타입이 9면 적에게 효과소스 출력
	
	switch (lpML->magicNum)
	{
    case 552: 
    case 555: 
    case 558: 
    case 561: 
    case 564: 
    case 567: target_Type=1; break;
	}
	
	switch(lpML->PCount)
	{
	case 0:
		InsertEffect(lpML);
		//효과리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 1;
		
		//지속마법인 경우 지속 시작시간 셋팅
		if(lpML->end_Time>0 && lpML->next_Effect==0 )	//지속마법이며 다음으로 이어지는 마법이 없는 경우
		{
			lpML->magic_Runtime= g_ClientTime + (g_lpET[lpML->magicNum].end_Time*1000);					//밀리세컨드
		}
		else if(lpML->end_Time<0 && lpML->next_Effect==0)
		{
			lpML->magic_Runtime	= g_ClientTime + g_lpMT[lpML->magicSerial].continue_Time/2*1000;
		}
		break;
		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거
		{
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				lpML->EffectCount = 0;
				
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림
				
				//if(lpML!=NULL || lpML->EffectCount!=NULL)
				//	lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				
				if (lpEL == NULL)
				{
					return buff;
				}
			}
			
			////////////////////////////////// 0112 이규훈 //////////////////////////////////////
			if( lpML->lpChar_Own == NULL || lpML->lpChar_Target == NULL )
			{
				MAGICLIST* buff;
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				lpML->EffectCount = 0;
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				
				//if(lpML!=NULL || lpML->EffectCount!=NULL)
				//	lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				
				if (lpEL == NULL)
				{
					return buff;
				}
			}
			
			if( lpML->lpChar_Own->ready_Magic==2 || lpML->lpChar_Own->ready_Magic==3)
			{
				if( TimeCheck(lpML->lpChar_Own->end_ReadyMagic) )	//마법 발사 유지 단계를 종료
				{
					EndCastMagic(lpML->lpChar_Own);
					//CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_BATTLE_NONE); 
				}
			}
			/////////////////////////// 종료 조건 확인하여 링크드 리스트에서 제외 ////////////////////////////
			if (lpML->flag == 0 && 
				(DayLightControl < 31) && 
				g_lpET[lpML->magicNum].end_Time != -1 && 
				IsHarmfulMagic(lpML->magicSerial))	//소멸되는 광원 초기화 시켜주기 
			{
				terminateFrame=(EffList[lpEL->sprite_Num].MaxFrame)/LIGHT_TERMINATE;
				
				if(terminateFrame<1)	terminateFrame=1;
				
				if(g_lpET[lpML->magicNum].light/100 == 1 )
					InsertLightOut( lpML->tarx, (int)(lpML->tary-lpEL->height), g_lpET[lpML->magicNum].light%100, terminateFrame, g_lpET[lpML->magicNum].light/100 );
				else
					InsertLightOut( lpML->tarx, lpML->tary, g_lpET[lpML->magicNum].light%100, terminateFrame, g_lpET[lpML->magicNum].light/100 );
				lpML->flag=1;
			}
			
			if( g_lpET[lpML->magicNum].end_Time )		//특정 유지시간을 가지고 있는 경우
			{
				if( lpML->magic_Runtime <= g_ClientTime )
				{						
					if(lpML->magicNum == 420)
					{
						POINT	tab_wizard[44]={6,0, 6,-1, 5,-1, 5,-2, 5,-3, 4,-3, 3,-3, 3,-4, 2,-4, 1,-4, 1,-5, 0,-5, -1,-5, -1,-4, -2,-4, -3,-4, -3,-3,
							-4,-3, -5,-3, -5,-2, -5,-1, -6,-1, -6,0, -6,1, -5,1, -5,2, -5,3, -4,3, -3,3, -3,4, -2,4, -1,4, -1,5, 0,5, 1,5,
							1,4, 2,4, 3,4, 3,3, 4,3, 5,3, 5,2, 5,1, 6,1};
						//Dont Tile 처리
						for(int i=0;i<44;i++)
						{
							if(TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].empty__6 == 1)
							{
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_dont = 0;
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_light= 0;
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].empty__6	= 0;
							}
						}
					}
					if(lpML->magicNum == 333)
					{
						POINT	tab_priest[36]={5,0, 5,-1, 4,-1, 4,-2, 3,-2, 3,-3, 2,-3, 1,-3, 1,-4, 0,-4, -1,-4, -1,-3, -2,-3, -3,-3, -3,-2, -4,-2, -4,-1,
							-5,-1, -5,0, -5,1, -4,1, -4,2, -3,2, -3,3, -2,3, -1,3, -1,4, 0,4, 1,4, 1,3, 2,3, 3,3, 3,2, 4,2, 4,1, 5,1 };
						//Dont Tile 처리
						for(int i=0;i<36;i++)
						{
							if(TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].empty__6 == 1)
							{
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_dont = 0;
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_light= 0;
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].empty__6	= 0;
							}
						}
					}
					
					lpML->magic_Runtime	= 0;
					
					lpEL->FrameCount=0;
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					
					continue;
				}
			}
			else if( ( lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame ) || ( lpML->magicSubNum==4 && lpEL->FrameCount >= EffList[lpEL->sprite_Num].MaxFrame/8 ))
			{
				if(lpML->next_Effect==0 || g_lpET[lpML->magicSerial].impect_Effect_Num==0)
				{
					lpML->lpChar_Own->changeActionFlag = 0;
				}
				
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			
			if (lpEL->Next == NULL /*& lpEL->total_Frame != 0*/ )		//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				break;
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
			}
			
			lpEL = lpML->EffHead;
			
			while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
			{
				//동시 출력하는 이팩트의 갯수가 복수일 때 
				if(lpML->EffectCount < lpML->object_Num-1)
				{
					InsertEffect(lpML);
					lpML->EffectCount ++;
				}
				
				//int runMagicNum = g_mgrBattle.Return(lpML->magicNum, lpML->lpChar_Own);
				int runMagicNum = lpML->magicSerial; 	// 030415 kyo
				
				if( lpEL->FrameCount >= 42 )
				{
					if(lpML->magicNum == 420)
					{
						POINT	tab_wizard[44]={6,0, 6,-1, 5,-1, 5,-2, 5,-3, 4,-3, 3,-3, 3,-4, 2,-4, 1,-4, 1,-5, 0,-5, -1,-5, -1,-4, -2,-4, -3,-4, -3,-3,
							-4,-3, -5,-3, -5,-2, -5,-1, -6,-1, -6,0, -6,1, -5,1, -5,2, -5,3, -4,3, -3,3, -3,4, -2,4, -1,4, -1,5, 0,5, 1,5,
							1,4, 2,4, 3,4, 3,3, 4,3, 5,3, 5,2, 5,1, 6,1};
						//Dont Tile 처리
						for(int i=0;i<44;i++)
						{
							if( TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_dont == 0 )
							{
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_dont	= 1;
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].attr_light = 1;
								TileMap[center_TileX+tab_wizard[i].x][center_TileY+tab_wizard[i].y].empty__6	= 1;
							}
						}
					}
					if(lpML->magicNum == 333)
					{
						POINT	tab_priest[36]={5,0, 5,-1, 4,-1, 4,-2, 3,-2, 3,-3, 2,-3, 1,-3, 1,-4, 0,-4, -1,-4, -1,-3, -2,-3, -3,-3, -3,-2, -4,-2, -4,-1,
							-5,-1, -5,0, -5,1, -4,1, -4,2, -3,2, -3,3, -2,3, -1,3, -1,4, 0,4, 1,4, 1,3, 2,3, 3,3, 3,2, 4,2, 4,1, 5,1 };
						//Dont Tile 처리
						for(int i=0;i<36;i++)
						{
							if( TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_dont == 0 )
							{
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_dont	= 1;
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].attr_light = 1;
								TileMap[center_TileX+tab_priest[i].x][center_TileY+tab_priest[i].y].empty__6	= 1;
							}
						}
					}
				}				
				
				// 마법 결과에 대한 답변 요청 (화염벽/얼음벽마법은 제외)
				if(lpML->next_Effect && lpML->link_Frame==lpEL->FrameCount && lpML->magicSerial && lpML->magicSerial!=34 && lpML->magicSerial!=42 && lpML->magicSerial!=45 && lpML->magicSerial!=41 && lpML->magicSerial != 162 && !lpML->failed)
				{
					if ((g_lpMT[lpML->magicSerial].magic_Type/10 != 5) && 
						(lpML->magicSerial==runMagicNum || runMagicNum==215))	//비공격마법이면서 시전의 링크 프레임인 경우
					{
						if(lpML->lpChar_Own == Hero && lpML->next_Flag==0)
						{
							if (g_lpMT[lpML->magicSerial].avail_Type < 2 || 
								(g_lpMT[lpML->magicSerial].avail_Type >=2 && IsHarmfulMagic(lpML->magicSerial)))
							{	//< CSD-TW-030606
								g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Target->id, lpML->tarx, lpML->tary);
							}	//> CSD-TW-030606
							else
							{
								//SendCMD_MAGIC_RESULT(29999, lpML->magicSerial, lpML->tarx, lpML->tary);
								if(lpML->magicSerial )	//지역 공격 마법의 경우
								{
									//폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
									int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
									int check_yl = check_xl*3/4;
									attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
								}
							}
					
							lpML->next_Flag=1;
						}
					}
				}
				
				if( lpML->link_Frame>0 && lpML->link_Frame==lpEL->FrameCount && !lpML->failed)
				{
					
					//////////////////////////// 0815 lkh 수정 //////////////////////////////////
					//////////////////////////// 내 자신은 체크하면 안되는 마법(공격)인 경우 ///////////////////////////////
					if( (lpML->magicSerial==45 && (lpML->magicNum!=lpML->magicSerial)) ||
						(lpML->magicSerial==41 && (lpML->magicNum!=lpML->magicSerial)) ||
						(lpML->magicSerial == 162 && (lpML->magicNum-150!=lpML->magicSerial)) )			//지역 공격 마법의 경우
					{
						
						//폭파이팩트의 효과 범위내에 적이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
						int check_xl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE;			//반경 타일
						int check_yl = check_xl*3/4;
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 0);
					}
					else if(lpML->magicSerial == MIRACLE_RECOVER)	//완치의 기적 마법
					{
						int	check_xl = g_lpMT[156].avail_Range*TILE_SIZE;			//반경 타일
						int check_yl = check_xl*3/4;
						//완치의 기적 효과가 미치는 범위 내에 캐릭이 있는 경우 체크 (ReturnMagicCheckedCharacter 함수에서 타격효과 구현)
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, lpML->x - check_xl, lpML->y - check_yl, (int)lpEL->height, (check_xl*2), (check_yl*2), 1);
					}
					else if (lpML->magicSerial == PHASING) // 장소이동 마법의 경우
					{
#ifdef _DEBUG // finito 060507
						if(SysInfo.notconectserver)
						{
							do	{
								move_X=rand()%19-9;
								move_Y=rand()%11-5;
							}	while( abs(move_X)<3 || abs(move_Y)<2);
							
							lpML->lpChar_Own->x += move_X*32;
							lpML->lpChar_Own->y += move_Y*32;
							lpML->t_X			 = lpML->lpChar_Own->x;
							lpML->t_Y			 = lpML->lpChar_Own->y;	
							
							//CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_BATTLE_NONE);
							lpML->lpChar_Own->pathcount=0;
							CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->basicAction);
							
							if(lpML->magicSubNum==8)	InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,9				,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);
							else						InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);
						}
#endif
					}
					else if (lpML->magicSerial == ACUQUIRE_DARK || lpML->magicSerial == BURNING_BLOODY)
					{	//< CSD-TW-030624
						if (g_lpET[lpML->magicNum].end_Time == 0)
						{
							CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction , lpML->lpChar_Own->basicAction);
							g_mgrBattle.SendMagicResult(lpML->magicSerial, lpML->lpChar_Own, lpML->lpChar_Target->id, lpML->tarx, lpML->tary);
							DeleteList(&(lpML->EffHead), lpEL);
							return NULL;
						}
					}	//> CSD-TW-030624
					else	//다음 연결 효과와 연결
					{
						//다음 연결 패턴이 4인 경유
						if( g_lpET[lpML->next_Effect].pattern_Num == 4 || g_lpET[lpML->next_Effect].pattern_Num == 3 ) //|| g_lpET[lpML->next_Effect].pattern_Num == 12)
						{
							//시전자의 방향과 액션 형태를 조정
							if(lpML->lpChar_Own == lpML->lpChar_Target)
							{
								lpML->lpChar_Own->changeActionFlag=0;
								CharDoAni( lpML->lpChar_Own, lpML->lpChar_Own->direction , ACTION_NONE );
							}
							else
							{
								lpML->lpChar_Own->changeActionFlag=0;
								CharDoAni( lpML->lpChar_Own, lpML->lpChar_Own->direction , lpML->lpChar_Own->basicAction );
							}
						}
						
						///////////////////////// 0715 lkh 추가 /////////////////////////
						// 폭죽과 같은 여러개의 이펙트 효과를 랜덤하게 조합해서 찍어야하는 경우
						else if(lpML->magicSubNum/100 == 1)			//100단위수가 1인 경우 단자리 수자 만큼 이후번호의 이펙트를 10단위의 숫자 만큼 연결 출력
						{
							int	link_Count = (lpML->magicSubNum%100)/10;
							int later_Count = (lpML->magicSubNum%10);
							lpML->magicSubNum = lpML->magicSubNum - 10;
							
							int	real_MagicNum = 375 + rand()%later_Count;
							
							if(link_Count && later_Count)
								InsertMagic( lpML->lpChar_Own, lpML->lpChar_Own, real_MagicNum, lpML->magicSubNum, lpML->s_X, lpML->s_Y, 0, 0);
						}
						
						else		//4패턴이 아닌 경우
						{
							lpML->lpChar_Own->changeActionFlag=0;
							
							switch (lpML->magicSerial)
							{
							case LIGHTNING_EXTREME:
								{ // 030513 kyo // 24번 패턴으로 두번 돈다.
									if( g_lpET[lpML->next_Effect].pattern_Num != 24 )
									{										
										g_mgrBattle.Effect(lpML);									
									}
									else
										InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->x,lpML->y,lpML->tarx, lpML->tary);
									break; 
								}

							case DARK_EXTREME:
								{
									break;
								}
							default:
								{
									CharDoAni( lpML->lpChar_Own, lpML->lpChar_Own->direction , ACTION_MAGIC_CASTING );
									break;
								}
							}
						}
						
						if(lpML->magicSubNum==8)
							InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,9				,lpML->x,lpML->y,lpML->tarx, lpML->tary);
						else
						{
							if(lpML->next_Effect == 371)		//얼음폭풍인 경우
							{
								InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->x,lpML->y,lpML->tarx, lpML->tary);
								InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->x,lpML->y,lpML->tarx, lpML->tary);
								InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->x,lpML->y,lpML->tarx, lpML->tary);
								InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->x,lpML->y,lpML->tarx, lpML->tary);
							}
							else
							{
								InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->x,lpML->y,lpML->tarx, lpML->tary);
							}
						}
					}
				}
				
				if( lpML->link_Frame<0 )			//음수인 경우 해당 조건 만족할 때 다음 연결 효과 링크
				{	
					evil_Eye_Delay--;
					if(evil_Eye_Delay<=0)
					{
						//성직자 5계열 [악에 대한 보호] 마법의 경우 대상체 효과 처리(detect 범위 -> 반경 5타일내 적이 있는 경우)
						attacked_Char=ReturnMagicCheckedCharacter(1, lpML, (lpML->x) - 160, (lpML->y) - 128, (int)lpEL->height, 320, 256, 0);
						evil_Eye_Delay=10;
					}
				}
				
				if(lpML->trans_Type/10!=0 && lpML->trans_Type%10==8)		//점점 밝게 생성 / 마지막 어둡게
				{
					if(lpEL->FrameCount<EffList[lpEL->sprite_Num].MaxFrame-5)
					{
						lpEL->trans_Level=(lpML->trans_Level/(float)EffList[lpEL->sprite_Num].MaxFrame)*lpEL->FrameCount;
						if(lpEL->trans_Level > lpML->trans_Level)	lpEL->trans_Level=(float)lpML->trans_Level;
					}
					else
					{
						lpEL->trans_Level-=(float)(lpML->trans_Level/4.);
						if(lpEL->trans_Level < 1) lpEL->trans_Level = 1;
					}
				}
				
				// 마지막 4프레임은 alpha off
				else if( (lpML->trans_Type/10!=0 && lpML->trans_Type%10==9) && lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame-4)	
				{
					lpEL->trans_Level-=(lpML->trans_Level/4);//-((lpML->trans_Level/EffList[lpEL->sprite_Num].MaxFrame)*(lpEL->FrameCount-1));
					if(lpEL->trans_Level<=1)	lpEL->trans_Level=1;
				}
				
				else if(lpML->trans_Type%10==7)	//바닥이 얼어붙는 효과
				{
					if(lpEL->FrameCount<=EffList[lpML->sprite_Num].MaxFrame/2)
					{
						lpEL->trans_Level=(float)lpEL->FrameCount/2;
						if(lpEL->trans_Level>=12)	lpEL->trans_Level=12;
					}
					else
					{
						lpEL->trans_Level-=(lpEL->trans_Level/(EffList[lpML->sprite_Num].MaxFrame/2));//(float)(EffList[lpML->sprite_Num].MaxFrame-lpEL->FrameCount);
						if(lpEL->trans_Level<0) lpEL->trans_Level=0;
					}
				}
				
				else if(g_lpET[lpML->magicNum].end_Time>0)			//Effect Table에 특정 유지시간이 주어진 유지 마법의 경우 색깔값 지속적 떨어트림
				{
					lpEL->trans_Level = (float) ((float)lpML->trans_Level * ( (lpML->magic_Runtime - g_ClientTime) / (g_lpET[lpML->magicNum].end_Time*1000.) ) );
					if(lpEL->trans_Level<2)	lpEL->trans_Level=2;
				}
				
				else if(g_lpET[lpML->magicNum].end_Time<0)			//Magic Table에 정의된 유지 시간동안 지속되는 마법의 색깔값 변환
				{
					if(lpML->magic_Runtime > g_ClientTime + (g_lpMT[lpML->magicSerial].continue_Time*1000))
						lpEL->trans_Level = (float)lpML->trans_Level;
					else
						lpEL->trans_Level = (float) ((float)lpML->trans_Level * ( (lpML->magic_Runtime - g_ClientTime) / (g_lpMT[lpML->magicSerial].continue_Time*1000.) ) );
					if(lpEL->trans_Level<2)	lpEL->trans_Level=2;
				}
				////////////////// 케릭터의 높이값에 따라 가감치 더해주기 //////////////////
				//int  change_Height=(lpML->lpChar_Own->sp->yl)-(lpML->lpChar_Target->sp->yl);
				//lpEL->y=lpML->y+change_Height;
				//lpEL->t_X=lpML->t_Y+change_Height;
				////////////////////////////////////////////////////////////////////////////
				if(g_lpET[lpML->magicNum].sound_loop)
					EffectSoundPlay(lpML, lpML->magicNum, g_lpET[lpML->magicNum].sound_Num, g_lpET[lpML->magicNum].sound_loop);
				
				if(lpML->flag==0 || g_lpET[lpML->magicNum].end_Time==-1)	//광원 소멸 함수 호출되지 않은 경우
				{
					if(!g_lpET[lpML->magicNum].end_Time)
					{ 
						DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, target_Type);
					} 
					else
					{
						int	light_Level = g_lpET[lpML->magicNum].light/100*100+(int)( g_lpET[lpML->magicNum].light%100*((lpML->magic_Runtime - g_ClientTime) / (g_lpMT[lpML->magicSerial].continue_Time*1000.))+.5);
						if(light_Level > g_lpET[lpML->magicNum].light)		light_Level = g_lpET[lpML->magicNum].light;
						DrawEffect(lpEL, lpML, light_Level, target_Type);
					}
				}
				else
					DrawEffect(lpEL, lpML, 0, target_Type);
				
				lpEL = lpEL->Next;			
			}
			break;
	}
	return lpML->Next;
}

void DrawEffect(USEEFFECTLIST *Temp, MAGICLIST* lpML , int light, bool target)
{
	int incFrame=0;
	int sort_Type=SPRITETYPE_EFFECT_NORMAL;
	int incHeight=0;			//인물 이미지의 높이값에 따른 높이 증가값
	int incOpsetHeight=2;		//앞/뒤 2장 찍을 때 앞에 찍히는 스프라이트 증가 높이치
	int sprc=0;
	int temp_Type=0;
	int inc=0;
	int sorting1=0, sorting2=0;
	int offset_X=0, offset_Y=0;
	int	tempTransType=0;
	
	///////////////////// 0525 lkh 추가 ///////////////////////
	if (EffList[Temp->sprite_Num].EffData == NULL)
		return;
	
	if (EffList[Temp->sprite_Num].EffData[Temp->FrameCount].SprNo == -1)		//해당 모션의 마지막 프레임에 도달할 경우
	{	//루핑을 위한 처리(단 화염벽 패턴의 처음 생성패턴은 한번만, 나머지 루핑)
		if(g_lpET[lpML->magicNum].pattern_Num==PROTECT_WALL && lpML->magicSerial==34)
			Temp->FrameCount=10;
		else if(g_lpET[lpML->magicNum].pattern_Num==PROTECT_WALL && lpML->magicSerial==42)
			Temp->FrameCount=19;
		//마법결계 이펙트의 42번 프레임부터 루핑처리되게(단 시전효과에는 처리안되게)
		else if(lpML->magicNum == 420)
			Temp->FrameCount=42;
		else
			Temp->FrameCount=0;
		
		Temp->RepeatCount = EffList[Temp->sprite_Num].EffData[Temp->FrameCount].RepeatFrame;
	}
	
	//if (g_lpET[lpML->magicNum].pattern_Num==PROTECTWALL)
	//	incFrame = (int)(lpML->EffectCount*EffList[Temp->sprite_Num].MaxFrame/4.);
	
	if(g_lpET[lpML->magicNum].pattern_Num==7)						//16방향 발사 패턴(화살)
		incFrame=(EffList[Temp->sprite_Num].MaxFrame/16)*Temp->dir;	//한 방향당 프레임수
	else if(g_lpET[lpML->magicNum].pattern_Num==8 || g_lpET[lpML->magicNum].pattern_Num==13 )	//8방향 발사 패턴
	{
		if(EffList[Temp->sprite_Num].MaxFrame%8==0)					//8방향 스프라이트이면 
			incFrame=(EffList[Temp->sprite_Num].MaxFrame/8)*lpML->dir;	//한 방향당 프레임수
	}
	else if(g_lpET[lpML->magicNum].pattern_Num==10)					//유성 떨어지는 패턴(2방향)
		incFrame=(EffList[Temp->sprite_Num].MaxFrame/2)*lpML->dir;
	else if(g_lpET[lpML->magicNum].pattern_Num==18)					//물고기 수면 위로 튀어 오르기
	{
		if(g_lpML->magicSubNum%2==1)
			incFrame=(EffList[Temp->sprite_Num].MaxFrame/2);
	}
	///////////// 0420 lkh -> 흩뿌리는 빛의 파티클 효과인 경우 //////////////
	else if(g_lpET[lpML->magicNum].pattern_Num==PARTICLE)
	{
		incFrame = Temp->dir * 10;		//방향은 0~4까지 5가지의 경우가 있음(5개 각각 다른 색의 빛구채)
	}
	
	if(g_lpET[lpML->magicNum].pattern_Num==4 && lpML->magicSubNum==4)	
	{
		incFrame=(EffList[Temp->sprite_Num].MaxFrame/8)*lpML->dir;
	}
	
	if(g_lpET[lpML->magicNum].pattern_Num==24 && lpML->magicSubNum==4)	
	{
		incFrame=(EffList[Temp->sprite_Num].MaxFrame/8)*lpML->dir;
	}
	
	if(g_lpET[lpML->magicNum].pattern_Num==20 && lpML->magicSubNum!=0)	//스킬마법중 성공->4/실패->2 여부 패턴 
	{
		if(lpML->magicSubNum<5)
			incFrame=(EffList[Temp->sprite_Num].MaxFrame/10)*(Temp->effect_Count); //lpML->magicSubNum-1);
		else
			incFrame=(EffList[Temp->sprite_Num].MaxFrame/10)*(Temp->effect_Count+5); //lpML->magicSubNum-1);
	}
	
	
	int		magic_X,magic_Y;
	if(target==0)		//내자신 또는 지역마법
	{
		switch (lpML->fallow)
		{
		case 0: // 어떤 방향으로 나가는 형태
			{
				magic_X = Temp->x;
				magic_Y = Temp->y;
				break;
			}
		case 1: // 시전자를 따라가는 형태
			{
				magic_X = lpML->lpChar_Own->x;
				magic_Y = lpML->lpChar_Own->y;
				break;
			}
		case 2: // 일전한 거리로 시전자를 따라오는 형태
			{
				magic_X = lpML->lpChar_Own->x + lpML->tarx;
				magic_Y = lpML->lpChar_Own->y + lpML->tary;
				break;
			}
		case 3:
			{
				magic_X = lpML->tarx;
				magic_Y = lpML->tary;
				break;
			}
		}
	}
	else				//대상마법
	{
		if(lpML->fallow==1)
		{
			magic_X=lpML->lpChar_Target->x;
			magic_Y=lpML->lpChar_Target->y;
		}
		else
		{
			magic_X=Temp->tarx;
			magic_Y=Temp->tary;
		}
	}
	
	/////////////////////// 집안에 있는 클라이언트는 집 밖에서 쓰는 마법의 광원을 보이지 않게 처리(단 일반화살과 비검의 경우는 예외) ////////////
	if( (TileMap[Hero->x/32][Hero->y/32].attr_inside==0 && TileMap[(int)((magic_X + Temp->incx)/32)][(int)((magic_Y + Temp->incy)/32)].attr_inside==0)
		|| ( TileMap[Hero->x/32][Hero->y/32].attr_inside==1 && TileMap[(int)((magic_X + Temp->incx)/32)][(int)((magic_Y + Temp->incy)/32)].attr_inside==1 ) )
	{
		if( light>0 && (DayLightControl < 31) )
		{
			if(light/100!=1)		//일반적인 방법인 지상에 광원 출력
				OutputLight( EffList[Temp->sprite_Num].EffData[Temp->FrameCount].x+ magic_X + (int)(Temp->incx+.5) + offset_X,
				EffList[Temp->sprite_Num].EffData[Temp->FrameCount].y+ magic_Y + (int)(Temp->incy+.5) + offset_Y , light%100, light/100);
			else		//높이값 더해서 이펙트에 바로 물려서
				OutputLight( EffList[Temp->sprite_Num].EffData[Temp->FrameCount].x+ magic_X + (int)(Temp->incx+.5) + offset_X,
				EffList[Temp->sprite_Num].EffData[Temp->FrameCount].y+ magic_Y + (int)(Temp->incy+.5) + offset_Y-(int)(Temp->height) , light%100, light/100);//(light-1)+rand()%2);
		}
		
	}
	
	if(g_lpET[lpML->magicNum].sort_Type==12)		//에니메이션 화일을 참조하여 소팅방법 선택
	{
		if(offset_X==0)
			Temp->sort_Type=9;
		else if(offset_X==1)
			Temp->sort_Type=10;
		else if(offset_X==2)
			Temp->sort_Type=11;
	}
	else
		Temp->sort_Type=g_lpET[lpML->magicNum].sort_Type;
	
	tempTransType=lpML->trans_Type;
	
	/////////////////////// 집안에 있는 클라이언트는 집 밖에서 쓰는 마법은 보이지 않게 처리(단 일반화살과 비검의 경우는 예외) ////////////
	if( (TileMap[Hero->x/32][Hero->y/32].attr_inside==0 && TileMap[(int)((magic_X + Temp->incx)/32)][(int)((magic_Y + Temp->incy)/32)].attr_inside==0)
		|| ( TileMap[Hero->x/32][Hero->y/32].attr_inside==1 && TileMap[(int)((magic_X + Temp->incx)/32)][(int)((magic_Y + Temp->incy)/32)].attr_inside==1 ) )
	{
		if((Temp->sort_Type == 13 || lpML->magicSubNum < 0) || Temp->sort_Type == 14 || Temp->sort_Type == 15)
		{ // 앞과 뒷쪽의 스프라이트를 따로 2장을 함께 찍을 경우
			int nTrans1 = 0, nTrans2 = 2;
			switch (Temp->sort_Type)
			{
			case 14:
				{
					sorting1 = 10;
					sorting2 = 10;
					nTrans1 = 2;
					nTrans2 = 5;
					break;
				}
			case 15:
				{
					sorting1 = 10;
					sorting2 = 10;
					nTrans1 = 5;
					nTrans2 = 2;
					break;
				}
			default:
				{
					sorting1 = 9;
					sorting2 = 9;
					
					if (lpML->trans_Type==9)
					{
						tempTransType=5;
						inc = -3;			//-1 : Half Trans
					}
					else if(lpML->trans_Type == 0)
					{
						tempTransType=1;
						inc=1;
					}
					else	tempTransType=lpML->trans_Type;
					nTrans1 = tempTransType;
					nTrans2 = tempTransType + inc;
					
					break;
				}
			}
			
			out_Result = EffectOutput(EffList[Temp->sprite_Num].EffData[Temp->FrameCount].x+ (magic_X - Mapx) + (int)(Temp->incx+.5) + offset_X,
				EffList[Temp->sprite_Num].EffData[Temp->FrameCount].y+ (magic_Y - Mapy) + (int)(Temp->incy+.5) + offset_Y-1,
				&Effspr[Temp->sprite_Num].EffSpr[EffList[Temp->sprite_Num].EffData[Temp->FrameCount+incFrame].SprNo],
				nTrans1, (int)Temp->trans_Level, (int)Temp->height+incHeight, sorting1);
			
			int tframe = Temp->FrameCount+1+incFrame;
			if( tframe >=  EffList[Temp->sprite_Num].MaxFrame ) tframe = EffList[Temp->sprite_Num].MaxFrame - 1;
			out_Result = EffectOutput(EffList[Temp->sprite_Num].EffData[Temp->FrameCount].x+ (magic_X - Mapx) + (int)(Temp->incx+.5) + offset_X,
				EffList[Temp->sprite_Num].EffData[Temp->FrameCount].y+ (magic_Y - Mapy) + (int)(Temp->incy+.5) + offset_Y+1,
				&Effspr[Temp->sprite_Num].EffSpr[EffList[Temp->sprite_Num].EffData[ tframe].SprNo], 
				nTrans2, (int)Temp->trans_Level, (int)Temp->height+incHeight+incOpsetHeight, sorting2);
		}
		else
		{
			if(lpML->magicSubNum==-2)	inc=1;
			
			out_Result=EffectOutput(EffList[Temp->sprite_Num].EffData[Temp->FrameCount].x+ (magic_X - (Mapx/*g_Map.x*TILE_SIZE*/)) + (int)(Temp->incx+.5) + offset_X,
				EffList[Temp->sprite_Num].EffData[Temp->FrameCount].y+ (magic_Y - (Mapy/*g_Map.y*TILE_SIZE*/)) + (int)(Temp->incy+.5) + offset_Y+1,
				&Effspr[Temp->sprite_Num].EffSpr[EffList[Temp->sprite_Num].EffData[Temp->FrameCount+incFrame].SprNo], tempTransType, (int)Temp->trans_Level, (int)Temp->height+incHeight, Temp->sort_Type);
		}
	}	
	////////////////////////////////////////////////////////////////////////////
	
	if (--Temp->RepeatCount <= 0)	//반복치가 있으면 반복출력 처리
	{
		if( (g_lpET[lpML->magicNum].sort_Type==13 || lpML->magicSubNum<0) || g_lpET[lpML->magicNum].sort_Type==14 || g_lpET[lpML->magicNum].sort_Type==15)		//2장을 함께 찍는 경우
			Temp->FrameCount+=2;
			/*
			////////////////////////// 0403 lkh 추가 /////////////////////////////
			else if(lpML->lpChar_Own->excute_MagicNum == 51)	//노바인 경우(한 써클괴적당 12개 )
			{
			Temp->total_Frame++;
			//if(Temp->total_Frame%3 == 0)	Temp->FrameCount++;
			Temp->FrameCount++;
			}
		*/
		else
			Temp->FrameCount+=1;
		//////////////////// 0308 lkh 추가 ////////////////////
		if( Temp->FrameCount < EffList[Temp->sprite_Num].MaxFrame )
			Temp->RepeatCount = EffList[Temp->sprite_Num].EffData[Temp->FrameCount].RepeatFrame;
	}
	lpML->loop_Count++;
}

void DrawEffect1(USEEFFECTLIST *Temp, MAGICLIST* lpML, int share, bool target)	//이펙트 스프라이트를 로딩하지 않고 원 이미지 변형하는 경우
// type->trans 방법 / share->신체부위(1~6사이의 숫자로 6자리 이내)
{
	int dir=0;//lpCharacter->direction;
	int sprc=0;//lpCharacter->AnimationTable[ lpCharacter->nCurrentAction].cFrame[  lpCharacter->nCurrentFrame.GetDecrypted() ];
	int i=0;
	int sub=0;
	int x=0,y=0;
	LPCHARACTER		lpChar_Effect;
	
	//	Temp->FrameCount++;
	if(target==0)
		lpChar_Effect=lpML->lpChar_Own;
	else
		lpChar_Effect=lpML->lpChar_Target;
	
	dir=lpChar_Effect->direction;
	sprc=lpChar_Effect->AnimationTable[ lpChar_Effect->nCurrentAction].cFrame[  lpChar_Effect->nCurrentFrame.GetDecrypted() ];
	
	x = lpChar_Effect->x - ( g_Map.x * TILE_SIZE)+1;
	y = lpChar_Effect->y - ( g_Map.y * TILE_SIZE)+1;
	
	switch(share/100000)
	{
	case 1:
		out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=100000;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=200000;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=300000;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=400000;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=500000;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=600000;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share/10000)
	{
	case 1:
		out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=10000;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=20000;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=30000;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=40000;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=50000;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=60000;
		break;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share/1000)
	{
	case 1:
		out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=1000;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=2000;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=3000;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=4000;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=5000;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=6000;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share/100)
	{
	case 1:
		out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=100;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=200;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=300;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=400;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=500;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=600;
		break;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share/10)
	{
	case 1:
		out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=10;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=20;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=30;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=40;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=50;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=60;
		break;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share%10)
	{
	case 1:
		out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
	}
}

bool DrawEffect2(USEEFFECTLIST *Temp, MAGICLIST* lpML, int share, DWORD rgb, bool target)
{
	bool	end=0;
	int		dir=0;//lpCharacter->direction;
	int		sprc=0;//lpCharacter->AnimationTable[ lpCharacter->nCurrentAction].cFrame[  lpCharacter->nCurrentFrame.GetDecrypted() ];
	static int grade=0;
	static int repeat=0;
	static int plus_Grade=4;
	int i=0;
	int sub=0;
	LPCHARACTER	lpChar_Effect;
	share	= 123456;
	
	if(target==0)
		lpChar_Effect=lpML->lpChar_Own;
	else
		lpChar_Effect=lpML->lpChar_Target;
	
	dir=lpChar_Effect->direction;
	sprc=lpChar_Effect->AnimationTable[ lpChar_Effect->nCurrentAction].cFrame[ lpChar_Effect->nCurrentFrame.GetDecrypted() ];
	
	grade+=plus_Grade;
	if(grade>=32 || grade<=0)
	{
		if(grade>=32)	grade=32;
		if(grade<=0)	grade=0;
		plus_Grade*=-1;
		repeat++;
	}
	
	int x = lpChar_Effect->x - ( g_Map.x * TILE_SIZE);
	int y = lpChar_Effect->y - ( g_Map.y * TILE_SIZE);
	
	switch(share/100000)
	{
	case 1:
		out_Result=EffectOutput( x, y+1, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=100000;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=200000;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=300000;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=400000;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=500000;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=600000;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share/10000)
	{
	case 1:
		out_Result=EffectOutput( x, y+1, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=10000;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=20000;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=30000;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=40000;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=50000;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=60000;
		break;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share/1000)
	{
	case 1:		//sorting 고려한 y+1
		out_Result=EffectOutput( x, y+1, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=1000;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=2000;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=3000;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=4000;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=5000;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=6000;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share/100)
	{
	case 1:
		out_Result=EffectOutput( x, y+1, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=100;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=200;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=300;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=400;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=500;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=600;
		break;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share/10)
	{
	case 1:
		out_Result=EffectOutput( x, y+1, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=10;
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=20;
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=30;
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=40;
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=50;
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		sub=60;
		break;
	default:
		sub=0;
	}
	
	share-=sub;
	
	switch(share%10)
	{
	case 1:
		out_Result=EffectOutput( x, y+1, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		break;
	case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		break;
	case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		break;
	case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		break;
	case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
		break;
	case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], 6, 0, 0, 9, rgb);
	}
	
	if(repeat==1)
	{
		repeat=0;
		return 1;
	}
	else
		return 0;
}

void DrawEffect3(USEEFFECTLIST *Temp, MAGICLIST* lpML, bool target)	//이펙트 스프라이트를 로딩하지 않고 원 이미지 변형하는 경우
// type->trans 방법 / share->신체부위(1~6사이의 숫자로 6자리 이내)
{
	int dir=0;//lpCharacter->direction;
	int sprc=0;//lpCharacter->AnimationTable[ lpCharacter->nCurrentAction].cFrame[  lpCharacter->nCurrentFrame.GetDecrypted() ];
	int i=0;
	int sub=0;
	int x=0,y=0;
	LPCHARACTER		lpChar_Effect;
	
	//	Temp->FrameCount++;
	if(target==0)
		lpChar_Effect=lpML->lpChar_Own;
	else
		lpChar_Effect=lpML->lpChar_Target;
	
	dir=lpChar_Effect->direction;
	sprc=lpChar_Effect->AnimationTable[ lpChar_Effect->nCurrentAction].cFrame[  lpChar_Effect->nCurrentFrame.GetDecrypted() ];
	
	//x = lpChar_Effect->x - ( g_Map.x * TILE_SIZE)+1;
	//y = lpChar_Effect->y - ( g_Map.y * TILE_SIZE)+1;
	x = Temp->x - (g_Map.x * TILE_SIZE)+1;
	y = Temp->y - (g_Map.y * TILE_SIZE)+1;
	
	EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level-1);
	EffectOutput( x, y, lpChar_Effect->sp0/*&CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc]*/, lpML->trans_Type, (int)Temp->trans_Level);
	EffectOutput( x, y, lpChar_Effect->sp1/*&CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc]*/, lpML->trans_Type, (int)Temp->trans_Level);
	EffectOutput( x, y, lpChar_Effect->sp2/*&CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc]*/, lpML->trans_Type, (int)Temp->trans_Level);
	EffectOutput( x, y, lpChar_Effect->sp3/*&CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc]*/, lpML->trans_Type, (int)Temp->trans_Level);
	//EffectOutput( x, y, lpChar_Effect->sp4/*&CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc]*/, lpML->trans_Type, (int)Temp->trans_Level);
	
	/*
	switch(share/100000)
	{
	case 1:
	out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
	sub=100000;
	break;
	case 2:
	out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
	sub=200000;
	break;
	case 3:
	out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
	sub=300000;
	break;
	case 4:
	out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
	sub=400000;
	break;
	case 5:
	out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
	sub=500000;
	break;
	case 6:
	out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
	sub=600000;
	default:
	sub=0;
	}
	
	  share-=sub;
	  
		switch(share/10000)
		{
		case 1:
		out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=10000;
		break;
		case 2:
		out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=20000;
		break;
		case 3:
		out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=30000;
		break;
		case 4:
		out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=40000;
		break;
		case 5:
		out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=50000;
		break;
		case 6:
		out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
		sub=60000;
		break;
		default:
		sub=0;
		}
		
		  share-=sub;
		  
			switch(share/1000)
			{
			case 1:
			out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
			sub=1000;
			break;
			case 2:
			out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
			sub=2000;
			break;
			case 3:
			out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
			sub=3000;
			break;
			case 4:
			out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
			sub=4000;
			break;
			case 5:
			out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
			sub=5000;
			break;
			case 6:
			out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
			sub=6000;
			default:
			sub=0;
			}
			
			  share-=sub;
			  
				switch(share/100)
				{
				case 1:
				out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
				sub=100;
				break;
				case 2:
				out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
				sub=200;
				break;
				case 3:
				out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
				sub=300;
				break;
				case 4:
				out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
				sub=400;
				break;
				case 5:
				out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
				sub=500;
				break;
				case 6:
				out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
				sub=600;
				break;
				default:
				sub=0;
				}
				
				  share-=sub;
				  
					switch(share/10)
					{
					case 1:
					out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
					sub=10;
					break;
					case 2:
					out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
					sub=20;
					break;
					case 3:
					out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
					sub=30;
					break;
					case 4:
					out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
					sub=40;
					break;
					case 5:
					out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
					sub=50;
					break;
					case 6:
					out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
					sub=60;
					break;
					default:
					sub=0;
					}
					
					  share-=sub;
					  
						switch(share%10)
						{
						case 1:
						out_Result=EffectOutput( x, y, &CharSpr[lpChar_Effect->sprno].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
						break;
						case 2:
						out_Result=EffectOutput( x, y, &CharSpr[1].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
						break;
						case 3:
						out_Result=EffectOutput( x, y, &CharSpr[2].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
						break;
						case 4:
						out_Result=EffectOutput( x, y, &CharSpr[3].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
						break;
						case 5:
						out_Result=EffectOutput( x, y, &CharSpr[4].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
						break;
						case 6:
						out_Result=EffectOutput( x, y, &CharSpr[5].sp[ dir ][ sprc], lpML->trans_Type, (int)Temp->trans_Level);
						}
*/
Temp->FrameCount++;
}
/*
bool SendServer(int magic_Num, LPCHARACTER scr)
{
return 1;
}
*/
bool DoingImpectEffect(int magic_Num, LPCHARACTER scr, LPCHARACTER dcr, int damage)
{
	DIRECTION	dir;
	
	ChangeDirection(&dir, scr->x, scr->y, dcr->x, dcr->y);
	
	InsertMagic( scr, dcr, magic_Num, 9, 0, 0, 0, 0);
	
	{
		//Damage에 따른 상태창에서의 클라이언트 HP 상태바 조정 출력
	}
	
	return 1;
}

/* 001212 KHS

  39  4 194 0	./effect/start_ice16   2154	100	186 12  2		 16  0  0   0  0 1   0   0 179	0
  
	1. effect_table.ef화일의 처음 필드에서 39를 찾는다.
	2. 2번째 필드는 patterntype.
	3. 3전째 필드는 sprite고유번호 즉, start_ice16은 194이다. 
	4. 이는 시전시 나오는 효과이고. ( 효과음은 2154 )
	5. 발사는 8번째 필드인 186번으로 간다. 
	
	  
		
		  새로운 마법을 추가하기 위해 ..
		  
			priest.txt
			priest_ex.txt에 추가하여 			magic폴더에 넣는다. 
			
			  wizard.txt
			  wizard_ex.txt에 추가하여 			magic 폴더에 넣는다. 
			  
*/

bool RbuttonContinue( int magic_Num, LPCHARACTER scr, LPCHARACTER dcr, int x, int y)
{
	switch(magic_Num)
	{
	case 212:
		InsertMagic(scr, scr, magic_Num, 0, Mox, Moy, 0, 0);
		break;
	}
	return 1;
}

//////////////////////////// 0509 lkh 수정 //////////////////////////////////
// 대상체 크기에 맞는 피스프라이트 포인터 넘겨줄것
BOOL PutDeadEffectBottom(LPCHARACTER ch)	//Effspr[66]
{		
	if( ch==NULL )		return 0;
	if (!system_info.gore) return 0;
	
	Spr*		spr;
	int sprType=0;
	int startFrame=0;
	int currentFrame=0;
	int x=0,y=0;
	int modify_X=0,modify_Y=0;
	
	// mandead1->동그란피,mandead2->약간 옆으로 ,mandead3->사람 죽음피, monsterdead->대형몬스터죽음피
	switch( ch->sprno )
	{	//< CSD-040316
	case 63: // 수호석1 
	case 64: // 수호석2 
	case 65: // 성문1
	case 66: // 성문2
	case 67: // 성문3
	case 91: 
	case 98:	
	case 99:
	case 177: // 성문4 
	case 178: // 수호석3
		{	
			return 1;
		}
	case 0:	 //주인공 여자
	case 1:  //주인공 남자
	case 69: //호비트여
	case 29: //개
		{
			modify_X =- 20;		//뒤로 쓰러짐
			modify_Y =- 15;
			sprType = MANDEAD3;	//적게 퍼지는 피
			break;	
		}
	case 3:	//스콜피언
	case 6:	//슬레임 - 넓은 피(붉은피)->치환:녹색피
		{
			sprType=SLIMEDEAD;
			break;
		}
	case 18:
		{	//키작은 경비원 - 낮은 피
			modify_X =- 40;
			modify_Y =- 30;
			sprType = MANDEAD1;	//흐르는 피
			break;
		}
	case 20:
		{	//뚱뚱한 경비원 - 넓은 피
			modify_X =- 40;
			modify_Y =- 30;
			sprType = MANDEAD2;	//넓게 퍼지는 피
			break;
		}
	case 8:	 // 고블린
	case 11: // 곰
	case 21: // 뚱뚱한 아주마 - 넓은 피
	case 32: // 맷돼지
	case 55: // 불독
		{
			sprType=MANDEAD2;
			break;
		}
	case 22: //낮은 좁은 피
	case 23: //     "
	case 24: //아저씨
	case 53: //양
	case 14: //라이칸스롭
		{
			sprType=MANDEAD3;
			break;
		}
	case 26: //갈색말
	case 27: //검정말
		{
			modify_X=-12;
			modify_Y=-6;
			sprType=MANDEAD3;//MONSTERDEAD;
			break;
		}
	case 28: //닭
	case 30: //소
	case 54: //독수리
	case 33: //늑대
		{
			sprType = MANDEAD2;
			break;
		}
	case 56:	//테페리
		{
			modify_X = 15;
			modify_Y = 15;
			sprType = MANDEAD2;
			break;
		}
	case  4:
	case 43:
	case 47: //도마뱀 인간(녹색피)->붉은피치환
	case 42: // 무지나
	case 62: // 오크2
		{
			sprType=MONSTERDEAD;
			break;
		}
	case 52:
		{	//물 흰개미
			sprType=ANTDEAD;
			break;
		}
	case 19:
		{	//키큰 경비병
			modify_X = 60;
			modify_Y = 45;
			sprType = MANDEAD3;
			break;
		}
	case 78:
		{	//default
			modify_X =- 16;
			modify_Y =- 16;
			sprType = MANDEAD2;
			break;
		}
	case 58:
		{	//신자1
			modify_X = 8;
			modify_Y = 8;
			sprType = MANDEAD1;
			break;
		}
	case 80:
		{	// 무명캐릭
			modify_X=20;
			modify_Y=20;
			sprType=MANDEAD3;
			break;
		}
	case 71:	//default	
	case 72:	//default
	case 73:	//default
	case 74:	//default
	case 75:	//default
	case 76:	//default
	case 77:	//default
	case 79:	//default
		{
			sprType=MANDEAD3;
			break;
		}
	}	//> CSD-040316
	
	switch(ch->direction)
	{
	case DIRECTION_DOWN:
		x=ch->x-Mapx;
		y=ch->y-Mapy+modify_Y;
		break;
	case DIRECTION_LEFTDOWN:
		x=ch->x-Mapx-(int)(modify_X*.75);
		y=ch->y-Mapy+(int)(modify_Y*.75);
		break;
	case DIRECTION_LEFT:
		x=ch->x-Mapx-modify_X;
		y=ch->y-Mapy;
		break;
	case DIRECTION_LEFTUP:
		x=ch->x-Mapx-(int)(modify_X*.75);
		y=ch->y-Mapy-(int)(modify_Y*.75);
		break;
	case DIRECTION_UP:
		x=ch->x-Mapx;
		y=ch->y-Mapy-modify_Y;
		break;
	case DIRECTION_RIGHTUP:
		x=ch->x-Mapx+(int)(modify_X*.75);
		y=ch->y-Mapy-(int)(modify_Y*.75);
		break;
	case DIRECTION_RIGHT:
		x=ch->x-Mapx+modify_X;
		y=ch->y-Mapy;
		break;
	case DIRECTION_RIGHTDOWN:
		x=ch->x-Mapx+(int)(modify_X*.75);
		y=ch->y-Mapy+(int)(modify_Y*.75);
		break;
	}
	
		//////////////////////// 0706 lkh 수정(버그퇴치) ////////////////////////
		if( (effSpr_Count[sprType]==0 || Effspr[sprType].EffSpr->img==NULL ) && sprType !=0 )	//처음 로딩되는 경우(magicNum는 0~499까지)	
		{
			char buf[30] = {0,};
			strcpy( buf,g_lpET2[sprType].sprite_Name );		//020808 YGI 
			strcat( buf ,".dat" );

#ifdef _EFFECT_DATA_USING_DIVIDED_FILE // 030821 kyo //kyo
			::LoadEffectSpriteDivFile( g_lpET[magicNum].sprite_Num );
#else
			::LoadEffectSprite(g_lpET[sprType].sprite_Num, buf);
#endif
		}

	startFrame=(ch->direction)*(Effspr[sprType].Count/8);	//방향에 따른 시작 프레임 정의
	currentFrame=(80-(ch->dieani))/2;
	if(currentFrame>5)	currentFrame=5;
	//currentFrame=(Effspr[sprType].Count/8-1)-((Effspr[sprType].Count/8-1)*ch->dieani/80);
    spr=&Effspr[sprType].EffSpr[startFrame+currentFrame];
	
	if(ch->dieani>32)
		PutCompressedImage(x, y, spr );
	else 
		PutCompressedImageFX(x, y, spr, ch->dieani, 1);
	
	return 1;
}	

/////////////////////////// 0417 lkh /////////////////////////////
// 만약 magicSubNum이 1이면 앞으로 분신과 함께 캐릭이 진행되며 공격하는 패턴
//			"		  2이면 먼저 분신이 나가면서 공격한후 마지막에 캐릭이 순간 이동 
MAGICLIST* Trace(MAGICLIST* lpML)		//캐릭터 및 아이템의 잔상/이미지의 잔상 마법
{
	USEEFFECTLIST* lpEL = lpML->EffHead;	//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소를 넘겨줌 
	LPITEMGROUND	lpItem = NULL;
	
	static int inc = 0;
	int	crash_Result=0;
	int backup_LevlpEL=0;
	int change_X=0,	change_Y=0;
	int	oldChange_X=0,	oldChange_Y=0;
	
	BOOL StartFlag = TRUE;
	
	switch(lpML->PCount)		//매직리스트의 실행되는 동일 마법의 갯수
	{
	case 0:
		{
			ChangeDirection((DIRECTION*)&lpML->lpChar_Own->direction, lpML->x, lpML->y, lpML->tarx, lpML->tary);
			lpML->lpChar_Own->todir = lpML->lpChar_Own->direction;
			CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, lpML->lpChar_Own->nCurrentAction);		//ACTION_ATTACK_ONESWORD2
			lpML->dir = lpML->lpChar_Own->direction;
			InsertEffect(lpML);
			lpML->PCount = 1;//매직리스트에 마법 하나 등록된 사실 셋팅
		}break;
	case 1:		//1개 등록된 경우
		{
			while(1)
			{
				if(lpML->EffHead == NULL)		//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
				{
					const int	tile_X= lpML->lpChar_Own->x/32;
					const int	tile_Y= lpML->lpChar_Own->y/32;
					
					MAGICLIST* buff = lpML->Next;//먼저 주인공이 서있는 타일 위치를 알아냄
					
					if( lpML->lpChar_Own->changeActionFlag == 1 )
					{
						lpML->lpChar_Own->changeActionFlag = 0;
					}
					
					CharDoAni( lpML->lpChar_Own , lpML->lpChar_Own->direction , lpML->lpChar_Own->basicAction);
					lpML->EffectCount = 0;
					
					//매직의 서브넘버가 2인 경우는 먼저 분신이 나가고 캐릭이 앞으로 순간 이동
					if (lpML->magicSubNum == 2)
					{ //< CSD-020820
						for (int ii=0; ii < 10; ++ii)
						{
							oldChange_X = change_X;
							oldChange_Y = change_Y;
							
							int nDiffX = lpML->lpChar_Target->x - lpML->lpChar_Own->x;
							if (nDiffX == 0)  nDiffX = 1;
							int nDiffY = lpML->lpChar_Target->y - lpML->lpChar_Own->y;
							
							switch (lpML->lpChar_Own->direction)
							{
							case 0:                 change_Y = 32;                break;
							case 1: change_X = -32; change_Y = -32*nDiffY/nDiffX;  break;
							case 2: change_X = -32;                               break;
							case 3: change_X = -32; change_Y = -32*nDiffY/nDiffX; break;
							case 4:                 change_Y = -32;               break;
							case 5: change_X = 32;  change_Y = 32*nDiffY/nDiffX; break;
							case 6: change_X = 32;                                break;
							case 7: change_X = 32;  change_Y = 32*nDiffY/nDiffX;  break;
							}
							
							lpML->lpChar_Own->x += change_X;
							lpML->lpChar_Own->y += change_Y;
							
							if (lpML->lpChar_Own->x == lpML->lpChar_Target->x && lpML->lpChar_Own->y == lpML->lpChar_Target->y)
							{
								break;
							}
							
							if (!IsMovable(lpML->lpChar_Own->x/32, lpML->lpChar_Own->y/32))
							{
								lpML->lpChar_Own->x -= change_X;
								lpML->lpChar_Own->y -= change_Y;
								goto LABELLKH;
							}
						}
						
						lpML->lpChar_Own->x -= change_X;
						lpML->lpChar_Own->y -= change_Y;
					} //> CSD-020820
					
					CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_ATTACK_ONESWORD2);
					
					if (Hero == lpML->lpChar_Own)
					{
						g_mgrBattle.SendPcAttack(Hero, lpML->lpChar_Target);
					}
LABELLKH:
					DeleteList(&g_lpML, lpML);
					if (lpEL == NULL)
					{
						return buff;
					}
				}
				
				////////////////// 최대 생성 이펙트보다 많은 경우 종료 //////////////////
				if( lpML->EffectCount >= lpML->object_Num )
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->EffectCount--;
					lpML->flag=1;
					continue;
				}
				
				if(lpML->flag == 1)
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->EffectCount--;
					continue;
				}
				
				//이동불가 지형타일과 충돌시 소멸시킬까?
				if(TileMap[(lpEL->x+(int)lpEL->incx)/32][(lpEL->y+(int)lpEL->incy)/32].attr_dont==1)
				{
					//InsertMagic(lpML->lpChar_Own, lpML->lpChar_Target,lpML->next_Effect,  9,  0,  0,  (int)(((lpEL->x+lpEL->oldincx)/32.)*32.+16.), (int)(((lpEL->y+lpEL->oldincy)/32.)*32.+16.) );
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->EffectCount--;
					lpML->flag=1;
					continue;
				}
				
				int check_xl = (int)(lpML->lpChar_Own->sp->xl);
				int check_yl = (int)(lpML->lpChar_Own->sp->yl);
				
				if( lpEL->FrameCount > (int)(lpML->object_Num/3) )
				{
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->EffectCount--;
					lpML->flag=1;
					continue;
				}
				
				//발사 이펙트가 적 케릭터에 맞았는지 체크(첫번째인자 1인 경우->복수 타일 체크 & 충돌처리를 함수에 맞김/마지막 인자->내 자신(시전자) 체크여부)
				attacked_Char=ReturnMagicCheckedCharacter(0, lpML, (lpEL->x+(int)(lpEL->incx+.5)+EffList[lpEL->sprite_Num].offset_X-(check_xl/2)), (lpEL->y+(int)(lpEL->incy+.5)+EffList[lpEL->sprite_Num].offset_Y-check_yl/2), (int)lpEL->height, check_xl, check_yl );
				//if(attacked_Char!=NULL && !(attacked_Char->nCurrentAction == MON1_ATTACKED || attacked_Char->nCurrentAction == ACTION_ATTACKED)) //충돌한 인물이 있으면
				if (attacked_Char != NULL) //충돌한 인물이 있으면
				{
					RandomAttackEffect(METAL_EFFECT, lpML->lpChar_Own, lpML->lpChar_Target);
					DeleteList(&(lpML->EffHead), lpEL);
					lpEL = lpML->EffHead;
					lpML->EffectCount--;
					lpML->flag=1;
					continue;
				}
				else
				{
					lpItem=ReturnMagicCheckedItem(0, lpML, (int)((lpEL->x+lpEL->incx+EffList[lpEL->sprite_Num].offset_X)-(check_xl/2.)), (int)((lpEL->y+lpEL->incy+EffList[lpEL->sprite_Num].offset_Y)-(check_yl/2.)), (int)lpEL->height, check_xl, check_yl);
					if(lpItem!=NULL)
					{
						DeleteList(&(lpML->EffHead), lpEL);
						lpEL = lpML->EffHead;
						lpML->EffectCount--;
						lpML->flag=1;
						continue;
					}
				}
				
				if (lpEL->Next == NULL)
				{
					break;
				}
				else 
				{
					lpEL = lpEL->Next;
				}
			}
			
			lpEL = lpML->EffHead;
			lpML->spr_Speed++;
			while(1)
			{
				if (lpEL == NULL) break;
				
				if( (inc%3 == 0) && ( lpML->EffectCount < lpML->object_Num) && (StartFlag == 1) && (lpML->flag==0))
				{
					switch(lpML->dir)		//가속처리(가속처리 싫으면 While문 바깥으로 뺄것)
					{
					case 0:	//down
						lpML->y += (int)(lpML->spr_Speed);
						break;
					case 1:	//leftdwon
						lpML->x -= (int)(lpML->spr_Speed*3/4);
						lpML->y += (int)(lpML->spr_Speed*3/4);
						break;
					case 2:	//left
						lpML->x -= (int)(lpML->spr_Speed);
						break;
					case 3:	//leftup
						lpML->x -= (int)(lpML->spr_Speed*3/4);
						lpML->y -= (int)(lpML->spr_Speed*3/4);
						break;
					case 4:	//up
						lpML->y -= (int)(lpML->spr_Speed);
						break;
					case 5:	//rightup
						lpML->x += (int)(lpML->spr_Speed*3/4);
						lpML->y -= (int)(lpML->spr_Speed*3/4);
						break;
					case 6:	//right
						lpML->x += (int)(lpML->spr_Speed);
						break;
					case 7:	//rightdown
						lpML->x += (int)(lpML->spr_Speed*3/4);
						lpML->y += (int)(lpML->spr_Speed*3/4);
						break;
					}
					
					if( !TileMap[(int)(lpML->x/TILE_SIZE)][(int)(lpML->y/TILE_SIZE)].attr_dont && lpML->magicSubNum == 1 )
					{
						lpML->lpChar_Own->x = lpML->x;
						lpML->lpChar_Own->y = lpML->y;
					}
					
					// 3번 loop마다 이펙트 인서트
					int temp_transLevel = lpML->trans_Level;
					lpML->trans_Level -= (int)((float)(lpML->trans_Level/lpML->object_Num)*lpML->EffectCount);
					if(lpML->trans_Level<2)
					{
						lpML->trans_Level=2;
					}
					
					if(lpML->flag == 0)
					{
						InsertEffect(lpML);
						lpML->EffectCount++;
					}
					
					if( lpML->EffectCount == 1 )
					{
						CharDoAni(lpML->lpChar_Own, lpML->lpChar_Own->direction, ACTION_ATTACK_ONESWORD2);
					}
					lpML->trans_Level = temp_transLevel;
				}
				
				lpEL->trans_Level -= (lpEL->trans_Level/3.f);
				if(lpEL->trans_Level<2) 
				{
					lpEL->trans_Level=2;
				}
				
				DrawEffect3(lpEL, lpML, 0);
				inc++;
				if(inc>1000)
				{
					inc=0;
				}
				
				if (lpEL->Next == NULL)
				{
					break;
				}
				else 
				{
					lpEL = lpEL->Next;
				}
			}
		}break;
	}
	return lpML->Next;
}

MAGICLIST*	RadiantStar(MAGICLIST*	lpML)		//특정 위치에 일정 패턴으로 움직이는 빛 조각들(1:아래로 떨어지는/2:시계방향으로 원운동/3:주변으로 퍼져 나가는)
{
	USEEFFECTLIST*  lpEL;
	int temp_X=0,temp_Y=0;
	
	lpEL=lpML->EffHead;
	
	switch(lpML->PCount)
	{
	case 0:
		temp_X = lpML->x;
		temp_Y = lpML->y;
		lpML->x += rand()%41-20;
		lpML->y += rand()%41-20;
		lpML->dir = rand()%5;
		InsertEffect(lpML);
		lpML->x = temp_X;
		lpML->y = temp_Y;
		//효과리스트에 마법 하나 등록된 사실 셋팅
		lpML->PCount = 1;
		break;
		
	case 1:
		while(1)	//종결조건 확인하여 매직 리스트에서 해당 이펙트를 제거
		{
			if (lpML->EffHead == NULL)			//MAGICLIST와 연결되어 있는 USEEFFECTLIST의 주소값 없을 때
			{
				MAGICLIST* buff;
				
				buff = lpML->Next;				//다음 매직리스트링트의 주소값을 임시 기억
				
				lpML->EffectCount = 0;			//실행되는 동일 이펙트수를 0으로 초기화
				DeleteList(&g_lpML, lpML);		//실제 이펙트링크가 존재하지 않는 매직리스트를 지워버림	
				
				if (lpEL == NULL)
				{
					return buff;
				}
			}
			
			// 최종 프레임에 도달하고 마법리스트의 동일 효과 반복횟수 만큼 출력한 경우
			//if( lpEL->FrameCount!=0 && lpEL->FrameCount%(EffList[lpEL->sprite_Num].MaxFrame/5)==0 /*&& lpML->object_Num<=0*/)
			if( (lpEL->FrameCount+1)%11 == 0 /*EffList[lpEL->sprite_Num].MaxFrame*/)		//각 10프레임 마다 다른 색의 이미지 가지고 있음
			{
				lpEL->FrameCount=0;
				DeleteList(&(lpML->EffHead), lpEL);
				lpEL = lpML->EffHead;
				continue;
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////////////////				
			if (lpEL->Next == NULL)			//이펙트 리스트의 다음 이펙트가 존재하지 않을 경우
				break;
			else lpEL = lpEL->Next;	//다음 이펙트 리스트로 이동
		}
		
		lpEL = lpML->EffHead;
		
		while(lpEL!=NULL)	//실제 이펙트를 연결시켜주는 루틴
		{
			//동시 출력하는 이팩트의 갯수가 복수일 때 
			if(lpML->EffectCount < lpML->object_Num-1 /*&& lpEL->FrameCount%(int)(EffList[lpEL->sprite_Num].MaxFrame/5)==0*/ )
			{
				temp_X = lpML->x;
				temp_Y = lpML->y;
				lpML->x += rand()%41-20;
				lpML->y += rand()%41-20;
				lpML->dir = rand()%5;
				InsertEffect(lpML);
				lpML->EffectCount ++;
				lpML->x = temp_X;
				lpML->y = temp_Y;
			}
			
			//연결프레임과 현재 프레임 동일하면
			if( lpML->link_Frame!=0 && lpML->link_Frame==lpEL->FrameCount)
			{
				//시전자의 방향과 액션 형태를 조정
				if(lpML->magicSubNum==8)		InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,9,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);
				else							InsertMagic(lpML->lpChar_Own,lpML->lpChar_Target,lpML->next_Effect,lpML->magicSubNum,lpML->s_X,lpML->s_Y, lpML->t_X, lpML->t_Y);	
			}
			
			if(lpML->trans_Type%10==8)		//점점 밝게 생성 / 마지막 어둡게
			{
				if(lpEL->FrameCount<EffList[lpEL->sprite_Num].MaxFrame-4)
				{
					lpEL->trans_Level=(lpML->trans_Level/(float)EffList[lpEL->sprite_Num].MaxFrame)*lpEL->FrameCount;
					if(lpEL->trans_Level > lpML->trans_Level)	lpEL->trans_Level=(float)lpML->trans_Level;
				}
				else
				{
					lpEL->trans_Level-=(float)(lpML->trans_Level/4.f);
					if(lpEL->trans_Level < 1) lpEL->trans_Level = 1;
				}
			}
			
			// 마지막 4프레임은 alpha off
			else if(lpML->trans_Type%10==9 && lpEL->FrameCount>=EffList[lpEL->sprite_Num].MaxFrame-4)	
			{
				lpEL->trans_Level-=(lpML->trans_Level/4);//-((lpML->trans_Level/EffList[lpEL->sprite_Num].MaxFrame)*(lpEL->FrameCount-1));
				if(lpEL->trans_Level<=1)	lpEL->trans_Level=1;
			}
			
			else if(lpML->trans_Type%10==7)	//바닥이 얼어붙는 효과
			{
				if(lpEL->FrameCount<=EffList[lpML->sprite_Num].MaxFrame/2)
				{
					lpEL->trans_Level=(float)lpEL->FrameCount;
					if(lpEL->trans_Level>=32)	lpEL->trans_Level=32;
				}
				else
				{
					lpEL->trans_Level=(float)(EffList[lpML->sprite_Num].MaxFrame-lpEL->FrameCount);
					if(lpEL->trans_Level<0) lpEL->trans_Level=0;
				}
			}
			
			else if(g_lpET[lpML->magicNum].end_Time!=0)			//쉴드와 같은 장시간 유지 마법의 경우 색깔값 지속적 떨어트림
			{
				lpEL->trans_Level=(float)(lpML->trans_Level*(g_lpET[lpML->magicNum].end_Time-lpEL->total_Frame)/g_lpET[lpML->magicNum].end_Time);
				if(lpEL->trans_Level<0)	lpEL->trans_Level=0;
			}
			
			// 실제 출력 -> y= -ax2+c (아래로 떨어지는 포물선 방정식)
			switch(lpML->magicSubNum)	//왼쪽으로 튀어 올라가기
			{
			case 1:		//아래로 가속도를 내면서 떨어짐(좌우로 약간 흔들림)
				lpEL->incx += (rand()%5)-2.f;
				lpEL->incy += (float)lpEL->FrameCount;
				break;
			case 2:		//생성점을 기준으로 원운동을 하는 패턴
				switch(lpEL->effect_Count%5)//rand()%5)
				{
				case 0:
					lpEL->incx += (float)( cos (lpEL->FrameCount%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) );
					lpEL->incy += (float)( sin (lpEL->FrameCount%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) ) + (float)lpEL->FrameCount;
					break;
				case 1:
					lpEL->incx += (float)( cos ((lpEL->FrameCount+2)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) );
					lpEL->incy += (float)( sin ((lpEL->FrameCount+2)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) ) + (float)lpEL->FrameCount;
					//lpEL->incx += ( cos ( (360 - lpEL->FrameCount%10*(36+rand()%9-4)) * M_PI / 180. ) * (lpML->EffectCount+4) );
					//lpEL->incy += ( sin ( (360 - lpEL->FrameCount%10*(36+rand()%9-4)) * M_PI / 180. ) * (lpML->EffectCount+4) ) + (float)lpEL->FrameCount;
					break;
				case 2:
					//lpEL->incx += ( cos ((lpEL->FrameCount+5)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount+4) );
					//lpEL->incy += ( sin ((lpEL->FrameCount+5)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount+4) ) + (float)lpEL->FrameCount;
					lpEL->incx += (float)( cos ((lpEL->FrameCount+4)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) );
					lpEL->incy += (float)( sin ((lpEL->FrameCount+4)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) ) + (float)lpEL->FrameCount;
					break;
				case 3:
					lpEL->incx += (float)( cos ((lpEL->FrameCount+6)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) );
					lpEL->incy += (float)( sin ((lpEL->FrameCount+6)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) ) + (float)lpEL->FrameCount;
					//lpEL->incx += ( cos ( (360 - (lpEL->FrameCount+5)%10*(36+rand()%9-4)) * M_PI / 180. ) * (lpML->EffectCount+4) );
					//lpEL->incy += ( sin ( (360 - (lpEL->FrameCount+5)%10*(36+rand()%9-4)) * M_PI / 180. ) * (lpML->EffectCount+4) ) + (float)lpEL->FrameCount;
					break;
				case 4:
					lpEL->incx += (float)( cos ((lpEL->FrameCount+8)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) );
					lpEL->incy += (float)( sin ((lpEL->FrameCount+8)%10*(36+rand()%9-4) * M_PI / 180. ) * (lpML->EffectCount/2+2) ) + (float)lpEL->FrameCount;
				}
				break;
				case 3:		//포물선 형태
							/*
							if(lpEL->effect_Count%2)
							lpEL->incx += 3.f/(lpEL->FrameCount+1.f);
							else
							lpEL->incx += -(3.f/(lpEL->FrameCount+1.f));
							lpEL->incy += (float)lpEL->FrameCount*1.7f;		//sqrt( lpEL->total_Frame*grade );		//아래로 향하는 포물선
					*/
					if(lpEL->effect_Count%2)
						lpEL->incx = (float)(lpEL->FrameCount*2.);
					else
						lpEL->incx = (float)(-lpEL->FrameCount*2.);
					lpEL->incy = (float)(2.*pow((float)abs(lpEL->FrameCount)-3,(float)2)+3);		//sqrt( lpEL->total_Frame*grade );		//아래로 향하는 포물선
					break;
			}
			
			DrawEffect(lpEL, lpML, g_lpET[lpML->magicNum].light, 0);
			lpEL->total_Frame ++;
			lpEL = lpEL->Next;
			}
			break;
	}
	return lpML->Next;
}	

void ResetCharacter(LPCHARACTER lpChar)
{	
	lpChar->excute_MagicNum=0;
	lpChar->ready_Magic=0;
	lpChar->end_Casting=0;
	lpChar->end_ReadyMagic=0;
	lpChar->end_RunMagic=0;
	lpChar->bow_Type=0;
	lpChar->castStartingTime = 0;
	
	if( lpChar->viewtype != VIEWTYPE_GHOST_ )
	{
		lpChar->viewtype = VIEWTYPE_NORMAL_;
		lpChar->condition = CON_NORMAL;
	}
}	

void ResetEffect(LPCHARACTER lpChar)
{ 
	MAGICLIST* pTemp = g_lpML;
	
	while (pTemp != NULL)
	{
		MAGICLIST* pNext = pTemp->Next;
		DeleteList(&g_lpML, pTemp);
		pTemp = pNext;	
	}
	
	lpChar->continue_RunMagic = 0; // CSD-021128
} 

BOOL NPC_MovePattern(LPCHARACTER ch)
{
	int pattern = 0;
	
	return TRUE;
}


void Log_BattleSave( char* filename, bool attack_Type, BATTLERESULTSAVE BR_Save)
{
	FILE* file;
	int buff = 0;
	
	//ar  DataPath[MAX_PATH]="./effect/effect_table.et";
	
	file = Fopen( filename, "at+" );			//read only+text file
	
	SCharacter *ch = &SCharacterData;
	
	if(file)
	{
		if(attack_Type==1)
		{
			fprintf( file,lan->OutputMessage(3,17),//010215 lsw
				BR_Save.Result, 
				BR_Save.caster_Name, 
				BR_Save.Level, 
				ch->nExp,
				BR_Save.MagicType, 
				BR_Save.Damage, 
				BR_Save.Change_MP, 
				BR_Save.target_Name,
				BR_Save.target_Statue, 
				ch->nCharacterHP,
				ch->nCharacterMP,
				ch->condition,
				ch->nCharacterAbility[STR],
				ch->nCharacterAbility[DEX],
				ch->nCharacterAbility[CON],
				ch->nCharacterAbility[WIS],
				ch->nCharacterAbility[INT_],
				ch->nCharacterAbility[CHA],
				ch->nCharacterAbility[MOVP],
				ch->nCharacterAbility[ENDU],
				ch->nCharacterAbility[MOR],
				ch->nCharacterAbility[WSPS], 
				ch->nCharacterAbility[LUCK] );
		}
		else 
		{
			fprintf( file, lan->OutputMessage(3,18),//010215 lsw
				BR_Save.Result, 
				BR_Save.caster_Name, 
				BR_Save.Level, 
				ch->nExp,
				BR_Save.MagicType, 
				BR_Save.Damage, 
				BR_Save.target_Name,
				BR_Save.target_Statue, 
				ch->nCharacterHP,
				ch->nCharacterMP,
				ch->condition,
				ch->nCharacterAbility[STR],
				ch->nCharacterAbility[DEX],
				ch->nCharacterAbility[CON],
				ch->nCharacterAbility[WIS],
				ch->nCharacterAbility[INT_],
				ch->nCharacterAbility[CHA],
				ch->nCharacterAbility[MOVP],
				ch->nCharacterAbility[ENDU],
				ch->nCharacterAbility[MOR],
				ch->nCharacterAbility[WSPS], 
				ch->nCharacterAbility[LUCK] );
		}
		
		fclose(file);
	}
}

bool IsApplyPK(int nMagic)
{	//< CSD-031014 : PK 적용이 가능한지 여부 판단
	switch (nMagic)
	{	
    case THROW_ARROW:
    case THROW_KNIFE:
	case THROW_FIRE_ARROW:
	case THROW_ICE_KNIFE:
	case THROW_SNOW_BALL:
	case THROW_WATER_BALL:
	case THROW_BIG_GULP:
		{
			return false;
		}
	}

	switch (nMagic)
	{
	case ITEM_EFFECT_FIRE:
	case ITEM_EFFECT_ICE:
	case ITEM_EFFECT_LIGHTNING:
		{
			return true;
		}
	}

	switch (g_lpMT[nMagic].magic_Type/10)
	{
    case 4:              // 저주계열의 마법인 경우
    case 5: return true; // 공격계열의 마법인 경우
	}
	
	return false;
}	//> CSD-031014

bool IsHarmfulMagic(int nMagic)
{ // 해로운 마법(공격, 저주계열의 마법인지 여부 판단
	switch (g_lpMT[nMagic].magic_Type/10)
	{
    case 4:              // 저주계열의 마법인 경우
    case 5: return true; // 공격계열의 마법인 경우
	}
	
	return false;
}

bool IsPointAttackMagic(int nMagic)
{ // 대상지정 공격 마법이지 여부 판단
	if (g_lpMT[nMagic].magic_Type/10 == 4)
	{
		switch (g_lpMT[nMagic].avail_Type)
		{
		case 2:              // 단일 대상자 공격
		case 3: return true; // 복수 대상자 공격
		}
	}
    
	return false;
}

int GetRiseFall(int nX1, int nY1, int nX2, int nY2)
{
	if (TileMap[nX1][nY1].empty__4 && !TileMap[nX2][nY2].empty__4)  return 1;
	if (!TileMap[nX1][nY1].empty__4 && TileMap[nX2][nY2].empty__4)  return 2;
	if (TileMap[nX1][nY1].empty__4 && TileMap[nX2][nY2].empty__4)  return 3;
	if (!TileMap[nX1][nY1].empty__4 && !TileMap[nX2][nY2].empty__4)  return 3;
	return 0;          
}

bool IsCollision(int nMagic, int nX1, int nY1, int nX2, int nY2)
{
	if (TileMap[nX2][nY2].attr_light)  return true;
	if (TileMap[nX2][nY2].empty__4)
	{
		const BYTE nRiseFall = GetRiseFall(nX1, nY1, nX2, nY2);
		
		switch (g_lpMT[nMagic].nRiseFall)
		{
		case 1: if (nRiseFall == 1 || nRiseFall == 2)  return true;
		case 2: if (nRiseFall == 2)  return true;
		case 3: if (nRiseFall == 1)  return true;
		}
	}
	
	return false;
}

bool IsFuntionItemEffect(int nEffect)
{	//< CSD-030422
	switch (nEffect)
	{
	case 474:
	case 475:
	case 476:
	case 477:
	case 478:
	case 479:
	case 480:
	case 481:
	case 482:
	case 483:
	case 484:
	case 485:
	case 486:
	case 487:
	case 488:
	case 489:
		{
			return true;
		}
	}

	return false;
}	//> CSD-030422

extern bool IncordFile( char *filename, void *pData, int size );
extern bool DecoadFile( char *filename, void *pData, int size );
void SaveEffectAnimationAll( char *pAlreadyRead )
{
	EffectListTblBasic *pSaveData;
	pSaveData = new EffectListTblBasic[MAX_EFFECT_COUNT];
	memset( pSaveData, 0, sizeof(EffectListTblBasic)*MAX_EFFECT_COUNT );
	
	for( int i=0; i<MAX_EFFECT_COUNT; i++ )
	{
		if( pAlreadyRead[i] )
		{
			EffList[i].CopyOut( pSaveData[i] );
			pSaveData[i].check_sum2 = EffList[i].Check();
		}
	}
	IncordFile( "./Data/NewSelectMapTmp.dat", pSaveData, sizeof( EffectListTblBasic )*MAX_EFFECT_COUNT );
	SAFE_DELETE_ARRAY(pSaveData);
}

bool LoadEffectAnimationAll()
{
	CEffectListTbl data;		// 일단 한줄씩 받아올 임시 버퍼
	EffectListTblBasic *pTargetData;
	pTargetData = new EffectListTblBasic[MAX_EFFECT_COUNT];
	int ret = DecoadFile( "./Data/NewSelectMapTmp.dat", pTargetData, sizeof( EffectListTblBasic )*MAX_EFFECT_COUNT );
	if( !ret ) 
	{
		SAFE_DELETE_ARRAY(pTargetData);
		return false;
	}
	for( int i=0; i<MAX_EFFECT_COUNT; i++ )
	{
		data.CopyIn( pTargetData[i] );
		EffList.LoadFromClass( i, &data, pTargetData[i].check_sum2 );
	}
	SAFE_DELETE_ARRAY(pTargetData);
	return true;
}

void CollisionEffect_Keeping(USEEFFECTLIST* lpEL, MAGICLIST*	lpML, const int nRan) // 030519 kyo
{	//이벡트충돌로 안한 효과를 보여준다. 
	//어떤 범위에 이펙트가 나오는 만큼 충돌 효과준다.
	
	//if( 1!=lpEL->FrameCount && rand()%100 != 0 )
	if( 1!=lpEL->FrameCount || rand()%nRan != 0 )
	//if(EACH_FRMAE != s_nCount)// && rand()%10 != 0)
	{
		return;
	}
	
	int nXl = g_lpMT[lpML->magicSerial].avail_Range*TILE_SIZE; //타일반경
	int nYl = nXl*3/4;
	//lpML->flag=1;
	LPCHARACTER attacked_Char  = ReturnMagicCheckedCharacter(1, lpML, lpML->tarx-nXl,lpML->tary-nYl,(int)lpEL->height,	(nXl*2), (nYl*2));//, 2);

}

bool LoadEffectSpriteDivFile( const int nSprite_Num) // 030821 kyo
{
	DWORD dwSize=0;
	FILE *fp = g_MergeData.GetEffectFileHandle(nSprite_Num, dwSize);

	if( 0>=dwSize || NULL == fp)
	{
		return false;
	}
	fread( &Effspr[nSprite_Num].Count, 2,1, fp);						//이펙트 스프라이트의 프레임수		
	fseek( fp, Effspr[nSprite_Num].Count * 4, SEEK_CUR );			//오프셋 스킵
	if( 0>=dwSize )
	{
		return false;
	}
	int nSize = dwSize - Effspr[nSprite_Num].Count * 4 * 2 - 2;
	BYTE  *Buff = NULL;
	MemAlloc( Buff, nSize);	
	
	DWORD templen = 0, size=0, nOff=0;	
	for( int i = 0 ; i < Effspr[nSprite_Num].Count ; i++)
	{
		fread( &size, 4,1, fp);
		
		fread( &Effspr[nSprite_Num].EffSpr[i].xl, sizeof( short ), 1, fp);	//X축의 스프라이트 크기
		fread( &Effspr[nSprite_Num].EffSpr[i].yl, sizeof( short ), 1, fp);	//Y축의 스프라이트 크기
		fread( &Effspr[nSprite_Num].EffSpr[i].ox, sizeof( short ), 1, fp);	//X축의 offset 위치
		fread( &Effspr[nSprite_Num].EffSpr[i].oy, sizeof( short ), 1, fp);	//Y축의 offset 위치
		fread( &Effspr[nSprite_Num].EffSpr[i].size, sizeof( unsigned int ), 1, fp);	//스프라이트 이미지의 크기
		
		fread( Buff + nOff, Effspr[nSprite_Num].EffSpr[i].size,1 ,fp);		//스프라이트의 실제 이미지
		Effspr[nSprite_Num].EffSpr[i].img = (char *)Buff + nOff;
		
		convert565to555( &Effspr[nSprite_Num].EffSpr[i] );
		
		templen += Effspr[nSprite_Num].EffSpr[i].size;
		nOff += size;
	}

	fclose(fp);
	return true;	
}

extern void ThunderStart( int number, int flag ); //Eleval 12/08/09 - Thunderstorm

bool PlayedThunder = false;

void PlayThunder() //Eleval 12/08/09 - To play thunder sound in case of thunder storm
{
	if(!g_IsThunderStorm) return;
	if(last_thunder_time == -1)
	{
		last_thunder_time = g_curr_time;
		thunder_wait_time = rand() % 180;
		//thunder_wait_time = 10;
		return;
	}

	if(bLowRain && thunder_wait_time <= 90)
		ThunderStart( 500, 0 );

	thunder_wait_time -= g_curr_time - last_thunder_time;
	last_thunder_time = g_curr_time;

	if(thunder_wait_time < 3 && !PlayedThunder)
	{
		//ColorCtrlBrightness( 255 );
		PlayedThunder = true;
	}

	if(thunder_wait_time <= 0)
	{
		if(rand()%2 == 0)
			MP3( SOUND_LIGHTNING1 );
		else
			MP3( SOUND_LIGHTNING2 );

		//EffectOutput(20, 20, &Effspr[g_lpET[LIGHTNING_BOLT].sprite_Num].EffSpr[EffList[g_lpET[LIGHTNING_BOLT].sprite_Num].EffData[0].SprNo], 1);
		
		last_thunder_time = g_curr_time;
		thunder_wait_time = rand() % 180 + 10;
		//thunder_wait_time = 10;

		if(thunder_wait_time >= 180)
			bLowRain = true;

		ThunderStart( 900, 0 );
		PlayedThunder = false;
	}
}

void PlayRainSound() //Eleval 12/08/09 - To loop rain sound
{
	if(Statue_Weather != 1)
		return;

	if(IsPlaying(SOUND_RAINY))	return;

	PlayListAutoSounds(SOUND_RAINY, 0, 0,0);
}