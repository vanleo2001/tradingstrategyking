// TechsView.cpp : implementation file
//

#include "stdafx.h"

#include "TechsView.h"
#include "GraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTechsView

IMPLEMENT_DYNCREATE(CTechsView, CTreeView)

CTechsView::CTechsView()
{
}

CTechsView::~CTechsView()
{
}


BEGIN_MESSAGE_MAP(CTechsView, CTreeView)
	//{{AFX_MSG_MAP(CTechsView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTechsView drawing

void CTechsView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTechsView diagnostics

#ifdef _DEBUG
void CTechsView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTechsView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTechsView message handlers

void CTechsView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CTreeCtrl &treectrl = GetTreeCtrl();
	HTREEITEM	hItem	=	treectrl.GetSelectedItem();
	CString	strText	=	treectrl.GetItemText(hItem);
	
	if( NULL != hItem )
	{
		UINT	nTech	=	treectrl.GetItemData(hItem);
		if( (nTech >= STT_MIN && nTech <= STT_MAX) || nTech >= STT_USER_MIN )
		{
			AfxGetProfile().ShowGraphViewTech( nTech );
			CGraphView * pGraphView = AfxGetGraphView();
			CView * pActiveView = AfxGetStaticDoc()->GetActiveView();
			if( pGraphView )
				pGraphView->ResetClientRect( );
			if( pGraphView && pGraphView->GetSafeHwnd() == pActiveView->GetSafeHwnd() )
				pGraphView->Invalidate();
			else
				AfxSwitchToStaticView( RUNTIME_CLASS(CGraphView) );
			SetFocus( );
		}
	}
	
	*pResult = 0;
}

int CTechsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create the image list for the tree control
	CTreeCtrl &treectrl = GetTreeCtrl();
	m_ImageList.Create(IDB_TECHSVIEW_ICONS, 16, 1, RGB(0,255,0));
	treectrl.SetImageList (&m_ImageList, TVSIL_NORMAL);

	ModifyStyle( WS_BORDER | WS_DLGFRAME | WS_THICKFRAME, 0 );
	ModifyStyleEx( WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE | WS_EX_STATICEDGE, 0 );

	OnColorChange( 0, 0 );

	return 0;
}

BOOL CTechsView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

void CTechsView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CTreeView::OnWindowPosChanged(lpwndpos);

	ShowScrollBar( SB_HORZ, FALSE );
}

void CTechsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CTreeCtrl &treectrl = GetTreeCtrl();
	treectrl.DeleteAllItems( );

	CString	strClassLast;
	HTREEITEM	hTechClass = NULL, hTech = NULL, hTechClassCLK = NULL, hTechClassUser = NULL;
	UINT nTechUserCount = CTechUser::GetTechUserCount();
	for( UINT i=STT_MIN; i <= STT_MAX+nTechUserCount; i ++ )
	{
		UINT	nTech	=	i;
		if( nTech > STT_MAX )
			nTech	=	i-STT_MAX-1+STT_USER_MIN;

		CString	strClass	=	AfxGetSTTClassName(nTech);
		if( strClass.GetLength() > 0 && 0 != strClass.Compare(strClassLast) )
		{
			hTechClass = treectrl.InsertItem(strClass,IMG_TECHCLASS,IMG_TECHCLASS_SEL);
			treectrl.SetItemData( hTechClass, 0 );
			if( nTech >= STT_CLK_MIN && nTech <= STT_CLK_MAX )
				hTechClassCLK	=	hTechClass;
			if( nTech >= STT_USER_MIN )
				hTechClassUser	=	hTechClass;
		}

		strClassLast	=	strClass;

		if( NULL == hTechClass )
			hTechClass	=	TVI_ROOT;
		CString	strTechName	=	AfxGetSTTFullName(nTech);
		hTech	=	treectrl.InsertItem(strTechName,IMG_TECH,IMG_TECH_SEL,hTechClass);
		treectrl.SetItemData( hTech, nTech );
	}

	treectrl.Expand( hTechClassCLK, TVE_EXPAND );
	treectrl.Expand( hTechClassUser, TVE_EXPAND );
	treectrl.Select( hTechClassCLK, TVGN_CARET );
	treectrl.Select( treectrl.GetRootItem(), TVGN_FIRSTVISIBLE );

	CWnd * pParent = GetParent();
	if( pParent && ::IsWindow(pParent->GetSafeHwnd()) )
		pParent->Invalidate();
}

void CTechsView::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	//Invalidate( );
}

LRESULT CTechsView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	CTreeCtrl &treectrl = GetTreeCtrl();
	COLORREF	clrBK	=	AfxGetProfile().GetColor( CColorClass::clrWorkspBK );
	COLORREF	clrText	=	AfxGetProfile().GetColor( CColorClass::clrWorkspText );
	treectrl.SetBkColor( clrBK );
	treectrl.SetTextColor( clrText );
	
	//CWnd * pParent = GetParent();
	//if( pParent && ::IsWindow(pParent->GetSafeHwnd()) )
	//	pParent->SendMessage( WM_GUITABWND_COLORCHANGE, clrBK, clrText );
	
	Invalidate( );
	return 0;
}
