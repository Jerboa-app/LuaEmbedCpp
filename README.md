# LuaCMake
MWE embed of Lua into a CMake project with a tiny demo

Run the ```build.sh``` script to 
observe the outputs on each branch 

## [Ex 1, main](https://github.com/Jerboa-app/Lua-Embed-CMake/tree/main) 

### Simply call some Lua code

```bash
$ ./build/test 
5	3
55
```

Lua code

```Lua
function fib(n)
   if n == 1 or n == 2 then
      return 1,1
   end
   prev, prevPrev = fib(n-1)
   return prev+prevPrev, prev
end

print(fib(5))
print((fib(10)))
```


## [Ex 2, addFunctions](https://github.com/Jerboa-app/Lua-Embed-CMake/tree/addFunctions) 

### Register a custom "C" library, call it from Lua

```bash
$ ./build/test 
2.5
```

Lua code (userLib is a custom "C" library)

```Lua
a = userLib.avg(1,2,3,4)
print(a)
```

## [Ex 3, addMemberFunction](https://github.com/Jerboa-app/Lua-Embed-CMake/tree/addMemberFunction) 

### Register a custom "C" library, encapsulating a C++ class's member functions, call them from Lua

refer to https://stackoverflow.com/questions/32416388/how-to-register-member-function-to-lua-without-lua-bind-in-c

```bash
$ ./build/test 
6.0
0.0
```

Lua code (userLib encapsulate a C++ class's member functions)

```Lua
userLib.pushValues(1,2,3)
a = userLib.sumValues()
print(a)
userLib.clearValues()
b = userLib.sumValues()
print(b)
```

## [Ex 4, multiClasses](https://github.com/Jerboa-app/Lua-Embed-CMake/tree/multiClasses)

### Register a custom "C" library, encapsulating *two* C++ classes' member functions, call them from Lua

Extends the previous example by wrapping the class pointers into a struct. Then setting Lua's extra space as that struct

```bash
$ ./build/test 
6.0
0.0
Class Foo here!
```

Lua code (userLib encapsulate a C++ class's member functions)

```Lua
userLib.pushValues(1,2,3)
a = userLib.sumValues()
print(a)
userLib.clearValues()
b = userLib.sumValues()
print(b)

userLib.fooSay()
```

## Building (CMake)

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

[This is a decent guide to Lua 5/C++ intro guide](https://www.cs.usfca.edu/~galles/cs420/lecture/LuaLectures/LuaAndC.html)
