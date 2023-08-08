// SymbolMgr_Client.h: interface for the CSymbolMgr_Client class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLMGR_CLIENT_H__9D89D9F0_6A0E_4CE1_8C8B_BD4D94069F9A__INCLUDED_)
#define AFX_SYMBOLMGR_CLIENT_H__9D89D9F0_6A0E_4CE1_8C8B_BD4D94069F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "mytypes.h"
#include "network.h"

using namespace std;

#define SYMBOL_ITEM_MAX		23
#define SYMBOL_LEVEL_MAX	5
#define SYMBOL_ATTR_COUNT	5

typedef struct tag_SYMBOL_ITEM
{
	int		nItemNumber;
	char	strName[65];
	char	strHanName[65];
	int		n1StClass;
	int		n2ndClass;
	int		nRare[5];
}SYMBOL_ITEM;

typedef struct tag_SYMBOL_RARE
{
	int anRare[SYMBOL_ITEM_MAX];
}SYMBOL_RARE;

typedef vector<SYMBOL_RARE> VT_SYMBOL_RARE;


class CSymbolMgr_Client
{
	Spr		m_ItemSpr[SYMBOL_ITEM_MAX];
	vector<SYMBOL_ITEM>		m_vtSymbolList;
	vector<VT_SYMBOL_RARE>	m_vtRareValueList;

public:	
	void SaveBinFile_Item_Symbol_Abilliy();
	void SaveBinFile_Item_Symbol();
	bool m_bBeta;
	void SaveBinaryFiles();
	bool m_bTextItemMode;//bool m_bTextMode; soto-031031
	bool m_bTextAbilliyMode;//soto-031031
	void Recv_SymbolUpGrade(t_SYMBOL_ITEM_MSG* pSymbol);
	void SetDualItem(int n1StClass, int n2StClass, int nLevel);
	void Recv_SymbolMake(t_SYMBOL_ITEM_MSG* pSymbol);
	bool IsHave();
	int m_nPutY;
	int m_nPutX;
	int GetItemNumber();
	void Load();
	int RareValue(int nItemNumber,int AttrCount, int nLevel);
	int GetRareNum(int nSymbolNumber,int AttrCount);// 주소가아니라. 몇번째인가가 인자로 쓰인다.
	void Explain(int nX, int nY);
	bool LoadTables();
	int m_nGrade;
	void SetDualItem(int nItemNumber,int nLevel);
	void PutSymbolItem();//아이템 이미지를 출력한다.
	void DestroyAllObject();
	void LoadItemImage();
	CSymbolMgr_Client();
	virtual ~CSymbolMgr_Client();
};

#endif // !defined(AFX_SYMBOLMGR_CLIENT_H__9D89D9F0_6A0E_4CE1_8C8B_BD4D94069F9A__INCLUDED_)
