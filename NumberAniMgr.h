// NumberAniMgr.h: interface for the CNumberAniMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NUMBERANIMGR_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_)
#define AFX_NUMBERANIMGR_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_HPUPDOWN		1000

typedef struct 
{
	bool flag;
	int id;
	int hp;
	int attackedresult;
	int hpupdown;
}t_hpupdown;

typedef struct tagNUMBERANI{	
	short x, y;
	short d, delay;
	int offset;
	char st;
	char type;
	int  number;
	struct tagNUMBERANI *prev, *next;
}NUMBERANI, *LPNUMBERANI;

class CNumberAniMgr  
{
public:
	CNumberAniMgr();
	virtual ~CNumberAniMgr();

};

#define NUMBER_ANIMATION_TYPE_UP_		0
#define NUMBER_ANIMATION_TYPE_DOWN_		1	// Health Down.,..
	
#define NUMBER_ANIMATION_ST_EMPTY_		0
#define NUMBER_ANIMATION_ST_ANIMATING_	1	
#define NUMBER_ANIMATION_ST_END_		2	

extern void DeleteNumberAni( LPNUMBERANI tt );
extern LPNUMBERANI AddNumberAni( int type, int off, int x, int y, int num );
extern void OutputNumberAni( void );

extern void		DisplayChatAndNumber( void );

extern LPNUMBERANI  g_NumberAni;
#endif // !defined(AFX_NUMBERANIMGR_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_)
