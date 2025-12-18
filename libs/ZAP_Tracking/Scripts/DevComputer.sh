#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Scripts/DevComputer.sh

echo Executing  Scripts/DevComputer.sh  ...

# ===== Execution ===========================================================

sudo apt install git
sudo apt install git-gui
sudo apt install python3-pip

python3 -m pip install cppyy --user

# ===== Instruction =========================================================

echo Do not forget to configure git by executing
echo     git config --global user.name "FirstName LastName"
echo     git config --global user.email user@domaine.com

# ===== End =================================================================

echo OK
exit 0
