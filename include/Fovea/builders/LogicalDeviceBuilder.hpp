#pragma once

#include <vector>
#include <array>
#include <bitset>

#include "PhysicalDeviceBuilder.hpp"

namespace Fovea{
	class LogicalDeviceBuilder{
		friend class LogicalDevice;
		public:
			void requireExtension(const char *extension);
			void requireQueue(PhysicalDeviceFamily family);
			void setQueuePriority(PhysicalDeviceFamily family, float priority);

		private:
			std::bitset<static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COUNT)> requiredQueues;
			std::array<float, static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COUNT)> queuePriorities;
			std::vector<const char*> requiredExtensions;
	};
}