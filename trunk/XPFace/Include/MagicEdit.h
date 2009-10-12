#if !defined(AFX_MAGICRICHEDIT_H__7B394F21_9081_11D2_9FB7_000021001856__INCLUDED_)
#define AFX_MAGICRICHEDIT_H__7B394F21_9081_11D2_9FB7_000021001856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MagicRichEdit.h : header file
//
#include	"../Src/Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CMagicDlg dialog
/***
	编辑时的下拉列表框
*/
class CLASS_EXPORT CMagicDlg : public CXFResDialog
{
// Construction
public:
	CMagicDlg(CWnd* pParent = NULL);   // standard constructor

	void	SetListStrings( CStringArray & astr );
	int		GetMaxLength( );

// Dialog Data
	//{{AFX_DATA(CMagicDlg)
	enum { IDD = IDD_XFMAGIC };
	CListBox	m_listMagic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		m_nMaxLength;

	// Generated message map functions
	//{{AFX_MSG(CMagicDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMagicEdit window
/***
	新建指标编辑框
*/
class CLASS_EXPORT CMagicEdit : public CEdit
{
// Construction
public:
	CMagicEdit();

	void SetListStrings( CStringArray & astr );

// Attributes
public:
	CMagicDlg		m_dlgMagic;
	CString			m_strSearch;
	int				m_nMagicBegin;

// Operations
public:
	void	ShowMagicDialog( );
	void	HideMagicDialog( );
	
	BOOL	m_bOpen;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagicEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMagicEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMagicEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGICRICHEDIT_H__7B394F21_9081_11D2_9FB7_000021001856__INCLUDED_)
