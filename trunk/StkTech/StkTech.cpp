// StkTech.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "StkTech.h"
#include "Stock.h"
#include "TechUser.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

/*
 *  功能：	获得实现的外挂指标总个数
 *  返回值：实现的外挂指标总个数
 */
TECHUSER_API UINT WINAPI GetTechUserCount()
{
	return 1;
}

/*
 *  功能：	获得指标信息
 *  nID:	指标标识，介于 0 <= nID < GetTechUserCount()
 *  pInfo:	返回信息
 *  返回值：TRUE成功，FALSE失败
 */
TECHUSER_API BOOL WINAPI GetTechUserInfo( UINT nID, PTECHUSER_INFO pInfo )
{
	switch( nID )
	{
	case 0:
		strcpy( pInfo->m_szName, "外挂指标" );
		strcpy( pInfo->m_szShortName, "WGZB" );
		pInfo->m_nLineCount	=	1;
		strcpy( pInfo->m_aszLineName[0], "LINE1" );
		pInfo->m_bAutoMA	=	TRUE;
		pInfo->m_nMADays	=	6;
		pInfo->m_itsGoldenFork	=	ITS_BUY;
		pInfo->m_itsDeadFork	=	ITS_SELL;
		return TRUE;
	default:;
	}
	return FALSE;
}

/*
 *  功能：	计算指标值
 *  nID:	指标标识，介于 0 <= nID < GetTechUserCount()
 *  pInfo:	计算数据，参数，并返回计算结果信息
 *  返回值：TRUE成功，FALSE失败
 */
TECHUSER_API BOOL WINAPI Calculate( UINT nID, PCALCULATE_INFO pInfo )
{
	CKData * pKData = pInfo->m_pKData;
	int nIndex = pInfo->m_nIndex;

	switch( nID )
	{
	case 0:
		{	// 10日OSC指标
			double	dCt = pKData->ElementAt(nIndex).m_fClose;
			double	dMA = 0;
			int	nCount	=	0;
			for( int k=nIndex; k>=0; k-- )
			{
				dMA	+=	pKData->ElementAt(k).m_fClose;

				nCount	++;
				if( nCount == 10 )
				{
					dMA	=	dMA / 10;
					if( dMA < 1e-4 )
						return FALSE;
					pInfo->m_dValue1	=	(dCt/dMA);
					return TRUE;
				}
			}
			return FALSE;
		}		
	default:;
	}
	return FALSE;
}

/*
 *  功能：	获得买卖信号
 *  nID:	指标标识，介于 0 <= nID < GetTechUserCount()
 *  pInfo:	计算数据，参数，并返回计算结果信息
 *  返回值：TRUE成功，FALSE失败
 */
TECHUSER_API BOOL WINAPI GetSignal( UINT nID, PCALCULATE_INFO pInfo )
{
	switch( nID )
	{
	case 0:
		//if( ... )
		//	pInfo->m_nSignal	=	ITS_BUY;
		//if( ... )
		//	pInfo->m_nSignal	=	ITS_SELL;
		pInfo->m_nSignal	=	ITS_NOTHING;
		break;
	default:;
	}
	return TRUE;
}

