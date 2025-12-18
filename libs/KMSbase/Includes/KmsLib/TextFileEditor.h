
// Product / Produit  KmsBase

/// \author	 KMS - Martin Dubois, ing.
/// \file	 Includes/KmsLib/TextFileEditor.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== KmsBase ============================================================
#include <KmsLib/TextFile.h>

namespace KmsLib
{

    // Class / Classe
    /////////////////////////////////////////////////////////////////////////

    // ==============              ========
    // TextFileEditor @---mRead--- TextFile
    // ==============              ========
    //       @
    //       |             ========
    //       +---mWrite--- TextFile
    //                     ========

    /// \cond   en
    /// \brief  This class provide text file edition functionnality.
    /// \endcond
    /// \cond   fr
    /// \brief  Cette classe fournis une fonctionnalite d'edition de fichier
    ///          texte.
    /// \endcond
    class TextFileEditor
    {

    public:

        /// \cond   en
        /// \brief  Constructor
        /// \endcond
        /// \cond   fr
        /// \brief  Constructeur
        /// \endcond
        TextFileEditor();

        /// \cond   en
        /// \brief  Destructor
        /// \endcond
        /// \cond   fr
        /// \brief  Destructeur
        /// \endcond
        ~TextFileEditor();

        /// \cond    en
        /// \brief   Line number accessor
        /// \return  This method returns the number of the last read line.
        /// \endcond
        /// \cond    fr
        /// \brief   Accesseur de numero de ligne
        /// \return  Cette methode retourne le numero de la derniere ligne
        ///          lue.
        /// \endcond
        unsigned int GetLineNumber_Read() const;

        /// \cond    en
        /// \brief   Line number accessor
        /// \return  This method returns the number of the last writen line.
        /// \endcond
        /// \cond    fr
        /// \brief   Accesseur de numero de ligne
        /// \return  Cette methode retourne le numero de la derniere ligne
        ///          ecrite.
        /// \endcond
        unsigned int GetLineNumber_Write() const;

        /// \cond    en
        /// \brief   State accessor
        /// \retval  false   The file is not opened
        /// \retval  true    The file is opened
        /// \endcond
        /// \cond    fr
        /// \brief   Accesseur de l'etat
        /// \retval  false   Le fichier n'est pas ouvert
        /// \retval  true    Le fichier est ouvert
        /// \endcond
        bool IsOpened() const;

        /// \cond   en
        /// \brief  Close file
        /// \endcond
        /// \cond   fr
        /// \brief  Fermer les fichier
        /// \endcond
        void Close();

        /// \cond   en
        /// \brief  Open file
        /// \param  aFileName  [---;R--]  The file name
        /// \param  aFlags                Reserved
        /// \endcond
        /// \cond   fr
        /// \brief  Ouvrir le fichier
        /// \param  aFileName  [---;R--]  Le nom du fichier
        /// \param  aFlags                Reserve
        /// \endcond
        /// \exception  Exception  CODE_IO_ERROR
        void Open(const char * aFileName, unsigned int aFlags = 0);

        /// \cond   en
        /// \brief  Read a line
        /// \param  aOut  [---;-W-]  The output buffer
        /// \param  aOutSize_byte    The output buffer size
        /// \endcond
        /// \cond   fr
        /// \brief  Lire une ligne
        /// \param  aOut  [---;-W-]  L'espace memoire de sortie
        /// \param  aOutSize_byte    La taille de l'espace memoire de
        ///                          sortie
        /// \endcond
        bool ReadLine(char * aOut, unsigned int aOutSize_byte);

        /// \cond   en
        /// \brief  Write a line
        /// \param  aIn  [---;R--]  The input buffer
        /// \endcond
        /// \cond   fr
        /// \brief  Ecrire une ligne
        /// \param  aIn  [---;R--]  L'espace memoire d'entree
        /// \endcond
        /// \exception  Exception  CODE_IO_ERROR
        void WriteLine(const char * aIn);

    private:

        TextFile mRead ;
        TextFile mWrite;

    };

}
