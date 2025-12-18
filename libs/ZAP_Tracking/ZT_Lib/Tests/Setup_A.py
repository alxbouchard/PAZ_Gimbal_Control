
# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Lib/Tests/Base.py

import cppyy

cppyy.add_include_path('../Includes')
cppyy.include('ZT/IGimbal.h')
cppyy.include('ZT/ISystem.h')

from cppyy.gbl import ZT

cppyy.load_library('../Binaries/ZT')

lSystem = ZT.ISystem.Create()
assert(lSystem)

# ISystem::Gimbals_Detect
lRet = lSystem.Gimbals_Detect()
assert lRet == ZT.ZT_OK

# ISystem::Gimbal_Get
lGimbal = lSystem.Gimbal_Get(0)
assert lGimbal

# IGimbal::Release - When the gimbal is not activated
lGimbal.Release()

lRet = lSystem.Gimbals_Detect()
assert lRet == ZT.ZT_OK

lGimbal = lSystem.Gimbal_Get(0)
assert lGimbal

# IGimbal::Position_Set - When the device is not activated
lPosition = ZT.IGimbal.Position()
lRet = lGimbal.Position_Set(lPosition)
assert lRet == ZT.ZT_ERROR_STATE

# IGimbal::Speed_Set - When the device is not activated
lSpeed = ZT.IGimbal.Speed()
lRet = lGimbal.Speed_Set(lSpeed)
assert lRet == ZT.ZT_ERROR_STATE

# IGimbal::Speed_Stop - When the device is not activated
lRet = lGimbal.Speed_Stop()
assert lRet == ZT.ZT_ERROR_STATE

# IGimbal::Activate
lRet = lGimbal.Activate()
assert lRet == ZT.ZT_OK

# IGimbal::Activate - When the device is already activated
lRet = lGimbal.Activate()
assert lRet == ZT.ZT_ERROR_RECEIVE

# IGimbal::Config_Get
lConfig = ZT.IGimbal.Config()
lGimbal.Config_Get(lConfig)

# IGimbal::Config_Set
lRet = lGimbal.Config_Set(lConfig)
assert lRet == ZT.ZT_OK

# IGimbal::Config_Set - When a speed is to small
lConfig.mAxis[ZT.IGimbal.AXIS_YAW].mSpeed_deg_s = 0.0
lRet = lGimbal.Config_Set(lConfig)
assert lRet == ZT.ZT_ERROR_SPEED

# IGimbal::Config_Set - When a min angle is to small
lConfig.mAxis[ZT.IGimbal.AXIS_YAW].mMax_deg = - 361.0
lRet = lGimbal.Config_Set(lConfig)
assert lRet == ZT.ZT_ERROR_ANGLE_MIN

# IGimbal::Config_Set - When a max angle is to large
lConfig.mAxis[ZT.IGimbal.AXIS_YAW].mMax_deg = 361.0
lRet = lGimbal.Config_Set(lConfig)
assert lRet == ZT.ZT_ERROR_ANGLE_MAX

# IGimbal::Position_Get
lRet = lGimbal.Position_Get(lPosition)
print(lRet)
assert lRet == ZT.ZT_OK

# IGimbal::Position_Set
lRet = lGimbal.Position_Set(lPosition)
assert lRet == ZT.ZT_OK

# IGimbal::Position_Set - When the angle is too large
lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW] = 181.0
lRet = lGimbal.Position_Set(lPosition)
assert lRet == ZT.ZT_ERROR_ANGLE_MAX

# IGimbal::Position_Set - When the angle is too small
lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW] = - 181.0
lRet = lGimbal.Position_Set(lPosition)
assert lRet == ZT.ZT_ERROR_ANGLE_MIN

# IGimbal::Speed_Set
lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_YAW] = 0.1
lRet = lGimbal.Speed_Set(lSpeed)
assert lRet == ZT.ZT_OK

# IGimbal::Position_Set - When the gimbal is moving
lPosition.mAxis_deg[ZT.IGimbal.AXIS_YAW] = 0.0
lRet = lGimbal.Position_Set(lPosition)
assert lRet == ZT.ZT_OK

# IGimbal::Speed_Set - When the speed is not valid
lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_YAW] = 1000.0
lRet = lGimbal.Speed_Set(lSpeed)
assert lRet == ZT.ZT_ERROR_SPEED_MAX

# IGimbal::Speed_Set - When the speed is not valid
lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_YAW] = - 1000.0
lRet = lGimbal.Speed_Set(lSpeed)
assert lRet == ZT.ZT_ERROR_SPEED_MIN

lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_YAW] = 0.1
lRet = lGimbal.Speed_Set(lSpeed)
assert lRet == ZT.ZT_OK

# IGimbal::Speed_Set - When the gimbal is moving
lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_YAW] = 0.0
lRet = lGimbal.Speed_Set(lSpeed)
assert lRet == ZT.ZT_OK

# IGimbal::Speed_Stop - When the gimbal is not moving
lRet = lGimbal.Speed_Stop()
assert lRet == ZT.ZT_OK

lSpeed.mAxis_deg_s[ZT.IGimbal.AXIS_YAW] = 0.1
lRet = lGimbal.Speed_Set(lSpeed)
assert lRet == ZT.ZT_OK

# IGimbal::Speed_Stop
lRet = lGimbal.Speed_Stop()
assert lRet == ZT.ZT_OK

# IGimbal::Debug
lGimbal.Debug()

# IGimbal::Release
lGimbal.Release()

lSystem.Release()

print("PASSED!")
