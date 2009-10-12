// WizardView.h : interface of the CWizardView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIZARDVIEW_H__2C0009CF_8889_46CD_B38B_4743C1A75F62__INCLUDED_)
#define AFX_WIZARDVIEW_H__2C0009CF_8889_46CD_B38B_4743C1A75F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/***
	œÚµº ”Õº
*/
class CWizardView : public CFormView
{
protected: // create from serialization only
	CWizardView();
	DECLARE_DYNCREATE(CWizardView)

public:
	//{{AFX_DATA(CWizardView)
	enum { IDD = IDD_WIZARD_FORM };
	CClickLink	m_linkInfo;
	CClickLink	m_linkBase;
	CClickLink	m_linkGraph;
	CClickLink	m_linkRealTime;
	CSearchBox	m_cmbStock;
	CEdit	m_editSoftNO;
	CButton	m_btnListType;
	CStatic	m_staticSoftNOTitle;
	CHyperLink	m_linkGoPurchase;
	CClickLink	m_linkHowtoReg;
	CButton	m_btnHelp;
	CButton	m_btnOpenStrategy;
	CButton	m_btnNewStrategy;
	CListCtrl	m_listRecent;
	//}}AFX_DATA

// Attributes
public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardView)
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
	virtual ~CWizardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CImageList	m_SmallImageList;
	CImageList	m_LargeImageList;

// Generated message map functions
protected:
	//{{AFX_MSG(CWizardView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnnewstrategy();
	afx_msg void OnBtnopenstrategy();
	afx_msg void OnBtnhelp();
	afx_msg void OnDblclkStrategylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWizardviewLargeicons();
	afx_msg void OnWizardviewSmallicons();
	afx_msg void OnWizardviewList();
	afx_msg void OnWizardviewDetails();
	afx_msg void OnUpdateWizardviewLargeicons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWizardviewSmallicons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWizardviewList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWizardviewDetails(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDVIEW_H__2C0009CF_8889_46CD_B38B_4743C1A75F62__INCLUDED_)
