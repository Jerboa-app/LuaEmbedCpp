#include <stdlib.h>
#include <iostream>
#include <vector>
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class Accumulator
{
	public:

		Accumulator() = default;

		int lua_sumValues(lua_State * lua)
		{
			double sum = 0.0;

			for (unsigned i = 0; i < values.size(); i++)
			{
				sum += values[i];
			}

			lua_pushnumber(lua, sum);

			return 1;
		}

		int lua_pushValues(lua_State * lua)
		{
			int n = lua_gettop(lua);
			for (unsigned i = 1; i <= n; i++)
			{
				values.push_back(lua_tonumber(lua,i));
			}
			return 0;
		}

		int lua_clearValues(lua_State * lua)
		{
			values.clear();
			return 0;
		}

	private:

	std::vector<double> values;

};

typedef int (Accumulator::*member)(lua_State * lua);

template <member function>
int dispatch(lua_State * lua)
{
	Accumulator * ptr = *static_cast<Accumulator**>(lua_getextraspace(lua));
	return ((*ptr).*function)(lua);
}

const luaL_Reg userLib[] = 
{
	{"sumValues", &dispatch<&Accumulator::lua_sumValues>},
	{"pushValues",&dispatch<&Accumulator::lua_pushValues>},
	{"clearValues",&dispatch<&Accumulator::lua_clearValues>},
	{NULL,NULL}
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

	Accumulator acc;

	*static_cast<Accumulator**>(lua_getextraspace(lua)) = &acc;
    
	luaL_requiref(lua,"userLib",load_userLib,1);

    luaL_dofile(lua,"test.lua");

    lua_close(lua); 
    return 0;
}

