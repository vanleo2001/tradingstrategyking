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
//	CMACD
CMACD::CMACD( )
{
	SetDefaultParameters( );
}

CMACD::CMACD( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CMACD::~CMACD()
{
	Clear( );
}

void CMACD::SetDefaultParameters( )
{
	m_nEMA1Days	=	12;
	m_nEMA2Days	=	26;
	m_nDIFDays	=	9;

	m_itsDeviateOnBottom	=	ITS_BUYINTENSE;
	m_itsDeviateOnTop		=	ITS_SELLINTENSE;
	m_itsGoldenFork			=	ITS_BUY;
	m_itsDeadFork			=	ITS_SELL;
}

void CMACD::AttachParameters( CMACD & src )
{
	m_nEMA1Days	=	src.m_nEMA1Days;
	m_nEMA2Days	=	src.m_nEMA2Days;
	m_nDIFDays	=	src.m_nDIFDays;

	m_itsDeviateOnBottom	=	src.m_itsDeviateOnBottom;
	m_itsDeviateOnTop		=	src.m_itsDeviateOnTop;
	m_itsGoldenFork			=	src.m_itsGoldenFork;
	m_itsDeadFork			=	src.m_itsDeadFork;
}

BOOL CMACD::IsValidParameters( )
{
	return ( VALID_DAYS(m_nEMA1Days) && VALID_DAYS(m_nEMA2Days) && VALID_DAYS(m_nDIFDays)
		&& VALID_ITS(m_itsDeviateOnBottom) && VALID_ITS(m_itsDeviateOnTop)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CMACD::Clear( )
{
	CTechnique::Clear( );
}

int CMACD::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	PrepareCache( 0, -1, FALSE );

	int	nMaxDays	=	max( max(m_nEMA1Days,m_nEMA2Days) , m_nDIFDays );
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.309, 0.682 ) )
		return ITS_NOTHING;

	double	dEMA1, dEMA2, dDIF, dDEA;
	if( !Calculate( &dEMA1, &dEMA2, &dDIF, &dDEA, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( IsDeviateOnBottom( nIndex, m_pdCache3, m_pdCache4 ) )
	{	// 底背离
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONBOTTOM;
		return m_itsDeviateOnBottom;
	}
	if( IsDeviateOnTop( nIndex, m_pdCache3, m_pdCache4 ) )
	{	// 顶背离
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONTOP;
		return m_itsDeviateOnTop;
	}
	if( dDIF < dLiminalLow && dDEA < dLiminalLow && IsGoldenFork( nIndex, m_pdCache3, m_pdCache4 ) )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dDIF > dLiminalHigh && dDEA > dLiminalHigh && IsDeadFork( nIndex, m_pdCache3, m_pdCache4 ) )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}
	if( dDIF < dLiminalLow && dDEA < dLiminalLow )
	{	// 低位
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dDIF > dLiminalHigh && dDEA > dLiminalHigh )
	{	// 高位
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}

	return	ITS_NOTHING;
}

/***
	EMA  = 短期移动均值
	EMA2 = 长期移动均值
	DIF  = 短期移动均值 - 长期移动均值
	DEA  = DIF的移动平滑值
	柱状线值 = DIF - DEA
*/
BOOL CMACD::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	STT_ASSERT_GETMINMAXINFO( m_pKData, nStart, nEnd );

	double	dMin = 0, dMax = 0;
	double	dEMA1 = 0, dEMA2 = 0, dDIF = 0, dDEA = 0;
	BOOL	bFirst	=	TRUE;
	for( int k=nStart; k<=nEnd; k++ )
	{
		if( Calculate( &dEMA1, &dEMA2, &dDIF, &dDEA, k, !bFirst ) )
		{
			if( bFirst )	{	dMin	=	dDIF;	dMax	=	dDIF;	}
			
			if( dDIF < dMin )	dMin	=	dDIF;
			if( dDEA < dMin )	dMin	=	dDEA;
			if( 2*(dDIF-dDEA) < dMin )	dMin	=	2*(dDIF-dDEA); // WARNING: different

			if( dDIF > dMax )	dMax	=	dDIF;
			if( dDEA > dMax )	dMax	=	dDEA;
			if( 2*(dDIF-dDEA) > dMax )	dMax	=	2*(dDIF-dDEA); // WARNING: different
			bFirst	=	FALSE;
		}
	}
	dMin	-=	fabs(dMin)*0.02;
	dMax	+=	fabs(dMax)*0.02;
	if( fabs(dMin) < 1e-4 )
		dMin	=	-0.01;
	if( fabs(dMax) < 1e-4 )
		dMax	=	0.01;
	if( dMax - dMin < 0.03 )
		dMax	=	dMin + 0.05;

	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;
	return TRUE;
}

BOOL CMACD::Calculate( double *pdEMA1, double *pdEMA2, double *pdDIF, double *pdDEA,
					int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nEMA1Days > nIndex+1 || m_nEMA2Days > nIndex+1 || m_nDIFDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pdEMA1, pdEMA2, pdDIF, pdDEA ) )
		return TRUE;

	// Calculate EMA1, EMA2, DIF, DEA
	double	dEMA1New = 0, dEMA2New = 0, dDIFNew = 0, dDEANew = 0;
	if( bUseLast && pdEMA1 && pdEMA2 && pdDEA )
	{
		dEMA1New	=	(*pdEMA1)*(m_nEMA1Days-1)/(m_nEMA1Days+1) + 2 * m_pKData->MaindataAt(nIndex) /(m_nEMA1Days+1);
		dEMA2New	=	(*pdEMA2)*(m_nEMA2Days-1)/(m_nEMA2Days+1) + 2 * m_pKData->MaindataAt(nIndex) /(m_nEMA2Days+1);
		dDIFNew		=	dEMA1New-dEMA2New;
		dDEANew		=	(*pdDEA)*(m_nDIFDays-1)/(m_nDIFDays+1) + 2 * dDIFNew/(m_nDIFDays+1);
	}
	else
	{
		double	factor1 = 1, factor2 = 1;
		int k;
		for( k=nIndex; k > 0; k-- )
		{
			factor1		*=	((double)(m_nEMA1Days-1))/(m_nEMA1Days+1);
			factor2		*=	((double)(m_nEMA2Days-1))/(m_nEMA2Days+1);
			if( factor1 < 0.001 && factor2 < 0.001 )
				break;
		}
		dEMA1New	=	m_pKData->MaindataAt(k);
		dEMA2New	=	m_pKData->MaindataAt(k);
		dDIFNew		=	dEMA1New - dEMA2New;
		dDEANew		=	dDIFNew;
		for( ; k<=nIndex; k++ )
		{
			dEMA1New	=	dEMA1New * (m_nEMA1Days-1)/(m_nEMA1Days+1) + 2 * m_pKData->MaindataAt(k) /(m_nEMA1Days+1);
			dEMA2New	=	dEMA2New * (m_nEMA2Days-1)/(m_nEMA2Days+1) + 2 * m_pKData->MaindataAt(k) /(m_nEMA2Days+1);
			dDIFNew		=	dEMA1New - dEMA2New;
			dDEANew		=	dDEANew * (m_nDIFDays-1)/(m_nDIFDays+1) + 2 * dDIFNew / (m_nDIFDays+1);
		}
	}

	if( pdEMA1 )		*pdEMA1	=	dEMA1New;
	if( pdEMA2 )		*pdEMA2	=	dEMA2New;
	if( pdDIF )			*pdDIF	=	dDIFNew;
	if( pdDEA )			*pdDEA	=	dDEANew;
	StoreToCache( nIndex, pdEMA1, pdEMA2, pdDIF, pdDEA );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CMIKE
CMIKE::CMIKE( )
{
	SetDefaultParameters( );
}

CMIKE::CMIKE( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CMIKE::~CMIKE()
{
	Clear( );
}

void CMIKE::SetDefaultParameters( )
{
	m_nDays	=	12;
}

void CMIKE::AttachParameters( CMIKE & src )
{
	m_nDays	=	src.m_nDays;
}

BOOL CMIKE::IsValidParameters( )
{
	return VALID_DAYS( m_nDays );
}

void CMIKE::Clear( )
{
	CTechnique::Clear( );
}

/***
	H:最高价	L:最低价	C:收盘价
	TP ＝ (H＋L＋C)÷3
	第一条窄通道的上下限计算如下：
		弱阻力WR＝TP＋(TP－L)		弱支撑WS＝TP－(H－TP)
	第二条通道的上下限计算如下：
		中阻力MR＝TP＋(H－L)		中支撑MS＝TP－(H－L)
	第三条阔通道的上下限计算如下：
		强阻力SR＝H＋(H－L)			强支撑SS＝L－(H－L)
*/
BOOL CMIKE::CalculateMIKE(	double *pWR, double *pMR, double *pSR,
							double *pWS, double *pMS, double *pSS, int nIndex )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays * 2 > nIndex + 1 )
		return FALSE;

	double	TP	=	(m_pKData->ElementAt(nIndex).m_fHigh + m_pKData->ElementAt(nIndex).m_fLow + m_pKData->ElementAt(nIndex).m_fClose)/3.0;
	double	minN = 0, min2N = 0, maxN = 0, max2N = 0;
	for( int k=nIndex; k>=0; k -- )
	{
		KDATA	kd	=	m_pKData->ElementAt(k);
		if( nIndex-k < m_nDays )
		{
			if( nIndex == k )	{	minN = kd.m_fLow;	maxN = kd.m_fHigh;	}
			if( kd.m_fLow < minN )		minN = kd.m_fLow;
			if( kd.m_fHigh > maxN )	maxN = kd.m_fHigh;
		}
		if( nIndex-k < m_nDays*2 )
		{
			if( nIndex == k )	{	min2N = kd.m_fLow;	max2N = kd.m_fHigh;	}
			if( kd.m_fLow < min2N )	min2N = kd.m_fLow;
			if( kd.m_fHigh > max2N )	max2N = kd.m_fHigh;
		}
		else
		{
			break;
		}
	}
	if( pWR )	*pWR	=	( TP + (TP - minN) ) ;
	if( pMR )	*pMR	=	( TP + (maxN - minN) ) ;
	if( pSR )	*pSR	=	( TP + (max2N - minN) ) ;
	if( pWS )	*pWS	=	( TP - (maxN - TP) ) ;
	if( pMS )	*pMS	=	( TP - (maxN - minN) ) ;
	if( pSS )	*pSS	=	( TP - (maxN - min2N) ) ;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CPSY
CPSY::CPSY( )
{
	SetDefaultParameters( );
}

CPSY::CPSY( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CPSY::~CPSY()
{
	Clear( );
}

void CPSY::SetDefaultParameters( )
{
	m_nDays			=	12;
	m_itsSold		=	ITS_BUY;
	m_itsBought		=	ITS_SELL;
}

void CPSY::AttachParameters( CPSY & src )
{
	m_nDays			=	src.m_nDays;
	m_itsSold		=	src.m_itsSold;
	m_itsBought		=	src.m_itsBought;
}

BOOL CPSY::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void CPSY::Clear( )
{
	CTechnique::Clear( );
}

int CPSY::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	if( nIndex <= 0 )
		return ITS_NOTHING;

	double	dPSY = 0, dPSYLast;
	if( !Calculate( &dPSYLast, nIndex-1, FALSE )
		|| !Calculate( &dPSY, nIndex, FALSE ) )
		return ITS_NOTHING;
	if( dPSY < 30 && dPSY >= dPSYLast )
	{	// 超卖
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dPSY > 70 && dPSY <= dPSYLast )
	{	// 超买
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}
	return	ITS_NOTHING;
}

BOOL CPSY::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	if( pdMin )	*pdMin	=	0;
	if( pdMax )	*pdMax	=	100;
	return TRUE;
}

/***
	        N日内上涨的天数
	PSY = —————————— × 100
	              N
*/
BOOL CPSY::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;
	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	UD	=	0;
	int		nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		if( m_pKData->MaindataAt(k) > m_pKData->MaindataAt(k-1) )
			UD	+=	1;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( pValue )	*pValue	=	UD * 100 / m_nDays;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	return FALSE;
}


