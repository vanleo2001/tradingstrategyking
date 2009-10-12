// TSCache.h: interface for the CTSCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSCACHE_H__61CC5EB5_4560_4F34_A6DA_AA35B09D79EC__INCLUDED_)
#define AFX_TSCACHE_H__61CC5EB5_4560_4F34_A6DA_AA35B09D79EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stockdrv.h"
#include "SPTime.h"

typedef struct _ts_packet_t {
	UINT		m_nMsgType;
	PRCV_DATA	m_pRCV_DATA;
} TS_PACKET, PTS_PACKET;
typedef CArray< TS_PACKET, TS_PACKET &> CTSPacketArray;

class CTSCache  
{
public:
	CTSCache();
	virtual ~CTSCache();

	void		SetLocalLatest(CSPTime t);
	CSPTime		GetLocalLatest();

	static CTSCache & GetInstance();

	void ClearReceiveBuffer() { m_nBufLen = 0; }

	// decode net received data
	int OnReceive( BYTE * buf, size_t len );
	int DecodePacket( );
	int FindFirstPacketLength( );
	size_t AlignBuffer( int nStart, BOOL bRemovePrev );
	int TryGetPacket( int nPacketLen );
	int DiscardPacket( int nPacketLen );

	// packets operation
	BOOL PushPacket( UINT nMsgType, PRCV_DATA pRCV_DATA );
	BOOL PopPacket( UINT & nMsgType, PRCV_DATA & pRCV_DATA );
	void FreePacket( PRCV_DATA pRCV_DATA );

	// set report
	BOOL StoreReports( );
	BOOL LoadReports( );
	BOOL ReserveReportsSize( int size );
	BOOL PushReport( int nNo, RCV_REPORT_STRUCTEx * pBuf );
	BOOL PushReport( char * pszStockCode, RCV_REPORT_STRUCTEx * pBuf );
	int  PushReport( RCV_REPORT_STRUCTEx * pBuf, int size );

	// get report
	int  GetTotalNumber();
	BOOL GetStockByNoEx( int nNo, RCV_REPORT_STRUCTEx * pBuf );
	BOOL GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf);

protected:
	CSPTime				m_tmLocalLatest;

	BYTE m_buffer[0x80000];	// 512k bytes buffer
	size_t m_nBufLen;
	CCriticalSection	m_mutexBuffer;

	CTSPacketArray		m_aPackets;
	CCriticalSection	m_mutexPackets;

	CMapStringToPtr		m_mapReports;
	CArray<RCV_REPORT_STRUCTEx, RCV_REPORT_STRUCTEx &>	m_aReports;
	CCriticalSection	m_mutexReports;
};

#endif // !defined(AFX_TSCACHE_H__61CC5EB5_4560_4F34_A6DA_AA35B09D79EC__INCLUDED_)
