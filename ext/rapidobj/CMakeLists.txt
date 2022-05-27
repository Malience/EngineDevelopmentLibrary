## External Header-Only Libraries
add_library(rapidobj INTERFACE)

target_include_directories(rapidobj INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
add_library(EXT::rapidobj ALIAS rapidobj)

## Install
include(GNUInstallDirs)

install(TARGETS rapidobj 
    EXPORT ext-rapidobj-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT ext-rapidobj-targets
    FILE ext-rapidobj-targets.cmake
    NAMESPACE EXT::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)