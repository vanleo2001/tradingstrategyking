// SimuRealOp.cpp : implementation file
//

#include "stdafx.h"

#include "SimuRealOp.h"
#include "SimuStatDlg.h"
#include "SimuAddOpDlg.h"
#include "SimuAddStockDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuRealOp property page

CSimuRealOp::CSimuRealOp(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSimuRealOp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuRealOp)
	//}}AFX_DATA_INIT
	m_pStrategy	=	NULL;
	SetTransparent( FALSE );

	m_bModified	=	FALSE;
}

CSimuRealOp::~CSimuRealOp()
{
}

void CSimuRealOp::SetStrategy( CStrategy * pStrategy )
{
	m_pStrategy	=	pStrategy;
}

/***
	初始化实战操作数据列表等
*/
BOOL CSimuRealOp::Initialize( )
{
	m_listNextOp.InitNextOpList( );
	m_listOpRecord.InitOpRecordList( );
	m_listStockOwn.InitStockOwnList( );
	m_listOpRecord.LoadOpRecordList( m_pStrategy->RealGetOpRecord() );
	m_listStockOwn.LoadStockOwnList( m_pStrategy->RealGetStockOwn(), m_pStrategy->RealGetCurrentCash(), m_pStrategy->RealGetCurrentTime().GetTime(),
						m_pStrategy->RealGetAsset(m_pStrategy->RealGetCurrentTime()), m_pStrategy->GetTechStockContainer() );

	// Curve Init
	m_curve.SetAssetSerial( m_pStrategy->RealGetAssetSerial(), m_pStrategy->GetOpParam().m_nStartAmount );
	m_curve.Initialize( );

	// if( !AfxAssertFunclimitProf( IDS_SIMUREALOP_EVACANNOTVIEWNEXTOP ) )
	//	return TRUE;

	m_listNextOp.LoadNextOpList( m_pStrategy->RealGetNextOp() );
	return TRUE;
}

void CSimuRealOp::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuRealOp)
	DDX_Control(pDX, IDC_STATYIELD, m_btnStatYield);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_TIMEEXPLAIN, m_staticTimeExplain);
	DDX_Control(pDX, IDC_STATICEXPLAIN, m_staticExplain);
	DDX_Control(pDX, IDC_RESETALL, m_btnResetAll);
	DDX_Control(pDX, IDC_MODIFIED, m_btnModified);
	DDX_Control(pDX, IDC_DELETESTOCK, m_btnDeleteStock);
	DDX_Control(pDX, IDC_ADDSTOCK, m_btnAddStock);
	DDX_Control(pDX, IDC_DELETEOP, m_btnDeleteOp);
	DDX_Control(pDX, IDC_ADDOP, m_btnAddOp);
	DDX_Control(pDX, IDC_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_STOCKOWN, m_listStockOwn);
	DDX_Control(pDX, IDC_CURVE, m_curve);
	DDX_Control(pDX, IDC_NEXTOP, m_listNextOp);
	DDX_Control(pDX, IDC_OPRECORD, m_listOpRecord);
	DDX_Control(pDX, IDC_TABCTRL, m_tabctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimuRealOp, CXFDialog)
	//{{AFX_MSG_MAP(CSimuRealOp)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, OnSelchangeTabctrl)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_STATYIELD, OnStatyield)
	ON_BN_CLICKED(IDC_ADDOP, OnAddop)
	ON_BN_CLICKED(IDC_DELETEOP, OnDeleteop)
	ON_BN_CLICKED(IDC_ADDSTOCK, OnAddstock)
	ON_BN_CLICKED(IDC_DELETESTOCK, OnDeletestock)
	ON_BN_CLICKED(IDC_MODIFIED, OnModified)
	ON_BN_CLICKED(IDC_RESETALL, OnResetall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuRealOp message handlers

BOOL CSimuRealOp::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ASSERT( m_pStrategy );

	// Set Attr
	CString	strItem;
	strItem.LoadString( IDS_SIMUREALOP_NEXTOP );
	m_tabctrl.InsertItem( 0, strItem );
	strItem.LoadString( IDS_SIMUREPORT_OPRECORD );
	m_tabctrl.InsertItem( 1, strItem );
	strItem.LoadString( IDS_SIMUREPORT_CURVE );
	m_tabctrl.InsertItem( 2, strItem );
	strItem.LoadString( IDS_SIMUREALOP_STOCKOWN );
	m_tabctrl.InsertItem( 3, strItem );

	// Init
	Initialize( );

	// Tab Show
	m_listNextOp.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
	m_listOpRecord.ShowWindow( SW_HIDE );
	m_curve.ShowWindow( SW_HIDE );
	m_listStockOwn.ShowWindow( SW_HIDE );
	m_btnCopy.EnableWindow( TRUE );
	m_btnAddOp.ShowWindow( SW_HIDE );
	m_btnDeleteOp.ShowWindow( SW_HIDE );
	m_btnAddStock.ShowWindow( SW_HIDE );
	m_btnDeleteStock.ShowWindow( SW_HIDE );
	m_btnModified.ShowWindow( SW_HIDE );

	CString	strExplain, strExplainFmt;
	strExplainFmt.LoadString( IDS_SIMUREALOP_TIMEEXPLAINFMT );
	strExplain.Format( strExplainFmt, m_pStrategy->RealGetCurrentTime().Format("%Y-%m-%d") );
	m_staticTimeExplain.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
	m_staticTimeExplain.SetWindowText( strExplain );

	m_staticExplain.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
	strExplain.LoadString( IDS_SIMUREALOP_EXPLAINNEXTOP );
	m_staticExplain.SetWindowText( strExplain );

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSimuRealOp::OnSelchangeTabctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int	nCurSel	=	m_tabctrl.GetCurSel();
	CString	strExplain;
	switch( nCurSel )
	{
	case	0:
		m_listNextOp.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_listOpRecord.ShowWindow( SW_HIDE );
		m_curve.ShowWindow( SW_HIDE );
		m_listStockOwn.ShowWindow( SW_HIDE );
		m_btnCopy.EnableWindow( TRUE );
		m_btnAddOp.ShowWindow( SW_HIDE );
		m_btnDeleteOp.ShowWindow( SW_HIDE );
		m_btnAddStock.ShowWindow( SW_HIDE );
		m_btnDeleteStock.ShowWindow( SW_HIDE );
		m_btnModified.ShowWindow( SW_HIDE );
		m_staticTimeExplain.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_staticExplain.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		strExplain.LoadString( IDS_SIMUREALOP_EXPLAINNEXTOP );
		m_staticExplain.SetWindowText( strExplain );
		break;
	case	1:
		m_listNextOp.ShowWindow( SW_HIDE );
		m_listOpRecord.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_curve.ShowWindow( SW_HIDE );
		m_listStockOwn.ShowWindow( SW_HIDE );
		m_btnCopy.EnableWindow( TRUE );
		m_btnAddOp.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_btnDeleteOp.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_btnAddStock.ShowWindow( SW_HIDE );
		m_btnDeleteStock.ShowWindow( SW_HIDE );
		m_btnModified.ShowWindow( SW_HIDE );
		m_staticTimeExplain.ShowWindow( SW_HIDE );
		m_staticExplain.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		strExplain.LoadString( IDS_SIMUREALOP_EXPLAINOPRECORD );
		m_staticExplain.SetWindowText( strExplain );
		break;
	case	2:
		m_listNextOp.ShowWindow( SW_HIDE );
		m_listOpRecord.ShowWindow( SW_HIDE );
		m_curve.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_listStockOwn.ShowWindow( SW_HIDE );
		m_btnCopy.EnableWindow( FALSE );
		m_btnAddOp.ShowWindow( SW_HIDE );
		m_btnDeleteOp.ShowWindow( SW_HIDE );
		m_btnAddStock.ShowWindow( SW_HIDE );
		m_btnDeleteStock.ShowWindow( SW_HIDE );
		m_btnModified.ShowWindow( SW_HIDE );
		m_staticTimeExplain.ShowWindow( SW_HIDE );
		m_staticExplain.ShowWindow( SW_HIDE );
		break;
	case	3:
		m_listNextOp.ShowWindow( SW_HIDE );
		m_listOpRecord.ShowWindow( SW_HIDE );
		m_curve.ShowWindow( SW_HIDE );
		m_listStockOwn.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_btnCopy.EnableWindow( TRUE );
		m_btnAddOp.ShowWindow( SW_HIDE );
		m_btnDeleteOp.ShowWindow( SW_HIDE );
		m_btnAddStock.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_btnDeleteStock.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_btnModified.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW );
		m_staticTimeExplain.ShowWindow( SW_HIDE );
		m_staticExplain.ShowWindow( SW_HIDE );
		break;
	default:
		ASSERT( FALSE );
		break;
	}
	*pResult = 0;
}

void CSimuRealOp::OnCopy() 
{
	if( m_listNextOp.IsWindowVisible() )
	{
		m_listNextOp.Copy( );
	}
	if( m_listOpRecord.IsWindowVisible() )
	{
		m_listOpRecord.Copy( );
	}
	else if( m_listStockOwn.IsWindowVisible() )
	{
		m_listStockOwn.Copy( );
	}
}

/***
	显示结果统计信息
*/
void CSimuRealOp::OnStatyield() 
{
	// TODO: Add your control notification handler code here
	CSimuStatDlg	dlg;
	dlg.SetStrategy( m_pStrategy );
	dlg.DoModal();
}

/***
	手动加入操作
*/
void CSimuRealOp::OnAddop() 
{
	// TODO: Add your control notification handler code here
	if( m_pStrategy && m_listOpRecord.IsWindowVisible() )
	{
		CSimuAddOpDlg	dlg;
		dlg.SetStrategy( m_pStrategy );
		if( IDOK == dlg.DoModal() )
		{
			Initialize( );
			m_bModified	=	TRUE;
		}
	}
}

/***
	手动删除操作
*/
void CSimuRealOp::OnDeleteop() 
{
	// TODO: Add your control notification handler code here
	if( m_pStrategy && m_listOpRecord.IsWindowVisible() )
	{
		int	nSelected	=	m_listOpRecord.GetSelected();
		if( -1 == nSelected )
		{
			AfxMessageBox( IDS_SIMUREALOP_OPRECORDNOSEL, MB_OK | MB_ICONINFORMATION );
			return;
		}

		if( IDYES != AfxMessageBox( IDS_SIMUREALOP_DELETELATER, MB_YESNO ) )
			return;

		int	nRecord	=	m_listOpRecord.GetItemData( nSelected );
		if( !m_pStrategy->RealDeleteOpRecordStrict( nRecord ) )
		{
			AfxMessageBox( IDS_SIMUREALOP_CANNOTDELETE, MB_OK | MB_ICONINFORMATION );
			return;
		}
		Initialize( );
		m_bModified	=	TRUE;
	}
}

/***
	手动加入拥有股票
*/
void CSimuRealOp::OnAddstock() 
{
	CSimuAddStockDlg	dlg;
	dlg.SetStrategy( m_pStrategy );
	dlg.SetMode( CSimuAddStockDlg::modeAddStock, "", 0, 0 );
	if( IDOK == dlg.DoModal( ) )
	{
		CString	strStockCode	=	dlg.GetStockCode();
		double	dShare			=	dlg.GetValue();
		double	dSharePrice		=	dlg.GetPrice();
		
		OPRECORD	record;
		memset( &record, 0, sizeof(record) );
		CSPTime	tmNow	=	CSPTime::GetCurrentTime();
		CSPTime	tmDay	=	CSPTime( tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), 0, 0, 0 );
		record.time		=	tmDay.GetTime();
		record.lOpType	=	STRATEGY_OPTYPE_ADDSTOCK;
		strncpy( record.szCode, strStockCode.GetBuffer(sizeof(record.szCode)+1), sizeof(record.szCode) );
		strStockCode.ReleaseBuffer();
		record.dwShare	=	(DWORD)dShare;
		record.dSharePrice	=	dSharePrice;
		if( m_pStrategy->RealOperate( record ) )
			Initialize( );
		else
			AfxMessageBox( IDS_SIMUREALOP_ADDSTOCKFAILED, MB_OK|MB_ICONINFORMATION );
		m_bModified	=	TRUE;
	}
}

/***
	手动移除拥有股票
*/
void CSimuRealOp::OnDeletestock() 
{
	// TODO: Add your control notification handler code here
	if( m_pStrategy && m_listStockOwn.IsWindowVisible() )
	{
		int	nSelected	=	m_listStockOwn.GetSelected();
		if( -1 == nSelected )
		{
			AfxMessageBox( IDS_SIMUREALOP_OPRECORDNOSEL, MB_OK | MB_ICONINFORMATION );
			return;
		}

		int	nStock	=	m_listStockOwn.GetItemData( nSelected );
		CStockOwnContainer	& stockown	=	m_pStrategy->RealGetStockOwn();
		BOOL	bOK	=	FALSE;
		if( nStock >= 0 && nStock < stockown.GetSize() )
		{
			OPRECORD	record;
			memset( &record, 0, sizeof(record) );
			CSPTime	tmNow	=	CSPTime::GetCurrentTime();
			CSPTime	tmDay	=	CSPTime( tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), 0, 0, 0 );
			record.time		=	tmDay.GetTime();
			record.lOpType	=	STRATEGY_OPTYPE_REMOVESTOCK;
			strncpy( record.szCode, stockown.ElementAt(nStock).szCode, sizeof(record.szCode) );
			record.dwShare	=	stockown.ElementAt(nStock).dwShare;
			bOK		=	m_pStrategy->RealOperate( record );
			Initialize( );
			m_bModified	=	TRUE;
		}
		
		if( !bOK )
			AfxMessageBox( IDS_SIMUREALOP_CANNOTDELETE, MB_OK | MB_ICONINFORMATION );
	}
}

/***
	手动改变拥有股票数量
*/
void CSimuRealOp::OnModified()
{
	// TODO: Add your control notification handler code here
	CSimuAddStockDlg	dlg;
	dlg.SetStrategy( m_pStrategy );
	dlg.SetMode( CSimuAddStockDlg::modeModifiedCash, "", m_pStrategy->RealGetCurrentCash(), 0 );
	if( IDOK == dlg.DoModal( ) )
	{
		double	dValue		=	dlg.GetValue();
		double	dValueOld	=	m_pStrategy->RealGetCurrentCash();
		
		OPRECORD	record;
		memset( &record, 0, sizeof(record) );
		CSPTime	tmNow	=	CSPTime::GetCurrentTime();
		CSPTime	tmDay	=	CSPTime( tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), 0, 0, 0 );
		record.time		=	tmDay.GetTime();
		if( dValue > dValueOld )
			record.lOpType	=	STRATEGY_OPTYPE_ADDCASH;
		else
			record.lOpType	=	STRATEGY_OPTYPE_REMOVECASH;
		record.dSharePrice	=	fabs(dValue-dValueOld);
		if( m_pStrategy->RealOperate( record ) )
			Initialize( );
		else
			AfxMessageBox( IDS_SIMUREALOP_MODIFIEDCASHFAILED, MB_OK|MB_ICONINFORMATION );
		m_bModified	=	TRUE;
	}
}

/***
	策略实战选股进度回调函数
*/
BOOL CALLBACK RealRunCallback(DWORD dwType, DWORD dwValue,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND hWnd = (HWND)cookie;

	if( SIMULATION_PROGRESS == dwType )
	{
		::SendMessage( hWnd, WM_USER_SIMULATION_PROGRESS, dwValue, (LPARAM)lpszMsg );
	}

	return TRUE;
}

/***
	全部重来
*/
void CSimuRealOp::OnResetall() 
{
	// TODO: Add your control notification handler code here
	if( m_pStrategy && IDYES == AfxMessageBox(IDS_SIMUREALOP_RESETCONFIRM,MB_YESNO) )
	{
		CWaitDlg	wait( AfxGetMainWnd(), FALSE );
		wait.SetProgressRange( 0, STRATEGY_MAX_PROGRESS );

		m_pStrategy->RealReset( );
		m_pStrategy->RealRun( RealRunCallback, wait.GetSafeHwnd() );

		wait.DestroyWindow();

		Initialize( );
	}
}
