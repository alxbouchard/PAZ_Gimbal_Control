
// License http ://www.apache.org/licenses/LICENSE-2.0
// Product KmsBase

/// \author    KMS - Martin Dubois, ing.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/KmsLib/ToolBase.h
/// \brief     ToolBase

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== KmsBase =============================================================
#include <KmsLib/Exception.h>

// Macro
/////////////////////////////////////////////////////////////////////////////

#define KMS_LIB_TOOL_BASE_FUNCTIONS                                                                                       \
    { "Abort"        , KmsLib::ToolBase::Abort        , "Abort {Code} [Desc]"       , NULL                             }, \
    { "ChangeDir"    , KmsLib::ToolBase::ChangeDir    , "ChangeDir {Directory}"     , NULL                             }, \
    { "Delay"        , KmsLib::ToolBase::Delay        , "Delay [Delay_ms]"          , NULL                             }, \
    { "Echo"         , KmsLib::ToolBase::Echo         , "Echo {Message}"            , NULL                             }, \
    { "Error"        , NULL                           , "Error ..."                 , KmsLib::ToolBase::ERROR_COMMANDS }, \
    { "ExecuteScript", KmsLib::ToolBase::ExecuteScript, "ExecuteScript {ScriptName}", NULL                             }, \
    { "Exit"         , KmsLib::ToolBase::Exit         , "Exit {Code} [Desc]"        , NULL                             }, \
    { "Pause"        , KmsLib::ToolBase::Pause        , "Pause {Message}"           , NULL                             }, \
    { "Repeat"       , KmsLib::ToolBase::Repeat       , "Repeat {Count} {Command}"  , NULL                             },

#define KMS_LIB_TOOL_BASE_HANDLER(N) static void N(KmsLib::ToolBase * aToolBase, const char * aArg)

namespace KmsLib
{

    // Class
    /////////////////////////////////////////////////////////////////////////

    /// \cond   en
    /// \brief  Command line tools base
    /// \endcond
    /// \cond   fr
    /// \brief  Base d'un outil en ligne de commande
    /// \endcond
    class ToolBase
    {

    public:

        /// \cond   en
        /// \brief  Type of reported information
        /// \endcond
        /// \cond   fr
        /// \brief  Type des informations raport&eacute;es
        /// \endcond
        typedef enum
        {
            REPORT_ERROR      ,
            REPORT_FATAL_ERROR,
            REPORT_INFO       ,
            REPORT_OK         ,
            REPORT_USER_ERROR ,
            REPORT_WARNING    ,

            REPORT_QTY
        }
        ReportType;

        /// \cond   en
        /// \brief  Type of function
        /// \endcond
        /// \cond   fr
        /// \brief  Type des fonctions
        /// \endcond
        typedef void(*Function)(ToolBase * aToolBase, const char * aArg);

        /// \cond   en
        /// \brief  Describe a command
        /// \endcond
        /// \cond   fr
        /// \brief  Decrit une commande
        /// \endcond
        typedef struct CommandInfo
        {
            const char               * mName       ;
            Function                   mFunction   ;
            const char               * mHelpLine   ;
            const struct CommandInfo * mSubCommands;
        }
        CommandInfo;

        // TODO KmsLib.ToolBase
        //      Low (Feature) - Make AskUser normal method and use the
        //      current stream for input.

        /// \cond   en
        /// \brief  Ask the user for a unsigned short value
        /// \param  aFile     The input stream
        /// \param  aName     Name of the requested value
        /// \param  aMin      Minimal value
        /// \param  aMax      Maximal value
        /// \param  aDefault  Default value
        /// \param  aOut     The value the user entered
        /// \endcond
        /// \cond   fr
        /// \brief  Demander une valeur unsigned short &agrave; l'utilisateur
        /// \param  aFile     Le fichier d'entr&eacute;e
        /// \param  aName     Nom de la valeur demand&eacute;e
        /// \param  aMin      Valeur minimale
        /// \param  aMax      Valeur maximale
        /// \param  aDefault  Valeur par d&eacute;faut
        /// \param  aOut      La valeur que l'utilisateur &agrave;
        ///                   entr&eacute;e
        /// \endcond
        /// \exception  Exception  CODE_FILE_READ_ERROR
        static void AskUser(FILE * aFile, const char * aName, unsigned short aMin, unsigned short aMax, unsigned short aDefault, unsigned short * aOut);

        /// \cond   en
        /// \brief  Ask the user for a string value
        /// \param  aFile          The input stream
        /// \param  aName          Name of the requested value
        /// \param  aOut           The value the user entered
        /// \param  aOutSize_byte  The value maximal size
        /// \endcond
        /// \cond   fr
        /// \brief  Demander une chaine de caracteres &agrave; l'utilisateur
        /// \param  aFile          Le fichier d'entr&eacute;e
        /// \param  aName          Nom de la valeur demand&eacute;e
        /// \param  aOut           La valeur que l'utilisateur &agrave;
        ///                        entr&eacute;e
        /// \param  aOutSize_byte  La taille maximale de la valeur
        /// \endcond
        /// \exception  Exception  CODE_FILE_READ_ERROR
        static void AskUser(FILE * aFile, const char * aName, char * aOut, unsigned int aOutSize_byte);

        /// \cond	en
        /// \brief  Ask the user for a string value
        /// \param  aFile          The input stream
        /// \param  aName          Name of the requested value
        /// \param  aDefault       Default value
        /// \param  aOut           The value the user entered
        /// \param	aOutSize_byte  The value maximal size
        /// \endcond
        /// \cond   fr
        /// \brief  Demander une chaine de caracteres &agrave; l'utilisateur
        /// \param	aFile          Le fichier d'entr&eacute;e
        /// \param  aName          Nom de la valeur demand&eacute;e
        /// \param  aDefault       Valeur par d&eacute;faut
        /// \param  aOut           La valeur que l'utilisateur &agrave;
        ///                        entr&eacute;e
        /// \param	aOutSize_byte  La taille maximale de la valeur
        /// \endcond
        /// \exception  Exception  CODE_FILE_READ_ERROR
        static void AskUser(FILE * aFile, const char * aName, const char * aDefault, char * aOut, unsigned int aOutSize_byte);

        /// \cond	en
        /// \brief  Ask the user for an input file name
        /// \param  aFile          The input stream
        /// \param  aName          Name of the requested value
        /// \param	aOut           The value the user entered
        /// \param  aOutSize_byte  The value maximal size
        /// \endcond
        /// \cond   fr
        /// \brief  Demander le nom d'un fichier d'entree &agrave; utilisateur
        /// \param  aFile         Le fichier d'entr&eacute;e
        /// \param  aName         Nom de la valeur demand&eacute;e
        /// \param  aOut          La valeur que l'utilisateur &agrave;
        ///                       entr&eacute;e
        /// \param aOutSize_byte  La taille maximale de la valeur
        /// \endcond
        /// \exception  Exception  CODE_FILE_READ_ERROR
        static void AskUser_InputFileName(FILE * aFile, const char * aName, char * aOut, unsigned int  aOutSize_byte);

        /// \cond   en
        /// \brief  Ask the user for an output file name
        /// \param  aFile          The input stream
        /// \param  aName          Name of the requested value
        /// \param  aDefault       Default value
        /// \param  aOut           The value the user entered
        /// \param  aOutSize_byte  The value maximal size
        /// \endcond
        /// \cond   fr
        /// \brief  Demander le nom d'un fichier de sortie &agrave;
        ///         l'utilisateur
        /// \param  aFile          Le fichier d'entr&eacute;e
        /// \param  aName          Nom de la valeur demand&eacute;e
        /// \param  aDefault       Valeur par d&eacute;faut
        /// \param  aOut           La valeur que l'utilisateur &agrave;
        ///                        entr&eacute;e
        /// \param  aOutSize_byte  La taille maximale de la valeur
        /// \endcond
        /// \exception  Exception  CODE_FILE_READ_ERROR
        static void AskUser_OutputFileName(FILE * aFile, const char * aName, const char * aDefault, char * aOut, unsigned int aOutSize_byte);

        /// \cond   en
        /// \brief  Report a result
        /// \param  aType  The report type
        /// \endcond
        /// \cond   fr
        /// \brief  Afficher un resultat
        /// \param  aType  Le type de rapport
        /// \endcond
        static void Report(ReportType aType);

        /// \cond   en
        /// \brief  Report an exception
        /// \param  aType       The report type
        /// \param  aException  The exception
        /// \endcond
        /// \cond   fr
        /// \brief  Afficher une exception
        /// \param  aType       Le type de rapport
        /// \param  aException  L'exception
        /// \endcond
        static void Report(ReportType aType, const KmsLib::Exception * aException);

        /// \cond   en
        /// \brief  Report an error
        /// \param  aType     The report type
        /// \param  aMessage  A simple message
        /// \endcond
        /// \cond   fr
        /// \brief  Afficher une erreur
        /// \param  aType     Le type de rapport
        /// \param  aMessage  Un message simple
        /// \endcond
        static void Report(ReportType aType, const char * aMessage);

        // ===== Function ===================================================

        static const CommandInfo ERROR_COMMANDS[];

        // [ErrorCode] [ErrorDescription]
        KMS_LIB_TOOL_BASE_HANDLER(Abort);

        // {Directory}
        KMS_LIB_TOOL_BASE_HANDLER(ChangeDir);

        // [Delay_ms]
        KMS_LIB_TOOL_BASE_HANDLER(Delay);

        // {Message}
        KMS_LIB_TOOL_BASE_HANDLER(Echo);

        KMS_LIB_TOOL_BASE_HANDLER(Error_Abort);
        KMS_LIB_TOOL_BASE_HANDLER(Error_Clear);
        KMS_LIB_TOOL_BASE_HANDLER(Error_Display);
        KMS_LIB_TOOL_BASE_HANDLER(Error_Exit);

        // {FileName}
        KMS_LIB_TOOL_BASE_HANDLER(ExecuteScript);

        // [ErrorCode] [ErrorDescription]
        KMS_LIB_TOOL_BASE_HANDLER(Exit);

        // {Message}
        KMS_LIB_TOOL_BASE_HANDLER(Pause);

        // {RepeatCount} {Command}
        KMS_LIB_TOOL_BASE_HANDLER(Repeat);

        /// \cond   en
        /// \brief  Constructor
        /// \param  aCommands  [-K-;R--]  The table describing the
        ///                               commands
        /// \endcond
        /// \cond   fr
        /// \brief  Contructor
        ///	\param  aCommands  [-K-;R--]  Le tableau definissant les
        ///                               commandes
        /// \endcond
        ToolBase(const CommandInfo * aCommands);

        /// \cond   en
        /// \brief  Clear the error code
        /// \endcond
        /// \cond   fr
        /// \brief  Effacer le code d'erreur
        /// \endcond
        void ClearError();

        /// \cond   en
        /// \brief  Retrieve the error code
        /// \return This method returns the error code
        /// \endcond
        /// \cond   fr
        /// \brief  Acceder le code d'erreur
        /// \return Cette m&eacute;thode retourne le code d'erreur
        /// \endcond
        int GetErrorCode();

        /// \cond   en
        /// \brief  Set the error if not already set
        /// \param  aCode  The error code
        /// \param  aDesc  The error description
        /// \param  aType  The report type
        /// \retval This method returns the real error code
        /// \endcond
        /// \cond   fr
        /// \brief  Initialiser le code d'erreur s'il n'est pas
        ///         initialis&eacute;
        ///	\param  aCode  Le code d'erreur
        /// \param  aDesc  La description de l'erreur
        /// \param  aType  Le type de rapport
        /// \endcond
        int SetError(int aCode, const char * aDesc, ReportType aType = REPORT_ERROR);

        /// \cond en
        /// \brief Parse a boolean value
        /// \param aArg The arguments
        /// \param aOut The value
        /// \retval false Error
        /// \endcond
        /// \cond fr
        /// \brief Interprete une valeur boolean
        /// \param aArg Les arguments
        /// \param aOut La valeur
        /// \retval false Erreur
        /// \endcond
        /// \retval true OK
        bool Parse(const char ** aArg, bool * aOut);

        /// \cond en
        /// \brief Parse a boolean value
        /// \param aArg     The arguments
        /// \param aOut     The value
        /// \param aDefault The default value
        /// \retval false Error
        /// \endcond
        /// \cond fr
        /// \brief Interprete une valeur boolean
        /// \param aArg     Les arguments
        /// \param aOut     La valeur
        /// \param aDefault La valeur par defaut
        /// \retval false Erreur
        /// \endcond
        /// \retval true OK
        bool Parse(const char ** aArg, bool * aOut, bool aDefault);

        /// \cond en
        /// \brief Parse an unsigned value
        /// \param aArg The arguments
        /// \param aOut The value
        /// \param aMin The minimum value
        /// \param aMax The maximum value
        /// \param aHex The value is in hexadecimal
        /// \retval false Error
        /// \endcond
        /// \cond fr
        /// \brief Interprete une valeur non sign&eacute;e
        /// \param aArg Les arguments
        /// \param aOut La valeur
        /// \param aMin La valeur minimal
        /// \param aMax La valeur maximal
        /// \param aHex La valeur est en hexadecimal
        /// \retval false Erreur
        /// \endcond
        /// \retval true OK
        bool Parse(const char ** aArg, unsigned int * aOut, unsigned int aMin = 0, unsigned int aMax = 0xffffffff, bool aHex = false);

        /// \cond en
        /// \brief Parse an unsigned value
        /// \param aArg     The arguments
        /// \param aOut     The value
        /// \param aMin     The minimum value
        /// \param aMax     The maximum value
        /// \param aHex     The value is in hexadecimal
        /// \param aDefault The default value
        /// \retval false Error
        /// \endcond
        /// \cond fr
        /// \brief Interprete une valeur non sign&eacute;e
        /// \param aArg Les arguments
        /// \param aOut     La valeur
        /// \param aMin     La valeur minimal
        /// \param aMax     La valeur maximal
        /// \param aHex     La valeur est en hexadecimal
        /// \param aDefault La valeur par defaut
        /// \retval false Erreur
        /// \endcond
        /// \retval true OK
        bool Parse(const char ** aArg, unsigned int * aOut, unsigned int aMin, unsigned int aMax, bool aHex, unsigned int aDefault);

        /// \cond en
        /// \brief Parse a text value without space
        /// \param aArg          The arguments
        /// \param aOut          The value
        /// \param aOutSize_byte The maximum size of the value including the
        ///                      end marker
        /// \param aDefault      The default value
        /// \retval false Error
        /// \endcond
        /// \cond fr
        /// \brief Interprete une valeur texte sans espace
        /// \param aArg          Les arguments
        /// \param aOut          La valeur
        /// \param aOutSize_byte La taille maximale de la valeur incluant le
        ///                      marqueur de fin
        /// \param aDefault      La valeur par defaut
        /// \retval false Erreur
        /// \endcond
        /// \retval true OK
        bool Parse(const char ** aArg, char * aOut, unsigned int aOutSize_byte, const char * aDefault = NULL);

        /// \cond    en
        /// \brief   Parse the arguments the user passed to the tools
        /// \param   aCount              Argument count
        /// \param   aVector  [---;R--]  Arguments
        /// \endcond
        /// \cond	 fr
        /// \brief   Interprete les artuments passees a l'outils par
        ///          l'utilisateur
        /// \param   aCount              Le nombre d'arguments
        /// \param   aVector  [---;R--]  Les arguments
        /// \endcond
        /// \exception  Exception  CODE_INVALID_COMMAND_LINE
        void ParseArguments(int aCount, const char ** aVector);

        /// \cond   en
        /// \brief  Parse the commands read from the standard input
        /// \endcond
        /// \cond   fr
        /// \brief  Interprete les commandes lues de l'entree standard
        /// \endcond
        /// \retval 0 OK
        /// \exception  Exception  CODE_...
        int ParseCommands();

        /// \cond   en
        /// \brief  Parse the commands read from a file
        /// \param  aFileName  [---;R--]  File name
        /// \endcond
        /// \cond   fr
        /// \brief  Interprete les commandes lues d'un fichier
        /// \param  aFileName  [---;R--]  Nom du fichier
        /// \endcond
        /// \retval 0 OK
        int ParseCommands(const char * aFileName);

    private:

        void Abort(const char * aArgs);
        void ChangeDir(const char * aArgs);
        void Delay(const char * aArgs);

        void Error_Abort  ();
        void Error_Display();
        void Error_Exit   ();

        void ExecuteCommand(                                                 const char * aLine);
        void ExecuteCommand(const KmsLib::ToolBase::CommandInfo * aCommands, const char * aLine);

        void Exit(const char * aArgs);
        void Exit(int aCode, const char * aDesc);

        int ParseCommands(FILE * aFile);

        void Repeat(const char * aArgs);

        bool TextToBool(const char * aText, bool * aOut);

        bool Verify(unsigned int aValue, unsigned int aMin, unsigned int aMax);

        const CommandInfo * mCommands;

        int         mError_Code;
        std::string mError_Desc;

        bool mExit;

    };

}
