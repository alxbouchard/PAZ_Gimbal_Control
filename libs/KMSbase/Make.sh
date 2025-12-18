#!/bin/sh

# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2020-2021 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# File      Make.sh
# Usage     ./Make.sh

# CODE REVIEW

echo Executing  Make.sh  ....

# ===== Function ============================================================

Make()
{
    cd $1
    echo Building $1 ...
    make
    if [ 0 != $? ] ; then
        echo ERROR  $1 - make  failed
        exit 10
    fi
    cd ..
}

# ===== Execution ===========================================================

Make KmsLib
Make KmsCopy
Make KmsVersion
Make KmsLib_Test

# ===== End =================================================================

echo OK
