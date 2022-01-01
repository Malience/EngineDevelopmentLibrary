#ifndef SHADER_LIB_H
#define SHADER_LIB_H

#include <stdint.h>
#include <string>
#include <vector>

enum class ShaderType : uint32_t {
	SHADER_TYPE_GLSL_VERTEX_SHADER          = 0,
	SHADER_TYPE_GLSL_FRAGMENT_SHADER        = 1,
	SHADER_TYPE_GLSL_GEOMETRY_SHADER        = 2,
	SHADER_TYPE_GLSL_TESS_CONTROL_SHADER    = 3,
	SHADER_TYPE_GLSL_TESS_EVALUATION_SHADER = 4,
	SHADER_TYPE_GLSL_TASK_SHADER            = 4,
	SHADER_TYPE_GLSL_MESH_SHADER            = 6,
	SHADER_TYPE_GLSL_COMPUTE_SHADER         = 7,
	SHADER_TYPE_GLSL_RAYGEN_SHADER          = 8,
	SHADER_TYPE_GLSL_MISS_SHADER            = 9,
	SHADER_TYPE_GLSL_CLOSESTHIT_SHADER      = 10,
	SHADER_TYPE_GLSL_ANYHIT_SHADER          = 11,
	SHADER_TYPE_GLSL_INTERSECTION_SHADER    = 12,
};

const uint32_t SHADER_TYPE_NUM_SHADER_TYPES = 13;

void compileShader(const std::string& shader_name, const ShaderType shader_type, const std::vector<char>& shader_code, size_t& compiled_size, std::vector<uint32_t>& compiled_code);

#endif // !SHADER_LIB_H
