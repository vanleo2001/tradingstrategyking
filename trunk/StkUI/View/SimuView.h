// SimuView.h : interface of the CSimuView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUVIEW_H__2C0009CF_8889_46CD_B38B_4743C1A75F62__INCLUDED_)
#define AFX_SIMUVIEW_H__2C0009CF_8889_46CD_B38B_4743C1A75F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class	CParamDlg;

#define	WM_USER_SIMULATION_PROGRESS	(WM_USER+1001)
#define	WM_USER_SIMULATION_YIELD	(WM_USER+1002)
#define	WM_USER_SIMULATION_END		(WM_USER+1003)

#include	"Simulation.h"
#include	"ParamDlg/SetParamDlg.h"
/***
	≤ﬂ¬‘ ”Õº
*/
class CSimuView : public CFormView
{
protected: // create from serialization only
	CSimuView();
	DECLARE_DYNCREATE(CSimuView)

public:
	//{{AFX_DATA(CSimuView)
	enum { IDD = IDD_SIMU_FORM };
	CEdit	m_editSellMulti;
	CButton	m_btnAdvanced;
	CEdit	m_editStockTech;
	CComboBox	m_comboSellLogic;
	CComboBox	m_comboBuyLogic;
	CButton	m_btnStop;
	CEdit	m_editStartAmount;
	CStatic		m_staticYield2;
	CStatic		m_staticYield;
	CDateTimeCtrl	m_tmctrlBegin;
	CDateTimeCtrl	m_tmctrlEnd;
	CComboBox	m_comboStoreDiv;
	CIntensityComboBox	m_comboSellLimit;
	CEdit		m_editBuyMulti;
	CIntensityComboBox	m_comboBuyLimit;
	CComboBox	m_comboMaindataType;
	CStatic		m_staticWarning;
	CStatic		m_staticTitle;
	CButton	m_btnRealOp;
	CButton	m_btnSetRule;
	CButton	m_btnSetRate;
	CButton	m_btnSetPrpt;
	CButton	m_btnReport;
	CButton	m_btnSelectStk;
	CButton	m_btnStart;
	CProgressCtrl	m_progress;
	CKTypeComboBox	m_comboKType;
	CString	m_strBuyMulti;
	CSPTime	m_tmBegin;
	CSPTime	m_tmEnd;
	long	m_nStartAmount;
	CString	m_strSellMulti;
	//}}AFX_DATA

// Attributes
public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }
	CStrategy	*	GetStrategy( );

// Operations
public:
	BOOL			CanChangeActiveStrategy( );
	virtual	void	OnActiveStrategyChanged( );
	BOOL			LoadFromStrategy( );
	BOOL			StoreToStrategy( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimuView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStrategy	*	m_pStrategy;

	CSimulation	m_simulation;

	BOOL		m_bDisableAllControls;

	// Initialize Functions
	BOOL	InitComboMaindataType( );
	BOOL	InitComboBuyLogic( );
	BOOL	InitComboBuyLimit( );
	BOOL	InitComboSellLogic( );
	BOOL	InitComboSellLimit( );
	BOOL	InitComboStoreDiv( );

	BOOL	EnableControls( );

	BOOL	ClearData( );
	BOOL	SelectComboItem( CComboBox & combo, DWORD dwData );

// Generated message map functions
protected:
	//{{AFX_MSG(CSimuView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetprpt();
	afx_msg void OnSelectstk();
	afx_msg void OnSetrule();
	afx_msg void OnSetrate();
	afx_msg void OnReport();
	afx_msg void OnRealOp();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnCloseupTimeBegin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseupTimeEnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAdvanced();
	//}}AFX_MSG
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCanCloseView( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSimulationProgress( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSimulationYield( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSimulationEnd( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUVIEW_H__2C0009CF_8889_46CD_B38B_4743C1A75F62__INCLUDED_)
