/*
This is the class file for vulkan renderpass objects
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>

namespace edl {
namespace vk {

VkRenderPass createRenderPass(VkDevice device, VkFormat format);

} // namespace vk
} // namespace edl