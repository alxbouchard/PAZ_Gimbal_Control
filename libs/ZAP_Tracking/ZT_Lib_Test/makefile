
# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT_Lib_Test/makefile

include ../User.mk

FRAMEWORKS = -framework IOKit -framework CoreFoundation

INCLUDES = -I ../Includes $(INCLUDE_IMPORT)

LIBRARIES = ../Libraries/ZT_Lib.a $(EthCAN_LIB_A)

OUTPUT = ../Binaries/ZT_Lib_Test

SOURCES =		    \
    ControlLink.cpp \
	Gamepad.cpp		\
    Gimbal.cpp      \
	System.cpp      \
	ZT_Lib_Test.cpp

# ===== Rules / Regles =======================================================

.cpp.o:
	g++ -c $(COMPILE_FLAGS) -o $@ $(INCLUDES) $<

# ===== Macros ===============================================================

OBJECTS = $(SOURCES:.cpp=.o)

# ===== Targets / Cibles =====================================================

$(OUTPUT) : $(OBJECTS) $(LIBRARIES)
	g++ -pthread $(FRAMEWORKS) -o $@ $^

depend:
	makedepend -Y $(INCLUDES) $(SOURCES)
