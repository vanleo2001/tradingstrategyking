#if !defined(AFX_STARTUPDLG_H__5B1B6E48_7583_4DF6_82F3_C4BAEBC42891__INCLUDED_)
#define AFX_STARTUPDLG_H__5B1B6E48_7583_4DF6_82F3_C4BAEBC42891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StartupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartupDlg dialog
#define	WM_USER_STARTUP_PROGRESS	(WM_USER+1001)

class STKNET_API CStartupDlg : public CDialog
{
// Construction
public:
	CStartupDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL	SetBitmap( HBITMAP hbmp );
	void	SetLoadProgramFunc( BOOL (CALLBACK *pfnLoadProgram)(HWND hWnd,int nMinProgress,int nMaxProgress) );
	void	SetDisableOffline( BOOL bDisableOffline );
	void	SetProgress(int nPos);

	CString GetServer() { return m_strAddress; }
	UINT GetPort() { return m_nPort; }
	CString GetUser() { return m_strUser; }
	CString GetPasswd() { return m_strPasswd; }

protected:
	CString	m_strAddress;
	UINT	m_nPort;

	CBitmap	m_bitmap;
	BOOL (CALLBACK *m_pfnLoadProgram)(HWND hwnd, int nMinProgress, int nMaxProgress);
	BOOL	m_bDisableOffline;

	void SetupComboServer( );
// Dialog Data
	//{{AFX_DATA(CStartupDlg)
//	enum { IDD = IDD_STARTUP };
	CEdit	m_editUser;
	CEdit	m_editPasswd;
	CButton	m_btnCheckSavePasswd;
	CButton	m_btnModify;
	CButton	m_btnDelete;
	CButton	m_btnAdd;
	CButton	m_btnCancel;
	CButton	m_btnProxy;
	CStatic	m_staticInfo;
	CProgressCtrl	m_ctrlProgress;
	CStatic	m_staticPicture;
	CButton	m_btnOffline;
	CButton	m_btnOK;
	CComboBox m_comboServer;
	CString	m_strUser;
	CString	m_strPasswd;
	BOOL	m_bSavePasswd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartupDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartupDlg)
	afx_msg void OnSelchangeComboServer();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	virtual void OnOK();
	afx_msg void OnProxySet();
	virtual BOOL OnInitDialog();
	afx_msg void OnOffline();
	//}}AFX_MSG
	afx_msg LRESULT OnStartupProgress( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTUPDLG_H__5B1B6E48_7583_4DF6_82F3_C4BAEBC42891__INCLUDED_)
