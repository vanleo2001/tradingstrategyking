// SimuStatDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SimuStatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuStatDlg dialog


CSimuStatDlg::CSimuStatDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CSimuStatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimuStatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStrategy	=	NULL;
}

void CSimuStatDlg::SetStrategy( CStrategy * pStrategy )
{
	m_pStrategy	=	pStrategy;
}

/***
	设定策略结果统计信息
*/
BOOL CSimuStatDlg::SetResults( )
{
	if( NULL == m_pStrategy )
		return FALSE;

	CString	strBuyTimes, strVictoryTimes, strYieldMax, strYieldMin, strYieldFin, strYieldIndex, strYieldStdDev, strXiapu;

	int nTimes = 0, nVictoryTimes = 0;
	double	dYieldMax = 0, dYieldMin = 0;
	COpRecordContainer & container = m_pStrategy->RealGetOpRecord();
	container.StatResults( &nTimes, &nVictoryTimes, &dYieldMax, &dYieldMin );

	strBuyTimes.Format( "%u", nTimes );
	m_editBuyTimes.SetWindowText( strBuyTimes );

	strVictoryTimes.Format( "%u", nVictoryTimes );
	m_editVictoryTimes.SetWindowText( strVictoryTimes );

	strYieldMax.Format( "%.2f%%", dYieldMax*100 );
	m_editYieldMax.SetWindowText( strYieldMax );

	strYieldMin.Format( "%.2f%%", dYieldMin*100 );
	m_editYieldMin.SetWindowText( strYieldMin );

	strYieldFin.Format( "%.2f%%", m_pStrategy->RealGetCurrentYieldPercent()*100 );
	m_editYieldFin.SetWindowText( strYieldFin );

	strYieldIndex.Format( "%.2f%%", m_pStrategy->RealGetCurrentYieldIndexPercent()*100 );
	m_editYieldIndex.SetWindowText( strYieldIndex );

	double	dStdDev = 0, dXiapu = 0;
	CAssetSerialContainer & assetserial = m_pStrategy->RealGetAssetSerial();
	if( assetserial.GetStdDev( &dStdDev, &dXiapu ) )
	{
		strYieldStdDev.Format( "%.2f%%", dStdDev*100 );
		strXiapu.Format( "%.2f%%", dXiapu*100 );
	}
	m_editYieldStdDev.SetWindowText( strYieldStdDev );
	m_editXiapu.SetWindowText( strXiapu );

	return TRUE;
}

void CSimuStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimuStatDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_XIAPU, m_editXiapu);
	DDX_Control(pDX, IDC_YIELDSTDDEV, m_editYieldStdDev);
	DDX_Control(pDX, IDC_YIELDINDEX, m_editYieldIndex);
	DDX_Control(pDX, IDC_YIELDFIN, m_editYieldFin);
	DDX_Control(pDX, IDC_YIELDMIN, m_editYieldMin);
	DDX_Control(pDX, IDC_YIELDMAX, m_editYieldMax);
	DDX_Control(pDX, IDC_VICTORYTIMES, m_editVictoryTimes);
	DDX_Control(pDX, IDC_BUYTIMES, m_editBuyTimes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimuStatDlg, CXFDialog)
	//{{AFX_MSG_MAP(CSimuStatDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuStatDlg message handlers

BOOL CSimuStatDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	ASSERT( m_pStrategy );
	SetResults( );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
