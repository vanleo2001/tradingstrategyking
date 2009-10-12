#if !defined(AFX_SELECTTRADERDLG_H__9B3C0B0D_C357_4A92_80C4_5B8661756302__INCLUDED_)
#define AFX_SELECTTRADERDLG_H__9B3C0B0D_C357_4A92_80C4_5B8661756302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectTraderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectTraderDlg dialog

class CSelectTraderDlg : public CXFDialog
{
// Construction
public:
	CSelectTraderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectTraderDlg)
	enum { IDD = IDD_SELECTTRADER };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CButton	m_btnExplore;
	CString	m_strFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectTraderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectTraderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFileExplorer();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTTRADERDLG_H__9B3C0B0D_C357_4A92_80C4_5B8661756302__INCLUDED_)
