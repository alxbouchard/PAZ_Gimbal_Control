#!/bin/bash

# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    Scripts/Start.sh

echo Executing  Start.sh  ...

# ===== Configuration =======================================================

CONTROL_MODULE=/home/zap/ZAP/Tracking/modules/control
LOGIC_MODULE=/home/zap/logic_module-develop
VISION_MODULE=/home/zap/vision_module-master

# ===== Verification ========================================================

if [ ! -d $CONTROL_MODULE ]
then
    echo ERROR  $CONTROL_MODULE  does not exist
    exit 10
fi

if [ ! -d $LOGIC_MODULE ]
then
    echo ERROR  $LOGIC_MODULE  does not exist
    exit 20
fi

if [ ! -d $VISION_MODULE ]
then
    echo ERROR  $VISION_MODULE  does not exist
    exit 30
fi

# ===== Execution ===========================================================

echo Copying configuration files ...

# TODO Config
#      Automaticaly generate the 3 configurations files

cp Control.yaml $CONTROL_MODULE/config_default.yaml
cp Logic.yaml   $LOGIC_MODULE/config_default.yaml
cp Vision.yaml  $VISION_MODULE/config_default.yaml

echo Starting the vision module ...

cd $VISION_MODULE
python3 app.py --cam_width 1280 --cam_height 720 --divide_image 1 --show_local_video 1 --scale_factor 0.5 --verbose 0 > Vision.out &

echo Starting the logic_module ...

cd $LOGIC_MODULE
# python3 -m venv venv
# source venv/bin/activate
python3 app.py -t 1 -tr 1 > Logic.out &

echo Starting the control_module ...

cd $CONTROL_MODULE
# python3 -m venv venv
# source venv/bin/activate
python3 app.py &

# ===== End =================================================================

echo OK
exit 0
