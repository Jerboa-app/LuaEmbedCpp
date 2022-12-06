# LuaCMake
MWE embed of Lua into a CMake project with a tiny demo

Following [this guide](https://www.cs.usfca.edu/~galles/cs420/lecture/LuaLectures/LuaAndC.html), run the ```build.sh``` script to 
observe the output 

```bash
$ ./build/test 
5	3
55

```

Lua is built using the CMake incantations

```CMake
file(GLOB LUA_SRC "vendored/lua-5.4.4/src/*.c")
include_directories(src include)
add_library(Lua STATIC ${LUA_SRC})
```

and linked to an exectuable as 

```CMake
target_link_libraries(test Lua)
```

The only change from base Lua 5.4.4 is the removal of ```lua.c``` from ```vendored/lua-5.4.4/src/``` this allows your code to define an entry point. 
