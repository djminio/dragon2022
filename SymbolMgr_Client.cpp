// SymbolMgr_Client.cpp: implementation of the CSymbolMgr_Client class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SymbolMgr_Client.h"
#include "Convert565to555.h"
#include "Hong_Sprite.h"
#include "Hong_Sub.h"
#include "MenuDef.h"
#include "TextOutMgr.h"
#include "Hangul.h"
#include "ItemTable.h"
#include "skill_lsw.h"
#include "CurrentMsgMgr.h"
#include "Menu.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CSymbolMgr_Client	g_SymBolMgr;

CSymbolMgr_Client::CSymbolMgr_Client()
{
	memset(m_ItemSpr,0,sizeof(Spr) * SYMBOL_ITEM_MAX);
	m_nGrade	= 0;
	m_vtSymbolList.reserve(SYMBOL_ITEM_MAX);

	m_bTextItemMode = false;//soto-031031
	m_bTextAbilliyMode = false;//soto-031031
	m_bBeta = false;

	m_nPutX	= 634;
	m_nPutY = 160;
}

CSymbolMgr_Client::~CSymbolMgr_Client()
{
	DestroyAllObject();
}

void CSymbolMgr_Client::LoadItemImage()
{
	FILE	*fp;
	char filename[MAX_FILENAME];
	for(int num = 0; num < 23; num++ )
	{
		sprintf(filename, "./itemdata/110%02d00.csp", num+1 );
		if( fp = Fopen(filename, "rb" ) )
		{
			fread( &m_ItemSpr[num].xl, sizeof( short ), 1, fp);
			fread( &m_ItemSpr[num].yl, sizeof( short ), 1, fp);
			fread( &m_ItemSpr[num].ox, sizeof( short ), 1, fp);
			fread( &m_ItemSpr[num].oy, sizeof( short ), 1, fp);
			fread( &m_ItemSpr[num].size, sizeof( unsigned int ), 1, fp);
			
			
			MemFree( m_ItemSpr[num].img );		
			MemAlloc( m_ItemSpr[num].img, m_ItemSpr[num].size );
			fread(m_ItemSpr[num].img, m_ItemSpr[num].size, 1, fp);
			convert565to555( &m_ItemSpr[num] );
			fclose(fp);
		}
		
	}
}

void CSymbolMgr_Client::DestroyAllObject()
{
	for(int i = 0; i < SYMBOL_ITEM_MAX;i++)
	{
		if(m_ItemSpr[i].img)MemFree( m_ItemSpr[i].img );
	}
}

void CSymbolMgr_Client::PutSymbolItem()
{
	if(QuickItemAttr[5].item_no)//아이템이 존재할때 찍는다.
	{
		m_nPutX	= 634 + 10;
		m_nPutY = 160 + 2;
		PutCompressedImage( m_nPutX, m_nPutY, &m_ItemSpr[QuickItemAttr[5].item_no-1] );
	}
}

void CSymbolMgr_Client::SetDualItem(int nItemNumber, int nLevel )
{
	//일단퀵에 저장된다.
	QuickItemAttr[5].item_no = nItemNumber;
	QuickItemAttr[5].attr[0] = nLevel;

	m_nGrade = nLevel;
}


bool CSymbolMgr_Client::LoadTables()
{
	FILE*	fp = NULL;
	char	buf[1024] = {NULL,};
	char*	tok = NULL;
	const	char*	token = "\t\n";

	char	strPathSymbolTxt[MAX_PATH] = "./Itemtable/Item_Symbol.txt";
	char	strPathSymbolBin[MAX_PATH] = "./Itemtable/Item_Symbol.Bin";
	
	if(checkbeta == '1')
	{	//< CSD-031030
		m_bBeta = true;
	}	//> CSD-031030
	else
	{
		m_bBeta = false;
	}

	fp = Fopen(strPathSymbolTxt,"rt");
	if(fp)
	{
		m_bTextItemMode = true;
	}
	else
	{
		fp = Fopen(strPathSymbolBin,"rb");
		
		if(fp)
		{
			m_bTextItemMode = false;
		}
		else
		{
			JustMsg(" 정보에 필요한 화일이 없습니다 \"./Itemtable/Item_Symbol.Bin\" ");
			return false;
		}
	}
		
	if(fp)
	{
		if(m_bTextItemMode)
		{
			int i = 0;
			while(!feof(fp))
			{
				fgets(buf,1023,fp);
				if(i > SYMBOL_ITEM_MAX - 1)break;
				SYMBOL_ITEM	Item; memset(&Item,0,sizeof(Item));
				
				tok = strtok(buf,token); if(!tok)return false;
				Item.nItemNumber = atoi(tok);	tok = strtok(NULL,token); if(!tok)return false;
				tok = strtok(NULL,token); if(!tok)return false;
				strcpy(Item.strName,tok);tok = strtok(NULL,token); if(!tok)return false;
				strcpy(Item.strHanName,tok); tok = strtok(NULL,token); if(!tok)return false;
				Item.n1StClass = atoi(tok); tok = strtok(NULL,token); if(!tok)return false;
				Item.n2ndClass = atoi(tok);	tok = strtok(NULL,token); if(!tok)return false;
				tok = strtok(NULL,token); if(!tok)return false;
				tok = strtok(NULL,token); if(!tok)return false;
				Item.nRare[0] = atoi(tok);tok = strtok(NULL,token); if(!tok)return false;
				Item.nRare[1] = atoi(tok);tok = strtok(NULL,token); if(!tok)return false;
				Item.nRare[2] = atoi(tok);tok = strtok(NULL,token); if(!tok)return false;
				Item.nRare[3] = atoi(tok);tok = strtok(NULL,token); if(!tok)return false;
				Item.nRare[4] = atoi(tok);

				m_vtSymbolList.push_back(Item);
				i++;
			}
		}
		else //binary Mode
		{
			for(int i = 0;i < SYMBOL_ITEM_MAX;i++)
			{
				int nItemNumber = 0;char strName[65] = {NULL,};char strHanName[65] = {NULL,};
				int n1StClass = 0,n2ndClass = 0;int nRare[5] = {0,};
				SYMBOL_ITEM	Item; memset(&Item,0,sizeof(Item));
				int size = fread(&nItemNumber,sizeof(int),1,fp); Item.nItemNumber = nItemNumber;
				size = fread(strName,sizeof(char),65,fp);strcpy(Item.strName,strName);
				size = fread(strHanName,sizeof(char),65,fp);strcpy(Item.strHanName,strHanName);
				size = fread(&n1StClass,sizeof(int),1,fp);Item.n1StClass = n1StClass;
				size = fread(&n2ndClass,sizeof(int),1,fp);Item.n2ndClass = n2ndClass;
				size = fread(nRare,sizeof(int),5,fp);memcpy(Item.nRare,nRare,sizeof(int)*5);

				m_vtSymbolList.push_back(Item);
			}
		}
		
		fclose(fp);
	}
	else // 화일이 없다.
	{
		
	}

	//i = 0;
	fp = NULL;
	for(int i = 0;i < SYMBOL_ATTR_COUNT - 1;i++)
	{
		VT_SYMBOL_RARE			vtSymbolRare;
		char szFileNameBeta[MAX_PATH] = {NULL,};
		char szFileNameBetaTxt[MAX_PATH] = {NULL,};
		char szFileName[MAX_PATH] = {NULL,};
		char szFileNameTxt[MAX_PATH] = {NULL,};
		//< CSD-031030
		sprintf(szFileName,"./Itemtable/Item_Symbol_Ability%d.bin",i+1);
		sprintf(szFileNameTxt,"./Itemtable/Item_Symbol_Ability%d.txt",i+1);

		
		fp = Fopen(szFileNameTxt,"rt");
		if(fp)
		{
			m_bTextAbilliyMode = true;
		}
		else
		{
			fp = Fopen(szFileName,"rb");
			if(fp)
			{
				m_bTextAbilliyMode = false;
			}
		}			
				
		if(0 == i && NULL == fp)return false; //하나도 없으면..에러.

		SYMBOL_RARE	SymbolRare;memset(&SymbolRare,0,sizeof(SYMBOL_RARE));
		
		if(fp)
		{
			if(m_bTextAbilliyMode)//텍스트모드.
			{
				while(!feof(fp))
				{
					if(!fgets(buf,1023,fp))break;

					tok = strtok(buf,token);				
					for(int j = 0;j < SYMBOL_ITEM_MAX;j++)
					{					
						if(tok == NULL)
						{
							fclose(fp);return false;
						}
						SymbolRare.anRare[j] = atoi(tok);
						
						tok = strtok(NULL,token);
					}

					vtSymbolRare.push_back(SymbolRare);
				}
			}
			else//바이너리 모드.
			{
				while(!feof(fp))
				{
					for(int j = 0;j < SYMBOL_ITEM_MAX;j++)
					{
						int num = 0;
						
						fread(&num,sizeof(int),1,fp);
						
						SymbolRare.anRare[j] = num;
					}
					vtSymbolRare.push_back(SymbolRare);
				}							
			}
			fclose(fp);
		}
		else
		{
			break;
		}

		m_vtRareValueList.push_back(vtSymbolRare);
	}

	SaveBinaryFiles();

	return true;
}

void CSymbolMgr_Client::Explain(int nX, int nY)
{
	
	const int nLineGab = DEFAULT_LINE_GAB;
	int nGab = 0;

	const int nWidth = TXT_BG_NORMAL_WIDTH;
	
	if(QuickItemAttr[5].item_no && m_nGrade) //아이템 넘버가 있어야 소지하고 있는것이다.
	{
		switch(m_nGrade) //그레이드에 따라 색을 바꿀수 있다.
		{
		case 1:
			TxtOut.Init(COLOR_BLUE,nWidth,5);
			break;
		case 2:
			TxtOut.Init(COLOR_RED,nWidth,5);
			break;
		case 3:
			TxtOut.Init(COLOR_GREEN,nWidth,5);
			break;
		case 4:
			TxtOut.Init(COLOR_BROWN,nWidth,5);
			break;
		case 5:
			TxtOut.Init(COLOR_BLACK,nWidth,5);
			break;
		}

		TxtOut.PutTextHeaderBGImg(nX,nY,nWidth);
		Hcolor(ConvertColor(255,0,255));
#ifdef	KOREA_LOCALIZING_
		TxtOut.RcTXTOutB(nX,nY,nWidth,nGab,"<%d차>",m_nGrade);
		Hcolor(ConvertColor(255,255,255));nGab += nLineGab;
		TxtOut.RcTXTOutB(nX,nY+nGab,nWidth,nGab,"[%s]",m_vtSymbolList[QuickItemAttr[5].item_no-1].strHanName);
#else
		TxtOut.RcTXTOutB(nX,nY,nWidth,nGab,"<Grade %d>",m_nGrade);
		Hcolor(ConvertColor(255,255,255));nGab += nLineGab;
		TxtOut.RcTXTOutB(nX,nY+nGab,nWidth,nGab,"[%s]",m_vtSymbolList[QuickItemAttr[5].item_no-1].strName);
#endif
		nGab += nLineGab;
		
		for(int i = 0;i < SYMBOL_ATTR_COUNT;i++)
		{
			int		nRareNumber = this->GetRareNum(QuickItemAttr[5].item_no,i+1);
			
			if(!nRareNumber || MAX_ITEM_RARE <= nRareNumber)break;

			switch(ItemRare[nRareNumber].iExpMark/10)
			{
			case 1://1이면 +
				{
					::Hcolor(FONT_COLOR_RARE_PLUS_POINT);//부호색
					TxtOut.RcTXTOutNoBG(nX,nY+nGab,nWidth,5,"%s +",ItemRare[nRareNumber].ExplainAdd);// 1 : name
				}break;
			case 2://2이면 -
				{
					::Hcolor(FONT_COLOR_RARE_MINUS_POINT);//부호색
					TxtOut.RcTXTOutNoBG(nX,nY+nGab,nWidth,5,"%s -",ItemRare[nRareNumber].ExplainAdd);// 1 : name
				}break;
			}
			
			switch(ItemRare[nRareNumber].iExpMark%10)
			{
			case 0:
				{
					::Hcolor(ConvertColor(0,255,255));
					TxtOut.RcTXTOut(nX,nY+nGab,nWidth,5,"%s  ",ItemRare[nRareNumber].ExplainAdd);
				}
				break;
			case 1:
				{	
					int nRareNum = RareValue(QuickItemAttr[5].item_no,i+1,m_nGrade);
					if(nRareNum)
					{
						::Hcolor(ConvertColor(0,255,255));
						TxtOut.RcTXTOut(nX,nY+nGab,nWidth,5,"%s  %d",ItemRare[nRareNumber].ExplainAdd,nRareNum);
					}
					
				}
				break;
			case 2:
				{
					
					int nRareNum = RareValue(QuickItemAttr[5].item_no,i+1,m_nGrade);
					if(nRareNum)
					{
						::Hcolor(ConvertColor(0,255,255));
						TxtOut.RcTXTOut(nX,nY+nGab,nWidth,5,"%s  %d%%",ItemRare[nRareNumber].ExplainAdd,nRareNum);
					}
				}
				break;
			case 3:
				{
					int nRareNum = RareValue(QuickItemAttr[5].item_no,i+1,m_nGrade);
					if(nRareNum)
					{
						::Hcolor(ConvertColor(0,255,255));
						TxtOut.RcTXTOut(nX,nY+nGab,nWidth,5,"%s  %dSec",ItemRare[nRareNumber].ExplainAdd,nRareNum);
					}
				}
				break;
			}			
			nGab += nLineGab;
		}

		TxtOut.PutTextTailBGImg(nX,nY+nGab,nWidth);
		
	}
}

int CSymbolMgr_Client::GetRareNum(int nSymbolNumber, int AttrCount)
{
	if(m_vtSymbolList.size() < nSymbolNumber || nSymbolNumber <= 0)
	{
		return 0;
	}
	if(AttrCount > SYMBOL_ATTR_COUNT || AttrCount <= 0)
	{
		return 0;
	}

	return m_vtSymbolList[nSymbolNumber-1].nRare[AttrCount-1];
}

int CSymbolMgr_Client::RareValue(int nItemNumber, int AttrCount, int nLevel)
{
	if(m_vtSymbolList.size() < nItemNumber || nItemNumber <= 0) return 0;
	if(AttrCount > SYMBOL_ATTR_COUNT || AttrCount <= 0) return 0;
	if(m_vtRareValueList.size() <= 0) return 0;
	if(m_vtRareValueList[0].size() < nLevel || nLevel <= 0) return 0;

	
	return m_vtRareValueList[AttrCount-1][nLevel - 1].anRare[nItemNumber-1];
}

void CSymbolMgr_Client::SetDualItem(int n1StClass, int n2StClass, int nLevel)
{
 		
 	for(int i = 0;i < SYMBOL_ITEM_MAX;i++)
 	{
		int _1st = m_vtSymbolList[i].n1StClass;
 		int _2st = m_vtSymbolList[i].n2ndClass;
 		if(_1st == n1StClass+1 && _2st == n2StClass+1)
 		{
 			SetDualItem(i+1,nLevel);
 		}
 	}
}

void CSymbolMgr_Client::Load()
{
	LoadItemImage();
	LoadTables();
}

int CSymbolMgr_Client::GetItemNumber()
{
	return QuickItemAttr[5].item_no;
}

bool CSymbolMgr_Client::IsHave()
{
	return (QuickItemAttr[5].item_no > 0 && QuickItemAttr[5].item_no <= 23);
}

//DEL void CSymbolMgr_Client::Send_SymbolMake(int n1StClass, int n2StClass)
//DEL {
//DEL 		
//DEL }

//심볼을 만든다.
void CSymbolMgr_Client::Recv_SymbolMake(t_SYMBOL_ITEM_MSG* pSymbol)
{
	SetDualItem(pSymbol->m_nItemNumber,pSymbol->m_nGrade);
	::AddCurrentStatusMessage(255,255,255,lan->OutputMessage(4,141));
}


//심볼 업그레이드.
void CSymbolMgr_Client::Recv_SymbolUpGrade(t_SYMBOL_ITEM_MSG *pSymbol)
{
	SetDualItem(pSymbol->m_nItemNumber,pSymbol->m_nGrade);
	::AddCurrentStatusMessage(255,255,255,lan->OutputMessage(4,142));
}

void CSymbolMgr_Client::SaveBinaryFiles()
{
	/*
	FILE*	fp = NULL;
	
	int SymbolCount = m_vtSymbolList.size();
	
	if(m_bBeta)
	{	//< CSD-031030
		fp = fopen("./Itemtable_b5/Item_Symbol.bin","w+b");
	}	//> CSD-031030
	else
	{
		fp = fopen("./Itemtable/Item_Symbol.bin","w+b");
	}	
		
	if(fp)
	{
		for(int i = 0;i < SymbolCount;i++)
		{
			int nItemNumber = m_vtSymbolList[i].nItemNumber;
			char strName[65] = {NULL,};strcpy(strName,m_vtSymbolList[i].strName);
			char strHanName[65] = {NULL,};strcpy(strHanName,m_vtSymbolList[i].strHanName);
			int n1StClass = m_vtSymbolList[i].n1StClass;
			int	n2ndClass = m_vtSymbolList[i].n2ndClass;
			int nRare[5] = {0,};memcpy(nRare,m_vtSymbolList[i].nRare,sizeof(int)*5);

			int size = fwrite(&nItemNumber,sizeof(int),1,fp);
			size = fwrite(strName,sizeof(char),65,fp);
			size = fwrite(strHanName,sizeof(char),65,fp);
			size = fwrite(&n1StClass,sizeof(int),1,fp);
			size = fwrite(&n2ndClass,sizeof(int),1,fp);
			size = fwrite(nRare,sizeof(int),5,fp);
		}

		fclose(fp);
	}

	fp = NULL;
	int RareListSize = m_vtRareValueList.size();

	for(int FileCount = 0;FileCount < RareListSize;FileCount++)
	{
		char szFileName[MAX_PATH] = {NULL,};
		if(m_bBeta)
		{	//< CSD-031030
			sprintf(szFileName,"./Itemtable_b5/Item_Symbol_Ability%d.bin",FileCount+1);
		}	//> CSD-031030
		else
		{
			sprintf(szFileName,"./Itemtable/Item_Symbol_Ability%d.bin",FileCount+1);			
		}

		fp = fopen(szFileName,"w+b");
		
		if(fp)
		{
			int nGradeCount = m_vtRareValueList[FileCount].size();
			for(int i = 0;i < nGradeCount;i++)
			{
				for(int j = 0;j < SYMBOL_ITEM_MAX;j++)
				{
					int nRare = m_vtRareValueList[FileCount][i].anRare[j];
					int size = fwrite(&nRare,sizeof(int),1,fp);
				}
			}
			fclose(fp);
		}	
	}
	*/
}

void CSymbolMgr_Client::SaveBinFile_Item_Symbol()
{
	if(!m_bTextItemMode)return;
	FILE*	fp = NULL;
	
	int SymbolCount = m_vtSymbolList.size();
	
	if(m_bBeta)
	{	//< CSD-031030
		fp = fopen("./Itemtable_b5/Item_Symbol.bin","w+b");
	}	//> CSD-031030
	else
	{
		fp = fopen("./Itemtable/Item_Symbol.bin","w+b");
	}	
		
	if(fp)
	{
		for(int i = 0;i < SymbolCount;i++)
		{
			int nItemNumber = m_vtSymbolList[i].nItemNumber;
			char strName[65] = {NULL,};strcpy(strName,m_vtSymbolList[i].strName);
			char strHanName[65] = {NULL,};strcpy(strHanName,m_vtSymbolList[i].strHanName);
			int n1StClass = m_vtSymbolList[i].n1StClass;
			int	n2ndClass = m_vtSymbolList[i].n2ndClass;
			int nRare[5] = {0,};memcpy(nRare,m_vtSymbolList[i].nRare,sizeof(int)*5);

			int size = fwrite(&nItemNumber,sizeof(int),1,fp);
			size = fwrite(strName,sizeof(char),65,fp);
			size = fwrite(strHanName,sizeof(char),65,fp);
			size = fwrite(&n1StClass,sizeof(int),1,fp);
			size = fwrite(&n2ndClass,sizeof(int),1,fp);
			size = fwrite(nRare,sizeof(int),5,fp);
		}

		fclose(fp);
	}		
}

void CSymbolMgr_Client::SaveBinFile_Item_Symbol_Abilliy()
{
	if(!m_bTextAbilliyMode)return;
	FILE*	fp = NULL;

	int RareListSize = m_vtRareValueList.size();

	for(int FileCount = 0;FileCount < RareListSize;FileCount++)
	{
		char szFileName[MAX_PATH] = {NULL,};

		if(m_bBeta)
		{	//< CSD-031030
			sprintf(szFileName,"./Itemtable_b5/Item_Symbol_Ability%d.bin",FileCount+1);
		}	//> CSD-031030
		else
		{
			sprintf(szFileName,"./Itemtable/Item_Symbol_Ability%d.bin",FileCount+1);			
		}

		fp = fopen(szFileName,"w+b");
		
		if(fp)
		{
			int nGradeCount = m_vtRareValueList[FileCount].size();
			for(int i = 0;i < nGradeCount;i++)
			{
				for(int j = 0;j < SYMBOL_ITEM_MAX;j++)
				{
					int nRare = m_vtRareValueList[FileCount][i].anRare[j];
					int size = fwrite(&nRare,sizeof(int),1,fp);
				}
			}
			fclose(fp);
		}	
	}
}
