
// Author / Auteur    KMS - Martin Dubois, ing.
// Product / Produit  KmsBase
// File / Fichier     KmsLib/String.cpp

// Last test coverage update / Derniere mise a jour de la couverture de test
// 2017-11-10

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include  <assert.h>
#include  <string.h>

// ===== Includes ===========================================================
#include  <KmsBase.h>

// ----- Includes/KmsLib ----------------------------------------------------
#include  <KmsLib/String.h>

namespace KmsLib
{

    namespace String
    {

        // Functions / Fonctions
        /////////////////////////////////////////////////////////////////////

        bool IsPatern(const char * aPatern)
        {
            assert(NULL != aPatern);

            if (NULL != strchr(aPatern, '*'))
            {
                return true;
            }

            if (NULL != strchr(aPatern, '?'))
            {
                return true;
            }

            return false;
        }

        bool MatchPatern(const char * aPatern, const char * aToMatch)
        {
            assert(NULL != aPatern );
            assert(NULL != aToMatch);

            for (;;)
            {
                switch (*aPatern)
                {
                case '\0':
                    return ('\0' == (*aToMatch));

                case '?':
                    if ('\0' == (*aToMatch))
                    {
                        return false;
                    }

                    aPatern  ++;
                    aToMatch ++;
                    break;

                case '*':
                    if ('\0' == (*aToMatch))
                    {
                        aPatern++;
                    }
                    else
                    {
                        if (MatchPatern(aPatern + 1, aToMatch))
                        {
                            return true;
                        }

                        aToMatch++;
                    }
                    break;

                default:
                    if ((*aPatern) != (*aToMatch))
                    {
                        return false;
                    }

                    aPatern  ++;
                    aToMatch ++;
                }
            }

            // NOT TESTED  DefensiveCode
            //             This return statement is there to avoid the warning / Ce
            //			   return est seulement la pour eviter le warning.
            return false;
        }

        void ReplaceSlash(char * aInOut)
        {
            assert(NULL != aInOut);

#if ( '/' != SLASH_C )

            char * lPtr = aInOut;

            while ('\0' != (*lPtr))
            {
                if ('/' == (*lPtr))
                {
                    (*lPtr) = SLASH_C;
                }

                lPtr++;
            }

#endif

        }

        void Trunk(char * aInOut, char aC)
        {
            assert(NULL != aInOut);
            assert('\0' != aC    );

            char  * lPtr = strchr(aInOut, aC);
            if (NULL != lPtr)
            {
                (*lPtr) = '\0';
            }
        }

        void Trunk(char * aInOut, const char * aC)
        {
            assert(NULL != aInOut);
            assert(NULL != aC    );

            size_t  lIndex = strcspn(aInOut, aC);
            aInOut[lIndex] = '\0';
        }

    }

}
