// VROCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VROCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVROCDlg dialog


CVROCDlg::CVROCDlg(CWnd* pParent, CVROC * pVROC )
{
	CParamDlg::CParamDlg(CVROCDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CVROCDlg)
	m_nDays = 0;
	m_nMADays = 0;
	//}}AFX_DATA_INIT

	m_pVROC	=	pVROC;
}


void CVROCDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVROCDlg)
	DDX_Control(pDX, IDC_DEVIATEONTOP, m_cmbDeviateOnTop);
	DDX_Control(pDX, IDC_DEVIATEONBOTTOM, m_cmbDeviateOnBottom);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Text(pDX, IDC_DAYS, m_nDays);
	DDX_Text(pDX, IDC_MADAYS, m_nMADays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVROCDlg, CParamDlg)
	//{{AFX_MSG_MAP(CVROCDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVROCDlg message handlers

BOOL CVROCDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pVROC );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVROCDlg::OnCancel() 
{
}

void CVROCDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pVROC->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CVROCDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pVROC->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CVROCDlg::OnHelp() 
{
	ParamHelp( STT_VROC );
}

BOOL CVROCDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pVROC->IsValidParameters() )
		m_pVROC->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pVROC->m_nDays	=	m_nDays;
		m_pVROC->m_nMADays	=	m_nMADays;

		m_pVROC->m_itsDeviateOnBottom	=	m_cmbDeviateOnBottom.GetSelect();
		m_pVROC->m_itsDeviateOnTop		=	m_cmbDeviateOnTop.GetSelect();
		return m_pVROC->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_VROC) );

		m_nDays		=	m_pVROC->m_nDays;
		m_nMADays	=	m_pVROC->m_nMADays;

		m_cmbDeviateOnBottom.ResetAndSelect( m_pVROC->m_itsDeviateOnBottom );
		m_cmbDeviateOnTop.ResetAndSelect( m_pVROC->m_itsDeviateOnTop );
		UpdateData( FALSE );
	}

	return TRUE;
}

