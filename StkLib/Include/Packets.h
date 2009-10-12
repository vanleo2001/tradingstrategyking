/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Packet convertor functions.
*/

#if	!defined( __STKLIB_PACKETS_H__ )
#define	__STKLIB_PACKETS_H__

STKLIB_API	PCOMMPACKET	AllocCommPacket( DWORD dwDataType, DWORD dwCount );
STKLIB_API	void	FreeCommPacket( PCOMMPACKET pCommPacket );

STKLIB_API	BOOL convert_REPORT_to_MINUTE( REPORT * pReport, MINUTE * pMinute );

STKLIB_API	BOOL UpdateKDATAByREPORT( KDATA &kd, REPORT * pReport );
STKLIB_API	BOOL UpdateStockInfoByREPORT( CStockInfo & info, REPORT * pReport );
STKLIB_API	BOOL UpdateStockContainerByREPORT( CStockContainer &container, REPORT * pReport, BOOL bAddIfNotExist, REPORT * pReportLast = NULL );
STKLIB_API	BOOL UpdateStockContainerByKData( CStockContainer &container, LPCTSTR lpszCode, CKData & kdata );


#endif	// __STKLIB_PACKETS_H__

