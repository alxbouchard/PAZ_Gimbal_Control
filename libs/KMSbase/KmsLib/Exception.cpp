
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib/Exception.cpp

// CODE REVIEW    2019-07-23  KMS - Martin Dubois, ing.

// TEST COVERAGE  2019-07-23  KMS - Martin Dubois, ing.

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

// ===== C++ ================================================================
#include <map>

#ifdef _KMS_OS_X_
    // ===== System =========================================================
    #include <sys/errno.h>
#endif

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KmsLib/Exception.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

static const char * CODE_NAMES[ KmsLib::Exception::CODE_QTY ] =
{
    "CODE_IO_ERROR"         , // 0
    "CODE_IOCTL_ERROR"      ,
    "CODE_NO_SUCH_DEVICE"   ,
    "CODE_NOT_ENOUGH_MEMORY",
    "CODE_PRIVATE"          ,
    "CODE_REGISTRY_ERROR"   , // 5
    "CODE_SETUP_API_ERROR"  ,
    "CODE_UNKNOWN"          ,
    "CODE_USER_ERROR"       ,

    "CODE_AUTHENTIFICATION_ERROR",
    "CODE_COMMUNICATION_ERROR"   , // 10
    "CODE_GUI_ERROR"             ,
    "CODE_HARDWARE_ERROR"        ,
    "CODE_INSTALLATION_ERROR"    ,
    "CODE_INVALID_ARGUMENT"      ,
    "CODE_INVALID_BUFFER_ADDRESS", // 15
    "CODE_INVALID_BUFFER_SIZE"   ,
    "CODE_INVALID_DATA"          ,
    "CODE_INVALID_DATE"          ,
    "CODE_INVALID_HANDLE"        ,
    "CODE_INVALID_TIME"          , // 20
    "CODE_NETWORK_ERROR"         ,
    "CODE_NOT_FOUND"             ,
    "CODE_OVERFLOW"              ,
    "CODE_PERMISSION_DENIED"     ,
    "CODE_PROTOCOL_ERROR"        , // 25
    "CODE_STATE_ERROR"           ,
    "CODE_SYSTEM_ERROR"          ,
    "CODE_THREAD_ERROR"          ,
    "CODE_TIMEOUT"               ,
    "CODE_UNDERFLOW"             , // 30

    "CODE_SERVICE_MANAGER_ERROR",
    "CODE_SYSTEM_LOG_ERROR"     ,

    "CODE_ACCESS_VIOLATION"      ,
    "CODE_INTEGER_DIVIDE_BY_ZERO",
    "CODE_INVALID_COMMAND_LINE"  , // 35
    "CODE_OPEN_CL_ERROR"         ,
    "CODE_STRUCTURED_EXCEPTION"  ,

    "CODE_CLOSE_HANDLE_ERROR" ,
    "CODE_BUFFER_TOO_SMALL"   ,
    "CODE_COM_PORT_ERROR"     , // 40
    "CODE_COPY_FILE_ERROR"    ,
    "CODE_CREATE_FILE_ERROR"  ,
    "CODE_DELETE_FILE_ERROR"  ,
    "CODE_DENIED"             ,
    "CODE_DIRECTORY_ERROR"    , // 45
    "CODE_ERROR"              ,
    "CODE_FILE_OPEN_ERROR"    ,
    "CODE_FILE_READ_ERROR"    ,
    "CODE_FILE_WRITE_ERROR"   ,
    "CODE_INVALID_ADDRESS"    , // 50
    "CODE_INVALID_PARAMETER"  ,
    "CODE_INVALID_PORT_NUMBER",
    "CODE_IO_CANCEL_ERROR"    ,
    "CODE_MOVE_FILE_ERROR"    ,
    "CODE_NO_COM_PORT"        , // 55
    "CODE_NOT_IMPLEMENTED"    ,
    "CODE_READ_FILE_ERROR"    ,
    "CODE_REG_CLOSE_ERROR"    ,
    "CODE_REG_CREATE_ERROR"   ,
    "CODE_REG_DELETE_ERROR"   , // 60
    "CODE_REG_OPEN_ERROR"     ,
    "CODE_REG_QUERY_ERROR"    ,
    "CODE_REG_SET_ERROR"      ,
    "CODE_SOCKET_ERROR"       ,
    "CODE_TO_DEFINE"          , // 65
    "CODE_UNSUCCESSFUL"       ,
    "CODE_WINSOCK_ERROR"      ,
    "CODE_WRITE_FILE_ERROR"   , // 58
};

// Data type
/////////////////////////////////////////////////////////////////////////////

typedef std::map< KmsLib::Exception::Code, std::string > NameMap;

// Static variables
/////////////////////////////////////////////////////////////////////////////

static NameMap sNames;

// Static method declarations
/////////////////////////////////////////////////////////////////////////////

// ===== Entry points =======================================================

#ifdef _KMS_WINDOWS_
    static void TranslateException(unsigned int aCode, struct _EXCEPTION_POINTERS * aStruct );
#endif // _KMS_WINDOWS_

namespace KmsLib
{

	// Public
	/////////////////////////////////////////////////////////////////////////

	void * Exception::RegisterTranslator()
	{
		#if defined( _KMS_LINUX_ ) || defined( _KMS_OS_X_ )
		    return NULL;
		#endif

		#ifdef _KMS_WINDOWS_
		    return _set_se_translator(TranslateException);
		#endif
	}

	void Exception::RestoreTranslator(void * aTranslator)
	{
		#ifdef _KMS_WINDOWS_
		    void * lTranslator = _set_se_translator(reinterpret_cast<_se_translator_function>(aTranslator));
		    assert(TranslateException == lTranslator);
		#endif
	}

    Exception::Code Exception::DefineCode(unsigned int aIndex, const char * aName)
    {
        Code lResult = static_cast<Code>(CODE_BASE + aIndex);

        SetCodeName(lResult, aName);

        return lResult;
    }

    const  char  * Exception::GetCodeName(Code  aCode)
    {
        NameMap::iterator lIt = sNames.find(aCode);
        if (sNames.end() != lIt)
        {
            return lIt->second.c_str();
        }

        if (CODE_QTY <= aCode)
        {
            return  NULL;
        }

        return  CODE_NAMES[aCode];
    }

    void Exception::SetCodeName(Code aCode, const char * aName)
    {
        if (NULL == aName)
        {
            sNames.erase(aCode);
        }
        else
        {
            NameMap::iterator lIt = sNames.find(aCode);
            if (sNames.end() == lIt)
            {
                sNames.insert(NameMap::value_type(aCode, aName));
            }
            else
            {
                lIt->second = aName;
            }
        }
    }

	Exception::Exception(Code aCode, const char * aWhat, const char * aMessage, const char * aFile, const char * aFunction, unsigned int aLine, unsigned int aInfoA) :
		mCode		(aCode			),
		mFile		(aFile			),
		mFunction	(aFunction		),
		mInfoA		(aInfoA			),
		mLine		(aLine			),
		mWhat		(aWhat			),
        #ifdef _KMS_WINDOWS_
            mLastError( ::GetLastError() )
        #else
            mLastError( errno )
        #endif
	{
		assert(NULL != aFile    );
		assert(NULL != aFunction);
		assert(0	<  aLine    );
		assert(NULL != aWhat    );

		if (NULL != aMessage)
		{
			mMessage = aMessage;
		}
	}

	const char * Exception::GetFile() const
	{
		assert(NULL != mFile);

		return mFile;
	}

	const char * Exception::GetFunction() const
	{
		return mFunction;
	}

	unsigned int Exception::GetLine() const
	{
		return mLine;
	}

	void Exception::Write(FILE * aFile) const
	{
		assert(NULL != aFile);

		fprintf(aFile,
            "    Exception - %s (%u, 0x%08x)\n"
			"        File       : %s (line %u)\n"
			"        Function   : %s\n"
			"        Info A     : %u (0x%08x)\n"
			"        Last error : %u (0x%08x)\n",
            GetCodeName(mCode), mCode, mCode,
            mFile, mLine,
			mFunction,
			mInfoA, mInfoA,
			mLastError, mLastError );

		if (0 < mMessage.size())
		{
			fprintf(aFile,
				"        Message    : %s\n",
				mMessage.c_str());
		}

		fprintf(aFile,
			"        What       : %s\n"
			"\n",
			what());
	}
	
	// ===== std::exception ==================================================
	
	Exception::~Exception() throw ()
	{
	}
	
	const char * Exception::what() const throw ()
	{
		assert(NULL != mWhat);
		
		return mWhat;
	}
	
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

// ===== Entry points =======================================================

#ifdef _KMS_WINDOWS_

    void TranslateException(unsigned int aCode, struct _EXCEPTION_POINTERS * aStruct )
    {
        assert(NULL != aStruct);

        KmsLib::Exception::Code   lCode ;
        const char              * lMsg  ;

        switch (aCode)
        {
        case 0xc0000005: lCode = KmsLib::Exception::CODE_ACCESS_VIOLATION	   ; lMsg = "Access violation"   ; break;
        case 0xc0000094: lCode = KmsLib::Exception::CODE_INTEGER_DIVIDE_BY_ZERO; lMsg = "Integet divide by 0"; break;

        default:
            // NOT TESTED  KmsLib.Exception
            //             Unexpected structured exception
            lCode = KmsLib::Exception::CODE_STRUCTURED_EXCEPTION; lMsg = "Structured exception";
        }

        throw new KmsLib::Exception(lCode, lMsg, NULL, __FILE__, __FUNCTION__, __LINE__, aCode);
    }

#endif
