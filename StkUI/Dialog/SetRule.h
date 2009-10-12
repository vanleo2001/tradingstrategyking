#if !defined(AFX_SETRULE_H__47F1A524_1DCF_400F_AEE3_349E9BD0EF50__INCLUDED_)
#define AFX_SETRULE_H__47F1A524_1DCF_400F_AEE3_349E9BD0EF50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRule dialog

#define	WM_USER_SETRULECLICKTREE	(WM_USER+1001)

class	CParamDlg;
/***
	设置策略的买卖规则的属性页对话框
*/
class CSetRule : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CSetRule)

// Construction
public:
	CSetRule();
	CSetRule( UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle );
	~CSetRule();

	DWORD		m_dwButtonFlags;
	BOOL		m_bModified;

	void	SetTechParameters( CTechParameters * pParam, CSPDWordArray *panSelectedTechs );

// Dialog Data
	//{{AFX_DATA(CSetRule)
	enum { IDD = IDD_SETRULE };
	CTreeCtrl	m_tree;
	CButton	m_btnAllDefault;
	//}}AFX_DATA

protected:
	CParamDlg *	m_pParamDlg;
	CImageList	m_ImageList;
	
	CTechParameters	*	m_pTechParameters;
	CSPDWordArray	*	m_panSelectedTechs;

	CSPDWordArray	m_anSelectedTechs;
	CSPDWordArray	m_anClassChecked;

	BOOL		InitTreeTechs( );
	void		GetCurrentSelectedTechs( CSPDWordArray & au );
	BOOL		ValidData( );
	BOOL		StoreData( );

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSetRule)
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
	//{{AFX_MSG(CSetRule)
	afx_msg void OnAlldefault();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnSetRuleClickTree( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETRULE_H__47F1A524_1DCF_400F_AEE3_349E9BD0EF50__INCLUDED_)
