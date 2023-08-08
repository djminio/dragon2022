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

#include "Stdafx.h"
#include <windows.h>

#include "Dragon.h"
#include "Hong_Sub.h"
#include "GameProc.h"
#include "Menu.h"
//#include "MenuDef.h"
#include "MenuNetWork.h"
#include "Char.h"
#include "Map.h"
#include "Counseler.h"		// 001129 KHS
#include "Music.h"
#include "LineCommand.h"
#include "smallmenu.h"
#include "menuset.h"
#include "SmallMenuSet.h"//020620 lsw
#include "chat.h"
#include "MailMgr.h"
#include "Skill.h"//021030 lsw
#include "Auction.h"//021030 lsw
#include "cScreenEffect.h" // CSD-030306
#include "ArenaManager.h"
#include "Hong_sprite.h" //Eleval
#include "Prison.h"
extern CPrison g_cPrison;
extern CHARACTERLIST g_CharacterList;
extern void SendSquadChatMessage(int type,char* Message);			
extern void WarMenuOpen();											
extern void WarMenuNoticeOpen();

extern void ShowCommanderInfo(); 				// 010924 LTS
extern void ShowSquadInfo();
extern void WarGiveLife();						// 011015 LTS
extern void WarControl(char* Arg);				// 011020 LTS
extern void WarLoopTime(char* Arg);				// 011025 LTS
extern void WarGiveLife(char* Arg);				// 011213 LTS
extern void WarGiveLife2(char* Arg);			
extern int	IsDead( LPCHARACTER ch );
extern void SendLocalWarChat(char* Arg);		// LTS NEW LOCALWAR	
extern void SendNationChat(char* Arg);
 extern void BannedUser(const char* szCharName);
extern void  SendWorldBBSChat(char* Arg);
extern void SendAction(int i,int Direction);	// LTS ACTION
extern void SendCMD_SET_QUESTMODE(char* Arg);	// LTS DRAGON MODIFY
extern void SendCheckDualFame();              // LTS DUAL FAME
extern bool g_bStopWeatherSystem; // CSD-CN-031215
extern bool g_bOffWeatherSystem; // CSD-CN-031217

extern bool g_allowDrop; //Eleval for /drop command 15/05/09
extern bool g_autoAttack; //Eleval 22/05/09 - for /auto attack command
extern bool g_autoDefence; //Eleval 23/05/09 - for /auto defence command

LINECOMMAND LC[MAX_LINE_COMMAND_] = 
{	//< CSD-030311 : 영어는 대문자로 써주세요..
	{"", LC_EAT_ITEM},					// 먹기										
	{"", LC_CHAT_WHISPER},				// @										
	{"", LC_CHAT_WHISPER},				// TO
	{"", LC_CHAT_PARTY},				// 파티에게
	{"", LC_CHAT_PARTY},				// 동료											
	{"", LC_CHAT_GUILD},				// 길드에게								
	{"", LC_TOTAL_MAP_CONNECTIONS},		// 맵현재인원								
	{"", LC_TOTAL_CONNECTIONS},			// 현재인원											
	{"", LC_TOTAL_CONNECTIONS},			// 누구											
	{"", LC_TOTAL_CONNECTIONS},			// USER												
	{"", LC_MAKE_PARTY},				// PARTY											
	{"", LC_MAKE_PARTY},				// 파티													
	{"", LC_DEFAULT_WALK},				// 걷기													
	{"", LC_DEFAULT_WALK},				// 기본걷기													
	{"", LC_DEFAULT_RUN},				// 달리기												
	{"", LC_DEFAULT_RUN},				// 기본달리기											
	{"", LC_TAME_STOP},					// 쉬어												
	{"", LC_TAME_STOP},					// 그만																
	{"", LC_TAME_STOP},					// STOP														
	{"", LC_TAME_ATTACK},				// 공격해													
	{"", LC_TAME_ATTACK},				// ATTACK												
	{"", LC_TAME_FOLLOWME},				// 따라와											
	{"", LC_TAME_FOLLOWME},				// FOLLOW												
	{"", LC_TAME_NAME},					// 네이름은												
	{"", LC_TAME_NAME},					// NAME											
	{"", LC_OPEN_TIMEMENU},				// 시간보기								
	{"", LC_OPEN_TIMEMENU},				// TIME 								
	{"", LC_CLOSE_TIMEMENU},			// 시간끄기									
	{"", LC_CLOSE_TIMEMENU},			// 시간닫기											
	{"", LC_OPEN_TIMEMENU},				// 시간										
	{"", LC_CLOSE_ALL},					// OFF													
	{"", LC_EAR_MESSAGE_REFUSE_WHO},	// 거부상대												
	{"", LC_EAR_MESSAGE_REFUSE},		// 귓속말거부									
	{"", LC_EAR_MESSAGE_OK},			// 귓속말허락									
	{"", LC_REPRINT_EAR_MESSAGE},		// 쪽지				
	{"", LC_GREETING},					// 인사말 								
	{"", LC_EXPLAIN_CLOSE},				// 설명닫기																
	{"", LC_EXPLAIN_CLOSE},				// 설명끄기									
	{"", LC_EXPLAIN_OPEN},				// 설명보기					
	{"", LC_REQ_MAN_TO_MAN},			// 결투						
	{"", LC_RESET_MAGIC_ARRAY},			// 신의권능 						
	{"", LC_RESET_MAGIC_ARRAY},			// 마법재배치										
	{"", LC_MSG_ALL_NATION},			// 모두							
	{"", LC_GUILD_EXIT},				// 길드탈퇴					
	{"", LC_GUILD_JOIN},				// 길드가입						
	{"", LC_GUILD_ALL_MSG},				// 길드모두					
	{"", LC_GUILD_ALL_MSG},				// 길드공지					
	{"", LC_GUILD_CHANGE_EACH_LEVEL},	// 길드등급							
	{"", LC_GUILD_CMD_INFO},			// 길드명령
	{"", LC_CHANGE_JOB},				// 직업바꾸기
	{"", LC_CHANGE_JOB},				// 직업교체						
	{"", LC_STOP_WAR},					// 휴전
	{"", LC_SCAN_TARGET_VYSEUS},		// 야사스의 눈이여
	{"", LC_SCAN_TARGET_ZYPERN},		// 닐림의 의지여
	{"", LC_KILL_CHARACTER},			// 휴식
	{"", LC_DIRECTIONS},				// 방향
	{"", LC_OPEN_DISPLAY_SKILL_EXP},	// 스킬보기
	{"", LC_CLOSE_DISPLAY_SKILL_EXP},	// 스킬닫기
	{"", LC_OPEN_DISPLAY_TAC_EXP},		// 택틱보기
	{"", LC_CLOSE_DISPLAY_TAC_EXP},		// 택틱닫기
	{"", LC_LIGHTOFRESCUE},				// 구원의빛
	{"", LC_AUTOLEVELUP_CLOSE},			// 자동해제
	{"", LC_AUTOLEVELUP_OPEN},			// 자동분배							
	{"", LC_ANNONCE_ON},			    // 도움말듣기
	{"", LC_ANNONCE_OFF},			    // 도움말끄기
	{"", LC_EAR_COMMAND_INFO},		    // 귓속말
	{"", LC_FIGHTMAP_ALL_CHAT},		    // 전체
	{"", LC_SMALL_TIPS},			    // 도움
	{"", LC_SMALL_TIPS},			    // HELP
	{"", LC_SMALL_TIPS},			    // ?
	{"", LC_EMOTION},				    // 감정
	{"", LC_EXIT_GUILD},
	{"", LC_VIEW_NK},				    // NK 수치 보기
	{"", LC_SQUAD_LEADER},			    // 사령관 -> 부대장
	{"", LC_SQUAD},					    // 사령관 -> 부대
	{"", LC_SQUAD_MEMBER},              // 부대장 -> 부대원
	{"", LC_SQUAD_MEMBER_TO_MEMBER},    // 부대원 -> 부대원	
	{"", LC_WAR_MENU_OPEN},		        // War Menu Open
	{"", LC_WAR_MENU_NOTICE_OPEN},	    // War Menu NOTICE
	{"", LC_GUILD_MAIL},			    // 길드메일
	{"", LC_SQUAD_CHAT},			    // 010915 LTS	//임시
	{"", LC_GUILD_CHAT},			    // #
	{"", LC_COMMANDER_INFO},		    // 010924 LTS
	{"", LC_SQUAD_INFO},			    // 010924 LTS	
	{"", LC_GUILD_CHAT_ON},		        // 길드채팅켜기
	{"", LC_GUILD_CHAT_OFF},		    // 길드채팅끄기
	{"", LC_RESET_ABILITY},		        // 어빌재분배
	{"", LC_WAR_GIVE_LIFE},		        // 011015 LTS	// 전쟁터 유령 살리기 // 부활
	{"", LC_PARTY_ALL_FREE},		    // 파티해제
	{"", LC_SEND_GUILD_MAIL},	        // 길드메일보내기
	{"", LC_SEND_MAIL},		 	        // 메일보내기
	{"", LC_SHOW_ME_DUAL},		  	    // 어둠을 걷고 시간을 달린다
	{"", LC_REPORTER_MODE},			    // 기자모드
	{"", LC_FRIEND_MENU},			    // 친구
	{"", LC_WAR_GIVE_LIFE2},		    // 부활장소				// 011213 LTS	N_YILSE
	{"", LC_SCAN_TARGET_YILSE},		    // 011217 LTS
	{"", LC_GETCOLOSSUS_INFO},		    // 콜로서스, 
	{"", LC_GETCOLOSSUS_INFO},		    // colossus, 
	{"", LC_INVITE_COLOSSUS},		    // 초대 
	{"", LC_JOIN_COLOSSUS},			    // 참가
	{"", LC_PK_ON_OFF},				    // PK
	{"", LC_COMBAT_SKILL_ON_OFF},	    // PK//020420 lsw
	{"", LC_LOCALWAR_CHAT},			    // 국지전	%		// LTS NEW LOCALWAR
	{"", LC_QUESTMODE},				    // LTS DRAGON MODIFY
	{"", LC_WEATHER_ON}, // CSD-CN-031215 : 날씨효과켜기
	{"", LC_WEATHER_OFF}, // CSD-CN-031215 : 날씨효과끄기
	{"", LC_COMBAT_RESET},			    // CSD-020611
	{"", LC_MAGIC_DISPLAY_ON},          // CSD-020620
	{"", LC_MAGIC_DISPLAY_OFF},         // CSD-020620
	{"", LC_SET_NORMAL_CHAT_MODE},      // CSD-020620
	{"", LC_REPRINT_NORMAL_CHAT},	    // 보통쪽지
	{"", LC_REPRINT_GUILD_CHAT},	    // 길드쪽지
	{"", LC_REPRINT_GUILD_BBS},	        // 길드공지쪽지
	{"", LC_TRADE},			            // 020808 YGI 
	{"", LC_CHECK_DUAL_FAME},		    // 듀얼명성     // LTS DUAL FAME
	{"", LC_WHAT_QUEST_IN_MAP},	        // 021007	//맵에 어떤 퀘스트가 있는가?
	{"", LC_MERCHANT_BBS},		        // 021022 상인과 직접 거래
	{"", LC_MERCHANT_EXCHANGE},		    // 021022 상인과 거래
	{"", LC_EVENT_MOVE_MAP},		    // 021107 YGI
	//kyo <<
	{"", LC_EMOTI_LOVE},
	{"", LC_EMOTI_SMILE},
	{"", LC_EMOTI_HAPPY},
	{"", LC_EMOTI_SHY},
	{"", LC_EMOTI_CRY},
	{"", LC_EMOTI_GOOD},
	{"", LC_EMOTI_SORRY},
	{"", LC_EMOTI_ANGER},
	{"", LC_EMOTI_THANKS},
	{"", LC_EMOTI_TELL},
	{"", LC_GAME_SCISSOR},
	{"", LC_GAME_ROCK},
	{"", LC_GAME_PAPER},
	{"", LC_GAME_GO},
	{"", LC_GAME_HELP},
	{"", LC_GAME_PARTY},
	{"", LC_GAME_HEAL},
	{"", LC_GAME_TRANSACTION},
	{"", LC_GAME_BEG},
	{"", LC_GAME_CONGRAGULATION},

	{"", LC_BBS_LIMITED_USER_ADD},
	{"", LC_BBS_ON},
	{"", LC_BBS_OFF},
	{"", LC_RAID_START}, // CSD-040310
	{"", LC_RAID_END}, // CSD-040310
	{"", LC_GET_SMP},
	{"", LC_REPRINT_PARTY_CHAT}, // Finito for /party note command
	{"", LC_DENY_DUEL_REQ}, // Finito 25/08/07 command to deny dual requests
	{"", LC_ALLOW_DUEL_REQ}, // Finito 25/08/07 command to allow dual requests
	{"", LC_TOGGLE_DROP},	// Eleval 15/05/09 command to allow / block drop
	{"", LC_TAME_AUTO_ATTACK}, // Eleval 22/05/09 command to enable / disable auto-attack for tamed monsters
	{"", LC_TAME_AUTO_DEFENCE}, // Eleval 23/05/09 command to enable / disable auto-defence for tamed monsters
	{"", LC_TAME_ALL_FOLLOW}, // Eleval 24/05/09 command to tell all tamed creatures to follow their master (/tame follow)
	{"", LC_TAME_ALL_STOP}, // Eleval 24/05/09 command to tell all tamed creatures to stop what they are doing (/tame stop)
	{"", LC_REPORTER_TELEPORT}, //Eleval 15/08/09 - Command for RCs to teleport themselves anywhere (/teleport map x y)
	{"", LC_HIDE}, //Eleval 18/08/09 - So that people can hide some stuff, like hats
	{"", LC_HONOR}, //Eleval 06/09/09 - To show the Player's Honor

	{"", LC_EXP_RESET},
	{"", LC_NATION_CHAT},
	{ "", LC_WORLDBBS_CHAT},
	{ "", LC_USER_IMPRISON },		// LTH-040812-KO 감옥에 수감!!
	{ "", LC_USER_RELEASE },		// LTH-040812-KO 감옥에 출옥!!
	{ "", LC_USER_BAN },
	//>>kyo
	{"", 0}				
};  //> CSD-030311

char	Lc_ChatType;
char	Lc_TempChatName[ 31];
char	Lc_DefaultMoveType;

int		Lc_AutoMovementCmd;


char	GreetingStr[ FILENAME_MAX];

/////////////////////////////////////////////////////////////////////////////////////////
//
//		User Functions Declaration..
//
//
void SendTotalMapConnections( void );
void SendTotalConnections( void );
void RecvTotalMapConnections( int no );
void RecvTotalConnections( int no );

void LC_SendInputParty( char *name );

void SendTameCommand( int command, char *s );
void RecvHostEnd(  tame_end *p );
void SendTeleportCommand(char* s); //Eleval 15/08/09 - To manage RCs Teleport
void HideSomething(char *s);	//Eleval 18/08/09 - To hide something




///////////////////////////////////////////////////////////////////////////////////////////
//
//		User Functions..
//
//

int LineCommandSortFunction( const void *first, const void *second )
{
	LINECOMMAND *lc1 = (LINECOMMAND *)first;
	LINECOMMAND *lc2 = (LINECOMMAND *)second;
	return ( strlen( lc2->reserved ) - strlen( lc1->reserved ) );
}

int IsLineCommand( char *message, int nsize )
{	
	//if( Hero->viewtype == VIEWTYPE_GHOST_ )// 유령일 경우
	//	return ;		// 명령어를 못 하게 한다.
	int c; 
	LPLINECOMMAND lc;
	LPCHARACTER ch = Hero;
	char *t, *t1;
	char tempmessage[MAX_CHAT_STRING];
	
	if( *message != '/' ) return 0;
	
	message ++;
	strcpy( tempmessage, message );
	//	CharUpper( message );//020926 lsw
	static int first = 1;
	if( first )
	{
		int i;
		for( i=0; i<MAX_LINE_COMMAND_; i++ )
		{
			if( !LC[i].reserved[0] ) break;
		}
		qsort( LC, i, sizeof( LINECOMMAND ), LineCommandSortFunction );
		first = 0;
	}
	
	c = 0;
	lc = &LC[ c];
	while( lc->reserved[0] )
	{		
		if( 0 == strnicmp( lc->reserved, message, strlen( lc->reserved )) )//020926 lsw
		{
			t = tempmessage + strlen(lc->reserved );
			t1 = EatFrontWhiteChar( t );
			
			switch (lc->command)
			{	
			case LC_GO_STOP:
				{
					Lc_AutoMovementCmd = 0;
					break;
				}
			case LC_GO_WEST:
			case LC_GO_NORTH:
			case LC_GO_SOUTH:
			case LC_GO_EAST:		
			case LC_GO_RANDOM:
				{
					Lc_AutoMovementCmd = lc->command;
				
					switch( Lc_AutoMovementCmd )
					{	
					case LC_GO_EAST:
						{
							SendMessage(g_hwndMain, WM_LBUTTONDOWN, 0, MAKELONG(Hero->x - Mapx + 100, Hero->y - Mapy));
							break;
						}
					case LC_GO_WEST:
						{
							SendMessage(g_hwndMain, WM_LBUTTONDOWN, 0, MAKELONG(Hero->x - Mapx - 100, Hero->y - Mapy));
							break;
						}
					case LC_GO_NORTH:
						{
							SendMessage(g_hwndMain, WM_LBUTTONDOWN, 0, MAKELONG(Hero->x - Mapx, Hero->y - Mapy  - 100));
							break;
						}
					case LC_GO_SOUTH:
						{
							SendMessage( g_hwndMain, WM_LBUTTONDOWN, 0, MAKELONG( Hero->x - Mapx, Hero->y - Mapy  + 100) );
							break;
						}
					}

					break;
				}
			case LC_EAT_ITEM:
			case LC_NOT_GO:
				{
					break;
				}
			case LC_CHAT_WHISPER:
				{
					SendWisperByLineCommand(t);
					break;
				}
			case LC_CHAT_PARTY:
				{
					SendPartyChatByLineCommand(t);//020701 lsw
					break;
				}
			case LC_CHAT_GUILD:
				{
					SendChatSet(CHATTYPE_GUILD, NULL);
					break;
				}
			case LC_TOTAL_MAP_CONNECTIONS:
				{
					SendTotalMapConnections();
					break;
				}
			case LC_TOTAL_CONNECTIONS:
				{
					SendTotalConnections();
					break;
				}
			case LC_MAKE_PARTY:
				{
					LC_SendInputParty(t1);
					break;
				}
			case LC_DEFAULT_RUN:
				{
					Lc_DefaultMoveType = 1;
					Kein_PutMessage(KM_INFO, kein_GetMenuString(39));
					break;
				}
			case LC_DEFAULT_WALK:
				{
					Lc_DefaultMoveType = 0;
					Kein_PutMessage(KM_INFO, kein_GetMenuString(40));
					break;
				}
			case LC_TAME_STOP:
				{
					SendTameCommand(LC_TAME_STOP, t1);
					break;
				}
			case LC_TAME_ATTACK:
				{
					SendTameCommand(LC_TAME_ATTACK, t1);
					break;
				}
			case LC_TAME_FOLLOWME:
				{
					SendTameCommand(LC_TAME_FOLLOWME, t1);
					break;
				}
			case LC_TAME_NAME:
				{
					SendTameCommand( LC_TAME_NAME, t1 );	
					break;
				}
			case LC_OPEN_TIMEMENU:
				{
					g_bTimeMenuOn = true;
					break;
				}
			case LC_CLOSE_TIMEMENU:
				{
					g_bTimeMenuOn = false;
					break;
				}
			case LC_CLOSE_ALL:
				{
					g_bTimeMenuOn = false; g_ExplainMenuOn = false;
					CloseAllMenu();
					break;
				}
			case LC_EXPLAIN_OPEN:
				{
					g_ExplainMenuOn = true;
					break;
				}
			case LC_EXPLAIN_CLOSE:
				{
					g_ExplainMenuOn = false;
					break;
				}
			case LC_EAR_MESSAGE_REFUSE: 
				{
					ChatMgr.SetRefuseWhiper(t1);//021025 lsw
					break;
				}
			case LC_EAR_MESSAGE_OK: 
				{
					ChatMgr.SetAcceptWhiper(t1);//021025 lsw
					break;
				}
			case LC_EAR_MESSAGE_REFUSE_WHO:
				{
					ChatMgr.DisplayRefuseList();
					break;
				}
			case LC_REQ_MAN_TO_MAN:	
				{
					SendReqManToMan(t1); 
					break;		// 0810 YGI
				}
			case LC_GREETING:
				{
					strcpy(GreetingStr, t1);
					AddCurrentStatusMessage(FONT_COLOR_SOLID_GREEN, lan->OutputMessage(3,49), GreetingStr);//010215 lsw
					AddCurrentStatusMessage(FONT_COLOR_SOLID_GREEN, lan->OutputMessage(3,48));//010215 lsw
					break;
				}
			case LC_RESET_MAGIC_ARRAY:
				{
					if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL)
					{
						MagicSetting2(); // 원래~ 프리스트 경우...
					}

					break;
				}
			case LC_MSG_ALL_NATION:
				{
					// SendAllMsgNation(t1);
					break;
				}
			case LC_GUILD_EXIT:
				{
					SendGuildExit(t1);
					break;
				}
			case LC_GUILD_JOIN:
				{
					SendGuildJoin(t1);
					break;
				}
			case LC_GUILD_ALL_MSG:
				{
					SendGuildAllMsg(t1);
					break;
				}
			case LC_GUILD_CHANGE_EACH_LEVEL:
				{
					SendGuildChangeEachLevel(t1);
					break;
				}
			case LC_GUILD_CMD_INFO:
				{
					ViewGuildCommandInfo();
					break;
				}
			case LC_CHANGE_JOB:
				{
					break;//021126 lsw 직업 바꾸기 커멘드 없음
				}
			case LC_STOP_WAR:
				{
					SendReqStopWar(t1);
					break;
				}
			case LC_EAR_COMMAND_INFO:
				{
					ViewEarCommandInfo();
					break;
				}
			case LC_SCAN_TARGET_VYSEUS:
				{
					if (!ScanTarget(N_VYSEUS))
					{
						return 0;
					}

					break;
				}
			case LC_SCAN_TARGET_ZYPERN:
				{
					if (!ScanTarget(N_ZYPERN))
					{
						return 0;
						
					}

					break;  
				}
			case LC_SCAN_TARGET_YILSE:
				{
					if (!ScanTarget(N_YILSE))
					{
						return 0;
					}

					break;		// 011217 LTS
				}
				// 001028 KHS
			case LC_KILL_CHARACTER:
				{
					if( Hero->IsCounselor() )
					{
						LPCHARACTER ch;
						t_packet p;
						
						if (g_GameInfo.lpvSelectedSprite)
						{
							if ((LPCHARACTER)g_GameInfo.lpvSelectedSprite != Hero)
							{
								ch = (LPCHARACTER)g_GameInfo.lpvSelectedSprite; 
								
								if (ch->sprno < 2)
								{
									p.h.header.type = CMD_MAKE_GHOST;
									p.u.make_ghost.id = ch->id;
									p.h.header.size = sizeof( t_make_ghost );
									QueuePacket( &p, 1 );
								}
							}
						}
					}

					break;
				}
			case LC_DIRECTIONS:
				{
					break;
					t_packet p;
					p.h.header.type = CMD_FACE_DIRECTIONS;
					p.h.header.size = sizeof(t_face_directions);
					p.u.face_directions.dir = atoi(t1);
					p.u.face_directions.id  = Hero->id;
					QueuePacket(&p , 1);
					Hero->todir = (DIRECTION)atoi(t1);
					break;
				}
				//020515 lsw
			case LC_OPEN_DISPLAY_SKILL_EXP://	OpenDisplaySkillExp(); break;
			case LC_OPEN_DISPLAY_TAC_EXP://	OpenDisplayTacExp(); break;
			case LC_CLOSE_DISPLAY_SKILL_EXP://	CloseDisplaySkillExp(); break;
			case LC_CLOSE_DISPLAY_TAC_EXP://	CloseDisplayTacExp(); break;
				{
					break;
				}
				// 001129 KHS
			case LC_LIGHTOFRESCUE:
				{
					gr.GuideGhost_TypeTheHelp(true);
					break;
				}
			case LC_AUTOLEVELUP_CLOSE:
				{
					if (SCharacterData.nLevel <= 30)	// 010815 YGI
					{
						AddCurrentStatusMessage(FONT_COLOR_SOLID_GREEN, lan->OutputMessage(3,19));//010215 lsw
					}

					AutoLevelUpOn = false; 
					break;
				}
			case LC_AUTOLEVELUP_OPEN:
				{
					if (SCharacterData.nLevel <= 30)	// 010815 YGI
					{
						AddCurrentStatusMessage(FONT_COLOR_SOLID_GREEN, lan->OutputMessage(3,20));//010215 lsw
					}
					
					AutoLevelUpOn = true; 
					break;
				}
			case LC_ANNONCE_ON:	//if ( pMusic->OpenFile( "./sound/start.mp3" ) )	pMusic->Play();
				{
					break;	
				}
			case LC_ANNONCE_OFF:
				{
					pMusic->Stop();
					break;
				}
			case LC_FIGHTMAP_ALL_CHAT:
				{
					SendChatDataGlobalFightMap(t1);
					break;
				}
			case LC_SMALL_TIPS:
				{
					CallViewSmallTips(t1);
					break;
				}
			case LC_EMOTION:
				{
					SendLcEmotion(t1);
					break;
				}
			case LC_EXIT_GUILD:
				{
					SendExitGuild();
					break;
				}
			case LC_VIEW_NK:
				{
					ViewNk(t1);
					break;
				}
			case LC_SQUAD_CHAT:
			case LC_SQUAD_LEADER:	
			case LC_SQUAD:	
			case LC_SQUAD_MEMBER:
			case LC_SQUAD_MEMBER_TO_MEMBER:
				{
					SendSquadChatMessage(lc->command,t1);	// Nation.cpp
					break;
				}
			case LC_WAR_MENU_OPEN:
				{
					WarControl(t1);	// 011020 LTS
					break;
				}
			case LC_WAR_MENU_NOTICE_OPEN:
				{
					WarLoopTime(t1);						// 011025 LTS
					break;
				}
			case LC_GUILD_MAIL:
				{
					break;//
				}
			case LC_GUILD_CHAT:
				{
					SendChatGuild(t1);
					break;
				}
			case LC_COMMANDER_INFO:
				{
					ShowCommanderInfo();
					break;				// 010924 LTS
				}
			case LC_SQUAD_INFO:
				{
					ShowSquadInfo();
					break;
				}
			case LC_GUILD_CHAT_ON:
				{
					g_Menu_Variable.m_bGuildChatOn = true;
					break;
				}
			case LC_GUILD_CHAT_OFF:
				{
					g_Menu_Variable.m_bGuildChatOn = false;
					break;
				}
			case LC_RESET_ABILITY:	
				{	// 020925 YGI
				#ifdef _NO_COMMAND_RESET_ABILITY
					break;
				#endif
					int iAble = 1;

					for (int i = 0; i < 8; i++)
					{
						if (EquipItemAttr[i].item_no)
						{	
							iAble = 0;
						}
					}

					if (iAble)
					{
						CallSmallMenu(MN_ABILITY_REDISTRIBUTION);
					}
					else
					{
						AddCurrentStatusMessage(FONT_COLOR_RED, lan->OutputMessage(0,500));//020701 lsw
					}

					break;	// LTS LOCALWAR
				}
			case LC_WAR_GIVE_LIFE:
				{
					WarGiveLife();
					break;					// 011015 LTS
				}
			case LC_PARTY_ALL_FREE:	
				{
					memset(SCharacterData.party, 0, sizeof(CharacterParty)*6);
					CallServer(CMD_PARTY_ALL_FREE);
					break;
				}
			case LC_SEND_MAIL:
			case LC_SEND_GUILD_MAIL:
				{
					::Call_MAIL_WRITE_MENU(t1);
					break;
				}
			case LC_SHOW_ME_DUAL:
				{
					CheckShowMeDual();
					break;
				}
			case LC_REPORTER_MODE:	
				{
					if (Hero->name_status.reporter)
					{
						if (!Hero->reporter_mode && IsDead(Hero))
						{
							break;
						}

						CallServer(CMD_REPORTER_MODE);
						//Hero->reporter_mode = !Hero->reporter_mode;
					} 
					else
					{
						Kein_PutMessage(KM_FAIL, kein_GetMenuString(97));
					}

					break;
				}
			case LC_FRIEND_MENU:
				{
					CallFriendMenu();
					break; //CallGuildMemberListMenu(); break;
				}
			case LC_WAR_GIVE_LIFE2:
				{
					WarGiveLife2(t1);
					break;			// 011213 LTS
				}
			case LC_GETCOLOSSUS_INFO:
				{
					CallServer(CMD_CHECK_COLOSSUS_MAP);
					Kein_PutMessage(KM_INFO , kein_GetMenuString(152));
					break;
				}
			case LC_INVITE_COLOSSUS:
				{	//< CSD-030521
					if (!g_pArenaManager->IsLeader(Hero->id)) // 리더가 아니라면
					{
						Kein_PutMessage(KM_FAIL, kein_GetMenuString(153));
					}
					else
					{	
						static DWORD time = g_curr_time - 20;

						DWORD gab = g_curr_time-time;

						if (gab < 10)
						{
							MP3(SN_WARNING);
							Kein_PutMessage(KM_FAIL, kein_GetMenuString(156), 10 - gab);
						}
						else
						{
							time = g_curr_time;
							CallServer(CMD_INVITE_COLOSSUS);
						}
					}
					
					break;
				}	//> CSD-030521
			case LC_JOIN_COLOSSUS: 
				{
					SendJoinColossusMap();
					break;
				}
			case LC_PK_ON_OFF:
				{
					CallServer(CMD_GET_PK_ON_OFF);
					break;	
				}
			case LC_COMBAT_SKILL_ON_OFF://020420 lsw
				{
					g_mgrBattle.SendCombatRequest(Hero);
					break;
				}
			case LC_LOCALWAR_CHAT:		// LTS NEW LOCALWAR
				{
					SendLocalWarChat(t1);		// LTS NEW LOCALWAR	
					break;
				}
			case LC_QUESTMODE: 
				{
					CharUpper(t1);
					SendCMD_SET_QUESTMODE(t1); 
					break;	// LTS DRAGON MODIFY
				}
			case LC_WEATHER_ON:
				{	//< CSD-CN-031222
					AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0,628));
					t_packet packet; //Packet to ask current weather
					packet.h.header.size = sizeof(t_rm_change_weather);
					packet.h.header.type = CMD_RM_CHANGE_WEATHER;
					packet.u.RmChangeWeather.bStopWeather = -1;
					packet.u.RmChangeWeather.bWeather = -1;
					packet.u.RmChangeWeather.dwAmount = -1;

					QueuePacket(&packet, 1);
					g_bOffWeatherSystem = false;
					break;
				}	//> CSD-CN-031222
			case LC_WEATHER_OFF:
				{	//< CSD-CN-031222
			
					AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0,629));
					RainStop();
					ThunderStop();
					SnowStop();
					g_bOffWeatherSystem = true;
					break;
				}	//> CSD-CN-031222
			case LC_COMBAT_RESET:
				{	//< CSD-030306
				#ifdef _DEBUG
					CallServer(CMD_COMBAT_RESET);
				#endif
					break;
				}	//> CSD-030306


			case LC_MAGIC_DISPLAY_ON:
				{	//< CSD-030306
					AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0,626));
					g_mgrBattle.SetDisplay(true);
					g_ParticleManager.SetDisplay(true);
					break;
				}	//> CSD-030306
			case LC_MAGIC_DISPLAY_OFF:
				{	//< CSD-030306
					AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0,627));
					g_mgrBattle.SetDisplay(false);
					g_ParticleManager.SetDisplay(false);
					break;
				}	//> CSD-030306
			case LC_SET_NORMAL_CHAT_MODE://020620 lsw
				{
					SetChatTarget(CHAT_TARGET_NORMAL);
					SendChatNormal(t1);		
					break;
				}
			case LC_REPRINT_EAR_MESSAGE	:	
				{
					ChatMgr.DisplayPastData(CHATTYPE_WHISPER);		//020704 lsw
					break;
				}
			case LC_REPRINT_NORMAL_CHAT:
				{
					ChatMgr.DisplayPastData(CHATTYPE_NORMAL);		//020704 lsw
					break;
				}
			case LC_REPRINT_GUILD_CHAT:
				{
					ChatMgr.DisplayPastData(CHATTYPE_GUILD);		//020704 lsw
					break;
				}
			case LC_REPRINT_GUILD_BBS:
				{
					ChatMgr.DisplayPastData(CHATTYPE_GUILD_BBS);		//020704 lsw
					break;
				}
			case LC_REPRINT_PARTY_CHAT: // Finito for /party note command
				{
					ChatMgr.DisplayPastData(CHATTYPE_PARTY);		//020704 lsw
					break;
				}
			case LC_TRADE://020808 YGI
				{
					TradeONOFF(t1);
					break;
				}
			case LC_CHECK_DUAL_FAME:	  // LTS DUAL FAME
				{
					SendCheckDualFame();
					break;
				}
			case LC_WHAT_QUEST_IN_MAP:	// 021007 kyo //퀘스트정보를 본다. 
				{
					SendWhatQuestInMap(t1);
					break;
				}
			case LC_MERCHANT_BBS://021126 lsw
				{
					if (Hero->viewtype == VIEWTYPE_GHOST_) //다른 사람은 고스트 이며	(딴사람은 유령)
					{
						break;
					}

					if (IsMerchant())
					{
						::SendChatData( t1, CHATTYPE_MERCHANT_BBS);
					}

					break;
				}
			case LC_MERCHANT_EXCHANGE://021026 lsw
				{
					Auction.SendCMD_MERCHANT_EXCHANGE_LIST_REQUEST(t1,0,1,true);
					break;
				}
			case LC_EVENT_MOVE_MAP:	// 021107 YGI
				{
					SendEvnetMoveMap();
					break;
				}
				//<< 031021 kyo
			case LC_EMOTI_SMILE:					
			case LC_EMOTI_SHY:
			case LC_EMOTI_CRY:
			case LC_EMOTI_ANGER:
			case LC_EMOTI_HAPPY:
			case LC_EMOTI_LOVE:
			case LC_EMOTI_TELL:
			case LC_EMOTI_THANKS:
			case LC_EMOTI_SORRY:
			case LC_EMOTI_GOOD:
			case LC_GAME_SCISSOR:
			case LC_GAME_ROCK:
			case LC_GAME_PAPER:
			case LC_GAME_GO:
			case LC_GAME_HELP:
			case LC_GAME_PARTY:
			case LC_GAME_HEAL:
			case LC_GAME_TRANSACTION:
			case LC_GAME_BEG:
			case LC_GAME_CONGRAGULATION:
				{
					::SendSmileFace((lc->command) - LC_EMOTI_SMILE + 2); // 얼굴아이콘이 1부터 시작한다.
					break;
				}
			case LC_BBS_LIMITED_USER_ADD:
				{
					if (!ChatMgr.AddLimintedBBSName(t1))
					{	// 제한인원에 걸린다.
						::AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(0, 512));
					}
					else if (strlen(t1) > 0)
					{
						::AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(0, 510), t1);
					}

					break;
				}
			case LC_BBS_ON:
				{
					ChatMgr.SetActiveBBS(true);
					::AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(0, 513));
					//ChatMgr.ShowLimitedList(); // 031031 kyo
					break;						
				}
			case LC_BBS_OFF:
				{
					ChatMgr.SetActiveBBS(false);
					::AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(0, 514));
					break;						
				}
				//<< 031021 kyo
			case LC_RAID_START:
				{	//< CSD-040310
				#ifdef _DEBUG
					CallServer(CMD_MONSTER_RAID_START);
				#endif
					break;
				}	//> CSD-040310
			case LC_RAID_END:
				{	//< CSD-040310
				#ifdef _DEBUG
					CallServer(CMD_MONSTER_RAID_END);
				#endif
					break;
				}	//> CSD-040310
			case LC_GET_SMP:
				{
					CallServer(CMD_GET_SKILL_POINT);
					break;
				}
			case LC_DENY_DUEL_REQ:
				{
					t_packet	packet;
					packet.h.header.type = CMD_DUEL_ALLOWDENY;
					{
						packet.u.DuelAllowDeny.bIsDuelAllow = false;
					}
					packet.h.header.size = sizeof( t_DuelAllowDeny );
					QueuePacket( &packet, 1 );		
					AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW , lan->OutputMessage(3, 293));
					break;
				}
			case LC_ALLOW_DUEL_REQ:
				{
					t_packet	packet;
					packet.h.header.type = CMD_DUEL_ALLOWDENY;
					{
						packet.u.DuelAllowDeny.bIsDuelAllow = true;
					}
					packet.h.header.size = sizeof( t_DuelAllowDeny );
					QueuePacket( &packet, 1 );		
					AddCurrentStatusMessage( FONT_COLOR_SOLID_YELLOW , lan->OutputMessage(3, 294));
					break;
				}
			case LC_TOGGLE_DROP: // Eleval for /drop function 22/05/09
				{
					if(g_allowDrop)
					{
						g_allowDrop = false;
						AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,209));
						break;
					}
					else
					{
						g_allowDrop = true;
						AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,210));
						break;
					}
					break;

				}
			case LC_TAME_AUTO_ATTACK: // Eleval for /auto attack function 22/05/09
				{
					if(g_autoAttack)
					{
						g_autoAttack = false;
						AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,295));
						break;
					}
					else
					{
						g_autoAttack = true;
						AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,296));
						break;
					}
					break;
				}
			case LC_TAME_AUTO_DEFENCE: // Eleval for /auto defence function 23/05/09
				{
					if(g_autoDefence)
					{
						g_autoDefence = false;
						AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,297));
						break;
					}
					else
					{
						g_autoDefence = true;
						AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW, lan->OutputMessage(3,298));
						break;
					}
					break;
				}
			case LC_TAME_ALL_FOLLOW: //Eleval for /tame follow command 24/05/09
				{
					g_mgrBattle.SendTameCommand(Hero, Hero, LC_TAME_FOLLOWME);
					break;
				}
			case LC_TAME_ALL_STOP: //Eleval for /tame stop command 24/05/09
				{
					g_mgrBattle.SendTameCommand(Hero, Hero, LC_TAME_STOP);
					break;
				}
			case LC_REPORTER_TELEPORT: //Eleval 15/08/09 - Command for RCs to teleport themselves anywhere
				{
					SendTeleportCommand(t1);
					break;
				}
			case LC_HIDE: //Eleval 18/08/09 - So that people can hide some stuff
				{
					HideSomething(t1);
					break;
				}
			case LC_HONOR: //Eleval 09/09/09 - To show the Player's current honor
				{
					CallServer(CMD_REQ_HONOR_AMOUNT);
					break;
				}

			case LC_EXP_RESET:
			{	//< CSD-030306
#ifdef _DEBUG
				CallServer(CMD_EXP_RESET);
#endif
				break;
			}	//> CSD-030306

			
		
			case LC_NATION_CHAT:
			{
				SendNationChat(t1);
			}
			case LC_USER_IMPRISON:
			{
				g_cPrison.BadUserImprison(t1);
			}
			break;
			case LC_USER_RELEASE:
			{
				g_cPrison.BadUserRelease(t1);
			}
				break;

			case LC_USER_BAN:
			{
				
				BannedUser(t1);
			}
			break;
			}
			
			return 1;
		}	

		lc = &LC[++c];
	}

	return 0;
}

void SendTotalMapConnections( void )
{
	//	t_packet p;
	//	p.h.header.type = CMD_TOTAL_MAP_CONNECTIONS;
	//	p.h.header.size = 0;
	//	QueuePacket( &p, 1 );
}

void SendTotalConnections( void )
{
	//	t_packet p;
	//	p.h.header.type = CMD_TOTAL_CONNECTIONS;
	//	p.h.header.size = 0;
	//	QueuePacket( &p, 1 );
}


void RecvTotalMapConnections( int no )
{
	AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(3,53), no );//010215 lsw
}

void RecvTotalConnections( int no )
{
	AddCurrentStatusMessage( 255,170,170, lan->OutputMessage(3,54),  no );//010215 lsw
}

void LC_SendInputParty( char *name )		// 0929 YGI
{
	LPCHARACTER target = ExistHe( name );
	if( !target ) return;
	
	int i;
	for(i=0; i<6; i++ )
	{
		if( !SCharacterData.party[i].m_Name[0] ) break;
	}
	if( i == 6 ) return;
	
	SetMouseCh( target );
	SendInputParty( PARTY, i, &mouse_ch);
	memset( &mouse_ch, 0, sizeof( CharacterParty ) );
}


void LC_SendInputnATIONParty()		// 0929 YGI
{

	t_packet packet;
	NationParty* N;
	N->On = TRUE;
	
}


void SendTameCommand(int command, char* s)
{	//< CSD-031106
	char* tok = strtok(s, " ");
	
	if (tok == NULL) 
	{
		switch (command)
		{
		case LC_TAME_STOP:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 55));
				break;
			}
		case LC_TAME_ATTACK:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 56));
				break;
			}
		case LC_TAME_FOLLOWME:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 57));
				break;
			}
		case LC_TAME_NAME:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 58));
				break;
			}
		}
		
		return;
	}

	char mybaby[FILENAME_MAX];
	strcpy(mybaby, tok);
	
	tok = strtok(NULL, " \n");

	char toname[FILENAME_MAX];

	if (tok == NULL) 
	{
		toname[0] = 0;
	}
	else
	{
		strcpy(toname, tok);
	}
	
	if (toname[0] == 0)
	{
		switch (command)
		{
		/*
		case LC_TAME_ATTACK:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 59));	
				return;
			}
		*/
		case LC_TAME_NAME:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 60));
				return;
			}
		}
	}
	
	t_packet p;
	p.h.header.type = CMD_TAME_COMMAND;
	p.u.tame_command.cmd = command;
	mybaby[19] = 0;
	EatRearWhiteChar(mybaby);
	strcpy(p.u.tame_command.mybaby, mybaby);
	EatRearWhiteChar( toname );
	toname[19] = 0;
	strcpy(p.u.tame_command.toname, toname);
	p.h.header.size= sizeof(t_tame_command);
	QueuePacket(&p, 1);
}	//> CSD-031106

void RecvTameCommandResult( int result )
{
	switch( result )
	{
	case TAME_FAIL_CHANGE_NAME		: AddCurrentStatusMessage( 214,100,122, lan->OutputMessage(3,74));		break;//010215 lsw
	case TAME_NO_CHAR				: AddCurrentStatusMessage( 214,100,122,lan->OutputMessage(3,75) ); break;
	case TAME_NO_NAME_CHAR			: AddCurrentStatusMessage( 214,100,122, lan->OutputMessage(3,76));	break;
	case TAME_FOLLOW				: AddCurrentStatusMessage( 114,100,122, lan->OutputMessage(3,77));	break;
	case TAME_STOP					: AddCurrentStatusMessage( 114,100,122, lan->OutputMessage(3,78));	break;
	case TAME_ATTACK				: AddCurrentStatusMessage( 114,100,122, lan->OutputMessage(3,79));	break;
	case TAME_SUCCESS_CHANGE_NAME	: AddCurrentStatusMessage( 114,100,122, lan->OutputMessage(3,80));	break;//010215 lsw
	}
}

void RecvTameNameChange( t_tame_name_change *p )
{
	LPCHARACTER ch = FindCharacter( &g_CharacterList, p->id);
	if( ch == NULL ) return;
	strcpy( ch->name, p->name );
}


void RecvHostName( tame_host_name *p )
{
	LPCHARACTER ch = FindCharacter( &g_CharacterList, p->id);
	if( ch == NULL ) return;
	strcpy( ch->HostName, p->hostname );
}

//   Tamming 끝..
void RecvHostEnd(  tame_end *p )
{
	LPCHARACTER ch = FindCharacter( &g_CharacterList, p->id);
	if( ch == NULL ) return;
	
	ch->HostName[0] = 0;
}

void SendBBS( char *bbs)
{
	t_packet p;
	int len;
	
	if( bbs == NULL ) return;
	len = strlen( bbs );
	if( len == 0 ) return;
	if( len >= MAX_PATH-1 ) return;
	
	p.h.header.type = CMD_BBS;
	strcpy( p.u.server_bbs.bbs, bbs );
	p.h.header.size = sizeof( t_server_bbs ) - MAX_PATH + len;
	
	QueuePacket( &p, 1 );
}

void SendBBSAll( char *bbs)
{
	t_packet p;
	int len;
	
	if( bbs == NULL ) return;
	len = strlen( bbs );
	if( len == 0 ) return;
	if( len >= MAX_MSG-1 ) return;
	
	p.h.header.type = CMD_SV_SEND_MESSAGE_ALL;
	strcpy( p.u.kein.default_msg, bbs );
	p.h.header.size = len;
	
	QueuePacket( &p, 1 );
}

////////////// 0810 lhs 퀘스트용  도움말 //////////////
void RecvBbs_quest( char *bbs )
{
	AddCurrentStatusMessage( 200,200,0, lan->OutputMessage(3,132) );//010215 lsw
	AddCurrentStatusMessage( 255,255,0, bbs );
}


////////////////////////////////////////////////////////////////////////////////
// CSmallTipCount member functions
CSmallTipCount	g_SmallTipCount;
int LoadSmallTipsPage()
{
	FILE *fp;
	fp = Fopen( "./data/small_tips.txt", "rt" );
	if( !fp ) return 0;
	
	char str[512];
	while( fgets( str, 512, fp ) )
	{
		if( str[0] == ':' ) break;
		char *token;
		token = strtok( str, "\t\n" );
		if( !token ) continue;
		
		int number = atoi( token );
		if( !number ) continue;
		
		token = strtok( NULL, "\t\n" );
		if( !token ) continue;
		
		while( token )
		{
			g_SmallTipCount.AddData( number, token );
			token = strtok( NULL, "\t\n" );
		}
	}
	
	fclose( fp );
	return 1;
}

int CSmallTipCount::AddData( int number, char *str )
{
	CSmallTipCount* target = this;
	
	while( target->next ) target = target->next;
	if( m_nPage == 0 )
	{
		strncpy( m_szKey, str, 49 );		// 020925 YGI
		m_szKey[49] = 0;					// 020925 YGI
		m_nPage = number;
		return 1;
	}
	else
	{
		target->next = new CSmallTipCount;
		strcpy( target->next->m_szKey, str );
		target->next->m_nPage = number;
		return 1;
	}
}

int CSmallTipCount::GetPage( char *key )
{
	if( !m_nPage ) return 0;
	
	CSmallTipCount *target = this;
	while( target )
	{
		if( strcmp( target->m_szKey, key ) == 0 ) return target->m_nPage;
		target = target->next;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void CallViewSmallTips( char *txt )
{
	if( IsDead( Hero ) ) return;
	char *token = strtok( txt, " \t\n" );
	if( !token ) token = txt;
	else
	{
		token = strtok( NULL, " \t\n" );
		if( !token ) token = txt;
	}
	static bool first = true;
	if( first ) 
	{
		LoadSmallTipsPage();
		first = false;
	}
	
	int t = 0;
	if( *token )
		t = g_SmallTipCount.GetPage( EatRearWhiteChar( token ) );
	
	CloseAllMenu();
	SMenu[MN_SMALL_TIPS].key = t;
	bSmallTips = true;
}

CTextCmd *g_lpTextCmd;
CEmotionCmd *g_lpEmotionCmd;
void SendLcEmotion( char *txt )
{
	return;
}

///////////////////////////////////////////////////////////////
// CTextCmd member fuction
void CTextCmd::AddText(char *txt)
{
	if( !m_txt )
	{
		m_txt = new char[strlen(txt)+1];
		strcpy( m_txt, txt );
		return;
	}
	
	CTextCmd *temp=this;
	for( ; temp->next; temp=temp->next ) ;
	
	CTextCmd *data;
	data = new CTextCmd;
	data->m_txt = new char[strlen(txt)+1];
	strcpy( data->m_txt, txt );
	temp->next = data;
	return;
}
char *CTextCmd::GetLastText()
{
	CTextCmd *temp=this;
	for( ; temp->next; temp=temp->next ) ;
	return temp->m_txt;
}
char *CTextCmd::GetFirstText()
{
	return m_txt;
}

///////////////////////////////////////////////////////////////
// CEmotionCmd member function
void CEmotionCmd::AddCmd( char *cmd )
{
	if( !m_Cmd )
	{
		m_Cmd = new char[strlen(cmd)+1];
		strcpy( m_Cmd, cmd );
		return;
	}
	
	CEmotionCmd *temp=this;
	for( ; temp->next; temp=temp->next ) ;
	
	CEmotionCmd *data;
	data = new CEmotionCmd;
	data->m_Cmd = new char[strlen(cmd)+1];
	strcpy( data->m_Cmd, cmd );
	temp->next = data;
	return;
}

int CEmotionCmd::GetCmd( char *txt )
{
	CEmotionCmd *temp = this;
	int count = 0;
	while( temp )
	{
		if( strcmp( temp->m_Cmd, txt ) == 0 )
			return count;
		count ++;
		temp = temp->next;
	}
	return -1;
}
///////////////////////////////////////////////////////////////
int LoadEmotionCmd( CEmotionCmd *lpEmotionCmd )
{
	FILE *fp;
	fp = Fopen( "./data/emotion.txt", "rt" );
	if( !fp ) return 0;
	
	char temp[256];
	while( fgets( temp, 256, fp ) )
	{
		if( temp[0] == ';' ) continue;
		lpEmotionCmd->AddCmd( EatRearWhiteChar(temp) );
	}
	fclose( fp );
	return 1;
}

void SendExitGuild()
{
	if (!CheckGuildCode(Hero->GetGuildCode())) 
	{	//< CSD-030324
		return;
	}	//> CSD-030324

	if( CheckGuildMaster( Hero ) ) return;		// 길마는 하지 못하게..
	CallServer( CMD_EXIT_GUILD_SELF );
}

void ViewNk( char *name )
{
	EatRearWhiteChar( name );
	name = EatRearWhiteChar( name );
	LPCHARACTER target = ExistHe( name );
	if( !target ) target = Hero;
	
	Kein_PutMessage( KM_INFO, "[%s] Vysues NK:%3d", target->name, IsNK( target, N_VYSEUS ));
	Kein_PutMessage( KM_INFO, "[%s] Zypern NK:%3d", target->name, IsNK( target, N_ZYPERN ));
	Kein_PutMessage( KM_INFO, "[%s] Yilse  NK:%3d", target->name, IsNK( target, N_YILSE ));
}

extern int IsDead( LPCHARACTER ch );

void RecvGuildChatData( t_packet *p )
{
	//if( IsDead( Hero ) ) return; To see guildchat while dead - Thralas
	
	static int first = 1;
	if( first && !g_Menu_Variable.m_bGuildChatOn )
	{
		// 길드원이 메시지를 보냈습니다. 길드 채팅 명령어를 이용하세요 /#, /길드채팅켜기, /길드채팅끄기
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 70 ) );
		first = 0;
	}
	char *name = p->u.kein.guild_chat.name;
	char *msg = p->u.kein.guild_chat.msg;
	ChatMgr.AddString( name, msg, CHATTYPE_GUILD );//021001 lsw
}

void CheckShowMeDual()
{
	if( EquipItemAttr[WT_NECK].item_no == 10175 )
	{
		CallServer( CMD_SHOW_ME_DUAL_CHAR );
	}
	else
	{
		CItem *item = ItemUnit( 10175 );
		if( item )
			Kein_PutMessage( KM_FAIL, kein_GetMenuString( 78 ), item->GetItemHanName() );
	}	
}
//void sendBanCommand(char* s)
//
//{
//	t_packet packet;
//	char* tok = strtok(s, " ");
//
//	if (tok == NULL)
//	{
//		AddCurrentStatusMessage(255, 0, 0, "Invalid name !");
//		return;
//	}
//	char charname[256];
//	strcpy(charname, tok);
//	tok = strtok(NULL, " ");
//
//	
//	strcpy(Reason, tok);
//
//	tok = strtok(NULL, " ");
//	char Duration[256];
//	strcpy(Duration, tok);
//	tok = strtok(NULL, " ");
//
//
//	AddCurrentStatusMessage(255, 0, 0, "Asking ban for %s %s, %s", charname, Reason, Duration);
//
//	
//	packet.u.BanAccount.characterName=charname;
//	packet.u.BanAccount.reason;
//	packet.u.BanAccount.duration = Duration;
//	strcpy(packet.u.kein.direct_map_move.name, "");
//	packet.h.header.type = CMD_DIRECT_MAP_MOVE;
//	packet.h.header.size = sizeof(k_direct_map_move) - 20;// + strlen(temp);
//	QueuePacket(&packet, 1);
//}
void SendTeleportCommand(char* s) //Eleval 15/08/09 - To manage RCs Teleport
{
	char* tok = strtok(s, " ");
	
	if (tok == NULL) 
	{
		AddCurrentStatusMessage(255, 0,0, "Invalid Map !");
		return;
	}

	char MapToChange[128];
	strcpy(MapToChange, tok);
	
	tok = strtok(NULL, " ");

	char CoordX[32];

	if (tok == NULL) 
	{
		char lpszPW[21] = {0,};

		strcpy(lpszPW, MapToChange);
		CallServer( CMD_TELEPORT_OTHER_CHAR, lpszPW, strlen(lpszPW)  );
		return;
	}
	else
	{
		strcpy(CoordX, tok);
	}

	tok = strtok(NULL, " ");

	char CoordY[32];

	if (tok == NULL) 
	{
		AddCurrentStatusMessage(255, 0,0, "Invalid Coords !");
		return;
	}
	else
	{
		strcpy(CoordY, tok);
	}

	int iCoordX = 0;
	int iCoordY = 0;

	for(int i = 0 ; i < 3 ; i++)
	{
		if(CoordX[i] >= 48 && CoordX[i] <= 57)iCoordX = iCoordX * 10 + (CoordX[i] - 48);
		if(CoordY[i] >= 48 && CoordY[i] <= 57)iCoordY = iCoordY * 10 + (CoordY[i] - 48);
	}

	if(iCoordX < 0 || iCoordX > 999 || iCoordY < 0 || iCoordY > 999)
	{
		AddCurrentStatusMessage(255, 0,0, "Invalid Coords !");
		return;
	}

	AddCurrentStatusMessage(255, 0,0, "Asking tp at %s %d, %d", MapToChange, iCoordX, iCoordY);

	t_packet packet;
	packet.u.kein.direct_map_move.map_number = ConvMapnameToMapnumber(MapToChange);
	packet.u.kein.direct_map_move.x = iCoordX;
	packet.u.kein.direct_map_move.y = iCoordY;
	strcpy(packet.u.kein.direct_map_move.name, "");
	packet.h.header.type = CMD_DIRECT_MAP_MOVE;
	packet.h.header.size = sizeof(k_direct_map_move) - 20;// + strlen(temp);
	QueuePacket(&packet, 1);
	
}

void HideSomething(char *s)
{
	char* tok = strtok(s, " \n");
	
	if (tok == NULL) 
	{
		AddCurrentStatusMessage(255, 0,0, "Command is /hide [object].");
		return;
	}

	if(stricmp(tok, "hat") == 0)
	{
		if(Hero->hat_is_hidden)
		{
			Hero->hat_is_hidden = false;

			t_packet packet;
			packet.h.header.type = CMD_CHAR_OPTIONS_CHANGE;
			packet.h.header.size = sizeof(t_change_char_options);
			packet.u.change_char_options.flag = false;
			packet.u.change_char_options.type = 1;

			QueuePacket(&packet, 1);
		}
		else
		{
			Hero->hat_is_hidden = true;

			t_packet packet;
			packet.h.header.type = CMD_CHAR_OPTIONS_CHANGE;
			packet.h.header.size = sizeof(t_change_char_options);
			packet.u.change_char_options.flag = true;
			packet.u.change_char_options.type = 1;

			QueuePacket(&packet, 1);
		}
		return;
	}
}