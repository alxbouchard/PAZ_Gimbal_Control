
# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Python_Test/ZT_Phyton_Test.py
# Usage   python3 ZT_Python_Text_2.py

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
    print("Detected")

    lRet = lGimbal.Activate()
    print(lRet)

    lPosition = ZT.IGimbal.Position()

    lPosition.mAxis_deg[ZT.IGimbal.AXIS_PITCH] = 0.0
    lPosition.mAxis_deg[ZT.IGimbal.AXIS_ROLL ] = 0.0
    lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW  ] = 0.0

    for x in range(500):
        time.sleep(0.01)
        lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW  ] += 0.01
        lRet = lGimbal.Position_Set(lPosition)
        if lRet:
            print(lRet)

    for x in range(500):
        time.sleep(0.01)
        lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW  ] -= 0.01
        lRet = lGimbal.Position_Set(lPosition)
        if lRet:
            print(lRet)

    lGimbal.Debug()
    lGimbal.Release()

lSystem.Release()

print("PASSED!")
