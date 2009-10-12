#if !defined(AFX_SETRATE_H__EAD836A1_B915_44E0_BFB2_99B55FE14FBD__INCLUDED_)
#define AFX_SETRATE_H__EAD836A1_B915_44E0_BFB2_99B55FE14FBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRate dialog
/***
	设置策略的交易费用的属性页对话框
*/
class CSetRate : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CSetRate)

// Construction
public:
	CSetRate();
	CSetRate( UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle );
	~CSetRate();

	DWORD		m_dwButtonFlags;
	BOOL		m_bModified;

	// operation
	void	SetStrategy( CStrategy * pStrategy );

// Dialog Data
	//{{AFX_DATA(CSetRate)
	enum { IDD = IDD_SETRATE };
	CString	m_strTaxShaa;
	CString	m_strTaxShab;
	CString	m_strTaxShafund;
	CString	m_strTaxSzna;
	CString	m_strTaxSznb;
	CString	m_strTaxSznfund;
	//}}AFX_DATA

protected:
	CStrategy	*	m_pStrategy;

	BOOL		ValidData( );
	BOOL		LoadFromStrategy( );
	BOOL		StoreToStrategy( );

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSetRate)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetActive();
	protected:
	virtual BOOL OnWizardFinish();
	virtual BOOL OnKillActive();
	virtual LRESULT OnWizardNext();
	virtual LRESULT OnWizardBack();
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetRate)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETRATE_H__EAD836A1_B915_44E0_BFB2_99B55FE14FBD__INCLUDED_)
