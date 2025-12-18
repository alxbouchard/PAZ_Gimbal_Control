@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      Build.cmd
rem Usage     .\Build.cmd

rem CODE REVIEW 2020-10-27 KMS - Martin Dubois, P.Eng.

echo Executing  Build.cmd  ...

rem ===== Initialisation ====================================================

set SCRIPT_FOLDER=Scripts

set KMS_BASE_BUILD_BEGIN_CMD="%SCRIPT_FOLDER%\KmsBase_Build_Begin.cmd"
set KMS_BASE_BUILD_INSTALLER_CMD="%SCRIPT_FOLDER%\KmsBase_Build_Installer.cmd"
set KMS_BASE_SIGN_CMD="%SCRIPT_FOLDER%\KmsBase_Sign.cmd"

rem ===== Execution =========================================================

call %KMS_BASE_BUILD_BEGIN_CMD% KmsBase
if ERRORLEVEL 1 (
	echo FATAL ERROR  call %KMS_BASE_BUILD_BEGIN_CMD% KmsBase  reported an error - %ERRORLEVEL%
	pause
	exit /B 10
)

%MSBUILD% %OPTIONS% /property:Configuration=Debug /property:Platform=Win32
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Debug /property:Platform=Win32  reported an error - %ERRORLEVEL%
	pause
	exit /B 20
)

%MSBUILD% %OPTIONS% /property:Configuration=Debug /property:Platform=x64
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Debug /property:Platform=x64  reported an error - %ERRORLEVEL%
	pause
	exit /B 30
)

%MSBUILD% %OPTIONS% /property:Configuration=Debug_DLL /property:Platform=Win32
if ERRORLEVEL 1 (
	echo  ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Debug_DLL /property:Platform=Win32  reported an error - %ERRORLEVEL%
	pause
	exit /B 40
)

%MSBUILD% %OPTIONS% /property:Configuration=Debug_DLL /property:Platform=x64
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Debug_DLL /property:Platform=x64  reported an error - %ERRORLEVEL%
	pause
	exit /B 50
)

%MSBUILD% %OPTIONS% /property:Configuration=Release /property:Platform=Win32
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Release /property:Platform=Win32  reported an error - %ERRORLEVEL%
	pause
	exit /B 60
)

%MSBUILD% %OPTIONS% /property:Configuration=Release /property:Platform=x64
if ERRORLEVEL 1 (
	echo  ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Release /property:Platform=x64  reported an error - %ERRORLEVEL%
	pause
	exit /B 70
)

%MSBUILD% %OPTIONS% /property:Configuration=Release_DLL /property:Platform=Win32
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Release_DLL /property:Platform=Win32  reported an error - %ERRORLEVEL%
	pause
	exit /B 80
)

%MSBUILD% %OPTIONS% /property:Configuration=Release_DLL /property:Platform=x64
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Release_DLL /property:Platform=x64  reported an error - %ERRORLEVEL%
	pause
	exit /B 90
)

call %TEST_CMD%
if ERRORLEVEL 1 (
	echo ERROR  call %TEST_CMD%  reported an error - %ERRORLEVEL%
	pause
	exit /B 100
)

call %KMS_BASE_BUILD_INSTALLER_CMD% %PRODUCT_ISS%
if ERRORLEVEL 1 (
	echo FATAL ERROR  call %KMS_BASE_BUILD_INSTALLER_CMD% %PRODUCT_ISS%  reported an error - %ERRORLEVEL%
	pause
	exit /B 110
)

call %KMS_BASE_SIGN_CMD% Installer/%PRODUCT%*.exe
if ERRORLEVEL 1 (
	echo ERROR  call %KMS_BASE_SIGN_CMD% Installer/%PRODUCT%*.exe  failed - %ERRORLEVEL%
	pause
	exit /B 115
)

%KMS_VERSION_EXE% %VERSION_H% %EXPORT_CMD_TXT%
if ERRORLEVEL 1 (
	echo ERROR  %KMS_VERSION_EXE% %VERSION_H% %EXPORT_CMD_TXT%  reported an error - %ERRORLEVEL%
	pause
	exit /B 120
)

%KMS_VERSION_EXE% -S %VERSION_H% %EXPORT_CMD%
if ERRORLEVEL 1 (
    echo ERROR  %KMS_VERSION_EXE% -S %VERSION_H% %EXPORT_CMD%  reported an error - %ERRORLEVEL%
	pause
	exit /B 130
)

rem ===== End ===============================================================

echo  OK
