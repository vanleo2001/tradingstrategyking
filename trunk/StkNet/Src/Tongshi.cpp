/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Tongshi interface convertor functions;
*/

#include	"StdAfx.h"
#include	"Stockdrv.h"
#include	"Tongshi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Convertor

// 股票市场
DWORD	TSMarket_to_Market( WORD wMarket )
{
	DWORD	dwMarket	=	CStock::marketCHNA;
	
	if( SH_MARKET_EX == wMarket )
		dwMarket	=	CStock::marketSHSE;
	else if( SZ_MARKET_EX == wMarket )
		dwMarket	=	CStock::marketSZSE;
	return dwMarket;
}

WORD	Market_to_TSMarket( DWORD dwMarket )
{
	WORD	wMarket	=	SH_MARKET_EX;
	
	if( CStock::marketSHSE == dwMarket )
		wMarket	=	SH_MARKET_EX;
	else if( CStock::marketSZSE == dwMarket )
		wMarket	=	SZ_MARKET_EX;
	return wMarket;
}

int		DataType_to_TSDataType( int nDataType )
{
	int nDataTypeTS = -1;
	switch( nDataType )
	{
	case CStock::dataInfo:
	case CStock::dataReport:
		nDataTypeTS = RCV_REPORT;
		break;
	case CStock::dataK:
		nDataTypeTS = FILE_HISTORY_EX;
		break;
	case CStock::dataDR:
		nDataTypeTS = FILE_POWER_EX;
		break;
	case CStock::dataBasetable:
		break;
	case CStock::dataBasetext:
		nDataTypeTS = FILE_BASE_EX;
		break;
	case CStock::dataNews:
		nDataTypeTS = FILE_NEWS_EX;
		break;
	case CStock::dataMinute:
		nDataTypeTS = FILE_MINUTE_EX;
		break;
	case CStock::dataOutline:
		break;
	case CStock::dataMultisort:
		nDataTypeTS = FILE_MULTISORT_EX;
		break;
	case CStock::dataDetail:
		nDataTypeTS = FILE_DETAIL_EX;
		break;
	default:;
	}
	return nDataTypeTS;
}

// 通视RCV_REPORT_STRUCTEx 转换为 标准 REPORT
BOOL convert_TSREPORT_to_REPORT( RCV_REPORT_STRUCTEx *pTSReport, REPORT * pReport )
{
	SP_ASSERT( pTSReport && pReport );
	if( NULL == pTSReport || NULL == pReport )
		return FALSE;

	memset( pReport, 0, sizeof(REPORT) );

	// 股票市场
	pReport->m_dwMarket	=	TSMarket_to_Market( pTSReport->m_wMarket );

	strncpy( pReport->m_szCode, pTSReport->m_szLabel, min(sizeof(pReport->m_szCode)-1,sizeof(pTSReport->m_szLabel)) );
	strncpy( pReport->m_szName, pTSReport->m_szName, min(sizeof(pReport->m_szName)-1,sizeof(pTSReport->m_szName)) );

	pReport->m_time		=	pTSReport->m_time;
	pReport->m_fLast	=	pTSReport->m_fLastClose;
	pReport->m_fOpen	=	pTSReport->m_fOpen;
	pReport->m_fHigh	=	pTSReport->m_fHigh;
	pReport->m_fLow		=	pTSReport->m_fLow;
	pReport->m_fNew		=	pTSReport->m_fNewPrice;
	pReport->m_fVolume	=	pTSReport->m_fVolume * 100;
	pReport->m_fAmount	=	pTSReport->m_fAmount;

	pReport->m_fBuyPrice[0]		=	pTSReport->m_fBuyPrice[0];
	pReport->m_fBuyPrice[1]		=	pTSReport->m_fBuyPrice[1];
	pReport->m_fBuyPrice[2]		=	pTSReport->m_fBuyPrice[2];
	
	pReport->m_fBuyVolume[0]	=	pTSReport->m_fBuyVolume[0] * 100;
	pReport->m_fBuyVolume[1]	=	pTSReport->m_fBuyVolume[1] * 100;
	pReport->m_fBuyVolume[2]	=	pTSReport->m_fBuyVolume[2] * 100;
	
	pReport->m_fSellPrice[0]	=	pTSReport->m_fSellPrice[0];
	pReport->m_fSellPrice[1]	=	pTSReport->m_fSellPrice[1];
	pReport->m_fSellPrice[2]	=	pTSReport->m_fSellPrice[2];

	pReport->m_fSellVolume[0]	=	pTSReport->m_fSellVolume[0] * 100;
	pReport->m_fSellVolume[1]	=	pTSReport->m_fSellVolume[1] * 100;
	pReport->m_fSellVolume[2]	=	pTSReport->m_fSellVolume[2] * 100;

	if( pTSReport->m_cbSize >= sizeof(RCV_REPORT_STRUCTEx) )
	{
		pReport->m_fBuyPrice[3]		=	pTSReport->m_fBuyPrice4;
		pReport->m_fBuyVolume[3]	=	pTSReport->m_fBuyVolume4 * 100;
		pReport->m_fSellPrice[3]	=	pTSReport->m_fSellPrice4;
		pReport->m_fSellVolume[3]	=	pTSReport->m_fSellVolume4 * 100;

		pReport->m_fBuyPrice[4]		=	pTSReport->m_fBuyPrice5;
		pReport->m_fBuyVolume[4]	=	pTSReport->m_fBuyVolume5 * 100;
		pReport->m_fSellPrice[4]	=	pTSReport->m_fSellPrice5;
		pReport->m_fSellVolume[4]	=	pTSReport->m_fSellVolume5 * 100;
	}

	return TRUE;
}

BOOL convert_TSHISTORY_to_KDATA( DWORD dwMarket, LPCTSTR lpszCode, RCV_HISTORY_STRUCTEx * pHistory, KDATA * pkd )
{
	SP_ASSERT( pHistory && pkd );
	if( NULL == pHistory || NULL == pkd )
		return FALSE;

	memset( pkd, 0, sizeof(KDATA) );

	pkd->m_dwMarket	=	dwMarket;
	if( NULL != lpszCode )
		strncpy( pkd->m_szCode, lpszCode, min(sizeof(pkd->m_szCode)-1,strlen(lpszCode)) );

	pkd->m_time			=	pHistory->m_time;
	CSPTime	sptime(pkd->m_time);
	if( 0 != pkd->m_time && -1 != pkd->m_time )
		pkd->m_date			=	sptime.ToStockTimeDay();
	pkd->m_fOpen		=	pHistory->m_fOpen;
	pkd->m_fHigh		=	pHistory->m_fHigh;
	pkd->m_fLow			=	pHistory->m_fLow;
	pkd->m_fClose		=	pHistory->m_fClose;
	pkd->m_fVolume		=	pHistory->m_fVolume * 100;
	pkd->m_fAmount		=	pHistory->m_fAmount;
	pkd->m_dwAdvance	=	pHistory->m_wAdvance;
	pkd->m_dwDecline	=	pHistory->m_wDecline;
	return TRUE;
}

BOOL convert_TSMINUTE_to_MINUTE( DWORD dwMarket, LPCTSTR lpszCode, RCV_MINUTE_STRUCTEx * pTSMinute, MINUTE * pMinute )
{
	SP_ASSERT( pTSMinute && pMinute );
	if( NULL == pTSMinute || NULL == pMinute )
		return FALSE;

	memset( pMinute, 0, sizeof(MINUTE) );

	pMinute->m_dwType	=	1;	// 1 min

	pMinute->m_dwMarket	=	dwMarket;
	if( NULL != lpszCode )
		strncpy( pMinute->m_szCode, lpszCode, min(sizeof(pMinute->m_szCode)-1,strlen(lpszCode)) );

	pMinute->m_time		=	pTSMinute->m_time;
	pMinute->m_fNew		=	pTSMinute->m_fPrice;
	pMinute->m_fHigh	=	pTSMinute->m_fPrice;
	pMinute->m_fLow		=	pTSMinute->m_fPrice;
	pMinute->m_fVolume	=	pTSMinute->m_fVolume * 100;
	pMinute->m_fAmount	=	pTSMinute->m_fAmount;
	return TRUE;
}

BOOL convert_TSMULTISORT_to_MULTISORT( RCV_MULTISORT_STRUCTEx * pTSMultisort, MULTISORT * pMultisort )
{
	SP_ASSERT( pTSMultisort && pMultisort );
	if( NULL == pTSMultisort || NULL == pMultisort )
		return FALSE;

	SP_ASSERT( sizeof(RCV_MULTISORT_STRUCTEx) == sizeof(MULTISORT) );
	memcpy( pMultisort, pTSMultisort, sizeof(MULTISORT) );
	return TRUE;
}

BOOL convert_TSPOWER_to_DRDATA( DWORD dwMarket, LPCTSTR lpszCode, RCV_POWER_STRUCTEx * pTSPower, DRDATA * pdr )
{
	SP_ASSERT( pTSPower && pdr );
	if( NULL == pTSPower || NULL == pdr )
		return FALSE;

	memset( pdr, 0, sizeof(DRDATA) );
	
	pdr->m_dwMarket	=	dwMarket;
	if( NULL != lpszCode )
		strncpy( pdr->m_szCode, lpszCode, min(sizeof(pdr->m_szCode)-1,strlen(lpszCode)) );

	pdr->m_time			=	pTSPower->m_time;
	CSPTime	sptime(pdr->m_time);
	if( 0 != pdr->m_time && -1 != pdr->m_time )
		pdr->m_date			=	sptime.ToStockTimeDay();

	pdr->m_fGive		=	pTSPower->m_fGive;
	pdr->m_fPei			=	pTSPower->m_fPei;
	pdr->m_fPeiPrice	=	pTSPower->m_fPeiPrice;
	pdr->m_fProfit		=	pTSPower->m_fProfit;
	return TRUE;
}

