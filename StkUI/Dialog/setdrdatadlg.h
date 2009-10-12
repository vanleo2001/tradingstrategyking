#if !defined(AFX_SETDRDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_)
#define AFX_SETDRDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDrdataDlg.h : header file
//
#include	"SetGroupDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSetDrdataDlg dialog
/***
	设置除权除息资料的对话框
*/
class CSetDrdataDlg : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CSetDrdataDlg)

// Construction
public:
	CSetDrdataDlg( );   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDrdataDlg)
	enum { IDD = IDD_SETDRDATA };
	CButton	m_btnImportFxj;
	CButton	m_btnImport;
	CButton	m_btnDelete;
	CButton	m_btnInsert;
	CStatic	m_staticStock;
	CDomainListBox	m_listStockSrc;
	CDomainComboBox	m_comboGroupSrc;
	CEdit	m_editCode;
	CSPTime	m_tmDrdata;
	//}}AFX_DATA
	CGridCtrl	m_Grid;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDrdataDlg)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString		m_strCurStockCode;

	BOOL	InitializeGrid( );
	void	LoadDrdata( CString strStockCode );
	void	StoreDrdata( CString strStockCode );
	
	// Generated message map functions
	//{{AFX_MSG(CSetDrdataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditcode();
	afx_msg void OnSelchangeComboGroupsrc();
	afx_msg void OnSelchangeListStocksrc();
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnImport();
	afx_msg void OnImportfxj();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDRDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_)
