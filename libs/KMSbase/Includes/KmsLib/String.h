
// Product / Produit  KmsBase

/// \author	 KMS - Martin Dubois, ing.
/// \file	 Includes/KmsLib/String.h

#pragma  once

namespace KmsLib
{

    namespace String
    {

        // Functions / Fonctions
        /////////////////////////////////////////////////////////////////////

        /// \cond    en
        /// \brief   Is the string a patern
        /// \param   aLine  [---;R--]  The string
        /// \retval  false  The string is not a patern
        /// \retval  true   The string is a patern
        /// \endcond
        /// \cond    fr
        /// \brief   La chaine est-elle un patron
        /// \param   aLine  [---;R--]  La chaine
        /// \retval  false  La chaine n'est pas un patron
        /// \retval  true   La chaine est un patron
        /// \endcond
        bool IsPatern(const char * aLine);

        /// \cond    en
        /// \brief   Is the string match the patern
        /// \param   aPatern   [---;R--]  The patern
        /// \param   aToMatch  [---;R--]  The string
        /// \retval  false  The string does not match the pattern
        /// \retval  true   The string matches the pattern
        /// \endcond
        /// \cond    fr
        /// \brief   La chaine respect-elle le patron
        /// \param   aPatern   [---;R--]  Le patron
        /// \param   aToMatch  [---;R--]  La chaine
        /// \retval  false  La chaine ne respecte pas le patron
        /// \retval  true   La chaine respecte le patron
        /// \endcond
        bool MatchPatern(const char * aPatern, const char * aToMatch);

        /// \cond    en
        /// \brief   Replace the /
        /// \param   aInOut   [---;RW-]   String
        /// \endcond
        /// \cond    fr
        /// \brief   Remplacer les /
        /// \param   aInOut   [---;RW-]   Chaine
        /// \endcond
        void ReplaceSlash(char * aInOut);

        /// \cond    en
        /// \brief   Trunk a string
        /// \param   aInOut  [---;RW-]  String to trunk
        /// \param   aC                 The character indicating where to
        ///                             trunk the string
        /// \endcond
        /// \cond    fr
        /// \brief   Couper une chaine de caracteres
        /// \param   aInOut  [---;RW-]  Chaine a couper
        /// \param   aC                 Le caractere indiquant ou couper
        ///                             la chaine
        /// \endcond
        void Trunk(char * aInOut, char aC);

        /// \cond    en
        /// \brief   Trunk a string
        /// \param   aInOut  [---;RW-]  String to trunk
        /// \param   aC                 The characters indicating where to
        ///                             trunk the string
        /// \endcond
        /// \cond    fr
        /// \brief   Couper une chaine de caracteres
        /// \param   aInOut  [---;RW-]  Chaine a couper
        /// \param   aC                 Les caracteres indiquant ou couper
        ///                             la chaine
        /// \endcond
        void  Trunk(char * aInOut, const char * aC);

    }

}
