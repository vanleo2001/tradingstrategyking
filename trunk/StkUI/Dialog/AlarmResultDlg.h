#if !defined(AFX_ALARMRESULTDLG_H__222B0868_0B4A_4784_8BED_FBDBB72A5918__INCLUDED_)
#define AFX_ALARMRESULTDLG_H__222B0868_0B4A_4784_8BED_FBDBB72A5918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmResultDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmResultDlg dialog

class CAlarmResultDlg : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CAlarmResultDlg)

// Construction
public:
	CAlarmResultDlg();
	~CAlarmResultDlg();

// Dialog Data
	//{{AFX_DATA(CAlarmResultDlg)
	enum { IDD = IDD_ALARMRESULT };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAlarmResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAlarmResultDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMRESULTDLG_H__222B0868_0B4A_4784_8BED_FBDBB72A5918__INCLUDED_)
