#if !defined(AFX_SPLASHDLG_H__2B492AA1_0780_11D3_9FB8_000021001856__INCLUDED_)
#define AFX_SPLASHDLG_H__2B492AA1_0780_11D3_9FB8_000021001856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashDlg.h : header file
//
#include	"../Src/Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog
#define	WM_USER_SPLASH_PROGRESS	(WM_USER+1001)

/***
	程序启动画面对话框
*/
class CLASS_EXPORT CSplashDlg : public CXFResDialog
{
// Construction
public:
	CSplashDlg(CWnd* pParent = NULL);   // standard constructor
	~CSplashDlg( );

// Dialog Data
	//{{AFX_DATA(CSplashDlg)
	enum { IDD = IDD_XFSPLASH };
	CProgressCtrl	m_ctrlProgress;
	CStatic	m_staticVersion;
	CStatic	m_staticUser;
	CStatic	m_staticRegCode;
	//}}AFX_DATA

	BOOL	SetBitmap( HBITMAP hbmp );
	BOOL	Create( CWnd* pParentWnd, int nShowTime = 1000,
					LPCTSTR lpszUser = NULL, LPCTSTR lpszRegCode = NULL, LPCTSTR lpszVersionString = NULL,
					BOOL bAutoDelete = FALSE, int nMin = 0, int nMax = 10000 );

	void	SetProgressRange(int nMin, int nMax);
	void	SetProgress(int nPos);

protected:
	CBrush		m_brushBK;
	
	CBitmap		m_bitmap;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSplashDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnSplashProgress( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHDLG_H__2B492AA1_0780_11D3_9FB8_000021001856__INCLUDED_)
