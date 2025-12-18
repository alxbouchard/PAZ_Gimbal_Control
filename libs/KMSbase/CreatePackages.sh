#!/bin/sh

# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2020-2021 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# Files     CreatePackages.sh

# CODE REVIEW 2021-08-17 KMS - Martin Dubois, P.Eng.

echo Executing  CreatePakcages.sh

# ===== Version =============================================================

OS=`uname`

# KmsVersion "PACKAGE_VERSION=" "\n" 13
PACKAGE_VERSION=3.0-34

# KmsVersion "VERSION=" "\n" 2
VERSION=3.0

# ===== Initialisation ======================================================

PACKAGE_NAME=kms-kmsbase_$PACKAGE_VERSION

# ===== Execution ===========================================================

echo Creating kms-kmsbase ...

mkdir Packages
mkdir Packages/$PACKAGE_NAME
mkdir Packages/$PACKAGE_NAME/usr
mkdir Packages/$PACKAGE_NAME/usr/local
mkdir Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
mkdir Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Binaries
mkdir Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Includes
mkdir Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Includes/KmsLib
mkdir Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Includes/KmsLib/Linux
mkdir Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Includes/KmsLib/Windows
mkdir Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Libraries

cp _DocUser/KmsBase.ReadMe.txt                       Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp Binaries/KmsCopy                                  Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Binaries
cp Binaries/KmsVersion                               Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Binaries
cp Includes/*.h                                      Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Includes
cp Includes/KmsLib/*.h                               Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Includes/KmsLib
cp Includes/KmsLib/Linux/*.h                         Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Includes/KmsLib/Linux
cp Includes/KmsLib/Windows/*.h                       Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION/Includes/KmsLib/Windows
cp KmsCopy/_DocUser/KmsBase.KmsCopy.ReadMe.txt       Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp KmsLib/_DocUser/KmsBase.KmsLib.ReadMe.txt         Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp KmsVersion/_DocUser/KmsBase.KmsVersion.ReadMe.txt Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp Scripts/Import.sh                                 Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp Scripts/Import.sh.txt                             Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp Scripts/Import.txt                                Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp DoxyFile_*.txt                                    Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp License.txt                                       Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION
cp RunDoxygen.sh                                     Packages/$PACKAGE_NAME/usr/local/KmsBase-$VERSION

if [ "Darwin" = "$OS" ]
then
	productbuild --root Packages/$PACKAGE_NAME/usr/local /Applications Packages/$PACKAGE_NAME.pkg
	if [ 0 != $? ]
	then
		echo ERROR  productbuild --root Packages/$PACKAGE_NAME/usr/local /Applications Packages/$PACKAGE_NAME.pkg  failed
		exit 5
	fi
fi

if [ "Linux" = "$OS" ]
then
	mkdir Packages/$PACKAGE_NAME/DEBIAN

	cp Scripts/control Packages/$PACKAGE_NAME/DEBIAN/control

	dpkg-deb --build Packages/$PACKAGE_NAME
	if [ $? -ne 0 ]
	then
		echo ERROR  dpkg-deb --build Packages/$PACKAGE_NAME  failed
		exit 10
	fi
fi

rm -r Packages/$PACKAGE_NAME

# ===== End =================================================================

echo OK
