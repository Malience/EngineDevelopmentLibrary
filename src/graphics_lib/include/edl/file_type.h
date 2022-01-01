#ifndef FILE_TYPE_H
#define FILE_TYPE_H

#include "io.h"

#include <filesystem>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>

// Should contain everything needed to load a file into memory
struct file_header {
	std::string path;
	std::string filename;
	std::string extension;
};

IOResult initFileHeader(std::string& path, file_header& header) {
	// Check if the file exists
	struct stat buffer;
	if (stat(path.c_str(), &buffer) == 0) return IOResult::IO_FAILED;

	std::filesystem::path p(path);
	header.path = std::filesystem::absolute(path).string();
	header.filename = p.filename().string();
	header.extension = p.extension().string();
	
	return IOResult::IO_SUCCESS;
}


#endif // !FILE_TYPE_H