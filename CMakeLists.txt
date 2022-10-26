cmake_minimum_required(VERSION 3.8)
project("FatalEngine")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4) # /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

FILE(GLOB_RECURSE engine_src "Source/Engine/*.cpp" "Source/Engine/*.h")
list(REMOVE_ITEM engine_src "Source/Engine/FatalPCH.h")

# FatalEngine: a dynamic lib
add_library(FatalEngineDL SHARED ${engine_src})
target_compile_definitions(FatalEngineDL PRIVATE FATAL_EXPORT=1)

find_package(Vulkan REQUIRED)
target_include_directories(FatalEngineDL PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/cgltf/
    ${Vulkan_INCLUDE_DIRS}
)

add_subdirectory(ThirdParty/lua)

target_link_libraries(FatalEngineDL PUBLIC ${Vulkan_LIBRARIES} lua_static)

FILE(GLOB_RECURSE src_exe "Source/Testbed/*.cpp" "Source/Testbed/*.h")
add_executable(FatalEngineEXE ${src_exe})

target_precompile_headers(FatalEngineEXE PUBLIC Source/Engine/FatalPCH.h)
target_link_libraries(FatalEngineEXE PUBLIC FatalEngineDL)
