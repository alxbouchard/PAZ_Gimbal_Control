#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Lib_Test/Clean.sh
# Usage   ./Clean.sh

echo Executing  ZT_Lib_Test/Clean.sh  ...

# ===== Execution ===========================================================

rm -f ../Binaries/ZT_Lib_Test

rm -f *.o

# ===== End =================================================================

echo OK
exit 0
