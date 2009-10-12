/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		CSPTimeSpan;
		CSPTime;
*/
#if	!defined( __SP_TIME_H__ )
#define	__SP_TIME_H__

#include	<time.h>

/////////////////////////////////////////////////////////////////////////////
// CSPTimeSpan and CSPTime

class STKLIB_API CSPTimeSpan
{
public:

// Constructors
	CSPTimeSpan();
	CSPTimeSpan(time_t time);
	CSPTimeSpan(LONG lDays, int nHours, int nMins, int nSecs);

	CSPTimeSpan(const CSPTimeSpan& timeSpanSrc);
	const CSPTimeSpan& operator=(const CSPTimeSpan& timeSpanSrc);

// Attributes
	// extract parts
	LONG GetDays() const;   // total # of days
	LONG GetTotalHours() const;
	int GetHours() const;
	LONG GetTotalMinutes() const;
	int GetMinutes() const;
	LONG GetTotalSeconds() const;
	int GetSeconds() const;

// Operations
	// time math
	CSPTimeSpan operator-(CSPTimeSpan timeSpan) const;
	CSPTimeSpan operator+(CSPTimeSpan timeSpan) const;
	const CSPTimeSpan& operator+=(CSPTimeSpan timeSpan);
	const CSPTimeSpan& operator-=(CSPTimeSpan timeSpan);
	BOOL operator==(CSPTimeSpan timeSpan) const;
	BOOL operator!=(CSPTimeSpan timeSpan) const;
	BOOL operator<(CSPTimeSpan timeSpan) const;
	BOOL operator>(CSPTimeSpan timeSpan) const;
	BOOL operator<=(CSPTimeSpan timeSpan) const;
	BOOL operator>=(CSPTimeSpan timeSpan) const;

#ifdef _UNICODE
	// for compatibility with MFC 3.x
	CSPString Format(LPCSTR pFormat) const;
#endif
	CSPString Format(LPCTSTR pFormat) const;

	// serialization

private:
	time_t m_timeSpan;
	friend class CSPTime;
};

class STKLIB_API CSPTime
{
public:

// Constructors
	static CSPTime PASCAL GetCurrentTime();
	static DWORD	GetTradeSecondsOfOneDay( DWORD dwMarket = -1/*Ignored Now*/ );
	static DWORD	GetStockTimeNext( DWORD dwDate, int ktype, DWORD dwYear = -1 );
	static time_t	GetTimeTradeLatest( time_t tmTradeFirstToday );
	static double	GetTimeTradeRatioOfOneDay( CSPTime tTradeLatestDay, CSPTime tNow );
	static time_t	GetTradeOffsetToTime( int offset, time_t tmDay);
	static time_t	GetLatestTradeTime( time_t tmNow );
	static BOOL		InTradeTime( time_t tm, int nInflateSeconds = 180 );

	BOOL	FromStockTimeDay( DWORD dwDate );
	BOOL	FromStockTimeMin( DWORD dwDate, DWORD dwYear = -1 );
	BOOL	FromStockTime( DWORD dwDate, BOOL bDayOrMin, DWORD dwYear = -1 );
	DWORD	ToStockTimeDay( );
	DWORD	ToStockTimeMin( );
	DWORD	ToStockTimeSecOrder( DWORD dwStockExchange = -1/*Ignored Now*/ );
	DWORD	ToStockTime( BOOL bDayOrMin );
	
	CSPTime();
	CSPTime(time_t time);
	CSPTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
		int nDST = -1);
	CSPTime(WORD wDosDate, WORD wDosTime, int nDST = -1);
	CSPTime(const CSPTime& timeSrc);

	CSPTime(const SYSTEMTIME& sysTime, int nDST = -1);
	CSPTime(const FILETIME& fileTime, int nDST = -1);
	const CSPTime& operator=(const CSPTime& timeSrc);
	const CSPTime& operator=(time_t t);

// Attributes
	struct tm* GetGmtTm(struct tm* ptm = NULL) const;
	struct tm* GetLocalTm(struct tm* ptm = NULL) const;
	BOOL GetAsSystemTime(SYSTEMTIME& timeDest) const;

	time_t GetTime() const;
	int GetYear() const;
	int GetMonth() const;       // month of year (1 = Jan)
	int GetDay() const;         // day of month
	int GetHour() const;
	int GetMinute() const;
	int GetSecond() const;
	int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat

// Operations
	// time math
	CSPTimeSpan operator-(CSPTime time) const;
	CSPTime operator-(CSPTimeSpan timeSpan) const;
	CSPTime operator+(CSPTimeSpan timeSpan) const;
	const CSPTime& operator+=(CSPTimeSpan timeSpan);
	const CSPTime& operator-=(CSPTimeSpan timeSpan);
	BOOL operator==(CSPTime time) const;
	BOOL operator!=(CSPTime time) const;
	BOOL operator<(CSPTime time) const;
	BOOL operator>(CSPTime time) const;
	BOOL operator<=(CSPTime time) const;
	BOOL operator>=(CSPTime time) const;

	// formatting using "C" strftime
	CSPString Format(LPCTSTR pFormat) const;
	CSPString FormatGmt(LPCTSTR pFormat) const;

#ifdef _UNICODE
	// for compatibility with MFC 3.x
	CSPString Format(LPCSTR pFormat) const;
	CSPString FormatGmt(LPCSTR pFormat) const;
#endif


private:
	time_t m_time;
};


inline	const CSPTimeSpan& CSPTimeSpan::operator=(const CSPTimeSpan& timeSpanSrc)
	{ m_timeSpan = timeSpanSrc.m_timeSpan; return *this; }
inline	LONG CSPTimeSpan::GetDays() const
	{ return m_timeSpan / (24*3600L); }
inline	LONG CSPTimeSpan::GetTotalHours() const
	{ return m_timeSpan/3600; }
inline	int CSPTimeSpan::GetHours() const
	{ return (int)(GetTotalHours() - GetDays()*24); }
inline	LONG CSPTimeSpan::GetTotalMinutes() const
	{ return m_timeSpan/60; }
inline	int CSPTimeSpan::GetMinutes() const
	{ return (int)(GetTotalMinutes() - GetTotalHours()*60); }
inline	LONG CSPTimeSpan::GetTotalSeconds() const
	{ return m_timeSpan; }
inline	int CSPTimeSpan::GetSeconds() const
	{ return (int)(GetTotalSeconds() - GetTotalMinutes()*60); }
inline	CSPTimeSpan CSPTimeSpan::operator-(CSPTimeSpan timeSpan) const
	{ return CSPTimeSpan(m_timeSpan - timeSpan.m_timeSpan); }
inline	CSPTimeSpan CSPTimeSpan::operator+(CSPTimeSpan timeSpan) const
	{ return CSPTimeSpan(m_timeSpan + timeSpan.m_timeSpan); }
inline	const CSPTimeSpan& CSPTimeSpan::operator+=(CSPTimeSpan timeSpan)
	{ m_timeSpan += timeSpan.m_timeSpan; return *this; }
inline	const CSPTimeSpan& CSPTimeSpan::operator-=(CSPTimeSpan timeSpan)
	{ m_timeSpan -= timeSpan.m_timeSpan; return *this; }
inline	BOOL CSPTimeSpan::operator==(CSPTimeSpan timeSpan) const
	{ return m_timeSpan == timeSpan.m_timeSpan; }
inline	BOOL CSPTimeSpan::operator!=(CSPTimeSpan timeSpan) const
	{ return m_timeSpan != timeSpan.m_timeSpan; }
inline	BOOL CSPTimeSpan::operator<(CSPTimeSpan timeSpan) const
	{ return m_timeSpan < timeSpan.m_timeSpan; }
inline	BOOL CSPTimeSpan::operator>(CSPTimeSpan timeSpan) const
	{ return m_timeSpan > timeSpan.m_timeSpan; }
inline	BOOL CSPTimeSpan::operator<=(CSPTimeSpan timeSpan) const
	{ return m_timeSpan <= timeSpan.m_timeSpan; }
inline	BOOL CSPTimeSpan::operator>=(CSPTimeSpan timeSpan) const
	{ return m_timeSpan >= timeSpan.m_timeSpan; }



inline	const CSPTime& CSPTime::operator=(const CSPTime& timeSrc)
	{ m_time = timeSrc.m_time; return *this; }
inline	const CSPTime& CSPTime::operator=(time_t t)
	{ m_time = t; return *this; }
inline	time_t CSPTime::GetTime() const
	{ return m_time; }
inline	int CSPTime::GetYear() const
	{ return (GetLocalTm(NULL)->tm_year) + 1900; }
inline	int CSPTime::GetMonth() const
	{ return GetLocalTm(NULL)->tm_mon + 1; }
inline	int CSPTime::GetDay() const
	{ return GetLocalTm(NULL)->tm_mday; }
inline	int CSPTime::GetHour() const
	{ return GetLocalTm(NULL)->tm_hour; }
inline	int CSPTime::GetMinute() const
	{ return GetLocalTm(NULL)->tm_min; }
inline	int CSPTime::GetSecond() const
	{ return GetLocalTm(NULL)->tm_sec; }
inline	int CSPTime::GetDayOfWeek() const
	{ return GetLocalTm(NULL)->tm_wday + 1; }
inline	CSPTimeSpan CSPTime::operator-(CSPTime time) const
	{ return CSPTimeSpan(m_time - time.m_time); }
inline	CSPTime CSPTime::operator-(CSPTimeSpan timeSpan) const
	{ return CSPTime(m_time - timeSpan.m_timeSpan); }
inline	CSPTime CSPTime::operator+(CSPTimeSpan timeSpan) const
	{ return CSPTime(m_time + timeSpan.m_timeSpan); }
inline	const CSPTime& CSPTime::operator+=(CSPTimeSpan timeSpan)
	{ m_time += timeSpan.m_timeSpan; return *this; }
inline	const CSPTime& CSPTime::operator-=(CSPTimeSpan timeSpan)
	{ m_time -= timeSpan.m_timeSpan; return *this; }
inline	BOOL CSPTime::operator==(CSPTime time) const
	{ return m_time == time.m_time; }
inline	BOOL CSPTime::operator!=(CSPTime time) const
	{ return m_time != time.m_time; }
inline	BOOL CSPTime::operator<(CSPTime time) const
	{ return m_time < time.m_time; }
inline	BOOL CSPTime::operator>(CSPTime time) const
	{ return m_time > time.m_time; }
inline	BOOL CSPTime::operator<=(CSPTime time) const
	{ return m_time <= time.m_time; }
inline	BOOL CSPTime::operator>=(CSPTime time) const
	{ return m_time >= time.m_time; }


#endif	// __SP_TIME_H__
