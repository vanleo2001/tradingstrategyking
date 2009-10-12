#if !defined(AFX_VOLUMEDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
#define AFX_VOLUMEDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VOLUMEDlg.h : header file
//
#include "SetParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CVOLUMEDlg dialog

class CVOLUMEDlg : public CParamDlg
{
// Construction
public:
	CVOLUMEDlg(CWnd* pParent,CVOLUME *pVOLUME);   // standard constructor

	CVOLUME	*	m_pVOLUME;
	virtual	BOOL	RefreshData( BOOL bSaveAndValidate );

// Dialog Data
	//{{AFX_DATA(CVOLUMEDlg)
	enum { IDD = IDDP_VOLUME };
	CIntensityComboBox	m_cmbShort;
	CIntensityComboBox	m_cmbLong;
	CIntensityComboBox	m_cmbDeviateOnTop;
	CIntensityComboBox	m_cmbDeviateOnBottom;
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
	//{{AFX_VIRTUAL(CVOLUMEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVOLUMEDlg)
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

#endif // !defined(AFX_VOLUMEDLG_H__0BB26BA5_C279_4BDF_B7D7_729E3B3F0430__INCLUDED_)
