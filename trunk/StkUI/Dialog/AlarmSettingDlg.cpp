// AlarmSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\stkui.h"
#include "AlarmSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmSettingDlg dialog


CAlarmSettingDlg::CAlarmSettingDlg() : CPropertyPageEx(CAlarmSettingDlg::IDD)
{
	//{{AFX_DATA_INIT(CAlarmSettingDlg)
	m_strDiffPercentMore = _T("");
	m_strDiffPercentLess = _T("");
	m_strTradeVolume = _T("");
	//}}AFX_DATA_INIT
	m_strDiffPercentMore = _T("4");
	m_strDiffPercentLess = _T("4");
	m_strTradeVolume = _T("1000");
}

CAlarmSettingDlg::~CAlarmSettingDlg()
{
}

void CAlarmSettingDlg::ResetListCond( )
{
	m_listCond.ResetContent();
	CAlarmCondContainer & container = AfxGetProfile().GetAlarmCondContainer();
	for( int nIndex=0; nIndex<container.GetSize(); nIndex++ )
	{
		ALARMCOND & cond = container.ElementAt(nIndex);
		CString strCond = container.AlarmCondToString( cond );
		int nItem = m_listCond.AddString( strCond );
		m_listCond.SetItemData( nItem, nIndex );
	}
}

BOOL CAlarmSettingDlg::GetCurrentCond( ALARMCOND & cond )
{
	memset( &cond, 0, sizeof(cond) );

	CString	strCode	=	m_cmbStock.GetSelectedStockCode();
	CStockInfo info;
	if( AfxGetStockContainer().GetStockInfo(strCode,&info) )
		cond.m_dwMarket = info.GetMarket();
	memcpy( cond.m_szCode, strCode, min(sizeof(cond.m_szCode)-1,strCode.GetLength()) );

	CButton *pbtnMore = (CButton*)GetDlgItem(IDC_RADIO_DIFFPERCENTMORE);
	CButton *pbtnLess = (CButton*)GetDlgItem(IDC_RADIO_DIFFPERCENTLESS);
	if( pbtnMore && pbtnMore->GetCheck() )
	{
		cond.m_type = ALARM_TYPE_DIFFPERCENTMORE;
		if( !IsNumber(m_strDiffPercentMore,FALSE) )
		{
			AfxMessageBox( IDS_ALARMSETTING_NOTNUMBER );
			return FALSE;
		}
		cond.m_fValue = atof(m_strDiffPercentMore);
	}
	else if( pbtnLess && pbtnLess->GetCheck() )
	{
		cond.m_type = ALARM_TYPE_DIFFPERCENTLESS;
		cond.m_fValue = atof(m_strDiffPercentLess);
		if( !IsNumber(m_strDiffPercentLess,FALSE) )
		{
			AfxMessageBox( IDS_ALARMSETTING_NOTNUMBER );
			return FALSE;
		}
	}
	else
	{
		cond.m_type = ALARM_TYPE_TRADEVOLUME;
		cond.m_fValue = atof(m_strTradeVolume)*100;
		if( !IsNumber(m_strTradeVolume,FALSE) )
		{
			AfxMessageBox( IDS_ALARMSETTING_NOTNUMBER );
			return FALSE;
		}
	}
	return TRUE;
}

void CAlarmSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmSettingDlg)
	DDX_Control(pDX, IDC_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_LISTCOND, m_listCond);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_TRADEVOLUME, m_editTradeVolume);
	DDX_Control(pDX, IDC_DIFFPERCENTLESS, m_editDiffPercentLess);
	DDX_Control(pDX, IDC_DIFFPERCENTMORE, m_editDiffPercentMore);
	DDX_Control(pDX, IDC_STOCK, m_cmbStock);
	DDX_Text(pDX, IDC_DIFFPERCENTMORE, m_strDiffPercentMore);
	DDX_Text(pDX, IDC_DIFFPERCENTLESS, m_strDiffPercentLess);
	DDX_Text(pDX, IDC_TRADEVOLUME, m_strTradeVolume);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarmSettingDlg, CPropertyPageEx)
	//{{AFX_MSG_MAP(CAlarmSettingDlg)
	ON_BN_CLICKED(IDC_RADIO_DIFFPERCENTMORE, OnRadioDiffpercentmore)
	ON_BN_CLICKED(IDC_RADIO_DIFFPERCENTLESS, OnRadioDiffpercentless)
	ON_BN_CLICKED(IDC_RADIO_TRADEVOLUME, OnRadioTradevolume)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_LBN_SELCHANGE(IDC_LISTCOND, OnSelchangeListcond)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmSettingDlg message handlers

BOOL CAlarmSettingDlg::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	CheckRadioButton( IDC_RADIO_DIFFPERCENTMORE, IDC_RADIO_TRADEVOLUME, IDC_RADIO_DIFFPERCENTMORE );
	OnRadioDiffpercentmore();

	ResetListCond( );

	m_cmbStock.InitStocks( FALSE, FALSE, TRUE );
	m_cmbStock.SetAutoHide( FALSE );
	m_cmbStock.SetCurSel( 0 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAlarmSettingDlg::OnSetActive() 
{
	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		CString	strFinish, strClose;
		strFinish.LoadString( IDS_FINISHBUTTON );
		strClose.LoadString( IDS_CLOSEBUTTON );

		CWnd * pWnd = pSheet->GetDlgItem( IDOK );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		{
			pWnd->SetWindowText( strFinish );
			pWnd->EnableWindow( FALSE );
		}

		pWnd = pSheet->GetDlgItem( IDCANCEL );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
			pWnd->SetWindowText( strClose );
	}
	
	return CPropertyPageEx::OnSetActive();
}

void CAlarmSettingDlg::OnRadioDiffpercentmore() 
{
	m_editDiffPercentMore.EnableWindow( TRUE );
	m_editDiffPercentLess.EnableWindow( FALSE );
	m_editTradeVolume.EnableWindow( FALSE );
}

void CAlarmSettingDlg::OnRadioDiffpercentless() 
{
	m_editDiffPercentMore.EnableWindow( FALSE );
	m_editDiffPercentLess.EnableWindow( TRUE );
	m_editTradeVolume.EnableWindow( FALSE );
}

void CAlarmSettingDlg::OnRadioTradevolume() 
{
	m_editDiffPercentMore.EnableWindow( FALSE );
	m_editDiffPercentLess.EnableWindow( FALSE );
	m_editTradeVolume.EnableWindow( TRUE );
}

void CAlarmSettingDlg::OnAdd() 
{
	UpdateData();

	CAlarmCondContainer & container = AfxGetProfile().GetAlarmCondContainer();

	ALARMCOND cond;
	if( GetCurrentCond(cond) )
	{
		int nIndex = container.Add( cond );
		int nItem = m_listCond.AddString(container.AlarmCondToString(cond));
		m_listCond.SetItemData( nItem, nIndex );
		m_listCond.SetCurSel( nItem );
		m_listCond.Invalidate();
	}
}

void CAlarmSettingDlg::OnModify() 
{
	UpdateData();

	int nSel = m_listCond.GetCurSel();
	if( LB_ERR == nSel )
	{
		AfxMessageBox( IDS_ALARMSETTING_NOCONDSELECTED );
		return;
	}

	CAlarmCondContainer & container = AfxGetProfile().GetAlarmCondContainer();
	int nData = m_listCond.GetItemData(nSel);
	if( nData >= 0 && nData < container.GetSize() )
	{
		ALARMCOND cond;
		if( GetCurrentCond( cond ) )
		{
			container.SetAt( nData, cond );
			ResetListCond();
			m_listCond.SetCurSel(nSel);
			m_listCond.Invalidate();
		}
	}
}

void CAlarmSettingDlg::OnDelete() 
{
	UpdateData();

	int nSel = m_listCond.GetCurSel();
	if( LB_ERR == nSel )
	{
		AfxMessageBox( IDS_ALARMSETTING_NOCONDSELECTED );
		return;
	}

	CAlarmCondContainer & container = AfxGetProfile().GetAlarmCondContainer();
	int nData = m_listCond.GetItemData(nSel);
	if( nData >= 0 && nData < container.GetSize() )
	{
		container.RemoveAt( nData );
		ResetListCond();
		m_listCond.SetCurSel( nSel >= m_listCond.GetCount() ? 0 : nSel );
		m_listCond.Invalidate();
	}
}

void CAlarmSettingDlg::OnSelchangeListcond() 
{
	UpdateData();

	int nSel = m_listCond.GetCurSel();
	if( LB_ERR == nSel )
		return;

	CAlarmCondContainer & container = AfxGetProfile().GetAlarmCondContainer();
	int nData = m_listCond.GetItemData(nSel);
	if( nData >= 0 && nData < container.GetSize() )
	{
		m_editDiffPercentMore.EnableWindow( FALSE );
		m_editDiffPercentLess.EnableWindow( FALSE );
		m_editTradeVolume.EnableWindow( FALSE );

		ALARMCOND & cond = container.ElementAt(nData);
		if( ALARM_TYPE_DIFFPERCENTMORE == cond.m_type )
		{
			CheckRadioButton( IDC_RADIO_DIFFPERCENTMORE, IDC_RADIO_TRADEVOLUME, IDC_RADIO_DIFFPERCENTMORE );
			m_editDiffPercentMore.EnableWindow( TRUE );
			m_strDiffPercentMore.Format( "%.2f", cond.m_fValue );
		}
		else if( ALARM_TYPE_DIFFPERCENTLESS == cond.m_type )
		{
			CheckRadioButton( IDC_RADIO_DIFFPERCENTMORE, IDC_RADIO_TRADEVOLUME, IDC_RADIO_DIFFPERCENTLESS );
			m_editDiffPercentLess.EnableWindow( TRUE );
			m_strDiffPercentLess.Format( "%.2f", cond.m_fValue );
		}
		else
		{
			CheckRadioButton( IDC_RADIO_DIFFPERCENTMORE, IDC_RADIO_TRADEVOLUME, IDC_RADIO_TRADEVOLUME );
			m_editTradeVolume.EnableWindow( TRUE );
			m_strTradeVolume.Format( "%.2f", cond.m_fValue*0.01 );
		}
		m_cmbStock.SelectString( 0, cond.m_szCode );

		UpdateData( FALSE );
	}
}

