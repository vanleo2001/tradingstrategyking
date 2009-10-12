
#include "stdafx.h"
#include "TWSocket.h"
#include "TSCache.h"
#include "StockDrv.h"
#include "SpTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TWAutoReportThreadMain

UINT TWAutoReportThreadMain(LPVOID pParam)
{
	static BOOL bFirst = TRUE;
	int nRequest;
	char szText[256];

	while (TRUE)
	{
		if (CTWSocket::GetInstance().GetAutoReport() && (CSPTime::InTradeTime(CSPTime::GetCurrentTime().GetTime(), 900) || bFirst))
		{
			int nSize = CTSCache::GetInstance().GetTotalNumber();
			if (nSize <= 0)
				continue;

			TW_STOCK stocks[160];
			memset(stocks, 0, sizeof(stocks));
			static int put = 0;

			RCV_REPORT_STRUCTEx report;
			for (int i=0; i<nSize; i++)
			{
				if (CTSCache::GetInstance().GetStockByNoEx(i, &report))
				{
					strncpy(stocks[put].m_code, report.m_szLabel, sizeof(stocks[put].m_code));
					stocks[put].m_type = typeshA;

					if (SH_MARKET_EX == report.m_wMarket)
						stocks[put].m_type = typeshA;
					else if(SZ_MARKET_EX == report.m_wMarket)
						stocks[put].m_type = typeszA;
					put++;

					TTrace::Debug()->Send("NetTS", report.m_szName);

					if (put >= 160)
					{
						nRequest = CTWSocket::GetInstance().RequestReport(stocks, put);

						sprintf(szText, "TWAutoReportThreadMain;;RequestReport() = %d", nRequest);
						g_pWinTrace->Debug()->Send("NetTS", szText);

						put = 0;
						Sleep(1000);
					}
				}
			}

			if (put > 0)
			{
				nRequest = CTWSocket::GetInstance().RequestReport(stocks, put);

				sprintf(szText, "TWAutoReportThreadMain;;RequestReport() = %d", nRequest);
				g_pWinTrace->Debug()->Send("NetTS", szText);
			}
		}

		if (!CTWSocket::GetInstance().IsReceiving())
		{
			CTWCommand cmd;

			if (CTWSocket::GetInstance().PopCommand(cmd))
			{
				CTWSocket::GetInstance().RequestStockData(cmd.m_nDataType, cmd.m_stocks.GetData(),
					cmd.m_stocks.GetSize(), cmd.m_nKType, cmd.m_nDataCount);
			}
		}

		Sleep(5000);
	}

	AfxEndThread(0, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CTWSocket

CTWSocket::CTWSocket()
{
	m_mapCommands.InitHashTable(2000, FALSE);
	m_bIsReceiving = FALSE;

	SetAutoReport();
	AfxBeginThread(TWAutoReportThreadMain, NULL, THREAD_PRIORITY_NORMAL);
}

CTWSocket::~CTWSocket()
{
	POSITION pos = m_mapCommands.GetStartPosition();

	while (NULL != pos)
	{
		CString rKey;
		void* rValue;
		m_mapCommands.GetNextAssoc(pos, rKey, rValue);
		if (NULL != rValue)
		{
			CSimpTWCommandArray* ptr = (CSimpTWCommandArray*) rValue;
			delete ptr;
		}
	}

	m_mapCommands.RemoveAll();
}

CTWSocket& CTWSocket::GetInstance()
{
	static CTWSocket s_twsocket;
	return s_twsocket;
}

void CTWSocket::Init()
{
	AfxSocketInit();
}

void CTWSocket::Release()
{
	if (GetInstance().IsWorking())
		GetInstance().EndWorking();

	AfxSocketTerm();
}

BOOL CTWSocket::IsWorking()
{
	CSingleLock	lock(&m_mutex, TRUE);

	if (INVALID_SOCKET == m_hSocket)
		return FALSE;

	CString rPeerAddress;
	UINT rPeerPort;
	if (GetPeerName(rPeerAddress, rPeerPort) && !rPeerAddress.IsEmpty())
		return TRUE;

	return FALSE;
}

BOOL CTWSocket::BeginWorking(LPCTSTR lpszHostAddress, UINT nHostPort, LPCTSTR lpszUser, LPCTSTR lpszPasswd)
{
	CSingleLock	lock(&m_mutex, TRUE);
	m_strHostAddress = lpszHostAddress;
	m_nHostPort = nHostPort;
	m_strUser = lpszUser;
	m_strPasswd = lpszPasswd;

	if (m_strHostAddress.IsEmpty())
		return FALSE;

	if (!Create())
		return FALSE;

	int nProxyType;
	CString strProxyAddress;
	UINT nProxyPort;
	AfxGetQSProfile().GetCompoundConfig(nProxyType, strProxyAddress, nProxyPort);
	SetProxyInfo(nProxyType, strProxyAddress, nProxyPort, AfxGetQSProfile().GetProxyUser(), AfxGetQSProfile().GetProxyPasswd());

	if (!ConnectThroughProxy(m_strHostAddress, m_nHostPort))
	{
		m_strLastError = AfxModuleLoadString(IDS_TWSOCKET_ERRLOGIN);

		// 没有连接成功，以后调用无参数的BeginWorking()不再连接
		m_strHostAddress.Empty();
		m_nHostPort = 0;
		m_strUser.Empty();
		m_strPasswd.Empty();
		Close();

		return FALSE;
	}

	if (!Login(lpszUser,lpszPasswd))
	{
		m_strLastError = AfxModuleLoadString(IDS_TWSOCKET_ERRLOGIN);

		// 没有连接成功，以后调用无参数的BeginWorking()不再连接
		m_strHostAddress.Empty();
		m_nHostPort = 0;
		m_strUser.Empty();
		m_strPasswd.Empty();
		Close();

		return FALSE;
	}

	return TRUE;
}

BOOL CTWSocket::BeginWorking()
{
	CSingleLock	lock(&m_mutex, TRUE);

	if (m_strHostAddress.IsEmpty())
		return FALSE;

	if (!Create())
		return FALSE;

	int nProxyType;
	CString strProxyAddress;
	UINT nProxyPort;
	AfxGetQSProfile().GetCompoundConfig(nProxyType, strProxyAddress, nProxyPort);
	SetProxyInfo(nProxyType, strProxyAddress, nProxyPort, AfxGetQSProfile().GetProxyUser(), AfxGetQSProfile().GetProxyPasswd());

	if (!ConnectThroughProxy(m_strHostAddress, m_nHostPort))
	{
		Close();

		return FALSE;
	}

	if (!Login(m_strUser,m_strPasswd))
	{
		m_strLastError = AfxModuleLoadString(IDS_TWSOCKET_ERRLOGIN);
		Close();

		return FALSE;
	}

	return TRUE;
}

void CTWSocket::EndWorking()
{
	CSingleLock	lock(&m_mutex, TRUE);

	ShutDown();
	Close();
}

BOOL CTWSocket::Login(LPCTSTR lpszUser, LPCTSTR lpszPasswd)
{
	BYTE sbuffer[512];
	int lenSend = 0;
	if (lenSend = ConstructLoginBuffer(sbuffer, sizeof(sbuffer), lpszUser, lpszPasswd))
	{
		if (Send(sbuffer, lenSend) == lenSend)
		{
			int lenRcv = Receive(m_rbuffer, sizeof(m_rbuffer));
			if (IsLoginOK(m_rbuffer, lenRcv))
			{
				CTSCache::GetInstance().LoadReports();
				RequestInit();

				return TRUE;
			}

			return FALSE;
		}
	}

	return FALSE;
}

int CTWSocket::SetAutoReport(int bAutoReport)
{
	int old = m_bAutoReport;
	m_bAutoReport = bAutoReport;

	return old;
}

int CTWSocket::GetAutoReport()
{
	return m_bAutoReport;
}

BOOL CTWSocket::OnNewCommand(int nDataType, TW_STOCK* pStock, int nMinTimeGap)
{
	if (nMinTimeGap <= 0)
		return TRUE;

	CSingleLock	lock(&m_mutex, TRUE);

	int	stocktype = 0x00;
	if (pStock)
		stocktype = pStock->m_type;
	char stockcode[sizeof(TW_STOCK)+1];
	memset(stockcode, 0, sizeof(stockcode));
	if (pStock)
		strncpy(stockcode, pStock->m_code, min(sizeof(stockcode),sizeof(pStock->m_code)));
	if (strlen(stockcode) <= 0)
		strncpy(stockcode, "NOCODE", sizeof(stockcode)-1);

	time_t now = time(NULL);
	CSimpTWCommand cmd(nDataType, stocktype, stockcode, now);

	void* rValue = NULL;
	CSimpTWCommandArray* ptr = NULL;
	if (m_mapCommands.Lookup(stockcode, rValue) && rValue)
	{
		ptr = (CSimpTWCommandArray*)rValue;
		for (int i=0; i<ptr->GetSize(); i++)
		{
			if (cmd.m_nType == ptr->ElementAt(i).m_nType
				&& cmd.m_nDataType == ptr->ElementAt(i).m_nDataType)
			{
				if ((cmd.m_time - ptr->ElementAt(i).m_time) < nMinTimeGap)
				{
					return FALSE;
				}
				else
				{
					ptr->ElementAt(i).m_time = now;
					return TRUE;
				}
			}
		}
		ptr->Add(cmd);
		return TRUE;
	}

	ptr = new CSimpTWCommandArray();
	if (ptr)
	{
		ptr->SetSize(0, TWCMD_DATATYPE_COUNT);
		ptr->Add(cmd);
		m_mapCommands.SetAt(stockcode, ptr);

		return TRUE;
	}

	return TRUE;
}

void CTWSocket::PushCommand(CTWCommand & cmd)
{
	CSingleLock	lock(&m_mutex, TRUE);
	m_queueCommands.Add(cmd);
}

BOOL CTWSocket::PopCommand(CTWCommand & cmd)
{
	CSingleLock	lock(&m_mutex, TRUE);

	if (m_queueCommands.GetSize() > 0)
	{
		cmd = m_queueCommands.ElementAt(0);
		m_queueCommands.RemoveAt(0);
		return TRUE;
	}

	return FALSE;
}

int CTWSocket::RequestStockData(int nDataType, TW_STOCK* pStocks, int nSize, int nKType, int nDataCount)
{
	if (m_bIsReceiving && time(NULL) - m_timeReceiveLast > 60)
		m_bIsReceiving = FALSE;

	if (m_bIsReceiving)
	{
		CTWCommand cmd(nDataType, pStocks, nSize, nKType, nDataCount);
		PushCommand(cmd);
		return nSize;
	}

	int nRet = 0;
	switch (nDataType)
	{
	case RCV_REPORT:
		if (OnNewCommand(nDataType, pStocks, 0))
			nRet = RequestReport(pStocks, nSize);
		break;
	case FILE_HISTORY_EX:
		if (OnNewCommand(nDataType, pStocks, 300))
			nRet = RequestHistory(pStocks, nSize, nKType, nDataCount);
		break;
	case FILE_MINUTE_EX:
		if (OnNewCommand(nDataType, pStocks, 60))
			nRet = RequestMinute(pStocks, nSize);
		break;
	case FILE_POWER_EX:
		if (OnNewCommand(nDataType, pStocks, 300))
			nRet = RequestPower(pStocks, nSize);
		break;
	case FILE_MULTISORT_EX:
		if (OnNewCommand(nDataType, pStocks, 0))
			nRet = RequestMultisort(pStocks, nSize);
		break;
	case FILE_DETAIL_EX:
		if(OnNewCommand(nDataType, pStocks, 60))
			nRet = RequestDetail(pStocks, nSize);
		break;
	case FILE_BASE_EX:
		if (OnNewCommand(nDataType, pStocks, 300))
			nRet = RequestBase(pStocks, nSize);
		break;
	case FILE_NEWS_EX:
		if (OnNewCommand(nDataType, pStocks, 60))
			nRet = RequestNews(pStocks, nSize);
		break;
	case FILE_HTML_EX:
		if (OnNewCommand(nDataType, pStocks, 60))
			nRet = RequestHtml(pStocks, nSize);
		break;
	case FILE_SOFTWARE_EX:
		if (OnNewCommand(nDataType, pStocks, 60))
			nRet = RequestSoftware(pStocks, nSize);
		break;
	case FILE_SHAZQDATA_EX:
		if (OnNewCommand(nDataType, pStocks, 60))
			nRet = RequestShazqdata(pStocks, nSize);
		break;
	default:
		break;
	}

	return nRet;
}

int CTWSocket::RequestInit()
{
	TW_ASK ask;
	int lenSend = ConstructAskInitBuffer(ask);
	if (lenSend > 0)
	{
		Send(&ask, lenSend);
		m_bIsReceiving = TRUE;
		m_timeReceiveLast = time(NULL);

		return 1;
	}

	return 0;
}

int CTWSocket::RequestReport(TW_STOCK* pStock, int nSize)
{
	TW_STOCK stocks[32];
	memset(stocks, 0, sizeof(stocks));
	int nMaxCount = sizeof(stocks)/sizeof(TW_STOCK);

	int putsize = 0;
	for (int i=0; i<nSize; i++)
	{
		memcpy(&(stocks[putsize]), &(pStock[i]), sizeof(TW_STOCK));
		putsize++;
		if (putsize >= nMaxCount || i==nSize-1)
		{
			TW_ASK ask;
			int lenSend = ConstructAskReportBuffer(ask, stocks, putsize);
			if (lenSend > 0)
				Send(&ask, lenSend);

			putsize = 0;
		}
	}

	return nSize;
}

int CTWSocket::RequestHistory(TW_STOCK* pStock, int nSize, int nKType, int nDataCount)
{
	for (int i=0; i<nSize; i++)
	{
		BOOL bChangeStock = TRUE;
		BOOL bFirstRequest = TRUE;

		while (nDataCount > 0)
		{
			int nCountNow = (nDataCount > 0x100 ? 0x100 : nDataCount);
			if (bChangeStock || bFirstRequest)
				nCountNow = 1;

			TW_ASK ask;
			int lenSend = ConstructAskHistoryBuffer(ask, &(pStock[i]), nKType, nCountNow, bChangeStock, bFirstRequest);
			if (lenSend > 0)
				Send(&ask, lenSend);
			nDataCount -= nCountNow;

			bChangeStock = FALSE;
			bFirstRequest = FALSE;
		}
	}

	return nSize;
}

int CTWSocket::RequestMinute(TW_STOCK* pStock, int nSize)
{
	for(int i=0; i<nSize; i++)
	{
		TW_ASK ask;
		int lenSend = ConstructAskMinuteBuffer(ask, &(pStock[i]));
		if (lenSend > 0)
			Send(&ask, lenSend);
	}

	return nSize;
}

int CTWSocket::RequestPower(TW_STOCK* pStock, int nSize)
{
	return 0;
}

int CTWSocket::RequestMultisort(TW_STOCK* pStock, int nSize)
{
	for (int i=0; i<nSize; i++)
	{
		TW_ASK ask;
		int lenSend = ConstructAskMultisortBuffer(ask, &(pStock[i]));
		if (lenSend > 0)
			Send(&ask, lenSend);
	}

	return nSize;
}

int CTWSocket::RequestDetail(TW_STOCK* pStock, int nSize)
{
	for (int i=0; i<nSize; i++)
	{
		TW_ASK ask;
		int lenSend = ConstructAskDetailBuffer(ask, &(pStock[i]));
		if (lenSend > 0)
			Send(&ask, lenSend);
	}

	return nSize;
}

int CTWSocket::RequestBase(TW_STOCK* pStock, int nSize)
{
	for(int i=0; i<nSize; i++)
	{
		TW_ASK ask;
		int lenSend = ConstructAskBaseBuffer(ask, &(pStock[i]));
		if(lenSend > 0)
		{
			Send(&ask, lenSend);
		}
	}

	return nSize;
}

int CTWSocket::RequestNews(TW_STOCK* pStock, int nSize)
{
	return 0;
}

int CTWSocket::RequestHtml(TW_STOCK* pStock, int nSize)
{
	return 0;
}

int CTWSocket::RequestSoftware(TW_STOCK* pStock, int nSize)
{
	return 0;
}

int CTWSocket::RequestShazqdata(TW_STOCK* pStock, int nSize)
{
	return 0;
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CTWSocket, CSocket)
END_MESSAGE_MAP()
#endif

/////////////////////////////////////////////////////////////////////////////
// CTWSocket member functions

void CTWSocket::OnClose(int nErrorCode) 
{
	CSocket::Close();

	CSocket::OnClose(nErrorCode);
}

void CTWSocket::OnConnect(int nErrorCode) 
{
	CSocket::OnConnect(nErrorCode);
}

void CTWSocket::OnReceive(int nErrorCode) 
{
	m_timeReceiveLast = time(NULL);

	int nReceive = Receive(m_rbuffer, sizeof(m_rbuffer));
	if (nReceive > 0)
	{
		CTSCache::GetInstance().OnReceive(m_rbuffer, nReceive);

		if (nReceive < 256 && TryGetLength(m_rbuffer, nReceive) < 256)	// 收到小块包,说明大包接收完毕
			m_bIsReceiving = FALSE;

		if (nReceive == sizeof(m_rbuffer))
			OnReceive(nErrorCode);
	}

	CSocket::OnReceive(nErrorCode);
}

void CTWSocket::OnSend(int nErrorCode) 
{
	CSocket::OnSend(nErrorCode);
}

int CTWSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	if (m_pbBlocking != NULL)
	{
		WSASetLastError(WSAEINPROGRESS);
		return FALSE;
	}

	int nResult;
	while ((nResult = CAsyncSocket::Receive(lpBuf, nBufLen, nFlags)) == SOCKET_ERROR)
	{
		if (GetLastError() == WSAEWOULDBLOCK)
		{
			if (!PumpMessages(FD_READ))
			{
				return SOCKET_ERROR;
			}
		}
		else
		{
			return SOCKET_ERROR;
		}
	}

	char szText[256];
	sprintf(szText, "→Receive Byte:%d", nResult);
	g_pWinTrace->Debug()->Send("CTWSocket::Receive", szText);

	return nResult;	
}

int CTWSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	char szText[256];
	sprintf(szText, "←Send Byte:%d", nBufLen);
	g_pWinTrace->Debug()->Send("CTWSocket::Send", szText);

	return CSocket::Send(lpBuf, nBufLen, nFlags);
}
