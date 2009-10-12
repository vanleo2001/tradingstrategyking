#if !defined(AFX_EDITKDATADLG_H__96E110F8_E0E0_4B48_B337_077E547BBB4D__INCLUDED_)
#define AFX_EDITKDATADLG_H__96E110F8_E0E0_4B48_B337_077E547BBB4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditKDATADlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditKDATADlg dialog
/***
	编辑K线数据对话框
*/
class CEditKDATADlg : public CXFDialog
{
// Construction
public:
	CEditKDATADlg(CWnd* pParent = NULL);   // standard constructor

	void	SetKDATA( KDATA kd, int nKType );
	KDATA	GetKDATA( );
// Dialog Data
	//{{AFX_DATA(CEditKDATADlg)
	enum { IDD = IDD_EDITKDATA };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CSPTime	m_tmDate;
	CString	m_strOpen;
	CString	m_strHigh;
	CString	m_strLow;
	CString	m_strClose;
	DWORD	m_dwVolume;
	DWORD	m_dwAmount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditKDATADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	KDATA	m_kd;
	int		m_nKType;

	// Generated message map functions
	//{{AFX_MSG(CEditKDATADlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITKDATADLG_H__96E110F8_E0E0_4B48_B337_077E547BBB4D__INCLUDED_)
