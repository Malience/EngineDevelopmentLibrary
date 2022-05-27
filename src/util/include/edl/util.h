#pragma once

#include <string>
#include <unordered_map>

namespace edl {

inline size_t align(size_t offset, size_t alignment) {
	return offset + (alignment - offset % alignment) % alignment;
}

inline size_t hash(const std::string& str) {
    std::hash<std::string> hasher;
    return hasher(str);
}

template<typename T>
inline T* getPointerOffset(void* data, uint32_t offset) {
	return reinterpret_cast<T*>(static_cast<char*>(data) + offset);
}

template<typename T>
inline const T* getPointerOffset(const void* data, uint32_t offset) {
	return reinterpret_cast<const T*>(static_cast<const char*>(data) + offset);
}

template<typename K, typename T>
inline bool maphas(const std::unordered_map<K, T>& map, const K& key) {
	return map.find(key) != map.end();
}

}