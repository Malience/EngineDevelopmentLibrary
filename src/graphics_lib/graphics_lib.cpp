#include "edl/graphics_lib.h"
/*
void loadShader(const char* filename, const ShaderType shaderType, VkShaderModule *shaderModule) {
	int size;
	char* code;
	uint32_t compiledSize;
	uint32_t* compiledCode;

	IOResult result = loadFile(filename, &size, &code);
	if (result == IOResult::IO_FAILED) {
		printf("Failed to load shader: %s\n", filename);
		return;
	}
	compileShader(filename, size, code, shaderType, &compiledSize, &compiledCode);
	createShaderModule(compiledSize, compiledCode, shaderModule);
}
*/