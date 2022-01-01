/*
This file contains vulkan surface functionality
*/
#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#include <cstdint>
#include <vector>

namespace edl {
namespace vk {

const uint32_t MAX_PRESENT_MODES = 7;
const uint32_t MAX_SURFACE_FORMATS = 8;

//TODO: Merge with Instance?
struct Surface {
    VkSurfaceKHR surface;

    uint32_t surfaceFormatsCount;
    VkSurfaceFormatKHR surfaceFormats[MAX_SURFACE_FORMATS];

    uint32_t presentModesCount;
    VkPresentModeKHR presentModes[MAX_PRESENT_MODES];

    //TODO: Might not need these
    uint32_t width, height;
    VkExtent2D extent;

    uint32_t minImageCount;
    uint32_t maxImageCount;

    VkSurfaceTransformFlagBitsKHR current_transform;

    void create(VkInstance instance, VkPhysicalDevice physicalDevice, uint32_t window_width, uint32_t window_height, void* window_handle);
    void destroy(VkInstance instance);

    bool getPhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;
    uint32_t Surface::getSurfaceImageCount(uint32_t requestedImageCount = 0) const;
};

} // namespace vk
} // namespace edl