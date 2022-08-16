#include "Fovea/Instance.hpp"

#include <string.h>
#include <stdexcept>
#include <unordered_set>

#include <SDL2/SDL_vulkan.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT){
		fprintf(stderr, "%s\n", pCallbackData->pMessage);
		
		return VK_FALSE;
	}

	return VK_FALSE;
}

static inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator){

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

namespace Fovea{
	Instance::~Instance(){
		if (validationLayerEnabled) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

		for (auto &layer : validationLayers){
			delete layer;
		}

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);
	}

	void Instance::initialize(InstanceBuilder &builder, void* window){
		validationLayerEnabled = builder.validationLayersEnabled;
		if (validationLayerEnabled){
			checkValidationLayers(builder.validationLayers);
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = builder.applicationName.c_str();
		appInfo.applicationVersion = builder.applicationVersion;
		appInfo.pEngineName = builder.engineName.c_str();
		appInfo.engineVersion = builder.engineVersion;
		appInfo.apiVersion = builder.apiVersion;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions(window);
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (validationLayerEnabled){
			createInfo.enabledLayerCount = static_cast<uint32_t>(builder.validationLayers.size());
			createInfo.ppEnabledLayerNames = builder.validationLayers.data();

			setDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		} else {
			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
			throw std::runtime_error("failed to create the vulkan instance");
		}

		checkRequiredExtensions(window);
		setupDebugMessenger();

		if (SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(window), instance, &surface) == SDL_FALSE){
			throw std::runtime_error("failed to create the window surface : " + std::string(SDL_GetError()));
		}

		validationLayers.resize(builder.validationLayers.size());
		for (int i=0; i<static_cast<int>(validationLayers.size()); i++){
			auto &l = validationLayers[i];
			l = new char[strlen(builder.validationLayers[i])];

			strcpy(l, builder.validationLayers[i]);
		}
	}

	void Instance::checkValidationLayers(std::vector<const char *> &validationLayers){
		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		std::vector<VkLayerProperties> availableLayers(count);
		vkEnumerateInstanceLayerProperties(&count, availableLayers.data());

		for (const auto &layerName : validationLayers){
			bool layerFound = false;

			for (const auto &layerProperties : availableLayers){
				if (strcmp(layerName, layerProperties.layerName) == 0){
					layerFound = true;
					break;
				}
			}

			if (!layerFound){
				throw std::runtime_error("cannot found \"" + std::string(layerName) + "\" validation layer");
			}
		}
	}

	std::vector<const char*> Instance::getRequiredExtensions(void* window){
		SDL_Window* win = static_cast<SDL_Window*>(window);

		unsigned int count = 0;
		SDL_Vulkan_GetInstanceExtensions(win, &count, nullptr);

		std::vector<const char*> extensions(count);
		SDL_Vulkan_GetInstanceExtensions(win, &count, extensions.data());

		if (validationLayerEnabled){
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void Instance::setDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo){
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
	}

	void Instance::checkRequiredExtensions(void *window){
		uint32_t extensionsCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionsCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensions.data());

		std::unordered_set<std::string> available;
		for (const auto &extension : extensions){
			available.insert(extension.extensionName);
		}

		auto requiredExtensions = getRequiredExtensions(window);

		for (const auto &required : requiredExtensions){
			if (available.find(required) == available.end()){
				throw std::runtime_error("missing required SDL2 vulkan extensions");
			}
		}
	}

	void Instance::setupDebugMessenger(){
		if (!validationLayerEnabled) return;
		
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		setDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS){
			throw std::runtime_error("failed to create the debug messenger");
		}
	}

	bool Instance::isValidationLayersEnabled() const{
		return validationLayerEnabled;
	}

	VkInstance Instance::getInstance() const{
		return instance;
	}

	VkSurfaceKHR Instance::getSurface() const{
		return surface;
	}

	std::vector<char*>& Instance::getValidationLayers(){
		return validationLayers;
	}
}