cmake_minimum_required(VERSION 3.10)

# 这里是CMake的缓存设置(得先干掉缓存再载入)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
unset(CMAKE_CXX_COMPILER CACHE)
unset(CMAKE_C_COMPILER CACHE)
set(CMAKE_CXX_COMPILER "/opt/homebrew/Cellar/gcc/14.2.0_1/bin/aarch64-apple-darwin24-g++-14" CACHE STRING "C++ Compiler" FORCE)
set(CMAKE_C_COMPILER "/opt/homebrew/Cellar/gcc/14.2.0_1/bin/aarch64-apple-darwin24-gcc-14" CACHE STRING "C Compiler" FORCE)

# 项目的名字
project(MyProject)

# 这里是设置宏(载入头文件)
set(SDL_H /opt/homebrew/Cellar/sdl2/2.32.2/include)

# 这里是将有用的头文件载入
include_directories(include)
include_directories(${SDL_H})

# 递归命名文件(用于链接编译)
file(GLOB_RECURSE SOURCES "include/*.cpp" REQUIRED) # 将所有这种文件命名为 SOURCES

# 这里是寻找包(用于跨平台)
find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)
get_property(_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
message(STATUS "Available imported targets:")
foreach (_target ${_targets})
	message(STATUS "  ${_target}")
endforeach()

# 这里是链接库
# target_link_libraries( SDL2::SDL2)