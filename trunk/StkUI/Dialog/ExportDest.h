#if !defined(AFX_EXPORTDEST_H__B3FD4321_9EE4_439D_8ABC_FA6802348D10__INCLUDED_)
#define AFX_EXPORTDEST_H__B3FD4321_9EE4_439D_8ABC_FA6802348D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDest.h : header file
//
#include	"ListExportDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CExportDest dialog
class	CExportOption;
/***
	K线数据导出对话框，指定导出文件和文件类型
*/
class CExportDest : public CPropertyPageEx
{
	DECLARE_DYNCREATE(CExportDest)

// Construction
public:
	CExportDest();
	CExportDest(UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle);
	~CExportDest();

	DWORD		m_dwButtonFlags;

	void	StoreProfile( );
	void	LoadProfile( );

	BOOL	ExportBegin( CStringArray &astrColumnName, CUIntArray &anWidth, BOOL bReportError );
	BOOL	ExportOpenTable( CStock &stock, int ktype, BOOL bReportError );
	int		ExportAddItem( CStringArray &astrItemText );
	void	ExportCloseTable( );
	BOOL	ExportFinish( );
	void	DoExport( );
	void	DoExportStock( CStock &stock, int nProgressStart, int nProgressEnd, CExportOption * pOption );
	BOOL	DoExportKDATAToStringArray( KDATA & kd, CStringArray & astrItemText, BOOL bDayOrMin, DWORD dateBegin, DWORD dateEnd, int nDigitBits );
	void	SetProgressRange( int nLower, int nUpper );
	void	SetProgress( int nPos );

	// dialog init value

	// dialog return value
	BOOL	m_bOverwrite;

// Dialog Data
	//{{AFX_DATA(CExportDest)
	enum { IDD = IDD_EXPORTDEST };
	CButton	m_btnODBCSource;
	CEdit	m_editODBCDest;
	CButton	m_btnODBCCheck;
	CStatic	m_staticProgress;
	CProgressCtrl	m_ctrlProgress;
	CButton	m_btnCsvExplorer;
	CEdit	m_editCsvDest;
	CButton	m_btnCsvCheck;
	CButton	m_btnMdbExplorer;
	CEdit	m_editMdbDest;
	CButton	m_btnMdbCheck;
	CButton	m_btnTxtExplorer;
	CEdit	m_editTxtDest;
	CButton	m_btnTxtCheck;
	CString	m_strTxtDest;
	CString	m_strMdbDest;
	CString	m_strCsvDest;
	BOOL	m_bTxtCheck;
	BOOL	m_bMdbCheck;
	BOOL	m_bCsvCheck;
	BOOL	m_bExeOnFinish;
	BOOL	m_bODBCCheck;
	CString	m_strODBCDest;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CExportDest)
	public:
	virtual LRESULT OnWizardBack();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CExportDB	m_exportdb;
	BOOL		m_bExporting;
	BOOL		m_bCanceled;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExportDest)
	virtual BOOL OnInitDialog();
	afx_msg void OnTxtCheck();
	afx_msg void OnTxtExplorer();
	afx_msg void OnMdbCheck();
	afx_msg void OnMdbExplorer();
	afx_msg void OnCsvCheck();
	afx_msg void OnCsvExplorer();
	afx_msg void OnODBCCheck();
	afx_msg void OnOdbcSource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTDEST_H__B3FD4321_9EE4_439D_8ABC_FA6802348D10__INCLUDED_)
