/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
*/

#ifndef	__STKLIB_COMMFUNS_H__
#define	__STKLIB_COMMFUNS_H__

/***
	计算平均收益率和β值的函数接口。
*/
BOOL CalculateYieldYear( CKData & kday, double * paverage, double * pstddev, long ldays, int nDayCurrent = -1 );
BOOL CalculateYieldDay( CKData & kday, double * paverage, double * pstddev, long ldays, int nDayCurrent = -1 );

BOOL CalculateBeiteYear( CKData & kday, CKData &kdayMain, double market_yield_average, double market_yield_d,
					double	* pbeite, long ldays, int nDayCurrent = -1 );
BOOL CalculateBeiteDay( CKData & kday, CKData &kdayMain, double market_yield_average, double market_yield_d,
					double	* pbeite, long ldays, int nDayCurrent = -1 );

#endif	//	__STKLIB_COMMFUNDS_H__
