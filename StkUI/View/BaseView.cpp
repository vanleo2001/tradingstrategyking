// BaseView.cpp : implementation of the CBaseView class
//

#include "stdafx.h"

#include "../StaticDoc.h"
#include "BaseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseView

IMPLEMENT_DYNCREATE(CBaseView, CEditView)

BEGIN_MESSAGE_MAP(CBaseView, CEditView)
	//{{AFX_MSG_MAP(CBaseView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(EN_CHANGE, OnEditChange)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
	ON_MESSAGE(WM_APP_STKRECEIVER_DATA, OnStkReceiverData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseView construction/destruction

CBaseView::CBaseView()
{
	// TODO: add construction code here
}

CBaseView::~CBaseView()
{
}

BOOL CBaseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	// cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

BOOL CBaseView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CBaseView::SetFont( LPLOGFONT pLogFont )
{
	ASSERT( pLogFont );

	m_font.DeleteObject();
	m_font.CreateFontIndirect(pLogFont);
	SendMessage( WM_SETFONT, (WPARAM)m_font.GetSafeHandle(), MAKELPARAM(1,0) );
}

void CBaseView::BeginNewLine( )
{
	GetEditCtrl().SetSel( -1, -1 );

	int nLineCount = GetEditCtrl().GetLineCount();
	if( nLineCount >= 1 )
	{
		//int nLineIndex = GetEditCtrl().LineIndex( nLineCount-1 );
		//int nLineLen = GetEditCtrl().LineLength( nLineIndex );
		CPoint point = GetEditCtrl().PosFromChar( UINT(-1)/*nLineIndex+nLineLen*/ );
		GetEditCtrl().SetCaretPos( point );
	}

	CString	string	=	STRING_CRLF;
	GetEditCtrl().ReplaceSel( string );
}

void CBaseView::OutputMsgHeader( )
{
	CString	string = "-->";
	GetEditCtrl().ReplaceSel( string );
}

void CBaseView::OutputMsgLine( const char * line )
{
	BeginNewLine( );
	OutputMsgHeader( );
	m_astrMsg.Add( line );
	GetEditCtrl().ReplaceSel( line );
}

void CBaseView::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
	CEditView::Serialize( ar );
}

/////////////////////////////////////////////////////////////////////////////
// CBaseView drawing

void CBaseView::OnDraw(CDC* pDC)
{
	CStaticDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CBaseView printing

BOOL CBaseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBaseView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CEditView::OnBeginPrinting( pDC, pInfo );
}

void CBaseView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CEditView::OnEndPrinting( pDC, pInfo );
}

void CBaseView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add customized printing code here
//	CRect	rectPrint	=	pInfo->m_rectDraw;
//	pDC->OffsetViewportOrg( rectPrint.Width()/20, 0 );
	CEditView::OnPrint( pDC, pInfo );
}

/////////////////////////////////////////////////////////////////////////////
// CBaseView diagnostics

#ifdef _DEBUG
void CBaseView::AssertValid() const
{
	CEditView::AssertValid();
}

void CBaseView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBaseView message handlers

int CBaseView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	GetEditCtrl().SetReadOnly( );
	GetEditCtrl().SetMargins( 20, 20 );

	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	AfxGetProfile().GetFontBaseView( &lf );
	SetFont( &lf );

	// 实时行情刷新
	AfxGetStkReceiver().AddRcvDataWnd( GetSafeHwnd() );

	return 0;
}

void CBaseView::OnDestroy() 
{
	// 停止行情刷新通知消息
	AfxGetStkReceiver().RemoveRcvDataWnd( GetSafeHwnd() );

	CEditView::OnDestroy();
}

void CBaseView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if( bActivate )
	{
		OnUpdate( NULL, UPDATE_HINT_BASEVIEW, NULL );
		CWnd * pParent = GetParent();
		if( pParent && ::IsWindow(pParent->GetSafeHwnd()) )
			pParent->Invalidate( );
	}
}

void CBaseView::OnEditChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEditView::OnInitDialog()
	// function and call CEditView().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}

HBRUSH CBaseView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	if( pDC )
	{
		pDC->SetBkColor( AfxGetProfile().GetColor(CColorClass::clrBaseBK) );
		pDC->SetTextColor( AfxGetProfile().GetColor(CColorClass::clrBaseText) );
	}

	static CBrush brushBK;
	static COLORREF	clrBKLast	=	RGB(255,255,255);
	COLORREF	clrBKNow	=	AfxGetProfile().GetColor( CColorClass::clrBaseBK );
	if( NULL == brushBK.GetSafeHandle() || clrBKLast != clrBKNow )
	{
		brushBK.DeleteObject( );
		VERIFY( brushBK.CreateSolidBrush( clrBKNow ) );
	}
	clrBKLast	=	clrBKNow;
	return (HBRUSH)brushBK.GetSafeHandle();
}

LRESULT CBaseView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString( IDS_TITLE_BASEVIEW );

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

LRESULT CBaseView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((LPARAM *)lParam)	=	ID_VIEW_BASE;
	return 0L;
}

LRESULT CBaseView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	Invalidate( );
	return 0L;
}

LRESULT CBaseView::OnStkReceiverData(WPARAM wParam, LPARAM lParam)
{
	PCOMMPACKET	pCommPacket	=	(PCOMMPACKET)lParam;

	switch( wParam )
	{
	case CStock::dataBasetext:
		if( pCommPacket && CStock::dataCode == pCommPacket->m_dwDataType )
		{
			for( DWORD i=0; i<pCommPacket->m_dwCount; i++ )
			{
				if( m_stock.GetStockInfo().IsEqualTo( pCommPacket->m_pStockcode[i].m_dwMarket, pCommPacket->m_pStockcode[i].m_szCode ) )
					OnUpdate( NULL, UPDATE_HINT_BASEVIEW, NULL );
			}
		}
		break;
	}
	return 0L;
}

void CBaseView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	OnUpdate( NULL, UPDATE_HINT_BASEVIEW, NULL );
}

void CBaseView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( UPDATE_HINT_BASEVIEW != lHint )
		return;
	if( IsWindowVisible() )
		SetFocus( );

	CStockInfo	info;
	if( !AfxGetProfile().GetCurrentStock( &info ) )
		return;
	
	CString	strHeader	=	info.GetStockCode();
	strHeader	+=	"  ";
	strHeader	+=	info.GetStockName();
	strHeader	+=	"\r\n\r\n ";
	GetEditCtrl().SetWindowText( strHeader );
	GetEditCtrl().SetSel( strHeader.GetLength()-1, strHeader.GetLength()-1 );

	m_stock.Clear( );
	m_stock.SetStockInfo( &info );
	m_stock.SetDatabase( &AfxGetDB() );

	m_stock.PrepareBaseInfo( TRUE );

	char	* pText	=	m_stock.GetBaseTextPtr();
	int		nLength	=	m_stock.GetBaseTextLength();
	if( pText && nLength > 0 )
	{
		GetEditCtrl().ReplaceSel( pText );
	}
	else
	{
		CString	strNoBaseData;
		strNoBaseData.LoadString( IDS_BASEVIEW_NOBASEDATA );
		GetEditCtrl().ReplaceSel( strNoBaseData );
	}

	GetEditCtrl().SetSel( 0, 0 );

	AfxGetStkReceiver().RequestStockData( CStock::dataBasetext, &info, 1, 0, 0 );
}

