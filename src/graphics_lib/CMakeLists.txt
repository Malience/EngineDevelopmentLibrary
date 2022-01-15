## Graphics Lib

add_library(graphics_lib STATIC
## Public Header Files
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/allocator.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/bindless_image_buffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/bindless_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/camera.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/descriptor_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/file_type.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/glfw_lib.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/glfw_defines.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/GlobalInfo.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/graphics_lib.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/image_table.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/io.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/mesh.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/model.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/NewShader.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/pipeline.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/ResourceHandles.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/ResourceSystem.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/shader.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/shader_object.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/ShaderLoader.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/ShaderReflection.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/StagingBuffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/StorageBuffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/UniformBufferObject.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/Util.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/edl/vertex_buffer.h
## Private Header Files
## Source Files
	${CMAKE_CURRENT_SOURCE_DIR}/allocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/descriptor_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/file_type.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/glfw_lib.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/graphics_lib.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/image_table.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/NewShader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pipeline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceHandles.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ResourceSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shader_object.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderLoader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderReflection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StagingBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StorageBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UniformBufferObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vertex_buffer.cpp
)
set_target_properties(graphics_lib PROPERTIES FOLDER "EngineLib")

target_include_directories(graphics_lib PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

add_library(EDL::graphics_lib ALIAS graphics_lib)

## Internal Dependencies
target_link_libraries(graphics_lib PRIVATE EDL::debug)
target_link_libraries(graphics_lib PRIVATE EDL::vulkan)
## External Dependencies
target_link_libraries(graphics_lib PRIVATE assimp::assimp)
target_link_libraries(graphics_lib PRIVATE glm::glm)
target_link_libraries(graphics_lib PRIVATE glfw)
target_link_libraries(graphics_lib PRIVATE shaderc)
target_link_libraries(graphics_lib PRIVATE stb::stb)
target_link_libraries(graphics_lib PRIVATE Vulkan::Vulkan)
##TEMPORARY
target_include_directories(graphics_lib PRIVATE "../../include")

## Install
include(GNUInstallDirs)

install(TARGETS graphics_lib 
    EXPORT edl-graphics_lib-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT edl-graphics_lib-targets
    FILE edl-graphics_lib-targets.cmake
    NAMESPACE EDL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/edl
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/edl
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)