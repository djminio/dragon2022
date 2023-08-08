#ifndef __KEIN_STL__H__
#define __KEIN_STL__H__


// 020620 YGI ----------------------------------------
// list 
template <class T1> class TKeinList
{
	typedef TKeinList<T1> LIST;
public :
	
	int m_nIndex;
	T1	*m_pData;
	LIST *m_pNext;
public :
	TKeinList() { m_nIndex = -1; m_pData = 0; GetNext() = 0; }
	~TKeinList() { if( m_pData ) delete m_pData; if( GetNext() ) delete GetNext(); }
	LIST *&GetNext(){ return m_pNext; }
	LIST *GetEndNode()
	{ 
		LIST *temp = this;
		for( ; temp->GetNext(); temp = temp->GetNext() );
		return temp;
	}
	T1& operator[](int nIndex)
	{
		LIST *pList = Find( nIndex );
		if( !pList )
		{
			T1 *pT = new T1;
			pList = AddNode( pT, nIndex );
		}
		return *pList->m_pData;
	}

	LIST *Find( int index )
	{
		if( index < 0 ) return NULL;
		LIST *temp = this->GetNext();
		LIST *old;
		int count = 0; 
		
		while( temp )
		{
			if( temp->m_nIndex == index ) return temp;
			old = temp;
			temp = temp->GetNext();
			count++;
		}
		return NULL;
	}

	LIST *AddNode( T1 *pData, int index )
	{
		if( index < 0 ) return false;
		LIST *pList = Find( index );
		if( pList )
		{
			delete pList->m_pData;
			pList->m_pData = pData;
			return pList;
		}
		else
		{
			LIST *pEnd = GetEndNode();
			LIST *pAddData = new LIST;
			pAddData->m_pData = pData;
			pAddData->m_nIndex = index;
			pEnd->GetNext() = pAddData;
			return pAddData;
		}
	}
	bool AddNode( T1 *pData )
	{
		LIST *pEnd = GetEndNode();
		LIST *pAddData = new LIST;
		pAddData->m_pData = pData;
		pEnd->GetNext() = pAddData;
		return 1;
	}
	//020808 YGI  ------------------------
	bool DeleteEndNode()
	{
		LIST *temp1 = this;
		LIST *temp2 = GetNext();
		if( !temp2 ) return 0;
		
		for( ; temp2->GetNext(); )
		{
			temp1 = temp2;
			temp2 = temp2->GetNext();
		}
		temp1->GetNext() = 0;
		delete temp2;
		return 1;
	}
	bool FreeAll()
	{
		if( GetNext() ) delete GetNext();
		GetNext() = 0;
		if( m_pData ) delete m_pData;
		m_pData = 0;
		return 1;
	}
	T1 *GetData()
	{
		return m_pData;
	}
	// ----------------------------
};

// YGI ----------------------------------------
// map
template <class T1> class TKeinMap
{
public :
	int index;
	TKeinList<T1> *m_pList;
	TKeinMap<T1> *m_pNext;
	TKeinMap<T1> *m_pPrev;

public :
	TKeinList<T1> *m_pFind;

public :
	TKeinMap() { index = 0; m_pList = 0; GetNext()= 0; GetPrev() = 0; m_pFind=0; }
	~TKeinMap() { if( m_pList ) delete m_pList; if( GetNext() ) delete GetNext(); }
	TKeinMap<T1> *&GetNext(){ return m_pNext; }
	TKeinMap<T1> *&GetPrev(){ return m_pPrev; }

	TKeinMap<T1> *GetEndNode()
	{ 
		TKeinMap<T1> *temp = this; 
		for( ; temp->GetNext(); temp = temp->GetNext() );
		return temp;
	}
	bool AddNode( T1 *pData, int index )
	{
		if( !index ) return false;
		TKeinList<T1> *pList = GetList( index );
		if( pList )
		{
			pList->AddNode( pData);
		}
		else
		{
			TKeinMap<T1> *pEnd = GetEndNode();
			TKeinMap<T1> *pAddData = new TKeinMap<T1>;

			pAddData->m_pList = new TKeinList<T1>;
			pAddData->index = index;
			pAddData->GetPrev() = pEnd;

			pAddData->m_pList->AddNode( pData );
			pEnd->GetNext() = pAddData;
		}
		return 1;
	}
	TKeinList<T1> *GetList( int index )
	{
		TKeinMap<T1> *pNode = GetNode( index );
		if( pNode ) return pNode->m_pList;
		else return NULL;
	}
	TKeinMap<T1> *GetNode( int index )
	{
		for( TKeinMap<T1> *temp = GetNext(); temp; temp = temp->GetNext() )
		{
			if( temp->index == index ) return temp;
		}
		return NULL;
	}
	bool DeleteList( int index )
	{
		TKeinMap<T1> *pNode = GetNode( index );
		if( pNode )
		{
			if( pNode->GetNext() )
			{
				pNode->GetNext()->GetPrev() = pNode->GetPrev();
				pNode->GetPrev()->GetNext() = pNode->GetNext();

				pNode->GetNext() = 0;
				delete pNode;
				return true;
			}
		}
		return false;
	}
	bool SetFind( int index )
	{
		m_pFind = GetList( index );
		if( m_pFind ) return true;
		else return false;
	}

	T1 *GetFindNext()
	{
		if( m_pFind )
		{
			m_pFind = m_pFind->GetNext();
			if( m_pFind )
			{
				return m_pFind->m_pData;
			}
		}	
		return NULL;
	}
};


// YGI ----------------------------------------
// array
// YGI ----------------------------------------
// array
const int MaxGuildBlock = 256;
template <typename T> class TKeinGuildMap
{	//< CSD-030326
public :
	typedef map<short, T* > GUILDHASH;
	typedef typename GUILDHASH::iterator LPGUILDHASHUNIT;
private :

	GUILDHASH	m_mpBank;	

public :
	T& operator[](int nIndex)
	{
		int nFirstParam = nIndex / MaxGuildBlock;
		int nSecondParam = nIndex % MaxGuildBlock;

		if( !m_mpBank[nFirstParam] )
		{
			m_mpBank[nFirstParam] = new T[MaxGuildBlock];
		}
		return (m_mpBank[nFirstParam])[nSecondParam];
	}
	TKeinGuildMap() { }
	~TKeinGuildMap()
	{
		Reset();
	}

	LPGUILDHASHUNIT Begin()
	{
		return m_mpBank.begin();
	}
	
	LPGUILDHASHUNIT End()
	{
		return m_mpBank.end();
	}

	void Reset()
	{
		for( LPGUILDHASHUNIT i = Begin(); i != End(); i++ )
		{
			delete [] i->second;
		}
		m_mpBank.clear();
	}
};	//> CSD-030326




#endif