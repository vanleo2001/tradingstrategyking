#if !defined(AFX_SLISTCOMBOS_H__6673D747_929F_451B_BB82_EFD0F22B8E37__INCLUDED_)
#define AFX_SLISTCOMBOS_H__6673D747_929F_451B_BB82_EFD0F22B8E37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SListCombos.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDateComboBox window
/***
	选择股票列表显示日期的ComboBox
*/
class CDateComboBox : public CComboBox
{
// Construction
public:
	CDateComboBox();

	BOOL	InitDates( );
	void	SetCurrentWindowText( );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDateComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDateComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelendok();
	afx_msg void OnSelendcancel();
	afx_msg void OnKillfocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSearchBox window

/***
	输入股票代码或者汉语简拼，显示股票指标的ComboBox
*/
class CSearchBox : public CComboBox
{
// Construction
public:
	CSearchBox();

	void	SetAutoHide( BOOL bAutoHide = FALSE );
	BOOL	InitStocks( BOOL bHasSTTTech, BOOL bShowOnSel, BOOL bDuplicate );
	BOOL	InitStocks( CSPStringArray & astocks, BOOL bHasSTTTech, BOOL bShowOnSel, BOOL bDuplicate );
	CString	GetSelectedStockCode( );
	void	SetCurrentWindowText( );

// Attributes
protected:
	HWND		m_hwndLastFocus;
	BOOL		m_bSegmentEnd;

	BOOL		m_bShowOnSel;
	BOOL		m_bDoNothing;

	BOOL		m_bAutoHide;

// Operations
public:
	static	BOOL	IsWantChar( int nVirtKey );
	void	OnChangeStatus( int nVirtKey, long lParam, BOOL bFocused );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSearchBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSearchBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillfocus();
	afx_msg void OnEditchange();
	afx_msg void OnSelendok();
	afx_msg void OnSelendcancel();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLISTCOMBOS_H__6673D747_929F_451B_BB82_EFD0F22B8E37__INCLUDED_)
