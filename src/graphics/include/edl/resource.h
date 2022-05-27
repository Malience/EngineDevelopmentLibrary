#pragma once

#include "edl/allocator.h"

#include <string>
#include <unordered_map>

namespace edl {

typedef uint64_t ResourceID;

enum class ResourceStatus : uint8_t {
    UNLOADED,
    LOADED,
    FINISHED,

    MAX_RESOURCE_STATUS
};

struct Resource {
    ResourceID id;
    ResourceStatus status;

    std::string type;
    std::string subtype;

    std::string name;
    std::string path;

    size_t size;
    void* data;
};

inline void allocateResourceData(Resource& res, size_t size, Allocator& allocator) {
    res.size = size;
    res.data = allocator.malloc(size);
}

template<typename T>
inline T& getResourceData(Resource& res) {
    return *static_cast<T*>(res.data);
}

template<typename T>
class Keychain {
public:
    void add(const std::string& id, T key) {
        chain.insert({ id, key });
    }

    T& get(const std::string& id) {
        return chain.at(id);
    }

    bool has(const std::string& id) {
        return chain.find(id) != chain.end();
    }

protected:
    std::unordered_map<std::string, T> chain;
};

class Toolchain : public Keychain<void*> {
public:
    template<typename T>
    T& getTool(const std::string& id) {
        return *static_cast<T*>(chain.at(id));
    }
};

typedef void (*LoadFunction)(edl::Toolchain& toolchain, edl::Resource& res);

}