
#pragma once

#include "ProxySocket.h"
#include "TW.h"

/////////////////////////////////////////////////////////////////////////////
// CTWSocket command target

class CSimpTWCommand
{
public:
	int m_nDataType;	// Data Type, such as FILE_HISTORY_EX, FILE_MINUTE_EX
	int m_nType;		// Stock Type
	char m_szCode[32];	// Stock Code
	long m_time;		// The time of appending this command, return from time(NULL).

	CSimpTWCommand()
	{
		m_nDataType	= 0;
		m_nType = 0;
		memset(m_szCode, 0, sizeof(m_szCode));
		m_time = 0;
	}

	CSimpTWCommand(int nDataType, int nStockType, char* szCode, long time) : m_nDataType(nDataType), m_nType(nStockType), m_time(time)
	{
		memset(m_szCode, 0, sizeof(m_szCode));
		strncpy(m_szCode, szCode, min(sizeof(m_szCode)-1, strlen(szCode)));
	}

	CSimpTWCommand(const CSimpTWCommand& src)
	{
		*this = src;
	}

	CSimpTWCommand& operator = (const CSimpTWCommand& src)
	{
		m_nDataType = src.m_nDataType;
		m_nType = src.m_nType;
		memcpy(m_szCode, src.m_szCode, sizeof(m_szCode));
		m_time = src.m_time;

		return *this;
	}
};

typedef CArray< CSimpTWCommand, CSimpTWCommand &> CSimpTWCommandArray;

#define TWCMD_DATATYPE_COUNT 12

class CTWCommand
{
public:
	int m_nDataType;
	CTWStockArray m_stocks;
	int	m_nKType;
	int	m_nDataCount;

	CTWCommand()
	{
		m_nDataType	= 0;
		m_nKType = 0;
		m_nDataCount = 0;
	}

	CTWCommand(int nDataType, TW_STOCK* pStocks, int nSize, int nKType, int nDataCount)	: m_nDataType(nDataType), m_nKType(nKType), m_nDataCount(nDataCount)
	{
		m_stocks.SetSize(0, nSize+1);
		for (int i=0; i<nSize; i++)
			m_stocks.Add(pStocks[i]);
	}

	CTWCommand(const CTWCommand& src)
	{
		*this = src;
	}

	CTWCommand& operator = (const CTWCommand& src)
	{
		m_nDataType	= src.m_nDataType;
		m_stocks.Copy(src.m_stocks);
		m_nKType = src.m_nKType;
		m_nDataCount= src.m_nDataCount;
		return *this;
	}
};

typedef CArray<CTWCommand, CTWCommand&> CTWCommandArray;

class CTWSocket : public CProxySocket
{
public:
	CTWSocket();
	virtual ~CTWSocket();

protected:
	CString	m_strHostAddress;
	UINT m_nHostPort;
	CString m_strUser;
	CString m_strPasswd;
	BOOL m_bIsReceiving;
	time_t m_timeReceiveLast;

	CCriticalSection m_mutex;
	CMapStringToPtr m_mapCommands;
	CTWCommandArray	m_queueCommands;

	int m_bAutoReport;
	BYTE m_rbuffer[0x10000];	// 64k 字节接收缓冲区

public:
	static CTWSocket& GetInstance();
	static void Init();
	static void Release();

	BOOL IsWorking();
	BOOL BeginWorking(LPCTSTR lpszHostAddress, UINT nHostPort, LPCTSTR lpszUser, LPCTSTR lpszPasswd);
	BOOL BeginWorking();
	void EndWorking();
	BOOL Login(LPCTSTR lpszUser, LPCTSTR lpszPasswd);

	int  SetAutoReport(int bAutoReport = FALSE);
	int  GetAutoReport();

	BOOL OnNewCommand(int DataType, TW_STOCK* pstock, int nMinTimeGap);

	void PushCommand(CTWCommand& cmd);
	BOOL PopCommand(CTWCommand& cmd);

	BOOL IsReceiving() { return m_bIsReceiving; }
	int RequestStockData(int nDataType, TW_STOCK* pStocks, int nSize, int nKType, int nDataCount);

	int RequestInit();
	int RequestReport(TW_STOCK* pStock, int nSize);
	int RequestHistory(TW_STOCK* pStock, int nSize, int nKType, int nDataCount);
	int RequestMinute(TW_STOCK* pStock, int nSize);
	int RequestPower(TW_STOCK* pStock, int nSize);
	int RequestMultisort(TW_STOCK* pStock, int nSize);
	int RequestDetail(TW_STOCK* pStock, int nSize);
	int RequestBase(TW_STOCK* pStock, int nSize);
	int RequestNews(TW_STOCK* pStock, int nSize);
	int RequestHtml(TW_STOCK* pStock, int nSize);
	int RequestSoftware(TW_STOCK* pStock, int nSize);
	int RequestShazqdata(TW_STOCK* pStock, int nSize);

public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
};
