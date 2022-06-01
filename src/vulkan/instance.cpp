#include "edl/vk/instance.h"

#include "edl/vk/debug.h"
#include "edl/vk/function.h"

#include <assert.h>

namespace edl {
namespace vk {

//TODO: Rewrite messenger code

VkDebugUtilsMessengerEXT debugMessenger;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
	log::info("VulkanAPI", "validation layer: %s", pCallbackData->pMessage);
	}
	else if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
	log::debug("VulkanAPI", "validation layer: %s", pCallbackData->pMessage);
	}
	else if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
	log::error("VulkanAPI", "validation layer: %s", pCallbackData->pMessage);
	}
	else {
	log::trace("VulkanAPI", "validation layer: %s", pCallbackData->pMessage);
	}
	
	return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

VkResult DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pDebugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, pDebugMessenger, pAllocator);
		return VK_SUCCESS;
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

VkBool32 VkReportCallback(
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char* pLayerPrefix,
	const char* pMessage,
	void* pUserData) {
	log::debug("VulkanAPI", pMessage);
	return VK_TRUE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}


uint32_t Instance::getMemoryType(int filter, VkMemoryPropertyFlags properties) const {
	//TODO: Branching
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if (filter & (1 << i) && memoryProperties.memoryTypes[i].propertyFlags & properties) return i;
	}

	log::error("VulkanAPI", "Failed to find memory type!");
	return 0;
}

void getPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, PhysicalDeviceFeatures& features) {
	VkPhysicalDeviceFeatures2& features2 = features.features2;
	VkPhysicalDeviceVulkan11Features& vulkan11Features = features.vulkan11Features;
	VkPhysicalDeviceVulkan12Features& vulkan12Features = features.vulkan12Features;
	VkPhysicalDeviceVulkan13Features& vulkan13Features = features.vulkan13Features;
	VkPhysicalDeviceMeshShaderFeaturesNV& meshShaderFeaturesNV = features.meshShaderFeaturesNV;
	VkPhysicalDeviceMeshShaderPropertiesNV& meshShaderPropertiesNV = features.meshShaderPropertiesNV;

	// Initialize the features
	features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	features2.pNext = &vulkan11Features;

	vulkan11Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
	vulkan11Features.pNext = &vulkan12Features;

	vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	vulkan12Features.pNext = &vulkan13Features;

	vulkan13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
	vulkan13Features.pNext = &meshShaderFeaturesNV;

	meshShaderFeaturesNV.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV;
	meshShaderFeaturesNV.pNext = 0;

	// Get features from device
	vkGetPhysicalDeviceFeatures2(physicalDevice, &features2);
}

void Instance::create(const std::string& applicationName, const std::string& engineName, const std::vector<const char*>& requestedLayers, const std::vector<const char*>& requestedExtension) {
	// Vulkan Instancce creation
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = NULL;
	appInfo.pApplicationName = applicationName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = engineName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(3, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkDebugReportCallbackCreateInfoEXT reportCallbackInfo = {};
	reportCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	reportCallbackInfo.pNext = nullptr;
	reportCallbackInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	reportCallbackInfo.pfnCallback = VkReportCallback;
	reportCallbackInfo.pUserData = nullptr;

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.pNext = nullptr;// &reportCallbackInfo;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr; // Optional

	std::vector<VkValidationFeatureEnableEXT> enabledFeatures = { VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT };
	std::vector<VkValidationFeatureDisableEXT> disabledFeatures = {};// { VK_VALIDATION_FEATURE_DISABLE_GPU_ASSISTED_EXT };

	VkValidationFeaturesEXT valfeatures = {};
	valfeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	valfeatures.pNext = nullptr;// &createInfo;
	valfeatures.enabledValidationFeatureCount = enabledFeatures.size();
	valfeatures.pEnabledValidationFeatures = enabledFeatures.data();
	valfeatures.disabledValidationFeatureCount = disabledFeatures.size();
	valfeatures.pDisabledValidationFeatures = disabledFeatures.data();

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pNext = &createInfo;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledLayerCount = requestedLayers.size();
	instanceCreateInfo.ppEnabledLayerNames = requestedLayers.data();
	instanceCreateInfo.enabledExtensionCount = requestedExtension.size();
	instanceCreateInfo.ppEnabledExtensionNames = requestedExtension.data();

	//TODO: Check layers and extensions for support

	uint32_t layer_properties_count;
	vkEnumerateInstanceLayerProperties(&layer_properties_count, NULL);
	std::vector<VkLayerProperties> layer_properties;
	layer_properties.resize(layer_properties_count);
	vkEnumerateInstanceLayerProperties(&layer_properties_count, layer_properties.data());

	checkResult(vkCreateInstance(&instanceCreateInfo, NULL, &instance), "Vulkan instance creation");

	VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
	checkResult(result, "Vulkan debug messenger creation");

	// Enumerate the physical devices
	vkEnumeratePhysicalDevices(instance, &availableDeviceCount, NULL);
	assert(availableDeviceCount <= MAX_PHYSICAL_DEVICES);
	vkEnumeratePhysicalDevices(instance, &availableDeviceCount, availableDevices);

	// Physical Device selection
	selectPhysicalDevice(-1);
	getPhysicalDeviceFeatures(physicalDevice, features);
	features.features2.features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
	features.vulkan12Features.bufferDeviceAddress = VK_TRUE;
	features.vulkan12Features.descriptorBindingPartiallyBound = VK_TRUE;
	features.vulkan12Features.runtimeDescriptorArray = VK_TRUE;

	VkPhysicalDeviceMeshShaderPropertiesNV meshShaderProperties = {};
	meshShaderProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV;

	VkPhysicalDeviceProperties2 properties = {};
	properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	properties.pNext = &meshShaderProperties;

	vkGetPhysicalDeviceProperties2(physicalDevice, &properties);

	MESH_SHADING = features.meshShaderFeaturesNV.meshShader;
	//TEMPORARY
	//MESH_SHADING = false;

	// Handle Queues
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	//assert(availableFamiliesCount <= MAX_QUEUE_FAMLIES);
	queueFamilyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

	//TODO: Look into this
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	//VkDeviceQueueCreateInfo deviceQueueCreateInfos[MAX_QUEUE_FAMLIES];
	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
	std::vector<float> priorities = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	deviceQueueCreateInfos.resize(queueFamilyCount);

	for (uint32_t i = 0; i < queueFamilyCount; i++) {
		VkDeviceQueueCreateInfo& info = deviceQueueCreateInfos[i];
		VkQueueFamilyProperties& family = queueFamilyProperties[i];

		info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.flags = 0;
		info.pNext = nullptr;
		info.queueFamilyIndex = i;
		info.queueCount = family.queueCount;
		info.pQueuePriorities = priorities.data();
	}

	// Logical Device Creation
	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.pNext = &features.features2;
	deviceCreateInfo.queueCreateInfoCount = queueFamilyCount;
	deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = nullptr; //These are sent to pNext instead

	std::vector<const char*> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_DEPTH_RANGE_UNRESTRICTED_EXTENSION_NAME };
	if (MESH_SHADING) {
		extensions.push_back(VK_NV_MESH_SHADER_EXTENSION_NAME);
	}
	deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
	deviceCreateInfo.enabledExtensionCount = extensions.size();

	//TODO: Fix validation
	deviceCreateInfo.enabledLayerCount = requestedLayers.size();
	deviceCreateInfo.ppEnabledLayerNames = requestedLayers.data();// &"VK_LAYER_LUNARG_standard_validation";

	checkResult(vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device), "Device creation");

	// Get queues
	for (uint32_t i = 0; i < queueFamilyCount; i++) {
		VkQueueFamilyProperties& family = queueFamilyProperties[i];
		for (uint32_t j = 0; j < family.queueCount; j++) {
			VkQueue queue = {};
			vkGetDeviceQueue(device, i, j, &queue);
			queues[queuesCount] = queue;
			queuesCount++;
			assert(queuesCount <= MAX_QUEUES);
		}
	}

	//TODO: Log this properly
	edl::log::trace("VulkanAPI", "Queue's Gathered!");
}

uint32_t scorePhysicalDevice(VkPhysicalDevice physicalDevice) {
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	PhysicalDeviceFeatures features;
	getPhysicalDeviceFeatures(physicalDevice, features);

	uint32_t score = 0;

	// Required features
	score =
		properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		features.features2.features.geometryShader &&
		features.features2.features.samplerAnisotropy &&
		features.vulkan11Features.shaderDrawParameters &&
		features.vulkan12Features.descriptorBindingPartiallyBound &&
		features.vulkan12Features.runtimeDescriptorArray &&
		features.vulkan13Features.dynamicRendering;

	return score;
}

//TODO: Score physical devices
void Instance::selectPhysicalDevice(int deviceIndex) {
	if (physicalDevice == availableDevices[deviceIndex]) return;

	if (device != 0) {
		//TODO: Cleanup, probably shouldn't care about this for a while
	}

	physicalDevice = 0;

	if (deviceIndex >= 0) {
		if (scorePhysicalDevice(availableDevices[deviceIndex]) > 0) {
			physicalDevice = availableDevices[deviceIndex];
		}
		else {
			deviceIndex = -1;
			edl::log::error("VulkanAPI", "Selected device is unsuitable! Finding suitable device...");
		}
	}

	if (deviceIndex < 0) {
		uint32_t top = 0;
		for (uint32_t i = 0; i < availableDeviceCount; i++) {
			VkPhysicalDevice currentDevice = availableDevices[i];
			uint32_t score = scorePhysicalDevice(availableDevices[i]);
			if (score > top) {
				top = score;
				physicalDevice = availableDevices[i];
			}
		}

		if (physicalDevice == 0) {
			edl::log::critical("VulkanAPI", "Failed to find suitable device!");
			return;
		}
	}

	//TODO: Log Physical device info
	edl::log::info("VulkanAPI", "Suitable Physical Device Found!");
}


} // namespace vk
} // namespace edl