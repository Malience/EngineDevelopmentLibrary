#pragma once

#include <string>
#include <vector>

namespace edl {

bool isFile(const std::string& path);
bool isDirectory(const std::string& path);
size_t filesize(const std::string& path);
size_t loadFile(const std::string& path, size_t size, void* data);
size_t loadFile(const std::string& path, std::vector<char>& data);
size_t loadFile(const std::string& path, std::string& data);

}