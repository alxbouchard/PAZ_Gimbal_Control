@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      Export.cmd
rem Usage     .\Export.cmd {Ma.Mi.Bu} [Type]

rem CODE REVIEW 2020-08-17 KMS - Martin Dubois, P.Eng.

echo  Executing  Export.cmd %1 %2  ...

rem ===== Initialisation ====================================================

set KMS_BASE_EXPORT_CMD="Scripts\KmsBase_Export.cmd"

rem ===== Execution =========================================================

call %KMS_BASE_EXPORT_CMD% KmsBase %1 %2
if ERRORLEVEL 1 (
	echo ERROR  call %KMS_BASE_EXPORT_CMD% KmsBase %1 %2  reported an error - %ERRORLEVEL%
	pause
	exit /B 10
)

rem ===== End ===============================================================

echo  OK
