// MultiSortView.h : interface of the CMultiSortView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTISORTVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
#define AFX_MULTISORTVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MultiSort.h"
/***
	◊€∫œ≈≈√˚Õº
*/
class CMultiSortView : public CView
{
protected: // create from serialization only
	CMultiSortView();
	DECLARE_DYNCREATE(CMultiSortView)

// Attributes
	CMultiSort	m_multisort;

public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }

	void	SetMultiSortClass( LONG type )	{	m_multisort.SetMultiSortClass(type);	}
	LONG	GetMultiSortClass( )			{	return m_multisort.GetMultiSortClass();	}

	void	SendRequestQuote( BOOL bForced );
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSortView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMultiSortView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMultiSortView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
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

#endif // !defined(AFX_MULTISORTVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
