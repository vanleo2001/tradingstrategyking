// SetKDataDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SetKDataDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetKDataDlg dialog

IMPLEMENT_DYNCREATE(CSetKDataDlg, CPropertyPageEx)

CSetKDataDlg::CSetKDataDlg( )
	: CPropertyPageEx(CSetKDataDlg::IDD)
{
	//{{AFX_DATA_INIT(CSetKDataDlg)
	m_tmDate = 0;
	m_strOpen = _T("");
	m_strHigh = _T("");
	m_strLow = _T("");
	m_strClose = _T("");
	m_dwVolume = 0;
	m_dwAmount = 0;
	//}}AFX_DATA_INIT
	m_tmDate = CSPTime::GetCurrentTime();
}

void CSetKDataDlg::EnableEdit( BOOL bEnable )
{
	m_editOpen.EnableWindow( bEnable );
	m_editHigh.EnableWindow( bEnable );
	m_editLow.EnableWindow( bEnable );
	m_editClose.EnableWindow( bEnable );
	m_editVolume.EnableWindow( bEnable );
	m_editAmount.EnableWindow( bEnable );
}

void CSetKDataDlg::LoadKData( CString strStockCode )
{
	if( strStockCode.GetLength() <= 0 )
		return;

	UpdateData();

	m_strOpen.Empty();
	m_strHigh.Empty();
	m_strLow.Empty();
	m_strClose.Empty();
	m_dwVolume = 0;
	m_dwAmount = 0;

	if( m_tmDate.GetTime() == -1 )
	{
		UpdateData( FALSE );
		return;
	}

	CStockInfo info;
	CStock	stock;
	if( !info.SetStockCode( CStock::marketUnknown, strStockCode ) )
		return;
	stock.SetStockInfo( &info );
	
	int nKType = m_comboKType.GetSelect();
	AfxPrepareStockData( &AfxGetDB(), stock, nKType,  CKData::formatOriginal, CKData::mdtypeClose, FALSE, TRUE );
	CKData & kdata = stock.GetKData(nKType);

	// 价格小数位数格式串
	CString strPriceFmt;
	strPriceFmt.Format( "%%.%df", info.DigitBit() );

	CSPTime sptime = m_tmDate.GetTime();
	int nIndex = kdata.GetIndexByDate( sptime.ToStockTime(CKData::IsDayOrMin(kdata.GetKType())) );
	if( nIndex >= 0 && nIndex < kdata.GetSize() )
	{
		KDATA kd = kdata.ElementAt(nIndex);

		m_strOpen.Format( strPriceFmt, kd.m_fOpen );
		m_strHigh.Format( strPriceFmt, kd.m_fHigh );
		m_strLow.Format( strPriceFmt, kd.m_fLow );
		m_strClose.Format( strPriceFmt, kd.m_fClose );
		m_dwVolume	=	(DWORD)(kd.m_fVolume/100);
		m_dwAmount	=	(DWORD)(kd.m_fAmount/1000);
	}

	UpdateData( FALSE );
}

BOOL CSetKDataDlg::StoreKData( CString strStockCode )
{
	if( strStockCode.GetLength() <= 0 )
		return FALSE;

	UpdateData( );

	if( m_tmDate.GetTime() == -1 )
		return FALSE;

	if( !IsNumber(m_strOpen,FALSE) || !IsNumber(m_strHigh,FALSE) || !IsNumber(m_strLow,FALSE)
		|| !IsNumber(m_strClose,FALSE) )
	{
		AfxMessageBox( IDS_SETKDATA_NOTNUMBER );
		return FALSE;
	}

	DWORD dwMarket = CStock::marketUnknown;
	CStockInfo info;
	if( info.SetStockCode(dwMarket, strStockCode) )
		dwMarket = info.GetMarket();
	
	int nKType = m_comboKType.GetSelect();

	CSPTime sptime = m_tmDate.GetTime();
	KDATA kd;
	memset( &kd, 0, sizeof(kd) );
	kd.m_dwMarket = dwMarket;
	strncpy( kd.m_szCode, strStockCode, min(sizeof(kd.m_szCode)-1,strStockCode.GetLength()) );
	kd.m_time = m_tmDate.GetTime();
	kd.m_date = sptime.ToStockTime(CKData::IsDayOrMin(nKType));
	kd.m_fOpen	=	(float)atof(m_strOpen);
	kd.m_fHigh	=	(float)atof(m_strHigh);
	kd.m_fLow		=	(float)atof(m_strLow);
	kd.m_fClose	=	(float)atof(m_strClose);
	kd.m_fVolume	=	(float)m_dwVolume*100;
	kd.m_fAmount	=	(float)m_dwAmount*1000;

	CKData kdata(nKType);
	kdata.Add( kd );
	AfxGetDB().InstallKData( kdata );

	return TRUE;
}

BOOL CSetKDataDlg::DeleteKData( CString strStockCode )
{
	if( strStockCode.GetLength() <= 0 )
		return FALSE;

	UpdateData( );

	if( m_tmDate.GetTime() == -1 )
		return FALSE;

	CStockInfo info;
	CStock	stock;
	if( !info.SetStockCode( CStock::marketUnknown, strStockCode ) )
		return FALSE;
	stock.SetStockInfo( &info );
	
	int nKType = m_comboKType.GetSelect();
	AfxPrepareStockData( &AfxGetDB(), stock, nKType,  CKData::formatOriginal, CKData::mdtypeClose, FALSE, TRUE );
	CKData & kdata = stock.GetKData(nKType);

	CSPTime sptime = m_tmDate.GetTime();
	int nIndex = kdata.GetIndexByDate( sptime.ToStockTime(CKData::IsDayOrMin(kdata.GetKType())) );
	if( nIndex >= 0 && nIndex < kdata.GetSize() )
	{
		kdata.RemoveAt(nIndex);
		AfxGetDB().InstallKData( kdata, TRUE );
		return TRUE;
	}
	return FALSE;
}

void CSetKDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetKDataDlg)
	DDX_Control(pDX, IDC_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_KTYPE, m_comboKType);
	DDX_Control(pDX, IDC_AMOUNT, m_editAmount);
	DDX_Control(pDX, IDC_VOLUME, m_editVolume);
	DDX_Control(pDX, IDC_CLOSE, m_editClose);
	DDX_Control(pDX, IDC_LOW, m_editLow);
	DDX_Control(pDX, IDC_HIGH, m_editHigh);
	DDX_Control(pDX, IDC_OPEN, m_editOpen);
	DDX_Control(pDX, IDC_DATE, m_dtcDate);
	DDX_Control(pDX, IDC_EDITCODE, m_editCode);
	DDX_Control(pDX, IDC_STATICSTOCK, m_staticStock);
	DDX_Control(pDX, IDC_LIST_STOCKSRC, m_listStockSrc);
	DDX_Control(pDX, IDC_COMBO_GROUPSRC, m_comboGroupSrc);
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_tmDate);
	DDX_Text(pDX, IDC_OPEN, m_strOpen);
	DDX_Text(pDX, IDC_HIGH, m_strHigh);
	DDX_Text(pDX, IDC_LOW, m_strLow);
	DDX_Text(pDX, IDC_CLOSE, m_strClose);
	DDX_Text(pDX, IDC_VOLUME, m_dwVolume);
	DDX_Text(pDX, IDC_AMOUNT, m_dwAmount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetKDataDlg, CPropertyPageEx)
	//{{AFX_MSG_MAP(CSetKDataDlg)
	ON_EN_CHANGE(IDC_EDITCODE, OnChangeEditcode)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUPSRC, OnSelchangeComboGroupsrc)
	ON_LBN_SELCHANGE(IDC_LIST_STOCKSRC, OnSelchangeListStocksrc)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE, OnDatetimechangeDate)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetKDataDlg message handlers

BOOL CSetKDataDlg::OnInitDialog()
{
	CPropertyPageEx::OnInitDialog();

	EnableEdit( FALSE );

	CWaitCursor	waitcursor;

	// TODO: Add extra initialization here
	CDomainContainer & groups = AfxGetGroupContainer( );
	CDomainContainer & domains = AfxGetDomainContainer( );
	CStockContainer & container = AfxGetStockContainer();

	// Src
	m_comboGroupSrc.InitStrings( TRUE, TRUE, groups );
	m_comboGroupSrc.SetCurSel( 0 );
	OnSelchangeComboGroupsrc();

	OnSelchangeListStocksrc();

	m_comboKType.InitializeDayMin5();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetKDataDlg::OnChangeEditcode() 
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

void CSetKDataDlg::OnSelchangeComboGroupsrc() 
{
	// TODO: Add your control notification handler code here
	int nType = m_comboGroupSrc.GetCurSelType();
	CString strName = m_comboGroupSrc.GetCurSelName();
	m_listStockSrc.SetCurrentStocks( nType, strName, AfxGetGroupContainer() );
}

BOOL CSetKDataDlg::PreTranslateMessage(MSG* pMsg) 
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

void CSetKDataDlg::OnSelchangeListStocksrc() 
{
	// TODO: Add your control notification handler code here
	HWND hWndFocus = ::GetFocus( );
	int	nSel	=	m_listStockSrc.GetCurSel( );

	if( LB_ERR != nSel )
	{
		CString	rString, strStockCode;
		m_listStockSrc.GetText(nSel,rString);
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );

		m_staticStock.SetWindowText( rString );

		m_strCurStockCode	=	strStockCode;
		LoadKData( m_strCurStockCode );
	}
	else
	{
		CString	strNoStock;
		strNoStock.LoadString( IDS_SETBASEDATA_NOSTOCK );
		m_staticStock.SetWindowText( strNoStock );
		m_strCurStockCode.Empty();
	}

	EnableEdit( m_strCurStockCode.GetLength() > 0 );
	::SetFocus( hWndFocus );
}

void CSetKDataDlg::OnCompleted( )
{
	AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy() );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
}

void CSetKDataDlg::OnOK() 
{
	OnCompleted( );

	CPropertyPageEx::OnOK();
}

void CSetKDataDlg::OnCancel() 
{
	OnCompleted( );
	
	CPropertyPageEx::OnCancel();
}

BOOL CSetKDataDlg::OnKillActive() 
{
	OnCompleted( );

	return CPropertyPageEx::OnKillActive();
}

void CSetKDataDlg::OnDatetimechangeDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LoadKData( m_strCurStockCode );
	*pResult = 0;
}

void CSetKDataDlg::OnSave() 
{
	StoreKData( m_strCurStockCode );
}

void CSetKDataDlg::OnDelete() 
{
	if( DeleteKData( m_strCurStockCode ) )
	{
		m_tmDate = CSPTime::GetCurrentTime();
		UpdateData( FALSE );
		LoadKData( m_strCurStockCode );
	}
}
