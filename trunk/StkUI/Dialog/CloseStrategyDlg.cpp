// CloseStrategyDlg.cpp : implementation file
//

#include "stdafx.h"

#include "CloseStrategyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCloseStrategyDlg dialog


CCloseStrategyDlg::CCloseStrategyDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CCloseStrategyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCloseStrategyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

POSITION CCloseStrategyDlg::GetFirstCloseStrategyPosition( )
{
	return m_listCloseStrategy.GetHeadPosition();
}

CStrategy	* CCloseStrategyDlg::GetNextCloseStrategy( POSITION &rPos )
{
	return (CStrategy*)m_listCloseStrategy.GetNext(rPos);
}


void CCloseStrategyDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCloseStrategyDlg)
	DDX_Control(pDX, IDC_CLOSEALL, m_btnCloseAll);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STRATEGYLIST, m_listStrategy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCloseStrategyDlg, CXFDialog)
	//{{AFX_MSG_MAP(CCloseStrategyDlg)
	ON_BN_CLICKED(IDC_CLOSEALL, OnCloseall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCloseStrategyDlg message handlers

BOOL CCloseStrategyDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listStrategy.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

// set image lists
	m_SmallImageList.Create(IDB_STRATEGY, 16, 1, RGB(0, 255, 0));
	m_listStrategy.SetImageList(&m_SmallImageList, LVSIL_SMALL);

// insert columns
	CString	strTitle, strPathName;
	strTitle.LoadString( IDS_CLOSESTRATEGYDLG_TITLE );
	strPathName.LoadString( IDS_CLOSESTRATEGYDLG_PATHNAME );
	m_listStrategy.InsertColumn( 0, strTitle, LVCFMT_LEFT, 160, 0 );
	m_listStrategy.InsertColumn( 1, strPathName, LVCFMT_LEFT, 210, 1 );

// insert items
	CStkUIApp * pApp = AfxGetStkUIApp();
	CStrategy  * pActiveStrategy =AfxGetActiveStrategy();
	if( pApp )
	{
		int	count	=	0;
		POSITION pos = pApp->GetFirstStrategyPosition( );
		while( NULL != pos )
		{
			CStrategy * pStrategy = pApp->GetNextStrategy( pos );
			int nItem = m_listStrategy.InsertItem( count, pStrategy->GetName(), 0 );
			m_listStrategy.SetItemText( nItem, 1, pStrategy->GetPathName() );
			m_listStrategy.SetItemData( nItem, DWORD(pStrategy) );
			if( pActiveStrategy == pStrategy )
				m_listStrategy.SetCheck( nItem );
			count ++;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCloseStrategyDlg::OnOK() 
{
	// TODO: Add extra validation here
	for( int i=0; i<m_listStrategy.GetItemCount( ); i++ )
	{
		ASSERT( m_listStrategy.GetItemData(i) );
		if( m_listStrategy.GetCheck(i) )
			m_listCloseStrategy.AddTail( (CStrategy *)m_listStrategy.GetItemData(i) );
	}
	
	CXFDialog::OnOK();
}

void CCloseStrategyDlg::OnCloseall() 
{
	// TODO: Add your control notification handler code here
	for( int i=0; i<m_listStrategy.GetItemCount( ); i++ )
	{
		ASSERT( m_listStrategy.GetItemData(i) );
		m_listCloseStrategy.AddTail( (CStrategy *)m_listStrategy.GetItemData(i) );
	}

	CXFDialog::OnOK();
}
