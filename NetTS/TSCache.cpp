// TSCache.cpp: implementation of the CTSCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TSCache.h"
#include "TW.h"
#include "SpTime.h"
#include "TWSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTSCache & CTSCache::GetInstance()
{
	static CTSCache s_tscache;
	return s_tscache;
}

CTSCache::CTSCache()
{
	m_nBufLen	=	0;
}

CTSCache::~CTSCache()
{
}

void CTSCache::SetLocalLatest(CSPTime t)
{
	m_tmLocalLatest	=	t;
}

CSPTime CTSCache::GetLocalLatest()
{
	return m_tmLocalLatest;
}

int CTSCache::OnReceive( BYTE * buf, size_t len )
{
	CSingleLock lock(&m_mutexBuffer,TRUE);

	if( NULL == buf || len <= 0 )
		return 0;
	if( len > sizeof(m_buffer) )
		return 0;
	if( m_nBufLen + len > sizeof(m_buffer) )
		m_nBufLen = 0;	// discard old
	memcpy( m_buffer+m_nBufLen, buf, len );
	m_nBufLen += len;
	int packets = DecodePacket();
	while( packets > 0 )
		packets = DecodePacket();
	return len;
}

int CTSCache::DecodePacket( )
{
	if( m_nBufLen <= 0 )
		return 0;

	int nPacketLen = FindFirstPacketLength();

	if( nPacketLen > 0 && nPacketLen <= (int)m_nBufLen )
	{
		TryGetPacket( nPacketLen );
		DiscardPacket( nPacketLen );
		return 1;
	}

	if( m_nBufLen > sizeof(m_buffer)/2 )
		m_nBufLen = 0;	// truncate if too big and no packets found.
	return 0;
}

int CTSCache::FindFirstPacketLength( )
{
	AlignBuffer(0, TRUE);

	size_t len = TryGetLength( m_buffer, m_nBufLen );
	size_t wantlen = len + sizeof(TW_HEADER);
	if( wantlen <= m_nBufLen )
		return wantlen;
//	else if( wantlen < sizeof(m_buffer) )
//		return 0;

	if( CTWSocket::GetInstance().IsReceiving() )	// 在接收大包
		return 0;

	len = AlignBuffer(4, FALSE);
	return len;
}

size_t CTSCache::AlignBuffer( int nStart, BOOL bRemove )
{
	if( m_nBufLen <= 4 )
		return 0;

	size_t i = 0;
	for( i=nStart; i<m_nBufLen-4; i++ )
	{
		if( TW_MAGIC_BYTE == m_buffer[i] )
		{
			if( TW_MAGIC_BYTE == m_buffer[i+1]
				&& TW_MAGIC_BYTE == m_buffer[i+2]
				&& TW_MAGIC_BYTE == m_buffer[i+3] )
			{
				break;
			}
		}
	}
	if( i > 0 && i+sizeof(TW_HEADER) < m_nBufLen )
	{
		int newlen = TryGetLength(m_buffer+i,sizeof(TW_HEADER));
		if( newlen > 0 && i+sizeof(TW_HEADER)+newlen <= m_nBufLen )
		{
			if( bRemove )
			{
				m_nBufLen = m_nBufLen - i;
				memmove( m_buffer, m_buffer+i, m_nBufLen );
			}
			return i;
		}
	}
	return 0;
}

int CTSCache::TryGetPacket( int nPacketLen )
{
	if( nPacketLen <= 0 || nPacketLen > (int)m_nBufLen )
		return 0;

	PRCV_DATA	prcvdata = NULL;
	PRCV_DATA	prcvdata2 = NULL;
	int nLen = 0;

	prcvdata = new RCV_DATA();
	memset( prcvdata, 0, sizeof(RCV_DATA) );
 	nLen = TryGetInit( m_buffer, nPacketLen, prcvdata );
	if( nLen > 0 )
	{
		{ // clear
			CSingleLock lock(&m_mutexReports,TRUE);
			m_aReports.RemoveAll();
			m_mapReports.RemoveAll();
		}
		PushReport( prcvdata->m_pReport, prcvdata->m_nPacketNum );
		PushPacket( RCV_REPORT, prcvdata );
		prcvdata = NULL;
		StoreReports( );
		return 1;
	}
	if( prcvdata )	FreePacket(prcvdata);

	prcvdata = new RCV_DATA();
	memset( prcvdata, 0, sizeof(RCV_DATA) );
	nLen = TryGetReport( m_buffer, nPacketLen, prcvdata );
	if( nLen > 0 )
	{
		PushReport( prcvdata->m_pReport, prcvdata->m_nPacketNum );
		PushPacket( RCV_REPORT, prcvdata );
		prcvdata = NULL;
		return 1;
	}
	if( prcvdata )	FreePacket(prcvdata);

	prcvdata = new RCV_DATA();
	prcvdata2 = new RCV_DATA();
	memset( prcvdata, 0, sizeof(RCV_DATA) );
	memset( prcvdata2, 0, sizeof(RCV_DATA) );
	nLen = TryGetMinute( m_buffer, nPacketLen, prcvdata, prcvdata2 );
	if( nLen > 0 )
	{
		PushReport( prcvdata2->m_pReport, prcvdata2->m_nPacketNum );
		PushPacket( RCV_FILEDATA, prcvdata );
		PushPacket( RCV_REPORT, prcvdata2 );
		prcvdata = NULL;
		prcvdata2 = NULL;
		return 1;
	}
	if( prcvdata )	FreePacket(prcvdata);
	if( prcvdata2 )	FreePacket(prcvdata2);

	prcvdata = new RCV_DATA();
	memset( prcvdata, 0, sizeof(RCV_DATA) );
	nLen = TryGetHistory( m_buffer, nPacketLen, prcvdata );
	if( nLen > 0 )
	{
		PushPacket( RCV_FILEDATA, prcvdata );
		prcvdata = NULL;
		return 1;
	}
	if( prcvdata )	FreePacket(prcvdata);

	prcvdata = new RCV_DATA();
	memset( prcvdata, 0, sizeof(RCV_DATA) );
	nLen = TryGetMultisort( m_buffer, nPacketLen, prcvdata );
	if( nLen > 0 )
	{
		PushPacket( RCV_FILEDATA, prcvdata );
		prcvdata = NULL;
		return 1;
	}
	if( prcvdata )	FreePacket(prcvdata);

	prcvdata = new RCV_DATA();
	memset( prcvdata, 0, sizeof(RCV_DATA) );
	nLen = TryGetDetail( m_buffer, nPacketLen, prcvdata );
	if( nLen > 0 )
	{
		PushPacket( RCV_REPORT, prcvdata );
		prcvdata = NULL;
		return 1;
	}
	if( prcvdata )	FreePacket(prcvdata);

	prcvdata = new RCV_DATA();
	memset( prcvdata, 0, sizeof(RCV_DATA) );
	nLen = TryGetBase( m_buffer, nPacketLen, prcvdata );
	if( nLen > 0 )
	{
		PushPacket( RCV_FILEDATA, prcvdata );
		prcvdata = NULL;
		return 1;
	}
	if( prcvdata )	FreePacket(prcvdata);

	return 0;
}

int CTSCache::DiscardPacket( int nPacketLen )
{
	if( nPacketLen > 0 && nPacketLen <= (int)m_nBufLen )
	{
		m_nBufLen = m_nBufLen - nPacketLen;
		if( m_nBufLen > 0 )
			memmove( m_buffer, m_buffer+nPacketLen, m_nBufLen );
		return nPacketLen;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// Packets

BOOL CTSCache::PushPacket( UINT nMsgType, PRCV_DATA pRCV_DATA )
{
	ASSERT( pRCV_DATA && pRCV_DATA->m_pData );

	CSingleLock lock(&m_mutexPackets,TRUE);

	TS_PACKET packet;
	packet.m_nMsgType = nMsgType;
	packet.m_pRCV_DATA = pRCV_DATA;
	m_aPackets.Add( packet );
	return TRUE;
}

BOOL CTSCache::PopPacket( UINT & nMsgType, PRCV_DATA & pRCV_DATA )
{
	CSingleLock lock(&m_mutexPackets,TRUE);

	if( m_aPackets.GetSize() > 0 )
	{
		nMsgType = m_aPackets.ElementAt(0).m_nMsgType;
		pRCV_DATA = m_aPackets.ElementAt(0).m_pRCV_DATA;
		m_aPackets.RemoveAt(0);
		return TRUE;
	}
	return FALSE;
}

void CTSCache::FreePacket( PRCV_DATA pRCV_DATA )
{
	if( pRCV_DATA )
	{
		if( pRCV_DATA->m_pData )
			delete [] pRCV_DATA->m_pData;
		delete pRCV_DATA;
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Reports

CString GetNetTSReportsFileName()
{
	static	CString	sReportsFile;
	if( sReportsFile.IsEmpty() )
	{
		sReportsFile = AfxGetQSProfile().GetWorkDirectory() + "dat/report.now";
	}
	return sReportsFile;
}

BOOL CTSCache::StoreReports( )
{
	CSingleLock lock(&m_mutexReports,TRUE);
	CFile file;
	if( m_aReports.GetSize() > 0 && file.Open( GetNetTSReportsFileName(), CFile::modeWrite | CFile::modeCreate ) )
	{
		CSPTime	sptime(m_aReports.ElementAt(0).m_time);
		DWORD	date = sptime.ToStockTimeDay();
		int		size = m_aReports.GetSize();

		SetLocalLatest( sptime );

		file.SeekToBegin();
		file.Write( &date, sizeof(date) );
		file.Write( &size, sizeof(size) );
		file.Write( m_aReports.GetData(), sizeof(RCV_REPORT_STRUCTEx)*m_aReports.GetSize() );
		file.Close();
		return TRUE;
	}
	
	return FALSE;
}

BOOL CTSCache::LoadReports( )
{
	CSingleLock lock(&m_mutexReports,TRUE);
	CFile file;
	if( file.Open( GetNetTSReportsFileName(), CFile::modeRead ) )
	{
		DWORD	date = -1;
		int		size = 0;
		DWORD	len = file.GetLength();

		file.SeekToBegin();
		if( sizeof(date) != file.Read( &date, sizeof(date) )
			|| sizeof(size) != file.Read( &size, sizeof(size) ) )
		{
			file.Close();
			return FALSE;
		}
		if( size <= 0 || size > 50000 || size*sizeof(RCV_REPORT_STRUCTEx)+sizeof(DWORD)+sizeof(int) != len )
		{
			file.Close();
			return FALSE;
		}
		CSPTime sptime;
		sptime.FromStockTimeDay( date );
		SetLocalLatest( sptime );
		
		m_aReports.RemoveAll();
		m_mapReports.RemoveAll();
		m_aReports.SetSize( size );
		file.Read( m_aReports.GetData(), sizeof(RCV_REPORT_STRUCTEx)*size );

		for( int nNo=0; nNo<m_aReports.GetSize(); nNo++ )
		{
			m_aReports[nNo].m_cbSize = sizeof(RCV_REPORT_STRUCTEx);
			m_mapReports.SetAt( m_aReports[nNo].m_szLabel, (void *)nNo );
		}

		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CTSCache::ReserveReportsSize( int size )
{
	CSingleLock lock(&m_mutexReports,TRUE);
	if( m_aReports.GetSize() < size )
		m_aReports.SetSize( size );
	return TRUE;
}

BOOL CTSCache::PushReport( int nNo, RCV_REPORT_STRUCTEx * pBuf )
{
	if( NULL == pBuf )
		return FALSE;

	CSingleLock lock(&m_mutexReports,TRUE);

	ASSERT(	pBuf->m_cbSize == sizeof(RCV_REPORT_STRUCTEx) );
	pBuf->m_cbSize = sizeof(RCV_REPORT_STRUCTEx);
	if( nNo >= 0 && nNo < m_aReports.GetSize() )
	{
		if( pBuf->m_fLastClose < 1e-4 )		pBuf->m_fLastClose = m_aReports.ElementAt(nNo).m_fLastClose;
		if( 0 == pBuf->m_wMarket )			pBuf->m_wMarket = m_aReports.ElementAt(nNo).m_wMarket;
		if( strlen(pBuf->m_szName) <= 0 )	memcpy(pBuf->m_szName,m_aReports.ElementAt(nNo).m_szName,sizeof(pBuf->m_szName));
		if( pBuf->m_time < m_aReports.ElementAt(nNo).m_time )
			return FALSE;
	}
	m_aReports.SetAtGrow( nNo, *pBuf );
	m_mapReports.SetAt( pBuf->m_szLabel, (void *)nNo );
	return TRUE;
}

BOOL CTSCache::PushReport( char * pszStockCode, RCV_REPORT_STRUCTEx * pBuf )
{
	if( NULL == pszStockCode || strlen(pszStockCode) <= 0 || NULL == pBuf )
		return FALSE;

	CSingleLock lock(&m_mutexReports,TRUE);

	// look for it in map
	void * rValue = NULL;
	if( m_mapReports.Lookup( pszStockCode, rValue ) )
	{
		int nNo = (int)rValue;
		ASSERT( nNo >= 0 && nNo < m_aReports.GetSize() && 0 == strncmp(pszStockCode, m_aReports.ElementAt(nNo).m_szLabel, sizeof(m_aReports.ElementAt(nNo).m_szLabel)) );
		if( nNo >= 0 && nNo < m_aReports.GetSize()
			&& ( 0 == (m_aReports.ElementAt(nNo).m_szLabel[0] )
				|| 0 == strncmp(pszStockCode, m_aReports.ElementAt(nNo).m_szLabel, sizeof(m_aReports.ElementAt(nNo).m_szLabel)) ) )
			return PushReport( nNo, pBuf );
		else // something error
			m_mapReports.RemoveKey( pszStockCode );
	}

	ASSERT(	pBuf->m_cbSize == sizeof(RCV_REPORT_STRUCTEx) );
	pBuf->m_cbSize = sizeof(RCV_REPORT_STRUCTEx);
	int nNo = m_aReports.Add( *pBuf );
	m_mapReports.SetAt( pszStockCode, (void *)nNo );
	return TRUE;
}

int CTSCache::PushReport( RCV_REPORT_STRUCTEx * pBuf, int size )
{
	if( NULL == pBuf || size <= 0 )
		return 0;
	for( int i=0; i<size; i++ )
		PushReport( pBuf[i].m_szLabel, &(pBuf[i]) );
	return size;
}

int CTSCache::GetTotalNumber()
{
	CSingleLock lock(&m_mutexReports,TRUE);
	
	if( m_aReports.GetSize() <= 0 )
	{
		LoadReports( );
	}
	return m_aReports.GetSize();
}

BOOL CTSCache::GetStockByNoEx( int nNo, RCV_REPORT_STRUCTEx * pBuf )
{
	CSingleLock lock(&m_mutexReports,TRUE);

	if( nNo < 0 || nNo >= m_aReports.GetSize() )
		return FALSE;
	if( pBuf && pBuf->m_cbSize > 0 )
	{
		memcpy( pBuf, &(m_aReports.ElementAt(nNo)), pBuf->m_cbSize );
	}
	else if( pBuf )
	{
		memcpy( pBuf, &(m_aReports.ElementAt(nNo)), sizeof(RCV_REPORT_STRUCTEx) );
	}

	return TRUE;
}

BOOL CTSCache::GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf)
{
	if( NULL == pszStockCode || strlen(pszStockCode) <= 0 )
		return FALSE;

	CSingleLock lock(&m_mutexReports,TRUE);

	// look for it in map
	void * rValue = NULL;
	if( m_mapReports.Lookup( pszStockCode, rValue ) )
	{
		int nNo = (int)rValue;
		ASSERT( nNo >= 0 && nNo < m_aReports.GetSize() && 0 == strncmp(pszStockCode, m_aReports.ElementAt(nNo).m_szLabel, sizeof(m_aReports.ElementAt(nNo).m_szLabel)) );
		if( nNo >= 0 && nNo < m_aReports.GetSize()
			&& 0 == strncmp(pszStockCode, m_aReports.ElementAt(nNo).m_szLabel, sizeof(m_aReports.ElementAt(nNo).m_szLabel)) )
			return GetStockByNoEx( nNo, pBuf );
		else // something error
			m_mapReports.RemoveKey( pszStockCode );
	}

	return FALSE;
}
