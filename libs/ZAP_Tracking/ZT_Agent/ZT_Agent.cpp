
// Author  KMS - Martin Dubois, P.Eng
// Client  ZAP
// Product Tracking
// File    ZT_Agent/ZT_Agent.cpp

// CODE REVIEW 2021-07-22 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C++ ================================================================
#include <list>

// ===== Import/Includes ====================================================
#include <KmsTool.h>

// ===== Includes ===========================================================
#include <ZT/ISystem.h>

// ===== ZT_Agent ===========================================================
#include "../Common/Version.h"

#include "Instance.h"
#include "MessageReceiver.h"

// Data types
// //////////////////////////////////////////////////////////////////////////

typedef std::list<Instance *> InstanceList;

// Static variables
// //////////////////////////////////////////////////////////////////////////

static InstanceList    sInstances;
static MessageReceiver sReceiver;
static ZT::ISystem *   sSystem = NULL;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static ZT::Result Init();

static ZT::Result Start();
static void       Stop();

static void Uninit();

extern "C"
{
    static void OnSigPipe(int aSig);
}

// Entry points
// //////////////////////////////////////////////////////////////////////////

int main()
{
    KMS_TOOL_BANNER("Tracking", "ZT_Agent", VERSION_STR, VERSION_TYPE);

    sig_t lPSH = signal(SIGPIPE, OnSigPipe);
    assert(SIG_ERR != lPSH);

    if (ZT::ZT_OK == Init())
    {
        if (0 < sInstances.size())
        {
            if (ZT::ZT_OK == Start())
            {
                while (!sReceiver.IsStopRequested())
                {
                    sleep(1);
                }

                Stop();
            }
        }
        else
        {
            // OSX will then restart the agent and it will check for gamepad
            // again.
            sleep(10);
        }
    }

    Uninit();

    return 0;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

ZT::Result Init()
{
    sSystem = ZT::ISystem::Create();
    assert(NULL != sSystem);

    ZT::Result lResult = sSystem->Gamepads_Detect();
    if (ZT::ZT_OK == lResult)
    {
        lResult = sSystem->Gimbals_Detect();
        if (ZT::ZT_OK == lResult)
        {
            ZT::IGamepad * lGamepad;

            while (NULL != (lGamepad = sSystem->Gamepad_Get(0)))
            {
                Instance * lInstance = new Instance(lGamepad, sInstances.size());
                assert(NULL != lInstance);

                lResult = lInstance->Init(sSystem);
                if (ZT::ZT_OK != lResult)
                {
                    fprintf(stderr, "ERROR  Instance::Init(  )  failed (%s)\n", ZT::Result_GetName(lResult));
                    break;
                }

                sInstances.push_back(lInstance);
            }
        }
        else
        {
            fprintf(stderr, "ERROR  ISystem::Gimbals_Detect()  failed (%s)\n", ZT::Result_GetName(lResult));
        }
    }
    else
    {
        fprintf(stderr, "ERROR  ISystem::Gamepads_Detect()  failed (%s)\n", ZT::Result_GetName(lResult));
    }

    return lResult;
}

ZT::Result Start()
{
    ZT::Result lResult = ZT::ZT_OK;

    for (InstanceList::iterator lIt = sInstances.begin(); (ZT::ZT_OK == lResult) && (lIt != sInstances.end()); lIt++)
    {
        assert(NULL != (*lIt));

        lResult = (*lIt)->Start(&sReceiver, MessageReceiver::CODE);
    }

    return lResult;
}

void Stop()
{
    for (InstanceList::iterator lIt = sInstances.begin(); lIt != sInstances.end(); lIt++)
    {
        assert(NULL != (*lIt));

        (*lIt)->Stop();
    }
}

void Uninit()
{
    assert(NULL != sSystem);

    for (InstanceList::iterator lIt = sInstances.begin(); lIt != sInstances.end(); lIt++)
    {
        assert(NULL != (*lIt));

        delete (*lIt);
    }

    sSystem->Release();
}

void OnSigPipe(int aSig)
{
    fprintf(stderr, "WARNING  OnSigPip( %d )\n", aSig);
}
