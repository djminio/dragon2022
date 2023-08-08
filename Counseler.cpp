#include "stdafx.h"
#include <Stdio.h>
#include <Process.h>
#include "resource.h"
#include "dragon.h"
#include "math.h"

#include "gameproc.h"
#include "network.h"
#include "map.h"
#include "char.h"
//#include "FPS.h"
#include "object.h"

#include "Hong_Sprite.h"
#include "Hong_Light.h"
#include "Hong_Sub.h"

#include "Hangul.h"
//#include "Menudef.h"
#include "Menu.h"
#include "MenuStartSet.h"
#include "DirectSound.h"
#include "Effect.h"
#include "LineCommand.h"
#include "Tool.h"
#include "ItemTable.h"
#include "Midi.h"
#include "MouseCursor.h"
#include "SkillTool.h"
#include "Skill.h"
#include "House.h"
#include "CharDataTable.h"
#include "Item.h"
#include "SmallMenu.h"
#include "Path.h"
#include "chat.h"
#include "TextOutMgr.h"


#define __COUNSELER_CPP__
#include "Counseler.h"



extern void SettingDestPoint( int x, int y );



void SendBlockAttack( void )
{	

#ifdef _DEBUG

	char temp[MAX_PATH];
	if( !g_block_attack )
	{
		sprintf( temp, lan->OutputMessage(7,2)   );//010215 lsw
	}
	else 
	{
		sprintf( temp, lan->OutputMessage(7,3)   ); //010215 lsw
	}

	if( YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES )//010215 lsw
	{	

	}
	else return;

	
	g_block_attack = ! g_block_attack;

	t_packet p;

	if( g_block_attack )
		p.h.header.type = CMD_BLOCK_ATTACK_START;	// 공격을 불능으로 만든다. 
	else  
		p.h.header.type = CMD_BLOCK_ATTACK_END;		// 공격을 가능하게 한다. 

	p.h.header.size = 0;

	QueuePacket( &p, 1 );


#endif
}	

void SendBlockMagic( void )
{
#ifdef _DEBUG
	char temp[MAX_PATH];
	if(!g_block_magic)
	{
		sprintf( temp, lan->OutputMessage(7,4)   );//010215 lsw
	}
	else 
	{
		sprintf( temp, lan->OutputMessage(7,5)   );//010215 lsw
	}

	if( YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES )//010215 lsw
	{	

	}
	else return;
	
	
	g_block_magic = ! g_block_magic;

	t_packet p;
	if( g_block_magic )
		p.h.header.type = CMD_BLOCK_MAGIC_START;	// 이동을  불능으로 만든다. 
	else  
		p.h.header.type = CMD_BLOCK_MAGIC_END;	// 이동을 가능하게 한다. 

	p.h.header.size = 0;

	QueuePacket( &p, 1 );

#endif
}
void SendBlockAntiTeam(void)//reece
{
	
#ifdef _DEBUG
	char temp[MAX_PATH];
	if (!g_block_anti_team)
	{
		sprintf(temp, lan->OutputMessage(7, 4));//010215 lsw
	}
	else
	{
		sprintf(temp, lan->OutputMessage(7, 5));//010215 lsw
	}

	if (YesOrNo(temp, lan->OutputMessage(8, 1)) == IDYES)//010215 lsw
	{

	}
	else return;


	g_block_anti_team = !g_block_anti_team;

	t_packet p;
	if (g_block_anti_team)
		
		p.h.header.type = CMD_BLOCK_ANTI_TEAM_START;	// 이동을  불능으로 만든다. 
	else
		p.h.header.type = CMD_BLOCK_ANTI_TEAM_END;	// 이동을 가능하게 한다. 

	p.h.header.size = 0;

	QueuePacket(&p, 1);

#endif
}
void SendBlockSkill(void)//020821 lsw
{
#ifdef _DEBUG
	char temp[MAX_PATH];
	if( !g_block_move )
	{
		sprintf(temp,lan->OutputMessage(7,250));//010215 lsw
	}
	else 
	{
		sprintf(temp,lan->OutputMessage(7,251));//010215 lsw
	}

	if( YesOrNo(temp, lan->OutputMessage(8,1) ) == IDYES )//010215 lsw
	{	

	}
	else return;
	
	g_block_skill = ! g_block_skill;

	t_packet p;
	if( g_block_skill )
	{
		p.h.header.type = CMD_BLOCK_SKILL_START;	// 이동을  불능으로 만든다. 
	}
	else  
	{
		p.h.header.type = CMD_BLOCK_SKILL_END;	// 이동을 가능하게 한다. 
	}	
	p.h.header.size = 0;
	QueuePacket( &p, 1 );
#endif
}

void SendBlockMove( void )
{
#ifdef _DEBUG
	char temp[MAX_PATH];
	if( !g_block_move )
	{
		sprintf( temp, lan->OutputMessage(7,6)  );//010215 lsw
	}
	else 
	{
		sprintf( temp, lan->OutputMessage(7,7)   );//010215 lsw
	}

	if( YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES )//010215 lsw
	{	

	}
	else return;
	
	g_block_move = ! g_block_move;

	t_packet p;
	if( g_block_move )
		p.h.header.type = CMD_BLOCK_MOVE_START;	// 이동을  불능으로 만든다. 
	else  
		p.h.header.type = CMD_BLOCK_MOVE_END;	// 이동을 가능하게 한다. 

	p.h.header.size = 0;

	QueuePacket( &p, 1 );
#endif
}

void SendBlockGiveLife( void )
{
#ifdef _DEBUG
	char temp[MAX_PATH];
	if( !g_block_GiveLife )
	{
		sprintf( temp, lan->OutputMessage(7,8)  );//010215 lsw
	}
	else 
	{
		sprintf( temp, lan->OutputMessage(7,9) );//010215 lsw
	}

	if( YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES )//010215 //010215 lsw
	{	

	}
	else return;

	g_block_GiveLife = ! g_block_GiveLife;


	t_packet p;
	if( g_block_GiveLife )
		p.h.header.type = CMD_BLOCK_GIVE_LIFE_START;	// 이동을  불능으로 만든다. 
	else  
		p.h.header.type = CMD_BLOCK_GIVE_LIFE_END;	// 이동을 가능하게 한다. 

	p.h.header.size = 0;

	QueuePacket( &p, 1 );
#endif
}

void ViewCounselerStatus( void )
{
#ifdef _DEBUG
	if( GetSysInfo( SI_GAME_MAKE_MODE ) ) 
	{	
		::HprintBold( 10, 130+0, (g_block_attack)?	RGB( 255, 0, 100 ):RGB( 0, 255, 100 ),0, lan->OutputMessage(7, (g_block_attack)? 11:12) );
		::HprintBold( 10, 130+15, (g_block_magic)?	RGB( 255, 0, 100 ):RGB( 0, 255, 100 ),0, lan->OutputMessage(7, (g_block_magic)?	 13:14) );
		::HprintBold( 10, 130+30, (g_block_move )?	RGB( 255, 0, 100 ):RGB( 0, 255, 100 ),0, lan->OutputMessage(7, (g_block_move )?	 15:16) );
		::HprintBold( 10, 130+45, (g_block_skill)?	RGB( 255, 0, 100 ):RGB( 0, 255, 100 ),0, lan->OutputMessage(7, (g_block_skill)?	 252:253) );
	}

	//if( g_absolute_logout ){ HprintBold( 280,25, RGB( 0, 255, 100 ), 0, lan->OutputMessage(7,17) ); }//010215 lsw
	//if( g_make_ghost      ){ HprintBold( 280,25, RGB( 0, 255, 100 ), 0, lan->OutputMessage(7,18) ); }//010215 lsw
	if( g_give_life		  ){ HprintBold( 280,25, RGB( 0, 255, 100 ), 0, lan->OutputMessage(7,19) ); }//010215 lsw
	if( g_absolute_move )  { HprintBold( 280,25, RGB( 0, 255, 100 ), 0, lan->OutputMessage(7,20) ); }//010215 lsw
	if( g_absolute_move_dk){ HprintBold( 280,25, RGB( 0, 255, 100 ), 0, lan->OutputMessage(7,21) ); }//010215 lsw
	if( AbsoluteMoveFlag)  { HprintBold( 280,25, RGB( 0, 255, 100 ), 0, lan->OutputMessage(7,22) ); }//010215 lsw
	if( AbsoluteMoveDKFlag){ HprintBold( 280,25, RGB( 0, 255, 100 ), 0, lan->OutputMessage(7,23) ); }//010215 lsw
#endif
}

bool CounselerSelectChar( void )
{
#ifdef _DEBUG

	t_packet p;
	if( AbsoluteMoveFlag )
	{
		AbsoluteMoveFlag = false;

		int mx = Mox / TILE_SIZE;
		int my = Moy / TILE_SIZE;
		CHARACTER t;
			
		memcpy( &t, lpAbsoluteMoveCh, sizeof( CHARACTER ) );
		t.moveFlag = TRUE;
		t.movetype = 0;
		t.gox = Mox;
		t.goy = Moy;
		t.destx = mx, t.desty = my;
		if ( PathBuild( &t ) )
		{	
			SettingDestPoint( Mox, Moy );
			
			t.moveFlag = FALSE;
			t.gox = Mox;
			t.goy = Moy;

			t.movetype += 100;
			SendMoveData( &t );	
			t.movetype -= 100;
		}		

		return true;
	}

	if( AbsoluteMoveDKFlag )
	{
		AbsoluteMoveDKFlag = false;

		int mx = Mox / TILE_SIZE;
		int my = Moy / TILE_SIZE;
    t_packet p;
    memset(&p, 0, sizeof(t_packet));
    p.h.header.type = CMD_ABSOLUTE_MOVE_DUNGEON_KEEPER;
		p.h.header.size = sizeof( t_server_magic_position );
		p.u.angra.server_magic_position.idTarget = WORD(lpAbsoluteMoveCh->id);
		p.u.angra.server_magic_position.nX = mx;
		p.u.angra.server_magic_position.nY = my;
		QueuePacket(&p,1);						// LTS LOCALWAR
		return true;
	}

	switch( g_GameInfo.nSelectedSpriteType )
	{
		case SPRITETYPE_ON_THE_CEILING_CHAR	:
		case SPRITETYPE_MONSTER				:
		case SPRITETYPE_NPC					:
		case SPRITETYPE_CHARACTER			:	 break;

		default : return false;
	}

	if( g_GameInfo.lpcharacter == NULL ) return false;
//	if( g_GameInfo.lpcharacter == Hero ) return false;
		
		
	LPCHARACTER ch = g_GameInfo.lpcharacter;
			
			
	char temp[ MAX_PATH];
			
	if( g_absolute_logout ) 
	{		
		sprintf( temp, lan->OutputMessage(7,24), ch->name );//010215 lsw
		if( 1/*YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES*/ )//010215 lsw
		{	
			p.h.header.type = CMD_ABSOLUTE_LOGOUT;
			p.u.absolute_logout.id = ch->id;
			p.h.header.size = sizeof( t_absolute_logout );
			QueuePacket( &p, 1 );
			return true;
		}	


	}		
	if( g_make_ghost )
	{
		sprintf( temp, lan->OutputMessage(7,25), ch->name );//010215 lsw
		if( 1/*YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES*/ )
		{
			p.h.header.type = CMD_MAKE_GHOST;
			p.u.make_ghost.id = ch->id;
			p.h.header.size = sizeof( t_make_ghost );
			QueuePacket( &p, 1 );
			//acer7
			char *type = "GIVE_LIFE";
			CallServer( CMD_USE_GM_TOOL, type, strlen( type ) );

			return true;
		}
	}
	if( g_give_life )
	{
		sprintf( temp, lan->OutputMessage(7,26), ch->name );//010215 lsw
		if( YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES )
		{
			p.h.header.type = CMD_GIVE_LIFE;
			p.u.make_ghost.id = ch->id;
			p.h.header.size = sizeof( t_give_life );
			QueuePacket( &p, 1 );
			return true;
		}
	}	
		
	if( g_absolute_move )	//  강제로 이동시킬수 있다. 
	{	
		sprintf( temp, lan->OutputMessage(7,27), ch->name );//010215 lsw
		if( YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES )
		{
			lpAbsoluteMoveCh = ch;
			AbsoluteMoveFlag = true;
			g_absolute_move = false;
			return true;
		}
	}
	
	if( g_absolute_move_dk )	//  강제로 이동시킬수 있다. 
	{	
		sprintf( temp, lan->OutputMessage(7,28), ch->name );//010215 lsw
		if( YesOrNo(  temp, lan->OutputMessage(8,1) ) == IDYES )
		{
			lpAbsoluteMoveCh = ch;
			AbsoluteMoveDKFlag = true;
			g_absolute_move_dk = false;
			return true;
		}
	}
#endif
	return false;
}		
	
//// 초보자를 위한 공지사항을 보낸다. (클라이언트에서만 처리한다.)
		
#define BEGINER_BBS_START		0
#define BEGINER_BBS_END			20
				
#define BEGINER_BBS_INTERVAL	10 // sec
				
int  BeginerBBS_start;
int  BeginerBBS_count;
int  BeginerBBS_x;
	
char BeginerBBS[ FILENAME_MAX];
	
void Check_Beginer_BBS( void )
{	
	static DWORD time;
	int i;
	char temp[FILENAME_MAX];
	
	if( SCharacterData.nLevel > 5 )  return;
	
	if( g_curr_time - time > BEGINER_BBS_INTERVAL )
	{
		time = g_curr_time;
	}
	else return;
	
	FILE *fp;
	fp = Fopen( "./data/beginer.txt", "rt" );
	if( fp == NULL ) return;
	
	for( i = 0 ; i < BeginerBBS_count ; i ++)
	{
		if( fgets( temp, FILENAME_MAX, fp ) == NULL ) 
		{ 
			fclose(fp); 
			BeginerBBS_count = BEGINER_BBS_START;
			return; 
		}
	}

	BeginerBBS_start = BEGINER_BBS_INTERVAL;
	BeginerBBS_x	 = 10;
	EatRearWhiteChar( temp );
	strcpy( BeginerBBS, temp );		
	BeginerBBS_count ++;
	if( BeginerBBS_count >= BEGINER_BBS_END ) BeginerBBS_count = BEGINER_BBS_START;

	fclose(fp);									
}	
	
void Display_Beginer_BBS( void )
{	
	static DWORD time;															

	return;
																								
	if( BeginerBBS_start <= 0 ) return;										
																					
	if( g_curr_time != time )										
	{																
		time = g_curr_time;													
		BeginerBBS_start--;											
	}														
																
	static int x;										
																		
	switch( BeginerBBS_start )										
	{															
		case 1 : BeginerBBS_x -= 20;		break;
	}										

	


//	Getcolor( &r,&g,&b );									
//	Hcolor( 0,255,255 );							
	HprintBoldcolor( BeginerBBS_x+1, 2+1, 20, BeginerBBS );
	HprintBoldcolor( BeginerBBS_x, 2, 100,BeginerBBS );
//	Hcolor( r,g,b );
}		
		
		
		
		
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
		

						
int ViewTipsCount = 0;	
						
						

char Tips[ TIPS_LINE][ FILENAME_MAX];
						
int ViewTipsLoad( int n )
{						
	static int t;		
	FILE  *fp;			
	int		i;			
	char s[FILENAME_MAX];
	char f[FILENAME_MAX];
					
	if( n < 0 ) return 0;
	if( Hero == NULL ) return 0;

	switch( SCharacterData.nCharacterData[CLASS] )
	{
		case WARRIOR	:	sprintf( f, "./data/warrior_tip%d.txt"	, Hero->name_status.nation );			break;
		case THIEF		:	sprintf( f, "./data/thief_tip%d.txt"	, Hero->name_status.nation );			break;
		case ARCHER		:	sprintf( f, "./data/archer_tip%d.txt"	, Hero->name_status.nation );			break;
		case WIZARD		:	sprintf( f, "./data/wizard_tip%d.txt"	, Hero->name_status.nation );			break;
		case PRIEST		:	sprintf( f, "./data/priest_tip%d.txt"	, Hero->name_status.nation );			break;
		default			:	return 0;
	}

	fp = Fopen( f, "rt" );
	if( fp == NULL )return 0 ;
	int count = 0;		
	while( 1 )			
	{					
		if( fgets( s, FILENAME_MAX, fp ) == NULL ) { fclose(fp); return -1; }
		
		if( *s == ':' ) 
		{
			if( count == n ) break;
			count ++;
		}
	}						
	
	for( i = 0 ; i<  TIPS_LINE  ; i ++) Tips[i][0] = 0;
	for( i = 0 ; i < TIPS_LINE  ; i ++)
	{						
		if( fgets( s, FILENAME_MAX, fp ) == NULL ) { fclose(fp); return -1; }
		if( *s == ':' )  { break; }
		EatRearWhiteChar( s );
		strcpy( Tips[i], s );
	}								
										
	fclose(fp);						
	
	return 1;
}		
	
void ViewTipsPrev( void )
{	
	ViewTipsCount --;
	if( ViewTipsLoad( ViewTipsCount ) )
	{
		PlayListAutoSounds( 64, 0, 0, 0 );
	}
	else ViewTipsCount = 0;
}	
	
void ViewTipsNext( void )
{	
	ViewTipsCount ++;

AGAIN_ :
	switch( ViewTipsLoad( ViewTipsCount )  )
	{
	case 1 : PlayListAutoSounds( 64, 0, 0, 0 );
		break;
	case -1: ViewTipsCount = 0; goto AGAIN_;

	default : ViewTipsCount =0; break;
	}
}		
			
void ViewTipsBack( void )
{			
	Spr *s;	
	int i,j;
	int xl,yl;
	int tx,ty;
	
	if( ViewTipsFlag == 0 ) return;
			
	s = &spr[ 71];
			
	xl = s->xl+1;
	yl = s->yl;
			
	int X = TIPS_X+TIPS_X;
	int Y = TIPS_Y+TIPS_Y;
			
	ty = Y - s->oy;
	for( j = 0 ; j < TIPS_YL / yl+1 ; j ++)
	{		
		tx = X +s->ox-9;
		for( i = 0 ; i < 6 ; i ++)
		{	
			PutCompressedImageFX( tx, ty+9, s, 20, 1 );
			tx += xl;
		}	
		ty += yl;
	}		
			
	Box( Mapx+1         , Mapy+1 + TIPS_Y-4,  TIPS_XL+X+Mapx-1, TIPS_YL+Y+Mapy-1, RGB16( 150, 150,150 ));
	Box( Mapx           ,   Mapy + TIPS_Y-4,  TIPS_XL+X+Mapx,   TIPS_YL+Y+Mapy,   RGB16( 100, 100,100 ));
			
}				
			
void ViewTipsMenu( void )
{	
	int gapy =  110;
	
	if( ViewTipsFlag )
	{
		int y = 8;
		int bx= TIPS_PRE_X;
		int by= TIPS_PRE_Y;
		int nx= TIPS_NEX_X;
		int ny= TIPS_NEX_Y;


		if( BoxAndDotCrash( 0,0, 80, 40, g_pointMouseX, g_pointMouseY ) )
		{		
			y++;
			if( LButtonDownIng ) y++;
		}		
		else	
		if( BoxAndDotCrash( 120+bx,0+115+by, 60, 20, g_pointMouseX, g_pointMouseY ) )
		{		
			by++;
			if( LButtonDownIng ) by++;
		}		
		else	
		if( BoxAndDotCrash( 65+120+nx,0+115+ny, 60, 20, g_pointMouseX, g_pointMouseY ) )
		{
			ny++;
			if( LButtonDownIng ) ny++;
		}

		Box( Mapx+1, Mapy+1+y,  80+Mapx-1+y, 20+Mapy-1+y, RGB16( 150, 150,150 ));
		Box( Mapx,   Mapy+y,    80+Mapx+y,   20+Mapy+y,   RGB16( 100, 100,100 ));
		HprintBold( 5,5+y, RGB(0xff,0xff,0),  RGB(0x7f,0x7f,0), lan->OutputMessage(7,31) );//010215 lsw
	
		char temp[ FILENAME_MAX];
		sprintf( temp, lan->OutputMessage(7,32), ViewTipsCount+1);//010215 lsw
		HprintBold( 120,15, RGB(0xbf,0xbf,0),  RGB(0x6f,0x6f,0), temp );
	
		Box( 120+Mapx+1+bx,   Mapy+1+115 +by,  120+60+Mapx-1+bx, 20+Mapy-1+115+by, RGB16( 150, 150,150 ));
		Box( 120+Mapx  +bx,   Mapy+115   +by,  120+60+Mapx+bx,   20+Mapy+115+by,   RGB16( 100, 100,100 ));
		HprintBold( 15+120+5+bx,5+115+by, RGB(0xff,0xff,0xff),  RGB(0x7f,0x7f,0x7f), lan->OutputMessage(7,33) );//010215 lsw
	
		Box( 65+120+Mapx+1+nx,  Mapy+1+115+ny,  65+120+60+Mapx-1+nx, 20+Mapy-1+115+ny,	RGB16( 150, 150,150 ));
		Box( 65+120+Mapx  +nx,   Mapy+115 +ny,    65+120+60+Mapx+nx,   20+Mapy+115+ny,  RGB16( 100, 100,100 ));
		HprintBold( 75+120+5+nx,5+115+ny, RGB(0xff,0xff,0xff),  RGB(0x7f,0x7f,0x7f), lan->OutputMessage(7,34) );//010215 lsw
	}
	else
	{
		/*		// 001128 YGI
		Box( Mapx+1, Mapy+1,  80+Mapx-1, 20+Mapy-1, RGB16( 150, 150,150 ));
		Box( Mapx,   Mapy,    80+Mapx,   20+Mapy,   RGB16( 100, 100,100 ));
		HprintBold( 10,5, RGB( 0xff, 0xff, 0xff),  RGB( 0x7f, 0x7f, 0x7f), "Tip 보기" );*/
	}	
}		
		
		
// 0818 khs
int ViewTipsCheck( void )
{		
	if( ViewTipToggle == 0 ) return 0;
		
	if( YouCanViewTipsCheck == 1 )	YouCanViewTipsCheck = 0; 
	else return 0;

		
	int bx= TIPS_PRE_X;
	int by= TIPS_PRE_Y;
	int nx= TIPS_NEX_X;
	int ny= TIPS_NEX_Y;
			
	if( ViewTipsFlag )
	{			
		if( BoxAndDotCrash( 0,0, 80, 40, g_pointMouseX, g_pointMouseY ) )
		{		
			ViewTipsFlag = 0;
			PlayListAutoSounds( 490, 0, 0, 0 );
			YouCanHeroActionProc =0;
			return 1;
		}		
		else	
		if( BoxAndDotCrash( 120+bx,0+115+by, 60, 20, g_pointMouseX, g_pointMouseY ) )
		{		
			ViewTipsPrev();
			YouCanHeroActionProc =0;
			return 1;
		}		
		else	
		if( BoxAndDotCrash( 65+120+nx,0+115+ny, 60, 20, g_pointMouseX, g_pointMouseY ) )
		{		
			ViewTipsNext();
			YouCanHeroActionProc =0;
			return 1;
		}		
	}			
	else		
	{			
		if( BoxAndDotCrash( 0,0, 80, 20, g_pointMouseX, g_pointMouseY ) )
		{		
			ViewTipsLoad( ViewTipsCount );
			ViewTipsFlag = 1;
			PlayListAutoSounds( 489, 0, 0, 0 );
			YouCanHeroActionProc =0;
			return 1;
		}		
	}			
				
	return 0;	
}				
				
				
// 0818 khs		
void ViewTips( void )
{	
	int i;
	int X = TIPS_X + 5;
	int Y = TIPS_Y + 40;
	int r,g,b;
	
	if( ViewTipToggle == 0 ) return;

	ViewTipsBack();		
	
	ViewTipsMenu();		
	
	if( ViewTipsFlag ) 
	for( i =  0 ; i < TIPS_LINE ; i ++)
	{
		Getcolor( &r,&g,&b );
		Hcolor( 70,70,70 );
		HprintBoldcolor( X+1, Y+1, 30, Tips[i] );
		Hcolor( 200,200,200 );
		HprintBoldcolor( X, Y, 100 , Tips[i] );
		Hcolor( r,g,b );
		Y += 16;
	}
	
}	















/////////////////////////////////////////////////////////////////////////////////////////////////////
// Hero는 외부변수...........................





void GuideRookie::GuideGhost_TypeTheHelp( bool t )
{
	GG_TypeTheHelp = t;
}


int GuideRookie::GuideGhost_CheckGo( void )
{				

	if( Hero == NULL ) return 0;
	if( Hero->viewtype != VIEWTYPE_GHOST_ ) { GG_oldstatus = GG_status; GG_status = GG_ALIVE; return 0; }
	
	if( GG_status == GG_ALIVE )
	{
		GG_oldstatus = GG_status;		
		GG_status = GG_DEATH;			// 금방 죽었다. 
	}
				
	if( GG_TypeTheHelp == false ) 
		if( Hero->lv > rookielevel ) return 0;
				
	// 이끌 좌표가 없으면 리턴한다. 
	switch( Hero->name_status.nation)
	{			
	case N_VYSEUS	:	if( MapInfo[ MapNumber].ghostguide3x == 0 ) return 0;		//
		break;	
	case N_ZYPERN	:	if( MapInfo[ MapNumber].ghostguide4x == 0 ) return 0;
		break;	
	}			
				
	GG_delay = GG_NO_MORE_DEATH; // 살아나더라도 약간은 남아 있어야 겠지.. 
				
	return 1;	
}				
				
void GuideRookie::GuideGhost_GetChatMessage( const char *msg, int d )
{	
	if( msg == NULL ) return;
	
	strcpy( szChatMessage, msg );
	ChatDelay = d;

	PlayListAutoSounds( GG_WAV_DIALOG, 0,0,0 );

}	
				
				
void GuideRookie::GuideGhost_PreSet( void )
{						
	static int dt;		
	int x, y;			
	int dx,dy;			
						
	if( GuideGhost_CheckGo() == 0 )  
	{
		if( GG_delay > 0 )
		{
		}
		else return;
	}

						
	switch( GG_status )	
	{					
		case GG_DEATH :	if( BoxAndDotCrash( Hero->x-50, Hero->y-50, 100, 100, gx, gy ))
						{
							GuideGhost_GetChatMessage( lan->OutputMessage(7,41) );//010215 lsw
							GG_oldstatus = GG_status;
							GG_status = GG_DEATH_GO;
							GG_status_delay = 100;
						}
			break;		
						
		case GG_DEATH_GO : if( --GG_status_delay <= 0 )				
						   {										
							switch( MapNumber )						
							{										
								// Scholium...	
							case 85 ://020827 lsw
							case 30 :	GuideGhost_GetChatMessage( lan->OutputMessage(7,42), 120 );//010215 lsw
										GG_status_delay = 100;		
					
										break;								
																	
							default :	GuideGhost_GetChatMessage( lan->OutputMessage(7,43) );//010215 lsw
										GG_oldstatus = GG_status;	
										GG_status = GG_DEATH_GO3;	
										GG_status_delay = 100;		
								break;								
							}										
						   }										
			break;													

		case GG_DEATH_GO3 :	if( --GG_status_delay <= 0 ) 
							{
								switch( MapNumber )
								{
									// Scholium...
									case 85 :	//020827 lsw
									case 30 :	GuideGhost_GetChatMessage( lan->OutputMessage(7,42), 120 );//010215 lsw
												GG_status_delay = 100;
										break;
									default :	switch( Hero->name_status.nation )
												{
													case N_VYSEUS :		GuideGhost_GetChatMessage( lan->OutputMessage(7,44) );//010215 lsw
																		if( MapNumber == 0 ) // ma-in
																		{
																			GG_oldstatus = GG_status;
																			GG_status = GG_DEATH_GO4;
																			GG_status_delay = 150;
																		}
																		else 
																		{
																			GG_status_delay = 150;
																		}

														break;
													case N_ZYPERN :		GuideGhost_GetChatMessage( lan->OutputMessage(7,45) );//010215 lsw
																		if( MapNumber == 19 ) // renes_c
																		{
																			GG_status_delay = 150;
																			GG_oldstatus = GG_status;
																			GG_status = GG_DEATH_GO4;
																		}
																		else 
																		{
																			GG_status_delay = 150;
																		}

														break;
												}
										break;
								}	
						   }
			break;

		case GG_DEATH_GO4 : switch( Hero->name_status.nation )
							{							
								case N_VYSEUS :		if( MapNumber == 0 && BoxAndDotCrash( Hero->x-150, Hero->y-150, 300, 300, MapInfo[ MapNumber].ghostguide3x * TILE_SIZE, MapInfo[ MapNumber].ghostguide3y * TILE_SIZE ))
													{		
														GG_oldstatus = GG_status;
														GG_status = GG_DEATH_GO5;
														GG_status_delay = 0;	
													}	
													else 
													{
														GG_status_delay = 150;
														GG_oldstatus = GG_status;
														GG_status = GG_DEATH_GO3;
													}
									break;				
														
								case N_ZYPERN :		if( MapNumber == 19 && BoxAndDotCrash( Hero->x-150, Hero->y-150, 300, 300, MapInfo[ MapNumber].ghostguide4x * TILE_SIZE, MapInfo[ MapNumber].ghostguide4y * TILE_SIZE ))
													{	
														GG_oldstatus = GG_status;
														GG_status = GG_DEATH_GO5;
														GG_status_delay = 0;	
													}	
													else 
													{
														GG_status_delay = 150;
														GG_oldstatus = GG_status;
														GG_status = GG_DEATH_GO3;
													}
									break;				
							}							
			break;
														
		case GG_DEATH_GO5 :	switch( Hero->name_status.nation )
							{							
								case N_VYSEUS :		if( MapNumber == 0 && BoxAndDotCrash( Hero->x-150, Hero->y-150, 300, 300, MapInfo[ MapNumber].ghostguide3x * TILE_SIZE, MapInfo[ MapNumber].ghostguide3y * TILE_SIZE ))
													{		
													}	
													else 
													{
														GG_status_delay = 150;
														GG_oldstatus = GG_status;
														GG_status = GG_DEATH_GO3;
													}
									break;				
														
								case N_ZYPERN :		if( MapNumber == 19 && BoxAndDotCrash( Hero->x-150, Hero->y-150, 300, 300, MapInfo[ MapNumber].ghostguide4x * TILE_SIZE, MapInfo[ MapNumber].ghostguide4y * TILE_SIZE ))
													{	
													}	
													else 
													{
														GG_status_delay = 150;
														GG_oldstatus = GG_status;
														GG_status = GG_DEATH_GO3;
													}
									break;				
							}							
		
			
							if( --GG_status_delay <= 0 ) 
							{
								GG_oldstatus = GG_status;
								GG_status = GG_DEATH_GO5;
								GG_status_delay = 120;
								GuideGhost_GetChatMessage( lan->OutputMessage(7,46), 120 );//010215 lsw
							}
			break;

		case GG_ALIVE :			switch ( GG_delay )
								{
									case GG_NO_MORE_DEATH - 2  :	GuideGhost_GetChatMessage( lan->OutputMessage(7,47));//010215 lsw
										break;
									case GG_NO_MORE_DEATH - 100:	GuideGhost_GetChatMessage( lan->OutputMessage(7,48) );//010215 lsw
																	
																	
										break;
								}
			break;		
	}					
						
	x = Hero->x;		
	y = Hero->y;		
						
	GuideGhost_changexy( Hero->name_status.nation, MapNumber,&tx, &ty );
						
	dx = (int)(tx-gx)/8;
	dy = (int)(ty-gy)/8;
				
	gx += dx;	
	gy += dy;	
}				
			
			
			
void GuideRookie::GuideGhost_OutPut( void )
{	
	static int d;
	static int startcirclemagic;
	int x, y;

	
	if( GuideGhost_CheckGo() == 0 )	
	{
		if( GG_delay > 0 )
		{
			--GG_delay;
		}
		else return;
	}

	
	if( d == 0 )
	{	
		if( startcirclemagic <= 0 ) startcirclemagic = 50;
		else startcirclemagic--;
		
		x = (int )(5.*cos((startcirclemagic *5)%360*3.1415/180.));
		y = (int )(5.*sin((startcirclemagic *5)%360*3.1415/180.));
		
		InsertMagic( Hero, Hero, 386, 9, 0, 0, gx+x, gy+y );
	}	
		
		
	GuideGhost_DisplayChatMessage();
		
	d++;
	if( d > 3 ) d = 0;
}		
		
		
		
void GuideRookie::GuideGhost_changexy( int nation, int mapno, int *x, int *y )
{		
	int nx = 0, ny = 0;
	
	switch( nation )
	{	
	case N_VYSEUS	:	nx = MapInfo[ mapno].ghostguide3x * TILE_SIZE;
						ny = MapInfo[ mapno].ghostguide3y * TILE_SIZE;
		break;
	case N_ZYPERN	:	nx = MapInfo[ mapno].ghostguide4x * TILE_SIZE;
						ny = MapInfo[ mapno].ghostguide4y * TILE_SIZE;
		break;
	}	
		
	int x1, y1, x2, y2, dist;
	float decline;
		
	x1 = Hero->x;
	y1 = Hero->y;
	x2 = nx;
	y2 = ny;

	if( GG_status == GG_DEATH )	dist = 20;
	else						dist = 200;
		
	decline = (float)atan2((float)y2-y1,(float)x2-x1);
	*x = x1 + (int)((float )dist*(float)cos(decline));
	*y = y1 + (int)((float )dist*(float)sin(decline));
}		
		

	
	
void GuideRookie::GuideGhost_DisplayChatMessage( void )
{	
	if( ChatDelay <= 0 ) 
	{
		return;
	}
	ChatDelay--;

	TxtOut.Init(COLOR_BLACK, TXT_BG_NORMAL_WIDTH,12);
		
	const int iX = gx - Mapx+20;
	const int iY = gy - Mapy-10;

	const int iGab = 10;//들여 쓰기
	const int iChatWidth = 135;
	const int iLGab = CHAT_BG_HEIGHT;//줄 간격

	::Hcolor(FONT_COLOR_WHITE);
	const int iLCt = TxtOut.RcChatOut(iX, iY, iChatWidth,iGab,CHAT_COLOR_TYPE_DEFAULT,szChatMessage);

	::Hcolor(FONT_COLOR_GREEN);
	TxtOut.RcChatHeaderOut(iX, iY-iLGab*(iLCt+1), iChatWidth,iGab,CHAT_COLOR_TYPE_DEFAULT,lan->OutputMessage(7,49));//iLCt+1 인 이유는 이름 때문
}		
		
		
		
		
		
		
		
	










