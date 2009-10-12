// SplashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SplashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog

#define	SPLASH_BKCOLOR		RGB(255,255,255)
#define	SPLASH_TEXTCOLOR	RGB(0,0,0)

CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CXFResDialog(CSplashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplashDlg)
	//}}AFX_DATA_INIT
}

CSplashDlg::~CSplashDlg( )
{
	if( GetSafeHwnd() )
		DestroyWindow( );
}

BOOL CSplashDlg::SetBitmap( HBITMAP hbmp )
{
	return m_bitmap.Attach( hbmp );
}

BOOL CSplashDlg::Create( CWnd* pParentWnd, int nShowTime, LPCTSTR lpszUser,
						LPCTSTR lpszRegCode, LPCTSTR lpszVersionString, 
						BOOL bAutoDelete, int nMin, int nMax )
{
	if( ! CXFResDialog::Create( CSplashDlg::IDD, pParentWnd) )
	{
		ASSERT( FALSE );
		return	FALSE;
	}
	ModifyStyleEx( 0, WS_EX_TOOLWINDOW );
	//ModifyStyleEx( 0, WS_EX_TRANSPARENT );

	if( nShowTime > 0 )
		SetTimer( 1, nShowTime, NULL );

	CString	strVersion	=	lpszVersionString;
	if( strVersion.GetLength() > 0 )
		m_staticVersion.SetWindowText( strVersion );

	CString	strUser;
	if( NULL != lpszUser )		strUser	=	lpszUser;
	if( strUser.IsEmpty() )		strUser	=	AfxModuleLoadString( IDS_XFAUTHORIZEDUSER_UNKNOWN );
	m_staticUser.SetWindowText( strUser );

	CString	strRegCode;
	if( NULL != lpszRegCode )	strRegCode	=	lpszRegCode;
	if( strRegCode.IsEmpty() )	strRegCode	=	AfxModuleLoadString( IDS_XFREGCODE_UNPURCHASED );
	m_staticRegCode.SetWindowText( strRegCode );

	m_bAutoDelete	=	bAutoDelete;

	SetProgressRange( nMin, nMax );

	return	TRUE;
}

void CSplashDlg::SetProgressRange(int nMin, int nMax)
{
	if( nMax <= nMin )
	{
		ASSERT( FALSE );
		nMax = nMin + 1;
	}
	m_ctrlProgress.SetRange( nMin, nMax );
}

void CSplashDlg::SetProgress(int nPos)
{
	if( !::IsWindow(m_ctrlProgress.GetSafeHwnd()) )
		return;

	int	nLower = 0, nUpper = 0;
	m_ctrlProgress.GetRange( nLower, nUpper );

	if( nLower < nUpper && nPos >= nLower && nPos <= nUpper )
	{
		m_ctrlProgress.SetPos(nPos);
	}
	else
		ASSERT( FALSE );
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFResDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashDlg)
	DDX_Control(pDX, IDC_XFPROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_XFVERSION, m_staticVersion);
	DDX_Control(pDX, IDC_XFUSER, m_staticUser);
	DDX_Control(pDX, IDC_XFREGCODE, m_staticRegCode);
	//}}AFX_DATA_MAP
}

BOOL CSplashDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST )
	{
		int	nVirtKey = (int) pMsg->wParam;
		if( VK_ESCAPE == nVirtKey
			|| VK_RETURN == nVirtKey )
		{
			return TRUE;
		}
	}
	
	return CXFResDialog::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CSplashDlg, CXFResDialog)
	//{{AFX_MSG_MAP(CSplashDlg)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_SPLASH_PROGRESS, OnSplashProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg message handlers

BOOL CSplashDlg::OnInitDialog() 
{
	CXFResDialog::OnInitDialog();

	CenterWindow();

	m_ctrlProgress.SetPos( 0 );


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSplashDlg::OnEraseBkgnd( CDC *pDC )
{
	return TRUE;
}

void CSplashDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rect;
	GetClientRect( rect );

	if( m_bitmap.GetSafeHandle() )
	{
		DrawTransparentBitmap( dc.GetSafeHdc(), (HBITMAP)m_bitmap.GetSafeHandle(), 0, 0, 0, 0, 
								rect.Width(), rect.Height(), RGB(0,255,0) );
	}

/*	
	CBitmap	bitmap;
	if( bitmap.LoadBitmap( IDB_SPLASH ) )
	{
		CDC dcMem;
		if (!dcMem.CreateCompatibleDC(&dc))
			return;
		CBitmap* pBitmapOld = dcMem.SelectObject(&bitmap);
		if (pBitmapOld == NULL)
			return;
		BITMAP	bmpInfo;
		bitmap.GetBitmap( &bmpInfo );

		dc.BitBlt(2, 2, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pBitmapOld);
	}
*/
}

void CSplashDlg::OnTimer(UINT nIDEvent) 
{
	if( 1 == nIDEvent )
	{
		DestroyWindow( );
		return;
	}
	CXFResDialog::OnTimer(nIDEvent);
}

void CSplashDlg::OnDestroy() 
{
	KillTimer( 1 );
	CXFResDialog::OnDestroy();
}

HBRUSH CSplashDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( CTLCOLOR_STATIC == nCtlColor && pDC )
	{
		pDC->SetBkColor( SPLASH_BKCOLOR );
		pDC->SetTextColor( SPLASH_TEXTCOLOR );
	}

	if( NULL == m_brushBK.GetSafeHandle() )
		m_brushBK.CreateSolidBrush( SPLASH_BKCOLOR );
	HBRUSH hbr = (HBRUSH)m_brushBK.GetSafeHandle();
	return hbr;
}

LRESULT CSplashDlg::OnSplashProgress(WPARAM wParam, LPARAM lParam)
{
	// set message and enable buttons
	SetProgress( wParam );

	return 1;
}

