/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CSPArray;
		class	CSPPtrArray;
		class	CSPDWordArray;
		class	CSPObArray;
		class	CSPStringArray;
		class	CSPMapStringToPtr;
*/

#include	"StdAfx.h"
#include	"../Include/SpColl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

#ifndef	_SP_ENABLE_INLINES
#define	_SPCOLL_INLINE
#include "../Include/SpColl.inl"
#undef	_SPCOLL_INLINE
#endif



/////////////////////////////////////////////////////////////////////////////
// CSPPtrArray out-of-line functions
CSPPtrArray::CSPPtrArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CSPPtrArray::~CSPPtrArray()
{
	delete [] (BYTE*)m_pData;
}

void CSPPtrArray::SetSize(int nNewSize, int nGrowBy /* = -1 */)
{
	SP_ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		delete [] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewSize * sizeof(void*) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (void**) new BYTE[nNewSize * sizeof(void*)];

		memset(m_pData, 0, nNewSize * sizeof(void*));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));
		}
		m_nSize = nNewSize;
	}
	else
	{
		// Otherwise grow array
		int nNewMax;
		if (nNewSize < m_nMaxSize + m_nGrowBy)
			nNewMax = m_nMaxSize + m_nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewMax * sizeof(void*) <= SIZE_T_MAX);  // no overflow
#endif
		void** pNewData = (void**) new BYTE[nNewMax * sizeof(void*)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(void*));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CSPPtrArray::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(void*) <= SIZE_T_MAX);  // no overflow
#endif
		void** pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (void**) new BYTE[m_nSize * sizeof(void*)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(void*));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

void CSPPtrArray::SetAtGrow(int nIndex, void* newElement)
{

	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CSPPtrArray::InsertAt(int nIndex, void* newElement, int nCount /*=1*/)
{
	SP_ASSERT(nIndex >= 0);    // will expand to meet need
	SP_ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(void*));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(void*));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void CSPPtrArray::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(void*));
	m_nSize -= nCount;
}

void CSPPtrArray::InsertAt(int nStartIndex, CSPPtrArray* pNewArray)
{
	SP_ASSERT(pNewArray != NULL);
	SP_ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

int CSPPtrArray::Append(const CSPPtrArray& src)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);

	memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(void*));

	return nOldSize;
}

void CSPPtrArray::Copy(const CSPPtrArray& src)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);

	memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(void*));
}

#ifdef	_DEBUG
void	CSPPtrArray::AssertValid( ) const
{
	Object::AssertValid();
	if (m_pData == NULL)
	{
		SP_ASSERT(m_nSize == 0);
		SP_ASSERT(m_nMaxSize == 0);
	}
	else
	{
		SP_ASSERT(m_nSize >= 0);
		SP_ASSERT(m_nMaxSize >= 0);
		SP_ASSERT(m_nSize <= m_nMaxSize);
		SP_ASSERT( SP_IsValidAddress(m_pData, m_nMaxSize * sizeof(void*)) );
	}
}

void	CSPPtrArray::Dump( ) const
{
	Object::Dump( );
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSPDWordArray out-of-line functions
CSPDWordArray::CSPDWordArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CSPDWordArray::~CSPDWordArray()
{
	delete [] (BYTE*)m_pData;
}

void CSPDWordArray::SetSize(int nNewSize, int nGrowBy /* = -1 */)
{
	SP_ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		delete [] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewSize * sizeof(DWORD) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (DWORD*) new BYTE[nNewSize * sizeof(DWORD)];

		memset(m_pData, 0, nNewSize * sizeof(DWORD));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(DWORD));

		}

		m_nSize = nNewSize;
	}
	else
	{
		// Otherwise grow array
		int nNewMax;
		if (nNewSize < m_nMaxSize + m_nGrowBy)
			nNewMax = m_nMaxSize + m_nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewMax * sizeof(DWORD) <= SIZE_T_MAX);  // no overflow
#endif
		DWORD* pNewData = (DWORD*) new BYTE[nNewMax * sizeof(DWORD)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(DWORD));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(DWORD));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CSPDWordArray::Copy(const CSPDWordArray& src)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);

	memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(DWORD));
}

void CSPDWordArray::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(DWORD) <= SIZE_T_MAX);  // no overflow
#endif
		DWORD* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (DWORD*) new BYTE[m_nSize * sizeof(DWORD)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(DWORD));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

void CSPDWordArray::SetAtGrow(int nIndex, DWORD newElement)
{
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CSPDWordArray::InsertAt(int nIndex, DWORD newElement, int nCount /*=1*/)
{
	SP_ASSERT(nIndex >= 0);    // will expand to meet need
	SP_ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(DWORD));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(DWORD));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void CSPDWordArray::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(DWORD));
	m_nSize -= nCount;
}

void CSPDWordArray::InsertAt(int nStartIndex, CSPDWordArray* pNewArray)
{
	SP_ASSERT(pNewArray != NULL);
	SP_ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

int CSPDWordArray::AddUnique( DWORD newElement )
{
	for( int i=0; i<GetSize(); i++ )
	{
		if( ElementAt(i) == newElement )
			return i;
	}
	return Add( newElement );
}

int SortDWord(const void *p1,const void *p2)
{
	DWORD *pTemp1 = (DWORD *)p1;
	DWORD *pTemp2 = (DWORD *)p2;

	if( pTemp1 && pTemp2 && *pTemp1 < *pTemp2 )
		return -1;
	else if( pTemp1 && pTemp2 && *pTemp1 > *pTemp2 )
		return 1;
	return 0;
}

void CSPDWordArray::Sort( )
{
	if( m_pData )
		qsort( m_pData, GetSize(), sizeof(DWORD), SortDWord );
}

BOOL CSPDWordArray::IsEqualTo( CSPDWordArray & adw )
{
	if( GetSize() != adw.GetSize() )
		return FALSE;
	for( int i=0; i<GetSize(); i++ )
	{
		if( ElementAt(i) != adw.ElementAt(i) )
			return FALSE;
	}
	return TRUE;
}

DWORD * CSPDWordArray::GetData()
{
	return m_pData;
}

void CSPDWordArray::Serialize(CSPArchive& ar)
{
	SP_ASSERT_VALID(this);

	if (ar.IsStoring())
	{
		ar << GetSize();
		// write each item individually so that it will be byte-swapped
		for (int i = 0; i < m_nSize; i++)
			ar << m_pData[i];
	}
	else
	{
		int	size;
		ar >> size;
		SetSize(size);
		for (int i = 0; i < size; i++)
		{
			DWORD	dw;
			ar >> dw;
			SetAt(i,dw);
		}
	}
}

#ifdef	_DEBUG

void	CSPDWordArray::AssertValid() const
{
	Object::AssertValid();
	if (m_pData == NULL)
	{
		SP_ASSERT(m_nSize == 0);
		SP_ASSERT(m_nMaxSize == 0);
	}
	else
	{
		SP_ASSERT(m_nSize >= 0);
		SP_ASSERT(m_nMaxSize >= 0);
		SP_ASSERT(m_nSize <= m_nMaxSize);
		SP_ASSERT( SP_IsValidAddress(m_pData, m_nMaxSize * sizeof(DWORD)) );
	}
}

void	CSPDWordArray::Dump( ) const
{
	Object::Dump( );
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSPObArray out-of-line functions

CSPObArray::CSPObArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CSPObArray::~CSPObArray()
{
	SP_ASSERT_VALID(this);

	delete [] (BYTE*)m_pData;
}

void CSPObArray::SetSize(int nNewSize, int nGrowBy /* = -1 */)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		delete [] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewSize * sizeof(Object*) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (Object**) new BYTE[nNewSize * sizeof(Object*)];

		memset(m_pData, 0, nNewSize * sizeof(Object*));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(Object*));

		}

		m_nSize = nNewSize;
	}
	else
	{
		// Otherwise grow array
		int nNewMax;
		if (nNewSize < m_nMaxSize + m_nGrowBy)
			nNewMax = m_nMaxSize + m_nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewMax * sizeof(Object*) <= SIZE_T_MAX);  // no overflow
#endif
		Object** pNewData = (Object**) new BYTE[nNewMax * sizeof(Object*)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(Object*));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(Object*));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CSPObArray::FreeExtra()
{
	SP_ASSERT_VALID(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(Object*) <= SIZE_T_MAX);  // no overflow
#endif
		Object** pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (Object**) new BYTE[m_nSize * sizeof(Object*)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(Object*));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

void CSPObArray::SetAtGrow(int nIndex, Object* newElement)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CSPObArray::InsertAt(int nIndex, Object* newElement, int nCount /*=1*/)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);    // will expand to meet need
	SP_ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(Object*));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(Object*));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void CSPObArray::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(Object*));
	m_nSize -= nCount;
}

void CSPObArray::InsertAt(int nStartIndex, CSPObArray* pNewArray)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(pNewArray != NULL);
	SP_ASSERT_VALID(pNewArray);
	SP_ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

// Diagnostics
#ifdef _DEBUG
void CSPObArray::Dump() const
{
	Object::Dump( );
}

void CSPObArray::AssertValid() const
{
	Object::AssertValid();
	if (m_pData == NULL)
	{
		SP_ASSERT(m_nSize == 0);
		SP_ASSERT(m_nMaxSize == 0);
	}
	else
	{
		SP_ASSERT(m_nSize >= 0);
		SP_ASSERT(m_nMaxSize >= 0);
		SP_ASSERT(m_nSize <= m_nMaxSize);
		SP_ASSERT(SP_IsValidAddress(m_pData, m_nMaxSize * sizeof(Object*)));
	}
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CSPStringArray out-of-line functions

CSPStringArray::CSPStringArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CSPStringArray::~CSPStringArray()
{
	SP_ASSERT_VALID(this);


	SPDestructElements(m_pData, m_nSize);
	delete [] (BYTE*)m_pData;
}

void CSPStringArray::SetSize(int nNewSize, int nGrowBy /* = -1 */)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing

		SPDestructElements(m_pData, m_nSize);
		delete [] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewSize * sizeof(CSPString) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (CSPString*) new BYTE[nNewSize * sizeof(CSPString)];

		SPConstructElements(m_pData, nNewSize);

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			SPConstructElements(&m_pData[m_nSize], nNewSize-m_nSize);

		}

		else if (m_nSize > nNewSize)  // destroy the old elements
			SPDestructElements(&m_pData[nNewSize], m_nSize-nNewSize);

		m_nSize = nNewSize;
	}
	else
	{
		// Otherwise grow array
		int nNewMax;
		if (nNewSize < m_nMaxSize + m_nGrowBy)
			nNewMax = m_nMaxSize + m_nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

#ifdef SIZE_T_MAX
		SP_ASSERT((long)nNewMax * sizeof(CSPString) <= SIZE_T_MAX);  // no overflow
#endif
		CSPString* pNewData = (CSPString*) new BYTE[nNewMax * sizeof(CSPString)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(CSPString));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		SPConstructElements(&pNewData[m_nSize], nNewSize-m_nSize);


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

int CSPStringArray::Append(const CSPStringArray& src)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);

	SPCopyElements(m_pData + nOldSize, src.m_pData, src.m_nSize);

	return nOldSize;
}

void CSPStringArray::Copy(const CSPStringArray& src)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);

	SPCopyElements(m_pData, src.m_pData, src.m_nSize);
}

void CSPStringArray::FreeExtra()
{
	SP_ASSERT_VALID(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(CSPString) <= SIZE_T_MAX);  // no overflow
#endif
		CSPString* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (CSPString*) new BYTE[m_nSize * sizeof(CSPString)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(CSPString));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

void CSPStringArray::SetAtGrow(int nIndex, const char* newElement)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CSPStringArray::InsertAt(int nIndex, const char* newElement, int nCount /*=1*/)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);    // will expand to meet need
	SP_ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(CSPString));

		// re-init slots we copied from

		SPConstructElements(&m_pData[nIndex], nCount);

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void CSPStringArray::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	SPDestructElements(&m_pData[nIndex], nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(CSPString));
	m_nSize -= nCount;
}

void CSPStringArray::InsertAt(int nStartIndex, CSPStringArray* pNewArray)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(pNewArray != NULL);
	SP_ASSERT_VALID(pNewArray);
	SP_ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

CSPStringArray * CSPStringArray::m_pSortSPStringArray = NULL;

int SortSPString(const void *p1,const void *p2)
{
	if( NULL == p1 || NULL == p2 )
		return 0;

	DWORD dw1 = *(DWORD *)p1;
	DWORD dw2 = *(DWORD *)p2;

	if( NULL == CSPStringArray::m_pSortSPStringArray
		|| dw1 < 0 || dw1 >= (DWORD)CSPStringArray::m_pSortSPStringArray->GetSize()
		|| dw2 < 0 || dw2 >= (DWORD)CSPStringArray::m_pSortSPStringArray->GetSize() )
	{
		SP_ASSERT(FALSE);
		return 0;
	}

	return strcmp(CSPStringArray::m_pSortSPStringArray->ElementAt(dw1),
				CSPStringArray::m_pSortSPStringArray->ElementAt(dw2) );
}

BOOL CSPStringArray::GetSortIDArray( CSPDWordArray & adwSortID )
{
	adwSortID.SetSize( 0, GetSize() + 1 );

	for( DWORD i=0; i<(DWORD)GetSize(); i++ )
		adwSortID.Add( i );

	m_pSortSPStringArray = this;
	if( adwSortID.GetData() )
		qsort( adwSortID.GetData(), adwSortID.GetSize(), sizeof(DWORD), SortSPString );
	m_pSortSPStringArray = NULL;

	return TRUE;
}

void CSPStringArray::Serialize(CSPArchive& ar)
{
	SP_ASSERT_VALID(this);

	if (ar.IsStoring())
	{
		ar.WriteCount(GetSize());
		for (int i = 0; i < m_nSize; i++)
			ar << m_pData[i];
	}
	else
	{
		DWORD nOldSize = ar.ReadCount();
		SetSize(nOldSize);
		for (DWORD i = 0; i < nOldSize; i++)
			ar >> m_pData[i];
	}
}

void CSPStringArray::SPConstructElements(CSPString* pElements, int nCount)
{
	SP_ASSERT(nCount == 0 ||
		SP_IsValidAddress(pElements, nCount * sizeof(CSPString)));

	for (; nCount--; ++pElements)
		memcpy(pElements, &afxEmptySPString, sizeof(*pElements));
}

void CSPStringArray::SPDestructElements(CSPString* pElements, int nCount)
{
	SP_ASSERT(nCount == 0 ||
		SP_IsValidAddress(pElements, nCount * sizeof(CSPString)));

	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~CSPString();
}

// Diagnostics

#ifdef _DEBUG
void CSPStringArray::Dump() const
{
	Object::Dump( );
}

void CSPStringArray::AssertValid() const
{
	Object::AssertValid();
	if (m_pData == NULL)
	{
		SP_ASSERT(m_nSize == 0);
		SP_ASSERT(m_nMaxSize == 0);
	}
	else
	{
		SP_ASSERT(m_nSize >= 0);
		SP_ASSERT(m_nMaxSize >= 0);
		SP_ASSERT(m_nSize <= m_nMaxSize);
		SP_ASSERT(SP_IsValidAddress(m_pData, m_nMaxSize * sizeof(CSPString)));
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSPPlex

CSPPlex* PASCAL CSPPlex::Create(CSPPlex*& pHead, UINT nMax, UINT cbElement)
{
	SP_ASSERT(nMax > 0 && cbElement > 0);
	CSPPlex* p = (CSPPlex*) new BYTE[sizeof(CSPPlex) + nMax * cbElement];
			// may throw exception
	p->pNext = pHead;
	pHead = p;  // change head (adds in reverse order for simplicity)
	return p;
}

void CSPPlex::FreeDataChain()     // free this one and links
{
	CSPPlex* p = this;
	while (p != NULL)
	{
		BYTE* bytes = (BYTE*) p;
		CSPPlex* pNext = p->pNext;
		delete[] bytes;
		p = pNext;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSPMapStringToPtr out-of-line functions

CSPMapStringToPtr::CSPMapStringToPtr(int nBlockSize)
{
	SP_ASSERT(nBlockSize > 0);

	m_pHashTable = NULL;
	m_nHashTableSize = 17;  // default size
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

inline UINT CSPMapStringToPtr::HashKey(LPCTSTR key) const
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

void CSPMapStringToPtr::InitHashTable(
	UINT nHashSize, BOOL bAllocNow)
//
// Used to force allocation of a hash table or to override the default
//   hash table size of (which is fairly small)
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(m_nCount == 0);
	SP_ASSERT(nHashSize > 0);

	if (m_pHashTable != NULL)
	{
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}

	if (bAllocNow)
	{
		m_pHashTable = new CAssoc* [nHashSize];
		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

static inline void DestructElement(CSPString* pOldData)
{
	pOldData->~CSPString();
}

void CSPMapStringToPtr::RemoveAll()
{
	SP_ASSERT_VALID(this);

	if (m_pHashTable != NULL)
	{
		// destroy elements
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				DestructElement(&pAssoc->key);  // free up string data
			}
		}

		// free hash table
		delete [] m_pHashTable;
		m_pHashTable = NULL;
	}

	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

CSPMapStringToPtr::~CSPMapStringToPtr()
{
	RemoveAll();
	SP_ASSERT(m_nCount == 0);
}

// Assoc helpers
// same as CList implementation except we store CAssoc's not CNode's
//    and CAssoc's are singly linked all the time

CSPMapStringToPtr::CAssoc*
CSPMapStringToPtr::NewAssoc()
{
	if (m_pFreeList == NULL)
	{
		// add another block
		CSPPlex* newBlock = CSPPlex::Create(m_pBlocks, m_nBlockSize,
							sizeof(CSPMapStringToPtr::CAssoc));
		// chain them into free list
		CSPMapStringToPtr::CAssoc* pAssoc =
				(CSPMapStringToPtr::CAssoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	SP_ASSERT(m_pFreeList != NULL);  // we must have something

	CSPMapStringToPtr::CAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	SP_ASSERT(m_nCount > 0);  // make sure we don't overflow
	//CSPString	emptyString;
	//memcpy(&pAssoc->key, &emptyString, sizeof(CSPString));
	memcpy(&pAssoc->key, &afxEmptySPString, sizeof(pAssoc->key));

	pAssoc->value = 0;

	return pAssoc;
}

void CSPMapStringToPtr::FreeAssoc(CSPMapStringToPtr::CAssoc* pAssoc)
{
	DestructElement(&pAssoc->key);  // free up string data

	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
	SP_ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

CSPMapStringToPtr::CAssoc*
CSPMapStringToPtr::GetAssocAt(LPCTSTR key, UINT& nHash) const
// find association (or return NULL)
{
	nHash = HashKey(key) % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	CAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (pAssoc->key == key)
			return pAssoc;
	}
	return NULL;
}

BOOL CSPMapStringToPtr::Lookup(LPCTSTR key, void*& rValue) const
{
	SP_ASSERT_VALID(this);

	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

BOOL CSPMapStringToPtr::LookupKey(LPCTSTR key, LPCTSTR& rKey) const
{
	SP_ASSERT_VALID(this);

	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rKey = pAssoc->key;
	return TRUE;
}

void*& CSPMapStringToPtr::operator[](LPCTSTR key)
{
	SP_ASSERT_VALID(this);

	UINT nHash;
	CAssoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
	{
		if (m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);

		// it doesn't exist, add a new Association
		pAssoc = NewAssoc();
		pAssoc->nHashValue = nHash;
		pAssoc->key = key;
		// 'pAssoc->value' is a constructed object, nothing more

		// put into hash table
		pAssoc->pNext = m_pHashTable[nHash];
		m_pHashTable[nHash] = pAssoc;
	}
	return pAssoc->value;  // return new reference
}


BOOL CSPMapStringToPtr::RemoveKey(LPCTSTR key)
// remove key - return TRUE if removed
{
	SP_ASSERT_VALID(this);

	if (m_pHashTable == NULL)
		return FALSE;  // nothing in the table

	CAssoc** ppAssocPrev;
	ppAssocPrev = &m_pHashTable[HashKey(key) % m_nHashTableSize];

	CAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (pAssoc->key == key)
		{
			// remove it
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			FreeAssoc(pAssoc);
			return TRUE;
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return FALSE;  // not found
}


// Iterating

void CSPMapStringToPtr::GetNextAssoc(SPPOSITION& rNextPosition,
	CSPString& rKey, void*& rValue) const
{
	SP_ASSERT_VALID(this);
	SP_ASSERT(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)rNextPosition;
	SP_ASSERT(pAssocRet != NULL);

	if (pAssocRet == (CAssoc*) BEFORE_START_SPPOSITION)
	{
		// find the first association
		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		SP_ASSERT(pAssocRet != NULL);  // must find something
	}

	// find next association
	SP_ASSERT(SP_IsValidAddress(pAssocRet, sizeof(CAssoc)));
	CAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (UINT nBucket = pAssocRet->nHashValue + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (SPPOSITION) pAssocNext;

	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}


// Diagnostics

#ifdef _DEBUG
void CSPMapStringToPtr::Dump( ) const
{
	Object::Dump();
}

void CSPMapStringToPtr::AssertValid() const
{
	Object::AssertValid();

	SP_ASSERT(m_nHashTableSize > 0);
	SP_ASSERT(m_nCount == 0 || m_pHashTable != NULL);
		// non-empty map should have hash table
}
#endif //_DEBUG
