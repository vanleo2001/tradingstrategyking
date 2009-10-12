// ExportDest.cpp : implementation file
//

#include "stdafx.h"

#include "ExportDest.h"
#include "SelectStk.h"
#include "ExportOption.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportDest property page

void CExportDest::StoreProfile( )
{
	AfxGetApp()->WriteProfileInt( "exportdest", "txtcheck", m_bTxtCheck );
	AfxGetApp()->WriteProfileInt( "exportdest", "mdbcheck", m_bMdbCheck );
	AfxGetApp()->WriteProfileInt( "exportdest", "csvcheck", m_bCsvCheck );
	AfxGetApp()->WriteProfileInt( "exportdest", "odbccheck", m_bODBCCheck );
	AfxGetApp()->WriteProfileString( "exportdest", "txtdest", m_strTxtDest );
	AfxGetApp()->WriteProfileString( "exportdest", "mdbdest", m_strMdbDest );
	AfxGetApp()->WriteProfileString( "exportdest", "csvdest", m_strCsvDest );
	AfxGetApp()->WriteProfileString( "exportdest", "odbcdest", m_strODBCDest );
	AfxGetApp()->WriteProfileInt( "exportdest", "exeonfinish", m_bExeOnFinish );
	AfxGetApp()->WriteProfileInt( "exportdest", "overwrite", m_bOverwrite );
}

void CExportDest::LoadProfile( )
{
	m_bTxtCheck		=	TRUE;
	m_bOverwrite	=	TRUE;
	m_bExeOnFinish	=	TRUE;
	m_strTxtDest	=	AfxGetProfile().GetExportPath() + "txt";
	m_strMdbDest	=	AfxGetProfile().GetExportPath() + "KData.mdb";
	m_strCsvDest	=	AfxGetProfile().GetExportPath() + "csv";
	m_strODBCDest	=	"ODBC;DSN=Excel Files;DBQ="
						+ AfxGetProfile().GetExportPath()
						+ "KData.xls;DefaultDir="
						+ AfxGetProfile().GetExportPath()
						+ ";DriverId=790;MaxBufferSize=2048;PageTimeout=5;";


	if( (0 != access( m_strTxtDest, 0 ) || FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( m_strTxtDest ) )
		&& !CreateDirectory( m_strTxtDest, NULL ) )
		m_strTxtDest	=	AfxGetProfile().GetExportPath();
	if( (0 != access( m_strCsvDest, 0 ) || FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( m_strCsvDest ) )
		&& !CreateDirectory( m_strCsvDest, NULL ) )
		m_strCsvDest	=	AfxGetProfile().GetExportPath();

	m_bTxtCheck		=	AfxGetApp()->GetProfileInt( "exportdest", "txtcheck", m_bTxtCheck );
	m_bMdbCheck		=	AfxGetApp()->GetProfileInt( "exportdest", "mdbcheck", m_bMdbCheck );
	m_bCsvCheck		=	AfxGetApp()->GetProfileInt( "exportdest", "csvcheck", m_bCsvCheck );
	m_bODBCCheck	=	AfxGetApp()->GetProfileInt( "exportdest", "odbccheck", m_bODBCCheck );
	m_strTxtDest	=	AfxGetApp()->GetProfileString( "exportdest", "txtdest", m_strTxtDest );
	m_strMdbDest	=	AfxGetApp()->GetProfileString( "exportdest", "mdbdest", m_strMdbDest );
	m_strCsvDest	=	AfxGetApp()->GetProfileString( "exportdest", "csvdest", m_strCsvDest );
	m_strODBCDest	=	AfxGetApp()->GetProfileString( "exportdest", "odbcdest", m_strODBCDest );
	m_bExeOnFinish	=	AfxGetApp()->GetProfileInt( "exportdest", "exeonfinish", m_bExeOnFinish );
	m_bOverwrite	=	AfxGetApp()->GetProfileInt( "exportdest", "overwrite", m_bOverwrite );
}

IMPLEMENT_DYNCREATE(CExportDest, CPropertyPageEx)

CExportDest::CExportDest() : CPropertyPageEx(CExportDest::IDD)
{
	//{{AFX_DATA_INIT(CExportDest)
	m_strTxtDest = _T("");
	m_strMdbDest = _T("");
	m_strCsvDest = _T("");
	m_bTxtCheck = FALSE;
	m_bMdbCheck = FALSE;
	m_bCsvCheck = FALSE;
	m_bExeOnFinish = FALSE;
	m_bODBCCheck = FALSE;
	m_strODBCDest = _T("");
	//}}AFX_DATA_INIT
	m_bExporting	=	FALSE;
	m_bCanceled		=	FALSE;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
}

CExportDest::CExportDest(UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle)
			 : CPropertyPageEx(CExportDest::IDD, nIDCaption, nIDTitle, nIDSubTitle)
{
	m_strTxtDest = _T("");
	m_strMdbDest = _T("");
	m_strCsvDest = _T("");
	m_bTxtCheck = FALSE;
	m_bMdbCheck = FALSE;
	m_bCsvCheck = FALSE;
	m_bExeOnFinish = FALSE;
	m_bODBCCheck = FALSE;
	m_strODBCDest = _T("");

	m_bExporting	=	FALSE;
	m_bCanceled		=	FALSE;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
}

CExportDest::~CExportDest()
{
}

BOOL CExportDest::ExportBegin(CStringArray &astrColumnName, CUIntArray &anWidth,
							BOOL	bReportError )
{
	CString	strTxt, strMdb, strCsv, strODBC;
	if( m_bTxtCheck )	strTxt	=	AfxGetFileDirectoryExist( m_strTxtDest, (LPCTSTR)AfxGetProfile().GetExportPath() );
	if( m_bMdbCheck )	strMdb	=	m_strMdbDest;
	if( m_bCsvCheck )	strCsv	=	AfxGetFileDirectoryExist( m_strCsvDest, (LPCTSTR)AfxGetProfile().GetExportPath() );
	if( m_bODBCCheck )	strODBC	=	m_strODBCDest;
	if( !m_exportdb.OpenAllDB(strTxt, TRUE, strMdb, strCsv, TRUE, strODBC,
							bReportError ) )
		return FALSE;

	if( ! m_exportdb.SetTableColumn( astrColumnName, anWidth ) )
		return FALSE;

	return TRUE;
}

BOOL CExportDest::ExportOpenTable( CStock &stock, int ktype, BOOL bReportError )
{
	CString	strTableName	=	stock.GetStockCode();
	strTableName	+=	AfxGetKTypeString( ktype );
	if( !m_exportdb.OpenAllTable( strTableName, m_bOverwrite, bReportError ) )
		return FALSE;
	return TRUE;
}

BOOL CExportDest::ExportAddItem( CStringArray &astrItemText )
{
	// Pump Message
	MSG		msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		AfxGetApp()->PumpMessage();

	return m_exportdb.AddItem( astrItemText );
}

void CExportDest::ExportCloseTable( )
{
	m_exportdb.CloseAllTable( );
}

BOOL CExportDest::ExportFinish( )
{
	m_exportdb.CloseAllDB( );
	
	if( m_bExeOnFinish )
	{
		if( m_bTxtCheck && m_strTxtDest.GetLength() > 0 )
			ShellExecute( NULL, "open", m_strTxtDest, NULL, NULL, SW_SHOW );
		if( m_bMdbCheck && m_strMdbDest.GetLength() > 0 )
			ShellExecute( NULL, "open", m_strMdbDest, NULL, NULL, SW_SHOW );
		if( m_bCsvCheck && m_strCsvDest.GetLength() > 0 )
			ShellExecute( NULL, "open", m_strCsvDest, NULL, NULL, SW_SHOW );
		if( m_bODBCCheck && m_strODBCDest.GetLength() > 0 )
			CExportDB::ExecuteODBCFile( m_strODBCDest );
	}

	// Set Progress
	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
	m_staticProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );

	int	nLower = 0, nUpper = 100;
	m_ctrlProgress.GetRange( nLower, nUpper );

	if( nUpper > nLower )
	{
		m_ctrlProgress.SetPos( nUpper );
		CString	strInfo;
		strInfo.LoadString( IDS_EXPORTDB_FINISHED );
		m_staticProgress.SetWindowText( strInfo );
	}

	return TRUE;
}

void CExportDest::DoExport( )
{
	// if( !AfxAssertFunclimitProf( ) )
	//	return;

	// EnableWindow FALSE
	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		CWnd * pWnd = pSheet->GetDlgItem( IDCANCEL );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
			pWnd->EnableWindow( FALSE );
		pWnd = pSheet->GetDlgItem( ID_WIZBACK );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
			pWnd->EnableWindow( FALSE );
		pWnd = pSheet->GetDlgItem( ID_WIZNEXT );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
			pWnd->EnableWindow( FALSE );
	}

	// Get Pages
	CSelectStk * pStk = (CSelectStk *)pSheet->GetPage(0);
	CExportOption * pOption = (CExportOption *)pSheet->GetPage(1);

	// Store CExportOption tmEnd
	CSPTime	timeEnd;
	if( !AfxGetDB().GetTimeLocalRange( &timeEnd, NULL, NULL ) )
		timeEnd	=	CSPTime::GetCurrentTime();
	if( pOption->m_bTimeDefine && timeEnd.GetTime() > pOption->m_tmEnd.GetTime() )
		timeEnd		=	pOption->m_tmEnd.GetTime();
	pOption->m_tmEnd	=	timeEnd.GetTime();
	pOption->StoreProfile( );

	// Columns
	CStringArray	astrColumn;
	CUIntArray		anWidth;
	CString	strTemp;
	strTemp.LoadString( IDS_EXPORTDEST_DATE );		astrColumn.Add( strTemp );		anWidth.Add( SLH_WIDTH_MIN );
	strTemp.LoadString( IDS_EXPORTDEST_OPEN );		astrColumn.Add( strTemp );		anWidth.Add( SLH_WIDTH_MIN );
	strTemp.LoadString( IDS_EXPORTDEST_HIGH );		astrColumn.Add( strTemp );		anWidth.Add( SLH_WIDTH_MIN );
	strTemp.LoadString( IDS_EXPORTDEST_LOW );		astrColumn.Add( strTemp );		anWidth.Add( SLH_WIDTH_MIN );
	strTemp.LoadString( IDS_EXPORTDEST_CLOSE );		astrColumn.Add( strTemp );		anWidth.Add( SLH_WIDTH_MIN );
	strTemp.LoadString( IDS_EXPORTDEST_AMOUNT );	astrColumn.Add( strTemp );		anWidth.Add( SLH_WIDTH_MIN );
	strTemp.LoadString( IDS_EXPORTDEST_VOLUME );	astrColumn.Add( strTemp );		anWidth.Add( SLH_WIDTH_MIN );

	m_bExporting	=	TRUE;
	m_bCanceled		=	FALSE;
	if( ExportBegin( astrColumn, anWidth, TRUE ) )
	{
		CString	strText;
		int	nCount = 0, nTotalCount = pStk->m_domainTemp.GetSize();
		SetProgressRange( 0, nTotalCount*10 );

		for( int i=0; i<pStk->m_domainTemp.GetSize(); i++ )
		{
			CStock	stock;
			stock.SetStockCode( CStock::marketUnknown, pStk->m_domainTemp.ElementAt(i) );
			stock.SetDatabase( &AfxGetDB() );

			DoExportStock( stock, nCount*10, nCount*10+10, pOption );
			nCount	++;
			SetProgress( nCount*10 );

			// Canceled 
			if( m_bCanceled )
				break;
		}
	}

	// Finish Export
	ExportFinish( );

	if( m_bCanceled )
	{
		ExportFinish( );
		CPropertyPageEx::OnCancel( );
		return;
	}

	m_bExporting	=	FALSE;
	m_bCanceled		=	FALSE;

	// EnableWindow TRUE
	if( pSheet )
	{
		CWnd * pWnd = pSheet->GetDlgItem( IDCANCEL );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
			pWnd->EnableWindow( TRUE );
		pWnd = pSheet->GetDlgItem( ID_WIZBACK );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
			pWnd->EnableWindow( TRUE );
		pWnd = pSheet->GetDlgItem( ID_WIZNEXT );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
			pWnd->EnableWindow( TRUE );
	}
}

void CExportDest::DoExportStock( CStock &stock, int nProgressStart, int nProgressEnd, CExportOption * pOption )
{
	// Prepare Data
	if( pOption->m_bKMin5 )
		stock.PrepareData( CStock::dataK, CKData::ktypeMin5 );
	if( pOption->m_bKMin15 )
		stock.PrepareData( CStock::dataK, CKData::ktypeMin15 );
	if( pOption->m_bKMin30 )
		stock.PrepareData( CStock::dataK, CKData::ktypeMin30 );
	if( pOption->m_bKMin60 )
		stock.PrepareData( CStock::dataK, CKData::ktypeMin60 );

	if( pOption->m_bNoResumeDR )
	{
		if( pOption->m_bKDay )
			stock.PrepareData( CStock::dataK, CKData::ktypeDay );
		if( pOption->m_bKMonth )
			stock.PrepareData( CStock::dataK, CKData::ktypeMonth );
		if( pOption->m_bKWeek )
			stock.PrepareData( CStock::dataK, CKData::ktypeWeek );
	}
	else
	{
		// DR Up or down, auto begin
		int	nFormat	=	CKData::formatXDRdown;
		if( pOption->m_bResumeDRUp )
			nFormat	=	CKData::formatXDRup;
		DWORD	dateAutoDRBegin	=	-1;
		if( pOption->m_bAutoResumeDR )
		{
			CSPTime	sptime( pOption->m_tmAutoResumeDRBegin.GetTime() );
			dateAutoDRBegin	=	sptime.ToStockTimeDay( );
		}

		stock.PrepareData( CStock::dataDR );
		CDRData	& drdata	=	stock.GetDRData();

		if( pOption->m_bKDay || pOption->m_bKMonth || pOption->m_bKWeek )
		{
			stock.PrepareData( CStock::dataK, CKData::ktypeDay );
			stock.GetKDataDay().SetDRData( drdata );
			stock.GetKDataDay().ChangeCurFormat( nFormat, dateAutoDRBegin, pOption->m_nAutoResumeDRLimit );

			if( pOption->m_bKMonth )
				stock.ExtractKData( CKData::ktypeMonth, TRUE );
			if( pOption->m_bKWeek )
				stock.ExtractKData( CKData::ktypeWeek, TRUE );
		}

		stock.GetKDataMin5().SetDRData( drdata );
		stock.GetKDataMin5().ChangeCurFormat( nFormat, dateAutoDRBegin, pOption->m_nAutoResumeDRLimit );
		stock.GetKDataMin15().SetDRData( drdata );
		stock.GetKDataMin15().ChangeCurFormat( nFormat, dateAutoDRBegin, pOption->m_nAutoResumeDRLimit );
		stock.GetKDataMin30().SetDRData( drdata );
		stock.GetKDataMin30().ChangeCurFormat( nFormat, dateAutoDRBegin, pOption->m_nAutoResumeDRLimit );
		stock.GetKDataMin60().SetDRData( drdata );
		stock.GetKDataMin60().ChangeCurFormat( nFormat, dateAutoDRBegin, pOption->m_nAutoResumeDRLimit );
	}

	// Calcuate Progress
	int	nTotal	=	0, nCount	=	0;
	if( pOption->m_bKMonth )	nTotal	+=	stock.GetKDataMonth().GetSize();
	if( pOption->m_bKWeek )		nTotal	+=	stock.GetKDataWeek().GetSize();
	if( pOption->m_bKDay )		nTotal	+=	stock.GetKDataDay().GetSize();
	if( pOption->m_bKMin5 )		nTotal	+=	stock.GetKDataMin5().GetSize();
	if( pOption->m_bKMin15 )	nTotal	+=	stock.GetKDataMin15().GetSize();
	if( pOption->m_bKMin30 )	nTotal	+=	stock.GetKDataMin30().GetSize();
	if( pOption->m_bKMin60 )	nTotal	+=	stock.GetKDataMin60().GetSize();

	if( 0 == nTotal )
		return;

	// time
	DWORD	dateDayBegin = 0, dateDayEnd = (DWORD(-1));
	DWORD	dateMinBegin = 0, dateMinEnd = (DWORD(-1));
	if( pOption->m_bTimeDefine )
	{
		dateDayBegin=	CSPTime(pOption->m_tmBegin.GetTime()).ToStockTimeDay();
		dateDayEnd	=	CSPTime(pOption->m_tmEnd.GetTime()).ToStockTimeDay();
		dateMinBegin=	CSPTime(pOption->m_tmBegin.GetTime()).ToStockTimeMin();
		dateMinEnd	=	CSPTime(pOption->m_tmEnd.GetTime()).ToStockTimeMin();
	}

	// Begin Export Month
	if( pOption->m_bKMonth
		&& ExportOpenTable( stock, CKData::ktypeMonth, TRUE ) )
	{
		CKData	& kdata = stock.GetKDataMonth( );
		for( int k=0; k<kdata.GetSize(); k++ )
		{
			KDATA	kd	=	kdata.ElementAt(k);
			CStringArray	astrItemText;
			if( DoExportKDATAToStringArray( kd, astrItemText, TRUE, dateDayBegin, dateDayEnd, stock.GetStockInfo().DigitBit() ) )
				ExportAddItem( astrItemText );
			nCount	++;
			SetProgress( nProgressStart + (nProgressEnd-nProgressStart)*nCount/nTotal );
			if( m_bCanceled )	return;
		}
		ExportCloseTable( );
	}

	// Begin Export Week
	if( pOption->m_bKWeek
		&& ExportOpenTable( stock, CKData::ktypeWeek, TRUE ) )
	{
		CKData	& kdata = stock.GetKDataWeek( );
		for( int k=0; k<kdata.GetSize(); k++ )
		{
			KDATA	kd	=	kdata.ElementAt(k);
			CStringArray	astrItemText;
			if( DoExportKDATAToStringArray( kd, astrItemText, TRUE, dateDayBegin, dateDayEnd, stock.GetStockInfo().DigitBit() ) )
				ExportAddItem( astrItemText );
			nCount	++;
			SetProgress( nProgressStart + (nProgressEnd-nProgressStart)*nCount/nTotal );
			if( m_bCanceled )	return;
		}
		ExportCloseTable( );
	}

	// Begin Export Day
	if( pOption->m_bKDay
		&& ExportOpenTable( stock, CKData::ktypeDay, TRUE ) )
	{
		CKData	& kdata = stock.GetKDataDay( );
		for( int k=0; k<kdata.GetSize(); k++ )
		{
			KDATA	kd	=	kdata.ElementAt(k);
			CStringArray	astrItemText;
			if( DoExportKDATAToStringArray( kd, astrItemText, TRUE, dateDayBegin, dateDayEnd, stock.GetStockInfo().DigitBit() ) )
				ExportAddItem( astrItemText );
			nCount	++;
			SetProgress( nProgressStart + (nProgressEnd-nProgressStart)*nCount/nTotal );
			if( m_bCanceled )	return;
		}
		ExportCloseTable( );
	}

	// Begin Export Min5
	if( pOption->m_bKMin5
		&& ExportOpenTable( stock, CKData::ktypeMin5, TRUE ) )
	{
		CKData	& kdata = stock.GetKDataMin5( );
		for( int k=0; k<kdata.GetSize(); k++ )
		{
			KDATA	kd	=	kdata.ElementAt(k);
			CStringArray	astrItemText;
			if( DoExportKDATAToStringArray( kd, astrItemText, FALSE, dateMinBegin, dateMinEnd, stock.GetStockInfo().DigitBit() ) )
				ExportAddItem( astrItemText );
			nCount	++;
			SetProgress( nProgressStart + (nProgressEnd-nProgressStart)*nCount/nTotal );
			if( m_bCanceled )	return;
		}
		ExportCloseTable( );
	}

	// Begin Export Min15
	if( pOption->m_bKMin15
		&& ExportOpenTable( stock, CKData::ktypeMin15, TRUE ) )
	{
		CKData	& kdata = stock.GetKDataMin15( );
		for( int k=0; k<kdata.GetSize(); k++ )
		{
			KDATA	kd	=	kdata.ElementAt(k);
			CStringArray	astrItemText;
			if( DoExportKDATAToStringArray( kd, astrItemText, FALSE, dateMinBegin, dateMinEnd, stock.GetStockInfo().DigitBit() ) )
				ExportAddItem( astrItemText );
			nCount	++;
			SetProgress( nProgressStart + (nProgressEnd-nProgressStart)*nCount/nTotal );
			if( m_bCanceled )	return;
		}
		ExportCloseTable( );
	}

	// Begin Export Min30
	if( pOption->m_bKMin30
		&& ExportOpenTable( stock, CKData::ktypeMin30, TRUE ) )
	{
		CKData	& kdata = stock.GetKDataMin30( );
		for( int k=0; k<kdata.GetSize(); k++ )
		{
			KDATA	kd	=	kdata.ElementAt(k);
			CStringArray	astrItemText;
			if( DoExportKDATAToStringArray( kd, astrItemText, FALSE, dateMinBegin, dateMinEnd, stock.GetStockInfo().DigitBit() ) )
				ExportAddItem( astrItemText );
			nCount	++;
			SetProgress( nProgressStart + (nProgressEnd-nProgressStart)*nCount/nTotal );
			if( m_bCanceled )	return;
		}
		ExportCloseTable( );
	}

	// Begin Export Min60
	if( pOption->m_bKMin60
		&& ExportOpenTable( stock, CKData::ktypeMin60, TRUE ) )
	{
		CKData	& kdata = stock.GetKDataMin60( );
		for( int k=0; k<kdata.GetSize(); k++ )
		{
			KDATA	kd	=	kdata.ElementAt(k);
			CStringArray	astrItemText;
			if( DoExportKDATAToStringArray( kd, astrItemText, FALSE, dateMinBegin, dateMinEnd, stock.GetStockInfo().DigitBit() ) )
				ExportAddItem( astrItemText );
			nCount	++;
			SetProgress( nProgressStart + (nProgressEnd-nProgressStart)*nCount/nTotal );
			if( m_bCanceled )	return;
		}
		ExportCloseTable( );
	}
}

BOOL CExportDest::DoExportKDATAToStringArray( KDATA & kd, CStringArray & astrItemText,
											BOOL bDayOrMin, DWORD dateBegin, DWORD dateEnd, int nDigitBits )
{
	if( bDayOrMin )
	{
		if( kd.m_date < dateBegin || kd.m_date > dateEnd )
			return FALSE;
	}
	else
	{
		if( kd.m_date % 100000000 < dateBegin % 100000000
			|| kd.m_date % 100000000 > dateEnd % 100000000 )
			return FALSE;
	}

	CString	strTemp;
	
	// Date
	CSPTime	sptime;
	sptime.FromStockTime( kd.m_date, bDayOrMin );
	strTemp	=	sptime.Format( "%Y-%m-%d" );
	astrItemText.Add( strTemp );

	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", nDigitBits );

	// Open
	strTemp.Format( strPriceFmt, kd.m_fOpen );
	astrItemText.Add( strTemp );

	// High
	strTemp.Format( strPriceFmt, kd.m_fHigh );
	astrItemText.Add( strTemp );

	// Low
	strTemp.Format( strPriceFmt, kd.m_fLow );
	astrItemText.Add( strTemp );

	// Close
	strTemp.Format( strPriceFmt, kd.m_fClose );
	astrItemText.Add( strTemp );

	// Amount
	strTemp.Format( "%u", DWORD(kd.m_fAmount/1000) );
	astrItemText.Add( strTemp );

	// Volume
	strTemp.Format( "%u", DWORD(kd.m_fVolume/100) );
	astrItemText.Add( strTemp );

	return TRUE;
}

void CExportDest::SetProgressRange( int nLower, int nUpper )
{
	if( nUpper > nLower )
	{
		m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
		m_staticProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
		m_ctrlProgress.SetRange32( nLower, nUpper );
	}
}

void CExportDest::SetProgress( int nPos )
{
	int	nLower = 0, nUpper = 100;
	m_ctrlProgress.GetRange( nLower, nUpper );

	if( nUpper > nLower )
	{
		m_ctrlProgress.SetPos( nPos );

		CString	strProgressNew;
		strProgressNew.Format( "%d%%", 100*nPos/(nUpper-nLower) );
		CString	strProgressOld;
		m_staticProgress.GetWindowText( strProgressOld );
		if( 0 != strProgressNew.Compare( strProgressOld ) )
			m_staticProgress.SetWindowText( strProgressNew );
	}
}

void CExportDest::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportDest)
	DDX_Control(pDX, IDC_ODBC_SOURCE, m_btnODBCSource);
	DDX_Control(pDX, IDC_ODBC_DEST, m_editODBCDest);
	DDX_Control(pDX, IDC_ODBC_CHECK, m_btnODBCCheck);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_staticProgress);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_CSV_EXPLORER, m_btnCsvExplorer);
	DDX_Control(pDX, IDC_CSV_DEST, m_editCsvDest);
	DDX_Control(pDX, IDC_CSV_CHECK, m_btnCsvCheck);
	DDX_Control(pDX, IDC_MDB_EXPLORER, m_btnMdbExplorer);
	DDX_Control(pDX, IDC_MDB_DEST, m_editMdbDest);
	DDX_Control(pDX, IDC_MDB_CHECK, m_btnMdbCheck);
	DDX_Control(pDX, IDC_TXT_EXPLORER, m_btnTxtExplorer);
	DDX_Control(pDX, IDC_TXT_DEST, m_editTxtDest);
	DDX_Control(pDX, IDC_TXT_CHECK, m_btnTxtCheck);
	DDX_Text(pDX, IDC_TXT_DEST, m_strTxtDest);
	DDX_Text(pDX, IDC_MDB_DEST, m_strMdbDest);
	DDX_Text(pDX, IDC_CSV_DEST, m_strCsvDest);
	DDX_Check(pDX, IDC_TXT_CHECK, m_bTxtCheck);
	DDX_Check(pDX, IDC_MDB_CHECK, m_bMdbCheck);
	DDX_Check(pDX, IDC_CSV_CHECK, m_bCsvCheck);
	DDX_Check(pDX, IDC_EXEONFINISH, m_bExeOnFinish);
	DDX_Check(pDX, IDC_ODBC_CHECK, m_bODBCCheck);
	DDX_Text(pDX, IDC_ODBC_DEST, m_strODBCDest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportDest, CPropertyPageEx)
	//{{AFX_MSG_MAP(CExportDest)
	ON_BN_CLICKED(IDC_TXT_CHECK, OnTxtCheck)
	ON_BN_CLICKED(IDC_TXT_EXPLORER, OnTxtExplorer)
	ON_BN_CLICKED(IDC_MDB_CHECK, OnMdbCheck)
	ON_BN_CLICKED(IDC_MDB_EXPLORER, OnMdbExplorer)
	ON_BN_CLICKED(IDC_CSV_CHECK, OnCsvCheck)
	ON_BN_CLICKED(IDC_CSV_EXPLORER, OnCsvExplorer)
	ON_BN_CLICKED(IDC_ODBC_CHECK, OnODBCCheck)
	ON_BN_CLICKED(IDC_ODBC_SOURCE, OnOdbcSource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDest message handlers

BOOL CExportDest::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );

	// txt 
	m_btnTxtCheck.SetCheck( m_bTxtCheck );
	m_editTxtDest.EnableWindow( m_bTxtCheck );
	m_btnTxtExplorer.EnableWindow( m_bTxtCheck );
	int	nCheckID	=	IDC_EXPORT_OVERWRITE;
	if( !m_bOverwrite )
		nCheckID	=	IDC_EXPORT_APPEND;
	CheckRadioButton( IDC_EXPORT_OVERWRITE, IDC_EXPORT_APPEND, nCheckID );

	// mdb
	m_btnMdbCheck.SetCheck( m_bMdbCheck );
	m_editMdbDest.EnableWindow( m_bMdbCheck );
	m_btnMdbExplorer.EnableWindow( m_bMdbCheck );

	// Csv
	m_btnCsvCheck.SetCheck( m_bCsvCheck );
	m_editCsvDest.EnableWindow( m_bCsvCheck );
	m_btnCsvExplorer.EnableWindow( m_bCsvCheck );

	// ODBC
	m_btnODBCCheck.SetCheck( m_bODBCCheck );
	m_editODBCDest.EnableWindow( m_bODBCCheck );
	m_btnODBCSource.EnableWindow( m_bODBCCheck );

	// progress ctrl
	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE );
	m_staticProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExportDest::OnTxtCheck() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnTxtCheck.GetCheck();
	m_editTxtDest.EnableWindow( nCheck );
	m_btnTxtExplorer.EnableWindow( nCheck );
}

void CExportDest::OnTxtExplorer() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CString	strTemp	=	AfxGetFileDirectoryExist( m_strTxtDest, (LPCTSTR)AfxGetProfile().GetExportPath() );

	if( AfxDoSelectDirectory( strTemp ) )
	{
		m_strTxtDest	=	strTemp;
		UpdateData( FALSE );
	}
}

void CExportDest::OnMdbCheck() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnMdbCheck.GetCheck();
	m_editMdbDest.EnableWindow( nCheck );
	m_btnMdbExplorer.EnableWindow( nCheck );
}

void CExportDest::OnMdbExplorer() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_ENABLESIZING,
		"Microsoft Access Database files (*.mdb)|*.mdb|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strMdbDest	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CExportDest::OnCsvCheck() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnCsvCheck.GetCheck();
	m_editCsvDest.EnableWindow( nCheck );
	m_btnCsvExplorer.EnableWindow( nCheck );
}

void CExportDest::OnODBCCheck() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnODBCCheck.GetCheck();
	m_editODBCDest.EnableWindow( nCheck );
	m_btnODBCSource.EnableWindow( nCheck );
}

void CExportDest::OnCsvExplorer() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CString	strTemp	=	AfxGetFileDirectoryExist( m_strCsvDest, (LPCTSTR)AfxGetProfile().GetExportPath() );

	if( AfxDoSelectDirectory( strTemp ) )
	{
		m_strCsvDest	=	strTemp;
		UpdateData( FALSE );
	}
}

void CExportDest::OnOdbcSource() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_exportdb.OpenODBCDB( NULL ) )
	{
		m_strODBCDest	=	m_exportdb.GetODBCConnect();
		UpdateData( FALSE );
	}
}

BOOL CExportDest::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_bExporting	=	FALSE;
	m_bCanceled		=	FALSE;

	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		pSheet->SetWizardButtons( m_dwButtonFlags );
		
		CWnd * pWnd = pSheet->GetDlgItem( ID_WIZNEXT );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		{
			CString	strTemp;
			strTemp.LoadString( IDS_EXPORT_START );
			pWnd->SetWindowText( strTemp );
		}
	}

	// progress ctrl
	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE );
	m_staticProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE );

	return CPropertyPageEx::OnSetActive();
}

LRESULT CExportDest::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPageEx::OnWizardBack();
}

LRESULT CExportDest::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );

	CButton * pbtn		= (CButton *)GetDlgItem( IDC_EXPORT_OVERWRITE );
	if( pbtn )	m_bOverwrite	=	pbtn->GetCheck();

	if( !m_bTxtCheck && !m_bMdbCheck && !m_bCsvCheck && !m_bODBCCheck )
	{
		AfxMessageBox( IDS_EXPORTDB_NODEST, MB_OK | MB_ICONINFORMATION );
		return -1;
	}

	if( m_bTxtCheck && m_strTxtDest.GetLength() == 0 )
	{
		AfxMessageBox( IDS_EXPORTDB_NOTXTDEST, MB_OK | MB_ICONINFORMATION );
		return -1;
	}
	if( m_bMdbCheck && m_strMdbDest.GetLength() == 0 )
	{
		AfxMessageBox( IDS_EXPORTDB_NOMDBDEST, MB_OK | MB_ICONINFORMATION );
		return -1;
	}
	if( m_bCsvCheck && m_strCsvDest.GetLength() == 0 )
	{
		AfxMessageBox( IDS_EXPORTDB_NOCSVDEST, MB_OK | MB_ICONINFORMATION );
		return -1;
	}
	if( m_bODBCCheck && m_strODBCDest.GetLength() == 0 )
	{
		AfxMessageBox( IDS_EXPORTDB_NOODBCDEST, MB_OK | MB_ICONINFORMATION );
		return -1;
	}

	if( m_bTxtCheck )
	{
		CPathDialog::MakeSurePathExists( m_strTxtDest );
		m_strTxtDest	=	AfxGetFileDirectoryExist( m_strTxtDest, (LPCTSTR)AfxGetProfile().GetExportPath() );
	}
	if( m_bMdbCheck )
	{
		int nIndex = m_strMdbDest.Find( "." );
		if( -1 == nIndex )
			m_strMdbDest	+=	".mdb";
		else if( m_strMdbDest.GetLength()-1 == nIndex )
			m_strMdbDest	+=	"mdb";
	}
	if( m_bCsvCheck )
	{
		CPathDialog::MakeSurePathExists( m_strCsvDest );
		m_strCsvDest	=	AfxGetFileDirectoryExist( m_strCsvDest, (LPCTSTR)AfxGetProfile().GetExportPath() );
	}

	UpdateData( FALSE );

	// Store profile
	StoreProfile( );

	// Export
	DoExport( );

	return -1;
}

void CExportDest::OnCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_bExporting )
	{
		if( IDOK == AfxMessageBox( IDS_EXPORTDEST_CONFIRMCLOSE, MB_OKCANCEL|MB_ICONINFORMATION ) )
			m_bCanceled	=	TRUE;
		return;
	}
	CPropertyPageEx::OnCancel();
}


BOOL CExportDest::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
		return TRUE;
	return CPropertyPageEx::PreTranslateMessage(pMsg);
}
