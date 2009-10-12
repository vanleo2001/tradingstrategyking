// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__AC8E720D_6379_11D2_9582_0040053E0F5B__INCLUDED_)
#define AFX_CHILDFRM_H__AC8E720D_6379_11D2_9582_0040053E0F5B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/***
	×Ó´°¿Ú¿ò¼ÜÀà
*/
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

	static	CChildFrame* CreateNewFrame( CStaticDoc* pStaticDoc );

// Attributes
public:
	//static	CEnTabCtrl	m_tabctrl;

// Operations
public:
	virtual	void OnUpdateFrameTitle(BOOL bAddToTitle);
	void	ResetClientPosition( BOOL bMaximized );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnClose();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__AC8E720D_6379_11D2_9582_0040053E0F5B__INCLUDED_)
