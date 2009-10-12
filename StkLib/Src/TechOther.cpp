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
//	CREI
CREI::CREI( )
{
	SetDefaultParameters( );
}

CREI::CREI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CREI::~CREI()
{
	Clear( );
}

void CREI::SetDefaultParameters( )
{
	m_nDays		=	8;
	m_itsLong	=	ITS_BUY;
	m_itsShort	=	ITS_SELL;
}

void CREI::AttachParameters( CREI & src )
{
	m_nDays		=	src.m_nDays;
	m_itsLong	=	src.m_itsLong;
	m_itsShort	=	src.m_itsShort;
}

BOOL CREI::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_ITS(m_itsLong) && VALID_ITS(m_itsShort) );
}

void CREI::Clear( )
{
	CTechnique::Clear( );
}

int CREI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	if( nIndex <= 0 )
		return ITS_NOTHING;

	double	dLast, dNow;
	if( !Calculate( &dLast, nIndex-1, FALSE )
		|| !Calculate( &dNow, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dLast < -0.6 && dNow > -0.6 )
	{	// 低位做多
		if( pnCode )	*pnCode	=	ITSC_LONG;
		return m_itsLong;
	}
	if( dLast > 0.6 && dNow < 0.6 )
	{	// 高位做空
		if( pnCode )	*pnCode	=	ITSC_SHORT;
		return m_itsShort;
	}

	return	ITS_NOTHING;
}

BOOL CREI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	DIF1 = 今日最高价 - 2日前最高价
	DIF2 = 今日最低价 - 2日前最低价
	A = N日内除满足以下情况日的（DIF1+DIF2）之和
		1. 2日前最高价 小于 7日前收盘价
		2. 2日前最高价 小于 8日前收盘价
		3. 今日前最高价 小于 5日前最低价
		4. 今日前最高价 小于 6日前最低价
		5. 2日前最低价 小于 7日前收盘价
		6. 2日前最低价 小于 8日前收盘价
		7. 今日前最低价 小于 5日前最高价
		8. 今日前最低价 小于 6日前最高价
	REIA = N日的DIF1绝对值之和 + N日的DIF2绝对值之和
	REI  = A / REIA
*/
BOOL CREI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays+7 > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dREI = 0, dREIA = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=8; k-- )
	{
		double	dDIF1 = 0, dDIF2 = 0;
		int		num1 = 1, num2 = 1;
		dDIF1	=	((double)m_pKData->ElementAt(k).m_fHigh) - m_pKData->ElementAt(k-2).m_fHigh;
		dDIF2	=	((double)m_pKData->ElementAt(k).m_fLow) - m_pKData->ElementAt(k-2).m_fLow;
		if( m_pKData->ElementAt(k-2).m_fHigh < m_pKData->ElementAt(k-7).m_fClose
			&& m_pKData->ElementAt(k-2).m_fHigh < m_pKData->ElementAt(k-8).m_fClose
			&& m_pKData->ElementAt(k).m_fHigh < m_pKData->ElementAt(k-5).m_fLow
			&& m_pKData->ElementAt(k).m_fHigh < m_pKData->ElementAt(k-6).m_fLow )
			num1	=	0;
		if( m_pKData->ElementAt(k-2).m_fLow > m_pKData->ElementAt(k-7).m_fClose
			&& m_pKData->ElementAt(k-2).m_fLow > m_pKData->ElementAt(k-8).m_fClose
			&& m_pKData->ElementAt(k).m_fLow > m_pKData->ElementAt(k-5).m_fHigh
			&& m_pKData->ElementAt(k).m_fLow > m_pKData->ElementAt(k-6).m_fHigh )
			num2	=	0;
		dREI	+=	(dDIF1+dDIF2) * num1 * num2;
		dREIA	+=	fabs(dDIF1) + fabs(dDIF2);

		nCount	++;
		if( nCount == m_nDays )
		{
			if( fabs(dREIA) < 1e-4 )
				return FALSE;
			if( pValue )
				*pValue	=	dREI / dREIA;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CDMKI
CDMKI::CDMKI( )
{
	SetDefaultParameters( );
}

CDMKI::CDMKI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CDMKI::~CDMKI()
{
	Clear( );
}

void CDMKI::SetDefaultParameters( )
{
	m_nDays		=	13;
	m_itsSold	=	ITS_BUY;
	m_itsBought	=	ITS_SELL;
}

void CDMKI::AttachParameters( CDMKI & src )
{
	m_nDays		=	src.m_nDays;
	m_itsSold	=	src.m_itsSold;
	m_itsBought	=	src.m_itsBought;
}

BOOL CDMKI::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void CDMKI::Clear( )
{
	CTechnique::Clear( );
}

int CDMKI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	double	dDMKI;
	if( !Calculate( &dDMKI, nIndex, FALSE ) )
		return ITS_NOTHING;
	if( dDMKI < 0.3 )
	{	// 超卖
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dDMKI > 0.7 )
	{	// 超买
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}
	return	ITS_NOTHING;
}

BOOL CDMKI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	DMH = N日内最高价大于昨日最高价日的 （最高价-昨日最高价）
	DML = N日内最低价小于昨日最低价日的 （昨日最低价-最低价）
	DMKI = DMH / (DMH+DML)
*/
BOOL CDMKI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dDMH = 0, dDML = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);
		KDATA	kdLast	=	m_pKData->ElementAt(k-1);
		if( kd.m_fHigh > kdLast.m_fHigh )
			dDMH	+=	(((double)kd.m_fHigh)-kdLast.m_fHigh);
		if( kd.m_fLow < kdLast.m_fLow )
			dDML	+=	(((double)kdLast.m_fLow)-kd.m_fLow);

		nCount	++;
		if( nCount == m_nDays )
		{
			if( fabs(dDMH + dDML) < 1e-4 )
				return FALSE;
			if( pValue )	*pValue	=	dDMH / (dDMH+dDML);
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CPCNT
CPCNT::CPCNT( )
{
	SetDefaultParameters( );
}

CPCNT::CPCNT( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CPCNT::~CPCNT()
{
	Clear( );
}

void CPCNT::SetDefaultParameters( )
{
	m_nMADays	=	6;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CPCNT::AttachParameters( CPCNT & src )
{
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CPCNT::IsValidParameters( )
{
	return ( VALID_DAYS(m_nMADays) && VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CPCNT::Clear( )
{
	CTechnique::Clear( );
}

int CPCNT::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	
	int	nMaxDays	=	m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.5, 0.5 ) )
		return ITS_NOTHING;

	double	dPCNT;
	if( !Calculate( &dPCNT, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( dPCNT < dLiminalLow && nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dPCNT > dLiminalHigh && nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}
	
	return ITS_NOTHING;
}

BOOL CPCNT::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	         今日收盘 - N日前收盘
	PCNT = ———————————— × 100%
	              昨日收盘
*/
BOOL CPCNT::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;
	
	int	nDays	=	1;	//	same as ROC with m_nDays==1
	if( nDays > nIndex )
		return FALSE;

	double	dROC = 0;
	if( m_pKData->MaindataAt(nIndex-nDays) <= 0
		|| m_pKData->MaindataAt(nIndex) <= 0 )
		return FALSE;

	double	x	=	m_pKData->MaindataAt(nIndex);
	double	y	=	m_pKData->MaindataAt(nIndex-nDays);
	if( pValue )
		*pValue	=	(x - y) * 100 / y;
	StoreToCache( nIndex, pValue );
	return TRUE;
}

/***
	计算PCNT及其移动平均值
*/
BOOL CPCNT::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CTechnique::CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CHLC
CHLC::CHLC( )
{
	SetDefaultParameters( );
}

CHLC::CHLC( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CHLC::~CHLC()
{
	Clear( );
}

void CHLC::SetDefaultParameters( )
{
	m_nDays		=	12;
	m_nMADays	=	6;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CHLC::AttachParameters( CHLC & src )
{
	m_nDays		=	src.m_nDays;
	m_nMADays	=	src.m_nMADays;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CHLC::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_DAYS(m_nMADays)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CHLC::Clear( )
{
	CTechnique::Clear( );
}

int CHLC::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	
	int	nMaxDays	=	m_nDays + m_nMADays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE*3, &dLiminalLow, &dLiminalHigh, 0.4, 0.6 ) )
		return ITS_NOTHING;

	double	dHLC;
	if( !Calculate( &dHLC, nIndex, FALSE ) )
		return ITS_NOTHING;

	int	nSignal	=	GetForkSignal( nIndex, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( nSignal == m_itsGoldenFork )
	{	// 低位金叉
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( nSignal == m_itsDeadFork )
	{	// 高位死叉
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}
	
	return ITS_NOTHING;
}

BOOL CHLC::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
}

/***
	TP = (收盘价+收盘价+最高价+最低价)/4
	HLC = N日TP平均值
*/
BOOL CHLC::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dMATP = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);
		double	dTP		= (kd.m_fHigh+kd.m_fLow+kd.m_fClose*2)/4.;
		dMATP	+=	dTP;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( pValue )	*pValue	=	dMATP / m_nDays;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	return FALSE;
}

/***
	计算HLC及其移动平均值
*/
BOOL CHLC::Calculate( double * pValue, double * pMA, int nIndex, BOOL bUseLast )
{
	return CTechnique::CalculateMA( pValue, pMA, nIndex, bUseLast, m_nMADays );
}

//////////////////////////////////////////////////////////////////////
//	CCDP
CCDP::CCDP( )
{
	SetDefaultParameters( );
}

CCDP::CCDP( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CCDP::~CCDP()
{
	Clear( );
}

void CCDP::SetDefaultParameters( )
{
}

void CCDP::AttachParameters( CCDP & src )
{
}

BOOL CCDP::IsValidParameters( )
{
	return TRUE;
}

void CCDP::Clear( )
{
	CTechnique::Clear( );
}

int CCDP::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	// 无买卖信号
	return	ITS_NOTHING;
}

BOOL CCDP::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo4( nStart, nEnd, pdMin, pdMax, this );
}

/***
	TP = (昨日收盘价+昨日收盘价+昨日最高价+昨日最低价)/4
	AH = TP + 昨日最高价 - 昨日最低价
	AL = TP - 最日最高价 + 昨日最低价
	NH = TP + TP - 最日最低价
	NL = TP + TP - 最日最高价
*/
BOOL CCDP::Calculate( double * pAH, double * pNH, double * pAL, double * pNL, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( nIndex < 1 )
		return FALSE;

	if( LoadFromCache( nIndex, pAH, pNH, pAL, pNL ) )
		return TRUE;

	KDATA	kdLast	=	m_pKData->ElementAt(nIndex-1);
	double	dTP	=	(kdLast.m_fHigh+kdLast.m_fLow+kdLast.m_fClose*2)/4.;
	if( pAH )	*pAH	=	(dTP + kdLast.m_fHigh - kdLast.m_fLow);
	if( pNH )	*pNH	=	(dTP + dTP - kdLast.m_fLow);
	if( pAL )	*pAL	=	(dTP - kdLast.m_fHigh + kdLast.m_fLow);
	if( pNL )	*pNL	=	(dTP - kdLast.m_fHigh + dTP);
	StoreToCache( nIndex, pAH, pNH, pAL, pNL );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CASI
CASI::CASI( )
{
	SetDefaultParameters( );
}

CASI::CASI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CASI::~CASI()
{
	Clear( );
}

void CASI::SetDefaultParameters( )
{
	m_nDays		=	6;
	m_itsDeviateOnBottom	=	ITS_BUY;
	m_itsDeviateOnTop		=	ITS_SELL;
}

void CASI::AttachParameters( CASI & src )
{
	m_nDays		=	src.m_nDays;
	m_itsDeviateOnBottom	=	src.m_itsDeviateOnBottom;
	m_itsDeviateOnTop		=	src.m_itsDeviateOnTop;
}

BOOL CASI::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_ITS(m_itsDeviateOnBottom) && VALID_ITS(m_itsDeviateOnTop) );
}

void CASI::Clear( )
{
	CTechnique::Clear( );
}

int CASI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh ) )
		return ITS_NOTHING;

	if( IsDeviateOnBottom( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// 底背离
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONBOTTOM;
		return m_itsDeviateOnBottom;
	}
	if( IsDeviateOnTop( nIndex, m_pdCache1, m_pdCache2 ) )
	{	// 顶背离
		if( pnCode )	*pnCode	=	ITSC_DEVIATEONTOP;
		return m_itsDeviateOnTop;
	}

	return	ITS_NOTHING;
}

BOOL CASI::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	A = 今最高 - 昨收盘
	B = 今最低 - 昨最低
	C = 今最高 - 昨最低
	D = 昨收盘 - 昨开盘
	E = 今收盘 - 昨收盘
	F = 今收盘 - 昨开盘
	G = 昨收盘 - 昨开盘
	X = E + 1/(2F) + G
	K = A、B二者之间较大者
	比较A、B、C三者的大小
		若A大，则R = A+1/(2B)+1/(4D)
		若B大，则R = B+1/(2A)+1/(4D)
		若C大，则R = C+1/(4D)
	L = 3
	SI = 50·X·K/(R·L)
	ASI = N日SI之和
*/
BOOL CASI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dASI = 0;
	double	A, B, C, D, E, F, G;
	double	R, X, K, SI;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);
		KDATA	kdLast	=	m_pKData->ElementAt(k-1);
		A	=	fabs(((double)kd.m_fHigh) - kdLast.m_fClose);
		B	=	fabs(((double)kd.m_fLow) - kdLast.m_fClose);
		C	=	fabs(((double)kd.m_fHigh) - kdLast.m_fLow);
		D	=	fabs(((double)kdLast.m_fClose) - kdLast.m_fOpen);
		E	=	((double)kd.m_fClose) - kdLast.m_fClose;
		F	=	((double)kd.m_fClose) - kd.m_fOpen;
		G	=	((double)kdLast.m_fClose) - kdLast.m_fOpen;

		if( fabs(A) < 1e-4 || fabs(B) < 1e-4 || fabs(D) < 1e-4 || fabs(F) < 1e-4 )
			continue;

		if( A >= B && A >= C )
			R	=	A + 1/(2*B) + 1/(4*D);
		else if( B >= A && B >= C )
			R	=	B + 1/(2*A) + 1/(4*D);
		else
			R	=	C + 1/(4*D);

		if( fabs(R) < 1e-4 )
			continue;

		X	=	E + 1/(2*F) + G;
		K	=	( A > B ? A : B );
		SI	=	X * K * 50 / (3*R);

		dASI	+=	SI;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( pValue )	*pValue	=	dASI;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//	CATR
CATR::CATR( )
{
	SetDefaultParameters( );
}

CATR::CATR( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CATR::~CATR()
{
	Clear( );
}

void CATR::SetDefaultParameters( )
{
	m_nDays		=	14;
	m_itsSold	=	ITS_BUY;
	m_itsBought	=	ITS_SELL;
}

void CATR::AttachParameters( CATR & src )
{
	m_nDays		=	src.m_nDays;
	m_itsSold	=	src.m_itsSold;
	m_itsBought	=	src.m_itsBought;
}

BOOL CATR::IsValidParameters( )
{
	return ( VALID_DAYS(m_nDays) && VALID_ITS(m_itsSold) && VALID_ITS(m_itsBought) );
}

void CATR::Clear( )
{
	CTechnique::Clear( );
}

int CATR::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int	nMaxDays	=	m_nDays;
	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh ) )
		return ITS_NOTHING;

	double	dATR;
	if( !Calculate( &dATR, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dATR > dLiminalHigh )
	{	// 超卖
		if( pnCode )	*pnCode	=	ITSC_OVERSOLD;
		return m_itsSold;
	}
	if( dATR < dLiminalLow )
	{	// 超买
		if( pnCode )	*pnCode	=	ITSC_OVERBOUGHT;
		return m_itsBought;
	}
	return	ITS_NOTHING;
}

BOOL CATR::GetMinMaxInfo(int nStart, int nEnd, double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	TR 为以下三者中的最大值
		最高价-最低价，(昨日收盘价-今日最高价)的绝对值，昨日收盘价-昨日最低价

	ATR = TR的N日平均
*/
BOOL CATR::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nDays > nIndex )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	double	dATR = 0;
	int	nCount	=	0;
	for( int k=nIndex; k>=1; k-- )
	{
		KDATA	kd		=	m_pKData->ElementAt(k);
		KDATA	kdLast	=	m_pKData->ElementAt(k-1);

		double	dTR	=	fabs(((double)kd.m_fHigh)-kd.m_fLow);
		if( fabs(((double)kdLast.m_fClose)-kd.m_fHigh) > dTR )
			dTR	=	fabs(((double)kdLast.m_fClose)-kd.m_fHigh);
		if( fabs(((double)kdLast.m_fClose)-kdLast.m_fLow) > dTR )
			dTR	=	fabs(((double)kdLast.m_fClose)-kdLast.m_fLow);

		dATR	+=	dTR;

		nCount	++;
		if( nCount == m_nDays )
		{
			if( pValue )	*pValue	=	dATR/m_nDays;
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
	}
	return FALSE;
}

