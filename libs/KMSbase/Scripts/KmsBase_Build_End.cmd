@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      Scripts\KmsBase_Build_End.cmd
rem Usage     Scripts\Build.cmd

echo Executing  Scripts\KmsBase_Build_End.cmd  ...

rem ===== Verification ======================================================

if not exist %EXPORT_CMD% (
	echo FATAL ERROR  %EXPORT_CMD%  does not exist
	pause
	exit /B 10
)

if not exist %EXPORT_CMD_TXT% (
	echo FATAL ERROR  %EXPORT_CMD_TXT%  does not exist
	pause
	exit /B 20
)

if not exist %PRODUCT_ISS% (
	echo FATAL ERROR  %PRODUCT_ISS%  does not exist
	pause
	exit /B 30
)

if not exist %TEST_CMD% (
	echo FATAL ERROR  %TEST_CMD%  does not exist
	pause
	exit /B 40
)

if not exist %VERSION_H% (
	echo FATAL ERROR  %VERSION_H%  does not exist
	pause
	exit /B 50
)

rem ===== Execution =========================================================

call %TEST_CMD%
if ERRORLEVEL 1 (
    echo ERROR  call %TEST_CMD%  reported an error - %ERRORLEVEL%
	pause
	exit /B 60
)

%KMS_VERSION% %VERSION_H% %EXPORT_CMD_TXT% %PRODUCT_ISS%
if ERRORLEVEL 1 (
	echo ERROR  %KMS_VERSION% %VERSION_H% %EXPORT_CMD_TXT% %PRODUCT_ISS%  reported an error - %ERRORLEVEL%
	pause
	exit /B 70
)

%INNO_COMPIL32% /cc %PRODUCT_ISS%
if ERRORLEVEL 1 (
	echo ERROR  %INNO_COMPIL32% /cc %PRODUCT_ISS%  repored an error - %ERRORLEVEL%
	pause
	exit /B 80
)

%KMS_VERSION_EXE% -S %VERSION_H% %EXPORT_CMD%
if ERRORLEVEL 1 (
    echo ERROR  %KMS_VERSION_EXE% -S %VERSION_H% %EXPORT_CMD%  reported an error - %ERRORLEVEL%
	pause
	exit /B 90
)

rem ===== End ===============================================================

echo OK
