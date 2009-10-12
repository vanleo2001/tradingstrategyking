#if !defined(AFX_STRATEGYVIEW_H__C73CC5D8_5B91_11D2_AB9F_441100C10000__INCLUDED_)
#define AFX_STRATEGYVIEW_H__C73CC5D8_5B91_11D2_AB9F_441100C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StrategyView.h : header file
//

#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CStrategyView view
#define	WM_USER_ADDSTRATEGY			(WM_USER+1001)
#define	WM_USER_REMOVESTRATEGY		(WM_USER+1002)
#define	WM_USER_UPDATESTRATEGY		(WM_USER+1003)
#define	WM_USER_ACTIVATESTRATEGY	(WM_USER+1004)

class	CStrategy;
/***
	²ßÂÔÊ÷ÊÓÍ¼
*/
class CStrategyView : public CTreeView
{
protected:
	CStrategyView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStrategyView)

// Attributes
public:
	CImageList m_ImageList;

// Operations
public:
	CStrategy * GetActiveStrategy( );
	void	SetActiveStrategy( CStrategy * pStrategy );
	BOOL	ActivateStrategy( CStrategy * pActiveStrategy, HTREEITEM hStrategy = NULL );

protected:
	CStrategy	*	GetStrategyByItem( HTREEITEM hItem );
	HTREEITEM	GetItemByStrategy( CStrategy * pCurStrategy );
	HTREEITEM	AddStrategy( CStrategy * pCurStrategy );
	void		UpdateStrategy( CStrategy * pCurStrategy );
	void		ExecuteStrategy( CStrategy * pStrategy, DWORD dwData );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrategyView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStrategyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStrategy	*	m_pActiveStrategy;

	// Generated message map functions
protected:
	//{{AFX_MSG(CStrategyView)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnAddStrategy( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnRemoveStrategy( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUpdateStrategy( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnActivateStrategy( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRATEGYVIEW_H__C73CC5D8_5B91_11D2_AB9F_441100C10000__INCLUDED_)
