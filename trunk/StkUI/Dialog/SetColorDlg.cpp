// SetColorDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SetColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetColorDlg dialog

void OnColorChanged( )
{
	::PostMessage( AfxGetStrategyView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
	::PostMessage( AfxGetGroupView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
	::PostMessage( AfxGetTechsView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
	::PostMessage( AfxGetSimuView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
	::PostMessage( AfxGetSListView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
	::PostMessage( AfxGetGraphView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );	
	::PostMessage( AfxGetRealTimeView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
	::PostMessage( AfxGetMultiSortView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
	::PostMessage( AfxGetBaseView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
	::PostMessage( AfxGetSelectorView()->GetSafeHwnd(), WM_USER_COLORCHANGE, 0, 0 );
}

CSetColorDlg::CSetColorDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSetColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSetColorDlg::LoadFromColorClass( CColorClass & cls )
{
	CColorClass	clsDefault;
	if( CColorClass::IsColorStandard( cls.GetName() ) )
		clsDefault.SetColorStandard( );
	else if( CColorClass::IsColorByzantinesque( cls.GetName() ) )
		clsDefault.SetColorByzantinesque( );
	else if( CColorClass::IsColorGothic( cls.GetName() ) )
		clsDefault.SetColorGothic( );
	else if( CColorClass::IsColorBaroque( cls.GetName() ) )
		clsDefault.SetColorBaroque( );
	else
		clsDefault.SetColorDefault( );

	/*
	m_btnWorkspBK.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrWorkspBK );
	m_btnWorkspBK.m_clrSelected	=	cls.GetColor( CColorClass::clrWorkspBK );
	m_btnWorkspBK.Invalidate( );

	m_btnWorkspText.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrWorkspText );
	m_btnWorkspText.m_clrSelected	=	cls.GetColor( CColorClass::clrWorkspText );
	m_btnWorkspText.Invalidate( );

	m_btnSimuBK.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrSimuBK );
	m_btnSimuBK.m_clrSelected	=	cls.GetColor( CColorClass::clrSimuBK );
	m_btnSimuBK.Invalidate( );

	m_btnSimuText.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrSimuText );
	m_btnSimuText.m_clrSelected	=	cls.GetColor( CColorClass::clrSimuText );
	m_btnSimuText.Invalidate( );

	m_btnSListBK.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrSListBK );
	m_btnSListBK.m_clrSelected	=	cls.GetColor( CColorClass::clrSListBK );
	m_btnSListBK.Invalidate( );

	m_btnSListSelected.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrSListSelected );
	m_btnSListSelected.m_clrSelected	=	cls.GetColor( CColorClass::clrSListSelected );
	m_btnSListSelected.Invalidate( );

	m_btnBaseBK.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrBaseBK );
	m_btnBaseBK.m_clrSelected	=	cls.GetColor( CColorClass::clrBaseBK );
	m_btnBaseBK.Invalidate( );

	m_btnBaseText.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrBaseText );
	m_btnBaseText.m_clrSelected	=	cls.GetColor( CColorClass::clrBaseText );
	m_btnBaseText.Invalidate( );

	m_btnGraphBK.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrGraphBK );
	m_btnGraphBK.m_clrSelected	=	cls.GetColor( CColorClass::clrGraphBK );
	m_btnGraphBK.Invalidate( );

	m_btnBorder.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrBorder );
	m_btnBorder.m_clrSelected	=	cls.GetColor( CColorClass::clrBorder );
	m_btnBorder.Invalidate( );

	m_btnRise.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrRise );
	m_btnRise.m_clrSelected	=	cls.GetColor( CColorClass::clrRise );
	m_btnRise.Invalidate( );

	m_btnFall.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrFall );
	m_btnFall.m_clrSelected	=	cls.GetColor( CColorClass::clrFall );
	m_btnFall.Invalidate( );

	m_btnFallEntity.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrFallEntity );
	m_btnFallEntity.m_clrSelected	=	cls.GetColor( CColorClass::clrFallEntity );
	m_btnFallEntity.Invalidate( );

	m_btnPlane.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrPlane );
	m_btnPlane.m_clrSelected	=	cls.GetColor( CColorClass::clrPlane );
	m_btnPlane.Invalidate( );

	m_btnNewKLine.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrNewKLine );
	m_btnNewKLine.m_clrSelected	=	cls.GetColor( CColorClass::clrNewKLine );
	m_btnNewKLine.Invalidate( );

	m_btnCW.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrCW );
	m_btnCW.m_clrSelected	=	cls.GetColor( CColorClass::clrCW );
	m_btnCW.Invalidate( );

	m_btnDJ.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrDJ );
	m_btnDJ.m_clrSelected	=	cls.GetColor( CColorClass::clrDJ );
	m_btnDJ.Invalidate( );

	m_btnTitle.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrTitle );
	m_btnTitle.m_clrSelected	=	cls.GetColor( CColorClass::clrTitle );
	m_btnTitle.Invalidate( );

	m_btnText.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrText );
	m_btnText.m_clrSelected	=	cls.GetColor( CColorClass::clrText );
	m_btnText.Invalidate( );

	m_btnLine1.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrLine1 );
	m_btnLine1.m_clrSelected	=	cls.GetColor( CColorClass::clrLine1 );
	m_btnLine1.Invalidate( );

	m_btnLine2.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrLine2 );
	m_btnLine2.m_clrSelected	=	cls.GetColor( CColorClass::clrLine2 );
	m_btnLine2.Invalidate( );

	m_btnLine3.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrLine3 );
	m_btnLine3.m_clrSelected	=	cls.GetColor( CColorClass::clrLine3 );
	m_btnLine3.Invalidate( );

	m_btnLine4.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrLine4 );
	m_btnLine4.m_clrSelected	=	cls.GetColor( CColorClass::clrLine4 );
	m_btnLine4.Invalidate( );

	m_btnLine5.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrLine5 );
	m_btnLine5.m_clrSelected	=	cls.GetColor( CColorClass::clrLine5 );
	m_btnLine5.Invalidate( );

	m_btnLine6.m_clrDefault	=	clsDefault.GetColor( CColorClass::clrLine6 );
	m_btnLine6.m_clrSelected	=	cls.GetColor( CColorClass::clrLine6 );
	m_btnLine6.Invalidate( );
	*/
}

void CSetColorDlg::StoreToColorClass( CColorClass & cls )
{
	/*
	cls.SetColor( CColorClass::clrWorkspBK, m_btnWorkspBK.m_clrSelected );
	cls.SetColor( CColorClass::clrWorkspText, m_btnWorkspText.m_clrSelected );
	cls.SetColor( CColorClass::clrSimuBK, m_btnSimuBK.m_clrSelected );
	cls.SetColor( CColorClass::clrSimuText, m_btnSimuText.m_clrSelected );
	cls.SetColor( CColorClass::clrSListBK, m_btnSListBK.m_clrSelected );

	cls.SetColor( CColorClass::clrSListSelected, m_btnSListSelected.m_clrSelected );
	cls.SetColor( CColorClass::clrBaseBK, m_btnBaseBK.m_clrSelected );
	cls.SetColor( CColorClass::clrBaseText, m_btnBaseText.m_clrSelected );
	cls.SetColor( CColorClass::clrGraphBK, m_btnGraphBK.m_clrSelected );
	cls.SetColor( CColorClass::clrBorder, m_btnBorder.m_clrSelected );

	cls.SetColor( CColorClass::clrRise, m_btnRise.m_clrSelected );
	cls.SetColor( CColorClass::clrFall, m_btnFall.m_clrSelected );
	cls.SetColor( CColorClass::clrFallEntity, m_btnFallEntity.m_clrSelected );
	cls.SetColor( CColorClass::clrPlane, m_btnPlane.m_clrSelected );
	cls.SetColor( CColorClass::clrNewKLine, m_btnNewKLine.m_clrSelected );

	cls.SetColor( CColorClass::clrCW, m_btnCW.m_clrSelected );
	cls.SetColor( CColorClass::clrDJ, m_btnDJ.m_clrSelected );
	cls.SetColor( CColorClass::clrTitle, m_btnTitle.m_clrSelected );
	cls.SetColor( CColorClass::clrText, m_btnText.m_clrSelected );
	cls.SetColor( CColorClass::clrLine1, m_btnLine1.m_clrSelected );
	
	cls.SetColor( CColorClass::clrLine2, m_btnLine2.m_clrSelected );
	cls.SetColor( CColorClass::clrLine3, m_btnLine3.m_clrSelected );
	cls.SetColor( CColorClass::clrLine4, m_btnLine4.m_clrSelected );
	cls.SetColor( CColorClass::clrLine5, m_btnLine5.m_clrSelected );
	cls.SetColor( CColorClass::clrLine6, m_btnLine6.m_clrSelected );
	*/
}

void CSetColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetColorDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_PREVIEW, m_btnPreview);
	DDX_Control(pDX, IDC_SAVEAS, m_btnSaveAs);
	DDX_Control(pDX, IDC_LINE6, m_btnLine6);
	DDX_Control(pDX, IDC_LINE5, m_btnLine5);
	DDX_Control(pDX, IDC_LINE4, m_btnLine4);
	DDX_Control(pDX, IDC_LINE3, m_btnLine3);
	DDX_Control(pDX, IDC_LINE2, m_btnLine2);
	DDX_Control(pDX, IDC_LINE1, m_btnLine1);
	DDX_Control(pDX, IDC_TEXT, m_btnText);
	DDX_Control(pDX, IDC_TITLE, m_btnTitle);
	DDX_Control(pDX, IDC_DJ, m_btnDJ);
	DDX_Control(pDX, IDC_CW, m_btnCW);
	DDX_Control(pDX, IDC_NEWKLINE, m_btnNewKLine);
	DDX_Control(pDX, IDC_PLANE, m_btnPlane);
	DDX_Control(pDX, IDC_FALLENTITY, m_btnFallEntity);
	DDX_Control(pDX, IDC_FALL, m_btnFall);
	DDX_Control(pDX, IDC_RISE, m_btnRise);
	DDX_Control(pDX, IDC_BORDER, m_btnBorder);
	DDX_Control(pDX, IDC_GRAPHBK, m_btnGraphBK);
	DDX_Control(pDX, IDC_BASETEXT, m_btnBaseText);
	DDX_Control(pDX, IDC_BASEBK, m_btnBaseBK);
	DDX_Control(pDX, IDC_SLISTSELECTED, m_btnSListSelected);
	DDX_Control(pDX, IDC_SLISTBK, m_btnSListBK);
	DDX_Control(pDX, IDC_SIMUTEXT, m_btnSimuText);
	DDX_Control(pDX, IDC_SIMUBK, m_btnSimuBK);
	DDX_Control(pDX, IDC_WORKSPTEXT, m_btnWorkspText);
	DDX_Control(pDX, IDC_WORKSPBK, m_btnWorkspBK);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_COLORCLASS, m_comboColorClass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetColorDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSetColorDlg)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_BN_CLICKED(IDC_SAVEAS, OnSaveas)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_CBN_SELCHANGE(IDC_COLORCLASS, OnSelchangeColorclass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetColorDlg message handlers

BOOL CSetColorDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CColorClassContainer & container = AfxGetProfile().GetColorClassContainer( );
	for( int i=0; i<container.GetSize(); i++ )
	{
		CColorClass & clr	=	container.ElementAt(i);
		m_comboColorClass.AddString( clr.GetName() );
	}

	m_comboColorClass.SetCurSel( container.GetSelected() );
	OnSelchangeColorclass();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetColorDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	CColorClassContainer & container = AfxGetProfile().GetColorClassContainer( );

	CString	strName;
	m_comboColorClass.GetWindowText( strName );
	
	CColorClass cls;
	cls.SetColorDefault();
	cls.SetName( strName );
	StoreToColorClass( cls );
	container.SetColorClass( cls );
	container.SelectColorClass( strName );
	
	CXFDialog::OnOK();

	AfxGetProfile().StoreProfile( );
	OnColorChanged( );
}

void CSetColorDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData();

	CColorClassContainer & container = AfxGetProfile().GetColorClassContainer( );
	container.SetColorClass( m_clsOriginal );
	
	CXFDialog::OnCancel();

	AfxGetProfile().StoreProfile( );
	OnColorChanged( );
}

void CSetColorDlg::OnPreview() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CColorClassContainer & container = AfxGetProfile().GetColorClassContainer( );

	CString	strName;
	m_comboColorClass.GetWindowText( strName );
	
	CColorClass cls;
	cls.SetColorDefault();
	cls.SetName( strName );
	StoreToColorClass( cls );
	container.SetColorClass( cls );
	container.SelectColorClass( strName );

	// Preview
	OnColorChanged( );
}

void CSetColorDlg::OnSaveas() 
{
	// TODO: Add your control notification handler code here
	CColorClassContainer & container = AfxGetProfile().GetColorClassContainer( );
	CColorClass	cls;
	cls.SetColorDefault();
	StoreToColorClass( cls );

	while( TRUE )
	{
		CStringDialog	dlg( this, IDS_SETCOLOR_SAVEASTITLE, IDS_SETCOLOR_SAVEASTITLE, NULL );
		if( IDOK != dlg.DoModal() )
			break;

		cls.SetName( dlg.GetStringWant() );
		if( -1 == container.AddColorClass( cls ) )
		{
			AfxMessageBox( IDS_SETCOLOR_COLORCLASSEXIST, MB_OK|MB_ICONINFORMATION );
			continue;
		}
		m_comboColorClass.AddString( cls.GetName() );
		m_comboColorClass.SelectString( 0, cls.GetName() );
		OnSelchangeColorclass( );
		break;
	}
}

void CSetColorDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	CString	strName;
	m_comboColorClass.GetWindowText( strName );
	CColorClassContainer & container = AfxGetProfile().GetColorClassContainer( );
	if( CColorClass::IsSysColorClass(strName) || m_comboColorClass.GetCount() <= 1 )
	{
		AfxMessageBox( IDS_SETCOLOR_CANNOTDELETESYS, MB_OK|MB_ICONINFORMATION );
		return;
	}

	if( IDYES == AfxMessageBox( IDS_SETCOLOR_DELETECONFIRM, MB_YESNO ) )
	{
		if( container.RemoveColorClass( strName ) )
		{
			int	nSel	=	m_comboColorClass.GetCurSel();
			m_comboColorClass.DeleteString( nSel );
			if( nSel >= m_comboColorClass.GetCount() )
				nSel	=	0;
			m_strNameCurrent.Empty( );
			m_comboColorClass.SetCurSel( nSel );
			OnSelchangeColorclass( );
		}
	}
}

void CSetColorDlg::OnSelchangeColorclass() 
{
	// TODO: Add your control notification handler code here
	CColorClassContainer & container = AfxGetProfile().GetColorClassContainer( );

	if( m_strNameCurrent.GetLength() > 0 )
	{
		CColorClass cls;
		cls.SetColorDefault();
		cls.SetName( m_strNameCurrent );
		StoreToColorClass( cls );
		VERIFY( container.SetColorClass( cls ) );
	}
	
	CString	strNameNow;
	m_comboColorClass.GetWindowText( strNameNow );
	
	CColorClass	clsNow;
	if( container.GetColorClass( strNameNow, clsNow ) )
	{
		LoadFromColorClass( clsNow );
		m_clsOriginal	=	clsNow;
		m_strNameCurrent	=	strNameNow;
		m_btnDelete.EnableWindow( !CColorClass::IsSysColorClass( strNameNow ) );
	}
}

