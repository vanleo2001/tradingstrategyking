/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Alarm functions.
*/

#if	!defined( __STKLIB_ALARM_H__ )
#define	__STKLIB_ALARM_H__

#pragma pack(1)

#define ALARM_TYPE_DIFFPERCENTMORE	1
#define ALARM_TYPE_DIFFPERCENTLESS	2
#define ALARM_TYPE_TRADEVOLUME		3

typedef struct _alarm_cond_t {
	DWORD	m_type;
	DWORD	m_dwMarket;
	char	m_szCode[STKLIB_MAX_CODE2];
	float	m_fValue;
	char	m_szReserved[32];
}ALARMCOND;

typedef CSPArray<ALARMCOND, ALARMCOND &>	CAlarmCondArray;

class STKLIB_API CAlarmCondContainer : public CAlarmCondArray
{
public:
	virtual ~CAlarmCondContainer()	{	RemoveAll();	}

	// for access CAlarmCondArray
	virtual	void	RemoveAll( )
	{
		CSPMutex::Scoped	l(m_mutex);
		CAlarmCondArray::RemoveAll();
	}

	virtual	int Add( ALARMCOND & newElement)
	{
		CSPMutex::Scoped	l(m_mutex);
		return CAlarmCondArray::Add(newElement);
	}
	virtual	void RemoveAt(int nIndex, int nCount = 1)
	{
		CSPMutex::Scoped	l(m_mutex);
		CAlarmCondArray::RemoveAt(nIndex,nCount);
	}

	// extra
	static CSPString	AlarmCondToString( ALARMCOND & cond );

protected:
	CSPMutex	m_mutex;
};

typedef struct _alarm_t {
	REPORT		report;
	ALARMCOND	cond;
}ALARM;

typedef CSPArray<ALARM, ALARM &>	CAlarmArray;

class STKLIB_API CAlarmContainer : public CAlarmArray
{
public:
	static CSPString	GetDescript( ALARM & alarm );

	enum { MaxAlarms = 1024 };

	virtual ~CAlarmContainer()	{	RemoveAll();	}

	// for access CAlarmArray
	virtual	void	RemoveAll( )
	{	CAlarmArray::RemoveAll();	}

	virtual	int Add( ALARM & newElement)
	{
		CSPMutex::Scoped	l(m_mutex);
		if( GetSize()>=MaxAlarms ) return -1;
		return CAlarmArray::Add(newElement);
	}
	virtual	void RemoveAt(int nIndex, int nCount = 1)
	{
		CSPMutex::Scoped	l(m_mutex);
		CAlarmArray::RemoveAt(nIndex,nCount);
	}

	// process
	BOOL	HasWarning( )	{	return GetSize() > 0; }
	int		AddAlarm( ALARMCOND * pcond, REPORT * pReport, REPORT * pReportLast );
	BOOL	OnReceiveReport( CStockInfo * pInfo, REPORT * pReport, REPORT * pReportLast );

protected:
	CSPMutex	m_mutex;
};

STKLIB_API CAlarmContainer & AfxGetAlarmContainer();

#pragma pack()

#endif	// __STKLIB_ALARM_H__

