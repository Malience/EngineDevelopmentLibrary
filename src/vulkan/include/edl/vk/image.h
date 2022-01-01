/*
This is the class file for vulkan pipeline objects
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>
#include <vector>

namespace edl {
namespace vk {

VkImage createImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
void transitionImage(VkCommandBuffer cb, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);

} // namespace vk
} // namespace edl