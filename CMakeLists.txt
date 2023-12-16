cmake_minimum_required(VERSION 3.27)

project(AdventOfCode VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

option(AOC_BUILD_DOCUMENTATION "Build documentation for AdventOfCode" YES)

# Hide symbols by default
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Catch2 3 REQUIRED)
find_package(CURL 8 REQUIRED)
find_package(cxxopts 3 REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(AdventOfCode)
include(Catch)

# NOTE: Use `CMAKE_COMPILE_WARNING_AS_ERROR` cache variable to treat warnings as errors or not.
if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -pedantic -Wno-gnu-anonymous-struct)
endif()

add_subdirectory(lib)
add_subdirectory(app)

add_subdirectory(2018)
add_subdirectory(2023)

if(BUILD_TESTING)
	add_subdirectory(tests)
endif()

if(AOC_BUILD_DOCUMENTATION)
	find_package(Doxygen REQUIRED)
	set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
	set(DOXYGEN_EXTRACT_ALL YES)
	doxygen_add_docs(Documentation lib doc)
endif()
