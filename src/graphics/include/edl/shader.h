#pragma once

#include "vulkan/vulkan.h"

#include <stdint.h>
#include <string>
#include <vector>

namespace edl {

enum class ShaderType : uint32_t {
	SHADER_TYPE_GLSL_VERTEX_SHADER = 0,
	SHADER_TYPE_GLSL_FRAGMENT_SHADER = 1,
	SHADER_TYPE_GLSL_GEOMETRY_SHADER = 2,
	SHADER_TYPE_GLSL_TESS_CONTROL_SHADER = 3,
	SHADER_TYPE_GLSL_TESS_EVALUATION_SHADER = 4,
	SHADER_TYPE_GLSL_TASK_SHADER = 4,
	SHADER_TYPE_GLSL_MESH_SHADER = 6,
	SHADER_TYPE_GLSL_COMPUTE_SHADER = 7,
	SHADER_TYPE_GLSL_RAYGEN_SHADER = 8,
	SHADER_TYPE_GLSL_MISS_SHADER = 9,
	SHADER_TYPE_GLSL_CLOSESTHIT_SHADER = 10,
	SHADER_TYPE_GLSL_ANYHIT_SHADER = 11,
	SHADER_TYPE_GLSL_INTERSECTION_SHADER = 12,

	SHADER_TYPE_NUM_SHADER_TYPES = 13,
};

inline ShaderType parseShaderType(const std::string& type) {
	if (type == "vertex" || type == "vert") return ShaderType::SHADER_TYPE_GLSL_VERTEX_SHADER;
	if (type == "fragment" || type == "frag") return ShaderType::SHADER_TYPE_GLSL_FRAGMENT_SHADER;
	if (type == "geometry" || type == "geom") return ShaderType::SHADER_TYPE_GLSL_GEOMETRY_SHADER;
	if (type == "tess_control" || type == "control" || type == "cont") return ShaderType::SHADER_TYPE_GLSL_TESS_CONTROL_SHADER;
	if (type == "tess_evaluation" || type == "evaluation" || type == "eval") return ShaderType::SHADER_TYPE_GLSL_TESS_EVALUATION_SHADER;
	if (type == "task") return ShaderType::SHADER_TYPE_GLSL_TASK_SHADER;
	if (type == "mesh") return ShaderType::SHADER_TYPE_GLSL_TASK_SHADER;
	if (type == "compute" || type == "comp") return ShaderType::SHADER_TYPE_GLSL_COMPUTE_SHADER;
	if (type == "raygen" || type == "gen") return ShaderType::SHADER_TYPE_GLSL_RAYGEN_SHADER;
	if (type == "miss") return ShaderType::SHADER_TYPE_GLSL_RAYGEN_SHADER;
	if (type == "closesthit" || type == "chit") return ShaderType::SHADER_TYPE_GLSL_CLOSESTHIT_SHADER;
	if (type == "anyhit" || type == "ahit") return ShaderType::SHADER_TYPE_GLSL_ANYHIT_SHADER;
	if (type == "intersection" || type == "int") return ShaderType::SHADER_TYPE_GLSL_INTERSECTION_SHADER;
}

void compileShader(const std::string& name, ShaderType type, const std::vector<char>& code, std::vector<char>& spv);

}