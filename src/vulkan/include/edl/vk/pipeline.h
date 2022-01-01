/*
This is the class file for vulkan pipeline objects
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>
#include <vector>

namespace edl {
namespace vk {

const uint32_t MAX_PIPELINE_SHADERS = 5;
const uint32_t MAX_PIPELINE_BINDINGS = 16;
const uint32_t MAX_PIPELINE_ATTRIBUTES = 16;
const uint32_t MAX_PIPELINE_ATTACHMENTS = 16;

struct PipelineBuilder {
    VkDevice device;

    uint32_t shadersCount;
    VkPipelineShaderStageCreateInfo shaderInfos[MAX_PIPELINE_SHADERS];
    uint32_t width, height;

    uint32_t bindingsCount;
    VkVertexInputBindingDescription bindings[MAX_PIPELINE_BINDINGS];

    uint32_t attributesCount;
    VkVertexInputAttributeDescription attributes[MAX_PIPELINE_ATTRIBUTES];

    uint32_t attachmentsCount;
    VkFormat attachments[MAX_PIPELINE_ATTACHMENTS];
    VkFormat depthStencilAttachment;

    void setDevice(VkDevice device);
    void setShaders(uint32_t count, const VkShaderModule* modules, const VkShaderStageFlagBits* stages);
    void setDimensions(uint32_t width, uint32_t height);
    void setBindingsCount(uint32_t count);
    void setBinding(uint32_t index, uint32_t binding, uint32_t stride);
    void setAttributesCount(uint32_t count);
    void setAttribute(uint32_t index, uint32_t location, uint32_t binding, VkFormat format, uint32_t offset);
    void setAttachmentCount(uint32_t count);
    void setAttachment(uint32_t index, VkFormat format);
    void setDepthStencilAttachment(VkFormat format);

    VkPipeline create(VkPipelineLayout layout);
};

VkPipelineLayout createPipelineLayout(VkDevice device, uint32_t setLayoutCount, const VkDescriptorSetLayout* pSetLayouts);

} // namespace vk
} // namespace edl