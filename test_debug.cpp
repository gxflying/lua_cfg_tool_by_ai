#include <iostream>
#include "luafile_map_tool.h"

using namespace gs::cnf;

int main() {
    const LuaFileMap_Tool& luareader = LuaFileMap_Tool::instance("config.lua", true);
    
    // Test cross-type lookup with debug info
    double mixed_int_as_double;
    std::cout << "Testing getDouble with integer value:" << std::endl;
    if (luareader.getDouble(mixed_int_as_double, "mixed_int_as_double")) {
        std::cout << "Retrieved: " << mixed_int_as_double << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }
    
    // Test non-integer double to integer conversion
    long non_integer_double_long;
    std::cout << "\nTesting getInteger with non-integer double value:" << std::endl;
    if (luareader.getInteger(non_integer_double_long, "non_integer_double")) {
        std::cout << "Retrieved: " << non_integer_double_long << std::endl;
    } else {
        std::cout << "Conversion failed (as expected)" << std::endl;
    }
    
    return 0;
}