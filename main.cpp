#include <stdlib.h>
#include <iostream>
#include <vector>
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class Foo 
{
	public:
		Foo() = default;
		int lua_fooSay(lua_State * lua)
		{
			std::cout << name << "\n";
			return 0;
		}
	private:
	const char * name = "Class Foo here!";
};

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

/* 
	refer to https://stackoverflow.com/questions/32416388/how-to-register-member-function-to-lua-without-lua-bind-in-c
*/

struct LuaExtraSpace
{
	Accumulator * acc;
	Foo * foo;
};

typedef int (Accumulator::*AccumulatorMember)(lua_State * lua);

template <AccumulatorMember function>
int dispatchAccumulator(lua_State * lua)
{
	LuaExtraSpace * ptr = *static_cast<LuaExtraSpace**>(lua_getextraspace(lua));
	Accumulator * acc = ptr->acc;
	return ((*acc).*function)(lua);
}

typedef int (Foo::*FooMember)(lua_State * lua);

template <FooMember function>
int dispatchFoo(lua_State * lua)
{
	LuaExtraSpace * ptr = *static_cast<LuaExtraSpace**>(lua_getextraspace(lua));
	Foo * foo = ptr->foo;
	return ((*foo).*function)(lua);
}

const luaL_Reg userLib[] = 
{
	{"sumValues", &dispatchAccumulator<&Accumulator::lua_sumValues>},
	{"pushValues",&dispatchAccumulator<&Accumulator::lua_pushValues>},
	{"clearValues",&dispatchAccumulator<&Accumulator::lua_clearValues>},
	{"fooSay",&dispatchFoo<&Foo::lua_fooSay>},
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
	Foo foo;

	LuaExtraSpace luaStorage;

	luaStorage.acc = &acc;
	luaStorage.foo = &foo;

	*static_cast<LuaExtraSpace**>(lua_getextraspace(lua)) = &luaStorage;
    
	luaL_requiref(lua,"userLib",load_userLib,1);

    luaL_dofile(lua,"test.lua");

    lua_close(lua); 
    return 0;
}

