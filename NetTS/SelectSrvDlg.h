#if !defined(AFX_SELECTSRVDLG_H__5B1B6E48_7583_4DF6_82F3_C4BAEBC42891__INCLUDED_)
#define AFX_SELECTSRVDLG_H__5B1B6E48_7583_4DF6_82F3_C4BAEBC42891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectSrvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectSrvDlg dialog

class CSelectSrvDlg : public CDialog
{
// Construction
public:
	CSelectSrvDlg(CWnd* pParent = NULL);   // standard constructor

	CString GetServer() { return m_strAddress; }
	UINT GetPort() { return m_nPort; }
	CString GetUser() { return m_strUser; }
	CString GetPasswd() { return m_strPasswd; }

protected:
	CString	m_strAddress;
	UINT	m_nPort;

	void SetupComboServer( );
// Dialog Data
	//{{AFX_DATA(CSelectSrvDlg)
	enum { IDD = IDD_SELECTSERVER };
	CButton	m_btnOK;
	CComboBox	m_comboServer;
	CString	m_strUser;
	CString	m_strPasswd;
	BOOL	m_bSavePasswd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectSrvDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectSrvDlg)
	afx_msg void OnSelchangeComboServer();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	virtual void OnOK();
	afx_msg void OnProxySet();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSRVDLG_H__5B1B6E48_7583_4DF6_82F3_C4BAEBC42891__INCLUDED_)
