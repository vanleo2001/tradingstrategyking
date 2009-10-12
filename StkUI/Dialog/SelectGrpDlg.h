#if !defined(AFX_SELECTGRPDLG_H__B131BD74_AEE9_4FA4_A109_ACE86F963FB1__INCLUDED_)
#define AFX_SELECTGRPDLG_H__B131BD74_AEE9_4FA4_A109_ACE86F963FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectGrpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectGrpDlg dialog
/***
	选择板块、分组、策略对话框
*/
class CSelectGrpDlg : public CXFDialog
{
// Construction
public:
	CSelectGrpDlg(CWnd* pParent = NULL);   // standard constructor

	enum	mode	{
		modeDomain		=	0x01,
		modeGroup		=	0x02,
		modeStrategy	=	0x03,
	};
	void		SetMode( int nMode = modeDomain );
	CString		GetSelectedName( );
	CStrategy *	GetSelectedStrategy( );

// Dialog Data
	//{{AFX_DATA(CSelectGrpDlg)
	enum { IDD = IDD_SELECTGRP };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CStatic	m_staticInfo;
	CButton	m_btnAddGroup;
	CListBox	m_listGroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectGrpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString		m_strSelectedName;
	CStrategy *	m_pSelectedStrategy;
	int			m_nMode;

	// Generated message map functions
	//{{AFX_MSG(CSelectGrpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkListgroup();
	afx_msg void OnAddgroup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTGRPDLG_H__B131BD74_AEE9_4FA4_A109_ACE86F963FB1__INCLUDED_)
