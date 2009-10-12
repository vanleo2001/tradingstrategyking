#if !defined(AFX_NETINFODLG_H__27E2D507_6A94_4DAA_8C99_FDD658EAB62B__INCLUDED_)
#define AFX_NETINFODLG_H__27E2D507_6A94_4DAA_8C99_FDD658EAB62B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetInfoDlg dialog
/***
	显示信息对话框
*/
class CNetInfoDlg : public CXFDialog
{
// Construction
public:
	CNetInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetInfoDlg)
	enum { IDD = IDD_NETINFO };
	CButton	m_btnCancel;
	CHyperLink	m_linkNetInfo;
	CEdit	m_editExplain;
	BOOL	m_bNotShowLater;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETINFODLG_H__27E2D507_6A94_4DAA_8C99_FDD658EAB62B__INCLUDED_)
