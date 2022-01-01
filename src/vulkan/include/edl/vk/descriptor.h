/*
This is the class file for vulkan pipeline objects
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>
#include <vector>

namespace edl {
namespace vk {

VkDescriptorPool createDescriptorPool(VkDevice device, uint32_t maxSets, uint32_t uniformBufferPoolSize, uint32_t imageSamplerPoolSize);
VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device, uint32_t bindingCount, const VkDescriptorSetLayoutBinding* bindings);
void allocateDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSetLayout* setLayouts, VkDescriptorSet* descriptorSets);

} // namespace vk
} // namespace edl