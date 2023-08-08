#ifndef	__SKILLTOOL_H__
#define	__SKILLTOOL_H__

#define		DEF_SKILLTILE_RANGE		4
#define		MAX_SKILLTILE_RANGE		10

#define		TOOL_FARMING			1
#define		TOOL_MINING				2
#define		TOOL_FISHING			3
#define		TOOL_CHOPPING			4
#define		TOOL_HUB				5	
#define		TOOL_NPC_GENER			6
#define		TOOL_DONTSKILL			7
#define		TOOL_BUILDHOUSE			8

//////////////// 0429 lkh 추가 ///////////////////
#define		MAX_KEY					4	//내집이 가질수 있는 문의 최대수
#define		MAX_HOUSEITEM			45	//한개 집이 수용알수 있는 아이템의 총갯수

//////////////////////////////////////////////////

typedef struct  tagMAPSKILLTABLE
{
	int type;							//일반적인 기술관련 타일인 경우->0 / NPC 생성 위치 정의->1
	int x, y;							//해당 기술속성을 지닌 타일의 좌표(타일단위) //기획요구->인터페이스의 사용편의 증대
	int skillno;						//기술속성 고유번호(1~18까지)
	int tile_Range;						//해당타일로부터 효과가 유효한 타일 거리(반지름) // 최대 발생수. 
	int probability;					//해당기술이 성공할 확률	//  Event번호..
	int type_Num;						//NPC 고유 타입 번호(0~99)
	int subType;						//해당 기술의 하위 관련 데이터값

	struct  tagMAPSKILLTABLE *prev;		//이전 스킬 속성 테이블 포인터	
	struct  tagMAPSKILLTABLE *next;		//다음 스킬 속성 포인터


}	MAPSKILLTABLE, *lpMAPSKILLTABLE;

typedef struct  tagDRAGMOUSE
{
	int sx, sy;
	int ex, ey;
}	DRAGMOUSE;

typedef struct	tagMYHOUSETOOL
{
//	int	map;			//맵종류(1->ma-in/2->k_sung2/3->source/4->gray/5->hu_vm)
	int sx, sy;			//시작점
	int ex, ey;			//종결점
	int type;			//집의 고유 번호
	int object_Num;		//집에 들어있는 오브젝트 갯수
//	TILE* startTile;	//시작점 타일의 주소
}	MYHOUSETOOL;

typedef struct	tagMyHouse
{
	BYTE		houseType;			//집의 종류
	char		Owner[20];			//집의 주인 - NM_LENGTH
	BYTE		mapType;			//집이 위치하는 맵의 종류
	POINT		position;			//집의 위치 정보
	BYTE		doorKey[MAX_KEY];	//집의 열쇠 정보(최대 문수4개)
	char		doorBoard[30];		//문패에 적힌 문구
	//ITEMGROUND	houseItem[MAX_HOUSEITEM];	//집에서 수용할수 있는 아이템
	int			durability;			//집의 내구도
	BYTE		houseStatue;		//집의상태정보 (0->부서짐/1->정상상태/2->심하게 망가져 수리요망)
}	MYHOUSE;

extern int		radio_Statue;
extern void		PutMyhouse(int x, int y);
extern void		MapSkillTool_Attr( int mx, int my );
extern BOOL		LoadSkillMapTable(void);
extern BOOL		SaveSkillMapTable(void);

extern MAPSKILLTABLE	g_MapSkillTable;	//한개 타일의 스킬 속성을 가지고 있는 구조체	
extern lpMAPSKILLTABLE	Header[8][8];
extern DRAGMOUSE		g_DragMouse;		//마우스의 드래그 처리를 위한 구조체
extern MYHOUSETOOL		g_MyhouseTool;		//내집 시스템 이미지타일 저장 구조체
extern MYHOUSE			g_MyHouse;			//실제 내집 정보 구조체
extern BOOL				g_MyhouseDlgOpen;

// thai2 YGI
extern char	subFarmType[20][25];
extern char	subMineType[20][25];
extern char	subHubType[20][25];

////////////////////// 0613 lkh 추가 ///////////////////////
extern void DeleteNPCGenerate(int npc_Num=0);
extern void DeleteAllSkillData( void );
extern void ChangeNPCGenerate(int npc_from, int npc_to );

extern void tool_MyHouseLBD( WPARAM wParam, LPARAM lParam );
extern void tool_MyHouseLBU( WPARAM wParam, LPARAM lParam );

extern void		DrawSkillBox(int mox, int moy);
extern BOOL		CALLBACK SkillToolproc( HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam );
extern BOOL		CALLBACK MyhouseToolproc( HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam );
extern void		CheckHouseObjectEdit ( HWND hwnd, HINSTANCE hInstance);

extern void DeleteSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE lpST);	//두번째 인자의 경우 FindSkill함수에 의해 넘겨진 포인터
extern void AddSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE	lpST );
extern lpMAPSKILLTABLE FindSkill( lpMAPSKILLTABLE *Header, int x, int y, int order = 0);		// 010314 KHS
extern void GetEffect2Pix(void);
extern void DeleteAllType(int delete_Type);



#endif	// __SKILLTOOL_H__