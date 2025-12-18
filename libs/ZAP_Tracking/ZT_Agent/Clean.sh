#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Agent/Clean.sh
# Usage   ./Clean.sh

echo Executing  ZT_Agent/Clean.sh  ...

# ===== Execution ===========================================================

rm -f ../Binaries/ZT_Agent

rm -f *.o

# ===== End =================================================================

echo OK
exit 0
