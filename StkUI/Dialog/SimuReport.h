#if !defined(AFX_SIMUREPORT_H__4E52BCB0_344B_439E_90DB_CAB43552CA6D__INCLUDED_)
#define AFX_SIMUREPORT_H__4E52BCB0_344B_439E_90DB_CAB43552CA6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuReport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpRecordListCtrl
/***
	策略报表ListCtrl
*/
class CReportListCtrl : public CListCtrl
{
public:
	int		GetSelected( );

	BOOL	InitOpRecordList( );
	BOOL	LoadOpRecordList( COpRecordContainer & container );

	BOOL	InitNextOpList( );
	BOOL	LoadNextOpList( COpRecordContainer & container );
	
	BOOL	InitStockOwnList( );
	BOOL	LoadStockOwnList( CStockOwnContainer & stockown, double dCash, CSPTime tmCur, double dAsset, CTechStockContainer & techstocks );

	void	Copy( );
};

/////////////////////////////////////////////////////////////////////////////
// CCurveButton
/***
	策略变动曲线
*/
class CCurveButton : public CButton
{
public:
	CCurveButton( );
	~CCurveButton( );

	void	SetAssetSerial( CAssetSerialContainer & src, double dStartAmount );

	BOOL	Initialize( );

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	virtual void PreSubclassWindow();
	BOOL	DrawCurve( CDC * pDC, CRect rectClient, int nStart );
	BOOL	PrepareDrawData(CRect rectClient, int nStart, int nWeight,
						   LPRECT prectDraw, int *pnStartNew, int *pnCount, double *pdMin, double *pdMax );

	void	ResetScrollBars();
	int		GetScrollPos32(int nBar, BOOL bGetTrackPos  = FALSE );
	BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE );
	int		GetScrollPage( int nBar );

	//{{AFX_MSG(CCurveButton)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CAssetSerialContainer	m_AssetSerial;
	double				m_dStartAmount;
	int					m_nStepWeight;
	int					m_nHScrollMax;
};

/////////////////////////////////////////////////////////////////////////////
// CSimuReport dialog
/***
	策略报告对话框
*/
class CSimuReport : public CXFDialog
{
// Construction
public:
	CSimuReport(CWnd* pParent = NULL);   // standard constructor
	~CSimuReport();

	// operation
	void	SetStrategy( CStrategy * pStrategy );

// Dialog Data
	//{{AFX_DATA(CSimuReport)
	enum { IDD = IDD_SIMUREPORT };
	CButton	m_btnOK;
	CEdit	m_editXiapu;
	CEdit	m_editYieldStdDev;
	CStatic	m_staticExplain;
	CCurveButton	m_curve;
	CButton	m_btnCopy;
	CEdit	m_editYieldIndex;
	CEdit	m_editYieldFin;
	CEdit	m_editYieldMin;
	CEdit	m_editYieldMax;
	CEdit	m_editVictoryTimes;
	CEdit	m_editBuyTimes;
	CReportListCtrl	m_listOpRecord;
	CReportListCtrl	m_listStockOwn;
	CTabCtrl	m_tabctrl;
	//}}AFX_DATA

protected:
	CStrategy	*	m_pStrategy;

	BOOL		SetResults( );

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSimuReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSimuReport)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabctrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUREPORT_H__4E52BCB0_344B_439E_90DB_CAB43552CA6D__INCLUDED_)
