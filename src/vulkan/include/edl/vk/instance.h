/*
This file contains the Instance class
*/
#pragma once

#include "vulkan/vulkan.h"

#include <cstdint>
#include <string>
#include <vector>

namespace edl {
namespace vk {

const uint32_t MAX_PHYSICAL_DEVICES = 5;
const uint32_t MAX_QUEUE_FAMLIES = 5;
const uint32_t MAX_QUEUES = 32;

struct PhysicalDeviceFeatures {
    VkPhysicalDeviceFeatures2 features2;
    VkPhysicalDeviceVulkan11Features vulkan11Features;
    VkPhysicalDeviceVulkan12Features vulkan12Features;
    VkPhysicalDeviceVulkan13Features vulkan13Features;
    VkPhysicalDeviceMeshShaderFeaturesNV meshShaderFeaturesNV;
    VkPhysicalDeviceMeshShaderPropertiesNV meshShaderPropertiesNV;
};

//TODO: Destruction
struct Instance {
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    PhysicalDeviceFeatures features;

    uint32_t availableDeviceCount;
    VkPhysicalDevice availableDevices[MAX_PHYSICAL_DEVICES];

    //uint32_t availableFamiliesCount;
    //VkQueueFamilyProperties availableFamilies[MAX_QUEUE_FAMLIES];
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;

    uint32_t queuesCount;
    VkQueue queues[MAX_QUEUES];

    bool MESH_SHADING;

    void create(const std::string& applicationName, const std::string& engineName, const std::vector<const char*>& requestedLayers, const std::vector<const char*>& requestedExtension);
    void selectPhysicalDevice(int deviceIndex);
    uint32_t getMemoryType(int filter, VkMemoryPropertyFlags properties) const;
};


} // namespace vk
} // namespace edl