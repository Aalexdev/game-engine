#pragma once

#include "builders/CommandPoolBuilder.hpp"
#include "../vulkan/vulkan.h"

namespace Fovea{
	class CommandPool{
		public:
			~CommandPool();

			void initialize(CommandPoolBuilder &builder);

			VkCommandPool getCommandPool();

		private:
			VkCommandPool commandPool;
			PhysicalDeviceFamily family;
			int flags;
	};
}