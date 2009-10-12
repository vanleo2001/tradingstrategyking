/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Alarm functions;
*/

#include	"StdAfx.h"
#include	"Alarm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CSPString CAlarmCondContainer::AlarmCondToString( ALARMCOND & cond )
{
	CSPString strResult;
	if( ALARM_TYPE_DIFFPERCENTMORE == cond.m_type )
	{
		strResult.Format( "%s %s > %.2f", cond.m_szCode, AfxGetVariantName( SLH_DIFFPERCENT, FALSE ), cond.m_fValue );
	}
	else if( ALARM_TYPE_DIFFPERCENTLESS == cond.m_type )
	{
		strResult.Format( "%s %s < %.2f", cond.m_szCode, AfxGetVariantName( SLH_DIFFPERCENT, FALSE ), cond.m_fValue );
	}
	else if( ALARM_TYPE_TRADEVOLUME == cond.m_type )
	{
		strResult.Format( "%s %s > %.2f", cond.m_szCode, AfxGetVariantName( SLH_VOLUME, FALSE ), cond.m_fValue*0.01 );
	}
	return strResult;
}

CSPString CAlarmContainer::GetDescript( ALARM & alarm )
{
	CSPString strResult;
	if( ALARM_TYPE_DIFFPERCENTMORE == alarm.cond.m_type )
	{
		strResult.Format( "%s >%.2f", AfxGetVariantName( SLH_DIFFPERCENT, FALSE ), alarm.cond.m_fValue );
	}
	else if( ALARM_TYPE_DIFFPERCENTLESS == alarm.cond.m_type )
	{
		strResult.Format( "%s <%.2f", AfxGetVariantName( SLH_DIFFPERCENT, FALSE ), alarm.cond.m_fValue );
	}
	else if( ALARM_TYPE_TRADEVOLUME == alarm.cond.m_type )
	{
		strResult.Format( "%s >%.2f", AfxGetVariantName( SLH_VOLUME, FALSE ), alarm.cond.m_fValue*0.01 );
	}
	return strResult;
}

CAlarmContainer & AfxGetAlarmContainer()
{
	static	CAlarmContainer g_alarm;
	return g_alarm;
}

int CAlarmContainer::AddAlarm( ALARMCOND * pcond, REPORT * pReport, REPORT * pReportLast )
{
	CSPMutex::Scoped	l(m_mutex);

	if( NULL == pcond || NULL == pReport || NULL == pReportLast )
		return -1;

	ALARM a;
	memcpy( &(a.cond), pcond, sizeof(a.cond) );
	memcpy( &(a.report), pReport, sizeof(a.report) );
	a.report.m_fVolume = pReport->m_fVolume - pReportLast->m_fVolume;
	a.report.m_fAmount = pReport->m_fAmount - pReportLast->m_fAmount;
	return Add( a );
}

BOOL CAlarmContainer::OnReceiveReport( CStockInfo * pInfo, REPORT * pReport, REPORT * pReportLast )
{
	if( NULL == pInfo || NULL == pReport || NULL == pReportLast || !pInfo->IsValidStock() )
		return FALSE;

	float fDiffPercent;
	float fTradeVolume;

	if( pReport->m_fLast <= 1e-4 || pReport->m_fNew < 1e-4 )
		return FALSE;
	fDiffPercent = (float)( 100. * pReport->m_fNew / pReport->m_fLast - 100 );
	fTradeVolume = pReport->m_fVolume - pReportLast->m_fVolume;
	if( fTradeVolume <= 0 )
		return FALSE;

	CAlarmCondContainer & conds = AfxGetProfile().GetAlarmCondContainer();
	for( int i=0; i<conds.GetSize(); i++ )
	{
		ALARMCOND cond = conds.GetAt(i);
		if( ALARM_TYPE_DIFFPERCENTMORE == cond.m_type )
		{
			if( fDiffPercent > cond.m_fValue && pInfo->IsEqualTo(cond.m_dwMarket,cond.m_szCode) )
			{
				AddAlarm( &cond, pReport, pReportLast );
				return TRUE;
			}
		}
		else if( ALARM_TYPE_DIFFPERCENTLESS == cond.m_type )
		{
			if( fDiffPercent < cond.m_fValue && pInfo->IsEqualTo(cond.m_dwMarket,cond.m_szCode) )
			{
				AddAlarm( &cond, pReport, pReportLast );
				return TRUE;
			}
		}
		else if( ALARM_TYPE_TRADEVOLUME == cond.m_type )
		{
			if( fTradeVolume > cond.m_fValue && pInfo->IsEqualTo(cond.m_dwMarket,cond.m_szCode) )
			{
				AddAlarm( &cond, pReport, pReportLast );
				return TRUE;
			}
		}
	}
	return FALSE;
}

