add_library(dithering_lib STATIC "")

file (GLOB COLLECTED_SOURCES "*.cpp")
target_sources(dithering_lib PRIVATE 
	${COLLECTED_SOURCES}
)

target_include_directories(dithering_lib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../
)

CONAN_TARGET_LINK_LIBRARIES(dithering_lib )
