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
	fprintf(stdout, (text).c_str());
	lua_pop(L, -1);
	return 0;
}

static void luaBindDebugPrint(lua_State* L)
{
	lua_pushcfunction(L, luaDebugPrint);
	lua_setglobal(L, "cPrint");
};

static void dumpstack(lua_State* L) {
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {
		printf("%d\t%s\t", i, luaL_typename(L, i));
		switch (lua_type(L, i)) {
		case LUA_TNUMBER:
			printf("%g\n", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			printf("%s\n", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
			break;
		case LUA_TNIL:
			printf("%s\n", "nil");
			break;
		default:
			printf("%p\n", lua_topointer(L, i));
			break;
		}
	}
}