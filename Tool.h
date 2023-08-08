

typedef struct {
	int dx;						// 0이면 전체모드, 1이면 창모드
	int notdead;				// 1이면 주인공은 무적
	int music;					// 1이면 음악을 켬 
	int effect;					// 1이면 효과을 켬 
	int cddata;					// dummy
	int onekill;				// 한방에 죽인다. 
	char notepad[FILENAME_MAX];	// notepad : 0  ultra editor : 1 
	int notconectserver;
	int enable_menu;
	int viewcharacterframe;
	int result;					// 
	int gamemakemode;
	int loaditemfromtoi2;
	int makeanitable_bin;
	char ip[ 16];
	char agentip[ 16];
	char proxyip[ 16];
	int port;
	int entergameserver;
	int enteragent;
	int enterproxy;
	int keyr;
	int keyp;
	int mapscrolltype;
	int mapx, mapy;
	int makesou2size;


}DEVELOPE_ENVIR;



#ifndef __TOOL_H__
#define __TOOL_H__




extern 	DEVELOPE_ENVIR SysInfo;
extern HWND CheckAnimationHwnd;
extern  int  tool_ViewAttrEventFlag;
extern  int  tool_ViewAttrDontFlag;
extern  int  tool_ViewAttrInside;
extern  int  tool_ViewAttrNoBattle;
extern  int  tool_ViewAttrCanMatch;
extern  int  tool_ViewAttrOccupied;
extern  int  tool_ViewAttrTop;			// 010904 LTS
extern bool tool_ViewMonsterNo;	// YonMy;8;
extern int tool_AnimationEditCurFrame;
extern int tool_AnimationEditTotalSpriteNo;
extern int tool_ViewAnimationFrame;
extern bool tool_DrawMap, tool_DrawObject, tool_DrawFrontMap, tool_ViewMessage, tool_ViewChat, tool_ViewNumber, tool_ViewMenu;
extern int tool_DrawRoofMap;
extern int tool_MultiChar;
extern int tool_FrameCheck;
extern int tool_mapobject_modify;
extern int tool_ViewAttrFrontFlag;
extern int tool_ViewAttrRoofOnOff;
extern int tool_ID_INPUT_MAP_ATTR;
extern int tool_ViewAttrRiver;
extern int tool_ViewAttrGEffect;
extern int tool_MouseCursorBlock;
extern char tool_ID_EFFECT_TEST;
extern char tool_IDD_INPUT_MAPOBJECT;
extern int tool_CurMapObjectId;
extern int tool_ID_SKILL_INPUT;
extern int tool_ID_VIEW_SKILLATTR;
extern HWND	InputSkillAttrHdlg;

extern int tMapAttr;
extern int tMapAttrAllAffect;
extern int tMapAttrX, tMapAttrY;
extern int tMapAttr_Offset_Modify;
extern int tMapAttrBrush;
extern char tool_ViewAllCharacter;
extern bool	tool_SkillTool_FARMING;
extern bool	tool_SkillTool_MINING;	
extern bool	tool_SkillTool_FISHING;	
extern bool	tool_SkillTool_CHOPPING;
extern bool	tool_SkillTool_HUB;
extern bool	tool_SkillTool_NPC;
extern bool	tool_SkillTool_ITEM_SEAL;

extern int tool_Pathfind;
extern bool tool_ViewMyExp;

extern int tool_ID_INPUT_MYHOUSE;
extern int tool_ID_OUTPUT_MYHOUSE;
extern HWND	InputMyhouseHdlg;


#ifdef _DEBUG
extern char	AnimationName[ MAX_ANIMATION][ 110];
#endif




extern int  SpriteOrderFrame, SpriteOrderDir;
extern int  AnimationEditWorkStartFlag;
extern int  CheckEquipmentFlag;
extern int  TempAnimationCount;
extern int	IDCheckedMapObject;

extern void Box( int x1, int y1,  int x2, int y2,  int x3, int y3,  int x4, int y4, int color );
extern void Box( int sx, int sy,  int ex, int ey, int color );
extern void Box_( int sx, int sy,  int xl, int yl, int color );
extern void PutDot( int x, int y, WORD color );
extern void Line(int sx, int sy, int ex, int ey, int color);
extern void XLine( int x, int y, DWORD xl, WORD color );
extern void Circle(int xc,int yc,int a0,int col );
extern void BoxFill( int sx, int sy, int xl, int yl, int color);

extern void DoSomeThingWithKey( WPARAM wParam, LPARAM lParam );
extern void CheckCharacterFrameOrder( WPARAM wParam, LPARAM lParam );

extern void ViewMapObjectInfo( void );
extern void ViewItemInfo( void );


extern void SystemMenuProcess( WPARAM wParam );
extern void ModeSetting ( HWND hwnd, HINSTANCE hInstance );
extern void CheckEquipmentData( HWND hwnd, HINSTANCE hInstance);


extern void tool_InputMapAttr( int mx, int my, int flag );

extern int AgreeStartGame( HWND hwnd, HINSTANCE hInstance );
extern void ToolLogoutCharacter( );

extern void MoveToCharacter();

#else



#endif

#define SI_GAME_MAKE_MODE		0
#define SI_NOT_CONNECT_MODE		1
extern inline int GetSysInfo( int type );
extern void StartOXEvent( );

// 020428 YGI acer
extern void ToolMapMove();

// 030307 soto
extern void ToolMapMoveMultiple();
extern bool IsNPC(LPCHARACTER pCharac);
extern void ToolGenerateMonster();
#ifdef _DEBUG
extern void	MainMenuSelected(WPARAM wParam,LPARAM lParam);
#endif

extern void SetLocalWarTime();
extern void SetNationWarTime();
extern BOOL CALLBACK SetLocalWarTimeProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK SetNationWarTimeProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);

// ProcSetScenario 의 Dlg 핸들을 저장해두기위한 외부 변수이다.
extern HWND g_hSetScenarioDlgWnd;
////// ProcSetScenario에서만 쓰이는 전용 함수이다. 딴데서 쓰지 맙시다.
//
extern void ReleaseListData(HWND hListWnd);
extern void SelectedTimeList(HWND hListWnd,int index);
extern void SendSelectedSenarioName(int index);
extern bool AddTimeList(void* data);
//
//////////////////////////////////////////////////////////////////////

// 030307 soto End.

// 020830 YGI
extern BOOL CALLBACK ProcSetScenario(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);
extern void StartSetScenarioTime();

#include <VECTOR>
using namespace std;

#include <atlstr.h>
#include<atltypes.h>
using namespace std;

struct  _DebugMsg
{
	CString Str;
	CPoint pos;
	_DebugMsg(CString& Msg, CPoint& Position)
	{
		Str = Msg;
		pos = Position;
	}
	~_DebugMsg()
	{
	}
};
class CDownMsgView :public TSingleton<CDownMsgView>
{
public:
	CDownMsgView();
	virtual ~CDownMsgView();

	inline void GetDrawRect(CRect& _Rect);
	void Init_SetHwnd(HWND hWND);
	void PutDebugMsg(CString& Msg, CPoint& pos = CPoint(10, 10));
	void OnDraw(HWND hHwnd);

	void ReleaseData();

private:
	HWND m_hLogHWND;
	std::vector<_DebugMsg> m_vDebugViewMsg;
};
//extern CDownMsgView g_MapMoveTU_Log;

#define g_DebugWnd_TU g_MapMoveTU_Log