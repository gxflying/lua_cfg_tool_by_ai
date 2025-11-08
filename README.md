# LuaFileMap API

This module provides a modified version of the LuaFile_Tool that stores Lua configuration parameters in std::map containers according to their types.

## Features

- Reads Lua configuration files
- Stores parameters in three separate maps by type:
  - DoubleMap for floating-point values
  - LongMap for integer values
  - StringMap for string values
- Provides accessors to retrieve the maps

## Usage

```cpp
#include "greencontrol/gcnf/apis/luaFileMapApi/luafile_map_tool.h"

// In your SystemC module or sc_main function:
LuaFileMap_Tool luareader("luareader");
luareader.config("config.lua");

// Access the maps
const auto& doubleMap = luareader.getDoubleMap();
const auto& longMap = luareader.getLongMap();
const auto& stringMap = luareader.getStringMap();

// Example of accessing values
for (const auto& pair : doubleMap) {
    std::cout << "Double param: " << pair.first << " = " << pair.second << std::endl;
}
```

## Dependencies

This module requires the same dependencies as the original LuaFile_Tool:
- SystemC
- Lua library
- GreenControl framework

## Integration

To use this module in another project:
1. Copy the entire `luaFileMapApi` directory
2. Ensure your build system includes the necessary paths for SystemC, Lua, and GreenControl
3. Link against the required libraries (SystemC, Lua)