/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CMinute;
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

CMinute::CMinute()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CMinute::CMinute( const CMinute &src )
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;

	*this	=	src;
}

CMinute::~CMinute()
{
	if( m_pData )
		delete [] (BYTE*)m_pData;
}

void CMinute::SetSize(int nNewSize, int nGrowBy /* = -1 */)
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
		SP_ASSERT((long)nNewSize * sizeof(MINUTE) <= SIZE_T_MAX);  // no overflow
#endif
		m_pData = (MINUTE*) new BYTE[nNewSize * sizeof(MINUTE)];

		memset(m_pData, 0, nNewSize * sizeof(MINUTE));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(MINUTE));

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
		SP_ASSERT((long)nNewMax * sizeof(MINUTE) <= SIZE_T_MAX);  // no overflow
#endif
		MINUTE* pNewData = (MINUTE*) new BYTE[nNewMax * sizeof(MINUTE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(MINUTE));

		// construct remaining elements
		SP_ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(MINUTE));


		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CMinute::FreeExtra()
{
	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		SP_ASSERT((long)m_nSize * sizeof(MINUTE) <= SIZE_T_MAX);  // no overflow
#endif
		MINUTE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (MINUTE*) new BYTE[m_nSize * sizeof(MINUTE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(MINUTE));
		}

		// get rid of old stuff (note: no destructors called)
		delete [] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CMinute::SetAtGrow(int nIndex, MINUTE newElement)
{
	SP_ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

void CMinute::InsertAt(int nIndex, MINUTE newElement, int nCount /*=1*/)
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
			(nOldSize-nIndex) * sizeof(MINUTE));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(MINUTE));

	}

	// insert new value in the gap
	SP_ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

void CMinute::RemoveAt(int nIndex, int nCount /* = 1 */)
{
	SP_ASSERT(nIndex >= 0);
	SP_ASSERT(nCount >= 0);
	SP_ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(MINUTE));
	m_nSize -= nCount;
}

int CMinute::InsertMinuteSort( MINUTE newElement )
{
	if( newElement.m_time <= 0 )
		return -1;
	else if( newElement.m_fNew < 1e-4 )
		return -1;
	else if( newElement.m_fVolume < 1e-4 )
		return -1;
	else if( !CSPTime::InTradeTime( newElement.m_time, 60 ) )
		return -1;

	time_t tmBegin = 0;
	if( GetSize() > 0 )
	{
		time_t tmLatest = ElementAt(GetSize()-1).m_time;
		CSPTime	sptime(tmLatest);
		CSPTime	sptimeBegin(sptime.GetYear(),sptime.GetMonth(),sptime.GetDay(),0,0,0);
		tmBegin = sptimeBegin.GetTime();
	}
	if( newElement.m_time <= tmBegin )
		return -1;

	for( int i=GetSize()-1; i>=0; i-- )
	{
		MINUTE	& temp = ElementAt(i);
		if( newElement.m_time == temp.m_time )
		{
			SetAt(i,newElement);
			return i;
		}
		if( newElement.m_time > temp.m_time )
		{
			InsertAt(i+1,newElement);
			return i+1;
		}
	}

	InsertAt( 0, newElement );
	return 0;
}

int SortMinute(const void *p1,const void *p2)
{
	MINUTE *pTemp1 = (MINUTE *)p1;
	MINUTE *pTemp2 = (MINUTE *)p2;

	if( pTemp1 && pTemp2 && pTemp1->m_time < pTemp2->m_time )
		return -1;
	else if( pTemp1 && pTemp2 && pTemp1->m_time > pTemp2->m_time )
		return 1;
	return 0;
}

void CMinute::Sort( )
{
	if( m_pData )
		qsort( m_pData, GetSize(), sizeof(MINUTE), SortMinute );
}

void CMinute::RemoveDirty( )
{
	time_t tmBegin = 0;
	if( GetSize() > 0 )
	{
		time_t tmLatest = ElementAt(GetSize()-1).m_time;
		CSPTime	sptime(tmLatest);
		CSPTime	sptimeBegin(sptime.GetYear(),sptime.GetMonth(),sptime.GetDay(),0,0,0);
		tmBegin = sptimeBegin.GetTime();
	}

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
		else if( ElementAt(i).m_time < tmBegin )
			RemoveAt(i);
		else if( i>1 && ElementAt(i).m_time == ElementAt(i-1).m_time )
			RemoveAt(i);
	}
}

void CMinute::InsertAt(int nStartIndex, CMinute* pNewArray)
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

CMinute	& CMinute::operator = ( const CMinute &src )
{
	Copy( src );
	return	*this;
}

void CMinute::Copy( const CMinute &src )
{
	SetSize( 0, src.GetSize()+5 );
	for( int i=0; i<src.GetSize(); i++ )
	{
		Add( src.GetAt(i) );
	}
}

BOOL CMinute::StatVolumeInfo( double *pdVolNow, double *pdVolOuter, double *pdVolInner )
{
	time_t	tmLatest	=	0;
	double	dVolNow = 0, dVolOuter = 0, dVolInner = 0;
	double	dPriceLast	=	-1;
	BOOL	bRise		=	TRUE;
	for( int k=0; k<GetSize(); k++ )
	{
		if( ElementAt(k).m_time > tmLatest )
		{
			tmLatest	=	ElementAt(k).m_time;
			dVolNow		=	ElementAt(k).m_fVolume;
		}
		double	dPrice	=	ElementAt(k).m_fNew;
		if( 0 != k )
		{
			double	dVolume	=	ElementAt(k).m_fVolume - ElementAt(k-1).m_fVolume;
			if( dPrice - dPriceLast > 1e-4 )
				bRise	=	TRUE;
			if( dPrice - dPriceLast < -1e-4 )
				bRise	=	FALSE;
			if( bRise )	dVolOuter	+=	dVolume;
			else		dVolInner	+=	dVolume;
		}
		dPriceLast	=	dPrice;
	}

	if( pdVolNow )
		*pdVolNow = dVolNow;
	if( pdVolOuter )
		*pdVolOuter = dVolOuter;
	if( pdVolInner )
		*pdVolInner = dVolInner;
	return TRUE;
}

BOOL CMinute::StatDealInfo( CSPDWordArray & adwPrice, CSPDWordArray & adwVolume, double * pdMaxVolume )
{
	adwPrice.RemoveAll();
	adwVolume.RemoveAll();

	for( int k=0; k<GetSize(); k++ )
		adwPrice.AddUnique( DWORD(ElementAt(k).m_fNew * 1000) );
	adwPrice.Sort( );

	adwVolume.SetSize( adwPrice.GetSize() );
	for( int k=0; k<GetSize(); k++ )
	{
		DWORD	dwPrice		=	DWORD(ElementAt(k).m_fNew * 1000);
		DWORD	dwVolume	=	DWORD(ElementAt(k).m_fVolume);
		if( k > 0 )
			dwVolume	=	DWORD( ElementAt(k).m_fVolume - ElementAt(k-1).m_fVolume );

		int i;
		for( i=0; i<adwPrice.GetSize(); i++ )
		{
			if( adwPrice[i] == dwPrice )
				break;
		}
		if( i >= adwPrice.GetSize() )
			continue;

		adwVolume[i]	+=	dwVolume;
	}

	double	dMaxVolume = 0;
	for( int k=0; k<adwVolume.GetSize(); k++ )
	{
		if( dMaxVolume < (double)adwVolume[k] )
			dMaxVolume	=	(double)adwVolume[k];
	}
	if( pdMaxVolume )
		*pdMaxVolume	=	dMaxVolume;

	return adwPrice.GetSize() > 0;
}

BOOL CMinute::GetDiffPercentMin5( double * pValue )
{
	if( GetSize( ) < 2 )
		return FALSE;
	MINUTE	& minCurrent = ElementAt(GetSize()-1);
	for( int i=GetSize()-2; i>=0; i-- )
	{
		LONG	l	=	minCurrent.m_time - ElementAt(i).m_time;
		if( l >= 200 && l <= 600 )
		{
			if( ElementAt(i).m_fNew > 1e-4 && minCurrent.m_fNew > 1e-4 )
			{
				if( pValue )	*pValue	=	100*minCurrent.m_fNew / ElementAt(i).m_fNew - 100;
				return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

BOOL CMinute::GetLBDKMinMaxInfo( double dVolAverage, double *pdMin, double *pdMax )
{
	if( dVolAverage < 1e-4 )
		return FALSE;
	
	double	dMulti	=	1 / dVolAverage;
	double	dMin = 0, dMax = 1;
	for( int i=0; i<GetSize(); i++ )
	{
		CSPTime	sptime(ElementAt(i).m_time);
		double	dCurrent = dMulti * ElementAt(i).m_fVolume / CSPTime::GetTimeTradeRatioOfOneDay( sptime, sptime );
		if( 0 == i )
		{
			dMin	=	dCurrent;
			dMax	=	dCurrent;
		}
		if( dCurrent < dMin )	dMin	=	dCurrent;
		if( dCurrent > dMax )	dMax	=	dCurrent;
	}

	if( dMax < 1e-4 )	// 数据为零，缺省dMax=1
		dMax	=	1;
	if( pdMin )	*pdMin	=	dMin;
	if( pdMax )	*pdMax	=	dMax;
	return TRUE;
}

BOOL CMinute::GetIndexWave( double *pdWave, int nIndex )
{
	double dWave = 0;
	for( int k=nIndex-12; k<=nIndex; k++ )
	{
		if( k > 0 && k < GetSize() )
			dWave	+=	ElementAt(k).m_fNew - ElementAt(k-1).m_fNew;
	}
	if( pdWave )
		*pdWave	=	dWave;
	return TRUE;
}

int CMinute::ToKData( CKData & kdata )
{
	// WILLCHECK

	kdata.Clear();
	if( GetSize() <= 0 )
		return 0;
	DWORD dwType = ElementAt(0).m_dwType;
	if( 5 == dwType || 15 == dwType || 30 == dwType || 60 == dwType )
	{
		if( 5 == dwType )
			kdata.SetKType(CKData::ktypeMin5);
		else if( dwType )
			kdata.SetKType(CKData::ktypeMin15);
		else if( 30 == dwType )
			kdata.SetKType(CKData::ktypeMin30);
		else if( 60 == dwType )
			kdata.SetKType(CKData::ktypeMin60);
		kdata.SetSize( 0, GetSize() + 1 );
		for( int i=0; i<GetSize(); i++ )
		{
			MINUTE & min = ElementAt(i);
			KDATA	kd;
			memset( &kd, 0, sizeof(kd) );
			kd.m_dwMarket	= min.m_dwMarket;
			strncpy( kd.m_szCode, min.m_szCode, sizeof(kd.m_szCode)-1 );
			kd.m_time		= min.m_time;
			CSPTime sptime(min.m_time);
			kd.m_date		= sptime.ToStockTimeMin();
			kd.m_fOpen		= (min.m_fHigh+min.m_fLow+min.m_fNew)/3;
			kd.m_fHigh		= min.m_fHigh;
			kd.m_fLow		= min.m_fLow;
			kd.m_fClose		= min.m_fNew;
			kd.m_fVolume	= min.m_fVolume;
			kd.m_fAmount	= min.m_fAmount;
			kdata.Add( kd );
		}
		return kdata.GetSize();
	}

	kdata.SetKType(CKData::ktypeMin5);
	kdata.SetSize( 0, GetSize() / 5 + 5 );

	int	nCount	=	0;
	KDATA	kd;
	for( int pos=0; pos<GetSize(); pos++ )
	{
		MINUTE & min = ElementAt( pos );

		nCount	++;
		if( 1 == nCount )
		{
			memset( &kd, 0, sizeof(kd) );
			kd.m_dwMarket	= min.m_dwMarket;
			strncpy( kd.m_szCode, min.m_szCode, sizeof(kd.m_szCode)-1 );
			kd.m_fOpen		= min.m_fNew;
			kd.m_fHigh		= min.m_fHigh;
			kd.m_fLow		= min.m_fLow;
			kd.m_fClose		= min.m_fNew;
			kd.m_fVolume	= min.m_fVolume;
			kd.m_fAmount	= min.m_fAmount;
		}
		else
		{
			if( kd.m_fHigh < min.m_fHigh )	kd.m_fHigh	=	min.m_fHigh;
			if( kd.m_fLow > min.m_fLow )	kd.m_fLow	=	min.m_fLow;
			kd.m_fAmount	+=	min.m_fAmount;
			kd.m_fVolume	+=	min.m_fVolume;
			kd.m_fClose		=	min.m_fNew;
		}

		CSPTime sptime(min.m_time);
		DWORD date	= sptime.ToStockTimeMin();
		if( !(date % 5) )
		{
			kd.m_time = min.m_time;
			kd.m_date = date;
			kdata.Add( kd );
			nCount = 0;
		}
	}
	
	return kdata.GetSize();
}
/////////////////////////////////////////////////////////////////////////////

