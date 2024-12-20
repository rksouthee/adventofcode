function(add_solution day)
	get_filename_component(AOC_YEAR ${CMAKE_CURRENT_LIST_DIR} NAME)
	string(LENGTH "${day}" DAY_LENGTH)
	if(${DAY_LENGTH} EQUAL 1)
		string(PREPEND day "0")
	endif()
	set(AOC_TARGET_NAME "${AOC_YEAR}_day${day}")
	add_library(${AOC_TARGET_NAME} MODULE "day${day}.cpp")
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
