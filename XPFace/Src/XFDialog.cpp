// XFDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XFDialog.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL IsNumber( CString & string, BOOL bCanHasZeroLength )
{
	if( !bCanHasZeroLength && string.GetLength() <= 0 )
		return FALSE;
	for( int i=0; i<string.GetLength(); i++ )
		if( !isdigit(string[i]) && '.' != string[i] && !isspace(string[i]) )
			return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXFDialog dialog

CXFDialog::CXFDialog( )
{
	m_bAutoDelete	=	FALSE;
	m_bTransparent	=	FALSE;
	m_byteDeep		=	250;
}

CXFDialog::CXFDialog(LPCTSTR lpszTemplateName, CWnd* pParent /*=NULL*/)
	: CDialog(lpszTemplateName, pParent)
{
	m_bAutoDelete	=	FALSE;
	m_bTransparent	=	FALSE;
	m_byteDeep		=	250;
}

CXFDialog::CXFDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CXFDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bAutoDelete	=	FALSE;
	m_bTransparent	=	FALSE;
	m_byteDeep		=	250;
}

void CXFDialog::SetAutoDelete( BOOL bAutoDelete )
{
	m_bAutoDelete	=	bAutoDelete;
}

void CXFDialog::SetTransparent( BOOL bTransparent, BYTE byteDeep )
{
	m_bTransparent	=	bTransparent;
	m_byteDeep		=	byteDeep;
}

void CXFDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXFDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXFDialog, CDialog)
	//{{AFX_MSG_MAP(CXFDialog)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXFDialog message handlers

HBRUSH CXFDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
/*	if( nCtlColor == CTLCOLOR_DLG
		|| nCtlColor == CTLCOLOR_STATIC
		|| nCtlColor == CTLCOLOR_MSGBOX )
	{
		if( nCtlColor == CTLCOLOR_STATIC )
			pDC->SetBkMode( TRANSPARENT );

		static CBrush brushBK;
		if( brushBK.GetSafeHandle() == NULL )
			VERIFY( brushBK.CreateSolidBrush( RGB(215,212,206) ) );
		return (HBRUSH)brushBK.GetSafeHandle();
	}
*/
	// TODO: Return a different brush if the default is not desired
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CXFDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if( m_bTransparent )
	{
		SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
		HINSTANCE hInst = LoadLibrary("User32.DLL");
		if(hInst)
		{
			typedef BOOL (WINAPI *TRANSPARENTFUNC)(HWND,COLORREF,BYTE,DWORD);
			TRANSPARENTFUNC	fun	=	NULL;
			//取得SetLayeredWindowAttributes函数指针     
			fun	=	(TRANSPARENTFUNC)GetProcAddress( hInst, "SetLayeredWindowAttributes" );
			if(fun)
				fun( GetSafeHwnd(), 0, m_byteDeep, 2 );
			FreeLibrary(hInst); 
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXFDialog::OnOK()
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	CXFDialog::PostNcDestroy( );
}

void CXFDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
	CXFDialog::PostNcDestroy( );
}

void CXFDialog::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if( m_bAutoDelete && NULL != this )
		delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CXFResDialog dialog
// 使用 XPFace DLL资源的 Dialog，需从CXFResDialog继承 

CXFResDialog::CXFResDialog( )
	: CXFDialog( )
{
}

CXFResDialog::CXFResDialog(LPCTSTR lpszTemplateName, CWnd* pParent /*=NULL*/)
	: CXFDialog(lpszTemplateName, pParent)
{
}

CXFResDialog::CXFResDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CXFDialog(nIDTemplate, pParent)
{
}

int CXFResDialog::DoModal() 
{
	HINSTANCE hInstanceOld = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetModuleHandle() );
	
	int nRet = CXFDialog::DoModal();
	
	AfxSetResourceHandle( hInstanceOld );
	return nRet;
}

/////////////////////////////////////////////////////////////////////////////
//  CKSFileDialog 2000 style

#define _countof(array) (sizeof(array)/sizeof(array[0]))

IMPLEMENT_DYNAMIC(CKSFileDialog, CFileDialog)

CKSFileDialog::CKSFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	
    m_ofn.Flags             = NULL;
	m_ofn.Flags				= dwFlags;//|OFN_ENABLEHOOK;
	
	m_pApp=NULL;
	m_pDoc=NULL;
	m_StrFilter.Empty();


}


BEGIN_MESSAGE_MAP(CKSFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CKSFileDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CKSFileDialog::DoModal()
{
	ASSERT_VALID(this);
	DWORD nOffset = lstrlen(m_ofn.lpstrFile)+1;
	memset(m_ofn.lpstrFile+nOffset, 0, (m_ofn.nMaxFile-nOffset)*sizeof(TCHAR));

	HWND hWndFocus = ::GetFocus();
	BOOL bEnableParent = FALSE;
	//m_ofn.hwndOwner = PreModal();
	
	CWnd dummy;
	dummy.CreateEx(NULL,AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),NULL,NULL,CRect(0,0,0,0),NULL,NULL);
	dummy.CenterWindow();
	CRect rect;
	dummy.GetWindowRect(rect);
	dummy.SetWindowPos(NULL, rect.left-230, rect.top-180, -1, -1,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);	dummy.ShowWindow(SW_HIDE);
	m_ofn.hwndOwner=dummy.GetSafeHwnd();
	if (m_ofn.hwndOwner==NULL) m_ofn.hwndOwner=PreModal();
	if (m_ofn.hwndOwner != NULL && ::IsWindowEnabled(m_ofn.hwndOwner))
	{
		bEnableParent = TRUE;
		::EnableWindow(m_ofn.hwndOwner, FALSE);
	}

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	ASSERT(pThreadState->m_pAlternateWndInit == NULL);

	m_ofn.lpfnHook = NULL;
	m_ofn.lpTemplateName = NULL;
	int nResult;
	if (m_bOpenFileDialog)
		nResult = ::GetOpenFileName(&m_ofn);
	else
		nResult = ::GetSaveFileName(&m_ofn);

	if (nResult)
		ASSERT(pThreadState->m_pAlternateWndInit == NULL);
	pThreadState->m_pAlternateWndInit = NULL;

	// WINBUG: Second part of special case for file open/save dialog.
	if (bEnableParent)
		::EnableWindow(m_ofn.hwndOwner, TRUE);
	if (::IsWindow(hWndFocus))
		::SetFocus(hWndFocus);

	PostModal();
	return nResult ? nResult : IDCANCEL;
}


BOOL CKSFileDialog::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
	BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	CString title;
	title	=	AfxModuleLoadString(nIDSTitle);
	if (m_pApp==NULL) {ASSERT (FALSE);} //set the m_pApp app point before you call this function
	ASSERT(m_pApp->m_pDocManager != NULL);
	m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	
	if (nIDSTitle==AFX_IDS_SAVEFILE || nIDSTitle==AFX_IDS_SAVEFILECOPY)
	{
		POSITION pos = m_pApp->GetFirstDocTemplatePosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_pApp->GetNextDocTemplate(pos);
			AppendFilterSuffix(strFilter, m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}
	else
	{
		if (m_StrFilter.GetLength()==0)
			strFilter = "My File Format (*.mff)|*.mff|My File Format Files (*.mff)";
		else
			strFilter = m_StrFilter;
	}
	m_ofn.nMaxCustFilter++;

	
	LPTSTR pch = strFilter.GetBuffer(0); // modify the buffer in place
		// MFC delimits with '|' not '\0'
	while ((pch = _tcschr(pch, '|')) != NULL)
			*pch++ = '\0';
	m_ofn.lpstrFilter = strFilter;
	m_ofn.lpstrTitle = title;
	m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	m_ofn.hwndOwner=m_pApp->m_pMainWnd->GetSafeHwnd() ;
	m_ofn.Flags&=OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT;//|OFN_ENABLEHOOK;
	int nResult = DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}

void CKSFileDialog::AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);
	
	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

void CKSFileDialog::SetAppPointer(CWinApp *pApp)
{
	m_pApp=pApp;
}

BOOL CKSFileDialog::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	if (m_pDoc==NULL){ASSERT (FALSE);return FALSE;}
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = m_pDoc->GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_pDoc->GetPathName();
		if (bReplace && newName.IsEmpty())
		{
			newName = m_pDoc->GetTitle();
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		if (DoPromptFileName(newName,bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!m_pDoc->OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			try
			{
				CFile::Remove(newName);
			}
			catch( CException * e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				do { e->Delete(); } while (0);
			}
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		m_pDoc->SetPathName(newName);

	return TRUE;        // success
}

void CKSFileDialog::SetDocumentPointer(CDocument *pDoc)
{
	m_pDoc=pDoc;
}

void CKSFileDialog::SetStringFilter(LPCSTR filter)
{
	m_StrFilter=filter;
}

