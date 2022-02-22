add_executable(dithering_app "")

file(CREATE_LINK
    ${PROJECT_SOURCE_DIR}/configs
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/configs
    SYMBOLIC)

file (GLOB COLLECTED_SOURCES "*.cpp")
target_sources(dithering_app PRIVATE 
	${COLLECTED_SOURCES}
)

target_include_directories(dithering_app PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dithering_app
	dithering_lib
)

CONAN_TARGET_LINK_LIBRARIES(dithering_app)
