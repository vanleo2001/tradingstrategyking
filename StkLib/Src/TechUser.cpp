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
//	CTechUserDrv

class CTechUserDrv
{
public:
	CTechUserDrv();
	~CTechUserDrv();

	BOOL	LoadDriver( );

	UINT	GetTechUserCount( );
	BOOL	GetTechUserInfo( UINT nID, PTECHUSER_INFO pInfo );
	BOOL	Calculate( UINT nID, PCALCULATE_INFO pInfo );
	int		GetSignal( UINT nID, PCALCULATE_INFO pInfo );

protected:
	UINT	(WINAPI * m_pfnGetTechUserCount)( );
	BOOL	(WINAPI * m_pfnGetTechUserInfo)( UINT nID, PTECHUSER_INFO pInfo );
	BOOL	(WINAPI * m_pfnCalculate)( UINT nID, PCALCULATE_INFO pInfo );
	int		(WINAPI * m_pfnGetSignal)( UINT nID, PCALCULATE_INFO pInfo );
	HINSTANCE	m_hDrv;
};

CTechUserDrv & GetTechUserDrv( )
{
	static	CTechUserDrv	g_techuserdrv;
	g_techuserdrv.LoadDriver( );
	return g_techuserdrv;
}

CTechUserDrv::CTechUserDrv()
{
	m_pfnGetTechUserCount	=	NULL;
	m_pfnGetTechUserInfo	=	NULL;
	m_pfnCalculate			=	NULL;
	m_pfnGetSignal			=	NULL;
	m_hDrv					=	NULL;
}

CTechUserDrv::~CTechUserDrv()
{
#ifdef	SP_WINDOWS
	if( m_hDrv )
		FreeLibrary( m_hDrv );
#endif
	m_pfnGetTechUserCount	=	NULL;
	m_pfnGetTechUserInfo	=	NULL;
	m_pfnCalculate			=	NULL;
	m_pfnGetSignal			=	NULL;
	m_hDrv					=	NULL;
}

BOOL CTechUserDrv::LoadDriver( )
{
#ifdef	SP_WINDOWS
	if( !m_hDrv )
	{
		m_hDrv = ::LoadLibrary( "StkTech.dll" );
		if( m_hDrv )
		{
			m_pfnGetTechUserCount	= (UINT (WINAPI *)())GetProcAddress(m_hDrv,"_GetTechUserCount@0");
			m_pfnGetTechUserInfo	= (BOOL (WINAPI *)(UINT,PTECHUSER_INFO))GetProcAddress(m_hDrv,"_GetTechUserInfo@8");
			m_pfnCalculate			= (BOOL (WINAPI *)(UINT,PCALCULATE_INFO))GetProcAddress(m_hDrv,"_Calculate@8");
			m_pfnGetSignal			= (int (WINAPI *)(UINT,PCALCULATE_INFO))GetProcAddress(m_hDrv,"_GetSignal@8");
			if( !m_pfnGetTechUserCount )
				m_pfnGetTechUserCount	= (UINT (WINAPI *)())GetProcAddress(m_hDrv,"GetTechUserCount");
			if( !m_pfnGetTechUserInfo )
				m_pfnGetTechUserInfo	= (BOOL (WINAPI *)(UINT,PTECHUSER_INFO))GetProcAddress(m_hDrv,"GetTechUserInfo");
			if( !m_pfnCalculate )
				m_pfnCalculate			= (BOOL (WINAPI *)(UINT,PCALCULATE_INFO))GetProcAddress(m_hDrv,"Calculate");
			if( !m_pfnGetSignal )
				m_pfnGetSignal			= (int (WINAPI *)(UINT,PCALCULATE_INFO))GetProcAddress(m_hDrv,"GetSignal");
		}
	}
#endif
	
	return NULL != m_hDrv;
}

UINT CTechUserDrv::GetTechUserCount( )
{
	if( !m_pfnGetTechUserCount )
		return 0;
	return( (*m_pfnGetTechUserCount)());
}

BOOL CTechUserDrv::GetTechUserInfo( UINT nID, PTECHUSER_INFO pInfo )
{
	if( !m_pfnGetTechUserInfo )
		return FALSE;
	return( (*m_pfnGetTechUserInfo)(nID-STT_USER_MIN,pInfo));
}

BOOL CTechUserDrv::Calculate( UINT nID, PCALCULATE_INFO pInfo )
{
	if( !m_pfnCalculate )
		return FALSE;
	return( (*m_pfnCalculate)(nID-STT_USER_MIN,pInfo));
}

int CTechUserDrv::GetSignal( UINT nID, PCALCULATE_INFO pInfo )
{
	if( !m_pfnCalculate )
		return ITS_NOTHING;
	return( (*m_pfnGetSignal)(nID-STT_USER_MIN,pInfo));
}

//////////////////////////////////////////////////////////////////////
//	CTechUser

//  static functions
UINT	CTechUser::GetTechUserCount()
{
	return GetTechUserDrv().GetTechUserCount();
}

CSPString CTechUser::GetTechUserName( int nTech )
{
	TECHUSER_INFO	info;
	memset( &info, 0, sizeof(info) );
	info.m_nStructSize	=	sizeof(info);
	if( GetTechUserDrv().GetTechUserInfo( nTech, &info ) )
		return info.m_szName;
	return "";
}

CSPString CTechUser::GetTechUserShortName( int nTech )
{
	TECHUSER_INFO	info;
	memset( &info, 0, sizeof(info) );
	info.m_nStructSize	=	sizeof(info);
	if( GetTechUserDrv().GetTechUserInfo( nTech, &info ) )
		return info.m_szShortName;
	return "";
}

CTechUser::CTechUser( )
{
	SetDefaultParameters( );
}

CTechUser::CTechUser( CKData * pKData )
	: CTechnique( pKData )
{
	SetDefaultParameters( );
}

CTechUser::~CTechUser()
{
	Clear( );
}

BOOL CTechUser::SetTechUserID( int nTechUserID )
{
	m_nTechUserID	=	nTechUserID;
	if( m_nTechUserID >= STT_USER_MIN
		&& m_nTechUserID-STT_USER_MIN<CTechUser::GetTechUserCount() )
		return GetTechUserDrv().GetTechUserInfo( m_nTechUserID, &m_techuserinfo );
	return FALSE;
}

UINT CTechUser::GetTechUserID( )
{
	return m_nTechUserID;
}

void CTechUser::SetStock( CStock * pStock )
{
	m_pStock	=	pStock;
}

UINT CTechUser::GetLineCount( )
{
	if( 1 == m_techuserinfo.m_nLineCount && m_techuserinfo.m_bAutoMA )
		return 2;
	return m_techuserinfo.m_nLineCount;
}

CSPString CTechUser::GetLineName( int nLine )
{
	switch( nLine )
	{
	case 1:		return m_techuserinfo.m_aszLineName[0];
	case 2:
		if( 1 == m_techuserinfo.m_nLineCount && m_techuserinfo.m_bAutoMA )
			return "MA";
		return m_techuserinfo.m_aszLineName[1];
	case 3:		return m_techuserinfo.m_aszLineName[2];
	case 4:		return m_techuserinfo.m_aszLineName[3];
	case 5:		return m_techuserinfo.m_aszLineName[4];
	default:;
	}
	return "";
}

void CTechUser::SetDefaultParameters( )
{
	m_nTechUserID	=	-1;
	memset( &m_techuserinfo, 0, sizeof(m_techuserinfo) );
}

void CTechUser::AttachParameters( CTechUser & src )
{
	m_nTechUserID	=	src.m_nTechUserID;
	memcpy( &m_techuserinfo, &(src.m_techuserinfo), sizeof(m_techuserinfo) );
}

BOOL CTechUser::IsValidParameters( )
{
	return ( m_nTechUserID >= STT_USER_MIN
		&& m_nTechUserID-STT_USER_MIN<CTechUser::GetTechUserCount() );
}

void CTechUser::Clear( )
{
	CTechnique::Clear( );
}

int CTechUser::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	if( ( m_techuserinfo.m_nLineCount == 2 || (m_techuserinfo.m_nLineCount == 1 && m_techuserinfo.m_bAutoMA ) )
		&& ( ITS_NOTHING != m_techuserinfo.m_itsGoldenFork || ITS_NOTHING != m_techuserinfo.m_itsDeadFork ) )
	{
		int	nMaxDays	=	0;
		if( m_techuserinfo.m_nLineCount == 1 && m_techuserinfo.m_bAutoMA )
			nMaxDays	=	m_techuserinfo.m_nMADays;
		double	dLiminalLow = 0, dLiminalHigh = 0;
		if( !IntensityPrepare( nIndex, pnCode, nMaxDays, ITS_GETMINMAXDAYRANGE, &dLiminalLow, &dLiminalHigh, 0.3, 0.7 ) )
			return ITS_NOTHING;

		if( nIndex <= 1 )
			return ITS_NOTHING;

		double	dValue1, dValue2;
		if( !Calculate( &dValue1, &dValue2, nIndex, FALSE ) )
			return ITS_NOTHING;

		int	nSignal	=	GetForkSignal( nIndex, m_techuserinfo.m_itsGoldenFork, m_techuserinfo.m_itsDeadFork, pnCode );
		if( dValue1 < dLiminalLow && nSignal == ITS_BUY )
		{	// 低位金叉
			if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
			return ITS_BUY;
		}
		if( dValue1 > dLiminalHigh && nSignal == ITS_SELL )
		{	// 高位死叉
			if( pnCode )	*pnCode	=	ITSC_DEADFORK;
			return ITS_SELL;
		}
	}

	CALCULATE_INFO	info;
	memset( &info, 0, sizeof(info) );
	info.m_nStructSize	=	sizeof(info);
	info.m_pStock		=	m_pStock;
	info.m_pStockInfo	=	&(m_pStock->GetStockInfo());
	info.m_pKData		=	m_pKData;
	info.m_nIndex		=	nIndex;
	int nSignal = ITS_NOTHING;
	if( GetTechUserDrv().GetSignal( m_nTechUserID, &info ) )
	{
		nSignal	=	info.m_nSignal;
		if( nSignal > ITS_NOTHING && pnCode )
			*pnCode	=	ITSC_LONG;
		if( nSignal < ITS_NOTHING && pnCode )
			*pnCode	=	ITSC_SHORT;
	}
	return nSignal;
}

// 得到趋势信号
int CTechUser::GetIntensity( int nIndex, UINT * pnCode, BOOL bTraceBack )
{
	return CTechnique::GetIntensity( nIndex, pnCode, bTraceBack );
}

int CTechUser::GetIntensityTraceback( int nIndex, UINT * pnCode )
{
	return CTechnique::GetIntensityTraceback( nIndex, pnCode );
}

BOOL CTechUser::GetMinMaxInfo(int nStart, int nEnd,
				   double *pdMin, double *pdMax )
{
	switch( m_techuserinfo.m_nLineCount )
	{
	case 1:
		return AfxGetMinMaxInfo1( nStart, nEnd, pdMin, pdMax, this );
	case 2:
		return AfxGetMinMaxInfo2( nStart, nEnd, pdMin, pdMax, this );
	case 3:
		return AfxGetMinMaxInfo3( nStart, nEnd, pdMin, pdMax, this );
	case 4:
		return AfxGetMinMaxInfo4( nStart, nEnd, pdMin, pdMax, this );
	case 5:
		return AfxGetMinMaxInfo5( nStart, nEnd, pdMin, pdMax, this );
	default:;
	}
	return FALSE;
}

BOOL CTechUser::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	SP_ASSERT( 1 == m_techuserinfo.m_nLineCount );
	if( 1 != m_techuserinfo.m_nLineCount )
		return FALSE;

	if( LoadFromCache( nIndex, pValue ) )
		return TRUE;

	CALCULATE_INFO	info;
	memset( &info, 0, sizeof(info) );
	info.m_nStructSize	=	sizeof(info);
	info.m_pStock		=	m_pStock;
	info.m_pStockInfo	=	&(m_pStock->GetStockInfo());
	info.m_pKData		=	m_pKData;
	info.m_nIndex		=	nIndex;
	info.m_bUseLast		=	bUseLast;
	if( bUseLast )
		info.m_dValue1	=	*pValue;
	if( !GetTechUserDrv().Calculate( m_nTechUserID, &info ) )
		return FALSE;

	if( pValue )
		*pValue	=	info.m_dValue1;
	StoreToCache( nIndex, pValue );
	return TRUE;
}

BOOL CTechUser::Calculate( double * pValue1, double * pValue2, int nIndex, BOOL bUseLast )
{
	if( 1 == m_techuserinfo.m_nLineCount && m_techuserinfo.m_bAutoMA )
		return CalculateMA( pValue1, pValue2, nIndex, bUseLast, m_techuserinfo.m_nMADays );

	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	SP_ASSERT( 2 == m_techuserinfo.m_nLineCount );
	if( 2 != m_techuserinfo.m_nLineCount )
		return FALSE;

	if( LoadFromCache( nIndex, pValue1, pValue2 ) )
		return TRUE;

	CALCULATE_INFO	info;
	memset( &info, 0, sizeof(info) );
	info.m_nStructSize	=	sizeof(info);
	info.m_pStock		=	m_pStock;
	info.m_pStockInfo	=	&(m_pStock->GetStockInfo());
	info.m_pKData		=	m_pKData;
	info.m_nIndex		=	nIndex;
	info.m_bUseLast		=	bUseLast;
	if( bUseLast )
	{
		info.m_dValue1	=	*pValue1;
		info.m_dValue2	=	*pValue2;
	}
	if( !GetTechUserDrv().Calculate( m_nTechUserID, &info ) )
		return FALSE;

	if( pValue1 )	*pValue1	=	info.m_dValue1;
	if( pValue2 )	*pValue2	=	info.m_dValue2;
	StoreToCache( nIndex, pValue1, pValue2 );
	return TRUE;
}

BOOL CTechUser::Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	SP_ASSERT( 3 == m_techuserinfo.m_nLineCount );
	if( 3 != m_techuserinfo.m_nLineCount )
		return FALSE;

	if( LoadFromCache( nIndex, pValue1, pValue2, pValue3 ) )
		return TRUE;

	CALCULATE_INFO	info;
	memset( &info, 0, sizeof(info) );
	info.m_nStructSize	=	sizeof(info);
	info.m_pStock		=	m_pStock;
	info.m_pStockInfo	=	&(m_pStock->GetStockInfo());
	info.m_pKData		=	m_pKData;
	info.m_nIndex		=	nIndex;
	info.m_bUseLast		=	bUseLast;
	if( bUseLast )
	{
		info.m_dValue1	=	*pValue1;
		info.m_dValue2	=	*pValue2;
		info.m_dValue3	=	*pValue3;
	}
	if( !GetTechUserDrv().Calculate( m_nTechUserID, &info ) )
		return FALSE;

	if( pValue1 )	*pValue1	=	info.m_dValue1;
	if( pValue2 )	*pValue2	=	info.m_dValue2;
	if( pValue3 )	*pValue3	=	info.m_dValue3;
	StoreToCache( nIndex, pValue1, pValue2, pValue3 );
	return TRUE;
}

BOOL CTechUser::Calculate( double * pValue1, double * pValue2, double * pValue3, double *pValue4, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	SP_ASSERT( 4 == m_techuserinfo.m_nLineCount );
	if( 4 == m_techuserinfo.m_nLineCount )
		return FALSE;

	if( LoadFromCache( nIndex, pValue1, pValue2, pValue3, pValue4 ) )
		return TRUE;

	CALCULATE_INFO	info;
	memset( &info, 0, sizeof(info) );
	info.m_nStructSize	=	sizeof(info);
	info.m_pStock		=	m_pStock;
	info.m_pStockInfo	=	&(m_pStock->GetStockInfo());
	info.m_pKData		=	m_pKData;
	info.m_nIndex		=	nIndex;
	info.m_bUseLast		=	bUseLast;
	if( bUseLast )
	{
		info.m_dValue1	=	*pValue1;
		info.m_dValue2	=	*pValue2;
		info.m_dValue3	=	*pValue3;
		info.m_dValue4	=	*pValue4;
	}
	if( !GetTechUserDrv().Calculate( m_nTechUserID, &info ) )
		return FALSE;

	if( pValue1 )	*pValue1	=	info.m_dValue1;
	if( pValue2 )	*pValue2	=	info.m_dValue2;
	if( pValue3 )	*pValue3	=	info.m_dValue3;
	if( pValue4 )	*pValue4	=	info.m_dValue4;
	StoreToCache( nIndex, pValue1, pValue2, pValue3, pValue4 );
	return TRUE;
}

BOOL CTechUser::Calculate( double * pValue1, double * pValue2, double * pValue3, double *pValue4, double *pValue5, int nIndex, BOOL bUseLast )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	SP_ASSERT( 5 == m_techuserinfo.m_nLineCount );
	if( 5 == m_techuserinfo.m_nLineCount )
		return FALSE;

	if( LoadFromCache( nIndex, pValue1, pValue2, pValue3, pValue4, pValue5 ) )
		return TRUE;

	CALCULATE_INFO	info;
	memset( &info, 0, sizeof(info) );
	info.m_nStructSize	=	sizeof(info);
	info.m_pStock		=	m_pStock;
	info.m_pStockInfo	=	&(m_pStock->GetStockInfo());
	info.m_pKData		=	m_pKData;
	info.m_nIndex		=	nIndex;
	info.m_bUseLast		=	bUseLast;
	if( bUseLast )
	{
		info.m_dValue1	=	*pValue1;
		info.m_dValue2	=	*pValue2;
		info.m_dValue3	=	*pValue3;
		info.m_dValue4	=	*pValue4;
		info.m_dValue5	=	*pValue5;
	}
	if( !GetTechUserDrv().Calculate( m_nTechUserID, &info ) )
		return FALSE;

	if( pValue1 )	*pValue1	=	info.m_dValue1;
	if( pValue2 )	*pValue2	=	info.m_dValue2;
	if( pValue3 )	*pValue3	=	info.m_dValue3;
	if( pValue4 )	*pValue4	=	info.m_dValue4;
	if( pValue5 )	*pValue5	=	info.m_dValue5;
	StoreToCache( nIndex, pValue1, pValue2, pValue3, pValue4, pValue5 );
	return TRUE;
}
