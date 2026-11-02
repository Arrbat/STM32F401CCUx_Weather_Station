cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(Weather_Station C CXX ASM)

add_executable(${PROJECT_NAME})

set(CUBEMX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/CubeMX_Meteostation)
add_subdirectory(${CUBEMX_DIR}/cmake/stm32cubemx cubemx_build)

target_compile_definitions(Weather_Station PRIVATE
    $<$<CONFIG:Debug>:USE_FULL_ASSERT>
)
target_compile_definitions(stm32cubemx INTERFACE
    $<$<CONFIG:Debug>:USE_FULL_ASSERT>
)

target_sources(${PROJECT_NAME} PRIVATE
    app/src/app.c
    app/src/logger.c
    drivers/driver_uart.c
)
target_include_directories(${PROJECT_NAME} PRIVATE   
    app/
    app/src/
    app/inc/
    drivers/
)

# CubeMX generated. CPP is not used in project, but if CPP will be added - do not remove this list
# "Remove wrong libob.a library dependency when using cpp files"
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# This part resolves the "linker script not found [...] No such file or directory [...]"
# Initially, CubeMX expects that root CMakeLists and .ld are in the same directory
# This redefines -T option for Linker Flags with actual path to .ld file
file(GLOB LINKER_SCRIPTS ${CUBEMX_DIR}/*_FLASH.ld)
list(LENGTH LINKER_SCRIPTS _ld_count)
if(NOT _ld_count EQUAL 1)
    message(FATAL_ERROR "Expected one *_FLASH.ld in ${CUBEMX_DIR}, but found: ${_ld_count}")
endif()
string(REGEX REPLACE "-T *\"[^\"]+\\.ld\"" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
target_link_options(${PROJECT_NAME} PRIVATE -T${LINKER_SCRIPTS})

# Making this field as PRIVATE will cause error
# /cmake/stm32cubemx/CMakeLists.txt uses the plain signature for target_link_libraries with same target
target_link_libraries(${PROJECT_NAME} stm32cubemx)