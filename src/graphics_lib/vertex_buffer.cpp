#include "edl/vertex_buffer.h"

//#include "edl/vulkan_constant.h"

#include "vulkan/vulkan.h"
/*
void createVertexBuffer(
	VkCommandPool commandPool,
	const uint32_t indicesCount,
	const uint32_t attributeCount,
	const VkDeviceSize* pOffsets,
	const VkDeviceSize size,
	const void *data,
	VertexBuffer *vertexBuffer) {

	//Set half of the variables in vertexBuffer, buffer and memory will be set later
	vertexBuffer->indicesCount = indicesCount;
	vertexBuffer->attributeCount = attributeCount;
	vertexBuffer->pOffsets = (VkDeviceSize *) malloc(attributeCount * sizeof(VkDeviceSize));
	memcpy(vertexBuffer->pOffsets, pOffsets, attributeCount * sizeof(VkDeviceSize));

	VkBuffer stagingBuffer;
	VulkanMemory stagingMemory;
	/*
	createVulkanBuffer(size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer, &stagingMemory);

	mapBufferToVulkanMemory(stagingMemory, size, data);

	createVulkanBuffer(size,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&vertexBuffer->buffer, &vertexBuffer->memory);

	VkCommandBuffer cb;
	allocateCommandBuffer(commandPool, &cb);
	vkBeginCommandBuffer(cb, &COMMAND_BUFFER_BEGIN_INFO);

	copyBuffer(cb, stagingBuffer, vertexBuffer->buffer, size);

	vkEndCommandBuffer(cb);
	submitGraphicsQueue(0, 0, NULL, NULL, 0, NULL, 1, &cb, NULL);

	//TODO: THIS
	graphicsQueueWaitIdle(0);
	freeCommandBuffers(commandPool, 1, &cb);

	destroyVulkanBuffer(stagingBuffer, stagingMemory);
	*//*
}

void destroyVertexBuffer(VertexBuffer *vertexBuffer) {

}
*/