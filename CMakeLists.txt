cmake_minimum_required(VERSION 3.20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX "d")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(EDL 
	LANGUAGES C CXX
	VERSION 0.1.0
)

find_package(stb REQUIRED FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED FATAL_ERROR)
find_package(Vulkan REQUIRED FATAL_ERROR)

if (NOT TARGET shaderc)
## ShaderC
find_path(shaderc_INCLUDE_DIR
    NAMES shaderc/shaderc.h
    HINTS "$ENV{VULKAN_SDK}/include")
find_library(shaderc_LIBRARY
    NAMES shaderc_combined
    HINTS "$ENV{VULKAN_SDK}/lib")

add_library(shaderc STATIC IMPORTED)
set_target_properties(shaderc PROPERTIES IMPORTED_LOCATION_RELEASE "$ENV{VULKAN_SDK}/lib/shaderc_combined.lib")
set_target_properties(shaderc PROPERTIES IMPORTED_LOCATION_DEBUG "$ENV{VULKAN_SDK}/lib/shaderc_combinedd.lib")
set_target_properties(shaderc PROPERTIES IMPORTED_LOCATION_MINSIZEREL "$ENV{VULKAN_SDK}/lib/shaderc_combined.lib")
set_target_properties(shaderc PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO "$ENV{VULKAN_SDK}/lib/shaderc_combined.lib")
set_target_properties(shaderc PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${shaderc_INCLUDE_DIR}")

endif()

## Build libraries
add_subdirectory(src)

#Testing
#enable_testing()
#add_subdirectory(test)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/edl-config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/edl-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/edl-config-version.cmake
    VERSION ${EDL_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        ${CMAKE_BINARY_DIR}/cmake/edl-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/edl-config-version.cmake
	CONFIGURATIONS Release Debug
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)

