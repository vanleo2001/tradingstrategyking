#if !defined(AFX_ALARMSETTINGDLG_H__AEC0B0D1_BB7F_435A_A7A9_7DC5361AB528__INCLUDED_)
#define AFX_ALARMSETTINGDLG_H__AEC0B0D1_BB7F_435A_A7A9_7DC5361AB528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmSettingDlg dialog

class CAlarmSettingDlg : public CPropertyPageEx
{
// Construction
public:
	CAlarmSettingDlg();
	~CAlarmSettingDlg();

	void	ResetListCond( );
	BOOL	GetCurrentCond( ALARMCOND & cond );

// Dialog Data
	//{{AFX_DATA(CAlarmSettingDlg)
	enum { IDD = IDD_ALARMSETTING };
	CButton	m_btnModify;
	CListBox	m_listCond;
	CButton	m_btnDelete;
	CButton	m_btnAdd;
	CEdit	m_editTradeVolume;
	CEdit	m_editDiffPercentLess;
	CEdit	m_editDiffPercentMore;
	CSearchBox	m_cmbStock;
	CString	m_strDiffPercentMore;
	CString	m_strDiffPercentLess;
	CString	m_strTradeVolume;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlarmSettingDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnRadioDiffpercentmore();
	afx_msg void OnRadioDiffpercentless();
	afx_msg void OnRadioTradevolume();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnSelchangeListcond();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMSETTINGDLG_H__AEC0B0D1_BB7F_435A_A7A9_7DC5361AB528__INCLUDED_)
