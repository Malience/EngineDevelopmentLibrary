#include "edl/vk/surface.h"

#include "edl/vk/debug.h"

#include <assert.h>

namespace edl {
namespace vk {

void Surface::create(VkInstance instance, VkPhysicalDevice physicalDevice, uint32_t window_width, uint32_t window_height, void* window_handle) {
	VkWin32SurfaceCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.pNext = NULL;
	info.flags = 0;
	info.hinstance = GetModuleHandle(NULL);
	info.hwnd = (HWND)window_handle;

	checkResult(vkCreateWin32SurfaceKHR(instance, &info, NULL, &surface), "Surface creation");

	width = window_width;
	height = window_height;

	//Get Capabilities
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

	//Find Surface Format
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, NULL);
	assert(surfaceFormatsCount <= MAX_SURFACE_FORMATS);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, surfaceFormats);

	//Find Present Mode
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, NULL);
	assert(presentModesCount <= MAX_PRESENT_MODES);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, presentModes);

	//Determine swapchain extent
	if (capabilities.currentExtent.width == UINT32_MAX) {
		extent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, width));
		extent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, height));
	}
	else {
		extent.width = capabilities.currentExtent.width;
		extent.height = capabilities.currentExtent.height;
	}

	minImageCount = capabilities.minImageCount;
	maxImageCount = capabilities.maxImageCount;

	current_transform = capabilities.currentTransform;
}

//TODO: Flesh out
void Surface::destroy(VkInstance instance) {
	vkDestroySurfaceKHR(instance, surface, NULL);
}

bool Surface::getPhysicalDeviceSurfaceSupport(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const {
	VkBool32 present_support;
	checkResult(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &present_support), "Surface support");
	return present_support;
}

uint32_t Surface::getSurfaceImageCount(uint32_t requestedImageCount) const {
	requestedImageCount = max(minImageCount, requestedImageCount);
	if (maxImageCount > 0 && requestedImageCount > maxImageCount)
		return maxImageCount;
	return requestedImageCount;
}

} // namespace vk
} // namespace edl