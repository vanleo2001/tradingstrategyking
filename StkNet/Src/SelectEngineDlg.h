#if !defined(AFX_SELECTENGINEDLG_H__92BB8475_56FD_4DF5_8767_D5C6C9CA6A27__INCLUDED_)
#define AFX_SELECTENGINEDLG_H__92BB8475_56FD_4DF5_8767_D5C6C9CA6A27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectEngineDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectEngineDlg dialog

class CSelectEngineDlg : public CDialog
{
// Construction
public:
	CSelectEngineDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL	GetSelectedDriver( CStringArray & astrProvider, CStringArray & astrDriver );

// Dialog Data
	//{{AFX_DATA(CSelectEngineDlg)
	enum { IDD = IDD_SELECTENGINE };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CButton	m_btnSetup;
	CCheckListBox	m_listEngine;
	BOOL	m_bAutoStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectEngineDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectEngineDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSetupreceiver();
	afx_msg void OnSelchangeListengine();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CStringArray	m_astrProvider;
	CStringArray	m_astrDriver;

	CSTKDRV			m_drv;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTENGINEDLG_H__92BB8475_56FD_4DF5_8767_D5C6C9CA6A27__INCLUDED_)
