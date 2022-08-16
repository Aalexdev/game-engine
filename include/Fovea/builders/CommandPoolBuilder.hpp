#pragma once

#include "PhysicalDeviceBuilder.hpp"
#include "../../vulkan/vulkan.h"

namespace Fovea{
	enum CommandPoolFlags{
		FLAG_TRANSIENT = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
		FLAG_RESET_BUFFER = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		FLAG_PROTECTED = VK_COMMAND_POOL_CREATE_PROTECTED_BIT
	};

	class CommandPoolBuilder{
		friend class CommandPool;
		public:
			void setFamily(PhysicalDeviceFamily family);
			void setFlag(int flags);

		private:
			PhysicalDeviceFamily family;
			int flags;
	};
}