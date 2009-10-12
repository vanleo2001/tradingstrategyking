// ListExportDlg.cpp : implementation file
//

#include "stdafx.h"

#include "ListExportDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class CExportDB

CExportDB::CExportDB( )
{
	m_pTableDef		=	NULL;
	m_pRecord		=	NULL;
}

CExportDB::~CExportDB( )
{
	CloseAllDB( );
}

CString CExportDB::ESCString( CString &string )
{
	CString	strResult;
	for( int i=0; i<string.GetLength(); i++ )
	{
		char ch = string.GetAt(i);
		if( ch == '\\' || ch == '\'' )
			strResult	+=	"\\";
		strResult	+=	ch;
	}
	return strResult;
}

CString CExportDB::ESCCSVString( CString &string )
{
	CString	strResult;
	for( int i=0; i<string.GetLength(); i++ )
	{
		char ch = string.GetAt(i);
		if( ch == '\\' || ch == '\'' || ch == ',' )
			strResult	+=	"\\";
		strResult	+=	ch;
	}
	return strResult;
}

BOOL CExportDB::ExecuteODBCFile( CString strODBCDest )
{
	if( -1 != strODBCDest.Find( "DSN=Excel" )
		|| -1 != strODBCDest.Find( "DSN=MS Access Database" ) )
	{
		int nIndex	=	strODBCDest.Find( "DBQ=" );
		if( -1 != nIndex )
		{
			CString	strDBQ	=	strODBCDest.Mid( nIndex+4 );
			nIndex	=	strDBQ.Find( ";" );
			if( -1 != nIndex )
				strDBQ	=	strDBQ.Left( nIndex );
			ShellExecute( NULL, "open", strDBQ, NULL, NULL, SW_SHOW );
			return TRUE;
		}
	}
	else
	{
		int nIndex	=	strODBCDest.Find( "DefaultDir=" );
		if( -1 != nIndex )
		{
			CString	strDefaultDir	=	strODBCDest.Mid( nIndex + 11 );
			nIndex	=	strDefaultDir.Find( ";" );
			if( -1 != nIndex )
				strDefaultDir	=	strDefaultDir.Left( nIndex );
			ShellExecute( NULL, "open", strDefaultDir, NULL, NULL, SW_SHOW );
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CExportDB::SetTableColumn( CStringArray & astrColumnName, CUIntArray & anWidth )
{
	ASSERT( astrColumnName.GetSize() == anWidth.GetSize() );
	if( astrColumnName.GetSize() != anWidth.GetSize() )
		return FALSE;

	m_astrColumnName.Copy( astrColumnName );
	m_anColumnWidth.Copy( anWidth );
	ASSERT( m_astrColumnName.GetSize() > 0 );
	return m_astrColumnName.GetSize() > 0;
}

BOOL CExportDB::OpenTxtDB( LPCTSTR lpszTxtDest, BOOL bTxtPath )
{
	if( NULL == lpszTxtDest || strlen(lpszTxtDest) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( bTxtPath && FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( lpszTxtDest ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	CloseTxtDB( );
	m_strTxtDest	=	lpszTxtDest;
	m_bTxtPath		=	bTxtPath;
	return TRUE;
}

BOOL CExportDB::OpenTxtTable( LPCTSTR lpszTableName, BOOL bOverwrite )
{
	if( NULL == lpszTableName || strlen(lpszTableName) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	if( m_strTxtDest.GetLength() == 0		/* Txt DB Not Open */
		|| m_astrColumnName.GetSize() == 0	/* Not Set Table Column */ )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	
	// Path Txt DB
	if( m_bTxtPath && FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( m_strTxtDest ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	CloseTxtTable( );

	// Path Txt DB
	CString	strTxtFile	=	m_strTxtDest;
	if( m_bTxtPath )
		strTxtFile	=	m_strTxtDest + lpszTableName + ".txt";

	UINT nOpenFlags = CFile::modeCreate | CFile::modeWrite;
	if( !bOverwrite )
		nOpenFlags	|=	CFile::modeNoTruncate;
	if( !m_fileTxt.Open( strTxtFile, nOpenFlags ) )
		return FALSE;

	m_fileTxt.SeekToEnd();

	if( bOverwrite )
	{
		m_fileTxt.WriteString( CString(lpszTableName) + "\n"/*STRING_CRLF*/ );

		int	nStrLenTotal = 0, cxTotal = 0;
		for( int nCol=0; nCol < m_astrColumnName.GetSize(); nCol ++ )
		{
			int	nColWidth	=	m_anColumnWidth[nCol];
			CString	strText	=	m_astrColumnName.ElementAt(nCol);
			strText	+=	" ";
			while( ( (nStrLenTotal+strText.GetLength()) * 6 ) < cxTotal+nColWidth )
				strText	=	" " + strText;

			m_fileTxt.WriteString( strText );
			nStrLenTotal	+=	strText.GetLength();
			cxTotal			+=	nColWidth;
		}
		m_fileTxt.WriteString( "\n"/*STRING_CRLF*/ );
	}
	return TRUE;
}

BOOL CExportDB::OpenMdbDB( LPCTSTR lpszMdbDest )
{
	if( NULL == lpszMdbDest || strlen(lpszMdbDest) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	TRY
	{
		CloseMdbDB( );

		if( 0 == access( lpszMdbDest, 0 ) )
			m_mdb.Open( lpszMdbDest );
		else
			m_mdb.Create( lpszMdbDest );
	}
	CATCH( CDaoException, e )
	{
		CloseMdbDB();
		e->ReportError( );
		return FALSE;
	}
	END_CATCH

	return m_mdb.IsOpen();
}

BOOL CExportDB::OpenMdbTable( LPCTSTR lpszTableName, BOOL bOverwrite )
{
	if( NULL == lpszTableName || strlen(lpszTableName) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	TRY
	{
		if( !m_mdb.IsOpen() || m_astrColumnName.GetSize() <= 0 )
		{
			ASSERT( FALSE );
			return FALSE;
		}
		CloseMdbTable();

		CString	strTableName	=	lpszTableName;

		BOOL	bCreateNew	=	bOverwrite;
		if( !bOverwrite )
		{
			try {
				CDaoTableDefInfo	tabledefinfo;
				m_mdb.GetTableDefInfo( strTableName, tabledefinfo );

				CDaoTableDef	tabledef( &m_mdb );
				tabledef.Open( strTableName );
				if( !tabledef.IsOpen() )
					bCreateNew	=	TRUE;
				else
				{
					if( m_astrColumnName.GetSize() != tabledef.GetFieldCount() )
						bCreateNew	=	TRUE;
					CDaoFieldInfo	fieldinfo;
					for( int i=0; i<tabledef.GetFieldCount(); i++ )
					{
						tabledef.GetFieldInfo( i, fieldinfo, AFX_DAO_ALL_INFO );
						bCreateNew	|=	( 0 != fieldinfo.m_strName.Compare(m_astrColumnName[i]) );
						bCreateNew	|=	( dbText != fieldinfo.m_nType );
						bCreateNew	|=	( 255 != fieldinfo.m_lSize );
						bCreateNew	|=	(! ( (dbVariableField | dbUpdatableField) & fieldinfo.m_lAttributes) );
					}
					tabledef.Close();
				}
			}catch( CDaoException * e ) {
				e->Delete();
				bCreateNew	=	TRUE;
			}
		}

		if( bCreateNew )
		{
			// Create New Table
			int	nCount	=	1;
			while( TRUE )
			{
				try {
					if( 1 != nCount )
						strTableName.Format( "%s%d", lpszTableName, nCount );
					CDaoTableDefInfo	tabledefinfo;
					m_mdb.GetTableDefInfo( strTableName, tabledefinfo );
				}catch ( CDaoException * e){
					e->Delete();
					break;
				}
				nCount	++;
			}

			//	create strTableName.
			CDaoTableDef	tabledef( &m_mdb );
			tabledef.Create( strTableName );

			//	def strTableName's field.
			CDaoFieldInfo	fieldinfo;

			fieldinfo.m_bRequired	=	false;
			fieldinfo.m_lCollatingOrder	=	dbSortGeneral;
			fieldinfo.m_strSourceTable	=	strTableName;
			for( int i=0; i<m_astrColumnName.GetSize(); i++ )
			{
				fieldinfo.m_bAllowZeroLength	=	true;
				fieldinfo.m_nOrdinalPosition	=	i+1;
				fieldinfo.m_strName	=	ESCString(m_astrColumnName[i]);
				fieldinfo.m_lSize	=	255;
				fieldinfo.m_nType		=	dbText;
				fieldinfo.m_lAttributes	=	dbVariableField | dbUpdatableField;
				fieldinfo.m_strSourceField	=	ESCString(m_astrColumnName[i]);
				tabledef.CreateField( fieldinfo );
			}
			tabledef.Append( );
			tabledef.Close( );
			// CreateNew End
		}

		//	Open Table
		if( !m_mdb.IsOpen( ) )
			return FALSE;
		m_pTableDef	=	new	CDaoTableDef( &m_mdb );
		m_pTableDef->Open( strTableName );
		m_pRecord	=	new	CDaoRecordset( &m_mdb );
		m_pRecord->Open( m_pTableDef, dbOpenDynaset );
		if( !m_pRecord->IsOpen() || !m_pTableDef->IsOpen() )
		{
			CloseMdbTable();
			return FALSE;
		}
	}
	CATCH( CDaoException, e )
	{
		CloseMdbTable();
		e->ReportError( );
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CExportDB::OpenCsvDB( LPCTSTR lpszCsvDest, BOOL bCsvPath )
{
	if( NULL == lpszCsvDest || strlen(lpszCsvDest) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( bCsvPath && FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( lpszCsvDest ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	CloseCsvDB( );
	m_strCsvDest	=	lpszCsvDest;
	m_bCsvPath		=	bCsvPath;
	return TRUE;
}

BOOL CExportDB::OpenCsvTable( LPCTSTR lpszTableName, BOOL bOverwrite )
{
	if( NULL == lpszTableName || strlen(lpszTableName) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	if( m_strCsvDest.GetLength() == 0		/* Csv DB Not Open */
		|| m_astrColumnName.GetSize() == 0	/* Not Set Table Column */ )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	// Path Csv DB
	if( m_bCsvPath && FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( m_strCsvDest ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	CloseCsvTable( );

	CString	strCsvFile	=	m_strCsvDest;
	if( m_bCsvPath )
		strCsvFile	=	m_strCsvDest + lpszTableName + ".csv";

	UINT nOpenFlags = CFile::modeCreate | CFile::modeWrite;
	if( !bOverwrite )
		nOpenFlags	|=	CFile::modeNoTruncate;
	if( !m_fileCsv.Open( strCsvFile, nOpenFlags ) )
	{
		return FALSE;
	}

	m_fileCsv.SeekToEnd();

	if( bOverwrite )
	{
		m_fileCsv.WriteString( CString(lpszTableName) + "\n" );
		
		CString	strText;
		for( int nCol=0; nCol < m_astrColumnName.GetSize(); nCol ++ )
		{
			int	nColWidth	=	m_anColumnWidth[nCol];
			if( strText.GetLength() > 0 )
				strText	+=	",";
			strText	+=	m_astrColumnName.ElementAt(nCol);
		}
		m_fileCsv.WriteString( strText );
		m_fileCsv.WriteString( "\n" );
	}
	return TRUE;
}

BOOL CExportDB::OpenODBCDB( LPCTSTR lpszDSN )
{
	TRY
	{
		CloseODBCDB( );

		if( !m_dbODBC.Open(lpszDSN, FALSE, FALSE, _T("ODBC;"), FALSE) )
			return FALSE;

		return TRUE;
	}
	CATCH(CException, e)
	{
		CloseODBCDB( );
		if(e)	e->ReportError( );
		return FALSE;
	}
	END_CATCH
}

BOOL CExportDB::OpenODBCTable( LPCTSTR lpszTableName, BOOL bOverwrite )
{
	if( NULL == lpszTableName || strlen(lpszTableName) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	TRY
	{
		if( !m_dbODBC.IsOpen() || m_astrColumnName.GetSize() <= 0 )
		{
			ASSERT( FALSE );
			return FALSE;
		}
		CloseODBCTable();

		// Get Next Table Name
		CString	strTableName	=	lpszTableName;

		BOOL	bCreateNew	=	bOverwrite;
		if( !bOverwrite )
		{
			try {
				CRecordset	rec( &m_dbODBC );
				if( !rec.Open( CRecordset::dynaset, "SELECT * FROM \"" + strTableName + "\"" ) )
					bCreateNew	=	TRUE;
				else
				{
					if( m_astrColumnName.GetSize() != rec.GetODBCFieldCount() )
						bCreateNew	=	TRUE;
					CODBCFieldInfo	fieldinfo;
					for( int i=0; i<rec.GetODBCFieldCount(); i++ )
					{
						rec.GetODBCFieldInfo( i, fieldinfo );
						bCreateNew	|=	( 0 != fieldinfo.m_strName.Compare(m_astrColumnName[i]) );
					}
					rec.Close();
				}
			}catch( CDaoException * e) {
				e->Delete();
				bCreateNew	=	TRUE;
			}
		}

		if( bCreateNew )
		{
			// Craete New Table
			int	nCount	=	1;
			while( TRUE )
			{
				try {
					if( 1 != nCount )
						strTableName.Format( "%s%d", lpszTableName, nCount );
					CRecordset	rec( &m_dbODBC );
					if( !rec.Open( CRecordset::dynaset, "SELECT * FROM \"" + strTableName + "\"" ) )
						break;
					rec.Close( );
				}catch ( CDBException * e ){
					e->Delete();
					break;
				}
				nCount	++;
			}

			// Create Table
			CString	sqlCreateTable	=	"CREATE TABLE \"" + strTableName + "\" (";
			for( int k=0; k<m_astrColumnName.GetSize(); k++ )
				sqlCreateTable	+=	( "\"" + ESCString(m_astrColumnName[k]) + "\"" + ( k==m_astrColumnName.GetSize()-1 ? " VARCHAR(255))" : " VARCHAR(255)," ) );
			m_dbODBC.ExecuteSQL( sqlCreateTable );
			// CreateNew End
		}

		m_strODBCTableName	=	strTableName;

		// Test
		CRecordset	rs( &m_dbODBC );
		if( !rs.Open( CRecordset::dynaset, "SELECT * FROM \"" + strTableName + "\"" ) )
			return FALSE;
		rs.Close();
		return TRUE;
	}
	CATCH(CException, e)
	{
		CloseODBCTable( );
		if(e)	e->ReportError( );
		return FALSE;
	}
	END_CATCH
}

BOOL CExportDB::AddItemTxt( CStringArray &astrItemText )
{
	if( CFile::hFileNull != m_fileTxt.m_hFile )
	{
		m_fileTxt.SeekToEnd();
		int	nStrLenTotal = 0, cxTotal = 0;
		for( int nCol=0; nCol < astrItemText.GetSize() && nCol < m_anColumnWidth.GetSize(); nCol ++ )
		{
			int	nColWidth	=	m_anColumnWidth[nCol];
			CString	strText	=	astrItemText.ElementAt(nCol);
			strText	=	" " + strText;
			while( ( (nStrLenTotal+strText.GetLength()) * 6 ) < cxTotal+nColWidth )
				strText	=	" " + strText;

			m_fileTxt.WriteString( strText );
			nStrLenTotal	+=	strText.GetLength();
			cxTotal			+=	nColWidth;
		}
		m_fileTxt.WriteString( "\n"/*STRING_CRLF*/ );
		return TRUE;
	}
	return FALSE;
}

BOOL CExportDB::AddItemMdb( CStringArray &astrItemText )
{
	TRY	{
		if( m_pRecord && m_pRecord->IsOpen() && m_mdb.IsOpen() )
		{
			m_pRecord->AddNew( );
			for( int i=0; i<astrItemText.GetSize(); i++ )
			{
				CString	strText = astrItemText.ElementAt(i);
				if( strText.GetLength() >= 255 )
					strText	=	strText.Left( 254 );
				if( strText.GetLength() > 0 && strText.GetLength() < 255 )
					m_pRecord->SetFieldValue( i, LPCTSTR(ESCString(strText)) );
			}
			m_pRecord->Update( );
			return TRUE;
		}
	}CATCH( CException, e )	{
		e->Delete();
	}END_CATCH
	return FALSE;
}

BOOL CExportDB::AddItemCsv( CStringArray &astrItemText )
{
	if( CFile::hFileNull != m_fileCsv.m_hFile )
	{
		m_fileCsv.SeekToEnd();
		CString	strText;
		for( int nCol=0; nCol < astrItemText.GetSize() && nCol < m_anColumnWidth.GetSize(); nCol ++ )
		{
			int	nColWidth	=	m_anColumnWidth[nCol];
			if( strText.GetLength() > 0 )
				strText	+=	",";
			strText	+=	ESCCSVString(astrItemText.ElementAt(nCol));
		}
		m_fileCsv.WriteString( strText );
		m_fileCsv.WriteString( "\n" );
		return TRUE;
	}
	return FALSE;
}

BOOL CExportDB::AddItemODBC( CStringArray &astrItemText )
{
	if( m_dbODBC.IsOpen() )
	{
		CString	sqlInsert	=	"INSERT INTO \"" + m_strODBCTableName + "\" VALUES(";
		for( int k=0; k<astrItemText.GetSize(); k++ )
			sqlInsert	+=	( "\'" + ESCString(astrItemText[k]) + ( k==astrItemText.GetSize()-1 ? "\')" : "\'," ) );

		TRY {
			m_dbODBC.ExecuteSQL( sqlInsert );
			return TRUE;
		} CATCH ( CException, e ) {
			e->Delete();
		} END_CATCH
	}
	return FALSE;
}

void CExportDB::CloseTxtDB( )
{
	CloseTxtTable();
	m_strTxtDest.Empty();
	m_bTxtPath	=	FALSE;
}

void CExportDB::CloseTxtTable( )
{
	if( CFile::hFileNull != m_fileTxt.m_hFile )
		m_fileTxt.Close();
}

void CExportDB::CloseMdbDB( )
{
	try
	{
		CloseMdbTable();
		if( m_mdb.IsOpen() )
			m_mdb.Close();
	}
	catch( CDaoException * e )
	{
		e->Delete();
		TRACE( "Close Mdb Exception\n" );
	}
}

void CExportDB::CloseMdbTable( )
{
	try
	{
		if( m_pRecord && m_pRecord->IsOpen() )
			m_pRecord->Close();
		if( m_pRecord )
		{
			delete m_pRecord;
			m_pRecord	=	NULL;
		}
		if( m_pTableDef && m_pTableDef->IsOpen() )
			m_pTableDef->Close();
		if( m_pTableDef )
		{
			delete	m_pTableDef;
			m_pTableDef	=	NULL;
		}
	}
	catch( CDaoException * e )
	{
		e->Delete();
		TRACE( "Close Mdb Exception\n" );
	}
}

void CExportDB::CloseCsvDB( )
{
	CloseCsvTable( );
	m_strCsvDest.Empty();
	m_bCsvPath	=	FALSE;
}

void CExportDB::CloseCsvTable( )
{
	if( CFile::hFileNull != m_fileCsv.m_hFile )
		m_fileCsv.Close();
}

void CExportDB::CloseODBCDB( )
{
	try
	{
		CloseODBCTable( );
		if( m_dbODBC.IsOpen() )
			m_dbODBC.Close();
	}
	catch(...)
	{
		TRACE(_T("Errors occurred.\n"));
	}
}

void CExportDB::CloseODBCTable( )
{
	TRY
	{
		if( m_dbODBC.IsOpen() )
		{
			// Close and Reopen to Flush Data
			CString strConnect = m_dbODBC.GetConnect();
			m_dbODBC.Close();
			m_dbODBC.Open( strConnect, FALSE, FALSE, _T("ODBC;"), FALSE);
		}
	}
	CATCH(CException, e)
	{
		if(e)	e->ReportError( );
	}
	END_CATCH
}

BOOL CExportDB::OpenAllDB(CString strTxt, BOOL bTxtPath, CString strMdb,
						  CString strCsv, BOOL bCsvPath, CString strODBC, BOOL bReportError )
{
	BOOL	bRet	=	TRUE;
	if( strTxt.GetLength() > 0 )
	{
		bRet	&=	OpenTxtDB( strTxt, bTxtPath );
		if( !bRet && bReportError )
			AfxMessageBox( IDS_EXPORTDB_ERRTXTDEST, MB_OK | MB_ICONINFORMATION );
	}
	if( strMdb.GetLength() > 0 )
	{
		bRet	&=	OpenMdbDB( strMdb );
		if( !bRet && bReportError )
			AfxMessageBox( IDS_EXPORTDB_ERRMDBDEST, MB_OK | MB_ICONINFORMATION );
	}
	if( strCsv.GetLength() > 0 )
	{
		bRet	&=	OpenCsvDB( strCsv, bCsvPath );
		if( !bRet && bReportError )
			AfxMessageBox( IDS_EXPORTDB_ERRCSVDEST, MB_OK | MB_ICONINFORMATION );
	}
	if( strODBC.GetLength() > 0 )
	{
		bRet	&=	OpenODBCDB( strODBC );
		if( !bRet && bReportError )
			AfxMessageBox( IDS_EXPORTDB_ERRODBCDEST, MB_OK | MB_ICONINFORMATION );
	}
	
	if( !bRet )
		CloseAllDB( );
	return bRet;
}

BOOL CExportDB::OpenAllTable( LPCTSTR lpszTableName, BOOL bOverwrite, BOOL bReportError )
{
	if( NULL == lpszTableName || strlen(lpszTableName) <= 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	BOOL	bRet	=	TRUE;
	if( m_strTxtDest.GetLength() > 0 )
	{
		bRet	&=	OpenTxtTable( lpszTableName, bOverwrite );
		if( !bRet && bReportError )
			AfxMessageBox( IDS_EXPORTDB_ERRTXTDEST, MB_OK | MB_ICONINFORMATION );
	}
	if( m_mdb.IsOpen() )
	{
		bRet	&=	OpenMdbTable( lpszTableName, bOverwrite );
		if( !bRet && bReportError )
			AfxMessageBox( IDS_EXPORTDB_ERRMDBDEST, MB_OK | MB_ICONINFORMATION );
	}
	if( m_strCsvDest.GetLength() > 0 )
	{
		bRet	&=	OpenCsvTable( lpszTableName, bOverwrite );
		if( !bRet && bReportError )
			AfxMessageBox( IDS_EXPORTDB_ERRCSVDEST, MB_OK | MB_ICONINFORMATION );
	}
	if( m_dbODBC.IsOpen() )
	{
		bRet	&=	OpenODBCTable( lpszTableName, bOverwrite );
		if( !bRet && bReportError )
			AfxMessageBox( IDS_EXPORTDB_ERRODBCDEST, MB_OK | MB_ICONINFORMATION );
	}
	
	if( !bRet )
		CloseAllTable( );
	return bRet;
}

BOOL CExportDB::AddItem( CStringArray &astrItemText )
{
	BOOL	bRet	=	TRUE;
	if( CFile::hFileNull != m_fileTxt.m_hFile )
	{
		bRet	&=	AddItemTxt( astrItemText );
	}
	if( m_pRecord && m_pRecord->IsOpen() && m_mdb.IsOpen() )
	{
		bRet	&=	AddItemMdb( astrItemText );
	}
	if( CFile::hFileNull != m_fileCsv.m_hFile )
	{
		bRet	&=	AddItemCsv( astrItemText );
	}
	if( m_dbODBC.IsOpen() )
	{
		bRet	&=	AddItemODBC( astrItemText );
	}

	return bRet;
}

void CExportDB::CloseAllDB( )
{
	CloseTxtDB( );
	CloseMdbDB( );
	CloseCsvDB( );
	CloseODBCDB( );
}

void CExportDB::CloseAllTable( )
{
	CloseTxtTable( );
	CloseMdbTable( );
	CloseCsvTable( );
	CloseODBCTable( );
}

/////////////////////////////////////////////////////////////////////////////
// CListExportDlg dialog

void CListExportDlg::StoreProfile( )
{
	AfxGetApp()->WriteProfileInt( "listexportdlg", "txtcheck", m_bTxtCheck );
	AfxGetApp()->WriteProfileInt( "listexportdlg", "mdbcheck", m_bMdbCheck );
	AfxGetApp()->WriteProfileInt( "listexportdlg", "csvcheck", m_bCsvCheck );
	AfxGetApp()->WriteProfileInt( "listexportdlg", "odbccheck", m_bODBCCheck );
	AfxGetApp()->WriteProfileString( "listexportdlg", "txtdest", m_strTxtDest );
	AfxGetApp()->WriteProfileString( "listexportdlg", "mdbdest", m_strMdbDest );
	AfxGetApp()->WriteProfileString( "listexportdlg", "csvdest", m_strCsvDest );
	AfxGetApp()->WriteProfileString( "listexportdlg", "odbcdest", m_strODBCDest );
	AfxGetApp()->WriteProfileInt( "listexportdlg", "exeonfinish", m_bExeOnFinish );
	AfxGetApp()->WriteProfileInt( "listexportdlg", "overwrite", m_bOverwrite );
}

void CListExportDlg::LoadProfile( )
{
	// Initial Data
	m_bTxtCheck		=	TRUE;
	m_bExeOnFinish	=	TRUE;
	m_bOverwrite	=	TRUE;
	m_strTxtDest	=	AfxGetProfile().GetExportPath() + "StockList.txt";
	m_strMdbDest	=	AfxGetProfile().GetExportPath() + "StockList.mdb";
	m_strCsvDest	=	AfxGetProfile().GetExportPath() + "StockList.csv";
	m_strODBCDest	=	"ODBC;DSN=Excel Files;DBQ="
						+ AfxGetProfile().GetExportPath()
						+ "StockList.xls;DefaultDir="
						+ AfxGetProfile().GetExportPath()
						+ ";DriverId=790;MaxBufferSize=2048;PageTimeout=5;";

	m_bTxtCheck		=	AfxGetApp()->GetProfileInt( "listexportdlg", "txtcheck", m_bTxtCheck );
	m_bMdbCheck		=	AfxGetApp()->GetProfileInt( "listexportdlg", "mdbcheck", m_bMdbCheck );
	m_bCsvCheck		=	AfxGetApp()->GetProfileInt( "listexportdlg", "csvcheck", m_bCsvCheck );
	m_bODBCCheck	=	AfxGetApp()->GetProfileInt( "listexportdlg", "odbccheck", m_bODBCCheck );
	m_strTxtDest	=	AfxGetApp()->GetProfileString( "listexportdlg", "txtdest", m_strTxtDest );
	m_strMdbDest	=	AfxGetApp()->GetProfileString( "listexportdlg", "mdbdest", m_strMdbDest );
	m_strCsvDest	=	AfxGetApp()->GetProfileString( "listexportdlg", "csvdest", m_strCsvDest );
	m_strODBCDest	=	AfxGetApp()->GetProfileString( "listexportdlg", "odbcdest", m_strODBCDest );
	m_bExeOnFinish	=	AfxGetApp()->GetProfileInt( "listexportdlg", "exeonfinish", m_bExeOnFinish );
	m_bOverwrite	=	AfxGetApp()->GetProfileInt( "listexportdlg", "overwrite", m_bOverwrite );
}

CListExportDlg::CListExportDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CListExportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListExportDlg)
	m_nItemBegin = 1;
	m_nItemEnd = 1;
	m_strTxtDest = _T("");
	m_strMdbDest = _T("");
	m_strCsvDest = _T("");
	m_bTxtCheck = FALSE;
	m_bMdbCheck = FALSE;
	m_bCsvCheck = FALSE;
	m_bExeOnFinish = FALSE;
	m_bODBCCheck = FALSE;
	m_strODBCDest = _T("");
	//}}AFX_DATA_INIT
	m_nItemMax		=	1;
	m_bItemAll		=	FALSE;
	m_bItemSelected	=	FALSE;
	m_bItemDefine	=	FALSE;
}

CListExportDlg::~CListExportDlg( )
{
}

BOOL CListExportDlg::ExportBegin(CStringArray &astrColumnName, CUIntArray &anWidth,
							BOOL	bReportError )
{
	CString	strTxt, strMdb, strCsv, strODBC;
	if( m_bTxtCheck )	strTxt	=	m_strTxtDest;
	if( m_bMdbCheck )	strMdb	=	m_strMdbDest;
	if( m_bCsvCheck )	strCsv	=	m_strCsvDest;
	if( m_bODBCCheck )	strODBC	=	m_strODBCDest;
	if( !m_exportdb.OpenAllDB(strTxt, FALSE, strMdb, strCsv, FALSE, strODBC,
							bReportError ) )
		return FALSE;

	if( ! m_exportdb.SetTableColumn( astrColumnName, anWidth ) )
		return FALSE;

	return TRUE;
}

BOOL CListExportDlg::ExportOpenTable(CSPTime &time, BOOL	bReportError )
{
	CString	strTableName;
	strTableName.LoadString( IDS_LISTEXPORT_TABLENAME );
	strTableName	+=	AfxGetTimeString( time.GetTime(), "%Y-%m-%d-", TRUE );
	if( !m_exportdb.OpenAllTable( strTableName, m_bOverwrite, bReportError ) )
		return FALSE;
	return TRUE;
}

BOOL CListExportDlg::ExportAddItem( CStringArray &astrItemText )
{
	return m_exportdb.AddItem( astrItemText );
}

void CListExportDlg::ExportCloseTable( )
{
	m_exportdb.CloseAllTable( );
}

BOOL CListExportDlg::ExportFinish( )
{
	m_exportdb.CloseAllDB( );
	
	if( m_bExeOnFinish )
	{
		if( m_bTxtCheck && m_strTxtDest.GetLength() > 0 )
			ShellExecute( NULL, "open", m_strTxtDest, NULL, NULL, SW_SHOW );
		if( m_bMdbCheck && m_strMdbDest.GetLength() > 0 )
			ShellExecute( NULL, "open", m_strMdbDest, NULL, NULL, SW_SHOW );
		if( m_bCsvCheck && m_strCsvDest.GetLength() > 0 )
			ShellExecute( NULL, "open", m_strCsvDest, NULL, NULL, SW_SHOW );
		if( m_bODBCCheck && m_strODBCDest.GetLength() > 0 )
			CExportDB::ExecuteODBCFile( m_strODBCDest );
	}

	// Set Progress
	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
	int	nLower = 0, nUpper = 100;
	m_ctrlProgress.GetRange( nLower, nUpper );

	if( nUpper > nLower )
	{
		m_ctrlProgress.SetPos( nUpper );
		CString	strInfo;
		strInfo.LoadString( IDS_EXPORTDB_FINISHED );
		m_staticProgress.SetWindowText( strInfo );
	}

	return TRUE;
}

void CListExportDlg::SetProgressRange( int nLower, int nUpper )
{
	if( nUpper > nLower )
	{
		m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
		m_ctrlProgress.SetRange32( nLower, nUpper );
	}
}

void CListExportDlg::SetProgress( int nPos )
{
	int	nLower = 0, nUpper = 100;
	m_ctrlProgress.GetRange( nLower, nUpper );

	if( nUpper > nLower )
	{
		m_ctrlProgress.SetPos( nPos );

		CString	strProgressNew;
		strProgressNew.Format( "%d%%", 100*nPos/(nUpper-nLower) );
		CString	strProgressOld;
		m_staticProgress.GetWindowText( strProgressOld );
		if( 0 != strProgressNew.Compare( strProgressOld ) )
			m_staticProgress.SetWindowText( strProgressNew );
	}
}

void CListExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListExportDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_ODBC_SOURCE, m_btnODBCSource);
	DDX_Control(pDX, IDC_ODBC_DEST, m_editODBCDest);
	DDX_Control(pDX, IDC_ODBC_CHECK, m_btnODBCCheck);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_staticProgress);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_CSV_EXPLORER, m_btnCsvExplorer);
	DDX_Control(pDX, IDC_CSV_DEST, m_editCsvDest);
	DDX_Control(pDX, IDC_CSV_CHECK, m_btnCsvCheck);
	DDX_Control(pDX, IDC_MDB_EXPLORER, m_btnMdbExplorer);
	DDX_Control(pDX, IDC_MDB_DEST, m_editMdbDest);
	DDX_Control(pDX, IDC_MDB_CHECK, m_btnMdbCheck);
	DDX_Control(pDX, IDC_TXT_EXPLORER, m_btnTxtExplorer);
	DDX_Control(pDX, IDC_TXT_DEST, m_editTxtDest);
	DDX_Control(pDX, IDC_TXT_CHECK, m_btnTxtCheck);
	DDX_Control(pDX, IDC_ITEM_END, m_editItemEnd);
	DDX_Control(pDX, IDC_ITEM_BEGIN, m_editItemBegin);
	DDX_Text(pDX, IDC_ITEM_BEGIN, m_nItemBegin);
	DDV_MinMaxInt(pDX, m_nItemBegin, 1, m_nItemMax);
	DDX_Text(pDX, IDC_ITEM_END, m_nItemEnd);
	DDV_MinMaxInt(pDX, m_nItemEnd, 1, m_nItemMax);
	DDX_Text(pDX, IDC_TXT_DEST, m_strTxtDest);
	DDX_Text(pDX, IDC_MDB_DEST, m_strMdbDest);
	DDX_Text(pDX, IDC_CSV_DEST, m_strCsvDest);
	DDX_Check(pDX, IDC_TXT_CHECK, m_bTxtCheck);
	DDX_Check(pDX, IDC_MDB_CHECK, m_bMdbCheck);
	DDX_Check(pDX, IDC_CSV_CHECK, m_bCsvCheck);
	DDX_Check(pDX, IDC_EXEONFINISH, m_bExeOnFinish);
	DDX_Check(pDX, IDC_ODBC_CHECK, m_bODBCCheck);
	DDX_Text(pDX, IDC_ODBC_DEST, m_strODBCDest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListExportDlg, CXFDialog)
	//{{AFX_MSG_MAP(CListExportDlg)
	ON_BN_CLICKED(IDC_ITEM_ALL, OnItemAll)
	ON_BN_CLICKED(IDC_ITEM_SELECTED, OnItemSelected)
	ON_BN_CLICKED(IDC_ITEM_DEFINE, OnItemDefine)
	ON_BN_CLICKED(IDC_TXT_CHECK, OnTxtCheck)
	ON_BN_CLICKED(IDC_TXT_EXPLORER, OnTxtExplorer)
	ON_BN_CLICKED(IDC_MDB_CHECK, OnMdbCheck)
	ON_BN_CLICKED(IDC_MDB_EXPLORER, OnMdbExplorer)
	ON_BN_CLICKED(IDC_CSV_CHECK, OnCsvCheck)
	ON_BN_CLICKED(IDC_CSV_EXPLORER, OnCsvExplorer)
	ON_BN_CLICKED(IDC_ODBC_CHECK, OnODBCCheck)
	ON_BN_CLICKED(IDC_ODBC_SOURCE, OnOdbcSource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListExportDlg message handlers

BOOL CListExportDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );

	// select item
	int	nCheckID	=	IDC_ITEM_ALL;
	if( m_bItemSelected )
		nCheckID	=	IDC_ITEM_SELECTED;
	else if( m_bItemDefine )
		nCheckID	=	IDC_ITEM_DEFINE;
	CheckRadioButton( IDC_ITEM_ALL, IDC_ITEM_DEFINE, nCheckID );
	m_editItemBegin.EnableWindow( IDC_ITEM_DEFINE == nCheckID );
	m_editItemEnd.EnableWindow( IDC_ITEM_DEFINE == nCheckID );

	// txt 
	m_btnTxtCheck.SetCheck( m_bTxtCheck );
	m_editTxtDest.EnableWindow( m_bTxtCheck );
	m_btnTxtExplorer.EnableWindow( m_bTxtCheck );
	nCheckID	=	IDC_EXPORT_OVERWRITE;
	if( !m_bOverwrite )
		nCheckID	=	IDC_EXPORT_APPEND;
	CheckRadioButton( IDC_EXPORT_OVERWRITE, IDC_EXPORT_APPEND, nCheckID );

	// mdb
	m_btnMdbCheck.SetCheck( m_bMdbCheck );
	m_editMdbDest.EnableWindow( m_bMdbCheck );
	m_btnMdbExplorer.EnableWindow( m_bMdbCheck );

	// Csv
	m_btnCsvCheck.SetCheck( m_bCsvCheck );
	m_editCsvDest.EnableWindow( m_bCsvCheck );
	m_btnCsvExplorer.EnableWindow( m_bCsvCheck );

	// ODBC
	m_btnODBCCheck.SetCheck( m_bODBCCheck );
	m_editODBCDest.EnableWindow( m_bODBCCheck );
	m_btnODBCSource.EnableWindow( m_bODBCCheck );

	// progress ctrl
	m_ctrlProgress.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CListExportDlg::OnItemAll() 
{
	// TODO: Add your control notification handler code here
	m_editItemBegin.EnableWindow( FALSE );
	m_editItemEnd.EnableWindow( FALSE );
}

void CListExportDlg::OnItemSelected() 
{
	// TODO: Add your control notification handler code here
	m_editItemBegin.EnableWindow( FALSE );
	m_editItemEnd.EnableWindow( FALSE );
}

void CListExportDlg::OnItemDefine() 
{
	// TODO: Add your control notification handler code here
	m_editItemBegin.EnableWindow( TRUE );
	m_editItemEnd.EnableWindow( TRUE );
}

void CListExportDlg::OnTxtCheck() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnTxtCheck.GetCheck();
	m_editTxtDest.EnableWindow( nCheck );
	m_btnTxtExplorer.EnableWindow( nCheck );
}

void CListExportDlg::OnTxtExplorer() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_ENABLESIZING,
		"Txt files (*.txt)|*.txt|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strTxtDest	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CListExportDlg::OnMdbCheck() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnMdbCheck.GetCheck();
	m_editMdbDest.EnableWindow( nCheck );
	m_btnMdbExplorer.EnableWindow( nCheck );
}

void CListExportDlg::OnMdbExplorer() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_ENABLESIZING,
		"Microsoft Access Database files (*.mdb)|*.mdb|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strMdbDest	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CListExportDlg::OnCsvCheck() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnCsvCheck.GetCheck();
	m_editCsvDest.EnableWindow( nCheck );
	m_btnCsvExplorer.EnableWindow( nCheck );
}

void CListExportDlg::OnODBCCheck() 
{
	// TODO: Add your control notification handler code here
	int	nCheck	=	m_btnODBCCheck.GetCheck();
	m_editODBCDest.EnableWindow( nCheck );
	m_btnODBCSource.EnableWindow( nCheck );
}

void CListExportDlg::OnCsvExplorer() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_ENABLESIZING,
		"CSV files (*.csv)|*.csv|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strCsvDest	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CListExportDlg::OnOdbcSource() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_exportdb.OpenODBCDB( NULL ) )
	{
		m_strODBCDest	=	m_exportdb.GetODBCConnect();
		UpdateData( FALSE );
	}
}

void CListExportDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	// if( !AfxAssertFunclimitProf( ) )
	//	return;

	CButton * pbtnAll		= (CButton *)GetDlgItem( IDC_ITEM_ALL );
	if( pbtnAll )	m_bItemAll	=	pbtnAll->GetCheck();
	CButton * pbtnSelected		= (CButton *)GetDlgItem( IDC_ITEM_SELECTED );
	if( pbtnSelected )	m_bItemSelected	=	pbtnSelected->GetCheck();
	CButton * pbtnDefine		= (CButton *)GetDlgItem( IDC_ITEM_DEFINE );
	if( pbtnDefine )	m_bItemDefine	=	pbtnDefine->GetCheck();

	if( !m_bItemAll && !m_bItemSelected && !m_bItemDefine )
		m_bItemAll	=	TRUE;

	CButton * pbtn		= (CButton *)GetDlgItem( IDC_EXPORT_OVERWRITE );
	if( pbtn )	m_bOverwrite	=	pbtn->GetCheck();

	if( !m_bTxtCheck && !m_bMdbCheck && !m_bCsvCheck && !m_bODBCCheck )
	{
		AfxMessageBox( IDS_EXPORTDB_NODEST, MB_OK | MB_ICONINFORMATION );
		return;
	}

	if( m_bTxtCheck && m_strTxtDest.GetLength() == 0 )
	{
		AfxMessageBox( IDS_EXPORTDB_NOTXTDEST, MB_OK | MB_ICONINFORMATION );
		return;
	}
	if( m_bMdbCheck && m_strMdbDest.GetLength() == 0 )
	{
		AfxMessageBox( IDS_EXPORTDB_NOMDBDEST, MB_OK | MB_ICONINFORMATION );
		return;
	}
	if( m_bCsvCheck && m_strCsvDest.GetLength() == 0 )
	{
		AfxMessageBox( IDS_EXPORTDB_NOCSVDEST, MB_OK | MB_ICONINFORMATION );
		return;
	}
	if( m_bODBCCheck && m_strODBCDest.GetLength() == 0 )
	{
		AfxMessageBox( IDS_EXPORTDB_NOODBCDEST, MB_OK | MB_ICONINFORMATION );
		return;
	}

	if( m_bMdbCheck )
	{
		int nIndex = m_strMdbDest.Find( "." );
		if( -1 == nIndex )
			m_strMdbDest	+=	".mdb";
		else if( m_strMdbDest.GetLength()-1 == nIndex )
			m_strMdbDest	+=	"mdb";
	}
	if( m_bCsvCheck )
	{
		int nIndex = m_strCsvDest.Find( "." );
		if( -1 == nIndex )
			m_strCsvDest	+=	".csv";
		else if( m_strCsvDest.GetLength()-1 == nIndex )
			m_strCsvDest	+=	"csv";
	}
	UpdateData( FALSE );

	// Store profile
	StoreProfile( );

	// Export
	CSListView * pView = AfxGetSListView( );
	if( pView )
		pView->ExportList( this );
	else
		ExportFinish( );
}

