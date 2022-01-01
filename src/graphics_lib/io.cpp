#include "edl/io.h"

#include "stb/stb_image.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

//#include "cglm/cglm.h"

//Basic file loader
IOResult loadFile(const std::string& filename, std::vector<char>& data) {
	FILE* file = fopen(filename.c_str(), "rb"); //Open the file
	if (!file) {
		fprintf(stderr, "File failed to open: %d\n", errno);
		//printf("Error: ", strerror(errno));
		return IOResult::IO_FAILED;
	}

	fseek(file, 0, SEEK_END);	//Move cursor to end of file
	size_t size = ftell(file);		//Get the total size of the file
	fseek(file, 0, SEEK_SET);	//Move cursor to beginning of file

	data.resize(size);
	//*data = static_cast<char*>(malloc(*size + 1));	//Allocate memory to hold file

	int result = fread(data.data(), sizeof(char), size, file);
	if (result != size) {
		size = 0;
		fprintf(stderr, "Failed to read file: %s\n", filename.c_str());
		return IOResult::IO_FAILED;
	}

	//data[size] = 0; //Add a null-terminator for strings

	if (fclose(file) == EOF) {
		fprintf(stderr, "Failed to close file: %s\n", filename.c_str()); //Close the file
		return IOResult::IO_FAILED;
	}
	return IOResult::IO_SUCCESS;
}

void image_info::free() {
	stbi_image_free(data);
}

//Simple STBI abstraction function
IOResult loadImage(const std::string& filename, image_info& image) {
	//TODO: Ideally images should be loaded directly into a mapped memory address to skip a memcpy
	image.data = stbi_load(filename.c_str(), &image.width, &image.height, &image.channels, STBI_rgb_alpha);
	if (!image.data) {
		fprintf(stderr, "Failed to load image!\n");
		return IOResult::IO_FAILED;
	}
	return IOResult::IO_SUCCESS;
}

void freeImage(unsigned char* data) {
	stbi_image_free(data);
}

//Assimp wrapper function with processing
//Loads an entire scene into the VertexBufferCreateInfo pointer
//Ideally this can be done more efficiently
IOResult loadModel(const std::string& filename, uint32_t* numMeshes, mesh_info** meshInfo) {
	//aiScene scene = aiImportFile();

	const aiScene* scene = aiImportFile(filename.c_str(),
		//aiProcess_Triangulate | 
		//aiProcess_JoinIdenticalVertices | 
		//aiProcess_SortByPType
		NULL
	);

	if (!scene) {
		printf("Failed to load scene!\n");
		printf(aiGetErrorString());
		return IOResult::IO_FAILED;
	}

	*numMeshes = scene->mNumMeshes;
	printf("Num Meshes %d\n", *numMeshes);

	mesh_info* info = (*meshInfo) = static_cast<mesh_info*>(malloc(*numMeshes * sizeof(mesh_info)));

	for (int i = 0; i < *numMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[i];
		//VertexBufferCreateInfo createInfo = createInfos[i];

		int numVertices = mesh->mNumVertices;
		int numIndices = mesh->mNumFaces;
		
		uint32_t indicesCount = numIndices * 3;
		uint32_t attributeCount = 3;

		uint32_t verticesCount = numVertices;
		info->indicesCount = indicesCount;
		info->attributeCount = attributeCount;

		uint32_t pAttributeSizes[] = { sizeof(float) * 3, sizeof(float) * 3, sizeof(float) * 2 };

		uint32_t vertexSize = sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 2;

		printf("Num Vertices: %d\n", numVertices);

		uint32_t indicesSize = indicesCount * sizeof(uint32_t);

		//Set half of the variables in vertexBuffer, buffer and memory will be set later
		uint64_t* pOffsets = info->pOffsets = static_cast<uint64_t*>(malloc(attributeCount * sizeof(uint64_t)));

		uint32_t verticesSize = 0;
		//Calculate the total size of all vertices as well as the offsets of each attribute
		for (int i = 0; i < attributeCount; ++i) {
			pOffsets[i] = indicesSize + verticesSize;
			verticesSize += pAttributeSizes[i] * verticesCount;
		}
		uint32_t size = indicesSize + verticesSize;
		info->size = size;

		info->data = malloc(size);

		uint32_t* indices = static_cast<uint32_t*>(info->data);
		for (int j = 0; j < numIndices; j++) {
			unsigned int* face = mesh->mFaces[j].mIndices;
			indices[j * 3 + 0] = face[0];
			indices[j * 3 + 1] = face[1];
			indices[j * 3 + 2] = face[2];
			//printf("%d, %d, %d\n", face[0], face[1], face[2]);
		}

		float* vertices = (float*)info->data + indicesCount * sizeof(uint32_t);

		int normalOffset = numVertices * 3;
		int texCoordOffset = normalOffset + numVertices * 3;

		for (int j = 0; j < numVertices; j++) {
			aiVector3D vertex = mesh->mVertices[j];
			aiVector3D normal = mesh->mNormals[j];
			//struct aiVector3D texCoord = mesh->mTextureCoords[0][j];

			vertices[j * 3 + 0] = vertex.x;
			vertices[j * 3 + 1] = vertex.y;
			vertices[j * 3 + 2] = vertex.z;

			vertices[j * 3 + 0 + normalOffset] = normal.x;
			vertices[j * 3 + 1 + normalOffset] = normal.y;
			vertices[j * 3 + 2 + normalOffset] = normal.z;

			vertices[j * 2 + 0 + texCoordOffset] = 0;// texCoord.x;
			vertices[j * 2 + 1 + texCoordOffset] = 0;// texCoord.y;
		}

		++info;
	}

	printf("Scene loaded: %s\n", filename);
	aiReleaseImport(scene);
	return IOResult::IO_SUCCESS;
}
