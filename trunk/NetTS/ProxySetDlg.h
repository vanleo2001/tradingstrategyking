#if !defined(AFX_PROXYSETDLG_H__AA909DBB_5657_40C4_9055_1D3901E11343__INCLUDED_)
#define AFX_PROXYSETDLG_H__AA909DBB_5657_40C4_9055_1D3901E11343__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProxySetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProxySetDlg dialog
/***
	代理设置对话框
*/
class CProxySetDlg : public CDialog
{
// Construction
public:
	CProxySetDlg(CWnd* pParent = NULL);   // standard constructor

	void EnableWindows( BOOL bEnable );

// Dialog Data
	//{{AFX_DATA(CProxySetDlg)
	enum { IDD = IDD_PROXYSET };
	CEdit	m_editProxyPasswd;
	CEdit	m_editProxyUser;
	CStatic	m_staticProxyPasswd;
	CStatic	m_staticProxyUser;
	CEdit	m_editProxyPort;
	CEdit	m_editProxyAddress;
	CStatic	m_staticProxyPort;
	CStatic	m_staticProxyAddress;
	CString	m_strProxyAddress;
	UINT	m_nProxyPort;
	CString	m_strProxyUser;
	CString	m_strProxyPasswd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProxySetDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProxySetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDirect();
	afx_msg void OnPreconfig();
	afx_msg void OnProxy();
	virtual void OnOK();
	afx_msg void OnProxySocks4();
	afx_msg void OnProxySocks5();
	afx_msg void OnProxyHttp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROXYSETDLG_H__AA909DBB_5657_40C4_9055_1D3901E11343__INCLUDED_)
