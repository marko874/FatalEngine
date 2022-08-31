cmake_minimum_required(VERSION 3.8)
project("FatalEngine")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

FILE(GLOB src_root Source/*.h)
FILE(GLOB src_cpp Source/Engine/Core/*.cpp)
FILE(GLOB src_engine_h Source/Engine/*.h)
FILE(GLOB src_engine_core_h Source/Engine/Core/*.h)
FILE(GLOB renderer_h Source/Engine/Renderer/*.h)
FILE(GLOB renderer_cpp Source/Engine/Renderer/*.cpp)

add_library(FatalEngineDL SHARED ${src_root} ${src_cpp} ${src_engine_h} ${src_engine_core_h} ${renderer_h} ${renderer_cpp})
target_compile_definitions(FatalEngineDL PRIVATE FATAL_EXPORT=1)
target_include_directories(FatalEngineDL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/)
target_include_directories(FatalEngineDL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

find_package(Vulkan REQUIRED)
target_include_directories(FatalEngineDL PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(FatalEngineDL PUBLIC ${Vulkan_LIBRARIES})


FILE(GLOB src_exe Source/Testbed/*.cpp)
add_executable(FatalEngineEXE ${src_exe})
target_precompile_headers(FatalEngineEXE PUBLIC Source/Engine/FatalPCH.h)
target_link_libraries(FatalEngineEXE PUBLIC FatalEngineDL)
