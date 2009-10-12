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
//	CKDJ
CKDJ::CKDJ( )
{
	SetDefaultParameters( );
}

CKDJ::CKDJ( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CKDJ::~CKDJ()
{
	Clear( );
}

void CKDJ::SetDefaultParameters( )
{
	m_nRSVDays	=	9;
	m_nKDays	=	3;
	m_nDDays	=	3;
	m_nJ		=	1;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CKDJ::AttachParameters( CKDJ & src )
{
	m_nRSVDays	=	src.m_nRSVDays;
	m_nKDays	=	src.m_nKDays;
	m_nDDays	=	src.m_nDDays;
	m_nJ		=	src.m_nJ;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CKDJ::IsValidParameters( )
{
	return ( VALID_DAYS(m_nRSVDays) && VALID_DAYS(m_nKDays)
			&& VALID_DAYS(m_nDDays) && VALID_DAYS(m_nJ)
			&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CKDJ::Clear( )
{
	CTechnique::Clear( );
}

// 计算RSV值
BOOL CKDJ::CalculateRSV( double * pValue, int nIndex )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	double	dH = 0, dL = 0, dRSV = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		if( nIndex == k )
		{
			dH	=	m_pKData->ElementAt(k).m_fHigh;
			dL	=	m_pKData->ElementAt(k).m_fLow;
		}
		if( dH < m_pKData->ElementAt(k).m_fHigh )
			dH	=	m_pKData->ElementAt(k).m_fHigh;
		if( dL > m_pKData->ElementAt(k).m_fLow )
			dL	=	m_pKData->ElementAt(k).m_fLow;

		nCount	++;
		if( nCount == m_nRSVDays )
		{
			if( dH-dL < 1e-4 )
				dRSV	=	100;
			else
				dRSV	=	(m_pKData->ElementAt(nIndex).m_fClose - dL) * 100 / (dH - dL);
			if( pValue )	*pValue	=	dRSV;
			return TRUE;
		}
	}
	return FALSE;
}

int CKDJ::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( !PrepareCache( 0, -1, FALSE ) )
		return ITS_NOTHING;

	double	dK, dD, dJ;
	if( !Calculate( &dK, &dD, &dJ, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dK < 25 && dD < 25 && IsGoldenFork(nIndex, m_pdCache1, m_pdCache2) )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dK > 75 && dD > 75 && IsDeadFork(nIndex, m_pdCache1, m_pdCache2) )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}
	return ITS_NOTHING;
}

BOOL CKDJ::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo3( nStart, nEnd, pdMin, pdMax, this );
}

/***
	AX = 今天的收盘价 - N天中的最低价
	BX = N天中的最高价 - N天中的最低价
	RSV = （AX ÷ BX）× 100%
	NK = K计算天数，一般取3
	ND = D计算天数，一般取3
	K = 前一日K×(NK-1)/NK + RSV×1/NK 
	D = 前一日D×(ND-1)/ND + K×1/3
	J = 3D － 2K （或 J = 3K - 2D）
	第一次计算时，前一日的K、D值皆以50代替。
*/
BOOL CKDJ::Calculate( double *pValue1, double *pValue2, double *pValue3, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nRSVDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue1, pValue2, pValue3 ) )
		return TRUE;

	double	dRSV = 0;
	double	dK = 50, dD = 50, dJ = 50;
	if( bUseLast && pValue1 && pValue2 )
	{
		if( CalculateRSV( &dRSV, nIndex ) )
		{
			dK	=	(m_nKDays-1)*(*pValue1)/m_nKDays + dRSV/m_nKDays;
			dD	=	(m_nDDays-1)*(*pValue2)/m_nDDays + dK/m_nDDays;
			if( mode3K2D == m_nJ )
				dJ	=	3 * dK - 2 * dD;
			else
				dJ	=	3 * dD - 2 * dK;
			if( pValue1 )	*pValue1	=	dK;
			if( pValue2 )	*pValue2	=	dD;
			if( pValue3 )	*pValue3	=	dJ;
			StoreToCache( nIndex, pValue1, pValue2, pValue3 );
			return TRUE;
		}
	}
	else
	{
		double	factor1 = 1, factor2 = 1;
		int k;
		for( k=nIndex; k > 0; k-- )
		{
			factor1		*=	((double)(m_nKDays-1))/m_nKDays;
			factor2		*=	((double)(m_nDDays-1))/m_nDDays;
			if( factor1 < 0.001 && factor2 < 0.001 )
				break;
		}
		for( ; k<=nIndex; k++ )
		{
			if( CalculateRSV( &dRSV, k ) )
			{
				dK	=	(m_nKDays-1)*(dK)/m_nKDays + dRSV/m_nKDays;
				dD	=	(m_nDDays-1)*(dD)/m_nDDays + dK/m_nDDays;
				if( mode3K2D == m_nJ )
					dJ	=	3 * dK - 2 * dD;
				else
					dJ	=	3 * dD - 2 * dK;
				if( nIndex == k )
				{
					if( pValue1 )	*pValue1	=	dK;
					if( pValue2 )	*pValue2	=	dD;
					if( pValue3 )	*pValue3	=	dJ;
					StoreToCache( nIndex, pValue1, pValue2, pValue3 );
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CR
CR::CR( )
{
	SetDefaultParameters( );
}

CR::CR( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CR::~CR()
{
	Clear( );
}

void CR::SetDefaultParameters( )
{
	m_nDays		=	20;
}

void CR::AttachParameters( CR & src )
{
	m_nDays		=	src.m_nDays;
}

BOOL CR::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) );
}

void CR::Clear( )
{
	CTechnique::Clear( );
}

int CR::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
/*	double	dR;
	if( !Calculate( &dR, nIndex, FALSE ) )
		return ITS_NOTHING;
	if( dR > 80 )
	{
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dR < 20 )
	{
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}
*/	
	// 无买卖信号
	return	ITS_NOTHING;
}

BOOL CR::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	if( pdMin )	*pdMin	=	0;
	if( pdMax )	*pdMax	=	100;
	return TRUE;
}

/***
	            H - C
	W%R指标值= ———— ×100
	            H - L                
	H = N日内最高价		L = N日内最低价		C = 当天收盘价
*/
BOOL CR::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex+1 )
		return FALSE;
	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dH = 0, dL = 0, dR = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		if( nIndex == k )
		{
			dH	=	m_pKData->ElementAt(k).m_fHigh;
			dL	=	m_pKData->ElementAt(k).m_fLow;
		}
		if( dH < m_pKData->ElementAt(k).m_fHigh )
			dH	=	m_pKData->ElementAt(k).m_fHigh;
		if( dL > m_pKData->ElementAt(k).m_fLow )
			dL	=	m_pKData->ElementAt(k).m_fLow;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( dH-dL < 1e-4 )
				dR	=	100;
			else
				dR	=	(dH - m_pKData->ElementAt(nIndex).m_fClose) * 100 / (dH - dL);
			if( pValue )	*pValue	=	dR;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CRSI
CRSI::CRSI( )
{
	SetDefaultParameters( );
}

CRSI::CRSI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CRSI::~CRSI()
{
	Clear( );
}

void CRSI::SetDefaultParameters( )
{
	m_adwDays.RemoveAll();
	m_adwDays.Add( 10 );
	m_adwDays.Add( 20 );
	m_itsSold		=	ITS_BUY;
	m_itsGoldenFork	=	ITS_BUYINTENSE;
	m_itsDeadFork	=	ITS_SELLINTENSE;
}

void CRSI::AttachParameters( CRSI & src )
{
	m_adwDays.Copy( src.m_adwDays );
	m_itsSold		=	src.m_itsSold;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CRSI::IsValidParameters( )
{
	STT_VALID_DAYSARRAY( m_adwDays );
	return ( VALID_ITS(m_itsSold) && VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CRSI::Clear( )
{
	CTechnique::Clear( );
}

int CRSI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	if( nIndex <= 0 )
		return ITS_NOTHING;

	int	nForkSignal	=	GetForkSignal(nIndex, m_adwDays, m_itsGoldenFork, m_itsDeadFork, pnCode );
	for( int k=0; k<m_adwDays.GetSize(); k++ )
	{
		double	dRSI, dRSILast;
		if( !Calculate( &dRSILast, nIndex-1, m_adwDays[k], FALSE )
			|| !Calculate( &dRSI, nIndex, m_adwDays[k], FALSE ) )
			return ITS_NOTHING;
		if( dRSI < 20 )
		{	// 超卖
			if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
			return m_itsSold;
		}
		if( dRSI < 40 && nForkSignal == m_itsGoldenFork )
		{	// 低位金叉
			if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
			return m_itsGoldenFork;
		}
		if( dRSI > 60 && nForkSignal == m_itsDeadFork )
		{	// 高位死叉
			if( pnCode )	*pnCode	=	ITSC_DEADFORK;
			return m_itsDeadFork;
		}
	}

	return	ITS_NOTHING;
}

BOOL CRSI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	if( pdMin )	*pdMin	=	0;
	if( pdMax )	*pdMax	=	100;
	return TRUE;
}

/***
	RSI =（N日内上涨幅度累计÷N日内上涨及下跌幅度累计）×100%
*/
BOOL CRSI::Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE( m_pKData, nIndex, nDays );

	if( nDays > nIndex )
		return FALSE;

	double	dUC = 0, dDC = 0, dRSI = 0;
	int	nCount = 0;
	for( int k=nIndex; k>=1; k-- )
	{
		if( m_pKData->MaindataAt(k) > m_pKData->MaindataAt(k-1) )
			dUC	+=	( m_pKData->MaindataAt(k) - m_pKData->MaindataAt(k-1) );
		else
			dDC	+=	( m_pKData->MaindataAt(k-1) - m_pKData->MaindataAt(k) );

		nCount	++;
		if( nCount == nDays )
		{
			if( dUC+dDC < 1e-4 )
				dRSI	=	100;
			else
				dRSI	=	100*dUC/(dUC+dDC);
			if( pValue )	*pValue	=	dRSI;
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CBIAS
CBIAS::CBIAS( )
{
	SetDefaultParameters( );
}

CBIAS::CBIAS( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CBIAS::~CBIAS()
{
	Clear( );
}

void CBIAS::SetDefaultParameters( )
{
	m_nDays		=	10;
	m_itsSold	=	ITS_BUY;
	m_itsBought	=	ITS_SELL;
}

void CBIAS::AttachParameters( CBIAS & src )
{
	m_nDays		=	src.m_nDays;
	m_itsSold	=	src.m_itsSold;
	m_itsBought	=	src.m_itsBought;
}

BOOL CBIAS::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void CBIAS::Clear( )
{
	CTechnique::Clear( );
}

int CBIAS::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	double	dBIAS;
	if( !Calculate( &dBIAS, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dBIAS < -10 )
	{	// 超卖
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dBIAS > 5 )
	{	// 超买
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}

	return	ITS_NOTHING;
}

BOOL CBIAS::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	              当日收盘价-N日移动平均值
	N日乖离率 = —————————————— ×100%
	                   N日移动平均值
*/
BOOL CBIAS::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex+1 )
		return FALSE;
	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dMA = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k -- )
	{
		dMA	+=	m_pKData->MaindataAt(k);

		nCount	++;
		if( nCount == m_nDays )
		{
			double	dResult	=	0;
			dMA	=	dMA / m_nDays;
			if( dMA > 1e-4 )
				dResult	=	100. * ( m_pKData->MaindataAt(nIndex) - dMA ) / dMA;
			if( pValue )	*pValue	=	dResult;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CMTM
CMTM::CMTM( )
{
	SetDefaultParameters( );
}

CMTM::CMTM( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CMTM::~CMTM()
{
	Clear( );
}

void CMTM::SetDefaultParameters( )
{
	m_nDays		=	10;
	m_nMADays	=	10;

	m_itsGoldenFork			=	ITS_BUY;
	m_itsDeadFork			=	ITS_SELL;
}

void CMTM::AttachParameters( CMTM & src )
{
	m_nDays		=	m_nDays;
	m_nMADays	=	m_nMADays;

	m_itsGoldenFork			=	src.m_itsGoldenFork;
	m_itsDeadFork			=	src.m_itsDeadFork;
}

BOOL CMTM::IsValidParameters( )
{
	return ( VALID_DAYS( m_nDays ) && VALID_DAYS( m_nMADays )
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CMTM::Clear( )
{
	CTechnique::Clear( );
}

int CMTM::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays + m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.3, 0.6 ) )
		return ITS_NOTHING;

	double	dMTM, dMA;
	if( !Calculate( &dMTM, &dMA, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dMTM < dLiminalLow && dMA < dLiminalLow && IsGoldenFork( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dMTM > dLiminalHigh && dMA > dLiminalHigh && IsDeadFork( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

BOOL CMTM::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	MTM ＝ C/Cn
	其中：C为当日收市价，Cn为Ｎ日前收市价，
	N为设定参数，一般选设10日，亦可在6日至14日之间选择。
*/
BOOL CMTM::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	if( m_pKData->MaindataAt(nIndex-m_nDays) > 0 )
	{
		if( pValue )
			*pValue	=	100. * m_pKData->MaindataAt(nIndex) / m_pKData->MaindataAt(nIndex-m_nDays);
		StoreToCache( nIndex, pValue );
		return TRUE;
	}
	return FALSE;
}

BOOL CMTM::Calculate( double * pValue, double *pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CDMI
CDMI::CDMI( )
{
	SetDefaultParameters( );
	m_pdDMICache		=	NULL;
	m_pnDMICacheIndex	=	NULL;
	m_nDMICacheCurrent	=	-1;
}

CDMI::CDMI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
	m_pdDMICache		=	NULL;
	m_pnDMICacheIndex	=	NULL;
	m_nDMICacheCurrent	=	-1;
}

CDMI::~CDMI()
{
	Clear( );
}

void CDMI::SetDefaultParameters( )
{
	m_nDays			=	7;
}

void CDMI::AttachParameters( CDMI & src )
{
	m_nDays			=	src.m_nDays;
}

BOOL CDMI::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) );
}

void CDMI::Clear( )
{
	CTechnique::Clear( );

	if( m_pdDMICache )
	{
		delete	[]	m_pdDMICache;
		m_pdDMICache	=	NULL;
	}
	if( m_pnDMICacheIndex )
	{
		delete	[]	m_pnDMICacheIndex;
		m_pnDMICacheIndex	=	NULL;
	}
	m_nDMICacheCurrent	=	-1;
}

/***
	求出真正波幅TR，TR是下列三者中绝对值最大的一个：
		昨收盘-今 最高、昨收盘-今最低、今最高-今最低
	
	+DM = 今最高-昨最高
	-DM = 昨最低-今最低
*/
BOOL CDMI::CalculateDM( double *pDMPlus, double *pDMMinus, double *pTR, int nIndex )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( nIndex < 1 )
		return FALSE;

	double	dDMPlus = 0, dDMMinus = 0, dTR = 0;
	KDATA	kdNow	=	m_pKData->ElementAt(nIndex);
	KDATA	kdLast	=	m_pKData->ElementAt(nIndex-1);

	dDMPlus		=	max( 0, ((double)kdNow.m_fHigh) - kdLast.m_fHigh );
	dDMMinus	=	max( 0, ((double)kdLast.m_fLow) - kdNow.m_fLow );

	dTR	=	max( fabs(((double)kdNow.m_fClose)-kdNow.m_fLow), fabs(((double)kdNow.m_fHigh)-kdLast.m_fClose) );
	dTR	=	max( dTR, fabs(((double)kdNow.m_fLow)-kdLast.m_fClose) );

	if( pDMPlus )	*pDMPlus	=	dDMPlus;
	if( pDMMinus )	*pDMMinus	=	dDMMinus;
	if( pTR )		*pTR		=	dTR;
	return TRUE;
}

/***
	TRn = N天TR之和
	+DMn = N天+DM之和
	-DMn = N天-DM之和
	+DI = （+DMn ÷ TRn）×100
	-DI = （-DMn ÷ TRn）×100
	DI差 = +DI-（-DI）
	DI和 = +DI+（-DI）
	DX = （DI差÷DI和）×100
*/
BOOL CDMI::CalculateDIDX( double *pDIPlus, double *pDIMinus, double *pDX, int nIndex, int nDays )
{
	STT_ASSERT_CALCULATE( m_pKData, nIndex, nDays );
	
	if( m_nDays > nIndex )
		return FALSE;

	double	dDX, dADX = 0;
	double	dDIPlus = 0, dDIMinus = 0, dTRSum = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		double	dDMPlus = 0, dDMMinus = 0, dTR = 0;
		if( !CalculateDM( &dDMPlus, &dDMMinus, &dTR, k ) )
			return FALSE;

		dDIPlus		+=	dDMPlus;
		dDIMinus	+=	dDMMinus;
		dTRSum		+=	dTR;

		nCount	++;
		if( nCount == nDays )
		{
			if( fabs(dTRSum) < 1e-4 || fabs(dDIPlus+dDIMinus) < 1e-4 )
				return FALSE;
			dDIPlus		=	(dDIPlus * 100/dTRSum);
			dDIMinus	=	(dDIMinus * 100/dTRSum);
			dDX	=	fabs(dDIPlus-dDIMinus) * 100 / (dDIPlus+dDIMinus);
			if( pDIPlus )	*pDIPlus	=	dDIPlus;
			if( pDIMinus )	*pDIMinus	=	dDIMinus;
			if( pDX )	*pDX	=	dDX;
			return TRUE;
		}
	}
	return FALSE;
}

int CDMI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
/*	PrepareCache( 0, -1, FALSE );

	if( nIndex <= 0 )
		return ITS_NOTHING;

	double	dDIPlus, dDIMinus, dADX, dADXR;
	double	dDIPlusLast, dDIMinusLast, dADXLast, dADXRLast;
	if( !Calculate( &dDIPlusLast, &dDIMinusLast, &dADXLast, &dADXRLast, nIndex-1, FALSE )
		|| !Calculate( &dDIPlus, &dDIMinus, &dADX, &dADXR, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dDIPlusLast < dDIMinusLast && dDIPlus > dDIMinus )
	{
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dDIMinusLast < dDIPlusLast && dDIMinus > dDIPlus )
	{
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}
*/
	// 无买卖信号
	return	ITS_NOTHING;
}

BOOL CDMI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo4( nStart, nEnd, pdMin, pdMax, this );
}

/***
	求出真正波幅TR，TR是下列三者中绝对值最大的一个：
		昨收盘-今 最高、昨收盘-今最低、今最高-今最低
	
	+DM = 今最高-昨最高
	-DM = 昨最低-今最低

	TRn = N天TR之和
	+DMn = N天+DM之和
	-DMn = N天-DM之和
	+DI = （+DMn ÷ TRn）×100
	-DI = （-DMn ÷ TRn）×100
	DI差 = +DI-（-DI）
	DI和 = +DI+（-DI）
	DX = （DI差÷DI和）×100

	ADX = DX的平滑均值
	ADXR = ADX的均值
*/
BOOL CDMI::Calculate( double * pDIPlus, double * pDIMinus, double *pADX, double *pADXR, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pDIPlus, pDIMinus, pADX, pADXR ) )
		return TRUE;

	double	dDX = 0, dADX = 0, dADXR = 0;
	if( !CalculateDIDX( pDIPlus, pDIMinus, &dDX, nIndex, m_nDays ) )
		return FALSE;

	if( NULL == m_pdDMICache )
	{
		m_pdDMICache		=	new double[m_nDays];
		m_pnDMICacheIndex	=	new	int[m_nDays];
		if( NULL == m_pdDMICache || NULL == m_pnDMICacheIndex )
			return FALSE;
		for( int i=0; i<m_nDays; i++ )
			m_pnDMICacheIndex[i]	=	-1;
		m_nDMICacheCurrent	=	-1;
	}

	if( bUseLast && pADX )
	{
		// Calculate ADX and ADXR
		dADX	=	( (m_nDays-1)*(*pADX) + dDX ) / m_nDays;
		for( int i=0; i<m_nDays; i++ )
		{
			if( m_pnDMICacheIndex[i] == nIndex-m_nDays )
			{
				dADXR	=	(m_pdDMICache[i] + dADX) / 2;
				break;
			}
		}

		// Save ADX to cache
		m_nDMICacheCurrent	++;
		if( m_nDMICacheCurrent < 0 || m_nDMICacheCurrent >= m_nDays )
			m_nDMICacheCurrent	=	0;
		m_pdDMICache[m_nDMICacheCurrent]	=	dADX;
		m_pnDMICacheIndex[m_nDMICacheCurrent]	=	nIndex;

		if( pADX )	*pADX	=	dADX;
		if( pADXR )	*pADXR	=	dADXR;
		StoreToCache( nIndex, pDIPlus, pDIMinus, pADX, pADXR );
		return TRUE;
	}
	else
	{
		double	factor = 1;
		int k;
		for( k=nIndex; k > 0; k-- )
		{
			factor	*=	((double)(m_nDays-1))/m_nDays;
			if( factor < 0.001 )
				break;
		}
		double	dADXLast = 0;
		BOOL	bHasADXR = FALSE;
		for( ; k<=nIndex; k++ )
		{
			if( !CalculateDIDX( NULL, NULL, &dDX, k, m_nDays ) )
				continue;

			// Calculate ADX and ADXR
			dADX	=	( (m_nDays-1)*dADX + dDX )/m_nDays;
			if( k == nIndex-m_nDays )
			{
				dADXLast	=	dADX;
				bHasADXR	=	TRUE;
			}

			// Save ADX to cache
			m_nDMICacheCurrent	++;
			if( m_nDMICacheCurrent < 0 || m_nDMICacheCurrent >= m_nDays )
				m_nDMICacheCurrent	=	0;
			m_pdDMICache[m_nDMICacheCurrent]	=	dADX;
			m_pnDMICacheIndex[m_nDMICacheCurrent]	=	k;
		}
		dADXR	=	(dADX + dADXLast) / 2;
		if( pADX )				*pADX	=	dADX;
		if( pADXR && bHasADXR )	*pADXR	=	dADXR;
		StoreToCache( nIndex, pDIPlus, pDIMinus, pADX, pADXR );
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CROC
CROC::CROC( )
{
	SetDefaultParameters( );
}

CROC::CROC( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CROC::~CROC()
{
	Clear( );
}

void CROC::SetDefaultParameters( )
{
	m_nDays		=	10;
	m_nMADays	=	10;

	m_itsGoldenFork			=	ITS_BUY;
	m_itsDeadFork			=	ITS_SELL;
}

void CROC::AttachParameters( CROC & src )
{
	m_nDays		=	src.m_nDays;
	m_nMADays	=	src.m_nMADays;

	m_itsGoldenFork			=	src.m_itsGoldenFork;
	m_itsDeadFork			=	src.m_itsDeadFork;
}

BOOL CROC::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CROC::Clear( )
{
	CTechnique::Clear( );
}

int CROC::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays + m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.309, 0.6 ) )
		return ITS_NOTHING;

	double	dROC, dMA;
	if( !Calculate( &dROC, &dMA, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dROC < dLiminalLow && dMA < dLiminalLow && IsGoldenFork( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dROC > dLiminalHigh && dMA > dLiminalHigh && IsDeadFork( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

BOOL CROC::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	ROC=（今收盘-前N日收盘）÷前N日的收盘×100 
*/
BOOL CROC::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	if( m_pKData->MaindataAt(nIndex-m_nDays) <= 0
		|| m_pKData->MaindataAt(nIndex) <= 0 )
		return FALSE;

	double	x	=	m_pKData->MaindataAt(nIndex);
	double	y	=	m_pKData->MaindataAt(nIndex-m_nDays);
	if( pValue )
		*pValue	=	(x - y) * 100 / y;
	StoreToCache( nIndex, pValue );
	return TRUE;
}

BOOL CROC::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CCCI
CCCI::CCCI( )
{
	SetDefaultParameters( );
}

CCCI::CCCI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CCCI::~CCCI()
{
	Clear( );
}

void CCCI::SetDefaultParameters( )
{
	m_nDays		=	14;
	m_dQuotiety	=	0.015;
	m_nMADays	=	10;

	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CCCI::AttachParameters( CCCI & src )
{
	m_nDays		=	src.m_nDays;
	m_dQuotiety	=	src.m_dQuotiety;
	m_nMADays	=	src.m_nMADays;

	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CCCI::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && m_dQuotiety > 0 && VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CCCI::Clear( )
{
	CTechnique::Clear( );
}

int CCCI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	double	dCCI;
	if( !Calculate( &dCCI, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nForkSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );

	if( dCCI < -100 && nForkSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dCCI > 100 && nForkSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

BOOL CCCI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	TP = 收盘价+收盘价+最高价+最低价
	A是TP的N日均值
	D是TP与A的离差均值
	CCI=(C-D)/(0.015D)
*/
BOOL CCCI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dTP = 0, dMATP = 0, dD = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		KDATA	kd	=	m_pKData->ElementAt(k);
		double	dTemp	=	(kd.m_fHigh+kd.m_fClose+kd.m_fClose+kd.m_fLow)/4;
		if( nIndex == k )
			dTP	=	dTemp;
		dMATP	+=	dTemp;

		nCount	++;
		if( nCount == m_nDays )
			break;
	}
	dMATP	=	dMATP / m_nDays;

	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		KDATA	kd	=	m_pKData->ElementAt(k);
		double	dTemp	=	(kd.m_fHigh+kd.m_fClose+kd.m_fClose+kd.m_fLow)/4;
		dD		+=	fabs(dTemp-dMATP);

		nCount	++;
		if( nCount == m_nDays )
			break;
	}

	dD	=	dD / m_nDays;
	if( fabs(dD) < 1e-4 )
		return FALSE;
	if( pValue )
		*pValue	=	(dTP - dMATP) / (m_dQuotiety * dD);
	StoreToCache( nIndex, pValue );
	return TRUE;
}

/***
	CCI 及其 移动平均值
*/
BOOL CCCI::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CCV
CCV::CCV( )
{
	SetDefaultParameters( );
}

CCV::CCV( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CCV::~CCV()
{
	Clear( );
}

void CCV::SetDefaultParameters( )
{
	m_nMAHLDays	=	10;
	m_nCVDays	=	10;
	m_itsSold				=	ITS_BUY;
	m_itsBought				=	ITS_SELL;
}

void CCV::AttachParameters( CCV & src )
{
	m_nMAHLDays	=	src.m_nMAHLDays;
	m_nCVDays	=	src.m_nCVDays;
	m_itsSold				=	src.m_itsSold;
	m_itsBought				=	src.m_itsBought;
}

BOOL CCV::IsValidParameters( )
{
	return ( VALID_DAYS(m_nMAHLDays) && VALID_DAYS(m_nCVDays)
		&& VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void CCV::Clear( )
{
	CTechnique::Clear( );
}

int CCV::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	if( nIndex <= 1 )
		return ITS_NOTHING;

	double	dNow, dLast, dLastLast;
	if( !Calculate( &dLastLast, nIndex-2, FALSE )
		|| !Calculate( &dLast, nIndex-1, FALSE )
		|| !Calculate( &dNow, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dLastLast < 0 && dLast < 0 && dNow < 0 && dLast <= dLastLast && dNow > dLast )
	{	// 超卖
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dLastLast > 0 && dLast > 0 && dNow > 0 && dLast >= dLastLast && dNow < dLast )
	{	// 超买
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}

	return	ITS_NOTHING;
}

BOOL CCV::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	A = 最高价 - 最低价
	MAHL	= A的m_nMAHLDays日平均值
	MAHLLast= m_nCVDays日前的MAHL
	CCI		= 100 * (MAHL - MAHLLast) / MAHLLast;
*/
BOOL CCV::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nMAHLDays+m_nCVDays > nIndex+2 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dMAHLNow = 0, dMAHLLast = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		KDATA	kd	=	m_pKData->ElementAt(k);
		dMAHLNow	+=	(kd.m_fHigh-kd.m_fLow);
		
		nCount	++;
		if( nCount == m_nMAHLDays )
			break;
	}

	nCount	=	0;
	for( int k=nIndex-m_nCVDays+1; k>=0; k-- )
	{
		KDATA	kd	=	m_pKData->ElementAt(k);
		dMAHLLast	+=	(((double)kd.m_fHigh)-kd.m_fLow);
		
		nCount	++;
		if( nCount == m_nMAHLDays )
			break;
	}
	
	dMAHLNow	=	dMAHLNow / m_nMAHLDays;
	dMAHLLast	=	dMAHLLast / m_nMAHLDays;
	if( fabs(dMAHLLast) < 1e-4 )
		return FALSE;
	if( pValue )
		*pValue	=	(dMAHLNow - dMAHLLast) * 100 / dMAHLLast;
	StoreToCache( nIndex, pValue );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CARBR
CARBR::CARBR( )
{
	SetDefaultParameters( );
}

CARBR::CARBR( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CARBR::~CARBR()
{
	Clear( );
}

void CARBR::SetDefaultParameters( )
{
	m_nDays	=	26;
	m_itsGoldenFork		=	ITS_BUYINTENSE;
	m_itsDeadFork		=	ITS_SELLINTENSE;
	m_itsSold			=	ITS_BUY;
	m_itsBought			=	ITS_SELL;
}

void CARBR::AttachParameters( CARBR & src )
{
	m_nDays	=	src.m_nDays;
	m_itsGoldenFork		=	src.m_itsGoldenFork;
	m_itsDeadFork		=	src.m_itsDeadFork;
	m_itsSold			=	src.m_itsSold;
	m_itsBought			=	src.m_itsBought;
}

BOOL CARBR::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork)
		&& VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void CARBR::Clear( )
{
	CTechnique::Clear( );
}

int CARBR::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	double	dAR, dBR;
	if( !Calculate( &dAR, &dBR, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nForkSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	
	if( (dAR < 50 || dBR < 50 ) && nForkSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( (dAR > 180 || dBR > 300) && nForkSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}
	if( dAR < 50 )
	{	// 超卖
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dAR > 200 )
	{	// 超买
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}

	return	ITS_NOTHING;
}

BOOL CARBR::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	      （H-O）n天之和
	AR = ———————— × 100
	      （O-L）n天之和
	      （H-C）n天之和
	BR = ———————— × 100
	      （C-L）n天之和

	H：最高价	L：最低价	O：开盘价	C：收盘价 
*/
BOOL CARBR::Calculate( double * pAR, double *pBR, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pAR, pBR ) )
		return TRUE;

	double	dUP = 0, dDG = 0, dBS = 0, dSS = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);
		KDATA	kdLast	=	m_pKData->ElementAt(k-1);
		dUP	+=	(((double)kd.m_fHigh)-kd.m_fOpen);
		dDG	+=	(((double)kd.m_fOpen)-kd.m_fLow);
		dBS	+=	max( 0, ((double)kd.m_fHigh) - kdLast.m_fClose );
		dSS	+=	max( 0, ((double)kdLast.m_fClose) - kd.m_fLow );

		nCount	++;
		if( nCount == m_nDays )
			break;
	}

	if( dDG < 1e-4 || dSS < 1e-4 )
		return FALSE;
	if( pAR )
		*pAR	=	dUP * 100 / dDG;
	if( pBR )
		*pBR	=	dBS * 100 / dSS;
	StoreToCache( nIndex, pAR, pBR );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CCR
CCR::CCR( )
{
	SetDefaultParameters( );
}

CCR::CCR( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CCR::~CCR()
{
	Clear( );
}

void CCR::SetDefaultParameters( )
{
	m_nDays		=	26;
	m_nMADaysA	=	10;
	m_nMADaysB	=	20;
	m_nMADaysC	=	40;
	m_nMADaysD	=	62;

	m_itsSold				=	ITS_BUY;
	m_itsBought				=	ITS_SELL;
}

void CCR::AttachParameters( CCR & src )
{
	m_nDays	=	src.m_nDays;
	m_nMADaysA	=	src.m_nMADaysA;
	m_nMADaysB	=	src.m_nMADaysB;
	m_nMADaysC	=	src.m_nMADaysC;
	m_nMADaysD	=	src.m_nMADaysD;

	m_itsSold				=	src.m_itsSold;
	m_itsBought				=	src.m_itsBought;
}

BOOL CCR::IsValidParameters( )
{
	return ( VALID_DAYS( m_nDays ) && VALID_DAYS( m_nMADaysA ) && VALID_DAYS( m_nMADaysB )
		&& VALID_DAYS( m_nMADaysC ) && VALID_DAYS( m_nMADaysD )
		&& VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void CCR::Clear( )
{
	CTechnique::Clear( );
}

int CCR::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	double	dCR;
	if( !Calculate( &dCR, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dCR < 40 )
	{	// 超卖
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dCR > 300 )
	{	// 超买
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}
	return	ITS_NOTHING;
}

BOOL CCR::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo5( nStart, nEnd, pdMin, pdMax, this );
}

/***
	MID = （开盘价 + 收盘价 + 最高价 + 最低价）÷ 2
	上升值 = 今最高 - 昨日MID （负值记为0）
	下跌值 = 昨MID - 今最低
	       n天上升值之和
	CR = ———————— × 100
	       n天下跌值之和
*/
BOOL CCR::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dBS = 0, dSS = 0, dCR = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);
		KDATA	kdLast	=	m_pKData->ElementAt(k-1);
		double	dTP	=	(kdLast.m_fHigh+kdLast.m_fLow+kdLast.m_fOpen+kdLast.m_fClose)/4;
		dBS	+=	max( 0, kd.m_fHigh - dTP );
		dSS	+=	max( 0, dTP - kd.m_fLow );

		nCount	++;
		if( nCount == m_nDays )
			break;
	}

	if( dSS < 1e-4 )
		return FALSE;
	dCR	=	dBS * 100 / dSS;
	if( pValue )
		*pValue	=	dCR;
	StoreToCache( nIndex, pValue );
	return TRUE;
}

/***
	A, B, C, D 分别是CR的N日平均值
	N分别为m_nMADaysA,m_nMADaysB,m_nMADaysC,m_nMADaysD
*/
BOOL CCR::Calculate( double * pValue, double * pA, double * pB, double * pC, double * pD, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	int	nMaxMADays	=	max( max(m_nMADaysA,m_nMADaysB), max(m_nMADaysC,m_nMADaysD) );
	if( m_nDays+nMaxMADays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue, pA, pB, pC, pD ) )
		return TRUE;

	double	dCR = 0, dA = 0, dB = 0, dC = 0, dD = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=m_nDays; k-- )
	{
		double	dTemp;
		if( Calculate( &dTemp, k, FALSE ) )
		{
			if( nIndex == k )
				dCR	=	dTemp;
			nCount	++;
			if( nCount <= m_nMADaysA )
				dA	+=	dTemp;
			if( nCount <= m_nMADaysB )
				dB	+=	dTemp;
			if( nCount <= m_nMADaysC )
				dC	+=	dTemp;
			if( nCount <= m_nMADaysD )
				dD	+=	dTemp;

			if( nCount > nMaxMADays )
			{
				if( pValue )	*pValue	=	dCR;
				if( pA )	*pA	=	dA / m_nMADaysA;
				if( pB )	*pB	=	dB / m_nMADaysB;
				if( pC )	*pC	=	dC / m_nMADaysC;
				if( pD )	*pD	=	dD / m_nMADaysD;
				StoreToCache( nIndex, pValue, pA, pB, pC, pD );
				return TRUE;
			}
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	COSC
COSC::COSC( )
{
	SetDefaultParameters( );
}

COSC::COSC( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

COSC::~COSC()
{
	Clear( );
}

void COSC::SetDefaultParameters( )
{
	m_nDays		=	10;
	m_nMADays	=	6;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void COSC::AttachParameters( COSC & src )
{
	m_nDays		=	src.m_nDays;
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL COSC::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void COSC::Clear( )
{
	CTechnique::Clear( );
}

int COSC::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays+m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.3, 0.6 ) )
		return ITS_NOTHING;

	double	dOSC;
	if( !Calculate( &dOSC, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dOSC < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dOSC > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

BOOL COSC::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	OSC = 今日收盘价/N日收盘价平均值
*/
BOOL COSC::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dCt = m_pKData->MaindataAt(nIndex);
	double	dMA = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		dMA	+=	m_pKData->MaindataAt(k);

		nCount	++;
		if( nCount == m_nDays )
		{
			dMA	=	dMA / m_nDays;
			if( dMA < 1e-4 )
				return FALSE;
			if( pValue )
				*pValue	=	(dCt/dMA);
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	return FALSE;
}

/***
	计算OSC及其移动平均值
*/
BOOL COSC::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CUOS
CUOS::CUOS( )
{
	SetDefaultParameters( );
}

CUOS::CUOS( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CUOS::~CUOS()
{
	Clear( );
}

void CUOS::SetDefaultParameters( )
{
	m_nDays1	=	7;
	m_nDays2	=	14;
	m_nDays3	=	28;
	m_nMADays	=	6;
	m_itsGoldenFork			=	ITS_BUY;
	m_itsDeadFork			=	ITS_SELL;
}

void CUOS::AttachParameters( CUOS & src )
{
	m_nDays1	=	src.m_nDays1;
	m_nDays2	=	src.m_nDays2;
	m_nDays3	=	src.m_nDays3;
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork			=	src.m_itsGoldenFork;
	m_itsDeadFork			=	src.m_itsDeadFork;
}

BOOL CUOS::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays1) && VALID_DAYS(m_nDays2) && VALID_DAYS(m_nDays3) && VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CUOS::Clear( )
{
	CTechnique::Clear( );
}

int CUOS::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	max( max(m_nDays1,m_nDays2),m_nDays3) +m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.4, 0.6 ) )
		return ITS_NOTHING;
	
	double	dUOS;
	if( !Calculate( &dUOS, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dUOS < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dUOS > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

BOOL CUOS::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	OSC1 = m_nDays1日OSC
	OSC2 = m_nDays2日OSC
	OSC3 = m_nDays3日OSC
	UOS = (OSC1+OSC2+OSC3)/3
*/
BOOL CUOS::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dOSC1 = 0, dOSC2 = 0, dOSC3 = 0;
	COSC	osc1(m_pKData), osc2(m_pKData), osc3(m_pKData);
	osc1.m_nDays	=	m_nDays1;
	osc2.m_nDays	=	m_nDays2;
	osc3.m_nDays	=	m_nDays3;
	if( osc1.Calculate( &dOSC1, nIndex, FALSE )
		&& osc2.Calculate( &dOSC2, nIndex, FALSE )
		&& osc3.Calculate( &dOSC3, nIndex, FALSE ) )
	{
		if( pValue )
			*pValue	=	(dOSC1+dOSC2+dOSC3)/3;
		StoreToCache( nIndex, pValue );
		return TRUE;
	}
	return FALSE;
}

/***
	计算UOS及其移动平均值
*/
BOOL CUOS::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CMAOSC
CMAOSC::CMAOSC( )
{
	SetDefaultParameters( );
}

CMAOSC::CMAOSC( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CMAOSC::~CMAOSC()
{
	Clear( );
}

void CMAOSC::SetDefaultParameters( )
{
	m_nDays1	=	6;
	m_nDays2	=	12;
	m_nMADays	=	6;
	m_itsGoldenFork			=	ITS_BUY;
	m_itsDeadFork			=	ITS_SELL;
}

void CMAOSC::AttachParameters( CMAOSC & src )
{
	m_nDays1	=	src.m_nDays1;
	m_nDays2	=	src.m_nDays2;
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork			=	src.m_itsGoldenFork;
	m_itsDeadFork			=	src.m_itsDeadFork;
}

BOOL CMAOSC::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays1) && VALID_DAYS(m_nDays2) && VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CMAOSC::Clear( )
{
	CTechnique::Clear( );
}

int CMAOSC::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nMADays + max(m_nDays1,m_nDays2);
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.4, 0.5 ) )
		return ITS_NOTHING;

	double	dMAOSC;
	if( !Calculate( &dMAOSC, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dMAOSC < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dMAOSC > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

BOOL CMAOSC::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	MAOSC = m_nDays1日收盘价移动平均值 - m_nDays2日收盘价移动平均值
*/
BOOL CMAOSC::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dMA1 = 0, dMA2 = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		if( nCount < m_nDays1 )
			dMA1	+=	m_pKData->MaindataAt(k);
		if( nCount < m_nDays2 )
			dMA2	+=	m_pKData->MaindataAt(k);

		nCount	++;
		if( nCount >= m_nDays1 && nCount >= m_nDays2 )
		{
			if( pValue )
				*pValue	=	(dMA1/m_nDays1 - dMA2/m_nDays2);
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

/***
	计算MAOSC及其移动平均值
*/
BOOL CMAOSC::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	C36BIAS
C36BIAS::C36BIAS( )
{
	SetDefaultParameters( );
}

C36BIAS::C36BIAS( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

C36BIAS::~C36BIAS()
{
	Clear( );
}

void C36BIAS::SetDefaultParameters( )
{
	m_itsSold	=	ITS_BUY;
	m_itsBought	=	ITS_SELL;
}

void C36BIAS::AttachParameters( C36BIAS & src )
{
	m_itsSold	=	src.m_itsSold;
	m_itsBought	=	src.m_itsBought;
}

BOOL C36BIAS::IsValidParameters( )
{
	return ( VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void C36BIAS::Clear( )
{
	CTechnique::Clear( );
}

int C36BIAS::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	6;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, 40, &dLiminalLow, &dLiminalHigh, 0.02, 0.98 ) )
		return ITS_NOTHING;

	double	d36BIAS;
	if( !Calculate( &d36BIAS, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( d36BIAS < dLiminalLow )
	{	// 超卖
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( d36BIAS > dLiminalHigh )
	{	// 超买
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}
	
	return	ITS_NOTHING;
}

BOOL C36BIAS::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	3-6BIAS = 3日收盘价移动平均值 - 6日收盘价移动平均值
*/
BOOL C36BIAS::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dMA1 = 0, dMA2 = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		if( nCount < 3 )
			dMA1	+=	m_pKData->MaindataAt(k);
		if( nCount < 6 )
			dMA2	+=	m_pKData->MaindataAt(k);

		nCount	++;
		if( nCount >= 3 && nCount >= 6 )
		{
			if( pValue )
				*pValue	=	(dMA1/3 - dMA2/6);
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CDPO
CDPO::CDPO( )
{
	SetDefaultParameters( );
}

CDPO::CDPO( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CDPO::~CDPO()
{
	Clear( );
}

void CDPO::SetDefaultParameters( )
{
	m_nDays		=	10;
	m_nMADays	=	6;
	m_itsGoldenFork		=	ITS_BUY;
	m_itsDeadFork		=	ITS_SELL;
}

void CDPO::AttachParameters( CDPO & src )
{
	m_nDays		=	src.m_nDays;
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork		=	src.m_itsGoldenFork;
	m_itsDeadFork		=	src.m_itsDeadFork;
}

BOOL CDPO::IsValidParameters( )
{
	return ( VALID_DAYS( m_nDays ) && VALID_DAYS( m_nMADays )
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CDPO::Clear( )
{
	CTechnique::Clear( );
}

int CDPO::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	3*m_nDays + m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.2, 0.7 ) )
		return ITS_NOTHING;

	double	dDPO;
	if( !Calculate( &dDPO, nIndex, FALSE ) )
		return FALSE;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dDPO < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dDPO > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

BOOL CDPO::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	DPO = 今日收盘价 - N日前的（N+N）日平均收盘价
*/
BOOL CDPO::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( 3*m_nDays-2 > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dCt	= 0, dMA = 0;
	double	nCount	=	0;
	for( int k=nIndex-m_nDays+1; k>=0; k-- )
	{
		dMA	+=	m_pKData->MaindataAt(k);

		nCount	++;
		if( nCount == m_nDays+m_nDays )
		{
			dCt	=	m_pKData->MaindataAt(nIndex);
			dMA	=	dMA / (m_nDays+m_nDays);
			if( pValue )
				*pValue	=	(dCt-dMA);
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

/***
	计算DPO及其移动平均值
*/
BOOL CDPO::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CKST
CKST::CKST( )
{
	SetDefaultParameters( );
}

CKST::CKST( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CKST::~CKST()
{
	Clear( );
}

void CKST::SetDefaultParameters( )
{
	m_adwROCDays.RemoveAll();
	m_adwROCDays.Add( 9 );
	m_adwROCDays.Add( 12 );
	m_adwROCDays.Add( 18 );
	m_adwROCDays.Add( 24 );
	m_nMADays	=	6;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CKST::AttachParameters( CKST & src )
{
	m_adwROCDays.Copy( src.m_adwROCDays );
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CKST::IsValidParameters( )
{
	STT_VALID_DAYSARRAY( m_adwROCDays );
	return ( VALID_DAYS(m_nMADays) && VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CKST::Clear( )
{
	CTechnique::Clear( );
}

int CKST::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	AfxGetMaxDays( m_adwROCDays )+m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.2, 0.6 ) )
		return ITS_NOTHING;

	if( nIndex <= 1 )
		return ITS_NOTHING;

	double	dKST;
	if( !Calculate( &dKST, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dKST < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dKST > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return ITS_NOTHING;
}

BOOL CKST::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	       9日ROC + 12日ROC×2 + 18日ROC×3 + 24日ROC×4
	KST = ------------------------------------------------
	                           10
*/
BOOL CKST::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	UINT	nMaxDays	=	AfxGetMaxDays( m_adwROCDays );
	if( (int)nMaxDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dKST = 0;
	int	nCount	=	0;
	for( int m=0; m<m_adwROCDays.GetSize(); m++ )
	{
		CROC	roc(m_pKData);
		roc.m_nDays		=	m_adwROCDays[m];
		double	dROC = 0;
		if( !roc.Calculate( &dROC, nIndex, FALSE ) )
			return FALSE;
		dKST	+=	dROC*(m+1);
		nCount	+=	(m+1);
	}

	if( nCount <= 0 )
		return FALSE;

	dKST	=	dKST / nCount;
	if( pValue )
		*pValue	=	dKST;
	StoreToCache( nIndex, pValue );
	return TRUE;
}

/***
	计算KST及其移动平均值
*/
BOOL CKST::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CTechnique::CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

