// SelectorView.h : interface of the CSelectorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTORVIEW_H__579B4821_52B1_4009_BB3B_590A4D06C998__INCLUDED_)
#define AFX_SELECTORVIEW_H__579B4821_52B1_4009_BB3B_590A4D06C998__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class	CStaticDoc;

#include "../Dialog/SetGroupDlg.h"
/***
	股票列表视图
*/
class CSelectorView : public CFormView
{
protected: // create from serialization only
	CSelectorView();
	DECLARE_DYNCREATE(CSelectorView)

public:
	//{{AFX_DATA(CSelectorView)
	enum { IDD = IDD_SELECTOR_FORM };
	CStatic	m_staticInfo;
	CProgressCtrl	m_progress;
	CButton		m_btnRunSelector;
	CTechComboBox	m_cmbTech;
	CKTypeComboBox	m_cmbKType;
	CDomainComboBox	m_cmbStockGroup;
	//}}AFX_DATA

	// Grid
	CGridCtrl	m_Grid;

// Attributes
public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }
	BOOL			InitializeGrid( );
	void			SetFont( LPLOGFONT pLogFont );

// Operations
public:
	void			GetSelectedStocks( CSPStringArray & astr );
	void			OnDblclkItem( int nStockIndex );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectorView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSelectorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL	m_bShouldUpdate;
	BOOL	m_bRunning;

	CStockContainer	m_container;
	CUIntArray		m_signals;
	CUIntArray		m_reasons;


	// operation


// Generated message map functions
protected:
	//{{AFX_MSG(CSelectorView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFilePrint();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnRunselector();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCanCloseView( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTORVIEW_H__579B4821_52B1_4009_BB3B_590A4D06C998__INCLUDED_)
