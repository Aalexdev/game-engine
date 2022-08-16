#pragma once

#include "builders/InstanceBuilder.hpp"
#include "../vulkan/vulkan.h"

namespace Fovea{
	class Instance{
		public:
			~Instance();

			void initialize(InstanceBuilder &builder, void* window);

			bool isValidationLayersEnabled() const;
			VkInstance getInstance() const;
			VkSurfaceKHR getSurface() const;

			std::vector<char*>& getValidationLayers();
		
		private:
			void checkValidationLayers(std::vector<const char *> &validationLayers);
			std::vector<const char*> getRequiredExtensions(void *window);
			void setDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
			void setupDebugMessenger();
			void checkRequiredExtensions(void *window);

			VkInstance instance;
			VkDebugUtilsMessengerEXT debugMessenger;
			VkSurfaceKHR surface;

			std::vector<char*> validationLayers;

			bool validationLayerEnabled;

	};
}