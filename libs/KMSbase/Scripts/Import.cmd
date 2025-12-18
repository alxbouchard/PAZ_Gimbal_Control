@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      Import.cmd
rem Usage     .\Import.cmd {Destination}

echo Executing  Import.cmd %1  ...

rem ===== Initialisation ====================================================

set DST=%1
set KMS_COPY_EXE="Binaries\Release_32\KmsCopy.exe"
set IMPORT_CMD_TXT="Import.cmd.txt"
set IMPORT_TXT="Import.txt"

rem ===== Verification ======================================================

if ""=="%1" (
	echo USER ERROR  Invalid command line
	echo Usage  .\Import.cmd {Destination}
	pause
	exit /B 10
)

rem ===== Execution =========================================================

if not exist %DST% mkdir %DST%

%KMS_COPY_EXE% . %DST% %IMPORT_TXT%
if ERRORLEVEL 1 (
    echo ERROR  %KMS_COPY_EXE% . %1 %IMPORT_TXT%  reported an error - %ERRORLEVEL%
    pause
    exit /B 20
)

%KMS_COPY_EXE% . %DST% %IMPORT_CMD_TXT%
if ERRORLEVEL 1 (
    echo ERROR  %KMS_COPY_EXE% . %1 %IMPORT_CMD_TXT%  reported an error - %ERRORLEVEL%
    pause
    exit /B 30
)

rem ===== Fin ===============================================================

echo OK
