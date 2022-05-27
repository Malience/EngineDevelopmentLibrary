#include "edl/shader.h"

#include "shaderc/shaderc.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace edl {

const shaderc_shader_kind SHADERC_SHADER_TYPES[static_cast<uint32_t>(ShaderType::SHADER_TYPE_NUM_SHADER_TYPES)] = {
	shaderc_shader_kind::shaderc_glsl_vertex_shader,          //SHADER_TYPE_GLSL_VERTEX_SHADER
	shaderc_shader_kind::shaderc_glsl_fragment_shader,        //SHADER_TYPE_GLSL_FRAGMENT_SHADER
	shaderc_shader_kind::shaderc_glsl_geometry_shader,        //SHADER_TYPE_GLSL_GEOMETRY_SHADER
	shaderc_shader_kind::shaderc_glsl_tess_control_shader,    //SHADER_TYPE_GLSL_TESS_CONTROL_SHADER
	shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader, //SHADER_TYPE_GLSL_TESS_EVALUATION_SHADER
	shaderc_shader_kind::shaderc_glsl_task_shader,            //SHADER_TYPE_GLSL_TASK_SHADER
	shaderc_shader_kind::shaderc_glsl_mesh_shader,            //SHADER_TYPE_GLSL_MESH_SHADER
	shaderc_shader_kind::shaderc_glsl_compute_shader,         //SHADER_TYPE_GLSL_COMPUTE_SHADER
	shaderc_shader_kind::shaderc_glsl_raygen_shader,          //SHADER_TYPE_GLSL_RAYGEN_SHADER
	shaderc_shader_kind::shaderc_glsl_miss_shader,            //SHADER_TYPE_GLSL_MISS_SHADER
	shaderc_shader_kind::shaderc_glsl_closesthit_shader,      //SHADER_TYPE_GLSL_CLOSESTHIT_SHADER
	shaderc_shader_kind::shaderc_glsl_anyhit_shader,          //SHADER_TYPE_GLSL_ANYHIT_SHADER
	shaderc_shader_kind::shaderc_glsl_intersection_shader,    //SHADER_TYPE_GLSL_INTERSECTION_SHADER
};

//Compiles shaders using shaderc
void compileShader(const std::string& name, ShaderType type, const std::vector<char>& code, std::vector<char>& spv) {
	shaderc_compiler_t compiler = shaderc_compiler_initialize();
	shaderc_compile_options_t options = shaderc_compile_options_initialize();

	shaderc_compile_options_set_source_language(options, shaderc_source_language_glsl);
	shaderc_compile_options_set_generate_debug_info(options);
	shaderc_compile_options_set_target_env(options, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

	const shaderc_shader_kind shaderc_type = SHADERC_SHADER_TYPES[static_cast<uint32_t>(type)];

	shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler, code.data(), code.size(), shaderc_type, name.c_str(), "main", options);
	shaderc_compilation_status status = shaderc_result_get_compilation_status(result);

	switch (status) {
	case shaderc_compilation_status_success:
		printf("Shader compiled successfully: %s\n", name.c_str());
		break;
	case shaderc_compilation_status_compilation_error:
		printf("Shader compilation error: %s\n", name.c_str());
		break;
	default:
		printf("Shader unknown error: %s\n", name.c_str());
		break;
	}
	if (status != shaderc_compilation_status_success) {
		printf(shaderc_result_get_error_message(result));
	}

	size_t size = shaderc_result_get_length(result);
	spv.resize(size);

	uint32_t* bytes = (uint32_t*)shaderc_result_get_bytes(result);

	printf(shaderc_result_get_error_message(result));

	memcpy(spv.data(), bytes, size);

	shaderc_result_release(result);
	shaderc_compile_options_release(options);
	shaderc_compiler_release(compiler);
}

}