// TSWnd.cpp: implementation of the CTSWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TSWnd.h"
#include "TSCache.h"

#include "tracetool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTSWnd & CTSWnd::GetInstance()
{
	static CTSWnd s_wndmsg;
	return s_wndmsg;
}

CTSWnd::CTSWnd()
{
	CTSWnd::GetInstance().StartDispatchThread();
}

CTSWnd::~CTSWnd()
{
}

int CTSWnd::AddWnd(HWND hWnd, UINT nMsg)
{
	CSingleLock	lock(&m_mutex, TRUE);

	TSWND_MSG wndmsg;
	wndmsg.hWnd = hWnd;
	wndmsg.nMsg = nMsg;
	return m_aWndMsg.Add(wndmsg);
}

void CTSWnd::RemoveWnd(HWND hWnd)
{
	CSingleLock	lock(&m_mutex, TRUE);

	for (int i=m_aWndMsg.GetSize()-1; i>=0; i--)
	{
		if (m_aWndMsg.ElementAt(i).hWnd == hWnd)
			m_aWndMsg.RemoveAt(i);
	}
}

void CTSWnd::SendMessage(WPARAM wParam, LPARAM lParam)
{
	CTSWndMsgArray		aWndMsgTemp;

	{
		CSingleLock	lock(&m_mutex, TRUE);
		aWndMsgTemp.SetSize(m_aWndMsg.GetSize());

		for (int i=0; i<m_aWndMsg.GetSize(); i++)	
			aWndMsgTemp.SetAt(i, m_aWndMsg.ElementAt(i));
	}

	for (int i=0; i<aWndMsgTemp.GetSize(); i++)
	{
		::SendMessage(aWndMsgTemp.ElementAt(i).hWnd, aWndMsgTemp.ElementAt(i).nMsg,
					wParam, lParam);
	}
}

BOOL CTSWnd::IsEmpty()
{
	CSingleLock	lock(&m_mutex, TRUE);

	return 0 == m_aWndMsg.GetSize();
}

HANDLE CTSWnd::m_hEventKillDispatchThread	= NULL;
HANDLE CTSWnd::m_hEventDispatchThreadKilled	= NULL;

// 分发数据线程
UINT TSDispatchThreadMain(LPVOID pParam)
{
	while(TRUE)
	{
		UINT nMsgType = 0;
		PRCV_DATA pRCV_DATA = NULL;
		if (CTSCache::GetInstance().PopPacket(nMsgType, pRCV_DATA))
		{
			ASSERT(pRCV_DATA && pRCV_DATA->m_pData);
			if (pRCV_DATA && pRCV_DATA->m_pData)
				CTSWnd::GetInstance().SendMessage(nMsgType, (LPARAM)pRCV_DATA);

			char szText[256];
			sprintf(szText, "↑DataType:%d PacketNum:%d", pRCV_DATA->m_wDataType, pRCV_DATA->m_nPacketNum);
			g_pWinTrace->Debug()->Send("TSDispatchThreadMain", szText);

			CTSCache::GetInstance().FreePacket(pRCV_DATA);
		}

		Sleep(1);

		// User wants to quit program
		if (WAIT_OBJECT_0 == WaitForSingleObject(CTSWnd::m_hEventKillDispatchThread,0))
		{
			SetEvent(CTSWnd::m_hEventDispatchThreadKilled);
			AfxEndThread(0, TRUE);
			return 0;
		}
	}
}

void CTSWnd::StartDispatchThread()
{
	m_hEventKillDispatchThread = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEventDispatchThreadKilled = CreateEvent(NULL,FALSE,FALSE,NULL);

	AfxBeginThread(TSDispatchThreadMain, NULL, THREAD_PRIORITY_NORMAL);
}

void CTSWnd::StopDispatchThread()
{
	/* Do not call this function. it does not work well. */
	ASSERT(FALSE);
/*
	// These codes do not work well. system will terminate the thread auto, but cause some memory leaks
	if (m_hEventKillDispatchThread)
		SetEvent(m_hEventKillDispatchThread);

	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEventKillDispatchThread,2000))
	{
		if (m_hEventKillDispatchThread)
			CloseHandle(m_hEventKillDispatchThread);
		if (m_hEventDispatchThreadKilled)
			CloseHandle(m_hEventDispatchThreadKilled);
		m_hEventKillDispatchThread	=	NULL;
		m_hEventDispatchThreadKilled	=	NULL;
	}
*/
}
