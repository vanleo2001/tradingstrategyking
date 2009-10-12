// NetInfoDlg.cpp : implementation file
//

#include "stdafx.h"

#include "NetInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetInfoDlg dialog


CNetInfoDlg::CNetInfoDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CNetInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetInfoDlg)
	m_bNotShowLater = FALSE;
	//}}AFX_DATA_INIT
}


void CNetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetInfoDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_NETINFOURL, m_linkNetInfo);
	DDX_Control(pDX, IDC_EDIT_EXPLAIN, m_editExplain);
	DDX_Check(pDX, IDC_NOTSHOWLATER, m_bNotShowLater);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetInfoDlg, CXFDialog)
	//{{AFX_MSG_MAP(CNetInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetInfoDlg message handlers

BOOL CNetInfoDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CSPString	strNetInfo, strNetInfourl;
	int bShow = 0;
	AfxGetProfile().GetNetInfo( strNetInfo, strNetInfourl, &bShow );

	m_bNotShowLater	=	! bShow;
	m_editExplain.SetWindowText( strNetInfo );

	m_linkNetInfo.SetWindowText( strNetInfourl );
	m_linkNetInfo.SetURL( (LPCTSTR)strNetInfourl );

#ifdef	CLKVER_OEM
	CSPString	strServerDomain = AfxGetProfile().GetMainServerDomain();
	if( 0 == strServerDomain.CompareNoCase( "www.balang.net" )
		|| 0 == strServerDomain.CompareNoCase( "balang.net" )
		|| 0 == strServerDomain.CompareNoCase( "www.zlinker.com" )
		|| 0 == strServerDomain.CompareNoCase( "zlinker.com" )
		|| 0 == strServerDomain.CompareNoCase( "www.ninebulls.com" )
		|| 0 == strServerDomain.CompareNoCase( "ninebulls.com" ) )
	{
		CXFDialog::OnCancel( );
		return FALSE;
	}
#endif

	if( !bShow )
	{
		CXFDialog::OnCancel( );
		return FALSE;
	}

	if( strNetInfo.IsEmpty() && strNetInfourl.IsEmpty() )
	{
		CXFDialog::OnCancel( );
		return FALSE;
	}

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNetInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData( );

	AfxGetProfile().SetNetInfoShowLater( !m_bNotShowLater );
	
	CXFDialog::OnOK();
}
