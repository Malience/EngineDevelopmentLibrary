## Debug Lib

add_library(Debug STATIC
## Public Header Files
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/Debug.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/Logger.h
## Private Header Files
## Source Files
	${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
)
set_target_properties(Debug PROPERTIES FOLDER "EngineLib")
set_target_properties(Debug PROPERTIES LANGUAGE CXX LINKER_LANGUAGE CXX)

target_include_directories(Debug PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_library(EDL::Debug ALIAS Debug)

## Internal Dependencies
## External Dependencies

## Install
include(GNUInstallDirs)

install(TARGETS Debug 
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