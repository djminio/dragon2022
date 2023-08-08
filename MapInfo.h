// MapInfo.h: interface for the CMapInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPINFO_H__6A59C12B_1C45_495B_958A_D5A51EFE0646__INCLUDED_)
#define AFX_MAPINFO_H__6A59C12B_1C45_495B_958A_D5A51EFE0646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MAP_		200		// 서버하고 똑같아야 한다.

#pragma pack(push, 1)
typedef struct MAPINFO
{
	char mapfile[30];
	char mapname[30];
	char cPosType; // CSD-030509
	int  nSaveX;   // CSD-030509
	int  nSaveY;   // CSD-030509
	char daylightcontrol;	// 지붕이 있으면 0     // 없으면  ( 아래것과 거의 같다 ) 
	char rain;		//  1 :이면 비가 오는곳이다.	0 : 비가 오지 않는 동굴같은 곳이다. 
	char nation;	//  국가코드. 
	char forrookie;
	int  ghostguide3x, ghostguide3y;	// nation code 3
	int  ghostguide4x, ghostguide4y;	// nation code 4..

	int	 port;
	int  group;//020420 lsw
	int  Class;//021113 lsw
	int  LvMin;//020420 lsw
	int  LvMax;//021113 lsw
	int  nDisableMagicType; // CSD-030516

	int	 m_MoveType;//GM이 셋팅 하는 데이타, 테이블에는 이런 필드가 없음

	MAPINFO()
	{
		cPosType = 0; // CSD-030509
		nSaveX = 0;   // CSD-030509
		nSaveY = 0;	  // CSD-030509
		daylightcontrol= 0;	// 지붕이 있으면 0     // 없으면  ( 아래것과 거의 같다 ) 
		rain= 0;		//  1 :이면 비가 오는곳이다.	0 : 비가 오지 않는 동굴같은 곳이다. 
		nation= 0;	//  국가코드. 
		forrookie= 0;
		ghostguide3x= 0, ghostguide3y= 0;	// nation code 3
		ghostguide4x= 0, ghostguide4y= 0;	// nation code 4..

		port = 0;
		group= 0;
		Class= 0;
		LvMin= 0;
		LvMax= 0;
		nDisableMagicType = 0; // CSD-030516

		m_MoveType = 0;
	}
}t_MapInfo,*LPMAPINFO;
#pragma pack(pop)

extern t_MapInfo MapInfo[MAX_MAP_];
#endif // !defined(AFX_MAPINFO_H__6A59C12B_1C45_495B_958A_D5A51EFE0646__INCLUDED_)
