/*
This is the class file for vulkan pipeline objects
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>
#include <vector>

namespace edl {
namespace vk {

VkDescriptorPool createDescriptorPool(VkDevice device, uint32_t maxSets, uint32_t size, VkDescriptorType type);

VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device, uint32_t bindingCount, const VkDescriptorSetLayoutBinding* bindings, VkDescriptorSetLayoutCreateFlags flags = 0);
VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t binding, VkShaderStageFlags stageFlags, VkDescriptorSetLayoutCreateFlags flags);
VkDescriptorSetLayout createBindlessImageDescriptorSetLayout(VkDevice device, uint32_t descriptorCount, uint32_t binding);

void allocateDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSetLayout* setLayouts, VkDescriptorSet* descriptorSets);
VkDescriptorSet allocateDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout);
VkDescriptorSet allocateBindlessDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, uint32_t descriptorCount);

} // namespace vk
} // namespace edl