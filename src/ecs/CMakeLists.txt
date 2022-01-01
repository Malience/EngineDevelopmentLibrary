
file(GLOB_RECURSE ECS_SOURCES *.h *.c *.cpp)

add_library(ecs STATIC ${ECS_SOURCES})

target_link_libraries(ecs cglm)

set_target_properties(ecs PROPERTIES FOLDER "EngineLib")

target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})