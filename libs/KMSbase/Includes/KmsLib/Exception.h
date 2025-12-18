
// Product  KmsBase

/// \author  KMS - Martin Dubois, ing.
/// \file    Includes/KmsLib/Exception.h

// CODE COVERAGE  2019-07-22  KMS - Martin Dubois, ing.

// CODE REVIEW    2019-07-22  KMS - Martin Dubois, ing.

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <stdio.h>

// ===== C++ ================================================================
#include <exception>
#include <string>

// ===== Windows ============================================================
#ifdef GetMessage
    #undef GetMessage
#endif

namespace KmsLib
{

    // Class
    /////////////////////////////////////////////////////////////////////////

    /// \cond    en
    /// \brief   Contains information about an exception
    /// \endcond
    /// \cond    fr
    /// \brief   Contient les informations au sujet d'un exception
    /// \endcond
    class Exception : public std::exception
    {

    public:

        /// \cond   en
        /// \brief  Exception code
        /// \endcond
        /// \cond   fr
        /// \brief  Code de l'exception
        /// \endcond
        /// \sa     GetCode, GetCodeName
        typedef enum
        {
            CODE_IO_ERROR      = 0,
            CODE_IOCTL_ERROR      ,
            CODE_NO_SUCH_DEVICE   ,
            CODE_NOT_ENOUGH_MEMORY,
            CODE_PRIVATE          ,
            CODE_REGISTRY_ERROR   , // 5
            CODE_SETUP_API_ERROR  ,
            CODE_UNKNOWN          ,
            CODE_USER_ERROR       ,

            CODE_AUTHENTIFICATION_ERROR,
            CODE_COMMUNICATION_ERROR   , // 10
            CODE_GUI_ERROR             ,
            CODE_HARDWARE_ERROR        ,
            CODE_INSTALLATION_ERROR    ,
            CODE_INVALID_ARGUMENT      ,
            CODE_INVALID_BUFFER_ADDRESS, // 15
            CODE_INVALID_BUFFER_SIZE   ,
            CODE_INVALID_DATA          ,
            CODE_INVALID_DATE          ,
            CODE_INVALID_HANDLE        ,
            CODE_INVALID_TIME          , // 20
            CODE_NETWORK_ERROR         ,
            CODE_NOT_FOUND             ,
            CODE_OVERFLOW              ,
            CODE_PERMISSION_DENIED     ,
            CODE_PROTOCOL_ERROR        , // 25
            CODE_STATE_ERROR           ,
            CODE_SYSTEM_ERROR          ,
            CODE_THREAD_ERROR          ,
            CODE_TIMEOUT               ,
            CODE_UNDERFLOW             , // 30

            CODE_SERVICE_MANAGER_ERROR,
            CODE_SYSTEM_LOG_ERROR     ,

            CODE_ACCESS_VIOLATION      ,
            CODE_INTEGER_DIVIDE_BY_ZERO,
            CODE_INVALID_COMMAND_LINE  , // 35
            CODE_OPEN_CL_ERROR         ,
            CODE_STRUCTURED_EXCEPTION  ,

            CODE_CLOSE_HANDLE_ERROR ,
            CODE_BUFFER_TOO_SMALL   ,
            CODE_COM_PORT_ERROR     , // 40
            CODE_COPY_FILE_ERROR    ,
            CODE_CREATE_FILE_ERROR  ,
            CODE_DELETE_FILE_ERROR  ,
            CODE_DENIED             ,
            CODE_DIRECTORY_ERROR    , // 45
            CODE_ERROR              ,
            CODE_FILE_OPEN_ERROR    ,
            CODE_FILE_READ_ERROR    ,
            CODE_FILE_WRITE_ERROR   ,
            CODE_INVALID_ADDRESS    , // 50
            CODE_INVALID_PARAMETER  ,
            CODE_INVALID_PORT_NUMBER,
            CODE_IO_CANCEL_ERROR    ,
            CODE_MOVE_FILE_ERROR    ,
            CODE_NO_COM_PORT        , // 55
            CODE_NOT_IMPLEMENTED    ,
            CODE_READ_FILE_ERROR    ,
            CODE_REG_CLOSE_ERROR    ,
            CODE_REG_CREATE_ERROR   ,
            CODE_REG_DELETE_ERROR   , // 60
            CODE_REG_OPEN_ERROR     ,
            CODE_REG_QUERY_ERROR    ,
            CODE_REG_SET_ERROR      ,
            CODE_SOCKET_ERROR       ,
            CODE_TO_DEFINE          , // 65
            CODE_UNSUCCESSFUL       ,
            CODE_WINSOCK_ERROR      ,
            CODE_WRITE_FILE_ERROR   , // 68

            CODE_QTY,

            CODE_BASE = 65536
        }
        Code;

        /// \cond    en
        /// \brief   Register a structured exception translator
        /// \return  This method returns the previous translater
        /// \endcond
        /// \cond    fr
        /// \brief   Enregistre un traducteur d'exceptions structur&eacute;s
        /// \return  Le traducteur pr&eacute;c&eacutedant
        /// \endcond
        /// \sa      RestoreTranslator
        static void * RegisterTranslator();

        /// \cond   en
        /// \brief  Restore a structured exception handler
        /// \param  aTranslator  Value previously returned by
        ///                      RegisterTranslator
        /// \endcond
        /// \cond   fr
        /// \brief  R;eacute;active un traducteur d'exceptions
        ///         structur&eacute;s
        /// \param  aTranslator  Valeur pr&eacute;c&eacute;dement
        ///                      retoutn&eacute;e par RegisterTranslator
        /// \endcond
        /// \sa     RegisterTranslator
        static void RestoreTranslator(void * aTranslator);

        /// \cond    en
        /// \brief   Define a new exception code
        /// \param   aIndex  Code index
        /// \param   aName   Code name
        /// \return  This method returns the defined code
        /// \endcond
        /// \cond    fr
        /// \brief   D&eacute;finir un nouveau code
        /// \param   aIndex  Indice du code
        /// \param   aName   Nom du code
        /// \return  Cette m&eacute;thode retourne le nouveau code
        /// \endcond
        /// \sa      GetCodeName
        static Code DefineCode(unsigned int aIndex, const char * aName = NULL);

        /// \cond    en
        /// \brief   Get the name of a code
        /// \param   aCode  The code
        /// \return  This method returns a constant address or NULL if the
        ///          code is not valid
        /// \endcond
        /// \cond    fr
        /// \brief	 Obtenir le nom d'un code
        /// \param   aCode  Le code
        /// \return  Cette m&eacute;thode retourne l'adresse d'une constante
        ///          ou NULL si le code n'est pas valid
        /// \endcond
        /// \sa      DefineCode, SetCodeName
        static const char * GetCodeName(Code aCode);

        /// \cond    en
        /// \brief   Set the name of a code
        /// \param   aCode  The code
        /// \param   aName  The name
        /// \endcond
        /// \cond    fr
        /// \brief	 Changer le nom d'un code
        /// \param   aCode  Le code
        /// \param   aName  Le nom
        /// \endcond
        /// \sa      DefineCode, GetCodeName
        static void SetCodeName(Code aCode, const char * aName);

        /// \cond   en
        /// \brief  Constructor
        /// \param  aCode      See Code
        /// \param  aWhat      Simple message
        /// \param  aMessage   Message to be copied into the exception
        /// \param  aFile      Source file name
        /// \param  aFunction  Function name
        /// \param  aLine      Line number
        /// \param  aInfoA     Information that depends on the exception
        /// \endcond
        /// \cond   fr
        /// \brief  Constructeur
        /// \param  aCode      Voir Code
        /// \param  aWhat      Message simple
        /// \param  aMessage   Message &agrave; &ecirc;tre copi&eacute; dans
        ///                    l'exception
        /// \param  aFile      Nom du fichier sources
        /// \param  aFunction  Nom de la fonction
        /// \param  aLine      Num&eacute;ro de la ligne dans le fichier
        ///                    source
        /// \param  aInfoA     Information qui d&eacute;pend de l'exception
        /// \endcond
        Exception(Code aCode, const char * aWhat, const char * aMessage, const char * aFile, const char * aFunction, unsigned int aLine, unsigned int aInfoA);

        /// \cond	 en
        /// \brief	 Returns the exception code
        /// \return  See CODE_...
        /// \endcond
        /// \cond	 fr
        /// \brief	 Retourne le code de l'exception
        /// \return  Voir CODE_...
        /// \endcond
        /// \sa      Code, GetCodeNAme
        Code GetCode() const;

        /// \cond    en
        /// \brief   Returns the source file name
        /// \return  The source file name
        /// \endcond
        /// \cond    fr
        /// \brief   Retourne le nom du fichier source
        /// \return  Le nom du fichier source
        /// \endcond
        /// \sa      GetLine
        const char * GetFile() const;

        /// \cond    en
        /// \brief   Returns the function name
        /// \return  The function name
        /// \endcond
        /// \cond    fr
        /// \brief   Retourne le nom de la fonction
        /// \return  Le nom de la fonction
        /// \endcond
        const char * GetFunction() const;

        /// \cond    en
        /// \brief   Returns the information A
        /// \return  The information A
        /// \endcond
        /// \cond    fr
        /// \brief   Retourne l'information A
        /// \return  L'information A
        /// \endcond
        unsigned int GetInfoA() const;

        /// \cond    en
        /// \brief   Returns the last error information
        /// \return  The last error information
        /// \endcond
        /// \cond    fr
        /// \brief   Retourne l'information sur la derniere erreur
        /// \return  L'information sur la derniere erreur
        /// \endcond
        unsigned int GetLastError() const;

        /// \cond    en
        /// \brief   Returns the line number
        /// \return  The line number
        /// \endcond
        /// \cond    fr
        /// \brief   Retourne le num&eacute;ro de la ligne dans le fichier
        ///          source
        ///	\return  Le num&eacute;ro de la ligne dans le fichier source
        /// \endcond
        /// \sa      GetFile
        unsigned int GetLine() const;

        /// \cond    en
        /// \brief   Returns the error message
        /// \return  A pointer to an internal buffer
        /// \endcond
        /// \cond    fr
        /// \brief   Retourne le message d'erreur
        ///	\return  Un pointeur vers un espace m&eacute;moire interne
        /// \endcond
        const char * GetMessage() const;

        /// \cond   en
        /// \brief  Write the exception into a stream
        /// \param  aFile  The stream to write to
        /// \endcond
        /// \cond   fr
        /// \brief  Ecrit l'exception dans un fichier
        ///	\param  aFile  Le fichier dans lequel &eacute;crire
        /// \endcond
        void Write(FILE * aFile) const;

        // ===== std::exception ========================================

        virtual ~Exception() throw ();

        virtual const char * what() const throw ();

    private:

        Code         mCode     ;
        const char * mFile     ;
        const char * mFunction ;
        unsigned int mInfoA    ;
        unsigned int mLastError;
        unsigned int mLine     ;
        std::string  mMessage  ;
        const char * mWhat     ;

    };

    // Public
    /////////////////////////////////////////////////////////////////////////

    inline Exception::Code Exception::GetCode() const
    {
        return mCode;
    }

    inline unsigned int Exception::GetInfoA() const
    {
        return mInfoA;
    }

    inline unsigned int Exception::GetLastError() const
    {
        return mLastError;
    }

    inline const char * Exception::GetMessage() const
    {
        return (0 < mMessage.size()) ? mMessage.c_str() : what();
    }

}
