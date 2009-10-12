// Simulation.h: interface for the CSimulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMULATION_H__52C07D2D_A43C_4021_B926_3A8FE98354AF__INCLUDED_)
#define AFX_SIMULATION_H__52C07D2D_A43C_4021_B926_3A8FE98354AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////
// CSimulation

class	CSimulation;

typedef	struct simulation_info_t	{
	HWND				hMainWnd;
	CStrategy	*		pStrategy;
} SIMULATION_INFO, * LPSIMULATION_INFO;
/***
	策略的模拟线程控制类
*/
class CSimulation  
{
public:
	CSimulation();
	virtual ~CSimulation();

	static HANDLE m_hEventKillSimulationThread;
	static HANDLE m_hEventSimulationThreadKilled;

	void	SetStrategy( CStrategy * pStrategy, HWND hMainWnd );
	BOOL	DownloadDataIfNeed( );
	void	Restart( );
	void	Pause( );
	void	Continue( );
	void	Stop();
	void	OnEnd( BOOL bFinished );

protected:
	SIMULATION_INFO *	m_pSimulationInfo;
	CStrategy		*	m_pStrategy;
	HWND				m_hMainWnd;
	BOOL				m_bStopAndReset;
};

#endif // !defined(AFX_SIMULATION_H__52C07D2D_A43C_4021_B926_3A8FE98354AF__INCLUDED_)
