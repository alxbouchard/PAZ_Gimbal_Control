
// Product / Produit  KmsBase

/// \author	 KMS - Martin Dubois, ing.
/// \file    Includes/KmsLib/TextFileEditor.h

#pragma  once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <stdio.h>

namespace KmsLib
{

    // Class / Classe
    /////////////////////////////////////////////////////////////////////////

    // ========
    // TextFile
    // ========

    /// \cond   en
    /// \brief  This class provide text file read/write	functionnality.
    /// \endcond
    /// \cond   fr
    /// \brief  Cette classe fournis une fonctionnalite de lecture/ecriture de
    ///         fichier texte.
    /// \endcond
    class TextFile
    {

    public:

        enum
        {
            FLAG_IGNORE_ERROR = 0x00000001,
        };

        /// \cond    en
        /// \brief   Find the first difference
        /// \param   aA  [---;R--]  The first file
        /// \param   aB  [---;R--]  The second file
        /// \return  This method returns 0xffffffff if the both file are
        ///          equal or the line number of the first difference
        /// \endcond
        /// \cond    fr
        /// \brief   Trouver la premiere difference
        /// \param   aA  [---;R--]  Le premier fichier
        /// \param   aB  [---;R--]  Le second fichier
        /// \return  Cette methode retourne 0xffffffff s'il n'y a pas de
        ///          difference ou le numero de la premiere ligne differente
        /// \endcond
        static unsigned int FindFirstDiff(const char * aA, const char * aB);

        /// \cond   en
        /// \brief  Constructor
        /// \endcond
        /// \cond   fr
        /// \brief  Constructeur
        /// \endcond
        TextFile();

        /// \cond   en
        /// \brief  Destructor
        /// \endcond
        /// \cond   fr
        /// \brief  Destructeur
        /// \endcond
        ~TextFile();

        /// \cond   en
        /// \brief  Cast operator
        /// \endcond
        /// \cond   fr
        /// \brief  Operateur de conversion
        /// \endcond
        operator FILE * ();

        /// \cond    en
        /// \brief   Line number accessor
        /// \return  This method returns the number of the last read or
        ///          writen line.
        /// \endcond
        /// \cond    fr
        /// \brief   Accesseur de numero de ligne
        /// \return  Cette methode retourne le numero de la derniere ligne
        ///          lue ou ecrite.
        /// \endcond
        unsigned int GetLineNumber() const;

        /// \cond    en
        /// \brief   State accessor
        /// \retval  false  The file is not opened
        /// \retval  true   The file is opened
        /// \endcond
        /// \cond    fr
        /// \brief   Accesseur de l'etat
        /// \retval  false  Le fichier n'est pas ouvert
        /// \retval  true   Le fichier est ouvert
        /// \endcond
        bool IsOpened() const;

        /// \cond   en
        /// \brief  Close the file
        /// \endcond
        /// \cond   fr
        /// \brief  Fermer le fichier
        /// \endcond
        void Close();

        /// \cond   en
        /// \brief  Create a new file
        /// \param  aFileName  [---;R--]  The file name
        /// \param  aFlags                Reserved
        /// \endcond
        /// \cond   fr
        /// \brief  Creer un nouveau fichier
        /// \param  aFileName  [---;R--]  Le nom du fichier
        /// \param  aFlags                Reserve
        /// \endcond
        void Create(const char * aFileName, unsigned int aFlags = 0);

        /// \cond   en
        /// \brief  Open a file
        /// \param  aFileName  [---;R--]  The file name
        /// \param  aFlags                See FLAG_...
        /// \endcond
        /// \cond   fr
        /// \brief  Ouvrir un fichier
        /// \param  aFileName  [---;R--]  Le nom du fichier
        /// \param  aFlags                Voir FLAG_...
        /// \endcond
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
        void  WriteLine(const char * aIn);

    private:

        FILE       * mFile      ;
        unsigned int mLineNumber;

    };
}
