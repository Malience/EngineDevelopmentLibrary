#include "edl/vk/descriptor.h"

#include "edl/vk/debug.h"

#include <assert.h>

namespace edl {
namespace vk {

VkDescriptorPool createDescriptorPool(VkDevice device, uint32_t maxSets, uint32_t uniformBufferPoolSize, uint32_t imageSamplerPoolSize) {
	assert(maxSets > 0);
	assert(uniformBufferPoolSize > 0 || imageSamplerPoolSize > 0);

	VkDescriptorPoolCreateInfo poolCreateInfo;
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.pNext = NULL;
	poolCreateInfo.flags = 0;
	poolCreateInfo.maxSets = maxSets;

	if (uniformBufferPoolSize > 0 && imageSamplerPoolSize > 0) {
		VkDescriptorPoolSize poolSizes[] = 
		{ { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, uniformBufferPoolSize },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, imageSamplerPoolSize } };

		poolCreateInfo.poolSizeCount = 2;
		poolCreateInfo.pPoolSizes = poolSizes;
	}
	else if (uniformBufferPoolSize > 0) {
		VkDescriptorPoolSize poolSizes[] =
		{ { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, uniformBufferPoolSize } };

		poolCreateInfo.poolSizeCount = 1;
		poolCreateInfo.pPoolSizes = poolSizes;
	}
	else if (imageSamplerPoolSize > 0) {
		VkDescriptorPoolSize poolSizes[] =
		{ { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, imageSamplerPoolSize } };

		poolCreateInfo.poolSizeCount = 1;
		poolCreateInfo.pPoolSizes = poolSizes;
	}
	VkDescriptorPool descriptorPool;
	checkResult(vkCreateDescriptorPool(device, &poolCreateInfo, NULL, &descriptorPool), "Descriptor pool creation");
	return descriptorPool;
}

VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device, uint32_t bindingCount, const VkDescriptorSetLayoutBinding* bindings) {
	VkDescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = NULL;
	layoutInfo.flags = 0;
	layoutInfo.bindingCount = bindingCount;
	layoutInfo.pBindings = bindings;

	VkDescriptorSetLayout descriptorSetLayout;
	checkResult(vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout), "Descriptor set layout creation");
	return descriptorSetLayout;
}

void allocateDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSetLayout* setLayouts, VkDescriptorSet* descriptorSets) {
	VkDescriptorSetAllocateInfo setAllocateInfo;
	setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocateInfo.pNext = NULL;
	setAllocateInfo.descriptorPool = descriptorPool;
	setAllocateInfo.descriptorSetCount = descriptorSetCount;
	setAllocateInfo.pSetLayouts = setLayouts;

	checkResult(vkAllocateDescriptorSets(device, &setAllocateInfo, descriptorSets), "Descriptor set allocation");
}

} // namespace vk
} // namespace edl