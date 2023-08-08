#if !defined(AFX_CHARDATATABLE_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_)
#define AFX_CHARDATATABLE_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define Num_Of_NPC_Lev 256
#define TAC_INDEX 3

struct NPCLev_to_Exp
{	//< CSD-030314
	int nMinExp;         // 현재 레벨의 최소 경험치
	int nMaxExp;         // 현재 레벨의 최대 경험치
	int nNeedExp;        // 다음 레벨로 올라가기 위해 획득해야될 경험치
	int nTrainingExp;    // 택틱 수련장에서 얻는 경험치
	int nExpReduction;	 // 사냥시 죽었을 경우 경험치 삭감률
	int nNKReduction;	 // NK에 죽었을 경우 경험치 삭감률
	int nTacRate;		 // 택틱 보정치
	int nMaxTactic;	     // 레벨에 따른 택틱 수련장에서 올릴 수 있는 최대 택틱 레벨
	int	nDyeingPrice;    // 염색 가격
	int nLessRate;       // 낮은 레벨 보정치  
	int nGreatRate;      // 높은 레벨 보정치  
	int nCspMax;         // 최대 전투스킬 포인터 획득수
	int nBaseCP;         // 전투스킬 기본량
	int bank_loan;	     // 010314 YGI
	int nStep;           // 경험치 단계
	int nWarriorLife;    // 전사의 기본 Life
	int nThiefLife;      // 도둑의 기본 Life
	int nArcherLife;     // 궁수의 기본 Life
	int nWizardLife;     // 마법사의 기본 Life
	int nPriestLife;     // 성직자의 기본 Life
	int nWarriorExpRate; // 전사의 경험치 보정률(단위%)
	int nThiefExpRate;   // 도둑의 경험치 보정률(단위%)
	int nArcherExpRate;  // 궁수의 경험치 보정률(단위%)
	int nWizardExpRate;  // 마법사의 경험치 보정률(단위%)
	int nPriestExpRate;  // 성직자의 경험치 보정률(단위%)
};	//> CSD-030314

extern LPCHARACTER ReturnCharacterPoint(int id);
extern NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev] ;

extern int			ReturnPCAutoWave(LPCHARACTER lpChar, BOOL type);
extern int			ReturnNPCAutoWave(LPCHARACTER lpChar, BOOL type);

extern int			ReturnWaveAnimation( LPCHARACTER lpCharacter );
extern int			ReturnEffectAnimation( LPCHARACTER ch );
extern int			ReturnAttackWaveAnimation( LPCHARACTER lpCharacter );
extern int			ReturnAttackedWaveAnimation( LPCHARACTER lpCharacter );
extern LPCHARACTER  ReturnCharacterID( int id );

extern LPCHARACTER	ReturnCharacterPoint( int server_id );
extern void			ReturnCharMoveDistance( LPCHARACTER ch, XorCrypt *dist );


extern int			IsDead( LPCHARACTER ch );
extern bool IsAttackAble( LPCHARACTER pTarget );


extern void			LoadLevelExpTable( void );

extern void SendSmileFace( int smileno );
extern void RecvSmileFace( int id, int smileno );

extern int	ReturnWaveNatural( LPCHARACTER	ch );

#endif //#if !defined(AFX_CHARDATATABLE_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_)
