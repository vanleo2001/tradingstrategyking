#if !defined(AFX_SETKDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_)
#define AFX_SETKDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetKDataDlg.h : header file
//
#include	"SetGroupDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSetKDataDlg dialog
/***
	设置除权除息资料的对话框
*/
class CSetKDataDlg : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CSetKDataDlg)

// Construction
public:
	CSetKDataDlg( );   // standard constructor

	void	OnCompleted( );

// Dialog Data
	//{{AFX_DATA(CSetKDataDlg)
	enum { IDD = IDD_SETKDATA };
	CButton	m_btnSave;
	CButton	m_btnDelete;
	CKTypeComboBox	m_comboKType;
	CEdit	m_editAmount;
	CEdit	m_editVolume;
	CEdit	m_editClose;
	CEdit	m_editLow;
	CEdit	m_editHigh;
	CEdit	m_editOpen;
	CDateTimeCtrl	m_dtcDate;
	CEdit	m_editCode;
	CStatic	m_staticStock;
	CDomainListBox	m_listStockSrc;
	CDomainComboBox	m_comboGroupSrc;
	CSPTime	m_tmDate;
	CString	m_strOpen;
	CString	m_strHigh;
	CString	m_strLow;
	CString	m_strClose;
	DWORD	m_dwVolume;
	DWORD	m_dwAmount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetKDataDlg)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString		m_strCurStockCode;

	void	EnableEdit( BOOL bEnable );
	void	LoadKData( CString strStockCode );
	BOOL	StoreKData( CString strStockCode );
	BOOL	DeleteKData( CString strStockCode );
	
	// Generated message map functions
	//{{AFX_MSG(CSetKDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditcode();
	afx_msg void OnSelchangeComboGroupsrc();
	afx_msg void OnSelchangeListStocksrc();
	afx_msg void OnDatetimechangeDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSave();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETKDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_)
