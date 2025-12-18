#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    CreatePackages.sh
# Usage   ./CreatePackages.sh

# CODE REVIEW 2021-07-22 KMS - Martin Dubois, P.Eng.

echo Excuting  CreatePackages.sh  ...

# ===== Version =============================================================

# KmsVersion "PACKAGE_VERSION=" "\n" 13
PACKAGE_VERSION=1.0-21

# KmsVersion "VERSION=" "\n" 2
VERSION=1.0

# ===== Initialisation ======================================================

PACKAGE_NAME=zap-tracking_$PACKAGE_VERSION

# ===== Execution ===========================================================

if [ ! -d Packages ]
then
    mkdir Packages
fi

mkdir Packages/$PACKAGE_NAME
mkdir Packages/$PACKAGE_NAME/usr
mkdir Packages/$PACKAGE_NAME/usr/local
mkdir Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION
mkdir Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION/Binaries

cp _DocUser/Tracking.ReadMe.txt                   Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION
cp Binaries/ZT_Agent                              Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION/Binaries
cp Scripts/Disable.sh                             Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION
cp Scripts/Enable.sh                              Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION
cp ZT_Agent/_DocUser/Tracking.ZT_Agent.ReadMe.txt Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION
cp ZT_Agent/com.kms-quebec.zap.tracking.plist     Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION
cp ZT_Lib/_DocUser/Tracking.ZT_Lib.ReadMe.txt     Packages/$PACKAGE_NAME/usr/local/ZT-$VERSION

productbuild --root Packages/$PACKAGE_NAME/usr/local /Applications Packages/$PACKAGE_NAME.pkg

if [ 0 != $? ]
then
    echo ERROR  productbuild --root Packages/$PACKAGE_NAME/usr/local /Applications Packages/$PACKAGE_NAME.pkg  failed
    exit 10
fi

# ===== Cleanup =============================================================

rm -r Packages/$PACKAGE_NAME

# ===== End =================================================================

echo OK
