#include "stdafx.h"
#include <ddraw.h>
#include "Hangul.h"
#include "dragon.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "SmallMenuSet.h"
#include "stdio.h"
#include "directsound.h"
#include "directinput.h"
#include "Char.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "skill.h"
#include "MenuNetwork.h"
#include "MouseCursor.h"
#include "CharDataTable.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "skill_lsw.h"
#include "menudef.h"
#include "menudef2.h"
#include "TextOutMgr.h"
#include "Auction.h"
#include "item.h"
#define DIVIDE_ITEM_UPGRDEABLE_MIN 2000
#define DIVIDE_ITEM_UPGRDEABLE_MAX 5000

//#define LIGHT_GM_CLIENT //Eleval 01/06/09

t_SkillMakeItemInfo	skillmakeiteminfo;
extern DWORD g_ClientTimeNoGap;
extern void ConvertSpecialWord(char *txt,const char *delTarget,const char AfterConvert);
t_CombatSkill CombatSkill[MAX_COMBAT_SKILL_STRUCT] = {0,};//전투 스킬 Doning 메뉴에서 쓰는것// 0,0 -> 사용할 스킬 번호 1,2,3->그룹 0 자리는 안쓴다 안에는 스킬 레벨만 저장 된다
t_CombatSkillExplain CombatSkillExplain[MAX_COMBAT_SKILL_EXPLAIN];//전투스킬 설명용
t_CombatSkillGroupExplain CombatSkillGroupExplain[MAX_COMBAT_SKILL_GROUP_EXPLAIN];//전투스킬 설명용
int gm_make_item_result_needitem[2];
MakeItem gm_make_item_list[MAX_UPGRADE_RARE]; //GM이 만들 아이템의 리스트
MakeItem gm_make_item_result;	//GM이 만들 결과물

//MakeItem aMRMakeSrc[3];

extern int iGM_Makeable_Item_List[300];
t_raregroup			RareGroup[5][MAX_RARE_GROUP];//0 은 레어그룹// 1은 펑션 그룹
t_ItemFunction		ItemFunction[MAX_ITEM_FUNCTION];
t_rareprobability	RareProbability[MAX_RARE_PROBABILITY];
t_ItemLegendSub     LegendSubBonus[MAX_ITEM_LEGEND_SUB];
ITEM_LEGEND			g_aItemLegend[MAX_ITEM_LEGEND];//021113 lsw
ITEM_LEGEND_BONUS	g_aLegendBonus[MAX_ITEM_LEGEND_BONUS];

bool CheckBy2ndGM_This_Item_Upgradeable( ItemAttr ia ,const int iType)//내가 이 아이템을 업그레이드 시킬수 있는가위해서 이걸 만들수 있나를 본다
{
	RareMain gmattr;
	CItem *t;
	t = ItemUnit(ia.item_no);
	const short int item_no   =  ia.item_no;

	if(!t)//잘못된 아이템이라면..
	{
		return false;
	}

	if(!item_no) {return false;}//아이템 넘버가 없으면

	const RareMain *pInRare = (RareMain*)(&ia.attr[IATTR_RARE_MAIN]);
	if(pInRare->soksung2 || pInRare->soksung3)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,880) );
		return false;
	}

	const int iUpgradeType = ItemMutantKind[t->GetItemKind()].upgrade_type - 1;//020701 lsw -1 헤서 참조 하라
	if(0 > iUpgradeType ) {return false;}//업글 안되는건데 ㅡㅡ;;
	
	if(DIVIDE_ITEM == t->GetRButton())//나누어 진다면 갯수 체크를 해야 한다.
	{
		if(ia.attr[IATTR_MUCH]<DIVIDE_ITEM_UPGRDEABLE_MIN || ia.attr[IATTR_MUCH]>DIVIDE_ITEM_UPGRDEABLE_MAX )
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,266),DIVIDE_ITEM_UPGRDEABLE_MIN,DIVIDE_ITEM_UPGRDEABLE_MAX);
			return false;
		}
	}

	if ( !(ia.attr[IATTR_RARE_MAIN]) )//속성이 없으면 업그레이드 불가
	{	
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,511));
		return false;
	}
	
	memcpy(&gmattr,&ia.attr[IATTR_RARE_MAIN],sizeof(int));

	if( gmattr.grade > 16)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,512));
		return false;
	}

	if(iType)//기성의 멀티레어 만들기 제한 
	{
		if( !ItemGrade[gmattr.grade].iAble)
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,512));
			return false;
		}
	}
	else//GM의 업그레이드 제한
	{
		if( !ItemGrade[gmattr.grade+1].iAble)
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,512));
			return false;
		}
	}
	if( gmattr.iHighLevel)//021119 lsw
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,519));//레어아이템이 아닙니다
		return false;
	}

	for( int i=0; i<SCharacterData.ItemIdCount; i++ )
	{
		if( item_no == iGM_Makeable_Item_List[i] )
		{
			gm_make_item_result.item_no		= HandItemAttr.item_no;
			gm_make_item_result.attr		= HandItemAttr;

			gm_make_item_result_needitem[0]  =  ItemMutantKind[GetItemAttr( item_no, ITEM_KIND)].AddItem[0]/100;//020505 lsw
			gm_make_item_result_needitem[1]  =  ItemMutantKind[GetItemAttr( item_no, ITEM_KIND)].AddItem[1]/100;
			return true;//업그레이드에 적합한 자신의 스킬에 맞는 아이템
		}
	}
	AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,513));
	return false;
}

const int MAX_CORRECT_RARE = 5;
const int Convert[2][MAX_CORRECT_RARE]=
{
	{RARE_POWER,		RARE_POWER,		RARE_POWER,		RARE_VIGOR,			RARE_POWER},
	{RARE_VIGOR,		RARE_VIGOR,		RARE_VIGOR,		RARE_PIERCING,		RARE_PIERCING}
};//020823 lsw
bool CorrectRareKindJustKind(const int iResultSok, const int iTargetSok)
{
	if(iResultSok == iTargetSok)
	{
		return true;
	}
	else
	{
		for(int i = 0; i < MAX_CORRECT_RARE; i++)
		{
			if(	Convert [0][i] == iResultSok)
			{
				if(	Convert [1][i] == iTargetSok)
				{
					return true;
				}
			}
			else if(	Convert [1][i] == iResultSok)
			{
				if(	Convert [0][i] == iTargetSok)
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool CorrectRareKind(const int iResultAttr,const int iCompareAttr)
{
	RareMain *result = ((LPRareMain)(&iResultAttr));
	RareMain *compare = ((LPRareMain)(&iCompareAttr));

	const int iResultSok = result->soksung1;
	const int iTargetSok = compare->soksung1;
	
	if( result->grade != compare->grade){return false;}
	if( result->iHighLevel != compare->iHighLevel){return false;}
	if( result->soksung2 != compare->soksung2){return false;}
	if( result->soksung3 != compare->soksung3){return false;}
	if( result->IsDynamicRare != compare->IsDynamicRare){return false;}

	for(int i = 0; i < MAX_CORRECT_RARE; i++)
	{
		if(	Convert [0][i] == iResultSok)
		{
			if(	Convert [1][i] == iTargetSok)
			{
				return true;
			}
		}
		else if(	Convert [1][i] == iResultSok)
		{
			if(	Convert [0][i] == iTargetSok)
			{
				return true;
			}
		}
	}
	return false;
}

bool CheckBy2ndGM_Insert_to_GMItemList(ItemAttr ia, const int iType)
{
	const int item_no = gm_make_item_result.item_no;//업그레이드 부가 아이템일 경우

	CItem *t;
	t = ItemUnit(ia.item_no);

	if(!t)
	{
		return false;
	}

	if( !ia.item_no )	
	{	AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,514) );
		return false;
	}

	const RareMain *pInRare = (RareMain*)(&ia.attr[IATTR_RARE_MAIN]);
	if(pInRare->soksung2 || pInRare->soksung3)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,880) );
		return false;
	}

	if(DIVIDE_ITEM == t->GetRButton())
	{
		if(ia.attr[IATTR_MUCH]<DIVIDE_ITEM_UPGRDEABLE_MIN || ia.attr[IATTR_MUCH]>DIVIDE_ITEM_UPGRDEABLE_MAX )
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,266),DIVIDE_ITEM_UPGRDEABLE_MIN,DIVIDE_ITEM_UPGRDEABLE_MAX);
			return false;
		}
	}

	if(iType)//기성의 업글
	{
		if( !ia.attr[IATTR_RARE_MAIN] )	//속성이 없냐
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,516));
			return false;
		}

		for(int ct = 0; ct <2; ct++)//아이템 자리가 같다( Pos 체크 )
		{
			if(!memcmp(&g_MultiRareSrc.MultiRareSrc[ct].pos , &IP_base,sizeof(POS)))
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,518));
				return false;	
			}
			
			const RareMain *pRare = (RareMain*)(&g_MultiRareSrc.MultiRareSrc[ct].attr.attr[IATTR_RARE_MAIN] );
			
			if(pInRare->soksung1)
			{
				if(MAX_DOUBLE_RARE_GRADE < pInRare->grade + pRare->grade)//021212 lsw
				{
					AddCurrentStatusMessage(FONT_COLOR_MAX, lan->OutputMessage(2,881));
					return false;
				}

				if(pRare->soksung1 == pInRare->soksung1)
				{
					AddCurrentStatusMessage(FONT_COLOR_MAX, lan->OutputMessage(2,890));
					return false;
				}
				else//
				{
					if(CorrectRareKindJustKind(pRare->soksung1, pInRare->soksung1))//다른데도 불구하도 같은건가
					{
						return false;
					}
				}
			}
			else
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,511));//레어가 아니잖아
				return false;
			}
			const int iTargetNo = g_MultiRareSrc.MultiRareSrc[ct].attr.item_no;
			if(iTargetNo)
			{
				if(ia.item_no != iTargetNo)
				{
					AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,515));
					return false;
				}
			}
		}
		return true;
	}
	else //지엠의 업글 
	{
		if(ItemMutantKind[GetItemAttr( item_no, ITEM_KIND)].AddItem[0]/100 == ia.item_no)//필요 아이템 1번 이냐?
		{
			return true;
		}

		if(ItemMutantKind[GetItemAttr( item_no, ITEM_KIND)].AddItem[1]/100 == ia.item_no)//필요 아이템 2번 이냐?
		{
			return true;
		}
	
		if( !ia.attr[IATTR_RARE_MAIN] )	//속성이 없냐
		{
			AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,516));
			return false;
		}

		for(int ct = 0; ct <20; ct++)//아이템 자리가 같다( Pos 체크 )
		{
			if(!memcmp(&gm_make_item_list[ct].pos , &IP_base,sizeof(POS)))
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,518));
				return false;	
			}
		}
		// 기능 아이템과 아닌 아이템을 구분 하자.
		if(((RareMain*)&ia.attr[IATTR_RARE_MAIN])->IsDynamicRare)//이 if문이 함수의 마지막을 장식해야 합니다.
		{	//기능 아이템 부분
			if(gm_make_item_result.attr.item_no != ia.item_no)
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,515));
				return false;
			}

			if( gm_make_item_result.attr.attr[IATTR_RARE_MAIN] == ia.attr[IATTR_RARE_MAIN] )//속성이 같다..
			{
				
			}
			else//멀티 기능이 된다
			{//속성 다른거네
				return false;
			}
		}
		else
		{
			if(gm_make_item_result.attr.item_no != ia.item_no)
			{
				AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,515));
				return false;
			}
			if( gm_make_item_result.attr.attr[IATTR_RARE_MAIN] != ia.attr[IATTR_RARE_MAIN] )//속성이 다르다..
			{
				if(CorrectRareKind(gm_make_item_result.attr.attr[IATTR_RARE_MAIN],ia.attr[IATTR_RARE_MAIN]))
				{
					//true로 리턴되면 괜찮음
				}
				else
				{
					AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,517));
					return false;
				}
			}
		}
		return true;
	}
	return false;
}
//010708 lsw
int Call_GM_2ND_Skill_Menu()
{
	CloseAllMenu();

	fRMoveMenu = MM_ITEM_OPEN;		// 위의 if문을 만족할 때만
	nItemOther = MN_GM_2ND_SKILL;
	y_MenuFocus = 0;

	ZeroMemory( &gm_make_item_result, sizeof(MakeItem) );	//리스트 초기화
	ZeroMemory(  gm_make_item_list	, sizeof(MakeItem[MAX_UPGRADE_RARE]) );
	return 1;
}

int Call_Skill_Master_Menu()
{
	CloseAllMenu();

	fRMoveMenu = MM_ITEM_OPEN;		// 위의 if문을 만족할 때만
	nItemOther = MN_SKILL_MASTER_MAKE;
	y_MenuFocus = 0;
	
	ZeroMemory( &g_MultiRareSrc, sizeof(MULTIRARESRC) );	//리스트 초기화
	return 1;
}

int Call_MERCHANT_QUEST_MENU(const int iGmQuestIndex)
{
	::SetGMQUESTMenu(iGmQuestIndex);	
	::CallSmallMenu(NM_MERCHANT_QUEST);
	return 0;
}

int Call_EXCHANGE_BOND_MONEY_Menu()//021126 lsw
{
	Auction.ClearBondItem();
	CloseAllMenu();	
	fRMoveMenu = MM_ITEM_OPEN;
	nItemOther = MN_EXCHANGE_BOND_MONEY;
	y_MenuFocus = 0;
	SetChatMode(CM_COMMAND);
	return 1;
}

int Call_MERCHANT_REGISTER_Menu()
{
	Auction.ClearSellItem();
	CloseAllMenu();	
	fRMoveMenu = MM_ITEM_OPEN;
	nItemOther = MN_MERCHANT_REGISTER;
	y_MenuFocus = 0;
	//판매 품목 클리어 준비
	SetChatMode(CM_COMMAND);
	SetFocus2(HWND_1);
	return 1;
}

void Send2NDGMMAKE(int skillno)
{
	ItemAttr *t = ::GetItemByPos(	gm_make_item_result.pos );
//	if(!t->item_no) return;
	if( !gm_make_item_result.item_no)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,504));
		return;
	}

	t_packet p;	
	p.h.header.type = CMD_SKILL_RARE_UPGRADE;
	p.h.header.size = sizeof( t_SkillRareUpgrade );
	p.u.SkillRareUpgrade.iSkillNo		= skillno;
	p.u.SkillRareUpgrade.SourceItem		= gm_make_item_result.attr;

	for (int ti = 0; ti <MAX_UPGRADE_RARE ; ti++)
	{
		p.u.SkillRareUpgrade.pos[ti] = gm_make_item_list[ti].pos;
		
		DeleteItem( gm_make_item_list[ti].pos ) ;//공구창의 재료 없에기
		memset( &gm_make_item_list[ti], 0, sizeof( MakeItem ) );
	}

	QueuePacket( &p, 1 );

	memset( &gm_make_item_result, 0, sizeof( MakeItem ) );
	return;
}

void SendSkillMasterMake(int skillno)
{
	ItemAttr *t = ::GetItemByPos(	gm_make_item_result.pos );
	if( !gm_make_item_result.item_no)
	{
		AddCurrentStatusMessage(FONT_COLOR_MAX,lan->OutputMessage(2,504));
		return;
	}

	t_packet p;	
	p.h.header.type = CMD_SKILL_RARE_UPGRADE;
	p.h.header.size = sizeof( t_SkillRareUpgrade );
	p.u.SkillRareUpgrade.iSkillNo		= skillno;
	p.u.SkillRareUpgrade.SourceItem		= gm_make_item_result.attr;

	for (int ti = 0; ti <MAX_UPGRADE_RARE ; ti++)
	{
		p.u.SkillRareUpgrade.pos[ti] = gm_make_item_list[ti].pos;
		
		DeleteItem( gm_make_item_list[ti].pos ) ;//공구창의 재료 없에기
		memset( &gm_make_item_list[ti], 0, sizeof( MakeItem ) );
	}

	QueuePacket( &p, 1 );

	memset( &gm_make_item_result, 0, sizeof( MakeItem ) );
	return;
}

void Recv2NDGMMAKEResult(t_SkillRareUpgradeResult *p)
{
	SkillStatus	= SKILL_OFF;
	
	p->SourceItem;//이거를 이용 해서 연출에 보너스를 주자
	//인터 페이스 연출가능 하도록 세팅
	SMenu[MN_GM_2ND_SKILL].nField[4].nWillDo = p->iSuccess;
	
	SkillStatus				= SKILL_OFF;
	nItemOther = 0 ;
	SkillSettingClear( );
	//011030 lsw
	// SmallMenuClose(); 버튼 누르면 닫히게 하기
	return;
}

//011030 lsw >
DWORD g_skillmaketime = 0;
//011030 lsw <

void Send2NDSKILLMAKE(int iTryRare, int iRareGroup)
{
	//만들 수량값과 대상 아이템을 보낸다
	POS s;
	
	memset(&s,0,sizeof(POS));
	
	s.p1 = iTryRare;
	s.p2 = iRareGroup;//rared 
	s.p3 = skillmakeiteminfo.MakeMuch;
	
	g_skillmaketime = (timeGetTime()+skillmakeiteminfo.MakeMuch*1500);
	
	SendSkillInfo( SkillNo, Mox, Moy, SKILLTARGETTYPE_TABLE_, item_select_result.item_no, s);
	//011025 >
	skillmakeiteminfo.MakeMuch = 0;
	SMenu[MN_ITEM_MAKE_SELECT].nField[10].nWillDo	=0;
	CloseAllMenu();
	//011025 <
	return;
}

inline void ReadRareGroup(char * buf,int i)
{
	char *token;

	token = strtok(buf, "\t\n\r");	if( token == NULL ) return;const int iIndex = RareGroup[0][i].no				= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(RareGroup[0][iIndex].name		,token,39);
	EatRearWhiteChar( RareGroup[0][iIndex].name );
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].rareset[0].rare,token,	39);
	EatRearWhiteChar( RareGroup[0][iIndex].rareset[0].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[0].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[0].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].rareset[1].rare	,token,	39);
	EatRearWhiteChar( RareGroup[0][iIndex].rareset[1].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;RareGroup[0][iIndex].rareset[1].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[1].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].rareset[2].rare	,token,	39);
	EatRearWhiteChar( RareGroup[0][iIndex].rareset[2].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[2].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[2].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(RareGroup[0][iIndex].rareset[3].rare	,token,	39);		
	EatRearWhiteChar( RareGroup[0][iIndex].rareset[3].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[3].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[3].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].rareset[4].rare	,token,	39);	
	EatRearWhiteChar( RareGroup[0][iIndex].rareset[4].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[4].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;RareGroup[0][iIndex].rareset[4].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].rareset[5].rare	,token,	39);	
	EatRearWhiteChar( RareGroup[0][iIndex].rareset[5].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[5].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[5].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].rareset[6].rare	,token,	39);	
	EatRearWhiteChar( RareGroup[0][iIndex].rareset[6].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;RareGroup[0][iIndex].rareset[6].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[6].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].rareset[7].rare	,token,	39);	
	EatRearWhiteChar( RareGroup[0][iIndex].rareset[7].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[7].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].rareset[7].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(RareGroup[0][iIndex].group_buffer_1	,token,	39);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].group_buffer_2	,token,	39);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[0][iIndex].group_buffer_3	,token,	39);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].group_buffer_4		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].group_buffer_5		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;RareGroup[0][iIndex].group_buffer_6		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].group_buffer_7		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].group_buffer_8		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].group_buffer_9		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[0][iIndex].group_buffer_10	= atoi(token);	
 
	return;
}

bool LoadRareGroup()
{
	char buf[1024];
	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	sprintf(filename, ".\\itemtable\\rare_group.txt");
	sprintf(filenameBin, ".\\itemtable\\rare_group.bin");
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_RARE_GROUP-1 ) break;
				ReadRareGroup(buf, i);
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( RareGroup[0], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fclose(fp);
		}
	 }
	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( RareGroup[0], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

inline void ReadRareProbability(char * buf,int i)
{
	char *token;
	int *TargetPointer;

	TargetPointer = &RareProbability[i].num;

	token=strtok(buf ,"\t\n\r");if( token == NULL ) return;	
	(*TargetPointer) = atoi(token);
		 TargetPointer++;

	for (int j  = 0 ; j < 18 ; j++)//맥스 갯수에서 처음꺼 뺸 개수
	{
		 token=strtok(NULL ,"\t\n\r");if( token == NULL ) return;	
		 (*TargetPointer) = atoi(token);
		 TargetPointer++;
	}

	return;
}

bool LoadRareProbability()
{
	char buf[1024] ;
	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	sprintf(filename, ".\\itemtable\\rare_probability.txt");
	sprintf(filenameBin, ".\\itemtable\\rare_probability.bin");
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_RARE_PROBABILITY-1 ) break;
				ReadRareProbability(buf, i);
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( RareProbability, sizeof(t_rareprobability)*MAX_RARE_PROBABILITY, 1, fp);
			fclose(fp);
		}
	 }
	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( RareProbability, sizeof(t_rareprobability)*MAX_RARE_PROBABILITY, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

void RecvCMD_RARE_UPGRADE_BBS(t_packet *p)
{
//#ifdef	KOREA_LOCALIZING_
	ItemAttr item;
	char name[20];
	memcpy ( name,p->u.RareUpgradeBBS.name,20);
	item = p->u.RareUpgradeBBS.Item;
	RareMain Soksung;
	memcpy(&Soksung,&item.attr[IATTR_RARE_MAIN],4);
	//011025 lsw >
	char itemname[300];
	
	strcpy(itemname,GetItemName(item.item_no));
	
	EatRearWhiteChar(itemname);
	//011030 lsw >
	MP3(67);
	if(!Soksung.IsDynamicRare)//020626 lsw
	{
		AddCurrentStatusMessage(FONT_COLOR_RARE_MAIN,lan->OutputMessage(2,503),name,ItemRare[Soksung.soksung1].LocalName,itemname,Soksung.grade);
	}
	else
	{
		AddCurrentStatusMessage(FONT_COLOR_RARE_MAIN,lan->OutputMessage(2,503),name,ItemFunction[Soksung.soksung1].NameAdd,itemname,Soksung.grade);
	}
	//011030 lsw <
//#endif
	return;
}

int GetSkillResorceAmount(int &r1count ,int &r2count ,int &r3count ,int &r4count)
{
	int join_no = item_select_result.join_no;		// 만들고자 하는 아이템이 몇번째 아이템인가...

	CItem  *rit1= ItemUnit(item_join[join_no].resource1);;
	CItem  *rit2= ItemUnit(item_join[join_no].resource2);;
	CItem  *rit3= ItemUnit(item_join[join_no].resource3);;
	CItem  *rit4= ItemUnit(item_join[join_no].resource4);;

	int r1amount= item_join[join_no].resource1_amount;	
	int r2amount= item_join[join_no].resource2_amount;	
	int r3amount= item_join[join_no].resource3_amount;	
	int r4amount= item_join[join_no].resource4_amount;	

	int r1= -1;
	int r2= -1;
	int r3= -1;
	int r4= -1;

	int jj=0;
	int kk=0;
	int ii=0;

	ItemAttr Item ; 
	const int ino1 = item_join[join_no].resource1;
	const int ino2 = item_join[join_no].resource2;
	const int ino3 = item_join[join_no].resource3;
	const int ino4 = item_join[join_no].resource4;

	for(jj=0;jj<3;jj++)
	{
		for(kk=0;kk<3;kk++) 
		{
			for(ii=0;ii<8;ii++)  //인벤토리창의 세로 최대 크기는 3
			{   
				Item = InvItemAttr[jj][kk][ii];
				if( rit1 )
				{
					if(ino1 == Item.item_no)
					{
						if(DIVIDE_ITEM == rit1->GetRButton()){r1count += Item.attr[IATTR_MUCH];}
						else{r1count++;}
					}

				}
				if( rit2 )
				{
					if(ino2 == Item.item_no)
					{
						if(DIVIDE_ITEM == rit2->GetRButton()){r2count += Item.attr[IATTR_MUCH];}
						else{r2count++;}
					}
				}
				if( rit3 )
				{
					if(ino3 == Item.item_no)
					{
						if(DIVIDE_ITEM==rit3->GetRButton()){r3count += Item.attr[IATTR_MUCH];}
						else{r3count++;}
					}
				}
				if( rit4 )
				{
					if(ino4 == Item.item_no)
					{
						if(DIVIDE_ITEM==rit4->GetRButton()){r4count += Item.attr[IATTR_MUCH];}
						else{r4count++;}
					}
				}
			}
		}
	}
	return 0;
}

void RecvCMD_ADD_EFFECT(t_packet *p)
{
	const	WORD	wEffectNo		=p->u.AddEffect.wEffectNo;	// 사용된 마법의 번호
	const	WORD	idTarget		=p->u.AddEffect.idTarget;	// 마법을 사용한 대상자
	const	DWORD	dwDuration		=p->u.AddEffect.wDuration*1000;	// 유지시간
	LPCHARACTER	ch = ReturnCharacterPoint(idTarget);
	if(ch)
	{
		InsertMagic(ch,ch,wEffectNo,0,ch->x,ch->y,ch->x,ch->y,0,0,0,g_ClientTime+dwDuration);
	}
}

void ReadItemFunctionTable(char * buf,int i)
{
	char *token;

	token = strtok(buf, "\t\n\r");	if( token == NULL ) return; const int iIndex = ItemFunction[i].iNo			= atoi(token);	
	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(ItemFunction[iIndex].Name		,token,39);
	EatRearWhiteChar( ItemFunction[iIndex].Name );
	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(ItemFunction[iIndex].NameAdd	,token,39);
	EatRearWhiteChar( ItemFunction[iIndex].NameAdd );

	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(ItemFunction[iIndex].Exp		,token,49);
	EatRearWhiteChar( ItemFunction[iIndex].Exp );
	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iExpMark	= atoi(token);	//020530 lsw
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iEffectNo	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iUpgradeAble	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iScrollNo	= atoi(token);	
	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iMakeRand[0]	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iMakeRand[1]	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iMakeRand[2]	= atoi(token);	
	
	for(int x = 0; x<17;x++)
	{
		token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].GRADE[x]	= atoi(token);	
	}
	
	for(int y = 0; y<17;y++)
	{
		token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iTime[y]	= atoi(token);	
	}

	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iBuffer1		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;ItemFunction[iIndex].iBuffer2		= atoi(token);	

	return;
}

bool LoadItemFunctionTable()
{
	char buf[1024];
	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	sprintf(filename, ".\\itemtable\\ItemFunction.txt");
	sprintf(filenameBin, ".\\itemtable\\ItemFunction.bin");
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_ITEM_FUNCTION-1 ) break;
				ReadItemFunctionTable(buf, i);
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( ItemFunction, sizeof(t_ItemFunction)*MAX_ITEM_FUNCTION, 1, fp);
			fclose(fp);
		}
	 }
	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( ItemFunction, sizeof(t_ItemFunction)*MAX_ITEM_FUNCTION, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

bool CanViewGhost(CHARACTER *ch)
{
	if(ch->bCanViewGhost)
	{
		if( g_ClientTimeNoGap < ch->dwCanViewGhostTime)
		{
			return true;	
		}
	}
	return false;
}

bool	CanViewMagicTrap(CHARACTER *ch)
{
	if(ch->bCanViewMagicTrap)
	{
		if( g_ClientTimeNoGap < ch->dwCanViewMagicTrapTime )
		{
			return true;	
		}
	}
	return false;
}

bool	CanViewTrap(CHARACTER *ch)
{
	if(ch->bCanViewTrap)
	{
		if( g_ClientTimeNoGap < ch->dwCanViewTrapTime )
		{
			return true;
		}
	}
	return false;
}

bool	CanViewBomb(CHARACTER *ch)//Trap 0
{
	if( ch->bCanViewBomb )
	{
		if( g_ClientTimeNoGap < ch->dwCanViewBombTime)
		{
			return true;
		}
	}
	return false;
}

void InitHeroCanView()
{
	Hero->bCanViewGhost			= 0;
	Hero->bCanViewMagicTrap		= 0;
	Hero->bCanViewBomb			= 0;
	Hero->bCanViewTrap			= 0;
}

//광원 셋팅 하는 함수 
void SetDayLight(const short int nValue)
{
	DayLightControl = nValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// 전투 스킬
/////////////////////////////////////////////////////// 전투 스킬 Point

int GetCombatSkillIndexStart(const int iGroupNo,const int iGroupType)
{
	return (iGroupNo-1)*12 +(iGroupType-1)*6 +1;
}

int SkillIdToCombatSkillIconImage(const int iSkilNo,const bool bIsMain)
{
	if(!bIsMain)
	{
		return (iSkilNo-iCSCovertSkillID);//i는 스킬 넘버고 iCSCovertSkillID번 부터 시작이니까 iCSCovertSkillID를 제외한다 이미지 묶은거에 36 번부터 새로운거라서 +35 한다
	}
	
	return (iSkilNo-iCSCovertSkillID)+36;//i는 스킬 넘버고 iCSCovertSkillID번 부터 시작이니까 iCSCovertSkillID를 제외한다
}

int GetKnowCSGroupType(const int iGroupNo)//그룹넘버를 가지고 나의 그룹타입을 반환한다. 그룹이 아무것도 없으면 0을 반환
{
	switch(iGroupNo)
	{
		case 1:		case 2:		case 3:
			break;
		default:
			JustMsg("GetKnowCSGroupType에 그룹값이 이상합니다");
			break;
	}
	const int iGroup1Start	= iCSCovertSkillID+(iGroupNo-1)*12;//1번 그룹부터 시작하니까
	const int iGroup1End	= iGroup1Start+5;	
	const int iGroup2Start	= iGroup1End+1;	
	const int iGroup2End	= iGroup2Start +5;	
	int iGroupTypeSum1 = 0, iGroupTypeSum2 =0 ,i = 0;
	
	for(i = iGroup1Start; i <=iGroup1End; i++)
	{
		iGroupTypeSum1 += SCharacterData.SkillId[i];
	}
	for(i = iGroup2Start; i <=iGroup2End; i++)
	{
		iGroupTypeSum2 += SCharacterData.SkillId[i];
	}

	if( iGroupTypeSum1 > iGroupTypeSum2)
	{
		return 1;
	}
	if( iGroupTypeSum2 > iGroupTypeSum1)
	{
		return 2;
	}
	return 0;
}

void SetCSPButtonType(const int iSelectedGroupButton)//위에 3개 있는 그룹 버튼을 누르는거 그 버튼의 nWillDo 값이 버튼이 가진 그룹 값이다
{//부른 그룹을 가지고 내가 이 그룹타입을 선택 해야 하는가?, 이미 배워서 업그레이드 단계인가를 뽑아 버튼을 정리한다
//단지 버튼 활성화만 책임진다	
	switch(GetKnowCSGroupType(iSelectedGroupButton))
	{
	case 0://그룹 선택전
		{
			SMenu[MN_COMBAT_SKILL_POINT].nImageNumber	=	9;//이미지 넘버를 그룹타입선택 할때 이미지로
			//OK		01
			SMenu[MN_COMBAT_SKILL_POINT].nField[1].nType = FT_NONE;//비활성
			//DONE		03
			SMenu[MN_COMBAT_SKILL_POINT].nField[3].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//활성
			
			SMenu[MN_COMBAT_SKILL_POINT].nField[7].nType = FT_NONE;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[8].nType = FT_NONE;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[9].nType = FT_NONE;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[10].nType = FT_NONE;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[11].nType = FT_NONE;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[12].nType = FT_NONE;//활성

			//그룹 선택 스킬1	02	//nWillDo 값이 그룹 타입 1타입 2타입
			SMenu[MN_COMBAT_SKILL_POINT].nField[13].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;//활성
			//그룹 선택 스킬2	02
			SMenu[MN_COMBAT_SKILL_POINT].nField[14].nType = FT_SHOW_SPECIAL_WILLDO_AUTO_PUT;//활성
			//포인트 업			01//한글 프린팅 까지 하는 버튼을 만들고 Rect 를 이용 하여 1 필드로 처리
			SMenu[MN_COMBAT_SKILL_POINT].nField[15].nType = FT_NONE;//활성

			SMenu[MN_COMBAT_SKILL_POINT].nField[16].nType  = FT_CS_GROUP_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[17].nType  = FT_CS_GROUP_EXPLAIN;
			
			SMenu[MN_COMBAT_SKILL_POINT].nField[18].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[19].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[20].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[21].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[22].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[23].nType  = FT_CS_EXPLAIN;

			SMenu[MN_COMBAT_SKILL_POINT].nField[24].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[25].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[26].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[27].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[28].nType  = FT_CS_EXPLAIN;
			SMenu[MN_COMBAT_SKILL_POINT].nField[29].nType  = FT_CS_EXPLAIN;
			}break;
	default://그룹이 있다
		{
			SMenu[MN_COMBAT_SKILL_POINT].nImageNumber	=	0;//이미지 넘버를 그룹타입선택 할때 이미지로
			//OK		01
			SMenu[MN_COMBAT_SKILL_POINT].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//비활성
			//DONE		03
			SMenu[MN_COMBAT_SKILL_POINT].nField[3].nType = FT_NONE;//활성
			
			SMenu[MN_COMBAT_SKILL_POINT].nField[7].nType = FT_CS_ICON;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[8].nType = FT_CS_ICON;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[9].nType = FT_CS_ICON;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[10].nType = FT_CS_ICON;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[11].nType = FT_CS_ICON;//활성
			SMenu[MN_COMBAT_SKILL_POINT].nField[12].nType = FT_CS_ICON;//활성

			//그룹 선택 스킬1	02	//nWillDo 값이 그룹 타입 1타입 2타입
			SMenu[MN_COMBAT_SKILL_POINT].nField[13].nType = FT_NONE;//활성
			//그룹 선택 스킬2	02
			SMenu[MN_COMBAT_SKILL_POINT].nField[14].nType = FT_NONE;//활성
			//포인트 업			01//한글 프린팅 까지 하는 버튼을 만들고 Rect 를 이용 하여 1 필드로 처리
			SMenu[MN_COMBAT_SKILL_POINT].nField[15].nType = FT_CSP_POINT_UP;//활성

			SMenu[MN_COMBAT_SKILL_POINT].nField[16].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[17].nType  = FT_NONE;

			SMenu[MN_COMBAT_SKILL_POINT].nField[18].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[19].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[20].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[21].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[22].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[23].nType  = FT_NONE;

			SMenu[MN_COMBAT_SKILL_POINT].nField[24].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[25].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[26].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[27].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[28].nType  = FT_NONE;
			SMenu[MN_COMBAT_SKILL_POINT].nField[29].nType  = FT_NONE;
		}break;
	}
}

void SetCSPButtonTypeWhenSelectGroupType()
{//버튼을 셋팅 한다
	SMenu[MN_COMBAT_SKILL_POINT].nImageNumber	=	0;//이미지 넘버를 그룹타입선택 할때 이미지로
	//OK		01
	SMenu[MN_COMBAT_SKILL_POINT].nField[1].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//비활성
	//CANCEL	02
	SMenu[MN_COMBAT_SKILL_POINT].nField[2].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;//비활성
	//DONE		03
	SMenu[MN_COMBAT_SKILL_POINT].nField[3].nType = FT_NONE;//활성
	
	SMenu[MN_COMBAT_SKILL_POINT].nField[7].nType = FT_CS_ICON;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[8].nType = FT_CS_ICON;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[9].nType = FT_CS_ICON;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[10].nType = FT_CS_ICON;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[11].nType = FT_CS_ICON;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[12].nType = FT_CS_ICON;//활성

	//그룹 선택 스킬1	02	//nWillDo 값이 그룹 타입 1타입 2타입
	SMenu[MN_COMBAT_SKILL_POINT].nField[13].nType = FT_NONE;//활성
	//그룹 선택 스킬2	02
	SMenu[MN_COMBAT_SKILL_POINT].nField[14].nType = FT_NONE;//활성
	//포인트 업			01//한글 프린팅 까지 하는 버튼을 만들고 Rect 를 이용 하여 1 필드로 처리
	SMenu[MN_COMBAT_SKILL_POINT].nField[15].nType = FT_CSP_POINT_UP;//활성

	SMenu[MN_COMBAT_SKILL_POINT].nField[16].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[17].nType = FT_NONE;//활성
	
	SMenu[MN_COMBAT_SKILL_POINT].nField[18].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[19].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[20].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[21].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[22].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[23].nType = FT_NONE;//활성

	SMenu[MN_COMBAT_SKILL_POINT].nField[24].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[25].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[26].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[27].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[28].nType = FT_NONE;//활성
	SMenu[MN_COMBAT_SKILL_POINT].nField[29].nType = FT_NONE;//활성
}

void SetCSPGroupButtonImage(const int iSelected)//그룹 버튼 이미지 셋팅
{
	SFIELD *f = &SMenu[MN_COMBAT_SKILL_POINT].nField[4];
	switch(GetKnowCSGroupType(1))
	{ 
	case 1:
		{
			f->nImageNumber	=	12;
			f->nRectImage	=	11;
		}break;
	case 2:
		{
			f->nImageNumber	=	14;
			f->nRectImage	=	13;
		}break;
	case 0:
		{
			f->nImageNumber	=	24;
			f->nRectImage	=	23;
		}break;
	default:
		break;
	}
	f = &SMenu[MN_COMBAT_SKILL_POINT].nField[5];
	switch(GetKnowCSGroupType(2))
	{
	case 1:
		{
			f->nImageNumber	=	16;
			f->nRectImage	=	15;
		}break;
	case 2:
		{
			f->nImageNumber	=	18;
			f->nRectImage	=	17;
		}break;
	case 0:
		{
			f->nImageNumber	=	26;
			f->nRectImage	=	25;
		}break;
	default:
		break;
	}
	f = &SMenu[MN_COMBAT_SKILL_POINT].nField[6];
	switch(GetKnowCSGroupType(3))
	{
	case 1:
		{
			f->nImageNumber	=	20;
			f->nRectImage	=	19;
		}break;
	case 2:
		{
			f->nImageNumber	=	22;
			f->nRectImage	=	21;
		}break;
	case 0:
		{
			f->nImageNumber	=	28;
			f->nRectImage	=	27;
		}break;
	default:
		break;
	}
	
	switch(iSelected)//선택된것은 밝은 버튼으로
	{
	case 1:
		{
			f = &SMenu[MN_COMBAT_SKILL_POINT].nField[4];
			f->nImageNumber	=	f->nRectImage;
		}break;
	case 2:
		{
			f = &SMenu[MN_COMBAT_SKILL_POINT].nField[5];
			f->nImageNumber	=	f->nRectImage;
		}break;
	case 3:
		{
			f = &SMenu[MN_COMBAT_SKILL_POINT].nField[6];
			f->nImageNumber	=	f->nRectImage;
		}break;
	default:
		break;
	}
	//그룹 타입 버튼 그림 리셋
	SMenu[MN_COMBAT_SKILL_POINT].nField[13].nRectImage = 34;
	SMenu[MN_COMBAT_SKILL_POINT].nField[13].nImageNumber = 33;
	SMenu[MN_COMBAT_SKILL_POINT].nField[14].nRectImage = 35;
	SMenu[MN_COMBAT_SKILL_POINT].nField[14].nImageNumber = 32;
}

void SetCSPIconImageANDnWillDo(const int iGroupNo,const int iForceGroupType)//선택된 그룹에 따라 CSP의 버튼 그림과 nWillDo를 셋팅한다
{
	int iGroupType =0;
	if(iForceGroupType)
	{
		iGroupType = iForceGroupType;
	}
	else
	{
		 iGroupType = GetKnowCSGroupType(iGroupNo);
	}
	int iCombatSkillIndex = GetCombatSkillIndexStart(iGroupNo, iGroupType);
	const int iGroup1Start	= iCSCovertSkillID+iCombatSkillIndex-1;//1번 그룹부터 시작하니까
	const int iGroup1End = iGroup1Start+6;
	int iStartField		= 7;//SMenu[MN_COMBAT_SKILL_POINT].nField[7] 부터 SC아이콘 이기 때문이다
	int i = 0;
	for(i = iGroup1Start; i <iGroup1End; i++)
	{
		SFIELD *f = &SMenu[MN_COMBAT_SKILL_POINT].nField[iStartField];
		if(SCharacterData.SkillId[i])//아는거
		{
			f->nImageNumber = SkillIdToCombatSkillIconImage(i,true);//i는 스킬 넘버고 iCSCovertSkillID번 부터 시작이니까 iCSCovertSkillID를 제외한다
			f->nRectImage	= SkillIdToCombatSkillIconImage(i,false);//i는 스킬 넘버고 iCSCovertSkillID번 부터 시작이니까 iCSCovertSkillID를 제외한다 이미지 묶은거에 36 번부터 새로운거라서 +35 한다
		}
		else //모르는거
		{
			f->nImageNumber = 73+(iGroupNo-1)*2+(iGroupType-1);
			f->nRectImage	= SkillIdToCombatSkillIconImage(i,false);//i는 스킬 넘버고 iCSCovertSkillID번 부터 시작이니까 iCSCovertSkillID를 제외한다 이미지 묶은거에 36 번부터 새로운거라서 +35 한다
		}
		f->nWillDo = iCombatSkillIndex++;//t_CombatSkill의 인덱스 저장 1 부터 시작 0번은 Doing의 메인 아이콘 값
		iStartField++;
	}

	if( iGroupNo >0)
	{
		for(int xx = 0; xx < 12;xx++)
		{
			const int iStartCSIndex = ((iGroupNo-1) * 12 +1);
			SMenu[MN_COMBAT_SKILL_POINT].nField[18+xx].nWillDo	=iStartCSIndex+xx;//설명 부분에 올리는거 FT_CS_EXPLAIN 의nWillDo 셋팅
		}
	}
}

void CallCombatSkillPointMenu()
{
	if(!SMenu[MN_COMBAT_SKILL_POINT].bActive)
	{
		CloseAllMenu();
		SMenu[MN_COMBAT_SKILL_POINT].bActive = true;	
	}
	else
	{
		SMenu[MN_COMBAT_SKILL_POINT].bActive = false;
	}

	if(0 >= Hero->hp)
	{
		CloseAllMenu();
		return;
	}
	const int iDefaultGroup =1;
	InitCombatSkill();
	SMenu[MN_COMBAT_SKILL_POINT].work = iDefaultGroup;//work에 현재 선택된 그룹값이 들어있다
	SetCSPButtonType(iDefaultGroup);//위에 3개 있는 그룹 버튼을 누르는거 그 버튼의 nWillDo 값이 버튼이 가진 그룹 값이다
	SetCSPGroupButtonImage(iDefaultGroup);//그룹 버튼 이미지를 셋팅 한다//SkillId 를 가지고 뽑는거다
	SetCSPIconImageANDnWillDo(iDefaultGroup);//인터체이스에 뜨는 6개의 아이콘 버튼 이미지와 nWillDo 셋팅

//	CallSmallMenu(MN_COMBAT_SKILL_POINT);
}

int iCSReservPoint = 10;
//타입은 어디다 저장 해 두지?
void SendCSLvUp(const int iCombatSkillIndex)//iSkillNo(1~6)
{
	if( !iCombatSkillIndex ) 
	{
#ifdef _DEBUG // finito 060507
		if( GetSysInfo(SI_GAME_MAKE_MODE ) )		// 020925 YGI
			AddCurrentStatusMessage(255,255,255,"전투스킬 인덱스가 이상합니다");
#endif
		return;
	}
	
  if (iCSReservPoint > 0)
  { 
    g_mgrBattle.SendCombatLearn(iCombatSkillIndex + 214, Hero);
  } 
}

void InitCombatSkill()
{
  memset(CombatSkill,0,sizeof(t_CombatSkill)*MAX_COMBAT_SKILL_STRUCT);
	int iCSN = 1;
	for(int i = 1; i < 36+1 ; i++)
	{	//1-6	7-12		//13-18	19-24		//25-30 31-36
		CombatSkill[i].iGroup			=	(i-1)/12 +1		;			//전투 스킬 그룹
		CombatSkill[i].iGroupType		=	(i-1)%12 /6 +1		;		//전투 스킬 타입
		CombatSkill[i].iCombatSkillNo	=	iCSN++										;	//전투 스킬 넘버
		CombatSkill[i].iSkillNo			=	iCSCovertSkillID + i - 1				;		//SkillID 의 넘버
		CombatSkill[i].iSkillLv			=	SCharacterData.SkillId[CombatSkill[i].iSkillNo];		//전투 스킬 레벨//SCharacterData.SkilID[iSkillNo]하면 나오는 값
		if(6 < iCSN)
		{
			iCSN=1;
		}
	}

  iCSReservPoint = Hero->aStepInfo[CSP_STEP];
}

/////////////////////////////////////////////////////// 전투 스킬 Doing
int iCpMax = 100;
int iCpNow = 50;

int iCpCountMax = 100;
int iCpCountNow = 50;
bool SetCSDFlied(const int iGroupNo, const int iGroupType, const int iStartFlieddNo)
{
	//전투스킬 그룹 타입이 있다면. CombatSkill을 검사 하여(ex>1~^)있으면 넣고 없으면 컨티뉴 한다
	int iFNo = iStartFlieddNo;//시작필드 넘버 필드를 선형적으로 배치 할것
	int iCSIndex = GetCombatSkillIndexStart(iGroupNo, iGroupType);
	bool bForceNoneType = false;
	bool iReturnValue = false;//리턴되는 값. 필드 하나라도 셋팅 되면 다음 필드로 민다
	if( !iGroupType)
	{
		bForceNoneType = true;
	}
	const int iCSIndexTarget = iCSIndex+6;
	//클리어 올 필드 라는걸 집어 넣자.

	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+1].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+2].nWillDo = 0;

	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+3].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+4].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].nField[iStartFlieddNo+5].nWillDo = 0;
	
	for(; iCSIndex<iCSIndexTarget ;iCSIndex++)
	{
		SFIELD *f = &SMenu[MN_COMBAT_SKILL_DOING].nField[iFNo];
//    if(	iCSIndex != (iCSIndexTarget-1) 
//		|| 
//			(iGroupNo==3 && iGroupType ==2) )//5번이 아니면 그린다 3번 그룹에 2번 타입( 바람 이면 그린다 )
//		{
		  if(CombatSkill[iCSIndex].iSkillLv&&(bForceNoneType == 0))//배운거다.. 1렙 이상이니까
		  {
			  f->nImageNumber = SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,true);
			  f->nRectImage	= SkillIdToCombatSkillIconImage(CombatSkill[iCSIndex].iSkillNo,false);
			  SetRect(f->rCheakBox, f->x, f->y, f->x+42, f->y+42);
			  f->nWillDo = iCSIndex;
			  iFNo++;//다음 필드로
			  iReturnValue = true;
      }
//    }
		else//쓸수 있는 스킬 아니면 영역과 nWillDo를 0으로 셋팅
		{
			f->nWillDo = 0;
			SetRect(f->rCheakBox, 0, 0, 0, 0);
		}
	}
	return iReturnValue;
}

void SetCSDIconRectAndnWillDo()
{
	int iSkillId = iCSCovertSkillID;
	int iFieldNo = 17;//필드 넘버 밀기
	int iGroupNo = 1;

//	( SetCSDFlied(1,GetKnowCSGroupType(1),17) );
//	( SetCSDFlied(2,GetKnowCSGroupType(2),17) );
//	( SetCSDFlied(3,GetKnowCSGroupType(3),5) );

	for ( iGroupNo =1; iGroupNo<4;iGroupNo++)
	{
		if( SetCSDFlied(iGroupNo,GetKnowCSGroupType(iGroupNo),iFieldNo) )
		{
			iFieldNo-=6;
		}
	}
}

void SetSCDMainAndOtherClear(const int iCombatSkillIndex)
{	
	SMenu[MN_COMBAT_SKILL_DOING].nField[1].nWillDo = 0;
	SMenu[MN_COMBAT_SKILL_DOING].work = 0;//게이지가 출력 중이다
	for(int iFNo=5 ; iFNo <=22 ;iFNo++)//필드전부 클리어 (클릭 방지 등등)
	{
		SFIELD *f = &SMenu[MN_COMBAT_SKILL_DOING].nField[iFNo];
		f->nWillDo = 0;
		SetRect(f->rCheakBox, 0, 0, 0, 0);//
	}
	if(!iCombatSkillIndex)
	{
		return;
	}//스킬 인덱스가 이러면 안되지
	SMenu[MN_COMBAT_SKILL_DOING].nField[1].nWillDo = iCombatSkillIndex;//nWillDo 셋팅
	SMenu[MN_COMBAT_SKILL_DOING].work = 2;//게이지가 출력 중이다
}

bool IsSelectCombat()
{	//< CSD-TW-030606
	return (SMenu[MN_COMBAT_SKILL_DOING].nField[1].nWillDo == 0) ? false:true;
}	//> CSD-TW-030606

void CallCombatSkillDoingMenu()
{ //무조건 엽니다
	if(!SMenu[MN_COMBAT_SKILL_DOING].bActive)
	{
		SMenu[MN_COMBAT_SKILL_DOING].bActive = true;	
	}
  /*
	else
	{
		SMenu[MN_COMBAT_SKILL_DOING].bActive = true;
	}
	InitCombatSkill();
	SetCSDIconRectAndnWillDo();
	SetSCDMainAndOtherClear(0);
  */
}

int iCSPMax = 0;
int iCSPNow	= 0;

int iCSPCMax = 0;
int iCSPCNow = 0;

void CSDMainIconClear()
{	//< CSD-TW-030627
	g_mgrBattle.SendCombatClear();
}	//> CSD-TW-030627

inline void ReadCombatSkillExplain(char * buf,int i)
{	
	char *token;

	token = strtok(buf, "\t\n\r");	if( token == NULL ) return;
	const int iIndex = CombatSkillExplain[i].iSkillNo				= atoi(token);	
	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;
	strcpy(CombatSkillExplain[iIndex].EngName	,token);
//	CombatSkillExplain[iIndex].EngName[30] = 0;
	EatRearWhiteChar( CombatSkillExplain[iIndex].EngName);
	
	//token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;
	//strcpy(CombatSkillExplain[iIndex].LocalName,token);
	////CombatSkillExplain[iIndex].LocalName[30] = 0;
	//EatRearWhiteChar( CombatSkillExplain[iIndex].LocalName);
	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;//사용법
	strcpy(CombatSkillExplain[iIndex].ClassName,token);
//	CombatSkillExplain[iIndex].ClassName[50] = 0;
	EatRearWhiteChar( CombatSkillExplain[iIndex].ClassName);

	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;//사용법
	strcpy(CombatSkillExplain[iIndex].WeaponTactic,token);
//	CombatSkillExplain[iIndex].WeaponTactic[30] = 0;
	EatRearWhiteChar( CombatSkillExplain[iIndex].WeaponTactic);

	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillExplain[iIndex].iNeedLv[0]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillExplain[iIndex].iNeedLv[1]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillExplain[iIndex].iNeedLv[2]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillExplain[iIndex].iLvCp[0]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillExplain[iIndex].iLvCp[1]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillExplain[iIndex].iLvCp[2]		= atoi(token);	

	for(int xx = 0; xx < 3; xx++)
	{
		token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;//사용법
		strcpy(CombatSkillExplain[iIndex].MicroExplain[xx],token);
//		CombatSkillExplain[iIndex].MicroExplain[xx][50] = 0;
		EatRearWhiteChar( CombatSkillExplain[iIndex].MicroExplain[xx]);
	}
	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;//사용법
	strcpy(CombatSkillExplain[iIndex].Use,token);
//	CombatSkillExplain[iIndex].Use[50] = 0;
	EatRearWhiteChar( CombatSkillExplain[iIndex].Use);

	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;
	strcpy(CombatSkillExplain[iIndex].Explain,token);
//	CombatSkillExplain[iIndex].Explain[100] = 0;
	EatRearWhiteChar( CombatSkillExplain[iIndex].Explain);
	
	ConvertSpecialWord(CombatSkillExplain[iIndex].Explain,"\\n",'\n');
	ConvertSpecialWord(CombatSkillExplain[iIndex].Explain,"\\t",'\t');
	ConvertSpecialWord(CombatSkillExplain[iIndex].Explain,"\\r",'\r');
	
	return;
}

bool LoadCombatSkillExplain()
{
#ifdef _DEBUG	// 031110 YGI
	char buf[1024];
#endif

	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	sprintf(filename, ".\\data\\CombatSkillExplain.txt");
	sprintf(filenameBin, ".\\data\\CombatSkillExplain.bin");

#ifdef _DEBUG	// 031110 YGI
#ifndef LIGHT_GM_CLIENT
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_COMBAT_SKILL_EXPLAIN-1 ) break;
				ReadCombatSkillExplain(buf, i);
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( CombatSkillExplain, sizeof(t_CombatSkillExplain)*MAX_COMBAT_SKILL_EXPLAIN, 1, fp);
			fclose(fp);
		}
	 }
	else
#endif
#endif
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( CombatSkillExplain, sizeof(t_CombatSkillExplain)*MAX_COMBAT_SKILL_EXPLAIN, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}


inline void ReadCombatSkillGroupExplain(char * buf,int i)
{
	char *token;

	token = strtok(buf, "\t\n\r");	if( token == NULL ) return;
	const int iIndex = CombatSkillGroupExplain[i].iNo				= atoi(token);	
	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillGroupExplain[iIndex].iGroupNo		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillGroupExplain[iIndex].iGroupNo		= atoi(token);	

	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;
	memcpy(CombatSkillGroupExplain[iIndex].EngName	,token,30);
	EatRearWhiteChar( CombatSkillGroupExplain[iIndex].EngName);
	
	//token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;
	//memcpy(CombatSkillGroupExplain[iIndex].LocalName,token,	30);
	//EatRearWhiteChar( CombatSkillGroupExplain[iIndex].LocalName);

	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillGroupExplain[iIndex].iSkillId[0]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillGroupExplain[iIndex].iSkillId[1]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillGroupExplain[iIndex].iSkillId[2]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillGroupExplain[iIndex].iSkillId[3]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillGroupExplain[iIndex].iSkillId[4]		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;CombatSkillGroupExplain[iIndex].iSkillId[5]		= atoi(token);	

	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;
	memcpy(CombatSkillGroupExplain[iIndex].Explain,token,	100);
	EatRearWhiteChar( CombatSkillGroupExplain[iIndex].Explain);
	ConvertSpecialWord(CombatSkillGroupExplain[iIndex].Explain,"\n",'\n');
	ConvertSpecialWord(CombatSkillGroupExplain[iIndex].Explain,"\t",'\t');
	ConvertSpecialWord(CombatSkillGroupExplain[iIndex].Explain,"\r",'\r');
	
	return;
}

bool LoadCombatSkillGroupExplain()
{
#ifdef _DEBUG	// 031110 YGI
	char buf[1024];
#endif

	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	sprintf(filename, ".\\data\\CombatSkillGroupExplain.txt");
	sprintf(filenameBin, ".\\data\\CombatSkillGroupExplain.bin");
	
#ifdef _DEBUG	//< BBD 031214	텍스트는 디버그모드만 읽자
#ifndef LIGHT_GM_CLIENT
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_COMBAT_SKILL_GROUP_EXPLAIN-1 ) break;
				ReadCombatSkillGroupExplain(buf, i);
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( CombatSkillGroupExplain, sizeof(t_CombatSkillGroupExplain)*MAX_COMBAT_SKILL_GROUP_EXPLAIN, 1, fp);
			fclose(fp);
		}
	 }
	else
#endif
#endif	//> BBD 031214	텍스트는 디버그모드만 읽자
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( CombatSkillGroupExplain, sizeof(t_CombatSkillGroupExplain)*MAX_COMBAT_SKILL_GROUP_EXPLAIN, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

void DrawRareExplain(const int iExplainX, const int iExplainY, const int iRareIndex, const int iRareType)
{
	int iCount = 0;
	const int iLineGab = 24;
	const int trans = 20;
	const int iWidth = 130;

	Spr *back_spr = GetSprOfMenu( MAIN_ETC, 55 );
	SetHangulAlign( TA_LEFT);

	char *txt;
	
	switch(iRareType)
	{
	case 0:
		{
			txt = ItemRare[iRareIndex].Explain;
		}break;
	case 2:
	case 3:
	case 4:
		{
			txt = ItemFunction[iRareIndex].Exp;
		}break;
	default:
		{
			return;
		}break;
	}
	
	const int line	= GetLineRectTextPut( iWidth,txt );

	for( int i = 0; i<line; i++ )
	{
		FieldTypeNomalPutFx( iExplainX, iExplainY+iLineGab*(iCount+i), 0,0, 71, 10, 1 );
	}

	Hcolor( FONT_COLOR_YELLOW );
	RectTextPut(iExplainX+8,iExplainY+8,iWidth,txt); 
}

void DrawCombatSkillExplain(const int iExplainX, const int iExplainY, const int iCSIndex)
{
	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_NORMAL_WIDTH;
	const int iCSkillIndex = CombatSkill[iCSIndex].iSkillNo;
	
	int iCount = 0;
	int iReultLineCount = 0;
	
	int iBlankGab = 0;
	int iReultBlankSize = 0;
	
	const int iLineGab = TxtOut.GetLineGab();
	const int iBlankGabSize = TxtOut.GetBlankGabSize();
	
	LPCombatSkillExplain pCSE  = &CombatSkillExplain[iCSkillIndex];

	//헤더그림
	TxtOut.Init(COLOR_BLACK,TXT_BG_NORMAL_WIDTH);
	TxtOut.PutTextHeaderBGImg(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth);

	//전투스킬 이름
	Hcolor(FONT_COLOR_RARE_MAIN);
	iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,411),pCSE->EngName);
	iCount += iReultLineCount;

	iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
	iBlankGab+=iReultBlankSize;

	//Class 글자 + //클래스 설명 이름
	Hcolor(FONT_COLOR_BLIGHT_GREEN2);
	iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,421),pCSE->ClassName);
	Hcolor(FONT_COLOR_RARE_MAIN);
	RectTextPut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,lan->OutputMessage(0,420));
	iCount += iReultLineCount;

	//택틱 설명 이름
	Hcolor(FONT_COLOR_BLIGHT_GREEN2);
	iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,lan->OutputMessage(0,417),pCSE->WeaponTactic);
	iCount += iReultLineCount;
	
	iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
	iBlankGab+=iReultBlankSize;

	//그냥 설명
	Hcolor(FONT_COLOR_WHITE);
	iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,pCSE->Explain);
	iCount += iReultLineCount;

	iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
	iBlankGab+=iReultBlankSize;

	if(1)//클래스 배우기 가능 하면
	{	//How To Use
	//	Hcolor(FONT_COLOR_RARE_MAIN);
		Hcolor(FONT_COLOR_BLIGHT_ORANGE);
		iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,418));
		iCount += iReultLineCount;
		//사용방법
		Hcolor(FONT_COLOR_BLIGHT_ORANGE);
		iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,pCSE->Use);
		iCount += iReultLineCount;
		
		iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
		iBlankGab+=iReultBlankSize;

		const int iSkillLv = SCharacterData.SkillId[iCSkillIndex];
	
		Hcolor(FONT_COLOR_RARE_MAIN);
		if(iSkillLv)//
		{
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,412),iSkillLv);
			iCount += iReultLineCount;	
			Hcolor(FONT_COLOR_RARE_MAIN);
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,lan->OutputMessage(0,413),pCSE->iLvCp[iSkillLv-1]);
			iCount += iReultLineCount;
			//배웠을때 레벨에 따른 기능
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,pCSE->MicroExplain[iSkillLv-1]);//Micro explain//배열이기 때문에 iSkillLv+1 해 준다
			iCount += iReultLineCount;
			
			iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
			iBlankGab+=iReultBlankSize;
		}
		else//아직 안 배웠을때
		{
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,414));
			iCount += iReultLineCount;
				
			iReultBlankSize = TxtOut.RcBlankOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,1);
			iBlankGab+=iReultBlankSize;
		}

		if(3 > iSkillLv)//다음 레벨 관련 정보//다음레벨이 3보다 작고 0 보다 커야 한다// 왜냐 . 다음 레벨 이니까
		{
			//Next Lv
			Hcolor(FONT_COLOR_SOLID_RED);
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,415),iSkillLv+1);//Next Level
			iCount += iReultLineCount;

			//Need Lv
			Hcolor(FONT_COLOR_SOLID_RED);
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,lan->OutputMessage(0,416),pCSE->iNeedLv[iSkillLv]);//Require 
			iCount += iReultLineCount;

			if((iSkillLv+1) > SCharacterData.SkillId[iCSkillIndex])
			Hcolor(FONT_COLOR_SOLID_RED);
			iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,8,pCSE->MicroExplain[iSkillLv]);//Micro explain
			iCount += iReultLineCount;

		}
	}
	else //클래스 다르게
	{
		Hcolor(FONT_COLOR_RED);
		iReultLineCount = TxtOut.RcTXTOut(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth,0,lan->OutputMessage(0,422));
		iCount += iReultLineCount;
	}
	//꼬리그림
	TxtOut.PutTextTailBGImg(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth);
}

void DrawCombatSkillGroupExplain(const int iExplainX, const int iExplainY, const int iGroupNo, const int iGroupType)
{
	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_SHORT_WIDTH;
	int iIndex = (iGroupNo-1)*2 +(iGroupType);
	int iCount = 0;
	const int iLineGab = TxtOut.GetLineGab();
	int iReultLineCount = 0;
	int iBlankGab = 0;
	const int iBlankGabSize = 3;

	LPCombatSkillGroupExplain pCSGE  = &CombatSkillGroupExplain[iIndex];

	//헤더그림
	TxtOut.Init(COLOR_BLACK,TXT_BG_SHORT_WIDTH);
	TxtOut.PutTextHeaderBGImg(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth);

	Hcolor(FONT_COLOR_GREEN);
	iReultLineCount = TxtOut.RcTXTOut( iExplainX, iExplainY+iLineGab*iCount,iWidth,0, "< %s >", pCSGE->EngName);  //줄간격은 15이다
	iCount+=iReultLineCount;

	Hcolor(FONT_COLOR_WHITE);
	iReultLineCount = TxtOut.RcTXTOut( iExplainX, iExplainY+iLineGab*iCount, iWidth,0, pCSGE->Explain);
	iCount+=iReultLineCount;

	//헤더그림
	TxtOut.PutTextTailBGImg(iExplainX,iExplainY+iBlankGab+iLineGab*iCount,iWidth);
}

inline void ReadFunctionGroup(char * buf,int i)
{
	char *token;

	token = strtok(buf, "\t\n\r");	if( token == NULL ) return;const int iIndex = RareGroup[1][i].no	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(RareGroup[1][iIndex].name		,token,39);
	EatRearWhiteChar( RareGroup[1][iIndex].name );
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].rareset[0].rare,token,	39);
	EatRearWhiteChar( RareGroup[1][iIndex].rareset[0].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[0].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[0].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].rareset[1].rare	,token,	39);
	EatRearWhiteChar( RareGroup[1][iIndex].rareset[1].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;RareGroup[1][iIndex].rareset[1].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[1].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].rareset[2].rare	,token,	39);
	EatRearWhiteChar( RareGroup[1][iIndex].rareset[2].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[2].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[2].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(RareGroup[1][iIndex].rareset[3].rare	,token,	39);		
	EatRearWhiteChar( RareGroup[1][iIndex].rareset[3].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[3].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[3].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].rareset[4].rare	,token,	39);	
	EatRearWhiteChar( RareGroup[1][iIndex].rareset[4].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[4].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;RareGroup[1][iIndex].rareset[4].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].rareset[5].rare	,token,	39);	
	EatRearWhiteChar( RareGroup[1][iIndex].rareset[5].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[5].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[5].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].rareset[6].rare	,token,	39);	
	EatRearWhiteChar( RareGroup[1][iIndex].rareset[6].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;RareGroup[1][iIndex].rareset[6].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[6].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].rareset[7].rare	,token,	39);	
	EatRearWhiteChar( RareGroup[1][iIndex].rareset[7].rare);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[7].rare_num		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].rareset[7].rare_rate	= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;memcpy(RareGroup[1][iIndex].group_buffer_1	,token,	39);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].group_buffer_2	,token,	39);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;memcpy(RareGroup[1][iIndex].group_buffer_3	,token,	39);
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].group_buffer_4		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].group_buffer_5		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;RareGroup[1][iIndex].group_buffer_6		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].group_buffer_7		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].group_buffer_8		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].group_buffer_9		= atoi(token);	
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	return;RareGroup[1][iIndex].group_buffer_10	= atoi(token);	
 
	return;
}

void RareGroupSet()
{
	int i1 = 1,i2 = 1,	i3 = 1,	i4 = 1;
	for (;i1 < MAX_RARE_GROUP;i1++)
	{
		const int iGroupSet = RareGroup[1][i1].group_buffer_10;
		switch(iGroupSet)
		{
		case 2:
			{
				RareGroup[iGroupSet][i2] = RareGroup[1][i1];
				i2++;
			}break;
		case 3:
			{
				RareGroup[iGroupSet][i3] = RareGroup[1][i1];
				i3++;
			}break;
		case 4:
			{
				RareGroup[iGroupSet][i4] = RareGroup[1][i1];
				i4++;
			}break;
		default:
			continue;
			break;
		}
		
	}
}

bool LoadFunctionGroup()
{
	char buf[1024];
	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	sprintf(filename, ".\\itemtable\\function_group.txt");
	sprintf(filenameBin, ".\\itemtable\\function_group.bin");
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_RARE_GROUP-1 ) break;
				ReadFunctionGroup(buf, i);
			i++;
		}
		fclose(fp);
		RareGroupSet();
		
		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( RareGroup[1], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fwrite( RareGroup[2], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fwrite( RareGroup[3], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fwrite( RareGroup[4], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fclose(fp);
		}
	 }
	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( RareGroup[1], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fread( RareGroup[2], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fread( RareGroup[3], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fread( RareGroup[4], sizeof(t_raregroup)*MAX_RARE_GROUP, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

bool GetItemDurTxt(ItemAttr *pItem, char* szTxt)
{
	CItem *t = ::ItemUnit(pItem->item_no);
	if(!t){return false;}
	const int iItemWeight	= t->GetItemWeight();
	const int iItemKind		= t->GetItemKind();
	if( iItemWeight )
	{
		sprintf( szTxt, lan->OutputMessage(7,153), iItemWeight );
		if( t->GetRButton() == DIVIDE_ITEM )
		{
			sprintf( szTxt, lan->OutputMessage(7,154), szTxt, pItem->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max=0, d_curr=0;
			GetItemDuration( *pItem, d_curr, d_max );
			sprintf( szTxt, lan->OutputMessage(7,155), szTxt, d_curr/10, d_max/10 );
		}
	}
	else
	{
		if( t->GetRButton() == DIVIDE_ITEM )
		{
			if( iItemKind == IK_MONEY )
				sprintf( szTxt, "%d CELL", pItem->attr[IATTR_MUCH] );
			if( iItemKind == IK_NEW_MONEY )
				sprintf( szTxt, "%d CRIT", pItem->attr[IATTR_MUCH] );
			else
				sprintf( szTxt, lan->OutputMessage(7,156), pItem->attr[IATTR_MUCH] );
		}
		else 
		{
			WORD d_max, d_curr;
			GetItemDuration( *pItem, d_curr, d_max );
			sprintf( szTxt, lan->OutputMessage(7,157), d_curr/10, d_max/10 );
		}
	}
	return true;
}

int CheckMerchantOrSkillMaster()
{//011108 lsw
	t_SkillExp *pExp = SCharacterData.nJobSkill;

	if(!::IsAllGM())//기본으로 All Gm이어야 한다
	{	
		return 0;
	}

	if(!::IsMerchant())//상인이 아니라면
	{
		return GM_QUEST_MERCHANT;//상인이 안되면 무조건 상인부터 되어라
	}

	SkillExp3 *pExp3 = (SkillExp3*)&pExp[SKILL_MASTER];

	if(	pExp3->skillType !=GM_QUEST_BLACKSMITH_MASTER	//대장장이 기성
	&&	pExp3->skillType !=GM_QUEST_CARPENTER_MASTER	//목수 기성    
	&&	pExp3->skillType !=GM_QUEST_BOWCRAFT_MASTER	//활제작사 기성
	&&	pExp3->skillType !=GM_QUEST_TAILOR_MASTER)	//재단사 기성
	{//아무것도 기성이 아닌 상태 라면
		return GM_QUEST_BLACKSMITH_MASTER;	//대장장이 기성
	}
	return 0;
}

extern void FreeTestAnimation();

extern int iSelectedCharacter;
extern void RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(t_packet *p);
void CallMN_START_MAP_Menu()
{
#ifdef _DEBUG
	if( SysInfo.notconectserver )
	{
		t_packet p;
		p.u.Hwoa.rang.SelectedCharResult.iKey = 1;
		RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(&p);
		return;
	}
#endif

	iSelectedCharacter = -1;
	StartPosition = 0;
	SCharacterData = SCharSource;
	FreeTestAnimation();//020530 lsw

/*	t_packet packet;
	packet.h.header.type = CMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON;
	packet.h.header.size = sizeof(SELECTEDCHARRESULT);
	packet.u.Hwoa.rang.SelectedCharResult.iLv  = SCharacterData.nLevel;
	packet.u.Hwoa.rang.SelectedCharResult.iKey  = 0;
	::memcpy(packet.u.Hwoa.rang.SelectedCharResult.szName,SCharacterData.sCharacterName,MAX_NAME);
	::QueuePacket( &packet, 1 );
	y_MenuFocus=MN_START_MAP; finito 060507 - removed just doesn't seem to do anything useful at all...*/
	t_packet p;
	p.u.Hwoa.rang.SelectedCharResult.iKey = 1;
	RecvCMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON(&p);
}