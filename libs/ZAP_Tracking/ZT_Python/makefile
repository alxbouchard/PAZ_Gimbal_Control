# ZT_Python Makefile
# Compile the C wrapper for Python ctypes

CXX = g++
CXXFLAGS = -std=c++11 -fPIC -Wall
INCLUDES = -I../Includes -I../Import/Includes

# Link against ZT.so for gimbal functions AND ZT_Lib.a for ATEM functions
ZT_SO = ../Binaries/ZT.so
ZT_LIB_A = ../Libraries/ZT_Lib.a
FRAMEWORKS = -framework IOKit -framework CoreFoundation
LDFLAGS = $(ZT_SO) $(ZT_LIB_A) $(FRAMEWORKS) -L../Import/Libraries -Wl,-rpath,@loader_path

TARGET = ../Binaries/libzt_python.dylib

SOURCES = zt_python.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -shared $(LDFLAGS) -o $@ $^
	@echo "Built $(TARGET)"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
