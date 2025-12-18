#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Test.sh
# Usage   ./Test.sh

echo Excuting  Test.sh  ...

# ===== Execution ===========================================================

./Binaries/ZT_Lib_Test
if [ 0 -ne $? ]
then
    echo ERROR  ./Binaries/ZT_Lib_Test  failed
    exit 10
fi

# ===== End =================================================================

echo OK
