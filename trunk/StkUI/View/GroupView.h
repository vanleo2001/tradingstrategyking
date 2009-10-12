#if !defined(AFX_GROUPVIEW_H__E60E3370_AF1C_4E08_93AB_1E225598FFB3__INCLUDED_)
#define AFX_GROUPVIEW_H__E60E3370_AF1C_4E08_93AB_1E225598FFB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupView view

#define	WM_USER_UPDATEGROUPS		(WM_USER+1001)
/***
	板块和分组树视图
*/
class CGroupView : public CTreeView
{
protected:
	CGroupView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGroupView)

// Attributes
public:
	CImageList m_ImageList;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGroupView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CGroupView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateGroups( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPVIEW_H__E60E3370_AF1C_4E08_93AB_1E225598FFB3__INCLUDED_)
