
# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Python_Test/ZT_Phyton_Test.py
# Usage   python3 ZT_Python_Text.py

import cppyy
import time

cppyy.add_include_path('../Includes')
cppyy.include('ZT/IGimbal.h')
cppyy.include('ZT/ISystem.h')

from cppyy.gbl import ZT

cppyy.load_library('../Binaries/ZT')

lSystem = ZT.ISystem.Create()

print("1. Detecting...")

lSystem.Gimbals_Detect()

lGimbal = lSystem.Gimbal_Get(0)
if lGimbal:

    print("2. Configuring...")

    lRet = lGimbal::Activate()
    print(lRet)

    lConfig = ZT.IGimbal.Config()

    lGimbal.Config_Get(lConfig)

    lConfig.mAxis[ZT.IGimbal.AXIS_PITCH].mMin_deg = -180.0
    lConfig.mAxis[ZT.IGimbal.AXIS_PITCH].mMax_deg =  180.0
    lConfig.mAxis[ZT.IGimbal.AXIS_ROLL ].mMin_deg = -180.0
    lConfig.mAxis[ZT.IGimbal.AXIS_ROLL ].mMax_deg =  180.0
    lConfig.mAxis[ZT.IGimbal.AXIS_YAW  ].mMin_deg = -180.0
    lConfig.mAxis[ZT.IGimbal.AXIS_YAW  ].mMax_deg =  180.0

    lRet = lGimbal.Config_Set(lConfig) # Tx 3, Rx 3
    print(lRet)

    print("3. Retrieving position...")

    lPosition = ZT.IGimbal.Position()

    lRet = lGimbal.Position_Get(lPosition) # Tx 3, Rx 3
    print(lRet)
    print(lPosition.mAxis_deg[ZT.IGimbal.AXIS_PITCH])
    print(lPosition.mAxis_deg[ZT.IGimbal.AXIS_ROLL])
    print(lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW])

    for x in range(2):
        print("4. Homing...")

        lPosition.mAxis_deg[ZT.IGimbal.AXIS_PITCH] = 0.0
        lPosition.mAxis_deg[ZT.IGimbal.AXIS_ROLL ] = 0.0
        lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW  ] = 0.0

        lRet = lGimbal.Position_Set(lPosition) # 3 frame - 3 frame
        print(lRet)
        time.sleep(2)

        print("5. Moving...")

        lPosition.mAxis_deg[ZT.IGimbal.AXIS_PITCH] = 10.0
        lPosition.mAxis_deg[ZT.IGimbal.AXIS_ROLL ] = 10.0
        lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW  ] = 10.0

        lRet = lGimbal.Position_Set(lPosition)
        print(lRet)
        time.sleep(2)

    print("6. Homing...")

    lPosition.mAxis_deg[ZT.IGimbal.AXIS_PITCH] = 0.0
    lPosition.mAxis_deg[ZT.IGimbal.AXIS_ROLL ] = 0.0
    lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW  ] = 0.0

    lRet = lGimbal.Position_Set(lPosition)
    print(lRet)
    time.sleep(2)

    print("7. Speed")

    lSpeed = ZT.IGimbal.Speed()

    lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_PITCH] = 5.0
    lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_ROLL ] = 5.0
    lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_YAW  ] = 5.0

    lRet = lGimbal.Speed_Set(lSpeed)
    print(lRet)
    time.sleep(2)

    lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_PITCH] = 0.0
    lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_ROLL ] = 0.0
    lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_YAW  ] = 0.0

    lRet = lGimbal.Speed_Set(lSpeed)
    print(lRet)

    print("8. Homing...")

    lPosition.mAxis_deg[ZT.IGimbal.AXIS_PITCH] = 0.0
    lPosition.mAxis_deg[ZT.IGimbal.AXIS_ROLL ] = 0.0
    lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW  ] = 0.0

    lRet = lGimbal.Position_Set(lPosition)
    print(lRet)
    time.sleep(2)

    print("9. Releasing...")

    lGimbal.Debug()
    lGimbal.Release()

lSystem.Release()

print("PASSED!")
