#if !defined(AFX_SETGROUPDLG_H__28A5314F_FA8C_4F1F_A80C_6803B63D2340__INCLUDED_)
#define AFX_SETGROUPDLG_H__28A5314F_FA8C_4F1F_A80C_6803B63D2340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDomainComboBox window
/***
	显示板块或分组的ComboBox
*/
class CDomainComboBox : public CComboBox
{
// Construction
public:
	CDomainComboBox();

// Operations
public:
	int		InitStrings( BOOL bHasClass, BOOL bHasDomain, CDomainContainer &groups );

	int		AddGroup( CString strGroup );
	BOOL	DeleteCurSel( );
	int		GetCurSelType( );
	CString	GetCurSelName( );
	BOOL	SelectGroupAll( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDomainComboBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDomainComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDomainComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDomainListBox window
/***
	显示板块或分组的股票的ListBox
*/
class CDomainListBox : public CListBox
{
// Construction
public:
	CDomainListBox();

// Operations
public:
	BOOL	SetCurrentStocks( int nType, LPCTSTR lpszDomain, CDomainContainer & groups );
	BOOL	SelectCode( int nStart, CString strText );
	BOOL	SelectShortname( int nStart, CString strText );
	BOOL	SelectUp( );
	BOOL	SelectDown( );
	BOOL	SelectPrior( );
	BOOL	SelectNext( );
	int		GetCountPerPage( );
	BOOL	SelectAndShowItem( int nSel );
	int		SelectItem( int nSel, BOOL bSelected = TRUE );
	int		GetSelected( int nItem );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDomainListBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDomainListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDomainListBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSetGroupDlg dialog
/***
	设置分组的对话框
*/
class CSetGroupDlg : public CXFDialog
{
// Construction
public:
	CSetGroupDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetInitialGroup( CString strGroupName );
	void	AddtoGroup( CSPStringArray & astrStocks );

// Dialog Data
	//{{AFX_DATA(CSetGroupDlg)
	enum { IDD = IDD_SETGROUP };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CStatic	m_staticInfo;
	CButton	m_btnRemoveAll;
	CButton	m_btnRemoveSelected;
	CButton	m_btnAddAll;
	CButton	m_btnAddSelected;
	CButton	m_btnDeleteGroup;
	CButton	m_btnAddGroup;
	CDomainListBox	m_listStockDest;
	CDomainComboBox	m_comboGroupDest;
	CDomainListBox	m_listStockSrc;
	CDomainComboBox	m_comboGroupSrc;
	CEdit	m_editCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetGroupDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDomainContainer	m_grpTemp;

	CString				m_strInitialGroupName;
	CSPStringArray		m_astrInitialGroupStocks;

	// Generated message map functions
	//{{AFX_MSG(CSetGroupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddgroup();
	afx_msg void OnDeletegroup();
	afx_msg void OnAddselected();
	afx_msg void OnAddall();
	afx_msg void OnRemoveselected();
	afx_msg void OnRemoveall();
	afx_msg void OnChangeEditcode();
	afx_msg void OnSelchangeComboGroupsrc();
	afx_msg void OnSelchangeComboGroupdest();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkListStocksrc();
	afx_msg void OnDblclkListStockdest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETGROUPDLG_H__28A5314F_FA8C_4F1F_A80C_6803B63D2340__INCLUDED_)
