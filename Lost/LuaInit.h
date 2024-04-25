#pragma once

#include <iostream> 

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#pragma comment(lib, "lua542/liblua54.a")

void checkLua(lua_State* L, int r);

static int luaDebugPrint(lua_State* L)
{
	std::string text = lua_tostring(L, -1);
	fprintf(stdout, (" [Lua::print] " + text + "\n").c_str());
	return 0;
}

static void luaBindDebugPrint(lua_State* L)
{
	lua_pushcfunction(L, luaDebugPrint);
	lua_setglobal(L, "cPrint");
};