/*
This file contains functionality to debug vulkan systems
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>
#include <vector>

namespace edl {
namespace vk {

struct Surface;

const uint32_t MAX_SWAPCHAIN_IMAGES = 5;


//TODO: Redo how recreate works
//TODO: Maybe transition?
//TODO: Destruction
//TODO: Merge with Instance? / Surface?
struct Swapchain {
    VkSwapchainKHR swapchain;
    VkPresentModeKHR presentMode;

    VkExtent2D extent;

    //TODO: Replace with VkSurfaceFormatKHR?
    VkFormat format;
    VkColorSpaceKHR colorSpace;

    uint32_t imageCount;
    VkImage images[MAX_SWAPCHAIN_IMAGES];

    void create(VkPhysicalDevice physicalDevice, VkDevice device, const Surface& surface);
};

} // namespace vk
} // namespace edl