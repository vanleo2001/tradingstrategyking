// StkUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "StkUI.h"
#include <io.h>
#include <afxsock.h>

#include "MainFrm.h"
#include "ChildFrm.h"
#include "StaticDoc.h"
#include "View/WizardView.h"
#include "View/SimuView.h"
#include "View/InfoView.h"
#include "Dialog/SetPrpt.h"
#include "Dialog/SelectStk.h"
#include "Dialog/SetRate.h"
#include "Dialog/SetRule.h"
#include "Dialog/SimuReport.h"
#include "Dialog/SimuRealOp.h"
#include "Dialog/CloseStrategyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStkUIApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CStkUIApp

BEGIN_MESSAGE_MAP(CStkUIApp, CWinApp)
	//{{AFX_MSG_MAP(CStkUIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_STRATEGY_NEW, OnStrategyNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_STRATEGY_SAVE, OnStrategySave)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_SAVE, OnUpdateStrategySave)
	ON_COMMAND(ID_STRATEGY_SAVE_AS, OnStrategySaveAs)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_SAVE_AS, OnUpdateStrategySaveAs)
	ON_COMMAND(ID_STRATEGY_CLOSE, OnStrategyClose)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_CLOSE, OnUpdateStrategyClose)
	ON_COMMAND(ID_STRATEGY_SAVEALL, OnStrategySaveall)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_SAVEALL, OnUpdateStrategySaveall)
	//}}AFX_MSG_MAP
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStkUIApp construction

CStkUIApp::CStkUIApp()
{
	m_pMutex = NULL;
	m_pStaticDoc = NULL;
	m_bAutoUpdateViews = TRUE;
}

BOOL CALLBACK SplashProgressCallback(DWORD dwCode, DWORD dwProgress,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND	hMainWnd	=	(HWND)cookie;

	if( PROG_PROGRESS == dwCode && ::IsWindow(hMainWnd) )
	{
		::SendMessage( hMainWnd, WM_USER_SPLASH_PROGRESS, dwProgress, (LPARAM)lpszMsg );
	}

	MSG		msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		AfxGetApp()->PumpMessage();
	return TRUE;
}

BOOL CALLBACK StartupProgressCallback(DWORD dwCode, DWORD dwProgress,
							LPCTSTR lpszMsg, void *cookie)
{
	HWND	hMainWnd	=	(HWND)cookie;

	if( PROG_PROGRESS == dwCode && ::IsWindow(hMainWnd) )
	{
		::SendMessage( hMainWnd, WM_USER_STARTUP_PROGRESS, dwProgress, (LPARAM)lpszMsg );
	}

	MSG		msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		AfxGetApp()->PumpMessage();
	return TRUE;
}

UINT LoadKDataCacheMain(LPVOID pParam)
{
	AfxGetDB().LoadKDataCache( AfxGetStockContainer(), NULL, NULL, 0, STKLIB_MAX_PROGRESS );
	AfxGetStockContainer().OnDataChanged( );

	AfxGetStkReceiver().RefreshStockContainer( AfxGetStockContainer(), TRUE );

	AfxReloadStockMain( );

	::PostMessage( AfxGetMainWnd()->GetSafeHwnd(), WM_USER_UPDATESLISTVIEW,0, 0 );
	::PostMessage( AfxGetMainWnd()->GetSafeHwnd(), WM_USER_INITDATES, 0, 0 );

	AfxEndThread( 0, TRUE );
	return 0;
}

BOOL CALLBACK LoadProgram( HWND hWnd, int nMinProgress, int nMaxProgress )
{
	int nMin = nMinProgress;
	int nMax = nMaxProgress;
	int nRange = nMaxProgress-nMinProgress;
	CStkUIApp * pApp = AfxGetStkUIApp();
	if( NULL == pApp )
		return FALSE;

	CStaticDoc * pStaticDoc = pApp->GetStaticDoc();
	CMainFrame * pMainFrame = AfxGetMainFrame();

	HINSTANCE hInstanceOld = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetInstanceHandle() );

	SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin, 0 );

	AfxGetStockContainer().Load( &AfxGetDB(), NULL, NULL );
	AfxGetDomainContainer().Load( AfxGetProfile().GetDomainFile() );
	AfxGetGroupContainer().Load( AfxGetProfile().GetGroupFile() );
	SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*4/30, 0 );

	pMainFrame->m_SearchBox.InitStocks( TRUE, TRUE, TRUE );
	//pMainFrame->m_DateBox.InitDates( );
	SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*8/30, 0 );

	// Open Simulation Strategy
	if( NULL == pApp->GetFirstStrategyPosition() )
	{
		if( 0 == pApp->OpenLastOpenedStrategy( ) )
			pApp->OpenDefaultStrategy( );
	}
	SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*12/30, 0 );

	// Show Views
	//if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CWizardView) ) )
	//	pStaticDoc->ShowStaticView( RUNTIME_CLASS(CWizardView), TRUE );
	//SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*14/30, 0 );
	//if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CSimuView) ) )
	//	pStaticDoc->ShowStaticView( RUNTIME_CLASS(CSimuView), TRUE );
	//SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*16/30, 0 );
	if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CSListView) ) )
		pStaticDoc->ShowStaticView( RUNTIME_CLASS(CSListView), TRUE );
	SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*18/30, 0 );
	//if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CRealTimeView) ) )
	//	pStaticDoc->ShowStaticView( RUNTIME_CLASS(CRealTimeView), TRUE );
	//SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*20/30, 0 );
	//if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CMultiSortView) ) )
	//	pStaticDoc->ShowStaticView( RUNTIME_CLASS(CMultiSortView), TRUE );
	//SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*22/30, 0 );
	//if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CGraphView) ) )
	//	pStaticDoc->ShowStaticView( RUNTIME_CLASS(CGraphView), TRUE );
	//SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*24/30, 0 );
	//if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CBaseView) ) )
	//	pStaticDoc->ShowStaticView( RUNTIME_CLASS(CBaseView), TRUE );
	//SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*26/30, 0 );
	//if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CInfoView) ) )
	//	pStaticDoc->ShowStaticView( RUNTIME_CLASS(CInfoView), TRUE );
	//SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMin+nRange*28/30, 0 );
	//if( !pStaticDoc->GetViewIfExist( RUNTIME_CLASS(CSelectorView) ) )
	//	pStaticDoc->ShowStaticView( RUNTIME_CLASS(CSelectorView), TRUE );

	AfxSwitchToStaticView( RUNTIME_CLASS(CSListView) );

	AfxGetProfile().SetCurrentStock( STKLIB_CODE_SZZS, FALSE );
	pMainFrame->m_SearchBox.SetCurrentWindowText( );
	//pMainFrame->m_DateBox.SetWindowPos(NULL,0,0,135,200,SWP_NOMOVE|SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOREDRAW);

	AfxBeginThread( LoadKDataCacheMain, NULL, THREAD_PRIORITY_NORMAL);

	SendMessage( hWnd, WM_USER_STARTUP_PROGRESS, nMax, 0 );

	AfxSetResourceHandle( hInstanceOld );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CStkUIApp initialization

TCHAR szRegKeyCompany[] = _T("Tsking Stock Software");
TCHAR szRegKeyApp[] = _T("Tsking");

BOOL CStkUIApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	AfxSocketInit( );

	AfxOleInit();

	// 程序只能启动一个实例
	m_pMutex = CreateMutex(NULL, FALSE, _T("TsKing"));
	if (m_pMutex != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(m_pMutex);
			m_pMutex = NULL;

			return FALSE;
		}
	}

	GetVersionNumber();

	AfxGetProfile().LoadProfile( );
	::SetCurrentDirectory( AfxGetProfile().GetWorkDirectory() );
	AfxGetSView().Load( AfxGetProfile().GetSViewFile() );

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	// SetRegistryKey( szRegKeyCompany );
	AfxRegSetValue( szRegKeyCompany, szRegKeyApp, "Install", "Path", AfxGetProfile().GetWorkDirectory() );
	AfxRegSetValue( szRegKeyCompany, szRegKeyApp, "Install", "Version", AfxGetProfile().GetVersion() );

	// change profile ( INI file )
	BOOL bEnable = AfxEnableMemoryTracking(FALSE);
	if( m_pszProfileName )	free((void*)m_pszProfileName);
	m_pszProfileName = _tcsdup(AfxGetProfile().GetWorkDirectory()+m_pszExeName+".ini");
	AfxEnableMemoryTracking(bEnable);

	LoadStdProfileSettings(64);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	// Enable DDE Execute open
	EnableShellOpen();
	AfxUnregisterShellFileTypes();
	// AfxRegisterShellFileTypes(FALSE);

	// Empty Temp Directory
	{
		CNetDatabase netdb;
		if (netdb.SetRootPath(AfxGetProfile().GetSelfDBPath(), IStStore::dbtypeSelfDB))
			netdb.EmptyTempDirectory();
	}
	AfxSetDB(&AfxGetNetDB());
	//*/

	// Load Data
	char szErr[1024];
	if (!AfxInitializeDB(szErr, sizeof(szErr)))
	{
		AfxMessageBox(szErr, MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	// 初始化
	AfxGetStockContainer().Load(&AfxGetDB(), NULL, NULL);				// 读取证券信息，代码表、财务、除权等
	AfxGetDomainContainer().Load(AfxGetProfile().GetDomainFile());		// 读取板块
	AfxGetGroupContainer().Load(AfxGetProfile().GetGroupFile());		// 读取自选板块

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

#ifdef _TSK_MAINFRAME
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CStaticDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CSListView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
#else
	// Create MainFrame and Static Doc and child frames and static views
	CStaticDoc* pStaticDoc = CStaticDoc::OpenDocumentFile(NULL);
	if (pStaticDoc == NULL)
	{
		AfxMessageBox(IDS_STATICDOC_FAILED, MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	pStaticDoc->m_bAutoDelete = FALSE;
	CString	strAppTitle;
	strAppTitle.LoadString(AFX_IDS_APP_TITLE);
	pStaticDoc->SetTitle(strAppTitle);
	SetStaticDoc(pStaticDoc);

	// Create MainFrame
	CMainFrame* pMainFrame = CMainFrame::CreateNewFrame();
	if (pMainFrame == NULL)
	{
		AfxMessageBox(IDS_STATICDOC_FAILED, MB_OK | MB_ICONINFORMATION);
		delete pStaticDoc;
		SetStaticDoc(NULL);
		return FALSE;
	}
	SetMainWnd(pMainFrame);
#endif

	// 股票行情接收
	AfxGetStkReceiver().CreateReceiver(m_pMainWnd);
	AfxGetStkReceiver().NetEngineSetAutoReport(FALSE);

	/* Show Startup Window
	CStartupDlg startup;
	CBitmap	bmp;
	bmp.LoadBitmap(IDB_DLGLEFTLOGO);
	startup.SetBitmap((HBITMAP)bmp.GetSafeHandle());
	startup.SetLoadProgramFunc(LoadProgram);
	if (IDOK != startup.DoModal())
	{
		pMainFrame->SendMessage(WM_CLOSE);
		return FALSE;
	}
	//*/

	/* Show Splash Window
	CSplashDlg splash;
	CBitmap bmp;
	bmp.LoadBitmap(IDB_SPLASH);
	splash.SetBitmap(HBITMAP(bmp.GetSafeHandle()));
	splash.Create(NULL, 0, AfxGetSView().GetU(), AfxGetSView().GetS(), AfxGetVersionString(), FALSE, 0, STKLIB_MAX_PROGRESS);
	splash.ShowWindow(SW_SHOW);
	splash.UpdateWindow();
	LoadProgram(splash.GetSafeHwnd(), 0, STKLIB_MAX_PROGRESS);
	splash.DestroyWindow();
	//*/

	// //////////////////////////////////////////////////////////////

	// Open Simulation Strategy
	if (GetFirstStrategyPosition() == NULL)
	{
		if (OpenLastOpenedStrategy() == 0)
			OpenDefaultStrategy();
	}

	//if (!AfxGetStaticDoc()->GetViewIfExist(RUNTIME_CLASS(CSListView)))
	//	AfxGetStaticDoc()->ShowStaticView(RUNTIME_CLASS(CSListView), TRUE);

	//AfxSwitchToStaticView(RUNTIME_CLASS(CSListView));

	AfxGetProfile().SetCurrentStock(STKLIB_CODE_SZZS, FALSE);
	((CMainFrame*)m_pMainWnd)->m_SearchBox.InitStocks( TRUE, TRUE, TRUE );
	((CMainFrame*)m_pMainWnd)->m_SearchBox.SetCurrentWindowText();

	//*//////////////////////////////////////////////////////////////

	// Dispatch commands specified on the command line
	if( CCommandLineInfo::AppUnregister == cmdInfo.m_nShellCommand )
	{
		AfxUnregisterShellFileTypes();
		if (!cmdInfo.m_bRunEmbedded)
			AfxMessageBox(AFX_IDP_UNREG_DONE, MB_OK | MB_ICONINFORMATION);
		if (m_pCmdInfo == NULL)
		{
			m_pCmdInfo = new CCommandLineInfo;
			m_pCmdInfo->m_nShellCommand = CCommandLineInfo::AppUnregister;
		}
		return FALSE;
	}

	// Dispatch commands specified on the command line
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
	{
		OpenStrategyFile(cmdInfo.m_strFileName);
		AfxSwitchToStaticView(RUNTIME_CLASS(CSimuView));
	}

	// 启动通视接收系统，不显示选择接收系统对话框
	//AfxGetStkReceiver().EngineBeginWorking(FALSE);

	// The one and only window has been initialized, so show and update it.
	// Load Last WindowPlacement
	WINDOWPLACEMENT wp;
	if (AfxGetProfile().GetWindowPlacement(&wp))
		m_pMainWnd->SetWindowPlacement(&wp);
	else
		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);

	// Enable drag/drop open
	//m_pMainWnd->DragAcceptFiles();

	// Check New Version
	m_pMainWnd->SetTimer(TIMER_AUTOUPDATE, 5000, NULL);

	return TRUE;
}

BOOL CStkUIApp::OnDBChanged( BOOL bRecalculate )
{
	CWaitCursor	waitcursor;

	// Reload DB
	AfxGetStockContainer().Load( &AfxGetDB(), NULL, NULL );
	AfxGetDomainContainer().Load( AfxGetProfile().GetDomainFile() );
	AfxGetGroupContainer().Load( AfxGetProfile().GetGroupFile() );
	AfxGetDB().LoadKDataCache( AfxGetStockContainer(), NULL, NULL, 0, STKLIB_MAX_PROGRESS );
	AfxGetStockContainer().OnDataChanged( );

	AfxGetStkReceiver().RefreshStockContainer( AfxGetStockContainer(), TRUE );

	// Reload AfxGetStockMain()
	AfxReloadStockMain( );

	// Recalcuate Yield
	if( bRecalculate && IDYES == AfxMessageBox( IDS_INSTALLPAC_IFRECALCULATE, MB_YESNO|MB_ICONINFORMATION ) )
	{
		AfxRecalculateYield( AfxGetProfile().GetYieldAverageDays(), TRUE );
		AfxGetStockContainer().ReloadBase( &AfxGetDB() );
	}

	// Update SListBar
	CMainFrame	* pMainFrame	=	AfxGetMainFrame();
	if( pMainFrame )
	{
		pMainFrame->m_SearchBox.InitStocks( TRUE, TRUE, TRUE );
		pMainFrame->m_SearchBox.SetCurrentWindowText( );
		//pMainFrame->m_DateBox.InitDates( );
		//pMainFrame->m_DateBox.SetCurrentWindowText( );
	}

	// Update Views
	AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy(), TRUE );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_BASEVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SIMUVIEW_REREALRUN, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
	::PostMessage( AfxGetGroupView()->GetSafeHwnd(), WM_USER_UPDATEGROUPS, 0, 0 );

	return TRUE;
}

void CStkUIApp::SaveLastOpenedStrategy( )
{
	CSPStringArray astr;
	POSITION	pos	=	GetFirstStrategyPosition();
	while(pos != NULL)
	{
		CStrategy* pStrategy = GetNextStrategy(pos);
		if( pStrategy )
			astr.Add( pStrategy->GetPathName() );
	}

	AfxGetProfile().SetLastOpenedStrategy( astr );
}

int CStkUIApp::OpenLastOpenedStrategy( )
{
	CSPStringArray	&	astrStrategyToOpen	=	AfxGetProfile().GetLastOpenedStrategy( );
	CSPString	strLastActive			=	AfxGetProfile().GetLastActiveStrategy( );
	if( 0 != access(strLastActive,0) )
		strLastActive.Empty();
	int	nCount	=	0;
	for( int i=0; i<astrStrategyToOpen.GetSize(); i++ )
	{
		if( 0 != access(astrStrategyToOpen.ElementAt(i),0) )
			continue;
		if( NULL != OpenStrategyFile(astrStrategyToOpen.ElementAt(i)) )
		{
			nCount	++;
			if( strLastActive.IsEmpty() )
				strLastActive	=	astrStrategyToOpen.ElementAt(i);
		}
	}

	if( !strLastActive.IsEmpty() )
		OpenStrategyFile(strLastActive);

	return nCount;
}

int CStkUIApp::OpenDefaultStrategy( )
{
	CSPStringArray	astrStrategyToOpen;

	// Recent
	if( m_pRecentFileList )
	{
		for( int i=m_pRecentFileList->GetSize()-1; i>=0; i-- )
		{
			CString strPath = (*m_pRecentFileList)[i];
			if( !strPath.IsEmpty() )
				astrStrategyToOpen.Add( strPath );
		}
	}

	// projects
	CString strExt	=	AfxGetStrategyFileExt( );
	if( !strExt.IsEmpty())
	{
		CFileFind finder;
		BOOL	bWorking	=	finder.FindFile( AfxGetFilePath( (LPCTSTR)AfxGetProfile().GetProjectPath(), LPCTSTR("*" + strExt) ) );
		while( bWorking )
		{
			bWorking	=	finder.FindNextFile();
			CString	strPath	=	finder.GetFilePath();

			int i;
			for( i=0; i<astrStrategyToOpen.GetSize(); i++ )
			{
				if( 0 == strPath.Compare( astrStrategyToOpen.ElementAt(i) ) )
					break;
			}
			if( i == astrStrategyToOpen.GetSize() )
				astrStrategyToOpen.Add( strPath );
		}
		finder.Close();
	}

	AfxGetProfile().SetLastOpenedStrategy( astrStrategyToOpen );
	return OpenLastOpenedStrategy( );
}

CStrategy * CStkUIApp::GetActiveStrategy( )
{
	CStrategy	* pStrategy = NULL;
	
	CStrategyView * pStrategyView = AfxGetStrategyView();
	if( pStrategyView )
		pStrategy = pStrategyView->GetActiveStrategy();

	return pStrategy;
}

void CStkUIApp::AddStrategy(CStrategy* pStrategy)
{
	if( NULL == pStrategy )
		return;
	ASSERT(m_strategyList.Find(pStrategy, NULL) == NULL); // must not be in list
	m_strategyList.AddTail(pStrategy);

	if( m_bAutoUpdateViews )
	{
		::SendMessage( AfxGetStrategyView()->GetSafeHwnd(), WM_USER_ADDSTRATEGY, 0, 0 );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_WIZARDVIEW, NULL );
		// AfxGetMainFrame()->ShowWorkspBar( );
	}
}

void CStkUIApp::RemoveStrategy(CStrategy* pStrategy)
{
	POSITION pos = m_strategyList.Find(pStrategy);
	ASSERT( NULL != pos );
	if( pos )
		m_strategyList.RemoveAt( pos );

	if( m_bAutoUpdateViews )
	{
		::SendMessage( AfxGetStrategyView()->GetSafeHwnd(), WM_USER_REMOVESTRATEGY, 0, 0 );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_WIZARDVIEW, NULL );
	}
}

POSITION CStkUIApp::GetFirstStrategyPosition() const
{
	return m_strategyList.GetHeadPosition();
}

CStrategy* CStkUIApp::GetNextStrategy(POSITION& rPos) const
{
	return (CStrategy*)m_strategyList.GetNext(rPos);
}

BOOL CStkUIApp::ExistStrategy( CStrategy * pStrategy )
{
	POSITION pos = GetFirstStrategyPosition( );
	while( NULL != pos )
	{
		CStrategy * p = GetNextStrategy( pos );
		if( p == pStrategy )
			return TRUE;
	}
	return FALSE;
}

BOOL CStkUIApp::IsFileOpened( LPCTSTR lpszFileName )
{
	POSITION pos = GetFirstStrategyPosition();
	while (pos != NULL)
	{
		CStrategy* pStrategy = GetNextStrategy(pos);
		if( 0 == pStrategy->GetPathName().CompareNoCase( lpszFileName ) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

CString CStkUIApp::GetLastStrategyDirectory( )
{
	CString	strLastStrategyDirectory;
	if( m_pRecentFileList && m_pRecentFileList->GetSize() > 0 )
	{
		strLastStrategyDirectory = (*m_pRecentFileList)[0];
		strLastStrategyDirectory	=	AfxGetFileDirectoryExist( strLastStrategyDirectory, "" );
	}

	if( strLastStrategyDirectory.IsEmpty() || 0 != access( strLastStrategyDirectory,0) )
	{
		strLastStrategyDirectory	=	AfxGetProfile().GetProjectPath();
	}
	return strLastStrategyDirectory;
}

CString	CStkUIApp::GetNextNewStrategyTitle( CString & strExtBuffer, CString strPath )
{
	CString	string;
	VERIFY( string.LoadString( IDS_STRATEGY_NONAME ) );

	CString strExt	=	AfxGetStrategyFileExt( );
	if( !strExt.IsEmpty())
		strExtBuffer	=	strExt;

	CStringArray	astrExistTitle;
	POSITION pos = GetFirstStrategyPosition();
	while( NULL != pos )
	{
		CStrategy * pStrategy = GetNextStrategy( pos );
		astrExistTitle.Add( AfxGetFileTitleNoExt(pStrategy->GetPathName()) );
	}

	CFileFind finder;
	BOOL	bWorking	=	finder.FindFile( AfxGetFilePath( (LPCTSTR)strPath, LPCTSTR("*" + strExt) ) );
	while( bWorking )
	{
		bWorking	=	finder.FindNextFile();
		astrExistTitle.Add( finder.GetFileTitle( ) );
	}
	finder.Close();

	for( int i=1; ; i++ )
	{
		CString	strTemp;
		strTemp.Format( "%s(%d)", string, i );
		
		BOOL	bHas	=	FALSE;
		for( int k=0; k<astrExistTitle.GetSize(); k++ )
		{
			if( 0 == strTemp.CompareNoCase( astrExistTitle.ElementAt(k) ) )
			{
				bHas	=	TRUE;
				break;
			}
		}
		
		if( !bHas )
		{
			string	=	strTemp;
			break;
		}
	}
	return string;
}

CStrategy* CStkUIApp::OpenStrategyFile( LPCTSTR lpszPathName )
{
	// Resolve File Name
	TCHAR szPath[_MAX_PATH];
	ASSERT(lstrlen(lpszPathName) < sizeof(szPath));
	if( NULL != lpszPathName )
	{
		TCHAR szTemp[_MAX_PATH];
		if (lpszPathName[0] == '\"')
			++lpszPathName;
		lstrcpyn(szTemp, lpszPathName, _MAX_PATH);
		LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
		if (lpszLast != NULL)
			*lpszLast = 0;
		AfxFullPath(szPath, szTemp);
		TCHAR szLinkName[_MAX_PATH];
		if (AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
			lstrcpy(szPath, szLinkName);
	}

	if( NULL != lpszPathName )
	{
		POSITION pos = GetFirstStrategyPosition();
		while (pos != NULL)
		{
			CStrategy* pStrategy = GetNextStrategy(pos);
			if( 0 == pStrategy->GetPathName().CompareNoCase( szPath ) )
			{
				if( m_bAutoUpdateViews )
				{
					::SendMessage( AfxGetStrategyView()->GetSafeHwnd(), WM_USER_ACTIVATESTRATEGY, DWORD(pStrategy), 0 );
					AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_WIZARDVIEW, NULL );
					// AfxGetMainFrame()->ShowWorkspBar( );
				}
				return pStrategy;
			}
		}
	}

	CStrategy   * pStrategy = new CStrategy();
	if( NULL == pStrategy )
		return NULL;
	char	szErr[256];
	if( !pStrategy->OpenStrategyFile( szPath, szErr, sizeof(szErr)-1 ) )
	{
		AfxMessageBox( szErr, MB_OK|MB_ICONINFORMATION );
		delete	pStrategy;
		return NULL;
	}
	
	AddStrategy( pStrategy );
	return pStrategy;
}

void CStkUIApp::CloseAllStrategy( BOOL bEndSession )
{
	POSITION pos = GetFirstStrategyPosition();
	while (pos != NULL)
	{
		CStrategy* pStrategy = GetNextStrategy(pos);
		pStrategy->OnClose();
		delete	pStrategy;
	}
	m_strategyList.RemoveAll();
	
	if( ! bEndSession && m_bAutoUpdateViews )
	{
		::SendMessage( AfxGetStrategyView()->GetSafeHwnd(), WM_USER_REMOVESTRATEGY, 0, 0 );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_WIZARDVIEW, NULL );
	}
}

CRecentFileList * CStkUIApp::GetRecentFileList( )
{
	return m_pRecentFileList;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CXFDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };

	CButton	m_btnOK;
	CStatic	m_staticVersion;
	CStatic	m_staticSoftNO;
	CStatic	m_staticRegCode;
	CStatic	m_staticUser;
	CHyperLink	m_linkMailMe;
	CHyperLink	m_linkProductURL;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CXFDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_VERSION, m_staticVersion);
	DDX_Control(pDX, IDC_SOFTNO, m_staticSoftNO);
	DDX_Control(pDX, IDC_REGCODE, m_staticRegCode);
	DDX_Control(pDX, IDC_USER, m_staticUser);
	DDX_Control(pDX, IDC_URL_MAILME, m_linkMailMe);
	DDX_Control(pDX, IDC_URL_ADDRESS, m_linkProductURL);
	//}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	CString	strMailMe	=	AfxGetProfile().GetSupportMailto();
	m_linkMailMe.SetURL( strMailMe );

	CString	strURL	=	AfxGetProfile().GetProductURL();
	m_linkProductURL.SetWindowText( strURL );
	m_linkProductURL.SetURL( strURL );

	CString	strVersion	=	AfxGetVersionString( );
	if( strVersion.GetLength() > 0 )
		m_staticVersion.SetWindowText( strVersion );

	CString	strUser	=	AfxGetSView().GetU();
	if( strUser.IsEmpty() )
		strUser.LoadString( IDS_AUTHORIZEDUSER_UNKNOWN );
	m_staticUser.SetWindowText( strUser );
	
	CString strHD	=	AfxGetSView().GetS();
	if( !strHD.IsEmpty() )
		m_staticSoftNO.SetWindowText( strHD );

	CString	strRegCode	=	AfxGetSView().GetR();
	if( strRegCode.IsEmpty() )
		strRegCode.LoadString( IDS_REGCODE_UNPURCHASED );
	m_staticRegCode.SetWindowText( strRegCode );

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CXFDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CStkUIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CStkUIApp message handlers


int CStkUIApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	SaveLastOpenedStrategy( );
	CloseAllStrategy( TRUE );

	AfxGetDB().StoreCodetable( AfxGetStockContainer() );
	AfxGetProfile().StoreProfile( );
	AfxGetSView().Store( AfxGetProfile().GetSViewFile() );
	AfxReleaseDB( );

	if( m_pStaticDoc )
	{
		//m_pStaticDoc->OnCloseDocument();
		//delete m_pStaticDoc;
		m_pStaticDoc	=	NULL;
	}
	return CWinApp::ExitInstance();
}

void CStkUIApp::OnFileNew() 
{
	// TODO: Add your command handler code here
//	OpenStrategyFile( NULL );
}

void CStkUIApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString newName;
	if( m_pRecentFileList && m_pRecentFileList->GetSize() > 0 )
		newName = (*m_pRecentFileList)[0];

	if (!AfxDoPromptFileName(newName, IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING, TRUE))
		return; // open cancelled

	if( NULL != OpenStrategyFile(newName) )
		AfxSwitchToStaticView( RUNTIME_CLASS(CSimuView) );
		// if returns NULL, the user has already been alerted
}

BOOL CStkUIApp::SaveAllModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	CStrategy * pActiveStrategy = AfxGetActiveStrategy();
	if( pActiveStrategy )
		pActiveStrategy->DoFileSave();
/*
	POSITION pos = GetFirstStrategyPosition();
	while (pos != NULL)
	{
		CStrategy* pStrategy = GetNextStrategy(pos);
		if (!pStrategy->DoFileSave() ) // >SaveModified())
			return FALSE;
	}
*/
	return TRUE;
}

void CStkUIApp::OnStrategyNew() 
{
	// TODO: Add your command handler code here
	CStrategy * pStrategy = new CStrategy();
	if( NULL == pStrategy )
	{
		AfxMessageBox( IDS_STRATEGY_FAILED, MB_OK | MB_ICONINFORMATION );
		return;
	}
	
	CSetPrpt		firstPage( NULL, IDS_STRATEGY_SETPRPT, NULL );
	firstPage.m_dwButtonFlags = PSWIZB_NEXT | PSWIZB_DISABLEDFINISH;
	firstPage.SetStrategy( pStrategy );

	CSelectStk		selectStk(NULL, IDS_STRATEGY_SELECTSTK, NULL);
	selectStk.m_dwButtonFlags = PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_DISABLEDFINISH;

	CSetRule		setrule(NULL, IDS_STRATEGY_SETRULE, NULL);
	setrule.m_dwButtonFlags = PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_DISABLEDFINISH;
	setrule.SetTechParameters( &(pStrategy->GetTechParametersBuy()), &(pStrategy->GetSelectedTechsBuy()) );

	CSetRate		setrate(NULL, IDS_STRATEGY_SETRATE, NULL);
	setrate.m_dwButtonFlags = PSWIZB_BACK | PSWIZB_FINISH;
	setrate.SetStrategy( pStrategy );

	CPropertySheetEx	sht;
	sht.AddPage( &firstPage );
	sht.AddPage( &selectStk );
	sht.AddPage( &setrule );
	sht.AddPage( &setrate );
	sht.SetWizardMode( );
	
	sht.m_psh.dwFlags |= ( PSH_WIZARD97 | PSH_WIZARDHASFINISH /* | PSH_HEADER /*Header Bitmap*/ );
	//CBitmap	bmp;
	//bmp.LoadBitmap( IDB_BALANG );
	//sht.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );

	if( ID_WIZFINISH == sht.DoModal() )
	{
		pStrategy->SetStocks( selectStk.m_domainTemp );
		pStrategy->SaveStrategyFile( firstPage.GetPathName() );
		AddStrategy( pStrategy );
		AfxSwitchToStaticView( RUNTIME_CLASS(CSimuView) );
	}
	else
	{
		delete pStrategy;
	}
}

void CStkUIApp::OnStrategySave() 
{
	// TODO: Add your command handler code here
//	CStrategy * pStrategy = GetActiveStrategy();
//	if( pStrategy )
//		pStrategy->DoFileSave( );
}

void CStkUIApp::OnUpdateStrategySave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CStrategy * pActive = GetActiveStrategy( );
	pCmdUI->Enable( FALSE );	//	NULL != pActiveStrategy );
}

void CStkUIApp::OnStrategySaveAs() 
{
	// TODO: Add your command handler code here
	CStrategy * pStrategy = GetActiveStrategy();
	if( pStrategy )
	{
		CString	newName = AfxGetFileTitleNoExt(pStrategy->GetPathName());
		// check for dubious filename
		int iBad = newName.FindOneOf(_T(" #%;/\\"));
		if (iBad != -1)
			newName.ReleaseBuffer(iBad);

		// append the default suffix if there is one
		newName += AfxGetStrategyFileExt( );

		if( !AfxDoPromptFileName(newName, IDS_SAVEFILE,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING, FALSE ) )
			return;

		pStrategy->SetPathName(newName);
		pStrategy->SetName( AfxGetFileTitleNoExt(newName) );
		pStrategy->DoFileSave( );
		AfxOnStrategyUpdated( pStrategy, FALSE );
	}
}

void CStkUIApp::OnUpdateStrategySaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable( NULL != GetActiveStrategy( ) );
}

void CStkUIApp::OnStrategySaveall() 
{
	// TODO: Add your command handler code here
	SaveAllModified();
}

void CStkUIApp::OnUpdateStrategySaveall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( NULL != GetFirstStrategyPosition() );
}

void CStkUIApp::OnStrategyClose() 
{
	// TODO: Add your command handler code here
	CSimuView	*	pSimuView	=	AfxGetSimuView();
	CStrategy	* pActive		=	AfxGetActiveStrategy();
	CStrategyView * pStrategyView			=	AfxGetStrategyView();

	CCloseStrategyDlg	dlg;
	if( IDOK == dlg.DoModal() )
	{
		BOOL	bOldValue	=	m_bAutoUpdateViews;
		m_bAutoUpdateViews	=	FALSE;
		
		POSITION pos = dlg.GetFirstCloseStrategyPosition( );
		while( NULL != pos )
		{
			CStrategy * pStrategy = dlg.GetNextCloseStrategy( pos );
			if( pSimuView && pStrategy == pActive && !pSimuView->CanChangeActiveStrategy() )
				continue;
			if( pStrategy == pActive && pStrategyView )
			{
				pStrategyView->SetActiveStrategy( NULL );
				if( pSimuView )
					pSimuView->OnActiveStrategyChanged( );
			}
			RemoveStrategy( pStrategy );
			pStrategy->OnClose( );
			delete	pStrategy;
		}

		m_bAutoUpdateViews	=	bOldValue;
		if( m_bAutoUpdateViews )
		{
			::SendMessage( AfxGetStrategyView()->GetSafeHwnd(), WM_USER_REMOVESTRATEGY, 0, 0 );
			AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_WIZARDVIEW, NULL );
		}
	}
}

void CStkUIApp::OnUpdateStrategyClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( NULL != GetFirstStrategyPosition() );
}

void CStkUIApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	// CWinApp::WinHelp(dwData, nCmd);
}

void CStkUIApp::GetVersionNumber()
{
	TCHAR szPath[MAX_PATH];
	DWORD dwSize;

	m_nVersion[0] = m_nVersion[1] = m_nVersion[2] = m_nVersion[3] = 0;

	GetModuleFileName( NULL, szPath, MAX_PATH );
	dwSize = GetFileVersionInfoSize( szPath, &dwSize );

	if ( dwSize )
	{
		BYTE* pBuffer = new BYTE[ dwSize ];

		if ( GetFileVersionInfo( szPath, NULL, dwSize, pBuffer ) )
		{
			VS_FIXEDFILEINFO* pTable;

			if ( VerQueryValue( pBuffer, _T("\\"), (VOID**)&pTable, (UINT*)&dwSize ) )
			{
				m_nVersion[0] = (WORD)( pTable->dwFileVersionMS >> 16 );
				m_nVersion[1] = (WORD)( pTable->dwFileVersionMS & 0xFFFF );
				m_nVersion[2] = (WORD)( pTable->dwFileVersionLS >> 16 );
				m_nVersion[3] = (WORD)( pTable->dwFileVersionLS & 0xFFFF );
			}
		}

		delete [] pBuffer;
	}

	m_sVersion.Format( _T("%i.%i.%i.%i"),
		m_nVersion[0], m_nVersion[1], m_nVersion[2], m_nVersion[3] );
}
