
// Product  KmsBase

/// \author  KMS - Martin Dubois, ing.
/// \file    Includes/KmsLib/ValueVector.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////

// ===== C ==============================================================
#include <stdint.h>
#include <stdio.h>

// Constants
/////////////////////////////////////////////////////////////////////////

#define VALUE_VECTOR_CONSTRAINT_IGNORE   { 0, 0, 0 }
#define VALUE_VECTOR_CONSTRAINT_RESERVED { 0, 0, 1 }
#define VALUE_VECTOR_CONSTRAINT_ZERO     { 0, 0, 1 }

#define VALUE_VECTOR_DESCRIPTION_RESERVED { NULL, NULL, 0 }

#define VALUE_VECTOR_DISPLAY_FLAG_HIDE_ZERO   (0x00000001)
#define VALUE_VECTOR_DISPLAY_FLAG_NO_NEW_LINE (0x00000002)

// Macros
/////////////////////////////////////////////////////////////////////////

#define VALUE_VECTOR_CONSTRAINT_MIN_MAX(Mi, Ma) { (Ma), (Mi)      ,   1 }
#define VALUE_VECTOR_CONSTRAINT_MULTIPLE_OF(M)  {    0, 0xffffffff, (M) }
#define VALUE_VECTOR_CONSTRAINT_VALUE(V)        { (V) , (V)       ,   1 }

#define VALUE_VECTOR_DESCRIPTION(N)                         { (N) , NULL,   0 }
#define VALUE_VECTOR_DESCRIPTION_RESERVED_WITH_LEVEL(L)     { NULL, NULL, (L) }
#define VALUE_VECTOR_DESCRIPTION_WITH_LEVEL(N,L)            { (N) , NULL, (L) }
#define VALUE_VECTOR_DESCRIPTION_WITH_UNIT(N,U)             { (N) , (U) ,   0 }
#define VALUE_VECTOR_DESCRIPTION_WITH_UNIT_AND_LEVEL(N,U,L) { (N) , (U) , (L) }

#define VALUE_VECTOR_RULE(A,B,T) { { 1.0, 1.0 }, { (A), (B) }, (T) }

namespace KmsLib
{

    namespace ValueVector
    {

        // Data types
        /////////////////////////////////////////////////////////////////////

        /// \cond   en
        /// \brief  Constraint for uint32_t values
        /// \endcond
        /// \cond   fr
        /// \brief  Contrainte pour les valeurs uint32_t
        /// \endcond
        typedef struct
        {
            uint32_t mMax       ;
            uint32_t mMin       ;
            uint32_t mMultipleOf;
        }
        Constraint_UInt32;

        /// \cond   en
        /// \brief  Value description
        /// \endcond
        /// \cond   fr
        /// \brief  Description d'une valeur
        /// \endcond
        typedef struct
        {
            const char * mName ;
            const char * mUnit ;
            unsigned int mLevel;

            // TODO  KmsLib.ValueVector
            //       Ajouter un pointeur vers une fonction ToString
        }
        Description;

        /// \cond   en
        /// \brief  Rule types
        /// \endcond
        /// \cond   fr
        /// \brief  Types de regle
        /// \endcond
        typedef enum
        {
            RULE_TYPE_ABOVE         ,
            RULE_TYPE_ABOVE_OR_EQUAL,
            RULE_TYPE_BELOW         ,
            RULE_TYPE_BELOW_OR_EQUAL,
            RULE_TYPE_EQUAL         ,
            RULE_TYPE_NOT_EQUAL     ,

            RULE_TYPE_QTY
        }
        RuleType;

        /// \cond   en
        /// \brief  Rule description
        /// \endcond
        /// \cond   fr
        /// \brief  Description d'une regle
        /// \endcond
        typedef struct
        {
            double       mFactors[2];
            unsigned int mIndex  [2];
            RuleType     mType      ;
        }
        Rule;

        // Functions
        /////////////////////////////////////////////////////////////////////

        /// \cond   en
        /// \brief  Initialize a constraint
        /// \param  aConstraint [---;-W-] The constraint to initialise
        /// \endcond
        /// \cond   fr
        /// \brief  Initialiser une contrainte
        /// \param  aConstraint [---;-W-] La contrainte a initialiser
        /// \endcond
        extern void Constraint_Init(Constraint_UInt32 * aConstraint);

        /// \cond   en
        /// \brief  Initialise constraints
        /// \param  aConstraints [---;-W-] The constraints to initialise
        /// \param  aCount                 The number of constraints
        /// \endcond
        /// \cond   fr
        /// \brief  Initialiser des contraintes
        /// \param  aConstraints [---;-W-] Les contraintes a initialiser
        /// \param  aCount                 Le nombre de contrainte
        /// \endcond
        extern void Constraint_Init(Constraint_UInt32 * aConstraints, unsigned int aCount);

        /// \cond   en
        /// \brief  Verify a constrainte
        /// \param  aValue                  The value
        /// \param  aIndex                  The index of the value into the
        ///                                 vector
        /// \param  aConstraint   [---;R--] The constraint
        /// \param  aOut          [--O;RW-] The output stream
        /// \param  aDescription  [--O;R--] The value description
        /// \endcond
        /// \cond   fr
        /// \brief  Verifier une contrainte
        /// \param  aValue                  La valeur
        /// \param  aIndex                  L'indice de la valeur dans le
        ///                                 vecteur
        /// \param  aConstraint   [---;R--] La contrainte
        /// \param  aOut          [--O;RW-] Le fichier de sortie
        /// \param  aDescription  [--O;R--] La description de la valeur
        /// \endcond
        extern unsigned int Constraint_Verify(uint32_t aValue, unsigned int aIndex, const Constraint_UInt32 & aConstraint, FILE * aOut = NULL, const Description * aDescription = NULL);

        /// \cond   en
        /// \brief  Verify constraints
        /// \param  aValues                 The values
        /// \param  aCount                  The number of values and
        ///                                 constraintes
        /// \param  aConstraints  [---;R--] The constraints
        /// \param  aOut          [--O;RW-] The output stream
        /// \param  aDescriptions [--O;R--] The value description
        /// \endcond
        /// \cond   fr
        /// \brief  Verifier des contraintes
        /// \param  aValues                 Les valeurs
        /// \param  aCount                  Le nombre de valeurs et de
        ///                                 contraintes
        /// \param  aConstraints  [---;R--] Les constraintes
        /// \param  aOut          [--O;RW-] Le fichier de sortie
        /// \param  aDescriptions [--O;R--] Les descriptions des valeurs
        /// \endcond
        extern unsigned int Constraint_Verify(const uint32_t * aValues, unsigned int aCount, const Constraint_UInt32 * aConstraints, FILE * aOut = NULL, const Description * aDescriptions = NULL);

        /// \cond   en
        /// \brief  Display a value
        /// \param  aValue                  The value
        /// \param  aIndex                  The index of values in the
        ///                                 vector
        /// \param  aFlags                  See VALUE_VECTOR_DISPLAY_FLAG_...
        /// \param  aOut          [--O;RW-] The output stream
        /// \param  aDescriptions [--O;R--] The value description
        /// \param  aMinLevel               The minimum level to be displayed
        /// \endcond
        /// \cond   fr
        /// \brief  Afficher une valeur
        /// \param  aValue                  La valeur
        /// \param  aIndex                  L'indice de la valeur dans le
        ///                                 vecteur
        /// \param  aFlags                  Voir VALUE_VECTOR_DISPLAY_FLAGS_...
        /// \param  aOut          [--O;RW-] Le fichier de sortie
        /// \param  aDescriptions [--O;R--] Les descriptions des valeurs
        /// \param  aMinLevel               Le niveau minimum pour etre affiche
        /// \endcond
        extern void Display(uint32_t aValue, unsigned int aIndex, unsigned int aFlags, FILE * aOut, const Description * aDescriptions = NULL, unsigned int aMinLevel = 0);

        /// \cond   en
        /// \brief  Display values
        /// \param  aValues                 The values
        /// \param  aCount                  The number of values and
        ///                                 constraints
        /// \param  aFlags                  See VALUE_VECTOR_DISPLAY_FLAG_...
        /// \param  aOut          [--O;RW-] The output stream
        /// \param  aDescriptions [--O;R--] The value description
        /// \param  aMinLevel               The minimum level to be displayed
        /// \endcond
        /// \cond   fr
        /// \brief  Afficher des valeurs
        /// \param  aValues                 Les valeurs
        /// \param  aCount                  Le nombre de valeurs et de
        ///                                 contraintes
        /// \param  aFlags                  Voir VALUE_VECTOR_DISPLAY_FLAGS_...
        /// \param  aOut          [--O;RW-] Le fichier de sortie
        /// \param  aDescriptions [--O;R--] Les descriptions des valeurs
        /// \param  aMinLevel               Le niveau minimum pour etre affiche
        /// \endcond
        extern void Display(const uint32_t * aValues, unsigned int aCount, unsigned int aFlags, FILE * aOut, const Description * aDescriptions = NULL, unsigned int aMinLevel = 0);

        /// \cond   en
        /// \brief  Verify a rule
        /// \param  aValues                 The values
        /// \param  aCount                  The number of values
        /// \param  aRule         [---;R--] The rule
        /// \param  aIndex                  The rule index
        /// \param  aOut          [--O;RW-] The output stream
        /// \param  aDescriptions [--O;R--] The value description
        /// \endcond
        /// \cond   fr
        /// \brief  Verifier une regle
        /// \param  aValues                 Les valeurs
        /// \param  aCount                  Le nombre de valeurs
        /// \param  aRule         [---;R--] La regle
        /// \param  aIndex                  L'indice de la regle
        /// \param  aOut          [--O;RW-] Le fichier de sortie
        /// \param  aDescriptions [--O;R--] Les descriptions des valeurs
        /// \endcond
        extern unsigned int Rule_Verify(const uint32_t * aValues, unsigned int aCount, const Rule & aRule, unsigned int aIndex, FILE * aOut = NULL, const Description * aDescriptions = NULL);

        /// \cond   en
        /// \brief  Verify rules
        /// \param  aValues                 The values
        /// \param  aCount                  The number of values
        /// \param  aRules        [---;R--] The rules
        /// \param  aRuleCount              The number of rule
        /// \param  aOut          [--O;RW-] The output stream
        /// \param  aDescriptions [--O;R--] The value description
        /// \endcond
        /// \cond   fr
        /// \brief  Verifier des regles
        /// \param  aValues                 Les valeurs
        /// \param  aCount                  Le nombre de valeurs
        /// \param  aRules        [---;R--] Les regles
        /// \param  aRuleCount              Le nombre de regles
        /// \param  aOut          [--O;RW-] Le fichier de sortie
        /// \param  aDescriptions [--O;R--] Les descriptions des valeurs
        /// \endcond
        extern unsigned int Rule_Verify(const uint32_t * aValues, unsigned int aCount, const Rule * aRules, unsigned int aRuleCount, FILE * aOut = NULL, const Description * aDescriptions = NULL);

    }

}
