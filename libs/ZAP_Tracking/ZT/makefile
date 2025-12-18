
# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    ZT/makefile

include ../User.mk

FRAMEWORKS = -framework IOKit -framework CoreFoundation

INCLUDES = -I ../Includes

LIBRARIES = ../Libraries/ZT_Lib.a

OUTPUT = ../Binaries/ZT.so

ifeq ($(shell uname), Darwin)
	SYSTEM_CREATE = __ZN2ZT7ISystem6CreateEv
endif

ifeq ($(shell uname), Linux)
	SYSTEM_CREATE = _ZN2ZT7ISystem6CreateEv
endif

# ===== Targets / Cibles =====================================================

$(OUTPUT) : $(LIBRARIES) $(EthCAN_LIB_A)
	@echo $(OS)
	g++ -shared -pthread -Wl,-u -Wl,$(SYSTEM_CREATE) $(FRAMEWORKS) -o $@ $^

depend:
	makedepend -Y $(INCLUDES)

# DO NOT DELETE
