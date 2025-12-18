
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsVersion/VersionAndType.cpp

#include "Component.h"

// ===== C ==================================================================
#include <stdio.h>

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>

// ===== KmsVersion =========================================================
#include "Version.h"

// Public
/////////////////////////////////////////////////////////////////////////////

Version::Version() : mMajor(0), mMinor(0), mBuild(0), mCompat(0)
{
}

bool Version::operator == (const Version & aB) const
{
    return  ((mMajor == aB.mMajor) && (mMinor == aB.mMinor) && (mBuild == aB.mBuild) && (mCompat == aB.mCompat));
}

bool Version::operator != (const Version & aB) const
{
    return  ((mMajor != aB.mMajor) || (mMinor != aB.mMinor) || (mBuild != aB.mBuild) || (mCompat != aB.mCompat));
}

// LIMITATION KmsVersion.Single-thread
//            The methods Version::GetText* are not thread safe.

// Return  This method returns the address of an internal buffer.
const char * Version::GetText1() const
{
    static char sText[64];

    sprintf_s(sText, "%u", mMajor);

    return sText;
}

// Return  This method returns the address of an internal buffer.
const char * Version::GetText2() const
{
    static char sText[64];

    sprintf_s(sText, "%u.%u", mMajor, mMinor);

    return sText;
}

// Return  This method returns the address of an internal buffer.
const char * Version::GetText3(char aS0, char aS1) const
{
    static char sText[64];

    sprintf_s(sText, "%u%c%u%c%u", mMajor, aS0, mMinor, aS1, mBuild);

    return sText;
}

// aSep  The separator
//
// Return  This method returns the address of an internal buffer.
const char * Version::GetText4(char aSep) const
{
    static char sText[64];

    switch (aSep)
    {
    case '.': sprintf_s(sText, "%u.%u.%u.%u", mMajor, mMinor, mBuild, mCompat); break;
    case ',': sprintf_s(sText, "%u,%u,%u,%u", mMajor, mMinor, mBuild, mCompat); break;

    default: assert(false);
    }

    return sText;
}

// aIn   The text to parse
// aSep  The separator
//
// Exception KmsLib::Exception *  CODE_INVALID_DATA
void Version::Parse(const char * aIn, char aSep)
{
    assert(NULL != aIn);

    int lRet;

    switch (aSep)
    {
    case '.': lRet = sscanf_s(aIn, "%u.%u.%u.%u", &mMajor, &mMinor, &mBuild, &mCompat); break;
    case ',': lRet = sscanf_s(aIn, "%u,%u,%u,%u", &mMajor, &mMinor, &mBuild, &mCompat); break;

    default: assert(false);
    }

    if (4 != lRet)
    {
        char lMsg[2048];

        sprintf_s(lMsg, "\"%s\" is not a valid version number", aIn);

        throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "Invalid version number", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
    }
}
