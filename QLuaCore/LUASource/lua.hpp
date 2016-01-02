// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

//extern "C" {
#if defined(_DEBUG)
#include "lstate.h"
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
LUA_API int lua_sub_state_counter(lua_State * L);
LUA_API int lua_add_state_counter(lua_State * L);
//}
