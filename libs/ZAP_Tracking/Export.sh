#!/bin/sh

# Author  KMS - Martin Dubois, P. Eng.
# Client  ZAP
# Product Tracking
# File    Export.sh
# Usage   ./Export.sh

# CODE REVIEW

# TODO Export
#      Use KmsCopy

echo Executing  Export.sh $1 $2  ...

# ===== Initialisation ======================================================

EXPORT=~/Export

EXP_CLIENT=$EXPORT/ZAP

EXP_PROD=$EXP_CLIENT/Tracking

DST_FOLDER=$EXP_PROD/$1_$2_Darwin

# ===== Execution ===========================================================

if [ ! -d $EXPORT ]
then
    mkdir $EXPORT
fi

if [ ! -d $EXP_CLIENT ]
then
    mkdir $EXP_CLIENT
fi

if [ ! -d $EXP_PROD ]
then
    mkdir $EXP_PROD
fi

mkdir $DST_FOLDER
if [ 0 != $? ] ; then
    echo ERROR  mkdir $DST_FOLDER  failed
    exit 10
fi

mkdir $DST_FOLDER/Packages

cp _DocUser/Tracking.ReadMe.txt                   $DST_FOLDER
cp ZT_Agent/_DocUser/Tracking.ZT_Agent.ReadMe.txt $DST_FOLDER
cp ZT_Lib/_DocUser/Tracking.ZT_Lib.ReadMe.txt     $DST_FOLDER
# KmsVersion "zap-tracking_" ".pkg" 13
cp Packages/zap-tracking_1.0-21.pkg                $DST_FOLDER/Packages

# ===== End =================================================================

echo OK
