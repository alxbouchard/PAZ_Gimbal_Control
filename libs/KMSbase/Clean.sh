#!/bin/sh

# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2020 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# File      Clean.sh
# Usage     ./Clean.sh

# CODE REVIEW 2019-10-26 KMS - Martin Dubois, P.Eng.

echo Executing  Clean.sh  ...

# ===== Execution ===========================================================

cd KmsLib
./Clean.sh
cd ..

cd KmsCopy
./Clean.sh
cd ..

cd KmsLib_Test
./Clean.sh
cd ..

cd KmsVersion
./Clean.sh
cd ..

# ===== End =================================================================

echo OK
