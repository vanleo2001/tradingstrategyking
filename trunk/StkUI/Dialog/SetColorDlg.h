#if !defined(AFX_SETCOLORDLG_H__AAA444B5_A158_49D8_8548_A8C3862CA6CE__INCLUDED_)
#define AFX_SETCOLORDLG_H__AAA444B5_A158_49D8_8548_A8C3862CA6CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetColorDlg dialog
/***
	设置颜色对话框
*/
class CSetColorDlg : public CXFDialog
{
// Construction
public:
	CSetColorDlg(CWnd* pParent = NULL);   // standard constructor

	void	LoadFromColorClass( CColorClass & cls );
	void	StoreToColorClass( CColorClass & cls );

// Dialog Data
	//{{AFX_DATA(CSetColorDlg)
	enum { IDD = IDD_SETCOLOR };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CButton	m_btnPreview;
	CButton	m_btnSaveAs;
	CButton	m_btnLine6;
	CButton	m_btnLine5;
	CButton	m_btnLine4;
	CButton	m_btnLine3;
	CButton	m_btnLine2;
	CButton	m_btnLine1;
	CButton	m_btnText;
	CButton	m_btnTitle;
	CButton	m_btnDJ;
	CButton	m_btnCW;
	CButton	m_btnNewKLine;
	CButton	m_btnPlane;
	CButton	m_btnFallEntity;
	CButton	m_btnFall;
	CButton	m_btnRise;
	CButton	m_btnBorder;
	CButton	m_btnGraphBK;
	CButton	m_btnBaseText;
	CButton	m_btnBaseBK;
	CButton	m_btnSListSelected;
	CButton	m_btnSListBK;
	CButton	m_btnSimuText;
	CButton	m_btnSimuBK;
	CButton	m_btnWorkspText;
	CButton	m_btnWorkspBK;
	CButton	m_btnDelete;
	CComboBox	m_comboColorClass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString		m_strNameCurrent;
	CColorClass	m_clsOriginal;

	// Generated message map functions
	//{{AFX_MSG(CSetColorDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPreview();
	afx_msg void OnSaveas();
	afx_msg void OnDelete();
	afx_msg void OnSelchangeColorclass();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCOLORDLG_H__AAA444B5_A158_49D8_8548_A8C3862CA6CE__INCLUDED_)
