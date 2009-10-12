// VRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVRDlg dialog


CVRDlg::CVRDlg(CWnd* pParent, CVR * pVR )
{
	CParamDlg::CParamDlg(CVRDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CVRDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pVR	=	pVR;
}


void CVRDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVRDlg)
	DDX_Control(pDX, IDC_SHORT, m_cmbShort);
	DDX_Control(pDX, IDC_LONG, m_cmbLong);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVRDlg, CParamDlg)
	//{{AFX_MSG_MAP(CVRDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVRDlg message handlers

BOOL CVRDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pVR );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVRDlg::OnCancel() 
{
}

void CVRDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pVR->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CVRDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pVR->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CVRDlg::OnHelp() 
{
	ParamHelp( STT_VR );
}

BOOL CVRDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pVR->IsValidParameters() )
		m_pVR->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pVR->m_nDays		=	m_nDays;
		
		m_pVR->m_itsLong	=	m_cmbLong.GetSelect();
		m_pVR->m_itsShort	=	m_cmbShort.GetSelect();
		return m_pVR->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_VR) );

		m_nDays		=	m_pVR->m_nDays;

		m_cmbLong.ResetAndSelect( m_pVR->m_itsLong );
		m_cmbShort.ResetAndSelect( m_pVR->m_itsShort );
		UpdateData( FALSE );
	}

	return TRUE;
}

