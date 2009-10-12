// BaseView.h : interface of the CBaseView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEVIEW_H__6DCA91CD_238F_4EFB_ABD3_023502A42EB1__INCLUDED_)
#define AFX_BASEVIEW_H__6DCA91CD_238F_4EFB_ABD3_023502A42EB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrm.h"
/***
	显示股票基本资料的视图
*/
class CBaseView : public CEditView
{
protected: // create from serialization only
	CBaseView();
	DECLARE_DYNCREATE(CBaseView)

// Attributes
public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }

// Operations
public:
	void	SetFont( LPLOGFONT pLogFont );
	
	void	BeginNewLine( );
	void	OutputMsgHeader( );
	void	OutputMsgLine( const char * line );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void Serialize(CArchive& ar);
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStringArray	m_astrMsg;
	CStock			m_stock;
	CFont			m_font;

// Generated message map functions
protected:
	//{{AFX_MSG(CBaseView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnEditChange();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverData( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEVIEW_H__6DCA91CD_238F_4EFB_ABD3_023502A42EB1__INCLUDED_)
