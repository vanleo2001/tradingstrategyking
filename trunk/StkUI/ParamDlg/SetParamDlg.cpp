// SetParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTechComboBox

BOOL CTechComboBox::Initialize( )
{
	ResetContent();
	
	for( int nTech = STT_MIN; nTech < STT_MAX; nTech ++ )
	{
		int nIndex	=	AddString( AfxGetSTTFullName(nTech) );
		SetItemData( nIndex, nTech );
	}

	SetCurSel( 0 );
	return TRUE;
}

BOOL CTechComboBox::Select( int nTech )
{
	for( int i=0; i<GetCount(); i++ )
	{
		if( ((UINT)nTech) == GetItemData(i) )
		{
			SetCurSel(i);
			return TRUE;
		}
	}
	return FALSE;
}

int CTechComboBox::GetSelect( )
{
	int nSel	=	GetCurSel();
	if( CB_ERR == nSel )
		return STT_MIN;
	int	nTech	=	GetItemData(nSel);
	if( nTech >= STT_MIN && nTech <= STT_MAX )
		return nTech;
	return STT_MIN;
}

/////////////////////////////////////////////////////////////////////////////
// CKTypeComboBox

BOOL CKTypeComboBox::Initialize( )
{
	ResetContent();
	
	int	ktype, nIndex, nSel = CB_ERR;

	ktype	=	CKData::ktypeMin5;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	ktype	=	CKData::ktypeMin15;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	ktype	=	CKData::ktypeMin30;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	ktype	=	CKData::ktypeMin60;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );
	
	ktype	=	CKData::ktypeDay;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );
	nSel	=	nIndex;

	ktype	=	CKData::ktypeWeek;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	ktype	=	CKData::ktypeMonth;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	SetCurSel( nSel );
	return TRUE;
}

BOOL CKTypeComboBox::InitializeDayMin5( )
{
	ResetContent();
	
	int	ktype, nIndex, nSel = CB_ERR;

	ktype	=	CKData::ktypeMin5;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	ktype	=	CKData::ktypeDay;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );
	nSel	=	nIndex;

	SetCurSel( nSel );
	return TRUE;
}

BOOL CKTypeComboBox::InitializeDay( )
{
	ResetContent();
	
	int	ktype, nIndex, nSel = CB_ERR;
	
	ktype	=	CKData::ktypeDay;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );
	nSel	=	nIndex;

	ktype	=	CKData::ktypeWeek;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	ktype	=	CKData::ktypeMonth;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	SetCurSel( nSel );
	return TRUE;
}

BOOL CKTypeComboBox::InitializeWeek( )
{
	ResetContent();
	
	int	ktype, nIndex, nSel = CB_ERR;

	ktype	=	CKData::ktypeWeek;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );
	nSel	=	nIndex;

	ktype	=	CKData::ktypeMonth;
	nIndex	=	AddString( AfxGetKTypeString(ktype) );
	SetItemData( nIndex, ktype );

	SetCurSel( nSel );
	return TRUE;
}

BOOL CKTypeComboBox::Select( int nKType )
{
	for( int i=0; i<GetCount(); i++ )
	{
		if( ((UINT)nKType) == GetItemData(i) )
		{
			SetCurSel(i);
			return TRUE;
		}
	}
	return FALSE;
}

int CKTypeComboBox::GetSelect( )
{
	int nSel	=	GetCurSel();
	if( CB_ERR == nSel )
		return CKData::ktypeDay;
	int	nKType	=	GetItemData(nSel);
	if( nKType >= CKData::ktypeMin && nKType <= CKData::ktypeMax )
		return nKType;
	return CKData::ktypeDay;
}

/////////////////////////////////////////////////////////////////////////////
// CIntensityComboBox

BOOL CIntensityComboBox::InitializeGate( )
{
	ResetContent();

	int	nSignal, nIndex, nSel = CB_ERR;
	for( nSignal = ITS_MAX; nSignal >= ITS_MIN; nSignal -- )
	{
		if( ITSG_BUYINTENSE != nSignal && ITSG_BUY != nSignal && ITSG_BUYFEEBLE != nSignal && ITS_NOTHING != nSignal
			&& ITSG_SELLINTENSE != nSignal && ITSG_SELL != nSignal && ITSG_SELLFEEBLE != nSignal )
			continue;
		nIndex	=	AddString( AfxGetIntensityString(nSignal) );
		SetItemData( nIndex, nSignal );
		if( ITS_NOTHING == nSignal )
			nSel	=	nIndex;
	}
	SetCurSel( nSel );
	return TRUE;
}

BOOL CIntensityComboBox::Initialize( )
{
	ResetContent();

	int	nSignal, nIndex, nSel = CB_ERR;
	for( nSignal = ITS_MAX; nSignal >= ITS_MIN; nSignal -- )
	{
		if( ITS_BUYINTENSE != nSignal && ITS_BUY != nSignal && ITS_BUYFEEBLE != nSignal && ITS_NOTHING != nSignal
			&& ITS_SELLINTENSE != nSignal && ITS_SELL != nSignal && ITS_SELLFEEBLE != nSignal )
			continue;
		nIndex	=	AddString( AfxGetIntensityString(nSignal) );
		SetItemData( nIndex, nSignal );
		if( ITS_NOTHING == nSignal )
			nSel	=	nIndex;
	}
	SetCurSel( nSel );
	return TRUE;
}

BOOL CIntensityComboBox::Select( UINT nITS )
{
	for( int i=0; i<GetCount(); i++ )
	{
		if( nITS == GetItemData(i) )
		{
			SetCurSel(i);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CIntensityComboBox::ResetAndSelect( UINT nITS )
{
	Initialize( );
	return Select( nITS );
}

int CIntensityComboBox::GetSelect( )
{
	int nSel	=	GetCurSel();
	if( CB_ERR == nSel )
		return ITS_NOTHING;
	int	nITS	=	GetItemData(nSel);
	if( nITS >= ITS_MIN && nITS <= ITS_MAX )
		return nITS;
	return ITS_NOTHING;
}

/////////////////////////////////////////////////////////////////////////////
// ParamHelp

void ParamHelp( UINT nTech )
{
	CString	strTech	=	AfxGetSTTShortName( nTech );
	if( !strTech.IsEmpty() )
		strTech	=	"#" + strTech;
	HtmlHelp(NULL, "StockAna.chm::/htm/techs.htm" + strTech, HH_DISPLAY_TOPIC, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CParamDlg
void CParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PARAMHELP, m_btnParamHelp);
	DDX_Control(pDX, IDC_DEFAULT, m_btnDefault);
}

BOOL CParamDlg::RefreshData(BOOL bSaveAndValidate)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSetParamDlg dialog

#define	SETPARAM_MAINLEFT	215
#define	SETPARAM_MAINTOP	23

CSetParamDlg::CSetParamDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSetParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetParamDlg)
	//}}AFX_DATA_INIT
	m_pParamDlg	=	NULL;
	m_nSTTFirstSelected	=	STT_MIN-1;
}

CSetParamDlg::~CSetParamDlg( )
{
	if( m_pParamDlg )
	{
		delete	m_pParamDlg;
		m_pParamDlg	=	NULL;
	}
}

void CSetParamDlg::SetFirstSelected( UINT nSTT )
{
	m_nSTTFirstSelected	=	nSTT;
}

void CSetParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetParamDlg)
	DDX_Control(pDX, IDC_ALLDEFAULT, m_btnAllDefault);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetParamDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSetParamDlg)
	ON_BN_CLICKED(IDC_ALLDEFAULT, OnAlldefault)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, OnSelchangingTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetParamDlg message handlers

BOOL CSetParamDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();

	m_ImageList.Create(IDB_TECHSVIEW_ICONS, 16, 1, RGB(0,255,0));
	m_tree.SetImageList (&m_ImageList, TVSIL_NORMAL);

	CString	strClassLast;
	HTREEITEM	hTechClass = NULL, hTech = NULL;
	HTREEITEM	hTechClassKLine = NULL, hTechFirstSelected = NULL;
	UINT nTechUserCount = CTechUser::GetTechUserCount();
	for( UINT i=STT_MIN; i <= STT_MAX+nTechUserCount; i ++ )
	{
		UINT	nTech	=	i;
		if( nTech > STT_MAX )
			nTech	=	i-STT_MAX-1+STT_USER_MIN;

		CString	strClass	=	AfxGetSTTClassName(nTech);
		if( strClass.GetLength() > 0 && 0 != strClass.Compare(strClassLast) )
		{
			hTechClass = m_tree.InsertItem(strClass,IMG_TECHCLASS,IMG_TECHCLASS_SEL);
			m_tree.SetItemData( hTechClass, 0 );
			if( nTech >= STT_KLINE_MIN && nTech <= STT_KLINE_MAX )
				hTechClassKLine	=	hTechClass;
		}

		strClassLast	=	strClass;

		if( NULL == hTechClass )
			hTechClass	=	TVI_ROOT;
		CString	strTechName	=	AfxGetSTTFullName(nTech);
		hTech	=	m_tree.InsertItem(strTechName,IMG_TECH,IMG_TECH_SEL,hTechClass);
		m_tree.SetItemData( hTech, nTech );
		if( m_nSTTFirstSelected == nTech )
			hTechFirstSelected	=	hTech;
	}

	if( hTechFirstSelected )
	{
		m_tree.Expand( m_tree.GetParentItem(hTechFirstSelected), TVE_EXPAND );
		m_tree.Select( hTechFirstSelected, TVGN_CARET );
	}
	else
	{
		m_tree.Expand( hTechClassKLine, TVE_EXPAND );
		HTREEITEM	hSelect	=	m_tree.GetChildItem( hTechClassKLine );
		m_tree.Select( hSelect, TVGN_CARET );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetParamDlg::OnAlldefault() 
{
	// TODO: Add your control notification handler code here
	AfxGetProfile().GetTechParameters().SetDefaultParametersAll( );
	if( m_pParamDlg && ::IsWindow(m_pParamDlg->GetSafeHwnd()) )
	{
		m_pParamDlg->RefreshData( FALSE );
	}
}

void CSetParamDlg::OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CSetParamDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM	hSelItem	=	m_tree.GetSelectedItem();
	if( hSelItem )
	{
		DWORD	dwData	=	m_tree.GetItemData( hSelItem );
		if( !( (dwData >= STT_MIN && dwData <= STT_MAX) || dwData >= STT_USER_MIN ) )
			return;

		CTechParameters	&	param	=	AfxGetProfile().GetTechParameters();
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

		case STT_CYO:		m_pParamDlg	=	new CCYODlg(NULL,&(param.cyo));	m_pParamDlg->Create( CCYODlg::IDD, this );break;
		case STT_DCYO:		m_pParamDlg	=	new CDCYODlg(NULL,&(param.dcyo));	m_pParamDlg->Create( CDCYODlg::IDD, this );break;
		case STT_HSL:		m_pParamDlg	=	new CHSLDlg(NULL,&(param.hsl));	m_pParamDlg->Create( CHSLDlg::IDD, this );break;
		case STT_DPER:		m_pParamDlg	=	new CDPERDlg(NULL,&(param.dper));	m_pParamDlg->Create( CDPERDlg::IDD, this );break;

		default:
			;
		}
		if( m_pParamDlg && ::IsWindow(m_pParamDlg->GetSafeHwnd()) )
			m_pParamDlg->SetWindowPos( NULL, SETPARAM_MAINLEFT, SETPARAM_MAINTOP, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOACTIVATE );
		m_tree.SetFocus();
	}
	
	*pResult = 0;
}

void CSetParamDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( m_pParamDlg && ::IsWindow(m_pParamDlg->GetSafeHwnd()) )
	{
		if( !m_pParamDlg->RefreshData( TRUE ) )
		{
			AfxMessageBox( IDS_SETPARAM_INVALID, MB_OK | MB_ICONINFORMATION );
			return;
		}
		m_pParamDlg->OnCmdMsg( IDOK, 0, NULL, NULL );
		delete	m_pParamDlg;
		m_pParamDlg	=	NULL;
	}

	AfxGetProfile().StoreProfile( );

	CXFDialog::OnOK();
}

