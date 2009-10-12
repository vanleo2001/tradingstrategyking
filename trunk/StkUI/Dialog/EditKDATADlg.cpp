// EditKDATADlg.cpp : implementation file
//

#include "stdafx.h"

#include "EditKDATADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditKDATADlg dialog


CEditKDATADlg::CEditKDATADlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CEditKDATADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditKDATADlg)
	m_tmDate = 0;
	m_strOpen = _T("");
	m_strHigh = _T("");
	m_strLow = _T("");
	m_strClose = _T("");
	m_dwVolume = 0;
	m_dwAmount = 0;
	//}}AFX_DATA_INIT
	m_tmDate	=	CSPTime::GetCurrentTime();
	memset( &m_kd, 0, sizeof(m_kd) );
}

void CEditKDATADlg::SetKDATA( KDATA kd, int nKType )
{
	m_kd		=	kd;
	m_nKType	=	nKType;

	CSPTime	sptime;
	if( sptime.FromStockTime( kd.m_date, CKData::IsDayOrMin(m_nKType) ) )
		m_tmDate	=	CSPTime( sptime.GetTime() );
	m_strOpen.Format( "%.3f", kd.m_fOpen );
	m_strHigh.Format( "%.3f", kd.m_fHigh );
	m_strLow.Format( "%.3f", kd.m_fLow );
	m_strClose.Format( "%.3f", kd.m_fClose );
	m_dwVolume	=	(DWORD)(kd.m_fVolume/100);
	m_dwAmount	=	(DWORD)(kd.m_fAmount/1000);
}

KDATA CEditKDATADlg::GetKDATA( )
{
	return m_kd;
}

void CEditKDATADlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditKDATADlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_tmDate);
	DDX_Text(pDX, IDC_OPEN, m_strOpen);
	DDX_Text(pDX, IDC_HIGH, m_strHigh);
	DDX_Text(pDX, IDC_LOW, m_strLow);
	DDX_Text(pDX, IDC_CLOSE, m_strClose);
	DDX_Text(pDX, IDC_VOLUME, m_dwVolume);
	DDX_Text(pDX, IDC_AMOUNT, m_dwAmount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditKDATADlg, CXFDialog)
	//{{AFX_MSG_MAP(CEditKDATADlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditKDATADlg message handlers

BOOL CEditKDATADlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditKDATADlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	if( !IsNumber( m_strOpen, FALSE ) || !IsNumber( m_strHigh, FALSE )
		|| !IsNumber( m_strLow, FALSE ) || !IsNumber( m_strClose, FALSE ) )
	{
		AfxMessageBox( IDS_EDITKDATA_NOTNUMBER, MB_OK|MB_ICONINFORMATION );
		return;
	}

	CSPTime	sptime( m_tmDate.GetTime() );
	m_kd.m_date		=	sptime.ToStockTime( CKData::IsDayOrMin(m_nKType) );
	m_kd.m_fOpen	=	(float)atof(m_strOpen);
	m_kd.m_fHigh	=	(float)atof(m_strHigh);
	m_kd.m_fLow		=	(float)atof(m_strLow);
	m_kd.m_fClose	=	(float)atof(m_strClose);
	m_kd.m_fVolume	=	(float)m_dwVolume*100;
	m_kd.m_fAmount	=	(float)m_dwAmount*1000;

	CXFDialog::OnOK();
}
