// SetBasedataDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SetBasedataDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetBasedataDlg dialog

IMPLEMENT_DYNCREATE(CSetBasedataDlg, CPropertyPageEx)

CSetBasedataDlg::CSetBasedataDlg( )
	: CPropertyPageEx(CSetBasedataDlg::IDD)
{
	//{{AFX_DATA_INIT(CSetBasedataDlg)
	m_strA = _T("");
	m_strB = _T("");
	m_strH = _T("");
	m_strNational = _T("");
	m_strCorp = _T("");
	m_strTotal = _T("");
	//}}AFX_DATA_INIT
}

void CSetBasedataDlg::EnableEdit( BOOL bEnable )
{
	m_editA.EnableWindow( bEnable );
	m_editB.EnableWindow( bEnable );
	m_editH.EnableWindow( bEnable );
	m_editNational.EnableWindow( bEnable );
	m_editCorp.EnableWindow( bEnable );
	m_editTotal.EnableWindow( bEnable );
}

void CSetBasedataDlg::LoadBasedata( CString strStockCode )
{
	if( strStockCode.GetLength() <= 0 )
		return;

	CStockInfo	info;
	if( AfxGetStockContainer().GetStockInfo( strStockCode, &info ) )
	{
		m_strA.Format( "%.0f", info.m_fShare_count_a * 0.0001f );
		m_strB.Format( "%.0f", info.m_fShare_count_b * 0.0001f );
		m_strH.Format( "%.0f", info.m_fShare_count_h * 0.0001f );
		m_strNational.Format( "%.0f", info.m_fShare_count_national * 0.0001f );
		m_strCorp.Format( "%.0f", info.m_fShare_count_corp * 0.0001f );
		m_strTotal.Format( "%.0f", info.m_fShare_count_total * 0.0001f );
	}

	UpdateData( FALSE );
}

BOOL CSetBasedataDlg::StoreBasedata( CString strStockCode )
{
	if( strStockCode.GetLength() <= 0 )
		return TRUE;

	UpdateData( );

	if( !IsNumber(m_strA,TRUE) || !IsNumber(m_strB,TRUE) || !IsNumber(m_strH,TRUE)
		|| !IsNumber(m_strNational,TRUE) || !IsNumber(m_strCorp,TRUE) || !IsNumber(m_strTotal,TRUE) )
	{
		AfxMessageBox( IDS_SETBASEDATA_NOTNUMBER );
		return FALSE;
	}

	CStockInfo	info;
	int	id = 0;
	CStockContainer & container = AfxGetStockContainer();
	if( container.GetStockInfo( strStockCode, &info, &id ) )
	{
		info.m_fShare_count_a	=	(float)(atof(m_strA) * 10000);
		info.m_fShare_count_b	=	(float)(atof(m_strB) * 10000);
		info.m_fShare_count_h	=	(float)(atof(m_strH) * 10000);
		info.m_fShare_count_national	=	(float)(atof(m_strNational) * 10000);
		info.m_fShare_count_corp		=	(float)(atof(m_strCorp) * 10000);
		info.m_fShare_count_total		=	(float)(atof(m_strTotal) * 10000);
		int nBaseIdx = info.m_basedata.GetSize()-1;
		if( nBaseIdx >= 0 )
		{
			info.m_basedata.ElementAt(nBaseIdx).m_fShare_count_a	=	info.m_fShare_count_a;
			info.m_basedata.ElementAt(nBaseIdx).m_fShare_count_b	=	info.m_fShare_count_b;
			info.m_basedata.ElementAt(nBaseIdx).m_fShare_count_h	=	info.m_fShare_count_h;
			info.m_basedata.ElementAt(nBaseIdx).m_fShare_count_national	=	info.m_fShare_count_national;
			info.m_basedata.ElementAt(nBaseIdx).m_fShare_count_corp		=	info.m_fShare_count_corp;
			info.m_basedata.ElementAt(nBaseIdx).m_fShare_count_total	=	info.m_fShare_count_total;
		}

		float temp = info.m_fShare_count_a+info.m_fShare_count_b+info.m_fShare_count_h+info.m_fShare_count_national+info.m_fShare_count_corp;
		if( info.m_fShare_count_total+1 < temp )
		{
			AfxMessageBox( IDS_SETBASEDATA_SHARENOTMATCH );
			return FALSE;
		}

		if( id >= 0 && id < container.GetSize() )
			AfxGetStockContainer().ElementAt( id )	=	info;
	}
	return TRUE;
}

void CSetBasedataDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetBasedataDlg)
	DDX_Control(pDX, IDC_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_EDITTOTAL, m_editTotal);
	DDX_Control(pDX, IDC_EDITCORP, m_editCorp);
	DDX_Control(pDX, IDC_EDITNATIONAL, m_editNational);
	DDX_Control(pDX, IDC_EDITH, m_editH);
	DDX_Control(pDX, IDC_EDITB, m_editB);
	DDX_Control(pDX, IDC_EDITA, m_editA);
	DDX_Control(pDX, IDC_STATICSTOCK, m_staticStock);
	DDX_Control(pDX, IDC_LIST_STOCKSRC, m_listStockSrc);
	DDX_Control(pDX, IDC_COMBO_GROUPSRC, m_comboGroupSrc);
	DDX_Control(pDX, IDC_EDITCODE, m_editCode);
	DDX_Text(pDX, IDC_EDITA, m_strA);
	DDX_Text(pDX, IDC_EDITB, m_strB);
	DDX_Text(pDX, IDC_EDITH, m_strH);
	DDX_Text(pDX, IDC_EDITNATIONAL, m_strNational);
	DDX_Text(pDX, IDC_EDITCORP, m_strCorp);
	DDX_Text(pDX, IDC_EDITTOTAL, m_strTotal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetBasedataDlg, CPropertyPageEx)
	//{{AFX_MSG_MAP(CSetBasedataDlg)
	ON_EN_CHANGE(IDC_EDITCODE, OnChangeEditcode)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUPSRC, OnSelchangeComboGroupsrc)
	ON_LBN_SELCHANGE(IDC_LIST_STOCKSRC, OnSelchangeListStocksrc)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBasedataDlg message handlers

BOOL CSetBasedataDlg::OnInitDialog()
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetBasedataDlg::OnChangeEditcode() 
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

void CSetBasedataDlg::OnSelchangeComboGroupsrc() 
{
	// TODO: Add your control notification handler code here
	int nType = m_comboGroupSrc.GetCurSelType();
	CString strName = m_comboGroupSrc.GetCurSelName();
	m_listStockSrc.SetCurrentStocks( nType, strName, AfxGetGroupContainer() );
}

BOOL CSetBasedataDlg::PreTranslateMessage(MSG* pMsg) 
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

void CSetBasedataDlg::OnSelchangeListStocksrc() 
{
	// TODO: Add your control notification handler code here
	HWND hWndFocus = ::GetFocus( );
	int	nSel	=	m_listStockSrc.GetCurSel( );

	//if( !StoreBasedata( m_strCurStockCode ) )
	//{
	//	m_listStockSrc.SelectCode( 0, m_strCurStockCode );
	//	return;
	//}

	if( LB_ERR != nSel )
	{
		CString	rString, strStockCode;
		m_listStockSrc.GetText(nSel,rString);
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );

		m_staticStock.SetWindowText( rString );

		m_strCurStockCode	=	strStockCode;
		LoadBasedata( m_strCurStockCode );
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

void CSetBasedataDlg::OnCompleted( )
{
	// StoreBasedata( m_strCurStockCode );
	AfxGetDB().StoreBasetable( AfxGetStockContainer() );
	AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy() );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_BASEVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
}

void CSetBasedataDlg::OnOK() 
{
	OnCompleted( );

	CPropertyPageEx::OnOK();
}

void CSetBasedataDlg::OnCancel() 
{
	OnCompleted( );
	
	CPropertyPageEx::OnCancel();
}

BOOL CSetBasedataDlg::OnKillActive() 
{
	OnCompleted( );

	return CPropertyPageEx::OnKillActive();
}

void CSetBasedataDlg::OnSave() 
{
	StoreBasedata( m_strCurStockCode );
}
