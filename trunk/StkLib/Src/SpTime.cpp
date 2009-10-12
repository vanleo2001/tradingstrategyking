// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include	"stdafx.h"
#include	"../Include/SpTime.h"
#include	<TCHAR.H>

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPTimeSpan
CSPTimeSpan::CSPTimeSpan()
	{ }
CSPTimeSpan::CSPTimeSpan(time_t time)
	{ m_timeSpan = time; }
CSPTimeSpan::CSPTimeSpan(LONG lDays, int nHours, int nMins, int nSecs)
	{ m_timeSpan = nSecs + 60* (nMins + 60* (nHours + 24* lDays)); }
CSPTimeSpan::CSPTimeSpan(const CSPTimeSpan& timeSpanSrc)
	{ m_timeSpan = timeSpanSrc.m_timeSpan; }

/////////////////////////////////////////////////////////////////////////////
// CSPTime - absolute time

CSPTime::CSPTime()
	{ }
CSPTime::CSPTime(time_t time)
	{ m_time = time; }
CSPTime::CSPTime(const CSPTime& timeSrc)
	{ m_time = timeSrc.m_time; }

CSPTime::CSPTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
	int nDST)
{
	struct tm atm;
	atm.tm_sec = nSec;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	SP_ASSERT(nDay >= 1 && nDay <= 31);
	atm.tm_mday = nDay;
	SP_ASSERT(nMonth >= 1 && nMonth <= 12);
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	SP_ASSERT(nYear >= 1900);
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = nDST;
	m_time = mktime(&atm);
	SP_ASSERT(m_time != -1);       // indicates an illegal input time
	if( m_time == -1 )
		m_time	=	::time(NULL);
}

CSPTime::CSPTime(WORD wDosDate, WORD wDosTime, int nDST)
{
	struct tm atm;
	atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
	atm.tm_min = (wDosTime & ~0xF800) >> 5;
	atm.tm_hour = wDosTime >> 11;

	atm.tm_mday = wDosDate & ~0xFFE0;
	atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
	atm.tm_year = (wDosDate >> 9) + 80;
	atm.tm_isdst = nDST;
	m_time = mktime(&atm);
	SP_ASSERT(m_time != -1);       // indicates an illegal input time
	if( m_time == -1 )
		m_time	=	::time(NULL);
}

CSPTime::CSPTime(const SYSTEMTIME& sysTime, int nDST)
{
	if (sysTime.wYear < 1900)
	{
		time_t time0 = 0L;
		CSPTime timeT(time0);
		*this = timeT;
	}
	else
	{
		CSPTime timeT(
			(int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
			(int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond,
			nDST);
		*this = timeT;
	}
}

CSPTime::CSPTime(const FILETIME& fileTime, int nDST)
{
	// first convert file time (UTC time) to local time
	FILETIME localTime;
	if (!FileTimeToLocalFileTime(&fileTime, &localTime))
	{
		m_time = 0;
		return;
	}

	// then convert that time to system time
	SYSTEMTIME sysTime;
	if (!FileTimeToSystemTime(&localTime, &sysTime))
	{
		m_time = 0;
		return;
	}

	// then convert the system time to a time_t (C-runtime local time)
	CSPTime timeT(sysTime, nDST);
	*this = timeT;
}

CSPTime PASCAL CSPTime::GetCurrentTime()
// return the current system time
{
	return CSPTime(::time(NULL));
}

DWORD CSPTime::GetTradeSecondsOfOneDay( DWORD dwMarket )
{
	return 14400;
}

DWORD CSPTime::GetStockTimeNext( DWORD dwDate, int ktype, DWORD dwYear )
{
	CSPTime	sptime;
	if( ktype == CKData::ktypeDay )
	{
		if( !sptime.FromStockTimeDay( dwDate ) )
			return -1;
		if( 6 == sptime.GetDayOfWeek() )	//	Friday
			sptime	+=	CSPTimeSpan(3,0,0,0);
		else
			sptime	+=	CSPTimeSpan(1,0,0,0);
		return sptime.ToStockTimeDay();
	}
	else if( ktype == CKData::ktypeWeek )
	{
		if( !sptime.FromStockTimeDay( dwDate ) )
			return -1;
		sptime	+=	CSPTimeSpan(7,0,0,0);
		return sptime.ToStockTimeDay();
	}
	else if( ktype == CKData::ktypeMonth )
	{
		if( !sptime.FromStockTimeDay( dwDate ) )
			return -1;
		int	nYearNew	=	sptime.GetYear();
		int nMonthNew	=	sptime.GetMonth();
		nYearNew	=	( nMonthNew >= 12 ? nYearNew+1 : nYearNew );
		nMonthNew	=	( nMonthNew >= 12 ? 1 : nMonthNew+1 );
		CSPTime	sptime2( nYearNew, nMonthNew, sptime.GetDay(), sptime.GetHour(),sptime.GetMinute(), sptime.GetSecond() );

		if( 6 == sptime2.GetDayOfWeek() )	//	Friday
			sptime2	+=	CSPTimeSpan(3,0,0,0);
		else
			sptime2	+=	CSPTimeSpan(1,0,0,0);
		return sptime2.ToStockTimeDay();
	}
	else if( ktype == CKData::ktypeMin5 )
	{
		if( !sptime.FromStockTimeMin( dwDate ) )
			return -1;
		if( sptime.GetHour() == 11 && sptime.GetMinute() >= 25 )
			sptime	+=	CSPTimeSpan(0,1,35,0);
		else if( sptime.GetHour() == 14 && sptime.GetMinute() >= 55 )
			sptime	+=	CSPTimeSpan(0,18,35,0);
		else
			sptime	+=	CSPTimeSpan(0,0,5,0);
		return sptime.ToStockTimeMin();
	}
	else if( ktype == CKData::ktypeMin15 )
	{
		if( !sptime.FromStockTimeMin( dwDate ) )
			return -1;
		if( sptime.GetHour() == 11 && sptime.GetMinute() >= 15 )
			sptime	+=	CSPTimeSpan(0,1,45,0);
		else if( sptime.GetHour() == 14 && sptime.GetMinute() >= 45 )
			sptime	+=	CSPTimeSpan(0,18,45,0);
		else
			sptime	+=	CSPTimeSpan(0,0,15,0);
		return sptime.ToStockTimeMin();
	}
	else if( ktype == CKData::ktypeMin30 )
	{
		if( !sptime.FromStockTimeMin( dwDate ) )
			return -1;
		if( sptime.GetHour() == 11 && sptime.GetMinute() >= 0 )
			sptime	+=	CSPTimeSpan(0,2,0,0);
		else if( sptime.GetHour() == 14 && sptime.GetMinute() >= 30 )
			sptime	+=	CSPTimeSpan(0,19,0,0);
		else
			sptime	+=	CSPTimeSpan(0,0,30,0);
		return sptime.ToStockTimeMin();
	}
	else if( ktype == CKData::ktypeMin60 )
	{
		if( !sptime.FromStockTimeMin( dwDate ) )
			return -1;
		if( (sptime.GetHour() == 10 && sptime.GetMinute() >= 30) || sptime.GetHour() == 11 )
			sptime	+=	CSPTimeSpan(0,2,30,0);
		else if( sptime.GetHour() == 14 && sptime.GetMinute() >= 0 )
			sptime	+=	CSPTimeSpan(0,19,30,0);
		else
			sptime	+=	CSPTimeSpan(0,1,0,0);
		return sptime.ToStockTimeMin();
	}
	else
	{
		SP_ASSERT( FALSE );
		return -1;
	}
}

time_t CSPTime::GetTimeTradeLatest( time_t tmTradeFirstToday )
{
	CSPTime	tNow	=	CSPTime::GetCurrentTime();
	CSPTime	tTradeFirstToday( tmTradeFirstToday );
	time_t	tmTradeLatest	=	-1;

	int	nYear	= tTradeFirstToday.GetYear();
	int nMonth	= tTradeFirstToday.GetMonth();
	int	nDay	= tTradeFirstToday.GetDay();

	if( tNow > tTradeFirstToday
		&& tNow.GetYear() == nYear && tNow.GetMonth() == nMonth && tNow.GetDay() == nDay )
	{
		tmTradeLatest	=	tNow.GetTime();
		if( (tNow.GetHour() == 11 && tNow.GetMinute() >= 30)
			|| tNow.GetHour() == 12 )
			tmTradeLatest	=	CSPTime(nYear,nMonth,nDay,11,30,0).GetTime();
		else if( tNow.GetHour() >= 15 )
			tmTradeLatest	=	CSPTime(nYear,nMonth,nDay,15,0,0).GetTime();
	}
	else if( tNow > tTradeFirstToday )
		tmTradeLatest	=	CSPTime(nYear,nMonth,nDay,15,0,0).GetTime();
	
	return tmTradeLatest;
}

double CSPTime::GetTimeTradeRatioOfOneDay( CSPTime tTradeLatestDay, CSPTime tNow )
{
	DWORD	dwSecOrder	=	tNow.ToStockTimeSecOrder();
	if( 0 == dwSecOrder )
		return 1;
	if( tTradeLatestDay.GetYear() == tNow.GetYear()
		&& tTradeLatestDay.GetMonth() == tNow.GetMonth()
		&& tTradeLatestDay.GetDay() == tNow.GetDay() )
		return ((double)dwSecOrder) / GetTradeSecondsOfOneDay();
	return 1;
}

time_t CSPTime::GetTradeOffsetToTime( int offset, time_t tmDay)
{
	time_t	ret = tmDay - ((tmDay+8*3600) % 86400);
	if( offset >= 0 && offset <= 120 )
		return ret + 9*3600 + (30+offset)*60;
	if( offset > 120 && offset <= 240 )
		return ret + 13*3600 + (offset-120)*60;
	return tmDay;
}

time_t CSPTime::GetLatestTradeTime( time_t tmNow )
{
	CSPTime	tNow(tmNow);

	CSPTime	tTradeLatest	=	tNow;
	if( 7 == tNow.GetDayOfWeek() )
	{	
		tNow -= CSPTimeSpan(1,0,0,0);
		tTradeLatest	=	CSPTime(tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),15,0,0);
	}
	else if( 1 == tNow.GetDayOfWeek() )
	{
		tNow -= CSPTimeSpan(2,0,0,0);
		tTradeLatest	=	CSPTime(tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),15,0,0);
	}
	else if( (tNow.GetHour() == 9 && tNow.GetMinute() < 25 )
		|| tNow.GetHour() < 9 )
	{
		tNow -= CSPTimeSpan(1,0,0,0);
		tTradeLatest	=	CSPTime(tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),15,0,0);
	}
	else if( (tNow.GetHour() == 11 && tNow.GetMinute() >= 30)
		|| tNow.GetHour() == 12 )
	{
		tTradeLatest	=	CSPTime(tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),11,30,0).GetTime();
	}
	else if( tNow.GetHour() >= 15 )
	{
		tTradeLatest	=	CSPTime(tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),15,0,0).GetTime();
	}

	return tTradeLatest.GetTime();
}

BOOL CSPTime::InTradeTime( time_t tm, int nInflateSeconds )
{
	CSPTime	t(tm);

	// 周六周日
	if( 7 == t.GetDayOfWeek() )
		return FALSE;
	else if( 1 == t.GetDayOfWeek() )
		return FALSE;

	time_t	day = (t.GetTime()+8*3600) % 86400;

	if( day >= 9*3600+25*60-nInflateSeconds && day <= 11*3600+30*60+nInflateSeconds )
		return TRUE;
	else if( day >= 13*3600-nInflateSeconds && day <= 15*3600+nInflateSeconds )
		return TRUE;
	return FALSE;
}

BOOL CSPTime::FromStockTimeDay( DWORD date )
{
	int nHour	=	0;
	int	nMinute	=	0;
	int nYear	=	date / 10000;
	int nMonth	=	(date - nYear*10000)/100;
	int nDay	=	(date - nYear*10000 - nMonth*100);

	struct tm atm;
	atm.tm_sec = 0;
	atm.tm_min = nMinute;
	atm.tm_hour = nHour;
	if( nDay < 1 || nDay > 31)		return FALSE;
	atm.tm_mday = nDay;
	if( nMonth < 1 && nMonth > 12)	return FALSE;
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	if( nYear < 1900 )				return FALSE;
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = -1;
	time_t	tmt = mktime(&atm);
	if( tmt == -1 )		// indicates an illegal input time
		return FALSE;

	*this	=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, 0 );
	return TRUE;
}

BOOL CSPTime::FromStockTimeMin( DWORD date, DWORD year )
{
	int	nYear	=	year;
	if( -1 == nYear )
		nYear	=	1990 + date/100000000;
	date		=	date % 100000000;
	int nMonth	=	date/1000000;
	int nDay	=	(date - nMonth*1000000)/10000;
	int nHour	=	(date - nMonth*1000000 - nDay*10000)/100;
	int nMinute	=	(date - nMonth*1000000 - nDay*10000 - nHour*100);

	struct tm atm;
	atm.tm_sec = 0;
	atm.tm_min = nMinute;
	atm.tm_hour = nHour;
	if( nDay < 1 || nDay > 31)		return FALSE;
	atm.tm_mday = nDay;
	if( nMonth < 1 || nMonth > 12)	return FALSE;
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	if( nYear < 1900 )				return FALSE;
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = -1;
	time_t	tmt = mktime(&atm);
	if( tmt == -1 )		// indicates an illegal input time
		return FALSE;

	*this	=	CSPTime( nYear, nMonth, nDay, nHour, nMinute, 0 );
	return TRUE;
}

BOOL CSPTime::FromStockTime( DWORD dwDate, BOOL bDayOrMin, DWORD dwYear )
{
	if( bDayOrMin )
		return FromStockTimeDay( dwDate );
	else
		return FromStockTimeMin( dwDate, dwYear );
}

DWORD CSPTime::ToStockTimeDay( )
{
	if( -1 == GetTime() || GetTime() < 0 )
		return -1;
	if( 0 == GetTime() )
		return 0;

	return ( GetYear() * 10000 + GetMonth() * 100 + GetDay() );
}

DWORD CSPTime::ToStockTimeMin( )
{
	if( -1 == GetTime() || GetTime() < 0 )
		return -1;
	if( 0 == GetTime() )
		return 0;

	return ( (GetYear() - 1990) * 100000000 + GetMonth() * 1000000 + GetDay() * 10000
			+ GetHour() * 100 + GetMinute() );
}

DWORD CSPTime::ToStockTimeSecOrder( DWORD dwStockExchange )
{
	if( -1 == GetTime() || GetTime() < 0 || 0 == GetTime() )
		return 0;

	if( GetHour() < 9 || (GetHour() == 9 && GetMinute() < 30) )
		return 0;

	CSPTime tmStart = CSPTime(GetYear(),GetMonth(),GetDay(),9,30,0);
	CSPTime tmEnd	= CSPTime(GetYear(),GetMonth(),GetDay(),15,0,0);
	if( *this < tmStart )
		return 0;
	if( *this > tmEnd )
		return 14400;
	CSPTimeSpan	tmSpan	=	*this - tmStart;
	
	int	nSec	=	tmSpan.GetTotalSeconds();
	if( nSec >= 0 && nSec <= 7200 )
		return nSec;
	if( nSec > 7200 && nSec < 12600 )
		return 7200;
	if( nSec >= 12600 && nSec <= 19800 )
		return nSec-5400;

	SP_ASSERT( FALSE );
	return 0;
}

DWORD CSPTime::ToStockTime( BOOL bDayOrMin )
{
	if( bDayOrMin )
		return ToStockTimeDay( );
	else
		return ToStockTimeMin( );
}

struct tm* CSPTime::GetGmtTm(struct tm* ptm) const
{
	if (ptm != NULL)
	{
		*ptm = *gmtime(&m_time);
		return ptm;
	}
	else
		return gmtime(&m_time);
}

struct tm* CSPTime::GetLocalTm(struct tm* ptm) const
{
	time_t	time_temp = m_time;
	if( m_time > 0 )
		time_temp = m_time+28800;	// 北京时间

	if (ptm != NULL)
	{
		struct tm* ptmTemp = gmtime(&time_temp);
		if (ptmTemp == NULL)
			return NULL;    // indicates the m_time was not initialized!

		*ptm = *ptmTemp;
		return ptm;
	}
	else
		return gmtime(&time_temp);
/*
	if (ptm != NULL)
	{
		struct tm* ptmTemp = localtime(&m_time);
		if (ptmTemp == NULL)
			return NULL;    // indicates the m_time was not initialized!

		*ptm = *ptmTemp;
		return ptm;
	}
	else
		return localtime(&m_time);
*/
}

BOOL CSPTime::GetAsSystemTime(SYSTEMTIME& timeDest) const
{
	struct tm* ptm = GetLocalTm(NULL);
	if (ptm == NULL)
		return FALSE;

	timeDest.wYear = (WORD) (1900 + ptm->tm_year);
	timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
	timeDest.wDayOfWeek = (WORD) ptm->tm_wday;
	timeDest.wDay = (WORD) ptm->tm_mday;
	timeDest.wHour = (WORD) ptm->tm_hour;
	timeDest.wMinute = (WORD) ptm->tm_min;
	timeDest.wSecond = (WORD) ptm->tm_sec;
	timeDest.wMilliseconds = 0;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// String formatting

#define maxTimeBufferSize       128
	// Verifies will fail if the needed buffer size is too large

#ifdef _UNICODE
#endif

CSPString CSPTimeSpan::Format(LPCTSTR pFormat) const
// formatting timespans is a little trickier than formatting CSPTimes
//  * we are only interested in relative time formats, ie. it is illegal
//      to format anything dealing with absolute time (i.e. years, months,
//         day of week, day of year, timezones, ...)
//  * the only valid formats:
//      %D - # of days -- NEW !!!
//      %H - hour in 24 hour format
//      %M - minute (0-59)
//      %S - seconds (0-59)
//      %% - percent sign
{
	TCHAR szBuffer[maxTimeBufferSize];
	TCHAR ch;
	LPTSTR pch = szBuffer;

	while ((ch = *pFormat++) != '\0')
	{
		SP_ASSERT(pch < &szBuffer[maxTimeBufferSize]);
		if (ch == '%')
		{
			switch (ch = *pFormat++)
			{
			default:
				SP_ASSERT(FALSE);      // probably a bad format character
			case '%':
				*pch++ = ch;
				break;
			case 'D':
				pch += wsprintf(pch, _T("%ld"), GetDays());
				break;
			case 'H':
				pch += wsprintf(pch, _T("%02d"), GetHours());
				break;
			case 'M':
				pch += wsprintf(pch, _T("%02d"), GetMinutes());
				break;
			case 'S':
				pch += wsprintf(pch, _T("%02d"), GetSeconds());
				break;
			}
		}
		else
		{
			*pch++ = ch;
			if (_istlead(ch))
			{
				SP_ASSERT(pch < &szBuffer[maxTimeBufferSize]);
				*pch++ = *pFormat++;
			}
		}
	}

	*pch = '\0';
	return szBuffer;
}

CSPString CSPTime::Format(LPCTSTR pFormat) const
{
	TCHAR szBuffer[maxTimeBufferSize];

	struct tm* ptmTemp = localtime(&m_time);
	if (ptmTemp == NULL ||
		!_tcsftime(szBuffer, sizeof(szBuffer), pFormat, ptmTemp))
		szBuffer[0] = '\0';
	return szBuffer;
}

CSPString CSPTime::FormatGmt(LPCTSTR pFormat) const
{
	TCHAR szBuffer[maxTimeBufferSize];

	struct tm* ptmTemp = gmtime(&m_time);
	if (ptmTemp == NULL ||
		!_tcsftime(szBuffer, sizeof(szBuffer), pFormat, ptmTemp))
		szBuffer[0] = '\0';
	return szBuffer;
}

#ifdef _UNICODE
// These functions are provided for compatibility with MFC 3.x
CSPString CSPTime::Format(LPCSTR pFormat) const
{
	CSPString strFormat(pFormat);
	return Format((LPCTSTR)strFormat);
}

CSPString CSPTime::FormatGmt(LPCSTR pFormat) const
{
	CSPString strFormat(pFormat);
	return FormatGmt((LPCTSTR)strFormat);
}

CSPString CSPTimeSpan::Format(LPCSTR pFormat) const
{
	CSPString strFormat = pFormat;
	return Format((LPCTSTR)strFormat);
}
#endif // _UNICODE

/////////////////////////////////////////////////////////////////////////////
