#if !defined(AFX_VROCDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
#define AFX_VROCDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VROCDlg.h : header file
//
#include "SetParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CVROCDlg dialog

class CVROCDlg : public CParamDlg
{
// Construction
public:
	CVROCDlg(CWnd* pParent,CVROC *pVROC);   // standard constructor

	CVROC	*	m_pVROC;
	virtual	BOOL	RefreshData( BOOL bSaveAndValidate );

// Dialog Data
	//{{AFX_DATA(CVROCDlg)
	enum { IDD = IDDP_VROC };
	CIntensityComboBox	m_cmbDeviateOnTop;
	CIntensityComboBox	m_cmbDeviateOnBottom;
	CStatic	m_staticName;
	long	m_nDays;
	long	m_nMADays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVROCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVROCDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDefault();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VROCDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
