#include "edl/vk/debug.h"

namespace edl {
namespace vk {

//TODO: LOL
//Look, I needed to call this code
static bool logger_loaded = []() -> bool { edl::log::setLevel(edl::log::Level::trace); edl::log::logger("VulkanAPI"); return true; }();

//Prints the result of a vulkan API call
void checkResult(VkResult result, const std::string& action) {
#ifdef _DEBUG

	if (result == VK_SUCCESS) {
		edl::log::debug("VulkanAPI", "%s: SUCCESS!", action.c_str());
		return;
	}

	switch (result) {
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Out of host Memory!");
		break;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Out of device Memory!");
		break;
	case VK_ERROR_INITIALIZATION_FAILED:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Initialization failed!");
		break;
	case VK_ERROR_LAYER_NOT_PRESENT:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Layer not present!");
		break;
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Extension not present!");
		break;
	case VK_ERROR_FEATURE_NOT_PRESENT:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Feature not present!");
		break;
	case VK_ERROR_TOO_MANY_OBJECTS:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Too many objects!");
		break;
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Incompatible driver!");
		break;
	case VK_ERROR_DEVICE_LOST:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Device lost!");
		break;
	case VK_ERROR_INVALID_SHADER_NV:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Invalid shader!");
		break;
	default:
		edl::log::error("VulkanAPI", "%s: FAILED - %s", action.c_str(), "Unknown error!");
		break;
	}

#endif // _DEBUG
}


} // namespace vk
} // namespace edl