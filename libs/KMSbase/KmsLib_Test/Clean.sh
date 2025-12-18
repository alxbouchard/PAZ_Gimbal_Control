#!/bin/sh

# Author   KMS - Martin Dubois, ing.
# Product  KmsBase
# File     KmsLib_Test/Clean.sh
# Usage    ./Clean.sh

# CODE REVIEW  2019-076-19  KMS - Martin Dubois, ing.

echo Executing  KmsLib_Test/Clean.sh  ...

# ===== Execution ===========================================================

rm -f ../Linux_Windows_Test0.txt

rm -f ../Binaries/KmsLib_Test

rm -f *.o

# ===== End =================================================================

echo OK
