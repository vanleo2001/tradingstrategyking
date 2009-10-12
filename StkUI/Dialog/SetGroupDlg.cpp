// SetGroupDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SetGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDomainComboBox

CDomainComboBox::CDomainComboBox()
{
}

CDomainComboBox::~CDomainComboBox()
{
}

int CDomainComboBox::InitStrings( BOOL bHasClass, BOOL bHasDomain, CDomainContainer & groups )
{
	ResetContent( );

	CDomainContainer & domains = AfxGetDomainContainer( );
	CStockContainer & container = AfxGetStockContainer();

	int	nItemCount	=	groups.GetSize();
	if( bHasClass )		nItemCount	+=	8;
	if( bHasDomain )	nItemCount	+=	domains.GetSize();
	
	// Init Storage
	InitStorage( nItemCount, 32 );
	CString	strGroup;

	if( bHasClass )
	{
		strGroup.LoadString( IDS_DOMAIN_ALL );
		int nItem = AddString( strGroup );
		SetItemData( nItem, CStockContainer::typeAll );

		strGroup.LoadString( IDS_DOMAIN_INDEX );
		nItem = AddString( strGroup );
		SetItemData( nItem, CStockContainer::typeIndex );
		
		strGroup.LoadString( IDS_DOMAIN_CLASSSHAA );
		nItem = AddString( strGroup );
		SetItemData( nItem, CStockContainer::typeClassShaa );

		strGroup.LoadString( IDS_DOMAIN_CLASSSHAB );
		nItem = AddString( strGroup );
		SetItemData( nItem, CStockContainer::typeClassShab );

		strGroup.LoadString( IDS_DOMAIN_CLASSSZNA );
		nItem = AddString( strGroup );
		SetItemData( nItem, CStockContainer::typeClassSzna );

		strGroup.LoadString( IDS_DOMAIN_CLASSSZNB );
		nItem = AddString( strGroup );
		SetItemData( nItem, CStockContainer::typeClassSznb );

		strGroup.LoadString( IDS_DOMAIN_CLASSSHABOND );
		nItem = AddString( strGroup );
		SetItemData( nItem, CStockContainer::typeClassShabond );

		strGroup.LoadString( IDS_DOMAIN_CLASSSZNBOND );
		nItem = AddString( strGroup );
		SetItemData( nItem, CStockContainer::typeClassSznbond );
	}

	// Domains
	if( bHasDomain )
	{
		for( int i=0; i<domains.GetSize(); i++ )
		{
			CDomain	& domain = domains.ElementAt(i);
			int nItem = AddString( domain.m_strName );
			SetItemData( nItem, CStockContainer::typeDomain );
		}
	}

	// Groups
	for( int i=0; i<groups.GetSize(); i++ )
	{
		CDomain	& domain = groups.ElementAt(i);
		int nItem = AddString( domain.m_strName );
		SetItemData( nItem, CStockContainer::typeGroup );
	}

	return nItemCount;
}

int CDomainComboBox::AddGroup( CString strGroup )
{
	int nItem = AddString( strGroup );
	SetItemData( nItem, CStockContainer::typeGroup );
	return nItem;
}

BOOL CDomainComboBox::DeleteCurSel( )
{
	int nSel = GetCurSel();
	if( CB_ERR != nSel )
	{
		DeleteString( nSel );
		return TRUE;
	}
	return FALSE;
}

int CDomainComboBox::GetCurSelType( )
{
	int nSel = GetCurSel();
	if( CB_ERR != nSel )
		return GetItemData( nSel );
	return -1;
}

CString CDomainComboBox::GetCurSelName( )
{
	int nSel = GetCurSel();
	if( CB_ERR != nSel )
	{
		CString	rString;
		GetLBText( nSel, rString );
		return rString;
	}
	return "";
}

BOOL CDomainComboBox::SelectGroupAll( )
{
	for( int i=0; i<GetCount(); i++ )
	{
		int	nType	=	GetItemData( i );
		if( CStockContainer::typeAll == nType )
		{
			SetCurSel( i );
			return TRUE;
		}
	}
	return FALSE;
}

BEGIN_MESSAGE_MAP(CDomainComboBox, CComboBox)
	//{{AFX_MSG_MAP(CDomainComboBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDomainComboBox message handlers

int CDomainComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDomainListBox

CDomainListBox::CDomainListBox()
{
}

CDomainListBox::~CDomainListBox()
{
}

BOOL CDomainListBox::SetCurrentStocks( int nType, LPCTSTR lpszDomain, CDomainContainer & groups )
{
	// Delete All
	ResetContent( );

	CStockContainer & container = AfxGetStockContainer();
	CStockContainer cntn;

	BOOL	bOK	=	FALSE;
	if( nType == CStockContainer::typeGroup )
	{
		CSPStringArray	astrSpecify;
		if( groups.GetDomainStocks( lpszDomain, astrSpecify ) )
			bOK	=	cntn.RetrieveSpecify( astrSpecify );	//	Not Set dwDate
	}
	else
	{
		bOK	=	cntn.RetrieveFromStatic( nType, lpszDomain, NULL, -1 );
	}

	if( bOK )
	{
		InitStorage( cntn.GetSize(), 32 );
		for( int i=0; i<cntn.GetSize(); i++ )
		{
			CStockInfo & info = cntn.ElementAt(i);
			CString	strItem	=	CString(info.GetStockCode()) + "(" + info.GetStockShortName() + ")    ";
			while( strItem.GetLength() < 16 )	strItem	+=	" ";
			strItem	+= info.GetStockName() ;
			int nItem = AddString( strItem );
		}
	}

	return bOK;
}

BOOL CDomainListBox::SelectCode( int nStart, CString strText )
{
	SelectItem( -1, FALSE );

	int	nLen	=	strText.GetLength();
	if( nLen <= 0 )
		return FALSE;

	CString	rString;
	int	nPos	=	nStart;
	for( int i=0; i<GetCount(); i++ )
	{
		if( nPos >= GetCount() || nPos < 0 )
			nPos	=	0;

		GetText( nPos, rString );
		if( 0 == strncmp( rString, strText, min(nLen,rString.GetLength()) ) )
		{
			SelectItem( i, TRUE );
			SetTopIndex( i > 6 ? i-6 : 0 );
			return TRUE;
		}

		nPos	++;
	}
	return FALSE;
}

BOOL CDomainListBox::SelectShortname( int nStart, CString strText )
{
	SelectItem( -1, FALSE );

	strText.MakeUpper();
	int	nLen	=	strText.GetLength();
	if( nLen <= 0 )
		return FALSE;

	CString	rString;
	int	nPos	=	nStart;
	for( int i=0; i<GetCount(); i++ )
	{
		if( nPos == GetCount() || nPos < 0 )
			nPos	=	0;

		GetText( nPos, rString );
		rString.MakeUpper();
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
		{
			rString	=	rString.Mid( nIndex+1 );
			if( 0 == strncmp( rString, strText, min(nLen,rString.GetLength()) ) )
			{
				SelectItem( i, TRUE );
				SetTopIndex( i > 6 ? i-6 : 0 );
				return TRUE;
			}
		}

		nPos	++;
	}
	return FALSE;
}

BOOL CDomainListBox::SelectUp( )
{
	for( int i=0; i<GetCount(); i++ )
	{
		int nStatus = GetSelected( i );
		if( LB_ERR != nStatus && nStatus )
		{
			SelectItem( -1, FALSE );
			SelectItem( i-1 < 0 ? 0 : i-1 );
			return TRUE;
		}
	}

	if( GetCount() > 0 )
		SelectItem( GetCount()-1, TRUE );

	return TRUE;
}

BOOL CDomainListBox::SelectDown( )
{
	int	nPageCount	=	GetCountPerPage( );

	for( int i=0; i<GetCount(); i++ )
	{
		int nStatus = GetSelected( i );
		if( LB_ERR != nStatus && nStatus )
		{
			SelectItem( -1, FALSE );
			int	nNewSel	=	(i+1 >= GetCount() ? GetCount()-1 : i+1);
			SelectAndShowItem( nNewSel );
			return TRUE;
		}
	}

	if( GetCount() > 0 )
		SelectItem( 0, TRUE );

	return TRUE;
}

// Page Up
BOOL CDomainListBox::SelectPrior( )
{
	int	nPageCount	=	GetCountPerPage( );

	for( int i=0; i<GetCount(); i++ )
	{
		int nStatus = GetSelected( i );
		if( LB_ERR != nStatus && nStatus )
		{
			SelectItem( -1, FALSE );
			SelectItem( i-nPageCount < 0 ? 0 : i-nPageCount );
			return TRUE;
		}
	}

	if( GetCount() > 0 )
		SelectItem( GetCount()-1, TRUE );

	return TRUE;
}

// Page Down
BOOL CDomainListBox::SelectNext( )
{
	int	nPageCount	=	GetCountPerPage( );

	for( int i=0; i<GetCount(); i++ )
	{
		int nStatus = GetSelected( i );
		if( LB_ERR != nStatus && nStatus )
		{
			SelectItem( -1, FALSE );
			int	nNewSel	=	(i+nPageCount >= GetCount() ? GetCount()-1 : i+nPageCount);
			SelectAndShowItem( nNewSel );
			return TRUE;
		}
	}

	if( GetCount() > 0 )
		SelectItem( 0, TRUE );

	return TRUE;
}

int CDomainListBox::GetCountPerPage( )
{
	int	nPageCount	=	6;
	CRect	rectItem, rectClient;
	GetClientRect( &rectClient );
	if( LB_ERR != GetItemRect( 0, &rectItem ) && rectItem.Height() > 0 )
		nPageCount	=	rectClient.Height() / rectItem.Height() - 1;

	return nPageCount;
}

BOOL CDomainListBox::SelectAndShowItem( int nSel )
{
	int	nPageCount	=	GetCountPerPage( );

	SelectItem( nSel, TRUE );
	int nTopIndex = GetTopIndex();
	if( LB_ERR != nTopIndex && nTopIndex+nPageCount<nSel )
		SetTopIndex( nSel-nPageCount );
	return TRUE;
}

int CDomainListBox::SelectItem( int nSel, BOOL bSelected )
{
	if( GetStyle( ) & LBS_MULTIPLESEL )
	{
		return SetSel( nSel, bSelected );
	}
	else
	{
		if( bSelected )
			return SetCurSel( nSel );
		else
		{
			if( GetCurSel() == nSel || -1 == nSel )
				return SetCurSel( -1 );
		}
		return -1;
	}
}

int CDomainListBox::GetSelected( int nItem )
{
	if( GetStyle( ) & LBS_MULTIPLESEL )
	{
		return GetSel( nItem );
	}
	else
	{
		if( -1 != nItem && GetCurSel() == nItem )
			return TRUE;
		return FALSE;
	}
}


BEGIN_MESSAGE_MAP(CDomainListBox, CListBox)
	//{{AFX_MSG_MAP(CDomainListBox)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDomainListBox message handlers

int CDomainListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSetGroupDlg dialog

CSetGroupDlg::CSetGroupDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSetGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetGroupDlg)
	//}}AFX_DATA_INIT
}

void CSetGroupDlg::SetInitialGroup( CString strGroupName )
{
	m_strInitialGroupName	=	strGroupName;
}

void CSetGroupDlg::AddtoGroup( CSPStringArray & astrStocks )
{
	m_astrInitialGroupStocks.RemoveAll( );
	m_astrInitialGroupStocks.Copy( astrStocks );
}

void CSetGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetGroupDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_REMOVEALL, m_btnRemoveAll);
	DDX_Control(pDX, IDC_REMOVESELECTED, m_btnRemoveSelected);
	DDX_Control(pDX, IDC_ADDALL, m_btnAddAll);
	DDX_Control(pDX, IDC_ADDSELECTED, m_btnAddSelected);
	DDX_Control(pDX, IDC_DELETEGROUP, m_btnDeleteGroup);
	DDX_Control(pDX, IDC_ADDGROUP, m_btnAddGroup);
	DDX_Control(pDX, IDC_LIST_STOCKDEST, m_listStockDest);
	DDX_Control(pDX, IDC_COMBO_GROUPDEST, m_comboGroupDest);
	DDX_Control(pDX, IDC_LIST_STOCKSRC, m_listStockSrc);
	DDX_Control(pDX, IDC_COMBO_GROUPSRC, m_comboGroupSrc);
	DDX_Control(pDX, IDC_EDITCODE, m_editCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetGroupDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSetGroupDlg)
	ON_BN_CLICKED(IDC_ADDGROUP, OnAddgroup)
	ON_BN_CLICKED(IDC_DELETEGROUP, OnDeletegroup)
	ON_BN_CLICKED(IDC_ADDSELECTED, OnAddselected)
	ON_BN_CLICKED(IDC_ADDALL, OnAddall)
	ON_BN_CLICKED(IDC_REMOVESELECTED, OnRemoveselected)
	ON_BN_CLICKED(IDC_REMOVEALL, OnRemoveall)
	ON_EN_CHANGE(IDC_EDITCODE, OnChangeEditcode)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUPSRC, OnSelchangeComboGroupsrc)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUPDEST, OnSelchangeComboGroupdest)
	ON_LBN_DBLCLK(IDC_LIST_STOCKSRC, OnDblclkListStocksrc)
	ON_LBN_DBLCLK(IDC_LIST_STOCKDEST, OnDblclkListStockdest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetGroupDlg message handlers

BOOL CSetGroupDlg::OnInitDialog()
{
	CXFDialog::OnInitDialog();
	
	CWaitCursor	waitcursor;

	// TODO: Add extra initialization here
	CDomainContainer & groups = AfxGetGroupContainer( );
	CDomainContainer & domains = AfxGetDomainContainer( );
	CStockContainer & container = AfxGetStockContainer();

	m_grpTemp	=	groups;

	// Src
	m_comboGroupSrc.InitStrings( TRUE, TRUE, groups );
	m_comboGroupSrc.SetCurSel( 0 );
	OnSelchangeComboGroupsrc();

	// Dest
	m_comboGroupDest.InitStrings( FALSE, FALSE, m_grpTemp );
	m_comboGroupDest.SetCurSel( 0 );
	if( m_strInitialGroupName.GetLength() > 0
		&& CB_ERR != m_comboGroupDest.SelectString( 0, m_strInitialGroupName ) )
	{
		for( int k=0; k<m_astrInitialGroupStocks.GetSize(); k++ )
			m_grpTemp.AddDomainStock( m_strInitialGroupName, m_astrInitialGroupStocks.ElementAt(k) );
	}
	OnSelchangeComboGroupdest();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetGroupDlg::OnAddgroup() 
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
		if( -1 != m_grpTemp.AddDomain( strNewName ) )
		{
			int nNew = m_comboGroupDest.AddGroup( strNewName );
			if( CB_ERR != nNew )
			{
				m_comboGroupDest.SetCurSel( nNew );
				OnSelchangeComboGroupdest( );
				strInfo.LoadString( IDS_SETGROUP_ADDSUCCESS );
			}
		}
		else
		{
			strInfo.LoadString( IDS_SETGROUP_GROUPALREADYEXIST );
		}
	}
	
	m_staticInfo.SetWindowText( strInfo );
}

void CSetGroupDlg::OnDeletegroup() 
{
	// TODO: Add your control notification handler code here
	CString	strInfo;
	strInfo.LoadString( IDS_SETGROUP_DELETEFAILED );

	int nSel = m_comboGroupDest.GetCurSel();
	if( CB_ERR != nSel )
	{
		if( IDYES == AfxMessageBox( IDS_SETGROUP_DELETECONFIRM, MB_YESNO | MB_ICONQUESTION ) )
		{
			CString	strName;
			m_comboGroupDest.GetWindowText( strName );
			if( m_grpTemp.RemoveDomain( strName )
				&& CB_ERR != m_comboGroupDest.DeleteString( nSel ) )
			{
				nSel	=	( nSel >= m_comboGroupDest.GetCount() ? 0 : nSel );
				m_comboGroupDest.SetCurSel( nSel );
				OnSelchangeComboGroupdest();
				strInfo.LoadString( IDS_SETGROUP_DELETESUCCESS );
			}
		}
		else
			strInfo.LoadString( IDS_SETGROUP_USERCANCEL );
	}
	else
		strInfo.LoadString( IDS_SETGROUP_NOSELECTEDGROUP );
	
	m_staticInfo.SetWindowText( strInfo );
}

void CSetGroupDlg::OnAddselected() 
{
	// TODO: Add your control notification handler code here
	CString	strDomainName;
	m_comboGroupDest.GetWindowText( strDomainName );
	if( strDomainName.GetLength() <= 0 )
	{
		AfxMessageBox( IDS_SETGROUP_NOSELECTEDGROUP, MB_OK|MB_ICONINFORMATION );
		return;
	}

	m_listStockDest.SelectItem( -1, FALSE );

	for( int i=0; i<m_listStockSrc.GetCount(); i++ )
	{
		int bSel = m_listStockSrc.GetSelected(i);
		if( LB_ERR != bSel && bSel )
		{
			CString	rString, strStockCode;
			m_listStockSrc.GetText(i,rString);
			int	nIndex	=	rString.Find( '(' );
			if( -1 != nIndex )
				strStockCode	=	rString.Left( nIndex );
			
			int	nSelDest	=	-1;
			if( m_grpTemp.AddDomainStock( strDomainName, strStockCode ) )
				nSelDest	=	m_listStockDest.AddString( rString );
			else
				nSelDest	=	m_listStockDest.FindString( -1, rString );
			m_listStockDest.SelectAndShowItem( nSelDest );
		}
	}
	m_editCode.SetWindowText( NULL );
}

void CSetGroupDlg::OnAddall() 
{
	// TODO: Add your control notification handler code here
	CString	strDomainName;
	m_comboGroupDest.GetWindowText( strDomainName );
	if( strDomainName.GetLength() <= 0 )
	{
		AfxMessageBox( IDS_SETGROUP_NOSELECTEDGROUP, MB_OK|MB_ICONINFORMATION );
		return;
	}

	m_listStockDest.SelectItem( -1, FALSE );

	for( int i=0; i<m_listStockSrc.GetCount(); i++ )
	{
		CString	rString, strStockCode;
		m_listStockSrc.GetText(i,rString);
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );
			
		int	nSelDest	=	-1;
		if( m_grpTemp.AddDomainStock( strDomainName, strStockCode ) )
			nSelDest	=	m_listStockDest.AddString( rString );
		else
			nSelDest	=	m_listStockDest.FindString( -1, rString );
		m_listStockDest.SelectItem( nSelDest, TRUE );
	}
	m_editCode.SetWindowText( NULL );
}

void CSetGroupDlg::OnRemoveselected() 
{
	// TODO: Add your control notification handler code here
	CString	strDomainName;
	m_comboGroupDest.GetWindowText( strDomainName );
	if( strDomainName.GetLength() <= 0 )
	{
		AfxMessageBox( IDS_SETGROUP_NOSELECTEDGROUP, MB_OK|MB_ICONINFORMATION );
		m_comboGroupDest.SetCurSel( 0 );
		OnSelchangeComboGroupdest();
		return;
	}

	for( int i=m_listStockDest.GetCount()-1; i>=0; i-- )
	{
		int bSel = m_listStockDest.GetSelected(i);
		if( LB_ERR != bSel && bSel )
		{
			CString	rString, strStockCode;
			m_listStockDest.GetText(i,rString);
			int	nIndex	=	rString.Find( '(' );
			if( -1 != nIndex )
				strStockCode	=	rString.Left( nIndex );

			if( m_grpTemp.RemoveDomainStock( strDomainName, strStockCode ) )
				m_listStockDest.DeleteString( i );
		}
	}
}

void CSetGroupDlg::OnRemoveall() 
{
	// TODO: Add your control notification handler code here
	CString	strDomainName;
	m_comboGroupDest.GetWindowText( strDomainName );
	if( strDomainName.GetLength() <= 0 )
	{
		AfxMessageBox( IDS_SETGROUP_NOSELECTEDGROUP, MB_OK|MB_ICONINFORMATION );
		m_comboGroupDest.SetCurSel( 0 );
		OnSelchangeComboGroupdest();
		return;
	}

	m_grpTemp.RemoveDomainStockAll( strDomainName );
	m_listStockDest.ResetContent( );
}

void CSetGroupDlg::OnDblclkListStocksrc() 
{
	// TODO: Add your control notification handler code here
	CString	strDomainName;
	m_comboGroupDest.GetWindowText( strDomainName );
	if( strDomainName.GetLength() <= 0 )
	{
		AfxMessageBox( IDS_SETGROUP_NOSELECTEDGROUP, MB_OK|MB_ICONINFORMATION );
		return;
	}

	m_listStockDest.SelectItem( -1, FALSE );

	int nCaretIndex = m_listStockSrc.GetCaretIndex();
	if( LB_ERR != nCaretIndex )
	{
		CString	rString, strStockCode;
		m_listStockSrc.GetText(nCaretIndex,rString);
		m_listStockSrc.SelectItem( nCaretIndex, FALSE );
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );
			
		int	nSelDest	=	-1;
		if( m_grpTemp.AddDomainStock( strDomainName, strStockCode ) )
			nSelDest	=	m_listStockDest.AddString( rString );
		else
			nSelDest	=	m_listStockDest.FindString( -1, rString );
		m_listStockDest.SelectAndShowItem( nSelDest );
	}
}

void CSetGroupDlg::OnDblclkListStockdest() 
{
	// TODO: Add your control notification handler code here
	CString	strDomainName;
	m_comboGroupDest.GetWindowText( strDomainName );
	if( strDomainName.GetLength() <= 0 )
	{
		AfxMessageBox( IDS_SETGROUP_NOSELECTEDGROUP, MB_OK|MB_ICONINFORMATION );
		m_comboGroupDest.SetCurSel( 0 );
		OnSelchangeComboGroupdest();
		return;
	}

	int nCaretIndex	=	m_listStockDest.GetCaretIndex();
	if( LB_ERR != nCaretIndex )
	{
		CString	rString, strStockCode;
		m_listStockDest.GetText(nCaretIndex,rString);
		int	nIndex	=	rString.Find( '(' );
		if( -1 != nIndex )
			strStockCode	=	rString.Left( nIndex );

		if( m_grpTemp.RemoveDomainStock( strDomainName, strStockCode ) )
			m_listStockDest.DeleteString( nCaretIndex );
	}
}

void CSetGroupDlg::OnChangeEditcode() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CXFDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString	strText;
	m_editCode.GetWindowText( strText );
	strText.TrimLeft();
	strText.TrimLeft();
	if( strText.GetLength() > 0 )
	{
		int	nSel	=	m_comboGroupSrc.GetCurSel();
		if( CB_ERR == nSel ||
			CStockContainer::typeAll != (int)(m_comboGroupSrc.GetItemData( nSel )) )
		{
			m_comboGroupSrc.SelectGroupAll( );
			OnSelchangeComboGroupsrc();
		}
	}
	if( !m_listStockSrc.SelectCode( 0, strText ) )
		m_listStockSrc.SelectShortname( 0, strText );
}

void CSetGroupDlg::OnSelchangeComboGroupsrc() 
{
	// TODO: Add your control notification handler code here
	int nType = m_comboGroupSrc.GetCurSelType();
	CString strName = m_comboGroupSrc.GetCurSelName();
	m_listStockSrc.SetCurrentStocks( nType, strName, AfxGetGroupContainer() );
}

void CSetGroupDlg::OnSelchangeComboGroupdest() 
{
	// TODO: Add your control notification handler code here
	int nType = m_comboGroupDest.GetCurSelType();
	CString strName = m_comboGroupDest.GetCurSelName();
	m_listStockDest.SetCurrentStocks( nType, strName, m_grpTemp );
}

void CSetGroupDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDomainContainer & groups = AfxGetGroupContainer( );
	groups	=	m_grpTemp;
	groups.Store( AfxGetProfile().GetGroupFile() );

	::PostMessage( AfxGetGroupView()->GetSafeHwnd(), WM_USER_UPDATEGROUPS, 0, 0 );
	::PostMessage( AfxGetMainFrame()->GetSafeHwnd(), WM_USER_UPDATEBARMENU, 0, 0 );

	CXFDialog::OnOK();
}

void CSetGroupDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CXFDialog::OnCancel();
}

BOOL CSetGroupDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message )
	{
		if( VK_UP == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectUp( );
				return TRUE;
			}
		}
		else if( VK_DOWN == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectDown( );
				return TRUE;
			}
		}
		else if( VK_PRIOR == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectPrior( );
				return TRUE;
			}
		}
		else if( VK_NEXT == pMsg->wParam )
		{
			if( ::GetFocus() == m_editCode.GetSafeHwnd() )
			{
				m_listStockSrc.SelectNext( );
				return TRUE;
			}
		}
		else if( VK_RETURN == pMsg->wParam )
		{
			OnAddselected( );
			return TRUE;
		}
	}
	
	return CXFDialog::PreTranslateMessage(pMsg);
}

