#!/bin/sh

# Author   KMS - Martin Dubois, ing.
# Product  KmsBase
# File     KmsLib/Clean.sh
# Usage    ./Clean.sh

# CODE REVIEW  2019-07-19  KMS - Martin Dubois, ing.

echo Executing  KmsLib/Clean.sh  ...

# ===== Execution ===========================================================

rm -f ../Libraries/KmsLib.a

rm -f *.o

# ===== End =================================================================

echo OK
