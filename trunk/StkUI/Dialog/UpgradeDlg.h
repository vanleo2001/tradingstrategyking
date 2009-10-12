#if !defined(AFX_UPGRADEDLG_H__2CA32914_5418_4E65_84FB_61198AA1CF62__INCLUDED_)
#define AFX_UPGRADEDLG_H__2CA32914_5418_4E65_84FB_61198AA1CF62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpgradeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpgradeDlg dialog

#define	WM_USER_UPGRADE_MESSAGE		(WM_USER+1001)
#define	WM_USER_UPGRADE_PROGRESS	(WM_USER+1002)
#define	WM_USER_UPGRADE_END			(WM_USER+1003)

typedef	struct upgrade_info_t	{
	HWND				hMainWnd;
} UPGRADE_INFO, * LPUPGRADE_INFO;
/***
	自动升级对话框
*/
class CUpgradeDlg : public CXFDialog
{
// Construction
public:
	CUpgradeDlg(CWnd* pParent = NULL);   // standard constructor
	~CUpgradeDlg( );

	void	SetAutoRun( BOOL bAutoRun = TRUE );

	static HANDLE m_hEventKillUpgradeThread;
	static HANDLE m_hEventUpgradeThreadKilled;
	static HFILE m_hFileUpgrade;

	UPGRADE_INFO *	m_pUpgradeInfo;

// Dialog Data
	//{{AFX_DATA(CUpgradeDlg)
	enum { IDD = IDD_AUTOUPGRADE };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CStatic	m_staticStatus;
	CStatic	m_staticProgress;
	CButton	m_btnProxySet;
	CEdit	m_editExplain;
	CProgressCtrl	m_ctrlProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgradeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL		m_bAutoRun;

	// Generated message map functions
	//{{AFX_MSG(CUpgradeDlg)
	virtual BOOL OnInitDialog();
public:
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnProxySet();
	//}}AFX_MSG
	afx_msg LRESULT OnUpgradeMessage( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUpgradeProgress( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUpgradeEnd( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPGRADEDLG_H__2CA32914_5418_4E65_84FB_61198AA1CF62__INCLUDED_)
