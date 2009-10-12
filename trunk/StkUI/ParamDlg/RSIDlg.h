#if !defined(AFX_RSIDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
#define AFX_RSIDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RSIDlg.h : header file
//
#include "SetParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CRSIDlg dialog

class CRSIDlg : public CParamDlg
{
// Construction
public:
	CRSIDlg(CWnd* pParent,CRSI *pRSI);   // standard constructor

	CRSI	*	m_pRSI;
	virtual	BOOL	RefreshData( BOOL bSaveAndValidate );

// Dialog Data
	//{{AFX_DATA(CRSIDlg)
	enum { IDD = IDDP_RSI };
	CIntensityComboBox	m_cmbSold;
	CIntensityComboBox	m_cmbDeadFork;
	CIntensityComboBox	m_cmbGoldenFork;
	CStatic	m_staticName;
	long	m_nDays1;
	long	m_nDays2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRSIDlg)
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

#endif // !defined(AFX_RSIDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
