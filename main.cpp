#include <stdlib.h>
#include <iostream>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

// int prod(lua_State * lua)
// {
//     int n = lua_gettop(lua);
//     lua_Number p = 1.0;
    
//     for (unsigned i = 1; i <= n; i++)
//     {
//         if (!lua_isnumber(lua,i))
//         {
//             lua_pushliteral(lua,"non numeric argument");
//             lua_error(lua);
//         }
//         p *= lua_tonumber(lua,i);
//     }
//     lua_pushnumber(lua,p);
//     std::cout << "prod\n";
//     return 1;
// }

static int average(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	/* loop through each argument */
	for (i = 1; i <= n; i++)
	{
		/* total the arguments */
		sum += lua_tonumber(L, i);
	}

	/* push the average */
	lua_pushnumber(L, sum / n);

	/* push the sum */
	lua_pushnumber(L, sum);

	/* return the number of results */
	return 2;
}

const struct luaL_Reg userLib[] = 
{
    {"avg",average},
    {NULL, NULL}
};

int load_userLib(lua_State * lua)
{
    luaL_newlib(lua,userLib);
    return 1;
}

int main(int argc, char ** argv) 
{
    lua_State * lua = luaL_newstate();

    luaL_openlibs(lua);

    luaL_requiref(lua, "userLib", load_userLib , 1);
    
    luaL_dofile(lua,"test.lua");

    lua_close(lua); 
    return 0;
}

