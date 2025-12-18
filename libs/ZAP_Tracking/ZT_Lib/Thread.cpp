
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Thread.cpp

#include "Component.h"

// ===== ZT_Lib =============================================================
#include "ZT_Lib/Thread.h"

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void * Run_Link(void * aContext);

namespace ZT_Lib
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    Thread::Thread() : mState(STATE_INIT)
    {

        int lRet = pthread_cond_init(&mCond, NULL);
        assert(0 == lRet);

        lRet = pthread_mutex_init(&mZone0, NULL);
        assert(0 == lRet);
    }

    Thread::~Thread()
    {
        bool lJoin = false;

        Zone0_Enter();
            switch (mState)
            {
            case STATE_INIT:
                break;

            case STATE_RUNNING:
            case STATE_STARTING:
                mState = STATE_STOPPING;
                lJoin = true;
                break;

            default: assert(false);
            }
        Zone0_Leave();

        int lRet;

        if (lJoin)
        {
            lRet = pthread_join(mThread, NULL);
            assert(0 == lRet);
        }


        lRet = pthread_cond_destroy(&mCond);
        assert(0 == lRet);

        lRet = pthread_mutex_destroy(&mZone0);
        assert(0 == lRet);
    }

    void Thread::Condition_Signal()
    {
        int lRet = pthread_cond_signal(&mCond);
        assert(0 == lRet);
    }

    ZT::Result Thread::Condition_Wait()
    {
        int lRet = pthread_cond_wait(&mCond, &mZone0);
        if (0 != lRet)
        {
            return ZT::ZT_ERROR_THREAD;
        }

        return ZT::ZT_OK;
    }

    ZT::Result Thread::Condition_Wait(const timespec & aAbsTime)
    {
        int lRet = pthread_cond_timedwait(&mCond, &mZone0, &aAbsTime);
        if (0 != lRet)
        {
            return ZT::ZT_ERROR_TIMEOUT;
        }

        return ZT::ZT_OK;
    }

    ZT::Result Thread::Start(ZT::IMessageReceiver * aReceiver, unsigned int aStart, unsigned int aIteration, unsigned int aStop)
    {
        assert(NULL != aReceiver);

        ZT::Result lResult = ZT::ZT_ERROR_STATE;

        Zone0_Enter();
        {
            switch (mState)
            {
            case STATE_INIT:
                mReceiver           = aReceiver;
                mReceiver_Iteration = aIteration;
                mReceiver_Start     = aStart;
                mReceiver_Stop      = aStop;
                
                mState = STATE_STARTING;

                int lRetI;

                lRetI = pthread_create(&mThread, NULL, Run_Link, this);
                assert(0 == lRetI);

                lResult = ZT::ZT_OK;
                break;

            case STATE_RUNNING:
            case STATE_STARTING:
            case STATE_STOPPING:
                break;

            default: assert(false);
            }

        }
        Zone0_Leave();

        return lResult;
    }

    ZT::Result Thread::Stop()
    {
        ZT::Result lResult = ZT::ZT_ERROR_STATE;

        Zone0_Enter();
        {
            switch (mState)
            {
            case STATE_INIT: lResult = ZT::ZT_OK; break;

            case STATE_RUNNING:
            case STATE_STARTING:
                mState = STATE_STOPPING;

                int lRet;
                
                Zone0_Leave();
                {
                    lRet = pthread_join(mThread, NULL);
                }
                Zone0_Enter();
                lResult = (0 == lRet) ? ZT::ZT_OK : ZT::ZT_ERROR_THREAD;
                break;

            case STATE_STOPPING: lResult = ZT::ZT_ERROR_ALREADY_STOPPING; break;

            default: assert(false);
            }
        }
        Zone0_Leave();

        return lResult;
    }

    void Thread::Zone0_Enter()
    {
        int lRet = pthread_mutex_lock(&mZone0);
        assert(0 == lRet);
    }

    void Thread::Zone0_Leave()
    {
        int lRet = pthread_mutex_unlock(&mZone0);
        assert(0 == lRet);
    }

    // Internal
    // //////////////////////////////////////////////////////////////////////

    void * Thread::Run()
    {
        bool lRun = true;

        do
        {
            Zone0_Enter();
            {
                try
                {
                    switch (mState)
                    {
                    case STATE_STARTING:
                        mState = STATE_RUNNING;

                        lRun = Call_Z0(mReceiver_Start);
                        if (!lRun)
                        {
                            break;
                        }
                        // no break

                    case STATE_RUNNING:
                        lRun = Call_Z0(mReceiver_Iteration);
                        break;

                    case STATE_STOPPING: lRun = false; break;

                    default:
                        assert(false);
                        lRun = false;
                    }
                }
                catch (...)
                {
                    TRACE_ERROR(stderr, "DJI_Gimbal::Run - Exception");
                    lRun = false;
                }
            }
            Zone0_Leave();
        }
        while (lRun);

        Zone0_Enter();
        {
            switch (mState)
            {
            case STATE_INIT: break;

            case STATE_RUNNING:
            case STATE_STOPPING:
                mState = STATE_INIT;
                Call_Z0(mReceiver_Stop);
                break;

            default: assert(false);
            }
        }
        Zone0_Leave();

        return 0;
    }

    // Private
    // //////////////////////////////////////////////////////////////////////

    bool Thread::Call_Z0(unsigned int aCode)
    {
        assert(NULL != mReceiver);

        bool lResult;

        Zone0_Leave();
        {
            lResult = mReceiver->ProcessMessage(this, aCode, NULL);
        }
        Zone0_Enter();

        return lResult;
    }

}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void * Run_Link(void * aContext)
{
    assert(NULL != aContext);

    ZT_Lib::Thread * lThis = reinterpret_cast<ZT_Lib::Thread *>(aContext);

    return lThis->Run();
}
