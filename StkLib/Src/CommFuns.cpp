// strings.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include	"StdAfx.h"
#include	"../Include/CommFuns.h"
#include	"math.h"

BOOL CalculateYieldYear( CKData & kday, double * paverage, double * pstddev, long ldays, int nDayCurrent )
{
	if( -1 != nDayCurrent && nDayCurrent >= kday.GetSize() )
		return FALSE;

	int	nDayCount		=	0;
	double	yieldSum = 0., yieldSquareSum = 0.;
	if( -1 == nDayCurrent )
		nDayCurrent	=	kday.GetSize()-1;
	int	nDayEnd = ( -1 == ldays ? 0 : nDayCurrent-ldays );
	if( nDayEnd - STKLIB_DAYS_INONEYEAR < 0 )
		return FALSE;
	for( int nDay = nDayCurrent; nDay >= nDayEnd; nDay -- )
	{
		int	nDayLast	=	nDay - STKLIB_DAYS_INONEYEAR;
		if( nDayLast < 0 )
			break;

		if( kday.MaindataAt(nDayLast) > 1e-4 )
		{
			nDayCount	++;
			double	yieldNow	=	(double( kday.MaindataAt(nDay) - kday.MaindataAt(nDayLast) )) / kday.MaindataAt(nDayLast);
			yieldSum	+=	yieldNow;
			yieldSquareSum	+=	yieldNow*yieldNow;
		}
	}
	if( nDayCount > 0 &&
		nDayCount >= (-1 == ldays ? STKLIB_DAYS_INONEYEAR : ldays-STKLIB_DAYS_INONEYEAR) )
	{
		double	average	=	yieldSum / nDayCount;
		if( kday.MaindataAt(nDayEnd) > 1e-4 )	// HERE!!!, reset average directly!!!
		{
			average	= (double)kday.MaindataAt(nDayCurrent) / kday.MaindataAt(nDayEnd);
			double dYearCount = ((double)nDayCount) / STKLIB_DAYS_INONEYEAR;
			average = pow( (double)10, (double)log10(fabs(average))/dYearCount ) - 1;
		}

		if( paverage )
			*paverage	=	average;
		if( pstddev )
		{
			*pstddev		=	yieldSquareSum / nDayCount - ((yieldSum / nDayCount)*(yieldSum / nDayCount));
			if( *pstddev > 0 )
				*pstddev	=	sqrt( *pstddev );
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CalculateYieldDay( CKData & kday, double * paverage, double * pstddev, long ldays, int nDayCurrent )
{
	if( -1 != nDayCurrent && nDayCurrent >= kday.GetSize() )
		return FALSE;

	int	nDayCount		=	0;
	double	yieldSum = 0., yieldSquareSum = 0.;
	if( -1 == nDayCurrent )
		nDayCurrent	=	kday.GetSize()-1;
	int	nDayEnd = ( -1 == ldays ? 0 : nDayCurrent-ldays );
	if( nDayEnd - 1 < 0 )
		return FALSE;
	for( int nDay = nDayCurrent; nDay >= nDayEnd; nDay -- )
	{
		int	nDayLast	=	nDay - 1;
		if( nDayLast < 0 )
			break;

		if( kday.MaindataAt(nDayLast) > 1e-4 )
		{
			nDayCount	++;
			double	yieldNow	=	(double( kday.MaindataAt(nDay) - kday.MaindataAt(nDayLast) )) / kday.MaindataAt(nDayLast);
			yieldSum	+=	yieldNow;
			yieldSquareSum	+=	yieldNow*yieldNow;
		}
	}
	if( nDayCount > 0 &&
		nDayCount >= (-1 == ldays ? STKLIB_DAYS_INONEYEAR : ldays) )
	{
		double	average	=	yieldSum / nDayCount;
		if( kday.MaindataAt(nDayEnd) > 1e-4 )	// HERE!!!, reset average directly!!!
		{
			average	= (double)kday.MaindataAt(nDayCurrent) / kday.MaindataAt(nDayEnd);
			average = pow( (double)10, (double)log10(fabs(average))/nDayCount ) - 1;
		}

		if( paverage )
			*paverage	=	average;
		if( pstddev )
		{
			*pstddev		=	yieldSquareSum / nDayCount - ((yieldSum / nDayCount)*(yieldSum / nDayCount));
			if( *pstddev > 0 )
				*pstddev	=	sqrt( *pstddev );
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CalculateBeiteYear( CKData & kday, CKData &kdayMain,
					double market_yield_average, double market_yield_d,
					double	* pbeite, long ldays, int nDayCurrent )
{
	if( -1 != nDayCurrent && nDayCurrent >= kday.GetSize() )
		return FALSE;
	if( kday.GetSize() == 0 || kdayMain.GetSize() == 0 )
		return FALSE;

	double	yield_average = 0., yield_d = 0.;
	if( !CalculateYieldYear( kday, &yield_average, &yield_d, ldays, nDayCurrent ) )
		return FALSE;

	int	nDayCount		=	0;
	double	sum	=	0.;

	// kday Index
	if( -1 == nDayCurrent )
		nDayCurrent	=	kday.GetSize()-1;
	int	nDayEnd = ( -1 == ldays ? 0 : nDayCurrent-ldays );
	if( nDayEnd - STKLIB_DAYS_INONEYEAR < 0 )
		return FALSE;

	// kdayMain Index
	int	nDayCurrentMain	=	kdayMain.GetSize()-(kday.GetSize()-nDayCurrent);
	if( nDayCurrentMain < 0 || nDayCurrentMain >= kdayMain.GetSize() )
		return FALSE;
	if( kday.ElementAt(nDayCurrent).m_date != kdayMain.ElementAt(nDayCurrentMain).m_date )
	{
		int k;
		for( k=kdayMain.GetSize()-1; k>=0 ; k-- )
		{
			if( kday.ElementAt(nDayCurrent).m_date == kdayMain.ElementAt(k).m_date )
			{
				nDayCurrentMain	=	k;
				break;
			}
		}
		if( k < 0 )
			return FALSE;
	}
	int	nDayEndMain = ( -1 == ldays ? 0 : nDayCurrentMain-ldays );
	if( nDayEndMain < 0 )
		return FALSE;

	// Begin Calculate
	int	nCountAdjust	=	0;
	for( int nDay = nDayCurrent; nDay >= nDayEnd ; nDay -- )
	{
		int	nDayLast	=	nDay - STKLIB_DAYS_INONEYEAR;
		int	nDayMain	=	nDayCurrentMain-(nDayCurrent-nDay);
		int nDayMainLast	= nDayMain - STKLIB_DAYS_INONEYEAR;
		if( nDayMainLast < 0 || nDayMain >= kdayMain.GetSize() || nDayLast < 0 || nDay >= kday.GetSize() )
			break;

		// SP_ASSERT( kday.ElementAt(nDay).m_date == kdayMain.ElementAt(nDayMain).m_date );
		if( kday.ElementAt(nDay).m_date < kdayMain.ElementAt(nDayMain).m_date )
		{
			nCountAdjust	++;
			if( nCountAdjust >= 5 )
				break;
			nDayCurrentMain --;
			continue;
		}
		if( kday.ElementAt(nDay).m_date > kdayMain.ElementAt(nDayMain).m_date )
		{
			nCountAdjust	++;
			if( nCountAdjust >= 5 )
				break;
			nDayCurrentMain ++;
			continue;
		}

		if( kday.MaindataAt(nDayLast) > 1e-4 && kdayMain.MaindataAt(nDayMainLast) > 1e-4 )
		{
			nDayCount	++;
			double	yieldNow	=	(double( kday.MaindataAt(nDay) - kday.MaindataAt(nDayLast) )) / kday.MaindataAt(nDayLast);
			double	yieldNowMain=	(double( kdayMain.MaindataAt(nDayMain) - kdayMain.MaindataAt(nDayMainLast) )) / kdayMain.MaindataAt(nDayMainLast);
			sum	+=	(yieldNow-yield_average) * (yieldNowMain-market_yield_average);
		}
	}
	if( nDayCount > 0 &&
		nDayCount+nCountAdjust >= (-1 == ldays ? STKLIB_DAYS_INONEYEAR : ldays-STKLIB_DAYS_INONEYEAR) )
	{
		if( pbeite )
			*pbeite	=	( sum / nDayCount) / (market_yield_d*market_yield_d);
		return TRUE;
	}

	return	FALSE;
}

BOOL CalculateBeiteDay( CKData & kday, CKData &kdayMain,
					double market_yield_average, double market_yield_d,
					double	* pbeite, long ldays, int nDayCurrent )
{
	if( -1 != nDayCurrent && nDayCurrent >= kday.GetSize() )
		return FALSE;
	if( kday.GetSize() == 0 || kdayMain.GetSize() == 0 )
		return FALSE;

	double	yield_average = 0., yield_d = 0.;
	if( !CalculateYieldDay( kday, &yield_average, &yield_d, ldays, nDayCurrent ) )
		return FALSE;

	int	nDayCount	=	0;
	double	sum	=	0.;

	// kday Index
	if( -1 == nDayCurrent )
		nDayCurrent	=	kday.GetSize()-1;
	int	nDayEnd = ( -1 == ldays ? 0 : nDayCurrent-ldays );
	if( nDayEnd -1 < 0 )
		return FALSE;

	// kdayMain Index
	int	nDayCurrentMain	=	kdayMain.GetSize()-(kday.GetSize()-nDayCurrent);
	if( nDayCurrentMain < 0 || nDayCurrentMain >= kdayMain.GetSize() )
		return FALSE;
	if( kday.ElementAt(nDayCurrent).m_date != kdayMain.ElementAt(nDayCurrentMain).m_date )
	{
		int k;
		for( k=kdayMain.GetSize()-1; k>=0 ; k-- )
		{
			if( kday.ElementAt(nDayCurrent).m_date == kdayMain.ElementAt(k).m_date )
			{
				nDayCurrentMain	=	k;
				break;
			}
		}
		if( k < 0 )
			return FALSE;
	}
	int	nDayEndMain = ( -1 == ldays ? 0 : nDayCurrentMain-ldays );
	if( nDayEndMain < 0 )
		return FALSE;

	// Begin Calculate
	int	nCountAdjust	=	0;
	for( int nDay = nDayCurrent; nDay >= nDayEnd; nDay -- )
	{
		int	nDayLast	=	nDay - 1;
		int	nDayMain	=	nDayCurrentMain-(nDayCurrent-nDay);
		int nDayMainLast	= nDayMain-1;
		if( nDayMainLast < 0 || nDayMain >= kdayMain.GetSize() || nDayLast < 0 || nDay >= kday.GetSize() )
			break;

		// SP_ASSERT( kday.ElementAt(nDay).m_date == kdayMain.ElementAt(nDayMain).m_date );
		if( kday.ElementAt(nDay).m_date < kdayMain.ElementAt(nDayMain).m_date )
		{
			nCountAdjust	++;
			if( nCountAdjust >= 5 )
				break;
			nDayCurrentMain --;
			continue;
		}
		if( kday.ElementAt(nDay).m_date > kdayMain.ElementAt(nDayMain).m_date )
		{
			nCountAdjust	++;
			if( nCountAdjust >= 5 )
				break;
			nDayCurrentMain ++;
			continue;
		}

		if( kday.MaindataAt(nDayLast) > 1e-4 && kdayMain.MaindataAt(nDayMainLast) > 1e-4 )
		{
			nDayCount	++;
			double	yieldNow	=	(double( kday.MaindataAt(nDay) - kday.MaindataAt(nDayLast) )) / kday.MaindataAt(nDayLast);
			double	yieldNowMain=	(double( kdayMain.MaindataAt(nDayMain) - kdayMain.MaindataAt(nDayMainLast) )) / kdayMain.MaindataAt(nDayMainLast);
			sum	+=	(yieldNow-yield_average) * (yieldNowMain-market_yield_average);
		}
	}
	if( nDayCount > 0 &&
		nDayCount+nCountAdjust >= (-1 == ldays ? STKLIB_DAYS_INONEYEAR : ldays) )
	{
		if( pbeite )
			*pbeite	=	( sum / nDayCount) / (market_yield_d*market_yield_d);
		return TRUE;
	}

	return	FALSE;
}


