
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsVersion/Version.h

#pragma once

// ===== KmsVersion =========================================================
#include  "Version.h"

// Classe
/////////////////////////////////////////////////////////////////////////////

class VersionInfo
{

public:

    VersionInfo();

    void Validate(const Version & aVersion) const;

    Version      mCount      ;
    unsigned int mCountRC    ;
    unsigned int mCountStr   ;
    unsigned int mCountStr0  ;
    unsigned int mCountType  ;
    Version      mVersionRC  ;
    Version      mVersionStr ;
    Version      mVersionStr0;

};

