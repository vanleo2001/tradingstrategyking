// GroupView.cpp : implementation file
//

#include "stdafx.h"
#include "GroupView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupView

#define	GV_ITEM_GROUP	1
#define	GV_ITEM_DOMAIN	2

#define	IMG_GROUPS		0
#define	IMG_GROUPS_SEL	0
#define	IMG_DOMAINS		1
#define	IMG_DOMAINS_SEL	1
#define	IMG_GROUP		2
#define	IMG_GROUP_SEL	2
#define	IMG_DOMAIN		3
#define	IMG_DOMAIN_SEL	3

IMPLEMENT_DYNCREATE(CGroupView, CTreeView)

CGroupView::CGroupView()
{
}

CGroupView::~CGroupView()
{
}


BEGIN_MESSAGE_MAP(CGroupView, CTreeView)
	//{{AFX_MSG_MAP(CGroupView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_UPDATEGROUPS, OnUpdateGroups)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupView drawing

void CGroupView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CGroupView diagnostics

#ifdef _DEBUG
void CGroupView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CGroupView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGroupView message handlers

void CGroupView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

int CGroupView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create the image list for the tree control
	CTreeCtrl &treectrl = GetTreeCtrl();
	m_ImageList.Create(IDB_GROUPVIEW_ICONS, 16, 1, RGB(0,255,0));
	treectrl.SetImageList (&m_ImageList, TVSIL_NORMAL);

	ModifyStyle( WS_BORDER | WS_DLGFRAME | WS_THICKFRAME, 0 );
	ModifyStyleEx( WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE | WS_EX_STATICEDGE, 0 );

	OnColorChange( 0, 0 );

	return 0;
}

BOOL CGroupView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

void CGroupView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CTreeView::OnWindowPosChanged(lpwndpos);

	ShowScrollBar( SB_HORZ, FALSE );
}

void CGroupView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CTreeCtrl &treectrl = GetTreeCtrl();
	treectrl.DeleteAllItems( );
	
	CString	strGroups, strDomains;
	VERIFY( strGroups.LoadString( IDS_GROUPVIEW_GROUP ) );
	VERIFY( strDomains.LoadString( IDS_GROUPVIEW_DOMAIN ) );

	HTREEITEM	hGroups = NULL, hDomains = NULL;
	VERIFY( hGroups = treectrl.InsertItem(strGroups,IMG_GROUPS,IMG_GROUPS_SEL) );
	VERIFY( hDomains = treectrl.InsertItem(strDomains,IMG_DOMAINS,IMG_DOMAINS_SEL) );
	
	if( NULL == hGroups || NULL == hDomains )
		return;

	HTREEITEM	hItem	=	NULL;
	CDomainContainer & groups = AfxGetGroupContainer();
	for( int k=0; k<groups.GetSize(); k ++ )
	{
		CDomain	&	domain	=	groups.ElementAt(k);
		VERIFY( hItem = treectrl.InsertItem(domain.m_strName, IMG_GROUP, IMG_GROUP_SEL,hGroups) );
		treectrl.SetItemData( hItem, GV_ITEM_GROUP );
	}
	CDomainContainer & domains = AfxGetDomainContainer();
	for( int k=0; k<domains.GetSize(); k ++ )
	{
		CDomain	&	domain	=	domains.ElementAt(k);
		VERIFY( hItem = treectrl.InsertItem(domain.m_strName, IMG_DOMAIN, IMG_DOMAIN_SEL,hDomains) );
		treectrl.SetItemData( hItem, GV_ITEM_DOMAIN );
	}
	
	treectrl.Expand( hGroups, TVE_EXPAND );
	treectrl.Expand( hDomains, TVE_EXPAND );
	treectrl.Select( hGroups, TVGN_CARET );

	CWnd * pParent = GetParent();
	if( pParent && ::IsWindow(pParent->GetSafeHwnd()) )
		pParent->Invalidate();
}

LRESULT CGroupView::OnUpdateGroups(WPARAM wParam, LPARAM lParam)
{
	OnUpdate( NULL, 0, NULL );
	return 0;
}

LRESULT CGroupView::OnColorChange(WPARAM wParam, LPARAM lParam)
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

void CGroupView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTreeCtrl &treectrl = GetTreeCtrl();
	HTREEITEM	hItem	=	treectrl.GetSelectedItem();
	CString	strText	=	treectrl.GetItemText(hItem);
	
	if( NULL != hItem )
	{
		DWORD	dwData	=	treectrl.GetItemData(hItem);
		if( GV_ITEM_GROUP == dwData )
			AfxShowSlist( CStockContainer::typeGroup, strText );
		else if( GV_ITEM_DOMAIN == dwData )
			AfxShowSlist( CStockContainer::typeDomain, strText );
		SetFocus( );
	}
	
	*pResult = 0;
}

BOOL CGroupView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message )
	{
		int	nVirtKey = (int) pMsg->wParam;
		if( VK_RETURN == nVirtKey )
		{
			CTreeCtrl &treectrl = GetTreeCtrl();
			HTREEITEM	hItem	=	treectrl.GetSelectedItem();
			CString	strText	=	treectrl.GetItemText(hItem);
			
			if( NULL != hItem )
			{
				DWORD	dwData	=	treectrl.GetItemData(hItem);
				if( GV_ITEM_GROUP == dwData )
					AfxShowSlist( CStockContainer::typeGroup, strText );
				else if( GV_ITEM_DOMAIN == dwData )
					AfxShowSlist( CStockContainer::typeDomain, strText );
				SetFocus( );
			}
			return TRUE;
		}
	}
	
	return CTreeView::PreTranslateMessage(pMsg);
}

void CGroupView::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	//Invalidate( );
}
