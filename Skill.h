// CrackMgr.h: interface for the CCrackMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLMGR_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_)
#define AFX_SKILLMGR_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SKILL_READY			1
#define SKILL_ON			2
#define SKILL_OFF			0
#define SKILL_ING			3
#define SKILL_BEFORE_ING	4

// Skill Target Type..
#define SKILLTARGETTYPE_ITEM_		10
#define SKILLTARGETTYPE_CHAR_		20
#define SKILLTARGETTYPE_MAP_		30
#define SKILLTARGETTYPE_HERO_		40
#define SKILLTARGETTYPE_TABLE_		50
#define SKILLTARGETTYPE_MAP_WITH_ITEM_  60
#define SKILLTARGETTYPE_2ND_GM	  100//021111 lsw
#define SKILLTARGETTYPE_SKILL_MASTER  110//021111 lsw

#define ITEMMAKE_YOUCAN					1

#define ITEMMAKE_UNKNOWN				-1
#define ITEMMAKE_NOT_RESOURCE			-2	
#define ITEMMAKE_NOT_TOOL				-3
#define ITEMMAKE_NOT_FLATFORM			-4
#define ITEMMAKE_TIRED					-5
#define ITEM_MAKE_NO_JOIN				-6
			
#define ITEMMAKE_GOGOGO					-7		// 생성확률 100%  작업을 위해 잠시 만든어둔 Definition...
#define ITEMMAKE_NO						-8

//  기술 실패 번호.... 
#define SKILLFAIL_NOT_PROPER_PLACE		 -9  // 기술을 위한 적당한 장소가 아닙니다. 
#define SKILLFAIL_UNKNOWN_ITEM			-10
#define SKILLFAIL_NOT_RESOURCE			-20	
#define SKILLFAIL_NOT_FLATFORM			-40
#define SKILLFAIL_TIRED					-41

#define SKILLFAIL_NOT_TOOL48			-48
#define SKILLFAIL_NOT_TOOL49			-49
#define SKILLFAIL_NOT_TOOL50			-50
#define SKILLFAIL_NOT_TOOL51			-51
#define SKILLFAIL_NOT_TOOL52			-52
#define SKILLFAIL_NOT_TOOL53			-53
#define SKILLFAIL_NOT_TOOL54			-54
#define SKILLFAIL_NOT_TOOL55			-55
#define SKILLFAIL_NOT_TOOL56			-56
#define SKILLFAIL_NOT_TOOL57			-57
#define SKILLFAIL_NOT_TOOL58			-58
#define SKILLFAIL_NOT_TOOL59			-59
#define SKILLFAIL_NOT_TOOL60			-60
#define SKILLFAIL_NOT_TOOL61			-61
#define SKILLFAIL_NOT_TOOL62			-62
#define SKILLFAIL_NOT_TOOL63			-63
#define SKILLFAIL_NOT_TOOL64			-64
#define SKILLFAIL_NOT_TOOL65			-65
#define SKILLFAIL_NOT_TOOL66			-66
#define SKILLFAIL_NOT_TOOL67			-67
#define SKILLFAIL_NOT_TOOL68			-68
#define SKILLFAIL_NOT_TOOL69			-69
#define SKILLFAIL_NOT_TOOL70			-70
#define SKILLFAIL_NOT_TOOL71			-71
#define SKILLFAIL_NOT_TOOL72			-72
#define SKILLFAIL_NOT_TOOL73			-73
#define SKILLFAIL_NOT_TOOL74			-74
#define SKILLFAIL_NOT_TOOL75			-75
#define SKILLFAIL_NOT_TOOL76			-76
#define SKILLFAIL_NOT_TOOL77			-77
#define SKILLFAIL_NOT_TOOL78			-78
#define SKILLFAIL_NOT_TOOL79			-79
#define SKILLFAIL_NOT_TOOL80			-80
#define SKILLFAIL_NOT_TOOL81			-81
#define SKILLFAIL_NOT_TOOL82			-82
#define SKILLFAIL_NOT_TOOL83			-83
#define SKILLFAIL_NOT_TOOL84			-84
#define SKILLFAIL_NOT_TOOL85			-85
#define SKILLFAIL_NOT_TOOL86			-86
#define SKILLFAIL_NOT_TOOL87			-87


#define SKILLFAIL_NO_SHEAR				-100
#define SKILLFAIL_NO_MEET				-101
#define SKILLFAIL_NO_MINING				-102	// 더이상 파낼것이 없어여..
#define SKILLFAIL_NO_DIGGING			-103	// 더이상 캐낼 약초가 없어여.
#define SKILLFAIL_NO_CUTTING			-104	// 더이상 잘라낼 나무가 없어여. 


#define SKILLFAIL_NO_SEED				-105
#define	SKILLFAIL_NOT_YET_HARVEST		-106
#define SKILLFAIL_NOT_YET_SOW			-107	// 땅이 척박하거나 한군대에 너무 많이 심었습니다. 
#define SKILLFAIL_NO_MORE_RECLAIM		-108	// 충분히 개간이 되었습니다. 이제 씨앗을 심으면 된다고 생각합니다.
#define SKILLFAIL_NO_FISHING			-109	// 더이상 물고기가 없습니다

#define SKILLFAIL_LOW_LEVEL_TO_TAMING	-110	//taming할 수 없는 능력치를 유저가 가지고 있다
#define SKILLFAIL_NO_TAMING_ALLOWED		-111	//taming할 수 없는 대상
#define SKILLFAIL_TARGET_IS_NOT_NPC		-112	//taming할 스 있는 NPC가 아니다
#define MAX_SKILL_		  150

const int MAX_IMR_FIELD_CT = 7;
typedef struct ITEMMULTIRARE
{
	int iGrade;
	int iAble;
	int aItemNo[MAX_IMR_FIELD_CT];//0 바이, 1 자이, 일스
	int aItemCt[MAX_IMR_FIELD_CT];//0 바이, 1 자이, 일스
	int	iAddCt;
}t_ItemMultiRare,*LPITEMMULTIRARE;

const int MAX_ITEM_MULTI_RARE= 17;//테이블의 카운트
extern ITEMMULTIRARE aIMR[MAX_ITEM_MULTI_RARE];

typedef struct t_skillmain
{
	int NO;
	char Skill_Minute[31];
	int Class_Warrior;
	int Class_Thief;
	int Class_Archery;
	int Class_Wizard;
	int Class_Priest;
	int Select_Farmer;
	int Select_miner;
	int Select_fisher;
	int Select_Lumberjack;
	int Select_Butcher;
	int Select_Herbdigger;
	int Select_herdman;
	int Select_Carpenter;
	int Select_Blacksmith;
	int Select_Cooking	;
	int Select_Tailoring	;
	int Select_Bowcraft_Fletcher;
	int Select_Alchemy	;
	int Select_Candlemaker	;
	int Select_Merchant	;
	int Howto	;
	int mouse_change	;
	int Interface1	;
	int Interface2	;
	int Success_interface	;
	int Fail_Interface	;
	int Work_target_self	;
	int Work_target_othercharacter	;
	int Work_target_object	;
	int Work_target_Item	;
	int Work_target_earth	;
	int Work_target_water	;
	int Work_target_forest	;
	int Work_target_interface	;
	int Tool	;
	int Object_Workshop	;
	int Object_target	;
	int Obj_Attribute	;
	int Resource1	;
	int Resource1_quantity	;
	int Resourece2	;
	int Resource2_quantity	;
	int Resource3	;
	int Resource3_quantity	;
	int Produce_type	;
	int Produce_Fix	;
	int Ability_Fix	;
	int Time_judge	;
	int Time_Maintain	;
	int Ability_min	;
	int Ability_max	;
	int Plus_element	;
	int Plus_type	;
	int Plus_Rate	;
	int Plus_min	;
	int SP_demand	;
	int Success	;
	int effectCharobj_status	;
	int Change_enermy_status	;
	int Change_enermy_status2	;
	int charvisible	;
	int Effectobj_status	;
	int Changeobj_attribute	;
	int changeobj_visible	;
	int Skill2job	;
	int effectChar_hide	;
	int Increase_mov	;
	int Increse_Life	;
	int Increse_mana	;
	int Increse_health	;
	int Change_mor	;
	int Obj_attribute1	;
	int ChangeObj_attribute1	;
	int Display_LV	;
	int DisPlay_Wis	;
	int Display_Int	;
	int DisPlay_Life	;
	int DisPlay_Health	;
	int DisPlay_Damage	;
	int DisPlay_AC	;
	int DisPlay_inventory	;
	int Display_Foot	;
	int DisPlay_Item_Changeall	;
	int DisPlay_royalty	;
	int DisPlay_Item_Status	;
	int DisPlay_name	;
	int DisPlay_item_Price	;
	int effect	;
	int effect_wave	;
	int effect_race	;
	int Success_range	;
	int Damage	;
	int Status_self	;
	int Effect1	;
	int Effect_wave1	;
	int Enemy_status	;
	int Range	;
	int Enermy_alram	;
	int map	;
	int Curser1	;
	int Curser2	;
	int Curser3	;
	int point	;
	int inclusive;
	int money;
	int need_exp;
	int MotherSkillType;
	int MotherSkillExp;

	int k1;
	int k2;
}SkillMain;

#define SKILLGAUGE_RESTTIME		738// 남은시간.
#define SKILLGAUGE_HAVEST		737// 수확율 
#define SKILLGAUGE_CULTIVATE	736// 인공적재배율
#define SKILLGAUGE_TAME			735// 조련도
#define SKILLGAUGE_RECLAIM		734// 토지개간도
#define SKILLGAUGE_SOWING		733// 파종율 
#define SKILLGAUGE_FISHING		739//1018 zhh //낚시를 위한 것

typedef struct tagSKILLGAUGE{	
	short skillno;
	short type;
	short x, y, off;
	short min, max;
	short delay;
	bool  toggle;

	struct tagSKILLGAUGE *prev, *next;
}SKILLGAUGE, *LPSKILLGAUGE;

extern void SendSkillInfo( int skillno, int x, int y, int targettype, int targetid ,const int iKey = 0, const char* TargetName = "");//030102 lsw
extern void SendSkillInfo( int skillno, int x, int y, int targettype, int targetid, POS s,const int iKey = 0, const char* TargetName = "");//030102 lsw
extern void LoadHongSkillTable( void);
extern void SkillSettingClear( void );
extern void RecvSkillInfo( t_server_skill_info *p );
extern void RecvSkillItemMakeResult( t_server_skill_itemmake_result *p );
extern void RecvSkillResultFail( t_server_skill_result_fail *p );
extern void RecvSkillResult( t_packet *p );
extern int SelectSkill( int skillno );
extern int CheckSkill( void );
extern void SendSkillMatter( MakeItem *matter );
extern void Send2NDGMMAKE(int skillno);
extern void SendCMD_FOOT_ON_TRAP_0_ITEM( int server_id );
extern void RecvCMD_FOOT_ON_TRAP_0_ITEM( t_server_foot_on_trap0 *p );
extern void SendCMD_FOOT_ON_TRAP_1_ITEM( int server_id );
extern void RecvCMD_FOOT_ON_TRAP_1_ITEM( t_server_foot_on_trap1 *p );
extern void RecvCMD_STEAL_ON_TRAP( t_server_steal_on_trap	*p );
extern void RecvViewCharInfo( t_view_char_info *p );
extern void RecvViewtypeChange( t_viewtype_change *p );
extern void GameStatusOutput( int no );
extern void PutSkillUseItem_skill_use_item( int skillno  );
extern void DrawSkilltile( void  );
extern void DrawSkillGauge( LPSKILLGAUGE n );
extern void DeleteSkillGauge( LPSKILLGAUGE tt );
extern LPSKILLGAUGE AddSkillGauge( int skillno, int x, int y, int min );
extern void OutputSkillGauge( void );
extern void AdjustSkillMouseCursor( int *x, int *y );
extern void SendSkillGauge( int x, int y, int skillno );
extern void RecvSkillGauge( t_server_skill_gauge *p );
extern void RecvSkillResultFromOther( server_skill_result_toother *tp );
extern void CheckTime_SkillMsg( void );
extern void  CircleMagic( void );
extern void RecvSkillAnimalLore( t_packet *p );

extern void RecvOutPutMessage(t_packet *p );//010118 zhh
extern void RecvSoundOut(t_packet *p );

extern int		SkillNo;
extern int		SkillStatus;
extern Spr	   *SkillIcon;
extern int		SkillItemNo;			// 기술에 사용될  Item의 번호.
extern POS		SkillItemPOS;			// 기술에 사용될  Item의 위치값.
extern char		SkillOutputMessage[ 31];
extern SkillMain SkillTable[ MAX_SKILL_];
extern LPSKILLGAUGE g_SkillGauge;

class CSkill
{
public :
	int		num;
	char	name[31];
	char	han_name[31];
	int		inclusive;			// 32개 짜리 어미 스킬
	int		series;				// 4개 짜리 구분
	char	explain[255];
	int		money;				// 배우는데 드는 비용
	int		skillable;			// 배우는데 필요한 능력치 
	char    targettype;

public :
	void ReadSkill( int num, char *string );
	void Explain(const int iX, const int iY);
	void ExplainSkillWhenMapOn(int x, int y);
	int	 CanLearnSkill( int );
	
};

extern CSkill	skill[MAX_SKILL];
const int MAX_GM_NEED_ITEM_COUNT = 3;
typedef struct tag_gm_quest
{//021030 lsw
	char	name[20];
	int		skill;
	int		job_level;
	int		need_money;
	
	int		need_item[MAX_GM_NEED_ITEM_COUNT];
	int		need_quantity[MAX_GM_NEED_ITEM_COUNT];

	int		need_questnum1;
	int		need_questnum2;
}GM_QUEST;


enum eMotherSkillIndex
{
	SPRITUALITY				=0,
	POISONING				=1,
	ENTRAPMENT				=2,
	SCROLLING				=3,
	CUNNING					=4,
	BOMBPLAY				=5,
	STEALING				=6,
	FARMING					=7,
	MINING					=8,
	FISHING					=9,
	CUTDOWN					=10,
	BUTCHERING				=11,
	HERBDIGGING				=12,
	HERDING					=13,
	CARPENTRY				=14,
	BLACKSMITHY				=15,
	COOKING					=16,
	TAILORING				=17,
	BOWCRAFTING				=18,
	ALCHEMING				=19,
	CANDLEMAKING			=20,
	REPARING				=21,
	REUTILIZING				=22,
	MUSICIANSHIP			=23,
	DETECT					=24,
	MERCHANT				=25,
	CAMPING					=26,
	TAIMING					=27,
	SKILL_MASTER			=28,
	ANIMAL_LORE				=29,
	ARMS_LORE				=30,
	EVALUATE_INTELIGENCE	=31,
	EVALUATE_WISDOM			=32,
	ANATOMY_JUDGEMENT		=33,
	ITEM_IDENTIFICATION		=34,
	TASTE_IDENTIFICATION	=35,
	APPRAISAL				=36, // 감정( 보석류 )
	HEALING					=37,
	JUMP					=38,
	RIDING					=39
};

enum eGmQuestIndex
{
	GM_QUEST_FARMER				=1,		//	농부 
	GM_QUEST_MINDER				=2,		//	광부         
	GM_QUEST_FISHER				=3,		//어부         
	GM_QUEST_CUTTER				=4,		//나무꾼       
	GM_QUEST_BUTCHER			=5,		//도살자       
	GM_QUEST_DIGGER				=6,		//심마니       
	GM_QUEST_CARPENTER			=7,		//목수         
	GM_QUEST_BLACKSMITH			=8,		//대장장이     
	GM_QUEST_COOKER				=9,		//요리사       
	GM_QUEST_TAILOR				=10,	//재단사       
	GM_QUEST_BOWCRAFT			=11,	//활제작사     
	GM_QUEST_ALCHEME			=12,	//연금술사     
	GM_QUEST_CANDLEMAKER		=13,	//초쟁이       
	GM_QUEST_MERCHANT			=14,	//상인         
	GM_QUEST_BLACKSMITH_MASTER	=15,	//대장장이 기성
	GM_QUEST_CARPENTER_MASTER	=16,	//목수 기성    
	GM_QUEST_BOWCRAFT_MASTER	=17,	//활제작사 기성
	GM_QUEST_TAILOR_MASTER		=18,	//재단사 기성
};

const int  MAX_GM_MAIN		=20;
#define MAX_JOB_OF_SKILL	20

extern GM_QUEST	g_GmMain[MAX_GM_MAIN];
extern int GM_temp_list[MAX_JOB_OF_SKILL];		// 그랜드 마스터에 등록할수 있는 스킬목록
extern int GM_temp_list_count;
extern MULTIRARESRC g_MultiRareSrc;

extern void SetGMskill();
extern void LoadGMQuest( void );
extern void DisplayGmRegistRequest(const int iGmQuestIndex, const int iMainX, const int iMainY);
extern bool IsMerchant();
extern int IsAllGM();//1차 2차 올 지엠?
extern int	LoadItemTableData();
extern void ItemRead();
#endif // !defined(AFX_CRACKMGR_H__916F254F_DB1F_4269_BC2B_36BC59E48BC9__INCLUDED_)
