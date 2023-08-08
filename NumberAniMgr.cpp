// NumberAniMgr.cpp: implementation of the CNumberAniMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Menu.h"
#include "dragon.h"
#include "gameproc.h"
#include "MenuSet.h"
#include "Chat.h"
#include "Skill.h"
#include "CurrentMsgMgr.h"
#include "NumberAniMgr.h"
#include "Hong_Sub.h"
#include "char.h"
#include "Tool.h"
#include "Hangul.h"
#include "CharDataTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPNUMBERANI		g_NumberAni;

CNumberAniMgr::CNumberAniMgr()
{

}

CNumberAniMgr::~CNumberAniMgr()
{

}


void DeleteNumberAni( LPNUMBERANI tt )
{		
	LPNUMBERANI  t = g_NumberAni;
			
	while( t != NULL )
	{		
		if( t == tt )
		{	
			if( t == g_NumberAni )
			{	
				t = g_NumberAni->next;
				MemFree( g_NumberAni );

				g_NumberAni = t;
				if( t != NULL )	g_NumberAni->prev = NULL;
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

LPNUMBERANI AddNumberAni( int type, int off, int x, int y, int num )
{
	LPNUMBERANI temp = NULL, t;
	MemAlloc( temp, sizeof( NUMBERANI ));
	
	///////////////////// 서버에서의 자료형 문제로 3으로 나누었기 때문에 3을 곱함(추후 제거) ///////////////////
	//num *= 3;

	temp->st		= NUMBER_ANIMATION_ST_ANIMATING_;
	temp->delay		= 40;
	temp->number	= num;
	temp->offset	= off;
	temp->type		= type;
	temp->x			= x + 20;	// 0105 KHS
	temp->y			= y;
	temp->d			= 20;
	temp->prev		= NULL;
	temp->next		= NULL;

	if( g_NumberAni == NULL )
	{	
		g_NumberAni			= temp;
	}	
	else
	{	
		t					= g_NumberAni;
		g_NumberAni			= temp;
		g_NumberAni->next	= t;
		t->prev				= g_NumberAni;
	}		

	return g_NumberAni;
}

void OutputNumberAni( void )	// 0105 KHS
{
	LPNUMBERANI t, temp;
	LPORDERTABLE o;
	
	t = g_NumberAni;
	while( t != NULL )
	{
		switch( t->type )
		{
		case NUMBER_ANIMATION_TYPE_DOWN_ :
			{
				t->x ++;
				t->y -= t->d;
				t->d -= 5;
				if( t->d < 2 ) t->d = 1;
				t->delay --;
				if( t->delay <= 0 ) { temp = t->next; DeleteNumberAni( t ); t = temp; continue; }
			}break;
		case NUMBER_ANIMATION_TYPE_UP_:	
			{
				if( t->d > 1 ) t->y -= t->d;
				t->d -= 3;
				if( t->d < 2 ) t->d = 1;
				t->delay --;
				if( t->delay <= 0 ) { temp = t->next; DeleteNumberAni( t ); t = temp; continue; }
			}break;
		}

		if( BoxAndDotCrash( Mapx, Mapy, GAME_SCREEN_XSIZE, GAME_SCREEN_YSIZE, t->x, t->y ) )
		{		
			o = &g_OrderInfo.order[ g_OrderInfo.count ];
					
			o->wType		= SPRITETYPE_NUMBER;
			o->x			= t->x;
			o->y			= t->y;
			o->height		= 0;
			o->offset		= t->offset;
			o->lpvData		= (LPVOID)t;
			o->show			= 1;
			o->sort			= 1;
			g_OrderInfo.count++;
		}
		t = t->next;
	}
}

t_hpupdown  HpUpDown[ MAX_HPUPDOWN];

void InitHpUpDown(void )
{
	int i; 

	for( i = 0 ; i< MAX_HPUPDOWN ; i ++)
		HpUpDown[i].flag = false;
}



void InputHpUpDown( int id, int hp, int attackedresult )
{
	int i;
	
	for( i = 0 ; i < MAX_HPUPDOWN ; i ++ )
	{
		if( HpUpDown[i].flag == false )
		{
			HpUpDown[i].id			= id;
			HpUpDown[i].hp			= hp;
			
			HpUpDown[i].attackedresult= attackedresult;
			HpUpDown[i].hpupdown	= 8;
			HpUpDown[i].flag		= true;
			break;
		}
	}

	if( i == MAX_HPUPDOWN )
	{
	}
}

void CheckHpUpDown( void )
{
	int ani = 0;
	for(int  i = 0 ;  i < MAX_HPUPDOWN ; i ++)
	{
		if( HpUpDown[i].flag == false ) 
		{
			continue;
		}

		HpUpDown[i].hpupdown --;

		if( HpUpDown[i].hpupdown > 0 ) 
		{
			continue;
		}
					
		HpUpDown[i].flag = false;
					
		LPCHARACTER ch = ReturnCharacterPoint( HpUpDown[i].id );
		if( ch == NULL ) 
		{
			continue;
		}
		
		SendReqHpOfParty(ch->name);
		::AddNumberAni( NUMBER_ANIMATION_TYPE_DOWN_, ch->y+1, ch->x, ch->y - 44, HpUpDown[i].hp );
#ifdef _DEBUG // finito 060507		
		if( GetSysInfo( SI_GAME_MAKE_MODE ) )
		{
			ch->_Hp += HpUpDown[i].hp;
		}
#endif					
		switch( HpUpDown[i].attackedresult )
		{			
		case 1 :	// Damage and Death..
			{
				if( ch == Hero )
				{
					SCharacterData.nCharacterHP = 0;
					SCharacterData.nCharacterMP = 0;
					SCharacterData.condition	= CON_DEATH;

					Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
				}
				ch->hp = 0;
				ch->mp = 0;
				ch->condition = CON_DEATH;

				switch( ch->animationType )
				{
					case ANIMATION_TYPE_MAN   :
					case ANIMATION_TYPE_WOMAN :	ani =  ACTION_DIE;	break;
					case ANIMATION_TYPE_MON1  :	ani =  MON1_DIE;	break;
				}

				if( ani != ch->nCurrentAction ) 
				{
					CharDoAni( ch, ch->direction, ani );
				}
				SkillSettingClear();// 혹시 기술을 사용하고 있는 중이었으면 기술에 관해 Setting되어 있던 부분을 없앤댜.
				ch->dummy_pk = 0;
			}break;
		case 0 :// Failed..
			{
				ch->NumberAnimationValue.PutEncrypted(0);
			}break;
		case 2 : 	// Damage and Not Death..
			{
				if( ch == Hero )
				{ //< CSD-020820
					ch->hp += HpUpDown[i].hp;
					SCharacterData.nCharacterHP += HpUpDown[i].hp;
					if( SCharacterData.nCharacterHP < 0 ) 
					{
						SCharacterData.nCharacterHP = 1;

						Hero->hp = SCharacterData.nCharacterHP; //Eleval 18/08/09 - HP BUG
					}
				}
			}break;	
		default :	
			__asm nop;
			break;	
		}			
		ch->NumberAnimationValue.PutEncrypted(0);
	}
}

void DisplayChatAndNumber( void )
{
	DWORD color, bcolor;
	int number;
	int r,g,b;																								
	for (register int i = 0; i < g_OrderInfo.count; i++ )									
	{																
		switch ( g_OrderInfo.order[ i ].wType )													
		{
		case	SPRITETYPE_CHARACTER:								
		case	SPRITETYPE_MONSTER  :						
		case	SPRITETYPE_NPC      :								
			{																	
				LPCHARACTER		ch = ( LPCHARACTER )g_OrderInfo.order[ i ].lpvData;
				::DisplayChatMessage( ch );														
			}break;
		case	SPRITETYPE_NUMBER:
			{
				LPORDERTABLE o = &g_OrderInfo.order[ i ];							
				o->show = 0;
				if( tool_ViewNumber ){break;}

				LPNUMBERANI n = ( LPNUMBERANI ) o->lpvData;
				switch( n->type )
				{
				case NUMBER_ANIMATION_TYPE_DOWN_:
					{
						if( n->number == 0 )								
						{																		
							Getcolor(&r, &g, &b );
							Hcolor( 0, 0xaa,0 );
							Hprint( n->x - Mapx - 30, n->y - Mapy, g_DestBackBuf, "Miss" );
						}															
						else								
						{												
							number = abs(n->number);
							if( number <= 50 ) 		   { Hcolor(    0,  255,  95  );}
							else if( number <=   75 )  { Hcolor(    0,  255, 135  );}
							else if( number <=  100 )  { Hcolor(    0,  255, 175  );}
							else if( number <=  180 )  { Hcolor(    0,  255, 215  );}
							else if( number <=  250 )  { Hcolor(    0,  248, 255  );}
							else if( number <=  300 )  { Hcolor(    0,  208, 255  );}
							else if( number <=  400 )  { Hcolor(    0,  168, 255  );}
							else if( number <=  500 )  { Hcolor(    0,  128, 255  );}
							else if( number <=  600 )  { Hcolor(    0,   88, 255  );}
							else if( number <=  730 )  { Hcolor(   20,    8, 255  );}
							else if( number <=  870 )  { Hcolor(   60,    8, 255  );}
							else if( number <= 1000 )  { Hcolor(  100,    8, 255  );}
							else if( number <= 1150 )  { Hcolor(  140,    8, 255  );}
							else if( number <= 1350 )  { Hcolor(  180,    8, 255  );}
							else if( number <= 1500 )  { Hcolor(  225,    8, 255  );}
							else if( number <= 1700 )  { Hcolor(  255,    0, 160  );}
							else if( number <= 1900 )  { Hcolor(  255,    0, 120  );}
							else if( number <= 2100 )  { Hcolor(  255,    5,  80  );}
							else if( number <= 2330 )  { Hcolor(  255,    5,  40  );}
							else if( number <= 2560 )  { Hcolor(  255,   40,   8  );}
							else if( number <= 2800 )  { Hcolor(  255,   80,   8  );}
							else if( number <= 3200 )  { Hcolor(  255,  120,   8  );}
							else if( number <= 3600 )  { Hcolor(  255,  160,   8  );}
							else if( number <= 4000 )  { Hcolor(  255,  200,   8  );}
							else   {                     Hcolor(  255,  255,   8  );}
							Hprint2( n->x - Mapx - 30, n->y - Mapy, g_DestBackBuf,"%d",number);
						}
					}break;//case NUMBER_ANIMATION_TYPE_DOWN_:
				case NUMBER_ANIMATION_TYPE_UP_	:	
					{
						number = abs(n->number);
						if( number      <=   50 ) { color = RGB(    0,  255,  95   ); bcolor = RGB(   0,  185,   5); }
						else if( number <=   75 ) { color = RGB(    0,  255, 135   ); bcolor = RGB(   0,  185,  55); }
						else if( number <=  100 ) { color = RGB(    0,  255, 175   ); bcolor = RGB(   0,  185,  95); }
						else if( number <=  180 ) { color = RGB(    0,  255, 215   ); bcolor = RGB(   0,  185, 125); }
						else if( number <=  250 ) { color = RGB(    0,  248, 255   ); bcolor = RGB(   0,  188, 125); }
						else if( number <=  300 ) { color = RGB(    0,  208, 255   ); bcolor = RGB(   0,  148, 125); }
						else if( number <=  400 ) { color = RGB(    0,  168, 255   ); bcolor = RGB(   0,   88, 125); }
						else if( number <=  500 ) { color = RGB(    0,  128, 255   ); bcolor = RGB(   0,   48, 155); }
						else if( number <=  600 ) { color = RGB(    0,   88, 255   ); bcolor = RGB(   0,    8, 125); }
						else if( number <=  730 ) { color = RGB(   20,    8, 255   ); bcolor = RGB(   0,    8, 155); }
						else if( number <=  870 ) { color = RGB(   60,    8, 255   ); bcolor = RGB(   0,    8, 155); }
						else if( number <= 1000 ) { color = RGB(  100,    8, 255   ); bcolor = RGB(  40,    8, 155); }
						else if( number <= 1150 ) { color = RGB(  140,    8, 255   ); bcolor = RGB(  00,    8, 155); }
						else if( number <= 1350 ) { color = RGB(  180,    8, 255   ); bcolor = RGB(  40,    8, 155); }
						else if( number <= 1500 ) { color = RGB(  225,    8, 255   ); bcolor = RGB( 185,    8, 155); }
						else if( number <= 1700 ) { color = RGB(  255,    0, 160   ); bcolor = RGB( 185,    0,  60); }
						else if( number <= 1900 ) { color = RGB(  255,    0, 120   ); bcolor = RGB( 185,    0,  20); }
						else if( number <= 2100 ) { color = RGB(  255,    5,  80   ); bcolor = RGB( 185,    5,   0); }
						else if( number <= 2330 ) { color = RGB(  255,    5,  40   ); bcolor = RGB( 185,    5,   0); }
						else if( number <= 2560 ) { color = RGB(  255,   40,   8   ); bcolor = RGB( 185,    0,   8); }
						else if( number <= 2800 ) { color = RGB(  255,   80,   8   ); bcolor = RGB( 185,   20,   8); }
						else if( number <= 3200 ) { color = RGB(  255,  120,   8   ); bcolor = RGB( 185,   60,   8); }
						else if( number <= 3600 ) { color = RGB(  255,  160,   8   ); bcolor = RGB( 185,   40,   8); }
						else if( number <= 4000 ) { color = RGB(  255,  200,   8   ); bcolor = RGB( 185,  140,   8); }
						else  { color = RGB(  255,  255,   8  );   bcolor = RGB(185,  180,   8 ); } 

						HprintBold( n->x - Mapx - 30, n->y - Mapy, color, bcolor, "%d", number);
					}break;
				}//switch( n->type )
			}break;//case	SPRITETYPE_NUMBER:
		default:
			{
			}break;
		}
	}
}
