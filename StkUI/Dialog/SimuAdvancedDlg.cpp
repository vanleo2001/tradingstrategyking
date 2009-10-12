// SimuAdvancedDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SimuAdvancedDlg.h"
#include "SetRule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuAdvancedDlg dialog


CSimuAdvancedDlg::CSimuAdvancedDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSimuAdvancedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuAdvancedDlg)
	m_strStopLosing = _T("");
	m_strStopProfit = _T("");
	//}}AFX_DATA_INIT

	m_pStrategy	=	NULL;
	m_bModified	=	FALSE;
}

void CSimuAdvancedDlg::SetStrategy( CStrategy * pStrategy )
{
	m_pStrategy	=	pStrategy;
}

void CSimuAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuAdvancedDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_RULELONG, m_btnRuleLong);
	DDX_Control(pDX, IDC_RULEINDEX, m_btnRuleIndex);
	DDX_Control(pDX, IDC_KTYPELONG, m_cmbKTypeLong);
	DDX_Control(pDX, IDC_KTYPEINDEX, m_cmbKTypeIndex);
	DDX_Control(pDX, IDC_CHECKLONG, m_btnCheckLong);
	DDX_Control(pDX, IDC_CHECKINDEX, m_btnCheckIndex);
	DDX_Control(pDX, IDC_STOPPROFIT, m_editStopProfit);
	DDX_Control(pDX, IDC_STOPLOSING, m_editStopLosing);
	DDX_Control(pDX, IDC_CHECKSTOPPROFIT, m_btnCheckStopProfit);
	DDX_Control(pDX, IDC_CHECKSTOPLOSING, m_btnCheckStopLosing);
	DDX_Text(pDX, IDC_STOPLOSING, m_strStopLosing);
	DDX_Text(pDX, IDC_STOPPROFIT, m_strStopProfit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimuAdvancedDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSimuAdvancedDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECKSTOPLOSING, OnCheckstoplosing)
	ON_BN_CLICKED(IDC_CHECKSTOPPROFIT, OnCheckstopprofit)
	ON_BN_CLICKED(IDC_CHECKLONG, OnChecklong)
	ON_BN_CLICKED(IDC_CHECKINDEX, OnCheckindex)
	ON_BN_CLICKED(IDC_RULELONG, OnRulelong)
	ON_BN_CLICKED(IDC_RULEINDEX, OnRuleindex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuAdvancedDlg message handlers

BOOL CSimuAdvancedDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	ASSERT( m_pStrategy );

	COpParam & param	=	m_pStrategy->GetOpParam();
	// Stop
	m_btnCheckStopLosing.SetCheck( param.m_bStopLosing );
	m_btnCheckStopProfit.SetCheck( param.m_bStopProfit );

	CString	strTemp;
	strTemp.Format( "%.2f", param.m_dStopLosing * 100 );
	m_editStopLosing.SetWindowText( strTemp );
	strTemp.Format( "%.2f", param.m_dStopProfit * 100 );
	m_editStopProfit.SetWindowText( strTemp );

	OnCheckstoplosing( );
	OnCheckstopprofit( );

	// Long Index
	m_btnCheckLong.SetCheck( param.m_bLongTrend );
	m_btnCheckIndex.SetCheck( param.m_bIndexTrend );

	OnChecklong();
	OnCheckindex();

	m_cmbKTypeLong.InitializeWeek();
	m_cmbKTypeIndex.InitializeDay();
	m_cmbKTypeLong.Select( param.m_nKTypeLong );
	m_cmbKTypeIndex.Select( param.m_nKTypeIndex );

	m_bModified	=	FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSimuAdvancedDlg::OnOK() 
{
	UpdateData( );

	ASSERT( m_pStrategy );

	// Stop
	if( m_btnCheckStopLosing.GetCheck() && !IsNumber(m_strStopLosing,FALSE) )
	{
		AfxMessageBox( IDS_SIMUADVANCED_NOTNUMBER, MB_OK|MB_ICONINFORMATION );
		return;
	}
	if( m_btnCheckStopProfit.GetCheck() && !IsNumber(m_strStopProfit,FALSE) )
	{
		AfxMessageBox( IDS_SIMUADVANCED_NOTNUMBER, MB_OK|MB_ICONINFORMATION );
		return;
	}
	if( m_btnCheckLong.GetCheck() && m_pStrategy->GetSelectedTechsLongTrend().GetSize() <= 0 )
	{
		AfxMessageBox( IDS_SIMUADVANCED_NOSELECTEDLONG, MB_OK|MB_ICONINFORMATION );
		return;
	}
	if( m_btnCheckIndex.GetCheck() && m_pStrategy->GetSelectedTechsIndexTrend().GetSize() <= 0 )
	{
		AfxMessageBox( IDS_SIMUADVANCED_NOSELECTEDINDEX, MB_OK|MB_ICONINFORMATION );
		return;
	}
	
	// Stop
	COpParam & param	=	m_pStrategy->GetOpParam();
	param.m_bStopLosing	=	m_btnCheckStopLosing.GetCheck();
	param.m_bStopProfit	=	m_btnCheckStopProfit.GetCheck();
	param.m_dStopLosing	=	atof(m_strStopLosing) / 100.;
	param.m_dStopProfit	=	atof(m_strStopProfit) / 100.;

	// Long Index
	param.m_bLongTrend	=	m_btnCheckLong.GetCheck();
	param.m_bIndexTrend	=	m_btnCheckIndex.GetCheck();
	param.m_nKTypeLong	=	m_cmbKTypeLong.GetSelect();
	param.m_nKTypeIndex	=	m_cmbKTypeIndex.GetSelect();

	CXFDialog::OnOK();
}

// 止损
void CSimuAdvancedDlg::OnCheckstoplosing() 
{
	int	nCheck	=	m_btnCheckStopLosing.GetCheck();
	m_editStopLosing.EnableWindow( nCheck );
	m_bModified	=	TRUE;
}

// 止赢
void CSimuAdvancedDlg::OnCheckstopprofit() 
{
	int	nCheck	=	m_btnCheckStopProfit.GetCheck();
	m_editStopProfit.EnableWindow( nCheck );
	m_bModified	=	TRUE;
}

// 长期趋势
void CSimuAdvancedDlg::OnChecklong() 
{
	int	nCheck	=	m_btnCheckLong.GetCheck();
	m_cmbKTypeLong.EnableWindow( nCheck );
	m_btnRuleLong.EnableWindow( nCheck );
	m_bModified	=	TRUE;

	if( nCheck && m_pStrategy && m_pStrategy->GetSelectedTechsLongTrend().GetSize() <= 0 )
	{
		OnRulelong();
		m_cmbKTypeLong.Invalidate();
	}
}

// 指数趋势
void CSimuAdvancedDlg::OnCheckindex() 
{
	int	nCheck	=	m_btnCheckIndex.GetCheck();
	m_cmbKTypeIndex.EnableWindow( nCheck );
	m_btnRuleIndex.EnableWindow( nCheck );
	m_bModified	=	TRUE;
	
	if( nCheck && m_pStrategy && m_pStrategy->GetSelectedTechsIndexTrend().GetSize() <= 0 )
	{
		OnRuleindex();
		m_cmbKTypeIndex.Invalidate();
	}
}

// 设定长期趋势参数
void CSimuAdvancedDlg::OnRulelong() 
{
	ASSERT( m_pStrategy );

	CSetRule		setrule(IDS_STRATEGY_SETRULELONG, IDS_STRATEGY_SETRULELONG, NULL);
	setrule.SetTechParameters( &(m_pStrategy->GetTechParametersLongTrend()), &(m_pStrategy->GetSelectedTechsLongTrend()) );

	CPropertySheetEx	sht;
	sht.AddPage( &setrule );
	sht.SetActivePage( &setrule );

	//CBitmap	bmp;
	//bmp.LoadBitmap( IDB_BALANG );
	//sht.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
	sht.DoModal();

	m_bModified	=	setrule.m_bModified;
}

// 设定指数趋势参数
void CSimuAdvancedDlg::OnRuleindex() 
{
	ASSERT( m_pStrategy );

	CSetRule		setrule(IDS_STRATEGY_SETRULEINDEX, IDS_STRATEGY_SETRULEINDEX, NULL);
	setrule.SetTechParameters( &(m_pStrategy->GetTechParametersIndexTrend()), &(m_pStrategy->GetSelectedTechsIndexTrend()) );

	CPropertySheetEx	sht;
	sht.AddPage( &setrule );
	sht.SetActivePage( &setrule );

	//CBitmap	bmp;
	//bmp.LoadBitmap( IDB_BALANG );
	//sht.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
	sht.DoModal();

	m_bModified	=	setrule.m_bModified;
}
