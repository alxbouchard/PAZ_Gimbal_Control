
# Author   KMS - Martin Dubois, ing.
# Product  KmsBase
# File     KmsLib_Test/makefile

include ../User.mk

LIBRARIES = ../Libraries/KmsLib.a

OUTPUT = ../Binaries/KmsLib_Test

SOURCES =	CmdLineParser.cpp	\
			DebugLog.cpp		\
			DriverHandle.cpp    \
			Dump.cpp			\
			Exception.cpp		\
			File.cpp			\
			FileHandle.cpp      \
			IgnoreList.cpp      \
			KmsLib_Test.cpp		\
			Linux/Linux_Windows.cpp	\
			MemTester.cpp		\
			Ring.cpp		\
			RLE.cpp				\
			String.cpp			\
			TextFile.cpp		\
			ToolBase.cpp		\
			ThreadBase.cpp      \
			ValueVector.cpp		\
			Walker.cpp

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

CmdLineParser.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
CmdLineParser.o: ../Includes/WindowsToLinux.h
CmdLineParser.o: ../Includes/KmsLib/CmdLineParser.h
CmdLineParser.o: ../Includes/KmsLib/Exception.h ../Includes/KmsTest.h
DebugLog.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
DebugLog.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/DebugLog.h
DebugLog.o: ../Includes/KmsLib/Exception.h ../Includes/KmsLib/TextFile.h
DebugLog.o: ../Includes/KmsTest.h
DriverHandle.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
DriverHandle.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
DriverHandle.o: ../Includes/KmsLib/DriverHandle.h
DriverHandle.o: ../Includes/KmsLib/FileHandle.h ../Includes/KmsTest.h
Dump.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
Dump.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Dump.h
Dump.o: ../Includes/KmsTest.h
Exception.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
Exception.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
Exception.o: ../Includes/KmsTest.h
File.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
File.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
File.o: ../Includes/KmsLib/File.h ../Includes/KmsTest.h
FileHandle.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
FileHandle.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
FileHandle.o: ../Includes/KmsLib/FileHandle.h ../Includes/KmsTest.h
IgnoreList.o: ../Includes/KmsLib/Exception.h ../Includes/KmsTest.h
IgnoreList.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
IgnoreList.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/IgnoreList.h
KmsLib_Test.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
KmsLib_Test.o: ../Includes/WindowsToLinux.h ../Includes/KmsTest.h
Linux/Linux_Windows.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
Linux/Linux_Windows.o: ../Includes/WindowsToLinux.h
Linux/Linux_Windows.o: ../Includes/KmsLib/Linux/Windows.h
Linux/Linux_Windows.o: ../Includes/KmsTest.h
MemTester.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
MemTester.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/MemTester.h
MemTester.o: ../Includes/KmsTest.h
Ring.o: ../Includes/KmsTest.h ../Includes/KmsBase.h ../Includes/SafeAPI.h
Ring.o: ../Includes/WindowsToLinux.h ../Includes/Ring.h
RLE.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
RLE.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
RLE.o: ../Includes/KmsLib/RLE.h ../Includes/KmsTest.h
String.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
String.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
String.o: ../Includes/KmsLib/String.h ../Includes/KmsTest.h
TextFile.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
TextFile.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
TextFile.o: ../Includes/KmsLib/File.h ../Includes/KmsLib/TextFile.h
TextFile.o: ../Includes/KmsTest.h
ToolBase.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
ToolBase.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
ToolBase.o: ../Includes/KmsLib/ToolBase.h ../Includes/KmsTest.h
ThreadBase.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
ThreadBase.o: ../Includes/WindowsToLinux.h ../Includes/KmsTest.h
ThreadBase.o: ../Includes/KmsLib/Exception.h ../Includes/KmsLib/ThreadBase.h
ValueVector.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
ValueVector.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/ValueVector.h
ValueVector.o: ../Includes/KmsTest.h
Walker.o: ../Includes/KmsBase.h ../Includes/SafeAPI.h
Walker.o: ../Includes/WindowsToLinux.h ../Includes/KmsLib/Exception.h
Walker.o: ../Includes/KmsTest.h ../Includes/KmsLib/Linux/Windows.h
Walker.o: ../Includes/KmsLib/Walker.h
