/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CReport;
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

CReport::CReport()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CReport::CReport( const CReport &src )
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;

	*this	=	src;
}

CReport::~CReport()
{
	if( m_pData )
		delete [] (BYTE*)m_pData;
}

void CReport::SetSize(int nNewSize, int nGrowBy /* = -1 */)
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
		SP_ASSERT((long)nNewSize * sizeof(REPORT) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (REPORT*) new BYTE[nNewSize * sizeof(REPORT)];

		memset(m_pData, 0, nNewSize * sizeof(REPORT));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(REPORT));

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
		SP_ASSERT((long)nNewMax * sizeof(REPORT) <= SIZE_T_MAX);  // no overflow
#endif
		REPORT* pNewData = (REPORT*) new BYTE[nNewMax * sizeof(REPORT)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(REPORT));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(REPORT));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CReport::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(REPORT) <= SIZE_T_MAX);  // no overflow
#endif
		REPORT* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (REPORT*) new BYTE[m_nSize * sizeof(REPORT)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(REPORT));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CReport::SetAtGrow(int nIndex, REPORT newElement)
{
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CReport::InsertAt(int nIndex, REPORT newElement, int nCount /*=1*/)
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
			(nOldSize-nIndex) * sizeof(REPORT));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(REPORT));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void CReport::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(REPORT));
	m_nSize -= nCount;
}

void CReport::InsertAt(int nStartIndex, CReport* pNewArray)
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

int CReport::InsertReportSort( REPORT newElement )
{
	if( newElement.m_time <= 0 )
		return -1;
	if( newElement.m_fNew < 1e-4 )
		return -1;
	if( newElement.m_fVolume < 1e-4 )
		return -1;
	if( !CSPTime::InTradeTime( newElement.m_time, 60 ) )
		return -1;

	for( int i=GetSize()-1; i>=0; i-- )
	{
		REPORT & temp = ElementAt(i);
		if( newElement.m_time == temp.m_time )
		{
			SetAt(i,newElement);
			return i;
		}
		if( fabs(newElement.m_fVolume - temp.m_fVolume) < 1e-4 )
		{
			SetAt(i,newElement);
			return i;
		}
		if( newElement.m_time >= temp.m_time && newElement.m_fVolume > temp.m_fVolume )
		{
			InsertAt(i+1,newElement);
			return i+1;
		}
		if( newElement.m_time > temp.m_time && newElement.m_fVolume < temp.m_fVolume )
			return -1;
		if( newElement.m_time < temp.m_time && newElement.m_fVolume > temp.m_fVolume )
			return -1;
	}
	
	InsertAt( 0, newElement );
	return 0;
}

int SortReport(const void *p1,const void *p2)
{
	REPORT *pTemp1 = (REPORT *)p1;
	REPORT *pTemp2 = (REPORT *)p2;

	if( pTemp1 && pTemp2 && pTemp1->m_time < pTemp2->m_time )
		return -1;
	else if( pTemp1 && pTemp2 && pTemp1->m_time > pTemp2->m_time )
		return 1;
	if( pTemp1 && pTemp2 && pTemp1->m_fVolume < pTemp2->m_fVolume )
		return -1;
	else if( pTemp1 && pTemp2 && pTemp1->m_fVolume > pTemp2->m_fVolume )
		return 1;
	return 0;
}

void CReport::Sort( )
{
	if( m_pData )
		qsort( m_pData, GetSize(), sizeof(REPORT), SortReport );
}

void CReport::RemoveDirty( )
{
	for( int i=GetSize()-1; i>=0; i-- )
	{
		if( ElementAt(i).m_time <= 0 )
			RemoveAt(i);
		else if( ElementAt(i).m_fNew < 1e-4 )
			RemoveAt(i);
		else if( ElementAt(i).m_fVolume < 1e-4 )
			RemoveAt(i);
		else if( !CSPTime::InTradeTime( ElementAt(i).m_time, 60 ) )
			RemoveAt(i);
		else if( i>0 && ElementAt(i).m_fVolume - ElementAt(i-1).m_fVolume < 1e-4 )
			RemoveAt(i);
	}
}

CReport	& CReport::operator = ( const CReport &src )
{
	Copy( src );
	return	*this;
}

void CReport::Copy( const CReport &src )
{
	SetSize( 0, src.GetSize()+5 );
	for( int i=0; i<src.GetSize(); i++ )
	{
		Add( src.GetAt(i) );
	}
}

BOOL CReport::GetMMLD( int nIndex, double *pdVolBuy, double *pdVolSell, double * pdVolDiff )
{
	SP_ASSERT( nIndex >= 0 && nIndex < GetSize() );
	if( nIndex < 0 || nIndex > GetSize()-1 )
		return FALSE;

	double	dVolBuy = 0;
	double	dVolSell = 0;
	REPORT & rpt = ElementAt(nIndex);
	for( int k=0; k<sizeof(rpt.m_fBuyVolume)/sizeof(rpt.m_fBuyVolume[0]); k++ )
		dVolBuy		+=	rpt.m_fBuyVolume[k];
	for( int k=0; k<sizeof(rpt.m_fSellVolume)/sizeof(rpt.m_fSellVolume[0]); k++ )
		dVolSell	+=	rpt.m_fSellVolume[k];
	
	if( pdVolBuy )	*pdVolBuy	=	dVolBuy;
	if( pdVolSell )	*pdVolSell	=	dVolSell;
	if( pdVolDiff )	*pdVolDiff	=	(dVolBuy-dVolSell);
	return TRUE;
}

BOOL CReport::GetMMLDMinMaxInfo( double *pdMin, double *pdMax )
{
	double	dMin = 0, dMax = 1;
	for( int i=0; i<GetSize(); i++ )
	{
		double	dVolBuy = 0;
		double	dVolSell = 0;
		GetMMLD( i, &dVolBuy, &dVolSell, NULL );

		if( 0 == i )
		{
			dMin	=	dVolBuy;
			dMax	=	dVolBuy;
		}
		if( dVolBuy < dMin )	dMin	=	dVolBuy;
		if( dVolBuy > dMax )	dMax	=	dVolBuy;
		if( dVolSell < dMin )	dMin	=	dVolSell;
		if( dVolSell > dMax )	dMax	=	dVolSell;
	}

	if( dMax < 500 )
		dMax	=	500;	// 最小为5手

	if( pdMin )	*pdMin	=	dMin;
	if( pdMax )	*pdMax	=	dMax;
	return TRUE;
}

BOOL CReport::StatBuySellEx( float * fSellPrice, float * fSellVolume, float * fBuyPrice, float * fBuyVolume, int nSize )
{
	int k;
	for( k=0; k<nSize; k++ )
	{
		fSellPrice[k]	=	0.;
		fSellVolume[k]	=	0.;
		fBuyPrice[k]	=	0.;
		fBuyVolume[k]	=	0.;
	}

	CSPDWordArray	adwSellPrice, adwSellVolume, adwBuyPrice, adwBuyVolume;
	REPORT	rpt;
	memset( &rpt, 0, sizeof(rpt) );
	int	nBSCount	=	sizeof(rpt.m_fBuyPrice)/sizeof(float);

	for( k=0; k<GetSize(); k++ )
	{
		REPORT	& report	=	ElementAt(k);
		
		// Insert
		for( int i=nBSCount-1; i>=0; i-- )
		{
			if( report.m_fSellPrice[i] > 1e-4 )
			{
				adwSellPrice.InsertAt( 0, DWORD(report.m_fSellPrice[i]*1000) );
				adwSellVolume.InsertAt( 0, DWORD(report.m_fSellVolume[i]) );
			}
			if( report.m_fBuyPrice[i] > 1e-4 )
			{
				adwBuyPrice.InsertAt( 0, DWORD(report.m_fBuyPrice[i]*1000) );
				adwBuyVolume.InsertAt( 0, DWORD(report.m_fBuyVolume[i]) );
			}
		}

		// Remove 
		DWORD	dwSellMin = 0;
		for( int i=0; i<adwSellPrice.GetSize(); i++ )
		{
			if( adwSellPrice[i] < dwSellMin+1e-4 )
			{
				adwSellPrice.RemoveAt(i);
				adwSellVolume.RemoveAt(i);
				i --;
			}
			else
			{
				dwSellMin	=	adwSellPrice[i];
			}
		}

		DWORD	dwBuyMax = 0;
		for( int i=0; i<adwBuyPrice.GetSize(); i++ )
		{
			if( 0 != dwBuyMax && adwBuyPrice[i] > dwBuyMax-1e-4 )
			{
				adwBuyPrice.RemoveAt(i);
				adwBuyVolume.RemoveAt(i);
				i --;
			}
			else
			{
				dwBuyMax	=	adwBuyPrice[i];
			}
		}
	}

	// Store
	for( k=0; k<nSize && k<adwSellPrice.GetSize(); k++ )
	{
		fSellPrice[k]	=	float(0.001 * adwSellPrice[k]);
		fSellVolume[k]	=	float(adwSellVolume[k]);
	}
	for( k=0; k<nSize && k<adwBuyPrice.GetSize(); k++ )
	{
		fBuyPrice[k]	=	float(0.001 * adwBuyPrice[k]);
		fBuyVolume[k]	=	float(adwBuyVolume[k]);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////



