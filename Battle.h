// Battle.h: interface for the CBattle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLE_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_)
#define AFX_BATTLE_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
///////////////////////////////////////////////////////////////////////////////
// 전투 관한 정의
namespace battle
{
	enum BATTLE_TYPE
	{	//< CSD-031014
		// 물리적 공격 
		PHYSICAL = 0,                  
		// 마법
		LIFE_UP = 1,                   // 체력회복 
		PROTECTION = 2,                // 보호
		SHIELD = 3,                    // 보호막
		DECIPHER = 4,                  // 해독 마법
		HEAL_DISEASE = 5,              // 질병치료
		GREAT_HEAL = 6,                // 대체력회복
		SLOW = 10,                     // 속도저하
		CONFUSION = 11,                // 혼란
		INTOXICATION = 12,             // 중독
		STONELY =	13,                // 석화
		ABSOLUTE_PARALYSIS = 14,       // 절대마비
		TOXIC_CLOUD = 15,              // 독구름  
		DISEASE_INDUCE = 16,           // 질병유발 
		VAMPIRE_TOUCH = 17,            // 뱀파이어의 손길
		SPIDER_WEB = 19,               // 거미줄  
		MAGICAL_TRAP = 20,             // 마법트랩
		ENCHANT_WEAPON = 21,           // 인첸트웨폰
		TRANSPARENCY = 22,             // 투명주문
		NULLITY_MAGIC = 24,            // 마법무효화  
		MINOR_AMPLIFY = 25,            // 소증폭
		MAGIC_AMPLIFY = 26,            // 마법증폭
		FIRE_RESIST = 28,              // 불에 대한 저항  
		FIRE_ARROW = 29,               // 화염화살 
		FIRE_BALL= 30,                 // 화염구
		SCORCHER = 31,                 // 화염군(화염방사)
		CIRCLE_FLAME= 33,              // 방사화염
		FIRE_WALL = 34,                // 화염벽
		FLAME_WAVE = 35,               // 움직이는 화염벽
		APOCALYPSE = 36,               // 지옥의 불
		ICE_RESIST = 37,               // 빙계 저항
		ICE_ARROW = 38,                // 얼음화살
		ICE_BALL = 39,                 // 얼음구 
		FREEZE_5TILE_RADIUS = 41,      // 반경 5타일 얼림	
		ICE_WALL = 42,                 // 얼음벽  	
		CIRCLE_ICE = 43,               // 원형얼음 공격
		ICE_STORM = 44,                // 얼음폭풍
		FREEZE_9TILE_RADIUS = 45,      // 반경 9타일 얼리기	
		LIGHT = 46,                    // 빛
		ELECT_RESIST = 47,             // 전격에 대한 저항
		LIGHTNING_BOLT = 48,           // 벼락
		CHAIN_LIGHTNING = 50,          // 유도 전격
		CIRCLE_LIGHTNING = 51,         // 원형 전격 
		LIGHTNING = 52,                // 직선 전격
		TWINKLE_STAR = 53,             // 수호성
		LIGHTNING_5TILE_RADIUS = 54,   // 반경 5타일 전격
		LOW_MONSTER_RECALL = 55,       // 저급 몬스터 소환
		MIDDLE_MONSTER_RECALL = 58,    // 중급 몬스터 소환
		SNOWSTORM	= 59,			   // 눈보라의 폭풍
		ICE_BREATH2	= 60,			   // 아이스 브레스2
		BLOOD_LIGHTNING = 61,		   // 블러드 라이트닝
		PHASING = 64,                  // 장소이동
		MEMORIZING_LOCATION = 65,      // 이동장소기억
		TELEPORT = 66,                 // 순간이동
		MULTI_PORT = 67,               // 복수차원이동
		TOWN_PORTAL = 68,              // 차원이동
		MAGIC_DETECT = 75,             // 마법탐지
		REMOVE_CURSE = 78,             // 저주해제
		PROTECT_RANGE_WEAPON = 79,     // 장거리 무기로부터 보호
		PHEONIX = 80,				   // 불새
		METEOR_STRIKE = 81,			   // 운석 // 030509 kyo //030506이전 110
		LOST_SIGHT = 82,               // 시력상실   
		SPEED_UP = 83,                 // 속도증가
		GREAT_THUNDER_BOLT = 84,       // 대전격
		GREAT_METEOR = 85,             // 대운석 
		DRAGON_SCALE = 86,             // 드래곤의 비늘
		DEATH_FIRE = 87,               // 데쓰파이어
		SUMMONING_SKELETON = 88,       // 데쓰나이트 소환
		FLYING_SPARK = 89,             // 플라잉 스파크
		VAMPIRE_POISON = 90,           // 뱀파이어의 독
		VAMPIRE_DODGE = 91,            // 뱀파이어 회피
		SUMMONING_UNDEAD = 92,         // 언데드 소환
		FEATHER_OF_CURSE = 93,         // 마비의 깃털
		CURSE_OF_HELL = 94,            // 헬의 저주
		RECALL_FOLLOW = 95,            // 부하 소환
		FIRE_EXPLOSION = 96,           // 화염폭발
		FIRE_STORM = 97,               // 파이어스톰
		SHADOW_OF_EVIL = 98,           // 악의 그림자
		FREEZING = 99,                 // 주위 얼림
		BLITZ_SPEAR = 100,             // 번개창
		LIGHTNING_CLOUD = 101,         // 전격 구름
		FIRE_BREATH = 102,             // 화염 브레쓰
		ICE_BREATH = 103,              // 얼음 브레쓰
		LIGHTNING_BREATH = 104,        // 전격 브레쓰
		FLAME_PILLAR = 105,            // 화염 기둥
		FLAME_POUR = 106,              // 화염 폭풍 
		OCTA_LIGHTNING = 107,          // 방사 전격
		BLOOD_SPRINKLE = 108,          // 피뿌리기
		GAZER = 109,				   // 게이저
		FLAME_ARROW = 110,             // 화염화살 

		AREA_BAT_ATTACK = 111,         // 지역 박쥐공격
		AREA_PARALYSIS = 112,          // 지역 마비
		AREA_POISON = 113,             // 지역 중독
		AREA_CONFUSION = 114,          // 지역 혼란
		AREA_SLOW = 115,               // 지역 속도저하

		RESTORE_AUTHORITY = 151,       // 회복의권능  
		PROTECT_DANGER = 152,          // 위험으로부터 보호
		BLESS = 153,                   // 신의 축복
		RECOVER_POISON = 154,          // 해독의 권능
		CURE_DISEASE = 155,            // 질병치료의 권능
		MIRACLE_RECOVER = 156,     	   // 완치의 기적
		PERFECT_PROTECT = 157,         // 완전한 결계  
		GREAT_SHIELD = 158,            // 보호의 기적
		CANCEL_DIVINE = 159,           // 반신력 무효화
		RECURRENCE_DIVINE = 160,       // 회귀의 권능
		PRAY_POWER = 161,              // 기도의 역사
		HOLY_LIGHT = 162,              // 성스러운 빛  
		ANTHEM = 163,                  // 성가
		RELIEF_AUTHORITY = 164,        // 구원의 빛 
		HOLY_FIRE = 165,               // 성스러운 불
		GREAT_PRAY = 166,              // 위대한 기도  
		APPRECIATE = 167,              // 바른 깨달음
		INFLUENCE_DIVINE = 168,        // 신에의 감화
		VOICE_SOUL = 169,              // 영혼의 소리
		HOLY_RAGE = 170,               // 성스런 분노
		TURN_UNDEAD = 171,             // 턴 언데드
		CURSE = 172,                   // 신의 저주 
		AUTHORITY_RECOVER_SIGHT = 173, // 시력회복의 권능	
		DIVINE_POWER = 174,            // 디바인 파워
		AUTHORITY_LUCK = 175,          // 절대 행운의 권능	
		AUTHORITY_LOST_SIGHT = 177,    // 시력상실의 권능
		PRAY = 178,	                   // 기도
		HOLY_CURE = 179,               // 신성한 치유
		AUTHORITY_SIMUNIAN = 181,      // 시무니안의 권능
		AUTHORITY_CHARISNUMEN = 182,   // 카리스 누멘의 권능
		AUTHORITY_LETHY = 183,         // 레티의 권능
		AUTHORITY_OF_ATHYAS	= 185,     // 아샤스의 권능
		AUTHORITY_OF_WHOARENCHA	= 187, // 화렌차의 권능
		RAGE_STONE = 189,              // 대지의 분노
		LIFE_DRAIN = 190,              // 생명력 탈취  
		RAGE_BLAST = 191,              // 화신의 분노
		RAGE_LIGHT_BALL = 192,         // 성령의 분노
		THROW_ARROW = 194,             // 화살
		THROW_KNIFE = 195,             // 비도
		// 번개 속성 전투스킬
		LIGHTNING_BOOM = 215,          
		THUNDER_BLOW = 216,
		LIGHTNING_SHOCK = 217,
		THUNDER_STRIKE = 218,
		LIGHTNING_SHIELD = 219,
		LIGHTNING_EXTREME = 220,
		// 대지 속성 전투스킬
		BLOOD_EARTH = 221,             
		GROUND_ATTACK = 222, 
		BLOOD_WILL = 223,
		STONE_ARMOR = 224,
		SIMUNIAN_BOSOM = 225,
		EARTH_EXTREME = 226,
		// 화염 속성 전투스킬 
		SWORD_N_ROSES = 227,
		MULTIPLE_FIRE = 228, 
		FLEET_MISSILE = 229,
		CONVERTING_ARMOR = 230, 
		BURNING_BLOODY = 231,
		FIRE_EXTREME = 232,
		// 어둠 속성 전투스킬
		POISONING_NOVA = 233,
		CHERROY_SHADE = 234, 
		DARK_BLADE = 235, 
		TYBERN_GIFT = 236,
		ACUQUIRE_DARK = 237,
		DARK_EXTREME = 238,
		// 얼음 속성 전투스킬
		ICING_BLAST = 239,
		CRITICAL_HIDING = 240, 
		SHARK_MISSILE = 241, 
		HORN_OF_ICEBERG = 242, 
		ICE_SHIELD = 243,
		ICE_EXTREME = 244,
		// 바람 속성 전투스킬
		CHARGING = 245,
		DOUBLE_ATTACK = 246, 
		WHILWIND = 247,
		TWISTER = 248, 
		GUST = 249, 
		WIND_EXTREME = 250,
		// 이벤트
		THROW_SNOW_BALL = 193,
		THROW_WATER_BALL = 661,
		THROW_BIG_GULP = 663,
		// 아이템 이펙트 효과
		THROW_FIRE_ARROW = 665,
		THROW_ICE_KNIFE = 666,
		// 아이템의 마법 효과
		ITEM_EFFECT_FIRE = 605,
		ITEM_EFFECT_ICE = 606,
		ITEM_EFFECT_LIGHTNING = 607
	};	//> CSD-031014
};
//
///////////////////////////////////////////////////////////////////////////////
struct USEEFFECTLIST;
struct MAGICLIST;
///////////////////////////////////////////////////////////////////////////////
// 전투 관련 클래스
class CBattle  
{
public:
	CBattle();
	virtual ~CBattle();
	
public:
	virtual int Action(LPCHARACTER pCaster) = 0;
	virtual void Effect(MAGICLIST* pEffect) = 0;
	
public:
	void EffectLocal(MAGICLIST* pMagic, DWORD dwNow);
	void EffectRound(MAGICLIST* pMagic, DWORD dwNow);
	void EffectStorm(MAGICLIST* pMagic, DWORD dwNow);
	void EffectSpread(MAGICLIST* pMagic);
	void EffectShoot(MAGICLIST* pMagic, int nCount);
	void EffectMulti(MAGICLIST* pMagic, int nCount);
	void EffectBreath(MAGICLIST* pMagic, int nX, int nY);
	void EffectPillar(MAGICLIST* pMagic, DWORD dwNow, const int nWidth=100, bool IsCkCollision=false); // 030519 kyo
	void EffectQuake(MAGICLIST* pMagic, int nCount, DWORD dwNow);
	void EffectDrop(MAGICLIST* pMagic, int nCount, DWORD dwNow);
	void EffectSpreadIrregular(MAGICLIST* pMagic, DWORD dwNow, const int nTile, const int nRan, int nLoop=2); //	030328 kyo
	
protected:
	/////////////////////////////////////////////////////////////////////////////
	// 이펙트 정보
	struct EFFECT_INFO
	{
		LPCHARACTER pCaster; // 시전자
		LPCHARACTER pTarget; // 대상자
		int         nType;   // 마법이나 스킬번호
		int         nEffect; // 이펙트 번호 
		int         nSub;    // 서브타입
		int         nDir;    // 방향
		int         nSrcX;   // 시작 위치
		int         nSrcY;   
		int         nDstX;   // 대상 위치
		int         nDstY;
	};
	//
	/////////////////////////////////////////////////////////////////////////////
	void AddEffect(EFFECT_INFO* pEffect);
	void ClearCrash();
	
	protected:
		void Coordinates(double dAngle, int& rX, int& rY);
};

extern int GetInt4Test( const char* szApp, const char* szKey, const char* szFile); // 030513 kyo //for test
extern void GetString4Test( char* szString,  const char* szApp, const char* szKey, const char* szFile); // 030513 kyo //for test

//
///////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_BATTLE_H__01911661_5F5A_426F_8E7B_2C16589738DD__INCLUDED_)
