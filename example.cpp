#include <iostream>
#include <string>
#include <map>

#include "luafile_map_tool.h"

int main(int argc, char *argv[]) {
    // Use the singleton instance of the LuaFileMap_Tool
    const char* configFile = "config.lua";  // Default config file
    if (argc > 1) {
        configFile = argv[1];
    }
    
    std::cout << "Loading configuration from: " << configFile << std::endl;
    
    // Load configuration using the singleton instance
    const LuaFileMap_Tool& luareader = LuaFileMap_Tool::instance(configFile, true);
    
    std::cout << "Configuration loaded successfully!" << std::endl;
    
    // Test individual parameter access
    std::cout << "\n=== Testing Individual Parameter Access ===" << std::endl;
    
    // Test double access
    double freq;
    if (luareader.getDouble(freq, "frequency")) {
        std::cout << "Frequency: " << freq << " Hz" << std::endl;
    } else {
        std::cout << "Frequency not found" << std::endl;
    }
    
    // Test long access using getInteger template
    long cores;
    if (luareader.getInteger(cores, "cores")) {
        std::cout << "Cores: " << cores << std::endl;
    } else {
        std::cout << "Cores not found" << std::endl;
    }
    
    // Test string access
    std::string processor;
    if (luareader.getString(processor, "processor")) {
        std::cout << "Processor: " << processor << std::endl;
    } else {
        std::cout << "Processor not found" << std::endl;
    }
    
    // Test integer template access
    int threads;
    if (luareader.getInteger(threads, "threads")) {
        std::cout << "Threads: " << threads << std::endl;
    } else {
        std::cout << "Threads not found" << std::endl;
    }
    
    // Test another string parameter
    std::string vendor;
    if (luareader.getString(vendor, "vendor")) {
        std::cout << "Vendor: " << vendor << std::endl;
    } else {
        std::cout << "Vendor not found" << std::endl;
    }
    
    // Test boolean parameter (stored as long) using getInteger template
    long enable_cache;
    if (luareader.getInteger(enable_cache, "enable_cache")) {
        std::cout << "Cache enabled: " << (enable_cache ? "true" : "false") << std::endl;
    } else {
        std::cout << "Cache enabled flag not found" << std::endl;
    }
    
    // Test mixed data types
    std::cout << "\n=== Testing Mixed Data Types ===" << std::endl;
    
    // Test double that looks like an integer (42.0) - stored as double
    double mixed_double_as_int_double;
    if (luareader.getDouble(mixed_double_as_int_double, "mixed_double_as_int")) {
        std::cout << "Mixed double as int (as double): " << mixed_double_as_int_double << std::endl;
    } else {
        std::cout << "Mixed double as int not found" << std::endl;
    }
    
    // Test integer value (100) - stored as long, can be retrieved as double using getDouble (new feature)
    double mixed_int_as_double;
    if (luareader.getDouble(mixed_int_as_double, "mixed_int_as_double")) {
        std::cout << "Mixed int as double (using getDouble): " << mixed_int_as_double << std::endl;
    } else {
        std::cout << "Mixed int as double not found" << std::endl;
    }
    
    // Test integer value (100) - stored as long, can be retrieved as long using getInteger
    long mixed_int_as_double_long;
    if (luareader.getInteger(mixed_int_as_double_long, "mixed_int_as_double")) {
        std::cout << "Mixed int as double (as long): " << mixed_int_as_double_long << std::endl;
    } else {
        std::cout << "Mixed int as double not found" << std::endl;
    }
    
    // Test negative integer
    long mixed_negative_int;
    if (luareader.getInteger(mixed_negative_int, "mixed_negative_int")) {
        std::cout << "Mixed negative int: " << mixed_negative_int << std::endl;
    } else {
        std::cout << "Mixed negative int not found" << std::endl;
    }
    
    // Test negative double
    double mixed_negative_double;
    if (luareader.getDouble(mixed_negative_double, "mixed_negative_double")) {
        std::cout << "Mixed negative double: " << mixed_negative_double << std::endl;
    } else {
        std::cout << "Mixed negative double not found" << std::endl;
    }
    
    // Test retrieving double as integer (should work if it's a whole number)
    long mixed_double_as_int_long;
    if (luareader.getInteger(mixed_double_as_int_long, "mixed_double_as_int")) {
        std::cout << "Mixed double as int (as long): " << mixed_double_as_int_long << std::endl;
    } else {
        std::cout << "Mixed double as int not found or not convertible to integer" << std::endl;
    }
    
    // Test non-integer double conversion to integer (now forced conversion)
    long non_integer_double_long;
    std::cout << "\nTesting forced conversion of non-integer double to integer:" << std::endl;
    if (luareader.getInteger(non_integer_double_long, "non_integer_double")) {
        std::cout << "Non-integer double converted to long: " << non_integer_double_long << std::endl;
    } else {
        std::cout << "Non-integer double conversion failed" << std::endl;
    }
    
    // Additional test cases for cross-type lookup
    std::cout << "\n=== Additional Cross-Type Lookup Test Cases ===" << std::endl;
    
    // Test double in integer map
    double test_double_in_int_map_double;
    if (luareader.getDouble(test_double_in_int_map_double, "test_double_in_int_map")) {
        std::cout << "Double value from double map: " << test_double_in_int_map_double << std::endl;
    } else {
        std::cout << "Double value not found in double map" << std::endl;
    }
    
    // Test integer retrieval of double value (should fail since it's actually stored in double map)
    long test_double_in_int_map_long;
    if (luareader.getInteger(test_double_in_int_map_long, "test_double_in_int_map")) {
        std::cout << "Double value retrieved as integer: " << test_double_in_int_map_long << std::endl;
    } else {
        std::cout << "Double value not found in integer map (as expected)" << std::endl;
    }
    
    // Test integer in double map
    long test_int_in_double_map_long;
    if (luareader.getInteger(test_int_in_double_map_long, "test_int_in_double_map")) {
        std::cout << "Integer value from integer map: " << test_int_in_double_map_long << std::endl;
    } else {
        std::cout << "Integer value not found in integer map" << std::endl;
    }
    
    // Test double retrieval of integer value (should work with cross-type lookup)
    double test_int_in_double_map_double;
    if (luareader.getDouble(test_int_in_double_map_double, "test_int_in_double_map")) {
        std::cout << "Integer value retrieved as double: " << test_int_in_double_map_double << std::endl;
    } else {
        std::cout << "Integer value not found in double map" << std::endl;
    }
    
    // Edge case tests
    std::cout << "\n=== Edge Case Tests ===" << std::endl;
    
    // Test zero values
    double edge_case_zero_double_val;
    if (luareader.getDouble(edge_case_zero_double_val, "edge_case_zero_double")) {
        std::cout << "Zero double value: " << edge_case_zero_double_val << std::endl;
    }
    
    long edge_case_zero_int_val;
    if (luareader.getInteger(edge_case_zero_int_val, "edge_case_zero_int")) {
        std::cout << "Zero integer value: " << edge_case_zero_int_val << std::endl;
    }
    
    // Test large values
    double large_double_value_val;
    if (luareader.getDouble(large_double_value_val, "large_double_value")) {
        std::cout << "Large double value: " << large_double_value_val << std::endl;
    }
    
    long large_int_value_val;
    if (luareader.getInteger(large_int_value_val, "large_int_value")) {
        std::cout << "Large integer value: " << large_int_value_val << std::endl;
    }
    
    // Demonstrate cross-type access
    std::cout << "\n=== Demonstrating Cross-Type Access ===" << std::endl;
    std::cout << "Note: With cross-type lookup enabled:" << std::endl;
    std::cout << "  - getDouble() can now retrieve integer values and convert them" << std::endl;
    std::cout << "  - getInteger() can now retrieve double values and force convert them" << std::endl;
    std::cout << "  - Non-integer doubles are truncated when converted to integers" << std::endl;
    std::cout << "  - Conversion warnings are always displayed:" << std::endl;
    std::cout << "      Format: warning :  found $name  in other list :   origin valid ->  convert value" << std::endl;
    
    return 0;
}