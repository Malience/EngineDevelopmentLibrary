#include "edl/ResourceSystem.h"

#include "edl/pipeline.h"
#include "edl/Util.h"
#include "edl/ShaderLoader.h"
#include "edl/bindless_types.h"
#include "edl/debug.h"
#include "edl/io.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace edl {

ResourceSystem::ResourceSystem() {
    allocator = new mem::DefaultAllocator();
}

ResourceSystem::~ResourceSystem() {

}

void ResourceSystem::init(vk::Instance& instance, global_info* globalInfo) {
    this->instance = instance;
    vulkan_physical_device = instance.physicalDevice;
    vulkan_device = instance.device;

    this->globalInfo = globalInfo;

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(vulkan_physical_device, &props);

    stagingBuffer.create(vulkan_physical_device, vulkan_device, 256 * 1028 * 1028, 0);
    imageTable.init(vulkan_physical_device, vulkan_device, 1920 * 1080 * 4 * 20);
    uniformBufferObject.create(vulkan_physical_device, vulkan_device, 20000, sizeof(float) * 16, props.limits.minUniformBufferOffsetAlignment);
    textureIndexObject.create(vulkan_physical_device, vulkan_device, 20000, sizeof(DrawData), props.limits.minUniformBufferOffsetAlignment);
    descriptorManager.init(vulkan_device);

    //TODO: Come on vulkan >.<
    VkBuffer wasteOfMemory = vk::createBuffer(vulkan_device, 1, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    uint32_t objectMax = 30000;
    uint32_t triangleMax = 64 * 1024 * 1024;
    uint32_t materialMax = 10000;

    bindlessImageDescriptor.create(instance.device, 0, materialMax);

    transformBuffer = createStorageBuffer(instance, sizeof(glm::mat4), objectMax);
    //materialBuffer = createStorageBuffer(instance, sizeof(glm::mat4x4), 10000);
    drawDataBuffer = createStorageBuffer(instance, sizeof(DrawData), objectMax * 5);

    positionBuffer = createStorageBuffer(instance, sizeof(glm::vec4), triangleMax, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    normalBuffer = createStorageBuffer(instance, sizeof(glm::vec4), triangleMax, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    texCoord0Buffer = createStorageBuffer(instance, sizeof(glm::vec2), triangleMax, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    texCoord1Buffer = createStorageBuffer(instance, sizeof(glm::vec2), triangleMax, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    indexBuffer = createStorageBuffer(instance, sizeof(uint16_t), triangleMax * 3, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    indirectBuffer = createStorageBuffer(instance, sizeof(VkDrawIndexedIndirectCommand), objectMax, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT);

    toolchain.add("system", this);
}

void ResourceSystem::loadScene(const std::string& filename) {
    std::string dir = "./res/scene/";
    std::vector<char> data;
    loadFile(dir + filename, data);

    rapidjson::Document d;
    d.Parse<rapidjson::kParseStopWhenDoneFlag>(data.data(), data.size());
    if (d.HasParseError()) {
        std::cout << "JSON parsing error, code: " << d.GetParseError() << ", offset: " << d.GetErrorOffset() << std::endl;
    }
    assert(d.IsObject());

    if (d.HasMember("Options")) {
        auto& options = d["Options"].GetObject();

        if (options.HasMember("Directories")) {
            for (auto* ptr = options["Directories"].GetArray().Begin(); ptr != options["Directories"].GetArray().End(); ++ptr) {
                const char* dir = ptr->GetString();
                directories.push_back(dir);
            }
        }
    }

    if (d.HasMember("Load")) {
        loadFiles(d["Load"].GetArray());
    }

    update(0);//TODO: Get rid of this

    if (d.HasMember("ShaderReflection")) {
        loadReflections(d["ShaderReflection"].GetArray());
    }

    if (d.HasMember("Pipelines")) {
        for (auto* ptr = d["Pipelines"].GetArray().Begin(); ptr != d["Pipelines"].GetArray().End(); ++ptr) {
            auto& pipeline = ptr->GetObject();

            assert(pipeline.HasMember("Name"));
            std::string name = pipeline["Name"].GetString();

            if (pipelines.find(name) != pipelines.end()) return; //Already loaded

            //Load shaders
            //TODO: Better Reflection
            //TODO: Caching and stuff

            //TODO: MOVE Shaders and meshes into own sections

            //Secondly, only handles (which should be uint64_t) should ever leave this object
            // Everything should link to this and use it in combination with the handles
            // ie. getMesh(string name) => mesh_handle
            // drawMesh(mesh_handle handle) => drawMesh()
            // Might need a hybrid system where you return objects that can be used externally 

            // Files loaded by outside file types would have the same name as their filename
            // ie. if Khadgar loaded Khadgar it's name and filename would both be "World/Generic/Human/Passive Doodads/Statues/StatueKhadgar.m2"

            //TODO: Async implementation
            std::vector<std::string> shaders; //TODO: This is kinda yikes
            assert(pipeline.HasMember("Shaders"));
            for (auto* ptr1 = pipeline["Shaders"].GetArray().Begin(); ptr1 != pipeline["Shaders"].GetArray().End(); ++ptr1) {
                shaders.push_back(ptr1->GetString());
            }


            //TODO: Handle multiple
            pipelines[name].init(pipeline, shaders, shader_handles, reflections, globalInfo);
            for (auto& ptr = pipelines[name].bindings.begin(); ptr != pipelines[name].bindings.end(); ++ptr) {
                layouts[ptr->first] = descriptorManager.createLayout(ptr->second);
            }
        }
    }

    if (d.HasMember("Models")) {
        auto& models = d["Models"].GetArray();
        for (auto* ptr = models.Begin(); ptr != models.End(); ++ptr) {
            loadModel(ptr->GetObject());
        }
    }

    if (d.HasMember("LoadLate")) {
        loadFiles(d["LoadLate"].GetArray());
    }

    if (d.HasMember("Scenes")) {
        for (auto* ptr1 = d["Scenes"].GetArray().Begin(); ptr1 != d["Scenes"].GetArray().End(); ++ptr1) {
            loadScene(ptr1->GetString());
        }
    }
    
}

void ResourceSystem::loadFiles(const rapidjson::GenericArray<false, rapidjson::Value>& files) {
    for (auto* ptr = files.Begin(); ptr != files.End(); ++ptr) {
        auto& obj = ptr->GetObject();

        assert(obj.HasMember("Name"));
        std::string name = obj["Name"].GetString();

        assert(obj.HasMember("Type"));
        std::string type = obj["Type"].GetString();

        std::string subtype = "";
        if (obj.HasMember("Subtype")) {
            subtype = obj["Subtype"].GetString();
        }

        assert(obj.HasMember("Filename"));
        std::string filename = obj["Filename"].GetString();

        res::Resource& res = createResource(name, filename, type, subtype);

        //TODO: Stop prioritizing shaders like this, pipelines need to be fixed first
        //if (type == "shader") {
        //    LoadFunction loadFunction = loadFunctionRegistry.at(res.type);
        //    loadFunction(*this, res);
        //}
        //else {
            requestResourceLoad(res.id);
        //}
    }
}

void ResourceSystem::loadReflections(const rapidjson::GenericArray<false, rapidjson::Value>& reflections) {
    for (auto* ptr = reflections.Begin(); ptr != reflections.End(); ++ptr) {
        loadReflection(ptr->GetObject());
    }
}

void ResourceSystem::loadReflection(const rapidjson::GenericObject<false, rapidjson::Value>& reflection) {
    assert(reflection.HasMember("Name"));
    std::string name = reflection["Name"].GetString();

    if (reflections.find(name) != reflections.end()) return; // If it is already loaded, don't

    VkShaderStageFlagBits stage;
    assert(reflection.HasMember("Type"));
    std::string type = reflection["Type"].GetString();
    if (type == "vert") stage = VK_SHADER_STAGE_VERTEX_BIT;
    else if (type == "frag") stage = VK_SHADER_STAGE_FRAGMENT_BIT;

    reflections[name].init(reflection, stage);
}

void ResourceSystem::loadModel(const rapidjson::GenericObject<false, rapidjson::Value>& model) {
    //assert(model.HasMember("Name"));
    //std::string name = model["Name"].GetString();

    //if (models.find(name) != models.end()) return; // If it is already loaded, don't

    assert(model.HasMember("Data"));
    //ModelData& data = modelDatas.at(model["Data"].GetString());
    models.push_back({});
    Model& m = models.back();

    //TODO: TEMPORARY
    m.pipeline = "M2Shader";

    m.transformHandle = getStorageBufferIndex(transformBuffer);// transformBuffer.createHandle();//uniformBufferObject.getHandle();

    //m.uniforms_descriptor_handle = descriptorManager.create(layouts[0]);

    //m.uniforms_descriptor_handle.bindBuffer(m.ubo_handle.buffer, m.ubo_handle.size, m.ubo_handle.offset, 0); //TODO: Fix hard-coded binding location
    std::string meshName = model["Data"].GetString();
    edl::res::ResourceID meshID = edl::hashString(meshName + "_mesh");

    m.mesh = meshID;//meshes[data.mesh];

    // Setup Position
    if (model.HasMember("Position")) {
        auto& arr = model["Position"].GetArray();
        m.position = glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
    }
    
    // Setup Rotation from gradian euler angles
    if (model.HasMember("Rotation")) {
        auto& arr = model["Rotation"].GetArray();
        m.rotation = glm::quat(glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat()));
    }

    // Setup Scale
    if (model.HasMember("Scale")) {
        auto& arr = model["Scale"].GetArray();
        m.scale = glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
    }

    m.calculateModel();
}

void ResourceSystem::updateModels(const glm::mat4& proj, const glm::mat4& view) {
    for (int i = 0; i < models.size(); ++i) {
        models[i].update(*this, proj, view);
    }
}

void ResourceSystem::draw(VkCommandBuffer& cb, uint32_t imageIndex) {
    std::string currentPipeline = "";
    
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)globalInfo->width;
    viewport.height = (float)globalInfo->height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    Pipeline& pipeline = pipelines[models[0].pipeline];
    VkRenderingInfoKHR& renderingInfo = pipeline.renderingInfos[imageIndex];

    PFN_vkCmdBeginRenderingKHR bRender = reinterpret_cast<PFN_vkCmdBeginRenderingKHR>(vkGetDeviceProcAddr(instance.device, "vkCmdBeginRenderingKHR"));
    bRender(cb, &renderingInfo);

    //edl::vk::vkCmdBeginRenderingKHR(cb, &pipeline.renderingInfos[imageIndex]);
    vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

    vkCmdSetViewport(cb, 0, 1, &viewport);

    uint32_t o[7] = { 0, 0, 0, 0, 0, 0, 0 };
    VkDescriptorSet sets[7] = { drawDataBuffer.descriptorSet, transformBuffer.descriptorSet, positionBuffer.descriptorSet, normalBuffer.descriptorSet, texCoord0Buffer.descriptorSet, texCoord1Buffer.descriptorSet, bindlessImageDescriptor.descriptorSet };
    vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipelineLayout, 0, 7, sets, 6, o);
    //
    ////TODO: Bind them all at the same time
    //vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[models[0].pipeline].pipelineLayout, 0, 1, &drawDataBuffer.descriptorSet, 1, &o);
    //vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[models[0].pipeline].pipelineLayout, 1, 1, &transformBuffer.descriptorSet, 1, &o);
    //vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[models[0].pipeline].pipelineLayout, 2, 1, &positionBuffer.descriptorSet, 1, &o);
    //vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[models[0].pipeline].pipelineLayout, 3, 1, &normalBuffer.descriptorSet, 1, &o);
    //vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[models[0].pipeline].pipelineLayout, 4, 1, &texCoord0Buffer.descriptorSet, 1, &o);
    //vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[models[0].pipeline].pipelineLayout, 5, 1, &texCoord1Buffer.descriptorSet, 1, &o);

    //vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[models[0].pipeline].pipelineLayout, 6, 1, &bindlessImageDescriptor.descriptorSet, 0, nullptr);

    vkCmdBindIndexBuffer(cb, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

    uint32_t drawID = 0;

    for (int i = 0; i < models.size(); ++i) {

        // Update the transform
        edl::updateStorageBuffer(stagingBuffer, transformBuffer, models[i].transformHandle, &models[i].mvp, 1);

        //TODO: EVERYTHING NEEDS TO BE FIXED
        //models[i].draw(cb, pipelines[models[i].pipeline].pipelineLayout);
        //drawMesh(cb, pipelines[models[i].pipeline].pipelineLayout, models[i].mesh);

        res::Resource& res = resMap.at(models[i].mesh);
        res::Mesh& mesh = res::getResourceData<res::Mesh>(res);

        DrawData drawData = {};
        drawData.positionOffset = mesh.positionOffset;
        drawData.normalOffset = mesh.normalOffset;
        drawData.texCoord0Offset = mesh.texCoord0Offset;
        drawData.texCoord1Offset = mesh.texCoord1Offset;
        drawData.transformOffset = models[i].transformHandle;

        for (int j = 0; j < mesh.batchCount; j++) {
            res::Batch& batch = mesh.batches[j];

            res::Resource& ires = resMap.at(batch.material);
            res::Image& material = res::getResourceData<res::Image>(ires);

            drawData.materialOffset = material.materialIndex;
            edl::updateStorageBuffer(stagingBuffer, drawDataBuffer, drawID, &drawData, 1);

            VkDrawIndexedIndirectCommand indirect = {};
            indirect.instanceCount = 1;
            indirect.vertexOffset = 0;
            indirect.firstIndex = mesh.indexOffset + batch.indexStart;
            indirect.indexCount = batch.indexCount;
            indirect.firstInstance = 0;
            edl::updateStorageBuffer(stagingBuffer, indirectBuffer, drawID, &indirect, 1);

            drawID++;
        }
    }

    vkCmdDrawIndexedIndirect(cb, indirectBuffer.buffer, 0, drawID, sizeof(VkDrawIndexedIndirectCommand));

    //vkCmdEndRenderingKHR(cb);
    edl::vk::vkCmdEndRenderingKHR(cb);
}

void ResourceSystem::registerLoadFunction(const std::string& type, LoadFunction function) {
    res::ResourceType restype = hashString(type);

    if (loadFunctionRegistry.find(restype) != loadFunctionRegistry.end()) {
        std::cout << "Loader already registered for type: " << type << std::endl;
    }

    loadFunctionRegistry.insert({ restype, function });
}

void ResourceSystem::update(double delta) {
    //fileLoader.loadChunks(100);
    while (queuebot != queuetop) {
        res::ResourceID id = loadQueue[queuebot];

        res::Resource& res = resMap.at(id);
        //TODO: Check requirements loaded
        if (res.status == res::ResourceStatus::UNLOADED) {
            LoadFunction loadFunction = loadFunctionRegistry.at(res.type);

            loadFunction(toolchain, res);
        }

        loadQueue[queuebot] = 0;
        queuebot++;
        if (queuebot == MAX_QUEUE) queuebot = 0;
    }


    //fileLoader.cleanup();
}

//Maybe multiple create functions, one for named resources, unnamed resources, and filenamed resources
res::Resource& ResourceSystem::createResource(std::string name, std::string filename, std::string type, std::string subtype) {
    uint64_t nameHash = hashString(name);
    uint64_t filenameHash = hashString(filename);

    res::ResourceID id = nameHash;//filenameHash == 0 ? nameHash : filenameHash;
    if (resMap.find(id) != resMap.end()) { //TODO: IDing needs to be fixed, need support for multiple resources loaded from the same file
        //printf("Resource already has name: %s\n", name.c_str());
        return resMap.at(id);
    }

    res::Resource& res = resMap[id];
    res.id = id;

    res.status = res::ResourceStatus::UNLOADED;

    res.nameHash = nameHash;
    res.name = (char*)allocator->malloc(name.size());
    strcpy(res.name, name.c_str());

    if (filename != "") {
        const char* dir = findDirectory(filename.c_str());
        size_t dirSize = strlen(dir);
        size_t filenameSize = filename.size();

        res.filenameHash = hashString(filename);
        res.path = (char*)allocator->malloc(dirSize + filenameSize);
        strcpy(res.path, dir);
        strcpy(res.path + dirSize, filename.c_str());
    }
    else {
        res.filenameHash = 0;
        res.path = 0;
    }

    res.type = hashString(type);
    res.subtype = hashString(subtype);

    res.dependenciesCount = 0;
    res.dependencies = 0;

    res.size = 0;
    res.data = 0;

    return res;
}

res::Resource& ResourceSystem::getResource(res::ResourceID id) {
    return resMap.at(id);
}

const char* ResourceSystem::findDirectory(const char* filename) {

    uint32_t size = directories.size();
    for (uint32_t i = 0; i < size; i++) {
        if (std::filesystem::exists(directories[i] + filename)) {
            return directories[i].c_str();
        }
    }

    assert(false, "Directory not found!");
}

void ResourceSystem::requestResourceLoad(res::ResourceID id) {
    assert(queuetop != queuebot - 1 || (queuetop == MAX_QUEUE - 1 && queuebot == 0), "Queue overflow");
    loadQueue[queuetop] = id;
    queuetop++;
    if (queuetop == MAX_QUEUE) queuetop = 0;
}

}