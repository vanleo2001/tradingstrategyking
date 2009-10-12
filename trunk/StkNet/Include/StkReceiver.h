
#pragma once

#define	WM_APP_STKRECEIVER_CONNECTSERVER	WM_APP+1
#define	WM_APP_STKRECEIVER_DISCONNECTSERVER	WM_APP+2
#define	WM_APP_STKRECEIVER_INIT				WM_APP+3
#define	WM_APP_STKRECEIVER_SETUP			WM_APP+4
#define	WM_APP_STKRECEIVER_QUIT				WM_APP+5
#define	WM_APP_STKRECEIVER_DATA				WM_APP+6
#define	WM_APP_STKRECEIVER_ALARM			WM_APP+7
#define	WM_APP_STKRECEIVER_TSDATA			WM_APP+8

#define	STKRCV_ALARM_REFRESH 1
#define	STKRCV_ALARM_WARNING 2

enum
{
	TSKMODE_NT,
	TSKMODE_TS,
	TSKMODE_SC,
	TSKMODE_WJF,
};

class CStkReceiver;
STKNET_API CStkReceiver& AfxGetStkReceiver();

/////////////////////////////////////////////////////////////////////////////
// CStkReceiver window

struct COMM_PACKET;

class STKNET_API CStkReceiver : public CWnd
{
public:
	CStkReceiver();
	virtual ~CStkReceiver();

protected:

	CArray<HWND, HWND>	m_awndRcvData;
	CArray<HWND, HWND>	m_awndRcvAlarm;

public:
	BOOL	m_bFirst;
	BOOL	m_bTongshiRunning;
	UINT	m_nMode;

	static HANDLE m_hEventKillProcessThread;
	static HANDLE m_hEventProcessThreadKilled;

public:
	BOOL	CreateReceiver(CWnd * pParentWnd);

	void	OnReceiveReport(REPORT * pReport);

	void	AddRcvAlarmWnd(HWND hWnd);
	BOOL	RemoveRcvAlarmWnd(HWND hWnd);
	void	NotifyWndRcvAlarm(WPARAM wParam, LPARAM lParam);

	void	AddRcvDataWnd(HWND hWnd);
	BOOL	RemoveRcvDataWnd(HWND hWnd);
	void	NotifyWndRcvData(WPARAM wParam, LPARAM lParam);

	BOOL	NetEngineBeginWorking();
	BOOL	NetEngineBeginWorking(LPCTSTR lpszAddress, UINT nPort, LPCTSTR lpszUser, LPCTSTR lpszPasswd);
	BOOL	NetEngineEndWorking();
	BOOL	NetEngineIsWorking();
	int		NetEngineSetAutoReport(int bAutoReport);

	int		RequestStockData(int nDataType, CStockInfo* pStockInfo, int nSize, int nKType, int nDataCount);

	BOOL	EngineBeginWorking(BOOL bShowSelectEngineDialog);
	BOOL	EngineSetup();
	BOOL	EngineEndWorking();
	BOOL	EngineIsWorking();

	int		RefreshStockContainer(CStockContainer & container, BOOL bAddIfNotExist);
	int		GetReport(CStockInfo &info, CReport &aReport);

	UINT	GetReceiverMode() { return m_nMode; }
	void	SetReceiverMode(UINT nMode) { m_nMode = nMode; }

	void	SCAskData(int nFunc, WORD wMarket, short nIndex);

	int		RequestKData(CStock* pstock, int period);
	int		RequestSCStockData(int nDataType, CStockInfo* pStockInfo, int nSize, int nKType, CSPTime tmBeginDate, CSPTime tmEndDate);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnStkReceiverConnectserver(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStkReceiverDisconnectserver(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStkReceiverInit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStkReceiverSetup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStkReceiverQuit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStkReceiverTsdata(UINT wFileType,LONG lPara);
	afx_msg LRESULT OnSCStockReply(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
