// Battle.cpp: implementation of the CBattle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Battle.h"

#include <math.h>
#include "Map.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "DirectSound.h"
///////////////////////////////////////////////////////////////////////////////
// Global Member
///////////////////////////////////////////////////////////////////////////////

static int s_aAngle8[8] = 
{
	6, 5, 4, 3, 2, 1, 0, 7
};

static int s_aAngle16[16] = 
{
	2, 4, 6, 8, 10, 12, 14, 0, 3, 5, 7, 9, 11, 13, 15, 1
};

static POINT s_aPoint[9] = 
{
	{-3, 0}, {-2, 2}, {0, 3}, {2, 2}, {0, 0}, {2, -2}, {0, -3}, {-2, -2}, {3, 0}
};

static POINT s_aRound[8] = 
{
	{-5, -1}, {-3,  4}, {-1,  5}, { 4,  3}, 
	{ 5,  1}, { 3, -4}, { 1, -5}, {-4, -3}
};

static int s_aOffset[8][5] = 
{
	{0,  8, 15, 1, 7},
	{1,  9,  8, 2, 0},
	{2, 10,  9, 3, 1},
	{3, 11, 10, 4, 2},
	{4, 12, 11, 5, 3},
	{5, 13, 12, 6, 4},
	{6, 14, 13, 7, 5},
	{7, 15, 14, 0, 6}
};

static POINT s_aDistance[8][5] = 
{
	{{0, 0}, {-50,   0}, { 50,   0}, {-100,   0}, { 100,    0}},
	{{0, 0}, {-40, -40}, { 40,  40}, { -80, -80}, {  80,   80}},
	{{0, 0}, {  0,  50}, {  0, -50}, {  0,  100}, {   0, -100}},
	{{0, 0}, {-40,  40}, { 40, -40}, { -80,  80}, {  80,  -80}},
	{{0, 0}, { 50,   0}, {-50,   0}, { 100,   0}, {-100,    0}},
	{{0, 0}, { 40,  40}, {-40, -40}, {  80,  80}, { -80,  -80}},
	{{0, 0}, {  0, -50}, {  0,  50}, {  0, -100}, {   0,  100}},
	{{0, 0}, { 40, -40}, {-40,  40}, {  80, -80}, { -80,   80}}
};

static POINT s_aCircle[29] = 
{
	{-2,  2}, {-1,  2}, { 0,  2}, { 1,  2}, { 2,  2},
	{-2,  1}, {-1,  1}, { 0,  1}, { 1,  1}, { 2,  1},
	{-2,  0}, {-1,  0}, { 0,  0}, { 1,  0}, { 2,  0},
	{-2, -1}, {-1, -1}, { 0, -1}, { 1, -1}, { 2, -1},
	{-2, -2}, {-1, -2}, { 0, -2}, { 1, -2}, { 2, -2},
	{-3,  0}, { 0,  3}, { 3,  0}, { 0, -3}
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CBattle::CBattle()
{
	
}

CBattle::~CBattle()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CBattle::EffectLocal(MAGICLIST* pMagic, DWORD dwNow)
{ // �������� ����ϴ� ����Ʈ ����
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;
	
	for (int i = 0; i < 29; i++)
	{
		::InsertMagic(pCaster, pTarget,
			pMagic->next_Effect, 
			9,
			pMagic->x,
			pMagic->y,
			pMagic->tarx + (s_aCircle[i].x<<CELL), 
			pMagic->tary + (s_aCircle[i].y<<CELL),
			false, 0, false, 
			dwNow + 100*(i + 1));
	}
}

void CBattle::EffectRound(MAGICLIST* pMagic, DWORD dwNow)
{ // �����ڸ� ���δ� ����Ʈ ����               
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;
	
	for (int i = 0; i < 8; ++i)
	{
		::InsertMagic(pCaster, pTarget,
			pMagic->next_Effect,
			pMagic->magicSubNum,
			pMagic->x,
			pMagic->y,
			s_aRound[i].x<<5, 
			s_aRound[i].y<<5,
			false, 0, false, 
			dwNow + 100*(i + 1));
	}
}

void CBattle::EffectStorm(MAGICLIST* pMagic, DWORD dwNow)
{ // �ϴÿ��� �������� �������� ����Ʈ ����   
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;
	
	int nRandX = 0, nRandY = 0;
	
	for (int i = 0; i < 9; i++)
	{
		nRandX = s_aPoint[i].x<<5;
		nRandY = s_aPoint[i].y<<5;
		::InsertMagic(pCaster, pTarget,
			pMagic->next_Effect,
			pMagic->magicSubNum,
			pMagic->x + nRandX,
			pMagic->y + nRandY,
			pMagic->tarx + nRandX, 
			pMagic->tary + nRandY,
			false, 0, false, 
			dwNow + 100*(i + 1));
	}
}

void CBattle::EffectSpread(MAGICLIST* pMagic)
{ // 8�������� ������ ����Ʈ ����
	const double dUnit = 0.785398; // PI�� 4���
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	EFFECT_INFO infEffect;
	int nDir = 0, nOffX = 0, nOffY = 0;
	double dAngle = 0.0;
	
	for (int i = 0; i < 8; i++)
	{
		nDir = s_aAngle8[i];
		nOffX = 100;
		nOffY = 100;
		dAngle = dUnit*nDir;
		Coordinates(dAngle, nOffX, nOffY);
		//
		memset(&infEffect, 0, sizeof(EFFECT_INFO));
		infEffect.pCaster = pCaster;
		infEffect.pTarget = pTarget;
		infEffect.nType = pCaster->excute_MagicNum;
		infEffect.nEffect = pMagic->next_Effect;
		infEffect.nSub = pMagic->magicSubNum;
		infEffect.nDir = nDir;
		infEffect.nSrcX = pMagic->x;
		infEffect.nSrcY = pMagic->y;
		infEffect.nDstX = pMagic->x + nOffX;
		infEffect.nDstY = pMagic->y + nOffY;
		AddEffect(&infEffect);
	}
}

void CBattle::EffectShoot(MAGICLIST* pMagic, int nCount)
{ // �����ڸ� �߽����� ���������� ����Ʈ ����
	const double dUnit = 0.392699; // PI�� 8���
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	EFFECT_INFO infEffect;
	int nDir = 0, nOffX = 0, nOffY = 0;
	double dAngle = 0.0;
	
	for (int i = 0; i < nCount; i++)
	{
		nDir = s_aOffset[pCaster->direction][i];
		nOffX = 100;
		nOffY = 100;
		dAngle = dUnit*s_aAngle16[nDir];
		Coordinates(dAngle, nOffX, nOffY);
		//
		memset(&infEffect, 0, sizeof(EFFECT_INFO));
		infEffect.pCaster = pCaster;
		infEffect.pTarget = pTarget;
		infEffect.nType = pCaster->excute_MagicNum;
		infEffect.nEffect = pMagic->next_Effect;
		infEffect.nSub = pMagic->magicSubNum;
		infEffect.nDir = nDir;
		infEffect.nSrcX = pMagic->x;
		infEffect.nSrcY = pMagic->y;
		infEffect.nDstX = pMagic->x + nOffX;
		infEffect.nDstY = pMagic->y + nOffY;
		AddEffect(&infEffect);
	}
}

void CBattle::EffectMulti(MAGICLIST* pMagic, int nCount)
{ // ���� �������� ������ ������ ����Ʈ ����
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	EFFECT_INFO infEffect;
	POINT ptDistance;
	
	for (int i = 0; i < nCount; i++)
	{   
		ptDistance = s_aDistance[pCaster->direction][i];
		//
		memset(&infEffect, 0, sizeof(EFFECT_INFO));
		infEffect.pCaster = pCaster;
		infEffect.pTarget = pTarget;
		infEffect.nType = pCaster->excute_MagicNum;
		infEffect.nEffect = pMagic->next_Effect;
		infEffect.nSub = pMagic->magicSubNum;
		infEffect.nDir = pCaster->direction;
		infEffect.nSrcX = pMagic->x + ptDistance.x;
		infEffect.nSrcY = pMagic->y + ptDistance.y;
		infEffect.nDstX = pMagic->tarx + ptDistance.x;
		infEffect.nDstY = pMagic->tary + ptDistance.y;
		AddEffect(&infEffect);
	}
}

void CBattle::EffectBreath(MAGICLIST* pMagic, int nX, int nY)
{ // �극�� ����Ʈ ����
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	const int nDir = s_aOffset[pCaster->direction][0];
	const double dAngle = 3.141592/4.0*s_aAngle8[nDir];
	Coordinates(dAngle, nX, nY);
	
	::InsertMagic(pCaster, pTarget,
		pMagic->next_Effect,
		pMagic->magicSubNum,
		pMagic->x + nX,
		pMagic->y - nY,
		pMagic->tarx + nX, 
		pMagic->tary - nY,
		false, 0, false, 
		pMagic->magic_Runtime);
}

void CBattle::EffectPillar(MAGICLIST* pMagic, DWORD dwNow, const int nWidth, bool IsCkCollision) // 030519 kyo
{ // �ϴÿ��� �������� �������� ����Ʈ ����   
	const double dUnit = 0.785398; // PI�� 4���
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	int nDir = 0, nOffX = 0, nOffY = 0;
	
	switch (pCaster->sprno)
	{ // �巡���� ���� ������ ���� ����
    case 50:
    case 51:
    case 52: nDir = s_aAngle8[1];                  break;
    //default: nDir = s_aAngle8[pCaster->direction]; break;
	default: nDir = s_aAngle8[pMagic->dir]; break;// 030514 kyo //��� ���⿡�� ����Ʈ ���� ����
	}
	
	double dAngle = 0.0;
	
	for (int i = 0; i < 10; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			nOffX = rand()%nWidth + nWidth*i;
			nOffY = j*nWidth + j*rand()%nWidth;
			dAngle = dUnit*nDir;
			Coordinates(dAngle, nOffX, nOffY);

			if(IsCkCollision)
			{// 030519 kyo ����Ʈ�� ���� ���� ���� �������� �ϸ� �׸� ��´�. 
				if( IsCollision((pMagic->x + nOffX)>>5,(pMagic->y - nOffY)>>5) )
				{
					return;
				}
			}
			::InsertMagic(pCaster, pTarget,
				pMagic->next_Effect,
				pMagic->magicSubNum,
				pMagic->x,
				pMagic->y,
				pMagic->x + nOffX, 
				pMagic->y - nOffY,
				false, 0, false, 
				dwNow + 100*(i + 1));
		}
	}
}

void CBattle::EffectQuake(MAGICLIST* pMagic, int nCount, DWORD dwNow)
{ // �ϴÿ��� �������� �������� ����Ʈ ����   
	const double dUnit = 0.785398; // PI�� 4���
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	int nDir = 0, nOffX = 0, nOffY = 0;
	double dAngle = 0.0;
	
	for (int i = 0; i < nCount; i++)
	{
		nDir = s_aAngle8[pCaster->direction];
		nOffX = 120*i;
		nOffY = 0;
		dAngle = dUnit*nDir;
		Coordinates(dAngle, nOffX, nOffY);
		//
		::InsertMagic(pCaster, pTarget,
			pMagic->next_Effect,
			pMagic->magicSubNum,
			pMagic->x,
			pMagic->y,
			pMagic->x + nOffX, 
			pMagic->y - nOffY,
			false, 0, false, 
			dwNow + 200*(i + 1));
	}
}

void CBattle::EffectDrop(MAGICLIST* pMagic, int nCount, DWORD dwNow)
{ // �ϴÿ��� �������� �������� ����Ʈ ����   
	const double dUnit = 0.785398; // PI�� 4���
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	int nDir = 0, nOffX = 0, nOffY = 0;
	double dAngle = 0.0;
	
	for (int i = 0; i < nCount; i++)
	{
		nDir = s_aAngle8[pCaster->direction];
		nOffX = 120*i;
		nOffY = 0;
		dAngle = dUnit*nDir;
		Coordinates(dAngle, nOffX, nOffY);
		//
		::InsertMagic(pCaster, pTarget,
			pMagic->next_Effect,
			pMagic->magicSubNum,
			pMagic->x,
			pMagic->y,
			pMagic->x + nOffX, 
			pMagic->y - nOffY,
			false, 0, false, 
			dwNow + 200*(i + 1));
	}
}

void CBattle::EffectSpreadIrregular(MAGICLIST* pMagic, DWORD dwNow, const int nTile, const int nRan, int nLoop) // 030511 kyo 
{ // �ֺ��� nSize���ݾȿ� �ұ�Ģ������ ����Ʈ�� �Ѹ���. ������ ����
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	//static const int nSpreadIrregular[3][25] =   { //�ִ� 3������ ������.		
	const int nSpreadIrregular[3][25] =   { //�ִ� 3������ ������. 
		//{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		//{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0},
		//{0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,1},
		{0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,1},
		{0,0,1,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0}};
							
	int nXl = nTile*TILE_SIZE;	
	int nYl = nXl;//*3/4;
	int nRand, nRandShow=0;

	//������ ���̰� nXl*2, �� �ٿ� nTile�� ���( 5Ÿ�� �����̶�� 10Ÿ�Ͽ� 5�� �ѷ���)
	const int nDiv = nXl*2/nTile;	

	if( nLoop > 3 || nLoop <= 0)
	{//������ ����� 1���� ���. 
		nLoop = 1;
	}

	for( int k=0; k < nLoop; k++)
	//for( int k=0; k < 1; k++)
	{//�ð����� ������ �Ѹ��� ����. ���ȣ�⸻�� for���� ���� 
		//for (int i = 0; i < 25; i++)
		for (int i = 0; i<nTile*nTile; i++)
		{				
			nRand = rand()%5;
			nRandShow = rand()%nRan;
			if(nRandShow==0)
			{					 
				::InsertMagic(pCaster, pTarget,
					  pMagic->next_Effect,
					  pMagic->magicSubNum,
					  pMagic->x,
					  pMagic->y,
					  pMagic->x - nXl + (nDiv * (i%nTile) ), 
					  pMagic->y - nYl + (nDiv * (i/nTile) ),					 
					  false, 0, false,//	0);			  
					  dwNow + 200*nRand + k*2000);	//�ð���(0~0.1�� + 2��)�� �ΰ� ����Ʈ ���
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CBattle::AddEffect(EFFECT_INFO* pEffect)
{ // ����Ʈ �߰�
	const int nEffect = pEffect->nEffect; // ����Ʈ ��ȣ
	MAGICLIST* pTemp = NULL;
	MemAlloc(pTemp, sizeof(MAGICLIST));
	pTemp->magicSerial = pEffect->nType;
	pTemp->magicNum = nEffect;	  
	pTemp->magicSubNum = pEffect->nSub;
	pTemp->sprite_Num = g_lpET[nEffect].sprite_Num;
	pTemp->spr_Speed = g_lpET[nEffect].spr_Speed;
	pTemp->spr_Height = g_lpET[nEffect].spr_Height;
	pTemp->target_Height = pEffect->pTarget->height;
	pTemp->dir = pEffect->nDir;
	pTemp->x = pTemp->s_X = pEffect->nSrcX; 
	pTemp->y = pTemp->s_Y = pEffect->nSrcY; 
	pTemp->tarx = pTemp->t_X = pEffect->nDstX;
	pTemp->tary = pTemp->t_Y = pEffect->nDstY;
	pTemp->map_Offset.x = g_Map.x;
	pTemp->map_Offset.y = g_Map.y;
	pTemp->trans_Type = g_lpET[nEffect].trans_Type;
	pTemp->trans_Level = g_lpET[nEffect].trans_Level;
	pTemp->object_Num = g_lpET[nEffect].object_Num;
	pTemp->fallow = g_lpET[nEffect].fallow;
	pTemp->firePattern = false;							   
	pTemp->lpChar_Own = pEffect->pCaster;
	pTemp->lpChar_Target = pEffect->pTarget;
	pTemp->Caster_id = pEffect->pCaster->id;
	pTemp->Target_id = pEffect->pTarget->id;
	pTemp->next_Effect = g_lpET[nEffect].next_Effect;
	pTemp->impect_Effect_Num = g_lpET[nEffect].impect_Effect_Num;
	pTemp->link_Frame = g_lpET[nEffect].link_Frame;
	pTemp->end_Time = g_lpET[nEffect].end_Time;
	pTemp->sound_LoopCount = g_lpET[nEffect].sound_loop;
	pTemp->flag = 0;
	pTemp->next_Flag = 0;
	pTemp->magic_Runtime = 0;
	pTemp->failed = false;
	// ���� �ε�
	if (g_lpET[nEffect].sound_Num != 0)
	{
		const int nSound = g_lpET[nEffect].sound_Num%1000;
		
		if (pEffect->pCaster == Hero)
		{
			::PlayListAutoSounds(g_lpET[nEffect].sound_Num, 0, 
				20 - (g_lpET[nEffect].sound_Volume/5), 0);
		}
		else
		{
			const int	nDistX = (pEffect->pCaster->x - Hero->x);
			const int nDistY = (pEffect->pCaster->y - Hero->y);
			::PlayListAutoSounds(g_lpET[nEffect].sound_Num, nDistX, nDistY, 0);
		}
	}
	
	if ((effSpr_Count[pTemp->sprite_Num]==0 || Effspr[pTemp->sprite_Num].EffSpr->img==NULL) && pTemp->sprite_Num != 0)
	{ // ó�� �ε��Ǵ� ���(magicNum�� 0~499����)	
		char buf[30];
		strcpy(buf, g_lpET2[nEffect].sprite_Name);	//020808 YGI 
		strcat(buf, ".dat");

#ifdef _EFFECT_DATA_USING_DIVIDED_FILE // 030821 kyo //kyo
		::LoadEffectSpriteDivFile(g_lpET[nEffect].sprite_Num);
#else
		::LoadEffectSprite(g_lpET[nEffect].sprite_Num, buf);
#endif
		
		strcpy(buf, g_lpET2[nEffect].sprite_Name);		//020808 YGI 
		::LoadEffectAnimation(g_lpET[nEffect].sprite_Num, buf);
	}
	//����Ʈ ���� ī��Ʈ ����
	++effSpr_Count[g_lpET[nEffect].sprite_Num];	
	::InsertMagicList(&g_lpML, pTemp);
}

void CBattle::ClearCrash()
{ //< CSD-021021
	LPCHARACTER pTemp = Hero;
	
	do
	{
		pTemp->bCrash = false;
		pTemp = pTemp->lpNext;
	}
	while (pTemp != NULL);
} //> CSD-021021

///////////////////////////////////////////////////////////////////////////////
// Private Method
///////////////////////////////////////////////////////////////////////////////

void CBattle::Coordinates(double dAngle, int& rX, int& rY)
{ // (0, 0) ��ǥ�� �������� ������ŭ ��ǥ��ȯ
	int nX = rX, nY = rY;
	rX = nX*cos(dAngle) - nY*sin(dAngle);
	rY = nX*sin(dAngle) + nY*cos(dAngle);
}
