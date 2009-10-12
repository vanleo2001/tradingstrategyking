#if !defined(AFX_SIMUADDOPDLG_H__82DA25A4_80FD_4806_8C77_28C11072814D__INCLUDED_)
#define AFX_SIMUADDOPDLG_H__82DA25A4_80FD_4806_8C77_28C11072814D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSimuAddOpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimuAddOpDlg dialog
/***
	策略的实战操作中，加入操作对话框
*/
class CSimuAddOpDlg : public CXFDialog
{
// Construction
public:
	CSimuAddOpDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetStrategy( CStrategy * pStrategy );

// Dialog Data
	//{{AFX_DATA(CSimuAddOpDlg)
	enum { IDD = IDD_SIMUADDOP };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CComboBox	m_comboOperate;
	CSearchBox	m_comboStock;
	CSPTime	m_time;
	CString	m_strShare;
	CString	m_strSharePrice;
	CString	m_strRateCost;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuAddOpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStrategy		*		m_pStrategy;

	void	AutoSetRateCost( );

	// Generated message map functions
	//{{AFX_MSG(CSimuAddOpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeShare();
	afx_msg void OnChangeShareprice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUADDOPDLG_H__82DA25A4_80FD_4806_8C77_28C11072814D__INCLUDED_)
