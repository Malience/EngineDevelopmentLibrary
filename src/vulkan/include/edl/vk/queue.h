/*
This file contains functionality to debug vulkan systems
*/
#ifndef VULKAN_QUEUE_OBJECT_H
#define VULKAN_QUEUE_OBJECT_H

#include <cstdint>
#include <vector>

struct VkCommandPool_T;
struct VkDevice_T;
struct VkSemaphore_T;
struct VkSwapchainKHR_T;
struct VkQueue_T;

typedef uint32_t VkFlags;

typedef VkCommandPool_T* VkCommandPool;
typedef VkFlags VkCommandPoolCreateFlags;
typedef VkDevice_T* VkDevice;
typedef VkSemaphore_T* VkSemaphore;
typedef VkSwapchainKHR_T* VkSwapchainKHR;
typedef VkQueue_T* VkQueue;

namespace edl {
//
//namespace vk {
//class Swapchain;
//}
//
//class VulkanQueue {
//public:
//    VulkanQueue();
//    virtual ~VulkanQueue();
//
//    void present(const std::vector<VkSemaphore> waitSemaphores, const vk::Swapchain& swapchain, const uint32_t* pImageIndices);
//
//    const VkQueue& getQueue() { return vulkan_queue; }
//
//    void setQueue(VkQueue queue) { vulkan_queue = queue; }
//
//private:
//    VkQueue vulkan_queue;
//};

} // namespace edl
#endif // !VULKAN_QUEUE_OBJECT_H