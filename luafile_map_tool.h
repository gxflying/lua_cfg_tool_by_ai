//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
//   http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Marcus Bartholomeu <bartho@greensocs.com>
//     GreenSocs
//     http://www.greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __LUAFILE_MAP_TOOL_H__
#define __LUAFILE_MAP_TOOL_H__

#include <map>
#include <string>
#include <iostream>
#include <cmath>

// Set to true (or use -DGC_LUA_VERBOSE=true argument) to show the parameters set
#ifndef GC_LUA_VERBOSE
#define GC_LUA_VERBOSE false
#endif

// Set to true (or use -DGC_LUA_DEBUG=true argument) to show what was not set as a parameter
#ifndef GC_LUA_DEBUG
#define GC_LUA_DEBUG false
#endif

//#define ENABLE_SHORT_COMMAND_LINE_OPTIONS  // enables the short synonyms for the gs_ options

// if this should use the unix getopt function for the parsing of the command line options
// or (if not) the boost program_options should be used (don't forget to link the lib boost_program_options!)
// default: do NOT define this!
// #define USE_GETOPT

// Removed SystemC support
// #include "greencontrol/config.h"

#ifdef USE_GETOPT
# include <getopt.h>
#else
# include <boost/program_options.hpp>
#endif

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace gs {
namespace cnf {

#ifndef USE_GETOPT
  namespace po = boost::program_options;
#endif
  
  /// Tool which reads a Lua configuration file and sets parameters in maps.
  /**
   * Lua Config File Tool which reads a configuration file and stores the parameters
   * in std::map containers according to their types (double, long, string).
   *
   * One instance can be used to read and configure several lua config files.
   */

  class LuaFileMap_Tool
  {
  
  public:
    typedef std::map<std::string, double> DoubleMap;
    typedef std::map<std::string, long> LongMap;
    typedef std::map<std::string, std::string> StringMap;

    /// Get the singleton instance
    /**
     * @param lua_cfg_file Path to the Lua config file (can be NULL to just get the instance)
     * @param reset If true, clear existing maps before loading; if false, append to existing maps
     * @return Reference to the singleton instance
     */
    static const LuaFileMap_Tool& instance(const char* lua_cfg_file = NULL, bool reset = false)
    {
      static LuaFileMap_Tool instance;
      if (lua_cfg_file != NULL) {
        instance.config(lua_cfg_file, reset);
      }
      return instance;
    }

    /// Get a double value from the map, with cross-type lookup
    /**
     * @param val Reference to store the value
     * @param param_name Name of the parameter to retrieve
     * @return true if the parameter exists and was successfully retrieved, false otherwise
     */
    bool getDouble(double& val, const char* param_name) const
    {
      // First try to find it in the double map
      auto it = mDoubleMap.find(param_name);
      if (it != mDoubleMap.end()) {
        val = it->second;
        return true;
      }
      
      // If not found, try to find it in the long map and convert
      auto long_it = mLongMap.find(param_name);
      if (long_it != mLongMap.end()) {
        val = static_cast<double>(long_it->second);
        std::cout << "warning :  found " << param_name << " in other list :   " 
                  << long_it->second << " ->  " << val << std::endl;
        return true;
      }
      
      // Not found in either map
      return false;
    }
    
    /// Get a string value from the map
    /**
     * @param val Reference to store the value
     * @param param_name Name of the parameter to retrieve
     * @return true if the parameter exists and was successfully retrieved, false otherwise
     */
    bool getString(std::string& val, const char* param_name) const
    {
      auto it = mStringMap.find(param_name);
      if (it != mStringMap.end()) {
        val = it->second;
        return true;
      }
      return false;
    }
    
    /// Template function to get integer values with different bit sizes, with cross-type lookup
    /**
     * @tparam T The integer type (int8_t, int16_t, int32_t, int64_t, etc.)
     * @param val Reference to store the value
     * @param param_name Name of the parameter to retrieve
     * @return true if the parameter exists and was successfully retrieved, false otherwise
     */
    template<typename T>
    bool getInteger(T& val, const char* param_name) const
    {
      // First try to find it in the long map
      auto long_it = mLongMap.find(param_name);
      if (long_it != mLongMap.end()) {
        val = static_cast<T>(long_it->second);
        return true;
      }
      
      // If not found, try to find it in the double map and convert
      auto double_it = mDoubleMap.find(param_name);
      if (double_it != mDoubleMap.end()) {
        double double_val = double_it->second;
        // Convert double to integer (force conversion even for non-integer values)
        long long_val = static_cast<long>(double_val);
        val = static_cast<T>(long_val);
        
        // Print information about the conversion
        std::cout << "warning :  found " << param_name << " in other list :   " 
                  << double_val << " ->  " << long_val << std::endl;
        return true;
      }
      
      // Not found in either map
      return false;
    }

  private:
    /// Private constructor for singleton
    LuaFileMap_Tool()
    {
      // Constructor left empty
    }
    
    /// Private destructor
    ~LuaFileMap_Tool()
    {
      // Destructor left empty
    }
    
    /// Disable copy constructor
    LuaFileMap_Tool(const LuaFileMap_Tool&) = delete;
    
    /// Disable assignment operator
    LuaFileMap_Tool& operator=(const LuaFileMap_Tool&) = delete;
    
    /// Makes the configuration
    /**
     * Configure parameters from a lua file and store them in appropriate maps by type.
     */
    int config(const char *config_file, bool reset = true)
    {
      // Removed SystemC-specific debug macro
      // GCNF_DUMP_N("LuaFileMap_Tool", "Read lua file '"<< config_file<<"'");

      // Clear maps for new configuration if reset is true
      if (reset) {
        mDoubleMap.clear();
        mLongMap.clear();
        mStringMap.clear();
      }

      // start Lua
      lua_State *L = luaL_newstate();
      luaL_openlibs(L);

      // load a script as the function "config_chunk"
      int error = luaL_loadfile(L, config_file);
      switch(error) {
      case 0:
        break;
      case LUA_ERRSYNTAX:
        fprintf(stderr, "Syntax error reading config file: %s\n", config_file);
        lua_close(L);
        return 1;
      case LUA_ERRMEM:
        fprintf(stderr, "Error allocating memory to read config file: %s\n", config_file);
        lua_close(L);
        return 1;
      case LUA_ERRFILE:
        fprintf(stderr, "Error to open/read the config file: %s\n", config_file);
        lua_close(L);
        return 1;
      default:
        fprintf(stderr, "Unknown error loading config file: %s\n", config_file);
        lua_close(L);
        return 1;
      }
      lua_setglobal(L, "config_chunk");

      // little script to run the file
      const char* config_loader =
        "-- put some commands here to run before the user script\n"
        "config_chunk()";

      // run
      if (luaL_dostring(L, config_loader)) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);  /* pop error message from the stack */
      }

      // traverse the environment table setting global variables as parameters
//      lua_getfield(L, LUA_GLOBALSINDEX, "_G");
      // getglobal should work for both lua 5.1 and 5.2
      lua_getglobal(L, "_G");
      error = setParamsFromLuaTable(L, lua_gettop(L));
      lua_close(L);
      if (error < 0) {
        fprintf(stderr, "Error loading lua config file: %s\n", config_file);
        return error;
      }
      return 0;
    }

    /// Get a long value from the map (kept for internal use)
    /**
     * @param val Reference to store the value
     * @param param_name Name of the parameter to retrieve
     * @return true if the parameter exists and was successfully retrieved, false otherwise
     */
    bool getLong(long& val, const char* param_name) const
    {
      auto it = mLongMap.find(param_name);
      if (it != mLongMap.end()) {
        val = it->second;
        return true;
      }
      return false;
    }

    /// Get reference to the double map
    const DoubleMap& getDoubleMap() const { return mDoubleMap; }
    
    /// Get reference to the long map
    const LongMap& getLongMap() const { return mLongMap; }
    
    /// Get reference to the string map
    const StringMap& getStringMap() const { return mStringMap; }

  protected:

    /// Maps to store parameters by type
    DoubleMap mDoubleMap;
    LongMap mLongMap;
    StringMap mStringMap;

    /// Traverse a Lua table setting global variables as parameters (recursive)
    /**
     * @param L Lua state
     * @param t Lua index
     * @param level (only for recursion) parameter name prefixed
     * @return number of elements if lua array or error if negative
     */
    int setParamsFromLuaTable(lua_State *L, int t, char* level=NULL)
    {
      /* start up */
      const int MAX_NAME_SIZE = 1000;
      static char static_key[MAX_NAME_SIZE];
      static char* key;
      int should_inc_integer_index_count;
      int integer_index_count = 0;
      char *next_level;
      if (level == NULL) {
        key = static_key;
        level = key;
      }

      /* test for overflow (hopefully unlikelly, so test only after it happens, for sanity) */
      if (level - static_key > MAX_NAME_SIZE) {
        static_key[MAX_NAME_SIZE-1] = 0;
        fprintf(stderr, "FATAL Error: parameter name too big (bigger then %d): %s", MAX_NAME_SIZE, static_key);
        return -1;
      }

      /* is it really a table? */
      if (lua_type(L, t) != LUA_TTABLE) {
        fprintf(stderr, "Error: argument is not a table");
        return -1;
      }

      /* traverse table */
      lua_pushnil(L);  /* first key */

      /* adjust t if relative index */
      if (t<0) --t;

      while (lua_next(L, t) != 0) {

        /* reset flag */
        should_inc_integer_index_count = false;

        /* set the key */
        switch(lua_type(L, -2)) {

        case LUA_TNUMBER:
          // key must be integer values (ignore floating part)
          // also convert from 1-based to 0-based indexes (decrement 1)
          should_inc_integer_index_count = true;
          #ifdef __MINGW32__
          next_level = level + __mingw_sprintf(level, "%lld",
                       (long long) lua_tonumber(L, -2) - 1);
          #else
          next_level = level + sprintf(level, "%lld",
                       (long long) lua_tonumber(L, -2) - 1);
          #endif
          break;

        case LUA_TSTRING:
          // avoid using stpcpy as it is not defined in MSVS, use strcpy+length instead
          next_level = strcpy(level, lua_tostring(L, -2));
          next_level += strlen(level);
          break;

        default:
          fprintf(stderr, "Error loading lua file: invalid key");
          return -1;
        }

        /* set key value in the database */
        switch(lua_type(L, -1)) {

        case LUA_TNUMBER:
          // Avoid setting some Lua specific values as parameters
          if (strcmp(key, "math.huge") == 0 ||
              strcmp(key, "math.pi") == 0 ||
              0) {
            if (GC_LUA_DEBUG) fprintf(stderr, "(%s) %s   (ignored because it's Lua specific)\n", lua_typename(L, lua_type(L, -1)), key);
          }
          else {
            // Use lua_isinteger if available (Lua 5.3+), otherwise fall back to previous method
            #if LUA_VERSION_NUM >= 503
            if (lua_isinteger(L, -1)) {
              // This is an integer
              lua_Integer intVal = lua_tointeger(L, -1);
              
              // Store in long map
              mLongMap[std::string(key)] = (long)intVal;
              if (GC_LUA_VERBOSE) fprintf(stderr, "(SET LONG) %s = %ld\n", key, (long)intVal);
            } else {
              // This is a float
              lua_Number numVal = lua_tonumber(L, -1);
              
              // Store in double map
              mDoubleMap[std::string(key)] = (double)numVal;
              if (GC_LUA_VERBOSE) fprintf(stderr, "(SET DOUBLE) %s = %f\n", key, (double)numVal);
            }
            #else
            // Fallback for older Lua versions
            long double num = lua_tonumber(L, -1);
            // test if it is an integer
            if ((long long) num == num) {
              // Store in long map
              mLongMap[std::string(key)] = (long)num;
              if (GC_LUA_VERBOSE) fprintf(stderr, "(SET LONG) %s = %ld\n", key, (long)num);
            }
            else {
              // Store in double map
              mDoubleMap[std::string(key)] = (double)num;
              if (GC_LUA_VERBOSE) fprintf(stderr, "(SET DOUBLE) %s = %f\n", key, (double)num);
            }
            #endif
            if (should_inc_integer_index_count) ++integer_index_count;
          }
          break;

        case LUA_TBOOLEAN:
          {
            bool boolVal = lua_toboolean(L, -1);
            // Store boolean as long (0 or 1)
            mLongMap[std::string(key)] = boolVal ? 1 : 0;
            if (GC_LUA_VERBOSE) fprintf(stderr, "(SET BOOL/LONG) %s = %s\n", key, boolVal? "true":"false");
            if (should_inc_integer_index_count) ++integer_index_count;
          }
          break;

        case LUA_TSTRING:
          // Avoid setting some Lua specific values as parameters
          if (strcmp(key, "_VERSION") == 0 ||
              strcmp(key, "package.cpath") == 0 ||
              strcmp(key, "package.config") == 0 ||
              strcmp(key, "package.path") == 0 ||
              0) {
            if (GC_LUA_DEBUG) fprintf(stderr, "(%s) %s   (ignored because it's Lua specific)\n", lua_typename(L, lua_type(L, -1)), key);
          }
          else {
            std::string strVal = lua_tostring(L, -1);
            // Store in string map
            mStringMap[std::string(key)] = strVal;
            if (GC_LUA_VERBOSE) fprintf(stderr, "(SET STRING) %s = %s\n", key, strVal.c_str());
            if (should_inc_integer_index_count) ++integer_index_count;
          }
          break;

        case LUA_TTABLE:
          // Avoid recursion on some tables
          if (strcmp(key, "_G") == 0 ||
              strcmp(key, "package.loaded") == 0) {
            if (GC_LUA_DEBUG) fprintf(stderr, "(%s) %s   (ignored to avoid recursion)\n", lua_typename(L, lua_type(L, -1)), key);
          }
          else {
            if (GC_LUA_DEBUG) fprintf(stderr, "(table) %s\n", key);
            *next_level++ = '.';
            // CS
            //int int_index_count = 
            setParamsFromLuaTable(L, -1, next_level);
            // CS
            //if (int_index_count > 0) {
              //mApi->setInitValue((std::string(key).substr(0, next_level-key) + "init_size").c_str(), value);
              //if (GC_LUA_VERBOSE) fprintf(stderr, "(SET number) %s = %s\n", (std::string(key).substr(0, next_level-key) + "init_size").c_str(), value);
            //}
            
          }
          break;

        case LUA_TFUNCTION:
        case LUA_TNIL:
        case LUA_TUSERDATA:
        case LUA_TTHREAD:
        case LUA_TLIGHTUSERDATA:
        default:
          // Ignore other types
          if (GC_LUA_DEBUG) fprintf(stderr, "(%s) %s\n", lua_typename(L, lua_type(L, -1)), key);
        }
     
        /* removes 'value'; keeps 'key' for next iteration */
        lua_pop(L, 1);
      }

      return integer_index_count;
    }

  };

} // end namespace cnf
} // end namespace gs

#endif