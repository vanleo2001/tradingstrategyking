// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "StkUI.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

#define	IDC_CHILDFRAME_TABCTRL	1000

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY(TCN_SELCHANGE, IDC_CHILDFRAME_TABCTRL, OnTabSelChange)
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChildFrame* CChildFrame::CreateNewFrame( CStaticDoc* pStaticDoc )
{
	if (pStaticDoc != NULL)
		ASSERT_VALID(pStaticDoc);
	// create a frame wired to the specified document

	ASSERT( pStaticDoc->GetIDResource() != 0); // must have a resource ID to load from
	CCreateContext context;
	context.m_pCurrentFrame = NULL;
	context.m_pCurrentDoc = NULL;		//pStaticDoc;
	context.m_pLastView = NULL;
	context.m_pNewViewClass = NULL; // pViewClass if this is set, a view will be created!
	context.m_pNewDocTemplate = NULL;

	CRuntimeClass* pFrameClass = RUNTIME_CLASS(CChildFrame);

	CChildFrame * pFrame = (CChildFrame*)pFrameClass->CreateObject();
	if (pFrame == NULL)
	{
		TRACE1("Warning: Dynamic create of frame %hs failed.\n",
			pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CChildFrame, pFrame);

	// create new from resource
	if (!pFrame->LoadFrame( pStaticDoc->GetIDResource(),
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			AfxGetMainFrame(), &context))
	{
		TRACE0("Warning: Couldn't create a child frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	// it worked !
	return pFrame;
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIChildWnd::PreCreateWindow(cs);
}

BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame * pMainFrame = AfxGetMainFrame( );
	if( WM_SYSKEYDOWN == pMsg->message
		|| WM_SYSKEYUP == pMsg->message 
		|| WM_SYSCHAR == pMsg->message )
	{
		if( pMainFrame && VK_F4 == pMsg->wParam )
		{
			pMainFrame->PostMessage( WM_CLOSE );
			return TRUE;
		}
	}
	else if( WM_RBUTTONUP == pMsg->message
		|| WM_RBUTTONDOWN == pMsg->message
		|| WM_NCRBUTTONUP == pMsg->message
		|| WM_NCRBUTTONDOWN == pMsg->message )
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(pMsg->hwnd);
		CView * pView = DYNAMIC_DOWNCAST( CView, pWnd );
		if( NULL == pView )
			pView = GetActiveView();
		
		UINT	nMenuID	=	0;
		if( pView && pView->IsKindOf(RUNTIME_CLASS(CWizardView)) )
			nMenuID	=	IDR_POPUP_WIZARDVIEW;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CSimuView)) )
			nMenuID	=	IDR_POPUP_SIMUVIEW;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CSListView)) )
			nMenuID	=	IDR_POPUP_SLISTVIEW;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView)) )
		{
			pView->PostMessage(WM_USER_MYRBUTTONDOWN,pMsg->wParam,pMsg->lParam);
			nMenuID	=	IDR_POPUP_GRAPHVIEW;
		}
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CRealTimeView)) )
			nMenuID	=	IDR_POPUP_REALTIMEVIEW;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CMultiSortView)) )
			nMenuID	=	IDR_POPUP_MULTISORTVIEW;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CBaseView)) )
			nMenuID	=	IDR_POPUP_BASEVIEW;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CSelectorView)) )
			nMenuID	=	IDR_POPUP_SETTING;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CStrategyView)) )
			nMenuID	=	IDR_POPUP_SIMUVIEW;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CGroupView)) )
			nMenuID	=	IDR_POPUP_SETTING;
		else if( pView && pView->IsKindOf(RUNTIME_CLASS(CTechsView)) )
			nMenuID	=	IDR_POPUP_SETTING;

		if( pMainFrame && 0 != nMenuID )
		{
			if( pWnd && WM_RBUTTONUP == pMsg->message )
			{
				CPoint pt;
				pt.x = LOWORD(pMsg->lParam);
				pt.y = HIWORD(pMsg->lParam);
				pWnd->ClientToScreen(&pt);
				
				CMenu	menu;
				if( menu.LoadMenu( nMenuID ) )
				{
					CMenu	*	pMenu	=	menu.GetSubMenu(0);
					for( UINT nMenu=0; pMenu && nMenu<pMenu->GetMenuItemCount(); nMenu++ )
					{
						CMenu	*	pPopupMenu	=	pMenu->GetSubMenu(nMenu);
						pMainFrame->InitMenuPopup( pPopupMenu );
					}
				
					/* ProfUIS Using Code
					CExtPopupMenuWnd * pPopupWnd = new CExtPopupMenuWnd;
					VERIFY( pPopupWnd->UpdateFromMenu( pMainFrame->GetSafeHwnd(), &menu, true, true ) );
					VERIFY( pPopupWnd->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, pMainFrame->GetSafeHwnd() ) );
					*/
					VERIFY( pMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, pMainFrame ) );
				}
			}
			return TRUE;
		}
	}

	/* ProfUIS Using Code
	if( pMainFrame && pMainFrame->m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	*/

	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

void CChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CView * pView = GetActiveView( );
	if (bAddToTitle)
	{
		TCHAR szText[256+_MAX_PATH];
		memset( szText, 0, sizeof(szText) );

		if (pView == NULL)
			lstrcpyn(szText, m_strTitle, 256+MAX_PATH);
		else
			pView->SendMessage( WM_USER_GETVIEWTITLE, 255+MAX_PATH, long(szText) );

		// set title if changed, but don't remove completely
		AfxSetWindowText(m_hWnd, szText);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

//CEnTabCtrl	CChildFrame::m_tabctrl;

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/* My Own Tab
	if( !::IsWindow( m_tabctrl.GetSafeHwnd() ) )
	{
		//Create the Tab Control
		if (!m_tabctrl.Create(WS_VISIBLE|WS_CHILD|TCS_TOOLTIPS,
			CRect(0,0,0,0), this, IDC_CHILDFRAME_TABCTRL))
		{
			TRACE0("Unable to create tab control bar\n");
			return -1;
		}

		// set "normal" GUI-font
		CFont	font;
		if( !font.CreateStockObject( DEFAULT_GUI_FONT ) )
			font.CreateStockObject( ANSI_VAR_FONT );
		m_tabctrl.SetFont(&font);
	}
	*/

	return 0;
}

void CChildFrame::ResetClientPosition( BOOL bMaximized )
{
	/* My Own Tab
	CRect	rect;
	GetClientRect( &rect );

	if( bMaximized )
	{
		CView * pView = GetActiveView();
		if( pView && ::IsWindow( pView->GetSafeHwnd() ) )
			pView->SetWindowPos( NULL, rect.left, rect.top+21, rect.Width(), rect.Height()-21, SWP_SHOWWINDOW );
		if( ::IsWindow( m_tabctrl.GetSafeHwnd() ) )
			m_tabctrl.SetWindowPos( NULL, rect.left-5, rect.top, rect.Width()+10, rect.Height(), SWP_SHOWWINDOW );
	}
	else
	{
		CView * pView = GetActiveView();
		if( pView && ::IsWindow( pView->GetSafeHwnd() ) )
			pView->SetWindowPos( NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW );
		if( ::IsWindow( m_tabctrl.GetSafeHwnd() ) )
			m_tabctrl.SetWindowPos( NULL, rect.left-5, rect.top, rect.Width()+10, rect.Height(), SWP_SHOWWINDOW );
	}
	*/
}

void CChildFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	/* My Own Tab
	if( !(lpwndpos->flags & SWP_NOSIZE) || !(lpwndpos->flags & SWP_NOMOVE)
		|| (lpwndpos->flags & SWP_SHOWWINDOW) )
	{
		CView * pView = GetActiveView();
		if( pView && ::IsWindow(pView->GetSafeHwnd()) )
			pView->SetWindowPos( NULL, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW );
	}
	*/

	CMDIChildWnd::OnWindowPosChanged(lpwndpos);

	/* My Own Tab
	if( !(lpwndpos->flags & SWP_NOSIZE) || !(lpwndpos->flags & SWP_NOMOVE)
		|| (lpwndpos->flags & SWP_SHOWWINDOW) )
	{
		ResetClientPosition( IsZoomed() );
	}
	*/
}

void CChildFrame::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	//int nSel = m_tabctrl.GetCurSel();
	//if( -1 == nSel )
	//	return;

	//CMainFrame	* pFrame = AfxGetMainFrame();
	//if( NULL == pFrame )
	//	return;

	//TCITEM tci;
	//tci.mask	=	TCIF_PARAM;
	//if( m_tabctrl.GetItem( nSel, &tci ) )
	//{
	//	pFrame->OnCmdMsg( tci.lParam, 0, NULL, NULL );
	//}
}

void CChildFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	// Get View Title
	CView * pView = GetActiveView( );
	TCHAR szText[256+_MAX_PATH];
	memset( szText, 0, sizeof(szText) );

	if (pView == NULL)
		lstrcpyn(szText, m_strTitle, 256+MAX_PATH);
	else
	{
		BOOL	bCanClose	=	TRUE;
		if( 0 == pView->SendMessage( WM_USER_CANCLOSEVIEW, NULL, (LPARAM)(&bCanClose) )
			&& !bCanClose )
			return;
		pView->SendMessage( WM_USER_GETVIEWTITLE, 255+MAX_PATH, long(szText) );
	}

	// Set sel
	/* My Own Tab
	char	buffer[256+_MAX_PATH];
	memset( buffer, 0, sizeof(buffer) );
	for( int nItem=0; nItem<m_tabctrl.GetItemCount(); nItem++ )
	{
		TCITEM tci;
		tci.mask	=	TCIF_TEXT;
		tci.pszText	=	buffer;
		tci.cchTextMax	=	sizeof(buffer)-1;
		if( m_tabctrl.GetItem( nItem, &tci )
			&& 0 == strncmp( szText, buffer, min(sizeof(szText),sizeof(buffer))-1 ) )
		{
			m_tabctrl.DeleteItem( nItem );
			break;
		}
	}
	*/

	CMDIChildWnd::OnClose();
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
	// CDocument * pDoc = this->GetActiveDocument( );	// CStaticDoc
	// CView * pView = this->GetActiveView();			// CWizardView, CSListView ...
	// CFrameWnd * pFrame = this->GetActiveFrame( );	// CChildFrame
	// CWnd * pWnd = this->GetActiveWindow( );			// CMainFrame

	/* My Own Tab
	if( bActivate )
	{
		m_tabctrl.SetParent( this );
		m_tabctrl.SetOwner( this );

		// Get View Title
		CView * pView = GetActiveView( );
		TCHAR szText[256+_MAX_PATH];
		memset( szText, 0, sizeof(szText) );

		if (pView == NULL)
			lstrcpyn(szText, m_strTitle, 256+MAX_PATH);
		else
			pView->SendMessage( WM_USER_GETVIEWTITLE, 255+MAX_PATH, long(szText) );

		// Set sel
		char	buffer[256+_MAX_PATH];
		memset( buffer, 0, sizeof(buffer) );
		BOOL	bHas	=	FALSE;
		for( int nItem=0; nItem<m_tabctrl.GetItemCount(); nItem++ )
		{
			TCITEM tci;
			tci.mask	=	TCIF_TEXT;
			tci.pszText	=	buffer;
			tci.cchTextMax	=	sizeof(buffer)-1;
			if( m_tabctrl.GetItem( nItem, &tci )
				&& 0 == strncmp( szText, buffer, min(sizeof(szText),sizeof(buffer))-1 ) )
			{
				m_tabctrl.SetCurSel( nItem );
				bHas	=	TRUE;
				break;
			}
		}

		if( pView && !bHas )
		{
			LPARAM	viewid	=	0;
			pView->SendMessage( WM_USER_GETVIEWCMDID, 0, (LPARAM)&viewid );

			TC_ITEM tci;
			tci.mask = TCIF_TEXT | TCIF_PARAM;
			tci.pszText =	szText;
			tci.lParam	=	viewid;
			m_tabctrl.InsertItem(m_tabctrl.GetItemCount(), &tci);
			m_tabctrl.SetCurSel(m_tabctrl.GetItemCount()-1);
		}
	}
	*/
}

void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	/* My Own Tab
	if( SIZE_MAXIMIZED == nType )
	{
		ResetClientPosition( IsZoomed() );
	}
	*/
}

