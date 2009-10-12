#if !defined(AFX_SETBASEDLG_H__B480DF8D_68D7_4F3B_A78F_4F3AAED894FC__INCLUDED_)
#define AFX_SETBASEDLG_H__B480DF8D_68D7_4F3B_A78F_4F3AAED894FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetBaseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg dialog
#define	WM_USER_INSTALLBASEPROGRESS	WM_USER+1001

class CSetBaseDlg : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CSetBaseDlg)

// Construction
public:
	CSetBaseDlg();
	~CSetBaseDlg();

// Dialog Data
	//{{AFX_DATA(CSetBaseDlg)
	enum { IDD = IDD_SETBASE };
	CButton	m_btnFxjBlockPathExplorer;
	CButton	m_btnF10FileExplorer;
	CButton	m_btnFxjFileExplorer;
	CButton	m_btnTdxFileExplorer;
	CButton	m_btnStart;
	CStatic	m_staticProgress;
	CProgressCtrl	m_ctrlProgress;
	CString	m_strFileTdx;
	CString	m_strFileFxj;
	CString	m_strPathFxjBlock;
	CString	m_strFileF10;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSetBaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetBaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnF10fileExplorer();
	afx_msg void OnStart();
	afx_msg void OnTdxfileExplorer();
	afx_msg void OnFxjfileExplorer();
	afx_msg void OnFxjblockpathExplorer();
	//}}AFX_MSG
	afx_msg LRESULT OnInstallBaseProgress( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBASEDLG_H__B480DF8D_68D7_4F3B_A78F_4F3AAED894FC__INCLUDED_)
