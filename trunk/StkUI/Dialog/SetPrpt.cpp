// SetPrpt.cpp : implementation file
//

#include "stdafx.h"

#include "SetPrpt.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetPrpt property page

IMPLEMENT_DYNCREATE(CSetPrpt, CPropertyPageEx)

CSetPrpt::CSetPrpt( ) : CPropertyPageEx(CSetPrpt::IDD)
{
	//{{AFX_DATA_INIT(CSetPrpt)
	m_strName = _T("");
	m_strPathName	=	_T("");
	m_strDescript = _T("");
	//}}AFX_DATA_INIT
	m_bAutoChangeTitle	=	TRUE;
	m_bAutoChangePath	=	TRUE;
	m_pStrategy			=	NULL;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
}

CSetPrpt::CSetPrpt( UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle )
		 : CPropertyPageEx(CSetPrpt::IDD,nIDCaption, nIDTitle, nIDSubTitle)
{
	m_strName = _T("");
	m_strPathName	=	_T("");
	m_strDescript = _T("");

	m_bAutoChangeTitle	=	TRUE;
	m_bAutoChangePath	=	TRUE;
	m_pStrategy			=	NULL;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
}

CSetPrpt::~CSetPrpt()
{
}

void CSetPrpt::SetStrategy( CStrategy * pStrategy )
{
	m_pStrategy	=	pStrategy;
}

void CSetPrpt::SetName( LPCTSTR lpszName )
{
	m_strName	=	lpszName;
	m_bAutoChangeTitle	=	FALSE;
	m_bAutoChangePath	=	FALSE;
}

CString CSetPrpt::GetName( )
{
	return m_strName;
}

void CSetPrpt::SetDescript( LPCTSTR lpszDescript )
{
	m_strDescript	=	lpszDescript;
}

CString CSetPrpt::GetDescript( )
{
	return m_strDescript;
}

void CSetPrpt::SetPathName( LPCTSTR lpszPathName )
{
	m_strPathName	=	lpszPathName;
	m_bAutoChangeTitle	=	FALSE;
	m_bAutoChangePath	=	FALSE;
}

CString	CSetPrpt::GetPathName( )
{
	return m_strPathName;
}

CString	CSetPrpt::GetPathName( CString strDirName, CString strTitle, CString strFileExt )
{
	int nLen = strDirName.GetLength();
	if( nLen > 0 &&
		( strDirName[nLen-1] == '\\' || strDirName[nLen-1] == '/' ) )
		strDirName	=	strDirName.Left( strDirName.GetLength()-1 );
	return strDirName + STRING_DIRSEP + strTitle + strFileExt;
}

BOOL CSetPrpt::ValidData( )
{
	if( m_strName.IsEmpty() )
	{
		AfxMessageBox( IDS_SETPRPT_INVALIDTITLE, MB_OK | MB_ICONINFORMATION );
		return FALSE;
	}

	CString strDirName	=	AfxGetFileDirectoryExist( m_strPathName, "" );
	if( m_strPathName.IsEmpty() || 
		(!strDirName.IsEmpty() && 0 != access(strDirName, 0)) )
	{
		AfxMessageBox( IDS_SETPRPT_INVALIDPATH, MB_OK | MB_ICONINFORMATION );
		return FALSE;
	}

	return TRUE;
}

BOOL CSetPrpt::LoadFromStrategy( )
{
	if( m_pStrategy )
	{
		SetName( m_pStrategy->GetName() );
		SetDescript( m_pStrategy->GetDescript() );
		SetPathName( m_pStrategy->GetPathName() );
		return TRUE;
	}
	return FALSE;
}

BOOL CSetPrpt::StoreToStrategy( )
{
	if( m_pStrategy )
	{
		m_pStrategy->SetName( GetName() );
		m_pStrategy->SetDescript( GetDescript() );
		m_pStrategy->SetPathName( GetPathName() );
		return TRUE;
	}
	return FALSE;
}

void CSetPrpt::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetPrpt)
	DDX_Control(pDX, IDC_EXPLORE, m_btnExplore);
	DDX_Control(pDX, IDC_DESCRIPT, m_editDescript);
	DDX_Text(pDX, IDC_TITLE, m_strName);
	DDV_MaxChars(pDX, m_strName, STRATEGY_MAX_TITLE);
	DDX_Text(pDX, IDC_FILENAME, m_strPathName);
	DDX_Text(pDX, IDC_DESCRIPT, m_strDescript);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetPrpt, CPropertyPageEx)
	//{{AFX_MSG_MAP(CSetPrpt)
	ON_BN_CLICKED(IDC_EXPLORE, OnExplore)
	ON_EN_CHANGE(IDC_TITLE, OnChangeTitle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetPrpt message handlers
BOOL CSetPrpt::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();

	ASSERT( m_pStrategy );

	LoadFromStrategy( );

	CStkUIApp * pApp = AfxGetStkUIApp();
	if( pApp && m_strName.IsEmpty() && m_strPathName.IsEmpty() )
	{
		CString	strDirName	=	pApp->GetLastStrategyDirectory( );
		m_strName		=	pApp->GetNextNewStrategyTitle( m_strFileExt, strDirName );
		m_strPathName	=	GetPathName( strDirName, m_strName, m_strFileExt );

		if( m_strName.IsEmpty() )
			m_strName.LoadString( IDS_STRATEGY_NONAME );
		if( m_strPathName.IsEmpty() )
			m_strPathName	=	m_strName + m_strFileExt;
	}

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetPrpt::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return FALSE;
	StoreToStrategy( );
	return CPropertyPageEx::OnKillActive();
}

LRESULT CSetPrpt::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return -1L;
	StoreToStrategy( );
	return CPropertyPageEx::OnWizardBack();
}

LRESULT CSetPrpt::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return -1L;
	StoreToStrategy( );
	return CPropertyPageEx::OnWizardNext();
}

BOOL CSetPrpt::OnWizardFinish() 
{
	// Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return FALSE;
	StoreToStrategy( );
	return CPropertyPageEx::OnWizardFinish();
}

void CSetPrpt::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return;
	StoreToStrategy( );
	CPropertyPageEx::OnOK();
}

void CSetPrpt::OnExplore() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoPromptFileName( m_strPathName, IDS_OPENFILE,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING, FALSE ) )
	{
		CString	strTitle	=	AfxGetFileTitleNoExt( m_strPathName );
		if( m_strName.IsEmpty() || m_bAutoChangeTitle )
			m_strName	=	strTitle;
		if( m_strName.IsEmpty() )
			m_strName.LoadString( IDS_STRATEGY_NONAME );

		UpdateData( FALSE );

		DWORD dwAttrib = GetFileAttributes(m_strPathName);
		if (dwAttrib & FILE_ATTRIBUTE_READONLY)
		{
			CFile	file;
			if( file.Open( m_strPathName, CFile::modeCreate ) )
				file.Close();
		}

		m_bAutoChangePath	=	FALSE;
	}
}

void CSetPrpt::OnChangeTitle() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPageEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData( );
	
	if( m_bAutoChangePath && !m_strName.IsEmpty() )
	{
		CString	strDirName	=	AfxGetFileDirectoryExist( m_strPathName, "" );
		m_strPathName		=	GetPathName( strDirName, m_strName, m_strFileExt );
	}

	UpdateData( FALSE );

	m_bAutoChangeTitle	=	FALSE;
}


BOOL CSetPrpt::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam 
		&& pMsg->hwnd != m_editDescript.GetSafeHwnd() )
		return TRUE;
	return CPropertyPageEx::PreTranslateMessage(pMsg);
}

BOOL CSetPrpt::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		pSheet->SetWizardButtons( m_dwButtonFlags );
	}
	
	return CPropertyPageEx::OnSetActive();
}
