#include "edl/shader.h"

#include "shaderc/shaderc.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

const shaderc_shader_kind SHADERC_SHADER_TYPES[SHADER_TYPE_NUM_SHADER_TYPES] = {
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
void compileShader(const std::string& shader_name, const ShaderType shader_type, const std::vector<char>& shader_code, size_t& compiled_size, std::vector<uint32_t>& compiled_code) {
	shaderc_compiler_t compiler = shaderc_compiler_initialize();
	shaderc_compile_options_t options = shaderc_compile_options_initialize();

	shaderc_compile_options_set_source_language(options, shaderc_source_language_glsl);
	shaderc_compile_options_set_generate_debug_info(options);
	shaderc_compile_options_set_target_env(options, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_1);

	const shaderc_shader_kind shaderc_type = SHADERC_SHADER_TYPES[static_cast<uint32_t>(shader_type)];

	shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler, shader_code.data(), shader_code.size(), shaderc_type, shader_name.c_str(), "main", options);
	shaderc_compilation_status status = shaderc_result_get_compilation_status(result);


	switch (status) {
	case shaderc_compilation_status_success:
		printf("Shader compiled successfully: %s\n", shader_name.c_str());
		break;
	case shaderc_compilation_status_compilation_error:
		printf("Shader compilation error: %s\n", shader_name.c_str());
		break;
	default:
		printf("Shader unknown error: %s\n", shader_name.c_str());
		break;
	}
	if (status != shaderc_compilation_status_success) {
		printf(shaderc_result_get_error_message(result));
	}
	

	compiled_size = shaderc_result_get_length(result);

	compiled_code.resize(compiled_size / sizeof(uint32_t));

	uint32_t* bytes = (uint32_t*) shaderc_result_get_bytes(result);

	printf(shaderc_result_get_error_message(result));

	memcpy(compiled_code.data(), bytes, compiled_size);

	shaderc_result_release(result);
	shaderc_compile_options_release(options);
	shaderc_compiler_release(compiler);
}