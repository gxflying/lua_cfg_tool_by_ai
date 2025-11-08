# LuaFileMap API

This module provides a tool that reads Lua configuration files and stores parameters in std::map containers according to their types.

## Features

- Reads Lua configuration files
- Stores parameters in three separate maps by type:
  - DoubleMap for floating-point values
  - LongMap for integer values
  - StringMap for string values
- Provides accessors to retrieve parameters by name
- Cross-type lookup support: getDouble() can retrieve integer values and convert them, getInteger() can retrieve double values and convert them
- Automatic conversion warnings to help understand data flow

## Usage

```cpp
#include "luafile_map_tool.h"

// Create an instance and load configuration
LuaFileMap_Tool luareader("luareader");
luareader.config("config.lua");

// Access individual parameters with type-specific getters
double frequency;
if (luareader.getDouble(frequency, "frequency")) {
    std::cout << "Frequency: " << frequency << " Hz" << std::endl;
}

long cores;
if (luareader.getInteger(cores, "cores")) {
    std::cout << "Cores: " << cores << std::endl;
}

std::string processor;
if (luareader.getString(processor, "processor")) {
    std::cout << "Processor: " << processor << std::endl;
}

// Cross-type lookup examples:
// Get an integer value as a double (automatically converted)
double coresAsDouble;
if (luareader.getDouble(coresAsDouble, "cores")) {
    std::cout << "Cores as double: " << coresAsDouble << std::endl;
}

// Get a double value as an integer (truncated if needed)
long frequencyAsLong;
if (luareader.getInteger(frequencyAsLong, "frequency")) {
    std::cout << "Frequency as long: " << frequencyAsLong << std::endl;
}
```

## Cross-Type Lookup

The LuaFileMap_Tool now supports cross-type lookup with automatic conversion:

- `getDouble()` can retrieve integer values from the LongMap and convert them to double
- `getInteger()` can retrieve double values from the DoubleMap and convert them to integer (with truncation for non-integer values)
- Conversion warnings are displayed to show when a value is found in a different type map:
  ```
  warning :  found $name  in other list :   origin valid ->  convert value
  ```

## Dependencies

This module requires:
- Lua library
- C++11 compatible compiler

## Integration

To use this module in another project:
1. Copy the `luafile_map_tool.h` file to your project
2. Ensure your build system includes the necessary paths for Lua
3. Link against the required libraries (Lua)