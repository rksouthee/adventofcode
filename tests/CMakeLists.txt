add_executable(TestAoc
	test_algorithm.cpp
	test_split.cpp)
target_link_libraries(TestAoc PRIVATE Aoc Catch2::Catch2WithMain)
catch_discover_tests(TestAoc)
