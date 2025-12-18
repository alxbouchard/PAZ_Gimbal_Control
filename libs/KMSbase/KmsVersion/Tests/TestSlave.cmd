@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      KmsVersion/Tests/TestSlave.cmd
rem Usage     Tests/TestSlave.cmd {Config} {Options} {Master} {SlaveToCopy} {Extension} {Expected}

echo Executing  Tests\TestSlave.cmd %1 %2 %3 %4 %5 %6  ...

rem ===== Initialisation ====================================================

set TEST_EXE="..\%1\KmsVersion.exe"

set OPTIONS=%2

set MASTER=%3

set SLAVE_TO_COPY=%4

set EXTENSION=%5

set EXPECTED=%6

rem ===== Validation ========================================================

if not exist %TEST_EXE% (
    echo FATAL ERROR  %TEST_EXE%  does not exist
	echo Build the solution
	pause
	exit /B 10
)

if not exist %MASTER% (
    echo FATAL ERROR  %MASTER%  does not exist
	pause
	exit /B 20
)

if not exist %SLAVE_TO_COPY% (
    echo FATAL ERROR  %SLAVE_TO_COPY%  does not exist
	pause
	exit /B 30
)

rem  ===== Execution ========================================================

copy %SLAVE_TO_COPY% "Tests\Test.%EXTENSION%"
if ERRORLEVEL 1 (
    echo FATAL ERROR  copy  %SLAVE_TO_COPY% "Tests\Test.%EXTENSION%"  reported an error - %ERRORLEVEL%
	pause
	exit /B 40
)

%TEST_EXE% %OPTIONS% %MASTER% "Tests\Test.%EXTENSION%"
if not %EXPECTED% == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% %OPTIONS% %MASTER% "Tests\Test.%EXTENSION%"  did not return the expected value - %ERRORLEVEL%
	pause
	exit /B 50
)

if not exist "Tests\Test.%EXTENSION%" (
    echo ERROR  "Tests\Test.%EXTENSION%"  does not exist
	pause
	exit /B 60
)

if not exist "Tests\Test.%EXTENSION%.bak" (
    echo ERROR  "Tests\Test.%EXTENSION%.bak"  does not exist
	pause
	exit /B 70
)

del "Tests\Test.%EXTENSION%" "Tests\Test.%EXTENSION%.bak"

rem ===== End ===============================================================

echo OK
