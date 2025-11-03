set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:aoc>/2025")
foreach(DAY RANGE 1 12)
	add_solution(${DAY})
endforeach()
