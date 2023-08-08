// HunterSystem.h: interface for the CHunterSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUNTERSYSTEM_H__818A3CCE_7C23_476F_AEFD_212049A85436__INCLUDED_)
#define AFX_HUNTERSYSTEM_H__818A3CCE_7C23_476F_AEFD_212049A85436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHunterSystem  
{
public:
	CHunterSystem();
	virtual ~CHunterSystem();

};

class CHunterList
{
public:
	int my_sight;		// 내 시야로 들어왔다. count를 넣어서 처음 들어올때 깜박이게 한다.
	bool my_regist;		// 내 목록에 등록한 놈이다.
	bool is_end;
	k_hunter_list list;
	CHunterList *next;
	CHunterList *prev;


public:
	CHunterList() { next = NULL; prev = NULL; is_end = 0; list.index = 0; my_regist=0; my_sight=0; }
	~CHunterList() { SAFE_DELETE(next); };
	void DeleteAll();
	CHunterList *AddNode( k_hunter_list *temp );
	int DeleteNode( char *name );		// 몇번째 노드를 삭제할 것인가..
	int print( int x, int y );
	int CheckList( char *name, int type );			// 시야 안으로 내 목록에 있는 넘이 들어 왔는지.. 나갔는지...
	int print2( int x, int y );						// 내가 등록한 현상범 목록 출력...
	int KillCharacter( char *name );
	bool ExistMyHunterList( char *name );
	int CompName( char *name );
	int CompTargetName( char *name );
	int CompTargetId( short int id );

	friend void CHunterList_Next( CHunterList *& );
	friend void CHunterList_Prev( CHunterList *& );

};
extern CHunterList *hunter_list;
extern CHunterList *curr_hunter_list;
extern CHunterList *my_hunter_list;
extern CHunterList *DeleteMyHunterList( CHunterList *header, char *name );

#endif // !defined(AFX_HUNTERSYSTEM_H__818A3CCE_7C23_476F_AEFD_212049A85436__INCLUDED_)
