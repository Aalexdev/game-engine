#include "Fovea/builders/InstanceBuilder.hpp"

namespace Fovea{
	void InstanceBuilder::setApplicationName(const char* name){
		applicationName = name;
	}

	void InstanceBuilder::setEngineName(const char* name){
		engineName = name;
	}

	void InstanceBuilder::setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch){
		applicationVersion = VK_MAKE_VERSION(major, minor, patch);
	}

	void InstanceBuilder::setEngineVersion(uint16_t major, uint16_t minor, uint16_t patch){
		engineVersion = VK_MAKE_VERSION(major, minor, patch);
	}

	void InstanceBuilder::setVulkanVersion(VulkanVersion&& version){
		switch (version){
			case VULKAN_1_0: apiVersion = VK_API_VERSION_1_0; break;
			case VULKAN_1_1: apiVersion = VK_API_VERSION_1_1; break;
			case VULKAN_1_2: apiVersion = VK_API_VERSION_1_2; break;
		}
	}

	void InstanceBuilder::requireExtension(const char* extension){
		requiredExtensions.push_back(extension);
	}

	void InstanceBuilder::requireValidationLayer(const char* layer){
		validationLayers.push_back(layer);
	}

	void InstanceBuilder::enableValidationLayers(bool enabled){
		validationLayersEnabled = enabled;
	}
}