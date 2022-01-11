#include "edl/vk/descriptor.h"

#include "edl/vk/debug.h"

#include <assert.h>

namespace edl {
namespace vk {

VkDescriptorPool createDescriptorPool(VkDevice device, uint32_t maxSets, uint32_t size, VkDescriptorType type) {
	assert(maxSets > 0 && size > 0);

	VkDescriptorPoolCreateInfo poolCreateInfo;
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.pNext = NULL;
	poolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
	poolCreateInfo.maxSets = maxSets;

	VkDescriptorPoolSize poolSizes[] =
	{ { type, size } };

	poolCreateInfo.poolSizeCount = 1;
	poolCreateInfo.pPoolSizes = poolSizes;

	VkDescriptorPool descriptorPool;
	checkResult(vkCreateDescriptorPool(device, &poolCreateInfo, NULL, &descriptorPool), "Descriptor pool creation");
	return descriptorPool;
}

VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device, uint32_t bindingCount, const VkDescriptorSetLayoutBinding* bindings, VkDescriptorSetLayoutCreateFlags flags) {
	VkDescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = NULL;
	layoutInfo.flags = flags;
	layoutInfo.bindingCount = bindingCount;
	layoutInfo.pBindings = bindings;

	VkDescriptorSetLayout descriptorSetLayout;
	checkResult(vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout), "Descriptor set layout creation");
	return descriptorSetLayout;
}

VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device, VkDescriptorType type, uint32_t descriptorCount, uint32_t binding, VkShaderStageFlags stageFlags, VkDescriptorSetLayoutCreateFlags flags) {
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
	layoutBinding.descriptorCount = descriptorCount;
	layoutBinding.binding = binding;
	layoutBinding.stageFlags = VK_SHADER_STAGE_ALL;
	layoutBinding.pImmutableSamplers = nullptr;
	
	VkDescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = NULL;
	layoutInfo.flags = flags;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &layoutBinding;

	VkDescriptorSetLayout descriptorSetLayout;
	checkResult(vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout), "Descriptor set layout creation");
	return descriptorSetLayout;
}

VkDescriptorSetLayout createBindlessImageDescriptorSetLayout(VkDevice device, uint32_t descriptorCount, uint32_t binding) {
	
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	layoutBinding.descriptorCount = descriptorCount;
	layoutBinding.binding = binding;
	layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	layoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorBindingFlags flags = 
		VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT
		| VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT
		| VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;

	VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlags = {};
	bindingFlags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
	bindingFlags.pNext = 0;
	bindingFlags.bindingCount = 1;
	bindingFlags.pBindingFlags = &flags;
	
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = &bindingFlags;
	layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &layoutBinding;

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

VkDescriptorSet allocateDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout) {
	VkDescriptorSetAllocateInfo setAllocateInfo;
	setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocateInfo.pNext = NULL;
	setAllocateInfo.descriptorPool = descriptorPool;
	setAllocateInfo.descriptorSetCount = 1;
	setAllocateInfo.pSetLayouts = &descriptorSetLayout;

	VkDescriptorSet descriptorSet;
	checkResult(vkAllocateDescriptorSets(device, &setAllocateInfo, &descriptorSet), "Descriptor set allocation");
	return descriptorSet;
}

VkDescriptorSet allocateBindlessDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, uint32_t descriptorCount) {
	uint32_t maxBinding = descriptorCount - 1;
	
	VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo = {};
	countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
	countInfo.pNext = 0;
	countInfo.descriptorSetCount = 1;
	countInfo.pDescriptorCounts = &maxBinding;

	VkDescriptorSetAllocateInfo setAllocateInfo = {};
	setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocateInfo.pNext = &countInfo;
	setAllocateInfo.descriptorPool = descriptorPool;
	setAllocateInfo.descriptorSetCount = 1;
	setAllocateInfo.pSetLayouts = &descriptorSetLayout;

	VkDescriptorSet descriptorSet;
	checkResult(vkAllocateDescriptorSets(device, &setAllocateInfo, &descriptorSet), "Descriptor set allocation");
	return descriptorSet;
}

} // namespace vk
} // namespace edl