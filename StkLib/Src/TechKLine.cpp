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
//	CKLine
CKLine::CKLine( )
{
	SetDefaultParameters( );
}

CKLine::CKLine( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CKLine::~CKLine()
{
	Clear( );
}

void CKLine::SetDefaultParameters( )
{
}

void CKLine::AttachParameters( CKLine & src )
{
}

BOOL CKLine::IsValidParameters( )
{
	return TRUE;
}

void CKLine::Clear( )
{
	CTechnique::Clear( );
}

/***
	得到K线价格的从nStart到nEnd的最小值和最大值
*/
BOOL CKLine::GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax )
{
	STT_ASSERT_GETMINMAXINFO( m_pKData, nStart, nEnd );

	double	dMin	=	-1;
	double	dMax	=	1;
	for( int k=nStart; k<=nEnd; k++ )
	{
		KDATA	& kd	=	m_pKData->ElementAt(k);
		if( nStart == k || dMin > kd.m_fLow )	dMin	=	(double)kd.m_fLow;
		if( nStart == k || dMax < kd.m_fHigh )	dMax	=	(double)kd.m_fHigh;
	}
	dMin	-=	fabs(dMin) * 0.01;
	dMax	+=	fabs(dMax) * 0.01;
	if( dMin <= 0 )
		dMin	=	0;
	if( dMax - dMin < 0.03 )
		dMax	=	dMin + 0.03;

	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CMA
CMA::CMA( )
{
	SetDefaultParameters( );
}

CMA::CMA( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CMA::~CMA()
{
	Clear( );
}

void CMA::SetDefaultParameters( )
{
	m_nType	=	typeMA;
	m_adwMADays.RemoveAll();
	m_adwMADays.Add( 5 );
	m_adwMADays.Add( 10 );
	m_adwMADays.Add( 20 );
	m_itsGoldenFork	=	ITS_BUYINTENSE;
	m_itsDeadFork	=	ITS_SELLINTENSE;
	m_itsLong		=	ITS_BUY;
	m_itsShort		=	ITS_SELL;
}

void CMA::AttachParameters( CMA & src )
{
	m_nType	=	src.m_nType;
	m_adwMADays.Copy( src.m_adwMADays );
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
	m_itsLong		=	src.m_itsLong;
	m_itsShort		=	src.m_itsShort;
}

BOOL CMA::IsValidParameters( )
{
	STT_VALID_DAYSARRAY( m_adwMADays );
	return ( (typeMA == m_nType || typeEXPMA == m_nType)
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork)
		&& VALID_ITS(m_itsLong) && VALID_ITS(m_itsShort) );
}

void CMA::Clear( )
{
	CTechnique::Clear( );
}

int CMA::GetSignal( int nIndex, UINT * pnCode )
{
	// 金叉或者死叉
	int	nSignal	=	GetForkSignal( nIndex, m_adwMADays, m_itsGoldenFork, m_itsDeadFork, pnCode );
	if( ITS_ISBUY(nSignal) || ITS_ISSELL(nSignal) )
		return nSignal;
	// 趋势
	return GetTrendIntensity( nIndex, m_adwMADays, m_itsLong, m_itsShort, pnCode );
}

BOOL CMA::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo( nStart, nEnd, pdMin, pdMax, this, m_adwMADays );
}

/***
	两种：
	1. MA
	    MA = n日收盘价的平均值	
	2. EXPMA
	    EXPMA(1) = CLOSE(1)
		EXPMA(i) = (1-α)EXPMA(i-1) + αCLOSE(i)
		其中 α = 2 / (n+1)
*/
BOOL CMA::Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE( m_pKData, nIndex, nDays );

	int	nCount	=	0;
	if( nDays > nIndex+1 )
		return FALSE;

	double	dResult	=	0;
	int	k = 0;
	switch( m_nType )
	{
	case typeMA:
		return m_pKData->GetMA( pValue, nIndex, nDays );
		break;
	case typeEXPMA:
		if( bUseLast && pValue )
		{
			if( 0 == nIndex )
				dResult	=	m_pKData->MaindataAt(nIndex);
			else
				dResult	=	(*pValue)*(nDays-1)/(nDays+1) + m_pKData->MaindataAt(nIndex) * 2./(nDays+1);
		}
		else
		{
			for( k=0; k<=nIndex; k++ )
			{
				if( 0 == k )
					dResult	=	m_pKData->MaindataAt(k);
				else
					dResult	=	dResult*(nDays-1)/(nDays+1) + m_pKData->MaindataAt(k) * 2./(nDays+1);
			}
		}
		if( pValue )
			*pValue	=	dResult;
		break;
	default:
		SP_ASSERT( FALSE );
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CBBI
CBBI::CBBI( )
{
	SetDefaultParameters( );
}

CBBI::CBBI( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CBBI::~CBBI()
{
	Clear( );
}

void CBBI::SetDefaultParameters( )
{
	m_nMA1Days	=	3;
	m_nMA2Days	=	6;
	m_nMA3Days	=	12;
	m_nMA4Days	=	24;
	m_itsGoldenFork	=	ITS_BUY;
	m_itsDeadFork	=	ITS_SELL;
}

void CBBI::AttachParameters( CBBI & src )
{
	m_nMA1Days	=	src.m_nMA1Days;
	m_nMA2Days	=	src.m_nMA2Days;
	m_nMA3Days	=	src.m_nMA3Days;
	m_nMA4Days	=	src.m_nMA4Days;
	m_itsGoldenFork	=	src.m_itsGoldenFork;
	m_itsDeadFork	=	src.m_itsDeadFork;
}

BOOL CBBI::IsValidParameters( )
{
	return ( VALID_DAYS( m_nMA1Days ) && VALID_DAYS( m_nMA2Days )
		&& VALID_DAYS( m_nMA3Days ) && VALID_DAYS( m_nMA4Days )
		&& VALID_ITS(m_itsGoldenFork) && VALID_ITS(m_itsDeadFork) );
}

void CBBI::Clear( )
{
	CTechnique::Clear( );
}

int CBBI::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( nIndex <= 0 )
		return ITS_NOTHING;

	double	dLiminalLow = 0, dLiminalHigh = 0;
	if( !IntensityPreparePrice( nIndex, pnCode, 0, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.4, 0.6 ) )
		return ITS_NOTHING;

	double	dBBINow = 0, dBBILast = 0;
	if( !Calculate( &dBBILast, nIndex-1, FALSE )
		|| !Calculate( &dBBINow, nIndex, FALSE ) )
		return ITS_NOTHING;

	double	dNowHigh	=	m_pKData->ElementAt(nIndex).m_fHigh;
	double	dNowLow		=	m_pKData->ElementAt(nIndex).m_fLow;
	double	dNowClose	=	m_pKData->ElementAt(nIndex).m_fClose;
	double	dLastHigh	=	m_pKData->ElementAt(nIndex-1).m_fHigh;
	double	dLastLow	=	m_pKData->ElementAt(nIndex-1).m_fLow;
	double	dLastClose	=	m_pKData->ElementAt(nIndex-1).m_fClose;

	if( dNowClose < dLiminalLow && dLastLow < dBBILast && dNowLow > dBBINow )
	{	// 低位趋势向上
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return m_itsGoldenFork;
	}
	if( dNowClose > dLiminalHigh && dLastHigh > dBBILast && dNowHigh < dBBINow )
	{	// 高位趋势向下
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return m_itsDeadFork;
	}
	return	ITS_NOTHING;
}

BOOL CBBI::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	BBI = 4 个 不同日期的MA 的平均值
*/
BOOL CBBI::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;
	
	double	dResult	=	0;
	double	dTemp	=	0;

	if( !m_pKData->GetMA( &dTemp, nIndex, m_nMA1Days ) )
		return FALSE;
	dResult	+=	dTemp;

	if( !m_pKData->GetMA( &dTemp, nIndex, m_nMA2Days ) )
		return FALSE;
	dResult	+=	dTemp;

	if( !m_pKData->GetMA( &dTemp, nIndex, m_nMA3Days ) )
		return FALSE;
	dResult	+=	dTemp;

	if( !m_pKData->GetMA( &dTemp, nIndex, m_nMA4Days ) )
		return FALSE;
	dResult	+=	dTemp;

	dResult	=	dResult / 4;
	if( pValue )
		*pValue	=	dResult;

	StoreToCache( nIndex, pValue );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CBOLL
CBOLL::CBOLL( )
{
	SetDefaultParameters( );
}

CBOLL::CBOLL( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CBOLL::~CBOLL()
{
	Clear( );
}

void CBOLL::SetDefaultParameters( )
{
	m_dMultiUp		=	2;
	m_dMultiDown	=	2;
	m_nMADays		=	20;
	m_itsSupport	=	ITS_BUY;
	m_itsResistance	=	ITS_SELL;
}

void CBOLL::AttachParameters( CBOLL & src )
{
	m_dMultiUp		=	src.m_dMultiUp;
	m_dMultiDown	=	src.m_dMultiDown;
	m_nMADays		=	src.m_nMADays;
	m_itsSupport	=	src.m_itsSupport;
	m_itsResistance	=	src.m_itsResistance;
}

BOOL CBOLL::IsValidParameters( )
{
	return ( m_dMultiUp > 0 && m_dMultiDown > 0 && VALID_DAYS( m_nMADays )
		&& VALID_ITS(m_itsSupport) && VALID_ITS(m_itsResistance) );
}

void CBOLL::Clear( )
{
	CTechnique::Clear( );
}

int CBOLL::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	double	dMA = 0, dUp = 0, dDown = 0;
	if( !Calculate( &dMA, &dUp, &dDown, nIndex, FALSE ) )
		return ITS_NOTHING;

	double	dClose	=	m_pKData->ElementAt(nIndex).m_fClose;

	if( dClose < dDown )
	{	// 跌破支撑位
		if( pnCode )	*pnCode	=	ITSC_SUPPORT;
		return m_itsSupport;
	}
	if( dClose > dUp )
	{	// 涨过阻力位
		if( pnCode )	*pnCode	=	ITSC_RESISTANCE;
		return m_itsResistance;
	}
	return	ITS_NOTHING;
}

BOOL CBOLL::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo3( nStart, nEnd, pdMin, pdMax, this );
}

/***
	布林带是以股价平均线MA为中心线，上方阻力线MA+αSn和下方支撑线MA-αSn之间的带状区域
	其中 Sn为n日收盘价的标准差
*/
BOOL CBOLL::Calculate( double * pdMA, double * pdUp, double * pdDown, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );
	
	if( m_nMADays < 2 )
		return FALSE;

	if( LoadFromCache( nIndex, pdMA, pdUp, pdDown ) )
		return TRUE;

	double	dMA = 0, dUp = 0, dDown = 0, dS = 0;

	if( !m_pKData->GetMA( &dMA, nIndex, m_nMADays ) )
		return FALSE;

	int	nCount	=	0;
	for( int k=nIndex; k>=0; k-- )
	{
		dS	+=	(m_pKData->MaindataAt(k) - dMA) * (m_pKData->MaindataAt(k) - dMA);
		nCount	++;
		if( nCount == m_nMADays )
			break;
	}

	dS	=	sqrt( dS / (m_nMADays-1) );

	dUp		=	dMA + m_dMultiUp * dS;
	dDown	=	dMA - m_dMultiDown * dS;
	if( pdMA )		*pdMA	=	dMA;
	if( pdUp )		*pdUp	=	dUp;
	if( pdDown )	*pdDown	=	dDown;

	StoreToCache( nIndex, pdMA, pdUp, pdDown );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CPV
CPV::CPV( )
{
	SetDefaultParameters( );
}

CPV::CPV( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CPV::~CPV()
{
	Clear( );
}

void CPV::SetDefaultParameters( )
{
}

void CPV::AttachParameters( CPV & src )
{
}

BOOL CPV::IsValidParameters( )
{
	return TRUE;
}

void CPV::Clear( )
{
	CTechnique::Clear( );
}

int CPV::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	// 无买卖信号
	return	ITS_NOTHING;
}

BOOL CPV::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	PV就是当日成交均价，成交额除以成交量
*/
BOOL CPV::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	KDATA	kd	=	m_pKData->ElementAt(nIndex);
	if( kd.m_fVolume <= 1e-4 || kd.m_fAmount <= 1e-4 )
		return  FALSE;
	
	int		nCount	=	0;
	double	average	=	((double)(kd.m_fAmount)) / kd.m_fVolume;
	while( average < kd.m_fLow && nCount < 10 )	{	average	*=	10;	nCount ++;	}
	while( average > kd.m_fHigh && nCount < 20 )	{	average	/=	10;	nCount ++;	}
	if( average < kd.m_fLow )		//	说明是指数
		average	=	(kd.m_fOpen+kd.m_fHigh+kd.m_fLow+kd.m_fClose)/4;

	double	dPV	=	average;
	if( pValue )
		*pValue	=	dPV;

	StoreToCache( nIndex, pValue );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CSAR
CSAR::CSAR( )
{
	SetDefaultParameters( );
}

CSAR::CSAR( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );

	m_bCurUp	=	m_bFirstUp;
	m_bTurn		=	FALSE;
	m_dCurAF	=	m_dAFStep;
	m_dCurHigh	=	-1;
	m_dCurLow	=	-1;
}

CSAR::~CSAR()
{
	Clear( );
}

void CSAR::SetDefaultParameters( )
{
	m_nInitDays	=	4;
	m_bFirstUp	=	TRUE;
	m_dAFStep	=	0.02;
	m_dAFMax	=	0.2;
	m_itsBuy	=	ITS_BUY;
	m_itsSell	=	ITS_SELL;
}

void CSAR::AttachParameters( CSAR & src )
{
	m_nInitDays	=	src.m_nInitDays;
	m_bFirstUp	=	src.m_bFirstUp;
	m_dAFStep	=	src.m_dAFStep;
	m_dAFMax	=	src.m_dAFMax;
	m_itsBuy	=	src.m_itsBuy;
	m_itsSell	=	src.m_itsSell;
}

BOOL CSAR::IsValidParameters( )
{
	return ( VALID_DAYS(m_nInitDays) && m_bFirstUp >= 0 && m_dAFStep > 0 && m_dAFMax > 0
		&& VALID_ITS(m_itsBuy) && VALID_ITS(m_itsSell) );
}

void CSAR::Clear( )
{
	CTechnique::Clear( );
}

BOOL CSAR::CalculateSAR( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nInitDays > nIndex + 1 )
		return FALSE;

	double	dResult	=	0;
	if( bUseLast && pValue && nIndex > 0 && !m_bTurn )
	{
		KDATA	kd	=	m_pKData->ElementAt(nIndex-1);
		if( m_bCurUp )
		{
			dResult	=	(*pValue) + m_dCurAF * (kd.m_fHigh - (*pValue) );
			if( kd.m_fHigh > m_dCurHigh )
			{
				m_dCurHigh	=	kd.m_fHigh;
				m_dCurAF	=	m_dCurAF + m_dAFStep;
				if( m_dCurAF > m_dAFMax )
					m_dCurAF	=	m_dAFMax;
			}
			if( m_pKData->ElementAt(nIndex).m_fLow < dResult )
				m_bTurn		=	TRUE;
		}
		else
		{
			dResult	=	(*pValue) - m_dCurAF * ((*pValue) - kd.m_fLow );
			if( kd.m_fLow < m_dCurLow )
			{
				m_dCurLow	=	kd.m_fLow;
				m_dCurAF	=	m_dCurAF + m_dAFStep;
				if( m_dCurAF > m_dAFMax )
					m_dCurAF	=	m_dAFMax;
			}
			if( m_pKData->ElementAt(nIndex).m_fHigh > dResult )
				m_bTurn		=	TRUE;
		}
	}
	else
	{
		for( int k=nIndex; k>=nIndex-m_nInitDays+1; k-- )
		{
			KDATA	kd	=	m_pKData->ElementAt(k);
			if( nIndex == k )
			{
				m_dCurHigh	=	kd.m_fHigh;
				m_dCurLow	=	kd.m_fLow;
			}
			else if( kd.m_fHigh > m_dCurHigh )
				m_dCurHigh	=	kd.m_fHigh;
			else if( kd.m_fLow < m_dCurLow )
				m_dCurLow	=	kd.m_fLow;
		}
		if( m_bTurn )
			m_bCurUp	=	! m_bCurUp;
		else
			m_bCurUp	=	m_bFirstUp;
		m_bTurn		=	FALSE;
		m_dCurAF	=	m_dAFStep;
		if( m_bCurUp )
			dResult	=	m_dCurLow;
		else
			dResult	=	m_dCurHigh;
	}

	if( pValue )
		*pValue	=	dResult;
	return TRUE;
}

int CSAR::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	Clear( );
	double dValue;
	if( !Calculate( &dValue, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( m_bTurn && !m_bCurUp )
	{	// 反转向上
		if( pnCode )	*pnCode	=	ITSC_LONG;
		return m_itsBuy;
	}
	if( m_bTurn && m_bCurUp )
	{	// 反转向下
		if( pnCode )	*pnCode	=	ITSC_SHORT;
		return m_itsSell;
	}
	return	ITS_NOTHING;
}

BOOL CSAR::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
}

/***
	计算SAR值
	先选定时间，判断价格是在上涨还是在下跌。
	若是看涨，则进场第一天的SAR必须是近期内的最低价，是看跌 则进场第一天的SAR必须是近期内的最高价。
	本处为缺省定义为看涨。

	进场第二天的SAR则为第一天的最高价（看涨时）或最低价（看跌时）与第一天的SAR的差距乘上调整系数，
	再加上第一天的SAR就可求得。
	按逐步递推的方法，每日的SAR可归纳如下： SAR（N）= SAR（N-1）+ AF * [ EP（N-1）-SAR（N-1）]
	其中SAR（N）为第N日的SAR值，AF是调整系数，EP为极点价
	
	第一个调整系数AF为0.02，若每隔一天的最高价比前一天的最高价还高，则AF递增0.02，若未创新高，
则AF沿用前一天的数值，但调整系数最高不超过0.2。
	若是买进期间，计算出某日的SAR比当日或前一日的最低价还高，则应以当日或者前一日的最低价为某日之SAR，
卖出期间也对应服从类似原则。
*/
BOOL CSAR::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	if( bUseLast && pValue && nIndex > 0 )
	{
		if( CalculateSAR( pValue, nIndex, bUseLast ) )
		{
			StoreToCache( nIndex, pValue );
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		double	dResult;
		BOOL	bHasLast	=	FALSE;
		for( int k=0; k<=nIndex; k++ )
		{
			if( CalculateSAR( &dResult, k, bHasLast ) )
			{
				bHasLast	=	TRUE;
				StoreToCache( k, &dResult );
			}
		}
		if( !bHasLast )
			return FALSE;
		if( pValue )
			*pValue	=	dResult;
		return TRUE;
	}
}

//////////////////////////////////////////////////////////////////////
//	CDJ
CStock		CDJ::m_stockSha;
CStock		CDJ::m_stockSzn;
CSPString	CDJ::m_strCodeOrg;
CDJ::CDJ( )
{
	SetDefaultParameters( );
}

CDJ::CDJ( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CDJ::~CDJ()
{
	Clear( );
}

void CDJ::SetDefaultParameters( )
{
	m_strCodeSha	=	STKLIB_CODE_MAIN;
	m_strCodeSzn	=	STKLIB_CODE_MAINSZN;
}

void CDJ::AttachParameters( CDJ & src )
{
	m_strCodeSha	=	src.m_strCodeSha;
	m_strCodeSzn	=	src.m_strCodeSzn;
}

BOOL CDJ::IsValidParameters( )
{
	return ( m_strCodeSha.GetLength() > 0 && m_strCodeSzn.GetLength() > 0 );
}

void CDJ::Clear( )
{
	CTechnique::Clear( );
}

/***
	K线叠加图，准备叠加K线的数据
*/
BOOL CDJ::PrepareStockData(CStDatabase * pDatabase, const char * szCodeOrg,
						   int nCurKType, int nCurKFormat, int nCurMaindataType,
						   DWORD dwAutoResumeDRBegin, int nAutoResumeDRLimit )
{
	SP_ASSERT( pDatabase );

	// bReload and kdayMain
	BOOL	bReload	=	(NULL!=szCodeOrg && 0!=strncmp(szCodeOrg,m_strCodeOrg,m_strCodeOrg.GetLength()) );
	m_strCodeOrg	=	szCodeOrg;

	// m_stockSha
	m_stockSha.SetStockCode( CStock::marketSHSE, m_strCodeSha );
	AfxPrepareStockData( pDatabase, m_stockSha, nCurKType, nCurKFormat, nCurMaindataType, FALSE, bReload );

	// m_stockSzn
	m_stockSzn.SetStockCode( CStock::marketSZSE, m_strCodeSzn );
	AfxPrepareStockData( pDatabase, m_stockSzn, nCurKType, nCurKFormat, nCurMaindataType, FALSE, bReload );
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//	CCW
CCW::CCW( )
{
	SetDefaultParameters( );
}

CCW::CCW( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CCW::~CCW()
{
	Clear( );
}

void CCW::SetDefaultParameters( )
{
	m_dChangeHand	=	1.5;
}

void CCW::AttachParameters( CCW & src )
{
	m_dChangeHand	=	src.m_dChangeHand;
}

BOOL CCW::IsValidParameters( )
{
	return ( m_dChangeHand > 0 );
}

void CCW::Clear( )
{
	CTechnique::Clear( );
}

/***
	根据换手率m_dChangeHand和终止日，计算起始日
*/
BOOL CCW::GetRange( int & nStart, int & nEnd, CStockInfo & info )
{
	if( !m_pKData || m_pKData->GetSize() <= 0 )
		return FALSE;
	if( nEnd < 0 || nEnd >= m_pKData->GetSize() )
		nEnd	=	m_pKData->GetSize()-1;
	
	BOOL	bIndex = FALSE;
	double	dShareCurrency = 0;
	if( !info.GetShareCurrency( &dShareCurrency ) || dShareCurrency < 1e+6 )
		bIndex	=	TRUE;

	if( bIndex )
		dShareCurrency	=	100 * m_dChangeHand;
	else
		dShareCurrency	*=	m_dChangeHand;

	double	dVol	=	0;
	int k;
	for( k=nEnd; k>=0; k-- )
	{
		if( bIndex )
			dVol	+=	1;
		else
			dVol	+=	m_pKData->ElementAt(k).m_fVolume;

		if( dVol > dShareCurrency )
			break;
	}
	nStart	=	k;
	if( nStart < 0 )
		nStart	=	0;
	return TRUE;
}

BOOL CCW::GetMinMaxInfo(int nStart, int nEnd, double dMinPrice, double dMaxPrice, double dStep,
				double *pdMinVolume, double *pdMaxVolume )
{
	STT_ASSERT_GETMINMAXINFO( m_pKData, nStart, nEnd );
	if( dMinPrice >= dMaxPrice || dStep < 1e-4 )
		return FALSE;

	double	dMinVolume = 0, dMaxVolume = 0, dVolume = 0;
	BOOL	bFirst	=	TRUE;
	for( double dPrice = dMinPrice; dPrice < dMaxPrice; dPrice += dStep )
	{
		if( CalculateCW( &dVolume, nStart, nEnd, dPrice, dStep ) )
		{
			if( bFirst || dVolume < dMinVolume )	dMinVolume	=	dVolume;
			if( bFirst || dVolume > dMaxVolume )	dMaxVolume	=	dVolume;
			bFirst	=	FALSE;
		}
	}

	dMinVolume	-=	fabs(dMinVolume)*0.01;
	dMaxVolume	+=	fabs(dMaxVolume)*0.01;
	if( dMaxVolume - dMinVolume < 3 )
		dMaxVolume	=	dMinVolume + 3;
	if( pdMinVolume )		*pdMinVolume	=	dMinVolume;
	if( pdMaxVolume )		*pdMaxVolume	=	dMaxVolume;

	return !bFirst;
}

/***
	筹码分布图，计算价格区间包括dPrice的日线的成交量
*/
BOOL CCW::CalculateCW( double *pdVolume, int nStart, int nEnd, double dPrice, double dStep )
{
	STT_ASSERT_GETMINMAXINFO( m_pKData, nStart, nEnd );

	double	dVolume	=	0;
	for( int k=nStart; k<=nEnd; k++ )
	{
		KDATA	kd	=	m_pKData->ElementAt(k);
		if( kd.m_fHigh-kd.m_fLow > 1e-4
			&& kd.m_fLow < dPrice && kd.m_fHigh > dPrice )
		{
			// 均匀分布 dVolAve
			double	dVolAve = kd.m_fVolume;
			if( dStep < kd.m_fHigh-kd.m_fLow )
				dVolAve	=	kd.m_fVolume * dStep / (kd.m_fHigh-kd.m_fLow);

			// 三角分布
			double	dFactor	=	min(dPrice-kd.m_fLow, kd.m_fHigh-dPrice);
			dVolume	+=	dVolAve * dFactor * 4 / (kd.m_fHigh-kd.m_fLow);
		}
	}
	
	if( pdVolume )
		*pdVolume	=	dVolume;
	return TRUE;
}

/***
	筹码分布图计算，计算筹码分布
*/
BOOL CCW::CalculateCW(	int nStart, int nEnd, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent )
{
	STT_ASSERT_GETMINMAXINFO( m_pKData, nStart, nEnd );
	if( dStep < 1e-4 )
		return FALSE;

	float dMinPrice = 0, dMaxPrice = 0;
	if( !m_pKData->GetMinMaxInfo( nStart, nEnd, &dMinPrice, &dMaxPrice ) )
		return FALSE;

	// Calculate
	int nMaxCount = (int)((dMaxPrice-dMinPrice)/dStep) + 10;
	adwPrice.SetSize( 0, nMaxCount );
	adwVolume.SetSize( 0, nMaxCount );
	double	dMinVolume = 0, dMaxVolume = 0, dTotalVolume = 0, dVolume = 0;
	BOOL	bFirst	=	TRUE;
	for( double dPrice = dMinPrice; dPrice < dMaxPrice; dPrice += dStep )
	{
		if( CalculateCW( &dVolume, nStart, nEnd, dPrice, dStep ) )
		{
			if( bFirst || dVolume < dMinVolume )	dMinVolume	=	dVolume;
			if( bFirst || dVolume > dMaxVolume )	dMaxVolume	=	dVolume;
			adwPrice.Add( DWORD(dPrice * 1000) );
			adwVolume.Add( DWORD(dVolume) );
			dTotalVolume	+=	dVolume;
			bFirst	=	FALSE;
		}
	}

	// Return
	// Min Max
	dMinVolume	-=	fabs(dMinVolume)*0.01;
	dMaxVolume	+=	fabs(dMaxVolume)*0.01;
	if( dMaxVolume - dMinVolume < 3 )
		dMaxVolume	=	dMinVolume + 3;
	if( pdMinVolume )	*pdMinVolume	=	dMinVolume;
	if( pdMaxVolume )	*pdMaxVolume	=	dMaxVolume;
	if( pdTotalVolume )	*pdTotalVolume	=	dTotalVolume;

	// VolPercent
	double dVolPercent = 1.0;
	double	dShareCurrency = 0;
	if( (!info.GetShareCurrency( &dShareCurrency ) || dShareCurrency < 1e+6) && nEnd-nStart+1 > 0 )
		dShareCurrency	=	dTotalVolume * 100 / (nEnd-nStart+1);
	if( dShareCurrency > 1e-4 )
		dVolPercent	=	dTotalVolume / (dShareCurrency*m_dChangeHand);
	if( dVolPercent > 1.0 )		dVolPercent	=	1.0;
	if( dVolPercent < 0.0 )		dVolPercent	=	0.0;
	if( pdVolPercent )	*pdVolPercent	=	dVolPercent;

	return adwPrice.GetSize() > 0;
}

/***
	筹码分布图计算，计算最近nDays天内的筹码分布
*/
BOOL CCW::CalculateRecentCW(int nEnd, int nDays, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent )
{
	// Prepare
	if( !m_pKData || m_pKData->GetSize() <= 0 )
		return FALSE;
	if( nEnd < 0 || nEnd >= m_pKData->GetSize() )
		nEnd	=	m_pKData->GetSize()-1;
	int	nStart = nEnd - nDays + 1;
	if( nStart < 0 || nStart >= m_pKData->GetSize() )
		return FALSE;

	return CalculateCW( nStart, nEnd, info, dStep, adwPrice, adwVolume, pdMinVolume, pdMaxVolume, pdTotalVolume, pdVolPercent );
}

/***
	筹码分布图计算，计算nDays天前内的筹码分布
*/
BOOL CCW::CalculatePastCW(int nEnd, int nDays, CStockInfo & info, double dStep,
						CSPDWordArray & adwPrice, CSPDWordArray & adwVolume,
						double * pdMinVolume, double * pdMaxVolume, double * pdTotalVolume, double * pdVolPercent )
{
	// Prepare
	if( !m_pKData || m_pKData->GetSize() <= 0 )
		return FALSE;
	if( nEnd < 0 || nEnd >= m_pKData->GetSize() )
		nEnd	=	m_pKData->GetSize()-1;
	int	nStart = nEnd - nDays;
	if( nStart < 0 || nStart >= m_pKData->GetSize() )
		return FALSE;

	nEnd = nStart;
	if( !GetRange( nStart, nEnd, info ) )
		return FALSE;

	BOOL bOK = CalculateCW( nStart, nEnd, info, dStep, adwPrice, adwVolume, pdMinVolume, pdMaxVolume, pdTotalVolume, pdVolPercent );

	// TotalVolumeRecent
	double dTotalVolumeRecent = 0;
	for( int k=nEnd+1; k<=nEnd+nDays && k<m_pKData->GetSize(); k ++ )
		dTotalVolumeRecent	+=	m_pKData->ElementAt(k).m_fVolume;

	// VolPercent
	double dVolPercent = 1.0;
	double	dShareCurrency = 0;
	if( (!info.GetShareCurrency( &dShareCurrency ) || dShareCurrency < 1e+6) && nDays > 0 )
		dShareCurrency	=	dTotalVolumeRecent * 100 / nDays;
	if( dShareCurrency > 1e-4 )
		dVolPercent	=	dTotalVolumeRecent / (dShareCurrency*m_dChangeHand);
	dVolPercent	=	1.0 - dVolPercent;
	if( dVolPercent > 1.0 )		dVolPercent	=	1.0;
	if( dVolPercent < 0.0 )		dVolPercent	=	0.0;
	if( pdVolPercent )	*pdVolPercent	=	dVolPercent;

	return bOK;
}

/***
	筹码分布图统计，获利比例统计
*/
BOOL CCW::StatGainPercent( double *pdGainPercent, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume, double dPriceSel )
{
	double dTotalVolume = 0;
	double dGainVolume = 0;
	for( int k=0; k<adwPrice.GetSize() && k<adwVolume.GetSize(); k++ )
	{
		dTotalVolume	+=	adwVolume[k];
		if( adwPrice[k] * 0.001 <= dPriceSel )
			dGainVolume	+=	adwVolume[k];
	}

	double dGainPercent = 0;
	if( dTotalVolume > 1e-4 )
		dGainPercent	=	dGainVolume / dTotalVolume;

	if( pdGainPercent )	*pdGainPercent = dGainPercent;
	return TRUE;
}

/***
	筹码分布图统计，平均成本统计
*/
BOOL CCW::StatCostAverage( double *pdCostAve, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume )
{
	double dTotalVolume = 0;
	double dTotalCost = 0;
	for( int k=0; k<adwPrice.GetSize() && k<adwVolume.GetSize(); k++ )
	{
		dTotalVolume	+=	adwVolume[k];
		dTotalCost		+=	0.001 * adwPrice[k] * adwVolume[k];
	}

	double dCostAve = 0;
	if( dTotalVolume > 1e-4 )
		dCostAve	=	dTotalCost / dTotalVolume;

	if( pdCostAve )	*pdCostAve = dCostAve;
	return TRUE;
}
	
/***
	筹码分布图统计，集中度统计
*/
BOOL CCW::StatMass( double *pdLower, double *pdUpper, double *pdMassPrice, CSPDWordArray &adwPrice, CSPDWordArray &adwVolume, double dMassVol )
{
	if( adwPrice.GetSize() != adwVolume.GetSize() || dMassVol < 0 || dMassVol > 1 )
		return FALSE;

	double dTotalVolume = 0;
	for( int k=0; k<adwPrice.GetSize() && k<adwVolume.GetSize(); k++ )
		dTotalVolume	+=	adwVolume[k];

	if( dTotalVolume > 1e-4 )
	{
		double dUpperVolume = dTotalVolume * (1-dMassVol) * 0.5;
		double dLowerVolume = dUpperVolume;
		int nLower = 0, nUpper = adwPrice.GetSize()-1;

		int k;
		for( k=0; k<adwPrice.GetSize(); k++ )
		{
			dLowerVolume -= (double)adwVolume[k];
			if( dLowerVolume < 0 )
				break;
		}
		nLower	=	k;

		for( k=adwPrice.GetSize()-1; k>=0; k-- )
		{
			dUpperVolume -= (double)adwVolume[k];
			if( dUpperVolume < 0 )
				break;
		}
		nUpper	=	k;
		
		if( nLower < 0 || nLower > nUpper || nUpper >= adwPrice.GetSize() )
			return FALSE;

		double dLower = 0.001 * adwPrice[nLower];
		double dUpper = 0.001 * adwPrice[nUpper];
		if( pdLower )	*pdLower	=	dLower;
		if( pdUpper )	*pdUpper	=	dUpper;
		if( pdMassPrice && adwPrice.GetSize() >= 2 )
		{
			double dPriceRange = 0.001 * ((double)adwPrice[adwPrice.GetSize()-1] - (double)adwPrice[0]);
			if( dPriceRange > 1e-4 )
				*pdMassPrice	=	(dUpper-dLower)/dPriceRange;
			if( *pdMassPrice < 0 )	*pdMassPrice	=	0;
			if( *pdMassPrice > 1 )	*pdMassPrice	=	1;
		}
	}

	return TRUE;
}
