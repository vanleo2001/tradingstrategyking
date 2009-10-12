#if !defined(AFX_MADLG_H__5B0315E3_9787_4CDF_AA23_7A9C2E67DFBB__INCLUDED_)
#define AFX_MADLG_H__5B0315E3_9787_4CDF_AA23_7A9C2E67DFBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MADlg.h : header file
//

#include "SetParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMADlg dialog

class CMADlg : public CParamDlg
{
// Construction
public:
	CMADlg(CWnd* pParent,CMA * pMA);   // standard constructor

	CMA			*	m_pMA;
	virtual	BOOL	RefreshData( BOOL bSaveAndValidate );

// Dialog Data
	//{{AFX_DATA(CMADlg)
	enum { IDD = IDDP_MA };
	CIntensityComboBox	m_cmbShort;
	CIntensityComboBox	m_cmbLong;
	CIntensityComboBox	m_cmbDeadFork;
	CIntensityComboBox	m_cmbGoldenFork;
	CStatic	m_staticName;
	BOOL	m_bMA1;
	BOOL	m_bMA2;
	BOOL	m_bMA3;
	BOOL	m_bMA4;
	long	m_nDays1;
	long	m_nDays2;
	long	m_nDays3;
	long	m_nDays4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMADlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDefault();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MADLG_H__5B0315E3_9787_4CDF_AA23_7A9C2E67DFBB__INCLUDED_)
