## Util Lib

add_library(Util STATIC
## Public Header Files
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/allocator.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/io.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/util.h
## Private Header Files
## Source Files
	${CMAKE_CURRENT_SOURCE_DIR}/allocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
)
set_target_properties(Util PROPERTIES FOLDER "EngineLib")
set_target_properties(Debug PROPERTIES LANGUAGE CXX LINKER_LANGUAGE CXX)

target_include_directories(Util PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_library(EDL::Util ALIAS Util)

## Internal Dependencies
target_link_libraries(Util PUBLIC EDL::Debug)
## External Dependencies

## Install
include(GNUInstallDirs)

install(TARGETS Util 
    EXPORT edl-util-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT edl-util-targets
    FILE edl-util-targets.cmake
    NAMESPACE EDL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/edl
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)