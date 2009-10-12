#if !defined(AFX_SIMUADVANCEDDLG_H__7B600EF7_F7D1_45B7_A504_C29C529F3FC9__INCLUDED_)
#define AFX_SIMUADVANCEDDLG_H__7B600EF7_F7D1_45B7_A504_C29C529F3FC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuAdvancedDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimuAdvancedDlg dialog
/***
	策略的高级条件对话框
*/
class CSimuAdvancedDlg : public CXFDialog
{
// Construction
public:
	CSimuAdvancedDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL	m_bModified;

	void	SetStrategy( CStrategy * pStrategy );

// Dialog Data
	//{{AFX_DATA(CSimuAdvancedDlg)
	enum { IDD = IDD_SIMUADVANCED };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CButton	m_btnRuleLong;
	CButton	m_btnRuleIndex;
	CKTypeComboBox	m_cmbKTypeLong;
	CKTypeComboBox	m_cmbKTypeIndex;
	CButton	m_btnCheckLong;
	CButton	m_btnCheckIndex;
	CEdit	m_editStopProfit;
	CEdit	m_editStopLosing;
	CButton	m_btnCheckStopProfit;
	CButton	m_btnCheckStopLosing;
	CString	m_strStopLosing;
	CString	m_strStopProfit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuAdvancedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSimuAdvancedDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckstoplosing();
	afx_msg void OnCheckstopprofit();
	afx_msg void OnChecklong();
	afx_msg void OnCheckindex();
	afx_msg void OnRulelong();
	afx_msg void OnRuleindex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CStrategy	*	m_pStrategy;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUADVANCEDDLG_H__7B600EF7_F7D1_45B7_A504_C29C529F3FC9__INCLUDED_)
