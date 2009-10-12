#if !defined(AFX_SETPARAMDLG_H__38801D90_E6EA_429D_8550_63495F77F6C0__INCLUDED_)
#define AFX_SETPARAMDLG_H__38801D90_E6EA_429D_8550_63495F77F6C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTechComboBox
class CTechComboBox : public CComboBox
{
public:
	BOOL	Initialize( );
	BOOL	Select( int nTech );
	int		GetSelect( );
};

/////////////////////////////////////////////////////////////////////////////
// CKTypeComboBox
class CKTypeComboBox : public CComboBox
{
public:
	BOOL	Initialize( );
	BOOL	InitializeDayMin5( );
	BOOL	InitializeDay( );
	BOOL	InitializeWeek( );
	BOOL	Select( int nKType );
	int		GetSelect( );
};

/////////////////////////////////////////////////////////////////////////////
// CIntensityComboBox
class CIntensityComboBox : public CComboBox
{
public:
	BOOL	InitializeGate( );
	BOOL	Initialize( );
	BOOL	Select( UINT nITS );
	BOOL	ResetAndSelect( UINT nITS );
	int		GetSelect( );
};

/////////////////////////////////////////////////////////////////////////////
// CParamDlg dialog
class CParamDlg : public CXFDialog
{
public:
	CParamDlg( )	{	};
	CParamDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL)
	{
		CXFDialog::CXFDialog( nIDTemplate, pParentWnd );
	}

	CButton	m_btnParamHelp;
	CButton	m_btnDefault;

	virtual	BOOL	RefreshData( BOOL bSaveAndValidate )/*	=	0*/;

protected:
	virtual	void	DoDataExchange(CDataExchange* pDX);
};

void ParamHelp( UINT nTech );

#include "MADlg.h"
#include "BBIDlg.h"
#include "BOLLDlg.h"
#include "PVDlg.h"
#include "SARDlg.h"
#include "DJDlg.h"
#include "CWDlg.h"
#include "MACDDlg.h"
#include "MIKEDlg.h"
#include "PSYDlg.h"
#include "VOLUMEDlg.h"
#include "NVIDlg.h"
#include "PVIDlg.h"
#include "VRDlg.h"
#include "VROCDlg.h"
#include "OBVDlg.h"
#include "MOBVDlg.h"
#include "MFIDlg.h"
#include "VMACDDlg.h"
#include "WVADDlg.h"
#include "EMVDlg.h"
#include "VRSIDlg.h"
#include "NVRSIDlg.h"
#include "ADDlg.h"
#include "CIDlg.h"
#include "KDJDlg.h"
#include "RDlg.h"
#include "RSIDlg.h"
#include "BIASDlg.h"
#include "MTMDlg.h"
#include "DMIDlg.h"
#include "ROCDlg.h"
#include "CCIDlg.h"
#include "CVDlg.h"
#include "ARBRDlg.h"
#include "CRDlg.h"
#include "OSCDlg.h"
#include "UOSDlg.h"
#include "MAOSCDlg.h"
#include "36BIASDlg.h"
#include "DPODlg.h"
#include "KSTDlg.h"
#include "REIDlg.h"
#include "DMKIDlg.h"
#include "PCNTDlg.h"
#include "HLCDlg.h"
#include "CDPDlg.h"
#include "ASIDlg.h"
#include "ATRDlg.h"
#include "CYODlg.h"
#include "DCYODlg.h"
#include "HSLDlg.h"
#include "DPERDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSetParamDlg dialog

class CSetParamDlg : public CXFDialog
{
// Construction
public:
	CSetParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetParamDlg();

	void	SetFirstSelected( UINT nSTT );

// Dialog Data
	//{{AFX_DATA(CSetParamDlg)
	enum { IDD = IDD_SETPARAM };
	CButton	m_btnAllDefault;
	CButton	m_btnOK;
	CTreeCtrl	m_tree;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAlldefault();
	virtual void OnOK();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CParamDlg	*	m_pParamDlg;
	CImageList		m_ImageList;
	UINT			m_nSTTFirstSelected;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPARAMDLG_H__38801D90_E6EA_429D_8550_63495F77F6C0__INCLUDED_)
