// MagicRichEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MagicEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMagicDlg dialog


CMagicDlg::CMagicDlg(CWnd* pParent /*=NULL*/)
	: CXFResDialog(CMagicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMagicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CMagicDlg::SetListStrings( CStringArray & astr )
{
	m_nMaxLength	=	0;

	ASSERT( ::IsWindow(m_hWnd) );
	if( ::IsWindow( m_hWnd ) )
	{
		for( int i=0; i<astr.GetSize(); i++ )
		{
			m_listMagic.AddString( astr[i] );
			if( astr[i].GetLength() > m_nMaxLength )
				m_nMaxLength	=	astr[i].GetLength();
		}
		if( m_listMagic.GetCount() > 0 )
			m_listMagic.SetCurSel( 0 );
	}
	m_nMaxLength	+=	1;
}

int CMagicDlg::GetMaxLength( )
{
	return m_nMaxLength;
}

void CMagicDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFResDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMagicDlg)
	DDX_Control(pDX, IDC_XFLISTMAGIC, m_listMagic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMagicDlg, CXFResDialog)
	//{{AFX_MSG_MAP(CMagicDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagicDlg message handlers
BOOL CMagicDlg::OnInitDialog() 
{
	CXFResDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CMagicEdit

CMagicEdit::CMagicEdit()
{
	m_bOpen		=	FALSE;
	m_nMagicBegin	=	0;
}

CMagicEdit::~CMagicEdit()
{
}

void CMagicEdit::SetListStrings( CStringArray & astr )
{
	if( !::IsWindow( m_dlgMagic.m_hWnd ) )
	{
		m_dlgMagic.Create( CMagicDlg::IDD, NULL );
		HideMagicDialog();
	}
	m_dlgMagic.SetListStrings( astr );
}

BEGIN_MESSAGE_MAP(CMagicEdit, CEdit)
	//{{AFX_MSG_MAP(CMagicEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagicEdit message handlers

void CMagicEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bOpen )
	{
		if( VK_RETURN == nChar || VK_SPACE == nChar )
		{
			int		nCurPos;
			nCurPos	=	m_dlgMagic.m_listMagic.GetCurSel();
			if( LB_ERR != nCurPos )
			{
				CString	strWord;
				m_dlgMagic.m_listMagic.GetText( nCurPos, strWord );

				SetSel( -1, 0 );
				CPoint	pt		=	GetCaretPos( );
				int	nCurChar	=	CharFromPos( pt );
				WORD nIndex	=	LOWORD( nCurChar );
				WORD nLine	=	HIWORD( nCurChar );
				if( nIndex >= m_nMagicBegin )
					SetSel( m_nMagicBegin, nIndex );
				ReplaceSel( strWord, TRUE);

				if( VK_SPACE == nChar )
					CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
			HideMagicDialog();
			return;
		}
		else if( isalpha(nChar) || ('_' == nChar) )
		{
			m_strSearch	+=	(unsigned char)nChar;
			int	nFind	=	m_dlgMagic.m_listMagic.FindString( -1, m_strSearch );
			m_dlgMagic.m_listMagic.SetCurSel( nFind );
			if( m_strSearch.GetLength() > m_dlgMagic.GetMaxLength() )
				HideMagicDialog( );
		}
		else if( VK_BACK == nChar && m_strSearch.GetLength() > 1 )
		{
			m_strSearch	=	m_strSearch.Left( m_strSearch.GetLength() -1 );
			int	nFind	=	m_dlgMagic.m_listMagic.FindString( -1, m_strSearch );
			m_dlgMagic.m_listMagic.SetCurSel( nFind );
		}
		else if( VK_ESCAPE == nChar )
		{
			HideMagicDialog();
			return;
		}
	}
	else
	{
		if( ':' == nChar )
		{
			CPoint	pt		=	GetCaretPos( );
			int	nCurChar	=	CharFromPos( pt );
			WORD nIndex	=	LOWORD( nCurChar );
			WORD nLine	=	HIWORD( nCurChar );

			CString	strText;
			GetWindowText( strText );
			if( nIndex > 0 && nIndex-1 < strText.GetLength() && strText[nIndex-1] == ':' )
			{
				m_nMagicBegin	=	nIndex+1;
				CEdit::OnChar(nChar, nRepCnt, nFlags);
				ShowMagicDialog();
				SetFocus( );
				return;
			}
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CMagicEdit::ShowMagicDialog( )
{
	if( !::IsWindow( m_dlgMagic.m_hWnd ) )
	{
		m_dlgMagic.Create( CMagicDlg::IDD, NULL );
	}
	CPoint	pt	=	GetCaretPos( );
	ClientToScreen( &pt );

	CFont	* pFont	=	GetFont( );
	LOGFONT	logfont;
	if( pFont && pFont->GetLogFont( &logfont ) && 0 != logfont.lfHeight )
		pt.y	+=	abs(logfont.lfHeight);
	else
		pt.y	+=	12;

	m_dlgMagic.SetWindowPos( &wndTopMost, pt.x, pt.y, 0, 0, 
		SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE );
	SetFocus();
	m_bOpen	=	TRUE;
}

void CMagicEdit::HideMagicDialog( )
{
	if( ::IsWindow( m_dlgMagic.m_hWnd ) )
	{
		m_dlgMagic.ShowWindow( SW_HIDE );
		m_dlgMagic.m_listMagic.SetCurSel( 0 );
		m_bOpen	=	FALSE;
		m_strSearch	=	"";
	}
}

void CMagicEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bOpen )
	{
		int		nCurPos;
		nCurPos	=	m_dlgMagic.m_listMagic.GetCurSel();
		switch( nChar )
		{
		case	VK_UP:
		case	VK_DOWN:
		case	VK_NEXT:
		case	VK_PRIOR:
			if( LB_ERR != nCurPos )
			{
				if( VK_UP == nChar )
					nCurPos--;
				else if( VK_DOWN == nChar )
					nCurPos++;
				else if( VK_NEXT == nChar )
					nCurPos	+=	8;
				else if( VK_PRIOR == nChar )
					nCurPos	-=	8;

				int	nCount = m_dlgMagic.m_listMagic.GetCount();
				if( nCurPos < 0 )
					nCurPos = 0;
				else if ( nCurPos >= nCount )
					nCurPos	=	nCount - 1;
				m_dlgMagic.m_listMagic.SetCurSel( nCurPos );
			}
			else
			{
				int	nCount = m_dlgMagic.m_listMagic.GetCount();
				if( nCount > 0 )
					m_dlgMagic.m_listMagic.SetCurSel( 0 );
			}
			return;
		case	VK_ESCAPE:
			HideMagicDialog();
			return;
		case	VK_LEFT:
		case	VK_RIGHT:
		case	VK_HOME:
		case	VK_END:
			HideMagicDialog();
			break;
		}
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMagicEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(	WM_KEYFIRST == pMsg->message
		|| WM_KEYDOWN == pMsg->message
		|| WM_KEYUP == pMsg->message
		|| WM_CHAR == pMsg->message
		|| WM_DEADCHAR == pMsg->message
		|| WM_SYSKEYDOWN == pMsg->message
		|| WM_SYSKEYUP == pMsg->message
		|| WM_SYSCHAR == pMsg->message
		|| WM_SYSDEADCHAR == pMsg->message
		|| WM_KEYLAST == pMsg->message )
	{
		if( VK_ESCAPE == pMsg->wParam && m_bOpen )
		{
			HideMagicDialog( );
			return TRUE;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}
