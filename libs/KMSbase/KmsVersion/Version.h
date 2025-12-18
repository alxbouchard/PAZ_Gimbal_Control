
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsVersion/Version.h

// CODE REVIEW 2020-11-05 KMS - Martin Dubois, P.Eng.

#pragma once

// Classe
/////////////////////////////////////////////////////////////////////////////

class Version
{

public:

    Version();

    bool operator == (const Version & aB) const;
    bool operator != (const Version & aB) const;

    const char * GetText1(          ) const;
    const char * GetText2(          ) const;
    const char * GetText3(char aS0 = '.', char aS1 = '.') const;
    const char * GetText4(char  aSep) const;

    void Parse(const char * aIn, char aSep);

    unsigned int mMajor ;
    unsigned int mMinor ;
    unsigned int mBuild ;
    unsigned int mCompat;

};
