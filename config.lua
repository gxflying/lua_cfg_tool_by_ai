-- Test configuration file for LuaFileMap_Tool

-- Double values
frequency = 1.5e9
voltage = 3.3
temperature = 25.5

-- Integer values
cores = 4
threads = 8
cache_size = 1024

-- String values
processor = "ARM Cortex-A53"
architecture = "ARMv8-A"
vendor = "ARM Limited"

-- Boolean values (stored as long: 1 for true, 0 for false)
enable_cache = true
enable_debug = false

-- Nested table
memory = {
    ram = "8GB",
    rom = "128MB",
    swap = 2.0  -- This will be stored as double
}

-- Array-like table
features = {
    "NEON",
    "VFP",
    "TrustZone"
}

-- Mixed data types for testing type conversion
mixed_double_as_int = 42.0    -- Double that looks like an integer
mixed_int_as_double = 100     -- Integer that should be retrieved as double
mixed_negative_int = -5       -- Negative integer
mixed_negative_double = -3.14 -- Negative double

-- Non-integer double for testing conversion failure
non_integer_double = 3.14159  -- Cannot be converted to integer

-- Additional test cases for cross-type lookup
test_double_in_int_map = 7.5      -- Double value that will be stored, but we'll try to get as int
test_int_in_double_map = 42       -- Integer value that will be stored, but we'll try to get as double
edge_case_zero_double = 0.0       -- Zero as double
edge_case_zero_int = 0            -- Zero as int
large_double_value = 1234567890.0 -- Large double value
large_int_value = 987654321       -- Large integer value