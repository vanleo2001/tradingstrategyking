#if !defined(AFX_SETCOLUMNDLG_H__812A1C1F_A53C_45B9_A5B9_21794C33EBDB__INCLUDED_)
#define AFX_SETCOLUMNDLG_H__812A1C1F_A53C_45B9_A5B9_21794C33EBDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetColumnDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColListCtrl window
/***
	设定显示列的ListCtrl
*/
class CColListCtrl : public CListCtrl
{
// Construction
public:
	CColListCtrl();

// Attributes
public:

// Operations
public:
	int			SetComboBoxColumn( int nCol );
	int			SetItemStrings( int nItem, CStringArray &astr );

	BOOL		CreateComboBox( int nItem );
	int			ShowItemComboBox( int nItem );
	int			HideItemComboBox( );

protected:
	int			m_nComboBoxColumn;
	CPtrArray	m_aptrItemStrings;
	CComboBox	m_comboBox;
	int			m_nCurItem;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColListCtrl)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColListCtrl)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSetColumnDlg dialog
/***
	设置股票列表显示列的对话框
*/
class CSetColumnDlg : public CXFDialog
{
// Construction
public:
	CSetColumnDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetColumnDlg)
	enum { IDD = IDD_SETCOLUMN };
	CButton	m_btnCancel;
	CButton	m_btnSetDefault;
	CButton	m_btnNewIndex;
	CButton	m_btnMoveUp;
	CButton	m_btnMoveDown;
	CButton	m_btnSetAll;
	CButton	m_btnOK;
	CButton	m_btnDelIndex;
	CButton	m_btnEditIndex;
	CColListCtrl	m_listColumn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetColumnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL	m_nYieldAverageDaysOld;
	BOOL	m_bUserIndexModified;
	void	SetListColumnItems( CSPDWordArray & anShow, BOOL bSetDefault );

	// Generated message map functions
	//{{AFX_MSG(CSetColumnDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	afx_msg void OnNewindex();
	afx_msg void OnEditindex();
	afx_msg void OnDelindex();
	virtual void OnOK();
	afx_msg void OnItemchangedColumnlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetdefault();
	afx_msg void OnDblclkColumnlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCOLUMNDLG_H__812A1C1F_A53C_45B9_A5B9_21794C33EBDB__INCLUDED_)
