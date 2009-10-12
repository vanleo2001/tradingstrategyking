/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	CTechParameters;
		class	CColorRefContainer;
		class	CColorClass;
		class	CColorClassContainer;
		class	CStProfile;
*/

#include "stdafx.h"
#include "../Include/Profile.h"
#include "SelfDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CTechParameters
CTechParameters::CTechParameters( )
{
}

void CTechParameters::Clear( )
{
	ma.Clear( );
	bbi.Clear( );
	boll.Clear( );
	pv.Clear( );
	sar.Clear( );
	dj.Clear( );
	cw.Clear( );

	macd.Clear( );
	mike.Clear( );
	psy.Clear( );

	volume.Clear( );
	nvi.Clear( );
	pvi.Clear( );
	vr.Clear( );
	vroc.Clear( );
	obv.Clear( );
	mobv.Clear( );
	mfi.Clear( );
	vmacd.Clear( );
	wvad.Clear( );
	emv.Clear( );
	vrsi.Clear( );
	nvrsi.Clear( );
	ad.Clear( );
	ci.Clear( );

	kdj.Clear( );
	r.Clear( );
	rsi.Clear( );
	bias.Clear( );
	mtm.Clear( );
	dmi.Clear( );
	roc.Clear( );
	cci.Clear( );
	cv.Clear( );
	arbr.Clear( );
	cr.Clear( );
	osc.Clear( );
	uos.Clear( );
	maosc.Clear( );
	bias36.Clear( );
	dpo.Clear( );
	kst.Clear( );

	rei.Clear( );
	dmki.Clear( );
	pcnt.Clear( );
	hlc.Clear( );
	cdp.Clear( );
	asi.Clear( );
	atr.Clear( );

	cyo.Clear( );
	dcyo.Clear( );
	hsl.Clear( );
	dper.Clear( );
}

void CTechParameters::SetKData( CKData * pKData )
{
	ma.SetKData( pKData );
	bbi.SetKData( pKData );
	boll.SetKData( pKData );
	pv.SetKData( pKData );
	sar.SetKData( pKData );
	dj.SetKData( pKData );
	cw.SetKData( pKData );

	macd.SetKData( pKData );
	mike.SetKData( pKData );
	psy.SetKData( pKData );

	volume.SetKData( pKData );
	nvi.SetKData( pKData );
	pvi.SetKData( pKData );
	vr.SetKData( pKData );
	vroc.SetKData( pKData );
	obv.SetKData( pKData );
	mobv.SetKData( pKData );
	mfi.SetKData( pKData );
	vmacd.SetKData( pKData );
	wvad.SetKData( pKData );
	emv.SetKData( pKData );
	vrsi.SetKData( pKData );
	nvrsi.SetKData( pKData );
	ad.SetKData( pKData );
	ci.SetKData( pKData );

	kdj.SetKData( pKData );
	r.SetKData( pKData );
	rsi.SetKData( pKData );
	bias.SetKData( pKData );
	mtm.SetKData( pKData );
	dmi.SetKData( pKData );
	roc.SetKData( pKData );
	cci.SetKData( pKData );
	cv.SetKData( pKData );
	arbr.SetKData( pKData );
	cr.SetKData( pKData );
	osc.SetKData( pKData );
	uos.SetKData( pKData );
	maosc.SetKData( pKData );
	bias36.SetKData( pKData );
	dpo.SetKData( pKData );
	kst.SetKData( pKData );

	rei.SetKData( pKData );
	dmki.SetKData( pKData );
	pcnt.SetKData( pKData );
	hlc.SetKData( pKData );
	cdp.SetKData( pKData );
	asi.SetKData( pKData );
	atr.SetKData( pKData );

	cyo.SetKData( pKData );
	dcyo.SetKData( pKData );
	hsl.SetKData( pKData );
	dper.SetKData( pKData );
}

void CTechParameters::SetDefaultParametersAll( )
{
	ma.SetDefaultParameters( );
	bbi.SetDefaultParameters( );
	boll.SetDefaultParameters( );
	pv.SetDefaultParameters( );
	sar.SetDefaultParameters( );
	dj.SetDefaultParameters( );
	cw.SetDefaultParameters( );

	macd.SetDefaultParameters( );
	mike.SetDefaultParameters( );
	psy.SetDefaultParameters( );

	volume.SetDefaultParameters( );
	nvi.SetDefaultParameters( );
	pvi.SetDefaultParameters( );
	vr.SetDefaultParameters( );
	vroc.SetDefaultParameters( );
	obv.SetDefaultParameters( );
	mobv.SetDefaultParameters( );
	mfi.SetDefaultParameters( );
	vmacd.SetDefaultParameters( );
	wvad.SetDefaultParameters( );
	emv.SetDefaultParameters( );
	vrsi.SetDefaultParameters( );
	nvrsi.SetDefaultParameters( );
	ad.SetDefaultParameters( );
	ci.SetDefaultParameters( );

	kdj.SetDefaultParameters( );
	r.SetDefaultParameters( );
	rsi.SetDefaultParameters( );
	bias.SetDefaultParameters( );
	mtm.SetDefaultParameters( );
	dmi.SetDefaultParameters( );
	roc.SetDefaultParameters( );
	cci.SetDefaultParameters( );
	cv.SetDefaultParameters( );
	arbr.SetDefaultParameters( );
	cr.SetDefaultParameters( );
	osc.SetDefaultParameters( );
	uos.SetDefaultParameters( );
	maosc.SetDefaultParameters( );
	bias36.SetDefaultParameters( );
	dpo.SetDefaultParameters( );
	kst.SetDefaultParameters( );

	rei.SetDefaultParameters( );
	dmki.SetDefaultParameters( );
	pcnt.SetDefaultParameters( );
	hlc.SetDefaultParameters( );
	cdp.SetDefaultParameters( );
	asi.SetDefaultParameters( );
	atr.SetDefaultParameters( );

	cyo.SetDefaultParameters( );
	dcyo.SetDefaultParameters( );
	hsl.SetDefaultParameters( );
	dper.SetDefaultParameters( );

	AfxGetProfile().StoreProfile( );
}

BOOL CTechParameters::FindParameters( UINT nTech, CTechnique * pTech )
{
	switch( nTech )
	{
		case STT_MA:		((CMA *)pTech)->AttachParameters( ma );		break;
		case STT_BBI:		((CBBI *)pTech)->AttachParameters( bbi );		break;
		case STT_BOLL:		((CBOLL *)pTech)->AttachParameters( boll );		break;
		case STT_PV:		((CPV *)pTech)->AttachParameters( pv );		break;
		case STT_SAR:		((CSAR *)pTech)->AttachParameters( sar );		break;
		case STT_DJ:		((CDJ *)pTech)->AttachParameters( dj );		break;
		case STT_CW:		((CCW *)pTech)->AttachParameters( cw );		break;
		
		case STT_MACD:		((CMACD *)pTech)->AttachParameters( macd );		break;
		case STT_MIKE:		((CMIKE *)pTech)->AttachParameters( mike );		break;
		case STT_PSY:		((CPSY *)pTech)->AttachParameters( psy );		break;

		case STT_VOLUME:	((CVOLUME *)pTech)->AttachParameters( volume );		break;
		case STT_NVI:		((CNVI *)pTech)->AttachParameters( nvi );		break;
		case STT_PVI:		((CPVI *)pTech)->AttachParameters( pvi );		break;
		case STT_VR:		((CVR *)pTech)->AttachParameters( vr );		break;
		case STT_VROC:		((CVROC *)pTech)->AttachParameters( vroc );		break;
		case STT_OBV:		((COBV *)pTech)->AttachParameters( obv );		break;
		case STT_MOBV:		((CMOBV *)pTech)->AttachParameters( mobv );		break;
		case STT_MFI:		((CMFI *)pTech)->AttachParameters( mfi );		break;
		case STT_VMACD:		((CVMACD *)pTech)->AttachParameters( vmacd );		break;
		case STT_WVAD:		((CWVAD *)pTech)->AttachParameters( wvad );		break;
		case STT_EMV:		((CEMV *)pTech)->AttachParameters( emv );		break;
		case STT_VRSI:		((CVRSI *)pTech)->AttachParameters( vrsi );		break;
		case STT_NVRSI:		((CNVRSI *)pTech)->AttachParameters( nvrsi );		break;
		case STT_AD:		((CAD *)pTech)->AttachParameters( ad );		break;
		case STT_CI:		((CCI *)pTech)->AttachParameters( ci );		break;

		case STT_KDJ:		((CKDJ *)pTech)->AttachParameters( kdj );		break;
		case STT_R:			((CR *)pTech)->AttachParameters( r );		break;
		case STT_RSI:		((CRSI *)pTech)->AttachParameters( rsi );		break;
		case STT_BIAS:		((CBIAS *)pTech)->AttachParameters( bias );		break;
		case STT_MTM:		((CMTM *)pTech)->AttachParameters( mtm );		break;
		case STT_DMI:		((CDMI *)pTech)->AttachParameters( dmi );		break;
		case STT_ROC:		((CROC *)pTech)->AttachParameters( roc );		break;
		case STT_CCI:		((CCCI *)pTech)->AttachParameters( cci );		break;
		case STT_CV:		((CCV *)pTech)->AttachParameters( cv );		break;
		case STT_ARBR:		((CARBR *)pTech)->AttachParameters( arbr );		break;
		case STT_CR:		((CCR *)pTech)->AttachParameters( cr );		break;
		case STT_OSC:		((COSC *)pTech)->AttachParameters( osc );		break;
		case STT_UOS:		((CUOS *)pTech)->AttachParameters( uos );		break;
		case STT_MAOSC:		((CMAOSC *)pTech)->AttachParameters( maosc );		break;
		case STT_36BIAS:	((C36BIAS *)pTech)->AttachParameters( bias36 );		break;
		case STT_DPO:		((CDPO *)pTech)->AttachParameters( dpo );		break;
		case STT_KST:		((CKST *)pTech)->AttachParameters( kst );		break;

		case STT_REI:		((CREI *)pTech)->AttachParameters( rei );		break;
		case STT_DMKI:		((CDMKI *)pTech)->AttachParameters( dmki );		break;
		case STT_PCNT:		((CPCNT *)pTech)->AttachParameters( pcnt );		break;
		case STT_HLC:		((CHLC *)pTech)->AttachParameters( hlc );		break;
		case STT_CDP:		((CCDP *)pTech)->AttachParameters( cdp );		break;
		case STT_ASI:		((CASI *)pTech)->AttachParameters( asi );		break;
		case STT_ATR:		((CATR *)pTech)->AttachParameters( atr );		break;

		case STT_CYO:		((CCYO *)pTech)->AttachParameters( cyo );		break;
		case STT_DCYO:		((CDCYO *)pTech)->AttachParameters( dcyo );		break;
		case STT_HSL:		((CHSL *)pTech)->AttachParameters( hsl );		break;
		case STT_DPER:		((CDPER *)pTech)->AttachParameters( dper );		break;

		default:
			SP_ASSERT( FALSE );
			return FALSE;
	}
	return TRUE;
}

void CTechParameters::Serialize( CSPArchive &ar )
{
	if( ar.IsStoring() )
	{
		// CMA
		ar << ma.m_nType;
		ma.m_adwMADays.Serialize( ar );
		ar << ma.m_itsGoldenFork;
		ar << ma.m_itsDeadFork;
		ar << ma.m_itsLong;
		ar << ma.m_itsShort;

		// CBBI
		ar << bbi.m_nMA1Days;
		ar << bbi.m_nMA2Days;
		ar << bbi.m_nMA3Days;
		ar << bbi.m_nMA4Days;
		ar << bbi.m_itsGoldenFork;
		ar << bbi.m_itsDeadFork;

		// CBOLL
		ar << boll.m_dMultiUp;
		ar << boll.m_dMultiDown;
		ar << boll.m_nMADays;
		ar << boll.m_itsSupport;
		ar << boll.m_itsResistance;

		// CPV
		
		// CSAR
		ar << sar.m_nInitDays;
		ar << sar.m_bFirstUp;
		ar << sar.m_dAFStep;
		ar << sar.m_dAFMax;
		ar << sar.m_itsBuy;
		ar << sar.m_itsSell;

		// CDJ
		ar << dj.m_strCodeSha;
		ar << dj.m_strCodeSzn;

		// CCW
		ar << cw.m_dChangeHand;

		// CMACD
		ar << macd.m_nEMA1Days;
		ar << macd.m_nEMA2Days;
		ar << macd.m_nDIFDays;
		ar << macd.m_itsDeviateOnBottom;
		ar << macd.m_itsDeviateOnTop;
		ar << macd.m_itsGoldenFork;
		ar << macd.m_itsDeadFork;

		// CMIKE
		ar << mike.m_nDays;

		// CPSY
		ar << psy.m_nDays;
		ar << psy.m_itsSold;
		ar << psy.m_itsBought;

		// CVOLUME
		volume.m_adwMADays.Serialize( ar );
		ar << volume.m_itsDeviateOnBottom;
		ar << volume.m_itsDeviateOnTop;
		ar << volume.m_itsLong;
		ar << volume.m_itsShort;

		// CNVI
		ar << nvi.m_nMADays;
		ar << nvi.m_itsGoldenFork;
		ar << nvi.m_itsDeadFork;

		// CPVI
		ar << pvi.m_nMADays;
		ar << pvi.m_itsGoldenFork;
		ar << pvi.m_itsDeadFork;

		// CVR
		ar << vr.m_nDays;
		ar << vr.m_itsLong;
		ar << vr.m_itsShort;

		// CVROC
		ar << vroc.m_nDays;
		ar << vroc.m_nMADays;
		ar << vroc.m_itsDeviateOnBottom;
		ar << vroc.m_itsDeviateOnTop;

		// COBV
		ar << obv.m_itsDeviateOnBottom;
		ar << obv.m_itsDeviateOnTop;

		// CMOBV
		ar << mobv.m_nDays1;
		ar << mobv.m_nDays2;
		ar << mobv.m_itsGoldenFork;
		ar << mobv.m_itsDeadFork;

		// CMFI
		ar << mfi.m_nDays;
		ar << mfi.m_itsLong;
		ar << mfi.m_itsShort;

		// CVMACD
		ar << vmacd.m_nEMA1Days;
		ar << vmacd.m_nEMA2Days;
		ar << vmacd.m_nDIFDays;
		ar << vmacd.m_itsDeviateOnBottom;
		ar << vmacd.m_itsDeviateOnTop;
		ar << vmacd.m_itsGoldenFork;
		ar << vmacd.m_itsDeadFork;

		// CWVAD
		ar << wvad.m_nDays;
		ar << wvad.m_itsLong;
		ar << wvad.m_itsShort;

		// CEMV
		ar << emv.m_nDays;
		ar << emv.m_nMADays;
		ar << emv.m_itsGoldenFork;
		ar << emv.m_itsDeadFork;

		// CVRSI
		ar << vrsi.m_nDays;
		ar << vrsi.m_itsDeviateOnBottom;
		ar << vrsi.m_itsDeviateOnTop;
		ar << vrsi.m_itsSold;
		ar << vrsi.m_itsBought;

		// CNVRSI
		ar << nvrsi.m_nDays;
		ar << nvrsi.m_itsDeviateOnBottom;
		ar << nvrsi.m_itsDeviateOnTop;
		ar << nvrsi.m_itsSold;
		ar << nvrsi.m_itsBought;

		// CAD
		ar << ad.m_nDays;

		// CCI

		// CKDJ
		ar << kdj.m_nRSVDays;
		ar << kdj.m_nKDays;
		ar << kdj.m_nDDays;
		ar << kdj.m_nJ;
		ar << kdj.m_itsGoldenFork;
		ar << kdj.m_itsDeadFork;

		// CR
		ar << r.m_nDays;

		// CRSI
		rsi.m_adwDays.Serialize( ar );
		ar << rsi.m_itsSold;
		ar << rsi.m_itsGoldenFork;
		ar << rsi.m_itsDeadFork;

		// CBIAS
		ar << bias.m_nDays;
		ar << bias.m_itsSold;
		ar << bias.m_itsBought;

		// CMTM
		ar << mtm.m_nDays;
		ar << mtm.m_nMADays;
		ar << mtm.m_itsGoldenFork;
		ar << mtm.m_itsDeadFork;

		// CDMI
		ar << dmi.m_nDays;

		// CROC
		ar << roc.m_nDays;
		ar << roc.m_nMADays;
		ar << roc.m_itsGoldenFork;
		ar << roc.m_itsDeadFork;

		// CCCI
		ar << cci.m_nDays;
		ar << cci.m_dQuotiety;
		ar << cci.m_nMADays;
		ar << cci.m_itsGoldenFork;
		ar << cci.m_itsDeadFork;

		// CCV
		ar << cv.m_nMAHLDays;
		ar << cv.m_nCVDays;
		ar << cv.m_itsSold;
		ar << cv.m_itsBought;

		// CARBR
		ar << arbr.m_nDays;
		ar << arbr.m_itsGoldenFork;
		ar << arbr.m_itsDeadFork;
		ar << arbr.m_itsSold;
		ar << arbr.m_itsBought;

		// CCR
		ar << cr.m_nDays;
		ar << cr.m_nMADaysA;
		ar << cr.m_nMADaysB;
		ar << cr.m_nMADaysC;
		ar << cr.m_nMADaysD;
		ar << cr.m_itsSold;
		ar << cr.m_itsBought;

		// COSC
		ar << osc.m_nDays;
		ar << osc.m_nMADays;
		ar << osc.m_itsGoldenFork;
		ar << osc.m_itsDeadFork;

		// CUOS
		ar << uos.m_nDays1;
		ar << uos.m_nDays2;
		ar << uos.m_nDays3;
		ar << uos.m_nMADays;
		ar << uos.m_itsGoldenFork;
		ar << uos.m_itsDeadFork;

		// CMAOSC
		ar << maosc.m_nDays1;
		ar << maosc.m_nDays2;
		ar << maosc.m_nMADays;
		ar << maosc.m_itsGoldenFork;
		ar << maosc.m_itsDeadFork;

		// C36BIAS
		ar << bias36.m_itsSold;
		ar << bias36.m_itsBought;

		// CDPO
		ar << dpo.m_nDays;
		ar << dpo.m_nMADays;
		ar << dpo.m_itsGoldenFork;
		ar << dpo.m_itsDeadFork;

		// CKST
		kst.m_adwROCDays.Serialize( ar );
		ar << kst.m_nMADays;
		ar << kst.m_itsGoldenFork;
		ar << kst.m_itsDeadFork;

		// CREI
		ar << rei.m_nDays;
		ar << rei.m_itsLong;
		ar << rei.m_itsShort;

		// CDMKI
		ar << dmki.m_nDays;
		ar << dmki.m_itsSold;
		ar << dmki.m_itsBought;

		// CPCNT
		ar << pcnt.m_nMADays;
		ar << pcnt.m_itsGoldenFork;
		ar << pcnt.m_itsDeadFork;

		// CHLC
		ar << hlc.m_nDays;
		ar << hlc.m_nMADays;
		ar << hlc.m_itsGoldenFork;
		ar << hlc.m_itsDeadFork;

		// CCDP
		
		// CASI
		ar << asi.m_nDays;
		ar << asi.m_itsDeviateOnBottom;
		ar << asi.m_itsDeviateOnTop;

		// CATR
		ar << atr.m_nDays;
		ar << atr.m_itsSold;
		ar << atr.m_itsBought;
		
		// CCYO
		cyo.m_adwMTMDays.Serialize( ar );
		ar << cyo.m_nMADays;
		ar << cyo.m_itsGoldenFork;
		ar << cyo.m_itsDeadFork;

		// CDCYO
		dcyo.m_adwMTMDays.Serialize( ar );
		ar << dcyo.m_nMADays;
		ar << dcyo.m_itsGoldenFork;
		ar << dcyo.m_itsDeadFork;

		// CHSL
		ar << hsl.m_nDays;
		ar << hsl.m_nMADays;
		ar << hsl.m_itsGoldenFork;
		ar << hsl.m_itsDeadFork;

		// CDPER
		ar << dper.m_nDays;
		ar << dper.m_nMADays;
		ar << dper.m_nDetrendDays;
		ar << dper.m_itsGoldenFork;
		ar << dper.m_itsDeadFork;

		DWORD	dwTemp	=	(DWORD)(-1);
		for( int i=0; i<200; i++ )
			ar << dwTemp;
	}
	else
	{
		// CMA
		ar >> ma.m_nType;
		ma.m_adwMADays.Serialize( ar );
		ar >> ma.m_itsGoldenFork;
		ar >> ma.m_itsDeadFork;
		ar >> ma.m_itsLong;
		ar >> ma.m_itsShort;

		// CBBI
		ar >> bbi.m_nMA1Days;
		ar >> bbi.m_nMA2Days;
		ar >> bbi.m_nMA3Days;
		ar >> bbi.m_nMA4Days;
		ar >> bbi.m_itsGoldenFork;
		ar >> bbi.m_itsDeadFork;

		// CBOLL
		ar >> boll.m_dMultiUp;
		ar >> boll.m_dMultiDown;
		ar >> boll.m_nMADays;
		ar >> boll.m_itsSupport;
		ar >> boll.m_itsResistance;

		// CPV
		
		// CSAR
		ar >> sar.m_nInitDays;
		ar >> sar.m_bFirstUp;
		ar >> sar.m_dAFStep;
		ar >> sar.m_dAFMax;
		ar >> sar.m_itsBuy;
		ar >> sar.m_itsSell;

		// CDJ
		ar >> dj.m_strCodeSha;
		ar >> dj.m_strCodeSzn;

		// CCW
		ar >> cw.m_dChangeHand;

		// CMACD
		ar >> macd.m_nEMA1Days;
		ar >> macd.m_nEMA2Days;
		ar >> macd.m_nDIFDays;
		ar >> macd.m_itsDeviateOnBottom;
		ar >> macd.m_itsDeviateOnTop;
		ar >> macd.m_itsGoldenFork;
		ar >> macd.m_itsDeadFork;

		// CMIKE
		ar >> mike.m_nDays;

		// CPSY
		ar >> psy.m_nDays;
		ar >> psy.m_itsSold;
		ar >> psy.m_itsBought;

		// CVOLUME
		volume.m_adwMADays.Serialize( ar );
		ar >> volume.m_itsDeviateOnBottom;
		ar >> volume.m_itsDeviateOnTop;
		ar >> volume.m_itsLong;
		ar >> volume.m_itsShort;

		// CNVI
		ar >> nvi.m_nMADays;
		ar >> nvi.m_itsGoldenFork;
		ar >> nvi.m_itsDeadFork;

		// CPVI
		ar >> pvi.m_nMADays;
		ar >> pvi.m_itsGoldenFork;
		ar >> pvi.m_itsDeadFork;

		// CVR
		ar >> vr.m_nDays;
		ar >> vr.m_itsLong;
		ar >> vr.m_itsShort;

		// CVROC
		ar >> vroc.m_nDays;
		ar >> vroc.m_nMADays;
		ar >> vroc.m_itsDeviateOnBottom;
		ar >> vroc.m_itsDeviateOnTop;

		// COBV
		ar >> obv.m_itsDeviateOnBottom;
		ar >> obv.m_itsDeviateOnTop;

		// CMOBV
		ar >> mobv.m_nDays1;
		ar >> mobv.m_nDays2;
		ar >> mobv.m_itsGoldenFork;
		ar >> mobv.m_itsDeadFork;

		// CMFI
		ar >> mfi.m_nDays;
		ar >> mfi.m_itsLong;
		ar >> mfi.m_itsShort;

		// CVMACD
		ar >> vmacd.m_nEMA1Days;
		ar >> vmacd.m_nEMA2Days;
		ar >> vmacd.m_nDIFDays;
		ar >> vmacd.m_itsDeviateOnBottom;
		ar >> vmacd.m_itsDeviateOnTop;
		ar >> vmacd.m_itsGoldenFork;
		ar >> vmacd.m_itsDeadFork;

		// CWVAD
		ar >> wvad.m_nDays;
		ar >> wvad.m_itsLong;
		ar >> wvad.m_itsShort;

		// CEMV
		ar >> emv.m_nDays;
		ar >> emv.m_nMADays;
		ar >> emv.m_itsGoldenFork;
		ar >> emv.m_itsDeadFork;

		// CVRSI
		ar >> vrsi.m_nDays;
		ar >> vrsi.m_itsDeviateOnBottom;
		ar >> vrsi.m_itsDeviateOnTop;
		ar >> vrsi.m_itsSold;
		ar >> vrsi.m_itsBought;

		// CNVRSI
		ar >> nvrsi.m_nDays;
		ar >> nvrsi.m_itsDeviateOnBottom;
		ar >> nvrsi.m_itsDeviateOnTop;
		ar >> nvrsi.m_itsSold;
		ar >> nvrsi.m_itsBought;

		// CAD
		ar >> ad.m_nDays;

		// CCI

		// CKDJ
		ar >> kdj.m_nRSVDays;
		ar >> kdj.m_nKDays;
		ar >> kdj.m_nDDays;
		ar >> kdj.m_nJ;
		ar >> kdj.m_itsGoldenFork;
		ar >> kdj.m_itsDeadFork;

		// CR
		ar >> r.m_nDays;

		// CRSI
		rsi.m_adwDays.Serialize( ar );
		ar >> rsi.m_itsSold;
		ar >> rsi.m_itsGoldenFork;
		ar >> rsi.m_itsDeadFork;

		// CBIAS
		ar >> bias.m_nDays;
		ar >> bias.m_itsSold;
		ar >> bias.m_itsBought;

		// CMTM
		ar >> mtm.m_nDays;
		ar >> mtm.m_nMADays;
		ar >> mtm.m_itsGoldenFork;
		ar >> mtm.m_itsDeadFork;

		// CDMI
		ar >> dmi.m_nDays;

		// CROC
		ar >> roc.m_nDays;
		ar >> roc.m_nMADays;
		ar >> roc.m_itsGoldenFork;
		ar >> roc.m_itsDeadFork;

		// CCCI
		ar >> cci.m_nDays;
		ar >> cci.m_dQuotiety;
		ar >> cci.m_nMADays;
		ar >> cci.m_itsGoldenFork;
		ar >> cci.m_itsDeadFork;

		// CCV
		ar >> cv.m_nMAHLDays;
		ar >> cv.m_nCVDays;
		ar >> cv.m_itsSold;
		ar >> cv.m_itsBought;

		// CARBR
		ar >> arbr.m_nDays;
		ar >> arbr.m_itsGoldenFork;
		ar >> arbr.m_itsDeadFork;
		ar >> arbr.m_itsSold;
		ar >> arbr.m_itsBought;

		// CCR
		ar >> cr.m_nDays;
		ar >> cr.m_nMADaysA;
		ar >> cr.m_nMADaysB;
		ar >> cr.m_nMADaysC;
		ar >> cr.m_nMADaysD;
		ar >> cr.m_itsSold;
		ar >> cr.m_itsBought;

		// COSC
		ar >> osc.m_nDays;
		ar >> osc.m_nMADays;
		ar >> osc.m_itsGoldenFork;
		ar >> osc.m_itsDeadFork;

		// CUOS
		ar >> uos.m_nDays1;
		ar >> uos.m_nDays2;
		ar >> uos.m_nDays3;
		ar >> uos.m_nMADays;
		ar >> uos.m_itsGoldenFork;
		ar >> uos.m_itsDeadFork;

		// CMAOSC
		ar >> maosc.m_nDays1;
		ar >> maosc.m_nDays2;
		ar >> maosc.m_nMADays;
		ar >> maosc.m_itsGoldenFork;
		ar >> maosc.m_itsDeadFork;

		// C36BIAS
		ar >> bias36.m_itsSold;
		ar >> bias36.m_itsBought;

		// CDPO
		ar >> dpo.m_nDays;
		ar >> dpo.m_nMADays;
		ar >> dpo.m_itsGoldenFork;
		ar >> dpo.m_itsDeadFork;

		// CKST
		kst.m_adwROCDays.Serialize( ar );
		ar >> kst.m_nMADays;
		ar >> kst.m_itsGoldenFork;
		ar >> kst.m_itsDeadFork;

		// CREI
		ar >> rei.m_nDays;
		ar >> rei.m_itsLong;
		ar >> rei.m_itsShort;

		// CDMKI
		ar >> dmki.m_nDays;
		ar >> dmki.m_itsSold;
		ar >> dmki.m_itsBought;

		// CPCNT
		ar >> pcnt.m_nMADays;
		ar >> pcnt.m_itsGoldenFork;
		ar >> pcnt.m_itsDeadFork;

		// CHLC
		ar >> hlc.m_nDays;
		ar >> hlc.m_nMADays;
		ar >> hlc.m_itsGoldenFork;
		ar >> hlc.m_itsDeadFork;

		// CCDP
		
		// CASI
		ar >> asi.m_nDays;
		ar >> asi.m_itsDeviateOnBottom;
		ar >> asi.m_itsDeviateOnTop;

		// CATR
		ar >> atr.m_nDays;
		ar >> atr.m_itsSold;
		ar >> atr.m_itsBought;
		
		// CCYO
		cyo.m_adwMTMDays.Serialize( ar );
		ar >> cyo.m_nMADays;
		ar >> cyo.m_itsGoldenFork;
		ar >> cyo.m_itsDeadFork;

		// CDCYO
		dcyo.m_adwMTMDays.Serialize( ar );
		ar >> dcyo.m_nMADays;
		ar >> dcyo.m_itsGoldenFork;
		ar >> dcyo.m_itsDeadFork;

		// CHSL
		ar >> hsl.m_nDays;
		ar >> hsl.m_nMADays;
		ar >> hsl.m_itsGoldenFork;
		ar >> hsl.m_itsDeadFork;

		// CDPER
		ar >> dper.m_nDays;
		ar >> dper.m_nMADays;
		ar >> dper.m_nDetrendDays;
		ar >> dper.m_itsGoldenFork;
		ar >> dper.m_itsDeadFork;

		DWORD	dwTemp;
		for( int i=0; i<200; i++ )
			ar >> dwTemp;
	}
}

//////////////////////////////////////////////////////////////////////
// class CColorClass

CColorClass::CColorClass( )
{
}

CColorClass::CColorClass( const CColorClass &src )
{
	*this	=	src;
}

CColorClass::~CColorClass( )
{
}

CColorClass &CColorClass::operator = ( const CColorClass &src )
{
	m_strName	=	src.m_strName;
	m_aColorRef.Copy( src.m_aColorRef );
	return *this;
}

void CColorClass::Serialize( CSPArchive & ar )
{
	if( ar.IsStoring() )
	{
		ar << m_strName;
		m_aColorRef.Serialize( ar );
	}
	else
	{
		ar >> m_strName;
		m_aColorRef.Serialize( ar );
	}
}

void CColorClass::SetName( LPCTSTR lpszName )
{
	m_strName	=	lpszName;
}

CSPString CColorClass::GetName( )
{
	return m_strName;
}

void CColorClass::SetColor( int nColorIndex, COLORREF clrRGB )
{
	SP_ASSERT( nColorIndex >= 0 && nColorIndex < m_aColorRef.GetSize() );
	if( nColorIndex >= 0 && nColorIndex < m_aColorRef.GetSize() )
		m_aColorRef.SetAt( nColorIndex, clrRGB );
}

COLORREF CColorClass::GetColor( int nColorIndex )
{
	if( nColorIndex >= 0 && nColorIndex < m_aColorRef.GetSize() )
		return m_aColorRef.ElementAt( nColorIndex );
	SP_ASSERT( FALSE );
	return NDCLR_DEFAULT;
}

void CColorClass::SetColorDefault( )
{
	// set color
	m_aColorRef.SetSize( clrMax + 1 );
	m_aColorRef[clrReserved1]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved2]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved3]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved4]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved5]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved6]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved7]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved8]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved9]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved10]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved11]	=	NDCLR_DEFAULT;
	m_aColorRef[clrReserved12]	=	NDCLR_DEFAULT;

	m_aColorRef[clrDefault]		=	NDCLR_DEFAULT;
	m_aColorRef[clrWorkspBK]	=	NDCLR_WATERBLUE;
	m_aColorRef[clrWorkspText]	=	NDCLR_WHITE;
	m_aColorRef[clrSimuBK]		=	NDCLR_WATER;
	m_aColorRef[clrSimuText]	=	NDCLR_BLACK;
	m_aColorRef[clrSListBK]		=	NDCLR_BLACK;
	m_aColorRef[clrSListSelected]	=	NDCLR_MINIBLUE;
	m_aColorRef[clrBaseBK]		=	NDCLR_BLACK;
	m_aColorRef[clrBaseText]	=	NDCLR_YELLOW;
	m_aColorRef[clrGraphBK]		=	NDCLR_BLACK;

	m_aColorRef[clrBorder]		=	NDCLR_FLATRED;
#ifdef	CLKLAN_ENGLISH_US
	m_aColorRef[clrRise]		=	NDCLR_GREEN;
	m_aColorRef[clrFall]		=	NDCLR_RED;
	m_aColorRef[clrFallEntity]	=	NDCLR_FLATRED;
#else
	m_aColorRef[clrRise]		=	NDCLR_RED;
	m_aColorRef[clrFall]		=	NDCLR_GREEN;
	m_aColorRef[clrFallEntity]	=	NDCLR_FLATBLUE;
#endif
	m_aColorRef[clrPlane]		=	NDCLR_WHITE;
	m_aColorRef[clrNewKLine]	=	NDCLR_MINIYELLOW;
	m_aColorRef[clrCW]			=	NDCLR_GREENBLUE;
	m_aColorRef[clrDJ]			=	NDCLR_DARKGREY;
	m_aColorRef[clrTitle]		=	NDCLR_YELLOW;
	m_aColorRef[clrText]		=	NDCLR_FLATBLUE;
	m_aColorRef[clrLine1]		=	NDCLR_WHITE;
	m_aColorRef[clrLine2]		=	NDCLR_YELLOW;
	m_aColorRef[clrLine3]		=	NDCLR_WATERRED;
	m_aColorRef[clrLine4]		=	NDCLR_BLUE;
	m_aColorRef[clrLine5]		=	NDCLR_GREEN;
	m_aColorRef[clrLine6]		=	NDCLR_MINIYELLOW;
}

void CColorClass::SetColorStandard( )
{
	m_strName	=	colorclass_standard;

	SetColorDefault( );
}

void CColorClass::SetColorByzantinesque( )
{
	m_strName	=	colorclass_byzantinesque;

	// set color
	SetColorDefault( );

	m_aColorRef[clrDefault]		=	NDCLR_DEFAULT;
	m_aColorRef[clrWorkspBK]	=	RGB(248,250,241);
	m_aColorRef[clrWorkspText]	=	RGB(0,0,0);
	m_aColorRef[clrSimuBK]		=	RGB(248,250,241);
	m_aColorRef[clrSimuText]	=	RGB(0,0,0);
	m_aColorRef[clrSListBK]		=	RGB(248,250,241);
	m_aColorRef[clrSListSelected]=	RGB(0,128,192);
	m_aColorRef[clrBaseBK]		=	RGB(248,250,241);
	m_aColorRef[clrBaseText]	=	RGB(0,0,0);
	m_aColorRef[clrGraphBK]		=	RGB(248,250,241);

	m_aColorRef[clrBorder]		=	RGB(0,128,192);
#ifdef	CLKLAN_ENGLISH_US
	m_aColorRef[clrRise]		=	RGB(78,133,90);
	m_aColorRef[clrFall]		=	RGB(170,0,98);
	m_aColorRef[clrFallEntity]	=	RGB(170,0,98);
#else
	m_aColorRef[clrRise]		=	RGB(170,0,98);
	m_aColorRef[clrFall]		=	RGB(78,133,90);
	m_aColorRef[clrFallEntity]	=	RGB(78,133,90);
#endif
	m_aColorRef[clrPlane]		=	RGB(0,0,0);
	m_aColorRef[clrNewKLine]	=	RGB(247,223,156);
	m_aColorRef[clrCW]			=	RGB(198,190,181);
	m_aColorRef[clrDJ]			=	RGB(198,195,198);
	m_aColorRef[clrTitle]		=	RGB(15,48,136);
	m_aColorRef[clrText]		=	RGB(0,0,0);
	m_aColorRef[clrLine1]		=	RGB(188,1,188);
	m_aColorRef[clrLine2]		=	RGB(66,152,176);
	m_aColorRef[clrLine3]		=	RGB(0,128,0);
	m_aColorRef[clrLine4]		=	RGB(0,255,0);
	m_aColorRef[clrLine5]		=	RGB(128,0,0);
	m_aColorRef[clrLine6]		=	RGB(0,0,128);
}

void CColorClass::SetColorGothic( )
{
	m_strName	=	colorclass_gothic;

	// set color
	SetColorDefault( );

	m_aColorRef[clrDefault]		=	NDCLR_DEFAULT;
	m_aColorRef[clrWorkspBK]	=	RGB(250,250,250);
	m_aColorRef[clrWorkspText]	=	RGB(0,0,0);
	m_aColorRef[clrSimuBK]		=	RGB(250,250,250);
	m_aColorRef[clrSimuText]	=	RGB(0,0,0);
	m_aColorRef[clrSListBK]		=	RGB(250,250,250);
	m_aColorRef[clrSListSelected]=	RGB(0,128,192);
	m_aColorRef[clrBaseBK]		=	RGB(250,250,250);
	m_aColorRef[clrBaseText]	=	RGB(0,0,0);
	m_aColorRef[clrGraphBK]		=	RGB(250,250,250);

	m_aColorRef[clrBorder]		=	RGB(0,128,192);
#ifdef	CLKLAN_ENGLISH_US
	m_aColorRef[clrRise]		=	RGB(72,138,94);
	m_aColorRef[clrFall]		=	RGB(234,2,106);
	m_aColorRef[clrFallEntity]	=	RGB(234,2,106);
#else
	m_aColorRef[clrRise]		=	RGB(234,2,106);
	m_aColorRef[clrFall]		=	RGB(72,138,94);
	m_aColorRef[clrFallEntity]	=	RGB(72,138,94);
#endif
	m_aColorRef[clrPlane]		=	RGB(0,0,0);
	m_aColorRef[clrNewKLine]	=	RGB(247,223,156);
	m_aColorRef[clrCW]			=	RGB(198,190,181);
	m_aColorRef[clrDJ]			=	RGB(198,195,198);
	m_aColorRef[clrTitle]		=	RGB(0,0,0);
	m_aColorRef[clrText]		=	RGB(0,0,0);
	m_aColorRef[clrLine1]		=	RGB(255,128,0);
	m_aColorRef[clrLine2]		=	RGB(62,152,159);
	m_aColorRef[clrLine3]		=	RGB(0,128,0);
	m_aColorRef[clrLine4]		=	RGB(0,255,0);
	m_aColorRef[clrLine5]		=	RGB(128,0,0);
	m_aColorRef[clrLine6]		=	RGB(0,0,128);
}

void CColorClass::SetColorBaroque( )
{
	m_strName	=	colorclass_baroque;

	// set color
	SetColorDefault( );

	m_aColorRef[clrDefault]		=	NDCLR_DEFAULT;
	m_aColorRef[clrWorkspBK]	=	RGB(0,83,123);
	m_aColorRef[clrWorkspText]	=	RGB(71,193,252);
	m_aColorRef[clrSimuBK]		=	RGB(0,83,123);
	m_aColorRef[clrSimuText]	=	RGB(71,193,252);
	m_aColorRef[clrSListBK]		=	RGB(0,83,123);
	m_aColorRef[clrSListSelected]=	RGB(160,193,199);
	m_aColorRef[clrBaseBK]		=	RGB(0,83,123);
	m_aColorRef[clrBaseText]	=	RGB(71,193,252);
	m_aColorRef[clrGraphBK]		=	RGB(0,83,123);

	m_aColorRef[clrBorder]		=	RGB(160,193,199);
#ifdef	CLKLAN_ENGLISH_US
	m_aColorRef[clrRise]		=	RGB(0,255,255);
	m_aColorRef[clrFall]		=	RGB(255,157,191);
	m_aColorRef[clrFallEntity]	=	RGB(255,157,191);
#else
	m_aColorRef[clrRise]		=	RGB(255,157,191);
	m_aColorRef[clrFall]		=	RGB(0,255,255);
	m_aColorRef[clrFallEntity]	=	RGB(0,255,255);
#endif
	m_aColorRef[clrPlane]		=	RGB(71,193,252);
	m_aColorRef[clrNewKLine]	=	RGB(247,223,156);
	m_aColorRef[clrCW]			=	RGB(198,190,181);
	m_aColorRef[clrDJ]			=	RGB(198,195,198);
	m_aColorRef[clrTitle]		=	RGB(255,255,0);
	m_aColorRef[clrText]		=	RGB(71,193,252);
	m_aColorRef[clrLine1]		=	RGB(255,215,74);
	m_aColorRef[clrLine2]		=	RGB(255,255,255);
	m_aColorRef[clrLine3]		=	RGB(0,128,0);
	m_aColorRef[clrLine4]		=	RGB(0,255,0);
	m_aColorRef[clrLine5]		=	RGB(0,0,160);
	m_aColorRef[clrLine6]		=	RGB(0,0,128);
}

BOOL CColorClass::IsSysColorClass( LPCTSTR lpszColorClassName )
{
	if( NULL == lpszColorClassName || strlen(lpszColorClassName) <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	CSPString	strName;
	strName	=	colorclass_standard;
	if( 0 == strName.Compare( lpszColorClassName ) )
		return TRUE;
	strName	=	colorclass_byzantinesque;
	if( 0 == strName.Compare( lpszColorClassName ) )
		return TRUE;
	strName	=	colorclass_gothic;
	if( 0 == strName.Compare( lpszColorClassName ) )
		return TRUE;
	strName	=	colorclass_baroque;
	if( 0 == strName.Compare( lpszColorClassName ) )
		return TRUE;
	return FALSE;
}

BOOL CColorClass::IsColorStandard( LPCTSTR lpszColorClassName )
{
	if( NULL == lpszColorClassName || strlen(lpszColorClassName) <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	CSPString	strName;
	strName	=	colorclass_standard;
	if( 0 == strName.Compare( lpszColorClassName ) )
		return TRUE;
	return FALSE;
}

BOOL CColorClass::IsColorByzantinesque( LPCTSTR lpszColorClassName )
{
	if( NULL == lpszColorClassName || strlen(lpszColorClassName) <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	CSPString	strName;
	strName	=	colorclass_byzantinesque;
	if( 0 == strName.Compare( lpszColorClassName ) )
		return TRUE;
	return FALSE;
}

BOOL CColorClass::IsColorGothic( LPCTSTR lpszColorClassName )
{
	if( NULL == lpszColorClassName || strlen(lpszColorClassName) <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	CSPString	strName;
	strName	=	colorclass_gothic;
	if( 0 == strName.Compare( lpszColorClassName ) )
		return TRUE;
	return FALSE;
}

BOOL CColorClass::IsColorBaroque( LPCTSTR lpszColorClassName )
{
	if( NULL == lpszColorClassName || strlen(lpszColorClassName) <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	CSPString	strName;
	strName	=	colorclass_baroque;
	if( 0 == strName.Compare( lpszColorClassName ) )
		return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// class CColorClassContainer

CColorClassContainer::CColorClassContainer( )
{
	CColorClass	cls;
	cls.SetColorDefault();
	Add( cls );
	m_nSelectedColorClass	=	0;
}

CColorClassContainer::CColorClassContainer( CColorClassContainer & src )
{
	*this	=	src;
}

CColorClassContainer::~CColorClassContainer( )
{
	RemoveAll( );
}

CColorClassContainer & CColorClassContainer::operator = ( CColorClassContainer & src )
{
	m_nSelectedColorClass	=	src.m_nSelectedColorClass;

	SetSize( src.GetSize() );
	for( int i=0; i<src.GetSize(); i++ )
	{
		CColorClass cls	=	src.ElementAt(i);
		SetAt( i, cls );
	}
	return *this;
}

void CColorClassContainer::Serialize( CSPArchive & ar )
{
	if( ar.IsStoring() )
	{
		ar << m_nSelectedColorClass;

		ar << GetSize();
		for( int i=0; i<GetSize(); i++ )
			ElementAt(i).Serialize( ar );
	}
	else
	{
		ar >> m_nSelectedColorClass;

		int	size;
		ar >> size;
		SetSize( size );
		for( int i=0; i<size; i++ )
		{
			CColorClass	element;
			element.Serialize( ar );
			SetAt( i, element );
		}
	}
}

int CColorClassContainer::AddColorClass( CColorClass & newColorClass )
{
	if( 0 == newColorClass.GetName().GetLength() )
	{
		SP_ASSERT( FALSE );
		return -1;
	}

	for( int i=0; i<GetSize(); i++ )
	{
		CColorClass & cls = ElementAt(i);
		if( 0 == newColorClass.GetName().CompareNoCase( cls.GetName() ) )
			return -1;
	}
	return Add( newColorClass );
}

BOOL CColorClassContainer::RemoveColorClass( LPCTSTR lpszColorClassName )
{
	if( NULL == lpszColorClassName || strlen(lpszColorClassName) <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	for( int i=0; i<GetSize(); i++ )
	{
		CColorClass & cls = ElementAt(i);
		if( 0 == cls.GetName().CompareNoCase( lpszColorClassName ) )
		{
			RemoveAt( i );
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CColorClassContainer::SetColorClass( CColorClass & clrcls )
{
	if( clrcls.GetName().GetLength() <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	for( int i=0; i<GetSize(); i++ )
	{
		CColorClass & cls = ElementAt(i);
		if( 0 == cls.GetName().CompareNoCase( clrcls.GetName() ) )
		{
			cls	=	clrcls;
			return TRUE;
		}
	}

	return (-1 != AddColorClass( clrcls ) );
}

BOOL CColorClassContainer::GetColorClass( LPCTSTR lpszColorClassName, CColorClass & clrcls )
{
	if( NULL == lpszColorClassName || strlen(lpszColorClassName) <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	for( int i=0; i<GetSize(); i++ )
	{
		CColorClass & cls = ElementAt(i);
		if( 0 == cls.GetName().CompareNoCase( lpszColorClassName ) )
		{
			clrcls	=	cls;
			return TRUE;
		}
	}

	SP_ASSERT( FALSE );
	return FALSE;
}

COLORREF CColorClassContainer::GetColor( int nColorIndex )
{
	SP_ASSERT( m_nSelectedColorClass >= 0 && m_nSelectedColorClass < GetSize() );
	if( m_nSelectedColorClass >= 0 && m_nSelectedColorClass < GetSize() )
		return ElementAt( m_nSelectedColorClass ).GetColor( nColorIndex );
	if( GetSize() > 0 )
		return ElementAt( 0 ).GetColor( nColorIndex );

	SP_ASSERT( FALSE );
	SetContainerDefault( );
	if( m_nSelectedColorClass >= 0 && m_nSelectedColorClass < GetSize() )
		return ElementAt( m_nSelectedColorClass ).GetColor( nColorIndex );
	SP_ASSERT( FALSE );
	return NDCLR_DEFAULT;
}

void CColorClassContainer::SetContainerDefault( )
{
	RemoveAll( );

	m_nSelectedColorClass	=	0;
	CColorClass		cls;
	cls.SetColorStandard( );
	Add( cls );
	cls.SetColorByzantinesque( );
	Add( cls );
	cls.SetColorGothic( );
	Add( cls );
	cls.SetColorBaroque( );
	Add( cls );
}

BOOL CColorClassContainer::SelectColorClass( LPCTSTR lpszColorClassName )
{
	if( NULL == lpszColorClassName || strlen(lpszColorClassName) <= 0 )
	{
		SP_ASSERT( FALSE );
		return FALSE;
	}

	for( int i=0; i<GetSize(); i++ )
	{
		CColorClass & cls = ElementAt(i);
		if( 0 == cls.GetName().CompareNoCase( lpszColorClassName ) )
		{
			m_nSelectedColorClass	=	i;
			return TRUE;
		}
	}

	SP_ASSERT( FALSE );
	return FALSE;
}

int CColorClassContainer::GetSelected( )
{
	return m_nSelectedColorClass;
}


//////////////////////////////////////////////////////////////////////
// CStProfile

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStProfile::CStProfile()
{
	m_nAccessType		=	0;		// INTERNET_OPEN_TYPE_PRECONFIG
	m_nProxyType		=	3;		// CProxySocket::TypeHTTP
	m_nProxyPort		=	80;		// INTERNET_DEFAULT_HTTP_PORT

	m_bCurStockFromSList	=	FALSE;

	m_nAutoResumeDRLimit	=	11;
	m_nDiffDays				=	-1;
	m_nDiffPercentDays		=	-1;
	m_nScopeDays			=	-1;
	m_nRatioChangeHandDays	=	-1;
	m_nRatioVolumeDays		=	-1;
	m_nRSDays				=	-1;
	m_nYieldAverageDays		=	-1;

	m_nInfoCurrentServer	=	0;

	m_nGraphKFormat			=	CKData::formatOriginal;
	m_nGraphMaindataType	=	CKData::mdtypeClose;
	m_dwAutoResumeDRBegin	=	STKLIB_AUTORESUMEDR_DATEBEGIN;
	m_nAutoResumeDRLimit	=	STKLIB_AUTORESUMEDR_LIMIT;

	m_bAutoSaveKDataMin		=	TRUE;

	m_bNotifyIcon			=	FALSE;

	m_nCacheDays			=	STKLIB_DAYS_CACHE;

	m_nLocalDataVersion		=	20050630;

	// SListView Font And BaseView Font
	memset( &m_lfSListView, 0, sizeof(m_lfSListView) );
	memset( &m_lfBaseView, 0, sizeof(m_lfBaseView) );
}

CStProfile::~CStProfile()
{
}

void CStProfile::StoreProfile( )
{
	Store( GetWorkDirectory() + "profile.set" );
}

void CStProfile::LoadProfile( )
{
	// Color
	m_ColorClassContainer.SetContainerDefault( );

	Load( GetWorkDirectory() + "profile.set" );
}

#define	SF_MAX_PROFILE_LENGTH	0x00080000 

char szProfileMagic[]			=	"MGCDPF";
DWORD	dwProfileVersion		=	0x20050630;

BOOL CStProfile::Load( LPCTSTR lpszFileName )
{
	if( NULL == lpszFileName || strlen(lpszFileName) <= 0 )
		return FALSE;

	CSPFile file;
	if( file.Open( lpszFileName, CSPFile::modeRead ) )
	{
		DWORD dwLen = file.GetLength();
		if( 0 == dwLen || dwLen > SF_MAX_PROFILE_LENGTH )
		{
			file.Close();
			return FALSE;
		}
		
		CSPArchive ar( &file, CSPArchive::load );
		FileSerialize( ar );
		ar.Close();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CStProfile::Store( LPCTSTR lpszFileName )
{
	if( NULL == lpszFileName || strlen(lpszFileName) <= 0 )
		return FALSE;

	CSPFile file;
	if( file.Open( lpszFileName, CSPFile::modeCreate | CSPFile::modeWrite ) )
	{
		CSPArchive ar( &file, CSPArchive::store );
		FileSerialize( ar );
		ar.Close();
		file.Close();
		return TRUE;
	}
	return FALSE;
}

void CStProfile::FileSerialize( CSPArchive &ar )
{
	if( ar.IsStoring() )
	{
		ar.WriteString( szProfileMagic );
		ar << dwProfileVersion;

		ar << m_strDefaultServer;
		m_astrDownloadServers.Serialize( ar );
		ar << m_nAccessType;
		ar << m_strProxyAddress;
		ar << m_nProxyPort;

		// Window Placement
		ar << m_strWindowPlacement;

		// Data path
		ar << m_strQianlongPath;

		// Color
		m_ColorClassContainer.Serialize( ar );

		// Last Opened Strategies
		m_astrLastOpenedStrategy.Serialize( ar );
		ar << m_strLastActiveStrategy;

		// Net Info
		ar << m_bNetInfoShow;
		ar << m_strNetInfo;
		ar << m_strNetInfourl;
		ar << m_strLatestver;
	
		// AutoResumeDRLimit
		ar << m_nAutoResumeDRLimit;

		// SList columns
		m_anSListColumnsShow.Serialize( ar );
		m_anSListColumnsOrder.Serialize( ar );
		m_aSListColumnsUser.FileSerialize( ar );

		ar << m_nDiffDays;
		ar << m_nDiffPercentDays;
		ar << m_nScopeDays;
		ar << m_nRatioChangeHandDays;
		ar << m_nRatioVolumeDays;
		ar << m_nRSDays;
		ar << m_nYieldAverageDays;

		// Info View
		ar << m_nInfoCurrentServer;

		// Graph Techs
		m_anGraphTechsShow.Serialize( ar );
		m_anGraphTechsKLine.Serialize( ar );
		m_anGraphTechsAll.Serialize( ar );
		ar << m_nGraphKFormat;
		ar << m_nGraphMaindataType;
		ar << m_dwAutoResumeDRBegin;

		ar.Write( &m_lfSListView, sizeof(m_lfSListView) );
		ar.Write( &m_lfBaseView, sizeof(m_lfBaseView) );

		m_techparameters.Serialize( ar );

		m_aHTMLURLToExplorer.Serialize( ar );

		m_astrProvider.Serialize( ar );
		m_astrDriver.Serialize( ar );

		ar << m_strTrader;
		ar << m_bAutoSaveKDataMin;

		ar << m_nProxyType;
		ar << m_strProxyUser;
		ar << m_strProxyPasswd;

		ar << m_bNotifyIcon;

		ar << m_nCacheDays;

		m_aAlarmCond.Serialize( ar );

		ar << m_nLocalDataVersion;
	}
	else
	{
		TCHAR buffer[64];
		memset( buffer, 0, sizeof(buffer) );
		ar.ReadString( buffer, strlen(szProfileMagic) );
		if( 0 != strncmp( szProfileMagic, buffer, strlen(szProfileMagic) ) )
			return;
				
		ar >> m_dwFileVersion;
		if( m_dwFileVersion > dwProfileVersion )
			return;

		ar >> m_strDefaultServer;
		m_astrDownloadServers.Serialize( ar );
		ar >> m_nAccessType;
		ar >> m_strProxyAddress;
		ar >> m_nProxyPort;

		// Window Placement
		ar >> m_strWindowPlacement;

		// Data path
		ar >> m_strQianlongPath;

		// Color
		m_ColorClassContainer.Serialize( ar );

		// Last Opened Strategies
		m_astrLastOpenedStrategy.Serialize( ar );
		ar >> m_strLastActiveStrategy;

		// Net Info
		ar >> m_bNetInfoShow;
		ar >> m_strNetInfo;
		ar >> m_strNetInfourl;
		ar >> m_strLatestver;

		// AutoResumeDRLimit
		ar >> m_nAutoResumeDRLimit;

		// SList Columns
		m_anSListColumnsShow.Serialize( ar );
		m_anSListColumnsOrder.Serialize( ar );
		m_aSListColumnsUser.FileSerialize( ar );

		ar >> m_nDiffDays;
		ar >> m_nDiffPercentDays;
		ar >> m_nScopeDays;
		ar >> m_nRatioChangeHandDays;
		ar >> m_nRatioVolumeDays;
		ar >> m_nRSDays;
		ar >> m_nYieldAverageDays;

		// Info View
		ar >> m_nInfoCurrentServer;

		// Graph Techs
		m_anGraphTechsShow.Serialize( ar );
		m_anGraphTechsKLine.Serialize( ar );
		m_anGraphTechsAll.Serialize( ar );
		ar >> m_nGraphKFormat;
		ar >> m_nGraphMaindataType;
		ar >> m_dwAutoResumeDRBegin;

		// SListView Font And BaseView Font
		ar.Read( &m_lfSListView, sizeof(m_lfSListView) );
		ar.Read( &m_lfBaseView, sizeof(m_lfBaseView) );

		m_techparameters.Serialize( ar );

		if( m_dwFileVersion >= 0x20030828 )
			m_aHTMLURLToExplorer.Serialize( ar );

		if( m_dwFileVersion >= 0x20030910 )
		{
			m_astrProvider.Serialize( ar );
			m_astrDriver.Serialize( ar );
		}

		if( m_dwFileVersion >= 0x20040710 )
		{
			ar >> m_strTrader;
			ar >> m_bAutoSaveKDataMin;
		}

		if( m_dwFileVersion >= 0x20040717 )
		{
			ar >> m_nProxyType;
			ar >> m_strProxyUser;
			ar >> m_strProxyPasswd;
		}

		if( m_dwFileVersion >= 0x20040803 )
		{
			ar >> m_bNotifyIcon;
			ar >> m_nCacheDays;
			m_nCacheDays	=	STKLIB_DAYS_CACHE;
		}

		if( m_dwFileVersion >= 0x20040903 )
		{
			m_aAlarmCond.Serialize( ar );
		}

		if( m_dwFileVersion >= 0x20050630 )
		{
			ar >> m_nLocalDataVersion;
		}
	}
}

CSPString CStProfile::GetMainServerDomain()
{
	CSPString strResult;

	char szMainServerDomainFmt[] = "%sin%su%sls%sco%s";
								//   ninebul ls.co m
								//   ^   ^^ ^   ^   ^
	strResult.Format( szMainServerDomainFmt, "n", "eb", "l", ".", "m" ); 
	strResult = "tsking.cn";

	return strResult;
}

CSPString CStProfile::GetHTTPServerDomain()
{
	CSPString strResult;

	strResult.Format("www.%s", (LPCTSTR)GetMainServerDomain());

	return strResult;
}

WORD CStProfile::GetHTTPServerPort( )
{
	return 80;
}

CSPString	CStProfile::GetMainServer( )
{
	CSPString	strInfo;
	strInfo	=	sz_mainserverinfo;
	CSPString	strResult;
	strResult.Format( "%s:%d:%s", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort(), (LPCTSTR)strInfo );
	return strResult;
}

CSPString CStProfile::GetProductURL( )
{
	CSPString	strResult;
#ifdef	CLKLAN_ENGLISH_US
	strResult.Format( "http://%s:%d/enu/product/ninebulls/", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#elif defined CLKVER_OEM
	strResult.Format( "http://%s:%d/product/stockana/", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#else
	strResult.Format( "http://%s:%d/product/clking/", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#endif
	return strResult;
}

CSPString CStProfile::GetDownloadURL( )
{
	CSPString	strResult;
#ifdef	CLKLAN_ENGLISH_US
	strResult.Format( "http://%s:%d/enu/download/ninebulls/", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#elif defined CLKVER_OEM
	strResult.Format( "http://%s:%d/download/stockana/", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#else
	strResult.Format( "http://%s:%d/download/clking/", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#endif
	return strResult;
}

CSPString CStProfile::GetPurchaseURL( )
{
	CSPString	strResult;
#ifdef	CLKLAN_ENGLISH_US
	strResult.Format( "http://%s:%d/enu/purchase/ninebulls", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#elif defined CLKVER_OEM
	strResult.Format( "http://%s:%d/purchase/stockana", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#else
	strResult.Format( "http://%s:%d/purchase/clking", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#endif
	return strResult;
}

CSPString CStProfile::GetAgentRootURL( )
{
	CSPString	strResult;
	strResult.Format( "http://%s:%d/agent/", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
	return strResult;
}

CSPString CStProfile::GetSupportMailto( )
{
	CSPString strResult;

#ifdef CLKVER_OEM
	strResult	=	"mailto:";
	strResult	+=	OEM_SUPPORT_MAILTO;
	strResult	+=	"?subject=";
#else
	strResult.Format( "www.%s", (LPCTSTR)GetMainServerDomain() );

	char szSupportMailtoFmt[]	=	"ma%sto%ssu%sor%s@%s?subject="; // ?subject=support&body=hello
	strResult.Format( szSupportMailtoFmt, "il", ":", "pp", "t", (LPCTSTR)GetMainServerDomain() );
#endif

	strResult	+=	sz_mailsubjectsupport;
	return strResult;
}

CSPString CStProfile::GetUpgradeURL( )
{
	CSPString	strResult;
#ifdef	CLKLAN_ENGLISH_US
	strResult.Format( "http://%s:%d/enu/download/ninebulls/upgrade3.exe", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#elif defined CLKVER_OEM
	strResult.Format( "http://%s:%d/download/stockana/upgrade3.exe", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#else
	strResult.Format( "http://%s:%d/download/tsking.exe", (LPCTSTR)GetHTTPServerDomain(), GetHTTPServerPort() );
#endif
	return strResult;
}

CSPString CStProfile::GetEvaluateEmail( )
{
	CSPString	strResult;
	strResult.Format( "evaluate@%s", (LPCTSTR)GetMainServerDomain() );
	return strResult;
}

CSPString CStProfile::GetEvaluateSMTPServer( )
{
	CSPString	strResult;
	strResult.Format( "mail.%s", (LPCTSTR)GetMainServerDomain() );
	return strResult;
}

CSPString CStProfile::GetEvaluateSMTPUser( )
{
	CSPString	strResult;
	strResult.Format( "evasend@%s", (LPCTSTR)GetMainServerDomain() );
	return strResult;
}

CSPString CStProfile::GetEvaluateSMTPPasswd( )
{
	return "evasend";
}

CSPString	CStProfile::GetDefaultServer( )
{
	if( m_strDefaultServer.IsEmpty() )
		return GetMainServer( );
	return m_strDefaultServer;
}

void CStProfile::SetDefaultServer( LPCTSTR lpszDefaultServer )
{
	m_strDefaultServer	=	lpszDefaultServer;
}

CSPStringArray & CStProfile::GetDownloadServers( )
{
	return m_astrDownloadServers;
}

void CStProfile::SetDownloadServers( CSPStringArray & astrAllServers )
{
	m_astrDownloadServers.RemoveAll();
	m_astrDownloadServers.Copy( astrAllServers );
}

/*
 * not use now, use CQSProfile's proxy settings
 *

char szQuoteServerProfileName[] = "QuoteServer.ini";
char szProfile_Proxy[] = "Proxy";
char szProfile_AccessType[] = "AccessType";
char szProfile_ProxyType[] = "ProxyType";
char szProfile_ProxyAddress[] = "ProxyAddress";
char szProfile_ProxyPort[] = "ProxyPort";
char szProfile_ProxyUser[] = "ProxyUser";
char szProfile_ProxyPasswd[] = "ProxyPasswd";
	
int CStProfile::GetAccessType( )
{
	//if( INTERNET_OPEN_TYPE_DIRECT != m_nAccessType
	//	&& INTERNET_OPEN_TYPE_PRECONFIG != m_nAccessType
	//	&& INTERNET_OPEN_TYPE_PROXY != m_nAccessType )
	//	return INTERNET_OPEN_TYPE_DIRECT;

	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	m_nAccessType = ::GetPrivateProfileInt( szProfile_Proxy, szProfile_AccessType, 0, sFileName );
	return m_nAccessType;
}

void CStProfile::SetAccessType( int nAccessType )
{
	m_nAccessType	=	nAccessType;

	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	CSPString sTemp;
	sTemp.Format( "%d", m_nAccessType );
	::WritePrivateProfileString( szProfile_Proxy, szProfile_AccessType, sTemp, sFileName );
}

int CStProfile::GetProxyType( )
{
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	m_nProxyType = ::GetPrivateProfileInt( szProfile_Proxy, szProfile_ProxyType, 3, sFileName );
	return m_nProxyType;
}

void CStProfile::SetProxyType( int nProxyType )
{
	m_nProxyType	=	nProxyType;
	
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	CSPString sTemp;
	sTemp.Format( "%d", m_nProxyType );
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyType, sTemp, sFileName );
}

CSPString	CStProfile::GetProxyAddress( )
{
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	char szAddress[1024];
	memset( szAddress, 0, sizeof(szAddress) );
	::GetPrivateProfileString( szProfile_Proxy, szProfile_ProxyAddress, "", szAddress, sizeof(szAddress)-2, sFileName );
	m_strProxyAddress	=	szAddress;
	return m_strProxyAddress;
}

void CStProfile::SetProxyAddress( LPCTSTR lpszProxyAddress )
{
	m_strProxyAddress	=	lpszProxyAddress;
	
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyAddress, m_strProxyAddress, sFileName );
}

WORD CStProfile::GetProxyPort( )
{
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	m_nProxyPort = ::GetPrivateProfileInt( szProfile_Proxy, szProfile_ProxyPort, 80, sFileName );
	return m_nProxyPort;
}

void CStProfile::SetProxyPort( WORD nPort )
{
	m_nProxyPort	=	nPort;
	
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	CSPString sTemp;
	sTemp.Format( "%d", m_nProxyPort );
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyPort, sTemp, sFileName );
}

CSPString	CStProfile::GetProxyUser( )
{
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	char szUser[1024];
	memset( szUser, 0, sizeof(szUser) );
	::GetPrivateProfileString( szProfile_Proxy, szProfile_ProxyUser, "", szUser, sizeof(szUser)-2, sFileName );
	m_strProxyUser		=	szUser;
	return m_strProxyUser;
}

void CStProfile::SetProxyUser( LPCTSTR lpszProxyUser )
{
	m_strProxyUser	=	lpszProxyUser;

	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyUser, m_strProxyUser, sFileName );
}

CSPString	CStProfile::GetProxyPasswd( )
{
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	char szPasswd[1024];
	memset( szPasswd, 0, sizeof(szPasswd) );
	::GetPrivateProfileString( szProfile_Proxy, szProfile_ProxyPasswd, "", szPasswd, sizeof(szPasswd)-2, sFileName );
	m_strProxyPasswd	=	szPasswd;
	return m_strProxyPasswd;
}

void CStProfile::SetProxyPasswd( LPCTSTR lpszProxyPasswd )
{
	m_strProxyPasswd	=	lpszProxyPasswd;
	
	CSPString sFileName = GetWorkDirectory() + szQuoteServerProfileName;
	::WritePrivateProfileString( szProfile_Proxy, szProfile_ProxyPasswd, m_strProxyPasswd, sFileName );
}
*/

static char szFormat[] = "%u,%u,%d,%d,%d,%d,%d,%d,%d,%d";

BOOL CStProfile::GetWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	if( m_strWindowPlacement.IsEmpty() )
		return FALSE;

	WINDOWPLACEMENT wp;
	int nRead = sscanf(m_strWindowPlacement, szFormat,
		&wp.flags, &wp.showCmd,
		&wp.ptMinPosition.x, &wp.ptMinPosition.y,
		&wp.ptMaxPosition.x, &wp.ptMaxPosition.y,
		&wp.rcNormalPosition.left, &wp.rcNormalPosition.top,
		&wp.rcNormalPosition.right, &wp.rcNormalPosition.bottom);

	if (nRead != 10)
		return FALSE;

	int cxScreen = GetSystemMetrics( SM_CXSCREEN );
	int cyScreen = GetSystemMetrics( SM_CYSCREEN );

	if( wp.rcNormalPosition.top < -5
		|| wp.rcNormalPosition.left < -cxScreen/2
		|| wp.rcNormalPosition.right < 5
		|| wp.rcNormalPosition.right - wp.rcNormalPosition.left < 1
		|| wp.rcNormalPosition.bottom - wp.rcNormalPosition.top < 1 )
		return FALSE;
 
	wp.length = sizeof wp;
	*pwp = wp;
	return TRUE;
}

void CStProfile::SetWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	TCHAR szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];

	sprintf(szBuffer, szFormat,
		pwp->flags, pwp->showCmd,
		pwp->ptMinPosition.x, pwp->ptMinPosition.y,
		pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
		pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
		pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	m_strWindowPlacement	=	szBuffer;
}

CSPString CStProfile::GetWorkDirectory( )
{
	if( m_strWorkDirectory.IsEmpty() )
	{
		TCHAR szLongPathName[_MAX_PATH];
		::GetModuleFileName( AfxGetModuleHandle(), szLongPathName, _MAX_PATH);
		m_strWorkDirectory	=	AfxGetFileDirectory( szLongPathName, "" );
	}
	return m_strWorkDirectory;
}

CSPString CStProfile::GetVersion( )
{
	return "3.10";
}

CSPString CStProfile::GetSViewFile( )
{
	return GetWorkDirectory() + "profile.dat";
}

CSPString CStProfile::GetDomainFile( )
{
	return GetWorkDirectory() + "history\\domain.dat";
}

CSPString CStProfile::GetGroupFile( )
{
	return GetWorkDirectory() + "group.dat";
}

CSPString CStProfile::GetProjectPath( )
{
	CSPString	strPath	=	GetWorkDirectory() + "projects" + STRING_DIRSEP;
	if( 0 != access( strPath, 0 ) || FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( strPath ) )
	{
		if( !CreateDirectory( strPath, NULL ) )
			strPath.Empty();
	}
	return strPath;
}

CSPString CStProfile::GetExportPath( )
{
	CSPString	strPath	=	GetWorkDirectory() + "export" + STRING_DIRSEP;
	if( 0 != access( strPath, 0 ) || FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( strPath ) )
	{
		if( !CreateDirectory( strPath, NULL ) )
			strPath.Empty();
	}
	return strPath;
}

CSPString CStProfile::GetYongxinPath( )
{
	CSPString	strPath	=	GetWorkDirectory() + "yongxin" + STRING_DIRSEP;
	if( 0 != access( strPath, 0 ) || FILE_ATTRIBUTE_DIRECTORY != GetFileAttributes( strPath ) )
	{
		if( !CreateDirectory( strPath, NULL ) )
			strPath.Empty();
	}
	return strPath;
}

CSPString CStProfile::GetSelfDBPath( )
{
	return GetWorkDirectory();
}

CSPString CStProfile::GetSelfDBNewsPath( DWORD dwMarket )
{
	return CSelfDB::GetNewsPath( dwMarket );
}

CSPString CStProfile::GetSelfDBBasePath( DWORD dwMarket )
{
	return CSelfDB::GetBasePath( dwMarket );
}

CSPString	CStProfile::GetQianlongPath( )
{
	if( 0 == access(m_strQianlongPath,0) )
		return m_strQianlongPath;
	return "";
}

void CStProfile::SetQianlongPath( LPCTSTR lpszPath )
{
	m_strQianlongPath	=	lpszPath;
	SP_ASSERT( 0 == access(m_strQianlongPath,0) );
}

COLORREF CStProfile::GetColor( int nColorIndex )
{
	return m_ColorClassContainer.GetColor( nColorIndex );
}

CColorClassContainer & CStProfile::GetColorClassContainer( )
{
	return m_ColorClassContainer;
}

CSPStringArray & CStProfile::GetLastOpenedStrategy( )
{
	return m_astrLastOpenedStrategy;
}

void CStProfile::SetLastOpenedStrategy( CSPStringArray & astr )
{
	m_astrLastOpenedStrategy.Copy( astr );
}

CSPString CStProfile::GetLastActiveStrategy( )
{
	return m_strLastActiveStrategy;
}

void CStProfile::SetLastActiveStrategy( LPCTSTR lpszPath )
{
	m_strLastActiveStrategy	=	lpszPath;
}

void CStProfile::SetNetInfoShowLater( BOOL bShowLater )
{
	m_bNetInfoShow	=	bShowLater;
}

void CStProfile::SetNetInfo( LPCTSTR lpszInfo, LPCTSTR lpszInfourl )
{
	if( 0 == m_strNetInfo.Compare( lpszInfo )
		&& 0 == m_strNetInfourl.Compare( lpszInfourl ) )
		return;
	m_strNetInfo	=	lpszInfo;
	m_strNetInfourl	=	lpszInfourl;
	m_bNetInfoShow	=	TRUE;
}

void CStProfile::GetNetInfo( CSPString &strInfo, CSPString &strInfourl, int * pbShow )
{
	strInfo		=	m_strNetInfo;
	strInfourl	=	m_strNetInfourl;
	if( pbShow )
		*pbShow	=	m_bNetInfoShow;
}

void CStProfile::SetLatestver( LPCTSTR lpszLatestver )
{
	m_strLatestver	=	lpszLatestver;
}

BOOL CStProfile::HasNewVersion( )
{
#ifdef	CLKVER_OEM
	CSPString	strServerDomain = AfxGetProfile().GetMainServerDomain();
	if( 0 == strServerDomain.CompareNoCase( "www.balang.net" )
		|| 0 == strServerDomain.CompareNoCase( "balang.net" )
		|| 0 == strServerDomain.CompareNoCase( "www.zlinker.com" )
		|| 0 == strServerDomain.CompareNoCase( "zlinker.com" )
		|| 0 == strServerDomain.CompareNoCase( "www.ninebulls.com" )
		|| 0 == strServerDomain.CompareNoCase( "ninebulls.com" ) )
	{
		return FALSE;
	}
#endif

	if( !m_strLatestver.IsEmpty() )
	{
		CSPString	strMyVersion = GetVersion();
		double dLatestVer = atof(m_strLatestver);
		double dMyVer = atof(strMyVersion);
		if( dLatestVer - dMyVer > 1e-4 )
			return TRUE;
	}
	return FALSE;
}

int CStProfile::GetAutoResumeDRLimit( )
{
	if( 0 == m_nAutoResumeDRLimit || -1 == m_nAutoResumeDRLimit )
		m_nAutoResumeDRLimit	=	11;
	return m_nAutoResumeDRLimit;
}

void CStProfile::SetAutoResumeDRLimit( int nAutoResumeDRLimit )
{
	m_nAutoResumeDRLimit	=	nAutoResumeDRLimit;
}

void CStProfile::SetSListColumnsDefault( )
{
	// All Order
	m_anSListColumnsOrder.RemoveAll();
#ifdef CLKLAN_ENGLISH_US
	for( UINT i=SLH_MIN; i<=SLH_MAX; i++ )
	{
		m_anSListColumnsOrder.Add( i );
	}
#else
	m_anSListColumnsOrder.Add( SLH_NAME );
	m_anSListColumnsOrder.Add( SLH_CODE );
	for( UINT i=SLH_MIN; i<=SLH_MAX; i++ )
	{
		if( SLH_NAME != i && SLH_CODE != i )
			m_anSListColumnsOrder.Add( i );
	}
#endif
	for( int k=0; k<m_aSListColumnsUser.GetSize(); k++ )
	{
		m_anSListColumnsOrder.Add( m_aSListColumnsUser.ElementAt(k).m_nID );
	}

	// Show
	m_anSListColumnsShow.RemoveAll( );
	m_anSListColumnsShow.Copy( m_anSListColumnsOrder );
}

CSPDWordArray & CStProfile::GetSListColumnsShow( )
{
	if( m_anSListColumnsShow.GetSize() == 0 )
		SetSListColumnsDefault( );

	// Remove some dulplicate
	for( int i=m_anSListColumnsShow.GetSize()-1; i>=0; i-- )
	{
		for( int k=0; k<i; k++ )
		{
			if( m_anSListColumnsShow[i] == m_anSListColumnsShow[k] )
			{
				m_anSListColumnsShow.RemoveAt( i );
				SP_ASSERT( FALSE );
				break;
			}
		}
	}

	return m_anSListColumnsShow;
}

CSPDWordArray & CStProfile::GetSListColumnsOrder( )
{
	int	nRealCount = SLH_MAX-SLH_MIN+1 + m_aSListColumnsUser.GetSize();
	if( m_anSListColumnsOrder.GetSize() != nRealCount )
		SetSListColumnsDefault( );

	// Remove some dulplicate
	for( int i=m_anSListColumnsOrder.GetSize()-1; i>=0; i-- )
	{
		for( int k=0; k<i; k++ )
		{
			if( m_anSListColumnsOrder[i] == m_anSListColumnsOrder[k] )
			{
				m_anSListColumnsOrder.RemoveAt( i );
				SP_ASSERT( FALSE );
				break;
			}
		}
	}

	return m_anSListColumnsOrder;
}

CIndexContainer &CStProfile::GetSListColumnsUser( )
{
	return m_aSListColumnsUser;
}

void CStProfile::SetDiffDays( int nDays )
{
	m_nDiffDays	=	nDays;
}

int CStProfile::GetDiffDays( )
{
	if( m_nDiffDays <= 0 )
		return 1;
	return m_nDiffDays;
}

void CStProfile::SetDiffPercentDays( int nDays )
{
	m_nDiffPercentDays	=	nDays;
}

int CStProfile::GetDiffPercentDays( )
{
	if( m_nDiffPercentDays <= 0 )
		return 1;
	return m_nDiffPercentDays;
}

void CStProfile::SetScopeDays( int nDays )
{
	m_nScopeDays	=	nDays;
}

int CStProfile::GetScopeDays( )
{
	if( m_nScopeDays <= 0 )
		return 1;
	return m_nScopeDays;
}

void CStProfile::SetRatioChangeHandDays( int nDays )
{
	m_nRatioChangeHandDays	=	nDays;
}

int CStProfile::GetRatioChangeHandDays( )
{
	if( m_nRatioChangeHandDays <= 0 )
		return 5;
	return m_nRatioChangeHandDays;
}

void CStProfile::SetRatioVolumeDays( int nDays )
{
	m_nRatioVolumeDays	=	nDays;
}

int CStProfile::GetRatioVolumeDays( )
{
	if( m_nRatioVolumeDays <= 0 )
		return 5;
	return m_nRatioVolumeDays;
}

void CStProfile::SetRSDays( int nDays )
{
	m_nRSDays	=	nDays;
}

int CStProfile::GetRSDays( )
{
	if( m_nRSDays <= 0 )
		return 5;
	return m_nRSDays;
}

void CStProfile::SetYieldAverageDays( int nDays )
{
	m_nYieldAverageDays	=	nDays;
}

int CStProfile::GetYieldAverageDays( )
{
	if( m_nYieldAverageDays <= 0 )
		return 3*STKLIB_DAYS_INONEYEAR;
	return m_nYieldAverageDays;
}

int CStProfile::GetMaxDaysCalculate( )
{
	int	nMaxDays	=	GetDiffDays( );
	nMaxDays	=	max( GetDiffPercentDays( ), nMaxDays );
	nMaxDays	=	max( GetScopeDays( ), nMaxDays );
	nMaxDays	=	max( GetRatioChangeHandDays( ), nMaxDays );
	nMaxDays	=	max( GetRatioVolumeDays( ), nMaxDays );
	nMaxDays	=	max( GetRSDays( ), nMaxDays );

	SP_ASSERT( nMaxDays <= STKLIB_DAYS_CALCULATE );
	nMaxDays	=	min( STKLIB_DAYS_CALCULATE, nMaxDays );
	return nMaxDays;
}

void CStProfile::SetCurrentStock( int nStockIndex, BOOL bFromSList )
{
	m_bCurStockFromSList	=	bFromSList;
	if( m_bCurStockFromSList )
		AfxGetSListStockContainer().SetCurrentStock( nStockIndex );
	else
		AfxGetStockContainer().SetCurrentStock( nStockIndex );
}

void CStProfile::SetCurrentStock( const char * szCode, BOOL bFromSList )
{
	m_bCurStockFromSList	=	bFromSList;
	if( m_bCurStockFromSList )
		AfxGetSListStockContainer().SetCurrentStock( szCode );
	else
		AfxGetStockContainer().SetCurrentStock( szCode );
}

BOOL CStProfile::GetCurrentStock( CStockInfo * pInfo )
{
	if( m_bCurStockFromSList )
		return AfxGetSListStockContainer().GetCurrentStock( pInfo );
	else
		return AfxGetStockContainer().GetCurrentStock( pInfo );
}

BOOL CStProfile::GetPrevStock( CStockInfo * pInfo )
{
	if( m_bCurStockFromSList )
		return AfxGetSListStockContainer().GetPrevStock( pInfo );
	else
		return AfxGetStockContainer().GetPrevStock( pInfo );
}

BOOL CStProfile::GetNextStock( CStockInfo * pInfo )
{
	if( m_bCurStockFromSList )
		return AfxGetSListStockContainer().GetNextStock( pInfo );
	else
		return AfxGetStockContainer().GetNextStock( pInfo );
}

int CStProfile::GetInfoCurrentServer( )
{
	return m_nInfoCurrentServer;
}

void CStProfile::SetInfoCurrentServer( int nServer )
{
	m_nInfoCurrentServer	=	nServer;
}

void CStProfile::GetGraphTechsDefault( CSPDWordArray & anShow )
{
	// All Order
	m_anGraphTechsAll.RemoveAll();
	for( UINT i=STT_MIN; i<=STT_MAX; i++ )
	{
		m_anGraphTechsAll.Add( i );
	}
	
	// Íâ¹ÒÖ¸±ê
	UINT nTechUserCount = CTechUser::GetTechUserCount();
	for( UINT i=0; i<nTechUserCount; i++ )
	{
		m_anGraphTechsAll.Add( STT_USER_MIN+i );
	}

	// Show
	anShow.RemoveAll( );
	anShow.Add( STT_VOLUME );
	anShow.Add( STT_MACD );

	if( nTechUserCount > 0 )
	{
		anShow.RemoveAll();
		anShow.Add( STT_VOLUME );
		anShow.Add( STT_USER_MIN );
	}
}

CSPDWordArray & CStProfile::GetGraphTechsShow( )
{
	static	int count	=	0;
	count	++;
	if( m_anGraphTechsShow.GetSize() == 0 && count == 1 )
		GetGraphTechsDefault( m_anGraphTechsShow );

	// Remove some dulplicate
	for( int i=m_anGraphTechsShow.GetSize()-1; i>=0; i-- )
	{
		if( m_anGraphTechsShow[i] >= STT_KLINE_MIN && m_anGraphTechsShow[i] <= STT_KLINE_MAX )
		{
			m_anGraphTechsShow.RemoveAt( i );
			continue;
		}
		for( int k=0; k<i; k++ )
		{
			if( m_anGraphTechsShow[i] == m_anGraphTechsShow[k] )
			{
				m_anGraphTechsShow.RemoveAt( i );
				SP_ASSERT( FALSE );
				break;
			}
		}
	}

	return m_anGraphTechsShow;
}

CSPDWordArray & CStProfile::GetGraphTechsKLine( )
{
	static	int	count	=	0;
	count	++;
	if( m_anGraphTechsKLine.GetSize() == 0 && count == 1 )
		m_anGraphTechsKLine.Add( STT_MA );

	// Remove some dulplicate
	for( int i=m_anGraphTechsKLine.GetSize()-1; i>=0; i-- )
	{
		if( m_anGraphTechsKLine[i] < STT_KLINE_MIN || m_anGraphTechsKLine[i] > STT_KLINE_MAX )
		{
			m_anGraphTechsKLine.RemoveAt( i );
			continue;
		}
		for( int k=0; k<i; k++ )
		{
			if( m_anGraphTechsKLine[i] == m_anGraphTechsKLine[k] )
			{
				m_anGraphTechsKLine.RemoveAt( i );
				SP_ASSERT( FALSE );
				break;
			}
		}
	}

	return m_anGraphTechsKLine;
}

CSPDWordArray & CStProfile::GetGraphTechsAll( )
{
	int	nRealCount = STT_MAX-STT_MIN+1+CTechUser::GetTechUserCount();
	if( m_anGraphTechsAll.GetSize() != nRealCount )
		GetGraphTechsDefault( m_anGraphTechsShow );

	// Remove some dulplicate
	for( int i=m_anGraphTechsAll.GetSize()-1; i>=0; i-- )
	{
		for( int k=0; k<i; k++ )
		{
			if( m_anGraphTechsAll[i] == m_anGraphTechsAll[k] )
			{
				m_anGraphTechsAll.RemoveAt( i );
				SP_ASSERT( FALSE );
				break;
			}
		}
	}

	return m_anGraphTechsAll;
}

BOOL CStProfile::IsGraphTechShow( UINT nTech )
{
	SP_ASSERT( (nTech >= STT_MIN && nTech <= STT_MAX) || nTech >= STT_USER_MIN );

	CSPDWordArray	& anTech	=	GetGraphTechsShow( );
	CSPDWordArray	& anKLine	=	GetGraphTechsKLine( );
	for( int k=0; k<anTech.GetSize(); k++ )
	{
		if( anTech[k] == nTech )
			return TRUE;
	}
	for( int k=0; k<anKLine.GetSize(); k++ )
	{
		if( anKLine[k] == nTech )
			return TRUE;
	}
	return FALSE;
}

void CStProfile::ToggleGraphViewTech( UINT nTech )
{
	SP_ASSERT( (nTech >= STT_MIN && nTech <= STT_MAX) || nTech >= STT_USER_MIN );

	if( nTech >= STT_KLINE_MIN && nTech <= STT_KLINE_MAX )
	{
		BOOL	bHas	=	FALSE;
		CSPDWordArray	& anKLine	=	GetGraphTechsKLine( );
		for( int k=anKLine.GetSize()-1; k>=0; k-- )
		{
			if( anKLine[k] == nTech )
			{
				anKLine.RemoveAt(k);
				bHas	=	TRUE;
			}
		}
		if( !bHas )
			anKLine.InsertAt( 0, nTech );
	}
	else
	{
		BOOL	bHas	=	FALSE;
		CSPDWordArray	& anShow	=	GetGraphTechsShow( );
		for( int k=anShow.GetSize()-1; k>=0; k-- )
		{
			if( anShow[k] == nTech )
			{
				anShow.RemoveAt(k);
				bHas	=	TRUE;
			}
		}
		if( !bHas )
			anShow.Add( nTech );
	}
}

void CStProfile::ShowGraphViewTech( UINT nTech )
{
	SP_ASSERT( (nTech >= STT_MIN && nTech <= STT_MAX) || nTech >= STT_USER_MIN );

	if( nTech >= STT_KLINE_MIN && nTech <= STT_KLINE_MAX )
	{
		CSPDWordArray	& anKLine	=	GetGraphTechsKLine( );
		for( int k=anKLine.GetSize()-1; k>=0; k-- )
		{
			if( anKLine[k] == nTech )
			{
				anKLine.RemoveAt(k);
				anKLine.Add( nTech );
				return;
			}
		}
		if( anKLine.GetSize() > 0 )
			anKLine.SetAt( 0, nTech );
		else
			anKLine.Add( nTech );
	}
	else
	{
		CSPDWordArray	& anShow	=	GetGraphTechsShow( );
		for( int k=anShow.GetSize()-1; k>=0; k-- )
		{
			if( anShow[k] == nTech )
			{
				anShow.RemoveAt(k);
				anShow.Add( nTech );
				return;
			}
		}
		if( anShow.GetSize() >= 2 )
			anShow.SetAt( anShow.GetSize()-1, nTech );
		else
			anShow.Add( nTech );
	}
}

int CStProfile::GetGraphKFormat( )
{
	if( m_nGraphKFormat < CKData::formatMin
		|| m_nGraphKFormat > CKData::formatMax )
		m_nGraphKFormat	=	CKData::formatOriginal;

	return m_nGraphKFormat;
}

void CStProfile::SetGraphKFormat( int kformat )
{
	m_nGraphKFormat	=	kformat;
}

int CStProfile::GetGraphMaindataType( )
{
	if( m_nGraphMaindataType < CKData::mdtypeMin
		|| m_nGraphMaindataType > CKData::mdtypeMax )
		m_nGraphMaindataType	=	CKData::mdtypeClose;

	return m_nGraphMaindataType;
}

void CStProfile::SetGraphMaindataType( int maindatatype )
{
	m_nGraphMaindataType	=	maindatatype;
}

DWORD CStProfile::GetAutoResumeDRBegin( )
{
	if( 0 == m_dwAutoResumeDRBegin || -1 == m_dwAutoResumeDRBegin )
		m_dwAutoResumeDRBegin	=	STKLIB_AUTORESUMEDR_DATEBEGIN;
	return m_dwAutoResumeDRBegin;
}

void CStProfile::SetAutoResumeDRBegin( DWORD dateBegin )
{
	m_dwAutoResumeDRBegin	=	dateBegin;
}

BOOL CStProfile::IsValidFont( LPLOGFONT pLogFont )
{
	SP_ASSERT( pLogFont );
	if( NULL == pLogFont )
		return FALSE;
	for( int i=0; i<sizeof(LOGFONT); i++ )
	{
		BYTE	bt	=	*( ((LPBYTE)pLogFont) + i );
		if( 0 != bt )
			return TRUE;
	}
	return FALSE;
}

void CStProfile::GetFontSListView( LPLOGFONT pLogFont )
{
	SP_ASSERT( pLogFont );
	if( NULL == pLogFont )
		return;
	if( IsValidFont( &m_lfSListView ) )
	{
		memcpy(pLogFont, &m_lfSListView, sizeof(LOGFONT));
	}
	else
	{
#ifndef _WIN32_WCE
		// Initially use the system message font for the GridCtrl font
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		SP_VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));    
		memcpy(pLogFont, &(ncm.lfMessageFont), sizeof(LOGFONT));
#else
		LOGFONT lf;
		GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
		memcpy(pLogFont, &lf, sizeof(LOGFONT));
#endif
	}
}

void CStProfile::SetFontSListView( LPLOGFONT pLogFont )
{
	SP_ASSERT( pLogFont );
	if( pLogFont )
		memcpy(&m_lfSListView, pLogFont, sizeof(LOGFONT));
}

void CStProfile::GetFontBaseView( LPLOGFONT pLogFont )
{
	SP_ASSERT( pLogFont );
	if( NULL == pLogFont )
		return;
	if( IsValidFont( &m_lfBaseView ) )
	{
		memcpy(pLogFont, &m_lfBaseView, sizeof(LOGFONT));
	}
	else
	{
		LOGFONT	lf;
		::GetObject(::GetStockObject(SYSTEM_FIXED_FONT), sizeof(LOGFONT), &lf);
		memcpy(pLogFont, &lf, sizeof(LOGFONT));
	}
}

void CStProfile::SetFontBaseView( LPLOGFONT pLogFont )
{
	SP_ASSERT( pLogFont );
	if( pLogFont )
		memcpy(&m_lfBaseView, pLogFont, sizeof(LOGFONT));
}

CTechParameters & CStProfile::GetTechParameters( )
{
	return m_techparameters;
}

void CStProfile::AddHTMLURLToExplorer( LPCTSTR lpszURL )
{
	m_aHTMLURLToExplorer.Add( lpszURL );
}

CSPStringArray & CStProfile::GetHTMLURLToExplorer( )
{
	return m_aHTMLURLToExplorer;
}

BOOL CStProfile::GetRcvDriverDefault( CSPStringArray & astrProvider, CSPStringArray & astrDriver )
{
	astrProvider.Copy( m_astrProvider );
	astrDriver.Copy( m_astrDriver );
	return astrDriver.GetSize() > 0 ;
}

void CStProfile::SetRcvDriverDefault( CSPStringArray & astrProvider, CSPStringArray & astrDriver )
{
	m_astrProvider.Copy( astrProvider );
	m_astrDriver.Copy( astrDriver );
}

CSPString CStProfile::GetTrader( )
{
	return m_strTrader;
}

void CStProfile::SetTrader( LPCTSTR lpszFileTrader )
{
	m_strTrader	=	lpszFileTrader;
}

BOOL CStProfile::GetAutoSaveKDataMin( )
{
	return m_bAutoSaveKDataMin;
}

void CStProfile::SetAutoSaveKDataMin( BOOL bSave )
{
	m_bAutoSaveKDataMin = bSave;
}

BOOL CStProfile::GetNotifyIcon( )
{
	return m_bNotifyIcon;
}

void CStProfile::SetNotifyIcon( BOOL bNotifyIcon )
{
	m_bNotifyIcon = bNotifyIcon;
}

UINT CStProfile::GetCacheDays( )
{
	return m_nCacheDays;
}

void CStProfile::SetCacheDays( UINT nCacheDays )
{
	m_nCacheDays = nCacheDays;
}

CAlarmCondContainer & CStProfile::GetAlarmCondContainer( )
{
	return m_aAlarmCond;
}

int CStProfile::GetLocalDataVersion( )
{
	return m_nLocalDataVersion;
}

void CStProfile::SetLocalDataVersion( int nDataVersion )
{
	m_nLocalDataVersion = nDataVersion;
}

