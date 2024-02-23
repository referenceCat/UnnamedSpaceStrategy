cmake_minimum_required(VERSION 3.10)

### initial setup

# find ccache to use incremental compilation
find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set(CMAKE_C_COMPILER_LAUNCHER "ccache")
endif (CCACHE_FOUND)

# Set path to custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Includes
include(message_macros)
include(CheckIPOSupported)
include(clang-cxx-dev-tools)

# Enable Inter-procedural Optimization
check_ipo_supported(RESULT ipo_supported)
if (ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()



project(UntitledSpaceStrategy)


### Project variables

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources")


### Dependencies

find_package(PkgConfig REQUIRED)
pkg_check_modules(allegro-5 REQUIRED allegro-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5)
set(ALLEGRO5_LIBRARIES, "allegro_image,allegro_primitives;allegro;impl")




add_subdirectory(src)


