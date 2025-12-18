
// Product / Produit   KmsBase

/// \author   KMS - Martin Dubois, ing.
/// \file     Includes/KmsLib/File.h

#pragma once

// Constants / Constantes
/////////////////////////////////////////////////////////////////////////////

#define  FILE_COPY_FLAG_FAIL_IF_EXIST  (0x00000001)

#define  FILE_EXIST_FLAG_READ          (0x00000001)

#define  FILE_MOVE_FLAG_FAIL_IF_EXIST  (0x00000001)

namespace  KmsLib
{

    namespace  File
    {

        // Functions / Fonctions
        /////////////////////////////////////////////////////////////////////

        /// \cond        en
        /// \brief       Copy a file
        /// \param       aSrc   [---;R--]   Source file
        /// \param       aDst   [---;R--]   Destination file
        /// \endcond
        /// \cond        fr
        /// \brief       Copier un fichier
        /// \param       aSrc   [---;R--]   Fichier source
        /// \param       aDst   [---;R--]   Fichier destination
        /// \endcond
        /// \param       aFlags      FILE_COPY_FLAG_FAIL_IF_EXIST
        /// \exception   Exception   CODE_COPY_FILE_ERROR
        void  Copy(const  char  * aSrc, const  char  * aDst, unsigned int  aFlags = 0);

        /// \cond        en
        /// \brief       Verify if a file exist
        /// \param       aName   [---;R--]   The file name
        /// \param       aFlags              Reserved
        /// \endcond
        /// \cond        fr
        /// \brief       Comparer 2 fichier
        /// \param       aName   [---;R--]   Le nom du fichier
        /// \param       aFlags              Reserve
        /// \endcond
        /// \exception   Exception   CODE_DELETE_FILE_ERROR
        void  Delete(const  char  * aName, unsigned int  aFlags  = 0);

        /// \cond        en
        /// \brief       Verify if a file exist
        /// \param       aName   [---;R--]   The file name
        /// \param       aFlags              Reserved
        /// \retval      false   The file does not exist
        /// \retval      true    The file exists
        /// \endcond
        /// \cond        fr
        /// \brief       Comparer 2 fichier
        /// \param       aName   [---;R--]   Le nom du fichier
        /// \param       aFlags              Reserve
        /// \retval      false   Le fichier n'existe pas
        /// \retval      true    Le fichier exist
        /// \endcond
        /// \exception   Exception   CODE_INVALID_PARAMETER
        bool  Exist(const char  * aName, unsigned int  aFlags = 0);

        /// \cond     en
        /// \brief    Compare 2 files
        /// \param    aA   [---;R--]   First file
        /// \param    aB   [---;R--]   Second file
        /// \param    aFlags           Reserved
        /// \retval   false   The files are not equal
        /// \retval   true    The files are equal
        /// \endcond
        /// \cond     fr
        /// \brief    Comparer 2 fichier
        /// \param    aA   [---;R--]   Premier fichier
        /// \param    aB   [---;R--]   Second fichier
        /// \param    aFlags           Reserve
        /// \retval   false   Les fichiers ne sont pas egal
        /// \retval   true    Les fichiers son egal
        /// \endcond
        bool  IsEqual(const  char  * aA, const  char  * aB, unsigned int  aFlags = 0);

        /// \cond        en
        /// \brief       Move a file
        /// \param       aSrc   [---;R--]   Source file
        /// \param       aDst   [---;R--]   Destination file
        /// \endcond
        /// \cond        fr
        /// \brief       Deplacer un fichier
        /// \param       aSrc   [---;R--]   Fichier source
        /// \param       aDst   [---;R--]   Fichier destination
        /// \endcond
        /// \param       aFlags      FILE_MOVE_FLAG_FAIL_IF_EXIST
        /// \exception   Exception   CODE_MOVE_FILE_ERROR
        void  Move(const  char  * aSrc, const  char  * aDst, unsigned int  aFlags = 0);

    }

}
