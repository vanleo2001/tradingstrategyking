
#pragma once

#include "TSKTabManager.h"

//=============================================================================
// CTSKTabControl

class CTSKTabControl : public CWnd, public CTSKTabManager
{
	DECLARE_DYNAMIC(CTSKTabControl)
public:
	CTSKTabControl();
	virtual ~CTSKTabControl();

protected:
	DECLARE_MESSAGE_MAP()
};
