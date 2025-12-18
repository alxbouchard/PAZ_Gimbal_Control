
// Auteur	KMS - Martin Dubois, ing.
// Produit	KmsBase
// Fichier	KmsLib_Test/KmsLib_Test.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

#include <KmsBase.h>

// ===== Interface ==========================================================
#include <KmsTest.h>

// Test
/////////////////////////////////////////////////////////////////////////////

// Setup 0	Un GPU supportant OpenCL est disponible.
// Setup A	Le convertisseur RS232 a USB est connecte a l'ordinateur.
// Setup B	Execute en tant qu'administrateur.
// Setup C	Le convertisseur RS232 a USB est connecte a l'ordinateur et le
//			programme de test s'execute en tant qu'administrateur.

KMS_TEST_GROUP_LIST_BEGIN
	KMS_TEST_GROUP_LIST_ENTRY("Base"	)

	KMS_TEST_GROUP_LIST_ENTRY("Setup-0")

	#ifdef _KMS_WINDOWS_
		KMS_TEST_GROUP_LIST_ENTRY("Setup-A" )
		KMS_TEST_GROUP_LIST_ENTRY("Setup-B"	)
		KMS_TEST_GROUP_LIST_ENTRY("Setup-C"	)
	#endif // _KMS_WINDOWS_
KMS_TEST_GROUP_LIST_END

extern int CmdLineParser_Base	();
extern int DebugLog_Base		();
extern int Dump_Base			();
extern int DriverHandle_Base    ();
extern int Exception_Base		();
extern int File_Base            ();
extern int FileHandle_Base      ();
extern int IgnoreList_Base		();
extern int Linux_Windows_Base	();
extern int MemTester_Base		();
extern int Ring_Base			();
extern int RLE_Base				();
extern int String_Base          ();
extern int TextFile_Base        ();
extern int ThreadBase_Base      ();
extern int ToolBase_Base		();
extern int ValueVector_Base     ();
extern int Walker_Base			();

#if defined( _KMS_LINUX_ ) || defined( _KMS_WINDOWS_ )
    // extern int OpenCL_Base			();
#endif // _KMS_LINUX_ || _KMS_WINDOWS_

#ifdef _KMS_WINDOWS_
	extern int ComPortHandle_Base		();
	extern int ComPortHandle_SetupA		();
	extern int DriverHandle_SetupA		();
	extern int DriverHandle_SetupC		();
	extern int NetworkAddress_Base      ();
	extern int RegistryKey_Base			();
	extern int Service_Base				();
	extern int Service_SetupB			();
	extern int Socket_Base              ();
	extern int SystemLog_Base			();
	extern int SystemLog_SetupB			();
	extern int TextFileEditor_Base  	();
	extern int Walker_Smart_Base		();
	extern int Windows_Windows_Base		();
	extern int Windows_Windows_SetupB	();
#endif // _KMS_WINDOWS_

KMS_TEST_LIST_BEGIN
	KMS_TEST_LIST_ENTRY(CmdLineParser_Base	, "CmdLineParser - Base"	, 0, 0)
	KMS_TEST_LIST_ENTRY(DebugLog_Base		, "DebugLog - Base"			, 0, 0)
    KMS_TEST_LIST_ENTRY(DriverHandle_Base   , "DriverHandle - Base"     , 0, 0)
    KMS_TEST_LIST_ENTRY(Dump_Base			, "Dump - Base"				, 0, 0)
	KMS_TEST_LIST_ENTRY(Exception_Base		, "Exception - Base"		, 0, 0)
    KMS_TEST_LIST_ENTRY(File_Base           , "File - Base"             , 0, 0)
    KMS_TEST_LIST_ENTRY(FileHandle_Base     , "FileHandle - Base"       , 0, 0)
    KMS_TEST_LIST_ENTRY(IgnoreList_Base		, "IgnoreList - Base"		, 0, 0)
	KMS_TEST_LIST_ENTRY(Linux_Windows_Base	, "Linux - Windows - Base"	, 0, 0)
	KMS_TEST_LIST_ENTRY(MemTester_Base		, "MemTester - Base"		, 0, 0)
	KMS_TEST_LIST_ENTRY(RLE_Base			, "RLE - Base"				, 0, 0)
	KMS_TEST_LIST_ENTRY(Ring_Base			, "Ring - Base"				, 0, 0)
    KMS_TEST_LIST_ENTRY(String_Base         , "String - Base"           , 0, 0)
    KMS_TEST_LIST_ENTRY(TextFile_Base       , "TextFile - Base"         , 0, 0)
	KMS_TEST_LIST_ENTRY(ToolBase_Base		, "ToolBase - Base"			, 0, 0)
    KMS_TEST_LIST_ENTRY(ThreadBase_Base     , "ThreadBase - Base"       , 0, 0)
    KMS_TEST_LIST_ENTRY(ValueVector_Base    , "ValueVector - Base"      , 0, 0)
	KMS_TEST_LIST_ENTRY(Walker_Base			, "Walker - Base"			, 0, 0)

    #if defined( _KMS_LINUX_ ) || defined( _KMS_WINDOWS_ )
        // KMS_TEST_LIST_ENTRY(OpenCL_Base			, "OpenCL - Base"			, 1, KMS_TEST_FLAG_INTERACTION_NEEDED	)
    #endif // _KMS_LINUX_ || _KMS_WINDOWS_

    #ifdef _KMS_WINDOWS_
		KMS_TEST_LIST_ENTRY(ComPortHandle_Base		, "ComPortHandle - Base"		, 0, 0									)
		KMS_TEST_LIST_ENTRY(ComPortHandle_SetupA	, "ComPortHandle - SetupA"		, 2, KMS_TEST_FLAG_INTERACTION_NEEDED	)
		KMS_TEST_LIST_ENTRY(DriverHandle_SetupA		, "DriverHandle - Setup A"		, 2, KMS_TEST_FLAG_INTERACTION_NEEDED	)
		KMS_TEST_LIST_ENTRY(DriverHandle_SetupC		, "DriverHandle - Setup C"		, 4, KMS_TEST_FLAG_INTERACTION_NEEDED	)
		KMS_TEST_LIST_ENTRY(NetworkAddress_Base     , "NetworkAddress - Base"       , 0, 0                                  )
		KMS_TEST_LIST_ENTRY(RegistryKey_Base		, "RegistryKey - Base"			, 0, 0									)
		KMS_TEST_LIST_ENTRY(Service_Base			, "Service - Base"				, 0, 0									)
		KMS_TEST_LIST_ENTRY(Service_SetupB			, "Service - Setup B"			, 3, KMS_TEST_FLAG_INTERACTION_NEEDED	)
		KMS_TEST_LIST_ENTRY(Socket_Base             , "Socket - Base"               , 0, 0                                  )
		KMS_TEST_LIST_ENTRY(SystemLog_Base			, "SystemLog - Base"			, 0, 0									)
		KMS_TEST_LIST_ENTRY(SystemLog_SetupB		, "SystemLog - SetupB"			, 3, KMS_TEST_FLAG_INTERACTION_NEEDED	)
		KMS_TEST_LIST_ENTRY(TextFileEditor_Base     , "TextFileEditor - Base"       , 0, 0                                  )
		KMS_TEST_LIST_ENTRY(Walker_Smart_Base       , "Walker_Smart - Base"         , 0, 0                                  )
		KMS_TEST_LIST_ENTRY(Windows_Windows_Base	, "Windows - Windows - Base"	, 0, 0									)
		KMS_TEST_LIST_ENTRY(Windows_Windows_SetupB	, "Windows - Windows - Setup B"	, 3, KMS_TEST_FLAG_INTERACTION_NEEDED	)
	#endif // _KMS_WINDOWS_
KMS_TEST_LIST_END

KMS_TEST_MAIN
