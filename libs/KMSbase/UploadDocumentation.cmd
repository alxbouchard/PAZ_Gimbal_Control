@echo off

rem Author     KMS - Martin Dubois, P.Eng.
rem Copyright  (C) 2020 KMS
rem License    http://www.apache.org/licenses/LICENSE-2.0
rem Product    KmsBase
rem File       UploadDocumentation.cmd

echo  Executing  UploadDocumentation.cmd  ...

rem ===== Configuration =====================================================

set FTP_SERVER=ftp.kms-quebec.com

rem ===== Execution =========================================================

ftp.exe -i -s:UploadDocumentation.cmd.txt %FTP_SERVER%
if ERRORLEVEL 1 (
    echo  ERROR  ftp -s:UploadDocumentation.txt %FTP_SERVER%  failed - %ERRORLEVEL%
    pause
    exit /B 10
)

rem ===== End ===============================================================

echo  OK
