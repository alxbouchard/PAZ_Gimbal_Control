#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Lib/Test.sh

echo Executing  ZT_Lib/Test.sh

# ===== Initialisation ======================================================

BASE_PY=Tests/Base.py

# ===== Execution ===========================================================

python3 $BASE_PY
if [ 0 -ne $? ]
then
    echo ERROR  python3 $BASE_PY  failed
    exit 10
fi

# ===== End =================================================================

echo OK
exit 0
