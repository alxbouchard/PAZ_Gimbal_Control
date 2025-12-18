
# Author    KMS - Martin Dubois, P.Eng.
# Copyright (C) 2020 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KmsBase
# File      KmsVersion/makefile

# CODE REVIEW 2020-10-26 KMS - Martin Dubois, P.Eng.

include ../User.mk

LIBRARIES = ../Libraries/KmsLib.a

OUTPUT = ../Binaries/KmsVersion

SOURCES = KmsVersion.cpp     \
          Version.cpp        \
		  VersionAndType.cpp \
		  VersionInfo.cpp

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

KmsVersion.o: Component.h ../Includes/KmsBase.h ../Includes/SafeAPI.h
KmsVersion.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/CmdLineParser.h
KmsVersion.o: ../Includes/KmsLib/Exception.h
KmsVersion.o: ../Includes/KmsLib/Linux/Windows.h ../Common/Version.h
KmsVersion.o: VersionAndType.h Version.h VersionInfo.h
Version.o: Component.h ../Includes/KmsBase.h ../Includes/SafeAPI.h
Version.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
Version.o: Version.h
VersionAndType.o: Component.h ../Includes/KmsBase.h ../Includes/SafeAPI.h
VersionAndType.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
VersionAndType.o: VersionAndType.h Version.h
VersionInfo.o: Component.h ../Includes/KmsBase.h ../Includes/SafeAPI.h
VersionInfo.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
VersionInfo.o: VersionInfo.h Version.h
