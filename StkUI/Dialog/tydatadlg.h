#if !defined(AFX_TYDATADLG_H__AC6DF032_B8CB_47A6_B8F9_EC962EE24A33__INCLUDED_)
#define AFX_TYDATADLG_H__AC6DF032_B8CB_47A6_B8F9_EC962EE24A33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TyDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTyDataDlg dialog

class CTyDataDlg : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CTyDataDlg)

// Construction
public:
	CTyDataDlg( );   // standard constructor

	void	StoreProfile( );
	void	LoadProfile( );

// Dialog Data
	//{{AFX_DATA(CTyDataDlg)
	enum { IDD = IDD_TYDATA };
	CButton	m_btnSrcSZExplorer;
	CButton	m_btnSrcSHExplorer;
	CButton	m_btnFileExplorer;
	CButton	m_btnStart;
	CStatic	m_staticProgress;
	CProgressCtrl	m_ctrlProgress;
	CString	m_strFileDest;
	CSPTime	m_tmBegin;
	CSPTime	m_tmEnd;
	CString	m_strPathSrcSH;
	CString	m_strPathSrcSZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTyDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTyDataDlg)
	afx_msg void OnFileExplorer();
	virtual BOOL OnInitDialog();
	afx_msg void OnSrcshExplorer();
	afx_msg void OnSrcszExplorer();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYDATADLG_H__AC6DF032_B8CB_47A6_B8F9_EC962EE24A33__INCLUDED_)
