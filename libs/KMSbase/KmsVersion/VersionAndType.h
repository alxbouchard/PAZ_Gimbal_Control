
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsVersion/VersionAndType.h

#pragma once

// ===== KmsVersion =========================================================
#include  "Version.h"

// Classe
/////////////////////////////////////////////////////////////////////////////

class VersionAndType : public Version
{

public:

    VersionAndType();

    const char * GetInternal() const;

    bool IsExternal() const;
    bool IsInternal() const;

    void SetExternal();
    void SetInternal(bool aInternal = true);
    void SetInternal(const char * aInternal);
    void SetType    (const char * aType);

    char mType[16];

};
