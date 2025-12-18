
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2021 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KmsBase
// File      KmsLib/ThreadBase.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
    #include <pthread.h>
    #include <signal.h>
#endif

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

#include <KmsLib/ThreadBase.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#define WAIT_RESULT_OK             (0)
#define WAIT_RESULT_TIMEOUT        (1)
#define WAIT_RESULT_ERROR          (2)
#define WAIT_RESULT_CRITICAL_ERROR (3)

#ifdef _KMS_WINDOWS_
    static const int PRIORITIES[KmsLib::ThreadBase::PRIORITY_QTY] =
    {
        0,
        THREAD_PRIORITY_IDLE         ,
        THREAD_PRIORITY_BELOW_NORMAL ,
        THREAD_PRIORITY_NORMAL       ,
        THREAD_PRIORITY_ABOVE_NORMAL ,
        THREAD_PRIORITY_TIME_CRITICAL,
    };
#endif

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
    static void * Run(void * aParameter);
#endif

#ifdef _KMS_WINDOWS_
    static DWORD WINAPI Run(PVOID aParameter);
#endif

namespace KmsLib
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    void ThreadBase::Sleep_ms(unsigned int aDelay_ms)
    {
        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
            usleep(aDelay_ms * 1000);
        #endif

        #ifdef _KMS_WINDOWS_
            Sleep(aDelay_ms);
        #endif
    }

    void ThreadBase::Sleep_s(unsigned int aDelay_s)
    {
        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
            sleep(aDelay_s);
        #endif

        #ifdef _KMS_WINDOWS_
            Sleep(1000 * aDelay_s);
        #endif
    }

    void ThreadBase::Sleep_us(unsigned int aDelay_us)
    {
        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)
            usleep(aDelay_us);
        #endif

        #ifdef _KMS_WINDOWS_
            Sleep(aDelay_us / 1000);
        #endif
    }

    ThreadBase::~ThreadBase()
    {
        #ifdef _KMS_WINDOWS_
            if (NULL != mThread)
            {
                bool lRetB = Wait(true, 10000);
                assert(lRetB);
            }
        #endif
    }

    ThreadBase::Priority ThreadBase::GetCurrentPriority() const
    {
        if (STATE_INIT != mState)
        {

            #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

                return PRIORITY_NORMAL;

            #endif

            #ifdef _KMS_WINDOWS_

                assert(NULL != mThread);

                switch (GetThreadPriority(mThread))
                {
                case THREAD_PRIORITY_ABOVE_NORMAL: return PRIORITY_HIGH;
                case THREAD_PRIORITY_BELOW_NORMAL: return PRIORITY_LOW ;

                case THREAD_PRIORITY_ERROR_RETURN:
                    throw new Exception(Exception::CODE_THREAD_ERROR, "GetThreadPriority(  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, 0);

                case THREAD_PRIORITY_HIGHEST      :
                case THREAD_PRIORITY_TIME_CRITICAL:
                    return PRIORITY_CRITICAL;

                case THREAD_PRIORITY_IDLE  :
                case THREAD_PRIORITY_LOWEST:
                    return PRIORITY_IDLE;

                case THREAD_PRIORITY_NORMAL: return PRIORITY_NORMAL;

                default: assert(false);
                }

            #endif

        }

        return PRIORITY_UNKNOWN;
    }

    ThreadBase::Priority ThreadBase::GetPriority() const
    {
        assert(PRIORITY_UNKNOWN != mPriority);
        assert(PRIORITY_QTY     >  mPriority);

        return mPriority;
    }

    bool ThreadBase::IsStopping() const
    {
        switch (mState)
        {
        case STATE_INIT           :
        case STATE_RUNNING        :
        case STATE_START_REQUESTED:
        case STATE_STARTING       :
            break;

        case STATE_STOP_REQUESTED:
        case STATE_STOPPING      :
            return true;

        default: assert(false);
        }

        return false;
    }

    void ThreadBase::SetPriority(ThreadBase::Priority aPriority)
    {
        assert(PRIORITY_UNKNOWN != aPriority);
        assert(PRIORITY_QTY     >  aPriority);

        assert(PRIORITY_UNKNOWN != mPriority);
        assert(PRIORITY_QTY     >  mPriority);
        assert(STATE_QTY        >  mState   );

        mPriority = aPriority;

        if (STATE_INIT != mState)
        {
            ApplyPriority();
        }
    }

    void ThreadBase::Start()
    {
        switch (mState)
        {
        case STATE_STOPPING:
            CloseThread();
            assert(STATE_INIT == mState);
            // no break;

        case STATE_INIT    :

            mState = STATE_START_REQUESTED;

            #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

                int lRet;

                lRet = pthread_create(&mThread, NULL, ::Run, this);
                if (0 != lRet)
                {
                    mState = STATE_INIT;

                    throw new Exception(Exception::CODE_THREAD_ERROR, "pthread_create( , , ,  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, lRet);
                }

            #endif

            #ifdef _KMS_WINDOWS_

                assert(NULL == mThread);

                mThread = CreateThread(NULL, 0, ::Run, this, 0, &mThreadId);
                if (NULL == mThread)
                {
                    // NOT TESTED  KmsLib.ThreadBase.ErrorHandling
                    //             CreateThread fail
                    mState = STATE_INIT;

                    throw new Exception(Exception::CODE_THREAD_ERROR, "CreateThread( , , , , ,  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, 0);
                }

            #endif

            break;

        default:
            throw new Exception(Exception::CODE_STATE_ERROR, "Invalid state", NULL, __FILE__, __FUNCTION__, __LINE__, mState);
        }
    }

    void ThreadBase::Stop()
    {
        switch (mState)
        {
        case STATE_INIT          :
        case STATE_STOP_REQUESTED:
        case STATE_STOPPING      :
            break;

        case STATE_RUNNING:
            mState = STATE_STOP_REQUESTED;
            break;

        default: assert(false);

        case STATE_START_REQUESTED:
        case STATE_STARTING       :
            // NOT TESTED  KmsLib.ThreadBase.ErrorHandling
            //             Stop while still starting
            throw new Exception(Exception::CODE_STATE_ERROR, "Invalid state", NULL, __FILE__, __FUNCTION__, __LINE__, mState);
        }
    }

    bool ThreadBase::StopAndWait(bool aForce, unsigned int aTimeout_ms)
    {
        Stop();

       return Wait(aForce, aTimeout_ms);
    }

    bool ThreadBase::Wait(bool aForce, unsigned int aTimeout_ms)
    {
        bool lResult = false;

        switch (mState)
        {
        case STATE_INIT :
            lResult = true;
            break;

        case STATE_RUNNING        :
        case STATE_START_REQUESTED:
        case STATE_STARTING       :
            mState = STATE_STOP_REQUESTED;
            // no break;

        case STATE_STOP_REQUESTED :
        case STATE_STOPPING       :
            switch (Wait_Internal(aTimeout_ms))
            {
            case WAIT_RESULT_ERROR:
                if (aForce)
                {
                    Terminate();
                }
                throw new Exception(Exception::CODE_THREAD_ERROR, "Wait_Internal(  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, 0);

            case WAIT_RESULT_CRITICAL_ERROR :
                CloseThread();
                throw new Exception(Exception::CODE_THREAD_ERROR, "Wait_Internal(  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, 0);

            case WAIT_RESULT_OK:
                CloseThread();
                lResult = true;
                break;

            case WAIT_RESULT_TIMEOUT:
                if (aForce)
                {
                    Terminate();
                }
                break;

            default: assert(false);
            }
            break;

        default: assert(false);
        }
    
        return lResult;
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    // Return
    //      0  OK
    //  Other  Error
    unsigned int ThreadBase::Run_Internal()
    {
        assert(STATE_START_REQUESTED == mState);

        mState = STATE_STARTING;

        unsigned int lResult;

        try
        {
            ApplyPriority();

            mState = STATE_RUNNING;

            lResult = Run();
        }
        catch (...)
        {
            lResult = __LINE__;
        }

        mState = STATE_STOPPING;

        return lResult;
    }

    // Protected
    /////////////////////////////////////////////////////////////////////////

    ThreadBase::ThreadBase()
        : mPriority(PRIORITY_NORMAL)
        , mState   (STATE_INIT     )
        #ifdef _KMS_WINDOWS_
            , mThread  (NULL)
            , mThreadId(   0)
        #endif
    {
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    // Exception  Exception  CODE_THREAD_ERROR
    void ThreadBase::ApplyPriority()
    {
        assert(PRIORITY_UNKNOWN != mPriority);
        assert(PRIORITY_QTY     >  mPriority);

        #ifdef _KMS_WINDOWS_

            assert(NULL != mThread);

            if (!SetThreadPriority(mThread, PRIORITIES[mPriority]))
            {
                // NOT TESTED  KmsLib.ThreadBase.ErrorHandling
                //             SetThreadPriority fail
                throw new Exception(Exception::CODE_THREAD_ERROR, "SetThreadPriority( ,  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, mPriority);
            }

        #endif
    }

    void ThreadBase::CloseThread()
    {
        assert(STATE_INIT != mState);
        assert(STATE_QTY  >  mState);

        #ifdef _KMS_WINDOWS_

            assert(NULL != mThread);

            BOOL lRetB = CloseHandle(mThread);
            assert(lRetB);

            mThread = NULL;

        #endif

        mState  = STATE_INIT;
    }

    // Exception  Exception  CODE_THREAD_ERROR
    void ThreadBase::Terminate()
    {
        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

            int lRet = pthread_kill(mThread, SIGKILL);

        #endif

        #ifdef _KMS_WINDOWS_

            assert(NULL != mThread);

            BOOL lRetB = TerminateThread(mThread, __LINE__);

        #endif

        CloseThread();

        assert(STATE_INIT == mState);

        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

            if (0 != lRet)
            {
                throw new Exception(Exception::CODE_THREAD_ERROR, "pthread_kill( ,  ) failed", NULL, __FILE__, __FUNCTION__, __LINE__, lRet);
            }

        #endif

        #ifdef _KMS_WINDOWS_

            assert(NULL == mThread);

            if (!lRetB)
            {
                // NOT TESTED  KmsLib::ThreadBase.ErrorHandling
                //             TerminateThread fail
                throw new Exception(Exception::CODE_THREAD_ERROR, "TerminateThread( ,  ) failed", 0, __FILE__, __FUNCTION__, __LINE__, 0);
            }

        #endif
    }

    // aTimeout_ms
    //
    // Return  See WAIT_RESULT_...
    unsigned int ThreadBase::Wait_Internal(unsigned int aTimeout_ms)
    {
        unsigned int lResult;

        #if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

            switch ( pthread_join(mThread, NULL) )
            {
            case 0     :
            case ESRCH :
                lResult = WAIT_RESULT_OK;
                break;

            case EDEADLK :
                lResult = WAIT_RESULT_ERROR;
                break;

            default :
                assert( false );
                // No break;

            case EINVAL :
                lResult = WAIT_RESULT_CRITICAL_ERROR;
            }

        #endif

        #ifdef _KMS_WINDOWS_

            assert(NULL != mThread);

            switch (WaitForSingleObject(mThread, aTimeout_ms))
            {
            case WAIT_OBJECT_0: lResult = WAIT_RESULT_OK     ; break;
            case WAIT_TIMEOUT : lResult = WAIT_RESULT_TIMEOUT; break;
            default           : lResult = WAIT_RESULT_ERROR  ; break;
            }

        #endif

        return lResult;
    }

}

// Static functions
/////////////////////////////////////////////////////////////////////////////

// ===== Entry point ========================================================

#if defined(_KMS_LINUX_) || defined(_KMS_OS_X_)

    void * Run(void * aParameter)
    {
        assert(NULL != aParameter);

        KmsLib::ThreadBase * lThis = reinterpret_cast<KmsLib::ThreadBase *>(aParameter);

        return reinterpret_cast<void *>(lThis->Run_Internal());
    }

#endif

#ifdef _KMS_WINDOWS_

    DWORD WINAPI Run(PVOID aParameter)
    {
        assert(NULL != aParameter);

        KmsLib::ThreadBase * lThis = reinterpret_cast<KmsLib::ThreadBase *>(aParameter);

        return lThis->Run_Internal();
    }

#endif
