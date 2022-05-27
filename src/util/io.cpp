#include "edl/io.h"

#include <filesystem>

namespace edl {

bool isFile(const std::string& path) {
    std::filesystem::directory_entry entry(path);
    return entry.exists() && !entry.is_directory();
}

bool isDirectory(const std::string& path) {
    std::filesystem::directory_entry entry(path);
    return entry.exists() && entry.is_directory();
}

size_t filesize(const std::string& path) {
    std::filesystem::directory_entry entry(path);
    return entry.file_size();
}

size_t loadFile(const std::string& path, size_t size, void* data) {
    FILE* f = fopen(path.c_str(), "rb");

    size_t bytes = 0;
    size_t bytesRead = 0;
    do {
        bytes = fread((char*)data + bytesRead, 1, 2048, f);
        bytesRead += bytes;
    } while (bytes > 0);

    if (ferror(f)) {
        printf("Error!\n");
    }

    fclose(f);

    return bytesRead;
}

size_t loadFile(const std::string& path, std::vector<char>& data) {
    FILE* f = fopen(path.c_str(), "rb");

    size_t bytes = 0;
    size_t bytesRead = 0;
    do {
        bytes = fread((char*)data.data() + bytesRead, 1, 2048, f);
        bytesRead += bytes;
    } while (bytes > 0);

    if (ferror(f)) {
        printf("Error!\n");
    }

    fclose(f);

    return bytesRead;
}

size_t loadFile(const std::string& path, std::string& data) {
    FILE* f = fopen(path.c_str(), "rb");

    size_t bytes = 0;
    size_t bytesRead = 0;
    do {
        bytes = fread((char*)data.data() + bytesRead, 1, 2048, f);
        bytesRead += bytes;
    } while (bytes > 0);

    if (ferror(f)) {
        printf("Error!\n");
    }

    fclose(f);

    return bytesRead;
}

}