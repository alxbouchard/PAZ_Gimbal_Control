@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      Test.cmd
rem Usage     .\Test.cmd

echo Executing  Test.cmd  ...

rem ===== Initialisation ====================================================

set DEBUG_32=Debug

set FOR_EACH_CONFIG_NO_DLL="ForEachConfig_NoDLL.cmd"

set FOR_EACH_CONFIG_DLL="ForEachConfig_DLL.cmd"

set KMS_LIB_TEST_EXE="KmsLib_Test.exe"

set RELEASE_32=Release

rem ===== Execution =========================================================

call %FOR_EACH_CONFIG_DLL% %KMS_LIB_TEST_EXE%
if ERRORLEVEL 1 (
    echo  ERROR  %FOR_EACH_CONFIG_DLL% %KMS_LIB_TEST_EXE%  reported an error - %ERRORLEVEL%
	pause
	exit /B 10
)

call %FOR_EACH_CONFIG_NO_DLL% %KMS_LIB_TEST_EXE%
if ERRORLEVEL 1 (
    echo  ERROR  %FOR_EACH_CONFIG_NO_DLL% %KMS_LIB_TEST_EXE%  reported an error - %ERRORLEVEL%
	pause
	exit /B 20
)

echo Testing  KmsVersion.exe  ...

pushd  KmsVersion

    call Test.cmd %DEBUG_32%
	if ERRORLEVEL 1 (
	    echo ERROR  call Test.cmd %DEBUG_32%  reported and error - %ERRORLEVEL%
		pause
		exit /B 30
	)

    call Test.cmd  %RELEASE_32%
	if ERRORLEVEL 1 (
	    echo ERROR  call Test.cmd  %RELEASE_32%  reported and error - %ERRORLEVEL%
		pause
		exit /B 40
	)

popd

rem ===== End ===============================================================

echo  OK
