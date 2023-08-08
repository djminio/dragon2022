#include "stdafx.h"
#include <Ddraw.h>
#include <Math.h>

#include "sys/stat.h"

#include "Dragon.h"	
#include "Hong_Sprite.h"
#include "Hong_Sub.h"
#include "char.h"  
#include "map.h"
#include "Path.h"

#include "MouseCursor.h"
#include "DirectSound.h"
#include "CharDataTable.h"
#include "Effect.h"
//#include "MenuDef.h"
#include "ItemTable.h"
#include "NetWork.h"
#include "GameProc.h"
#include "Hangul.h"
#include "Tool.h"
#include "Item.h"
#include "SkillTool.h"
#include "Menu.h"
#include "TextOutMgr.h"
#include "smallMenu.h"
#include "Skill.h"
#include "language.h"
#include "NumberAniMgr.h"

#include "SymbolMgr_Client.h"//soto-1.02

//#define LIGHT_GM_CLIENT //Eleval 01/06/09

extern CSymbolMgr_Client	g_SymBolMgr;//soto-1.02

SkillMain SkillTable[ MAX_SKILL_];
char	  SkillOutputMessage[ 31];
LPSKILLGAUGE g_SkillGauge;
ITEMMULTIRARE aIMR[MAX_ITEM_MULTI_RARE];//021111 lsw
extern void SetBattlePeace( const int type );
/*
��ȣ����

1 Farmer
2 Miner
3 Fisher
4 Lumberjack
5 Butcher
6 Herb-digger
7 Herdsman
8 Carpenter
9 Blacksmith
10 Cooker
11 Tailor
12 Bowcraft/Fletcher
13 Alchemist
14 Candlemaker
15 Merchant
*/

/* // Skill No.....
1	Detect Ghost	
2	Talk With 
3	Poisoning	
4	Detect Poison	
5	Find Traps	
6	Remove Traps	
7	Entrapmen			-------------------
8	Magic Scrolling	
9	Ability Scrolling	
10	Emergency Alram	
11	First Aid	
12	Fast Foot	
13	Secrethear	
14	Wallclimbing	
15	Intuition	
16	Detect Bomb	
17	Bomb in			--------------------
18	Remove Bomb	
19	Stealing	
20	Lockpicking	
21	Reclaim	
22	Sow	
23	Cultivate	
24	Havest	
25	Detect Mineral	
26	Mining	
27	Refine	
28	Fish 
29	Baitmaking	
30	Netting	
31	Sailing	
32	Cutting	
33	Lumbering	
34	Butchering	
35	Detect Herb	
36	Digging	
37	Makepowder	
38	Herding	
39	Milking	
40	Midwifery	
41	Shear	
42	Manufacturing	
43	Manufacturing	
44	Cooking	
45	Clothing	
46	Dyeing	
47	Tailoring	
48	Bowcrafting	
49	Fletching	
50	MakePotion	
51	MakeMetal	
52	MakeBottle	
53	Candlemaking	
54	Oiling	
55	Repairng	
56	Reutilizing	
57	Playing	
58	Peacemaking	
59	Paralysis	
60	Hallucination	
61	Provocation	
62	Enticement	
63	Noise	
64	Tracking	
65	Snooping	
66	Search	
67	Silentmove	
68	Disguise	
69	Chameleon	
70	Hide on 
71	Object Hidding	
72	Hide	
73	Camping	
74	Makefire	
75	Taming	
76	Communicate with 
77	Communicate with 
78	Animal lore	
79	Arms lore	
80	Evaluate Inteligence	
81	Evaluate Wisdom	
82	Anatomy Judgement	
83	Item Identification	
84	Taste Identification	
85	Appraisal	
86	Healing	
87	Tactics	
88	Crapple	
89	Swordmanship	
90	Archery	
91	Fencing	
92	Mace Fighting	
93	Pierce	
94	Whirl	
95	Hurl	
96	Parrying
97	Double Swordmanship	
98	Double Macefighting	
99	Magery
100	Orison

*/	
	
	
/*
1
2
3
4
8
9
10
11
12
13
14
15




								case 5 :
								case 6 :
								case 7 :
								case 16:
								case 17:
								case 18:
								case 19:
20
								case 21:
								case 22:
								case 23:
								case 24:
								case 25:
								case 26:
								case 27:
								case 28:
29
30
31
								case 32:
								case 33:
								case 34:
								case 35:
								case 36:
								case 37:
38
39
40
								case 41:
42
43
								case 44:
								case 45:
								case 46:
								case 47:
								case 48:
								case 49:
50
								case 51:
								case 52:
								case 53:
								case 54:
55
56
57
58
59
60
61
62
63
64
								case 65:
66
67
68
69
70
71
72
73
								case 74:
								case 75:
76
77
78
79
								case 80:
								case 81:
								case 82:
83	
84	
85	
86	
87	

101
102
103
104
105

  						
								  
									
									  
										
										  
											
											  
												c[cn].chrlst.Skill[i] = 1;
*/


////////////////////////////////////////////////////////////////////////////////////
///	
///					External variables...
///   
	
extern Spr		skill_spr[102];

extern void		SetItemPos(int type, POS*);
extern void		SetItemPos(int type, int p, POS*);
extern void		SetItemPos(int type, int p1, int p2, int p3, POS*);
extern ItemAttr *GetItemByPos( POS item );

extern struct		InterfaceFlag main_interface;

extern int SkillOn;

extern int nItemOther;
extern	ItemAttr InvItemAttr[3][3][8];
extern	ItemAttr EquipItemAttr[8];
extern	ItemAttr QuickItemAttr[7];


extern PosItem		skill_use_item[5][10];
extern PosItem		skill_use_item_result;


	
/////////////////////////////////////////////////////////////////////////////////////
///	
///					User variables...
///   
int		SkillNo;
int		SkillStatus;
Spr	   *SkillIcon;
int		SkillItemNo;			// ����� ����  Item�� ��ȣ.
POS		SkillItemPOS;			// ����� ����  Item�� ��ġ��.

//1005 zhh
LPITEMGROUND Skill_CursorCheckedItem;//��ų�� �������� �ʿ�� �� ���




/////////////////////////////////////////////////////////////////////////////////////
///
///					User Functions Declaration...
///   
void GameStatusOutput( int no );
void AgainSkill( int skillno );
void DrawSkilltile( int skillno, int x, int y );
void DrawSkillGauge( LPSKILLGAUGE n );
void DeleteSkillGauge( LPSKILLGAUGE tt );
LPSKILLGAUGE AddSkillGauge( int skillno, int x, int y, int min );
void OutputSkillGauge( void );
	
	
void SendSkillGauge( int x, int y, int skillno );
void RecvSkillGauge( t_server_skill_gauge *p );
	
	
	
//1004 zhh
int AmIEquitSkillTool(int Skillno);	
	
//010205 lsw
int skill_fail(int skill);//1�� ���� ���� ���н� ������ �޼��� 
	
	
/////////////////////////////////////////////////////////////////////////////////////
///		
///					User Functions...
///		
void LoadHongSkillTable( void )
{			
	//1024 zhh Dragon.cpp�� ��ġ�� �ʱ� ���ؼ� ����ٰ� �߰�
		//010215 lsw �Ʒ� ���� �ּ� ó�� ����.
	//	if(!LoadLanguageText())
	//	MessageBox(NULL,"Loading Language Pack Error","LanguagePack",NULL);

	FILE *fp;
	int   i;
	char  crc, crc1 = 0;
			
	fp = Fopen( "./data/skilltable.bin", "rb" );
	if( fp == NULL ) return;
	fread( &crc, 1,1, fp );
	fread( SkillTable, sizeof( SkillMain ), MAX_SKILL_, fp);
	fclose(fp);
			
	char *tt = (char *)SkillTable;
	for( i = 0 ; i < sizeof( SkillMain ) * MAX_SKILL_ ; i ++)
	{		
		crc1 += (( *tt + *tt ) - i );
		*tt -= ( 200 + i );
		tt ++;
	}		
			
	for( i = 0 ; i < MAX_SKILL ; i ++)
	{		
		EatRearWhiteChar( SkillTable[i].Skill_Minute);
	}		
	//< CSD-030324
	/*			
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{		
		fp = Fopen( "./data/�����(SkillTable_effect_wave).txt", "wb" );
		if( fp )
		{	
			for( int i = 0 ; i < 102 ; i ++)
			{
				fprintf( fp, "%d : 	%d	\n", i, 	SkillTable[ i].effect_wave );
			}
			fclose(fp);
		}	
	}			
	*/
	//> CSD-030324
	if( CheckSumError == 0 ) 
	{
		CheckSumError = abs( crc - crc1 );
		if( CheckSumError ) JustMsg( "skilltable.bin CheckSumError " );
	}
}		
	


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��   �� : ��������� ���� ������ ���� �����.( ����� ������.)
// ��   �� :  (1). ��� ���� �� ���·� ����.						
//			  (2). ��� ���� �� ���·� ����( ��������� ������. )
// ��    �� : ����.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
void SkillSettingClear( void )
{	
	SkillStatus = SKILL_OFF;
	CursorNo( 1 );
	MouseCursorOnOff = true;
	SkillOn = -1;
}	
	
	
void SendSkillMatter( MakeItem *matter )
{			
//	ItemAttr *t;
	t_packet p;		
	int i;			
					
	p.h.header.type = CMD_SKILL_ITEM_MATTER;
	{				
		for( i = 0 ; i < 20 ; i ++)
			p.u.client_skill_item_matter.pos[i] = matter[i].pos;
	}				
	p.h.header.size = sizeof( t_client_skill_item_matter );
	QueuePacket( &p, 1 );
		
	SkillStatus = SKILL_ING;
		
	for( i = 0 ; i < 20 ; i ++)
	{	
		ItemAttr *t = ::GetItemByPos( matter[i].pos );
		DeleteItem( t );
	}	
		
	CursorNo( 1 );
}					
		
extern void CheckTreasure( int x, int y );		// 040105 YGI
// 040105 YGI
void SendSkillInfo( int skillno, int x, int y, int targettype, int targetid ,const int iKey, const char* TargetName)//030102 lsw
{		
	POS s;
	s.type = 0;
	memset(&s,0,sizeof(POS));
	SendSkillInfo( skillno, x, y, targettype, targetid, s, iKey,TargetName);//020531 lsw
	if( skillno == SKILL_MINING )
	{
		CheckTreasure( x, y);
	}
}				

extern int g_EventLocalWarBegin;	
		
void SendSkillInfo( int skillno, int x, int y, int targettype, int targetid, POS s,const int iKey, const char* TargetName)//030102 lsw
{	
	if ((MapNumber==39)&&(g_EventLocalWarBegin==3)) return;	// LTS BUG

	switch( skillno )
	{			
		// view ghost...
	case 1 :
	case 2 :
		{
			if( CanViewGhost(Hero) )	// ����� �ߺ��Ͽ� ����Ҽ� ����. 
			{
				strcpy( Hero->szChatMessage, lan->OutputMessage(2,189));
				Hero->ChatDelay = 70;
				SkillSettingClear();
				return;
			}
		}
		break;	
				
	case 5  :	
	case 16 :	InsertMagic( Hero, Hero, 212, 0, Mox, Moy - 80, 0,0 );
		break;	
			
	case 34 :   
		
		break;
	}	
	//010204 lsw
	//1024 zhh
	if(skillno >100)
	{
		AddCurrentStatusMessage( 150,150,255,lan->OutputMessage(2,31), 	SkillTable[skillno].Skill_Minute );
	}

	t_packet p;	
	p.h.header.type = CMD_SKILL_INFO;
	{			
		p.u.client_skill_info.skillno	= SkillNo;
		p.u.client_skill_info.x			= x;
		p.u.client_skill_info.y			= y;
		p.u.client_skill_info.targettype= targettype;
		p.u.client_skill_info.target_id = targetid;
		p.u.client_skill_info.s			= s;
		p.u.client_skill_info.iKey		= iKey;
		if(TargetName == NULL)
		{
			::strcpy(p.u.client_skill_info.TargetName,"");//030102 lsw
		}
		else
		{
			::strcpy(p.u.client_skill_info.TargetName,TargetName);
		}
		
	}

	p.h.header.size = sizeof( t_client_skill_info );
	QueuePacket( &p, 1 );
				
	SkillStatus	= SKILL_ING;
//	CursorNo( 1 );
}				
				
				
				
				
				
				
				
				
/* ����  250,	1, 
//				2,
//				3,
//				4, 
//				5- ����...
//				6- ����...

		���� Wave��ȣ : 108
		���� Wave��ȣ : 109

	case 0:		//Skill->Level Up
		InsertMagic( scr, scr, 211, -1, 0, 0, x, y);				//Level Up 
		break;
	case 1:		//Skill->Detect
		InsertMagic( scr, scr, 212, 0, Mox, Moy, 0, 0);				//Skill_Detect
		break;
	case 2:		//Skill->Identify
		InsertMagic( scr, scr, 213, 0, Mox, Moy, 0, 0 );			//Skill_Identify
		break;
	case 3:		//Skill->�к�
		InsertMagic( scr, scr, 214, -2, Mox, Moy, 0, 0 );			//Skill_�к� 
		break;
	case 4:
		InsertMagic( scr, scr, 218, 0, Mox, Moy, 0, 0 );			//������.���ݼ��� ��������
		break;
	case 5:
		InsertMagic( scr, scr, 208, 0, Mox, Moy, 0, 0 );			//���� �������� �κ�Ʈ�� ����
		break;
	case 6:
		InsertMagic( scr, scr, 215, -2, Mox, Moy, 0, 0 );			//�����-������
		break;
	case 7:
		InsertMagic( scr, scr, 207, 0, Mox, Moy, 0, 0 );			//���/��ġ�� ����ũ
		break;
	case 8:
		InsertMagic( scr, scr, 210, 0, Mox, Moy, 0, 0 );			//�ν˵� ����ũ
		break;
	case 9:
		InsertMagic( scr, scr, 216, 0, Mox, Moy, 0, 0 );			//����/�뱤�� ����(�����)
		break;
	case 10:
		InsertMagic( scr, scr, 217, -2, Mox, Moy, 0, 0 );			//���ۺ� 
		break;
	case 11:
		InsertMagic( scr, scr, 219, rand()%6, Mox, Moy, 0, 0 );		//����� �������� �پ�ö����
		break;
	case 12:
		InsertMagic( scr, scr, 226+rand()%5, rand()%6, Mox, Moy, 0, 0 );	//�ι� ����� ȿ�� 5�� ����(random ���)
		break;
*/		
		
		

static int startcirclemagic;
static int startcirclemagicID;

void StartCircleMagic( int id, int type )
{
	switch( type )
	{
	case 0 :	startcirclemagic = 50;
				startcirclemagicID = id;
		break;
	}
}


void  CircleMagic( void )
{		
	if( startcirclemagic == 0 ) return;
		
	int x, y;
		
	startcirclemagic--;
	if( startcirclemagic )
	{	
		LPCHARACTER ch = ReturnCharacterPoint( startcirclemagicID );
		if( ch == NULL ) return;
			
		x = (int )((20.+(50.))*cos((startcirclemagic *30)%360*3.1415/180.));
		y = (int )((10.+(50.))*sin((startcirclemagic *30)%360*3.1415/180.));
			
		InsertMagic( ch, ch, 213, 9, 0, 0, ch->x + x, ch->y + y);//, 0,0 );	
	}	
}		
		
		
		
		
									
void RecvSkillInfo( t_server_skill_info *p )
{					
	int skillno, x, y, targettype, target_id;
	int id;
	LPCHARACTER ch = ReturnCharacterPoint( p->server_id );
	if( ch == NULL ) return;//
	
	id		  = p->server_id;
	x         = p->x;			
	y         = p->y;			
	skillno   = p->skillno;		
	targettype= p->targettype;	
	target_id = p->target_id;	
	
//	if( ch == Hero ) CursorNo( 1 );

	// ȭ�鿡  �����ϴ°��� ���濡�Դ� �������� �ʴ´�.  ( Cursor )...
	if( ch != Hero )
	switch( skillno )
	{
		// stealing..
		case  1 :
		case  2 :
		case  3 :	break;

		case 19 : 	return;	//1007 zhh ��ġ�⸸ ������ �� �����ش�

		default :	
					ChangeDirection( &ch->todir, ch->x, ch->y, x, y );
					//1006 zhh //1007 zhh
				//	AddSkillCursor( Hero, skillno, SkillTable[ skillno].Curser3, x, y, 1 );
				break;
	}				
					
	//AddSkillCursor( skilno, SkillTable[ skillno].Curser2, x, y, 3 );	
	if(SkillTable[ skillno].effect_wave!=0)
		PlayListAutoSounds( SkillTable[ skillno].effect_wave , 0, 0, 0 );
					
	/////////////////////////////////////// �̱��� 1226(����) ///////////////////////////////////////		
	switch( skillno )
	{				
		//  Ghost Detect...
		case  1 :		
		case  2 :	
		case  3 :  StartCircleMagic( p->server_id, 0 );
			break;	
					
		case 6 :	InsertMagic( Hero, Hero, 212, 9, 0, 0, x, y-80);//, 0,0 );	
			return;

		case 7 :	InsertMagic( Hero, Hero, 212, 9, 0, 0, x, y-80);//, 0,0 );	
			return;

			// Reclaim
		case 21 :	
		case 24 :	
		case 32 :	
		case 36 :	
			//1006 zhh
		case 75 :	if( ch == Hero ) 
					{
						StopDoingSkillCursor( ch );
						//1006 zhh
						AddSkillCursor(  ch,  skillno, SkillTable[ skillno].Curser2,  x,  y,  2 );
					}
	 				PlayListAutoSounds( 2438, 0, 0, 0 );

			break;	
				
		//1017 zhh
		case 30:
		case 28 :	if( TileMap[ x/TILE_SIZE][y/TILE_SIZE].attr_river )	
						InsertMagic( Hero, Hero, 219, rand()%8, x, y, 0, 0 );		//����� �������� �پ�ö����

					//1017 zhh
					if( ch == Hero ) 
					{
						StopDoingSkillCursor( ch );
						//1006 zhh
						AddSkillCursor(  ch,  skillno, SkillTable[ skillno].Curser2,  x,  y,  2 );
					}
			return;

		case 33 :	
			//1006 zhh
			if( ch == Hero ) 
				//1006 zhh
				AddSkillCursor(  ch,  skillno, SkillTable[ skillno].Curser2,  x,  y,  4 );
			break;


		case 25 : 
		case 35 :	//AddDetectTile( 0, x, y, RGB16( 0, 255, 0 )); 
					if( ch == Hero ) 
						//1006 zhh
						AddSkillCursor(  ch,  skillno, SkillTable[ skillno].Curser2,  x,  y,  4 );
					InsertMagic( Hero, Hero, 253, 9, 0, 0, x, y);//,0,0 );
			break;

			// MakeFire
		case 74 :  	InsertMagic( Hero, Hero, 210, 9, 0, 0, x, y);//, 0, 0 );
			return;

			//1011 zhh
		default:	if( ch == Hero ) 
					{						
						StopDoingSkillCursor( ch );
						//1006 zhh
						AddSkillCursor(  ch,  skillno, SkillTable[ skillno].Curser2,  x,  y,  2 );
					}
			break;
	
	}
}



//1024 zhh
//1019 zhh
void RecvSkillAnimalLore( t_packet *p )
{
	char temp[ FILENAME_MAX];	

	t_server_skill_animal_lore haha = p->u.server_skill_animal_lore;	
	
	switch(haha.result)
	{
	case 0:
		sprintf( temp," <%s>  HP  %d/%d", haha.name , haha.NowLife , haha.MaxLife );
		AddCurrentStatusMessage( 255,150,150, temp );
		break;
	case 1:	
		sprintf( temp," <%s>  HP  %d/%d", haha.name , haha.NowLife , haha.MaxLife );
		AddCurrentStatusMessage( 255,150,150, temp );
		sprintf( temp," <%s>  Level:%d   Exp:%d", haha.name , haha.Level , haha.exp );
		AddCurrentStatusMessage( 255,150,150, temp );
		break;
	case 2:
		sprintf( temp," <%s>  HP  %d/%d", haha.name , haha.NowLife , haha.MaxLife );
		AddCurrentStatusMessage( 255,150,150, temp );
		sprintf( temp," <%s>  Level:%d   Exp:%d", haha.name , haha.Level , haha.exp );
		AddCurrentStatusMessage( 255,150,150, temp );
		sprintf( temp," <%s>  Loyalty:%d", haha.name , haha.loyalty );
		AddCurrentStatusMessage( 255,150,150, temp );
		break;
	case 3:		
		sprintf( temp," <%s>  HP  %d/%d", haha.name , haha.NowLife , haha.MaxLife );
		AddCurrentStatusMessage( 255,150,150, temp );
		sprintf( temp," <%s>  Level:%d   Exp:%d", haha.name , haha.Level , haha.exp );
		AddCurrentStatusMessage( 255,150,150, temp );
		sprintf( temp," <%s>  Loyalty:%d", haha.name , haha.loyalty );
		AddCurrentStatusMessage( 255,150,150, temp );
		sprintf( temp," <%s>  Damage:%d", haha.name ,haha.damage );
		AddCurrentStatusMessage( 255,150,150, temp );
		break;
	default:
		return ;
	}		
	::strcpy( Hero->szChatMessage, lan->OutputMessage(2,33) );
	Hero->ChatDelay = 50;
	SkillSettingClear();
}

void RecvOutPutMessage(t_packet *p )
{
	DWORD para1 = p->u.server_output_message.para1;
	DWORD para2 = p->u.server_output_message.para2;
	::AddCurrentStatusMessage( FONT_COLOR_ORANGE, lan->OutputMessage(para1,para2) );
}

void RecvSoundOut(t_packet *p )
{
	DWORD para1 = p->u.server_output_message.para1;
	DWORD para2 = p->u.server_output_message.para2;
	::MP3( para2 );
}



void RecvSkillResult( t_packet *p )
{		
	char temp[ FILENAME_MAX] = {0,};
	int result = p->u.server_skill_result.result;

	if( result == -999 )
	{
		::SkillSettingClear();
		::AddCurrentStatusMessage( 255,150,150, lan->OutputMessage(2,32) );
	}

	//////////////////////////////////// �̱��� 1226(����) ////////////////////////////////////
	::InsertMagic( Hero, Hero, 250, (rand()%4) + 1, p->u.server_skill_result.x, p->u.server_skill_result.y, 0, 0);//, 0, 0);

	::PlayListAutoSounds( 2408, 0, 0, 0 );
	
	Hero->ChatDelay = 0;
/* ����  250,	1, 
//				2,
//				3,
//				4, 
//				5- ����...
//				6- ����...

		���� Wave��ȣ : 108
		���� Wave��ȣ : 109 */
	
	switch( p->h.header.type )
	{
	case CMD_SKILL_RESULT : 
		
		switch( p->u.server_skill_result.skillno )
		{
		case 1 :	//020320 lsw	��Ʈ���� ��Ʈ�� ���ϱ� ����
		case 2 :
			{		
					Hero->bCanViewGhost = true;
					//1024 zhh
					AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,22),  result );
					SkillSettingClear();
				break;
			}

		case 105 : 
			{
				LPCHARACTER ch = ReturnCharacterPoint( result );

				//1024 zhh
				if( ch ) AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,24), ch->name );
			//	if( ch ) AddCurrentStatusMessage( 255,255,0 , "����� ��������  '%s'(��)�� �������� ���� �½��ϴ�.", ch->name );

				SkillSettingClear();
				break;
			}

		case 5 :	
			{
				Hero->bCanViewTrap = true;
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,27),  result );//020401 LSW
				SkillSettingClear();
			}break;
		case 16 :
			{
				Hero->bCanViewBomb = true;
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,28),  result );//020401 LSW
				SkillSettingClear();
			}break;

		case 6  :   
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,141) );
				SkillSettingClear();
			}break;
		case 7 :
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,142) );
				SkillSettingClear();
			}break;
		case 17  :
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,144) );
				SkillSettingClear();
			}break;
		case 18 :	
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,145) );
				SkillSettingClear();
			}break;
		case 19 :
			{
				if( p->u.server_skill_result.result == 2 )  // ���״�. 
				{
					AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,146) );
				}
				else
				{
					AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,147) );
				//	AddCurrentStatusMessage( 255,255,0 , " �ϳ� ��ú���ϴ�." );
				}

				SkillSettingClear();
			}break;

		case 21 :	
			{
				sprintf( SkillOutputMessage, "%d", p->u.server_skill_result.result );
				GameStatusOutput( 7 );

				if(p->u.server_skill_result.result<100)
				{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,148),  p->u.server_skill_result.result);
				}
				else
				{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,139));
				}
				AddSkillGauge( SkillNo, p->u.server_skill_result.x, p->u.server_skill_result.y, p->u.server_skill_result.result );
			}break;

		case 22 :
			{
			switch( p->u.server_skill_result.result )
				{
					case 0 : AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,149) );
						break;
						//1024 zhh
					case 1 : AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,150) );
						break;
				}
				strcpy( Hero->szChatMessage, lan->OutputMessage(2,151) );
				Hero->ChatDelay = 50;
			}
			break;

		case 23 :
			{
				if(p->u.server_skill_result.result)
				{
					AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,152) );
				}
				else 
				{
					AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,153) );
				}
			}break;

		case 24 :   
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,154)); 
					SkillSettingClear();
			}break;
		case 25 :	
			{
				switch(rand()%2)
				{
					case 0 : sprintf( temp, lan->OutputMessage(2,155) ); break;
					case 1 : sprintf( temp, lan->OutputMessage(2,156), subMineType[p->u.server_skill_result.result] +2); break;
				}	
				strcpy( Hero->szChatMessage, temp );
				Hero->ChatDelay = 50;
				SkillSettingClear();
			}break;		
		case 26 :  	
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,157) );
				SkillSettingClear();
			}
			break;

		case 28 :  	
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,158) );
				SkillSettingClear();
			}break;
		case 30 :  	
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,159) );
				SkillSettingClear();
			}break;
		case 32 :   
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,160) );
				SkillSettingClear();
			}break;

		case 33 :
			{
				CItem *ti = ItemUnit(Skill_CursorCheckedItem->no);
				if( ti )
				{
					AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,161) ,GetItemHanName( Skill_CursorCheckedItem->no )
						,GetItemHanName( ti->GetMouse_MRD() / 100 ));
					SkillSettingClear();
				}
			}
			break;
		case 34 :   
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,162) );
				SkillSettingClear();
			}break;
		case 35 :	
			{
				sprintf( temp, lan->OutputMessage(2,163), subHubType[p->u.server_skill_result.result] + 2 );
				strcpy( Hero->szChatMessage, temp );
				Hero->ChatDelay = 50;
			}break;		


		case 36 :  
			{
				CItem *ti = ItemUnit(p->u.server_skill_result.result/1000,p->u.server_skill_result.result%1000);
				if( ti )
				{
					AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,164), ti->GetItemHanName() );
					SkillSettingClear();
				}
			}
			break;
		case 41 :   
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,165) );
				SkillSettingClear();
			}break;
		case 46 :
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,166) );
				SkillSettingClear();
			}break;
		case 59 :
			{ 
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,167) );
				SkillSettingClear();
			}break;
		case 58 :
			{ 
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,168) );
				SkillSettingClear();
			}break;

		case 54 :
			{ 
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,169) );
				SkillSettingClear();
			}break;
		case 65	:
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,170) );
				SkillSettingClear();
			}break;
		case 67	:
			{   
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,171) );
				SkillSettingClear();
			}break;
		case 69	:
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,172) );
				SkillSettingClear();
			}break;

		case 70	:
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,173) );
				SkillSettingClear();
			}break;
		case 71	:
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,174) );
				SkillSettingClear();
			}break;
		case 74 :
			{
				AddCurrentStatusMessage( 255,255,0 , lan->OutputMessage(2,175) );
				SkillSettingClear();
			}break;
		
		case 75 :   switch( rand()%4 )
					{	
						case 0 : sprintf( temp, lan->OutputMessage(2,176) );	break;
						case 1 : sprintf( temp, lan->OutputMessage(2,177) );	break;
						case 2 : sprintf( temp, lan->OutputMessage(2,178) );	break;
						case 3 : sprintf( temp, lan->OutputMessage(2,179) );	break;
					}	
					strcpy( Hero->szChatMessage, temp );
					Hero->ChatDelay = 50;

					if( p->u.server_skill_result.result == 200 )
					{
						AddCurrentStatusMessage( 180,214,82,lan->OutputMessage(2,180) );
					}
					else 
					{
						AddCurrentStatusMessage( 180,214,82,lan->OutputMessage(2,181), p->u.server_skill_result.result );
					}

					SkillSettingClear();
			break;	

		default :	
			{
				switch( rand()%3 )
					{
						case 0 :	sprintf( temp, lan->OutputMessage(2,182) ); break;
						case 1 :	sprintf( temp, lan->OutputMessage(2,183) );					break;
						default: 	sprintf( temp, lan->OutputMessage(2,184) );							break;
					}
					strcpy( Hero->szChatMessage, temp );
					Hero->ChatDelay = 70;
			}break;	
		}				
  }
	AgainSkill( p->u.server_skill_result.skillno );
}
			
void RecvSkillResultFail( t_server_skill_result_fail *p )
{
	ItemAttr item;
	char temp[ FILENAME_MAX];
	
	StopDoingSkillCursor( Hero );
	
	InsertMagic( Hero, Hero, 250, (rand()%2) + 5, Hero->x, Hero->y, 0, 0);
	PlayListAutoSounds( 2409, 0, 0, 0 );
	
	SkillSettingClear();

/*	switch ( p->why )
	{
		default : 
			{
*/				GameStatusOutput( 1 );	
				if(p->why!=0)
				{
					AddCurrentStatusMessage( 255,255 ,0,  lan->OutputMessage(2,abs(p->why)));//020526 lsw
				}
/*			}
			break;
	}
*/

	skill_fail(p->skillno);

	switch( p->skillno )
	{
	case 21 :	sprintf( temp, lan->OutputMessage(2,405) );
				strcpy( Hero->szChatMessage, temp );
				Hero->ChatDelay = 70;
				AddSkillGauge( SkillNo, 0, 0, 999 );
			break;	

	case 1	:		
	case 25 :		
	case 35 : 	switch( rand()%3)
				{
					case 0 :	sprintf( temp, lan->OutputMessage(2,186) ); break;
					case 1 :	sprintf( temp, lan->OutputMessage(2,187) );	break;
					default: 	sprintf( temp, lan->OutputMessage(2,188) );	break;
				}	
				strcpy( Hero->szChatMessage, temp );
				Hero->ChatDelay = 70;
				SkillSettingClear();
			break;		

	default :	switch( rand()%3 )
				{
				case 0 : 
				case 1 : sprintf( temp, lan->OutputMessage(2,189) );			break;
				case 2 : sprintf( temp, lan->OutputMessage(2,190) );			break;
				}
				strcpy( Hero->szChatMessage, temp );
				Hero->ChatDelay = 70;	
	
				memset( &item, 0, sizeof( ItemAttr ) );
				QuickItemAttr[6]= item;
	
				SkillSettingClear();

		break;
	}
	
	AgainSkill( p->skillno );
}	
	
extern DWORD g_skillmaketime;
void RecvSkillItemMakeResult( t_server_skill_itemmake_result *p )
{									
	ItemAttr item = p->item;	
	
	// 250 ����.. 
	InsertMagic( Hero, Hero, 250, (rand()%4) + 1, 0,0,Hero->x,	Hero->y, 0,0 );
	PlayListAutoSounds( 2408, 0, 0, 0 );
	CursorNo( 1 );

	QuickItemAttr[6]= item;
	
	nItemOther = MN_ITEM_MAKE_OK;
	
	CItem *t = ItemUnit( item.item_no/1000, item.item_no%1000 );
	if( !t ) return ;
	AddCurrentStatusMessage( 255,255 ,0,  lan->OutputMessage(2,224), t->GetItemHanName(),p->TryItemCount ,p->MakeItemCount );			// 0721 YGI

	if( p->TryItemCount > 1)
	{
	//011025 lsw >
		g_skillmaketime = timeGetTime() + (p->TryItemCount-1)*1500;
	//011025 lsw <
	}
	SkillSettingClear();
}

static DWORD SkillResultFromOtherTime;	// �߻��� �ð�.
static int	SkillResultFromOtherDelay;	// ���ʵڿ� Message�� ����ұ�..
static char SkillResultFromOther_Msg[ MAX_PATH]; // ����� Message..


// �������� Delay�Ŀ� ����Ѵ�. 
// ������ 1���� ����Ѵ�. 
void RecvSkillResultFromOther( t_server_skill_result_toother *tp )
{
	LPCHARACTER ch = ReturnCharacterPoint( tp->id );	// 
	if( ch == NULL ) return;

	switch( tp->skillno )
	{
	case 19 : // stealing
		switch( tp->result )
		{
		case FALSE :
			{
				SkillResultFromOtherTime = g_curr_time;
				SkillResultFromOtherDelay= 5;
				sprintf( SkillResultFromOther_Msg, lan->OutputMessage(2,192), ch->name );
			}break;
		
		case 1 :
			{
				SkillResultFromOtherTime = g_curr_time;
				SkillResultFromOtherDelay= 5;
				sprintf( SkillResultFromOther_Msg, lan->OutputMessage(2,193), ch->name );
			}break;
		}
		break;
	}
}

// �����ð��� ������ ����ص� Message�� ����Ѵ�. 
void CheckTime_SkillMsg( void )
{
	if( SkillResultFromOtherTime == 0 ) return;

	if( g_curr_time - SkillResultFromOtherTime > SkillResultFromOtherDelay )
	{
		AddCurrentStatusMessage( 255,0,0, SkillResultFromOther_Msg );
		SkillResultFromOtherTime = 0;
	}
}

	
	
extern int before_skill_no;
int SelectSkill( int skillno )
{
	before_skill_no = skillno;			// �ٷ� ���ܰ� ��ų�� ������ �ΰ� �ִ´�.		// 001120
	if( SkillStatus == SKILL_OFF )
	{
	}
	else 
	{
		//1006 zhh
		return 1;
	}

  if (Hero->nCurrentAction==17) 
	{
		AddCurrentStatusMessage( 255,255,0,lan->OutputMessage(0,343));
		return 1;			// LTS SITDOWN
	}

	//1004 zhh
	if(AmIEquitSkillTool(skillno)==0)
		return(1);

	SkillIcon		= &skill_spr[ skillno];
	SkillStatus		= SKILL_ON;
	SkillNo			= skillno;

	CursorNo( SkillTable[ SkillNo].Curser1 );

	switch(skill[SkillNo].series) //010222 lsw
	{
		case 0:
			AddCurrentStatusMessage( 255,100,255, lan->OutputMessage(2,35) );//�� ��� ��� �غ��մϴ�.			
		break;

		case 1:

			AddCurrentStatusMessage( 255,100,255, lan->OutputMessage(2,36) );//�� Ŭ������� ��� �غ��մϴ�.			
		break;
	}


	switch( SkillNo )//  �밡�ٰ� �ʿ��� ����� ���� �ʱ�ȭ�� �Ѵ�. 
	{
		//  ����..
	case 21	://Reclaim
	case 23	://Cultivate
	case 24	://Havest
	case 26	://Mining
	case 32	://Cutting
	case 36	://Digging
			break;

	case 65	: // Snooping
				AddCurrentStatusMessage( 255, 100, 100, lan->OutputMessage(4,100) );//lsw
			break;
	}			
																		
//	CursorSpriteNo( 879 );
	return 0;								
}
											
int CheckSkill( void )
{		
	int x,y;
	int dis;
	int mx, my;				
				
	if( IsDead( Hero ) ) return 0;
				
	if( SkillStatus == SKILL_OFF )							return 0;

	static DWORD time = timeGetTime();
	if( SkillStatus == SKILL_ING )
	{
		if( timeGetTime() - time < PACKET_SEND_MINIMUM_TIME*2 ) return 0;		// 010815 YGI
		time = timeGetTime();
	}
	else
	{
		time = timeGetTime();
	}
				
	//1004 zhh
	if(AmIEquitSkillTool(SkillNo)==0)
		return(1);

	if( SkillNo >= MAX_SKILL_ ){ SkillStatus = SKILL_OFF;	return 0;	}
	int ret = false;
				
	if( Moy - Mapy > GAME_SCREEN_YSIZE ) return 0;
				
	x = Hero->x;
	y = Hero->y;
	//010204 lsw

				
	if( Hero->peacests == 1 ) 
	{
		AddCurrentStatusMessage( 255,100,255, lan->OutputMessage(2,194) );
		SetBattlePeace( PEACE_MODE );
	}			
					
	//  �ʹ� �ָ� ������ �װ����� �̵��Ѵ�. 
	dis = (Mox-x)*(Mox-x)+(Moy-y)*(Moy-y);  
	
	//1017 zhh
	if( (SkillNo==28 || SkillNo==30) && dis<64*64*8 )
		dis=10;
	
	if( dis > 64 * 64 ) 
	{			
		if( ReturnXYNearPosition( &x, &y, &mx, &my, 64, Hero, Mox, Moy ))
		{		
			Hero->destx = mx, Hero->desty = my;
			if ( PathBuild( Hero ) )
			{	
#ifdef _DEBUG // finito 060507
				if( SysInfo.notconectserver )
				{
					if ( Hero->nCurrentAction != Hero->basicWalk )
					{
						Hero->moveFlag = TRUE;
						Hero->gox = x;
						Hero->goy = y;
						Hero->movetype = 0;
						Hero->nCurrentAction	= Hero->basicWalk;
						Hero->nCurrentFrame.PutEncrypted(0);
					}
				}
				else
#endif
				{
					Hero->moveFlag = FALSE;
					Hero->movetype = 0;
					Hero->gox = x;
					Hero->goy = y;
//					SendMoveData( Hero );
				}
			}	
			else
			{	
				ChangeDirection( &Hero->todir, Hero->x, Hero->y, x, y );
			}		
		}		
		else	
		{
			//1024 zhh
			AddCurrentStatusMessage( 0,155,255, lan->OutputMessage(2,195) );
		//	AddCurrentStatusMessage( 0,155,255, "�ʹ� �־� ����� ���� �����ϴ�." );
			PlayListAutoSounds( 82,0,0,0 ); // �̵�����.
			return 1;
		}		
	}
	

	int targettype = SkillTable[ SkillNo].Howto;
	int target_id = 0;
	int target_item_no = 0;
	char TargetName[31] = {0,};//020531 lsw
	switch( targettype )
	{
	case SKILLTARGETTYPE_TABLE_ :	return 0;
	case SKILLTARGETTYPE_ITEM_	:	if( CursorCheckedItem == NULL ) 
									{
										//1024 zhh
										AddCurrentStatusMessage( 125,100,255, lan->OutputMessage(2,196));
									//	AddCurrentStatusMessage( 125,100,255, "����� ���� Item�� Ŭ���ϼ��� .");
										return 0; 
									}
									target_id = CursorCheckedItem->server_id; 
									target_item_no = CursorCheckedItem->no;

									//1005 zhh
									Skill_CursorCheckedItem = CursorCheckedItem;
				break;
	case SKILLTARGETTYPE_CHAR_	:	if( g_GameInfo.lpcharacter == NULL ) 
									{
										//1024 zhh
										AddCurrentStatusMessage( 125,100,255, lan->OutputMessage(2,197) );
									//	AddCurrentStatusMessage( 125,100,255, "����� �� ĳ���͸� Ŭ���ϼ���." );
										return 0;
									}
									target_id = g_GameInfo.lpcharacter->id;
									memcpy(TargetName, g_GameInfo.lpcharacter->name,30);//020531 lsw
				break;

	case SKILLTARGETTYPE_MAP_	:	target_id = 0;   
				
				break;

	case SKILLTARGETTYPE_HERO_	:	if(	g_GameInfo.lpcharacter != Hero ) 
									{
										//1024 zhh
										AddCurrentStatusMessage( 125,100,255, lan->OutputMessage(2,198) );
									//	AddCurrentStatusMessage( 125,100,255, "�ڽſ��Ը� ����� �� �ִ� ����Դϴ�." );
										return 0;
									}
									target_id = Hero->id; 
				break;

	case SKILLTARGETTYPE_MAP_WITH_ITEM_:  
									if( SkillStatus != SKILL_READY ) 
									{
										return 0;
									}
									SendSkillInfo( SkillNo, Mox, Moy, targettype, SkillItemNo, SkillItemPOS );
				break;
	}
	
	switch( SkillNo )
	{
	case 19  :	if( Hero->id == target_id ) 
				{
					//1024 zhh
					AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,201) );
				//	AddCurrentStatusMessage( 255,255,255, "����� �ڽſ��� ����Ͽ����ϴ�. ����� �Ǽ��Ͽ� â���� �մϴ�." );
					return 1;
				}
				if( target_id < 10000 || g_GameInfo.lpcharacter->eventnpc )//soto-Steal����. ���Ϳ����� �����ϴ�.
				{
					//1024 zhh
					AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,202) );
				//	AddCurrentStatusMessage( 255,255,255, "��ġ�� ����� Player���Ը� ����� �� �ֽ��ϴ�." );
					return 1;
				}
				SendSkillInfo( SkillNo, Mox, Moy, targettype, target_id, target_item_no, TargetName);//020531 lsw
		break;

		// Harvest..
	case 24 :	{
					if( CursorCheckedItem == NULL ) break;

					CItem *t = ItemUnit( CursorCheckedItem->no/1000, CursorCheckedItem->no%1000 );	
					if( t )
					if( t->GetItemKind() == IK_SEED )
					{
						SendSkillInfo( SkillNo, Mox, Moy, targettype, CursorCheckedItem->server_id );
						return 1;	
					}
				}
		break;		
					
	case 28 :   //	����.. 
				if( TileMap[ Mox/TILE_SIZE][Moy/ TILE_SIZE].attr_river == 0 ) 
				{	
					//1024 zhh	
					AddCurrentStatusMessage( 255,155,155,lan->OutputMessage(2,203), SkillTable[SkillNo].Skill_Minute );
				//	AddCurrentStatusMessage( 255,155,155,"�̰��� ��Ⱑ ���°����� '%s'����� ����Ҽ� �����ϴ�.", SkillTable[SkillNo].Skill_Minute );
					goto SKILL_FAIL_;
				}	
				else
				{
					//SendSkillGauge( Mox, Moy, SkillNo );
					SendSkillInfo( SkillNo, Mox, Moy, targettype, target_id );
				}
		break;		
						
	default :	SendSkillInfo( SkillNo, Mox, Moy, targettype, target_id ,target_item_no,TargetName);//020531 lsw
			return 1;	
	}
					
	return ret;		
					
SKILL_FAIL_ :

	return 0;//Skill
}		

typedef struct tagDETECTTILE{	
	short x, y;
	short type;
	WORD color;
	short delay;

	struct tagDETECTTILE *prev, *next;
}DETECTTILE, *LPDETECTTILE;


LPDETECTTILE g_DetectTile;


void DeleteDetectTile( LPDETECTTILE tt )
{		
	LPDETECTTILE  t = g_DetectTile;
			
	while( t != NULL )
	{		
		if( t == tt )
		{	
			if( t == g_DetectTile )
			{	
				t = g_DetectTile->next;
				MemFree( g_DetectTile );

				g_DetectTile = t;
				if( t != NULL )	g_DetectTile->prev = NULL;
				return;
			}
			else 
			{
				if ( t->prev != NULL )
					t->prev->next = t->next;

				if( t->next != NULL )
					t->next->prev = t->prev;

   				MemFree( t );
				return;
			}
		}
		t = t->next;
	}		
}		


LPDETECTTILE AddDetectTile( int type, int x, int y, WORD color )
{			
	LPDETECTTILE temp = NULL, t;
	MemAlloc( temp, sizeof( DETECTTILE ));
			
	temp->delay		= 80;
	temp->type		= type;
	temp->x			= x * TILE_SIZE + 16 ;
	temp->y			= y * TILE_SIZE + 16 ;
	temp->color		= color;
	temp->prev		= NULL;
	temp->next		= NULL;
				
	if( g_DetectTile == NULL )
	{		
		g_DetectTile			= temp;
	}		
	else	
	{		
		t					= g_DetectTile;
		g_DetectTile		= temp;
		g_DetectTile->next	= t;
		t->prev				= g_DetectTile;
	}												
												
	return g_DetectTile;								
}														
void OutputDetectTile( void )							
{											
	LPDETECTTILE t, temp;										
	LPORDERTABLE o;								
												
	t = g_DetectTile;										
	while( t != NULL )									
	{									
		t->delay --;									
		if( t->delay <= 0 ) { temp = t->next; DeleteDetectTile( t ); t = temp; continue; }
														
		if( BoxAndDotCrash( Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, t->x, t->y ) )
		{															
			o = &g_OrderInfo.order[ g_OrderInfo.count ];
														
			o->wType		= SPRITETYPE_DETECT_TILE;
			o->x			= t->x;
			o->y			= t->y;
			o->height		= 0;
			o->offset		= t->y;
			o->lpvData		= (LPVOID)t;
			o->show			= 1;
			o->sort			= 1;
			g_OrderInfo.count++;
		}										
		t = t->next;
	}		
}			
	
	
/////////////
/////////////
/////////////			Number Animation.....
/////////////			Number Animation.....
/////////////			Number Animation.....
/////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
//	ȭ���ܿ� ���� ��Ȳ�� �����Ѵ�. 
void GameStatusOutput( int no )
{
	static char text[ FILENAME_MAX];
	char temp[ FILENAME_MAX];

//	static int oldno = -1;
	char *token;
	int tno;
	FILE *fp;

//	if( no == oldno ) goto NEXT_;
//	oldno = no;

	fp = Fopen( "./data/gamestatusexplain.txt", "rt" );
	if( fp == NULL ) return;

	while( !feof( fp ) )
	{
		fgets( temp, FILENAME_MAX, fp );

		token = strtok( temp, "\t\n");
		tno = atoi( token );
		if( tno == no )
		{
			token = strtok( NULL, "\t\n");
			strcpy( text, token );
			fclose(fp);

			if( strchr( text, '%' ) )
			{
				sprintf( temp, text, SkillOutputMessage );
				AddCurrentStatusMessage( 255,0,0, temp );
			}
			else
			{
				AddCurrentStatusMessage( 255,0,0, temp );
			}
			return;
		}
	}
	fclose(fp);
}		
	
// Bomb...								
void SendCMD_FOOT_ON_TRAP_0_ITEM( int server_id )
{				
	t_packet p;	
	p.h.header.type = CMD_FOOT_ON_TRAP_0_ITEM;
  p.u.client_foot_on_trap0.idServer = server_id;
	p.h.header.size = sizeof(t_client_foot_on_trap0 );
	QueuePacket( &p, 1 );
}				

void RecvCMD_STEAL_ON_TRAP( t_server_steal_on_trap	*p )
{						
	LPCHARACTER ch = ReturnCharacterPoint(p->idServer);
	if( ch == NULL ) return;
	int no;

	int t = rand()%4 +1;
	for( int j = 0 ; j < t; j ++)
		InsertMagic( ch, ch, 287,0, ch->x+(rand()%32) - 16, ch->y+ (rand()%32)-16, 0,0 );

	if( ch== Hero ) 
	{
		no = SCharacterData.nCharacterHP - p->nLife;
		SCharacterData.nCharacterHP = p->nLife;

		Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
	}
	else 
	{
		no = ch->hp - p->nLife;
		ch->hp = p->nLife;
	}

	SendReqHpOfParty(ch->name);
	AddNumberAni( NUMBER_ANIMATION_TYPE_DOWN_, ch->y+1, ch->x, ch->y, no );

	//1013 zhh
	if( ch->hp > 0 )
	{
		if( ch->sprno <= 1 )
		{
			ch->nCurrentAction = ACTION_ATTACKED;
			ch->nCurrentFrame.PutEncrypted(0);
		}
		else
		{
			ch->nCurrentAction = MON1_ATTACKED;
			ch->nCurrentFrame.PutEncrypted(0);
		}
	}
}
void RecvCMD_FOOT_ON_TRAP_0_ITEM( t_server_foot_on_trap0 *p )
{						
	LPCHARACTER ch = ReturnCharacterPoint(p->idServer);
	if( ch == NULL ) return;
	LPITEMGROUND i = FindItemList(p->idItem);
	if( i == NULL ) return;
	int no;

	int t = rand()%4 +1;
	for( int j = 0 ; j < t; j ++)
		InsertMagic( ch, ch, 287,0, i->x+(rand()%32) - 16, i->y+ (rand()%32)-16, 0,0 );

	if( ch== Hero ) 
	{
		no = SCharacterData.nCharacterHP - p->nLife;
		SCharacterData.nCharacterHP = p->nLife;

		Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
	}
	else 
	{
		no = ch->hp - p->nLife;
		ch->hp = p->nLife;
	}
	no = i->attr[IATTR_DURATION];//021030 lsw
	SendReqHpOfParty(ch->name);
	AddNumberAni( NUMBER_ANIMATION_TYPE_DOWN_, ch->y+1, ch->x, ch->y, no );


	//1013 zhh
	if( ch->hp > 0 )
	{
		if( ch->sprno <= 1 )
		{
			ch->nCurrentAction = ACTION_ATTACKED;
			ch->nCurrentFrame.PutEncrypted(0);
		}
		else
		{
			ch->nCurrentAction = MON1_ATTACKED;
			ch->nCurrentFrame.PutEncrypted(0);
		}
	}
}				
// Trap...		
void SendCMD_FOOT_ON_TRAP_1_ITEM( int server_id )
{	
	t_packet p;
	p.h.header.type = CMD_FOOT_ON_TRAP_1_ITEM;
  p.u.client_foot_on_trap1.idServer = server_id;
	p.h.header.size = sizeof(t_client_foot_on_trap1 );
	QueuePacket( &p, 1 );
}	

void RecvCMD_FOOT_ON_TRAP_1_ITEM( t_server_foot_on_trap1 *p )
{	
	LPCHARACTER ch = ReturnCharacterPoint(p->idServer);
	if( ch == NULL ) return;
	LPITEMGROUND i = FindItemList(p->idItem);
	if( i == NULL ) return;
	int no;

//	int t = rand()%4 +1;
//	for( int j = 0 ; j < t; j ++)
//		InsertMagic( ch, ch, 208,0, i->x+(rand()%32) - 16, i->y+ (rand()%32)-16, 0,0 );

	InsertMagic( ch, ch, 288,0, i->x+(rand()%32) - 16, i->y+ (rand()%32)-16, 0,0 );
	
	if( ch== Hero ) 
	{
		no = SCharacterData.nCharacterHP - p->nLife;//���ξ�
		SCharacterData.nCharacterHP = p->nLife;

		Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
	}
	else 
	{
		no = ch->hp - p->nLife;
		ch->hp = p->nLife;
	}
	
	no = i->attr[IATTR_DURATION];//021030 lsw
	SendReqHpOfParty(ch->name);
	AddNumberAni( NUMBER_ANIMATION_TYPE_DOWN_, ch->y+1, ch->x, ch->y, no );

	if( ch->hp > 0 )
	{
		if( ch->sprno <= 1 )
		{
			ch->nCurrentAction = ACTION_ATTACKED;
			ch->nCurrentFrame.PutEncrypted(0);
		}
		else
		{
			ch->nCurrentAction = MON1_ATTACKED;
			ch->nCurrentFrame.PutEncrypted(0);
		}
	}
}	
	
void RecvViewtypeChange( t_viewtype_change *p )
{	
	LPCHARACTER	ch = ReturnCharacterPoint( p->server_id );
	if( ch == NULL ) return;
	ch->viewtype = p->viewtype;
	
	switch( ch->viewtype )
	{	
		////////////////// 1220 �̱��� ///////////////////////
		case VIEWTYPE_CONFUSION_: break;
		case VIEWTYPE_TREE_		: break;
		case VIEWTYPE_GHOST_	:	
			{
				ch->ghostani = rand()%10 + 15;	
				ch->peacests = 0;

				if( Hero == ch )	// 0711 YGI
				{
					SetBattlePeace( PEACE_MODE );
					SmallMenuClose();
					// �����϶� �޴� Ŭ����
					CSDMainIconClear(); // CSD-TW-030627

					if(Hero->lv <= 20)//������ ��Ƴ��� 020627 lsw
					{
						if( MapNumber!=85 && MapNumber!=30)//�ʺ��� ���üҰ� �ƴ϶�� //020827 lsw
						{
							CallSmallMenu(MN_GHOST_TO_ALIVE);
						}
					}
				}

				SettingBasicActionSub( ch );
				ch->nCurrentAction = ch->basicAction;
				
				CharDoAni( ch, ch->direction, ch->basicAction);
				
			}break;

		case VIEWTYPE_CAMELEON_ : 
			break;
	}
}	
	

void RecvViewCharInfo( t_view_char_info *p )
{
	char temp[ FILENAME_MAX];
	int id = 	p->server_id;

	LPCHARACTER ch = ReturnCharacterPoint( id );
	if( ch == NULL ) return;
	
	SkillSettingClear();

	switch( p->type )
	{
		case WIS :	if( ch->name[0])
					{
						//1024 zhh
						AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,204), ch->name, p->data );
						sprintf( temp, lan->OutputMessage(2,205), ch->name, p->data  );
					//	AddCurrentStatusMessage( 255,255,255, "%s�� ������ %d�Դϴ�. ", ch->name, p->data );
					//	sprintf( temp, "%s�� ����� %d���� �ǰڱ�...", ch->name, p->data  );
					}
					else
					{
						//1024 zhh
						AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,206), p->data );
						sprintf( temp, lan->OutputMessage(2,207), p->data  );
					//	AddCurrentStatusMessage( 255,255,255, "�˾ƺ���, ������ %d�Դϴ�. ", p->data );
					//	sprintf( temp, "����� %d���� �ǰڱ�...", p->data  );
					}
					strcpy( Hero->szChatMessage, temp );
					Hero->ChatDelay = 70;
			break;
		case INT_ :	if( ch->name[0])
					{
						//1024 zhh
						AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,208), ch->name, p->data );
						sprintf( temp, lan->OutputMessage(2,209), ch->name, p->data  );
					//	AddCurrentStatusMessage( 255,255,255, "%s�� ���ɴ� %d�Դϴ�. ", ch->name, p->data );
					//	sprintf( temp, "%s�� ���ɴ� %d���� �ǰڱ�...", ch->name, p->data  );
					}
					else
					{
						//1024 zhh
						AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,210), p->data );
						sprintf( temp, lan->OutputMessage(2,211), p->data  );
					//	AddCurrentStatusMessage( 255,255,255, "�˾ƺ���, ���ɴ� %d�Դϴ�. ", p->data );
					//	sprintf( temp, "���ɴ� %d���� �ǰڱ�...", p->data  );
					}
					strcpy( Hero->szChatMessage, temp );
					Hero->ChatDelay = 70;
			break;
			// ���� ��� ��Ȳ..
		case 9999:	if( p->killer == NULL )
					{
						NNEXTT_:
						//1024 zhh
						AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,212) );
						sprintf( temp, lan->OutputMessage(2,213) );
					//	AddCurrentStatusMessage( 255,255,255, "���� ����� ��Ȳ�� �ľ��� ���� �ʽ��ϴ�. " );
					//	sprintf( temp, "�� �׾����� �� �𸣰ھ�." );
					}
					else
					{
						if( p->killer[0] == 0 ) goto NNEXTT_;

						if( ch->name[0] )
						{
							//1024 zhh
							AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,214), ch->name, p->killer );
							sprintf( temp, lan->OutputMessage(2,215), p->killer );
						//	AddCurrentStatusMessage( 255,255,255, "%s��(��) %s�� ���� ������ ���߽��ϴ�.", ch->name, p->killer );
						//	sprintf( temp, "%s�� ���Ѱ� ����.", p->killer );
						}
						else
						{
							//1024 zhh
							AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(2,216), p->killer );
							sprintf( temp, lan->OutputMessage(2,217), p->killer );
						//	AddCurrentStatusMessage( 255,255,255, "%s�� ���� ������ ���߽��ϴ�.", p->killer );
						//	sprintf( temp, "%s�� ���Ѱ� ����.", p->killer );
						}
					}
					strcpy( Hero->szChatMessage, temp );
					Hero->ChatDelay = 70;
				
			break;
	}
}


//  Skill�ϰ� ���� �ٽ� ��ư�� �����ʿ���� �ݺ��Ҽ� �ְ� �Ѵ�.
void AgainSkill( int skillno )
{
	switch( skillno )
	{
	case	4 :
	case	5 :
	case	16 :
	case	21 :
	case	23 :
	case	24 :
	case	25 :
	case	26 :
	case	28 :
	case	30 :
	case	32 :
//	case	34 :
	case	35 :
	case	36 :
	case	38 :
	case	39 :
	case	41 :	
		{

					//1004 zhh
					if(AmIEquitSkillTool(skillno)==0)
						break;
					SelectSkill( skillno );
					SkillStatus = SKILL_ON;
					//1007 zhh ����� �������� ��� �������� ������ ���� ���´٤�
					int para = 1;
					if( skill[skillno].series==1 ) para = 0;					
					SkillOn = para;
		}
		break;

		//
	case	22 :	//   ����   �������� ���� �ִٸ�..
		{
					//1004 zhh
					if(AmIEquitSkillTool(skillno)==0)
						break;
					
					SelectSkill( skillno );
					SkillStatus = SKILL_READY;  // SelectSkill() �ؿ��;� �Ѵ�. 
					//1007 zhh ����� �������� ��� �������� ������ ���� ���´٤�
					int para = 1;
					if( skill[skillno].series==1 ) para = 0;					
					SkillOn = para;
		}
		break;
	}
}




void PutSkillUseItem_skill_use_item( int skillno  )
{
	int c;
	int ino;
	CItem *ii;

	switch( skillno )
	{
		// Sow..
		case 22 :	
		{
			c = 0;
			for( int i= 0 ; i < 3 ; i ++)
				for( int j= 0 ; j < 3 ; j ++)
					for( int k= 0 ; k < 8 ; k ++)
					{
						ino = InvItemAttr[i][j][k].item_no;
						ii = ItemUnit( ino / 1000, ino % 1000 );
						if( ii )
							if( ii->GetItemKind() == IK_SEED )
							{
								skill_use_item[c/10][c%10].item_no = ino;
								SetItemPos( INV, i,j,k, &skill_use_item[c/10][c%10].pos );
								c++;
							}
					}

			//  Quick..
			for( int k= 0 ; k < 5 ; k ++)
			{
				ino = QuickItemAttr[k].item_no;
				ii = ItemUnit( ino / 1000, ino % 1000 );
				if( ii )
				{
					if( ii->GetItemKind() == IK_SEED )
					{
						skill_use_item[c/10][c%10].item_no = ino;
						SetItemPos( QUICK, k, &skill_use_item[c/10][c%10].pos );
						c++;
					}
				}
			}
		}
		break;
	}
}


void DrawSkilltile( void  )
{
	int x, y;
	int skillno = SkillNo;
	int mx = Mox / TILE_SIZE;
	int my = Moy / TILE_SIZE;
	int rx, ry;
	int r;
	int mapx, mapy;
	DWORD		lineColor;
	lpMAPSKILLTABLE	result;

	if( SkillStatus == SKILL_OFF ) return;

	switch( skillno )
	{
	case 21	://Reclaim	
	case 22	://Sow	
	case 23	://Cultivate	
	case 24	://Havest	
					skillno = 1;
					lineColor=RGB16(221,189,140);
					
			break;

	case 25	://Detect Mineral	
	case 26	://Mining	
					skillno = 2;
					lineColor=RGB16(203,185,156);
					
			break;
		
	case 28	://Fish 
	case 30	://Netting	
					skillno = 3;
					lineColor=RGB16(118,100,85);
			break;

	case 32	://Cutting	
	case 33	://Lumbering	
					skillno = 4;
					lineColor=RGB16(197,79,0);
			break;

	case 35	://Detect Herb	
	case 36	://Digging	
					skillno = 5;
					lineColor=RGB16(137,202,157);
			break;

	default : return;
	}

	

	mapy = g_Map.y;
	
	for ( y = 0; y < GAME_SCREEN_YSIZE ; y += TILE_SIZE, mapy ++ )
	{	
		for ( mapx = g_Map.x, x = 0; x < GAME_SCREEN_XSIZE ; x+= TILE_SIZE, mapx ++ )
		{
			result=FindSkill(&Header[(int)(mapx/(int)((g_Map.file.wWidth+7)/8))][(int)(mapy/(int)((g_Map.file.wHeight+7)/8))],mapx,mapy);
			if( result )
			{
				if( result->skillno == skillno  )
				{
					r = result->tile_Range;
					if( BoxAndDotCrash( mapx - r,  mapy - r, r+r, r+r, mx, my ))
					{
						rx=(mapx-r)*TILE_SIZE;
						ry=(mapy-r)*TILE_SIZE;
						r = (r+r)*TILE_SIZE + TILE_SIZE;

						Box( rx-1, ry-1, rx+r+1, ry+r+1, lineColor);
						Box( rx  , ry  , rx+r  , ry+r  , lineColor);
						Box( rx+1, ry+1, rx+r-1, ry+r-1, lineColor);

					
					return;
					}
				}
			}
		}
	}
}	
	
	
	
void DrawSkillGauge( LPSKILLGAUGE n )
{	
	Spr *sp, *g;
	int sx, sy, ex, ey;
	int x,y;
	int gx,gy;

	sp = &spr[ n->type];
	g =  &spr[732];

	x = n->x - Mapx;
	y = n->y - Mapy - 100;

	gx = -g->xl/2;
	gy = g->yl/2;

	sx = x - g->xl/2;
	sy = y - g->yl/2;
	ex = sx + g->xl * n->min / 100;
	ey = sy + g->yl;

	//PutCompressedImage( x, y + 50, g );
	// sx, sy, ex, ey

	switch( n->type )
	{
	case SKILLGAUGE_RESTTIME	: 
		// ��Ȯ��					
	case SKILLGAUGE_HAVEST		:
		// �ΰ��������				
	case SKILLGAUGE_CULTIVATE	:
		// ���õ�					
	case SKILLGAUGE_TAME		:
		// ����������				
	case SKILLGAUGE_RECLAIM		:	
		// ������					
	case SKILLGAUGE_SOWING		:

		//1018 zhh		
	case SKILLGAUGE_FISHING :

		break;
	}


	


	//PutCompressImageApart( sx, sy+gy, g, 0,0, 640,480, n->delay );
	if( n->delay > 31 )		PutCompressedImage( x, y, sp );
	else PutCompressedImageFX( x, y, sp, n->delay, 1 );

	PutCompressImageApart( sx, sy+gy, g, 0, 0, ex, SCREEN_HEIGHT, n->delay );

}	



void DeleteSkillGauge( LPSKILLGAUGE tt )
{		
	LPSKILLGAUGE  t = g_SkillGauge;
			
	while( t != NULL )
	{		
		if( t == tt )
		{	
			if( t == g_SkillGauge )
			{	
				t = g_SkillGauge->next;
				MemFree( g_SkillGauge );

				g_SkillGauge = t;
				if( t != NULL )	g_SkillGauge->prev = NULL;
				return;
			}
			else 
			{
				if ( t->prev != NULL )
					t->prev->next = t->next;

				if( t->next != NULL )
					t->next->prev = t->prev;

   				MemFree( t );
				return;
			}
		}
		t = t->next;
	}		
}		



LPSKILLGAUGE FindSkillGauge( int type )
{
	LPSKILLGAUGE t;

	t = g_SkillGauge;
	while( t != NULL )
	{
		if( t->type == type) return t;
		t = t->next;
	}

	return t;
}



//  ���  type�� ���Ͽ�  ó�� ȣ��Ǹ� Gage�� �׸��Եȴ�. 
//  �׸��� �ٽ� �� type�� ȣ��Ǹ� 

LPSKILLGAUGE AddSkillGauge( int skillno, int x, int y, int min )
{
	LPSKILLGAUGE temp = NULL, t;
	int type;

	switch( skillno )	
	{						
		case 26	://Mining	
		case 32	://Cutting	
		case 36	://Digging		
					type = SKILLGAUGE_RESTTIME;
				break;		
								
		case 24	://Havest		
					type = SKILLGAUGE_HAVEST;
				break;		
							
		case 75 : 	// ���õ�
					type = SKILLGAUGE_TAME;
				break;		
							
		case 21	://Reclaim		
					type = SKILLGAUGE_RECLAIM;
				break;		
							
		case 22	://Sow		
					type = 	SKILLGAUGE_SOWING;
				break;	

				//1018 zhh
		case 28:
		case 30:
				type = SKILLGAUGE_RESTTIME;
			//	type = 	SKILLGAUGE_FISHING;
					break;	

		default : return NULL;
	}						

	t = FindSkillGauge( type );
	if( t )
	{	if( min == 999 )  // �׳� �����ּ���...
		{
			t->min = min;
			return NULL;
		}
		else		// ������� �������� �����ּ���..
		{
			t->delay = 75;
			t->min = min;
//			t->max = min;
			t->toggle = true;
			//1018 zhh
			t->x = x;
			t->y = y;
			return NULL;
		}
	}

	MemAlloc( temp, sizeof( SKILLGAUGE ));

	temp->skillno   = skillno;
	temp->type		= type;
	temp->delay		= 0;
	temp->x			= x;
	temp->y			= y;
	temp->off		= 0;
	temp->min		= min;
	temp->max		= 100;
	temp->toggle    = false;//true;

	temp->prev		= NULL;
	temp->next		= NULL;

	if( g_SkillGauge == NULL )
	{	
		g_SkillGauge		= temp;
	}	
	else
	{	
		t					= g_SkillGauge;
		g_SkillGauge		= temp;
		g_SkillGauge->next	= t;
		t->prev				= g_SkillGauge;
	}		
			
	return g_SkillGauge;
}					
void OutputSkillGauge( void )
{	
	LPSKILLGAUGE t, temp;
	LPORDERTABLE o;
		
	t = g_SkillGauge;
	while( t != NULL )
	{	
		//t->off ++;
		if( t->toggle == false ) // ����....
		{
			t->delay +=3;
			if( t->delay > 400 ) t->delay = 400;
		}
		else t->delay --; 

//		if( t->max > t->min ) t->min ++;
		 
		//  �����ϰų� .. �ʹ����� �ð��� �帥���̸�..( ���� 400 frame )
		if( t->delay <= 0 || t->min == 999 ) { temp = t->next; DeleteSkillGauge( t ); t = temp; continue; }
		
		if( BoxAndDotCrash( Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, t->x, t->y ) )
		{		
			o = &g_OrderInfo.order[ g_OrderInfo.count ];
					
			o->wType		= SPRITETYPE_SKILLGAUGE;
			o->x			= t->x;
			o->y			= t->y;
			o->height		= t->off;
			o->offset		= t->y;
			o->lpvData		= (LPVOID)t;
			o->show			= 1;
			o->sort			= 1;
			g_OrderInfo.count++;
		}
		t = t->next;
	}	
}		
//   �밡�� ��� ������ ���콺��ǥ�� ������ ����� �̸� �������ش�. 
void AdjustSkillMouseCursor( int *x, int *y )
{		
	int sx, sy, ex, ey;


	return;


//	if( SkillMouseDontMoveFlag == true )
	{	
//		sx = SkillDontMoveSx - Mapx;
//		sy = SkillDontMoveSy - Mapy;
//		ex = sx + SkillDontMoveXl - 2;
//		ey = sy + SkillDontMoveYl - 2;
		
		//  A tale that wasn't right -- Hallowin
		if( BoxAndDotCrash( sx, sy, ex-sx, ey-sy, *x, *y ))
		{ 
		}
		else
		{
			if( *x < sx+3 ) *x = sx;
			if( *y < sy+3 ) *y = sy;
			if( *x > ex ) *x = ex;
			if( *y > ey ) *y = ey;
													
		}
	}
}	
void SendSkillGauge( int x, int y, int skillno )
{	
	t_packet p;
		
	p.h.header.type = CMD_SKILL_GAUGE;
	{	
		p.u.client_skill_gauge.skillno = skillno;
		p.u.client_skill_gauge.x = x;
		p.u.client_skill_gauge.y = y;
	}	
	p.h.header.size = sizeof( t_client_skill_gauge );
	
	QueuePacket( &p, 1 );
}	
void RecvSkillGauge( t_server_skill_gauge *p )
{	
	AddSkillGauge( p->skillno, p->x, p->y, p->min);
}


//1024 zhh
//1004 zhh		//� �������� �������� �ʾҴ��� ǥ���Ѵ�
int IDontEquitSkillTool(int Skillno)	//void RecvSkillResultFail( t_server_skill_result_fail *p )���� ������ ������ �׷��� ����
{
	//1024 zhh
	if(SkillTable[ Skillno].Tool!=0)
		AddCurrentStatusMessage( 255,255 ,0,  lan->OutputMessage(2,SkillTable[Skillno].Tool) );
	return(0);
}

//1004 zhh
int AmIEquitSkillTool(int Skillno)		//selectskill���� ���� ����� �������� �������� �����ϰ� �ִ��� üũ�ϴ� ��ƾ.
{
	
	switch(SkillTable[ Skillno].Tool)
	{
	case -1:	break;
	case 0:		break;
	default:
		{//���⿡�� SkillTable[ Skillno].Tool�� ������ ������ ������ ������
			int tool;
			tool=EquipItemAttr[WT_WEAPON].item_no;
			CItem_Tool *weapon	= (CItem_Tool *)ItemUnit( tool/1000 , tool%1000 );
			int type=0;
			if(weapon!=NULL)
				type=weapon->GetItemKind();
			if(weapon!=NULL && type == SkillTable[ Skillno].Tool )//������ �ʿ��ϰ� ���� �����ϰ� ������
			{
				return(2);
			}
			else
			{
				IDontEquitSkillTool(Skillno);		//� �������� �������� �ʾҴ��� ǥ���Ѵ�
				return(0);//�������� ����� �������� �ʾҴ�.
			}
		}
		break;
	}
	return(1);
}

//1013 zhh
#define HP_GAUGE_DOT_MAX	40
extern int PutGrowingGauge( int x, int y);

//1013 zhh
int PutGrowingGauge( int x, int y)
{
	int gauge;

	switch(CursorCheckedItem->Anitype)
	{
	case ITEMANIMATION_FARM0: gauge = 4; break;
	case ITEMANIMATION_FARM1: gauge = 8; break;
	case ITEMANIMATION_FARM2: gauge = 12; break;
	case ITEMANIMATION_FARM3: gauge = 16; break;
	case ITEMANIMATION_FARM4: gauge = 20; break;
	case ITEMANIMATION_FARM5: gauge = 24; break;
	case ITEMANIMATION_FARM6: gauge = 28; break;
	case ITEMANIMATION_FARM7: gauge = 32; break;
	default: gauge = 0; break;
	}
	
	WORD rgb;
//	rgb = RGB16( 0, 0, 255 );					// �������� ���� ������,
rgb = RGB16( 267 - (gauge *233) / 30 , 0 ,  (gauge *233) / 30 );	
	if(CursorCheckedItem->Anitype==ITEMANIMATION_FARM7)
		rgb = RGB16( 0 , 255 , 0 );					// �������� ���� ������,

	

	for( int count=0; count<gauge; count+=2 )
		for( int a=0; a<2; a++ )
			for( int b=0; b<2; b++ )
			{
				//1013 zhh
				PutDot( x+a+count-gauge/2-4, y+b, rgb );
			}

	return 1;
}



/* itemjoin ���� 

   Item�� ����� ���� 


  ����ġ ���.. 


  ����� ����ġ ���� ���........


	skill_exp[ 31]   <- skillmain Table/ ����.. 

	ch->skill_exp[ MotherSkillType ];

	MotherSkillExp;





	�� ����� ���� 


	����ġ�� ���� 31�Ǿ� �ְ� �̴� ( MotherSkillType  ) �����Ѵ�. 
		
	
	
	
	
	if( SkillTbl[    ].
	
	
  				(���� skill Lv - �ּ� �ʿ� Lv)  
	������ =	------------------------------    
				(�ִ� �ʿ� Lv - �ּ� �ʿ� Lv)	
				
	
		(���� skill Lv
				  
	ch->skill_exp[ MotherSkillType ];				
	
	base = skillTbl[skillno].Ability_max - skillTbl[skillno].Ability_min;
	
	ch->skill_exp[ skillTMotherSkillType ];
	if( base )
	{
		hitrate =  skillTbl[skillTbl[skillno].Ability_min
	}
	
	if( ������ <= 0 ) ����.
	
	����ġ ������ ( ���� )
	
	ch->skillexp[ SkillTbl[skillno].MotherSkillType ] 	+=  ( 100 - ������ )  * SkillTbl[].Ability_Fix;
	
	
	ch->skill[ 
	
	
	
	
	
1. �ܼ� ���� ���� ����.....

		-> Table�� ���� ������ ����ġ�� ��´�. 
		-> �������� 0 - 100 ������ Lev�� �����ȴ�. 

2. 	Item���� �� ����� ���� ����ġ ��������... 

		
		������ 

				�ʿ��� skill1, skill2, skill3 ������ 0�� �ƴ� ��ġ�� ������ ������ �ʿ����̴�.
					skill2, skill3�� �ܼ� �ʿ������� �ȴ�. skill2_min, skill3_min���� �����ȴ�. 



				( item_join[].skill1_max - item_join[].skill1_min )



				if( item_join[].skill2 && item_join[].skill2_min >= ch->skill[ skillno]  )
				if( item_join[].skill3 && item_join[].skill2_min >= ch->skill[ skillno]  )

		

*/


//010205 lsw
int skill_fail(int skill)
{
	if(skill <= 341 && skill >=321)//1�� ������ ��� 
	{
		switch( rand()%2)
			{
				case 0 :	AddCurrentStatusMessage( 255,255 ,0,  lan->OutputMessage(2,406)); break;
				default: 	AddCurrentStatusMessage( 255,255 ,0,  lan->OutputMessage(2,407)); break;
			}
	}

	else if(skill <= 353 && skill >=343)//2�� ������ ��� 
	{
		switch( rand()%2)
			{
				case 0 :	AddCurrentStatusMessage( 255,255 ,0,  lan->OutputMessage(2,408)); break;
				default: 	AddCurrentStatusMessage( 255,255 ,0,  lan->OutputMessage(2,409)); break;
			}
	}

	else
	{
		AddCurrentStatusMessage( 255,255 ,0,  lan->OutputMessage(2,410));//����Ʈ
	}
	
	return 0;
}

void CheckDeleteSkill()
{
	if( SCharacterData.SkillId[46] )		// ���� ��ų�� ����ٸ�...
	{
		CallServer( CMD_CHECK_SKILL );
		SCharacterData.SkillId[46] = 0;
	}
}

void CMagic::Explain(const int iX, const int iY, const BOOL bSpell )
{
	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_SHORT_WIDTH;
	int iCount = 0;
	int iReultLineCount = 0;
	
	int iBlankGab = 0;
	int iReultBlankSize = 0;
	
	const int iLineGab = TxtOut.GetLineGab();
	const int iBlankGabSize = TxtOut.GetBlankGabSize();

#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) ) Hprint2( iX, iY-15, g_DestBackBuf, "%d", GetMagicNum() );
#endif
	
	switch(bSpell)
	{
	case WIZARD_SPELL:
		{	//����׸�
			TxtOut.Init(COLOR_BLUE,TXT_BG_SHORT_WIDTH);

			TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);

			Hcolor( FONT_COLOR_BLIGHT_BLUE );
			iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"<%s>", GetName() );
			iCount += iReultLineCount;

			Hcolor( FONT_COLOR_NUMBER );
			iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"Mana : %3d", g_lpMT[GetMagicNum()].exhaust_MP);
			Hcolor( FONT_COLOR_SPECIAL );
			Hprint( iX+1,iY+iBlankGab+iLineGab*iCount, g_DestBackBuf, "Mana :");
			iCount += iReultLineCount;
			
			Hcolor( FONT_COLOR_NUMBER );
			iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"Mp : %3d", g_lpMT[GetMagicNum()].exhaust_WS);
			Hcolor( FONT_COLOR_SPECIAL );
			Hprint( iX+1,iY+iBlankGab+iLineGab*iCount, g_DestBackBuf, "Mp :");
			iCount += iReultLineCount;
		}break;
	case PRIEST_SPELL:
		{	//����׸�
			TxtOut.Init(COLOR_GREEN,TXT_BG_SHORT_WIDTH);
			TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);

			Hcolor( FONT_COLOR_BLIGHT_GREEN );
			iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"<%s>", GetName() );
			iCount += iReultLineCount;

			Hcolor( FONT_COLOR_NUMBER );
			iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"Divine : %3d", g_lpMT[GetMagicNum()+150].exhaust_MP);
			Hcolor( FONT_COLOR_SPECIAL );
			Hprint( iX+1,iY+iBlankGab+iLineGab*iCount, g_DestBackBuf, "Divine :");
			iCount += iReultLineCount;
		}break;
	}	
	Hcolor( FONT_COLOR_WHITE );
	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,1,"%s",GetExplain());//����
	iCount += iReultLineCount;

	//�����׸�
	TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
}

void CSkill::Explain(const int iX, const int iY)//020815 lsw
{
	SetHangulAlign(TA_LEFT);
	const int iWidth = TXT_BG_SHORT_WIDTH;
	int iCount = 0;
	int iReultLineCount = 0;
	
	int iBlankGab = 0;
	int iReultBlankSize = 0;
	
	const int iLineGab = TxtOut.GetLineGab();
	const int iBlankGabSize = TxtOut.GetBlankGabSize();
	
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) ) Hprint2( iX, iY-20, g_DestBackBuf, "%d, %d, %d", num, inclusive, series );
#endif

	//����׸�
	TxtOut.Init(COLOR_BLACK,TXT_BG_SHORT_WIDTH);
	TxtOut.PutTextHeaderBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);

	Hcolor( FONT_COLOR_SKILL );
	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,0,"<%s>",han_name);
	iCount += iReultLineCount;

	if(	targettype !=SKILLTARGETTYPE_2ND_GM
	&&	targettype !=SKILLTARGETTYPE_SKILL_MASTER)//021122 lsw
	{
		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,0,"Ability : %3d  %4d",SCharacterData.nJobSkill[inclusive].skillexp/10000, SCharacterData.nJobSkill[inclusive].skillexp%10000 );//020815 lsw
		iCount+=iReultLineCount;//020815 lsw
		Hcolor( FONT_COLOR_SPECIAL );
		Hprint2( iX, iY+iBlankGab+iLineGab*iCount, g_DestBackBuf, "Ability :");
	}
	Hcolor( FONT_COLOR_WHITE );
	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlankGab+iLineGab*iCount,iWidth,0,"%s",explain);
	iCount += iReultLineCount;
	
	//�����׸�
	TxtOut.PutTextTailBGImg(iX,iY+iBlankGab+iLineGab*iCount,iWidth);
}

//011030 lsw >
void CSkill::ExplainSkillWhenMapOn(int x, int y)
{
	Hcolor( FONT_COLOR_SPECIAL );
	PutCompressedImage( x , y+12 , &spr[71] );
	Hprint( x+1 , y+13,  g_DestBackBuf, name );
}

int CSkill::CanLearnSkill( int guild )
{
	int check_ability = 1;		// �ɷ� üũ ���� ������
	if( series == 1 ) 
	{
		check_ability = 0;
		switch( inclusive )
		{
			//011012 lsw >
			case COOKING :			if( SCharacterData.nJobSkill[FARMING].skillexp/10000 < 50
										&& SCharacterData.nJobSkill[FISHING].skillexp/10000 < 50 ) 	return 1;	//�丮�� : ���, ���
				break;
			case BLACKSMITHY :		if( SCharacterData.nJobSkill[MINING].skillexp/10000 < 50 ) 		return 1;	//�������� : ����, 
				break;
			case CANDLEMAKING :		if( SCharacterData.nJobSkill[FISHING].skillexp/10000 < 50 )		return 1;	//������ : ���
				break;

			case CARPENTRY :		
			case BOWCRAFTING :		if( SCharacterData.nJobSkill[CUTDOWN].skillexp/10000 < 50 )		return 1;	//���, Ȱ���ۻ� : ������
				break;
			case TAILORING :		if( SCharacterData.nJobSkill[BUTCHERING].skillexp/10000 < 50 )	return 1;	//���ܻ� : ������
				break;
			case ALCHEMING :		if( SCharacterData.nJobSkill[HERBDIGGING].skillexp/10000 < 50 )	return 1;	//���ݼ��� : �ɸ���
				break;
			//011012 lsw <
			//default : return 5;
		}
	}
	else
	{
		switch( inclusive )
		{
			case TAIMING :
				if( SCharacterData.nLevel < 35 ) return 1;		// ��ī���̼� ���� �¹� : ���� 35�̻� �� 10���� �̻�	�ٸ� �ɷ� üũ�� �ʿ� ����.
				check_ability = 0;
				break;
			case ANIMAL_LORE :
				//011012 lsw
				if( SCharacterData.nJobSkill[TAIMING].skillexp/10000 < 40 ) return 1;		// ��ī���̼� ���� �¹� : ���� 35�̻� �� 10���� �̻�	�ٸ� �ɷ� üũ�� �ʿ� ����.
				check_ability = 0;
				break;
		}
	}

	if( SCharacterData.SkillId[num] ) return 4;			// �̹� �����
	//011022 YGI
	if( check_ability && SCharacterData.nJobSkill[inclusive].skillexp< SkillTable[num].need_exp ) return 1;				// �ɷ��� ���ڶ�
	if( SCharacterData.nMoney < (DWORD) SkillTable[num].money ) return 2;		// ���� ���ڶ�

	SendLearnSkill( num, guild );			// �ϴ� ���� ��忡���� ��ų�� ���ٴ� �����Ͽ�...

	return 3; // ������ �ִ�
}


void RecvCMD_NOW_SKILL_POINT(t_packet *p)
{
	const int iPoint = p->u.NowSkillPoint.iNowPoint;
	switch( p->u.NowSkillPoint.item.item_no)
	{
	case 1046:
		{
			AddCurrentStatusMessage(FONT_COLOR_YELLOW,lan->OutputMessage(2,700),iPoint);
		}break;
	case 1047:
		{
			AddCurrentStatusMessage(FONT_COLOR_YELLOW,lan->OutputMessage(2,701),iPoint);
		}break;
	default:
		{

		}break;
	}
}


DWORD jobColor[100];
void LoadJobLvColor()
{
	char *filename = "./data/job_color.txt";
	char *filename_bin = "./data/job_color.bin";

#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
	FILE *fp = Fopen( filename, "rt" );
	if( fp )
	{
		char temp[512];
		int count = 0;
		while( fgets( temp, 512, fp ) != NULL && count < 255 && count<100 )
		{
			if( temp == NULL|| temp[0] == ';'  || temp[0] == '#' ) continue;
			int r, g, b;
			char *token;
			token = strtok( temp, "\t\n" );
			if( !token ) continue;
			r = atoi( token );
			token = strtok( NULL, "\t\n" );
			g = atoi( token );
			token = strtok( NULL, "\t\n" );
			b = atoi( token );
			jobColor[count] = RGB( r, g, b );
			count ++;
		}
		fclose( fp );

		fp = Fopen( filename_bin, "wb" );
		if( fp )
		{
			fwrite( jobColor, sizeof( DWORD ), 100, fp );
			fclose( fp );
		}
	}
	else
#endif
#endif
	{
		FILE *fp = Fopen( filename_bin, "rb" );
		if( fp )
		{
			fread( jobColor, sizeof( DWORD ), 100, fp );
			fclose( fp );
		}
	}
}
//011012 lsw >
// �׷��� �����ͷ� ����� ���� ��ų�� �ִ����� ���Ѵ�.
int GM_temp_list[MAX_JOB_OF_SKILL];
int GM_temp_list_count;

void SetGMskill()//�������̽��� GM���� ����Ʈ ���
{//021108 lsw
	memset( GM_temp_list, 0, sizeof( MAX_JOB_OF_SKILL ) );
	GM_temp_list_count = 0;

	t_SkillExp *pExp = SCharacterData.nJobSkill;

	for( int i=0; i<MAX_GM_MAIN ; i++ )//021108 lsw
	{	
		if( 999999 == pExp[g_GmMain[i].skill].skillexp )
		{
			const int iJobLv = g_GmMain[i].job_level;
			switch(iJobLv)
			{
			case 1:
				{
					GM_temp_list[GM_temp_list_count++] = i;
				}break;
			case 2:
				{
					int ok =0;
					switch(g_GmMain[i].skill)
					{
					case CARPENTRY		:	{ok=((pExp[CUTDOWN].skillexp == 1000000)?1:0);}break;
					case BLACKSMITHY	:	{ok=((pExp[MINING].skillexp == 1000000)?1:0);}break;
					case COOKING		:	{ok=((pExp[FARMING].skillexp == 1000000 
												||pExp[FISHING].skillexp == 1000000)?1:0);}break;
					case TAILORING		:	{ok=((pExp[BUTCHERING].skillexp == 1000000)?1:0);}break;
					case BOWCRAFTING	:	{ok=((pExp[CUTDOWN].skillexp == 1000000)?1:0);}break;
					case ALCHEMING		:	{ok=((pExp[HERBDIGGING].skillexp == 1000000)?1:0);}break;
					case CANDLEMAKING	:	{ok=((pExp[FISHING].skillexp == 1000000)?1:0);}break;
					default:{}break;
					}
					if(ok&& pExp[g_GmMain[i].skill].skillexp == 999999)
					{
						GM_temp_list[GM_temp_list_count++] = i;
					}
				}break;
			case 3:
			case 4:
//				{
//				}break;
			default:
				{
				}break;
			}//switch(iJobLv)
		}
	}
}

GM_QUEST g_GmMain[MAX_GM_MAIN];
void LoadGMQuest( void )		// �׷��� �����Ϳ� ����Ҷ� üũ�ؾ��� ���̺� 
{			
	FILE *fp;
	int   i;
	char  crc, crc1 = 0;
			
	fp = Fopen( "./data/GMquest.bin", "rb" );
	if( fp == NULL ) return;
	fread( &crc, 1,1, fp );
	fread( g_GmMain, sizeof( GM_QUEST), MAX_GM_MAIN, fp);
	fclose(fp);
			
	char *tt = (char *)g_GmMain;
	for( i = 0 ; i < sizeof(GM_QUEST ) * MAX_GM_MAIN; i ++)
	{		
		crc1 += (( *tt + *tt ) - i );
		*tt -= ( 200 + i );
		tt ++;
	}		

	if( CheckSumError == 0 ) 
	{
		CheckSumError = abs( crc - crc1 );
		if( CheckSumError ) JustMsg( "GMquest.bin CheckSumError " );
	}
}		

int CanGmRegist( int guild_count )		// gm�� ����Ҽ� �ִ���?		// ���� üũ
{//021030 lsw
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		AddCurrentStatusMessage( 250,0,0, lan->OutputMessage(7,137) );//010215 lsw
		return 1;
	}
#endif
	// �ش� ����� 999999 �̾�� �ϰ�... 
	// ����Ҽ� �ִ� ���� �ִ°�?
	// �ش� �������� ���� ��ŭ �ִ°�...
	// �ش� ����Ʈ�� �ߴ°�...			// ����� üũ���� �ʴ´�.
	// �� 2 �� ����� ���� gm �� �������� �ʴ´�.
	
	//010708 lsw 2�� ���� Ǯ��
	GM_QUEST *pGmMain = &g_GmMain[guild_count];
	switch(pGmMain->job_level)
	{
	case 1:
	case 2:
		{
			if( GM_temp_list_count == 0 )
			{
				return -4;	// GM�� ����� ���� ����� ����.
			}
		}
	case 3:
	case 4:
		{//1~4 �� ������ ����
		}break;
	default:
		{
			return -1;
		}
	}

	if( pGmMain->need_money > SCharacterData.nMoney )
	{
		return -3;	// ���� �����ϴٸ�
	}

	for( int i=0; i<MAX_GM_NEED_ITEM_COUNT; i++ )
	{
		const int iItemNo = pGmMain->need_item[i];
		if( iItemNo )
		{
			// ���� �� �������� ������ �ִ��� üũ( �κ��丮�� ������ �־�� �Ѵ�.);
			int ct = ::CheckInventory( iItemNo, pGmMain->need_quantity[i] );
			if( ct < 0 ) return -2;		// Ư�� �������� ������ �����ϴ�.
		}
	}

	return 1;
}

void GetSelectableJob()		// ���� ���� ������ ���� ��� �����ֱ�
{//021030 lsw
	memset( GM_temp_list, 0, sizeof( MAX_JOB_OF_SKILL ) );		// ���� �ٲܼ� �ִ� ���� ����Ʈ�� �ӽ� ���
	GM_temp_list_count = 0;		
	for( int i=0; i<MAX_GM_MAIN; i++)//021119 lsw
	{
		if(	g_GmMain[i].job_level ==1
		||	g_GmMain[i].job_level ==2)
		{
			if( SCharacterData.nJobSkill[g_GmMain[i].skill].skillexp >= 50000 ) 
			{
				GM_temp_list[GM_temp_list_count++] = i;
			}
		}
	}
}

int IsAllGM()//1�� 2�� �� ����?
{
	t_SkillExp *pExp = SCharacterData.nJobSkill;

	for(int i = 0; MAX_GM_MAIN > i ; i++)
	{
		if(	g_GmMain[i].job_level ==1
		||	g_GmMain[i].job_level ==2)
		{
			if(1000000 != pExp[g_GmMain[i].skill].skillexp)
			{
				return 0;
			}
		}
	}//�⺻���� All Gm�̾�� �Ѵ�
	return 1;
}

bool IsMerchant()
{ 
	if(IsAllGM())
	{
		t_SkillExp *pExp = SCharacterData.nJobSkill;
		if(pExp[MERCHANT].skillexp == 1000000)	
		{
			return true;
		}
	}
	return false;
}

void PutJobLv( int x, int y, int job, int call_job_lv )
{//021113 lsw
	const int call_job = GetCallJob( call_job_lv );
	
	if( call_job == -1 || job < 0 || job >= MAX_GM_MAIN ) return;
	
	GM_QUEST *pGmMain = &g_GmMain[job];
	switch(pGmMain->job_level)
	{
	case 1:
	case 2:
		{
			if( call_job_lv == 100 )
			{
				int image_num;
				if( job >= 7 ) image_num = 35;
				else image_num = 30;
				Spr *s = GetSprOfMenu( MAIN_ETC, image_num );
				if( s )
				PutCompressedImage( x+s->ox, y+s->oy-2, s );
				::HprintBold( x+s->xl+6, y, RGB(155, 255, 155), 0, pGmMain->name );
			}
			else
			{
				::HprintBold( x, y, jobColor[call_job_lv-1], 0, "%s   %s", pGmMain->name, lan->OutputMessage(2,600+call_job)  );//021111 lsw
				::HprintBold( x, y, RGB(255, 255, 0), 0, pGmMain->name );
			}
		}break;
	case 3:
		{
			::FieldTypeNomalPut(x,y,0,0,0,SKILL_MASTER_MARK_IMG);
			::HprintBold( x+45, y+2, RGB(155, 255, 155), 0, pGmMain->name );
		}break;
	case 4:
		{
			::FieldTypeNomalPut(x+10,y,0,0,1,SKILL_MASTER_MARK_IMG);
			::HprintBold( x+70, y+2, RGB(155, 255, 155), 0, pGmMain->name );
		}break;
	}
}

inline void LoadMultiRare(char * buf,int i)
{
	char *token;
	token = strtok(buf, "\t\n\r");	if( token == NULL ) {return;}
	const int iIndex = atoi(token);	
	{aIMR[iIndex].iGrade = iIndex;}
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{aIMR[iIndex].iAble = atoi(token);}
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemNo[0] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemNo[1] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemNo[2] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemNo[3] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemNo[4] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemNo[5] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemNo[6] = atoi(token); }

	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemCt[0] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemCt[1] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemCt[2] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemCt[3] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemCt[4] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemCt[5] = atoi(token); }
	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].aItemCt[6] = atoi(token); }

	token = strtok(NULL,"\t\n\r");	if( token == NULL )	{return;}{ aIMR[iIndex].iAddCt = atoi(token);}
	return;
}

int LoadItemMultiRare()
{
	char buf[1024];
	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	sprintf(filename, ".\\itemtable\\MultiRare.txt");
	sprintf(filenameBin, ".\\itemtable\\MultiRare.bin");
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_ITEM_MULTI_RARE-1 ) break;
				LoadMultiRare(buf, i);
			i++;
		}
		fclose(fp);
		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( aIMR, sizeof(ITEMMULTIRARE)*MAX_ITEM_MULTI_RARE, 1, fp);
			fclose(fp);
		}
	 }
	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( aIMR, sizeof(ITEMMULTIRARE)*MAX_ITEM_MULTI_RARE, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

int LoadItemLegend()
{//021113 lsw
	FILE *fp = Fopen( "./ItemTable/item_Legend.bin", "rb" );
	if( !fp ) return 0;

	fread( g_aItemLegend, sizeof(ITEM_LEGEND)*MAX_ITEM_LEGEND,1, fp );
	fclose( fp );

	return 1;
}

int LoadItemLegendBonus()
{

	FILE *fp = Fopen( "./ItemTable/item_Legend_bonus.bin", "rb" );
	if( !fp ) return 0;

	fread( g_aLegendBonus, sizeof(ITEM_LEGEND_BONUS)*MAX_ITEM_LEGEND_BONUS,1, fp );
	fclose( fp );

	return 1;
}

int	LoadItemTableData()
{
	ItemRead();
	if(!LoadItemSokSungTable()){return 0;}
	if(!LoadItemSokSungKindTable()){return 0;}
	if(!LoadItemSokSungGradeTable()){return 0;}
	if(!LoadItemFunctionTable()){return 0;}
	if(!LoadRareGroup()){return 0;}
	if(!LoadFunctionGroup()){return 0;}
	if(!LoadRareProbability()){return 0;}
	if(!LoadCombatSkillExplain()){return 0;}
	if(!LoadCombatSkillGroupExplain()){return 0;}
	if(!LoadItemMultiRare()){return 0;}
	
	if(!LoadItemLegend()){return 0;}
	if(!LoadItemLegendBonus()){return 0;}

	g_SymBolMgr.Load();//soto-1.02

	return 1;
}

MULTIRARESRC g_MultiRareSrc;

void SendCMD_SKILL_MASTER_MULTI_RARE_MAKE(const int iSkillNo)
{
	t_packet p;
	p.h.header.type = CMD_SKILL_MASTER_MULTI_RARE_MAKE;
	p.h.header.size = sizeof(t_SkillMasterMultiRareMake);
	p.u.Hwoa.rang.SMMultiRareMake.iCn = Hero->id;
	p.u.Hwoa.rang.SMMultiRareMake.iSkillNo = iSkillNo;
	p.u.Hwoa.rang.SMMultiRareMake.MultiRareSrc = g_MultiRareSrc;
	QueuePacket( &p, 1 );
	
	::memset(&g_MultiRareSrc,0,sizeof(MULTIRARESRC));//Ŭ����~
}

void SendReqSkillInfo( LPCHARACTER ch )		// ������ ��ų ������ ��û�Ѵ�.
{
	if( ch->job ) return;		// �������� ������ �� ��û���� �ʴ´�.

	short int id = ch->id;

	t_packet p;

	p.h.header.type = CMD_GET_SKILL_INFO;
		p.u.kein.default_short_int = id;
	p.h.header.size = sizeof( short int);
	QueuePacket( &p, 1 );
}

void RecvGetSkillInfo( t_packet *p )
{
	k_get_skill_info *target = &p->u.kein.get_skill_info;

	LPCHARACTER ch = ReturnCharacterPoint( target->target_id );
	if( !ch ) return;

	ch->job = target->job;
	ch->call_job_lv = target->call_lv;
}

void RecvRegsitGmWho( t_packet *p )
{
	int gm_index		= p->u.kein.send_gm_regist_ok.gm_index; 
	int gm_rank			= p->u.kein.send_gm_regist_ok.gm_rank; 
	int total_gm_rank	= p->u.kein.send_gm_regist_ok.total_gm_rank; 
	char *name			= p->u.kein.send_gm_regist_ok.name; 

	AddCurrentStatusMessage( 250,250,60,lan->OutputMessage(3,113), name, g_GmMain[gm_index].name, gm_rank, total_gm_rank );//010215 lsw
	LPCHARACTER ch = ExistHe( name );
	if( !ch ) return;

	ch->call_job_lv = 100;		// gm ��ũ �޾� �ش�. Ȥ�� � ȿ���� �������� ���⿡...
	ch->job = gm_index;
	if( ch == Hero )
	{
		//011012 lsw >
		SCharacterData.nJobSkill[g_GmMain[gm_index].skill].skillexp = 1000000;
		//011012 lsw <
	}
	MP3( SN_GM_REGIST_OK );
}

void ShowComeOutGM( char *name )
{
	AddCurrentStatusMessage( 250,250,60, lan->OutputMessage(7,138), name );//010215 lsw//reece
	LPCHARACTER ch = ExistHe( name );
	if( !ch ) return;

	
	//ch->end_Casting;
	//ch->castStartingTime;
	ch->call_job_lv = 100;		// gm ��ũ �޾� �ش�. Ȥ�� � ȿ���� �������� ���⿡...
	MP3( SN_GM_REGIST_OK );
}