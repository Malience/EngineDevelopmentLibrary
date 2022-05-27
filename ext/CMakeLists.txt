add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER "Externals/GLFW")
set_target_properties(uninstall PROPERTIES FOLDER "Externals/GLFW")
set_target_properties(update_mappings PROPERTIES FOLDER "Externals/GLFW")

add_subdirectory(glm)
add_subdirectory(jsonh)
add_subdirectory(rapidobj)
add_subdirectory(stb)

