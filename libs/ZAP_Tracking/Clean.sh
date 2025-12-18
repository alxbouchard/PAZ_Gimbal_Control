#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Clean.sh
# Usage   ./Clean.sh

# CODE REVIEW

echo Excuting  Clean.sh  ...

# ===== Functions ===========================================================

Clean()
{
    cd $1
    ./Clean.sh
    cd ..
}

# ===== Execution ===========================================================

Clean ZT
Clean ZT_Agent
Clean ZT_Lib
Clean ZT_Lib_Test

# ===== End =================================================================

echo OK
exit 0
