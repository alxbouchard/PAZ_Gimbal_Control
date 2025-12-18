#!/bin/sh

# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2021 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# File      Export.sh

# CODE REVIEW 2020-08-17 KMS - Martin Dubois, P.Eng.

echo  Executing Export.sh $1 ...

# ===== Initialisation ======================================================

OS=`uname`

EXPORT=~/Export

EXP_PROD=$EXPORT/KmsBase

DST=$EXP_PROD/$1_$OS

EXPORT_SH_TXT=Export.$OS.sh.txt

KMS_COPY=Binaries/KmsCopy

# ===== Verification ========================================================

if [ "$1" = "" ] ; then
    echo  USER ERROR  Invalid command line
    echo  Usage  ./Export.sh {Ma.Mi.Bu_Type}
    exit 10
fi

if test -d $DST ; then
    echo  USER ERROR  $DST  already exist
    exit 20
fi

if [ ! -x $KMS_COPY ] ; then
    echo  FATAL ERROR  $KMS_COPY  does not exist
    echo  Compile before exporting
    exit 30
fi

# ===== Execution ===========================================================

if [ ! -d $EXPORT ]
then
    mkdir $EXPORT
fi

if [ ! -d %EXP_PROD ]
then
    mkdir $EXP_PROD
fi

$KMS_COPY . $DST Export.txt
if [ 0 != $? ] ; then
    echo ERROR  $KMS_COPY . $DST Export.txt  failed
    exit 40
fi

$KMS_COPY . $DST $EXPORT_SH_TXT
if [ 0 != $? ] ; then
    echo ERROR  $KMS_COPY . $DST $EXPORT_SH_TXT  failed
    exit 50
fi

# ===== End =================================================================

echo  OK!
