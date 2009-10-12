/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CStockInfoArray;
		class	CStockContainer;
		class	CDomain;
		class	CDomainContainer;
*/

#include "stdafx.h"
#include "../Include/Container.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CStockContainer

CStockContainer * CStockContainer::m_pSortContainer	=	NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStockContainer::CStockContainer()
{
	m_nType		=	typeNone;
	m_dwDate	=	-1;
	m_nCurrentStock	=	-1;

	m_nSortVariantID	=	SLH_INVALID;
	m_bSortAscend		=	FALSE;
}

CStockContainer::~CStockContainer()
{
	Clear( );
}

BOOL CStockContainer::Lock()
{
	return m_mutex.Lock();
}

BOOL CStockContainer::UnLock()
{
	return m_mutex.UnLock();
}

int CStockContainer::Add( CStockInfo &newElement )
{
	CSPMutex::Scoped	l(m_mutex);

	int nIndex = CStockInfoArray::Add(newElement);
	m_map.SetAt( newElement.GetStockCode(), (void *)nIndex );
	return nIndex;
}

BOOL CStockContainer::GetCurrentType( int *pType, CSPString * pDomain, DWORD *pdwDate )
{
	CSPMutex::Scoped	l(m_mutex);

	if( pType )
		*pType	=	m_nType;
	if( pDomain )
		*pDomain	=	m_strDomain;
	if( pdwDate )
		*pdwDate	=	m_dwDate;
	return TRUE;
}

BOOL CStockContainer::GetPioneerTechDate( DWORD * pdwDate )
{
	CStockInfo	info;
	if( GetStockInfo( STKLIB_CODE_MAIN, &info )
		&& info.m_kdata.GetSize() > 0 )
	{
		*pdwDate	=	info.m_kdata.ElementAt(0).m_date;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockContainer::GetLatestTechDate( DWORD * pdwDate )
{
	CStockInfo	info;
	if( GetStockInfo( STKLIB_CODE_MAIN, &info )
		&& info.m_kdata.GetSize() > 0 )
	{
		*pdwDate	=	info.m_kdata.ElementAt(info.m_kdata.GetSize()-1).m_date;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockContainer::GetTechDateArray( CSPDWordArray & dwArray )
{
	CStockInfo	info;
	if( GetStockInfo( STKLIB_CODE_MAIN, &info ) )
	{
		dwArray.SetSize( 0, info.m_kdata.GetSize()+10 );
		for( int i=0; i<info.m_kdata.GetSize(); i++ )
			dwArray.Add( info.m_kdata.ElementAt(i).m_date );
		return TRUE;
	}
	return FALSE;
}

BOOL CStockContainer::RetrieveFromStatic( int nType, LPCTSTR lpszDomain, CStrategy * pStrategy, DWORD dwDate )
{
	CSPMutex::Scoped	l(m_mutex);

	Clear( );

	if( nType == typeNone )
	{
		m_nType	=	typeNone;
		return TRUE;
	}

	if( nType < typeMin || nType > typeMax )
		return FALSE;
	m_nType		=	nType;

	m_dwDate	=	dwDate;
	
	CSPStringArray	astrSpecify;

	switch( nType )
	{
	case typeAll:
		CopyData( AfxGetStockContainer() );
		break;
	case typeIndex:
	case typeA:
	case typeBond:
	case typeFund:
	case typeClassShaa:
	case typeClassShab:
	case typeClassSzna:
	case typeClassSznb:
	case typeClassShabond:
	case typeClassSznbond:
	case typeClassMsmall:
	case typeRight:
		{
			int	nCount	=	0;
			SetSize( 0, AfxGetStockContainer().GetSize() );
			for( int i=0; i<AfxGetStockContainer().GetSize(); i++ )
			{
				CStockInfo	& info = AfxGetStockContainer().ElementAt(i);
				LONG	stocktype	=	info.GetType();
				if( ( typeIndex == nType && CStock::typeshIndex == stocktype )
					|| ( typeIndex == nType && CStock::typeszIndex == stocktype )
					|| ( typeA == nType && (CStock::typeshA == stocktype || CStock::typeszA == stocktype) && !info.IsFund() )
					|| ( typeBond == nType && (CStock::typeshBond == stocktype || CStock::typeszBond == stocktype) )
					|| ( typeFund == nType && info.IsFund() )
					|| ( typeClassShaa == nType && CStock::typeshA == stocktype )
					|| ( typeClassShab == nType && CStock::typeshB == stocktype )
					|| ( typeClassSzna == nType && CStock::typeszA == stocktype )
					|| ( typeClassSznb == nType && CStock::typeszB == stocktype )
					|| ( typeClassShabond == nType && CStock::typeshBond == stocktype )
					|| ( typeClassSznbond == nType && CStock::typeszBond == stocktype )
					|| ( typeClassMsmall == nType && CStock::typeszMsmall == stocktype )
					|| ( typeRight == nType && ( CStock::typeshRight == stocktype || CStock::typeszRight == stocktype) ) )
				{
					Add( info );
					nCount	++;
				}
			}
			SetSize( nCount );
		}
		break;
	case typeStrategy:
		{
			if( pStrategy )
				RetrieveSpecify( pStrategy->GetStocks() );
		}
		break;
	case typeGroup:
		{
			if( lpszDomain && strlen(lpszDomain) > 0 )
			{
				AfxGetGroupContainer().GetDomainStocks( lpszDomain, astrSpecify );
				RetrieveSpecify( astrSpecify );
				m_strDomain	=	lpszDomain;
			}
			else
				return AfxGetGroupContainer().GetAllDomainInfo( this, m_dwDate );
		}
		break;
	case typeDomain:
		{
			if( lpszDomain && strlen(lpszDomain) > 0 )
			{
				AfxGetDomainContainer().GetDomainStocks( lpszDomain, astrSpecify );
				RetrieveSpecify( astrSpecify );
				m_strDomain	=	lpszDomain;
			}
			else
				return AfxGetDomainContainer().GetAllDomainInfo( this, m_dwDate );
		}
		break;
	default:
		SP_ASSERT( FALSE );
		SetMap( );
		SetAverage( );
		return FALSE;
	}

	SetMap( );

	DWORD	dwMainDate = -1;
	AfxGetStockContainer().GetCurrentType( NULL, NULL, &dwMainDate );
	if( m_dwDate != dwMainDate )
		OnDataChanged();
	else
		SetAverage( );
	return TRUE;
}

BOOL CStockContainer::ReRetrieveFromStatic( CStrategy * pStrategy, BOOL bUpToDate )
{
	CSPMutex::Scoped	l(m_mutex);

	int	type;
	CSPString	strDomain;
	DWORD	dwDate;
	GetCurrentType( &type, &strDomain, &dwDate );
	if( bUpToDate )
		dwDate	=	-1;
	return RetrieveFromStatic( type, strDomain, pStrategy, dwDate );
}

BOOL CStockContainer::RetrieveSpecify( CSPStringArray & astr )
{
	CSPMutex::Scoped	l(m_mutex);

	SetSize( 0, astr.GetSize() );

	int	nCount	=	0;
	for( int i=0; i<astr.GetSize(); i++ )
	{
		CSPString	string	=	astr.ElementAt(i);
		CStockInfo	info;
		if( AfxGetStockContainer().GetStockInfo( string, &info ) )
		{
			Add( info );
			nCount	++;
		}
	}
	SetSize( nCount );

	return TRUE;
}

BOOL CStockContainer::GetStockInfo( const char * szCode, CStockInfo * pInfo, int * pid )
{
	CSPMutex::Scoped	l(m_mutex);

	if( NULL == szCode || strlen(szCode) <= 0 )
		return FALSE;

	void * pArrayID	=	NULL;
	if( m_map.Lookup( szCode, pArrayID ) )
	{
		if( ((int)pArrayID) >= 0 && ((int)pArrayID) < GetSize() )
		{
			CStockInfo	& info	=	ElementAt((int)pArrayID);
			if( pInfo )
				*pInfo	=	info;
			if( pid )
				*pid	=	(int)pArrayID;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CStockContainer::SetCurrentStock( const char * szCode )
{
	CSPMutex::Scoped	l(m_mutex);

	int	nStockIndex	=	-1;
	CStockInfo	info;
	if( GetStockInfo( szCode, &info, &nStockIndex ) )
		return SetCurrentStock( nStockIndex );
	else
	{
		m_nCurrentStock			=	-1;
		m_strCurrentStockCode	=	szCode;
		return FALSE;
	}
}

BOOL CStockContainer::SetCurrentStock( int nStockIndex )
{
	CSPMutex::Scoped	l(m_mutex);

	if( nStockIndex >= 0 && nStockIndex < GetSize() )
	{
		m_nCurrentStock	=	nStockIndex;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockContainer::GetCurrentStock( CStockInfo * pInfo )
{
	CSPMutex::Scoped	l(m_mutex);

	SP_ASSERT( pInfo );
	if( m_nCurrentStock >= 0 && m_nCurrentStock < GetSize() )
	{
		CStockInfo	& info	=	ElementAt(m_nCurrentStock);
		if( pInfo )
			* pInfo	=	info;
		return TRUE;
	}
	else if( m_strCurrentStockCode.GetLength() > 0 )
	{
		CStockInfo	info;
		info.SetStockCode( CStock::marketUnknown, m_strCurrentStockCode );
		if( pInfo )
			*pInfo	=	info;
		return TRUE;
	}
	return FALSE;
}

BOOL CStockContainer::GetPrevStock( CStockInfo * pInfo )
{
	CSPMutex::Scoped	l(m_mutex);

	SP_ASSERT( pInfo );

	if( GetSize() == 0 )
		return FALSE;

	if( m_nCurrentStock < 0 || m_nCurrentStock >= GetSize() )
		m_nCurrentStock	=	-1;
	
	if( m_auidSort.GetSize() == GetSize() )
	{
		if( m_nCurrentStock == -1 )
			m_nCurrentStock	=	m_auidSort[m_auidSort.GetSize()-1];
		else
		{
			int i;
			for( i=0; i<m_auidSort.GetSize(); i++ )
			{
				if( m_nCurrentStock == (int)m_auidSort[i] )
					break;
			}
			if( i >= m_auidSort.GetSize() || i == 0 )
			{
				m_nCurrentStock	=	-1;
				return FALSE;
			}
			i --;
			m_nCurrentStock	= m_auidSort[i];
		}
	}
	else
	{
		if( m_nCurrentStock < 0 )
			m_nCurrentStock	=	GetSize()-1;
		else
			m_nCurrentStock	--;
		if( m_nCurrentStock < 0 )
		{
			m_nCurrentStock	=	-1;
			return FALSE;
		}
	}
	
	if( m_nCurrentStock >= 0 && m_nCurrentStock < GetSize() )
	{
		CStockInfo	& info	=	ElementAt(m_nCurrentStock);
		if( pInfo )
			* pInfo	=	info;
		return TRUE;
	}
	else
		m_nCurrentStock	=	-1;
		
	return FALSE;
}

BOOL CStockContainer::GetNextStock( CStockInfo * pInfo )
{
	CSPMutex::Scoped	l(m_mutex);

	SP_ASSERT( pInfo );

	if( GetSize() == 0 )
		return FALSE;

	if( m_nCurrentStock < 0 || m_nCurrentStock >= GetSize() )
		m_nCurrentStock	=	-1;
	
	if( m_auidSort.GetSize() == GetSize() )
	{
		if( m_nCurrentStock == -1 )
			m_nCurrentStock	=	m_auidSort[0];
		else
		{
			int i;
			for( i=0; i<m_auidSort.GetSize(); i++ )
			{
				if( m_nCurrentStock == (int)m_auidSort[i] )
					break;
			}
			i ++;
			if( i >= m_auidSort.GetSize() )
			{
				m_nCurrentStock	=	-1;
				return FALSE;
			}
			m_nCurrentStock	= m_auidSort[i];
		}
	}
	else
	{
		if( m_nCurrentStock < 0 )
			m_nCurrentStock	=	0;
		else
			m_nCurrentStock	++;
		if( m_nCurrentStock >= GetSize() )
		{
			m_nCurrentStock	=	-1;
			return FALSE;
		}
	}
	
	if( m_nCurrentStock >= 0 && m_nCurrentStock < GetSize() )
	{
		CStockInfo	& info	=	ElementAt(m_nCurrentStock);
		if( pInfo )
			* pInfo	=	info;
		return TRUE;
	}
	else
		m_nCurrentStock	=	-1;
		
	return FALSE;
}

CStockInfo & CStockContainer::GetStockInfoByID( int nID )
{
	CSPMutex::Scoped	l(m_mutex);

	if( ID_STOCKCNTN_AVERAGE == nID )
		return m_infoAverage;
	if( ID_STOCKCNTN_WEIGHTAVERAGE == nID )
		return m_infoWeightAverage;
	if( nID >= 0 && nID < GetSize() )
		return ElementAt( nID );
	SP_ASSERT( FALSE );
	return m_infoNull;
}

CStockInfo & CStockContainer::GetStockInfoByIDSort( int nID )
{
	CSPMutex::Scoped	l(m_mutex);

	if( ID_STOCKCNTN_AVERAGE == nID )
		return m_infoAverage;
	if( ID_STOCKCNTN_WEIGHTAVERAGE == nID )
		return m_infoWeightAverage;
	if( m_auidSort.GetSize() == GetSize() 
		&& nID >= 0 && nID < GetSize() )
	{
		if( m_auidSort[nID] >= 0 && (int)m_auidSort[nID] < GetSize() )
			return ElementAt(m_auidSort[nID]);
	}
	else if( nID >= 0 && nID < GetSize() )
		return ElementAt( nID );
	SP_ASSERT( FALSE );
	return m_infoNull;
}

CStockInfo & CStockContainer::GetAverage( )
{
	return m_infoAverage;
}

CStockInfo & CStockContainer::GetWeightAverage( )
{
	return m_infoWeightAverage;
}

BOOL CStockContainer::Load( CStDatabase * pDB, PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd )
{
	CSPMutex::Scoped	l(m_mutex);

	SP_ASSERT( nProgStart <= nProgEnd );
	SP_ASSERT( pDB );
	if( nProgStart > nProgEnd || NULL == pDB )
		return FALSE;

	Clear( );

	int	nLen	=	pDB->LoadCodetable( *this );
	SetMap( );

	pDB->LoadBasetable( *this );
	// pDB->LoadKDataCache( *this, fnCallback, cookie, nProgStart, nProgEnd );

	m_nType		=	typeAll;
	m_dwDate	=	-1;

	OnDataChanged( );
	return TRUE;
}

void CStockContainer::OnDataChanged( )
{
	CSPMutex::Scoped	l(m_mutex);

	DWORD	dateLatest = m_dwDate;
	if( -1 == dateLatest )
		AfxGetStockContainer().GetLatestTechDate( &dateLatest );

	for( int i=0; i<GetSize(); i++ )
	{
		CStockInfo	& info	=	ElementAt(i);
		info.StatBaseIndex( dateLatest );
		info.StatTechIndex( dateLatest );
	}

	SetAverage( );
}

BOOL CStockContainer::ReloadBase( CStDatabase * pDB )
{
	CSPMutex::Scoped	l(m_mutex);

	SP_ASSERT( pDB );
	if( GetSize() <= 0 || NULL == pDB )
		return Load( NULL, NULL );
	pDB->LoadBasetable( *this );

	OnDataChanged();
	return TRUE;
}

BOOL CStockContainer::SetMap( )
{
	CSPMutex::Scoped	l(m_mutex);

	m_map.RemoveAll( );

	if( GetSize() == 0 )
		return TRUE;

	m_map.InitHashTable( GetSize() * 2 + 10 );

	for( int i=0; i<GetSize(); i++ )
	{
		CStockInfo	&info	=	ElementAt(i);
		m_map.SetAt( info.GetStockCode(), (void *)i );
	}
	return TRUE;
}

BOOL CStockContainer::SetAverage( PROGRESS_CALLBACK fnCallback, void *cookie, int nProgStart, int nProgEnd )
{
	CSPMutex::Scoped	l(m_mutex);

	SP_ASSERT( nProgStart <= nProgEnd );
	if( nProgStart > nProgEnd )
		return FALSE;

	CStockInfo & ave = m_infoAverage;
	CStockInfo & wave = m_infoWeightAverage;
	
	ave.Clear( );
	wave.Clear( );

	ave.SetStockCode( CStock::marketUnknown, slh_avrcode );
	ave.SetStockName( slh_avrname );
	wave.SetStockCode( CStock::marketUnknown, slh_wavrcode );
	wave.SetStockName( slh_wavrname );
	
	ave.m_reporttype	=	CStock::reportUnknown;
	wave.m_reporttype	=	CStock::reportUnknown;

	float	fTemp	=	0;

	float	dc[100], wdc[100];
	int i = 0;
	for( i=0; i<sizeof(dc)/sizeof(dc[0]); i++ )		dc[i]	=	0.;
	for( i=0; i<sizeof(wdc)/sizeof(wdc[0]); i++ )	wdc[i]	=	(float)0.0001;

	int		nProgressSegment	=	GetSize() / 25;
	float	dProgressRatio		=	( 0 == GetSize() ? 1 : ((float)(nProgEnd - nProgStart))/GetSize() );

	for( int nCount=0; nCount<GetSize(); nCount++ )
	{
		if( fnCallback && !(nCount % nProgressSegment) )
			fnCallback( PROG_PROGRESS, (int)(nProgStart+nCount*dProgressRatio), NULL, cookie );

		CStockInfo & info = ElementAt(nCount);

		float	factor	=	1; // (float)CStock::GetReportFactor( info.m_reporttype );

		if( CStock::typeshIndex == info.GetType() || CStock::typeszIndex == info.GetType()
			|| CStock::typeshBond == info.GetType() || CStock::typeszBond == info.GetType() )
			continue;

		ave.m_datebase		=	info.m_datebase;
		ave.m_datetech		=	info.m_datetech;
		wave.m_datebase		=	info.m_datebase;
		wave.m_datetech		=	info.m_datetech;

		float	w	=	info.m_fShare_count_total;

		/* 平均值 *********************************************************************/
		// ★技术数据
		int	m = 0, n = 0;
		if( info.m_fLast > 1e-4 )
		{
			ave.m_fLast		=	( ave.m_fLast * dc[m] + info.m_fLast )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fLast	=	( wave.m_fLast * wdc[n] + info.m_fLast * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( info.m_fOpen > 1e-4 )
		{
			ave.m_fOpen		=	( ave.m_fOpen * dc[m] + info.m_fOpen )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fOpen	=	( wave.m_fOpen * wdc[n] + info.m_fOpen * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( info.m_fClose > 1e-4 )
		{	
			ave.m_fClose	=	( ave.m_fClose * dc[m] + info.m_fClose )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fClose	=	( wave.m_fClose * wdc[n] + info.m_fClose * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( info.m_fHigh > 1e-4 )
		{
			ave.m_fHigh		=	( ave.m_fHigh * dc[m] + info.m_fHigh )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fHigh	=	( wave.m_fHigh * wdc[n] + info.m_fHigh * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( info.m_fLow > 1e-4 )
		{
			ave.m_fLow		=	( ave.m_fLow * dc[m] + info.m_fLow )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fLow		=	( wave.m_fLow * wdc[n] + info.m_fLow * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( info.m_fVolume > 1e-4 )
		{
			ave.m_fVolume	=	( ave.m_fVolume * dc[m] + info.m_fVolume )/(dc[m]+1);	dc[m]	+=	1;
		}
		m++;	n++;
		if( info.m_fAmount > 1e-4 )
		{
			ave.m_fAmount	=	( ave.m_fAmount * dc[m] + info.m_fAmount )/(dc[m]+1);	dc[m]	+=	1;
		}

		for( i=0; i<sizeof(ave.m_fBuyPrice)/sizeof(ave.m_fBuyPrice[0]); i++ )
		{
			m++;	n++;
			if( info.m_fBuyPrice[i] > 1e-4 )
			{
				ave.m_fBuyPrice[i]		=	( ave.m_fBuyPrice[i] * dc[m] + info.m_fBuyPrice[i] )/(dc[m]+1);		dc[m]	+=	1;
				wave.m_fBuyPrice[i]		=	( wave.m_fBuyPrice[i] * wdc[n] + info.m_fBuyPrice[i] )/(wdc[n]+1);	wdc[n]	+=	w;
			}
		}
		for( i=0; i<sizeof(ave.m_fBuyVolume)/sizeof(ave.m_fBuyVolume[0]); i++ )
		{
			m++;	n++;
			if( info.m_fBuyVolume[i] > 1e-4 )
			{
				ave.m_fBuyVolume[i]		=	( ave.m_fBuyVolume[i] * dc[m] + info.m_fBuyVolume[i] )/(dc[m]+1);		dc[m]	+=	1;
				wave.m_fBuyVolume[i]	=	( wave.m_fBuyVolume[i] * wdc[n] + info.m_fBuyVolume[i] )/(wdc[n]+1);	wdc[n]	+=	w;
			}
		}
		for( i=0; i<sizeof(ave.m_fSellPrice)/sizeof(ave.m_fSellPrice[0]); i++ )
		{
			m++;	n++;
			if( info.m_fSellPrice[i] > 1e-4 )
			{
				ave.m_fSellPrice[i]		=	( ave.m_fSellPrice[i] * dc[m] + info.m_fSellPrice[i] )/(dc[m]+1);		dc[m]	+=	1;
				wave.m_fSellPrice[i]	=	( wave.m_fSellPrice[i] * wdc[n] + info.m_fSellPrice[i] )/(wdc[n]+1);	wdc[n]	+=	w;
			}
		}
		for( i=0; i<sizeof(ave.m_fSellVolume)/sizeof(ave.m_fSellVolume[0]); i++ )
		{
			m++;	n++;
			if( info.m_fSellVolume[i] > 1e-4 )
			{
				ave.m_fSellVolume[i]	=	( ave.m_fSellVolume[i] * dc[m] + info.m_fSellVolume[i] )/(dc[m]+1);		dc[m]	+=	1;
				wave.m_fSellVolume[i]	=	( wave.m_fSellVolume[i] * wdc[n] + info.m_fSellVolume[i] )/(wdc[n]+1);	wdc[n]	+=	w;
			}
		}

		// ★偿债能力
		m++;	n++;
		if( fabs(info.m_fRatio_liquidity) > 1e-4 )
		{
			ave.m_fRatio_liquidity		=	( ave.m_fRatio_liquidity * dc[m] + info.m_fRatio_liquidity )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fRatio_liquidity		=	( wave.m_fRatio_liquidity * wdc[n] + info.m_fRatio_liquidity * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fRatio_quick) > 1e-4 )
		{
			ave.m_fRatio_quick			=	( ave.m_fRatio_quick * dc[m] + info.m_fRatio_quick )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fRatio_quick			=	( wave.m_fRatio_quick * wdc[n] + info.m_fRatio_quick * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fVelocity_receivables) > 1e-4 )
		{
			ave.m_fVelocity_receivables	=	( ave.m_fVelocity_receivables * dc[m] + info.m_fVelocity_receivables )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fVelocity_receivables=	( wave.m_fVelocity_receivables * wdc[n] + info.m_fVelocity_receivables * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}

		// ★经营能力
		m++;	n++;
		if( fabs(info.m_fVelocity_merchandise) > 1e-4 )
		{
			ave.m_fVelocity_merchandise	=	( ave.m_fVelocity_merchandise * dc[m] + info.m_fVelocity_merchandise )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fVelocity_merchandise=	( wave.m_fVelocity_merchandise * wdc[n] + info.m_fVelocity_merchandise * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fMain_income) > 1e-4 )
		{
			ave.m_fMain_income			=	( ave.m_fMain_income * dc[m] + factor*info.m_fMain_income )/(dc[m]+1);		dc[m]	+=	1;
		}
		m++;	n++;
		if( fabs(info.m_fCash_ps) > 1e-4 )
		{
			ave.m_fCash_ps				=	( ave.m_fCash_ps * dc[m] + factor*info.m_fCash_ps )/(dc[m]+1);				dc[m]	+=	1;
			wave.m_fCash_ps				=	( wave.m_fCash_ps * wdc[n] + factor * info.m_fCash_ps * w )/(wdc[n]+w);		wdc[n]	+=	w;
		}

		// ★盈利能力
		m++;	n++;
		if( fabs(info.m_fProfit_margin) > 1e-4 )
		{
			ave.m_fProfit_margin	=	( ave.m_fProfit_margin * dc[m] + info.m_fProfit_margin )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fProfit_margin	=	( wave.m_fProfit_margin * wdc[n] + info.m_fProfit_margin * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fNetasset_yield) > 1e-4 )
		{
			ave.m_fNetasset_yield	=	( ave.m_fNetasset_yield * dc[m] + factor*info.m_fNetasset_yield )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fNetasset_yield	=	( wave.m_fNetasset_yield * wdc[n] + factor * info.m_fNetasset_yield * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}

		// ★资本结构
		m++;	n++;
		if( info.m_fShare_count_total > 1e-4 )
		{
			ave.m_fShare_count_total	=	( ave.m_fShare_count_total * dc[m] + info.m_fShare_count_total )/(dc[m]+1);	dc[m]	+=	1;
		}
		m++;	n++;
		if( info.m_fShare_count_a > 1e-4 )
		{
			ave.m_fShare_count_a		=	( ave.m_fShare_count_a * dc[m] + info.m_fShare_count_a )/(dc[m]+1);			dc[m]	+=	1;
		}
		m++;	n++;
		if( info.m_fShare_count_b > 1e-4 )
		{
			ave.m_fShare_count_b		=	( ave.m_fShare_count_b * dc[m] + info.m_fShare_count_b )/(dc[m]+1);			dc[m]	+=	1;
		}
		m++;	n++;
		if( info.m_fShare_count_h > 1e-4 )
		{
			ave.m_fShare_count_h		=	( ave.m_fShare_count_h * dc[m] + info.m_fShare_count_h )/(dc[m]+1);			dc[m]	+=	1;
		}
		m++;	n++;
		if( info.m_fShare_count_national > 1e-4 )
		{
			ave.m_fShare_count_national	=	( ave.m_fShare_count_national * dc[m] + info.m_fShare_count_national )/(dc[m]+1);	dc[m]	+=	1;
		}
		m++;	n++;
		if( info.m_fShare_count_corp > 1e-4 )
		{
			ave.m_fShare_count_corp		=	( ave.m_fShare_count_corp * dc[m] + info.m_fShare_count_corp )/(dc[m]+1);	dc[m]	+=	1;
		}
		m++;	n++;
		if( fabs(info.m_fProfit_psud) > 1e-4 )
		{
			ave.m_fProfit_psud			=	( ave.m_fProfit_psud * dc[m] + info.m_fProfit_psud )/(dc[m]+1);				dc[m]	+=	1;
			wave.m_fProfit_psud			=	( wave.m_fProfit_psud * wdc[n] + info.m_fProfit_psud * w )/(wdc[n]+w);		wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fAsset) > 1e-4 )
		{
			ave.m_fAsset				=	( ave.m_fAsset * dc[m] + info.m_fAsset )/(dc[m]+1);							dc[m]	+=	1;
		}
		m++;	n++;
		if( fabs(info.m_fRatio_holderright) > 1e-4 )
		{
			ave.m_fRatio_holderright	=	( ave.m_fRatio_holderright * dc[m] + info.m_fRatio_holderright )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fRatio_holderright	=	( wave.m_fRatio_holderright * wdc[n] + info.m_fRatio_holderright * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fRatio_longdebt) > 1e-4 )
		{
			ave.m_fRatio_longdebt		=	( ave.m_fRatio_longdebt * dc[m] + info.m_fRatio_longdebt )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fRatio_longdebt		=	( wave.m_fRatio_longdebt * wdc[n] + info.m_fRatio_longdebt * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fRatio_debt) > 1e-4 )
		{
			ave.m_fRatio_debt			=	( ave.m_fRatio_debt * dc[m] + info.m_fRatio_debt )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fRatio_debt			=	( wave.m_fRatio_debt * wdc[n] + info.m_fRatio_debt * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}

		// ★投资收益能力
		m++;	n++;
		if( fabs(info.m_fNetasset_ps) > 1e-4 )
		{
			ave.m_fNetasset_ps		=	( ave.m_fNetasset_ps * dc[m] + info.m_fNetasset_ps )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fNetasset_ps		=	( wave.m_fNetasset_ps * wdc[n] + info.m_fNetasset_ps * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fNetasset_ps_regulate) > 1e-4 )
		{
			ave.m_fNetasset_ps_regulate	=	( ave.m_fNetasset_ps_regulate * dc[m] + info.m_fNetasset_ps_regulate )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fNetasset_ps_regulate=	( wave.m_fNetasset_ps_regulate * wdc[n] + info.m_fNetasset_ps_regulate * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fEps) > 1e-4 )
		{
			ave.m_fEps			=	( ave.m_fEps * dc[m] + factor*info.m_fEps )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fEps			=	( wave.m_fEps * wdc[n] + factor * info.m_fEps * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fEps_deduct) > 1e-4 )
		{
			ave.m_fEps_deduct	=	( ave.m_fEps_deduct * dc[m] + factor*info.m_fEps_deduct )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fEps_deduct	=	( wave.m_fEps_deduct * wdc[n] + factor * info.m_fEps_deduct * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}
		m++;	n++;
		if( fabs(info.m_fNet_profit) > 1e-4 )
		{
			ave.m_fNet_profit	=	( ave.m_fNet_profit * dc[m] + factor*info.m_fNet_profit )/(dc[m]+1);	dc[m]	+=	1;
		}
		m++;	n++;
		if( fabs(info.m_fMain_profit) > 1e-4 )
		{
			ave.m_fMain_profit	=	( ave.m_fMain_profit * dc[m] + factor*info.m_fMain_profit )/(dc[m]+1);	dc[m]	+=	1;
		}
		m++;	n++;
		if( fabs(info.m_fTotal_profit) > 1e-4 )
		{
			ave.m_fTotal_profit	=	( ave.m_fTotal_profit * dc[m] + factor*info.m_fTotal_profit )/(dc[m]+1);dc[m]	+=	1;
		}

		// ★增长率
		m++;	n++;
		ave.m_fProfit_inc	=	( ave.m_fProfit_inc * dc[m] + info.m_fProfit_inc )/(dc[m]+1);			dc[m]	+=	1;
		wave.m_fProfit_inc	=	( wave.m_fProfit_inc * wdc[n] + info.m_fProfit_inc * w )/(wdc[n]+w);	wdc[n]	+=	w;

		m++;	n++;
		ave.m_fIncome_inc	=	( ave.m_fIncome_inc * dc[m] + info.m_fIncome_inc )/(dc[m]+1);			dc[m]	+=	1;
		wave.m_fIncome_inc	=	( wave.m_fIncome_inc * wdc[n] + info.m_fIncome_inc * w )/(wdc[n]+w);	wdc[n]	+=	w;

		m++;	n++;
		ave.m_fAsset_inc	=	( ave.m_fAsset_inc * dc[m] + info.m_fAsset_inc )/(dc[m]+1);			dc[m]	+=	1;
		wave.m_fAsset_inc	=	( wave.m_fAsset_inc * wdc[n] + info.m_fAsset_inc * w )/(wdc[n]+w);	wdc[n]	+=	w;

		// ★技术数据统计值
		m++;	n++;
		if( fabs(STKLIB_DATA_INVALID - info.m_fYield_average) > 1e-4 )
		{
			ave.m_fYield_average	=	( ave.m_fYield_average * dc[m] + info.m_fYield_average )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fYield_average	=	( wave.m_fYield_average * wdc[n] + info.m_fYield_average * w )/(wdc[n]+w);	wdc[n]	+=	w;
			m++;	n++;
			if( fabs(STKLIB_DATA_INVALID - info.m_fYield_stddev) > 1e-4 )
			{
				ave.m_fYield_stddev	=	( ave.m_fYield_stddev * dc[m] + info.m_fYield_stddev )/(dc[m]+1);			dc[m]	+=	1;
				wave.m_fYield_stddev=	( wave.m_fYield_stddev * wdc[n] + info.m_fYield_stddev * w )/(wdc[n]+w);	wdc[n]	+=	w;
			}
		}
		m++;	n++;
		if( fabs(STKLIB_DATA_INVALID - info.m_fBeite) > 1e-4 )
		{
			ave.m_fBeite	=	( ave.m_fBeite * dc[m] + info.m_fBeite )/(dc[m]+1);			dc[m]	+=	1;
			wave.m_fBeite	=	( wave.m_fBeite * wdc[n] + info.m_fBeite * w )/(wdc[n]+w);	wdc[n]	+=	w;
		}

		// K线数据
		m++;	n++;
		if( 0 == nCount )
		{
			m_infoAverage.m_kdata		=	info.m_kdata;
			m_infoWeightAverage.m_kdata	=	info.m_kdata;
		}
		for( int k=info.m_kdata.GetSize()-1; k>=0; k-- )
		{
			KDATA	&	kdInfo	=	info.m_kdata.ElementAt(k);
			// 平均值
			int	nIndex = k;
			if( nIndex >= m_infoAverage.m_kdata.GetSize() || m_infoAverage.m_kdata.ElementAt(nIndex).m_date != kdInfo.m_date )
				nIndex	=	m_infoAverage.m_kdata.GetIndexByDate(kdInfo.m_date);
			if( -1 == nIndex )
			{
				KDATA	kdNew;
				memset( &kdNew, 0, sizeof(kdNew) );
				kdNew.m_date	=	kdInfo.m_date;
				nIndex	=	m_infoAverage.m_kdata.InsertKDataSort(kdNew);
			}
			if( -1 == nIndex )
				continue;
			KDATA	&	kdAve	=	m_infoAverage.m_kdata.ElementAt(nIndex);
			kdAve.m_fOpen	=	(kdAve.m_fOpen * dc[m] + kdInfo.m_fOpen)/(dc[m]+1);
			kdAve.m_fHigh	=	(kdAve.m_fHigh * dc[m] + kdInfo.m_fHigh)/(dc[m]+1);
			kdAve.m_fLow	=	(kdAve.m_fLow * dc[m] + kdInfo.m_fLow)/(dc[m]+1);
			kdAve.m_fClose	=	(kdAve.m_fClose * dc[m] + kdInfo.m_fClose)/(dc[m]+1);
			kdAve.m_fVolume	=	(kdAve.m_fVolume * dc[m] + kdInfo.m_fVolume)/(dc[m]+1);
			kdAve.m_fAmount	=	(kdAve.m_fAmount * dc[m] + kdInfo.m_fAmount)/(dc[m]+1);

			// 加权平均值
			nIndex = k;
			if( nIndex >= m_infoWeightAverage.m_kdata.GetSize() || m_infoWeightAverage.m_kdata.ElementAt(nIndex).m_date != kdInfo.m_date )
				nIndex	=	m_infoWeightAverage.m_kdata.GetIndexByDate(kdInfo.m_date);
			if( -1 == nIndex )
			{
				KDATA	kdNew;
				memset( &kdNew, 0, sizeof(kdNew) );
				kdNew.m_date	=	kdInfo.m_date;
				nIndex	=	m_infoWeightAverage.m_kdata.InsertKDataSort(kdNew);
			}
			if( -1 == nIndex )
				continue;
			KDATA	&	kdWAve	=	m_infoWeightAverage.m_kdata.ElementAt(nIndex);
			kdWAve.m_fOpen		=	(kdWAve.m_fOpen * wdc[n] + kdInfo.m_fOpen * w)/(wdc[n]+w);
			kdWAve.m_fHigh		=	(kdWAve.m_fHigh * wdc[n] + kdInfo.m_fHigh * w)/(wdc[n]+w);
			kdWAve.m_fLow		=	(kdWAve.m_fLow * wdc[n] + kdInfo.m_fLow * w)/(wdc[n]+w);
			kdWAve.m_fClose		=	(kdWAve.m_fClose * wdc[n] + kdInfo.m_fClose * w)/(wdc[n]+w);
			kdWAve.m_fVolume	=	(kdWAve.m_fVolume * wdc[n] + kdInfo.m_fVolume * w)/(wdc[n]+w);
			kdWAve.m_fAmount	=	(kdWAve.m_fAmount * wdc[n] + kdInfo.m_fAmount * w)/(wdc[n]+w);
		}
		dc[m]	+=	1;
		wdc[n]	+=	w;

		SP_ASSERT( m <= sizeof(dc)/sizeof(dc[0]) );
		SP_ASSERT( n <= sizeof(wdc)/sizeof(wdc[0]) );
	
	}

	if( fnCallback )
		fnCallback( PROG_PROGRESS, nProgEnd, NULL, cookie );

	return TRUE;
}

BOOL CStockContainer::Clear( )
{
	CSPMutex::Scoped	l(m_mutex);

	ClearVariantSaveValue( );

	// WARNING: Cannot clear this m_nCurrentStock	=	-1;
	// WARNING: Cannot clear this m_strCurrentStockCode.Empty();
	m_nType		=	typeNone;
	m_strDomain.Empty();
	m_dwDate = -1;
	RemoveAll();
	m_map.RemoveAll( );
	m_infoAverage.Clear( );
	m_infoWeightAverage.Clear( );

	m_auidSort.RemoveAll();
	return TRUE;
}

void CStockContainer::CopyData( CStockContainer & src )
{
	CSPMutex::Scoped	l(m_mutex);

	SetSize( src.GetSize() );
	for( int i=0; i<src.GetSize(); i++ )
	{
		CStockInfo	& info	=	src.ElementAt(i);
		SetAt( i, info );
	}
}

int CStockContainer::SortFunction(const void *s1,const void *s2)
{
	CStockInfo *pStock1 = (CStockInfo *)s1;
	CStockInfo *pStock2 = (CStockInfo *)s2;

	if( NULL == pStock2 )
		return 1;

	if( NULL == pStock1 )
		return -1;

	if( NULL == CStockContainer::m_pSortContainer )
		return 1;

	int nRet = AfxCompareVariantValue(	CStockContainer::m_pSortContainer->m_nSortVariantID, *pStock1, *pStock2,
										CStockContainer::m_pSortContainer );
	if( CStockContainer::m_pSortContainer->m_bSortAscend )
		return nRet;
	if( nRet > 0 )
		return -1;
	else if( nRet < 0 )
		return 1;
	return 0;
}

int CStockContainer::StockInfoCompareFunc( const void *s1,const void *s2, CStockContainer * pContainer, int nSortVariantID )
{
	CStockInfo *pStock1 = (CStockInfo *)s1;
	CStockInfo *pStock2 = (CStockInfo *)s2;

	if( NULL == pStock2 )
		return 1;

	if( NULL == pStock1 )
		return -1;

	return AfxCompareVariantValue(	nSortVariantID, *pStock1, *pStock2, pContainer );
}


BOOL CStockContainer::Sort( int nSortVariantID, BOOL bAscend )
{
	CSPMutex::Scoped	l(m_mutex);

	m_auidSort.RemoveAll();	// if call this Sort(...), then not use m_auidSort
	
	CStockContainer::m_pSortContainer	=	this;
	m_nSortVariantID	=	nSortVariantID;
	m_bSortAscend		=	bAscend;

	if( NULL != GetData() )
		qsort( GetData(), GetSize(), sizeof(CStockInfo), SortFunction );
	return TRUE;
}

void CStockContainer::SetSortID( CSPDWordArray & auidsort )
{
	CSPMutex::Scoped	l(m_mutex);

	m_auidSort.RemoveAll();
	m_auidSort.Copy( auidsort );
}

BOOL CStockContainer::GetMultiSortIDArray( CSPDWordArray & adwSortID, LONG lStockType, UINT nSLH, BOOL bAsc, int nCount )
{
	CSPMutex::Scoped	l(m_mutex);

	SP_ASSERT( nCount > 0 && nSLH >= SLH_MIN && nSLH <= SLH_MAX );
	if( nCount <= 0 || nSLH < SLH_MIN || nSLH > SLH_MAX )
		return FALSE;

	adwSortID.RemoveAll();

	for( int n=0; n<nCount; n++ )
	{
		int		nMinMaxIndex	=	-1;
		double	dMinMaxValue	=	0;
		BOOL	bInited	=	FALSE;
		for( int i=0; i<GetSize(); i++ )
		{
			CStockInfo & info	=	ElementAt(i);

			// 股票类型
			if( info.GetType() != lStockType )
				continue;

			// 如果加过，则继续寻找下一个
			int k;
			for( k=0; k<adwSortID.GetSize(); k++ )
			{
				if( adwSortID[k] == (DWORD)i )
					break;
			}
			if( k != adwSortID.GetSize() )
				continue;

			// 判断大小
			double	dValue	=	0;
			if( AfxGetVariantValue( nSLH, info, &dValue, NULL ) )
			{
				if( !bInited )
				{
					nMinMaxIndex	=	i;
					dMinMaxValue	=	dValue;
					bInited	=	TRUE;
				}
				if( bAsc && dValue < dMinMaxValue )
				{
					nMinMaxIndex	=	i;
					dMinMaxValue	=	dValue;
				}
				if( !bAsc && dValue > dMinMaxValue )
				{
					nMinMaxIndex	=	(DWORD)i;
					dMinMaxValue	=	dValue;
				}
			}
		}

		if( -1 == nMinMaxIndex )
			break;
		adwSortID.Add( nMinMaxIndex );
	}

	return TRUE;
}

BOOL CStockContainer::GetVariantSaveValue( double *pValue, UINT nVariantID, CStockInfo &info, BOOL *pNoValue )
{
	CSPMutex::Scoped	l(m_mutex);

	void * pArrayID	=	NULL;
	if( !m_map.Lookup( info.GetStockCode(), pArrayID ) )
		return FALSE;

	int	nIndex	=	(int)pArrayID;
	if( nIndex >= 0 && nIndex < m_aptrSaveValueArray.GetSize() )
	{
		CSPPtrArray * pArray = (CSPPtrArray *)m_aptrSaveValueArray.ElementAt( nIndex );
		if( pArray )
		{
			VARIANT_SAVEVALUE * pSave = NULL;
			for( int j=0; j<pArray->GetSize(); j++ )
			{
				pSave = (VARIANT_SAVEVALUE *) pArray->GetAt(j);
				if( pSave->nVariantID == nVariantID )
				{
					if( pValue )	*pValue		=	pSave->Value;
					if( pNoValue )	*pNoValue	=	pSave->bNoValue;
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

void CStockContainer::SetVariantSaveValue( double Value, UINT nVariantID, CStockInfo &info, BOOL bNoValue )
{
	CSPMutex::Scoped	l(m_mutex);

	void * pArrayID	=	NULL;
	if( !m_map.Lookup( info.GetStockCode(), pArrayID ) )
		return;

	// alloc for m_aptrSaveValueArray
	if( m_aptrSaveValueArray.GetSize() == 0 )
	{
		m_aptrSaveValueArray.SetSize( GetSize() );
		for( int i=0; i<m_aptrSaveValueArray.GetSize(); i++ )
			m_aptrSaveValueArray.SetAt( i, NULL );
	}

	int	nIndex	=	(int)pArrayID;
	if( nIndex >= 0 && nIndex < m_aptrSaveValueArray.GetSize() )
	{
		CSPPtrArray * pArray = (CSPPtrArray *)m_aptrSaveValueArray.ElementAt( nIndex );
		if( NULL == pArray )
		{
			pArray	=	new	CSPPtrArray;
			m_aptrSaveValueArray.SetAt( nIndex, pArray );
		}
		
		// 如果已经存在，覆盖之
		for( int j=0; j<pArray->GetSize(); j++ )
		{
			VARIANT_SAVEVALUE * pSave = (VARIANT_SAVEVALUE *) pArray->GetAt(j);
			if( pSave->nVariantID == nVariantID )
			{
				pSave->Value	=	Value;
				pSave->bNoValue	=	bNoValue;
				return;
			}
		}
		
		// 加入新的
		VARIANT_SAVEVALUE * pSaveValue = new VARIANT_SAVEVALUE;
		pSaveValue->nVariantID	=	nVariantID;
		pSaveValue->Value		=	Value;
		pSaveValue->bNoValue	=	bNoValue;
		pArray->Add( pSaveValue );
	}
}

void CStockContainer::ClearVariantSaveValue( )
{
	CSPMutex::Scoped	l(m_mutex);

	for( int i=0; i<m_aptrSaveValueArray.GetSize(); i++ )
	{
		CSPPtrArray * pArray = (CSPPtrArray *)m_aptrSaveValueArray.ElementAt( i );
		if( pArray )
		{
			for( int j=0; j<pArray->GetSize(); j++ )
				delete	pArray->GetAt(j);
			delete	pArray;
		}
	}
	m_aptrSaveValueArray.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// class CDomain

CDomain::CDomain( )
{
}

CDomain::CDomain( const CDomain &src )
{
	*this	=	src;
}

CDomain::~CDomain( )
{
}

BOOL CDomain::AddStock( LPCTSTR lpszStockCode )
{
	if( NULL == lpszStockCode || strlen(lpszStockCode) <= 0 )
		return FALSE;

	for( int k=0; k<GetSize(); k++ )
	{
		int nCmp = ElementAt(k).CompareNoCase(lpszStockCode);
		if( 0 == nCmp )
			return FALSE;
	}

	Add( lpszStockCode );
	return TRUE;
}

BOOL CDomain::AddStockSort( LPCTSTR lpszStockCode )
{
	if( NULL == lpszStockCode || strlen(lpszStockCode) <= 0 )
		return FALSE;

	for( int k=0; k<GetSize(); k++ )
	{
		int nCmp = ElementAt(k).CompareNoCase(lpszStockCode);
		if( 0 == nCmp )
			return FALSE;
		if( nCmp > 0 )
		{
			InsertAt(k, lpszStockCode );
			return TRUE;
		}
	}

	Add( lpszStockCode );
	return TRUE;
}

BOOL CDomain::RemoveStock( LPCTSTR lpszStockCode )
{
	if( NULL == lpszStockCode || strlen(lpszStockCode) <= 0 )
		return FALSE;

	for( int k=0; k<GetSize(); k++ )
	{
		if( 0 == ElementAt(k).CompareNoCase(lpszStockCode) )
		{
			RemoveAt(k);
			return TRUE;
		}
	}
	return FALSE;
}

CDomain &CDomain::operator = ( const CDomain &src )
{
	m_strName		=	src.m_strName;
	Copy( src );
	return *this;
}

struct fxj_block_t {
	WORD	wMarket;
	char	szCode[10];
};

int CDomain::AddFxjDomain( LPCTSTR lpszFile )
{
	CSPFile file;
	DWORD	dwMagic;
	if( !file.Open( lpszFile, CSPFile::modeRead )
		|| sizeof(dwMagic) != file.Read(&dwMagic,sizeof(dwMagic)) || dwMagic != 0xFF5100A5 )
		return 0;

	int nCount = 0;
	struct fxj_block_t stock;
	while( sizeof(stock) == file.Read(&stock,sizeof(stock)) )
	{
		if( 0 == stock.szCode[6] )
		{
			CSPString sCode = stock.szCode;
			if( sCode.GetLength() == 4 )
				sCode = "00" + sCode;
			if( sCode.GetLength() == 6 )
				AddStock( sCode );
		}
	}
	
	return GetSize();
}


//////////////////////////////////////////////////////////////////////
// class CDomainContainer

CDomainContainer::CDomainContainer( )
{
}

CDomainContainer::CDomainContainer( CDomainContainer & src )
{
	*this	=	src;
}

CDomainContainer::~CDomainContainer( )
{
	RemoveAll( );
}

CDomainContainer & CDomainContainer::operator = ( CDomainContainer & src )
{
	SetSize( src.GetSize() );
	for( int i=0; i<src.GetSize(); i++ )
	{
		CDomain domain = src.ElementAt(i);
		SetAt( i, domain );
	}
	return *this;
}

int CDomainContainer::AddDomain( LPCTSTR lpszDomainName )
{
	if( NULL == lpszDomainName || strlen(lpszDomainName) <=0  )
		return -1;
	CDomain	newdomain;
	newdomain.m_strName	=	lpszDomainName;
	return AddDomain( newdomain );
}

int CDomainContainer::AddDomain( CDomain & newdomain )
{
	if( 0 == newdomain.m_strName.GetLength() )
		return -1;

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain & domain = ElementAt(i);
		if( 0 == newdomain.m_strName.CompareNoCase( domain.m_strName ) )
			return -1;
	}
	return Add( newdomain );
}

int CDomainContainer::AddDomainReplace( CDomain & newdomain )
{
	if( 0 == newdomain.m_strName.GetLength() )
		return -1;

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain & domain = ElementAt(i);
		if( 0 == newdomain.m_strName.CompareNoCase( domain.m_strName ) )
		{
			SetAt( i, newdomain );
			return i;
		}
	}
	return Add( newdomain );
}

int CDomainContainer::AddDomainReplace( CDomainContainer & newdomains )
{
	int nCount = 0;
	for( int i=0; i<newdomains.GetSize(); i++ )
	{
		if( AddDomainReplace( newdomains.ElementAt(i) ) >= 0 )
			nCount ++;
	}
	return nCount;
}

BOOL CDomainContainer::RemoveDomain( LPCTSTR lpszDomainName )
{
	if( NULL == lpszDomainName || strlen(lpszDomainName) <= 0 )
		return FALSE;

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain & domain = ElementAt(i);
		if( 0 == domain.m_strName.CompareNoCase( lpszDomainName ) )
		{
			RemoveAt( i );
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CDomainContainer::AddDomainStock( LPCTSTR lpszDomainName, LPCTSTR lpszStockCode )
{
	if( NULL == lpszDomainName || strlen(lpszDomainName) < 0
		|| NULL == lpszStockCode || strlen(lpszStockCode) < 0 )
		return FALSE;

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain	& domain	=	ElementAt(i);
		if( 0 == domain.m_strName.CompareNoCase( lpszDomainName ) )
		{
			return domain.AddStock( lpszStockCode );
		}
	}

	return FALSE;
}

BOOL CDomainContainer::AddDomainStock( LPCTSTR lpszDomainName, CSPStringArray & astr )
{
	if( NULL == lpszDomainName || strlen(lpszDomainName) < 0 )
		return FALSE;

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain	& domain	=	ElementAt(i);
		if( 0 == domain.m_strName.CompareNoCase( lpszDomainName ) )
		{
			for( int j=0; j<astr.GetSize(); j++ )
				domain.AddStock( astr.ElementAt(j) );
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDomainContainer::RemoveDomainStock( LPCTSTR lpszDomainName, LPCTSTR lpszStockCode )
{
	if( NULL == lpszDomainName || strlen(lpszDomainName) < 0
		|| NULL == lpszStockCode || strlen(lpszStockCode) < 0 )
		return FALSE;

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain	& domain	=	ElementAt(i);
		if( 0 == domain.m_strName.CompareNoCase( lpszDomainName ) )
		{
			for( int k=0; k<domain.GetSize(); k++ )
			{
				if( 0 == domain.ElementAt(k).CompareNoCase(lpszStockCode) )
				{
					domain.RemoveAt(k);
					return TRUE;
				}
			}
			break;
		}
	}

	return FALSE;
}

BOOL CDomainContainer::RemoveDomainStockAll( LPCTSTR lpszDomainName )
{
	if( NULL == lpszDomainName || strlen(lpszDomainName) < 0 )
		return FALSE;

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain	& domain	=	ElementAt(i);
		if( 0 == domain.m_strName.CompareNoCase( lpszDomainName ) )
		{
			domain.RemoveAll();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDomainContainer::GetDomainStocks( LPCTSTR lpszDomain, CSPStringArray &astr )
{
	if( NULL == lpszDomain || strlen(lpszDomain) < 0 )
		return FALSE;

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain	& domain	=	ElementAt(i);
		if( 0 == domain.m_strName.CompareNoCase( lpszDomain ) )
		{
			astr.Copy( domain );
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDomainContainer::GetDomains( CSPStringArray & astr )
{
	astr.SetSize( GetSize() );

	for( int i=0; i<GetSize(); i++ )
	{
		CDomain	& domain	=	ElementAt(i);
		CSPString	string	=	domain.m_strName;
		astr.SetAt( i, string );
	}
	return TRUE;
}

BOOL CDomainContainer::GetAllDomainInfo( CStockContainer * pContainer, DWORD dwDate )
{
	SP_ASSERT( pContainer );
	if( NULL == pContainer )
		return FALSE;

	DWORD	dwDateLatest = -1;
	AfxGetStockContainer().GetLatestTechDate( &dwDateLatest );

	for( int nDomain=0; nDomain<GetSize(); nDomain++ )
	{
		CDomain	&	domain	=	ElementAt(nDomain);
		CStockContainer	cntn;
		if( cntn.RetrieveSpecify( domain ) )
		{
			if( -1 != dwDate && dwDateLatest != dwDate )
			{
				for( int i=0; i<cntn.GetSize(); i++ )
				{
					CStockInfo	& info	=	cntn.ElementAt(i);
					info.StatBaseIndex( dwDate );
					info.StatTechIndex( dwDate );
				}
			}
			// cntn.SetMap( );
			cntn.SetAverage( );

			CStockInfo	info	=	cntn.GetAverage( );
			info.SetStockName( domain.m_strName );
			info.SetStockShortName( domain.m_strName );
			pContainer->Add( info );
		}
	}
	pContainer->SetMap();
	pContainer->SetAverage();
	return TRUE;
}

#define	SF_MAX_DOMAINFILE_LENGTH	0x10000000

BOOL CDomainContainer::Load( LPCTSTR lpszFileName )
{
	if( NULL == lpszFileName || strlen(lpszFileName) <= 0 )
		return FALSE;

	BOOL	bOK	=	FALSE;
	CSPFile file;
	
	if( file.Open( lpszFileName, CSPFile::modeRead ) )
	{
		DWORD dwLen = file.GetLength();
		if( 0 == dwLen || dwLen > SF_MAX_DOMAINFILE_LENGTH )
		{
			file.Close();
			return FALSE;
		}

		SetSize( 0, min(dwLen / 300, 20) );

		CDomain	domain;
		CSPString	rString;
		while( file.ReadString( rString ) )
		{
			int	nIndex	=	rString.Find( "\r" );
			if( -1 == nIndex )
				nIndex	=	rString.Find( "\n" );
			if( -1 != nIndex )
				rString	=	rString.Left( nIndex );
			rString.TrimLeft();
			rString.TrimRight();

			if( rString.GetLength() > 0 )
			{
				if( domain.m_strName.IsEmpty() )	// 新版块名称
					domain.m_strName	=	rString;
				else
					domain.AddStock( rString );
			}
			else
			{	// 版块结束
				if( !domain.m_strName.IsEmpty() )
					AddDomain( domain );
				domain.m_strName.Empty();
				domain.RemoveAll();
			}
		}
		if( !domain.m_strName.IsEmpty() )
			Add( domain );
		domain.m_strName.Empty();
		domain.RemoveAll();

		file.Close();
		bOK	=	TRUE;
	}

	return bOK;
}

BOOL CDomainContainer::Store( LPCTSTR lpszFileName )
{
	if( NULL == lpszFileName || strlen(lpszFileName) <= 0 )
		return FALSE;

	CSPFile file;
	if( file.Open( lpszFileName, CSPFile::modeCreate | CSPFile::modeWrite ) )
	{
		for( int i=0; i<GetSize(); i++ )
		{
			CDomain	& domain = ElementAt(i);
			CSPString	strDomain	=	domain.m_strName;
			file.Write( strDomain.GetBuffer(strDomain.GetLength()+1), strDomain.GetLength() );
			file.Write( STRING_CRLF, strlen(STRING_CRLF) );
			for( int k=0; k<domain.GetSize(); k++ )
			{
				CSPString	strStock	=	domain[k];
				if( strStock.GetLength() > 0 )
				{
					file.Write( strStock.GetBuffer(strStock.GetLength()), strStock.GetLength() );
					file.Write( STRING_CRLF, strlen(STRING_CRLF) );
				}
			}
			file.Write( STRING_CRLF, strlen(STRING_CRLF) );
		}
		
		file.Close();
		return TRUE;
	}
	return FALSE;
}

