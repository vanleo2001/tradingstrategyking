/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

*/

#include	"StdAfx.h"
#include	"../Include/Stock.h"
#include	"../Include/Technique.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//	CCYO
CCYO::CCYO( )
{
	SetDefaultParameters( );
}

CCYO::CCYO( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CCYO::~CCYO()
{
	Clear( );
}

void CCYO::SetDefaultParameters( )
{
	m_adwMTMDays.RemoveAll();
	m_adwMTMDays.Add( 9 );
	m_adwMTMDays.Add( 12 );
	m_adwMTMDays.Add( 18 );
	m_adwMTMDays.Add( 24 );
	m_nMADays	=	6;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CCYO::AttachParameters( CCYO & src )
{
	m_adwMTMDays.Copy( src.m_adwMTMDays );
	m_nMADays		=	src.m_nMADays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CCYO::IsValidParameters( )
{
	STT_VALID_DAYSARRAY( m_adwMTMDays );
	return ( VALID_DAYS(m_nMADays) && VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CCYO::Clear( )
{
	CTechnique::Clear( );
}

int CCYO::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	AfxGetMaxDays( m_adwMTMDays )+m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.2, 0.6 ) )
		return ITS_NOTHING;

	if( nIndex <= 1 )
		return ITS_NOTHING;

	double	dValue;
	if( !Calculate( &dValue, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dValue < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dValue > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return ITS_NOTHING;
}

BOOL CCYO::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	       9日MTM + 12日MTM×2 + 18日MTM×3 + 24日MTM×4
	CYO = ------------------------------------------------
	                           10
*/
BOOL CCYO::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	UINT	nMaxDays	=	AfxGetMaxDays( m_adwMTMDays );
	if( (int)nMaxDays > nIndex )
		return FALSE;

	double	dValue = 0;
	int	nCount	=	0;
	for( int m=0; m<m_adwMTMDays.GetSize(); m++ )
	{
		if( int(nIndex-m_adwMTMDays[m]) < 0 )
			return FALSE;

		double dMTM = 100;
		if( m_pKData->MaindataAt(nIndex-m_adwMTMDays[m]) > 0 )
			dMTM	=	100. * m_pKData->MaindataAt(nIndex) / m_pKData->MaindataAt(nIndex-m_adwMTMDays[m]);

		dValue	+=	dMTM*(m+1);
		nCount	+=	(m+1);
	}

	if( nCount <= 0 )
		return FALSE;

	dValue	=	dValue / nCount;

	if( pValue )
		*pValue	=	dValue;
	StoreToCache( nIndex, pValue );
	return TRUE;
}

/***
	计算CYO及其移动平均值
*/
BOOL CCYO::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CDCYO
CDCYO::CDCYO( )
{
	SetDefaultParameters( );
}

CDCYO::CDCYO( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CDCYO::~CDCYO()
{
	Clear( );
}

void CDCYO::SetDefaultParameters( )
{
	m_adwMTMDays.RemoveAll();
	m_adwMTMDays.Add( 9 );
	m_adwMTMDays.Add( 12 );
	m_adwMTMDays.Add( 18 );
	m_adwMTMDays.Add( 24 );
	m_nMADays		=	6;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CDCYO::AttachParameters( CDCYO & src )
{
	m_adwMTMDays.Copy( src.m_adwMTMDays );
	m_nMADays		=	src.m_nMADays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CDCYO::IsValidParameters( )
{
	STT_VALID_DAYSARRAY( m_adwMTMDays );
	return ( VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CDCYO::Clear( )
{
	CTechnique::Clear( );
}

int CDCYO::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	AfxGetMaxDays( m_adwMTMDays )+m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.45, 0.55 ) )
		return ITS_NOTHING;

	if( nIndex <= 1 )
		return ITS_NOTHING;

	double	dValue;
	if( !Calculate( &dValue, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dValue < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dValue > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return ITS_NOTHING;
}

BOOL CDCYO::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	       9日MTM + 12日MTM×2 + 18日MTM×3 + 24日MTM×4
	CYO = ------------------------------------------------
	                           10
	DCYO = CYO的m_nMADays日平均值
*/
BOOL CDCYO::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	UINT	nMaxDays	=	AfxGetMaxDays( m_adwMTMDays );
	if( (int)nMaxDays > nIndex )
		return FALSE;

	int	nMACount	=	0;
	double	dMA = 0;
	for( int k=nIndex; k>=0; k-- )
	{
		double	dValue = 0;
		double	nCount	=	0;
		for( int m=0; m<m_adwMTMDays.GetSize(); m++ )
		{
			if( int(k-m_adwMTMDays[m]) < 0 )
				return FALSE;

			double dMTM = 100;
			if( m_pKData->MaindataAt(k-m_adwMTMDays[m]) > 0 )
				dMTM	=	100. * m_pKData->MaindataAt(k) / m_pKData->MaindataAt(k-m_adwMTMDays[m]);

			dValue	+=	dMTM*(m+1);
			nCount	+=	(m+1);
		}

		if( nCount <= 0 )
			return FALSE;

		dValue	=	dValue / nCount;

		dMA	+=	dValue;
		nMACount	++;
		if( nMACount >= m_nMADays )
		{
			dMA	=	dMA/m_nMADays;
			if( pValue )
				*pValue	=	dMA;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	return FALSE;
}

/***
	计算DCYO及其移动平均值
*/
BOOL CDCYO::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CHSL
CHSL::CHSL( )
{
	SetDefaultParameters( );
}

CHSL::CHSL( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CHSL::~CHSL()
{
	Clear( );
}

double CHSL::GetMainChangeHand( DWORD dwMarket, CKData & kdata, int nIndexKD )
{
	if( nIndexKD < 0 || nIndexKD >= kdata.GetSize() )
		return 0.0;

	KDATA	kd = kdata.ElementAt(nIndexKD);

	// ignore dwMarket
	dwMarket	=	CStock::marketSHSE;

	CStock & stockMain = AfxGetStockMain( dwMarket );
	CStockInfo & infoMain = stockMain.GetStockInfo();
	CKData & kdataMain = stockMain.GetKData( kdata.GetKType() );
	int nIndexMain = kdataMain.GetIndexByDate( kd.m_date );
	if( nIndexMain < 0 || nIndexMain >= kdataMain.GetSize() )
		return 0.0;
	KDATA kdMain = kdataMain.ElementAt( nIndexMain );

	double	dPriceAvg = 0.0;
	if( kdMain.m_fVolume > 1 )
		dPriceAvg = kdMain.m_fAmount / kdMain.m_fVolume;

	double	dCapitalValue = 0.0;
	if( 0 == strcmp( STKLIB_CODE_MAIN, infoMain.GetStockCode() ) )
	{
		double	dFactor = 5.85 * 1e8;
		CSPTime	sptime;
		if( sptime.FromStockTime( kd.m_date, CKData::IsDayOrMin(kdata.GetKType()) ) )
		{
			CSPTime	tm0(2004,5,19,0,0,0);
			CSPTimeSpan	span = sptime - tm0;
			
			if( span.GetDays() < -3000 )
			{
				dFactor *= ( 1 - 0.08*3000.0/365.0);
				dFactor *= ( 1 - (-3000 - span.GetDays())/2000.0 );
			}
			else
			{
				dFactor	*= ( 1 + 0.08*span.GetDays()/365.0 );
			}
		}
		dCapitalValue	=	dFactor * (kdMain.m_fClose+kdMain.m_fOpen+kdMain.m_fHigh+kdMain.m_fLow)/4;
	}
	else if( 0 == strcmp( STKLIB_CODE_MAINSZN, infoMain.GetStockCode() ) )
	{
		// 深证成指
		dCapitalValue	=	0.41 * 1e8 * (kdMain.m_fClose+kdMain.m_fOpen+kdMain.m_fHigh+kdMain.m_fLow)/4;
	}

	double dChangeHand = 0.0;
	if( dCapitalValue > 1e-6 )
		dChangeHand	=	100. * kdMain.m_fAmount / dCapitalValue;

	return dChangeHand;
}

void CHSL::SetDefaultParameters( )
{
	m_nDays		=	30;
	m_nMADays	=	6;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CHSL::AttachParameters( CHSL & src )
{
	m_nDays			=	src.m_nDays;
	m_nMADays		=	src.m_nMADays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CHSL::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_DAYS(m_nMADays) && VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CHSL::Clear( )
{
	CTechnique::Clear( );
}

int CHSL::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	return ITS_NOTHING;
}

BOOL CHSL::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	相对换手率
*/
BOOL CHSL::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dRatioChangeHand = 0;
	double	dVolume = m_pKData->ElementAt(nIndex).m_fVolume;
	if( m_stockinfo.IsValidStock() && m_stockinfo.GetRatioChangeHand( &dRatioChangeHand, dVolume ) )
	{
		// 相对换手率
		double	dMainChangeHand = GetMainChangeHand( m_stockinfo.GetMarket(), *m_pKData, nIndex );
		double	dRelativeChangeHand = 1.0;
		if( dMainChangeHand > 1e-6 )
		{
			dRelativeChangeHand	=	dRatioChangeHand / dMainChangeHand;
			if( pValue )
				*pValue	=	dRelativeChangeHand;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	else
	{
		if( pValue )
			*pValue	=	1.0;
		StoreToCache( nIndex, pValue );
		return TRUE;
	}

	return FALSE;
}

/***
	计算HSL及其移动平均值
*/
BOOL CHSL::Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( nDays > nIndex+1 )
		return FALSE;

	double	dValue = 0, dMA = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		double	dTemp = 0;
		if( Calculate( &dTemp, k, FALSE ) )
		{
			if( nIndex == k )
				dValue	=	dTemp;
			dMA	+=	dTemp;

			nCount	++;
			if( nCount == nDays )
			{
				dMA	=	dMA / nDays;
				if( pValue )	*pValue	=	dMA;
				return TRUE;
			}
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CDPER
CDPER::CDPER( )
{
	SetDefaultParameters( );
}

CDPER::CDPER( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CDPER::~CDPER()
{
	Clear( );
}

void CDPER::SetDefaultParameters( )
{
	m_nDays			=	30;
	m_nMADays		=	6;
	m_nDetrendDays	=	12;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CDPER::AttachParameters( CDPER & src )
{
	m_nDays			=	src.m_nDays;
	m_nMADays		=	src.m_nMADays;
	m_nDetrendDays	=	src.m_nDetrendDays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CDPER::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_DAYS(m_nMADays) && VALID_DAYS(m_nDetrendDays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CDPER::Clear( )
{
	CTechnique::Clear( );
}

int CDPER::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays+m_nMADays+m_nDetrendDays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.15, 0.7 ) )
		return ITS_NOTHING;

	if( nIndex <= 1 )
		return ITS_NOTHING;

	double	dValue;
	if( !Calculate( &dValue, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dValue < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dValue > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return ITS_NOTHING;
}

BOOL CDPER::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	if( pdMin )	*pdMin	=	0;
	if( pdMax )	*pdMax	=	100;
	return TRUE;
}

/***
	A = 今日收盘价 - （m_nDetrendDays+m_nDetrendDays）日的平均收盘价
	       N日内A小于今日A的天数	
	DPER = ----------------------
	                N
	
*/
BOOL CDPER::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays+m_nDetrendDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dNow = 0;
	int	nCount = 0;
	double	dCountLow = 0;
	for( int k=nIndex; k>=0; k-- )
	{
		double	dCur = 0;

		double	dCt	= 0, dMA = 0;
		double	nDetrendCount	=	0;
		for( int l=k-1; l>=0; l-- )
		{
			dMA	+=	m_pKData->MaindataAt(l);

			nDetrendCount	++;
			if( nDetrendCount == m_nDetrendDays+m_nDetrendDays )
			{
				dCt	=	m_pKData->MaindataAt(k);
				dMA	=	dMA / (m_nDetrendDays+m_nDetrendDays);
				dCur	=	dCt - dMA;
				break;
			}
		}

		if( nIndex == k )
		{
			dNow	=	dCur;
			continue;
		}


		if( dCur < dNow )
			dCountLow	+=	1;

		nCount	++;
		if( nCount >= m_nDays )
		{
			if( pValue )
				*pValue	=	100. * dCountLow / m_nDays;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

/***
	计算CYO及其移动平均值
*/
BOOL CDPER::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

