#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT/Clean.sh
# Usage   ./Clean.sh

echo Executing  ZT/Clean.sh  ...

# ===== Execution ===========================================================

rm -f ../Binaries/ZT.so

rm -f *.o

# ===== End =================================================================

echo OK
exit 0
