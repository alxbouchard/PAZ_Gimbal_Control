
// Author   KMS - Martin Dubois, ing.
// Product  KmsBase
// File     KmsLib/ValueVector.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <KmsLib/ValueVector.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

static const char * RULE_MESSAGES[KmsLib::ValueVector::RULE_TYPE_QTY] =
{
    "Is not above"         ,
    "Is not above or equal",
    "Is not below"         ,
    "Is not below or equal",
    "Are not equal"        ,
    "Are equal"            ,
};

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Display_Rule_Error(const uint32_t * aValues, const KmsLib::ValueVector::Rule & aRule, unsigned int aIndex, const double * aOperands, FILE * aOut, const KmsLib::ValueVector::Description * aDescriptions);

namespace KmsLib
{

    namespace ValueVector
    {

        // Functions
        /////////////////////////////////////////////////////////////////////

        void Constraint_Init(Constraint_UInt32 * aConstraint)
        {
            assert(NULL != aConstraint);

            aConstraint->mMax        = 0;
            aConstraint->mMin        = 0;
            aConstraint->mMultipleOf = 1;
        }

        void Constraint_Init(Constraint_UInt32 * aConstraints, unsigned int aCount)
        {
            assert(NULL != aConstraints);
            assert(   0 <  aCount      );

            for (unsigned int i = 0; i < aCount; i++)
            {
                Constraint_Init(aConstraints + i);
            }
        }

        // NOT TESTED  KmsLib::ValueVector
        //             A douple fault to a single constraint.
        unsigned int Constraint_Verify(uint32_t aValue, unsigned int aIndex, const Constraint_UInt32 & aConstraint, FILE * aOut, const Description * aDescription)
        {
            assert(NULL != (&aConstraint));

            unsigned int lResult = 0;

            if (aConstraint.mMax < aValue)
            {
                if (NULL != aOut)
                {
                    if (0 == lResult)
                    {
                        Display(aValue, aIndex, VALUE_VECTOR_DISPLAY_FLAG_NO_NEW_LINE, aOut, aDescription);
                    }
                    fprintf(aOut, "\t<== Above %u", aConstraint.mMax);
                }
                lResult++;
            }

            if (aConstraint.mMin > aValue)
            {
                if (NULL != aOut)
                {
                    if (0 == lResult)
                    {
                        Display(aValue, aIndex, VALUE_VECTOR_DISPLAY_FLAG_NO_NEW_LINE, aOut, aDescription);
                    }
                    fprintf(aOut, "\t<== Below %u", aConstraint.mMin);
                }
                lResult++;
            }

            if (0 != (aValue % aConstraint.mMultipleOf))
            {
                if (NULL != aOut)
                {
                    if (0 == lResult)
                    {
                        Display(aValue, aIndex, VALUE_VECTOR_DISPLAY_FLAG_NO_NEW_LINE, aOut, aDescription);
                    };
                    fprintf(aOut, "\t<== Not a multiple of %u", aConstraint.mMultipleOf);
                }
                lResult++;
            }

            if ((0 < lResult) && (NULL != aOut))
            {
                fprintf(aOut, "\n");
            }

            return lResult;
        }

        unsigned int Constraint_Verify(const uint32_t * aValues, unsigned int aCount, const Constraint_UInt32 * aConstraints, FILE * aOut, const Description * aDescriptions)
        {
            assert(NULL != aValues     );
            assert(   0 <  aCount      );
            assert(NULL != aConstraints);

            unsigned int lResult = 0;

            for (unsigned int i = 0; i < aCount; i++)
            {
                lResult += Constraint_Verify(aValues[i], i, aConstraints[i], aOut, (NULL == aDescriptions) ? NULL : (aDescriptions + i));
            }

            return lResult;
        }

        void Display(uint32_t aValue, unsigned int aIndex, unsigned int aFlags, FILE * aOut, const Description * aDescription, unsigned int aMinLevel)
        {
            assert(NULL != aOut);

            if ((0 != (aFlags & VALUE_VECTOR_DISPLAY_FLAG_HIDE_ZERO)) && (0 == aValue))
            {
                return;
            }

            if (NULL == aDescription)
            {
                fprintf(aOut, "Value %u\t= %u", aIndex, aValue);

                if (0 == (aFlags & VALUE_VECTOR_DISPLAY_FLAG_NO_NEW_LINE))
                {
                    fprintf(aOut, "\n");
                }
            }
            else
            {
                if (aMinLevel <= aDescription->mLevel)
                {
                    if (NULL == aDescription->mName)
                    {
                        fprintf(aOut, "Reserved %u\t= %u", aIndex, aValue);
                    }
                    else
                    {
                        fprintf(aOut, "%s\t= %u", aDescription->mName, aValue);
                    }

                    if (NULL != aDescription->mUnit)
                    {
                        fprintf(aOut, " %s", aDescription->mUnit);
                    }

                    if (0 == (aFlags & VALUE_VECTOR_DISPLAY_FLAG_NO_NEW_LINE))
                    {
                        fprintf(aOut, "\n");
                    }
                }
            }
        }

        void Display(const uint32_t * aValues, unsigned int aCount, unsigned int aFlags, FILE * aOut, const Description * aDescriptions, unsigned int aMinLevel)
        {
            assert(NULL != aValues);
            assert(   0 <  aCount );
            assert(NULL != aOut   );

            for (unsigned int i = 0; i < aCount; i++)
            {
                Display(aValues[i], i, aFlags, aOut, (NULL == aDescriptions) ? NULL : (aDescriptions + i), aMinLevel);
            }
        }

        unsigned int Rule_Verify(const uint32_t * aValues, unsigned int aCount, const Rule & aRule, unsigned int aIndex, FILE * aOut, const Description * aDescriptions)
        {
            assert(NULL != aValues );
            assert(   0 <  aCount  );
            assert(NULL != (&aRule));

            double lValues[2];

            for (unsigned int i = 0; i < 2; i++)
            {
                if ((0.0 == aRule.mFactors[i]) || (aCount <= aRule.mIndex[i]))
                {
                    if (NULL != aOut)
                    {
                        fprintf(aOut, "Rule %u : Invalid rule parameter %u\n", aIndex, i);
                    }

                    return 1;
                }

                lValues[i] = aRule.mFactors[i] * aValues[aRule.mIndex[i]];
            }

            switch (aRule.mType)
            {
            case RULE_TYPE_ABOVE         : if (lValues[0] >  lValues[1]) { return 0; } break;
            case RULE_TYPE_ABOVE_OR_EQUAL: if (lValues[0] >= lValues[1]) { return 0; } break;
            case RULE_TYPE_BELOW         : if (lValues[0] <  lValues[1]) { return 0; } break;
            case RULE_TYPE_BELOW_OR_EQUAL: if (lValues[0] <= lValues[1]) { return 0; } break;
            case RULE_TYPE_EQUAL         : if (lValues[0] == lValues[1]) { return 0; } break;
            case RULE_TYPE_NOT_EQUAL     : if (lValues[0] != lValues[1]) { return 0; } break;

            default:
                if (NULL != aOut)
                {
                    fprintf(aOut, "Rule %u : Invalid rule type\n", aIndex);
                }
                return 1;
            }

            Display_Rule_Error(aValues, aRule, aIndex, lValues, aOut, aDescriptions);

            return 1;
        }

        unsigned int Rule_Verify(const uint32_t * aValues, unsigned int aCount, const Rule * aRules, unsigned int aRuleCount, FILE * aOut, const Description * aDescriptions)
        {
            assert(NULL != aValues   );
            assert(   0 <  aCount    );
            assert(NULL != aRules    );
            assert(   0 <  aRuleCount);

            unsigned int lResult = 0;

            for (unsigned int i = 0; i < aRuleCount; i++)
            {
                lResult += Rule_Verify(aValues, aCount, aRules[i], i, aOut, aDescriptions);
            }

            return lResult;
        }

    }

}

// Static functions
/////////////////////////////////////////////////////////////////////////////

void Display_Rule_Error(const uint32_t * aValues, const KmsLib::ValueVector::Rule & aRule, unsigned int aIndex, const double * aOperands, FILE * aOut, const KmsLib::ValueVector::Description * aDescriptions)
{
    assert(NULL                               != aValues    );
    assert(NULL                               != (&aRule)   );
    assert(KmsLib::ValueVector::RULE_TYPE_QTY >  aRule.mType);
    assert(NULL                               != aOperands  );

    if (NULL != aOut)
    {
        fprintf(aOut, "Rule %u : %s\n", aIndex, RULE_MESSAGES[aRule.mType]);

        for (unsigned int i = 0; i < 2; i++)
        {
            fprintf(aOut, "  ");
            KmsLib::ValueVector::Display(aValues[aRule.mIndex[i]], aRule.mIndex[i], VALUE_VECTOR_DISPLAY_FLAG_NO_NEW_LINE, aOut, (NULL == aDescriptions) ? NULL : (aDescriptions + i));
            fprintf(aOut, "\t==> %f\n", aOperands[i]);
        }
    }
}
