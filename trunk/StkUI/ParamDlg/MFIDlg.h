#if !defined(AFX_MFIDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
#define AFX_MFIDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MFIDlg.h : header file
//
#include "SetParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CMFIDlg dialog

class CMFIDlg : public CParamDlg
{
// Construction
public:
	CMFIDlg(CWnd* pParent,CMFI *pMFI);   // standard constructor

	CMFI	*	m_pMFI;
	virtual	BOOL	RefreshData( BOOL bSaveAndValidate );

// Dialog Data
	//{{AFX_DATA(CMFIDlg)
	enum { IDD = IDDP_MFI };
	CIntensityComboBox	m_cmbShort;
	CIntensityComboBox	m_cmbLong;
	CStatic	m_staticName;
	long	m_nDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMFIDlg)
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

#endif // !defined(AFX_MFIDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
