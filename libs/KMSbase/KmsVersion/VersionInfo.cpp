
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsVersion/VersionAndType.cpp

// CODE REVIEW 2020-10-26 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include  <KmsLib/Exception.h>

// ===== KmsVersion =========================================================
#include  "VersionInfo.h"

// Public
/////////////////////////////////////////////////////////////////////////////

VersionInfo::VersionInfo() : mCountRC(0), mCountStr(0), mCountStr0(0), mCountType(0)
{
}

// Exception KmsLib::Exception *  CODE_INVALID_DATA
void VersionInfo::Validate(const Version & aVersion) const
{
    assert(NULL != &aVersion);

    if (   (1        != mCount.mMajor )
        || (1        != mCount.mMinor )
        || (1        != mCount.mBuild )
        || (1        != mCount.mCompat)
        || (1        != mCountRC      )
        || (1        != mCountStr     )
        || (1        != mCountStr0    )
        || (1        >  mCountType    )
        || (aVersion != mVersionRC    )
        || (aVersion != mVersionStr   )
        || (aVersion != mVersionStr0  ))
    {
        throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The master file is inconsistent", NULL, __FILE__, __FUNCTION__, __LINE__, 0);
    }
}
