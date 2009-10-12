// SetBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetBaseDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg property page

IMPLEMENT_DYNCREATE(CSetBaseDlg, CPropertyPageEx)

CSetBaseDlg::CSetBaseDlg() : CPropertyPageEx(CSetBaseDlg::IDD)
{
	//{{AFX_DATA_INIT(CSetBaseDlg)
	m_strFileTdx = _T("");
	m_strFileFxj = _T("");
	m_strPathFxjBlock = _T("");
	m_strFileF10 = _T("");
	//}}AFX_DATA_INIT

	m_strFileF10	=	AfxGetApp()->GetProfileString( "setbasedlg", "filef10", "" );
	m_strFileTdx	=	AfxGetApp()->GetProfileString( "setbasedlg", "filetdx", "" );
	m_strFileFxj	=	AfxGetApp()->GetProfileString( "setbasedlg", "filefxj", "" );
	m_strPathFxjBlock	=	AfxGetApp()->GetProfileString( "setbasedlg", "pathfxjblock", "" );
}

CSetBaseDlg::~CSetBaseDlg()
{
}

void CSetBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetBaseDlg)
	DDX_Control(pDX, IDC_FXJBLOCKPATH_EXPLORER, m_btnFxjBlockPathExplorer);
	DDX_Control(pDX, IDC_F10FILE_EXPLORER, m_btnF10FileExplorer);
	DDX_Control(pDX, IDC_FXJFILE_EXPLORER, m_btnFxjFileExplorer);
	DDX_Control(pDX, IDC_TDXFILE_EXPLORER, m_btnTdxFileExplorer);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_staticProgress);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Text(pDX, IDC_FILE_TDX, m_strFileTdx);
	DDX_Text(pDX, IDC_FILE_FXJ, m_strFileFxj);
	DDX_Text(pDX, IDC_PATH_FXJBLOCK, m_strPathFxjBlock);
	DDX_Text(pDX, IDC_FILE_F10, m_strFileF10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetBaseDlg, CPropertyPageEx)
	//{{AFX_MSG_MAP(CSetBaseDlg)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_TDXFILE_EXPLORER, OnTdxfileExplorer)
	ON_BN_CLICKED(IDC_F10FILE_EXPLORER, OnF10fileExplorer)
	ON_BN_CLICKED(IDC_FXJFILE_EXPLORER, OnFxjfileExplorer)
	ON_BN_CLICKED(IDC_FXJBLOCKPATH_EXPLORER, OnFxjblockpathExplorer)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_INSTALLBASEPROGRESS, OnInstallBaseProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg message handlers

BOOL CSetBaseDlg::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetBaseDlg::OnF10fileExplorer() 
{
	UpdateData( );

	CKSFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING,
		"Base Package files (*.zip,*.txt)|*.zip;*.txt|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strFileF10	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CSetBaseDlg::OnTdxfileExplorer() 
{
	UpdateData( );

	CKSFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING,
		"Tdx Finance Data files (*.zip,*.dbf)|*.zip;*.dbf|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strFileTdx	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CSetBaseDlg::OnFxjfileExplorer() 
{
	UpdateData( );

	CKSFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING,
		"Fxj Finance Data files (*.zip,*.fin)|*.zip;*.fin|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strFileFxj	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CSetBaseDlg::OnFxjblockpathExplorer() 
{
	UpdateData( );

	if( AfxDoSelectDirectory( m_strPathFxjBlock ) )
		UpdateData( FALSE );
}

LRESULT CSetBaseDlg::OnInstallBaseProgress(WPARAM wParam, LPARAM lParam)
{
	// Progress
	m_ctrlProgress.SetPos( wParam );
	CString	strProgressNew;
	strProgressNew.Format( "%.2f%%", 100.*wParam/STKLIB_MAXF_PROGRESS );
	CString	strProgressOld;
	m_staticProgress.GetWindowText( strProgressOld );
	if( 0 != strProgressNew.Compare( strProgressOld ) )
		m_staticProgress.SetWindowText( strProgressNew );
	return 0L;
}

BOOL CALLBACK InstallBaseCallback(DWORD dwCode, DWORD dwProgress,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND	hMainWnd	=	(HWND)cookie;

	if( PROG_PROGRESS == dwCode && ::IsWindow(hMainWnd) )
	{
		::SendMessage( hMainWnd, WM_USER_INSTALLBASEPROGRESS, dwProgress, (LPARAM)lpszMsg );
	}

	return TRUE;
}

void CSetBaseDlg::OnStart() 
{
	UpdateData( );

	CNetDatabase	netdb;
	if( !netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
	{
		AfxMessageBox( IDS_ROOTPATH_ERROR, MB_OK | MB_ICONINFORMATION );
		return;
	}

	if( ( m_strFileF10.IsEmpty() && m_strFileTdx.IsEmpty() && m_strFileFxj.IsEmpty() && m_strPathFxjBlock.IsEmpty() )
		|| ( 0 != access(m_strFileF10,0) && 0 != access(m_strFileTdx,0) && 0 != access(m_strFileFxj,0) && 0 != access(m_strPathFxjBlock,0) ) )
	{
		AfxMessageBox( IDS_SETBASE_FILENOTEXISTS, MB_OK|MB_ICONINFORMATION );
		return;
	}

	m_btnStart.EnableWindow( FALSE );

	AfxGetApp()->WriteProfileString( "setbasedlg", "filef10", m_strFileF10 );
	AfxGetApp()->WriteProfileString( "setbasedlg", "filetdx", m_strFileTdx );
	AfxGetApp()->WriteProfileString( "setbasedlg", "filefxj", m_strFileFxj );
	AfxGetApp()->WriteProfileString( "setbasedlg", "pathfxjblock", m_strPathFxjBlock );

	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
	m_ctrlProgress.SetRange( 0, STKLIB_MAX_PROGRESS );
	m_ctrlProgress.SetPos( 0 );

	if( m_strFileF10.GetLength() > 0 )
	{
		CPackage	pac;
		pac.m_nType		=	CPackage::packageBasetext;
		pac.m_bIsZipped	=	( m_strFileF10.GetLength() > 4 && 0 == m_strFileF10.Right(4).CompareNoCase(".zip") );
		pac.m_strDescript	=	m_strFileF10;
		if( netdb.InstallPackageEx( pac, m_strFileF10, InstallBaseCallback, GetSafeHwnd() ) )
			AfxMessageBox( IDS_SETBASE_INSTALLF10OK, MB_OK|MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_SETBASE_INSTALLF10FAIL, MB_OK|MB_ICONINFORMATION );
	}
	
	if( m_strFileTdx.GetLength() > 0 )
	{
		CPackage	pac;
		pac.m_nType		=	CPackage::packageBasetable;
		pac.m_bIsZipped	=	( m_strFileTdx.GetLength() > 4 && 0 == m_strFileTdx.Right(4).CompareNoCase(".zip") );
		pac.m_strDescript	=	m_strFileTdx;
		if( netdb.InstallPackageEx( pac, m_strFileTdx, NULL, NULL ) )
			AfxMessageBox( IDS_SETBASE_INSTALLTDXOK, MB_OK|MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_SETBASE_INSTALLTDXFAIL, MB_OK|MB_ICONINFORMATION );
	}
	
	if( m_strFileFxj.GetLength() > 0 )
	{
		CPackage	pac;
		pac.m_nType		=	CPackage::packageBasetable;
		pac.m_bIsZipped	=	( m_strFileFxj.GetLength() > 4 && 0 == m_strFileFxj.Right(4).CompareNoCase(".zip") );
		pac.m_strDescript	=	m_strFileFxj;
		if( netdb.InstallPackageEx( pac, m_strFileFxj, NULL, NULL ) )
			AfxMessageBox( IDS_SETBASE_INSTALLFXJOK, MB_OK|MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_SETBASE_INSTALLFXJFAIL, MB_OK|MB_ICONINFORMATION );
	}

	if( m_strPathFxjBlock.GetLength() > 0 )
	{
		CPackage	pac;
		pac.m_nType		=	CPackage::packageCode;
		pac.m_bIsZipped	=	( m_strPathFxjBlock.GetLength() > 4 && 0 == m_strPathFxjBlock.Right(4).CompareNoCase(".zip") );
		pac.m_strDescript	=	m_strPathFxjBlock;

		BOOL bOK = FALSE;
		int	nFileCountTotal	=	CNetDatabase::GetFileCount( m_strPathFxjBlock, FALSE );

		DWORD	attr = GetFileAttributes( m_strPathFxjBlock );
		if( 0xFFFFFFFF != attr && (attr & FILE_ATTRIBUTE_DIRECTORY) )
			bOK =	netdb.InstallPackagePath( pac, m_strPathFxjBlock, InstallBaseCallback, GetSafeHwnd(), nFileCountTotal, 0, FALSE );
		else
			bOK =	netdb.InstallPackage( pac, m_strPathFxjBlock, InstallBaseCallback, GetSafeHwnd() );

		if( bOK )
		{
			AfxGetDomainContainer().Store( AfxGetProfile().GetDomainFile() );
			AfxMessageBox( IDS_SETBASE_INSTALLFXJBLOCKOK, MB_OK|MB_ICONINFORMATION );
		}
		else
			AfxMessageBox( IDS_SETBASE_INSTALLFXJBLOCKFAIL, MB_OK|MB_ICONINFORMATION );
	}

	m_ctrlProgress.SetPos( STKLIB_MAX_PROGRESS );
	m_btnStart.EnableWindow( TRUE );

	AfxGetStockContainer().ReloadBase( &AfxGetDB() );
	AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy() );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_BASEVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
}

