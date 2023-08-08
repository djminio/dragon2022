#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <math.h>

#include "dragon.h"

#include "hong_sprite.h"

#include "char.h"
#include "Map.h"
#include "item.h"
#include "effect.h"
#include "gameProc.h"
#include "hong_sub.h"
#include "tool.h"
#include "CharDataTable.h"

#define		FIGHT_RANGE		12


void Display_Fight_Time(int);	//1:1 �������� ����
void Check_Time(void);
void Check_Length(int, int);
void Fight_Stone_End(void);
void SetFightMode(int target_id, int	stone_X, int stone_Y);
//void One_Print(void);


void DisplayFightStone(void)
{
	int hero_length;
	static int fram_check=0;
	int color;

	if(Hero->fight_flag == 1)	// 1:1 ����尡 �Ǹ� ����..
	{
		int display_x, display_y;			//	���� ���� ��ǥ..
		int radius = FIGHT_RANGE*TILE_SIZE;	//������..	//��..���̷���??

		display_x = Hero->stone_x - g_Map.x * TILE_SIZE;	//����� ȭ����� ��ǥ...
		display_y = Hero->stone_y - g_Map.y * TILE_SIZE;	//

		DWORD early = (g_ClientTime%16);	//���ɿ� �������� �����ϱ� ����
		
		if((early >=2 && early <= 3) || (early >=8 && early <= 9) || (early >=12 && early <=13))
		{
			color = (int)( RGB16( 40, 180, 40 ) );
			Circle(display_x, display_y, radius+1, color);
			color = (int)( RGB16( 100, 255, 100 ) );
			Circle(display_x, display_y, radius-1, color);
			//color = (int)( RGB16( 50, 200, 80 ) );
			//Circle(xc, yc, radius+1, color);
		}
		
		//�Ÿ����ϴ� ����.. sqrt((pow(x1-x2), 2) + (pow(y1-y2), 2))..........
		hero_length = (int)sqrt(pow((float)Hero->stone_x - Hero->x, (float)2) + pow((float)Hero->stone_y - Hero->y, (float)2));
		
		Check_Length(FIGHT_RANGE*TILE_SIZE, hero_length);	//452, 469
		
		Check_Time();
	}
}


/*
////////////  ���̿ܿ� �ѻ���� �ִ°�  ////////////
int Fight_search(void)
{
	LPCHARACTER lpCharacter;

	int count=0;

	lpCharacter = Hero;
	while(lpCharacter != NULL)
	{
		if(lpCharacter == Hero )	lpCharacter->lpNext;
		else	
		{	
			lpCharacter->lpNext;
			count++;
		}
	}
	return count;
}
*/


void Check_Length(int length, int hero_length)	//������ �����.. ��..
{
	if(length < hero_length)
	{
		SendCMD_CHARACTER_DEMAND(CCD_FIGHT_LOSE, Hero->id);		//����:Ŀ��� �޽��� #define��Ī,��ǥid
		Hero->fight_flag = 0;
	}
}

void Check_Time(void)
{	
	DWORD clinet_time = g_ClientTime/1000;
	DWORD delay = (clinet_time - Hero->fight_time);	//������ 6�� ����..
	if(!delay)  return;
		
	if( delay%30 ==0 )
	{	
		Display_Fight_Time(delay/30);
		if(delay >= 300)	//5���� �������..
		{
			SendCMD_CHARACTER_DEMAND(CCD_FIGHT_DRAW, Hero->id);		//����:Ŀ��� �޽��� #define��Ī,��ǥid
			Hero->fight_flag = 0;
		}
	}	
}		
		
static int old_degree=0;

void Display_Fight_Time(int degree)			//degree -> 1~5 30�ʴ�..
{
	if(old_degree==degree)	return;			//�� �ѹ��� ����� ����
	
	if(degree==1)
	{
		AddCurrentStatusMessage(255, 120, 30, lan->OutputMessage(3,198), degree*30);//lsw
		old_degree = degree;
	}
	else if( degree && !((degree*30)%60) )
	{
		AddCurrentStatusMessage(255, 120, 30, lan->OutputMessage(3,199), (degree*30)/60);//lsw
		old_degree = degree;
	}
	else if( degree && (degree*30)%60 )
	{
		AddCurrentStatusMessage(255, 120, 30, lan->OutputMessage(3,200), (degree*30)/60, (degree*30)-((degree*30)/60)*60);//lsw
		old_degree = degree;
	}
}

void Fight_Stone_End(void)
{
	//LPCHARACTER		lpTarget;
	//if(Hero->fight_id!=NULL)
	//	lpTarget = ReturnCharacterPoint(Hero->fight_id);

	Hero->fight_flag= 0;
	Hero->stone_x	= 0;
	Hero->stone_y	= 0;
	Hero->fight_id	= 0;
	Hero->fight_time= 0;

	/*
	if(lpTarget!=NULL)
	{
		lpTarget->fight_flag = 0;
		lpTarget->stone_x	= 0;
		lpTarget->stone_y	= 0;
		lpTarget->fight_id	= 0;
		lpTarget->fight_time= 0;
	}
	*/
}


void SetFightMode(int target_id, int	stone_X, int stone_Y)			//���� ���� 1��1 ������� ����
{
	Hero->fight_flag = 1;
	Hero->stone_x = stone_X;
	Hero->stone_y = stone_Y;
	Hero->fight_id = target_id;
	Hero->fight_time = g_ClientTime/1000;			//�ʴ��� �ð��� �־��ش�.
	
	t_packet	packet;
	packet.h.header.type =  CMD_SERVER_FIGHT_STONE;
	packet.h.header.size = sizeof( t_fight_stone );
	packet.u.angra.server_fight_stone.fight_flag = Hero->fight_flag;
	packet.u.angra.server_fight_stone.fight_id   = Hero->fight_id;

	QueuePacket( &packet, 1 );
}
