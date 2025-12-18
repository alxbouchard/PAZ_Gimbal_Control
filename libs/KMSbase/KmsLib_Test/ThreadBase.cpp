
// Author   KMS - Martin Dubois, ing.
// Product  KmsBase
// File     KmsLib_Test/ThreadBase.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <assert.h>

// ===== C++ ================================================================
#include <exception>

// ===== Includes ===========================================================
#include <KmsTest.h>

// ----- KmsLib -------------------------------------------------------------
#include <KmsLib/Exception.h>
#include <KmsLib/ThreadBase.h>

// Class
/////////////////////////////////////////////////////////////////////////////

class Test : public KmsLib::ThreadBase
{

public:

    typedef enum
    {
        MODE_EXCEPTION,
        MODE_HANG     ,
        MODE_NORMAL   ,

        MODE_QTY
    }
    Mode;

    Test();

    void SetMode(Mode aMode);

protected:

    // ===== KmsLib::ThreadBase =============================================
    virtual unsigned int Run();

private:

    Mode mMode;

};

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(ThreadBase_Base)
{
    Test lT0;

    // ===== GetCurrentPriority =============================================
    KMS_TEST_COMPARE(KmsLib::ThreadBase::PRIORITY_UNKNOWN, lT0.GetCurrentPriority());

    // ===== GetPriority ====================================================
    KMS_TEST_COMPARE(KmsLib::ThreadBase::PRIORITY_NORMAL, lT0.GetPriority());

    // ===== GetState =======================================================
    KMS_TEST_COMPARE(KmsLib::ThreadBase::STATE_INIT, lT0.GetState());

    // ===== IsRunning ======================================================
    KMS_TEST_COMPARE(false, lT0.IsRunning());

    // ===== IsStopped ======================================================
    KMS_TEST_COMPARE(1, lT0.IsStopped());

    // ===== IsStopping =====================================================
    KMS_TEST_COMPARE(false, lT0.IsStopping());

    // ===== SetPriority ====================================================
    lT0.SetPriority(KmsLib::ThreadBase::PRIORITY_IDLE);

    // ===== Start ==========================================================
    lT0.Start();

    try
    {
        lT0.Start();
        KMS_TEST_ASSERT(false);
    }
    catch (KmsLib::Exception * eE)
    {
        KMS_TEST_ERROR_INFO;
        eE->Write(stdout);
        KMS_TEST_COMPARE(KmsLib::Exception::CODE_STATE_ERROR, eE->GetCode());
    }

    KmsLib::ThreadBase::Sleep_ms(1);

    // ===== Stop ===========================================================
    lT0.Stop();

    KmsLib::ThreadBase::Sleep_ms(1);

    // ===== StopAndWait ====================================================
    KMS_TEST_ASSERT(lT0.StopAndWait(0, 0xffffffff));

    // ===== Sequences ======================================================

    #ifdef _KMS_WINDOWS_

        lT0.SetMode(Test::MODE_HANG);
        lT0.SetPriority(KmsLib::ThreadBase::PRIORITY_LOW);
        lT0.Start();

        KmsLib::ThreadBase::Sleep_ms(2);

        lT0.GetCurrentPriority();
        lT0.SetPriority(KmsLib::ThreadBase::PRIORITY_NORMAL);
        lT0.GetCurrentPriority();

        lT0.Stop();
        KMS_TEST_ASSERT(!lT0.Wait(true, 3000));
        KMS_TEST_ASSERT(lT0.Wait());

        KmsLib::ThreadBase::Sleep_ms(1);

    #endif

    lT0.SetMode(Test::MODE_EXCEPTION);
    lT0.SetPriority(KmsLib::ThreadBase::PRIORITY_HIGH);
    lT0.Start();

    KmsLib::ThreadBase::Sleep_ms(2);

    lT0.GetCurrentPriority();

    KMS_TEST_ASSERT(lT0.StopAndWait());

    lT0.SetMode(Test::MODE_NORMAL);
    lT0.SetPriority(KmsLib::ThreadBase::PRIORITY_CRITICAL);
    lT0.Start();

    KmsLib::ThreadBase::Sleep_us(2000);

    lT0.GetCurrentPriority();

    lT0.Stop();
}
KMS_TEST_END_2

// Public
/////////////////////////////////////////////////////////////////////////////

Test::Test() : mMode(MODE_NORMAL)
{
}

void Test::SetMode(Mode aMode)
{
    assert(MODE_QTY > aMode);

    assert(MODE_QTY > mMode);

    mMode = aMode;
}

// Protected
/////////////////////////////////////////////////////////////////////////////

// ===== KmsLib::ThreadBase =================================================

unsigned int Test::Run()
{
    switch (mMode)
    {
    case MODE_EXCEPTION:
        throw std::exception();

    case MODE_HANG:
        for (;;)
        {
            KmsLib::ThreadBase::Sleep_s(1);
        }
        break;

    case MODE_NORMAL:
        while (!IsStopping())
        {
            KmsLib::ThreadBase::Sleep_s(1);
        }
        break;

    default: assert(false);
    }

    return 0;
}
