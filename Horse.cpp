#include "stdafx.h"
#include <io.h>

#include "gameproc.h"
#include "NetWork.h"
#include "menu.h"
#include "SmallMenu.h"
#include "map.h"
#include "LineCommand.h"
#include "tool.h"
#include "hong_sub.h"
#include "Horse.h"
#include "char.h"
#include "Battle.h"
using namespace battle;

#define CHECKSUM_ERROR_ANIMATIONDATA	20008

cHorseRider	g_HorseRider;

extern DWORD		g_curr_time;

char HorseWeaponConvertTable[2][38]=
{																									                     // 18번 부터 깃발  	
	{ 0, 0,-1,-1,-1, 4, 4, 6, 6,10, 8,10, 8, 9, 9,11,11,12,12,13,13,-1,-1,-1,-1,-1,17,17,17,17,17,17,18,19,20,21,22,23},
	{ 0, 0,-1,-1,-1, 4, 4, 5, 5, 6, 6,10, 8, 9, 9,10, 8,11,11,12,12,13,13,-1,-1,-1,17,17,17,17,17,17,18,19,20,21,22,23}	
};	// Sprite File

char HorseEffectWeaponConvertTable[2][10] =
{	//< CSD-031020																										                     // 18번 부터 깃발  	
	{-1, 0, 12, -1, 6, 9, 10, 11, -1, -1},
	{-1, 0, 12, 13, 6, 9, 10, 11, 13, 5}	
};	//> CSD-031020

extern int			convert565to555_LoadSprite(Spr *sp);
extern void			PutCompressedImage(int x, int y, Spr *sp );
extern BOOL			PutDeadEffectBottom(LPCHARACTER ch);
extern void			PutCompressedImageCharRGB( int x, int y, Spr *sp, DWORD RGB );
extern void			PutCompressedImageBlendRGB( int x, int y, Spr *sp, DWORD RGB );
extern void			PutCompressedImageFX(int x, int y, Spr *sp, DWORD alpha, DWORD op);
extern void			PutCompressedImageVoid(int x, int y, Spr *sp );
extern void			PutCompressedImageShadow( int x, int y, Spr *sp );
extern int			IsDead( LPCHARACTER ch );






void InitHorseAniTable()
{
	g_HorseRider.LoadHorseRiderAniMationTable(0,0,"./char/hrAniTable0000.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(0,1,"./char/hrAniTable0100.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(0,2,"./char/hrAniTable0200.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(1,0,"./char/hrAniTable0001.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(1,1,"./char/hrAniTable0101.txt");
	g_HorseRider.LoadHorseRiderAniMationTable(1,2,"./char/hrAniTable0201.txt");
}

cHorseRider::cHorseRider()
{
}

cHorseRider::~cHorseRider()
{
	ClearSpriteAll();
}

int cHorseRider::LoadRiderSprite(int SexType,int SpriteKind)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if( RiderPicBuffer[SexType][SpriteKind] != NULL )
	{
		return FALSE;
	}

	sprintf(tempFileName,"./char/HR%03d%03d.spr",SexType,SpriteKind);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc( RiderPicBuffer[SexType][SpriteKind], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &RiderPic[SexType][SpriteKind][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_RIDER_SPR_DATA_SUB);
			}						
			fread( RiderPicBuffer[SexType][SpriteKind] + nOff, s->size, 1, fp);
			s->img = RiderPicBuffer[SexType][SpriteKind] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}

int cHorseRider::LoadRiderBandSprite(int SexType,int SpriteKind)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if( RiderBandPicBuffer[SexType][SpriteKind] != NULL )
	{
		return FALSE;
	}

	sprintf(tempFileName,"./char/RB%03d%03d.spr",SexType,SpriteKind);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc( RiderBandPicBuffer[SexType][SpriteKind], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &RiderBandPic[SexType][SpriteKind][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_RIDER_BAND_SPR_DATA_SUB);
			}						
			fread( RiderBandPicBuffer[SexType][SpriteKind] + nOff, s->size, 1, fp);
			s->img = RiderBandPicBuffer[SexType][SpriteKind] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}

int cHorseRider::LoadRiderMantleSprite(int SexType)					// Mantle Data
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if( RiderMantlePicBuffer[SexType]!=NULL )
	{
		return FALSE;
	}

	sprintf(tempFileName,"./char/RM%03d.spr",SexType);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc(RiderMantlePicBuffer[SexType], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &RiderMantlePic[SexType][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_RIDER_MANTLE_SPR_DATA_SUB);
			}						
			fread( RiderMantlePicBuffer[SexType] + nOff, s->size, 1, fp);
			s->img = RiderMantlePicBuffer[SexType] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}


int cHorseRider::LoadRiderGradeSprite(int SexType,int SpriteKind,int GradeKind)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if( RiderGradePicBuffer[SexType][SpriteKind][GradeKind] != NULL )
	{
		return FALSE;
	}

	sprintf(tempFileName,"./char/RGR%02d%02d%02d.spr",SexType,SpriteKind,GradeKind);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc(RiderGradePicBuffer[SexType][SpriteKind][GradeKind], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &RiderGradePic[SexType][SpriteKind][GradeKind][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_RIDER_GRADE_SPR_DATA_SUB);
			}						
			fread( RiderGradePicBuffer[SexType][SpriteKind][GradeKind] + nOff, s->size, 1, fp);
			s->img = RiderGradePicBuffer[SexType][SpriteKind][GradeKind] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}


int cHorseRider::LoadHorseSprite(int SpriteKind)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if( HorsePicBuffer[SpriteKind] != NULL )
	{
		return FALSE;
	}

	sprintf(tempFileName,"./char/H%03d.spr",SpriteKind);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc( HorsePicBuffer[SpriteKind], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &HorsePic[SpriteKind][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_HORSE_SPR_DATA_SUB);
			}						
			fread( HorsePicBuffer[SpriteKind] + nOff, s->size, 1, fp);
			s->img = HorsePicBuffer[SpriteKind] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}

int cHorseRider::LoadHorseSaddleSprite(int SpriteKind)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if( HorseSaddlePicBuffer[SpriteKind] != NULL )
	{
		return FALSE;
	}

	sprintf(tempFileName,"./char/HS%03d.spr",SpriteKind);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc( HorseSaddlePicBuffer[SpriteKind], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &HorseSaddlePic[SpriteKind][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_HORSE_SADDEL_SPR_DATA_SUB);
			}						
			fread( HorseSaddlePicBuffer[SpriteKind] + nOff, s->size, 1, fp);
			s->img = HorseSaddlePicBuffer[SpriteKind] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}

int cHorseRider::LoadHorseGradeSprite(int HorseKind,int GradeKind)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if( HorseGradePicBuffer[HorseKind][GradeKind]!= NULL )
	{
		return FALSE;
	}

	sprintf(tempFileName,"./char/HGR%02d%03d.spr",HorseKind,GradeKind);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc(  HorseGradePicBuffer[HorseKind][GradeKind], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &HorseGradePic[HorseKind][GradeKind][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_HORSE_GRADE_SPR_DATA_SUB);
			}						
			fread(HorseGradePicBuffer[HorseKind][GradeKind] + nOff, s->size, 1, fp);
			s->img = HorseGradePicBuffer[HorseKind][GradeKind] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}

void cHorseRider::LoadHorseData(LPCHARACTER ch)
{
	if (ch->HorseNo>0)
	{
		int Sex=CheckSex(ch);
		int RiderKind=CheckRiderKind(ch);
		int RiderGrade=CheckRiderGrade(ch);
		int HorseKind=CheckHorseKind(ch);
		int HorseGrade=CheckHorseGrade(ch);
		int WeaponKind=CheckWeaponKind(ch);
		int GuardKind=CheckGuardKind(ch);
		
		if (RiderKind>=0) 
		{
			LoadRiderSprite(Sex,RiderKind);
			LoadRiderBandSprite(Sex,RiderKind);
		}
		if (RiderGrade>=0) LoadRiderGradeSprite(Sex,RiderKind,RiderGrade);
		if (HorseKind>=0) 
		{
			LoadHorseSprite(HorseKind);
			LoadHorseSaddleSprite(HorseKind);
		}
		if (HorseGrade>=0) LoadHorseGradeSprite(HorseKind,HorseGrade);
		if (WeaponKind>=0) LoadWeaponSprite(Sex,WeaponKind);
		if (GuardKind>=0) LoadGuardSprite(Sex,GuardKind);
		if (ch->mantle) LoadRiderMantleSprite(Sex);
	}
}

int	cHorseRider::CheckWeaponSpriteData(int SexType,int SpriteKind)
{
	if (SpriteKind==13)
	{
		if (SexType==HTYPE_MAN)
		{
			if( WeaponPicBuffer[SexType][SpriteKind] != NULL )
			{
				return FALSE;
			}
		}
		else
		{
			if( WeaponPicBuffer[SexType][SpriteKind] != NULL )
			{
				return FALSE;
			}
		}
		
	}
	else
	{
		if( WeaponPicBuffer[SexType][SpriteKind] != NULL )
		{
			return FALSE;
		}
	}
	return TRUE;
}

void cHorseRider::GetWeaponFileName(int SexType,int SpriteKind,char* FileName)
{
	if (SpriteKind==13)
	{
		if (SexType==HTYPE_WOMAN)
		{
			SpriteKind++;
		}
		sprintf(FileName,"./char/W001%03d.spr",SpriteKind);
	}
	else
	{
		sprintf(FileName,"./char/W001%03d.spr",SpriteKind);
	}
}


int cHorseRider::LoadWeaponSprite(int SexType,int SpriteKind)	// SpriteKind must be Converted..
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if(WeaponPicBuffer[SexType][SpriteKind]!=NULL)
	{
		return FALSE;
	}
	
	sprintf(tempFileName,"./char/W%03d%03d.spr",SexType,SpriteKind);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc( WeaponPicBuffer[SexType][SpriteKind], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &WeaponPic[SexType][SpriteKind][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_WEAPON_SPR_DATA);
			}						
			fread(WeaponPicBuffer[SexType][SpriteKind] + nOff, s->size, 1, fp);
			s->img = WeaponPicBuffer[SexType][SpriteKind] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}

int cHorseRider::LoadGuardSprite(int SexType,int SpriteKind)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   i,j;
	Spr*  s;
	int length;
	char tempFileName[MAX_PATH];

	if( GuardPicBuffer[SexType]!= NULL )
	{
		return FALSE;
	}

	sprintf(tempFileName,"./char/w%03d017.spr",SexType);
	
	fp=Fopen(tempFileName,"rb" );
	if (fp==NULL)
	{
		return FALSE;
	}
			
	fread( &no, 2,1, fp);
	fseek( fp, no * 4, SEEK_CUR ); 
	length = sprfilelength = _filelength( _fileno( fp) ) - no * 4 * 2 - 2;

	MemAlloc( GuardPicBuffer[SexType], sprfilelength );
	for (i=0;i<8;i++)
	{			
		for (j=0;j<75;j++)
		{
			s = &GuardPic[SexType][i][j];
			fread( &size, 4,1, fp);
			if( size== 0 )	
			{				
				s->xl = 0;	
				s->yl = 0;	
				s->size = 0;
				s->img = NULL;
				continue;	
			}				
			fread( &(s->xl),	sizeof( short ), 1, fp);
			fread( &(s->yl),	sizeof( short ), 1, fp);
			fread( &(s->ox),	sizeof( short ), 1, fp);
			fread( &(s->oy),	sizeof( short ), 1, fp);
			fread( &(s->size),	sizeof( unsigned int ), 1, fp);
			
			if( nOff + s->size >= sprfilelength ) 
			{						
				JustMsg( "%s File Error !!",tempFileName);
				ExitApplication(EA_LOAD_GUARD_SPR_DATA);
			}						
			fread(GuardPicBuffer[SexType] + nOff, s->size, 1, fp);
			s->img = GuardPicBuffer[SexType] + nOff;
			
			convert565to555_LoadSprite( s );
			
			nOff += size;		
		}
	}		
	fclose(fp);	
	return TRUE;
}

void cHorseRider::LoadHorseRiderAniMationTable(int Sex,int HorseNo,char* FileName)
{
	FILE *fp;
	int i, no, count;
	char temp[ FILENAME_MAX];
	int x, y, attackframe, wave, height, effect;
	static int tempsprno;
	char gabage[20];
	char filename[MAX_PATH];

								
	if( GetSysInfo( SI_GAME_MAKE_MODE ) )	fp = Fopen( FileName, "rt" );
	else fp = NULL;
	if( fp )
	{							
		for( i = 0 ; i < 50 ; i ++)
		{			
			count =0;
	AGAIN_:		
			if( fgets( temp, FILENAME_MAX, fp ) == NULL ) break;
												
			if( temp[0] == '#' || temp[0] == ' ' || temp[0] == 10 || temp[0] == '\n' )  
			{
				if( temp[0] == '#' )
				{
					char *p;
					if( (p = strchr( temp, 10 )) ) *p = 0;
#ifdef _DEBUG
					strncpy( AnimationName[i], temp + 1, 100 );
#endif
				}
				
				goto AGAIN_;
			}
			else goto PROCESS__;
				
			while( fgets( temp, FILENAME_MAX, fp ) != NULL )
			{	
	PROCESS__:				
				if( temp[ 0] == ' ' || temp[0] == '\n' || temp[0] == '\r' )
				{
					HorseRiderAniTable[Sex][HorseNo][i].nCount = count;
					break;
				}
				
				sscanf( temp, "%d %d %d %d %d %d %d ", &no, &x, &y, &attackframe, &wave, &height, &effect );
	//			no = atoi( temp ) -1;
				HorseRiderAniTable[Sex][HorseNo][i].cFrame[ count]	= no-1;
				HorseRiderAniTable[Sex][HorseNo][i].dx[ count]		=  x;
				HorseRiderAniTable[Sex][HorseNo][i].dy[ count]		=  y;
				HorseRiderAniTable[Sex][HorseNo][i].attackf[ count]	=  attackframe;
				HorseRiderAniTable[Sex][HorseNo][i].wave[ count]		=  wave;
				HorseRiderAniTable[Sex][HorseNo][i].height[ count]	=  height;
				HorseRiderAniTable[Sex][HorseNo][i].effect[ count]	=  effect;
				count++;
				HorseRiderAniTable[Sex][HorseNo][i].nCount = count;
			}	
		}		
		fclose(fp);
			
		int dist = 0;
			
		sprintf( filename, "./CHAR/HRAniTable%02d%02d.bin",HorseNo,Sex);
		fp = Fopen( filename, "wb" );
		if( fp )
		{
			fwrite( gabage, 20,1, fp );
			char *tt = (char *)HorseRiderAniTable[Sex][HorseNo];
			char crc = 0;
			for( i = 0 ; i < sizeof( PCANIMATIONTABLE ) * MAX_ANIMATION ; i ++)
			{
				crc += *tt;
				tt ++;
			}
			fwrite( &crc, 1,1, fp );
			fwrite( HorseRiderAniTable[Sex][HorseNo], sizeof( PCANIMATIONTABLE ), MAX_ANIMATION, fp);
			fclose(fp);
		}
	}
	else 
	{
		sprintf( filename, "./CHAR/HRAniTable%02d%02d.bin",HorseNo,Sex);
		fp = Fopen( filename, "rb" );
		char *tt = (char *)HorseRiderAniTable[Sex][HorseNo];
		char crc = 0, crc1;
		if( fp )
		{
			fseek( fp, 20, SEEK_SET );
			fread( &crc1, 1,1, fp );
			fread( HorseRiderAniTable[Sex][HorseNo], sizeof( PCANIMATIONTABLE ), MAX_ANIMATION, fp);
			for( i = 0 ; i < sizeof( PCANIMATIONTABLE ) * MAX_ANIMATION ;  i ++)
			{
				crc += *tt;
				tt ++;
			}
			fclose(fp);

			if( abs(crc1 - crc) )
			{
				//if( SysInfo.gamemakemode ) JustMsg( "CHECKSUM_ERROR_ANIMATIONDATA : ch->sprno = %d ",  ch->sprno );
				CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;
			}

		}
		else 
		{
			if( GetSysInfo( SI_GAME_MAKE_MODE ) ) JustMsg( "CheckNumError : HorseTable");	// 010612 KHS
			CheckSumError = CHECKSUM_ERROR_ANIMATIONDATA;			
		}
	}
}

void cHorseRider::CheckAnimation(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	if (ch->nCurrentFrame.GetDecrypted()>=HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount )
	{	
		ch->nCurrentFrame.PutEncrypted(0);
	}
}

void cHorseRider::CheckDeathAnimation(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	if (ch->nCurrentFrame.GetDecrypted()>=HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount )
	{	
		ch->nCurrentFrame.PutEncrypted(HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount-1);
		if (ch->nCurrentAction==ACTION_DIE)
		{
#ifdef _DEBUG
			if( SysInfo.notconectserver )
			{
				if( ch->dieani == 0 ) // 죽었으면..
				{
					ch->diedelay	=  3;	// 사라지는동작을 천천히...
					ch->dieani		= 80;	// 32보다 작으면  사라진다.
					ch->dieanitype  =  0;	// 
				}
			}
			else	//서버와 연결된 경우
#endif
			{
				if( ch->blood_Set == false && ch->dieani == 0 )
				{
					ch->diedelay	= 3;
					ch->dieani		= 80;
					ch->blood_Set	= true;
				}
			}
		}
	}
}

void cHorseRider::CheckMoveAnimation(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	if (ch->nCurrentFrame.GetDecrypted()>=HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount )
	{
		if( !IsDead(ch) && (ch->pathcount > 2 || ch->moveFlag == TRUE) )
		{
			CharDoAni_Moving(ch, ch->movetype );
		}
		else
		{
			ch->nCurrentFrame.PutEncrypted(0);
		}
	}
}


void cHorseRider::CheckAttackAnimation(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	if (ch->nCurrentFrame.GetDecrypted()>=HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].nCount )
	{
		if( !IsDead(ch) && (ch->pathcount > 2 || ch->moveFlag == TRUE) )
		{
			CharDoAni_Moving(ch, ch->movetype );
		}
		else
		{
			ch->changeActionFlag	= 0;
			ch->nCurrentAction		= ch->basicAction;
			ch->nCurrentFrame.PutEncrypted(0);
			ch->offset.x = 0; ch->offset.y = 0;

			if (ch->nActiveCombat.GetDecrypted() == SHARK_MISSILE)
			{
				ch->attackanimationON = 0;
			}
		}
	}
}

int cHorseRider::GetDistance(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	return HorseRiderAniTable[CheckSex(ch)][HorseNo][ ch->nCurrentAction].dx[ ch->nCurrentFrame.GetDecrypted()];
}

inline void cHorseRider::FreeRiderSprite(int SexType,int SpriteKind)
{
	MemFree(RiderPicBuffer[SexType][SpriteKind]);
}

inline void cHorseRider::ClearRiderSprite()
{
	for (int i=0;i<HSEX_TYPE_MAX;i++)
	{
		for (int j=0;j<HRIDER_TYPE_MAX;j++)
		{
			FreeRiderSprite(i,j);
		}
	}
}

inline void cHorseRider::FreeRiderBandSprite(int SexType,int SpriteKind)
{
	MemFree(RiderBandPicBuffer[SexType][SpriteKind]);
}

inline void cHorseRider::ClearRiderBandSprite()
{
	for (int i=0;i<HSEX_TYPE_MAX;i++)
	{
		for (int j=0;j<HRIDER_TYPE_MAX;j++)
		{
			FreeRiderBandSprite(i,j);
		}
	}
}


inline void cHorseRider::FreeHorseSprite(int SpriteKind)
{
	MemFree(HorsePicBuffer[SpriteKind]);
}

inline void cHorseRider::ClearHorseSprite()
{
	for (int i=0;i<HHORSE_TYPE_MAX;i++)
	{
		FreeHorseSprite(i);
	}
}

inline void cHorseRider::FreeHorseSaddleSprite(int SpriteKind)
{
	MemFree(HorseSaddlePicBuffer[SpriteKind]);
}

inline void cHorseRider::ClearHorseSaddleSprite()
{
	for (int i=0;i<HHORSE_TYPE_MAX;i++)
	{
		FreeHorseSaddleSprite(i);
	}
}

inline void cHorseRider::FreeWeaponSprite(int SexType,int SpriteKind)
{
	MemFree(WeaponPicBuffer[SexType][SpriteKind]);
}

inline void cHorseRider::ClearWeaponSprite()
{
//	for (int i=0;i<HSEX_TYPE_MAX;i++)
//	{
		for (int j=0;j<HWEAPON_TYPE_MAX;j++)
		{
			FreeWeaponSprite(0,j);
			FreeWeaponSprite(1,j);
		}
//	}
}

inline void cHorseRider::FreeGuardSprite(int SexType,int SpriteKind)
{
	MemFree(GuardPicBuffer[SexType]);
}

inline void cHorseRider::ClearGuardSprite()
{
//	for (int i=0;i<HSEX_TYPE_MAX;i++)
//	{
//		for (int j=0;j<HGUARD_TYPE_MAX;j++)
//		{
			//FreeGuardSprite(i,j);
	FreeGuardSprite(0,0);
//		}
//	}
}

inline void cHorseRider::FreeRiderGradeSprite(int SexType,int SpriteKind,int GradeType)
{
	MemFree(RiderGradePicBuffer[SexType][SpriteKind][GradeType]);
}

inline void cHorseRider::ClearRiderGradeSprite()
{
	for (int i=0;i<HSEX_TYPE_MAX;i++)
	{
		for (int j=0;j<HHORSE_TYPE_MAX;j++)
		{
			for (int k=0;k<HGRADE_MAX;k++)
			{
				FreeRiderGradeSprite(i,j,k);
			}
		}
	}
}

inline void cHorseRider::FreeHorseGradeSprite(int SpriteKind,int GradeType)
{
	MemFree(HorseGradePicBuffer[SpriteKind][GradeType]);
}

inline void cHorseRider::ClearHorseGradeSprite()
{
	for (int i=0;i<HHORSE_TYPE_MAX;i++)
	{
		for (int j=0;j<HHORSE_GRADE_MAX;j++)
		{
			FreeHorseGradeSprite(i,j);
		}
	}
}

inline void cHorseRider::FreeRiderMantleSprite(int SexType)
{
	MemFree(RiderMantlePicBuffer[SexType]);

}
inline void	cHorseRider::ClearRiderMantleSprite()
{
	FreeRiderMantleSprite(0);
	FreeRiderMantleSprite(1);
}


void cHorseRider::ClearSpriteAll()
{
	ClearRiderSprite();
	ClearHorseSprite();
	ClearWeaponSprite();
	ClearGuardSprite();
	ClearRiderGradeSprite();
	ClearHorseGradeSprite();
	ClearRiderMantleSprite();
}

inline int cHorseRider::CheckSex(LPCHARACTER ch)
{
	return ch->sprno;
}

inline int cHorseRider::CheckRiderKind(LPCHARACTER ch)			// 교체 
{
	return ch->RiderNo-1;
}

inline int cHorseRider::CheckHorseKind(LPCHARACTER ch)			// 교체
{
	switch (ch->HorseNo)
	{
	case 0 :
	case 1 :
	case 2 :
	case 3 : return ch->HorseNo-1; break;
	default : return 0;
	}
}

inline int cHorseRider::CheckWeaponKind(LPCHARACTER ch)		// 교체
{	//< CSD-031020
	int	Accessory=ch->accessory[2];						// 61은 무기의 시작번호 

	if (Accessory>=61&&Accessory<93)
	{
		Accessory-=61;										// 0번부터 
	}
	else
	if (Accessory>=121&&Accessory<127)
	{
		Accessory-=121;										// 0번부터
		Accessory+=32;										// 여기부터 깃발데이터
	}
	else if (Accessory >= 160 && Accessory < 170)
	{
		Accessory -= 160;
		return HorseEffectWeaponConvertTable[CheckSex(ch)][Accessory];
	}
	else if (Accessory >= 170 && Accessory < 180)
	{
		Accessory -= 170;
		return HorseEffectWeaponConvertTable[CheckSex(ch)][Accessory];
	}
	else 
	{
		return -1;
	}

	return HorseWeaponConvertTable[CheckSex(ch)][Accessory];
}	//> CSD-031020

inline int cHorseRider::CheckGuardKind(LPCHARACTER ch)		// 교체
{
	int	Accessory=ch->accessory[3]-87;						// 87은 방패의 시작번호 

/*	if (Accessory<0||Accessory>6) 
	{
		MessageBox(NULL,"Error!!","Shield Sprite Number Error...",MB_OK);
		return 0;
	}*/
	if (Accessory<0) return -1;
	return 0;
}

inline int cHorseRider::CheckRiderGrade(LPCHARACTER ch)		// 교체
{
	return ch->RiderGrade-1;
}

inline int cHorseRider::CheckHorseGrade(LPCHARACTER ch)		// 교체
{
	return ch->HorseGrade-1;
}

inline Spr* cHorseRider::GetSpriteData(int SexType,int SpriteType,int SpriteKind,int Direction,int Frame)
{
	switch(SpriteType)
	{
	case HSRIDER	: return &RiderPic[SexType][SpriteKind][Direction][Frame];
	case HSRBAND	: return &RiderBandPic[SexType][SpriteKind][Direction][Frame];
	case HSHORSE	: return &HorsePic[SpriteKind][Direction][Frame];
	case HSSADDLE	: return &HorseSaddlePic[SpriteKind][Direction][Frame];
	case HSWEAPON	: return &WeaponPic[SexType][SpriteKind][Direction][Frame];
	case HSSHEILD	: return &GuardPic[SexType][Direction][Frame];
	}
	return NULL;
}

inline void cHorseRider::PutConfusion(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB( 255, 0, 0);
	DWORD Color2 = ReturnBlendRGB( 255, 100, 100);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutCurse(LPCHARACTER ch)
{
	int Trans=5;
	int Operation=2;

	DrawFX(ch,Trans,Operation);
}

inline void cHorseRider::PutFrozen(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB(0, 0, 255);
	DWORD Color2 = ReturnBlendRGB(100, 100, 255);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutGhost(LPCHARACTER ch)
{
	int Trans=10;
	int Operation=1;

	DrawFX(ch,Trans,Operation);
}

inline void cHorseRider::PutGreen(LPCHARACTER ch)
{
	DWORD	//color = ReturnBlendRGB( 116,254,178 );
			//color = ReturnBlendRGB( 192,251,148 );
			Color2 = ReturnBlendRGB( 125,251,226 );

	DrawRGBColor(ch,0,Color2);
}

inline void cHorseRider::PutPoison(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB( 0,255, 0);
	DWORD Color2 = ReturnBlendRGB( 100, 255, 100);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutRed(LPCHARACTER ch)
{
	DWORD Color2 = ReturnBlendRGB( 255,100,100 );

	DrawRGBColor(ch,0,Color2);
}

inline void cHorseRider::PutSlow(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB( 255, 255, 0);
	DWORD Color2 = ReturnBlendRGB( 255, 255, 100);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutStone(LPCHARACTER ch)
{
	DWORD Color1 = ReturnBlendRGB( 100, 100, 100);
	DWORD Color2 = ReturnBlendRGB( 100, 100, 100);

	DrawRGBColor(ch,Color1,Color2);
}

inline void cHorseRider::PutTransparency(LPCHARACTER ch)
{
	if (ch==Hero)
	{
		int Trans=6;
		int Operation=1;

		DrawFX(ch,Trans,Operation);
	}
	else
	{
		DrawVoid(ch);
	}
}

inline void cHorseRider::DrawRider(LPCHARACTER ch)			// LTS HORSERIDER ADD
{
	int RiderGrade=CheckRiderGrade(ch);
	int	SexType=CheckSex(ch);
	int RiderKind=CheckRiderKind(ch);		
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* RiderSpr=GetSpriteData(SexType,HSRIDER,RiderKind,ch->direction,Frame);
	Spr* RiderBandSpr=GetSpriteData(SexType,HSRBAND,RiderKind,ch->direction,Frame);
	Spr* RiderGradeSpr=NULL;
	Spr* RiderMantleSpr=NULL;

	if (ch->mantlecolor!=0&&ch->mantlecolor!=65535) RiderMantleSpr=&RiderMantlePic[SexType][ch->direction][Frame];
	
	if (RiderGrade>=0) RiderGradeSpr=&RiderGradePic[SexType][RiderKind][RiderGrade][ch->direction][Frame];	// 범위연산에 문제 생김.

	if (ch->direction==DIRECTION_DOWN||ch->direction==DIRECTION_RIGHTDOWN||ch->direction==DIRECTION_LEFTDOWN)
		PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,ch->mantlecolor);
	//PutCompressedImage(LocateX,LocateY,RiderMantleSpr);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,ch->mantlecolor);
	PutCompressedImage(LocateX,LocateY,RiderSpr);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderBandSpr, ch->bodycolor);
	PutCompressedImage(LocateX,LocateY,RiderGradeSpr);
	if (ch->direction!=DIRECTION_DOWN&&ch->direction!=DIRECTION_RIGHTDOWN&&ch->direction!=DIRECTION_LEFTDOWN)
		PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,ch->mantlecolor);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,ch->mantlecolor);
	//PutCompressedImage(LocateX,LocateY,RiderMantleSpr);
}

inline void cHorseRider::DrawRiderColor(LPCHARACTER ch,DWORD Color1,DWORD Color2)
{
	int RiderGrade=CheckRiderGrade(ch);
	int	SexType=CheckSex(ch);
	int RiderKind=CheckRiderKind(ch);		
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];


	Spr* RiderSpr=GetSpriteData(SexType,HSRIDER,RiderKind,ch->direction,Frame);
	Spr* RiderGradeSpr=NULL;
	Spr* RiderMantleSpr=NULL;

	if (ch->mantlecolor!=0&&ch->mantlecolor!=65535) RiderMantleSpr=&RiderMantlePic[SexType][ch->direction][Frame];
	if (RiderGrade>=0) RiderGradeSpr=&RiderGradePic[SexType][RiderKind][RiderGrade][ch->direction][Frame];	// 범위연산에 문제 생김.

	if (ch->direction==DIRECTION_DOWN||ch->direction==DIRECTION_RIGHTDOWN||ch->direction==DIRECTION_LEFTDOWN)
		PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,Color2);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderSpr,Color2);
	PutCompressedImageCharRGB(LocateX,LocateY,RiderGradeSpr,Color2);
	if (ch->direction!=DIRECTION_DOWN&&ch->direction!=DIRECTION_RIGHTDOWN&&ch->direction!=DIRECTION_LEFTDOWN)
		PutCompressedImageCharRGB(LocateX,LocateY,RiderMantleSpr,Color2);
}

inline void cHorseRider::DrawRiderFX(LPCHARACTER ch,int Trans,int Operation)	// LTS HORSERIDER ADD	
{
	int RiderGrade=CheckRiderGrade(ch);
	int SexType=CheckSex(ch);
	int RiderKind=CheckRiderKind(ch);		
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* RiderSpr=GetSpriteData(SexType,HSRIDER,RiderKind,ch->direction,Frame);
	Spr* RiderGradeSpr=NULL;
	Spr* RiderMantleSpr=NULL;

	if (ch->mantlecolor!=0&&ch->mantlecolor!=65535) RiderMantleSpr=&RiderMantlePic[SexType][ch->direction][Frame];
	if (RiderGrade>=0) RiderGradeSpr=&RiderGradePic[SexType][RiderKind][RiderGrade][ch->direction][Frame];	// 범위연산에 문제 생김.

	if (ch->direction==DIRECTION_DOWN||ch->direction==DIRECTION_RIGHTDOWN||ch->direction==DIRECTION_LEFTDOWN)
		PutCompressedImageFX(LocateX,LocateY,RiderMantleSpr,Trans,Operation);
	PutCompressedImageFX(LocateX,LocateY,RiderSpr,Trans,Operation);
	PutCompressedImageFX(LocateX,LocateY,RiderGradeSpr,Trans,Operation);
}

inline void cHorseRider::DrawRiderVoid(LPCHARACTER ch)
{
	int  RiderGrade=CheckRiderGrade(ch);
	int	SexType=CheckSex(ch);
	int RiderKind=CheckRiderKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];


	Spr* RiderSpr=GetSpriteData(SexType,HSRIDER,RiderKind,ch->direction,Frame);
	Spr* RiderGradeSpr=&RiderGradePic[RiderGrade][SexType][RiderKind][ch->direction][Frame];	// 범위연산에 문제 생김.

	PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),RiderSpr);
	PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),RiderGradeSpr);
}

inline void cHorseRider::DrawHorse(LPCHARACTER ch)
{
	int HorseGrade=CheckHorseGrade(ch);
	int HorseKind=CheckHorseKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseKind][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* HorseSpr=GetSpriteData(0,HSHORSE,HorseKind,ch->direction,Frame);
	Spr* HorseSaddleSpr=GetSpriteData(0,HSSADDLE,HorseKind,ch->direction,Frame);
	Spr* HorseGradeSpr=NULL;

	if (HorseGrade>=0) HorseGradeSpr=&HorseGradePic[HorseKind][HorseGrade][ch->direction][Frame];

	switch (HorseKind)
	{
	case 0 :
	case 1 :	PutCompressedImageShadow(LocateX,LocateY,HorseSpr);
				PutCompressedImage(LocateX,LocateY,HorseSpr);
				PutCompressedImage(LocateX,LocateY,HorseSaddleSpr);
				PutCompressedImage(LocateX,LocateY,HorseGradeSpr);
				break;
	case 2 :
				PutCompressedImageFX(LocateX,LocateY,HorseSpr,3,2);	
				PutCompressedImage(LocateX,LocateY,HorseSaddleSpr);
				PutCompressedImage(LocateX,LocateY,HorseGradeSpr);
				break;
	}
}

inline void cHorseRider::DrawHorseColor(LPCHARACTER ch,DWORD Color1,DWORD Color2)
{
	int HorseGrade=CheckHorseGrade(ch);
	int HorseKind=CheckHorseKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseKind][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* HorseSpr=GetSpriteData(0,HSHORSE,HorseKind,ch->direction,Frame);
	Spr* HorseGradeSpr=NULL;

	if (HorseGrade>=0) HorseGradeSpr=&HorseGradePic[HorseKind][HorseGrade][ch->direction][Frame];

	PutCompressedImageShadow(LocateX,LocateY,HorseSpr);
	PutCompressedImageCharRGB(LocateX,LocateY,HorseSpr,Color2);
	PutCompressedImageCharRGB(LocateX,LocateY,HorseGradeSpr,Color2);
}

inline void cHorseRider::DrawHorseFX(LPCHARACTER ch,int Trans,int Operation)
{
	int HorseGrade=CheckHorseGrade(ch);
	int HorseKind=CheckHorseKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseKind][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* HorseSpr=GetSpriteData(0,HSHORSE,HorseKind,ch->direction,Frame);
	Spr* HorseGradeSpr=NULL;

	if (HorseGrade>=0) HorseGradeSpr=&HorseGradePic[HorseKind][HorseGrade][ch->direction][Frame];

	PutCompressedImageFX(LocateX,LocateY,HorseSpr,Trans,Operation);
	PutCompressedImageFX(LocateX,LocateY,HorseGradeSpr,Trans,Operation);
}

inline void cHorseRider::DrawHorseVoid(LPCHARACTER ch)
{
	int HorseGrade=CheckHorseGrade(ch);
	int HorseKind=CheckHorseKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseKind][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];

	Spr* HorseSpr=GetSpriteData(0,HSHORSE,HorseKind,ch->direction,Frame);
	Spr* HorseGradeSpr=&HorseGradePic[HorseKind][HorseGrade][ch->direction][Frame];

	PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),HorseSpr);
	PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),HorseGradeSpr);
}

inline void cHorseRider::DrawFX(LPCHARACTER ch,int Trans,int Operation)
{
	int SexType=CheckSex(ch);
	int WeaponKind=CheckWeaponKind(ch);
	int GuardKind=CheckGuardKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	Spr* Weapon=NULL;
	Spr* Guard=NULL;
	
	if (ch->peacests==1)
	{
		if (WeaponKind>=0) Weapon=GetSpriteData(SexType,HSWEAPON,WeaponKind,ch->direction,Frame);
		if (GuardKind>=0) Guard=GetSpriteData(SexType,HSSHEILD,GuardKind,ch->direction,Frame);
	}

	int tempDirection=ch->direction;
	if (WeaponKind==5 || WeaponKind==13)			// 양손검은 전투 걷기가 손위로 올라온다.
	{
		tempDirection=DIRECTION_UP;
	}

	switch (tempDirection)
	{
	case DIRECTION_UP			:		
									DrawHorseFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Guard,Trans,Operation);
									DrawRiderFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Weapon,Trans,Operation);	break;
	case DIRECTION_RIGHTUP		:
	case DIRECTION_RIGHT		:
	case DIRECTION_RIGHTDOWN	:
									PutCompressedImageFX(LocateX,LocateY,Guard,Trans,Operation);
									DrawHorseFX(ch,Trans,Operation);
									DrawRiderFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Weapon,Trans,Operation);	break;
	case DIRECTION_DOWN			:		
									DrawHorseFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Weapon,Trans,Operation);
									DrawRiderFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Guard,Trans,Operation);	break;
	case DIRECTION_LEFTDOWN		:		
	case DIRECTION_LEFT			:
	case DIRECTION_LEFTUP		:		
									PutCompressedImageFX(LocateX,LocateY,Weapon,Trans,Operation);
									DrawHorseFX(ch,Trans,Operation);
									DrawRiderFX(ch,Trans,Operation);
									PutCompressedImageFX(LocateX,LocateY,Guard,Trans,Operation);	break;
	}
}

inline void cHorseRider::DrawRGBColor(LPCHARACTER ch,DWORD Color1,DWORD Color2)
{
	int SexType=CheckSex(ch);
	int WeaponKind=CheckWeaponKind(ch);
	int GuardKind=CheckGuardKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	Spr* Weapon=NULL;
	Spr* Guard=NULL;

	if (ch->peacests==1)
	{
		if (WeaponKind>=0) Weapon=GetSpriteData(SexType,HSWEAPON,WeaponKind,ch->direction,Frame);
		if (GuardKind>=0) Guard=GetSpriteData(SexType,HSWEAPON,GuardKind,ch->direction,Frame);
	}

	int tempDirection=ch->direction;
	if (WeaponKind==5 || WeaponKind==13)			// 양손검은 전투 걷기가 손위로 올라온다.
	{
		tempDirection=DIRECTION_UP;
	}

	switch (tempDirection)
	{
	case DIRECTION_UP			:	
									DrawHorseColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Guard,Color2);
									DrawRiderColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Weapon,Color2);		break;
	case DIRECTION_RIGHTUP		:
	case DIRECTION_RIGHT		:
	case DIRECTION_RIGHTDOWN	:
									PutCompressedImageCharRGB(LocateX,LocateY,Guard,Color2);
									DrawHorseColor(ch,Color1,Color2);
									DrawRiderColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Weapon,Color2);		break;
	case DIRECTION_DOWN			:	
									DrawHorseColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Weapon,Color2); 
									DrawRiderColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Guard,Color2);		break;
	case DIRECTION_LEFTDOWN		:	
	case DIRECTION_LEFT			:
	case DIRECTION_LEFTUP		:		
									PutCompressedImageCharRGB(LocateX,LocateY,Weapon,Color2); 
									DrawHorseColor(ch,Color1,Color2);
									DrawRiderColor(ch,Color1,Color2);
									PutCompressedImageCharRGB(LocateX,LocateY,Guard,Color2);		break;
	}
}

inline void cHorseRider::DrawVoid(LPCHARACTER ch)
{
	int SexType=CheckSex(ch);
	int WeaponKind=CheckWeaponKind(ch);
	int GuardKind=CheckGuardKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[SexType][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	Spr* Weapon=NULL;
	Spr* Guard=NULL;

	if (ch->peacests==1)
	{
		if (WeaponKind>=0) Weapon=GetSpriteData(SexType,HSWEAPON,WeaponKind,ch->direction,Frame);
		if (GuardKind>=0) Guard=GetSpriteData(SexType,HSSHEILD,GuardKind,ch->direction,Frame);
	}

	int tempDirection=ch->direction;
	if (WeaponKind==5 || WeaponKind==13)			// 양손검은 전투 걷기가 손위로 올라온다.
	{
		tempDirection=DIRECTION_UP;
	}

	switch (ch->direction)
	{
	case DIRECTION_UP			:	
									DrawHorseVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Guard);
									DrawRiderVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Weapon);		break;
	case DIRECTION_RIGHTUP		:
	case DIRECTION_RIGHT		:
	case DIRECTION_RIGHTDOWN	:
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Guard);
									DrawHorseVoid(ch);
									DrawRiderVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Weapon);		break;
	case DIRECTION_DOWN			:	
									DrawHorseVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Weapon); 
									DrawRiderVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Guard);		break;
	case DIRECTION_LEFTDOWN		:	
	case DIRECTION_LEFT			:
	case DIRECTION_LEFTUP		:	
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Weapon); 
									DrawHorseVoid(ch);
									DrawRiderVoid(ch);
									PutCompressedImageVoid(LocateX+(rand()%5-2),LocateY+(rand()%5-2),Guard);		break;
	}
}

inline void cHorseRider::Draw1(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second)
{
	DrawHorse(ch);
	PutCompressedImage(LocateX,LocateY,First);
	DrawRider(ch);
	PutCompressedImage(LocateX,LocateY,Second);
}
inline void cHorseRider::DrawRGB1(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second,DWORD Color)
{
	DrawHorseColor(ch,0,Color);
	PutCompressedImageCharRGB(LocateX,LocateY,First,Color);
	DrawRiderColor(ch,0,Color);
	PutCompressedImageCharRGB(LocateX,LocateY,Second,Color);
}

inline void cHorseRider::Draw2(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second)
{
	PutCompressedImage(LocateX,LocateY,First);
	DrawHorse(ch);
	DrawRider(ch);
	PutCompressedImage(LocateX,LocateY,Second);
}
inline void cHorseRider::DrawRGB2(LPCHARACTER ch,int LocateX,int LocateY,Spr* First,Spr* Second,DWORD Color)
{
	PutCompressedImageCharRGB(LocateX,LocateY,First,Color);
	DrawHorseColor(ch,0,Color);
	DrawRiderColor(ch,0,Color);
	PutCompressedImageCharRGB(LocateX,LocateY,Second,Color);
}

inline int cHorseRider::DirectionChange(LPCHARACTER ch,int WeaponKind)
{
	if (!CheckSex(ch)) return ch->direction;
	if (WeaponKind==5||WeaponKind==13) // 양손검은 전투 걷기가 손위로 올라온다.
	{
		switch(ch->direction)
		{
		case DIRECTION_UP			:	// 칼을 제일 나중에 그린다.
		case DIRECTION_LEFT			:	// 칼을 제일 나중에 그린다.
		case DIRECTION_LEFTUP		:	// 칼을 제일 나중에 그린다
		case DIRECTION_RIGHTUP		:	// 칼을 제일 나중에 그린다.
		case DIRECTION_RIGHT		:	// 칼을 제일 나중에 그린다.
										return DIRECTION_UP;
										break;
		case DIRECTION_DOWN			:	// 칼을 제일 먼저그린다.
		case DIRECTION_RIGHTDOWN	:	// 칼을 제일 먼저 그린다.
		case DIRECTION_LEFTDOWN		:	// 칼을 제일 먼저 그린다.
										return DIRECTION_DOWN;
										break;
		}
	}
	else return ch->direction;
	return ch->direction;
}

inline void cHorseRider::DrawAction(LPCHARACTER ch)
{
	if( (ch->blood_Set == true || ch->dieani!=0 ) && ch->nCurrentAction==ACTION_DIE ) PutDeadEffectBottom(ch);

	int SexType=CheckSex(ch);
	int WeaponKind=CheckWeaponKind(ch);
	int GuardKind=CheckGuardKind(ch);
	int LocateX=ch->x-Mapx;
	int LocateY=ch->y-Mapy;
	int HorseNo=CheckHorseKind(ch);
	int Frame=HorseRiderAniTable[SexType][HorseNo][ch->nCurrentAction].cFrame[ch->nCurrentFrame.GetDecrypted()];
	Spr* Weapon=NULL;
	Spr* Guard=NULL;
	DWORD Color=0;

	if (ch->peacests==1)
	{
		if (WeaponKind>=0) Weapon=GetSpriteData(SexType,HSWEAPON,WeaponKind,ch->direction,Frame);
		if (GuardKind>=0) Guard=GetSpriteData(SexType,HSSHEILD,GuardKind,ch->direction,Frame);
	}

	int tempDirection=DirectionChange(ch,WeaponKind);							// 양손검은 전투 걷기가 손위로 올라온다.	

	if (ch==g_GameInfo.lpcharacter)
	{
		if (ch==Hero) Color=ReturnBlendRGB( 125,251,226 );
		else Color=ReturnBlendRGB( 255,100,100 );
	}

	switch (tempDirection)							// 방패때문에 찍는 순서를 달리한다.
	{
	case DIRECTION_UP			:	
									if (Color) DrawRGB1(ch,LocateX,LocateY,Guard,Weapon,Color);
									else Draw1(ch,LocateX,LocateY,Guard,Weapon);
									break;
	case DIRECTION_RIGHTUP		:
	case DIRECTION_RIGHT		:
	case DIRECTION_RIGHTDOWN	:
									if (Color) DrawRGB2(ch,LocateX,LocateY,Guard,Weapon,Color);
									else Draw2(ch,LocateX,LocateY,Guard,Weapon);
									break;
	case DIRECTION_DOWN			:	
									if (Color) DrawRGB1(ch,LocateX,LocateY,Weapon,Guard,Color);
									else Draw1(ch,LocateX,LocateY,Weapon,Guard);
									break;
	case DIRECTION_LEFTDOWN		:	
	case DIRECTION_LEFT			:
	case DIRECTION_LEFTUP		:		
									if (Color) DrawRGB2(ch,LocateX,LocateY,Weapon,Guard,Color);
									else Draw2(ch,LocateX,LocateY,Weapon,Guard);
									break;
	}
}

int cHorseRider::GetWaveNo(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	return HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].wave[ch->nCurrentFrame.GetDecrypted()];
}

int cHorseRider::GetEffectWaveNo(LPCHARACTER ch)
{
	int HorseNo=CheckHorseKind(ch);
	return HorseRiderAniTable[CheckSex(ch)][HorseNo][ch->nCurrentAction].effect[ch->nCurrentFrame.GetDecrypted()];
}

void cHorseRider::DrawHorseRider(LPCHARACTER ch)
{
	switch (ch->viewtype)
	{	//< CSD-030509
	case VIEWTYPE_NORMAL_			: DrawAction(ch);					break;
	case VIEWTYPE_CONFUSION_		: PutConfusion(ch);					break;
	case VIEWTYPE_POISON_			: PutPoison(ch);					break;
	case VIEWTYPE_STONE_			: PutStone(ch);						break;
	case VIEWTYPE_SLOW_				: PutSlow(ch);						break;
	case VIEWTYPE_FROZEN_			: PutFrozen(ch);					break;
	case VIEWTYPE_CURSE_			: PutCurse(ch);						break;
	case VIEWTYPE_TRANSPARENCY_		: PutTransparency(ch);				break;
	case VIEWTYPE_GHOST_			: PutGhost(ch);						break;
	}	//> CSD-030509	
}


