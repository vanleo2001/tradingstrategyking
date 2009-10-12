/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Packet convertor functions;
*/

#include	"StdAfx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//	allocate and free memory for COMMPACKET

PCOMMPACKET	AllocCommPacket( DWORD dwDataType, DWORD dwCount )
{
	DWORD	dwUnitSize	=	0;
	switch( dwDataType )
	{
	case CStock::dataDR:
		dwUnitSize	=	sizeof(DRDATA);
		break;
	case CStock::dataK:
		dwUnitSize	=	sizeof(KDATA);
		break;
	case CStock::dataReport:
		dwUnitSize	=	sizeof(REPORT);
		break;
	case CStock::dataMinute:
		dwUnitSize	=	sizeof(MINUTE);
		break;
	case CStock::dataMultisort:
		dwUnitSize	=	sizeof(MULTISORT);
		break;
	case CStock::dataOutline:
		dwUnitSize	=	sizeof(OUTLINE);
		break;
	case CStock::dataCode:
		dwUnitSize	=	sizeof(STOCKCODE);
		break;
	default:
		SP_ASSERT(FALSE);
	}
	
	DWORD	nSize = sizeof(COMMPACKET)+dwCount*dwUnitSize;
	COMMPACKET * pCommPacket = (COMMPACKET *)new BYTE[nSize];
	if( NULL == pCommPacket )
		return NULL;

	memset( pCommPacket, 0, nSize );
	pCommPacket->m_dwTag		=	STKLIB_COMMPACKET_TAG;
	pCommPacket->m_dwDataType	=	dwDataType;
	pCommPacket->m_dwCount		=	dwCount;
	pCommPacket->m_pData		=	(void *)( pCommPacket + 1 );
	if( 0 == dwCount )
		pCommPacket->m_pData	=	NULL;
	
	return pCommPacket;
}

void FreeCommPacket( PCOMMPACKET pCommPacket )
{
	SP_ASSERT( pCommPacket );
	if( pCommPacket )
		delete [] (BYTE *)pCommPacket;
}

BOOL convert_REPORT_to_MINUTE( REPORT * pReport, MINUTE * pMinute )
{
	SP_ASSERT( pReport && pMinute );
	if( NULL == pReport || NULL == pMinute )
		return FALSE;

	memset( pMinute, 0, sizeof(MINUTE) );

	pMinute->m_dwType	=	1;	// 1 min

	pMinute->m_dwMarket	=	pReport->m_dwMarket;
	strncpy( pMinute->m_szCode, pReport->m_szCode, min(sizeof(pMinute->m_szCode)-1,sizeof(pReport->m_szCode)) );

	time_t	temp = 60 * (pReport->m_time/60);
	if( temp < pReport->m_time )
		temp	+=	60;
	pMinute->m_time		=	temp;
	pMinute->m_fNew		=	pReport->m_fNew;
	pMinute->m_fHigh	=	pReport->m_fHigh;
	pMinute->m_fLow		=	pReport->m_fLow;
	pMinute->m_fVolume	=	pReport->m_fVolume;
	pMinute->m_fAmount	=	pReport->m_fAmount;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Updator

// update KDATA by Report data
BOOL UpdateKDATAByREPORT( KDATA &kd, REPORT * pReport )
{
	SP_ASSERT( pReport );
	if( NULL == pReport )
		return FALSE;

	memset( &kd, 0, sizeof(kd) );

	kd.m_dwMarket	=	pReport->m_dwMarket;
	strncpy( kd.m_szCode, pReport->m_szCode, min(sizeof(kd.m_szCode)-1,sizeof(pReport->m_szCode)) );

	kd.m_time	=	pReport->m_time;
	if( 0 == pReport->m_time || -1 == pReport->m_time )
		kd.m_date	=	CSPTime::GetCurrentTime().ToStockTimeDay();
	else
		kd.m_date	=	CSPTime(pReport->m_time).ToStockTimeDay();
	kd.m_fOpen		=	pReport->m_fOpen;
	kd.m_fHigh		=	pReport->m_fHigh;
	kd.m_fLow		=	pReport->m_fLow;
	kd.m_fClose		=	pReport->m_fNew;
	kd.m_fAmount	=	pReport->m_fAmount;
	kd.m_fVolume	=	pReport->m_fVolume;
	return TRUE;
}

// update CStockInfo by Report data
BOOL UpdateStockInfoByREPORT( CStockInfo & info, REPORT * pReport )
{
	SP_ASSERT( pReport );
	if( !pReport )
		return FALSE;

	// 股票市场
	if( strlen(pReport->m_szCode) > 0 )
		info.SetStockCode( pReport->m_dwMarket, pReport->m_szCode );
	if( strlen(pReport->m_szName) > 0  )
		info.SetStockName( pReport->m_szName );

	if( info.GetType() == 0 )
	{
		if( CStock::marketSHSE == pReport->m_dwMarket )
			info.SetType( CStock::typeshA );
		else if( CStock::marketSZSE == pReport->m_dwMarket )
			info.SetType( CStock::typeszA );
		else
			info.SetType( CStock::typeshA );
	}

	// 成交买卖价量信息
	if( pReport->m_fLast > 1e-4 )	info.m_fLast		=	pReport->m_fLast;
	info.m_fOpen		=	pReport->m_fOpen;
	info.m_fHigh		=	pReport->m_fHigh;
	info.m_fLow			=	pReport->m_fLow;
	info.m_fClose		=	pReport->m_fNew;
	info.m_fVolume		=	pReport->m_fVolume;
	info.m_fAmount		=	pReport->m_fAmount;

	info.m_fBuyPrice[0]		=	pReport->m_fBuyPrice[0];
	info.m_fBuyPrice[1]		=	pReport->m_fBuyPrice[1];
	info.m_fBuyPrice[2]		=	pReport->m_fBuyPrice[2];
	info.m_fBuyPrice[3]		=	pReport->m_fBuyPrice[3];
	info.m_fBuyPrice[4]		=	pReport->m_fBuyPrice[4];
	info.m_fBuyVolume[0]	=	pReport->m_fBuyVolume[0];
	info.m_fBuyVolume[1]	=	pReport->m_fBuyVolume[1];
	info.m_fBuyVolume[2]	=	pReport->m_fBuyVolume[2];
	info.m_fBuyVolume[3]	=	pReport->m_fBuyVolume[3];
	info.m_fBuyVolume[4]	=	pReport->m_fBuyVolume[4];
	info.m_fSellPrice[0]	=	pReport->m_fSellPrice[0];
	info.m_fSellPrice[1]	=	pReport->m_fSellPrice[1];
	info.m_fSellPrice[2]	=	pReport->m_fSellPrice[2];
	info.m_fSellPrice[3]	=	pReport->m_fSellPrice[3];
	info.m_fSellPrice[4]	=	pReport->m_fSellPrice[4];
	info.m_fSellVolume[0]	=	pReport->m_fSellVolume[0];
	info.m_fSellVolume[1]	=	pReport->m_fSellVolume[1];
	info.m_fSellVolume[2]	=	pReport->m_fSellVolume[2];
	info.m_fSellVolume[3]	=	pReport->m_fSellVolume[3];
	info.m_fSellVolume[4]	=	pReport->m_fSellVolume[4];

	// K线数据、日期
	KDATA	kd;
	UpdateKDATAByREPORT( kd, pReport );

	int nLen = info.m_kdata.GetSize();
	if( nLen > 0 && info.m_kdata.ElementAt(nLen-1).m_date == kd.m_date )
		info.m_kdata.SetAt( nLen-1, kd );
	else
		info.m_kdata.Add( kd );
	info.m_datetech	=	kd.m_date;

	// 保存
	memcpy( &(info.m_reportLatest), pReport, sizeof(info.m_reportLatest) );
	return TRUE;
}

// update CStockContainer by Report data
BOOL UpdateStockContainerByREPORT( CStockContainer &container, REPORT * pReport, BOOL bAddIfNotExist, REPORT * pReportLast )
{
	SP_ASSERT( pReport );
	if( NULL == pReport )
		return FALSE;

	REPORT	reportLast;
	memset( &reportLast, 0, sizeof(reportLast) );

	int	id = 0;
	if( container.GetStockInfo( pReport->m_szCode, NULL, &id ) )
	{
		container.Lock();

		CStockInfo	& info	=	container.ElementAt(id);
		reportLast	=	info.m_reportLatest;
		UpdateStockInfoByREPORT( info, pReport );

		container.UnLock();
	}
	else if( bAddIfNotExist && strlen(pReport->m_szCode)>0 )
	{
		CStockInfo	info;
		info.SetStockCode( pReport->m_dwMarket, pReport->m_szCode );
		if( UpdateStockInfoByREPORT( info, pReport ) )
		{
			container.Add( info );
		}
	}

	if( pReportLast )
		memcpy( pReportLast, &reportLast, sizeof(reportLast) );

	return TRUE;
}

BOOL UpdateStockContainerByKData( CStockContainer &container, LPCTSTR lpszCode, CKData & kdata )
{
	container.Lock();

	int	id = 0;
	if( CKData::ktypeDay == kdata.GetKType()
		&& container.GetStockInfo( lpszCode, NULL, &id ) )
	{
		CStockInfo	& info	=	container.ElementAt(id);
		if( kdata.GetSize() > (int)AfxGetProfile().GetCacheDays() )
		{
			// 只使用AfxGetProfile().GetCacheDays()天的数据
			CKData	temp( kdata.GetKType() );
			for( int i=kdata.GetSize()-AfxGetProfile().GetCacheDays(); i<kdata.GetSize(); i++ )
				temp.Add( kdata.ElementAt(i) );
			info.m_kdata.MergeKData( &temp );
		}
		else
			info.m_kdata.MergeKData( &kdata );

		
		// 指数涨跌家数
		int nSize = info.m_kdata.GetSize();
		if( nSize > 0 )
		{
			info.m_dwAdvance	=	info.m_kdata.ElementAt(nSize-1).m_dwAdvance;
			info.m_dwDecline	=	info.m_kdata.ElementAt(nSize-1).m_dwDecline;
		}
	}
	container.UnLock();
	return TRUE;
}

