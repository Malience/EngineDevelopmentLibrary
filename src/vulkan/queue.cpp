#include "edl/vk/queue.h"

#include "edl/vk/debug.h"

#include "vulkan/vulkan.h"

//TODO: Anything here
namespace edl {
namespace vk {

//
//VulkanQueue::VulkanQueue() {
//	vulkan_queue = nullptr;
//}
//
//VulkanQueue::~VulkanQueue() {
//
//}
//
//void VulkanQueue::present(const std::vector<VkSemaphore> waitSemaphores, const vk::Swapchain& swapchain, const uint32_t* pImageIndices) {
//	VkPresentInfoKHR presentInfo = {};
//	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//	presentInfo.pNext = NULL;
//	presentInfo.waitSemaphoreCount = waitSemaphores.size();
//	presentInfo.pWaitSemaphores = waitSemaphores.data();
//	presentInfo.swapchainCount = 1;
//	presentInfo.pSwapchains = &swapchain.swapchain;
//	presentInfo.pImageIndices = pImageIndices;
//	presentInfo.pResults = NULL;
//
//	if (vkQueuePresentKHR(vulkan_queue, &presentInfo) != VK_SUCCESS) {
//		//TODO: FIX THIS
//		printf("Failed to present!\n");
//	}
//}

} // namespace vk
} // namespace edl