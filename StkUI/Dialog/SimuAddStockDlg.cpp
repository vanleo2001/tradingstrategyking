// SimuAddStockDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SimuAddStockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuAddStockDlg dialog


CSimuAddStockDlg::CSimuAddStockDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSimuAddStockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuAddStockDlg)
	m_strValue = _T("");
	m_strPrice = _T("");
	//}}AFX_DATA_INIT
	m_pStrategy		=	NULL;
	m_nMode			=	modeAddStock;
	m_dValue		=	0;
}

void CSimuAddStockDlg::SetStrategy( CStrategy * pStrategy )
{
	m_pStrategy	=	pStrategy;
}

/***
	modeAddStock		表示该对话框完成添加拥有股票的功能
	modeModifiedCash	表示该对话框完成修改拥有现金的功能
*/
void CSimuAddStockDlg::SetMode( int nMode, CString sCode, double dValue, double dPrice )
{
	m_nMode			=	nMode;
	m_strStockCode	=	sCode;
	m_dValue		=	dValue;
	m_dPrice		=	dPrice;
}

CString CSimuAddStockDlg::GetStockCode( )
{
	return m_strStockCode;
}

double CSimuAddStockDlg::GetValue( )
{
	return m_dValue;
}

double CSimuAddStockDlg::GetPrice( )
{
	return m_dPrice;
}

void CSimuAddStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuAddStockDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STATICPRICE, m_staticPrice);
	DDX_Control(pDX, IDC_PRICE, m_editPrice);
	DDX_Control(pDX, IDC_STATICVALUE, m_staticValue);
	DDX_Control(pDX, IDC_STATICSTOCK, m_staticStock);
	DDX_Control(pDX, IDC_STOCK, m_comboStock);
	DDX_Text(pDX, IDC_VALUE, m_strValue);
	DDX_Text(pDX, IDC_PRICE, m_strPrice);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimuAddStockDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSimuAddStockDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuAddStockDlg message handlers

BOOL CSimuAddStockDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	ASSERT( m_pStrategy );
	
	m_comboStock.InitStocks( m_pStrategy->GetStocks(), FALSE, FALSE, FALSE );
	m_comboStock.SetAutoHide( FALSE );
	m_comboStock.SetCurSel( 0 );

	if( modeAddStock == m_nMode )
	{
		m_strValue.Format( "%u", 0 );
	}
	else if( modeModifiedCash == m_nMode )
	{
		m_comboStock.ShowWindow( SW_HIDE );
		m_staticStock.ShowWindow( SW_HIDE );
		m_editPrice.ShowWindow( SW_HIDE );
		m_staticPrice.ShowWindow( SW_HIDE );
		CString	string;
		string.LoadString( IDS_SIMUADDSTOCK_CASH );
		m_staticValue.SetWindowText( string );
		string.LoadString( IDS_SIMUADDSTOCK_MODCASH );
		SetWindowText( string );
		m_strValue.Format( "%.2f", m_dValue );
	}
	
	UpdateData( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSimuAddStockDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	if( !IsNumber(m_strValue,FALSE) )
	{
		AfxMessageBox( IDS_SIMUADDOP_NOTNUMBER, MB_OK|MB_ICONINFORMATION );
		return;
	}
	m_dValue	=	atof(m_strValue);

	if( modeAddStock == m_nMode )
	{
		m_strStockCode	=	m_comboStock.GetSelectedStockCode();
		if( !IsNumber(m_strPrice,FALSE) )
		{
			AfxMessageBox( IDS_SIMUADDOP_NOTNUMBER, MB_OK|MB_ICONINFORMATION );
			return;
		}
		m_dPrice	=	atof(m_strPrice);
	}

	CXFDialog::OnOK();
}
