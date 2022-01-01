#include "edl/vk/swapchain.h"

#include "edl/vk/debug.h"
#include "edl/vk/surface.h"

#include "vulkan/vulkan.h"

#include <assert.h>

namespace edl {
namespace vk {

void Swapchain::create(VkPhysicalDevice physicalDevice, VkDevice device, const Surface& surface) {
	surface.getPhysicalDeviceSurfaceSupport(physicalDevice, 0); //TODO: Don't do this

	for (uint32_t i = 0; i < surface.surfaceFormatsCount; i++) {
		if (surface.surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM && surface.surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			format = surface.surfaceFormats[i].format;
			colorSpace = surface.surfaceFormats[i].colorSpace;
			break;
		}
	}

	presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (uint32_t i = 0; i < surface.presentModesCount; i++) {
		if (surface.presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			break;

		} if (surface.presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}

	extent = surface.extent;

	VkSwapchainCreateInfoKHR swapchainCreateInfo;
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.pNext = NULL;

	swapchainCreateInfo.surface = surface.surface;
	swapchainCreateInfo.minImageCount = surface.getSurfaceImageCount();
	swapchainCreateInfo.imageFormat = format;
	swapchainCreateInfo.imageColorSpace = colorSpace;
	swapchainCreateInfo.imageExtent = extent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = NULL;
	swapchainCreateInfo.preTransform = surface.current_transform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	checkResult(vkCreateSwapchainKHR(device, &swapchainCreateInfo, NULL, &swapchain), "Swapchain creation");

	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL);
	assert(imageCount <= MAX_SWAPCHAIN_IMAGES);
	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images);
}


} // namespace vk
} // namespace edl