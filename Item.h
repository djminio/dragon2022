#if !defined(AFX_ITEM_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
#define AFX_ITEM_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_
//021014 lsw
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//021119 lsw
const int ITEM_EQUIP_MAX	=8;
const int ITEM_QUICK_MAX	=6;
const int ITEM_INV_MAX		=72;	//3*3*8
const int ITEM_BANK_MAX		=90;	//5*3*6
const int ITEM_HAND_MAX		=1;
const int ITEM_BUFFER_MAX	=1;

enum IA2_TYPE
{
	IA2_HIDE			= 0x00000001,
	IA2_NOTMOVE			= 0x00000002,
	IA2_POISONED		= 0x00000004,
	IA2_OPENED			= 0x00000008,
	IA2_BROKEN			= 0x00000010,
	IA2_HOUSE			= 0x00000020,
	IA2_MAGICED			= 0x00000040,
	IA2_CANNOTUSE		= 0x00000080,
	IA2_NOIDENTIFIED	= 0x00000100,
	IA2_DOOR			= 0x00000200,
	IA2_BOX				= 0x00000400,
	IA2_CHAIR			= 0x00000800,
	IA2_TABLE			= 0x00001000,
	IA2_FARM0			= 0x00002000,
	IA2_FARM1			= 0x00004000,
	IA2_FARM2			= 0x00008000,
	IA2_FARM3			= 0x00010000,
	IA2_FARM4			= 0x00020000,
	IA2_FARM5			= 0x00040000,
	IA2_FARM6			= 0x00080000,
	IA2_FARM7			= 0x00100000,
	IA2_FIRE			= 0x00200000,
	IA2_TRAPED0			= 0x00400000,
	IA2_TRAPED1			= 0x00800000,
	IA2_TRAPED2			= 0x01000000,
//	IA2_FIRST_ITEM		= 0x02000000,
	IA2_SEAL_NOR		= 0x04000000,
	IA2_SEAL_SPC		= 0x08000000,
	IA2_COLOSSUS_STONE	= 0x10000000,
	IA2_SCENARIO_POTAL	= 0x20000000,
};

enum ITEM_STRUCT_INDEX
{
	IATTR_DURATION		=0,
	IATTR_LIMIT			=1,
	IATTR_ATTR			=2,
	IATTR_ID			=3,
	IATTR_RESERVED0		=4,
	IATTR_RESERVED1		=5,
};
const int IATTR_MUCH			= IATTR_DURATION;
const int IATTR_SEAL_STONE_NO	= IATTR_LIMIT; 
const int IATTR_SPRITE_TYPE		= IATTR_LIMIT;
const int IATTR_RARE_MAIN		= IATTR_ID;
const int IATTR_TREASURE_MAP	= IATTR_RESERVED0;	// 040105 YGI
const int IATTR_MALL_TIMED		= IATTR_RESERVED0;  // Used by items in the mall that have a time limit

enum ITEMANIMATION_TYPE
{
	ITEMANIMATION_NO		=0,	
	ITEMANIMATION_REPEAT	=1,
	ITEMANIMATION_RETURN	=2,
	ITEMANIMATION_OPENNING	=3,
	ITEMANIMATION_OPENED	=4,
	ITEMANIMATION_CLOSING	=5,
	ITEMANIMATION_CLOSED	=6,
	ITEMANIMATION_FARM0		=7,
	ITEMANIMATION_FARM1		=8,
	ITEMANIMATION_FARM2		=9,
	ITEMANIMATION_FARM3		=10,
	ITEMANIMATION_FARM4		=11,
	ITEMANIMATION_FARM5		=12,
	ITEMANIMATION_FARM6		=13,
	ITEMANIMATION_FARM7		=14,
	ITEMANIMATION_FIRE		=15,
	ITEMANIMATION_ACTIVE	=16,
};
//# Type - 0:보통 Item  1: 의자   2: Table  3: 도어  4:Box 
enum ITEMTYPE_INDEX
{
	ITEMTYPE_NORMAL			=0,
	ITEMTYPE_CHAIR			=1,
	ITEMTYPE_TABLE			=2,
	ITEMTYPE_DOOR			=3,
	ITEMTYPE_BOX			=4,
	ITEMTYPE_SIGNBOARD		=5,
	ITEMTYPE_DIRECTBOARD	=6,
};
enum ITEMTYPE_WORKTABLE_TYPE
{
	ITEMTYPE_WORKTABLE_MINING		=71,	// 작업대 용광로(71)
	ITEMTYPE_WORKTABLE_BATTLE       =72,	// 작업대 배틀
	ITEMTYPE_WORKTABLE_SUNBAN		=73,  // 작업대 선반
	ITEMTYPE_WORKTABLE_MORU			=74,	// 작업대 모루
	ITEMTYPE_WORKTABLE_HWADUK		=75,	// 작업대 화덕
	ITEMTYPE_WORKTABLE_SIHUMKWAN	=76,  // 작업대 시험관류
	ITEMTYPE_WORKTABLE_MULAE		=77,	// 작업대 물레
	ITEMTYPE_WORKTABLE_BULDAE		=78,// 작업대 불대 
	ITEMTYPE_SEAL_NOR				=79,
	ITEMTYPE_SEAL_SPC				=80,
	ITEMTYPE_HP_UP					=81,
	ITEMTYPE_MP_UP					=82,
	ITEMTYPE_SCALE					=83,
	ITEMTYPE_NATION_STONE			=84,
	ITEMTYPE_GUILD_STONE			=85,
	ITEMTYPE_SENARIO_STONE			=86,
	ITEMTYPE_COLOSSUS_STONE			=87,
};

typedef struct tagItemSprite
{
	int   no;
	short count;
	Spr sp;
	struct tagItemSprite *prev, *next;
}ITEMSPRITE, *LPITEMSPRITE;

typedef struct sItem 
{ 
	int server_id;
	int no;
	unsigned int  attr[6];
	short status;
	short sx, sy;
	short offx, offy;
	short x, y;
	short mx, my;
	Spr *spr[ 10];
	short  dsx[ 10], dsy[10], ddx[ 10], ddy[10];

	DWORD	host;
	BYTE	type;
	BYTE	getable;
	BYTE	Anitype;
	char	Sorttype;
	BYTE	Where;
	BYTE	shadow;
	char	framecount;
	char	dirno;
	short	dx[4], dy[4];

	short  curframe;
	char   anitoggle;

	struct sItem *prev;
	struct sItem *next;
}ITEMGROUND, *LPITEMGROUND;

extern void AddItemList( LPITEMGROUND i );
extern void DeleteItemList( int server_id );
extern LPITEMGROUND FindItemList( int x, int y );
extern void DrawItemList( void );
extern void DrawObjectItem_SealStone( void );
extern void LoadItem( LPITEMGROUND i );
extern void FreeItem( LPITEMGROUND i );
extern int CheckEventWith_Item( void );
extern LPITEMGROUND FindItemList( int server_id );
extern void DestroyItemList( void );
extern void RecvItemAttrChange( t_item_attr_change *p );
extern void Send_CMD_ITEM_BOX_BREAK( LPITEMGROUND i );
extern void Send_CMD_JUST_ATTACK_ANIMATION( LPITEMGROUND i );
extern void Recv_CMD_ITEM_BOX_BREAK( t_server_item_box_break *p );
extern void Send_CMD_ITEM_BOX_BREAK_RESULT( int item_id );
extern void Recv_CMD_ITEM_BOX_MAGIC_BREAK_RESULT( t_server_item_box_magic_break_result *p );
extern void Send_CMD_ITEM_BOX_MAGIC_BREAK( int item_id );
extern bool IsPossibleAttackColossusStone( LPITEMGROUND item );
extern void SendAttackColossusItem( LPITEMGROUND item );
extern void CheckTrap( void );
extern void RecvGiveItem( t_server_give_item *p );
extern LPITEMGROUND g_Item, CursorCheckedItem;
extern ITEMGROUND   temp_Item;
extern void InitItemAttrs();
extern bool GetItemDurTxt(ItemAttr *pItem,char* szTxt);
extern char* GetTacticName(const int iTacticKind);
#endif // !defined(AFX_ITEM_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
