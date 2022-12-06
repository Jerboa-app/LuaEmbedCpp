project(test)
set(CMAKE_CXX_STANDARD 14)

option(USE_POSIX OFF)

if (USE_POSIX)
add_compile_definitions(LUA_USE_POSIX)
endif()

file(GLOB LUA_SRC "vendored/lua-5.4.4/src/*.c")
include_directories(src include)
add_library(Lua STATIC ${LUA_SRC})

add_executable(test main.cpp)

target_link_libraries(test Lua)
