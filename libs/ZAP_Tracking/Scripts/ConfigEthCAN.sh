#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Scripts/ConfigEthCAN.sh

echo Executing  ConfigEthCAN.sh  ...

# ===== Initialisation ======================================================

EthCAN_TOOL=../Import/Binaries/EthCAN_Tool

# ===== Verification ========================================================

if [ ! -x $EthCAN_TOOL ] ; then
    echo FATAL ERROR  $EthCAN_TOOL  does not exist
    exit 10
fi

# ===== Execution ===========================================================

$EthCAN_TOOL "Execute=ExecuteScript ConfigEthCAN.sh.txt"
if [ 0 != $? ] ; then
    echo ERROR  $EthCAN_TOOL "Execute=ExecuteScript ConfigEthCAN.sh.txt"  failed
    exit 20
fi

# ===== End =================================================================

echo OK
exit 0
