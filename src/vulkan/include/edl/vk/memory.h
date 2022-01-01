/*
This is the class file for vulkan buffer
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>

namespace edl {
namespace vk {

VkDeviceMemory allocateMemory(VkDevice device, VkDeviceSize size, uint32_t memoryType);
void bindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
VkBuffer createBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage);

} // namespace vk
} // namespace edl