#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "vulkan/vulkan.h"

#include <stdint.h>
#include <string>
#include <vector>

enum class ShaderType : uint32_t;

class ShaderObject {
public:
    ShaderObject();
    ~ShaderObject();

    void setFilename(const std::string& filename) { this->filename = filename; }
    void setShaderType(const ShaderType& shaderType) { this->shaderType = shaderType; }

    void loadShaderFile();
    void compileShaderFile();
    void createModule(const VkDevice& device);

    size_t getCompiledSize() { return compiledSize; }
    std::vector<uint32_t>& getCompiledCode() { return compiledCode; }

    VkShaderModule& getShaderModule() { return shaderModule; }

private:
    std::string filename;
    ShaderType shaderType;

    std::vector<char> code;

    size_t compiledSize;
    std::vector<uint32_t> compiledCode;

    VkShaderModule shaderModule;
};



#endif // !SHADER_OBJECT_H
