
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2019-2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsVersion/KmsVersion.cpp

#include "Component.h"

// ===== C ==================================================================
#include <stdio.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== KmsBase ============================================================
#include <KmsLib/CmdLineParser.h>
#include <KmsTool.h>

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
    #include <KmsLib/Linux/Windows.h>
#endif

// ===== Common =============================================================
#include "../Common/Version.h"

// ===== Local ==============================================================
#include "VersionAndType.h"
#include "VersionInfo.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define OPTS "?234ABCIMSTabcehimqt"

#define OPT_2              '2'
#define OPT_3              '3'
#define OPT_4              '4'
#define OPT_MAJOR_A        'A'
#define OPT_BUILD_B        'B'
#define OPT_COMPAT_C       'C'
#define OPT_INTERNAL_I     'I'
#define OPT_MINOR_M        'M'
#define OPT_SCRIPT_S       'S'
#define OPT_TYPE_T         'T'
#define OPT_INC_MAJOR_a    'a'
#define OPT_INC_BUILD_b    'b'
#define OPT_INC_COMPAT_c   'c'
#define OPT_SET_EXTERNAL_e 'e'
#define OPT_HELP_h         'h'
#define OPT_SET_INTERNAL_i 'i'
#define OPT_INC_MINOR_m    'm'
#define OPT_QUIET_q        'q'
#define OPT_TXT_t          't'

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Command_Execute(FILE * aRead, FILE * aWrite, const Version & aVersion, unsigned int aN, const char * aBefore, const char * aAfter);

static void DisplayHelp       (FILE * aOut);
static void DisplayUserError  (const char * aMsg);
static void DisplayVersionInfo(const VersionAndType & aVersion, const KmsLib::CmdLineParser & aCLP);

static void File_Close (FILE * aRead);
static void File_Close (FILE * aRead, FILE * aWrite);
static void File_Open  (const char * aFileName, FILE ** aRead);
static void File_Open  (const char * aFileName, FILE ** aRead, FILE ** aWrite);
static void File_Revert(const char * aFileName);

static void Master_Parse         (const char * aMaster, VersionAndType * aOut);
static void Master_ParseAndModify(const char * aMaster, VersionAndType * aOut, const KmsLib::CmdLineParser & aCLP);

static void Process              (Version        * aVersion, const KmsLib::CmdLineParser & aCLP);
static void Process_Build        (Version        * aVersion, const KmsLib::CmdLineParser & aCLP);
static void Process_Compatibility(Version        * aVersion, const KmsLib::CmdLineParser & aCLP);
static void Process_Internal     (VersionAndType * aVersion, const KmsLib::CmdLineParser & aCLP);
static void Process_Major        (Version        * aVersion, const KmsLib::CmdLineParser & aCLP);
static void Process_Minor        (Version        * aVersion, const KmsLib::CmdLineParser & aCLP);
static void Process_Type         (VersionAndType * aVersion, const KmsLib::CmdLineParser & aCLP);

static bool ReplaceVersion(char * aLine, FILE * aWrite, const Version & aVerstion, unsigned int aNumber, const char * aBefore, const char * aAfter);

static int  Script_Execute(const KmsLib::CmdLineParser & aCLP, const VersionAndType & aV);

static void Slave_Modify    (const KmsLib::CmdLineParser & aCLP   , const Version & aVersion);
static void Slave_Modify    (const char                  * aSlave , const Version & aVersion);
static void Slave_Modify_CMD(const char                  * aSlave , const Version & aVersion);
static void Slave_Modify_TXT(const KmsLib::CmdLineParser & aCLP   , const Version & aVersion);
static void Slave_Modify_TXT(const char                  * aSlave , const Version & aVersion);
static void Slave_Modify_XML(const char                  * aSlave , const Version & aVersion);

static void String_Escape(char * aInOut);

static void  String_Trunk(char * aStr, char aEnd);

#ifdef _KMS_WINDOWS_
    static void Slave_Modify_INF(const char * aSlave , const Version & aVersion);
    static void Slave_Modify_ISS(const char * aSlave , const Version & aVersion);
#endif

// Entry point
/////////////////////////////////////////////////////////////////////////////

// REQUIREMENT KmsVersion.Master.Parse
//             KmsVersion extracts version information from a master header file and validate this information.

// REQUIREMENT KmsVersion.Master.Modify
//             KmsVersion provides function to modify the master header file.

int main(int aCount, const char ** aVector)
{
    assert(1    <= aCount );
    assert(NULL != aVector);

    KmsLib::CmdLineParser lCLP(OPTS, "");

    lCLP.Parse(aCount, aVector);

    if (!lCLP.IsPresent(OPT_QUIET_q))
    {
        KMS_TOOL_BANNER("KmsBase", "KmsVersion", VERSION_STR, VERSION_TYPE);
    }

    if (0 != lCLP.GetErrorFlags())
    {
        DisplayUserError("The command line is not valid!");
        return 1;
    }

    if (lCLP.IsPresent('?') || lCLP.IsPresent(OPT_HELP_h))
    {
        DisplayHelp(stdout);
        return 0;
    }

    int lResult;

    if (lCLP.IsPresent(0))
    {
        try
        {
            VersionAndType lVT;

            char lMaster[2048];

            lCLP.GetData(0, lMaster, sizeof(lMaster));

            if (   lCLP.IsPresent(OPT_INC_MAJOR_a   )
                || lCLP.IsPresent(OPT_INC_MINOR_m   )
                || lCLP.IsPresent(OPT_INC_BUILD_b   )
                || lCLP.IsPresent(OPT_INC_COMPAT_c  )
                || lCLP.IsPresent(OPT_SET_EXTERNAL_e)
                || lCLP.IsPresent(OPT_SET_INTERNAL_i))
            {
                Master_ParseAndModify(lMaster, &lVT, lCLP);
            }
            else
            {
                Master_Parse(lMaster, &lVT);
            }

            DisplayVersionInfo(lVT, lCLP);

            if (lCLP.IsPresent(OPT_SCRIPT_S))
            {
                lResult = Script_Execute(lCLP, lVT);
            }
            else if (lCLP.IsPresent(OPT_TXT_t))
            {
                Slave_Modify_TXT(lCLP, lVT);
                lResult = 0;
            }
            else
            {
                Slave_Modify(lCLP, lVT);
                lResult = 0;
            }
        }
        catch (KmsLib::Exception * eE)
        {
            fprintf(stderr, "\nEXCEPTION :\n");
            eE->Write(stderr);
            lResult = 2;
        }
    }
    else
    {
        DisplayUserError("The command line is not valid!");
        lResult = 3;
    }

    return lResult;
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

void Command_Execute(FILE * aRead, FILE * aWrite, const Version & aVersion, unsigned int aN, const char * aBefore, const char * aAfter)
{
    assert(NULL != aRead);
    assert(NULL != aWrite);
    assert(NULL != &aVersion);
    assert(NULL != aBefore);
    assert(NULL != aAfter);

    char lLine[1024];

    if (NULL == fgets(lLine, sizeof(lLine), aRead))
    {
        throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The KmsVersion instruction cannot be executed", NULL, __FILE__, __FUNCTION__, __LINE__, 0);
    }

    if (!ReplaceVersion(lLine, aWrite, aVersion, aN, aBefore, aAfter))
    {
        throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The KmsVersion instruction failed", NULL, __FILE__, __FUNCTION__, __LINE__, 0);
    }
}

void DisplayHelp(FILE * aOut)
{
    assert(NULL != aOut);

    fprintf(aOut,
        "KmsVersion.exe [Options] {Master} [Slave|Script] ...\n"
        "   Options\n"
        "      ?   Display this message\n"
        "      B   Read the build number\n"                   // OPT_BUILD_B
        "      C   Read the compatibility number\n"           // OPT_COMPAT_C
        "      I   Read the internal status\n"                // OPT_INTERNAL_I
        "      M   Read the minor number\n"                   // OPT_MINOR_M
        "      S   Execute scripts"                           // OPT_SCRIPT_S
        "      T   Read the version type\n"                   // OPT_TYPE_T
        "      a   Increment the major number\n"              // OPT_INC_MAJOR_a
        "      b   Increment the build number\n"              // OPT_INC_BUILD_B
        "      c   Increment the compatibility number\n"      // OPT_INC_COMPAT_c
        "      e   Set external\n"                            // OPT_SET_EXTERNAL_e
        "      h   Display this message\n"                    // OPT_HELP_h
        "      i   Set internal\n"                            // OPT_SET_INTERNAL_i
        "      m   Increment the minor number\n"              // OPT_INC_MINOR_m
        "      q   Quiet\n"                                   // OPT_QUIET_q
        "      t   Use txt file type for all slave files\n"); // OPT_TXT_t
}

void DisplayUserError(const char * aMsg)
{
    assert(NULL != aMsg);

    fprintf(stderr, "\nUSER ERROR : %s\n\n", aMsg);

    DisplayHelp(stderr);
}

// REQUIREMENT KmsVersion.Display
//             KmsVersion displays the version in difference formats.

void DisplayVersionInfo(const VersionAndType & aVT, const KmsLib::CmdLineParser & aCLP)
{
    assert(NULL != &aVT );
    assert(NULL != &aCLP);

    if (aCLP.IsPresent(OPT_MAJOR_A   )) { printf("%s\n", aVT.GetText1()   ); }
    if (aCLP.IsPresent(OPT_BUILD_B   )) { printf("%u\n", aVT.mBuild       ); }
    if (aCLP.IsPresent(OPT_COMPAT_C  )) { printf("%u\n", aVT.mCompat      ); }
    if (aCLP.IsPresent(OPT_INTERNAL_I)) { printf("%s\n", aVT.GetInternal()); }
    if (aCLP.IsPresent(OPT_MINOR_M   )) { printf("%u\n", aVT.mMinor       ); }
    if (aCLP.IsPresent(OPT_TYPE_T    )) { printf("%s\n", aVT.mType        ); }
    if (aCLP.IsPresent(OPT_2         )) { printf("%s\n", aVT.GetText2()   ); }
    if (aCLP.IsPresent(OPT_3         )) { printf("%s\n", aVT.GetText3()   ); }
    if (aCLP.IsPresent(OPT_4         )) { printf("%s\n", aVT.GetText4('.')); }
}

void File_Close(FILE * aRead)
{
    assert(NULL != aRead);

    int lRet = fclose(aRead);
    assert(0 == lRet);
}

void  File_Close(FILE * aRead, FILE * aWrite)
{
    assert(NULL != aRead );
    assert(NULL != aWrite);

    int lRet = fclose(aRead);
    assert(0 == lRet);

    lRet = fclose(aWrite);
    assert(0 == lRet);
}

// aRead [---;-W-]
//
// Exception KmsLib::Exception *  CODE_IO_ERROR
void File_Open(const char * aFileName, FILE ** aRead)
{
    assert(NULL != aFileName);
    assert(NULL != aRead    );

    (*aRead) = NULL;

    int  lRet = fopen_s(aRead, aFileName, "r");
    if (0 != lRet)
    {
        char  lMsg[2048];
        sprintf_s(lMsg, "fopen( , \"%s\",  ) failed", aFileName);
        throw new KmsLib::Exception(KmsLib::Exception::CODE_IO_ERROR, "fopen( , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
    }
}

// aRead       [---;-W-]
// aWrite      [---;-W-]
//
// Exception KmsLib::Exception *  CODE_IO_ERROR
void  File_Open(const char * aFileName, FILE ** aRead, FILE ** aWrite)
{
    assert(NULL != aFileName);
    assert(NULL != aRead    );
    assert(NULL != aWrite   );

    *aRead  = NULL;
    *aWrite = NULL;

    char lMsg[2048];

    char lBackup[2048];

    sprintf_s(lBackup, "%s.bak", aFileName);

    if (!CopyFile(aFileName, lBackup, FALSE))
    {
        sprintf_s(lMsg, "CopyFile( \"%s\", \"%s\",  ) failed", aFileName, lBackup);
        throw new KmsLib::Exception(KmsLib::Exception::CODE_IO_ERROR, "CopyFile( , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
    }

    File_Open(lBackup, aRead);

    int  lRet = fopen_s(aWrite, aFileName, "w");
    if (0 != lRet)
    {
        sprintf_s(lMsg, "fopen( , \"%s\",  ) failed", aFileName);
        throw new KmsLib::Exception(KmsLib::Exception::CODE_IO_ERROR, "fopen( , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
    }
}

// Exception KmsLib::Exception *  CODE_IO_ERROR
void File_Revert(const char * aFileName)
{
    assert(NULL != aFileName);

    char lBackup[2048];

    sprintf_s(lBackup, "%s.bak", aFileName);

    if (!CopyFile(lBackup, aFileName, FALSE))
    {
        // NOT TESTED KmsVersion.ModifySlave.Error
        //            Fail to backup slave file.
        char  lMsg[2048];
        sprintf_s(lMsg, "CopyFile( \"%s\", \"%s\",  ) failed", lBackup, aFileName);
        throw new KmsLib::Exception(KmsLib::Exception::CODE_IO_ERROR,
            "CopyFile( , ,  ) failed", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
    }
}

// aVT [---;-W-]
//
// Exception KmsLib::Exception *  CODE_INVALID_DATA, CODE_IO_ERROR
void Master_Parse(const char * aMaster, VersionAndType * aVT)
{
    assert(NULL != aMaster);
    assert(NULL != aVT    );

    FILE * lRead;

    File_Open(aMaster, &lRead);

    VersionInfo lInfo;

    char lLine[2048];
    char lStr [  64];

    while (NULL != fgets(lLine, sizeof(lLine), lRead))
    {
        if      (1 == sscanf_s(lLine, "#define VERSION_MAJOR (%u)"        , &aVT->mMajor )) { lInfo.mCount.mMajor ++; }
        else if (1 == sscanf_s(lLine, "#define VERSION_MINOR (%u)"        , &aVT->mMinor )) { lInfo.mCount.mMinor ++; }
        else if (1 == sscanf_s(lLine, "#define VERSION_BUILD (%u)"        , &aVT->mBuild )) { lInfo.mCount.mBuild ++; }
        else if (1 == sscanf_s(lLine, "#define VERSION_COMPATIBILITY (%u)", &aVT->mCompat)) { lInfo.mCount.mCompat++; }
        else if (1 == sscanf_s(lLine, "#define VERSION_INTERNAL (%s"      , lStr SIZE_INFO(sizeof(lStr))))
        {
            String_Trunk(lStr, ')');
            aVT->SetInternal(lStr);
            lInfo.mCountType++;
        }
        else if (1 == sscanf_s(lLine, "#define VERSION_RC %s"             , lStr SIZE_INFO(sizeof(lStr))))
        {
            lInfo.mVersionRC.Parse(lStr, ',');
            lInfo.mCountRC++;
        }
        else if (1 == sscanf_s(lLine, "#define VERSION_STR \"%s\""        , lStr SIZE_INFO(sizeof(lStr))))
        {
            String_Trunk(lStr, '"');
            lInfo.mVersionStr.Parse(lStr, '.');
            lInfo.mCountStr++;
        }
        else if (1 == sscanf_s(lLine, "#define VERSION_STR0 \"%s"         , lStr SIZE_INFO(sizeof(lStr))))
        {
            String_Trunk(lStr, '\\');
            lInfo.mVersionStr0.Parse(lStr, '.');
            lInfo.mCountStr0++;
        }
        else if (1 == sscanf_s(lLine, "#define VERSION_TYPE \"%s"         , lStr SIZE_INFO(sizeof(lStr))))
        {
            String_Trunk(lStr, '"');
            aVT->SetType(lStr);
            lInfo.mCountType++;
        }
    }

    File_Close(lRead);

    lInfo.Validate(*aVT);
}

// aVT [---;-W-]
void Master_ParseAndModify(const char * aMaster, VersionAndType * aVT, const KmsLib::CmdLineParser & aCLP)
{
    assert(NULL !=  aMaster);
    assert(NULL !=  aVT    );
    assert(NULL != &aCLP   );

    FILE * lRead ;
    FILE * lWrite;

    File_Open(aMaster, &lRead, &lWrite);

    VersionInfo lInfo;

    char lStr [  64];
    char lLine[2048];

    try
    {
        while (NULL != fgets(lLine, sizeof(lLine), lRead))
        {
            if (1 == sscanf_s(lLine, "#define VERSION_MAJOR (%u)", &aVT->mMajor))
            {
                Process_Major(aVT, aCLP);
                fprintf(lWrite, "#define  VERSION_MAJOR           (%u)\n", aVT->mMajor);
                lInfo.mCount.mMajor++;
            }
            else if (1 == sscanf_s(lLine, "#define VERSION_MINOR (%u)", &aVT->mMinor))
            {
                Process_Minor(aVT, aCLP);
                fprintf(lWrite, "#define  VERSION_MINOR           (%u)\n", aVT->mMinor);
                lInfo.mCount.mMinor++;
            }
            else if (1 == sscanf_s(lLine, "#define VERSION_BUILD (%u)", &aVT->mBuild))
            {
                Process_Build(aVT, aCLP);
                fprintf(lWrite, "#define  VERSION_BUILD           (%u)\n", aVT->mBuild);
                lInfo.mCount.mBuild++;
            }
            else if (1 == sscanf_s(lLine, "#define VERSION_COMPATIBILITY (%u)", &aVT->mCompat))
            {
                Process_Compatibility(aVT, aCLP);
                fprintf(lWrite, "#define  VERSION_COMPATIBILITY   (%u)\n", aVT->mCompat);
                lInfo.mCount.mCompat++;
            }
            else if (1 == sscanf_s(lLine, "#define VERSION_INTERNAL (%s", lStr SIZE_INFO(sizeof(lStr))))
            {
                String_Trunk(lStr, ')');
                aVT->SetInternal(lStr);
                Process_Internal(aVT, aCLP);
                fprintf(lWrite, "#define  VERSION_INTERNAL  (%s)\n", aVT->IsInternal() ? "true" : "false");
                lInfo.mCountType++;
            }
            else if (1 == sscanf_s(lLine, "#define VERSION_RC %s", lStr SIZE_INFO(sizeof(lStr))))
            {
                lInfo.mVersionRC.Parse(lStr, ',');
                Process(&lInfo.mVersionRC, aCLP);
                fprintf(lWrite, "#define  VERSION_RC     %s\n", lInfo.mVersionRC.GetText4(','));
                lInfo.mCountRC++;
            }
            else if (1 == sscanf_s(lLine, "#define VERSION_STR \"%s", lStr SIZE_INFO(sizeof(lStr))))
            {
                String_Trunk(lStr, '"');
                lInfo.mVersionStr.Parse(lStr, '.');
                Process(&lInfo.mVersionStr, aCLP);
                fprintf(lWrite, "#define  VERSION_STR    \"%s\"\n", lInfo.mVersionStr.GetText4('.'));
                lInfo.mCountStr++;
            }
            else if (1 == sscanf_s(lLine, "#define VERSION_STR0 \"%s", lStr SIZE_INFO(sizeof(lStr))))
            {
                String_Trunk(lStr, '\\');
                lInfo.mVersionStr0.Parse(lStr, '.');
                Process(&lInfo.mVersionStr0, aCLP);
                fprintf(lWrite, "#define  VERSION_STR0   \"%s\\0\"\n", lInfo.mVersionStr0.GetText4('.'));
                lInfo.mCountStr0++;
            }
            else if (1 == sscanf_s(lLine, "#define VERSION_TYPE \"%s", lStr SIZE_INFO(sizeof(lStr))))
            {
                String_Trunk(lStr, '"');
                aVT->SetType(lStr);
                Process_Type(aVT, aCLP);
                fprintf(lWrite, "#define  VERSION_TYPE  \"%s\"\n", aVT->mType);
                lInfo.mCountType++;
            }
            else
            {
                fprintf(lWrite, "%s", lLine);
            }
        }

        lInfo.Validate(*aVT);
    }
    catch (...)
    {
        File_Close (lRead, lWrite);
        File_Revert(aMaster);
        throw;
    }

    File_Close(lRead, lWrite);
}

void Process(Version * aVersion, const KmsLib::CmdLineParser & aCLP)
{
    assert(NULL !=  aVersion);
    assert(NULL != &aCLP    );

    Process_Major        (aVersion, aCLP);
    Process_Minor        (aVersion, aCLP);
    Process_Build        (aVersion, aCLP);
    Process_Compatibility(aVersion, aCLP);
}

void Process_Build(Version * aVersion, const KmsLib::CmdLineParser & aCLP)
{
    assert(NULL !=  aVersion);
    assert(NULL != &aCLP    );

    if (aCLP.IsPresent(OPT_INC_MAJOR_a)) { aVersion->mBuild = 0; }
    if (aCLP.IsPresent(OPT_INC_MINOR_m)) { aVersion->mBuild = 0; }
    if (aCLP.IsPresent(OPT_INC_BUILD_b)) { aVersion->mBuild ++ ; }
}

void Process_Compatibility(Version * aVersion, const KmsLib::CmdLineParser & aCLP)
{
    assert(NULL !=  aVersion);
    assert(NULL != &aCLP    );

    if (aCLP.IsPresent(OPT_INC_COMPAT_c)) { aVersion->mCompat++; }
}

void Process_Internal(VersionAndType * aVT, const  KmsLib::CmdLineParser & aCLP)
{
    assert(NULL !=  aVT );
    assert(NULL != &aCLP);

    if (aCLP.IsPresent(OPT_SET_EXTERNAL_e)) { aVT->SetExternal(); }
    if (aCLP.IsPresent(OPT_SET_INTERNAL_i)) { aVT->SetInternal(); }
}

void Process_Major(Version * aVersion, const KmsLib::CmdLineParser & aCLP)
{
    assert(NULL !=  aVersion);
    assert(NULL != &aCLP    );

    if (aCLP.IsPresent(OPT_INC_MAJOR_a)) { aVersion->mMajor ++; }
}

void Process_Minor(Version * aVersion, const KmsLib::CmdLineParser & aCLP)
{
    assert(NULL !=  aVersion);
    assert(NULL != &aCLP    );

    if (aCLP.IsPresent(OPT_INC_MAJOR_a)) { aVersion->mMinor = 0; }
    if (aCLP.IsPresent(OPT_INC_MINOR_m)) { aVersion->mMinor  ++; }
}

void Process_Type(VersionAndType * aVT, const KmsLib::CmdLineParser & aCLP)
{
    assert(NULL !=  aVT );
    assert(NULL != &aCLP);

    if (aCLP.IsPresent(OPT_SET_EXTERNAL_e))
    {
        aVT->SetExternal();
    }

    if (aCLP.IsPresent(OPT_SET_INTERNAL_i))
    {
        if (aVT->IsExternal())
        {
            aVT->SetInternal();
        }
    }
}

// Return
//  false No problem, the string aLine have not been modified
//  true  Replaced
//
// Exception KmsLib::Exception *  CODE_INVALID_ARGUMENT
bool ReplaceVersion(char * aLine, FILE * aWrite, const Version & aVersion, unsigned int aNumber, const char * aBefore, const char * aAfter)
{
    assert(NULL !=  aLine   );
    assert(NULL !=  aWrite  );
    assert(NULL != &aVersion);
    assert(NULL !=  aBefore );
    assert(NULL !=  aAfter  );

    char * lBefore = strstr(aLine, aBefore);
    if (NULL == lBefore)
    {
        return false;
    }

    lBefore += strlen(aBefore);

    char * lAfter = strstr(lBefore, aAfter);
    if (NULL == lAfter)
    {
        return false;
    }

    (*lBefore) = '\0';

    fprintf(aWrite, "%s", aLine);
    
    const char * lVersion;

    switch (aNumber)
    {
    case 1: lVersion = aVersion.GetText1(   ); break;
    case 2: lVersion = aVersion.GetText2(   ); break;
    case 3: lVersion = aVersion.GetText3(   ); break;
    case 4: lVersion = aVersion.GetText4('.'); break;

    case 13: lVersion = aVersion.GetText3('.', '-'); break;

    default: throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_ARGUMENT, "Invalid aNumber value", NULL, __FILE__, __FUNCTION__, __LINE__, aNumber);
    }

    fprintf(aWrite, "%s%s", lVersion, lAfter);

    return true;
}

// Return  APP RESULT CODE
//  0      OK
//  Other  Error
int Script_Execute(const KmsLib::CmdLineParser & aCLP, const VersionAndType & aVT)
{
    assert(NULL != &aCLP);
    assert(NULL != &aVT );

    int lResult = 0;

    for (unsigned int i = 1; i < aCLP.GetArgumentCount(); i++)
    {
        char lScript[2048];

        aCLP.GetData(i, lScript, sizeof(lScript));

        char lCmd[2048];

        sprintf_s(lCmd, "%s %s %s", lScript, aVT.GetText3(), aVT.mType);

        int lRet = system(lCmd);
        if (0 != lRet)
        {
            fprintf(stderr, "\nERROR : %s  reported an error - %d\n\n", lCmd, lRet);
            lResult = lRet;
        }
    }

    return lResult;
}

// Exception KmsLib::Exception *  See Slave_Modify
void Slave_Modify(const KmsLib::CmdLineParser & aCLP, const Version & aVersion)
{
    assert(NULL != &aCLP    );
    assert(NULL != &aVersion);

    for (unsigned int i = 1; i < aCLP.GetArgumentCount(); i++)
    {
        char lSlave[2048];

        aCLP.GetData(i, lSlave, sizeof(lSlave));

        Slave_Modify(lSlave, aVersion);
    }
}

// Exception KmsLib::Exception *  CODE_USER_ERROR
void Slave_Modify(const char * aSlave, const Version & aVersion)
{
    assert(NULL !=  aSlave  );
    assert(NULL != &aVersion);

    char lMsg[2048];

    const char * lExt = strrchr(aSlave, '.');
    if (NULL == lExt)
    {
        sprintf_s(lMsg, "%s is not a valid slave file name", aSlave);
        throw new KmsLib::Exception(KmsLib::Exception::CODE_USER_ERROR, "Invalid slave file name", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
    }

    if      (0 == _strnicmp(".bat" , lExt, 5)) { Slave_Modify_CMD(aSlave, aVersion); }
    else if (0 == _strnicmp(".cmd" , lExt, 5)) { Slave_Modify_CMD(aSlave, aVersion); }
    else if (0 == _strnicmp(".conf", lExt, 6)) { Slave_Modify_TXT(aSlave, aVersion); }
    else if (0 == _strnicmp(".sh"  , lExt, 4)) { Slave_Modify_TXT(aSlave, aVersion); }
    else if (0 == _strnicmp(".txt" , lExt, 5)) { Slave_Modify_TXT(aSlave, aVersion); }
    else if (0 == _strnicmp(".wxs" , lExt, 5)) { Slave_Modify_XML(aSlave, aVersion); }
    else if (0 == _strnicmp(".xml" , lExt, 5)) { Slave_Modify_XML(aSlave, aVersion); }
    #ifdef _KMS_WINDOWS_
        else if (0 == _strnicmp(".inf", lExt, 5)) { Slave_Modify_INF(aSlave, aVersion); }
        else if (0 == _strnicmp(".inx", lExt, 5)) { Slave_Modify_INF(aSlave, aVersion); }
        else if (0 == _strnicmp(".iss", lExt, 5)) { Slave_Modify_ISS(aSlave, aVersion); }
    #endif
    else
    {
        sprintf_s(lMsg, "%s is not a supported slave file name", aSlave);
        throw new KmsLib::Exception(KmsLib::Exception::CODE_USER_ERROR, "Unsupported slave file name", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
    }
}

// REQUIREMENT KmsVersion.Slave.CMD
//             KmsVersion process .cmd or .bat slave files.

// Exception KmsLib::Exception *  CODE_UNKNOWN
void  Slave_Modify_CMD(const char * aSlave, const Version & aVersion)
{
    assert(NULL !=  aSlave  );
    assert(NULL != &aVersion);

    FILE * lRead ;
    FILE * lWrite;

    File_Open(aSlave, &lRead, &lWrite);

    try
    {
        unsigned int lCount = 0;
        char         lLine[1024];
        char         lMsg [2048];

        while (NULL != fgets(lLine, sizeof(lLine), lRead))
        {
            char         lAfter [1024];
            char         lBefore[1024];
            unsigned int lN;

            if (0 == _strnicmp("rem", lLine, 3))
            {
                int lRet = sscanf_s(lLine + 3, " KmsVersion \"%[^\"]\" \"%[^\"]\" %u", lBefore SIZE_INFO(sizeof(lBefore)), lAfter SIZE_INFO(sizeof(lAfter)), &lN);
                switch (lRet)
                {
                case 0:
                    fprintf(lWrite, "%s", lLine);
                    break;

                case 3 :
                    fprintf(lWrite, "%s", lLine);
                    Command_Execute(lRead, lWrite, aVersion, lN, lBefore, lAfter);
                    lCount++;
                    break;

                default :
                    sprintf_s(lMsg, "The KmsVersion instruction \"%s\" is not valid", lLine);
                    throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The KmsVersion instruction is not valid", lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
                }
            }
            else
            {
                fprintf(lWrite, "%s", lLine);
            }
        }

        if (0 >= lCount)
        {
            sprintf_s(lMsg, "The %s file does not contain KmsVersion instruction", aSlave);
            throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The file does not contain KmsVersion instruction", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
        }
    }
    catch (...)
    {
        File_Close (lRead, lWrite);
        File_Revert(aSlave);
        throw;
    }

    File_Close(lRead, lWrite);
}

#ifdef _KMS_WINDOWS_

// REQUIREMENT KmsVersion.Slave.INF
//              KmsVersion process .inf or .inx slave files.

// Exception KmsLib::Exception *  CODE_UNKNOWN
void Slave_Modify_INF(const char * aSlave, const Version & aVersion)
{
    assert(NULL !=  aSlave  );
    assert(NULL != &aVersion);

    FILE * lRead ;
    FILE * lWrite;

    File_Open(aSlave, &lRead, &lWrite);

    try
    {
        unsigned int lCount = 0;
        char         lLine[1024];
        char         lMsg [2048];

        SYSTEMTIME lTime;

        GetSystemTime(&lTime);

        while (NULL != fgets(lLine, sizeof(lLine), lRead))
        {
            // LIMITATION KmsVersion.NoComment
            //            If comment follow the DriverVer directive in a .inf or .inx file, this comment is dropped.
            if (0 == strncmp("DriverVer", lLine, 9))
            {
                fprintf(lWrite, "DriverVer   = %02u/%02u/%04u,%s\n", lTime.wMonth, lTime.wDay, lTime.wYear, aVersion.GetText4('.'));
                lCount++;
            }
            else
            {
                fprintf(lWrite, "%s", lLine);
            }
        }

        if (1 != lCount)
        {
            sprintf_s(lMsg, "The %s file is not a valid INF file", aSlave);
            throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The file is not a valid INF file", lMsg, __FILE__, __FUNCTION__, __LINE__, lCount);
        }
    }
    catch (...)
    {
        File_Close (lRead, lWrite);
        File_Revert(aSlave);
        throw;
    }

    File_Close(lRead, lWrite);
}

// REQUIREMENT KmsVersion.Slave.ISS
//             KmsVersion processes .iss slave files.

// Exception KmsLib::Exception *  CODE_UNKNOWN
void Slave_Modify_ISS(const char * aSlave, const Version & aVersion)
{
    assert(NULL !=  aSlave  );
    assert(NULL != &aVersion);

    FILE * lRead ;
    FILE * lWrite;

    File_Open(aSlave, &lRead, &lWrite);

    try
    {
        unsigned int lCount =  0;
        char         lLine[1024];
        char         lMsg [2048];

        SYSTEMTIME lTime;

        GetSystemTime(&lTime);

        while (NULL != fgets(lLine, sizeof(lLine), lRead))
        {
            if (0 == strncmp("AppVersion", lLine, 10))
            {
                fprintf(lWrite, "AppVersion=%s\n", aVersion.GetText3());
                lCount++;
            }
            else if (0 == strncmp("OutputBaseFilename", lLine, 18))
            {
                String_Trunk(lLine, '_');
                fprintf(lWrite, "%s_%s\n", lLine, aVersion.GetText3());
                lCount++;
            }
            else
            {
                fprintf(lWrite, "%s", lLine);
            }
        }

        if (2 != lCount)
        {
            sprintf_s(lMsg, "The %s file is not a valid ISS file", aSlave);
            throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The file is not a valid ISS file", lMsg, __FILE__, __FUNCTION__, __LINE__, lCount);
        }
    }
    catch (...)
    {
        File_Close (lRead, lWrite);
        File_Revert(aSlave);
        throw;
    }

    File_Close(lRead, lWrite);
}

#endif

// Exception KmsLib::Exception *  See Slave_Modify_TXT
void Slave_Modify_TXT(const KmsLib::CmdLineParser & aCLP, const Version & aVersion)
{
    assert(NULL != &aCLP    );
    assert(NULL != &aVersion);

    for (unsigned int i = 1; i < aCLP.GetArgumentCount(); i++)
    {
        char lSlave[2048];

        aCLP.GetData(i, lSlave, sizeof(lSlave));

        Slave_Modify_TXT(lSlave, aVersion);
    }
}

// REQUIREMENT KmsVersion.Slave.TXT
//             KmsVersion processes .txt slave files.

// Exception KmsLib::Exception *  CODE_UNKNOWN
void Slave_Modify_TXT(const char * aSlave, const Version & aVersion)
{
    assert(NULL !=  aSlave  );
    assert(NULL != &aVersion);

    FILE * lRead ;
    FILE * lWrite;

    File_Open(aSlave, &lRead, &lWrite);

    try
    {
        unsigned int lCount  = 0;
        char         lLine[1024];
        char         lMsg [2048];

        while (NULL != fgets(lLine, sizeof(lLine), lRead))
        {
            char         lAfter [1024];
            char         lBefore[1024];
            unsigned int lN;

            if (0 == _strnicmp("#", lLine, 1))
            {
                int lRet = sscanf_s(lLine + 1, " KmsVersion \"%[^\"]\" \"%[^\"]\" %u", lBefore SIZE_INFO(sizeof(lBefore)), lAfter SIZE_INFO(sizeof(lAfter)), &lN);
                switch (lRet)
                {
                case 0:
                    fprintf(lWrite, "%s", lLine);
                    break;

                case 3:
                    fprintf(lWrite, "%s", lLine);
                    String_Escape(lAfter);
                    Command_Execute(lRead, lWrite, aVersion, lN, lBefore, lAfter);
                    lCount++;
                    break;

                default:
                    sprintf_s(lMsg, "The KmsVersion instruction \"%s\" is not valid", lLine);
                    throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The KmsVersion instruction is not valid", lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
                }
            }
            else
            {
                fprintf(lWrite, "%s", lLine);
            }
        }

        if (0 >= lCount)
        {
            sprintf_s(lMsg, "The %s file does not contain KmsVersion instruction", aSlave);
            throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The file does not contain KmsVersion instruction", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
        }
    }
    catch (...)
    {
        File_Close (lRead, lWrite);
        File_Revert(aSlave);
        throw;
    }

    File_Close(lRead, lWrite);
}

// REQUIREMENT KmsVersion.Slave.XML
//             KmsVersion processes .xml or .wxs slave files.

// Exception KmsLib::Exception *  See File_Open
void Slave_Modify_XML(const char * aSlave, const Version & aVersion)
{
    assert(NULL !=  aSlave  );
    assert(NULL != &aVersion);

    FILE * lRead ;
    FILE * lWrite;

    File_Open(aSlave, &lRead, &lWrite);

    try
    {
        unsigned int  lCount = 0;
        char          lLine[1024];
        char          lMsg [2048];

        while (NULL != fgets(lLine, sizeof(lLine), lRead))
        {
            char         lAfter [1024];
            char         lBefore[1024];
            unsigned int lN;

            int lRet = sscanf_s(lLine, " <!-- KmsVersion (%[^)]) (%[^)]) %u -->", lBefore SIZE_INFO(sizeof(lBefore)), lAfter SIZE_INFO(sizeof(lAfter)), &lN);
            switch (lRet)
            {
            case EOF :
            case 0   :
                fprintf(lWrite, "%s", lLine);
                break;

            case 3:
                fprintf(lWrite, "%s", lLine);
                Command_Execute(lRead, lWrite, aVersion, lN, lBefore, lAfter);
                lCount++;
                break;

            default:
                sprintf_s(lMsg, "The KmsVersion instruction \"%s\" is not valid", lLine);
                throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The KmsVersion instruction is not valid", lMsg, __FILE__, __FUNCTION__, __LINE__, lRet);
            }
        }

        if (0 >= lCount)
        {
            sprintf_s(lMsg, "The %s file does not contain KmsVersion instruction", aSlave);
            throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "The file does not contain KmsVersion instruction", lMsg, __FILE__, __FUNCTION__, __LINE__, 0);
        }
    }
    catch (...)
    {
        File_Close (lRead, lWrite);
        File_Revert(aSlave);
        throw;
    }

    File_Close(lRead, lWrite);
}

void String_Escape(char * aInOut)
{
    assert(NULL != aInOut);

    const char * lIn = aInOut;

    char * lOut = aInOut;

    for (;;)
    {
        switch (*lIn)
        {
        case '\0': *lOut = *lIn; return;

        case '\\':
            (void)*lIn ++;
            switch (*lIn)
            {
            case 'n' : *lOut = '\n'; break;
            default: *lOut = * lIn;
            }
            break;

        default: *lOut = *lIn;
        }

        lIn++;
        lOut++;
    }
}

// Exception KmsLib::Exception *  CODE_INVALID_DATA
void String_Trunk(char * aStr, char aEnd)
{
    assert(NULL != aStr);
    assert('\0' != aEnd);

    char * lPtr = strchr(aStr, aEnd);
    if (NULL == lPtr)
    {
        char lMsg[2048];

        sprintf_s(lMsg, "Invalid string end - \"%s\"", aStr);

        throw new KmsLib::Exception(KmsLib::Exception::CODE_INVALID_DATA, "Invalid string end", lMsg, __FILE__, __FUNCTION__, __LINE__, aEnd);
    }

    (*lPtr) = '\0';
}
