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
{ // 여러개를 출력하는 이펙트 형태
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
{ // 시전자를 감싸는 이펙트 형태               
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
{ // 하늘에서 여러개가 떨어지는 이펙트 형태   
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
{ // 8방향으로 나가는 이펙트 형태
	const double dUnit = 0.785398; // PI를 4등분
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
{ // 시전자를 중심으로 퍼져나가는 이펙트 형태
	const double dUnit = 0.392699; // PI를 8등분
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
{ // 일정 간격으로 여러개 나가는 이펙트 형태
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
{ // 브레스 이펙트 형태
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
{ // 하늘에서 여러개가 떨어지는 이펙트 형태   
	const double dUnit = 0.785398; // PI를 4등분
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	int nDir = 0, nOffX = 0, nOffY = 0;
	
	switch (pCaster->sprno)
	{ // 드래곤인 경우는 강제로 방향 설정
    case 50:
    case 51:
    case 52: nDir = s_aAngle8[1];                  break;
    //default: nDir = s_aAngle8[pCaster->direction]; break;
	default: nDir = s_aAngle8[pMagic->dir]; break;// 030514 kyo //사람 방향에서 이펙트 찍은 방향
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
			{// 030519 kyo 이팩트가 갈수 없는 곳에 찍힐려고 하면 그만 찍는다. 
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
{ // 하늘에서 여러개가 떨어지는 이펙트 형태   
	const double dUnit = 0.785398; // PI를 4등분
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
{ // 하늘에서 여러개가 떨어지는 이펙트 형태   
	const double dUnit = 0.785398; // PI를 4등분
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
{ // 주변에 nSize간격안에 불규칙적으로 이펙트를 뿌린다. 찌지미 형태
	LPCHARACTER pCaster = pMagic->lpChar_Own;
	LPCHARACTER pTarget = pMagic->lpChar_Target;
	if (pCaster == NULL || pTarget == NULL) 	return;  
	
	//static const int nSpreadIrregular[3][25] =   { //최대 3번까지 찌진다.		
	const int nSpreadIrregular[3][25] =   { //최대 3번까지 찌진다. 
		//{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		//{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0},
		//{0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,1},
		{0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,1},
		{0,0,1,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0}};
							
	int nXl = nTile*TILE_SIZE;	
	int nYl = nXl;//*3/4;
	int nRand, nRandShow=0;

	//가로의 길이가 nXl*2, 한 줄에 nTile개 출력( 5타일 간격이라면 10타일에 5개 뿌려짐)
	const int nDiv = nXl*2/nTile;	

	if( nLoop > 3 || nLoop <= 0)
	{//범위를 벗어나면 1번만 출력. 
		nLoop = 1;
	}

	for( int k=0; k < nLoop; k++)
	//for( int k=0; k < 1; k++)
	{//시간차를 가지고 뿌리기 위해. 재귀호출말고 for문을 돌림 
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
					  dwNow + 200*nRand + k*2000);	//시간차(0~0.1초 + 2초)를 두고 이펙트 출력
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

void CBattle::AddEffect(EFFECT_INFO* pEffect)
{ // 이펙트 추가
	const int nEffect = pEffect->nEffect; // 이펙트 번호
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
	// 사운드 로딩
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
	{ // 처음 로딩되는 경우(magicNum는 0~499까지)	
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
	//이펙트 누적 카운트 더함
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
{ // (0, 0) 좌표를 기준으로 각도만큼 좌표변환
	int nX = rX, nY = rY;
	rX = nX*cos(dAngle) - nY*sin(dAngle);
	rY = nX*sin(dAngle) + nY*cos(dAngle);
}
