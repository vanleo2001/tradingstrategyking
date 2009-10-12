#if !defined(AFX_DATASRCDLG_H__97A1D12F_AA5B_4084_BDC5_452E47D37602__INCLUDED_)
#define AFX_DATASRCDLG_H__97A1D12F_AA5B_4084_BDC5_452E47D37602__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSrcDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataSrcDlg dialog
/***
	指定附加数据源对话框
*/
class CDataSrcDlg : public CXFDialog
{
// Construction
public:
	CDataSrcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataSrcDlg)
	enum { IDD = IDD_DATASOURCE };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CButton	m_btnQLTest;
	CButton	m_btnQLExplorer;
	CEdit	m_editQLPath;
	CButton	m_btnCheckQianlong;
	CString	m_strQLPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSrcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataSrcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckQianlong();
	afx_msg void OnQlexplorer();
	virtual void OnOK();
	afx_msg void OnQltest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASRCDLG_H__97A1D12F_AA5B_4084_BDC5_452E47D37602__INCLUDED_)
