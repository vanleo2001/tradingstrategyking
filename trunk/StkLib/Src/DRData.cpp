/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CDRData;
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

CDRData::CDRData()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CDRData::CDRData( const CDRData &src )
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;

	*this	=	src;
}

CDRData::~CDRData()
{
	if( m_pData )
		delete [] (BYTE*)m_pData;
}

void CDRData::SetSize(int nNewSize, int nGrowBy /* = -1 */)
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
		SP_ASSERT((long)nNewSize * sizeof(DRDATA) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (DRDATA*) new BYTE[nNewSize * sizeof(DRDATA)];

		memset(m_pData, 0, nNewSize * sizeof(DRDATA));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(DRDATA));

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
		SP_ASSERT((long)nNewMax * sizeof(DRDATA) <= SIZE_T_MAX);  // no overflow
#endif
		DRDATA* pNewData = (DRDATA*) new BYTE[nNewMax * sizeof(DRDATA)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(DRDATA));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(DRDATA));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CDRData::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(DRDATA) <= SIZE_T_MAX);  // no overflow
#endif
		DRDATA* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (DRDATA*) new BYTE[m_nSize * sizeof(DRDATA)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(DRDATA));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDRData::SetAtGrow(int nIndex, DRDATA newElement)
{
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CDRData::InsertAt(int nIndex, DRDATA newElement, int nCount /*=1*/)
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
			(nOldSize-nIndex) * sizeof(DRDATA));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(DRDATA));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void CDRData::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(DRDATA));
	m_nSize -= nCount;
}

int CDRData::InsertDRDataSort( DRDATA newElement )
{
	for( int i=0; i<GetSize(); i++ )
	{
		DRDATA	& temp = ElementAt(i);
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

void CDRData::InsertAt(int nStartIndex, CDRData* pNewArray)
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

CDRData	& CDRData::operator = ( const CDRData &src )
{
	Copy( src );
	return	*this;
}

void CDRData::Copy( const CDRData &src )
{
	SetSize( 0, src.GetSize()+5 );
	for( int i=0; i<src.GetSize(); i++ )
	{
		Add( src.GetAt(i) );
	}
}

int SortDRData(const void *p1,const void *p2)
{
	DRDATA *pTemp1 = (DRDATA *)p1;
	DRDATA *pTemp2 = (DRDATA *)p2;

	if( pTemp1 && pTemp2 && pTemp1->m_date < pTemp2->m_date )
		return -1;
	else if( pTemp1 && pTemp2 && pTemp1->m_date > pTemp2->m_date )
		return 1;
	return 0;
}

void CDRData::Sort( )
{
	if( m_pData )
		qsort( m_pData, GetSize(), sizeof(DRDATA), SortDRData );
}

BOOL CDRData::IsSameAs( CDRData * psrc )
{
	if( NULL == psrc || GetSize() != psrc->GetSize() )
		return FALSE;

	for( int i=0; i<GetSize(); i++ )
	{
		DRDATA & dr = ElementAt(i);
		DRDATA & drsrc = psrc->ElementAt(i);

		if( /* dr.m_dwMarket != drsrc.m_dwMarket || */
			0 != strncmp(dr.m_szCode,drsrc.m_szCode,sizeof(dr.m_szCode))
			|| dr.m_date != drsrc.m_date
			// || dr.m_time != drsrc.m_time
			|| fabs(dr.m_fGive-drsrc.m_fGive) > 1e-5
			|| fabs(dr.m_fPei-drsrc.m_fPei) > 1e-5
			|| fabs(dr.m_fPeiPrice-drsrc.m_fPeiPrice) > 1e-5
			|| fabs(dr.m_fProfit-drsrc.m_fProfit) > 1e-5
			|| dr.m_dwReserved != drsrc.m_dwReserved )
		{
			return FALSE;
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

