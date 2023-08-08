#ifndef	__EFFECT_H__
#define	__EFFECT_H__

#include "ddraw.h"
//#include "hong_sprite.h"


#define CHECKSUM_ERROR_EFFECTTABLE		20001				//����Ʈ ���̺� �ε� ����
#define CHECKSUM_ERROR_MAGICTABLE		20002				//���� ���̺� �ε� ����
#define CHECKSUM_ERROR_CONDITIONTABLE	20003				//����� ���̺� �ε� ����
#define CHECKSUM_ERROR_MOVEPATTERN		20004				//NPC�� �̵����� ���̺� �ε� ����
#define CHECKSUM_ERROR_ANIMATIONDATA	20005
#define CHECKSUM_ERROR_ATTACKRANGE		20006
#define CHECKSUM_ERROR_ACCELATOR_USER	20007//010605 lms


#define MANDEAD1	220 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 66	
#define MANDEAD2    221 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 67
#define	MANDEAD3    222 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 68
#define MONSTERDEAD 223 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 69
#define SLIMEDEAD	224 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 70
#define ANTDEAD		225 // finito 24/05/07 changed to new sprite_Num. Old sprite_Num: 71

#define MAX_EFFECT_COUNT	700	//�ִ� ��������Ʈ ���� ����
#define MAX_EFFECT_FRAME	100	//���� ��������Ʈ�� �ִ� �����Ӽ�
#define BLOOD_EFFECT		  0
#define METAL_EFFECT		  1
#define MAX_NPC				    100		//�ִ� NPC�� 

//////////////////// 0126 lkh ////////////////////////
#define HOUR_SPRITE		129
#define MINUTE_SPRITE	130

#define CLOCK_X			8800 //8768	//8869
#define CLOCK_Y			3260
#define CLOCK_HEIGHT	(155 + (3260 - 3180) )

// ����&ȿ�� ���� ��ȣ ����
#define EFFECT_TRAP_EXPOSE1	125 //122	//283
#define EFFECT_TRAP_EXPOSE2 126	//123	//284
#define EFFECT_TRAP_EXPOSE3	127 //124	//285

enum EFFECT_TYPE
{	//< CSD-TW-030606
	STRAIGHT_FORWARD = 1,  // ȭ������ ���� ���ִ� �������� ȿ���� �߻�
	PROTECT_WALL = 2,	   // ���ܺ�(ȭ����/������)
	CHANGE_COLOE = 3,	   // ĳ���� �� ����(RGBBLEND)
	FIX_EFFECT = 4,	       // Ư�� ��ġ�� ���� ȿ�� ���
	CIRCLE_FORWARD = 5,	   // ���� ���� ����(�����Ǻ�/���/����������)
	CIRCLE_RING = 6,	   // ��ȣ��(2���� Ÿ���� �̵�����)
	SHOOT_16 = 7,	       // 16���� ����ź(�̾��� ��������)
	SHOOT_8 = 8,	       // 8���� ����ź
	ROUND_FIRE = 9,	       // ���� �ֵ��� ȭ����
	STORM = 10,            // Ư���������� �ٸ� Ư���������� ���̰� ������ ������(����)
	CASTING_COMBAT = 11,   // ������ų ����
	POINT_TO_POINT = 12,   // Ư���������� �ٸ� Ư���������� �ܼ� �̵�
	RAINY_OBJECT = 14,     // �������� �ټ� ȿ�� �������� ȿ��
	MOVE_WALL = 15,        // �̵� �庮 ����(�̵�ȭ����/�̵�������)
	CASTING_MAGIC = 16,    // ĳ���� ����
	CASTING_MAINTAIN = 17, // ĳ���� ���� ����
	FISH_JUMP = 18,        // ����� ƨ�ܿ�����
	HOMING = 19,           // ����ź
	SKILL_RESULT = 20,     // ��� ���� ����
	NOVA = 21,             // Nova ����
	TRACE = 22,            // �ܻ����ȿ��
	PARTICLE = 23,         // ���� �������� �������� ������ ����
	TYPE4_LIKE = 24,       // 4�� Ÿ�԰� ������� hp <= 0 �̾ ��µȴ�.  
	LOCAL_CLASH = 25       // ���� �浹
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
	short int	end_Time;			//��������� �� �����Ӽ�
	short int	light;				//����ȿ�� ������?(0->���� ����/100�ڸ���ġ->0�̸� ������ ���/1�̸� ���̰� ���ؼ� ���(1~20 ������ ���� �ܰ�)
	short int	sound_Volume;		//����Ʈ�� ����� ������ ��������	
	char		static_Effect;		//��������Ʈ�� ��Ÿƽ ����
	short int	sprite_Num;			//��������Ʈ�� ���� ��ȣ
	short int	pattern_Num;		//����(�Լ�)�� ��ȣ	
	char		send_result;		//ȿ���� ������ ����� ��û�ϴ��� ����
	short int	spr_Height;			//����Ʈ�� ����
	short int	sound_Num;			//����Ʈ�� ����� ������ ������ȣ
	short int	spr_Speed;			//����Ʈ�� �̵�ġ
	short int	impect_Effect_Num;	//Ÿ�ݽ� ����Ǵ� ����Ʈ�� ��ȣ
	short int	sort_Type;			//ȭ�� sorting ���(9(9+0)->�Ϲ��� ����/10(9+1)->������ �ֻ�/11(9+2)->������ ����/12(9+3)->���ϸ��̼� ȭ�� �����Ͽ� ��������/13(9+4)->������ �յڿ� 2���� ��������Ʈ ����)
	short int	trans_Level;		//Ʈ���� ����
	short int	effect_Num;			//����Ʈ�� ����(�Ϸ�) ��ȣ
	short int	object_Num;			//����Ʈ �ҽ��� ������� ����
	short int	link_Frame;			//���� ����Ʈ�� ���۵� ������
	char		fallow;				//�ι��� ������->1/����->0
	short int	next_Effect;		//���� ����Ʈ ��ȣ
	short int	sound_loop;			//���� ��� Ƚ��
	short int	trans_Type;			//Ʈ���� ���
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
	int	order_Type;			     // �迭(1->��/2->��/3->�Ϲ�/4->ȭ��/5->����/6->����/7->��ȯ/8->�ð�/9->�ż�/10->����/11->����/12->Ư��)
	int	serial_Num;			     // ������ȣ(1~)
	int	basic_Level;		     // ��� �⺻ ����(1~9)
	int	WS;					         // wizwrd spell(or priest spell) point 
	int	before_Magic;		     // �� �ܰ� ����(���� ������ �� ����)
	int	basic_Magery ;		   // �⺻ ������
	int	cast_Time;			     // ĳ���� �ð�(����ð�)
	int	cast_Continue;		   // ĳ���� ���� �ð�(����ð�)
	int	continue_Time;		   // ���ӽð�(0->����,������/1~����ð�)	
	int	avail_Type;			     // ȿ�� Ÿ��(0->�ڽ�/1->�ڽ� �ƴ� 1��/2~->��ȿ������ �ټ�)
	int	avail_Range;		     // �����Ÿ�(Ÿ�ϴ���/0->�ڽ�/99->����)		
	int	exhaust_MP;			     // �Ҹ� MP
	int	exhaust_WS;			     // �Ҹ� WS
	int	require_WP;			     // �䱸 ����					
	int	require_IT;			     // �䱸 ����
	int	point_WS;			       // WS ����Ʈ(0->not available/1~)
	int invalid_TargetIT;    // ���� ��밡�� ����
	int	spell_Type ;		     // 0 Wizard, 1~13 priest(God�� type�� ������ �پ�)
	int	appoint_Type;		     // ��� ������� Ÿ��(0->���ڽ�/1->�������(����/�������)/2->Ư������(Ŭ����ġ)/3->��������)
	int	excute_Type;		     // ���� ĳ���� ������ ����ɼ� �ִ� Ƚ��(0->������/1->�ѹ���)
	int	magic_Type;			     // ���� ��з�
	int	Price;				       // ������ ���µ� �ʿ��� ���
	int	Exp;				         // �Ϲݸ����� ��� ������
	int	Learnable;	         // �� �ź����� Ÿ�Կ� ���� ���� �ִ� ���
	int	nSummonMax;   // CSD-030314 : ��ȯ������ ��ȯ���� �ִ� ������ ��
	int nCombatCount; // CSD-030314 : ��밡���� ������ų ��
	int nResist;             // ���� �� �ִ� ���׷�
	int nRiseFall;           // ������ ���� ����
	int nDualStep;			//����� 	// 030415 kyo
};

extern TFileMgr<CMagicTbl, MAX_MAGIC> g_lpMT;
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
struct USEEFFECTLIST // �̵��ϴ� ��������Ʈ ���� ��� ���
{
	int	sprite_Num;		// ��������Ʈ ��ȣ
	int	FrameCount;		// ��������
	int	total_Frame;	// ������� ���� �����Ӽ�
	int	effect_Count;	// ���� ������ ���� ����Ʈ �� ������ȣ
	int	RepeatCount;	// �ݺ���
	int	x;				    // ó����ġ(���ΰ� ù ��ġ)
	int	y;
	int	s_X;			     // ���ΰ��� �ƴ� �ٸ� ��ġ���� �߻�� ��
	int	s_Y;
	float oldincx;
	float	oldincy;
	float	incx;			   // ������
	float	incy;
	int	dir;			     // 8���� (������ : 1, �ð����, 0�ϰ�� ��ǥ��ǥ ����)
	float trans_Level; // ����ó�� ����(1~32)
	int	tarx;			     // ��ǥ��ǥ
	int	tary;
	int	t_X;			     // ��ǥ�ι��� �ƴ� �ٸ� ��ġ�� ���� �� 
	int	t_Y;
	float height;			 // ���̰�
	int	speed;			   // �̵���
	float first_Angle; // �߻� ����
	int	flag;			     // �Ҹ� ����(0->��/1->��)
	int	StartFlag;		 // �Ҹ� ����(0->��/1->��)
	int	sort_Type;		 // ��������Ʈ ���� ���(0->�Ϲ� ����/1->�ֻ���/2->������)
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
	int	magicSerial;		   //���� ���� ��ȣ
	int	magicNum;			     // ���� ��ȣ
	int	magicSubNum;		   // ���� ���� ��ȣ
	int	PCount;				     // ��������count
	int	sprite_Num;			   // ��������Ʈ �ѹ�
	int	spr_Speed;			   // ����Ʈ�� �̵� �ӵ�
	int	spr_Height;			   // ����Ʈ�� ����
	int	target_Height;		 // ��ǥ�� ���� 	
	int	EffectCount;		   // ����Ǵ� ���� ����Ʈ �� 
	int	x;					       // ���ΰ��� ��ǥ X & ������ǥ
	int	y;					       // ���ΰ��� ��ǥ Y
	int	s_X;        			 // ������ǥ
	int	s_Y;
	int	t_X;				       // ��ǥ��ǥ
	int	t_Y;				       
	int	tarx;				       // ��ǥ ��ǥ X
	int	tary;				       // ��ǥ ��ǥ Y
	float	first_incx;		   // ���� �߻�� X�� ����ġ
	float	first_incy;		   // ���� �߻�� Y�� ����ġ
	float	first_height;	   // ���� �߻�� ���̰� ����ġ
	POINT map_Offset;		   // ���� �ɼ���ǥ
	int	dir;				       // ���ΰ��� ���� ���� ����
	int	trans_Type;			   // ���� ó�� ���
	int	trans_Level;		   // ���� �ܰ�
	int	body_Part;			   // ���õ� ���ΰ� ��ü �κ�
	int object_Num;			   // ���ÿ� �����Ǵ� �ִ� ����Ʈ ����
	int fallow;				   // ���ΰ� ������ ����(1->����)
	bool firePattern;		   // �����ұ�?:0 /�����ұ�?:1
	int	next_Effect;		   // ���� ����Ʈ�� ��ȣ
	int	link_Frame;			   // ���� ȿ���� ���۵Ǵ� ������
	int	loop_Count;			   // �ش� ��������Ʈ�� ���� count
	int	sound_LoopCount;   // ������ ���� ī��Ʈ
	int	end_Time;			     // ���Ḧ ���� ���� Ƚ��
	int impect_Effect_Num; // �¾����� ���ü Ÿ�� ȿ�� ����Ʈ ��ȣ
	bool flag;				     // ����� ����ġ ����
	int next_Flag;			   // CSD-021128 : ���� �������� �ǳʰ��� ���� ����ġ
	DWORD	magic_Runtime;	 // ������ �ߵ��Ǿ� ����� �ð�
	bool failed;				   // ������ ������ ǥ���� ����
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
	short	SprNo;			 // ��������Ʈ�� ��ȣ
	short	RepeatFrame; // �ݺ��ؼ� ���� Ƚ�� (default : 1)
	short	x;					 // �̵��� (������ ������� Data���� 
	short	y;					 // �̵����� �� ��� ���)
};
//> CSD-030123
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//< CSD-030123

struct EffectListTblBasic
{
	int	    offset_X;		// X���� ������ǥ
	int	    offset_Y;		// Y���� ������ǥ
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
	
	int	    offset_X;		// X���� ������ǥ
	int	    offset_Y;		// Y���� ������ǥ
	int	    MaxFrame;    
	EFFECT  EffData[200];
};
//> CSD-030123
///////////////////////////////////////////////////////////////////////////////

//#define Num_Of_Magic 142 // ���� �о��(DB�� �ִ�) ������ ������ 
//////////////////// SoundUp lkh �߰� /////////////////////
typedef struct tagBGESOUND
{
	int		mapNum;
	int		dayPattern;			//�����ȿ������ ����ϴ� ����(0�̻��̸� Ư�� ���Ͽ� ����)
	int		day_Slot[10];		//��ȿ���� ��ȣ
	int		day_Rate[10];		//ȿ���� ��� ��
	int		day_Type[10];		//�������
	int		use_DayNum;			//���� �÷��̵� ȿ����(����)��
	
	int		nightPattern;		//����ȿ������ ����ϴ� ����(0�̻��̸� Ư�� ���Ͽ� ����)
	int		night_Slot[10];		//��ȿ���� ��ȣ
	int		night_Rate[10];		//ȿ���� ��� ��
	int		night_Type[10];		//�������
	int		use_NightNum;		//���� �÷��̵� ȿ����(����)��
}	BGESOUND;


typedef struct	tagEFFECT_TABLE
{
	int				effect_Num;			//����Ʈ�� ����(�Ϸ�) ��ȣ
	
	int				pattern_Num;		//����(�Լ�)�� ��ȣ	
	int				sprite_Num;			//��������Ʈ�� ���� ��ȣ
	BOOL			static_Effect;		//��������Ʈ�� ��Ÿƽ ����
	char			sprite_Name[30];	//��������Ʈ�� �̸�
	int				sound_Num;			//����Ʈ�� ����� ������ ������ȣ
	int				sound_Volume;		//����Ʈ�� ����� ������ ��������
	//				1000���� �̻� ����->�ݺ�Ƚ��(�� 99�� ��� ����ñ���) / 100�������� ���� ���� ��ȣ 
	int				sound_loop;			//���� ��� Ƚ��
	
	int				next_Effect;		//���� ����Ʈ ��ȣ
	int				link_Frame;			//���� ����Ʈ�� ���۵� ������
	int				trans_Type;			//Ʈ���� ���
	int				trans_Level;		//Ʈ���� ����
	int				sort_Type;			//ȭ�� sorting ���(9(9+0)->�Ϲ��� ����/10(9+1)->������ �ֻ�/11(9+2)->������ ����/12(9+3)->���ϸ��̼� ȭ�� �����Ͽ� ��������/13(9+4)->������ �յڿ� 2���� ��������Ʈ ����)
	int				spr_Speed;			//����Ʈ�� �̵�ġ
	int				spr_Height;			//����Ʈ�� ����
	//		int				body_Part;			//�������� �� 6�κп� ���� Ȱ������ ��
	int				object_Num;			//����Ʈ �ҽ��� ������� ����
	int			fallow;				//�ι��� ������->1/����->0
	int				light;				//����ȿ�� ������?(0->���� ����/100�ڸ���ġ->0�̸� ������ ���/1�̸� ���̰� ���ؼ� ���(1~20 ������ ���� �ܰ�)
	int				end_Time;			//��������� �� �����Ӽ�
	int				impect_Effect_Num;	//Ÿ�ݽ� ����Ǵ� ����Ʈ�� ��ȣ
	BOOL			send_result;		//ȿ���� ������ ����� ��û�ϴ��� ����
}	EFFECT_TABLE;

typedef struct tagEFFSPRITE
{ 
	short		Count;									// ��������Ʈ�� ����
	Spr			EffSpr[MAX_EFFECT_FRAME];				// ��������Ʈ�� Data
	
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

///////////////////////// SoundUp lkh �߰� ////////////////////////
extern BGESOUND						g_BGES[MAX_MAP_];

//extern bool	ready_Magic;		///////��������-> ���� ����� �غ� ���� �˸��� �÷��� /////// 
extern int							g_SkillNum;
extern EFFSPRITE					Effspr[MAX_EFFECT_COUNT];
//extern EFFECTLIST					EffList[MAX_EFFECT_COUNT];
extern int							effSpr_Count[MAX_EFFECT_COUNT];
extern int							g_Condition_Table[8][8];
extern DWORD						g_LoadSpriteTime[MAX_EFFECT_COUNT];
extern DWORD						g_FireTime;
//extern BATTLERESULTSAVE				BattleResultSave;

///////////////////////// 0816 lkh �߰� /////////////////////////////
extern void	SendHexaEditing(short int type, short int id);

//////////////////////////////// SoundUp lkh �߰� ///////////////////////////////
extern void		StopBGE(void);												//BGE �Ҹ� ����
extern int		ReturnRoonWord(LPCHARACTER caster, int magic_Num);			//�õ� ���(�������-�������о���)
extern int		ReturnCastingWord(LPCHARACTER caster, int magic_Num);		//�õ���casting(900->�����糲��/901->�����翩��/902->�����ڳ���/903->�����ڿ���)
extern int		ReturnStartingWord(LPCHARACTER caster, int magic_Num);		//������starting(904->������1�迭�����......)

//////////////////////////////// SoundUp lkh �߰� ///////////////////////////////
extern BOOL	LoadBGESoundTable(void);
extern void BackEffectSound2(int);

/////////////////////// 0625 lkh �߰� ////////////////////////////
extern void	ReturnDazeMove(int& dest_x, int& dest_y);

//////////////////////////////// �̱��� 1228 //////////////////////////////////////////////
extern void BackEffectSound(int MapNumber);			//���� ȿ���� ���� ���� �Լ�
extern BOOL	CheckCrackRange(LPCHARACTER lpChar);	//���� �������� ���� �ִ� ��� äũ						
extern void ConditionSet(LPCHARACTER ch, BYTE condition, DWORD	Time=0);	//�������� �����Լ�
extern void LoadConditionTable(void);
extern BOOL NPC_MovePattern(LPCHARACTER ch);							//NPC�� ���ɿ� ���� ����ȭ�� �̵����� ó��

extern void RecvCMD_SERVER_MAGICTRAP_EXPOSE( t_server_magictrap_expose *p );
extern void RecvCMD_SERVER_MAGICAL_DETECT( t_server_magical_detect *p);

extern void SendCMD_CHARACTER_DEMAND( int command_type, int target_id);
extern void RecvCMD_ARIGEMENT_RESPONSE(t_server_arigement_response *p );

// 010620 YGI
extern void RecvCMD_CHARACTER_CONDITION(t_server_character_condition *p, int type = 0);
extern void RecvCMD_SERVER_MAGICVIEWTYPE( t_server_magicviewtype *p);
extern void RecvCMD_SERVER_MAGICCOMMAND(t_server_magic_command *p);

extern void RecvCMD_MAGIC_POSITION( t_server_magic_position *p);
extern void EndCastMagic(LPCHARACTER scr/*MAGICLIST* lpML*/);								//ĳ���ý��� �� ���� ȿ���� �����ϰ� ����·� ����
extern void CheckDoingAppointMagic( void );								//�����ҷ����� ������ üũ Ÿ���� ��������(GameProc.cpp ���)
extern void CheckEffectSpriteFree( void );
extern int  CallbackCharacterData(int char_ID, DWORD index/* #define�Ǿ� �ִ� ������ ĳ���� ���� �ε���(����) */);

extern MAGICLIST* SkillResult(MAGICLIST* lpML);							//���ȿ��(����/����)
////////////////////// 0529 lkh ����(������ ����:����� ���ڿ�����׸� �������� �÷���) ////////////////////////
//�߻�� ��ü�� ��λ� ��ֹ��� �ִ� ���� üũ(Ÿ�ϴ���) : ����->Ÿ�� ������ x,y & Ÿ�ϳ��� x,y & ��λ��� �ٸ� �� ���� Ȯ�� ��ƾüũ ����
extern BOOL TileLineCheck(int sx, int sy, int ex, int ey, BOOL check_ShootItem=0);

extern void InsertLightOut(int x, int y, int light, int terminateFrame, int color=0);	//�Ҹ��� ���� ������ �߰��ϴ� �Լ�(color->0:white)
extern MAGICLIST* CastingMagic(MAGICLIST* magic_List);	 // CSD-TW-030606 : ���� ���� ȿ�� ���
extern MAGICLIST* CastingCombat(MAGICLIST* magic_List);	 // CSD-TW-030606 : ������ų ���� ȿ�� ���
extern MAGICLIST* CastingContinueMagic(MAGICLIST*	lpML);				//ĳ���� ���� (�߻���) ȿ�� ��� �Լ�
extern void	ResetCharacter(LPCHARACTER lpChar);
extern void ResetEffect(LPCHARACTER lpChar);
extern bool DoingImpectEffect(int magic_Num, LPCHARACTER scr, LPCHARACTER dcr, int damage);	//������ ���� ����� Ÿ��ó���� ���� ��� �Լ�
extern bool TimeCheck(DWORD end_Time);									//�ð��� üũ�ϴ� �Լ�
extern bool	ReadyMagic(LPCHARACTER	lpRead_Char, int trans_Type=2, int trans_Level=20);					//ĳ������ ������ �����߻� �غ� �Ϸ�� �ι� ó��

extern BOOL LoadEffectSprite( int sprite_Num, char* sprite_Name);		//�ݵ�� ��������Ʈ�� ��ȣ�� �̸��� ��ġ�Ͽ��� ��;
extern BOOL LoadEffectAnimation(int animate_Num, char* animate_Name);
extern bool LoadEffectSpriteDivFile(const int nSprite_Num); // 030821 kyo
extern void FreeEffectSprite( int sprite_Num );

void		RenameSoundFile(void);										//���� ����Ʈ �� ������ ȭ�ϸ��� ��������
void		RenameEffectFile(void);										//�ѱ۸�Ī�� ȭ���� rename�ϱ� ���� ����

///////////////// 0810 lkh �߰� /////////////////
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

// ���� ���� ������ ��� �浹�� ���� �����ϴ� �Լ�
extern LPCHARACTER ReturnMagicCheckedCharacter(int range_Check, MAGICLIST* lpML, int x, int y, int z, int xl, int yl, int my_Check=0, int air_Surface=0, BOOL	curve=0);

extern int EffectOutput(int x, int y, Spr* sp, int type, int alpha=0, int height=0, int sort_Type=9, DWORD rgb=0x00);

/////////////////////// 0126 lkh /////////////////////////////
extern int ClockOutPut(int x, int y, Spr* sp);		//��ǥ -> ������ǥ
extern bool	RandomAttackEffect(int Effect_Type, LPCHARACTER ch, LPCHARACTER tchar);
extern bool	RandomThrowEffect(int nKind, LPCHARACTER ch, LPCHARACTER tchar);
extern bool RbuttonContinue( int magic_Num, LPCHARACTER scr=Hero, LPCHARACTER dcr=Hero, int x=0, int y=0);			//���콺 ������ ��ư�� ��� ������ �ִ� ��� ����Ǵ� ����
// ���� ����Ʈ�� ȿ�� ���� ��� (����:����Ʈ�� ������ȣ/������ǥ X,Y/������ �̹��� ��ȭ��Ű�� �������� ���->(1~6 ������ ���ڷ� 6�ڸ�����) 
extern void InsertMagic(LPCHARACTER lpChar_Own, LPCHARACTER lpChar_Target, int magicNum, int magicSubNum, int s_X, int s_Y, int t_X, int t_Y, bool firePattern=0, int next_Effect=0, bool failed=0, DWORD dwDuration = 0);
// ����Ʈ ����Ʈ�� ���� ����Ʈ ��� (����:����Ʈ�� ������ȣ/��� ī��Ʈ(����/�߰�/����)/���ǵ�/����/Trans���/����/������ ������           
extern void InsertEffect(MAGICLIST *Temp, int start_Frame=0, int nOffX = 0, int nOffY = 0);
// ���� ȿ��1(ȿ�� ��������Ʈ)�� ȭ�鿡 ������ (����:Trnas ���/����/������ ������)
extern void DrawEffect(USEEFFECTLIST *Temp, MAGICLIST* magic_List, int light=0, bool target=0);
// ���� ȿ��2(������ �̹��� ����ȭ-Trans)�� ȭ�鿡 ������(����:Trans ���/��������Ű�����ϴ� ��ü ����(1~6))
extern void DrawEffect1(USEEFFECTLIST *Temp, MAGICLIST* magic_List, int share=0, bool target=0);
// ���� ȿ��3(������ �̹��� ����ȭ-RGB_Blend)�� ȭ�鿡 ������(����:��������Ű�����ϴ� ��ü ����(1~6), �����Ű�����ϴ� RGB��)
extern bool DrawEffect2(USEEFFECTLIST *Temp, MAGICLIST* magic_List, int share, DWORD rgb, bool target=0);
// ���� ȿ�����(�������� �ܻ��� ���)-����Ʈ ��������Ʈ�� �ε����� �ʰ� �� �̹��� �����ϴ� ���
extern void DrawEffect3(USEEFFECTLIST *Temp, MAGICLIST* lpML, bool target);
// ���ִ� �������� ȭ���⵿(1) Ƣ��ö���� �߻� ����(����: ȿ��������ȣ/��ǳѹ�/���ǵ�/����/Trans���/����/������ ������)                              
extern MAGICLIST* SampleMagic1(MAGICLIST *magic_List);//, int Num, int motion_Num, int max_Object, int speed, int height, int type, int level, int end_Frame=-1);
// Ŭ���� ������ ȭ�����(2) Ƣ��ö���� �߻� ����
extern MAGICLIST* SampleMagic1_1(MAGICLIST *magic_List);
// 8���� ���ȿ�� ���� �߻� ����(����:ȿ��������ȣ/��ǳѹ�/���ǵ�/Trans���/����/������ ������)
extern MAGICLIST* SampleMagic2(MAGICLIST *magic_List, int Num, int motion_Num, int speed, int height, int type, int level, int end_Frame=-1);
// �������� �̹��� ���� ������Ű�� ���� ����(����:ȿ�� ������ȣ/������Ű�����ϴ� ��ü����(1~6�� 6�ڸ�))
extern MAGICLIST* SampleMagic3(MAGICLIST *magic_List);//, int Num, int share_Num, int looping=0);
// Ư�� ȿ�� Ư�� ��ġ�� ����ֱ�
extern MAGICLIST* SampleMagic4(MAGICLIST* magic_List);
// ���� �� ����
extern MAGICLIST* SampleMagic5(MAGICLIST* magic_List);//, int Num, int motion_Num, int speed, int height, int way, int type, int level, int end_Frame=-1);
// 2 cercle ring
extern MAGICLIST* SampleMagic6(MAGICLIST* magic_List);//, int Num, int motion_Num, int speed, int height, int type, int level, int end_Frame=-1);		//��������Ʈ�� ������ȣ
// ������ ��ġ���� ��Ȯ�� ��ǥ/�ӵ��� �̵��ϴ� ȭ�� ����(����: ȿ��������ȣ/���ΰ��� �߻�� ����/��ǳѹ�/���ǵ�/����/Trans���/����)                              
extern MAGICLIST* FireArrow(MAGICLIST *magic_List);	//, int Num, int motion_Num, int speed, int height, int type, int level);
// �߻����/�ӵ��� (����/1��) ���ư��� ����
extern MAGICLIST* SampleMagic8(MAGICLIST *magic_List);	
// 8 way ȭ�����
extern MAGICLIST* SampleMagic9(MAGICLIST* magic_List);//, int Num, int motion_Num, int speed, int height, int type, int level, int end_Frame=-1);
// ����ź 
extern MAGICLIST* SampleMagic10(MAGICLIST *magic_List);//, int Num, int motion_Num, int speed, int height, int type, int level, int end_Frame=-1);
// ���� ����
extern MAGICLIST* SampleMagic11(MAGICLIST* magic_List, int Num, int motion_Num, int max_Object, int speed, int height, int type, int level, int end_Frame);		//��������Ʈ�� ������ȣ
// 2 circle wing
extern MAGICLIST* SampleMagic12(MAGICLIST* magic_List);
// ���� �浹 ����
extern MAGICLIST* SampleMagic25(MAGICLIST *magic_List);
// ������ �ܺο��� �����ڿ��Է� ȿ���ҽ� �̵�(1��)
extern MAGICLIST* OutToCharacter(MAGICLIST* magic_List);
// Ư���������� �������� �ҽ� ����
extern MAGICLIST* RainyObject(MAGICLIST* magic_List);
// ���ΰ� ���ִ� �������� �����庮(Wave-�����̴�) �߻�
extern MAGICLIST* Firewall1(MAGICLIST* magic_List);
// Skill_FishJump -> ����� �������� �����Ͽ� �������� ȿ��
extern MAGICLIST* FishJump(MAGICLIST* magic_List);
// �����̻��� 
extern MAGICLIST* Homing(MAGICLIST* lpML);
// ���ܺ� ȿ�� ����
extern MAGICLIST* WallCreate(MAGICLIST* lpML);
// Nova ȿ�� ����
extern MAGICLIST* Nova(MAGICLIST* lpML);
//ĳ���� �� �������� �ܻ�/�̹����� �ܻ� ����
extern MAGICLIST* Trace(MAGICLIST* lpML);
//Ư�� ��ġ�� ���� �������� �����̴� �� ������(1:�Ʒ��� ��������/2:�ð�������� ���/3:�ֺ����� ���� ������)
extern MAGICLIST* RadiantStar(MAGICLIST* lpML);
// SampleMagic4�� ������ hp <= 0 �̾ ����ȴ�. 
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
