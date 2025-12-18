@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2019 KMS. All rights reserved.
rem License   http://www.apache.org/licenses/LICENSE-2.0
rem Product   KmsBase
rem File      KmsVersion/Test.cmd
rem Usage     .\Test.cmd [Debug|Release]

echo Executing  KmsVersion/Test.cmd %1  ...

setlocal

rem ===== Initialisation ====================================================

set CONFIG=%1

set TEST_EXE="..\%CONFIG%\KmsVersion.exe"

rem ===== Verification ======================================================

if not exist "..\%CONFIG%" (
    echo USER ERROR  "..\%CONFIG%"  does not exist
    pause
	exit /B 10
)

if not exist %TEST_EXE% (
    echo FATAL ERROR  %TEST_EXE%  does not exist
	echo Build the solution
    pause
	exit /B 20
)

rem ===== Execution =========================================================

rem TEST INFO KmsVersion
rem           Does not display the version information when the -1 option is present.

rem TEST INFO KmsVersion.Error
rem           The command line is incomplet.

%TEST_EXE% -q
if not 3 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% -q  did not reported the expected error - %ERRORLEVEL%
	pause
	exit /B 30
)

rem TEST INFO KmsVersion
rem           Diaplays the version information when the -q option is not present.

%TEST_EXE%
if not 3 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE%  did not reported the expected error - %ERRORLEVEL%
	pause
	exit /B 40
)

rem TEST INFO KmsVersion.Error
rem           An invalid option is present.

%TEST_EXE% -z
if not 1 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% -z  did not reported the expected error - %ERRORLEVEL%
	pause
	exit /B 50
)

rem TEST INFO KmsVersion
rem           Displays help when the -? option is present.

%TEST_EXE% -?
if ERRORLEVEL 1 (
    echo ERROR  %TEST_EXE% -?  reported an error - %ERRORLEVEL%
	pause
	exit /B 60
)

rem TEST INFO KmsVersion
rem           Displays help when the -h option is present.

%TEST_EXE% -h
if ERRORLEVEL 1 (
    echo ERROR  %TEST_EXE% -h  reported an error - %ERRORLEVEL%
	pause
	exit /B 70
)

rem ----- Master ------------------------------------------------------------

rem TEST INFO KmsVersion.Error
rem           The master file does not exist.

%TEST_EXE% DoesNotExist
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% DoesNotExist  did not reported the expected error - %ERRORLEVEL%
	pause
	exit /B 80
)

rem TEST INFO KmsVersion.Error
rem           The master file does not exist and an option request to modify it.

%TEST_EXE% -a DoesNoExist
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% -a DoesNoExist  did not reported the expected error - %ERRORLEVEL%
	pause
	exit /B 90
)

rem TEST INFO KmsVersion
rem           Parse a valid master file.

rem TEST INFO KmsVersion
rem           Displays the version with the requested number of elements.

%TEST_EXE% -2 Tests\Master_OK.h
if ERRORLEVEL 1 (
    echo ERROR  %TEST_EXE% -2 Tests\Master_OK.h  reported an error - %ERRORLEVEL%
	pause
	exit /B 100
)

%TEST_EXE% -3 Tests\Master2_OK.h
if ERRORLEVEL 1 (
    echo ERROR  %TEST_EXE% -3 Tests\Master2_OK.h  reported an error - %ERRORLEVEL%
	pause
	exit /B 110
)

%TEST_EXE% -4 Tests\MasterT_OK.h
if ERRORLEVEL 1 (
    echo ERROR  %TEST_EXE% -4 Tests\MasterT_OK.h  reported an error - %ERRORLEVEL%
	pause
	exit /B 120
)

rem TEST INFO KmsVersion.ExecuteScript
rem           Accept the -S option even if no script name are present.

%TEST_EXE% -S Tests\Master_OK.h
if ERRORLEVEL 1 (
    echo  ERROR  %TEST_EXE% -S Tests\Master_OK.h  reported an error - %ERRORLEVEL%
	pause
	exit /B 130
)

rem TEST INFO KmsVersion.ExecuteScript.Error
rem           The script does not exist.

%TEST_EXE% -S Tests\Master2_OK.h DoesNotExist.cmd
if not 1 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% -S Tests\Master2_OK.h DoesNotExist.cmd  did not reported the expected error - %ERRORLEVEL%
	pause
	exit /B 140
)

rem TEST INFO KmsVersion
rem           Increment or modyfy the version as requested.

rem TEST INFO KmsVersion
rem           Displays the element of a version.

call Tests\TestMaster.cmd %CONFIG% -Aa Tests\MasterT_OK.h 0
if ERRORLEVEL 1 exit /B 150

call Tests\TestMaster.cmd %CONFIG% -Mm Tests\Master_OK.h 0
if ERRORLEVEL 1 exit /B 160

call Tests\TestMaster.cmd %CONFIG% -Bb Tests\Master2_OK.h 0
if ERRORLEVEL 1 exit /B 170

call Tests\TestMaster.cmd %CONFIG% -Cc Tests\MasterT_OK.h 0
if ERRORLEVEL 1 exit /B 180

call Tests\TestMaster.cmd %CONFIG% -Ii Tests\Master_OK.h 0
if ERRORLEVEL 1 exit /B 190

call Tests\TestMaster.cmd %CONFIG% -e Tests\Master2_OK.h 0
if ERRORLEVEL 1 exit /B 200

rem TEST INFO KmsVersion.Error
rem           Modifting master file is request and impossible.

copy Tests\MasterT_OK.h Tests\Test.h
if ERRORLEVEL 1 (
    echo ERROR  copy Tests\MasterT_OK.h Tests\Test.h  reported an error - %ERRORLEVEL%
	pause
	exit /B 210
)

attrib +R Tests\Test.h
if ERRORLEVEL 1 (
    echo ERROR  attrib +R Tests\Test.h  reported an error - %ERRORLEVEL%
	pause
	exit /B 220
)

%TEST_EXE% -b Tests\Test.h
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% -b Tests\Test.h  did not report the expected error - %ERRORLEVEL%
	pause
	exit /B 230
)

del /F Tests\Test.h Tests\Test.h.bak

rem TEST INFO KmsVersion
rem           See Master_... files.

call Tests\TestMaster.cmd %CONFIG% -a Tests\Master_00.h 2
if ERRORLEVEL 1 exit /B 240

call Tests\TestMaster.cmd %CONFIG% -c Tests\Master_01.h 2
if ERRORLEVEL 1 exit /B 250

%TEST_EXE% Tests\Master_OK.h Invalid
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE%  Tests\Master_OK.h Invalid  did not report the expected error - %ERRORLEVEL%
	pause
	exit /B 260
)

rem ----- Slave -------------------------------------------------------------

rem TEST INFO KmsVersion.Error
rem           The extension of the slave file is not supported.

%TEST_EXE% Tests\Master2_OK.h Unsupported.aaa
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% Tests\Master2_OK.h Unsupported.aaa  did not report the expected error - %ERRORLEVEL%
	pause
	exit /B 270
)

rem TEST INFO KmsVersion.Error
rem           The slave file does not exist.

%TEST_EXE% Tests\MasterT_OK.h DoesNotExist.cmd
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% Tests\MasterT_OK.h DoesNotExist.cmd  did not report the expected error - %ERRORLEVEL%
	pause
	exit /B 280
)

%TEST_EXE% Tests\Master_OK.h DoesNotExist.inf
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% Tests\Master_OK.h DoesNotExist.inf  did not report the expected error - %ERRORLEVEL%
	pause
	exit /B 290
)

%TEST_EXE% Tests\Master_OK.h DoesNotExist.iss
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% Tests\Master_OK.h DoesNotExist.iss  did not report the expected error - %ERRORLEVEL%
	pause
	exit /B 300
)

%TEST_EXE% Tests\Master_OK.h DoesNotExist.txt
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% Tests\Master_OK.h DoesNotExist.txt  did not report the expected error - %ERRORLEVEL%
	pause
	exit /B 310
)

%TEST_EXE% Tests\Master2_OK.h DoesNotExist.xml
if not 2 == %ERRORLEVEL% (
    echo ERROR  %TEST_EXE% Tests\Master2_OK.h DoesNotExist.xml  did not report the expected error - %ERRORLEVEL%
	pause
	exit /B 32
)

rem TEST INFO KmsVersion
rem           Modify slage file as expected.

call Tests\TestSlave.cmd %CONFIG% -q Tests\MasterT_OK.h Tests\Slave_OK.cmd cmd 0
if ERRORLEVEL 1 exit /B 330

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_OK.inf inf 0
if ERRORLEVEL 1 exit /B 340

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_OK.iss iss 0
if ERRORLEVEL 1 exit /B 350

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_OK.txt txt 0
if ERRORLEVEL 1 exit /B 360

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master2_OK.h Tests\Slave_OK.xml xml 0
if ERRORLEVEL 1 exit /B 370

rem TEST INFO KmsVersion.Error
rem           A slave file does not contain instruction.

call Tests\TestSlave.cmd %CONFIG% -q Tests\MasterT_OK.h Tests\Slave_00.cmd bat 2
if ERRORLEVEL 1 exit /B 380

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_00.inf inx 2
if ERRORLEVEL 1 exit /B 390

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_00.iss iss 2
if ERRORLEVEL 1 exit /B 400

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_00.txt iss 2
if ERRORLEVEL 1 exit /B 410

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master2_OK.h Tests\Slave_00.xml wxs 2
if ERRORLEVEL 1 exit /B 420

rem TEST INFO KmsVersion.Errror
rem           A slave file contains a incomplet instruction.

call Tests\TestSlave.cmd %CONFIG% -q Tests\MasterT_OK.h Tests\Slave_01.cmd cmd 2
if ERRORLEVEL 1 exit /B 430

call Tests\TestSlave.cmd %CONFIG% -q Tests\MasterT_OK.h Tests\Slave_01.txt txt 2
if ERRORLEVEL 1 exit /B 440

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_01.xml xml 2
if ERRORLEVEL 1 exit /B 450

rem TEST INFO KmsVersion.Error
rem           A slave file contains an instruction on the last line.

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master2_OK.h Tests\Slave_02.cmd bat 2
if ERRORLEVEL 1 exit /B 460

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master2_OK.h Tests\Slave_02.txt txt 2
if ERRORLEVEL 1 exit /B 470

call Tests\TestSlave.cmd %CONFIG% -q Tests\MasterT_OK.h Tests\Slave_02.xml wxs 2
if ERRORLEVEL 1 exit /B 480

rem TEST INFO KmsVersion.Error
rem           The line after the directive cannot be processed.

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_03.cmd cmd 2
if ERRORLEVEL 1 exit /B 490

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_03.txt txt 2
if ERRORLEVEL 1 exit /B 500

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master2_OK.h Tests\Slave_03.xml xml 2
if ERRORLEVEL 1 exit /B 510

call Tests\TestSlave.cmd %CONFIG% -q Tests\MasterT_OK.h Tests\Slave_04.cmd bat 2
if ERRORLEVEL 1 exit /B 520

call Tests\TestSlave.cmd %CONFIG% -q Tests\MasterT_OK.h Tests\Slave_04.txt txt 2
if ERRORLEVEL 1 exit /B 530

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_04.xml wxs 2
if ERRORLEVEL 1 exit /B 540

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master2_OK.h Tests\Slave_05.cmd cmd 2
if ERRORLEVEL 1 exit /B 550

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master2_OK.h Tests\Slave_05.txt txt 2
if ERRORLEVEL 1 exit /B 560

call Tests\TestSlave.cmd %CONFIG% -q Tests\MasterT_OK.h Tests\Slave_05.xml xml 2
if ERRORLEVEL 1 exit /B 570

rem TEST INFO KmsVersion.Error
rem           A slave file contains more directive than expected.

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_06.iss iss 2
if ERRORLEVEL 1 exit /B 580

call Tests\TestSlave.cmd %CONFIG% -q Tests\Master_OK.h Tests\Slave_06.inf xml 2
if ERRORLEVEL 1 exit /B 590

rem ===== End ===============================================================

endlocal

echo  OK
