/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CTechnique;
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

//////////////////////////////////////////////////////////////////
// GetMinMaxInfo
// 一些反复使用的代码，用函数代替，得到技术指标在指定范围内的最大最小值
// 技术指标有不同日期作为参数
BOOL AfxGetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech, CSPDWordArray & adwDays )
{
	SP_ASSERT( pTech );
	if( nStart < 0 || nEnd < 0 || nStart > nEnd || !pTech )
		return FALSE;

	double	dMin = 0, dMax = 0;
	double	dValue = 0;
	BOOL	bFirst	=	TRUE;
	for( int i=0; i<adwDays.GetSize(); i++ )
	{
		for( int k=nStart; k<=nEnd; k++ )
		{
			if( pTech->Calculate( &dValue, k, adwDays[i], !bFirst ) )
			{
				if( bFirst || dValue < dMin )	dMin	=	dValue;
				if( bFirst || dValue > dMax )	dMax	=	dValue;
				bFirst	=	FALSE;
			}
		}
	}
	dMin	-=	fabs(dMin)*0.02;
	dMax	+=	fabs(dMax)*0.02;
	if( dMax - dMin < 0.03 )
		dMax	=	dMin + 0.05;
	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;
	
	return !bFirst;
}

// 技术指标有1个最终值
BOOL AfxGetMinMaxInfo1( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech )
{
	SP_ASSERT( pTech );
	if( nStart < 0 || nEnd < 0 || nStart > nEnd || !pTech )
		return FALSE;

	double	dMin = 0, dMax = 0;
	double	dValue = 0;
	BOOL	bFirst	=	TRUE;
	for( int k=nStart; k<=nEnd; k++ )
	{
		if( pTech->Calculate( &dValue, k, !bFirst ) )	// 技术指标有1个最终值
		{
			if( bFirst || dValue < dMin )	dMin	=	dValue;
			if( bFirst || dValue > dMax )	dMax	=	dValue;
			bFirst	=	FALSE;
		}
	}
	dMin	-=	fabs(dMin)*0.02;
	dMax	+=	fabs(dMax)*0.02;
	if( dMax - dMin < 0.03 )
		dMax	=	dMin + 0.05;
	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;

	return !bFirst;
}

// 技术指标有2个最终值
BOOL AfxGetMinMaxInfo2( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech )
{
	SP_ASSERT( pTech );
	if( nStart < 0 || nEnd < 0 || nStart > nEnd || !pTech )
		return FALSE;

	double	dMin = 0, dMax = 0;
	double	dValue1 = 0, dValue2 = 0;
	BOOL	bFirst	=	TRUE;
	for( int k=nStart; k<=nEnd; k++ )
	{
		if( pTech->Calculate( &dValue1, &dValue2, k, !bFirst ) )	// 技术指标有2个最终值
		{
			if( bFirst || dValue1 < dMin )	dMin	=	dValue1;
			if( bFirst || dValue1 > dMax )	dMax	=	dValue1;
			if( dValue2 < dMin )	dMin	=	dValue2;
			if( dValue2 > dMax )	dMax	=	dValue2;
			bFirst	=	FALSE;
		}
	}
	dMin	-=	fabs(dMin)*0.02;
	dMax	+=	fabs(dMax)*0.02;
	if( dMax - dMin < 0.03 )
		dMax	=	dMin + 0.05;
	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;
	
	return !bFirst;
}

// 技术指标有3个最终值
BOOL AfxGetMinMaxInfo3( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech )
{
	SP_ASSERT( pTech );
	if( nStart < 0 || nEnd < 0 || nStart > nEnd || !pTech )
		return FALSE;

	double	dMin = 0, dMax = 0;
	double	dValue1 = 0, dValue2 = 0, dValue3 = 0;
	BOOL	bFirst	=	TRUE;
	for( int k=nStart; k<=nEnd; k++ )
	{
		if( pTech->Calculate( &dValue1, &dValue2, &dValue3, k, !bFirst ) )	// 技术指标有3个最终值
		{
			if( bFirst || dValue1 < dMin )	dMin	=	dValue1;
			if( bFirst || dValue1 > dMax )	dMax	=	dValue1;
			if( dValue2 < dMin )	dMin	=	dValue2;
			if( dValue2 > dMax )	dMax	=	dValue2;
			if( dValue3 < dMin )	dMin	=	dValue3;
			if( dValue3 > dMax )	dMax	=	dValue3;
			bFirst	=	FALSE;
		}
	}
	dMin	-=	fabs(dMin)*0.02;
	dMax	+=	fabs(dMax)*0.02;
	if( dMax - dMin < 0.03 )
		dMax	=	dMin + 0.05;
	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;

	return !bFirst;
}

// 技术指标有4个最终值
BOOL AfxGetMinMaxInfo4( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech )
{
	SP_ASSERT( pTech );
	if( nStart < 0 || nEnd < 0 || nStart > nEnd || !pTech )
		return FALSE;

	double	dMin = 0, dMax = 0;
	double	dValue1 = 0, dValue2 = 0, dValue3 = 0, dValue4 = 0;
	BOOL	bFirst	=	TRUE;
	for( int k=nStart; k<=nEnd; k++ )
	{
		if( pTech->Calculate( &dValue1, &dValue2, &dValue3, &dValue4, k, !bFirst ) )	// 技术指标有4个最终值
		{
			if( bFirst || dValue1 < dMin )	dMin	=	dValue1;
			if( bFirst || dValue1 > dMax )	dMax	=	dValue1;
			if( dValue2 < dMin )	dMin	=	dValue2;
			if( dValue2 > dMax )	dMax	=	dValue2;
			if( dValue3 < dMin )	dMin	=	dValue3;
			if( dValue3 > dMax )	dMax	=	dValue3;
			if( dValue4 < dMin )	dMin	=	dValue4;
			if( dValue4 > dMax )	dMax	=	dValue4;
			bFirst	=	FALSE;
		}
	}
	dMin	-=	fabs(dMin)*0.02;
	dMax	+=	fabs(dMax)*0.02;
	if( dMax - dMin < 0.03 )
		dMax	=	dMin + 0.05;
	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;

	return !bFirst;
}

// 技术指标有5个最终值
BOOL AfxGetMinMaxInfo5( int nStart, int nEnd, double *pdMin, double *pdMax, CTechnique * pTech )
{
	SP_ASSERT( pTech );
	if( nStart < 0 || nEnd < 0 || nStart > nEnd || !pTech )
		return FALSE;

	double	dMin = 0, dMax = 0;
	double	dValue1 = 0, dValue2 = 0, dValue3 = 0, dValue4 = 0, dValue5 = 0;
	BOOL	bFirst	=	TRUE;
	for( int k=nStart; k<=nEnd; k++ )
	{
		if( pTech->Calculate( &dValue1, &dValue2, &dValue3, &dValue4, &dValue5, k, !bFirst ) )	// 技术指标有5个最终值
		{
			if( bFirst || dValue1 < dMin )	dMin	=	dValue1;
			if( bFirst || dValue1 > dMax )	dMax	=	dValue1;
			if( dValue2 < dMin )	dMin	=	dValue2;
			if( dValue2 > dMax )	dMax	=	dValue2;
			if( dValue3 < dMin )	dMin	=	dValue3;
			if( dValue3 > dMax )	dMax	=	dValue3;
			if( dValue4 < dMin )	dMin	=	dValue4;
			if( dValue4 > dMax )	dMax	=	dValue4;
			if( dValue5 < dMin )	dMin	=	dValue5;
			if( dValue5 > dMax )	dMax	=	dValue5;
			bFirst	=	FALSE;
		}
	}
	dMin	-=	fabs(dMin)*0.02;
	dMax	+=	fabs(dMax)*0.02;
	if( dMax - dMin < 0.03 )
		dMax	=	dMin + 0.05;
	if( pdMin )		*pdMin	=	dMin;
	if( pdMax )		*pdMax	=	dMax;

	return !bFirst;
}

DWORD	AfxGetMaxDays( CSPDWordArray & adw )
{
	DWORD	dwMax	=	0;
	for( int i=0; i<adw.GetSize(); i++ )
	{
		if( adw.ElementAt(i) > dwMax )
			dwMax	=	adw.ElementAt(i);
	}
	return dwMax;
}

//////////////////////////////////////////////////////////////////////
//	CTechnique

CTechnique::CTechnique( )
{
	m_pKData		=	NULL;

	m_pdCache1		=	NULL;
	m_pdCache2		=	NULL;
	m_pdCache3		=	NULL;
	m_pdCache4		=	NULL;
	m_pdCache5		=	NULL;
	m_pbHasCache1	=	NULL;
	m_pbHasCache2	=	NULL;
	m_pbHasCache3	=	NULL;
	m_pbHasCache4	=	NULL;
	m_pbHasCache5	=	NULL;
	m_nLength		=	0;
	m_nIndexStart	=	-1;

	m_nLastIntensity		=	ITS_NOTHING;
	m_nLastIntensityCode	=	ITSC_NOTHING;
}

CTechnique::CTechnique( CKData * pKData )
{
	m_pKData	=	pKData;

	m_pdCache1		=	NULL;
	m_pdCache2		=	NULL;
	m_pdCache3		=	NULL;
	m_pdCache4		=	NULL;
	m_pdCache5		=	NULL;
	m_pbHasCache1	=	NULL;
	m_pbHasCache2	=	NULL;
	m_pbHasCache3	=	NULL;
	m_pbHasCache4	=	NULL;
	m_pbHasCache5	=	NULL;
	m_nLength		=	0;
	m_nIndexStart	=	-1;

	m_nLastIntensity		=	ITS_NOTHING;
	m_nLastIntensityCode	=	ITSC_NOTHING;
}

CTechnique::~CTechnique()
{
	CTechnique::Clear( );
}

void CTechnique::SetKData( CKData * pKData )
{
	m_pKData	=	pKData;
	CTechnique::Clear( );
}

CKData * CTechnique::GetKData( )
{
	return m_pKData;
}

void CTechnique::Clear( )
{
	if( m_pdCache1 )
		delete	[]	m_pdCache1;
	if( m_pdCache2 )
		delete	[]	m_pdCache2;
	if( m_pdCache3 )
		delete	[]	m_pdCache3;
	if( m_pdCache4 )
		delete	[]	m_pdCache4;
	if( m_pdCache5 )
		delete	[]	m_pdCache5;

	if( m_pbHasCache1 )
		delete	[]	m_pbHasCache1;
	if( m_pbHasCache2 )
		delete	[]	m_pbHasCache2;
	if( m_pbHasCache3 )
		delete	[]	m_pbHasCache3;
	if( m_pbHasCache4 )
		delete	[]	m_pbHasCache4;
	if( m_pbHasCache5 )
		delete	[]	m_pbHasCache5;

	m_pdCache1		=	NULL;
	m_pdCache2		=	NULL;
	m_pdCache3		=	NULL;
	m_pdCache4		=	NULL;
	m_pdCache5		=	NULL;
	m_pbHasCache1	=	NULL;
	m_pbHasCache2	=	NULL;
	m_pbHasCache3	=	NULL;
	m_pbHasCache4	=	NULL;
	m_pbHasCache5	=	NULL;
	m_nLength		=	0;
	m_nIndexStart	=	-1;

	m_nLastIntensity		=	ITS_NOTHING;
	m_nLastIntensityCode	=	ITSC_NOTHING;
}

void CTechnique::ClearLastIntensity( )
{
	m_nLastIntensity		=	ITS_NOTHING;
	m_nLastIntensityCode	=	ITSC_NOTHING;
}

// 准备缓冲区
BOOL CTechnique::PrepareCache( int nStart, int nEnd, BOOL bForceRecalculate )
{
	SP_ASSERT( m_pKData );
	if( NULL == m_pKData )
		return FALSE;
	if( -1 == nEnd )
		nEnd	=	m_pKData->GetSize()-1;
	if( nStart < 0 || nEnd < nStart || nEnd >= m_pKData->GetSize() )
		return FALSE;

	if( bForceRecalculate )
		Clear( );

	return GetMinMaxInfo( nStart, nEnd, NULL, NULL );
}

// 从缓冲区中读取计算好的数据
BOOL CTechnique::LoadFromCache( int nIndex, double * pValue1, double * pValue2, double * pValue3, double * pValue4, double * pValue5 )
{
	if( m_nLength <= 0 || m_nIndexStart < 0
		|| nIndex < m_nIndexStart || nIndex-m_nIndexStart>=m_nLength )
		return FALSE;

	BOOL	bOK	=	TRUE;
	if( pValue1 && !( m_pdCache1 && m_pbHasCache1 && m_pbHasCache1[nIndex-m_nIndexStart] ) )
		bOK	=	FALSE;
	if( pValue2 && !( m_pdCache2 && m_pbHasCache2 && m_pbHasCache2[nIndex-m_nIndexStart] ) )
		bOK	=	FALSE;
	if( pValue3 && !( m_pdCache3 && m_pbHasCache3 && m_pbHasCache3[nIndex-m_nIndexStart] ) )
		bOK	=	FALSE;
	if( pValue4 && !( m_pdCache4 && m_pbHasCache4 && m_pbHasCache4[nIndex-m_nIndexStart] ) )
		bOK	=	FALSE;
	if( pValue5 && !( m_pdCache5 && m_pbHasCache5 && m_pbHasCache5[nIndex-m_nIndexStart] ) )
		bOK	=	FALSE;

	if( !bOK )
		return FALSE;

	if( pValue1 )
		*pValue1	=	m_pdCache1[nIndex-m_nIndexStart];
	if( pValue2 )
		*pValue2	=	m_pdCache2[nIndex-m_nIndexStart];
	if( pValue3 )
		*pValue3	=	m_pdCache3[nIndex-m_nIndexStart];
	if( pValue4 )
		*pValue4	=	m_pdCache4[nIndex-m_nIndexStart];
	if( pValue5 )
		*pValue5	=	m_pdCache5[nIndex-m_nIndexStart];
	return TRUE;
}

// 将计算好的数据保存至缓冲区
BOOL CTechnique::StoreToCache( int nIndex, double * pValue1, double *pValue2, double *pValue3, double * pValue4, double * pValue5 )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( m_nLength <= 0 || m_nIndexStart < 0 )
	{
		m_nLength		=	( NULL!=m_pKData ? m_pKData->GetSize() : 0 );
		if( m_nLength > 0 )
			m_nIndexStart	=	0;
	}
	if( m_nLength <= 0 || m_nIndexStart <  0 )
		return FALSE;
	
	// Realocate
	if( pValue1 )
	{
		if( !m_pdCache1 )
			m_pdCache1		=	new double[m_nLength];
		if( !m_pbHasCache1 )
		{
			m_pbHasCache1	=	new BOOL[m_nLength];
			if( !m_pbHasCache1 )	return FALSE;
			for( int i=0; i<m_nLength; i++ )
				m_pbHasCache1[i]	=	FALSE;
		}
		if( NULL == m_pbHasCache1 || NULL == m_pdCache1 )
			return FALSE;
	}
	if( pValue2 )
	{
		if( !m_pdCache2 )
			m_pdCache2		=	new double[m_nLength];
		if( !m_pbHasCache2 )
		{
			m_pbHasCache2	=	new BOOL[m_nLength];
			if( !m_pbHasCache2 )	return FALSE;
			for( int i=0; i<m_nLength; i++ )
				m_pbHasCache2[i]	=	FALSE;
		}
		if( NULL == m_pbHasCache2 || NULL == m_pdCache2 )
			return FALSE;
	}
	if( pValue3 )
	{
		if( !m_pdCache3 )
			m_pdCache3		=	new double[m_nLength];
		if( !m_pbHasCache3 )
		{
			m_pbHasCache3	=	new BOOL[m_nLength];
			if( !m_pbHasCache3 )	return FALSE;
			for( int i=0; i<m_nLength; i++ )
				m_pbHasCache3[i]	=	FALSE;
		}
		if( NULL == m_pbHasCache3 || NULL == m_pdCache3 )
			return FALSE;
	}
	if( pValue4 )
	{
		if( !m_pdCache4 )
			m_pdCache4		=	new double[m_nLength];
		if( !m_pbHasCache4 )
		{
			m_pbHasCache4	=	new BOOL[m_nLength];
			if( !m_pbHasCache4 )	return FALSE;
			for( int i=0; i<m_nLength; i++ )
				m_pbHasCache4[i]	=	FALSE;
		}
		if( NULL == m_pbHasCache4 || NULL == m_pdCache4 )
			return FALSE;
	}
	if( pValue5 )
	{
		if( !m_pdCache5 )
			m_pdCache5		=	new double[m_nLength];
		if( !m_pbHasCache5 )
		{
			m_pbHasCache5	=	new BOOL[m_nLength];
			if( !m_pbHasCache5 )	return FALSE;
			for( int i=0; i<m_nLength; i++ )
				m_pbHasCache5[i]	=	FALSE;
		}
		if( NULL == m_pbHasCache5 || NULL == m_pdCache5 )
			return FALSE;
	}

	if( nIndex < m_nIndexStart || nIndex-m_nIndexStart>=m_nLength )
		return FALSE;

	// Store	
	if( pValue1 && m_pdCache1 && m_pbHasCache1 )
	{
		m_pdCache1[nIndex-m_nIndexStart]		=	*pValue1;
		m_pbHasCache1[nIndex-m_nIndexStart]		=	TRUE;
	}
	if( pValue2 && m_pdCache2 && m_pbHasCache2 )
	{
		m_pdCache2[nIndex-m_nIndexStart]		=	*pValue2;
		m_pbHasCache2[nIndex-m_nIndexStart]		=	TRUE;
	}
	if( pValue3 && m_pdCache3 && m_pbHasCache3 )
	{
		m_pdCache3[nIndex-m_nIndexStart]		=	*pValue3;
		m_pbHasCache3[nIndex-m_nIndexStart]		=	TRUE;
	}
	if( pValue4 && m_pdCache4 && m_pbHasCache4 )
	{
		m_pdCache4[nIndex-m_nIndexStart]		=	*pValue4;
		m_pbHasCache4[nIndex-m_nIndexStart]		=	TRUE;
	}
	if( pValue5 && m_pdCache5 && m_pbHasCache5 )
	{
		m_pdCache5[nIndex-m_nIndexStart]		=	*pValue5;
		m_pbHasCache5[nIndex-m_nIndexStart]		=	TRUE;
	}
	return TRUE;
}

// 准备数据，得到较大值和较小值
BOOL CTechnique::IntensityPrepare( int nIndex, UINT * pnCode, int nMaxDays, int nMinMaxDays, double *pdLiminalLow, double *pdLiminalHigh, double dGateLow, double dGateHigh )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	if( nIndex < nMaxDays + nMinMaxDays )
		return FALSE;
	int	nStart	=	nIndex - nMinMaxDays + 1;
	if( nStart < 0 )
		return FALSE;

	double	dMin = 0, dMax = 0;
	if( !GetMinMaxInfo( nStart, nIndex, &dMin, &dMax ) )
		return FALSE;

	if( pdLiminalLow )	*pdLiminalLow	=	dMin + (dMax-dMin)*dGateLow;
	if( pdLiminalHigh )	*pdLiminalHigh	=	dMin + (dMax-dMin)*dGateHigh;
	return TRUE;
}

// 准备数据，得到价格较大值和较小值
BOOL CTechnique::IntensityPreparePrice( int nIndex, UINT * pnCode, int nMaxDays, int nMinMaxDays, double *pdLiminalLow, double *pdLiminalHigh, double dGateLow, double dGateHigh )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	if( nIndex < nMaxDays + nMinMaxDays )
		return FALSE;
	int	nStart	=	nIndex - nMinMaxDays + 1;
	if( nStart < 0 )
		return FALSE;

	float	fMin = 0, fMax = 0;
	if( !m_pKData || !m_pKData->GetMinMaxInfo( nStart, nIndex, &fMin, &fMax ) )
		return FALSE;

	if( pdLiminalLow )	*pdLiminalLow	=	(fMin + (fMax-fMin)*dGateLow);
	if( pdLiminalHigh )	*pdLiminalHigh	=	(fMin + (fMax-fMin)*dGateHigh);
	return TRUE;
}

// 是否创新高或者创新低
BOOL CTechnique::IsNewValue( int nIndex, BOOL bTopOrBottom, int nDays, double * pdValue1, double * pdValue2 )
{
	SP_ASSERT( nDays > 3 );
	if( nDays < 3 )
		return FALSE;
	SP_ASSERT( NULL != pdValue1 || NULL != pdValue2 );
	if( NULL == pdValue1 && NULL == pdValue2 )
		return FALSE;
	if( m_nLength <= 0 || m_nIndexStart < 0
		|| nIndex < m_nIndexStart || nIndex-m_nIndexStart>=m_nLength
		|| nIndex-m_nIndexStart < nDays-1 )
		return FALSE;

	BOOL	bFirst = TRUE;
	double	dLast1 = 0, dLast2 = 0;
	int		nLast1 = 0, nLast2 = 0;
	double	dNow1 = 0, dNow2 = 0;
	if( NULL != pdValue1 )
		dNow1	=	pdValue1[nIndex-m_nIndexStart];
	if( NULL != pdValue2 )
		dNow2	=	pdValue2[nIndex-m_nIndexStart];
	for( int k=nIndex-m_nIndexStart-1; k>=0 && k>nIndex-m_nIndexStart-nDays; k-- )
	{
		if( bFirst )
		{
			if( pdValue1 )	dLast1	=	pdValue1[k];
			if( pdValue2 )	dLast2	=	pdValue2[k];
			nLast1	=	nLast2	=	k;
			bFirst	=	FALSE;
		}

		if( bTopOrBottom )
		{
			if( pdValue1 && pdValue1[k] > dNow1 )
				return FALSE;
			if( pdValue2 && pdValue2[k] > dNow2 )
				return FALSE;

			if( pdValue1 && pdValue1[k] > dLast1 )
			{
				dLast1	=	pdValue1[k];
				nLast1	=	k;
			}
			if( pdValue2 && pdValue2[k] > dLast2 )
			{
				dLast2	=	pdValue2[k];
				nLast2	=	k;
			}
		}
		else
		{
			if( pdValue1 && pdValue1[k] < dNow1 )
				return FALSE;
			if( pdValue2 && pdValue2[k] < dNow2 )
				return FALSE;

			if( pdValue1 && pdValue1[k] < dLast1 )
			{
				dLast1	=	pdValue1[k];
				nLast1	=	k;
			}
			if( pdValue2 && pdValue2[k] < dLast2 )
			{
				dLast2	=	pdValue2[k];
				nLast2	=	k;
			}
		}
	}

	if( bFirst )
		return FALSE;

	if( ( !pdValue1 || (nLast1 >= nIndex-30 && nLast1 <= nIndex-5) )
		&& ( !pdValue2 || (nLast2 >= nIndex-30 && nLast2 <= nIndex-5) ) )
		return TRUE;

	return FALSE;
}

// 是否底背离
BOOL CTechnique::IsDeviateOnBottom( int nIndex, double * pdValue1, double * pdValue2 )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( !m_pKData->IsNewValue( nIndex, FALSE, ITS_DAYS_DEVIATE ) )
		return FALSE;

	if( IsNewValue( nIndex, FALSE, ITS_DAYS_DEVIATE, pdValue1, pdValue2 ) )
		return FALSE;

	return TRUE;
}

// 是否顶背离
BOOL CTechnique::IsDeviateOnTop( int nIndex, double * pdValue1, double * pdValue2 )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( !m_pKData->IsNewValue( nIndex, TRUE, ITS_DAYS_DEVIATE ) )
		return FALSE;

	if( IsNewValue( nIndex, TRUE, ITS_DAYS_DEVIATE, pdValue1, pdValue2 ) )
		return FALSE;

	return TRUE;
}

// 是否金叉
BOOL CTechnique::IsGoldenFork( int nIndex, double * pdValue1, double * pdValue2 )
{
	SP_ASSERT( NULL != pdValue1 && NULL != pdValue2 );
	if( NULL == pdValue1 || NULL == pdValue2 )
		return FALSE;
	if( m_nLength <= 0 || m_nIndexStart < 0
		|| nIndex < m_nIndexStart || nIndex-m_nIndexStart>=m_nLength
		|| nIndex-m_nIndexStart < 1 )
		return FALSE;

	int	k	=	nIndex-m_nIndexStart;
	if( pdValue1[k-1] <= pdValue2[k-1] && pdValue1[k] > pdValue2[k]
		&& pdValue1[k] >= pdValue1[k-1] && pdValue2[k] >= pdValue2[k-1] )
		return TRUE;
	return FALSE;
}

// 是否死叉
BOOL CTechnique::IsDeadFork( int nIndex, double * pdValue1, double * pdValue2 )
{
	SP_ASSERT( NULL != pdValue1 && NULL != pdValue2 );
	if( NULL == pdValue1 || NULL == pdValue2 )
		return FALSE;
	if( m_nLength <= 0 || m_nIndexStart < 0
		|| nIndex < m_nIndexStart || nIndex-m_nIndexStart>=m_nLength
		|| nIndex-m_nIndexStart < 1 )
		return FALSE;

	int	k	=	nIndex-m_nIndexStart;
	if( pdValue1[k-1] >= pdValue2[k-1] && pdValue1[k] < pdValue2[k]
		&& pdValue1[k] <= pdValue1[k-1] && pdValue2[k] <= pdValue2[k-1] )
		return TRUE;
	return FALSE;
}

// 得到金叉或者死叉信号
int CTechnique::GetForkSignal(int nIndex, CSPDWordArray & adwDays,
								 UINT itsGoldenFork, UINT itsDeadFork, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( nIndex <= 0 )
		return ITS_NOTHING;

	for( int k=1; k<adwDays.GetSize(); k++ )
	{
		double	dMANow1, dMANow2, dMALast1, dMALast2;
		if( !Calculate( &dMANow1, nIndex, min(adwDays[k-1],adwDays[k]), FALSE )
			|| !Calculate( &dMANow2, nIndex, max(adwDays[k-1],adwDays[k]), FALSE )
			|| !Calculate( &dMALast1, nIndex-1, min(adwDays[k-1],adwDays[k]), FALSE )
			|| !Calculate( &dMALast2, nIndex-1, max(adwDays[k-1],adwDays[k]), FALSE ) )
			return ITS_NOTHING;
		
		if( dMALast1 <= dMALast2 && dMANow1 > dMANow2
			&& dMANow1 >= dMALast1 && dMANow2 >= dMALast2 )
		{
			if( adwDays.GetSize()-1 == k )
			{
				if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
				return itsGoldenFork;
			}
			for( int l=k+1; l<adwDays.GetSize(); l++ )
			{
				double	dMANow3, dMALast3;
				if( !Calculate( &dMANow3, nIndex, adwDays[l], FALSE )
					|| !Calculate( &dMALast3, nIndex-1, adwDays[l], FALSE ) )
					return ITS_NOTHING;
				if( dMANow3 >= dMALast3 )
				{
					if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
					return itsGoldenFork;
				}
			}
		}
		if( dMALast1 >= dMALast2 && dMANow1 < dMANow2
			&& dMANow1 <= dMALast1 && dMANow2 <= dMALast2 )
		{
			if( adwDays.GetSize()-1 == k )
			{
				if( pnCode )	*pnCode	=	ITSC_DEADFORK;
				return itsDeadFork;
			}
			for( int l=k+1; l<adwDays.GetSize(); l++ )
			{
				double	dMANow3, dMALast3;
				if( !Calculate( &dMANow3, nIndex, adwDays[l], FALSE )
					|| !Calculate( &dMALast3, nIndex-1, adwDays[l], FALSE ) )
					return ITS_NOTHING;
				if( dMANow3 <= dMALast3 )
				{
					if( pnCode )	*pnCode	=	ITSC_DEADFORK;
					return itsDeadFork;
				}
			}
		}
	}
	return ITS_NOTHING;
}

// 得到金叉或者死叉信号
int CTechnique::GetForkSignal( int nIndex, UINT itsGoldenFork, UINT itsDeadFork, UINT *pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( nIndex <= 0 )
		return ITS_NOTHING;

	double dLast1 = 0, dLast2 = 0, dNow1 = 0, dNow2 = 0;
	if( !Calculate( &dLast1, &dLast2, nIndex-1, FALSE )
		|| !Calculate( &dNow1, &dNow2, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dLast1 <= dLast2 && dNow1 > dNow2
		&& dNow1 >= dLast1 && dNow2 >= dLast2 )
	{
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return itsGoldenFork;
	}
	if( dLast1 >= dLast2 && dNow1 < dNow2
		&& dNow1 <= dLast1 && dNow2 <= dLast2 )
	{
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return itsDeadFork;
	}

	return ITS_NOTHING;
}

// 得到金叉或者死叉信号
int CTechnique::GetForkSignal(int nIndex, double * pdValue1, double * pdValue2,
							  UINT itsGoldenFork, UINT itsDeadFork, UINT *pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	if( IsGoldenFork( nIndex, pdValue1, pdValue2 ) )
	{
		if( pnCode )	*pnCode	=	ITSC_GOLDENFORK;
		return itsGoldenFork;
	}
	if( IsDeadFork( nIndex, pdValue1, pdValue2 ) )
	{
		if( pnCode )	*pnCode	=	ITSC_DEADFORK;
		return itsDeadFork;
	}
	return ITS_NOTHING;
}

// 得到趋势信号
int CTechnique::GetTrendIntensity(int nIndex, CSPDWordArray & adwDays,
								 UINT itsLong, UINT itsShort, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( nIndex <= 0 )
		return ITS_NOTHING;

	int	nRet	=	ITS_NOTHING;
	for( int k=1; k<adwDays.GetSize(); k++ )
	{
		double	dMALast1, dMALast2, dMANow1, dMANow2;
		if( !Calculate( &dMALast1, nIndex-1, min(adwDays[k-1],adwDays[k]), FALSE )
			|| !Calculate( &dMALast2, nIndex-1, max(adwDays[k-1],adwDays[k]), FALSE )
			|| !Calculate( &dMANow1, nIndex, min(adwDays[k-1],adwDays[k]), FALSE )
			|| !Calculate( &dMANow2, nIndex, max(adwDays[k-1],adwDays[k]), FALSE ) )
			return ITS_NOTHING;
		
		if( dMANow1 >= dMALast1 && dMANow2 >= dMALast2
			&& dMANow1 > dMANow2 && (dMANow1-dMANow2)>=(dMALast1-dMALast2)
			&& (ITS_ISBUY(nRet) || 1==k) )
		{
			if( pnCode )	*pnCode	=	ITSC_LONG;
			nRet	=	itsLong;
		}
		else if( dMANow1 <= dMALast1 && dMANow2 <= dMALast2
			&& dMANow1 < dMANow2 && (dMANow1-dMANow2)<=(dMALast1-dMALast2)
			&& (ITS_ISSELL(nRet) || 1==k) )
		{
			if( pnCode )	*pnCode	=	ITSC_SHORT;
			nRet	=	itsShort;
		}
		else
		{
			if( pnCode )	*pnCode	=	ITSC_NOTHING;
			return ITS_NOTHING;
		}
	}
	return nRet;
}

// 得到趋势信号
int CTechnique::GetTrendIntensity1( int nIndex, UINT itsLong, UINT itsShort, UINT *pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( nIndex <= 0 )
		return ITS_NOTHING;

	double	dLast = 0, dNow = 0;
	if( !Calculate( &dLast, nIndex-1, FALSE )
		|| !Calculate( &dNow, nIndex, FALSE ) )
		return ITS_NOTHING;

	if( dNow > dLast )
	{
		if( pnCode )	*pnCode	=	ITSC_LONG;
		return itsLong;
	}
	if( dNow < dLast )
	{
		if( pnCode )	*pnCode	=	ITSC_SHORT;
		return itsShort;
	}
	return ITS_NOTHING;
}

// 得到趋势信号
int CTechnique::GetTrendIntensity2( int nIndex, UINT itsLong, UINT itsShort, UINT *pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	if( nIndex <= 0 )
		return ITS_NOTHING;

	double	dLast1 = 0, dLast2 = 0, dNow1 = 0, dNow2 = 0;
	if( Calculate( &dLast1, &dLast2, nIndex-1, FALSE )
		&& Calculate( &dNow1, &dNow2, nIndex, FALSE ) )
	{
		if( dNow1 >= dLast1 && dNow2 >= dLast2
			&& dNow1 > dNow2 && (dNow1-dNow2)>=(dLast1-dLast2) )
		{
			if( pnCode )	*pnCode	=	ITSC_LONG;
			return itsLong;
		}
		if( dNow1 <= dLast1 && dNow2 <= dLast2
			&& dNow1 < dNow2 && (dNow1-dNow2)<=(dLast1-dLast2) )
		{
			if( pnCode )	*pnCode	=	ITSC_SHORT;
			return itsShort;
		}
	}
	return ITS_NOTHING;
}

// 得到趋势信号
int CTechnique::GetTrendIntensity(int nIndex, double * pdValue1, double * pdValue2,
							  UINT itsLong, UINT itsShort, UINT *pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	SP_ASSERT( NULL != pdValue1 && NULL != pdValue2 );
	if( NULL == pdValue1 || NULL == pdValue2 )
		return FALSE;
	if( m_nLength <= 0 || m_nIndexStart < 0
		|| nIndex < m_nIndexStart || nIndex-m_nIndexStart>=m_nLength
		|| nIndex-m_nIndexStart < 1 )
		return FALSE;

	int	k	=	nIndex-m_nIndexStart;
	if( pdValue1[k] >= pdValue1[k-1] && pdValue2[k] >= pdValue2[k-1]
		&& pdValue1[k] > pdValue2[k] && (pdValue1[k]-pdValue2[k])>=(pdValue1[k-1]-pdValue2[k-1]) )
	{
		if( pnCode )	*pnCode	=	ITSC_LONG;
		return itsLong;
	}
	if( pdValue1[k] <= pdValue1[k-1] && pdValue2[k] <= pdValue2[k-1]
		&& pdValue1[k] < pdValue2[k] && (pdValue1[k]-pdValue2[k])<=(pdValue1[k-1]-pdValue2[k-1]) )
	{
		if( pnCode )	*pnCode	=	ITSC_SHORT;
		return itsShort;
	}
	return ITS_NOTHING;
}

void CTechnique::SetDefaultParameters( )
{
	SP_ASSERT( FALSE );
}

void CTechnique::AttachParameters( CTechnique & src )
{
	SP_ASSERT( FALSE );
}

BOOL CTechnique::IsValidParameters( )
{
	SP_ASSERT( FALSE );
	return FALSE;
}

// 创建新的技术指标对象
CTechnique * CTechnique::CreateTechnique( UINT nTech, CKData * pKData )
{
	CTechnique	*	pTech	=	NULL;
	switch( nTech )
	{
		case STT_MA:		pTech	=	new CMA( pKData );		break;
		case STT_BBI:		pTech	=	new CBBI( pKData );		break;
		case STT_BOLL:		pTech	=	new CBOLL( pKData );	break;
		case STT_PV:		pTech	=	new CPV( pKData );		break;
		case STT_SAR:		pTech	=	new CSAR( pKData );		break;
		case STT_DJ:		pTech	=	new CDJ( pKData );		break;
		case STT_CW:		pTech	=	new CCW( pKData );		break;
		
		case STT_MACD:		pTech	=	new CMACD( pKData );	break;
		case STT_MIKE:		pTech	=	new CMIKE( pKData );	break;
		case STT_PSY:		pTech	=	new CPSY( pKData );		break;

		case STT_VOLUME:	pTech	=	new CVOLUME( pKData );	break;
		case STT_NVI:		pTech	=	new CNVI( pKData );		break;
		case STT_PVI:		pTech	=	new CPVI( pKData );		break;
		case STT_VR:		pTech	=	new CVR( pKData );		break;
		case STT_VROC:		pTech	=	new CVROC( pKData );	break;
		case STT_OBV:		pTech	=	new COBV( pKData );		break;
		case STT_MOBV:		pTech	=	new CMOBV( pKData );	break;
		case STT_MFI:		pTech	=	new CMFI( pKData );		break;
		case STT_VMACD:		pTech	=	new CVMACD( pKData );	break;
		case STT_WVAD:		pTech	=	new CWVAD( pKData );	break;
		case STT_EMV:		pTech	=	new CEMV( pKData );		break;
		case STT_VRSI:		pTech	=	new CVRSI( pKData );	break;
		case STT_NVRSI:		pTech	=	new CNVRSI( pKData );	break;
		case STT_AD:		pTech	=	new CAD( pKData );		break;
		case STT_CI:		pTech	=	new CCI( pKData );		break;

		case STT_KDJ:		pTech	=	new CKDJ( pKData );		break;
		case STT_R:			pTech	=	new CR( pKData );		break;
		case STT_RSI:		pTech	=	new CRSI( pKData );		break;
		case STT_BIAS:		pTech	=	new CBIAS( pKData );	break;
		case STT_MTM:		pTech	=	new CMTM( pKData );		break;
		case STT_DMI:		pTech	=	new CDMI( pKData );		break;
		case STT_ROC:		pTech	=	new CROC( pKData );		break;
		case STT_CCI:		pTech	=	new CCCI( pKData );		break;
		case STT_CV:		pTech	=	new CCV( pKData );		break;
		case STT_ARBR:		pTech	=	new CARBR( pKData );	break;
		case STT_CR:		pTech	=	new CCR( pKData );		break;
		case STT_OSC:		pTech	=	new COSC( pKData );		break;
		case STT_UOS:		pTech	=	new CUOS( pKData );		break;
		case STT_MAOSC:		pTech	=	new CMAOSC( pKData );	break;
		case STT_36BIAS:	pTech	=	new C36BIAS( pKData );	break;
		case STT_DPO:		pTech	=	new CDPO( pKData );		break;
		case STT_KST:		pTech	=	new CKST( pKData );		break;

		case STT_REI:		pTech	=	new CREI( pKData );		break;
		case STT_DMKI:		pTech	=	new CDMKI( pKData );	break;
		case STT_PCNT:		pTech	=	new CPCNT( pKData );	break;
		case STT_HLC:		pTech	=	new CHLC( pKData );		break;
		case STT_CDP:		pTech	=	new CCDP( pKData );		break;
		case STT_ASI:		pTech	=	new CASI( pKData );		break;
		case STT_ATR:		pTech	=	new CATR( pKData );		break;

		case STT_CYO:		pTech	=	new CCYO( pKData );		break;
		case STT_DCYO:		pTech	=	new CDCYO( pKData );	break;
		case STT_HSL:		pTech	=	new CHSL( pKData );		break;
		case STT_DPER:		pTech	=	new CDPER( pKData );	break;

		default:
			SP_ASSERT( FALSE );
	}
	return pTech;
}

// 得到买卖信号
int CTechnique::GetSignal( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;
	return	ITS_NOTHING;
}

// 得到趋势信号
int CTechnique::GetIntensity( int nIndex, UINT * pnCode, BOOL bTraceBack )
{
	UINT	nCode	=	ITSC_NOTHING;
	int	nSignal	=	GetSignal( nIndex, &nCode );
	if( ITS_NOTHING != nSignal )
	{
		m_nLastIntensity		=	nSignal;
		m_nLastIntensityCode	=	nCode;
		if( pnCode )	*pnCode	=	nCode;
		return nSignal;
	}

	if( m_nLastIntensity > 1 )
		m_nLastIntensity	--;
	if( m_nLastIntensity < -1  )
		m_nLastIntensity	++;

	if( bTraceBack && ITS_NOTHING == m_nLastIntensity )
		m_nLastIntensity		=	GetIntensityTraceback( nIndex, &m_nLastIntensityCode );

	if( pnCode )	*pnCode	=	m_nLastIntensityCode;
	return m_nLastIntensity;
}

int CTechnique::GetIntensityTraceback( int nIndex, UINT * pnCode )
{
	if( pnCode )	*pnCode	=	ITSC_NOTHING;

	int		nIntensity	=	ITS_NOTHING;
	UINT	nCode		=	ITSC_NOTHING;
	int k;
	for( k=nIndex; k>=0; k -- )
	{
		nIntensity	=	GetSignal(k,&nCode);
		if( ITS_NOTHING != nIntensity )
			break;
	}
	if( k < 0 )
		return ITS_NOTHING;
	for( k=k+1; k<=nIndex; k++ )
	{
		if( nIntensity > 1 )
			nIntensity	--;
		else if( nIntensity < -1  )
			nIntensity	++;
		else
			break;
	}
	if( pnCode )	*pnCode	=	nCode;
	return nIntensity;
}

BOOL CTechnique::GetMinMaxInfo( int nStart, int nEnd, double *pdMin, double *pdMax )
{
	SP_ASSERT( FALSE );
	return FALSE;
}

BOOL CTechnique::Calculate( double * pValue, int nIndex, int nDays, BOOL bUseLast )
{
	SP_ASSERT( FALSE );
	return FALSE;
}

BOOL CTechnique::Calculate( double * pValue, int nIndex, BOOL bUseLast )
{
	SP_ASSERT( FALSE );
	return FALSE;
}

BOOL CTechnique::Calculate( double * pValue1, double * pValue2, int nIndex, BOOL bUseLast )
{
	SP_ASSERT( FALSE );
	return FALSE;
}

BOOL CTechnique::Calculate( double * pValue1, double * pValue2, double * pValue3, int nIndex, BOOL bUseLast )
{
	SP_ASSERT( FALSE );
	return FALSE;
}

BOOL CTechnique::Calculate( double * pValue1, double * pValue2, double * pValue3, double *pValue4, int nIndex, BOOL bUseLast )
{
	SP_ASSERT( FALSE );
	return FALSE;
}

BOOL CTechnique::Calculate( double * pValue1, double * pValue2, double * pValue3, double *pValue4, double *pValue5, int nIndex, BOOL bUseLast )
{
	SP_ASSERT( FALSE );
	return FALSE;
}

// 计算指标值和均值
BOOL CTechnique::CalculateMA( double * pValue, double * pMA, int nIndex, BOOL bUseLast, int nMADays )
{
	STT_ASSERT_CALCULATE1( m_pKData, nIndex );

	if( nMADays > nIndex+1 )
		return FALSE;

	if( LoadFromCache( nIndex, pValue, pMA ) )
		return TRUE;

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
			if( nCount == nMADays )
			{
				dMA	=	dMA / nMADays;
				if( pValue )	*pValue	=	dValue;
				if( pMA )		*pMA	=	dMA;
				StoreToCache( nIndex, pValue, pMA );
				return TRUE;
			}
		}
	}
	return FALSE;
}
