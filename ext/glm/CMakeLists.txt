add_subdirectory(glm)

## Install
include(GNUInstallDirs)

install(TARGETS glm 
    EXPORT ext-glm-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT ext-glm-targets
    FILE ext-glm-targets.cmake
    NAMESPACE EXT::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)