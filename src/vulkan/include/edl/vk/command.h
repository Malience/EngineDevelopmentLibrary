/*
This file contains the VulkanCommandPool class
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>

namespace edl {
namespace vk {

VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamily, const VkCommandPoolCreateFlags flags);
void createCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBuffersCount, VkCommandBuffer* commandBuffers, VkCommandBufferLevel commandBufferLevel = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

} // namespace vk
} // namespace edl