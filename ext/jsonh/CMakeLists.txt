## External Header-Only Libraries
add_library(jsonh INTERFACE)

target_include_directories(jsonh INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
add_library(EXT::jsonh ALIAS jsonh)

## Install
include(GNUInstallDirs)

install(TARGETS jsonh 
    EXPORT ext-jsonh-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT ext-jsonh-targets
    FILE ext-jsonh-targets.cmake
    NAMESPACE EXT::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)