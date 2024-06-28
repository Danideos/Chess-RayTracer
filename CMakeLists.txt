cmake_minimum_required(VERSION 3.22)
project(RealChess-RayTracer)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS -Ofast)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
file(GLOB SRC scripts/*.cpp scripts/RayTrace/*.cpp scripts/Random/*.cpp)
add_executable(${PROJECT_NAME} ${SRC})

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

#target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} mingw32 ole32 oleaut32 imm32 winmm version Setupapi.lib)

# Assuming you have a target named 'your_target_name'
target_compile_options(${PROJECT_NAME} PRIVATE -O2)

