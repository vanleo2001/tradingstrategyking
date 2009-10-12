// SimuAddOpDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SimuAddOpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuAddOpDlg dialog


CSimuAddOpDlg::CSimuAddOpDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSimuAddOpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuAddOpDlg)
	m_time = 0;
	m_strShare = _T("");
	m_strSharePrice = _T("");
	m_strRateCost = _T("");
	//}}AFX_DATA_INIT
	m_time	=	CSPTime::GetCurrentTime();
	m_strShare = _T("");
	m_strSharePrice = _T("");
	m_strRateCost = _T("0");

	m_pStrategy	=	NULL;
}

void CSimuAddOpDlg::SetStrategy( CStrategy * pStrategy )
{
	m_pStrategy	=	pStrategy;
}

/***
	自动设定交易费用
*/
void CSimuAddOpDlg::AutoSetRateCost( )
{
	UpdateData( );

	if( NULL == m_pStrategy )
		return;

	// Share
	if( !IsNumber( m_strShare, TRUE ) || !IsNumber( m_strSharePrice, TRUE ) || !IsNumber( m_strRateCost, TRUE ) )
	{
		AfxMessageBox( IDS_SIMUADDOP_NOTNUMBER, MB_OK|MB_ICONINFORMATION );
		return;
	}

	DWORD	dwShare		=	atol(m_strShare);
	double	dSharePrice	=	atol(m_strSharePrice);
	
	CString	strStockCode	=	m_comboStock.GetSelectedStockCode();
	CStockInfo	info;
	if( !AfxGetStockContainer().GetStockInfo( strStockCode, &info )
		|| !info.IsValidStock() )
		return;
	m_strRateCost.Format( "%.2f", dwShare*dSharePrice*m_pStrategy->GetRateParam().GetRate(info) );
	UpdateData( FALSE );
}

void CSimuAddOpDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuAddOpDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_OPERATE, m_comboOperate);
	DDX_Control(pDX, IDC_STOCK, m_comboStock);
	DDX_DateTimeCtrl(pDX, IDC_TIME, m_time);
	DDX_Text(pDX, IDC_SHARE, m_strShare);
	DDX_Text(pDX, IDC_SHAREPRICE, m_strSharePrice);
	DDX_Text(pDX, IDC_RATECOST, m_strRateCost);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimuAddOpDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSimuAddOpDlg)
	ON_EN_CHANGE(IDC_SHARE, OnChangeShare)
	ON_EN_CHANGE(IDC_SHAREPRICE, OnChangeShareprice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuAddOpDlg message handlers

BOOL CSimuAddOpDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	ASSERT( m_pStrategy );

	// Operate
	CString	strBuy, strSell;
	strBuy.LoadString( IDS_SIMUREPORT_BUY );
	strSell.LoadString( IDS_SIMUREPORT_SELL );
	m_comboOperate.SetItemData( m_comboOperate.AddString( strBuy ), STRATEGY_OPTYPE_BUY );
	m_comboOperate.SetItemData( m_comboOperate.AddString( strSell ), STRATEGY_OPTYPE_SELL );
	m_comboOperate.SetCurSel( 0 );

	// Stock
	m_comboStock.InitStocks( m_pStrategy->GetStocks(), FALSE, FALSE, FALSE );
	m_comboStock.SetAutoHide( FALSE );
	m_comboStock.SetCurSel( 0 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSimuAddOpDlg::OnOK() 
{
	UpdateData( );

	OPRECORD	record;
	memset( &record, 0, sizeof(record) );
	
	// Time
	CSPTime	tmDay( m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), 0, 0, 0 );
	record.time	=	tmDay.GetTime();

	// Stock
	CString	strStockCode	=	m_comboStock.GetSelectedStockCode();
	if( strStockCode.GetLength() <= 0 )
	{
		AfxMessageBox( IDS_SIMUADDOP_NOSTOCK, MB_OK|MB_ICONINFORMATION );
		return;
	}
	strncpy( record.szCode, strStockCode.GetBuffer(sizeof(record.szCode)+1), sizeof(record.szCode) );
	strStockCode.ReleaseBuffer( );

	// Operate
	int	nSel	=	m_comboOperate.GetCurSel();
	if( CB_ERR != nSel )
		record.lOpType	=	m_comboOperate.GetItemData(nSel);

	// Share
	if( !IsNumber( m_strShare, FALSE ) || !IsNumber( m_strSharePrice, FALSE ) || !IsNumber( m_strRateCost, TRUE ) )
	{
		AfxMessageBox( IDS_SIMUADDOP_NOTNUMBER, MB_OK|MB_ICONINFORMATION );
		return;
	}

	record.dwShare		=	atol(m_strShare);
	record.dSharePrice	=	atol(m_strSharePrice);
	record.dRateCost	=	atol(m_strRateCost);

	COpRecordContainer & container = m_pStrategy->RealGetOpRecord();
	if( container.GetSize() > 0 && container.ElementAt(container.GetSize()-1).time > record.time
		&& IDYES != AfxMessageBox( IDS_SIMUADDOP_ADDWILLDELETE, MB_YESNO ) )
	{
		return;
	}

	if( !m_pStrategy || !m_pStrategy->RealAddOpRecordStrict( record ) )
	{
		AfxMessageBox( IDS_SIMUADDOP_LOGICERROR, MB_OK|MB_ICONINFORMATION );
		return;
	}
	
	CXFDialog::OnOK();
}

void CSimuAddOpDlg::OnChangeShare() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CXFDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	AutoSetRateCost( );
}

void CSimuAddOpDlg::OnChangeShareprice() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CXFDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	AutoSetRateCost( );
}
