#include "edl/shader_object.h"

#include "edl/io.h"
#include "edl/shader.h"
//#include "edl/vulkan.h"

ShaderObject::ShaderObject() {
    compiledSize = 0;

    shaderType = ShaderType::SHADER_TYPE_GLSL_VERTEX_SHADER;

    shaderModule = nullptr;

    filename = "";
}

ShaderObject::~ShaderObject() {

}

void ShaderObject::loadShaderFile() {
    IOResult result = loadFile(filename, code);
    if (result == IOResult::IO_FAILED) {
        printf("Failed to load shader: %s\n", filename.c_str());
        return;
    }
}

void ShaderObject::compileShaderFile() {
    compileShader(filename.c_str(), shaderType, code, compiledSize, compiledCode);
}

void ShaderObject::createModule(const VkDevice& device) {
    //createShaderModule(device, compiledSize, compiledCode, shaderModule);
}