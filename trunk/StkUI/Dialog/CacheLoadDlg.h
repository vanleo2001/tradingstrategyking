#if !defined(AFX_CACHELOADDLG_H__D0A89D8F_4158_4169_BF4B_1A077EB66474__INCLUDED_)
#define AFX_CACHELOADDLG_H__D0A89D8F_4158_4169_BF4B_1A077EB66474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CacheLoadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCacheLoadDlg dialog

class CCacheLoadDlg : public CXFDialog
{
// Construction
public:
	CCacheLoadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCacheLoadDlg)
	enum { IDD = IDD_CACHELOAD };
	CSPTime	m_tmDate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCacheLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCacheLoadDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CACHELOADDLG_H__D0A89D8F_4158_4169_BF4B_1A077EB66474__INCLUDED_)
