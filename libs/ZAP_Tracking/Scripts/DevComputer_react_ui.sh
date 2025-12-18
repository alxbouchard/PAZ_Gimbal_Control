#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Scripts/DevComputer_reac_ui.sh

echo Executing  Scripts/DevComputer_react_ui.sh  ...

# ===== Execution ===========================================================

sudo apt install curl
sudo apt install npm

curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
if [ $? -ne 0 ]
then
    echo ERROR  curl  failed
    exit 10
fi

sudo apt install nodejs

# ===== End =================================================================

echo OK
exit 0
