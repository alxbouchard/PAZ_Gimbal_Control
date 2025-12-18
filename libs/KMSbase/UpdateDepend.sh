#!/bin/sh

# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2020 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# File      UpdateDepend.sh
# Usage     ./UpdateDepend.sh

# CODE REVIEW 2020-10-26 KMS - Martin Dubois, P.Eng.

echo Executing  UpdateDepend.sh  ....

# ===== Execution ===========================================================

echo Updating dependencies for KmsLib
cd KmsLib
make depend
if [ 0 != $? ] ; then
    echo ERROR  KmsLib - make depend  failed
    exit 10
fi
cd ..

echo Updating dependencies for KmsCopy
cd KmsCopy
make depend
if [ 0 != $? ] ; then
    echo ERROR  KmsCopy - make depend  failed
    exit 15
fi
cd ..

echo Updating dependencies for KmsVersion
cd KmsVersion
make depend
if [ 0 != $? ] ; then
    echo ERROR  KmsVersion - make depend  failed
    exit 18
fi
cd ..

echo Updating dependencies for KmsLib_Test ...
cd KmsLib_Test
make depend
if [ 0 != $? ] ; then
    echo ERROR  KmsLib_Test - make depend  failed
    exit 20
fi
cd ..

# ===== End =================================================================

echo OK
