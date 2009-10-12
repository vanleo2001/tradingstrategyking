#if !defined(AFX_DOWNLOADDLG_H__047CD860_CF70_45E7_BC04_CB594D31D679__INCLUDED_)
#define AFX_DOWNLOADDLG_H__047CD860_CF70_45E7_BC04_CB594D31D679__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg dialog
#define	WM_USER_DOWNLOAD_MESSAGE	(WM_USER+1001)
#define	WM_USER_DOWNLOAD_PROGRESS	(WM_USER+1002)
#define	WM_USER_DOWNLOAD_END		(WM_USER+1003)

typedef	struct download_info_t	{
	HWND				hMainWnd;
	CNetDatabase	*	pNetDB;
	BOOL		bReport;
	BOOL		bDay;
	time_t		tmDayFrom;
	time_t		tmDayTo;
	BOOL		bMin5;
	time_t		tmMin5From;
	time_t		tmMin5To;
	BOOL		bBase;
	BOOL		bXDR;
} DOWNLOAD_INFO, * LPDOWNLOAD_INFO;

/***
	数据包下载对话框
*/
class CDownloadDlg : public CXFDialog
{
// Construction
public:
	CDownloadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDownloadDlg();

	void	SetInitDownloadTime( CSPTime tmDLBegin, CSPTime tmDLEnd, BOOL bAutoRun );

	static HANDLE m_hEventKillDownloadThread;
	static HANDLE m_hEventDownloadThreadKilled;

// Dialog Data
	//{{AFX_DATA(CDownloadDlg)
	enum { IDD = IDD_DOWNLOAD };
	CStatic	m_staticLatest;
	CStatic	m_staticProgress;
	CButton	m_btnCheckMin5;
	CButton	m_btnCheckReport;
	CButton	m_btnProxySet;
	CButton	m_btnCancel;
	CStatic	m_staticStatus;
	CButton	m_btnOK;
	CButton	m_btnStopRefresh;
	CButton	m_btnStop;
	CButton	m_btnRefreshServers;
	CButton	m_btnCheckXDR;
	CButton	m_btnCheckDay;
	CButton	m_btnCheckBase;
	CDateTimeCtrl	m_dtctrlDayTo;
	CDateTimeCtrl	m_dtctrlDayFrom;
	CProgressCtrl	m_ctrlProgress;
	CStatic	m_staticRefreshServer;
	CComboBox	m_comboServers;
	CSPTime	m_tmDayFrom;
	CSPTime	m_tmDayTo;
	CSPTime	m_tmMin5From;
	CSPTime	m_tmMin5To;
	//}}AFX_DATA

protected:
	BOOL			m_bStopRefresh;
	DOWNLOAD_INFO *	m_pDownloadInfo;
	CNetDatabase	m_netdb;
	CStringArray	m_astrErrorPac;

	CSPTime			m_tmLatestOld;

	BOOL			m_bDBModified;

	// Init download time
	BOOL			m_bInitDownloadTime;
	CSPTime			m_tmDLBegin;
	CSPTime			m_tmDLEnd;
	BOOL			m_bAutoRun;

	// report
	UINT			m_nReportCount;
    BOOL			m_bReportFinished;
	UINT			m_nReportTotal;
	UINT			m_nReportRequestSent;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitComboServers( );

	// Generated message map functions
	//{{AFX_MSG(CDownloadDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRefreshServers();
	afx_msg void OnStopRefresh();
	afx_msg void OnStop();
	afx_msg void OnSetfocusTimeDay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnProxySet();
	afx_msg void OnSetfocusTimeMin5(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnDownloadMessage( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnDownloadProgress( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnDownloadEnd( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverData( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADDLG_H__047CD860_CF70_45E7_BC04_CB594D31D679__INCLUDED_)
