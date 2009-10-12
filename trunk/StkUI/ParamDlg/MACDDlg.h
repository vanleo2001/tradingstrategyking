#if !defined(AFX_MACDDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
#define AFX_MACDDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MACDDlg.h : header file
//
#include "SetParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CMACDDlg dialog

class CMACDDlg : public CParamDlg
{
// Construction
public:
	CMACDDlg(CWnd* pParent,CMACD *pMACD);   // standard constructor

	CMACD	*	m_pMACD;
	virtual	BOOL	RefreshData( BOOL bSaveAndValidate );

// Dialog Data
	//{{AFX_DATA(CMACDDlg)
	enum { IDD = IDDP_MACD };
	CIntensityComboBox	m_cmbDeadFork;
	CIntensityComboBox	m_cmbGoldenFork;
	CIntensityComboBox	m_cmbDeviateOnTop;
	CIntensityComboBox	m_cmbDeviateOnBottom;
	CStatic	m_staticName;
	long	m_nEMADays1;
	long	m_nEMADays2;
	long	m_nDIFDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMACDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMACDDlg)
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

#endif // !defined(AFX_MACDDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
