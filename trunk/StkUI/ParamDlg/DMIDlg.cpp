// DMIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DMIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMIDlg dialog


CDMIDlg::CDMIDlg(CWnd* pParent, CDMI * pDMI )
{
	CParamDlg::CParamDlg(CDMIDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CDMIDlg)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pDMI	=	pDMI;
}


void CDMIDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMIDlg)
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDMIDlg, CParamDlg)
	//{{AFX_MSG_MAP(CDMIDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMIDlg message handlers

BOOL CDMIDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pDMI );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMIDlg::OnCancel() 
{
}

void CDMIDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pDMI->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CDMIDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pDMI->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CDMIDlg::OnHelp() 
{
	ParamHelp( STT_DMI );
}

BOOL CDMIDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pDMI->IsValidParameters() )
		m_pDMI->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pDMI->m_nDays		=	m_nDays;
		return m_pDMI->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_DMI) );

		m_nDays	=	m_pDMI->m_nDays;
		UpdateData( FALSE );
	}

	return TRUE;
}

