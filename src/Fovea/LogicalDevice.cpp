#include "Fovea/LogicalDevice.hpp"
#include "Fovea/core.hpp"

#include <stdexcept>

namespace Fovea{
	void LogicalDevice::initialize(LogicalDeviceBuilder &builder){
		std::vector<PhysicalDevice::FamilyDetails> families = getInstance().physicalDevice.getFamilies();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::vector<PhysicalDevice::FamilyDetails> uniqueFamilies;

		for (const auto &family : families){
			bool isUnique = true;

			for (const auto &uniqueFamilie : uniqueFamilies){
				if (uniqueFamilie.family == family.family){
					isUnique = false;
					break;
				}
			}

			if (isUnique) uniqueFamilies.push_back(family);
		}

		std::vector<float> priorities(uniqueFamilies.size());

		for (int i=0; i<uniqueFamilies.size(); i++){
			auto& queueFamily = uniqueFamilies[i];

			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily.family;
			auto& priority = priorities[i];

			queueCreateInfo.queueCount = 1;
			queueCreateInfo.queueFamilyIndex = queueFamily.family;
			queueCreateInfo.pQueuePriorities = &priority;
			
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures features = getInstance().physicalDevice.getEnabledFeatures();

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		
		createInfo.pEnabledFeatures = &features;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(builder.requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = builder.requiredExtensions.data();

		if (getInstance().instance.isValidationLayersEnabled()){
			createInfo.enabledLayerCount = static_cast<uint32_t>(getInstance().instance.getValidationLayers().size());
			createInfo.ppEnabledLayerNames = getInstance().instance.getValidationLayers().data();
		} else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(getInstance().physicalDevice.getDevice(), &createInfo, nullptr, &device) != VK_SUCCESS){
			throw std::runtime_error("failed to create the logical device");
		}

		queues.fill(VK_NULL_HANDLE);
		for (int i=0; i<uniqueFamilies.size(); i++){
			auto &family = uniqueFamilies[i];
			auto &queue = queues[static_cast<size_t>(family.type)];

			vkGetDeviceQueue(device, family.family, 0, &queues[static_cast<size_t>(family.type)]);

			for (auto &f : families){
				if (f.type == family.type) continue;
				if (f.family == family.family){
					queues[static_cast<size_t>(f.type)] = queue;
				}
			}
		}
	}

	LogicalDevice::~LogicalDevice(){
		vkDestroyDevice(device, nullptr);
	}

	VkDevice LogicalDevice::getDevice() const{
		return device;
	}

	void LogicalDevice::createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory){
		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = getInstance().physicalDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		if (vkBindImageMemory(device, image, imageMemory, 0) != VK_SUCCESS) {
			throw std::runtime_error("failed to bind image memory!");
		}
	}

	void LogicalDevice::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,VkDeviceMemory &bufferMemory){
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = getInstance().physicalDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	VkQueue LogicalDevice::getQueue(PhysicalDeviceFamily family){
		return queues[static_cast<size_t>(family)];
	}
}