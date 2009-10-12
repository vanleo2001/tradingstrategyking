#if !defined(AFX_FILTERINFODLG_H__5ADD31D4_EA1D_4568_9EF9_2FAD346CAB16__INCLUDED_)
#define AFX_FILTERINFODLG_H__5ADD31D4_EA1D_4568_9EF9_2FAD346CAB16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterInfoDlg dialog

typedef	struct	FilterInfoCondition	{
	UINT	nVariantID;
	BOOL	bHasMoreThan;
	BOOL	bHasLessThan;
	double	dMoreThan;
	double	dLessThan;
}FILTERINFO_CONDITION;

/***
	基本资料和行情选股对话框
*/
class CFilterInfoDlg : public CXFDialog
{
// Construction
public:
	CFilterInfoDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL	InitListInfo( );

// Dialog Data
	//{{AFX_DATA(CFilterInfoDlg)
	enum { IDD = IDD_FILTERINFO };
	CButton	m_btnCancel;
	CButton	m_btnStart;
	CButton	m_btnAddToGroup;
	CButton	m_btnRemove;
	CButton	m_btnAdd;
	CStatic	m_staticTime;
	CButton	m_btnSznb;
	CButton	m_btnSzna;
	CButton	m_btnShab;
	CButton	m_btnShaa;
	CButton	m_btnAll;
	CProgressCtrl	m_ctrlProgress;
	CListBox	m_listResult;
	CListBox	m_listCond;
	CListBox	m_listInfo;
	CString	m_strMoreThan;
	CString	m_strLessThan;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListinfo();
	afx_msg void OnSelchangeListcond();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnAll();
	afx_msg void OnStart();
	afx_msg void OnAddtogroup();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERINFODLG_H__5ADD31D4_EA1D_4568_9EF9_2FAD346CAB16__INCLUDED_)
