#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sprite.h"
#include "stdio.h"
#include "convert565to555.h"
#include "Hong_Sub.h"
#include "tool.h"
#include "MenuSet.h"
//#include "MenuDef.h"			// 010904 LTS
#include "itemtable.h"
#include "skill.h"
#include "Effect.h"
#include "NationSys.h"
#include "gameproc.h"
#include "SymbolMgr_Client.h" // soto-1.02

//#define LIGHT_GM_CLIENT //Added by eleval 01/06/09 for light gm client

CSymbolMgr_Client	g_SymBolMgr; // soto-1.02 reece check out furth

t_ItemMutantKind ItemMutantKind[MAX_ITEM_MUTANT_KIND];
t_ItemGrade ItemGrade[MAX_ITEM_GRADE];
t_ItemRare ItemRare[MAX_ITEM_RARE];
RareMain   itemtime[MAX_ITEM_GRADE];
extern bool IsEquipAbleCheckAbleDay(int start, int end);//011215 lsw
extern void	SendReCallItemFromServer( int type );
extern void DeleteItemByItemNumber( int item );
//011025 lsw >
extern DWORD g_skillmaketime;
//011025 lsw <

//extern void RectTextPut(int x, int y, int nWidth, char *sSource);
//extern struct		InterfaceFlag main_interface;

Spr		Icon1[ITEM_MAX][10];
Spr		Icon2[ITEM_MAX][10];
Spr		Icon3[ITEM_MAX][10];
Spr		Icon4[ITEM_MAX][10];
Spr		Icon5[ITEM_MAX][10];
Spr		Icon6[ITEM_MAX][10];
Spr		Icon7[ITEM_MAX][10];
Spr		Icon8[ITEM_MAX][10];
Spr		Icon9[ITEM_MAX][10];
Spr		Icon10[ITEM_MAX][10];
Spr		Icon11[ITEM_MAX][10];
Spr		Icon12[ITEM_MAX][10];
Spr		Icon13[ITEM_MAX][10];


int Num_Of_CItem_Plant		;
int Num_Of_CItem_Mineral	;
int Num_Of_CItem_Herb		;
int Num_Of_CItem_Cook		;
int Num_Of_CItem_Potion		;
int Num_Of_CItem_Tool		;
int Num_Of_CItem_Weapon		;
int Num_Of_CItem_Disposable	;
int Num_Of_CItem_Armor		;
int Num_Of_CItem_Accessory	;
int Num_Of_CItem_Etc		;

struct CItem_List Item_Ref;			// 아이템//reece
//CItem_Plant *ItemPlant ;

//void GetName(char* temp, char* name, int n=0);
bool ReadMagicIconImage( int type, int num );
bool ReadSkillIcon( int num );
int GetItemAttr(int item_id, int attr);

void CalcGuildToSkill( );	// 스킬을 계열 별로 분류

int menu_to_set[] = { 2, 1, 5, 3, 12, 9, 6, 8, 4, 7, 11 };		// 메뉴 번호를 아이템 조인 테이블에 있는 번호와 일치시킨다.

int n_MagicBagic[2][12][15] = 
{ //< CSD-020702
	{
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
	},
	{
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
		{0, },
	}
};//> CSD-020702

/*###########################################################################
			아이템 클레스 멤버 함수 및 아이템 관련 함수 
###########################################################################*/
void GetName(char* temp, char* name, FILE *fp, int n=0)
{
	char *source;
	source = name;

	int i=0;
	int max = ITEM_NAME_MAX;

	if( n ) if( n != ITEM_NAME_MAX+9 ) max = n-9;

	while(*(temp+i))
	{
		if(*(temp+i) == '<') break;
		i++;
	}
	i++;
	while(*(temp+i))
	{
		if(*(temp+i) == '>') 
		{
			*source = '\0';
			fseek(fp, -max-7 + i, SEEK_CUR);
			return;
		}
		*source = *(temp+i);
		source++;
		i++ ;
	}
}


void SaveName( char *token )
{
	FILE *fp = Fopen( "30자 넘는 아이템.txt", "at+" );
	if( fp )
	{
		fprintf( fp, "%s\n", token );
		fclose( fp );
	}	
}

int ReadItemTableFunction( char *text, int &Item_id, char *Name, char *Han_Name, int *item_value, int max )
{
	char *token;

	token = strtok( text, "\t\n");
	Item_id = atoi( token );

	token = strtok( NULL, "\t\n");
	EatRearWhiteChar( token );
	strcpy(Name, token); 

	token = strtok( NULL, "\t\n");
	EatRearWhiteChar( token );
	strcpy(Han_Name, token); 

	
	for(int i=0;i<max;i++)
	{
		token = strtok( NULL, "\t\n");
		*item_value = atoi( token );
		item_value++;
	}
	return 1;
}

int CItem_Plant::ReadItemTable( char *temp )
{
	int max = ITEM_Plant_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Mineral::ReadItemTable( char *temp )
{
	int max =ITEM_Mineral_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Herb::ReadItemTable( char *temp )
{
	int max =ITEM_Herb_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Cook::ReadItemTable( char *temp )
{
	int max =ITEM_Cook_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Potion::ReadItemTable( char *temp )
{
	int max =ITEM_Potion_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Tool::ReadItemTable( char *temp )
{
	int max =ITEM_Tool_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Weapon::ReadItemTable( char *temp )
{
	int max =ITEM_Weapon_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Disposable::ReadItemTable( char *temp )
{
	int max =ITEM_Disposable_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Armor::ReadItemTable( char *temp )
{
	int max =ITEM_Armor_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Accessory::ReadItemTable( char *temp )
{
	int max =ITEM_Accessory_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}

int CItem_Etc::ReadItemTable( char *temp )
{
	int max =ITEM_Etc_MAX-3;
	return ReadItemTableFunction( temp, Item_id, Name, Han_Name, &lv, max );
}



CItem_Plant			*Plant = NULL;
CItem_Mineral		*Mineral= NULL;
CItem_Herb			*Herb= NULL;
CItem_Cook			*Cook= NULL;
CItem_Potion		*Potion= NULL;
CItem_Tool			*Tool= NULL;
CItem_Weapon		*Weapon= NULL;
CItem_Disposable	*Disposable= NULL;
CItem_Armor			*Armor= NULL;
CItem_Accessory		*Accessory= NULL;
CItem_Etc			*Etc= NULL;

//############################ 아이템 로드 ##############################
extern int YesOrNo( char *s, char *title );
extern BOOL  ShowErrorMessage( char* lpszMessage );


void FreeItemTable()
{
	SAFE_DELETE_ARRAY(Plant);
	SAFE_DELETE_ARRAY(Mineral);
	SAFE_DELETE_ARRAY(Herb);
	SAFE_DELETE_ARRAY(Cook);
	SAFE_DELETE_ARRAY(Potion);
	SAFE_DELETE_ARRAY(Tool);
	SAFE_DELETE_ARRAY(Weapon);
	SAFE_DELETE_ARRAY(Disposable);
	SAFE_DELETE_ARRAY(Armor);
	SAFE_DELETE_ARRAY(Accessory);
	SAFE_DELETE_ARRAY(Etc);
}


/////////////////////////////////////////////
// 아이템 테이블(bin) 읽어오는 특별한 경우
int GetLineItemByItemTableBin( char *filename )
{
	int line = 0;
	FILE *fp;

	if(fp=Fopen( filename, "rb" ) )
	{
		fread( &line,sizeof(int ), 1,fp);		
		fclose( fp );
	}

	return line;
}


void ItemRead()
{
	char *line_count_file_name_txt;
	char *line_count_file_name_bin;
	
	line_count_file_name_txt = "./itemtable/itemtable0.txt";
	line_count_file_name_bin = "./itemtable/itemtable0.bin";

#ifdef _DEBUG
		Num_Of_CItem_Plant		= GetTextLine( line_count_file_name_txt );
		if( !Num_Of_CItem_Plant)
#endif
			Num_Of_CItem_Plant = GetLineItemByItemTableBin( line_count_file_name_bin );

	line_count_file_name_txt = "./itemtable/itemtable1.txt";
	line_count_file_name_bin = "./itemtable/itemtable1.bin";
#ifdef _DEBUG
	Num_Of_CItem_Mineral	 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Mineral )
#endif
		Num_Of_CItem_Mineral = GetLineItemByItemTableBin( line_count_file_name_bin );

	line_count_file_name_txt = "./itemtable/itemtable2.txt";
	line_count_file_name_bin = "./itemtable/itemtable2.bin";
#ifdef _DEBUG
	Num_Of_CItem_Herb		 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Herb)
#endif
		Num_Of_CItem_Herb = GetLineItemByItemTableBin( line_count_file_name_bin );

	line_count_file_name_txt = "./itemtable/itemtable3.txt";
	line_count_file_name_bin = "./itemtable/itemtable3.bin";
#ifdef _DEBUG
	Num_Of_CItem_Cook		 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Cook )
#endif
		Num_Of_CItem_Cook = GetLineItemByItemTableBin( line_count_file_name_bin );


	line_count_file_name_txt = "./itemtable/itemtable4.txt";
	line_count_file_name_bin = "./itemtable/itemtable4.bin";
#ifdef _DEBUG
	Num_Of_CItem_Potion		 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Potion )
#endif
		Num_Of_CItem_Potion = GetLineItemByItemTableBin( line_count_file_name_bin );


	line_count_file_name_txt = "./itemtable/itemtable5.txt";
	line_count_file_name_bin = "./itemtable/itemtable5.bin";
#ifdef _DEBUG
	Num_Of_CItem_Tool		 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Tool )
#endif
		Num_Of_CItem_Tool = GetLineItemByItemTableBin( line_count_file_name_bin );

	line_count_file_name_txt = "./itemtable/itemtable6.txt";
	line_count_file_name_bin = "./itemtable/itemtable6.bin";
#ifdef _DEBUG
	Num_Of_CItem_Weapon		 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Weapon )
#endif
		Num_Of_CItem_Weapon = GetLineItemByItemTableBin( line_count_file_name_bin );

	line_count_file_name_txt = "./itemtable/itemtable7.txt";
	line_count_file_name_bin = "./itemtable/itemtable7.bin";
#ifdef _DEBUG
	Num_Of_CItem_Disposable	 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Disposable )
#endif	
		Num_Of_CItem_Disposable = GetLineItemByItemTableBin( line_count_file_name_bin );

	line_count_file_name_txt = "./itemtable/itemtable8.txt";
	line_count_file_name_bin = "./itemtable/itemtable8.bin";
#ifdef _DEBUG
	Num_Of_CItem_Armor		 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Armor )
#endif
		Num_Of_CItem_Armor = GetLineItemByItemTableBin( line_count_file_name_bin );

	line_count_file_name_txt = "./itemtable/itemtable9.txt";
	line_count_file_name_bin = "./itemtable/itemtable9.bin";
#ifdef _DEBUG
	Num_Of_CItem_Accessory	 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Accessory )
#endif
		Num_Of_CItem_Accessory = GetLineItemByItemTableBin( line_count_file_name_bin );

	line_count_file_name_txt = "./itemtable/itemtable10.txt";
	line_count_file_name_bin = "./itemtable/itemtable10.bin";
#ifdef _DEBUG
	Num_Of_CItem_Etc		 = GetTextLine( line_count_file_name_txt );
	if( !Num_Of_CItem_Etc )
#endif
		Num_Of_CItem_Etc = GetLineItemByItemTableBin( line_count_file_name_bin );


	if( !Num_Of_CItem_Plant || !Num_Of_CItem_Mineral || !Num_Of_CItem_Herb || !Num_Of_CItem_Cook 
		|| !Num_Of_CItem_Potion || !Num_Of_CItem_Tool || !Num_Of_CItem_Weapon || !Num_Of_CItem_Disposable	 
		|| !Num_Of_CItem_Armor || !Num_Of_CItem_Accessory || !Num_Of_CItem_Etc ) 
	{
		JustMsg( lan->OutputMessage(6,20) );//010215 lsw
		return;
	}


	Plant = new CItem_Plant[Num_Of_CItem_Plant];
	Dbg_Assert(Plant != NULL);
	Mineral	= new CItem_Mineral[Num_Of_CItem_Mineral];
	Dbg_Assert(Mineral != NULL);
	Herb = new CItem_Herb[Num_Of_CItem_Herb];
	Dbg_Assert(Herb != NULL);
	Cook = new CItem_Cook[Num_Of_CItem_Cook];
	Dbg_Assert(Cook != NULL);
	Potion = new CItem_Potion[Num_Of_CItem_Potion];
	Dbg_Assert(Potion != NULL);
	Tool = new CItem_Tool[Num_Of_CItem_Tool];
	Dbg_Assert(Tool != NULL);
	Weapon = new CItem_Weapon[Num_Of_CItem_Weapon];
	Dbg_Assert(Weapon != NULL);
	Disposable = new CItem_Disposable[Num_Of_CItem_Disposable];
	Dbg_Assert(Disposable != NULL);
	Armor = new CItem_Armor[Num_Of_CItem_Armor];
	Dbg_Assert(Armor != NULL);
	Accessory = new CItem_Accessory[Num_Of_CItem_Accessory];
	Dbg_Assert(Accessory != NULL);
	Etc = new CItem_Etc[Num_Of_CItem_Etc];
	Dbg_Assert(Etc != NULL);

	CItem_Plant* plant = new CItem_Plant[Num_Of_CItem_Plant];
	Dbg_Assert(plant != NULL);
	CItem_Mineral* mineral = new CItem_Mineral[	Num_Of_CItem_Mineral];
	Dbg_Assert(mineral != NULL);
	CItem_Herb* herb = new CItem_Herb[Num_Of_CItem_Herb];
	Dbg_Assert(herb != NULL);
	CItem_Cook* cook = new CItem_Cook[Num_Of_CItem_Cook];
	Dbg_Assert(cook != NULL);
	CItem_Potion* potion = new CItem_Potion[Num_Of_CItem_Potion];
	Dbg_Assert(potion != NULL);
	CItem_Tool* tool = new CItem_Tool[Num_Of_CItem_Tool];
	Dbg_Assert(tool != NULL);
	CItem_Weapon* weapon = new CItem_Weapon[Num_Of_CItem_Weapon];
	Dbg_Assert(weapon != NULL);
	CItem_Disposable* disposable = new CItem_Disposable[Num_Of_CItem_Disposable];
	Dbg_Assert(disposable != NULL);
	CItem_Armor* armor = new CItem_Armor[Num_Of_CItem_Armor];
	Dbg_Assert(armor != NULL);
	CItem_Accessory* accessory	= new CItem_Accessory[Num_Of_CItem_Accessory];
	Dbg_Assert(accessory != NULL);
	CItem_Etc* etc = new CItem_Etc[Num_Of_CItem_Etc];
	Dbg_Assert(etc != NULL);

	// set the number of items
	Item_Ref.nItem[0] = Num_Of_CItem_Plant;
	Item_Ref.nItem[1] = Num_Of_CItem_Mineral;
	Item_Ref.nItem[2] = Num_Of_CItem_Herb;
	Item_Ref.nItem[3] = Num_Of_CItem_Cook;
	Item_Ref.nItem[4] = Num_Of_CItem_Potion;
	Item_Ref.nItem[5] = Num_Of_CItem_Tool;
	Item_Ref.nItem[6] = Num_Of_CItem_Weapon;
	Item_Ref.nItem[7] = Num_Of_CItem_Disposable;
	Item_Ref.nItem[8] = Num_Of_CItem_Armor;
	Item_Ref.nItem[9] = Num_Of_CItem_Accessory;
	Item_Ref.nItem[10] = Num_Of_CItem_Etc;

	int z = 0;

	for (z = 0; z < Num_Of_CItem_Plant; ++z)
	{
		Item_Ref.Item_type[0][z] = &plant[z];
	}

	for (z = 0; z < Num_Of_CItem_Mineral; ++z)
	{
		Item_Ref.Item_type[1][z] = &mineral[z];
	}

	for (z = 0; z < Num_Of_CItem_Herb; ++z)
	{
		Item_Ref.Item_type[2][z] = &herb[z];
	}

	for (z = 0; z < Num_Of_CItem_Cook; ++z)
	{
		Item_Ref.Item_type[3][z] = &cook[z];
	}
	
	for (z = 0; z < Num_Of_CItem_Potion; ++z)
	{
		Item_Ref.Item_type[4][z] = &potion[z];
	}

	for (z = 0; z < Num_Of_CItem_Tool; ++z)
	{
		Item_Ref.Item_type[5][z] = &tool[z];
	}

	for (z = 0; z < Num_Of_CItem_Weapon; ++z)
	{
		Item_Ref.Item_type[6][z] = &weapon[z];
	}

	for (z = 0; z < Num_Of_CItem_Disposable; ++z)
	{
		Item_Ref.Item_type[7][z] = &disposable[z];
	}

	for (z = 0; z < Num_Of_CItem_Armor; ++z)
	{
		Item_Ref.Item_type[8][z] = &armor[z];
	}

	for (z = 0; z < Num_Of_CItem_Accessory; ++z)
	{
		Item_Ref.Item_type[9][z] = &accessory[z];
	}

	for (z = 0; z < Num_Of_CItem_Etc; ++z)
	{
		Item_Ref.Item_type[10][z] = &etc[z];
	}

	const int	ItemAttrMax[11] = { 
		ITEM_Plant_MAX, ITEM_Mineral_MAX, ITEM_Herb_MAX, ITEM_Cook_MAX, ITEM_Potion_MAX, ITEM_Tool_MAX,
		ITEM_Weapon_MAX, ITEM_Disposable_MAX, ITEM_Armor_MAX, ITEM_Accessory_MAX, ITEM_Etc_MAX,
	};

	for( int i=0 ; i < Num_Of_Item_Type ; i++)
	{
		FILE *fp = NULL;
		char filename[MAX_FILENAME],filenameBin[MAX_FILENAME];		
		sprintf(filename, "./itemtable/itemtable%d.txt", i);
		sprintf(filenameBin, "./itemtable/itemtable%d.bin", i);
#ifdef _DEBUG
#ifndef LIGHT_GM_CLIENT
		if( ( fp=Fopen(filename, "rt")) && (Item_Ref.nItem[i] > 0)  )
		{
			char text[MAX_STRING_];
			char temp[MAX_STRING_];
			while( fgets( text, MAX_STRING_, fp ) != NULL )
			{
				char *token;
				strcpy( temp, text );
				token = strtok( text, "\t\n");
				int item_id = atoi( token );

				CItem *t = ItemUnit( i, item_id );
				if( t ) t->ReadItemTable( temp );
			}
			fclose(fp);

			if( fp=Fopen(filenameBin, "wb") )
			{
				CHSEL_STREAM ItemEn;
				HSEL_INITIAL eninit;
				eninit.iEncryptType	=	HSEL_ENCRYPTTYPE_RAND;
				eninit.iDesCount	=	HSEL_DES_TRIPLE;
				eninit.iDesCount	=	HSEL_DES_SINGLE;				
				eninit.iCustomize	=	HSEL_KEY_TYPE_DEFAULT;
				eninit.iSwapFlag	=	HSEL_SWAP_FLAG_ON;
				if (!ItemEn.Initial(eninit))
				{
					JustMsg("ItemTable make bin Error");
					fclose(fp);
				}
				HSEL_INITIAL deinit =  ItemEn.GetHSELCustomizeOption();

				int* piNumOfItemTable	= NULL;
				int iTempNumOfItemTable = 0;
				char* pTable = NULL;
				char* pTableStart = NULL;
				int iTableSize = 0;
				switch(i)
				{
				case PLANT:
					{	
						pTable = (char*)(&plant[0]);
						pTableStart = (char*)(&plant[0]);
						iTableSize = sizeof(CItem_Plant);						
						piNumOfItemTable = &Num_Of_CItem_Plant;
						iTempNumOfItemTable = Num_Of_CItem_Plant;
					}break;
				case MINERAL:
					{	
						pTable = (char*)(&mineral[0]);						
						pTableStart = (char*)(&mineral[0]);						
						iTableSize = sizeof(CItem_Mineral);						
						piNumOfItemTable = &Num_Of_CItem_Mineral;
						iTempNumOfItemTable = Num_Of_CItem_Mineral;						
					}break;
				case HERB:
					{	
						pTable = (char*)(&herb[0]);
						pTableStart = (char*)(&herb[0]);
						iTableSize = sizeof(CItem_Herb);						
						piNumOfItemTable = &Num_Of_CItem_Herb;
						iTempNumOfItemTable = Num_Of_CItem_Herb;						
					}break;
				case COOK:
					{	
						pTable = (char*)(&cook[0]);	
						pTableStart = (char*)(&cook[0]);						
						iTableSize = sizeof(CItem_Cook);						
						piNumOfItemTable = &Num_Of_CItem_Cook;
						iTempNumOfItemTable = Num_Of_CItem_Cook;
					}break;					
				case POTION:
					{	
						pTable = (char*)(&potion[0]);						
						pTableStart = (char*)(&potion[0]);
						iTableSize = sizeof(CItem_Potion);						
						piNumOfItemTable = &Num_Of_CItem_Potion;
						iTempNumOfItemTable = Num_Of_CItem_Potion;						
					}break;					
				case TOOL:
					{	
						pTable = (char*)(&tool[0]);
						pTableStart = (char*)(&tool[0]);
						iTableSize = sizeof(CItem_Tool);						
						piNumOfItemTable = &Num_Of_CItem_Tool;
						iTempNumOfItemTable = Num_Of_CItem_Tool;						
					}break;					
				case WEAPON:
					{	
						pTable = (char*)(&weapon[0]);
						pTableStart = (char*)(&weapon[0]);
						iTableSize = sizeof(CItem_Weapon);						
						piNumOfItemTable = &Num_Of_CItem_Weapon;
						iTempNumOfItemTable = Num_Of_CItem_Weapon;						
					}break;					
				case DISPOSABLE:
					{	
						pTable = (char*)(&disposable[0]);						
						pTableStart = (char*)(&disposable[0]);
						iTableSize = sizeof(CItem_Disposable);						
						piNumOfItemTable = &Num_Of_CItem_Disposable;
						iTempNumOfItemTable = Num_Of_CItem_Disposable;						
					}break;					
				case ARMOR:
					{	
						pTable = (char*)(&armor[0]);						
						pTableStart = (char*)(&armor[0]);
						iTableSize = sizeof(CItem_Armor);						
						piNumOfItemTable = &Num_Of_CItem_Armor;
						iTempNumOfItemTable = Num_Of_CItem_Armor;						
					}break;					
				case ACCESSORY:
					{	
						pTable = (char*)(&accessory[0]);						
						pTableStart = (char*)(&accessory[0]);
						iTableSize = sizeof(CItem_Accessory);						
						piNumOfItemTable = &Num_Of_CItem_Accessory;
						iTempNumOfItemTable = Num_Of_CItem_Accessory;						
					}break;					
				case ETC:
					{	
						pTable = (char*)(&etc[0]);						
						pTableStart = (char*)(&etc[0]);						
						iTableSize = sizeof(CItem_Etc);						
						piNumOfItemTable = &Num_Of_CItem_Etc;
						iTempNumOfItemTable = Num_Of_CItem_Etc;						
					}break;
				default:
					{
						JustMsg("ItemTable Read Error txt %d",i);
						fclose(fp);
					}break;
				}
		
			//암호 하기
			fwrite( &iTempNumOfItemTable, 4, 1, fp );//갯수를 그냥 기록

			fwrite( &deinit, sizeof(HSEL_INITIAL), 1, fp );

			ItemEn.Encrypt((char *)piNumOfItemTable, sizeof(int));
			fwrite( piNumOfItemTable, 4, 1, fp );
			
			int iCRC = ItemEn.GetCRCConvertInt();//CRC 기록
			fwrite(&iCRC,sizeof(int),1,fp);
			for(int ct = 0 ; iTempNumOfItemTable > ct ;ct++)
			{
				ItemEn.Encrypt(pTable, iTableSize);
				fwrite(pTable,iTableSize,1,fp);//암호화된 데이타 기록

				const int iCRC = ItemEn.GetCRCConvertInt();//CRC 기록
				fwrite(&iCRC,sizeof(int),1,fp);
				pTable += iTableSize;
			}
			
			fclose(fp);//파일 기록

			//암호 풀기
			CHSEL_STREAM ItemDe;
			ItemDe.Initial(deinit);
			ItemDe.Decrypt((char *)piNumOfItemTable, sizeof(int));
			for(int ct2 = 0 ; *piNumOfItemTable > ct2 ;ct2++)
			{
				ItemDe.Decrypt((char*)pTableStart, iTableSize);
				pTableStart += iTableSize;
			}
			
			}
		}
		else
#endif
#endif
		{
			if(fp=Fopen( filenameBin, "rb" ) )
			{
				CHSEL_STREAM ItemDe;
				HSEL_INITIAL deinit;
				int iLineNum = 0;
				fread( &iLineNum,sizeof(int), 1,fp);

				fread( &deinit,sizeof(HSEL_INITIAL), 1,fp);
				if (!ItemDe.Initial(deinit))
				{
					JustMsg("ItemTable make bin Error");
					fclose(fp);
				}		
				int *piNumOfItemTable  = NULL;
				int iTableSize = 0;
				char* pTable = NULL;

				switch(i)
				{				
				case PLANT:
					{
						pTable = (char*)&plant[0];
						iTableSize = sizeof(CItem_Plant);
						piNumOfItemTable = &Num_Of_CItem_Plant;
					}break;
				case MINERAL:
					{
						pTable = (char*)&mineral[0];
						iTableSize = sizeof(CItem_Mineral);
						piNumOfItemTable = &Num_Of_CItem_Mineral;
					}break;
				case HERB:
					{
						pTable = (char*)&herb[0];
						iTableSize = sizeof(CItem_Herb);
						piNumOfItemTable = &Num_Of_CItem_Herb;
					}break;
				case COOK:
					{
						pTable = (char*)&cook[0];
						iTableSize = sizeof(CItem_Cook);
						piNumOfItemTable = &Num_Of_CItem_Cook;
					}break;
				case POTION:
					{
						pTable = (char*)&potion[0];
						iTableSize = sizeof(CItem_Potion);
						piNumOfItemTable = &Num_Of_CItem_Potion;
					}break;
				case TOOL:
					{
						pTable = (char*)&tool[0];
						iTableSize = sizeof(CItem_Tool);
						piNumOfItemTable = &Num_Of_CItem_Tool;
					}break;
				case WEAPON:
					{
						pTable = (char*)&weapon[0];
						iTableSize = sizeof(CItem_Weapon);
						piNumOfItemTable = &Num_Of_CItem_Weapon;
					}break;
				case DISPOSABLE:
					{
						pTable = (char*)&disposable[0];
						iTableSize = sizeof(CItem_Disposable);
						piNumOfItemTable = &Num_Of_CItem_Disposable;
					}break;
				case ARMOR:
					{
						pTable = (char*)&armor[0];
						iTableSize = sizeof(CItem_Armor);
						piNumOfItemTable = &Num_Of_CItem_Armor;
					}break;
				case ACCESSORY:
					{
						pTable = (char*)&accessory[0];
						iTableSize = sizeof(CItem_Accessory);
						piNumOfItemTable = &Num_Of_CItem_Accessory;
					}break;
				case ETC:
					{
						pTable = (char*)&etc[0];
						iTableSize = sizeof(CItem_Etc);
						piNumOfItemTable = &Num_Of_CItem_Etc;
					}break;
				default:
					{
					}break;
				}
				{
					fread( piNumOfItemTable,sizeof(int), 1,fp);
					ItemDe.Decrypt((char *)piNumOfItemTable, sizeof(int));
					
					if( iLineNum  != *piNumOfItemTable)
					{
						JustMsg("ItemTable Read Error %d",i);
						fclose(fp);
					}				

					int iCRC1 =0;
					const int iCRC2 = ItemDe.GetCRCConvertInt();//CRC 기록
					fread(&iCRC1,sizeof(int),1,fp);
					if( iCRC2  != iCRC1)
					{
						JustMsg("ItemTable Read Error %d",i);
						fclose(fp);
					}
					for(int ct = 0 ; *piNumOfItemTable > ct ;ct++)
					{
						fread((char*)pTable,iTableSize,1,fp);//암호화된 데이타 기록
						ItemDe.Decrypt((char*)pTable, iTableSize);
						int iCRC1=0;//CRC 기록
						const int iCRC2 = ItemDe.GetCRCConvertInt();//CRC 기록
						fread(&iCRC1,sizeof(int),1,fp);
						if( iCRC2  != iCRC1)
						{
							JustMsg("ItemTable Read Error %d , %d",i,ct);
							fclose(fp);
						}
						pTable += iTableSize;
					}
				}
				fclose(fp);
			}
			else return; 
		}
	}

	for( z=0; z < Num_Of_CItem_Plant ; z++)		{ Plant[z].CopyItemClass( &plant[z], ITEM_Plant_MAX	); Item_Ref.Item_type[0][z] = &Plant[z]; }
	for( z=0; z < Num_Of_CItem_Mineral ; z++)	{ Mineral[z].CopyItemClass( &mineral[z], ITEM_Mineral_MAX ); Item_Ref.Item_type[1][z] = &Mineral[z]; }
	for( z=0; z < Num_Of_CItem_Herb ; z++)		{ Herb[z].CopyItemClass( &herb[z], ITEM_Herb_MAX ); Item_Ref.Item_type[2][z] = &Herb[z]; }
	for( z=0; z < Num_Of_CItem_Cook ; z++)		{ Cook[z].CopyItemClass( &cook[z], ITEM_Cook_MAX ); Item_Ref.Item_type[3][z] = &Cook[z]; }
	for( z=0; z < Num_Of_CItem_Potion ; z++)	{ Potion[z].CopyItemClass( &potion[z], ITEM_Potion_MAX ); Item_Ref.Item_type[4][z] = &Potion[z]; }
	for( z=0; z < Num_Of_CItem_Tool ; z++)		{ Tool[z].CopyItemClass( &tool[z], ITEM_Tool_MAX ); Item_Ref.Item_type[5][z] = &Tool[z]; }
	for( z=0; z < Num_Of_CItem_Weapon ; z++)	{ Weapon[z].CopyItemClass( &weapon[z], ITEM_Weapon_MAX ); Item_Ref.Item_type[6][z] = &Weapon[z]; }
	for( z=0; z < Num_Of_CItem_Disposable ; z++){ Disposable[z].CopyItemClass( &disposable[z], ITEM_Disposable_MAX ); Item_Ref.Item_type[7][z] = &Disposable[z]; }
	for( z=0; z < Num_Of_CItem_Armor ; z++)		{ Armor[z].CopyItemClass( &armor[z], ITEM_Armor_MAX	); Item_Ref.Item_type[8][z] = &Armor[z]; }
	for( z=0; z < Num_Of_CItem_Accessory ; z++)	{ Accessory[z].CopyItemClass( &accessory[z], ITEM_Accessory_MAX ); Item_Ref.Item_type[9][z] = &Accessory[z]; }
	for( z=0; z < Num_Of_CItem_Etc ; z++)		{ Etc[z].CopyItemClass( &etc[z], ITEM_Etc_MAX ); Item_Ref.Item_type[10][z]= &Etc[z]; }

	SAFE_DELETE_ARRAY(plant		);
	SAFE_DELETE_ARRAY(mineral	);
	SAFE_DELETE_ARRAY(herb		);
	SAFE_DELETE_ARRAY(cook		);
	SAFE_DELETE_ARRAY(potion	);
	SAFE_DELETE_ARRAY(tool		);
	SAFE_DELETE_ARRAY(weapon	);
	SAFE_DELETE_ARRAY(disposable);
	SAFE_DELETE_ARRAY(armor		);
	SAFE_DELETE_ARRAY(accessory	);
	SAFE_DELETE_ARRAY(etc		);
}


inline int GetItemNumber( int item_number , int& type, int& num )
{	
	type	= item_number / 1000;
	num		= item_number % 1000;

	if( (type<0) || (type>10) ) return 0;
//	if( (num<0) || (num>= Item_Ref.nItem[type]) ) return 0;

	return 1;
}	



///////////////////////////////////////////////////////////////////////////////////////
void LoadItemIconData(Spr *s, int num)
{
	FILE	*fp;
	char filename[MAX_FILENAME];
	sprintf(filename, "./itemdata/%07d.csp", num );
	if( fp = Fopen(filename, "rb" ) )
	{
		fread( &s->xl, sizeof( short ), 1, fp);
		fread( &s->yl, sizeof( short ), 1, fp);
		fread( &s->ox, sizeof( short ), 1, fp);
		fread( &s->oy, sizeof( short ), 1, fp);
		fread( &s->size, sizeof( unsigned int ), 1, fp);
		dw_TotalImageSize+=s->size;

		MemFree( s->img );		
		MemAlloc( s->img, s->size );
		fread(s->img, s->size, 1, fp);
		convert565to555( s );
		fclose(fp);
	}
	else LoadItemIconData(s, 0);	// 0번그림 로드
}



//------------------------------------------------------------------------
//	아이콘 그림 찍어주는 함수 ( 에니메이션도 가능 )
//------------------------------------------------------------------------
void PutItemIcon(int x, int y, int item_number, int delay, int effect, int is_not_equip)
{
	if( !item_number ) return;
	int image;					// 아이템이 가지고 있는 이미지
	int kind;						// 아이콘 그림 타입
	int index;
	
	GetItemNumber(item_number, kind, index);
	CItem *t = ItemUnit(kind, index);
	if( !t ) 
	{
		DeleteItemByItemNumber( item_number );
		return;
	}

	image = t->GetImageNumber();
	if( !GetItemNumber((int)image/100, kind, index) ) 
	{
#ifdef _DEBUG // finito 060507
		if( GetSysInfo( SI_GAME_MAKE_MODE ) ) 
		{
			JustMsg( lan->OutputMessage(6,81), item_number );//010215 lsw
			exit( 0 );
		} 		
		return;
#endif
	}
	if( !index )		// 아이템 그림이 0이면
	{
		DeleteItemByItemNumber( item_number );
		return;
	}
	Spr* s;

	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	if( index >= ITEM_MAX ) 
	{
		JustMsg( lan->OutputMessage(6,69), item_number  );//010215 lsw
	}
	switch(kind)
	{
		case 0 :	s = &Icon1[index][0]; break;
		case 1 :	s = &Icon2[index][0]; break;
		case 2 :	s = &Icon3[index][0]; break;
		case 3 :	s = &Icon4[index][0]; break;
		case 4 :	s = &Icon5[index][0]; break;
		case 5 :	s = &Icon6[index][0]; break;
		case 6 :	s = &Icon7[index][0]; break;
		case 7 :	s = &Icon8[index][0]; break;
		case 8 :	s = &Icon9[index][0]; break;
		case 9 :	s = &Icon10[index][0]; break;
		case 10 :	s = &Icon11[index][0]; break;
		case 11 :	s = &Icon12[index][0]; break;
		case 12 :	s = &Icon13[index][0]; break;


		default :	JustMsg( "if(!s->img) : Error " );		break;
	}
	if(!s->img) 
	{
		LoadItemIconData(s, image);
	}
	if( delay ) PutCompressedImageFX(x, y, s, delay, effect);
	else
	{
		PutCompressedImage(x, y, s);
		/*if( GetItemAttrWeapon(item_number) && is_not_equip==0 ) PutCompressedImage(x, y, s);
		else 
		{
			PutCompressedImageBlendRGB( x, y, s, RGB( 0,0,200 ) );
			PutCompressedImageFX( x, y, s, 10, 2);
		}*/
	}
}

void PutItemIconAni( int x, int y, int item_number )
{
	int image;					// 아이템이 가지고 있는 이미지
	int kind;						// 아이콘 그림 타입
	int index;

	static int ct=0;
	static int flag=0;
	
	GetItemNumber(item_number, kind, index);

	CItem *t = ItemUnit( kind , index );
	if( !t ) return;
	image = t->GetImageNumber();

	image+=ct;

	GetItemNumber((int)image/100, kind, index);

	Spr* s;

	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	if( index >= ITEM_MAX ) 
	{
		JustMsg( lan->OutputMessage(6,69), item_number  );//010215 lsw
	}
	switch(kind)
	{
		case 0 :	s = &Icon1[index][ct]; break;
		case 1 :	s = &Icon2[index][ct]; break;
		case 2 :	s = &Icon3[index][ct]; break;
		case 3 :	s = &Icon4[index][ct]; break;
		case 4 :	s = &Icon5[index][ct]; break;
		case 5 :	s = &Icon6[index][ct]; break;
		case 6 :	s = &Icon7[index][ct]; break;
		case 7 :	s = &Icon8[index][ct]; break;
		case 8 :	s = &Icon9[index][ct]; break;
		case 9 :	s = &Icon10[index][ct]; break;
		case 10 :	s = &Icon11[index][ct]; break;
		case 11 :	s = &Icon12[index][ct]; break;
		case 12 :	s = &Icon13[index][ct]; break;



		default :	JustMsg( "if(!s->img) : Error " );		break;
	}
	if(!s->img)
	{
		LoadItemIconData(s, image);
	}
	PutCompressedImage(x, y, s);
	flag++;
	if( !(flag%3) ) ct++;
	ct %=4;
}


char explain_tactics[13][20] = {0,/*	"주먹격투"		,
									"한손검"		,	
									"활쏘기"		 ,
									"펜싱검"		,
									"철퇴격투"		,
									"창찌르기"		,
									"봉돌리기"		,
									"비검던지기"		,
									"방패방어"		,
									"양손검"		,
									"양손도끼"		,
									"공격마법운용"	,
									"보호신법운용"	,
									*/
								  };



////////////////////////////////////////////////////////////////////////
char *GetItemName( int item_id )
{
	int type = item_id;
	int num = item_id;
	GetItemNumber( item_id , type, num );

	CItem *t = ItemUnit( type , num );
	if( t ) return t->GetItemHanName();
	else return temp_menu_string;
}

char *GetItemHanName( int item_id )
{
	int type = item_id;
	int num = item_id;
	GetItemNumber( item_id , type, num );

	CItem *t = ItemUnit( type , num );
	if( t ) return t->GetItemHanName();
	else return NULL;
}


int GetItemAttr2(int gender, int item_id, int attr )
{
	int type;
	int num;
	GetItemNumber( item_id , type, num );

	int is_man = gender;

	CItem *t = ItemUnit( type, num );
	if( !t ) return 0;

	switch(attr)
	{
		case WEAR_ABLE	:	return t->GetItemClass();
		case WEIGHT		:	return t->GetItemWeight();
		case VALUE		:	if( !t->curr_value ) ReqItemValue(item_id);
							return t->curr_value;

///		case VALUE_SELL	:	if( !t->curr_value_sell ) ReqItemValue_Sell(item_id);
//							return t->curr_value_sell;

		case VALUE_REPAIR:	return t->curr_value_repair;
		case DURATION	:	return t->GetItemDuration();
		case ITEMSHOW	:	return t->GetItemShow( is_man );
		case EQUIPABLE	:	return t->is_equipable();
		case DO_RBUTTON :	return t->GetRButton();
		case USE_JUNK	: 	return t->GetUseJunk();
		case RBUTTON_WAVE : return t->GetRButtonSound();
		case ITEM_KIND	:	return t->GetItemKind();
		case ITEM_LIMIT :	return t->GetLimit() ;
		case IMAGE_NUMBER : return t->GetImageNumber();
		case REPAIR_ABLE :	return t->GetRepairAble();
		case OBJECT_IMAGE : return t->GetObjectNumber();
		case ITEM_COLOR   :  return t->GetItemColor();		// 011018 KHS 망또
		case RAJA_EMBLEM  : // Gets the price (amount of raja emblems) of the item in the item mall
			if (!t->curr_value) ReqMallItemPrice(item_id);
			return t->curr_value;
	}	
	return 0;
}

int GetItemAttr( int item_id, int attr )
{
	return GetItemAttr2( SCharacterData.nCharacterData[GENDER], item_id, attr );
}

int GetItemValueBuy( int item_id )
{
	int type = item_id;
	int num = item_id;
	GetItemNumber( item_id , type, num );
	CItem *t= ItemUnit( type , num );
	if( t ) return t->GetItemValue();
	else return 0;

}

int GetItemValueSell( ItemAttr item )
{
	int type;
	int num; 
	GetItemNumber( item.item_no, type, num );

	CItem *t = ItemUnit( type , num );
	if( t ) //020925 lsw
	{
		int price = 0;
		const int base = t->curr_value;//GetItemAttr( item.item_no, VALUE );
		if( type == DISPOSABLE )
		{
			int much = t->GetQuantity();
			if( !much ) much = 1;
			price = (int)(
				((float)base * 0.9+0.5)* (float)item.attr[IATTR_MUCH] / (float)much);//020820 lsw
			return price; 
		}
		else if( GetItemAttr( item.item_no, DO_RBUTTON )==DIVIDE_ITEM )		// 수량을 의미...
		{
			int much = t->GetItemDuration();
			if( !much ) much = 1;
			price = (int)(
				((float)base * 0.9+0.5)* (float)item.attr[IATTR_MUCH] / (float)much);//020820 lsw
			return price;
		}

		WORD d_curr, d_max;
		GetItemDuration( item, d_curr, d_max );		// d_max => 현재 내구도 최대값
		WORD o_d_max = t->GetItemDuration();		// o_d_max => 실제 내구도 최대값
		if( d_max < o_d_max ) d_max = o_d_max;		// d_max에 더 큰 수를 넣는다.

		if( d_max )
		{	
			if( (item.attr[IATTR_LIMIT]-g_curr_time) > 0 )		// 유통기한이 남아 있어야 한다.
			{
				price = (int)( (base)*((float)d_curr/(float)d_max*.9)+.5 );
				return price;
			}
			// 유통기한을 넣야 한다.
			price = (int)( (base)*((float)d_curr/(float)d_max*.9)+.5 );		// 임시 
			return price;
		}
	}
	
	return 0;
}

void SetItemPos(int type, POS* IP_set)
{
	IP_set->type = (char)type;
	IP_set->p1 = 0;
	IP_set->p2 = 0;
	IP_set->p3 = 0;
}

void SetItemPos(int type, int p, POS* IP_set)
{
	IP_set->type = (char)type;
	IP_set->p1 = 0;
	IP_set->p2 = 0;
	IP_set->p3 = (short int)p;
}

void SetItemPos(int type, int p1, int p2, int p3, POS* IP_set)
{
	IP_set->type = ( char )type;
	IP_set->p1 = (short int)p1;
	IP_set->p2 = (short int)p2;
	IP_set->p3 = (short int)p3;
}

//extern int		aItemInventory[3][3][8];
//extern int		aAccelInventory[6];
//extern int		aArmor_Inventory[8];

//extern ItemAttr InvItemAttr[3][3][8];
//extern ItemAttr EquipItemAttr[8];
//extern ItemAttr QuickItemAttr[7];

//extern int WearAble[9];


void SendMoveItem( int item_no, POS s, POS t, bool sound )
{
	t_packet	packet;

	packet.h.header.type = CMD_ITEM_EXCHANGE;
	packet.h.header.size = sizeof( t_item_exchange );

	packet.u.item_exchange.item_no  = item_no;
	packet.u.item_exchange.pos_s  = s;
	packet.u.item_exchange.pos_t  = t;

	QueuePacket( &packet, 1 );
	
	if( sound )
	{
		if( t.type == HAND ) ItemSoundOfGetItem( item_no );
		else if( t.type == EQUIP || t.type == INV ) EquipSound( item_no );
		else if( s.type == HAND ) DropItemSound( item_no );
	}
	
	int type = WearAble[GetItemAttr( item_no, WEAR_ABLE)];
	ChangeEquip( Hero, EquipItemAttr, type );
}


void RecvChrItemInfo_0( t_chr_item_info0 *i )
{//021022 lsw
	int a = i->ar0;
	int b = i->ar1;
	int ct = (a*3)+b;

	for(int c=0;c<8;c++)
	{
		InvItemAttr[a][b][c] = i->inv[c];
	}
	ct++;
	if( !g_StartMenuOn )
	{
		if( ct==9 )
		{
			SendReCallItemFromServer( CMD_RESEND_ITEM_EQUIP );
			ct= 0;
		}
	}
	else
	{
		if( ct==9 )
		{
			ReqMagicInfo();
			ct = 0;
		}
		else
		{
			ReqItemInfo2( ct );
		}
	}
}

void RecvChrItemInfo_1( t_chr_item_info1 *i )
{	//< CSD-031001
	const WORD idWeaponItem = i->equip[WT_WEAPON].item_no;
	
	if (idWeaponItem/1000 == 6)
	{	//< CSD-031101
		CItem_Weapon* pWeapon = static_cast<CItem_Weapon*>(ItemUnit(idWeaponItem));

		if (pWeapon != NULL)
		{
			const int nEffect = pWeapon->effect;

			if (nEffect > 0)
			{
				LoadHeroClothAccessoryDataOne(Hero->sprno, nEffect);
			}

			switch (Hero->sprno)
			{
			case 0: LoadHeroClothAccessoryDataOne(Hero->sprno, pWeapon->shape_woman); break;
			case 1: LoadHeroClothAccessoryDataOne(Hero->sprno, pWeapon->shape_man); break;
			}
		}
	}	//> CSD-031101

	Hero->idWeaponItem = idWeaponItem;
	//> CSD-031001
	for(int a = 0; a < 8; ++a)
	{
		EquipItemAttr[a] = i->equip[a];
//		CallItemVirtualFunction( VF_EQUIP_ITEM, EquipItemAttr[a].item_no );
	}
	
	CheckAbility();

	g_SokSungFlag = 0;			// 속성 체크
	
	for (int t = 0; t < 8; ++t)
	{
		if (EquipItemAttr[t].attr[IATTR_RARE_MAIN])
		{
			g_SokSungFlag = 1;
			break;
		}
	}

	if (!g_StartMenuOn)
	{
		SendReCallItemFromServer( CMD_RESEND_ITEM_QUICK );
	}
	else
	{
		ReqItemInfo2(0);
	}
	// 011018 KHS 망또
	// Hero의 장착한 Item으로 처리할것은 여기서 한다. 
	Hero->mantle = EquipItemAttr[ WT_NECK].item_no;
	if (EquipItemAttr[ WT_NECK].attr[IATTR_RESERVED1] > 0)
	{
		int color = EquipItemAttr[ WT_NECK].attr[IATTR_RESERVED1];
		int r, g, b;
		r = (color>>16);
		color -= (r<<16);
		g = (color>>8);
		color -= (g<<8);
		b = color;
		Hero->mantlecolor = ReturnBlendRGB(r, g, b);
	}
	else
	{
		Hero->mantlecolor = GetItemAttr( Hero->mantle, ITEM_COLOR );
	}


	if (Hero->mantle)
	{
		int shape = GetItemAttr2(Hero->sprno, Hero->mantle, ITEMSHOW);
		LoadHeroClothAccessoryDataOne(Hero->sprno, shape);
		
		// finito 08/08/07 load extra images for mantle of dragon and mantle of raja
		if (shape == 137)
		{
			LoadHeroClothAccessoryDataOne(Hero->sprno, 138);
		}
		else if(shape == 145)
		{
			LoadHeroClothAccessoryDataOne(Hero->sprno, 146);
			LoadHeroClothAccessoryDataOne(Hero->sprno, 147);
		}
		// end of load extra images
	}
	// 011030 KHS JJING
	const int jjing = ((RareMain*)&EquipItemAttr[WT_ARMOR].attr[IATTR_RARE_MAIN])->grade;

	if (jjing >= 5)
	{
		Hero->jjing = jjing;
		LoadHeroClothAccessoryDataOne( Hero->sprno, GetItemAttr( EquipItemAttr[ WT_ARMOR].item_no, ITEMSHOW )+200);
	}
}

void RecvChrItemInfo_2( t_chr_item_info2 *i )
{
	int a; 

	for(a=0;a<6;a++)
	{
		QuickItemAttr[a] = i->quick[a];
	}

	//soto-1.02
	if(QuickItemAttr[5].item_no)
	{
		g_SymBolMgr.SetDualItem(QuickItemAttr[5].item_no,QuickItemAttr[5].attr[0]);
	}
	//soto-1.02

	if( g_StartMenuOn ) ReqItemInfo1( );
}

void RecvItemEquip( t_chr_status_info *chr )
{
	if( chr->fStr ) { SCharacterData.nCharacterAbility[STR] = chr->Str; } 
	if(	chr->fCon ) { SCharacterData.nCharacterAbility[CON] = chr->Con; } 
	if( chr->fDex ) { SCharacterData.nCharacterAbility[DEX] = chr->Dex; } 
	if(	chr->fWis ) { SCharacterData.nCharacterAbility[WIS] = chr->Wis; } 
	if(	chr->fInt ) { SCharacterData.nCharacterAbility[INT_] = chr->Int; } 
	if(	chr->fMoveP ) { SCharacterData.nCharacterAbility[MOVP] = chr->MoveP; } 
	if(	chr->fChar ) { SCharacterData.nCharacterAbility[CHA] = chr->Char; } 
	if(	chr->fEndu ) { SCharacterData.nCharacterAbility[ENDU] = chr->Endu; } 
	if(	chr->fHealth ) { SCharacterData.nCharacterAbility[HEALTH] = chr->Health; } 
	if(	chr->fMoral ) { SCharacterData.nCharacterAbility[MOR] = chr->Moral; } 
	if(	chr->fLuck ) { SCharacterData.nCharacterAbility[LUCK] = chr->Luck; } 
	if(	chr->fHp ) { SCharacterData.nCharacterHP = chr->Hp; } 
	if(	chr->fAp ) { SCharacterData.nCharacterSP = chr->Ap; } 
	if(	chr->fMp ) { SCharacterData.nCharacterMP = chr->Mp; } 
	if(	chr->fAc ) { SCharacterData.nCharacterAbility[AC] = chr->Ac; } 

	if(	chr->fDamage ) { SCharacterData.nCharacterAbility[DAMAGE] = chr->Dam_Min*1000 + chr->Dam_Max;} 
	if( chr->fHit ) { SCharacterData.nCharacterAbility[HIT] = chr->Hit; } 
	if(	chr->fWsPs ) { SCharacterData.nCharacterAbility[WSPS] = chr->WsPs; } 

	
//  에니메이션 번호를  old 데이터와 비교하여 다르면 서버로 정보를 보내준다.
//	SendAnimationInfo( );
//
}


//############################ 매직 아이콘 테이블 로드및 이미지 로드  ##########################//
CMagic magic[MAX_MAGIC_];
Spr		magic_spr[MAGIC_SPR_MAX];

void CMagic::LoadExplain(int num, FILE *fp)
{
	char temp[MAX_STRING_];
	char *token;

	fgets( temp, MAX_STRING_, fp );

	token = strtok(temp, "\t\n");
	token = strtok(NULL, "\t\n");
	token = strtok(NULL, "\t\n");
	if( token )	SetExplain(token);
}

bool ReadMagicExplain()
{
	char filename[MAX_FILENAME];
	FILE *fp;
	int num;

	if( SCharacterData.nCharacterData[SPELL] )
		sprintf(filename, "./magic/priest_ex.txt");
	else
		sprintf(filename, "./magic/wizard_ex.txt");

	if( fp=Fopen(filename, "rt") )
	{
		while( !feof(fp) )
		{
			char temp[20];
			char temp2[1023];

			if( !fscanf(fp, "%s", temp) ) break;
			if( temp[0] == '#' || temp[0] == ';') 
			{
				fgets(temp2, 1023, fp);
				continue;
			}
			num = atoi(temp);
			magic[num].LoadExplain(num, fp);
		}
		fclose(fp);
	}
	return 1;
}

bool ReadMagicTable()
{
  FILE* fp = NULL;
	//ReadMagicExplain();		// 먼저 설명을 읽어 들인후 데이타 값을 읽어서 한꺼번에 저장한다.
	char filename[MAX_FILENAME], filenameBin[MAX_FILENAME];
  memset(filename, 0, MAX_FILENAME);
  memset(filenameBin, 0, MAX_FILENAME);
	//< CSD-020702
  sprintf(filename, "./magic/");
  sprintf(filenameBin, "./magic/");
  //> CSD-020702
  switch (SCharacterData.nCharacterData[SPELL])
  {
    case WIZARD_SPELL:
    {
      strcat(filename, "wizard.txt");
      strcat(filenameBin, "wizard.bin");
      break;
    }
    case PRIEST_SPELL:
    {
      strcat(filename, "priest.txt");
      strcat(filenameBin, "priest.bin");
      break;
    }
  }

	if( fp=Fopen(filename, "rt") )
	{
		char temp[2048];

		memset( magic, 0, sizeof( CMagic ) * MAX_MAGIC_ );
		while( fgets( temp, 2048, fp ) )
		{
			if( temp[0] == '#' || temp[0] == ';') continue;
			char *token = strtok( temp, "\t\n" );
			int magic_no = atoi( token );
			magic[magic_no].SetMagicNum(magic_no);

			token = strtok( NULL, "\t\n" );
			magic[magic_no].SetName(token );

			token = strtok( NULL, "\t\n" );
			magic[magic_no].SetHanName(token );

			token = strtok( NULL, "\t\n" );
			
			magic[magic_no].SetData(0,atoi( token ));

			token = strtok( NULL, "\t\n" );
			magic[magic_no].SetData(1,atoi( token ));
			
			token = strtok( NULL, "\t\n" );
			magic[magic_no].SetData(2,atoi( token ));

			token = strtok( NULL, "\t\n" );
			magic[magic_no].SetData(3,atoi( token ));

			token = strtok( NULL, "\t\n" );
			magic[magic_no].SetExplain(token);
		}
		fclose(fp);
		fp=Fopen( filenameBin, "wb" );
		if(fp)
		{
			char *tt = (char *)magic;
			char crc = 0;
			for( int i=0; i< sizeof(CMagic)*MAX_MAGIC_ ; i++ )
			{
				crc += *tt;
				tt++ ;
			}
			fwrite( &crc, 1, 1, fp );
			fwrite( magic, sizeof(CMagic), MAX_MAGIC_, fp);
			fwrite( &crc, 1, 1, fp );
			fclose(fp);
		}
		for( int i=0; i<MAX_MAGIC_; i++ )
		{
			if( magic[i].GetMagicNum() ) 
				ReadMagicIconImage( SCharacterData.nCharacterData[SPELL], magic[i].GetImage() );
		}
	}
	else
	{
		fp = Fopen( filenameBin, "rb" );
		if(fp)
		{
			char *tt = (char *) magic;
			char crc = 0, crc1, crc2;
			fread( &crc1, 1, 1, fp );
			fread( magic, sizeof(CMagic), MAX_MAGIC_, fp);
			fread( &crc2, 1, 1, fp );
			fclose(fp);
			for( int i=0; i< sizeof( CMagic ) *MAX_MAGIC_; i++ )
			{
				crc += *tt;
				tt++ ;
			}
			if( crc != crc1 || crc != crc2 ) 
			{
				CheckSumError = -1;
				JustMsg(lan->OutputMessage(6,82) );//CheckSumError = 1;//010215 lsw
			}
			else	// 매직 아이콘 읽어 오기...
			{
				for( int i=0; i<MAX_MAGIC_; i++ )
					if( magic[i].GetMagicNum() ) 
						ReadMagicIconImage( SCharacterData.nCharacterData[SPELL], magic[i].GetImage() );
			}
		}
		else return 0;
	}
/*
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		fp = fopen( "kein_wizard.txt", "wt" );
		if( fp) 
		{
			for( int i=0; i<MAX_MAGIC_; i++ )
			{
				if( magic[i].MagicNum )
				fprintf( fp, "%d	%s	%s	%d	%d	%d	%d	%s\n", 
					magic[i].MagicNum, magic[i].Name, magic[i].HanName,
					magic[i].data[0], magic[i].data[1], magic[i].data[2], magic[i].data[3], 
					magic[i].explain );
			}
			fclose( fp );
		}
	}
	*/


	return 1;
}

void CMagic::DeleteMagic()
{
	if( !GetMagicNum() ) return;

	if( magic_spr[ m_data[ImgNum] ].img )
	{
		GlobalFree( magic_spr[ m_data[ImgNum] ].img );
	}
	magic_spr[ m_data[ImgNum] ].img = NULL;

	m_iMagicNum=0;
	SetName("");
	SetHanName("");
	memset( m_data, 0, sizeof( int[MAGIC_DATA_MAX] ) );
}



bool ReadMagicIconImage( int type, int num )
{
	char filename[MAX_FILENAME];
	FILE *fp;

	Spr *s;

	switch( type )
	{
		case WZD :		sprintf( filename, "./Magic/w%03d.csp", num ); break;
		case PRI :		sprintf( filename, "./Magic/p%03d.csp", num ); break;
	}
	s = &magic_spr[num];
	if( s->img ) return 1;

	if( fp = Fopen(filename, "rb" ) )
	{
		fread( &s->xl, sizeof( short ), 1, fp);
		fread( &s->yl, sizeof( short ), 1, fp);
		fread( &s->ox, sizeof( short ), 1, fp);
		fread( &s->oy, sizeof( short ), 1, fp);
		fread( &s->size, sizeof( unsigned int ), 1, fp);

		MemFree( s->img );
		MemAlloc( s->img, s->size );
		fread(s->img, s->size, 1, fp);
		convert565to555( s );
		fclose(fp);
	}
	else return 0;
	return 1;
}

void FreeMagicIcon( )
{
	for( int i=0; i<MAGIC_SPR_MAX; i++ )
	{
		if( magic_spr[i].img ) MemFree( magic_spr[i].img );
	}
}

void PutMagicIcon( int x, int y, int num, int flag, int type )
{
	Spr *s;
//	if( !magic[num].GetImage() ) return;
	s = &magic_spr[magic[num].GetImage()];
	if( flag ) 
	{
		PutCompressedImageFX(x, y, s, flag, type); 
	}
	else 
	{
		PutCompressedImage(x, y, s);
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//
//		매직 관련 계산 메소드 --> 서버
//
//
////////////////////////////////////////////////////////////////////////////////////////




//###########################################################################################

/*
extern ItemAttr InvItemAttr[3][3][8];
extern ItemAttr EquipItemAttr[8];
extern ItemAttr QuickItemAttr[7];
extern ItemAttr HandItemAttr;

extern ItemAttr InvAttrBackUp[3][3][8];
extern ItemAttr EquipAttrBackUp[8];
extern ItemAttr QuickAttrBackUp[6];
extern ItemAttr HandAttrBackUp;

extern int aAccelInventory[6];		
extern int AccelBackUp[6];

extern int aArmor_Inventory[8];
extern int ArmorBackUp[8];

extern int aItemInventory[3][3][8];
extern int InvBackUp[3][3][8];

extern int HandBackUp;
extern int HandBackUptype;

extern int g_MouseItemNumber;   
extern int g_MouseItemType;     
*/

void InventoryBackUp()
{
	memcpy( InvAttrBackUp, InvItemAttr, sizeof( ItemAttr[3][3][8] ) );
	memcpy( EquipAttrBackUp, EquipItemAttr, sizeof( ItemAttr[8] ) );
	memcpy( QuickAttrBackUp, QuickItemAttr, sizeof( ItemAttr[7] ) );
	HandAttrBackUp = HandItemAttr;

//	memcpy( AccelBackUp, aAccelInventory, sizeof( int[6] ) );
//	memcpy( ArmorBackUp, aArmor_Inventory, sizeof( int[8] ) );
//	memcpy( InvBackUp, aItemInventory, sizeof( int[3][3][8] ) );
	HandBackUp = g_MouseItemNumber;
	HandBackUptype = g_MouseItemType;

}

void InventoryRestore()
{
	// 재 전송 받는다.
	CallServer( CMD_RESEND_ITEM );
	DeleteItem( &HandItemAttr );
	g_MouseItemType = 0;
	g_MouseItemNumber = 0;

	ChangeEquip( Hero, EquipItemAttr, 0 );
}

int Item_Attack_Ani[27][8] = 
{
	0,	50,	65,		0,	0,	0,	0,	0,		//양손검	
	61,	50,	70,		4,	3,	5,	6,	8,		//한손칼..
	62,	50,	70,		4,	3,	5,	6,	8,		//한손칼..
	63,	50,	70,		4,	3,	5,	6,	8,		//불칼.
	64,	50,	70,		4,	3,	5,	6,	8,		//
	65,	50,	70,		4,	3,	5,	6,	8,		//
	66,	50,	70,		4,	3,	5,	6,	0,		//
	67,	50,	70,		4,	3,	5,	6,	0,		//
	68,	50,	70,		22,	21,	7,	23,	0,		//해머
	69,	50,	70,		4,	3,	5,	6,	0,		//
	70,	50,	70,		4,	3,	7,	8,	23,		//봉1.
	71,	50,	70,		4,	3,	7,	6,	23,		//
	72,	70,	90,		22,	21,	7,	6,	23,		//
	73,	70,	90,		22,	21,	7,	2,	30,		//	
	74,	80,	100,	22,	21,	8,	0,	0,		//창1.
	75,	70,	90,		22,	21,	8,	0,	0,		//
	76,	70,	90,		4,	3,	19,	0,	0,		//
	77,	70,	90,		4,	3,	19,	0,	0,		//
	78,	50,	480,	4,	3,	9,	0,	0,		//활1
	79,	70,	530,	4,	3,	9,	0,	0,		//	
	80,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	81,	40,	70,		10,	3,	11,	0,	0,		//
	82,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	83,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	84,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	85,	40,	70,		10,	3,	11,	0,	0,		//....?,,
	86,	40,	70,		10,	3,	11,	0,	0,		//....?,,
};

void GetItemAnimationNumber(int num, int &a, int &b, int &c, int &d, int &e, int &f, int &g)
{
	if(num)
	{
		a = Item_Attack_Ani[num-60][1];
		b = Item_Attack_Ani[num-60][2];
		c = Item_Attack_Ani[num-60][3];
		d = Item_Attack_Ani[num-60][4];
		e = Item_Attack_Ani[num-60][5];
		f = Item_Attack_Ani[num-60][6];
		g = Item_Attack_Ani[num-60][7];
	}
	else
	{
		a = Item_Attack_Ani[0][1];
		b = Item_Attack_Ani[0][2];
		c = Item_Attack_Ani[0][3];
		d = Item_Attack_Ani[0][4];
		e = Item_Attack_Ani[0][5];
		f = Item_Attack_Ani[0][6];
		g = Item_Attack_Ani[0][7];
	}
}
// 020620 YGI
void CSkill::ReadSkill( int no, char *buf )
{
	num = no;
	char *token;

#ifdef KOREA_LOCALIZING_
	token = strtok(buf, "\t\n");
	strcpy(name, token);

	token = strtok(NULL, "\t\n");
	strcpy(han_name, token);
#else
	token = strtok(buf, "\t\n");
	strcpy(han_name, token);

	token = strtok(NULL, "\t\n");
	strcpy(name, token);
#endif

	token= strtok(NULL, "\t\n") ;
	inclusive = atoi(token);

	token= strtok(NULL, "\t\n") ;
	series = atoi(token);

	token= strtok(NULL, "\t\n") ;
	money = atoi(token);

	token= strtok(NULL, "\t\n\r") ;
	skillable = atoi(token);

	token= strtok(NULL, "\t\n") ;
	strcpy(explain, token);

	static int count=0;

	int ct = strlen( token );
	if( ct > count ) count = ct;

}

CSkill	skill[MAX_SKILL];
Spr		skill_spr[MAX_SKILL];
int		inclusive_to_skill[MAX_SKILL_KIND][8];

bool ReadSkillTable()
{
	char buf[511] ;
	char temp[20];

	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	
	sprintf(filename, "./skill/skill.txt");
	sprintf(filenameBin, "./skill/skill.bin");

	if( fp= Fopen(filename, "rt") )
	{
		//int i=0;
		int num = 0;
		while( !feof(fp) && num<MAX_SKILL )
		{
			if( !fscanf(fp, "%s", temp) ) break;
			if( temp[0] == '#' || temp[0] == ';') 
			{
				fgets(buf, 511, fp);
				continue;
			}
			num = atoi( temp );
			fgets(buf, 511, fp);
			skill[num].ReadSkill(num, buf);
			ReadSkillIcon( num );
//			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			char *tt = (char *) skill;
			char crc = 0; 
			for( int i=0; i<sizeof( CSkill ) *MAX_SKILL; i++ )
			{
				crc += *tt;
				tt++;
			}

			fwrite( &crc, 1, 1, fp);
			fwrite( skill, sizeof(CSkill), MAX_SKILL, fp);
			fwrite( &crc, 1, 1, fp);
			fclose(fp);
		}
	}

	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			char *tt = (char *) skill ;
			char crc = 0, crc1, crc2;
			fread( &crc1, 1, 1, fp );
			fread( skill, sizeof(CSkill), MAX_SKILL, fp);
			fread( &crc2, 1, 1, fp );
			fclose(fp);

			for( int i=0; i<sizeof( CSkill )*MAX_SKILL; i++)
			{
				crc += *tt;
				tt++;
			}
			if( crc != crc1 || crc != crc2 ) 
			{
				CheckSumError = -1;
				JustMsg(lan->OutputMessage(6,83) );//CheckSumError = 1;//010215 lsw
			}
			else		// 이미지 읽어 오기
			{
				for( int i=0; i<MAX_SKILL; i++ )
				{
					if( skill[i].num ) 
					{
						ReadSkillIcon( i );
					}
				}

#ifdef _DEBUG	// 031110 YGI
				if( GetSysInfo( SI_NOT_CONNECT_MODE ) )
				{	//< CSD-031201 : 클라이언트 실행시 크래쉬되어 막음
					/*
					FILE *fp = fopen( filename, "wt" );
					if( fp )
					{
						for( int i=0; i<MAX_SKILL; i++ )
						{
							if( skill[i].num )
							fprintf( fp, "%d	%s	%s	%d	%d	%d	%d	%s\n", 
								skill[i].num, skill[i].name, skill[i].han_name, skill[i].inclusive,
								skill[i].series, skill[i].money, skill[i].skillable, skill[i].explain );							
						}
						fclose( fp );
					}*/
				}	//> CSD-031201
#endif
			}
		}
		else return 0;
	}

	CalcGuildToSkill( );		// 포괄적 스킬당 세부 스킬로 분류한다.

	ConvertSkillTable( );		// skillmain과 값을 동기화 한다.
	return 1;
}

////////////////////////////////////////////////
//
//	SkillMain과 맞춘다.
void ConvertSkillTable( )
{
	for( int i=0; i<MAX_SKILL_; i++ )
	{
		skill[i].inclusive	= SkillTable[i].MotherSkillType;
		skill[i].money		= SkillTable[i].money;
		skill[i].skillable	= SkillTable[i].need_exp;
		skill[i].targettype	= SkillTable[i].Howto;
	}
}

bool ReadSkillIcon( int num )
{
	char filename[MAX_FILENAME];
	FILE *fp;
	Spr *s;

	sprintf( filename, "./Skill/s%03d.csp", num );
	s = &skill_spr[num];

	if( fp = Fopen(filename, "rb" ) )
	{
		fread( &s->xl, sizeof( short ), 1, fp);
		fread( &s->yl, sizeof( short ), 1, fp);
		fread( &s->ox, sizeof( short ), 1, fp);
		fread( &s->oy, sizeof( short ), 1, fp);
		fread( &s->size, sizeof( unsigned int ), 1, fp);
		MemFree( s->img );
		MemAlloc( s->img, s->size );
		fread(s->img, s->size, 1, fp);
		convert565to555( s );
		fclose(fp);
	}
	else return 0;

	return 1;
}


void PutSkillIcon( int x, int y, int num, int flag, int flag2 )
{
	Spr *s;
	s = &skill_spr[num];
	if(!s->img) return ;
	if( flag ) PutCompressedImageFX(x, y, s, flag, flag2);
	else PutCompressedImage(x, y, s);
}



#define MAX_SKILL_OF_KIND		20
//
//	특정 스킬종류에 속해 있는 모든 스킬 번호를 가져온다.
//	para : 33개 짜리 스킬 종류, 그 스킬 번호를 담아갈 배열, 그 크기
//
int GetSkillMother( int kind , int skill_mother[], int max )
{
	memset( skill_mother, 0, sizeof( int )*max );
	int count = 0;
	for( int i=0; i<MAX_SKILL; i++ )
	{
		if( skill[i].num )
		{
			if( skill[i].inclusive == kind )
			{
				skill_mother[count++] = skill[i].num;
				if( count >= max ) break;
			}
		}
	}
	return count;
}

/*
int skill_mother[32][2] = {	{1,3},   {3,5},   {5,8},   {8,10},  {10,16}, {16,19}, {19,21}, {21,25}, {25,28}, {28,32},
							{32,34}, {34,35}, {35,38}, {38,42}, {42,43}, {43,44}, {44,45}, {45,48}, {48,50}, {50,53},
							{53,55}, {55,56}, {56,57}, {57,64}, {64,67}, {67,73}, {73,75}, {75,76}, {76,78}, {78,86},
							{86,87}, {87,101}	};
*/	
extern int SkillInventory[2][2][4];
extern int SkillInventory2[2][2][4];
void ChangeSkillMenuInventory( int x )
{
	int i, ct;
	memset( SkillInventory, 0 , sizeof( int[2][2][4] ) );
	memset( SkillInventory2, 0 , sizeof( int[2][2][4] ) );

	int skill_mother[MAX_SKILL_OF_KIND];
	int count = GetSkillMother( x, skill_mother, MAX_SKILL_OF_KIND );		// 같은 종류의 스킬을 찾는다.
	
	for( ct=0; ct<count; ct++ )
	{
		i = skill_mother[ct];
		if( SCharacterData.SkillId[i] ) SkillInventory[0][ct/4][ct%4] = i;
		else SkillInventory2[0][ct/4][ct%4] = i;
	}

	count = GetSkillMother( x+1, skill_mother, MAX_SKILL_OF_KIND );		// 같은 종류의 스킬을 찾는다.
	for( ct=0; ct<count; ct++ )
	{
		i = skill_mother[ct];
		if( SCharacterData.SkillId[i] ) SkillInventory[1][ct/4][ct%4] = i;
		else SkillInventory2[1][ct/4][ct%4] = i;
	}
}

bool	IsEquipAbleItemKind(const int iItemKind)
{
	if (IK_FALG == iItemKind)
	{
		CItem * t = ItemUnit( EquipItemAttr[WT_SHOES].item_no);
		if(!t)
		{
			return false;
		}
		const int iCmpItemKind= t->GetItemKind();
		if(IK_SADDLE != iCmpItemKind)
		{
			return false;
		}
	}	
	return true;
}

bool	IsEquipAbleAbilityCheck	(const int iNeedAbility, const int iMyAbility)
{
	if( (!iNeedAbility) || (iNeedAbility<=iMyAbility) )
	{
		return true;
	}
	return false;
}

bool	IsEquipAbleCheckWisInt	(const int iNeedWis, const int iNeedInt, const int iMyWis, const int iMyInt)
{
	if( iNeedWis && (!iNeedInt))//Wis만 요구 하면
	{
		return IsEquipAbleAbilityCheck(iNeedWis,iMyWis);
	}
	if( (!iNeedWis) && iNeedInt)//Int만 요구 하면
	{
		return IsEquipAbleAbilityCheck(iNeedInt,iMyInt);
	}
	if( (!iNeedWis) && (!iNeedInt))//둘다 필요 하지 않다
	{
		return true;
	}
	if( iNeedWis && iNeedInt )//둘다 요구 하면
	{
		if(IsEquipAbleAbilityCheck(iNeedWis,iMyWis) || IsEquipAbleAbilityCheck(iNeedInt,iMyInt))//둘다 만족 해야 함
		{
			return true;
		}
	}
	return false;
}

bool	IsEquipAbleCheckWsPs	(const int iNeedWs, const int iNeedPs, const int iMyWsPs, const int iSpellType)
{
	switch (iSpellType)
	{
		case WIZARD_SPELL:
			{
				if(IsEquipAbleAbilityCheck(iNeedWs,iMyWsPs))
				{
					return true;
				}
			}break;
		case PRIEST_SPELL:
			{
				if(IsEquipAbleAbilityCheck(iNeedPs,iMyWsPs))
				{
					return true;
				}
			}break;
		default:
			{
			}break;
	}
	return false;
}
bool	IsEquipAbleCheckGender	(const int iNeedGender, const int iMyGender)
{
	switch(iNeedGender)
	{
	case 0:
		{
			return true;
		}break;
	case 1:
		{
			if(iMyGender == MALE)
			{
				return true;
			}
		}break;
	case 2:
		{
			if(iMyGender == FEMALE)
			{
				return true;
			}
		}break;
	default :
		{
			return false;
		}break;
	}
	return false;
}
bool	IsEquipAbleCheckMouseMRD	(const int iNeedMouseMRD, const int iMyMouseMRD)// 택틱체크
{
	if( (iNeedMouseMRD <= 1) || (iNeedMouseMRD <= iMyMouseMRD) )
	{
		return true;
	}
	return false;
}

inline bool IsEquipAbleCheckNation(const int iAbleNation, const CHARACTER *ch)//000  100 바이 10 자이 1 일스
{
	switch(ch->name_status.nation)
	{
	case NW_BY:
		{
			if((iAbleNation%1000)/100)
			{
				return true;
			}
		}break;
	case NW_ZY:
		{
			if((iAbleNation%100)/10)
			{
				return true;
			}
		}break;
	case NW_YL:
		{
			if((iAbleNation%10)/1)
			{
				return true;
			}
		}break;
	default :
		{
			return false;
		}break;
	}
	return false;
}

inline bool IsEquipAbleCheckLv(const int iAbleLv, const CHARACTER *ch)
{
	if(!iAbleLv){return true;}//ㅇ구 
	if( iAbleLv <= ch->lv)
	{
		return true;
	}
	return false;
}

inline bool IsEquipAbleCheckFame(const int iAbleFame, const CHARACTER *ch)
{
	if(!iAbleFame){return true;}//ㅇ구 
	if( iAbleFame <= ch->fame)
	{
		return true;
	}
	return false;
}

bool	IsEquipAbleDualStep(const int iDualStep, const CHARACTER *ch)
{
	return ((iDualStep <= ch->aStepInfo[CLS_STEP])?true:false);
}

bool	IsEquipAbleClass(const int iWarrior,const int iThief,
						 const int iArcher,const int iWizard,
						 const int iPriest,const CHARACTER *ch)
{
	switch(ch->class_type)
	{
	case WARRIOR:
		{
			if(!iWarrior){return false;}
		}break;
	case THIEF	:
		{
			if(!iThief){return false;}
		}break;
	case ARCHER	:
		{
			if(!iArcher){return false;}
		}break;
	case WIZARD	:
		{
			if(!iWizard){return false;}
		}break;
	case PRIEST	:
		{
			if(!iPriest){return false;}
		}break;
	default:
		{
			return false;
		}break;
	}
	return true;
}

//################################## 장착 가능 판단 함수 #########################################
int CItem_Weapon::is_equipable()
{
	if(!IsEquipAbleAbilityCheck	(Need3_str	, Hero_Ability[STR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_con	, Hero_Ability[CON]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_dex	, Hero_Ability[DEX]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_end	, Hero_Ability[ENDU]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_cha	, Hero_Ability[CHA]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_mor	, Hero_Ability[MOR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_Movp	, Hero_Ability[MOVP]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_luc	, Hero_Ability[LUCK]))	{return 0;}

	if(!IsEquipAbleCheckMouseMRD(Mouse_MRD	, SCharacterData.SkillId[Skill_Ability])){return 0;}
	if(!IsEquipAbleCheckWisInt	(Need3_wis	, Need3_int, Hero_Ability[WIS],Hero_Ability[INT_]) ){return 0;}
	if(!IsEquipAbleCheckWsPs	(Need3_ws	, Need3_ps, Hero_Ability[WSPS], Hero->spell) ){return 0;}
	if(!IsEquipAbleCheckGender	(Need3_gender, SCharacterData.nCharacterData[GENDER]) )			{return 0;}
	if(!IsEquipAbleCheckAbleDay	(Repair_Skill2_min,Repair_Res1) )		{return 0;}
	if(!IsEquipAbleCheckNation(Imunity_Cure_4, Hero))		{return 0;}
	if(!IsEquipAbleCheckLv(Imunity_Cure_5, Hero))		{return 0;}
	if(!IsEquipAbleCheckFame(Imunity_Cure_6, Hero))		{return 0;}
	if(!IsEquipAbleItemKind(GetItemKind())) {return 0;}
	if(!IsEquipAbleDualStep(GetNeedDualStep(),Hero)){return 0;}
	if(!IsEquipAbleClass(
		GetClassWarriorAble(),GetClassThiefAble(),
		GetClassArcherAble(),GetClassWizardAble(),
		GetClassClericAble(),Hero)){return 0;}//021113 lsw
	return 1;
}

int CItem_Tool::is_equipable()
{
	return 1;
}

int CItem_Armor::is_equipable()
{
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver ) return 1;
#endif
	if(!IsEquipAbleAbilityCheck	(Need3_str	, Hero_Ability[STR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_con	, Hero_Ability[CON]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_dex	, Hero_Ability[DEX]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_end	, Hero_Ability[ENDU]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_cha	, Hero_Ability[CHA]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_mor	, Hero_Ability[MOR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_Movp	, Hero_Ability[MOVP]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_luc	, Hero_Ability[LUCK]))	{return 0;}

	if(!IsEquipAbleCheckMouseMRD(Mouse_MRD	, SCharacterData.SkillId[Skill_Ability])){return 0;}
	if(!IsEquipAbleCheckWisInt	(Need3_wis	, Need3_int, Hero_Ability[WIS],Hero_Ability[INT_]) ){return 0;}
	if(!IsEquipAbleCheckWsPs	(Need3_ws	, Need3_ps, Hero_Ability[WSPS], Hero->spell) ){return 0;}
	if(!IsEquipAbleCheckGender	(Need3_gender, SCharacterData.nCharacterData[GENDER]) )			{return 0;}
	if(!IsEquipAbleCheckAbleDay	(Repair_Skill2_min,Repair_Res1) )		{return 0;}
	if(!IsEquipAbleCheckNation(Imunity_Cure_4, Hero))		{return 0;}
	if(!IsEquipAbleCheckLv(Imunity_Cure_5, Hero))		{return 0;}
	if(!IsEquipAbleCheckFame(Imunity_Cure_6, Hero))		{return 0;}
	if(!IsEquipAbleItemKind(GetItemKind())){return 0;}
	if(!IsEquipAbleDualStep(GetNeedDualStep(),Hero)){return 0;}
	if(!IsEquipAbleClass(
		GetClassWarriorAble(),GetClassThiefAble(),
		GetClassArcherAble(),GetClassWizardAble(),
		GetClassClericAble(),Hero)){return 0;}//021113 lsw
	return 1;
}

int CItem_Disposable::is_equipable()
{
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver ) return 1;
#endif
	if(!IsEquipAbleAbilityCheck	(Need3_str	, Hero_Ability[STR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_con	, Hero_Ability[CON]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_dex	, Hero_Ability[DEX]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_end	, Hero_Ability[ENDU]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_cha	, Hero_Ability[CHA]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_mor	, Hero_Ability[MOR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_Movp	, Hero_Ability[MOVP]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_luc	, Hero_Ability[LUCK]))	{return 0;}

	if(!IsEquipAbleCheckMouseMRD(Mouse_MRD	, SCharacterData.SkillId[Skill_Ability])){return 0;}
	if(!IsEquipAbleCheckWisInt	(Need3_wis	, Need3_int, Hero_Ability[WIS],Hero_Ability[INT_]) ){return 0;}
	if(!IsEquipAbleCheckWsPs	(Need3_ws	, Need3_ps, Hero_Ability[WSPS], Hero->spell) ){return 0;}
	if(!IsEquipAbleCheckGender	(Need3_gender, SCharacterData.nCharacterData[GENDER]) )			{return 0;}
//	if(!IsEquipAbleCheckAbleDay	(Repair_Skill2_min,Repair_Res1) )		{return 0;}
//	if(!IsEquipAbleCheckNation(Imunity_Cure_4, Hero))		{return 0;}
//	if(!IsEquipAbleCheckLv(Imunity_Cure_5, Hero))		{return 0;}
//	if(!IsEquipAbleCheckFame(Imunity_Cure_6, Hero))		{return 0;}
	if(!IsEquipAbleItemKind(GetItemKind())){return 0;}
	return 1;
}
int CItem_Accessory::is_equipable()
{
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver ) return 1;
#endif
		if(!IsEquipAbleAbilityCheck	(Need3_str	, Hero_Ability[STR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_con	, Hero_Ability[CON]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_dex	, Hero_Ability[DEX]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_end	, Hero_Ability[ENDU]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_cha	, Hero_Ability[CHA]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_mor	, Hero_Ability[MOR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_Movp	, Hero_Ability[MOVP]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_luc	, Hero_Ability[LUCK]))	{return 0;}

	if(!IsEquipAbleCheckMouseMRD(Mouse_MRD	, SCharacterData.SkillId[Skill_Ability])){return 0;}
	if(!IsEquipAbleCheckWisInt	(Need3_wis	, Need3_int, Hero_Ability[WIS],Hero_Ability[INT_]) ){return 0;}
	if(!IsEquipAbleCheckWsPs	(Need3_ws	, Need3_ps, Hero_Ability[WSPS], Hero->spell) ){return 0;}
	if(!IsEquipAbleCheckGender	(Need3_gender, SCharacterData.nCharacterData[GENDER]) )			{return 0;}
	if(!IsEquipAbleCheckAbleDay	(Repair_Skill2_min,Repair_Res1) )		{return 0;}
	if(!IsEquipAbleCheckNation(Imunity_Cure_4, Hero))		{return 0;}
	if(!IsEquipAbleCheckLv(Imunity_Cure_5, Hero))		{return 0;}
	if(!IsEquipAbleCheckFame(Imunity_Cure_6, Hero))		{return 0;}
	if(!IsEquipAbleItemKind(GetItemKind())){return 0;}
	if(!IsEquipAbleDualStep(GetNeedDualStep(),Hero)){return 0;}
	if(!IsEquipAbleClass(
		GetClassWarriorAble(),GetClassThiefAble(),
		GetClassArcherAble(),GetClassWizardAble(),
		GetClassClericAble(),Hero)){return 0;}//021113 lsw
	return 1;
}

int CItem_Etc::is_equipable()
{
#ifdef _DEBUG // finito 060507
	if( SysInfo.notconectserver ) return 1;
#endif
	if(!IsEquipAbleAbilityCheck	(Need3_str	, Hero_Ability[STR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_con	, Hero_Ability[CON]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_dex	, Hero_Ability[DEX]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_end	, Hero_Ability[ENDU]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_cha	, Hero_Ability[CHA]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_mor	, Hero_Ability[MOR]))		{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_Movp	, Hero_Ability[MOVP]))	{return 0;}
	if(!IsEquipAbleAbilityCheck	(Need3_luc	, Hero_Ability[LUCK]))	{return 0;}

	if(!IsEquipAbleCheckMouseMRD(Mouse_MRD	, SCharacterData.SkillId[Skill_Ability])){return 0;}
	if(!IsEquipAbleCheckWisInt	(Need3_wis	, Need3_int, Hero_Ability[WIS],Hero_Ability[INT_]) ){return 0;}
	if(!IsEquipAbleCheckWsPs	(Need3_ws	, Need3_ps, Hero_Ability[WSPS], Hero->spell) ){return 0;}
	if(!IsEquipAbleCheckGender	(Need3_gender, SCharacterData.nCharacterData[GENDER]) )			{return 0;}
	if(!IsEquipAbleCheckAbleDay	(Repair_Skill2_min,Repair_Res1) )		{return 0;}
	if(!IsEquipAbleCheckNation(Imunity_Cure_4, Hero))		{return 0;}
	if(!IsEquipAbleCheckLv(Imunity_Cure_5, Hero))		{return 0;}
	if(!IsEquipAbleCheckFame(Imunity_Cure_6, Hero))		{return 0;}
	if(!IsEquipAbleItemKind(GetItemKind())){return 0;}
	if(!IsEquipAbleDualStep(GetNeedDualStep(),Hero)){return 0;}
	if(!IsEquipAbleClass(
		GetClassWarriorAble(),GetClassThiefAble(),
		GetClassArcherAble(),GetClassWizardAble(),
		GetClassClericAble(),Hero)){return 0;}//021113 lsw
	return 1;
}

//int equip_able[] = { 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 서버와 똑같이 장착 루틴 구현
int CItem_Weapon::is_equipable2( int dest_pos )
{
	if( wear_able != WearAble[dest_pos] )
	{
		if( (wear_able != WEAR_TWO_HAND) ) return NOT;
		if( EquipItemAttr[WT_SHIELD].item_no ) 
		{
			switch(Item_kind)
			{
			case IK_DUAL_BOW://030102 lsw Dual BOW
			case IK_SHORT_BOW:
			case IK_LONG_BOW:
				{
				}break;
			default:
				{
					return NOT;		// 활이 아닌경우
				}
			}
			CItem *t = ItemUnit( EquipItemAttr[WT_SHIELD] );
			if( t )
			{
				switch(t->GetItemKind())
				{
				case IK_TWO_ARROW:
				case IK_DUAL_ARROW://030102 lsw ARROW
					{
					}break;
				default:
					{
						return NOT;// 활이긴 한데 활이외의 것을 들고 있는 경우 
					}break;
				}
			}
		}
		if( dest_pos != WT_WEAPON ) return NOT;
	}
	else
	{
		switch(wear_able)
		{
		case WEAR_RIGHT_HAND://WT_WEAPON Position
			{
				CItem *t = ItemUnit( EquipItemAttr[WT_SHIELD] );
				if( t )
				{
					switch(t->GetItemKind())
					{
					case IK_TWO_ARROW:
					case IK_DUAL_ARROW://030102 lsw ARROW
						{
							return NOT;//SHIELD Position in Arrow
						}break;
					}
				}
			}break;
		case WEAR_LEFT_HAND:
			{
				switch(GetItemKind())
				{
				case IK_TWO_ARROW:
				case IK_DUAL_ARROW:
					{
						CItem *t = ItemUnit( EquipItemAttr[WT_WEAPON] );
						if( t )
						{
							switch(t->GetItemKind())
							{
							case IK_DUAL_BOW://030102 lsw Dual BOW
							case IK_SHORT_BOW:
							case IK_LONG_BOW:
								{	
								}break;
							default:
								{
									return NOT;
								}break;
							}
						}
					}
				}
			}break;
		}
	}
	return is_equipable();
}

int CItem_Tool::is_equipable2( int dest_pos )
{
	return is_equipable();
}

int CItem_Armor::is_equipable2( int dest_pos )
{
	if( wear_able != WearAble[dest_pos] )			// 위치가 안 맞을 경우 // 0621 YGI
	{
		if( (wear_able != WEAR_TWO_HAND) ) return NOT;
		if( EquipItemAttr[WT_SHIELD].item_no ) return NOT;
		if( dest_pos != WT_WEAPON ) return NOT;
	}

	if( wear_able == WEAR_LEFT_HAND )						// 양손검을 사용할때 방패를 착용하면...
	{
		CItem *t = ItemUnit( EquipItemAttr[WT_WEAPON] );
		if( t && t->GetItemClass() == WEAR_TWO_HAND ) return NOT;
	}
	return is_equipable();
}


int CItem_Disposable::is_equipable2( int dest_pos )
{
	if( wear_able != WearAble[dest_pos] )			// 위치가 안 맞을 경우 // 0621 YGI
	{
		if( (wear_able != WEAR_TWO_HAND) ) return NOT;		// 양손무기가 아닐때
		if( EquipItemAttr[WT_SHIELD].item_no ) return NOT;		// 방패자리에 뭔가 있다면
		if( dest_pos != WT_WEAPON ) return NOT;				// 위치가 무기 자리가 아니면 NOT를 리턴
	}
	else 
	{
		if(	Item_kind == IK_TWO_ARROW
		||	Item_kind == IK_DUAL_ARROW)					// 양손검을 사용할때 화살을 장착하면//030102 lsw Arrow
		{
			CItem *t = ItemUnit( EquipItemAttr[WT_WEAPON] );
			if(t)
			{
				switch(t->GetItemKind())
				{
				case IK_DUAL_BOW://030102 lsw Dual BOW
				case IK_SHORT_BOW:
				case IK_LONG_BOW:
					{
					}break;
				default:
					{
						return NOT;		// 활이 아닌경우
					}
				}
			}
			else
			{
				return NOT;
			}
		}
	}
	return is_equipable();
}

int CItem_Accessory::is_equipable2( int dest_pos )
{
	if( wear_able != WearAble[dest_pos] ) return NOT;		// 위치가 안 맞을 경우
	return is_equipable();
}

int CItem_Etc::is_equipable2( int dest_pos )
{
	if( wear_able != WearAble[dest_pos] ) return NOT;		// 위치가 안 맞을 경우

	return is_equipable();
}
// 0624 YGI////////////////////////////////////////////////////////////////////////////////// 여기까지















//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//###################### 매직 설명문 #######################


void GetLearnKnowlageInv( int start )			// 지식 기술을 배울때 가져올 스킬
{
	int a, b, s;
	memset( SkillInventory, 0, sizeof( int[2][2][4] ) );

	motherSkillImageNo[0] = 636;	// IDENTIFICATION // 현재는 임시
//	motherSkillImageNo[1] = 637;	// HEALING
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[6].nSHideNomalCount=1;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[6].nSHideNomalStart=start;
	SMenu[MN_SKILLGUILD_KNOWLAGE].nField[5].nType=FT_NOMAL_PUT;	// other button

	int skill_mother[MAX_SKILL_OF_KIND];
	int count = GetSkillMother( ITEM_IDENTIFICATION, skill_mother, MAX_SKILL_OF_KIND );
	s=skill_mother[0]; a=0; b=0;
	while(  count>0 && s <= skill_mother[count-1] )
	{
		//011012 lsw
		if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[ITEM_IDENTIFICATION].skillexp >= ( unsigned int )skill[s].skillable )
		{
			SkillInventory[0][a][b] = s;
			b++;
			if( b > 3 ) { b=0; a++; }
		}
		s++;
	}

	if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nSHideNomalCount=2;	// 배울게 많으면
	else SMenu[MN_SKILLGUILD_KNOWLAGE].nField[2].nSHideNomalCount=1;		

//	s=skill_mother[IDENTIFICATION][0]; a=0; b=0;
//	while( s < skill_mother[IDENTIFICATION][1] )
//	{
//		if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[IDENTIFICATION] >= ( unsigned int )skill[s].skillable) 
//		{
//			SkillInventory[1][a][b] = s;
//			b++;
//			if( b > 3 ) { b=0; a++; }
//		}
//		s++;
//	}
}

void GetLearnSkillInv( int type, int start )
{
	int a, b, s, ct;
	memset( SkillInventory, 0, sizeof( int[2][2][4] ) );
	int skill_mother[MAX_SKILL_OF_KIND];
	int count;
	motherSkillImageType[0] = 0;
	motherSkillImageType[1] = 0;

	switch( type )
	{
		case	MN_BLACKSMITH :	motherSkillImageNo[0] = 623;	// MINING
								motherSkillImageNo[1] = 630;	// BLACKSMITHY
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=2;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;	// other button

								count = GetSkillMother( MINING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];

									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[MINING].skillexp>= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}

								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;		

								count = GetSkillMother( BLACKSMITHY, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[BLACKSMITHY].skillexp>= ( unsigned int )skill[s].skillable) 
									{
										SkillInventory[1][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								break;
		case	MN_FARM	:		motherSkillImageNo[0] = 0;		// FARMING
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( FARMING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[FARMING].skillexp >= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;
								

		case	MN_RESTAURANT:	motherSkillImageNo[0] = 631;	// COOKING
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( COOKING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[COOKING].skillexp>= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;

		case	MN_HERBSHOP:	motherSkillImageNo[0] = 627;	// HERBDIGGING
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( HERBDIGGING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[HERBDIGGING].skillexp >= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;

		case	MN_SHAMBLES:	motherSkillImageNo[0] = 626;	// BUTCHERING
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( BUTCHERING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[BUTCHERING].skillexp>= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;

		case	MN_CANDLE	:	motherSkillImageNo[0] = 635;	// CANDLEMAKING
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( CANDLEMAKING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[CANDLEMAKING].skillexp >= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;

		case	MN_SLOPSHOP:	motherSkillImageNo[0] = 632;	// TAILORING
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( TAILORING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[TAILORING].skillexp>= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;

		case	MN_ALCHEMY	:	motherSkillImageNo[0] = 634;	// ALCHEMY
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( ALCHEMING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[ALCHEMING].skillexp >= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;

		case	MN_WOODWORKING:	motherSkillImageNo[0] = 629;	// CARPENTRY
								motherSkillImageNo[1] = 625;	// CUTDOWN
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=2;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;	// other button

								count = GetSkillMother( CARPENTRY, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[CARPENTRY].skillexp>= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}

								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;		

								count = GetSkillMother( CUTDOWN, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[CUTDOWN].skillexp >= ( unsigned int )skill[s].skillable ) 
									{
										SkillInventory[1][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								break;

		case	MN_BOWCRAFT	:	motherSkillImageNo[0] = 633;	// BOWCRAFTING
								motherSkillImageNo[1] = 625;	// CUTDOWN
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=2;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;	// other button

								count = GetSkillMother( BOWCRAFTING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[BOWCRAFTING].skillexp>= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}

								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;		

								count = GetSkillMother( CUTDOWN, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw 
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[CUTDOWN].skillexp >= ( unsigned int )skill[s].skillable ) 
									{
										SkillInventory[1][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								break;

		case	MN_SHIPYARD:	motherSkillImageNo[0] = 624;	// FISHING
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( FISHING, skill_mother, MAX_SKILL_OF_KIND );
								ct=0; a=0; b=0;							
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[FISHING].skillexp>= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}
								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;

		case	MN_ACADEMY:		motherSkillImageNo[0] = 5;	// TAMING 
								motherSkillImageType[0] = SMALL_ETC;
								
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalCount=1;
								SMenu[MN_SKILLGUILD_SKILL].nField[6].nSHideNomalStart=start;
								SMenu[MN_SKILLGUILD_SKILL].nField[5].nType=FT_NOMAL_PUT;	// other button

								count = GetSkillMother( TAIMING, skill_mother, MAX_SKILL_OF_KIND );
								skill_mother[count++] = 78;	// ANIMAL_LORE
								ct=0; a=0; b=0;
								while(  count>0 && ct <= count )
								{
									s = skill_mother[ct];
									if( !skill[s].num ) { ct++; continue; }
									//011012 lsw
									if( !SCharacterData.SkillId[s] && SCharacterData.nJobSkill[TAIMING].skillexp>= ( unsigned int )skill[s].skillable )
									{
										SkillInventory[0][a][b] = s;
										b++;
										if( b > 3 ) { b=0; a++; }
									}
									ct++;
								}

								if( SkillInventory[0][1][0] ) SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=2;	// 배울게 많으면
								else SMenu[MN_SKILLGUILD_SKILL].nField[2].nSHideNomalCount=1;
								break;
	}
}


void ItemSound( int item )		// 아이템에 따른 소리
{

	int type = GetItemAttr( item, ITEM_KIND );
	switch( type )
	{
		case	IK_DRINK		:	MP3(SN_DRINK_BEER ); break;
		case	IK_POTION		:	MP3( SN_EAT_POTION ); break;

		case	IK_HERB			:
		case	IK_HERB_POWDER	:
		case	IK_SEED			:
		case	IK_GRAIN		:	
		case	IK_FOOD_MASS	:	
		case	IK_PLATE_FOOD	:	MP3( SN_EAT_BREAD ); break;

		case	IK_MONEY		:	
		case	IK_NEW_MONEY	:	break;
		default					:	break;
	}
}


void DropItemSound( int item )		// 아이템 땅에 떨어트리는 소리
{
	int type = GetItemAttr( item, ITEM_KIND );
	switch( type )
	{
		case	IK_NEW_MONEY	:	
		case	IK_MONEY		:	MP3( SN_PUT_MONEY ); break;
		default :					MP3( SN_DROP_ITEM ); break;
	}
}



void ChangeItem( int start )			// 단축키를 이용하여 아이템을 바꿔준다.
{
	memset( InvItemAttr, 0, sizeof( ItemAttr )*3*3*8 );
	ItemAttr *item_array;

	int max = Num_Of_CItem_Plant+Num_Of_CItem_Mineral+Num_Of_CItem_Herb+Num_Of_CItem_Cook+Num_Of_CItem_Potion+Num_Of_CItem_Tool
			+Num_Of_CItem_Weapon+Num_Of_CItem_Disposable+Num_Of_CItem_Armor+Num_Of_CItem_Accessory+Num_Of_CItem_Etc;

	int inv_total = 3*3*8;
	start *= inv_total;
	item_array = new ItemAttr[max];

	ZeroMemory( item_array, sizeof( ItemAttr)*max );

	int ct=0, i;

	for( i=1; i<Num_Of_CItem_Plant		; i++ ) { item_array[ct].item_no=i+PLANT		*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Mineral	; i++ ) { item_array[ct].item_no=i+MINERAL	*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Herb		; i++ ) { item_array[ct].item_no=i+HERB		*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Cook		; i++ ) { item_array[ct].item_no=i+COOK		*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Potion		; i++ ) { item_array[ct].item_no=i+POTION	*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Tool		; i++ ) { item_array[ct].item_no=i+TOOL		*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Weapon		; i++ ) { item_array[ct].item_no=i+WEAPON	*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Disposable	; i++ ) { item_array[ct].item_no=i+DISPOSABLE*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Armor		; i++ ) { item_array[ct].item_no=i+ARMOR		*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Accessory	; i++ ) { item_array[ct].item_no=i+ACCESSORY	*1000;  ct++; }
	for( i=1; i<Num_Of_CItem_Etc		; i++ ) { item_array[ct].item_no=i+ETC		*1000;  ct++; }


	int count=0;
	for( i=start; i<start+ inv_total ; i++)
	{
		ct = i % max;
		if( item_array[ct].item_no < 0 ) break;
		if( GetItemAttr( item_array[ct].item_no, IMAGE_NUMBER ) )
		{
			*((ItemAttr *)InvItemAttr+count) = item_array[ct];
		}
		count++;
	}

	SAFE_DELETE_ARRAY(item_array);
}


void MagicSetting2()		// 자동으로 메모라이즈 한다.
{

	memset( aMagicItem, 0, sizeof( int[3][4][3][5] ) );
	for( int a=MAX_MAGIC_-1; a>=1; a-- )
	{
		if( SCharacterData.MagicId[a] )
		{
			const int iNo = magic[a].GetMagicNum();
			if( iNo )
			{
				magic[iNo ].SetArray2();
				InsertMagicQuickArray( iNo );
			}
		}
	}
}
void CMagic::SetArray2()
{
	int exist = false;

	int first = (m_data[MagicClass]-1) / 4;
	int second = (m_data[MagicClass]-1) % 4;
	int third=0;  

	for( int b=0; b<15; b++)
	{
		if( n_MagicBagic[SCharacterData.nCharacterData[SPELL]][m_data[MagicClass]-1][b] == GetMagicNum() ) 
		{
			for( int z=0; z<15; z++ )
				if( !aMagicItem[first][second][z/5][z%5] )
				{
					third = z;
					exist = true;
					break;
				}
			break;
		}
	}

	if( !exist ) return;

	int end = third % 5;
	third /= 5;

	aMagicItem[first][second][third][end] = GetMagicNum();
}


void MagicSetting()		// 자동 메모라이즈를 하지 않는다.
{
	memset( aMagicItem, 0, sizeof( int[3][4][3][5] ) );
	for( int a=0; a<MAX_MAGIC_; a++ )
	{
		if( SCharacterData.curr_magic[a] )
		{
			magic[SCharacterData.curr_magic[a]].SetArray();
		}
		else break;
	}
}

void CMagic::SetArray()
{
	int e = false;

	int first = (m_data[MagicClass]-1) / 4;
	int second = (m_data[MagicClass]-1) % 4;
	int third=0;  

	for( int b=0; b<3; b++)
	{
		for( int c=0; c<5; c++ )
		{
			if( !aMagicItem[first][second][b][c] ) 
			{
				aMagicItem[first][second][b][c] = GetMagicNum();
				e = true;
				break;
			}
		}
		if( e ) break;
	}
}



/////////////////////////////////////////////////////////////////////////////////
// 
//		아이템을 먹을 때의 캐릭터 능력치 변화시키는 아이템 버철 함수 
//
/////////////////////////////////////////////////////////////////////////////////
int CItem_Plant::EatItem()
{
	SCharacter &ch = SCharacterData;
	if( IsHeroDead() ) return 0;

	ch.nCharacterSP += ap;
	if( ch.nCharacterSP > ch.nCharacterMAXSP ) ch.nCharacterSP = ch.nCharacterMAXSP;

	int *temp = &cure_disease1;
	int i;
	for( i=0; i<6; i++ )
	{
		if( *(temp+i) && ch.disease[i] )
		{
			ch.disease[i] = 0;
		}
	}

	temp = &take_disease1;
	for( i=0; i<6; i++ )
	{
		if( *(temp+i) && !ch.disease[i] )
		{
			// 확률상 질병에 걸릴 것인가를 체크하는 함수 루틴 삽입
			// ch.disease[i] = TakeDisease( disease_type, char_meet, char_talk, char_sex, ob_touch, ob_eat );
			// ch.disease_no_time = 0;
		}
	}

	return 1;
}
int CItem_Herb::EatItem()
{
	SCharacter &ch = SCharacterData;
	if( IsHeroDead() ) return 0;
	
	ch.nCharacterSP += ap;
	if( ch.nCharacterSP > ch.nCharacterMAXSP ) ch.nCharacterSP = ch.nCharacterMAXSP;

	int *temp = &cure_disease1;
	int i;
	for( i=0; i<6; i++ )
	{
		if( *(temp+i) && ch.disease[i] )
		{
			ch.disease[i] = 0;
//			ch.disease_no_time[i] = 10;   10분을 넣어 준다. 
		}
	}

	temp = &take_disease1;
	for( i=0; i<6; i++ )
	{
		if( *(temp+i) && !ch.disease[i] )
		{
			// 확률상 질병에 걸릴 것인가를 체크하는 함수 루틴 삽입
			// ch.disease[i] = TakeDisease( disease_type, char_meet, char_talk, char_sex, ob_touch, ob_eat );
			// ch.disease_no_time = 0;
		}
	}

	if( ch.nAbilityPlusOfPotion[STR] < Change_str )
	{
		ch.nAbilityPlusOfPotion[STR] = Change_str;
	}
	// ch.nAbilityPluseTime[STR] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[DEX] < Change_dex )
	{
		ch.nAbilityPlusOfPotion[DEX] = Change_dex;
	}
	// ch.nAbilityPluseTime[DEX] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[CON] < Change_con )
	{
		ch.nAbilityPlusOfPotion[CON] = Change_con;
	}
	// ch.nAbilityPluseTime[con] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[WIS] < Change_wis )
	{
		ch.nAbilityPlusOfPotion[WIS] = Change_wis;
	}
	// ch.nAbilityPluseTime[WIS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[INT_] < Change_int )
	{
		ch.nAbilityPlusOfPotion[INT_] = Change_int;
	}
	// ch.nAbilityPluseTime[INT_] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[CHA] < Change_cha )
	{
		ch.nAbilityPlusOfPotion[CHA] = Change_cha;
	}
	// ch.nAbilityPluseTime[CHA] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[ENDU] < Change_end )
	{
		ch.nAbilityPlusOfPotion[ENDU] = Change_end;
	}
	// ch.nAbilityPluseTime[ENDU] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[MOVP] < Change_Movp )
	{
		ch.nAbilityPlusOfPotion[MOVP] = Change_Movp;
	}
	// ch.nAbilityPluseTime[MOVP] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[MOR] < Change_mor )
	{
		ch.nAbilityPlusOfPotion[MOR] = Change_mor;
	}
	// ch.nAbilityPluseTime[MOR] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[LUCK] < Change_luc )
	{
		ch.nAbilityPlusOfPotion[LUCK] = Change_luc;
	}
	// ch.nAbilityPluseTime[LUCK] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nCharacterData[SPELL] )
	{
		if( ch.nAbilityPlusOfPotion[WSPS] < Change_PS )
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_PS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.
	}
	else
	{
		if( ch.nAbilityPlusOfPotion[WSPS] < Change_WS )
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_WS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.
	}

	return 1;
}

int CItem_Cook::EatItem()
{
	SCharacter &ch = SCharacterData;
	if( IsHeroDead() ) return 0;

	ch.nCharacterSP += ap;
	if( ch.nCharacterSP > ch.nCharacterMAXSP ) ch.nCharacterSP = ch.nCharacterMAXSP;


	int *temp = &cure_disease1;
	int i;
	for( i=0; i<6; i++ )
	{
		if( *(temp+i) && ch.disease[i] )
		{
			ch.disease[i] = 0;
//			ch.disease_no_time[i] = 10;   10분을 넣어 준다. 
		}
	}

	temp = &take_disease1;
	for( i=0; i<6; i++ )
	{
		if( *(temp+i) && !ch.disease[i] )
		{
			// 확률상 질병에 걸릴 것인가를 체크하는 함수 루틴 삽입
			// ch.disease[i] = TakeDisease( disease_type, char_meet, char_talk, char_sex, ob_touch, ob_eat );
			// ch.disease_no_time = 0;
		}
	}

	if( ch.nAbilityPlusOfPotion[STR] < Change_str )
	{
		ch.nAbilityPlusOfPotion[STR] = Change_str;
	}
	// ch.nAbilityPluseTime[STR] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[DEX] < Change_dex )
	{
		ch.nAbilityPlusOfPotion[DEX] = Change_dex;
	}
	// ch.nAbilityPluseTime[DEX] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[CON] < Change_con )
	{
		ch.nAbilityPlusOfPotion[CON] = Change_con;
	}
	// ch.nAbilityPluseTime[con] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[WIS] < Change_wis )
	{
		ch.nAbilityPlusOfPotion[WIS] = Change_wis;
	}
	// ch.nAbilityPluseTime[WIS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[INT_] < Change_int )
	{
		ch.nAbilityPlusOfPotion[INT_] = Change_int;
	}
	// ch.nAbilityPluseTime[INT_] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[CHA] < Change_cha )
	{
		ch.nAbilityPlusOfPotion[CHA] = Change_cha;
	}
	// ch.nAbilityPluseTime[CHA] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[ENDU] < Change_end )
	{
		ch.nAbilityPlusOfPotion[ENDU] = Change_end;
	}
	// ch.nAbilityPluseTime[ENDU] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[MOVP] < Change_Movp )
	{
		ch.nAbilityPlusOfPotion[MOVP] = Change_Movp;
	}
	// ch.nAbilityPluseTime[MOVP] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[MOR] < Change_mor )
	{
		ch.nAbilityPlusOfPotion[MOR] = Change_mor;
	}
	// ch.nAbilityPluseTime[MOR] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[LUCK] < Change_luc )
	{
		ch.nAbilityPlusOfPotion[LUCK] = Change_luc;
	}
	// ch.nAbilityPluseTime[LUCK] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nCharacterData[SPELL] )
	{
		if( ch.nAbilityPlusOfPotion[WSPS] < Change_Ps )
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_Ps;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.
	}
	else
	{
		if( ch.nAbilityPlusOfPotion[WSPS] < Change_WS )
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_WS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.
	}
	return 1;
}
int CItem_Potion::EatItem()
{
	SCharacter &ch = SCharacterData;
	if( IsHeroDead() ) return 0;

	ch.nCharacterSP += ap;
	if( ch.nCharacterSP > ch.nCharacterMAXSP ) ch.nCharacterSP = ch.nCharacterMAXSP;


	int *temp = &cure_disease1;
	int i;
	for( i=0; i<6; i++ )
	{
		if( *(temp+i) && ch.disease[i] )
		{
			ch.disease[i] = 0;
//			ch.disease_no_time[i] = 10;   10분을 넣어 준다. 
		}
	}

	temp = &take_disease1;
	for( i=0; i<6; i++ )
	{
		if( *(temp+i) && !ch.disease[i] )
		{
			// 확률상 질병에 걸릴 것인가를 체크하는 함수 루틴 삽입
			// ch.disease[i] = TakeDisease( disease_type, char_meet, char_talk, char_sex, ob_touch, ob_eat );
			// ch.disease_no_time = 0;
		}
	}

	if( ch.nAbilityPlusOfPotion[STR] < Change_str )
	{
		ch.nAbilityPlusOfPotion[STR] = Change_str;
	}
	// ch.nAbilityPluseTime[STR] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[DEX] < Change_dex )
	{
		ch.nAbilityPlusOfPotion[DEX] = Change_dex;
	}
	// ch.nAbilityPluseTime[DEX] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[CON] < Change_con )
	{
		ch.nAbilityPlusOfPotion[CON] = Change_con;
	}
	// ch.nAbilityPluseTime[con] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[WIS] < Change_wis )
	{
		ch.nAbilityPlusOfPotion[WIS] = Change_wis;
	}
	// ch.nAbilityPluseTime[WIS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[INT_] < Change_int )
	{
		ch.nAbilityPlusOfPotion[INT_] = Change_int;
	}
	// ch.nAbilityPluseTime[INT_] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[CHA] < Change_cha )
	{
		ch.nAbilityPlusOfPotion[CHA] = Change_cha;
	}
	// ch.nAbilityPluseTime[CHA] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[ENDU] < Change_end )
	{
		ch.nAbilityPlusOfPotion[ENDU] = Change_end;
	}
	// ch.nAbilityPluseTime[ENDU] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[MOVP] < Change_movp )
	{
		ch.nAbilityPlusOfPotion[MOVP] = Change_movp;
	}
	// ch.nAbilityPluseTime[MOVP] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[MOR] < Change_mor )
	{
		ch.nAbilityPlusOfPotion[MOR] = Change_mor;
	}
	// ch.nAbilityPluseTime[MOR] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nAbilityPlusOfPotion[LUCK] < Change_luc )
	{
		ch.nAbilityPlusOfPotion[LUCK] = Change_luc;
	}
	// ch.nAbilityPluseTime[LUCK] = 10; 시간도 체크하여 긴 시간으로 바꾼다.

	if( ch.nCharacterData[SPELL] )
	{
		if( ch.nAbilityPlusOfPotion[WSPS] < Change_PS )
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_PS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.
	}
	else
	{
		if( ch.nAbilityPlusOfPotion[WSPS] < Change_WS )
		{
			ch.nAbilityPlusOfPotion[WSPS] = Change_WS;
		}
		// ch.nAbilityPluseTime[WSPS] = 10; 시간도 체크하여 긴 시간으로 바꾼다.
	}
	
/*	switch( ch.condition )
	{
		case CC_NORMAL	:	break;
		case CC_POISON	:	if( Cure_poison ) ch.condition = CC_NORMAL; break;
		case CC_STONE	:	if( Cure_stone_paralysis ) ch.condition = CC_NORMAL; break;
		case CC_MAD		:	if( Cure_Mad ) ch.condition = CC_NORMAL; break;
		case CC_SLUG	:	if( Cure_sluggish ) ch.condition = CC_NORMAL; break;
		case CC_ACTIVE	:	if( Cure_Active ) ch.condition = CC_NORMAL; break;
	}
*/
	if( ChangeNatural2 ) 
	{
		ch.condition = ChangeNatural2;
	}

	return 1;
}



/////////////////////////////////////////////////////////////////////////////////
//
// 아이템 장착에 따른 캐릭터 능력치 변화를 계산하는 멤버 함수
//
/////////////////////////////////////////////////////////////////////////////////
// 무기 장착
int CItem_Weapon::EquipItem()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] += Change_str;
	plus[DEX ] += Change_dex;
	plus[CON ] += Change_con;
	plus[WIS ] += Change_wis;
	plus[INT_ ] += Change_int;
	plus[CHA ] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR ] += Change_mor;
	plus[LUCK] += Change_luc;

	if( ch.nCharacterData[SPELL] )	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;

	ch.nCharacterMAXHP	+=	Change_Life_max;
	ch.nCharacterHP +=	Change_Life_max;

	if( ch.nCharacterData[SPELL] )
	{
		ch.nCharacterMAXMP += Change_Divine_Max;
		ch.nCharacterMP += Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP	+=	Change_mana_max;
		ch.nCharacterMP	+=	Change_mana_max;
	}

	ch.nCharacterMAXSP += Change_health_max;
	ch.nCharacterSP += Change_health_max;

	int *temp = &imunity_Cure_1;
	for( int i=0; i<6; i++ )
	{
		if( *(temp+i) && ch.disease[i] )
		{
			ch.disease[i] = 0;
			ch.disease_no_time[i] = 0;
		}
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////
//
// 초기 장착에 따른 능력치 계산
//
/////////////////////////////////////////////////////////////////////////////////
// 무기 장착
int CItem_Weapon::CurrEquip()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] += Change_str;
	plus[DEX ] += Change_dex;
	plus[CON ] += Change_con;
	plus[WIS ] += Change_wis;
	plus[INT_ ] += Change_int;
	plus[CHA ] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR ] += Change_mor;
	plus[LUCK] += Change_luc;

	if( ch.nCharacterData[SPELL] )	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;
	return 1;
}

//#####################################################
// 아머 장착
int CItem_Armor::EquipItem()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] += Change_str;
	plus[DEX ] += Change_dex;
	plus[CON ] += Change_con;
	plus[WIS ] += Change_wis;
	plus[INT_ ] += Change_int;
	plus[CHA ] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR ] += Change_mor;
	plus[LUCK] += Change_luc;

	if( ch.nCharacterData[SPELL] )	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;

	ch.nCharacterMAXHP	+=	Change_Life_max;
	ch.nCharacterHP +=	Change_Life_max;
	
	if( ch.nCharacterData[SPELL] )
	{
		ch.nCharacterMAXMP += Change_Divine_Max;
		ch.nCharacterMP += Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP	+=	Change_mana_max;
		ch.nCharacterMP	+=	Change_mana_max;
	}

	ch.nCharacterMAXSP += Change_health_max;
	ch.nCharacterSP += Change_health_max;

	int *temp = &imunity_Cure_1;
	for( int i=0; i<6; i++ )
	{
		if( *(temp+i) && ch.disease[i] )
		{
			ch.disease[i] = 0;
			ch.disease_no_time[i] = 0;
		}
	}
	return 1;	
}
//#####################################################
// 아머 현재 장착
int CItem_Armor::CurrEquip()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] += Change_str;
	plus[DEX ] += Change_dex;
	plus[CON ] += Change_con;
	plus[WIS ] += Change_wis;
	plus[INT_ ] += Change_int;
	plus[CHA ] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR ] += Change_mor;
	plus[LUCK] += Change_luc;

	if( ch.nCharacterData[SPELL] )	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;
	return 1;
}


//#####################################################
// 악세사리 장착
int CItem_Accessory::EquipItem()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] += Change_str;
	plus[DEX ] += Change_dex;
	plus[CON ] += Change_con;
	plus[WIS ] += Change_wis;
	plus[INT_ ] += Change_int;
	plus[CHA ] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR ] += Change_mor;
	plus[LUCK] += Change_luc;


	if( ch.nCharacterData[SPELL] )	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;

	ch.nCharacterMAXHP	+=	Change_Life_max;
	ch.nCharacterHP +=	Change_Life_max;
	
	if( ch.nCharacterData[SPELL] )
	{
		ch.nCharacterMAXMP += Change_Divine_Max;
		ch.nCharacterMP += Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP	+=	Change_mana_max;
		ch.nCharacterMP	+=	Change_mana_max;
	}

	ch.nCharacterMAXSP += Change_health_max;
	ch.nCharacterSP += Change_health_max;

	int *temp = &imunity_Cure_1;
	for( int i=0; i<6; i++ )
	{
		if( *(temp+i) && ch.disease[i] )
		{
			ch.disease[i] = 0;
			ch.disease_no_time[i] = 0;
		}
	}
	return 1;	
}

//#####################################################
int CItem_Accessory::CurrEquip()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] += Change_str;
	plus[DEX ] += Change_dex;
	plus[CON ] += Change_con;
	plus[WIS ] += Change_wis;
	plus[INT_ ] += Change_int;
	plus[CHA ] += Change_cha;
	plus[ENDU] += Change_end;
	plus[MOVP] += Change_Movp;
	plus[MOR ] += Change_mor;
	plus[LUCK] += Change_luc;


	if( ch.nCharacterData[SPELL] )	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;
	return 1;
}


//#####################################################
// ETC 장착
int CItem_Etc::EquipItem()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] += Change_str;
	plus[DEX ] += Change_dex;
	plus[CON ] += Change_con;
	plus[WIS ] += Change_wis;
	plus[INT_ ] += Change_int;
	plus[CHA ] += Change_cha;
	plus[ENDU] += Change_end;
//	plus[MOVP] += Change_Movp;
	plus[MOR ] += Change_mor;
	plus[LUCK] += Change_luc;


	if( ch.nCharacterData[SPELL] )	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;

	ch.nCharacterMAXHP	+=	Change_Life_max;
	ch.nCharacterHP +=	Change_Life_max;
	
	if( ch.nCharacterData[SPELL] )
	{
		ch.nCharacterMAXMP += Change_Divine_Max;
		ch.nCharacterMP += Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP	+=	Change_mana_max;
		ch.nCharacterMP	+=	Change_mana_max;
	}

	ch.nCharacterMAXSP += Change_health_max;
	ch.nCharacterSP += Change_health_max;

	int *temp = &imunity_Cure_1;
	for( int i=0; i<6; i++ )
	{
		if( *(temp+i) && ch.disease[i] )
		{
			ch.disease[i] = 0;
			ch.disease_no_time[i] = 0;
		}
	}
	return 1;	
}

///////////////////////////////////////////////////////////////////////
//   기타 아이템 현재 장착
int CItem_Etc::CurrEquip()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] += Change_str;
	plus[DEX ] += Change_dex;
	plus[CON ] += Change_con;
	plus[WIS ] += Change_wis;
	plus[INT_ ] += Change_int;
	plus[CHA ] += Change_cha;
	plus[ENDU] += Change_end;
//	plus[MOVP] += Change_Movp;
	plus[MOR ] += Change_mor;
	plus[LUCK] += Change_luc;

	if( ch.nCharacterData[SPELL] )	plus[WSPS] += Change_ps;
	else	plus[WSPS] += Change_ws;
	return 1;
}


/////////////////////////////////////////////////////////////////////////////////
// 아이템 해제시 캐릭터 능력치 감소 체크 
//
/////////////////////////////////////////////////////////////////////////////////
// 무기 해제
int CItem_Weapon::ReleaseItem()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] -= Change_str;
	plus[DEX ] -= Change_dex;
	plus[CON ] -= Change_con;
	plus[WIS ] -= Change_wis;
	plus[INT_ ] -= Change_int;
	plus[CHA ] -= Change_cha;
	plus[ENDU] -= Change_end;
	plus[MOVP] -= Change_Movp;
	plus[MOR ] -= Change_mor;
	plus[LUCK] -= Change_luc;

	if( ch.nCharacterData[SPELL] )	plus[WSPS] -= Change_ps;
	else	plus[WSPS] -= Change_ws;

	ch.nCharacterMAXHP	-=	Change_Life_max;
	ch.nCharacterHP -=	Change_Life_max;
	
	if( ch.nCharacterData[SPELL] )
	{
		ch.nCharacterMAXMP -= Change_Divine_Max;
		ch.nCharacterMP -= Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP	-=	Change_mana_max;
		ch.nCharacterMP	-=	Change_mana_max;
	}

	ch.nCharacterMAXSP -= Change_health_max;
	ch.nCharacterSP -= Change_health_max;
	return 1;
}

//#####################################################
// 갑옷 방패 해제
int CItem_Armor::ReleaseItem()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] -= Change_str;
	plus[DEX ] -= Change_dex;
	plus[CON ] -= Change_con;
	plus[WIS ] -= Change_wis;
	plus[INT_ ] -= Change_int;
	plus[CHA ] -= Change_cha;
	plus[ENDU] -= Change_end;
	plus[MOVP] -= Change_Movp;
	plus[MOR ] -= Change_mor;
	plus[LUCK] -= Change_luc;

	if( ch.nCharacterData[SPELL] )	plus[WSPS] -= Change_ps;
	else	plus[WSPS] -= Change_ws;

	ch.nCharacterMAXHP	-=	Change_Life_max;
	ch.nCharacterHP -=	Change_Life_max;
	
	if( ch.nCharacterData[SPELL] )
	{
		ch.nCharacterMAXMP -= Change_Divine_Max;
		ch.nCharacterMP -= Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP	-=	Change_mana_max;
		ch.nCharacterMP	-=	Change_mana_max;
	}

	ch.nCharacterMAXSP -= Change_health_max;
	ch.nCharacterSP -= Change_health_max;
	return 1;	
}
//#####################################################
// 악세사리 해제
int CItem_Accessory::ReleaseItem()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] -= Change_str;
	plus[DEX ] -= Change_dex;
	plus[CON ] -= Change_con;
	plus[WIS ] -= Change_wis;
	plus[INT_ ] -= Change_int;
	plus[CHA ] -= Change_cha;
	plus[ENDU] -= Change_end;
	plus[MOVP] -= Change_Movp;
	plus[MOR ] -= Change_mor;
	plus[LUCK] -= Change_luc;


	if( ch.nCharacterData[SPELL] )	plus[WSPS] -= Change_ps;
	else	plus[WSPS] -= Change_ws;

	ch.nCharacterMAXHP	-=	Change_Life_max;
	ch.nCharacterHP -=	Change_Life_max;
	
	if( ch.nCharacterData[SPELL] )
	{
		ch.nCharacterMAXMP -= Change_Divine_Max;
		ch.nCharacterMP -= Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP	-=	Change_mana_max;
		ch.nCharacterMP	-=	Change_mana_max;
	}

	ch.nCharacterMAXSP -= Change_health_max;
	ch.nCharacterSP -= Change_health_max;
	return 1;	
}

//#####################################################
// ETC 해제
int CItem_Etc::ReleaseItem()
{
	SCharacter &ch = SCharacterData;
	int *plus = ch.nAbilityPlusOfWeapon;

	plus[STR ] -= Change_str;
	plus[DEX ] -= Change_dex;
	plus[CON ] -= Change_con;
	plus[WIS ] -= Change_wis;
	plus[INT_ ] -= Change_int;
	plus[CHA ] -= Change_cha;
	plus[ENDU] -= Change_end;
//	plus[MOVP] -= Change_Movp;
	plus[MOR ] -= Change_mor;
	plus[LUCK] -= Change_luc;


	if( ch.nCharacterData[SPELL] )	plus[WSPS] -= Change_ps;
	else	plus[WSPS] -= Change_ws;

	ch.nCharacterMAXHP	-=	Change_Life_max;
	ch.nCharacterHP -=	Change_Life_max;
	
	if( ch.nCharacterData[SPELL] )
	{
		ch.nCharacterMAXMP -= Change_Divine_Max;
		ch.nCharacterMP -= Change_Divine_Max;
	}
	else
	{
		ch.nCharacterMAXMP	-=	Change_mana_max;
		ch.nCharacterMP	-=	Change_mana_max;
	}

	ch.nCharacterMAXSP -= Change_health_max;
	ch.nCharacterSP -= Change_health_max;
	return 1;	
}











/////////////////////////////////////////////////////////////////////////
// 
//  아이템 버철 함수 콜하는 범용함수
/////////////////////////////////////////////////////////////////////////

int CallItemVirtualFunction( int function, int item_number )
{
	if( !item_number ) return 0;

	int type;
	int number;

	GetItemNumber(item_number, type, number);

	CItem *t = ItemUnit( type, number);
	if( !t ) return 0;
	switch( function )
	{
	case VF_EQUIP_ITEM :	return t->EquipItem();
	case VF_RELEASE_ITEM :	return t->ReleaseItem();
	case VF_CURR_EQUIP :	return t->CurrEquip();
	case VF_EAT_ITEM :		
		{
			int ret= t->EatItem(); 
			if( ret )
			{
				if( IsHeroDead() ) return 0;
				Hero->hp = SCharacterData.nCharacterHP;
				Hero->mp = SCharacterData.nCharacterMP;
			}
			return ret;
		}break;
	default : return 0;
	}

	return 1;
}


int ChangeItemAttr( ItemAttr *item, int type, int plus, int skill )
{
	int item_type;
	int index;

	GetItemNumber( item->item_no, item_type, index );

	switch( type )
	{     
		case CIA_DURATION : {
								WORD dur;
								WORD dur_max;
								dur = LOWORD( item->attr[IATTR_DURATION] );
								dur_max = HIWORD( item->attr[IATTR_DURATION] );
								
								dur += plus;

								if( dur > dur_max ) dur = dur_max;
								else if( dur < 1 ) return 0; // 아이템 파괴함수 콜...
								
								item->attr[IATTR_DURATION] = MAKELONG( dur, dur_max );
								break;
							}
		case CIA_VIEW_CHANGE : {
									item->item_no = plus;
									break;
							   }
		case CIA_MUCH : {
							;
							item->attr[IATTR_MUCH] += plus;
							CheckAutoDivideMoney( item );
							break;
						}
	}

	return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
//		결합 테이블 로드 해오기
//
//////////////////////////////////////////////////////////////////////////////////////////
void CItem_Join::ReadItemJoin( char *buf )
{
	char *token;
	int *target;
	token = strtok(buf,"\t\n\r");
	if( token == NULL ) return;
	no = atoi(token);

	token = strtok(NULL,"\t\n\r");
	if( token == NULL ) return;
	item_id = atoi(token);
	
	token= strtok(NULL, "\t\n");
	if( token == NULL ) return;
	strcpy(item_name, token);

	token = strtok(NULL,"\t\n\r");
	if( token == NULL ) return;
	item_amount = atoi(token);

	token= strtok(NULL, "\t\n");
	if( token == NULL ) return;
	strcpy(han_name, token);
	
	target = &need_know;
	int ct=0;
	while( ( token= strtok(NULL, "\t\n\r")) != NULL )
	{
		int t = atoi(token);
		if( !t && (token[0]<'0' || token[0]>'9') )
		{
			strcpy(explain, token);
		}
		else 
		{
			*(target+ct) = t;
		}
		ct++;	
	}
}

CItem_Join	item_join[ITEM_JOIN_MAX];
bool LoadItemJoinTable()
{
	char buf[1024] ;

	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	
	sprintf(filename, "./itemtable/ItemJoin.txt");
	sprintf(filenameBin, "./itemtable/ItemJoin.bin");

	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			item_join[i].ReadItemJoin(buf);
			item_join[i].item_id /= 100;
			item_join[i].resource1 /= 100;
			item_join[i].resource2 /= 100;
			item_join[i].resource3 /= 100;
			item_join[i].resource4 /= 100;
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			char *tt = (char *) item_join;
			char crc=0;
			for( int i=0; i<sizeof(CItem_Join)*ITEM_JOIN_MAX; i++ )
			{
				crc += *tt;
				tt++;
			}
			fwrite( &crc, 1, 1, fp );
			fwrite( item_join, sizeof(CItem_Join), ITEM_JOIN_MAX, fp);
			fwrite( &crc, 1, 1, fp );
			fclose(fp);
		}
	}

	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			char *tt = (char *) item_join;
			char crc=0, crc1, crc2;
			fread( &crc1, 1, 1, fp );
			fread( item_join, sizeof(CItem_Join), ITEM_JOIN_MAX, fp);
			fread( &crc2, 1, 1, fp );
			fclose(fp);

			for( int i=0; i<sizeof(CItem_Join)*ITEM_JOIN_MAX; i++ )
			{
				crc += *tt;
				tt++;
			}
			if( crc != crc1 || crc != crc2 ) 
			{
				CheckSumError = -1;
				JustMsg(lan->OutputMessage(6,84) );//CheckSumError = 1;//010215 lsw
			}
		}
		else return 0;
	}
	return 1;
}

//011030 lsw >
extern char* GetItemName(int nItemno);
inline void ReadItemSokSungKind(char * buf,int i)
{
	char *token;
	token = strtok(buf,"\t\n\r");	if( token == NULL ) return;	
	//011111 lsw >
	i = atoi(token);
	if(i > MAX_ITEM_MUTANT_KIND) {JustMsg("ItemSokSungKind Read Error"); exit(0); return;}
	ItemMutantKind[i].item_kind = atoi(token);	
	//011111 lsw <

	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;	strcpy(ItemMutantKind[i].exp_kind, token);
	EatRearWhiteChar( ItemMutantKind[i].exp_kind );
	
	token= strtok(NULL, "\t\n");if( token == NULL ) return;	ItemMutantKind[i].upgrade_type = atoi(token);
	int Count  =0;
	for(Count = 0 ;Count<8;Count++)
	{
	token= strtok(NULL, "\t\n");if( token == NULL ) return;	ItemMutantKind[i].AddItem[Count] = atoi(token);
	
	}

	token= strtok(NULL, "\t\n");if( token == NULL ) return;	ItemMutantKind[i].iBuffer1 = atoi(token);
	token= strtok(NULL, "\t\n");if( token == NULL ) return;	ItemMutantKind[i].iBuffer2 = atoi(token);
	token= strtok(NULL, "\t\n");if( token == NULL ) return;	ItemMutantKind[i].iBuffer3 = atoi(token);
	token= strtok(NULL, "\t\n");if( token == NULL ) return;	ItemMutantKind[i].iBuffer4 = atoi(token);
	return;
}

bool LoadItemSokSungKindTable()
{
#ifdef _DEBUG	// 031110 YGI
	char buf[1024] ;
#endif

	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	
	sprintf(filename, ".\\itemtable\\ItemSokSungKind.txt");
	sprintf(filenameBin, ".\\itemtable\\ItemSokSungKind.bin");

#ifdef _DEBUG	// 031110 YGI
#ifndef LIGHT_GM_CLIENT
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_ITEM_MUTANT_KIND-1 ) break;
			GetItemName(i);
				ReadItemSokSungKind(buf, i);
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( ItemMutantKind, sizeof(t_ItemMutantKind)*MAX_ITEM_MUTANT_KIND, 1, fp);
			fclose(fp);
		}
	
	 }
	else
#endif
#endif
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( ItemMutantKind, sizeof(t_ItemMutantKind)*MAX_ITEM_MUTANT_KIND, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

inline void ReadItemSokSungGrade(char * buf,int i)
{
	char *token;
	
/*
typedef struct ITEMGRADE
{
	int iGrade;
	int iAble;
	int ResourceAmount[4];
	int iBuffer1;
	int iBuffer2;
	int iBuffer3;
	int iBuffer4;
}t_ItemGrade,*LPITEM_GRADE;
const int MAX_ITEM_GRADE = 18;//MAX_ITEM_GRADE를 avail으로 체크한 값으로 받아 올수 있도록 코딩 할것
extern t_ItemGrade ItemGrade[MAX_ITEM_GRADE];
*/
	int *TargetPointer = &ItemGrade[i].iGrade;
	token=strtok(buf ,"\t\n\r");if( token == NULL ) return;	
	//011111 lsw >
	i = atoi(token);
	if(i > MAX_ITEM_GRADE) {JustMsg("ItemSokSungGrade Read Error"); exit(0); return;}
	//011111 lsw <
	(*TargetPointer) = atoi(token);
		 TargetPointer++;

	for (int j  = 0 ; j < 10 ; j++)//맥스 갯수에서 처음꺼 뺸 개수
	{
		 token=strtok(NULL ,"\t\n\r");if( token == NULL ) return;	
		 (*TargetPointer) = atoi(token);
		 TargetPointer++;
	}
	

	return;
}

bool LoadItemSokSungGradeTable()
{
	char buf[1024] ;
	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	sprintf(filename, ".\\itemtable\\ItemSokSungGrade.txt");
	sprintf(filenameBin, ".\\itemtable\\ItemSokSungGrade.bin");
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_ITEM_GRADE-1 ) break;
				ReadItemSokSungGrade(buf, i);
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )
		{
			fwrite( ItemGrade, sizeof(t_ItemGrade)*MAX_ITEM_GRADE, 1, fp);
			fclose(fp);
		}
	 }
	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( ItemGrade, sizeof(t_ItemGrade)*MAX_ITEM_GRADE, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

inline void ReadItemSokSung(char * buf,int i)
{
	char *token;
/*
typedef struct ITEMRARE
{
	int		iNo		;
	int		iType	;
	char	EngName[100];
	char	LocalName[100];
	char	Explain[100];
	char	ExplainAdd[100];
	int		iExpMark;// + - 기호 표시
	int		iMakeAble;
	int		RaisePro[8];// 0번은 DefaultRaisePro
	int		iUpgradeAble;
	int		iAbleNation;//
	int		GRADE[16];//0번은 MAX 나머지는 그레이드별 값
	int		Buffer1;
	int		Buffer2;
	int		Buffer3;
	int		Buffer4;
	int		Buffer5;
	int		Buffer6;
}t_ItemRare,*LPITEMRARE;
const int MAX_ITEM_RARE = 100;
extern t_ItemRare ItemRare[MAX_ITEM_RARE];
////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
	token = strtok(buf,"\t\n\r");	if( token == NULL ) return;	
	i = atoi(token);
	if(i > MAX_ITEM_RARE) {JustMsg("ItemSokSung Read Error"); exit(0); return;}
	ItemRare[i].iNo	 = i;
	token = strtok(NULL,"\t\n\r");	if( token == NULL ) return;	ItemRare[i].iType= atoi(token);
	token= strtok(NULL, "\t\n");if( token == NULL ) return;		strcpy(ItemRare[i].EngName, token);	EatRearWhiteChar( ItemRare[i].EngName );
	token= strtok(NULL, "\t\n");if( token == NULL ) return;		strcpy(ItemRare[i].LocalName, token);	EatRearWhiteChar( ItemRare[i].LocalName );
	token= strtok(NULL, "\t\n");if( token == NULL ) return;		strcpy(ItemRare[i].Explain, token);		EatRearWhiteChar( ItemRare[i].Explain );
	token= strtok(NULL, "\t\n");if( token == NULL ) return;		strcpy(ItemRare[i].ExplainAdd, token);	EatRearWhiteChar( ItemRare[i].ExplainAdd );
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].iExpMark= atoi(token);
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].iMakeAble= atoi(token);
	int Count  =0;
	for(Count = 0 ;Count<8;Count++)
	{
		token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].RaisePro[Count]= atoi(token);
	}
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].iUpgradeAble= atoi(token);
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].iAbleNation= atoi(token);
	for(Count = 0 ;Count<16;Count++)
	{
		token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].GRADE[Count]= atoi(token);
	}
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].Buffer1= atoi(token);
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].Buffer2= atoi(token);
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].Buffer3= atoi(token);
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].Buffer4= atoi(token);
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].Buffer5= atoi(token);
	token = strtok(NULL,"\t\n");if( token == NULL ) return;		ItemRare[i].Buffer6= atoi(token);
	return;
}

bool LoadItemSokSungTable()
{
#ifdef _DEBUG	// 031110 YGI
	char buf[1024] ;
#endif

	FILE *fp ;
	char	filename[MAX_FILENAME],filenameBin[MAX_FILENAME];
	
	sprintf(filename, ".\\itemtable\\ItemSokSung.txt");
	sprintf(filenameBin, ".\\itemtable\\ItemSokSung.bin");

#ifdef _DEBUG	//< BBD 031214	텍스트는 디버그모드만 읽자
#ifndef LIGHT_GM_CLIENT
	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		while( !feof(fp) )
		{
			fgets(buf, 1023, fp);
			if( *buf == ';' ) continue;
			if(i> MAX_ITEM_RARE-1 ) break;
				ReadItemSokSung(buf, i);
			i++;
		}
		fclose(fp);

		if(fp=Fopen( filenameBin, "wb" ) )	
		{
			fwrite( ItemRare, sizeof(t_ItemRare)*MAX_ITEM_RARE, 1, fp);
			fclose(fp);
		}
	
	 }
	else
#endif
#endif	//> BBD 031214	텍스트는 디버그모드만 읽자
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			fread( ItemRare, sizeof(t_ItemRare)*MAX_ITEM_RARE, 1, fp);
			fclose(fp);
		}
		else return 0;
	}
	return 1;
}

extern SkillMain SkillTable[ MAX_SKILL_];
void SkillUseItem( int skill , int menu )
{
	memset( skill_use_item, 0, sizeof( PosItem[5][10] ) );
	memset( &skill_use_item_result, 0, sizeof( PosItem ) );

	PutSkillUseItem_skill_use_item( skill );

	MainMenuClose( );
	CallMenu( MN_ITEM_SELECT, menu, true );
//	y_MenuFocus = MN_ITEM_SELECT;
}

//010708 lsw 만들수 있는 gm아템 목록
int iGM_Makeable_Item_List[ITEM_JOIN_MAX];
// 마더 스킬
const int iGM_2ND_Mother_Skill[7]			= {	SKILL_CARPENTRY,	SKILL_BLACKSMITHY,	SKILL_COOKING,	SKILL_TAILORING,	SKILL_BOWCRAFTING,	SKILL_MAKEPOTION,	SKILL_CANDLEMAKING};
const int iSkill_Master_Mother_Skill[4]		= {	SKILL_BLACKSMITHY,	SKILL_CARPENTRY, SKILL_BOWCRAFTING	,SKILL_TAILORING	};
int CanMakeItemListSet( int s, int menu )		// 만들수 있는 아이템 등록
{//스킬에 따른 인터페이스도 부름 //021111 lsw
	int iHowTo= SkillTable[ s ].Howto ;
	int count = 0;
	switch(iHowTo)
	{
	case SKILLTARGETTYPE_TABLE_:
		{
			memset(Item_Select, 0, sizeof( ItemJoinSelect[MAX_LEARN_SKILL_PAGE][10] ) );
			memset( &item_select_result, 0, sizeof( ItemJoinSelect ) );
			for( int i=0; i<ITEM_JOIN_MAX; i++)
			{
				if( item_join[i].item_id && GetItemAttr( item_join[i].item_id, IMAGE_NUMBER ) )
				{
					if(item_join[i].need_skill1 == s)
					{
						if( !FindLearnItem( item_join[i].item_id ) ) continue;

						Item_Select[count/10][count%10].item_no = item_join[i].item_id;
						Item_Select[count/10][count%10].skill_min =item_join[i].skill1_minimum;
						Item_Select[count/10][count%10].kind	=	ItemUnit(item_join[i].item_id)->GetItemKind();
						Item_Select[count/10][count%10].skillability	=	ItemUnit(item_join[i].item_id)->GetSkillAbility();
						count ++;
					}
				}
			}
			//정렬루틴 넣는곳..
			{
				int i, j ;
				int n = count;
				ItemJoinSelect t;
				ItemJoinSelect *a = &Item_Select[0][0];

				for (i = 1; i < n;i++)
				{
					t = a[i];
					j = i;
					while(a[j-1].item_no > t.item_no && j > 0)//삽입될 곳 찾음
					{
						a[j] = a[j-1];//뒤로 옮김
						j--;
					}
					a[j] = t;//삽입
				}


				for (i = 1; i < n;i++)
				{
					t = a[i];
					j = i;
					while(a[j-1].skillability > t.skillability && j > 0 && a[j-1].item_no/1000 == t.item_no/1000)//삽입될 곳 찾음
					{
						a[j] = a[j-1];//뒤로 옮김
						j--;
					}
					a[j] = t;//삽입
				}

				for (i = 1; i < n;i++)
				{
					t = a[i];
					j = i;
					while(a[j-1].kind > t.kind && j > 0 && a[j-1].kind == t.kind )//삽입될 곳 찾음
					{
						a[j] = a[j-1];//뒤로 옮김
						j--;
					}
					a[j] = t;//삽입
				}
				
				for (i = 1; i < n;i++)
				{
					t = a[i];
					j = i;
					while(a[j-1].skill_min > t.skill_min && j > 0 && a[j-1].kind == t.kind )//삽입될 곳 찾음
					{
						a[j] = a[j-1];//뒤로 옮김
						j--;
					}
					a[j] = t;//삽입
				}

			//	이걸 ㅡㅡ;;
				//배울때는 배웠나 안배웠나 나오고..
				//스킬 Kind JOin 테이블
				//SKill_min
				//아이템 넘버
				//011111 lsw 12345 -->> 로 정렬
				//a^=b^=a^=b 하면 된다 ^^
				//item Type 받아 정렬 해 주고  /1000
				//카인드로 정렬
				//타입별로 SKil_min으로 정렬   /
				//만약 SKil_min 이 같으면 아이템 넘버로 정렬
				
			}
			if( count )	
			{
			MainMenuClose( );
			//011025 lsw >
				if(  timeGetTime()+1000  > g_skillmaketime )
				{
					sprintf(SMenu[MN_ITEM_MAKE_SELECT].nField[10].temp,"0");
					CallMenu( MN_ITEM_MAKE_SELECT, menu, true );//011012 lsw
					SMenu[MN_ITEM_MAKE_SELECT].nField[2].nSHideNomalCount=count/10+1;
					g_skillmaketime = 0;
				}
				else
				{
					SkillStatus				= SKILL_OFF;
					nItemOther = 0 ;
					SkillSettingClear( );
				//011110 lsw 
				//	char msg[300];
				//	sprintf( msg,lan->OutputMessage(2,256),(g_skillmaketime - timeGetTime())/1000);
					//CallOkCancelMessageBox(FALSE,0,0,msg,0);
					AddCurrentStatusMessage(FONT_COLOR_YELLOW,lan->OutputMessage(2,256),(g_skillmaketime - timeGetTime())/1000);
			
				}
				//011025 lsw <
			}
			else 
			{
				MP3( SN_WARNING );
				CallOkCancelMessageBox( menu, 0,0, lan->OutputMessage(6,85), 0 );//010215 lsw
				SkillSettingClear( );
			}
			return count;
		}break;
	case	SKILLTARGETTYPE_MAP_WITH_ITEM_:
		{
			SkillUseItem( s, menu );
			return 0;
		}break;
	case	SKILLTARGETTYPE_2ND_GM://2ND 스킬 지엠//변수 명 다 바꿀 것
		{
			count = 0;
			memset(iGM_Makeable_Item_List, 0,4*ITEM_JOIN_MAX);// 초기화 40 개 맞나? 
			if( 106 > s|| 112 < s) return 0 ;//106인 이유는 gm스킬 시작점이 106 이니까 ㅡ.ㅡ
			int convert_skill = iGM_2ND_Mother_Skill[s-106];
			for( int i=0; i<ITEM_JOIN_MAX; i++)
			{
				if( item_join[i].item_id && GetItemAttr( item_join[i].item_id, IMAGE_NUMBER ) )
				{
					if(item_join[i].need_skill1 == convert_skill)//스킬별 매칭이 있어야 한다.)
					{
						if( !FindLearnItem( item_join[i].item_id ) ) continue;
						if( count > ITEM_JOIN_MAX)//만드는 아이템이 500개 넘지 못한다.
						{
							MP3( SN_WARNING );
							CallOkCancelMessageBox( menu, 0,0, lan->OutputMessage(4,106), 0 );//실패
							SkillSettingClear( );
						}
						iGM_Makeable_Item_List[count] = (item_join[i].item_id);//만들수 있는 아템 리스트
						count++;
					}
				}
			}
			if( count )	
			{
				::MainMenuClose( );
				::Call_GM_2ND_Skill_Menu();
			}
			else
			{
				CallOkCancelMessageBox( menu, 0,0, lan->OutputMessage(4,107), 0 );//실패
			}
		}break;
	case	SKILLTARGETTYPE_SKILL_MASTER:
		{
			
			count = 0;

			if(!::IsMerchant())return count;//soto-HK030928 상인이 아니면 기성도 아니다.

			memset(iGM_Makeable_Item_List, 0,4*ITEM_JOIN_MAX);// 초기화 40 개 맞나? 
			if( s >72 || s < 68 ) 
			{
				return 0 ;//skillmain 테이블 때문에
			}
			int convert_skill = iSkill_Master_Mother_Skill[s-68];
			for( int i=0; i<ITEM_JOIN_MAX; i++)
			{
				if( item_join[i].item_id && GetItemAttr( item_join[i].item_id, IMAGE_NUMBER ) )
				{
					if(item_join[i].need_skill1 == convert_skill)//스킬별 매칭이 있어야 한다.)
					{
						if( !FindLearnItem( item_join[i].item_id ) ) continue;
						if( count > ITEM_JOIN_MAX)//만드는 아이템이 500개 넘지 못한다.
						{
							MP3( SN_WARNING );
							CallOkCancelMessageBox( menu, 0,0, lan->OutputMessage(4,106), 0 );//실패
							SkillSettingClear( );
						}
						iGM_Makeable_Item_List[count] = (item_join[i].item_id);//만들수 있는 아템 리스트
						count++;
					}
				}
			}
			if( count )	
			{
				::MainMenuClose( );
				::Call_Skill_Master_Menu();
			}
			else
			{
				CallOkCancelMessageBox( menu, 0,0, lan->OutputMessage(4,107), 0 );//실패
			}
		}break;
	default:
		break;
	}
	return count;
}


void CalcGuildToSkill( )
{
	memset( inclusive_to_skill, 0, sizeof(int )*MAX_SKILL_KIND*8 );
	for( int i=0; i<MAX_SKILL; i++ )
	{
		int para = skill[i].inclusive;
		for( int j=0; j<8; j++ )
		{
			if( !inclusive_to_skill[para][j] )
			{
				inclusive_to_skill[para][j] = i;	// 스킬 번호 입력
				break;
			}
		}
	}
}



ItemAttr *GetItemByPos( POS item )
{
	switch( item.type )
	{
	case	HAND	:	return &HandItemAttr;
	case	INV		:	return &InvItemAttr[item.p1][item.p2][item.p3];
	case	EQUIP	:	return &EquipItemAttr[item.p3];
	case	QUICK	:	return &QuickItemAttr[item.p3];
	case	BANK	:	return &BankItemAttr[item.p1][item.p2][item.p3];
	case	MAKE	:	// 추가
	case	OTHER_CH:
	default:
		{
			return NULL;
		}break;
	}
	return NULL;
}



////////////////////////////////////////////////////////////////////////////////
//
//	아이템 번호와 타입이 허용된 겂을 넘었는지를 판다한하는 함수
//
//		파라메터는 타입과 번호를 입력 받고
//		참조자로 리턴한다.	잘 못 됐을 경우 NULL리턴
//
//
////////////////////////////////////////////////////////////////////////////////
CItem *ItemUnit( int type, int item_no )
{
//	if( !item_no ) return NULL;
	if( (type<0) || (type>10) ) goto ITEM_UNIT_NOT;
	if( (item_no<0) || (item_no >= Item_Ref.nItem[type]) ) goto ITEM_UNIT_NOT;

	return Item_Ref.Item_type[type][item_no];

ITEM_UNIT_NOT :
	//if( GetSysInfo( SI_GAME_MAKE_MODE ) ) _asm int 3;
	//JustMsg( "ItemUnit : %2d %3d", type, item_no );
	return NULL;
}

CItem *ItemUnit( ItemAttr item )
{
	int type;
	int num; 
	GetItemNumber( item.item_no, type, num );

	return ItemUnit( type , num );
}
CItem *ItemUnit( int item_no )
{
	return ItemUnit( item_no / 1000, item_no%1000 );
}

int DeleteItem( ItemAttr *item )
{
	int type, num;
	GetItemNumber( item->item_no, type, num );

	CItem *t = ItemUnit( type, num );
	if( !t ) return -1;

	int wt = WearAble[GetItemAttr( item->item_no, WEAR_ABLE)];
	memset( item, 0, sizeof( ItemAttr ) );
	if( wt ) ChangeEquip( Hero, EquipItemAttr, type );
	
	return 1;
}

int DeleteItem( POS pos )
{
	ItemAttr *t = ::GetItemByPos( pos );
	if( t )	return DeleteItem( t );
	else return 0;
}


///////////////////////////////////////////////////
// 배울 필요가 없는 아이템을 join 테이블에서 꺼내서 
// 배운 아이템에 넣어 둔다.
void SetLearnItem()
{
	for( int i=0; i<ITEM_JOIN_MAX; i++)
	{
		if( !item_join[i].need_know && item_join[i].item_id ) 
		{
			SCharacterData.ItemId[SCharacterData.ItemIdCount++] = item_join[i].item_id;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//		아이템 번호와 스킬 번호로 아이템 조인 테이블을 참조하여 
//		내가 배울수 있는 아이템인지 아닌지를 판단한다.
//////////////////////////////////////////////////////////////////////////////
bool CanLearnItem( int item, int price, int &join_no )
{
	for( int i=0; i<ITEM_JOIN_MAX; i++)
	{
		if( item_join[i].item_id==item && item_join[i].tech_price == menu_to_set[price] )
		{
			join_no = i;
			return true;
		}
	}
	return false;
}

bool CanLearnItemForExplain( int item )
{
	for( int i=0; i<ITEM_JOIN_MAX; i++)
	{
		if( item_join[i].item_id==item )
		{
			//011012 lsw
			if( (	 item_join[i].need_skill1? (SCharacterData.SkillId[item_join[i].need_skill1]&& SCharacterData.nJobSkill[skill[item_join[i].need_skill1].inclusive].skillexp/10000 >= ( unsigned int )item_join[i].skill1_minimum ):1 )
				&& ( item_join[i].need_skill2? (SCharacterData.SkillId[item_join[i].need_skill2]&& SCharacterData.nJobSkill[skill[item_join[i].need_skill2].inclusive].skillexp/10000 >= ( unsigned int )item_join[i].skill2_minimum ):1 )	)
			{
				return true;
			}
			else return false;
		}
	}
	return false;
}

bool CanLearnItemByNeedSkill( int join_no )
{
	//011012 lsw
	if( (	 item_join[join_no].need_skill1? (SCharacterData.SkillId[item_join[join_no].need_skill1]&& SCharacterData.nJobSkill[skill[item_join[join_no].need_skill1].inclusive].skillexp/10000 >= ( unsigned int )item_join[join_no].skill1_minimum ):1 )
		&& ( item_join[join_no].need_skill2? (SCharacterData.SkillId[item_join[join_no].need_skill2]&& SCharacterData.nJobSkill[skill[item_join[join_no].need_skill2].inclusive].skillexp/10000 >= ( unsigned int )item_join[join_no].skill2_minimum ):1 )	)
	{
		return true;
	}
	else return false;
}


int GetWeight( ItemAttr item )
{
	int weight = 0;
	int type, num;
	GetItemNumber( item.item_no, type, num );

	CItem *t = ItemUnit( type, num );
	if( !t ) return 1;
	weight = t->GetItemWeight();
	
	if( type == DISPOSABLE )		// 비검 등...
	{
		int dur = t->GetQuantity();
		if( dur > 0 ) weight *= item.attr[IATTR_MUCH] / dur;
		else weight *= item.attr[IATTR_MUCH];
	}
	else if( t->GetRButton() == DIVIDE_ITEM )
	{
		int dur = t->GetItemDuration();		// 단위..
		if( dur > 0 )
			weight *= item.attr[IATTR_MUCH] / dur;
		else weight *= item.attr[IATTR_MUCH];
	}
	return weight;
}



ItemAttr Item_Number2Attr( int item )
{
	ItemAttr temp = {0, };

	int type, num;
	GetItemNumber( item, type, num );

	CItem *t = ItemUnit( type, num );
	if( t ) 
	{
		WORD dur = GetItemAttr( item, DURATION );
		temp.item_no = item;
		temp.attr[IATTR_DURATION] = MAKELONG( dur, dur );
		int limit = GetItemAttr( item, ITEM_LIMIT );
		if( limit > 0 )
		{
			temp.attr[IATTR_LIMIT] = limit + g_curr_time;
		}
	}
	return temp;
}

void GetItemDuration( ItemAttr item, WORD &d_curr, WORD &d_max )
{
	d_curr = LOWORD(item.attr[IATTR_DURATION]);
	d_max = HIWORD(item.attr[IATTR_DURATION]);
}



void DeleteItemByItemNumber( int item )
{
	int i;
	for( i=0; i<3; i++ )		// inventory
		for( int j=0; j<3; j++ )
			for( int k=0; k<8; k++ )
			{
				if( item == InvItemAttr[i][j][k].item_no )
				{
					DeleteItem( &InvItemAttr[i][j][k] );
					return;
				}
			}
	for( i=0; i<8; i++ )			// equip inv
	{
		if( item == EquipItemAttr[i].item_no )
		{
			DeleteItem( &EquipItemAttr[i] );
			return;
		}
	}

	for( i=0; i<6; i++ )			// quick inv
	{
		if( item == QuickItemAttr[i].item_no )
		{
			DeleteItem( &QuickItemAttr[i] );
			return;
		}
	}
}

void CItem_Plant::CopyItemClass( CItem_Plant *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Mineral::CopyItemClass( CItem_Mineral *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Herb::CopyItemClass( CItem_Herb *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Cook::CopyItemClass( CItem_Cook *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Potion::CopyItemClass( CItem_Potion *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Tool::CopyItemClass( CItem_Tool *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Weapon::CopyItemClass( CItem_Weapon *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Disposable::CopyItemClass( CItem_Disposable *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Armor::CopyItemClass( CItem_Armor *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Accessory::CopyItemClass( CItem_Accessory *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}
void CItem_Etc::CopyItemClass( CItem_Etc *source, int ct )
{
	Item_id = source->Item_id;
	strcpy(Name, source->Name); 
	strcpy(Han_Name, source->Han_Name); 
	
	int *item_value= &lv ;
	int *source_item_value = &source->lv ;
	for(int i=0;i<ct-3;i++)
	{
		*item_value = *source_item_value;
		item_value++;
		source_item_value++;
	}
	CItem::CopyItemClass();
}


int GetItemEquipSound( int item_no )
{
	int kind = GetItemAttr( item_no, ITEM_KIND );
	switch( kind )
	{
		case IK_ROBE :
		case IK_LEATHER_ARMOR :
		case IK_NORMAL_CLOTHE :  return SN_CLOTHE;
	}
	
	return 0;
}

void EquipSound( int item_no )
{
	int type = GetItemAttr( item_no, WEAR_ABLE );
	int sound = 0;

	switch( type )
	{
		case WEAR_BELT			:	
		case WEAR_ANYTHING		:
		case WEAR_ONLY_INVEN	:	break;
		case WEAR_LEFT_HAND		:	MP3( SN_SHIELD); break;
		case WEAR_TWO_HAND		:
		case WEAR_RIGHT_HAND	:	MP3( SN_WEAPON	); break;
		case WEAR_HEAD			:	MP3( SN_HELMET ); break;
		case WEAR_BODY			:	sound = GetItemEquipSound( item_no );
									if( sound ) MP3( sound );
									else MP3( SN_ARMOR );
									break;
		case WEAR_NECK			:	MP3( SN_NECK ); break;
		case WEAR_SHOES			:	MP3( SN_SHOES ); break;
		case WEAR_UNIQUE		:	MP3( SN_UNIQ ); break;
	}
	return;
}


void FreeItemIcon()
{
	for( int i=0; i< ITEM_MAX; i++ )
	{
		for( int j=0; j<10; j++ )
		{
			if( Icon1[i][j].img ) MemFree( Icon1[i][j].img );
			if( Icon2[i][j].img ) MemFree( Icon2[i][j].img );
			if( Icon3[i][j].img ) MemFree( Icon3[i][j].img );
			if( Icon4[i][j].img ) MemFree( Icon4[i][j].img );
			if( Icon5[i][j].img ) MemFree( Icon5[i][j].img );
			if( Icon6[i][j].img ) MemFree( Icon6[i][j].img );
			if( Icon7[i][j].img ) MemFree( Icon7[i][j].img );
			if( Icon8[i][j].img ) MemFree( Icon8[i][j].img );
			if( Icon9[i][j].img ) MemFree( Icon9[i][j].img );
			if( Icon10[i][j].img ) MemFree( Icon10[i][j].img );
			if( Icon11[i][j].img ) MemFree( Icon11[i][j].img );
			if( Icon12[i][j].img ) MemFree( Icon12[i][j].img );
			if( Icon13[i][j].img ) MemFree( Icon13[i][j].img );
			
		}
	}
}


int GetTotalWeight( )
{
	int a, b, c;
	int weight = 0;
	for(a=0;a<3;a++)
		for(b=0;b<3;b++)
			for(c=0;c<8;c++)
				if(InvItemAttr[a][b][c].item_no) 
					weight += GetWeight( InvItemAttr[a][b][c] );
	for(a=0;a<6;a++)
		if(QuickItemAttr[a].item_no)	weight += GetWeight(QuickItemAttr[a]);
	for(a=0;a<8;a++)
		if(EquipItemAttr[a].item_no)	weight += GetWeight(EquipItemAttr[a]);
	if( HandItemAttr.item_no ) weight += GetWeight( HandItemAttr );
	return weight;
}

int CalMovpByWeight()
{
	int result=0;
	int max = Hero_Ability[STR] * 100;		// 들수 있는 무게 맥스
	if( SCharacterData.nWeight <= max ) return Hero_Ability[MOVP];
	
	if(  max )
	{
		float rate =   (SCharacterData.nWeight - max) / ((float)max) * 100.0f;
		int move_down = (int)((rate*2)/100.0f * Hero_Ability[MOVP]);
		result = Hero_Ability[MOVP] - move_down;
	}
	if( result < 0 ) result = 0;
	return result;
}

char g_MagicName[MAX_MAGIC_][31];
int LoadMagicName( )		// 마법 이름만 다 읽어 온다. // priest는 매직넘버에 +150을 해서 사용한다.
{
	CMagic temp_magic[MAX_MAGIC_];
	char filenameBin[MAX_FILENAME];
	FILE *fp;

	sprintf(filenameBin, "./magic/wizard.bin");
	
	for( int i=0; i<2; i++ )
	{
		fp = Fopen( filenameBin, "rb" );
		if(fp)
		{
			char crc = 0, crc1, crc2;
			fread( &crc1, 1, 1, fp );
			fread( temp_magic, sizeof(CMagic), MAX_MAGIC_, fp);
			fread( &crc2, 1, 1, fp );
			fclose(fp);

			if( i == 0 )
				for( int j=0; j<150; j++ ) strcpy( g_MagicName[j], temp_magic[j].GetName() ); 
			else 
				for( int j=0; j<50; j++ ) strcpy( g_MagicName[j+150], temp_magic[j].GetName() ); 

			sprintf(filenameBin, "./magic/priest.bin");
		}
		else return 0;
	}

	return 1;
}

int GetItemAttrWeapon( int item )		// 장착가능한 것중에 정말 장착이 가능한지 확인하는 것.
{
	int position = GetItemAttr(item, WEAR_ABLE );
	if( position== WEAR_ONLY_INVEN || position == WEAR_ANYTHING || position == WEAR_BELT || item/1000 == TOOL ) return 1;
	return GetItemAttr( item, EQUIPABLE );
}

////////////////////////////////////////////////////////
//
//	아이템 생성 함수
//
////////////////////////////////////////////////////////
ItemAttr GenerateItem( int item_no )	
{
	int type, num;
	GetItemNumber( item_no, type, num );
	ItemAttr item={0,};

	CItem *t = ItemUnit(type, num);
	if(t)
	{
		if( !t->GetImageNumber() ) return item ;
		item.item_no = item_no;
		

		if( type == DISPOSABLE )
			item.attr[IATTR_DURATION] = t->GetQuantity();
		else if( t->GetRButton() == DIVIDE_ITEM )
		{
			item.attr[IATTR_DURATION] = t->GetItemDuration();
		}
		else item.attr[IATTR_DURATION] = MAKELONG( t->GetItemDuration(), t->GetItemDuration() );

		//item.attr[IATTR_LIMIT] = t->GetLimit()*6*3600 + g_curr_time;
	}

	return item;
}


void ItemSoundOfGetItem( int item_no )
{
	int kind = GetItemAttr( item_no, ITEM_KIND );
	switch( kind )
	{
		case IK_NEW_MONEY :
		case IK_MONEY : MP3( SN_GET_MONEY ); break;
		default : MP3( SN_GET_ITEM ); break;
	}
}

void ItemSoundOfDropItem( int item_no )
{
	int kind = GetItemAttr( item_no, ITEM_KIND );
	switch( kind )
	{
		case IK_NEW_MONEY :
		case IK_MONEY : MP3( SN_PUT_MONEY ); break;
		default : MP3( SN_DROP_ITEM ); break;
	}
}

void CheckAbility()
{
	::memset( SCharacterData.nAbilityPlusOfWeapon, 0, sizeof( int )*11 );
	for( int i=0; i<8; i++ )		// 현재 입고 시작하는 아이템에 대한...
	{
		CallItemVirtualFunction( VF_CURR_EQUIP, EquipItemAttr[i].item_no );
	}
}

/////////////////////////////////////////////////////////////////
// 그랜드 마스터에 등록하기위해 사용하는 아이템 설명 테이블
CItemExplain g_GM_Item;
void CItemExplain::GetItemExplain( int n )
{
	char buf[1024] ;

	FILE *fp ;
	char	filename[MAX_FILENAME], filenameBin[MAX_FILENAME];

	int nation;
	if( n ) nation = n;
	else 
	{
		static int nation_num[] = { 1, 1, 1, 1, 2, 1, 3, 1 };
		nation = nation_num[SCharacterData.nation];		// 바이서스를 1로 자이펀을 2로 일스를 3으로 만들기 위해...
	}
	
	sprintf(filename, "./itemtable/GMItem%d.txt", nation);
	sprintf(filenameBin, "./itemtable/GMItem%d.bin", nation);

	if( fp= Fopen(filename, "rt") )
	{
		int i=0;
		int real_count = 0;
		while( fgets(buf, 2048, fp) != NULL )
		{
			real_count++;
			if( *buf == ';' || *buf == '#' ) continue;
			int ret = SetValue(i, buf);
			if( ret < 0 ) 
			{
				if(  GetSysInfo( SI_GAME_MAKE_MODE ) )
					JustMsg(lan->OutputMessage(6,86), filename,real_count );//010215 lsw
				continue;
			}
			i++;
		}
		m_nCount = i;
		fclose(fp);
		SortingById();		// 아이템을 아이디 순으로 정리한다.

		if(fp=Fopen( filenameBin, "wb" ) )		// bin 파일 생성
		{
			char *tt = (char *) m_Item;
			char crc=0;
			for( int i=0; i<sizeof(CItemExplainBasic)*m_nCount; i++ )
			{
				crc += *tt;
				tt++;
			}
			fwrite( &m_nCount , sizeof( int ), 1, fp );			// 갯수 저장
			fwrite( &crc, 1, 1, fp );
			fwrite( m_Item, sizeof(CItemExplainBasic), m_nCount, fp);
			fwrite( &crc, 1, 1, fp );
			fclose(fp);
		}
	}
	else
	{
		if(fp=Fopen( filenameBin, "rb" ) )
		{
			char *tt = (char *) m_Item;
			char crc=0, crc1, crc2;
			fread( &m_nCount, sizeof( int ), 1, fp );
			fread( &crc1, 1, 1, fp );
			fread( m_Item, sizeof(CItemExplainBasic), m_nCount, fp);
			fread( &crc2, 1, 1, fp );
			fclose(fp);

			for( int i=0; i<sizeof(CItemExplainBasic)*m_nCount; i++ )
			{
				crc += *tt;
				tt++;
			}
			if( crc != crc1 || crc != crc2 ) 
			{
				CheckSumError = -1;
				JustMsg(lan->OutputMessage(6,87) );//CheckSumError = 1;//010215 lsw
			}
		}
	}
}
int CItemExplain::SetValue( int para, char *buf )
{
	char *token;

	token = strtok(buf,"\t\n\r");		// 필요없는 번호
	if( token == NULL ) return 0;

	token = strtok(NULL,"\t\n\r");
	if( token == NULL ) return -1;
	m_Item[para].m_nItemId = atoi(token) / 100;

	token = strtok(NULL,"\t\n\r");		// 없으면 안되는 항목
	if( token == NULL ) return -1;
	strcpy( m_Item[para].m_szHanName, token );
	
	token= strtok(NULL, "\t\n");
	if( token == NULL ) return 0;		// 없을수도 있는 항목
#ifdef _DEBUG // finito 060507
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )
	{
		int len = strlen( token );
		if( len >= MAX_ITEM_EXPLAIN_MSG )
			JustMsg( lan->OutputMessage(6,88) );//010215 lsw
	}
#endif
	strcpy(m_Item[para].explain, token);

	return 1;
}
void CItemExplain::SortingById()
{
	for( int i=0; i<m_nCount-1; i++ )
	{
		for( int j=i; j<m_nCount; j++ )	
		{
			if( m_Item[i].m_nItemId == 0 || m_Item[i].m_nItemId > m_Item[j].m_nItemId )
			{
				CItemExplainBasic temp = m_Item[i];
				m_Item[i] = m_Item[j];
				m_Item[j] = temp;
			}
		}
	}
}

int CItemExplain::SearchById( int id )		// 2진 탐색
{
	int low=0, high = m_nCount-1;
	int mid = 0;
	while( low <= high )
	{
		mid = (low+high)/2;
		if( m_Item[mid].m_nItemId == id ) return mid;
		if( m_Item[mid].m_nItemId < id ) low = mid+1;
		else high = mid-1;
	}

	return -1;
}

char *CItemExplain::GetName( int para )
{
	if( para < 0 || para >= MAX_ITEM_EXPLAIN_COUNT ) return NULL;
	return m_Item[para].m_szHanName;
}

char *CItemExplain::GetExplain( int para )
{
	if( para < 0 || para >= MAX_ITEM_EXPLAIN_COUNT ) return NULL;
	return m_Item[para].explain;
}

// 특정 아이템이 갯수만큼 있는지 체크
// 둘 사이의 차이를 보내준다.
int CheckInventory( const int iItemNo, const int iNeedCount )//021111 lsw
{
	int ct = 0;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				if(InvItemAttr[a][b][c].item_no == iItemNo ) ct++;
			}

	return ct - iNeedCount;
}


///////////////////////////////////////////////////////////////////
//
void CItem_Weapon::GetItemNeedAbility( int &count, int *type, int *value )
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) need[10] = Need3_ps;

	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS };
	int max = sizeof( need ) / sizeof( int );


	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Armor::GetItemNeedAbility( int &count, int *type, int *value )
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) need[10] = Need3_ps;
	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS};
	int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Disposable::GetItemNeedAbility( int &count, int *type, int *value )
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) need[10] = Need3_ps;
	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS};
	int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Accessory::GetItemNeedAbility( int &count, int *type, int *value )
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) need[10] = Need3_ps;
	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS};
	int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Etc::GetItemNeedAbility( int &count, int *type, int *value )
{
	int need[] = { Need3_str, Need3_dex, Need3_con, Need3_wis, Need3_int, Need3_cha, Need3_end, Need3_Movp, Need3_mor, Need3_luc, Need3_ws, };
	if( SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL ) need[10] = Need3_ps;
	int t[] = { STR, DEX, CON, WIS, INT_, CHA, ENDU, MOVP, MOR, LUCK, WSPS};
	int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}


void CItem_Herb		::GetItemChangeAbility( int &count, int *type, int *value )		
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc, };
	static int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Cook		::GetItemChangeAbility( int &count, int *type, int *value )		
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc, };
	static int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Potion		::GetItemChangeAbility( int &count, int *type, int *value )	
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_movp, Change_mor, Change_luc };
	static int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Tool		::GetItemChangeAbility( int &count, int *type, int *value )		
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_movp, Change_mor, Change_luc, };
	static int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Weapon		::GetItemChangeAbility( int &count, int *type, int *value )	
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc, 
		Change_ws, Change_ps, Change_mag, Change_dev, casting_Time, Change_Life_max, Change_mana_max, Change_Divine_Max, Change_health_max, };
	static int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Armor		::GetItemChangeAbility( int &count, int *type, int *value )	
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc, 
		Change_ws, Change_ps, Change_mag, Change_dev, casting_Time, Change_Life_max, Change_mana_max, Change_Divine_Max, Change_health_max, };
	static int max = sizeof( need ) / sizeof( int );

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}
void CItem_Etc		::GetItemChangeAbility( int &count, int *type, int *value)		
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_mor, Change_luc, 
		Change_ws, Change_ps, Change_mag, Change_dev, casting_Time, Change_Life_max, Change_mana_max, Change_Divine_Max, Change_health_max, };
	static int max = sizeof( need ) / sizeof( int );

	int t[] = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = t[i];
			value[count++] = need[i];
		}
	}
}
void CItem_Accessory::GetItemChangeAbility( int &count, int *type, int *value )
{
	int need[] = { Change_str, Change_dex, Change_con, Change_wis, Change_int, Change_cha, Change_end, Change_Movp, Change_mor, Change_luc, 
		Change_ws, Change_ps, Change_mag, Change_dev, casting_Time, Change_Life_max, Change_mana_max, Change_Divine_Max, Change_health_max, };
	static int max = sizeof( need ) / sizeof( int );

	

	count = 0;
	for( int i = 0; i< max; i++ )
	{
		if( need[i] )
		{
			type[count] = i;
			value[count++] = need[i];
		}
	}
}

int GetAntiMagic_1( int *anti, int *ability )
{
	int count = 0;
	for( int i=0; i<6; i++ )
	{
		if( *(ability+i) )
		{
			anti[count++] = *(ability+i);
		}
	}
	return count;
}

bool EqualItem( const ItemAttr *source, const ItemAttr *target )
{
	/*
	if( source->item_no != target->item_no ) return 0;
	for( int i=0; i<6; i++ )
	{
		if( source->attr[i] != target->attr[i] ) return 0 ;
	}
*/
	if( memcmp( source, target, sizeof( ItemAttr) ) == 0 ) return 1;
	return 0;
}
