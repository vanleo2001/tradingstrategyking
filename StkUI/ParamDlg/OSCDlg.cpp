// OSCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OSCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COSCDlg dialog


COSCDlg::COSCDlg(CWnd* pParent, COSC * pOSC )
{
	CParamDlg::CParamDlg(COSCDlg::IDD, pParent);

	//{{AFX_DATA_INIT(COSCDlg)
	m_nDays = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pOSC	=	pOSC;
}


void COSCDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COSCDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COSCDlg, CParamDlg)
	//{{AFX_MSG_MAP(COSCDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COSCDlg message handlers

BOOL COSCDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pOSC );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COSCDlg::OnCancel() 
{
}

void COSCDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pOSC->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void COSCDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pOSC->SetDefaultParameters( );
	RefreshData( FALSE );
}

void COSCDlg::OnHelp() 
{
	ParamHelp( STT_OSC );
}

BOOL COSCDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pOSC->IsValidParameters() )
		m_pOSC->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pOSC->m_nDays		=	m_nDays;
		m_pOSC->m_nMADays	=	m_nMADays;

		m_pOSC->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect();
		m_pOSC->m_itsDeadFork	=	m_cmbDeadFork.GetSelect();
		return m_pOSC->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_OSC) );

		m_nDays		=	m_pOSC->m_nDays;
		m_nMADays	=	m_pOSC->m_nMADays;

		m_cmbGoldenFork.ResetAndSelect( m_pOSC->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pOSC->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

