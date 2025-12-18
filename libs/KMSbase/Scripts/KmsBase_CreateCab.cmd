@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      KmsBase_CreateCab.cmd

echo Executing  Scripts\KmsBase_CreateCab.cmd %1  ...

rem ===== Initialisation ====================================================

set IMPORT_SCRIPT_FOLDER=Import\Scripts

set PRODUCT=%1

set PRODUCT_CAB=disk1\%PRODUCT%.cab

set PRODUCT_DDF=Product.ddf

set SIGN_CMD="%IMPORT_SCRIPT_FOLDER%\KmsBase_Sign.cmd"

rem ===== Verification ======================================================

if ""=="%1" (
    echo USER ERROR  Invalid command line
    echo Usage  Scripts\KmsBase_CreateCab.cmd {Product}
    pause
    exit /B 10
)

if not exist %IMPORT_SCRIPT_FOLDER% (
    echo  FATAL ERROR  %IMPORT_SCRIPT_FOLDER%  does not exist
    echo  Execute .\Import.cmd
    pause
    exit /B 20
)

if not exist %PRODUCT_DDF% (
    echo  FATAL ERROR  %PRODUCT_DDF%  does not exist
    pause
    exit /B 30
)

if not exist %SIGN_CMD% (
    echo  FATAL ERROR  %SIGN_CMD%  does not exist
    echo  Execute .\Import.cmd
    pause
    exit /B 40
)

rem ===== Execution =========================================================

makecab -f %PRODUCT_DDF%
if ERRORLEVEL 1 (
	echo ERROR  makecab -f %PRODUCT_DDF%  failed - %ERRORLEVEL%
	pause
	exit /B 50
)

call %SIGN_CMD% %PRODUCT_CAB%
if ERRORLEVEL 1 (
	echo ERROR  call %SIGN_CMD% %PRODUCT_CAB%  failed - %ERRORLEVEL%
	pause
	exit /B 60
)

rem ===== End ===============================================================

echo OK
