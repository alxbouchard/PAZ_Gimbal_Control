
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KmsBase

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS
/// \file      Includes/KmsTool.h
/// \brief     KMS_TOOL_BANNER

#pragma once

// Macros
/////////////////////////////////////////////////////////////////////////////

#if ( ! defined( _DEBUG ) ) || defined( NDEBUG )

    #define KMS_TOOL_BANNER(P,C,V,T)                            \
        printf( P " - " C "\n" );                               \
        printf( "Version " V " " T "\n" );                      \
        printf( "Compiled at " __TIME__ " on " __DATE__ "\n" ); \
        printf( "http://www.kms-quebec.com\n")

#else

    #define KMS_TOOL_BANNER(P,C,V,T)                            \
        printf( P " - " C "\n" );                               \
        printf( "Version " V " " T " (Debug)\n" );              \
        printf( "Compiled at " __TIME__ " on " __DATE__ "\n" ); \
        printf( "http://www.kms-quebec.com\n")

#endif
