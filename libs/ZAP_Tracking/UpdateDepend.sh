#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    UpdateDepend.sh
# Usage   ./UpdateDepend.sh

# CODE REVIEW

echo Excuting  UpdateDepend.sh  ...

# ===== Initialisation ======================================================

RESULT=0

# ===== Functions ===========================================================

UpdateDepend()
{
    cd $1
    make depend > /dev/null 2> /dev/null
    if [ 0 != $? ] ; then
        echo ERROR  $1 - make depend  failed
        RESULT=10
    fi
    cd ..
}

# ===== Execution ===========================================================

UpdateDepend ZT
UpdateDepend ZT_Lib

# ===== End =================================================================

if [ 0 = $RESULT ] ; then
    echo OK
fi

exit $RESULT
