@echo off

rem  Auteur		KMS -	Martin Dubois, ing.
rem  Produit	KmsBase
rem  Fichier	ForEachConfig_DLL.cmd
rem  Usage		ForEachConfig_Dll.cmd {EXR} [Arg0] [Arg1]

echo  Executing ForEachConfig_DLL.cmd %1 %2 %3 ...

rem  ===== Verification =====================================================

if "" == "%1" (
	echo  USER ERROR : Invalid command line!
	echo  Usage : call ForEachConfig.cmd {EXE} [Arg0] [Arg1]
	pause
	exit /B 1
)

set  BIN_32_DEBUG_DLL=Debug_DLL
if not exist "%BIN_32_DEBUG_DLL%" (
    echo  FATAL ERROR : %BIN_32_DEBUG_DLL% does not exist
	pause
	exit /B 2
)

set  BIN_32_RELEASE_DLL=Release_DLL
if not exist "%BIN_32_RELEASE_DLL%" (
    echo  FATAL ERROR : %BIN_32_RELEASE_DLL% does not exist
	pause
	exit /B 3
)

set  BIN_64_DEBUG_DLL=x64\Debug_DLL
if not exist "%BIN_64_DEBUG_DLL%" (
    echo  FATAL ERROR : %BIN_64_DEBUG_DLL% does not exist
	pause
	exit /B 4
)

set  BIN_64_RELEASE_DLL=x64\Release_DLL
if not exist "%BIN_64_RELEASE_DLL%" (
    echo  FATAL ERROR : %BIN_64_RELEASE_DLL% does not exist
	pause
	exit /B 5
)

rem  ===== Execution ========================================================

%BIN_32_DEBUG_DLL%\%1 %2 %3
if ERRORLEVEL 1 (
    echo  ERROR : 32 bits - Debug DLL - Failed
	pause
	exit /B 6
)

%BIN_32_RELEASE_DLL%\%1 %2 %3
if ERRORLEVEL 1 (
    echo  ERROR : 32 bits - Release DLL - Failed
	pause
	exit /B 7
)

%BIN_64_DEBUG_DLL%\%1 %2 %3
if ERRORLEVEL 1 (
    echo  ERROR : 64 bits - Debug DLL - Failed
	pause
	exit /B 8
)

%BIN_64_RELEASE_DLL%\%1 %2 %3
if ERRORLEVEL 1 (
    echo  ERROR : 64 bits - Release DLL - Failed
	pause
	exit /B 9
)

rem  ===== Fin ==============================================================

echo  OK
