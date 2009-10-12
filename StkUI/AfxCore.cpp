// AfxCore.cpp : implementation of the AfxCore Functions
//

#include	"stdafx.h"
#include	"AfxCore.h"

#include <math.h>

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CSPTime& value)
{
	CTime	tm	=	value.GetTime();
	DDX_DateTimeCtrl(pDX, nIDC,tm);
	value	=	tm.GetTime();
}

CFont * AfxSelectDCFont( CDC * pDC, int nHeight )
{
	static	CFont	font;
	font.DeleteObject();

#ifdef CLKLAN_ENGLISH_US
	if( nHeight <= 15 )
	{
		CFont *pFont = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
		return	pDC->SelectObject( pFont );
	}
	else
	{
		return NULL;
	}
#else
	font.CreateFont(nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH,
					"楷体" );
	return	pDC->SelectObject( &font );
#endif
}

UINT AfxRecalculateYield( int nDays, BOOL bReport )
{
	// 计算市场年平均收益率
	double	market_yield_average = 0.0, market_yield_d = 0.0;
	double	market_dayyield_average = 0.0, market_dayyield_d = 0.0;
	CKData	& kdayMain	=	AfxGetStockMain().GetKDataDay();

	if( kdayMain.GetSize() <= nDays )
	{
		if( bReport )
			AfxMessageBox( IDS_RECALYIELD_NOSZZS, MB_OK | MB_ICONINFORMATION );
		return 0;
	}
	BOOL bmarket_yield_ok = CalculateYieldYear( kdayMain, &market_yield_average, &market_yield_d, nDays );
	BOOL bmarket_dayyield_ok = CalculateYieldDay( kdayMain, &market_dayyield_average, &market_dayyield_d, nDays );

	if( !bmarket_yield_ok && !bmarket_dayyield_ok )
	{
		if( bReport )
			AfxMessageBox( IDS_RECALYIELD_NOSZZS, MB_OK | MB_ICONINFORMATION );
	}
	
	// 开始生成结果数据
	CStockContainer & container = AfxGetStockContainer();
	int	nCount	=	0;
	// 等待对话框
	CWaitDlg	*	pWait	=	NULL;
	if( bReport )
	{
		pWait	=	new CWaitDlg( AfxGetMainWnd() );
		pWait->SetProgressRange( 0, container.GetSize()-1 );
	}
	for( int i=0; i<container.GetSize(); i++ )
	{
		if( pWait )
		{
			pWait->SetProgress( i );
			if( pWait->WaitForCancel( ) )
				break;
		}

		CStockInfo	& info	=	container.ElementAt(i);

		LONG	stocktype	=	info.GetType();
		if( CStock::typeshIndex != stocktype && CStock::typeshA != stocktype && CStock::typeshB != stocktype
			&& CStock::typeszIndex != stocktype && CStock::typeszA != stocktype && CStock::typeszB != stocktype )
			continue;

		CStock	stock;
		stock.SetStockInfo( &info );
		AfxPrepareStockData(&AfxGetDB(),stock,CKData::ktypeDay,CKData::formatXDRup,CKData::mdtypeClose,TRUE,TRUE);
		CKData	& kday	=	stock.GetKDataDay();

		info.m_fYield_average		=	(float)STKLIB_DATA_INVALID;
		info.m_fYield_stddev		=	(float)STKLIB_DATA_INVALID;
		info.m_fBeite				=	(float)STKLIB_DATA_INVALID;

		double	yield_average = 0., yield_d = 0., beite = 0. ;
		if( CalculateYieldYear( kday, &yield_average, &yield_d, nDays ) )
		{
			info.m_fYield_average	=	(float)( 100. * yield_average );
			info.m_fYield_stddev	=	(float)( 100. * yield_d );
			if( bmarket_yield_ok && CalculateBeiteYear( kday, kdayMain, market_yield_average, market_yield_d, &beite, nDays ) )
				info.m_fBeite		=	(float)( beite );
			else if( bmarket_dayyield_ok && CalculateBeiteDay( kday, kdayMain, market_dayyield_average, market_dayyield_d, &beite, nDays ) )
				info.m_fBeite		=	(float)( beite );
		}
		else if( CalculateYieldDay( kday, &yield_average, &yield_d, nDays ) )
		{
			info.m_fYield_average	=	(float)( 100. * (pow(1+yield_average,STKLIB_DAYS_INONEYEAR)-1) );
			info.m_fYield_stddev	=	(float)( 100. * sqrt((double)STKLIB_DAYS_INONEYEAR) * yield_d );
			if( bmarket_dayyield_ok && CalculateBeiteDay( kday, kdayMain, market_dayyield_average, market_dayyield_d, &beite, nDays ) )
				info.m_fBeite		=	(float)( beite );
		}

		nCount	++;
	}

	// 保存到硬盘文件
	AfxGetDB().StoreBasetable( container );

	// 关闭等待对话框和目标文件
	if( pWait )
	{
		pWait->DestroyWindow();
		delete	pWait;
	}

	return nCount;
}

// 快捷键
void AfxExecuteAccelerator( UINT nAcce )
{
	HWND	hWnd	=	AfxGetMainFrame()->GetSafeHwnd();
	switch( nAcce )
	{
	case ACCE_01:		// 热键F1
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_REPORTDETAIL, 0 );
		break;
	case ACCE_02:		// 热键F2
		::PostMessage( hWnd, WM_COMMAND, ID_VIEW_SLIST, 0 );
		break;
	case ACCE_03:		// 热键F3
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_VIEWSZZS, 0 );
		break;
	case ACCE_04:		// 热键F4
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_VIEWSZNCZ, 0 );
		break;
	case ACCE_05:		// 热键F5
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			if( pView && pView->IsKindOf( RUNTIME_CLASS(CGraphView) ) )
				::PostMessage( hWnd, WM_COMMAND, ID_VIEW_REALTIME, 0 );
			else
				::PostMessage( hWnd, WM_COMMAND, ID_VIEW_GRAPH, 0 );
		}
		break;
	case ACCE_06:		// 热键F6
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_GROUPSELF, 0 );
		break;
	case ACCE_07:		// 热键F7
		::PostMessage( hWnd, WM_COMMAND, ID_VIEW_MULTISORT, 0 );
		break;
	case ACCE_08:		// 热键F8
		break;
	case ACCE_09:		// 热键F9
		break;
	case ACCE_10:		// 热键F10
		::PostMessage( hWnd, WM_COMMAND, ID_VIEW_F10, 0 );
		break;
	case ACCE_1:		// 上A报价
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SHAA, 0 );
		break;
	case ACCE_2:		// 上B报价
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SHAB, 0 );
		break;
	case ACCE_3:		// 深A报价
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SZNA, 0 );
		break;
	case ACCE_4:		// 深B报价
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SZNB, 0 );
		break;
	case ACCE_5:		// 上债报价
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SHABOND, 0 );
		break;
	case ACCE_6:		// 深债报价
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SZNBOND, 0 );
		break;
	case ACCE_61:		// 上A涨跌
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SHAASORTDP, 0 );
		break;
	case ACCE_62:		// 上B涨跌
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SHABSORTDP, 0 );
		break;
	case ACCE_63:		// 深A涨跌
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SZNASORTDP, 0 );
		break;
	case ACCE_64:		// 深B涨跌
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SZNBSORTDP, 0 );
		break;
	case ACCE_65:		// 上债涨跌
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SHABONDSORTDP, 0 );
		break;
	case ACCE_66:		// 深债涨跌
		::PostMessage( hWnd, WM_COMMAND, ID_SLIST_SZNBONDSORTDP, 0 );
		break;
	case ACCE_71:		// 上证信息
		break;
	case ACCE_72:		// 深证信息
		break;
	case ACCE_73:		// 券商信息
		break;
	case ACCE_81:		// 上A综合
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_MSSZAG, 0 );
		break;
	case ACCE_82:		// 上B综合
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_MSSZBG, 0 );
		break;
	case ACCE_83:		// 深A综合
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_MSSZNAG, 0 );
		break;
	case ACCE_84:		// 深B综合
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_MSSZNBG, 0 );
		break;
	case ACCE_85:		// 上债综合
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_MSSZZQ, 0 );
		break;
	case ACCE_86:		// 深债综合
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_MSSZNZQ, 0 );
		break;
	case ACCE_LBDK:		// 量比多空
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_LBDK, 0 );
		break;
	case ACCE_MMLD:		// 买卖力道
		::PostMessage( hWnd, WM_COMMAND, ID_REALTIME_MMLD, 0 );
		break;
	default:
		ASSERT( FALSE );
	}
}



AFX_STATIC_DATA const TCHAR _afxShellOpenFmt[] = _T("%s\\shell\\open\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintFmt[] = _T("%s\\shell\\print\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
AFX_STATIC_DATA const TCHAR _afxDefaultIconFmt[] = _T("%s\\DefaultIcon");
AFX_STATIC_DATA const TCHAR _afxShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

AFX_STATIC_DATA const TCHAR _afxIconIndexFmt[] = _T(",%d");
AFX_STATIC_DATA const TCHAR _afxCommand[] = _T("command");
AFX_STATIC_DATA const TCHAR _afxOpenArg[] = _T(" \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintArg[] = _T(" /p \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
AFX_STATIC_DATA const TCHAR _afxDDEArg[] = _T(" /dde");

AFX_STATIC_DATA const TCHAR _afxDDEExec[] = _T("ddeexec");
AFX_STATIC_DATA const TCHAR _afxDDEOpen[] = _T("[open(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrint[] = _T("[print(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

AFX_STATIC_DATA const TCHAR _afxShellNewValueName[] = _T("NullFile");
AFX_STATIC_DATA const TCHAR _afxShellNewValue[] = _T("");

void AfxUnregisterShellFileTypes()
{
	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	CString	strMainFrame;
	VERIFY(strMainFrame.LoadString( IDR_MAINFRAME ));

	CString strFilterExt, strFileTypeId, strFileTypeName;
	if (AfxExtractSubString( strFileTypeId,strMainFrame,
	   CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
	{
		// enough info to register it
		if (!AfxExtractSubString( strFileTypeName, strMainFrame,
		   CDocTemplate::regFileTypeName))
			strFileTypeName = strFileTypeId;    // use id name

		ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

		strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
		AfxDeleteRegKey(strTemp);

		// path\shell\open\command = path filename
		strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		AfxDeleteRegKey(strTemp);

		// path\shell\print\command = path /p filename
		strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		AfxDeleteRegKey(strTemp);

		// path\shell\printto\command = path /pt filename printer driver port
		strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		AfxDeleteRegKey(strTemp);

		AfxExtractSubString( strFilterExt, strMainFrame, CDocTemplate::filterExt);
		if (!strFilterExt.IsEmpty())
		{
			ASSERT(strFilterExt[0] == '.');

			LONG lSize = _MAX_PATH * 2;
			LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
				strTemp.GetBuffer(lSize), &lSize);
			strTemp.ReleaseBuffer();

			if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
				strTemp == strFileTypeId)
			{
				strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
				AfxDeleteRegKey(strTemp);

				// no association for that suffix
				AfxDeleteRegKey(strFilterExt);
			}
		}
	}
}

void AfxRegisterShellFileTypes(BOOL bCompat)
{
	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	CString strOpenCommandLine = strPathName;
	CString strPrintCommandLine = strPathName;
	CString strPrintToCommandLine = strPathName;
	CString strDefaultIconCommandLine = strPathName;

	CString	strMainFrame;
	VERIFY(strMainFrame.LoadString( IDR_MAINFRAME ));

	if (bCompat)
	{
		CString strIconIndex;
		HICON hIcon = ::ExtractIcon(AfxGetInstanceHandle(), strPathName, 1);
		if (hIcon != NULL)
		{
			strIconIndex.Format(_afxIconIndexFmt, 1);
			DestroyIcon(hIcon);
		}
		else
		{
			strIconIndex.Format(_afxIconIndexFmt, DEFAULT_ICON_INDEX);
		}
		strDefaultIconCommandLine += strIconIndex;
	}

	CString strFilterExt, strFileTypeId, strFileTypeName;

	if (AfxExtractSubString( strFileTypeId, strMainFrame,
		CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
	{
		// enough info to register it
		if (!AfxExtractSubString( strFileTypeName, strMainFrame, 
		   CDocTemplate::regFileTypeName))
			strFileTypeName = strFileTypeId;    // use id name

		ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

		// first register the type ID of our server
		if (!AfxSetRegKey(strFileTypeId, strFileTypeName))
			return;       // just skip it

		if (bCompat)
		{
			// path\DefaultIcon = path,1
			strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
			if (!AfxSetRegKey(strTemp, strDefaultIconCommandLine))
				return;       // just skip it
		}

		// path\shell\open\command = path filename
		// path\shell\print\command = path /p filename
		// path\shell\printto\command = path /pt filename printer driver port
		strOpenCommandLine += _afxOpenArg;
		if (bCompat)
		{
			strPrintCommandLine += _afxPrintArg;
			strPrintToCommandLine += _afxPrintToArg;
		}

		// path\shell\open\command = path filename
		strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		if (!AfxSetRegKey(strTemp, strOpenCommandLine))
			return;       // just skip it

		if (bCompat)
		{
			// path\shell\print\command = path /p filename
			strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!AfxSetRegKey(strTemp, strPrintCommandLine))
				return;       // just skip it

			// path\shell\printto\command = path /pt filename printer driver port
			strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!AfxSetRegKey(strTemp, strPrintToCommandLine))
				return;       // just skip it
		}

		AfxExtractSubString( strFilterExt, strMainFrame, CDocTemplate::filterExt);
		if (!strFilterExt.IsEmpty())
		{
			ASSERT(strFilterExt[0] == '.');

			LONG lSize = _MAX_PATH * 2;
			LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
				strTemp.GetBuffer(lSize), &lSize);
			strTemp.ReleaseBuffer();

			if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
				strTemp == strFileTypeId)
			{
				// no association for that suffix
				if (!AfxSetRegKey(strFilterExt, strFileTypeId))
					return;

				if (bCompat)
				{
					strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
					(void)AfxSetRegKey(strTemp, _afxShellNewValue, _afxShellNewValueName);
				}
			}
		}
	}
}

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

AFX_STATIC void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
											  CString *pstrDefaultExt)
{
	CString	strMainFrame;
	VERIFY(strMainFrame.LoadString( IDR_MAINFRAME ));

	CString strFilterExt, strFilterName;
	if (AfxExtractSubString(strFilterExt, strMainFrame, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 AfxExtractSubString(strFilterName, strMainFrame, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

BOOL AfxDoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog )
{
	CKSFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;
	
	CString strFilter;
	CString strDefault;
	
	_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn,&strDefault);

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	CString	strPath = dlgFile.GetPathName( );
	return nResult == IDOK;
}

BOOL AfxDoSelectDirectory( CString & strSrc, HWND hWnd )
{
/*	The Old Version */
/*
	BROWSEINFO bi;
	char szWorkDir[MAX_PATH+1]	=	"";
	LPITEMIDLIST piidlRoot;
	SHGetSpecialFolderLocation(hWnd, CSIDL_DRIVES, &piidlRoot);
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = hWnd;
	bi.pidlRoot = piidlRoot;
	bi.pszDisplayName = szWorkDir;
	CString	strTitle;
	strTitle.LoadString( IDS_SELECT_DIRECTORY );
	if( GetSystemMetrics(SM_DBCSENABLED) )
	{
		bi.lpszTitle = strTitle.GetBuffer(strTitle.GetLength()+1);
	}
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	LPITEMIDLIST	piidlTarget = SHBrowseForFolder(&bi);
	if( piidlTarget )
	{
		::SHGetPathFromIDList( piidlTarget, szWorkDir ) ;
		strSrc		=	szWorkDir;
		::CoTaskMemFree( piidlTarget );
		return TRUE;
	}
	return FALSE;
*/
	/* The New Version Using CPathDialog */
	CString strCaption;
	CString strTitle;
	strCaption.LoadString( IDS_SELECT_DIRECTORY );
	strTitle.LoadString( IDS_SELECT_DIRECTORY );

	CPathDialog dlg(strCaption, strTitle, strSrc);

	if(dlg.DoModal()==IDOK
		&& CPathDialog::MakeSurePathExists(dlg.GetPathName())==0 )
	{
		strSrc	=	dlg.GetPathName();
		return TRUE;
	}
	return FALSE;
}

CString	AfxGetStrategyFileExt( )
{
	CString	strMainFrame;
	strMainFrame.LoadString( IDR_MAINFRAME );
	CString strExt;
	VERIFY( AfxExtractSubString( strExt, strMainFrame,CDocTemplate::filterExt) );
	ASSERT( !strExt.IsEmpty() && strExt[0] == '.');
	return strExt;
}

CString	AfxGetApplicationName( )
{
	CString	strMainFrame;
	strMainFrame.LoadString( IDR_MAINFRAME );
	CString strName;
	VERIFY( AfxExtractSubString( strName, strMainFrame,CDocTemplate::windowTitle) );
	return strName;
}

CString AfxGetStockIndexReportTip( )
{
	CString strResult;

	CString strSHTipFmt, strSZTipFmt;
	strSHTipFmt.LoadString( IDS_MAINFRAME_SHINDEXTIP );
	strSZTipFmt.LoadString( IDS_MAINFRAME_SZINDEXTIP );

	double dDevided = 100000000;
#ifdef CLKLAN_ENGLISH_US
	dDevided = 1000000000;
#endif

	CStockInfo infoMain, infoMainSZN;
	double dDiff;
	CString strSH, strSZ;
	if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAIN, &infoMain )
		&& infoMain.GetDiff(&dDiff, infoMain.m_datetech ,1) )
		strSH.Format( strSHTipFmt, infoMain.m_fClose, dDiff, infoMain.m_fAmount/dDevided );
	if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAINSZN, &infoMainSZN )
		&& infoMainSZN.GetDiff(&dDiff, infoMainSZN.m_datetech,1) )
		strSZ.Format( strSZTipFmt, infoMainSZN.m_fClose, dDiff, infoMainSZN.m_fAmount/dDevided );

	if( strSH.IsEmpty() && strSZ.IsEmpty() )
		return AfxGetApplicationName();
	
	strResult = strSH + "  " + strSZ;
	return strResult;
}

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef AFX_TERM_SEG
#pragma code_seg(AFX_TERM_SEG)
#endif

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
