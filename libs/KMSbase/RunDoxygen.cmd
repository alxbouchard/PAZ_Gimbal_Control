@echo off

rem  Author / Auteur	:	KMS -	Martin Dubois, ing.
rem  Product / Produit	:	KmsBase
rem  File / Fichier		:	RunDoxygen.cmd

echo  Executing RunDoxygen.cmd ...

rem  ===== Validation =======================================================

set DOXYFILE_EN="DoxyFile_en.txt"
if not exist %DOXYFILE_EN% (
    echo  FATAL ERROR : %DOXYFILE_EN% does not exist!
	pause
	exit /B 1
)

set DOXYFILE_FR="DoxyFile_fr.txt"
if not exist %DOXYFILE_FR% (
    echo  FATAL ERROR : %DOXYFILE_FR% does not exist!
	pause
	exit /B 1
)

set DOXYGEN="C:\Program Files\doxygen\bin\doxygen.exe"
if not exist %DOXYGEN% (
    echo  FATAL ERROR : %DOXYGEN% does not exist!
	echo  Install doxygen!
	pause
	exit /B 1
)

rem  ===== Execution ========================================================

%DOXYGEN% %DOXYFILE_EN%
if ERRORLEVEL 1 (
    echo  ERROR : %DOXYGEN% reported an error! {%DOXYFILE_EN%}
	pause
	exit /B 1
)

%DOXYGEN% %DOXYFILE_FR%
if ERRORLEVEL 1 (
    echo  ERROR : %DOXYGEN% reported an error! {%DOXYFILE_FR%}
	pause
	exit /B 1
)

rem  ===== End ==============================================================

echo  OK!
