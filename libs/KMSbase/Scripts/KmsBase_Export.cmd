@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      KmsBase_Export.cmd

echo Executing  Scripts\KmsBase_Export.cmd %1 %2 %3  ...

rem ===== Initialisation ====================================================

set PRODUCT=%1
set VERSION_MMB=%2
set VERSION_TYPE=%3

set EXPORT_CMD_TXT="Export.cmd.txt"
set EXPORT_TXT="Export.txt"

set KMS_COPY="Import\Binaries\Release_32\KmsCopy.exe"

if not exist %KMS_COPY% set KMS_COPY="Debug\KmsCopy.exe"

set EXPORT_FOLDER=K:\Export\%PRODUCT%

if ""=="%VERSION_TYPE%" (
    set DST="%EXPORT_FOLDER%\%VERSION_MMB%_Windows"
) else (
    set DST="%EXPORT_FOLDER%\%VERSION_MMB%_%VERSION_TYPE%_Windows"
)

rem ===== Verification ======================================================

if ""=="%1" (
    echo USER ERROR  Invalid command line
    echo Usage  Scripts\KmsBase_Export.cmd {Product} {Ma.Mi.Bu} [Type]
    pause
    exit /B 10
)

if ""=="%2" (
    echo USER ERROR  Invalid command line
    echo Usage  Scripts\KmsBase_Export.cmd {Product} {Ma.Mi.Bu} [Type]
    pause
    exit /B 20
)

if exist %DST% (
    echo USER ERROR  %DST%  already exist
    pause
    exit /B 30
)

if not exist %EXPORT_CMD_TXT% (
	echo FATAL ERROR  %EXPORT_CMD_TXT%  does not exist
	pause
	exit /B 40
)

if not exist %EXPORT_FOLDER% (
	echo FATAL ERROR  %EXPORT_FOLDER%  does not exist
	echo Create %EXPORT_FOLDER%
	pause
	exit /B 50
)

if not exist %EXPORT_TXT% (
	echo FATAL ERROR  %EXPORT_TXT%  does not exist
	pause
	exit /B 60
)

if not exist %KMS_COPY% (
    echo  FATAL ERROR  %KMS_COPY%  does not exist
    echo  Execute .\Import.cmd
    pause
    exit /B 70
)

rem ===== Execution =========================================================

%KMS_COPY% . %DST% %EXPORT_TXT%
if ERRORLEVEL 1 (
    echo ERROR  %KMS_COPY% . %DST% %EXPORT_TXT%  reported an error - %ERRORLEVEL%
    pause
    exit /B 80
)

%KMS_COPY% . %DST% %EXPORT_CMD_TXT%
if ERRORLEVEL 1 (
    echo  ERROR  %KMS_COPY% . %DST% %EXPORT_CMD_TXT%  reported an error - %ERRORLEVEL%
    pause
    exit /B 90
)

rem ===== End ===============================================================

echo  OK
