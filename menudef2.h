/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

#ifndef __MENUDEF2_H__
#define __MENUDEF2_H__
class CMenuImage
{
	int		index;
	int		image_max;
	char	file_name[40];
	BYTE	*buf;
	Spr		*spr;
public :
	CMenuImage( int i, int max, char *file )
	{
		index = i;
		image_max = max; 
		strcpy( file_name, file );
		spr = new Spr[max];
		memset( spr, 0, sizeof( Spr )*max );// 020620 YGI
		buf = 0;
	}
	CMenuImage()
	{
		image_max = 0;
		buf = 0;
		spr = 0;
	}
	~CMenuImage()
	{
		clear();
	}		

	int clear();
	int	LoadMenuSpr();		// 020701 YGI
	int GetIndex() { return index-10000; }
	Spr *GetSpr( int img_number );
};




// 020620 YGI
class CMenuImageMgr
{
	int m_nMax;
	CMenuImage	**m_ppMenuImage;
public :
	CMenuImageMgr( int m )
	{
		m_nMax = m;
		m_ppMenuImage = new CMenuImage *[m_nMax];
		for( int i=0; i<m_nMax; i++ )
		{
			m_ppMenuImage[i] = 0;
		}
	}
	~CMenuImageMgr()
	{ 
		for (int i = 0; i < m_nMax; ++i)
		{
			if (m_ppMenuImage[i])
			{
				delete m_ppMenuImage[i];
				m_ppMenuImage[i] = NULL;
			}
		}
		
		if (m_ppMenuImage) 
		{
			delete [] m_ppMenuImage;
			m_ppMenuImage = NULL;
		}
	}
	int InsetData( int param, CMenuImage *data )
	{
		m_ppMenuImage[param] = data;
		return 1;
	}
	Spr *GetSpr( int type, int img_number )
	{
		if( type >= m_nMax ) return NULL;
		return m_ppMenuImage[type]->GetSpr( img_number );
	}
};



class CMenuImageLink
{
	CMenuImage *m_pData;
	CMenuImageLink *next;
	CMenuImageLink *find;
public :
	CMenuImageLink() 
	{
		m_pData = 0;
		next = 0;
	}
	~CMenuImageLink()
	{
		if( next ) delete next;
	}
	void InputData( int index, int max, char *file )
	{
		CMenuImageLink *temp = this;
		for( ;temp->next; temp = temp->next ) ;
		CMenuImageLink *data = new CMenuImageLink;
		data->m_pData = new CMenuImage( index, max, file );
		temp->next = data;
	}
	void SetFind()
	{
		find = this;
	}
	CMenuImageLink *Next()
	{
		find = find->next;
		return find;
	}
	int GetParam()
	{
		return m_pData->GetIndex();
	}
	CMenuImage *GetDataPoint() { return m_pData; }
	// 020620 YGI
	int SetNext( CMenuImageLink *pNext ) { next = pNext; return 1; }
};


extern CMenuImageMgr *g_MenuImageMgr;
extern bool CheckContinueLine( char check );
extern void SMenuReg(int iMainType,int x, int y , int iImageType,int iImageNumber, int iFieldCount,int iCheakType = 0,int iTemp = 0  ,int iWork = 0,int iKey = 0);
//021030 lsw
extern void SMenuFTReg(const int iMainType,const int iFieldNo,const int iButtonType,const int iSpecailWillDo,
						const int iX, const int iY,const int iWidth, const int iHeight,	const int iImageType, const int iRectImage = 0, const int iImageNumber = 0,const int iWillDo = 0);
#endif