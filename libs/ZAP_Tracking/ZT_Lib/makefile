
# Author  KMS - Martin Dubois, P. Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Lib/makefile

include ../User.mk

INCLUDES = -I ../Includes $(INCLUDE_IMPORT)

OUTPUT = ../Libraries/ZT_Lib.a

SOURCES =		     \
    Atem.cpp         \
	BM/BMDSwitcherAPIDispatch.cpp \
	ControlLink.cpp  \
	DJI.cpp          \
    DJI_CRC.cpp      \
    DJI_Detector.cpp \
	DJI_Gimbal.cpp   \
	DJI_Transaction.cpp \
	Gamepad.cpp      \
	Gimbal.cpp       \
	IControlLink.cpp \
	IDetector.cpp    \
	IGamepad.cpp     \
	IGimbal.cpp      \
	ISystem.cpp      \
	OSX_Detector.cpp \
	OSX_Gamepad.cpp  \
	Result.cpp       \
	Stats.cpp        \
	System.cpp       \
	Thread.cpp       \
	Value.cpp

# ===== Rules / Regles =======================================================

.cpp.o:
	g++ -c $(COMPILE_FLAGS) -o $@ $(INCLUDES) $<

# ===== Macros ===============================================================

OBJECTS = $(SOURCES:.cpp=.o)

# ===== Targets / Cibles =====================================================

$(OUTPUT) : $(OBJECTS)
	ar r $@ $^
	ranlib $@

depend:
	makedepend -Y $(INCLUDES) $(SOURCES)
	# DO NOT DELETE

ISystem.o: Component.h System.h ../Includes/ZT/ISystem.h
ISystem.o: ../Includes/ZT/IObject.h
System.o: Component.h System.h ../Includes/ZT/ISystem.h
System.o: ../Includes/ZT/IObject.h
# DO NOT DELETE

Display.o: Component.h ../Includes/ZT/Result.h
DJI.o: Component.h DJI.h
DJI_CRC.o: Component.h DJI_CRC.h
DJI_Detector.o: Component.h ../Includes/ZT/Result.h DJI_Gimbal.h
DJI_Detector.o: ../Import/Includes/EthCAN/Device.h
DJI_Detector.o: ../Import/Includes/EthCAN_Result.h
DJI_Detector.o: ../Import/Includes/EthCAN_Types.h
DJI_Detector.o: ../Import/Includes/EthCAN/Object.h DJI.h Gimbal.h
DJI_Detector.o: ../Includes/ZT/IGimbal.h ../Includes/ZT/IObject.h Stats.h
DJI_Detector.o: DJI_Detector.h ../Import/Includes/EthCAN/System.h
DJI_Detector.o: ../Import/Includes/EthCAN_Protocol.h IDetector.h
DJI_Gimbal.o: Component.h ../Import/Includes/EthCAN/Display.h
DJI_Gimbal.o: ../Import/Includes/EthCAN_Protocol.h
DJI_Gimbal.o: ../Import/Includes/EthCAN_Result.h
DJI_Gimbal.o: ../Import/Includes/EthCAN_Types.h
DJI_Gimbal.o: ../Includes/ZT/Result.h DJI_CRC.h DJI_Gimbal.h
DJI_Gimbal.o: ../Import/Includes/EthCAN/Device.h
DJI_Gimbal.o: ../Import/Includes/EthCAN/Object.h DJI.h Gimbal.h
DJI_Gimbal.o: ../Includes/ZT/IGimbal.h ../Includes/ZT/IObject.h Stats.h
Gimbal.o: Component.h Gimbal.h ../Includes/ZT/IGimbal.h
Gimbal.o: ../Includes/ZT/IObject.h ../Includes/ZT/Result.h
ISystem.o: Component.h System.h ../Includes/ZT/ISystem.h
ISystem.o: ../Includes/ZT/IObject.h ../Includes/ZT/Result.h IDetector.h
ISystem.o: Gimbal.h ../Includes/ZT/IGimbal.h
Stats.o: Component.h ../Includes/ZT/Result.h Stats.h
System.o: Component.h DJI_Detector.h ../Import/Includes/EthCAN/System.h
System.o: ../Import/Includes/EthCAN_Result.h
System.o: ../Import/Includes/EthCAN_Protocol.h
System.o: ../Import/Includes/EthCAN_Types.h
System.o: ../Import/Includes/EthCAN/Object.h IDetector.h Gimbal.h
System.o: ../Includes/ZT/IGimbal.h ../Includes/ZT/IObject.h
System.o: ../Includes/ZT/Result.h System.h ../Includes/ZT/ISystem.h
