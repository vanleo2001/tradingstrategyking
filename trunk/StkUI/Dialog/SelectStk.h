#if !defined(AFX_SELECTSTK_H__0A4948DB_4DD0_4DA7_BBAC_61DCF55CA81B__INCLUDED_)
#define AFX_SELECTSTK_H__0A4948DB_4DD0_4DA7_BBAC_61DCF55CA81B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectStk.h : header file
//

#include	"SetGroupDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CSelectStk dialog
/***
	选择股票对话框
*/
class CSelectStk : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CSelectStk)

// Construction
public:
	CSelectStk();
	CSelectStk(UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle);
	~CSelectStk();

	CDomain		m_domainTemp;
	DWORD		m_dwButtonFlags;
	BOOL		m_bModified;

// Dialog Data
	//{{AFX_DATA(CSelectStk)
	enum { IDD = IDD_SELECTSTK };
	CButton	m_btnRemoveAll;
	CButton	m_btnRemoveSelected;
	CButton	m_btnAddAll;
	CButton	m_btnAddSelected;
	CDomainListBox	m_listStockDest;
	CDomainListBox	m_listStockSrc;
	CDomainComboBox	m_comboGroupSrc;
	CEdit	m_editCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSelectStk)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSelectStk)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddselected();
	afx_msg void OnAddall();
	afx_msg void OnRemoveselected();
	afx_msg void OnRemoveall();
	afx_msg void OnChangeEditcode();
	afx_msg void OnSelchangeComboGroupsrc();
	afx_msg void OnDblclkListStocksrc();
	afx_msg void OnDblclkListStockdest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSTK_H__0A4948DB_4DD0_4DA7_BBAC_61DCF55CA81B__INCLUDED_)
