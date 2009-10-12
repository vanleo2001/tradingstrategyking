#if !defined(AFX_SETBASEDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_)
#define AFX_SETBASEDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetBasedataDlg.h : header file
//
#include	"SetGroupDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSetBasedataDlg dialog
/***
	设置除权除息资料的对话框
*/
class CSetBasedataDlg : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CSetBasedataDlg)

// Construction
public:
	CSetBasedataDlg( );   // standard constructor

	void	OnCompleted( );

// Dialog Data
	//{{AFX_DATA(CSetBasedataDlg)
	enum { IDD = IDD_SETBASEDATA };
	CButton	m_btnSave;
	CEdit	m_editTotal;
	CEdit	m_editCorp;
	CEdit	m_editNational;
	CEdit	m_editH;
	CEdit	m_editB;
	CEdit	m_editA;
	CStatic	m_staticStock;
	CDomainListBox	m_listStockSrc;
	CDomainComboBox	m_comboGroupSrc;
	CEdit	m_editCode;
	CString	m_strA;
	CString	m_strB;
	CString	m_strH;
	CString	m_strNational;
	CString	m_strCorp;
	CString	m_strTotal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetBasedataDlg)
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
	void	LoadBasedata( CString strStockCode );
	BOOL	StoreBasedata( CString strStockCode );
	
	// Generated message map functions
	//{{AFX_MSG(CSetBasedataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditcode();
	afx_msg void OnSelchangeComboGroupsrc();
	afx_msg void OnSelchangeListStocksrc();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBASEDATADLG_H__03465F7B_741C_4358_A2F2_636329DF6D09__INCLUDED_)
