
# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Lib/Tests/Base.py

import cppyy

cppyy.add_include_path('../Includes')
cppyy.include('ZT/ISystem.h')

from cppyy.gbl import ZT

cppyy.load_library('../Binaries/ZT')

# ISystem::Create
lSystem = ZT.ISystem.Create()
assert lSystem

# ISystem::Gimbals_Detect
lRet = lSystem.Gimbals_Detect()
assert ZT.ZT_OK == lRet

# ISystem::Gimbal_Find_IPv4
lGimbal = lSystem.Gimbal_Find_IPv4("1.2.3.4")
assert not lGimbal

# ISystem::Gimbal_Get
lGimbal = lSystem.Gimbal_Get(0)
assert not lGimbal

# ISystem::Release
lSystem.Release()

print("PASSED!")
