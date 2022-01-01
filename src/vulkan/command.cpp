#include "edl/vk/command.h"

#include "edl/vk/debug.h"

namespace edl {
namespace vk {

VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamily, const VkCommandPoolCreateFlags flags) {
	VkCommandPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolCreateInfo.pNext = NULL;
	poolCreateInfo.flags = flags;
	poolCreateInfo.queueFamilyIndex = queueFamily;

	VkCommandPool commandPool;
	checkResult(vkCreateCommandPool(device, &poolCreateInfo, NULL, &commandPool), "Command pool creation");
	return commandPool;
}


void createCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBuffersCount, VkCommandBuffer* commandBuffers, VkCommandBufferLevel commandBufferLevel) {
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = NULL;
	commandBufferAllocateInfo.level = commandBufferLevel;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.commandBufferCount = commandBuffersCount;

	checkResult(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers), "Command buffer allocation");
}

} // namespace vk
} // namespace edl