// DCYODlg.cpp : implementation file
//

#include "stdafx.h"
#include "DCYODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCYODlg dialog


CDCYODlg::CDCYODlg(CWnd* pParent, CDCYO * pDCYO )
{
	CParamDlg::CParamDlg(CDCYODlg::IDD, pParent);

	//{{AFX_DATA_INIT(CDCYODlg)
	//}}AFX_DATA_INIT

	m_pDCYO	=	pDCYO;
}


void CDCYODlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDCYODlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDCYODlg, CParamDlg)
	//{{AFX_MSG_MAP(CDCYODlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCYODlg message handlers

BOOL CDCYODlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pDCYO );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDCYODlg::OnCancel() 
{
}

void CDCYODlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pDCYO->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CDCYODlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pDCYO->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CDCYODlg::OnHelp() 
{
	ParamHelp( STT_DCYO );
}

BOOL CDCYODlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pDCYO->IsValidParameters() )
		m_pDCYO->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pDCYO->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect( );
		m_pDCYO->m_itsDeadFork	=	m_cmbDeadFork.GetSelect( );
		return m_pDCYO->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_DCYO) );

		m_cmbGoldenFork.ResetAndSelect( m_pDCYO->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pDCYO->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

