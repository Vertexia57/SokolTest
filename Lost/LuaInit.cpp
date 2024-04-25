#include "LuaInit.h"
#include <cassert>
#include <Windows.h>
#include <string>

#define assertm(exp, msg) assert(((void)msg, exp))

void checkLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::cout << "\n=[Lua Error!]=====================================================================================================\n\n >>> " << lua_tostring(L, -1) << "\n\n=[Lua Error!]=====================================================================================================\n" << std::endl;
		std::string error = std::string("=[Lua Error!]================================\n >>> ") + lua_tostring(L, -1);

		int part = 0;
		std::string line = "";
		std::string errorCode = "";
		for (char c : std::string(lua_tostring(L, -1)))
		{
			if (c == ']' && part == 0)
			{
				part = 1;
			}
			else if (c == ':' && part == 1 && line.size() >= 1)
			{
				part = 2;
			}
			else if (c != ':')
			{
				if (part == 1)
					line += c;
				if (part == 2)
					errorCode += c;
			}
		}

		std::string errorText = (error + "\n >>> Error At Line: " + line + "\n >>> Error Message:" + errorCode + "\n=[Lua Error!]================================");

		std::wstring wStrErrorText = std::wstring(errorText.begin(), errorText.end());

		MessageBox(NULL, wStrErrorText.c_str(), L"Error!", MB_ICONERROR | MB_OK);
		assert(false);
	}
}