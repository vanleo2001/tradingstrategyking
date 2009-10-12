/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Packet convertor functions.
*/

#if	!defined( __STKLIB_DAYTRADER_H__ )
#define	__STKLIB_DAYTRADER_H__

#include "StdAfx.h"

class STKLIB_API CDayTrader : public CStrategy
{
public:
	CDayTrader();
	virtual ~CDayTrader();

	BOOL	SetMaxStocks( int nMaxStocks );
	BOOL	BuyIt( CSPTime tmCur, CTechStock & techstock );
	BOOL	SellIt( CSPTime tmCur, CTechStock & techstock );


	virtual	BOOL	SimuRun( SIMULATION_CALLBACK fnCallback, void * cookie );

protected:
};

#endif	// __STKLIB_DAYTRADER_H__

