@echo off

rem  Author / Auteur	:	KMS -	Martin Dubois, ing.
rem  Product / Produit	:	KmsBase
rem  File / Fichier		:	ForEachConfig_NoDLL.cmd

echo  Executing ForEachConfig_NoDLL.cmd %1 %2 %3 ...

rem  ===== Verification =====================================================

if "" == "%1" (
	echo  USER ERROR : Invalid command line!
	echo  Usage : call ForEachConfig.cmd {EXE} [Arg0] [Arg1]
	pause
	exit /B 1
)

set  BIN_32_DEBUG=Debug
if not exist "%BIN_32_DEBUG%" (
    echo  FATAL ERROR : %BIN_32_DEBUG% does not exist!
	pause
	exit /B 2
)

set  BIN_32_RELEASE=Release
if not exist "%BIN_32_RELEASE%" (
    echo  FATAL ERROR : %BIN_32_RELEASE% does not exist!
	pause
	exit /B 3
)

set  BIN_64_DEBUG=x64\Debug
if not exist "%BIN_64_DEBUG%" (
    echo  FATAL ERROR : %BIN_64_DEBUG% does not exist!
	pause
	exit /B 4
)

set  BIN_64_RELEASE=x64\Release
if not exist "%BIN_64_RELEASE%" (
    echo  FATAL ERROR : %BIN_64_RELEASE% does not exist!
	pause
	exit /B 5
)

rem  ===== Execution ========================================================

%BIN_32_DEBUG%\%1 %2 %3
if ERRORLEVEL 1 (
    echo  ERROR : 32 bits - Debug - Failed!
	pause
	exit /B 6
)

%BIN_32_RELEASE%\%1 %2 %3
if ERRORLEVEL 1 (
    echo  ERROR : 32 bits - Release - Failed!
	pause
	exit /B 7
)

%BIN_64_DEBUG%\%1 %2 %3
if ERRORLEVEL 1 (
    echo  ERROR : 64 bits - Debug - Failed!
	pause
	exit /B 8
)

%BIN_64_RELEASE%\%1 %2 %3
if ERRORLEVEL 1 (
    echo  ERROR : 64 bits - Release - Failed!
	pause
	exit /B 9
)

rem  ===== Fin ==============================================================

echo OK!
