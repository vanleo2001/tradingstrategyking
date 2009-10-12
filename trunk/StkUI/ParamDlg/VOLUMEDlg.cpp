// VOLUMEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VOLUMEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVOLUMEDlg dialog


CVOLUMEDlg::CVOLUMEDlg(CWnd* pParent, CVOLUME * pVOLUME )
{
	CParamDlg::CParamDlg(CVOLUMEDlg::IDD, pParent);

	//{{AFX_DATA_INIT(CVOLUMEDlg)
	m_bMA1 = FALSE;
	m_bMA2 = FALSE;
	m_bMA3 = FALSE;
	m_bMA4 = FALSE;
	m_nDays1 = 0;
	m_nDays2 = 0;
	m_nDays3 = 0;
	m_nDays4 = 0;
	//}}AFX_DATA_INIT

	m_pVOLUME	=	pVOLUME;
}


void CVOLUMEDlg::DoDataExchange(CDataExchange* pDX)
{
	CParamDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVOLUMEDlg)
	DDX_Control(pDX, IDC_SHORT, m_cmbShort);
	DDX_Control(pDX, IDC_LONG, m_cmbLong);
	DDX_Control(pDX, IDC_DEVIATEONTOP, m_cmbDeviateOnTop);
	DDX_Control(pDX, IDC_DEVIATEONBOTTOM, m_cmbDeviateOnBottom);
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


BEGIN_MESSAGE_MAP(CVOLUMEDlg, CParamDlg)
	//{{AFX_MSG_MAP(CVOLUMEDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_PARAMHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVOLUMEDlg message handlers

BOOL CVOLUMEDlg::OnInitDialog() 
{
	CParamDlg::OnInitDialog();
	
	ASSERT( m_pVOLUME );

	// TODO: Add extra initialization here
	RefreshData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVOLUMEDlg::OnCancel() 
{
}

void CVOLUMEDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( !RefreshData( TRUE ) )
		m_pVOLUME->SetDefaultParameters( );

	CParamDlg::OnOK();
}

void CVOLUMEDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_pVOLUME->SetDefaultParameters( );
	RefreshData( FALSE );
}

void CVOLUMEDlg::OnHelp() 
{
	ParamHelp( STT_VOLUME );
}

BOOL CVOLUMEDlg::RefreshData( BOOL bSaveAndValidate )
{
	if( !m_pVOLUME->IsValidParameters() )
		m_pVOLUME->SetDefaultParameters();

	if( bSaveAndValidate )
	{
		UpdateData( TRUE );
		
		m_pVOLUME->m_adwMADays.RemoveAll();
		if( m_bMA1 )	m_pVOLUME->m_adwMADays.Add( m_nDays1 );
		if( m_bMA2 )	m_pVOLUME->m_adwMADays.Add( m_nDays2 );
		if( m_bMA3 )	m_pVOLUME->m_adwMADays.Add( m_nDays3 );
		if( m_bMA4 )	m_pVOLUME->m_adwMADays.Add( m_nDays4 );

		m_pVOLUME->m_itsDeviateOnBottom	=	m_cmbDeviateOnBottom.GetSelect( );
		m_pVOLUME->m_itsDeviateOnTop	=	m_cmbDeviateOnTop.GetSelect( );
		m_pVOLUME->m_itsLong		=	m_cmbLong.GetSelect( );
		m_pVOLUME->m_itsShort		=	m_cmbShort.GetSelect( );
		return m_pVOLUME->IsValidParameters();
	}
	else
	{
		m_staticName.SetWindowText( AfxGetSTTFullName(STT_VOLUME) );

		m_bMA1	=	m_pVOLUME->m_adwMADays.GetSize() >= 1;
		m_bMA2	=	m_pVOLUME->m_adwMADays.GetSize() >= 2;
		m_bMA3	=	m_pVOLUME->m_adwMADays.GetSize() >= 3;
		m_bMA4	=	m_pVOLUME->m_adwMADays.GetSize() >= 4;

		if( m_bMA1 )	m_nDays1	=	m_pVOLUME->m_adwMADays[0];
		if( m_bMA2 )	m_nDays2	=	m_pVOLUME->m_adwMADays[1];
		if( m_bMA3 )	m_nDays3	=	m_pVOLUME->m_adwMADays[2];
		if( m_bMA4 )	m_nDays4	=	m_pVOLUME->m_adwMADays[3];

		m_cmbDeviateOnBottom.ResetAndSelect( m_pVOLUME->m_itsDeviateOnBottom );
		m_cmbDeviateOnTop.ResetAndSelect( m_pVOLUME->m_itsDeviateOnTop );
		m_cmbLong.ResetAndSelect( m_pVOLUME->m_itsLong );
		m_cmbShort.ResetAndSelect( m_pVOLUME->m_itsShort );
		UpdateData( FALSE );
	}

	return TRUE;
}

