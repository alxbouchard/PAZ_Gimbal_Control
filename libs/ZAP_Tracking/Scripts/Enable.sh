#~/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Scripts/Enable.sh

# CODE REVIEW 2021-07-22 KMS - Martin Dubois, P.Eng.

echo Executing  Enable.sh  ...

# ===== Initialisation ======================================================

DST_FOLDER=~/Library/LaunchAgents

# KmsVersion "ZT-" "/com" 2
FILE_PLIST=/Application/ZT-1.0/com.kms-quebec.zap.tracking.plist

# ===== Verification ========================================================

if [ ! -d $DST_FOLDER ]
then
    echo FATAL ERROR  $DST_FOLDER  does not exist
    exit 10
fi

if [ ! -f $FILE_PLIST ]
then
    echo FATAL ERROR  $FILE_PLIST  does not exist
    echo Corrupted package!
    exit 20
fi

# ===== Execution ===========================================================

cp $FILE_PLIST $DST_FOLDER
if [ 0 != $? ]
then
    echo ERROR  cp $FILE_PLIST $DST_FOLDER  failed
    exit 30
fi

# ===== End =================================================================

echo OK
