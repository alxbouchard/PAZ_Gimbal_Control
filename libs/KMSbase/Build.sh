#!/bin/sh

# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2020-2021 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# File      Build.sh
# Usage     ./Build.sh

# CODE REVIEW 2021-08-17 KMS - Martin Dubois, P.Eng.

echo Executing  Build.sh  ...

# ===== Initialisation ======================================================

KMS_VERSION=Binaries/KmsVersion

OS=`uname`

VERSION_H=Common/Version.h

EXPORT_SH_TXT=Export.$OS.sh.txt

# ===== Execution ===========================================================

if [ ! -d Binaries ]
then
	mkdir Binaries
fi

if [ ! -d Libraries ]
then
	mkdir Libraries
fi

./Clean.sh

./Make.sh
if [ $? -ne 0 ]
then
	echo ERROR  ./Make.sh  failed
	exit 10
fi

./Test.sh

if [ $? -ne 0 ]
then
	echo ERROR  Test.sh  failed
	exit 20
fi

$KMS_VERSION $VERSION_H CreatePackages.sh $EXPORT_SH_TXT
if [ $? -ne 0 ]
then
	echo ERROR  $KMS_VERSION $VERSION_H CreatePackages.sh $EXPORT_SH_TXT  failed
	exit 25
fi

./CreatePackages.sh
if [ $? -ne 0 ]
then
	echo ERROR  ./CreatePackages.sh  failed
	exit 30
fi

$KMS_VERSION -S $VERSION_H ./Export.sh

if [ $? -ne 0 ]
then
	echo ERROR  $KMS_VERSION -S $VERSION_H ./Export.sh  failed
	exit 40
fi

# ===== End =================================================================

echo OK
