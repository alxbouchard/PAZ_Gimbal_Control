
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib_Test/Ring.h

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Includes ===========================================================
#include "../Includes/KmsTest.h"
#include "../Includes/Ring.h"

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Ring_Base)
{
    Ring<unsigned int, 4> lR0;

    KMS_TEST_ASSERT( lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_ASSERT(NULL != lR0.In());
    KMS_TEST_COMPARE(0, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(0, lR0.Out_GetIndex());

    lR0.In_Push(1);

    KMS_TEST_ASSERT(!lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(1, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(1, lR0.Out());
    KMS_TEST_COMPARE(0, lR0.Out_GetIndex());

    *lR0.In() = 2;

    lR0.In_Next();

    KMS_TEST_ASSERT(!lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(2, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(1, lR0.Out());
    KMS_TEST_COMPARE(0, lR0.Out_GetIndex());

    lR0.In_Push(3);

    KMS_TEST_ASSERT(!lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(3, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(1, lR0.Out());
    KMS_TEST_COMPARE(0, lR0.Out_GetIndex());

    lR0.In_Push(4);

    KMS_TEST_ASSERT(!lR0.IsEmpty());
    KMS_TEST_ASSERT( lR0.IsFull ());
    KMS_TEST_COMPARE(0, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(1, lR0.Out());
    KMS_TEST_COMPARE(0, lR0.Out_GetIndex());

    lR0.Out_Next();

    KMS_TEST_ASSERT(!lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(0, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(2, lR0.Out());
    KMS_TEST_COMPARE(1, lR0.Out_GetIndex());

    lR0.Out_Next();

    KMS_TEST_ASSERT(!lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(0, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(3, lR0.Out());
    KMS_TEST_COMPARE(2, lR0.Out_GetIndex());

    lR0.Out_Next();

    KMS_TEST_ASSERT(!lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(0, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(4, lR0.Out());
    KMS_TEST_COMPARE(3, lR0.Out_GetIndex());

    lR0.Out_Next();

    KMS_TEST_ASSERT( lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(0, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(0, lR0.Out_GetIndex());

    lR0.In_Push(5);

    KMS_TEST_ASSERT(!lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(1, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(5, lR0.Out());
    KMS_TEST_COMPARE(0, lR0.Out_GetIndex());

    lR0.Reset();

    KMS_TEST_ASSERT( lR0.IsEmpty());
    KMS_TEST_ASSERT(!lR0.IsFull ());
    KMS_TEST_COMPARE(0, lR0.In_GetIndex ());
    KMS_TEST_COMPARE(0, lR0.Out_GetIndex());
}
KMS_TEST_END
