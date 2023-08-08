#include "stdafx.h"																	
#include "String.h"
#include "mmsystem.h"														
#include "Dragon.h"	
//#include "MenuDef.h"

#include "network.h"																	

#include "char.h"
#include "chat.h"//021026 lsw

#include "Hong_Sprite.h"														
#include "Hong_Sub.h"														
#include "CharDataTable.h"													
#include "Path.h"																
#include "Hong_Light.h"
#include "Effect.h"
#include "DirectSound.h"
#include "Map.h"
#include "Object.h"
#include "Menu.H"
#include "GameProc.h"
#include "SmallMenu.h"
#include "Tool.h"
#include "Prison.h"
//nclude "MenuSet.h"
#include "Skill.h"
#include "Item.h"
#include "MenuNetwork.h"
#include "ExchangeMgr.h"
extern int				ListenCommand;
extern void RecvHeroInit( t_server_user_db_data *p );
//extern void CallOkCancelMessageBox( int menu, int x, int y, char *source, bool ok_cancel=true, int param = 0 );
extern CPrison g_cPrison;

int CheckHandleByKein( t_packet *p )
{
	//	char text_buffer[TEXT_LENGTH];
	
	switch( p->h.header.type )
	{
		
	case CMD_PUBLIC_NOTICE_TITLE_REQ:	RecvPublicNoticeTitle( &p->u.server_menu_message_title ); break;
	case CMD_RESEND_CHARLIST		:	RecvCharInfomation( &p->u.server_user_db_data ); break;
	case CMD_CHAR_INFO_BASIC		:	RecvCharInfoBasic( &p->u.char_info_basic ); break;
		//	RecvHeroInit( &p->u.server_user_db_data ); break;
		
	case CMD_OTHER_CH_INV			:	RecvOtherChInv( &p->u.kein.server_other_ch_inv ); break;
		//021030 lsw 박스는 없습니다.
	case CMD_DELETE_ITEM_BY_DURATION:	RecvDeleteItem( p->u.kein.server_delete_item.item_pos, 1 ); break;
	case CMD_DELETE_ITEM			:	RecvDeleteItem( p->u.kein.server_delete_item.item_pos ); break;
	case CMD_DECREASE_ITEM			:	RecvDecreaseItem( &p->u.kein.server_decrease_item ); break;
	case CMD_LEVELDOWN_OK			:	RecvLevelDownResult( p->u.kein.client_lvup_point_each.type ); break;
		
	case CMD_LEARN_ITEM				:	RecvLearnItemOk( p->u.kein.item_no ); break;
		
	case CMD_ITEMREPAIR_OK			:	RecvItemRepar( &p->u.kein.server_item_repair_ok ); break;
	case CMD_ITEMREPAIR_HAMMER_OK	:	RecvItemReparHammer( &p->u.kein.server_item_repair_ok ); break;
	case CMD_RESULT					:	RecvServerResult( &p->u.kein.server_result ); break;
	case CMD_CHAR_INFO_TAC_SKILL_EXP:	RecvTacSkillExp( &p->u.char_info_tac_skillexp ); break;
	case CMD_LEARN_SKILL_OK			:	RecvLearnSkill( p->u.kein.server_learn_skill_ok.skill_no ); break;
	case CMD_SKILL_EXP				:	RecvSkillExp( p->u.kein.server_skill_exp.type, p->u.kein.server_skill_exp.exp ); break;
	case CMD_OPEN_STATUS_REQ		:	RecvStatusOpen( &p->u.kein.server_status_open );break;
		
	case  CMD_SEND_BANK_INV			:	RecvBankInventory( &p->u.kein.server_bank_item ); break;
		
		//		case CMD_SEND_CHAR_DISEASE		:	RecvCharDisease( &p->u.kein.server_disease ); break;	
	case CMD_PARTY					:	RecvParty( &p->u.kein.server_party ); break;			// 서버에서 파티맺은 사람을 보내준다
	case CMD_RELATION				:	RecvRelation( &p->u.kein.server_relation ); break;
	case CMD_EMPLOYMENT				:	RecvEmployment( &p->u.kein.server_party ); break;
		//		case CMD_ITEM_INDEX				:	RecvItemIndex( &p->u.kein.server_item_index); break;
		
	case CMD_LEARN_MAGIC_OK			:	RecvLearnMagicOk( p->u.kein.learn_magic_ok.magic_num ); break;
	case CMD_PREVENTING				:	RecvPreventingOk( p->u.kein.client_disease.disease_num ); break;
	case CMD_CURE_DISEASE			:	RecvCureDiseaseOk( p->u.kein.client_disease.disease_num ); break;
		
	case CMD_REQ_ITEM_VALUE			:	RecvItemValue( p->u.kein.server_item_money.item_no, p->u.kein.server_item_money.money ); break;
	case CMD_REQ_ITEM_VALUE_SELL	:	RecvItemValue_Sell( p->u.kein.server_item_money.item_no, p->u.kein.server_item_money.money ); break;
	case CMD_REQ_ITEM_VALUE_REPAIR	:	RecvItemValue_Repair( p->u.kein.server_item_money.item_no, p->u.kein.server_item_money.money ); break;
	case CMD_REQ_MALL_ITEM_PRICE	:	RecvMallItemPrice( p->u.kein.server_item_money.item_no, p->u.kein.server_item_money.money ); break;
	case CMD_SERVER_PARTY			:	RecvPartyInfo( &p->u.kein.party_info ); break;
	case CMD_DELETE_PARTY			:	RecvDeleteParty_server( p->u.kein.client_delete_party.type, p->u.kein.client_delete_party.ct ); break;
	case CMD_SERVER_WAKE_UP			:	RecvServerWakeUp(); break;
	case CMD_REQ_JOIN_GAME			:	RecvJoinGameWho( p->u.kein.client_char_name.name ); break;
	case CMD_SERVER_EXIT_GAME		:	RecvExitGameWho( p->u.kein.client_char_name.name ); break;
	case CMD_REQ_MY_PARTY			:	RecvMyPartyExist( p->u.kein.client_char_name.name ); break;		//
	case CMD_PARTY_JOIN_OK			:	RecvPartyJoinOk( p->u.kein.client_char_name.name ); break;
	case CMD_PARTY_JOIN_RELEASE		:	RecvPartyJoinRelease( p->u.kein.client_char_name.name ); break;		//
	case CMD_SERVER_EACH_ITEM		:	RecvServerEachItem( &p->u.kein.server_item ); break;		// 아이템 자리에 아이템을 집어 넣는다.
	case CMD_OPEN_ABILITY			:	RecvServerResist( &p->u.kein.server_resist ); break;
	case CMD_SUBTRACT_MONEY			:	SubtractMoney( p->u.kein.server_money.money ); break;
	case CMD_ADD_MONEY				:	AddMoney( p->u.kein.server_money.money ); break;
		
	case CMD_EXCHANGE_ITEM_READY	:	RecvExchangeItemOk( &p->u.kein.exchange_item_start ); break;
	case CMD_EXCHANGE_ITEM_REQ		:	RecvExchangeItemReq( &p->u.kein.exchange_item_start );	break;
	case CMD_EXCHANGE_ITEM			:	RecvExchange(); break;		// 교환 성립
	case CMD_EXCHANGE_ITEM_RESULT	:	RecvExchangeItemResult( p->u.kein.try_item_trade.result );	break;
	case CMD_EXCHANGE_OK_SIGN		:	
		{
			ExchangeMgr.SetOtherSideConformState(true); //021030 lsw
		}break;
	case CMD_EXCHANGE_CANCEL_SIGN		:	
		{
			ExchangeMgr.SetMySideConformState(false); //030108 lsw
			ExchangeMgr.SetOtherSideConformState(false); //030108 lsw
		}break;
	case CMD_EXCHANGE_CANCEL		:	RecvExchangeCancel(); break;
	case CMD_EXCHANGE_ITEM_EACH		:	RecvExchangeEach( &p->u.kein.transfer_item.item ); break;
		
	case CMD_CHECK_BETA_TEST_OK		:	ListenCommand = 100; break;
	case CMD_CHECK_BETA_TEST_NOT	:	ListenCommand = 10; break;
		
	case CMD_IATTR_DURATION			:	RecvItemAttrChange( CIA_DURATION, &p->u.kein.server_item_attr ); break;
	case CMD_IATTR_MUCH				:	RecvItemAttrChange( CIA_MUCH, &p->u.kein.server_item_attr ); break;
		//		case CMD_IATTR_LIMIT			:
		//		case CMD_IATTR_ATTR				:
		//		case CMD_IATTR_ID				:
		//		case CMD_IATTR_RESERVED0		;
		//		case CMD_IATTR_RESERVED1		:
		
	case CMD_REQ_AC					:	SCharacterData.ac = p->u.kein.server_ac.ac; break;
	case CMD_REQ_DMG				:	SCharacterData.nCharacterAbility[DAMAGE] = p->u.kein.default_int; break;
	case CMD_SERVER_KILLMON			:	SCharacterData.beat = p->u.kein.server_killmon.killmon; break;
	case CMD_OTHER_GHOST			:	RecvOtherGhost(); break;
		
	case CMD_CREATE_ABILITY			:	RecvCreateAbility( ); break;
	case CMD_REQ_MAGERY:	
		{
			RecvServerMagery(p->u.kein.server_tactics.nStep, p->u.kein.server_tactics.magery); 
			break;
		}
	case CMD_THROW_DICE				:	RecvThrowDice( &p->u.kein.start_throw_dice ); break;
	case CMD_EXCHANGE_ITEM_DEL		:	RecvExchangeItemDelWhereRecv( p->u.kein.transfer_item.item ); break;
		
	case CMD_GET_SERVER_TEXT		:	RecvServerMessage( &p->u.kein.get_server_text ); break;
		
	case CMD_PREACH_NAME			:	RecvPreachName( &p->u.kein.server_client_preach_name ); break;
		
	case CMD_NOW_GOD_MEETING_START	:	RecvGodMeetinMainStart( ); break;
	case CMD_NOW_GOD_MEETING_END	:	RecvGodMeetingMainEnd( ); break;
	case CMD_CHECK_NEW_CHAR_OK		:	ListenCommand = 100; break;
	case CMD_CHECK_NEW_CHAR_NOT		:	ListenCommand = 10; break;
		
	case CMD_MEETING_TEXT			:	RecvServerMessageDefault( &p->u.kein.server_text_default, &g_ServerText ); break;
	case CMD_GET_CHARINFO_BY_KEIN	:	RecvCharInfoPlus( &p->u.kein.server_to_client_char_info_plus ); break;		// 추가 캐릭터 정보 받아오기
		
	case CMD_CAN_MEMORIZE_OK		:	RecvCanMemorizeOk(); break;
	case CMD_CAN_MEMORIZE_NOT		:	CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(4,23), 0 ); break;//lsw
		
	case CMD_SEND_EAR_MESSAGE		:	//021026 lsw
		{
			ChatMgr.RecvWhisper( p->u.kein.send_ear_message.name, p->u.kein.send_ear_message.message, p->u.kein.send_ear_message.send_game_server, p->u.kein.send_ear_message.send_server_id ); 
		}break;
	case CMD_SEND_EAR_MESSAGE_OK	:	
	case CMD_SEND_EAR_MESSAGE_NOT	:	
		{// 같은 서버에 있을때
			RecvWhisperReply( p->h.header.type, p->u.kein.send_ear_message_ok.name );
		}break;	
	case CMD_GAME2CLIENT_EAR_RESULT :	
		{
			RecvWhisperReply2( p->u.kein.game2client_ear_result.ret ); 
		}break;
	case CMD_SEND_EAR_MESSAGE_RESULT_ETC:	
		{
			RecvWhisperReplyEtc( p ); 
		}break;
	case CMD_DIVINE_UP				:	AddCurrentStatusMessage( 255, 255, 255, lan->OutputMessage(4,25)); break;//lsw
	case CMD_DIVINE_DOWN			:	AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(4,26) ); break;//lsw
	case CMD_OPEN_SUPPER			:	RecvOpenSupper( p->u.kein.server_to_client_open_supper.much ); break;
		
	case CMD_NOTE_MEETING			:	RecvSetNoteMeetingMessage( p->u.kein.note_meeting.believe_god, p->u.kein.note_meeting.meeting_type ); break;
	case CMD_NOT_EXIST_GAMESERVER	:	ListenCommand = CMD_NOT_EXIST_GAMESERVER;
		
	case CMD_REQ_CHAR_INFO			:	RecvReqCharInfo( &p->u.kein.server_req_char_info ); break;
	case CMD_REQ_PARTY_INFO			:	RecvReqPartyInfo( &p->u.kein.rep_party_info); break;
	case CMD_LOGIN_TO_GAME_PUBLIC_BBS	:	RecvCallPublicBbs( &p->u.kein.server_text_default ); break;
	case CMD_SEND_LEARN_ITEM_INDEX	:	RecvLearnedItem( &p->u.kein.server_learn_item ); break;
	case CMD_HUNTER_REGIST			:	if( p->u.kein.game2client_ear_result.ret )
											AddCurrentStatusMessage( 255, 255, 255, lan->OutputMessage(4,27)); //lsw
		else AddCurrentStatusMessage( 255, 0, 0, lan->OutputMessage(4,28) ); //lsw
		break;
		
	case CMD_SEND_HUNTER_LIST		:	RecvHunterList( &p->u.kein.hunter_list ); break;
	case CMD_KILL_HUNTER_LIST_CANCEL :	MP3( SN_WARNING ); AddCurrentStatusMessage( 200, 50, 50, lan->OutputMessage(4,29));  break;//lsw
	case CMD_KILL_HUNTER_LIST_OK	:	RecvKillHunterOk( &p->u.kein.kill_hunter_ok ); break;
		//		case CMD_HUNTER_REGIST_RESULT	:	RecvHunterRegistResult( p->u.kein.kill_who_whom.who_name, p->u.kein.kill_who_whom.whom_name ); break;
	case CMD_KILLED_BY_HUNTER		:	MP3( SN_WARNING ); AddCurrentStatusMessage( 250, 50, 50, lan->OutputMessage(4,30));  break;//lsw
		
	case CMD_KILL_WHO				:	RecvKillWho( p->u.kein.default_name ); break;
	case CMD_DANGER_OF_HUNTER		:	RecvDangerByHunter( p->u.kein.default_char ); break;
	case CMD_PARTY_NOT_BY_LEVEL		:	RecvPartyNotByLevel( p->u.kein.default_short_int ); break;
	case CMD_DO_PARTY_OK			:	RecvDoPartyOk( p->u.kein.do_party_ok.name, p->u.kein.do_party_ok.type ); break;
		
	case CMD_REQ_MAN_TO_MAN			:	RecvReqManToMan( p->u.kein.req_man_to_man.id ); break;
	case CMD_REQ_MAN_TO_MAN_RESULT	:	RecvReqManToManResult( p->u.kein.default_char ); break;
	case CMD_REQ_MAN_TO_MAN_CONSENT :	RecvStartManToMan( p->u.kein.default_short_int ); break;
		
	case CMD_REP_QUEST_INDEX		:	RecvQuestIndex( &p->u.kein.rep_quest_index ); break;
	case CMD_REQ_MAN_TO_MAN_REFUSAL	:	RecvReqManToManRefusal( ); break;
		
	case CMD_FIGHT_MAP_ADD_OK		:	RecvFightMapAddOk( p); break;
	case CMD_FIGHT_MAP_ADD_FULL		:	RecvFightMapAddNot( p); break;
		
	case CMD_FIGHT_MAP_START		:	RecvFightMapStart( ); break;
	case CMD_FIGHT_MAP_END			:	RecvFightMapEnd( ); break;
		
	case CMD_CHANGE_DURATION		:	RecvChangeDuration( p ); break;
	case CMD_THIS_MAN_IS_CANDIDATE_OK:	RecvCallRegistCandidate( ); break;		// 후보 등록 메뉴 띄우기
	case CMD_THIS_MAN_IS_CANDIDATE_NOT:	RecvCallVoteSupport( p ); break;		// 투표 메뉴 띄울지 등등의 경우 수 대비하기 
		
	case CMD_CHECK_VOTE_OK			:	RecvCheckVoteOk( p ); break;
	case CMD_CHECK_VOTE_NOT			:	RecvCheckVoteNot( p ); break;
		
	case CMD_REGIST_CANDIDATE_OK	:	CallOkCancelMessageBox( 0, 0, 0, lan->OutputMessage(4,31), 0 ); break;//lsw
	case CMD_REGIST_CANDIDATE_NOT	:	RecvRegistCandidateNot( ); break;
	case CMD_VOTE_NOT				:	AddCurrentStatusMessage( 240, 10, 10, lan->OutputMessage(4,32)); break;//lsw
	case CMD_VOTE_OK				:	AddCurrentStatusMessage( 100, 100, 240, lan->OutputMessage(4,33)); break;//lsw
	case CMD_RETRY					:	AddCurrentStatusMessage( 230, 230, 40, lan->OutputMessage(4,34)); break;//lsw
	case CMD_REP_VOTE_SCORE			:	RecvVoteScore( p ); break;		// 후보 득표 정보 가져오기
	case CMD_GET_SKILL_INFO			:	RecvGetSkillInfo( p ); break;
		
	case CMD_RECV_ALL_MSG			:	RecvNationMassage( p->u.kein.default_msg );break;
		
	case CMD_YOU_ARE_KING_LOG2CLIENT:	RecvYouAreKing( 1 ); break;		// 왕으로 만들어 준다.
	case CMD_YOU_ARE_NO_KING_LOG2CLIENT	: RecvYouAreKing( 0 ); break;		// 왕에서 해임한다.
		
	case CMD_GUILD_EXIT_YOU			:	RecvGuildExistYou( ); break;		// 길드에서 탈퇴당한 경우
	case CMD_GUILD_EXIT_OK			:	RecvGuildExistOk(  p ); break;			// 길드 탈퇴명령 성공
	case CMD_GUILD_EXIT_WHO			:	RecvGuildExistWho(  p ); break;			// 길드 탈퇴명령 성공
		
		
	case CMD_GUILD_JOIN_SERVER_OK	:	RecvGuildJoinOk( p ); break;
	case CMD_GUILD_JOIN_SERVER_NOT	:	RecvGuildJoinFail( p ); break;			// 가입 시키는 데 실패했다.
		
	case CMD_GUILD_JOIN_CONFIRM		:	RecvGuildJoinConfirm( p ); break;		// yes no 결정 필요
		
	case CMD_GUILD_JOIN_CONFIRM_NOT	:	AddCurrentStatusMessage( 200,60,60,lan->OutputMessage(4,35) ); break;//lsw
		
	case CMD_SCAN_TARGET			:	RecvScanTarget( p ); break;
	case CMD_GUILD_ALL_MSG_RECV		:	RecvGuildAllMsg( p ); break;
		
	case CMD_GUILD_EACH_LEVEL		:	RecvGuildEachLevel( p ); break;
		
	case CMD_GUILD_JOIN_CONFIRM_GUILMA : RecvGuildJoinConfirmIng( ); break;
		
	case CMD_BBS_ALL				:	RecvBbsAll( p ); break;
	case CMD_COME_OUT_GM_FROM_LOGIN	:	RecvRegsitGmWho( p ); break;
		
	case CMD_SCRIPT_MESSAGE_BOX		:	RecvScriptMessageBox( p ); break;
	case CMD_SEND_SOUND				:	MP3( p->u.kein.default_short_int ); break;
		
	case CMD_DB2GAME_SALVATION		:	RecvRepVationMoney( p ); break;
	case CMD_MSG_ALL_SALVATION_TOTAL_MONEY : RecvSalvationTotalMoneyMsg( p ); break;
	case CMD_CHANGE_MONEY_OK		:	RecvChangeMoney( p ); break;
		
		
		//		case CMD_ALL_TEAM_MEMBER_VIEW	:	RecvAllTeamInfo( p ); break;		// 팀배틀에 참여한 모든 사람의 정보를 보내준다.
		//		case CMD_ADD_TEAM_MEMBER_INFO	:	RecvAddMemberInfo( p ); break;		// 새로 추가된 놈의 정보
		
	case CMD_EAT_CHOCOLATE_OK		:	RecvEatChocolateOk( p ); break;
	case CMD_PAY_RUMOR_OK			:	RecvRumor( p ); break;
		
	case CMD_PAY_RUMOR_NOT			:	RecvRumorNot(); break;
		
	case CMD_TEAMBATTLE_INFO		:	RecvTEAMBATTLE_INFO(); break;
	case CMD_OPENSCHOOL_DATA_OK		:	RecvOpenSchoolResult( p ); break;
		
		
	case CMD_GUILD_EXIT_COMMAND_GLOBAL_OK :	MP3( SN_EAR_MESSAGE_RECV );
		AddCurrentStatusMessage( 250,250,60, kein_GetMenuString(7) );
		break;
	case CMD_GUILD_EXIT_COMMAND_GLOBAL_NOT:	MP3( SN_WARNING );
		AddCurrentStatusMessage( 250,60,60, kein_GetMenuString(6) );
		break;
		
	case CMD_EAT_CANDY_OK				:	MP3( SN_CONGRATULATIONS );
		AddCurrentStatusMessage( 250,255,0, kein_GetMenuString(8), p->u.kein.name2.name1, p->u.kein.name2.name2 );
		break;
		
	case CMD_REQ_LOAN_INFO				:	RecvLoanInfo( p ); break;
	case CMD_FIGHT_MAP_YOU_WIN			:	g_fight_win = -1;
		AddCurrentStatusMessage( 250,255,255, kein_GetMenuString(10) );
		break;
	case CMD_FIGHT_MAP_YOU_DEFEAT		:	g_fight_lose = -1;
		AddCurrentStatusMessage( 250,0,0, kein_GetMenuString(11) ); 
		break;
	case CMD_FIGHT_MAP_YOU_DISCONNECT	:	AddCurrentStatusMessage( 250,0,0, kein_GetMenuString(22) ); 
		break;
	case CMD_FIGHT_MAP_CHAR_INFO		:	RecvFightMapCharInfo( p ); break;
	case CMD_MERCHANT_BBS				:	RecvMerchantMessage( p ); break;
	case CMD_FIGHT_MAP_REMAIN_TIME		:	RecvFightRemainTime( p ); break;
	case CMD_FIGHT_MAP_ADD_FAIL_FIGHT_START	:	g_FightMapStart = 1; AddCurrentStatusMessage( 250,0,0, kein_GetMenuString(24) ); break;
	case CMD_ALIVE_FAIL_RESULT			:	RecvAliveFailResult( p ); break;
	case CMD_MERCHANT_ITEM_BBS			:	RecvMerchantItemBbs( p ); break;
	case CMD_REP_RANK_LADDER			:	RecvRankLadderData( p ); break;
	case CMD_REP_RANK_LADDER_FINISH		:	RecvRankLadderFinish( ); break;
		
	case CMD_REQ_SEARCH_RANK_LADDER_FAIL:	RecvSearchRankLadderFail(); break;
	case CMD_SV_SEND_MESSAGE_ALL		:	RecvtEffectByScript( p ); break;
	case CMD_REQ_VISA					:	RecvReqVisaItem( p ); break;
		
		
	case CMD_BUY_VISA_FAIL				:	g_VisaBuyFlag = 0;
		MP3( SN_WARNING );
		AddCurrentStatusMessage( 255,0,0 , kein_GetMenuString(37) );
	case CMD_BUY_VISA_OK				:	
		g_VisaBuyFlag = 0;
		MP3( SN_ITEM_BUY );
		break;
		
	case CMD_REGIST_GUILD_IMSI_OK		:	RecvWhoRegistGuildOk( p ); break;
		
	case CMD_GUILD_INFO_NAME			:	RecvGuildInfoName( p ); break;
	case CMD_GUILD_INFO_DEGREE			:	
	case CMD_GUILD_INFO_IMAGE			:	
		break;
	case CMD_GUILD_INFO_DELETED			:	RecvDeleteGuild( p ); break;
		break;
		
	case CMD_REQ_GUILD_EXPLAIN			:	RecvGuildExplain( p ); break;
	case CMD_GUILD_MARK_IMAGE			:	RecvGuildMarkImage( p ); break;
	case CMD_GUILD_DEGREE_INFO			:	RecvGuildDegreeInfo( p ); break;
	case CMD_REQ_GUILD_INTRODUCTION		:	RecvReqGuildIntroduction( 1, p ); break;
	case CMD_REQ_GUILD_INTRODUCTION_PLUS:	RecvReqGuildIntroduction( 0, p ); break;
		
	case CMD_GUILD_MASTER_AND_SUBMASTER	:	RecvGetGuildMasterName( p ); break;
		
	case CMD_PUT_MENU_STRING			:	RecvPutMenuString( p ); break;
	case CMD_REGIST_GUILD_IMSI_FAIL		:	RecvRegistGuildImsiFail( p ); break;
		
	case CMD_REGIST_GUILD_CHECK_OK		:	RecvRegistGuildCheckOk(); break;
		
		// 010924 YGI
	case CMD_REGIST_GUILD_CHECK_FAIL	:	//RecvRegistGuildCheckOk(); break;
		RecvRegistGuildCheckFail( p ); break;
		
	case CMD_CHECK_MY_GUILD				:	RecvCheckGuildCode( p ); break;
		
	case CMD_CHECK_SUB_MASTER_OK		:	RecvCheckSubMasterOk( p ); break;
	case CMD_CHECK_SUB_MASTER_FAIL		:	RecvCheckSubMasterFail( p ); break;
		
	case CMD_REGIST_GUILD_OK			:	RecvRegistGuildOk( p ); break;
	case CMD_REFRESH_GUILD_CODE			:	RecvRefreshGuild( p->u.kein.default_short_int ); break;
		
	case CMD_GUILD_CHAT					:	RecvGuildChatData( p ); break;
	case CMD_REQ_RESET_ABILITY			:	RecvResetAbility( p ); break;
		
	case CMD_GET_GHOST_QUEST_INFO		:	RecvGhostQuestInfo( p ); break;
	case CMD_CHANGE_COLOR				:	RecvChangeColor( p ); break;
	case CMD_SALVATION_RESULT_ADD_FAME	:	RecvSalvationAddFame( p ); break;
		
	case CMD_REP_NATION_MONEY			:	RecvRepNationMoney( p ); break;
	case CMD_CHECK_SALVATION_NAME		:	RecvCheckSalvationName( p ); break;
	case CMD_GAMBLE_ITEM_RESULT			:	RecvGambleItemResult( p ); break;
	case CMD_GAMBLE_START				:	RecvGambleStart( p ); break;
	case CMD_REPORTER_MODE				:	RecvReporterMode( p ); break;
	case CMD_GET_NAME_GUILD_MEMBER_LIST	:	RecvGuildMemberName( p ); break;
	case CMD_GET_GUILD_NOTICE			:	RecvGetGuildNotice( p ); break;
	case CMD_REGIST_FRIEND				:	RecvRegistFriendResult( p ); break;
		//		case CMD_GET_FRIEND					:	RecvRegistFriendName( p ); break;
	case CMD_ADD_FRIEND					:	RecvAddFriend( p ); break;
	case CMD_DELETE_FRIEND				:	Kein_PutMessage( KM_FAIL, kein_GetMenuString( 103 ) ); break;
		
	case CMD_GET_TODAY_GAMBLE_ITEM		:	RecvGetTodayGambleItem( p ); break;
	case CMD_TODAY_ITEM_GET_WHO			:	RecvTodyaItemGetWho( p ); break;
	case CMD_OX_QUESTION				:	RecvOXQuestion( p ); break;
	case CMD_OX_SET_TIME				:	RecvSetOXTime( p ); break;
		
	case CMD_GET_GUILD_ITEM_FIRST		:	RecvGuildItemFirst( p ); break;
	case CMD_GET_GUILD_ITEM				:	RecvGuildItem( p ); break;
	case CMD_GET_GUILD_BOX_MONEY		:	RecvGuildBoxMoney( p ); break;
	case CMD_GET_GUILD_ITEM_COUNT		:	RecvGuildItemCount( p ); break;
	case CMD_CHANGE_GUILD_MASTER		:	RecvChangeGuildDegree( p ); break;
		
	case CMD_CHECK_CHANGE_GUILD_INFO	:	RecvCheckGuildChange( p ); break;
	case CMD_GET_NATION_ITEM			:	RecvGetNationItem( p ); break;
	case CMD_BUY_NATION_ITEM_OK			:	RecvBuyNationItemOk( p ); break;
	case CMD_CHECK_COLOSSUS_MAP_REPLY	:	RecvCheckColossusMapReply( p ); break;
	case CMD_INVITE_COLOSSUS_REPLY		:	RecvInviteColossusReply( p ); break;
	case CMD_ATTACK_COLOSSUS_STONE_RESULT	:	RecvAttackColossusStoneResult( p ); break;
	case CMD_CALL_MAIN_MENU					:	RecvCallMainMenu( p ); break;
	case CMD_GUILD_HOUSE_INFO			:	RecvGuildHouseInfo( p ); break;
		
	case CMD_PK_ON_OFF	:					RecvSetPkOnOff( p ); break;
	case CMD_GET_PK_ON_OFF	:				RecvGetPkOnOff( p ); break;
		
	case CMD_FAIL_ATTACK_BY_PK_MODE		:	Kein_PutMessage( KM_FAIL, kein_GetMenuString( 171 ) ); break;
		
		// acer4
	case CMD_SEACH_OTHER_CHAR			:	RecvSeachOtherChar( p ); break;
		
		// 020620 YGI
	case CMD_SETTING_MAP_TILE_EVENT		:	RecvSettingMapTile( p ); break;
	case CMD_DELETE_MAP_TILE_EVENT		:	RecvDeleteMapTileEvent( p ); break;
		
	case CMD_GET_SCENARIO_INFO			:	RecvGetScenarioInfo( p ); break;
	case CMD_SCENARIO_MESSAGE			:	RecvScenarioMessage( p ); break;
	case CMD_TRAP_INFO					:	RecvScenarioTrapInfo( p ); break;
	case CMD_SCENARIO_TRAP_KEY			:	RecvScenarioTrapKey(p); break;	
	case CMD_SCENARIO_TIME_INFO			:	RecvGetScenarioTime(p); break;	// CSD-030306
		//case CMD_MOVEMAP_FOR_LIGHT_VERSION	:	RecvMoveMapForLightVersion( p ); break;	// 021008 YGI
		// 021117 YGI
	case CMD_CHECK_EVENT_MOVE_MAP		:	RecvCheckEventMoveMap( p ); break;
	// 040105 YGI
	case CMD_SEND_EFFECT :
		{	//< CSD-030730
			extern void RecvCMD_SEND_EFFECT( t_packet *p );
			RecvCMD_SEND_EFFECT(p);
			break;
		}	//> CSD-030730		
	case CMD_MANTLE_DYE: RecvMantleDye(p); break;
	case CMD_EXILE_NOT					:	RecvApplicationOfExileResult( p ); break; //Eleval 12/06/09
	case CMD_EXILE_OK					:	RecvApplicationOfExileResult( p ); break; //Eleval 12/06/09
	case CMD_SEX_CHANGE_OK				:	RecvSexChangeResult( p ); break;	//Eleval - Sex change
	case CMD_TOOL_PLAY_SOUND			:	RecvToolPlaySound( p ); break; // Eleval 19/06/09 - Tool Play Sound
	case CMD_CHAR_OPTIONS_CHANGE		:	RecvCharOptionsChange( p ); break; //Eleval 18/08/09 - To change a char's options
	case CMD_TELEPORT_OTHER_CHAR		:	RecvTeleportToCharResult(p); break; //Eleval 19/08/09 - Tool to teleport to a char (result)
	case CMD_REQ_HONOR_AMOUNT			:	RecvReqHonorAmount(p); break; //Eleval 07/09/09 - To request the player's current honor
	case CMD_PRISON_SYS_PROC_RESULT:
	{
		g_cPrison.RecvPrisonSystemResult(p);
	}
	break;
	default	:
		{
			return( -31 );
		}
	}
	return 1;
}