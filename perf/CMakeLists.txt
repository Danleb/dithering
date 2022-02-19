add_executable(dithering_perf "")

file (GLOB COLLECTED_SOURCES "*.cpp")
target_sources(dithering_perf PRIVATE 
	${COLLECTED_SOURCES}
)

target_include_directories(dithering_perf PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)

CONAN_TARGET_LINK_LIBRARIES(dithering_perf)

target_link_libraries(dithering_perf
	dithering_lib
)
