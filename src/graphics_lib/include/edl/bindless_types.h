#pragma once

#include <cstdint>

struct DrawData {
	uint32_t positionOffset;
	uint32_t normalOffset;
	uint32_t texCoord0Offset;
	uint32_t texCoord1Offset;

	uint32_t materialOffset;
	uint32_t transformOffset;

	uint32_t padding0;
	uint32_t padding1;
};