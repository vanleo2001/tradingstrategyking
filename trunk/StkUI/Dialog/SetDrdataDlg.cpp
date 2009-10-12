// SetDrdataDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SetDrdataDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDrdataDlg dialog

#define		SETDR_COLUMN_COUNT	5

static CString FormatDate( CSPTime &tm )
{
	return (LPCTSTR)tm.Format( "%Y/%m/%d" );
}

static CString FormatDate( DWORD date )
{
	CSPTime	sptime;
	if( sptime.FromStockTimeDay( date ) )
	{
		return (LPCTSTR)sptime.Format( "%Y/%m/%d" );
	}
	return "----/--/--";
}

static BOOL ParseDate( CString strDate, DWORD * pDate )
{
	CSPTime	tm;
	char	sepDate	=	'/';

	strDate.TrimLeft();
	strDate.TrimRight();
	if( strDate.IsEmpty() )
		return FALSE;

	int	nYear = 1971, nMonth = 1, nDay = 1;
	int nHour = 0, nMinute= 0, nSecond = 0;
	CString	strTemp;
	int nIndex;
	
	// year
	nIndex = strDate.Find( sepDate );
	if( -1 != nIndex )
	{
		strTemp	=	strDate.Left(nIndex);
		strDate	=	strDate.Mid(nIndex+1);
		nYear	=	atol(strTemp);
	}
	else
	{
		nYear	=	atol(strDate);
		tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		if( pDate )	*pDate	=	tm.GetYear()*10000 + tm.GetMonth() * 100 + tm.GetDay();
		return (-1 != tm.GetTime());
	}

	// month
	nIndex = strDate.Find( sepDate );
	if( -1 != nIndex )
	{
		strTemp	=	strDate.Left(nIndex);
		strDate	=	strDate.Mid(nIndex+1);
		nMonth	=	atol(strTemp);
	}
	else
	{
		nMonth	=	atol(strDate);
		tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
		if( pDate )	*pDate	=	tm.GetYear()*10000 + tm.GetMonth() * 100 + tm.GetDay();
		return (-1 != tm.GetTime());
	}

	// day
	nDay	=	atol(strDate);
	if( 0 == nYear || 0 == nMonth || 0 == nDay )
		return FALSE;
	tm		=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );
	if( -1 == tm.GetTime() )
		return FALSE;
	if( pDate )	*pDate	=	tm.GetYear()*10000 + tm.GetMonth() * 100 + tm.GetDay();
	return TRUE;
}

IMPLEMENT_DYNCREATE(CSetDrdataDlg, CPropertyPageEx)

CSetDrdataDlg::CSetDrdataDlg( )
	: CPropertyPageEx(CSetDrdataDlg::IDD)
{
	//{{AFX_DATA_INIT(CSetDrdataDlg)
	m_tmDrdata = 0;
	//}}AFX_DATA_INIT
	m_tmDrdata	=	CSPTime::GetCurrentTime();
}

BOOL CSetDrdataDlg::InitializeGrid( )
{
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(TRUE);
	m_Grid.SetHeaderSort(FALSE);
	m_Grid.SetSingleRowSelection(TRUE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.EnableTitleTips( TRUE );
	m_Grid.SetRowResize( FALSE );
	m_Grid.SetColumnResize( TRUE );

	m_Grid.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

	TRY {
		m_Grid.SetRowCount(1);
		m_Grid.SetColumnCount(SETDR_COLUMN_COUNT);
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

	CRect	rectGrid;
	m_Grid.GetClientRect( &rectGrid );
	int	nWidth	=	rectGrid.Width() / m_Grid.GetColumnCount() - 1;
	m_Grid.SetColumnWidth( 0, nWidth + 10 );
	m_Grid.SetColumnWidth( 1, nWidth - 2 );
	m_Grid.SetColumnWidth( 2, nWidth - 2 );
	m_Grid.SetColumnWidth( 3, nWidth - 2 );
	m_Grid.SetColumnWidth( 4, nWidth - 2 );

	// Set Column Header
	UINT	idsHeader[SETDR_COLUMN_COUNT] = {IDS_SETDRDATA_DATE, IDS_SETDRDATA_BONUS,
							IDS_SETDRDATA_BONUSSHARE, IDS_SETDRDATA_RATIONSHARE,
							IDS_SETDRDATA_RATIONVALUE };
	for( int nCol=0; nCol<SETDR_COLUMN_COUNT; nCol++ )
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

void CSetDrdataDlg::LoadDrdata( CString strStockCode )
{
	m_Grid.DeleteNonFixedRows();

	if( strStockCode.GetLength() <= 0 )
		return;

	CStock	stock;
	stock.SetStockCode( CStock::marketUnknown, strStockCode );
	stock.SetDatabase( &AfxGetDB() );
	stock.PrepareData( CStock::dataDR, CKData::ktypeDay );
	CDRData	 & drdata	=	stock.GetDRData();
	for( int i=0; i<drdata.GetSize(); i++ )
	{
		DRDATA	dr	=	drdata.ElementAt(i);
		CStringArray	astr;
		astr.SetSize( SETDR_COLUMN_COUNT );
		astr.ElementAt(0)	=	FormatDate( dr.m_date );
		astr.ElementAt(1).Format( "%g", dr.m_fProfit * 10 );
		astr.ElementAt(2).Format( "%g", dr.m_fGive * 10 );
		astr.ElementAt(3).Format( "%g", dr.m_fPei * 10 );
		astr.ElementAt(4).Format( "%g", dr.m_fPeiPrice );

		int	nRow	=	m_Grid.InsertRow( astr[0] );
		for( int nCol=0; nCol<SETDR_COLUMN_COUNT; nCol++ )
		{
			GV_ITEM item;
			item.mask = GVIF_TEXT|GVIF_FORMAT;
			item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			item.row = nRow;
			item.col = nCol;
			item.szText	=	astr[nCol];
			m_Grid.SetItem(&item);
		}
	}
	m_Grid.Invalidate();
}

void CSetDrdataDlg::StoreDrdata( CString strStockCode )
{
	if( strStockCode.GetLength() <= 0 )
		return;

	// Save
	CStock	stock;
	stock.SetStockCode( CStock::marketUnknown, strStockCode );
	stock.SetDatabase( &AfxGetDB() );
	CDRData	 & drdata	=	stock.GetDRData();
	drdata.RemoveAll();
	for( int k=1; k<m_Grid.GetRowCount(); k++ )
	{
		DRDATA	dr;
		memset( &dr, 0, sizeof(dr) );
		CString	string	=	m_Grid.GetItemText( k, 0 );
		if( ParseDate( string, &(dr.m_date) ) )
		{
			dr.m_dwMarket	=	CStock::marketUnknown;
			strncpy( dr.m_szCode, strStockCode, min(sizeof(dr.m_szCode)-1,strStockCode.GetLength()) );
			CSPTime	sptime;
			if( sptime.FromStockTimeDay(dr.m_date) )
				dr.m_time	=	sptime.GetTime();
			dr.m_fProfit	=	(float)( atof( m_Grid.GetItemText(k,1) ) * 0.1 );
			dr.m_fGive		=	(float)( atof( m_Grid.GetItemText(k,2) ) * 0.1 );
			dr.m_fPei		=	(float)( atof( m_Grid.GetItemText(k,3) ) * 0.1 );
			dr.m_fPeiPrice	=	(float)( atof( m_Grid.GetItemText(k,4) ) );
			drdata.InsertDRDataSort( dr );
		}
	}
	stock.StoreDRData( );
}

void CSetDrdataDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDrdataDlg)
	DDX_Control(pDX, IDC_IMPORTFXJ, m_btnImportFxj);
	DDX_Control(pDX, IDC_IMPORT, m_btnImport);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_INSERT, m_btnInsert);
	DDX_Control(pDX, IDC_STATICSTOCK, m_staticStock);
	DDX_Control(pDX, IDC_LIST_STOCKSRC, m_listStockSrc);
	DDX_Control(pDX, IDC_COMBO_GROUPSRC, m_comboGroupSrc);
	DDX_Control(pDX, IDC_EDITCODE, m_editCode);
	DDX_DateTimeCtrl(pDX, IDC_TIME_DRDATA, m_tmDrdata);
	//}}AFX_DATA_MAP

	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CSetDrdataDlg, CPropertyPageEx)
	//{{AFX_MSG_MAP(CSetDrdataDlg)
	ON_EN_CHANGE(IDC_EDITCODE, OnChangeEditcode)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUPSRC, OnSelchangeComboGroupsrc)
	ON_LBN_SELCHANGE(IDC_LIST_STOCKSRC, OnSelchangeListStocksrc)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_IMPORTFXJ, OnImportfxj)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDrdataDlg message handlers

BOOL CSetDrdataDlg::OnInitDialog()
{
	CPropertyPageEx::OnInitDialog();

	CWaitCursor	waitcursor;

	// TODO: Add extra initialization here
	CDomainContainer & groups = AfxGetGroupContainer( );
	CDomainContainer & domains = AfxGetDomainContainer( );
	CStockContainer & container = AfxGetStockContainer();

	// Src
	m_comboGroupSrc.InitStrings( TRUE, TRUE, groups );
	m_comboGroupSrc.SetCurSel( 0 );
	OnSelchangeComboGroupsrc();

	// Grid DrData
	InitializeGrid( );

	OnSelchangeListStocksrc();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetDrdataDlg::OnKillActive() 
{
	StoreDrdata( m_strCurStockCode );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );

	return CPropertyPageEx::OnKillActive();
}

void CSetDrdataDlg::OnOK() 
{
	StoreDrdata( m_strCurStockCode );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );

	CPropertyPageEx::OnOK();
}

void CSetDrdataDlg::OnCancel() 
{
	StoreDrdata( m_strCurStockCode );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );

	CPropertyPageEx::OnOK();
}

void CSetDrdataDlg::OnChangeEditcode() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString	strText;
	m_editCode.GetWindowText( strText );
	strText.TrimLeft();
	strText.TrimLeft();
	if( strText.GetLength() > 0 )
	{
		int	nSel	=	m_comboGroupSrc.GetCurSel();
		if( CB_ERR == nSel ||
			CStockContainer::typeAll != (int)(m_comboGroupSrc.GetItemData( nSel )) )
		{
			m_comboGroupSrc.SelectGroupAll( );
			OnSelchangeComboGroupsrc();
		}
	}
	if( !m_listStockSrc.SelectCode( 0, strText ) )
		m_listStockSrc.SelectShortname( 0, strText );
	OnSelchangeListStocksrc( );
}

void CSetDrdataDlg::OnSelchangeComboGroupsrc() 
{
	// TODO: Add your control notification handler code here
	int nType = m_comboGroupSrc.GetCurSelType();
	CString strName = m_comboGroupSrc.GetCurSelName();
	m_listStockSrc.SetCurrentStocks( nType, strName, AfxGetGroupContainer() );
}

BOOL CSetDrdataDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message )
	{
		if( VK_UP == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectUp( );
				OnSelchangeListStocksrc( );
				return TRUE;
			}
		}
		else if( VK_DOWN == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectDown( );
				OnSelchangeListStocksrc( );
				return TRUE;
			}
		}
		else if( VK_PRIOR == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectPrior( );
				OnSelchangeListStocksrc( );
				return TRUE;
			}
		}
		else if( VK_NEXT == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectNext( );
				OnSelchangeListStocksrc( );
				return TRUE;
			}
		}
		else if( VK_RETURN == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_editCode.SetWindowText( NULL );
				return TRUE;
			}
		}
	}
	
	return CPropertyPageEx::PreTranslateMessage(pMsg);
}

void CSetDrdataDlg::OnSelchangeListStocksrc() 
{
	// TODO: Add your control notification handler code here
	HWND hWndFocus = ::GetFocus( );
	int	nSel	=	m_listStockSrc.GetCurSel( );

	StoreDrdata( m_strCurStockCode );

	if( LB_ERR != nSel )
	{
		CString	rString, strStockCode;
		m_listStockSrc.GetText(nSel,rString);
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );

		m_staticStock.SetWindowText( rString );

		m_strCurStockCode	=	strStockCode;
		LoadDrdata( m_strCurStockCode );
	}
	else
	{
		m_Grid.DeleteNonFixedRows();
		m_Grid.Invalidate( );

		CString	strNoStock;
		strNoStock.LoadString( IDS_SETDRDATA_NOSTOCK );
		m_staticStock.SetWindowText( strNoStock );
		m_strCurStockCode.Empty();
	}

	m_btnInsert.EnableWindow( m_strCurStockCode.GetLength() > 0 );
	m_btnDelete.EnableWindow( m_strCurStockCode.GetLength() > 0 );
	::SetFocus( hWndFocus );
}

void CSetDrdataDlg::OnInsert() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CString	strNewDate	=	FormatDate( m_tmDrdata );
	if( m_strCurStockCode.GetLength() > 0 )
	{
		CStringArray	astr;
		astr.SetSize( SETDR_COLUMN_COUNT );
		astr.ElementAt(0)	=	strNewDate;
		astr.ElementAt(1).Format( "%.3f", 0. );
		astr.ElementAt(2).Format( "%.2f", 0. );
		astr.ElementAt(3).Format( "%.2f", 0. );
		astr.ElementAt(4).Format( "%.3f", 0. );
		
		int nRow = m_Grid.InsertRow( strNewDate );
		for( int nCol=1; nCol<SETDR_COLUMN_COUNT; nCol++ )
		{
			GV_ITEM item;
			item.mask = GVIF_TEXT|GVIF_FORMAT;
			item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			item.row = nRow;
			item.col = nCol;
			item.szText	=	astr[nCol];
			m_Grid.SetItem(&item);
		}
	}
	m_Grid.Invalidate( );
}

void CSetDrdataDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here

	for( int nRow=m_Grid.GetRowCount()-1; nRow >=1; nRow -- )
	{
		for( int nCol=0; nCol<m_Grid.GetColumnCount(); nCol ++ )
		{
			if( m_Grid.GetItemState(nRow,nCol) & GVIS_SELECTED )
			{
				m_Grid.DeleteRow( nRow );
				int	nRowNext	=	-1;
				if( nRow < m_Grid.GetRowCount() )
					nRowNext	=	nRow;
				else if( nRow-1 >= 1 && nRow-1 < m_Grid.GetRowCount() )
					nRowNext	=	nRow -1;
				if( nRowNext != -1 )
				{
					for( int nColTemp=0; nColTemp<m_Grid.GetColumnCount(); nColTemp ++ )
						m_Grid.SetItemState(nRowNext,nColTemp,m_Grid.GetItemState(nRowNext,nColTemp) | GVIS_SELECTED);
				}
				m_Grid.Invalidate( );
				return;
			}
		}
	}
}

BOOL CALLBACK ImportDRCallback(DWORD dwCode, DWORD dwProgress,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND	hMainWnd	=	(HWND)cookie;

	if( PROG_PROGRESS == dwCode && ::IsWindow(hMainWnd) )
	{
		::SendMessage( hMainWnd, WM_USER_WAITPROGRESS, dwProgress, (LPARAM)lpszMsg );
	}

	return TRUE;
}

void CSetDrdataDlg::OnImport() 
{
	CNetDatabase	netdb;
	if( !netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
	{
		AfxMessageBox( IDS_ROOTPATH_ERROR, MB_OK | MB_ICONINFORMATION );
		return;
	}

	CKSFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING,
		"XDR Data files (*.zip,*.dat)|*.zip;*.dat|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		CString	sFileName	=	dlg.GetPathName();
		if( sFileName.GetLength() <= 0 || 0 != access(sFileName,0) )
		{
			AfxMessageBox( IDS_SETDRDATA_FILENOTEXISTS, MB_OK|MB_ICONINFORMATION );
			return;
		}
		
		CWaitDlg	waitdlg( NULL, FALSE );
		waitdlg.SetProgressRange( 0, STKLIB_MAX_PROGRESS );

		CPackage	pac;
		pac.m_nType		=	CPackage::packageDR;
		pac.m_bIsZipped	=	( sFileName.GetLength() > 4 && 0 == sFileName.Right(4).CompareNoCase(".zip") );
		pac.m_strDescript	=	sFileName;
		if( netdb.InstallPackageEx( pac, sFileName, ImportDRCallback, waitdlg.GetSafeHwnd() ) )
			AfxMessageBox( IDS_SETDRDATA_INSTALLOK, MB_OK|MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_SETDRDATA_INSTALLFAIL, MB_OK|MB_ICONINFORMATION );

		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
	}
}

void CSetDrdataDlg::OnImportfxj() 
{
	CNetDatabase	netdb;
	if( !netdb.SetRootPath( AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB ) )
	{
		AfxMessageBox( IDS_ROOTPATH_ERROR, MB_OK | MB_ICONINFORMATION );
		return;
	}

	CKSFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING,
		"Fxj Power files (*.zip,*.pwr)|*.zip;*.pwr|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		CString	sFileName	=	dlg.GetPathName();
		if( sFileName.GetLength() <= 0 || 0 != access(sFileName,0) )
		{
			AfxMessageBox( IDS_SETDRDATA_FILENOTEXISTS, MB_OK|MB_ICONINFORMATION );
			return;
		}
		
		CPackage	pac;
		pac.m_nType		=	CPackage::packageDR;
		pac.m_bIsZipped	=	( sFileName.GetLength() > 4 && 0 == sFileName.Right(4).CompareNoCase(".zip") );
		pac.m_strDescript	=	sFileName;
		if( netdb.InstallPackageEx( pac, sFileName, NULL, NULL ) )
			AfxMessageBox( IDS_SETDRDATA_INSTALLOK, MB_OK|MB_ICONINFORMATION );
		else
			AfxMessageBox( IDS_SETDRDATA_INSTALLFAIL, MB_OK|MB_ICONINFORMATION );

		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
	}
}
