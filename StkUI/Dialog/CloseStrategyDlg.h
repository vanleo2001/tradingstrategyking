#if !defined(AFX_CLOSESTRATEGY_H__0AD8D512_42BC_4FD8_B206_C4440194E97B__INCLUDED_)
#define AFX_CLOSESTRATEGY_H__0AD8D512_42BC_4FD8_B206_C4440194E97B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CloseStrategy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCloseStrategyDlg dialog

/***
	关闭策略对话框，选择要关闭的策略
*/
class CCloseStrategyDlg : public CXFDialog
{
// Construction
public:
	CCloseStrategyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCloseStrategyDlg)
	enum { IDD = IDD_CLOSESTRATEGYDLG };
	CButton	m_btnCloseAll;
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CListCtrl	m_listStrategy;
	//}}AFX_DATA

public:
	// operate
	POSITION	GetFirstCloseStrategyPosition( );
	CStrategy *	GetNextCloseStrategy( POSITION &rPos );

protected:
	CPtrList	m_listCloseStrategy;          // CStrategy to close
	CImageList	m_SmallImageList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCloseStrategyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCloseStrategyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCloseall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOSESTRATEGY_H__0AD8D512_42BC_4FD8_B206_C4440194E97B__INCLUDED_)
