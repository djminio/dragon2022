#if !defined(AFX_SKILL_LSW_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)
#define AFX_SKILL_LSW_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_UPGRADE_RARE 20//021113 lsw

extern bool CheckBy2ndGM_Insert_to_GMItemList(ItemAttr ia, const int iType);
extern bool CheckBy2ndGM_This_Item_Upgradeable( ItemAttr ia,const int iType);//내가 이 아이템을 업그레이드 시킬수 있는가위해서 이걸 만들수 있나를 본다
extern void Send2NDSKILLMAKE(int iTryRare, int iRareGroup);


extern bool LoadRareGroup();
extern bool LoadFunctionGroup();
extern bool LoadRareProbability();
extern void ReadRareGroup(char * buf,int i);
extern void ReadRareProbability(char * buf,int i);
extern bool LoadItemFunctionTable();
extern int GetSkillResorceAmount(int &r1count ,int &r2count ,int &r3count ,int &r4count);
extern bool DoButtonCheckOfMenu_SpecialWillDo_lsw(int i, int j);

typedef struct RARESET
{
	char rare[40]	;
	int rare_num	;
	int rare_rate	;
}t_rareset,*LPRARESET;

typedef struct rare_group
{
	int no;
	char name[40];

	t_rareset rareset[8];

	char group_buffer_1[40]	;
	char group_buffer_2[40]	;
	char group_buffer_3[40]	;
	int	 group_buffer_4		;
	int	 group_buffer_5		;
	int	 group_buffer_6		;
	int	 group_buffer_7		;
	int	 group_buffer_8		;
	int	 group_buffer_9		;
	int	 group_buffer_10	;
}t_raregroup,*LPRARE_GROUP;

typedef struct rare_probability
{
	int num				;
	int exp_rare_suc[4]	;
	int max_rare_day	;
	int bonus_suc		;
	int max_suc			;	
	int buffer_1		;
	int buffer_2		;
	int buffer_3		;
	int buffer_4		;
	int buffer_5		;
	int buffer_6		;
	int buffer_7		;
	int buffer_8		;
	int buffer_9		;
	int buffer_10		;
}t_rareprobability,*LPRAREPROBABILITY;

typedef struct SkillMakeItemInfo
{
	int item_no			;
	int join_no			;
	int TryRareType		;
	int RareGroupType	;
	int MakeMuch		;
}t_SkillMakeItemInfo,*LPSKILLMAKEITEMINFO; 

extern t_SkillMakeItemInfo	skillmakeiteminfo;

#define MAX_RARE_GROUP			18//020515 lsw
#define MAX_RARE_PROBABILITY	110

extern t_raregroup			RareGroup[5][MAX_RARE_GROUP];//0 은 레어그룹// 1은 펑션 그룹
extern t_rareprobability	RareProbability[MAX_RARE_PROBABILITY];

#define CMD_ADD_EFFECT	9115
typedef struct ADD_EFFECT
{
	WORD wEffectNo;	// 사용된 마법의 번호
	WORD idTarget;	// 마법을 사용한 대상자
	WORD wDuration;	// 유지시간
}t_AddEffect,*LPADDEFFECT;

#define MAX_ITEM_FUNCTION	100
typedef struct item_function
{
	int		iNo;
	char	Name[40];
	char	NameAdd[40];
	char	Exp[50];//추가 설명
	int		iExpMark;//020530 lsw
	int		iEffectNo;
	int		iUpgradeAble;
	int		iScrollNo;
	int		iMakeRand[3];
	int		GRADE[17];
	int		iTime[17];
	int		iBuffer1;
	int		iBuffer2;
}t_ItemFunction,*LPITEMFUNCTION;
extern t_ItemFunction ItemFunction[MAX_ITEM_FUNCTION];

extern bool	CanViewBomb(CHARACTER *ch);//Trap0
extern bool	CanViewTrap(CHARACTER *ch);
extern bool	CanViewMagicTrap(CHARACTER *ch);
extern bool	CanViewGhost(CHARACTER *ch);
extern void InitHeroCanView();//020510 lsw

enum eSkillNoIndex
{
	SKILL_DETECT_GHOST						=1,	
	SKILL_TALK_WITH_GHOST					=2,	
	SKILL_POISONING							=3,	
	SKILL_DETECT_POISON						=4,	
	SKILL_FIND_TRAPS						=5,	
	SKILL_REMOVE_TRAPS						=6,	
	SKILL_ENTRAPMENT						=7,	
//	SKILL_MAGIC_SCROLLING					=8,	
//	SKILL_ABILITY_SCROLLING					=9,	
//	SKILL_EMERGENCY_ALRAM					=10,	
//	SKILL_FIRST_AID							=11,	
//	SKILL_FAST_FOOT							=12,	
//	SKILL_SECRETHEAR						=13,	
//	SKILL_WALLCLIMBING						=14,	
//	SKILL_INTUITION							=15,	
	SKILL_DETECT_BOMB						=16,	
	SKILL_BOMB_IN_BOX						=17,	
	SKILL_REMOVE_BOMB						=18,	
	SKILL_STEALING							=19,	
//	SKILL_LOCKPICKING						=20,	
	SKILL_RECLAIM							=21,	
	SKILL_SOW								=22,	
//	SKILL_CULTIVATE							=23,	
	SKILL_HAVEST							=24,	
//	SKILL_DETECT_MINERAL					=25,	
	SKILL_MINING							=26,	
	SKILL_REFINE							=27,	
	SKILL_FISH_HOOKING						=28,	
//	SKILL_BAITMAKING						=29,	
	SKILL_NETTING							=30,	
//	SKILL_SAILING							=31,	
	SKILL_CUTTING							=32,	
	SKILL_LUMBERING							=33,	
	SKILL_BUTCHERING						=34,	
//	SKILL_DETECT_HERB						=35,	
	SKILL_DIGGING							=36,	
	SKILL_MAKEPOWDER						=37,	
//	SKILL_HERDING							=38,	
//	SKILL_MILKING							=39,	
//	SKILL_MIDWIFERY							=40,	
	SKILL_SHEAR								=41,	
	SKILL_CARPENTRY							=42,	
	SKILL_BLACKSMITHY						=43,	
	SKILL_COOKING							=44,	
	SKILL_CLOTHING							=45,	
//	SKILL_DYEING							=46,	
	SKILL_TAILORING							=47,	
	SKILL_BOWCRAFTING						=48,	
	SKILL_FLETCHING							=49,	
	SKILL_MAKEPOTION						=50,	
	SKILL_MAKEMETAL							=51,	
	SKILL_MAKEBOTTLE						=52,	
	SKILL_CANDLEMAKING						=53,	
//	SKILL_WAXING							=54,	
//	SKILL_REPAIRNG							=55,	
//	SKILL_REUTILIZING						=56,	
//	SKILL_PLAYING							=57,	
//	SKILL_PEACEMAKING						=58,	
//	SKILL_PARALYSIS							=59,	
//	SKILL_HALLUCINATION						=60,	
//	SKILL_PROVOCATION						=61,	
//	SKILL_ENTICEMENT						=62,	
//	SKILL_NOISE								=63,	
//	SKILL_TRACKING							=64,	
	SKILL_SNOOPING							=65,	
//	SKILL_SEARCH							=66,	
	SKILL_MERCHANT							=67,	
	SKILL_BLACKSMITHY_MASTER				=68,	//대장장이 기성	
	SKILL_CARPENTRY_MASTER					=69,	//목수 기성	
	SKILL_BOWCRAFTING_MASTER				=70,	//활제작사 기성	
	SKILL_TAILORING_MASTER					=71,	//재단사 기성	
//	SKILL_HIDE								=72,	
//	SKILL_CAMPING							=73,	
	SKILL_MAKEFIRE							=74,	
	SKILL_TAMING							=75,	
//	SKILL_COMMUNICATE_WITH_ANIMAL			=76,	
//	SKILL_COMMUNICATE_WITH_OTHER_TRIBE		=77,	
//	SKILL_ANIMAL_LORE						=78,	
//	SKILL_ARMS_LORE							=79,	
	SKILL_EVALUATE_INTELIGENCE				=80,	
	SKILL_EVALUATE_WISDOM					=81,	
	SKILL_ANATOMY_JUDGEMENT					=82,	
	SKILL_ITEM_IDENTIFICATION				=83,	
//	SKILL_TASTE_IDENTIFICATION				=84,	
//	SKILL_APPRAISAL							=85,	
//	SKILL_HEALING							=86,	
	SKILL_TACTICS							=87,
	TACTICS_Crapple							=88,
	TACTICS_swordmanship					=89,
	TACTICS_Archery							=90,
	TACTICS_Fencing							=91,	
	TACTICS_Mace_fighting					=92,
	TACTICS_Pierce							=93,
	TACTICS_Whirl							=94,
	TACTICS_Hurl							=95,
	TACTICS_Parrying						=96,
	TACTICS_Double_Swordmanship				=97,
	TACTICS_Double_MaceFighting				=98,	
	TACTICS_Magery							=99,
	TACTICS_Orison							=100,
//	SKILL_THREAD							=101,	
//	SKILL_COLORING							=102,	
//	SKILL_JUMP								=103,	
//	SKILL_RIDING							=104,	
	SKILL_GHOST_RESURRECTION				=105,	
	SKILL_GM_CARPENTER						=106,	
	SKILL_GM_BLACKSMITH						=107,	
	SKILL_GM_COOK							=108,	
	SKILL_GM_TAILOR							=109,	
	SKILL_GM_BOWCRAFT						=110,	
	SKILL_GM_ALCHEMIST						=111,	
	SKILL_GM_CANDLEMAKER					=112,	
};

const int TACTICS_START = SKILL_TACTICS;//이값 미만이 되면 에러에요
const int TACTICS_MIN = TACTICS_Crapple;//이값 미만이 되면 에러에요
const int TACTICS_MAX = TACTICS_Orison;//이값 초과하면 에러에요

////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMMUTANTKIND
{
	int		item_kind;
	char	exp_kind[100];
	int		upgrade_type;
	int		AddItem[4];
	int		iBuffer1;
	int		iBuffer2;
	int		iBuffer3;
	int		iBuffer4;
}t_ItemMutantKind,*LPITEM_MUTANT_KIND;
const int MAX_ITEM_MUTANT_KIND = 201;
extern t_ItemMutantKind ItemMutantKind[MAX_ITEM_MUTANT_KIND];
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMGRADE
{
	int iGrade;
	int iAble;
	int ResourceAmount[4];
	int iBuffer1;
	int iBuffer2;
	int iBuffer3;
	int iBuffer4;
}t_ItemGrade,*LPITEM_GRADE;
const int MAX_ITEM_GRADE = 18;//MAX_ITEM_GRADE를 avail으로 체크한 값으로 받아 올수 있도록 코딩 할것
extern t_ItemGrade ItemGrade[MAX_ITEM_GRADE];
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ITEMRARE
{
	int		iNo		;
	int		iType	;
	char	EngName[100];
	char	LocalName[100];
	char	Explain[100];
	char	ExplainAdd[100];
	int		iExpMark;// + - 기호 표시
	int		iMakeAble;
	int		RaisePro[8];// 0번은 DefaultRaisePro
	int		iUpgradeAble;
	int		iAbleNation;//
	int		GRADE[16];//0번은 MAX 나머지는 그레이드별 값
	int		Buffer1;
	int		Buffer2;
	int		Buffer3;
	int		Buffer4;
	int		Buffer5;
	int		Buffer6;
}t_ItemRare,*LPITEMRARE;
const int MAX_STATIC_RARE = 100;
const int MAX_ITEM_RARE = 200;
extern t_ItemRare ItemRare[MAX_ITEM_RARE];
////////////////////////////////////////////////////////////////////////////////////////////////////////

const int MAX_DOUBLE_RARE_GRADE = 10;//021212 lsw

typedef struct ITEMRAREMAIN
{
	DWORD	grade			: 4;//등급
	DWORD	soksung1		: 8;//속성1
	DWORD	soksung2		: 8;//속성2 멀티 속성
	DWORD	soksung3		: 8;//시간등급
	DWORD	iHighLevel		: 3;//1이면 하이 아이템
	DWORD	IsDynamicRare	: 1;//1 이면 기능 아이템 0 이면 레어 아이템
}RareMain, *LPRareMain;
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
const int MAX_COMBAT_SKILL_STRUCT = 40;//0 번은 두잉이 쓰고 나머지는 전투스킬 갯수 만큼 
typedef struct COMBATSKILL
{
	int iGroup;			//전투 스킬 그룹
	int iGroupType;		//전투 스킬 타입
	int iCombatSkillNo;	//전투 스킬 넘버
	int iSkillNo;		//SkillID 의 넘버
	int iSkillLv;		//전투 스킬 레벨//SCharacter.SkilID[iSkillNo]하면 나오는 값
}t_CombatSkill,*LPCombatSkill;
extern t_CombatSkill CombatSkill[MAX_COMBAT_SKILL_STRUCT];//전투 스킬 Doning 메뉴에서 쓰는것// 0,0 -> 사용할 스킬 번호 1,2,3->그룹
extern int iCSPMax;
extern int iCSPNow;

extern int iCSPCMax;
extern int iCSPCNow;
extern int iCSReservPoint;

const int iCSCovertSkillID = 115;

const int MAX_COMBAT_SKILL_EXPLAIN = 200;
typedef struct COMBATSKILLEXPLAIN
{
	int		iSkillNo;		//	스킬번호	//및 인덱스
	char	EngName[31];	//	영문이름	
	//char	LocalName[31];	//	한글이름	
	
	char	ClassName[51];	//	가능 클래스 설명( 한글로 쳐라 )
	char	WeaponTactic[51];	//	가능 클래스 설명( 한글로 쳐라 )
	
	int		iNeedLv[3];		//	요구레벨1	
	int		iLvCp[3];			//	레벨1_CP	

	char	MicroExplain[3][51];//레벨당 설명
	
	char	Use[71];			//사용법 설명

	char	Explain[101];		//설명
}t_CombatSkillExplain,*LPCombatSkillExplain;
extern t_CombatSkillExplain CombatSkillExplain[MAX_COMBAT_SKILL_EXPLAIN];//전투스킬 설명용

const int MAX_COMBAT_SKILL_GROUP_EXPLAIN = 10;
typedef struct COMBATSKILLGROUPEXPLAIN
{
	int		iNo;		//	스킬번호	//및 인덱스
	int		iGroupNo;		//	스킬번호	//및 인덱스
	int		iGroupType;		//	스킬번호	//및 인덱스
	char	EngName[30];	//	영문이름	
	char	LocalName[30];	//	한글이름	
	int		iSkillId[6];
	char	Explain[100];		//설명
}t_CombatSkillGroupExplain,*LPCombatSkillGroupExplain;
extern t_CombatSkillGroupExplain CombatSkillGroupExplain[MAX_COMBAT_SKILL_GROUP_EXPLAIN];//전투스킬 설명용

extern void CallCombatSkillDoingMenu();
extern void CallCombatSkillPointMenu();
extern void SetCSPButtonType(const int iSelectedGroupButton);
extern void SetCSPGroupButtonImage(const int iSelected);//그룹 버튼 이미지 셋팅
extern void SetCSPIconImageANDnWillDo(const int iGroupNo,const int iForceGroupType = 0);
extern void SendCSLvUp(const int iCombatSkillIndex);//iSkillNo(1~6)
extern void InitCombatSkill();
extern void SetSCDMainAndOtherClear(const int iCombatSkillIndex);
extern bool IsSelectCombat(); // CSD-TW-030606
extern void CSDMainIconClear(); // CSD-TW-030627
extern int SkillIdToCombatSkillIconImage(const int iSkilNo,const bool bIsMain);
extern void SetCSDIconRectAndnWillDo();
extern void DrawCombatSkillExplain(const int iExplainX, const int iExplainY, const int iCSIndex);
extern void DrawCombatSkillGroupExplain(const int iExplainX, const int iExplainY, const int iGroupNo, const int iGroupType);
extern void SetCSPButtonTypeWhenSelectGroupType();
extern bool LoadCombatSkillExplain();
extern bool LoadCombatSkillGroupExplain();
extern void DrawRareExplain(const int iExplainX, const int iExplainY, const int iRareIndex,const int iRareType);
extern bool IsEquipAbleCheckNation(const int iAbleNation, const CHARACTER *ch);//000  100 바이 10 자이 1 일스
extern bool IsEquipAbleCheckLv(const int iAbleLv, const CHARACTER *ch);
extern bool IsEquipAbleCheckFame(const int iAbleFame, const CHARACTER *ch);
extern bool	IsEquipAbleDualStep(const int iDualStep, const CHARACTER *ch);
extern bool	IsEquipAbleClass(const int iWarrior,const int iThief,
						 const int iArcher,const int iWizard,
						 const int iPriest,const CHARACTER *ch);
extern int Call_MERCHANT_QUEST_MENU(const int iGmQuestIndex);//021030 lsw

enum eRareType
{
	RARE_POWER				=1,	//	short	nPower				//아이템의 물리적 Damage 증가	
	RARE_ACCURATE			=2,	//	char	nAccurate			//물리적 공격 성공률 증가 (타격시 miss 확률 감소)	
	RARE_WISE				=3,	//	short	nWise				//마법 캐스팅 시간 00만큼 단축	
	RARE_BEAR				=4,	//	short	nBear				//캐스팅 유지 시간 00만큼 증가	
	RARE_DISTANT			=5,	//	char	cDistant			//공격 범위가 멀었을 때의 데미지 증가 (현재 100%/90%/85%로 감소)	
	RARE_VIGOR				=6,	//	short	nVigor				//아이템의 물리적 Damage % 증가	
	RARE_CURSE				=7,	//	short	nCurse				//자신이 시전한 저주 계열 마법 효과 지속 시간 00 증가	
	RARE_AID				=8,	//	short	nAid				//자신이 시전한 보조 계열 마법 효과 지속 시간 00 증가	
	RARE_MASKING			=9,	//	short	nMasking			//자신이 시전한 보호 계열 마법 효과 지속 시간 00 증가	
	RARE_PIERCING			=10,	//	char	cPiercing			//00% 확률로 상대의 보호 계열 마법을 무시하여 물리적 데미지를 입힘	
	RARE_BASIC_RESIST		=11,	//	char	cBasicResist		//00% 확률로 상대의 마법 저항력을 무시(기본저항력)하여 마법 데미지를 입힘	
	RARE_HARDEST			=12,	//	short	nHardest			//ac 증가	
	RARE_SMART				=13,	//	char	cSmart				//물리적 공격 회피율 증가 (상대가 물리적 타격시 miss 확률 증가)	
	RARE_RESIST_FREEZE		=14,	//	short	nResist_freeze		//얼음저항력 증가	
	RARE_RESIST_VENOM		=15,	//	short	nResist_venom		//독저항력 증가	
	RARE_RESIST_LIGHTNING	=16,	//	short	nResist_lightning	//전격저항력 증가	
	RARE_RESIST_BLAZE		=17,	//	short	nResist_blaze		//불저항력 증가	
	RARE_RESIST_PARALYSIS	=18,	//	short	nResist_paralysis	//마비저항력 증가	
	RARE_RESIST_HOLY		=19,	//	short	nResist_holy		//신법저항력 증가	
	RARE_RESIST_ALL			=20,	//	short	nResist_all			//모든 저항력 증가	//위에꺼 따다 씀
	RARE_SPEED				=21,	//	short	nSpeed				//이동 속도 증가	
	RARE_LIFE				=22,	//	short	nLife				//Life max 증가	
	RARE_MANA				=23,	//	short	nMana				//Mana max 증가	
	RARE_DIVINE				=24,	//	short	nDivine				//Divine max 증가	
	RARE_HEALTH				=25,	//	short	nHealth				//Health max 증가	
	RARE_BROOD				=26,	//	short	nBrood				//Life max % 증가	
	RARE_MIND				=27,	//	short	nMind				//Mana max % 증가	
	RARE_SPIRIT				=28,	//	short	nSpirit				//Divine max % 증가	
	RARE_YOUTH				=29,	//	short	nYouth				//Health max % 증가	
	RARE_FATAL				=30,	//	short	nFatal				//Life 1회 자동 회복량 증가	
	RARE_MAGIC				=31,	//	short	nMagic				//Mana 1회 자동 회복량 증가	
	RARE_NATURE				=32,	//	short	nNature				//Divine 1회 자동 회복량 증가	
	RARE_STRENGTH			=33,	//	short	nStrength			//str 증가	
	RARE_DEXTERITY			=34,	//	short	nDexterity			//dex 증가	
	RARE_CONSTITUTION		=35,	//	short	nConstitution		//con 증가	
	RARE_WISDOM				=36,	//	short	nWisdom				//wis 증가	
	RARE_INTELLIGENCE		=37,	//	short	nIntelligence		//int 증가	
	RARE_CHARISMA			=38,	//	short	nCharisma			//cha 증가	
	RARE_ENDURANCE			=39,	//	short	nEndurance			//endu 증가	
	RARE_MOVEMENT			=40,	//	short	nMovement			//movp 증가	
	RARE_MORAL				=41,	//	short	nMoral				//mor 증가	
	RARE_LUCKY				=42,	//	short	nLucky				//luck 증가	
	RARE_WIZARD				=43,	//	short	nWizard				//WS 증가	
	RARE_PRIEST				=44,	//	short	nPriest				//PS 증가	
	RARE_MAGIC_POWER		=45,	//	short	nMagic_power		//위자드 계열 마법 데미지 증폭	
	RARE_MAGIC_VIGOR		=46,	//	char	cMagic_vigor		//위자드 계열 마법 데미지 00% 증폭	
	RARE_MAGIC_STAY			=47,	//	short	nMagic_stay			//위자드 계열 마법 지속 시간 증폭	
	RARE_NATURE_POWER		=48,	//	short	nNature_power		//프리스트 계열 마법 데미지 증폭	
	RARE_NATURE_VIGOR		=49,	//	char	cNature_vigor		//프리스트 계열 마법 데미지 00% 증폭	
	RARE_NATURE_STAY		=50,	//	short	nNature_stay		//프리스트 계열 마법 지속시간 증폭	
	RARE_LIFE_RECOVERY		=51,	//	char	cLife_recovery		//Life 자동 회복량 최대치 증가 (전체의 00%까지 자동 회복)
	RARE_MANA_RECOVERY		=52,	//	char	cMana_recovery		//Mana 자동 회복량 최대치 증가 (전체의 00%까지 자동 회복)
	RARE_CRITICAL			=53,	//	char 	cCritical			//크리티컬 데미지 나올 확률 00% 증가	
	RARE_GUARDIAN			=54,	//	short	nGuardian			//캐릭터의 물리적 방어력 증가	
	RARE_EVASION			=55,	//	char	nEvasion			//공격 마법 회피율 증가 (상대의 공격 마법 무효화 확률)	
	RARE_56					=56,	//		56					//	
	RARE_57					=57,	//		57					//	
	RARE_58					=58,	//		58					//	
	RARE_59					=59,	//		59					//	
	RARE_60					=60,	//		60					//	
	RARE_61					=61,	//		61					//	
	RARE_62					=62,	//		62					//	
	RARE_63					=63,	//		63					//	
	RARE_64					=64,	//		64					//	
	RARE_65					=65,	//		65					//	
	RARE_66					=66,	//		66					//	
	RARE_67					=67,	//		67					//	
	RARE_WITCH				=68,	//	short	nWitch				//타격 시 상대의 Mana를 00만큼 뺏어옴	
	RARE_DEVIL				=69,	//	char	cDevil				//타격 시 상대의 Mana를 00% 만큼 뺏어옴	
	RARE_DESIRE				=70,	//	short	nDesire				//타격 시 상대의 Life를 부가적으로 00만큼 뺏어옴	
	RARE_ROBBER				=71,	//	char	cRobber				//타격 시 상대의 Life를 부가적으로 00% 만큼 뺏어옴	
	RARE_EVIL				=72,	//	short	nEvil				//타격 시 상대의 Divine을 부가적으로 00만큼 뺏어옴	
	RARE_SATAN				=73,	//	char	cSatan				//타격 시 상대의 Divine을 부가적으로 00% 만큼 뺏어옴	
	RARE_WEARY				=74,	//	short	nWeary				//타격 시 상대의 Health를 부가적으로 00만큼 뺏어옴	
	RARE_FATIGUE			=75,	//	char	cFatigue			//타격 시 상대의 Health를 부가적으로 00% 만큼 뺏어옴	
	RARE_COUNTERATTACK		=76,	//	short	nCounterattack		//공격자에게 00의 데미지를 돌려줌	
	RARE_STRIKE				=77,	//	char	cStrike				//공격자의 데미지에서 00%를 돌려줌	
	RARE_78					=78,	//		83					//	
	RARE_SNAG				=79,	//	short	nSnag				//타격 시 상대를 00(시간)동안 마비시킴	
	RARE_VERTIGLY			=80,	//	short	nVertigly			//타격 시 상대를 00(시간)동안 혼란에 걸리게 함	
	RARE_POISON				=81,	//	short	nPoison				//타격 시 상대를 00(시간)동안 중독에 걸리게 함	
	RARE_STIFF				=82,	//	short	nStiff				//타격 시 상대를 00(시간)동안 석화에 걸리게 함	
	RARE_SLACK				=83,	//	short	nSlack				//타격 시 상대를 00(시간)동안 속도저하에 걸리게 함	
	RARE_84					=84,	//		84					//	
	RARE_GHOST				=85,	//	char	bGhost				//착용하게 되면 유령을 볼 수 있음	
	RARE_LANDMINE			=86,	//	char	bLandmine			//일반 트랩을 감지할 수 있음	
	RARE_TRAPING			=87,	//	char	bTraping			//마법 트랩을 감지할 수 있음	
	RARE_DETECT				=88,	//	char	bDetect				//모든 트랩을 감지할 수 있음	//위에꺼 따다 씀 
	RARE_PUNY = 88,	//	char	bDetect				//모든 트랩을 감지할 수 있음	//위에꺼 따다 씀 
};

enum eHighLv
{
	H_LV_NO_HIGH_ITEM = 0,
	H_LV_HIGH_ITEM = 1,
	H_LV_LEGEND_ITEM = 2,
};

const int MAX_ITEM_LEGEND_SUB_BONUS_MAX = 4;
typedef struct ITEM_LEGEND_SUB
{
	int iGroupIndex;
	int iGroupMax;
	char* szName;
	int iNeeds;
	int iWARHand2;
	int iWALHand3;
	int iWAHead4;
	int iWABody5;
	int iWANeck6;
	int iWAShoe7;
	int iWAUnique8;
	int iWATwoHand10;
	int aiBonus[MAX_ITEM_LEGEND_SUB_BONUS_MAX];
}t_ItemLegendSub,*LPITEMLEGENDSUB;

const int MAX_ITEM_LEGEND_SUB = 10;
const int MAX_ITEM_LEGEND	= 30;

typedef struct ITEM_LEGEND
{
	int iNo;
	int iGroupNo;
	int iGroupIndexMax;
	t_ItemLegendSub aSub[MAX_ITEM_LEGEND_SUB];
}t_ItemLegend,*LPITEMLEGEND;


const int MAX_ITEM_LEGEND_BONUS	= 1000;
const int MAX_ITEM_LEGEND_BONUS_KIND	= 6;
typedef struct ITEM_LEGEND_BONUS
{
	int iNo;
	char szName[20];
	int aiKind[MAX_ITEM_LEGEND_BONUS_KIND];
	int aiGrade[MAX_ITEM_LEGEND_BONUS_KIND];
}t_ItemLegendBonus,*LPITEMLEGENDBONUS;

extern ITEM_LEGEND			g_aItemLegend[MAX_ITEM_LEGEND];
extern ITEM_LEGEND_BONUS	g_aLegendBonus[MAX_ITEM_LEGEND_BONUS];

extern void SetGMQUESTMenu(const int iGmQuestIndex);
extern int CheckMerchantOrSkillMaster();
extern int skill_index[5];
extern void SendCMD_SKILL_MASTER_MULTI_RARE_MAKE(const int iSkillNo);

extern void CallMN_START_MAP_Menu();
#endif // !defined(AFX_SKILL_LSW_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)

