#include "stdafx.h"
#include <Math.H>
#include "gameproc.h"

#include "map.h"
#include "char.h"
#include "path.h"
#include "network.h"
#include "object.h"
#include"Prison.h"
#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"
#include "menu.h"
#include "Hangul.h"
#include "Dragon.h"
#include "Effect.h"
#include "DirectSound.H"
#include "CharDataTable.h"
#include "Tool.h"
#include "MouseCursor.h"
#include "Skill.h"
#include "Item.h"
#include "SkillTool.h"
#include "SmallMenu.h"
#include "LineCommand.h"
#include "Counseler.h"
#include "NPC_Pattern.h"

#include "Resource.h"
#include "chat.h"

#include "CurrentMsgMgr.h"
#include "ExchangeMgr.h"
#include "NumberAniMgr.h"
#include "NumberAniMgr.h"
#include "TextOutMgr.h"
#include "FrameMgr.h"
#include "BuffTimer.h"

///////////////////////////////////////////////////////////////////////////////
//
extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern CHARACTERLIST	g_CharacterList;
extern void DisplayEmote(const int x, const int y, const int iSmileNo);//020515 lsw
extern void WeatherStatus(void);
extern void PutMapLight(LPVOID dest, DWORD alpha);
extern BOOL LoadGameInfo(void);
extern int		GetItemAttr(int item_id, int attr); // MenuSet.h

extern void SendAction(int i, int Direction);			// LTS ACTION

extern bool g_bWarBegin;	// 011022 LTS
extern int	g_EventLocalWarBegin;		// 020115 LTS

extern int g_MouseItemType;	// menu.cpp
extern int g_MouseItemNumber;
extern int g_ActionAnger;		// LTS ACTION
extern int g_BoadAndSignNumber;	// CSD-030324 // thai2 YGI

extern bool g_allowDrop; //Eleval 22/05/09 for /drop command

////////////////////// SoundUp lkh 추가 ///////////////////////
extern int	BGM_SoundVolume;
extern BOOL	BGM_FadeInOut;

ORDERIFNO		g_OrderInfo;
int				g_OldOrderInfoCount;

static DWORD STDTime;
extern int NPCCOUNT;
extern	int			ListenCommand;		// network.cpp
extern	SystemInfo	system_info;
extern  int BetaTest;
int TestDeter;
DWORD	BeforeWalkClickTime, AfterWalkClickTime;
bool	DontMoveAttackFlag = false;
bool    CheckPC = false;

int NewOrderC = 0;
ORDERTABLE neworder[2000] = { 0, };

int PathBuildSuccess = 0;

bool HeroActionProcChecked;//  만약 마우스클릭에   HeroActionProc()이 호출되지 않았다면 DoLButtonUp()에서 호출하게 한다. 


const char ordermsg[6][30] = { "Body.Weapon.Shield",
								"Body.Shield.Weapon",
								"Weapon.Body.Shield",
								"Weapon.Shield,Body",
								"Shield.Weapon,Body",
								"Shield.Body,Weapon" };



int AutoAttackFlag;
int AutoAttackSpriteType;
LPCHARACTER AutoAttacklpcharacter;
int AutoAttackx, AutoAttacky;

bool ScreenCaptureStartFlag;

int YouCanHeroActionProc;
int YouCanViewTipsCheck;
int ViewTipToggle;		// 0818 khs

int Moveoldx, Moveoldy;	// 이동을 위해 마우스 클릭한 지점을 기억하고 있는다. 
DWORD Movetime;

LPCHARACTER MouseCheckCharacterName;  //  마우스에 의해 선택되는 





///////////////// 0430 lkh 추가 ////////////////////
BOOL	g_AttackSkill_Trace1 = FALSE;
BOOL	g_AttackSkill_Trace2 = FALSE;


int ChangeMapFlag;				// 0825 KHS
t_server_change_map ChangeMapPacket;


///////////////////////////////////////////////////////////////////////////////
//

BOOL		GameProc(LPDIRECTDRAWINFO lpDirectDrawInfo);
void		KeyboardProc(void);
void		MouseProc(void);

int			ProtocolProc(t_connection* c, bool flag);
void		CharacterProcCheck(void);
void		CharacterProcFrameCount(void);

void		DoLButtonDown(void);
void		DoLButtonUp(void);
void		DoRButtonDown(void);
void		DoRButtonUp(void);
void		DoLDButtonDown(void);
void		DoRDButtonDown(void);
void		DoLbuttonStill();

void		CalcOrder(void);
void		DisplaySprite(void);
void		DisplaySkillCursor(void);
void		DisplayTileFirst(void);
void		DisplaySpriteBottom(void);
void		DisplayShadowFirst(void);
void		DisplaySpriteCeiling(void);
void		DisplaySpriteTrans(void);
void		DisplaySpriteUnderTile(void);
void		DisplaySpriteNo(void);

void		_DisplayMapObjectAfter(void);

void		ViewAllCharacter(int x, int y);

void		SettingDestPoint(int x, int y);
void		PutDestPoint(void);
void		UnDisplayDestPoing(LPCHARACTER ch);


void		DoHeroAttack(int x, int y);

///////////////////// 0204 lkh 수정 /////////////////////////
bool		ReturnCheckObject(LPMAPOBJECT mo);
bool		GetBoardData(int board_Number, char* str_Data);
bool		GetDirectBoardData(int board_Number, DIRECTBOARDDATA* directboardData);
extern void	PutBoardMenu(int board_Number, char* str_Data);
extern void	PutDirectBoardMenu(const DIRECTBOARDDATA* directboardData);

extern void DisplayFightStone(void);
extern bool isOurSquad(LPCHARACTER hero, LPCHARACTER ch);		// 011018 LTS
extern void LocalWarInfoDisPlay();								// LTS LOCALWAR
extern void DrawParticleEffect();	// LTS DRAGON
extern void	CheckParticleEffect();	// LTS DRAGON

extern void ShowMonsterWarInfo();	// BBD 040311

///////////////////// SoundUp lkh 추가 /////////////////////////
void	ObjectSoundPlay(void)
{
	int soundNo = 0, soundFrame = 0, soundDelay = 0;
	int	soundCount = 0, soundOverlap = 0, soundType = 0;

	LPMAPOBJECT		mo;
	Spr* sp;

	for (int i = 0; i < TotalMapObject; i++)
	{
		mo = &Mo[i];
		int mox = mo->x;
		int moy = mo->y;

		if (IsScreenAreaView(mox, moy))
		{
			sp = &MoImage[mo->id - 1].spr[mo->curframe];


			soundNo = mo->soundno % 1000;

			if (soundNo > 0)
			{
				soundCount = mo->soundno / 1000;

				soundFrame = mo->soundframe % 1000;
				soundOverlap = mo->soundframe / 1000;		//중첩 효과음 출력을 위해

				soundDelay = mo->sounddelay % 1000;
				soundType = mo->sounddelay / 1000;		//세부 출력 방법

				if (mo->delay == 0)// || soundFrame==99)		//map_delay가 0인 경우 또는 무조건 중첩 루핑 출력인 경우
				{
					int sx = mo->x;
					int	sy = mo->y;

					//int distance_X = abs(Hero->x - sx)/(TILE_SIZE);		//거리를 타일 단위로 환산	// LTS SOUND
					//int distance_Y = abs(Hero->y - sy)/(TILE_SIZE);									// LTS SOUND

					int distance_X = sx - Hero->x;		//거리를 타일 단위로 환산	// LTS SOUND
					int distance_Y = sy - Hero->y;									// LTS SOUND
					int tempsx = sx;				// LTS SOUND
					int tempsy = sy;

					//if( sx < Hero->x)	distance_X = -distance_X;
					//if( sy < Hero->y)	distance_Y = -distance_Y;

					int mo_X = sx / TILE_SIZE;
					int	mo_Y = sy / TILE_SIZE;

					if (soundNo && (mo->curframe == soundFrame || soundFrame == 99))			//루핑 효과음이거나 사운드 출력 해당 프레임인 경우인 경우
					{
						BOOL	play = 1;

						if (soundType != 2 && soundDelay)		// type->random 출력
						{
							if ((soundCount) <= 0)
								mo->soundno = (mo->soundno % 1000) + soundDelay * 1000;		//soundcount set
							else
								play = 0;
							mo->soundno -= 1000;			//soundcount--
						}

						int	sound_Num = 0;

						BOOL	play_Rate = 0;//	= (mo->sounddelay%1000)/100;	//0->해당 프레임에서 무조건 출력 1->해당 프레임에서 해당 비율에 맞춰 출력

						int		probability = 0;

						switch (soundType)
						{
						case 1:		//3way
							sound_Num = soundNo + rand() % 3;
							break;
						case 2:		//확률에 의해 소리 출력
							play_Rate = 1;
							probability = soundDelay;
							sound_Num = soundNo;
							break;
						default:
							sound_Num = soundNo;
							break;
						}

						if (play)
						{
							if ((play_Rate && (rand() % 101 <= probability)) || !play_Rate)	//확률에 의해 출력되는 경우와 확률에 관계 없는 경우
							{
								if (soundOverlap)		//true->무조건 해당 프레임에서 출력(오브젝트의 프레임이 적은 경우 중첩됨)
									//PlayListAutoSounds( sound_Num, distance_X, distance_Y, 0 );	//3D sound로 표현 // LTS SOUND
									PlayListAutoSounds(sound_Num, distance_X, distance_Y, 0);	//3D sound로 표현 // LTS SOUND
								else					//false->출력 프레임에 상관없이 중첩되지 않게 
								{
									if (!IsPlaying(sound_Num))
										//PlayListAutoSounds( sound_Num, distance_X, distance_Y, 0 );
										PlayListAutoSounds(sound_Num, distance_X, distance_Y, 0);	//3D sound로 표현 // LTS SOUND
								}
							}
						}
					}
				}
			}
		}
	}
}

void DisplayDebug(void)
{
#ifdef _DEBUG				
	int i;
	ViewCounselerStatus();

	if (tool_ID_INPUT_MAP_ATTR)
	{
		Hcolor(0, 255, 0);
		Hprint2(10, 300, g_DestBackBuf, "Working Map Attr.");
		if (tMapAttrAllAffect == IDC_SOURCE_AFFECT_YES)
		{
			Hcolor(255, 0, 0);
			Hprint2(10, 320, g_DestBackBuf, "Effect to All Source");
		}

		switch (tMapAttr)
		{
		case IDC_ATTR_DONT:	Hcolor(255, 0, 0);
			Hprint2(10, 340, g_DestBackBuf, "Dont Attr");
			break;
		case IDC_ATTR_RIVER:	Hcolor(0, 100, 200);
			Hprint2(10, 340, g_DestBackBuf, "Watter Attr");
			break;
		case IDC_ATTR_GEFFECT:	Hcolor(0xffff);
			Hprint2(10, 340, g_DestBackBuf, "EffectWall Attr");
			break;
		}
	}

	Hcolor(0, 255, 0);
	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		Hprint2(400, 400, g_DestBackBuf, "%d ", Hero->toflag);
		if (tool_MouseCursorBlock)
		{
			Hprint2(10 - 1, 110 - 1, g_DestBackBuf, "CursorAni NO : %d ", CurMouseCursorNo);
			Hprint2(10 + 1, 110 + 1, g_DestBackBuf, "CursorAni NO : %d ", CurMouseCursorNo);
			Hcolor(255, 0, 0);
			Hprint2(10, 110, g_DestBackBuf, "CursorAni NO : %d ", CurMouseCursorNo);
			Hcolor(0, 255, 0);
		}
#ifdef _DEBUG // finito 060507
		if (SysInfo.notconectserver == 0)
		{
			Hcolor(0, 255, 0);
			Hprint2(200, 440, g_DestBackBuf, "R:%d S:%d", g_RecvBytes, g_SendBytes);
			Hprint2(200, 455, g_DestBackBuf, "0:%d 1:%d", connections.packet_count[0], connections.packet_count[1]);
		}
#endif
		if (SkillStatus)
		{
			switch (SkillStatus)
			{
			case SKILL_ON: Hprint2(200, 180, g_DestBackBuf, "Skill_ON");		break;
			case SKILL_ING:  Hprint2(200, 180, g_DestBackBuf, "Skill_ING");	break;
			}
		}

		LPCHARACTER ch = Hero;
		i = 1;
		int npcno = 0, pcno = 0;
		int itemno_ = 0;
		if (SysInfo.notconectserver == 0)
			while (ch != NULL)
			{
				if (ch->id >= 10000) npcno++;
				else pcno++;
				ch = ch->lpNext;
			}
		LPITEMGROUND titem = g_Item;
		while (titem)
		{
			itemno_++;
			titem = titem->next;
		}
		Hprint2(640, 7, g_DestBackBuf, "NPC: %d  PC:%d  Item:%d ", npcno, pcno, itemno_);

		if (tool_ViewAllCharacter)	ViewAllCharacter(0, 0);

		if (tool_IDD_INPUT_MAPOBJECT)
		{
			static int tt, tto;
			if (tto) { tt++; if (tt > 5) { tto = !tto; } }
			else { tt--; if (tt < -5) { tto = !tto; } }

			if (TempMoImage.spr[0].img)	PutCompressedImage(160, 300 + tt, &TempMoImage.spr[0]);
		}
	}

	if (SysInfo.viewcharacterframe)
	{
		h_DisplayPC_Temp(Hero);
		Hcolor(0xffff);
		Hprint2(10, 120, g_DestBackBuf, "Dir:%d  Frame:%d ", SpriteOrderDir, SpriteOrderFrame);
		Hprint2(10, 140, g_DestBackBuf, "%s", ordermsg[SpriteOrder[Hero->sprno][SpriteOrderDir][SpriteOrderFrame]]);
	}
#endif
}

BOOL StartMenuProc(LPDIRECTDRAWINFO lpDirectDrawInfo)
{	//< CSD-030806

	char* pTemp = NULL;//soto-HK;
	if (ProtocolProc(&connections) < 0)
	{
		return	FALSE;
	}

	if (!g_bIsActive)
	{
		goto SKIP__;
	}

	if (UpdateInputState() == DI_OK)
	{
		KeyboardProc();
		MouseProc();
	}

	if (!g_FrameMgr.CanNextFrame())
	{
		goto SKIP__;
	}
#ifndef _SDL
	if (g_DirectDrawInfo.lpDirectDraw == NULL)
	{
		if (!InitDirectDraw(g_hwndMain, &g_DirectDrawInfo))
		{
			return 0;
		}
	}

	g_DestBackBuf = GetSurfacePointer(g_DirectDrawInfo.lpDirectDrawSurfaceBack);
#else // !_SDL

	if (!InitSDL())
	{
		return 0;
	}
	g_DestBackBuf = SDL_GetSurfacePointer();
	#endif
	StartMenuChecking();
	StartMenuDisplay();
	g_nLButtonState = 0; g_nRButtonState = 0;
	g_nLDButtonState = 0; g_nRDButtonState = 0;
	ViewTips(); // 001030 KHS	
	CursorDisplayBack();
#ifndef _SDL
	FlipScreen(lpDirectDrawInfo);
#else // !_SDL

	SDL_FlipScreen();
	SDL_Draw();
#endif

	//<soto-HK	
	g_FrameMgr.DoFrameDelay(&pTemp);

	strcpy(pTemp, "c8");
	delete pTemp; pTemp = NULL;
	//>soto-HK

SKIP__:
	CheckingAccelator_Sub1();	// 020620 YGI
	return	TRUE;
}	//> CSD-030806
//010604 lsw	
extern DWORD ServerAccelTime;
extern void CheckAccelerator();
extern DWORD g_ClientTime_accel;
extern DWORD g_ClientTimeNoGap;		//011122 lsw

extern int RecvChangeMap(t_server_change_map* p);

BOOL GameProc(LPDIRECTDRAWINFO lpDirectDrawInfo)//030703 lsw
{
#ifdef USE_PROFILER	// 031013 kyo
	g_ProFileMgr.StartCounter("GameProc");
#endif 
	char* pTemp = NULL;//soto-HK
	g_OldOrderInfoCount = g_OrderInfo.count;
	g_OrderInfo.count = 0;
	LightCount = 0;
	HeroActionProcChecked = false;

	ViewCheckRoutine(0);

	DWORD time = ::timeGetTime();
	g_ClientTime = time + g_ServerClientTimeGap;

	CrackMgr.AutoCheckAccel();
	CheckingAccelator_Sub1();

	if (ProtocolProc(&connections, false) < 0) { return FALSE; }			// 010612 YGI

	ViewCheckRoutine(10);

	if (ChangeMapFlag)
	{
		if (RecvChangeMap(&ChangeMapPacket) == FALSE)
		{
			return FALSE;
		}
	}

	if (CheckSumError)
	{
		SendHexaEditing(CheckSumError, Hero->id);
		PostMessage(g_hwndMain, WM_CLOSE, 0, 0);
	}

	ViewCheckRoutine(20);

	if (g_GameInfo.displayOptionDelay > 0)
	{
		if (g_GameInfo.displayOption == DISPLAY_EARTHQUAKE)
		{
			g_Map.oX = (rand() % 10) - 5;
			g_Map.oY = (rand() % 10) - 5;
		}
		g_GameInfo.displayOptionDelay--;
		if (g_GameInfo.displayOptionDelay <= 0) 		g_GameInfo.displayOption = 0;
	}

	///////////////// 0604 lkh 추가(만약 마우스 큐에 가용한 데이터 값이 있는 경우: 처리후 리턴) //////////////
	if (g_queue.lb && g_queue.ch != Hero)	//왼쪽 버튼이 눌린 큐 데이터
	{
		if (g_queue.ch == g_GameInfo.lpcharacter && g_queue.ch != NULL && g_GameInfo.lpcharacter != NULL)
		{
			if (!(Hero->animationType == ACTION_ATTACKED || Hero->animationType == ACTION_DIE ||
				Hero->animationType == ACTION_NONE || Hero->animationType == ACTION_BASIC_WALK) && !IsDead(g_queue.ch))
			{
				int minrange, maxrange;
				minrange = AttackRangeTable[Hero->sprno][Hero->accessory[2]][0];
				maxrange = AttackRangeTable[Hero->sprno][Hero->accessory[2]][1];

				int minormax = IsAttackRange(&minrange, &maxrange, Hero, g_queue.ch->x, g_queue.ch->y);
				if (minormax == ATTACKRANGE_IN_AREA_)
				{
					g_mgrBattle.SendPcAttack(Hero, g_queue.ch);
					ZeroMemory(&g_queue, sizeof(t_queue));
					goto	JUMP_SKIP;
				}
			}
		}
	}

	ViewCheckRoutine(30);

	if (UpdateInputState() == DI_OK)
	{
		KeyboardProc();
		MouseProc();
	}

	ViewCheckRoutine(40);

JUMP_SKIP:

	CharacterProcCheck();

	ViewCheckRoutine(50);

	CircleMagic();
	EffectProc();
	DrawItemList();
	OutputNumberAni();
	OutputSkillGauge();

	CheckTime_SkillMsg();

	ViewCheckRoutine(60);

	CalcOrder();

	if (!tool_ViewMenu)
		MenuChecking();     //메뉴 구성요소의 충돌체크MenuChecking();     

	g_nLButtonState = 0;	g_nRButtonState = 0;
	g_nLDButtonState = 0;	g_nRDButtonState = 0;

	RainAnimationSetting();
	SnowAnimationSetting();
	CurrentMsgMgr.CheckCurrentStatusMessage();

	ViewCheckRoutine(70);

	OutputSkillCursor();
	CheckEventAttrTile();

	// SoundUp 
	BackEffectSound2(MapNumber);
	PlayRainSound();
	PlayThunder(); //Eleval 12/08/09 - To play thunder sound if thunderstorm

	ViewCheckRoutine(80);

	CheckHpUpDown();

	if (ProtocolWritingProc(&connections) <= 0)
	{
		// todo: add disconnection message menu here! finito 09/05/08
		return FALSE;
	}

	gr.GuideGhost_PreSet();

	if (!g_FrameMgr.CanNextFrame())
	{
		goto SKIP__;
	}

	if (!g_bIsActive)
	{	//< CSD-030723
		goto SKIP__;
	}	//> CSD-030723
#ifndef _SDL
	g_DestBackBuf = GetSurfacePointer(g_DirectDrawInfo.lpDirectDrawSurfaceBack);
#else // !_SDL
	g_DestBackBuf = SDL_GetSurfacePointer();
#endif
#ifdef  _DEBUG



	if (SysInfo.notconectserver || tool_DrawMap)	EraseScreen(lpDirectDrawInfo, RGB(0x00, 0x00, 0x00));
#endif //  _DEBUG
	curr_direct_draw_info = lpDirectDrawInfo;		// 1027 YGI
	CursorStatus();
	ViewCheckRoutine(100);

	DisplaySpriteUnderTile();
	MapDisplay(0);
	ViewCheckRoutine(110);
	DrawSkilltile();

	DisplaySpriteBottom();
	PutDestPoint();
	DisplayShadowFirst();
	MapDisplay(2);

	ViewCheckRoutine(120);

	DisplaySprite();

	MapDisplay(1);


	ViewCheckRoutine(130);

	DisplaySpriteCeiling();

	DisplaySkillCursor();

	ViewCheckRoutine(140);

	DisplaySpriteTrans();

	ViewCheckRoutine(150);

	MapDisplay_Attr();

	DisplayFightStone();

	_DisplayMapObjectAfter();

	ViewCheckRoutine(160);

	ViewRoofData();
	RainAnimationOutput();
	SnowAnimationOutput();

	DisplaySpriteNo();

	ViewCheckRoutine(170);

	CheckParticleEffect();	// LTS DRAGON
	DrawParticleEffect();	// LTS DRAGON

	MapBright();

	ViewCheckRoutine(180);

	DisplayChatAndNumber();

	LocalWarInfoDisPlay();		// LTS LOCALWAR
	ShowMonsterWarInfo();		// BBD 040311

	DrawClock(); //마을 맵인 경우 // finito 06/07/07 altered the function to draw the current server time.
	DrawDaysofWeek();
	DrawPing();
	//drawfps();
	//BuffTimeStruct buff;
	ShowBuffTimer();
	if (!tool_ViewMenu)
		MenuDisplay();     //메뉴 구성요소의 충돌체크MenuChecking();     //메뉴 구성요소의 충돌체크MenuDisplay();      //메뉴의 활성여부를 판단해 출력하는 함수	

	gr.GuideGhost_OutPut();

	CurrentMsgMgr.ViewCurrentStatusMessage();

	ViewTips();

	Check_Beginer_BBS();

	Display_Beginer_BBS();

	ViewCheckRoutine(210);

	ViewLoadMask();

	DisplayDebug();

	ViewCheckRoutine(220);

	_DisplayDebugCharInfo();

	CursorDisplayBack();

	ViewCheckRoutine(230);

	//	TxtOut.RcTXTOut(100,100,100,1,"dwFrameSkipGab:[%d]",dwFrameSkipGab);
#ifdef USE_PROFILER	// 031013 kyo
	g_ProFileMgr.StopCounter("GameProc");
	g_ProFileMgr.DrawCounter(g_ProFileType);
#endif 	
#ifndef _SDL
	FlipScreen(lpDirectDrawInfo);
#else // !_SDL
	SDL_FlipScreen();
	SDL_Draw();
#endif

	//<soto-HK
	g_FrameMgr.DoFrameDelay(&pTemp);
	strcpy(pTemp, "c8");
	delete pTemp; pTemp = NULL;
	//>soto-HK

SKIP__:

	CharacterProcFrameCount();
	CheckTrap();

	///////////// SoundUp lkh 추가 /////////////
	ObjectSoundPlay();
	WeatherFXSound();
	//021030 YGI
	StartSound(0);
	//BGMFadeInOut();
	////////////////////////////////////////////

	CheckEffectSpriteFree(); //필요없어진 이펙트 스프라이트 날리기

	CPrison g_cPrison;
	CheckAutoWave();
	
	static DWORD dwStartTemp = 0;
	static DWORD dwEndTemp = g_curr_time;
	if (true == g_cPrison.IsBadUser())
	{
		g_cPrison.SendUpdateImprisonTerm();
		if ((dwEndTemp - dwStartTemp) > 0)//180
		{
			g_cPrison.SendUpdateImprisonTerm();
			dwStartTemp = dwEndTemp;
		}
		dwEndTemp = g_curr_time;
	}

	return	TRUE;
	
}

//////////////////////////////////////////////////////////////////////////////

BOOL GameTestProc(LPDIRECTDRAWINFO lpDirectDrawInfo)
{
	static DWORD tick;
	static DWORD oldtick = ::timeGetTime();
	static DWORD deltatick;
	static int tickcount = 0;
	static int skip = 65;

	LPCHARACTER ch = NULL;

	if (SysInfo.notconectserver == 0) // normal 이면.
	{
		if (ProtocolProc(&connections) < 0)
		{
			return	FALSE;
		}
	}

	if (UpdateInputState() == DI_OK)
	{
		MouseProc();
	}

	g_OrderInfo.count = 0;
	LightCount = 0;

	CharacterProcCheck();
	EffectProc();
	DrawItemList();
	CalcOrder();
	MenuChecking();     //메뉴 구성요소의 충돌체크MenuChecking();     //메뉴 구성요소의 충돌체크MenuDisplay();      //메뉴의 활성여부를 판단해 출력하는 함수

	g_nLButtonState = 0;
	g_nRButtonState = 0;
	g_nLDButtonState = 0;
	g_nRDButtonState = 0;

	RainAnimationSetting();
	SnowAnimationSetting();
	CurrentMsgMgr.CheckCurrentStatusMessage();
	CheckEventAttrTile();

	tick = ::timeGetTime();
	deltatick = tick - oldtick;
	tickcount += deltatick;

	if (tickcount > skip)
	{	//< CSD-031014	
		oldtick = ::timeGetTime();
		tickcount -= skip;
	}	//> CSD-031014
	else
	{
		oldtick = ::timeGetTime();
		tickcount -= skip + 2;
	}
	//-------------------------------------------------
#ifndef _SDL
	if (g_DirectDrawInfo.lpDirectDraw == NULL)
	{
		if (!InitDirectDraw(g_hwndMain, &g_DirectDrawInfo)) return  FALSE;
	}

	g_DestBackBuf = GetSurfacePointer(g_DirectDrawInfo.lpDirectDrawSurfaceBack);
#else // !_SDL


	
	g_DestBackBuf = SDL_GetSurfacePointer();

#endif
	
	//	MDrawObject( lpDirectDrawInfo );
	DisplaySpriteUnderTile();
	MapDisplay(0);
	DisplaySpriteBottom();
	MapDisplay(2);
	DisplaySprite();
	MapDisplay(1);
	DisplaySpriteCeiling();
	DisplaySpriteTrans();

	DisplaySpriteNo();

	MapDisplay_Attr();
	_DisplayMapObjectAfter();
	ViewRoofData();

	RainAnimationOutput();
	SnowAnimationOutput();

	ch = Hero->lpNext;
	if (ch && tool_ViewAnimationFrame)
	{
		if (tool_AnimationEditCurFrame - 1 >= 0)
			PutCompressedImage(300, 150, &CharSpr[ch->sprno].sp[(int)ch->direction][tool_AnimationEditCurFrame - 1]);
		Hprint2(300, 30, g_DestBackBuf, "%d", tool_AnimationEditCurFrame - 1 + 1);
		PutCompressedImage(400, 150, &CharSpr[ch->sprno].sp[(int)ch->direction][tool_AnimationEditCurFrame]);
		Hprint2(400, 30, g_DestBackBuf, "%d", tool_AnimationEditCurFrame + 1);
		if (tool_AnimationEditCurFrame + 1 < tool_AnimationEditTotalSpriteNo / 8)
			PutCompressedImage(500, 150, &CharSpr[ch->sprno].sp[(int)ch->direction][tool_AnimationEditCurFrame + 1]);
		Hprint2(500, 30, g_DestBackBuf, "%d", tool_AnimationEditCurFrame + 1 + 1);
	}

	//	MapCurtain();
	MapBright();

	MenuDisplay();     //메뉴 구성요소의 충돌체크MenuChecking();     //메뉴 구성요소의 충돌체크MenuDisplay();      //메뉴의 활성여부를 판단해 출력하는 함수	

	ViewVersion(g_GameInfo.version);

	//	CursorDisplay( &MouseCursorFlag );	
	CurrentMsgMgr.ViewCurrentStatusMessage();
	DisplayDebug();


	CursorDisplayBack();


#ifndef _SDL
	FlipScreen(lpDirectDrawInfo);
#else // !_SDL
	SDL_FlipScreen();
	SDL_Draw();
#endif
	
	while (((::timeGetTime()) - tick) < (DWORD)skip) {}

	//SKIP__:

	CharacterProcFrameCount();

	//	if( SkipTime - STDTime < 60 )
	//	{
	//		while( 1 )	
	//		{
	//			SkipTime = ::timeGetTime();
	//			if( SkipTime - STDTime > 60 ) 
	//			{
	//				STDTime = SkipTime;
	//				break;
	//			}		
	//		}			
	//	}
	//	else
	//	{
	//		STDTime = SkipTime;
	//	}	

	if (Hero->nCurrentAction < 0 || Hero->nCurrentAction > 23)
	{
		_asm nop;
	}

	return	TRUE;
}






static int	effect_Test = 1;
///////////////// 0501 lkh 수정 /////////////////
void KeyboardProc(void)
{
	/** Added by Tya : shift prevents screen from moving, to allow editing **/
	//bool canMove = (g_aCurrentKeys[DIK_RSHIFT] & 0x80 || g_aCurrentKeys[DIK_LSHIFT] & 0x80) == FALSE;
	//int foc = EWndMgr.GetFocusWndIndex();

	//if(foc == HWND_MAIL1 || foc==HWND_MAIL2 || foc==HWND_MAIL3)
	//	canMove = false;

	if (SysInfo.viewcharacterframe) return;

	if ((g_aCurrentKeys[DIK_LEFT] & 0x80))
	{
		MoveScreen(DIRECTION_LEFT);
	}
	if ((g_aCurrentKeys[DIK_RIGHT] & 0x80))
	{
		MoveScreen(DIRECTION_RIGHT);
	}
	if ((g_aCurrentKeys[DIK_UP] & 0x80))
	{
		MoveScreen(DIRECTION_UP);
	}
	if ((g_aCurrentKeys[DIK_DOWN] & 0x80))
	{
		MoveScreen(DIRECTION_DOWN);
	}
	if (!IsChatBoxActive())//채팅 모드가 아니면
	{
		if (g_aCurrentKeys[DIK_SPACE] & 0x80)
		{
			SetHeroPosition(1);		// 0110
		}
	}

	static bool callskilltable0;
	static bool callskilltable1;

	if ((g_aCurrentKeys[DIK_LSHIFT] & 0x80) || (g_aCurrentKeys[DIK_RSHIFT] & 0x80))
	{
		DontMoveAttackFlag = true;
	}
	else
	{
		DontMoveAttackFlag = false;
	}

	if ((g_aCurrentKeys[DIK_LCONTROL] & 0x80) || (g_aCurrentKeys[DIK_RCONTROL] & 0x80))
		CheckPC = true;
	else
		CheckPC = false;
}


void MouseProc(void)
{
	//	if( StartMapFlag )
	//	{
	//		StartMapFlag = 0;
	//		g_Map.tox = g_Map.x = StartMapx;
	//		g_Map.toy = g_Map.y = StartMapy;
	//	}


	if (tool_ID_INPUT_MAP_ATTR)
	{
		if (tMapAttr_Offset_Modify == IDC_ATTR_OFFSET_YES2)
		{
			if (LButtonDownIng)
			{
				tMapAttrX = Mox / TILE_SIZE;
				tMapAttrY = Moy / TILE_SIZE;
				return;
			}
		}
		else
		{
			if (LButtonDownIng) { tool_InputMapAttr(Mox / TILE_SIZE, Moy / TILE_SIZE, 1); return; }
			if (RButtonDownIng) { tool_InputMapAttr(Mox / TILE_SIZE, Moy / TILE_SIZE, 0); return; }
		}
	}
	if (Hero)	SetView(Hero, Hero->sight);



	if (SysInfo.dx == 0)
	{
	}
	else if (g_StartMenuOn == false) // 게임내에 들어왔을때, 
	{
		int mousechange = 0;

		if (g_pointMouseX < 2)					if (MoveScreen(DIRECTION_LEFT))	mousechange = 1;	// 왼쪽..
		if (g_pointMouseX > SCREEN_WIDTH - 2)		if (MoveScreen(DIRECTION_RIGHT)) mousechange = 1;   // 오른쪽..
		if (g_pointMouseY < 2)					if (MoveScreen(DIRECTION_UP))	mousechange = 1;	// 위..
		if (g_pointMouseY > SCREEN_HEIGHT - 2)	if (MoveScreen(DIRECTION_DOWN))	mousechange = 1;	// 아래..

		if (g_pointMouseX < 2)
		{
			if (g_pointMouseY < 2) { PushMouseCursor();	CursorNo(11); }
			else if (g_pointMouseY > SCREEN_HEIGHT - 2) { PushMouseCursor();	CursorNo(9); }
			else { PushMouseCursor();	CursorNo(10); }
		}
		else if (g_pointMouseX > SCREEN_WIDTH - 2)
		{
			if (g_pointMouseY < 2) { PushMouseCursor();	CursorNo(13); }
			else if (g_pointMouseY > SCREEN_HEIGHT - 2) { PushMouseCursor();	CursorNo(15); }
			else { PushMouseCursor();	CursorNo(14); }
		}
		else
		{
			if (g_pointMouseY < 2) { PushMouseCursor();	CursorNo(12); }
			else if (g_pointMouseY > SCREEN_HEIGHT - 2) { PushMouseCursor();	CursorNo(8); }
			else										PopMouseCursor();
		}
	}

	//----------------------

	Mapx = g_Map.x * TILE_SIZE - g_Map.oX;
	Mapy = g_Map.y * TILE_SIZE - g_Map.oY;

	Mox = Mapx + g_pointMouseX;
	Moy = Mapy + g_pointMouseY;

	//	if( AutoAttackFlag )
	////	{
		////	AutoAttackFlag = 0;
	//		g_GameInfo.nSelectedSpriteType = AutoAttackSpriteType;
	//		g_GameInfo.lpcharacter = AutoAttacklpcharacter;
	//
	//		DoHeroAttack( AutoAttackx, AutoAttacky );
	//		return;
	//	}



	switch (g_nLButtonState)
	{
	case	STATE_BUTTON_PRESSED:
		DoLButtonDown();		break;
	case	STATE_BUTTON_RELEASED:		DoLButtonUp();			break;
	case	STATE_BUTTON_STILL:	DoLbuttonStill(); break;
	}

	switch (g_nRButtonState)
	{
	case	STATE_BUTTON_PRESSED:		DoRButtonDown();		break;
	case	STATE_BUTTON_RELEASED:		DoRButtonUp();			break;
	}
	if (g_nLDButtonState == STATE_BUTTON_DOUBLECLICK)	DoLDButtonDown();
	if (g_nRDButtonState == STATE_BUTTON_DOUBLECLICK)	DoRDButtonDown();
}


//1206 zhh
#include "CReadArgument.h"

// LogIn Server에 접근...
int CheckAgentServer(t_connection* c, char* id, char* password)
{//021021 lsw
	int ret = 0;
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver == 1)
	{
		goto __CheckSuccss;
	}
#endif
#ifndef JAPAN_LOCALIZING_
	if (!IsASCII(id))
	{
		ret = -9;
		goto __CheckFail;
	}
#endif
	//001215 zhh
	if (g_pBill->ReturnLineNum() < 0)
	{
		ret = GetAgentServerIPAddress(c);
		if (ret != 1)
		{
			goto __CheckFail;
		}
	}
	else
	{
		strcpy(g_GameInfo.agent_ip, g_pBill->ReturnIPaddress());
		g_GameInfo.agent_port = g_pBill->ReturnPORTNumber();
	}

	Release(c);
	if (ConnectServer(c, g_GameInfo.agent_ip, g_GameInfo.agent_port) <= 0)
	{
		ret = -1;
		goto __CheckFail;
	}

	//1206 zhh
	if (g_pBill->ReturnLineNum() >= 0)
	{
		g_pBill->SendSpecifiedMessage(g_hwndMain);
	}
	goto __CheckSuccss;

__CheckSuccss://성공 하면
	{
		ret = 1;
		strcpy(IIDD, sId);//아뒤 패스를 넣고
		strcpy(PPWW, sPassword);
		goto __ClearID_PASSWORD;
	}
__CheckFail://성공 하면
	{
		strcpy(IIDD, "");
		strcpy(PPWW, "");
		goto __ClearID_PASSWORD;
	}
__ClearID_PASSWORD:
	{
		strcpy(sId, "");//아뒤 패스 입력되어있는거 지우고
		strcpy(sPassword, "");
		EWndMgr.ClearAllTxt();//Hwnd에 있는 메세지도 지운다
		return ret;
	}
}

/////////////////////////////////  0131 YGI 베타 테스트 아이디 체크 ////////////////////
int CheckBetaTestId(char* id)
{
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver == 1) return 1;
#endif
	LoadGameInfo(); //  LogIn 서버에 접속할 IP/Port를 읽는다.


	if (connections.state == CONNECT_FAIL)
		if (ConnectServer(&connections, g_GameInfo.proxy_ip[0], g_GameInfo.proxy_port) < 0)
		{
			return	-1;
		}

	double		StartTime, Duration;
	t_packet	packet;
	packet.h.header.type = CMD_CHECK_BETA_TEST;
	{
		memcpy(packet.u.kein.beta_check_id.id, id, ID_LENGTH);
	}
	packet.h.header.size = sizeof(k_beta_check_id);
	QueuePacket(&packet, 1);

	StartTime = ::timeGetTime();
	ListenCommand = 0;

	while (1)
	{
		Duration = ::timeGetTime() - StartTime;
		if (Duration > WAIT_TIME_RESPONSE_DURATION) //030930 kyo
		{
			return(-2);
		}

		if (HandleRunning(&connections) <= 0)
		{
			Release(&connections);
			return(-3);
		}

		if (ListenCommand == 100)		// 정상 연결
		{
			break;
		}
		else if (ListenCommand == 10)
		{
			return(-4);			// 베타 테스터가 아니다.
		}
	}

	return 1;
}
////////////////////////////////////////////////////////////////////////////////
int ProtocolProc(t_connection* c, bool flag)
{
	if (HandleRunning(c, flag) <= 0)
	{
		Release(c);
	}

	return	0;
}


int ProtocolWritingProc(t_connection* c)
{
	int ret;
	struct timeval	tv;
	fd_set			wfds;

	if (!c->socket)	return(1);
	if (c->outbuf == NULL) return 1;

	FD_ZERO(&wfds);
	FD_SET(c->socket, &wfds);


	tv.tv_sec = 0;
	tv.tv_usec = 0;
	ret = select(c->socket, NULL, &wfds, NULL, &tv);

	if (ret)
		ret = HandleWriting(c);

	if (ret < 0)
	{
		Release(c);
		return 0;
	}

	return 1;
}



void CharacterProcCheck(void)
{
	LPCHARACTER		tempch, ch = Hero;
	int sprc;

	//	if( ch != NULL)						
	//		if( IsDead( ch ) && ch->viewtype != VIEWTYPE_GHOST_ )				
	//			if( ch->nCurrentAction != ACTION_DIE )	{ ch->nCurrentAction = ACTION_DIE; ch->nCurrentFrame.PutEncrypted(0); }

	while (ch != NULL)
	{
		switch (ch->animationType)
		{
		case ANIMATION_TYPE_TEST:
			AnimationTypeTestCheck(ch);
			break;

		case ANIMATION_TYPE_MAN:
		case ANIMATION_TYPE_WOMAN:
			AnimationTypeWomanCheck(ch);
			break;

		case ANIMATION_TYPE_MON1:
			if (ch->dieani == 1)
			{
				tempch = ch->lpNext;
				DeleteCharacter(&g_CharacterList, ch);
				ch = tempch;
				continue;
			}

			AnimationType_Mon1_Check(ch);
			Check_NPC_Pattern(ch);
			break;
		}
		// 주인공이 집안으로 들어가거나 나와서 g_Inside값이 바뀔수 있다. 
		if (ch == Hero)
		{
			DWORD	mx = ch->position.x;
			DWORD	my = ch->position.y;
			g_Inside = (TileMap[mx][my].attr_inside == 1);
		}

		sprc = ch->AnimationTable[ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
		ch->height = ch->AnimationTable[ch->nCurrentAction].height[ch->nCurrentFrame.GetDecrypted()];
		ch->sp = &CharSpr[ch->sprno].sp[ch->direction][sprc];

		if (ch->flagattacked > 0)	//  서버에서 죽는 플레그가 넘어오면 죽어야 한다. 
		{
			ch->flagattacked--;
			if (ch->flagattacked < 1)
			{
				if (Hero == ch)
				{
					SCharacterData.nCharacterHP = 0;
					Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
				}
				ch->hp = 0;
			}
		}

		//  마우스에 의해 Check되었을경우  이름을 보여주여야 하는데..   MouseCheckCharacterName가 Setting되므로 이를 이용하여 이름을 출력한다.  약간의 Delay를 주어 깜박이지 않게 한다. 
		if (ch->viewtype != VIEWTYPE_TRANSPARENCY_) // 투명인간 마법을 쓰면 이름또한 보여주지 않는다.
			if (ch == MouseCheckCharacterName)
			{
				if (ch->namedisplaycount == 0)
				{
					if (ch != Hero)
					{
						int	auto_Wave = 0;
						int DistanceX = (ch->x - Hero->x)/* / TILE_SIZE*/;	// LTS SOUND
						int DistanceY = (ch->y - Hero->y)/* / TILE_SIZE*/;	// LTS SOUND

						//////////////////////// 0810 lkh 마우스 커서를 올려 놨을때 자음 효과음 물리기 ////////////////////////
						switch (ch->animationType)
						{
						case ANIMATION_TYPE_MAN:
						case ANIMATION_TYPE_WOMAN:
							if (ch->sex)
								auto_Wave = 70 + rand() % 3;
							else	auto_Wave = 75 + rand() % 3;

							PlayListAutoSounds(auto_Wave, DistanceX, DistanceY, 0);	//3D sound로 표현 // LTS SOUND
							//PlayListAutoSounds(auto_Wave, DistanceX,DistanceY,0);	// LTS SOUND
							break;
						case ANIMATION_TYPE_MON1:
							//	auto_Wave = ReturnWaveNatural( ch );
							//	PlayListAutoSounds(auto_Wave, 0,6,0);
							break;
						default:
							break;
						}
					}
				}
				if (ch->namedisplaycount < 30) ch->namedisplaycount = 30;
			}
			else
			{
				if (ch->namedisplaycount > 0) ch->namedisplaycount--;
			}

		if (ch->animationType == ANIMATION_TYPE_MON1)
		{
			ch->dirdelay = !ch->dirdelay;
			if (ch->dirdelay == 0)
			{
				int todir = (int)ch->todir;
				int dir = (int)ch->direction;
				if (todir - dir > 0)
				{
					if (abs(todir - dir) <= 3) { dir++; dir %= 8; }
					else { dir += 7; dir %= 8; }
				}
				else if (todir - dir < 0)
				{
					if (abs(dir - todir) <= 4) { dir += 7; dir %= 8; }
					else { dir++; dir %= 8; }
				}

				ch->direction = (DIRECTION)dir;
			}
		}

		ch = ch->lpNext;
	}

	if (LButtonDownIng == 1)
	{
		if (Hero != NULL)	// 게임안에 들어왔다면..				
		{
			if (Hero->peacests == 0 && SMenu[MN_PARTY].bActive == true)// 파티를 구성하기 위한 
			{
				if (g_GameInfo.lpcharacter == g_GameInfo.lpoldcharacter)
				{
					g_GameInfo.PopupFaceCount++;
					if (g_GameInfo.PopupFaceCount == 15)
					{
						switch (g_GameInfo.nSelectedSpriteType)
						{
						case SPRITETYPE_ON_THE_CEILING_CHAR:
						case SPRITETYPE_CHARACTER:
						case SPRITETYPE_NPC:
						case SPRITETYPE_MONSTER:		g_MouseItemType = PARTY;
							SetMouseCh(g_GameInfo.lpcharacter);	// 0929 YGI
							break;
						default:
							break;
						}
					}
				}
				else g_GameInfo.PopupFaceCount = 0;
				{
				}

			}
		}
	}
	else
	{
		g_GameInfo.PopupFaceCount = 0;
	}

	g_GameInfo.lpoldcharacter = g_GameInfo.lpcharacter;
}


void  CharacterProcFrameCount(void)
{
	LPCHARACTER		ch = Hero;

	while (ch != NULL)
	{
		switch (ch->animationType)
		{
		case ANIMATION_TYPE_TEST:
			AnimationTypeTestFrameCount(ch);
			break;

		case ANIMATION_TYPE_MAN:
		case ANIMATION_TYPE_WOMAN:
			AnimationTypeWomanFrameCount(ch);
			break;

		case ANIMATION_TYPE_MON1:
			AnimationType_Mon1_FrameCount(ch);
			break;
		}

		ch = ch->lpNext;
	}

}





void AttackProc(LPCHARACTER ch, int minr, int maxr)
{
	LPCHARACTER dch;
	LPITEMGROUND i;
	int ani;
	//	캐릭터를 선택 물리적인 공격...


	switch (ch->animationType)
	{
	case ANIMATION_TYPE_TEST:
		break;

	case ANIMATION_TYPE_MAN:
	case ANIMATION_TYPE_WOMAN:		if (ch->changeActionFlag == 0) // 다른 공격동작을 하고 있지 않다면,
	{
		switch (ch->nAttackedType)
		{
		case SPRITETYPE_ITEM:
		{
			i = (LPITEMGROUND)ch->lpAttacked;

			ChangeDirection(&ch->direction, ch->x, ch->y, i->x, i->y);
			ch->todir = ch->direction;
			ani = AttackAction(ch);
			if (ani == ch->nCurrentAction)	return;
			else  ch->nCurrentAction = ani;
			ch->nCurrentFrame.PutEncrypted(0);
		}
		break;


		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_MONSTER:
		{
			ch->attackMinRange = minr;
			ch->attackMaxRange = maxr;


			dch = (LPCHARACTER)(ch->lpAttacked);
			ani = AttackAction(ch);
			if (ani == ch->nCurrentAction)	return;
			else  ch->nCurrentAction = ani;
			ChangeDirection(&ch->direction, ch->x, ch->y, dch->x, dch->y);
			ch->todir = ch->direction;
			ch->nCurrentFrame.PutEncrypted(0);
		}
		break;
		}
	}
							 break;

	case ANIMATION_TYPE_MON1:		if (ch->changeActionFlag == 0) // 다른 공격동작을 하고 있지 않다면,
	{
		ch->attackMinRange = minr;
		ch->attackMaxRange = maxr;
		dch = (LPCHARACTER)(ch->lpAttacked);
		ChangeDirection(&ch->direction, ch->x, ch->y, dch->x, dch->y);
		ch->todir = ch->direction;
		ani = AttackAction(ch);
		if (ani == ch->nCurrentAction)	return;
		else  ch->nCurrentAction = ani;
		ch->nCurrentFrame.PutEncrypted(0);
	}
							break;
	}


}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////			
//////			
//////			Mouse 처리....
//////			
//////			

/* // 이부분은 Server에서 허락을 받고 이동한다.
if ( Hero->nCurrentAction != ACTION_RUN )
{
Hero->nCurrentAction = ACTION_RUN;
Hero->nCurrentFrame.PutEncrypted(0);
}
*/
// type : 0- walk..
// type : 1- run..



void DoHeroMove(int movetype, int x, int y)
{
	int mx = x / TILE_SIZE;
	int my = y / TILE_SIZE;
	CHARACTER t;
	DWORD temptime;

	static int oldmovetype;

	if (DontMoveAttackFlag == TRUE)
		return;

	if (Hero->nCurrentAction == 17)		// LTS 011214 LTS			// 앉은 상태에서 방향을 바꾸어 준다.
	{
		/*AddCurrentStatusMessage( 255,255,0,"앉기 상태에서는 움직일수 없습니다.");
		switch(Hero->direction)
		{
		case DIRECTION_UP			: Hero->direction=DIRECTION_RIGHTUP;	break;
		case DIRECTION_RIGHTUP		: Hero->direction=DIRECTION_RIGHT;		break;
		case DIRECTION_RIGHT		: Hero->direction=DIRECTION_RIGHTDOWN;	break;
		case DIRECTION_RIGHTDOWN	: Hero->direction=DIRECTION_DOWN;		break;
		case DIRECTION_DOWN			: Hero->direction=DIRECTION_LEFTDOWN;	break;
		case DIRECTION_LEFTDOWN		: Hero->direction=DIRECTION_LEFT;		break;
		case DIRECTION_LEFT			: Hero->direction=DIRECTION_LEFTUP;		break;
		case DIRECTION_LEFTUP		: Hero->direction=DIRECTION_UP;			break;
		}
		t_packet p;
		p.h.header.type=CMD_SITDOWN_DIRECTION_CHANGED;
		p.u.LocalWar.SDChanged.Direction=Hero->direction;
		p.h.header.size=sizeof(t_SDChanged);
		QueuePacket(&p,1);*/
		return;	// LTS SITDOWN
	}
	//if( !SysInfo.gamemakemode )

	PathBuildSuccess = 0;

	if (SkillStatus == SKILL_OFF)
	{
	}
	else
	{
		return;
	}

	if (IsDead(Hero) && Hero->viewtype != VIEWTYPE_GHOST_)
	{
		if (!Hero->sex)		//여자
			PlayListAutoSounds(80, 0, 0, 0); // 이동실패.
		else
			PlayListAutoSounds(81, 0, 0, 0); // 이동실패.
		AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(5, 20));//010216 lsw
		return;
	}
	else
		if (SCharacterData.nCharacterSP <= 0 && !IsDead(Hero))
		{
			//	0910 YGI
			//		if(!Hero->sex)		//여자
			//			PlayListAutoSounds( 80,0,0,0 ); // 이동실패.
			//		else
			//			PlayListAutoSounds( 81,0,0,0 ); // 이동실패.
			AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 3));//010215 lsw
		}

	if (TileMap[Hero->x / TILE_SIZE][Hero->y / TILE_SIZE].attr_dont) return;		// 돈트박스면 못움직인다. 

	if (!TileMap[mx][my].attr_dont && FreeTile(Hero, mx, my, mx, my))
	{
		//		if( IsTooNearCharacter( x, y, Hero ) )	
		//		{
		//			PlayListAutoSounds( 10,0,0,0 ); // 이동실패.
		//			return; // 공격범위보다 안쪽으로 가려한다면..
		//		}
		//		달리는도중 500ms전에 마우스가 클릭되면 

		temptime = ::timeGetTime();//::timeGetTime();	
		if (temptime - Movetime < 300)
		{
			return;
		}
		if (Moveoldx == mx && Moveoldy == my)
		{
			if (mx != Hero->x / TILE_SIZE || my != Hero->y / TILE_SIZE)
				return;
		}

		//		
		//			if( oldmovetype == 1 )
		//			{
		//				movetype = 1;
		//				if( oldx == mx && oldy == my ) return;
		//			}
		//			else
		//			{
		//			}

		Movetime = temptime;
		Moveoldx = mx;
		Moveoldy = my;

		memcpy(&t, Hero, sizeof(CHARACTER));
		t.moveFlag = TRUE;
		t.movetype = movetype;
		t.gox = x;
		t.goy = y;
		t.destx = mx, t.desty = my;
		if (PathBuild(&t))
		{
			SettingDestPoint(x, y);

			PathBuildSuccess = 1;

			if (Hero)
				//			if( SysInfo.notconectserver )
			{
				Hero->moveFlag = TRUE;
				Hero->gox = x;
				Hero->goy = y;
				Hero->movetype = movetype;

				memcpy(Hero, &t, sizeof(CHARACTER));

				if (movetype == 0)
					if (Hero->pathcount > 5) movetype = 1;

				switch (movetype)
				{
				case 0:	if (Hero->nCurrentAction != Hero->basicWalk)
				{
					Hero->nCurrentAction = Hero->basicWalk;
					Hero->nCurrentFrame.PutEncrypted(0);
				}
					  break;
				case 1:	if (Hero->nCurrentAction != ACTION_RUN)
				{
					Hero->nCurrentAction = ACTION_RUN;
					Hero->nCurrentFrame.PutEncrypted(0);
				}
					  break;
				}

				SendMoveData(&t);
			}
			else
			{
				t.moveFlag = FALSE;
				t.gox = x;
				t.goy = y;
				SendMoveData(&t);
			}
		}
		else
		{
			if (Hero->x != x || Hero->y != y) //  다른 자리로 이동하려 한다면.
				if (Hero->x / TILE_SIZE == mx && Hero->y / TILE_SIZE == my)
				{
					Hero->gox = x;
					Hero->goy = y;
					Hero->moveFlag = TRUE;
					Hero->movetype = 1;

					if (Hero->nCurrentAction != ACTION_RUN)
					{
						Hero->nCurrentAction = ACTION_RUN;
						Hero->nCurrentFrame.PutEncrypted(0);
					}
				}

			if (!Hero->sex)		//여자
				PlayListAutoSounds(80, 0, 0, 0); // 이동실패.
			else
				PlayListAutoSounds(81, 0, 0, 0); // 이동실패.
		}
	}
	else
	{

		if (Hero->x != x || Hero->y != y) //  다른 자리로 이동하려 한다면.
			if (Hero->x / TILE_SIZE == mx && Hero->y / TILE_SIZE == my)
			{
				Hero->gox = x;
				Hero->goy = y;
				Hero->moveFlag = TRUE;
				Hero->movetype = 1;

				if (Hero->nCurrentAction != ACTION_RUN)
				{
					Hero->nCurrentAction = ACTION_RUN;
					Hero->nCurrentFrame.PutEncrypted(0);
				}
			}

		if (!Hero->sex)		//여자
			PlayListAutoSounds(80, 0, 0, 0); // 이동실패.
		else
			PlayListAutoSounds(81, 0, 0, 0); // 이동실패.
	}

	if (!IsHeroDead()) CheckNpcTalk();	// 0923 YGI 추가 

}

/*

  GetItemAttr( itemno, 		ITEM_KIND );




		  */

void DoHeroAttack(int x, int y)
{
	int minrange, maxrange, range;
	int minormax;
	int dx, dy;
	int destx, desty;
	LPCHARACTER d;
	LPITEMGROUND i;
	CHARACTER t;

	AutoAttackx = x;
	AutoAttacky = y;

	if (IsDead(Hero) || Hero->nCurrentAction == ACTION_DIE)
	{
		PlayListAutoSounds(82, 0, 0, 0); // 이동실패.
		AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 4));
		return;
	}

	if (SCharacterData.nCharacterSP == 0)
	{
		PlayListAutoSounds(82, 0, 0, 0); // 이동실패.
		AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 5));//010215 lsw
		return;
	}

	if (g_GameInfo.lpcharacter == NULL) return;

	minrange = AttackRangeTable[Hero->sprno][Hero->accessory[2]][0];
	maxrange = AttackRangeTable[Hero->sprno][Hero->accessory[2]][1];

	switch (g_GameInfo.nSelectedSpriteType)
	{
	case SPRITETYPE_ITEM:	i = (LPITEMGROUND)(g_GameInfo.lpcharacter);
		destx = i->x + i->offx;
		desty = i->y + i->offy;
		minormax = IsAttackRange(&minrange, &maxrange, Hero, destx, desty);
		if (minormax == ATTACKRANGE_IN_AREA_)
		{
			if (i->type == ITEMTYPE_COLOSSUS_STONE)
			{
				if (IsPossibleAttackColossusStone(i))
				{
					SendAttackColossusItem(i);
				}
				break;
			}
			else
			{
				if (Hero->sprno == 0) //여자이면.
				{
					switch (Hero->accessory[2])
					{
					case 76:case 77:case 78: case 79: // 활이나 비검인경우로 이무기는 Magic처리에서 Send하게된다. 
						Send_CMD_JUST_ATTACK_ANIMATION(i);
						break;
					default:
						Send_CMD_ITEM_BOX_BREAK(i);
						break;
					}
				}
				else
				{
					switch (Hero->accessory[2])
					{
					case 78: case 79:case 80:case 81:	// 활이나 비검인경우로 이무기는 Magic처리에서 Send하게된다. 
						Send_CMD_JUST_ATTACK_ANIMATION(i);
						break;
					default:
					{
						Send_CMD_ITEM_BOX_BREAK(i);
						break;
					}
					}
				}
			}
			return;
		}
		break;

	case SPRITETYPE_ON_THE_CEILING_CHAR:
	case SPRITETYPE_CHARACTER:
	case SPRITETYPE_MONSTER:
	case SPRITETYPE_NPC:
		if (IsDead(g_GameInfo.lpcharacter)) return;
		if (Hero == g_GameInfo.lpcharacter) return;

		if (g_GameInfo.lpcharacter->sprno == 100 ||
			g_GameInfo.lpcharacter->sprno == 101)
		{
			return;
		}


		d = g_GameInfo.lpcharacter;
		destx = d->x;
		desty = d->y;

		////////////////////// 0609 lkh 추가(벽.장애물 뒤에서 공격하는 상황 막기위해) ////////////////////////
		minormax = IsAttackRange(&minrange, &maxrange, Hero, destx, desty);
		if (TileLineCheck(destx, desty, Hero->x, Hero->y))		//장애물이 없는 경우
		{
			if (minormax == ATTACKRANGE_IN_AREA_)
			{
				CursorNo(53);
				g_mgrBattle.SendPcAttack(Hero, g_GameInfo.lpcharacter);
				return;
			}
		}
		else
		{
			return;
		}
		break;
	default: return;
	}

	switch (minormax)
	{
		//////////////////////// 0614 lkh 추가 /////////////////////
	case ATTACKRANGE_IN_MIN_:	range = minrange + 16;				break;		//0525 lkh 수치 수정
	case ATTACKRANGE_OUT_MAX_:	range = maxrange - 16;				break;		//			"

	case ATTACKRANGE_IN_AREA_:
	default:						range = (minrange + maxrange) / 2;	break;

	}

	//	if( ReturnAccesablePosition( Hero, destx, desty, Hero->x, Hero->y, range, &dx, &dy ) )

	int ttt = 0;
	if (DontMoveAttackFlag == FALSE)
		if (Hero->nCurrentAction != Hero->basicWalk)
			if (ReturnXYNearPosition(&x, &y, &dx, &dy, range, Hero, destx, desty))
			{
				ttt = 1;
				memcpy(&t, Hero, sizeof(CHARACTER));
				t.moveFlag = TRUE;
				t.movetype = 1;

				//////////////////////// 0625 lkh 추가 ////////////////////////////
				if (GetIndexOfCondition(SCharacterData.condition) == CC_DAZE)		return;//혼란상태


				t.gox = x;
				t.goy = y;

				if (Hero->destx == dx && Hero->desty == dy) return;

				t.destx = dx, t.desty = dy;
				if (PathBuild(&t))
				{
					ttt = 2;
					if (Hero)
					{
						memcpy(Hero, &t, sizeof(CHARACTER));
						Hero->moveFlag = TRUE;
						Hero->gox = x;
						Hero->goy = y;
						Hero->movetype = 1;

						if (Hero->nCurrentAction != ACTION_RUN)
						{
							Hero->nCurrentAction = ACTION_RUN;
							Hero->nCurrentFrame.PutEncrypted(0);
						}

						SendMoveData(Hero);
					}
					else
					{
						t.moveFlag = FALSE;
						t.gox = x;
						t.goy = y;
						SendMoveData(&t);
					}
				}
				else
				{
					if (Hero->x != x || Hero->y != y) //  다른 자리로 이동하려 한다면.
					{
						if ((Hero->x - x) * (Hero->x - x) + (Hero->y - y) * (Hero->y - y) < 2048) // 타일하나 보다 작은 크기 범위.
						{
							//if( Hero->x /TILE_SIZE == dx && Hero->y /TILE_SIZE == dy ) 

							Hero->gox = x;
							Hero->goy = y;
							Hero->moveFlag = TRUE;
							Hero->movetype = 1;

							if (Hero->nCurrentAction != ACTION_RUN)
							{
								Hero->nCurrentAction = ACTION_RUN;
								Hero->nCurrentFrame.PutEncrypted(0);
							}
						}
					}
				}
			}
}

int CheckLeftControlKey()		// LTS ACTION
{
	if (g_aCurrentKeys[DIK_LCONTROL] & 0x80) return true;
	return false;
}


void DoHeroEvent(int x, int y)
{
	int range = 100;
	if (CheckLeftControlKey()) range = 32;	// LTS DRAGON MODIFY
	int sx, sy, ex, ey;
	int dx, dy;

	if (Hero == NULL) return;

	if (IsDead(Hero)) return;

	ex = ((LPCHARACTER)(Hero->lpAttacked))->x;
	ey = ((LPCHARACTER)(Hero->lpAttacked))->y;
	sx = Hero->x;
	sy = Hero->y;

	if ((ex - sx) * (ex - sx) + (ey - sy) * (ey - sy) > 20000) goto ACCESS_;

	if (Hero == (LPCHARACTER)(Hero->lpAttacked)) return;

	if (CheckEventWith_NPC(Hero, (LPCHARACTER)(Hero->lpAttacked)) == FALSE)
	{
	ACCESS_:

		if (ReturnXYNearNPC(&x, &y, &dx, &dy, range, Hero, (LPCHARACTER)(Hero->lpAttacked)))
		{
			g_ActionAnger = TRUE;						// LTS ACTION
			Hero->destx = dx, Hero->desty = dy;
			if (PathBuild(Hero))
			{
				// ### 서버에 보낸다. 
#ifdef _DEBUG // finito 060507
				if (SysInfo.notconectserver)
				{
					if (Hero->nCurrentAction != Hero->basicWalk)
					{
						Hero->moveFlag = TRUE;
						Hero->gox = x;
						Hero->goy = y;
						Hero->movetype = 0;
						Hero->nCurrentAction = Hero->basicWalk;
						Hero->nCurrentFrame.PutEncrypted(0);
					}
				}
				else
#endif
				{
					if (GetIndexOfCondition(SCharacterData.condition) == CC_DAZE)		return;//혼란상태

					Hero->moveFlag = TRUE;
					Hero->gox = x;
					Hero->goy = y;

					if (Hero->peacests)
					{
						Hero->movetype = 1;
					}
					else
					{
						if (Hero->pathcount > 5) Hero->movetype = 1;
						else Hero->movetype = 0;
					}

					switch (Hero->movetype)
					{
					case 0:
						if (Hero->nCurrentAction != Hero->basicWalk)
						{
							if (Hero->nCurrentAction != ACTION_SITDOWN)		// 011213 LTS
							{
								Hero->nCurrentAction = Hero->basicWalk;
								Hero->nCurrentFrame.PutEncrypted(0);
							}
						}
						break;

					case 1:
						if (Hero->nCurrentAction != ACTION_RUN)
						{
							if (Hero->nCurrentAction != ACTION_SITDOWN)		// 011214 LTS
							{
								Hero->nCurrentAction = ACTION_RUN;
								Hero->nCurrentFrame.PutEncrypted(0);
							}
						}
						break;
					}

					if (CheckLeftControlKey())			// LTS DRAGON MODIFY
					{
						LPCHARACTER tempch = (LPCHARACTER)(Hero->lpAttacked);

						if (abs(tempch->x - Hero->x) < 50 && abs(tempch->y - Hero->y) < 50)	//아주가까운 거리에 있어서 패스빌드가 실패한경우
						{
							ChangeDirection(&Hero->direction, Hero->x, Hero->y, tempch->x, tempch->y);
							SendAction(ACTION_ANGER, Hero->direction);					// LTS ACTION
						}
						else
						{
							SendMoveData(Hero);
						}
					}
					else
					{
						SendMoveData(Hero);
					}

					if ((LPCHARACTER)(Hero->lpAttacked))
					{
						if (((LPCHARACTER)(Hero->lpAttacked))->id < 10000)
						{
							SendChatData(GreetingStr);
						}
					}
				}
			}
			else
			{
			}
		}
	}
}

int NotNeededMouseLButtonClick(void)
{
	if (Hero == NULL) return 1;

	if (g_StartMenuOn == TRUE)  return 1;
	if (g_pointMouseY > GAME_SCREEN_YSIZE) return 1;
	if (g_MouseInMenu) return 1;		// 0306 YGI

	return 0;
}


//////////////////////////////

//////////////////////////////// 0313 lkh 수정 ////////////////////////////////
///////////////////////// 이규훈 1220 /////////////////////////////
// 마우스를 클릭한곳의 Map좌표를 얻는다.
// 만약 술에 취하면 엉뚱한곳의 좌표를 준다. 
void MouseClickPosition(int* mx, int* my)
{
	int distance_X = 0;
	int	temp_Dist_X = 0;
	int distance_Y = 0;
	int	temp_Dist_Y = 0;

	if (GetIndexOfCondition(SCharacterData.condition) == CC_DAZE)		//혼란상태		// 0116 YGI
	{
		////////////////////// 0331 lkh 수정 ////////////////////////
		temp_Dist_X = abs(Hero->x - Mox);
		temp_Dist_Y = abs(Hero->y - Moy);


		if (temp_Dist_X < 32)		temp_Dist_X = 32;
		if (temp_Dist_Y < 32)		temp_Dist_Y = 32;

		distance_X = (temp_Dist_X * 2) + ((rand() % temp_Dist_X / 2) - (temp_Dist_X / 4));
		distance_Y = (temp_Dist_Y * 2) + ((rand() % temp_Dist_Y / 2) - (temp_Dist_Y / 4));

		switch (rand() % 4)
		{
		case 0:		//x 대칭
			if (Hero->x > Mox)
				Mox += distance_X;
			else
				Mox -= distance_X;
			break;
		case 1:		//y 대칭
			if (Hero->y > Moy)
				Moy += distance_Y;
			else
				Moy -= distance_Y;
			break;
		case 2:		//x,y 대칭
			if (Hero->x > Mox)
				Mox += distance_X;
			else
				Mox -= distance_X;
			if (Hero->y > Moy)
				Moy += distance_Y;
			else
				Moy -= distance_Y;
			break;
		case 3:		//random pattern
			Mox = rand() % distance_X - distance_X / 2;
			Moy = rand() % distance_Y - distance_Y / 2;
			break;
		}
	}


	*mx = Mox;
	*my = Moy;
}


#define MELEE				0
#define RANGE_BOW			1	
#define RANGE_DEFAULT		2

int GetItemKind(int itemno)
{
	int itemkind = GetItemAttr(itemno, ITEM_KIND);
	if (itemkind == 0) return -1;

	switch (itemkind)
	{
	case IK_LONG_BOW:
	case IK_ARROW_POINT:
	case IK_HURL:
	case IK_SHORT_BOW:
	case IK_DUAL_BOW://030102 lsw
	case IK_TWO_ARROW:
	case IK_DUAL_ARROW://030102 lsw
	case IK_SNOW_BALL:
	{ // 장거리 무기
		return RANGE_BOW;
	}
	case IK_WHIRL:
	case IK_STAFE:
	case IK_SPEAR:
	case IK_FORK:
	{ // 중거리 무기
		return RANGE_DEFAULT;
	}
	}

	return MELEE; // 단거리무기
}

// mousebutton : 0 : LeftClick 
//				 1 : Left Double Click..
//////////////////////////// 0522 LKH 수정 //////////////////////////////
void HeroActionProc(int mousebutton, int mx, int my)
{	//< CSD-030930
	if (g_StartMenuOn == TRUE)  return;
	if (IsMouseOnMenu()) return;

	mx /= TILE_SIZE;	mx *= TILE_SIZE;	mx += (TILE_SIZE / 2);
	my /= TILE_SIZE;	my *= TILE_SIZE;	my += (TILE_SIZE / 2);

	if (Hero)
	{
		if (abs(mx - Hero->x) / TILE_SIZE > Hero->sight * 2)	return;
		if (abs(my - Hero->y) / TILE_SIZE > Hero->sight * 2)	return;
	}
	//< CSD-021021
	if (Hero->condition & CON_STONE)
	{ // 아래 상태에서는 프레임 정지 및 이동불가
		return;
	}
	// 다시 상태 회복
	if (Hero->viewtype == VIEWTYPE_STONE_)
	{
		Hero->viewtype = VIEWTYPE_NORMAL_;
	}
	//> CSD-021021
	switch (Hero->peacests)
	{
	case PC_STATUS_PEACE:
	{
		switch (g_GameInfo.nSelectedSpriteType)
		{
		case SPRITETYPE_NONE:  // 눌려진 상태에서 아무것도 Checking된것이 없다면.. 그냥 이동한다.
		{
			Hero->nAttackedType = 0;
			Hero->lpAttacked = NULL;
			DoHeroMove(mousebutton, mx, my);
			break;
		}
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		{
			Hero->nAttackedType = g_GameInfo.nSelectedSpriteType;
			Hero->lpAttacked = (void*)g_GameInfo.lpcharacter;
			DoHeroEvent(mx, my);
			break;
		}
		}

		break;
	}
	case PC_STATUS_BATTLE:
	{
		switch (g_GameInfo.nSelectedSpriteType)
		{
		case SPRITETYPE_NONE:
		{	// 눌려진 상태에서 아무것도 Checking된것이 없다면.. 그냥 이동한다. 
			if (ACTION_ATTACK_BOW == Hero->nCurrentAction || ACTION_THROW == Hero->nCurrentAction)
			{	//화살쏘기나 비도 던지기 동작중에 타겟을 초기화 하지 않토록 한다.
				goto NEXT_PROCESS_;
			}	// 0303011 kyo

			Hero->nAttackedType = 0;
			Hero->lpAttacked = NULL;
			DoHeroMove(mousebutton, mx, my);
			break;
		}
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		{
			switch ((g_GameInfo.lpcharacter)->sprno)	//만약에 내가 공격하려는 더미가 칼에 맞지 않으면....
			{
			case 92:
			case 93:
			{	// 단거리 무기 연습용 더미
				if (GetItemKind(EquipItemAttr[WT_WEAPON].item_no) != MELEE)
				{
					AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 6));//010215 lsw
					goto RET_;
				}

				goto NEXT_PROCESS_;
			}
			case 94:
			{	// 장거리 무기 연습용	
				if (GetItemKind(EquipItemAttr[WT_WEAPON].item_no) != RANGE_BOW)
				{
					AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 7));//010215 lsw
					goto RET_;
				}

				goto NEXT_PROCESS_;
			}
			case 95:
			{	// 중거리 무기 연습용
				if (GetItemKind(EquipItemAttr[WT_WEAPON].item_no) != RANGE_DEFAULT)
				{
					AddCurrentStatusMessage(255, 100, 100, lan->OutputMessage(9, 8));//010215 lsw
					goto RET_;
				}

				goto NEXT_PROCESS_;
			}
			}
		}
		case SPRITETYPE_ITEM:
		{
		NEXT_PROCESS_:
			DoHeroAttack(mx, my);
			break;
		}
		}
		break;
	}
	}

RET_:
	HeroActionProcChecked = true;
}	//> CSD-030930

int g_CurrCharStatus;
#define CCS_NONE		0	
#define CCS_ATTACKING	1	// 공격중이었다.
#define CCS_MOVING		2	// 걷는중이었다.


extern void MouseProcess(UINT message, WPARAM wParam, LPARAM lParam);
void DoLbuttonStill()
{
	//if( !GetSysInfo( SI_GAME_MAKE_MODE ) ) return;

	static bool flag = true;
	switch (g_CurrCharStatus)
	{
	case CCS_ATTACKING:
	{
		if (flag == false)
		{
			static int ct = 1;
			if (!(ct % 10))		// 15번당 1번
			{
				DWORD Param;
				Param = MAKELONG(g_pointMouseX, g_pointMouseY);
				MouseProcess(WM_LBUTTONDOWN, 0, Param);
				DoLButtonDown();
			}
			ct++;
		}
		else flag = false;
		break;
	}
	default: flag = true;
	}
}

void DoLButtonDown(void)	// LButton이 다운되면 실행되어 지는곳...
{
	int mx, my;
	if (ViewTipsCheck()) return;
	if (MouseCursorOnOff == false) return;// 기술시전중.
	if (NotNeededMouseLButtonClick()) return;
	if (CounselerSelectChar()) return;
	if (CheckSkill()) return;

	////////////// 이규훈 1112 //////////////////
	if (tool_ID_SKILL_INPUT)		//skill 속성 입력창이 활성화되어 있는 경우
	{
		char temp[FILENAME_MAX];
		mx = Mox / 32;
		my = Moy / 32;
		g_MapSkillTable.x = mx;
		g_MapSkillTable.y = my;
		sprintf(temp, "%d", mx);
		Edit_SetText(GetDlgItem(InputSkillAttrHdlg, IDC_INPUT_TILE_X), temp);	//선택된 타일 종좌표
		sprintf(temp, "%d", my);
		Edit_SetText(GetDlgItem(InputSkillAttrHdlg, IDC_INPUT_TILE_Y), temp);	//선택된 타일 종좌표
		MapSkillTool_Attr(mx, my);
		return;
	}
	/////////////////////////////////////////////////

	if (tool_ID_INPUT_MYHOUSE)
		return;


	if (tool_mapobject_modify == 0) IDCheckedMapObject = -1;
	if (IDCheckedMapObject != -1)
	{
		ViewMapObjectInfo();
		return;
	}
	if (tool_IDD_INPUT_MAPOBJECT)
	{
		InsertMapObject(tool_CurMapObjectId, Mox, Moy);
		return;
	}

	if (YouCanHeroActionProc)
	{
		YouCanHeroActionProc = 0;
		MouseClickPosition(&mx, &my);
		if (CheckEventWith_Item()) return;
		HeroActionProc(Lc_DefaultMoveType, mx, my);
		g_CurrCharStatus = CCS_ATTACKING;
	}
	else return;

	// 011210 YGI
	ClickCharacter();		//  클릭 
}


extern	void	PutMouseItemCheck();
void DoLButtonUp(void)
{
	g_CurrCharStatus = CCS_NONE;
	//	int mx, my;										
	if (ViewTipsCheck())  return;

	if (g_MouseItemType == 1)//아이템을 들고 있고 어딘가에 놓는다
	{
		switch (g_GameInfo.nSelectedSpriteType)
		{
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:	break;
		case SPRITETYPE_CHARACTER:	//021030 lsw //그 대상이 캐릭터라면
		{
			if (g_GameInfo.lpcharacter && g_allowDrop)		// 0131 YGI //Eleval 22/05/09: to disable droping on char that end in an exchange when g_allowDrop == false
			{
				if (g_MouseInMenu)
				{
					return;		// 0224 YGI
				}
				if (g_GameInfo.lpcharacter->id >= 0 && g_GameInfo.lpcharacter->id <= 10000)
				{
					if (g_GameInfo.lpcharacter != Hero)//상대가 내 캐릭터가 아니라면
					{
						if (CheckPC && g_GameInfo.lpcharacter->viewtype != VIEWTYPE_GHOST_)//030127 lsw
						{
							LButtonDown = 0;
							MouseDrag = 0;
							::PutMouseItemCheck();// 실제로 아이템을 되돌려 놓는다. ( 꼭 이자리에.. )
							::SendExchangeItemWithOtherCh(g_GameInfo.lpcharacter->id, EXCHANGE_TYPE_NORMAL);//021030 lsw
							g_DropOnHero = true;
						}
					}
				}
			}
		}break;
		}
		return;
	}

	if (YouCanHeroActionProc)
	{
		int mx, my;
		YouCanHeroActionProc = 0;
		MouseClickPosition(&mx, &my);
		if (CheckEventWith_Item()) return;
		HeroActionProc(Lc_DefaultMoveType, mx, my);
	}
	else return;

}



void DoRButtonDown(void)
{
	static int t;

	int x = 0, y = 0;

	if (MouseCursorOnOff == false) return;// 기술시전중.



	//////////////////// 0311 lkh 추가 ////////////////////
	switch (g_Operator_Function)
	{
	case OPERATOR_PLAYSOUND_EVENT1:
		SendCMD_CHARACTER_DEMAND(CCD_PLAYSOUND_EVENT1, Hero->id);
		g_Operator_Function = 0;
		break;
	case OPERATOR_PLAYSOUND_EVENT2:
		SendCMD_CHARACTER_DEMAND(CCD_PLAYSOUND_EVENT2, Hero->id);
		g_Operator_Function = 0;
		break;
	case OPERATOR_OUTPUT_CELEBRATE:
		if (g_GameInfo.lpcharacter == NULL)
		{
			g_Operator_Function = 0;
			break;
		}
		if (g_GameInfo.lpcharacter->type == SPRITETYPE_CHARACTER || g_GameInfo.lpcharacter->type == SPRITETYPE_MONSTER)
		{	//< CSD-TW-030606
			g_mgrBattle.SendMagicExecute((char)OPERATOR_OUTPUT_CELEBRATE, g_GameInfo.lpcharacter, g_GameInfo.lpcharacter->x, g_GameInfo.lpcharacter->y);
			g_Operator_Function = 0;
		}	//> CSD-TW-030606
		break;
	}
	//////////////////// 여기까지 ////////////////////////

	SkillSettingClear();

	if (g_StartMenuOn) return;


	//	if( CursorCheckedItem != NULL ) { ViewItemInfo(); return; }

		// 011210 YGI
	if (tool_MultiChar || (IsReporterMode() && TileMap[Mox / TILE_SIZE][Moy / TILE_SIZE].attr_dont == 0))
	{
		//Mox-=TILE_SIZE;
		Moy -= TILE_SIZE;
		TileMap[Hero->position.x][Hero->position.y].occupied = 0;
		Hero->x = Mox;
		Hero->y = Moy;
		Hero->gox = Mox;
		Hero->goy = Moy;
		Hero->position.x = Mox / TILE_SIZE;
		Hero->position.y = Moy / TILE_SIZE;
		SendMoveData(Hero);
		HeroActionProc(Lc_DefaultMoveType, Mox, Moy + TILE_SIZE);
		return;
	}

	////////////// 이규훈 1112 //////////////////
	if (tool_ID_SKILL_INPUT)		//skill 속성 입력창이 활성화되어 있는 경우
	{
		lpMAPSKILLTABLE	result;
		int	mx = Mox / 32;
		int	my = Moy / 32;

		char temp[FILENAME_MAX];
		sprintf(temp, "%d", mx);
		Edit_SetText(GetDlgItem(InputSkillAttrHdlg, IDC_INPUT_TILE_X), temp);	//선택된 타일 종좌표
		sprintf(temp, "%d", my);
		Edit_SetText(GetDlgItem(InputSkillAttrHdlg, IDC_INPUT_TILE_Y), temp);	//선택된 타일 종좌표

		// 010312 KHS
		int hx, hy;
		hx = (int)(mx / (int)((g_Map.file.wWidth + 7) / 8));
		hy = (int)(my / (int)((g_Map.file.wHeight + 7) / 8));

		// 0308 lkh
		result = FindSkill(&Header[hx][hy], mx, my);
		if (result)
		{
			DeleteSkill(&Header[hx][hy], result);
			result = FindSkill(&Header[hx][hy], mx, my);
			if (result == NULL)
				TileMap[mx][my].attr_skill = 0;
		}
		return;
	}
	/////////////////////////////////////////////////

	if (tool_IDD_INPUT_MAPOBJECT)
	{
		DeleteMapObject(IDCheckedMapObject);
		return;
	}


	/////////////////////////// 0430 lkh 수정 //////////////////////////////
	if (!IsMenuActive())		// 활성화된 메뉴안에 마우스가 있는지를 판단
	{
		if (g_AttackSkill_Trace1)
		{
			//InsertMagic( Hero, Hero, 8, 1, 0, 0, 0, 0, 0); // 030402 kyo //이상 차징 현상
		}
		else if (g_AttackSkill_Trace2)
		{
			//InsertMagic( Hero, Hero, 8, 2, 0, 0, 0, 0, 0); // 030402 kyo
		}
		else
		{
			CheckDoingAppointMagic();
		}
	}

	if (Hero->nCurrentAction == 17)		// 011214 LTS // 앉은 상태에서 방향을 바꾸어 준다.
	{
		switch (Hero->direction)
		{
		case DIRECTION_UP: Hero->direction = DIRECTION_RIGHTUP;	break;
		case DIRECTION_RIGHTUP: Hero->direction = DIRECTION_RIGHT;		break;
		case DIRECTION_RIGHT: Hero->direction = DIRECTION_RIGHTDOWN;	break;
		case DIRECTION_RIGHTDOWN: Hero->direction = DIRECTION_DOWN;		break;
		case DIRECTION_DOWN: Hero->direction = DIRECTION_LEFTDOWN;	break;
		case DIRECTION_LEFTDOWN: Hero->direction = DIRECTION_LEFT;		break;
		case DIRECTION_LEFT: Hero->direction = DIRECTION_LEFTUP;		break;
		case DIRECTION_LEFTUP: Hero->direction = DIRECTION_UP;			break;
		}
		t_packet p;
		p.h.header.type = CMD_SITDOWN_DIRECTION_CHANGED;
		p.u.LocalWar.SDChanged.Direction = Hero->direction;
		p.h.header.size = sizeof(t_SDChanged);
		QueuePacket(&p, 1);
		return;
	}


	/*
		int magic_no;
		if( !tool_ID_EFFECT_TEST )
		{
			magic_no = 30;
		}
		else
		{
			magic_no = Hero->excute_MagicNum;
		}
	*/


}



void DoRButtonUp(void)
{
}

void DoLDButtonDown(void)
{
	int mx, my;

	return;

	if (NotNeededMouseLButtonClick()) return;

	MouseClickPosition(&mx, &my);

	if (Lc_DefaultMoveType == 0) //  기본달리기로 설정되어있다면 DoubleClick 을 적용하나 그렇지 않으면 호출하지 않는다.  
		HeroActionProc(1, mx, my);
}


void DoRDButtonDown(void)
{

}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////			
//////			
//////			실제적으로  출력하는곳..
//////			Sorting하여 출력한다.
//////					

int BoxAndBoxCrash_(int ax, int ay, int axl, int ayl, int bx, int by, int bxl, int byl)
{
	if (ax + axl >= bx && bx + bxl >= ax && ay + ayl >= by && by + byl >= ay)
		return 1;

	return 0;
}


int CrashTileandEffect(LPORDERTABLE oj, LPORDERTABLE oi)
{
	int ax, ay, axl, ayl, bx, by, bxl, byl;
	Spr* sp;

	bx = oj->x, by = oj->y;
	bxl = TILE_SIZE;  byl = TILE_SIZE;
	sp = (Spr*)(oi->lpvData);
	ax = oi->x - sp->ox; ay = oi->y - sp->oy - oi->height;
	axl = sp->xl; ayl = sp->yl;

	return BoxAndBoxCrash_(ax, ay, axl, ayl, bx, by, bxl, byl);
}



int CrashOiOj(LPORDERTABLE oi, LPORDERTABLE oj)
{
	//	return 1;
	int ax, ay, axl, ayl, bx, by, bxl, byl;
	LPCHARACTER		ch;
	LPITEMGROUND    i;
	LPMAPOBJECT		mo;
	Spr* sp;


	switch (oi->wType)
	{
	case SPRITETYPE_ALPHAOBJECT:
	case SPRITETYPE_OBJECT:
	{
		switch (oj->wType)
		{
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_MONSTER:
			mo = (LPMAPOBJECT)oi->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			ax = mo->x - sp->ox + mo->offx; ay = mo->y - sp->oy + mo->offy;
			axl = sp->xl; ayl = sp->yl;

			ch = (LPCHARACTER)oj->lpvData;
			sp = ch->sp;
			bx = oj->x - sp->ox, by = oj->y - sp->oy;
			bxl = sp->xl;   byl = sp->yl;

			return BoxAndBoxCrash_(ax, 0, axl, 2, bx, 0, bxl, 3);
			break;
		case SPRITETYPE_ITEM:		i = (LPITEMGROUND)oj->lpvData;
			//											if( ( i->ddx[i->curframe/2] != 0 ) && ( i->ddy[i->curframe/2] != 0 )  ) return 0;
			sp = i->spr[i->curframe / 2];
			bx = oj->x - sp->ox, by = oj->y - sp->oy;
			bxl = sp->xl;  byl = sp->yl;

			mo = (LPMAPOBJECT)oi->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			ax = mo->x - sp->ox + mo->offx; ay = mo->y - sp->oy + mo->offy;
			axl = sp->xl; ayl = sp->yl;

			break;
		case SPRITETYPE_TILE:		bx = oj->x, by = oj->y;
			bxl = TILE_SIZE;  byl = TILE_SIZE;
			mo = (LPMAPOBJECT)oi->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			ax = mo->x - sp->ox; ay = mo->y - sp->oy;
			axl = sp->xl; ayl = sp->yl;
			break;

		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT: 	mo = (LPMAPOBJECT)oi->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			ax = mo->x - sp->ox + mo->offx; ay = mo->y - sp->oy + mo->offy;
			axl = sp->xl; ayl = sp->yl;

			mo = (LPMAPOBJECT)oj->lpvData;
			sp = &MoImage[mo->id - 1].spr[0];
			bx = mo->x - sp->ox + mo->offx; by = mo->y - sp->oy + mo->offy;
			bxl = sp->xl; byl = sp->yl;

			return BoxAndBoxCrash_(ax, 0, axl, 2, bx, 0, bxl, 3);


			break;



		default: return 0;
		}

		//			int t = 

		return BoxAndBoxCrash_(ax, ay, axl, ayl, bx, by, bxl, byl);
	}
	break;

	case SPRITETYPE_ITEM:
	{
		switch (oj->wType)
		{
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_MONSTER:
			i = (LPITEMGROUND)oi->lpvData;
			sp = i->spr[i->curframe / 2];
			ax = oi->x - sp->ox, ay = oi->y - sp->oy;
			axl = sp->xl;  ayl = sp->yl;

			ch = (LPCHARACTER)oj->lpvData;
			sp = ch->sp;
			bx = oj->x - sp->ox, by = oj->y - sp->oy;
			bxl = sp->xl;   byl = sp->yl;
			break;
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:	mo = (LPMAPOBJECT)oi->lpvData;
			if (mo->attr2) return 0;
			sp = &MoImage[mo->id - 1].spr[0];
			bx = mo->x - sp->ox + mo->offx; by = mo->y - sp->oy + mo->offy;
			bxl = sp->xl; byl = sp->yl;

			i = (LPITEMGROUND)oi->lpvData;
			sp = i->spr[i->curframe / 2];
			ax = oi->x - sp->ox, ay = oi->y - sp->oy;
			axl = sp->xl;  ayl = sp->yl;
			break;
		case SPRITETYPE_TILE:		bx = oj->x, by = oj->y;
			bxl = TILE_SIZE;  byl = TILE_SIZE;
			i = (LPITEMGROUND)oi->lpvData;
			sp = i->spr[i->curframe / 2];
			ax = oi->x - sp->ox, ay = oi->y - sp->oy;
			axl = sp->xl;  ayl = sp->yl;
			break;
		default: return 0;
		}
		return BoxAndBoxCrash_(ax, ay, axl, ayl, bx, by, bxl, byl);
	}
	break;
	default: return 0;
	}

	return 0;
}

int SortWithItem(LPORDERTABLE oi, LPORDERTABLE oj)
{
	if (oi->wType != SPRITETYPE_ITEM) return 0;

	LPITEMGROUND i = (LPITEMGROUND)oi->lpvData;

	if (i->type != ITEMTYPE_DOOR) return 0;

	return CrashOiOj(oi, oj);
}

int SortWithObject(LPORDERTABLE oi, LPORDERTABLE oj)
{
	LPMAPOBJECT mo;

	if (oi->wType != SPRITETYPE_OBJECT) return 0;
	if (oi->wType != SPRITETYPE_ALPHAOBJECT) return 0;
	mo = ((LPMAPOBJECT)(oi->lpvData));
	if (mo->attr2 == 0) return 0;

	//	if( oj->wType == SPRITETYPE_OBJECT ) 
	//	{
	//		mo = ((LPMAPOBJECT )(oj->lpvData));
	//		if( mo->attr2 ) return 0; 
	//	}

	return CrashOiOj(oi, oj);
}

int SortWithObjectObject(LPORDERTABLE oi, LPORDERTABLE oj)
{
	if (oi->wType != SPRITETYPE_OBJECT) return 0;
	if (oi->wType != SPRITETYPE_ALPHAOBJECT) return 0;
	if (oj->wType != SPRITETYPE_OBJECT) return 0;
	if (oj->wType != SPRITETYPE_ALPHAOBJECT) return 0;

	return 1;
}




void CalcOrder(void)
{
	register int	i, j;
	LPORDERTABLE	pi;

	LPCHARACTER		ch;
	//	LPTILE			lpTile;

	Spr* s;
	LPORDERTABLE	o, oj, lpmin;
	LPMAPOBJECT		mo;
	ORDERTABLE min;

	//	MapObjectTouchOrder();

	WaveTileStart++;
	WaveTileStart %= 39;

	ch = Hero;
	int cc = 0;
	while (ch != NULL)
	{
		cc++;
		if (IsScreenArea(ch))
		{
			if (g_OrderInfo.count >= MAX_ORDERTABLE) break;

			o = &g_OrderInfo.order[g_OrderInfo.count];

			if (ch->height > 100) o->wType = SPRITETYPE_ON_THE_CEILING_CHAR;
			else o->wType = ch->type;
			o->x = ch->x;
			o->y = ch->y;
			o->height = ch->height;
			o->offset = ch->y;
			o->lpvData = ch;
			o->show = 1;
			o->sort = 1;
			g_OrderInfo.count++;


		}
		else
		{
			if (ch != Hero)
			{
				// 음... 주인공의 시야+SIGHT_PLUS에서 벗어나면 없앤다. 
//				if( (ch->position.x >Hero->position.x) * ( ch->x - Hero->x )  +  (ch->y-Hero->y) * (ch->y-Hero->y) > Hero->sight + 90000 )
//				if( abs( ch->position.x - Hero->position.x ) > 20 && abs( ch->position.y - Hero->position.y ) > 20 )
//				{
//					tempch = ch->lpNext;
//					DeleteCharacter( &g_CharacterList, ch );
//					ch = tempch;
//					continue;
//				}		
			}
		}
		ch = ch->lpNext;
	}




	// object				
	//lpObjectHeader = g_ObjectListHeader.lpHead;
	IDCheckedMapObject = -1;
	g_BoadAndSignNumber = 0; // CSD-030324 // thai2 YGI

	if (tool_DrawObject == 0)
		for (i = 0; i < TotalMapObject; i++)
		{
			mo = &Mo[i];
			if (IsScreenAreaCSP(mo->x + mo->offx, mo->y + mo->offy, &MoImage[mo->id - 1].spr[0]))
			{
				bool	check = ReturnCheckObject(mo);

				//////////////////////////// 0812 lkh 추가 ///////////////////////////////
				//	현상금 게시판을 더블클릭(왼쪽버튼)하는 경우 게시판 메뉴 뜬다.
	//			Spr *sp = &MoImage[ mo->id-1].spr[0];
	///			if( mo->id >= 2482 && mo->id <=2485 )
	///				if( BoxAndDotCrash( (int)(mo->x+mo->offx-sp->ox), (int)(mo->y+mo->offy-sp->oy), sp->xl, sp->yl, Mox, Moy) && g_MouseInMenu == false )
	//					if( g_nLDButtonState )
	//						CallSmallMenu(MN_HUNTER_MAIN);
				//////////////////////////////////////////////////////////////////////////

				CheckMapObject(mo);
				{
					if (g_OrderInfo.count >= MAX_ORDERTABLE) break;

					o = &g_OrderInfo.order[g_OrderInfo.count];

					if (check)	o->wType = SPRITETYPE_ALPHAOBJECT;
					else		o->wType = SPRITETYPE_OBJECT;		//ALPHAOBJECT
					o->x = mo->x;
					o->y = mo->y;
					o->offset = mo->y;
					o->rgb = i; // rgb로 쓰는거이 아니라 배열번호를 쓴다. 

					o->height = 0;
					o->lpvData = (LPVOID)(mo);
					o->show = 1;
					o->sort = 1;
					if (mo->light) OutputLight(mo->x + mo->offx + mo->lightx, mo->y + mo->offy + mo->lighty, mo->light);
					g_OrderInfo.count++;
				}
#ifdef _DEBUG // finito 060507							
				if (GetSysInfo(SI_GAME_MAKE_MODE))
				{
					// MapObject를 Check할때.1. 찍은 좌표와 충돌되는 MapObject를 찾는다. 
					if (BoxAndDotCrash(mo->x - 10, mo->y - 10, 20, 20, Mox, Moy))
					{
						IDCheckedMapObject = i;
					}
				}
#endif
			}
		}


	// MapObject를 Check할때.2. 마우스와 충돌되는 MapObject를 찾는다. 
	if (GetSysInfo(SI_GAME_MAKE_MODE))
		if (IDCheckedMapObject == -1)
			for (i = 0; i < TotalMapObject; i++)
			{
				mo = &Mo[i];
				if (IsScreenAreaCSP(mo->x + mo->offx, mo->y + mo->offy, &MoImage[mo->id - 1].spr[0]))
				{
					{
						Spr* sp = &MoImage[mo->id - 1].spr[mo->curframe];
						if (BoxAndDotCrash(mo->x + mo->offx - sp->ox, mo->y + mo->offy - sp->oy, sp->xl, sp->yl, Mox, Moy))
						{
							IDCheckedMapObject = i;
						}
					}
				}
			}

	int minid;

	NewOrderC = 0;
	for (i = 0; i < g_OrderInfo.count; i++)
	{
		for (j = 0; j < g_OrderInfo.count; j++)
		{
			oj = &g_OrderInfo.order[j];
			if (oj->sort == 0) continue;

			switch (oj->wType)
			{
			case SPRITETYPE_DETECT_TILE:
			case SPRITETYPE_SKILLCURSOR:
				continue;
			}
			memcpy(&min, oj, sizeof(ORDERTABLE));
			lpmin = oj;
			minid = j;
			break;
		}

		for (j = 0; j < g_OrderInfo.count; j++)
		{
			if (minid == j) continue;

			oj = &g_OrderInfo.order[j];
			if (oj->sort == 0) continue;

			switch (oj->wType)
			{
			case SPRITETYPE_DETECT_TILE:
			case SPRITETYPE_SKILLCURSOR:
				continue;
			}

			if (SortWithItem(lpmin, oj))
				//if( CrashlpminOj( lpmin, oj ) )
			{
				LPITEMGROUND ti = (LPITEMGROUND)(lpmin->lpvData);
				int c = ti->curframe >> 1;
				int kkx = lpmin->x, kky = lpmin->y;
				if (Determinant(ti->dsx[c], ti->dsy[c], ti->ddx[c], ti->ddy[c], oj->x, oj->offset) < 0)
				{
					//memcpy( &min, oj, sizeof( ORDERTABLE ) );
					lpmin = oj;//min;
					minid = j;
				}
			}
			else if (SortWithItem(oj, lpmin))
			{
				LPITEMGROUND tj = (LPITEMGROUND)(oj->lpvData);
				int c = tj->curframe >> 1;
				int kkx = oj->x, kky = oj->y;
				if (Determinant(tj->dsx[c], tj->dsy[c], tj->ddx[c], tj->ddy[c], lpmin->x, lpmin->offset) > 0)
				{
					//memcpy( &min, oj, sizeof( ORDERTABLE ) );
					lpmin = oj;//min;
					minid = j;
				}
			}
			else if (SortWithObjectObject(oj, lpmin))
			{
				LPMAPOBJECT mo1 = (LPMAPOBJECT)(oj->lpvData);
				LPMAPOBJECT mo2 = (LPMAPOBJECT)(lpmin->lpvData);

				int mo1minoff = min(mo1->y + mo1->dy0[mo1->curframe], mo1->y + mo1->dy1[mo1->curframe]);
				int mo2minoff = min(mo2->y + mo2->dy0[mo2->curframe], mo2->y + mo2->dy1[mo2->curframe]);

				if (mo1minoff < mo2minoff)
				{
					lpmin = oj;//min;
					minid = j;
				}
			}
			else if (SortWithObject(oj, lpmin))
			{
				LPMAPOBJECT mo = (LPMAPOBJECT)(oj->lpvData);
				int kkx = mo->x, kky = mo->y;
				int c = mo->curframe;
				if (Determinant(kkx + mo->dx0[c], kky + mo->dy0[c], kkx + mo->dx1[c], kky + mo->dy1[c], lpmin->x, lpmin->offset) > 0)
				{
					//memcpy( &min, oj, sizeof( ORDERTABLE ) );
					lpmin = oj;//min;
					minid = j;
				}
			}

			else if (SortWithObject(lpmin, oj))
			{
				mo = (LPMAPOBJECT)(lpmin->lpvData);
				int kkx = lpmin->x, kky = lpmin->y;
				int c = mo->curframe;
				TestDeter = Determinant(kkx + mo->dx0[c], kky + mo->dy0[c], kkx + mo->dx1[c], kky + mo->dy1[c], oj->x, oj->offset);
				if (TestDeter < 0)
				{
					//memcpy( &min, oj, sizeof( ORDERTABLE ) );
					lpmin = oj;//min;
					minid = j;
				}
			}
			else if (lpmin->offset > oj->offset)
			{
				//memcpy( &min, oj, sizeof( ORDERTABLE ) );
				lpmin = oj;//min;
				minid = j;
			}
			else if (lpmin->offset == oj->offset)
			{
				if (oj->wType == SPRITETYPE_TILE)
				{
					switch (lpmin->wType)
					{
						//						case	SPRITETYPE_CHARACTER:
						//						{		
						//							memcpy( &temp,	oj, sizeof( ORDERTABLE ) );
						//							memcpy( oj,		lpmin, sizeof( ORDERTABLE ) );
						//							memcpy( lpmin,	 &temp, sizeof( ORDERTABLE ) );
						//							break;
						//						}		
					case SPRITETYPE_ALPHAOBJECT:
					case	SPRITETYPE_OBJECT:
					{
						LPMAPOBJECT	mo = (LPMAPOBJECT)lpmin->lpvData;

						if (mo->attr == OBJECTATTR_BACK_ABOVE || mo->attr == OBJECTATTR_FRONT_ABOVE || mo->attr == OBJECTATTR_FRONT_BELOW)
						{
							//memcpy( &min, oj, sizeof( ORDERTABLE ) );
							lpmin = oj;//min;
							minid = j;
						}
						break;
					}
					}
				}
			}
		}

		g_OrderInfo.order[minid].sort = 0;
		memcpy(&neworder[NewOrderC], &g_OrderInfo.order[minid], sizeof(ORDERTABLE));
		NewOrderC++;
	}

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		pi = &g_OrderInfo.order[i];

		if (pi->sort)
		{
			pi->sort = 0;

			memcpy(&neworder[NewOrderC], pi, sizeof(ORDERTABLE));
			NewOrderC++;
		}
	}

	LPITEMGROUND item;
	Spr* sp;
	bool bClickOn = false;

	MouseCheckCharacterName = NULL;
	g_GameInfo.nSelectedSpriteType = SPRITETYPE_NONE;
	g_GameInfo.lpcharacter = NULL;
	for (i = 0; i < g_OrderInfo.count; i++)
	{
		pi = &g_OrderInfo.order[i];
		pi->rank = i;

		switch (pi->wType)
		{
		case SPRITETYPE_ITEM:
			item = (LPITEMGROUND)pi->lpvData;
			sp = item->spr[item->curframe / 2];
			if (item->attr[IATTR_ATTR] & IA2_BOX || item->attr[IATTR_ATTR] & IA2_COLOSSUS_STONE)
			{
				if (BoxAndDotCrash(item->x + item->offx - sp->ox, item->y + item->offy - sp->oy, sp->xl, sp->yl, Mox, Moy))
				{
					g_GameInfo.lpvSelectedSprite = pi->lpvData;
					g_GameInfo.nSelectedSpriteType = pi->wType;
					g_GameInfo.lpcharacter = (LPCHARACTER)pi->lpvData;
					return;
				}
			}
			break;

		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		case SPRITETYPE_CHARACTER:

			ch = (LPCHARACTER)pi->lpvData;
			int dir = ch->direction;
			int sprc = ch->AnimationTable[ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
			int xl, yl;
			int ox, oy;
			//int height	= ch->AnimationTable[ ch->nCurrentAction].height[  ch->nCurrentFrame.GetDecrypted() ];
			s = &(CharSpr[ch->sprno].sp[dir][sprc]);

			ox = s->ox;
			oy = s->oy;
			xl = s->xl; if (xl < 15) { ox <<= 2;  xl <<= 2; }
			yl = s->yl;	if (yl < 15) { oy <<= 2;  xl <<= 2; }


			if (ch->just_ani == false)
				if (BoxAndDotCrash(pi->x - ox, pi->y - oy - ch->height, xl, yl, Mox, Moy))
				{
					if (ch->viewtype == VIEWTYPE_OBSERVE_)
					{	//< CSD-030509
						return;
					}	//> CSD-030509
					//	내가 유령이거나 유령을 볼 있을 때만 상대유령을 선택할수 있다. 
					if (ch->viewtype == VIEWTYPE_GHOST_)
					{
						// YGI ACER
						if (IsHidenGhost(ch)) break;//020510 lsw
						if (!CanViewGhost(Hero) && Hero->viewtype != VIEWTYPE_GHOST_) break;
						g_GameInfo.lpvSelectedSprite = pi->lpvData;
						g_GameInfo.nSelectedSpriteType = pi->wType;
						g_GameInfo.lpcharacter = (LPCHARACTER)pi->lpvData;
						MouseCheckCharacterName = (LPCHARACTER)pi->lpvData;		//1027 YGI
						return;
					}

					MouseCheckCharacterName = (LPCHARACTER)pi->lpvData;

					//	죽어 있을경우..............
					if (IsDead(ch))
					{
						if (SkillOn == -1) break;
					}


					//	일반 PC일겅우에는 
					if (ch->id < 10000)			// 0906 YGI	fightmap_2
					{
						if (g_FightMapStart) /* 전투 맵에서 서로 다른 팀일경우 컨트롤 키 없이도 선택이 가능하게...*/
						{
							if (Hero->fight_map_team_num && ch->fight_map_team_num && Hero->fight_map_team_num != ch->fight_map_team_num) bClickOn = true;
						}

#ifdef _NATION_APPLY_
						else if (ch->name_status.nation != Hero->name_status.nation && ch->lv >= 1)	// 011022 LTS
						{	//< CSD-040414
							if (!CheckNotClickNation())
							{
								bClickOn = true;	// 클릭 불가능이 아니면 // 010724 YGI

								for (int i = 0; i < 6; ++i)
								{
									if (SCharacterData.party[i].ch == ch)
									{
										bClickOn = false;
										break;
									}
								}
							}
						}	//> CSD-040414
#endif
						else if (IsNK(ch) >= 5 && ch != Hero)				bClickOn = true;
						else if (Hero->fight_flag && Hero->fight_id == ch->id) bClickOn = true; // 결투시.. 

						if (g_bWarBegin)	// 전쟁중이면	// 011022 LTS
						{
							if (isOurSquad(Hero, ch)) bClickOn = false; //국가전이고 기타등등이면
							else bClickOn = true;
						}
						if (Hero->DragonLordWarTeam > 0 && ch->DragonLordWarTeam > 0)		// LTS DRAGONLORD
						{
							if (Hero->DragonLordWarTeam != ch->DragonLordWarTeam) bClickOn = true;		// 다른편이면 
							else bClickOn = false;
						}
						if ((MapNumber == 39) && (g_EventLocalWarBegin == 3)) bClickOn = true;	// LTS BUG

						if (!bClickOn)
							if (CheckPC == false && SkillOn == -1 && g_mgrBattle.IsSelect(ch) && !bClickOn)
							{
								break;
							}
					}
					else
					{	//< CSD-031013
						if (CheckPC == false && (ch->HostName[0]))
						{
							if (GetBattlePeace() != PK_MODE)
							{
								break;
							}
						}
					}	//> CSD-031013

					g_GameInfo.lpvSelectedSprite = pi->lpvData;
					g_GameInfo.nSelectedSpriteType = pi->wType;
					g_GameInfo.lpcharacter = (LPCHARACTER)pi->lpvData;
					return;
				}
			break;
		}
	}
}

void DisplaySpriteUnderTile(void)
{
	int i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPORDERTABLE o = &g_OrderInfo.order[i];

		if (o->show == 0) continue;

		switch (o->wType)
		{
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:
		{
			//				LPMAPOBJECT mo;
			if (((LPMAPOBJECT)o->lpvData)->attr == OBJECTATTR_BACK_BELOW)
			{
				o->show = 0;
				DisplayMapObject((LPMAPOBJECT)o->lpvData);
			}
			break;
		}
		}
	}
}
struct BuffTimer
{
	int magicid;
	int duration;
};
void ShowBuffTimer()
{
	BuffTimer Bufft;

	uint16_t verticalSpaceCount = 0;
	CMagicTbl k;

	int spellid = g_CurrUsedMagic;
	int duration = g_curr_time + k.continue_Time;




	static std::vector<BuffTimer> m_BuffTimer;

	vector<BuffTimer>::iterator itr = m_BuffTimer.begin();
	for (itr = m_BuffTimer.begin(); itr != m_BuffTimer.end(); ++itr)

	{
		m_BuffTimer.push_back(*itr);
		verticalSpaceCount += 50;

		PutMagicIcon((SCREEN_WIDTH / 2) - 140, 40 + verticalSpaceCount, g_CurrUsedMagic);
		HprintBold((SCREEN_WIDTH / 2) - 40, 40 + verticalSpaceCount, RGB(20, 255, 20), 0, "Time Buff : %d", duration);
	}
}


void DisplaySpriteBottom(void)
{
	register int	i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE o = &g_OrderInfo.order[i];
		switch (o->wType)
		{
		case SPRITETYPE_EFFECT_BOTTOM:
		{
			o->show = 0;
			switch (o->type)
			{
			case 1:
			case 2:
			case 3:
			case 4:
				PutCompressedImageFX(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->alpha, o->type);
				break;
			case 5:
				PutCompressedImage(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
				break;
			case 6:
				PutCompressedImageBlendRGB(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->rgb);
				break;

			case 7:
				PutCompressedImageVoid(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
				break;

			}
			break;
		}
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:
		{
			//				LPMAPOBJECT mo;
			if (((LPMAPOBJECT)o->lpvData)->attr == OBJECTATTR_BACK_ABOVE)
			{
				o->show = 0;
				DisplayMapObject((LPMAPOBJECT)o->lpvData);
			}
			break;
		}
		case SPRITETYPE_ITEM_BOTTOM:
		{
			o->show = 0;

			LPITEMGROUND  item = (LPITEMGROUND)o->lpvData;
			Spr* sp = item->spr[item->curframe / 2];

			if (item->shadow)	PutCompressedImageShadow(o->x - Mapx, o->y - Mapy, sp);
			PutCompressedImage(o->x - Mapx + item->offx, o->y - Mapy + item->offy, sp);
			if (o->rgb && item->getable)	PutCompressedImageFX(o->x - Mapx + item->offx, o->y - Mapy + item->offy, sp, o->alpha, 2);
			MaskMapItem(o->x + item->offx, o->y + item->offy, o->y, sp);
#ifdef _DEBUG
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				if (item->type == ITEMTYPE_DOOR)
				{
					Hcolor(0, 0xff, 0);
					Hprint2(item->dx[0] - Mapx, item->dy[0] - 40 - Mapy, g_DestBackBuf, "ITEMTYPE_DOOR");
					Hcolor(0xffff);
				}
				Hprint2(item->dx[0], item->dy[0] - 20, g_DestBackBuf, "%d", item->no);

				Box(item->dx[0], item->dy[0], item->dx[1], item->dy[1], item->dx[2], item->dy[2], item->dx[3], item->dy[3], 0xffff);
			}
#endif
			break;
		}
		//			case  SPRITETYPE_DEST_POINT_BOTTOM :
		//			{
		//				o->show = 0;
		//				PutCompressedImageFX( o->x-Mapx, o->y-Mapy, (Spr*)(o->lpvData), o->alpha, 1 );
		//				break;
		//			}

		}
	}
}


void DisplayShadowFirst(void)
{
	register int	i;


	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPORDERTABLE o = &g_OrderInfo.order[i];

		switch (o->wType)
		{
		case SPRITETYPE_ON_THE_CEILING_CHAR:
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		{
			LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
			if (ch->sprno > 1)	h_DisplayNPC(ch, 0);
			else h_DisplayPC(ch, 0);
			break;
		}
		}
	}
}



void DisplayTileFirst(void)
{
	register int	i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE	o = &g_OrderInfo.order[i];

		if (o->show == 0) continue;

		switch (o->wType)
		{
		case	SPRITETYPE_TILE:
		{
			o->show = 0;
			int mx = o->x - Mapx;
			int my = o->y - Mapy;
			int front = ((LPTILE)o->lpvData)->front;

			if (Sou[front].xl == -1)  PutTile(mx, my, Sou[front].img, g_DestBackBuf);
			else						PutCompressedImage(mx, my, &Sou[front]);
			break;
		}
		}
	}
}



extern int PutGrowingGauge(int x, int y);
extern Spr	  SquadPic[3][4];		// 011018 LTS
extern void DrawSquadPic(int x, int y, LPORDERTABLE o, LPCHARACTER ch);
extern void	DrawLocalWarSquadPic(int x, int y, LPORDERTABLE o, LPCHARACTER ch);	// LTS HORSERIDER
extern bool isEnemy(LPCHARACTER ch1, LPCHARACTER ch2);	// LTS DRAGON BUG
extern int		g_LocalWarBegin;		// LTS DRAGON BUG

void DisplaySprite_Character(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
	if (tool_MultiChar)
	{
		int tx = ch->x;
		int ty = ch->y;
		ch->x = o->x;
		ch->y = o->y;
		if (TileMap[ch->x / TILE_SIZE][ch->y / TILE_SIZE].attr_dont == 0)
		{
			if (ch->sprno > 1)	h_DisplayNPC(ch, 1);
			else h_DisplayPC(ch, 1);
		}
		ch->x = tx;
		ch->y = ty;
	}
	else
	{
		if (ch->sprno > 1)	h_DisplayNPC(ch, 1);
		else h_DisplayPC(ch, 1);
	}

	x = ch->x - Mapx + 1;
	y = ch->y - Mapy - CharSpr[ch->sprno].sp[0][0].yl - 20;

	if (o->wType == SPRITETYPE_CHARACTER) // 011018 LTS
	{
		if (ch == Hero)//020626 lsw
		{
			switch (Hero->viewtype)
			{	//< CSD-030509
			case VIEWTYPE_GHOST_:
			case VIEWTYPE_OBSERVE_:
			{
				break;
			}
			default:
			{
				if (Hero->HorseNo)
				{
					::PutHeroHpGauge(x - 15, y - ch->height - 30);	   // npc HP게이지 찍기  // 1015 YGI
					::PutHeroMpGauge(x - 15, y - ch->height + 3 - 30); // npc HP게이지 찍기  // 1015 YGI
				}
				else
				{
					::PutHeroHpGauge(x - 15, y - ch->height + 10);	   // npc HP게이지 찍기  // 1015 YGI
					::PutHeroMpGauge(x - 15, y - ch->height + 3 + 10); // npc HP게이지 찍기  // 1015 YGI
				}

				break;
			}
			}	//> CSD-030509		
		}

		if (ch->HorseNo)
		{
			::DrawSquadPic(o->x - Mapx, o->y - Mapy - 115, o, ch);
		}
		else
		{
			::DrawSquadPic(o->x - Mapx, o->y - Mapy - 90, o, ch);
		}
		if (ch->JoinLocalWar)
		{
			if (ch->HorseNo)
			{
				DrawLocalWarSquadPic(o->x - Mapx, o->y - Mapy - 115, o, ch);	// LTS HORSERIDER
			}
			else
			{
				DrawLocalWarSquadPic(o->x - Mapx, o->y - Mapy - 90, o, ch);	// LTS HORSERIDER
			}
		}
	}
#ifdef _DEBUG
	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		if (ch->dieani)  // 0910 KHS
		{
			::HprintBold(x + 10 - 40, y - 4 - ch->height, RGB(100, 100, 0xff), 0, "%d", ch->dieani);
		}

		if (ch->id > 10000)
		{
			if (ch->ChairNum)
			{
				::HprintBold(x + 10 - 40, y - 4 - ch->height + 20, RGB(255, 255, 255), 0, "S_AI");	// LTS AI
			}
			else
			{
				::HprintBold(x + 10 - 40, y - 4 - ch->height + 40, RGB(255, 255, 255), 0, "C_AI");	// LTS AI
			}
		}
	}
#endif

	int ttty;
	if (ch->viewtype == VIEWTYPE_GHOST_)
	{
		ttty = -15;		// 1027 YGI
	}
	else
	{
		ttty = 0;
	}

	if (!GetSysInfo(SI_GAME_MAKE_MODE))
	{
		if (ch != Hero)					// LTS DRAGON MODIFY
		{	//< CSD-040322 : 국지전시 타국가의 정보가 보이도록 수정
			/*
			if (g_LocalWarBegin)
			{
				if (ch->sprno<=1&&Hero->JoinLocalWar&&ch->JoinLocalWar)
				{
					if (ch->name_status.nation!=Hero->name_status.nation)
					{
						return;
					}
				}
			}*/
			//> CSD-040322
			if (g_bWarBegin)
			{
				if (ch->sprno <= 1)
				{
					if (isEnemy(Hero, ch)) { return; }
				}
			}
			if (g_EventLocalWarBegin)
			{
				if (MapInfo[MapNumber].port == 5390)
				{
					if (ch->sprno <= 1) { return; }
				}
			}
			if (Hero->DragonLordWarTeam > 0 && ch->DragonLordWarTeam > 0)		// LTS DRAGONLORD
			{
				if (Hero->DragonLordWarTeam != ch->DragonLordWarTeam)		// 다른팀이면 
				{
					return;
				}
			}
		}
	}


	if (ch->smiledelay)
	{// 031112 kyo
		::DisplayEmote(x, y, ch->smileno);//020515 lsw
	}

	if (ch->namedisplaycount > 1 && !ch->ChatDelay)
	{
		int tttx = spr[71].xl / 2; // 이름글씨뒤에 찍히는 배경..
		if (ch->type == SPRITETYPE_CHARACTER) // PC들만 사진이 있다....
		{	//< CSD-030324
			if (!CheckPC && CheckGuildCode(ch->GetGuildCode()))		// 1027 YGI		// 컨트롤을 눌렀을 경운 
			{
				::PutGuildImage(x - tttx + -20, y - 24 - ch->height + ttty, ch->GetGuildCode());		// 길드 마크를 찍는다.
			}
			else
			{
				::PutCharImage(x - tttx + -20, y - 24 - ch->height + ttty, ch->face, 0, FS_MICRO, 0, ch->sex);
			}
		}	//> CSD-030324
		else
		{
			::PutHpGauge(x + 5, y - ch->height, ch);		// npc HP게이지 찍기  // 1015 YGI
		}

		if (ch->name[0])
		{	//< CSD-031013	
			if (ch->type == SPRITETYPE_CHARACTER)
			{
				if (ch->smiledelay)
				{
					::DisplayEmote(x, y, ch->smileno);//020515 lsw
				}

#ifdef _NATION_APPLY_
				if (!CheckPC)// 1027 YGI
				{
					if (CheckGuildCode(ch->GetGuildCode()))// 1004 YGI
					{	//< CSD-030324
						char* guild_text;
						if (g_GuildMgr[ch->GetGuildCode()].first)
						{
							guild_text = g_GuildMgr[ch->GetGuildCode()].m_szName;
						}
						else
						{
							guild_text = CallGuildDegreeName(ch->GetGuildCode(), ch->name_status.guild_master);
						}
						int TxtWidth = GetTextWidth("%s", strlen(guild_text), guild_text);
						PutLvNameByColor(x - tttx + TxtWidth + 15, y - 4 - 15 - ch->height + ttty, ch);		// 레벨 출력

					}	//> CSD-030324
					else
					{
						PutLvNameByColor(x - tttx + 10, y - 4 - 15 - ch->height + ttty, ch);		// 레벨 출력
					}
					PutDualMark(x - tttx + 5, y - 4 - 20 - ch->height + ttty, ch);
				}
				else
				{
					// 직업 레벨 출력
					if (ch->job)
					{
						PutJobLv(x - tttx + 10, y - 19 - ch->height + ttty, ch->job, ch->call_job_lv);		// 1004 YGI( 직업 등급 출력 )
					}
				}

#else
				PutLvNameByColor(x - tttx + 10, y - 4 - 15 - ch->height + ttty, ch);		// 0811 YGI
#endif
				char nation[20];
				int  nk;

				nk = IsNK(ch, nation);
				if (nk >= 5 || (nk > 0 && ch == Hero))
				{
					::HprintBold(x - tttx + 10, y - 4 - ch->height + ttty, RGB(0xff, 0xff, 0xff), 0, "%s", ch->name);
					::HprintBold(x - tttx + 10 + strlen(ch->name) * 6 + 6, y - 4 - ch->height + ttty, RGB(0xff, 0, 0), 0, "(%s%d)", nation, nk);
				}
				else
				{
					::HprintBold(x - tttx + 10, y - 4 - ch->height + ttty, RGB(0xff, 0xff, 0xff), 0, "%s", ch->name);
				}
				// 나라이름을 출력.....
#ifdef _NATION_APPLY_				
				if (!CheckPC && CheckGuildCode(ch->GetGuildCode()))// 1004 YGI
				{	//< CSD-030324
					PutGuildLvByColor(x - tttx + 10, y - 19 - ch->height + ttty, ch->GetGuildCode(), ch->name_status.guild_master);	// 길드내 직책 출력
				}	//> CSD-030324
//					else
				{
					switch (ch->name_status.nation)		// 1004 YGI
					{
					case 0:
						break;
					default:	::HprintBold(x - tttx + 10, y - 34 - ch->height + ttty, NationColor[ch->name_status.nation], 0, "%s", NationName[ch->name_status.nation]);		// 001029 YGI( 나라명 출력 )
						break;
					}
				}

				if (ch->name_status.king)
				{
					::HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(200, 0xff, 0xff), 0, "[%s]", PosOfNation[ch->sex]);		// 1031 YGI( 나라명 출력 )
				}
#endif						
				if (ch->name_status.reporter)
				{
					// 기자단
					::HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(200, 0xff, 0xff), 0, kein_GetMenuString(93));
				}
				if (ch->name_status.ChampionTitle)
				{

					switch (ch->name_status.ChampionTitle)
					{

					case 1:

						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2023]");
						break;
					case 2:
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2023]");
						break;
					case 3:
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2024]");
						break;
					case 4:
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2025]");
						break;
					case 5:
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Champion 2026]");
						break;
					}

				
		
				}
				else
				{
					switch (ch->name_status.counselor)
					{
					case 1:	// 운영자
					{
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, kein_GetMenuString(42)); //Game Master
						break;
					}
					case 2:	// 도우미
					{
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, kein_GetMenuString(43)); //Helper
						break;
					}
					case 3:
					{
						HprintBold(x - tttx + 10, y - 52 - ch->height + ttty, RGB(250, 255, 255), 0, "[Development Team]");
					}
					}
				}
			}
			else
			{
				if (ch->nRace != HUMAN)
				{
					EatRearWhiteChar(g_infNpc[ch->sprno].szName);
					//::HprintBold( x -tttx+ 10, y-4-15-ch->height , RGB( 0, 0xff, 0 ),0, "%s(Lv:%d)", MonsterName[ ch->sprno].name, ch->lv );		// 0808 YGI NPC
					const int nBaseX = x - tttx + 10;
					const int nBaseY = y - ch->height - 19;

					if (ch->nAttr == AT_NORMAL)
					{
#ifdef _DEBUG
						if (tool_ViewMonsterNo)
							HprintBold(nBaseX, nBaseY, RGB(0xff, 0xff, 0x00), 0, "Lv%d.%s(%d)", g_infNpc[ch->GetNpcLevel()].szName, g_infNpc[ch->GetSprNo()].szName, ch->GetID());
						else
#endif
					  
						
						HprintBold(nBaseX, nBaseY, RGB(0xff, 0xff, 0), 0, "Level:%d %s", g_infNpc[ch->GetNpcLevel()].szName, g_infNpc[ch->sprno].szName);
					}
					else
					{
						const int nName = 190 + ch->nAttr;
						const int nOffset = (strlen(lan->OutputMessage(4, nName)) + 1) * FONTSIZE_;
						HprintBold(nBaseX, nBaseY, RGB(0xff, 0, 0), 0, "%s", lan->OutputMessage(4, nName));
						HprintBold(nBaseX + nOffset, nBaseY, RGB(0xff, 0xff, 0), 0, "%s", g_infNpc[ch->sprno].szName);
					}
				}
				// Tamming된 Character
				if (ch->HostName[0])
				{
					HprintBold(x - tttx + 10, y - 4 - ch->height, RGB(100, 100, 0xff), 0, "%s(%s)", ch->name, ch->HostName);
				}
				else
				{
					HprintBold(x - tttx + 10, y - 4 - ch->height, RGB(0xff, 0xff, 0xff), 0, "%s", ch->name);
				}
			}
		}	//> CSD-031013
#ifdef _DEBUG
		if (GetSysInfo(SI_GAME_MAKE_MODE))
		{
			::HprintBold(x - tttx - 30, y - 4 - ch->height + 32, RGB(0xff, 0xff, 0xff), 0, "%d, %d", ch->sprno, ch->id);
		}
#endif
	}
	else if (ch->fight_map_team_num)// 0906 YGI	fightmap_2
	{
		switch (ch->viewtype)
		{	//< CSD-030520
		case VIEWTYPE_GHOST_:
		case VIEWTYPE_OBSERVE_:
		case VIEWTYPE_TRANSPARENCY_:
		{
			break;
		}
		default:
		{
			::HprintBold(x - 10, y - 4 - ch->height, FightTeamColor[ch->fight_map_team_num - 1], 0, "%s", FightTeam[ch->fight_map_team_num - 1]);
			break;
		}
		}	//> CSD-030520
	}

	o->show = 0;
}


void DisplaySprite_Item(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
	o->show = 0;
	LPITEMGROUND  item = (LPITEMGROUND)o->lpvData;

	int skip = 0;
	const int iIX = item->x;
	const int iIY = item->y;
	const int iIOX = item->offx;
	const int iIOY = item->offy;

	switch (item->Anitype)
	{
	case ITEMANIMATION_FIRE:
	{
		::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, item->spr[0]);
	}break;
	case ITEMANIMATION_ACTIVE: // 020701 YGI
	{
		switch (item->attr[IATTR_SPRITE_TYPE] & 0x0003)
		{
		case OBJECTSPRITE_ALPHA:
		{
			::PutCompressedImage(iIX - Mapx, iIY - Mapy, item->spr[0]);
		}
		case OBJECTSPRITE_TRANS:
		{
			::PutCompressedImageFX(iIX - Mapx, iIY - Mapy, item->spr[item->curframe / 2], item->attr[IATTR_SPRITE_TYPE] >> 3, 2);
			OutputLight(iIX, iIY, 16);		// 광원
		}break;
		}
		skip = 1;
	}break;
	}
	if (skip) { return; }
	if (item->curframe < 0)
	{
		item->curframe = 0;		// 020701 YGI
	}
	///////////////////////////////////////

	Spr* sp = item->spr[item->curframe / 2];
	//if( item->shadow)	::PutCompressedImageShadow(  o->x - Mapx, o->y-Mapy, sp );	

	if (::CanViewBomb(Hero)) //020320 lsw
	{
		if (item->attr[IATTR_ATTR] & IA2_TRAPED0)
		{
			::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
		}
	}
	if (::CanViewTrap(Hero)) //020320 lsw
	{
		if (item->attr[IATTR_ATTR] & IA2_TRAPED1)
		{
			::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
		}
	}

	if (item->attr[IATTR_ATTR] & IA2_FIRE) // 불을 피웠어..
	{
		::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY - 16, sp, 10 + (rand() % 10), 2);
	}
	else if (item->attr[IATTR_ATTR] & IA2_HIDE) // 안보이는 속성이면...
	{
		if (Hero->canviewitem)
		{
			::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, rand() % 15, 2);
			::PutCompressedImageVoid(iIX - Mapx + iIOX + rand() % 4 - 2, iIY - Mapy + iIOY + rand() % 4 - 2, sp);
		}
		else
		{
			// 않찍어.. 왜.. 안보이니까...
		}
	}
	else if (item->attr[IATTR_ATTR] & IA2_COLOSSUS_STONE)
	{	//팀에 따른 컬러 변화..
		const int team = GetTeamByStone(item->attr[IATTR_ATTR]);
		sp = item->spr[team + 1];
		if (sp->img)
		{
			::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
		}
	}
	else if (item->attr[IATTR_ATTR] & IA2_DOOR)
	{
		::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
	}
	else
	{	//< CSD-031029
		if (item->no / 1000 == 6)
		{	//< CSD-031101
			CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(item->no));

			if (pWeapon != NULL)
			{
				const int nEffect = pWeapon->effect;

				if (nEffect > 0)
				{

					::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, item->spr[0]);

					if (item->curframe / 2 != 0)
					{
						::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, item->spr[item->curframe / 2], 10 + Random(4), 2);
					}
					else
					{
						::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, item->spr[1], 10 + Random(4), 2);
					}
				}
				else
				{
					if (item->attr[IATTR_RARE_MAIN]) //레어류의 좋은 것이라면
					{
						::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
						::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, rand() % 15, 2);
					}
					else
					{
						::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
					}
				}
			}
			else
			{
				if (item->attr[IATTR_RARE_MAIN]) //레어류의 좋은 것이라면
				{
					::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
					::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, rand() % 15, 2);
				}
				else
				{
					::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
				}
			}
		}	//> CSD-031101
		else
		{
			if (item->attr[IATTR_RARE_MAIN]) //레어류의 좋은 것이라면
			{
				::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
				::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, rand() % 15, 2);
			}
			else
			{
				::PutCompressedImage(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp);
			}
		}
	}	//> CSD-031029

	if (o->rgb)
	{	//	안보여도 선택은 될수 있다. 
		if ((CanViewBomb(Hero) && (item->attr[IATTR_ATTR] & IA2_TRAPED0))
			|| (CanViewTrap(Hero) && (item->attr[IATTR_ATTR] & IA2_TRAPED1)))
		{
		}
		else
		{
			if (item->attr[IATTR_ATTR] & IA2_FIRE) // 불을 피웠어..
			{
				::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY - 16, sp, o->alpha, 2);
			}
			else
			{
				::PutCompressedImageFX(iIX - Mapx + iIOX, iIY - Mapy + iIOY, sp, o->alpha, 2);
			}
		}

		if (CursorCheckedItem)
		{
			if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM0)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM1)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM2)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM3)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM4)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM5)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM6)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
			else if (CursorCheckedItem->attr[IATTR_ATTR] & ITEMANIMATION_FARM7)
			{
				::PutGrowingGauge(CursorCheckedItem->x - Mapx + CursorCheckedItem->offx, CursorCheckedItem->y - Mapy + CursorCheckedItem->offy - 20);
			}
		}
	}

	if (sp)
	{
		::MaskMapItem(iIX + iIOX, iIY + iIOY, iIY, sp); // 020620 YGI
	}
#ifdef _DEBUG	
	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		if (item->type == ITEMTYPE_DOOR)
		{
			Hcolor(0, 0xff, 0);
			::Hprint2(item->dx[0] - Mapx, item->dy[0] - 40 - Mapy, g_DestBackBuf, "ITEMTYPE_DOOR");
			Hcolor(0xffff);
		}

		::Hprint2(item->dx[0] - Mapx, item->dy[0] - 20 - Mapy, g_DestBackBuf, "%d", item->no);
		Box(item->dx[0], item->dy[0], item->dx[1], item->dy[1], item->dx[2], item->dy[2], item->dx[3], item->dy[3], 0xffff);

		int cc = item->curframe / 2;
		Line(item->dsx[cc], item->dsy[cc], item->ddx[cc], item->ddy[cc], RGB16(0, 0, 255));
	}
#endif
}

void DisplaySprite_Effect(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
	switch (o->type)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		::PutCompressedImageFX(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->alpha, o->type);
		break;
	case 5:
		::PutCompressedImage(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
		break;
	case 6:
		::PutCompressedImageBlendRGB(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->rgb);
		break;
	case 7:
		::PutCompressedImageVoid(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
		break;
	case 8:			//0502 lkh 추가
		::PutCompressedImageVoid(o->x + (rand() % 5 - 2) - Mapx, o->y - o->height + (rand() % 5 - 2) - Mapy, (Spr*)(o->lpvData));
		::PutCompressedImageFX(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->alpha, 2);
		break;
	}
	::MaskMapEffect(o->x, o->y, o->height, (Spr*)(o->lpvData));
}

void DisplaySprite_Tile(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
#ifdef _DEBUG	
	if (tool_DrawMap) { return; }
#endif
	o->show = 0;
	const int mx = o->x - Mapx;
	const int my = o->y - Mapy;
	const int front = ((LPTILE)o->lpvData)->front;

	if (Sou[front].xl == -1)
	{
		//		if( mx >= 0 && mx < SCREEN_WIDTH && my >= 0 && my < SCREEN_HEIGHT )
		::PutTile(mx, my, Sou[front].img, g_DestBackBuf);
	}
	else
	{
		::PutCompressedImage(mx, my, &Sou[front]);
	}
}

void DisplaySprite_DestPoint(int& x, int& y, LPCHARACTER ch, LPORDERTABLE o)//021108 lsw
{
	o->show = 0;
	if (o->rgb)
	{
		::PutCompressedImage(o->x - Mapx, o->y - Mapy, (Spr*)(o->lpvData));
	}
	else
	{
		::PutCompressedImageFX(o->x - Mapx, o->y - Mapy, (Spr*)(o->lpvData), o->alpha, 1);
	}
}

//////////////////////// 0502 lkh 추가 (8번 패턴->Void & High Trans 복합) ////////////////////////////
void DisplaySprite(void)
{
	int x = 0, y = 0;
	int count = 0;
	for (register int i = 0; i < NewOrderC; i++)
	{
		LPCHARACTER	ch = (LPCHARACTER)neworder[i].lpvData;
		LPORDERTABLE o = &neworder[i];

		if (o->show == 0) continue;
		switch (o->wType)
		{
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:
		{
			::DisplaySprite_Character(x, y, ch, o);//021108 lsw		
		}break;
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:
		{
			if (((LPMAPOBJECT)o->lpvData)->attr != OBJECTATTR_BACK_ABOVE)
				if (((LPMAPOBJECT)o->lpvData)->attr != OBJECTATTR_BACK_BELOW)
				{
					::DisplayMapObject((LPMAPOBJECT)o->lpvData);
				}
			o->show = 0;
		}break;
		case SPRITETYPE_EFFECT_NORMAL:
		{
			::DisplaySprite_Effect(x, y, ch, o);
		}break;
		case SPRITETYPE_ITEM:
		{//021108 lsw
			::DisplaySprite_Item(x, y, ch, o);
		}break;
		case	SPRITETYPE_TILE:
		{
			::DisplaySprite_Tile(x, y, ch, o);
		}break;
		case  SPRITETYPE_DEST_POINT:
		{
			::DisplaySprite_DestPoint(x, y, ch, o);
		}break;
		case SPRITETYPE_SKILLGAUGE:
		{
			o->show = 0;
			LPSKILLGAUGE n = (LPSKILLGAUGE)o->lpvData;
			::DrawSkillGauge(n);
		}break;
		//////////////////////////// 0126 lkh ////////////////////////////
		case SPRITETYPE_CLOCKNIDDLE:
		{
			o->show = 0;
		}break;
		default:
		{
			continue;
		}break;
		}
	}
}

void DisplaySpriteNo(void)
{
#ifdef _DEBUG // finito 060507
	register int	i;
	int count = 0;

	//	return;

	if (GetSysInfo(SI_GAME_MAKE_MODE) == 0) return;

	for (i = 0; i < NewOrderC; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)neworder[i].lpvData;
		//g_OrderInfo.order[ i ].lpvData;
		LPORDERTABLE o = &neworder[i];

		//		HprintBold( o->x -Mapx, o->y - Mapy, 0xffff, 0, "%d", count  );

		switch (neworder[i].wType)
		{
		case SPRITETYPE_CHARACTER:
		case SPRITETYPE_MONSTER:
		case SPRITETYPE_NPC:	HprintBold(o->x - Mapx, o->y - Mapy - 60, RGB(0xff, 0, 0), 0, "      VY_NK:%2d", ch->nk3);
			HprintBold(o->x - Mapx, o->y - Mapy - 45, RGB(0xff, 0, 0), 0, "      ZY_NK:%2d", ch->nk4);
			HprintBold(o->x - Mapx, o->y - Mapy - 30, RGB(0xff, 0, 0), 0, "      YI_NK:%2d", ch->nk6);

			break;
		}
		count++;
	}
#endif
}

void DisplaySkillCursor(void)
{
	int i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPSKILLCURSOR sc = (LPSKILLCURSOR)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE o = &g_OrderInfo.order[i];

		if (o->show == 0) continue;

		switch (o->wType)
		{
		case SPRITETYPE_SKILLCURSOR:
		{
			PutCompressedImage(o->x - Mapx, o->y - Mapy, sc->sp);
#ifdef _DEBUG
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				HprintBold(o->x - Mapx, o->y - Mapy, 0xffff, 0, "%d %d", sc->skillno, sc->cursorno);
			}
#endif
		}break;
		}
	}
}



void DisplaySpriteCeiling(void)
{
	register int	i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE o = &g_OrderInfo.order[i];

		if (o->show == 0) continue;

		switch (o->wType)
		{
		case SPRITETYPE_EFFECT_CEILING:
		{
			switch (o->type)
			{
			case 1:
			case 2:
			case 3:
			case 4:
				PutCompressedImageFX(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->alpha, o->type);
				break;
			case 5:
				PutCompressedImage(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
				break;
			case 6:
				PutCompressedImageBlendRGB(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData), o->rgb);
				break;

			case 7:
				PutCompressedImageVoid(o->x - Mapx, o->y - o->height - Mapy, (Spr*)(o->lpvData));
				break;

			}
			break;
		}

		case SPRITETYPE_ON_THE_CEILING_CHAR:
		{

			if (tool_MultiChar)
			{
				int tx = ch->x;
				int ty = ch->y;
				ch->x = o->x;
				ch->y = o->y;
				if (TileMap[ch->x / TILE_SIZE][ch->y / TILE_SIZE].attr_dont == 0)
				{
					if (ch->sprno > 1)	h_DisplayNPC(ch, 1);
					else h_DisplayPC(ch, 1);
				}
				ch->x = tx;
				ch->y = ty;
			}
			else
			{
				if (ch->sprno > 1)	h_DisplayNPC(ch, 1);
				else h_DisplayPC(ch, 1);
			}

			// ####나중에지워...
			int x = ch->x - Mapx + 1;
			int y = ch->y - Mapy - CharSpr[ch->sprno].sp[0][0].yl - 20;


			if (ch->namedisplaycount > 5 && !ch->ChatDelay)
			{
				int tttx = spr[71].xl / 2; // 이름글씨뒤에 찍히는 배경..
				if (ch->type == SPRITETYPE_CHARACTER) // PC들만 사진이 있다....
				{
					PutCharImage(x - tttx + -20, y - 24 - ch->height, ch->face, 0, FS_MICRO, 0, ch->sex);
				}
				if (ch->name[0])
				{
					//PutCompressedImageFX( x, y, &spr[ 71], 10, 1 );	
					if (ch->HostName[0]) //  Tamming된 Character.
					{
						HprintBold(x - tttx + 10, y - 4 - ch->height, RGB(100, 100, 0xff), 0, "%s(%s)", ch->name, ch->HostName);
						if (strcmp(ch->HostName, Hero->HostName) == 0)
						{
							//  내 동물에 대한 레벨을 보여준다. 
						}

					}
					else
					{
						HprintBold(x - tttx + 10, y - 4 - ch->height, RGB(0xff, 0xff, 0xff), 0, "%s", ch->name);
					}
				}
#ifdef _DEBUG
				if (GetSysInfo(SI_GAME_MAKE_MODE))
				{
					HprintBold(x - tttx - 70, y - 4 - ch->height, RGB(0xff, 0xff, 0xff), 0, "Sprno:%d", ch->sprno);
				}
#endif
			}
			else
				if (ch->smiledelay)
				{
					PutCompressedImage(x - 16, y, &spr[267 + ch->smileno]);
				}

			o->show = 0;
			break;
		}
		}
	}
}



void DisplaySpriteTrans(void)
{
	register int	i;
	LPORDERTABLE o;


	for (i = 0; i < g_OrderInfo.count; i++)
	{
		if (0)
			if (GetSysInfo(SI_GAME_MAKE_MODE))
			{
				switch (g_OrderInfo.order[i].wType)
				{
				case	SPRITETYPE_CHARACTER:
				case	SPRITETYPE_MONSTER:
				case	SPRITETYPE_NPC:
				{
					LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
					Box_(ch->x - ch->sp->ox, ch->y - ch->sp->oy, ch->sp->xl, ch->sp->yl, RGB16(0, 255, 0));
					break;
				}
				//				case SPRITETYPE_ALPHAOBJECT :
				//				case SPRITETYPE_OBJECT :
				//				{	
				//					DisplayMapObject(( LPMAPOBJECT )o->lpvData );
				//				}	
				case	SPRITETYPE_TILE:
				{
					Box_(o->x, o->y, TILE_SIZE, TILE_SIZE, RGB16(255, 255, 0));
					break;
				}
				case  SPRITETYPE_DEST_POINT_BOTTOM:
				{
					Spr* sp = (Spr*)(o->lpvData);
					Box_(o->x - sp->ox, o->y - sp->oy, sp->xl, sp->yl, RGB16(255, 255, 255));
					break;
				}
				case SPRITETYPE_ITEM:
				case SPRITETYPE_ITEM_BOTTOM:
				{
					LPITEMGROUND  item = (LPITEMGROUND)o->lpvData;
					Spr* sp = item->spr[item->curframe / 2];
					Box_(o->x - sp->ox + item->offx, o->y - sp->oy + item->offy, sp->xl, sp->yl, RGB16(255, 255, 255));

					Box_(o->x - sp->ox + item->offx, o->y - sp->oy + item->offy, sp->xl, sp->yl, RGB16(255, 255, 255));
					break;
				}
				case SPRITETYPE_EFFECT_CEILING:
				case SPRITETYPE_EFFECT_NORMAL:
				case SPRITETYPE_EFFECT_BOTTOM:
				{
					Spr* sp = (Spr*)(o->lpvData);
					Box_(o->x - sp->ox, o->y - sp->oy, sp->xl, sp->yl, RGB16(0, 0, 255));
					break;
				}
				}
				Hcolor(255, 0, 0);
				Hprint2(o->x - Mapx, o->y - Mapy, g_DestBackBuf, "%d", o->rank);
				Hcolor(255, 255, 255);
				Hprint2(o->x - Mapx, o->y - Mapy, g_DestBackBuf, "%d", o->rank);
			}

		switch (g_OrderInfo.order[i].wType)
		{
		case	SPRITETYPE_CHARACTER:
		case	SPRITETYPE_MONSTER:
		case	SPRITETYPE_NPC:
		{
			LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
			switch (ch->sprno)
			{
			case 0:
			case 1: h_DisplayPC2(ch);	break;
			}
			break;
		}
		}
	}
}

void _DisplayMapObjectAfter(void)
{
#ifdef _DEBUG // finito 060507
	int i;

	if (!GetSysInfo(SI_GAME_MAKE_MODE)) return;


	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPORDERTABLE o = &g_OrderInfo.order[i];
		LPMAPOBJECT mo = (LPMAPOBJECT)o->lpvData;
		switch (o->wType)
		{
		case SPRITETYPE_ALPHAOBJECT:
		case	SPRITETYPE_OBJECT:
		{
			DisplayMapObjectAfter(mo, IDCheckedMapObject == (int)o->rgb);


			switch (mo->objectoritem)
			{
				//  기술용작업대..
			case 71:		break;//010215 lsw
			case 72:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 12));		break;//
			case 73:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 13));		break;//
			case 74:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 14));		break;//
			case 75:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 15));		break;//
			case 76:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 16));	break;//
			case 77:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 17));		break;//
			case 78:HprintBold(o->x - Mapx, o->y - Mapy + 12, RGB(255, 255, 255), RGB16(0, 0, 0), lan->OutputMessage(9, 18));		break;//010215 lsw
				break;
			}

			if (mo->objectoritem == 0) // mapobject
			{
			}
			else
			{
				HprintBold(o->x - Mapx, o->y - Mapy, RGB(255, 255, 255), RGB16(0, 0, 0), "Item:(%d)", mo->dum);
			}
			break;
		}
		}
	}
#endif
}


///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void smallmap_CalcOrder(void)
{
	register int	i, j;
	ORDERTABLE		temp;
	LPTILE			lpTile;

	LPORDERTABLE  o, oi, oj;


	g_OrderInfo.count = 0;

	//MapObjectTouchOrder();
	// character
	// object							

	for (i = 0; i < TotalMapObject; i++)
	{
		if (IsScreenAreaCSP(Mo[i].x + Mo[i].offx, Mo[i].y + Mo[i].offy, &MoImage[Mo[i].id - 1].spr[0]))
		{
			if (Mo[i].attr == OBJECTATTR_BACK_ABOVE || Mo[i].attr == OBJECTATTR_FRONT_ABOVE ||

				Mo[i].attr == OBJECTATTR_BACK_BELOW)
			{
				if (g_OrderInfo.count >= MAX_ORDERTABLE) break;

				o = &g_OrderInfo.order[g_OrderInfo.count];

				o->wType = SPRITETYPE_OBJECT;
				o->x = Mo[i].x;
				o->y = Mo[i].y;
				o->offset = Mo[i].y;
				o->rgb = i; // rgb로 쓰는거이 아니라 배열번호를 쓴다. 

				o->height = 0;
				o->lpvData = (LPVOID)(&Mo[i]);
				o->show = 1;
				o->sort = 1;

				g_OrderInfo.count++;
			}
		}
	}

	// front
	for (j = 0; j < g_Map.wScreenHeight; j++)
	{
		for (i = 0; i < g_Map.wScreenWidth; i++)
		{
			lpTile = &TileMap[g_Map.x + i][g_Map.y + j];

			if (lpTile->front)
			{
				if (g_OrderInfo.count >= MAX_ORDERTABLE) break;
				o = &g_OrderInfo.order[g_OrderInfo.count];
				o->wType = SPRITETYPE_TILE;
				o->x = (g_Map.x + i) * TILE_SIZE;
				o->y = (g_Map.y + j) * TILE_SIZE;
				o->offset = (lpTile->frontoffset) * TILE_SIZE + TILE_SIZE;
				o->height = 0;
				o->lpvData = lpTile;
				o->show = 1;
				o->sort = 1;

				g_OrderInfo.count++;
			}
		}
	}

	// sorting...
	for (i = 0; i < g_OrderInfo.count - 1; i++)
	{
		for (j = i + 1; j < g_OrderInfo.count; j++)
		{
			oi = &g_OrderInfo.order[i];
			oj = &g_OrderInfo.order[j];

			if (oi->offset > oj->offset)
			{
				memcpy(&temp, oj, sizeof(ORDERTABLE));
				memcpy(oj, oi, sizeof(ORDERTABLE));
				memcpy(oi, &temp, sizeof(ORDERTABLE));
			}
			else if (oi->offset == oj->offset)
			{
				if (oj->wType == SPRITETYPE_TILE)
				{
					switch (oi->wType)
					{
					case	SPRITETYPE_CHARACTER:
					{
						memcpy(&temp, oj, sizeof(ORDERTABLE));
						memcpy(oj, oi, sizeof(ORDERTABLE));
						memcpy(oi, &temp, sizeof(ORDERTABLE));
						break;
					}
					case SPRITETYPE_ALPHAOBJECT:
					case	SPRITETYPE_OBJECT:
					{
						LPMAPOBJECT	mo = (LPMAPOBJECT)oi->lpvData;

						if (mo->attr == OBJECTATTR_BACK_ABOVE)
						{
							memcpy(&temp, oj, sizeof(ORDERTABLE));
							memcpy(oj, oi, sizeof(ORDERTABLE));
							memcpy(oi, &temp, sizeof(ORDERTABLE));
						}

						/*LPOBJECT	lpObject = ( LPOBJECT )oi->lpvData;

						if ( lpObject->ObjectAttr.cAttr == OBJECTATTR_BACK_ABOVE )
						{
							memcpy( &temp,	oj, sizeof( ORDERTABLE ) );
							memcpy( oj,		oi, sizeof( ORDERTABLE ) );
							memcpy( oi,	 &temp, sizeof( ORDERTABLE ) );
						}*/


						break;
					}
					}
				}
			}
		}
	}
}




void small_DisplaySpriteUnderTile(void)
{
	int i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPORDERTABLE o = &g_OrderInfo.order[i];

		if (o->show == 0) continue;

		switch (o->wType)
		{
		case SPRITETYPE_ALPHAOBJECT:
		case SPRITETYPE_OBJECT:
		{
			//				LPMAPOBJECT mo;
			if (((LPMAPOBJECT)o->lpvData)->attr == OBJECTATTR_BACK_BELOW)
			{
				o->show = 0;
				DisplayMapObject((LPMAPOBJECT)o->lpvData);
			}
			break;
		}
		}
	}
}





void smallmap_DisplaySprite(void)
{
	register int	i;

	for (i = 0; i < g_OrderInfo.count; i++)
	{
		LPCHARACTER		ch = (LPCHARACTER)g_OrderInfo.order[i].lpvData;
		LPORDERTABLE o = &g_OrderInfo.order[i];

		if (o->show == 0) continue;

		switch (o->wType)
		{
		case SPRITETYPE_ALPHAOBJECT:
		case	SPRITETYPE_OBJECT:
		{
			o->show = 0;

			LPMAPOBJECT mo;
			mo = (LPMAPOBJECT)o->lpvData;
			{
				int ox, oy;
				int xl, yl;

				ox = MoImage[mo->id - 1].spr[0].ox;
				oy = MoImage[mo->id - 1].spr[0].oy;
				xl = MoImage[mo->id - 1].spr[0].xl;
				yl = MoImage[mo->id - 1].spr[0].yl;

				if (BoxAndDotCrash(mo->x - Mapx - ox, mo->y - Mapy - oy, xl, yl, Mox - Mapx, Moy - Mapy))
				{
					int mx = mo->x + mo->offx - Mapx;
					int my = mo->y + mo->offy - Mapy;

					IDCheckedMapObject = g_OrderInfo.order[i].rgb;// 배열번호를 나타낸다. 
					Spr* sp;
					sp = &MoImage[mo->id - 1].spr[mo->curframe];

					PutCompressedImage(mo->x - Mapx + +mo->offx, mo->y - Mapy + mo->offy, &MoImage[mo->id - 1].spr[0]);


					switch (mo->attr1 & 0x0003)
					{
					case OBJECTSPRITE_NORMAL:	PutCompressedImage(mx, my, sp);			break;
					case OBJECTSPRITE_TRANS:  PutCompressedImageFX(mx, my, sp, mo->attr1 >> 3, 1);	break;
					case OBJECTSPRITE_ALPHA:
						if (mo->light && DayLightControl >= 31)
						{
						}
						else
						{
							PutCompressedImageFX(mx, my, sp, mo->attr1 >> 3, 2);	break;
						}
					}
				}
				else
				{
					PutCompressedImage(mo->x - Mapx + mo->offx, mo->y - Mapy + mo->offy, &MoImage[mo->id - 1].spr[0]);
				}
			}

			break;
		}
		case	SPRITETYPE_TILE:
		{
			o->show = 0;
			int mx = o->x - Mapx;
			int my = o->y - Mapy;
			int front = ((LPTILE)o->lpvData)->front;

			if (Sou[front].xl == -1)
			{
				//					if( mx >= 0 && mx < SCREEN_WIDTH && my >= 0 && my < SCREEN_HEIGHT )
				PutTile(mx, my, Sou[front].img, g_DestBackBuf);
			}
			else
				PutCompressedImage(mx, my, &Sou[front]);
			break;
		}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////
/////
/////	 전체 레이다....
/////
/////


void ViewAllCharacter(int x, int y)
{
	LPCHARACTER ch = Hero;
	int mxl, myl;
	int px, py;
	WORD color;
	int sxl, syl;
	static int tog;
	int smx, smy, smxl, smyl;

	mxl = g_Map.file.wWidth * TILE_SIZE;
	myl = g_Map.file.wHeight * TILE_SIZE;

	Box(x + Mapx, y + Mapy, x + Mapx + RADARXL, y + Mapy + RADARYL, 0);
	Box(x + Mapx + 1, y + Mapy + 1, x + Mapx + RADARXL + 1, y + Mapy + RADARYL + 1, 0xffff);
	smx = x + Mapx + (Hero->position.x - 20) * RADARXL / g_Map.file.wWidth;
	smy = y + Mapy + (Hero->position.y - 20) * RADARYL / g_Map.file.wHeight;
	smxl = 40 * RADARXL / g_Map.file.wWidth;
	smyl = 40 * RADARYL / g_Map.file.wHeight;
	Box(smx, smy, smx + smxl, smy + smyl, RGB16(0, 0xff, 0xff));

	smx = x + Mapx + (g_Map.x) * RADARXL / g_Map.file.wWidth;
	smy = y + Mapy + (g_Map.y) * RADARYL / g_Map.file.wHeight;
	smxl = 20 * RADARXL / g_Map.file.wWidth;
	smyl = 11 * RADARYL / g_Map.file.wHeight;
	Box(smx, smy, smx + smxl, smy + smyl, RGB16(0, 0xff, 0));

	smx = x + Mapx + (Hero->position.x - 30) * RADARXL / g_Map.file.wWidth;
	smy = y + Mapy + (Hero->position.y - 30) * RADARYL / g_Map.file.wHeight;
	smxl = 60 * RADARXL / g_Map.file.wWidth;
	smyl = 60 * RADARYL / g_Map.file.wHeight;
	Box(smx, smy, smx + smxl, smy + smyl, RGB16(0xff, 0xff, 0xff));



	sxl = RADARXL * 640 / mxl;
	syl = RADARYL * 480 / myl;

	tog = !tog;
	if (tog)
	{
		color = 0xffff;
		while (ch != NULL)
		{
			px = ch->x * RADARXL / mxl;
			py = ch->y * RADARYL / myl;
			PutDot(x + px - 1, y + py, color);
			PutDot(x + px + 1, y + py, color);
			PutDot(x + px - 2, y + py, color);
			PutDot(x + px + 2, y + py, color);
			PutDot(x + px, y + py - 1, color);
			PutDot(x + px, y + py + 1, color);
			color = RGB16(0xff, 0, 0);
			ch = ch->lpNext;
		}
	}
	else
	{
		color = RGB16(0, 0xff, 0);
		while (ch != NULL)
		{
			px = ch->x * RADARXL / mxl;
			py = ch->y * RADARYL / myl;
			PutDot(x + px - 1, y + py, color);
			PutDot(x + px + 1, y + py, color);
			PutDot(x + px - 2, y + py, color);
			PutDot(x + px + 2, y + py, color);
			PutDot(x + px, y + py - 1, color);
			PutDot(x + px, y + py + 1, color);
			color = 0xffff;
			ch = ch->lpNext;
		}
	}



	if (tool_SkillTool_FARMING || tool_SkillTool_MINING || tool_SkillTool_FISHING || tool_SkillTool_CHOPPING || tool_SkillTool_HUB || tool_SkillTool_NPC || tool_SkillTool_ITEM_SEAL)
	{
	}
	else
	{
		LPEVENTLIST e = g_EventListHeaderMap.lpFirst;

		color = RGB16(0, 0, 0xFF);
		while (e != NULL)
		{
			color = 0xffff;
			px = e->x * TILE_SIZE * RADARXL / mxl;
			py = e->y * TILE_SIZE * RADARYL / myl;
			PutDot(x + px - 1, y + py, color);
			PutDot(x + px + 1, y + py, color);
			PutDot(x + px - 2, y + py, color);
			PutDot(x + px + 2, y + py, color);
			PutDot(x + px, y + py - 1, color);
			PutDot(x + px, y + py + 1, color);
			e = e->lpNext;
		}
	}

	if (tool_ViewAllCharacter)
	{
		int r, g, b;
		Getcolor(&r, &g, &b);

		for (int a = 0; a < 8; a++)
		{
			for (int tb = 0; tb < 8; tb++)
			{
				lpMAPSKILLTABLE	h = Header[a][tb];
				while (h != NULL)
				{
					//if(h->skillno==step)
					{
						switch (h->skillno)
						{
						case TOOL_FARMING:	if (tool_SkillTool_FARMING == false) break;

							px = h->x * TILE_SIZE * RADARXL / mxl;
							py = h->y * TILE_SIZE * RADARYL / myl;
							Hcolor(255, 255, 0);
							Hprint2(x + px, y + py, g_DestBackBuf, "%d-%d", h->subType, h->tile_Range, h->probability);
							break;
						case TOOL_MINING:	if (tool_SkillTool_MINING == false) break;
							px = h->x * TILE_SIZE * RADARXL / mxl;
							py = h->y * TILE_SIZE * RADARYL / myl;
							Hcolor(255, 255, 0);
							Hprint2(x + px, y + py, g_DestBackBuf, "%d-%d", h->subType, h->tile_Range, h->probability);
							break;
						case TOOL_FISHING:	if (tool_SkillTool_FISHING == false) break;
							px = h->x * TILE_SIZE * RADARXL / mxl;
							py = h->y * TILE_SIZE * RADARYL / myl;
							Hcolor(255, 255, 0);
							Hprint2(x + px, y + py, g_DestBackBuf, "%d-%d", h->subType, h->tile_Range, h->probability);
							break;
						case TOOL_CHOPPING:	if (tool_SkillTool_CHOPPING == false) break;
							px = h->x * TILE_SIZE * RADARXL / mxl;
							py = h->y * TILE_SIZE * RADARYL / myl;
							Hcolor(255, 255, 0);
							Hprint2(x + px, y + py, g_DestBackBuf, "%d-%d", h->subType, h->tile_Range, h->probability);
							break;
						case TOOL_HUB:	if (tool_SkillTool_HUB == false) break;
							px = h->x * TILE_SIZE * RADARXL / mxl;
							py = h->y * TILE_SIZE * RADARYL / myl;
							Hcolor(255, 255, 0);
							Hprint2(x + px, y + py, g_DestBackBuf, "%d-%d", h->subType, h->probability);
							break;

						case TOOL_NPC_GENER:	if (tool_SkillTool_NPC == false) break;
							px = h->x * TILE_SIZE * RADARXL / mxl;
							py = h->y * TILE_SIZE * RADARYL / myl;

							color = RGB16(0, 0, 255);
							PutDot(x + px, y + py, color);
							PutDot(x + px + 1, y + py, color);
							PutDot(x + px + 2, y + py, color);
							PutDot(x + px + 3, y + py, color);


							Getcolor(&r, &g, &b);

							if (h->type_Num == 98 || h->type_Num == 99)
							{
								Hcolor(255, 255, 0);
								Hprint2(x + px, y + py, g_DestBackBuf, lan->OutputMessage(9, 10), h->probability);//010215 lsw
								Hcolor(255, 0, 0);
								Hprint2(x + px + 1, y + py + 1, g_DestBackBuf, lan->OutputMessage(9, 10), h->probability);//010215 lsw
							}
							else
							{	//< CSD-030419
								Hcolor(255, 255, 0);
								Hprint2(x + px, y + py, g_DestBackBuf, "%d.%s", h->type_Num, GetNpcName(h->type_Num));
							}	//> CSD-030419

							break;
						}
					}
					h = h->next;
				}
			}
		}

		DrawObjectItem_SealStone();

		Hcolor(r, g, b);
	}
}



int	 DestPointX, DestPointY;
int	 DestPointAni;
char DestPointOutFlag;

void SettingDestPoint(int x, int y)
{
	DestPointX = x, DestPointY = y;
	DestPointAni = 22;
	DestPointOutFlag = 0;
	CursorNo(3);
}


void UnDisplayDestPoing(LPCHARACTER ch)
{
	if (ch == Hero)  DestPointOutFlag = 1;
}


void PutDestPoint(void)
{

	int  sp;

	if (DestPointAni == 0) return;

	switch (DestPointAni)
	{
	case 22: sp = 957; break;
	case 21: sp = 957; break;
	case 20: sp = 957; break;
	case 19: sp = 957; break;
	case 18: sp = 957; break;
	case 17: sp = 957; break;
	case 16: sp = 958; break;
	case 15: sp = 959; break;
	case 14: sp = 960; break;
	case 13: sp = 961; break;
		//	case 12 : sp = 962; break;
		//	case 11 : sp = 963; break;
	default: sp = 962; break;
	}

	PutCompressedImage(DestPointX - Mapx, DestPointY - Mapy, &spr[sp]);

	if (DestPointOutFlag == 0 && DestPointAni > 11)	DestPointAni--;
	if (DestPointOutFlag == 1) DestPointAni--;
}



extern void GetRealTime(int& mon, int& day, int& hour);
// 010724 KHS	
static 	char nMapBGM[MAX_MAP_] = { 3,6,2,5,0, 4,6,2,2,5,
									5,4,4,0,0, 0,0,0,3,3,
									4,2,0,0,6, 6,4,0,2,2,
									3,3,3,3,0, 3,3,4,4,0,
									4,4,0,0,0, 5,0,0,0,0,
									0,0,0,0,0, 0,0,0,0,4,
									5,3,3,0,3, 0,0,0,0, };

//021030 YGI
int	ReturnBGMNumber(int map_Number)
{
	if (map_Number < 0) return 1;
	int sound = g_Menu_Variable.GetEventSound();
	if (sound > 0) return sound;
	return nMapBGM[map_Number];
}	// 010724 KHS	
//021030 YGI
void BackgroundMusic()
{
	int return_BGM = 0;
	return_BGM = ReturnBGMNumber(MapNumber);
	if (return_BGM)
	{
		PlayListAutoSounds(return_BGM, 0, 0, 1);
	}
}

void SendIsThereCharName(char* name)
{
	t_packet p;
	p.h.header.type = CMD_ISTHERE_CHARNAME;
	strcpy(p.u.client_isthere_charname.name, name);
	p.h.header.size = sizeof(t_client_isthere_charname) - NM_LENGTH + strlen(name);
	QueuePacket(&p, 1);
}


//   Create Character 할때,  이름이 존재하는지 알아본다. 

//   name이 DB에 존재하면 1을 리턴한다. 
//   name이 DB에 존재하지 않으면 0을 리턴한다. 
bool CheckCharName(char* name)
{
	if (NULL == name)
	{
		return false;
	}

#ifdef 	USA_LOCALIZING_//
#ifndef _DEBUG
	if (!::IsASCII(name))
	{
		return false;
	}
#endif
#endif

	SendIsThereCharName(name);

	DWORD StartTime = ::timeGetTime();
	ListenCommand = CMD_NONE;
	while (1)
	{
		DWORD Duration = ::timeGetTime() - StartTime;	// 5초를 기다린다. 
		if (Duration > WAIT_TIME_RESPONSE_DURATION)
		{
			return false;
		}

		if (HandleRunning(&connections) <= FALSE)
		{
			Release(&connections);
			return false;
		}
		else if (ListenCommand == CMD_THEREIS_NO_CHARNAME)
		{
			return false;
		}
		else if (ListenCommand == CMD_THEREIS_CHARNAME)
		{
			return true;
		}
	}
}

void SendDebugReqCharInfo(int id)
{
	t_packet p;

	p.h.header.type = CMD_DEBUG_REQ_CHAR_INFO;
	p.u.debug_client_req_char_info.id = id;
	p.h.header.size = sizeof(t_debug_client_req_char_info);

	QueuePacket(&p, 1);
}

void RecvDebugReqCharInfo(t_debug_server_req_char_info* p)
{
	LPCHARACTER ch = ReturnCharacterPoint(p->id);
	if (ch == NULL) return;

	ch->_Hp = p->Hp;
	ch->_HpMax = p->HpMax;
	ch->event_no = p->event_no;	// 0131 YGI
	ch->_lev = p->lv;
	ch->viewdebugcharinfo = 75;
}

void _DisplayDebugCharInfo(void)// 0624 YGI//020705 lsw
{
	{
		LPCHARACTER ch = Hero;
		ch = Hero;
		int c = 0;

		if (SysInfo.notconectserver == 0)
			while (ch != NULL)
			{
				if (ch->viewdebugcharinfo >= 1)
					if (g_GameInfo.lpcharacter == ch)
					{
						ch->viewdebugcharinfo--;
					}
				if (ch->viewdebugcharinfo == 0)
				{
					ch->viewdebugcharinfo = -1;
					SendReqCharInfo(ch->id);
					SendReqSkillInfo(ch);				//1004 YGI
				}
				ch = ch->lpNext;
			}
	}

#ifdef _DEBUG

	LPCHARACTER ch = Hero;		// 0624 YGI		
	ch = Hero;
	int c = 0;

	if (GetSysInfo(SI_GAME_MAKE_MODE))
		if (SysInfo.notconectserver == 0)
			while (ch != NULL)
			{
				if (ch->viewdebugcharinfo >= 1)
					if (g_GameInfo.lpcharacter == ch)
					{
						HprintBold(10, 150 + 13 * c, 0xffff, 0, "event_no %d ", ch->event_no);		// 0131 YGI
						c++; HprintBold(10, 150 + 13 * c, 0xffff, 0, "Hp      %d ", ch->_Hp);
						c++; HprintBold(10, 150 + 13 * c, 0xffff, 0, "HpMax   %d ", ch->_HpMax);
						c++; HprintBold(10, 150 + 13 * c, 0xffff, 0, "Level   %d ", ch->_lev);
						ch->viewdebugcharinfo--;
					}
				ch = ch->lpNext;
			}

#endif
}

void RecvPleaseGiveLife(int id, int hp, int mana, int hungryp, int who)
{
	LPCHARACTER ch = ReturnCharacterPoint(id);
	if (ch == NULL) return;

	// Healing효과......

//	if( ch->type != SPRITETYPE_MONSTER ) 
//		if( ch->viewtype != VIEWTYPE_GHOST_  && ch->hp != 0 ) return;

	if (ch == Hero)
	{
		SCharacterData.nCharacterHP = hp;
		SCharacterData.nCharacterMP = mana;
		SCharacterData.nCharacterSP = hungryp;
		SCharacterData.condition = CON_NORMAL;
		// 부활 때 전투스킬 초기화
		iCSPCMax = iCSPCNow = 0; // 030226 
		CSDMainIconClear(); // 030428 kyo

		Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
	}
	//< CSD-020826
	ch->hp = hp;
	ch->condition = CON_NORMAL;
	ch->blood_Set = false;
	ch->viewtype = VIEWTYPE_NORMAL_;
	ch->dieani = 0;
	ch->peacests = 0;
	ch->dummy_pk = 0;
	ch->gox = ch->x;
	ch->goy = ch->y;

	//> CSD-020826
	EndCastMagic(ch);
	SettingBasicActionSub(ch);



	if (ch == Hero)
	{
		PlayListAutoSounds(2408, 0, 0, 0);	// LTS SOUND

		//<soto-030627
		g_mgrBattle.ClearDelay(); // CSD-TW-030624
		g_mgrBattle.ClearSwitchDelay(); // CSD-TW-030624
		//>soto-030627

	}
	else
	{
		int distx = ch->x - Hero->x;
		int disty = ch->y - Hero->y;
		PlayListAutoSounds(2408, distx, disty, 0);
	}

	InsertMagic(ch, ch, 160, 0, 0, 0, ch->x, ch->y);
	if (ch->sealstone)
	{
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);
		InsertMagic(ch, ch, 160, 0, 0, 0, ch->x + (rand() % 100) - 50, ch->y + (rand() % 100) - 50);

		return;
	}

	if (ch->changeActionFlag == 1)		ch->changeActionFlag = 0;
	CharDoAni(ch, ch->direction, ch->basicAction);

	LPCHARACTER whoch = ReturnCharacterPoint(who);
	if (whoch == NULL) return;
	if (ch == Hero)
	{
		AddCurrentStatusMessage(250, 200, 200, lan->OutputMessage(9, 21), whoch->name);//010215 lsw
	}
	else if (whoch != Hero)
	{

		AddCurrentStatusMessage(0, 200, 200, lan->OutputMessage(9, 22), whoch->name, ch->name);//010215 lsw
	}
}






/////////////////////////// 0206 lkh 전부 복사 ///////////////////////////////////
bool	ReturnCheckObject(LPMAPOBJECT		mo)
{
	static int			buttonOnCount;
	static int			selectObject = 0;				//마우스에 의해 체크된 오브젝트 번호

	static int			selectObject_X;					//선택된 오브젝트의 X축
	static int			selectObject_Y;					//선택된 오브젝트의 Y축

	static int			tempObject = 0;				//이전에 선택된 오브젝트 기억
	static int			objectType = 0;				//체크된 오브젝트의 종류(5:간판/6:이정표)
	static int			displayBoard = 0;			//현재 보여지고 있는 간판번호
	static int			displayDirect = 0;			//현재 보여지고 있는 이정표번호	

	char				str_Data[80];

	DIRECTBOARDDATA		directboardData;
	int					onCount = 15;				//1초

	if (g_MouseInMenu)  return 0;

	//WORD  objectoritem;				//object Type(0~65535)

	//////////////////////////////// 0420 lkh 추가 (메뉴가 열려 마우스가 메뉴위에 있는 경우는 체크하지 않음) 
	Spr* sp = &MoImage[mo->id - 1].spr[0];
	if (BoxAndDotCrash((int)(mo->x + mo->offx - sp->ox), (int)(mo->y + mo->offy - sp->oy), sp->xl, sp->yl, Mox, Moy))
	{
		selectObject = mo->dum;
		selectObject_X = mo->x;
		selectObject_Y = mo->y;
		objectType = mo->objectoritem;

		if (!IsDead(Hero))
			if (g_nLButtonState == STATE_BUTTON_PRESSED)
			{
				switch (objectType)
				{
				case ITEMTYPE_SEAL_NOR:
					break;
				case ITEMTYPE_SEAL_SPC:
					break;
				case ITEMTYPE_HP_UP:
					break;
				case ITEMTYPE_MP_UP:
					break;
				case ITEMTYPE_NATION_STONE:	if (MapInfo[MapNumber].nation == Hero->name_status.nation)		// 1004 YGI---
				{
					MP3(SN_OPEN_NATION_BBS, 1);				// 1027 YGI
					CallSmallMenu(MN_NATION_BBS);
				}
										  break;
				case ITEMTYPE_GUILD_STONE:
					break;
				case ITEMTYPE_SENARIO_STONE:	CallSmallMenu(MN_SCENARIO); break;
				case ITEMTYPE_SCALE:	CallSmallMenu(MN_SCALE); break;

				default:
					if ((mo->id >= 336 && mo->id <= 337) || (mo->id >= 2482 && mo->id <= 2485))
					{
						CallSmallMenu(MN_HUNTER_MAIN);
					}
					break;
				}
			}


		if ((mo->objectoritem == ITEMTYPE_SIGNBOARD || mo->objectoritem == ITEMTYPE_DIRECTBOARD) && !g_MouseInMenu)
		{
			buttonOnCount++;
			if (buttonOnCount >= onCount + 10)	buttonOnCount = onCount + 10;
			if (buttonOnCount == onCount || (buttonOnCount >= onCount && selectObject != tempObject))
			{
				if (objectType == ITEMTYPE_SIGNBOARD)
				{
					g_BoadAndSignNumber = mo->dum; // CSD-030324 // thai2 YGI

					if (GetBoardData(mo->dum, str_Data) == true)
					{
						displayBoard = mo->dum;
						PutBoardMenu(mo->dum, str_Data);
						tempObject = selectObject;
						SMenu[MN_DISTANCE_TABLE].bActive = false;
						displayDirect = 0;
					}
					return true;
				}
				else if (objectType == ITEMTYPE_DIRECTBOARD)
				{
					g_BoadAndSignNumber = mo->dum + 10000; // CSD-030324 // thai2 YGI

					if (GetDirectBoardData(mo->dum, &directboardData) == true)
					{
						displayDirect = mo->dum;
						PutDirectBoardMenu(&directboardData);
						tempObject = selectObject;
						SMenu[MN_STORE_SIGN].bActive = false;
						displayBoard = 0;
					}
					return true;
				}

			}
		}
		else
		{
			if ((selectObject == mo->dum && objectType == mo->objectoritem && (selectObject_X == mo->x && selectObject_Y == mo->y)) || (selectObject == 0))
			{
				buttonOnCount--;
				if (buttonOnCount < 0)		buttonOnCount = 0;
				selectObject = 0;
				selectObject_X = 0;
				selectObject_Y = 0;
				tempObject = 0;
				objectType = 0;

				if (buttonOnCount == onCount)
				{
					if (displayBoard != 0)
					{
						SMenu[MN_STORE_SIGN].bActive = false;
						displayBoard = 0;
					}
					if (displayDirect != 0)
					{
						SMenu[MN_DISTANCE_TABLE].bActive = false;
						displayDirect = 0;
					}
				}
			}
		}
	}

	/*
	else		//다른 타입의 오브젝트를 선택했을 때
	{
		Spr *sp = &MoImage[ mo->id-1].spr[ mo->curframe ];
		if( BoxAndDotCrash( (int)(mo->x+mo->offx-sp->ox), (int)(mo->y+mo->offy-sp->oy), sp->xl, sp->yl, Mox, Moy) )
		{
			buttonOnCount--;
			selectObject=0;
			objectType=0;

			if(buttonOnCount<0)		buttonOnCount=0;
			if(buttonOnCount==onCount)
			{
				if(displayBoard != 0)
				{
					SMenu[MN_STORE_SIGN].bActive = false;
					displayBoard = 0;
				}
				if(displayDirect !=0 )
				{
					SMenu[MN_DISTANCE_TABLE].bActive = false;
					displayDirect = 0;
				}
			}
		}
	}
	*/
	return false;
}

// thai2 YGI
extern bool CheckContinueLine(char check);
bool	GetBoardData(int board_Number, char* str_Data)
{	//< CSD-030324
	char* token;

	char		str_boardData[100];	//한글 50자/영문 100자를 벗어나지 못함
	BOARDDATA	boardData;
	FILE* fp;
	char		DataPath[MAX_PATH];

	sprintf(DataPath, "./map/%s.bsd", MapName);

	fp = Fopen(DataPath, "rt");			//read only+text file
	if (fp == NULL)	return false;

	while (!feof(fp))
	{
		fgets(str_boardData, 99, fp);
		if (CheckContinueLine(str_boardData[0])) continue;

		token = strtok(str_boardData, "\t\n");
		if (!token) continue;

		boardData.boardNumber = atoi(token);
		token = strtok(NULL, "\t\n");
		if (token)  strcpy(boardData.str_boardData, token);

		if (boardData.boardNumber == board_Number)
		{
			strcpy(str_Data, boardData.str_boardData);
			fclose(fp);
			return true;
		}
		//return	boardData.str_boardData;
	}
	fclose(fp);
	return	false;
}	//> CSD-030324

// thai2 YGI
bool	GetDirectBoardData(int board_Number, DIRECTBOARDDATA* directboardData)
{	//< CSD-030324
	char* token;

	char		str_boardData[160];	//한글 50자/영문 100자를 벗어나지 못함
	//DIRECTBOARDDATA		directboardData;
	FILE* fp;
	char		DataPath[MAX_PATH];

	sprintf(DataPath, "./map/%s.dsd", MapName);

	fp = Fopen(DataPath, "rt");			//read only+text file
	if (fp == NULL)	return false;

	while (!feof(fp))
	{
		fgets(str_boardData, 159, fp);

		if (CheckContinueLine(str_boardData[0])) continue;
		token = strtok(str_boardData, "\t\n");
		if (!token) continue;

		directboardData->boardNumber = atoi(token);

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_Title, token);
		if (!strncmp(directboardData->str_Title, ".", 1))	memset(directboardData->str_Title, 0, sizeof(char[30]));

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_East, token);
		if (!strncmp(directboardData->str_East, ".", 1))		memset(directboardData->str_East, 0, sizeof(char[30]));

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_South, token);
		if (!strncmp(directboardData->str_South, ".", 1))	memset(directboardData->str_South, 0, sizeof(char[30]));

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_West, token);
		if (!strncmp(directboardData->str_West, ".", 1))		memset(directboardData->str_West, 0, sizeof(char[30]));

		token = strtok(NULL, "\t\n");
		strcpy(directboardData->str_North, token);
		if (!strncmp(directboardData->str_North, ".", 1))	memset(directboardData->str_North, 0, sizeof(char[30]));

		if (directboardData->boardNumber == board_Number)
		{
			fclose(fp);
			return true;
		}
	}

	fclose(fp);
	return	false;
}	//> CSD-030324

////////////////////////////////// 여기까지 ////////////////////////////////////////

extern bool isInWarMap(); //Eleval 15/08/09

bool IsHidenGhost(LPCHARACTER ch)		// 볼수 없는 고스트 인가??//운영자 못건드리게 
{
	if (Hero->IsCounselor() || Hero->IsReporter()) return false;
	if ((ch->IsCounselor() || ch->IsReporter()) && ch->viewtype == VIEWTYPE_GHOST_) return true;

	if (g_LocalWarBegin && isInWarMap()) //Eleval 15/08/09 - So players from X Nation can't see ghosts from Y Nation
	{
		if (Hero->JoinLocalWar && ch->JoinLocalWar)
		{
			if (Hero->name_status.nation == ch->name_status.nation)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

