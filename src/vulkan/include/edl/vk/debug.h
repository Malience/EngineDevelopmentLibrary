/*
This file contains functionality to debug vulkan systems
*/
#pragma once

#include "edl/logger.h"

#include "vulkan/vulkan.h"

namespace edl {
namespace vk {

//Prints the result of a vulkan API call
void checkResult(VkResult result, const std::string& action);

} // namespace vk
} // namespace edl