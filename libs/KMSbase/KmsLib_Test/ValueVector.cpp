
// Author   KMS - Martin Dubois, ing.
// Product  KmsBase
// File     KmsLib_Test/ValueVector.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== C ==================================================================
#include <stdio.h>

// ===== Includes ===========================================================
#include <KmsLib/ValueVector.h>
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

static const KmsLib::ValueVector::Description DESCRIPTIONS[] =
{
    VALUE_VECTOR_DESCRIPTION_RESERVED,
    VALUE_VECTOR_DESCRIPTION_WITH_UNIT( "Must be 1", "unit" ),
    VALUE_VECTOR_DESCRIPTION_WITH_LEVEL( "Must be multiple of 2", 1 ),
};

static const KmsLib::ValueVector::Rule RULES[] =
{
    VALUE_VECTOR_RULE(0, 2, KmsLib::ValueVector::RULE_TYPE_EQUAL         ),
    VALUE_VECTOR_RULE(0, 1, KmsLib::ValueVector::RULE_TYPE_NOT_EQUAL     ),
    VALUE_VECTOR_RULE(0, 1, KmsLib::ValueVector::RULE_TYPE_ABOVE         ),
    VALUE_VECTOR_RULE(0, 2, KmsLib::ValueVector::RULE_TYPE_ABOVE_OR_EQUAL),
    VALUE_VECTOR_RULE(1, 0, KmsLib::ValueVector::RULE_TYPE_BELOW         ),
    VALUE_VECTOR_RULE(0, 2, KmsLib::ValueVector::RULE_TYPE_BELOW_OR_EQUAL),

    VALUE_VECTOR_RULE(0, 3, KmsLib::ValueVector::RULE_TYPE_EQUAL         ),
    VALUE_VECTOR_RULE(3, 0, KmsLib::ValueVector::RULE_TYPE_EQUAL         ),
    VALUE_VECTOR_RULE(0, 1, static_cast<KmsLib::ValueVector::RuleType>(9)),
};

KMS_TEST_BEGIN(ValueVector_Base)
{
    KmsLib::ValueVector::Constraint_UInt32 lConstraints[3];

    KmsLib::ValueVector::Constraint_Init(lConstraints, sizeof(lConstraints) / sizeof(lConstraints[0]));

    KMS_TEST_COMPARE(0, lConstraints[0].mMax       );
    KMS_TEST_COMPARE(0, lConstraints[0].mMin       );
    KMS_TEST_COMPARE(1, lConstraints[0].mMultipleOf);

    lConstraints[1].mMin = 1;
    lConstraints[1].mMax = 1;

    lConstraints[2].mMax        = 0xffffffff;
    lConstraints[2].mMultipleOf = 2;

    uint32_t lValues[3];

    lValues[0] = 0;
    lValues[1] = 1;
    lValues[2] = 2;

    KMS_TEST_COMPARE(0, KmsLib::ValueVector::Constraint_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), lConstraints));
    KMS_TEST_COMPARE(0, KmsLib::ValueVector::Constraint_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), lConstraints, stdout));
    KMS_TEST_COMPARE(0, KmsLib::ValueVector::Constraint_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), lConstraints, stdout, DESCRIPTIONS));

    lValues[0] = 1;
    lValues[1] = 0;
    lValues[2] = 1;

    KMS_TEST_COMPARE(3, KmsLib::ValueVector::Constraint_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), lConstraints));
    KMS_TEST_COMPARE(3, KmsLib::ValueVector::Constraint_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), lConstraints, stdout));
    KMS_TEST_COMPARE(3, KmsLib::ValueVector::Constraint_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), lConstraints, stdout, DESCRIPTIONS));

    KmsLib::ValueVector::Display(lValues, sizeof(lValues) / sizeof(lValues[0]), VALUE_VECTOR_DISPLAY_FLAG_HIDE_ZERO, stdout);
    KmsLib::ValueVector::Display(lValues, sizeof(lValues) / sizeof(lValues[0]), VALUE_VECTOR_DISPLAY_FLAG_HIDE_ZERO, stdout, DESCRIPTIONS);

    KMS_TEST_COMPARE(3, KmsLib::ValueVector::Rule_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), RULES, sizeof(RULES) / sizeof(RULES[0])));
    KMS_TEST_COMPARE(3, KmsLib::ValueVector::Rule_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), RULES, sizeof(RULES) / sizeof(RULES[0]), stdout));
    KMS_TEST_COMPARE(3, KmsLib::ValueVector::Rule_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), RULES, sizeof(RULES) / sizeof(RULES[0]), stdout, DESCRIPTIONS));

    lValues[1] = 1;
    lValues[2] = 2;

    KMS_TEST_COMPARE(8, KmsLib::ValueVector::Rule_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), RULES, sizeof(RULES) / sizeof(RULES[0])));
    KMS_TEST_COMPARE(8, KmsLib::ValueVector::Rule_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), RULES, sizeof(RULES) / sizeof(RULES[0]), stdout));
    KMS_TEST_COMPARE(8, KmsLib::ValueVector::Rule_Verify(lValues, sizeof(lValues) / sizeof(lValues[0]), RULES, sizeof(RULES) / sizeof(RULES[0]), stdout, DESCRIPTIONS));
}
KMS_TEST_END
