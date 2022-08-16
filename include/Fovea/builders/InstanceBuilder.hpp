#pragma once

#include "../../vulkan/vulkan.h"
#include <string>
#include <vector>

namespace Fovea{
	class InstanceBuilder{
		friend class Instance;
		public:
			enum VulkanVersion{
				VULKAN_1_0,
				VULKAN_1_1,
				VULKAN_1_2
			};

			void setApplicationName(const char* name);
			void setEngineName(const char* name);
			void setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch);
			void setEngineVersion(uint16_t major, uint16_t minor, uint16_t patch);
			void setVulkanVersion(VulkanVersion&& version);
			void requireExtension(const char* extension);
			void requireValidationLayer(const char* layer);
			void enableValidationLayers(bool enabled);
		
		private:
			std::string applicationName = "NoNameApp";
			std::string engineName = "FoveaEngine";
			uint32_t applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);
			uint32_t apiVersion = VK_API_VERSION_1_1;
			std::vector<std::string> requiredExtensions;
			std::vector<const char*> validationLayers;
			bool validationLayersEnabled = false;
	};
}