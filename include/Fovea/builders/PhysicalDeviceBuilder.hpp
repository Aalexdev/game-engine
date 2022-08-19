#pragma once

#include "../../vulkan/vulkan.h"
#include "../Instance.hpp"

#include <bitset>
#include <vector>

namespace Fovea{

	enum class PhysicalDeviceFeature{
		ROBUST_BUFFER_ACCES = 0,
		FULL_DRAW_INDEX_UINT_32 = 1,
		IMAGE_CUBE_ARRAY = 2,
		INDEPENDENT_BLEND = 3,
		GEOMETRY_SHADER = 4,
		TESSELLATION_SHADER = 5,
		SAMPLE_RATE_SHADING = 6,
		DUAL_SRC_BLEND = 7,
		LOGIC_OP = 8,
		MULTI_DRAW_INDIRECT = 9,
		DRAW_INDIRECT_FISRT_INSTANCE = 10,
		DEPTH_CLAMP = 11,
		DEPTH_BIAS_CLAMP = 12,
		FILL_MODE_NON_SOLID = 13,
		DEPTH_BOUNDS = 14,
		WIDE_LINES = 15,
		LARGE_POINTS = 16,
		ALPHA_TO_ONE = 17,
		MULTI_VIEWPORT = 18,
		SAMPLER_ANISOTROPY = 19,
		TEXTURE_COMPRESSION_ETC2 = 20,
		TEXTURE_COMPRESSION_ASTC_LDR = 21,
		TEXTURE_COMPRESSION_BC = 22,
		OCCLUSION_QUERY_PRECISE = 23,
		PIPELINE_STATISTICS_QUERY = 24,
		VERTEX_PIPELINE_STORES_AND_ATOMICS = 25,
		FRAGMENT_STORES_AND_ATOMICS = 26,
		SHADER_TESSELLATION_AND_GEOMETRY_POINTS = 27,
		SHADER_IMAGE_GATHER_EXTENDED = 28,
		SHADER_STORAGE_IMAGE_EXTENDED_FORMAT = 29,
		SHADER_STORAGE_IMAGE_MULTISAMPLE = 30,
		SHADER_STORAGE_IMAGE_READ_WITHOUT_ANY_FORMAT = 31,
		SHADER_STORAGE_IMAGE_WRITE_WITHOUT_ANY_FORMAT = 32,
		SHADER_UNIFORM_BUFFER_ARRAY_DYNAMIC_INDEXING = 33,
		SHADER_SAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING = 34,
		SHADER_STORAGE_BUFFER_ARRAY_DUNAMIC_INDEXING = 35,
		SHADER_STORAGE_IMAGE_ARRAY_DYNAMIC_INDEXING = 36,
		SHADER_CLIP_DISTANCE = 37,
		SHADER_CULL_DISTANCE = 38,
		SHADER_FLOAT_64 = 39,
		SHADER_INT_64 = 40,
		SHADER_INT_16 = 41,
		SHADER_RESOURCE_RESIDENCY = 42,
		SHADER_RESOURCE_MIN_LOD = 43,
		SPARSE_BINDING = 44,
		SPARSE_RESIDENCY_BUFFER = 45,
		SPARSE_RESIDENCY_IMAGE_2D = 46,
		SPARSE_RESIDENCY_IMAGE_3D = 47,
		SPARSE_RESIDENCY_2_SAMPLES = 48,
		SPARSE_RESIDENCY_4_SAMPLES = 49,
		SPARSE_RESIDENCY_8_SAMPLED = 50,
		SPARSE_RESIDENCY_16_SAMPLES = 51,
		SPARSE_RESODENCY_ALIASED = 52,
		VARIABLE_MUTISAMPLE_RATE = 53,
		INHERIED_QUERIES = 54,
		FEATURES_COUNT,
		NONE = -1
	};

	enum class PhysicalDeviceFamily{
		FAMILY_GRAPHIC = 0,
		FAMILY_PRESENT = 1,
		FAMILY_COMPUTE = 2,
		FAMILY_TRANSFER = 3,
		FAMILY_PROTECTED = 4,
		FAMILY_SPARSE_BINDING = 5,
		FAMILY_COUNT,
		FAMILY_NONE = -1
	};

	class PhysicalDeviceBuidler{
		friend class PhysicalDevice;
		public:
			void requireExtension(const char* ext);
			void requireFamily(PhysicalDeviceFamily family);
			void requireFeature(PhysicalDeviceFeature feature);
			
		private:
			VkPhysicalDeviceFeatures getFeatures();

			std::vector<std::string> requiredExtensions;
			std::bitset<static_cast<size_t>(PhysicalDeviceFeature::FEATURES_COUNT)> requiredFeatures;
			std::bitset<static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COUNT)> requiredFamilies; 

	};
}