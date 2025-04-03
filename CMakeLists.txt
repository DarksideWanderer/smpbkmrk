cmake_minimum_required(VERSION 3.10)

# 这里是CMake的缓存设置(得先干掉缓存再载入)
set(CMAKE_BUILD_TYPE Debug)
unset(CMAKE_CXX_COMPILER CACHE)
unset(CMAKE_CXX_STANDARD_REQUIRED CACHE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -std=c++20")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -std=c17")
set(CMAKE_CXX_COMPILER "/opt/homebrew/Cellar/gcc/14.2.0_1/bin/aarch64-apple-darwin24-g++-14" CACHE STRING "C++ Compiler" FORCE)
set(CMAKE_C_COMPILER "/opt/homebrew/Cellar/gcc/14.2.0_1/bin/aarch64-apple-darwin24-gcc-14" CACHE STRING "C Compiler" FORCE)

# 项目的名字
project(MyProject)

# 这里是设置宏(载入头文件)
set(Necessery_H 
	/opt/homebrew/Cellar/sdl2/2.32.2/include 
	/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include
	/opt/homebrew/Cellar/sdl2_gfx/1.0.4/include
)

# 这里是将有用的头文件载入
include_directories(include)
include_directories(${Necessery_H})
include_directories(${DOOL_PATH})

# 递归命名文件(用于链接编译)
file(GLOB_RECURSE SOURCES "include/*.cpp" REQUIRED) # 将所有这种文件命名为 SOURCES

# 这里是寻找包(用于跨平台)
find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)
find_package(sdl2_ttf REQUIRED)
find_package(sdl2_net REQUIRED)

get_property(_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
message(STATUS "Available imported targets:")
foreach (_target ${_targets})
	message(STATUS "  ${_target}")
endforeach()

# 这里是链接库
add_executable(main src/main.cpp ${SOURCES} ${DTOOL})
# add_executable(test src/test.cpp ${SOURCES})
target_link_libraries(main SDL2::SDL2 SDL2_ttf::SDL2_ttf)
target_link_libraries(main /opt/homebrew/Cellar/sdl2_gfx/1.0.4/lib/libSDL2_gfx-1.0.0.dylib)