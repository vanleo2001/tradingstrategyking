// SelectorView.cpp : implementation of the CSelectorView class
//

#include "stdafx.h"

#include "../StaticDoc.h"
#include "SelectorView.h"
#include "../Dialog/ListExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectorView

IMPLEMENT_DYNCREATE(CSelectorView, CFormView)

BEGIN_MESSAGE_MAP(CSelectorView, CFormView)
	//{{AFX_MSG_MAP(CSelectorView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_BN_CLICKED(IDC_RUNSELECTOR, OnRunselector)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
	ON_MESSAGE(WM_USER_CANCLOSEVIEW, OnCanCloseView)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectorView construction/destruction

CSelectorView::CSelectorView()
	: CFormView(CSelectorView::IDD)
{
	m_bShouldUpdate	=	FALSE;
	m_bRunning	=	FALSE;
}

CSelectorView::~CSelectorView()
{
}

#define		SELECTORVIEW_COLUMN_COUNT	6

BOOL CSelectorView::InitializeGrid( )
{
	// Create GridCtrl
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(TRUE);
	m_Grid.SetHeaderSort(FALSE);
	m_Grid.SetSingleRowSelection(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.SetGridLines( GVL_NONE );
	m_Grid.EnableTitleTips( TRUE );
	m_Grid.SetRowResize( FALSE );
	m_Grid.SetColumnResize( TRUE );

	m_Grid.SetBkColor( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
	m_Grid.SetTextBkColor( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
	m_Grid.SetSelectedBkColor(AfxGetProfile().GetColor(CColorClass::clrSListSelected));

	TRY {
		m_Grid.SetRowCount(1);
		m_Grid.SetColumnCount(SELECTORVIEW_COLUMN_COUNT);
		m_Grid.SetFixedRowCount(1);
		m_Grid.SetFixedColumnCount(1);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	END_CATCH

	for( int nCol=0; nCol<m_Grid.GetColumnCount(); nCol++ )
	{
		m_Grid.SetItemBkColour( 0, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
		m_Grid.SetItemFgColour( 0, nCol, AfxGetProfile().GetColor(CColorClass::clrTitle) );
	}

	CRect	rectGrid;
	m_Grid.GetClientRect( &rectGrid );
	int	nWidth	=	rectGrid.Width() / m_Grid.GetColumnCount() - 1;
	m_Grid.SetColumnWidth( 0, 70 );
	m_Grid.SetColumnWidth( 1, nWidth );
	m_Grid.SetColumnWidth( 2, 70 );
	m_Grid.SetColumnWidth( 3, 70 );
	m_Grid.SetColumnWidth( 4, nWidth + 30 );
	m_Grid.SetColumnWidth( 5, nWidth + 40 );

	// Set Column Header
	UINT	idsHeader[SELECTORVIEW_COLUMN_COUNT] = {IDS_SELECTORVIEW_CODE, IDS_SELECTORVIEW_NAME,
							IDS_SELECTORVIEW_DATE, IDS_SELECTORVIEW_CLOSE, IDS_SELECTORVIEW_SIGNAL,
							IDS_SELECTORVIEW_DISCRIPT };
	for( int nCol=0; nCol<SELECTORVIEW_COLUMN_COUNT; nCol++ )
	{
		GV_ITEM item;
		item.mask = GVIF_TEXT|GVIF_FORMAT;
		item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		item.row = 0;
		item.col = nCol;
		item.szText.LoadString( idsHeader[nCol] );
		m_Grid.SetItem(&item);
	}

	return TRUE;
}

void CSelectorView::SetFont( LPLOGFONT pLogFont )
{
	ASSERT( pLogFont );

	HFONT hFont = ::CreateFontIndirect(pLogFont);
	m_Grid.SendMessage( WM_SETFONT, (WPARAM)hFont, MAKELPARAM(1, 0) );
	// m_Grid.AutoSize( );
	DeleteObject(hFont);
}

void CSelectorView::GetSelectedStocks( CSPStringArray & astr )
{
	int	nTotalCount	=	m_Grid.GetSelectedCount();
	astr.RemoveAll();
	astr.SetSize( 0, nTotalCount > 10 ? nTotalCount : -1 );
	for( int nRow=1; nRow<m_Grid.GetRowCount(); nRow++ )
	{
		BOOL	bSelected	=	FALSE;
		for( int nCol=0; nCol<m_Grid.GetColumnCount(); nCol ++ )
			bSelected	|=	( m_Grid.GetItemState(nRow,nCol) & GVIS_SELECTED );
		if( !bSelected )
			continue;

		LPARAM	id	=	m_Grid.GetItemData(nRow,0);
		CStockInfo & info	=	m_container.GetStockInfoByID(id);
		astr.Add( info.GetStockCode() );
	}
}

void CSelectorView::OnDblclkItem( int nStockIndex )
{
	if( nStockIndex >= 0 && nStockIndex < m_container.GetSize() )
	{
		CStockInfo & info = m_container.ElementAt(nStockIndex);
		AfxShowStockGraph( info.GetStockCode() );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSelectorView overrides

BOOL CSelectorView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CSelectorView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( ::IsWindow(m_Grid.GetSafeHwnd()) )
		if( m_Grid.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CSelectorView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( m_Grid.GetSafeHwnd() == pMsg->hwnd )
	{
		if( WM_LBUTTONDBLCLK == pMsg->message
			|| ( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam ) )
		{
			int	nColumnCount	=	m_Grid.GetColumnCount();
			int	nRowCount		=	m_Grid.GetRowCount();
			if( nColumnCount <=0 || nRowCount <= 0 )
				return CFormView::PreTranslateMessage(pMsg);
			CRect	rectCell;
			m_Grid.GetCellRect(0,0,&rectCell);
			CPoint	pt	=	pMsg->pt;
			::ScreenToClient( m_Grid.GetSafeHwnd(), &pt );
			if( pt.y >= rectCell.top && pt.y < rectCell.bottom )
				return CFormView::PreTranslateMessage(pMsg);

			int	nSelRow	=	m_Grid.GetFocusCell().row;
			if( nSelRow >= 1 && nSelRow < m_Grid.GetRowCount() )
			{
				int	id	=	m_Grid.GetItemData(nSelRow,0);
				OnDblclkItem( id );
			}
		}
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}

void CSelectorView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectorView)
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_RUNSELECTOR, m_btnRunSelector);
	DDX_Control(pDX, IDC_TECH, m_cmbTech);
	DDX_Control(pDX, IDC_KTYPE, m_cmbKType);
	DDX_Control(pDX, IDC_STOCKGROUP, m_cmbStockGroup);
	//}}AFX_DATA_MAP

	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}

void CSelectorView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	InitializeGrid( );

	m_staticInfo.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	m_progress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );

	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	AfxGetProfile().GetFontSListView( &lf );
	SetFont( &lf );

	m_cmbStockGroup.InitStrings( TRUE, TRUE, AfxGetGroupContainer( ) );
	m_cmbStockGroup.SetCurSel( 0 );
	m_cmbStockGroup.SelectGroupAll( );

	m_cmbKType.InitializeDay( );

	m_cmbTech.Initialize();
}

void CSelectorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( UPDATE_HINT_SELECTORVIEW != lHint )
		return;
	if( IsWindowVisible() )
		SetFocus( );
	int	nColumnCount	=	m_Grid.GetColumnCount();
	if( nColumnCount <= 0 )
		return;

	if( GetParentFrame()->GetSafeHwnd() != AfxGetMainFrame()->GetActiveFrame()->GetSafeHwnd() )
	{
		m_bShouldUpdate	=	TRUE;
		return;
	}
	m_bShouldUpdate	=	FALSE;

	m_Grid.DeleteNonFixedRows();

	// Progress
	CMainFrame	* pMainFrame = AfxGetMainFrame();
	if( pMainFrame )
	{
		pMainFrame->ShowProgressBar( );
		pMainFrame->SetProgress( 0 );
		pMainFrame->SetMessageText( IDS_MAINFRAME_WAITING );
	}

	CRect rectClient;
	GetClientRect( &rectClient );
	int nPageCount = 1 + rectClient.Height() / abs(m_Grid.GetFixedRowHeight()) + 1;

	for( int i=0; i<m_container.GetSize() && i<m_signals.GetSize(); i++ )
	{
		CStockInfo & info = m_container.GetStockInfoByID(i);

		int nRow = m_Grid.InsertRow( info.GetStockName() );
		m_Grid.SetItemData( nRow, 0, i );

		for( int nCol=0; nCol<SELECTORVIEW_COLUMN_COUNT; nCol++ )
		{
			m_Grid.SetItemFormat( nRow, nCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
			if( 0 == nCol )
				m_Grid.SetItemText( nRow, nCol, AfxGetVariantDispString( SLH_CODE, info, &m_container ) );
			else if( 1 == nCol )
				m_Grid.SetItemText( nRow, nCol, AfxGetVariantDispString( SLH_NAME, info, &m_container ) );
			else if( 2 == nCol )
				m_Grid.SetItemText( nRow, nCol, AfxGetVariantDispString( SLH_DATE, info, &m_container ) );
			else if( 3 == nCol )
				m_Grid.SetItemText( nRow, nCol, AfxGetVariantDispString( SLH_CLOSE, info, &m_container ) );
			else if( 4 == nCol )
				m_Grid.SetItemText( nRow, nCol, AfxGetIntensityString( m_signals.GetAt(i) ) );
			else if( 5 == nCol && i<m_reasons.GetSize() )
				m_Grid.SetItemText( nRow, nCol, AfxGetIntensityCodeString( m_reasons.GetAt(i) ) );
			m_Grid.SetItemBkColour( nRow, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
			if( 4 == nCol )
				m_Grid.SetItemFgColour( nRow, nCol, ((int)m_signals[i]) > 0 ? AfxGetProfile().GetColor(CColorClass::clrRise) : AfxGetProfile().GetColor(CColorClass::clrFall) );
			else
				m_Grid.SetItemFgColour( nRow, nCol, AfxGetProfile().GetColor(CColorClass::clrText) );
		}

		if( i == nPageCount+5 )
		{
			m_Grid.Invalidate( );
			MSG		msg;
			while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
				AfxGetApp()->PumpMessage();
		}
		if( pMainFrame )
			pMainFrame->SetProgress( 100*i/m_container.GetSize() );
	}

	if( pMainFrame )
	{
		pMainFrame->SetProgress( 100 );
		pMainFrame->HideProgressBar( );
		pMainFrame->SetMessageText( IDS_MAINFRAME_FINISHED );
	}

	m_Grid.Invalidate( );
}

/////////////////////////////////////////////////////////////////////////////
// CSelectorView printing

BOOL CSelectorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSelectorView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if( ::IsWindow( m_Grid.GetSafeHwnd() ) )
		m_Grid.OnBeginPrinting(pDC, pInfo);
}

void CSelectorView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	if( ::IsWindow( m_Grid.GetSafeHwnd() ) )
		m_Grid.OnPrint(pDC, pInfo);
}

void CSelectorView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if( ::IsWindow( m_Grid.GetSafeHwnd() ) )
		m_Grid.OnEndPrinting(pDC, pInfo);
}

void CSelectorView::OnFilePrint() 
{
	if( ::IsWindow( m_Grid.GetSafeHwnd() ) )
		m_Grid.Print( );
}

void CSelectorView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( ::IsWindow( m_Grid.GetSafeHwnd() ) );
}

/////////////////////////////////////////////////////////////////////////////
// CSelectorView diagnostics

#ifdef _DEBUG
void CSelectorView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSelectorView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSelectorView message handlers

int CSelectorView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CSelectorView::OnDestroy() 
{
	CFormView::OnDestroy();
}

void CSelectorView::OnTimer(UINT nIDEvent) 
{
	CFormView::OnTimer(nIDEvent);
}

void CSelectorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( bActivate && m_bShouldUpdate )
	{
		OnUpdate( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
		m_bShouldUpdate	=	FALSE;
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

LRESULT CSelectorView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString( IDS_TITLE_SELECTORVIEW );

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

LRESULT CSelectorView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((LPARAM *)lParam)	=	ID_VIEW_SELECTOR;
	return 0L;
}

LRESULT CSelectorView::OnCanCloseView(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((BOOL *)lParam)	=	!m_bRunning;

	return 0L;
}

LRESULT CSelectorView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	m_Grid.SetBkColor( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
	m_Grid.SetTextBkColor( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
	m_Grid.SetSelectedBkColor(AfxGetProfile().GetColor(CColorClass::clrSListSelected));

	OnUpdate( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
	Invalidate();
	return 0L;
}

BOOL CSelectorView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if( pDC && pDC->IsKindOf(RUNTIME_CLASS(CDC)) )
	{
		CRect rect;
		GetClientRect( &rect );
		CBrush brush;
		brush.CreateSolidBrush( AfxGetProfile().GetColor(CColorClass::clrSimuBK) );
		pDC->FillRect( &rect, &brush );
		return TRUE;
	}
	return CFormView::OnEraseBkgnd(pDC);
}

void CSelectorView::OnSize(UINT nType, int cx, int cy) 
{
	CSize	sizeTotal( cx, cy );
	SetScaleToFitSize( sizeTotal );

	CFormView::OnSize(nType, cx, cy);
}

void CSelectorView::OnSetFocus(CWnd* pOldWnd) 
{
	CFormView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	m_Grid.SetFocus();
	if( !m_Grid.GetFocusCell().IsValid()
		&& m_Grid.GetRowCount() > 1 && m_Grid.GetColumnCount() > 1 )
	{
		m_Grid.SetFocusCell( 1, 1 );
		for( int nCol=0; nCol<m_Grid.GetColumnCount(); nCol ++ )
			m_Grid.SetItemState(1,nCol,m_Grid.GetItemState(1,nCol) | GVIS_SELECTED);
	}
}

HBRUSH CSelectorView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( pDC )
		pDC->SetTextColor( AfxGetProfile().GetColor( CColorClass::clrSimuText ) );
	if( nCtlColor == CTLCOLOR_DLG
		|| nCtlColor == CTLCOLOR_STATIC
		|| nCtlColor == CTLCOLOR_MSGBOX )
	{
		if( nCtlColor == CTLCOLOR_STATIC )
			pDC->SetBkMode( TRANSPARENT );

		static CBrush brushBK;
		static COLORREF	clrBKLast	=	RGB(255,255,255);
		COLORREF	clrBKNow	=	AfxGetProfile().GetColor( CColorClass::clrSimuBK );
		if( NULL == brushBK.GetSafeHandle() || clrBKLast != clrBKNow )
		{
			brushBK.DeleteObject( );
			VERIFY( brushBK.CreateSolidBrush( clrBKNow ) );
		}
		clrBKLast	=	clrBKNow;
		return (HBRUSH)brushBK.GetSafeHandle();
	}
	return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CSelectorView::OnRunselector() 
{
	m_bRunning	=	TRUE;

	// clear
	m_btnRunSelector.EnableWindow( FALSE );
	m_staticInfo.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );

	m_container.Clear();
	m_signals.RemoveAll();
	m_reasons.RemoveAll();
	OnUpdate( NULL, UPDATE_HINT_SELECTORVIEW, NULL );

	// get parameters
	CStockContainer cntn;
	cntn.RetrieveFromStatic( m_cmbStockGroup.GetCurSelType(), m_cmbStockGroup.GetCurSelName(), NULL, -1 );
	int nKType = m_cmbKType.GetSelect();
	int nTech = m_cmbTech.GetSelect();

	// progress
	m_progress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	m_progress.SetRange( 0, cntn.GetSize() );
	m_progress.SetPos( 0 );

	// run selector
	for( int i=0; i<cntn.GetSize(); i++ )
	{
		m_progress.SetPos( i+1 );
		MSG		msg;
		while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
			AfxGetApp()->PumpMessage();

		CStockInfo & info = cntn.ElementAt(i);
		int		its		= ITS_NOTHING;
		UINT	itsc	= ITSC_NOTHING;

		CStock	stock;
		if( !stock.SetStockInfo( &info ) )
			continue;
		if( !AfxPrepareStockData( &AfxGetDB(), stock, nKType, CKData::formatXDRdown, AfxGetProfile().GetGraphMaindataType(), FALSE, FALSE ) )
			continue;
		CKData & kdata = stock.GetKData( nKType );
		if( kdata.GetSize() < 1 )
			continue;
		CTechnique * pTech	=	CTechnique::CreateTechnique( nTech, &kdata );
		if( NULL == pTech )
			continue;
		AfxGetProfile().GetTechParameters().FindParameters( nTech, pTech );
		its = pTech->GetSignal( kdata.GetSize()-1, &itsc );
		if( ITS_NOTHING != its )
		{
			m_container.Add( info );
			m_signals.Add( its );
			m_reasons.Add( itsc );
		}
		delete pTech;
	}

	// display result
	OnUpdate( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
	if( 0 == m_container.GetSize() )
		m_staticInfo.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );

	m_btnRunSelector.EnableWindow( TRUE );

	m_bRunning	=	FALSE;
}

