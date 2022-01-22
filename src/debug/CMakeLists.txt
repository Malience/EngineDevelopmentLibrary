## Debug Lib

add_library(debug STATIC
## Public Header Files
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/debug.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/logger.h
## Private Header Files
## Source Files
	${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
)
set_target_properties(debug PROPERTIES FOLDER "EngineLib")
set_target_properties(debug PROPERTIES LANGUAGE CXX LINKER_LANGUAGE CXX)

target_include_directories(debug PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_library(EDL::debug ALIAS debug)

## Internal Dependencies
## External Dependencies

## Install
include(GNUInstallDirs)

install(TARGETS debug 
    EXPORT edl-debug-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT edl-debug-targets
    FILE edl-debug-targets.cmake
    NAMESPACE EDL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/edl
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)