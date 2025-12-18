
// Author   KMS - Martin Dubois, ing.
// Product  KmsBase
// File     KmsLib_Test/ToolBase.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <stdio.h>

// ===== Interface ==========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/ToolBase.h>
#include <KmsTest.h>

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

#define TEST_FOLDER "KmsLib_Test" SLASH "Tests"

// Tests
/////////////////////////////////////////////////////////////////////////////

static const char * ARGUMENTS_C0[] =
{
	"KmsLib_Test.exe",
	"Execute=Invalid",
};

static const char * ARGUMENTS_C1[] =
{
	"KmsLib_Test.exe"                   ,
	"Execute=ExecuteScript DoesNotExist",
};

static const char * ARGUMENTS_C2[] =
{
	"KmsLib_Test.exe",
	"Execute=A"		 ,
};

static const char * ARGUMENTS_C3[] =
{
	"KmsLib_Test.exe",
	"Execute=Help"   ,
};

static const char * ARGUMENTS_C5[] =
{
	"KmsLib_Test.exe"		                                  ,
	"Execute=ExecuteScript " TEST_FOLDER SLASH "ToolBase0.txt",
};

static const char * ARGUMENTS_C6[] =
{
    "KmsLib_Test.exe"		             ,
    "Execute=ChangeDir " TEST_FOLDER     ,
    "Execute=ExecuteScript ToolBase0.txt",
    "Execute=ChangeDir .." SLASH ".."    ,
};

static void A(KmsLib::ToolBase * aThis, const char * aArguments)
{
	assert(NULL != aThis		);
	assert(NULL != aArguments	);
}

static const KmsLib::ToolBase::CommandInfo COMMANDS_A[] =
{
	{ "A", A, "A                             A", NULL },

	{ NULL, NULL, NULL, NULL }
};

static const KmsLib::ToolBase::CommandInfo COMMANDS[] =
{
	{ "A"				, NULL								, "A {...}                       A"							, COMMANDS_A	},

    KMS_LIB_TOOL_BASE_FUNCTIONS

	{ NULL, NULL, NULL, NULL }
};

KMS_TEST_BEGIN(ToolBase_Base)
{
    FILE * lFile;

    errno_t lRetE = fopen_s(&lFile, TEST_FOLDER SLASH "ToolBase1.txt", "r");
    KMS_TEST_COMPARE(0, lRetE);

    // ===== AskUser - unsigned short =======================================

    unsigned short lValue;

    KmsLib::ToolBase::AskUser(lFile, "Name", 10, 99, 54, &lValue);
    KMS_TEST_COMPARE(54, lValue);

    KmsLib::ToolBase::AskUser(lFile, "Name", 10, 99, 54, &lValue);
    KMS_TEST_COMPARE(60, lValue);

    // ===== AskUser - char * ===============================================

    char lString[32];

    KmsLib::ToolBase::AskUser(lFile, "Name", lString, sizeof(lString));
    KMS_TEST_COMPARE(0, strcmp("Test", lString));

    KmsLib::ToolBase::AskUser(lFile, "Name", lString, sizeof(lString));
    KMS_TEST_COMPARE(0, strcmp("", lString));

    // ===== AskUser - char * - With default ================================

    KmsLib::ToolBase::AskUser(lFile, "Name", "Default", lString, sizeof(lString));
    KMS_TEST_COMPARE(0, strcmp("Test", lString));

    KmsLib::ToolBase::AskUser(lFile, "Name", "Default", lString, sizeof(lString));
    KMS_TEST_COMPARE(0, strcmp("Default", lString));

    // ===== AskUser_InputFileName ==========================================

    // TODO  KmsLib.ToolBase  Test AskUser_InputFileName

    // ===== AskUser_OutputFileName =========================================

    // TODO  KmsLib.ToolBase  Test AskUser_OutputFileName

    int lRetI = fclose(lFile);
    KMS_TEST_COMPARE(0, lRetI);

    // ===== Report =========================================================
    KmsLib::ToolBase::Report(KmsLib::ToolBase::REPORT_WARNING);

    // ===== Report - Exception =============================================

    KmsLib::Exception  lE(KmsLib::Exception::CODE_UNKNOWN, "Test", NULL, __FILE__, __FUNCTION__, __LINE__, 0);

    KmsLib::ToolBase::Report(KmsLib::ToolBase::REPORT_ERROR, &lE);

    // ===== Report - const  char  * ========================================
    KmsLib::ToolBase::Report(KmsLib::ToolBase::REPORT_OK, "OK");

    KmsLib::ToolBase lTB0(COMMANDS);

    // ===== Abort ==========================================================
    KmsLib::ToolBase::Abort(&lTB0, "Invalid");
    KMS_TEST_COMPARE(-3, lTB0.GetErrorCode());

    // ===== ChangeDir ======================================================
    lTB0.ClearError();
    KmsLib::ToolBase::ChangeDir(&lTB0, "");
    KMS_TEST_COMPARE(-592, lTB0.GetErrorCode());

    // ===== Delay ==========================================================

    KmsLib::ToolBase::Delay(&lTB0, "");

    lTB0.ClearError();
    KmsLib::ToolBase::Delay(&lTB0, "Invalid");
    KMS_TEST_COMPARE(-4, lTB0.GetErrorCode());

    // ===== Echo ===========================================================
    KmsLib::ToolBase::Echo(&lTB0, "");

    // ===== Error_Abort ====================================================

    lTB0.ClearError();
    KmsLib::ToolBase::Error_Abort(&lTB0, "");

    // ===== Error_Clear ====================================================
    KmsLib::ToolBase::Error_Clear(&lTB0, "");

    // ===== Error_Display ==================================================

    KmsLib::ToolBase::Error_Display(&lTB0, "");

    lTB0.SetError(1, "Test", KmsLib::ToolBase::REPORT_FATAL_ERROR);
    KmsLib::ToolBase::Error_Display(&lTB0, "");

    // ===== Error_Exit =====================================================

    KmsLib::ToolBase::Error_Exit(&lTB0, "");

    lTB0.ClearError();
    KmsLib::ToolBase::Error_Exit(&lTB0, "");

    // ===== ExecuteScript ==================================================
    KmsLib::ToolBase::ExecuteScript(&lTB0, "DoesNotExist.txt");
    KMS_TEST_COMPARE(-2, lTB0.GetErrorCode());

    // ===== Exit ===========================================================

    KmsLib::ToolBase::Exit(&lTB0, "" );
    KmsLib::ToolBase::Exit(&lTB0, "0");
    KmsLib::ToolBase::Exit(&lTB0, "1");

    lTB0.ClearError();
    KmsLib::ToolBase::Exit(&lTB0, "Invalid");
    KMS_TEST_COMPARE(-7, lTB0.GetErrorCode());

    // ===== Pause ==========================================================
    // Require interaction

    // ===== Repeat =========================================================

    // We use a new ToolBase instance, because previous test set mExit in TB0
    KmsLib::ToolBase lTB1(COMMANDS);

    KmsLib::ToolBase::Repeat(&lTB1, "2 Echo Repeat");
    KmsLib::ToolBase::Repeat(&lTB1, "Invalid");
    KMS_TEST_COMPARE(-8, lTB1.GetErrorCode());

    // ===== ToolBase =======================================================
    // Already tested

    // ===== ClearError =====================================================
    lTB0.ClearError();

    // ===== GetErrorCode ===================================================
    KMS_TEST_COMPARE(0, lTB0.GetErrorCode());

    // ===== SetError =======================================================
    lTB0.SetError(0, "", KmsLib::ToolBase::REPORT_OK);
    KMS_TEST_COMPARE(0, lTB0.GetErrorCode());
    lTB0.SetError(1, "Test");
    KMS_TEST_COMPARE(1, lTB0.GetErrorCode());
    lTB0.SetError(0, "", KmsLib::ToolBase::REPORT_OK);
    KMS_TEST_COMPARE(1, lTB0.GetErrorCode());
    lTB0.SetError(2, "Test", KmsLib::ToolBase::REPORT_ERROR);
    KMS_TEST_COMPARE(1, lTB0.GetErrorCode());

    // ===== Parse ==========================================================

    const char * lArg;
    bool lBool;

    lArg = "Invalid";
    KMS_TEST_ASSERT(!lTB0.Parse(&lArg, &lBool));
    lArg = "";
    KMS_TEST_ASSERT(!lTB0.Parse(&lArg, &lBool));
    lArg = "false";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool));
    KMS_TEST_ASSERT(!lBool);
    lArg = "0";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool));
    KMS_TEST_ASSERT(!lBool);
    lArg = "true";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool));
    KMS_TEST_ASSERT(lBool);
    lArg = "1";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool));
    KMS_TEST_ASSERT(lBool);
    lArg = " false other";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool));
    KMS_TEST_ASSERT(0 == strcmp(" other", lArg));
    KMS_TEST_ASSERT(!lBool);
    lArg = " 0 other";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool));
    KMS_TEST_ASSERT(0 == strcmp(" other", lArg));
    KMS_TEST_ASSERT(!lBool);
    lArg = " true other";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool));
    KMS_TEST_ASSERT(0 == strcmp(" other", lArg));
    KMS_TEST_ASSERT(lBool);
    lArg = " 1 other";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool));
    KMS_TEST_ASSERT(0 == strcmp(" other", lArg));
    KMS_TEST_ASSERT(lBool);

    lArg = "Invalid";
    KMS_TEST_ASSERT(!lTB0.Parse(&lArg, &lBool, false));
    lArg = "";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool, false));
    KMS_TEST_ASSERT(!lBool);
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool, true));
    KMS_TEST_ASSERT(lBool);
    lArg = " false other";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lBool, false));
    KMS_TEST_ASSERT(0 == strcmp(" other", lArg));
    KMS_TEST_ASSERT(!lBool);

    unsigned int lUnsigned;

    lArg = "Invalid";
    KMS_TEST_ASSERT(!lTB0.Parse(&lArg, &lUnsigned));
    lArg = "0 other";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lUnsigned));
    KMS_TEST_ASSERT(0 == strcmp(" other", lArg));
    KMS_TEST_COMPARE(0, lUnsigned);
    lArg = " 1";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lUnsigned));
    KMS_TEST_COMPARE(1, lUnsigned);
    lArg = "a";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lUnsigned, 0, 15, true));
    KMS_TEST_COMPARE(0xa, lUnsigned);
    lArg = "10";
    KMS_TEST_ASSERT(!lTB0.Parse(&lArg, &lUnsigned, 0, 15, true));
    lArg = "10";
    KMS_TEST_ASSERT(!lTB0.Parse(&lArg, &lUnsigned, 32, 64, true));

    lArg = "Invalid";
    KMS_TEST_ASSERT(!lTB0.Parse(&lArg, &lUnsigned, 0, 0xffffffff, false, 32));
    lArg = "";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lUnsigned, 0, 0xffffffff, false, 32));
    KMS_TEST_COMPARE(32, lUnsigned);
    lArg = "64";
    KMS_TEST_ASSERT(lTB0.Parse(&lArg, &lUnsigned, 0, 0xffffffff, false, 32));
    KMS_TEST_COMPARE(64, lUnsigned);

    // ===== ParseArguments =================================================

    lTB0.ClearError();
    lTB0.ParseArguments(1, ARGUMENTS_C0);
    KMS_TEST_COMPARE(0, lTB0.GetErrorCode());

    lTB0.ClearError();
    lTB0.ParseArguments(2, ARGUMENTS_C0);
    KMS_TEST_COMPARE(-5, lTB0.GetErrorCode());

    lTB0.ClearError();
    lTB0.ParseArguments(2, ARGUMENTS_C1);
    KMS_TEST_COMPARE(-2, lTB0.GetErrorCode());

    lTB0.ClearError();
    lTB0.ParseArguments(2, ARGUMENTS_C2);
    KMS_TEST_COMPARE(-6, lTB0.GetErrorCode());

    lTB0.ClearError();
    lTB0.ParseArguments(2, ARGUMENTS_C3);
    KMS_TEST_COMPARE(0, lTB0.GetErrorCode());

    lTB0.ClearError();
    lTB0.ParseArguments(2, ARGUMENTS_C5);
    KMS_TEST_COMPARE(0, lTB0.GetErrorCode());

    lTB0.ClearError();
    lTB0.ParseArguments(4, ARGUMENTS_C6);
    KMS_TEST_COMPARE(0, lTB0.GetErrorCode());

    // ===== ParseCommands ==================================================
    // Require interaction

    // ===== ParseCommands - With file name =================================
    lTB0.ClearError();
    lTB0.ParseCommands("");
    KMS_TEST_COMPARE(-1, lTB0.GetErrorCode());
}
KMS_TEST_END_2
