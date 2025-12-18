# ZT_Python Makefile
# Compile the C wrapper for Python ctypes

CXX = g++
CXXFLAGS = -std=c++11 -fPIC -Wall
INCLUDES = -I../Includes -I../Import/Includes
ZT_LIB = ../Binaries/ZT.so
LDFLAGS = $(ZT_LIB) -L../Import/Libraries -Wl,-rpath,@loader_path

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
