// SetRule.cpp : implementation file
//

#include "stdafx.h"

#include "SetRule.h"
#include "../ParamDlg/SetParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRule property page

#define	SETRULE_MAINLEFT	228
#define	SETRULE_MAINTOP		23


IMPLEMENT_DYNCREATE(CSetRule, CPropertyPageEx)

CSetRule::CSetRule() : CPropertyPageEx(CSetRule::IDD)
{
	//{{AFX_DATA_INIT(CSetRule)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParamDlg			=	NULL;
	m_pTechParameters	=	NULL;
	m_panSelectedTechs	=	NULL;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
	m_bModified		=	FALSE;
}

CSetRule::CSetRule( UINT nIDCaption, UINT nIDTitle, UINT nIDSubTitle )
		 : CPropertyPageEx(CSetRule::IDD,nIDCaption, nIDTitle, nIDSubTitle)
{
	m_pParamDlg			=	NULL;
	m_pTechParameters	=	NULL;
	m_panSelectedTechs	=	NULL;

	m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;
	m_bModified		=	FALSE;
}

CSetRule::~CSetRule()
{
	if( m_pParamDlg )
	{
		delete	m_pParamDlg;
		m_pParamDlg	=	NULL;
	}
}

void CSetRule::SetTechParameters( CTechParameters * pParam, CSPDWordArray *panSelectedTechs )
{
	ASSERT( pParam && panSelectedTechs );
	m_pTechParameters	=	pParam;
	m_panSelectedTechs	=	panSelectedTechs;
	m_anSelectedTechs.RemoveAll();
	m_anSelectedTechs.Copy( *panSelectedTechs );
}

BOOL CSetRule::InitTreeTechs( )
{
	if( NULL == m_ImageList.GetSafeHandle() )
	{
		m_ImageList.Create(IDB_TECHSVIEW_ICONS, 16, 1, RGB(0,255,0));
		m_tree.SetImageList (&m_ImageList, TVSIL_NORMAL);
	}

	m_tree.DeleteAllItems();

	CString	strClassLast;
	HTREEITEM	hTechClass = NULL, hTech = NULL, hTechFirstSelected = NULL;
	UINT nTechUserCount = CTechUser::GetTechUserCount();
	for( UINT i=STT_MIN; i <= STT_MAX+nTechUserCount; i ++ )
	{
		UINT	nTech	=	i;
		if( nTech > STT_MAX )
			nTech	=	i-STT_MAX-1+STT_USER_MIN;

		if( STT_PV == nTech || STT_DJ == nTech || STT_CW == nTech || STT_MIKE == nTech
			|| STT_AD == nTech || STT_CI == nTech || STT_R == nTech || STT_DMI == nTech
			|| STT_CDP == nTech )
			continue;

		CString	strClass	=	AfxGetSTTClassName(nTech);
		if( strClass.GetLength() > 0 && 0 != strClass.Compare(strClassLast) )
		{
			hTechClass = m_tree.InsertItem(strClass,IMG_TECHCLASS,IMG_TECHCLASS_SEL);
			m_tree.SetItemData( hTechClass, 0 );
			m_anClassChecked.Add( 0 );
		}

		strClassLast	=	strClass;

		if( NULL == hTechClass )
			hTechClass	=	TVI_ROOT;
		CString	strTechName	=	AfxGetSTTFullName(nTech);
		hTech	=	m_tree.InsertItem(strTechName,IMG_TECH,IMG_TECH_SEL,hTechClass);
		m_tree.SetItemData( hTech, nTech );

		for( int i=0; i<m_anSelectedTechs.GetSize(); i++ )
		{
			if( m_anSelectedTechs[i] == nTech )
			{
				m_tree.SetCheck( hTech, TRUE );
				if( NULL == hTechFirstSelected )
					hTechFirstSelected	=	hTech;
			}
		}
	}

	HTREEITEM	hParent	=	m_tree.GetParentItem(hTechFirstSelected);
	if( NULL == hParent )
		hParent	=	m_tree.GetRootItem();
	m_tree.Expand( hParent, TVE_EXPAND );
	m_tree.Select( hTechFirstSelected, TVGN_CARET );
	
	::PostMessage( GetSafeHwnd(), WM_USER_SETRULECLICKTREE, 0, 0 );
	return TRUE;
}

void CSetRule::GetCurrentSelectedTechs( CSPDWordArray & au )
{
	au.RemoveAll();
	HTREEITEM	hItem	=	m_tree.GetRootItem();
	while( hItem )
	{
		HTREEITEM	hChild	=	m_tree.GetChildItem( hItem );
		while( hChild )
		{
			if( m_tree.GetCheck( hChild ) )
			{
				UINT	nTech	=	m_tree.GetItemData(hChild);
				if( (nTech >= STT_MIN && nTech <= STT_MAX) || nTech >= STT_USER_MIN )
					au.Add( nTech );
			}
			hChild	=	m_tree.GetNextItem( hChild, TVGN_NEXT );
		}
		
		hItem = m_tree.GetNextItem( hItem, TVGN_NEXT );
	}
}

BOOL CSetRule::ValidData( )
{
	if( m_pParamDlg && ::IsWindow(m_pParamDlg->GetSafeHwnd()) )
	{
		if( !m_pParamDlg->RefreshData( TRUE ) )
		{
			AfxMessageBox( IDS_SETPARAM_INVALID, MB_OK | MB_ICONINFORMATION );
			return FALSE;
		}
	}

	GetCurrentSelectedTechs( m_anSelectedTechs );
	if( m_anSelectedTechs.GetSize() <= 0 )
	{
		AfxMessageBox( IDS_SETRULE_NOSELECTED, MB_OK | MB_ICONINFORMATION );
		return FALSE;
	}

	return TRUE;
}

BOOL CSetRule::StoreData( )
{
	GetCurrentSelectedTechs( m_anSelectedTechs );
	if( m_panSelectedTechs )
		m_bModified = (m_bModified || !m_panSelectedTechs->IsEqualTo(m_anSelectedTechs) );
	if( m_panSelectedTechs )
		m_panSelectedTechs->Copy( m_anSelectedTechs );

	if( m_pParamDlg && ::IsWindow(m_pParamDlg->GetSafeHwnd()) )
	{
		m_pParamDlg->OnCmdMsg( IDOK, 0, NULL, NULL );
		delete	m_pParamDlg;
		m_pParamDlg	=	NULL;
	}
	return TRUE;
}

void CSetRule::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRule)
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_ALLDEFAULT, m_btnAllDefault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRule, CPropertyPageEx)
	//{{AFX_MSG_MAP(CSetRule)
	ON_BN_CLICKED(IDC_ALLDEFAULT, OnAlldefault)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, OnSelchangingTree)
	ON_NOTIFY(NM_CLICK, IDC_TREE, OnClickTree)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_SETRULECLICKTREE, OnSetRuleClickTree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRule message handlers

BOOL CSetRule::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	InitTreeTechs( );

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetRule::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return FALSE;
	StoreData( );
	return CPropertyPageEx::OnKillActive();
}

LRESULT CSetRule::OnWizardBack() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return -1L;
	StoreData( );
	return CPropertyPageEx::OnWizardBack();
}

LRESULT CSetRule::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return -1L;
	StoreData( );
	return CPropertyPageEx::OnWizardNext();
}

BOOL CSetRule::OnWizardFinish() 
{
	// Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return FALSE;
	StoreData( );
	return CPropertyPageEx::OnWizardFinish();
}

void CSetRule::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData( );
	if( !ValidData( ) )
		return;
	StoreData( );
	CPropertyPageEx::OnOK();
}

void CSetRule::OnAlldefault() 
{
	// TODO: Add your control notification handler code here
	if( m_pTechParameters )
	{
		m_pTechParameters->SetDefaultParametersAll( );
	}

	if( m_pParamDlg && ::IsWindow(m_pParamDlg->GetSafeHwnd()) )
	{
		m_pParamDlg->RefreshData( FALSE );
	}
}

void CSetRule::OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( m_pParamDlg && ::IsWindow(m_pParamDlg->GetSafeHwnd()) )
	{
		if( !m_pParamDlg->RefreshData( TRUE ) )
		{
			AfxMessageBox( IDS_SETPARAM_INVALID, MB_OK | MB_ICONINFORMATION );
			*pResult = -1;
			return;
		}
		m_pParamDlg->OnCmdMsg( IDOK, 0, NULL, NULL );
		delete	m_pParamDlg;
		m_pParamDlg	=	NULL;
	}
	
	*pResult = 0;
}

void CSetRule::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( NULL == m_pTechParameters )
	{
		*pResult = 0;
		return;
	}

	HTREEITEM	hSelItem	=	m_tree.GetSelectedItem();
	if( hSelItem )
	{
		DWORD	dwData	=	m_tree.GetItemData( hSelItem );
		if( !( (dwData >= STT_MIN && dwData <= STT_MAX) || dwData >= STT_USER_MIN ) )
			return;

		CTechParameters	&	param	=	(*m_pTechParameters);
		switch( dwData )
		{
		case STT_MA:		m_pParamDlg	=	new CMADlg(NULL,&(param.ma));		m_pParamDlg->Create( CMADlg::IDD, this );	break;
		case STT_BBI:		m_pParamDlg	=	new CBBIDlg(NULL,&(param.bbi));		m_pParamDlg->Create( CBBIDlg::IDD, this );	break;
		case STT_BOLL:		m_pParamDlg	=	new CBOLLDlg(NULL,&(param.boll));	m_pParamDlg->Create( CBOLLDlg::IDD, this );	break;
		case STT_PV:		m_pParamDlg	=	new CPVDlg(NULL,&(param.pv));		m_pParamDlg->Create( CPVDlg::IDD, this );	break;
		case STT_SAR:		m_pParamDlg	=	new CSARDlg(NULL,&(param.sar));		m_pParamDlg->Create( CSARDlg::IDD, this );	break;
		case STT_DJ:		m_pParamDlg	=	new CDJDlg(NULL,&(param.dj));		m_pParamDlg->Create( CDJDlg::IDD, this );	break;
		case STT_CW:		m_pParamDlg	=	new CCWDlg(NULL,&(param.cw));		m_pParamDlg->Create( CCWDlg::IDD, this );	break;
		
		case STT_MACD:		m_pParamDlg	=	new CMACDDlg(NULL,&(param.macd));	m_pParamDlg->Create( CMACDDlg::IDD, this );	break;
		case STT_MIKE:		m_pParamDlg	=	new CMIKEDlg(NULL,&(param.mike));	m_pParamDlg->Create( CMIKEDlg::IDD, this );	break;
		case STT_PSY:		m_pParamDlg	=	new CPSYDlg(NULL,&(param.psy));		m_pParamDlg->Create( CPSYDlg::IDD, this );	break;

		case STT_VOLUME:	m_pParamDlg	=	new CVOLUMEDlg(NULL,&(param.volume));	m_pParamDlg->Create( CVOLUMEDlg::IDD, this );	break;
		case STT_NVI:		m_pParamDlg	=	new CNVIDlg(NULL,&(param.nvi));		m_pParamDlg->Create( CNVIDlg::IDD, this );	break;
		case STT_PVI:		m_pParamDlg	=	new CPVIDlg(NULL,&(param.pvi));		m_pParamDlg->Create( CPVIDlg::IDD, this );	break;
		case STT_VR:		m_pParamDlg	=	new CVRDlg(NULL,&(param.vr));		m_pParamDlg->Create( CVRDlg::IDD, this );	break;
		case STT_VROC:		m_pParamDlg	=	new CVROCDlg(NULL,&(param.vroc));	m_pParamDlg->Create( CVROCDlg::IDD, this );	break;
		case STT_OBV:		m_pParamDlg	=	new COBVDlg(NULL,&(param.obv));		m_pParamDlg->Create( COBVDlg::IDD, this );	break;
		case STT_MOBV:		m_pParamDlg	=	new CMOBVDlg(NULL,&(param.mobv));	m_pParamDlg->Create( CMOBVDlg::IDD, this );	break;
		case STT_MFI:		m_pParamDlg	=	new CMFIDlg(NULL,&(param.mfi));		m_pParamDlg->Create( CMFIDlg::IDD, this );	break;
		case STT_VMACD:		m_pParamDlg	=	new CVMACDDlg(NULL,&(param.vmacd));	m_pParamDlg->Create( CVMACDDlg::IDD, this );	break;
		case STT_WVAD:		m_pParamDlg	=	new CWVADDlg(NULL,&(param.wvad));	m_pParamDlg->Create( CWVADDlg::IDD, this );	break;
		case STT_EMV:		m_pParamDlg	=	new CEMVDlg(NULL,&(param.emv));		m_pParamDlg->Create( CEMVDlg::IDD, this );	break;
		case STT_VRSI:		m_pParamDlg	=	new CVRSIDlg(NULL,&(param.vrsi));	m_pParamDlg->Create( CVRSIDlg::IDD, this );	break;
		case STT_NVRSI:		m_pParamDlg	=	new CNVRSIDlg(NULL,&(param.nvrsi));	m_pParamDlg->Create( CNVRSIDlg::IDD, this );	break;
		case STT_AD:		m_pParamDlg	=	new CADDlg(NULL,&(param.ad));		m_pParamDlg->Create( CADDlg::IDD, this );	break;
		case STT_CI:		m_pParamDlg	=	new CCIDlg(NULL,&(param.ci));		m_pParamDlg->Create( CCIDlg::IDD, this );	break;

		case STT_KDJ:		m_pParamDlg	=	new CKDJDlg(NULL,&(param.kdj));		m_pParamDlg->Create( CKDJDlg::IDD, this );	break;
		case STT_R:			m_pParamDlg	=	new CRDlg(NULL,&(param.r));			m_pParamDlg->Create( CRDlg::IDD, this );	break;
		case STT_RSI:		m_pParamDlg	=	new CRSIDlg(NULL,&(param.rsi));		m_pParamDlg->Create( CRSIDlg::IDD, this );	break;
		case STT_BIAS:		m_pParamDlg	=	new CBIASDlg(NULL,&(param.bias));	m_pParamDlg->Create( CBIASDlg::IDD, this );	break;
		case STT_MTM:		m_pParamDlg	=	new CMTMDlg(NULL,&(param.mtm));		m_pParamDlg->Create( CMTMDlg::IDD, this );	break;
		case STT_DMI:		m_pParamDlg	=	new CDMIDlg(NULL,&(param.dmi));		m_pParamDlg->Create( CDMIDlg::IDD, this );	break;
		case STT_ROC:		m_pParamDlg	=	new CROCDlg(NULL,&(param.roc));		m_pParamDlg->Create( CROCDlg::IDD, this );	break;
		case STT_CCI:		m_pParamDlg	=	new CCCIDlg(NULL,&(param.cci));		m_pParamDlg->Create( CCCIDlg::IDD, this );	break;
		case STT_CV:		m_pParamDlg	=	new CCVDlg(NULL,&(param.cv));		m_pParamDlg->Create( CCVDlg::IDD, this );	break;
		case STT_ARBR:		m_pParamDlg	=	new CARBRDlg(NULL,&(param.arbr));	m_pParamDlg->Create( CARBRDlg::IDD, this );	break;
		case STT_CR:		m_pParamDlg	=	new CCRDlg(NULL,&(param.cr));		m_pParamDlg->Create( CCRDlg::IDD, this );	break;
		case STT_OSC:		m_pParamDlg	=	new COSCDlg(NULL,&(param.osc));		m_pParamDlg->Create( COSCDlg::IDD, this );	break;
		case STT_UOS:		m_pParamDlg	=	new CUOSDlg(NULL,&(param.uos));		m_pParamDlg->Create( CUOSDlg::IDD, this );	break;
		case STT_MAOSC:		m_pParamDlg	=	new CMAOSCDlg(NULL,&(param.maosc));	m_pParamDlg->Create( CMAOSCDlg::IDD, this );	break;
		case STT_36BIAS:	m_pParamDlg	=	new C36BIASDlg(NULL,&(param.bias36));	m_pParamDlg->Create( C36BIASDlg::IDD, this );	break;
		case STT_DPO:		m_pParamDlg	=	new CDPODlg(NULL,&(param.dpo));		m_pParamDlg->Create( CDPODlg::IDD, this );	break;
		case STT_KST:		m_pParamDlg	=	new CKSTDlg(NULL,&(param.kst));		m_pParamDlg->Create( CKSTDlg::IDD, this );	break;

		case STT_REI:		m_pParamDlg	=	new CREIDlg(NULL,&(param.rei));		m_pParamDlg->Create( CREIDlg::IDD, this );	break;
		case STT_DMKI:		m_pParamDlg	=	new CDMKIDlg(NULL,&(param.dmki));	m_pParamDlg->Create( CDMKIDlg::IDD, this );	break;
		case STT_PCNT:		m_pParamDlg	=	new CPCNTDlg(NULL,&(param.pcnt));	m_pParamDlg->Create( CPCNTDlg::IDD, this );	break;
		case STT_HLC:		m_pParamDlg	=	new CHLCDlg(NULL,&(param.hlc));		m_pParamDlg->Create( CHLCDlg::IDD, this );	break;
		case STT_CDP:		m_pParamDlg	=	new CCDPDlg(NULL,&(param.cdp));		m_pParamDlg->Create( CCDPDlg::IDD, this );	break;
		case STT_ASI:		m_pParamDlg	=	new CASIDlg(NULL,&(param.asi));		m_pParamDlg->Create( CASIDlg::IDD, this );	break;
		case STT_ATR:		m_pParamDlg	=	new CATRDlg(NULL,&(param.atr));		m_pParamDlg->Create( CATRDlg::IDD, this );	break;

		case STT_CYO:		m_pParamDlg	=	new CCYODlg(NULL,&(param.cyo));	m_pParamDlg->Create( CCYODlg::IDD, this );	break;
		case STT_DCYO:		m_pParamDlg	=	new CDCYODlg(NULL,&(param.dcyo));	m_pParamDlg->Create( CDCYODlg::IDD, this );	break;
		case STT_HSL:		m_pParamDlg	=	new CHSLDlg(NULL,&(param.hsl));	m_pParamDlg->Create( CHSLDlg::IDD, this );	break;
		case STT_DPER:		m_pParamDlg	=	new CDPERDlg(NULL,&(param.dper));	m_pParamDlg->Create( CDPERDlg::IDD, this );	break;

		default:
			;
		}
		if( m_pParamDlg && ::IsWindow(m_pParamDlg->GetSafeHwnd()) )
			m_pParamDlg->SetWindowPos( NULL, SETRULE_MAINLEFT, SETRULE_MAINTOP, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOACTIVATE );
		m_tree.SetFocus();
	}
	
	*pResult = 0;
}

void CSetRule::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	::PostMessage( GetSafeHwnd(), WM_USER_SETRULECLICKTREE, 0, 0 );

	*pResult = 0;
}

LRESULT CSetRule::OnSetRuleClickTree(WPARAM wParam, LPARAM lParam)
{
	// Check Children same as Class Item
	int	nCount	=	0;
	HTREEITEM	hItem	=	m_tree.GetRootItem();
	while( hItem )
	{
		UINT	temp1	=	m_anClassChecked[nCount];
		UINT	temp2	=	m_tree.GetCheck( hItem );
		if( m_anClassChecked[nCount] != (UINT)m_tree.GetCheck( hItem ) )
		{
			BOOL	bChecked	=	m_tree.GetCheck( hItem );
			m_anClassChecked.SetAt( nCount, bChecked );
			HTREEITEM	hChild	=	m_tree.GetChildItem( hItem );
			while( hChild )
			{
				m_tree.SetCheck( hChild, bChecked );
				hChild	=	m_tree.GetNextItem( hChild, TVGN_NEXT );
			}
		}
		hItem = m_tree.GetNextItem( hItem, TVGN_NEXT );
		nCount	++;
	}

	// Check Class Item Check according Children
	nCount	=	0;
	hItem	=	m_tree.GetRootItem();
	while( hItem )
	{
		BOOL	bCheckedAll	=	TRUE;
		HTREEITEM	hChild	=	m_tree.GetChildItem( hItem );
		while( hChild )
		{
			if( !m_tree.GetCheck( hChild ) )
				bCheckedAll	=	FALSE;
			hChild	=	m_tree.GetNextItem( hChild, TVGN_NEXT );
		}
		
		m_anClassChecked.SetAt( nCount, bCheckedAll );
		m_tree.SetCheck( hItem, bCheckedAll );
	
		hItem = m_tree.GetNextItem( hItem, TVGN_NEXT );
		nCount	++;
	}

	return 0L;
}

BOOL CSetRule::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
		return TRUE;
	return CPropertyPageEx::PreTranslateMessage(pMsg);
}

BOOL CSetRule::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheetEx * pSheet = DYNAMIC_DOWNCAST( CPropertySheetEx, GetParent()	);
	if( pSheet )
	{
		pSheet->SetWizardButtons( m_dwButtonFlags );
	}
	
	return CPropertyPageEx::OnSetActive();
}
