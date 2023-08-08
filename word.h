#if !defined(AFX_WORD_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
#define AFX_WORD_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//definition
const int MAX_NPC_REPLY		= 200;//Maximum words NPC can reply.
const int MAX_PCWORD		=200;
const int CLOSE_ENOUGH		= 8;  // The talkable distance between NPC and Player
const int REPLY_RATE		= 5;  // 20% NPC start talking to Player 

extern int max_pc_word;			// ���� NPC�� ����Ҽ��ִ� ������ ��
extern int max_npc_word;			// ���� NPC�� ����Ҽ��ִ� ������ ��

//Functions
LPCHARACTER whoIsClose(int*); //0615 woo
DWORD itemPriceWhenSell( char *item_name);
DWORD itemPriceWhenBuy( char *item_name);
bool isThatMyItem( char *item_name );
char* getRidOfStuff( char *text );
int GetRandomMessageByNpc( char *file_name, int get_line, char *result_str );
int amountItem( char *item_name );
int itemNameToNumber( char *item_name );
int whichCommand( char *text );
int makeDeal( LPSTR lpStr);
void languageSystem(LPSTR lpStr);
void CheckNpcTalk( );

class CPcWord
{
public :
	char		*word;
	short int	cmd;

public :
	CPcWord();
	void AddNode( char *str, int num ); // �߰��� �ܾ�� ��� ����
	void DeleteNode( );
	~CPcWord();
	int CompareWord( const char *target_str );
	char *ReplyWords( int num );
	char *ReturnWords( );
};

void replyTable(char* getFile, CPcWord getWord[], int &max);
char *ReplyNpc( char*, CPcWord[], CPcWord[] );
extern CPcWord pc_word[MAX_PCWORD];			// PC�� ���� ������ �߿��� �ܾ�
extern CPcWord npc_word[MAX_PCWORD];			// NPC�� ����� ����Ʈ
#endif	// AFX_WORD_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_