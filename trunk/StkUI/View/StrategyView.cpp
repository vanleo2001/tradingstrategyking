// StrategyView.cpp : implementation file
//

#include "stdafx.h"

#include "StrategyView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStrategyView

#define	SV_ITEM_PROPERTY		1
#define	SV_ITEM_STOCKS		2
#define	SV_ITEM_RATE			3
#define	SV_ITEM_RULE			4
#define	SV_ITEM_SIMU			5
#define	SV_ITEM_REPORT		6
#define	SV_ITEM_REALOP		7

#define	IMG_WORKSP				0
#define	IMG_WORKSP_SEL			0
#define	IMG_STRATEGY			1
#define	IMG_STRATEGY_SEL		1
#define	IMG_PROPERTY			2
#define	IMG_PROPERTY_SEL		2
#define	IMG_STOCKS				3
#define	IMG_STOCKS_SEL			3
#define	IMG_RATE				4
#define	IMG_RATE_SEL			4
#define	IMG_RULE				5
#define	IMG_RULE_SEL			5
#define	IMG_SIMU				6
#define	IMG_SIMU_SEL			6
#define	IMG_REPORT				7
#define	IMG_REPORT_SEL			7
#define	IMG_REALOP				8
#define	IMG_REALOP_SEL			8


IMPLEMENT_DYNCREATE(CStrategyView, CTreeView)

CStrategyView::CStrategyView()
{
	m_pActiveStrategy	=	NULL;
}

CStrategyView::~CStrategyView()
{
}

CStrategy * CStrategyView::GetActiveStrategy( )
{
	return m_pActiveStrategy;
}

void CStrategyView::SetActiveStrategy( CStrategy * pStrategy )
{
	m_pActiveStrategy	=	pStrategy;
}

BOOL CStrategyView::ActivateStrategy( CStrategy * pActiveStrategy, HTREEITEM hStrategy )
{
	CTreeCtrl &treectrl = GetTreeCtrl();

	CStrategy	*	pOldActiveStrategy	=	m_pActiveStrategy;
	HTREEITEM	hOldActive		=	NULL;
	HTREEITEM	hNewActive		=	NULL;

	CSimuView	*	pSimuView = AfxGetSimuView();
	if( pSimuView && !pSimuView->CanChangeActiveStrategy( ) )
		return FALSE;

	if( m_pActiveStrategy )
	{
		hOldActive = GetItemByStrategy( m_pActiveStrategy );
		m_pActiveStrategy	=	NULL;
	}

	if( pActiveStrategy )
	{
		hNewActive = GetItemByStrategy( pActiveStrategy );
		m_pActiveStrategy	=	pActiveStrategy;
	}
	else if( hStrategy )
	{
		hNewActive	=	hStrategy;
		m_pActiveStrategy	= (CStrategy *)treectrl.GetItemData( hStrategy );
	}
	else
	{
		HTREEITEM hWorksp = treectrl.GetRootItem( );
		if( NULL != hWorksp && treectrl.ItemHasChildren( hWorksp ) )
		{
			HTREEITEM hFirstStrategy = treectrl.GetChildItem( hWorksp );
			hNewActive	=	hFirstStrategy;
			m_pActiveStrategy	= (CStrategy *)treectrl.GetItemData( hFirstStrategy );
		}
	}

	// Changed
	if( pOldActiveStrategy != m_pActiveStrategy )
	{
		if( pSimuView )
			pSimuView->OnActiveStrategyChanged();
	}
	if( hOldActive != hNewActive )
	{
		if( NULL != hOldActive && NULL != pOldActiveStrategy )
		{
			treectrl.SetItemText( hOldActive, pOldActiveStrategy->GetName() );
			treectrl.Expand( hOldActive, TVE_COLLAPSE );
			treectrl.SetItemState( hOldActive, 0, TVIS_BOLD );
		}
		if( NULL != hNewActive && NULL != m_pActiveStrategy )
		{
			CString	strActived;
			strActived.LoadString( IDS_STRATEGYVIEW_ACTIVED );
			treectrl.SetItemText( hNewActive, (LPCTSTR)m_pActiveStrategy->GetName() + strActived );
			treectrl.Expand( hNewActive, TVE_EXPAND );
			treectrl.SetItemState( hNewActive, TVIS_BOLD, TVIS_BOLD );
			treectrl.SelectItem( hNewActive );
		}
	}
	return TRUE;
}

CStrategy	* CStrategyView::GetStrategyByItem( HTREEITEM hItem )
{
	CTreeCtrl &treectrl = GetTreeCtrl();

	HTREEITEM hWorksp = treectrl.GetRootItem( );
	if( NULL == hWorksp )
		return NULL;

	if( NULL == hItem || hItem == hWorksp )
		return NULL;

	HTREEITEM hParent = treectrl.GetParentItem( hItem );
	if( hItem && hParent == hWorksp )
	{
		CStrategy * pStrategy = (CStrategy *)treectrl.GetItemData( hItem );
		return pStrategy;
	}
	else if( hItem && hParent && hWorksp == treectrl.GetParentItem(hParent) )
	{
		CStrategy * pStrategy = (CStrategy *)treectrl.GetItemData( hParent );
		return pStrategy;
	}

	return NULL;
}

HTREEITEM CStrategyView::GetItemByStrategy( CStrategy * pCurStrategy )
{
	CTreeCtrl &treectrl = GetTreeCtrl();
	
	HTREEITEM hWorksp = treectrl.GetRootItem( );
	if( NULL == hWorksp )
		return NULL;
	
	if( treectrl.ItemHasChildren( hWorksp ) )
	{
		HTREEITEM hStrategy = treectrl.GetChildItem( hWorksp );
		while( NULL != hStrategy )
		{
			CStrategy * pStrategy = (CStrategy *)treectrl.GetItemData( hStrategy );
			if( pStrategy == pCurStrategy )
			{
				return hStrategy;
			}
			hStrategy = treectrl.GetNextSiblingItem( hStrategy );
		}
	}
	return NULL;
}

HTREEITEM CStrategyView::AddStrategy( CStrategy * pCurStrategy )
{
	ASSERT( pCurStrategy );
	if( NULL == pCurStrategy )
		return NULL;

	CString	strWorksp, strProperty, strStocks, strRate, strRule;
	CString	strSimu, strReport, strRealOp;
	VERIFY( strWorksp.LoadString( IDS_STRATEGYVIEW_STRATEGY ) );
	VERIFY( strProperty.LoadString( IDS_STRATEGYVIEW_PROPERTY ) );
	VERIFY( strStocks.LoadString( IDS_STRATEGYVIEW_STOCKS ) );
	VERIFY( strRate.LoadString( IDS_STRATEGYVIEW_RATE ) );
	VERIFY( strRule.LoadString( IDS_STRATEGYVIEW_RULE ) );
	VERIFY( strSimu.LoadString( IDS_STRATEGYVIEW_SIMU ) );
	VERIFY( strReport.LoadString( IDS_STRATEGYVIEW_REPORT ) );
	VERIFY( strRealOp.LoadString( IDS_STRATEGYVIEW_REALOP ) );

	CTreeCtrl &treectrl = GetTreeCtrl();
	
	HTREEITEM hWorksp = treectrl.GetRootItem( );
	if( NULL == hWorksp )
		VERIFY( hWorksp = treectrl.InsertItem(strWorksp,IMG_WORKSP,IMG_WORKSP_SEL) );
	if( NULL == hWorksp )
		return NULL;


	HTREEITEM hStrategy = treectrl.InsertItem( pCurStrategy->GetName(), IMG_STRATEGY, IMG_STRATEGY_SEL, hWorksp, TVI_LAST);

	// add the children of the parent item
	if( hStrategy )
	{
		HTREEITEM hItem	=	NULL;
		// hItem	=	treectrl.InsertItem ( strProperty, IMG_PROPERTY, IMG_PROPERTY_SEL, hStrategy, TVI_LAST);
		// treectrl.SetItemData( hItem, SV_ITEM_PROPERTY );
		// hItem	=	treectrl.InsertItem ( strStocks, IMG_STOCKS, IMG_STOCKS_SEL, hStrategy, TVI_LAST);
		// treectrl.SetItemData( hItem, SV_ITEM_STOCKS );
		// hItem	=	treectrl.InsertItem ( strRate, IMG_RATE, IMG_RATE_SEL, hStrategy, TVI_LAST);
		// treectrl.SetItemData( hItem, SV_ITEM_RATE );
		// hItem	=	treectrl.InsertItem ( strRule, IMG_RULE, IMG_RULE_SEL, hStrategy, TVI_LAST);
		// treectrl.SetItemData( hItem, SV_ITEM_RULE );
		// hItem	=	treectrl.InsertItem ( strSimu, IMG_SIMU, IMG_SIMU_SEL, hStrategy, TVI_LAST);
		// treectrl.SetItemData( hItem, SV_ITEM_SIMU );
		// hItem	=	treectrl.InsertItem ( strReport, IMG_REPORT, IMG_REPORT_SEL, hStrategy, TVI_LAST);
		// treectrl.SetItemData( hItem, SV_ITEM_REPORT );
		// hItem	=	treectrl.InsertItem ( strRealOp, IMG_REALOP, IMG_REALOP_SEL, hStrategy, TVI_LAST);
		// treectrl.SetItemData( hItem, SV_ITEM_REALOP );

		treectrl.SetItemData( hStrategy, DWORD(pCurStrategy) );
	}

	return hStrategy;
}

void CStrategyView::UpdateStrategy( CStrategy * pCurStrategy )
{
	HTREEITEM hStrategy = GetItemByStrategy( pCurStrategy );
	if( NULL != hStrategy )
	{
		CTreeCtrl &treectrl = GetTreeCtrl();
		CString	strName	=	pCurStrategy->GetName();
		if( pCurStrategy == m_pActiveStrategy )
		{
			CString	strActived;
			strActived.LoadString( IDS_STRATEGYVIEW_ACTIVED );
			strName	+=	strActived;
		}
		treectrl.SetItemText( hStrategy, strName );
	}
}

void CStrategyView::ExecuteStrategy( CStrategy * pStrategy, DWORD dwData )
{
	if( NULL != pStrategy )
	{
		if( SV_ITEM_PROPERTY == dwData )
			::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_PROPERTY, 0 );
		else if( SV_ITEM_STOCKS == dwData )
			AfxShowSlist( CStockContainer::typeStrategy, NULL );
		else if( SV_ITEM_RULE == dwData )
			::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_SETRULE, 0 );
		else if( SV_ITEM_RATE == dwData )
			::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_SETRATE, 0 );
		else if( SV_ITEM_SIMU == dwData )
			AfxSwitchToStaticView( RUNTIME_CLASS(CSimuView) );
		else if( SV_ITEM_REPORT == dwData )
			::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_REPORT, 0 );
		else if( SV_ITEM_REALOP == dwData )
			::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_REALOP, 0 );
		else
			AfxSwitchToStaticView( RUNTIME_CLASS(CSimuView) );
	}
}


BEGIN_MESSAGE_MAP(CStrategyView, CTreeView)
	//{{AFX_MSG_MAP(CStrategyView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SETFOCUS()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ADDSTRATEGY, OnAddStrategy)
	ON_MESSAGE(WM_USER_REMOVESTRATEGY, OnRemoveStrategy)
	ON_MESSAGE(WM_USER_UPDATESTRATEGY, OnUpdateStrategy)
	ON_MESSAGE(WM_USER_ACTIVATESTRATEGY, OnActivateStrategy)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrategyView drawing

void CStrategyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CStrategyView diagnostics

#ifdef _DEBUG
void CStrategyView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CStrategyView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStrategyView message handlers

void CStrategyView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CStrategyView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

int CStrategyView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create the image list for the tree control
	CTreeCtrl &treectrl = GetTreeCtrl();
	m_ImageList.Create(IDB_STRATEGYVIEW_ICONS, 16, 1, RGB(0,255,0));
	treectrl.SetImageList (&m_ImageList, TVSIL_NORMAL);

	ModifyStyle( WS_BORDER | WS_DLGFRAME | WS_THICKFRAME, 0 );
	ModifyStyleEx( WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE | WS_EX_STATICEDGE, 0 );

	OnColorChange( 0, 0 );

	return 0;
}

BOOL CStrategyView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

void CStrategyView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CTreeView::OnWindowPosChanged(lpwndpos);

	ShowScrollBar( SB_HORZ, FALSE );
}

void CStrategyView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
}

LRESULT CStrategyView::OnAddStrategy(WPARAM wParam, LPARAM lParam)
{
	CStkUIApp	* pApp	=	AfxGetStkUIApp( );
	if( ! pApp )
		return -1;

	CTreeCtrl &treectrl = GetTreeCtrl();

	CString	strWorksp;
	VERIFY( strWorksp.LoadString( IDS_STRATEGYVIEW_STRATEGY ) );
	HTREEITEM hWorksp = treectrl.GetRootItem( );
	if( NULL == hWorksp )
		VERIFY( hWorksp = treectrl.InsertItem(strWorksp,IMG_WORKSP,IMG_WORKSP_SEL) );
	
	if( NULL == hWorksp )
		return -1;

	POSITION pos = pApp->GetFirstStrategyPosition();
	while( NULL != pos )
	{
		CStrategy * pCurStrategy = pApp->GetNextStrategy( pos );
		BOOL	bExist	=	FALSE;
		
		if( NULL == GetItemByStrategy( pCurStrategy ) )
		{
			HTREEITEM hStrategy = AddStrategy( pCurStrategy );
			ActivateStrategy( pCurStrategy, hStrategy );
		}
	}

	treectrl.Expand (hWorksp, TVE_EXPAND);

	return 0;
}

LRESULT CStrategyView::OnRemoveStrategy(WPARAM wParam, LPARAM lParam)
{
	CStkUIApp	* pApp	=	AfxGetStkUIApp( );
	if( ! pApp )
		return -1;

	CTreeCtrl &treectrl = GetTreeCtrl();
	
	HTREEITEM hWorksp = treectrl.GetRootItem( );
	if( NULL == hWorksp )
		return -1;
	
	if( treectrl.ItemHasChildren( hWorksp ) )
	{
		HTREEITEM hStrategy = treectrl.GetChildItem( hWorksp );
		while( NULL != hStrategy )
		{
			CStrategy * pStrategy = (CStrategy *)treectrl.GetItemData( hStrategy );
			HTREEITEM	hStrategyNext = treectrl.GetNextSiblingItem( hStrategy );
			if( !pApp->ExistStrategy( pStrategy ) )
			{
				treectrl.DeleteItem( hStrategy );
				if( m_pActiveStrategy == pStrategy )
					m_pActiveStrategy	=	NULL;
			}
			hStrategy	=	hStrategyNext;
		}
	}

	ActivateStrategy( m_pActiveStrategy );	//default activate the first one

	return 0;
}

LRESULT CStrategyView::OnUpdateStrategy(WPARAM wParam, LPARAM lParam)
{
	CStrategy	* pStrategy = (CStrategy *)wParam;
	UpdateStrategy( pStrategy );

	return 0;
}

LRESULT CStrategyView::OnActivateStrategy(WPARAM wParam, LPARAM lParam)
{
	CStrategy	* pStrategy = (CStrategy *)wParam;
	ActivateStrategy( pStrategy, NULL );

	return 0;
}

LRESULT CStrategyView::OnColorChange(WPARAM wParam, LPARAM lParam)
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

void CStrategyView::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	//Invalidate( );
}

void CStrategyView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTreeCtrl &treectrl = GetTreeCtrl();
	HTREEITEM	hItem	=	treectrl.GetSelectedItem();
	
	if( NULL != hItem )
	{
		DWORD	dwData	=	treectrl.GetItemData(hItem);
		
		CStrategy	* pStrategy = GetStrategyByItem( hItem );
		ActivateStrategy( pStrategy, NULL );
		ExecuteStrategy( pStrategy, dwData );
		SetFocus( );
	}
	
	*pResult = 0;
}

BOOL CStrategyView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message )
	{
		int	nVirtKey = (int) pMsg->wParam;
		if( VK_RETURN == nVirtKey )
		{
			CTreeCtrl &treectrl = GetTreeCtrl();
			HTREEITEM	hItem	=	treectrl.GetSelectedItem();
			
			if( NULL != hItem )
			{
				DWORD	dwData	=	treectrl.GetItemData(hItem);
				
				CStrategy	* pStrategy = GetStrategyByItem( hItem );
				ActivateStrategy( pStrategy, NULL );
				ExecuteStrategy( pStrategy, dwData );
				SetFocus( );
			}
			return TRUE;
		}
	}
	
	return CTreeView::PreTranslateMessage(pMsg);
}

void CStrategyView::OnDestroy() 
{
	CStrategy * pStrategy = GetActiveStrategy();
	if( pStrategy )
		AfxGetProfile().SetLastActiveStrategy( (LPCTSTR)m_pActiveStrategy->GetPathName() );

	CTreeView::OnDestroy();
}

