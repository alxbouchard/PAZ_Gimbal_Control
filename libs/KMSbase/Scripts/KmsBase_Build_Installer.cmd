@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      Scripts\KmsBase_Build_Installer.cmd

echo Executing  Scripts\KmsBase_Build_Installer.cmd %1  ...

rem ===== Initialisation ====================================================

set FILE_ISS=%1

set INNO_COMPIL32="C:\Program Files (x86)\Inno Setup 6\Compil32.exe"

rem ===== Verification ======================================================

if ""==%1 (
    echo FATAL ERROR  Invalid command line
    echo Usage  Scripts\KmsBase_Build_Installer.cmd {FileISS}
    pause
    exit /B 10
)

if not exist %FILE_ISS% (
    echo FATAL ERROR  %FILE_ISS%  does not exist
    pause
    exit /B 10
)

if not exist %INNO_COMPIL32% (
    echo FATAL ERROR  %INNO_COMPIL32%  does not exist
    echo Install Inno Setup 5.6.1
    pause
    exit /B 20
)

rem ===== Execution =========================================================

%KMS_VERSION_EXE% %VERSION_H% %FILE_ISS%
if ERRORLEVEL 1 (
    echo ERROR  %KMS_VERSION% %VERSION_H% %FILE_ISS%  reported an error - %ERRORLEVEL%
    pause
    exit /B 30
)

%INNO_COMPIL32% /cc %FILE_ISS%
if ERRORLEVEL 1 (
    echo ERROR  %INNO_COMPIL32% /cc %FILE_ISS%  reported an error - %ERRORLEVEL%
    pause
    exit /B 40
)

rem ===== End ===============================================================

echo OK
