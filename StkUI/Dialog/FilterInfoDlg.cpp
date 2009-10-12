// StockFilterDlg.cpp : implementation file
//

#include "stdafx.h"

#include "FilterInfoDlg.h"
#include "SelectGrpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterInfoDlg dialog


CFilterInfoDlg::CFilterInfoDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CFilterInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterInfoDlg)
	m_strMoreThan = _T("");
	m_strLessThan = _T("");
	//}}AFX_DATA_INIT
}

BOOL CFilterInfoDlg::InitListInfo( )
{
	m_listInfo.ResetContent();

	CSPDWordArray & anOrder	=	AfxGetProfile().GetSListColumnsOrder();
	for( int i=0; i<anOrder.GetSize(); i++ )
	{
		UINT	nVariantID	=	anOrder.GetAt(i);
		if( SLH_CODE == nVariantID || SLH_NAME == nVariantID
			|| SLH_DATE == nVariantID || SLH_REPORTTYPE == nVariantID
			|| SLH_DATE_BEGIN == nVariantID || SLH_TRADE == nVariantID
			|| SLH_PROVINCE == nVariantID )
			continue;

		CString	strName	=	(LPCTSTR)AfxGetVariantName( nVariantID, TRUE );
		int nItem	=	m_listInfo.AddString( strName );
		m_listInfo.SetItemData( nItem, nVariantID );
	}
	m_listInfo.SetCurSel( 0 );
	return TRUE;
}

void CFilterInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterInfoDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_ADDTOGROUP, m_btnAddToGroup);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_STATICTIME, m_staticTime);
	DDX_Control(pDX, IDC_SZNB, m_btnSznb);
	DDX_Control(pDX, IDC_SZNA, m_btnSzna);
	DDX_Control(pDX, IDC_SHAB, m_btnShab);
	DDX_Control(pDX, IDC_SHAA, m_btnShaa);
	DDX_Control(pDX, IDC_ALL, m_btnAll);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_LISTRESULT, m_listResult);
	DDX_Control(pDX, IDC_LISTCOND, m_listCond);
	DDX_Control(pDX, IDC_LISTINFO, m_listInfo);
	DDX_Text(pDX, IDC_EDITMORETHAN, m_strMoreThan);
	DDX_Text(pDX, IDC_EDITLESSTHAN, m_strLessThan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterInfoDlg, CXFDialog)
	//{{AFX_MSG_MAP(CFilterInfoDlg)
	ON_LBN_SELCHANGE(IDC_LISTINFO, OnSelchangeListinfo)
	ON_LBN_SELCHANGE(IDC_LISTCOND, OnSelchangeListcond)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_ADDTOGROUP, OnAddtogroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterInfoDlg message handlers

BOOL CFilterInfoDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();

	DWORD	dwDate	=	0;
	if( AfxGetStockContainer().GetCurrentType( NULL, NULL, &dwDate )
		&& (-1 != dwDate || AfxGetStockContainer().GetLatestTechDate(&dwDate)) )
	{
		CSPTime	sptime;
		if( -1 != dwDate && sptime.FromStockTimeDay( dwDate ) )
		{
			CString	string	=	sptime.Format( "%Y-%m-%d" );
			m_staticTime.SetWindowText( string );
		}
	}
	
	InitListInfo( );
	OnSelchangeListinfo();

	CheckRadioButton( IDC_RADIOAND, IDC_RADIOOR, IDC_RADIOAND );

	m_btnAll.SetCheck( 1 );
	OnAll( );

	m_btnAddToGroup.EnableWindow( FALSE );

	m_ctrlProgress.ShowWindow( SW_HIDE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterInfoDlg::OnCancel() 
{
	for( int i=0; i<m_listCond.GetCount(); i++ )
	{
		FILTERINFO_CONDITION * pCond = (FILTERINFO_CONDITION *)m_listCond.GetItemData(i);
		if( NULL != pCond )
			delete	pCond;
	}
	m_listCond.ResetContent();

	CXFDialog::OnCancel();
}

void CFilterInfoDlg::OnSelchangeListinfo() 
{
	UpdateData( );

	m_strMoreThan	=	_T("");
	m_strLessThan	=	_T("");

	int	nSel	=	m_listInfo.GetCurSel();
	if( LB_ERR == nSel )
		return;
	UINT	nVariantID	=	m_listInfo.GetItemData(nSel);

	for( int i=0; i<m_listCond.GetCount(); i++ )
	{
		FILTERINFO_CONDITION * pCond = (FILTERINFO_CONDITION *)m_listCond.GetItemData(i);
		if( NULL != pCond )
		{
			if( pCond->nVariantID == nVariantID )
			{
				m_listCond.SetCurSel( i );
				if( pCond->bHasMoreThan )
				{
					m_strMoreThan.Format( "%f", pCond->dMoreThan );
					while( m_strMoreThan.GetLength() > 1 && m_strMoreThan[m_strMoreThan.GetLength()-1] == '0' )
						m_strMoreThan	=	m_strMoreThan.Left( m_strMoreThan.GetLength()-1 );
				}
				if( pCond->bHasLessThan )
				{
					m_strLessThan.Format( "%f", pCond->dLessThan );
					while( m_strLessThan.GetLength() > 1 && m_strLessThan[m_strLessThan.GetLength()-1] == '0' )
						m_strLessThan	=	m_strLessThan.Left( m_strLessThan.GetLength()-1 );
				}
				break;
			}
		}
	}

	UpdateData( FALSE );
}

void CFilterInfoDlg::OnSelchangeListcond() 
{
	UpdateData( );

	m_strMoreThan	=	_T("");
	m_strLessThan	=	_T("");

	int	nSel	=	m_listCond.GetCurSel();
	if( LB_ERR == nSel )
		return;
	FILTERINFO_CONDITION * pCond = (FILTERINFO_CONDITION *)m_listCond.GetItemData(nSel);
	
	if( NULL != pCond )
	{
		for( int i=0; i<m_listInfo.GetCount(); i++ )
		{
			UINT	nVariantID	=	m_listInfo.GetItemData(i);
			if( pCond->nVariantID == nVariantID )
			{
				m_listInfo.SetCurSel( i );
				if( pCond->bHasMoreThan )
				{
					m_strMoreThan.Format( "%f", pCond->dMoreThan );
					while( m_strMoreThan.GetLength() > 1 && m_strMoreThan[m_strMoreThan.GetLength()-1] == '0' )
						m_strMoreThan	=	m_strMoreThan.Left( m_strMoreThan.GetLength()-1 );
				}
				if( pCond->bHasLessThan )
				{
					m_strLessThan.Format( "%f", pCond->dLessThan );
					while( m_strLessThan.GetLength() > 1 && m_strLessThan[m_strLessThan.GetLength()-1] == '0' )
						m_strLessThan	=	m_strLessThan.Left( m_strLessThan.GetLength()-1 );
				}
				break;
			}
		}
	}

	UpdateData( FALSE );
}

void CFilterInfoDlg::OnAdd() 
{
	UpdateData( );

	int	nSel	=	m_listInfo.GetCurSel();
	if( LB_ERR == nSel )
	{
		AfxMessageBox( IDS_FILTERINFO_NOSELECTEDINFO, MB_OK|MB_ICONINFORMATION );
		return;
	}

	if( ( m_strMoreThan.GetLength()<=0 && m_strLessThan.GetLength()<=0 )
		|| !IsNumber(m_strMoreThan,TRUE) || !IsNumber(m_strLessThan,TRUE) )
	{
		AfxMessageBox( IDS_FILTERINFO_INVALIDCOND, MB_OK|MB_ICONINFORMATION );
		return;
	}

	UINT nVariantID	=	m_listInfo.GetItemData(nSel);
	CString	strCond	=	AfxGetVariantName( nVariantID, TRUE );
	CString	strME, strAnd, strLE;
	strME.LoadString( IDS_FILTERINFO_ME );
	strAnd.LoadString( IDS_FILTERINFO_AND );
	strLE.LoadString( IDS_FILTERINFO_LE );
	strCond	+=	"(";
	if( m_strMoreThan.GetLength() > 0 )
		strCond	+=	strME + m_strMoreThan;
	if( m_strMoreThan.GetLength() > 0 && m_strLessThan.GetLength() > 0 )
		strCond	+=	strAnd;
	if( m_strLessThan.GetLength() > 0 )
		strCond	+=	strLE + m_strLessThan;
	strCond	+=	")";

	int i;
	for( i=0; i<m_listCond.GetCount(); i++ )
	{
		FILTERINFO_CONDITION * pCond = (FILTERINFO_CONDITION *)m_listCond.GetItemData(i);
		if( NULL != pCond )
		{
			if( pCond->nVariantID == nVariantID )
			{
				pCond->bHasMoreThan	=	( m_strMoreThan.GetLength()>0 ? TRUE : FALSE );
				pCond->bHasLessThan	=	( m_strLessThan.GetLength()>0 ? TRUE : FALSE );
				pCond->dMoreThan	=	atof(m_strMoreThan);
				pCond->dLessThan	=	atof(m_strLessThan);
				m_listCond.DeleteString( i );
				m_listCond.InsertString( i, strCond );
				m_listCond.SetItemData( i, (DWORD)pCond );
				m_listCond.SetCurSel( i );
				break;
			}
		}
	}
	if( i == m_listCond.GetCount() )
	{
		FILTERINFO_CONDITION * pCond = new FILTERINFO_CONDITION;
		if( pCond )
		{
			pCond->nVariantID	=	nVariantID;
			pCond->bHasMoreThan	=	( m_strMoreThan.GetLength()>0 ? TRUE : FALSE );
			pCond->bHasLessThan	=	( m_strLessThan.GetLength()>0 ? TRUE : FALSE );
			pCond->dMoreThan	=	atof(m_strMoreThan);
			pCond->dLessThan	=	atof(m_strLessThan);
			int nItem	=	m_listCond.AddString( strCond );
			m_listCond.SetItemData( nItem, (DWORD)pCond );
			m_listCond.SetCurSel( nItem );
		}
	}
}

void CFilterInfoDlg::OnRemove() 
{
	int	nSel	=	m_listCond.GetCurSel();
	if( LB_ERR == nSel )
	{
		AfxMessageBox( IDS_FILTERINFO_NOSELECTEDCOND, MB_OK|MB_ICONINFORMATION );
		return;
	}

	FILTERINFO_CONDITION * pCond = (FILTERINFO_CONDITION *)m_listCond.GetItemData(nSel);
	
	if( NULL != pCond )
		delete	pCond;

	m_listCond.DeleteString( nSel );
	nSel	=	nSel >= m_listCond.GetCount() ? 0 : nSel;
	m_listCond.SetCurSel( nSel );
	OnSelchangeListcond( );
}

void CFilterInfoDlg::OnAll() 
{
	int	nCheck	=	m_btnAll.GetCheck( );
	m_btnShaa.EnableWindow( !nCheck );
	m_btnShab.EnableWindow( !nCheck );
	m_btnSzna.EnableWindow( !nCheck );
	m_btnSznb.EnableWindow( !nCheck );
}

void CFilterInfoDlg::OnStart() 
{
	UpdateData( );

	if( m_listCond.GetCount() <= 0 )
	{
		AfxMessageBox( IDS_FILTERINFO_NOCOND, MB_OK|MB_ICONINFORMATION );
		return;
	}

	BOOL	bAnd	=	TRUE;
	CButton * pbtn	=	(CButton *)GetDlgItem(IDC_RADIOAND);
	if( pbtn )	bAnd	=	pbtn->GetCheck();

	m_listResult.ResetContent();

	CStockContainer & container	=	AfxGetStockContainer();
	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE );
	m_ctrlProgress.SetRange( 0, container.GetSize() );
	for( int i=0; i<container.GetSize(); i++ )
	{
		m_ctrlProgress.SetPos( i );

		CStockInfo	&	info	=	container.ElementAt(i);
		LONG	stocktype	=	info.GetType();
		if( CStock::typeshA == stocktype )
		{	if( !m_btnAll.GetCheck() && !m_btnShaa.GetCheck() )	continue;	}
		else if( CStock::typeshB == stocktype )
		{	if( !m_btnAll.GetCheck() && !m_btnShab.GetCheck() )	continue;	}
		else if( CStock::typeszA == stocktype )
		{	if( !m_btnAll.GetCheck() && !m_btnSzna.GetCheck() )	continue;	}
		else if( CStock::typeszB == stocktype )
		{	if( !m_btnAll.GetCheck() && !m_btnSznb.GetCheck() )	continue;	}
		else
		{	continue;	}

		BOOL	bSelected	=	bAnd;
		for( int k=0; k<m_listCond.GetCount(); k++ )
		{
			FILTERINFO_CONDITION * pCond = (FILTERINFO_CONDITION *)m_listCond.GetItemData(k);
			double	dValue	=	0;
			BOOL	bOK		=	FALSE;
			if( pCond && AfxGetVariantValue( pCond->nVariantID, info, &dValue, &container ) )
			{
				bOK	=	TRUE;
				if( pCond->bHasMoreThan && dValue < pCond->dMoreThan )
					bOK	=	FALSE;
				if( pCond->bHasLessThan && dValue > pCond->dLessThan )
					bOK	=	FALSE;
			}

			if( bAnd && !bOK )
			{
				bSelected	=	FALSE;
				break;
			}
			if( !bAnd && bOK )
			{
				bSelected	=	TRUE;
				break;
			}
		}

		if( bSelected )
		{
			int nItem = m_listResult.AddString( CString(info.GetStockCode()) + "(" + info.GetStockName() + ")" );
			m_listResult.SetItemData( nItem, i );
		}
	}
	m_ctrlProgress.SetPos( container.GetSize() );
	
	m_btnAddToGroup.EnableWindow( TRUE );
}

void CFilterInfoDlg::OnAddtogroup() 
{
	CSelectGrpDlg	dlg;
	dlg.SetMode( CSelectGrpDlg::modeGroup );
	if( IDOK == dlg.DoModal() )
	{
		CString	strGrpName	=	dlg.GetSelectedName();
		CDomainContainer & groups	=	AfxGetGroupContainer();

		CSPStringArray	astrStocks;
		astrStocks.SetSize( 0, m_listResult.GetCount() );
		for( int i=0; i<m_listResult.GetCount(); i++ )
		{
			CString	rString;
			m_listResult.GetText( i, rString );
			if( rString.GetLength() > 0 )
			{
				int nIndex = rString.Find( "(" );
				if( -1 != nIndex )
					rString	=	rString.Left(nIndex);
				astrStocks.Add( rString );
			}
		}
		
		groups.AddDomainStock( strGrpName, astrStocks );
	}
}

