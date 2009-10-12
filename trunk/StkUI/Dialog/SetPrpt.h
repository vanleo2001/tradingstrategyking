#if !defined(AFX_SETPRPT_H__19C7D055_26D1_410E_88BB_55EA3060DE04__INCLUDED_)
#define AFX_SETPRPT_H__19C7D055_26D1_410E_88BB_55EA3060DE04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetPrpt.h : header file
//

class	CStrategy;

/////////////////////////////////////////////////////////////////////////////
// CSetPrpt dialog
/***
	设置策略属性的属性页对话框
*/
class CSetPrpt : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CSetPrpt)

// Construction
public:
	CSetPrpt();
	CSetPrpt( UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle );
	~CSetPrpt();

	DWORD		m_dwButtonFlags;

	// operation
	void	SetStrategy( CStrategy * pStrategy );
	void	SetName( LPCTSTR lpszName );
	CString	GetName( );
	void	SetPathName( LPCTSTR lpszPathName );
	CString	GetPathName( );
	void	SetDescript( LPCTSTR lpszDescript );
	CString	GetDescript( );

protected:
	CStrategy	*	m_pStrategy;
	CString		m_strFileExt;
	BOOL		m_bAutoChangeTitle;
	BOOL		m_bAutoChangePath;

	CString		GetPathName( CString strDirName, CString strTitle, CString strFileExt );
	BOOL		ValidData( );
	BOOL		LoadFromStrategy( );
	BOOL		StoreToStrategy( );

// Dialog Data
	//{{AFX_DATA(CSetPrpt)
	enum { IDD = IDD_SETPRPT };
	CButton	m_btnExplore;
	CEdit	m_editDescript;
	CString	m_strName;
	CString	m_strPathName;
	CString	m_strDescript;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSetPrpt)
	public:
	virtual BOOL OnWizardFinish();
	virtual BOOL OnKillActive();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetPrpt)
	virtual BOOL OnInitDialog();
	afx_msg void OnExplore();
	afx_msg void OnChangeTitle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPRPT_H__19C7D055_26D1_410E_88BB_55EA3060DE04__INCLUDED_)
