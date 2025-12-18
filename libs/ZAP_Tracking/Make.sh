#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Make.sh
# Usage   ./Make.sh

# CODE REVIEW

echo Excuting  Make.sh  ...

# ===== Functions ===========================================================

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

Make ZT_Lib
Make ZT
Make ZT_Agent
Make ZT_Lib_Test

# ===== End =================================================================

echo OK
