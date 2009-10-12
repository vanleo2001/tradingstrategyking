// TyDataDlg.cpp : implementation file
//

#include "stdafx.h"

#include "TyDataDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTyDataDlg dialog

void CTyDataDlg::StoreProfile( )
{
	AfxGetApp()->WriteProfileString( "tydata", "pathsrcsh", m_strPathSrcSH );
	AfxGetApp()->WriteProfileString( "tydata", "pathsrcsz", m_strPathSrcSZ );
	AfxGetApp()->WriteProfileString( "tydata", "filedest", m_strFileDest );
}

void CTyDataDlg::LoadProfile( )
{
	m_strFileDest	=	AfxGetProfile().GetExportPath() + "Tysj.stk";

	m_strPathSrcSH	=	AfxGetApp()->GetProfileString( "tydata", "pathsrcsh", m_strPathSrcSH );
	m_strPathSrcSZ	=	AfxGetApp()->GetProfileString( "tydata", "pathsrcsz", m_strPathSrcSZ );
	m_strFileDest	=	AfxGetApp()->GetProfileString( "tydata", "filedest", m_strFileDest );

	if( (0 != access( m_strPathSrcSH, 0 ) || FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( m_strPathSrcSH ) ) )
		m_strPathSrcSH	=	AfxGetProfile().GetWorkDirectory() + "data\\shase\\day\\";
	if( (0 != access( m_strPathSrcSZ, 0 ) || FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( m_strPathSrcSZ ) ) )
		m_strPathSrcSZ	=	AfxGetProfile().GetWorkDirectory() + "data\\sznse\\day\\";
}

IMPLEMENT_DYNCREATE(CTyDataDlg, CPropertyPageEx)

CTyDataDlg::CTyDataDlg( )
	: CPropertyPageEx(CTyDataDlg::IDD)
{
	//{{AFX_DATA_INIT(CTyDataDlg)
	m_strFileDest = _T("");
	m_tmBegin = 0;
	m_tmEnd = 0;
	m_strPathSrcSH = _T("");
	m_strPathSrcSZ = _T("");
	//}}AFX_DATA_INIT
	m_tmBegin = CSPTime::GetCurrentTime();
	m_tmEnd = CSPTime::GetCurrentTime();
}

void CTyDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTyDataDlg)
	DDX_Control(pDX, IDC_SRCSZ_EXPLORER, m_btnSrcSZExplorer);
	DDX_Control(pDX, IDC_SRCSH_EXPLORER, m_btnSrcSHExplorer);
	DDX_Control(pDX, IDC_FILE_EXPLORER, m_btnFileExplorer);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_staticProgress);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Text(pDX, IDC_FILE_DEST, m_strFileDest);
	DDX_DateTimeCtrl(pDX, IDC_TIME_BEGIN, m_tmBegin);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_tmEnd);
	DDX_Text(pDX, IDC_PATH_SRCSH, m_strPathSrcSH);
	DDX_Text(pDX, IDC_PATH_SRCSZ, m_strPathSrcSZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTyDataDlg, CPropertyPageEx)
	//{{AFX_MSG_MAP(CTyDataDlg)
	ON_BN_CLICKED(IDC_FILE_EXPLORER, OnFileExplorer)
	ON_BN_CLICKED(IDC_SRCSH_EXPLORER, OnSrcshExplorer)
	ON_BN_CLICKED(IDC_SRCSZ_EXPLORER, OnSrcszExplorer)
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTyDataDlg message handlers

BOOL CTyDataDlg::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	LoadProfile( );
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTyDataDlg::OnFileExplorer() 
{
	UpdateData( );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_ENABLESIZING,
		"General Stock Data files (*.stk)|*.stk|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strFileDest	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CTyDataDlg::OnSrcshExplorer() 
{
	UpdateData( TRUE );
	if( AfxDoSelectDirectory( m_strPathSrcSH ) )
		UpdateData( FALSE );
}

void CTyDataDlg::OnSrcszExplorer() 
{
	UpdateData( TRUE );
	if( AfxDoSelectDirectory( m_strPathSrcSZ ) )
		UpdateData( FALSE );
}

void CTyDataDlg::OnStart() 
{
	UpdateData( );
	StoreProfile();

	if( m_tmBegin > m_tmEnd )
	{
		AfxMessageBox( IDS_TYDATADLG_ERRORTIME, MB_OK|MB_ICONINFORMATION );
		return;
	}

	if( 0 == access(m_strFileDest,0)
		&& IDNO == AfxMessageBox(IDS_TYDATADLG_OVERWRITE,MB_YESNO) )
		return;

	CFile	file;
	if( !file.Open( m_strFileDest, CFile::modeCreate | CFile::modeWrite ) )
	{
		AfxMessageBox( IDS_TYDATADLG_ERRORCREATEFILE, MB_OK|MB_ICONINFORMATION );
		return;
	}
	file.Close();

	m_btnStart.EnableWindow( FALSE );

	int	nFileCountTotal	=	CNetDatabase::GetFileCount( m_strPathSrcSH, FALSE )
							+ CNetDatabase::GetFileCount( m_strPathSrcSZ, FALSE );
	int	nFileCount = 0, nCount = 0, nProgress = 0;
	
	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
	m_ctrlProgress.SetRange( 0, nFileCountTotal );
	m_ctrlProgress.SetPos( 0 );

	CFileFind finder;
	// ÉÏº£
	BOOL	bWorking	=	finder.FindFile( AfxGetFilePath( (LPCTSTR)m_strPathSrcSH, LPCTSTR("*.day") ) );
	while( bWorking )
	{
		bWorking	=	finder.FindNextFile();
		CString	sKFile	=	finder.GetFilePath();
		CString	sCode	=	AfxGetFileTitleNoExt(sKFile);
		CString	sName;
		CStockInfo	info;
		if( AfxGetStockContainer().GetStockInfo( sCode, &info ) )
			sName	=	info.GetStockName();
		
		int	nRet = CStDatabase::AppendToTyData( sCode, sName, CKData::ktypeDay, sKFile,
					m_strFileDest, m_tmBegin.GetTime(), m_tmEnd.GetTime() );
		
		nCount	+=	nRet;
		if( nRet > 0 )	nFileCount	++;

		// Progress
		m_ctrlProgress.SetPos(++nProgress);
		CString	strProgressNew;
		if( nFileCountTotal > 0 )
			strProgressNew.Format( "%d%%", 100*nProgress/nFileCountTotal );
		CString	strProgressOld;
		m_staticProgress.GetWindowText( strProgressOld );
		if( 0 != strProgressNew.Compare( strProgressOld ) )
			m_staticProgress.SetWindowText( strProgressNew );
	}
	finder.Close();

	// ÉîÛÚ
	bWorking	=	finder.FindFile( AfxGetFilePath( (LPCTSTR)m_strPathSrcSZ, LPCTSTR("*.day") ) );
	while( bWorking )
	{
		bWorking	=	finder.FindNextFile();
		CString	sKFile	=	finder.GetFilePath();
		CString	sCode	=	AfxGetFileTitleNoExt(sKFile);
		CString	sName;
		CStockInfo	info;
		if( AfxGetStockContainer().GetStockInfo( sCode, &info ) )
			sName	=	info.GetStockName();
		
		int nRet = CStDatabase::AppendToTyData( sCode, sName, CKData::ktypeDay, sKFile,
					m_strFileDest, m_tmBegin.GetTime(), m_tmEnd.GetTime() );
		
		nCount	+=	nRet;
		if( nRet > 0 )	nFileCount	++;

		// Progress
		m_ctrlProgress.SetPos(++nProgress);
		CString	strProgressNew;
		if( nFileCountTotal > 0 )
			strProgressNew.Format( "%.2f%%", 100.*nProgress/nFileCountTotal );
		CString	strProgressOld;
		m_staticProgress.GetWindowText( strProgressOld );
		if( 0 != strProgressNew.Compare( strProgressOld ) )
			m_staticProgress.SetWindowText( strProgressNew );
	}
	finder.Close();

	CString	strReportFmt, strReport;
	strReportFmt.LoadString( IDS_TYDATADLG_REPORTFMT );
	strReport.Format( strReportFmt, nFileCount, nCount );
	AfxMessageBox( strReport, MB_OK|MB_ICONINFORMATION );

	m_btnStart.EnableWindow( TRUE );
}

