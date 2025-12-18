#~/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Scripts/Disable.sh

# CODE REVIEW 2021-07-22 KMS - Martin Dubois, P.Eng.

echo Executing  Disable.sh  ...

# ===== Initialisation ======================================================

FILE_PLIST=~/Library/LaunchAgents/com.kms-quebec.zap.tracking.plist

# ===== Verification ========================================================

if [ ! -f $FILE_PLIST ]
then
    echo FATAL ERROR  $FILE_PLIST  does not exist
    echo Not enabled!
    exit 10
fi

# ===== Execution ===========================================================

rm $FILE_PLIST
if [ 0 != $? ]
then
    echo ERROR  rm $FILE_PLIST  failed
    exit 20
fi

# ===== End =================================================================

echo OK
