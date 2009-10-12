#if !defined(AFX_INFOVIEW_H__5930176C_20CB_425B_9B05_FC7B08253945__INCLUDED_)
#define AFX_INFOVIEW_H__5930176C_20CB_425B_9B05_FC7B08253945__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoView html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>
/***
	在线资讯视图
*/
class CInfoView : public CHtmlView
{
protected:
	CInfoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInfoView)

	enum infoServers	{
		serverUnknown		=	0x00,
		serverMin			=	0x01,
		serverSelf			=	0x02,
		serverF10			=	0x03,
		serverStockstar		=	0x04,
		serverSohu			=	0x05,
		serverSina			=	0x06,
		server163			=	0x07,
		serverShse			=	0x08,
		serverSzse			=	0x09,
		serverHkex			=	0x0A,
		serverLse			=	0x0B,
		serverNyse			=	0x0C,
		serverNasdaq		=	0x0D,
		serverP5w			=	0x0E,
		serverSsnews		=	0x0F,
		serverCs			=	0x10,
		serverMax			=	0x10
	};

// html Data
public:
	//{{AFX_DATA(CInfoView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }

// Operations
public:
	void	SetCurrentServer( int nServer );
	int		GetCurrentServer( );
	void	OnRefresh( );
	void	ShowInfo( CStockInfo & info );
	BOOL	IsViewF10( );

protected:
	int			m_nServer;
	CStockInfo	m_infoCurrent;
	int			m_nServerShowed;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CInfoView)
	afx_msg void OnViewF10();
	//}}AFX_MSG
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOVIEW_H__5930176C_20CB_425B_9B05_FC7B08253945__INCLUDED_)
