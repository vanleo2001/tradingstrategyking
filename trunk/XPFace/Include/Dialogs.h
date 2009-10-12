#if !defined(AFX_DIALOGS_H__9E89DE8F_4796_46DB_8262_75BFFEF26FC0__INCLUDED_)
#define AFX_DIALOGS_H__9E89DE8F_4796_46DB_8262_75BFFEF26FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialogs.h : header file
//
#include	"../Src/Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog
/***
	等待对话框，显示进度条
*/
#define	WM_USER_WAITPROGRESS	WM_USER+1001

class CLASS_EXPORT CWaitDlg : public CXFResDialog
{
// Construction
public:
	CWaitDlg(CWnd* pParent = NULL, BOOL bEnableCancel = TRUE, BOOL bTopMost = TRUE, CRect rect = CRect(0,0,0,0) );   // standard constructor

	BOOL	m_bTopMost;
	CRect	m_rectPosition;

	BOOL	m_bEnableCancel;
	BOOL	m_bCanceled;

// Dialog Data
	//{{AFX_DATA(CWaitDlg)
	enum { IDD = IDD_XFWAITDLG };
	CButton	m_btnCancel;
	CStatic	m_Static;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA

	void	SetProgress(int nPos);
	void	SetProgressRange(int nLower, int nUpper);

	BOOL	WaitForCancel( );

private:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaitDlg)
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnWaitProgress( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CXFPropertySheetEx
/***
	属性对话框，可以显示上方位图
*/
class CLASS_EXPORT CXFPropertySheetEx : public CPropertySheetEx
{
	DECLARE_DYNAMIC(CXFPropertySheetEx)

// Construction
public:
	virtual ~CXFPropertySheetEx();

// Attributes
public:
	BOOL	SetBitmap( HBITMAP hbmp );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXFPropertySheetEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL OnInitDialog();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXFPropertySheetEx)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBitmap		m_bmpHeader;

	CButton	m_btnWizback;
	CButton	m_btnWiznext;
	CButton	m_btnWizfinish;
	CButton	m_btnOk;
	CButton	m_btnCancel;
	CButton	m_btnApplynow;
	CButton	m_btnHelp;
};

/////////////////////////////////////////////////////////////////////////////
// CPathDialog

#include "shlobj.h"

class CPathDialog;

// CPathDialogSub - intercepts messages from child controls
class CLASS_EXPORT CPathDialogSub : public CWnd
{
	friend CPathDialog;
public:
	CPathDialog* m_pPathDialog;
protected:
    afx_msg void OnOK();              // OK button clicked
	afx_msg void OnChangeEditPath();
    DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////
// CPathDialog dialog
/***
	路径选择对话框
*/
class CLASS_EXPORT CPathDialog
{
	friend CPathDialogSub;
// Construction
public:
	CPathDialog(LPCTSTR lpszCaption=NULL,
		LPCTSTR lpszTitle=NULL,
		LPCTSTR lpszInitialPath=NULL, 
		CWnd* pParent = NULL);

	CString GetPathName();
	virtual int DoModal();

	static int Touch(LPCTSTR lpPath, BOOL bValidate=TRUE);
	static int MakeSurePathExists(LPCTSTR lpPath);
	static BOOL IsFileNameValid(LPCTSTR lpFileName);
	static int ConcatPath(LPTSTR lpRoot, LPCTSTR lpMorePath);

private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam, LPARAM pData);

	LPCTSTR m_lpszCaption;
	LPCTSTR m_lpszInitialPath;

	TCHAR m_szPathName[MAX_PATH];

	BROWSEINFO m_bi;
	HWND m_hWnd;
	CWnd*	m_pParentWnd;
	BOOL m_bParentDisabled;
	BOOL m_bGetSuccess;

	CPathDialogSub m_PathDialogSub;
};


/////////////////////////////////////////////////////////////////////////////
// CStringDialog dialog
/***
	输入字符串对话框
*/
class CLASS_EXPORT CStringDialog : public CXFResDialog
{
// Construction
public:
	CStringDialog(CWnd* pParent = NULL, UINT nIDTitle = 0, UINT nIDDescript = 0,
				LPCTSTR lpszInitValue = NULL );   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStringDialog)
	enum { IDD = IDD_XFSTRINGDLG };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CEdit	m_editWant;
	CString	m_strWant;
	CString	m_strDescript;
	//}}AFX_DATA

	CString	GetStringWant( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT	m_nIDTitle;
	UINT	m_nIDDescript;

	// Generated message map functions
	//{{AFX_MSG(CStringDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditwant();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGS_H__9E89DE8F_4796_46DB_8262_75BFFEF26FC0__INCLUDED_)
