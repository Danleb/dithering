macro(InstallAndSetupConanPackages)
	InstallConanPackages()
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
endmacro()

#=====================================================
macro(InstallConanPackages)
	find_program(CONAN_PATH conan)
	if(NOT EXISTS ${CONAN_PATH})
		message(FATAL_ERROR "Conan package manager is required. Please run this command to install and setup conan: pip install conan")
	else()
		message(STATUS "Conan path = ${CONAN_PATH}")
	endif()
    
	execute_process(COMMAND ${CONAN_PATH} install ${CMAKE_CURRENT_SOURCE_DIR} -if ${CMAKE_BINARY_DIR} -s build_type=${CMAKE_BUILD_TYPE} --build=missing RESULT_VARIABLE return_code)
	#execute_process(COMMAND ${CONAN_PATH} install ${CMAKE_CURRENT_SOURCE_DIR} -if ${CMAKE_BINARY_DIR} -s build_type=${CMAKE_BUILD_TYPE} RESULT_VARIABLE return_code)
	message("Conan packages install return code:" ${return_code})	
endmacro()
