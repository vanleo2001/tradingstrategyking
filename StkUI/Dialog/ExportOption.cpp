// ExportOption.cpp : implementation file
//

#include "stdafx.h"

#include "ExportOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportOption property page

void CExportOption::StoreProfile( )
{
	AfxGetApp()->WriteProfileInt( "exportoption", "timeall", m_bTimeAll );
	AfxGetApp()->WriteProfileInt( "exportoption", "timedefine", m_bTimeDefine );
	CSPTime( AfxGetApp()->WriteProfileInt( "exportoption", "tmbegin", m_tmBegin.GetTime() ) );
	CSPTime( AfxGetApp()->WriteProfileInt( "exportoption", "tmend", m_tmEnd.GetTime() ) );

	AfxGetApp()->WriteProfileInt( "exportoption", "kmonth", m_bKMonth );
	AfxGetApp()->WriteProfileInt( "exportoption", "kweek", m_bKWeek );
	AfxGetApp()->WriteProfileInt( "exportoption", "kday", m_bKDay );
	AfxGetApp()->WriteProfileInt( "exportoption", "kmin60", m_bKMin60 );
	AfxGetApp()->WriteProfileInt( "exportoption", "kmin30", m_bKMin30 );
	AfxGetApp()->WriteProfileInt( "exportoption", "kmin15", m_bKMin15 );
	AfxGetApp()->WriteProfileInt( "exportoption", "kmin5", m_bKMin5 );

	AfxGetApp()->WriteProfileInt( "exportoption", "noresumedr", m_bNoResumeDR );
	AfxGetApp()->WriteProfileInt( "exportoption", "resumedrup", m_bResumeDRUp );
	AfxGetApp()->WriteProfileInt( "exportoption", "resumedrdown", m_bResumeDRDown );

	AfxGetApp()->WriteProfileInt( "exportoption", "autoresumedr", m_bAutoResumeDR );
}

void CExportOption::LoadProfile( )
{
	// Initial Data
	m_bTimeAll		=	FALSE;
	m_bTimeDefine	=	FALSE;
	m_tmBegin = CSPTime::GetCurrentTime();
	m_tmEnd = CSPTime::GetCurrentTime();

	m_bKDay	=	TRUE;

	m_bNoResumeDR	=	FALSE;
	m_bResumeDRUp	=	FALSE;
	m_bResumeDRDown	=	FALSE;
	m_tmAutoResumeDRBegin	=	CSPTime::GetCurrentTime();
	CSPTime	sptime;
	if( sptime.FromStockTimeDay( AfxGetProfile().GetAutoResumeDRBegin() ) )
		m_tmAutoResumeDRBegin	=	sptime.GetTime();

	m_nAutoResumeDRLimit	=	AfxGetProfile().GetAutoResumeDRLimit();

	m_bTimeAll		=	AfxGetApp()->GetProfileInt( "exportoption", "timeall", m_bTimeAll );
	m_bTimeDefine	=	AfxGetApp()->GetProfileInt( "exportoption", "timedefine", m_bTimeDefine );
	m_tmBegin		=	CSPTime( AfxGetApp()->GetProfileInt( "exportoption", "tmend", m_tmEnd.GetTime() ) );
	m_tmBegin	+=	CSPTimeSpan( 1, 0, 0, 0 );
	if( m_tmBegin > m_tmEnd )	m_tmBegin	=	m_tmEnd;

	m_bKMonth		=	AfxGetApp()->GetProfileInt( "exportoption", "kmonth", m_bKMonth );
	m_bKWeek		=	AfxGetApp()->GetProfileInt( "exportoption", "kweek", m_bKWeek );
	m_bKDay			=	AfxGetApp()->GetProfileInt( "exportoption", "kday", m_bKDay );
	m_bKMin60		=	AfxGetApp()->GetProfileInt( "exportoption", "kmin60", m_bKMin60 );
	m_bKMin30		=	AfxGetApp()->GetProfileInt( "exportoption", "kmin30", m_bKMin30 );
	m_bKMin15		=	AfxGetApp()->GetProfileInt( "exportoption", "kmin15", m_bKMin15 );
	m_bKMin5		=	AfxGetApp()->GetProfileInt( "exportoption", "kmin5", m_bKMin5 );

	m_bNoResumeDR	=	AfxGetApp()->GetProfileInt( "exportoption", "noresumedr", m_bNoResumeDR );
	m_bResumeDRUp	=	AfxGetApp()->GetProfileInt( "exportoption", "resumedrup", m_bResumeDRUp );
	m_bResumeDRDown	=	AfxGetApp()->GetProfileInt( "exportoption", "resumedrdown", m_bResumeDRDown );

	m_bAutoResumeDR	=	AfxGetApp()->GetProfileInt( "exportoption", "autoresumedr", m_bAutoResumeDR );
}


IMPLEMENT_DYNCREATE(CExportOption, CPropertyPageEx)

CExportOption::CExportOption() : CPropertyPageEx(CExportOption::IDD)
{
	//{{AFX_DATA_INIT(CExportOption)
	m_tmBegin = 0;
	m_tmEnd = 0;
	m_bKMonth = FALSE;
	m_bKWeek = FALSE;
	m_bKDay = FALSE;
	m_bKMin60 = FALSE;
	m_bKMin30 = FALSE;
	m_bKMin15 = FALSE;
	m_bKMin5 = FALSE;
	m_bAutoResumeDR = FALSE;
	m_tmAutoResumeDRBegin = 0;
	m_nAutoResumeDRLimit = 0;
	//}}AFX_DATA_INIT

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
}

CExportOption::CExportOption(UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle)
			 : CPropertyPageEx(CExportOption::IDD, nIDCaption, nIDTitle, nIDSubTitle)
{
	m_tmBegin = 0;
	m_tmEnd = 0;
	m_bKMonth = FALSE;
	m_bKWeek = FALSE;
	m_bKDay = FALSE;
	m_bKMin60 = FALSE;
	m_bKMin30 = FALSE;
	m_bKMin15 = FALSE;
	m_bKMin5 = FALSE;
	m_bAutoResumeDR = FALSE;
	m_tmAutoResumeDRBegin = 0;
	m_nAutoResumeDRLimit = 0;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
}

CExportOption::~CExportOption()
{
}

void CExportOption::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportOption)
	DDX_Control(pDX, IDC_AUTORESUMEDR, m_btnAutoResumeDR);
	DDX_Control(pDX, IDC_TIME_AUTORESUMEDRBEGIN, m_ctrlTimeAutoResumeDRBegin);
	DDX_Control(pDX, IDC_AUTORESUMEDR_LIMIT, m_editAutoResumeDRLimit);
	DDX_Control(pDX, IDC_TIME_END, m_ctrlTimeEnd);
	DDX_Control(pDX, IDC_TIME_BEGIN, m_ctrlTimeBegin);
	DDX_DateTimeCtrl(pDX, IDC_TIME_BEGIN, m_tmBegin);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_tmEnd);
	DDX_Check(pDX, IDC_KMONTH, m_bKMonth);
	DDX_Check(pDX, IDC_KWEEK, m_bKWeek);
	DDX_Check(pDX, IDC_KDAY, m_bKDay);
	DDX_Check(pDX, IDC_KMIN60, m_bKMin60);
	DDX_Check(pDX, IDC_KMIN30, m_bKMin30);
	DDX_Check(pDX, IDC_KMIN15, m_bKMin15);
	DDX_Check(pDX, IDC_KMIN5, m_bKMin5);
	DDX_Check(pDX, IDC_AUTORESUMEDR, m_bAutoResumeDR);
	DDX_DateTimeCtrl(pDX, IDC_TIME_AUTORESUMEDRBEGIN, m_tmAutoResumeDRBegin);
	DDX_Text(pDX, IDC_AUTORESUMEDR_LIMIT, m_nAutoResumeDRLimit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportOption, CPropertyPageEx)
	//{{AFX_MSG_MAP(CExportOption)
	ON_BN_CLICKED(IDC_TIME_ALL, OnTimeAll)
	ON_BN_CLICKED(IDC_TIME_DEFINE, OnTimeDefine)
	ON_BN_CLICKED(IDC_NORESUMEDR, OnNoresumedr)
	ON_BN_CLICKED(IDC_RESUMEDRUP, OnResumedrup)
	ON_BN_CLICKED(IDC_RESUMEDRDOWN, OnResumedrdown)
	ON_BN_CLICKED(IDC_AUTORESUMEDR, OnAutoresumedr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportOption message handlers

BOOL CExportOption::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );

	// select item
	int	nCheckID	=	IDC_TIME_ALL;
	if( m_bTimeDefine )
		nCheckID	=	IDC_TIME_DEFINE;
	CheckRadioButton( IDC_TIME_ALL, IDC_TIME_DEFINE, nCheckID );
	m_ctrlTimeBegin.EnableWindow( m_bTimeDefine );
	m_ctrlTimeEnd.EnableWindow( m_bTimeDefine );

	// auto resume dr option
	nCheckID	=	IDC_NORESUMEDR;
	if( m_bResumeDRUp )
		nCheckID	=	IDC_RESUMEDRUP;
	else if( m_bResumeDRDown )
		nCheckID	=	IDC_RESUMEDRDOWN;
	CheckRadioButton( IDC_NORESUMEDR, IDC_RESUMEDRDOWN, nCheckID );
	m_btnAutoResumeDR.EnableWindow( m_bAutoResumeDR && IDC_NORESUMEDR != nCheckID );
	m_ctrlTimeAutoResumeDRBegin.EnableWindow( m_bAutoResumeDR && IDC_NORESUMEDR != nCheckID );
	m_editAutoResumeDRLimit.EnableWindow( m_bAutoResumeDR && IDC_NORESUMEDR != nCheckID );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExportOption::OnTimeAll() 
{
	// TODO: Add your control notification handler code here
	m_ctrlTimeBegin.EnableWindow( FALSE );
	m_ctrlTimeEnd.EnableWindow( FALSE );
}

void CExportOption::OnTimeDefine() 
{
	// TODO: Add your control notification handler code here
	m_ctrlTimeBegin.EnableWindow( TRUE );
	m_ctrlTimeEnd.EnableWindow( TRUE );
}

void CExportOption::OnNoresumedr() 
{
	// TODO: Add your control notification handler code here
	m_btnAutoResumeDR.EnableWindow( FALSE );
	m_ctrlTimeAutoResumeDRBegin.EnableWindow( FALSE );
	m_editAutoResumeDRLimit.EnableWindow( FALSE );
}

void CExportOption::OnResumedrup() 
{
	// TODO: Add your control notification handler code here
	m_btnAutoResumeDR.EnableWindow( TRUE );
	m_ctrlTimeAutoResumeDRBegin.EnableWindow( TRUE );
	m_editAutoResumeDRLimit.EnableWindow( TRUE );
}

void CExportOption::OnResumedrdown() 
{
	// TODO: Add your control notification handler code here
	m_btnAutoResumeDR.EnableWindow( TRUE );
	m_ctrlTimeAutoResumeDRBegin.EnableWindow( TRUE );
	m_editAutoResumeDRLimit.EnableWindow( TRUE );
}

void CExportOption::OnAutoresumedr() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_btnAutoResumeDR.GetCheck();
	m_ctrlTimeAutoResumeDRBegin.EnableWindow( nCheck );
	m_editAutoResumeDRLimit.EnableWindow( nCheck );
}

BOOL CExportOption::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		pSheet->SetWizardButtons( m_dwButtonFlags );
		
		CWnd * pWnd = pSheet->GetDlgItem( ID_WIZNEXT );
		if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		{
			CString	strTemp;
			strTemp.LoadString( IDS_PROPERTYSHEET_NEXT );
			pWnd->SetWindowText( strTemp );
		}
	}

	return CPropertyPageEx::OnSetActive();
}

LRESULT CExportOption::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPageEx::OnWizardBack();
}

LRESULT CExportOption::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );

	// Time Radio
	CButton * pbtn		= (CButton *)GetDlgItem( IDC_TIME_ALL );
	if( pbtn )	m_bTimeAll	=	pbtn->GetCheck();
	pbtn		= (CButton *)GetDlgItem( IDC_TIME_DEFINE );
	if( pbtn )	m_bTimeDefine	=	pbtn->GetCheck();
	if( !m_bTimeAll && !m_bTimeDefine )
		m_bTimeAll	=	TRUE;

	// Resume Radio
	pbtn		= (CButton *)GetDlgItem( IDC_NORESUMEDR );
	if( pbtn )	m_bNoResumeDR	=	pbtn->GetCheck();
	pbtn		= (CButton *)GetDlgItem( IDC_RESUMEDRUP );
	if( pbtn )	m_bResumeDRUp	=	pbtn->GetCheck();
	pbtn		= (CButton *)GetDlgItem( IDC_RESUMEDRDOWN );
	if( pbtn )	m_bResumeDRDown	=	pbtn->GetCheck();
	if( !m_bNoResumeDR && !m_bResumeDRUp && !m_bResumeDRDown )
		m_bNoResumeDR	=	TRUE;
	
	// Store profile
	StoreProfile( );

	return CPropertyPageEx::OnWizardNext();
}


BOOL CExportOption::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
		return TRUE;
	return CPropertyPageEx::PreTranslateMessage(pMsg);
}
