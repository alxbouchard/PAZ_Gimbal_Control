
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsVersion/VersionAndType.cpp

#include "Component.h"

// ===== C ==================================================================
#include  <string.h>

// ===== Includes ===========================================================
#include  <KmsLib/Exception.h>

// ===== KmsVersion =========================================================
#include  "VersionAndType.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define TYPE_EXTERNAL ""
#define TYPE_INTERNAL "Internal"
#define TYPE_UNKNOWN  "Unknown"

// Public
/////////////////////////////////////////////////////////////////////////////

VersionAndType::VersionAndType()
{
    SetType(TYPE_UNKNOWN);
}

// Return  This method returns a pointer to a constant.
const char * VersionAndType::GetInternal() const
{
    return IsInternal() ? "Internal" : "External";
}

bool VersionAndType::IsExternal() const
{
    return (0 == strcmp(TYPE_EXTERNAL, mType));
}

bool VersionAndType::IsInternal() const
{
    return (0 != strcmp(TYPE_EXTERNAL, mType));
}

void VersionAndType::SetExternal()
{
    SetType(TYPE_EXTERNAL);
}

void VersionAndType::SetInternal(bool aInternal)
{
    if (aInternal)
    {
        if ((0 == strcmp(TYPE_EXTERNAL, mType)) || (0 == strcmp(TYPE_UNKNOWN, mType)))
        {
            SetType(TYPE_INTERNAL);
        }
    }
    else
    {
        SetType(TYPE_EXTERNAL);
    }
}

// Exception KmsLib::Exception *  CODE_INVALID_DATA
void VersionAndType::SetInternal(const char * aInternal)
{
    assert(NULL != aInternal);

    if      (0 == strcmp("false", aInternal)) { SetInternal(false); }
    else if (0 == strcmp("true" , aInternal)) { SetInternal(true ); }
    else
    {
        char  lMsg[2048];

        sprintf_s(lMsg, "Invalid Internal value - \"%s\"", aInternal);

        throw  new  KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "Invalid Internal value", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
    }
}

void  VersionAndType::SetType(const  char  * aType)
{
    assert(NULL != aType);

    strcpy_s(mType, aType);
}
