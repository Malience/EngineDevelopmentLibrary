#ifndef IO_LIB_H
#define IO_LIB_H

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>

enum class IOResult : uint32_t {
	IO_SUCCESS = 0,
	IO_FAILED = 1,
};

struct mesh_info {
	uint32_t indicesCount;
	uint32_t attributeCount;
	uint64_t *pOffsets;
	uint64_t size;
	void *data;
};

struct image_info {
	int width;
	int height;
	int channels;
	unsigned char* data;

	void free();
};

IOResult loadFile(const std::string& filename, std::vector<char>& data);
IOResult loadImage(const std::string& filename, image_info& image);
void freeImage(unsigned char* data);

IOResult loadModel(const std::string& filename, uint32_t* numMeshes, mesh_info** vertexBufferCreateInfo);

#endif // !IO_LIB_H