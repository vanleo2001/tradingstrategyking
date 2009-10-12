/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CBaseData;
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

CBaseData::CBaseData()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CBaseData::CBaseData( const CBaseData &src )
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;

	*this	=	src;
}

CBaseData::~CBaseData()
{
	if( m_pData )
		delete [] (BYTE*)m_pData;
}

void CBaseData::SetSize(int nNewSize, int nGrowBy /* = -1 */)
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
		SP_ASSERT((long)nNewSize * sizeof(BASEDATA) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (BASEDATA*) new BYTE[nNewSize * sizeof(BASEDATA)];

		memset(m_pData, 0, nNewSize * sizeof(BASEDATA));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(BASEDATA));

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
		SP_ASSERT((long)nNewMax * sizeof(BASEDATA) <= SIZE_T_MAX);  // no overflow
#endif
		BASEDATA* pNewData = (BASEDATA*) new BYTE[nNewMax * sizeof(BASEDATA)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(BASEDATA));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(BASEDATA));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CBaseData::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(BASEDATA) <= SIZE_T_MAX);  // no overflow
#endif
		BASEDATA* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (BASEDATA*) new BYTE[m_nSize * sizeof(BASEDATA)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(BASEDATA));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CBaseData::SetAtGrow(int nIndex, BASEDATA newElement)
{
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CBaseData::InsertAt(int nIndex, BASEDATA newElement, int nCount /*=1*/)
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
			(nOldSize-nIndex) * sizeof(BASEDATA));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(BASEDATA));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void CBaseData::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(BASEDATA));
	m_nSize -= nCount;
}

void CBaseData::InsertAt(int nStartIndex, CBaseData* pNewArray)
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

int CBaseData::InsertBaseDataSort( BASEDATA newElement )
{
	for( int i=0; i<GetSize(); i++ )
	{
		BASEDATA	& temp = ElementAt(i);
		if( temp.m_date == newElement.m_date )
		{
			SetAt(i,newElement);
			return i;
		}
		if( temp.m_date > newElement.m_date )
		{
			InsertAt(i,newElement);
			return i;
		}
	}
	return Add( newElement );
}

int SortBaseData(const void *p1,const void *p2)
{
	BASEDATA *pTemp1 = (BASEDATA *)p1;
	BASEDATA *pTemp2 = (BASEDATA *)p2;

	if( pTemp1 && pTemp2 && pTemp1->m_date < pTemp2->m_date )
		return -1;
	else if( pTemp1 && pTemp2 && pTemp1->m_date > pTemp2->m_date )
		return 1;
	return 0;
}

void CBaseData::Sort( )
{
	if( m_pData )
		qsort( m_pData, GetSize(), sizeof(BASEDATA), SortBaseData );
}

CBaseData	& CBaseData::operator = ( const CBaseData &src )
{
	Copy( src );
	return	*this;
}

void CBaseData::Copy( const CBaseData &src )
{
	SetSize( 0, src.GetSize()+5 );
	for( int i=0; i<src.GetSize(); i++ )
	{
		Add( src.GetAt(i) );
	}
}

/////////////////////////////////////////////////////////////////////////////




