add_executable(aoc main.cpp)
target_link_libraries(aoc PRIVATE cxxopts::cxxopts Catch2::Catch2 CURL::libcurl)
target_compile_features(aoc PRIVATE cxx_std_20)
target_include_directories(aoc PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>)
