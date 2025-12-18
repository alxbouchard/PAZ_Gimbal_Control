
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Component.h

#pragma once

// ===== C ==================================================================
#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>

// ===== C++ ================================================================
#include <exception>

// Macros
/////////////////////////////////////////////////////////////////////////////

#define TRACE_DEBUG(S,M)   if (NULL != (S)) fprintf((S),"DEBUG  %s\n", (M))
#define TRACE_ERROR(S,M)   if (NULL != (S)) fprintf((S),"ERROR  %s\n", (M))
#define TRACE_INFO(S,M)    if (NULL != (S)) fprintf((S),"INFO  %s\n", (M))
#define TRACE_WARNING(S,M) if (NULL != (S)) fprintf((S),"WARNING  %s\n", (M))

#define TRACE_RESULT(S, R) \
    if (ZT::ZT_OK != (R)) { fprintf((S), "ERROR  %s (%u) - %s\n", __FUNCTION__, __LINE__, Result_GetName(R)); }
