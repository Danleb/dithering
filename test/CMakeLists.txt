add_executable(dithering_test "")

message(BINARY FOLDER: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

file(CREATE_LINK
    ${PROJECT_SOURCE_DIR}/pics
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pics
    SYMBOLIC)

file (GLOB COLLECTED_SOURCES "*.cpp")
target_sources(dithering_test PRIVATE 
	${COLLECTED_SOURCES}
 "main.cpp")

target_include_directories(dithering_test PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)

CONAN_TARGET_LINK_LIBRARIES(dithering_test)

target_link_libraries(dithering_test
	dithering_lib
)

enable_testing()
add_test(NAME dithering_test
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	COMMAND dithering_test
)
