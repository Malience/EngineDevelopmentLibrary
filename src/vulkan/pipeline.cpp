#include "edl/vk/pipeline.h"

#include "edl/vk/debug.h"

#include <assert.h>

namespace edl {
namespace vk {

void PipelineBuilder::setDevice(VkDevice device) {
	this->device = device;
}

void PipelineBuilder::setShaders(uint32_t count, const VkShaderModule* modules, const VkShaderStageFlagBits* stages) {
	assert(count < MAX_PIPELINE_SHADERS);

	shadersCount = count;

	for (uint32_t i = 0; i < count; i++) {
		VkPipelineShaderStageCreateInfo& info = shaderInfos[i];

		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = NULL;
		info.stage = stages[i];
		info.module = modules[i];
		info.pName = "main";
		info.pSpecializationInfo = NULL;
	}
}

void PipelineBuilder::setDimensions(uint32_t width, uint32_t height) {
	this->width = width;
	this->height = height;
}

void PipelineBuilder::setBindingsCount(uint32_t count) {
	bindingsCount = count;
}

void PipelineBuilder::setBinding(uint32_t index, uint32_t binding, uint32_t stride) {
	assert(index < bindingsCount);

	VkVertexInputBindingDescription& desc = bindings[index];
	desc.binding = binding;
	desc.stride = stride;
	desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

}

void PipelineBuilder::setAttributesCount(uint32_t count) {
	attributesCount = count;
}

void PipelineBuilder::setAttribute(uint32_t index, uint32_t location, uint32_t binding, VkFormat format, uint32_t offset) {
	assert(index < attributesCount);

	VkVertexInputAttributeDescription& desc = attributes[index];
	desc.binding = binding;
	desc.format = format;
	desc.location = location;
	desc.offset = offset;
}

void PipelineBuilder::setAttachmentCount(uint32_t count) {
	attachmentsCount = count;
}

void PipelineBuilder::setAttachment(uint32_t index, VkFormat format) {
	assert(index < attachmentsCount);
	attachments[index] = format;
}

void PipelineBuilder::setDepthStencilAttachment(VkFormat format) {
	depthStencilAttachment = format;
}


VkPipeline PipelineBuilder::create(VkPipelineLayout layout) {
	//TODO: support
	VkPipelineRenderingCreateInfoKHR infoKHR = {};
	infoKHR.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
	infoKHR.pNext = nullptr;
	infoKHR.colorAttachmentCount = attachmentsCount;
	infoKHR.pColorAttachmentFormats = attachments;
	infoKHR.depthAttachmentFormat = depthStencilAttachment;
	infoKHR.stencilAttachmentFormat = depthStencilAttachment;
	infoKHR.viewMask = 0;

	//TODO: support
	VkPipelineVertexInputStateCreateInfo inputInfo = {};
	inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	inputInfo.pNext = NULL;
	inputInfo.flags = 0;
	inputInfo.vertexBindingDescriptionCount = bindingsCount;
	inputInfo.pVertexBindingDescriptions = bindings;
	inputInfo.vertexAttributeDescriptionCount = attributesCount;
	inputInfo.pVertexAttributeDescriptions = attributes;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)width;
	viewport.height = (float)height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = width;
	scissor.extent.height = height;

	VkPipelineViewportStateCreateInfo viewportInfo;
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.pNext = NULL;
	viewportInfo.flags = 0;
	viewportInfo.viewportCount = 1;
	viewportInfo.pViewports = &viewport;
	viewportInfo.scissorCount = 1;
	viewportInfo.pScissors = &scissor;

	//TODO: support
	VkPipelineRasterizationStateCreateInfo rasterInfo;
	rasterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterInfo.pNext = NULL;
	rasterInfo.flags = 0;
	rasterInfo.depthClampEnable = VK_FALSE; //Clamps fragments outside of near/far instead of discarding
	rasterInfo.rasterizerDiscardEnable = VK_FALSE; //From lunarg, "controls whether primitives are discarded immediately before the rasterization stage"
	rasterInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterInfo.lineWidth = 1.0f;
	rasterInfo.cullMode = VK_CULL_MODE_BACK_BIT;//VK_CULL_MODE_BACK_BIT; //TODO: This is why it's so slow >.<
	rasterInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	//rasterInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterInfo.depthBiasEnable = VK_FALSE;
	rasterInfo.depthBiasConstantFactor = 0.0f;
	rasterInfo.depthBiasClamp = 0.0f;
	rasterInfo.depthBiasSlopeFactor = 0.0f;

	//TODO: support
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleInfo.pNext = NULL;
	multisampleInfo.flags = 0;
	multisampleInfo.sampleShadingEnable = VK_FALSE;
	multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleInfo.minSampleShading = 1.0f;
	multisampleInfo.pSampleMask = NULL;
	multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleInfo.alphaToOneEnable = VK_FALSE;

	//TODO: support
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
	colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachmentState.blendEnable = VK_FALSE;
	colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

	//TODO: support
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.pNext = NULL;
	colorBlendInfo.flags = 0;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &colorBlendAttachmentState;
	colorBlendInfo.blendConstants[0] = 0.0f;
	colorBlendInfo.blendConstants[1] = 0.0f;
	colorBlendInfo.blendConstants[2] = 0.0f;
	colorBlendInfo.blendConstants[3] = 0.0f;

	//TODO: support
	VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
	depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateInfo.pNext = NULL;
	depthStencilStateInfo.flags = 0;
	depthStencilStateInfo.depthTestEnable = VK_TRUE;
	depthStencilStateInfo.depthWriteEnable = VK_TRUE;
	depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilStateInfo.minDepthBounds = 0.0f;
	depthStencilStateInfo.maxDepthBounds = 1.0f;
	depthStencilStateInfo.stencilTestEnable = VK_FALSE;

	//TODO: support
	VkPipelineInputAssemblyStateCreateInfo assemblyStateCreateInfo = {};
	assemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assemblyStateCreateInfo.pNext = nullptr;
	assemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
	assemblyStateCreateInfo.flags = NULL;
	assemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	//TODO: support
	//Dynamic state nonsense
	VkDynamicState dynamicStates[] = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_LINE_WIDTH
	};

	//TODO: support
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	//TODO: support
	VkGraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = NULL;
	pipelineInfo.flags = 0;
	pipelineInfo.stageCount = shadersCount;
	pipelineInfo.pStages = shaderInfos;

	pipelineInfo.pVertexInputState = &inputInfo;
	pipelineInfo.pInputAssemblyState = &assemblyStateCreateInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &rasterInfo;
	pipelineInfo.pMultisampleState = &multisampleInfo;
	pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
	pipelineInfo.pColorBlendState = &colorBlendInfo;
	pipelineInfo.pDynamicState = &dynamicState;

	pipelineInfo.layout = layout;
	pipelineInfo.renderPass = VK_NULL_HANDLE;
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = NULL;
	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.pTessellationState = NULL;

	VkPipeline pipeline = 0;
	checkResult(vkCreateGraphicsPipelines(device, NULL, 1, &pipelineInfo, NULL, &pipeline), "Graphics pipeline creation");
	return pipeline;
}

//TODO: add descriptors and push constants
VkPipelineLayout createPipelineLayout(VkDevice device, uint32_t setLayoutCount, const VkDescriptorSetLayout* pSetLayouts) {
	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = NULL;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = setLayoutCount;
	pipelineLayoutInfo.pSetLayouts = pSetLayouts;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = NULL;

	VkPipelineLayout pipelineLayout = 0;
	checkResult(vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout), "Pipeline layout creation");
	return pipelineLayout;
}

} // namespace vk
} // namespace edl