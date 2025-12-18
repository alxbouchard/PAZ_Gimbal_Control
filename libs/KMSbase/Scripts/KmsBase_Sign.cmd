@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      KmsBase_Sign.cmd

rem CODE REVIEW 2020-10-27 KMS - Martin Dubois, P.Eng.

echo Executing  Scripts\KmsBase_Sign.cmd %1  ...

rem ===== Initialisation ====================================================

set CERT_SHA=D5B26E6B4E3159472C2C98307C45D40B129F2078

set FILES="%1"

set SIGNTOOL_EXE="C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x64\signtool.exe"

rem ===== Verification ======================================================

if "" == "%1" (
	echo USER ERROR  Invalid command line
	echo Usage  Scripts\KmsBase_Sign.cmd {Files}
	pause
	exit /B 10
)

if not exist %SIGNTOOL_EXE% (
	echo FATAL ERROR  %SIGNTOOL_EXE%  does not exist
	echo Install the WDK
	pause
	exit /B 20
)

rem ===== Execution =========================================================

%SIGNTOOL_EXE% sign /fd sha256 /sha1 %CERT_SHA% /td sha256 /tr http://timestamp.digicert.com %FILES%
if ERRORLEVEL 1 (
	echo ERROR  %SIGNTOOL_EXE% sign /fd sha256 /sha1 %CERT_SHA% /td sha256 /tr http://timestamp.digicert.com %FILES%  failed - %ERRORLEVEL%
	pause
	exit /B 30
)

rem ===== End ===============================================================

echo  OK
