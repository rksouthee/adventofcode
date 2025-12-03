# We pad the name so that the targets are ordered correctly
function(aoc_get_filename_padded DAY OUTPUT_VARIABLE)
	if (DAY LESS 10)
		set(${OUTPUT_VARIABLE} "0${DAY}" PARENT_SCOPE)
	else()
		set(${OUTPUT_VARIABLE} "${DAY}" PARENT_SCOPE)
	endif()
endfunction()

function(add_solution day)
	get_filename_component(AOC_YEAR ${CMAKE_CURRENT_LIST_DIR} NAME)
	aoc_get_filename_padded(${day} day)
	set(AOC_TARGET_NAME "${AOC_YEAR}_day${day}")
	add_library(${AOC_TARGET_NAME} MODULE "day${day}.cpp")
	# Make all of these a dependency on the aoc target so that building the main app will build all solutions
	add_dependencies(aoc ${AOC_TARGET_NAME})
	set_target_properties(${AOC_TARGET_NAME} PROPERTIES
		OUTPUT_NAME "day${day}"
		FOLDER ${AOC_YEAR}
	)
	target_include_directories(${AOC_TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/app")
	target_link_libraries(${AOC_TARGET_NAME} PRIVATE Elf AoCTester)
	if(WIN32)
		target_link_options(${AOC_TARGET_NAME} PRIVATE "LINKER:-export:solve,-export:test")
	endif()
endfunction()

set(AOC_SOLUTION_TEMPLATE "#include \"aoc.h\"

namespace
{
}

TEST_CASE(\"Part One\")
{
}

SOLVE
{
}
")

# Generate the files needed for a new year, will create the CMakeLists.txt file
# and a solution file for each of the days if they don't already exist.
# This is expected to be called when configuring the project.
function(aoc_generate_template YEAR)
	set(NUMBER_OF_DAYS 25)
	if(YEAR GREATER_EQUAL 2025)
		set(NUMBER_OF_DAYS 12)
	endif()

	file(MAKE_DIRECTORY "${YEAR}")

	# Create the CMakeLists.txt file, will use the YEAR and NUMBER_OF_DAYS to generate the correct information.
	set(AOC_CMAKE_FILE "${CMAKE_CURRENT_LIST_DIR}/${YEAR}/CMakeLists.txt")
	if (NOT EXISTS "${AOC_CMAKE_FILE}")
		configure_file("cmake/CMakeLists.txt.in" "${AOC_CMAKE_FILE}" @ONLY)
	endif()

	foreach(DAY RANGE 1 ${NUMBER_OF_DAYS})
		aoc_get_filename_padded(${DAY} DAY)
		set(DAY_PATH "${CMAKE_CURRENT_LIST_DIR}/${YEAR}/day${DAY}.cpp")
		if(NOT EXISTS "${DAY_PATH}")
			file(WRITE "${DAY_PATH}" "${AOC_SOLUTION_TEMPLATE}")
		endif()
	endforeach()
endfunction()
