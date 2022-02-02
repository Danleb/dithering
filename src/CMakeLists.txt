add_executable(dithering_app "")

file (GLOB COLLECTED_SOURCES "*.cpp")
target_sources(dithering_app PRIVATE 
	${COLLECTED_SOURCES}
)

target_include_directories(dithering_app PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)

#target_link_libraries(dithering_app
#	dithering
#)

CONAN_TARGET_LINK_LIBRARIES(dithering_app)
