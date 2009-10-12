// AlarmResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\stkui.h"
#include "AlarmResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmResultDlg property page

IMPLEMENT_DYNCREATE(CAlarmResultDlg, CPropertyPageEx)

CAlarmResultDlg::CAlarmResultDlg() : CPropertyPageEx(CAlarmResultDlg::IDD)
{
	//{{AFX_DATA_INIT(CAlarmResultDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAlarmResultDlg::~CAlarmResultDlg()
{
}

void CAlarmResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmResultDlg)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarmResultDlg, CPropertyPageEx)
	//{{AFX_MSG_MAP(CAlarmResultDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmResultDlg message handlers

BOOL CAlarmResultDlg::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	// TODO: Add extra initialization here
	m_list.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
						| LVS_EX_FLATSB | LVS_EX_REGIONAL | LVS_EX_INFOTIP | LVS_EX_UNDERLINEHOT );
	m_list.EnableToolTips( );

	// insert columns
	{
		CString strCode = AfxGetVariantName(SLH_CODE,FALSE);
		CString strDiffPercent = AfxGetVariantName(SLH_DIFFPERCENT,FALSE);
		CString strReason;
		strReason.LoadString( IDS_ALARMRESULT_REASON );

		m_list.InsertColumn( 0, strCode, LVCFMT_CENTER, 90 );
		m_list.InsertColumn( 1, strDiffPercent, LVCFMT_CENTER, 110 );
		m_list.InsertColumn( 2, strReason, LVCFMT_CENTER, 220 );
	}

	CAlarmContainer & container = AfxGetAlarmContainer();
	for( int nIndex=0; nIndex<container.GetSize(); nIndex++ )
	{
		ALARM & alarm = container.ElementAt(nIndex);
		CString strCode = alarm.report.m_szCode;
		CString strDiffPercent = "-";
		if( alarm.report.m_fLast > 1e-4 )
			strDiffPercent.Format( ".2f", alarm.report.m_fNew/alarm.report.m_fLast-1 );
		CString strReason = container.GetDescript( alarm );

		int nItem = m_list.InsertItem( nIndex, strCode );
		m_list.SetItemText( nItem, 0, strCode );
		m_list.SetItemText( nItem, 1, strDiffPercent );
		m_list.SetItemText( nItem, 2, strReason );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
