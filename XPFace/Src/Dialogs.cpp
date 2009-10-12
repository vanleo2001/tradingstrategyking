// Dialogs.cpp : implementation file
//

#include "stdafx.h"
#include "Dialogs.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog

CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/, BOOL bEnableCancel, BOOL bTopMost, CRect rect )
	: CXFResDialog(CWaitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitDlg)
	//}}AFX_DATA_INIT
	m_bEnableCancel	=	bEnableCancel;
	m_bTopMost	=	bTopMost;
	m_bCanceled	=	false;
	m_rectPosition	=	rect;

	CXFResDialog::Create( CWaitDlg::IDD, pParent );

	MSG		msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		AfxGetApp()->PumpMessage();
}


void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFResDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_XFSTATIC_PRO, m_Static);
	DDX_Control(pDX, IDC_XFPROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}

void CWaitDlg::SetProgress(int nPos)
{
	if( !::IsWindow(m_Progress.GetSafeHwnd()) )
		return;

	int	nLower = 0, nUpper = 0;
	m_Progress.GetRange( nLower, nUpper );

	if( nLower < nUpper && nPos >= nLower && nPos <= nUpper )
	{
		double x = 100.*(nPos-nLower)/(nUpper-nLower);

		CString strMsgNew, strMsgOld;
		CString	strTemp;
		strTemp	=	AfxModuleLoadString( IDS_XFHASFINISHED );

		// static
		m_Static.GetWindowText( strMsgOld );
		strMsgNew.Format( "%s%.1f%%", strTemp, x );
		
		if( 0 != strMsgOld.Compare( strMsgNew ) )
			m_Static.SetWindowText(strMsgNew);

		// progress
		m_Progress.SetPos(nPos);

		// title
		CString	strTitleOld, strTitleNew;
		GetWindowText( strTitleOld );
		int	nIndex	=	strTitleOld.Find( " - " );
		if( -1 != nIndex )
			strTitleNew	=	strTitleOld.Mid( nIndex + strlen(" - ") );
		else
			strTitleNew	=	strTitleOld;
		strTitleNew	=	strMsgNew + " - " + strTitleNew;
		if( 0 != strTitleOld.Compare( strTitleNew ) )
			SetWindowText( strTitleNew );
	}
}

void CWaitDlg::SetProgressRange(int nMin, int nMax)
{
	if( nMax <= nMin )
		nMax = nMin + 1;
	m_Progress.SetRange( nMin, nMax );
}

BOOL CWaitDlg::WaitForCancel( )
{
	MSG		msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
	{
		AfxGetApp()->PumpMessage();
	}
	return	m_bCanceled;
}


BEGIN_MESSAGE_MAP(CWaitDlg, CXFResDialog)
	//{{AFX_MSG_MAP(CWaitDlg)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_WAITPROGRESS, OnWaitProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg message handlers

BOOL CWaitDlg::OnInitDialog() 
{
	CXFResDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnCancel.EnableWindow( m_bEnableCancel );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaitDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bCanceled	=	true;
}

int CWaitDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXFResDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if( m_bTopMost )
	{
		int	cx	=	GetSystemMetrics( SM_CXSCREEN );
		int	cy	=	GetSystemMetrics( SM_CYSCREEN );
		CRect	rect;
		GetClientRect( &rect );
		m_rectPosition.left	=	(cx-rect.Width())/2;
		m_rectPosition.top	=	(cy-rect.Height())/2;
		m_rectPosition.right=	m_rectPosition.left + rect.Width();
		m_rectPosition.bottom=	m_rectPosition.bottom + rect.Height();

		SetWindowPos( &wndTopMost, m_rectPosition.left, m_rectPosition.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE );
	}
	
	return 0;
}

void CWaitDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bEnableCancel )
		m_bCanceled	=	true;
}

LRESULT CWaitDlg::OnWaitProgress(WPARAM wParam, LPARAM lParam)
{
	SetProgress( wParam );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CXFPropertySheetEx
CXFPropertySheetEx::~CXFPropertySheetEx()
{	
}

BOOL CXFPropertySheetEx::SetBitmap(HBITMAP hbmp)
{
	return m_bmpHeader.Attach(hbmp);
}

IMPLEMENT_DYNAMIC(CXFPropertySheetEx, CPropertySheetEx)

BEGIN_MESSAGE_MAP(CXFPropertySheetEx, CPropertySheetEx)
	//{{AFX_MSG_MAP(CXFPropertySheetEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXFPropertySheetEx message handlers

BOOL CXFPropertySheetEx::OnInitDialog( )
{
	CPropertySheetEx::OnInitDialog( );
	
	SetWizardButtons( PSWIZB_NEXT | PSWIZB_DISABLEDFINISH );

	HWND hWnd = ::GetDlgItem( GetSafeHwnd(), ID_WIZBACK );
	if( hWnd && ::IsWindow( hWnd ) )
	{
		CWnd::ModifyStyle( hWnd, 0, BS_FLAT, 0 );
		m_btnWizback.SubclassWindow( hWnd );
	}

	hWnd = ::GetDlgItem( GetSafeHwnd(), ID_WIZNEXT );
	if( hWnd && ::IsWindow( hWnd ) )
	{
		CWnd::ModifyStyle( hWnd, 0, BS_FLAT, 0 );
		m_btnWiznext.SubclassWindow( hWnd );
	}

	hWnd = ::GetDlgItem( GetSafeHwnd(), ID_WIZFINISH );
	if( hWnd && ::IsWindow( hWnd ) )
	{
		CWnd::ModifyStyle( hWnd, 0, BS_FLAT, 0 );
		m_btnWizfinish.SubclassWindow( hWnd );
	}

	hWnd = ::GetDlgItem( GetSafeHwnd(), IDOK );
	if( hWnd && ::IsWindow( hWnd ) )
	{
		CWnd::ModifyStyle( hWnd, 0, BS_FLAT, 0 );
		m_btnOk.SubclassWindow( hWnd );
	}

	hWnd = ::GetDlgItem( GetSafeHwnd(), IDCANCEL );
	if( hWnd && ::IsWindow( hWnd ) )
	{
		CWnd::ModifyStyle( hWnd, 0, BS_FLAT, 0 );
		m_btnCancel.SubclassWindow( hWnd );
	}

	hWnd = ::GetDlgItem( GetSafeHwnd(), ID_APPLY_NOW );
	if( hWnd && ::IsWindow( hWnd ) )
	{
		CWnd::ModifyStyle( hWnd, 0, BS_FLAT, 0 );
		m_btnApplynow.SubclassWindow( hWnd );
	}

	hWnd = ::GetDlgItem( GetSafeHwnd(), IDHELP );
	if( hWnd && ::IsWindow( hWnd ) )
	{
		CWnd::ModifyStyle( hWnd, 0, BS_FLAT, 0 );
		m_btnHelp.SubclassWindow( hWnd );
	}

/*
	CWnd * pWnd = GetDlgItem( ID_WIZBACK );
	if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		pWnd->ModifyStyle( 0, BS_FLAT );

	pWnd = GetDlgItem( ID_WIZNEXT );
	if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		pWnd->ModifyStyle( 0, BS_FLAT );

	pWnd = GetDlgItem( ID_WIZFINISH );
	if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		pWnd->ModifyStyle( 0, BS_FLAT );

	pWnd = GetDlgItem( IDOK );
	if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		pWnd->ModifyStyle( 0, BS_FLAT );

	pWnd = GetDlgItem( IDCANCEL );
	if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		pWnd->ModifyStyle( 0, BS_FLAT );

	pWnd = GetDlgItem( ID_APPLY_NOW );
	if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		pWnd->ModifyStyle( 0, BS_FLAT );

	pWnd = GetDlgItem( IDHELP );
	if( pWnd && ::IsWindow( pWnd->GetSafeHwnd() ) )
		pWnd->ModifyStyle( 0, BS_FLAT );
*/	
	return TRUE;
}

BOOL CXFPropertySheetEx::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CPropertySheetEx::OnEraseBkgnd(pDC);

	if( (m_psh.dwFlags & PSH_WIZARD97) || (m_psh.dwFlags & PSH_WIZARD) )
	{
		CRect rc;
		GetClientRect(&rc);
		rc.bottom = rc.top + 59;
		pDC->FillSolidRect(&rc,GetSysColor(COLOR_WINDOW));
		CPen pen,*pOldPen;

		pen.CreatePen(PS_SOLID,0,GetSysColor(COLOR_BTNHILIGHT));
		pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(rc.left,rc.bottom);
		pDC->LineTo(rc.right,rc.bottom);
		pDC->SelectObject( pOldPen );
		pen.DeleteObject();

		pen.CreatePen(PS_SOLID,0,GetSysColor(COLOR_BTNSHADOW));
		pDC->SelectObject(&pen);
		pDC->MoveTo(rc.left,rc.bottom-1);
		pDC->LineTo(rc.right,rc.bottom-1);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();

		if( m_bmpHeader.GetSafeHandle() )
		{
			BITMAP	bmpStruct;
			m_bmpHeader.GetBitmap(&bmpStruct);
			DrawTransparentBitmap( pDC->GetSafeHdc(), (HBITMAP)m_bmpHeader.GetSafeHandle(), rc.right-80, rc.top, 0, 0, 
				bmpStruct.bmWidth, bmpStruct.bmHeight, RGB(0,255,0) );
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPathDialogSub & CPathDialog

#define PATHDLG_IDC_FOLDERTREE		0x3741
#define PATHDLG_IDC_TITLE			0x3742
#define PATHDLG_IDC_STATUSTEXT		0x3743

#define PATHDLG_IDC_NEW_EDIT_PATH	0x3744

// Class CDlgWnd
BEGIN_MESSAGE_MAP(CPathDialogSub, CWnd)
        ON_BN_CLICKED(IDOK, OnOK)
		ON_EN_CHANGE(PATHDLG_IDC_NEW_EDIT_PATH, OnChangeEditPath)
END_MESSAGE_MAP()

void CPathDialogSub::OnOK()
{
	::GetWindowText(::GetDlgItem(m_hWnd, PATHDLG_IDC_NEW_EDIT_PATH),
		m_pPathDialog->m_szPathName, MAX_PATH);

	if(CPathDialog::MakeSurePathExists(m_pPathDialog->m_szPathName)==0)
	{
		m_pPathDialog->m_bGetSuccess=TRUE;
		::EndDialog(m_pPathDialog->m_hWnd, IDOK);
	}
	else
	{
		::SetFocus(::GetDlgItem(m_hWnd, PATHDLG_IDC_NEW_EDIT_PATH));
	}
}

void CPathDialogSub::OnChangeEditPath()
{
	::GetWindowText(::GetDlgItem(m_hWnd, PATHDLG_IDC_NEW_EDIT_PATH),
		m_pPathDialog->m_szPathName, MAX_PATH);
	BOOL bEnableOKButton = (_tcslen(m_pPathDialog->m_szPathName)>0);
	SendMessage(BFFM_ENABLEOK, 0, bEnableOKButton);
}
/////////////////////////////////////////////////////////////////////////////
// CPathDialog dialog


CPathDialog::CPathDialog(LPCTSTR lpszCaption, 
						 LPCTSTR lpszTitle, 
						 LPCTSTR lpszInitialPath, 
						 CWnd* pParent)
{
	m_hWnd=NULL;
	m_PathDialogSub.m_pPathDialog= this;
    m_bParentDisabled = FALSE;

    // Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

	m_lpszCaption = lpszCaption;
	m_lpszInitialPath = lpszInitialPath;

	memset(&m_bi, 0, sizeof(BROWSEINFO) );
	m_bi.hwndOwner = (m_pParentWnd==NULL)?NULL:m_pParentWnd->GetSafeHwnd();
	m_bi.pszDisplayName = 0;
	m_bi.pidlRoot = 0;
	m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.lpfn = BrowseCallbackProc;
	m_bi.lpszTitle = lpszTitle;
}


/////////////////////////////////////////////////////////////////////////////
// CPathDialog message handlers

CString CPathDialog::GetPathName()
{
	return CString(m_szPathName);
}

int CALLBACK CPathDialog::BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam, LPARAM pData) 
{
	CPathDialog* pDlg = (CPathDialog*)pData;

	switch(uMsg) 
	{
	case BFFM_INITIALIZED: 
		{
			RECT rc;
			HWND hEdit;
			HFONT hFont;

			pDlg->m_hWnd = hwnd;

			if(pDlg->m_lpszCaption!=NULL)
			{
				::SetWindowText(hwnd, pDlg->m_lpszCaption);
			}

			VERIFY(pDlg->m_PathDialogSub.SubclassWindow(hwnd));
			::ShowWindow(::GetDlgItem(hwnd, PATHDLG_IDC_STATUSTEXT), SW_HIDE);
			::GetWindowRect(::GetDlgItem(hwnd, PATHDLG_IDC_FOLDERTREE), &rc);
			rc.bottom = rc.top - 4;
			rc.top = rc.bottom - 23;
			::ScreenToClient(hwnd, (LPPOINT)&rc);
			::ScreenToClient(hwnd, ((LPPOINT)&rc)+1);
			hEdit = ::CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
				WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
				rc.left, rc.top, 
				rc.right-rc.left, rc.bottom-rc.top, 
				hwnd, NULL, NULL, NULL);
			::SetWindowLong(hEdit, GWL_ID, PATHDLG_IDC_NEW_EDIT_PATH);
			::ShowWindow(hEdit, SW_SHOW);

			hFont = (HFONT)::SendMessage(hwnd, WM_GETFONT, 0, 0);
			::SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

			LPCTSTR lpszPath = pDlg->m_lpszInitialPath;
			TCHAR szTemp[MAX_PATH];
			if(lpszPath==NULL)
			{
				::GetCurrentDirectory(MAX_PATH, szTemp );
				lpszPath = szTemp;
			}
			// WParam is TRUE since you are passing a path.
			// It would be FALSE if you were passing a pidl.
			::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,
				(LPARAM)lpszPath);
			break;
		}
	case BFFM_SELCHANGED:
		{
			char szSelection[MAX_PATH];
			if(!::SHGetPathFromIDList((LPITEMIDLIST)lParam, szSelection) ||
				szSelection[1]!=':')
			{
				szSelection[0] = '\0';
				::SendMessage(hwnd, BFFM_ENABLEOK, 0, FALSE);
			}
			else
			{
				::SendMessage(hwnd, BFFM_ENABLEOK, 0, TRUE);
			}
			::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szSelection);
			::SetWindowText(::GetDlgItem(hwnd, PATHDLG_IDC_NEW_EDIT_PATH), szSelection);
			break;
		}
	default:
		break;
	}
	return 0;
}



int CPathDialog::DoModal() 
{

	/////////////////////////////////////////////////////////
	TCHAR szPathTemp[MAX_PATH];
    m_bi.lpfn = BrowseCallbackProc;  // address of callback function
    m_bi.lParam = (LPARAM)this;      // pass address of object to callback function
	m_bi.pszDisplayName = szPathTemp;

	LPITEMIDLIST pidl;
	LPMALLOC pMalloc;

	int iResult=-1;
	if(SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
//		if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
//		{
//		  m_pParentWnd->EnableWindow(FALSE);
//		  m_bParentDisabled = TRUE;
//		}
		m_bGetSuccess = FALSE;
		pidl = SHBrowseForFolder(&m_bi);
		if (pidl!=NULL) 
		{
			//not need do this because OnOK function did
			//bSucceeded = SHGetPathFromIDList(pidl, m_szPathName);
			// In C++: 
			pMalloc->Free(pidl);
			//In C:
			//pMalloc->lpVtbl->Free(pMalloc,pidl);
			//pMalloc->lpVtbl->Release(pMalloc);
		}
		if(m_bGetSuccess)
		{
			iResult = IDOK;
		}
		pMalloc->Release();
	}

    if(m_bParentDisabled && (m_pParentWnd!=NULL))
	{
		m_pParentWnd->EnableWindow(TRUE);
	}
    m_bParentDisabled=FALSE;

	return iResult;
}

BOOL CPathDialog::IsFileNameValid(LPCTSTR lpFileName)
{
	if(lpFileName==NULL)
	{
		return FALSE;
	}

	int nLen = _tcslen(lpFileName);
	if(nLen<=0)
	{
		return FALSE;
	}

	//check first char
	switch(lpFileName[0])
	{
	case _T('.'):
	case _T(' '):
	case _T('\t'):
		return FALSE;
	}

	//check last char
	switch(lpFileName[nLen-1])
	{
	case _T('.'):
	case _T(' '):
	case _T('\t'):
		return FALSE;
	}

	//check all
	int i=0;
	while(lpFileName[i]!=0)
	{
		switch(lpFileName[i])
		{
		case _T('\\'):
		case _T('/'):
		case _T(':'):
		case _T('*'):
		case _T('?'):
		case _T('\"'):
		case _T('<'):
		case _T('>'):
		case _T('|'):
			return FALSE;
		}
		i++;
	}


	return TRUE;
}

/*
const TCHAR c_FolderDoesNotExist[] = _T(
		"The folder:\n\n"
		"%s\n\n"
		"does not exist. Do you want the folder to be created?");
const TCHAR c_szErrInvalidPath[] = _T(
		"The folder:"
		"\n\n"
		"%s\n\n"
		"is invalid. Please reenter.");
const TCHAR c_szErrCreatePath[] = _T(
		"The folder:"
		"\n\n"
		"%s"
		"\n\ncan not be created. Please double check.");
*/

//return -1: user break;
//return 0: no error
//return 1: lpPath is invalid
//return 2: can not create lpPath
int CPathDialog::MakeSurePathExists(LPCTSTR lpPath)
{
	CString	strPathLocal	=	"";
	if( lpPath )
		strPathLocal	=	lpPath;
	int	nLen	=	strPathLocal.GetLength();
	if( nLen > 0 && (strPathLocal[nLen-1] == '\\' || strPathLocal[nLen-1] == '/') )
		strPathLocal	=	strPathLocal.Left( nLen-1 );

	CString strMsg, strMsgFmt;
	int iRet;
	try
	{
		//validate path
		iRet=Touch(strPathLocal, TRUE);
		if(iRet!=0)
		{
			throw iRet;
		}

		if(_taccess(strPathLocal, 0)==0)
		{
			return (int)0;
		}

		strMsgFmt	=	AfxModuleLoadString( IDS_XFPATHDLG_FOLDERNOTEXIST );
		strMsg.Format(strMsgFmt, lpPath);
		if(AfxMessageBox(strMsg, MB_YESNO|MB_ICONQUESTION) != IDYES)
		{
			return (int)-1;
		}

		//create path
		iRet=Touch(strPathLocal, FALSE);
		if(iRet!=0)
		{
			throw iRet; 
		}

		return 0;
	}
	catch(int nErrCode)
	{
		switch(nErrCode)
		{
		case 1:
			strMsgFmt	=	AfxModuleLoadString( IDS_XFPATHDLG_ERRINVALIDPATH );
			break;
		case 2:
		default:
			strMsgFmt	=	AfxModuleLoadString( IDS_XFPATHDLG_ERRCREATEPATH );
			break;
		}
		strMsg.Format(strMsgFmt, lpPath);
		AfxMessageBox(strMsg, MB_OK|MB_ICONEXCLAMATION);
	}

	return iRet;
}

//return 0: no error
//return 1: lpPath is invalid
//return 2: lpPath can not be created(bValidate==FALSE)
int CPathDialog::Touch(LPCTSTR lpPath, BOOL bValidate)
{
	if(lpPath==NULL)
	{
		return 1;
	}

	TCHAR szPath[MAX_PATH];
	_tcscpy(szPath, lpPath);
	int nLen = _tcslen(szPath);

	//path must be "x:\..."
	if( ( nLen<3 ) || 
		( ( szPath[0]<_T('A') || _T('Z')<szPath[0] ) && 
		  ( szPath[0]<_T('a') || _T('z')<szPath[0] ) ||
		( szPath[1]!=_T(':') )|| 
		( szPath[2]!=_T('\\') )
		)
	  )
	{
		return 1;
	}

	int i;
	if(nLen==3)
	{
		if(!bValidate)
		{
			if(_access(szPath, 0)!=0)
			{
				return 2;
			}
		}
		return 0;
	}

	i = 3;
	BOOL bLastOne=TRUE;
	LPTSTR lpCurrentName;
	while(szPath[i]!=0)
	{
		lpCurrentName = &szPath[i];
		while( (szPath[i]!=0) && (szPath[i]!=_T('\\')) )
		{
			i++;
		}

		bLastOne =(szPath[i]==0);
		szPath[i] = 0;

		if( !IsFileNameValid(lpCurrentName) )
		{
			return 1;
		}

		if(!bValidate)
		{
			CreateDirectory(szPath, NULL);
			if(_taccess(szPath, 0)!=0)
			{
				return 2;
			}
		}

		if(bLastOne)
		{
			break; //it's done
		}
		else
		{
			szPath[i] = _T('\\');
		}

		i++;
	}

	return (bLastOne?0:1);
}

//return 0: ok
//return 1: error
int CPathDialog::ConcatPath(LPTSTR lpRoot, LPCTSTR lpMorePath)
{
	if(lpRoot==NULL)
	{
		return 1;
	}

	int nLen = _tcslen(lpRoot);

	if(nLen<3)
	{
		return 1;
	}

	if(lpMorePath==NULL)
	{
		return 0;
	}

	if(nLen==3)
	{
		_tcscat(lpRoot, lpMorePath);
		return 0;
	}

	_tcscat(lpRoot, _T("\\"));
	_tcscat(lpRoot, lpMorePath);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CStringDialog dialog

CStringDialog::CStringDialog(CWnd* pParent /*=NULL*/, UINT nIDTitle, UINT nIDDescript,
				LPCTSTR lpszInitValue )
	: CXFResDialog(CStringDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStringDialog)
	m_strWant = _T("");
	m_strDescript = _T("");
	//}}AFX_DATA_INIT

	m_nIDTitle		=	nIDTitle;
	m_nIDDescript	=	nIDDescript;
	if( lpszInitValue )
		m_strWant		=	lpszInitValue;
}

CString CStringDialog::GetStringWant( )
{
	return	m_strWant;
}

void CStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CXFResDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStringDialog)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_XFEDITWANT, m_editWant);
	DDX_Text(pDX, IDC_XFEDITWANT, m_strWant);
	DDX_Text(pDX, IDC_XFDESCRIPT, m_strDescript);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStringDialog, CXFResDialog)
	//{{AFX_MSG_MAP(CStringDialog)
	ON_EN_CHANGE(IDC_XFEDITWANT, OnChangeEditwant)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStringDialog message handlers

BOOL CStringDialog::OnInitDialog() 
{
	CXFResDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString	strTitle, strDescript;
	strTitle	=	AfxModuleLoadString( m_nIDTitle );
	strDescript	=	AfxModuleLoadString( m_nIDDescript );
	if( strTitle.GetLength() > 0 )
		SetWindowText( strTitle );
	if( strDescript.GetLength() > 0 )
		m_strDescript	=	strDescript;

	UpdateData( FALSE );

	m_btnOK.EnableWindow( m_strWant.GetLength() > 0 );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStringDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	m_strWant.TrimLeft();
	m_strWant.TrimRight();

	if( m_strWant.GetLength() <= 0 )
		CXFResDialog::OnCancel();
	else
		CXFResDialog::OnOK();
}

void CStringDialog::OnChangeEditwant() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CXFResDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString	rString;
	m_editWant.GetWindowText( rString );
	rString.TrimLeft();
	rString.TrimRight();
	m_btnOK.EnableWindow( rString.GetLength() > 0 );
}


