#if !defined(AFX_BASEINDEXDLG_H__610CB2A6_5BCB_4843_8F58_E7D743CC67D9__INCLUDED_)
#define AFX_BASEINDEXDLG_H__610CB2A6_5BCB_4843_8F58_E7D743CC67D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseIndexDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseIndexDlg dialog
/***
	新建基本指标定义类。
*/
class CBaseIndexDlg : public CXFDialog
{
// Construction
public:
	CBaseIndexDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetIndexInfo( UINT	nID );
	CIndex	GetIndexInfo( );

// Dialog Data
	//{{AFX_DATA(CBaseIndexDlg)
	enum { IDD = IDD_BASEINDEX };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CMagicEdit	m_editIndexExpress;
	CString	m_strIndexName;
	CString	m_strIndexExpress;
	BOOL	m_bAddToList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseIndexDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CIndex	m_index;

	// Generated message map functions
	//{{AFX_MSG(CBaseIndexDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEINDEXDLG_H__610CB2A6_5BCB_4843_8F58_E7D743CC67D9__INCLUDED_)
