// AfxCore.inl : implementation file
//

/////////////////////////////////////////////////////////////////////////////
// AfxCore

// Assert Valid Functions
_AFXCORE_INLINE void AfxAssertNull( )
{
	CStockInfo	infoTemp, info;
	CMathParser	parser;
	parser.Parse( );
}

_AFXCORE_INLINE BOOL AfxAssertValid( )
{
	BOOL	b	=	AfxGetSView().AssertValid();
	AfxAssertNull();
	if( b )
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return b;
}

_AFXCORE_INLINE BOOL AfxAssertValidStd( )
{
	BOOL	b	=	AfxGetSView().IsStd();
	AfxAssertNull();
	if( b )
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return b;
}

_AFXCORE_INLINE BOOL AfxAssertValidProf( )
{
	BOOL	b	=	AfxGetSView().IsProf();
	AfxAssertNull();
	if( b )
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return b;
}

_AFXCORE_INLINE BOOL AfxAssertFunclimitStd( UINT nIDString )
{
	if( !AfxGetSView().IsStd() && !AfxGetSView().IsProf() )
	{
		AfxAssertNull();
		AfxMessageBox( nIDString, MB_OK | MB_ICONINFORMATION );
		AfxAssertNull();
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE BOOL AfxAssertFunclimitProf( UINT nIDString )
{
	if( !AfxGetSView().IsProf() )
	{
		AfxAssertNull();
		AfxMessageBox( nIDString, MB_OK | MB_ICONINFORMATION );
		AfxAssertNull();
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE BOOL AfxAssertRegisterQuit( )
{
	if( !AfxGetSView().AssertValid() )
	{
		AfxAssertNull();
		AfxAssertQuit();
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE BOOL AfxAssertMessageQuit( )
{
	if( !AfxGetSView().AssertValid() )
	{
		AfxAssertNull();
		AfxMessageBox( IDS_SECRET_ASSERTMESSAGE, MB_OK | MB_ICONINFORMATION );
		AfxGetStkUIApp()->SaveAllModified();
		AfxGetMainFrame()->PostMessage( WM_CLOSE );
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE BOOL AfxAssertQuit( )
{
	if( !AfxGetSView().AssertValid() )
	{
		AfxAssertNull();
		AfxGetStkUIApp()->SaveAllModified();
		AfxGetMainFrame()->PostMessage( WM_CLOSE );
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE	CStkUIApp * AfxGetStkUIApp( )
{
	CStkUIApp * pApp = (CStkUIApp *)AfxGetApp();
	ASSERT( pApp );
	return pApp;
}

_AFXCORE_INLINE	CMainFrame	* AfxGetMainFrame( )
{
	CMainFrame * pFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	ASSERT( !pFrame || pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)) );
	return pFrame;
}

_AFXCORE_INLINE	CStaticDoc	* AfxGetStaticDoc( )
{
	CStkUIApp * pApp = AfxGetStkUIApp();
	CStaticDoc * pDoc = pApp->GetStaticDoc();
#ifdef _TSK_MAINFRAME
	if (pDoc == NULL)
		return (CStaticDoc*)AfxGetMainFrame()->GetActiveDocument();
#endif
	ASSERT( pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CStaticDoc)) );
	return pDoc;
}

_AFXCORE_INLINE	CStrategy	* AfxGetActiveStrategy( )
{
	CStkUIApp * pApp = AfxGetStkUIApp();
	CStrategy * pStrategy = pApp->GetActiveStrategy();
	return pStrategy;
}

_AFXCORE_INLINE	CStrategyView * AfxGetStrategyView( )
{
	CStrategyView * pView = NULL;
	CMainFrame * pFrame = AfxGetMainFrame();
	//if( pFrame )
	//	pView = DYNAMIC_DOWNCAST(CStrategyView,pFrame->GetStrategyView( ) );
	return pView;
}

_AFXCORE_INLINE	CGroupView * AfxGetGroupView( )
{
	CGroupView * pView = NULL;
	CMainFrame * pFrame = AfxGetMainFrame();
	//if( pFrame )
	//	pView = DYNAMIC_DOWNCAST(CGroupView,pFrame->GetGroupView( ) );
	return pView;
}

_AFXCORE_INLINE	CTechsView * AfxGetTechsView( )
{
	CTechsView * pView = NULL;
	CMainFrame * pFrame = AfxGetMainFrame();
	//if( pFrame )
	//	pView = DYNAMIC_DOWNCAST(CTechsView,pFrame->GetTechsView( ) );
	return pView;
}

_AFXCORE_INLINE	CWizardView * AfxGetWizardView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CWizardView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CWizardView)) );
}

_AFXCORE_INLINE	CSimuView * AfxGetSimuView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CSimuView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CSimuView)) );
}

_AFXCORE_INLINE	CSListView * AfxGetSListView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CSListView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CSListView)) );
}

_AFXCORE_INLINE	CGraphView * AfxGetGraphView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CGraphView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CGraphView)) );
}

_AFXCORE_INLINE	CRealTimeView * AfxGetRealTimeView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CRealTimeView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CRealTimeView)) );
}

_AFXCORE_INLINE	CMultiSortView * AfxGetMultiSortView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CMultiSortView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CMultiSortView)) );
}

_AFXCORE_INLINE	CBaseView * AfxGetBaseView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CBaseView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CBaseView)) );
}

_AFXCORE_INLINE	CInfoView * AfxGetInfoView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CInfoView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CInfoView)) );
}

_AFXCORE_INLINE	CSelectorView * AfxGetSelectorView( )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	return (CSelectorView *)( pStaticDoc->GetViewIfExist(RUNTIME_CLASS(CSelectorView)) );
}

_AFXCORE_INLINE	BOOL AfxSwitchToStaticView( CRuntimeClass * pViewClass  )
{
	CStaticDoc * pStaticDoc = AfxGetStaticDoc();
	BOOL bOK = pStaticDoc->ShowStaticView( pViewClass );
	CView * pView = pStaticDoc->GetViewIfExist( pViewClass );
	if( pView )
		pView->SetFocus( );
	return bOK;
}

_AFXCORE_INLINE void AfxShowSlist( int nType, LPCTSTR lpszDomain, DWORD dwDate )
{
	CSListView	* pSListView = AfxGetSListView( );
	if( pSListView )
	{
		pSListView->SetCurrentStatus( nType, lpszDomain, dwDate );
		AfxSwitchToStaticView( RUNTIME_CLASS(CSListView) );
	}
	else
	{
		AfxSwitchToStaticView( RUNTIME_CLASS(CSListView) );
		pSListView = AfxGetSListView( );
		if( pSListView )
			pSListView->SetCurrentStatus( nType, lpszDomain, dwDate );
	}
}

_AFXCORE_INLINE void AfxShowSlistSortDP( int nType, LPCTSTR lpszDomain, DWORD dwDate )
{
	CSListView	* pSListView = AfxGetSListView( );
	if( pSListView )
	{
		pSListView->SetSortVariant( SLH_DIFFPERCENT );
		pSListView->SetCurrentStatus( nType, lpszDomain, dwDate );
		AfxSwitchToStaticView( RUNTIME_CLASS(CSListView) );
	}
	else
	{
		AfxSwitchToStaticView( RUNTIME_CLASS(CSListView) );
		pSListView = AfxGetSListView( );
		if( pSListView )
		{
			pSListView->SetSortVariant( SLH_DIFFPERCENT );
			pSListView->SetCurrentStatus( nType, lpszDomain, dwDate );
		}
	}
}

_AFXCORE_INLINE	void AfxShowInfo( int nServer )
{
	CInfoView	* pInfoView = AfxGetInfoView( );
	if( pInfoView )
	{
		pInfoView->SetCurrentServer( nServer );
		AfxSwitchToStaticView( RUNTIME_CLASS(CInfoView) );
		pInfoView->OnRefresh( );
	}
	else
	{
		AfxSwitchToStaticView( RUNTIME_CLASS(CInfoView) );
		pInfoView = AfxGetInfoView( );
		if( pInfoView )
		{
			pInfoView->SetCurrentServer( nServer );
			pInfoView->OnRefresh( );
		}
	}
}

_AFXCORE_INLINE	void AfxShowMultiSort( LONG type )
{
	CMultiSortView	* pMultiSortView = AfxGetMultiSortView( );
	if( pMultiSortView )
	{
		pMultiSortView->SetMultiSortClass( type );
		pMultiSortView->Invalidate();
		AfxSwitchToStaticView( RUNTIME_CLASS(CMultiSortView) );
	}
	else
	{
		AfxSwitchToStaticView( RUNTIME_CLASS(CMultiSortView) );
		pMultiSortView = AfxGetMultiSortView( );
		if( pMultiSortView )
		{
			pMultiSortView->SetMultiSortClass( type );
			pMultiSortView->Invalidate();
		}
	}
}

_AFXCORE_INLINE	BOOL AfxShowStockRealTime( int nStockIndex, BOOL bFromSList )
{
	AfxGetProfile().SetCurrentStock( nStockIndex, bFromSList );

	CRealTimeView * pView = AfxGetRealTimeView();
	if( pView )
	{
		pView->ChangeDrawMode( CRealTime::modePriceLine );
		pView->ShowMultiStock( 1 );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_REALTIMEVIEW, NULL );
		AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
	}
	else
	{
		AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
		pView = AfxGetRealTimeView();
		if( pView )
		{
			pView->ChangeDrawMode( CRealTime::modePriceLine );
			pView->ShowMultiStock( 1 );
		}
	}
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxShowStockRealTime( const char * szCode )
{
	AfxGetProfile().SetCurrentStock( szCode, FALSE );

	CRealTimeView * pView = AfxGetRealTimeView();
	if( pView )
	{
		pView->ChangeDrawMode( CRealTime::modePriceLine );
		pView->ShowMultiStock( 1 );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_REALTIMEVIEW, NULL );
		AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
	}
	else
	{
		AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
		pView = AfxGetRealTimeView();
		if( pView )
		{
			pView->ChangeDrawMode( CRealTime::modePriceLine );
			pView->ShowMultiStock( 1 );
		}
	}
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxShowStockGraph( int nStockIndex, BOOL bFromSList )
{
	AfxGetProfile().SetCurrentStock( nStockIndex, bFromSList );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );

	AfxSwitchToStaticView( RUNTIME_CLASS(CGraphView) );
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxShowStockGraph( const char * szCode )
{
	AfxGetProfile().SetCurrentStock( szCode, FALSE );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );

	AfxSwitchToStaticView( RUNTIME_CLASS(CGraphView) );
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxShowStockBase( int nStockIndex, BOOL bFromSList )
{
	AfxGetProfile().SetCurrentStock( nStockIndex, bFromSList );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_BASEVIEW, NULL );
	
	AfxSwitchToStaticView( RUNTIME_CLASS(CBaseView) );
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxShowStockBase( const char * szCode )
{
	AfxGetProfile().SetCurrentStock( szCode, FALSE );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_BASEVIEW, NULL );
	
	AfxSwitchToStaticView( RUNTIME_CLASS(CBaseView) );
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxShowStockInfo( int nStockIndex, BOOL bFromSList )
{
	AfxGetProfile().SetCurrentStock( nStockIndex, bFromSList );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_INFOVIEW, NULL );
	
	AfxSwitchToStaticView( RUNTIME_CLASS(CInfoView) );
	::PostMessage( AfxGetInfoView()->GetSafeHwnd(), WM_COMMAND, ID_VIEW_F10, 0 );
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxShowStockInfo( const char * szCode )
{
	AfxGetProfile().SetCurrentStock( szCode, FALSE );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_INFOVIEW, NULL );
	
	AfxSwitchToStaticView( RUNTIME_CLASS(CInfoView) );
	::PostMessage( AfxGetInfoView()->GetSafeHwnd(), WM_COMMAND, ID_VIEW_F10, 0 );
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxShowStockTech( UINT nTech )
{
	if( nTech >= STT_KLINE_MIN && nTech <= STT_KLINE_MAX )
	{
		CSPDWordArray	&	anKLine	=	AfxGetProfile().GetGraphTechsKLine();
		anKLine.RemoveAll();
		anKLine.Add( nTech );
	}
	else
	{
		CSPDWordArray	&	anTech	=	AfxGetProfile().GetGraphTechsShow();
		for( int k=anTech.GetSize()-1; k>=0; k-- )
		{
			if( nTech == anTech[k] )
				anTech.RemoveAt(k);
		}
		if( anTech.GetSize() == 0 )
			anTech.Add( nTech );
		else
			anTech.SetAt( anTech.GetSize()-1, nTech );
	}

	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
	AfxSwitchToStaticView( RUNTIME_CLASS(CGraphView) );
	return TRUE;
}

// 策略改动之后，刷新视图
_AFXCORE_INLINE	void AfxOnStrategyUpdated( CStrategy * pStrategy, BOOL bReRealRun )
{
	CStrategyView * pStrategyView = AfxGetStrategyView();
	if( pStrategyView )
		pStrategyView->SendMessage( WM_USER_UPDATESTRATEGY, DWORD(pStrategy) );
	CStaticDoc  * pStaticDoc  = AfxGetStaticDoc();
	if( pStaticDoc )
	{
		pStaticDoc->UpdateAllViews( NULL, UPDATE_HINT_WIZARDVIEW, NULL );
		if( bReRealRun )
			pStaticDoc->UpdateAllViews( NULL, UPDATE_HINT_SIMUVIEW_REREALRUN, NULL );
		else
			pStaticDoc->UpdateAllViews( NULL, UPDATE_HINT_SIMUVIEW, NULL );
	}
}

_AFXCORE_INLINE	BOOL AfxMenuInsertDomain( CMenu *pMenu, int nMenuPos, UINT nIDStart, UINT nIDEnd )
{
	if( !pMenu || !::IsMenu( pMenu->m_hMenu ) )
		return FALSE;

	CDomainContainer &container = AfxGetDomainContainer();
	for( UINT nID=nIDStart; (int(nID-nIDStart)) < container.GetSize() && nID <= nIDEnd; nID ++ )
	{
		CDomain	& domain = container.ElementAt(nID-nIDStart);
		UINT nFlags	=	MF_BYPOSITION | MF_STRING;
		if( nID-nIDStart+1 >= 79 || nID == nIDEnd )
		{
			CString	strMoreDomain;
			strMoreDomain.LoadString( IDS_DOMAIN_MOREDOMAIN );
			pMenu->InsertMenu( nMenuPos++, MF_BYPOSITION | MF_SEPARATOR, NULL, "" );
			pMenu->InsertMenu( nMenuPos++, nFlags, nIDEnd, strMoreDomain );
			break;
		}
		if( ! ((nID-nIDStart+1)%20) && nID != nIDStart )
			nFlags	|=	MF_MENUBARBREAK;
		pMenu->InsertMenu( nMenuPos++, nFlags, nID, domain.m_strName );
	}

	if( 0 == container.GetSize() )
	{
		CString	strNoDomain;
		strNoDomain.LoadString( IDS_SLISTBAR_NODOMAIN );
		pMenu->InsertMenu( nMenuPos++, MF_BYPOSITION | MF_STRING, ID_SEPARATOR, strNoDomain );
	}
	return TRUE;
}

_AFXCORE_INLINE	BOOL AfxMenuInsertGroup(CMenu *pMenu, int nMenuPos, UINT nIDStart, UINT nIDEnd, BOOL bEnable )
{
	if( !pMenu || !::IsMenu( pMenu->m_hMenu ) )
		return FALSE;

	CDomainContainer &container = AfxGetGroupContainer();
	for( UINT nID=nIDStart; (int(nID-nIDStart)) < container.GetSize() && nID <= nIDEnd; nID ++ )
	{
		CDomain	& domain = container.ElementAt(nID-nIDStart);
		UINT nFlags	=	MF_BYPOSITION | MF_STRING;
		if( !bEnable )
			nFlags	|=	MF_GRAYED;
		if( nID-nIDStart+1 >= 79 || nID == nIDEnd )
		{
			CString	strMoreGroup;
			strMoreGroup.LoadString( IDS_DOMAIN_MOREGROUP );
			pMenu->InsertMenu( nMenuPos++, MF_BYPOSITION | MF_SEPARATOR, NULL, "" );
			pMenu->InsertMenu( nMenuPos++, nFlags, nIDEnd, strMoreGroup );
			break;
		}
		if( ! ((nID-nIDStart+1)%20) && nID != nIDStart )
			nFlags	|=	MF_MENUBARBREAK;
		pMenu->InsertMenu( nMenuPos ++, nFlags, nID, domain.m_strName );
	}

	if( 0 == container.GetSize() )
	{
		CString	strNoGroup;
		strNoGroup.LoadString( IDS_SLISTBAR_NOGROUP );
		pMenu->InsertMenu( nMenuPos++, MF_BYPOSITION | MF_STRING, ID_SEPARATOR, strNoGroup );
	}
	return TRUE;
}

