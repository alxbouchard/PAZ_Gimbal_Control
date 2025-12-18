
// Author  KMS - Martin Dubois, P.Eng
// Client  ZAP
// Product Tracking
// File    ZT_Agent/Instance.cpp

// CODE REVIEW 2021-07-22 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== ZT_Agent ===========================================================
#include "MessageReceiver.h"

#include "Instance.h"

// Public
// //////////////////////////////////////////////////////////////////////////

Instance::Instance(ZT::IGamepad * aGamepad, unsigned int aIndex)
    : mControlLink(ZT::IControlLink::Create())
    , mGamepad(aGamepad)
    , mIndex(aIndex)
{
    assert(NULL != aGamepad);

    assert(NULL != mControlLink);
}

Instance::~Instance()
{
    assert(NULL != mControlLink);
    assert(NULL != mGamepad);

    mControlLink->Release();
    mGamepad    ->Release();
}

ZT::Result Instance::Init(ZT::ISystem * aSystem)
{
    assert(NULL != aSystem);

    assert(NULL != mControlLink);

    ZT::Result lResult = ZT::ZT_OK;

    char lFileName[1024];

    sprintf(lFileName, "%s/.ZT_Gamepad_%u.txt", getenv("HOME"), mIndex);
    if (0 == access(lFileName, R_OK))
    {
        printf("Reading %s\n", lFileName);
        lResult = mControlLink->ReadConfigFile(lFileName);
    }

    if (ZT::ZT_OK == lResult)
    {
        lResult = mControlLink->Gamepad_Set(mGamepad);
        assert(ZT::ZT_OK == lResult);

        lResult = mControlLink->Gimbals_Set(aSystem);
        if (ZT::ZT_OK != lResult)
        {
            fprintf(stderr, "ERROR  IControlLink::Gimbals_Set(  )  failed (%s)\n", ZT::Result_GetName(lResult));
        }
    }
    else
    {
        fprintf(stderr, "ERROR  ZT::IControlLink::ReadConfigFile( \"%s\" )  failed (%s)\n", lFileName, ZT::Result_GetName(lResult));
    }

    return lResult;
}

ZT::Result Instance::Start(MessageReceiver * aReceiver, unsigned int aCode)
{
    assert(NULL != aReceiver);

    assert(NULL != mControlLink);

    ZT::Result lResult = mControlLink->Receiver_Set(aReceiver, aCode, 0);
    assert(ZT::ZT_OK == lResult);

    lResult = mControlLink->Start();
    if (ZT::ZT_OK != lResult)
    {
        fprintf(stderr, "ERROR  IControlLink::Start()  failed (%s)\n", ZT::Result_GetName(lResult));
    }

    return lResult;
}

void Instance::Stop()
{
    assert(NULL != mControlLink);

    mControlLink->Stop();
}
