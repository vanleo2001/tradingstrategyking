// SelectEngineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StkNet.h"
#include "Stockdrv.h"
#include "STKDRV.h"
#include "SelectEngineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectEngineDlg dialog


CSelectEngineDlg::CSelectEngineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectEngineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectEngineDlg)
	m_bAutoStart = FALSE;
	//}}AFX_DATA_INIT
	m_bAutoStart	=	TRUE;
}

BOOL CSelectEngineDlg::GetSelectedDriver( CStringArray & astrProvider, CStringArray & astrDriver )
{
	astrProvider.Copy( m_astrProvider );
	astrDriver.Copy( m_astrDriver );
	return astrDriver.GetSize() > 0 ;
}

int CSelectEngineDlg::DoModal() 
{
	HINSTANCE hInstanceOld = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetModuleHandle() );
	
	int nRet = CDialog::DoModal();
	
	AfxSetResourceHandle( hInstanceOld );
	return nRet;
}

void CSelectEngineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectEngineDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_SETUPRECEIVER, m_btnSetup);
	DDX_Control(pDX, IDC_LISTENGINE, m_listEngine);
	DDX_Check(pDX, IDC_CHECKAUTOSTART, m_bAutoStart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectEngineDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectEngineDlg)
	ON_BN_CLICKED(IDC_SETUPRECEIVER, OnSetupreceiver)
	ON_LBN_SELCHANGE(IDC_LISTENGINE, OnSelchangeListengine)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectEngineDlg message handlers
BOOL CSelectEngineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 读取注册表中的通视兼容设备
	AfxRegQueryString( HKEY_LOCAL_MACHINE, _T("Software"), _T("stockdrv"), _T("兼容设备"), m_astrProvider, m_astrDriver );
	m_astrProvider.InsertAt( 0, AfxRegQueryString( HKEY_LOCAL_MACHINE, _T("Software"), _T("stockdrv"), "", "", _T("Provider"), "Tongshi" ) );
	m_astrDriver.InsertAt( 0, AfxRegQueryString( HKEY_LOCAL_MACHINE, _T("Software"), _T("stockdrv"), "", "", _T("Driver"), "Stock.dll" ) );

	// 取得缺省启动设备
	CSPStringArray	aProDefault, aDrvDefault;
	AfxGetProfile().GetRcvDriverDefault( aProDefault, aDrvDefault );
	if( 0 == aDrvDefault.GetSize() )
	{
		aProDefault.InsertAt( 0, m_astrProvider[0] );
		aDrvDefault.InsertAt( 0, m_astrDriver[0] );
	}

	m_listEngine.SetCurSel( 0 );

	// 加入m_listEngine
	for( int i=0; i<m_astrProvider.GetSize() && i<m_astrDriver.GetSize(); i++ )
	{
		CString	strTemp	=	" " + m_astrProvider[i];
		while( strTemp.GetLength() < 15 )	strTemp	+=	" ";
		int nItem = m_listEngine.AddString( strTemp + m_astrDriver[i] );
		m_listEngine.SetItemData( nItem, i );

		// 选择，如果是缺省启动
		for( int j=0; j<aProDefault.GetSize() && j<aDrvDefault.GetSize(); j++ )
		{
			if( 0 == m_astrProvider[i].Compare(aProDefault[j])
				&& 0 == m_astrDriver[i].Compare(aDrvDefault[j]) )
				m_listEngine.SetCheck( nItem, TRUE );
		}
	}
	m_listEngine.SetCurSel( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectEngineDlg::OnOK() 
{
	UpdateData( );

	CStringArray	aProStart, aDrvStart;
	for( int i=0; i<m_listEngine.GetCount(); i++ )
	{
		if( m_listEngine.GetCheck(i) )
		{
			int	nData = (int)m_listEngine.GetItemData(i);
			if( nData >= 0 && nData<m_astrProvider.GetSize() && nData<m_astrDriver.GetSize() )
			{
				aProStart.Add( m_astrProvider[nData] );
				aDrvStart.Add( m_astrDriver[nData] );
			}
		}
	}

	m_astrProvider.Copy( aProStart );
	m_astrDriver.Copy( aDrvStart );

	if( m_bAutoStart )
	{
		CSPStringArray	aProDefault, aDrvDefault;
		for( int k=0; k<aProStart.GetSize(); k++ )	aProDefault.Add(aProStart[k]);
		for( int k=0; k<aDrvStart.GetSize(); k++ )		aDrvDefault.Add(aDrvStart[k]);
		AfxGetProfile().SetRcvDriverDefault( aProDefault, aDrvDefault );
	}
	
	CDialog::OnOK();
}

void CSelectEngineDlg::OnSetupreceiver() 
{
	int	nSel = m_listEngine.GetCurSel();
	if( LB_ERR == nSel )
	{
		AfxMessageBox( AfxModuleLoadString(IDS_SELECTENGINE_ERRSELECT), MB_OK );
		return;
	}

	int	nData = (int)m_listEngine.GetItemData(nSel);
	if( nData >= 0 && nData<m_astrDriver.GetSize() )
	{
		m_drv.Stock_Quit(GetSafeHwnd());
		m_drv.UnloadDriver();

		if( m_drv.LoadDriver( m_astrDriver[nData] )
			&& m_drv.Stock_Init(GetSafeHwnd(),WM_APP_STKRECEIVER_TSDATA,RCV_WORK_SENDMSG) )
		{
			m_drv.SetupReceiver(TRUE);
		}
		else
			AfxMessageBox( AfxModuleLoadString(IDS_SELECTENGINE_ERRLOAD), MB_OK );
	}
	m_btnSetup.EnableWindow( FALSE );
}

void CSelectEngineDlg::OnSelchangeListengine() 
{
	int	nSel = m_listEngine.GetCurSel();
	m_btnSetup.EnableWindow( LB_ERR != nSel );
}

void CSelectEngineDlg::OnDestroy() 
{
	m_drv.Stock_Quit(GetSafeHwnd());
	m_drv.UnloadDriver();
	
	CDialog::OnDestroy();
}
