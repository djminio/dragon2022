#ifndef	__EFFECT_H__
#define	__EFFECT_H__

#include "ddraw.h"
//#include "hong_sprite.h"


#define CHECKSUM_ERROR_EFFECTTABLE		20001				//이펙트 테이블 로딩 에러
#define CHECKSUM_ERROR_MAGICTABLE		20002				//매직 테이블 로딩 에러
#define CHECKSUM_ERROR_CONDITIONTABLE	20003				//컨디션 테이블 로딩 에러
#define CHECKSUM_ERROR_MOVEPATTERN		20004				//NPC의 이동패턴 테이블 로딩 에러
#define CHECKSUM_ERROR_ANIMATIONDATA	20005
#define CHECKSUM_ERROR_ATTACKRANGE		20006
#define CHECKSUM_ERROR_ACCELATOR_USER	20007//010605 lms


#define MANDEAD1	220 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 66	
#define MANDEAD2    221 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 67
#define	MANDEAD3    222 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 68
#define MONSTERDEAD 223 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 69
#define SLIMEDEAD	224 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 70
#define ANTDEAD		225 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 71

#define MAX_EFFECT_COUNT	700	//최대 스프라이트 종류 갯수
#define MAX_EFFECT_FRAME	100	//단일 스프라이트의 최대 프레임수
#define BLOOD_EFFECT		  0
#define METAL_EFFECT		  1
#define MAX_NPC				    100		//최대 NPC수 

//////////////////// 0126 lkh ////////////////////////
#define HOUR_SPRITE		129
#define MINUTE_SPRITE	130

#define CLOCK_X			8800 //8768	//8869
#define CLOCK_Y			3260
#define CLOCK_HEIGHT	(155 + (3260 - 3180) )

// 마법&효과 고유 번호 정의
#define EFFECT_TRAP_EXPOSE1	125 //122	//283
#define EFFECT_TRAP_EXPOSE2 126	//123	//284
#define EFFECT_TRAP_EXPOSE3	127 //124	//285

enum EFFECT_TYPE
{	//< CSD-TW-030606
	STRAIGHT_FORWARD = 1,  // 화염군과 같은 서있는 방향으로 효과군 발사
	PROTECT_WALL = 2,	   // 차단벽(화염벽/얼음벽)
	CHANGE_COLOE = 3,	   // 캐릭의 색 변형(RGBBLEND)
	FIX_EFFECT = 4,	       // 특정 위치에 고정 효과 출력
	CIRCLE_FORWARD = 5,	   // 원형 공격 패턴(지옥의불/노바/원형얼음벽)
	CIRCLE_RING = 6,	   // 수호성(2개의 타원고리 이동패턴)
	SHOOT_16 = 7,	       // 16방향 직사탄(미약한 유도개념)
	SHOOT_8 = 8,	       // 8방향 직사탄
	ROUND_FIRE = 9,	       // 방향 휘돌이 화염구
	STORM = 10,            // 특정지점에서 다른 특정지점으로 높이값 가지고 떨어짐(유성)
	CASTING_COMBAT = 11,   // 전투스킬 시전
	POINT_TO_POINT = 12,   // 특정지점에서 다른 특정지점으로 단순 이동
	RAINY_OBJECT = 14,     // 복수개의 다수 효과 떨어지는 효과
	MOVE_WALL = 15,        // 이동 장벽 공격(이동화염벽/이동얼음벽)
	CASTING_MAGIC = 16,    // 캐스팅 마법
	CASTING_MAINTAIN = 17, // 캐스팅 유지 마법
	FISH_JUMP = 18,        // 물고기 튕겨오르기
	HOMING = 19,           // 유도탄
	SKILL_RESULT = 20,     // 기술 성공 여부
	NOVA = 21,             // Nova 패턴
	TRACE = 22,            // 잔상공격효과
	PARTICLE = 23,         // 작은 별빛들이 무리지어 찍히는 패턴
	TYPE4_LIKE = 24,       // 4번 타입과 비슷나나 hp <= 0 이어도 출력된다.  
	LOCAL_CLASH = 25       // 지역 충돌
};	//> CSD-TW-030606

#define		DAY_			     0
#define		NIGHT_			   1

#define HIT_FAILED			      0
#define HIT_AND_DEAD		      1
#define HIT_AND_NOTDEAD	      2
#define HIT_AND_ALIVE		      3
#define HIT_GHOSTRESURRECTION 4
#define HIT_SUMMONING	        5

///////////////////////////////////////////////////////////////////////////////
//
class CEffectTbl
{ 
public:
	CEffectTbl()
	{
		effect_Num = 0;
		pattern_Num = 0;
		sprite_Num = 0;
		static_Effect = false;
		sound_Num = 0;
		sound_Volume = 0;
		sound_loop = 0;
		next_Effect = 0;
		link_Frame = 0;
		trans_Type = 0;
		trans_Level = 0;
		sort_Type = 0;
		spr_Speed = 0;
		spr_Height = 0;
		object_Num = 0;
		fallow = false;
		light = 0;
		end_Time = 0;
		impect_Effect_Num = 0;
		send_result = false;
	}
	
public:
	//020808 YGI  ---------------------------
	int Check() const 
	{
		int value;
		char *pValue = (char *)&value;
		pValue[0] = Check1()+Check3();
		pValue[1] = Check2()+Check4();
		pValue[2] = 0;
		pValue[3] = 0;
		return (short int)value;
	}
	char Check1() const
	{
		return (char)(effect_Num+end_Time);
	}
	char Check2() const
	{
		return (char)(sort_Type+next_Effect+sort_Type+object_Num);
	}
	char Check3() const
	{
		return (char)(spr_Speed + spr_Height + object_Num);
	}
	char Check4() const
	{
		return (char)(pattern_Num + sprite_Num + link_Frame);
	}
	
public:
	short int	end_Time;			//종료까지의 총 프레임수
	short int	light;				//광원효과 넣을까?(0->넣지 않음/100자리수치->0이면 정상적 출력/1이면 높이값 더해서 출력(1~20 사이의 광원 단계)
	short int	sound_Volume;		//이펙트와 연결된 사운드의 볼륨레벨	
	char		static_Effect;		//스프라이트의 스타틱 여부
	short int	sprite_Num;			//스프라이트의 고유 번호
	short int	pattern_Num;		//패턴(함수)의 번호	
	char		send_result;		//효과가 서버에 결과를 요청하는지 여부
	short int	spr_Height;			//이팩트의 높이
	short int	sound_Num;			//이펙트와 연결될 사운드의 고유번호
	short int	spr_Speed;			//이팩트의 이동치
	short int	impect_Effect_Num;	//타격시 연결되는 이팩트의 번호
	short int	sort_Type;			//화면 sorting 방법(9(9+0)->일반적 소팅/10(9+1)->무조건 최상/11(9+2)->무조건 최하/12(9+3)->에니메이션 화일 참조하여 가변소팅/13(9+4)->시전자 앞뒤에 2장의 스프라이트 찍음)
	short int	trans_Level;		//트렌스 정도
	short int	effect_Num;			//이팩트의 고유(일련) 번호
	short int	object_Num;			//이팩트 소스의 동시출력 갯수
	short int	link_Frame;			//다음 이펙트가 시작될 프레임
	char		fallow;				//인물을 따라갈지->1/말지->0
	short int	next_Effect;		//다음 이펙트 번호
	short int	sound_loop;			//사운드 출력 횟수
	short int	trans_Type;			//트렌스 방법
};

extern TFileMgr<CEffectTbl, MAX_EFFECT_COUNT> g_lpET;
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CEffectTbl2
{ 
public:
	int Check() const 
	{
		int value = 0;
		for( int i=0; i<30; i++ )
			value += sprite_Name[i];
		
		return value;
	}
public:
	char sprite_Name[30];
};

extern TFileMgr<CEffectTbl2, MAX_EFFECT_COUNT> g_lpET2;	//020808 YGI 
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
class CMagicTbl
{
public:
	CMagicTbl()
	{
		order_Type = 0;
		serial_Num = 0;
		basic_Level = 0;
		WS = 0;
		before_Magic = 0;
		basic_Magery = 0;
		cast_Time = 0;
		cast_Continue = 0;
		continue_Time = 0;
		avail_Type = 0;
		avail_Range = 0;
		exhaust_MP = 0;
		exhaust_WS = 0;
		require_WP = 0;
		require_IT = 0;
		point_WS = 0;
		invalid_TargetIT = 0;
		spell_Type = 0;
		appoint_Type = 0;
		excute_Type = 0;
		magic_Type = 0;
		Price = 0;
		Exp = 0;
		Learnable = 0;
		nSummonMax = 0;
		nCombatCount = 0;
		nResist = 0;
		nRiseFall = 0;
		nDualStep = 0;	// 030415 kyo
	}
	
public:
	int Check() const 
	{
		return serial_Num + basic_Level + cast_Time + cast_Continue + 
			continue_Time + avail_Range + excute_Type + appoint_Type +
			avail_Type + spell_Type + magic_Type + nCombatCount;
	}
	
public:
	int	order_Type;			     // 계열(1->생/2->사/3->일반/4->화염/5->얼음/6->전격/7->소환/8->시공/9->신성/10->정동/11->정념/12->특수)
	int	serial_Num;			     // 고유번호(1~)
	int	basic_Level;		     // 사용 기본 레벨(1~9)
	int	WS;					         // wizwrd spell(or priest spell) point 
	int	before_Magic;		     // 전 단계 마법(먼저 익혀야 할 마법)
	int	basic_Magery ;		   // 기본 마법력
	int	cast_Time;			     // 캐스팅 시간(절대시간)
	int	cast_Continue;		   // 캐스팅 지속 시간(절대시간)
	int	continue_Time;		   // 지속시간(0->순간,영원히/1~절대시간)	
	int	avail_Type;			     // 효용 타입(0->자신/1->자신 아닌 1명/2~->유효범위내 다수)
	int	avail_Range;		     // 사정거리(타일단위/0->자신/99->무한)		
	int	exhaust_MP;			     // 소모 MP
	int	exhaust_WS;			     // 소모 WS
	int	require_WP;			     // 요구 지혜					
	int	require_IT;			     // 요구 지능
	int	point_WS;			       // WS 포인트(0->not available/1~)
	int invalid_TargetIT;    // 마법 사용가능 범위
	int	spell_Type ;		     // 0 Wizard, 1~13 priest(God별 type들 때문에 다양)
	int	appoint_Type;		     // 대상 지정방식 타입(0->나자신/1->대상지정(단일/복수대상)/2->특정지점(클릭위치)/3->지역마법)
	int	excute_Type;		     // 마법 캐스팅 유지시 시행될수 있는 횟수(0->여러번/1->한번만)
	int	magic_Type;			     // 마법 대분류
	int	Price;				       // 마법을 배우는데 필요한 비용
	int	Exp;				         // 일반마법의 경우 경험피
	int	Learnable;	         // 각 신봉신의 타입에 따라 배울수 있는 경우
	int	nSummonMax;   // CSD-030314 : 소환마법시 소환가능 최대 몬스터의 수
	int nCombatCount; // CSD-030314 : 사용가능한 전투스킬 수
	int nResist;             // 막을 수 있는 저항력
	int nRiseFall;           // 높낮이 적용 범위
	int nDualStep;			//듀얼스텝 	// 030415 kyo
};

extern TFileMgr<CMagicTbl, MAX_MAGIC> g_lpMT;
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
struct USEEFFECTLIST // 이동하는 스프라이트 찍을 경우 사용
{
	int	sprite_Num;		// 스프라이트 번호
	int	FrameCount;		// 현프레임
	int	total_Frame;	// 현재까지 누적 프레임수
	int	effect_Count;	// 현재 생성된 동일 이팩트 중 생성번호
	int	RepeatCount;	// 반복값
	int	x;				    // 처음위치(주인공 첫 위치)
	int	y;
	int	s_X;			     // 주인공이 아닌 다른 위치에서 발사될 때
	int	s_Y;
	float oldincx;
	float	oldincy;
	float	incx;			   // 증가값
	float	incy;
	int	dir;			     // 8방향 (오른쪽 : 1, 시계방향, 0일경우 목표좌표 참조)
	float trans_Level; // 투명처리 정도(1~32)
	int	tarx;			     // 목표좌표
	int	tary;
	int	t_X;			     // 목표인물이 아닌 다른 위치로 날라갈 떄 
	int	t_Y;
	float height;			 // 높이값
	int	speed;			   // 이동값
	float first_Angle; // 발사 각도
	int	flag;			     // 소멸 유무(0->생/1->사)
	int	StartFlag;		 // 소멸 유무(0->생/1->사)
	int	sort_Type;		 // 스프라이트 소팅 방법(0->일반 소팅/1->최상위/2->최하위)
	USEEFFECTLIST	*Next;
	USEEFFECTLIST()
	{
		Next = NULL;
	}
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
struct MAGICLIST
{
	int	magicSerial;		   //실제 매직 번호
	int	magicNum;			     // 매직 번호
	int	magicSubNum;		   // 매직 서브 번호
	int	PCount;				     // 매직진행count
	int	sprite_Num;			   // 스프라이트 넘버
	int	spr_Speed;			   // 이펙트의 이동 속도
	int	spr_Height;			   // 이펙트의 높이
	int	target_Height;		 // 목표의 높이 	
	int	EffectCount;		   // 실행되는 동일 이펙트 수 
	int	x;					       // 주인공의 좌표 X & 시작좌표
	int	y;					       // 주인공의 좌표 Y
	int	s_X;        			 // 시작좌표
	int	s_Y;
	int	t_X;				       // 목표좌표
	int	t_Y;				       
	int	tarx;				       // 목표 좌표 X
	int	tary;				       // 목표 좌표 Y
	float	first_incx;		   // 최초 발사시 X축 증감치
	float	first_incy;		   // 최초 발사시 Y축 증감치
	float	first_height;	   // 최초 발사시 높이값 증감치
	POINT map_Offset;		   // 맵의 옵셋좌표
	int	dir;				       // 주인공의 마법 시작 방향
	int	trans_Type;			   // 투명 처리 방법
	int	trans_Level;		   // 투명 단계
	int	body_Part;			   // 관련된 주인공 신체 부분
	int object_Num;			   // 동시에 생성되는 최대 이팩트 갯수
	int fallow;				   // 주인공 따라갈지 여부(1->따라감)
	bool firePattern;		   // 직진할까?:0 /유도할까?:1
	int	next_Effect;		   // 다음 이펙트의 번호
	int	link_Frame;			   // 다음 효과가 시작되는 프레임
	int	loop_Count;			   // 해당 매직리스트의 루핑 count
	int	sound_LoopCount;   // 사운드의 루핑 카운트
	int	end_Time;			     // 종료를 위한 루핑 횟수
	int impect_Effect_Num; // 맞았을시 대상체 타격 효과 이팩트 번호
	bool flag;				     // 예비용 스위치 변수
	int next_Flag;			   // CSD-021128 : 다음 스텝으로 건너가기 위한 스위치
	DWORD	magic_Runtime;	 // 마법이 발동되어 경과한 시간
	bool failed;				   // 실패한 마법의 표현을 위해
	LPCHARACTER lpChar_Own;
	LPCHARACTER lpChar_Target;
	int	Caster_id;
	int	Target_id;
	USEEFFECTLIST* EffHead;
	MAGICLIST* Next;

	MAGICLIST()
	{
		lpChar_Own = NULL;
		lpChar_Target = NULL;
		EffHead = NULL;
		Next = NULL;
	}
};

extern MAGICLIST*	g_lpML;
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//< CSD-030123
struct EFFECT
{ 
	short	SprNo;			 // 스프라이트의 번호
	short	RepeatFrame; // 반복해서 찍을 횟수 (default : 1)
	short	x;					 // 이동값 (엔진과 상관없이 Data에서 
	short	y;					 // 이동값을 줄 경우 사용)
};
//> CSD-030123
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//< CSD-030123

struct EffectListTblBasic
{
	int	    offset_X;		// X축의 수정좌표
	int	    offset_Y;		// Y축의 수정좌표
	int	    MaxFrame;    
	EFFECT  EffData[200];
	
	short int	check_sum1;
	short int	check_sum2;
};

class CEffectListTbl// : public EffectListTblBasic
{
public: 
	CEffectListTbl()
	{
		offset_X = 0;
		offset_Y = 0;
		MaxFrame = 0;
		memset(EffData, 0, sizeof(EFFECT)*200);
	}
	
public:
	void CopyIn( EffectListTblBasic &data )
	{
		offset_X = data.offset_X;
		offset_Y = data.offset_Y;
		MaxFrame = data.MaxFrame;
		memcpy(EffData, data.EffData, sizeof(EFFECT)*200);
	}
	void CopyOut( EffectListTblBasic &data )
	{
		data.offset_X = offset_X;
		data.offset_Y = offset_Y;
		data.MaxFrame = MaxFrame;
		memcpy( data.EffData, EffData, sizeof(EFFECT)*200);
	}
	
	int Check() const
	{
		int nCheck = offset_X + offset_Y + MaxFrame;
		
		for (int i = 0; i < MaxFrame; ++i)
		{
			nCheck = EffData[i].SprNo + EffData[i].RepeatFrame + EffData[i].x + EffData[i].y;
		}
		
		return nCheck;
	}
	
public:
	
	int	    offset_X;		// X축의 수정좌표
	int	    offset_Y;		// Y축의 수정좌표
	int	    MaxFrame;    
	EFFECT  EffData[200];
};
//> CSD-030123
///////////////////////////////////////////////////////////////////////////////

//#define Num_Of_Magic 142 // 현재 읽어올(DB에 있는) 마법의 가지수 
//////////////////// SoundUp lkh 추가 /////////////////////
typedef struct tagBGESOUND
{
	int		mapNum;
	int		dayPattern;			//낮배경효과음을 출력하는 패턴(0이상이면 특정 패턴에 따라)
	int		day_Slot[10];		//낮효과음 번호
	int		day_Rate[10];		//효과음 출력 빈도
	int		day_Type[10];		//출력패턴
	int		use_DayNum;			//실제 플레이될 효과음(슬롯)수
	
	int		nightPattern;		//밤배경효과음을 출력하는 패턴(0이상이면 특정 패턴에 따라)
	int		night_Slot[10];		//밤효과음 번호
	int		night_Rate[10];		//효과음 출력 빈도
	int		night_Type[10];		//출력패턴
	int		use_NightNum;		//실제 플레이될 효과음(슬롯)수
}	BGESOUND;


typedef struct	tagEFFECT_TABLE
{
	int				effect_Num;			//이팩트의 고유(일련) 번호
	
	int				pattern_Num;		//패턴(함수)의 번호	
	int				sprite_Num;			//스프라이트의 고유 번호
	BOOL			static_Effect;		//스프라이트의 스타틱 여부
	char			sprite_Name[30];	//스프라이트의 이름
	int				sound_Num;			//이펙트와 연결될 사운드의 고유번호
	int				sound_Volume;		//이펙트와 연결된 사운드의 볼륨레벨
	//				1000단위 이상 숫자->반복횟수(단 99일 경우 종료시까지) / 100단위까지 실제 사운드 번호 
	int				sound_loop;			//사운드 출력 횟수
	
	int				next_Effect;		//다음 이펙트 번호
	int				link_Frame;			//다음 이펙트가 시작될 프레임
	int				trans_Type;			//트렌스 방법
	int				trans_Level;		//트렌스 정도
	int				sort_Type;			//화면 sorting 방법(9(9+0)->일반적 소팅/10(9+1)->무조건 최상/11(9+2)->무조건 최하/12(9+3)->에니메이션 화일 참조하여 가변소팅/13(9+4)->시전자 앞뒤에 2장의 스프라이트 찍음)
	int				spr_Speed;			//이팩트의 이동치
	int				spr_Height;			//이팩트의 높이
	//		int				body_Part;			//시전자의 몸 6부분에 대한 활성정의 값
	int				object_Num;			//이팩트 소스의 동시출력 갯수
	int			fallow;				//인물을 따라갈지->1/말지->0
	int				light;				//광원효과 넣을까?(0->넣지 않음/100자리수치->0이면 정상적 출력/1이면 높이값 더해서 출력(1~20 사이의 광원 단계)
	int				end_Time;			//종료까지의 총 프레임수
	int				impect_Effect_Num;	//타격시 연결되는 이팩트의 번호
	BOOL			send_result;		//효과가 서버에 결과를 요청하는지 여부
}	EFFECT_TABLE;

typedef struct tagEFFSPRITE
{ 
	short		Count;									// 스프라이트의 갯수
	Spr			EffSpr[MAX_EFFECT_FRAME];				// 스프라이트의 Data
	
} EFFSPRITE;

typedef	struct tagMovePattern
{
	int		pattern[3];
} MovePattern;

typedef struct tagBATTLERESULTSAVE
{
	int		Result;
	char	caster_Name[MAX_PATH];
	int		Level;
	BOOL	MagicType;
	int		Damage;
	int		Change_MP;
	char	target_Name[MAX_PATH];
	int		target_Statue;
}	BATTLERESULTSAVE;

///////////////////////// SoundUp lkh 추가 ////////////////////////
extern BGESOUND						g_BGES[MAX_MAP_];

//extern bool	ready_Magic;		///////전역변수-> 마법 사용할 준비 됨을 알리는 플레그 /////// 
extern int							g_SkillNum;
extern EFFSPRITE					Effspr[MAX_EFFECT_COUNT];
//extern EFFECTLIST					EffList[MAX_EFFECT_COUNT];
extern int							effSpr_Count[MAX_EFFECT_COUNT];
extern int							g_Condition_Table[8][8];
extern DWORD						g_LoadSpriteTime[MAX_EFFECT_COUNT];
extern DWORD						g_FireTime;
//extern BATTLERESULTSAVE				BattleResultSave;

///////////////////////// 0816 lkh 추가 /////////////////////////////
extern void	SendHexaEditing(short int type, short int id);

//////////////////////////////// SoundUp lkh 추가 ///////////////////////////////
extern void		StopBGE(void);												//BGE 소리 끄기
extern int		ReturnRoonWord(LPCHARACTER caster, int magic_Num);			//시동 룬어(랜덤출력-남여구분없다)
extern int		ReturnCastingWord(LPCHARACTER caster, int magic_Num);		//시동어casting(900->마법사남자/901->마법사여자/902->성직자남자/903->성직자여자)
extern int		ReturnStartingWord(LPCHARACTER caster, int magic_Num);		//시전어starting(904->마법사1계열시전어남......)

//////////////////////////////// SoundUp lkh 추가 ///////////////////////////////
extern BOOL	LoadBGESoundTable(void);
extern void BackEffectSound2(int);

/////////////////////// 0625 lkh 추가 ////////////////////////////
extern void	ReturnDazeMove(int& dest_x, int& dest_y);

//////////////////////////////// 이규훈 1228 //////////////////////////////////////////////
extern void BackEffectSound(int MapNumber);			//랜덤 효과음 사운드 연출 함수
extern BOOL	CheckCrackRange(LPCHARACTER lpChar);	//일정 지역내에 적이 있는 경우 채크						
extern void ConditionSet(LPCHARACTER ch, BYTE condition, DWORD	Time=0);	//상태전이 결정함수
extern void LoadConditionTable(void);
extern BOOL NPC_MovePattern(LPCHARACTER ch);							//NPC의 지능에 따른 차별화된 이동패턴 처리

extern void RecvCMD_SERVER_MAGICTRAP_EXPOSE( t_server_magictrap_expose *p );
extern void RecvCMD_SERVER_MAGICAL_DETECT( t_server_magical_detect *p);

extern void SendCMD_CHARACTER_DEMAND( int command_type, int target_id);
extern void RecvCMD_ARIGEMENT_RESPONSE(t_server_arigement_response *p );

// 010620 YGI
extern void RecvCMD_CHARACTER_CONDITION(t_server_character_condition *p, int type = 0);
extern void RecvCMD_SERVER_MAGICVIEWTYPE( t_server_magicviewtype *p);
extern void RecvCMD_SERVER_MAGICCOMMAND(t_server_magic_command *p);

extern void RecvCMD_MAGIC_POSITION( t_server_magic_position *p);
extern void EndCastMagic(LPCHARACTER scr/*MAGICLIST* lpML*/);								//캐스팅시작 및 유지 효과를 종료하고 평상태로 전이
extern void CheckDoingAppointMagic( void );								//구사할려고한 마법이 체크 타입이 무었인지(GameProc.cpp 사용)
extern void CheckEffectSpriteFree( void );
extern int  CallbackCharacterData(int char_ID, DWORD index/* #define되어 있는 서버쪽 캐릭터 정보 인덱스(정수) */);

extern MAGICLIST* SkillResult(MAGICLIST* lpML);							//기술효과(성공/실패)
////////////////////// 0529 lkh 수정(마지막 인자:비검의 상자연사버그를 막기위한 플래그) ////////////////////////
//발사된 객체의 경로상 장애물이 있는 여부 체크(타일단위) : 인자->타일 시작점 x,y & 타일끝점 x,y & 경로상의 다른 적 유무 확인 루틴체크 여부
extern BOOL TileLineCheck(int sx, int sy, int ex, int ey, BOOL check_ShootItem=0);

extern void InsertLightOut(int x, int y, int light, int terminateFrame, int color=0);	//소멸해 가는 광원을 추가하는 함수(color->0:white)
extern MAGICLIST* CastingMagic(MAGICLIST* magic_List);	 // CSD-TW-030606 : 마법 시전 효과 출력
extern MAGICLIST* CastingCombat(MAGICLIST* magic_List);	 // CSD-TW-030606 : 전투스킬 시전 효과 출력
extern MAGICLIST* CastingContinueMagic(MAGICLIST*	lpML);				//캐스팅 유지 (발사대기) 효과 출력 함수
extern void	ResetCharacter(LPCHARACTER lpChar);
extern void ResetEffect(LPCHARACTER lpChar);
extern bool DoingImpectEffect(int magic_Num, LPCHARACTER scr, LPCHARACTER dcr, int damage);	//서버에 의해 날라온 타격처리에 대한 출력 함수
extern bool TimeCheck(DWORD end_Time);									//시간을 체크하는 함수
extern bool	ReadyMagic(LPCHARACTER	lpRead_Char, int trans_Type=2, int trans_Level=20);					//캐스팅이 끝나고 마법발사 준비 완료된 인물 처리

extern BOOL LoadEffectSprite( int sprite_Num, char* sprite_Name);		//반드시 스프라이트의 번호와 이름은 일치하여야 함;
extern BOOL LoadEffectAnimation(int animate_Num, char* animate_Name);
extern bool LoadEffectSpriteDivFile(const int nSprite_Num); // 030821 kyo
extern void FreeEffectSprite( int sprite_Num );

void		RenameSoundFile(void);										//사운드 리스트 및 데이터 화일명을 영문으로
void		RenameEffectFile(void);										//한글명칭인 화일을 rename하기 위한 변수

///////////////// 0810 lkh 추가 /////////////////
void		OrderSoundFile(void);

extern bool LoadEffect(void);
extern bool LoadMagicTable(void);
extern bool LoadMagicTable2(void);

extern BOOL InitEffect();

extern void InitList(void);
extern void DestoryList(void);

extern void DeleteOwnerPoint(LPCHARACTER ch);
extern void EffectProc(void);

extern BOOL PutDeadEffectBottom(LPCHARACTER ch);	//Effspr[66]
extern int	DistanceTile(int srcX, int srcY, int dstX, int dstY);
extern void InsertMagicList(MAGICLIST **Head, MAGICLIST *TempList);
extern void InsertList(USEEFFECTLIST **Head, USEEFFECTLIST *TempList);
extern USEEFFECTLIST *FindList(USEEFFECTLIST *Head, int EffectNum);
extern void DeleteList(USEEFFECTLIST **Head, USEEFFECTLIST *TempList);
extern void DeleteAllList(USEEFFECTLIST **Head);

extern MAGICLIST *FindList(MAGICLIST *Head, int EffectNum);
extern void DeleteList(MAGICLIST **Head, MAGICLIST *TempList);
extern void DeleteAllList(MAGICLIST **Head);
extern void	DrawClock();
extern void DrawDaysofWeek();
extern void	ShowBuffTimer();
extern void	DrawPing();

// 복수 지역 마법인 경우 충돌한 적을 리턴하는 함수
extern LPCHARACTER ReturnMagicCheckedCharacter(int range_Check, MAGICLIST* lpML, int x, int y, int z, int xl, int yl, int my_Check=0, int air_Surface=0, BOOL	curve=0);

extern int EffectOutput(int x, int y, Spr* sp, int type, int alpha=0, int height=0, int sort_Type=9, DWORD rgb=0x00);

/////////////////////// 0126 lkh /////////////////////////////
extern int ClockOutPut(int x, int y, Spr* sp);		//좌표 -> 절대좌표
extern bool	RandomAttackEffect(int Effect_Type, LPCHARACTER ch, LPCHARACTER tchar);
extern bool	RandomThrowEffect(int nKind, LPCHARACTER ch, LPCHARACTER tchar);
extern bool RbuttonContinue( int magic_Num, LPCHARACTER scr=Hero, LPCHARACTER dcr=Hero, int x=0, int y=0);			//마우스 오른쪽 버튼을 계속 누르고 있는 경우 실행되는 마법
// 마법 리스트에 효과 마법 등록 (인자:이펙트의 고유번호/중점좌표 X,Y/시전자 이미지 변화시키는 마법에서 사용->(1~6 사이의 숫자로 6자리까지) 
extern void InsertMagic(LPCHARACTER lpChar_Own, LPCHARACTER lpChar_Target, int magicNum, int magicSubNum, int s_X, int s_Y, int t_X, int t_Y, bool firePattern=0, int next_Effect=0, bool failed=0, DWORD dwDuration = 0);
// 이펙트 리스트에 마법 리스트 등록 (인자:이펙트의 고유번호/모션 카운트(생성/중간/종말)/스피드/방향/Trans방법/투명도/마지막 프레임           
extern void InsertEffect(MAGICLIST *Temp, int start_Frame=0, int nOffX = 0, int nOffY = 0);
// 실제 효과1(효과 스프라이트)를 화면에 보여줌 (인자:Trnas 방법/투명도/마지막 프레임)
extern void DrawEffect(USEEFFECTLIST *Temp, MAGICLIST* magic_List, int light=0, bool target=0);
// 실제 효과2(시전자 이미지 색변화-Trans)를 화면에 보여줌(인자:Trans 방법/색변형시키고자하는 신체 부위(1~6))
extern void DrawEffect1(USEEFFECTLIST *Temp, MAGICLIST* magic_List, int share=0, bool target=0);
// 실제 효과3(시전자 이미지 색변화-RGB_Blend)를 화면에 보여줌(인자:색변형시키고자하는 신체 부위(1~6), 적용시키고자하는 RGB값)
extern bool DrawEffect2(USEEFFECTLIST *Temp, MAGICLIST* magic_List, int share, DWORD rgb, bool target=0);
// 실제 효과출력(시전자의 잔상을 출력)-이펙트 스프라이트를 로딩하지 않고 원 이미지 변형하는 경우
extern void DrawEffect3(USEEFFECTLIST *Temp, MAGICLIST* lpML, bool target);
// 서있는 방향으로 화염기동(1) 튀어올라오며 발사 패턴(인자: 효과고유번호/모션넘버/스피드/방향/Trans방법/투명도/마지막 프레임)                              
extern MAGICLIST* SampleMagic1(MAGICLIST *magic_List);//, int Num, int motion_Num, int max_Object, int speed, int height, int type, int level, int end_Frame=-1);
// 클리한 곳으로 화염기둥(2) 튀어올라오며 발사 패턴
extern MAGICLIST* SampleMagic1_1(MAGICLIST *magic_List);
// 8방향 방사효과 마법 발사 패턴(인자:효과고유번호/모션넘버/스피드/Trans방법/투명도/마지막 프레임)
extern MAGICLIST* SampleMagic2(MAGICLIST *magic_List, int Num, int motion_Num, int speed, int height, int type, int level, int end_Frame=-1);
// 시전자의 이미지 색을 변형시키는 마법 패턴(인자:효과 고유번호/변형시키고자하는 신체부위(1~6로 6자리))
extern MAGICLIST* SampleMagic3(MAGICLIST *magic_List);//, int Num, int share_Num, int looping=0);
// 특정 효과 특정 위치에 찍어주기
extern MAGICLIST* SampleMagic4(MAGICLIST* magic_List);
// 불의 벽 생성
extern MAGICLIST* SampleMagic5(MAGICLIST* magic_List);//, int Num, int motion_Num, int speed, int height, int way, int type, int level, int end_Frame=-1);
// 2 cercle ring
extern MAGICLIST* SampleMagic6(MAGICLIST* magic_List);//, int Num, int motion_Num, int speed, int height, int type, int level, int end_Frame=-1);		//스프라이트의 고유번호
// 시전자 위치에서 정확한 좌표/속도로 이동하는 화살 패턴(인자: 효과고유번호/주인공의 발사시 방향/모션넘버/스피드/높이/Trans방법/투명도)                              
extern MAGICLIST* FireArrow(MAGICLIST *magic_List);	//, int Num, int motion_Num, int speed, int height, int type, int level);
// 발사방향/속도로 (복수/1개) 날아가는 마법
extern MAGICLIST* SampleMagic8(MAGICLIST *magic_List);	
// 8 way 화염기둥
extern MAGICLIST* SampleMagic9(MAGICLIST* magic_List);//, int Num, int motion_Num, int speed, int height, int type, int level, int end_Frame=-1);
// 유성탄 
extern MAGICLIST* SampleMagic10(MAGICLIST *magic_List);//, int Num, int motion_Num, int speed, int height, int type, int level, int end_Frame=-1);
// 얼음 마법
extern MAGICLIST* SampleMagic11(MAGICLIST* magic_List, int Num, int motion_Num, int max_Object, int speed, int height, int type, int level, int end_Frame);		//스프라이트의 고유번호
// 2 circle wing
extern MAGICLIST* SampleMagic12(MAGICLIST* magic_List);
// 지역 충돌 마법
extern MAGICLIST* SampleMagic25(MAGICLIST *magic_List);
// 시전자 외부에서 시전자에게로 효과소스 이동(1개)
extern MAGICLIST* OutToCharacter(MAGICLIST* magic_List);
// 특정지역에서 복수개의 소스 내림
extern MAGICLIST* RainyObject(MAGICLIST* magic_List);
// 주인공 서있는 방향으로 불의장벽(Wave-움직이는) 발사
extern MAGICLIST* Firewall1(MAGICLIST* magic_List);
// Skill_FishJump -> 물고기 수면위로 점프하여 떨어지는 효과
extern MAGICLIST* FishJump(MAGICLIST* magic_List);
// 유도미사일 
extern MAGICLIST* Homing(MAGICLIST* lpML);
// 차단벽 효과 마법
extern MAGICLIST* WallCreate(MAGICLIST* lpML);
// Nova 효과 마법
extern MAGICLIST* Nova(MAGICLIST* lpML);
//캐릭터 및 아이템의 잔상/이미지의 잔상 마법
extern MAGICLIST* Trace(MAGICLIST* lpML);
//특정 위치에 일정 패턴으로 움직이는 빛 조각들(1:아래로 떨어지는/2:시계방향으로 원운동/3:주변으로 퍼져 나가는)
extern MAGICLIST* RadiantStar(MAGICLIST* lpML);
// SampleMagic4와 같으나 hp <= 0 이어도 실행된다. 
extern MAGICLIST* SampleMagic_Type4_Like(MAGICLIST*	lpML); 
extern void Log_BattleSave( char* filename, bool attack_Type, BATTLERESULTSAVE BR_Save);
extern void TransAfToAb();

MAGICLIST* FindList(MAGICLIST *Head, int MagicNum, int idTarget);
bool IsApplyPK(int nMagic);
bool IsHarmfulMagic(int nMagic);
bool IsPointAttackMagic(int nMagic);
int GetRiseFall(int nX1, int nY1, int nX2, int nY2);
bool IsCollision(int nMagic, int nX1, int nY1, int nX2, int nY2);
bool IsFuntionItemEffect(int nEffect); // CSD-030422

void PlayThunder(); //Eleval 12/08/09 - Thunderstorm
void PlayRainSound(); //Eleval 12/08/09 - Thunderstorm
#endif	// __EFFECT_H__
