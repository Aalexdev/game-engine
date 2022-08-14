#pragma once

#include "../../vulkan/vulkan.h"

#include <vector>

namespace Fovea{
	class DescriptorPoolBuilder{
		friend class DescriptorPool;
		public:
			void setMaxSets(uint32_t count);
			void setPoolFlags(VkDescriptorPoolCreateFlags flags);
			void addPoolSize(VkDescriptorType type, uint32_t count);

		private:
			std::vector<VkDescriptorPoolSize> poolSizes = {};
			uint32_t maxSets;
			VkDescriptorPoolCreateFlags poolFlags = 0;
			
	};
}