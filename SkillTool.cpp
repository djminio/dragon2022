#include "stdafx.h"
#include "stdio.h"
#include <windowsx.H>
#include "Resource.h"
#include "SkillTool.h"
#include "Map.h"
#include "gameproc.h"
#include "Char.h"
#include "Tool.h"
#include "Hong_Sub.h"

#include "Effect.h"
#include "Hong_sprite.h"
#include "directdraw.h"
#include "dragon.h"
#include "object.h"

HWND			InputSkillTableHwnd;
HWND			CheckHouseObjectHwnd;
//< CSD-030324	
char subFarmType[20][25] = {{""},};
char subMineType[20][25] = {{""},};
char subHubType[20][25] = {{""},};
//> CSD-030324
int             radio_Statue = 0;

MAPSKILLTABLE	g_MapSkillTable;
lpMAPSKILLTABLE Header[8][8];
MYHOUSETOOL		g_MyhouseTool;
DRAGMOUSE		g_DragMouse;
BOOL			g_MyhouseDlgOpen;

extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
//--------------------------------------------------------------------------------------------------------------
//  기  능 :  NPC배치를 지운다. 
//  인  자 : 0 :모든 NPC배치를 없앤다. 
//           n : n번 NPC배치를 없앤다. 
//  결  과 : 없슴.
//--------------------------------------------------------------------------------------------------------------
//////////////////////// 0613 lkh 추가 /////////////////////////
void DeleteNPCGenerate(int npc_Num)
{
	for( int y=0; y<g_Map.file.wHeight; y++)
	{
		for(int x=0; x<g_Map.file.wWidth; x++)
		{
			lpMAPSKILLTABLE result;
			
			if(TileMap[x][y].attr_skill == 1)
			{
				result=FindSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], x, y);
				
				if(result==NULL)
					TileMap[x][y].attr_skill = 0;
				else if(result!= NULL && result->skillno == 6)		// npc생성 포인트 관련 데이터인 경우
				{
					if(!npc_Num)
					{
						TileMap[x][y].attr_skill=0;
						DeleteSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], result);
					}
					else if(result->type_Num == npc_Num)
					{
						TileMap[x][y].attr_skill=0;
						DeleteSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], result);
					}
				}
			}
		}
	}
}




void ChangeNPCGenerate(int npc_from, int npc_to )
{
	for( int y=0; y<g_Map.file.wHeight; y++)
	{
		for(int x=0; x<g_Map.file.wWidth; x++)
		{
			lpMAPSKILLTABLE result;
			
			if(TileMap[x][y].attr_skill == 1)
			{
				result=FindSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], x, y);
				
				if(result==NULL)
					TileMap[x][y].attr_skill = 0;
				else if(result!= NULL && result->skillno == 6)		// npc생성 포인트 관련 데이터인 경우
				{
					if( result->type_Num == npc_from )
					{
						result->type_Num = npc_to;
					}
				}
			}
		}
	}
}



void DeleteAllSkillData( void )
{
	for( int i = 0 ; i < 8 ; i ++ )
	{
		for( int j = 0 ; j < 8 ; j ++ )
		{
			while( Header[i][j] )
			{
				DeleteSkill( &Header[i][j], Header[i][j] );
			}
		}
	}
}

/*id DeleteAllSkillData( void )
{
	for( int y=0; y<g_Map.file.wHeight; y++)
	{
		for(int x=0; x<g_Map.file.wWidth; x++)
		{
			lpMAPSKILLTABLE result;
			
			if(TileMap[x][y].attr_skill == 1)
			{	
				result=FindSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], x, y);
				if(result==NULL)
					TileMap[x][y].attr_skill = 0;
				else
				{
					TileMap[x][y].attr_skill = 0;
					DeleteSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], result);
				}
			}
		}
	}
}*/  // 0907 KHS



void DrawSkillBox(int mox, int moy)
{
	int sx=(mox-Mapx)/TILE_SIZE*32;
	int sy=(moy-Mapy)/TILE_SIZE*32;
	Box( sx+1, sy+1, sx+TILE_SIZE-1, sy+TILE_SIZE-1, RGB(29,184,12));
}

BOOL LoadSkillMapTable(void)
{
	int		i=0;
	char	temp[FILENAME_MAX];
	FILE *fp ;
	MAPSKILLTABLE st;
	int c=0;
	int ret;
	
 	sprintf( temp, "./skill/%s.skb", MapName );
	fp = Fopen( temp, "rb" );
	if(fp == NULL) 		return FALSE;

	int dx = (g_Map.file.wWidth+7)/8;
	int dy = (g_Map.file.wHeight+7)/8;

	while( !feof( fp ) )
	{
		ret = fread( &st, sizeof( MAPSKILLTABLE), 1, fp );
		if( ret < 1 ) break;

		int x = (int)(st.x / dx);
		int y = (int)(st.y / dy);
		TileMap[ st.x ][ st.y ].attr_skill = 1;
		AddSkill( &Header[ x][y], &st );
		c++;
	}

	fclose(fp);

	return TRUE;
}


void MapSkillTool_Attr( int mx, int my )		//해당 타일의 기술 속성값 활성 여부 셋팅함수
{
	if( mx < 0 ) return;
	if( my < 0 ) return;

	if( g_Map.file.wWidth <= mx ) return;
	if( g_Map.file.wHeight <= my ) return;
	
	LPTILE t = &TileMap[ mx][my];

	if( g_MapSkillTable.skillno!=0 )
	{
		if( t->attr_skill==FALSE )		//해당 타일의 기술 속성을 최초로 셋팅하는 경우
		{
			t->attr_skill = 1;
			AddSkill( &Header[(int)(mx/(int)((g_Map.file.wWidth+7)/8))][(int)(my/(int)((g_Map.file.wHeight+7)/8))], &g_MapSkillTable);
		}
		else							//해당 타일의 기술 속성에 이미 속성이 셋팅되어 있는 경우
		{
			lpMAPSKILLTABLE	result;
			result=FindSkill( &Header[(int)(mx/(int)((g_Map.file.wWidth+7)/8))][(int)(my/(int)((g_Map.file.wHeight+7)/8))], mx, my);
			if(result==NULL)		return;		//그런 일은 없겠지만 해당좌표값을 갖는 링크리스트 없는 경우
	
			result->x = g_MapSkillTable.x;
			result->y = g_MapSkillTable.y;
			result->type = g_MapSkillTable.type;
			result->skillno = g_MapSkillTable.skillno;

			if(result->skillno == TOOL_DONTSKILL )
			{
				result->tile_Range	= 0;
				result->probability	= 0;
				result->type_Num	= 0;
				result->subType		= 0;
			}
			else if(result->skillno == TOOL_BUILDHOUSE)
			{
				result->tile_Range	= g_MapSkillTable.tile_Range;
				result->probability	= 0;
				result->type_Num	= 0;
				result->subType		= 0;
			}
			else
			{
				result->tile_Range	= g_MapSkillTable.tile_Range;
				result->probability	= g_MapSkillTable.probability;
				if(result->skillno == TOOL_NPC_GENER )
				{
					result->type_Num= g_MapSkillTable.type_Num;
					result->subType = 0;
				}
				else
				{
					result->type_Num= 0;
					result->subType	= g_MapSkillTable.subType;
				}
			}
		}
	}
	return;
}


void AddSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE	lpST )		//링크드 리스트의 머리에 붙여 넣기
{		
	lpMAPSKILLTABLE t, temp; 
	
	if( *Header == NULL )		//링크드 리스트 최초 생성
	{	
		MemAlloc( *Header, sizeof( MAPSKILLTABLE ));
		(*Header)->type			= lpST->type;
		(*Header)->x			= lpST->x; 
		(*Header)->y			= lpST->y; 
		(*Header)->skillno		= lpST->skillno;
		
		(*Header)->tile_Range	= lpST->tile_Range;
		(*Header)->probability	= lpST->probability;
		(*Header)->type_Num		= lpST->type_Num;
		(*Header)->subType		= lpST->subType;
		(*Header)->prev			= NULL;
		(*Header)->next			= NULL;
	}		
	else						//이미 생성된 리스트에 첨가 연결/로딩해온 데이터가 있을 경우
	{	
		t = NULL;
		MemAlloc( t, sizeof( MAPSKILLTABLE ));

		t->type			= lpST->type;
		t->x			= lpST->x; 
		t->y			= lpST->y; 
		t->skillno		= lpST->skillno;
		
		t->tile_Range	= lpST->tile_Range;
		t->probability	= lpST->probability;
		t->type_Num		= lpST->type_Num;
		t->subType		= lpST->subType;

		temp = *Header ;
		temp->prev = t;
		t->next = temp;
		t->prev = NULL;
		*Header = t;
	}
	/*	
		if(lpST->skillno == TOOL_FARMING || lpST->skillno == TOOL_MINING || lpST->skillno == TOOL_FISHING ||
			lpST->skillno == TOOL_CHOPPING || lpST->skillno == TOOL_HUB )
		{
			t->tile_Range	= lpST->tile_Range;
			t->probability	= lpST->probability;
			t->type_Num		= 0;
			t->subType		= lpST->subType;
		}

		else if(lpST->skillno == TOOL_NPC_GENER )
		{
			t->tile_Range	= lpST->tile_Range;
			t->probability	= lpST->probability;
			t->type_Num		= lpST->type_Num;
			t->subType		= 0;
		}

		else if(lpST->skillno == TOOL_BUILDHOUSE )
		{
			t->tile_Range	= lpST->tile_Range;
			t->probability	= 0;
			t->type_Num		= 0;
			t->subType		= 0;
		}
		
		temp = *Header ;
		temp->prev = t;
		t->next = temp;
		t->prev = NULL;
		*Header = t;
	}	
	*/
}	
	
	
void DeleteSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE f)
{	
	lpMAPSKILLTABLE  t = *Header;//g_lpMapSkillTable;
	
	while( t != NULL )
	{		
		if( t == f )
		{
			if( f == *Header )		//header가 제거되는 경우
			{
				t = (*Header)->next;

				if(*Header!=NULL)
					MemFree( *Header );

				if(t!=NULL)
				{
					*Header = t;
					(*Header)->prev = NULL;
				}
				return;
			}
			else 
			{
				if ( t->prev != NULL )
				{
					t->prev->next = t->next;
				}
				if( t->next != NULL )
				{
					t->next->prev = t->prev;
				}

				MemFree( t );
				return;
			}
		}

		t= t->next;
	}
}	
	
//	맵은 절대좌표...
// 010314 KHS  
lpMAPSKILLTABLE FindSkill( lpMAPSKILLTABLE *H, int x, int y, int order )
{	
	int c = 0;
	lpMAPSKILLTABLE t;
	t = *H;
	while( t != NULL )
	{	
		if( t->x == x && t->y == y )
		{
			if( order == c ) return t;
			c++;
		}
		t = t->next;
	}		
	return NULL;
}	
	
	
///////////////////////////// 0724 lkh 수정 ///////////////////////////////
BOOL CALLBACK SkillToolproc( HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam )
{	
	char			temp[FILENAME_MAX];
	static int		map_X, map_Y;
	static POINT	checkON_Tile;
	int				i=0,j=0;
	static int		radio_Table;
	static int		skill_Tile_Range=DEF_SKILLTILE_RANGE;
	int				Farmming_Count;
	int				Mining_Count;
	int				Fishing_Count;
	int				Chopping_Count;
	int				Hub_Count;
	int				NPC_Count;
	int				NPCPositionCount;
	int				NPCEventCount;	
	int				NPCNoEventCount;
	static int		delete_Type;
	FILE			*fp;
	RECT			rect, grect;	
	
	int tempmonsterno;
	
	switch(Message)		
	{						
	case WM_INITDIALOG:	
		map_X=Mox/32;
		map_Y=Moy/32;
		sprintf (temp, "%d", g_MapSkillTable.skillno);
		Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//기술속성
	
		sprintf (temp, "%d", g_MapSkillTable.x);
		Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_TILE_X ), temp);	//선택된 타일 종좌표		

		sprintf (temp, "%d", g_MapSkillTable.y);
		Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_TILE_Y ), temp);	//선택된 타일 횡좌표

		sprintf (temp, "%d", g_MapSkillTable.tile_Range);
		Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);	//속성영향이 미치는 거리(반지름)

		sprintf (temp, "%d", g_MapSkillTable.type_Num);
		Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_NPCNUM ), temp);	//선택된 NPC 번호
		//< CSD-030419
		if (IsExistNpcSprNo(g_MapSkillTable.type_Num))
		{
			tempmonsterno = g_MapSkillTable.type_Num;
		}
		else
		{
			tempmonsterno = g_MapSkillTable.type_Num%100;
		}
		
		Edit_SetText(GetDlgItem(hDlg, IDC_MONSTER_NAME), g_infNpc[tempmonsterno].szName);
		//> CSD-030419
		
		sprintf (temp, "%d", g_MapSkillTable.probability);
		Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);	//기술의 성공 여부 확률

		switch(delete_Type)
		{
		case 0:sprintf(temp, lan->OutputMessage(4,151));break;
		case 1:sprintf(temp, lan->OutputMessage(4,152));break;
		case 2:sprintf(temp,lan->OutputMessage(4,153) );break;
		case 3:sprintf(temp, lan->OutputMessage(4,154));break;
		case 4:sprintf(temp, lan->OutputMessage(4,155));break;
		case 5:sprintf(temp, lan->OutputMessage(4,156));break;
		case 6:sprintf(temp, lan->OutputMessage(4,157));break;
		case 7:sprintf(temp, lan->OutputMessage(4,158));break;
		}

		// 라디오 버튼 체크&출력

		switch( g_MapSkillTable.skillno )
		{
			case TOOL_FARMING				 :  radio_Table = IDC_FARMING;		break;
			case TOOL_MINING				 :	radio_Table = IDC_MINING;		break;
			case TOOL_FISHING				 : 	radio_Table = IDC_FISHING;		break;
			case TOOL_CHOPPING				 :	radio_Table = IDC_CHOPPING;		break;
			case TOOL_HUB					 :	radio_Table = IDC_HUB;			break;
			case TOOL_NPC_GENER				 :  radio_Table = IDC_NPC_GENER;	break;
			case TOOL_DONTSKILL				 :	radio_Table = IDC_DONTSKILL;	break;
				//default							 :	radio_Table = NULL;
		}
		CheckRadioButton( hDlg, IDC_FARMING, IDC_DONTSKILL, radio_Table );

		switch( radio_Statue )
		{
			case 0							 :  radio_Statue = IDC_RADIO_ALL;		break;
			case 1	          				 :	radio_Statue = IDC_RADIO_FARM;		break;
			case 2           				 : 	radio_Statue = IDC_RADIO_MINE;		break;
			case 3            				 :	radio_Statue = IDC_RADIO_FISH;		break;
			case 4             				 :	radio_Statue = IDC_RADIO_CHOPPING;	break;
			case 5            				 :  radio_Statue = IDC_RADIO_HUB;		break;
			case 6             				 :	radio_Statue = IDC_RADIO_NPC;		break;
				//default							 :	radio_Table = NULL;
		}
		CheckRadioButton( hDlg, IDC_RADIO_ALL, IDC_RADIO_NPC, radio_Statue );

	GetWindowRect( hDlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
	GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
	MoveWindow( hDlg, grect.right+1, 0, rect.right, rect.bottom, true);

		return TRUE;	

	case WM_PAINT:
		break;			

	case WM_CLOSE:
		tool_ID_SKILL_INPUT=0;
		break;
						
	case WM_COMMAND:	
		switch( LOWORD( wParam )) 
			{		
				case IDOK     :
						Edit_GetText( GetDlgItem(hDlg, IDC_VIEW_ATTRIB), temp, 9);
						g_MapSkillTable.skillno = atoi (temp );
						Edit_GetText( GetDlgItem(hDlg, IDC_INPUT_TILE_X), temp, 9);
						g_MapSkillTable.x = atoi (temp );
						Edit_GetText( GetDlgItem(hDlg, IDC_INPUT_TILE_Y), temp, 9);
						g_MapSkillTable.y = atoi (temp );
						Edit_GetText( GetDlgItem(hDlg, IDC_INPUT_RANGE), temp, 9);
						g_MapSkillTable.tile_Range = atoi (temp );
						Edit_GetText( GetDlgItem(hDlg, IDC_INPUT_PERCENT), temp, 9);
						g_MapSkillTable.probability = atoi (temp );
						Edit_GetText( GetDlgItem(hDlg, IDC_VIEW_NPCNUM), temp, 9);
						g_MapSkillTable.type_Num = atoi (temp );
						
						EndDialog(hDlg, LOWORD(wParam));
						tool_ID_SKILL_INPUT=0;
						return TRUE;

				case IDCANCEL :	
						EndDialog(hDlg, LOWORD(wParam));	
						tool_ID_SKILL_INPUT=0;
						return TRUE;
				
				case IDC_BUTTON_SAVE :

						if(YesOrNo( "SkilTable Save OK? ", "Warning" ) == IDYES )
						{
							sprintf( temp, "./Skill/%s.skb", MapName );
							fp = Fopen( temp, "wb" );
							if(fp)
							{
								int c=0;
								for( int a=0; a<8; a++)
									for(int b=0; b<8; b++)
									{
										lpMAPSKILLTABLE h=Header[a][b];
										while(h!=NULL)
										{
											fwrite(h, sizeof(MAPSKILLTABLE), 1, fp);
											c++;
											h=h->next;
										}
									}
								fclose(fp);
							}
							
							int step;
							int serial=1;

							sprintf( temp, "./Skill/%s.stx", MapName );
							fp = Fopen( temp, "wt" );
							if(fp)
							{
								SYSTEMTIME l;										
								GetLocalTime(&l);
								fprintf( fp, "\n MapName : %s \n", MapName );
								fprintf( fp, "   Date  : %4d.%2d.%2d %2d:%2d.%2d \n\n", l.wYear, l.wMonth, l.wDay, l.wHour, l.wMinute, l.wSecond );

								Farmming_Count=0; 
								Mining_Count=0; 
								Fishing_Count=0; 
								Chopping_Count=0; 
								Hub_Count=0; 
								NPC_Count=0;
								NPCPositionCount = 0;
								NPCEventCount = 0;
								NPCNoEventCount = 0;

								char temp[ FILENAME_MAX];
								sprintf( temp, "%s\n",lan->OutputMessage(4,161) );
								fprintf (fp, temp );

								for( step=1; step<8; step++)
								{
									for( int a=0; a<8; a++)
									{
										for(int b=0; b<8; b++)
										{
											lpMAPSKILLTABLE	h=Header[a][b];
											while(h!=NULL)
											{
												if(h->skillno==step)
												{
													fprintf(fp, "%3d	%3d	%3d	%3d	%3d	%3d	%3d	%3d\n",
													serial, h->skillno, h->x, h->y, h->tile_Range, h->probability, h->type_Num , h->subType );
													switch(h->skillno)
													{
													case 1:	Farmming_Count++;			break;
													case 2:	Mining_Count++;				break;
													case 3:	Fishing_Count++;			break;
													case 4:	Chopping_Count++;			break;
													case 5:	Hub_Count++;				break;
													case 6:	NPCPositionCount++; NPC_Count += h->tile_Range;	
															if( h->probability ) NPCEventCount ++;	else NPCNoEventCount++;
																break;
													}
													serial++;
												}
												h=h->next;
											}
										}	
									}
								}
								
								
								sprintf( temp, "%s\n", lan->OutputMessage(4,162) );
								fprintf(fp, temp,	Farmming_Count, Mining_Count, Fishing_Count, Chopping_Count, Hub_Count);
								sprintf( temp, "%s\n", lan->OutputMessage(4,163) );
								fprintf(fp, temp,	NPC_Count, NPCPositionCount, NPCEventCount, NPCNoEventCount );
									
								fclose(fp);
							}
						}	
							
						return TRUE;
							
				case IDC_MINER10:
					{	//< CSD-030419
						g_MapSkillTable.type_Num -= 10;

						if (g_MapSkillTable.type_Num < 0)
						{
							g_MapSkillTable.type_Num=0;
						}

						sprintf (temp, "%d", g_MapSkillTable.type_Num);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_NPCNUM ), temp);
						
						if (IsExistNpcSprNo(g_MapSkillTable.type_Num))
						{
							tempmonsterno = g_MapSkillTable.type_Num;
						}
						else
						{
							tempmonsterno = g_MapSkillTable.type_Num%100;
						}
						
						Edit_SetText (GetDlgItem(hDlg, IDC_MONSTER_NAME), g_infNpc[tempmonsterno].szName);
						return TRUE;
					}	//> CSD-030419
				case IDC_MINER1:
					{	//< CSD-030419
						g_MapSkillTable.type_Num -= 1;
						if(g_MapSkillTable.type_Num<0)	g_MapSkillTable.type_Num=0;
						sprintf (temp, "%d", g_MapSkillTable.type_Num);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_NPCNUM ), temp);
						
						if (IsExistNpcSprNo(g_MapSkillTable.type_Num))
						{
							tempmonsterno = g_MapSkillTable.type_Num;
						}
						else
						{
							tempmonsterno = g_MapSkillTable.type_Num%100;
						}
						
						Edit_SetText(GetDlgItem(hDlg, IDC_MONSTER_NAME ), g_infNpc[tempmonsterno].szName);
						return TRUE;
					}	//> CSD-030419
				case IDC_PLUS1:
					{	//< CSD-030419
						g_MapSkillTable.type_Num += 1;
						if(g_MapSkillTable.type_Num>599)	g_MapSkillTable.type_Num=599;
						sprintf (temp, "%d", g_MapSkillTable.type_Num);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_NPCNUM ), temp);
						
						if (IsExistNpcSprNo(g_MapSkillTable.type_Num))
						{
							tempmonsterno = g_MapSkillTable.type_Num;
						}
						else
						{
							tempmonsterno = g_MapSkillTable.type_Num%100;
						}
						
						Edit_SetText(GetDlgItem(hDlg, IDC_MONSTER_NAME), g_infNpc[tempmonsterno].szName);
						return TRUE;
					}	//> CSD-030419
				case IDC_PLUS10	:
					{	//< CSD-030419
						g_MapSkillTable.type_Num += 10;
						if(g_MapSkillTable.type_Num>599)	g_MapSkillTable.type_Num=599;
						sprintf (temp, "%d", g_MapSkillTable.type_Num);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_NPCNUM ), temp);
					
						if (IsExistNpcSprNo(g_MapSkillTable.type_Num))
						{
							tempmonsterno = g_MapSkillTable.type_Num;
						}
						else
						{
							tempmonsterno = g_MapSkillTable.type_Num%100;
						}
						
						Edit_SetText(GetDlgItem(hDlg, IDC_MONSTER_NAME), g_infNpc[tempmonsterno].szName);
						return TRUE;
					}	//> CSD-030419
				case IDC_BUTTON_UP :
						g_MapSkillTable.tile_Range++;
						//if(g_MapSkillTable.tile_Range>MAX_SKILLTILE_RANGE)	g_MapSkillTable.tile_Range=MAX_SKILLTILE_RANGE;
						sprintf (temp, "%d", g_MapSkillTable.tile_Range);
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						return TRUE;
				case IDC_BUTTON_DOWN: 
						g_MapSkillTable.tile_Range--;
						if(g_MapSkillTable.tile_Range<0)	g_MapSkillTable.tile_Range=0;
						sprintf (temp, "%d", g_MapSkillTable.tile_Range);
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						return TRUE;
				case IDC_MINER_PERCENT:
						g_MapSkillTable.probability -= 1; // finito changed value 10 -> 1
						if(g_MapSkillTable.probability<0)	g_MapSkillTable.probability=0;
						sprintf (temp, "%d", g_MapSkillTable.probability);
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_PLUS_PERCENT:
						g_MapSkillTable.probability += 1; // finito changed value 10 -> 1
						if(g_MapSkillTable.probability>100)	g_MapSkillTable.probability=100;
						sprintf (temp, "%d", g_MapSkillTable.probability);
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_FARMING:
						g_MapSkillTable.skillno = TOOL_FARMING;
						sprintf (temp, "%d", g_MapSkillTable.skillno);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//경작 속성
						g_MapSkillTable.tile_Range=0;
						sprintf (temp, "0");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						g_MapSkillTable.probability=0;
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_MINING:
						g_MapSkillTable.skillno = TOOL_MINING;
						sprintf (temp, "%d", g_MapSkillTable.skillno);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//채광 속성
						g_MapSkillTable.tile_Range=0;
						sprintf (temp, "0");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						g_MapSkillTable.probability=0;
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_FISHING:
						g_MapSkillTable.skillno = TOOL_FISHING;
						sprintf (temp, "%d", g_MapSkillTable.skillno);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//낚시 속성
						g_MapSkillTable.tile_Range=0;
						sprintf (temp, "0");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						g_MapSkillTable.probability=0;
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_CHOPPING:
						g_MapSkillTable.skillno = TOOL_CHOPPING;
						sprintf (temp, "%d", g_MapSkillTable.skillno);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//벌목 속성
						g_MapSkillTable.tile_Range=0;
						sprintf (temp, "0");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						g_MapSkillTable.probability=0;
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_HUB:
						g_MapSkillTable.skillno = TOOL_HUB;
						sprintf (temp, "%d", g_MapSkillTable.skillno);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//약초 속성
						g_MapSkillTable.tile_Range=0;
						sprintf (temp, "0");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						g_MapSkillTable.probability=0;
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_NPC_GENER:
						g_MapSkillTable.skillno = TOOL_NPC_GENER;
						sprintf (temp, "%d", g_MapSkillTable.skillno);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//NPC 생성 포인트 속성
						g_MapSkillTable.tile_Range=1;
						sprintf (temp, "1");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						g_MapSkillTable.probability=0;
						sprintf (temp, "0");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_DONTSKILL:
						g_MapSkillTable.skillno = TOOL_DONTSKILL;
						sprintf (temp, "%d", g_MapSkillTable.skillno);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//DON'T SKILL 속성
						g_MapSkillTable.tile_Range=0;
						sprintf (temp, "0");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						g_MapSkillTable.probability=0;
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_BUILD_HOUSE:
						g_MapSkillTable.skillno = TOOL_BUILDHOUSE;
						sprintf (temp, "%d", g_MapSkillTable.skillno);
						Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_ATTRIB ), temp);	//집짖기 속성
						g_MapSkillTable.tile_Range=0;
						sprintf (temp, "0");
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_RANGE ), temp);
						g_MapSkillTable.probability=0;
						Edit_SetText (GetDlgItem(hDlg, IDC_INPUT_PERCENT ), temp);
						return TRUE;
				case IDC_FARM_MINER:
						if(g_MapSkillTable.skillno==1)
						{
							g_MapSkillTable.subType --;
							if(g_MapSkillTable.subType<0)	g_MapSkillTable.subType=0;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBFARM ), temp);	//경작의 서브->토양속성
						}
						return TRUE;
				case IDC_FARM_PLUS:
						if(g_MapSkillTable.skillno==1)
						{
							g_MapSkillTable.subType ++;
							if(g_MapSkillTable.subType>20)	g_MapSkillTable.subType=20;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBFARM ), temp);	//경작의 서브->토양속성
						}
						return TRUE;
				case IDC_MINE_MINER:
						if(g_MapSkillTable.skillno==2)
						{
							g_MapSkillTable.subType --;
							if(g_MapSkillTable.subType<0)	g_MapSkillTable.subType=0;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBMINE ), temp);	//채광의 서브->광물속성
						}
						return TRUE;
				case IDC_MINE_PLUS:
						if(g_MapSkillTable.skillno==2)
						{
							g_MapSkillTable.subType ++;
							if(g_MapSkillTable.subType>20)	g_MapSkillTable.subType=20;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBMINE ), temp);	//채광의 서브->광물속성
						}
						return TRUE;
				case IDC_FISH_MINER:
						if(g_MapSkillTable.skillno==3)
						{
							g_MapSkillTable.subType --;
							if(g_MapSkillTable.subType<0)	g_MapSkillTable.subType=0;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBFISH ), temp);	//사는 물고기 종류
						}
						return TRUE;
				case IDC_FISH_PLUS:
						if(g_MapSkillTable.skillno==3)
						{
							g_MapSkillTable.subType ++;
							if(g_MapSkillTable.subType>20)	g_MapSkillTable.subType=20;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBFISH ), temp);	//사는 물고기 종류
						}
						return TRUE;
				case IDC_CUT_MINER:
						if(g_MapSkillTable.skillno==4)
						{
							g_MapSkillTable.subType --;
							if(g_MapSkillTable.subType<0)	g_MapSkillTable.subType=0;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBCUT ), temp);	//사는 물고기 종류
						}
						return TRUE;
				case IDC_CUT_PLUS:
						if(g_MapSkillTable.skillno==4)
						{
							g_MapSkillTable.subType ++;
							if(g_MapSkillTable.subType>20)	g_MapSkillTable.subType=20;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBCUT ), temp);	//사는 물고기 종류
						}
						return TRUE;
				case IDC_HUB_MINER:
						if(g_MapSkillTable.skillno==5)
						{
							g_MapSkillTable.subType --;
							if(g_MapSkillTable.subType<0)	g_MapSkillTable.subType=0;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBHUB ), temp);	//약초채취의 서브->약초속성
						}
						return TRUE;
				case IDC_HUB_PLUS:
						if(g_MapSkillTable.skillno==5)
						{
							g_MapSkillTable.subType ++;
							if(g_MapSkillTable.subType>20)	g_MapSkillTable.subType=20;
							sprintf (temp, "%d", g_MapSkillTable.subType);
							Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SUBHUB ), temp);	//약초채취의 서브->약초속성
						}
						return TRUE;

				case IDC_SCRIPT_MINER10:
						if(g_MapSkillTable.skillno == 6)
						{
							g_MapSkillTable.probability -= 10;		//NPC의 스크립트 번호
							if(g_MapSkillTable.probability<0)	g_MapSkillTable.probability=0;
							sprintf (temp, "%d", g_MapSkillTable.probability);
							Edit_SetText (GetDlgItem(hDlg, IDC_SCRIPT_NO ), temp);
						}
						return TRUE;
				case IDC_SCRIPT_MINER1:
						if(g_MapSkillTable.skillno == 6)
						{
							g_MapSkillTable.probability --;		//NPC의 스크립트 번호
							if(g_MapSkillTable.probability<0)	g_MapSkillTable.probability=0;
							sprintf (temp, "%d", g_MapSkillTable.probability);
							Edit_SetText (GetDlgItem(hDlg, IDC_SCRIPT_NO ), temp);
						}
						return TRUE;
				case IDC_SCRIPT_PLUS1:
						if(g_MapSkillTable.skillno == 6)
						{
							g_MapSkillTable.probability ++;		//NPC의 스크립트 번호
							if(g_MapSkillTable.probability>50)	g_MapSkillTable.probability=50;
							sprintf (temp, "%d", g_MapSkillTable.probability);
							Edit_SetText (GetDlgItem(hDlg, IDC_SCRIPT_NO ), temp);
						}
						return TRUE;
				case IDC_SCRIPT_PLUS10:
						if(g_MapSkillTable.skillno == 6)
						{
							g_MapSkillTable.probability += 10;		//NPC의 스크립트 번호
							if(g_MapSkillTable.probability>50)	g_MapSkillTable.probability=50;
							sprintf (temp, "%d", g_MapSkillTable.probability);
							Edit_SetText (GetDlgItem(hDlg, IDC_SCRIPT_NO ), temp);
						}
						return TRUE;
				case IDC_NPCNO_UP:			//NPC발생수
						if(g_MapSkillTable.skillno == 6)
						{
							g_MapSkillTable.tile_Range++;
							//if(g_MapSkillTable.tile_Range>10)	g_MapSkillTable.tile_Range=10;
							sprintf (temp, "%d", g_MapSkillTable.tile_Range);
							Edit_SetText (GetDlgItem(hDlg, IDC_GENER_NO ), temp);
						}
						return TRUE;
				case IDC_NPCNO_DOWN:		//NPC발생수
						if(g_MapSkillTable.skillno == 6)
						{
							g_MapSkillTable.tile_Range--;
							if(g_MapSkillTable.tile_Range<1)	g_MapSkillTable.tile_Range=1;
							sprintf (temp, "%d", g_MapSkillTable.tile_Range);
							Edit_SetText (GetDlgItem(hDlg, IDC_GENER_NO ), temp);
						}
						return TRUE;

				case IDC_RADIO_ALL:		radio_Statue=0;	return TRUE;
				case IDC_RADIO_FARM:	radio_Statue=1;	return TRUE;
				case IDC_RADIO_MINE:	radio_Statue=2;	return TRUE;
				case IDC_RADIO_FISH:	radio_Statue=3;	return TRUE;
				case IDC_RADIO_CHOPPING:radio_Statue=4;	return TRUE;
				case IDC_RADIO_HUB:		radio_Statue=5;	return TRUE;
				case IDC_RADIO_NPC:		radio_Statue=6;	return TRUE;
				case IDC_RADIO_HOUSE:	radio_Statue=7;	return TRUE;

				case IDC_DELETE_UP:
						delete_Type++;
						if(delete_Type>8)	delete_Type=8;
						
						switch(delete_Type)
						{
						case 1:
							sprintf(temp, lan->OutputMessage(4,151));//lsw
							break;
						case 2:
							sprintf(temp, lan->OutputMessage(4,152));
							break;
						case 3:
							sprintf(temp, lan->OutputMessage(4,153));
							break;
						case 4:
							sprintf(temp, lan->OutputMessage(4,154));
							break;
						case 5:
							sprintf(temp, lan->OutputMessage(4,155));
							break;
						case 6:
							sprintf(temp, lan->OutputMessage(4,156));
							break;
						case 7:
							sprintf(temp, lan->OutputMessage(4,157));
							break;
						case 8:
							sprintf(temp, lan->OutputMessage(4,158));//lsw
							break;
						}
						Edit_SetText (GetDlgItem(hDlg, IDC_DELETE_TYPE ), temp);
						return TRUE;

				case IDC_DELETE_DOWN:
						delete_Type--;
						if(delete_Type<1)	delete_Type=1;
						
						switch(delete_Type)
						{
						case 1:
							sprintf(temp, lan->OutputMessage(4,151));	break;
						case 2:
							sprintf(temp, lan->OutputMessage(4,152));	break;
						case 3:
							sprintf(temp, lan->OutputMessage(4,153));	break;
						case 4:
							sprintf(temp, lan->OutputMessage(4,154));	break;
						case 5:
							sprintf(temp, lan->OutputMessage(4,155));	break;
						case 6:
							sprintf(temp, lan->OutputMessage(4,156));	break;
						case 7:
							sprintf(temp, lan->OutputMessage(4,157));	break;
						case 8:
							sprintf(temp, lan->OutputMessage(4,158));	break;
						}
						Edit_SetText (GetDlgItem(hDlg, IDC_DELETE_TYPE ), temp);
						return TRUE;

				case IDC_DELETE_BUTTON:
					if(YesOrNo( "Select Tile infomation delete?", "warning" ) == IDYES )//lsw
						DeleteAllType(delete_Type);
					return TRUE;

			}			
			break;		
	}				
					
	return FALSE;	
}



void tool_MyHouseLBU( WPARAM wParam, LPARAM lParam )
{
	RECT	rect,grect;
////////////////////////////////// 이규훈 1124 내집 시스템 Tool ///////////////////////////////
		if(tool_ID_INPUT_MYHOUSE==1 && g_DragMouse.sx!=0 && g_MyhouseDlgOpen==0)
		{
			InputMyhouseHdlg = CreateDialog( g_hInstance, MAKEINTRESOURCE( IDD_INPUT_MYHOUSE ), g_hwndMain, (DLGPROC)MyhouseToolproc );
			ShowWindow( InputMyhouseHdlg, SW_HIDE);
			int ScreenX = GetSystemMetrics(SM_CXSCREEN);
			GetWindowRect( InputMyhouseHdlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
			GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
			ShowWindow( InputMyhouseHdlg, SW_SHOW);
			g_MyhouseDlgOpen=1;
		}
///////////////////////////////////////////////////////////////////////////////////////////////
}



void tool_MyHouseLBD( WPARAM wParam, LPARAM lParam )
{
	g_DragMouse.sx=LOWORD(lParam)+Mapx;
	g_DragMouse.ex=LOWORD(lParam)+Mapx;
	g_DragMouse.sy=HIWORD(lParam)+Mapy;
	g_DragMouse.ey=HIWORD(lParam)+Mapy;

	SetCapture( g_hwndMain);
	if(tool_ID_OUTPUT_MYHOUSE)
		PutMyhouse((LOWORD(lParam)+Mapx)/TILE_SIZE, (HIWORD(lParam)+Mapy)/TILE_SIZE);
}











void CheckHouseObjectEdit ( HWND hwnd, HINSTANCE hInstance)
{	
	//HINSTANCE  hinst;		//error?
	hInstance = LoadLibrary("riched32.dll");
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_INPUT_MYHOUSE ), NULL, (DLGPROC)MyhouseToolproc );
	FreeLibrary( hInstance /*hinst*/ );
}

BOOL CALLBACK MyhouseToolproc( HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam )
{
	char			temp[FILENAME_MAX];
	static int		map_X, map_Y;
	RECT rect, grect;
	static bool lButtonDown=false;
	int a,b;

	switch(Message)
	{
	case WM_INITDIALOG:
		/*
		map_X=Mox/32;
		map_Y=Moy/32;

		sprintf (temp, "%d", g_MyhouseTool.sx);
		Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SX ), temp);
		sprintf (temp, "%d", g_MyhouseTool.sy);
		Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_SY ), temp);
		sprintf (temp, "%d", g_MyhouseTool.ex);
		Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_EX ), temp);
		sprintf (temp, "%d", g_MyhouseTool.ey);
		Edit_SetText (GetDlgItem(hDlg, IDC_VIEW_EY ), temp);
		*/

		//CheckHouseObjectEdit = GetDlgItem(hDlg, IDC_OBJECT_EDIT );

		GetWindowRect( hDlg, &rect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
		GetWindowRect( g_hwndMain, &grect );// 화면 오른쪽이 붙어나오게 하려고 한건데 ... 어떻게 해야 할지..
		MoveWindow( hDlg, grect.right+1, 0, rect.right, rect.bottom, true);
		return TRUE;

	case WM_CLOSE:
		tool_ID_INPUT_MYHOUSE=0;
		g_MyhouseDlgOpen=0;
		memset(&g_DragMouse, 0 , sizeof(DRAGMOUSE));
		break;

	case WM_COMMAND:
		switch( LOWORD( wParam ))
		{
			case IDOK:
				EndDialog(hDlg, LOWORD(wParam));
				tool_ID_INPUT_MYHOUSE=0;
				g_MyhouseDlgOpen=0;
				
				memset(&g_DragMouse, 0 , sizeof(DRAGMOUSE));
				return TRUE;
			case IDCANCEL:	
				EndDialog(hDlg, LOWORD(wParam));
				tool_ID_INPUT_MYHOUSE=0;
				g_MyhouseDlgOpen=0;
				memset(&g_DragMouse, 0 , sizeof(DRAGMOUSE));
				return TRUE;
			case IDC_SAVE:
				if(YesOrNo( "내집 시스템 타일(이미지) Table을 저장합니다. ", "확인" ) == IDYES )
				{
					FILE*		fp;
					TILE*		temp_Tile;
					int			object_Count=0;
//					MYHOUSETOOL Myhouse; //TileMap

					sprintf( temp, "./map/%s.mhb", MapName );	//확장자->My House Binery
					fp = Fopen( temp, "wb" );
					if(fp)
					{
						/////////////////////////// 헤더부분 /////////////////////////
						int length_X=g_MyhouseTool.ex-g_MyhouseTool.sx;	//셀렉트하여 자른 복수타일의 종타일 길이
						fwrite(&length_X, sizeof(int), 1, fp);
						int length_Y=g_MyhouseTool.ey-g_MyhouseTool.sy; //셀렉트하여 자른 복수타일의 횡타일 길이
						fwrite(&length_Y, sizeof(int), 1, fp);
						int tile_Num=length_X*length_Y;					//셀렉트된 타일의 총갯수
						fwrite(&tile_Num, sizeof(int), 1, fp);
						
						//if(fgetpos(fp, temp_fp)==0)	
						//	;							//마지막에 오브젝트의 총 갯수를 체크한후 다시 셋팅하기 위해
						fwrite(&object_Count, sizeof(int), 1, fp);		//셀렉트된 타일 위에 위치한 총 오브젝트수 
						///////////////////////////////////////////////////////////////

						//////////////////////////// 타일의 정보 셋팅 //////////////////////////
						for( a=g_MyhouseTool.sx; a<=g_MyhouseTool.ex; a++)
							for( b=g_MyhouseTool.sy; b<=g_MyhouseTool.ey; b++)
							{
								temp_Tile=&TileMap[a][b];
								fwrite(temp_Tile, sizeof(TILE), 1, fp);
							}
						////////////////////////////////////////////////////////////////////////

						/////////////////////////// 오브젝트의 정보 셋팅 ///////////////////////////
						for(int i = 0 ; i < TotalMapObject ; i++ )
						{
							if( g_MyhouseTool.sx <= (Mo[i].x/TILE_SIZE) && g_MyhouseTool.ex >= (Mo[i].x/TILE_SIZE) &&
								g_MyhouseTool.sy <= (Mo[i].y/TILE_SIZE) && g_MyhouseTool.ey >= (Mo[i].y/TILE_SIZE) )
							{
								int temp_Mox=Mo[i].x;
								int temp_Moy=Mo[i].y;
								Mo[i].x -= (g_MyhouseTool.sx*TILE_SIZE);	//잘린 타일의 상대 좌표로 전환
								Mo[i].y -= (g_MyhouseTool.sy*TILE_SIZE);
								
								fwrite( &Mo[i], sizeof( MAPOBJECT ), 1, fp );
								Mo[i].x = temp_Mox;
								Mo[i].y = temp_Moy;
								object_Count++;
							}
						}
						///////////////////////////////////////////////////////////////////////////
						
						/*
						/////////////////////////// 지붕 타일 정보 셋팅 ///////////////////////////
						for(a=g_MyhouseTool.sx; a<=g_MyhouseTool.ex; a++)
							for(b=g_MyhouseTool.sy; b<=g_MyhouseTool.ey; b++)
							{
								temp_Tile=&TileMap[a][b];
								temp_Tile->attr_room;

									if ( TileMap[ a ][ b ].attr_room == 1 )		//지붕속성의 타일이며 지붕이 활성화(지붕이 가리고 있는 경우)
									{
										LPEVENTLIST		lpEventList;
										LPROOFGROUP		lpRoofGroup;
										LPROOF			lpRoof;

										lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )a, ( WORD )b );
										if ( lpEventList != NULL )
										{
											lpRoofGroup = FindRoofGroup( &g_RoofHeader, lpEventList->index );	//지붕 리스트에서 find
											if ( lpRoofGroup != NULL )
											{
												lpRoof = lpRoofGroup->lpFirst;
												while ( lpRoof )
												{
													TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 1;
													lpRoof = lpRoof->lpNext;
												}
											}
										}
									}
									else					//지붕속성 타일이며 지붕이 비활성화되어 있는 경우(뚜껑이 열려있는 경우)
									{
										LPEVENTLIST		lpEventList;
										LPROOFGROUP		lpRoofGroup;
										LPROOF			lpRoof;

										lpEventList = FindEventList( &g_EventListHeaderRoom, ( WORD )a, ( WORD )b );
										if ( lpEventList != NULL )
										{
											lpRoofGroup = FindRoofGroup( &g_RoofHeader, lpEventList->index );
											if ( lpRoofGroup != NULL )
											{
												lpRoof = lpRoofGroup->lpFirst;
												while ( lpRoof )
												{
													TileMap[ lpRoof->x ][ lpRoof->y ].show_roof = 0;

													lpRoof = lpRoof->lpNext;
												}
											}
										}
									}
								
							}
							*/
						///////////////////////////////////////////////////////////////////////////

						fseek(fp, 3*sizeof(int), SEEK_SET);
						fwrite(&object_Count, sizeof(int), 1, fp);
						fseek(fp, 0, SEEK_END);
						fclose(fp);
						g_MyhouseTool.object_Num=object_Count;
						sprintf (temp, "%d", g_MyhouseTool.object_Num);
						Edit_SetText (GetDlgItem(InputMyhouseHdlg, IDC_VIEW_TILENUM ), temp);
					}
				}
				return TRUE;
			
			//case IDC_OBJECT_EDIT:		richEdit 메시지 처리
			//	return TRUE;
		}				
	}				
					
	return FALSE;	
}

void PutMyhouse(int x, int y)		// 단 인자 x,y는 절대 타일좌표계로 넘어와야 함
{
	char			temp[FILENAME_MAX];
	static int		map_X, map_Y;
///	RECT			rect, grect;
	static bool		lButtonDown=false;
	int				a,b;
	FILE*			fp;
	TILE			temp_Tile;
	int				object_Count=0;
//	MYHOUSETOOL		Myhouse; //TileMap
	MAPOBJECT		temp_Object;

	sprintf( temp, "./map/%s.mhb", MapName );	//확장자->My House Binery
	fp = Fopen( temp, "rb" );
	if(fp)
	{
		//////////////////////////////// 헤더부분 //////////////////////////////
		int length_X;//=g_MyhouseTool.ex-g_MyhouseTool.sx;	//셀렉트하여 자른 복수타일의 종타일 길이
		fread(&length_X, sizeof(int), 1, fp);
		int length_Y;//=g_MyhouseTool.ey-g_MyhouseTool.sy; //셀렉트하여 자른 복수타일의 횡타일 길이
		fread(&length_Y, sizeof(int), 1, fp);
		int tile_Num;//=length_X*length_Y;					//셀렉트된 타일의 총갯수
		fread(&tile_Num, sizeof(int), 1, fp);
				
		fread(&object_Count, sizeof(int), 1, fp);		//셀렉트된 타일 위에 위치한 총 오브젝트수 
		////////////////////////////////////////////////////////////////////////

		//////////////////////////// 타일의 정보 셋팅 //////////////////////////
		for( a=x; a<=x+length_X; a++)
			for( b=y; b<=y+length_Y; b++)
			{
				fread(&temp_Tile, sizeof(TILE), 1, fp);
				TileMap[a][b]=temp_Tile;
			}
		////////////////////////////////////////////////////////////////////////

		/////////////////////////// 오브젝트의 정보 셋팅 ///////////////////////////
		for(int i = 0 ; i < object_Count ; i++ )
		{
			fread(&temp_Object, sizeof(MAPOBJECT), 1, fp);
			temp_Object.x += x;		//offset 좌표 떨구기->32(TILE_SIZE)를 곱해야지
			temp_Object.x *= TILE_SIZE;
			temp_Object.y += y;
			temp_Object.y *= TILE_SIZE;
			//temp_Object.id += MapObjectLevel;			//해당 레벨의 오브젝트 고유번호 옵셋만큼 더함(1000단위)
			Mo[TotalMapObject+i]=temp_Object;
		}
	}
}
//------------------------------------------------




void GetEffect2Pix(void)
{
	// EffectDataLoad...
	
	int j=0;
	char filename[141][30];

	FILE* file;
	int temp = 0;
	int buff = 0;
	char buf[30];

//	buf = (char*) calloc(30, sizeof(char));
//	filename = (char*) calloc( 141, sizeof(char)*30);

	char  DataPath[MAX_PATH]="./effect/list.txt";
	
	file = Fopen( DataPath, "rt" );			//read only+text file
	
	if(file)
	{
		for(int i=0; i<141; i++)
		{
			fscanf( file, "%s\n", buf);
//			strcpy(filename[i], buf);
			sprintf(filename[i], "./effect/%s",buf);
		}
		fclose( file );
	}

	for(int i=1; i<142;i++)
	{
		if(LoadEffectSprite(i, filename[i-1]))
		{
			for (int j = 0; j < Effspr[i].Count; j++)
			{

#ifndef _SDL
				g_DestBackBuf = GetSurfacePointer(g_DirectDrawInfo.lpDirectDrawSurfaceBack);
				EraseScreen(&g_DirectDrawInfo, RGB(0x00, 0x00, 0x00));

				PutCompressedImage(320, 240, &Effspr[i].EffSpr[j]);

				FlipScreen(&g_DirectDrawInfo);
				CaptureScreen();
			}
#else // _SDL
			}
			    g_DestBackBuf = SDL_GetSurfacePointer();
			    SDL_EraseScreen();

				PutCompressedImage(320, 240, &Effspr[i].EffSpr[j]);

				SDL_FlipScreen();
				CaptureScreen();
#endif
		}
	}
}


//--------------------------------------------------
		
void DeleteAllType(int delete_Type)
{
	lpMAPSKILLTABLE	temp;
	for( int i=0; i<8; i++)
	{
		for( int j=0; j<8; j++)
		{
			lpMAPSKILLTABLE temp_Table=Header[i][j];
			while(temp_Table!=NULL)
			{
				if(temp_Table->skillno == delete_Type)
				{
					TileMap[i][j].attr_skill=0;
					//temp_Skill = FindSkill(g_MapSkillTable, mx, my);
					temp=temp_Table->next;
					DeleteSkill( &Header[i][j], temp_Table);
					temp_Table = temp;
				}
				else temp_Table = temp_Table->next;

			}
		}
	}
}
