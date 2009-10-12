// InfoView.cpp : implementation file
//

#include "stdafx.h"

#include "../StaticDoc.h"
#include "InfoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoView

IMPLEMENT_DYNCREATE(CInfoView, CHtmlView)

CInfoView::CInfoView()
{
	//{{AFX_DATA_INIT(CInfoView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	SetCurrentServer( AfxGetProfile().GetInfoCurrentServer( ) );

	m_nServerShowed	=	serverUnknown;
}

CInfoView::~CInfoView()
{
	AfxGetProfile().SetInfoCurrentServer( m_nServer );
}

void CInfoView::SetCurrentServer( int nServer )
{
	m_nServer	=	nServer;
	if( nServer < serverMin || nServer > serverMax )
		m_nServer	=	serverSelf;
}

int CInfoView::GetCurrentServer( )
{
	return m_nServer;
}

void CInfoView::OnRefresh( )
{
	OnUpdate( NULL, UPDATE_HINT_INFOVIEW, NULL );
}

char	szInfoSelf[]	=	"http://%s/info/%s/?market=%s&code=%s";

char	szInfoF10[]		=	"http://www.f10.com.cn/ggzx/ggzl.asp?zqdm=%s";

char	szInfoStockstar[]	=	"http://quote.stockstar.com/stock/external_chart.asp?code=%s";
char	szInfoSohu[]		=	"http://stock.sohu.com/stock_image/fenshi.php?code=%s";
char	szInfoSinaSz[]	=	"http://stock.sina.com.cn/cgi-bin/stock/quote/minline.cgi?symbol=%s&country=sz";
char	szInfoSinaSh[]	=	"http://stock.sina.com.cn/cgi-bin/stock/quote/minline.cgi?symbol=%s&country=sh";
char	szInfo163[]		=	"http://stock2.163.com/stock/show_full.php?code=%s";

char	szInfoShse[]	=	"http://www.sse.com.cn/";
char	szInfoSzse[]	=	"http://www.szse.cn/";
char	szInfoHkex[]	=	"http://www.hkex.com.hk/";
char	szInfoLse[]		=	"http://www.londonstockexchange.com/";
char	szInfoNyse[]	=	"http://www.nyse.com/";
char	szInfoNasdaq[]	=	"http://www.nasdaq.com/";
char	szInfoP5w[]		=	"http://www.p5w.net/docs/stimes/today/";
char	szInfoSsnews[]	=	"http://www.ssnews.com.cn/shzqb/";
char	szInfoCs[]		=	"http://www.cs.com.cn/csnews/jrbz.htm";

void CInfoView::ShowInfo( CStockInfo & info )
{
	CString	strURL;
	switch( m_nServer )
	{
	case	serverSelf:
		{
			char	szMarket[16];
			memset( szMarket, 0, sizeof(szMarket) );
			*((DWORD*)(szMarket))	=	info.GetMarket();
#ifdef	CLKLAN_ENGLISH_US
			strURL.Format( szInfoSelf, (LPCTSTR)AfxGetProfile().GetHTTPServerDomain(), "enu", szMarket, info.GetStockCode() );
#elif defined CLKVER_OEM
			strURL = CString("http://") + (LPCTSTR)AfxGetProfile().GetHTTPServerDomain();
#else
			strURL.Format( szInfoSelf, (LPCTSTR)AfxGetProfile().GetHTTPServerDomain(), "chs", szMarket, info.GetStockCode() );
#endif
		}
		break;
	case	serverF10:			strURL.Format( szInfoF10, info.GetStockCode() );			break;

	case	serverStockstar:	strURL.Format( szInfoStockstar, info.GetStockCode() );		break;
	case	serverSohu:			strURL.Format( szInfoSohu, info.GetStockCode() );			break;
	case	serverSina:			strURL.Format( info.IsShangHai() ? szInfoSinaSh : szInfoSinaSz, info.GetStockCode() );	break;
	case	server163:			strURL.Format( szInfo163, info.GetStockCode() );			break;

	case	serverShse:			strURL	=	szInfoShse;				break;
	case	serverSzse:			strURL	=	szInfoSzse;				break;
	case	serverHkex:			strURL	=	szInfoHkex;				break;
	case	serverLse:			strURL	=	szInfoLse;				break;
	case	serverNyse:			strURL	=	szInfoNyse;				break;
	case	serverNasdaq:		strURL	=	szInfoNasdaq;			break;
	case	serverP5w:			strURL	=	szInfoP5w;				break;
	case	serverSsnews:		strURL	=	szInfoSsnews;			break;
	case	serverCs:			strURL	=	szInfoCs;				break;

	default:
		strURL.Format( szInfoStockstar, info.GetStockCode() );
		break;
	}
	Navigate( strURL );
}

BOOL CInfoView::IsViewF10( )
{
	return ( serverF10 == GetCurrentServer() );
}

void CInfoView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CInfoView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if( bActivate )
		OnUpdate( NULL, UPDATE_HINT_INFOVIEW, NULL );
}


BEGIN_MESSAGE_MAP(CInfoView, CHtmlView)
	//{{AFX_MSG_MAP(CInfoView)
	ON_COMMAND(ID_VIEW_F10, OnViewF10)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoView diagnostics

#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CInfoView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInfoView message handlers

void CInfoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( UPDATE_HINT_INFOVIEW != lHint )
		return;
	if( IsWindowVisible() )
		SetFocus( );

	static	int	count	=	0;
	count	++;
	if( count <= 2 && !IsWindowVisible() )
		return;

	CStockInfo	info;
	if( AfxGetProfile().GetCurrentStock( &info ) )
	{
	}
	else if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAIN, &info ) )
	{
		AfxGetProfile().SetCurrentStock( STKLIB_CODE_MAIN, FALSE );
	}

	if( m_nServerShowed != m_nServer )
	{
		ShowInfo( info );
		m_infoCurrent	=	info;
		m_nServerShowed	=	m_nServer;
		return;
	}

	if( !m_infoCurrent.IsEqualTo(info.GetMarket(), info.GetStockCode()) )
	{
		if( serverSelf == m_nServer || serverF10 == m_nServer
			|| serverStockstar == m_nServer || serverSohu == m_nServer
			|| serverSina == m_nServer || server163 == m_nServer )
		{
			ShowInfo( info );
			m_infoCurrent	=	info;
			m_nServerShowed	=	m_nServer;
			return;
		}
	}
}

void CInfoView::OnViewF10() 
{
	SetCurrentServer( serverF10 );
	OnRefresh();
}

LRESULT CInfoView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString( IDS_TITLE_INFOVIEW );

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

LRESULT CInfoView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((LPARAM *)lParam)	=	ID_VIEW_INFO;
	return 0L;
}

