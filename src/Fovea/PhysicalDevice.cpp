#include "Fovea/PhysicalDevice.hpp"
#include "Fovea/core.hpp"

#include <stdexcept>
#include <set>

namespace Fovea{
	void PhysicalDevice::initialize(PhysicalDeviceBuidler &builder){
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(getInstance().instance.getInstance(), &deviceCount, nullptr);

		if (deviceCount == 0){
			throw std::runtime_error("cannot found a GPU with a vulkan support");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(getInstance().instance.getInstance(), &deviceCount, devices.data());

		for (auto &device : devices){
			if (isSuitableDevice(device, builder)){
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE){
			throw std::runtime_error("failed to found a suitable GPU");
		}
		
		features = builder.getFeatures();
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	}

	PhysicalDevice::~PhysicalDevice(){
	}

	void PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device, PhysicalDeviceBuidler &builder){

	}

	bool PhysicalDevice::isSuitableDevice(VkPhysicalDevice device, PhysicalDeviceBuidler &builder){
		auto families = getFamilies(device, builder);
		if (!checkDeviceExtensions(device, builder)) return false;

		swapChainSupport = getSwapChainSupport(device);
		bool swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return swapChainAdequate && checkFeatures(supportedFeatures, builder);
	}

	std::array<uint32_t, static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COUNT)> PhysicalDevice::getFamilies(VkPhysicalDevice physicalDevice, PhysicalDeviceBuidler &builder){
		std::bitset<static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COUNT)> availableFamilies;
		std::array<uint32_t, static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COUNT)> families;

		// query availables queues
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		uint32_t i=0;
		for (const auto &queueFamily : queueFamilies){
			if (queueFamily.queueCount == 0) continue;

			// graphic queue
			if (builder.requiredFamilies[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_GRAPHIC)] && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
				families[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_GRAPHIC)] = i;
				availableFamilies.set(static_cast<size_t>(PhysicalDeviceFamily::FAMILY_GRAPHIC));
				this->families.push_back({i, PhysicalDeviceFamily::FAMILY_GRAPHIC});
			}

			// compute queue
			if (builder.requiredFamilies[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COMPUTE)] && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT){
				families[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COMPUTE)] = i;
				availableFamilies.set(static_cast<size_t>(PhysicalDeviceFamily::FAMILY_COMPUTE));
				this->families.push_back({i, PhysicalDeviceFamily::FAMILY_COMPUTE});
			}

			// transfer queue
			if (builder.requiredFamilies[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_TRANSFER)] && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT){
				families[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_TRANSFER)] = i;
				availableFamilies.set(static_cast<size_t>(PhysicalDeviceFamily::FAMILY_TRANSFER));
				this->families.push_back({i, PhysicalDeviceFamily::FAMILY_TRANSFER});
			}

			// protected queue
			if (builder.requiredFamilies[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_PROTECTED)] && queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT){
				families[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_PROTECTED)] = i;
				availableFamilies.set(static_cast<size_t>(PhysicalDeviceFamily::FAMILY_PROTECTED));
				this->families.push_back({i, PhysicalDeviceFamily::FAMILY_PROTECTED});
			}

			// spars queue
			if (builder.requiredFamilies[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_SPARSE_BINDING)] && queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT){
				families[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_SPARSE_BINDING)] = i;
				availableFamilies.set(static_cast<size_t>(PhysicalDeviceFamily::FAMILY_SPARSE_BINDING));
				this->families.push_back({i, PhysicalDeviceFamily::FAMILY_SPARSE_BINDING});
			}

			// present
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, getInstance().instance.getSurface(), &presentSupport);
			if (builder.requiredFamilies[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_PRESENT)] && presentSupport){
				families[static_cast<size_t>(PhysicalDeviceFamily::FAMILY_PRESENT)] = i;
				availableFamilies.set(static_cast<size_t>(PhysicalDeviceFamily::FAMILY_PRESENT));
				this->families.push_back({i, PhysicalDeviceFamily::FAMILY_PRESENT});
			}

			if (availableFamilies == builder.requiredFamilies) break;
			i++;
		}


		if (availableFamilies != builder.requiredFamilies)
			throw std::runtime_error("failed to find the wanted queues");
		
		return families;
	}

	bool PhysicalDevice::checkDeviceExtensions(VkPhysicalDevice device, PhysicalDeviceBuidler &builder){
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(builder.requiredExtensions.begin(), builder.requiredExtensions.end());

		for (const auto &extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);
		
		return requiredExtensions.empty();
	}

	PhysicalDevice::SwapChainSupport PhysicalDevice::getSwapChainSupport(){
		return getSwapChainSupport(physicalDevice);
	}


	PhysicalDevice::SwapChainSupport PhysicalDevice::getSwapChainSupport(VkPhysicalDevice device){
		SwapChainSupport details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, getInstance().instance.getSurface(), &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, getInstance().instance.getSurface(), &formatCount, nullptr);

		if (formatCount != 0){
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, getInstance().instance.getSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, getInstance().instance.getSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0){
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, getInstance().instance.getSurface(), &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	bool PhysicalDevice::checkFeatures(VkPhysicalDeviceFeatures features, PhysicalDeviceBuidler &builder){
		VkPhysicalDeviceFeatures requiredFeatures = builder.getFeatures();
		VkBool32* requiredArr = reinterpret_cast<VkBool32*>(&requiredFeatures);
		VkBool32* arr = reinterpret_cast<VkBool32*>(&features);

		for (int i=0; i<static_cast<int>(PhysicalDeviceFeature::FEATURES_COUNT); i++){
			if (requiredArr[i] && !arr[i]){
				return false;
			}
		}	
		return true;	
	}

	PhysicalDevice::FamilyDetails PhysicalDevice::getFamily(PhysicalDeviceFamily family) const{
		for (auto &f : families){
			if (f.type == family) return f;
		}
		throw std::runtime_error("failed to found the given family");
	}

	VkFormat PhysicalDevice::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			} else if (
					tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}
		throw std::runtime_error("failed to find supported format!");
	}

	uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type");
	}

	VkPhysicalDeviceProperties PhysicalDevice::getProperties(){
		return properties;
	}

	VkPhysicalDeviceFeatures PhysicalDevice::getEnabledFeatures(){
		return features;
	}

	VkPhysicalDevice PhysicalDevice::getDevice(){
		return physicalDevice;
	}
}