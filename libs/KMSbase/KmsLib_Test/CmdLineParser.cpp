
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib_Test/CmdLineParser.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Interface ==========================================================
#include <KmsLib/CmdLineParser.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(CmdLineParser_Base)
{
    FILE		  * lFile;
    char			lStr[64];
    unsigned int	lUInt;


    KmsLib::CmdLineParser lCLP0("");

    KMS_TEST_ASSERT(!lCLP0.IsPresent(0));

    KMS_TEST_COMPARE(0, lCLP0.GetArgumentCount());
    KMS_TEST_COMPARE(0, lCLP0.GetErrorFlags   ());

    lCLP0.GetData(0, lStr, sizeof(lStr), "Default");
    KMS_TEST_COMPARE(0, strcmp("Default", lStr));

    try
    {
        lCLP0.GetData(0, lStr, sizeof(lStr));
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_INVALID_COMMAND_LINE, eE->GetCode());
    }

    lCLP0.GetData(0, &lUInt, 10, 1);
    KMS_TEST_COMPARE(1, lUInt);

    try
    {
        lCLP0.GetData(0, &lUInt, 16);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_INVALID_COMMAND_LINE, eE->GetCode());
    }

    lCLP0.GetData(0, &lFile, "r", stdin);
    KMS_TEST_ASSERT(stdin == lFile);

    try
    {
        lCLP0.GetData(0, &lFile, "r");
        KMS_TEST_ERROR()
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_INVALID_COMMAND_LINE, eE->GetCode());
    }

    KmsLib::CmdLineParser lCLP1("a", "bc");

    const char * lVector[7];

    lVector[0] = "ExecName";
    lVector[1] = "-abc";
    lVector[2] = "KmsLib_Test/CmdLineParser.cpp";
    lVector[3] = "-";
    lVector[4] = "10";
    lVector[5] = "2";
    lVector[6] = "-dc";

    lCLP1.Parse(7, lVector);

    KMS_TEST_ASSERT(!lCLP1.IsPresent(0));
    KMS_TEST_ASSERT( lCLP1.IsPresent(1));
    KMS_TEST_ASSERT( lCLP1.IsPresent(2));
    KMS_TEST_ASSERT(!lCLP1.IsPresent(3));
    KMS_TEST_ASSERT( lCLP1.IsPresent('a'));
    KMS_TEST_ASSERT( lCLP1.IsPresent('b'));
    KMS_TEST_ASSERT(!lCLP1.IsPresent('c'));

    lCLP1.GetData(1, lStr, sizeof(lStr));
    KMS_TEST_COMPARE(0, strcmp("10", lStr));

    lCLP1.GetData(1, &lUInt, 10);
    KMS_TEST_COMPARE(10, lUInt);

    lCLP1.GetData(1, &lUInt, 16, 0x0);
    KMS_TEST_COMPARE(0x10, lUInt);

    try
    {
        lCLP1.GetData('b', &lUInt, 10);
        KMS_TEST_ERROR();
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_INVALID_COMMAND_LINE, eE->GetCode());
    }

    lCLP1.GetData('b', &lFile, "r");
    KMS_TEST_ASSERT(NULL != lFile);

    int lRet = fclose(lFile);
    KMS_TEST_COMPARE(0, lRet);

    try
    {
        lCLP1.GetData(1, &lFile, "r", stdin);
        KMS_TEST_ERROR()
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_FILE_OPEN_ERROR, eE->GetCode());
    }

    KMS_TEST_COMPARE((KmsLib::CmdLineParser::ERROR_FLAG_IGNORED_OPTIONS | KmsLib::CmdLineParser::ERROR_FLAG_INCOMPLET_OPTION | KmsLib::CmdLineParser::ERROR_FLAG_INVALID_OPTION), lCLP1.GetErrorFlags());
}
KMS_TEST_END_2
