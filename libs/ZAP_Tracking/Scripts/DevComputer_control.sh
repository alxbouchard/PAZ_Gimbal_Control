#!/bin/sh

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Scripts/DevComputer_reac_ui.sh

echo Executing  Scripts/DevComputer_react_ui.sh  ...

# ===== Execution ===========================================================

python3 -m pip install cppyy --no-binary=cppyy-cling --user

# ===== End =================================================================

echo OK
exit 0
