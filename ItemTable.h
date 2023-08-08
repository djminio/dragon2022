#if !defined(AFX_ITEM_TABLE_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
#define AFX_ITEM_TABLE_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_
//021014 lsw
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern DWORD g_CurrentTime;
extern int GetAntiMagic_1( int *anti, int *ability );
extern DWORD ReturnBlendRGB( int r, int g, int b );

enum ITEM_MAIN_GROUP_TYPE
{
	PLANT			=0,
	MINERAL			=1,
	HERB			=2,
	COOK			=3,
	POTION			=4,
	TOOL			=5,
	WEAPON			=6,
	DISPOSABLE		=7,
	ARMOR			=8,
	ACCESSORY		=9,
	ETC				=10,
	OBJECTITEM		=13,
};
//######## 아이템 번호 정의 ########
#define IN_COIN			10029
#define IN_COINS		10030

#define IN_NEW_COIN		10122
#define IN_NEW_COINS	10123

#define IN_BLUE_SIGNET	10059
#define IN_RED_SIGNET	10058

//#define MAX_EXPLAN		300		// 최대 설명 글자
#define ITEM_MAX			1000	// 각 계열당 아이템 수
#define MAX_STRING_			4000	// 한번에 읽어 들일 스트링의 최대값
#define	ITEM_NAME_MAX		31		// 아이템 구조체 멤버 변수중 스트링의 최대치
									
#define ITEM_Plant_MAX		43		
#define ITEM_Mineral_MAX	17		
#define ITEM_Herb_MAX		63		
#define ITEM_Cook_MAX		57		
#define ITEM_Potion_MAX		64		
#define ITEM_Tool_MAX		49		
#define ITEM_Weapon_MAX		118		
#define ITEM_Disposable_MAX	53		
#define ITEM_Armor_MAX		122		
#define ITEM_Accessory_MAX	177		
#define ITEM_Etc_MAX		184		

#define Num_Of_Item_Type		(ETC+1)

extern int Num_Of_CItem_Plant;
extern int Num_Of_CItem_Mineral	;
extern int Num_Of_CItem_Herb		;
extern int Num_Of_CItem_Cook		;
extern int Num_Of_CItem_Potion		;
extern int Num_Of_CItem_Tool		;
extern int Num_Of_CItem_Weapon		;
extern int Num_Of_CItem_Disposable	;
extern int Num_Of_CItem_Armor		;
extern int Num_Of_CItem_Accessory	;
extern int Num_Of_CItem_Etc			;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 무기,헬멧, 아머, 방패의 판단 WT(Weapon Type)
enum WearTypeGroup
{
	WEAR_ANYTHING				=0,
	WEAR_ONLY_INVEN				=1,
	WEAR_RIGHT_HAND				=2,
	WEAR_LEFT_HAND				=3,
	WEAR_HEAD					=4,
	WEAR_BODY					=5,
	WEAR_NECK					=6,
	WEAR_SHOES					=7,
	WEAR_UNIQUE					=8,
	WEAR_BELT					=9,
	WEAR_TWO_HAND				=10
};

enum eLegendNeed
{
	LEGEND_NEED_WEAPON	= 0x00000001,
	LEGEND_NEED_SHIELD	= 0x00000002,
	LEGEND_NEED_HELMET	= 0x00000004,
	LEGEND_NEED_ARMOR	= 0x00000008,
	LEGEND_NEED_NECK	= 0x00000010,
	LEGEND_NEED_SHOES	= 0x00000020,
	LEGEND_NEED_UNIQUE	= 0x00000040
};

enum eWTTypes
{
	WT_WEAPON	= 0,
	WT_SHIELD	= 1,
	WT_HELMET	= 2,
	WT_ARMOR	= 3,
	WT_NECK		= 4,
	WT_SHOES	= 5,
	WT_UNIQUE1	= 6,
	WT_UNIQUE2	= 7, 
	WT_POTION	= 8,
};
//////////////////////////////////////////////////////////////////////////////////////////////
// 아이템 kind
enum eItemKindType
{
	IK_NONE							=0,	//	non
	IK_GON							=1,	//	곤			
	IK_PENCING						=2,	//	펜싱검		
	IK_ONE_SWORD					=3,	//	한손검
	IK_ONE_AXE						=4,	//	한손도끼
	IK_ONE_MACE						=5,	//	한손철퇴
	IK_CLUB							=6,	//	몽둥이
	IK_WHIRL						=7,	//	봉
	IK_STAFE						=8,	//	지팡이
	IK_SPEAR						=9,	//	보통창
	IK_FORK							=10,	//	포크
	IK_HURL							=11,	//	비검,던지는무기
	IK_SHORT_BOW					=12,	//	소형활
	IK_TWO_SWORD					=13,	//	양손검
	IK_TWO_AXE						=14,	//	양손도끼
	IK_TWO_MACE						=15,	//	양손 철퇴
	IK_TWO_ARROW					=16,	//	화살
	IK_IRON_ARMOR					=17,	//	쇠갑옷
	IK_LEATHER_ARMOR				=18,	//	가죽갑옷
	IK_RING							=19,	//	반지
	IK_SHIELD						=20,	//	방패
	IK_SHOES						=21,	//	신발
	IK_ROBE							=22,	//	로브
	IK_FULL_HELMET					=23,	//	플헬멧
	IK_HALF_HELMET					=24,	//	하프헬멧
	IK_NORMAL_CLOTHE				=25,	//	보통옷
	IK_AMULET						=26,	//	목거리
	IK_ARMLET						=27,	//	팔찌
	IK_MUSIC						=28,	//	악기
	IK_SCROLL						=29,	//	스크롤
	IK_LONG_BOW						=30,	//	대형활
	IK_BOX							=31,	//	상자
	IK_GRAIN						=32,	//	곡식
	IK_POTION						=33,	//	포션
	IK_FOOD_MASS					=34,	//	덩어리 음식
	IK_PLATE_FOOD					=35,	//	쟁반 음식
	IK_DRINK						=36,	//	음료
	IK_ARROW_POINT					=37,	//	화살 촉
	IK_FEATHER						=38,	//	깃털		
	IK_SEED							=39,	//  씨앗.
	IK_MONEY						=40,	//	돈
	IK_MINERAL						=41,	//	광석
	IK_MINERAL_LUMP					=42,	//	가공한 광석( 철괴, 금괴... )
	IK_HERB							=43,	//	약초					// 0626 YGI
	IK_HERB_POWDER					=44,	//	약초 가루
	IK_WOOD_BEFOR_USE				=45,		//가공전의 나무
	IK_WOOD_AFTER_USE				=46,		//가공후의 나무
	IK_TENT							=47,		//텐트류
	IK_TOOL_RECLAI					=48,		//도구 개간용
	IK_TOOL_HAVEST					=49,		//도구 추수용
	IK_TOOL_MINING					=50,		//도구 채광용
	IK_TOOL_REFINE					=51,		//도구 제련용
	IK_TOOL_FISH_HOOKING			=52,		//도구 낚시용
	IK_TOOL_CUTTING					=53,		//도구 벌목용
	IK_TOOL_LUMBERING				=54,		//도구 목재가공용
	IK_TOOL_BUTCHERING				=55,		//도구 도축용
	IK_TOOL_DIGGING					=56,		//도구 약초채집용
	IK_TOOL_MAKEPOWDER				=57,		//도구 약재만들기용
	IK_TOOL_SHEAR					=58,		//도구 가위류
	IK_TOOL_CARPENTRY				=59,		//도구 목공용
	IK_TOOL_BLACKSMITHY				=60,		//도구 철공용
	IK_TOOL_COOKING_FIRE			=61,		//도구 대우는요리용
	IK_TOOL_COOKING_CUT				=62,		//도구 써는요리용
	IK_TOOL_PIN						=63,		//도구 바늘류
	IK_TOOL_BOWCRAFTING				=64,		//도구 활제작용
	IK_TOOL_FIRE_BOAD				=65,		//도구 불대
	IK_TOOL_CANDLEMAKING			=66,		//도구 양초제작용
	IK_TOOL_OILING					=67,		//도구 밀납가공용
	IK_TOOL_TAMING					=68,		//도구 테이밍용
	IK_TOOL_NETTING					=69,		//도구 그물류
	IK_TOOL_MAKEFIRE				=70,		//도구 부싯돌류
	IK_WORK_BOAD_BLAST_FURNACE 		=71,		//작업대 용광로
	IK_WORK_BOAD_BAETLE				=72,		//작업대 배틀
	IK_WORK_BOAD_BLOCK				=73,		//작업대 선반
	IK_WORK_BOAD_ANVIL				=74,		//작업대 모루
	IK_WORK_BOAD_BRAZIER			=75,		//작업대 화덕
	IK_WORK_BOAD_TEST_TUBE			=76,		//작업대 시험관류
	IK_WORK_BOAD_SPINNING_WHEEL		=77,		//작업대 물레
	IK_WORK_BOAD_FIRE_BOAD			=78,		//작업대 불대
	IK_TOOL_BUCKET					=79,		//도구 양동이
	IK_TOOL_RECYCLE					=80,		//도구 재활용
	IK_TOOL_REPAIR					=81,		//도구 수리용
	IK_IRON_SHIELD					=82,		//쇠방패	
	IK_IRON_SHOES					=83,		//쇠 신발	
	IK_CAP							=84,		//모자	
	IK_GOKAL						=85,		//두건	
	IK_PRIEST_ROBE					=86,		//성직로브	
	IK_CANDLE						=87,		//양초	
	IK_MAN_TO_MAN					=88,		//결투서	
	IK_CRACKER						=89,		// 폭죽			// 010210 YGI
	IK_RAJA_EMBLEM					=90,		// 라자 엠블렘
	IK_POTION_CHANGE_COLOR_CLOTHES	=91,	// 옷 염색약
	IK_POTION_CHANGE_COLOR_HAIR		=92,	// 피부 염색약
	IK_NEW_MONEY					=93,	// 신권돈
	IK_LOCK_STEAL					=94,	// 도둑방지 덫
	IK_MAP							=95,	//	지도
	IK_GAUNTLET						=96,		//건틀렛
	IK_GOOD_FOOD					=97,		//고급음식
	IK_GOOD_POTION					=98,		//고급포션
	IK_GOOD_CANDLE					=99,		//고급양초
	IK_UNIQ_RESOURCE				=100,		//유니크재료
	IK_GENERAL_GAMBLE				=101,		//일반겜블아이템
	IK_GRADE_GAMBLE					=102,		//그레이드겜블아이템
	IK_CLOAK						=103,		//망토
	IK_CHAIR						=104,		//의자			// LTS SITDOWN
	IK_BOMB							=105,		//011212 lsw 
	IK_TRAP							=106,		//011212 lsw 
	IK_SNOW_BALL					=107,		// 눈덩이
	IK_POTION_BOX					=108,		// 포션 주머니		020418 YGI
	IK_FALG							=109,		// 깃발
	IK_SADDLE						=110,		// 말안장
	IK_DUAL_GAUNTLET				=111,	
	IK_DUAL_SWORD					=112,	
	IK_DUAL_BOW						=113,	
	IK_DUAL_FENCING					=114,	
	IK_DUAL_AXE						=115,	
	IK_DUAL_SPEAR					=116,	
	IK_DUAL_KNIFE					=117,	
	IK_DUAL_DOUBLE_SWORD			=118,	
	IK_DUAL_DOUBLE_AXE				=119,	
	IK_DUAL_MAGICIAN_STAFF			=120,	
	IK_DUAL_PRIEST_STAFF			=121,	
	IK_DUAL_SHIELD					=122,	
	IK_DUAL_ARMOR					=123,	
	IK_DUAL_ARCHER_LEATHER			=124,	
	IK_DUAL_MAGICIAN_ROBE			=125,	
	IK_DUAL_HELM					=126,	
	IK_DUAL_ARCHER_CAP				=127,	
	IK_DUAL_MAGICIAN_HAT			=128,	
	IK_DUAL_BOOTS					=129,	
	IK_DUAL_ARCHER_SHOES			=130,	
	IK_DUAL_MAGICIAN_SHOES			=131,	
	IK_DUAL_THIEF_SHOES				=132,	
	IK_DUAL_PRIEST_SHOES			=133,	
	IK_DUAL_THIEF_CAP				=134,	
	IK_DUAL_PRIEST_HAT				=135,	
	IK_DUAL_THIEF_LEATHER			=136,	
	IK_DUAL_PRIEST_ROBE				=137,	
	IK_DUAL_THIEF_SHIELD			=138,	
	IK_DUAL_ARROW					=139,	
	IK_ABYSS_MAZE					=140,	//아비스 미궁용 아이템	
	IK_UNDEAD_DUNGEON				=141,	//언데드 던전용 아이템	
	IK_GREAT_MAZE					=142,	//대미궁 용 아이템	
	IK_TREASURE_MAP					=145,	// 보물지도	// 040105 YGI
	IK_TREASURE_BOX					=146,	// BBD 040213 보물지도 // 필요할것 같아 일단 추가해둠
	IK_POTION_BAG					=147,	// BBD 040213 포션백
	IK_NOEXCHANGE					=148,   // Finito 040707 prevent from exchanging item (Auction/Sell/Trade/Drop/Etc)
	IK_SEX_CHANGE					=151, 
};
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// GetItemAttr()에서 사용할 정의		-> 아이템의 보편적 멤버 변수의 값을 가져 올때
#define	WEAR_ABLE		500
#define	WEIGHT			510
#define VALUE			520
#define DURATION		530
#define ITEMHAND		540
#define ITEMSHOW		550
#define EQUIPABLE		560
#define ITEM_NAME		570
#define DO_RBUTTON		580
#define USE_JUNK		590
#define RBUTTON_WAVE	600
#define ITEM_KIND		610
#define ITEM_LIMIT		620
#define IMAGE_NUMBER	630
#define REPAIR_ABLE		640
#define VALUE_SELL		650
#define VALUE_REPAIR	660
#define OBJECT_IMAGE	670
#define ITEM_COLOR		680		//011018 KHS 망또
#define RAJA_EMBLEM		690		// The price (raja emblems) of an item in the item mall


////////////////////////////////////////////////////////////////////////////////////////////////////
//	CallItemVirtualFunction( int function ) 함수에서 사용할 define -> 단순히 함수를 호출하기만 할때..
////////////////////////////////////////////////////////////////////////////////////////////////////
#define VF_EQUIP_ITEM	100	
#define VF_RELEASE_ITEM 101	
#define VF_EAT_ITEM		102		
#define VF_CURR_EQUIP	103





////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagSkill
{
	int		m_Id;
	int		m_MotherId;						// 포괄적 기술
	char	m_Name[ITEM_NAME_MAX];
	int		m_Image;
	int		m_ProductionHow;				// 생산방식
	int		m_StandardOutK;					// 기준량 K-> 생산량을 계산하기 위함
	int		m_AbilityUp;					// 능력치 증가량
	int		m_Limit;						// 제약조건
	int		m_TimeCuntinu;					// 지속 시간
	int		m_PlusHow;						// 보정방법
	int		m_PlusParameter;				// 보정 인자
	int		m_PlusConstant;					// 보정 상수
	int		m_StandardOutY;					// 기준량 Y
	int		m_TermStudy;					// 학습 조건


public :


}Skill, *LpSkill;


class CItem
{
// protected :	
	// 31
public: 
	int curr_value;
	int curr_value_sell;
	int curr_value_repair;

public:
	virtual int ReadItemTable( char *temp ) { return 1 ; }
	virtual int GetItemId(){return 1;}
	virtual int GetImageNumber() { return 0; }
	virtual int GetObjectNumber() { return 0; }
	virtual int GetItemClass(){return 1;}
	virtual int GetItemWeight(){ return 0; }
	virtual int GetItemValue(){ return 0; }
	virtual int GetItemDuration() { return 1; }
	virtual int GetItemShow( int is_man )	{ return 0; }
	virtual int GetItemColor()	{ return 0xffff; }	// 011018 KHS 망또
	virtual int GetItemColorR( ) { return 0xff; }
	virtual int GetItemColorG( ) { return 0xff; }
	virtual int GetItemColorB( ) { return 0xff; }
	virtual int is_equipable() { return 0; }
	virtual void GetItemNeedAbility( int &count, int *type, int *value ) { count = 0; return; }
	virtual int is_equipable2(int dest_pos ) { return 0; }		// 게임 서버와 똑같이 간다.
	virtual char *GetItemName() { return 0; }
	virtual char *GetItemHanName() { return 0; }
	virtual int GetRButton() { return 0; }
	virtual int GetUseJunk() { return 0; }
	virtual int EatItem() { return 0; }
	virtual int EquipItem(){ return 0; }
	virtual int CurrEquip(){ return 0; }		// 현재 내가 착용하고 있는 아이템에 의한 능력치 변한
	virtual int ReleaseItem(){ return 0; }
	virtual int GetDamage() { return 0; }
	virtual int GetRButtonSound() { return 0; }
	virtual int GetItemKind() { return 0; }
	virtual int GetLimit() { return 0; }
	virtual int GetRepairAble() { return 0; }
	virtual int GetSkillAbility() { return -1; }
	virtual int GetGender() { return 0; }
	virtual int GetQuantity() { return 0; }

	virtual void SetItemValue(int p) { return; }
	virtual int GetMouse_MRD() { return 0; }
	void CopyItemClass() { curr_value = 0; curr_value_sell = 0; curr_value_repair = 0; }
	virtual int GetAR_or_DH() { return 0; }
	virtual int GetHit_rate() { return 0; }
	virtual int GetDefense_rate() { return 0; }
	virtual int GetMovePoint() { return 0; }
	virtual int GetAntiMagic( int *anti ) { return 0; }
	virtual int GetMouse_MR_Effect() { return 0; }
	virtual int GetNeedNation() { return 0; }
	virtual int GetNeedLv() { return 0; }
	virtual int GetNeedFame() { return 0; }

	virtual void GetItemChangeAbility( int &count, int *type, int *value ) { count = 0; return; }

	virtual int GetNeedDualStep(){ return 0; }//021113 lsw

	virtual int GetClassWarriorAble(){ return 1;}//클라이언트 디스플레이를 위해서 1. 서버는 0 입니다.
	virtual int GetClassArcherAble(){ return 1;}
	virtual int GetClassThiefAble(){ return 1;}
	virtual int GetClassWizardAble(){ return 1;}
	virtual int GetClassClericAble(){ return 1;}
};

class CItem_Plant :	public CItem
{
//protected:			
public:
	int Item_id   ;
	char Name[ITEM_NAME_MAX] ;
	char Han_Name[ITEM_NAME_MAX] ;
	int lv   ;
	int Image_Number   ;
	int hp   ;
	int ap   ;
	int mp   ;
	int nutrition1   ;
	int nutrition2   ;
	int nutrition3   ;
	int anti_poison   ;
	int anti_paralysis   ;
	int anti_magic   ;
	int anti_fire   ;
	int anti_ice   ;
	int anti_electric   ;
	int Time_maintain   ;
	int cure_disease1   ;
	int cure_disease2   ;
	int cure_disease3   ;
	int cure_disease4   ;
	int cure_disease5   ;
	int take_disease1   ;
	int take_disease2   ;
	int take_disease3   ;
	int take_disease4   ;
	int take_disease5   ;
	int weight   ;
	int circulation_limit   ;
	int max_duration   ;
	int Maximum   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int	Item_kind;



public:
	virtual int GetImageNumber() { return Image_Number; }
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual int GetItemDuration() { if( max_duration ) return max_duration; else return 1; }
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int EatItem();
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLimit() { return circulation_limit; }
	void CopyItemClass( CItem_Plant *source, int ct );
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetAntiMagic( int *anti ) { return GetAntiMagic_1( anti, &anti_poison ); }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }

};

class CItem_Mineral : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX] ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int weight   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind;



public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	void CopyItemClass( CItem_Mineral *source, int ct );	
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }

};


class CItem_Herb : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int hp   ;
	int ap   ;
	int mp   ;
	int nutrition1   ;
	int nutrition2   ;
	int nutrition3   ;
	int anti_poison   ;
	int anti_paralysis   ;
	int anti_magic   ;
	int anti_fire   ;
	int anti_ice   ;
	int anti_electric   ;
	int Time_maintain   ;
	int cure_disease1   ;
	int cure_disease2   ;
	int cure_disease3   ;
	int cure_disease4   ;
	int cure_disease5   ;
	int cure_disease6   ;
	int take_disease1   ;
	int take_disease2   ;
	int take_disease3   ;
	int take_disease4   ;
	int take_disease5   ;
	int take_disease6   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_Movp   ;
	int Change_end   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_WS   ;
	int Change_PS   ;
	int Time_maintain2   ;
	int Cure_poison   ;
	int Cure_stone_paralysis   ;
	int Cure_Mad   ;
	int Cure_sluggish   ;
	int Cure_Active   ;
	int ChangeNatural2   ;
	int Time_maintain3   ;
	int weight   ;
	int circulation_limit   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind;


public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int EatItem();
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLimit() { return circulation_limit; }
	void CopyItemClass( CItem_Herb *source, int ct );
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetAntiMagic( int *anti ) { return GetAntiMagic_1( anti, &anti_poison ); }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
	virtual void GetItemChangeAbility( int &count, int *type, int *value );
};


class CItem_Cook : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int hp   ;
	int ap   ;
	int mp   ;
	int nutrition1   ;
	int nutrition2   ;
	int nutrition3   ;
	int anti_poison   ;
	int anti_paralysis   ;
	int anti_magic   ;
	int anti_fire   ;
	int anti_ice   ;
	int anti_electric   ;
	int Time_maintain   ;
	int cure_disease1   ;
	int cure_disease2   ;
	int cure_disease3   ;
	int cure_disease4   ;
	int cure_disease5   ;
	int cure_disease6   ;
	int take_disease1   ;
	int take_disease2   ;
	int take_disease3   ;
	int take_disease4   ;
	int take_disease5   ;
	int take_disease6   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_Movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_WS   ;
	int Change_Ps   ;
	int Time_maintain2;
	int weight   ;
	int circulation_limit   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Use_Junk;
	int Item_kind;


public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int GetUseJunk() { return Use_Junk; }
	virtual int EatItem();
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetLimit() { return circulation_limit; }
	void CopyItemClass( CItem_Cook *source, int ct );
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetAntiMagic( int *anti ) { return GetAntiMagic_1( anti, &anti_poison ); }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
	virtual void GetItemChangeAbility( int &count, int *type, int *value );
};

class CItem_Potion	: public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int hp   ;
	int ap   ;
	int mp   ;
	int nutrition1   ;
	int nutrition2   ;
	int nutrition3   ;
	int anti_poison   ;
	int anti_paralysis   ;
	int anti_magic   ;
	int anti_fire   ;
	int anti_ice   ;
	int anti_electric   ;
	int Time_maintain   ;
	int cure_disease1   ;
	int cure_disease2   ;
	int cure_disease3   ;
	int cure_disease4   ;
	int cure_disease5   ;
	int cure_disease6   ;
	int take_disease1   ;
	int take_disease2   ;
	int take_disease3   ;
	int take_disease4   ;
	int take_disease5   ;
	int take_disease6   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_WS   ;
	int Change_PS   ;
	int Time_maintain2  ;
	int Cure_poison   ;
	int Cure_stone_paralysis   ;
	int Cure_Mad   ;
	int Cure_sluggish   ;
	int Cure_Active   ;
	int ChangeNatural2   ;
	int Time_maintain3   ;
	int weight   ;
	int circulation_limit   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Use_Junk;
	int Item_kind;


public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual int GetRButton() { return Mouse_MR; }
	virtual int GetUseJunk() { return Use_Junk; }
	virtual int EatItem();
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetLimit() { return circulation_limit; }
	virtual int GetItemKind() { return Item_kind; }
	void CopyItemClass( CItem_Potion *source, int ct );
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetAntiMagic( int *anti ) { return GetAntiMagic_1( anti, &anti_poison ); }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
	virtual void GetItemChangeAbility( int &count, int *type, int *value );
};


class CItem_Tool : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX] ;
	int lv   ;
	int Image_Number   ;
	int Repairable   ;
	int Repair_min   ;
	int Repair_max   ;
	int Repair_Skill2   ;
	int Repair_Skill2_min   ;
	int Repair_Res1   ;
	int Amt_of_Repair_Res1   ;
	int Repair_Res2   ;
	int Amt_of_Repair_Res2   ;
	int Repair_Res3   ;
	int Amt_of_Repair_Res3   ;
	int Repair_Tool   ;
	int Amt_of_abrasion   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_ws   ;
	int Change_ps   ;
	int Change_mag   ;
	int Change_dev   ;
	int shape_woman   ;
	int shape_man   ;
	int success_rate   ;
	int abrasion_per_turn   ;
	int weight   ;
	int max_duration   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind;


public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual int GetItemDuration() { if( max_duration ) return max_duration; else return 1; }
	virtual int GetItemShow( int is_man ) { return is_man? shape_man : shape_woman; }
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetRepairAble() { return Repairable; }
	void CopyItemClass( CItem_Tool *source, int ct );	
	virtual int is_equipable();
	virtual int is_equipable2(int dest_pos );
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
	virtual void GetItemChangeAbility( int &count, int *type, int *value );
};


class CItem_Weapon : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX] ;
	int lv   ;
	int Image_Number   ;
	int Repairable   ;
	int Repair_min   ;
	int Repair_max   ;
	int Repair_Skill2   ;
	int Repair_Skill2_min   ;
	int Repair_Res1   ;
	int Amt_of_Repair_Res1   ;
	int Repair_Res2   ;
	int Amt_of_Repair_Res2   ;
	int Repair_Res3   ;
	int Amt_of_Repair_Res3   ;
	int Repair_Tool   ;
	int Amt_of_abrasion   ;
	int Need2_lv   ;
	int Need2_str   ;
	int Need2_dex   ;
	int Need2_con   ;
	int Need2_wis   ;
	int Need2_int   ;
	int Need2_cha   ;
	int Need2_end   ;
	int Need2_Movp   ;
	int Need2_mor   ;
	int Need2_luc   ;
	int Need2_ws   ;
	int Need2_ps   ;
	int Need2_fame   ;
	int Need2_rank   ;
	int Need2_gender   ;
	int Need2_race   ;
	int Need2_min_age   ;
	int Need2_max_age   ;
	int Need3_lv   ;
	int Need3_str   ;
	int Need3_dex   ;
	int Need3_con   ;
	int Need3_wis   ;
	int Need3_int   ;
	int Need3_cha   ;
	int Need3_end   ;
	int Need3_Movp   ;
	int Need3_mor   ;
	int Need3_luc   ;
	int Need3_ws   ;
	int Need3_ps   ;
	int Need3_fame   ;
	int Need3_rank   ;
	int Need3_gender   ;
	int Need3_race   ;
	int Need3_min_age   ;
	int Need3_max_age   ;
	int Class_Warrior   ;
	int Class_Archer   ;
	int Class_Thief   ;
	int Class_Wizard   ;
	int Class_Cleric   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_Movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_ws   ;
	int Change_ps   ;
	int Change_mag   ;
	int Change_dev   ;
	int casting_Time   ;
	int Change_Life_max   ;
	int Change_mana_max   ;
	int Change_Divine_Max   ;
	int Change_health_max   ;
	int Increase_Poisoning   ;
	int Increase_Stone   ;
	int Increase_Magic   ;
	int Increase_Fire   ;
	int Increase_Ice   ;
	int Increase_lightning   ;
	int imunity_Cure_1   ;
	int Imunity_Cure_2   ;
	int Imunity_Cure_3   ;
	int Imunity_Cure_4   ;
	int Imunity_Cure_5   ;
	int Imunity_Cure_6   ;
	int SkillEffect   ;
	int shape_woman   ;
	int shape_man   ;
	int Skill_Ability   ;
	int Damage   ;
	int Hit_rate   ;
	int Defense_rate   ;
	int ArorDH   ;
	int nRange; // CSD-031101
	int nWeaponSpeed; // CSD-031101
	int effect   ;
	int weight   ;
	int max_duration   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Item_kind;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int kind_of_magic;
	int Incr_damage;


	// 34
public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual int GetItemDuration() { if( max_duration ) return max_duration; else return 1; }
	virtual int GetItemColor( ) { return  ( int )ReturnBlendRGB( imunity_Cure_1, Imunity_Cure_2, Imunity_Cure_3 ); }	// 011018 KHS 망또
	virtual int GetItemColorR( ) { return imunity_Cure_1; }
	virtual int GetItemColorG( ) { return Imunity_Cure_2; }
	virtual int GetItemColorB( ) { return Imunity_Cure_3; }
	virtual int GetItemShow( int is_man ) { return is_man? shape_man : shape_woman; }
	virtual int is_equipable();
	virtual void GetItemNeedAbility( int &count, int *type, int *value );
	virtual int is_equipable2(int dest_pos );
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int EquipItem();
	virtual int CurrEquip();
	virtual int ReleaseItem();
	virtual int GetDamage() { return Damage; }
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetRepairAble() { return Repairable; }
	virtual int GetSkillAbility() { return Skill_Ability; }
	virtual int GetGender() { return Need3_gender; }
	void CopyItemClass( CItem_Weapon *source, int ct );
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetHit_rate() { return Hit_rate; }
	virtual int GetDefense_rate() { return Defense_rate; }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
	virtual void GetItemChangeAbility( int &count, int *type, int *value );
	virtual int GetAR_or_DH() { return ArorDH; }
	virtual int GetNeedNation() { return Imunity_Cure_4; }
	virtual int GetNeedLv() { return Imunity_Cure_5; }
	virtual int GetNeedFame() { return Imunity_Cure_6; }

	virtual int GetNeedDualStep(){ return Need2_max_age; }//021113 lsw

	virtual int GetClassWarriorAble(){ return Class_Warrior;}
	virtual int GetClassArcherAble(){ return Class_Archer;}
	virtual int GetClassThiefAble(){ return Class_Thief;}
	virtual int GetClassWizardAble(){ return Class_Wizard;}
	virtual int GetClassClericAble(){ return Class_Cleric;}
};

class CItem_Disposable : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX] ;
	char Han_Name[ITEM_NAME_MAX] ;
	int lv   ;
	int Image_Number   ;
	int Need3_lv   ;
	int Need3_str   ;
	int Need3_dex   ;
	int Need3_con   ;
	int Need3_wis   ;
	int Need3_int   ;
	int Need3_cha   ;
	int Need3_end   ;
	int Need3_Movp   ;
	int Need3_mor   ;
	int Need3_luc   ;
	int Need3_ws   ;
	int Need3_ps   ;
	int Need3_fame   ;
	int Need3_rank   ;
	int Need3_gender   ;
	int Need3_race   ;
	int Need3_min_age   ;
	int Need3_max_age   ;
	int Class_Warrior   ;
	int Class_Archer   ;
	int Class_Thief   ;
	int Class_Wizard   ;
	int Class_Cleric   ;
	int shape_woman   ;
	int shape_man   ;
	int Skill_Ability   ;
	int Damage   ;
	int Hit_rate   ;
	int fire_speed   ;
	int nRange; // CSD-031101
	int damage_rage   ;
	int Through   ;
	int nWeaponSpeed; // CSD-031101
	int effect_num   ;
	int weight   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Quantity   ;
	int Item_kind   ;


public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual int GetItemShow( int is_man ) { return is_man? shape_man : shape_woman; }
	virtual int is_equipable();
	virtual void GetItemNeedAbility( int &count, int *type, int *value );
	virtual int is_equipable2(int dest_pos );
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int GetDamage() { return Damage; }
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetGender() { return Need3_gender; }
	virtual int GetQuantity() { return Quantity; }
	void CopyItemClass( CItem_Disposable *source, int ct );
	virtual int GetSkillAbility() { return Skill_Ability; }
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetHit_rate() { return Hit_rate; }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
};


class CItem_Armor : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int Repairable   ;
	int Repair_min   ;
	int Repair_max   ;
	int Repair_Skill2   ;
	int Repair_Skill2_min   ;
	int Repair_Res1   ;
	int Amt_of_Repair_Res1   ;
	int Repair_Res2   ;
	int Amt_of_Repair_Res2   ;
	int Repair_Res3   ;
	int Amt_of_Repair_Res3   ;
	int Repair_Tool   ;
	int Amt_of_abrasion   ;
	int Need2_lv   ;
	int Need2_str   ;
	int Need2_dex   ;
	int Need2_con   ;
	int Need2_wis   ;
	int Need2_int   ;
	int Need2_cha   ;
	int Need2_end   ;
	int Need2_Movp   ;
	int Need2_mor   ;
	int Need2_luc   ;
	int Need2_ws   ;
	int Need2_ps   ;
	int Need2_fame   ;
	int Need2_rank   ;
	int Need2_gender   ;
	int Need2_race   ;
	int Need2_min_age   ;
	int Need2_max_age   ;
	int Need3_lv   ;
	int Need3_str   ;
	int Need3_dex   ;
	int Need3_con   ;
	int Need3_wis   ;
	int Need3_int   ;
	int Need3_cha   ;
	int Need3_end   ;
	int Need3_Movp   ;
	int Need3_mor   ;
	int Need3_luc   ;
	int Need3_ws   ;
	int Need3_ps   ;
	int Need3_fame   ;
	int Need3_rank   ;
	int Need3_gender   ;
	int Need3_race   ;
	int Need3_min_age   ;
	int Need3_max_age   ;
	int Class_Warrior   ;
	int Class_Archer   ;
	int Class_Thief   ;
	int Class_Wizard   ;
	int Class_Cleric   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_Movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_ws   ;
	int Change_ps   ;
	int Change_mag   ;
	int Change_dev   ;
	int casting_Time   ;
	int Change_Life_max   ;
	int Change_mana_max   ;
	int Change_Divine_Max   ;
	int Change_health_max   ;
	int Increase_Poisoning   ;
	int Increase_Stone   ;
	int Increase_Magic   ;
	int Increase_Fire   ;
	int Increase_Ice   ;
	int Increase_lightning   ;
	int imunity_Cure_1   ;
	int Imunity_Cure_2   ;
	int Imunity_Cure_3   ;
	int Imunity_Cure_4   ;
	int Imunity_Cure_5   ;
	int Imunity_Cure_6   ;
	int SkillEffect   ;
	int shape_woman   ;
	int shape_man   ;
	int Skill_Ability   ;
	int Damage   ;
	int Hit_rate   ;
	int Defense_rate   ;
	int AR_or_DH   ;
	int Defense_power   ;
	int body   ;
	int arms   ;
	int head   ;
	int leg   ;
	int neck   ;
	int hand   ;
	int weight   ;
	int max_duration   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind   ;
	int kind_of_magic;
	int Incr_damage;

public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual int GetItemDuration() { if( max_duration ) return max_duration; else return 1; }
	virtual int GetItemShow( int is_man ) { return is_man? shape_man : shape_woman; }
	virtual int GetItemColor( ) { return  ( int )ReturnBlendRGB( imunity_Cure_1, Imunity_Cure_2, Imunity_Cure_3 ); }	// 011018 KHS 망또
	virtual int GetItemColorR( ) { return imunity_Cure_1; }
	virtual int GetItemColorG( ) { return Imunity_Cure_2; }
	virtual int GetItemColorB( ) { return Imunity_Cure_3; }	
	virtual int is_equipable();
	virtual void GetItemNeedAbility( int &count, int *type, int *value );
	virtual int is_equipable2(int dest_pos );
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int EquipItem();
	virtual int CurrEquip();
	virtual int ReleaseItem();
	virtual int GetDamage() { return Damage; }
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int GetRepairAble() { return Repairable; }
	virtual int GetGender() { return Need3_gender; }
	void CopyItemClass( CItem_Armor *source, int ct );
	virtual int GetSkillAbility() { return Skill_Ability; }
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetAR_or_DH() { return AR_or_DH; }
	virtual int GetHit_rate() { return Hit_rate; }
	virtual int GetDefense_rate() { return Defense_rate; }
	virtual int GetMovePoint() { return Defense_power; }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
	virtual void GetItemChangeAbility( int &count, int *type, int *value );
	virtual int GetNeedNation() { return Imunity_Cure_4; }
	virtual int GetNeedLv() { return Imunity_Cure_5; }
	virtual int GetNeedFame() { return Imunity_Cure_6; }
		
	virtual int GetNeedDualStep(){ return Need2_max_age; }//021113 lsw

	virtual int GetClassWarriorAble(){ return Class_Warrior;}
	virtual int GetClassArcherAble(){ return Class_Archer;}
	virtual int GetClassThiefAble(){ return Class_Thief;}
	virtual int GetClassWizardAble(){ return Class_Wizard;}
	virtual int GetClassClericAble(){ return Class_Cleric;}
	
};

class CItem_Accessory : public CItem
{
//protected :
public :
	int Item_id   ;
	char Name[ITEM_NAME_MAX]  ;
	char Han_Name[ITEM_NAME_MAX]  ;
	int lv   ;
	int Image_Number   ;
	int Repairable   ;
	int Repair_min   ;
	int Repair_max   ;
	int Repair_Skill2   ;
	int Repair_Skill2_min   ;
	int Repair_Res1   ;
	int Amt_of_Repair_Res1   ;
	int Repair_Res2   ;
	int Amt_of_Repair_Res2   ;
	int Repair_Res3   ;
	int Amt_of_Repair_Res3   ;
	int Repair_Tool   ;
	int Amt_of_abrasion   ;
	int Need2_lv   ;
	int Need2_str   ;
	int Need2_dex   ;
	int Need2_con   ;
	int Need2_wis   ;
	int Need2_int   ;
	int Need2_cha   ;
	int Need2_end   ;
	int Need2_Movp   ;
	int Need2_mor   ;
	int Need2_luc   ;
	int Need2_ws   ;
	int Need2_ps   ;
	int Need2_fame   ;
	int Need2_rank   ;
	int Need2_gender   ;
	int Need2_race   ;
	int Need2_min_age   ;
	int Need2_max_age   ;
	int Need3_lv   ;
	int Need3_str   ;
	int Need3_dex   ;
	int Need3_con   ;
	int Need3_wis   ;
	int Need3_int   ;
	int Need3_cha   ;
	int Need3_end   ;
	int Need3_Movp   ;
	int Need3_mor   ;
	int Need3_luc   ;
	int Need3_ws   ;
	int Need3_ps   ;
	int Need3_fame   ;
	int Need3_rank   ;
	int Need3_gender   ;
	int Need3_race   ;
	int Need3_min_age   ;
	int Need3_max_age   ;
	int Class_Warrior   ;
	int Class_Archer   ;
	int Class_Thief   ;
	int Class_Wizard   ;
	int Class_Cleric   ;
	int Change_str   ;
	int Change_dex   ;
	int Change_con   ;
	int Change_wis   ;
	int Change_int   ;
	int Change_cha   ;
	int Change_end   ;
	int Change_Movp   ;
	int Change_mor   ;
	int Change_luc   ;
	int Change_ws   ;
	int Change_ps   ;
	int Change_mag   ;
	int Change_dev   ;
	int casting_Time   ;
	int Change_Life_max   ;
	int Change_mana_max   ;
	int Change_Divine_Max   ;
	int Change_health_max   ;
	int Increase_Poisoning   ;
	int Increase_Stone   ;
	int Increase_Magic   ;
	int Increase_Fire   ;
	int Increase_Ice   ;
	int Increase_lightning   ;
	int imunity_Cure_1   ;
	int Imunity_Cure_2   ;
	int Imunity_Cure_3   ;
	int Imunity_Cure_4   ;
	int Imunity_Cure_5   ;
	int Imunity_Cure_6   ;
	int Skill_effect   ;
	int Skill_Ability   ;
	int Damage   ;
	int Hit_rate   ;
	int Defense_rate   ;
	int AR_or_DH   ;
	int Defense_power   ;
	int body   ;
	int arms   ;
	int head   ;
	int leg   ;
	int neck   ;
	int hand   ;
	int JobSkill_Farming   ;
	int JobSkill_Mining   ;
	int JobSkill_Fishing   ;
	int JobSkill_Cutdown   ;
	int JobSkill_Butchering   ;
	int JobSkill_Herb_digging   ;
	int JobSkill_Herding   ;
	int JobSkill_Carpentary   ;
	int JobSkill_Blacksmithy   ;
	int JobSkill_Cooking   ;
	int JobSkill_Tailoring   ;
	int JobSkill_Bowcraft   ;
	int JobSkill_Alchemy   ;
	int JobSkill_Candle   ;
	int JobSkill_repair   ;
	int JobSkill_reutil   ;
	int JobSkill_music   ;
	int JobSkill_detect   ;
	int JobSkill_hide   ;
	int JobSkill_campiing   ;
	int JobSkill_taming   ;
	int comm_animal   ;
	int comm_human   ;
	int comm_orc   ;
	int comm_elf   ;
	int comm_fairy   ;
	int comm_hobbit   ;
	int comm_dwarf   ;
	int comm_dragon   ;
	int animal_lore   ;
	int arms_lore   ;
	int evaluate_int   ;
	int evaluate_wis   ;
	int analomy   ;
	int item_ident   ;
	int taste_ident   ;
	int Appraisal   ;
	int healing   ;
	int spirituality   ;
	int poisoning   ;
	int entrapment   ;
	int scrooling   ;
	int cunning   ;
	int bomb_play   ;
	int tactice   ;
	int crapple   ;
	int sword   ;
	int archey   ;
	int fencing   ;
	int mace   ;
	int pierce   ;
	int whid   ;
	int hurt   ;
	int parrying   ;
	int double_sword   ;
	int double_mace   ;
	int weight   ;
	int max_duration   ;
	int Durationpermin   ;
	int object   ;
	int wear_able   ;
	int base_value   ;
	int base_store   ;
	int store_const   ;
	int store_plus_const   ;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Item_kind   ;
	int kind_of_magic;
	int Incr_damage;


public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual int GetItemDuration() { if( max_duration ) return max_duration; else return 1; }
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int EquipItem();
	virtual int CurrEquip();
	virtual int ReleaseItem();
	virtual int GetDamage() { return Damage; }
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int is_equipable();
	virtual void GetItemNeedAbility( int &count, int *type, int *value );
	virtual int is_equipable2(int dest_pos );
	virtual int GetRepairAble() { return Repairable; }
	virtual int GetGender() { return Need3_gender; }
	void CopyItemClass( CItem_Accessory *source, int ct );
	virtual int GetSkillAbility() { return Skill_Ability; }
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetAR_or_DH() { return AR_or_DH; }
	virtual int GetHit_rate() { return Hit_rate; }
	virtual int GetDefense_rate() { return Defense_rate; }
	virtual int GetMovePoint() { return Defense_power; }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
	virtual void GetItemChangeAbility( int &count, int *type, int *value );
	virtual int GetItemColor( ) { return  ( int )ReturnBlendRGB( imunity_Cure_1, Imunity_Cure_2, Imunity_Cure_3 ); }	// 011018 KHS 망또
	virtual int GetItemColorR( ) { return imunity_Cure_1; }
	virtual int GetItemColorG( ) { return Imunity_Cure_2; }
	virtual int GetItemColorB( ) { return Imunity_Cure_3; }	
	virtual int GetNeedNation() { return Imunity_Cure_4; }
	virtual int GetNeedLv() { return Imunity_Cure_5; }
	virtual int GetNeedFame() { return Imunity_Cure_6; }
		
	virtual int GetNeedDualStep(){ return Need2_max_age; }//021113 lsw

	virtual int GetClassWarriorAble(){ return Class_Warrior;}
	virtual int GetClassArcherAble(){ return Class_Archer;}
	virtual int GetClassThiefAble(){ return Class_Thief;}
	virtual int GetClassWizardAble(){ return Class_Wizard;}
	virtual int GetClassClericAble(){ return Class_Cleric;}
};


class CItem_Etc : public CItem
{
// protected :
public :
	int Item_id;
	char Name[ITEM_NAME_MAX];
	char Han_Name[ITEM_NAME_MAX];
	int lv;
	int Image_Number;
	int Repairable;
	int Repair_min;
	int Repair_max;
	int Repair_Skill2;
	int Repair_Skill2_min;
	int Repair_Res1;
	int Amt_of_Repair_Res1;
	int Repair_Res2;
	int Amt_of_Repair_Res2;
	int Repair_Res3;
	int Amt_of_Repair_Res3;
	int Repair_Tool;
	int Amt_of_abrasion;
	int Need2_lv;
	int Need2_str;
	int Need2_dex;
	int Need2_con;
	int Need2_wis;
	int Need2_int;
	int Need2_cha;
	int Need2_end;
	int Need2_Movp;
	int Need2_mor;
	int Need2_luc;
	int Need2_ws;
	int Need2_ps;
	int Need2_Magery;
	int Need2_Orison;
	int Need2_fame;
	int Need2_rank;
	int Need2_gender;
	int Need2_race;
	int Need2_min_age;
	int Need2_max_age;
	int Need3_lv;
	int Need3_str;
	int Need3_dex;
	int Need3_con;
	int Need3_wis;
	int Need3_int;
	int Need3_cha;
	int Need3_end;
	int Need3_Movp;
	int Need3_mor;
	int Need3_luc;
	int Need3_ws;
	int Need3_ps;
	int Need3_fame;
	int Need3_rank;
	int Need3_gender;
	int Need3_race;
	int Need3_min_age;
	int Need3_max_age;
	int Class_Warrior;
	int Class_Archer;
	int Class_Thief;
	int Class_Wizard;
	int Class_Cleric;
	int Change_str;
	int Change_dex;
	int Change_con;
	int Change_wis;
	int Change_int;
	int Change_cha;
	int Change_end;
	int Change_mor;
	int Change_luc;
	int Change_ws;
	int Change_ps;
	int Change_mag;
	int Change_dev;
	int casting_Time;
	int Change_Life_max;
	int Change_mana_max;
	int Change_Divine_Max;
	int Change_health_max;
	int Increase_Poisoning;
	int Increase_Stone;
	int Increase_Magic;
	int Increase_Fire;
	int Increase_Ice;
	int Increase_lightning;
	int imunity_Cure_1;
	int Imunity_Cure_2;
	int Imunity_Cure_3;
	int Imunity_Cure_4;
	int Imunity_Cure_5;
	int Imunity_Cure_6;
	int Skill_effect;
	int Substitution_magic;
	int Skill_Ability;
	int Damage;
	int Hit_rate;
	int Defense_rate;
	int AR_or_DH;
	int Defense_power;
	int body;
	int arms;
	int head;
	int leg;
	int neck;
	int hand;
	int JobSkill_Farming;
	int JobSkill_Mining;
	int JobSkill_Fishing;
	int JobSkill_Cutdown;
	int JobSkill_Butchering;
	int JobSkill_Herb_digging;
	int JobSkill_Herding;
	int JobSkill_Carpentary;
	int JobSkill_Blacksmithy;
	int JobSkill_Cooking;
	int JobSkill_Tailoring;
	int JobSkill_Bowcraft;
	int JobSkill_Alchemy;
	int JobSkill_Candle;
	int JobSkill_repair;
	int JobSkill_reutil;
	int JobSkill_music;
	int JobSkill_detect;
	int JobSkill_hide;
	int JobSkill_campiing;
	int JobSkill_taming;
	int comm_animal;
	int comm_human;
	int comm_orc;
	int comm_elf;
	int comm_fairy;
	int comm_hobbit;
	int comm_dwarf;
	int comm_dragon;
	int animal_lore;
	int arms_lore;
	int evaluate_int;
	int evaluate_wis;
	int analomy;
	int item_ident;
	int taste_ident;
	int Appraisal;
	int healing;
	int spirituality;
	int poisoning;
	int entrapment;
	int scrooling;
	int cunning;
	int bomb_play;
	int tactice;
	int crapple;
	int sword;
	int archey;
	int fencing;
	int mace;
	int pierce;
	int whid;
	int hurt;
	int parrying;
	int double_sword;
	int double_mace;
	int change_music;
	int change_detect;
	int change_hide;
	int change_amping;
	int change_taming;
	int weight;
	int max_duration;
	int object;
	int wear_able;
	int base_value;
	int base_store;
	int store_const;
	int store_plus_const;
	int Item_kind;
	int Mouse_MR;
	int Mouse_MRD;
	int Mouse_MR_Effect;
	int Mouse_Mr_Wave;
	int Use_Junk;
	int kind_of_magic;
	int Incr_damage;




public:
	virtual int GetImageNumber() { return Image_Number;}
	virtual int GetObjectNumber() { return object; }
	virtual int ReadItemTable( char *temp );
	virtual int GetItemId(){return Item_id;}
	virtual int GetItemClass(){return wear_able;}
	virtual int GetItemWeight(){ return weight; }
	virtual int GetItemValue(){ return base_value; }
	virtual void SetItemValue(int p) { base_value = p; return; }
	virtual int GetItemDuration() { if( max_duration ) return max_duration; else return 1; }
	virtual char *GetItemName() { return Name; }
	virtual char *GetItemHanName() 
	{ 
		#ifdef USA_LOCALIZING_
				return Name; 
		#else
				return Han_Name; 
		#endif
	}
	virtual int GetRButton() { return Mouse_MR; }
	virtual int GetUseJunk() { return Use_Junk; }
	virtual int EquipItem();
	virtual int CurrEquip();		
	virtual int ReleaseItem();
	virtual int GetDamage() { return Damage; }
	virtual int GetRButtonSound() { return Mouse_Mr_Wave; }
	virtual int GetItemKind() { return Item_kind; }
	virtual int is_equipable();
	virtual void GetItemNeedAbility( int &count, int *type, int *value );
	virtual void GetItemChangeAbility( int &count, int *type, int *value );
	virtual int is_equipable2(int dest_pos );
	virtual int GetRepairAble() { return Repairable; }
	virtual int GetGender() { return Need3_gender; }
	void CopyItemClass( CItem_Etc *source, int ct );
	virtual int GetSkillAbility() { return Skill_Ability; }
	virtual int GetMouse_MRD() { return Mouse_MRD; }
	virtual int GetAR_or_DH() { return AR_or_DH; }
	virtual int GetHit_rate() { return Hit_rate; }
	virtual int GetDefense_rate() { return Defense_rate; }
	virtual int GetMovePoint() { return Defense_power; }
	virtual int GetMouse_MR_Effect() { return Mouse_MR_Effect; }
	virtual int GetItemColor( ) { return  ( int )ReturnBlendRGB( imunity_Cure_1, Imunity_Cure_2, Imunity_Cure_3 ); }	// 011018 KHS 망또
	virtual int GetItemColorR( ) { return imunity_Cure_1; }
	virtual int GetItemColorG( ) { return Imunity_Cure_2; }
	virtual int GetItemColorB( ) { return Imunity_Cure_3; }	
	virtual int GetNeedNation() { return Imunity_Cure_4; }
	virtual int GetNeedLv() { return Imunity_Cure_5; }
	virtual int GetNeedFame() { return Imunity_Cure_6; }

		
	virtual int GetNeedDualStep(){ return Need2_max_age; }//021113 lsw

	virtual int GetClassWarriorAble(){ return Class_Warrior;}
	virtual int GetClassArcherAble(){ return Class_Archer;}
	virtual int GetClassThiefAble(){ return Class_Thief;}
	virtual int GetClassWizardAble(){ return Class_Wizard;}
	virtual int GetClassClericAble(){ return Class_Cleric;}
};

struct CItem_List 
{
	CItem *Item_type[Num_Of_Item_Type][ITEM_MAX];
	int nItem[Num_Of_Item_Type] ;
};

/////////////////////////////////////////////////////////////////////////////////
// 아이템 원래 포지션을 위한 디파인과 스트럭
#define HAND		0
#define INV			1
#define EQUIP		2
#define	QUICK		3
#define BANK		4
#define BOX			5
#define MAKE		6
#define OTHER_CH	7

#define IM_CHARACTER	124
#define GROUND	125
#define USE		126


/////////////////////////////////////////////////////////////////////////////////
typedef struct tagPOS
{
	char type;
	short int p1, p2, p3;
} POS;						// 아이템 위치 저장

/////////////////////////////////////////////////////////////////////////////////
// 040105 YGI 보물
///////////////////////////////////////////////////////
struct CTreasureMapItem
{
	DWORD mapnumber	:7;
	DWORD x			:10;
	DWORD y			:10;
//	DWORD use		:1;	// 사용했는가?
	DWORD isChecked	:1;	// 생성한 후에 좌표를 얻어 왔는가?
	DWORD offset	:2;
	DWORD offset2	:2;
};

/////////////////////////////////////////////////////////////////
// 아이템 객체 속성
typedef struct ITEMATTR//021126 lsw
{
	short int item_no;
	unsigned int attr[6];	
} ItemAttr;

#define OTHER_INV_MAX	15
typedef struct tag_Other_Ch		// 다른 PC 정보
{
	DWORD	id;
	bool	bLive;
	ItemAttr inv[OTHER_INV_MAX];

}OtherCharacter, *lpOtherCharacter;
extern OtherCharacter You;
//////////////////////////////////////////////////////////////////

typedef struct tagMakeItem		// 작업대에서 사용할 아이템 구조체
{
	int			item_no;
	int			x;
	int			y;
	RECT		rect;
	ItemAttr	attr;
	POS			pos;
} MakeItem;

extern MakeItem make_item_list[20];
// ----------------------- magic -----------------------//
#define		WZD				0
#define		PRI				1

#define		Link			0	// 조합 마법		
#define		ImgNum			1	// 그림 번호
#define		MagicClass		2	// 계열
#define		Lv				3	// 매직 레벨
//#define		Lv_Img			4	// 2레벨 그림
//#define		Lv_Rect			6	// 2레벨 밝은 그림

#define		MAGIC_DATA_MAX	4

#define MAX_MAGIC_			200
#define Explain_MAX			200
#define MAGIC_SPR_MAX		108

class CMagic 
{
private :
	int		m_iMagicNum;
	char	m_szName[ITEM_NAME_MAX];
	char	m_szHanName[ITEM_NAME_MAX];
	int		m_data[MAGIC_DATA_MAX];
	char	m_szExplain[1024];

public :

	int	 GetImage( ) { return m_data[ImgNum]; }
	int  GetClass() { return m_data[MagicClass]; }
	void Explain(const int iX, const int iY, const BOOL bSpell );		// wizard
	void SetArray();					// 그 매직이 들어갈 자리에 셋팅한다.
	void SetArray2();
	
	const char* GetName() const { return m_szName; }
	void SetName(const char* szName) { strcpy(m_szName,szName); }
	
	const char* GetHanName() const { return m_szHanName; }
	void SetHanName(const char* szName) { strcpy(m_szHanName,szName); }

	char* GetExplain() { return m_szExplain; }
	void SetExplain(const char* szName) { strcpy(m_szExplain,szName); }
	
	int GetMagicNum() const{ return m_iMagicNum; }
	void SetMagicNum(const int iValue) { m_iMagicNum = iValue;}

	void SetData(const int iType, const int iValue){m_data[iType] = iValue; }
		
	void DeleteMagic( ); 
	void LoadExplain( int num, FILE *fp );
};
//#### 오른쪽 마우스 클릭하면 나오는 메뉴의 그림 번호 정의 ####
#define		BOX_OPEN	941
#define		MONEY_BOX	942
#define		MAGIC_BOOK	943

//#### 오른쪽 마우스 버튼 클릭후 실행하는 종류 정의 ####
#define		USE_ITEM			1		// 소비되어 사라진다.
#define		DIVIDE_ITEM			3		// 아이템의 숫자를 분리한다.
#define		REMAIN_ITEM			4		// 사용후 껍데기 남음
#define		MAGIC_BOOK_ITEM		5		// 마법책이 뜬다.
#define		DIARY_ITEM			6		// 일기책이 뜬다.
#define		BOX_ITEM			7		// 뭔지 알지?
#define		TIME_LIMIT			9		// 뭔지 알지?//011212 lsw
#define		DUAL_CHANGE_ITEM	8 // 듀얼 클래스로 변환하기 위한 아이템
#define		SOKSUNG_ITEM		9 // 듀얼 클래스로 변환하기 위한 아이템

class CItem_Join
{
public :	
	int no;
	int item_id;
	char item_name[31];
	int item_amount;
	char han_name[31];	
	int need_know;	
	int teach_place;	
	int need_skill1;	
	int skill1_minimum;	
	int skill1_maximum;	
	int need_skill2;	
	int skill2_minimum;	
	int skill2_maximum;	
	int tech_price;	
	int Tool_kind;	
	int Flatform_kind;	
	int resource1;	
	int resource1_amount;	
	int resource2;	
	int resource2_amount;	
	int resource3;	
	int resource3_amount;	
	int resource4;	
	int resource4_amount;	
	int Result_resource1;	
	int Result_resource1_amount;	
	int Result_resource2;	
	int Result_resource2_amount;	
	int reutilizerble;	
	int k1;	
	int k2;
	char explain[MAX_STRING];

public :
	void ReadItemJoin( char *string );

};

typedef struct tag_ItemJoinSelect		 // 아이템 만들기에서 선택
{
	int join_no;		// 결함 테이블의 카운트 번호	--> 재료 테이블을 가져오기위한 첨자로 사용
	int item_no;		// 아이템 아이디
	int skill_min;
	int kind;
	int skillability;
}ItemJoinSelect;

typedef struct tag_PosItem
{
	int item_no;
	POS pos;
}PosItem;

typedef struct tag_MagicByClass
{
	int magic[13][20];
	int max[13];
}MagicByClass;

#define ITEM_JOIN_MAX	1000
#define MAX_SKILL		200

#define MAX_ITEM_EXPLAIN_MSG	500
#define MAX_ITEM_EXPLAIN_COUNT	50		// 설명가능한 아이템

struct CItemExplainBasic
{
public : 
	int		m_nItemId;
	char	m_szHanName[30];
	char	explain[MAX_ITEM_EXPLAIN_MSG];
};


class CItemExplain
{
public : 
	CItemExplainBasic	m_Item[MAX_ITEM_EXPLAIN_COUNT];
	int m_nCount;		// 실제 수...

public :
	CItemExplain() { memset( m_Item, 0 , sizeof( CItemExplainBasic) * MAX_ITEM_EXPLAIN_COUNT ); }
	void GetItemExplain( int n = 0 );
	void SortingById();				// text로 읽어 들일때 아이디 순으로 소팅을 한다.
	int  SearchById( int id );		// 2분법으로 값을 찾는다.		// 파라메타를 넘겨주고 찾지 못하면 -1을 넘겨 준다.
	char *GetName( int para );
	char *GetExplain( int para );

	int SetValue( int para, char *text );

};

extern CItemExplain g_GM_Item;

/////////////////////////////////////////////////////////////////////////////////
extern CMagic	magic[MAX_MAGIC_];
extern Spr		magic_spr[MAGIC_SPR_MAX];
extern CItem_Join	item_join[ITEM_JOIN_MAX];
extern int		inclusive_to_skill[MAX_SKILL_KIND][8];
extern int		Hero_Ability[20];
/////////////////////////////////////////////////////////////////////////////////


extern Spr		Icon1[ITEM_MAX][10];
extern Spr		Icon2[ITEM_MAX][10];
extern Spr		Icon3[ITEM_MAX][10];
extern Spr		Icon4[ITEM_MAX][10];
extern Spr		Icon5[ITEM_MAX][10];
extern Spr		Icon6[ITEM_MAX][10];
extern Spr		Icon7[ITEM_MAX][10];
extern Spr		Icon8[ITEM_MAX][10];
extern Spr		Icon9[ITEM_MAX][10];
extern Spr		Icon10[ITEM_MAX][10];
extern Spr		Icon11[ITEM_MAX][10];
extern Spr		Icon12[ITEM_MAX][10];
extern Spr		Icon13[ITEM_MAX][10];


extern struct CItem_List Item_Ref;			// 아이템
extern int HandItemOn;

extern void		InventoryBackUp();
extern void		InventoryRestore();

extern void		ChangeEquip( LPCHARACTER lpCharacter , ItemAttr *equip_item, int type ); //  Menu.cpp
extern int		CallItemVirtualFunction( int function, int item_number );
extern void		SetItemMakeOk( ItemAttr Item );

////////////////////////////////////////////////////////////////////////////////////
#define	CIA_DURATION		101
#define CIA_VIEW_CHANGE		102
#define CIA_MUCH			103

extern int	ChangeItemAttr( ItemAttr *item, int type, int plus, int skill );
extern CItem *ItemUnit( int type, int item_no );
extern CItem *ItemUnit( ItemAttr item );
extern CItem *ItemUnit( int item_no );
extern int DeleteItem( ItemAttr *item );
extern int DeleteItem( POS pos );
extern bool CanLearnItem( int item, int price, int &join_no);
extern bool CanLearnItemForExplain( int item );
extern ItemAttr Item_Number2Attr( int item );


extern	void	ReqItemInfo0();
extern	void	ReqItemInfo1();
extern	void	ReqItemInfo2( int count );
extern	void	ReqMagicInfo();
extern	void	ReqSkillInfo();
extern	void	ReqConnectInfo();

extern	void	SendLearnSkill( int skill_no, int guild_no );
extern	void	GetItemDuration( ItemAttr item, WORD &d_curr, WORD &d_max );
extern	inline int GetItemNumber( int item_number , int& type, int& num );

extern	void	SetItemPos(int type, POS*);
extern	void	SetItemPos(int type, int p, POS*);
extern	void	SetItemPos(int type, int p1, int p2, int p3, POS*);
extern	POS		IP_base;					//  아이템이 원래 있었던 자리

extern	ItemAttr InvItemAttr[3][3][8];
extern	ItemAttr EquipItemAttr[8];
extern	ItemAttr QuickItemAttr[7];
extern	ItemAttr BankItemAttr[5][3][6];
extern	ItemAttr HandItemAttr;
extern	ItemAttr SupperItemAttr[3][15];

extern	ItemAttr InvAttrBackUp[3][3][8];
extern	ItemAttr EquipAttrBackUp[8];
extern	ItemAttr QuickAttrBackUp[7];
extern	ItemAttr HandAttrBackUp;

extern	void FreeItemIcon();
extern	int GetTotalWeight( );		// 현재 내가 소지하고 있는 아이템의 총 무게 
extern	int CalMovpByWeight();		// 아이템 무게에 의한 이동수치 변환 계산 메소드
extern char g_MagicName[MAX_MAGIC_][31];
extern int n_MagicBagic[2][12][15];

extern ItemAttr GenerateItem( int item_no )	;
extern int GetLineItemExplain( ItemAttr item , char explain[5][40]);
extern void FreeItemTable();

extern int menu_to_set[];
extern char *GetItemHanName( int item_id );
extern void CheckAbility();
extern void ItemSoundOfGetItem( int item_no );

extern int CheckInventory( const int iItemNo, const int iNeedCount );//021111 lsw
extern int GetSkillMother( int kind , int skill_mother[], int max );
extern bool EqualItem( const ItemAttr *source, const ItemAttr *target );		// 두 아이템이 같은지 체크
#endif//AFX_ITEM_TABLE_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_
