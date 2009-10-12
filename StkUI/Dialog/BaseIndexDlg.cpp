// BaseIndexDlg.cpp : implementation file
//

#include "stdafx.h"

#include "BaseIndexDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseIndexDlg dialog


CBaseIndexDlg::CBaseIndexDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CBaseIndexDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBaseIndexDlg)
	m_strIndexName = _T("");
	m_strIndexExpress = _T("");
	m_bAddToList = TRUE;
	//}}AFX_DATA_INIT
}

void CBaseIndexDlg::SetIndexInfo( UINT	nID )
{
	CIndexContainer	& acol	=	AfxGetProfile().GetSListColumnsUser( );
	m_index		=	acol.GetIndex( nID );
}

CIndex CBaseIndexDlg::GetIndexInfo( )
{
	return m_index;
}

void CBaseIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseIndexDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_INDEXEXPRESS, m_editIndexExpress);
	DDX_Text(pDX, IDC_INDEXNAME, m_strIndexName);
	DDV_MaxChars(pDX, m_strIndexName, SLMAX_SLHTEXTLEN);
	DDX_Text(pDX, IDC_INDEXEXPRESS, m_strIndexExpress);
	DDX_Check(pDX, IDC_ADDTOLIST, m_bAddToList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaseIndexDlg, CXFDialog)
	//{{AFX_MSG_MAP(CBaseIndexDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseIndexDlg message handlers

BOOL CBaseIndexDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strIndexName.LoadString( IDS_BASEINDEX_NAMEDEFAULT );
	m_strIndexExpress.LoadString( IDS_BASEINDEX_EXPRESSEXAMPLE );

	if( !m_index.IsInvalidID() )
	{
		m_strIndexName		=	m_index.m_strName;
		m_strIndexExpress	=	m_index.GetExpressString( );
	}
	
	UpdateData( FALSE );

	CSPStringArray	aspstr;
	AfxGetVariantNameArray( aspstr, FALSE );
	CStringArray	astr;
	astr.SetSize( 0, aspstr.GetSize() );
	for( int i=0; i<aspstr.GetSize(); i++ )
	{
		if( aspstr.ElementAt(i).GetLength() > 0 )
			astr.Add( CString(aspstr.ElementAt(i)) );
	}
	m_editIndexExpress.SetListStrings( astr );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBaseIndexDlg::OnOK() 
{
	UpdateData( );

	// if( !AfxAssertFunclimitProf( ) )
	//	return;

	if( m_strIndexName.IsEmpty() )
	{
		AfxMessageBox( IDS_BASEINDEX_NONAME, MB_OK | MB_ICONINFORMATION );
		return;
	}

	if( m_strIndexName.GetLength() > SLMAX_SLHTEXTLEN )
	{
		AfxMessageBox( IDS_BASEINDEX_NAMETOOLONG, MB_OK | MB_ICONINFORMATION );
		return;
	}

	m_index.m_strName		=	m_strIndexName;
	m_index.SetExpressString( (LPCTSTR)m_strIndexExpress );

	if( !m_index.CheckExpressValid() )
	{
		AfxMessageBox( m_index.GetLastErrorMsg(), MB_OK | MB_ICONINFORMATION );
		return;
	}

	CIndexContainer	& aindex	= AfxGetProfile().GetSListColumnsUser( );
	CSPDWordArray	& anOrder	= AfxGetProfile().GetSListColumnsOrder( );
	CSPDWordArray	& anShow	= AfxGetProfile().GetSListColumnsShow( );
	if( m_index.IsInvalidID() )
	{
		m_index.SetNextID( );
		aindex.Add( m_index );
		anOrder.Add( m_index.m_nID );
	}
	else
	{
		aindex.EditIndex( m_index );
	}

	if( m_bAddToList )
	{
		int k;
		for( k=0; k<anShow.GetSize(); k++ )
		{
			if( m_index.m_nID == anShow[k] )
				break;
		}
		if( k == anShow.GetSize() )
			anShow.Add( m_index.m_nID );
	}

	CXFDialog::OnOK();
}

