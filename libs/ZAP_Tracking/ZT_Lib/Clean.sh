#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Lib/Clean.sh
# Usage   ./Clean.sh

echo Executing  ZT_Lib/Clean.sh  ...

# ===== Execution ===========================================================

rm -f ../Libraries/ZT_Lib.a

rm -f *.o

# ===== End =================================================================

echo OK
exit 0
