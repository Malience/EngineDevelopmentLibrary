#pragma once

#include "edl/image_table.h"
#include "edl/StagingBuffer.h"
#include "edl/descriptor_manager.h"
#include "edl/StorageBuffer.h"
#include "edl/vk/vulkan.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ResourceHandles.h"
#include "Mesh.h"

#include <vector>
#include <string>

namespace edl {

class ResourceSystem;
class Pipeline;

class Model {
public:
    Model();
    void draw(VkCommandBuffer cb, VkPipelineLayout layout);
    void calculateModel();
    void update(ResourceSystem& system, const glm::mat4& proj, const glm::mat4& view);

    std::string name;

    std::string pipeline;

    std::vector<ImageHandle> textures;

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    glm::mat4 model;
    glm::mat4 mvp;

    uint32_t transformHandle;
    res::ResourceID mesh;
    //DescriptorHandle uniforms_descriptor_handle;
};

}