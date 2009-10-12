// WizardView.cpp : implementation of the CWizardView class
//

#include "stdafx.h"

#include "../StaticDoc.h"
#include "WizardView.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardView

IMPLEMENT_DYNCREATE(CWizardView, CFormView)

BEGIN_MESSAGE_MAP(CWizardView, CFormView)
	//{{AFX_MSG_MAP(CWizardView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNNEWSTRATEGY, OnBtnnewstrategy)
	ON_BN_CLICKED(IDC_BTNOPENSTRATEGY, OnBtnopenstrategy)
	ON_BN_CLICKED(IDC_BTNHELP, OnBtnhelp)
	ON_NOTIFY(NM_DBLCLK, IDC_STRATEGYLIST, OnDblclkStrategylist)
	ON_COMMAND(ID_WIZARDVIEW_LARGEICONS, OnWizardviewLargeicons)
	ON_COMMAND(ID_WIZARDVIEW_SMALLICONS, OnWizardviewSmallicons)
	ON_COMMAND(ID_WIZARDVIEW_LIST, OnWizardviewList)
	ON_COMMAND(ID_WIZARDVIEW_DETAILS, OnWizardviewDetails)
	ON_UPDATE_COMMAND_UI(ID_WIZARDVIEW_LARGEICONS, OnUpdateWizardviewLargeicons)
	ON_UPDATE_COMMAND_UI(ID_WIZARDVIEW_SMALLICONS, OnUpdateWizardviewSmallicons)
	ON_UPDATE_COMMAND_UI(ID_WIZARDVIEW_LIST, OnUpdateWizardviewList)
	ON_UPDATE_COMMAND_UI(ID_WIZARDVIEW_DETAILS, OnUpdateWizardviewDetails)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardView construction/destruction

CWizardView::CWizardView()
	: CFormView(CWizardView::IDD)
{
	//{{AFX_DATA_INIT(CWizardView)
	//}}AFX_DATA_INIT
}

CWizardView::~CWizardView()
{
}

void CWizardView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardView)
	DDX_Control(pDX, IDC_GOTO_INFO, m_linkInfo);
	DDX_Control(pDX, IDC_GOTO_BASE, m_linkBase);
	DDX_Control(pDX, IDC_GOTO_GRAPH, m_linkGraph);
	DDX_Control(pDX, IDC_GOTO_REALTIME, m_linkRealTime);
	DDX_Control(pDX, IDC_COMBO_STOCK, m_cmbStock);
	DDX_Control(pDX, IDC_SOFTNO, m_editSoftNO);
	DDX_Control(pDX, IDC_LISTTYPE, m_btnListType);
	DDX_Control(pDX, IDC_STATIC_SOFTNOTITLE, m_staticSoftNOTitle);
	DDX_Control(pDX, IDC_GOPURCHASE, m_linkGoPurchase);
	DDX_Control(pDX, IDC_HOWTO_REG, m_linkHowtoReg);
	DDX_Control(pDX, IDC_BTNHELP, m_btnHelp);
	DDX_Control(pDX, IDC_BTNOPENSTRATEGY, m_btnOpenStrategy);
	DDX_Control(pDX, IDC_BTNNEWSTRATEGY, m_btnNewStrategy);
	DDX_Control(pDX, IDC_STRATEGYLIST, m_listRecent);
	//}}AFX_DATA_MAP
}

BOOL CWizardView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void RunLinkRealTime()
{
	CWizardView * pView = AfxGetWizardView();
	if( pView && ::IsWindow(pView->GetSafeHwnd()) )
	{
		CString strCode = pView->m_cmbStock.GetSelectedStockCode();
		AfxShowStockRealTime( strCode );
	}
}

void RunLinkGraph()
{
	CWizardView * pView = AfxGetWizardView();
	if( pView && ::IsWindow(pView->GetSafeHwnd()) )
	{
		CString strCode = pView->m_cmbStock.GetSelectedStockCode();
		AfxShowStockGraph( strCode );
	}
}

void RunLinkBase()
{
	CWizardView * pView = AfxGetWizardView();
	if( pView && ::IsWindow(pView->GetSafeHwnd()) )
	{
		CString strCode = pView->m_cmbStock.GetSelectedStockCode();
		AfxShowStockBase( strCode );
	}
}

void RunLinkInfo()
{
	CWizardView * pView = AfxGetWizardView();
	if( pView && ::IsWindow(pView->GetSafeHwnd()) )
	{
		CString strCode = pView->m_cmbStock.GetSelectedStockCode();
		AfxShowStockInfo( strCode );
	}
}

void CWizardView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	EnableToolTips(TRUE);

	// TODO: Add extra initialization here
	m_listRecent.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
						| LVS_EX_FLATSB | LVS_EX_REGIONAL | LVS_EX_INFOTIP | LVS_EX_UNDERLINEHOT );
	m_listRecent.EnableToolTips( );

	// set image lists
	m_LargeImageList.Create(IDB_STRATEGYLARGE, 32, 1, RGB(0, 255, 0));
	m_SmallImageList.Create(IDB_STRATEGY, 16, 1, RGB(0, 255, 0));

	m_listRecent.SetImageList(&m_LargeImageList, LVSIL_NORMAL);
	m_listRecent.SetImageList(&m_SmallImageList, LVSIL_SMALL);

	// list buttons
	//m_btnListType.m_menu.LoadMenu( IDR_POPUP_WIZARDVIEWLISTTYPE );

	// insert columns
	CString	strName, strPathName, strModTime, strStatus;
	strName.LoadString( IDS_STRATEGYNAME );
	strPathName.LoadString( IDS_STRATEGYPATH );
	strModTime.LoadString( IDS_STRATEGYMODTIME );
	strStatus.LoadString( IDS_STRATEGYOPENSTATUS );
	m_listRecent.InsertColumn( 0, strName, LVCFMT_CENTER, 122 );
	m_listRecent.InsertColumn( 1, strPathName, LVCFMT_CENTER, 152 );
	m_listRecent.InsertColumn( 2, strModTime, LVCFMT_CENTER, 82 );
	m_listRecent.InsertColumn( 3, strStatus, LVCFMT_CENTER, 62 );

	// Show Large Icons
	OnWizardviewLargeicons( );

	// link go purchase
	if( AfxGetSView().IsEva() )
	{
		CString strSoftNO	=	AfxGetSView().GetS();
		if( !strSoftNO.IsEmpty() )
			m_editSoftNO.SetWindowText( strSoftNO );
		m_linkGoPurchase.SetURL( (LPCTSTR)AfxGetProfile().GetPurchaseURL() );
		m_linkHowtoReg.SetModeHelp( 0, "StockAna.chm::/htm/purchase.htm" );
	}
	else
	{
		m_staticSoftNOTitle.SetWindowPos( NULL, 0,0,0,0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
		m_editSoftNO.SetWindowPos( NULL, 0,0,0,0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
		m_linkGoPurchase.SetWindowPos( NULL, 0,0,0,0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
		m_linkHowtoReg.SetWindowPos( NULL, 0,0,0,0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE );
	}

	m_cmbStock.InitStocks( FALSE, FALSE, FALSE );
	m_cmbStock.SetAutoHide( FALSE );
	m_cmbStock.SelectString( 0, STKLIB_CODE_MAIN );

	m_linkRealTime.SetModeRun( RunLinkRealTime );
	m_linkGraph.SetModeRun( RunLinkGraph );
	m_linkBase.SetModeRun( RunLinkBase );
	m_linkInfo.SetModeRun( RunLinkInfo );

	OnUpdate( NULL, UPDATE_HINT_WIZARDVIEW, NULL );
}

void CWizardView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( UPDATE_HINT_WIZARDVIEW != lHint )
	{
		if( ::IsWindow( m_listRecent.GetSafeHwnd() ) )
			m_listRecent.Invalidate( );
		return;
	}
	if( IsWindowVisible() )
		SetFocus( );

	m_listRecent.DeleteAllItems( );

	//	insert items
	CStkUIApp * pApp = AfxGetStkUIApp();
	CRecentFileList * pRecent = NULL;
	if( pApp )
		pRecent = pApp->GetRecentFileList( );

	CString	strOpened;
	strOpened.LoadString( IDS_STRATEGYOPENED );

	CStringArray	astrAdded;

	int	count	=	0;
	if( pRecent )
	{
		for( int i=pRecent->GetSize()-1; i>=0; i-- )
		{
			CString strPath = (*pRecent)[i];
			if( strPath.IsEmpty() )
				continue;
			if( 0 != access(strPath,0) )
			{
				pRecent->Remove(i);
				continue;
			}

			TCHAR	szFullPath[_MAX_PATH];
			AfxFullPath( szFullPath, strPath );
			strPath	=	szFullPath;

			CString strName	=	CStrategy::GetName( strPath );
			BOOL	bOpened	=	pApp->IsFileOpened( strPath );
			int nItem = m_listRecent.InsertItem( count, strPath, bOpened ? 1 : 0 );
			m_listRecent.SetItemText( nItem, 0, strName );
			m_listRecent.SetItemText( nItem, 1, strPath );
			if( bOpened )
				m_listRecent.SetItemText( nItem, 3, strOpened );
			astrAdded.Add( strPath );
			
			CString	strTime;
			CFileStatus	status;
			if( CFile::GetStatus( strPath, status ) )
			{
				strTime = status.m_mtime.Format( "%Y-%m-%d" );
				m_listRecent.SetItemText( nItem, 2, strTime );
			}
			count ++;
		}
	}

	CString strExt	=	AfxGetStrategyFileExt( );
	if( !strExt.IsEmpty())
	{
		CFileFind finder;
		BOOL	bWorking	=	finder.FindFile( AfxGetFilePath( (LPCTSTR)AfxGetProfile().GetProjectPath(), LPCTSTR("*" + strExt) ) );
		while( bWorking )
		{
			bWorking	=	finder.FindNextFile();
			CString	strPath	=	finder.GetFilePath();

			int i;
			for( i=0; i<astrAdded.GetSize(); i++ )
			{
				if( 0 == strPath.CompareNoCase( astrAdded.ElementAt(i) ) )
					break;
			}
			if( i < astrAdded.GetSize() )
				continue;

			CString strName	=	CStrategy::GetName( strPath );
			BOOL	bOpened	=	pApp->IsFileOpened( strPath );
			int nItem = m_listRecent.InsertItem( count, strPath, bOpened ? 1 : 0 );
			m_listRecent.SetItemText( nItem, 0, strName );
			m_listRecent.SetItemText( nItem, 1, strPath );
			if( bOpened )
				m_listRecent.SetItemText( nItem, 3, strOpened );

			CString	strTime;
			CFileStatus	status;
			if( CFile::GetStatus( strPath, status ) )
			{
				strTime = status.m_mtime.Format( "%Y-%m-%d" );
				m_listRecent.SetItemText( nItem, 2, strTime );
			}
			count ++;

			if( count >= 256 )
				break;
		}
		finder.Close();
	}

	m_listRecent.Invalidate( );
}

/////////////////////////////////////////////////////////////////////////////
// CWizardView diagnostics

#ifdef _DEBUG
void CWizardView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWizardView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWizardView message handlers

int CWizardView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CWizardView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if( ::IsWindow( m_listRecent.GetSafeHwnd() ) )
		m_listRecent.Invalidate( );
}

void CWizardView::OnSize(UINT nType, int cx, int cy) 
{
	CSize	sizeTotal( cx, cy );
	SetScaleToFitSize( sizeTotal );

	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

void CWizardView::OnSetFocus(CWnd* pOldWnd) 
{
	CFormView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	if( ::IsWindow( m_listRecent.GetSafeHwnd() ) )
		m_listRecent.Invalidate( );
}

LRESULT CWizardView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString( IDS_TITLE_WIZARDVIEW );

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

LRESULT CWizardView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((LPARAM *)lParam)	=	ID_VIEW_WIZARD;
	return 0L;
}

HBRUSH CWizardView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( nCtlColor == CTLCOLOR_DLG
		|| nCtlColor == CTLCOLOR_STATIC
		|| nCtlColor == CTLCOLOR_MSGBOX )
	{
		if( nCtlColor == CTLCOLOR_STATIC )
			pDC->SetBkMode( TRANSPARENT );

		static CBrush brushBK;
		static COLORREF	clrBKLast	=	RGB(255,255,255);
		COLORREF	clrBKNow	=	RGB(255,255,255);
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

void CWizardView::OnBtnnewstrategy() 
{
	// TODO: Add your control notification handler code here
	CStkUIApp * pApp = AfxGetStkUIApp();
	if( pApp )
		pApp->OnStrategyNew( );
}

void CWizardView::OnBtnopenstrategy() 
{
	// TODO: Add your control notification handler code here
	CStkUIApp * pApp = AfxGetStkUIApp();
	if( pApp )
		pApp->OnFileOpen( );
}

void CWizardView::OnBtnhelp() 
{
	::HtmlHelp(NULL, "StockAna.chm::/htm/index.htm", HH_DISPLAY_TOPIC, 0);
}

void CWizardView::OnDblclkStrategylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	for( int i=0; i<m_listRecent.GetItemCount( ); i++ )
	{
		if( LVIS_SELECTED == m_listRecent.GetItemState(i,LVIS_SELECTED) )
		{
			CString strPath = m_listRecent.GetItemText( i, 1 );
			if( NULL != AfxGetStkUIApp()->OpenStrategyFile( strPath ) )
				AfxSwitchToStaticView( RUNTIME_CLASS(CSimuView) );
		}
	}
	
	*pResult = 0;
}


void CWizardView::OnWizardviewLargeicons() 
{
	// TODO: Add your command handler code here
	if( (m_listRecent.GetStyle() & LVS_TYPEMASK) != LVS_ICON )
		m_listRecent.ModifyStyle(LVS_TYPEMASK,LVS_ICON);
}

void CWizardView::OnWizardviewSmallicons() 
{
	// TODO: Add your command handler code here
	if( (m_listRecent.GetStyle() & LVS_TYPEMASK) != LVS_SMALLICON )
		m_listRecent.ModifyStyle(LVS_TYPEMASK,LVS_SMALLICON);
}

void CWizardView::OnWizardviewList() 
{
	// TODO: Add your command handler code here
	if( (m_listRecent.GetStyle() & LVS_TYPEMASK) != LVS_LIST )
		m_listRecent.ModifyStyle(LVS_TYPEMASK,LVS_LIST);
}

void CWizardView::OnWizardviewDetails() 
{
	// TODO: Add your command handler code here
	if( (m_listRecent.GetStyle() & LVS_TYPEMASK) != LVS_REPORT )
		m_listRecent.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
}

void CWizardView::OnUpdateWizardviewLargeicons(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( (m_listRecent.GetStyle() & LVS_TYPEMASK) == LVS_ICON );
}

void CWizardView::OnUpdateWizardviewSmallicons(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( (m_listRecent.GetStyle() & LVS_TYPEMASK) == LVS_SMALLICON );
}

void CWizardView::OnUpdateWizardviewList(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( (m_listRecent.GetStyle() & LVS_TYPEMASK) == LVS_LIST );
}

void CWizardView::OnUpdateWizardviewDetails(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( (m_listRecent.GetStyle() & LVS_TYPEMASK) == LVS_REPORT );
}

