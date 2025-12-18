
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/ControlLink.cpp

#include "Component.h"

// ===== ZTLib ==============================================================
#include "Value.h"

// Functions
// //////////////////////////////////////////////////////////////////////////

double Value_Limit(double aValue, double aMin, double aMax)
{
    assert(aMin < aMax);

    if (aMin > aValue)
    {
        return aMin;
    }

    if (aMax < aValue)
    {
        return aMax;
    }

    return aValue;
}

ZT::Result Value_Validate(double aValue, double aMin, double aMax)
{
    assert(aMin < aMax);

    if (aMin > aValue)
    {
        return ZT::ZT_ERROR_MIN;
    }

    if (aMax < aValue)
    {
        return ZT::ZT_ERROR_MAX;
    }

    return ZT::ZT_OK;
}
