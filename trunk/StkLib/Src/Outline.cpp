/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	COutline;
*/

#include	"StdAfx.h"
#include	"../Include/Stock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

COutline::COutline()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

COutline::COutline( const COutline &src )
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;

	*this	=	src;
}

COutline::~COutline()
{
	if( m_pData )
		delete [] (BYTE*)m_pData;
}

void COutline::SetSize(int nNewSize, int nGrowBy /* = -1 */)
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
		SP_ASSERT((long)nNewSize * sizeof(OUTLINE) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (OUTLINE*) new BYTE[nNewSize * sizeof(OUTLINE)];

		memset(m_pData, 0, nNewSize * sizeof(OUTLINE));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(OUTLINE));

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
		SP_ASSERT((long)nNewMax * sizeof(OUTLINE) <= SIZE_T_MAX);  // no overflow
#endif
		OUTLINE* pNewData = (OUTLINE*) new BYTE[nNewMax * sizeof(OUTLINE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(OUTLINE));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(OUTLINE));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void COutline::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(OUTLINE) <= SIZE_T_MAX);  // no overflow
#endif
		OUTLINE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (OUTLINE*) new BYTE[m_nSize * sizeof(OUTLINE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(OUTLINE));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

/////////////////////////////////////////////////////////////////////////////

void COutline::SetAtGrow(int nIndex, OUTLINE newElement)
{
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void COutline::InsertAt(int nIndex, OUTLINE newElement, int nCount /*=1*/)
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
			(nOldSize-nIndex) * sizeof(OUTLINE));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(OUTLINE));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void COutline::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(OUTLINE));
	m_nSize -= nCount;
}

int COutline::InsertOutlineSort( OUTLINE newElement )
{
	for( int i=0; i<GetSize(); i++ )
	{
		OUTLINE	& temp = ElementAt(i);
		if( temp.m_time == newElement.m_time )
		{
			SetAt(i,newElement);
			return i;
		}
		if( temp.m_time > newElement.m_time )
		{
			InsertAt(i,newElement);
			return i;
		}
	}
	return Add( newElement );
}

int SortOutline(const void *p1,const void *p2)
{
	OUTLINE *pTemp1 = (OUTLINE *)p1;
	OUTLINE *pTemp2 = (OUTLINE *)p2;

	if( pTemp1 && pTemp2 && pTemp1->m_time < pTemp2->m_time )
		return -1;
	else if( pTemp1 && pTemp2 && pTemp1->m_time > pTemp2->m_time )
		return 1;
	return 0;
}

void COutline::Sort( )
{
	if( m_pData )
		qsort( m_pData, GetSize(), sizeof(OUTLINE), SortOutline );
}

void COutline::InsertAt(int nStartIndex, COutline* pNewArray)
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

COutline	& COutline::operator = ( const COutline &src )
{
	Copy( src );
	return	*this;
}

void COutline::Copy( const COutline &src )
{
	SetSize( 0, src.GetSize()+5 );
	for( int i=0; i<src.GetSize(); i++ )
	{
		Add( src.GetAt(i) );
	}
}

/////////////////////////////////////////////////////////////////////////////

