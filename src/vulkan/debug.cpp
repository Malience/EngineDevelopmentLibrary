#include "edl/vk/debug.h"

namespace edl {
namespace vk {

//TODO: LOL
//Look, I needed to call this code
static bool logger_loaded = []() -> bool { edl::log::setLevel(edl::log::Level::trace); edl::log::logger("VulkanAPI"); return true; }();

//Prints the result of a vulkan API call
void checkResult(VkResult result, const char* action) {
#ifdef _DEBUG

	if (result == VK_SUCCESS) {
		edl::log::debug("VulkanAPI", "%s: SUCCESS!", action);
		return;
	}

	switch (result) {
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Out of host Memory!");
		break;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Out of device Memory!");
		break;
	case VK_ERROR_INITIALIZATION_FAILED:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Initialization failed!");
		break;
	case VK_ERROR_LAYER_NOT_PRESENT:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Layer not present!");
		break;
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Extension not present!");
		break;
	case VK_ERROR_FEATURE_NOT_PRESENT:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Feature not present!");
		break;
	case VK_ERROR_TOO_MANY_OBJECTS:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Too many objects!");
		break;
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Incompatible driver!");
		break;
	case VK_ERROR_DEVICE_LOST:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Device lost!");
		break;
	case VK_ERROR_INVALID_SHADER_NV:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Invalid shader!");
		break;
	default:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action, "Unknown error!");
		break;
	}

#endif // _DEBUG
}


} // namespace vk
} // namespace edl