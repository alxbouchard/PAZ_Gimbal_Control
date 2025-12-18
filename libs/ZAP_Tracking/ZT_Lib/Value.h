
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/ControlLink.cpp

#pragma once

// ===== Includes ===========================================================
#include <ZT/Result.h>

// Functions
// //////////////////////////////////////////////////////////////////////////

extern double Value_Limit(double aValue, double aMin, double aMax);

extern ZT::Result Value_Validate(double aValue, double aMin, double aMax);
