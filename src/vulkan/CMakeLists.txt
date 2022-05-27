## Vulkan Objects Lib

add_library(Vulkan STATIC
## Public Header Files
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/command.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/debug.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/descriptor.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/function.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/image.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/instance.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/memory.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/pipeline.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/queue.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/renderpass.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/surface.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/swapchain.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vk/vulkan.h
## Private Header Files
## Source Files
	${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/debug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/descriptor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/function.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/image.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/instance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/memory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pipeline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/queue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderpass.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/swapchain.cpp
)
set_target_properties(Vulkan PROPERTIES FOLDER "EngineLib")

target_include_directories(Vulkan PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_library(EDL::Vulkan ALIAS Vulkan)

## Internal Dependencies
target_link_libraries(Vulkan PUBLIC EDL::Debug)
## External Dependencies
target_compile_definitions(Vulkan PRIVATE vulkan_USE_PLATFORM_WIN32_KHR)
target_link_libraries(Vulkan PUBLIC Vulkan::Vulkan)
target_link_libraries(Vulkan PRIVATE glm::glm)
#target_link_libraries(vulkan PRIVATE vma)

## Install
include(GNUInstallDirs)

install(TARGETS Vulkan 
    EXPORT edl-vulkan-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT edl-vulkan-targets
    FILE edl-vulkan-targets.cmake
    NAMESPACE EDL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/edl
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)