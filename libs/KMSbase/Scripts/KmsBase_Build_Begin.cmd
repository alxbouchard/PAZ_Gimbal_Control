@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      Scripts\KmsBase_Build_Begin.cmd

echo Executing  KmsBase_Build_Begin.cmd %1  ...

rem ===== Initialisation ====================================================

set PRODUCT=%1

set EXPORT_CMD="Export.cmd"
set EXPORT_CMD_TXT="Export.cmd.txt"
set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe"
set TEST_CMD="Test.cmd"
set VERSION_H="Common/Version.h"

set KMS_VERSION_EXE="Import\Binaries\Release_32\KmsVersion.exe"

if not exist %KMS_VERSION_EXE% set KMS_VERSION_EXE="Debug\KmsVersion.exe"

set OPTIONS="%PRODUCT%.sln" /target:rebuild
set PRODUCT_ISS="Product.iss"

rem ===== Verification ======================================================

if ""==%1 (
	echo FATAL ERROR  Invalid command line
	echo Usage  Scripts\KmsBase_Build_Begin.cmd {Product}
	pause
	exit /B 10
)

if not exist %EXPORT_CMD% (
	echo FATAL ERROR  %EXPORT_CMD%  does not exist
	pause
	exit /B 13
)

if not exist %EXPORT_CMD_TXT% (
	echo FATAL ERROR  %EXPORT_CMD_TXT%  does not exist
	pause
	exit /B 16
)

if not exist %KMS_VERSION_EXE% (
	echo FATAL ERROR  %KMS_VERSION_EXE%  does not exist
	echo Execute .\Import.cmd
	pause
	exit /B 20
)

if not exist %MSBUILD% (
	echo FATAL ERROR  %MSBUILD%  does not exist
    echo Install Visual Studio 2017
	pause
	exit /B 30
)

if not exist %PRODUCT_ISS% (
	echo FATAL ERROR  %PRODUCT_ISS%  does not exist
	pause
	exit /B 40
)

if not exist %TEST_CMD% (
	echo FATAL ERROR  %TEST_CMD%  does not exist
	pause
	exit /B 50
)

if not exist %VERSION_H% (
	echo FATAL ERROR  %VERSION_H%  does not exist
	pause
	exit /B 60
)

rem ===== End ===============================================================

echo  OK
