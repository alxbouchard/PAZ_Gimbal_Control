
# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2020 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# File      KmsCopy/makefile

# CODE REVIEW 2020-10-26 KMS - Martin Dubois, P.Eng.

include ../User.mk

LIBRARIES = ../Libraries/KmsLib.a

OUTPUT = ../Binaries/KmsCopy

SOURCES =	KmsCopy.cpp

# ===== Rules / Regles ======================================================

.cpp.o:
	g++ -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Macros ==============================================================

OBJECTS = $(SOURCES:.cpp=.o)

# ===== Targets / Cibles ====================================================

$(OUTPUT) : $(OBJECTS) $(LIBRARIES)
	g++ -pthread -o $@ $^

depend:
	makedepend -Y $(INCLUDES) $(SOURCES)

# DO NOT DELETE

KmsCopy.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
KmsCopy.o: ../Includes/WindowsToLinux.h ../Includes/KmsTool.h
KmsCopy.o: ../Includes/KmsLib/Linux/Windows.h ../Common/Version.h
