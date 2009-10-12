#if !defined(AFX_XFDIALOG_H__9E89DE8F_4796_46DB_8262_75BFFEF26FC0__INCLUDED_)
#define AFX_XFDIALOG_H__9E89DE8F_4796_46DB_8262_75BFFEF26FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XFDialog.h : header file
//

BOOL CLASS_EXPORT IsNumber( CString & string, BOOL bCanHasZeroLength );

/////////////////////////////////////////////////////////////////////////////
// CXFDialog dialog

class CLASS_EXPORT CXFDialog : public CDialog
{
// Construction
public:
	CXFDialog( );
	CXFDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CXFDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);	// standard constructor

	void	SetAutoDelete( BOOL bAutoDelete = FALSE );
	void	SetTransparent( BOOL bTransparent = TRUE, BYTE byteDeep = 244 );

// Dialog Data
	//{{AFX_DATA(CXFDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXFDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CXFDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL	m_bAutoDelete;
	BOOL	m_bTransparent;
	BYTE	m_byteDeep;
};

/////////////////////////////////////////////////////////////////////////////
// CXFResDialog dialog
// 使用 XPFace DLL资源的 Dialog，需从CXFResDialog继承 

class CLASS_EXPORT CXFResDialog : public CXFDialog
{
// Construction
public:
	CXFResDialog( );
	CXFResDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CXFResDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);	// standard constructor

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectEngineDlg)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL
};

/////////////////////////////////////////////////////////////////////////////
// CKSFileDialog 2000 Style

class CLASS_EXPORT CKSFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CKSFileDialog)
public:
	void SetStringFilter(LPCSTR filter);
	void SetDocumentPointer (CDocument* pDoc);
	void SetAppPointer(CWinApp* pApp);
	int DoModal();
	CKSFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_EXPLORER|OFN_ENABLESIZING| OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
						  BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);



protected:
	void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt);
	//{{AFX_MSG(CKSFileDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CWinApp *m_pApp;
	CDocument *m_pDoc;
	CString m_StrFilter;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XFDIALOG_H__9E89DE8F_4796_46DB_8262_75BFFEF26FC0__INCLUDED_)
