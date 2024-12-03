add_library(Elf)
target_sources(Elf
	PRIVATE
		aoc/string_helpers.cpp
	PUBLIC
		FILE_SET interface
		TYPE HEADERS
		BASE_DIRS ${CMAKE_CURRENT_LIST_DIR}
		FILES
		aoc/algorithm.h
		aoc/string_helpers.h
		aoc/orbit_structure.h
)
