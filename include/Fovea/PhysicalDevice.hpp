#pragma once

#include "builders/PhysicalDeviceBuilder.hpp"
#include "../vulkan/vulkan.h"
#include <array>

namespace Fovea{
	class PhysicalDevice{
		public:
			struct SwapChainSupport{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			};

			struct FamilyDetails{
				uint32_t family = 0;
				PhysicalDeviceFamily type = PhysicalDeviceFamily::FAMILY_NONE;
			};

			~PhysicalDevice();

			void initialize(PhysicalDeviceBuidler &builder);

			FamilyDetails getFamily(PhysicalDeviceFamily family) const;

			VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

			std::vector<FamilyDetails> getFamilies() const {return families;}

			VkPhysicalDeviceProperties getProperties(); 
			VkPhysicalDeviceFeatures getEnabledFeatures();

			VkPhysicalDevice getDevice();

			SwapChainSupport getSwapChainSupport();


		private:
			void checkDeviceExtensionSupport(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			bool isSuitableDevice(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			std::array<uint32_t, static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COUNT)> getFamilies(VkPhysicalDevice physicalDevice, PhysicalDeviceBuidler &builder);
			bool checkDeviceExtensions(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			SwapChainSupport getSwapChainSupport(VkPhysicalDevice device);
			bool checkFeatures(VkPhysicalDeviceFeatures features, PhysicalDeviceBuidler &builder);

			std::vector<FamilyDetails> families;
			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
			VkPhysicalDeviceProperties properties;
			VkPhysicalDeviceFeatures features;
			SwapChainSupport swapChainSupport;
			
	};
}