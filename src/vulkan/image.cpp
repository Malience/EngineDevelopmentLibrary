#include "edl/vk/image.h"

#include "edl/vk/debug.h"

namespace edl {
namespace vk {

VkImage createImage(VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage) {
	VkImageCreateInfo imageInfo;
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.pNext = NULL;
	imageInfo.flags = 0;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.queueFamilyIndexCount = 0;
	imageInfo.pQueueFamilyIndices = NULL;

	VkImage image;
	checkResult(vkCreateImage(device, &imageInfo, nullptr, &image), "Image creation");
	return image;
}

void transitionImage(VkCommandBuffer cb, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
	VkImageMemoryBarrier memoryBarrier;
	memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	memoryBarrier.pNext = NULL;

	memoryBarrier.oldLayout = oldLayout;
	memoryBarrier.newLayout = newLayout;

	//Eventually this will matter
	memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	memoryBarrier.image = image;
	memoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

	VkPipelineStageFlagBits srcMask, dstMask;
	//TODO: Fix this mess
	switch (oldLayout) {
	case VK_IMAGE_LAYOUT_UNDEFINED:
		memoryBarrier.srcAccessMask = 0;
		srcMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		srcMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
		break;
	}

	switch (newLayout) {
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		dstMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
		break;
	case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
		memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		//TODO: We could possible use the image in other pipeline stages
		dstMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		break;
	case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
		memoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dstMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;// VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		break;
	case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
		memoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dstMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;// VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		memoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 };
		break;
	}

	vkCmdPipelineBarrier(cb, srcMask, dstMask, 0, 0, NULL, 0, NULL, 1, &memoryBarrier);
}

VkImageView createImageView(VkDevice device, VkImage image, VkFormat format) {
	VkImageViewCreateInfo viewInfo;
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.pNext = NULL;
	viewInfo.flags = 0;

	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;

	viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	viewInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };

	VkImageView imageview;
	checkResult(vkCreateImageView(device, &viewInfo, NULL, &imageview), "ImageView creation");
	return imageview;
}

} // namespace vk
} // namespace edl