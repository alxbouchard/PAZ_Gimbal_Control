#!/bin/sh

# Author  KMS - Martin Dubois, P. Eng.
# Client  ZAP
# Product Tracking
# File    Import.sh
# Usage   ./Import.sh

echo Executing  Import.sh  ...

# ===== Dependencies =========================================================

EXPORT_FOLDER=~/Export

OS=`uname`

PROCESSOR=`uname -p`

if [ "$OS" = "Darwin" ]
then
    if [ "arm" = "$PROCESSOR" ]
    then
        EthCAN=$EXPORT_FOLDER/EthCAN/1.1.10_Darwin_arm
        KMS_BASE=$EXPORT_FOLDER/KmsBase/3.0.34_Darwin_arm64
    else
        EthCAN=$EXPORT_FOLDER/EthCAN/1.1.6_Darwin
        KMS_BASE=$EXPORT_FOLDER/KmsBase/3.0.32_Darwin
    fi
fi

if [ "$OS" = "Linux" ]
then
    if [ "$PROCESSOR" = "aarch64" ]
    then
        EthCAN=$EXPORT_FOLDER/EthCAN/1.0.9_KI_Linux
        KMS_BASE=$EXPORT_FOLDER/KmsBase/3.0.31_Linux
    fi

    if [ "$PROCESSOR" = "x86_64" ]
    then
        EthCAN=$EXPORT_FOLDER/EthCAN/1.0.8_KI_Linux
        KMS_BASE=$EXPORT_FOLDER/KmsBase/3.0.28_Linux
    fi
fi

if [ ! -d $EthCAN ]
then
    EthCAN=/Applications/EthCAN-1.0
fi

if [ ! -d $KMS_BASE ]
then
    KMS_BASE=/Applications/KmsBase-3.0
fi

# ===== Constants ============================================================

DST_FOLDER=$PWD/Import

# ===== Verification =========================================================

if [ ! -d $EthCAN ]
then
    echo FATAL ERROR  $EthCAN  does not exist
    exit 10
fi

if [ ! -d $KMS_BASE ]
then
    echo FATAL ERROR  $KMS_BASE  does not exist
    exit 20
fi

# ===== Execution ===========================================================

if [ ! -d Binaries ]
then
    mkdir Binaries
fi

if [ ! -d Libraries ]
then
    mkdir Libraries
fi

if [ ! -d $DST_FOLDER ]
then
    mkdir $DST_FOLDER
fi

cd $KMS_BASE

./Import.sh $DST_FOLDER

if [ 0 -ne $? ]
then
    echo ERROR  ./Import.sh $DST_FOLDER  failed
    exit 50
fi

cd $EthCAN

./Import.sh $DST_FOLDER

if [ 0 -ne $? ]
then
    echo ERROR  ./Import.sh $DST_FOLDER  failed
    exit 60
fi

# ===== End =================================================================

echo OK
exit 0
