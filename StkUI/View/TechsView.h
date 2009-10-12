#if !defined(AFX_TECHSVIEW_H__E60E3370_AF1C_4E08_93AB_1E225598FFB3__INCLUDED_)
#define AFX_TECHSVIEW_H__E60E3370_AF1C_4E08_93AB_1E225598FFB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TechsView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTechsView view

#define	IMG_TECHCLASS		0
#define	IMG_TECHCLASS_SEL	0
#define	IMG_TECH			1
#define	IMG_TECH_SEL		1
/***
	技术指标树视图
*/
class CTechsView : public CTreeView
{
protected:
	CTechsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTechsView)

// Attributes
public:
	CImageList m_ImageList;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTechsView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTechsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CTechsView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHSVIEW_H__E60E3370_AF1C_4E08_93AB_1E225598FFB3__INCLUDED_)
