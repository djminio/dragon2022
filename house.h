#if !defined(AFX_HOUSE_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
#define AFX_HOUSE_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_
//021014 lsw
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagABOUTHOUSE{

	short openhouse[ 100];
}ABOUTHOUSE, *LPABOUTHOUSE;

extern ABOUTHOUSE			Housing;
extern int					CurOpenHouse; //현재 열려있는 지붕의 번호.
extern void SendRoofOpenHouse( int roofno );
extern void SendRoofCloseHouse( int roofno );
extern void RecvRoofOpen( int roofno );
#endif//AFX_HOUSE_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_