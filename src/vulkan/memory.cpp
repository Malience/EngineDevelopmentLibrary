#include "edl/vk/memory.h"

#include "edl/vk/debug.h"

namespace edl {
namespace vk {

//TODO: add descriptors and push constants
VkDeviceMemory allocateMemory(VkDevice device, VkDeviceSize size, uint32_t memoryType) {
	// TEMP: Kinda a hack
	VkMemoryAllocateFlagsInfo flagsInfo = {};
	flagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
	flagsInfo.pNext = nullptr;
	flagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
	flagsInfo.deviceMask = 0x0;

	VkMemoryAllocateInfo allocateInfo;
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = &flagsInfo;
	allocateInfo.allocationSize = size;
	allocateInfo.memoryTypeIndex = memoryType;

	VkDeviceMemory memory;
	checkResult(vkAllocateMemory(device, &allocateInfo, NULL, &memory), "VK memory allocation");
	return memory;
}

void bindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
	checkResult(vkBindBufferMemory(device, buffer, memory, memoryOffset), "VK Memory binding");
}

VkBuffer createBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage) {
	VkBufferCreateInfo bufferInfo;
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = NULL;
	bufferInfo.flags = 0;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices = NULL;

	VkBuffer buffer;
	checkResult(vkCreateBuffer(device, &bufferInfo, NULL, &buffer), "VK Buffer creation");
	return buffer;
}

} // namespace vk
} // namespace edl