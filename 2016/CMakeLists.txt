set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:aoc>/2016")
foreach(i RANGE 1 25)
	add_solution(${i})
endforeach()
