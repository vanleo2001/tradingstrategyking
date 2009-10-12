// TSWnd.h: interface for the CTSWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSWND_H__C4D14693_930D_4D76_AF53_A411A50D8D63__INCLUDED_)
#define AFX_TSWND_H__C4D14693_930D_4D76_AF53_A411A50D8D63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTSWnd  
{
public:
	CTSWnd();
	virtual ~CTSWnd();

	static CTSWnd & GetInstance( );

	int AddWnd( HWND hWnd, UINT nMsg );
	void RemoveWnd( HWND hWnd );
	void SendMessage( WPARAM wParam, LPARAM lParam );
	BOOL IsEmpty();

	static HANDLE m_hEventKillDispatchThread;
	static HANDLE m_hEventDispatchThreadKilled;

	void StartDispatchThread( );
	void StopDispatchThread( );

protected:
	typedef struct tswnd_msg_t {
		HWND	hWnd;
		UINT	nMsg;
	}TSWND_MSG;
	typedef CArray<TSWND_MSG,TSWND_MSG &> CTSWndMsgArray;
	CTSWndMsgArray		m_aWndMsg;

	CCriticalSection	m_mutex;
};

#endif // !defined(AFX_TSWND_H__C4D14693_930D_4D76_AF53_A411A50D8D63__INCLUDED_)
