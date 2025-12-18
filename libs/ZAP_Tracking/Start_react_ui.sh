#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Start_react_ui.sh
# Usage   ./Start_react_ui.sh

echo Executing  Start_react_ui.sh  ...

# ===== Execution ===========================================================

cd modules/react_ui

npm start
if [ $? -ne 0 ]
then
    echo ERROR  npm start  failed
    echo Execute ./Import.sh
fi

cd ../..

# ===== End =================================================================

echo OK
exit 0
