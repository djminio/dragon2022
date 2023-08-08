// DualMgr.cpp: implementation of the CDualMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DualMgr.h"

#include "Char.h"
#include "Effect.h"
#include "Network.h"
#include "CharDataTable.h"
#include "SmallMenuSet.h"
#include "SymbolMgr_Client.h"

extern CHARACTERLIST g_CharacterList;
extern int UseItemByRbutton(POS pos_s, ItemAttr &item_attr);
extern CSymbolMgr_Client	g_SymBolMgr;

//class	CSymbolMgr_Client;

CDualMgr g_mgrDual;
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualMgr::CDualMgr()
{
	m_nPara = 0;
	m_nPosX = 0;
	m_nPosY = 0;
}

CDualMgr::~CDualMgr()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CDualMgr::Load(const char* pFile)
{
	/*
	ifstream fin(pFile, ios::in|ios::binary);

	for (int i = 0; i < 5; ++i)
	{
		for  (int j = 0; j < 15; ++j)
		{
			fin.read((char*)(&m_aBank[i][j]), sizeof(HERO_NAME));
		}
	}
  
	fin.close();
	*/
	ifstream fin(pFile);
	string strLine;
	int i = 0, j = 0;
	
	while (getline(fin, strLine, fin.widen('\n')))
	{
		int nBegin = 0, nEnd = strLine.find("\t", 0); 
		
		if (i != atoi(strLine.substr(nBegin , nEnd - nBegin).c_str()))
		{
			i = atoi(strLine.substr(nBegin , nEnd - nBegin).c_str());
			j = 0;
		}
		// ���� Ŭ���� �о����
		m_aBank[i][j].nBefore = i;
		// ������ Ŭ���� �о����
		nBegin = nEnd + 1;
		nEnd = strLine.find("\t", nBegin);
		if (nEnd == string::npos)  continue;
		m_aBank[i][j].nAfter = atoi(strLine.substr(nBegin , nEnd - nBegin).c_str());
		// Ŭ���� �ܰ� �о����
		nBegin = nEnd + 1;
		nEnd = strLine.find("\t", nBegin);
		if (nEnd == string::npos)  continue;
		m_aBank[i][j].nStep = atoi(strLine.substr(nBegin , nEnd - nBegin).c_str());
		// ��Ī�� ��� ������ ���� �о����
		nBegin = nEnd + 1;
		nEnd = strLine.find("\t", nBegin);
		if (nEnd == string::npos)  continue;
		m_aBank[i][j].nLimit = atoi(strLine.substr(nBegin , nEnd - nBegin).c_str());
		// Ŭ���� ��Ī �о����
		nBegin = nEnd + 1;
		nEnd = strLine.find("\t", nBegin);
		if (nEnd == string::npos)  continue;
		m_aBank[i][j].strName = string(strLine, nBegin , nEnd - nBegin);
		// ��Ī�� ���� ���� �о����
		nBegin = nEnd + 1;
		m_aBank[i][j].strExplain = string(strLine, nBegin , strLine.length() - nBegin);    
		++j;
	}
	
	fin.close();
}

void CDualMgr::Save(const char* pFile)
{
/*
ofstream fout(pFile, ios::out|ios::binary);

  for (int i = 0; i < 5; ++i)
  {
  for  (int j = 0; j < 15; ++j)
  {
  fout.write((char*)(&m_aBank[i][j]), sizeof(HERO_NAME));
  }
  }
  
	fout.close();
	*/
	int i = 12, j = 8920;
	string strTemp = "Test best!";
	ofstream fout(pFile);
	fout << i << j << strTemp << endl;  
	fout.close();
}

string CDualMgr::GetClassName(int nBefore, int nAfter, int nStep, int nLevel) const
{
	switch (nStep)
	{
    case 0: return (nLevel >= 100) ? "Crossing Class":m_aBank[nBefore][(nLevel - 1)/10].strName;
    case 1:
    case 2:
    case 3: 
    case 4: return m_aBank[nBefore][nAfter + 10].strName;
    case 5: return "Dragon Slayer";
	}
	
	return "";
}

string CDualMgr::GetDulaExplain(int nBefore, int nAfter) const
{
	return m_aBank[nBefore][nAfter + 10].strExplain; 
}

void CDualMgr::SendDualEnable(LPCHARACTER pMaster, BYTE nPara, BYTE nX, BYTE nY)
{
	if (pMaster == NULL)  return;
	
	POS pos;
	// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
	::SetItemPos(INV, nPara, nY, nX, &pos);
	// ������ų ������ ȹ�� ������ Ȯ��
	ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];
	
	if (rItemAttr.item_no == 4028)
	{ // Packet ����
		t_packet packet;
		packet.h.header.type = CMD_DUAL_ENABLE;
		packet.h.header.size = sizeof(t_client_dual_enable);
		packet.u.combat.client_combat_obtain.idMaster = WORD(pMaster->id);
		packet.u.combat.client_combat_obtain.nPara = nPara;
		packet.u.combat.client_combat_obtain.nPosX = nX;
		packet.u.combat.client_combat_obtain.nPosY = nY;
		::QueuePacket(&packet, 1);  
	} 
}

void CDualMgr::SendDualChange(LPCHARACTER pMaster, BYTE nNext)
{
	if (pMaster == NULL)  return;
	
	POS pos;
	// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
	::SetItemPos(INV, m_nPara, m_nPosY, m_nPosX, &pos);
	// ������ų ������ ȹ�� ������ Ȯ��
	ItemAttr& rItemAttr = InvItemAttr[m_nPara][m_nPosY][m_nPosX];
	
	if (rItemAttr.item_no == 4028)
	{ // Packet ����
		t_packet packet;
		packet.h.header.type = CMD_DUAL_CHANGE;
		packet.h.header.size = sizeof(t_client_dual_change);
		packet.u.dual.client_dual_change.idMaster = WORD(pMaster->id);
		packet.u.dual.client_dual_change.nPara = m_nPara;
		packet.u.dual.client_dual_change.nPosX = m_nPosX;
		packet.u.dual.client_dual_change.nPosY = m_nPosY;
		packet.u.dual.client_dual_change.nNext = nNext;
		::QueuePacket(&packet, 1);  
	}
}

void CDualMgr::SendDualDivide(LPCHARACTER pMaster, BYTE nNext)
{
	if (pMaster == NULL)  return;
	
	t_packet packet;
	packet.h.header.type = CMD_DUAL_DIVIDE;
	packet.h.header.size = sizeof(t_client_dual_divide);
	packet.u.dual.client_dual_divide.idMaster = WORD(pMaster->id);
	packet.u.dual.client_dual_divide.nNext = nNext;
	::QueuePacket(&packet, 1);  
}

void CDualMgr::RecvDualEnable(t_server_dual_enable* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;
	
	const BYTE nPara = pPacket->nPara;
	const BYTE nX = pPacket->nPosX;
	const BYTE nY = pPacket->nPosY;
	// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	// ������ų ������ ȹ�� ������ Ȯ��
	ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];
	
	if (rItemAttr.item_no == 4028)
	{ // Packet ����
		m_nPara = nPara;
		m_nPosX = nX;
		m_nPosY = nY;
		::CallDualInterFace(MT_DUAL_CHANGE);
	}
}

void CDualMgr::RecvDualChange(t_server_dual_change* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;
	
	if (pDual == Hero)
	{
		m_nPara = 0;
		m_nPosX = 0;
		m_nPosY = 0;
		
		pDual->aStepInfo[CLS_STEP] = 1;              // ��� Ŭ���� �ܰ� ���
		pDual->aStepInfo[DUAL_CLS] = pPacket->nDual; // ��� Ŭ���� ����
		SCharacterData.nCharacterAbility[FAME] = pPacket->dwFame; // ������ fame ���� �缳��
		SCharacterData.nCharacterAbility[STR] = pPacket->wStr;
		SCharacterData.nCharacterAbility[DEX] = pPacket->wDex;
		SCharacterData.nCharacterAbility[CON] = pPacket->wCon;
		SCharacterData.nCharacterAbility[WIS] = pPacket->wWis;
		SCharacterData.nCharacterAbility[INT_] = pPacket->wInt;
		SCharacterData.nCharacterAbility[CHA] = pPacket->wCha;
		SCharacterData.nCharacterAbility[MOVP] = pPacket->wMovep;
		SCharacterData.nCharacterAbility[ENDU] = pPacket->wEndu;
		SCharacterData.nCharacterAbility[MOR] = pPacket->wMor;
		SCharacterData.nCharacterAbility[LUCK] = pPacket->wLuck;
		SCharacterData.nCharacterAbility[WSPS] = pPacket->wWsPs;
		SCharacterData.LvUpPoint = pPacket->wPoint;
		
		
	}
	
	::InsertMagic(pDual, pDual, 443, -1, 0, 0, 0, 0);
}

void CDualMgr::RecvDualDivide(t_server_dual_divide* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;
	
	if (pDual == Hero)
	{
		
		++Hero->aStepInfo[CLS_STEP];                // ��� Ŭ���� �ܰ� ���
		Hero->aStepInfo[DUAL_CLS] = pPacket->nDual; // ��� Ŭ���� ����
		SCharacterData.nCharacterAbility[STR] = pPacket->wStr;
		SCharacterData.nCharacterAbility[DEX] = pPacket->wDex;
		SCharacterData.nCharacterAbility[CON] = pPacket->wCon;
		SCharacterData.nCharacterAbility[WIS] = pPacket->wWis;
		SCharacterData.nCharacterAbility[INT_] = pPacket->wInt;
		SCharacterData.nCharacterAbility[CHA] = pPacket->wCha;
		SCharacterData.nCharacterAbility[MOVP] = pPacket->wMovep;
		SCharacterData.nCharacterAbility[ENDU] = pPacket->wEndu;
		SCharacterData.nCharacterAbility[MOR] = pPacket->wMor;
		SCharacterData.nCharacterAbility[LUCK] = pPacket->wLuck;
		SCharacterData.nCharacterAbility[WSPS] = pPacket->wWsPs;
		SCharacterData.LvUpPoint = pPacket->wPoint;
		
	}
	
	::InsertMagic(pDual, pDual, 443, -1, 0, 0, 0, 0);
}

int CDualMgr::GetAbility(BYTE nType)
{	// �����Ƽ �ִ밪 ��������
	const int nClass = Hero->class_type;
	const int nStep = Hero->aStepInfo[CLS_STEP];
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(nClass, nStep);
	
	switch (nType)
	{
    case STR:  return pLimit->GetStr();  // ��
    case CON:	 return pLimit->GetCon();  // ü��
    case DEX:	 return pLimit->GetDex();  // ��ø
    case WIS:	 return pLimit->GetWis();  // ����
    case INT_: return pLimit->GetInt();	 // ����
    case CHA:	 return pLimit->GetCha();  // �ŷ�
    case MOVP: return pLimit->GetMovp(); // �̵���
    case ENDU: return pLimit->GetEndu(); // �γ�
    case MOR:	 return pLimit->GetMor();  // ���
    case LUCK: return pLimit->GetLuck(); // ���
    case WSPS:
		{
			switch (SCharacterData.nCharacterData[SPELL])
			{
			case WIZARD_SPELL: return pLimit->GetWs(); // ������
			case PRIEST_SPELL: return pLimit->GetPs(); // ������
			}
		}
	}
	
	return 0;
}