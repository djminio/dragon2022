#ifndef _HORSE_H_
#define _HORSE_H_

#include <List>

using namespace std;

#define	HSEX_TYPE_MAX			2
#define	HRIDER_TYPE_MAX			3
#define	HHORSE_TYPE_MAX			3
#define HWEAPON_TYPE_MAX		19//26
#define HGUARD_TYPE_MAX			7
#define	HGRADE_MAX				2
#define	HRIDER_GRADE_MAX		3
#define	HHORSE_GRADE_MAX		2



#define HTYPE_WOMAN				0
#define HTYPE_MAN				1



#define	HSRIDER					0
#define	HSHORSE					1
#define	HSWEAPON				2
#define HSSHEILD				3
#define HSRBAND					4
#define HSSADDLE				5

#define HHORSE					0
#define HCOW					1
#define HPENTHOM				2

									// Weapon Sprite File Number
#define HONEHAND_SWORD1			0	// 0
#define HONEHAND_SWORD2			1	// 0
#define HONEHAND_FIRE_SWORD		2	// 1
#define	HONEHAND_ICE_SWORD		3	// 2
#define HONEHAND_ELECT_SWORD	4	// 3
#define HONEHAND_AXE1			5	// 4
#define HONEHAND_AXE2			6	// 4
#define HTWOHAND_AXE1			7	// 5
#define HTWOHAND_AXE2			8	// 5
#define HMACE					9	// 6
#define HTWOHAND_HAMMER			10	// 7
#define	HWHIRL1					11	// 8
#define HWHIRL2					12	// 8
#define HLENCE1					13	// 9
#define HLENCE2					14	// 9
#define HWAND1					15	// 10
#define HWAND2					16	// 10
#define	HKNIFE1					17	// 11
#define HKNIFE2					18	// 11
#define HBOW1					19	// 12
#define HBOW2					20	// 12
#define HTWOHAND_SWORD1			21	// 13		WOMAN	-> 13
#define HTWOHAND_SWORD2			22	// 13		WOMAN	-> 13
#define HTWOHAND_FIRE_SWORD		23	// 14	
#define HTWOHAND_ICE_SWORD		24	// 15
#define HTWOHAND_ELECT_SWORD	25  // 16  


#define HSHIELD1				0	// 17		
#define HSHIELD2				1
#define HSHIELD3				2
#define HSHIELD4				3
#define HSHIELD5				4
#define HSHIELD6				5
#define HSHIELD7				6


class cHorseRider
{
	char*				RiderPicBuffer[2][3];	
	Spr					RiderPic[2][3][8][75];				// 2종류 // 3가지 // 8방향 // 75프레임
	char*				RiderBandPicBuffer[2][3];			// 띠
	Spr					RiderBandPic[2][3][8][75];

	char*				HorsePicBuffer[3];	
	Spr					HorsePic[3][8][75];					// 3종류 // 8방향 // 75프레임
	char*				HorseSaddlePicBuffer[3];
	Spr					HorseSaddlePic[3][8][75];			// 3종류 // 8방향 // 75프레임

	char*				WeaponPicBuffer[2][24];
	Spr					WeaponPic[2][24][8][75];				// 2종료 // 19가지 // 8방향 // 75프레임

	char*				GuardPicBuffer[2];
	Spr					GuardPic[2][8][75];					// 2가지 // 8방향 // 75프레임

	char*				RiderGradePicBuffer[2][3][2];	
	Spr					RiderGradePic[2][3][2][8][75];		// 2종류 // 3가지 // 8방향 // 75프레임

	char*				HorseGradePicBuffer[3][2];	
	Spr					HorseGradePic[3][2][8][75];			// 3종류 // 2가지 // 8방향 // 75프레임

	char*				RiderMantlePicBuffer[2];			// 2종류
	Spr					RiderMantlePic[2][8][75];

	PCANIMATIONTABLE	HorseRiderAniTable[2][3][MAX_ANIMATION];		// LTS HORSERIDER SOUND

	inline void			PutConfusion(LPCHARACTER ch);
	inline void			PutPoison(LPCHARACTER ch);
	inline void			PutStone(LPCHARACTER ch);
	inline void			PutSlow(LPCHARACTER ch);
	inline void			PutFrozen(LPCHARACTER ch);
	inline void			PutCurse(LPCHARACTER ch);
	inline void			PutTransparency(LPCHARACTER ch);
	inline void			PutTree(LPCHARACTER ch);
	inline void			PutGreen(LPCHARACTER ch);
	inline void			PutRed(LPCHARACTER ch);
	inline void			PutGhost(LPCHARACTER ch);

	inline void			DrawRider(LPCHARACTER ch);
	inline void			DrawRiderColor(LPCHARACTER ch,DWORD Color1,DWORD Color2);
	inline void			DrawRiderFX(LPCHARACTER ch,int Trans,int Operation);
	inline void			DrawRiderVoid(LPCHARACTER ch);
	
	inline void			DrawHorse(LPCHARACTER ch);
	inline void			DrawHorseColor(LPCHARACTER ch,DWORD Color1,DWORD Color2);
	inline void			DrawHorseFX(LPCHARACTER ch,int Trans,int Operation);
	inline void			DrawHorseVoid(LPCHARACTER ch);
	
	inline void			Draw1(LPCHARACTER ch,int LocationX,int LocationY,Spr* First,Spr* Second);
	inline void			DrawRGB1(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second,DWORD Color);
	inline void			Draw2(LPCHARACTER ch,int LocationX,int LocationY,Spr* First,Spr* Second);
	inline void			DrawRGB2(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second,DWORD Color);

	inline void			DrawAction(LPCHARACTER ch);
	inline void			DrawRGBColor(LPCHARACTER ch,DWORD Color1,DWORD Color2);
	inline void			DrawFX(LPCHARACTER ch,int Trans,int Operation);
	inline void			DrawVoid(LPCHARACTER ch);

	inline int			CheckSex(LPCHARACTER ch);
	inline int			CheckRiderKind(LPCHARACTER ch);
	inline int			CheckHorseKind(LPCHARACTER ch);
	inline int			CheckWeaponKind(LPCHARACTER ch);
	inline int			CheckGuardKind(LPCHARACTER ch);
	inline int			CheckRiderGrade(LPCHARACTER ch);
	inline int			CheckHorseGrade(LPCHARACTER ch);

	inline int			CheckWeaponSpriteData(int SexType,int SpriteKind);
	inline void			GetWeaponFileName(int SexType,int SpriteKind,char* FileName);

	inline void			FreeRiderSprite(int SexType,int SpriteKind);
	inline void			ClearRiderSprite();
	inline void			FreeRiderBandSprite(int SexType,int SpriteKind);
	inline void			ClearRiderBandSprite();

	inline void			FreeHorseSprite(int SpriteKind);
	inline void			ClearHorseSprite();
	inline void			FreeHorseSaddleSprite(int SpriteKind);
	inline void			ClearHorseSaddleSprite();

	inline void			FreeWeaponSprite(int SexType,int SpriteKind);
	inline void			ClearWeaponSprite();
	inline void			FreeGuardSprite(int SexType,int SpriteKind);
	inline void			ClearGuardSprite();

	inline void			FreeRiderGradeSprite(int SexType,int SpriteKind,int GradeType);
	inline void			ClearRiderGradeSprite();
	inline void			FreeHorseGradeSprite(int SpriteKind,int GradeType);
	inline void			ClearHorseGradeSprite();

	inline void			FreeRiderMantleSprite(int SexType);
	inline void			ClearRiderMantleSprite();

	inline int			DirectionChange(LPCHARACTER ch,int WeaponKind);
	


	inline Spr*			GetSpriteData(int SexType,int SpriteType,int SpriteKind,int Direction,int Frame);

	// Loading Routine Intergration Need..
	int					LoadRiderSprite(int SexType,int SpriteKind);		// Rider Sprite Data
	int					LoadRiderGradeSprite(int SexType,int SpriteKind,int GradeKind);	// Rider Grade Sprite Data
	int					LoadRiderBandSprite(int SexType,int SpriteKind);	// Rider Band Data
	int					LoadHorseSprite(int SpriteKind);					// Horse Sprite Data
	int					LoadHorseSaddleSprite(int SpriteKind);				// Horse Saddle Data
	int					LoadHorseGradeSprite(int HorseType,int GradeKind);	// Horse Grade Sprite Data
	int					LoadRiderMantleSprite(int SexType);					// Mantle Data

	int					LoadWeaponSprite(int SexType,int SpriteKind);
	int					LoadGuardSprite(int SexType,int SpriteKind);		// 로드함수 범용루틴으로 만든다.

public :

	cHorseRider();
	~cHorseRider();

	void				LoadHorseData(LPCHARACTER ch);
//	void				LoadHorseRiderAniMationTable(char* FileName);
	void				LoadHorseRiderAniMationTable(int Sex,int HorseNo,char* FileName);
	void				CheckAnimation(LPCHARACTER ch);
	void				CheckDeathAnimation(LPCHARACTER ch);
	void				CheckMoveAnimation(LPCHARACTER ch);
	void				CheckAttackAnimation(LPCHARACTER ch);
	int					GetDistance(LPCHARACTER ch);
	void				ClearSpriteAll();
	void				DrawHorseRider(LPCHARACTER ch);
	int					GetWaveNo(LPCHARACTER ch);
	int					GetEffectWaveNo(LPCHARACTER ch);

};

#endif