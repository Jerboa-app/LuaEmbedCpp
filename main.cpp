    #include <stdlib.h>
    #include <stdio.h>

extern "C"{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

    int main(char argv[], int argc) {
           lua_State * lua = luaL_newstate();
	   luaL_openlibs(lua);
	   
	   luaL_dofile(lua,"test.lua");

	   lua_close(lua); 
           return 0;
    }

