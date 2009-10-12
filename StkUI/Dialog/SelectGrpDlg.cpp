// SelectGrpDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SelectGrpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectGrpDlg dialog


CSelectGrpDlg::CSelectGrpDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSelectGrpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectGrpDlg)
	//}}AFX_DATA_INIT
	m_pSelectedStrategy	=	NULL;
	m_nMode				=	modeDomain;
}

void CSelectGrpDlg::SetMode( int nMode )
{
	m_nMode		=	nMode;
}

CString CSelectGrpDlg::GetSelectedName( )
{
	return m_strSelectedName;
}

CStrategy * CSelectGrpDlg::GetSelectedStrategy( )
{
	return m_pSelectedStrategy;
}

void CSelectGrpDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectGrpDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STATICINFO, m_staticInfo);
	DDX_Control(pDX, IDC_ADDGROUP, m_btnAddGroup);
	DDX_Control(pDX, IDC_LISTGROUP, m_listGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectGrpDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSelectGrpDlg)
	ON_LBN_DBLCLK(IDC_LISTGROUP, OnDblclkListgroup)
	ON_BN_CLICKED(IDC_ADDGROUP, OnAddgroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectGrpDlg message handlers

BOOL CSelectGrpDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	if( modeDomain == m_nMode )
	{
		m_btnAddGroup.ShowWindow( SW_HIDE );

		CDomainContainer & domains = AfxGetDomainContainer();
		for( int i=0; i<domains.GetSize(); i++ )
		{
			CDomain	& domain = domains.ElementAt(i);
			int nItem = m_listGroup.AddString( domain.m_strName );
			m_listGroup.SetItemData( nItem, NULL );
		}
	}
	else if( modeGroup == m_nMode )
	{
		CDomainContainer & groups = AfxGetGroupContainer();
		for( int i=0; i<groups.GetSize(); i++ )
		{
			CDomain	& domain = groups.ElementAt(i);
			int nItem = m_listGroup.AddString( domain.m_strName );
			m_listGroup.SetItemData( nItem, NULL );
		}
	}
	else if( modeStrategy == m_nMode )
	{
		m_btnAddGroup.ShowWindow( SW_HIDE );

		CStkUIApp * pApp = AfxGetStkUIApp();
		if( NULL == pApp )
			return TRUE;
		CStrategy * pActiveStrategy	=	AfxGetActiveStrategy();
		int	nSel	=	LB_ERR;

		POSITION pos	=	pApp->GetFirstStrategyPosition();
		while( pos )
		{
			CStrategy * pStrategy =	pApp->GetNextStrategy( pos );
			if( pStrategy )
			{
				int	nItem = m_listGroup.AddString( pStrategy->GetName() );
				m_listGroup.SetItemData( nItem, (DWORD)pStrategy );
				if( pStrategy == pActiveStrategy )
					nSel	=	nItem;
			}
		}
		m_listGroup.SetCurSel( nSel );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectGrpDlg::OnOK() 
{
	// TODO: Add extra validation here
	int nSel	=	m_listGroup.GetCurSel( );
	if( LB_ERR == nSel )
	{
		m_strSelectedName	=	"";
		m_pSelectedStrategy	=	NULL;
	}
	else
	{
		m_listGroup.GetText( nSel, m_strSelectedName );
		m_pSelectedStrategy	=	(CStrategy *)m_listGroup.GetItemData( nSel );
	}
	CXFDialog::OnOK();
}

void CSelectGrpDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_strSelectedName	=	"";
	m_pSelectedStrategy	=	NULL;

	CXFDialog::OnCancel();
}

void CSelectGrpDlg::OnDblclkListgroup() 
{
	// TODO: Add your control notification handler code here
	OnOK( );
}

void CSelectGrpDlg::OnAddgroup() 
{
	// TODO: Add your control notification handler code here
	CStringDialog	dlg( this, IDS_SETGROUP_INPUTGNAME, IDS_SETGROUP_INPUTGNAME, NULL );
	if( IDOK != dlg.DoModal() )
		return;
	CString	strNewName	=	dlg.GetStringWant( );
	CString	strInfo;
	strInfo.LoadString( IDS_SETGROUP_ADDFAILED );
	if( strNewName.GetLength() > 0 )
	{
		CDomainContainer & groups = AfxGetGroupContainer();
		if( -1 != groups.AddDomain( strNewName ) )
		{
			int nNew = m_listGroup.AddString( strNewName );
			if( CB_ERR != nNew )
			{
				m_listGroup.SetItemData( nNew, NULL );
				m_listGroup.SetCurSel( nNew );
				strInfo.LoadString( IDS_SETGROUP_ADDSUCCESS );

				groups.Store( AfxGetProfile().GetGroupFile() );
				::PostMessage( AfxGetGroupView()->GetSafeHwnd(), WM_USER_UPDATEGROUPS, 0, 0 );
				::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_USER_UPDATEBARMENU, 0, 0 );
			}
		}
		else
		{
			strInfo.LoadString( IDS_SETGROUP_GROUPALREADYEXIST );
		}
	}
	
	m_staticInfo.SetWindowText( strInfo );
}
