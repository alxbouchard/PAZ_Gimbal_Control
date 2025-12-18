
// Author  KMS - Martin Dubois, P.Eng.
// Client  ZAP
// Product Tacking
// File    ZT_Lib/Thread.h

#pragma once

// ===== C ==================================================================
#include <pthread.h>

// ===== Includes ===========================================================
#include <ZT/IMessageReceiver.h>
#include <ZT/Result.h>

namespace ZT_Lib
{
    class Thread
    {

    public:

        Thread();

        ~Thread();

        void Condition_Signal();

        ZT::Result Condition_Wait();
        ZT::Result Condition_Wait(const timespec & aAbsTime);

        ZT::Result Start(ZT::IMessageReceiver * aReceiver, unsigned int aStart, unsigned int aIteration, unsigned int aStop);

        ZT::Result Stop();

        void Zone0_Enter();
        void Zone0_Leave();

    // Internal

        void * Run();

    private:

        // --> INIT <==+-----------+
        //      |      |           |
        //      +--> STARTING --> STOPPING <--+
        //            |                       |
        //            +--> RUNNING -----------+
        typedef enum
        {
            STATE_INIT,
            STATE_RUNNING,
            STATE_STARTING,
            STATE_STOPPING,
        }
        State;

        bool Call_Z0(unsigned int aCode);

        pthread_t mThread;

        ZT::IMessageReceiver *mReceiver;
        unsigned int          mReceiver_Iteration;
        unsigned int          mReceiver_Start;
        unsigned int          mReceiver_Stop;

        // ===== Zone 0 =========================================================
        pthread_mutex_t mZone0;

        pthread_cond_t mCond;

        State mState;

    };

}
