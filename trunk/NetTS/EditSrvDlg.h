#if !defined(AFX_EDITSRVDLG_H__D3E17091_C46A_4D95_A849_99F5898B06E4__INCLUDED_)
#define AFX_EDITSRVDLG_H__D3E17091_C46A_4D95_A849_99F5898B06E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditSrvDlg.h : header file
//

#include "QSProfile.h"

/////////////////////////////////////////////////////////////////////////////
// CEditSrvDlg dialog

class CEditSrvDlg : public CDialog
{
// Construction
public:
	CEditSrvDlg(CWnd* pParent = NULL);   // standard constructor

	void SetQuoteServer( CQuoteServer & qs );
	void GetQuoteServer( CQuoteServer & qs );

// Dialog Data
	//{{AFX_DATA(CEditSrvDlg)
	enum { IDD = IDD_EDITSERVER };
	CString	m_strName;
	CString	m_strAddress;
	UINT	m_nPort;
	CString	m_strUser;
	CString	m_strPasswd;
	BOOL	m_bSavePasswd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSrvDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditSrvDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSRVDLG_H__D3E17091_C46A_4D95_A849_99F5898B06E4__INCLUDED_)
