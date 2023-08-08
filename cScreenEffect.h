#ifndef _CSCREENEFFECT_H_
#define _CSCREENEFFECT_H_


#define MAX_ARG_DATA_SIZE	50
#define	MAX_EFFECT_SPRITE	20
#define	MAX_SPRITE_TYPE		50	
#define	MAX_FRAME_COUNT		40

#include <list>
#include "effect.h"

struct t_FogArg
{
	char	SpriteIndex;
	POINT	Location;
	DWORD	StartTime;
	DWORD	DelayTime;
};

struct t_BreathArg
{
	char	SpriteIndex;
	POINT	StartLocation;
	POINT	DestLocation;
	int		Direction;
	int		Power;
	DWORD	StartTime;
};

struct t_CommonArg						// 아규먼트를 공통으로 쓰기위한것.
{
	union 
	{
		char			Data[MAX_ARG_DATA_SIZE];
		t_FogArg		FogArg;
		t_BreathArg		BreathArg;
	};
};


class cParticle					// Interface Class
{
public :
	virtual cParticle*	Insert(cParticle* Arg)=0;
	virtual void		GetData(t_CommonArg* Arg)=0;
	virtual int			Draw()=0;
	virtual int			CheckAndRemove()=0;
};

typedef list<cParticle*>			tParticleList;
typedef tParticleList::iterator		tParticleListItor;

class cParticleManager
{
	tParticleList	ParticleList;
	
	cParticle*		m_pContinueParticle;
	int				m_iContinueparticleCount;
	bool            m_bDisplay; // CSD-030306
	
public:
	cParticleManager();
	~cParticleManager();
	
	void		DeleteParticle();
	void		Insert(cParticle* Particle);
	void		Draw();
	void		SetContinueParticle(cParticle* Particle,int Count);
	void		CheckContinueParticle();
public:
	void SetDisplay(bool bDisplay)
	{	//< CSD-030306
		m_bDisplay = bDisplay;
	}	//> CSD-030306
};

class cFog : public cParticle
{
	char			m_cSpriteIndex;
	char			m_cFogType;
	POINT			m_Location;
	unsigned char	m_cFrameCount;
	DWORD			StartTime;
	DWORD			DelayTime;
	
public :
	
	cFog(t_CommonArg* Arg);
	
	cParticle*	Insert(cParticle* Arg);
	void		GetData(t_CommonArg* Arg);
	void		GetArgData(cParticle* Arg,t_FogArg* ReturnArg);
	int			Draw();
	int			CheckAndRemove();
};

class cBreath : public cParticle
{
	char			m_cSpriteIndex;
	POINT			m_Start;
	POINT			m_Dest;
	char			m_cDirection;						// 방향
	unsigned char	m_ucPower;							// 진행 속도
	unsigned char	m_cFrameCount;						// 프레임 카운트
	char			m_cStatus;							// 발사 되었나 안되었나.
	DWORD			m_uiStartTime;
	
public :
	
	cBreath(t_CommonArg* Arg);
	
	cParticle*	Insert(cParticle* Arg);
	void		GetData(t_CommonArg* Arg);
	void		GetArgData(cParticle* Arg,t_CommonArg* ReturnArg);
	int			Draw();
	int 		CheckAndRemove();
};

struct tEffectFrameList
{
	unsigned char	CheckRepeat;
	unsigned char	MaxFrame;
	unsigned char	TotalFrame;
	unsigned char*	Frame;
};

class cEffectSpriteManager
{
	char*				m_pSpriteData[MAX_EFFECT_SPRITE];
	Spr					m_Sprite[MAX_SPRITE_TYPE][MAX_FRAME_COUNT];
	tEffectFrameList	m_pEffectList[MAX_EFFECT_SPRITE];
	
public :
	
	cEffectSpriteManager();
	~cEffectSpriteManager();
	
	int		LoadEFFile(int Index);
	int		LoadSprite(int Index,int FrameCount);
	Spr*	GetSpriteData(int Index,unsigned char& Frame);
	
	void	CheckAndLoadSpriteData(int Index);
};


extern cParticleManager			g_ParticleManager;
extern cEffectSpriteManager		g_EffectSpriteManager;



/*struct FogObject
{
POINT	Location;
char	FogType;				//스프라이트번호
int		FrameCount;
DWORD	StartTime;				// 시작시간
DWORD	DelayTime;				// 종료시간

  FogObject(t_CommonArg* CommonArg)
  {
		FogType		=	CommonArg->FogArg.FogType;
		Location	=	CommonArg->FogArg.Location;
		StartTime	=	CommonArg->FogArg.StartTime;
		DelayTime	=	CommonArg->FogArg.DelayTime;
		FrameCount	=	0;
		}
		
};*/

/*
typedef list<FogObject*> tFogList;
typedef tFogList::iterator FogListItor;

  class cSEffectManager
  {
  char*				FogPicBuffer[3];
  Spr					FogPic[3][30];			// Fog1 : 악령탐색 20프레임 Fog2 : 마법탐지 28프레임
  int					FogFrameCount[3];		// Fog의 프레임 카운트이다.
  int					FogStatus[3];			//현재 드로잉인지 아닌지 나타내는 변수
  int					FogDelay[3];			// 나타날 딜레이값
  
	tFogList			FogList[3];				// 포그 오브젝트의 갯수 
	
	  public :
	  cSEffectManager();
	  ~cSEffectManager();
	  
		void				ClearAllFog();
		void				ClearFog(int Index);
		
		  void				ClearFogList(int Type);
		  void				ClearFogList();
		  
			
			  int					LoadFogData(char* FileName,int FogIndex);
			  void				InsertFog(int FogType,int X,int Y,DWORD StartTime,DWORD DelayTime);
			  void				PrepareForDrawing(int Type,DWORD DelayTime,POINTS* Location);
			  void				FogDraw(int Type);
			  void				FogDraw();
			  
				};
				
				  struct DragonBreath
				  {
				  POINT	Start;
				  POINT	Dest;
				  int		Direction;						// 방향
				  int		Power;							// 진행 속도
				  int		FrameCount;						// 프레임 카운트
				  int		Status;							// 발사 되었나 안되었나.
				  
					DragonBreath(t_CommonArg* CommonArg)
					{
					Start		=	CommonArg->BreathArg.StartLocation;
					Dest		=	CommonArg->BreathArg.DestLocation;
					Direction	=	CommonArg->BreathArg.Direction;
					Power		=	CommonArg->BreathArg.Power;
					FrameCount	=	0;
					Status		=	0;
					}
					};
					
					  typedef list<DragonBreath*>	tDragonBreathList;
					  typedef tDragonBreathList::iterator	DragonBreathItor;
					  
						#define DRAGON_BREATH_FRAME_COUNT	6
						
						  class cDragonEffect
						  {
						  tDragonBreathList	DragonBreathList;
						  
							char*				BreathPicBuffer;
							Spr					BreathPic[30];			// Fog1 : 악령탐색 20프레임 Fog2 : 마법탐지 28프레임
							
							  public :
							  
								cDragonEffect();
								~cDragonEffect();
								
								  void ClearAllBreath();
								  void ClearSprite();
								  
									int LoadBreathData(char* strFileName);
									void InsertBreath(POINT tStart,POINT tDest,int tDirection,int tPower);
									void BreathPrepare(POINT tStart,POINT tDest,int Count);
									void DrawBreath();
									
};*/


#endif