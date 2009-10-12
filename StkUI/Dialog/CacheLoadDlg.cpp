// CacheLoadDlg.cpp : implementation file
//

#include "stdafx.h"

#include "CacheLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCacheLoadDlg dialog


CCacheLoadDlg::CCacheLoadDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CCacheLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCacheLoadDlg)
	m_tmDate = 0;
	//}}AFX_DATA_INIT

	DWORD dwDate = 0;
	CSPTime sptime;
	if( AfxGetStockContainer().GetPioneerTechDate( &dwDate )
		&& sptime.FromStockTimeDay( dwDate ) )
	{
		m_tmDate = sptime.GetTime();
	}
	else
	{
		m_tmDate = CSPTime::GetCurrentTime();
		CSPTimeSpan span(AfxGetProfile().GetCacheDays()*7/5,0,0,0);
		m_tmDate = m_tmDate - span;
	}
}


void CCacheLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCacheLoadDlg)
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_tmDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCacheLoadDlg, CXFDialog)
	//{{AFX_MSG_MAP(CCacheLoadDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCacheLoadDlg message handlers

BOOL CCacheLoadDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CALLBACK CacheLoadCallback(DWORD dwType, DWORD dwValue,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND hWnd = (HWND)cookie;

	if( PROG_PROGRESS == dwType )
	{
		::SendMessage( hWnd, WM_USER_WAITPROGRESS, dwValue, (LPARAM)lpszMsg );
	}

	MSG		msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		AfxGetApp()->PumpMessage();

	return TRUE;
}

void CCacheLoadDlg::OnOK() 
{
	UpdateData();

	int nCacheDays = -1;

	CSPTime sptime = m_tmDate.GetTime();
	CKData & kdata = AfxGetStockMain().GetKDataDay();
	DWORD dwDate = sptime.ToStockTimeDay();
	int nIndex = kdata.GetAboutIndexByDate(dwDate);
	if( nIndex >= 0 )
		nCacheDays = kdata.GetSize() - nIndex + 1;
	else
		nCacheDays = kdata.GetSize();

	AfxGetProfile().SetCacheDays( nCacheDays );

	// LOAD
	CWaitDlg	dlg;

	CWaitDlg	wait( AfxGetMainWnd(), FALSE );
	wait.SetProgressRange( 0, STKLIB_MAX_PROGRESS );

	AfxGetDB().LoadKDataCache( AfxGetStockContainer(), CacheLoadCallback, wait.GetSafeHwnd(), 0, STKLIB_MAX_PROGRESS );

	wait.DestroyWindow();

	//CMainFrame	* pMainFrame	=	AfxGetMainFrame();
	//if( pMainFrame )
	//	pMainFrame->m_DateBox.InitDates( );
	
	CXFDialog::OnOK();
}

