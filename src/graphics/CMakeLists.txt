## Graphics Lib

add_library(Graphics STATIC
## Public Header Files
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/glfw_lib.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/glfw_defines.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/graphics_lib.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/resource.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/shader.h
## Private Header Files
## Source Files
	${CMAKE_CURRENT_SOURCE_DIR}/glfw_lib.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/graphics_lib.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
)
set_target_properties(Graphics PROPERTIES FOLDER "EngineFrame")

target_include_directories(Graphics PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_library(EDL::Graphics ALIAS Graphics)

## Internal Dependencies
target_link_libraries(Graphics PUBLIC EDL::Debug)
target_link_libraries(Graphics PUBLIC EDL::Util)
target_link_libraries(Graphics PUBLIC EDL::Vulkan)
## External Dependencies
target_link_libraries(Graphics PUBLIC EXT::jsonh)
target_link_libraries(Graphics PUBLIC EXT::rapidobj)
target_link_libraries(Graphics PUBLIC EXT::stb)
target_link_libraries(Graphics PRIVATE glm::glm)
target_link_libraries(Graphics PRIVATE glfw)
target_link_libraries(Graphics PRIVATE shaderc)

## Install
include(GNUInstallDirs)

install(TARGETS Graphics 
    EXPORT edl-graphics-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT edl-graphics-targets
    FILE edl-graphics-targets.cmake
    NAMESPACE EDL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/edl
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)