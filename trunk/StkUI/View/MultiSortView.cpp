// MultiSortView.cpp : implementation of the CMultiSortView class
//

#include "stdafx.h"

#include "../StaticDoc.h"
#include "MultiSortView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSortView

IMPLEMENT_DYNCREATE(CMultiSortView, CView)

BEGIN_MESSAGE_MAP(CMultiSortView, CView)
	//{{AFX_MSG_MAP(CMultiSortView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
	ON_MESSAGE(WM_APP_STKRECEIVER_DATA, OnStkReceiverData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSortView construction/destruction

#define	MSV_TIMER_REFRESH	1

CMultiSortView::CMultiSortView()
{
	// TODO: add construction code here
}

CMultiSortView::~CMultiSortView()
{
}

void CMultiSortView::SendRequestQuote( BOOL bForced )
{
	BOOL bInTrade = CSPTime::InTradeTime(CSPTime::GetCurrentTime().GetTime(),900);
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	if( bForced || (bInTrade && pView == this) )
	{
		CStockInfo info;
		info.SetType( m_multisort.GetMultiSortClass( ) );
		AfxGetStkReceiver().RequestStockData( CStock::dataMultisort, &info, 1, 0, 0 );
	}
}

BOOL CMultiSortView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMultiSortView drawing

void CMultiSortView::OnDraw(CDC* pDC)
{
	CStaticDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CRect	rectClient;
	GetClientRect( &rectClient );
	m_multisort.Redraw( pDC, rectClient );
}

/////////////////////////////////////////////////////////////////////////////
// CMultiSortView printing

BOOL CMultiSortView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMultiSortView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CView::OnBeginPrinting( pDC, pInfo );
}

void CMultiSortView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CView::OnEndPrinting( pDC, pInfo );
}

void CMultiSortView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add customized printing code here
	pDC->SetMapMode( MM_ISOTROPIC );
	CRect	rectClient( 0, 0, 1020, 586 );
	// GetClientRect( &rectClient );
	pDC->SetWindowOrg( rectClient.left, rectClient.top );
	pDC->SetWindowExt( rectClient.Width(), rectClient.Height() );
	pDC->SetViewportOrg( pInfo->m_rectDraw.left, pInfo->m_rectDraw.top );
	pDC->SetViewportExt( pInfo->m_rectDraw.Width(), pInfo->m_rectDraw.Height() );
	
	m_multisort.Redraw( pDC, rectClient );
	// CView::OnPrint(pDC,pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CMultiSortView diagnostics

#ifdef _DEBUG
void CMultiSortView::AssertValid() const
{
	CView::AssertValid();
}

void CMultiSortView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMultiSortView message handlers

int CMultiSortView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CFont	font;
	if( !font.CreateStockObject( DEFAULT_GUI_FONT ) )
		font.CreateStockObject( ANSI_VAR_FONT );
	SetFont( &font );

	m_multisort.SetParent( this );

	// 实时行情刷新
	AfxGetStkReceiver().AddRcvDataWnd( GetSafeHwnd() );

	SetTimer( MSV_TIMER_REFRESH, 30000, NULL );

	return 0;
}

void CMultiSortView::OnDestroy() 
{
	// 停止行情刷新通知消息
	AfxGetStkReceiver().RemoveRcvDataWnd( GetSafeHwnd() );

	KillTimer( MSV_TIMER_REFRESH );

	CView::OnDestroy();
}

void CMultiSortView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( bActivate )
		OnUpdate( NULL, UPDATE_HINT_MULTISORTVIEW, NULL );

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CMultiSortView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if( pDC && pDC->IsKindOf(RUNTIME_CLASS(CDC)) )
	{
		CRect rect;
		GetClientRect( &rect );
		CBrush brush;
		brush.CreateSolidBrush( AfxGetProfile().GetColor(CColorClass::clrGraphBK) );
		pDC->FillRect( &rect, &brush );
		return TRUE;
	}
	return CView::OnEraseBkgnd(pDC);
}

LRESULT CMultiSortView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString( IDS_TITLE_MULTISORTVIEW );

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

LRESULT CMultiSortView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((LPARAM *)lParam)	=	ID_VIEW_MULTISORT;
	return 0L;
}

LRESULT CMultiSortView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	Invalidate( );
	return 0L;
}

LRESULT CMultiSortView::OnStkReceiverData(WPARAM wParam, LPARAM lParam)
{
	return m_multisort.OnStkReceiverData(wParam,lParam);
}

void CMultiSortView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	OnUpdate( NULL, UPDATE_HINT_MULTISORTVIEW, NULL );
}

void CMultiSortView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( UPDATE_HINT_MULTISORTVIEW != lHint )
		return;
	if( !IsWindowVisible() )
		return;
	SetFocus( );

	m_multisort.RecalculateSortID( FALSE, FALSE );
	Invalidate( );

	SendRequestQuote(TRUE);
}


void CMultiSortView::OnTimer(UINT nIDEvent) 
{
	if( MSV_TIMER_REFRESH == nIDEvent )
	{
		SendRequestQuote(FALSE);

		CView * pView = AfxGetStaticDoc()->GetActiveView();
		if( pView == this )
			m_multisort.RecalculateSortID( TRUE, TRUE );
	}

	CView::OnTimer(nIDEvent);
}

void CMultiSortView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_multisort.OnLButtonDown( nFlags, point, IsSHIFTpressed(), IsCTRLpressed() );
	
	CView::OnLButtonDown(nFlags, point);
}

void CMultiSortView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	m_multisort.OnLButtonDblClk( nFlags, point );
	
	CView::OnLButtonDblClk(nFlags, point);
}
