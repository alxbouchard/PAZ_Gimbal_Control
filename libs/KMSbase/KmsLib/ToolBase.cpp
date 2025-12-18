
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020-2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib/ToolBase.cpp

// CODE REVIEW 2020-08-267 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-08-26 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <stdio.h>
#include <string.h>

#ifdef _KMS_WINDOWS_

	#include <io.h>

    // ===== Windows ========================================================
    #include <windows.h>

#endif // _KMS_WINDOWS_

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>
#include <KmsLib/File.h>
#include <KmsLib/ThreadBase.h>

#include <KmsLib/ToolBase.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#define LINE_LENGTH_MAX	(1024)

// Macro
/////////////////////////////////////////////////////////////////////////////

#define  BLUE    (0x09)
#define  GREEN   (0x0a)
#define  RED     (0x0c)
#define  WHITE   (0x07)
#define  YELLOW  (0x0e)

#ifdef _KMS_WINDOWS_
    #define  COLOR(A)  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (A))
#else
    #define  COLOR(A)
#endif

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void  DisplayHelp(const KmsLib::ToolBase::CommandInfo * aCommandInfo);

static void GoOverBlank(const char ** aArg);

static void  ReadLine(FILE * aFile, char * aOut, unsigned int aOutsize_byte);

static int ScanUnsigned(const char ** aArg, unsigned int * aOut, bool aHex);

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////

    // NOT TESTED  KmsLib.ToolBase.Console
    //             Ask a question on the stdin

    // aOut  [---;-W-]
	void ToolBase::AskUser(FILE * aFile, const char * aName, unsigned short aMin, unsigned short aMax, unsigned short aDefault, unsigned short * aOut)
	{
		assert(NULL != aFile   );
		assert(NULL != aName   );
		assert(aMin <  aMax    );
		assert(aMin <= aDefault);
		assert(aMax >= aDefault);
		assert(NULL != aOut    );

		for (;;)
		{
			if (stdin == aFile)
			{
				printf("%s\t(Range: %u to %u;\tDefault: %u) : ", aName, aMin, aMax, aDefault);
			}

			char lLine[LINE_LENGTH_MAX];

			ReadLine(aFile, lLine, sizeof(lLine));

			unsigned int lValue;

			int lRetI = sscanf_s(lLine, "%u", &lValue);
			switch ( lRetI )
			{
			case EOF:
				(*aOut) = aDefault;
				return;

			case 1 :
				if ((aMin <= lValue) && (aMax >= lValue))
				{
					(*aOut) = lValue;
					return;
				}
				// No break;

			default:
				Report(REPORT_USER_ERROR, "Invalid value, try again!");
				break;
			}
		}
	}

    // aOut  [---;-W-]
	void ToolBase::AskUser(FILE * aFile, const char * aName, char * aOut, unsigned int aOutSize_byte)
	{
		assert(NULL != aFile        );
		assert(NULL != aName        );
		assert(NULL != aOut         );
		assert(0	<  aOutSize_byte);

		for (;;)
		{
			if (stdin == aFile)
			{
				printf("%s : ", aName);
			}

			char lLine[LINE_LENGTH_MAX];

			ReadLine(aFile, lLine, sizeof(lLine));

			char lValue[2048];

			memset(&lValue, 0, sizeof(lValue));

			int lRetI = sscanf_s(lLine, "%[^\n\r\t]", lValue SIZE_INFO(sizeof(lValue) - 1));
			switch (lRetI)
			{
			case 1:
				size_t lLen;

				lLen = strlen(lValue);
				if (aOutSize_byte <= lLen)
				{
					Report(REPORT_USER_ERROR, "Too long, try again!");
					break;
				}

				// No break;

			default :
				strcpy_s(aOut SIZE_INFO( aOutSize_byte ), lValue);
				return;
			}
		}
	}

    // aOut     [---;-W-]
	void ToolBase::AskUser(FILE * aFile, const char * aName, const char * aDefault, char * aOut, unsigned int aOutSize_byte)
	{
		assert(NULL != aFile        );
		assert(NULL != aName        );
		assert(NULL != aDefault     );
		assert(NULL != aOut         );
		assert(0	<  aOutSize_byte);

		for (;;)
		{
			if (stdin == aFile)
			{
                printf("%s\t(Default: \"%s\") : ", aName, aDefault);
			}

			char lLine[LINE_LENGTH_MAX];

			ReadLine(aFile, lLine, sizeof(lLine));

			char lValue[2048];

			memset(&lValue, 0, sizeof(lValue));

			int lRetI = sscanf_s(lLine, "%[^\n\r\t]", lValue SIZE_INFO(sizeof(lValue) - 1));
			switch (lRetI)
			{
			case 1:
				size_t lLen;

				lLen = strlen(lValue);
				if (aOutSize_byte <= lLen)
				{
					Report(REPORT_USER_ERROR, "Too long, try again!");
					break;
				}

				strcpy_s(aOut SIZE_INFO( aOutSize_byte ), lValue);
				return;

			default:
				strcpy_s(aOut SIZE_INFO( aOutSize_byte ), aDefault);
				return;
			}
		}
	}

    // NOT TESTED  KmsLib.ToolBase.AskUser_InputFileName
    //             Asking a input file name to the user

    // aOut  [---;-W-]
	void ToolBase::AskUser_InputFileName(FILE * aFile, const char * aName, char * aOut, unsigned int aOutSize_byte)
	{
		assert(NULL != aFile        );
		assert(NULL != aName        );
		assert(NULL != aOut         );
		assert(0	<  aOutSize_byte);

		for (;;)
		{
			AskUser(aFile, aName, aOut, aOutSize_byte);

			if (KmsLib::File::Exist(aOut, FILE_EXIST_FLAG_READ))
			{
				break;
			}

			Report(REPORT_USER_ERROR, "The file does not exist, try again!");
		}
	}

    // NOT TESTED  KmsLib.ToolBase.AskUser_OutputFileName
    //             Asking a output file name to the user

    // aOut     [---;-W-]
    void ToolBase::AskUser_OutputFileName(FILE * aFile, const char * aName, const char * aDefault, char * aOut, unsigned int aOutSize_byte)
	{
		assert(NULL != aFile        );
		assert(NULL != aName        );
		assert(NULL != aDefault     );
		assert(NULL != aOut         );
		assert(0	<  aOutSize_byte);

		for (;;)
		{
			AskUser(aFile, aName, aDefault, aOut, aOutSize_byte);

			if (!KmsLib::File::Exist(aOut))
			{
				break;
			}

			Report(REPORT_USER_ERROR, "The file already exists, try again!");
		}
	}

	void ToolBase::Report(ReportType aType)
	{
        unsigned short lColor;
        const char   * lTypeName = NULL; // Avoid warning

		switch (aType)
		{
		case REPORT_ERROR		: lColor = RED   ; lTypeName = "ERROR"		; break;
		case REPORT_FATAL_ERROR	: lColor = RED   ; lTypeName = "FATAL ERROR"; break;
        case REPORT_INFO		: lColor = BLUE  ; lTypeName = "INFO"		; break;
		case REPORT_OK			: lColor = GREEN ; lTypeName = "OK"			; break;
		case REPORT_USER_ERROR	: lColor = RED   ; lTypeName = "USER ERROR"	; break;
		case REPORT_WARNING		: lColor = YELLOW; lTypeName = "WARNING"	; break;

		default: assert(false);
		}

        COLOR(lColor);
		printf("\n%s : ", lTypeName);
        COLOR(WHITE);
	}

	void ToolBase::Report(ReportType aType, const KmsLib::Exception * aException)
	{
		assert(NULL != aException);

		Report(aType);

		printf("Exception\n");

		aException->Write(stdout);
	}

	void ToolBase::Report(ReportType aType, const char * aMessage)
	{
		assert(NULL != aMessage);

		Report(aType);

		printf("%s\n", aMessage);
	}

    // ===== Function =======================================================

    const ToolBase::CommandInfo ToolBase::ERROR_COMMANDS[] =
    {
        { "Abort"  , ToolBase::Error_Abort  , "Abort"  , NULL },
        { "Clear"  , ToolBase::Error_Clear  , "Clear"  , NULL },
        { "Display", ToolBase::Error_Display, "Display", NULL },
        { "Exit"   , ToolBase::Error_Exit   , "Exit"   , NULL },

        { NULL, NULL, NULL, NULL }
    };

    // NOT TESTED  KmsLib::ToolBase.Abort
    //             Aborting execution

    void ToolBase::Abort(ToolBase * aThis, const char * aArgs)
    {
        assert(NULL != aThis);
        assert(NULL != aArgs);

        aThis->Abort(aArgs);
    }

    void ToolBase::ChangeDir(ToolBase * aThis, const char * aArgs)
    {
        assert(NULL != aThis);
        assert(NULL != aArgs);

        aThis->ChangeDir(aArgs);
    }

    void ToolBase::Delay(ToolBase * aThis, const char * aArgs)
    {
        assert(NULL != aThis);
        assert(NULL != aArgs);

        aThis->Delay(aArgs);
    }

    void ToolBase::Echo(ToolBase * aThis, const char * aArgs)
    {
        assert(NULL != aArgs);

        printf("%s\n", aArgs);
    }

    // aArg  [---;---]
    void ToolBase::Error_Abort(ToolBase * aThis, const char * aArgs)
    {
        assert(NULL != aThis);

        aThis->Error_Abort();
    }

    // aArg  [---;---]
    void ToolBase::Error_Clear(ToolBase * aThis, const char * aArg)
    {
        assert(NULL != aThis);

        aThis->ClearError();
    }

    // aArg  [---;---]
    void ToolBase::Error_Display(ToolBase * aThis, const char * aArg)
    {
        assert(NULL != aThis);

        aThis->Error_Display();
    }

    // aArg  [---;---] 
    void ToolBase::Error_Exit(ToolBase * aThis, const char * aArg)
    {
        assert(NULL != aThis);

        aThis->Error_Exit();
    }

    void ToolBase::ExecuteScript(ToolBase * aThis, const char * aArgs)
    {
        assert(NULL != aThis);
        assert(NULL != aArgs);

        aThis->ParseCommands(aArgs);
    }

    void ToolBase::Exit(ToolBase * aThis, const char * aArgs)
    {
        assert(NULL != aThis);
        assert(NULL != aArgs);

        aThis->Exit(aArgs);
    }

    // NOT TESTED  KmsLib::ToolBase.Pause
    //             Pause command

    void ToolBase::Pause(ToolBase * aThis, const char * aArg)
    {
        assert(NULL != aThis);
        assert(NULL != aArg );

        printf("%s\n", aArg);
        printf("Press ENTER to continue\n");

        char lLine[2048];

        ReadLine(stdin, lLine, sizeof(lLine));
    }

    void ToolBase::Repeat(ToolBase * aThis, const char * aArgs)
    {
        assert(NULL != aThis);
        assert(NULL != aArgs);

        aThis->Repeat(aArgs);
    }

    // aCommands [-K-;R--]
    ToolBase::ToolBase(const CommandInfo * aCommands) : mCommands(aCommands), mError_Code(0), mExit(false)
	{
		assert(NULL != mCommands);
	}

    void ToolBase::ClearError()
    {
        mError_Code = 0;
    }

    int ToolBase::GetErrorCode()
    {
        return mError_Code;
    }

    int ToolBase::SetError(int aCode, const char * aDesc, ReportType aType)
    {
        assert(NULL       != aDesc);
        assert(REPORT_QTY >  aType);

        if (0 != aCode)
        {
            Report(aType);
            printf("%d - %s\n", aCode, aDesc);

            if (0 == mError_Code)
            {
                mError_Code = aCode;
                mError_Desc = aDesc;
            }
        }

        return mError_Code;
    }

    bool ToolBase::Parse(const char ** aArg, bool * aOut)
    {
        char lWord[8];

        bool lResult = Parse(aArg, lWord, sizeof(lWord));
        if (lResult)
        {
            lResult = TextToBool(lWord, aOut);
        }

        return lResult;
    }

    bool ToolBase::Parse(const char ** aArg, bool * aOut, bool aDefault)
    {
        char lWord[8];

        bool lResult = Parse(aArg, lWord, sizeof(lWord), aDefault ? "true" : "false");
        if (lResult)
        {
            lResult = TextToBool(lWord, aOut);
        }

        return lResult;
    }

    bool ToolBase::Parse(const char ** aArg, unsigned int * aOut, unsigned int aMin, unsigned int aMax, bool aHex)
    {
        assert(NULL != aArg);
        assert(NULL != aOut);
        assert(aMin <= aMax);

        GoOverBlank(aArg);

        int lRet = ScanUnsigned(aArg, aOut, aHex);
        switch (lRet)
        {
        case 1: return Verify(*aOut, aMin, aMax);

        default: SetError(__LINE__, "Invalid unsigned value"); return false;
        }

        return true;
    }

    bool ToolBase::Parse(const char ** aArg, unsigned int * aOut, unsigned int aMin, unsigned int aMax, bool aHex, unsigned int aDefault)
    {
        assert(NULL != aArg);
        assert(NULL != aOut);
        assert(aMin <= aMax);

        GoOverBlank(aArg);

        int lRet = ScanUnsigned(aArg, aOut, aHex);
        switch (lRet)
        {
        case EOF: *aOut = aDefault; break;

        case 1: return Verify(*aOut, aMin, aMax);

        default: SetError(__LINE__, "Invalid unsigned value"); return false;
        }

        return true;
    }

    bool ToolBase::Parse(const char ** aArg, char * aOut, unsigned int aOutSize_byte, const char * aDefault)
    {
        assert(NULL != aArg);
        assert(NULL != aOut);
        assert(1 < aOutSize_byte);

        GoOverBlank(aArg);

        switch (sscanf_s(*aArg, "%s", aOut SIZE_INFO(aOutSize_byte)))
        {
        case EOF:
            if (NULL == aDefault)
            {
                SetError(__LINE__, "Invalid text value");
                return false;
            }
            strcpy_s(aOut SIZE_INFO(aOutSize_byte), aDefault);
            break;

        case 1: (*aArg) += strlen(aOut); break;

        default: SetError(__LINE__, "Invalid text value"); return false;
        }

        return true;
    }

    void ToolBase::ParseArguments(int aCount, const char ** aVector)
	{
        assert(NULL != aVector);

        for (int i = 1; i < aCount; i++)
        {
            const char * lArg = aVector[i];
            assert(NULL != lArg);

            char lData[LINE_LENGTH_MAX];

            if (1 == sscanf_s(lArg, "Execute=%[^\n\r\t]", lData SIZE_INFO(sizeof(lData)))) { ExecuteCommand(lData); }
        }
	}

    // NOT TESTED  KmsLib.ToolBase.ParseCommands
    //             Reading command from stdin

    int ToolBase::ParseCommands()
	{
        return ParseCommands(stdin);
	}

	int ToolBase::ParseCommands(const char * aFileName)
	{
		assert(NULL != aFileName);

        char lMsg[LINE_LENGTH_MAX];
        int  lResult;

		switch (aFileName[0])
		{
		case '\0':
		case '\n':
		case '\r':
		case '\t':
            sprintf_s(lMsg, "The file name is not valid (%s)\n", aFileName);
            lResult = SetError(-1, lMsg, REPORT_USER_ERROR);
            break;

		default:
            // TODO  KmsLib.ToolBase
            //       Low (Cleanup) Use TextFile
			FILE  * lFile;

			int lRet = fopen_s(&lFile, aFileName, "r");
			if (0 == lRet)
			{
				Report(REPORT_INFO);
				printf("Executing commands from file %s ...\n", aFileName);

				lResult = ParseCommands(lFile);

				lRet = fclose(lFile);
				assert( 0 == lRet );

				Report(REPORT_INFO);
				printf("End of file %s\n", aFileName);
			}
			else
			{
                sprintf_s(lMsg, "Cannot open file %s\n", aFileName);
                lResult = SetError(-2, lMsg, REPORT_ERROR);
			}
		}

        return lResult;
	}

	// Private
	/////////////////////////////////////////////////////////////////////////

    void ToolBase::Abort(const char * aArgs)
    {
        assert(NULL != aArgs);

        int  lCode;
        char lDesc[LINE_LENGTH_MAX];

        switch (sscanf_s(aArgs, "%d %[^\n\r]", &lCode, lDesc SIZE_INFO(sizeof(lDesc))))
        {
        case EOF: exit(0);
        case 1  : exit(SetError(lCode, "Abort", REPORT_FATAL_ERROR));
        case 2  : exit(SetError(lCode, lDesc  , REPORT_FATAL_ERROR));

        default:
            SetError(-3, "Invalid argument", REPORT_USER_ERROR);
        }
    }

    void ToolBase::ChangeDir(const char * aArgs)
    {
        assert(NULL != aArgs);

        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
            if (0 == chdir(aArgs))
        #endif

        #ifdef _KMS_WINDOWS_
            if (SetCurrentDirectory(aArgs))
        #endif

        {
            Report(REPORT_OK, aArgs);
        }
        else
        {
            SetError(-592, "Invalid directory", REPORT_USER_ERROR);
        }
    }

    void ToolBase::Delay(const char * aArgs)
    {
        assert(NULL != aArgs);

        unsigned int lDelay_ms;

        switch (sscanf_s(aArgs, "%u", &lDelay_ms))
        {
        case EOF:
            lDelay_ms = 3000;
            // No break;
        case 1:
            KmsLib::ThreadBase::Sleep_ms(lDelay_ms);
            break;

        default:
            SetError(-4, "Invalid argument", REPORT_USER_ERROR);
        }
    }

    void ToolBase::Error_Abort()
    {
        if (0 != mError_Code)
        {
            // NOT TESTED  KmsLib::ToolBase::Error_Abort
            //             Aborting execution
            exit(mError_Code);
        }
    }

    void ToolBase::Error_Display()
    {
        if (0 == mError_Code)
        {
            printf("No error\n");
        }
        else
        {
            printf("Error %d - %s\n", mError_Code, mError_Desc.c_str());
        }
    }

    void ToolBase::Error_Exit()
    {
        if (0 != mError_Code)
        {
            mExit = true;
        }
    }

	void ToolBase::ExecuteCommand(const char * aLine)
	{
		assert(NULL != aLine);

		ExecuteCommand(mCommands, aLine);
	}

	void ToolBase::ExecuteCommand(const KmsLib::ToolBase::CommandInfo * aCommands, const char * aLine)
	{
		assert(NULL != aCommands);
		assert(NULL != aLine    );

		char lName     [LINE_LENGTH_MAX];
		char lArguments[LINE_LENGTH_MAX];
        char lMsg      [LINE_LENGTH_MAX];

		bool	lFound	;
		size_t	lLen	;

		int lRet = sscanf_s(aLine, "%[0-1A-Za-z] %[^\n\r\t]", lName SIZE_INFO( LINE_LENGTH_MAX ), lArguments SIZE_INFO( LINE_LENGTH_MAX ) );
		switch (lRet)
		{
		case 1:
			memset(&lArguments, 0, sizeof(lArguments));

			// No break

		case 2:
			lLen = strlen(lName);

			for (unsigned int i = 0;; i++)
			{
				if (NULL == aCommands[i].mName)
				{
					lFound = false;
					break;
				}

				assert((NULL != aCommands[i].mFunction) || (NULL != aCommands[i].mSubCommands));
				assert((NULL == aCommands[i].mFunction) || (NULL == aCommands[i].mSubCommands));

				if (0 == _strnicmp(aCommands[i].mName, lName, lLen))
				{
					if (NULL != aCommands[i].mFunction)
					{
						aCommands[i].mFunction(this,lArguments);
					}
					else
					{
						ExecuteCommand(aCommands[i].mSubCommands, lArguments);
					}

					lFound = true;
					break;
				}
			}

			if (!lFound)
			{
				if (0 == _strnicmp("Help", lName, lLen))
				{
					DisplayHelp(aCommands);
				}
				else
				{
                    sprintf_s(lMsg, "Invalid command (%s)\n", lName);
                    SetError(-5, lMsg, REPORT_USER_ERROR);
				}
			}

			break;

		default:
			sprintf_s(lMsg, "Invalid command line (%s)\n", aLine);
            SetError(-6, lMsg, REPORT_USER_ERROR);
		}
	}

    void ToolBase::Exit(const char * aArgs)
    {
        assert(NULL != aArgs);

        int  lCode;
        char lDesc[LINE_LENGTH_MAX];

        switch (sscanf_s(aArgs, "%d %[^\n\r]", &lCode, lDesc SIZE_INFO(sizeof(lDesc))))
        {
        case EOF:
            mExit = true;
            break;

        case 1:
            strcpy_s(lDesc, "Exit");
            // No break;

        case 2:
            if (0 == lCode)
            {
                ClearError();
            }
            else
            {
                SetError(lCode, lDesc, REPORT_ERROR);
            }
            mExit = true;
            break;

        default:
            SetError(-7, "Invalid arguments", REPORT_USER_ERROR);
        }
    }

	int ToolBase::ParseCommands(FILE * aFile)
	{
		assert(NULL != aFile);

		while ( ! mExit )
		{
			if (stdin == aFile)
			{
                // NOT TESTED  KmsLib::ToolBase::Console
                //             Parse command from the stdin
				printf("\n> ");
			}

			char lLine[LINE_LENGTH_MAX];

			memset(&lLine, 0, sizeof(lLine));

			if (NULL == fgets(lLine, LINE_LENGTH_MAX - 1, aFile))
			{
				break;
			}

			size_t lLen = strlen(lLine);
			if ((0 < lLen) && ('#' != lLine[0]))
			{
				while (0 < lLen)
				{
					lLen--;
					switch (lLine[lLen])
					{
					case '\0' :
						assert(false);

					case ' '	:
					case '\n'	:
					case '\r'	:
					case '\t'	:
						lLine[lLen] = '\0';
						break;

					default:
						ExecuteCommand(lLine);
						lLen = 0;
					}
				}
			}
		}

        return mError_Code;
	}

    void ToolBase::Repeat(const char * aArgs)
    {
        assert(NULL != aArgs);

        char         lCommand[LINE_LENGTH_MAX];
        unsigned int lCount;

        switch (sscanf_s(aArgs, "%u %[^\n\r]", &lCount, lCommand SIZE_INFO(sizeof(lCommand))))
        {
        case 2:
            for (unsigned int i = 0; (! mExit) && ( i < lCount); i++)
            {
                ExecuteCommand(lCommand);
            }
            break;

        default:
            SetError(-8, "Invalid argument", REPORT_USER_ERROR);
        }
    }

    bool ToolBase::TextToBool(const char * aText, bool * aOut)
    {
        assert(NULL != aText);
        assert(NULL != aOut);

        if ((0 == _stricmp("false", aText)) || (0 == _stricmp("0", aText)))
        {
            *aOut = false;
            return true;
        }

        if ((0 == _stricmp("true", aText)) || (0 == _stricmp("1", aText)))
        {
            *aOut = true;
            return true;
        }

        SetError(__LINE__, "Invalid boolean value");
        return false;
    }

    bool ToolBase::Verify(unsigned int aValue, unsigned int aMin, unsigned int aMax)
    {
        assert(aMin <= aMax);

        if (aMin > aValue)
        {
            SetError(__LINE__, "The unsigned value is too small");
            return false;
        }

        if (aMax < aValue)
        {
            SetError(__LINE__, "The unsigned value is too large");
            return false;
        }

        return true;
    }

};

// Fonction statique
/////////////////////////////////////////////////////////////////////////////

void DisplayHelp(const KmsLib::ToolBase::CommandInfo * aCommandInfo)
{
	assert(NULL != aCommandInfo);

	printf("\n");

	for (unsigned int i = 0; NULL != aCommandInfo[i].mName; i++)
	{
		printf("%s\n", aCommandInfo[i].mHelpLine);
	}

	printf("Help                          Display this help message\n");
}

void GoOverBlank(const char ** aArg)
{
    assert(NULL != aArg);
    assert(NULL != (*aArg));

    while ((' ' == **aArg) || ('\t' == **aArg))
    {
        (*aArg)++;
    }
}

// aOut  [---;-W-]
//
// Exception  KmsLib::Exception  CODE_FILE_READ_ERROR
void ReadLine(FILE * aFile, char * aOut, unsigned int aOutSize_byte)
{
	memset(aOut, 0, aOutSize_byte);

	if (NULL == fgets(aOut, aOutSize_byte - 1, aFile))
	{
        // NOT TESTED  KmsLib.ToolBase.ReadLine
        //             Cannot read input line
		throw new KmsLib::Exception(KmsLib::Exception::CODE_FILE_READ_ERROR, "fgets( , ,  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, aOutSize_byte);
	}
}

int ScanUnsigned(const char ** aArg, unsigned int * aOut, bool aHex)
{
    assert(NULL != aArg);
    assert(NULL != (*aArg));
    assert(NULL != aOut);

    int lResult;

    if (aHex)
    {
        lResult = sscanf_s(*aArg, "%x", aOut);
        if (1 == lResult)
        {
            while ((('0' <= **aArg) && ('9' >= **aArg))
                || (('a' <= **aArg) && ('f' >= **aArg))
                || (('A' <= **aArg) && ('F' >= **aArg)))
            {
                (*aArg)++;
            }
        }
    }
    else
    {
        lResult = sscanf_s(*aArg, "%u", aOut);
        if (1 == lResult)
        {
            while (('0' <= **aArg) && ('9' >= **aArg))
            {
                (*aArg)++;
            }
        }
    }

    return lResult;
}
