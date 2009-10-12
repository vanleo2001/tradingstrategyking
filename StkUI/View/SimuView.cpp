// SimuView.cpp : implementation of the CSimuView class
//

#include "stdafx.h"

#include "../StaticDoc.h"
#include "SimuView.h"
#include "Simulation.h"
#include "../ParamDlg/SetParamDlg.h"
#include "../Dialog/SimuAdvancedDlg.h"
#include "../Dialog/SimuRealOp.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuView

#define		SIMUVIEW_TIMER_WARNING	1

IMPLEMENT_DYNCREATE(CSimuView, CFormView)

BEGIN_MESSAGE_MAP(CSimuView, CFormView)
	//{{AFX_MSG_MAP(CSimuView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SETPRPT, OnSetprpt)
	ON_BN_CLICKED(IDC_SELECTSTK, OnSelectstk)
	ON_BN_CLICKED(IDC_SETRULE, OnSetrule)
	ON_BN_CLICKED(IDC_SETRATE, OnSetrate)
	ON_BN_CLICKED(IDC_REPORT, OnReport)
	ON_BN_CLICKED(IDC_REALOP, OnRealOp)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_NOTIFY(DTN_CLOSEUP, IDC_TIME_BEGIN, OnCloseupTimeBegin)
	ON_NOTIFY(DTN_CLOSEUP, IDC_TIME_END, OnCloseupTimeEnd)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ADVANCED, OnAdvanced)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
	ON_MESSAGE(WM_USER_CANCLOSEVIEW, OnCanCloseView)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
	ON_MESSAGE(WM_USER_SIMULATION_YIELD, OnSimulationYield)
	ON_MESSAGE(WM_USER_SIMULATION_PROGRESS, OnSimulationProgress)
	ON_MESSAGE(WM_USER_SIMULATION_END, OnSimulationEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuView construction/destruction

CSimuView::CSimuView()
	: CFormView(CSimuView::IDD)
{
	//{{AFX_DATA_INIT(CSimuView)
	m_strBuyMulti = _T("");
	m_tmBegin = 0;
	m_tmEnd = 0;
	m_nStartAmount = 0;
	m_strSellMulti = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pStrategy		=	NULL;
	m_nStartAmount	=	100;
	m_bDisableAllControls	=	FALSE;
}

CSimuView::~CSimuView()
{
}

/***
	初始化主数据类型ComboBox
*/
BOOL CSimuView::InitComboMaindataType( )
{
	m_comboMaindataType.ResetContent();
	
	int	mdtype, nIndex, nSel = CB_ERR;
	for( mdtype = CKData::mdtypeMin; mdtype <= CKData::mdtypeMax; mdtype ++ )
	{
		nIndex	=	m_comboMaindataType.AddString( AfxGetMaindataTypeString(mdtype) );
		m_comboMaindataType.SetItemData( nIndex, mdtype );
		if( CKData::mdtypeClose == mdtype )
			nSel	=	nIndex;
	}

	m_comboMaindataType.SetCurSel( nSel );
	return TRUE;
}

/***
	初始化买入逻辑选择ComboBox
*/
BOOL CSimuView::InitComboBuyLogic( )
{
	m_comboBuyLogic.ResetContent();

	int	nLogic, nIndex, nSel = CB_ERR;
	for( nLogic = COpParam::logicMin; nLogic <= COpParam::logicMax; nLogic ++ )
	{
		nIndex	=	m_comboBuyLogic.AddString( AfxGetLogicString(nLogic) );
		m_comboBuyLogic.SetItemData( nIndex, nLogic );
		if( COpParam::logicAnd == nLogic )
			nSel	=	nIndex;
	}

	m_comboBuyLogic.SetCurSel( nSel );
	return TRUE;
}

/***
	初始化买入信号ComboBox
*/
BOOL CSimuView::InitComboBuyLimit( )
{
	return ( m_comboBuyLimit.Initialize( )
		&& m_comboBuyLimit.Select( ITSG_BUY ) );
}

/***
	初始化卖出逻辑ComboBox
*/
BOOL CSimuView::InitComboSellLogic( )
{
	m_comboSellLogic.ResetContent();

	int	nLogic, nIndex, nSel = CB_ERR;
	for( nLogic = COpParam::logicMin; nLogic <= COpParam::logicMax; nLogic ++ )
	{
		nIndex	=	m_comboSellLogic.AddString( AfxGetLogicString(nLogic) );
		m_comboSellLogic.SetItemData( nIndex, nLogic );
		if( COpParam::logicOr == nLogic )
			nSel	=	nIndex;
	}

	m_comboSellLogic.SetCurSel( nSel );
	return TRUE;
}

/***
	初始化卖出信号ComboBox
*/
BOOL CSimuView::InitComboSellLimit( )
{
	return ( m_comboSellLimit.Initialize()
		&& m_comboSellLimit.Select( ITSG_SELL ) );
}

/***
	初始化操作仓位ComboBox
*/
BOOL CSimuView::InitComboStoreDiv( )
{
	m_comboStoreDiv.ResetContent();

	int	nStoreDiv, nIndex, nSel = CB_ERR;
	for( nStoreDiv = COpParam::storedivMin; nStoreDiv <= COpParam::storedivMax; nStoreDiv ++ )
	{
		nIndex	=	m_comboStoreDiv.AddString( AfxGetStoreDivString(nStoreDiv) );
		m_comboStoreDiv.SetItemData( nIndex, nStoreDiv );
		if( COpParam::storedivThird == nStoreDiv )
			nSel	=	nIndex;
	}

	m_comboStoreDiv.SetCurSel( nSel );
	return TRUE;
}

/***
	根据当前状态，使不能修改的控件变灰
*/
BOOL CSimuView::EnableControls( )
{
	if( !::IsWindow(m_btnStart.GetSafeHwnd())
		|| !::IsWindow(m_btnSetPrpt.GetSafeHwnd())
		|| !::IsWindow(m_btnSelectStk.GetSafeHwnd())
		|| !::IsWindow(m_btnSetRate.GetSafeHwnd())
		|| !::IsWindow(m_btnSetRule.GetSafeHwnd())
		|| !::IsWindow(m_btnReport.GetSafeHwnd())
		|| !::IsWindow(m_btnRealOp.GetSafeHwnd()) )
		return FALSE;

	CStrategy	*	pStrategy	=	GetStrategy( );

	BOOL	bEnable	=	TRUE;

	CString	strBtnStart;
	if( NULL == pStrategy )
	{
		bEnable	=	FALSE;

		strBtnStart.LoadString( IDS_SIMUVIEW_START );
		m_btnStart.SetWindowText( strBtnStart );
		m_btnStart.EnableWindow( FALSE );
		m_btnStop.EnableWindow( FALSE );
		m_staticYield.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_staticYield2.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_progress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		CString	strWarning;
		strWarning.LoadString( IDS_SIMUVIEW_MSGNOSTRATEGY );
		m_staticWarning.SetWindowText( strWarning );
	}
	else if( pStrategy->SimuIsStatusInit() )
	{
		bEnable	=	TRUE;

		strBtnStart.LoadString( IDS_SIMUVIEW_START );
		m_btnStart.SetWindowText( strBtnStart );
		m_btnStart.EnableWindow( TRUE );
		m_btnStop.EnableWindow( FALSE );
		m_staticYield.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_staticYield2.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_progress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	}
	else if( pStrategy->SimuIsStatusRunning() )
	{
		bEnable	=	FALSE;

		strBtnStart.LoadString( IDS_SIMUVIEW_PAUSE );
		m_btnStart.SetWindowText( strBtnStart );
		m_btnStart.EnableWindow( TRUE );
		m_btnStop.EnableWindow( TRUE );
		m_staticYield.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_staticYield2.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_progress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	}
	else if( pStrategy->SimuIsStatusPaused() )
	{
		bEnable	=	FALSE;

		strBtnStart.LoadString( IDS_SIMUVIEW_CONTINUE );
		m_btnStart.SetWindowText( strBtnStart );
		m_btnStart.EnableWindow( TRUE );
		m_btnStop.EnableWindow( TRUE );
		m_staticYield.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_staticYield2.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_progress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	}
	else
	{
		bEnable	=	TRUE;
		strBtnStart.LoadString( IDS_SIMUVIEW_START );
		m_btnStart.SetWindowText( strBtnStart );
		m_btnStart.EnableWindow( TRUE );
		m_btnStop.EnableWindow( FALSE );
		m_staticYield.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_staticYield2.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		m_progress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	}

	if( m_bDisableAllControls )
		bEnable	=	FALSE;

	m_btnSetPrpt.EnableWindow( bEnable );
	m_btnSelectStk.EnableWindow( bEnable );
	m_btnSetRate.EnableWindow( bEnable );
	m_btnSetRule.EnableWindow( bEnable );
	m_btnReport.EnableWindow( bEnable );
	m_btnRealOp.EnableWindow( bEnable );
	m_comboKType.EnableWindow( bEnable );
	m_comboMaindataType.EnableWindow( bEnable );
	m_comboBuyLogic.EnableWindow( bEnable );
	m_comboBuyLimit.EnableWindow( bEnable );
	m_comboSellLogic.EnableWindow( bEnable );
	m_comboSellLimit.EnableWindow( bEnable );
	m_comboStoreDiv.EnableWindow( bEnable );
	m_btnAdvanced.EnableWindow( bEnable );
	m_editBuyMulti.EnableWindow( bEnable );
	m_editSellMulti.EnableWindow( bEnable );
	m_editStartAmount.EnableWindow( bEnable );
	m_tmctrlBegin.EnableWindow( bEnable );
	m_tmctrlEnd.EnableWindow( bEnable );

	if( !m_bDisableAllControls && pStrategy && pStrategy->SimuIsStatusPaused() )
	{
		m_btnReport.EnableWindow( TRUE );
		m_btnRealOp.EnableWindow( TRUE );
	}

	if( m_bDisableAllControls )
	{
		m_btnStart.EnableWindow( FALSE );
		m_btnStop.EnableWindow( FALSE );
	}

	return TRUE;
}

/***
	重新初始化各个控件的初始选择
*/
BOOL CSimuView::ClearData( )
{
	// KType
	m_comboKType.InitializeDay( );

	// Maindata Type
	InitComboMaindataType( );

	// BuyLogic
	InitComboBuyLogic( );

	// Buy Rule
	InitComboBuyLimit( );

	// SellLogic
	InitComboSellLogic( );

	// Sell Rule
	InitComboSellLimit( );

	// StoreDiv
	InitComboStoreDiv( );

	m_editStockTech.SetWindowText( NULL );

	m_progress.SetRange( 0, STRATEGY_MAX_PROGRESS );
	m_progress.SetPos( 0 );

	return TRUE;
}

/***
	通用函数，选择ComboBox的相应项
*/
BOOL CSimuView::SelectComboItem( CComboBox & combo, DWORD dwData )
{
	for( int i=0; i<combo.GetCount(); i++ )
	{
		if( dwData == combo.GetItemData(i) )
		{
			combo.SetCurSel(i);
			return TRUE;
		}
	}
	return FALSE;
}

CStrategy * CSimuView::GetStrategy( )
{
	return m_pStrategy;
}

/***
	是否能改变当前活动策略
*/
BOOL CSimuView::CanChangeActiveStrategy( )
{
	CStrategy * pStrategy = GetStrategy();
	if( pStrategy
		&& pStrategy->SimuIsStatusRunning()
		&& IDNO == AfxMessageBox( IDS_SIMUVIEW_RUNCANNOTCLOSE, MB_YESNO ) )
	{
		return FALSE;
	}

	if( pStrategy && pStrategy->SimuIsStatusRunning() )
		m_simulation.Pause();

	return StoreToStrategy( );
}

/***
	当前活动策略换成了其他策略时，调用此函数
*/
void CSimuView::OnActiveStrategyChanged( )
{
	if( m_pStrategy )
	{
		m_pStrategy->ClearCache();
		StoreToStrategy( );
		m_pStrategy	=	NULL;
	}

	m_pStrategy	=	AfxGetActiveStrategy( );

	OnUpdate( NULL, UPDATE_HINT_SIMUVIEW_REREALRUN, NULL );
}

/***
	将策略显示到策略视图中
*/
BOOL CSimuView::LoadFromStrategy( )
{
	ClearData( );

	CStrategy	*	pStrategy	=	GetStrategy( );

	// Set Title
	CString	strTitle;
	strTitle.LoadString( IDS_SIMUVIEW_SIMU );
	if( pStrategy )
		strTitle	=	CString("[") + (LPCTSTR)pStrategy->GetName() + "]" + strTitle;
	m_staticTitle.SetWindowText( strTitle );
	m_staticWarning.SetWindowText( "" );

	if( NULL == pStrategy )
		return FALSE;

	m_editStockTech.SetWindowText( pStrategy->GetStockTechString( ) );

	COpParam & opparam = pStrategy->GetOpParam( );
	if( !opparam.IsValid() )
		opparam.SetDefaultOfInvalidMember();

	// KType
	if( !m_comboKType.Select( opparam.m_nKType ) )
		m_comboKType.InitializeDay();

	// MaindataType
	if( !SelectComboItem( m_comboMaindataType, opparam.m_nMaindataType ) )
		InitComboMaindataType( );

	// BuyLogic
	if( !SelectComboItem( m_comboBuyLogic, opparam.m_nBuyLogic ) )
		InitComboBuyLogic( );

	// BuyLimit
	if( !SelectComboItem( m_comboBuyLimit, opparam.m_nBuyLimit ) )
		InitComboBuyLimit( );

	// SellLogic
	if( !SelectComboItem( m_comboSellLogic, opparam.m_nSellLogic ) )
		InitComboSellLogic( );

	// SellLimit
	if( !SelectComboItem( m_comboSellLimit, opparam.m_nSellLimit ) )
		InitComboSellLimit( );

	// StoreDiv
	if( !SelectComboItem( m_comboStoreDiv, opparam.m_nStoreDiv ) )
		InitComboStoreDiv( );

	// Buy and Sell Multi
	m_strBuyMulti.Format( "%.2f", opparam.m_dBuyMulti * 100. );
	m_strSellMulti.Format( "%.2f", opparam.m_dSellMulti * 100 );

	// Start Amount
	m_nStartAmount	=	opparam.m_nStartAmount/10000;

	// Time
	CSPTime	tmCurrent	=	CSPTime::GetCurrentTime();
	m_tmBegin	=	CSPTime(tmCurrent.GetYear(),tmCurrent.GetMonth(),tmCurrent.GetDay(),0,0,0,0);
	m_tmBegin	=	m_tmBegin - CSPTimeSpan( 365 * 3, 0, 0, 0 );
	m_tmEnd		=	CSPTime(tmCurrent.GetYear(),tmCurrent.GetMonth(),tmCurrent.GetDay(),23,59,59,0);
	m_tmEnd		=	m_tmEnd - CSPTimeSpan( 1, 0, 0, 0 );
	if( opparam.m_atmBegin.GetSize() > 0 )
		m_tmBegin	=	opparam.m_atmBegin.ElementAt(0).GetTime();
	if( opparam.m_atmEnd.GetSize() > 0 )
		m_tmEnd		=	opparam.m_atmEnd.ElementAt(0).GetTime();

	// TIMEZONES

	// Simulation
	m_simulation.SetStrategy( pStrategy, GetSafeHwnd() );

	SendMessage( WM_USER_SIMULATION_PROGRESS, pStrategy->SimuGetCurrentProgress(STRATEGY_MAX_PROGRESS), NULL );
	SendMessage( WM_USER_SIMULATION_YIELD, NULL, (LPARAM)(pStrategy->SimuGetCurrentYield()) );

	UpdateData( FALSE );
	EnableControls( );
	return TRUE;
}

/***
	将策略视图中的策略选项保存到策略文件中
*/
BOOL CSimuView::StoreToStrategy( )
{
	UpdateData( );

	CStrategy	*	pStrategy	=	GetStrategy( );
	if( NULL == pStrategy )
		return TRUE;

	COpParam & opparam = pStrategy->GetOpParam( );
	if( !opparam.IsValid() )
		opparam.SetDefaultOfInvalidMember();

	// KType
	opparam.m_nKType		=	m_comboKType.GetSelect();

	// MaindataType
	opparam.m_nMaindataType	=	m_comboMaindataType.GetItemData(m_comboMaindataType.GetCurSel());

	// BuyLogic
	opparam.m_nBuyLogic		=	m_comboBuyLogic.GetItemData(m_comboBuyLogic.GetCurSel());

	// BuyLimit
	opparam.m_nBuyLimit		=	m_comboBuyLimit.GetItemData(m_comboBuyLimit.GetCurSel());

	// SellLogic
	opparam.m_nSellLogic	=	m_comboSellLogic.GetItemData(m_comboSellLogic.GetCurSel());

	// SellLimit
	opparam.m_nSellLimit	=	m_comboSellLimit.GetItemData(m_comboSellLimit.GetCurSel());

	// StoreDiv
	opparam.m_nStoreDiv		=	m_comboStoreDiv.GetItemData(m_comboStoreDiv.GetCurSel());

	// Buy and Sell Multi
	if( IsNumber(m_strBuyMulti,FALSE) )
		opparam.m_dBuyMulti		=	atof(m_strBuyMulti) / 100.;
	if( IsNumber(m_strSellMulti,FALSE) )
		opparam.m_dSellMulti	=	atof(m_strSellMulti) / 100.;

	// Start Amount
	opparam.m_nStartAmount	=	m_nStartAmount*10000;

	// Time
	m_tmBegin	=	CSPTime(m_tmBegin.GetYear(),m_tmBegin.GetMonth(),m_tmBegin.GetDay(),0,0,0,0);
	m_tmEnd		=	CSPTime(m_tmEnd.GetYear(),m_tmEnd.GetMonth(),m_tmEnd.GetDay(),23,59,59,0);
	opparam.m_atmBegin.RemoveAll();
	opparam.m_atmEnd.RemoveAll();
	opparam.m_atmBegin.Add( CSPTime(m_tmBegin.GetTime()) );
	opparam.m_atmEnd.Add( CSPTime(m_tmEnd.GetTime()) );

	// TIMEZONES

	if( !opparam.IsValid() || !IsNumber(m_strBuyMulti,FALSE) || !IsNumber(m_strSellMulti,FALSE) )
	{
		CString	strMsg;
		strMsg.LoadString( IDS_SIMUVIEW_INVALIDDATA );
		strMsg	=	(LPCTSTR)pStrategy->GetName() + CString("  ") + strMsg;
		if( IDYES == AfxMessageBox( strMsg, MB_YESNO ) )
		{
			if( !IsNumber(m_strBuyMulti,FALSE) )
				opparam.m_dBuyMulti	=	0;
			if( !IsNumber(m_strSellMulti,FALSE) )
				opparam.m_dSellMulti	=	0;
			opparam.SetDefaultOfInvalidMember( );
			pStrategy->DoFileSave( );
			LoadFromStrategy( );
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		pStrategy->DoFileSave( );
		return TRUE;
	}
}

void CSimuView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuView)
	DDX_Control(pDX, IDC_SELLMULTI, m_editSellMulti);
	DDX_Control(pDX, IDC_ADVANCED, m_btnAdvanced);
	DDX_Control(pDX, IDC_STOCKTECH, m_editStockTech);
	DDX_Control(pDX, IDC_SELLLOGIC, m_comboSellLogic);
	DDX_Control(pDX, IDC_BUYLOGIC, m_comboBuyLogic);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_STARTAMOUNT, m_editStartAmount);
	DDX_Control(pDX, IDC_STATICYIELD2, m_staticYield2);
	DDX_Control(pDX, IDC_STATICYIELD, m_staticYield);
	DDX_Control(pDX, IDC_TIME_BEGIN, m_tmctrlBegin);
	DDX_Control(pDX, IDC_TIME_END, m_tmctrlEnd);
	DDX_Control(pDX, IDC_STOREDIV, m_comboStoreDiv);
	DDX_Control(pDX, IDC_SELLLIMIT, m_comboSellLimit);
	DDX_Control(pDX, IDC_BUYMULTI, m_editBuyMulti);
	DDX_Control(pDX, IDC_BUYLIMIT, m_comboBuyLimit);
	DDX_Control(pDX, IDC_MAINDATATYPE, m_comboMaindataType);
	DDX_Control(pDX, IDC_STATICWARNING, m_staticWarning);
	DDX_Control(pDX, IDC_STATICTITLE, m_staticTitle);
	DDX_Control(pDX, IDC_REALOP, m_btnRealOp);
	DDX_Control(pDX, IDC_SETRULE, m_btnSetRule);
	DDX_Control(pDX, IDC_SETRATE, m_btnSetRate);
	DDX_Control(pDX, IDC_SETPRPT, m_btnSetPrpt);
	DDX_Control(pDX, IDC_REPORT, m_btnReport);
	DDX_Control(pDX, IDC_SELECTSTK, m_btnSelectStk);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_KTYPE, m_comboKType);
	DDX_Text(pDX, IDC_BUYMULTI, m_strBuyMulti);
	DDX_DateTimeCtrl(pDX, IDC_TIME_BEGIN, m_tmBegin);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_tmEnd);
	DDX_Text(pDX, IDC_STARTAMOUNT, m_nStartAmount);
	DDX_Text(pDX, IDC_SELLMULTI, m_strSellMulti);
	//}}AFX_DATA_MAP
}

BOOL CSimuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CSimuView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// GetParentFrame()->RecalcLayout();
	// ResizeParentToFit();

	// TODO: Add extra initialization here
	OnActiveStrategyChanged( );
}

void CSimuView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( UPDATE_HINT_SIMUVIEW != lHint && UPDATE_HINT_SIMUVIEW_REREALRUN != lHint )
		return;
	if( IsWindowVisible() )
		SetFocus( );

	LoadFromStrategy( );

	if( m_pStrategy )
	{
		if( UPDATE_HINT_SIMUVIEW_REREALRUN == lHint )
		{
			CWaitDlg	wait( AfxGetMainWnd(), FALSE );
			wait.SetProgressRange( 0, STRATEGY_MAX_PROGRESS );

			m_pStrategy->ClearCache();
			m_pStrategy->RealRun( RealRunCallback, wait.GetSafeHwnd() );

			wait.DestroyWindow();
		}

		if( m_pStrategy->RealGetNextOp().GetSize() > 0 )
		{
			CString	strWarning;
			strWarning.LoadString( IDS_SIMUVIEW_REALHASNEXTOP );
			m_staticWarning.SetWindowText( strWarning );
		}
		else
			m_staticWarning.SetWindowText( "" );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSimuView diagnostics

#ifdef _DEBUG
void CSimuView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSimuView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSimuView message handlers

int CSimuView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTimer( SIMUVIEW_TIMER_WARNING, 250, NULL );
	return 0;
}

void CSimuView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( SIMUVIEW_TIMER_WARNING == nIDEvent )
	{
		// 显示当前实战操作是否有操作
		static int timercount	=	0;
		timercount	++;
		if( timercount % 4 == 0 )
		{
			m_staticWarning.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		}
		else
		{
			m_staticWarning.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
		}
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CSimuView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if( bActivate )
		EnableControls( );
}

void CSimuView::OnSize(UINT nType, int cx, int cy) 
{
	// CSize	sizeTotal( cx, cy );
	// SetScaleToFitSize( sizeTotal );

	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

void CSimuView::OnSetFocus(CWnd* pOldWnd) 
{
	CFormView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
}

LRESULT CSimuView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString( IDS_TITLE_SIMUVIEW );

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

LRESULT CSimuView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((LPARAM *)lParam)	=	ID_VIEW_SIMU;
	return 0L;
}

LRESULT CSimuView::OnCanCloseView(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((BOOL *)lParam)	=	CanChangeActiveStrategy( );

	return 0L;
}

LRESULT CSimuView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	Invalidate( );
	return 0L;
}

HBRUSH CSimuView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( pDC )
		pDC->SetTextColor( AfxGetProfile().GetColor( CColorClass::clrSimuText ) );
	if( nCtlColor == CTLCOLOR_DLG
		|| nCtlColor == CTLCOLOR_STATIC
		|| nCtlColor == CTLCOLOR_MSGBOX )
	{
		if( nCtlColor == CTLCOLOR_STATIC )
			pDC->SetBkMode( TRANSPARENT );

		static CBrush brushBK;
		static COLORREF	clrBKLast	=	RGB(255,255,255);
		COLORREF	clrBKNow	=	AfxGetProfile().GetColor( CColorClass::clrSimuBK );
		if( NULL == brushBK.GetSafeHandle() || clrBKLast != clrBKNow )
		{
			brushBK.DeleteObject( );
			VERIFY( brushBK.CreateSolidBrush( clrBKNow ) );
		}
		clrBKLast	=	clrBKNow;
		return (HBRUSH)brushBK.GetSafeHandle();
	}
	return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}

/***
	设定策略属性
*/
void CSimuView::OnSetprpt() 
{
	if( !StoreToStrategy( ) )
		return;

	::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_PROPERTY, 0 );
}

/***
	选择策略备选股票
*/
void CSimuView::OnSelectstk() 
{
	if( !StoreToStrategy( ) )
		return;

	::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_STOCKS, 0 );
}

/***
	设定策略指标
*/
void CSimuView::OnSetrule() 
{
	if( !StoreToStrategy( ) )
		return;

	::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_SETRULE, 0 );
}

/***
	设定交易费用
*/
void CSimuView::OnSetrate() 
{
	if( !StoreToStrategy( ) )
		return;

	::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_SETRATE, 0 );
}

/***
	显示策略模拟结果报表
*/
void CSimuView::OnReport() 
{
	if( !StoreToStrategy( ) )
		return;

	::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_REPORT, 0 );
}

/***
	显示策略实战操作
*/
void CSimuView::OnRealOp() 
{
	if( !StoreToStrategy( ) )
		return;

	::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STRATEGY_REALOP, 0 );
}

/***
	策略高级条件
*/
void CSimuView::OnAdvanced() 
{
	if( !StoreToStrategy( ) )
		return;

	CStrategy	*	pStrategy	=	GetStrategy( );
	if( pStrategy )
	{
		CSimuAdvancedDlg	advanced;
		advanced.SetStrategy( pStrategy );
		advanced.DoModal();
		pStrategy->DoFileSave( );
		AfxOnStrategyUpdated( pStrategy, advanced.m_bModified );
	}
}

/***
	开始模拟或者暂停模拟
*/
void CSimuView::OnStart() 
{
	if( !StoreToStrategy( ) )
		return;

	CStrategy	*	pStrategy	=	GetStrategy( );
	if( NULL == pStrategy )
		return;
	pStrategy->DoFileSave();

	m_bDisableAllControls	=	TRUE;
	EnableControls( );

	if( pStrategy->SimuIsStatusPaused() )
	{
		m_simulation.Continue( );
	}
	else if( pStrategy->SimuIsStatusRunning() )
	{
		m_simulation.Pause( );
	}
	else
	{
		m_simulation.Restart( );
	}
	
	m_bDisableAllControls	=	FALSE;
	EnableControls( );
}

/***
	停止并清楚结果
*/
void CSimuView::OnStop() 
{
	// TODO: Add your control notification handler code here
	m_bDisableAllControls	=	TRUE;
	EnableControls( );
	m_simulation.Stop( );
	m_bDisableAllControls	=	FALSE;
	EnableControls( );
}

/***
	策略模拟进度消息响应
*/
LRESULT CSimuView::OnSimulationProgress(WPARAM wParam, LPARAM lParam)
{
	if( wParam < 0 )
		wParam	=	0;
	if( wParam > STRATEGY_MAX_PROGRESS )
		wParam = STRATEGY_MAX_PROGRESS;
	if( ::IsWindow( m_progress.GetSafeHwnd() ) )
		m_progress.SetPos( wParam );

	return 1;
}

/***
	策略模拟收益消息响应
*/
LRESULT CSimuView::OnSimulationYield(WPARAM wParam, LPARAM lParam)
{
	if( lParam < 0 )
		lParam	=	0;
	if( !::IsWindow( m_staticYield2.GetSafeHwnd() ) )
		return 1;

	CString	strYieldNew;
	strYieldNew.Format( "%.2f%%", 100.*(lParam-STRATEGY_BASEF_YIELD)/STRATEGY_BASEF_YIELD );
	CString	strYieldOld;
	m_staticYield2.GetWindowText( strYieldOld );
	if( 0 != strYieldNew.Compare( strYieldOld ) )
		m_staticYield2.SetWindowText( strYieldNew );
	return 1;
}

/***
	策略模拟结束消息响应
*/
LRESULT CSimuView::OnSimulationEnd(WPARAM wParam, LPARAM lParam)
{
	BOOL	bFinished	=	wParam;
	m_simulation.OnEnd( bFinished );

	if( bFinished )
	{
		if( ::IsWindow( m_progress.GetSafeHwnd() ) )
			m_progress.SetPos( STRATEGY_MAX_PROGRESS );
	}

	EnableControls( );
	return 1;
}

/***
	策略模拟开始时间选定消息响应
*/
void CSimuView::OnCloseupTimeBegin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CSPTime	tmOld	=	m_tmBegin;
	UpdateData( );

	if( m_tmBegin >= m_tmEnd )
	{
		AfxMessageBox( IDS_SIMUVIEW_TIMEINVALID, MB_OK|MB_ICONINFORMATION );
		m_tmBegin	=	tmOld;
	}
	if( m_tmBegin >= m_tmEnd )
		m_tmBegin	=	m_tmEnd;

	UpdateData( FALSE );
	
	*pResult = 0;
}

/***
	策略模拟结束时间选定消息响应
*/
void CSimuView::OnCloseupTimeEnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CSPTime	tmOld	=	m_tmEnd;
	UpdateData( );

	if( m_tmEnd < m_tmBegin )
	{
		AfxMessageBox( IDS_SIMUVIEW_TIMEINVALID, MB_OK|MB_ICONINFORMATION );
		m_tmEnd	=	tmOld;
	}
	if( m_tmEnd < m_tmBegin )
		m_tmEnd	=	m_tmBegin;

	UpdateData( FALSE );

	*pResult = 0;
}

