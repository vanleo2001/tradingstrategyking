
#include "stdafx.h"
#include "../Include/Database.h"
#include "../Include/SpString.h"
#include "SpLock.h"
#include <direct.h>
#include "SelfDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////
// SelfDB strings

extern	char ml_dat[];
extern	char ml_dat[];
extern	char ml_sh_info[];
extern	char ml_sz_info[];
extern	char ml_sh_now[];
extern	char ml_sz_now[];
extern	char ml_sh_pyjc[];
extern	char ml_sz_pyjc[];
extern	char ml_sh_trace[];
extern	char ml_sz_trace[];
extern	char ml_sh_minute[];
extern	char ml_sz_minute[];

extern	char ml_data[];
extern	char ml_sh[];
extern	char ml_sz[];
extern	char ml_base[];
extern	char ml_month[];
extern	char ml_week[];
extern	char ml_day[];
extern	char ml_min5[];
extern	char ml_sh_base[];
extern	char ml_sz_base[];
extern	char ml_sh_month[];
extern	char ml_sz_month[];
extern	char ml_sh_week[];
extern	char ml_sz_week[];
extern	char ml_sh_day[];
extern	char ml_sz_day[];
extern	char ml_sh_min[];
extern	char ml_sz_min[];
extern	char ml_ext_base[];
extern	char ml_ext_month[];
extern	char ml_ext_week[];
extern	char ml_ext_day[];
extern	char ml_ext_min5[];

char self_ext_xdr[]		= ".dat";

char self_sh_xdr[]		= "data\\sh\\xdr\\";
char self_sz_xdr[]		= "data\\sz\\xdr\\";
char self_news[]		= "news\\";

char self_chna_xdr[]	= "data\\chna.pwr";		// 除权文件，与分析家格式相同
char self_chna_basetbl[]= "data\\shna.bst";		// 财务资料表

// 代码表文件
char self_sh_code[]		= "data\\shse.cod";		// 上海代码表
char self_sz_code[]		= "data\\szse.cod";		// 深圳代码表

// 行情缓存文件
char self_sh_report[]	= "data\\shse.rpt";		// 上海行情保存文件
char self_sz_report[]	= "data\\szse.rpt";		// 深圳行情保存文件
char self_sh_minute[]	= "data\\shse.min";		// 上海分时保存文件
char self_sz_minute[]	= "data\\szse.min";		// 深圳分时保存文件
char self_outline[]		= "data\\clkoutln.out";

/////////////////////////////////////////////////////////////////////////////////////
// SelfDB convertor

BOOL convert_CLK_DRDATA_to_DRDATA(DWORD dwMarket, const char *szCode, CLK_DRDATA * pdbdr, DRDATA * pdr)
{
	SP_ASSERT(pdbdr && pdr);
	if (NULL == pdbdr || NULL == pdr)
		return FALSE;
	memset(pdr, 0, sizeof(DRDATA));

	pdr->m_dwMarket	= dwMarket;
	if (szCode)
		strncpy(pdr->m_szCode, szCode, min(sizeof(pdr->m_szCode)-1,strlen(szCode)));
	
	pdr->m_date	= pdbdr->date;
	CSPTime	sptime;
	if (sptime.FromStockTimeDay(pdr->m_date))
		pdr->m_time	= sptime.GetTime();

	pdr->m_fProfit	= (float)(pdbdr->bonus * 0.0001);
	pdr->m_fGive	= (float)(pdbdr->bonus_share * 0.001);
	pdr->m_fPei		= (float)(pdbdr->ration_share * 0.001);
	pdr->m_fPeiPrice= (float)(pdbdr->ration_value * 0.001);

	return TRUE;
}

BOOL convert_DRDATA_to_CLK_DRDATA(DRDATA * pdr, CLK_DRDATA * pdbdr)
{
	SP_ASSERT(pdbdr && pdr);
	if (NULL == pdbdr || NULL == pdr)
		return FALSE;
	memset(pdbdr, 0, sizeof(CLK_DRDATA));

	pdbdr->date	= pdr->m_date;

	pdbdr->bonus		= (DWORD)(pdr->m_fProfit * 10000);
	pdbdr->bonus_share	= (DWORD)(pdr->m_fGive * 1000);
	pdbdr->ration_share	= (DWORD)(pdr->m_fPei * 1000);
	pdbdr->ration_value	= (DWORD)(pdr->m_fPeiPrice * 1000);
	return TRUE;
}

BOOL convert_FXJ_DRDATA_to_DRDATA(DWORD dwMarket, const char *szCode, FXJ_DRDATA * pfxjdr, DRDATA * pdr)
{
	SP_ASSERT(pfxjdr && pdr);
	if (NULL == pfxjdr || NULL == pdr)
		return FALSE;
	memset(pdr, 0, sizeof(DRDATA));

	pdr->m_dwMarket	= dwMarket;
	if (szCode)
		strncpy(pdr->m_szCode, szCode, min(sizeof(pdr->m_szCode)-1,strlen(szCode)));
	
	pdr->m_time	= pfxjdr->m_time;
	CSPTime	sptime(pdr->m_time);
	pdr->m_date	= sptime.ToStockTimeDay();

	pdr->m_fProfit	= pfxjdr->m_fProfit;
	pdr->m_fGive	= pfxjdr->m_fGive;
	pdr->m_fPei		= pfxjdr->m_fPei;
	pdr->m_fPeiPrice= pfxjdr->m_fPeiPrice;

	return TRUE;
}

double StringToDouble(const char *sz, int nLen)
{
	char	szTemp[32];
	if (NULL == sz || nLen >= sizeof(szTemp))
	{
		SP_ASSERT(FALSE);
		return 0;
	}

	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, sz, nLen);

	int	nPos	= 0;
	while(isspace(szTemp[nPos]) && nPos < nLen)
		nPos ++;
	if (nPos == nLen)
		return 0;
	return atof(&szTemp[nPos]);
}

BOOL convert_TDX_BASEDATA_to_BASEDATA(TDX_BASEDATA * ptdxbd, BASEDATA *pbd)
{
	SP_ASSERT(ptdxbd && pbd);
	if (NULL == ptdxbd || NULL == pbd)
		return FALSE;
	memset(pbd, 0, sizeof(BASEDATA));
	
	pbd->m_dwMarket	= CStock::marketCHNA;
	strncpy(pbd->m_szCode, ptdxbd->m_szCode, min(sizeof(pbd->m_szCode)-1,sizeof(ptdxbd->m_szCode)));

	CStockInfo	info;
	if (AfxGetStockContainer().GetStockInfo(pbd->m_szCode, &info))
	{
		int	nSize	= info.m_basedata.GetSize();
		if (nSize > 0)
			memcpy(pbd, &(info.m_basedata.ElementAt(nSize-1)), sizeof(BASEDATA));
		pbd->m_dwMarket	= info.GetMarket();
	}
	
	// char	m_szDomain[STKLIB_MAX_DOMAIN];		// 板块
	// char	m_szProvince[STKLIB_MAX_PROVINCE];	// 省份

	if ('0' == ptdxbd->m_szMarket)
		pbd->m_dwMarket	= CStock::marketSZSE;
	else if ('1' == ptdxbd->m_szMarket)
		pbd->m_dwMarket	= CStock::marketSHSE;

	pbd->m_date	= (DWORD)StringToDouble(ptdxbd->m_szDate_modified, sizeof(ptdxbd->m_szDate_modified));
	pbd->m_reporttype	= CStock::reportAnnals;	// 报告类型：年报、中报、季报
	CSPTime	sptime;
	if (sptime.FromStockTimeDay(pbd->m_date))
	{
		pbd->m_time	= sptime.GetTime();
		int nMonth = sptime.GetMonth();
		if (nMonth >= 4 && nMonth <= 6)
			pbd->m_reporttype	= CStock::reportQuarter;
		else if (nMonth >= 7 && nMonth <= 9)
			pbd->m_reporttype	= CStock::reportMid;
		else if (nMonth >= 10 && nMonth <= 12)
			pbd->m_reporttype	= CStock::reportQuarter3;
	}

	// float	m_fErate_dollar;			// 当期美元汇率
	// float	m_fErate_hkdollar;			// 当期港币汇率

	// ★偿债能力
	// float	m_fRatio_liquidity;			// 流动比率
	// float	m_fRatio_quick;				// 速动比率
	// float	m_fVelocity_receivables;	// 应收帐款周率
	
	// ★经营能力
	// float	m_fVelocity_merchandise;	// 存货周转率
	pbd->m_fMain_income			= (float)StringToDouble(ptdxbd->m_szMain_income, sizeof(ptdxbd->m_szMain_income)) * 1000;
	// float	m_fCash_ps;					// 每股净现金流量
	
	// ★资本结构
	pbd->m_datebegin			= (DWORD)StringToDouble(ptdxbd->m_szDate_begin, sizeof(ptdxbd->m_szDate_begin));
	pbd->m_fShare_count_total	= (float)StringToDouble(ptdxbd->m_szShare_count_total, sizeof(ptdxbd->m_szShare_count_total)) * 10000;
	pbd->m_fShare_count_a		= (float)StringToDouble(ptdxbd->m_szShare_count_currency, sizeof(ptdxbd->m_szShare_count_currency)) * 10000;
	pbd->m_fShare_count_b		= (float)StringToDouble(ptdxbd->m_szShare_count_b, sizeof(ptdxbd->m_szShare_count_b)) * 10000;
	pbd->m_fShare_count_h		= (float)StringToDouble(ptdxbd->m_szShare_count_h, sizeof(ptdxbd->m_szShare_count_h)) * 10000;
	pbd->m_fShare_count_national= (float)StringToDouble(ptdxbd->m_szShare_count_national, sizeof(ptdxbd->m_szShare_count_national)) * 10000;
	pbd->m_fShare_count_corp	= (float)StringToDouble(ptdxbd->m_szShare_count_corp, sizeof(ptdxbd->m_szShare_count_corp)) * 10000;
	if (CStock::typeshB == info.GetType() || CStock::typeszB == info.GetType())	// A B股 交换
	{
		float	fTemp	= pbd->m_fShare_count_a;
		pbd->m_fShare_count_a	= pbd->m_fShare_count_b;
		pbd->m_fShare_count_b	= fTemp;
	}
	
	if (pbd->m_fShare_count_total > 1e-4)
		pbd->m_fProfit_psud		= (float)StringToDouble(ptdxbd->m_szProfit_ud, sizeof(ptdxbd->m_szProfit_ud)) * 1000 / pbd->m_fShare_count_total;
	pbd->m_fAsset				= (float)StringToDouble(ptdxbd->m_szAsset, sizeof(ptdxbd->m_szAsset)) * 1000;
	if (pbd->m_fAsset > 1e-4)
	{
		pbd->m_fRatio_holderright= (float)(100 * StringToDouble(ptdxbd->m_szNet_asset, sizeof(ptdxbd->m_szNet_asset)) * 1000 / pbd->m_fAsset);
		pbd->m_fRatio_longdebt	= (float)(100 * StringToDouble(ptdxbd->m_szDebt_long, sizeof(ptdxbd->m_szDebt_long)) * 1000 / pbd->m_fAsset);
		pbd->m_fRatio_debt		= (float)(100 * (StringToDouble(ptdxbd->m_szDebt_long, sizeof(ptdxbd->m_szDebt_long)) * 1000 / pbd->m_fAsset
										+ StringToDouble(ptdxbd->m_szDebt_long, sizeof(ptdxbd->m_szDebt_long)) * 1000 / pbd->m_fAsset));
	}

	// ★投资收益能力
	pbd->m_fNetasset_ps_regulate= (float)StringToDouble(ptdxbd->m_szNet_asset_ps_regulate, sizeof(ptdxbd->m_szNet_asset_ps_regulate));
	if (pbd->m_fShare_count_total > 1e-4)
	{
		pbd->m_fNetasset_ps		= (float)StringToDouble(ptdxbd->m_szNet_asset, sizeof(ptdxbd->m_szNet_asset)) * 1000 / pbd->m_fShare_count_total;
		pbd->m_fEps				= (float)StringToDouble(ptdxbd->m_szNet_profit, sizeof(ptdxbd->m_szNet_profit)) * 1000 / pbd->m_fShare_count_total;
		pbd->m_fEps_deduct		= (float)(pbd->m_fEps - StringToDouble(ptdxbd->m_szOut_profit, sizeof(ptdxbd->m_szOut_profit)) * 1000 / pbd->m_fShare_count_total);
	}

	pbd->m_fNet_profit			= (float)StringToDouble(ptdxbd->m_szNet_profit, sizeof(ptdxbd->m_szNet_profit)) * 1000;
	pbd->m_fMain_profit			= (float)StringToDouble(ptdxbd->m_szMain_profit, sizeof(ptdxbd->m_szMain_profit)) * 1000;
	pbd->m_fTotal_profit		= (float)StringToDouble(ptdxbd->m_szTotal_profit, sizeof(ptdxbd->m_szTotal_profit)) * 1000;

	// ★盈利能力
	if (pbd->m_fMain_income > 1e-4)
		pbd->m_fProfit_margin	= (float)(100 * pbd->m_fMain_profit / pbd->m_fMain_income);
	if (pbd->m_fNetasset_ps > 1e-4)
		pbd->m_fNetasset_yield	= (float)(100 * pbd->m_fEps / pbd->m_fNetasset_ps);

	return TRUE;
}

BOOL convert_FXJ_BASEDATA_to_BASEDATA(FXJ_BASEDATA * pfxjbd, BASEDATA *pbd, DWORD dwDate)
{
	SP_ASSERT(pfxjbd && pbd);
	if (NULL == pfxjbd || NULL == pbd)
		return FALSE;
	memset(pbd, 0, sizeof(BASEDATA));
	
	pbd->m_dwMarket	= CStock::marketCHNA;
	strncpy(pbd->m_szCode, pfxjbd->m_szCode, min(sizeof(pbd->m_szCode)-1,sizeof(pfxjbd->m_szCode)));

	CStockInfo	info;
	if (AfxGetStockContainer().GetStockInfo(pbd->m_szCode, &info))
	{
		int	nSize	= info.m_basedata.GetSize();
		if (nSize > 0)
			memcpy(pbd, &(info.m_basedata.ElementAt(nSize-1)), sizeof(BASEDATA));
		pbd->m_dwMarket	= info.GetMarket();
	}
	
	// char	m_szDomain[STKLIB_MAX_DOMAIN];		// 板块
	// char	m_szProvince[STKLIB_MAX_PROVINCE];	// 省份

	if ('ZS' == pfxjbd->m_wMarket)
		pbd->m_dwMarket	= CStock::marketSZSE;
	else if ('HS' == pfxjbd->m_wMarket)
		pbd->m_dwMarket	= CStock::marketSHSE;

	pbd->m_date	= dwDate;
	pbd->m_reporttype	= CStock::reportAnnals;	// 报告类型：年报、中报、季报
	CSPTime	sptime;
	if (sptime.FromStockTimeDay(pbd->m_date))
	{
		pbd->m_time	= sptime.GetTime();
		int nMonth = sptime.GetMonth();
		if (nMonth >= 4 && nMonth <= 6)
			pbd->m_reporttype	= CStock::reportQuarter;
		else if (nMonth >= 7 && nMonth <= 9)
			pbd->m_reporttype	= CStock::reportMid;
		else if (nMonth >= 10 && nMonth <= 12)
			pbd->m_reporttype	= CStock::reportQuarter3;
	}

	// float	m_fErate_dollar;			// 当期美元汇率
	// float	m_fErate_hkdollar;			// 当期港币汇率

	// ★偿债能力
	// float	m_fRatio_liquidity;			// 流动比率
	// float	m_fRatio_quick;				// 速动比率
	// float	m_fVelocity_receivables;	// 应收帐款周率
	
	// ★经营能力
	// float	m_fVelocity_merchandise;	// 存货周转率
	pbd->m_fMain_income			= pfxjbd->m_fData[21]*1000;
	// float	m_fCash_ps;					// 每股净现金流量
	
	// ★盈利能力
	if (pbd->m_fMain_income > 1e-4)
		pbd->m_fProfit_margin	= (float)(100 * pfxjbd->m_fData[22]*1000 / pbd->m_fMain_income);
	pbd->m_fNetasset_yield		= pfxjbd->m_fData[38];

	// ★资本结构
	// DWORD	m_datebegin					// 上市日期
	pbd->m_fShare_count_total	= pfxjbd->m_fData[2]*10000;
	pbd->m_fShare_count_a		= pfxjbd->m_fData[8]*10000;
	pbd->m_fShare_count_b		= pfxjbd->m_fData[6]*10000;
	pbd->m_fShare_count_h		= pfxjbd->m_fData[7]*10000;
	pbd->m_fShare_count_national= pfxjbd->m_fData[3]*10000;
	pbd->m_fShare_count_corp	= pfxjbd->m_fData[5]*10000;
//	if (CStock::typeshB == info.GetType() || CStock::typeszB == info.GetType())	// A B股 交换
//	{
//		float	fTemp	= pbd->m_fShare_count_a;
//		pbd->m_fShare_count_a	= pbd->m_fShare_count_b;
//		pbd->m_fShare_count_b	= fTemp;
//	}
	
	pbd->m_fProfit_psud			= pfxjbd->m_fData[33];
	pbd->m_fAsset				= pfxjbd->m_fData[11]*1000;
	pbd->m_fRatio_holderright	= pfxjbd->m_fData[37];
	if (pbd->m_fAsset > 1e-4)
		pbd->m_fRatio_longdebt	= (float)(100 * pfxjbd->m_fData[17] * 1000 / pbd->m_fAsset);
	pbd->m_fRatio_debt			= 100 - pbd->m_fRatio_holderright;

	// ★投资收益能力
	pbd->m_fNetasset_ps			= pfxjbd->m_fData[35];
	pbd->m_fNetasset_ps_regulate= pfxjbd->m_fData[36];
	pbd->m_fEps					= pfxjbd->m_fData[34];
	if (pbd->m_fShare_count_total > 1e-4)
		pbd->m_fEps_deduct		= (float)(pbd->m_fEps - pfxjbd->m_fData[25] * 1000 / pbd->m_fShare_count_total);

	pbd->m_fNet_profit			= pfxjbd->m_fData[31]*1000;
	pbd->m_fMain_profit			= pfxjbd->m_fData[22]*1000;
	pbd->m_fTotal_profit		= pfxjbd->m_fData[29]*1000;

	return TRUE;
}

DWORD ReadMinute(CStock* pstock, LPCTSTR lpszFileName)
{
	SP_ASSERT(pstock && pstock->GetStockInfo().IsValidStock());
	if (!pstock || !pstock->GetStockInfo().IsValidStock())
		return 0;

	CMinute& aMinute = pstock->GetMinute();
	aMinute.RemoveAll();

	CTSKFile file;
	if (file.Open(lpszFileName))
	{
		DWORD dwDataRecordCount = file.GetDataRecordCount(pstock->GetStockInfo().GetMarket(), pstock->GetStockCode());
		aMinute.SetSize(dwDataRecordCount);
		DWORD dwCount = file.LoadDataRecord(pstock->GetStockInfo().GetMarket(), pstock->GetStockCode(), aMinute.GetData(), sizeof(MINUTE), dwDataRecordCount);
		aMinute.SetSize(dwCount);

		aMinute.Sort();
		aMinute.RemoveDirty();
	}

	return aMinute.GetSize();
}

//=============================================================================
// CTSKFile

CTSKFile::CTSKFile()
{
	Close();
}

CTSKFile::~CTSKFile()
{
	Close();
}

BOOL CTSKFile::Open(LPCTSTR lpszFileName)
{
	Close();

	if (!m_file.Open(lpszFileName, CSPFile::modeReadWrite))
		return FALSE;

	if (m_file.Read(&m_header, sizeof(m_header)) != sizeof(m_header))
	{
		SP_ASSERT(FALSE);
		return FALSE;
	}

	if (m_header.m_dwMagic != CLK_FHEADER_MAGIC)
	{
		SP_ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

void CTSKFile::Close()
{
	if (m_file.m_hFile != (UINT)CSPFile::hFileNull)
	{
		m_header.m_time = time(NULL);
		m_file.Seek(0, CSPFile::begin);
		m_file.Write(&m_header, sizeof(m_header));
		m_file.Close();
	}

	memset(&m_header, 0, sizeof(m_header));
	memset(&m_CurIndexRecord, 0, sizeof(m_CurIndexRecord));

	m_dwPosCurIndex	= -1;
}

// 创建空的数据文件
BOOL CTSKFile::BuildEmptyFile(LPCTSTR lpszFileName,
						DWORD dwDataType,	// 数据分类说明，see CStock::DataType
						DWORD dwMarket,			// 市场，see CStock::StockMarket，如果文件包含多个市场，则=0无效
						DWORD dwIndexRecordCount,	// 索引区记录单元个数
						DWORD dwRecordPerBlock,		// 每个Block的记录数
						BOOL bRebuildIfExists)
{
	SP_ASSERT(NULL != lpszFileName && strlen(lpszFileName) > 0);
	if (NULL == lpszFileName || strlen(lpszFileName) <= 0)
		return FALSE;

	// 如果文件存在
	if (!bRebuildIfExists && 0 == access(lpszFileName,0))
		return TRUE;

	CLK_FHEADER	header;
	memset(&header, 0,sizeof(header));
	header.m_dwMagic	= CLK_FHEADER_MAGIC;
	header.m_dwVerMajor	= CLK_FHEADER_VERMAJOR;
	header.m_dwVerMinor	= CLK_FHEADER_VERMINOR;
	strcpy(header.m_szDescript, "CLKing Stock File Structure. See www.ninebulls.com for more information.");
	header.m_dwDataType	= dwDataType;
	header.m_dwMarket	= dwMarket;
	header.m_time		= time(NULL);
	
	// 索引区信息
	header.m_dwPosFirstIndex	= sizeof(header);
	header.m_dwIndexRecordSize	= sizeof(CLK_INDEXRECORD);
	header.m_dwIndexRecordCount	= dwIndexRecordCount;
	header.m_dwStockCount		= 0;

	// 数据区信息
	switch (dwDataType)
	{
	case CStock::dataReport:
		header.m_dwDataRecordSize	= sizeof(REPORT);
		break;
	case CStock::dataMinute:
		header.m_dwDataRecordSize	= sizeof(MINUTE);
		break;
	case CStock::dataK:
		header.m_dwDataRecordSize	= sizeof(KDATA);
	default:
		SP_ASSERT(FALSE);
		return FALSE;
	}
	header.m_dwRecordPerBlock	= dwRecordPerBlock;
	header.m_dwBlockSize		= sizeof(CLK_BLOCKHEADER) + header.m_dwDataRecordSize * header.m_dwRecordPerBlock;
	header.m_dwPosFirstBlock	= sizeof(CLK_FHEADER) + header.m_dwIndexRecordSize * header.m_dwIndexRecordCount;
	header.m_dwPosFirstBlankBlock	= -1;

	CSPFile	file;
	if (!file.Open(lpszFileName, CSPFile::modeCreate | CSPFile::modeReadWrite))
	{
		SP_ASSERT(FALSE);
		return FALSE;
	}
	file.Write(&header, sizeof(header));

	for (DWORD i=0; i<header.m_dwIndexRecordCount; i++)
	{
		CLK_INDEXRECORD	index;
		memset(&index, 0, sizeof(index));
		index.m_dwMagic			= CLK_INDEXRECORD_MAGIC;
		index.m_dwPosFirstBlock	= -1;
		file.Write(&index, sizeof(index));
	}
	
	file.Flush();
	return TRUE;
}

// 重新创建，如果日期不是tmNow
BOOL CTSKFile::RemoveFileIfOutoftime(LPCTSTR lpszFileName, time_t tmNow)
{
	SP_ASSERT(NULL!=lpszFileName && strlen(lpszFileName) > 0);
	if (NULL==lpszFileName || strlen(lpszFileName) <= 0)
		return FALSE;

	CTSKFile	file;
	if (file.Open(lpszFileName))
	{
		CSPTime	ft(file.m_header.m_time);
		CSPTime	tNow(tmNow);
		
		if (CLK_FHEADER_VERMAJOR == file.m_header.m_dwVerMajor 
			&& CLK_FHEADER_VERMINOR == file.m_header.m_dwVerMinor)	// 版本相同
		{
			if (ft.GetYear() == tNow.GetYear() && ft.GetMonth() == tNow.GetMonth() && ft.GetDay() == tNow.GetDay())
				return FALSE;
			int	nDayOfWeek = tNow.GetDayOfWeek();
			if (1 == nDayOfWeek || 7 == nDayOfWeek)
				return FALSE;

			BOOL	bEmpty	= FALSE; // file.EmptyAll();
			file.Close();

			if (!bEmpty)
				return ::DeleteFile(lpszFileName);
			return TRUE;
		}
		else
		{
			file.Close();
			return ::DeleteFile(lpszFileName);
		}
	}

	return FALSE;
}

static CSPMutex g_mutexClkFile;

BOOL CTSKFile::EmptyAll()
{
	CSPMutex::Scoped locker(g_mutexClkFile);

	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return FALSE;

	// 顺序寻找
	DWORD	dwCount	= 0;
	for (DWORD i=0; i<m_header.m_dwIndexRecordCount; i++)
	{
		DWORD	dwPosIndex	= m_header.m_dwPosFirstIndex + m_header.m_dwIndexRecordSize * i;
		if (m_file.GetPosition() != dwPosIndex)
			m_file.Seek(dwPosIndex, CSPFile::begin);

		CLK_INDEXRECORD	index;
		if (sizeof(index) != m_file.Read(&index,sizeof(index)))
			return FALSE;
		if (CLK_INDEXRECORD_MAGIC != index.m_dwMagic)
			return FALSE;
		if (strlen(index.m_szCode) <= 0)
			continue;
		
		EmptyBlockChain(index.m_dwPosFirstBlock);
		index.m_dwDataRecordCountTotal	= 0;
		m_file.Seek(dwPosIndex, CSPFile::begin);
		m_file.Write(&index, sizeof(index));

		dwCount	++;
	}

	m_file.Flush();
	SP_ASSERT(dwCount == m_header.m_dwStockCount);
	return dwCount > 0; // == m_header.m_dwStockCount;
}

// 保存数据，并修改相应索引信息
DWORD CTSKFile::StoreDataRecord(DWORD dwMarket, const char * szCode,
							void * pData, DWORD dwDataElementSize, DWORD dwDataElementCount,
							BOOL bOverWrite)	// 返回成功保存记录数
{
	CSPMutex::Scoped locker(g_mutexClkFile);

	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return 0;

	CLK_INDEXRECORD	index;
	DWORD	dwPosIndexFind	= -1;
	if (!GetDataInfo(dwMarket, szCode, index, dwPosIndexFind, TRUE))
		return 0;

	if (bOverWrite)
	{
		EmptyBlockChain(index.m_dwPosFirstBlock);
		index.m_dwDataRecordCountTotal	= 0;
	}

	if (-1 == index.m_dwPosFirstBlock || 0 == index.m_dwPosFirstBlock)
		index.m_dwPosFirstBlock	= GetFirstBlankBlockPos(TRUE, TRUE);
	DWORD	dwCount = WriteData(index.m_dwPosFirstBlock, pData, dwDataElementSize, dwDataElementCount, FALSE);
	index.m_dwDataRecordCountTotal	+= dwCount;
	SetDataInfo(dwPosIndexFind, index, FALSE);

	// m_file.Flush();
	return dwCount;
}

// 得到某一股票的数据记录数	
DWORD CTSKFile::GetDataRecordCount(DWORD dwMarket, const char * szCode)
{
	CSPMutex::Scoped locker(g_mutexClkFile);

	CLK_INDEXRECORD	index;
	DWORD	dwPosIndexFind	= -1;
	if (GetDataInfo(dwMarket, szCode, index, dwPosIndexFind, FALSE))
		return index.m_dwDataRecordCountTotal;
	return 0;
}

// 读取某一股票的数据记录
DWORD CTSKFile::LoadDataRecord(DWORD dwMarket, const char * szCode,
							void * pData, DWORD dwDataElementSize, DWORD dwMaxDataElement)// 返回成功读取记录数
{
	CSPMutex::Scoped locker(g_mutexClkFile);

	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return 0;

	CLK_INDEXRECORD	index;
	DWORD	dwPosIndexFind	= -1;
	if (!GetDataInfo(dwMarket, szCode, index, dwPosIndexFind, FALSE))
		return 0;

	if (dwMaxDataElement < index.m_dwDataRecordCountTotal)
		return 0;

	return ReadData(index.m_dwPosFirstBlock, pData, dwDataElementSize, dwMaxDataElement);
}

DWORD CTSKFile::Hash(LPCTSTR key, DWORD dwMax)
{
	DWORD dwHash = 0;
	while (*key)
		dwHash = (dwHash<<5) + dwHash + *key++;
	return dwHash % dwMax;
}

// 得到某一股票的索引区信息，如果bAddIfNotExists并且不存在，则添加
BOOL CTSKFile::GetDataInfo(	DWORD dwMarket, const char * szCode, CLK_INDEXRECORD & idxRet, DWORD & dwPosIndexFind, BOOL bAddIfNotExists)
{
	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return FALSE;

	// 是否当前Cache
	if (m_CurIndexRecord.m_dwMarket == dwMarket
		&& 0 == strcmp(m_CurIndexRecord.m_szCode, szCode))
	{
		idxRet			= m_CurIndexRecord;
		dwPosIndexFind	= m_dwPosCurIndex;
		return TRUE;
	}

	DWORD	posBegin	= Hash(szCode, m_header.m_dwIndexRecordCount);
	// Hash顺序寻找
	for (DWORD i=posBegin; i<m_header.m_dwIndexRecordCount; i++)
	{
		DWORD	dwPosIndex	= m_header.m_dwPosFirstIndex + m_header.m_dwIndexRecordSize * i;
		if (m_file.GetPosition() != dwPosIndex)
			m_file.Seek(dwPosIndex, CSPFile::begin);

		CLK_INDEXRECORD	index;
		if (sizeof(index) != m_file.Read(&index,sizeof(index))
			|| CLK_INDEXRECORD_MAGIC != index.m_dwMagic)
		{
			SP_ASSERT(FALSE);
			return FALSE;
		}
		if (dwMarket == index.m_dwMarket
			&& 0 == strcmp(szCode, index.m_szCode))
		{
			idxRet				= index;
			dwPosIndexFind		= dwPosIndex;
			m_CurIndexRecord	= index;
			m_dwPosCurIndex		= dwPosIndex;
			return TRUE;
		}
		if (0 == strlen(index.m_szCode))
		{
			if (bAddIfNotExists)
			{
				index.m_dwMarket	= dwMarket;
				strncpy(index.m_szCode, szCode, min(sizeof(index.m_szCode)-1,strlen(szCode)));
				index.m_dwDataRecordCountTotal	= 0;
				index.m_dwPosFirstBlock	= GetFirstBlankBlockPos(TRUE, TRUE);

				m_file.Seek(dwPosIndex, CSPFile::begin);
				m_file.Write(&index, sizeof(index));

				// 文件头
				m_header.m_dwStockCount	+= 1;
				m_file.Seek(0, CSPFile::begin);
				m_file.Write(&m_header, sizeof(m_header));
				// m_file.Flush();

				// return
				idxRet				= index;
				dwPosIndexFind		= dwPosIndex;
				m_CurIndexRecord	= index;
				m_dwPosCurIndex		= dwPosIndex;
				return TRUE;
			}
			return FALSE;
		}

		// 循环
		if (m_header.m_dwIndexRecordCount-1 == i)
			i = -1;
		if (posBegin-1 == i)
			break;
	}

	return FALSE;
}

// 保存某一股票的索引区信息
BOOL CTSKFile::SetDataInfo(	DWORD dwPosIndex, CLK_INDEXRECORD idx, BOOL bFlush)
{
	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return FALSE;

	// 是否当前Cache
	if (m_CurIndexRecord.m_dwMarket == idx.m_dwMarket
		&& 0 == strcmp(m_CurIndexRecord.m_szCode, idx.m_szCode))
	{
		m_CurIndexRecord	= idx;
	}

	if (-1 != dwPosIndex)
	{
		m_file.Seek(dwPosIndex, CSPFile::begin);
		m_file.Write(&idx, sizeof(idx));
		if (bFlush)
			m_file.Flush();
		return TRUE;
	}

	return FALSE;
}

// 得到某一空数据块
DWORD CTSKFile::GetFirstBlankBlockPos(BOOL bAddIfNotExists, BOOL bUseIt)
{
	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return -1;

	DWORD	dwPosBlock	= m_header.m_dwPosFirstBlankBlock;
	if (-1 != dwPosBlock && 0 != dwPosBlock)
	{
		m_file.Seek(dwPosBlock, CSPFile::begin);
		
		CLK_BLOCKHEADER	bheader;
		if (sizeof(bheader) == m_file.Read(&bheader,sizeof(bheader)))
		{
			SP_ASSERT(CLK_BLOCKHEADER_MAGIC == bheader.m_dwMagic);
			SP_ASSERT(!bheader.m_bUsed);

			if (bUseIt)
			{
				bheader.m_bUsed	= bUseIt;
				bheader.m_dwDataRecordCount	= 0;
				bheader.m_dwPosNextBlock	= -1;
				m_file.Seek(dwPosBlock, CSPFile::begin);
				m_file.Write(&bheader, sizeof(bheader));
			
				m_header.m_dwPosFirstBlankBlock	= bheader.m_dwPosNextBlock;
				m_file.Seek(0, CSPFile::begin);
				m_file.Write(&m_header, sizeof(m_header));
				// m_file.Flush();
			}
			return dwPosBlock;
		}
	}

	// Add
	if (bAddIfNotExists)
	{
		SP_ASSERT(bUseIt);	// Must Use It

		DWORD	dwDataSize = m_header.m_dwDataRecordSize*m_header.m_dwRecordPerBlock;
		if (dwDataSize <= 0)
			return -1;

		m_file.SeekToEnd();
		dwPosBlock	= m_file.GetPosition();

		CLK_BLOCKHEADER	bheader;
		memset(&bheader, 0, sizeof(bheader));
		bheader.m_dwMagic	= CLK_BLOCKHEADER_MAGIC;
		bheader.m_bUsed		= bUseIt;
		bheader.m_dwPosNextBlock	= -1;
		bheader.m_dwPosFirstRecord	= dwPosBlock + sizeof(bheader);
		m_file.Write(&bheader, sizeof(bheader));
		char	* temp	= new char[dwDataSize];
		if (!temp)
			return -1;
		memset(temp, 0, m_header.m_dwDataRecordSize);
		m_file.Write(temp, dwDataSize);
		delete	[] temp;
		// m_file.Flush();
	}

	return dwPosBlock;
}

// 清空数据Block链中的数据，并将除第一个Block外的其他Block置为未用
DWORD CTSKFile::EmptyBlockChain(DWORD dwPosFirstBlock)
{
	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return FALSE;

	DWORD	dwCount	= 0;
	DWORD	dwPosBlock	= dwPosFirstBlock;

	while(-1 != dwPosBlock && 0 != dwPosBlock)
	{
		m_file.Seek(dwPosBlock, CSPFile::begin);

		DWORD	dwPosNextBlock	= -1;
		CLK_BLOCKHEADER	bheader;
		if (sizeof(bheader) != m_file.Read(&bheader,sizeof(bheader))
			|| CLK_BLOCKHEADER_MAGIC != bheader.m_dwMagic)
		{
			SP_ASSERT(FALSE);
		}
		else
		{
			dwPosNextBlock	= bheader.m_dwPosNextBlock;
		}

		// Empty it
		bheader.m_bUsed	= (dwPosFirstBlock == dwPosBlock);	// 第一块继续使用
		bheader.m_dwDataRecordCount	= 0;
		bheader.m_dwPosNextBlock	= -1;
		if (!bheader.m_bUsed)
			bheader.m_dwPosNextBlock	= m_header.m_dwPosFirstBlankBlock;
		m_file.Seek(dwPosBlock, CSPFile::begin);
		m_file.Write(&bheader, sizeof(bheader));
		
		// 加入Blank Block Chain
		if (!bheader.m_bUsed)
		{
			m_header.m_dwPosFirstBlankBlock	= dwPosBlock;
			m_file.Seek(0, CSPFile::begin);
			m_file.Write(&m_header, sizeof(m_header));
		}
		// m_file.Flush();
		dwCount	++;

		dwPosBlock	= dwPosNextBlock;
	}
	
	return dwCount;
}

// 读数据记录
DWORD CTSKFile::ReadData(DWORD dwPosBlock, void * pData, DWORD dwDataElementSize, DWORD dwMaxDataElement)
{
	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return FALSE;

	if (NULL == pData || dwMaxDataElement == 0)
		return 0;

	DWORD	dwCount	= 0;

	while(-1 != dwPosBlock && 0 != dwPosBlock)
	{
		m_file.Seek(dwPosBlock, CSPFile::begin);

		CLK_BLOCKHEADER	bheader;
		if (sizeof(bheader) != m_file.Read(&bheader,sizeof(bheader))
			|| CLK_BLOCKHEADER_MAGIC != bheader.m_dwMagic)
		{
			SP_ASSERT(FALSE);
			return dwCount;
		}
		
		for (DWORD i=0; i<bheader.m_dwDataRecordCount; i++)
		{
			DWORD	dwPos	= bheader.m_dwPosFirstRecord + i * m_header.m_dwDataRecordSize;
			if (m_file.GetPosition() != dwPos)
				m_file.Seek(dwPos, CSPFile::begin);
			m_file.Read(((BYTE *)pData)+dwDataElementSize*dwCount, min(dwDataElementSize,m_header.m_dwDataRecordSize));
			dwCount ++;
			if (dwCount >= dwMaxDataElement)
				return dwCount;
		}
		
		dwPosBlock	= bheader.m_dwPosNextBlock;
	}
	
	return dwCount;
}

// 写数据记录
DWORD CTSKFile::WriteData(DWORD dwPosBlock, void * pData, DWORD dwDataElementSize, DWORD dwDataElementCount, BOOL bFlush)
{
	SP_ASSERT(CSPFile::hFileNull != m_file.m_hFile);
	if (CSPFile::hFileNull == m_file.m_hFile)
		return 0;

	SP_ASSERT(-1 != dwPosBlock && 0 != dwPosBlock);
	if (-1 == dwPosBlock || 0 == dwPosBlock)
		return 0;

	DWORD	dwCount	= 0;

	while(dwCount < dwDataElementCount && -1 != dwPosBlock && 0 != dwPosBlock)
	{
		m_file.Seek(dwPosBlock, CSPFile::begin);

		CLK_BLOCKHEADER	bheader;
		if (sizeof(bheader) != m_file.Read(&bheader,sizeof(bheader))
			|| CLK_BLOCKHEADER_MAGIC != bheader.m_dwMagic)
		{
			SP_ASSERT(FALSE);
			return dwCount;
		}

		if (-1 != bheader.m_dwPosNextBlock && 0 != bheader.m_dwPosNextBlock)
		{
			SP_ASSERT(bheader.m_dwDataRecordCount == m_header.m_dwRecordPerBlock);
			if (bheader.m_dwDataRecordCount == m_header.m_dwRecordPerBlock)
			{
				dwPosBlock	= bheader.m_dwPosNextBlock;
				continue;
			}
		}

		// Write
		DWORD	dwCountOld	= dwCount;
		for (DWORD i=bheader.m_dwDataRecordCount; i<m_header.m_dwRecordPerBlock; i++)
		{
			DWORD	dwPos	= bheader.m_dwPosFirstRecord + i * m_header.m_dwDataRecordSize;
			if (m_file.GetPosition() != dwPos)
				m_file.Seek(dwPos, CSPFile::begin);
			m_file.Write(((BYTE *)pData)+dwDataElementSize*dwCount, min(dwDataElementSize,m_header.m_dwDataRecordSize));
			dwCount ++;
			if (dwCount >= dwDataElementCount)
				break;
		}
		
		// 修改Block Header
		bheader.m_bUsed	= TRUE;
		bheader.m_dwDataRecordCount	+= (dwCount-dwCountOld);
		bheader.m_dwPosNextBlock	= -1;
		if (dwCount < dwDataElementCount)
			bheader.m_dwPosNextBlock	= GetFirstBlankBlockPos(TRUE, TRUE);
		m_file.Seek(dwPosBlock, CSPFile::begin);
		m_file.Write(&bheader, sizeof(bheader));

		// 新Block
		dwPosBlock	= bheader.m_dwPosNextBlock;
	}

	if (bFlush)
		m_file.Flush();
	return dwCount;
}

//=============================================================================
// CSelfDB

CSelfDB::CSelfDB(const char * rootpath, BOOL bOK) : CQianlong(rootpath, bOK)
{
}

CSelfDB::~CSelfDB()
{
}

BOOL CSelfDB::GetFileName(CSPString& sFileName, int nDataType, CStockInfo* pInfo, int nKType)
{
	if (nDataType == CStock::dataOutline)
	{
		sFileName = GetRootPath();
		sFileName += self_outline;
		return TRUE;
	}

	if (pInfo==  NULL || !pInfo->IsValidStock())
		return FALSE;

	// 如果钱龙文件存在，就返回钱龙文件名
	CSPString sFileNameQL;
	if (CStock::dataDR != nDataType
		&& CQianlong::GetFileName(sFileNameQL, nDataType, pInfo, nKType)
		&& access(sFileNameQL,0) == 0)
	{
		sFileName = sFileNameQL;
		return TRUE;
	}

	// 确定市场类型
	if (pInfo->GetMarket() == CStock::marketUnknown)
		pInfo->ResolveTypeAndMarket();

	// 如果在 ml_sh 目录下找到文件，就返回找到的文件名
	// 否则，若钱龙文件名长度大于0，就返回钱龙文件名，等于0就返回 ml_sh 下的文件名
	if (nDataType == CStock::dataBasetext)
	{
		sFileName = GetRootPath();
		sFileName += ml_sh_base;
		sFileName += CSPString(pInfo->GetStockCode()) + ml_ext_base;
		if (access(sFileName, 0) != 0 && sFileNameQL.GetLength() > 0)
			sFileName = sFileNameQL;

		return TRUE;
	}
	else if (nDataType == CStock::dataK)
	{
		sFileName = GetRootPath();
		sFileName += ml_sh;

		switch (nKType)
		{
		case CKData::ktypeMonth:
			sFileName += CSPString(ml_month) + pInfo->GetStockCode() + ml_ext_month;
			break;
		case CKData::ktypeWeek:
			sFileName += CSPString(ml_week) + pInfo->GetStockCode() + ml_ext_week;
			break;
		case CKData::ktypeDay:
			sFileName += CSPString(ml_day) + pInfo->GetStockCode() + ml_ext_day;
			break;
		case CKData::ktypeMin60:
			return FALSE;
		case CKData::ktypeMin30:
			return FALSE;
		case CKData::ktypeMin15:
			return FALSE;
		case CKData::ktypeMin5:
			sFileName += CSPString(ml_min5) + pInfo->GetStockCode() + ml_ext_min5;
			break;
		default:
			return FALSE;
		}

		if (access(sFileName, 0) != 0 && sFileNameQL.GetLength() > 0)
			sFileName = sFileNameQL;

		return TRUE;
	}
	else if (nDataType == CStock::dataDR)
	{
		sFileName = GetRootPath();
		if (pInfo->IsShenZhen())
			sFileName += self_sz_xdr;
		else
			sFileName += self_sh_xdr;
		sFileName += CSPString(pInfo->GetStockCode()) + self_ext_xdr;
		if (access(sFileName, 0) == 0)
			return TRUE;

		CSPString sTemp = GetRootPath();
		sTemp += self_sh_xdr;
		sTemp += CSPString(pInfo->GetStockCode()) + self_ext_xdr;
		if (access(sTemp,0) == 0)
			sFileName = sTemp;

		return TRUE;
	}
	else if (nDataType == CStock::dataReport)
	{
		sFileName = GetRootPath();
		if (pInfo->IsShenZhen())
			sFileName += self_sz_report;
		else
			sFileName += self_sh_report;

		return TRUE;
	}
	else if (nDataType == CStock::dataMinute)
	{
		sFileName = GetRootPath();
		if (pInfo->IsShenZhen())
			sFileName += self_sz_minute;
		else
			sFileName += self_sh_minute;

		return TRUE;
	}

	return FALSE;
}

CSPString CSelfDB::GetNewsPath(DWORD dwMarket)
{
	CSPString	strPath	= AfxGetProfile().GetSelfDBPath();
	strPath	+= self_news;
	return strPath;
}

CSPString CSelfDB::GetBasePath(DWORD dwMarket)
{
	CSPString	strPath	= AfxGetProfile().GetSelfDBPath();
	switch (dwMarket)
	{
	case CStock::marketSHSE:
		strPath	+= ml_sh_base;
		break;
	case CStock::marketSZSE:
		strPath	+= ml_sz_base;
		break;
	default:
		strPath	+= ml_sh_base;
		SP_ASSERT(FALSE);
	}
	return strPath;
}

BOOL CSelfDB::CreateSelfDB(const char * rootpath)
{
	if (NULL == rootpath || strlen(rootpath) == 0)
		return FALSE;

	// get rootpath
	CSPString	strRoot	= rootpath;
	int nLen = strRoot.GetLength();
	if (strRoot[nLen-1] != '\\' && strRoot[nLen-1] != '/')
		strRoot	+= CHAR_DIRSEP;
	nLen	= strRoot.GetLength();

	if (0 != access(strRoot, 0))
		return FALSE;

	if (0 != access(strRoot + ml_dat, 0))
		mkdir(strRoot + ml_dat);
	if (0 != access(strRoot + ml_data, 0))
		mkdir(strRoot + ml_data);

	if (0 != access(strRoot + ml_sh, 0))
		mkdir(strRoot + ml_sh);
	if (0 != access(strRoot + ml_sh_base, 0))
		mkdir(strRoot + ml_sh_base);
	if (0 != access(strRoot + ml_sh_month, 0))
		mkdir(strRoot + ml_sh_month);
	if (0 != access(strRoot + ml_sh_week, 0))
		mkdir(strRoot + ml_sh_week);
	if (0 != access(strRoot + ml_sh_day, 0))
		mkdir(strRoot + ml_sh_day);
	if (0 != access(strRoot + ml_sh_min, 0))
		mkdir(strRoot + ml_sh_min);
	if (0 != access(strRoot + self_sh_xdr, 0))
		mkdir(strRoot + self_sh_xdr);

	if (0 != access(strRoot + ml_sz, 0))
		mkdir(strRoot + ml_sz);
	if (0 != access(strRoot + ml_sz_base, 0))
		mkdir(strRoot + ml_sz_base);
	if (0 != access(strRoot + ml_sz_month, 0))
		mkdir(strRoot + ml_sz_month);
	if (0 != access(strRoot + ml_sz_week, 0))
		mkdir(strRoot + ml_sz_week);
	if (0 != access(strRoot + ml_sz_day, 0))
		mkdir(strRoot + ml_sz_day);
	if (0 != access(strRoot + ml_sz_min, 0))
		mkdir(strRoot + ml_sz_min);
	if (0 != access(strRoot + self_sz_xdr, 0))
		mkdir(strRoot + self_sz_xdr);

	if (0 != access(strRoot + self_news, 0))
		mkdir(strRoot + self_news);

	return TRUE;
}

int LoadCodeTable(CStockContainer& container, LPCTSTR lpszFileName, DWORD dwMarket)
{
	int nCount = 0;

	char seps[]   = ",\t\r\n";
	char quotation_mark[] = "\"";

	BOOL bHasQuotationMark = FALSE;
	CSPFile	file;
	if (file.Open(lpszFileName, CSPFile::modeRead | CSPFile::shareDenyNone))
	{
		CSPString	rString;
		while(file.ReadString(rString))
		{
			CStockInfo	info;

			// 600000,浦发银行,17,PFYH,"PuFaYinHang"
			// 英文名称可以用双引号引起来
			int nToken = 0;
			while(!rString.IsEmpty())
			{
				nToken ++;

				CSPString sTemp;
				
				int nIndex = -1;
				if (5 == nToken && -1 != (nIndex=rString.Find(quotation_mark)))
				{
					rString = rString.Mid(nIndex+1);
					nIndex = rString.Find(quotation_mark);
					if (-1 != nIndex)	sTemp = rString.Left(nIndex);
					if (-1 != nIndex)	rString = rString.Mid(nIndex+1);
					nIndex = rString.FindOneOf(seps);
					if (-1 != nIndex)
						rString = rString.Mid(nIndex+1);
					else
						rString.Empty();
				}
				else
				{
					nIndex = rString.FindOneOf(seps);
					if (-1 != nIndex)
					{
						sTemp = rString.Left(nIndex);
						rString = rString.Mid(nIndex+1);
					}
					else
					{
						sTemp = rString;
						rString.Empty();
					}
				}

				if (1 == nToken)
					info.SetStockCode(dwMarket, sTemp);
				else if (2 == nToken)
					info.SetStockName(sTemp);
				else if (3 == nToken)
					info.SetType(atol(sTemp));
				else if (4 == nToken)
					info.SetStockShortName(sTemp);
				else if (5 == nToken)
				{
					info.SetStockNameEnu(sTemp);
					break;
				}
			}

			if (info.IsValidStock())
				container.Add(info);
			nCount ++;
		}

		file.Close();
	}

	return nCount;
}

int	CSelfDB::LoadCodetable(CStockContainer& container)
{
	if (!m_bIsOK)	return 0;

	container.SetSize(0, 2000);

	// 上海指数
	CSPString	sFileName	= GetRootPath();
	sFileName	+= self_sh_code;
	LoadCodeTable(container, sFileName, CStock::marketSHSE);

	// 深圳指数
	sFileName	= GetRootPath();
	sFileName	+= self_sz_code;
	LoadCodeTable(container, sFileName, CStock::marketSZSE);
	
	if (container.GetSize() <= 0)
		CQianlong::LoadCodetable(container);

	return container.GetSize();
}

int	CSelfDB::StoreCodetable(CStockContainer& container)
{
	if (!m_bIsOK)	return 0;

	if (container.GetSize() <= 0)
		return 0;

	// 上海指数
	CSPString	sFileNameSH	= GetRootPath();
	sFileNameSH	+= self_sh_code;

	// 深圳指数
	CSPString	sFileNameSZ	= GetRootPath();
	sFileNameSZ	+= self_sz_code;
	
	CSPFile	fileSHSE, fileSZSE;
	if (!fileSHSE.Open(sFileNameSH, CSPFile::modeCreate | CSPFile::modeWrite)
		|| !fileSZSE.Open(sFileNameSZ, CSPFile::modeCreate | CSPFile::modeWrite))
	{
		return 0;
	}

	container.Lock();
	for (int i=0; i<container.GetSize(); i++)
	{
		CStockInfo & info = container.ElementAt(i);
		CSPString	str;

		str.Format("%s,%s,%d,%s,\"%s\"\r\n", info.GetStockCode(), info.GetStockNameChs(), info.GetType(),
						info.GetStockShortName(), info.GetStockNameEnu());

		if (info.IsShangHai())
		{
			fileSHSE.Write(str, str.GetLength());
		}
		if (info.IsShenZhen())
		{
			fileSZSE.Write(str, str.GetLength());
		}
	}
	container.UnLock();

	return container.GetSize();
}

int CSelfDB::LoadKData(CStock* pstock, int nKType)
{
	// WILLCHECK
	int ret = CQianlong::LoadKData(pstock, nKType);

	if (CKData::ktypeMin5 == nKType && LoadMinute(pstock) > 0)
	{
		CKData	& kdataMin5 = pstock->GetKDataMin5();
		CMinute & minute = pstock->GetMinute();
		CKData kdataNew;
		if (minute.ToKData(kdataNew) && CKData::ktypeMin5 == kdataNew.GetKType())
		{
			kdataMin5.MergeKData(&kdataNew);
		}

		return kdataMin5.GetSize();
	}
	return ret;
}

int CSelfDB::LoadBasetable(CStockContainer& container)
{
	container.Lock();

	CSPMapStringToPtr	map;
	map.InitHashTable(container.GetSize() + container.GetSize() + 100);

	for (int i=0; i<container.GetSize(); i++)
	{
		CStockInfo	& info	= container.ElementAt(i);
		map.SetAt(info.GetStockCode(), (void *)i);
		info.m_basedata.RemoveAll();
	}
	
	CSPString	sFileName	= GetRootPath();
	sFileName	+= self_chna_basetbl;
	CSPFile	file;
	if (file.Open(sFileName, CSPFile::modeRead | CSPFile::shareDenyNone))
	{
		BASEDATA	block;
		while(sizeof(block) == file.Read(&block, sizeof(block)))
		{
			void * pArrayID	= NULL;
			if (map.Lookup(block.m_szCode, pArrayID))
			{
				int	nIndex	= ((int)pArrayID);
				if (nIndex >= 0 && nIndex < container.GetSize())
				{
					container.ElementAt(nIndex).m_basedata.InsertBaseDataSort(block);
				}
			}
		}
		file.Close();
	}

	container.UnLock();
	return container.GetSize();
}

int CSelfDB::StoreBasetable(CStockContainer& container)
{
	SP_ASSERT(m_bIsOK && container.GetSize() > 0);
	if (!m_bIsOK || container.GetSize() <= 0)	return 0;

	int	nCount	= 0;
	CSPString	sRoot		= GetRootPath();
	CSPString	sFileName	= sRoot + self_chna_basetbl;

	container.Lock();
	CSPFile	file;
	if (file.Open(sFileName, CSPFile::modeWrite | CSPFile::modeCreate))
	{
		for (int i=0; i<container.GetSize(); i++)
		{
			CStockInfo	& info = container.ElementAt(i);
			for (int k=0; k<info.m_basedata.GetSize(); k++)
			{
				BASEDATA & block = info.m_basedata.ElementAt(k);
				if (k == info.m_basedata.GetSize() - 1)
				{
					block.m_fYield_average	= info.m_fYield_average;
					block.m_fYield_stddev	= info.m_fYield_stddev;
					block.m_fBeite			= info.m_fBeite;
				}
				file.Write(&block, sizeof(block));
			}
			nCount	++;
		}
	}

	container.UnLock();
	return nCount;
}

int CSelfDB::LoadDRData(CStock* pstock)
{
	SP_ASSERT(m_bIsOK && pstock && pstock->GetStockInfo().IsValidStock());
	if (!m_bIsOK || !pstock || !pstock->GetStockInfo().IsValidStock())	return 0;

	CSPString	sFileName;
	GetFileName(sFileName, CStock::dataDR, &(pstock->GetStockInfo()));
	
	CDRData	&	drdata = pstock->GetDRData();

	DWORD	dwMarket	= pstock->GetStockInfo().GetMarket();
	CSPString	sCode	= pstock->GetStockCode();
	CSPFile	file;
	if (file.Open(sFileName, CSPFile::modeRead | CSPFile::shareDenyNone))
	{
		DWORD dwLen = file.GetLength();
		CLK_DRDATA	dbdr;
		int	nSize	= dwLen/sizeof(dbdr);
		drdata.SetSize(0, nSize);

		while(sizeof(dbdr) == file.Read(&dbdr, sizeof(dbdr)))
		{
			DRDATA	dr;
			if (convert_CLK_DRDATA_to_DRDATA(dwMarket, sCode, &dbdr, &dr))
				drdata.InsertDRDataSort(dr);
		}

		file.Close();
		return drdata.GetSize();
	}

	// Load From FXJ_DRDATA
	sFileName	= GetRootPath();
	sFileName	+= self_chna_xdr;
	WORD	wFxjMarket	= 'HS';
	if (CStock::marketSZSE == dwMarket)
		wFxjMarket	= 'ZS';
	if (file.Open(sFileName, CSPFile::modeRead | CSPFile::shareDenyNone))
	{
		if (8 == file.Seek(8, CSPFile::begin))
		{
			BOOL	bMy	= FALSE;
			FXJ_DRDATA	fxjdr;
			while(sizeof(fxjdr) == file.Read(&fxjdr, sizeof(fxjdr)))
			{
				BOOL	bMyOld	= bMy;
				if (-1 == fxjdr.m_dwMagic)
					bMy	= (fxjdr.m_wMarket == wFxjMarket && 0 == strncmp(sCode, fxjdr.m_szCode, min(sizeof(fxjdr.m_szCode),sCode.GetLength())));
				else if (bMy)
				{
					DRDATA	dr;
					if (convert_FXJ_DRDATA_to_DRDATA(dwMarket, sCode, &fxjdr, &dr))
						drdata.InsertDRDataSort(dr);
				}
				if (bMyOld && !bMy)
					break;
			}
		}
	}

	return drdata.GetSize();
}

int CSelfDB::StoreDRData(CStock* pstock)
{
	SP_ASSERT(m_bIsOK && pstock && pstock->GetStockInfo().IsValidStock());
	if (!m_bIsOK || !pstock || !pstock->GetStockInfo().IsValidStock())	return 0;

	CSPString	sFileName;
	GetFileName(sFileName, CStock::dataDR, &(pstock->GetStockInfo()));
	
	CDRData	&	drdata = pstock->GetDRData();

	if (drdata.GetSize() == 0)
	{
		::DeleteFile(sFileName);
		return 0;
	}

	// Compare
	CStock	stocktemp;
	stocktemp.SetStockInfo(&(pstock->GetStockInfo()));
	if (LoadDRData(&stocktemp) > 0 && stocktemp.GetDRData().IsSameAs(&drdata))
		return drdata.GetSize();

	// Store
	CSPFile	file;
	if (file.Open(sFileName, CSPFile::modeCreate | CSPFile::modeWrite | CSPFile::shareDenyNone))
	{
		for (int i=0; i<drdata.GetSize(); i++)
		{
			CLK_DRDATA	dbdr;
			convert_DRDATA_to_CLK_DRDATA(&(drdata.ElementAt(i)), &dbdr);
			file.Write(&dbdr, sizeof(dbdr));
		}
	}

	return drdata.GetSize();
}

DWORD ReadReport(CStock* pstock, LPCTSTR lpszFileName)
{
	SP_ASSERT(pstock && pstock->GetStockInfo().IsValidStock());
	if (!pstock || !pstock->GetStockInfo().IsValidStock())
		return 0;

	CReport& aReport = pstock->GetReport();
	aReport.RemoveAll();

	CTSKFile file;
	if (file.Open(lpszFileName))
	{
		DWORD dwDataRecordCount	= file.GetDataRecordCount(pstock->GetStockInfo().GetMarket(), pstock->GetStockCode());
		aReport.SetSize(dwDataRecordCount);
		DWORD dwCount = file.LoadDataRecord(pstock->GetStockInfo().GetMarket(), pstock->GetStockCode(), aReport.GetData(), sizeof(REPORT), dwDataRecordCount);
		aReport.SetSize(dwCount);

		aReport.Sort();
		int nOldSize = aReport.GetSize();
		aReport.RemoveDirty();
		if (nOldSize - aReport.GetSize() > 50)
		{
			SP_VERIFY(aReport.GetSize() == (int)file.StoreDataRecord(pstock->GetStockInfo().GetMarket(), pstock->GetStockCode(),
				aReport.GetData(), sizeof(REPORT), aReport.GetSize(), TRUE));
		}
	}

	return aReport.GetSize();
}

int CSelfDB::LoadReport(CStock* pstock)
{
	SP_ASSERT(m_bIsOK && pstock && pstock->GetStockInfo().IsValidStock());
	if (!m_bIsOK || !pstock || !pstock->GetStockInfo().IsValidStock())
		return 0;

	pstock->GetReport().RemoveAll();

	CSPString sFileName;
	SP_VERIFY(GetFileName(sFileName, CStock::dataReport, &(pstock->GetStockInfo())));
	ReadReport(pstock, sFileName);

	return pstock->GetReport().GetSize();
}

int CSelfDB::LoadMinute(CStock* pstock)
{
	SP_ASSERT(m_bIsOK && pstock && pstock->GetStockInfo().IsValidStock());
	if (!m_bIsOK || !pstock || !pstock->GetStockInfo().IsValidStock())
		return 0;

	pstock->GetMinute().RemoveAll();

	CSPString sFileName;
	SP_VERIFY(GetFileName(sFileName, CStock::dataMinute, &(pstock->GetStockInfo())));
	ReadMinute(pstock, sFileName);

	return pstock->GetMinute().GetSize();
}

int CSelfDB::LoadOutline(CStock* pstock)
{
	SP_ASSERT(m_bIsOK && pstock && pstock->GetStockInfo().IsValidStock());
	if (!m_bIsOK || !pstock || !pstock->GetStockInfo().IsValidStock())	return 0;

	COutline	& aOutline	= pstock->GetOutline();
	aOutline.RemoveAll();

	CSPString	sFileName;
	SP_VERIFY(GetFileName(sFileName, CStock::dataOutline, &(pstock->GetStockInfo())));

	CSPFile		file;
	if (file.Open(sFileName, CSPFile::modeRead))
	{
		OUTLINE outline;
		aOutline.SetSize(aOutline.GetSize(), file.GetLength()/sizeof(outline) + 10);
		while(sizeof(outline) == file.Read(&outline,sizeof(outline)))
			aOutline.Add(outline);
	}

	return aOutline.GetSize();
}

int	CSelfDB::StoreReport(REPORT* pReport, int nCount, BOOL bBigTrade)
{
	SP_ASSERT(m_bIsOK && pReport && nCount > 0);
	if (!m_bIsOK || !pReport || nCount <= 0)
		return 0;

	static BOOL	bFirstCall	= TRUE;
	if (bFirstCall)
	{
		bFirstCall	= FALSE;

		// remove old
		CSPString	sRoot	= GetRootPath();
		CTSKFile::RemoveFileIfOutoftime(sRoot+self_sh_report, pReport->m_time);
		CTSKFile::RemoveFileIfOutoftime(sRoot+self_sz_report, pReport->m_time);
	}

	CSPString	sFileName;
	CTSKFile	file;
	DWORD	dwStoreCount	= 0;
	for (int i=0; i<nCount; i++)
	{
		CStockInfo	info;
		CSPString	sFileNameNew;
		if (!info.SetStockCode(pReport[i].m_dwMarket, pReport[i].m_szCode)
			|| !GetFileName(sFileNameNew, CStock::dataReport, &info))
			continue;

		if (0 != sFileNameNew.Compare(sFileName))
		{
			sFileName	= sFileNameNew;
			SP_VERIFY(CTSKFile::BuildEmptyFile(sFileName, CStock::dataReport, pReport->m_dwMarket, 5000, 125, FALSE));
			SP_VERIFY(file.Open(sFileName));
		}

		// store
		if (bBigTrade)
		{
			dwStoreCount += file.StoreDataRecord(	pReport[i].m_dwMarket, STKLIB_CODE_ZLDD,
													&(pReport[i]), sizeof(REPORT), 1, FALSE);
		}
		else
		{
			dwStoreCount += file.StoreDataRecord(	pReport[i].m_dwMarket, pReport[i].m_szCode,
													&(pReport[i]), sizeof(REPORT), 1, FALSE);
		}
	}

	return dwStoreCount;
}

int	CSelfDB::StoreMinute(MINUTE* pMinute, int nCount)
{
	SP_ASSERT(m_bIsOK && pMinute && nCount > 0);
	if (!m_bIsOK || !pMinute || nCount <= 0)
		return 0;

	static BOOL bFirstCall = TRUE;
	if (bFirstCall)
	{
		bFirstCall = FALSE;

		CSPString sRoot = GetRootPath();
		CTSKFile::RemoveFileIfOutoftime(sRoot + self_sh_minute, pMinute->m_time);
		CTSKFile::RemoveFileIfOutoftime(sRoot + self_sz_minute, pMinute->m_time);
	}

	CSPString sFileName;
	CTSKFile file;
	DWORD dwStoreCount = 0;
	CMinute aMinute;
	for (int i = 0; i < nCount; i++)
	{
		CStockInfo info;
		CSPString sFileNameNew;
		if (!info.SetStockCode(pMinute[i].m_dwMarket, pMinute[i].m_szCode)
			|| !GetFileName(sFileNameNew, CStock::dataMinute, &info))
			continue;

		if (sFileNameNew.Compare(sFileName) != 0)
		{
			sFileName = sFileNameNew;
			SP_VERIFY(CTSKFile::BuildEmptyFile(sFileName, CStock::dataMinute, pMinute->m_dwMarket, 5000, 250, FALSE));
			SP_VERIFY(file.Open(sFileName));
		}

		if (aMinute.GetSize() > 0 && (aMinute[0].m_dwMarket != pMinute[i].m_dwMarket
			|| strncmp(aMinute[0].m_szCode,pMinute[i].m_szCode, sizeof(pMinute[i].m_szCode))) != 0)
		{
			dwStoreCount++;

			file.StoreDataRecord(aMinute[0].m_dwMarket, aMinute[0].m_szCode, aMinute.GetData(), sizeof(MINUTE), aMinute.GetSize(), TRUE);
			aMinute.RemoveAll();
		}

		aMinute.Add(pMinute[i]);
	}

	if (aMinute.GetSize() > 0)
	{
		dwStoreCount++;

		file.StoreDataRecord(aMinute[0].m_dwMarket, aMinute[0].m_szCode, aMinute.GetData(), sizeof(MINUTE), aMinute.GetSize(), TRUE);
		aMinute.RemoveAll();
	}

	return dwStoreCount;
}

int	CSelfDB::StoreOutline(OUTLINE * pOutline, int nCount)
{
	SP_ASSERT(m_bIsOK && pOutline && nCount > 0);
	if (!m_bIsOK || !pOutline || nCount <= 0)	return 0;

	CSPString	sFileName;
	SP_VERIFY(GetFileName(sFileName,CStock::dataOutline,NULL));
	
	CSPFile	file;
	if (file.Open(sFileName, CSPFile::modeCreate | CSPFile::modeWrite))
	{
		file.Write(pOutline, sizeof(OUTLINE)*nCount);
		file.Close();
		return nCount;
	}
	return 0;
}

BOOL CSelfDB::GetAccurateRoot(const char * rootpath, char *accurateroot, int maxlen)
{
	if (0 == rootpath || strlen(rootpath)==0)
		return FALSE;

	// get rootpath
	CSPString	strRoot	= rootpath;
	int nLen = strRoot.GetLength();
	if (strRoot[nLen-1] != '\\' && strRoot[nLen-1] != '/')
		strRoot	+= CHAR_DIRSEP;
	nLen = strRoot.GetLength();

	if (0 != access(strRoot + ml_dat, 0))
		return FALSE;
	if (0 != access(strRoot + ml_data, 0))
		return FALSE;
	if (0 != access(strRoot + ml_sh, 0))
		return FALSE;
	if (0 != access(strRoot + ml_sh_base, 0))
		return FALSE;
	if (0 != access(strRoot + ml_sh_month, 0))
		return FALSE;
	if (0 != access(strRoot + ml_sh_week, 0))
		return FALSE;
	if (0 != access(strRoot + ml_sh_day, 0))
		return FALSE;
	if (0 != access(strRoot + ml_sh_min, 0))
		return FALSE;
	if (0 != access(strRoot + self_sh_xdr, 0))
		return FALSE;
	if (0 != access(strRoot + self_news, 0))
		return FALSE;

	strncpy(accurateroot, strRoot, maxlen-1);
	accurateroot[maxlen-1]	= '\0';

	return TRUE;
}

int CSelfDB::InstallCodetbl(const char * filename, const char *orgname)
{
	if (NULL == filename || strlen(filename) == 0
		|| NULL == orgname || strlen(orgname) == 0)
		return 0;

	CSPString	sFileName	= GetRootPath();

	CSPString	sOrgName = "dat\\";
	sOrgName	+= orgname;
	if (0 == sOrgName.CompareNoCase(ml_sh_info)
		|| 0 == sOrgName.CompareNoCase(ml_sz_info)
		|| 0 == sOrgName.CompareNoCase(ml_sh_now)
		|| 0 == sOrgName.CompareNoCase(ml_sz_now)
		|| 0 == sOrgName.CompareNoCase(ml_sh_pyjc)
		|| 0 == sOrgName.CompareNoCase(ml_sz_pyjc)
		|| 0 == sOrgName.CompareNoCase(ml_sh_trace)
		|| 0 == sOrgName.CompareNoCase(ml_sz_trace)
		|| 0 == sOrgName.CompareNoCase(ml_sh_minute)
		|| 0 == sOrgName.CompareNoCase(ml_sz_minute))
		sFileName	+= ml_dat;
	else
		sFileName	+= ml_data;

	sFileName	+= orgname;

	return CSPFile::CopyFile(filename, sFileName, FALSE);
}

int CSelfDB::InstallCodetblBlock(const char * filename, const char *orgname)
{
	if (NULL == filename || strlen(filename) == 0
		|| NULL == orgname || strlen(orgname) == 0)
		return 0;

	int nRet = 0;
	CDomainContainer newdomains;
	if (newdomains.Load(filename))
	{
		nRet = AfxGetDomainContainer().AddDomainReplace(newdomains);
		AfxGetDomainContainer().Store(AfxGetProfile().GetDomainFile());
	}
	return nRet;
}

int CSelfDB::InstallCodetblFxjBlock(const char * filename, const char *orgname)
{
	if (NULL == filename || strlen(filename) == 0
		|| NULL == orgname || strlen(orgname) == 0)
		return 0;

	CSPString	sName	= orgname;
	int nIndex = sName.Find('.');
	if (-1 != nIndex)
		sName	= sName.Left(nIndex);
	CDomain	domain;
	domain.m_strName = sName;
	int nRet = domain.AddFxjDomain(filename);
	if (nRet > 0)
	{
		AfxGetDomainContainer().AddDomainReplace(domain);
	}
	return nRet;
}

int CSelfDB::InstallDRData(CDRData & drdata)
{
	if (drdata.GetSize() <= 0)
		return 0;

	CSPString	sFileName;
	CStockInfo	stockinfo;
	DWORD	dwMarket	= drdata.ElementAt(0).m_dwMarket;
	CSPString	sCode	= drdata.ElementAt(0).m_szCode;
	stockinfo.SetStockCode(dwMarket, sCode);
	if (!GetFileName(sFileName, CStock::dataDR, &stockinfo))
		return 0;

	int	nCount	= 0;

	CSPFile	fileTo;
	if (fileTo.Open(sFileName, CSPFile::modeCreate | CSPFile::modeNoTruncate | CSPFile::modeReadWrite))
	{
		for (int i=0; i<drdata.GetSize(); i++)
		{
			CLK_DRDATA	dbdrnew;
			convert_DRDATA_to_CLK_DRDATA(&(drdata.ElementAt(i)), &dbdrnew);
			
			fileTo.SeekToBegin();
			CLK_DRDATA	dbdr;
			memset(&dbdr, 0, sizeof(dbdr));
			BOOL	bHas	= FALSE;
			BOOL	bInsert	= FALSE;
			while(sizeof(dbdr) == fileTo.Read(&dbdr, sizeof(dbdr)))
			{
				if (dbdr.date == dbdrnew.date)
				{
					bHas	= TRUE;
					fileTo.Seek(fileTo.GetPosition()-sizeof(dbdr), CSPFile::begin);
					break;
				}
				if (dbdr.date > dbdrnew.date)
				{
					bInsert	= TRUE;
					break;
				}
			}
			if (bHas || !bInsert)
				fileTo.Write(&dbdrnew, sizeof(dbdrnew));
			else if (bInsert)
			{
				int nCur = fileTo.GetPosition();
				int nLen = fileTo.GetLength();
				BYTE * pbuffer = new BYTE[nLen-nCur+1];
				if (pbuffer)
				{
					if (nLen - nCur > 0)	fileTo.Read(pbuffer, nLen-nCur);
					fileTo.Seek(nCur-sizeof(dbdr), CSPFile::begin);
					fileTo.Write(&dbdrnew, sizeof(dbdrnew));
					fileTo.Write(&dbdr, sizeof(dbdr));
					if (nLen - nCur > 0)	fileTo.Write(pbuffer, nLen-nCur);
					delete [] pbuffer;
				}
			}
			nCount	++;
			fileTo.Flush();
		}
		fileTo.Close();
	}
	return nCount;
}

int CSelfDB::InstallDRDataClk(const char * filename, const char *orgname)
{
	if (NULL == filename || strlen(filename) == 0
		|| NULL == orgname || strlen(orgname) == 0)
		return 0;

	CSPString	sFileName;
	CSPString	sCode	= orgname;
	int nIndex = sCode.Find('.');
	if (-1 != nIndex)
		sCode	= sCode.Left(nIndex);

	CStockInfo	info;
	info.SetStockCode(CStock::marketUnknown, sCode);
	DWORD dwMarket = info.GetMarket();

	CDRData	drdata;
	CSPFile	file;
	if (file.Open(filename, CSPFile::modeRead))
	{
		CLK_DRDATA	dbdr;
		while(sizeof(dbdr) == file.Read(&dbdr,sizeof(dbdr)))
		{
			DRDATA	dr;
			convert_CLK_DRDATA_to_DRDATA(dwMarket, sCode, &dbdr, &dr);
			drdata.Add(dr);
		}
		file.Close();
	}
	return InstallDRData(drdata);
}

int CSelfDB::InstallDRDataFxj(const char * fxjfilename)
{
	if (NULL == fxjfilename || strlen(fxjfilename) == 0)
		return 0;

	CSPString	sFileName	= GetRootPath();
	sFileName	+= self_chna_xdr;
	
	return CSPFile::CopyFile(fxjfilename, sFileName, FALSE);
}

int CSelfDB::InstallBasetable(const char * filename, const char * orgname)
{
	if (NULL == filename || strlen(filename) == 0
		|| NULL == orgname || strlen(orgname) == 0)
		return 0;
	
	CSPString	sFileName	= GetRootPath();
	sFileName	+= ml_data;
	sFileName	+= orgname;

	return CSPFile::CopyFile(filename, sFileName, FALSE);
}

int CSelfDB::InstallBasetableTdx(const char * filename)
{
	if (NULL == filename || strlen(filename) == 0)
		return 0;
	
	int	nCount	= 0;

	CSPString	sRoot		= GetRootPath();
	CSPString	sFileNameSHSZ	= sRoot + self_chna_basetbl;
	CSPFile	fileSHSZ, fileTdx;
	if (fileSHSZ.Open(sFileNameSHSZ, CSPFile::modeWrite | CSPFile::modeCreate)
		&& fileTdx.Open(filename, CSPFile::modeRead))
	{
		fileTdx.Seek(TDX_FHEADER_SIZE, CSPFile::begin);

		TDX_BASEDATA	tdxblock;
		while(sizeof(tdxblock) == fileTdx.Read(&tdxblock, sizeof(tdxblock)))
		{
			BASEDATA	block;
			if (convert_TDX_BASEDATA_to_BASEDATA(&tdxblock, &block))
			{
				fileSHSZ.Write(&block, sizeof(block));
				nCount	++;
			}
		}
	}

	return nCount;
}

int CSelfDB::InstallBasetableFxj(const char * filename)
{
	if (NULL == filename || strlen(filename) == 0)
		return 0;

	DWORD	dwDate = CSPTime::GetCurrentTime().ToStockTimeDay();
	{
		CSPString	sTemp = filename;
		int index1 = sTemp.ReverseFind('/');
		int index2 = sTemp.ReverseFind('\\');
		int index = max(index1,index2);
		if (index > 0)
			sTemp = sTemp.Mid(index+1);
		DWORD	dwTemp = atol(sTemp);
		CSPTime	sptime;
		if (sptime.FromStockTimeDay(dwTemp))
			dwDate = dwTemp;
	}
	
	int	nCount	= 0;

	CSPString	sRoot		= GetRootPath();
	CSPString	sFileNameSHSZ	= sRoot + self_chna_basetbl;
	CSPFile	fileSHSZ, fileFxj;
	if (fileSHSZ.Open(sFileNameSHSZ, CSPFile::modeWrite | CSPFile::modeCreate)
		&& fileFxj.Open(filename, CSPFile::modeRead))
	{
		fileFxj.Seek(FXJ_FHEADER_SIZE, CSPFile::begin);

		FXJ_BASEDATA	fxjblock;
		while(sizeof(fxjblock) == fileFxj.Read(&fxjblock, sizeof(fxjblock)))
		{
			BASEDATA	block;
			if (convert_FXJ_BASEDATA_to_BASEDATA(&fxjblock, &block, dwDate))
			{
				fileSHSZ.Write(&block, sizeof(block));
				nCount	++;
			}
		}
	}

	return nCount;
}

int CSelfDB::InstallNewsText(const char * filename, const char *orgname)
{
	if (NULL == filename || strlen(filename) == 0
		|| NULL == orgname || strlen(orgname) == 0)
		return 0;
	
	CSPString	sFileName	= GetRootPath();
	sFileName	+= self_news;
	sFileName	+= orgname;

	return CSPFile::CopyFile(filename, sFileName, FALSE);
}

int CSelfDB::InstallNewsText(const char * buffer, int nLen, const char *orgname)
{
	if (NULL == buffer || nLen <= 0
		|| NULL == orgname || strlen(orgname) == 0)
		return 0;
	
	CSPString	sFileName	= GetRootPath();
	sFileName	+= self_news;
	sFileName	+= orgname;

	CSPFile	file;
	if (file.Open(sFileName,CSPFile::modeWrite|CSPFile::modeCreate))
	{
		file.Write(buffer, nLen);
		file.Close();
		return 1;
	}
	return 0;
}
