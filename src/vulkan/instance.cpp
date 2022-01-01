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

	edl:log::trace("VulkanAPI", "validation layer: %s", pCallbackData->pMessage);

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
	VkPhysicalDeviceDynamicRenderingFeaturesKHR& dynamicRenderingFeaturesKHR = features.dynamicRenderingFeaturesKHR;

	// Initialize the features
	features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	features2.pNext = &vulkan11Features;

	vulkan11Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
	vulkan11Features.pNext = &dynamicRenderingFeaturesKHR;

	dynamicRenderingFeaturesKHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
	dynamicRenderingFeaturesKHR.pNext = 0;

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
	appInfo.apiVersion = VK_API_VERSION_1_1;

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr; // Optional

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&createInfo;
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

	//Enumerate the physical devices
	vkEnumeratePhysicalDevices(instance, &availableDeviceCount, NULL);
	assert(availableDeviceCount <= MAX_PHYSICAL_DEVICES);
	vkEnumeratePhysicalDevices(instance, &availableDeviceCount, availableDevices);

	// Physical Device selection
	selectPhysicalDevice(-1);
	getPhysicalDeviceFeatures(physicalDevice, features);
	features.features2.features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;

	//Handle Queues
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &availableFamiliesCount, nullptr);
	assert(availableFamiliesCount <= MAX_QUEUE_FAMLIES);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &availableFamiliesCount, availableFamilies);

	VkDeviceQueueCreateInfo deviceQueueCreateInfos[MAX_QUEUE_FAMLIES];

	float priorities[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

	for (uint32_t i = 0; i < availableFamiliesCount; i++) {
		VkDeviceQueueCreateInfo& info = deviceQueueCreateInfos[i];
		VkQueueFamilyProperties family = availableFamilies[i];

		info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.flags = 0;
		info.pNext = nullptr;
		info.queueFamilyIndex = i;
		info.queueCount = family.queueCount;
		info.pQueuePriorities = priorities;
	}


	//Logical Device Creation
	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.pNext = &features.features2;
	deviceCreateInfo.queueCreateInfoCount = availableFamiliesCount;
	deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos;
	deviceCreateInfo.pEnabledFeatures = NULL; //These are sent to pNext instead

	const char* const extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME };
	deviceCreateInfo.ppEnabledExtensionNames = extensions;
	deviceCreateInfo.enabledExtensionCount = 2;

	//TODO: Fix validation
	deviceCreateInfo.enabledLayerCount = requestedLayers.size();
	deviceCreateInfo.ppEnabledLayerNames = requestedLayers.data();// &"VK_LAYER_LUNARG_standard_validation";

	checkResult(vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device), "Device creation");

	//Get queues
	queuesCount = 0;
	for (uint32_t i = 0; i < availableFamiliesCount; i++) {
		VkQueueFamilyProperties family = availableFamilies[i];
		for (uint32_t j = 0; j < family.queueCount; j++) {
			vkGetDeviceQueue(device, i, j, &queues[queuesCount]);
			queuesCount++;
			assert(queuesCount <= MAX_QUEUES);
		}
	}

	//TODO: Log this properly
	edl::log::trace("VulkanAPI", "Queue's Gathered!");

	// Load functions
	vkCmdBeginRenderingKHR = reinterpret_cast<PFN_vkCmdBeginRenderingKHR>(vkGetDeviceProcAddr(device, "vkCmdBeginRenderingKHR"));
	vkCmdEndRenderingKHR = reinterpret_cast<PFN_vkCmdEndRenderingKHR>(vkGetDeviceProcAddr(device, "vkCmdEndRenderingKHR"));
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
		features.dynamicRenderingFeaturesKHR.dynamicRendering;

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