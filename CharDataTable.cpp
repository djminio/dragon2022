/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/

#include "StdAfx.h"


#include "Dragon.h"
#include "Char.h"
#include "Hong_Sub.h"
#include "directsound.h"
#include "CharDataTable.h"
#include "Horse.h"										// LTS HOSERIDER



extern CHARACTERLIST		g_CharacterList;
extern cHorseRider			g_HorseRider;				// LTS HORSERIDER


#define WAVE_ANIMATION  0


NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev] ;


////////////////////////////////////////////////////////////////////////////////////////////////
//
//	User Function...
//

///////////////// 0807 lkh 추가 ////////////////
int ReturnWaveHeroAnimation( LPCHARACTER ch, int wave )
{
  switch ( ch->nCurrentAction )
  {			
    case ACTION_BATTLE_NONE_TWOHAND	://전투기본 2( 양손 )
    case ACTION_BATTLE_NONE			://전투기본
	case ACTION_NONE				://기본동작
	case ACTION_DIE					://죽기
	case ACTION_MAGIC_BEFORE		://마법시전
	case ACTION_RUN					://뛰기
	case ACTION_BATTLE_WALK_OWNHAND	://전투걷기
	case ACTION_BATTLE_WALK_TWOHAND	://전투걷기 2 ( 양손에 검 )
	case ACTION_BASIC_WALK			://기본걷기


	case ACTION_ATTACK_ONESWORD1	://한손공격
	case ACTION_ATTACK_ONESWORD2	://한손공격2
	case ACTION_ATTACK_BONG_DOWN	://봉, 곤봉, 공격( 내려치기 )
	case ACTION_ATTACK_BONG_JJIRKI	://창, 봉, 공격( 찌르기 )
	case ACTION_ATTACK_BOW			://활쏘기	
	case ACTION_ATTACK_PENSWORD2	://펜싱자세
	case ACTION_ATTACK_PENSING1		://펜싱공격 1
	case ACTION_ATTACK_PENSING2		://펜싱공격 2
	case ACTION_THROW				://던지기공격
	case ACTION_ATTACK_BONG_ROUND	://봉, 창, 곤봉, 공격( 휘두르기 )
			
			return AttackRangeTable[ch->sprno][ ch->accessory[ 2]][8 + ch->sex];
		
		break;


#define WAV_ARMOR_IRON_HITED		214
#define WAV_ARMOR_ROBE_HITED		215
#define WAV_ARMOR_SKIN_HITED		216

//	214       쇠갑옷타격.wav
//	215       로브갑옷타격.wav
//	216       맨살타격.wav 

//	217       돌덩이타격.wav
//	218       얼음타격.wav
//	219       나무타격.wav
//	220       흙타격.wav

//	221       쇠방패.wav
//	222       나무방패.wav
	case ACTION_ATTACKED			://맞기  // ####아머 질감에 따라 타격시 나는 소리...
		// #### 
		if( wave == 1 )
		{
			switch( ch->accessory[0] )
			{
			case 3 :  
			case 5 :  
			case 7 :  
			case 13:  
			case 15:  return WAV_ARMOR_IRON_HITED;
				
				
			case 1 :  
			case 9 :  
			case 11:  
			case 17:  
			case 19:  
			case 21:  if( Random(2) )
						return WAV_ARMOR_ROBE_HITED; 

			default :  return 216; //     맨살타격.wav 
			}
		}
		else 
		{	
			if( ch->sex == 0 )
			{
				// #### 여자 '윽'소리를 넣는다. 
			}
			else 
			{
				// #### 남자 '윽'소리를 넣는다. 
			}
		}
		break;

	case ACTION_MAGIC_CASTING		://마법캐스팅
	case ACTION_SITDOWN				://않기
	case ACTION_HORSE				://말타기
		break;
  }

	return wave;
}


int ReturnWaveNPCAnimation( LPCHARACTER ch, int wave )
{	
	switch ( ch->nCurrentAction )
	{
	case MON1_NONE			:	//기본동작
	case MON1_DIE			:	// 죽기....
	case MON1_ATTACKED		:	//맞기	
	case MON1_BATTLE_ATTACK :	// 한손공격
	case MON1_CASTING_MAGIC :		// 08마법시전		// LTS DRAGON 
	case MON1_BATTLE_ATTACK2 :	// 09공격 2
	case MON1_BATTLE_ATTACK3 :	// 10공격 3
	case MON1_JUST_ACTION :		// 11임의의 동작 	
	break;
	} // switch..
	
	return wave;
}	

/*	
	MON1_NONE = 0,		// 기본동작
	MON1_BATTLE_WALK,	// 기본걷기
	MON1_ATTACKED,		// 맞기
	MON1_DIE,			// 죽기....
	MON1_BATTLE_ATTACK  // 한손공격
*/


///////////////////////////// SoundUp lkh 수정 ///////////////////////////////////
int ReturnPCAutoWave(LPCHARACTER	lpChar, int type)		//PC(lpChar->효과음 주체/대상 캐릭터 | type->0:공자/1:피타격자)
{
	int wave_Num =300;
	int	weapon_Type = IK_ONE_SWORD;
	int	attack_Pattern;			//맨손:0/한손검:1/창:2/곤,몽둥이:3/지팡이:4/한손철퇴:5/양손도끼:6/비검:7/한손부:8/봉:9/물어뜯기:10/기타:11

	// type -> 0:공격음 1:타격음
	//int sound		= lpChar->효과음 주체;

	CItem *weapon	= ItemUnit( EquipItemAttr[WT_WEAPON] );

	if(weapon)
		weapon_Type = weapon->GetItemKind();

	///////////////////////////// 타격음 ////////////////////////////////
	if(type)
	{
		LPCHARACTER			attacker=ReturnCharacterPoint(lpChar->attacker_ID);
		if(attacker==NULL)	return 0;

		//////////////////////////// npc의 공격 패턴 분류 /////////////////////////////////
		switch(attacker->sprno)
		{
			case 7:		//스켈레톤
			case 8:		//고블린
			case 18:	//키 작은 경비원
			case 19:	//키 큰 경비원
			case 20:	//뚱뚱한 경비원
			case 57:	//에델브로이
			case 58:	//레티
			case 59:	//화렌차
			case 62:	//오크2
			case 68:	//호비트남
			case 69:	//호비트여
			case 78:	//초쟁이
			case 63:	//엘프남
			case 64:	//엘프여
				attack_Pattern=1;		//한손검휘둘기
				break;
			case 10:	//가고일
			case 44:	//수정조각상
				attack_Pattern=2;		//창 계열
				break;
			case 13:	//트롤
				attack_Pattern=3;		//곤/몽둥이 계열
				break;
			case 17:	//타이번
			case 24:	//아저씨
			case 39:	//하급밴시
				attack_Pattern=4;		//지팡이
				break;
			case 37:	//얼음거인
				attack_Pattern=5;		//한손철퇴
				break;
			case 75:	//무기점 주인
			case 61:	//카리스누맨		
				attack_Pattern=6;		//양손도끼
				break;
			case 40:	//맨스콜피언
				attack_Pattern=7;		//비검던지기
				break;
			case 65:
			case 66:
				attack_Pattern=8;		//한손부
				break;
			case 60:	//시무니안
			case 74:	//대장간/목공소 주인
				attack_Pattern=9;		//봉 계열
				break;
			case 12:
			case 43:
			case 45:
			case 70:
				attack_Pattern=0;		//맨손휘둘기
				break;
			case 55:	//불독
			case 9:		//식인식물
			case 29:	//개
			case 47:	//도마뱀인간
			case 2:		//들쥐
			case 33:	//늑대
			case 32:	//맷돼지
				attack_Pattern=10;		//물어뜯기
				break;
			default:
				attack_Pattern=11;		//기타
				break;
		}
		
		BOOL	shield_Use=0;		//방패 착용 여부
		BOOL	armor_Use=0;		//방어복 착용 여부
		
		CItem *shield = ItemUnit( EquipItemAttr[WT_SHIELD] );
		if( shield && lpChar->peacests==1 )	shield_Use = TRUE;		//전투모드인 경우
		
		CItem *armor = ItemUnit( EquipItemAttr[WT_ARMOR] );
		if( armor )		armor_Use = TRUE;

		int		aw = attacker->accessory[2];

		//방패를 들고 있을 경우 25% 확률로 방패 타격음 처리
		if( shield_Use && rand()%101<=25 )
		{
			switch(lpChar->accessory[3])	//방패 타격
			{
			case 91:		//나무방패
			{
				// 공자의 공격동작에 따라
				if(attacker->nCurrentAction == ACTION_ATTACK_ONESWORD1 || attack_Pattern == 1 || attack_Pattern == 8 )
				{
					if( aw == 61 || aw == 62 || aw == 71 || attack_Pattern == 1 )
						wave_Num=337;		//한손칼
					else if( aw == 66 || aw == 67 || attack_Pattern == 8 )
						wave_Num=335;		//한손부
					else 
						wave_Num=334;		//철퇴
				}

				else if(attacker->nCurrentAction == ACTION_ATTACK_ONESWORD2)
				{
					switch( aw )
					{
						case 61:			//한손칼
						case 62:			
						case 66:			//한손도끼
						case 67:
							wave_Num=337;
							break;
						default:
							wave_Num=334;
							break;
					}
				}

				else if(attacker->nCurrentAction == ACTION_ATTACK_BONG_DOWN || attack_Pattern == 4 || attack_Pattern == 9 )
				{
					if( aw == 72 || aw == 73 || aw == 76 || aw == 77 || attack_Pattern == 4 || attack_Pattern == 9 )
						wave_Num=334;
					
					else if( aw == 68 || aw == 69 || attack_Pattern == 6 )		//양손부
						wave_Num=336+rand()%2;
					
					else if( aw == 70 || attack_Pattern == 5 )					//한손철퇴
						wave_Num=337;
				}

				else if(attacker->nCurrentAction == ACTION_ATTACK_BONG_JJIRKI || attacker->nCurrentAction == ACTION_ATTACK_BOW ||
					attacker->nCurrentAction == ACTION_THROW || attacker->nCurrentAction == ACTION_ATTACK_PENSING1 || attack_Pattern == 7 || attack_Pattern == 2)
					wave_Num=335;
				
				
				else if(attacker->nCurrentAction == ACTION_ATTACK_PENSING2)			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
				{
					if(attacker->sprno)		//남자
						wave_Num=335;
					else					//여자
						wave_Num=336;
				}

				else if(attacker->nCurrentAction == ACTION_ATTACK_BONG_ROUND)		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
				{
					if(attacker->sprno)		//남자
						wave_Num=335;
					else					//여자
						wave_Num=334;
				}
				
				else	wave_Num=335;
			}
			break;

			case 89:			//가죽방패
				wave_Num=330;
				break;
			
			default:			//금속방패
			{
				if(attacker->nCurrentAction==ACTION_ATTACK_ONESWORD1 || attack_Pattern == 1)
					wave_Num=331;
				else if(attacker->nCurrentAction==ACTION_ATTACK_ONESWORD2 || attack_Pattern == 5 || attack_Pattern == 8 )
				{
					if( aw == 61 || aw == 62 || aw == 71 || aw == 66 || aw == 67 || attack_Pattern == 5 || attack_Pattern == 8 )
						wave_Num=331;
					else
						wave_Num=332;
				}
				else if(attacker->nCurrentAction==ACTION_ATTACK_BONG_DOWN || attack_Pattern == 9 || attack_Pattern == 4 || attack_Pattern == 6 )
				{
					if( aw == 72 || aw == 73 || aw == 70 || attack_Pattern == 9 )
						wave_Num=331;
					else if( aw == 76 || aw == 77 || attack_Pattern == 4 )
						wave_Num=337;
					else if( aw == 68 || aw == 69 || attack_Pattern == 6 )
						wave_Num=332;
				}
				else if(attacker->nCurrentAction==ACTION_ATTACK_BONG_JJIRKI || attacker->nCurrentAction==ACTION_ATTACK_BOW ||
					attacker->nCurrentAction==ACTION_THROW || attacker->nCurrentAction==ACTION_ATTACK_PENSING1 || attack_Pattern == 7 || attack_Pattern == 2)
					wave_Num=333;
				else if(attacker->nCurrentAction==ACTION_ATTACK_PENSING2)
				{
					if(attacker->sprno)		//남자
						wave_Num=331;
					else					//여자
						wave_Num=333;
				}
				else if(attacker->nCurrentAction==ACTION_ATTACK_BONG_ROUND)
				{
					if(attacker->sprno)		//남자
						wave_Num=331;
					else
					{
						if( aw == 72 || aw == 73 )
							wave_Num=331;
						else
							wave_Num=337;
					}
				}
				else	wave_Num=332;
			}
			break;
			}		//end switch

		}
		
		else			//방패를 들고 있지 않은 경우(갑옷타격음)
		{
			if( armor && armor->GetItemKind() == IK_IRON_ARMOR )			//금속갑옷	// 0929 YGI
			{
				if(attacker->type==SPRITETYPE_CHARACTER)
				{
					switch(attacker->nCurrentAction)		//공자의 무기에 따른 차별적인 소리 연출
					{
					case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
						wave_Num=300+rand()%2;
						break;

					case ACTION_ATTACK_ONESWORD2:		//   6 한손공격 2연타
						switch( attacker->accessory[2] )
						{
							case 61:			//한손칼
							case 62:
							case 66:			//한손도끼
							case 67:
								wave_Num=300+rand()%2;
								break;
							default:
								wave_Num=302;
								break;
						}
						break;

					case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
						switch( attacker->accessory[2] )
						{
							case 72:			//	봉
							case 73:
							case 76:			//	지팡이
							case 77:
								wave_Num=303;
								break;
							case 68:		//	양손도끼
							case 69:
								wave_Num=302;
								break;
							case 70:		//	양손철퇴
								wave_Num=305;
								break;
						}
						break;

					case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
					case ACTION_ATTACK_BOW:				//   9 활쏘기
					case ACTION_THROW:					//  19 던지기공격
					case ACTION_ATTACK_PENSING1:			//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
						wave_Num=304;
						break;

					case ACTION_ATTACK_PENSING2:			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
						wave_Num=301;
						break;

					case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
						if(lpChar->sprno)		//남자
							wave_Num=304;
						else					//여자
							wave_Num=305;
						break;

					default:
						wave_Num=303;
						break;
					}
				}
				else		//공자가 npc인 경우
				{
					//맨손:0/한손검:1/창:2/곤,몽둥이:3/지팡이:4/한손철퇴:5/양손도끼:6/비검:7/한손부:8/봉:9/물어뜯기:10/기타:11
					switch(attack_Pattern)
					{
					case 0:
						wave_Num=303;
						break;
					case 1:
						wave_Num=300+rand()%2;
						break;
					case 2:
						wave_Num=304;
						break;
					case 3:
						wave_Num=302;
						break;
					case 4:
						wave_Num=305;
						break;
					case 5:
						wave_Num=305;
						break;
					case 6:
						wave_Num=302;
						break;
					case 7:
						wave_Num=304;
						break;
					case 8:
						wave_Num=300+rand()%2;
						break;
					case 9:
						wave_Num=303;
						break;
					case 10:
						//wave_Num=물어뜯기;
						break;
					case 11:
						//wave_Num=기타음;
						break;
					}
				}

			}
			else			//맨살,가죽,천,로브
			{
				if(attacker->type==SPRITETYPE_CHARACTER)
				{
					switch(attacker->nCurrentAction)		//공자의 무기에 따른 차별적인 소리 연출
					{
					case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
						switch( attacker->accessory[2] )
						{
							case 61:			//한손칼
							case 62:
								wave_Num=307;
								break;
							case 66:
							case 67:
								wave_Num=308;
								break;
							default:
								wave_Num=310;
								break;
						}
						break;

					case ACTION_ATTACK_ONESWORD2:		//   6 한손공격 2연타
						switch( attacker->accessory[2] )
						{
							case 61:			//한손칼
							case 62:
							case 66:			//한손도끼
							case 67:
								wave_Num=307;
								break;
							default:
								wave_Num=318;
								break;
						}
						break;

					case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
						switch( attacker->accessory[2] )
						{
							case 72:			//	봉
							case 73:
							case 76:			//	지팡이
							case 77:
								wave_Num=316;
								break;
							case 68:		//	양손도끼
							case 69:
								wave_Num=308;
								break;
							case 70:		//	양손철퇴
								wave_Num=316;
								break;
						}
						break;

					case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
						wave_Num=304;
						break;

					case ACTION_ATTACK_BOW:				//   9 활쏘기
					case ACTION_THROW:					//  19 던지기공격
						wave_Num=312;
						break;

					case ACTION_ATTACK_PENSING1:		//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
						if(lpChar->sprno)		//남자
							wave_Num=319;
						else
							wave_Num=312;
						break;
					
					case ACTION_ATTACK_PENSING2:		//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
						if(lpChar->sprno)		//남자
							wave_Num=319;
						else
							wave_Num=320;
						break;

					case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
						if(lpChar->sprno)		//남자
							wave_Num=306;
						else					//여자
							wave_Num=317;
						break;

					default:
						wave_Num=315;
						break;
					}
				}
				else		//공자가 npc인 경우
				{
					//맨손:0/한손검:1/창:2/곤,몽둥이:3/지팡이:4/한손철퇴:5/양손도끼:6/비검:7/한손부:8/봉:9/물어뜯기:10/기타:11
					switch(attack_Pattern)
					{
					case 0:
						wave_Num=315;
						break;
					case 1:
						wave_Num=307;
						break;
					case 2:
						wave_Num=304;
						break;
					case 3:
						wave_Num=318;
						break;
					case 4:
						wave_Num=317;
						break;
					case 5:
						wave_Num=316;
						break;
					case 6:
						wave_Num=308;
						break;
					case 7:
						wave_Num=312;
						break;
					case 8:
						wave_Num=307;
						break;
					case 9:
						wave_Num=316;
						break;
					case 10:
						//wave_Num=물어뜯기;
						break;
					case 11:
						//wave_Num=기타음;
						break;
					}
				}
				//wave_Num=315;
			}
		}
	}

	//////////////////////////// 공격음 /////////////////////////////
	else		//공자(공격)
	{
		if( !weapon )
		{	
			wave_Num = 219;	//맨주먹	(SOUND_NONEHAND)
			return wave_Num;
		}

		switch(lpChar->nCurrentAction)
		{
		case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
			switch( weapon_Type )
			{
				case IK_ONE_SWORD:			//한손칼
					wave_Num=200+rand()%2;
					break;
				case IK_ONE_AXE:			//한손도끼
					wave_Num=202+rand()%2;
					break;
				case IK_ONE_MACE:			//한손철퇴
					wave_Num=204;
					break;
				case IK_GON:				//한손채찍
					wave_Num=207;
					break;
			}
			break;

		case ACTION_ATTACK_ONESWORD2:		//   6 한손공격 2연타
			switch( weapon_Type )
			{
				case IK_ONE_SWORD:			//한손칼
					wave_Num=200+(lpChar->odd_Even%2);
					lpChar->odd_Even++;
					break;
				case IK_ONE_AXE:			//한손도끼
					wave_Num=202+(lpChar->odd_Even%2);
					lpChar->odd_Even++;
					break;
				case IK_GON:				//한손채찍
					wave_Num=207+(lpChar->odd_Even%2);
					lpChar->odd_Even++;
					break;
			}
			break;

		case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
			switch( weapon_Type )
			{
				case IK_WHIRL:			//	봉
					wave_Num=209;
					break;
				case IK_STAFE:			//	지팡이
					wave_Num=210;
					break;
				case IK_TWO_AXE:		//	양손도끼
					wave_Num=211;
					break;
				case IK_TWO_MACE:		//	양손철퇴
					wave_Num=212;
					break;
			}
			break;

		case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
			wave_Num=213;
			break;

		case ACTION_ATTACK_BOW:				//   9 활쏘기
			wave_Num=214;
			break;

		case ACTION_THROW:					//  19 던지기공격
			wave_Num=216;
			break;

		case ACTION_ATTACK_PENSING1:			//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
			if(lpChar->sprno)		//남자
				wave_Num=215;
			else					//여자
				wave_Num=217;
			break;

		case ACTION_ATTACK_PENSING2:			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
			if(lpChar->sprno)		//남자
				wave_Num=215;
			else					//여자
				wave_Num=217;
			break;

		case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
			if(lpChar->sprno)		//남자
				wave_Num=209;
			else					//여자
				wave_Num=218;
			break;

		default:
			wave_Num=219;
			break;
		}
	}


	if( wave_Num == 67 )
	{
		_asm nop;
	}

	return wave_Num;
}

//////////////// SoundUp lkh 수정 ////////////////
int ReturnNPCAutoWave(LPCHARACTER	lpChar, int type)		//NPC(						"						)
{
	int wave_Num = 200; // DEFAULT..
	int	material;		//재질분류:0->금속/1->나무/2->맨살,가죽/3->물렁(내장)/4->돌,얼음/5->뼈,각질껍질
	LPCHARACTER			attacker=ReturnCharacterPoint(lpChar->attacker_ID);
	// 만약 대상이 없는 경우(즉 지역마법인 경우)
	if(attacker==NULL)		return 0;

	// type -> 0:공격음 1:타격음

	//////////////// 대상->타격음 /////////////////
	if(type==1)		
	{
		switch(lpChar->sprno)
		{
		// 금속 재질
		case 20:
		case 80:
		case 19:
		case 63:
		case 18:
		case 64:
			material = 0;
			break;
		case 92:
		case 93:
		case 94:
		case 95:
			material = 1;
			break;
		case 5:
		case 6:
		case 45:
			material = 3;
			break;
		case 44:
		case 36:
		case 38:
			material = 4;
			break;
		case 7:		//스켈레톤
		case 48:
		case 3:
		case 40:
		case 70:
		case 35:
		case 52:
			material = 5;
			break;
		default:
			material = 2;
			break;
		}

		switch(material)
		{
		case 0:			//금속재질
			switch(attacker->nCurrentAction)		//공자의 무기에 따른 차별적인 소리 연출
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
				wave_Num = 300+rand()%2;
				break;

			case ACTION_ATTACK_ONESWORD2:
				switch( attacker->accessory[2] )
				{
				case 61:
				case 62:
				case 66:
				case 67:
					wave_Num = 300+rand()%2;
					break;
				default:
					wave_Num = 302;
					break;
				}
				break;

			case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
				switch( attacker->accessory[2] )
				{
					case 72:			//	봉
					case 73:
					case 76:			//	지팡이
					case 77:
						wave_Num=303;
						break;
					case 68:		//	양손도끼
					case 69:
						wave_Num=302;
						break;
					default:
						wave_Num=305;
						break;
				}
				break;

			case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
			case ACTION_ATTACK_BOW:				//   9 활쏘기
				wave_Num=304;
				break;

			case ACTION_THROW:					//  19 던지기공격
			case ACTION_ATTACK_PENSING1:		//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
				wave_Num=304;
				break;

			case ACTION_ATTACK_PENSING2:		//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
				wave_Num=301;
				break;

			case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
				wave_Num=305;
				break;

			default:
				wave_Num=303;
				break;
			}
			break;
			
		case 1:			//나무
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:
						wave_Num=335;
						break;
					case 66:
					case 67:
						wave_Num=306;
						break;
					default:
						wave_Num=309;
						break;
				}
				break;

			case ACTION_ATTACK_ONESWORD2:		//   6 한손공격 2연타
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:			
					case 66:			//한손도끼
					case 67:			
						wave_Num=313;
						break;
					default:
						wave_Num=318;
						break;
				}
				break;

			case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
				switch( attacker->accessory[2] )
				{
					case 72:			//봉
					case 73:
					case 76:			//지팡이
					case 77:
						wave_Num=319;
						break;
					case 68:			//양손도끼
					case 69:
						wave_Num=313;
						break;
					default:
						wave_Num=309;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
			case ACTION_ATTACK_PENSING1:		//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
				wave_Num = 322;
				break;

			case ACTION_ATTACK_BOW:				//   9 활쏘기
			case ACTION_THROW:					//  19 던지기공격
				wave_Num = 338;
				break;
				
			case ACTION_ATTACK_PENSING2:			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
				if(attacker->sprno)		//남자
					wave_Num=319;
				else					//여자
					wave_Num=324;
				break;

			case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
				wave_Num=334;
				break;

			default:
				wave_Num=335;
				break;
			}
			break;
		
		case 2:			//맨살/가죽
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:
						wave_Num=307;
						break;
					case 66:
					case 67:
						wave_Num=308;
						break;
					default:
						wave_Num=310;
						break;
				}
				break;
			case ACTION_ATTACK_ONESWORD2:		//   6 한손공격 2연타
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:			
					case 66:			//한손도끼
					case 67:			
						wave_Num=307;
						break;
					default:
						wave_Num=318;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
				switch( attacker->accessory[2] )
				{
					case 72:			//봉
					case 73:
					case 76:			//지팡이
					case 77:
						wave_Num=316;
						break;
					case 68:			//양손도끼
					case 69:
						wave_Num=308;
						break;
					default:
						wave_Num=316;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
				wave_Num = 304;
				break;
			case ACTION_ATTACK_PENSING1:		//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
			case ACTION_ATTACK_BOW:				//   9 활쏘기
			case ACTION_THROW:					//  19 던지기공격
				wave_Num = 312;
				break;
			case ACTION_ATTACK_PENSING2:			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
				if(attacker->sprno)		//남자
					wave_Num=319;
				else					//여자
					wave_Num=320;
				break;

			case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
				wave_Num=317;
				break;

			default:
				wave_Num=315;
				break;
			}
			break;
		
		case 3:			//슬라임/내장
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:
					case 66:
					case 67:
						wave_Num=325;
						break;
					default:
						wave_Num=326;
						break;
				}
				break;
			case ACTION_ATTACK_ONESWORD2:		//   6 한손공격 2연타
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:			
					case 66:			//한손도끼
					case 67:			
						wave_Num=325;
						break;
					default:
						wave_Num=326;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
					wave_Num=326;
					break;
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
			case ACTION_ATTACK_BOW:				//   9 활쏘기
			case ACTION_THROW:					//  19 던지기공격
			case ACTION_ATTACK_PENSING1:		//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
					wave_Num=327;
					break;
			case ACTION_ATTACK_PENSING2:			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
				if(attacker->sprno)		//남자
					wave_Num=326;
				else					//여자
					wave_Num=325;
				break;

			case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
			default:
				wave_Num=326;
				break;
			}
			break;
		case 4:
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:
					case 66:
					case 67:
						wave_Num=314;
						break;
					default:
						wave_Num=309;
						break;
				}
				break;
			case ACTION_ATTACK_ONESWORD2:		//   6 한손공격 2연타
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:			
					case 66:			//한손도끼
					case 67:			
						wave_Num=313;
						break;
					default:
						wave_Num=315;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
				switch( attacker->accessory[2] )
				{
					case 72:			//봉
					case 73:
						wave_Num=303;
						break;
					case 76:
					case 77:
					case 68:			//양손도끼
					case 69:
						wave_Num=313;
						break;
					default:
						wave_Num=310;
						break;
				}
				break;
					
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
				wave_Num=312;
				break;
			case ACTION_ATTACK_BOW:				//   9 활쏘기
			case ACTION_THROW:					//  19 던지기공격
				wave_Num=323;
				break;
			case ACTION_ATTACK_PENSING1:		//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
				wave_Num=323;
				break;
			case ACTION_ATTACK_PENSING2:			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
				if(attacker->sprno)		//남자
					wave_Num=313;
				else					//여자
					wave_Num=318;
				break;
			case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
				if(attacker->sprno)		//남자
					wave_Num=303;
				else
					wave_Num=313;
				break;
			default:
				wave_Num=311;
				break;
			}
			break;
		case 5:
			switch(attacker->nCurrentAction)
			{
			case ACTION_ATTACK_ONESWORD1:		//   5 한손공격 단방
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:
					case 66:
					case 67:
						wave_Num=320;
						break;
					default:
						wave_Num=321;
						break;
				}
				break;
			case ACTION_ATTACK_ONESWORD2:		//   6 한손공격 2연타
				switch( attacker->accessory[2] )
				{
					case 61:			//한손칼
					case 62:			
					case 66:			//한손도끼
					case 67:			
						wave_Num=322;
						break;
					default:
						wave_Num=321;
						break;
				}
				break;
			case ACTION_ATTACK_BONG_DOWN:		//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
				switch( attacker->accessory[2] )
				{
					case 72:			//봉
					case 73:
					case 76:
					case 77:
					case 68:			//양손도끼
					case 69:
						wave_Num=322;
						break;
					default:
						wave_Num=321;
						break;
				}
				break;
					
			case ACTION_ATTACK_BONG_JJIRKI:		//   8 창 공 격( 한손 찌르기-어깨 위에서 )
				wave_Num=323;
				break;
			case ACTION_ATTACK_BOW:				//   9 활쏘기
			case ACTION_THROW:					//  19 던지기공격
				wave_Num=324;
				break;
			case ACTION_ATTACK_PENSING1:		//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
				wave_Num=324;
				break;
			case ACTION_ATTACK_PENSING2:			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
				if(attacker->sprno)		//남자
					wave_Num=322;
				else					//여자
					wave_Num=320;
				break;
			case ACTION_ATTACK_BONG_ROUND:		//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
				if(attacker->sprno)		//남자
					wave_Num=320;
				else
					wave_Num=320;
				break;
			default:
				wave_Num=321;
				break;
			}
			break;
		}
	}	
	
	//////////////// 공자(공격음) /////////////////
	else if(type==0)
	{
		switch(lpChar->sprno)
		{
		case 7:		//스켈레톤
		case 8:		//고블린
		case 9:		//식인식물
		case 18:	//키 작은 경비원
		case 19:	//키 큰 경비원
		case 20:	//뚱뚱한 경비원
		case 57:	//에델브로이
		case 58:	//레티
		case 59:	//화렌차
		case 62:	//오크2
		case 68:	//호비트남
		case 69:	//호비트여
		case 78:	//초쟁이
		case 63:	//엘프남
		case 64:	//엘프여
			wave_Num=200+rand()%2;		//한손검휘둘기
			break;
		case 10:	//가고일
		case 44:	//수정조각상
			wave_Num=213;		//창 계열
			break;
		case 13:	//트롤
			wave_Num=207+rand()%2;		//곤/몽둥이 계열
			break;
		case 17:	//타이번
		case 24:	//아저씨
		case 39:	//하급밴시
			if(rand()%2)
				wave_Num=210;		//지팡이 계열
			else
				wave_Num=218;
			break;
		case 37:	//얼음거인
			wave_Num=204;		//한손철퇴
			break;
		case 75:	//무기점 주인
		case 61:	//카리스누맨		
			wave_Num=211;		//양손도끼
			break;
		case 40:	//맨스콜피언
			wave_Num=216;		//비검던지기
			break;
		case 65:
		case 66:
			wave_Num=202+rand()%2;		//한손부
			break;
		case 60:	//시무니안
		case 74:	//대장간/목공소 주인
			wave_Num=209;		//봉 계열
			break;
		case 12:
		case 43:
		case 45:
		case 70:
			wave_Num=219;		//맨손휘둘기
			break;
		}
	}

	else if(type==2)//몬스터 죽기에 대한 효과음
	{
		switch(lpChar->sprno)
		{
		case 7:		//스켈레톤
			wave_Num=440;		//부서죽기
			break;
		case 37:
		case 38:
		case 44:
			wave_Num=445;		//돌터져죽기
			break;
		case 65:	case 66:
			wave_Num=444;
			break;
		case 10:
		case 41:
		case 43:
		case 48:
		case 49:
		case 54:
			wave_Num=443;	//떨어져
			break;
		case 9:		//식인식물
			wave_Num=441;	//찢어져
			break;
		default:
			if(rand()%2)
				wave_Num=446;	//내장 터지는 1
			else
				wave_Num=447;	//내장 터지는 2
		}

	}

	return  wave_Num;
}


////////////////// 0810 lkh 추가  ////////////////////
int	ReturnWaveNatural( LPCHARACTER	ch )
{
	int return_Wave = 0;
		
	int	max_count = ch->AnimationTable[ACTION_NONE].nCount;
	
	for(int i=0; i<max_count; i++)
	{
		return_Wave = ch->AnimationTable[ACTION_NONE].wave[i];
		if(return_Wave/1000==8)
			return (return_Wave%1000);
	}
	return 0;
}

////////////////// SoundUp lkh 수정(5Type효과음처리) ////////////////////
int ReturnWaveAnimation( LPCHARACTER ch )
{	
	int	return_Wave=0;

	int wave=0;
	if (ch->HorseNo>0)		// LTS HORSERIDER ADD
	{
		if (ch->animationType==ANIMATION_TYPE_MON1) wave= ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; // nCurrentFrame
		else wave=g_HorseRider.GetWaveNo(ch);
	}
	else
	{
		wave= ch->AnimationTable[ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame
	}

	if( wave == 67 )
	{
		_asm nop;
	}

	if( wave <= 0 )								return 0;
//	if( wave > MAX_SOUND && wave != 9999 )		return 0;

	/////////////////////////// 40% 확률로 소리남 ////////////////////////
	//if( (wave>=66 && wave<=80) || (wave>=84 && wave<=98) || (wave>=267 && wave<=318)
	//	|| wave==132 || wave==134 || wave==135 || wave==136 ) //기합과 비명 소리의 경우
	//{
	//	if(rand()%5<2)
	//		return wave;
	//	else
	//		return 0;
	//}
	
	// wave 테이블 항목의 값이 9999인 경우 캐릭터의 장착된 장비 및 NPC종류에 따라 알맞은 효과음 출력
	if(wave == 9999)
	{
		if( ch->type==SPRITETYPE_CHARACTER )									//PC Character
		{
			switch( ch->nCurrentAction )
			{
			case ACTION_ATTACK_ONESWORD1:		//한손공격1
			case ACTION_ATTACK_ONESWORD2:		//한손공격2
			case ACTION_ATTACK_BONG_DOWN:		//내려치기공격
			case ACTION_ATTACK_BONG_JJIRKI:		//찌르기공격
			case ACTION_ATTACK_PENSING1:		//팬싱공격1
			case ACTION_ATTACK_PENSING2:		//팬싱공격2
			case ACTION_ATTACK_BONG_ROUND:		//휘두르기
			case ACTION_ATTACK_BOW:				//활쏘기
			case ACTION_THROW:					//비검던지기
				return_Wave=ReturnPCAutoWave(ch, 0);	//해당 캐릭터가 장착하고 있는 무기에 따라 다른 소리 리턴
				break;
			case ACTION_ATTACKED:				//맞기
				return_Wave=ReturnPCAutoWave(ch, 1);	//해당 캐릭터가 장착하고 있는 보호구에 따라 다른 소리 리턴
				break;
			}
		}
		
		else if( ch->type==SPRITETYPE_NPC || ch->type==SPRITETYPE_MONSTER )		//NPC Character
		{	
			switch( ch->nCurrentAction )
			{
			case MON1_BATTLE_ATTACK:				//한손공격
				return_Wave=ReturnNPCAutoWave(ch, 0);
				break;
			case MON1_ATTACKED:						//맞기	
				return_Wave=ReturnNPCAutoWave(ch, 1);
				break;
			}
		}
		return return_Wave;
	}

	// 8XXX인 경우 -> 캐릭의 고유음인 경우(특정 에니메이션이 계속 반복되는 경우 특정 간격 마다 자연음이 출력되게 하는 루틴 / XXX->특정번호)
	if(wave/1000 == 8)
	{
		//int	gap = 5;					//고유음 플레이시 에니메이션 반복 횟수
		int	wave_Natural = wave%1000;
				
		if(ch->naturalSound_Time < g_curr_time)		//다음 자연음을 발생시킬 만큼 시간이 경과한 경우
		{
			// 현재시간에다 13~25초 이후마다
			ch->naturalSound_Time = g_curr_time + 13 + ( rand()%13 );
			if(rand()%3==0)		return	wave_Natural;		//고유음
			else				return 0;
		}
	}
	
	// 33% 확률로 소리 내기 7XXX
	if( wave/1000 == 7 )
	{
		if(rand()%3)
			return (wave%1000);
	}

	// 만단위가 1인 경우:해당번호 이후 몇번째 화일까지 랜덤 플레이하는 경우
	if(wave/10000 == 1)					//천단위 숫자:기준번호 이후 몇번째 화일까지, 백단위 이하:실제 기준 사운드 번호
	{											
		BOOL	play_OnOff=TRUE;
		
		// 기합소리는 어쩌다 한번(50% 확률)
		switch( ch->nCurrentAction )
		{
		case ACTION_ATTACK_ONESWORD1:			//   5 한손공격 단방
		case ACTION_ATTACK_ONESWORD2:			//   6 한손공격 2연타
		case ACTION_ATTACK_BONG_DOWN:			//   7 봉, 곤봉,  공격( 양손 내려치기-머리위에서 아래로 )
		case ACTION_ATTACK_BONG_JJIRKI:			//   8 창 공 격( 한손 찌르기-어깨 위에서 )
		case ACTION_ATTACK_BOW:					//   9 활쏘기
		case ACTION_ATTACK_PENSING1:			//  11 여자:펜싱공격 1 (길게 한방 찌르기)			남자:양손 위에서 대각선으로 베기
		case ACTION_ATTACK_PENSING2:			//  12 여자:펜싱공격 2 (위에서 아래로 내려 찌르기)	남자:양손 길게 찌르기
		case ACTION_THROW:						//  19 던지기공격
		case ACTION_ATTACK_BONG_ROUND:			//  23 여자:봉,창,곤봉 옆에서 휘두르기				남자:3연타 찌르기
			//if(rand()%2)	play_OnOff = FALSE;	//	50% 확률 적용
			break;
		default:
			break;
		}
		
		if(play_OnOff)
		{
			int		wave_Real = (wave%1000);		//실제 사운드 번호
			int		conti = (wave%10000)/1000;		//첫번째 사운드 번호 이후 몇번째까지
			//acer7
			if( conti )
				wave_Real = wave_Real + rand()%conti;	//실제 플레이될 사운드 번호
			return wave_Real;
		}
	}
	return wave;
}	
///////////////////////////////////////////////////////////////////////////	
	
///////////////////////////////////////////////////////////////////////////	
int ReturnEffectAnimation( LPCHARACTER ch )		// LTS HORSERIDER ADD
{
	int effect;
	if (ch->HorseNo>0)
	{
		if (ch->animationType==ANIMATION_TYPE_MON1) effect=ch->AnimationTable[ ch->nCurrentAction].effect[ ch->nCurrentFrame.GetDecrypted()];
		else effect=g_HorseRider.GetEffectWaveNo(ch);
	}
	else
	{
		effect=ch->AnimationTable[ ch->nCurrentAction].effect[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame

    if (effect == 3)
    {
      int k =0;
    }
	}
	return effect;
}
	
int ReturnAttackWaveAnimation( LPCHARACTER ch )		// LTS HORSERIDER ADD
{	
	int wave;
	
	if (ch->HorseNo>0)
	{
		if (ch->animationType==ANIMATION_TYPE_MON1) wave=ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame
		else wave=g_HorseRider.GetWaveNo(ch);
	}
	else
	{
		wave=ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame
	}


	return wave;
}	
	
int ReturnAttackedWaveAnimation( LPCHARACTER ch )	// LTS HORSERIDER ADD
{	
	int wave;
	if (ch->HorseNo>0)
	{
		if (ch->animationType==ANIMATION_TYPE_MON1) wave=ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()];
		else wave=g_HorseRider.GetWaveNo(ch);
	}
	else
	{
		wave=ch->AnimationTable[ ch->nCurrentAction].wave[ ch->nCurrentFrame.GetDecrypted()]; //nCurrentFrame
	}
	
	return wave;
}

LPCHARACTER  ReturnCharacterID( int id )
{
	LPCHARACTER lpcharacter = 	Hero;
	while( lpcharacter != NULL )
	{
		if( lpcharacter->id == id ) return lpcharacter;
		lpcharacter = lpcharacter->lpNext;
	}

	return NULL;
}
	
LPCHARACTER ReturnCharacterPoint( int id )
{	
	LPCHARACTER lpcharacter = Hero;
	while( lpcharacter != NULL )
	{
		if( lpcharacter->id == id ) return lpcharacter;
		lpcharacter = lpcharacter->lpNext;
	}
	
	return NULL;
}	

XorCrypt PlusMovePDisTance( LPCHARACTER ch, XorCrypt dx )
{	
	XorCrypt ddx, rest;

	if( dx.GetDecrypted() == 0 )
	{
		ddx.PutEncrypted(0);
		return ddx;
	}

	ddx.PutEncrypted(ch->movp.GetDecrypted() * dx.GetDecrypted() / 50);

	rest.PutEncrypted( ddx.GetDecrypted() * 50 / dx.GetDecrypted() );

	ch->restdx += abs( ch->movp.GetDecrypted() - rest.GetDecrypted() );
	
	if( ch->restdx >= ch->movp.GetDecrypted() )  
	{
		ch->restdx -= ch->movp.GetDecrypted();
		ddx.PutEncrypted(ddx.GetDecrypted()+1);
		//ddx ++;
	}

	//ch->ddx = ddx;
	return ddx;
}	

void ReturnCharMoveDistance( LPCHARACTER ch, XorCrypt *dist )
{
	switch( ch->animationType )
	{	
		case ANIMATION_TYPE_TEST  : 
		case ANIMATION_TYPE_MAN   :
		case ANIMATION_TYPE_WOMAN : switch( ch->nCurrentAction )
									{
										case ACTION_BATTLE_WALK_TWOHAND	://전투걷기 2 ( 양손에 검 )
										case ACTION_BATTLE_WALK_OWNHAND	://전투걷기	
										case ACTION_BASIC_WALK			://기본걷기		
											if (ch->HorseNo)										// LTS HORSERIDER
											{
												dist->PutEncrypted(g_HorseRider.GetDistance(ch));
											}
											else
											{
												dist->PutEncrypted(ch->AnimationTable[ ch->nCurrentAction].dx[ ch->nCurrentFrame.GetDecrypted()]);
											}
											*dist = PlusMovePDisTance( ch, *dist );
											return;
										case ACTION_RUN					://뛰기	 : 겉기의 1.5배..
											if (ch->HorseNo)										// LTS HORSERIDER
											{
												dist->PutEncrypted(g_HorseRider.GetDistance(ch)*150/100);
											}
											else
											{
												dist->PutEncrypted((ch->AnimationTable[ ch->nCurrentAction].dx[ ch->nCurrentFrame.GetDecrypted()])*150/100);
											}
											*dist = PlusMovePDisTance( ch, *dist );
											return;
									}	
			break;					
										
		case ANIMATION_TYPE_MON1  : switch( ch->nCurrentAction )
									{
										case MON1_BATTLE_WALK	://전투걷기
												dist->PutEncrypted( ch->AnimationTable[ ch->nCurrentAction].dx[ ch->nCurrentFrame.GetDecrypted()]);
												*dist = PlusMovePDisTance( ch, *dist );
												return;
									}	
			break;
	}	
}		
		
int IsDead( LPCHARACTER ch )
{		
	if( ch == NULL ) return 1;
	if( ch == Hero ) 
	{	
		return (SCharacterData.nCharacterHP <= 0);
	}	
	else 
	{	
		return ( ch->hp <= 0 );
	}	
		
	return 0;
}		

bool IsAttackAble( LPCHARACTER pTarget )
{
	//Return false if the passed target is a GM, and the player itself is NOT
	if(pTarget->type == SPRITETYPE_CHARACTER && (pTarget->IsCounselor() && !Hero->IsCounselor() && pTarget->name_status.santa_power))
	{ // 메시지 출력 필요
		return false;
	}

	if( pTarget->viewtype == VIEWTYPE_GHOST_ )
	{
		return false;
	}
	return true;
}
		
void LoadLevelExpTable( void )
{		
	FILE  *fp;
	int		i;
	char    crc, crc1=0;

	fp = Fopen( "./data/levelexp.bin", "rb" ); // CSD-030306

	if( fp )
	{	
		fread( NPC_Lev_Ref, sizeof( NPCLev_to_Exp ), Num_Of_NPC_Lev, fp );
		fread( &crc, 1, 1, fp );
		fclose(fp);
		
		char *tt = (char *)NPC_Lev_Ref;
		for( i = 0 ; i  < sizeof( NPCLev_to_Exp )*Num_Of_NPC_Lev ; i ++)
		{	
			crc1 += *tt;
			*tt -=( 100 + i );
			tt ++;
		}
		if( CheckSumError == 0 ) 
		{
			CheckSumError = abs( crc1 - crc );
			if( CheckSumError ) JustMsg( "LevelUpTable Error" );
		}
	}	
}		

extern void SendAction(int i,int Direction);			// LTS ACTION 
void SendSmileFace( int smileno )
{
	t_packet p;
	
	p.h.header.type = CMD_SMILE_FACE;
	{
		p.u.client_smile_face.smileno = smileno;
	}
	p.h.header.size = sizeof( t_client_smile_face );

	//kyo
	switch(smileno)
	{
	case 3:
	case 4:
	case 6:
	case 7:
	case 21:
	case 27:
		{
			SendAction(ACTION_ANGER,Hero->direction);
		}break;
	case 1:
	case 5:
	case 8:
	case 13:
	case 19:
	case 20:
	case 22:
	case 24:
	case 25:
		{
			SendAction(ACTION_VICTORY,Hero->direction);
		}break;
	}
	

	QueuePacket( &p, 1 );

	Hero->smiledelay = 120;
	Hero->smileno = smileno;
}

 
void RecvSmileFace( int id, int smileno )
{	
	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;

	if( smileno < 0 || smileno > 29 ) return;

	ch->smiledelay = 120;
	ch->smileno = smileno;
}





//-----------------------------------------------------------------------------------

/*

extern SystemInfo	system_info;

void PlaySoundWalkingWater( LPCHARACTER ch, int anino )
{
	int waveno;
	int vol;
	int left;

	if( ch == Hero )
	{
		waveno = ReturnAttackWaveAnimation( ch );
		if( waveno )
		{
			vol = system_info.sound;

			switch( anino )
			{
				case ACTION_BATTLE_WALK_OWNHAND	://전투걷기
				case ACTION_BATTLE_WALK_TWOHAND	://전투걷기 2 ( 양손에 검 )
				case ACTION_BASIC_WALK			://기본걷기

					waveno = 1144;


			// 다족류( 큰놈 ) 
			// 다족류(중간놈 )
			// 다족류( 작은놈 )
			// 대형
			// 큰발
			// 중간발
			// 작은발

			case ACTION_RUN					://뛰기
					waveno = 1146;
			}

			// 뛰기 
			//초소형
			// 절뚝거림
			// 기어감
			// 유령하급
			// 슬래임 
		}

	
	}
	else
	{
		vol = system_info.sound / 2;


		switch( ch->sprno )
		{
			// 다족류( 큰놈 ) 
			// 다족류(중간놈 )
			// 다족류( 작은놈 )
			// 대형
			// 큰발
			// 중간발
			// 작은발
			// 뛰기 
			//초소형
			// 절뚝거림
			// 기어감
			// 유령하급
			// 슬래임 
		}

	}


	SetVolumeOne( waveno, vol );


}





*/







