#if !defined(AFX_LISTEXPORTDLG_H__573849DE_65B5_4266_AA2E_547A89CAF6FC__INCLUDED_)
#define AFX_LISTEXPORTDLG_H__573849DE_65B5_4266_AA2E_547A89CAF6FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListExportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// class CExportDB
/***
	数据导出类，用于导出到Txt，Mdb，Csv，ODBC等
*/
class CExportDB
{
public:
	CExportDB( );
	~CExportDB( );

	static	CString	ESCString( CString &string );
	static	CString	ESCCSVString( CString &string );
	
	static	BOOL	ExecuteODBCFile( CString strODBCDest );

	// Attribute
	CString	GetODBCConnect( ) { return m_dbODBC.GetConnect(); }
	BOOL	SetTableColumn( CStringArray & astrColumnName, CUIntArray & anWidth );

	BOOL	OpenTxtDB( LPCTSTR lpszTxtDest, BOOL bTxtPath );
	BOOL	OpenTxtTable( LPCTSTR lpszTableName, BOOL bOverwrite );
	BOOL	OpenMdbDB( LPCTSTR lpszMdbDest );
	BOOL	OpenMdbTable( LPCTSTR lpszTableName, BOOL bOverwrite );
	BOOL	OpenCsvDB( LPCTSTR lpszCsvDest, BOOL bCsvPath );
	BOOL	OpenCsvTable( LPCTSTR lpszTableName, BOOL bOverwrite );
	BOOL	OpenODBCDB( LPCTSTR lpszDSN );
	BOOL	OpenODBCTable( LPCTSTR lpszTableName, BOOL bOverwrite );
	BOOL	AddItemTxt( CStringArray &astrItemText );
	BOOL	AddItemMdb( CStringArray &astrItemText );
	BOOL	AddItemCsv( CStringArray &astrItemText );
	BOOL	AddItemODBC( CStringArray &astrItemText );
	void	CloseTxtDB( );
	void	CloseTxtTable( );
	void	CloseMdbDB( );
	void	CloseMdbTable( );
	void	CloseCsvDB( );
	void	CloseCsvTable( );
	void	CloseODBCDB( );
	void	CloseODBCTable( );

	BOOL	OpenAllDB(CString strTxt, BOOL bTxtPath, CString strMdb, CString strCsv, BOOL bCsvPath, CString strODBC, BOOL bReportError);
	BOOL	OpenAllTable( LPCTSTR lpszTableName, BOOL bOverwrite, BOOL bReportError );
	BOOL	AddItem( CStringArray &astrItemText );
	void	CloseAllDB( );
	void	CloseAllTable( );
protected:
	// Columne Info
	CString			m_strTableName;
	CStringArray	m_astrColumnName;
	CUIntArray		m_anColumnWidth;

	// Txt Dest
	CString			m_strTxtDest;
	BOOL			m_bTxtPath;
	CStdioFile		m_fileTxt;
	// Mdb Dest
	CDaoDatabase	m_mdb;
	CDaoTableDef	* m_pTableDef;
	CDaoRecordset	* m_pRecord;
	// CSV Dest
	CString			m_strCsvDest;
	BOOL			m_bCsvPath;
	CStdioFile		m_fileCsv;
	// Advnaced ODBC Database
	CDatabase		m_dbODBC;
	CString			m_strODBCTableName;
};


/////////////////////////////////////////////////////////////////////////////
// CListExportDlg dialog
/***
	股票列表导出对话框
*/
class CListExportDlg : public CXFDialog
{
// Construction
public:
	CListExportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual	~CListExportDlg( );

	void	StoreProfile( );
	void	LoadProfile( );

	BOOL	ExportBegin( CStringArray &astrColumnName, CUIntArray &anWidth, BOOL bReportError );
	BOOL	ExportOpenTable( CSPTime &time, BOOL	bReportError );
	int		ExportAddItem( CStringArray &astrItemText );
	void	ExportCloseTable( );
	BOOL	ExportFinish( );
	void	SetProgressRange( int nLower, int nUpper );
	void	SetProgress( int nPos );

	// dialog init value
	int		m_nItemMax;

	// dialog return value
	BOOL	m_bItemAll;
	BOOL	m_bItemSelected;
	BOOL	m_bItemDefine;

	BOOL	m_bOverwrite;

// Dialog Data
	//{{AFX_DATA(CListExportDlg)
	enum { IDD = IDD_LISTEXPORT };
	CButton	m_btnCancel;
	CButton	m_btnOK;
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
	CEdit	m_editItemEnd;
	CEdit	m_editItemBegin;
	int		m_nItemBegin;
	int		m_nItemEnd;
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

protected:
	CExportDB	m_exportdb;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CListExportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemAll();
	afx_msg void OnItemSelected();
	afx_msg void OnItemDefine();
	afx_msg void OnTxtCheck();
	afx_msg void OnTxtExplorer();
	afx_msg void OnMdbCheck();
	afx_msg void OnMdbExplorer();
	afx_msg void OnCsvCheck();
	afx_msg void OnCsvExplorer();
	afx_msg void OnODBCCheck();
	afx_msg void OnOdbcSource();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTEXPORTDLG_H__573849DE_65B5_4266_AA2E_547A89CAF6FC__INCLUDED_)
