#if !defined(AFX_SIMUADDSTOCKDLG_H__317B07B0_55E5_404F_82C8_C7EB5B49FE8B__INCLUDED_)
#define AFX_SIMUADDSTOCKDLG_H__317B07B0_55E5_404F_82C8_C7EB5B49FE8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuAddStockDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimuAddStockDlg dialog
/***
	策略的实战操作中，加入股票对话框
*/
class CSimuAddStockDlg : public CXFDialog
{
// Construction
public:
	CSimuAddStockDlg(CWnd* pParent = NULL);   // standard constructor

	enum mode {
		modeAddStock		=	0x01,
		modeModifiedCash	=	0x02,
	};

	void	SetStrategy( CStrategy * pStrategy );
	void	SetMode( int nMode, CString sCode, double dValue, double dPrice );
	CString	GetStockCode( );
	double	GetValue( );
	double	GetPrice( );

// Dialog Data
	//{{AFX_DATA(CSimuAddStockDlg)
	enum { IDD = IDD_SIMUADDSTOCK };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CStatic	m_staticPrice;
	CEdit	m_editPrice;
	CStatic	m_staticValue;
	CStatic	m_staticStock;
	CSearchBox	m_comboStock;
	CString	m_strValue;
	CString	m_strPrice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuAddStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStrategy	*		m_pStrategy;

	int				m_nMode;
	CString			m_strStockCode;
	double			m_dValue;
	double			m_dPrice;

	// Generated message map functions
	//{{AFX_MSG(CSimuAddStockDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUADDSTOCKDLG_H__317B07B0_55E5_404F_82C8_C7EB5B49FE8B__INCLUDED_)
