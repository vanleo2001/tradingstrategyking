// MADlg.cpp : implementation file
//

#include "stdafx.h"
#include "MADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMADlg dialog


CMADlg::CMADlg(CWnd* pParent, CMA * pMA)
{
	CParamDlg::CParamDlg(CMADlg::IDD, pParent);

	//{{AFX_DATA_INIT(CMADlg)
	m_bMA1 = FALSE;
	m_bMA2 = FALSE;
	m_bMA3 = FALSE;
	m_bMA4 = FALSE;
	m_nDays1 = 0;
	m_nDays2 = 0;
	m_nDays3 = 0;
	m_nDays4 = 0;
	//}}AFX_DATA_INIT

	m_pMA	=	pMA;
}


void CMADlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMADlg)
	DDX_Control(pDX, IDC_SHORT, m_cmbShort);
	DDX_Control(pDX, IDC_LONG, m_cmbLong);
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Check(pDX, IDC_MA1, m_bMA1);
	DDX_Check(pDX, IDC_MA2, m_bMA2);
	DDX_Check(pDX, IDC_MA3, m_bMA3);
	DDX_Check(pDX, IDC_MA4, m_bMA4);
	DDX_Text(pDX, IDC_DAYS1, m_nDays1);
	DDX_Text(pDX, IDC_DAYS2, m_nDays2);
	DDX_Text(pDX, IDC_DAYS3, m_nDays3);
	DDX_Text(pDX, IDC_DAYS4, m_nDays4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMADlg, CParamDlg)
	//{{AFX_MSG_MAP(CMADlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMADlg message handlers

BOOL CMADlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();

	ASSERT( m_pMA );
	
	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMADlg::OnCancel() 
{
}

void CMADlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pMA->SetDefaultParameters( );
	
	CParamDlg::OnOK();
}

void CMADlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pMA->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CMADlg::OnHelp() 
{
	ParamHelp( STT_MA );
}

BOOL CMADlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pMA->IsValidParameters() )
		m_pMA->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );

		CButton * pbtn = (CButton *)GetDlgItem(IDC_EXPMA);
		if( pbtn && pbtn->GetCheck() )
			m_pMA->m_nType	=	CMA::typeEXPMA;
		else
			m_pMA->m_nType	=	CMA::typeMA;
		
		m_pMA->m_adwMADays.RemoveAll();
		if( m_bMA1 )	m_pMA->m_adwMADays.Add( m_nDays1 );
		if( m_bMA2 )	m_pMA->m_adwMADays.Add( m_nDays2 );
		if( m_bMA3 )	m_pMA->m_adwMADays.Add( m_nDays3 );
		if( m_bMA4 )	m_pMA->m_adwMADays.Add( m_nDays4 );

		m_pMA->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect( );
		m_pMA->m_itsDeadFork	=	m_cmbDeadFork.GetSelect( );
		m_pMA->m_itsLong		=	m_cmbLong.GetSelect( );
		m_pMA->m_itsShort		=	m_cmbShort.GetSelect( );
		return m_pMA->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_MA) );

		CheckRadioButton( IDC_STDMA, IDC_EXPMA, CMA::typeEXPMA == m_pMA->m_nType ? IDC_EXPMA : IDC_STDMA );

		m_bMA1	=	m_pMA->m_adwMADays.GetSize() >= 1;
		m_bMA2	=	m_pMA->m_adwMADays.GetSize() >= 2;
		m_bMA3	=	m_pMA->m_adwMADays.GetSize() >= 3;
		m_bMA4	=	m_pMA->m_adwMADays.GetSize() >= 4;

		if( m_bMA1 )	m_nDays1	=	m_pMA->m_adwMADays[0];
		if( m_bMA2 )	m_nDays2	=	m_pMA->m_adwMADays[1];
		if( m_bMA3 )	m_nDays3	=	m_pMA->m_adwMADays[2];
		if( m_bMA4 )	m_nDays4	=	m_pMA->m_adwMADays[3];

		m_cmbGoldenFork.ResetAndSelect( m_pMA->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pMA->m_itsDeadFork );
		m_cmbLong.ResetAndSelect( m_pMA->m_itsLong );
		m_cmbShort.ResetAndSelect( m_pMA->m_itsShort );
		UpdateData( FALSE );
	}

	return TRUE;
}

