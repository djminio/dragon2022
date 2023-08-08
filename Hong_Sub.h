


#ifndef _INC_STDIO
#include "stdio.h"
#endif

#define __DEBUG__

#ifdef __DEBUG__
	#define MYASSERT(x)		_ASSERT(x)
#else
	#define MYASSERT(x)
#endif

#define MemAlloc(x, y)			__Alloc( (char **)(&##x), (y), __FILE__, __LINE__ )

#define MemFree( x )			MemFree_Sub( (char **)(&##x), __FILE__, __LINE__ )

#define NumberType( a, b )		{  INumberStartSprNo = a; INumberTypeWidth = b; }

#define Random( x )				(::rand() % ( (0 < (x) )? (x) : (1) ))

#define Fopen( filename, filemode )   FopenSub_( filename, filemode, __FILE__, __LINE__ )

#ifndef __HONG_SUB_H__
#define __HONG_SUB_H__
	extern void Error ( char *mssg, ... );
	extern void MemFree_Sub( char **buf, char *file, int line  );
	extern void __Alloc( char **s, int size, const char *file, int line );
	extern FILE *FopenSub_( char *name, char *mode, char *wherefile, int line );
	extern void MakeFullPathName( char *d, char *path, char *filename );
	extern void MakeFullPathNameCD( char *d, char drive, char *path, char *filename );
	extern void MakeFullPathNameWithNum( char *d, char *path, char *filename,... );
	extern void MakeFullPathNameWithNumCD( char *d, char drive, char *path, char *filename,... );
	extern int  Distance( int sx, int sy, int dx, int dy );
	extern double DistanceF( float sx, float sy, float dx, float dy );
	extern void Change( int *x, int *y );

	extern void GetImage( int x, int y, int xl, int yl, int axl, char *des, char *sor );
	extern void PutImage( int x, int y, int axl, char *des, char *sor );
	extern void GetImage2File( int index, int x, int y, int xl, int yl, int axl, char *sor );
	extern void PutImage2File( int index, char *des );

	extern void JustMsg( const char *s, ... );
	extern int	YesOrNo( char *s, char *title );

	extern void CaptureScreen ( void );

	extern int  INumberStartSprNo, INumberTypeWidth;

	extern int  BoxAndDotCrash( int dx, int dy, int dxl, int dyl, int x, int y );
	extern int  BoxAndDotCrash( int dx, int dy, int dxl, int dyl, int x, int y, int dz, int dzl, int z);
	extern int  BoxAndBoxCrash( int ax, int ay, int axl, int ayl,  int bx, int by, int bxl, int byl );
	extern int  BoxAndBoxCrash( int ax, int ay, int axl, int ayl,  int bx, int by, int bxl, int byl, int az, int azl, int bz, int bzl );
	extern int CheckIntersect(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y);


	extern int  ViewCheckRoutine( const int t );

	extern int	Determinant( int ax, int ay, int bx, int by, int X, int Y);
	extern int	IsInBox( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x, int y );

	extern void Log( char *msg, char *name, ... );

	extern void	ReturnCircleResult(int s_X, int s_Y, int e_X, int e_Y, float &result_X, float &result_Y, int angle);
	extern void CurrentGameDate( DWORD t, int *y, int *mo, int *d, int *h, int *mi, int *sec );
	
	extern int ReturnFileNumber( char *sou );
	extern void Dir2DirCopy( char *sou, char *dest );
	
	extern int ReturnLanguageID( void );
				
	extern int	_SIN[ 256], _COS[ 256];
	extern int  ITotalUseMemory;
	extern int  IMemUseCount;

	extern int GetWindowVersion( void );
	extern void CheckingAccelator_Sub1( void );
	extern void CheckingAccelator( DWORD t );	// 010314 KHS 

	extern void DeleteDir( char *dir );
	
	
#else
	
	char				OpenFileBoxFileName[ FILENAME_MAX], OpenFileBoxTitle[ FILENAME_MAX];
	int					ITotalUseMemory =0;
	int					IMemUseCount = 0;
	
	int _SIN[256] = { 0, 
24, 49, 73, 97, 122, 146, 
170, 194, 218, 242, 266, 290, 
313, 336, 359, 382, 405, 427, 
449, 471, 492, 513, 534, 555, 
575, 595, 614, 634, 652, 671, 
689, 706, 723, 740, 756, 772, 
788, 802, 817, 831, 844, 857, 
869, 881, 892, 903, 913, 923, 
932, 941, 949, 956, 963, 969, 
975, 980, 985, 989, 992, 995, 
997, 998, 999, 999, 999, 998, 
997, 995, 992, 989, 985, 980, 
975, 970, 964, 957, 949, 941, 
933, 924, 914, 904, 893, 882, 
870, 858, 845, 832, 818, 803, 
789, 773, 757, 741, 725, 707, 
690, 672, 654, 635, 616, 596, 
576, 556, 536, 515, 494, 472, 
450, 428, 406, 383, 361, 338, 
315, 291, 268, 244, 220, 196, 
172, 148, 123, 99, 75, 50, 
26, 1, -22, -47, -71, -96, 
-120, -145, -169, -193, -217, -241, 
-265, -288, -312, -335, -358, -381, 
-403, -425, -447, -469, -491, -512, 
-533, -553, -574, -594, -613, -632, 
-651, -670, -688, -705, -722, -739, 
-755, -771, -787, -801, -816, -830, 
-843, -856, -869, -880, -892, -903, 
-913, -923, -932, -940, -948, -956, 
-963, -969, -975, -980, -984, -988, 
-992, -994, -997, -998, -999, -999, 
-999, -998, -997, -995, -992, -989, 
-985, -981, -976, -970, -964, -957, 
-950, -942, -933, -924, -915, -905, 
-894, -883, -871, -859, -846, -832, 
-819, -804, -790, -774, -759, -742, 
-726, -709, -691, -673, -655, -636, 
-617, -597, -578, -557, -537, -516, 
-495, -473, -452, -430, -407, -385, 
-362, -339, -316, -293, -269, -245, 
-222, -198, -174, -149, -125, -101, 
-76, -52, -27};

int _COS[256] = { 1000, 
999, 998, 997, 995, 992, 989, 
985, 980, 975, 970, 963, 956, 
949, 941, 933, 923, 914, 904, 
893, 882, 870, 857, 845, 831, 
817, 803, 788, 773, 757, 741, 
724, 707, 689, 671, 653, 634, 
615, 596, 576, 555, 535, 514, 
493, 471, 450, 428, 405, 383, 
360, 337, 314, 290, 267, 243, 
219, 195, 171, 147, 123, 98, 
74, 49, 25, 0, -23, -48, 
-72, -97, -121, -145, -170, -194, 
-218, -242, -265, -289, -312, -335, 
-358, -381, -404, -426, -448, -470, 
-491, -513, -534, -554, -574, -594, 
-614, -633, -652, -670, -688, -706, 
-723, -740, -756, -772, -787, -802, 
-816, -830, -844, -857, -869, -881, 
-892, -903, -913, -923, -932, -941, 
-949, -956, -963, -969, -975, -980, 
-985, -988, -992, -995, -997, -998, 
-999, -999, -999, -998, -997, -995, 
-992, -989, -985, -981, -976, -970, 
-964, -957, -950, -942, -933, -924, 
-914, -904, -894, -882, -870, -858, 
-845, -832, -818, -804, -789, -774, 
-758, -742, -725, -708, -690, -673, 
-654, -635, -616, -597, -577, -557, 
-536, -515, -494, -473, -451, -429, 
-407, -384, -361, -338, -315, -292, 
-268, -245, -221, -197, -173, -149, 
-124, -100, -75, -51, -26, -2, 
22, 46, 71, 95, 119, 144, 
168, 192, 216, 240, 264, 287, 
311, 334, 357, 380, 402, 425, 
447, 469, 490, 511, 532, 553, 
573, 593, 613, 632, 651, 669, 
687, 705, 722, 739, 755, 771, 
786, 801, 815, 829, 843, 856, 
868, 880, 891, 902, 913, 922, 
931, 940, 948, 956, 962, 969, 
975, 980, 984, 988, 992, 994, 
997, 998, 999};


#endif
