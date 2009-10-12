// DPERDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DPERDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDPERDlg dialog


CDPERDlg::CDPERDlg(CWnd* pParent, CDPER * pDPER )
{
	CParamDlg::CParamDlg(CDPERDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CDPERDlg)
	//}}AFX_DATA_INIT

	m_pDPER	=	pDPER;
}


void CDPERDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDPERDlg)
	DDX_Control(pDX, IDC_DEADFORK, m_cmbDeadFork);
	DDX_Control(pDX, IDC_GOLDENFORK, m_cmbGoldenFork);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDPERDlg, CParamDlg)
	//{{AFX_MSG_MAP(CDPERDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDPERDlg message handlers

BOOL CDPERDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pDPER );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDPERDlg::OnCancel() 
{
}

void CDPERDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pDPER->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CDPERDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pDPER->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CDPERDlg::OnHelp() 
{
	ParamHelp( STT_DPER );
}

BOOL CDPERDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pDPER->IsValidParameters() )
		m_pDPER->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pDPER->m_itsGoldenFork	=	m_cmbGoldenFork.GetSelect( );
		m_pDPER->m_itsDeadFork	=	m_cmbDeadFork.GetSelect( );
		return m_pDPER->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_DPER) );

		m_cmbGoldenFork.ResetAndSelect( m_pDPER->m_itsGoldenFork );
		m_cmbDeadFork.ResetAndSelect( m_pDPER->m_itsDeadFork );
		UpdateData( FALSE );
	}

	return TRUE;
}

