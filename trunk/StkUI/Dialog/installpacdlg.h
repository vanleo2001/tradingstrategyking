#if !defined(AFX_INSTALLPACDLG_H__5678D47A_D1DE_42C4_875A_A911D8D1173C__INCLUDED_)
#define AFX_INSTALLPACDLG_H__5678D47A_D1DE_42C4_875A_A911D8D1173C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InstallPacDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInstallPacDlg dialog

#define	WM_USER_INSTALL_MESSAGE		(WM_USER+1001)
#define	WM_USER_INSTALL_PROGRESS	(WM_USER+1002)
#define	WM_USER_INSTALL_END			(WM_USER+1003)

typedef	struct install_info_t	{
	HWND				hMainWnd;
	CNetDatabase	*	pNetDB;
	int					nType;
	CStringArray	*	pStringArray;
} INSTALL_INFO, * LPINSTALL_INFO;

/***
	安装下载数据包对话框
*/
class CInstallPacDlg : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CInstallPacDlg)

// Construction
public:
	CInstallPacDlg( );   // standard constructor
	virtual	~CInstallPacDlg( );

	static HANDLE m_hEventKillInstallThread;
	static HANDLE m_hEventInstallThreadKilled;

	void	OnCompleted( );

// Dialog Data
	//{{AFX_DATA(CInstallPacDlg)
	enum { IDD = IDD_INSTALLPACKAGE };
	CStatic	m_staticProgress;
	CButton	m_btnStart;
	CButton	m_btnStop;
	CButton	m_btnRemove;
	CButton	m_btnExplorer;
	CListCtrl	m_listFiles;
	CProgressCtrl	m_ctrlProgress;
	CStatic	m_staticStatus;
	CHyperLink	m_linkGoWeb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstallPacDlg)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_imageList;
	CStringArray	m_astrPacFiles;
	CStringArray	m_astrErrorPac;
	INSTALL_INFO *	m_pInstallInfo;
	CNetDatabase	m_netdb;

	CSPTime			m_tmLatestOld;
	BOOL			m_bCodetableInstalled;

	BOOL	m_bDBModified;

	// Generated message map functions
	//{{AFX_MSG(CInstallPacDlg)
	afx_msg void OnExplorer();
	afx_msg void OnRemove();
	afx_msg void OnDblclkListFiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStop();
	afx_msg void OnStart();
	//}}AFX_MSG
	afx_msg LRESULT OnInstallMessage( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnInstallProgress( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnInstallEnd( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTALLPACDLG_H__5678D47A_D1DE_42C4_875A_A911D8D1173C__INCLUDED_)
