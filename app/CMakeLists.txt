add_executable(aoc main.cpp)
target_link_libraries(aoc PRIVATE cxxopts::cxxopts CURL::libcurl)
target_include_directories(aoc PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>)

add_library(AoCTester OBJECT tester.cpp)
target_link_libraries(AoCTester PUBLIC Catch2::Catch2)
