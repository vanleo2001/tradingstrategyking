#if !defined(AFX_SIMUREALOP_H__F3F245E1_587C_477B_BE3A_650BE5D1A27E__INCLUDED_)
#define AFX_SIMUREALOP_H__F3F245E1_587C_477B_BE3A_650BE5D1A27E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuRealOp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimuRealOp dialog
#include	"SimuReport.h"

BOOL CALLBACK RealRunCallback(DWORD dwType, DWORD dwValue,
							LPCTSTR lpszMsg, void *cookie);

/***
	策略的实战操作对话框
*/
class CSimuRealOp : public CXFDialog
{
// Construction
public:
	CSimuRealOp(CWnd* pParent = NULL);   // standard constructor
	~CSimuRealOp();

	BOOL	m_bModified;

	// operation
	void	SetStrategy( CStrategy * pStrategy );
	BOOL	Initialize( );

// Dialog Data
	//{{AFX_DATA(CSimuRealOp)
	enum { IDD = IDD_SIMUREALOP };
	CButton	m_btnStatYield;
	CButton	m_btnOK;
	CStatic	m_staticTimeExplain;
	CStatic	m_staticExplain;
	CButton	m_btnResetAll;
	CButton	m_btnModified;
	CButton	m_btnDeleteStock;
	CButton	m_btnAddStock;
	CButton	m_btnDeleteOp;
	CButton	m_btnAddOp;
	CButton	m_btnCopy;
	CReportListCtrl	m_listStockOwn;
	CCurveButton	m_curve;
	CReportListCtrl	m_listNextOp;
	CReportListCtrl	m_listOpRecord;
	CTabCtrl	m_tabctrl;
	//}}AFX_DATA

protected:
	CStrategy	*	m_pStrategy;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSimuRealOp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSimuRealOp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabctrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCopy();
	afx_msg void OnStatyield();
	afx_msg void OnAddop();
	afx_msg void OnDeleteop();
	afx_msg void OnAddstock();
	afx_msg void OnDeletestock();
	afx_msg void OnModified();
	afx_msg void OnResetall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUREALOP_H__F3F245E1_587C_477B_BE3A_650BE5D1A27E__INCLUDED_)
