#if !defined(AFX_EXPORTOPTION_H__397174B1_B794_4182_AD46_192A64265D78__INCLUDED_)
#define AFX_EXPORTOPTION_H__397174B1_B794_4182_AD46_192A64265D78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportOption dialog
/***
	K线数据导出选项对话框，选择导出日期，是否除权等
*/
class CExportOption : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CExportOption)

// Construction
public:
	CExportOption();
	CExportOption(UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle);
	~CExportOption();

	DWORD		m_dwButtonFlags;

	void	StoreProfile( );
	void	LoadProfile( );

	// dialog return value
	BOOL	m_bTimeAll;
	BOOL	m_bTimeDefine;

	BOOL	m_bNoResumeDR;
	BOOL	m_bResumeDRUp;
	BOOL	m_bResumeDRDown;


// Dialog Data
	//{{AFX_DATA(CExportOption)
	enum { IDD = IDD_EXPORTOPTION };
	CButton	m_btnAutoResumeDR;
	CDateTimeCtrl	m_ctrlTimeAutoResumeDRBegin;
	CEdit	m_editAutoResumeDRLimit;
	CDateTimeCtrl	m_ctrlTimeEnd;
	CDateTimeCtrl	m_ctrlTimeBegin;
	CSPTime	m_tmBegin;
	CSPTime	m_tmEnd;
	BOOL	m_bKMonth;
	BOOL	m_bKWeek;
	BOOL	m_bKDay;
	BOOL	m_bKMin60;
	BOOL	m_bKMin30;
	BOOL	m_bKMin15;
	BOOL	m_bKMin5;
	BOOL	m_bAutoResumeDR;
	CSPTime	m_tmAutoResumeDRBegin;
	UINT	m_nAutoResumeDRLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CExportOption)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExportOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimeAll();
	afx_msg void OnTimeDefine();
	afx_msg void OnNoresumedr();
	afx_msg void OnResumedrup();
	afx_msg void OnResumedrdown();
	afx_msg void OnAutoresumedr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTOPTION_H__397174B1_B794_4182_AD46_192A64265D78__INCLUDED_)
