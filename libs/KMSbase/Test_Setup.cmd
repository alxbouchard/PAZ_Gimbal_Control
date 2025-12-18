@echo off

rem  Auteur		KMS -	Martin Dubois, ing.
rem  Produit	KmsBase
rem  Fichier	Test_Setup.cmd
rem  Usage		Test_Setup.cmd {1|2|3|4}

echo  Executing Test_Setup0.cmd %1 ...

rem  ===== Verification =====================================================

if "1" EQU "%1" goto ArgOK
if "2" EQU "%1" goto ArgOK
if "3" EQU "%1" goto ArgOK
if "4" EQU "%1" goto ArgOK

echo  USER ERROR : Invalid command line
pause
exit /B 1

:ArgOK

set FOR_EACH_CONFIG_DLL="ForEachConfig_DLL.cmd"
if not exist %FOR_EACH_CONFIG_DLL% (
    echo  FATAL ERROR : %FOR_EACH_CONFIG_DLL% does not exist
	pause
	exit /B 2
)

set FOR_EACH_CONFIG_NO_DLL="ForEachConfig_NoDLL.cmd"
if not exist %FOR_EACH_CONFIG_NO_DLL% (
    echo  FATAL ERROR : %FOR_EACH_CONFIG_NO_DLL% does not exist
	pause
	exit /B 3
)

rem  ===== Execution ========================================================

call %FOR_EACH_CONFIG_DLL% KmsLib_Test.exe group %1
if ERRORLEVEL 1 (
    echo  ERROR : %FOR_EACH_CONFIG_DLL% reported an error
	pause
	exit /B 4
)

call %FOR_EACH_CONFIG_NO_DLL% KmsLib_Test.exe group %1
if ERRORLEVEL 1 (
    echo  ERROR : %FOR_EACH_CONFIG_NO_DLL% reported an error
	pause
	exit /B 5
)

rem  ===== Fin ==============================================================

echo OK!
