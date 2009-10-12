/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Tongshi interface convertor functions.
*/

#if	!defined( __SP_TONGSHI_H__ )
#define	__SP_TONGSHI_H__

DWORD	TSMarket_to_Market( WORD wMarket );
WORD	Market_to_TSMarket( DWORD dwMarket );
int		DataType_to_TSDataType( int nDataType );
BOOL convert_TSREPORT_to_REPORT( RCV_REPORT_STRUCTEx *pTSReport, REPORT * pReport );
BOOL convert_TSHISTORY_to_KDATA( DWORD dwMarket, LPCTSTR lspzCode, RCV_HISTORY_STRUCTEx * pHistory, KDATA * pkd );
BOOL convert_TSMINUTE_to_MINUTE( DWORD dwMarket, LPCTSTR lspzCode, RCV_MINUTE_STRUCTEx * pTSMinute, MINUTE * pMinute );
BOOL convert_TSMULTISORT_to_MULTISORT( RCV_MULTISORT_STRUCTEx * pTSMultisort, MULTISORT * pMultisort );
BOOL convert_TSPOWER_to_DRDATA( DWORD dwMarket, LPCTSTR lspzCode, RCV_POWER_STRUCTEx * pTSPower, DRDATA * pdr );

#endif	// __SP_TONGSHI_H__

