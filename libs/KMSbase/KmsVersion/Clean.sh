#!/bin/sh

# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2020 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# File      KmsVersion/Clean.sh
# Usage     ./Clean.sh

# CODE REVIEW 2020-10-26 KMS - Martin Dubois, P.Eng.

echo Executing  KmsVersion/Clean.sh  ...

# ===== Execution ===========================================================

rm -f ../Binaries/KmsVersion

rm -f *.o

# ===== End =================================================================

echo OK
