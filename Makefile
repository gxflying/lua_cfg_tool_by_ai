# Makefile for LuaFileMap_Tool example

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Paths (adjust these according to your system setup)
GREENCONTROL_HOME ?= /usr/local/greencontrol
LUA_HOME ?= /usr

# Include directories
INCLUDES = -I. \
           -I$(GREENCONTROL_HOME)/include \
           -I$(LUA_HOME)/include/lua5.3

# Library directories
LIBDIRS = -L$(GREENCONTROL_HOME)/lib \
          -L$(LUA_HOME)/lib/x86_64-linux-gnu

# Libraries to link
LIBS = -llua5.3 -ldl -lpthread

# Source files
SRCS = example.cpp
TEST_SRC = test_debug.cpp

# Target executable
TARGET = luafile_example
TEST_TARGET = test_debug

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBDIRS) -o $@ $^ $(LIBS)

# Build the test executable
$(TEST_TARGET): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBDIRS) -o $@ $^ $(LIBS)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TEST_TARGET)

# Run the example
run: $(TARGET)
	./$(TARGET)

# Run the debug test
run-test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential liblua5.3-dev

.PHONY: all clean run run-test install-deps