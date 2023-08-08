#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sprite.h"
#include "stdio.h"
#include "String.h"
#include "convert565to555.h"
#include "Hong_Sub.h"
#include "tool.h"
#include "MenuSet.h"
#include "itemtable.h"
#include "skill.h"
#include "Effect.h"
#include "word.h"
#include <MATH.H>
#include "char.h"

extern void MouseClickPosition(int*,int*);


CPcWord pc_word[MAX_PCWORD];			// PC가 말한 내용중 중요한 단어
CPcWord npc_word[MAX_PCWORD];			// NPC가 대답할 리스트
CPcWord random_npc_word[MAX_PCWORD];	// NPC가 Random으로 대답할 리스트
char white_str[20][6]; 

char cmd[5][20];//lsw
static int start = 0;//lsw
static int start2 = 0;//lsw
static int start3 = 0;//lsw

char sell_text[10][10];		//lsw
char buy_text[10][10];		//lsw
char repair_text[10][10];	//lsw

//공사중: 아직 구현 돼지 않은 함수
//임시 사용중: Test를 위해 임시적으로 사용중인 variables... and etc. 

////////////////////////////////////////////////////////////////////////////////
//		Function isThatMyItem
//		Input: name of item
//		Output: return true or false depense on owning the inputed item or not.
////////////////////////////////////////////////////////////////////////////////

bool isThatMyItem( char *item_name )
{
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				int item_no = InvItemAttr[a][b][c].item_no;
				if( item_no )
				{
					char *my_item_name = GetItemName( item_no );
					if( !strcmp( item_name, my_item_name ) ) return true;
				}
			}

	return false;
}

////////////////////////////////////////////////////////////////////////////
//		Function amountItem
//		Input: name of item
//		Output: number of items user has.		
////////////////////////////////////////////////////////////////////////////
int amountItem( char *item_name )
{
	int count = 0;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				int item_no = InvItemAttr[a][b][c].item_no;
				if( item_no )
				{
					char *my_item_name = GetItemName( item_no );
					if( !strcmp( item_name, my_item_name ) ) count++;
				}
			}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////
// Function itemNameToNumber
// Input: The name of item.
// Output: The number of item.
////////////////////////////////////////////////////////////////////////////////////
int itemNameToNumber( char *item_name )
{
/*
#define ITEM_Plant_MAX		43
#define ITEM_Mineral_MAX	17
#define ITEM_Herb_MAX		63
#define ITEM_Cook_MAX		57	
#define ITEM_Potion_MAX		64	
#define ITEM_Tool_MAX		49
#define ITEM_Weapon_MAX		118
#define ITEM_Disposable_MAX	53	
#define ITEM_Armor_MAX		122
#define ITEM_Accessory_MAX	177	
#define ITEM_Etc_MAX		184		
*/
	int item_max[] = {	ITEM_Plant_MAX, ITEM_Mineral_MAX	,
						ITEM_Herb_MAX, ITEM_Cook_MAX		,
						ITEM_Potion_MAX	, ITEM_Tool_MAX		,
						ITEM_Weapon_MAX	, ITEM_Disposable_MAX	,
						ITEM_Armor_MAX	,ITEM_Accessory_MAX	,
						ITEM_Etc_MAX		, 
	};

	for( int i=0; i<11; i++ )
	{
		for( int j=0; j<item_max[i]; j++ )
		{
			int item_no = i*1000+j;
			char *source_item_name = GetItemName( item_no );
			if( !strcmp( item_name, source_item_name ) ) return item_no;
		}
	}
	return 0;
}


//ReqItemValue( item_no );
//////////////////////////////////////////////////////////////////////
//		Function itemPrice		
//		return : Price of one. 
//		input : Name of Item.
//////////////////////////////////////////////////////////////////////
DWORD itemPriceWhenSell( char *item_name)
{
	DWORD price = 0;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				int item_no = InvItemAttr[a][b][c].item_no;
				if( item_no )
				{
					char *my_item_name = GetItemName( item_no );
					if( !strcmp( item_name, my_item_name ) ) 
					{
						return ( GetItemValueSell( InvItemAttr[a][b][c] ));
					}
				}
			}
			return 0; // The user doesn't have this item.
}

DWORD itemPriceWhenBuy( char *item_name)
{
	DWORD price = 0;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				int item_no = InvItemAttr[a][b][c].item_no;
				if( item_no )
				{
					char *my_item_name = GetItemName( item_no );
					if( !strcmp( item_name, my_item_name ) ) 
					{
						return ( (DWORD)GetItemValueBuy( InvItemAttr[a][b][c].item_no ));
					}
				}
			}
			return 0; // The user doesn't have this item.
}



//	price = GetItemAttr(t.item_no, VALUE);
//	price = (int)( (float)price/d_max*(d_max - d_curr)*DUR_RATIO +.5 );



///////////////////////////////////////////////////////////////////////////
//	Erase usless words from input.
//  return the inputed sentence without usless words. 
///////////////////////////////////////////////////////////////////////////
char* getRidOfStuff( char *text )
{
	if( !start2 )
	{
		for(int ii=0;ii<20; ii++)
		{
		
			strcpy(white_str[ii],lan->OutputMessage(7,ii+211));//lsw
		}
		start2 = 1;
	}

	for( int i=0; i<20; i++ )
	{
		if( white_str[i] )
		{
			char *ptr;
			while( ptr = strstr( text, white_str[i] ) ) 
			{
				int len = strlen( white_str[i] );
				for( int j=0; j<len; j++ ) 
					*( ptr+j ) = ' ';
			}
		}
		else break;
	}						/// 불필요한 단어를 없애는 작업
	return text;
}


////////////////////////////////////////////////////////////////////////////////
//	This function finds out what kind of operation does it need.
//	And return integers depense on the result.
////////////////////////////////////////////////////////////////////////////////
int whichCommand( char *text )
{
	if( !start3 )
	{
		int ii;
		for(ii=0;ii<10; ii++)
		{
			
		strcpy(sell_text[ii],lan->OutputMessage(7,ii+181));//lsw
		}
		
		for(ii=0;ii<10; ii++)
		{
			
		strcpy(buy_text[ii],lan->OutputMessage(7,ii+191));//lsw
		}

		for(ii=0;ii<10; ii++)
		{
			
		strcpy(repair_text[ii],lan->OutputMessage(7,ii+201));//lsw
		}
		start3 = 1;
	}

	int i;
	for( i=0; i<20; i++ )
	{
		if( (sell_text[i]) ) 
		{
			char *ptr = strstr( text, sell_text[i] );
			if( ptr ) return 1; // selling command return 1.
		}
	}

	for( i=0; i<20; i++ )
	{
		if( (buy_text[i]) )
		{
			char *ptr = strstr( text, buy_text[i] );
			if( ptr ) return 2; // buying command return 2.
		}
	}

	for( i=0; i<20; i++ )
	{
		if( (repair_text[i]) )
		{
			char *ptr = strstr( text, repair_text[i] );
			if( ptr ) return 3; // repair command return 3.
		}
	}
	return 0; // invalid command return 0.
}// End of function, whichCommand.

////////////////////////////////////////////////////////////////////////////
//	공사중
//  Function makeDeal: 흥정 system
//  void makeDeal(parameters) // inputed line from chat. 
//  No return value.
////////////////////////////////////////////////////////////////////////////

int makeDeal( LPSTR lpStr) //메뉴에서 사용할 언어 시스템
{	
	char* text = getRidOfStuff(lpStr); //get a text line without crap.
	int operation = whichCommand(text); //which command do we need to excute.
	char *word;
	word = strtok( text, " \n");
	char *name = NULL;
	int amount = 0;
	int costs = 0;
	float dum = (0.8f + ((rand()%5)/10.0f)) ;
		

	////////////////// Read in the name of item ///////////////////////
		switch( operation )
		{
			case 1 :	// command sell.
				{
					if( isThatMyItem( word ) )
					{
						char *temp_word;
						temp_word = strtok( NULL, " \n");
						amount = atoi( temp_word );
						if( amount <= 0 ) return -200;// You don't have anything with you. 

						temp_word = strtok( NULL, " \n");
						costs = atoi( temp_word);

						if( costs <= 0 ) return -300;// Are you out of your mind?
													 // You don't want money for that item?
						if(costs <= (amount * (itemPriceWhenSell(word))*dum))
							return 0; // The deal is made.
						else
							return -100; // The deal wasn't good. Nah... I don't want to buy.
					}
					else 
						return -400; // user doesn't have this item. You don't have this item.					
				}
			case 2 :	// command buy.
				{
					//Need function that check inventory of NPC
					// 
					//	서버로 상점 리스트 부탁~
					//	receive 하는 곳에서 npc의 대화를 처리한다.
					//	npc의 event_no가 가고 아이템이 간다.
					//  받는 것은 가격을 받아온다. ( true :상점에 그 아이템이 존재할경우 )
				//	else 
					return -500; // NPC doesn't have this item.
								 // Sorry NPC doesn't have this item.
				}

			case 3 :	// command repair.
				{
					/*if( isThatMyItem( word ) )
					{
						(char*)costs = strtok( text, "\n");

						if(cost <= /*function which returns the price of repair/ * dum)
						{
							return 0; // good deal!
							break;
						}
						else
						{
							return -100; // The deal was not good. You have to pay more...
							break;
						}
					}
					else return -400; //Hey you don't have this item. */ 
					return -400;
				}

			default : return -600; // this command is not availiable.
								// what do yo want?
		}
}

int max_pc_word;			// 현재 PC가 대답할수있는 문장의 수
int max_npc_word;			// 현재 NPC가 대답할수있는 문장의 수
int max_random_npc_word;		// 현재 NPC가 random으로 대답할수 있는 문장수

//////////////////////////////////////////////////////////////////////////////////////////////
//	공사중
//	Function languageSystem(LPSTR lpStr)
//		Player가 Non-Event NPC에게 말을 걸었을때, NPC 응답. 
//////////////////////////////////////////////////////////////////////////////////////////////
void languageSystem( LPSTR lpStr )
{
	int getDistance = 0; // distance from the nearest NPC.
	LPCHARACTER ch = whoIsClose(&getDistance); // nearest NPC.
	// when the player is in certain distance from the NPC. 
	//Event begins.
	//************************************************************************************//
	if( !ch || getDistance > CLOSE_ENOUGH){return;}
	
	if (ch->nRace != HUMAN)	
	{	//< CSD-030419
		return;
	}	//> CSD-030419
	
	if( !start )
	{
		for(int ii=0;ii<5; ii++)
		{
			sprintf(cmd[ii],"%s",lan->OutputMessage(4,ii+171));//010217 lsw
		}
		start = 1;
	}

	int i=0;
	int ret = 0;
	while( *cmd[i] )
	{
		if( strstr( lpStr, cmd[i] ) )
		{
			ret = 1; break;
		}				
		i++;
	}

	if( ret )
	{
		SendEvent( EVENT_TYPE_NPC, ch->id );
	}
	else
	{	
		return;		// 잠시 빼놓음..
	}
}//end of function languageSystem();

////////////////////////////////////////////////////////////////////////////////////////////////
//	Function CheckNpcTalk()
//	Player가 NPC근처에 가까이 왔을때 NPC가 Player에게 말을건다. 
////////////////////////////////////////////////////////////////////////////////////////////////
void CheckNpcTalk( )
{
	return;
	int realNPC = 0;
	char* fileRandomNPCTalk = "./data/randomTalk.dat";
	int currentPosition;
	LPCHARACTER target = whoIsClose(&currentPosition); //returns closest NPC and the distance.
	
	if (target)
	{
		if (target->nRace != HUMAN)	
		{	//< CSD-030419
			int answerRate = (rand()%((int)(REPLY_RATE))); // Currently 5% // 0905 kkh 수정 
			
			if (!max_random_npc_word)
			{
				replyTable(fileRandomNPCTalk, random_npc_word, max_random_npc_word);
			}

			int whichLine = (rand()%max_random_npc_word);
			char* getReply = random_npc_word[whichLine].ReturnWords();
			
			if (getReply && answerRate ==0 && currentPosition < CLOSE_ENOUGH) 
			{
				strcpy( target->szChatMessage, getReply );
				target->ChatDelay = 70; 
			}	
		}	//> CSD-030419
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
//	공사끝... Ha Ha ha ^o^
//	Function replyTable()
//		This function Initialize the data into the linked list. 
//		
//////////////////////////////////////////////////////////////////////////////////////////////
void replyTable(char* getFile, CPcWord getWord[], int &max)
{	
	int whichOperation = 0;
	char *getNPCWord;
	char buf[512];
	char *token;
	char delemeter[] = "\t\n";

	FILE *fp;
	
	fp = Fopen(getFile, "rt");
	if(fp)
	{
			max = 0;
//			for(int i = 0; (i < NUM_NPCWORD && !feof(fp)); i++)
			while(fgets( buf, 511, fp ) != NULL )
			{
//				fgets( buf, 511, fp );
				//getWord[i].AddNode( getNPCWord, getOperation );
				token = strtok( buf, delemeter);
				whichOperation = atoi(token);
				token = strtok( NULL, delemeter );
				//token = strtok( buf, "\n");
				getNPCWord = token;
				getWord[max].AddNode(getNPCWord, whichOperation);
				max++;
			}
			fclose(fp);
	}
	
	
}//End of fn() replyTable
//////////////////////////////////////////////////////////////////////////////////////////////
//	Function *ReplyNpc(char)
//		I need data set, pc_word[100] and npc_word[100].
//		They are 100 nodes which include a character.
//		When player asked something NPC answers them...
//		I don't think this will work. Why? if the pc_word = "whaz"		a.com(
//////////////////////////////////////////////////////////////////////////////////////////////
char *ReplyNpc( char *str, CPcWord pc[], CPcWord npc[] )
{
	int random_cmd[100] = {0,};

	for( int i=0; i<max_pc_word; i++ )
	{
		int cmd = pc[i].CompareWord( str );
		if( cmd != -1 )
		{
			int count = 0;
			for( int j=0; j<max_npc_word; j++ )
			{
				if( npc[j].ReplyWords( cmd ) ) 
				{
					random_cmd[count++] = j;
				}
			}
			if( count )
			{
				count = rand()%count;
				return npc[random_cmd[count]].ReturnWords();
			}
		}
	}

	return NULL;
}



///////////////////////////////////////////////////////////////////////////////////////
//Class CPcWord.
///////////////////////////////////////////////////////////////////////////////////////
 
CPcWord::CPcWord()
{
	word = NULL;
	cmd = -1;
}

CPcWord::~CPcWord()
{
	DeleteNode();
}

void CPcWord::AddNode( char *str, int num ) // 추가할 단어와 명령 형식
{
	if( word ) delete word;
	int len = strlen( str );
	word = new char[len+1];
	strcpy( word, str );

	cmd = num;
}

void CPcWord::DeleteNode( )
{
	if( word ) delete word;
	cmd = -1;
}
	
int CPcWord::CompareWord( const char *target_str )
{
	if( strstr( target_str, word ) ) return cmd;
	else return -1;
}

char* CPcWord::ReplyWords( int num )
{
	if( num == cmd ) return word;
	else return NULL;
}

char* CPcWord::ReturnWords( )
{ 
	return word;
}





//////////////////////////////////////////////////////////////////////////////////////////////
//	Function whoIsclose(int*, int*)
//		가장가까운 거리에있는 NPC와의 거리를 구하고, 그 NPC를 return함.  
//
//////////////////////////////////////////////////////////////////////////////////////////////

LPCHARACTER whoIsClose(int* getDistance)
{	
	LPCHARACTER	ch = Hero->lpNext;
	//////This is for exchanging the value of dot position to tile///////
	POINT	hero_Position;

	int mx=0, my=0;
	MouseClickPosition( &mx, &my );
	hero_Position.x = mx/TILE_SIZE;
	hero_Position.y = my/TILE_SIZE;
	
	int distance=0;
	int currentPosition = 2000000;
	// change the above line to the sum of position where player clicked.

	LPCHARACTER target= NULL;

	// This while-roof finds the closest NPC and the distance between NPC and Player.
	while( ch )
	{
		/**********  임시 사용중 SPRITETYPE_NPC  **********************************/
//		if( ch->type == SPRITETYPE_NPC ) // make sure ch is NPC.
		if( ch->type == SPRITETYPE_MONSTER ) // make sure ch is NPC.
		/****************************************************************************/
		{
			POINT	npc_Position;
			npc_Position.x = ch->x/TILE_SIZE;
			npc_Position.y = ch->y/TILE_SIZE;
			distance = abs(hero_Position.x-npc_Position.x)+abs(hero_Position.y-npc_Position.y);//(int)sqrt(pow(hero_Position.x-npc_Position.x,2)+pow(hero_Position.y-npc_Position.y,2));
			
			if( currentPosition > distance ) 
			{
				target = ch;
				currentPosition = distance;
			}
		}
		ch = ch->lpNext;
	}
	*getDistance = currentPosition;
	return target;
}