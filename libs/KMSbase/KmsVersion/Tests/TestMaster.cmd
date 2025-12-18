@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      KmsVersion/Tests/TestMaster.cmd
rem Usage     Tests/TestMaster.cmd {Config} {Options} {MasterToCopy} {Expected}

echo Executing  Tests\TestMaster.cmd %1 %2 %3 %4  ...

rem ===== Initialisation ====================================================

set TEST_EXE="..\%1\KmsVersion.exe"

set OPTIONS=%2

set MASTER_TO_COPY=%3

set EXPECTED=%4

rem ===== Validation ========================================================

if not exist %TEST_EXE% (
    echo FATAL ERROR  %TEST_EXE%  does not exist
	echo Build the solution
	pause
	exit /B 10
)

if not exist %MASTER_TO_COPY% (
    echo FATAL ERROR  %MASTER_TO_COPY%  does not exist
	pause
	exit /B 20
)

rem ===== Execution =========================================================

copy %MASTER_TO_COPY% "Tests\Test.h"
if ERRORLEVEL 1 (
    echo FATAL ERROR  copy %MASTER_TO_COPY% "Tests\Test.h"  reported an error - %ERRORLEVEL%
	pause
	exit /B 30
)

%TEST_EXE% %OPTIONS% "Tests\Test.h"
if not %EXPECTED% == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% %OPTIONS% "Tests\Test.h"  did not return the expected value - %ERRORLEVEL%
	pause
	exit /B 40
)

if not exist "Tests\Test.h" (
    echo ERROR  "Tests\Test.h"  does not exist
	pause
	exit /B 50
)

if not 0 == %EXPECTED% goto Backup

%TEST_EXE% Tests\Test.h
if ERRORLEVEL 1 (
	echo ERROR  %TEST_EXE% Tests\Test.h  reported an error - %ERRORLEVEL%
	pause
	exit /B 60
)

:Backup

if not exist "Tests\Test.h.bak" (
    echo ERROR  "Tests\Test.h.bak"  does not exist
	pause
	exit /B 70
)

del "Tests\Test.h" "Tests\Test.h.bak"

rem ===== End ===============================================================

echo OK
